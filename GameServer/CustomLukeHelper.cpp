// CustomRewardMaster.cpp: implementation of the CCustomRewardMaster class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomLukeHelper.h"
#include "ItemBagManager.h"
#include "Map.h"
#include "MapManager.h"
#include "MemScript.h"
#include "Message.h"
#include "Monster.h"
#include "Notice.h"
#include "RandomManager.h"
#include "ScheduleManager.h"
#include "ServerInfo.h"
#include "Util.h"

CCustomLukeHelper gCustomLukeHelper;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomLukeHelper::CCustomLukeHelper() // OK
{
	this->m_State = CUSTOM_LUKE_HELPER_BLANK;
	this->m_RemainTime = 0;
	this->m_TargetTime = 0;
	this->m_TickCount = GetTickCount();
	this->m_LukeHelperIndex = -1;
	this->m_MinutesLeft = -1;
	this->m_RespawnIndex = -1;

	this->m_CustomLukeHelperRespawn.clear();
	this->m_CustomLukeHelperStartTime.clear();
}

CCustomLukeHelper::~CCustomLukeHelper() // OK
{

}

void CCustomLukeHelper::Init() // OK
{
	if(this->m_CustomLukeHelperSwitch == 0)
	{
		this->SetState(CUSTOM_LUKE_HELPER_BLANK);
	}
	else
	{
		this->SetState(CUSTOM_LUKE_HELPER_EMPTY);
	}
}

void CCustomLukeHelper::ReadCustomLukeHelperInfo(char* section,char* path) // OK
{
	this->m_CustomLukeHelperSwitch = GetPrivateProfileInt(section,"CustomLukeHelperSwitch",0,path);
}

void CCustomLukeHelper::Load(char* path) // OK
{
	CMemScript* lpMemScript = new CMemScript;

	if(lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR,path);
		return;
	}

	if(lpMemScript->SetBuffer(path) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	this->m_CustomLukeHelperRespawn.clear();

	this->m_CustomLukeHelperStartTime.clear();

	try
	{
		while (true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			int section = lpMemScript->GetNumber();

			while (true)
			{
				if(section == 0)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					this->m_EventTime = lpMemScript->GetNumber();
				}
				else if(section == 1)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					CUSTOM_LUKE_HELPER_RESPAWN info;

					info.Index = this->m_CustomLukeHelperRespawn.size();

					info.Map = lpMemScript->GetNumber();

					info.Class = lpMemScript->GetAsNumber();

					info.StartX = lpMemScript->GetAsNumber();

					info.StartY = lpMemScript->GetAsNumber();

					info.EndX = lpMemScript->GetAsNumber();

					info.EndY = lpMemScript->GetAsNumber();

					info.BagSpecial = lpMemScript->GetAsNumber();

					this->m_CustomLukeHelperRespawn.insert(std::pair<int,CUSTOM_LUKE_HELPER_RESPAWN>(info.Index,info));
				}
				else if(section == 2)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					CUSTOM_LUKE_HELPER_START_TIME info;

					info.Year = lpMemScript->GetNumber();

					info.Month = lpMemScript->GetAsNumber();

					info.Day = lpMemScript->GetAsNumber();

					info.DayOfWeek = lpMemScript->GetAsNumber();

					info.Hour = lpMemScript->GetAsNumber();

					info.Minute = lpMemScript->GetAsNumber();

					info.Second = lpMemScript->GetAsNumber();

					this->m_CustomLukeHelperStartTime.push_back(info);
				}
				else
				{
					break;
				}
			}
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

CUSTOM_LUKE_HELPER_RESPAWN* CCustomLukeHelper::GetInfo(int index) // OK
{
	std::map<int,CUSTOM_LUKE_HELPER_RESPAWN>::iterator ir = this->m_CustomLukeHelperRespawn.find(index);

	if(ir != this->m_CustomLukeHelperRespawn.end())
	{
		return &ir->second;

	}

	return 0;
}

void CCustomLukeHelper::MainProc() // OK
{
	if((GetTickCount()-this->m_TickCount) >= 1000)
	{
		this->m_TickCount = GetTickCount();

		this->m_RemainTime = (int)difftime(this->m_TargetTime,time(0));

		switch(this->m_State)
		{
			case CUSTOM_LUKE_HELPER_BLANK:
				this->ProcState_BLANK();
				break;
			case CUSTOM_LUKE_HELPER_EMPTY:
				this->ProcState_EMPTY();
				break;
			case CUSTOM_LUKE_HELPER_START:
				this->ProcState_START();
				break;
		}
	}
}

void CCustomLukeHelper::ProcState_BLANK() // OK
{

}

void CCustomLukeHelper::ProcState_EMPTY() // OK
{
	if(this->m_RemainTime > 0 && this->m_RemainTime < 300)
	{
		int minutes = this->m_RemainTime/60;

		if((this->m_RemainTime % 60) == 0)
		{
			minutes--;
		}

		if(this->m_MinutesLeft != minutes)
		{
			this->m_MinutesLeft = minutes;

			gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(530),(this->m_MinutesLeft+1));
		}

	}

	if(this->m_RemainTime <= 0)
	{
		CRandomManager RandomManager;

		for(std::map<int,CUSTOM_LUKE_HELPER_RESPAWN>::iterator it = this->m_CustomLukeHelperRespawn.begin(); it != this->m_CustomLukeHelperRespawn.end(); it++)
		{
			RandomManager.AddElement(it->second.Index,1);
		}

		RandomManager.GetRandomElement(&this->m_RespawnIndex);

		CUSTOM_LUKE_HELPER_RESPAWN* lpInfo = this->GetInfo(this->m_RespawnIndex);

		if(lpInfo == 0)
		{
			this->SetState(CUSTOM_LUKE_HELPER_EMPTY);
			return;
		}

		gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(531),gMapManager.GetMapName(lpInfo->Map));
		gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(532));

		this->SetState(CUSTOM_LUKE_HELPER_START);
	}
}

void CCustomLukeHelper::ProcState_START() // OK
{
	if(this->m_RemainTime <= 0)
	{
		gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(533));

		this->SetState(CUSTOM_LUKE_HELPER_EMPTY);
	}
}

void CCustomLukeHelper::SetState(int state) // OK
{
	this->m_State = state;

	switch (this->m_State)
	{
	case CUSTOM_LUKE_HELPER_BLANK:
		this->SetState_BLANK();
		break;
	case CUSTOM_LUKE_HELPER_EMPTY:
		this->SetState_EMPTY();
		break;
	case CUSTOM_LUKE_HELPER_START:
		this->SetState_START();
		break;
	}
}

void CCustomLukeHelper::SetState_BLANK() // OK
{

}

void CCustomLukeHelper::SetState_EMPTY() // OK
{
	if(OBJECT_RANGE(this->m_LukeHelperIndex) != 0)
	{
		gObjDel(this->m_LukeHelperIndex);
		this->m_LukeHelperIndex = -1;
		this->m_MinutesLeft = -1;
	}

	this->CheckSync();
}

void CCustomLukeHelper::SetState_START() // OK
{
	this->SetMonster();

	this->m_RemainTime = this->m_EventTime*60;

	this->m_TargetTime = (int)(time(0)+this->m_RemainTime);
}

void CCustomLukeHelper::CheckSync() // OK
{
	if(this->m_CustomLukeHelperStartTime.empty() != 0)
	{
		this->SetState(CUSTOM_LUKE_HELPER_BLANK);
		return;
	}

	CTime ScheduleTime;

	CScheduleManager ScheduleManager;

	for(std::vector<CUSTOM_LUKE_HELPER_START_TIME>::iterator it = this->m_CustomLukeHelperStartTime.begin(); it != this->m_CustomLukeHelperStartTime.end(); it++)
	{
		ScheduleManager.AddSchedule(it->Year,it->Month,it->Day,it->Hour,it->Minute,it->Second,it->DayOfWeek);
	}

	if(ScheduleManager.GetSchedule(&ScheduleTime) == 0)
	{
		this->SetState(CUSTOM_LUKE_HELPER_BLANK);
		return;
	}

	this->m_RemainTime = (int)difftime(ScheduleTime.GetTime(),time(0));

	this->m_TargetTime = (int)ScheduleTime.GetTime();
}

void CCustomLukeHelper::SetMonster() // OK
{
	if(OBJECT_RANGE(this->m_LukeHelperIndex) != 0)
	{
		return;
	}

	CUSTOM_LUKE_HELPER_RESPAWN* lpInfo = this->GetInfo(this->m_RespawnIndex);

	if(lpInfo == 0)
	{
		this->SetState(CUSTOM_LUKE_HELPER_EMPTY);
		return;
	}

	int index = gObjAddMonster(lpInfo->Map);

	if(OBJECT_RANGE(index) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[index];

	int px = lpInfo->StartX;
	int py = lpInfo->StartY;

	if(gObjGetRandomFreeLocation(lpInfo->Map,&px,&py,(lpInfo->EndX - lpInfo->StartX),(lpInfo->EndY - lpInfo->StartY),10) == 0)
	{
		px = lpInfo->StartX;
		py = lpInfo->StartY;
	}

	lpObj->PosNum = -1;
	lpObj->X = px;
	lpObj->Y = py;
	lpObj->TX = px;
	lpObj->TY = py;
	lpObj->OldX = px;
	lpObj->OldY = py;
	lpObj->StartX = px;
	lpObj->StartY = py;
	lpObj->Dir = GetLargeRand() % 8;
	lpObj->Map = lpInfo->Map;

	if(gObjSetMonster(index,lpInfo->Class) == 0)
	{
		gObjDel(index);
		return;
	}

	this->m_LukeHelperIndex = index;

	LogAdd(LOG_BLACK,"[Luke Helper] SetMonster (%s)(%d)(%d)",gMapManager.GetMapName(lpObj->Map),lpObj->X,lpObj->Y);
}

bool CCustomLukeHelper::NpcLukeHelper(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	if(this->m_State != CUSTOM_LUKE_HELPER_START)
	{
		return 0;
	}

	if(lpNpc->Index != this->m_LukeHelperIndex)
	{
		return 0;
	}

	for(int n=0;n < 10;n++)
	{
		GCServerCommandSend(lpObj->Index, 0, (lpObj->X + (((GetLargeRand() % 5) * 2) - 4)), (lpObj->Y + (((GetLargeRand() % 5) * 2) - 4)), 1);
	}

	GCChatTargetSend(lpObj,lpNpc->Index,gMessage.GetMessage(534));

	gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(535),lpObj->Name);

	this->SetState(CUSTOM_LUKE_HELPER_EMPTY);

	CUSTOM_LUKE_HELPER_RESPAWN* lpInfo = this->GetInfo(this->m_RespawnIndex);

	if(lpInfo != 0)
	{
		gItemBagManager.DropItemBySpecialValue(lpInfo->BagSpecial,-1,-1,lpObj,lpObj->Map,lpObj->X,lpObj->Y);
	}

	return 1;
}