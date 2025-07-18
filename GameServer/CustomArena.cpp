// CustomArena.cpp: implementation of the CCustomArena class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomArena.h"
#include "DSProtocol.h"
#include "EffectManager.h"
#include "Gate.h"
#include "GuildManager.h"
#include "ItemBagManager.h"
#include "Map.h"
#include "MapServerManager.h"
#include "MemScript.h"
#include "Message.h"
#include "Notice.h"
#include "ScheduleManager.h"
#include "Util.h"

CCustomArena gCustomArena;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomArena::CCustomArena() // OK
{
	for(int n=0;n < MAX_CUSTOM_ARENA;n++)
	{
		CUSTOM_ARENA_INFO* lpInfo = &this->m_CustomArenaInfo[n];

		lpInfo->Index = n;
		lpInfo->State = CUSTOM_ARENA_STATE_BLANK;
		lpInfo->RemainTime = 0;
		lpInfo->TargetTime = 0;
		lpInfo->TickCount = GetTickCount();
		lpInfo->EnterEnabled = 0;
		lpInfo->AlarmMinSave = -1;
		lpInfo->AlarmMinLeft = -1;

		this->CleanUser(lpInfo);
	}
}

CCustomArena::~CCustomArena() // OK
{

}

void CCustomArena::Init() // OK
{
	for(int n=0;n < MAX_CUSTOM_ARENA;n++)
	{
		if(this->m_CustomArenaSwitch == 0)
		{
			this->SetState(&this->m_CustomArenaInfo[n],CUSTOM_ARENA_STATE_BLANK);
		}
		else
		{
			this->SetState(&this->m_CustomArenaInfo[n],CUSTOM_ARENA_STATE_EMPTY);
		}
	}
}

void CCustomArena::ReadCustomArenaInfo(char* section,char* path) // OK
{
	this->m_CustomArenaSwitch = GetPrivateProfileInt(section,"CustomArenaSwitch",0,path);

	this->m_CustomArenaMapNumber = GetPrivateProfileInt(section,"CustomArenaMapNumber",0,path);

	this->m_CustomArenaVictimScoreDecrease = GetPrivateProfileInt(section,"CustomArenaVictimScoreDecrease",0,path);

	this->m_CustomArenaKillerScoreIncrease = GetPrivateProfileInt(section,"CustomArenaKillerScoreIncrease",0,path);
}

void CCustomArena::Load(char* path) // OK
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

	for(int n=0;n < MAX_CUSTOM_ARENA;n++)
	{
		this->m_CustomArenaInfo[n].StartTime.clear();
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

					CUSTOM_ARENA_START_TIME info;

					int index = lpMemScript->GetNumber();

					info.Year = lpMemScript->GetAsNumber();

					info.Month = lpMemScript->GetAsNumber();

					info.Day = lpMemScript->GetAsNumber();

					info.DayOfWeek = lpMemScript->GetAsNumber();

					info.Hour = lpMemScript->GetAsNumber();

					info.Minute = lpMemScript->GetAsNumber();

					info.Second = lpMemScript->GetAsNumber();

					this->m_CustomArenaInfo[index].StartTime.push_back(info);
				}
				else if(section == 1)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					int index = lpMemScript->GetNumber();

					strcpy_s(this->m_CustomArenaInfo[index].RuleInfo.Name,lpMemScript->GetAsString());

					this->m_CustomArenaInfo[index].RuleInfo.AlarmTime = lpMemScript->GetAsNumber();

					this->m_CustomArenaInfo[index].RuleInfo.StandTime = lpMemScript->GetAsNumber();

					this->m_CustomArenaInfo[index].RuleInfo.EventTime = lpMemScript->GetAsNumber();

					this->m_CustomArenaInfo[index].RuleInfo.CloseTime = lpMemScript->GetAsNumber();

					this->m_CustomArenaInfo[index].RuleInfo.StartGate = lpMemScript->GetAsNumber();

					this->m_CustomArenaInfo[index].RuleInfo.FinalGate = lpMemScript->GetAsNumber();

					this->m_CustomArenaInfo[index].RuleInfo.MinUser = lpMemScript->GetAsNumber();

					this->m_CustomArenaInfo[index].RuleInfo.MaxUser = lpMemScript->GetAsNumber();

					this->m_CustomArenaInfo[index].RuleInfo.MaxDeath = lpMemScript->GetAsNumber();

					this->m_CustomArenaInfo[index].RuleInfo.MinLevel = lpMemScript->GetAsNumber();

					this->m_CustomArenaInfo[index].RuleInfo.MaxLevel = lpMemScript->GetAsNumber();

					#if(GAMESERVER_UPDATE>=401)

					this->m_CustomArenaInfo[index].RuleInfo.MinMasterLevel = lpMemScript->GetAsNumber();

					this->m_CustomArenaInfo[index].RuleInfo.MaxMasterLevel = lpMemScript->GetAsNumber();

					#else

					this->m_CustomArenaInfo[index].RuleInfo.MinMasterLevel = 0;

					this->m_CustomArenaInfo[index].RuleInfo.MaxMasterLevel = 0;

					#endif

					this->m_CustomArenaInfo[index].RuleInfo.MinReset = lpMemScript->GetAsNumber();

					this->m_CustomArenaInfo[index].RuleInfo.MaxReset = lpMemScript->GetAsNumber();

					this->m_CustomArenaInfo[index].RuleInfo.MinMasterReset = lpMemScript->GetAsNumber();

					this->m_CustomArenaInfo[index].RuleInfo.MaxMasterReset = lpMemScript->GetAsNumber();

					for(int n=0;n < MAX_CLASS;n++){this->m_CustomArenaInfo[index].RuleInfo.RequireClass[n] = lpMemScript->GetAsNumber();}
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

void CCustomArena::MainProc() // OK
{
	for(int n=0;n < MAX_CUSTOM_ARENA;n++)
	{
		CUSTOM_ARENA_INFO* lpInfo = &this->m_CustomArenaInfo[n];

		if((GetTickCount()-lpInfo->TickCount) >= 1000)
		{
			lpInfo->TickCount = GetTickCount();

			lpInfo->RemainTime = (int)difftime(lpInfo->TargetTime,time(0));

			switch(lpInfo->State)
			{
				case CUSTOM_ARENA_STATE_BLANK:
					this->ProcState_BLANK(lpInfo);
					break;
				case CUSTOM_ARENA_STATE_EMPTY:
					this->ProcState_EMPTY(lpInfo);
					break;
				case CUSTOM_ARENA_STATE_STAND:
					this->ProcState_STAND(lpInfo);
					break;
				case CUSTOM_ARENA_STATE_START:
					this->ProcState_START(lpInfo);
					break;
				case CUSTOM_ARENA_STATE_CLEAN:
					this->ProcState_CLEAN(lpInfo);
					break;
			}
		}
	}
}

void CCustomArena::ProcState_BLANK(CUSTOM_ARENA_INFO* lpInfo) // OK
{

}

void CCustomArena::ProcState_EMPTY(CUSTOM_ARENA_INFO* lpInfo) // OK
{
	if(lpInfo->RemainTime > 0 && lpInfo->RemainTime <= (lpInfo->RuleInfo.AlarmTime*60))
	{
		this->CheckUser(lpInfo);

		lpInfo->EnterEnabled = 1;

		if((lpInfo->AlarmMinSave=(((lpInfo->RemainTime%60)==0)?((lpInfo->RemainTime/60)-1):(lpInfo->RemainTime/60))) != lpInfo->AlarmMinLeft)
		{
			lpInfo->AlarmMinLeft = lpInfo->AlarmMinSave;

			gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(501),lpInfo->RuleInfo.Name,(lpInfo->AlarmMinLeft+1));
		}
	}

	if(lpInfo->RemainTime <= 0)
	{
		gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(502),lpInfo->RuleInfo.Name);
		this->NoticeSendToAll(lpInfo,1,gMessage.GetMessage(503),lpInfo->RuleInfo.Name,lpInfo->RuleInfo.StandTime);
		this->SetState(lpInfo,CUSTOM_ARENA_STATE_STAND);
	}
}

void CCustomArena::ProcState_STAND(CUSTOM_ARENA_INFO* lpInfo) // OK
{
	this->CheckUser(lpInfo);

	if(this->GetUserCount(lpInfo) < ((lpInfo->RuleInfo.MinUser==-1)?1:lpInfo->RuleInfo.MinUser))
	{
		this->SetState(lpInfo,CUSTOM_ARENA_STATE_EMPTY);
		return;
	}

	if(lpInfo->RemainTime <= 0)
	{
		this->NoticeSendToAll(lpInfo,0,gMessage.GetMessage(504),lpInfo->RuleInfo.Name);
		this->SetState(lpInfo,CUSTOM_ARENA_STATE_START);
	}
}

void CCustomArena::ProcState_START(CUSTOM_ARENA_INFO* lpInfo) // OK
{
	this->CheckUser(lpInfo);

	if(this->GetUserCount(lpInfo) == 0)
	{
		this->SetState(lpInfo,CUSTOM_ARENA_STATE_EMPTY);
		return;
	}

	if(this->GetUserCount(lpInfo) == 1)
	{
		this->SetState(lpInfo,CUSTOM_ARENA_STATE_CLEAN);
		return;
	}

	if(lpInfo->RemainTime <= 0)
	{
		this->NoticeSendToAll(lpInfo,0,gMessage.GetMessage(505),lpInfo->RuleInfo.Name);
		this->SetState(lpInfo,CUSTOM_ARENA_STATE_CLEAN);
	}
}

void CCustomArena::ProcState_CLEAN(CUSTOM_ARENA_INFO* lpInfo) // OK
{
	this->CheckUser(lpInfo);

	if(lpInfo->RemainTime <= 0)
	{
		this->SetState(lpInfo,CUSTOM_ARENA_STATE_EMPTY);
	}
}

void CCustomArena::SetState(CUSTOM_ARENA_INFO* lpInfo,int state) // OK
{
	switch((lpInfo->State=state))
	{
		case CUSTOM_ARENA_STATE_BLANK:
			this->SetState_BLANK(lpInfo);
			break;
		case CUSTOM_ARENA_STATE_EMPTY:
			this->SetState_EMPTY(lpInfo);
			break;
		case CUSTOM_ARENA_STATE_STAND:
			this->SetState_STAND(lpInfo);
			break;
		case CUSTOM_ARENA_STATE_START:
			this->SetState_START(lpInfo);
			break;
		case CUSTOM_ARENA_STATE_CLEAN:
			this->SetState_CLEAN(lpInfo);
			break;
	}
}

void CCustomArena::SetState_BLANK(CUSTOM_ARENA_INFO* lpInfo) // OK
{

}

void CCustomArena::SetState_EMPTY(CUSTOM_ARENA_INFO* lpInfo) // OK
{
	lpInfo->EnterEnabled = 0;
	lpInfo->AlarmMinSave = -1;
	lpInfo->AlarmMinLeft = -1;

	this->ClearUser(lpInfo);

	this->CheckSync(lpInfo);
}

void CCustomArena::SetState_STAND(CUSTOM_ARENA_INFO* lpInfo) // OK
{
	lpInfo->EnterEnabled = 0;
	lpInfo->AlarmMinSave = -1;
	lpInfo->AlarmMinLeft = -1;

	lpInfo->RemainTime = lpInfo->RuleInfo.StandTime*60;

	lpInfo->TargetTime = (int)(time(0)+lpInfo->RemainTime);
}

void CCustomArena::SetState_START(CUSTOM_ARENA_INFO* lpInfo) // OK
{
	lpInfo->EnterEnabled = 0;
	lpInfo->AlarmMinSave = -1;
	lpInfo->AlarmMinLeft = -1;

	lpInfo->RemainTime = lpInfo->RuleInfo.EventTime*60;

	lpInfo->TargetTime = (int)(time(0)+lpInfo->RemainTime);
}

void CCustomArena::SetState_CLEAN(CUSTOM_ARENA_INFO* lpInfo) // OK
{
	lpInfo->EnterEnabled = 0;
	lpInfo->AlarmMinSave = -1;
	lpInfo->AlarmMinLeft = -1;

	this->CalcUserRank(lpInfo);

	for(int n=0;n < MAX_CUSTOM_ARENA_USER;n++)
	{
		if(OBJECT_RANGE(lpInfo->User[n].Index) != 0)
		{
			if(lpInfo->User[n].Rank == 1)
			{
				gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(506),lpInfo->RuleInfo.Name,gObj[lpInfo->User[n].Index].Name);
			}
			
			gItemBagManager.DropItemBySpecialValue(ITEM_BAG_SPECIAL_CUSTOM_ARENA,lpInfo->Index,lpInfo->User[n].Rank,&gObj[lpInfo->User[n].Index],gObj[lpInfo->User[n].Index].Map,gObj[lpInfo->User[n].Index].X,gObj[lpInfo->User[n].Index].Y);
			
			gNotice.GCNoticeSend(lpInfo->User[n].Index,1,0,0,0,0,0,gMessage.GetMessage(507),lpInfo->RuleInfo.Name,lpInfo->User[n].Rank,lpInfo->User[n].Score);
		}
	}

	lpInfo->RemainTime = lpInfo->RuleInfo.CloseTime*60;

	lpInfo->TargetTime = (int)(time(0)+lpInfo->RemainTime);
}

void CCustomArena::CheckSync(CUSTOM_ARENA_INFO* lpInfo) // OK
{
	if(lpInfo->StartTime.empty() != 0)
	{
		this->SetState(lpInfo,CUSTOM_ARENA_STATE_BLANK);
		return;
	}

	CTime ScheduleTime;

	CScheduleManager ScheduleManager;

	for(std::vector<CUSTOM_ARENA_START_TIME>::iterator it=lpInfo->StartTime.begin();it != lpInfo->StartTime.end();it++)
	{
		ScheduleManager.AddSchedule(it->Year,it->Month,it->Day,it->Hour,it->Minute,it->Second,it->DayOfWeek);
	}

	if(ScheduleManager.GetSchedule(&ScheduleTime) == 0)
	{
		this->SetState(lpInfo,CUSTOM_ARENA_STATE_BLANK);
		return;
	}

	lpInfo->RemainTime = (int)difftime(ScheduleTime.GetTime(),time(0));

	lpInfo->TargetTime = (int)ScheduleTime.GetTime();
}

int CCustomArena::GetState(int index) // OK
{
	if(index < 0 || index >= MAX_CUSTOM_ARENA)
	{
		return CUSTOM_ARENA_STATE_BLANK;
	}

	return this->m_CustomArenaInfo[index].State;
}

int CCustomArena::GetRemainTime(int index) // OK
{
	if(index < 0 || index >= MAX_CUSTOM_ARENA)
	{
		return 0;
	}

	CUSTOM_ARENA_INFO* lpInfo = &this->m_CustomArenaInfo[index];

	if(lpInfo->StartTime.empty() != 0)
	{
		return 0;
	}

	CTime ScheduleTime;

	CScheduleManager ScheduleManager;

	for(std::vector<CUSTOM_ARENA_START_TIME>::iterator it=lpInfo->StartTime.begin();it != lpInfo->StartTime.end();it++)
	{
		ScheduleManager.AddSchedule(it->Year,it->Month,it->Day,it->Hour,it->Minute,it->Second,it->DayOfWeek);
	}

	if(ScheduleManager.GetSchedule(&ScheduleTime) == 0)
	{
		return 0;
	}

	int RemainTime = (int)difftime(ScheduleTime.GetTime(),time(0));

	return (((RemainTime%60)==0)?(RemainTime/60):((RemainTime/60)+1));
}

int CCustomArena::GetEnterEnabled(int index) // OK
{
	if(index < 0 || index >= MAX_CUSTOM_ARENA)
	{
		return 0;
	}

	return this->m_CustomArenaInfo[index].EnterEnabled;
}

int CCustomArena::GetEnteredUserCount(int index) // OK
{
	if(index < 0 || index >= MAX_CUSTOM_ARENA)
	{
		return CUSTOM_ARENA_STATE_BLANK;
	}

	return this->GetUserCount(&this->m_CustomArenaInfo[index]);
}

bool CCustomArena::CheckEnteredUser(int index,int aIndex) // OK
{
	if(index < 0 || index >= MAX_CUSTOM_ARENA)
	{
		return 0;
	}

	return ((this->GetUser(&this->m_CustomArenaInfo[index],aIndex)==0)?0:1);
}

bool CCustomArena::CheckEnterEnabled(LPOBJ lpObj,int gate) // OK
{
	if(OBJECT_RANGE(lpObj->PartyNumber) != 0)
	{
		GCNewMessageSend(lpObj->Index,1,2,gMessage.GetMessage(508));
		return 0;
	}

	if(lpObj->Guild != 0 && lpObj->Guild->WarState == 1)
	{
		GCNewMessageSend(lpObj->Index,1,2,gMessage.GetMessage(508));
		return 0;
	}

	if(OBJECT_RANGE(lpObj->DuelUser) != 0)
	{
		GCNewMessageSend(lpObj->Index,1,2,gMessage.GetMessage(508));
		return 0;
	}

	if(lpObj->PShopOpen != 0)
	{
		GCNewMessageSend(lpObj->Index,1,2,gMessage.GetMessage(508));
		return 0;
	}

	if(this->CheckMap(lpObj->Map) != 0)
	{
		GCNewMessageSend(lpObj->Index,1,2,gMessage.GetMessage(508));
		return 0;
	}

	if(gMapServerManager.CheckMapServer(this->m_CustomArenaMapNumber) == 0)
	{
		GCNewMessageSend(lpObj->Index,1,2,gMessage.GetMessage(508));
		return 0;
	}

	for(int n=0;n < MAX_CUSTOM_ARENA;n++)
	{
		CUSTOM_ARENA_INFO* lpInfo = &this->m_CustomArenaInfo[n];

		if(lpInfo->RuleInfo.StartGate != gate)
		{
			continue;
		}

		if(this->GetEnterEnabled(lpInfo->Index) == 0)
		{
			GCNewMessageSend(lpObj->Index,1,2,gMessage.GetMessage(509));
			return 0;
		}

		if(this->GetUserCount(lpInfo) >= ((lpInfo->RuleInfo.MaxUser>MAX_CUSTOM_ARENA_USER)?MAX_CUSTOM_ARENA_USER:lpInfo->RuleInfo.MaxUser))
		{
			GCNewMessageSend(lpObj->Index,1,2,gMessage.GetMessage(510));
			return 0;
		}

		if(lpInfo->RuleInfo.MinLevel != -1 && lpInfo->RuleInfo.MinLevel > lpObj->Level)
		{
			GCNewMessageSend(lpObj->Index,1,2,gMessage.GetMessage(511),lpInfo->RuleInfo.MinLevel);
			return 0;
		}

		if(lpInfo->RuleInfo.MaxLevel != -1 && lpInfo->RuleInfo.MaxLevel < lpObj->Level)
		{
			GCNewMessageSend(lpObj->Index,1,2,gMessage.GetMessage(512),lpInfo->RuleInfo.MaxLevel);
			return 0;
		}

		if(lpInfo->RuleInfo.MinMasterLevel != -1 && lpInfo->RuleInfo.MinMasterLevel > lpObj->MasterLevel)
		{
			GCNewMessageSend(lpObj->Index,1,2,gMessage.GetMessage(513),lpInfo->RuleInfo.MinMasterLevel);
			return 0;
		}

		if(lpInfo->RuleInfo.MaxMasterLevel != -1 && lpInfo->RuleInfo.MaxMasterLevel < lpObj->MasterLevel)
		{
			GCNewMessageSend(lpObj->Index,1,2,gMessage.GetMessage(514),lpInfo->RuleInfo.MaxMasterLevel);
			return 0;
		}

		if(lpInfo->RuleInfo.MinReset != -1 && lpInfo->RuleInfo.MinReset > lpObj->Reset)
		{
			GCNewMessageSend(lpObj->Index,1,2,gMessage.GetMessage(515),lpInfo->RuleInfo.MinReset);
			return 0;
		}

		if(lpInfo->RuleInfo.MaxReset != -1 && lpInfo->RuleInfo.MaxReset < lpObj->Reset)
		{
			GCNewMessageSend(lpObj->Index,1,2,gMessage.GetMessage(516),lpInfo->RuleInfo.MaxReset);
			return 0;
		}

		if(lpInfo->RuleInfo.MinMasterReset != -1 && lpInfo->RuleInfo.MinMasterReset > lpObj->MasterReset)
		{
			GCNewMessageSend(lpObj->Index,1,2,gMessage.GetMessage(517),lpInfo->RuleInfo.MinMasterReset);
			return 0;
		}

		if(lpInfo->RuleInfo.MaxMasterReset != -1 && lpInfo->RuleInfo.MaxMasterReset < lpObj->MasterReset)
		{
			GCNewMessageSend(lpObj->Index,1,2,gMessage.GetMessage(518),lpInfo->RuleInfo.MaxMasterReset);
			return 0;
		}

		if(lpInfo->RuleInfo.RequireClass[lpObj->Class] == 0 || lpInfo->RuleInfo.RequireClass[lpObj->Class] > (lpObj->ChangeUp+1))
		{
			GCNewMessageSend(lpObj->Index,1,2,gMessage.GetMessage(519));
			return 0;
		}

		gEffectManager.ClearAllEffect(lpObj);

		return this->AddUser(lpInfo,lpObj->Index);
	}

	return 0;
}

bool CCustomArena::CheckPlayerJoined(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	if(this->CheckMap(lpObj->Map) != 0)
	{
		for(int n=0;n < MAX_CUSTOM_ARENA;n++)
		{
			if(this->GetState(n) != CUSTOM_ARENA_STATE_BLANK)
			{
				if(this->CheckEnteredUser(n,lpObj->Index) != 0)
				{
					return 1;
				}
			}
		}
	}

	return 0;
}

bool CCustomArena::CheckPlayerTarget(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	if(this->CheckMap(lpObj->Map) != 0)
	{
		for(int n=0;n < MAX_CUSTOM_ARENA;n++)
		{
			if(this->GetState(n) == CUSTOM_ARENA_STATE_START)
			{
				if(this->CheckEnteredUser(n,lpObj->Index) != 0)
				{
					return 1;
				}
			}
		}
	}

	return 0;
}

bool CCustomArena::CheckMap(int index) // OK
{
	if(index == this->m_CustomArenaMapNumber)
	{
		return 1;
	}

	return 0;
}

bool CCustomArena::AddUser(CUSTOM_ARENA_INFO* lpInfo,int aIndex) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	if(this->GetUser(lpInfo,aIndex) != 0)
	{
		return 0;
	}

	for(int n=0;n < MAX_CUSTOM_ARENA_USER;n++)
	{
		if(OBJECT_RANGE(lpInfo->User[n].Index) != 0)
		{
			continue;
		}

		lpInfo->User[n].Index = aIndex;
		lpInfo->User[n].Score = 0;
		lpInfo->User[n].Rank = 0;
		lpInfo->User[n].UserKillCount = 0;
		lpInfo->User[n].UserKillStreak = 0;
		lpInfo->User[n].UserDeathCount = 0;
		lpInfo->User[n].UserDeathStreak = 0;
		return 1;
	}

	return 0;
}

bool CCustomArena::DelUser(CUSTOM_ARENA_INFO* lpInfo,int aIndex) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	CUSTOM_ARENA_USER* lpUser = this->GetUser(lpInfo,aIndex);

	if(lpUser == 0)
	{
		return 0;
	}

	lpUser->Reset();
	return 1;
}

CUSTOM_ARENA_USER* CCustomArena::GetUser(CUSTOM_ARENA_INFO* lpInfo,int aIndex) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	for(int n=0;n < MAX_CUSTOM_ARENA_USER;n++)
	{
		if(lpInfo->User[n].Index == aIndex)
		{
			return &lpInfo->User[n];
		}
	}

	return 0;
}

void CCustomArena::CleanUser(CUSTOM_ARENA_INFO* lpInfo) // OK
{
	for(int n=0;n < MAX_CUSTOM_ARENA_USER;n++)
	{
		lpInfo->User[n].Reset();
	}
}

void CCustomArena::ClearUser(CUSTOM_ARENA_INFO* lpInfo) // OK
{
	for(int n=0;n < MAX_CUSTOM_ARENA_USER;n++)
	{
		if(OBJECT_RANGE(lpInfo->User[n].Index) == 0)
		{
			continue;
		}

		gObjMoveGate(lpInfo->User[n].Index,lpInfo->RuleInfo.FinalGate);

		lpInfo->User[n].Reset();
	}
}

void CCustomArena::CheckUser(CUSTOM_ARENA_INFO* lpInfo) // OK
{
	for(int n=0;n < MAX_CUSTOM_ARENA_USER;n++)
	{
		if(OBJECT_RANGE(lpInfo->User[n].Index) == 0)
		{
			continue;
		}

		if(gObjIsConnected(lpInfo->User[n].Index) == 0)
		{
			this->DelUser(lpInfo,lpInfo->User[n].Index);
			continue;
		}

		if(this->CheckMap(gObj[lpInfo->User[n].Index].Map) == 0)
		{
			this->DelUser(lpInfo,lpInfo->User[n].Index);
			continue;
		}
	}
}

int CCustomArena::GetUserCount(CUSTOM_ARENA_INFO* lpInfo) // OK
{
	int count = 0;

	for(int n=0;n < MAX_CUSTOM_ARENA_USER;n++)
	{
		if(OBJECT_RANGE(lpInfo->User[n].Index) != 0)
		{
			count++;
		}
	}

	return count;
}

void CCustomArena::CalcUserRank(CUSTOM_ARENA_INFO* lpInfo) // OK
{
	for(int n=0;n < MAX_CUSTOM_ARENA_USER;n++)
	{
		if(OBJECT_RANGE(lpInfo->User[n].Index) == 0)
		{
			continue;
		}

		int rank = MAX_CUSTOM_ARENA_USER;

		for(int i=0;i < MAX_CUSTOM_ARENA_USER;i++)
		{
			if(OBJECT_RANGE(lpInfo->User[i].Index) == 0)
			{
				rank--;
				continue;
			}

			if(lpInfo->User[n].Index == lpInfo->User[i].Index)
			{
				rank--;
				continue;
			}

			if(lpInfo->User[n].Score > lpInfo->User[i].Score)
			{
				rank--;
				continue;
			}

			if(lpInfo->User[n].Score == lpInfo->User[i].Score && n < i)
			{
				rank--;
				continue;
			}
		}

		lpInfo->User[n].Rank = (rank+1);
	}
}

bool CCustomArena::GetUserRespawnLocation(LPOBJ lpObj,int* gate,int* map,int* x,int* y,int* dir,int* level) // OK
{
	if(this->m_CustomArenaSwitch == 0)
	{
		return 0;
	}

	for(int n=0;n < MAX_CUSTOM_ARENA;n++)
	{
		CUSTOM_ARENA_INFO* lpInfo = &this->m_CustomArenaInfo[n];

		if(this->GetState(lpInfo->Index) != CUSTOM_ARENA_STATE_START)
		{
			continue;
		}

		CUSTOM_ARENA_USER* lpUser = this->GetUser(lpInfo,lpObj->Index);

		if(lpUser == 0)
		{
			continue;
		}

		if(lpInfo->RuleInfo.MaxDeath != -1 && lpUser->UserDeathCount >= lpInfo->RuleInfo.MaxDeath)
		{
			if(gGate.GetGate(lpInfo->RuleInfo.FinalGate,gate,map,x,y,dir,level) != 0)
			{
				this->DelUser(lpInfo,lpObj->Index);
				return 1;
			}
		}
		else
		{
			if(gGate.GetGate(lpInfo->RuleInfo.StartGate,gate,map,x,y,dir,level) != 0)
			{
				return 1;
			}
		}
	}

	return 0;
}

void CCustomArena::UserDieProc(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	if(this->m_CustomArenaSwitch == 0)
	{
		return;
	}

	for(int n=0;n < MAX_CUSTOM_ARENA;n++)
	{
		CUSTOM_ARENA_INFO* lpInfo = &this->m_CustomArenaInfo[n];

		if(this->GetState(lpInfo->Index) != CUSTOM_ARENA_STATE_START)
		{
			continue;
		}

		CUSTOM_ARENA_USER* lpUserA = this->GetUser(lpInfo,lpObj->Index);

		if(lpUserA == 0)
		{
			return;
		}

		CUSTOM_ARENA_USER* lpUserB = this->GetUser(lpInfo,lpTarget->Index);

		if(lpUserB == 0)
		{
			return;
		}

		lpUserA->Score -= ((lpUserA->Score>this->m_CustomArenaVictimScoreDecrease)?this->m_CustomArenaVictimScoreDecrease:lpUserA->Score);

		lpUserA->UserKillStreak = 0;

		lpUserA->UserDeathCount++;

		lpUserA->UserDeathStreak++;

		lpUserB->Score += this->m_CustomArenaKillerScoreIncrease;

		lpUserB->UserKillCount++;

		lpUserB->UserKillStreak++;

		lpUserB->UserDeathStreak = 0;
	}
}

void CCustomArena::NoticeSendToAll(CUSTOM_ARENA_INFO* lpInfo,int type,char* message,...) // OK
{
	char buff[256];

	va_list arg;
	va_start(arg,message);
	vsprintf_s(buff,message,arg);
	va_end(arg);

	for(int n=0;n < MAX_CUSTOM_ARENA_USER;n++)
	{
		if(OBJECT_RANGE(lpInfo->User[n].Index) != 0)
		{
			gNotice.GCNoticeSend(lpInfo->User[n].Index,type,0,0,0,0,0,buff);
		}
	}
}

void CCustomArena::DataSendToAll(CUSTOM_ARENA_INFO* lpInfo,BYTE* lpMsg,int size) // OK
{
	for(int n=0;n < MAX_CUSTOM_ARENA_USER;n++)
	{
		if(OBJECT_RANGE(lpInfo->User[n].Index) != 0)
		{
			DataSend(lpInfo->User[n].Index,lpMsg,size);
		}
	}
}
