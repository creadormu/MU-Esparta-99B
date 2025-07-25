// CustomEventDrop.cpp: implementation of the CCustomEventDrop class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomEventDrop.h"
#include "DSProtocol.h"
#include "Map.h"
#include "MapManager.h"
#include "MemScript.h"
#include "Message.h"
#include "Notice.h"
#include "ScheduleManager.h"
#include "User.h"
#include "Util.h"
#include "Viewport.h"

CCustomEventDrop gCustomEventDrop;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomEventDrop::CCustomEventDrop() // OK
{
	for(int n=0;n < MAX_CUSTOM_EVENT_DROP;n++)
	{
		CUSTOM_EVENT_DROP_INFO* lpInfo = &this->m_CustomEventDropInfo[n];

		lpInfo->Index = n;
		lpInfo->State = CUSTOM_EVENT_DROP_STATE_BLANK;
		lpInfo->RemainTime = 0;
		lpInfo->TargetTime = 0;
		lpInfo->TickCount = GetTickCount();
		lpInfo->AlarmMinSave = -1;
		lpInfo->AlarmMinLeft = -1;
	}
}

CCustomEventDrop::~CCustomEventDrop() // OK
{

}

void CCustomEventDrop::Init() // OK
{
	for(int n=0;n < MAX_CUSTOM_EVENT_DROP;n++)
	{
		if(this->m_CustomEventDropSwitch == 0)
		{
			this->SetState(&this->m_CustomEventDropInfo[n],CUSTOM_EVENT_DROP_STATE_BLANK);
		}
		else
		{
			this->SetState(&this->m_CustomEventDropInfo[n],CUSTOM_EVENT_DROP_STATE_EMPTY);
		}
	}
}

void CCustomEventDrop::ReadCustomEventDropInfo(char* section,char* path) // OK
{
	this->m_CustomEventDropSwitch = GetPrivateProfileInt(section,"CustomEventDropSwitch",0,path);
}

void CCustomEventDrop::Load(char* path) // OK
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

	for(int n=0;n < MAX_CUSTOM_EVENT_DROP;n++)
	{
		this->m_CustomEventDropInfo[n].StartTime.clear();
		this->m_CustomEventDropInfo[n].RuleInfo.DropItem.clear();
	}

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

					CUSTOM_EVENT_DROP_START_TIME info;

					int index = lpMemScript->GetNumber();

					info.Year = lpMemScript->GetAsNumber();

					info.Month = lpMemScript->GetAsNumber();

					info.Day = lpMemScript->GetAsNumber();

					info.DayOfWeek = lpMemScript->GetAsNumber();

					info.Hour = lpMemScript->GetAsNumber();

					info.Minute = lpMemScript->GetAsNumber();

					info.Second = lpMemScript->GetAsNumber();

					this->m_CustomEventDropInfo[index].StartTime.push_back(info);
				}
				else if(section == 1)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					int index = lpMemScript->GetNumber();

					strcpy_s(this->m_CustomEventDropInfo[index].RuleInfo.Name,lpMemScript->GetAsString());

					this->m_CustomEventDropInfo[index].RuleInfo.Map = lpMemScript->GetAsNumber();

					this->m_CustomEventDropInfo[index].RuleInfo.X = lpMemScript->GetAsNumber();

					this->m_CustomEventDropInfo[index].RuleInfo.Y = lpMemScript->GetAsNumber();

					this->m_CustomEventDropInfo[index].RuleInfo.Range = lpMemScript->GetAsNumber();

					this->m_CustomEventDropInfo[index].RuleInfo.AlarmTime = lpMemScript->GetAsNumber();

					this->m_CustomEventDropInfo[index].RuleInfo.EventTime = lpMemScript->GetAsNumber();
				}
				else if(section == 2)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					CUSTOM_EVENT_DROP_ITEM_INFO info;

					int index = lpMemScript->GetNumber();

					info.ItemIndex = SafeGetItem(GET_ITEM(lpMemScript->GetAsNumber(),lpMemScript->GetAsNumber()));

					info.ItemLevel = lpMemScript->GetAsNumber();

					info.DropCount = lpMemScript->GetAsNumber();

					info.DropDelay = lpMemScript->GetAsNumber();

					this->m_CustomEventDropInfo[index].RuleInfo.DropItem.push_back(info);
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
}

void CCustomEventDrop::MainProc() // OK
{
	for(int n=0;n < MAX_CUSTOM_EVENT_DROP;n++)
	{
		CUSTOM_EVENT_DROP_INFO* lpInfo = &this->m_CustomEventDropInfo[n];

		if((GetTickCount()-lpInfo->TickCount) >= 1000)
		{
			lpInfo->TickCount = GetTickCount();

			lpInfo->RemainTime = (int)difftime(lpInfo->TargetTime,time(0));

			switch(lpInfo->State)
			{
				case CUSTOM_EVENT_DROP_STATE_BLANK:
					this->ProcState_BLANK(lpInfo);
					break;
				case CUSTOM_EVENT_DROP_STATE_EMPTY:
					this->ProcState_EMPTY(lpInfo);
					break;
				case CUSTOM_EVENT_DROP_STATE_START:
					this->ProcState_START(lpInfo);
					break;
			}
		}
	}
}

void CCustomEventDrop::ProcState_BLANK(CUSTOM_EVENT_DROP_INFO* lpInfo) // OK
{

}

void CCustomEventDrop::ProcState_EMPTY(CUSTOM_EVENT_DROP_INFO* lpInfo) // OK
{
	if(lpInfo->RemainTime > 0 && lpInfo->RemainTime <= (lpInfo->RuleInfo.AlarmTime*60))
	{
		if((lpInfo->AlarmMinSave=(((lpInfo->RemainTime%60)==0)?((lpInfo->RemainTime/60)-1):(lpInfo->RemainTime/60))) != lpInfo->AlarmMinLeft)
		{
			lpInfo->AlarmMinLeft = lpInfo->AlarmMinSave;

			gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(525),lpInfo->RuleInfo.Name,(lpInfo->AlarmMinLeft+1));
		}
	}

	if(lpInfo->RemainTime <= 0)
	{
		gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(526),lpInfo->RuleInfo.Name);
		this->SetState(lpInfo,CUSTOM_EVENT_DROP_STATE_START);
	}
}

void CCustomEventDrop::ProcState_START(CUSTOM_EVENT_DROP_INFO* lpInfo) // OK
{
	for(std::vector<CUSTOM_EVENT_DROP_ITEM_INFO>::iterator it=lpInfo->RuleInfo.DropItem.begin();it != lpInfo->RuleInfo.DropItem.end();it++)
	{
		if(it->DropState == 0 && ((lpInfo->RuleInfo.EventTime*60)-lpInfo->RemainTime) >= it->DropDelay)
		{
			for(int n=0;n < it->DropCount;n++)
			{
				it->DropState = 1;

				int px = lpInfo->RuleInfo.X;
				int py = lpInfo->RuleInfo.Y;

				if(gObjGetRandomFreeLocation(lpInfo->RuleInfo.Map,&px,&py,lpInfo->RuleInfo.Range,lpInfo->RuleInfo.Range,50) == 0)
				{
					px = lpInfo->RuleInfo.X;
					py = lpInfo->RuleInfo.Y;
				}

				this->GCFireworksSendToNearUser(lpInfo->RuleInfo.Map,px,py);

				GDCreateItemSend(this->GetDummyUserIndex(),lpInfo->RuleInfo.Map,px,py,it->ItemIndex,it->ItemLevel,0,0,0,0,-1,0,0,0,0,0,0xFF,0);
			}
		}
	}

	if(lpInfo->RemainTime <= 0)
	{
		gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(527),lpInfo->RuleInfo.Name);
		this->SetState(lpInfo,CUSTOM_EVENT_DROP_STATE_EMPTY);
	}
}

void CCustomEventDrop::SetState(CUSTOM_EVENT_DROP_INFO* lpInfo,int state) // OK
{
	switch((lpInfo->State=state))
	{
		case CUSTOM_EVENT_DROP_STATE_BLANK:
			this->SetState_BLANK(lpInfo);
			break;
		case CUSTOM_EVENT_DROP_STATE_EMPTY:
			this->SetState_EMPTY(lpInfo);
			break;
		case CUSTOM_EVENT_DROP_STATE_START:
			this->SetState_START(lpInfo);
			break;
	}
}

void CCustomEventDrop::SetState_BLANK(CUSTOM_EVENT_DROP_INFO* lpInfo) // OK
{

}

void CCustomEventDrop::SetState_EMPTY(CUSTOM_EVENT_DROP_INFO* lpInfo) // OK
{
	lpInfo->AlarmMinSave = -1;
	lpInfo->AlarmMinLeft = -1;

	for(std::vector<CUSTOM_EVENT_DROP_ITEM_INFO>::iterator it=lpInfo->RuleInfo.DropItem.begin();it != lpInfo->RuleInfo.DropItem.end();it++)
	{
		it->DropState = 0;
	}

	this->CheckSync(lpInfo);
}

void CCustomEventDrop::SetState_START(CUSTOM_EVENT_DROP_INFO* lpInfo) // OK
{
	lpInfo->AlarmMinSave = -1;
	lpInfo->AlarmMinLeft = -1;

	lpInfo->RemainTime = lpInfo->RuleInfo.EventTime*60;

	lpInfo->TargetTime = (int)(time(0)+lpInfo->RemainTime);
}

void CCustomEventDrop::CheckSync(CUSTOM_EVENT_DROP_INFO* lpInfo) // OK
{
	if(lpInfo->StartTime.empty() != 0)
	{
		this->SetState(lpInfo,CUSTOM_EVENT_DROP_STATE_BLANK);
		return;
	}

	CTime ScheduleTime;

	CScheduleManager ScheduleManager;

	for(std::vector<CUSTOM_EVENT_DROP_START_TIME>::iterator it=lpInfo->StartTime.begin();it != lpInfo->StartTime.end();it++)
	{
		ScheduleManager.AddSchedule(it->Year,it->Month,it->Day,it->Hour,it->Minute,it->Second,it->DayOfWeek);
	}

	if(ScheduleManager.GetSchedule(&ScheduleTime) == 0)
	{
		this->SetState(lpInfo,CUSTOM_EVENT_DROP_STATE_BLANK);
		return;
	}

	lpInfo->RemainTime = (int)difftime(ScheduleTime.GetTime(),time(0));

	lpInfo->TargetTime = (int)ScheduleTime.GetTime();
}

LONG CCustomEventDrop::GetDummyUserIndex() // OK
{
	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnectedGP(n) != 0)
		{
			return n;
		}
	}

	return OBJECT_START_USER;
}

void CCustomEventDrop::GCFireworksSendToNearUser(int map,int x,int y) // OK
{
	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnectedGP(n) != 0)
		{
			if(gViewport.CheckViewportObjectPosition(n,map,x,y,gMapManager.GetMapViewRange(map)) != 0){GCServerCommandSend(n,0,x,y,0);}
		}
	}
}
