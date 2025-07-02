// AcheronGuardian.cpp: implementation of the CAcheronGuardian class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AcheronGuardian.h"
#include "ArcaBattle.h"
#include "DSProtocol.h"
#include "GameMain.h"
#include "ItemBagManager.h"
#include "Map.h"
#include "MapManager.h"
#include "MapServerManager.h"
#include "MemScript.h"
#include "Message.h"
#include "Monster.h"
#include "Notice.h"
#include "ScheduleManager.h"
#include "ServerInfo.h"
#include "Util.h"

CAcheronGuardian gAcheronGuardian;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAcheronGuardian::CAcheronGuardian() // OK
{
	#if(GAMESERVER_UPDATE>=801)

	this->m_State = AG_STATE_EMPTY;
	this->m_SyncState = AG_STATE_EMPTY;
	this->m_RemainTime = 0;
	this->m_TargetTime = 0;
	this->m_TickCount = GetTickCount();
	this->m_TimeNotify = 0;
	this->m_Stage = 0;
	this->m_Group = 0;
	this->m_Map = 0;
	this->m_ObeliskX = 0;
	this->m_ObeliskY = 0;

	this->m_AcheronGuardianStartTime.clear();

	this->m_AcheronGuardianGroup.clear();

	this->m_AcheronGuardianSpawn.clear();

	this->m_AcheronGuardianMonster.clear();

	#endif
}

CAcheronGuardian::~CAcheronGuardian() // OK
{

}

void CAcheronGuardian::Init() // OK
{
	#if(GAMESERVER_UPDATE>=801)

	if(gServerInfo.m_AcheronGuardianEvent == 0)
	{
		this->SetState(AG_STATE_BLANK);
	}
	else
	{
		this->SetState(AG_STATE_EMPTY);
	}

	#endif
}

void CAcheronGuardian::Load(char* path) // OK
{
	#if(GAMESERVER_UPDATE>=801)

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

	this->m_AcheronGuardianStartTime.clear();

	this->m_AcheronGuardianGroup.clear();

	this->m_AcheronGuardianSpawn.clear();

	this->m_AcheronGuardianMonster.clear();

	try
	{
		while(true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}
		
			int section = lpMemScript->GetNumber();

			while(true)
			{
				if(section == 0)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					ACHERON_GUARDIAN_START_TIME info;

					info.Year = lpMemScript->GetNumber();

					info.Month = lpMemScript->GetAsNumber();

					info.Day = lpMemScript->GetAsNumber();

					info.DayOfWeek = lpMemScript->GetAsNumber();

					info.Hour = lpMemScript->GetAsNumber();

					info.Minute = lpMemScript->GetAsNumber();

					info.Second = lpMemScript->GetAsNumber();

					this->m_AcheronGuardianStartTime.push_back(info);
				}
				else if(section == 1)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					ACHERON_GUARDIAN_GROUP info;

					info.Index = lpMemScript->GetNumber();

					info.ObeliskMapN = lpMemScript->GetAsNumber();

					info.ObeliskMapX = lpMemScript->GetAsNumber();

					info.ObeliskMapY = lpMemScript->GetAsNumber();

					info.Rate = lpMemScript->GetAsNumber();

					this->m_AcheronGuardianGroup.push_back(info);
				}
				else if(section == 2)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					ACHERON_GUARDIAN_SPAWN info;

					info.Index = lpMemScript->GetNumber();

					info.Group = lpMemScript->GetAsNumber();

					info.Minute = lpMemScript->GetAsNumber();

					this->m_AcheronGuardianSpawn.push_back(info);
				}
				else if(section == 3)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					ACHERON_GUARDIAN_MONSTER info;

					info.Index = lpMemScript->GetNumber();

					info.Group = lpMemScript->GetAsNumber();

					info.MonsterClass = lpMemScript->GetAsNumber();

					info.Count = lpMemScript->GetAsNumber();

					info.X = lpMemScript->GetAsNumber();

					info.Y = lpMemScript->GetAsNumber();

					info.TX = lpMemScript->GetAsNumber();

					info.TY = lpMemScript->GetAsNumber();

					this->m_AcheronGuardianMonster.push_back(info);
				}
				else
				{
					break;
				}
			}
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;

	#endif
}

void CAcheronGuardian::MainProc() // OK
{
	#if(GAMESERVER_UPDATE>=801)

	if((GetTickCount()-this->m_TickCount) >= 1000)
	{
		this->m_TickCount = GetTickCount();

		this->m_RemainTime = (int)difftime(this->m_TargetTime,time(0));

		switch(this->m_State)
		{
			case AG_STATE_BLANK:
				this->ProcState_BLANK();
				break;
			case AG_STATE_EMPTY:
				this->ProcState_EMPTY();
				break;
			case AG_STATE_START:
				this->ProcState_START();
				break;
			case AG_STATE_CLEAN:
				this->ProcState_CLEAN();
				break;
		}
	}

	#endif
}

void CAcheronGuardian::ProcState_BLANK() // OK
{

}

void CAcheronGuardian::ProcState_EMPTY() // OK
{
	#if(GAMESERVER_UPDATE>=801)

	if(this->m_RemainTime > 0 && this->m_RemainTime <= 300 && this->m_TimeNotify == 0)
	{
		this->m_TimeNotify = 1;

		GDGlobalNoticeSend(gMapServerManager.GetMapServerGroup(),0,0,0,0,0,0,gMessage.GetMessage(702));
	}

	if(this->m_RemainTime <= 0)
	{
		GDGlobalNoticeSend(gMapServerManager.GetMapServerGroup(),0,0,0,0,0,0,gMessage.GetMessage(703));

		this->SetState(AG_STATE_START);
	}

	#endif
}

void CAcheronGuardian::ProcState_START() // OK
{
	#if(GAMESERVER_UPDATE>=801)

	for(std::vector<ACHERON_GUARDIAN_SPAWN>::iterator it=this->m_AcheronGuardianSpawn.begin();it != this->m_AcheronGuardianSpawn.end();it++)
	{
		if(this->m_Group == it->Index && this->m_Stage == it->Group && (((gServerInfo.m_AcheronGuardianEventTime*60)-this->m_RemainTime)/60) == it->Minute)
		{
			this->AddMonster(it->Index,it->Group);
			this->m_Stage++;
			break;
		}
	}

	if(this->m_RemainTime <= 0)
	{
		gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(704));

		this->SetState(AG_STATE_CLEAN);
	}

	#endif
}

void CAcheronGuardian::ProcState_CLEAN() // OK
{
	#if(GAMESERVER_UPDATE>=801)

	if(this->m_RemainTime > 0 && this->m_RemainTime <= 10 && this->m_TimeNotify == 0)
	{
		this->m_TimeNotify = 1;

		PSBMSG_HEAD pMsg;

		pMsg.set(0xF8,0x4D,sizeof(pMsg));

		this->DataSendToAll((BYTE*)&pMsg,pMsg.size);
	}

	if(this->m_RemainTime <= 0)
	{
		this->SetState(AG_STATE_EMPTY);
	}

	#endif
}

void CAcheronGuardian::SetState(int state) // OK
{
	#if(GAMESERVER_UPDATE>=801)

	this->m_State = state;

	this->GDAcheronGuardianSyncSend(state);

	switch(this->m_State)
	{
		case AG_STATE_BLANK:
			this->SetState_BLANK();
			break;
		case AG_STATE_EMPTY:
			this->SetState_EMPTY();
			break;
		case AG_STATE_START:
			this->SetState_START();
			break;
		case AG_STATE_CLEAN:
			this->SetState_CLEAN();
			break;
	}

	#endif
}

void CAcheronGuardian::SetState_BLANK() // OK
{
	
}

void CAcheronGuardian::SetState_EMPTY() // OK
{
	#if(GAMESERVER_UPDATE>=801)

	this->m_TimeNotify = 0;
	this->m_Stage = 1;
	this->m_Group = 0;
	this->m_Map = 0;
	this->m_ObeliskX = 0;
	this->m_ObeliskY = 0;

	this->ClearUser();

	this->SetAcheronMapAttr(0);

	this->CheckSync();

	LogAdd(LOG_BLACK,"[Acheron Guardian] SetState EMPTY");

	#endif
}

void CAcheronGuardian::SetState_START() // OK
{
	#if(GAMESERVER_UPDATE>=801)

	this->m_RemainTime = gServerInfo.m_AcheronGuardianEventTime*60;

	this->m_TargetTime = (int)(time(0)+this->m_RemainTime);

	this->SetAcheronMapAttr(1);

	while(this->m_Group == 0)
	{
		for(std::vector<ACHERON_GUARDIAN_GROUP>::iterator it=this->m_AcheronGuardianGroup.begin();it != this->m_AcheronGuardianGroup.end();it++)
		{
			if((GetLargeRand()%100) < it->Rate)
			{
				this->m_Group = it->Index;
				this->m_Map = it->ObeliskMapN;
				this->m_ObeliskX = it->ObeliskMapX;
				this->m_ObeliskY = it->ObeliskMapY;
				break;
			}
		}
	}

	this->AddObelisk();

	LogAdd(LOG_BLACK,"[Acheron Guardian] SetState START");

	#endif
}

void CAcheronGuardian::SetState_CLEAN() // OK
{
	#if(GAMESERVER_UPDATE>=801)

	this->m_RemainTime = 60;

	this->m_TargetTime = (int)(time(0)+this->m_RemainTime);

	this->m_TimeNotify = 0;

	this->ClearMonster();

	LogAdd(LOG_BLACK,"[Acheron Guardian] SetState CLEAN");

	#endif
}

void CAcheronGuardian::CheckSync() // OK
{
	#if(GAMESERVER_UPDATE>=801)

	if(this->m_AcheronGuardianStartTime.empty() != 0)
	{
		this->SetState(AG_STATE_BLANK);
		return;
	}

	CTime ScheduleTime;

	CScheduleManager ScheduleManager;

	for(std::vector<ACHERON_GUARDIAN_START_TIME>::iterator it=this->m_AcheronGuardianStartTime.begin();it != this->m_AcheronGuardianStartTime.end();it++)
	{
		ScheduleManager.AddSchedule(it->Year,it->Month,it->Day,it->Hour,it->Minute,it->Second,it->DayOfWeek);
	}

	if(ScheduleManager.GetSchedule(&ScheduleTime) == 0)
	{
		this->SetState(AG_STATE_BLANK);
		return;
	}

	this->m_RemainTime = (int)difftime(ScheduleTime.GetTime(),time(0));

	this->m_TargetTime = (int)ScheduleTime.GetTime();

	LogAdd(LOG_BLACK,"[Acheron Guardian] Sync Start Time. [%d] min remain",(this->m_RemainTime/60));

	#endif
}

int CAcheronGuardian::GetState() // OK
{
	#if(GAMESERVER_UPDATE>=801)

	return this->m_State;

	#else

	return 0;

	#endif
}

int CAcheronGuardian::GetSyncState() // OK
{
	#if(GAMESERVER_UPDATE>=801)

	return this->m_SyncState;

	#else

	return 0;

	#endif
}

void CAcheronGuardian::ClearUser() // OK
{
	#if(GAMESERVER_UPDATE>=801)

	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnectedGP(n) != 0)
		{
			if(AB_MAP_RANGE(gObj[n].Map) != 0)
			{
				gObjMoveGate(n,27);
			}
		}
	}

	#endif
}

void CAcheronGuardian::ClearMonster() // OK
{
	#if(GAMESERVER_UPDATE>=801)

	for(int n=0;n < MAX_OBJECT_MONSTER;n++)
	{
		if(gObjIsConnected(n) != 0 && gObj[n].Attribute == ATTRIBUTE_ACHERON)
		{
			gObjDel(n);
		}
	}

	#endif
}

void CAcheronGuardian::AddMonster(int stage,int group) // OK 
{
	#if(GAMESERVER_UPDATE>=801)

	for(std::vector<ACHERON_GUARDIAN_MONSTER>::iterator it=this->m_AcheronGuardianMonster.begin();it != this->m_AcheronGuardianMonster.end();it++)
	{
		if(it->Index != stage || it->Group != group)
		{
			continue;
		}

		for(int n=0;n < it->Count;n++)
		{
			int index = gObjAddMonster(this->m_Map);

			if(OBJECT_RANGE(index) == 0)
			{
				continue;
			}

			LPOBJ lpObj = &gObj[index];

			int px = it->X;
			int py = it->Y;

			if(gObjGetRandomFreeLocation(this->m_Map,&px,&py,(it->TX-it->X),(it->TY-it->Y),10) == 0)
			{
				px = it->X;
				py = it->Y;
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
			lpObj->Dir = GetLargeRand()%8;
			lpObj->Map = this->m_Map;

			if(gObjSetMonster(index,it->MonsterClass) == 0)
			{
				gObjDel(index);
				continue;
			}

			lpObj->DieRegen = 0;
			lpObj->RegenTime = 1;
			lpObj->Attribute = ATTRIBUTE_ACHERON;
			lpObj->MaxRegenTime = 1000;
			lpObj->LastCheckTick = GetTickCount();
		}
	}

	#endif
}

void CAcheronGuardian::AddObelisk() // OK
{
	#if(GAMESERVER_UPDATE>=801)

	int index = gObjAddMonster(this->m_Map);

	if(OBJECT_RANGE(index) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[index];

	int px = this->m_ObeliskX;
	int py = this->m_ObeliskY;

	lpObj->PosNum = -1;
	lpObj->X = px;
	lpObj->Y = py;
	lpObj->TX = px;
	lpObj->TY = py;
	lpObj->OldX = px;
	lpObj->OldY = py;
	lpObj->StartX = px;
	lpObj->StartY = py;
	lpObj->Dir = 1;
	lpObj->Map = this->m_Map;

	if(gObjSetMonster(index,627) == 0)
	{
		gObjDel(index);
		return;
	}

	LogAdd(LOG_BLACK,"[Acheron Guardian] Create Obelisk [%s][%d][%d]",gMapManager.GetMapName(lpObj->Map),lpObj->X,lpObj->Y);

	return;

	#endif
}

void CAcheronGuardian::DropTowerItem(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	#if(GAMESERVER_UPDATE>=801)

	if(gServerInfo.m_AcheronGuardianEvent == 0)
	{
		return;
	}

	if(this->m_Map != lpObj->Map)
	{
		return;
	}

	gItemBagManager.DropItemBySpecialValue(ITEM_BAG_SPECIAL_ACHERON_GUARDIAN,-1,-1,lpTarget,lpObj->Map,lpObj->X,lpObj->Y);

	gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(705),lpTarget->Name);
		
	this->SetState(AG_STATE_CLEAN);

	#endif
}

void CAcheronGuardian::DataSendToAll(BYTE* lpMsg,int size) // OK
{
	#if(GAMESERVER_UPDATE>=801)

	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnectedGP(n) != 0)
		{
			if(AB_MAP_RANGE(gObj[n].Map) != 0)
			{
				DataSend(n,lpMsg,size);
			}
		}
	}

	#endif
}

void CAcheronGuardian::LoadAcheronMapAttr(char* path,int map,int state) // OK
{
	#if(GAMESERVER_UPDATE>=801)

	this->m_AcheronMapAttr[map][state].Load(path,(map==0)?MAP_ACHERON2:MAP_DEVENTER2);

	#endif
}

void CAcheronGuardian::SetAcheronMapAttr(int state) // OK
{
	#if(GAMESERVER_UPDATE>=801)

	memcpy(gMap[MAP_ACHERON2].m_MapAttr,this->m_AcheronMapAttr[0][state].m_MapAttr,(this->m_AcheronMapAttr[0][state].m_width*this->m_AcheronMapAttr[0][state].m_height));

	memcpy(gMap[MAP_DEVENTER2].m_MapAttr,this->m_AcheronMapAttr[1][state].m_MapAttr,(this->m_AcheronMapAttr[1][state].m_width*this->m_AcheronMapAttr[1][state].m_height));

	#endif
}

void CAcheronGuardian::GDAcheronGuardianSyncSend(int state) // OK
{
	#if(GAMESERVER_UPDATE>=801)

	SDHP_ACHERON_GUARDIAN_SYNC_SEND pMsg;

	pMsg.header.set(0x14,sizeof(pMsg));

	pMsg.MapServerGroup = gMapServerManager.GetMapServerGroup();

	pMsg.AcheronState = state;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,-1);

	#endif
}

void CAcheronGuardian::DGAcheronGuardianSyncRecv(SDHP_ACHERON_GUARDIAN_SYNC_RECV* lpMsg) // OK
{
	#if(GAMESERVER_UPDATE>=801)

	if(gMapServerManager.GetMapServerGroup() != lpMsg->MapServerGroup)
	{
		return;
	}

	if(this->m_State == lpMsg->AcheronState || this->m_SyncState == lpMsg->AcheronState)
	{
		return;
	}

	if(lpMsg->AcheronState == AG_STATE_START)
	{
		this->SetAcheronMapAttr(1);
	}
	else if(lpMsg->AcheronState == AG_STATE_EMPTY)
	{
		this->SetAcheronMapAttr(0);
	}

	LogAdd(LOG_BLACK,"[Acheron Guardian] Sync State: (%d)",lpMsg->AcheronState);

	this->m_SyncState = lpMsg->AcheronState;

	#endif
}

void CAcheronGuardian::ForceStart() // OK
{
	#if(GAMESERVER_UPDATE>=801)

	SYSTEMTIME SystemTime;

	GetLocalTime(&SystemTime);

	ACHERON_GUARDIAN_START_TIME info;

	info.Year = SystemTime.wYear;

	info.Month = SystemTime.wMonth;

	info.Day = SystemTime.wDay;

	info.DayOfWeek = -1;

	info.Hour = SystemTime.wHour;

	info.Minute = (SystemTime.wMinute+1);

	info.Second = SystemTime.wSecond;

	this->m_AcheronGuardianStartTime.push_back(info);

	this->Init();

	#endif
}