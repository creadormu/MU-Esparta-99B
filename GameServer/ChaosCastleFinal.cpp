// ChaosCastleFinal.cpp: implementation of the CChaosCastleFinal class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChaosCastleFinal.h"
#include "ChaosCastle.h"
#include "DSProtocol.h"
#include "EffectManager.h"
#include "EventSpawnStage.h"
#include "GameMain.h"
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
#include "ScheduleManager.h"
#include "ServerInfo.h"
#include "Util.h"

CChaosCastleFinal gChaosCastleFinal;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChaosCastleFinal::CChaosCastleFinal() // OK
{
	#if(GAMESERVER_UPDATE>=803)

	this->m_State = CCF_STATE_BLANK;
	this->m_RemainTime = 0;
	this->m_TargetTime = 0;
	this->m_TickCount = GetTickCount();
	this->m_EnterEnabled = 0;
	this->m_MinutesLeft = -1;
	this->m_TimeCount = 0;
	this->m_Stage = 0;
	this->m_MaxMonster = 0;
	this->m_WinnerIndex = -1;

	this->CleanUser();

	this->CleanMonster();

	#endif
}

CChaosCastleFinal::~CChaosCastleFinal() // OK
{

}

void CChaosCastleFinal::Init() // OK
{
	#if(GAMESERVER_UPDATE>=803)

	if(gServerInfo.m_ChaosCastleSurvivalEvent == 0)
	{
		this->SetState(CCF_STATE_BLANK);
	}
	else
	{
		this->SetState(CCF_STATE_EMPTY);
	}

	#endif
}

void CChaosCastleFinal::Load(char* path) // OK
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

	this->m_ChaosCastleFinalStartTime.clear();

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

					this->m_Tryout1 = lpMemScript->GetNumber();
					
					this->m_Tryout2 = lpMemScript->GetAsNumber();
					
					this->m_Tryout3 = lpMemScript->GetAsNumber();
					
					this->m_SemiFinal1 = lpMemScript->GetAsNumber();
					
					this->m_SemiFinal2 = lpMemScript->GetAsNumber();
					
					this->m_Final = lpMemScript->GetAsNumber();
				}
				else if(section == 1)
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
				else if(section == 2)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					CHAOS_CASTLE_FINAL_START_TIME info;

					info.Hour = lpMemScript->GetNumber();

					info.Minute = lpMemScript->GetAsNumber();

					info.Second = lpMemScript->GetAsNumber();

					info.Stage1 = lpMemScript->GetAsNumber();

					info.Stage2 = lpMemScript->GetAsNumber();

					info.Stage3 = lpMemScript->GetAsNumber();

					this->m_ChaosCastleFinalStartTime.push_back(info);
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

void CChaosCastleFinal::MainProc() // OK
{
	#if(GAMESERVER_UPDATE>=803)

	if((GetTickCount()-this->m_TickCount) >= 1000)
	{
		this->m_TickCount = GetTickCount();

		this->m_RemainTime = (int)difftime(this->m_TargetTime,time(0));

		switch(this->m_State)
		{
			case CCF_STATE_BLANK:
				this->ProcState_BLANK();
				break;
			case CCF_STATE_EMPTY:
				this->ProcState_EMPTY();
				break;
			case CCF_STATE_STAND:
				this->ProcState_STAND();
				break;
			case CCF_STATE_START:
				this->ProcState_START();
				break;
			case CCF_STATE_CLEAN:
				this->ProcState_CLEAN();
				break;
		}

		this->CheckDelayScore();
	}
	
	#endif
}

void CChaosCastleFinal::ProcState_BLANK() // OK
{

}

void CChaosCastleFinal::ProcState_EMPTY() // OK
{
	#if(GAMESERVER_UPDATE>=803)

	if(this->m_RemainTime > 0 && this->m_RemainTime <= (this->m_WarningTime*60))
	{
		this->CheckUser();

		int minutes = this->m_RemainTime/60;

		if((this->m_RemainTime % 60) == 0)
		{
			minutes--;
		}

		if(this->m_MinutesLeft != minutes)
		{
			this->m_MinutesLeft = minutes;

			this->SendMapServerGroupMsg(gMessage.GetMessage(23),(this->m_MinutesLeft+1));
		}

		this->m_EnterEnabled = 1;
	}

	if(this->m_RemainTime > 0 && this->m_RemainTime <= 30 && this->m_TimeCount == 0)
	{
		PMSG_TIME_COUNT_SEND pMsg;

		pMsg.header.set(0x92,sizeof(pMsg));

		pMsg.type = 20;

		DataSendAll((BYTE*)&pMsg,pMsg.header.size);

		this->m_TimeCount = 1;
	}

	if(this->m_RemainTime <= 0)
	{
		this->SendMapServerGroupMsg(gMessage.GetMessage(24));
		this->NoticeSendToAll(1,gMessage.GetMessage(25),this->m_NotifyTime);
		this->SetState(CCF_STATE_STAND);
	}
	
	#endif
}

void CChaosCastleFinal::ProcState_STAND() // OK
{
	#if(GAMESERVER_UPDATE>=803)

	this->CheckUser();

	if(this->GetUserCount() < gServerInfo.m_ChaosCastleSurvivalMinUser)
	{
		LogAdd(LOG_BLACK,"[Chaos Castle Survival] Not enough users");
		this->NoticeSendToAll(0,gMessage.GetMessage(28));
		this->SetState(CCF_STATE_EMPTY);
		return;
	}

	if(this->m_RemainTime > 0 && this->m_RemainTime <= 30 && this->m_TimeCount == 0)
	{
		PMSG_TIME_COUNT_SEND pMsg;

		pMsg.header.set(0x92,sizeof(pMsg));

		pMsg.type = 21;

		this->DataSendToAll((BYTE*)&pMsg,pMsg.header.size);

		this->m_TimeCount = 1;
	}

	if(this->m_RemainTime <= 0)
	{
		this->NoticeSendToAll(0,gMessage.GetMessage(26));

		this->SetState(CCF_STATE_START);
	}
	
	#endif
}

void CChaosCastleFinal::ProcState_START() // OK
{
	#if(GAMESERVER_UPDATE>=803)

	this->CheckUser();

	if(this->GetUserCount() == 0)
	{
		LogAdd(LOG_BLACK,"[Chaos Castle Survival] Not enough users");
		this->SetState(CCF_STATE_EMPTY);
		return;
	}

	switch(this->m_Stage)
	{
	case 0:
		this->SetStage0();
		break;
	case 1:
		this->SetStage1();
		break;
	case 2:
		this->SetStage2();
		break;
	case 3:
		this->SetStage3();
		break;
	}

	this->CheckUserPosition();
	this->CheckMonsterPosition();
	this->CheckItemPosition();

	this->GCChaosCastleFinalStateSend(6);

	if(this->GetUserCount() == 1 && this->GetMonsterCount() == 0)
	{
		this->SetState(CCF_STATE_CLEAN);
		return;
	}

	if(this->m_RemainTime > 0 && this->m_RemainTime <= 30 && this->m_TimeCount == 0)
	{
		PMSG_TIME_COUNT_SEND pMsg;

		pMsg.header.set(0x92,sizeof(pMsg));

		pMsg.type = 22;

		this->DataSendToAll((BYTE*)&pMsg,pMsg.header.size);

		this->m_TimeCount = 1;
	}

	if(this->m_RemainTime <= 0)
	{
		this->NoticeSendToAll(0,gMessage.GetMessage(27));

		this->SetState(CCF_STATE_CLEAN);
	}
	
	#endif
}

void CChaosCastleFinal::ProcState_CLEAN() // OK
{
	#if(GAMESERVER_UPDATE>=803)

	this->CheckUser();

	if(this->m_RemainTime > 0 && this->m_RemainTime <= 30 && this->m_TimeCount == 0)
	{
		PMSG_TIME_COUNT_SEND pMsg;

		pMsg.header.set(0x92,sizeof(pMsg));

		pMsg.type = 23;

		this->DataSendToAll((BYTE*)&pMsg,pMsg.header.size);

		this->m_TimeCount = 1;
	}

	if(this->m_RemainTime <= 0)
	{
		this->SetState(CCF_STATE_EMPTY);
	}
	
	#endif
}

void CChaosCastleFinal::SetState(int state) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	switch((this->m_State=state))
	{
		case CCF_STATE_BLANK:
			this->SetState_BLANK();
			break;
		case CCF_STATE_EMPTY:
			this->SetState_EMPTY();
			break;
		case CCF_STATE_STAND:
			this->SetState_STAND();
			break;
		case CCF_STATE_START:
			this->SetState_START();
			break;
		case CCF_STATE_CLEAN:
			this->SetState_CLEAN();
			break;
	}
	
	#endif
}

void CChaosCastleFinal::SetState_BLANK() // OK
{
	
}

void CChaosCastleFinal::SetState_EMPTY() // OK
{
	#if(GAMESERVER_UPDATE>=803)

	this->m_EnterEnabled = 0;
	this->m_MinutesLeft = -1;
	this->m_TimeCount = 0;
	this->m_Stage = 0;
	this->m_MaxMonster = 0;
	this->m_WinnerIndex = -1;

	this->GCChaosCastleFinalSimpleStateSend(7,-1);

	this->ClearUser();

	this->ClearMonster();

	this->SetSafeZone();

	this->DelHollowZone(1);

	this->DelHollowZone(2);

	this->DelHollowZone(3);

	this->DelCenterHollowZone();

	this->CheckSync();

	LogAdd(LOG_BLACK,"[Chaos Castle Survival] SetState EMPTY");
	
	#endif
}

void CChaosCastleFinal::SetState_STAND() // OK
{
	#if(GAMESERVER_UPDATE>=803)

	this->m_EnterEnabled = 0;
	this->m_MinutesLeft = -1;
	this->m_TimeCount = 0;

	this->SetCenterHollowZone();

	this->m_RemainTime = this->m_NotifyTime*60;

	this->m_TargetTime = (int)(time(0)+this->m_RemainTime);

	LogAdd(LOG_BLACK,"[Chaos Castle Survival] SetState STAND");
	
	#endif
}

void CChaosCastleFinal::SetState_START() // OK
{
	#if(GAMESERVER_UPDATE>=803)

	this->m_EnterEnabled = 0;
	this->m_MinutesLeft = -1;
	this->m_TimeCount = 0;
	this->m_MaxMonster = this->GetUserCount();

	this->DelSafeZone();

	this->StageSpawn(0);

	this->GCChaosCastleFinalSimpleStateSend(5,-1);

	this->m_RemainTime = this->m_EventTime*60;

	this->m_TargetTime = (int)(time(0)+this->m_RemainTime);

	LogAdd(LOG_BLACK,"[Chaos Castle Survival] SetState START");
	
	#endif
}

void CChaosCastleFinal::SetState_CLEAN() // OK
{
	#if(GAMESERVER_UPDATE>=803)

	this->m_EnterEnabled = 0;
	this->m_MinutesLeft = -1;
	this->m_TimeCount = 0;
	this->m_WinnerIndex = this->GetUserWinner();

	this->ClearMonster();

	for(int n = 0; n < MAX_CCF_USER; n++)
	{
		if(OBJECT_RANGE(this->User[n].Index) != 0)
		{
			if(this->User[n].Index == this->m_WinnerIndex)
			{
				this->User[n].Score += 3;
				this->GCChaosCastleFinalScoreSend(this->User[n].Index,1);
				this->GDChaosCastleFinalScoreSaveSend(this->User[n].Index);
				if(this->GetDay() == 3)
				{
					this->GDChaosCastleFinalScoreClearSend();
					gItemBagManager.DropItemBySpecialValue(ITEM_BAG_SPECIAL_CHAOS_CASTLE_FINAL,-1,-1,&gObj[this->User[n].Index],gObj[this->User[n].Index].Map,gObj[this->User[n].Index].X,gObj[this->User[n].Index].Y);
					LogAdd(LOG_BLACK,"[Chaos Castle Survival] Finalized Winner[%s][%s]",gObj[this->User[n].Index].Account,gObj[this->User[n].Index].Name);
				}
				
			}
			else
			{
				this->GCChaosCastleFinalScoreSend(this->User[n].Index,2);
				this->GDChaosCastleFinalScoreSaveSend(this->User[n].Index);
			}
		}
	}

	this->GCChaosCastleFinalSimpleStateSend(7,-1);

	this->m_RemainTime = this->m_CloseTime*60;

	this->m_TargetTime = (int)(time(0)+this->m_RemainTime);

	LogAdd(LOG_BLACK,"[Chaos Castle Survival] SetState CLEAN");
	
	#endif
}

void CChaosCastleFinal::SetStage0() // OK
{
	#if(GAMESERVER_UPDATE>=803)

	if((this->GetUserCount()+this->GetMonsterCount()) > 40)
	{
		return;
	}

	this->m_Stage = 1;

	this->GCChaosCastleFinalSimpleStateSend(8,-1);

	this->SetHollowZone(1);
	
	#endif
}

void CChaosCastleFinal::SetStage1() // OK
{
	#if(GAMESERVER_UPDATE>=803)

	if((this->GetUserCount()+this->GetMonsterCount()) > 30)
	{
		return;
	}

	this->m_Stage = 2;

	this->GCChaosCastleFinalSimpleStateSend(9,-1);

	this->SetHollowZone(2);
	
	#endif
}

void CChaosCastleFinal::SetStage2() // OK
{
	#if(GAMESERVER_UPDATE>=803)

	if((this->GetUserCount()+this->GetMonsterCount()) > 20)
	{
		return;
	}

	this->m_Stage = 3;

	this->GCChaosCastleFinalSimpleStateSend(10,-1);

	this->SetHollowZone(3);
	
	#endif
}

void CChaosCastleFinal::SetStage3() // OK
{

}

void CChaosCastleFinal::CheckSync() // OK
{
	#if(GAMESERVER_UPDATE>=803)

	if(this->m_ChaosCastleFinalStartTime.empty() != 0)
	{
		this->SetState(CCF_STATE_BLANK);
		return;
	}

	CTime ScheduleTime;

	CScheduleManager ScheduleManager;

	for(std::vector<CHAOS_CASTLE_FINAL_START_TIME>::iterator it = this->m_ChaosCastleFinalStartTime.begin(); it != this->m_ChaosCastleFinalStartTime.end(); it++)
	{
		if(it->Stage1 != -1)
		{
			ScheduleManager.AddSchedule(-1,-1,this->m_Tryout1,it->Hour,it->Minute,it->Second,-1);
			ScheduleManager.AddSchedule(-1,-1,this->m_Tryout2,it->Hour,it->Minute,it->Second,-1);
			ScheduleManager.AddSchedule(-1,-1,this->m_Tryout3,it->Hour,it->Minute,it->Second,-1);
		}

		if(it->Stage2 != -1)
		{
			ScheduleManager.AddSchedule(-1,-1,this->m_SemiFinal1,it->Hour,it->Minute,it->Second,-1);
			ScheduleManager.AddSchedule(-1,-1,this->m_SemiFinal2,it->Hour,it->Minute,it->Second,-1);
		}

		if(it->Stage3 != -1)
		{
			ScheduleManager.AddSchedule(-1,-1,this->m_Final,it->Hour,it->Minute,it->Second,-1);
		}
	}

	if(ScheduleManager.GetSchedule(&ScheduleTime) == 0)
	{
		this->SetState(CCF_STATE_BLANK);
		return;
	}

	this->m_RemainTime = (int)difftime(ScheduleTime.GetTime(),time(0));

	this->m_TargetTime = (int)ScheduleTime.GetTime();

	LogAdd(LOG_BLACK,"[Chaos Castle Survival] Sync Start Time. [%d] min remain",(this->m_RemainTime/60));
	
	#endif
}

int CChaosCastleFinal::GetState() // OK
{
	#if(GAMESERVER_UPDATE>=803)

	return this->m_State;

	#else

	return 0;

	#endif
}

int CChaosCastleFinal::GetRemainTime() // OK
{
	#if(GAMESERVER_UPDATE>=803)

	if(this->m_ChaosCastleFinalStartTime.empty() != 0)
	{
		return 0;
	}

	CTime ScheduleTime;

	CScheduleManager ScheduleManager;

	for(std::vector<CHAOS_CASTLE_FINAL_START_TIME>::iterator it = this->m_ChaosCastleFinalStartTime.begin(); it != this->m_ChaosCastleFinalStartTime.end(); it++)
	{
		if(it->Stage1 != -1)
		{
			ScheduleManager.AddSchedule(-1,-1,this->m_Tryout1,it->Hour,it->Minute,it->Second,-1);
			ScheduleManager.AddSchedule(-1,-1,this->m_Tryout2,it->Hour,it->Minute,it->Second,-1);
			ScheduleManager.AddSchedule(-1,-1,this->m_Tryout3,it->Hour,it->Minute,it->Second,-1);
		}

		if(it->Stage2 != -1)
		{
			ScheduleManager.AddSchedule(-1,-1,this->m_SemiFinal1,it->Hour,it->Minute,it->Second,-1);
			ScheduleManager.AddSchedule(-1,-1,this->m_SemiFinal2,it->Hour,it->Minute,it->Second,-1);
		}

		if(it->Stage3 != -1)
		{
			ScheduleManager.AddSchedule(-1,-1,this->m_Final,it->Hour,it->Minute,it->Second,-1);
		}
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

bool CChaosCastleFinal::AddUser(int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	if(this->GetUser(aIndex) != 0)
	{
		return 0;
	}

	for(int n = 0; n < MAX_CCF_USER; n++)
	{
		if(OBJECT_RANGE(this->User[n].Index) != 0)
		{
			continue;
		}

		this->User[n].Index = aIndex;
		this->User[n].Score = 0;
		this->User[n].UserKillCount = 0;
		this->User[n].MonsterKillCount = 0;
		this->User[n].RewardExperience = 0;
		return 1;
	}

	return 0;
	
	#else

	return 0;
	
	#endif
}

bool CChaosCastleFinal::DelUser(int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	CHAOS_CASTLE_FINAL_USER* lpUser = this->GetUser(aIndex);

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
	
	#else

	return 0;
	
	#endif
}

CHAOS_CASTLE_FINAL_USER* CChaosCastleFinal::GetUser(int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	for(int n = 0; n < MAX_CCF_USER; n++)
	{
		if(this->User[n].Index == aIndex)
		{
			return &this->User[n];
		}
	}

	return 0;
	
	#else

	return 0;
	
	#endif
}

void CChaosCastleFinal::CleanUser() // OK
{
	#if(GAMESERVER_UPDATE>=803)

	for(int n = 0; n < MAX_CCF_USER; n++)
	{
		this->User[n].Index = -1;
		this->User[n].Score = 0;
		this->User[n].UserKillCount = 0;
		this->User[n].MonsterKillCount = 0;
		this->User[n].RewardExperience = 0;
	}
	
	#endif
}

void CChaosCastleFinal::ClearUser() // OK
{
	#if(GAMESERVER_UPDATE>=803)

	for(int n = 0; n < MAX_CCF_USER; n++)
	{
		if(OBJECT_RANGE(this->User[n].Index) == 0)
		{
			continue;
		}

		gObjMoveGate(this->User[n].Index,22);

		this->User[n].Index = -1;
		this->User[n].Score = 0;
		this->User[n].UserKillCount = 0;
		this->User[n].MonsterKillCount = 0;
		this->User[n].RewardExperience = 0;
	}
	
	#endif
}

void CChaosCastleFinal::CheckUser() // OK
{
	#if(GAMESERVER_UPDATE>=803)

	for(int n = 0; n < MAX_CCF_USER; n++)
	{
		if(OBJECT_RANGE(this->User[n].Index) == 0)
		{
			continue;
		}

		if(gObjIsConnected(this->User[n].Index) == 0)
		{
			this->DelUser(this->User[n].Index);
			continue;
		}

		if(gObj[this->User[n].Index].Map != MAP_CHAOS_CASTLE_FINAL)
		{
			this->DelUser(this->User[n].Index);
			continue;
		}
	}
	
	#endif
}

int CChaosCastleFinal::GetUserCount() // OK
{
	#if(GAMESERVER_UPDATE>=803)

	int count = 0;

	for(int n = 0; n < MAX_CCF_USER; n++)
	{
		if(OBJECT_RANGE(this->User[n].Index) != 0)
		{
			count++;
		}
	}

	return count;
	
	#else

	return 0;
	
	#endif
}

int CChaosCastleFinal::GetUserWinner() // OK
{
	#if(GAMESERVER_UPDATE>=803)

	if(this->GetMonsterCount() > 0)
	{
		return -1;
	}

	int WinnerIndex = -1;
	int WinnerScore = -1;

	for(int n = 0; n < MAX_CCF_USER; n++)
	{
		if(OBJECT_RANGE(this->User[n].Index) == 0)
		{
			continue;
		}

		if(this->User[n].Score > WinnerScore)
		{
			WinnerIndex = this->User[n].Index;
			WinnerScore = this->User[n].Score;
		}
	}

	return WinnerIndex;
	
	#else

	return -1;
	
	#endif
}

bool CChaosCastleFinal::AddMonster(int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	if(this->GetMonster(aIndex) != 0)
	{
		return 0;
	}

	for(int n = 0; n < MAX_CCF_MONSTER; n++)
	{
		if(OBJECT_RANGE(this->MonsterIndex[n]) != 0)
		{
			continue;
		}

		this->MonsterIndex[n] = aIndex;
		return 1;
	}

	return 0;
	
	#else

	return 0;
	
	#endif
}

bool CChaosCastleFinal::DelMonster(int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	int* index = this->GetMonster(aIndex);

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

int* CChaosCastleFinal::GetMonster(int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	for(int n = 0; n < MAX_CCF_MONSTER; n++)
	{
		if(this->MonsterIndex[n] == aIndex)
		{
			return &this->MonsterIndex[n];
		}
	}

	return 0;
	
	#else

	return 0;
	
	#endif
}

void CChaosCastleFinal::CleanMonster() // OK
{
	#if(GAMESERVER_UPDATE>=803)

	for(int n = 0; n < MAX_CCF_MONSTER; n++)
	{
		this->MonsterIndex[n] = -1;
	}
	
	#endif
}

void CChaosCastleFinal::ClearMonster() // OK
{
	#if(GAMESERVER_UPDATE>=803)

	for(int n = 0; n < MAX_CCF_MONSTER; n++)
	{
		if(OBJECT_RANGE(this->MonsterIndex[n]) != 0)
		{
			gObjDel(this->MonsterIndex[n]);
			this->MonsterIndex[n] = -1;
		}
	}
	
	#endif
}

int CChaosCastleFinal::GetMonsterCount() // OK
{
	#if(GAMESERVER_UPDATE>=803)

	int count = 0;

	for(int n = 0; n < MAX_CCF_MONSTER; n++)
	{
		if(OBJECT_RANGE(this->MonsterIndex[n]) != 0)
		{
			count++;
		}
	}

	return count;
	
	#else

	return 0;
	
	#endif
}

void CChaosCastleFinal::StageSpawn(int Stage) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	for(std::vector<EVENT_SPAWN_STAGE_INFO>::iterator it = gEventSpawnStage.m_EventSpawnStageInfo.begin(); it != gEventSpawnStage.m_EventSpawnStageInfo.end(); it++)
	{
		if(it->Event != 4 || it->Stage != Stage)
		{
			continue;
		}

		this->SetMonster(it->MonsterClass,it->MaxRegenTime);
	}
	
	#endif
}

void CChaosCastleFinal::SetMonster(int MonsterClass,int MaxRegenTime) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	for(int n=0;n < gMonsterSetBase.m_count;n++)
	{
		if(this->m_MaxMonster >= MAX_CCF_MONSTER)
		{
			break;
		}

		MONSTER_SET_BASE_INFO* lpInfo = &gMonsterSetBase.m_MonsterSetBaseInfo[n];

		if(lpInfo->Type != 4 || lpInfo->MonsterClass != MonsterClass || lpInfo->Map != MAP_CHAOS_CASTLE_FINAL)
		{
			continue;
		}

		int index = gObjAddMonster(MAP_CHAOS_CASTLE_FINAL);

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

		if(this->AddMonster(index) == 0)
		{
			gObjDel(index);
			continue;
		}

		this->m_MaxMonster++;
	}
	
	#endif
}

void CChaosCastleFinal::SetSafeZone() // OK
{
	#if(GAMESERVER_UPDATE>=803)

	for(int x = gChaosCastleSafeZone[0]; x <= gChaosCastleSafeZone[2]; x++)
	{
		for(int y = gChaosCastleSafeZone[1]; y <= gChaosCastleSafeZone[3]; y++)
		{
			gMap[MAP_CHAOS_CASTLE_FINAL].SetAttr(x,y,1);
		}
	}
	
	for(int n = 0; n < MAX_CCF_USER; n++)
	{
		if(OBJECT_RANGE(this->User[n].Index) != 0)
		{
			GCMapAttrSend(this->User[n].Index,0,1,0,1,(PMSG_MAP_ATTR*)gChaosCastleSafeZone);
		}
	}
	
	#endif
}

void CChaosCastleFinal::DelSafeZone() // OK
{
	#if(GAMESERVER_UPDATE>=803)

	for(int x = gChaosCastleSafeZone[0]; x <= gChaosCastleSafeZone[2]; x++)
	{
		for(int y = gChaosCastleSafeZone[1]; y <= gChaosCastleSafeZone[3]; y++)
		{
			gMap[MAP_CHAOS_CASTLE_FINAL].DelAttr(x,y,1);
		}
	}

	for(int n = 0; n < MAX_CCF_USER; n++)
	{
		if(OBJECT_RANGE(this->User[n].Index) != 0)
		{
			GCMapAttrSend(this->User[n].Index,0,1,1,1,(PMSG_MAP_ATTR*)gChaosCastleSafeZone);
		}
	}
	
	#endif
}

void CChaosCastleFinal::SetHollowZone(int stage) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	for(int n = 0; n < MAX_CC_HOLLOW_ZONE; n++)
	{
		for(int x = gChaosCastleHollowZone[stage][n][0]; x <= gChaosCastleHollowZone[stage][n][2]; x++)
		{
			for(int y = gChaosCastleHollowZone[stage][n][1]; y <= gChaosCastleHollowZone[stage][n][3]; y++)
			{
				gMap[MAP_CHAOS_CASTLE_FINAL].SetAttr(x,y,8);
			}
		}
	}
	
	#endif
}

void CChaosCastleFinal::DelHollowZone(int stage) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	for(int n = 0; n < MAX_CC_HOLLOW_ZONE; n++)
	{
		for(int x = gChaosCastleHollowZone[stage][n][0]; x <= gChaosCastleHollowZone[stage][n][2]; x++)
		{
			for(int y = gChaosCastleHollowZone[stage][n][1]; y <= gChaosCastleHollowZone[stage][n][3]; y++)
			{
				gMap[MAP_CHAOS_CASTLE_FINAL].DelAttr(x,y,8);
			}
		}
	}
	
	#endif
}

void CChaosCastleFinal::SetCenterHollowZone() // OK
{
	#if(GAMESERVER_UPDATE>=803)

	for(int n = 0; n < MAX_CC_CENTER_HOLLOW_ZONE; n++)
	{
		for(int x = gChaosCastleCenterHollowZone[n][0]; x <= gChaosCastleCenterHollowZone[n][2]; x++)
		{
			for(int y = gChaosCastleCenterHollowZone[n][1]; y <= gChaosCastleCenterHollowZone[n][3]; y++)
			{
				gMap[MAP_CHAOS_CASTLE_FINAL].SetAttr(x,y,8);
			}
		}
	}
	
	#endif
}

void CChaosCastleFinal::DelCenterHollowZone() // OK
{
	#if(GAMESERVER_UPDATE>=803)

	for(int n = 0; n < MAX_CC_CENTER_HOLLOW_ZONE; n++)
	{
		for(int x = gChaosCastleCenterHollowZone[n][0]; x <= gChaosCastleCenterHollowZone[n][2]; x++)
		{
			for(int y = gChaosCastleCenterHollowZone[n][1]; y <= gChaosCastleCenterHollowZone[n][3]; y++)
			{
				gMap[MAP_CHAOS_CASTLE_FINAL].SetAttr(x,y,8);
			}
		}
	}
	
	#endif
}

void CChaosCastleFinal::CheckUserPosition() // OK
{
	#if(GAMESERVER_UPDATE>=803)

	for(int n = 0; n < MAX_CCF_USER; n++)
	{
		if(OBJECT_RANGE(this->User[n].Index) == 0)
		{
			continue;
		}

		if(gMap[MAP_CHAOS_CASTLE_FINAL].CheckAttr(gObj[this->User[n].Index].X,gObj[this->User[n].Index].Y,8) != 0)
		{
			gObj[this->User[n].Index].Life = 0;
			gObj[this->User[n].Index].State = OBJECT_DYING;
			gObj[this->User[n].Index].KillerType = 1;
			gObj[this->User[n].Index].RegenTime = GetTickCount();
			gObj[this->User[n].Index].DieRegen = 1;
			gObj[this->User[n].Index].PathCount = 0;

			GCUserDieSend(&gObj[this->User[n].Index],this->User[n].Index,0,0);

			this->GCChaosCastleFinalDelayScoreSend(this->User[n].Index,2);

			this->GDChaosCastleFinalScoreSaveSend(this->User[n].Index);

			this->DelUser(this->User[n].Index);
		}
	}
	
	#endif
}

void CChaosCastleFinal::CheckMonsterPosition() // OK
{
	#if(GAMESERVER_UPDATE>=803)

	for(int n = 0; n < MAX_CCF_MONSTER; n++)
	{
		if(OBJECT_RANGE(this->MonsterIndex[n]) == 0)
		{
			continue;
		}

		if(gMap[MAP_CHAOS_CASTLE_FINAL].CheckAttr(gObj[this->MonsterIndex[n]].X,gObj[this->MonsterIndex[n]].Y,8) != 0)
		{
			int x = gObj[this->MonsterIndex[n]].X;

			int y = gObj[this->MonsterIndex[n]].Y;

			if(this->GetFreePosition(&x,&y) != 0)
			{
				gObjSetPosition(this->MonsterIndex[n],x,y);

				gObj[this->MonsterIndex[n]].ChaosCastleBlowTime = GetTickCount();
			}
		}
	}
	
	#endif
}

void CChaosCastleFinal::CheckItemPosition() // OK
{
	#if(GAMESERVER_UPDATE>=803)

	for(int n = 0; n < MAX_MAP_ITEM; n++)
	{
		if(gMap[MAP_CHAOS_CASTLE_FINAL].m_Item[n].IsItem() == 0)
		{
			continue;
		}

		if(gMap[MAP_CHAOS_CASTLE_FINAL].CheckAttr(gMap[MAP_CHAOS_CASTLE_FINAL].m_Item[n].m_X,gMap[MAP_CHAOS_CASTLE_FINAL].m_Item[n].m_Y,8) != 0)
		{
			gMap[MAP_CHAOS_CASTLE_FINAL].m_Item[n].m_Live = 0;
			gMap[MAP_CHAOS_CASTLE_FINAL].m_Item[n].m_Give = 1;
			gMap[MAP_CHAOS_CASTLE_FINAL].m_Item[n].m_State = OBJECT_DIECMD;
		}
	}
	
	#endif
}

void CChaosCastleFinal::SearchUserAndBlow(int x,int y) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	for(int n = 0; n < MAX_CCF_USER; n++)
	{
		if(OBJECT_RANGE(this->User[n].Index) == 0)
		{
			continue;
		}

		if(gObj[this->User[n].Index].DieRegen != 0 || gObj[this->User[n].Index].Teleport != 0)
		{
			continue;
		}

		if(gObj[this->User[n].Index].X >= (x - 3) && gObj[this->User[n].Index].X <= (x+3) && gObj[this->User[n].Index].Y >= (y - 3) && gObj[this->User[n].Index].Y <= (y+3))
		{
			this->BlowUserFromPoint(this->User[n].Index,x,y);
		}
	}
	
	#endif
}

void CChaosCastleFinal::BlowUserFromPoint(int aIndex,int x,int y) // OK
{
	#if(GAMESERVER_UPDATE>=803)

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
	
	#endif
}

bool CChaosCastleFinal::GetFreePosition(int* ox,int* oy) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	for(int x = ((*ox) - 8); x < ((*ox)+8); x++)
	{
		for(int y = ((*oy) - 8); y < ((*oy)+8); y++)
		{
			if(gMap[MAP_CHAOS_CASTLE_FINAL].CheckAttr(x,y,2) == 0 && gMap[MAP_CHAOS_CASTLE_FINAL].CheckAttr(x,y,4) == 0 && gMap[MAP_CHAOS_CASTLE_FINAL].CheckAttr(x,y,8) == 0)
			{
				(*ox) = x;
				(*oy) = y;
				return 1;
			}
		}
	}

	return 0;
	
	#else

	return 0;
	
	#endif
}

void CChaosCastleFinal::UserDieProc(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	if(this->GetState() != CCF_STATE_START)
	{
		return;
	}

	if(this->GetUser(lpObj->Index) == 0)
	{
		return;
	}

	this->GCChaosCastleFinalDelayScoreSend(lpObj->Index,2);

	this->GDChaosCastleFinalScoreSaveSend(lpObj->Index);

	this->DelUser(lpObj->Index);

	if(lpTarget->Type != OBJECT_USER)
	{
		return;
	}

	CHAOS_CASTLE_FINAL_USER* lpUser = this->GetUser(lpTarget->Index);

	if(lpUser == 0)
	{
		return;
	}

	lpUser->Score += 2;

	lpUser->UserKillCount++;

	#endif
}

void CChaosCastleFinal::MonsterDieProc(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	int aIndex = gObjMonsterGetTopHitDamageUser(lpObj);

	if(OBJECT_RANGE(aIndex) != 0)
	{
		lpTarget = &gObj[aIndex];
	}

	if(this->GetState() != CCF_STATE_START)
	{
		return;
	}

	if(this->GetMonster(lpObj->Index) == 0)
	{
		return;
	}

	if((GetLargeRand() % 100) < gServerInfo.m_ChaosCastleSurvivalBlowUserRate)
	{
		this->SearchUserAndBlow(lpObj->X,lpObj->Y);
	}

	this->DelMonster(lpObj->Index);

	if(lpTarget->Type != OBJECT_USER)
	{
		return;
	}

	CHAOS_CASTLE_FINAL_USER* lpUser = this->GetUser(lpTarget->Index);

	if(lpUser == 0)
	{
		return;
	}

	lpUser->Score += 1;

	lpUser->MonsterKillCount++;
	
	#endif
}

void CChaosCastleFinal::CheckDelayScore() // OK
{
	#if(GAMESERVER_UPDATE>=803)

	for(std::vector<CHAOS_CASTLE_FINAL_DELAY_SCORE>::iterator it = this->DelayScore.begin(); it != this->DelayScore.end();)
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

		it = this->DelayScore.erase(it);
	}
	
	#endif
}

void CChaosCastleFinal::SendMapServerGroupMsg(char* lpMsg,...) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	char buff[256];

	va_list arg;
	va_start(arg,lpMsg);
	vsprintf_s(buff,lpMsg,arg);
	va_end(arg);

	GDGlobalNoticeSend(gMapServerManager.GetMapServerGroup(),0,0,0,0,0,0,buff);
	
	#endif
}

void CChaosCastleFinal::NoticeSendToAll(int type,char* message,...) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	char buff[256];

	va_list arg;
	va_start(arg,message);
	vsprintf_s(buff,message,arg);
	va_end(arg);

	for(int n = 0; n < MAX_CCF_USER; n++)
	{
		if(OBJECT_RANGE(this->User[n].Index) != 0)
		{
			gNotice.GCNoticeSend(this->User[n].Index,type,0,0,0,0,0,buff);
		}
	}
	
	#endif
}

void CChaosCastleFinal::DataSendToAll(BYTE* lpMsg,int size) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	for(int n = 0; n < MAX_CCF_USER; n++)
	{
		if(OBJECT_RANGE(this->User[n].Index) != 0)
		{
			DataSend(this->User[n].Index,lpMsg,size);
		}
	}
	
	#endif
}

void CChaosCastleFinal::CGChaosCastleFinalRankingSend(int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	if(gServerInfo.m_ChaosCastleSurvivalEvent == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	lpObj->Interface.use = 1;
	lpObj->Interface.type = INTERFACE_CHAOS_CASTLE;
	lpObj->Interface.state = 0;

	BYTE send[4096];

	PMSG_CHAOS_CASTLE_FINAL_TIME_SEND pMsg;

	pMsg.header.set(0xAF,0x03,0);

	int size = sizeof(pMsg);

	pMsg.count = 0;

	pMsg.Tryout1 = this->m_Tryout1;
	
	pMsg.Tryout2 = this->m_Tryout2;
	
	pMsg.Tryout3 = this->m_Tryout3;
	
	pMsg.SemiFinal1 = this->m_SemiFinal1;
	
	pMsg.SemiFinal2 = this->m_SemiFinal2;
	
	pMsg.Final = this->m_Final;

	pMsg.Type = this->GetDay();

	PMSG_CHAOS_CASTLE_FINAL_TIME info;

	for(std::vector<CHAOS_CASTLE_FINAL_START_TIME>::iterator it = this->m_ChaosCastleFinalStartTime.begin(); it != this->m_ChaosCastleFinalStartTime.end(); it++)
	{
		info.Hour = it->Hour;
		info.Minute = it->Minute;
		info.Stage1 = it->Stage1;
		info.Stage2 = it->Stage2;
		info.Stage3 = it->Stage3;

		memcpy(&send[size],&info,sizeof(info));
		size += sizeof(info);

		pMsg.count++;
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);

	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send,&pMsg,sizeof(pMsg));

	DataSend(lpObj->Index,send,size);
	
	#endif
}

void CChaosCastleFinal::CGChaosCastleFinalClose(int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	lpObj->Interface.use = 0;
	lpObj->Interface.type = INTERFACE_NONE;
	lpObj->Interface.state = 0;
	
	#endif
}

void CChaosCastleFinal::CGChaosCastleFinalEnterRecv(int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	if(gServerInfo.m_ChaosCastleSurvivalEvent == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	PMSG_CHAOS_CASTLE_FINAL_ENTER_SEND pMsg;

	pMsg.header.set(0xAF,0x05,sizeof(pMsg));

	pMsg.result = 0;
	pMsg.RemainTime[0] = 0;
	pMsg.RemainTime[1] = 0;

	if(lpObj->Inventory[INVENTORY_SLOT_HELPER].m_Index == GET_ITEM(13,2) || lpObj->Inventory[INVENTORY_SLOT_HELPER].m_Index == GET_ITEM(13,3) || lpObj->Inventory[INVENTORY_SLOT_HELPER].m_Index == GET_ITEM(13,37))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(159));
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
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(159));
		return;
	}

	if(this->m_EnterEnabled == 0)
	{
		pMsg.result = 2;
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	if(gMapServerManager.CheckMapServer(MAP_CHAOS_CASTLE_FINAL) == 0)
	{
		pMsg.result = 2;
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	if(lpObj->Money < ((DWORD)gChaosCastleFinalEnterMoney))
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

	if(this->AddUser(aIndex) == 0)
	{
		pMsg.result = 5;
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	lpObj->Money -= gChaosCastleFinalEnterMoney;
	GCMoneySend(aIndex,lpObj->Money);

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);

	if(OBJECT_RANGE(lpObj->SummonIndex) != 0)
	{
		gObjSummonKill(aIndex);
		GCSummonLifeSend(aIndex,0,1);
	}

	lpObj->Interface.use = 0;
	lpObj->Interface.type = 0;
	lpObj->Interface.state = 0;

	gObjMoveGate(aIndex,443);
	
	#endif
}

void CChaosCastleFinal::CGChaosCastleFinalPositionRecv(PMSG_CHAOS_CASTLE_FINAL_POSITION_RECV* lpMsg,int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	CHAOS_CASTLE_FINAL_USER* lpUser = this->GetUser(aIndex);

	if(lpUser == 0)
	{
		return;
	}

	gObjSetPosition(lpUser->Index,lpMsg->x,lpMsg->y);

	lpObj->ChaosCastleBlowTime = GetTickCount();
	
	#endif
}

void CChaosCastleFinal::GCChaosCastleFinalRankingRecv(int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(lpObj->Interface.use == 0 || lpObj->Interface.type != INTERFACE_CHAOS_CASTLE)
	{
		return;
	}

	SDHP_CHAOS_CASTLE_RANKING_SEND pMsg;

	pMsg.header.set(0x1B,0x00,sizeof(pMsg));

	pMsg.index = aIndex;
	
	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);
	
	#endif
}

void CChaosCastleFinal::GCChaosCastleFinalScoreSend(int aIndex,int type) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	CHAOS_CASTLE_FINAL_USER* lpUser = this->GetUser(aIndex);

	if(lpUser == 0)
	{
		return;
	}

	PMSG_CHAOS_CASTLE_FINAL_SCORE_SEND pMsg;

	pMsg.header.set(0xAF,0x09,sizeof(pMsg));

	pMsg.day = this->GetDay();

	pMsg.type = type;

	pMsg.Score = lpUser->Score;

	pMsg.MonsterKillCount = lpUser->MonsterKillCount;

	pMsg.UserKillCount = lpUser->UserKillCount;

	DataSend(lpUser->Index,(BYTE*)&pMsg,pMsg.header.size);
	
	#endif
}

void CChaosCastleFinal::GCChaosCastleFinalDelayScoreSend(int aIndex,int type) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	CHAOS_CASTLE_FINAL_USER* lpUser = this->GetUser(aIndex);

	if(lpUser == 0)
	{
		return;
	}

	CHAOS_CASTLE_FINAL_DELAY_SCORE DelayScore;

	DelayScore.Delay = GetTickCount();

	DelayScore.Index = lpUser->Index;

	DelayScore.Score.header.set(0xAF,0x09,sizeof(DelayScore.Score));

	DelayScore.Score.day = this->GetDay();

	DelayScore.Score.type = type;

	DelayScore.Score.Score = lpUser->Score;

	DelayScore.Score.MonsterKillCount = lpUser->MonsterKillCount;

	DelayScore.Score.UserKillCount = lpUser->UserKillCount;

	this->DelayScore.push_back(DelayScore);
	
	#endif
}

void CChaosCastleFinal::GCChaosCastleFinalStateSend(int state) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	PMSG_CHAOS_CASTLE_FINAL_STATE_SEND pMsg;

	pMsg.header.set(0x9B,sizeof(pMsg));

	pMsg.state = state;

	pMsg.time = this->m_RemainTime;

	pMsg.MaxMonster = this->m_MaxMonster;

	pMsg.CurMonster = this->GetUserCount()+this->GetMonsterCount();

	pMsg.EventItemOwner = 0;

	pMsg.EventItemLevel = 0xFF;

	this->DataSendToAll((BYTE*)&pMsg,pMsg.header.size);
	
	#endif
}

void CChaosCastleFinal::GCChaosCastleFinalSimpleStateSend(int state,int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	PMSG_CHAOS_CASTLE_FINAL_STATE_SEND pMsg;

	pMsg.header.set(0x9B,sizeof(pMsg));

	pMsg.state = state;

	pMsg.time = 0;

	pMsg.MaxMonster = 0;

	pMsg.CurMonster = 0;

	pMsg.EventItemOwner = 0;

	pMsg.EventItemLevel = 0xFF;

	if(OBJECT_RANGE(aIndex) == 0)
	{
		this->DataSendToAll((BYTE*)&pMsg,pMsg.header.size);
	}
	else
	{
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
	}
	
	#endif
}

void CChaosCastleFinal::GCChaosCastleFinalRankingSend(SDHP_CHAOS_CASTLE_RANKING_RECV* lpMsg) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	LPOBJ lpObj = &gObj[lpMsg->index];

	if(gObjIsConnectedGP(lpMsg->index) == 0)
	{
		return;
	}

	if(lpObj->Interface.use == 0 || lpObj->Interface.type != INTERFACE_CHAOS_CASTLE)
	{
		return;
	}

	BYTE send[4096];

	PMSG_CHAOS_CASTLE_FINAL_RANKING_SEND pMsg;

	pMsg.header.set(0xAF,0x07,0);

	int size = sizeof(pMsg);

	pMsg.Result = 0;

	pMsg.count = 0;

	SDHP_CHAOS_CASTLE_RANKING info;

	for(int n=0;n < lpMsg->count;n++)
	{
		if(this->GetDay() == 0)
		{
			pMsg.Result = 1;
			break;
		}

		SDHP_CHAOS_CASTLE_RANKING* lpInfo = (SDHP_CHAOS_CASTLE_RANKING*)(((BYTE*)lpMsg)+sizeof(SDHP_CHAOS_CASTLE_RANKING_RECV)+(sizeof(SDHP_CHAOS_CASTLE_RANKING)*n));

		info.Rank = lpInfo->Rank;
		memcpy(info.name,lpInfo->name,sizeof(info.name));
		info.Point = lpInfo->Point;

		memcpy(&send[size],&info,sizeof(info));
		size += sizeof(info);

		pMsg.count++;
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);
	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send,&pMsg,sizeof(pMsg));

	DataSend(lpObj->Index,send,size);
	
	#endif
}

void CChaosCastleFinal::GDChaosCastleFinalScoreSaveSend(int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=803)

	CHAOS_CASTLE_FINAL_USER* lpUser = this->GetUser(aIndex);

	if(lpUser == 0)
	{
		return;
	}

	SDHP_CHAOS_CASTLE_RANKING_SAVE_SEND pMsg;

	pMsg.header.set(0x1B,0x01,sizeof(pMsg));

	memcpy(pMsg.name,gObj[aIndex].Name,sizeof(pMsg.name));

	pMsg.Point = lpUser->Score;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);
	
	#endif
}

void CChaosCastleFinal::GDChaosCastleFinalScoreClearSend() // OK
{
	#if(GAMESERVER_UPDATE>=803)

	SDHP_CHAOS_CASTLE_RANKING_CLEAR_SEND pMsg;

	pMsg.header.set(0x1B,0x02,sizeof(pMsg));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,-1);
	
	#endif
}

int CChaosCastleFinal::GetDay()
{
	#if(GAMESERVER_UPDATE>=803)

	tm aTime;

	time_t rawtime;

	time(&rawtime);

	localtime_s(&aTime,&rawtime);

	if(aTime.tm_mday == this->m_Tryout1 || aTime.tm_mday == this->m_Tryout2 || aTime.tm_mday == this->m_Tryout3)
	{
		return 1;
	}
	else if(aTime.tm_mday == this->m_SemiFinal1 || aTime.tm_mday == this->m_SemiFinal2)
	{
		return 2;
	}
	else if(aTime.tm_mday == this->m_Final)
	{
		return 3;
	}
	else
	{
		return 0;
	}
	
	#else

	return 0;
	
	#endif
}