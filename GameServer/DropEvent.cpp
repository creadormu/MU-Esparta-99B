// DropEvent.cpp: implementation of the CDropEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DropEvent.h"
#include "CrywolfSync.h"
#include "DSProtocol.h"
#include "ItemDrop.h"
#include "ItemManager.h"
#include "ItemOptionRate.h"
#include "MapManager.h"
#include "MemScript.h"
#include "Message.h"
#include "Monster.h"
#include "Notice.h"
#include "ObjectManager.h"
#include "RandomManager.h"
#include "ScheduleManager.h"
#include "ServerInfo.h"
#include "Util.h"

CDropEvent gDropEvent;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDropEvent::CDropEvent() // OK
{
	for(int n = 0; n < MAX_DROP_EVENT; n++)
	{
		DROP_EVENT_INFO* lpInfo = &this->m_EventDropInfo[n];

		lpInfo->Index = n;
		lpInfo->State = DROP_EVENT_STATE_BLANK;
		lpInfo->RemainTime = 0;
		lpInfo->TargetTime = 0;
		lpInfo->TickCount = GetTickCount();

		lpInfo->StartTime.clear();

		lpInfo->ItemInfo.clear();
	}
}

CDropEvent::~CDropEvent() // OK
{

}

void CDropEvent::Init() // OK
{
	for(int n = 0; n < MAX_DROP_EVENT; n++)
	{
		if(gServerInfo.m_DropEventSwitch == 0)
		{
			this->SetState(&this->m_EventDropInfo[n],DROP_EVENT_STATE_BLANK);
		}
		else
		{
			this->SetState(&this->m_EventDropInfo[n],DROP_EVENT_STATE_EMPTY);
		}
	}
}

void CDropEvent::Load(char* path) // OK
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

	for(int n = 0; n < MAX_DROP_EVENT; n++)
	{
		this->m_EventDropInfo[n].StartMessage = -1;
		this->m_EventDropInfo[n].FinalMessage = -1;
		this->m_EventDropInfo[n].DropEventTime = 0;
		this->m_EventDropInfo[n].StartTime.clear();
		this->m_EventDropInfo[n].ItemInfo.clear();
	}

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

					DROP_EVENT_START_TIME info;

					int index = lpMemScript->GetNumber();

					info.Year = lpMemScript->GetAsNumber();

					info.Month = lpMemScript->GetAsNumber();

					info.Day = lpMemScript->GetAsNumber();

					info.DayOfWeek = lpMemScript->GetAsNumber();

					info.Hour = lpMemScript->GetAsNumber();

					info.Minute = lpMemScript->GetAsNumber();

					info.Second = lpMemScript->GetAsNumber();

					this->m_EventDropInfo[index].StartTime.push_back(info);
				}
				else if(section == 1)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					int index = lpMemScript->GetNumber();

					this->m_EventDropInfo[index].StartMessage = lpMemScript->GetAsNumber();

					this->m_EventDropInfo[index].FinalMessage = lpMemScript->GetAsNumber();

					this->m_EventDropInfo[index].DropEventTime = lpMemScript->GetAsNumber();
				}
				else if(section == 2)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					DROP_EVENT_ITEM_INFO info;

					memset(&info,0,sizeof(info));

					int index = lpMemScript->GetNumber();

					info.ItemIndex = SafeGetItem(GET_ITEM(lpMemScript->GetAsNumber(),lpMemScript->GetAsNumber()));

					info.Level = lpMemScript->GetAsNumber();

					info.Durability = lpMemScript->GetAsNumber();

					info.Option0 = lpMemScript->GetAsNumber();

					info.Option1 = lpMemScript->GetAsNumber();

					info.Option2 = lpMemScript->GetAsNumber();

					info.Option3 = lpMemScript->GetAsNumber();

					info.Option4 = lpMemScript->GetAsNumber();

					info.Option5 = lpMemScript->GetAsNumber();

					#if(GAMESERVER_UPDATE>=401)

					info.Option6 = lpMemScript->GetAsNumber();

					info.Duration = lpMemScript->GetAsNumber();

					#endif

					info.MapNumber = lpMemScript->GetAsNumber();

					info.MonsterClass = lpMemScript->GetAsNumber();

					info.MonsterLevelMin = lpMemScript->GetAsNumber();

					info.MonsterLevelMax = lpMemScript->GetAsNumber();

					info.AccountLevel = lpMemScript->GetAsNumber();

					info.DropRate = lpMemScript->GetAsNumber();

					this->m_EventDropInfo[index].ItemInfo.push_back(info);
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

void CDropEvent::MainProc() // OK
{
	for(int n = 0; n < MAX_DROP_EVENT; n++)
	{
		DROP_EVENT_INFO* lpInfo = &this->m_EventDropInfo[n];

		if((GetTickCount()- lpInfo->TickCount) >= 1000)
		{
			lpInfo->TickCount = GetTickCount();

			lpInfo->RemainTime = (int)difftime(lpInfo->TargetTime,time(0));

			switch(lpInfo->State)
			{
				case DROP_EVENT_STATE_BLANK:
					this->ProcState_BLANK(lpInfo);
					break;
				case DROP_EVENT_STATE_EMPTY:
					this->ProcState_EMPTY(lpInfo);
					break;
				case DROP_EVENT_STATE_START:
					this->ProcState_START(lpInfo);
					break;
			}
		}
	}
}

void CDropEvent::ProcState_BLANK(DROP_EVENT_INFO* lpInfo) // OK
{

}

void CDropEvent::ProcState_EMPTY(DROP_EVENT_INFO* lpInfo) // OK
{
	if(lpInfo->RemainTime <= 0)
	{
		if(lpInfo->StartMessage != -1) { gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(lpInfo->StartMessage)); }
		this->SetState(lpInfo,DROP_EVENT_STATE_START);
	}
}

void CDropEvent::ProcState_START(DROP_EVENT_INFO* lpInfo) // OK
{
	if(lpInfo->RemainTime <= 0)
	{
		if(lpInfo->FinalMessage != -1) { gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(lpInfo->FinalMessage)); }
		this->SetState(lpInfo,DROP_EVENT_STATE_EMPTY);
	}
}

void CDropEvent::SetState(DROP_EVENT_INFO* lpInfo,int state) // OK
{
	switch ((lpInfo->State = state))
	{
	case DROP_EVENT_STATE_BLANK:
		this->SetState_BLANK(lpInfo);
		break;
	case DROP_EVENT_STATE_EMPTY:
		this->SetState_EMPTY(lpInfo);
		break;
	case DROP_EVENT_STATE_START:
		this->SetState_START(lpInfo);
		break;
	}
}

void CDropEvent::SetState_BLANK(DROP_EVENT_INFO* lpInfo) // OK
{
	
}

void CDropEvent::SetState_EMPTY(DROP_EVENT_INFO* lpInfo) // OK
{
	this->CheckSync(lpInfo);
}

void CDropEvent::SetState_START(DROP_EVENT_INFO* lpInfo) // OK
{
	lpInfo->RemainTime = lpInfo->DropEventTime;

	lpInfo->TargetTime = (int)(time(0)+lpInfo->RemainTime);
}

void CDropEvent::CheckSync(DROP_EVENT_INFO* lpInfo) // OK
{
	if(lpInfo->StartTime.empty() != 0)
	{
		this->SetState(lpInfo,DROP_EVENT_STATE_BLANK);
		return;
	}

	CTime ScheduleTime;

	CScheduleManager ScheduleManager;

	for(std::vector<DROP_EVENT_START_TIME>::iterator it = lpInfo->StartTime.begin(); it != lpInfo->StartTime.end(); it++)
	{
		ScheduleManager.AddSchedule(it->Year,it->Month,it->Day,it->Hour,it->Minute,it->Second,it->DayOfWeek);
	}

	if(ScheduleManager.GetSchedule(&ScheduleTime) == 0)
	{
		this->SetState(lpInfo,DROP_EVENT_STATE_BLANK);
		return;
	}

	lpInfo->RemainTime = (int)difftime(ScheduleTime.GetTime(),time(0));

	lpInfo->TargetTime = (int)ScheduleTime.GetTime();

	LogAdd(LOG_BLACK,"[Drop Event] (%d) Sync Start Time. [%d] min remain",lpInfo->Index,(lpInfo->RemainTime/60));
}

int CDropEvent::GetState(int index) // OK
{
	if(CHECK_RANGE(index,MAX_DROP_EVENT) == 0)
	{
		return DROP_EVENT_STATE_BLANK;
	}
	else
	{
		return this->m_EventDropInfo[index].State;
	}
}

int CDropEvent::GetRemainTime(int index) // OK
{
	if(CHECK_RANGE(index,MAX_DROP_EVENT) == 0)
	{
		return 0;
	}

	DROP_EVENT_INFO* lpInfo = &this->m_EventDropInfo[index];

	if(lpInfo->StartTime.empty() != 0)
	{
		return 0;
	}

	CTime ScheduleTime;

	CScheduleManager ScheduleManager;

	for(std::vector<DROP_EVENT_START_TIME>::iterator it = lpInfo->StartTime.begin(); it != lpInfo->StartTime.end(); it++)
	{
		ScheduleManager.AddSchedule(it->Year,it->Month,it->Day,it->Hour,it->Minute,it->Second,it->DayOfWeek);
	}

	if(ScheduleManager.GetSchedule(&ScheduleTime) == 0)
	{
		return 0;
	}

	int RemainTime = (int)difftime(ScheduleTime.GetTime(),time(0));

	return (((RemainTime % 60) == 0) ? (RemainTime/60) : ((RemainTime/60)+1));
}

int CDropEvent::DropItem(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	if(gMapManager.GetMapItemDrop(lpTarget->Map) == 0)
	{
		return 0;
	}

	CRandomManager RandomManager;

	for(int n = 0; n < MAX_DROP_EVENT; n++)
	{
		if(this->m_EventDropInfo[n].State == DROP_EVENT_STATE_START)
		{
			for(std::vector<DROP_EVENT_ITEM_INFO>::iterator it = this->m_EventDropInfo[n].ItemInfo.begin(); it != this->m_EventDropInfo[n].ItemInfo.end(); it++)
			{
				int DropRate;

				ITEM_INFO ItemInfo;

				if(gItemManager.GetInfo(it->ItemIndex,&ItemInfo) == 0)
				{
					continue;
				}

				if(it->MapNumber != -1 && it->MapNumber != lpObj->Map)
				{
					continue;
				}

				if(it->MonsterClass != -1 && it->MonsterClass != lpObj->Class)
				{
					continue;
				}

				if(it->MonsterLevelMin != -1 && it->MonsterLevelMin > lpObj->Level)
				{
					continue;
				}

				if(it->MonsterLevelMax != -1 && it->MonsterLevelMax < lpObj->Level)
				{
					continue;
				}

				if(it->AccountLevel != -1 && it->AccountLevel != lpTarget->AccountLevel)
				{
					continue;
				}

				if((DropRate = it->DropRate) == -1 || (GetLargeRand() % 1000000) < (DropRate = this->GetItemDropRate(lpObj,lpTarget,it->ItemIndex,it->DropRate)))
				{
					int rate = (1000000/((DropRate == -1) ? 1000000 : DropRate));

					RandomManager.AddElement((int)(&(*it)),rate);
				}
			}
		}
	}

	ITEM_DROP_INFO* lpItemDropInfo;

	if(RandomManager.GetRandomElement((int*)&lpItemDropInfo) == 0)
	{
		return 0;
	}
	else
	{
		WORD ItemIndex = lpItemDropInfo->ItemIndex;
		BYTE ItemLevel = lpItemDropInfo->Level;
		BYTE ItemDur = lpItemDropInfo->Durability;
		BYTE ItemOption1 = 0;
		BYTE ItemOption2 = 0;
		BYTE ItemOption3 = 0;
		BYTE ItemNewOption = 0;
		BYTE ItemSetOption = 0;
		BYTE ItemSocketOption[MAX_SOCKET_OPTION] = { 0xFF,0xFF,0xFF,0xFF,0xFF };

		gItemOptionRate.GetItemOption0(lpItemDropInfo->Option0,&ItemLevel);

		gItemOptionRate.GetItemOption1(lpItemDropInfo->Option1,&ItemOption1);

		gItemOptionRate.GetItemOption2(lpItemDropInfo->Option2,&ItemOption2);

		gItemOptionRate.GetItemOption3(lpItemDropInfo->Option3,&ItemOption3);

		gItemOptionRate.GetItemOption4(lpItemDropInfo->Option4,&ItemNewOption);

		gItemOptionRate.GetItemOption5(lpItemDropInfo->Option5,&ItemSetOption);

		gItemOptionRate.GetItemOption6(lpItemDropInfo->Option6,&ItemSocketOption[0]);

		gItemOptionRate.MakeNewOption(ItemIndex,1,ItemNewOption,&ItemNewOption);

		gItemOptionRate.MakeSetOption(ItemIndex,ItemSetOption,&ItemSetOption);

		gItemOptionRate.MakeSocketOption(ItemIndex,ItemSocketOption[0],&ItemSocketOption[0]);

		GDCreateItemSend(lpTarget->Index,lpObj->Map,(BYTE)lpObj->X,(BYTE)lpObj->Y,ItemIndex,ItemLevel,ItemDur,ItemOption1,ItemOption2,ItemOption3,lpTarget->Index,ItemNewOption,ItemSetOption,0,0,ItemSocketOption,0xFF,((lpItemDropInfo->Duration>0)?((DWORD)time(0)+lpItemDropInfo->Duration):0));

		return 1;
	}
}

int CDropEvent::GetItemDropRate(LPOBJ lpObj,LPOBJ lpTarget,int ItemIndex,int DropRate) // OK
{
	if(gItemManager.IsJewelItem(ItemIndex) != 0 && gCrywolfSync.CheckApplyPenalty() != 0 && gCrywolfSync.GetOccupationState() == 1)
	{
		if((GetLargeRand() % 100) >= gCrywolfSync.GetGemDropPenaltiyRate())
		{
			return 0;
		}
	}

	return DropRate;
}