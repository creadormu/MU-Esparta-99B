// ChaosCastle.cpp: implementation of the CChaosCastle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChaosCastle.h"
#include "DSProtocol.h"
#include "EffectManager.h"
#include "EventEntryLevel.h"
#include "EventSpawnStage.h"
#include "GuildManager.h"
#include "ItemBagManager.h"
#include "Map.h"
#include "MapServerManager.h"
#include "MemScript.h"
#include "Message.h"
#include "Monster.h"
#include "MonsterSetBase.h"
#include "Notice.h"
#include "ObjectManager.h"
#include "QuestWorldObjective.h"
#include "RandomManager.h"
#include "ScheduleManager.h"
#include "ServerInfo.h"
#include "Util.h"

CChaosCastle gChaosCastle;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChaosCastle::CChaosCastle() // OK
{
	for(int n = 0; n < MAX_CC_LEVEL; n++)
	{
		CHAOS_CASTLE_LEVEL* lpLevel = &this->m_ChaosCastleLevel[n];

		lpLevel->Level = n;
		lpLevel->State = CC_STATE_BLANK;
		#if(GAMESERVER_UPDATE>=401)
		lpLevel->Map = ((n<6)?(MAP_CHAOS_CASTLE1+n):MAP_CHAOS_CASTLE7);
		#else
		lpLevel->Map = (MAP_CHAOS_CASTLE1+n);
		#endif
		lpLevel->RemainTime = 0;
		lpLevel->TargetTime = 0;
		lpLevel->TickCount = GetTickCount();
		lpLevel->EnterEnabled = 0;
		lpLevel->MinutesLeft = -1;
		lpLevel->TimeCount = 0;
		lpLevel->Stage = 0;
		lpLevel->MaxMonster = 0;
		lpLevel->WinnerIndex = -1;

		this->CleanUser(lpLevel);

		this->CleanMonster(lpLevel);
	}

	memset(this->m_ChaosCastleExperienceTable,0,sizeof(this->m_ChaosCastleExperienceTable));
}

CChaosCastle::~CChaosCastle() // OK
{

}

void CChaosCastle::Init() // OK
{
	for(int n = 0; n < MAX_CC_LEVEL; n++)
	{
		if(gServerInfo.m_ChaosCastleEvent == 0)
		{
			this->SetState(&this->m_ChaosCastleLevel[n],CC_STATE_BLANK);
		}
		else
		{
			this->SetState(&this->m_ChaosCastleLevel[n],CC_STATE_EMPTY);
		}
	}
}

void CChaosCastle::Load(char* path) // OK
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

	this->m_ChaosCastleStartTime.clear();

	memset(this->m_ChaosCastleExperienceTable,0,sizeof(this->m_ChaosCastleExperienceTable));

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

					this->m_WarningTime = lpMemScript->GetNumber();

					this->m_NotifyTime = lpMemScript->GetAsNumber();

					this->m_EventTime = lpMemScript->GetAsNumber();

					this->m_CloseTime = lpMemScript->GetAsNumber();
				}
				else if(section == 1)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					CHAOS_CASTLE_START_TIME info;

					info.Year = lpMemScript->GetNumber();

					info.Month = lpMemScript->GetAsNumber();

					info.Day = lpMemScript->GetAsNumber();

					info.DayOfWeek = lpMemScript->GetAsNumber();

					info.Hour = lpMemScript->GetAsNumber();

					info.Minute = lpMemScript->GetAsNumber();

					info.Second = lpMemScript->GetAsNumber();

					this->m_ChaosCastleStartTime.push_back(info);
				}
				else if(section == 2)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					int level = lpMemScript->GetNumber();

					this->m_ChaosCastleExperienceTable[level][0] = lpMemScript->GetAsNumber();

					this->m_ChaosCastleExperienceTable[level][1] = lpMemScript->GetAsNumber();
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

void CChaosCastle::MainProc() // OK
{
	for(int n = 0; n < MAX_CC_LEVEL; n++)
	{
		CHAOS_CASTLE_LEVEL* lpLevel = &this->m_ChaosCastleLevel[n];

		if((GetTickCount()-lpLevel->TickCount) >= 1000)
		{
			lpLevel->TickCount = GetTickCount();

			lpLevel->RemainTime = (int)difftime(lpLevel->TargetTime,time(0));

			switch(lpLevel->State)
			{
				case CC_STATE_BLANK:
					this->ProcState_BLANK(lpLevel);
					break;
				case CC_STATE_EMPTY:
					this->ProcState_EMPTY(lpLevel);
					break;
				case CC_STATE_STAND:
					this->ProcState_STAND(lpLevel);
					break;
				case CC_STATE_START:
					this->ProcState_START(lpLevel);
					break;
				case CC_STATE_CLEAN:
					this->ProcState_CLEAN(lpLevel);
					break;
			}

			this->CheckDelayScore(lpLevel);
		}
	}
}

void CChaosCastle::ProcState_BLANK(CHAOS_CASTLE_LEVEL* lpLevel) // OK
{

}

void CChaosCastle::ProcState_EMPTY(CHAOS_CASTLE_LEVEL* lpLevel) // OK
{
	if(lpLevel->RemainTime > 0 && lpLevel->RemainTime <= (this->m_WarningTime*60))
	{
		this->CheckUser(lpLevel);

		int minutes = lpLevel->RemainTime/60;

		if((lpLevel->RemainTime % 60) == 0)
		{
			minutes--;
		}

		if(lpLevel->MinutesLeft != minutes)
		{
			lpLevel->MinutesLeft = minutes;

			if(lpLevel->Level == 0)
			{
				gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(16),(lpLevel->MinutesLeft+1));
			}
		}

		lpLevel->EnterEnabled = 1;
	}

	if(lpLevel->RemainTime > 0 && lpLevel->RemainTime <= 30 && lpLevel->TimeCount == 0)
	{
		PMSG_TIME_COUNT_SEND pMsg;

		pMsg.header.set(0x92,sizeof(pMsg));

		pMsg.type = 10;

		DataSendAll((BYTE*)&pMsg,pMsg.header.size);

		lpLevel->TimeCount = 1;
	}

	if(lpLevel->RemainTime <= 0)
	{
		if(lpLevel->Level == 0) { gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(17)); }
		this->NoticeSendToAll(lpLevel,1,gMessage.GetMessage(18),(lpLevel->Level+1),this->m_NotifyTime);
		this->SetState(lpLevel,CC_STATE_STAND);
	}
}

void CChaosCastle::ProcState_STAND(CHAOS_CASTLE_LEVEL* lpLevel) // OK
{
	this->CheckUser(lpLevel);

	if(this->GetUserCount(lpLevel) < gServerInfo.m_ChaosCastleMinUser)
	{
		LogAdd(LOG_BLACK,"[Chaos Castle] (%d) Not enough users",(lpLevel->Level+1));
		this->NoticeSendToAll(lpLevel,0,gMessage.GetMessage(21));
		this->SetState(lpLevel,CC_STATE_EMPTY);
		return;
	}

	if(lpLevel->RemainTime > 0 && lpLevel->RemainTime <= 30 && lpLevel->TimeCount == 0)
	{
		PMSG_TIME_COUNT_SEND pMsg;

		pMsg.header.set(0x92,sizeof(pMsg));

		pMsg.type = 11;

		this->DataSendToAll(lpLevel,(BYTE*)&pMsg,pMsg.header.size);

		lpLevel->TimeCount = 1;
	}

	if(lpLevel->RemainTime <= 0)
	{
		this->NoticeSendToAll(lpLevel,0,gMessage.GetMessage(19));

		this->SetState(lpLevel,CC_STATE_START);
	}
}

void CChaosCastle::ProcState_START(CHAOS_CASTLE_LEVEL* lpLevel) // OK
{
	this->CheckUser(lpLevel);

	if(this->GetUserCount(lpLevel) == 0)
	{
		LogAdd(LOG_BLACK,"[Chaos Castle] (%d) Not enough users",(lpLevel->Level+1));
		this->SetState(lpLevel,CC_STATE_EMPTY);
		return;
	}

	switch (lpLevel->Stage)
	{
	case 0:
		this->SetStage0(lpLevel);
		break;
	case 1:
		this->SetStage1(lpLevel);
		break;
	case 2:
		this->SetStage2(lpLevel);
		break;
	case 3:
		this->SetStage3(lpLevel);
		break;
	}

	this->CheckUserPosition(lpLevel);
	this->CheckMonsterPosition(lpLevel);
	this->CheckItemPosition(lpLevel);

	this->GCChaosCastleStateSend(lpLevel,6);

	if(this->GetUserCount(lpLevel) == 1 && this->GetMonsterCount(lpLevel) == 0)
	{
		this->SetState(lpLevel,CC_STATE_CLEAN);
		return;
	}

	if(lpLevel->RemainTime > 0 && lpLevel->RemainTime <= 30 && lpLevel->TimeCount == 0)
	{
		PMSG_TIME_COUNT_SEND pMsg;

		pMsg.header.set(0x92,sizeof(pMsg));

		pMsg.type = 12;

		this->DataSendToAll(lpLevel,(BYTE*)&pMsg,pMsg.header.size);

		lpLevel->TimeCount = 1;
	}

	if(lpLevel->RemainTime <= 0)
	{
		this->NoticeSendToAll(lpLevel,0,gMessage.GetMessage(20));

		this->SetState(lpLevel,CC_STATE_CLEAN);
	}
}

void CChaosCastle::ProcState_CLEAN(CHAOS_CASTLE_LEVEL* lpLevel) // OK
{
	this->CheckUser(lpLevel);

	if(lpLevel->RemainTime > 0 && lpLevel->RemainTime <= 30 && lpLevel->TimeCount == 0)
	{
		PMSG_TIME_COUNT_SEND pMsg;

		pMsg.header.set(0x92,sizeof(pMsg));

		pMsg.type = 13;

		this->DataSendToAll(lpLevel,(BYTE*)&pMsg,pMsg.header.size);

		lpLevel->TimeCount = 1;
	}

	if(lpLevel->RemainTime <= 0)
	{
		this->SetState(lpLevel,CC_STATE_EMPTY);
	}
}

void CChaosCastle::SetState(CHAOS_CASTLE_LEVEL* lpLevel,int state) // OK
{
	lpLevel->State = state;

	switch (lpLevel->State)
	{
	case CC_STATE_BLANK:
		this->SetState_BLANK(lpLevel);
		break;
	case CC_STATE_EMPTY:
		this->SetState_EMPTY(lpLevel);
		break;
	case CC_STATE_STAND:
		this->SetState_STAND(lpLevel);
		break;
	case CC_STATE_START:
		this->SetState_START(lpLevel);
		break;
	case CC_STATE_CLEAN:
		this->SetState_CLEAN(lpLevel);
		break;
	}
}

void CChaosCastle::SetState_BLANK(CHAOS_CASTLE_LEVEL* lpLevel) // OK
{
	
}

void CChaosCastle::SetState_EMPTY(CHAOS_CASTLE_LEVEL* lpLevel) // OK
{
	lpLevel->EnterEnabled = 0;
	lpLevel->MinutesLeft = -1;
	lpLevel->TimeCount = 0;
	lpLevel->Stage = 0;
	lpLevel->MaxMonster = 0;
	lpLevel->WinnerIndex = -1;

	this->GCChaosCastleSimpleStateSend(lpLevel,7,-1);

	this->ClearUser(lpLevel);

	this->ClearMonster(lpLevel);

	this->SetSafeZone(lpLevel);

	this->DelHollowZone(lpLevel,1);

	this->DelHollowZone(lpLevel,2);

	this->DelHollowZone(lpLevel,3);

	this->DelCenterHollowZone(lpLevel);

	this->CheckSync(lpLevel);

	LogAdd(LOG_BLACK,"[Chaos Castle] (%d) SetState EMPTY",(lpLevel->Level+1));
}

void CChaosCastle::SetState_STAND(CHAOS_CASTLE_LEVEL* lpLevel) // OK
{
	lpLevel->EnterEnabled = 0;
	lpLevel->MinutesLeft = -1;
	lpLevel->TimeCount = 0;

	this->GiveUserRewardMoney(lpLevel);

	this->SetCenterHollowZone(lpLevel);

	lpLevel->RemainTime = this->m_NotifyTime*60;

	lpLevel->TargetTime = (int)(time(0)+lpLevel->RemainTime);

	LogAdd(LOG_BLACK,"[Chaos Castle] (%d) SetState STAND",(lpLevel->Level+1));
}

void CChaosCastle::SetState_START(CHAOS_CASTLE_LEVEL* lpLevel) // OK
{
	lpLevel->EnterEnabled = 0;
	lpLevel->MinutesLeft = -1;
	lpLevel->TimeCount = 0;
	lpLevel->MaxMonster = this->GetUserCount(lpLevel);

	this->DelSafeZone(lpLevel);

	this->StageSpawn(lpLevel,0);

	this->GCChaosCastleSimpleStateSend(lpLevel,5,-1);

	lpLevel->RemainTime = this->m_EventTime*60;

	lpLevel->TargetTime = (int)(time(0)+lpLevel->RemainTime);

	LogAdd(LOG_BLACK,"[Chaos Castle] (%d) SetState START",(lpLevel->Level+1));
}

void CChaosCastle::SetState_CLEAN(CHAOS_CASTLE_LEVEL* lpLevel) // OK
{
	lpLevel->EnterEnabled = 0;
	lpLevel->MinutesLeft = -1;
	lpLevel->TimeCount = 0;
	lpLevel->WinnerIndex = this->GetUserWinner(lpLevel);

	this->ClearMonster(lpLevel);

	for(int n = 0; n < MAX_CC_USER; n++)
	{
		if(OBJECT_RANGE(lpLevel->User[n].Index) != 0)
		{
			if(lpLevel->User[n].Index == lpLevel->WinnerIndex)
			{
				this->GiveUserRewardExperience(lpLevel,lpLevel->User[n].Index);

				this->GiveUserRewardItem(lpLevel,lpLevel->User[n].Index);

				this->GCChaosCastleScoreSend(lpLevel,lpLevel->User[n].Index,1);

				GDRankingChaosCastleSaveSend(lpLevel->User[n].Index,lpLevel->User[n].Score);

				gQuestWorldObjective.QuestWorldObjectiveSpecial(&gObj[lpLevel->User[n].Index],QUEST_WORLD_OBJECTIVE_CHAOS_CASTLE_CLEAR,lpLevel->Level+1);
			}
			else
			{
				this->GiveUserRewardExperience(lpLevel,lpLevel->User[n].Index);

				this->GCChaosCastleScoreSend(lpLevel,lpLevel->User[n].Index,0);

				GDRankingChaosCastleSaveSend(lpLevel->User[n].Index,lpLevel->User[n].Score);

				gQuestWorldObjective.QuestWorldObjectiveSpecial(&gObj[lpLevel->User[n].Index],QUEST_WORLD_OBJECTIVE_CHAOS_CASTLE_CLEAR,lpLevel->Level+1);
			}
		}
	}

	this->GCChaosCastleSimpleStateSend(lpLevel,7,-1);

	lpLevel->RemainTime = this->m_CloseTime*60;

	lpLevel->TargetTime = (int)(time(0)+lpLevel->RemainTime);

	LogAdd(LOG_BLACK,"[Chaos Castle] (%d) SetState CLEAN",(lpLevel->Level+1));
}

void CChaosCastle::SetStage0(CHAOS_CASTLE_LEVEL* lpLevel) // OK
{
	if((this->GetUserCount(lpLevel)+this->GetMonsterCount(lpLevel)) > 40)
	{
		return;
	}

	lpLevel->Stage = 1;

	this->GCChaosCastleSimpleStateSend(lpLevel,8,-1);

	this->SetHollowZone(lpLevel,1);
}

void CChaosCastle::SetStage1(CHAOS_CASTLE_LEVEL* lpLevel) // OK
{
	if((this->GetUserCount(lpLevel)+this->GetMonsterCount(lpLevel)) > 30)
	{
		return;
	}

	lpLevel->Stage = 2;

	this->GCChaosCastleSimpleStateSend(lpLevel,9,-1);

	this->SetHollowZone(lpLevel,2);
}

void CChaosCastle::SetStage2(CHAOS_CASTLE_LEVEL* lpLevel) // OK
{
	if((this->GetUserCount(lpLevel)+this->GetMonsterCount(lpLevel)) > 20)
	{
		return;
	}

	lpLevel->Stage = 3;

	this->GCChaosCastleSimpleStateSend(lpLevel,10,-1);

	this->SetHollowZone(lpLevel,3);
}

void CChaosCastle::SetStage3(CHAOS_CASTLE_LEVEL* lpLevel) // OK
{

}

void CChaosCastle::CheckSync(CHAOS_CASTLE_LEVEL* lpLevel) // OK
{
	if(this->m_ChaosCastleStartTime.empty() != 0)
	{
		this->SetState(lpLevel,CC_STATE_BLANK);
		return;
	}

	CTime ScheduleTime;

	CScheduleManager ScheduleManager;

	for(std::vector<CHAOS_CASTLE_START_TIME>::iterator it = this->m_ChaosCastleStartTime.begin(); it != this->m_ChaosCastleStartTime.end(); it++)
	{
		ScheduleManager.AddSchedule(it->Year,it->Month,it->Day,it->Hour,it->Minute,it->Second,it->DayOfWeek);
	}

	if(ScheduleManager.GetSchedule(&ScheduleTime) == 0)
	{
		this->SetState(lpLevel,CC_STATE_BLANK);
		return;
	}

	lpLevel->RemainTime = (int)difftime(ScheduleTime.GetTime(),time(0));

	lpLevel->TargetTime = (int)ScheduleTime.GetTime();

	LogAdd(LOG_BLACK,"[Chaos Castle] (%d) Sync Start Time. [%d] min remain",(lpLevel->Level+1),(lpLevel->RemainTime/60));
}

int CChaosCastle::GetState(int level) // OK
{
	if(CC_LEVEL_RANGE(level) == 0)
	{
		return CC_STATE_BLANK;
	}

	return this->m_ChaosCastleLevel[level].State;
}

int CChaosCastle::GetRemainTime(int level) // OK
{
	if(CC_LEVEL_RANGE(level) == 0)
	{
		return 0;
	}

	if(this->m_ChaosCastleStartTime.empty() != 0)
	{
		return 0;
	}

	CTime ScheduleTime;

	CScheduleManager ScheduleManager;

	for(std::vector<CHAOS_CASTLE_START_TIME>::iterator it = this->m_ChaosCastleStartTime.begin(); it != this->m_ChaosCastleStartTime.end(); it++)
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

int CChaosCastle::GetEnterEnabled(int level) // OK
{
	if(CC_LEVEL_RANGE(level) == 0)
	{
		return 0;
	}

	return this->m_ChaosCastleLevel[level].EnterEnabled;
}

int CChaosCastle::GetEnteredUserCount(int level) // OK
{
	if(CC_LEVEL_RANGE(level) == 0)
	{
		return 0;
	}

	return this->GetUserCount(&this->m_ChaosCastleLevel[level]);
}

bool CChaosCastle::AddUser(CHAOS_CASTLE_LEVEL* lpLevel,int aIndex) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	if(this->GetUser(lpLevel,aIndex) != 0)
	{
		return 0;
	}

	for(int n = 0; n < MAX_CC_USER; n++)
	{
		if(OBJECT_RANGE(lpLevel->User[n].Index) != 0)
		{
			continue;
		}

		lpLevel->User[n].Index = aIndex;
		lpLevel->User[n].Score = 0;
		lpLevel->User[n].UserKillCount = 0;
		lpLevel->User[n].MonsterKillCount = 0;
		lpLevel->User[n].RewardExperience = 0;
		gObjEventEntrySaveSend(&gObj[aIndex],1);
		return 1;
	}

	return 0;
}

bool CChaosCastle::DelUser(CHAOS_CASTLE_LEVEL* lpLevel,int aIndex) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	CHAOS_CASTLE_USER* lpUser = this->GetUser(lpLevel,aIndex);

	if(lpUser == 0)
	{
		return 0;
	}

	lpUser->Index = -1;
	lpUser->Score = 0;
	lpUser->UserKillCount = 0;
	lpUser->MonsterKillCount = 0;
	lpUser->RewardExperience = 0;
	return 1;
}

CHAOS_CASTLE_USER* CChaosCastle::GetUser(CHAOS_CASTLE_LEVEL* lpLevel,int aIndex) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	for(int n = 0; n < MAX_CC_USER; n++)
	{
		if(lpLevel->User[n].Index == aIndex)
		{
			return &lpLevel->User[n];
		}
	}

	return 0;
}

void CChaosCastle::CleanUser(CHAOS_CASTLE_LEVEL* lpLevel) // OK
{
	for(int n = 0; n < MAX_CC_USER; n++)
	{
		lpLevel->User[n].Index = -1;
		lpLevel->User[n].Score = 0;
		lpLevel->User[n].UserKillCount = 0;
		lpLevel->User[n].MonsterKillCount = 0;
		lpLevel->User[n].RewardExperience = 0;
	}
}

void CChaosCastle::ClearUser(CHAOS_CASTLE_LEVEL* lpLevel) // OK
{
	for(int n = 0; n < MAX_CC_USER; n++)
	{
		if(OBJECT_RANGE(lpLevel->User[n].Index) == 0)
		{
			continue;
		}

		gObjMoveGate(lpLevel->User[n].Index,22);

		lpLevel->User[n].Index = -1;
		lpLevel->User[n].Score = 0;
		lpLevel->User[n].UserKillCount = 0;
		lpLevel->User[n].MonsterKillCount = 0;
		lpLevel->User[n].RewardExperience = 0;
	}
}

void CChaosCastle::CheckUser(CHAOS_CASTLE_LEVEL* lpLevel) // OK
{
	for(int n = 0; n < MAX_CC_USER; n++)
	{
		if(OBJECT_RANGE(lpLevel->User[n].Index) == 0)
		{
			continue;
		}

		if(gObjIsConnected(lpLevel->User[n].Index) == 0)
		{
			this->DelUser(lpLevel,lpLevel->User[n].Index);
			continue;
		}

		if(gObj[lpLevel->User[n].Index].Map != lpLevel->Map)
		{
			this->DelUser(lpLevel,lpLevel->User[n].Index);
			continue;
		}
	}
}

int CChaosCastle::GetUserCount(CHAOS_CASTLE_LEVEL* lpLevel) // OK
{
	int count = 0;

	for(int n = 0; n < MAX_CC_USER; n++)
	{
		if(OBJECT_RANGE(lpLevel->User[n].Index) != 0)
		{
			count++;
		}
	}

	return count;
}

int CChaosCastle::GetUserWinner(CHAOS_CASTLE_LEVEL* lpLevel) // OK
{
	if(this->GetMonsterCount(lpLevel) > 0)
	{
		return -1;
	}

	int WinnerIndex = -1;
	int WinnerScore = -1;

	for(int n = 0; n < MAX_CC_USER; n++)
	{
		if(OBJECT_RANGE(lpLevel->User[n].Index) == 0)
		{
			continue;
		}

		if(lpLevel->User[n].Score > WinnerScore)
		{
			WinnerIndex = lpLevel->User[n].Index;
			WinnerScore = lpLevel->User[n].Score;
		}
	}

	return WinnerIndex;
}

void CChaosCastle::GiveUserRewardMoney(CHAOS_CASTLE_LEVEL* lpLevel) // OK
{
	if(gServerInfo.m_ChaosCastleMoneyRate == 0)
	{
		return;
	}

	int UserCount = 0;
	int WinnerIndex = -1;

	if((UserCount=this->GetUserCount(lpLevel)) == 0)
	{
		return;
	}

	CRandomManager RandomManager;

	for (int n = 0; n < MAX_CC_USER; n++)
	{
		if(OBJECT_RANGE(lpLevel->User[n].Index) != 0)
		{
			RandomManager.AddElement(lpLevel->User[n].Index,1);
		}
	}

	if(RandomManager.GetRandomElement(&WinnerIndex) == 0)
	{
		return;
	}

	if(OBJECT_RANGE(WinnerIndex) == 0)
	{
		return;
	}

	DWORD Money = UserCount*(gChaosCastleEnterMoney[lpLevel->Level]*gServerInfo.m_ChaosCastleMoneyRate)/100;

	if(gObjCheckMaxMoney(WinnerIndex,Money) == 0)
	{
		gObj[WinnerIndex].Money = MAX_MONEY;
	}
	else
	{
		gObj[WinnerIndex].Money += Money;
	}

	GCMoneySend(WinnerIndex,gObj[WinnerIndex].Money);

	this->NoticeSendToAll(lpLevel,1,gMessage.GetMessage(724),gObj[WinnerIndex].Name,Money);
}

void CChaosCastle::GiveUserRewardExperience(CHAOS_CASTLE_LEVEL* lpLevel,int aIndex) // OK
{
	CHAOS_CASTLE_USER* lpUser = this->GetUser(lpLevel,aIndex);

	if(lpUser == 0)
	{
		return;
	}

	lpUser->RewardExperience = (lpUser->UserKillCount*this->m_ChaosCastleExperienceTable[lpLevel->Level][0])+(lpUser->MonsterKillCount*this->m_ChaosCastleExperienceTable[lpLevel->Level][1]);

	lpUser->Score = lpUser->RewardExperience/5;

	lpUser->RewardExperience = lpUser->RewardExperience*gServerInfo.m_AddEventExperienceRate[gObj[lpUser->Index].AccountLevel];

	if(gEffectManager.CheckEffect(&gObj[lpUser->Index],EFFECT_SEAL_OF_SUSTENANCE1) != 0 || gEffectManager.CheckEffect(&gObj[lpUser->Index],EFFECT_SEAL_OF_SUSTENANCE2) != 0)
	{
		return;
	}

	if(gObjectManager.CharacterLevelUp(&gObj[lpUser->Index],lpUser->RewardExperience,gServerInfo.m_MaxLevelUpEvent,EXPERIENCE_CHAOS_CASTLE) != 0)
	{
		return;
	}

	GCRewardExperienceSend(lpUser->Index,lpUser->RewardExperience);
}

void CChaosCastle::GiveUserRewardItem(CHAOS_CASTLE_LEVEL* lpLevel,int aIndex) // OK
{
	CHAOS_CASTLE_USER* lpUser = this->GetUser(lpLevel,aIndex);

	if(lpUser == 0)
	{
		return;
	}

	gItemBagManager.DropItemBySpecialValue(ITEM_BAG_SPECIAL_CHAOS_CASTLE,lpLevel->Level,1,&gObj[lpUser->Index],gObj[lpUser->Index].Map,gObj[lpUser->Index].X,gObj[lpUser->Index].Y);
}

bool CChaosCastle::AddMonster(CHAOS_CASTLE_LEVEL* lpLevel,int aIndex) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	if(this->GetMonster(lpLevel,aIndex) != 0)
	{
		return 0;
	}

	for(int n = 0; n < MAX_CC_MONSTER; n++)
	{
		if(OBJECT_RANGE(lpLevel->MonsterIndex[n]) != 0)
		{
			continue;
		}

		lpLevel->MonsterIndex[n] = aIndex;
		return 1;
	}

	return 0;
}

bool CChaosCastle::DelMonster(CHAOS_CASTLE_LEVEL* lpLevel,int aIndex) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	int* index = this->GetMonster(lpLevel,aIndex);

	if(index == 0)
	{
		return 0;
	}

	(*index) = -1;
	return 1;
}

int* CChaosCastle::GetMonster(CHAOS_CASTLE_LEVEL* lpLevel,int aIndex) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	for(int n = 0; n < MAX_CC_MONSTER; n++)
	{
		if(lpLevel->MonsterIndex[n] == aIndex)
		{
			return &lpLevel->MonsterIndex[n];
		}
	}

	return 0;
}

void CChaosCastle::CleanMonster(CHAOS_CASTLE_LEVEL* lpLevel) // OK
{
	for(int n = 0; n < MAX_CC_MONSTER; n++)
	{
		lpLevel->MonsterIndex[n] = -1;
	}
}

void CChaosCastle::ClearMonster(CHAOS_CASTLE_LEVEL* lpLevel) // OK
{
	for(int n = 0; n < MAX_CC_MONSTER; n++)
	{
		if(OBJECT_RANGE(lpLevel->MonsterIndex[n]) != 0)
		{
			gObjDel(lpLevel->MonsterIndex[n]);
			lpLevel->MonsterIndex[n] = -1;
		}
	}
}

int CChaosCastle::GetMonsterCount(CHAOS_CASTLE_LEVEL* lpLevel) // OK
{
	int count = 0;

	for(int n = 0; n < MAX_CC_MONSTER; n++)
	{
		if(OBJECT_RANGE(lpLevel->MonsterIndex[n]) != 0)
		{
			count++;
		}
	}

	return count;
}

void CChaosCastle::StageSpawn(CHAOS_CASTLE_LEVEL* lpLevel,int Stage) // OK
{
	for(std::vector<EVENT_SPAWN_STAGE_INFO>::iterator it = gEventSpawnStage.m_EventSpawnStageInfo.begin(); it != gEventSpawnStage.m_EventSpawnStageInfo.end(); it++)
	{
		if(it->Event != 1 || it->Level != lpLevel->Level || it->Stage != Stage)
		{
			continue;
		}

		this->SetMonster(lpLevel,it->MonsterClass,it->MaxRegenTime);
	}
}

void CChaosCastle::SetMonster(CHAOS_CASTLE_LEVEL* lpLevel,int MonsterClass,int MaxRegenTime) // OK
{
	for(int n = 0; n < gMonsterSetBase.m_count; n++)
	{
		if(lpLevel->MaxMonster >= MAX_CC_MONSTER)
		{
			break;
		}

		MONSTER_SET_BASE_INFO* lpInfo = &gMonsterSetBase.m_MonsterSetBaseInfo[n];

		if(lpInfo->Type != 4 || lpInfo->MonsterClass != MonsterClass || lpInfo->Map != lpLevel->Map)
		{
			continue;
		}

		int index = gObjAddMonster(lpLevel->Map);

		if(OBJECT_RANGE(index) == 0)
		{
			continue;
		}

		LPOBJ lpObj = &gObj[index];

		if(gObjSetPosMonster(index,n) == 0)
		{
			gObjDel(index);
			continue;
		}

		if(gObjSetMonster(index,lpInfo->MonsterClass) == 0)
		{
			gObjDel(index);
			continue;
		}

		if(this->AddMonster(lpLevel,index) == 0)
		{
			gObjDel(index);
			continue;
		}

		lpLevel->MaxMonster++;
	}
}

void CChaosCastle::SetSafeZone(CHAOS_CASTLE_LEVEL* lpLevel) // OK
{
	for(int x = gChaosCastleSafeZone[0]; x <= gChaosCastleSafeZone[2]; x++)
	{
		for(int y = gChaosCastleSafeZone[1]; y <= gChaosCastleSafeZone[3]; y++)
		{
			gMap[lpLevel->Map].SetAttr(x,y,1);
		}
	}

	for(int n = 0; n < MAX_CC_USER; n++)
	{
		if(OBJECT_RANGE(lpLevel->User[n].Index) != 0)
		{
			GCMapAttrSend(lpLevel->User[n].Index,0,1,0,1,(PMSG_MAP_ATTR*)gChaosCastleSafeZone);
		}
	}
}

void CChaosCastle::DelSafeZone(CHAOS_CASTLE_LEVEL* lpLevel) // OK
{
	for(int x = gChaosCastleSafeZone[0]; x <= gChaosCastleSafeZone[2]; x++)
	{
		for(int y = gChaosCastleSafeZone[1]; y <= gChaosCastleSafeZone[3]; y++)
		{
			gMap[lpLevel->Map].DelAttr(x,y,1);
		}
	}

	for(int n = 0; n < MAX_CC_USER; n++)
	{
		if(OBJECT_RANGE(lpLevel->User[n].Index) != 0)
		{
			GCMapAttrSend(lpLevel->User[n].Index,0,1,1,1,(PMSG_MAP_ATTR*)gChaosCastleSafeZone);
		}
	}
}

void CChaosCastle::SetHollowZone(CHAOS_CASTLE_LEVEL* lpLevel,int stage) // OK
{
	for(int n = 0; n < MAX_CC_HOLLOW_ZONE; n++)
	{
		for(int x = gChaosCastleHollowZone[stage][n][0]; x <= gChaosCastleHollowZone[stage][n][2]; x++)
		{
			for(int y = gChaosCastleHollowZone[stage][n][1]; y <= gChaosCastleHollowZone[stage][n][3]; y++)
			{
				gMap[lpLevel->Map].SetAttr(x,y,8);
			}
		}
	}
}

void CChaosCastle::DelHollowZone(CHAOS_CASTLE_LEVEL* lpLevel,int stage) // OK
{
	for(int n = 0; n < MAX_CC_HOLLOW_ZONE; n++)
	{
		for(int x = gChaosCastleHollowZone[stage][n][0]; x <= gChaosCastleHollowZone[stage][n][2]; x++)
		{
			for(int y = gChaosCastleHollowZone[stage][n][1]; y <= gChaosCastleHollowZone[stage][n][3]; y++)
			{
				gMap[lpLevel->Map].DelAttr(x,y,8);
			}
		}
	}
}

void CChaosCastle::SetCenterHollowZone(CHAOS_CASTLE_LEVEL* lpLevel) // OK
{
	for(int n = 0; n < MAX_CC_CENTER_HOLLOW_ZONE; n++)
	{
		for(int x = gChaosCastleCenterHollowZone[n][0]; x <= gChaosCastleCenterHollowZone[n][2]; x++)
		{
			for(int y = gChaosCastleCenterHollowZone[n][1]; y <= gChaosCastleCenterHollowZone[n][3]; y++)
			{
				gMap[lpLevel->Map].SetAttr(x,y,8);
			}
		}
	}
}

void CChaosCastle::DelCenterHollowZone(CHAOS_CASTLE_LEVEL* lpLevel) // OK
{
	for(int n = 0; n < MAX_CC_CENTER_HOLLOW_ZONE; n++)
	{
		for(int x = gChaosCastleCenterHollowZone[n][0]; x <= gChaosCastleCenterHollowZone[n][2]; x++)
		{
			for(int y = gChaosCastleCenterHollowZone[n][1]; y <= gChaosCastleCenterHollowZone[n][3]; y++)
			{
				gMap[lpLevel->Map].SetAttr(x,y,8);
			}
		}
	}
}

void CChaosCastle::CheckUserPosition(CHAOS_CASTLE_LEVEL* lpLevel) // OK
{
	for(int n = 0; n < MAX_CC_USER; n++)
	{
		if(OBJECT_RANGE(lpLevel->User[n].Index) == 0)
		{
			continue;
		}

		if(gMap[lpLevel->Map].CheckAttr(gObj[lpLevel->User[n].Index].X,gObj[lpLevel->User[n].Index].Y,8) != 0)
		{
			gObj[lpLevel->User[n].Index].Life = 0;
			gObj[lpLevel->User[n].Index].State = OBJECT_DYING;
			gObj[lpLevel->User[n].Index].KillerType = 1;
			gObj[lpLevel->User[n].Index].RegenTime = GetTickCount();
			gObj[lpLevel->User[n].Index].DieRegen = 1;
			gObj[lpLevel->User[n].Index].PathCount = 0;

			GCUserDieSend(&gObj[lpLevel->User[n].Index],lpLevel->User[n].Index,0,0);

			this->GiveUserRewardExperience(lpLevel,lpLevel->User[n].Index);

			this->GCChaosCastleDelayScoreSend(lpLevel,lpLevel->User[n].Index,0);

			this->DelUser(lpLevel,lpLevel->User[n].Index);
		}
	}
}

void CChaosCastle::CheckMonsterPosition(CHAOS_CASTLE_LEVEL* lpLevel) // OK
{
	for(int n = 0; n < MAX_CC_MONSTER; n++)
	{
		if(OBJECT_RANGE(lpLevel->MonsterIndex[n]) == 0)
		{
			continue;
		}

		if(gMap[lpLevel->Map].CheckAttr(gObj[lpLevel->MonsterIndex[n]].X,gObj[lpLevel->MonsterIndex[n]].Y,8) != 0)
		{
			int x = gObj[lpLevel->MonsterIndex[n]].X;

			int y = gObj[lpLevel->MonsterIndex[n]].Y;

			if(this->GetFreePosition(lpLevel,&x,&y) != 0)
			{
				gObjSetPosition(lpLevel->MonsterIndex[n],x,y);

				gObj[lpLevel->MonsterIndex[n]].ChaosCastleBlowTime = GetTickCount();
			}
		}
	}
}

void CChaosCastle::CheckItemPosition(CHAOS_CASTLE_LEVEL* lpLevel) // OK
{
	for(int n = 0; n < MAX_MAP_ITEM; n++)
	{
		if(gMap[lpLevel->Map].m_Item[n].IsItem() == 0)
		{
			continue;
		}

		if(gMap[lpLevel->Map].CheckAttr(gMap[lpLevel->Map].m_Item[n].m_X,gMap[lpLevel->Map].m_Item[n].m_Y,8) != 0)
		{
			gMap[lpLevel->Map].m_Item[n].m_Live = 0;
			gMap[lpLevel->Map].m_Item[n].m_Give = 1;
			gMap[lpLevel->Map].m_Item[n].m_State = OBJECT_DIECMD;
		}
	}
}

void CChaosCastle::SearchUserAndBlow(CHAOS_CASTLE_LEVEL* lpLevel,int x,int y) // OK
{
	for(int n = 0; n < MAX_CC_USER; n++)
	{
		if(OBJECT_RANGE(lpLevel->User[n].Index) == 0)
		{
			continue;
		}

		if(gObj[lpLevel->User[n].Index].DieRegen != 0 || gObj[lpLevel->User[n].Index].Teleport != TELEPORT_NONE)
		{
			continue;
		}

		if(gObj[lpLevel->User[n].Index].X >= (x - 3) && gObj[lpLevel->User[n].Index].X <= (x+3) && gObj[lpLevel->User[n].Index].Y >= (y - 3) && gObj[lpLevel->User[n].Index].Y <= (y+3))
		{
			this->BlowUserFromPoint(lpLevel,lpLevel->User[n].Index,x,y);
		}
	}
}

void CChaosCastle::BlowUserFromPoint(CHAOS_CASTLE_LEVEL* lpLevel,int aIndex,int x,int y) // OK
{
	int px = gObj[aIndex].X - x;
	int py = gObj[aIndex].Y - y;

	int SignX = ((px<0) ? -1 : ((px>0) ? 1 : 0));

	if(SignX == 0)
	{
		SignX = (((GetLargeRand() % 2) == 0) ? 1 : -1);
	}

	int SignY = ((py<0) ? -1 : ((py>0) ? 1 : 0));

	if(SignY == 0)
	{
		SignY = (((GetLargeRand() % 2) == 0) ? 1 : -1);
	}

	int dis = (int)sqrt((float)(px*px)+(float)(py*py));

	int tx = gObj[aIndex].X+(((4 - dis)+(GetLargeRand() % 2))*SignX);
	int ty = gObj[aIndex].Y+(((4 - dis)+(GetLargeRand() % 2))*SignY);

	gObjSetPosition(aIndex,tx,ty);

	gObj[aIndex].ChaosCastleBlowTime = GetTickCount();
}

bool CChaosCastle::GetFreePosition(CHAOS_CASTLE_LEVEL* lpLevel,int* ox,int* oy) // OK
{
	for(int x = ((*ox) - 8); x < ((*ox)+8); x++)
	{
		for(int y = ((*oy) - 8); y < ((*oy)+8); y++)
		{
			if(gMap[lpLevel->Map].CheckAttr(x,y,2) == 0 && gMap[lpLevel->Map].CheckAttr(x,y,4) == 0 && gMap[lpLevel->Map].CheckAttr(x,y,8) == 0)
			{
				(*ox) = x;
				(*oy) = y;
				return 1;
			}
		}
	}

	return 0;
}

void CChaosCastle::UserDieProc(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	int level = GET_CC_LEVEL(lpObj->Map);

	if(CC_LEVEL_RANGE(level) == 0)
	{
		return;
	}

	if(this->GetState(level) != CC_STATE_START)
	{
		return;
	}

	if(this->GetUser(&this->m_ChaosCastleLevel[level],lpObj->Index) == 0)
	{
		return;
	}

	this->GiveUserRewardExperience(&this->m_ChaosCastleLevel[level],lpObj->Index);

	this->GCChaosCastleDelayScoreSend(&this->m_ChaosCastleLevel[level],lpObj->Index,0);

	this->DelUser(&this->m_ChaosCastleLevel[level],lpObj->Index);

	if(lpTarget->Type != OBJECT_USER)
	{
		return;
	}

	CHAOS_CASTLE_USER* lpUser = this->GetUser(&this->m_ChaosCastleLevel[level],lpTarget->Index);

	if(lpUser == 0)
	{
		return;
	}

	lpUser->Score += 1;

	lpUser->UserKillCount++;

	gQuestWorldObjective.QuestWorldObjectiveSpecial(lpTarget,QUEST_WORLD_OBJECTIVE_CHAOS_CASTLE_PLAYER_KILL,level+1);
}

void CChaosCastle::MonsterDieProc(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	int aIndex = gObjMonsterGetTopHitDamageUser(lpObj);

	if(OBJECT_RANGE(aIndex) != 0)
	{
		lpTarget = &gObj[aIndex];
	}

	int level = GET_CC_LEVEL(lpObj->Map);

	if(CC_LEVEL_RANGE(level) == 0)
	{
		return;
	}

	if(this->GetState(level) != CC_STATE_START)
	{
		return;
	}

	if(this->GetMonster(&this->m_ChaosCastleLevel[level],lpObj->Index) == 0)
	{
		return;
	}

	if((GetLargeRand() % 100) < gServerInfo.m_ChaosCastleBlowUserRate)
	{
		this->SearchUserAndBlow(&this->m_ChaosCastleLevel[level],lpObj->X,lpObj->Y);
	}

	this->DelMonster(&this->m_ChaosCastleLevel[level],lpObj->Index);

	if(lpTarget->Type != OBJECT_USER)
	{
		return;
	}

	CHAOS_CASTLE_USER* lpUser = this->GetUser(&this->m_ChaosCastleLevel[level],lpTarget->Index);

	if(lpUser == 0)
	{
		return;
	}

	lpUser->Score += 2;

	lpUser->MonsterKillCount++;

	gQuestWorldObjective.QuestWorldObjectiveSpecial(lpTarget,QUEST_WORLD_OBJECTIVE_CHAOS_CASTLE_MONSTER_KILL,level+1);
}

void CChaosCastle::CheckDelayScore(CHAOS_CASTLE_LEVEL* lpLevel) // OK
{
	for(std::vector<CHAOS_CASTLE_DELAY_SCORE>::iterator it = lpLevel->DelayScore.begin(); it != lpLevel->DelayScore.end();)
	{
		if((GetTickCount() - it->Delay) < 8000)
		{
			it++;
			continue;
		}

		if(gObjIsConnected(it->Index) != 0)
		{
			DataSend(it->Index,(BYTE*)&it->Score,it->Score.header.size);
		}

		it = lpLevel->DelayScore.erase(it);
	}
}

void CChaosCastle::NoticeSendToAll(CHAOS_CASTLE_LEVEL* lpLevel,int type,char* message,...) // OK
{
	char buff[256];

	va_list arg;
	va_start(arg,message);
	vsprintf_s(buff,message,arg);
	va_end(arg);

	for(int n = 0; n < MAX_CC_USER; n++)
	{
		if(OBJECT_RANGE(lpLevel->User[n].Index) != 0)
		{
			gNotice.GCNoticeSend(lpLevel->User[n].Index,type,0,0,0,0,0,buff);
		}
	}
}

void CChaosCastle::DataSendToAll(CHAOS_CASTLE_LEVEL* lpLevel,BYTE* lpMsg,int size) // OK
{
	for(int n = 0; n < MAX_CC_USER; n++)
	{
		if(OBJECT_RANGE(lpLevel->User[n].Index) != 0)
		{
			DataSend(lpLevel->User[n].Index,lpMsg,size);
		}
	}
}

void CChaosCastle::CGChaosCastleEnterRecv(PMSG_CHAOS_CASTLE_ENTER_RECV* lpMsg,int aIndex) // OK
{
	if(gServerInfo.m_ChaosCastleEvent == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	PMSG_CHAOS_CASTLE_ENTER_SEND pMsg;

	pMsg.header.set(0xAF,0x01,sizeof(pMsg));

	pMsg.result = 0;

	lpMsg->level = gEventEntryLevel.GetCCLevel(lpObj);

	if(CC_LEVEL_RANGE(lpMsg->level) == 0)
	{
		GCNewMessageSend(aIndex,0,0,gMessage.GetMessage(22));
		return;
	}

	if(gServerInfo.m_ChaosCastleMaxEntryCount[lpObj->AccountLevel] != 0 && lpObj->CCCount >= gServerInfo.m_ChaosCastleMaxEntryCount[lpObj->AccountLevel])
	{
		GCNewMessageSend(aIndex,0,0,gMessage.GetMessage(714),gServerInfo.m_ChaosCastleMaxEntryCount[lpObj->AccountLevel]);
		return;
	}

	CHAOS_CASTLE_LEVEL* lpLevel = &this->m_ChaosCastleLevel[lpMsg->level];

	if(INVENTORY_FULL_RANGE(lpMsg->slot) == 0)
	{
		pMsg.result = 1;
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	if(lpObj->Inventory[lpMsg->slot].m_Index != GET_ITEM(13,29))
	{
		pMsg.result = 1;
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	if(lpObj->Inventory[INVENTORY_SLOT_HELPER].m_Index == GET_ITEM(13,2) || lpObj->Inventory[INVENTORY_SLOT_HELPER].m_Index == GET_ITEM(13,3) || lpObj->Inventory[INVENTORY_SLOT_HELPER].m_Index == GET_ITEM(13,37))
	{
		GCNewMessageSend(aIndex,0,0,gMessage.GetMessage(159));
		return;
	}

	if(OBJECT_RANGE(lpObj->PartyNumber) != 0)
	{
		pMsg.result = 1;
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	if(lpObj->Guild != 0 && lpObj->Guild->WarState == 1)
	{
		pMsg.result = 1;
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	if(OBJECT_RANGE(lpObj->DuelUser) != 0)
	{
		pMsg.result = 1;
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	if(lpObj->PShopOpen != 0)
	{
		pMsg.result = 1;
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	if(lpObj->Change != -1)
	{
		GCNewMessageSend(aIndex,0,0,gMessage.GetMessage(159));
		return;
	}

	if(this->GetEnterEnabled(lpMsg->level) == 0)
	{
		pMsg.result = 2;
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	if(gMapServerManager.CheckMapServer(lpLevel->Map) == 0)
	{
		pMsg.result = 2;
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	if(lpObj->Money < ((DWORD)gChaosCastleEnterMoney[lpMsg->level]))
	{
		pMsg.result = 7;
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	if(gServerInfo.m_PKLimitFree == 0 && lpObj->PKLevel > 4 && gServerInfo.m_PKLimitEventEntry != 0)
	{
		pMsg.result = 8;
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	if(this->AddUser(lpLevel,aIndex) == 0)
	{
		pMsg.result = 5;
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	lpObj->Money -= gChaosCastleEnterMoney[lpMsg->level];
	GCMoneySend(aIndex,lpObj->Money);

	gItemManager.InventoryDelItem(aIndex,lpMsg->slot);
	gItemManager.GCItemDeleteSend(aIndex,lpMsg->slot,1);

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);

	if(OBJECT_RANGE(lpObj->SummonIndex) != 0)
	{
		gObjSummonKill(aIndex);
		GCSummonLifeSend(aIndex,0,1);
	}

	lpObj->Interface.use = 0;
	lpObj->Interface.type = 0;
	lpObj->Interface.state = 0;

	switch (lpMsg->level)
	{
	case 0:
		gObjMoveGate(aIndex,82);
		break;
	case 1:
		gObjMoveGate(aIndex,83);
		break;
	case 2:
		gObjMoveGate(aIndex,84);
		break;
	case 3:
		gObjMoveGate(aIndex,85);
		break;
	case 4:
		gObjMoveGate(aIndex,86);
		break;
	case 5:
		gObjMoveGate(aIndex,87);
		break;
	case 6:
		gObjMoveGate(aIndex,272);
		break;
	}
}

void CChaosCastle::CGChaosCastlePositionRecv(PMSG_CHAOS_CASTLE_POSITION_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	int level = GET_CC_LEVEL(lpObj->Map);

	if(CC_LEVEL_RANGE(level) == 0)
	{
		return;
	}

	CHAOS_CASTLE_USER* lpUser = this->GetUser(&this->m_ChaosCastleLevel[level],aIndex);

	if(lpUser == 0)
	{
		return;
	}

	gObjSetPosition(lpUser->Index,lpMsg->x,lpMsg->y);

	lpObj->ChaosCastleBlowTime = GetTickCount();
}

void CChaosCastle::GCChaosCastleScoreSend(CHAOS_CASTLE_LEVEL* lpLevel,int aIndex,int type) // OK
{
	CHAOS_CASTLE_USER* lpUser = this->GetUser(lpLevel,aIndex);

	if(lpUser == 0)
	{
		return;
	}

	PMSG_CHAOS_CASTLE_SCORE_SEND pMsg;

	pMsg.header.set(0x93,sizeof(pMsg));

	pMsg.type = type;

	pMsg.flag = 0xFE;

	memcpy(pMsg.name,gObj[lpUser->Index].Name,sizeof(pMsg.name));

	pMsg.MonsterKillCount = lpUser->MonsterKillCount;

	pMsg.RewardExperience = lpUser->RewardExperience;

	pMsg.UserKillCount = lpUser->UserKillCount;

	DataSend(lpUser->Index,(BYTE*)&pMsg,pMsg.header.size);
}

void CChaosCastle::GCChaosCastleDelayScoreSend(CHAOS_CASTLE_LEVEL* lpLevel,int aIndex,int type) // OK
{
	CHAOS_CASTLE_USER* lpUser = this->GetUser(lpLevel,aIndex);

	if(lpUser == 0)
	{
		return;
	}

	CHAOS_CASTLE_DELAY_SCORE DelayScore;

	DelayScore.Delay = GetTickCount();

	DelayScore.Index = lpUser->Index;

	DelayScore.Score.header.set(0x93,sizeof(DelayScore.Score));

	DelayScore.Score.type = type;

	DelayScore.Score.flag = 0xFE;

	memcpy(DelayScore.Score.name,gObj[lpUser->Index].Name,sizeof(DelayScore.Score.name));

	DelayScore.Score.MonsterKillCount = lpUser->MonsterKillCount;

	DelayScore.Score.RewardExperience = lpUser->RewardExperience;

	DelayScore.Score.UserKillCount = lpUser->UserKillCount;

	lpLevel->DelayScore.push_back(DelayScore);
}

void CChaosCastle::GCChaosCastleStateSend(CHAOS_CASTLE_LEVEL* lpLevel,int state) // OK
{
	PMSG_CHAOS_CASTLE_STATE_SEND pMsg;

	pMsg.header.set(0x9B,sizeof(pMsg));

	pMsg.state = state;

	pMsg.time = lpLevel->RemainTime;

	pMsg.MaxMonster = lpLevel->MaxMonster;

	pMsg.CurMonster = this->GetUserCount(lpLevel)+this->GetMonsterCount(lpLevel);

	pMsg.EventItemOwner = 0;

	pMsg.EventItemLevel = 0xFF;

	this->DataSendToAll(lpLevel,(BYTE*)&pMsg,pMsg.header.size);
}

void CChaosCastle::GCChaosCastleSimpleStateSend(CHAOS_CASTLE_LEVEL* lpLevel,int state,int aIndex) // OK
{
	PMSG_CHAOS_CASTLE_STATE_SEND pMsg;

	pMsg.header.set(0x9B,sizeof(pMsg));

	pMsg.state = state;

	pMsg.time = 0;

	pMsg.MaxMonster = 0;

	pMsg.CurMonster = 0;

	pMsg.EventItemOwner = 0;

	pMsg.EventItemLevel = 0xFF;

	if(OBJECT_RANGE(aIndex) == 0)
	{
		this->DataSendToAll(lpLevel,(BYTE*)&pMsg,pMsg.header.size);
	}
	else
	{
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
	}
}

void CChaosCastle::ForceStart() // OK
{
	SYSTEMTIME SystemTime;

	GetLocalTime(&SystemTime);

	CHAOS_CASTLE_START_TIME info;

	info.Year = SystemTime.wYear;

	info.Month = SystemTime.wMonth;

	info.Day = SystemTime.wDay;

	info.DayOfWeek = -1;

	info.Hour = SystemTime.wHour;

	info.Minute = (SystemTime.wMinute+1);

	info.Second = SystemTime.wSecond;

	this->m_ChaosCastleStartTime.push_back(info);

	this->Init();
}