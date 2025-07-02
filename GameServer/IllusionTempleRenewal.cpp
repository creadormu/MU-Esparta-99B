// IllusionTempleRenewal.cpp: implementation of the CIllusionTempleRenewal class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IllusionTempleRenewal.h"
#include "DSProtocol.h"
#include "EffectManager.h"
#include "EventEntryLevel.h"
#include "EventSpawnStage.h"
#include "Gate.h"
#include "GuildManager.h"
#include "IllusionTemple.h"
#include "ItemBagManager.h"
#include "Map.h"
#include "MapServerManager.h"
#include "MemScript.h"
#include "Message.h"
#include "Monster.h"
#include "MonsterSetBase.h"
#include "Notice.h"
#include "NpcTalk.h"
#include "ObjectManager.h"
#include "Party.h"
#include "ScheduleManager.h"
#include "ServerInfo.h"
#include "SkillManager.h"
#include "Util.h"
#include "Viewport.h"

CIllusionTempleRenewal gIllusionTempleRenewal;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIllusionTempleRenewal::CIllusionTempleRenewal() // OK
{
	#if(GAMESERVER_UPDATE>=803)

	for(int n = 0; n < MAX_IT_LEVEL; n++)
	{
		ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel = &this->m_IllusionTempleRenewalLevel[n];

		lpLevel->Level = n;
		lpLevel->State = ITR_STATE_BLANK;
		lpLevel->Map = MAP_ILLUSION_TEMPLE1+n;
		lpLevel->RemainTime = 0;
		lpLevel->TargetTime = 0;
		lpLevel->TickCount = GetTickCount();
		lpLevel->EnterEnabled = 0;
		lpLevel->MinutesLeft = -1;
		lpLevel->TimeCount = 0;
		lpLevel->Score[ITR_TEAM_Y] = 0;
		lpLevel->Score[ITR_TEAM_B] = 0;
		lpLevel->EventItemSerial = -1;
		lpLevel->EventItemNumber = -1;
		lpLevel->EventItemOwner = -1;
		lpLevel->EventItemLevel = -1;
		lpLevel->EventItemStatus = 0;
		lpLevel->PartyNumber[ITR_TEAM_Y] = -1;
		lpLevel->PartyNumber[ITR_TEAM_B] = -1;
		lpLevel->WinnerTeam = ITR_TEAM_NONE;

		this->CleanUser(lpLevel);

		lpLevel->StoneStatueIndex = -1;

		this->CleanMonster(lpLevel);
	}

	#endif
}

CIllusionTempleRenewal::~CIllusionTempleRenewal() // OK
{

}

void CIllusionTempleRenewal::Init() // OK
{
	#if(GAMESERVER_UPDATE>=803)

	for(int n = 0; n < MAX_IT_LEVEL; n++)
	{
		if(gServerInfo.m_IllusionTempleEvent == 0)
		{
			this->SetState(&this->m_IllusionTempleRenewalLevel[n],ITR_STATE_BLANK);
		}
		else
		{
			this->SetState(&this->m_IllusionTempleRenewalLevel[n],ITR_STATE_EMPTY);
		}
	}

	#endif
}

void CIllusionTempleRenewal::Load(char* path) // OK
{
	#if(GAMESERVER_UPDATE>=803)

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

	this->m_IllusionTempleRenewalStartTime.clear();

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

					ILLUSION_TEMPLE_RENEWAL_START_TIME info;

					info.Year = lpMemScript->GetNumber();

					info.Month = lpMemScript->GetAsNumber();

					info.Day = lpMemScript->GetAsNumber();

					info.DayOfWeek = lpMemScript->GetAsNumber();

					info.Hour = lpMemScript->GetAsNumber();

					info.Minute = lpMemScript->GetAsNumber();

					info.Second = lpMemScript->GetAsNumber();

					this->m_IllusionTempleRenewalStartTime.push_back(info);
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

	#endif
}

void CIllusionTempleRenewal::MainProc() // OK
{
	#if(GAMESERVER_UPDATE>=803)

	for(int n = 0; n < MAX_IT_LEVEL; n++)
	{
		ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel = &this->m_IllusionTempleRenewalLevel[n];

		if((GetTickCount()-lpLevel->TickCount) >= 1000)
		{
			lpLevel->TickCount = GetTickCount();

			lpLevel->RemainTime = (int)difftime(lpLevel->TargetTime,time(0));

			switch(lpLevel->State)
			{
				case ITR_STATE_BLANK:
					this->ProcState_BLANK(lpLevel);
					break;
				case ITR_STATE_EMPTY:
					this->ProcState_EMPTY(lpLevel);
					break;
				case ITR_STATE_STAND:
					this->ProcState_STAND(lpLevel);
					break;
				case ITR_STATE_START:
					this->ProcState_START(lpLevel);
					break;
				case ITR_STATE_CLEAN:
					this->ProcState_CLEAN(lpLevel);
					break;
			}
		}
	}

	#endif
}

void CIllusionTempleRenewal::ProcState_BLANK(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel) // OK
{

}

void CIllusionTempleRenewal::ProcState_EMPTY(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel) // OK
{
	#if(GAMESERVER_UPDATE>=803)

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
				gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(40),(lpLevel->MinutesLeft+1));
			}
		}

		lpLevel->EnterEnabled = 1;
	}

	if(lpLevel->RemainTime > 0 && lpLevel->RemainTime <= 30 && lpLevel->TimeCount == 0)
	{
		PMSG_TIME_COUNT_SEND pMsg;

		pMsg.header.set(0x92,sizeof(pMsg));

		pMsg.type = 14;

		DataSendAll((BYTE*)&pMsg,pMsg.header.size);

		lpLevel->TimeCount = 1;
	}

	if(lpLevel->RemainTime <= 0)
	{
		if(lpLevel->Level == 0) { gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(41)); }
		this->NoticeSendToAll(lpLevel,1,gMessage.GetMessage(42),(lpLevel->Level+1),this->m_NotifyTime);
		this->SetState(lpLevel,ITR_STATE_STAND);
	}

	#endif
}

void CIllusionTempleRenewal::ProcState_STAND(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	this->CheckUser(lpLevel);

	if(this->GetUserCount(lpLevel,ITR_TEAM_Y) < (gServerInfo.m_IllusionTempleMinUser/2) || this->GetUserCount(lpLevel,ITR_TEAM_B) < (gServerInfo.m_IllusionTempleMinUser/2))
	{
		LogAdd(LOG_BLACK,"[Illusion Temple Renewal] (%d) Not enough users",(lpLevel->Level+1));
		this->NoticeSendToAll(lpLevel,0,gMessage.GetMessage(49));
		this->SetState(lpLevel,ITR_STATE_EMPTY);
		return;
	}

	if(lpLevel->RemainTime > 0 && lpLevel->RemainTime <= 30 && lpLevel->TimeCount == 0)
	{
		PMSG_TIME_COUNT_SEND pMsg;

		pMsg.header.set(0x92,sizeof(pMsg));

		pMsg.type = 15;

		this->DataSendToAll(lpLevel,(BYTE*)&pMsg,pMsg.header.size);

		lpLevel->TimeCount = 1;
	}

	if(lpLevel->RemainTime <= 0)
	{
		this->NoticeSendToAll(lpLevel,0,gMessage.GetMessage(43));

		this->SetState(lpLevel,ITR_STATE_START);
	}

	#endif
}

void CIllusionTempleRenewal::ProcState_START(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	this->CheckUser(lpLevel);

	if(this->GetUserCount(lpLevel,ITR_TEAM_Y) == 0 || this->GetUserCount(lpLevel,ITR_TEAM_B) == 0)
	{
		LogAdd(LOG_BLACK,"[Illusion Temple Renewal] (%d) Not enough users",(lpLevel->Level+1));
		this->SetState(lpLevel,ITR_STATE_CLEAN);
		return;
	}

	this->GCIllusionTempleRenewalBattleInfoSend(lpLevel);

	if(lpLevel->Score[ITR_TEAM_Y] >= 7 || lpLevel->Score[ITR_TEAM_B] >= 7)
	{
		this->SetState(lpLevel,ITR_STATE_CLEAN);
		return;
	}

	if(lpLevel->RemainTime <= 0)
	{
		this->NoticeSendToAll(lpLevel,0,gMessage.GetMessage(44));

		this->SetState(lpLevel,ITR_STATE_CLEAN);
	}

	#endif
}

void CIllusionTempleRenewal::ProcState_CLEAN(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	this->CheckUser(lpLevel);

	if(lpLevel->RemainTime <= 0)
	{
		this->SetState(lpLevel,ITR_STATE_EMPTY);
	}

	#endif
}

void CIllusionTempleRenewal::SetState(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel,int state) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	lpLevel->State = state;

	switch (lpLevel->State)
	{
	case ITR_STATE_BLANK:
		this->SetState_BLANK(lpLevel);
		break;
	case ITR_STATE_EMPTY:
		this->SetState_EMPTY(lpLevel);
		break;
	case ITR_STATE_STAND:
		this->SetState_STAND(lpLevel);
		break;
	case ITR_STATE_START:
		this->SetState_START(lpLevel);
		break;
	case ITR_STATE_CLEAN:
		this->SetState_CLEAN(lpLevel);
		break;
	}

	#endif
}

void CIllusionTempleRenewal::SetState_BLANK(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel) // OK
{
	
}

void CIllusionTempleRenewal::SetState_EMPTY(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	lpLevel->EnterEnabled = 0;
	lpLevel->MinutesLeft = -1;
	lpLevel->TimeCount = 0;
	lpLevel->Score[ITR_TEAM_Y] = 0;
	lpLevel->Score[ITR_TEAM_B] = 0;
	lpLevel->EventItemSerial = -1;
	lpLevel->EventItemNumber = -1;
	lpLevel->EventItemOwner = -1;
	lpLevel->EventItemLevel = -1;
	lpLevel->EventItemStatus = 1;
	lpLevel->WinnerTeam = ITR_TEAM_NONE;

	if(OBJECT_RANGE(lpLevel->PartyNumber[ITR_TEAM_Y]) != 0)
	{
		gParty.Destroy(lpLevel->PartyNumber[ITR_TEAM_Y]);
		lpLevel->PartyNumber[ITR_TEAM_Y] = -1;
	}

	if(OBJECT_RANGE(lpLevel->PartyNumber[ITR_TEAM_B]) != 0)
	{
		gParty.Destroy(lpLevel->PartyNumber[ITR_TEAM_B]);
		lpLevel->PartyNumber[ITR_TEAM_B] = -1;
	}

	this->GCIllusionTempleRenewalChangeScenarioSend(lpLevel,0,-1);

	this->ClearUser(lpLevel);

	if(OBJECT_RANGE(lpLevel->StoneStatueIndex) != 0)
	{
		gObjDel(lpLevel->StoneStatueIndex);
		lpLevel->StoneStatueIndex = -1;
	}

	this->ClearMonster(lpLevel);

	this->CheckSync(lpLevel);

	LogAdd(LOG_BLACK,"[Illusion Temple Renewal] (%d) SetState EMPTY",(lpLevel->Level+1));

	#endif
}

void CIllusionTempleRenewal::SetState_STAND(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	lpLevel->EnterEnabled = 0;
	lpLevel->MinutesLeft = -1;
	lpLevel->TimeCount = 0;

	int team = GetLargeRand() % MAX_IT_TEAM;

	for(int n = 0; n < MAX_IT_USER; n++)
	{
		if(OBJECT_RANGE(lpLevel->User[n].Index) != 0)
		{
			lpLevel->User[n].Team = team;
			gObjMoveGate(lpLevel->User[n].Index,148+(team*6)+lpLevel->Level);
			gObj[lpLevel->User[n].Index].Change = 404+team;
			gObjViewportListProtocolCreate(&gObj[lpLevel->User[n].Index]);

			if(OBJECT_RANGE(lpLevel->PartyNumber[team]) == 0)
			{
				if(gParty.Create(lpLevel->User[n].Index) != 0)
				{
					lpLevel->PartyNumber[team] = gObj[lpLevel->User[n].Index].PartyNumber;
				}
			}
			else
			{
				if(gParty.AddMember(lpLevel->PartyNumber[team],lpLevel->User[n].Index) != 0)
				{
					lpLevel->PartyNumber[team] = gObj[lpLevel->User[n].Index].PartyNumber;
				}
			}

			team ^= 1;
		}
	}

	this->GCIllusionTempleRenewalChangeScenarioSend(lpLevel,1,-1);

	lpLevel->RemainTime = this->m_NotifyTime*60;

	lpLevel->TargetTime = (int)(time(0)+lpLevel->RemainTime);

	LogAdd(LOG_BLACK,"[Illusion Temple Renewal] (%d) SetState STAND",(lpLevel->Level+1));

	#endif
}

void CIllusionTempleRenewal::SetState_START(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	lpLevel->EnterEnabled = 0;
	lpLevel->MinutesLeft = -1;
	lpLevel->TimeCount = 0;

	this->SetStoneStatue(lpLevel);

	this->StageSpawn(lpLevel,0);

	this->GCIllusionTempleRenewalChangeScenarioSend(lpLevel,2,-1);

	lpLevel->RemainTime = this->m_EventTime*60;

	lpLevel->TargetTime = (int)(time(0)+lpLevel->RemainTime);

	LogAdd(LOG_BLACK,"[Illusion Temple Renewal] (%d) SetState START",(lpLevel->Level+1));

	#endif
}

void CIllusionTempleRenewal::SetState_CLEAN(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	lpLevel->EnterEnabled = 0;
	lpLevel->MinutesLeft = -1;
	lpLevel->TimeCount = 0;

	if(OBJECT_RANGE(lpLevel->EventItemOwner) != 0)
	{
		this->SearchUserDeleteEventItem(lpLevel->EventItemOwner);
	}

	if(lpLevel->Score[ITR_TEAM_Y] > lpLevel->Score[ITR_TEAM_B])
	{
		lpLevel->WinnerTeam = ITR_TEAM_Y;
	}
	else if(lpLevel->Score[ITR_TEAM_B] > lpLevel->Score[ITR_TEAM_Y])
	{
		lpLevel->WinnerTeam = ITR_TEAM_B;
	}
	else
	{
		
	}

	for(int n = 0; n < MAX_IT_USER; n++)
	{
		if(OBJECT_RANGE(lpLevel->User[n].Index) != 0)
		{
			gObjMoveGate(lpLevel->User[n].Index,148+(lpLevel->User[n].Team*6)+lpLevel->Level);
		}
	}

	if(OBJECT_RANGE(lpLevel->StoneStatueIndex) != 0)
	{
		gObjDel(lpLevel->StoneStatueIndex);
		lpLevel->StoneStatueIndex = -1;
	}

	this->ClearMonster(lpLevel);

	this->GCIllusionTempleRenewalChangeScenarioSend(lpLevel,3,-1);

	this->GCIllusionTempleRenewalRewardScoreSend(lpLevel);

	lpLevel->RemainTime = this->m_CloseTime*60;

	lpLevel->TargetTime = (int)(time(0)+lpLevel->RemainTime);

	LogAdd(LOG_BLACK,"[Illusion Temple Renewal] (%d) SetState CLEAN",(lpLevel->Level+1));

	#endif
}

void CIllusionTempleRenewal::CheckSync(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	if(this->m_IllusionTempleRenewalStartTime.empty() != 0)
	{
		this->SetState(lpLevel,ITR_STATE_BLANK);
		return;
	}

	CTime ScheduleTime;

	CScheduleManager ScheduleManager;

	for(std::vector<ILLUSION_TEMPLE_RENEWAL_START_TIME>::iterator it = this->m_IllusionTempleRenewalStartTime.begin(); it != this->m_IllusionTempleRenewalStartTime.end(); it++)
	{
		ScheduleManager.AddSchedule(it->Year,it->Month,it->Day,it->Hour,it->Minute,it->Second,it->DayOfWeek);
	}

	if(ScheduleManager.GetSchedule(&ScheduleTime) == 0)
	{
		this->SetState(lpLevel,ITR_STATE_BLANK);
		return;
	}

	lpLevel->RemainTime = (int)difftime(ScheduleTime.GetTime(),time(0));

	lpLevel->TargetTime = (int)ScheduleTime.GetTime();

	LogAdd(LOG_BLACK,"[Illusion Temple Renewal] (%d) Sync Start Time. [%d] min remain",(lpLevel->Level+1),(lpLevel->RemainTime/60));

	#endif
}

int CIllusionTempleRenewal::GetState(int level) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	if(IT_LEVEL_RANGE(level) == 0)
	{
		return ITR_STATE_BLANK;
	}

	return this->m_IllusionTempleRenewalLevel[level].State;

	#else

	return 0;

	#endif
}

int CIllusionTempleRenewal::GetRemainTime(int level) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	if(IT_LEVEL_RANGE(level) == 0)
	{
		return 0;
	}

	if(this->m_IllusionTempleRenewalStartTime.empty() != 0)
	{
		return 0;
	}

	CTime ScheduleTime;

	CScheduleManager ScheduleManager;

	for(std::vector<ILLUSION_TEMPLE_RENEWAL_START_TIME>::iterator it = this->m_IllusionTempleRenewalStartTime.begin(); it != this->m_IllusionTempleRenewalStartTime.end(); it++)
	{
		ScheduleManager.AddSchedule(it->Year,it->Month,it->Day,it->Hour,it->Minute,it->Second,it->DayOfWeek);
	}

	if(ScheduleManager.GetSchedule(&ScheduleTime) == 0)
	{
		return 0;
	}

	int RemainTime = (int)difftime(ScheduleTime.GetTime(),time(0));

	return (((RemainTime % 60) == 0) ? (RemainTime/60) : ((RemainTime/60)+1));

	#else

	return 0;

	#endif
}

int CIllusionTempleRenewal::GetEnterEnabled(int level) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	if(IT_LEVEL_RANGE(level) == 0)
	{
		return 0;
	}

	return this->m_IllusionTempleRenewalLevel[level].EnterEnabled;

	#else

	return 0;

	#endif
}

int CIllusionTempleRenewal::GetEnteredUserCount(int level) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	if(IT_LEVEL_RANGE(level) == 0)
	{
		return 0;
	}

	return this->GetUserCount(&this->m_IllusionTempleRenewalLevel[level],ITR_TEAM_NONE);

	#else

	return 0;

	#endif
}

bool CIllusionTempleRenewal::AddUser(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel,int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	if(this->GetUser(lpLevel,aIndex) != 0)
	{
		return 0;
	}

	for(int n = 0; n < MAX_IT_USER; n++)
	{
		if(OBJECT_RANGE(lpLevel->User[n].Index) != 0)
		{
			continue;
		}

		lpLevel->User[n].Index = aIndex;
		lpLevel->User[n].Team = ITR_TEAM_NONE;
		lpLevel->User[n].Point = 0;
		lpLevel->User[n].Score = 0;
		return 1;
	}

	return 0;

	#else

	return 0;

	#endif
}

bool CIllusionTempleRenewal::DelUser(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel,int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	ILLUSION_TEMPLE_RENEWAL_USER* lpUser = this->GetUser(lpLevel,aIndex);

	if(lpUser == 0)
	{
		return 0;
	}

	if(lpLevel->State == ITR_STATE_START)
	{
		this->SearchUserDropEventItem(aIndex);
	}
	else
	{
		this->SearchUserDeleteEventItem(aIndex);
	}

	if(OBJECT_RANGE(lpLevel->PartyNumber[lpUser->Team]) != 0)
	{
		gParty.DelMember(lpLevel->PartyNumber[lpUser->Team],aIndex);
		lpLevel->PartyNumber[lpUser->Team] = ((gParty.IsParty(lpLevel->PartyNumber[lpUser->Team]) == 0) ? -1 : lpLevel->PartyNumber[lpUser->Team]);
	}

	if(gObj[aIndex].Change >= 0)
	{
		gObj[aIndex].Change = -1;
		gObjViewportListProtocolCreate(&gObj[aIndex]);
	}

	lpUser->Index = -1;
	lpUser->Team = ITR_TEAM_NONE;
	lpUser->Point = 0;
	lpUser->Score = 0;
	return 1;
	
	#else

	return 0;

	#endif
}

ILLUSION_TEMPLE_RENEWAL_USER* CIllusionTempleRenewal::GetUser(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel,int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	for(int n = 0; n < MAX_IT_USER; n++)
	{
		if(lpLevel->User[n].Index == aIndex)
		{
			return &lpLevel->User[n];
		}
	}

	return 0;

	#else

	return 0;

	#endif
}

void CIllusionTempleRenewal::CleanUser(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	for(int n = 0; n < MAX_IT_USER; n++)
	{
		lpLevel->User[n].Index = -1;
		lpLevel->User[n].Team = ITR_TEAM_NONE;
		lpLevel->User[n].Point = 0;
		lpLevel->User[n].Score = 0;
	}

	#endif
}

void CIllusionTempleRenewal::ClearUser(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	for(int n = 0; n < MAX_IT_USER; n++)
	{
		if(OBJECT_RANGE(lpLevel->User[n].Index) == 0)
		{
			continue;
		}

		if(gObj[lpLevel->User[n].Index].Change >= 0)
		{
			gObj[lpLevel->User[n].Index].Change = -1;
			gObjViewportListProtocolCreate(&gObj[lpLevel->User[n].Index]);
		}

		gObjMoveGate(lpLevel->User[n].Index,267);

		lpLevel->User[n].Index = -1;
		lpLevel->User[n].Team = ITR_TEAM_NONE;
		lpLevel->User[n].Point = 0;
		lpLevel->User[n].Score = 0;
	}

	#endif
}

void CIllusionTempleRenewal::CheckUser(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	for(int n = 0; n < MAX_IT_USER; n++)
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

	#endif
}

int CIllusionTempleRenewal::GetUserCount(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel,int team) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	int count = 0;

	for(int n = 0; n < MAX_IT_USER; n++)
	{
		if(OBJECT_RANGE(lpLevel->User[n].Index) == 0)
		{
			continue;
		}

		if(team == ITR_TEAM_NONE || lpLevel->User[n].Team == team)
		{
			count++;
		}
	}

	return count;

	#else

	return 0;

	#endif
}

int CIllusionTempleRenewal::GetUserJoinTeam(LPOBJ lpObj) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	int level = GET_IT_LEVEL(lpObj->Map);

	if(IT_LEVEL_RANGE(level) == 0)
	{
		return -1;
	}

	ILLUSION_TEMPLE_RENEWAL_USER* lpUser = this->GetUser(&this->m_IllusionTempleRenewalLevel[level],lpObj->Index);

	if(lpUser == 0)
	{
		return -1;
	}

	return lpUser->Team;

	#else

	return -1;

	#endif
}

bool CIllusionTempleRenewal::GetUserRespawnLocation(LPOBJ lpObj,int* gate,int* map,int* x,int* y,int* dir,int* level) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	int IllusionTempleRenewalLevel = GET_IT_LEVEL(lpObj->Map);

	if(IT_LEVEL_RANGE(IllusionTempleRenewalLevel) == 0)
	{
		return 0;
	}

	ILLUSION_TEMPLE_RENEWAL_USER* lpUser = this->GetUser(&this->m_IllusionTempleRenewalLevel[IllusionTempleRenewalLevel],lpObj->Index);

	if(lpUser != 0 && lpUser->Team != ITR_TEAM_NONE)
	{
		if(gGate.GetGate(((148+(lpUser->Team*6))+IllusionTempleRenewalLevel),gate,map,x,y,dir,level) == 0)
		{
			return 0;
		}
	}
	else
	{
		if(gGate.GetGate(267,gate,map,x,y,dir,level) == 0)
		{
			return 0;
		}
	}

	return 1;

	#else

	return 0;

	#endif
}

void CIllusionTempleRenewal::SearchUserDropEventItem(int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	LPOBJ lpObj = &gObj[aIndex];

	int level = GET_IT_LEVEL(lpObj->Map);

	if(IT_LEVEL_RANGE(level) == 0)
	{
		return;
	}

	ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel = &this->m_IllusionTempleRenewalLevel[level];

	if(this->GetUser(lpLevel,aIndex) == 0)
	{
		return;
	}

	if(lpLevel->EventItemOwner != aIndex)
	{
		return;
	}

	int slot = gItemManager.GetInventoryItemSlot(lpObj,GET_ITEM(14,64),lpLevel->EventItemLevel);

	if(slot == -1)
	{
		return;
	}

	if(lpObj->Inventory[slot].m_Serial != lpLevel->EventItemSerial)
	{
		return;
	}

	PMSG_ITEM_DROP_SEND pMsg;

	pMsg.header.set(0x23,sizeof(pMsg));

	pMsg.result = 0;

	pMsg.slot = slot;

	CItem* lpItem = &gObj[aIndex].Inventory[slot];

	if(gMap[lpObj->Map].ItemDrop(lpItem->m_Index,lpItem->m_Level,lpItem->m_Durability,lpObj->X,lpObj->Y,lpItem->m_Option1,lpItem->m_Option2,lpItem->m_Option3,lpItem->m_NewOption,lpItem->m_SetOption,lpItem->m_Serial,aIndex,lpItem->m_PetItemLevel,lpItem->m_PetItemExp,lpItem->m_JewelOfHarmonyOption,lpItem->m_ItemOptionEx,lpItem->m_SocketOption,lpItem->m_SocketOptionBonus,lpItem->m_PeriodicItemTime) != 0)
	{
		pMsg.result = 1;
		gItemManager.InventoryDelItem(aIndex,slot);
		gItemManager.UpdateInventoryViewport(aIndex,slot);
	}

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);

	#endif
}

void CIllusionTempleRenewal::SearchUserDeleteEventItem(int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	LPOBJ lpObj = &gObj[aIndex];

	int level = GET_IT_LEVEL(lpObj->Map);

	if(IT_LEVEL_RANGE(level) == 0)
	{
		return;
	}

	ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel = &this->m_IllusionTempleRenewalLevel[level];

	if(this->GetUser(lpLevel,aIndex) == 0)
	{
		return;
	}

	if(lpLevel->EventItemOwner != aIndex)
	{
		return;
	}

	int slot = gItemManager.GetInventoryItemSlot(lpObj,GET_ITEM(14,64),lpLevel->EventItemLevel);

	if(slot == -1)
	{
		return;
	}

	if(lpObj->Inventory[slot].m_Serial != lpLevel->EventItemSerial)
	{
		return;
	}

	gItemManager.InventoryDelItem(lpObj->Index,slot);
	gItemManager.GCItemDeleteSend(lpObj->Index,slot,1);

	lpLevel->EventItemSerial = -1;
	lpLevel->EventItemNumber = -1;
	lpLevel->EventItemOwner = -1;
	lpLevel->EventItemLevel = -1;
	lpLevel->EventItemStatus = 1;

	#endif
}

bool CIllusionTempleRenewal::AddMonster(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel,int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	if(this->GetMonster(lpLevel,aIndex) != 0)
	{
		return 0;
	}

	for(int n = 0; n < MAX_IT_MONSTER; n++)
	{
		if(OBJECT_RANGE(lpLevel->MonsterIndex[n]) != 0)
		{
			continue;
		}

		lpLevel->MonsterIndex[n] = aIndex;
		return 1;
	}

	return 0;

	#else
	
	return 0;

	#endif
}

bool CIllusionTempleRenewal::DelMonster(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel,int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=803)

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

	#else

	return 0;

	#endif
}

int* CIllusionTempleRenewal::GetMonster(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel,int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	for(int n = 0; n < MAX_IT_MONSTER; n++)
	{
		if(lpLevel->MonsterIndex[n] == aIndex)
		{
			return &lpLevel->MonsterIndex[n];
		}
	}

	return 0;

	#else

	return 0;

	#endif
}

void CIllusionTempleRenewal::CleanMonster(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	for(int n = 0; n < MAX_IT_MONSTER; n++)
	{
		lpLevel->MonsterIndex[n] = -1;
	}

	#endif
}

void CIllusionTempleRenewal::ClearMonster(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	for(int n = 0; n < MAX_IT_MONSTER; n++)
	{
		if(OBJECT_RANGE(lpLevel->MonsterIndex[n]) != 0)
		{
			gObjDel(lpLevel->MonsterIndex[n]);
			lpLevel->MonsterIndex[n] = -1;
		}
	}

	#endif
}

int CIllusionTempleRenewal::GetMonsterCount(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	int count = 0;

	for(int n = 0; n < MAX_IT_MONSTER; n++)
	{
		if(OBJECT_RANGE(lpLevel->MonsterIndex[n]) != 0)
		{
			count++;
		}
	}

	return count;

	#else

	return 0;

	#endif
}

void CIllusionTempleRenewal::SetStoneStatue(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	if(OBJECT_RANGE(lpLevel->StoneStatueIndex) != 0)
	{
		return;
	}

	int index = gObjAddMonster(lpLevel->Map);

	if(OBJECT_RANGE(index) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[index];

	int px = 169;
	int py = 85;

	lpObj->PosNum = -1;
	lpObj->X = px;
	lpObj->Y = py;
	lpObj->TX = px;
	lpObj->TY = py;
	lpObj->OldX = px;
	lpObj->OldY = py;
	lpObj->StartX = px;
	lpObj->StartY = py;
	lpObj->Dir = 2;
	lpObj->Map = lpLevel->Map;

	if(gObjSetMonster(index,658) == 0)
	{
		gObjDel(index);
		return;
	}

	lpLevel->StoneStatueIndex = index;

	#endif
}

void CIllusionTempleRenewal::StageSpawn(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel,int Stage) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	for(std::vector<EVENT_SPAWN_STAGE_INFO>::iterator it = gEventSpawnStage.m_EventSpawnStageInfo.begin(); it != gEventSpawnStage.m_EventSpawnStageInfo.end(); it++)
	{
		if(it->Event != 3 || it->Level != lpLevel->Level || it->Stage != Stage)
		{
			continue;
		}

		this->SetMonster(lpLevel,it->MonsterClass,it->MaxRegenTime);
	}

	#endif
}

void CIllusionTempleRenewal::SetMonster(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel,int MonsterClass,int MaxRegenTime) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	for(int n = 0; n < gMonsterSetBase.m_count; n++)
	{
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

		(MaxRegenTime != -1) ? (lpObj->MaxRegenTime = MaxRegenTime) : (lpObj->PosNum = -1);

		if(this->AddMonster(lpLevel,index) == 0)
		{
			gObjDel(index);
			continue;
		}
	}

	#endif
}

bool CIllusionTempleRenewal::CheckEventItemSerial(int map,CMapItem* lpItem) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	int level = GET_IT_LEVEL(map);

	if(IT_LEVEL_RANGE(level) == 0)
	{
		return 0;
	}

	if(this->m_IllusionTempleRenewalLevel[level].EventItemSerial == -1)
	{
		return 0;
	}

	if(lpItem->m_Index == GET_ITEM(14,64) && lpItem->m_Serial == this->m_IllusionTempleRenewalLevel[level].EventItemSerial)
	{
		return 1;
	}

	return 0;

	#else

	return 0;

	#endif
}

void CIllusionTempleRenewal::SetEventItemSerial(int map,int index,int serial) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	int level = GET_IT_LEVEL(map);

	if(IT_LEVEL_RANGE(level) == 0)
	{
		return;
	}

	if(this->m_IllusionTempleRenewalLevel[level].EventItemSerial != -1)
	{
		return;
	}

	if(index == GET_ITEM(14,64))
	{
		this->m_IllusionTempleRenewalLevel[level].EventItemSerial = serial;
	}

	#endif
}

void CIllusionTempleRenewal::DropEventItem(int map,int number,int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	int level = GET_IT_LEVEL(map);

	if(IT_LEVEL_RANGE(level) == 0)
	{
		return;
	}

	gMap[map].m_Item[number].m_Time = GetTickCount()+15000;

	gMap[map].m_Item[number].m_LootTime = GetTickCount();

	this->m_IllusionTempleRenewalLevel[level].EventItemNumber = number;

	this->m_IllusionTempleRenewalLevel[level].EventItemOwner = -1;

	this->m_IllusionTempleRenewalLevel[level].EventItemLevel = -1;

	this->m_IllusionTempleRenewalLevel[level].EventItemStatus = 1;

	this->NoticeSendToAll(&this->m_IllusionTempleRenewalLevel[level],0,gMessage.GetMessage(47),gObj[aIndex].Name);

	#endif
}

void CIllusionTempleRenewal::GetEventItem(int map,int aIndex,CMapItem* lpItem) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	int level = GET_IT_LEVEL(map);

	if(IT_LEVEL_RANGE(level) == 0)
	{
		return;
	}

	this->m_IllusionTempleRenewalLevel[level].EventItemNumber = -1;

	this->m_IllusionTempleRenewalLevel[level].EventItemOwner = aIndex;

	this->m_IllusionTempleRenewalLevel[level].EventItemLevel = lpItem->m_Level;

	this->m_IllusionTempleRenewalLevel[level].EventItemStatus = 0;

	this->GCIllusionTempleRenewalEventItemOwnerSend(&this->m_IllusionTempleRenewalLevel[level]);

	this->NoticeSendToAll(&this->m_IllusionTempleRenewalLevel[level],0,gMessage.GetMessage(48),gObj[aIndex].Name);

	#endif
}

void CIllusionTempleRenewal::DestroyEventItem(int map,CMapItem* lpItem) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	int level = GET_IT_LEVEL(map);

	if(IT_LEVEL_RANGE(level) == 0)
	{
		return;
	}

	lpItem->m_State = OBJECT_DIECMD;

	this->m_IllusionTempleRenewalLevel[level].EventItemSerial = -1;

	this->m_IllusionTempleRenewalLevel[level].EventItemNumber = -1;

	this->m_IllusionTempleRenewalLevel[level].EventItemOwner = -1;

	this->m_IllusionTempleRenewalLevel[level].EventItemLevel = -1;

	this->m_IllusionTempleRenewalLevel[level].EventItemStatus = 1;

	this->SetStoneStatue(&this->m_IllusionTempleRenewalLevel[level]);

	#endif
}

void CIllusionTempleRenewal::UserDieProc(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	int level = GET_IT_LEVEL(lpObj->Map);

	if(IT_LEVEL_RANGE(level) == 0)
	{
		return;
	}

	if(this->GetState(level) != ITR_STATE_START)
	{
		return;
	}

	if(this->GetUser(&this->m_IllusionTempleRenewalLevel[level],lpObj->Index) == 0)
	{
		return;
	}

	this->SearchUserDropEventItem(lpObj->Index);

	gEffectManager.DelEffect(lpObj,EFFECT_ORDER_OF_PROTECTION);

	gEffectManager.DelEffect(lpObj,EFFECT_ORDER_OF_RESTRAINT);

	if(lpTarget->Type != OBJECT_USER)
	{
		return;
	}

	ILLUSION_TEMPLE_RENEWAL_USER* lpUser = this->GetUser(&this->m_IllusionTempleRenewalLevel[level],lpTarget->Index);

	if(lpUser == 0)
	{
		return;
	}

	lpUser->Point += 5;

	#endif
}

void CIllusionTempleRenewal::MonsterDieProc(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	int aIndex = gObjMonsterGetTopHitDamageUser(lpObj);

	if(OBJECT_RANGE(aIndex) != 0)
	{
		lpTarget = &gObj[aIndex];
	}

	int level = GET_IT_LEVEL(lpObj->Map);

	if(IT_LEVEL_RANGE(level) == 0)
	{
		return;
	}

	if(this->GetState(level) != ITR_STATE_START)
	{
		return;
	}

	if(this->GetMonster(&this->m_IllusionTempleRenewalLevel[level],lpObj->Index) == 0)
	{
		return;
	}

	if(lpTarget->Type != OBJECT_USER)
	{
		return;
	}

	ILLUSION_TEMPLE_RENEWAL_USER* lpUser = this->GetUser(&this->m_IllusionTempleRenewalLevel[level],lpTarget->Index);

	if(lpUser == 0)
	{
		return;
	}

	lpUser->Point += 2;

	#endif
}

bool CIllusionTempleRenewal::CheckPlayerTarget(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	int level = GET_IT_LEVEL(lpObj->Map);

	if(IT_LEVEL_RANGE(level) == 0)
	{
		return 0;
	}

	if(this->GetState(level) != ITR_STATE_START)
	{
		return 0;
	}

	ILLUSION_TEMPLE_RENEWAL_USER* lpUser1 = this->GetUser(&this->m_IllusionTempleRenewalLevel[level],lpObj->Index);

	if(lpUser1 == 0)
	{
		return 0;
	}

	ILLUSION_TEMPLE_RENEWAL_USER* lpUser2 = this->GetUser(&this->m_IllusionTempleRenewalLevel[level],lpTarget->Index);

	if(lpUser2 == 0)
	{
		return 0;
	}

	if(lpUser1->Team == lpUser2->Team)
	{
		return 0;
	}

	return 1;

	#else

	return 0;

	#endif
}

void CIllusionTempleRenewal::NoticeSendToAll(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel,int type,char* message,...) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	char buff[256];

	va_list arg;
	va_start(arg,message);
	vsprintf_s(buff,message,arg);
	va_end(arg);

	for(int n = 0; n < MAX_IT_USER; n++)
	{
		if(OBJECT_RANGE(lpLevel->User[n].Index) != 0)
		{
			gNotice.GCNoticeSend(lpLevel->User[n].Index,type,0,0,0,0,0,buff);
		}
	}

	#endif
}

void CIllusionTempleRenewal::DataSendToAll(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel,BYTE* lpMsg,int size) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	for(int n = 0; n < MAX_IT_USER; n++)
	{
		if(OBJECT_RANGE(lpLevel->User[n].Index) != 0)
		{
			DataSend(lpLevel->User[n].Index,lpMsg,size);
		}
	}

	#endif
}

void CIllusionTempleRenewal::CGIllusionTempleRenewalSkillRecv(PMSG_ILLUSION_TEMPLE_RENEWAL_SKILL_RECV* lpMsg,int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	LPOBJ lpObj = &gObj[aIndex];

	int level = GET_IT_LEVEL(lpObj->Map);

	if(IT_LEVEL_RANGE(level) == 0)
	{
		return;
	}

	ILLUSION_TEMPLE_RENEWAL_USER* lpUser = this->GetUser(&this->m_IllusionTempleRenewalLevel[level],aIndex);

	if(lpUser == 0)
	{
		return;
	}

	int bIndex = MAKE_NUMBERW(lpMsg->index[0],lpMsg->index[1]);

	if(OBJECT_RANGE(bIndex) == 0)
	{
		return;
	}
	
	int skill = MAKE_NUMBERW(lpMsg->skill[0],lpMsg->skill[1]);

	SKILL_INFO SkillInfo;

	if(gSkillManager.GetInfo(skill,&SkillInfo) == 0)
	{
		this->GCIllusionTempleRenewalAddSkillSend(aIndex,0,skill,bIndex);
		return;
	}

	if(lpUser->Point < SkillInfo.RequireKillCount)
	{
		this->GCIllusionTempleRenewalAddSkillSend(aIndex,0,skill,bIndex);
		return;
	}

	int result = 0;

	switch (skill)
	{
	case SKILL_ORDER_OF_PROTECTION:
		result = gSkillManager.SkillOrderOfProtection(aIndex,bIndex);
		break;
	case SKILL_ORDER_OF_RESTRAINT:
		result = gSkillManager.SkillOrderOfRestraint(aIndex,bIndex);
		break;
	case SKILL_ORDER_OF_TRACKING:
		result = gSkillManager.SkillOrderOfTracking(aIndex,this->m_IllusionTempleRenewalLevel[level].EventItemOwner);
		break;
	case SKILL_ORDER_OF_WEAKEN:
		result = gSkillManager.SkillOrderOfWeaken(aIndex,bIndex);
		break;
	}

	if(result == 0)
	{
		this->GCIllusionTempleRenewalAddSkillSend(aIndex,0,skill,bIndex);
	}
	else
	{
		lpUser->Point -= SkillInfo.RequireKillCount;
		this->GCIllusionTempleRenewalAddSkillSend(aIndex,1,skill,bIndex);
	}

	#endif
}

void CIllusionTempleRenewal::CGIllusionTempleRenewalButtonClickRecv(int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	LPOBJ lpObj = &gObj[aIndex];

	int level = GET_IT_LEVEL(lpObj->Map);

	if(IT_LEVEL_RANGE(level) == 0)
	{
		return;
	}

	if(this->GetState(level) != ITR_STATE_CLEAN)
	{
		return;
	}

	ILLUSION_TEMPLE_RENEWAL_USER* lpUser = this->GetUser(&this->m_IllusionTempleRenewalLevel[level],lpObj->Index);

	if(lpUser == 0)
	{
		return;
	}

	if(lpUser->Team != this->m_IllusionTempleRenewalLevel[level].WinnerTeam)
	{
		return;
	}

	gItemBagManager.DropItemBySpecialValue(ITEM_BAG_SPECIAL_ILLUSION_TEMPLE,level,1,lpObj,lpObj->Map,lpObj->X,lpObj->Y);

	#endif
}

void CIllusionTempleRenewal::CGIllusionTempleRenewalStorageRecv(PMSG_ILLUSION_TEMPLE_RENEWAL_STORAGE_RECV* lpMsg,int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	if(gServerInfo.m_IllusionTempleEvent == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	int level = GET_IT_LEVEL(lpObj->Map);

	if(IT_LEVEL_RANGE(level) == 0)
	{
		return;
	}

	ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel = &this->m_IllusionTempleRenewalLevel[level];

	if(this->GetState(level) != IT_STATE_START)
	{
		return;
	}

	int bIndex = MAKE_NUMBERW(lpMsg->index[0],lpMsg->index[1]);

	if(OBJECT_RANGE(bIndex) == 0)
	{
		return;
	}

	LPOBJ lpTarget = &gObj[bIndex];

	if(lpTarget->Index != lpLevel->StoneStatueIndex)
	{
		return;
	}

	if(lpLevel->EventItemSerial != -1)
	{
		return;
	}

	ILLUSION_TEMPLE_RENEWAL_USER* lpUser = this->GetUser(lpLevel,lpObj->Index);

	if(lpUser == 0)
	{
		return;
	}

	GDCreateItemSend(lpObj->Index,lpTarget->Map,(BYTE)lpTarget->X,(BYTE)lpTarget->Y,GET_ITEM(14,64),0,255,0,0,0,lpObj->Index,0,0,0,0,0,0xFF,0);

	GCUserDieSend(&gObj[lpLevel->StoneStatueIndex],lpLevel->StoneStatueIndex,0,0);

	gObjDel(lpLevel->StoneStatueIndex);

	lpLevel->StoneStatueIndex = -1;

	//LPOBJ lpObj = &gObj[aIndex];

	//int level = GET_IT_LEVEL(lpObj->Map);

	//if(IT_LEVEL_RANGE(level) == 0)
	//{
	//	return;
	//}

	//ILLUSION_TEMPLE_RENEWAL_USER* lpUser = this->GetUser(&this->m_IllusionTempleRenewalLevel[level],aIndex);

	//if(lpUser == 0)
	//{
	//	return;
	//}

	//int bIndex = MAKE_NUMBERW(lpMsg->index[0],lpMsg->index[1]);

	//if(OBJECT_RANGE(bIndex) == 0)
	//{
	//	return;
	//}

	//LPOBJ lpTarget = &gObj[bIndex];

	//if(abs(lpObj->Y-lpTarget->Y) > 3 || abs(lpObj->X-lpTarget->X) > 3)
	//{
	//	return;
	//}

	//if (lpMsg->status[0] || lpMsg->status[1])
	//{
	//	if (lpMsg->status[0] != 1 || lpMsg->status[1])
	//	{
	//		if (lpMsg->status[0] || lpMsg->status[1] != 1)
	//		{
	//			if (lpMsg->status[0] == 1 && lpMsg->status[1] == 1)
	//			{
	//				//RegisterRelics
	//				LogAdd(LOG_RED, "1");
	//			}
	//		}
	//		else
	//		{
	//			//Check_RegisterRelics
	//			LogAdd(LOG_RED, "2");
	//		}
	//	}
	//	else
	//	{
	//		//GetRelics
	//		LogAdd(LOG_RED, "3");
	//	}
	//}
	//else
	//{
	//	//Check_GetRelics
	//	LogAdd(LOG_RED, "4");
	//}

	#endif
}

void CIllusionTempleRenewal::CGIllusionTempleRenewalEnterRecv(int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	if(gServerInfo.m_IllusionTempleEvent == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	PMSG_ILLUSION_TEMPLE_RENEWAL_ENTER_SEND pMsg;

	pMsg.header.set(0xBF,0x70,sizeof(pMsg));

	pMsg.result = 0;

	int level = gEventEntryLevel.GetITLevel(lpObj);

	if(lpObj->Change != -1) // OK
	{
		GCNewMessageSend(aIndex,0,0,gMessage.GetMessage(159));
		pMsg.result = 7;
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	if(IT_LEVEL_RANGE(level) == 0) // OK
	{
		pMsg.result = 8;
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	if(this->GetEnterEnabled(level) == 0) // OK
	{
		pMsg.result = 8;
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	//if(OBJECT_RANGE(lpObj->PartyNumber) == 0) // OK
	//{
	//	pMsg.result = 1;
	//	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
	//	return;
	//}

	//if(gParty.IsLeader(lpObj->PartyNumber,aIndex) != 0) // OK
	//{
	//	pMsg.result = 3;
	//	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
	//	return;
	//}

	//if(gParty.GetMemberCount(lpObj->PartyNumber) < gServerInfo.m_IllusionTempleMinUser) // OK
	//{
	//	pMsg.result = 4;
	//	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
	//	return;
	//}

	if(gServerInfo.m_PKLimitFree == 0 && lpObj->PKLevel > 4 && gServerInfo.m_PKLimitEventEntry != 0)
	{
		pMsg.result = 12;
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel = &this->m_IllusionTempleRenewalLevel[level];

	if(gMapServerManager.CheckMapServer(lpLevel->Map) == 0) // OK
	{
		pMsg.result = 10;
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	if(OBJECT_RANGE(lpObj->DuelUser) != 0) // OK
	{
		pMsg.result = 15;
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	if(lpObj->Guild != 0 && lpObj->Guild->WarState == 1) // OK
	{
		pMsg.result = 16;
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	if(lpObj->Inventory[INVENTORY_SLOT_HELPER].m_Index == GET_ITEM(13,2) || lpObj->Inventory[INVENTORY_SLOT_HELPER].m_Index == GET_ITEM(13,3) || lpObj->Inventory[INVENTORY_SLOT_HELPER].m_Index == GET_ITEM(13,37)) // OK
	{
		pMsg.result = 17;
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	/*if(lpObj->PShopOpen != 0)
	{
		pMsg.result = 1;
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}*/

	if(this->AddUser(lpLevel,aIndex) == 0) // OK
	{
		pMsg.result = 14;
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);

	gObjMoveGate(aIndex,(142+level));

	this->GCIllusionTempleRenewalChangeScenarioSend(lpLevel,0,aIndex);

	#endif
}

void CIllusionTempleRenewal::GCIllusionTempleRenewalRewardScoreSend(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel) // OK
{
	// NEED FIX
	return;
	#if(GAMESERVER_UPDATE>=803)

	BYTE send[256];

	PMSG_ILLUSION_TEMPLE_RENEWAL_REWARD_SCORE_SEND pMsg;

	pMsg.header.set(0xBF,0x04,0);

	int size = sizeof(pMsg);

	pMsg.score[ITR_TEAM_Y] = lpLevel->Score[ITR_TEAM_Y];

	pMsg.score[ITR_TEAM_B] = lpLevel->Score[ITR_TEAM_B];

	pMsg.winner = lpLevel->WinnerTeam;

	pMsg.count = 0;

	PMSG_ILLUSION_TEMPLE_RENEWAL_REWARD_SCORE info;

	for(int n = 0; n < MAX_IT_USER; n++)
	{
		if(OBJECT_RANGE(lpLevel->User[n].Index) == 0)
		{
			continue;
		}

		memcpy(info.name,gObj[lpLevel->User[n].Index].Name,sizeof(info.name));

		info.map = lpLevel->Map;

		info.team = lpLevel->User[n].Team;

		info.Class = (BYTE)gObj[lpLevel->User[n].Index].Class;

		info.Point = lpLevel->User[n].Point;

		GDRankingIllusionTempleSaveSend(lpLevel->User[n].Index,lpLevel->User[n].Score);

		memcpy(&send[size],&info,sizeof(info));
		size += sizeof(info);

		pMsg.count++;
	}

	pMsg.header.size = size;

	memcpy(send,&pMsg,sizeof(pMsg));

	this->DataSendToAll(lpLevel,send,size);

	#endif
}

void CIllusionTempleRenewal::GCIllusionTempleRenewalChangeScenarioSend(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel,int state,int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	PMSG_ILLUSION_TEMPLE_RENEWAL_CHANGE_SCENARIO_SEND pMsg;

	pMsg.header.set(0xBF,0x60,sizeof(pMsg));

	pMsg.level = lpLevel->Level;

	pMsg.state = state;

	if(OBJECT_RANGE(aIndex) == 0)
	{
		this->DataSendToAll(lpLevel,(BYTE*)&pMsg,pMsg.header.size);
	}
	else
	{
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
	}

	#endif
}

void CIllusionTempleRenewal::GCIllusionTempleRenewalEventItemOwnerSend(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	PMSG_ILLUSION_TEMPLE_RENEWAL_EVENT_ITEM_OWNER_SEND pMsg;

	pMsg.header.set(0xBF,0x61,sizeof(pMsg));

	pMsg.index = lpLevel->EventItemOwner;

	memcpy(pMsg.name,gObj[lpLevel->EventItemOwner].Name,sizeof(pMsg.name));

	pMsg.state = lpLevel->EventItemStatus;

	this->DataSendToAll(lpLevel,(BYTE*)&pMsg,pMsg.header.size);

	#endif
}

void CIllusionTempleRenewal::GCIllusionTempleRenewalBattleInfoSend(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	for(int n = 0; n < MAX_IT_USER; n++)
	{
		ILLUSION_TEMPLE_RENEWAL_USER* lpUser = &lpLevel->User[n];

		if(OBJECT_RANGE(lpUser->Index) == 0)
		{
			continue;
		}

		PMSG_ILLUSION_TEMPLE_RENEWAL_BATTLE_INFO_SEND pMsg;

		pMsg.header.set(0xBF,0x62,sizeof(pMsg));

		pMsg.time = lpLevel->RemainTime;

		pMsg.score[ITR_TEAM_Y] = lpLevel->Score[ITR_TEAM_Y];

		pMsg.score[ITR_TEAM_B] = lpLevel->Score[ITR_TEAM_B];

		pMsg.team = lpUser->Team;

		DataSend(lpUser->Index,(BYTE*)&pMsg,pMsg.header.size);
	}

	#endif
}

void CIllusionTempleRenewal::GCIllusionTempleRenewalDelSkillSend(int aIndex,int skill) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	PMSG_ILLUSION_TEMPLE_RENEWAL_DEL_SKILL_SEND pMsg;

	pMsg.header.set(0xBF,0x63,sizeof(pMsg));
	
	pMsg.skill[0] = SET_NUMBERHB(skill);

	pMsg.skill[1] = SET_NUMBERLB(skill);

	pMsg.index = aIndex;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
	MsgSendV2(&gObj[aIndex],(BYTE*)&pMsg,pMsg.header.size);

	#endif
}

void CIllusionTempleRenewal::GCIllusionTempleRenewalAddSkillSend(int aIndex,int result,int skill,int bIndex) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	PMSG_ILLUSION_TEMPLE_RENEWAL_ADD_SKILL_SEND pMsg;

	pMsg.header.set(0xBF,0x67,sizeof(pMsg));

	pMsg.result = result;
	
	pMsg.skill[0] = SET_NUMBERHB(skill);

	pMsg.skill[1] = SET_NUMBERLB(skill);

	pMsg.index = aIndex;

	pMsg.target = bIndex;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);

	if(result != 0)
	{
		MsgSendV2(&gObj[aIndex],(BYTE*)&pMsg,pMsg.header.size);
	}

	#endif
}

void CIllusionTempleRenewal::GCIllusionTempleRenewalResultSend(int aIndex,int bIndex, BYTE result, BYTE status) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	PMSG_ILLUSION_TEMPLE_RENEWAL_RESULT_SEND pMsg;

	pMsg.header.set(0xF8,0x6A,sizeof(pMsg));

	pMsg.result = result;
	
	pMsg.index[0] = SET_NUMBERHB(aIndex);

	pMsg.index[1] = SET_NUMBERLB(aIndex);

	pMsg.Class = gObj[aIndex].Class;

	pMsg.status = status;

	DataSend(bIndex,(BYTE*)&pMsg,pMsg.header.size);

	#endif
}

void CIllusionTempleRenewal::ForceStart() // OK
{
	#if(GAMESERVER_UPDATE>=803)

	SYSTEMTIME SystemTime;

	GetLocalTime(&SystemTime);

	ILLUSION_TEMPLE_RENEWAL_START_TIME info;

	info.Year = SystemTime.wYear;

	info.Month = SystemTime.wMonth;

	info.Day = SystemTime.wDay;

	info.DayOfWeek = -1;

	info.Hour = SystemTime.wHour;

	info.Minute = (SystemTime.wMinute+1);

	info.Second = SystemTime.wSecond;

	this->m_IllusionTempleRenewalStartTime.push_back(info);

	this->Init();

	#endif
}