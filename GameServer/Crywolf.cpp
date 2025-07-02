// Crywolf.cpp: implementation of the CCrywolf class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Crywolf.h"
#include "CrywolfAltar.h"
#include "CrywolfStatue.h"
#include "CrywolfSync.h"
#include "CrywolfUtil.h"
#include "DSProtocol.h"
#include "EffectManager.h"
#include "Gate.h"
#include "ItemBagManager.h"
#include "MapServerManager.h"
#include "MemScript.h"
#include "Message.h"
#include "Monster.h"
#include "MonsterAIGroup.h"
#include "MonsterSetBase.h"
#include "Notice.h"
#include "ObjectManager.h"
#include "ServerInfo.h"
#include "User.h"
#include "Util.h"

CCrywolf gCrywolf;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCrywolf::CCrywolf() // OK
{
	this->m_CrywolfState = CRYWOLF_STATE_NONE;
	this->m_OccupationState = 0;
	this->m_CrywolfNotifyMsgStartTick = 0;
	this->m_CrywolfStartProcTick = 0;
	this->m_TurnUpBoss = 0;
	this->m_ChangeAI = 0;
	this->m_MonsterGroupNumberCount = 0;
	this->m_BossIndex = -1;
}

CCrywolf::~CCrywolf() // OK
{

}

void CCrywolf::DelAllData() // OK
{
	this->m_DBDataLoadOK = 0;
	this->m_DBDataLoading = 0;
	this->m_ScheduleMode = 0;

	for(int n=0;n < MAX_CRYWOLF_STATE_TIME;n++)
	{
		this->m_StartTimeInfo[n].Reset();
	}

	for(int n=0;n < CRYWOLF_STATE_MAX;n++)
	{
		this->m_StateTimeInfo[n].Reset();
	}

	this->m_StartTimeInfoCount = 0;
	this->m_MonsterGroupNumberCount = 0;
	this->m_MonsterGroupChangeAITime = 0;
	this->m_BossGroupNumber = 0;
	this->m_BossTurnUpTime = 0;

	memset(this->m_MonsterGroupNumberArray,-1,sizeof(this->m_MonsterGroupNumberArray));

	memset(this->m_MVPScoreTable,0,sizeof(this->m_MVPScoreTable));

	memset(this->m_MVPRankScoreTable,0,sizeof(this->m_MVPRankScoreTable));

	memset(this->m_MVPRankExpTable,0,sizeof(this->m_MVPRankExpTable));

	this->SetState(CRYWOLF_STATE_NONE);
}

void CCrywolf::Load(char* path) // OK
{
	#if(GAMESERVER_UPDATE>=201)

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

	this->DelAllData();

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

					this->m_ScheduleMode = lpMemScript->GetNumber();
					this->m_StartTimeInfo[this->m_StartTimeInfoCount].m_StateNumber = lpMemScript->GetAsNumber();
					this->m_StartTimeInfo[this->m_StartTimeInfoCount].m_Month = lpMemScript->GetAsNumber();
					this->m_StartTimeInfo[this->m_StartTimeInfoCount].m_Day = lpMemScript->GetAsNumber();
					this->m_StartTimeInfo[this->m_StartTimeInfoCount].m_DayOfWeek = lpMemScript->GetAsNumber();
					this->m_StartTimeInfo[this->m_StartTimeInfoCount].m_Hour = lpMemScript->GetAsNumber();
					this->m_StartTimeInfo[this->m_StartTimeInfoCount].m_Minute = lpMemScript->GetAsNumber();
					this->m_StartTimeInfo[this->m_StartTimeInfoCount].m_ContinuanceTime = lpMemScript->GetAsNumber();
					this->m_StartTimeInfo[this->m_StartTimeInfoCount].m_Used = 1;
					this->m_StartTimeInfoCount++;
				}
				else if(section == 1)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					int state = lpMemScript->GetNumber();

					int hour = lpMemScript->GetAsNumber();

					int minute = lpMemScript->GetAsNumber();

					int second = lpMemScript->GetAsNumber();

					this->m_StateTimeInfo[state].m_StateNumber = state;
					this->m_StateTimeInfo[state].m_Hour = hour;
					this->m_StateTimeInfo[state].m_Minute = minute;
					this->m_StateTimeInfo[state].m_Second = second;
					this->m_StateTimeInfo[state].m_ContinuanceTime = (((hour*60)*60)*1000)+((minute*60)*1000)+(second*1000);
					this->m_StateTimeInfo[state].m_Used = 1;
				}
				else if(section == 2)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					this->m_MonsterGroupNumberArray[this->m_MonsterGroupNumberCount++] = lpMemScript->GetNumber();
				}
				else if(section == 3)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					this->m_BossGroupNumber = lpMemScript->GetNumber();

					this->m_BossTurnUpTime = lpMemScript->GetAsNumber();
				}
				else if(section == 4)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					this->m_MonsterGroupChangeAITime = lpMemScript->GetNumber();
				}
				else if(section == 5)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					this->m_MVPScoreTable[0] = lpMemScript->GetNumber();

					this->m_MVPScoreTable[1] = lpMemScript->GetAsNumber();

					this->m_MVPScoreTable[2] = lpMemScript->GetAsNumber();

					this->m_MVPScoreTable[3] = lpMemScript->GetAsNumber();

					this->m_MVPScoreTable[4] = lpMemScript->GetAsNumber();

					this->m_MVPScoreTable[5] = lpMemScript->GetAsNumber();

					this->m_MVPScoreTable[6] = lpMemScript->GetAsNumber();
				}
				else if(section == 6)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					this->m_MVPRankScoreTable[0] = lpMemScript->GetNumber();

					this->m_MVPRankScoreTable[1] = lpMemScript->GetAsNumber();

					this->m_MVPRankScoreTable[2] = lpMemScript->GetAsNumber();

					this->m_MVPRankScoreTable[3] = lpMemScript->GetAsNumber();

					this->m_MVPRankScoreTable[4] = lpMemScript->GetAsNumber();
				}
				else if(section == 7)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					this->m_MVPRankExpTable[0] = lpMemScript->GetNumber();

					this->m_MVPRankExpTable[1] = lpMemScript->GetAsNumber();

					this->m_MVPRankExpTable[2] = lpMemScript->GetAsNumber();

					this->m_MVPRankExpTable[3] = lpMemScript->GetAsNumber();

					this->m_MVPRankExpTable[4] = lpMemScript->GetAsNumber();
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

void CCrywolf::MainProc() // OK
{
	#if(GAMESERVER_UPDATE>=201)

	if(gCrywolfSync.CheckEnableCrywolf() == 0)
	{
		return;
	}

	if(this->m_StartTimeInfo[0].m_Used == 0)
	{
		return;
	}

	this->CheckStateTimeSync();

	switch(this->m_CrywolfState)
	{
		case CRYWOLF_STATE_NONE:
			this->ProcState_NONE();
			break;
		case CRYWOLF_STATE_NOTIFY_1:
			this->ProcState_NOTIFY1();
			break;
		case CRYWOLF_STATE_NOTIFY_2:
			this->ProcState_NOTIFY2();
			break;
		case CRYWOLF_STATE_READY:
			this->ProcState_READY();
			break;
		case CRYWOLF_STATE_START:
			this->ProcState_START();
			break;
		case CRYWOLF_STATE_END:
			this->ProcState_END();
			break;
		case CRYWOLF_STATE_END_CYCLE:
			this->ProcState_ENDCYCLE();
			break;
	}

	#endif
}

void CCrywolf::ProcState_NONE() // OK
{

}

void CCrywolf::ProcState_NOTIFY1() // OK
{
	if((GetTickCount()-this->m_CrywolfNotifyMsgStartTick) > 70000)
	{
		gCrywolfUtil.SendMapServerGroupMsg(gMessage.GetMessage(70));
		this->m_CrywolfNotifyMsgStartTick = GetTickCount();
	}
}

void CCrywolf::ProcState_NOTIFY2() // OK
{
	if((GetTickCount()-this->m_CrywolfNotifyMsgStartTick) > 70000)
	{
		if(((this->m_StateTimeInfo[this->GetCrywolfState()].GetLeftTime()/1000)/60) == 0)
		{
			gCrywolfUtil.SendMapServerGroupMsg(gMessage.GetMessage(78),(this->m_StateTimeInfo[this->GetCrywolfState()].GetLeftTime()/1000));
		}
		else
		{
			gCrywolfUtil.SendMapServerGroupMsg(gMessage.GetMessage(79),((this->m_StateTimeInfo[this->GetCrywolfState()].GetLeftTime()/1000)/60));
		}

		this->m_CrywolfNotifyMsgStartTick = GetTickCount();
	}
}

void CCrywolf::ProcState_READY() // OK
{
	this->NotifyCrywolfStatueAndAltarInfo();

	if((GetTickCount()-this->m_CrywolfNotifyMsgStartTick) > 70000)
	{
		if(((this->m_StateTimeInfo[this->GetCrywolfState()].GetLeftTime()/1000)/60) == 0)
		{
			gCrywolfUtil.SendMapServerGroupMsg(gMessage.GetMessage(80),(this->m_StateTimeInfo[this->GetCrywolfState()].GetLeftTime()/1000));
		}
		else
		{
			gCrywolfUtil.SendMapServerGroupMsg(gMessage.GetMessage(81),((this->m_StateTimeInfo[this->GetCrywolfState()].GetLeftTime()/1000)/60));
		}

		this->m_CrywolfNotifyMsgStartTick = GetTickCount();
	}
}

void CCrywolf::ProcState_START()
{
	if(gCrywolfAltar.GetContractedAltarCount() == 0)
	{
		gCrywolfUtil.SendCrywolfUserAnyMsg(0,gMessage.GetMessage(82));
		this->SetOccupationState(1);
		this->SetState(CRYWOLF_STATE_END);
		return;
	}

	this->NotifyCrywolfStatueAndAltarInfo();

	this->NotifyCrywolfBossMonsterInfo();

	this->NotifyCrywolfStateLeftTime();

	if((GetTickCount()-this->m_CrywolfStartProcTick) > ((DWORD)(this->m_BossTurnUpTime*1000)) && this->m_TurnUpBoss == 0)
	{
		this->m_TurnUpBoss = 1;
		gCrywolfUtil.SendCrywolfUserAnyMsg(0,gMessage.GetMessage(83));
		this->TurnUpBoss();
	}

	if((GetTickCount()-this->m_CrywolfStartProcTick) > ((DWORD)(this->m_MonsterGroupChangeAITime*1000)) && this->m_ChangeAI == 0)
	{
		this->m_ChangeAI = 1;
		gCrywolfUtil.SendCrywolfUserAnyMsg(0,gMessage.GetMessage(84));
		this->ChangeAI(2);
	}

	if(this->m_TurnUpBoss != 0 && OBJECT_RANGE(this->m_BossIndex) != 0 && gObj[this->m_BossIndex].Live == 0)
	{
		gCrywolfUtil.SendCrywolfUserAnyMsg(0,gMessage.GetMessage(85));
		this->SetOccupationState(0);
		this->SetState(CRYWOLF_STATE_END);
	}
}

void CCrywolf::ProcState_END() // OK
{

}

void CCrywolf::ProcState_ENDCYCLE() // OK
{

}

void CCrywolf::SetState(int state) // OK
{
	#if(GAMESERVER_UPDATE>=201)

	switch(state)
	{
		case CRYWOLF_STATE_NONE:
			this->SetState_NONE();
			break;
		case CRYWOLF_STATE_NOTIFY_1:
			this->SetState_NOTIFY1();
			break;
		case CRYWOLF_STATE_NOTIFY_2:
			this->SetState_NOTIFY2();
			break;
		case CRYWOLF_STATE_READY:
			this->SetState_READY();
			break;
		case CRYWOLF_STATE_START:
			this->SetState_START();
			break;
		case CRYWOLF_STATE_END:
			this->SetState_END();
			break;
		case CRYWOLF_STATE_END_CYCLE:
			this->SetState_ENDCYCLE();
			break;
	}

	this->NotifyCrywolfCurrentState();

	this->CrywolfInfoDBSave();

	#endif
}

void CCrywolf::SetState_NONE() // OK
{
	LogAdd(LOG_BLACK,"[Crywolf] SetState NONE");

	this->SetCrywolfState(CRYWOLF_STATE_NONE);

	this->SetCrywolfStateAppliedTime(CRYWOLF_STATE_NONE);
}

void CCrywolf::SetState_NOTIFY1() // OK
{
	gCrywolfUtil.SendMapServerGroupMsg(gMessage.GetMessage(70));

	LogAdd(LOG_BLACK,"[Crywolf] SetState NOTIFY1");

	this->SetCrywolfState(CRYWOLF_STATE_NOTIFY_1);

	this->SetCrywolfStateAppliedTime(CRYWOLF_STATE_NOTIFY_1);

	this->m_CrywolfNotifyMsgStartTick = GetTickCount();
}

void CCrywolf::SetState_NOTIFY2() // OK
{
	gCrywolfUtil.SendMapServerGroupMsg(gMessage.GetMessage(71));

	LogAdd(LOG_BLACK,"[Crywolf] SetState NOTIFY2");

	this->SetCrywolfState(CRYWOLF_STATE_NOTIFY_2);

	this->SetCrywolfStateAppliedTime(CRYWOLF_STATE_NOTIFY_2);

	this->SetOccupationState(2);

	this->SetCrywolfCommonNPC(2);

	this->SetCrywolfMapAttr(2);

	this->RemoveCrywolfCommonMonster();

	this->m_CrywolfNotifyMsgStartTick = GetTickCount();
}

void CCrywolf::SetState_READY() // OK
{
	gCrywolfUtil.SendMapServerGroupMsg(gMessage.GetMessage(72));

	LogAdd(LOG_BLACK,"[Crywolf] SetState READY");

	this->SetCrywolfState(CRYWOLF_STATE_READY);

	this->SetCrywolfStateAppliedTime(CRYWOLF_STATE_READY);

	this->SetOccupationState(2);

	this->SetCrywolfCommonNPC(2);

	this->SetCrywolfMapAttr(2);

	this->RemoveCrywolfCommonMonster();

	gCrywolfAltar.SetAllAltarObjectIndex();

	gCrywolfAltar.SetAllAltarViewState(0x10000);

	for(int n=0;n < this->m_MonsterGroupNumberCount;n++)
	{
		CMonsterAIGroup::Init(this->m_MonsterGroupNumberArray[n]);
	}

	this->m_CrywolfNotifyMsgStartTick = GetTickCount();

	this->ResetAllUserMVPScore();
}

void CCrywolf::SetState_START() // OK
{
	gCrywolfUtil.SendMapServerGroupMsg(gMessage.GetMessage(73));

	LogAdd(LOG_BLACK,"[Crywolf] SetState START");

	this->SetCrywolfState(CRYWOLF_STATE_START);

	this->SetCrywolfStateAppliedTime(CRYWOLF_STATE_START);

	if(gCrywolfAltar.GetContractedAltarCount() == 0)
	{
		this->SetOccupationState(1);
		this->SetState(CRYWOLF_STATE_END);
		gCrywolfUtil.SendCrywolfUserAnyMsg(0,gMessage.GetMessage(74));
		return;
	}

	this->ChangeAI(1);

	this->m_CrywolfStartProcTick = GetTickCount();

	this->m_TurnUpBoss = 0;

	this->m_ChangeAI = 0;
}

void CCrywolf::SetState_END()
{
	LogAdd(LOG_BLACK,"[Crywolf] SetState END");

	if(this->m_TurnUpBoss != 0 && OBJECT_RANGE(this->m_BossIndex) != 0 && gObj[this->m_BossIndex].Live != 0)
	{
		gCrywolfUtil.SendCrywolfUserAnyMsg(0,gMessage.GetMessage(75));
		this->SetOccupationState(1);
	}

	this->m_CrywolfStartProcTick = 0;

	this->m_TurnUpBoss = 0;

	this->m_ChangeAI = 0;

	this->SetCrywolfBossIndex(-1);

	if(this->GetOccupationState() == 1)
	{
		gCrywolfUtil.SendMapServerGroupMsg(gMessage.GetMessage(76));
		this->SetOccupationState(1);
	}
	else
	{
		gCrywolfUtil.SendMapServerGroupMsg(gMessage.GetMessage(77));
		this->SetOccupationState(0);
	}

	this->SetCrywolfState(CRYWOLF_STATE_END);

	this->SetCrywolfStateAppliedTime(CRYWOLF_STATE_END);

	this->NotifyCrywolfCurrentState();

	this->ChangeAI(0);

	for(int n=0;n < MAX_CRYWOLF_ALTAR;n++)
	{
		if(OBJECT_RANGE(gCrywolfAltar.GetAltarUserIndex((205+n))) != 0)
		{
			this->GetUserScore(gCrywolfAltar.GetAltarUserIndex((205+n)),-1,6);
			this->GiveUserRewardItem(gCrywolfAltar.GetAltarUserIndex((205+n)));
		}
	}

	gCrywolfAltar.ResetAllAltar();

	this->NotifyCrywolfPersonalRank();
}

void CCrywolf::SetState_ENDCYCLE() // OK
{
	LogAdd(LOG_BLACK,"[Crywolf] SetState ENDCYCLE");

	this->SetCrywolfState(CRYWOLF_STATE_END_CYCLE);

	this->SetCrywolfStateAppliedTime(CRYWOLF_STATE_END_CYCLE);

	this->SetCrywolfMapAttr(this->GetOccupationState());

	this->SetCrywolfCommonNPC(this->GetOccupationState());

	for(int n=0;n < this->m_MonsterGroupNumberCount;n++)
	{
		CMonsterAIGroup::DelGroupInstance(this->m_MonsterGroupNumberArray[n]);
	}

	CMonsterAIGroup::DelGroupInstance(this->m_BossGroupNumber);

	this->CreateCrywolfCommonMonster();

	this->NotifyCrywolfHeroList();

	this->ResetAllUserMVPScore();
}

void CCrywolf::CheckStateTimeSync() // OK
{
	if(this->m_CrywolfState == CRYWOLF_STATE_NONE)
	{
		for(int n=0;n < this->m_StartTimeInfoCount;n++)
		{
			if(this->m_StartTimeInfo[n].CheckScheduleTime() != 0)
			{
				this->SetState(this->m_StartTimeInfo[n].m_StateNumber);
			}
		}
	}
	else
	{
		if(this->m_StateTimeInfo[this->m_CrywolfState].m_Used == 0)
		{
			return;
		}

		if(this->m_StateTimeInfo[this->m_CrywolfState].CheckContinuanceTime() == 0)
		{
			this->m_StateTimeInfo[this->m_CrywolfState].ResetAppliedTime();

			if(this->m_CrywolfState == CRYWOLF_STATE_END_CYCLE)
			{
				if(this->m_ScheduleMode == 1)
				{
					this->SetState(CRYWOLF_STATE_NOTIFY_1);
				}
				else
				{
					this->SetState(CRYWOLF_STATE_NONE);
				}
			}
			else
			{
				this->SetState((this->m_CrywolfState+1));
			}
		}
	}
}

void CCrywolf::CrywolfSecondAct() // OK
{
	this->CrywolfServerGroupSync();

	if(this->m_DBDataLoadOK == 0 && this->m_DBDataLoading == 0)
	{
		this->CrywolfInfoDBLoad();
	}
}

void CCrywolf::CrywolfServerGroupSync() // OK
{
	if(this->m_DBDataLoadOK != 0)
	{
		GDCrywolfSyncSend(gMapServerManager.GetMapServerGroup(),this->GetCrywolfState(),this->GetOccupationState());
	}
}

void CCrywolf::CrywolfInfoDBSave() // OK
{
	if(this->m_DBDataLoadOK != 0)
	{
		GDCrywolfInfoSaveSend(gMapServerManager.GetMapServerGroup(),this->GetCrywolfState(),this->GetOccupationState());
	}
}

void CCrywolf::CrywolfInfoDBLoad() // OK
{
	this->m_DBDataLoading = 1;

	GDCrywolfInfoSend(gMapServerManager.GetMapServerGroup());
}

void CCrywolf::ApplyCrywolfDBInfo(int state) // OK
{
	this->SetDBDataLoad(1);

	this->SetOccupationState(state);

	this->SetCrywolfCommonNPC(state);

	this->SetCrywolfMapAttr(state);
}

void CCrywolf::ResetAllUserMVPScore() // OK
{
	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnected(n) != 0 && gObj[n].Type == OBJECT_USER && gObj[n].Map == MAP_CRYWOLF)
		{
			gObj[n].CrywolfMVPScore = 0;
		}
	}
}

void CCrywolf::TurnUpBoss() // OK
{
	CMonsterAIGroup::Init(this->m_BossGroupNumber);
	CMonsterAIGroup::ChangeAIOrder(this->m_BossGroupNumber,1);
	this->m_BossIndex = CMonsterAIGroup::FindGroupLeader(this->m_BossGroupNumber);
}

void CCrywolf::ChangeAI(int AIOrder) // OK
{
	for(int n=0;n < this->m_MonsterGroupNumberCount;n++)
	{
		CMonsterAIGroup::ChangeAIOrder(this->m_MonsterGroupNumberArray[n],AIOrder);
	}
}

void CCrywolf::CrywolfNpcAct(int aIndex) // OK
{
	if(this->GetCrywolfState() == CRYWOLF_STATE_READY || this->GetCrywolfState() == CRYWOLF_STATE_START)
	{
		gCrywolfStatue.CrywolfStatueAct(aIndex);

		gCrywolfAltar.CrywolfAltarAct(aIndex);
	}
}

void CCrywolf::CreateCrywolfCommonMonster() // OK
{
	for(int n=0;n < gMonsterSetBase.m_count;n++)
	{
		MONSTER_SET_BASE_INFO* lpInfo = &gMonsterSetBase.m_MonsterSetBaseInfo[n];

		if(lpInfo->Type == 3 || lpInfo->Type == 4)
		{
			continue;
		}

		if(lpInfo->Map != MAP_CRYWOLF)
		{
			continue;
		}

		if(lpInfo->MonsterClass >= 204 && lpInfo->MonsterClass <= 257)
		{
			continue;
		}

		int index = gObjAddMonster(lpInfo->Map);

		if(OBJECT_RANGE(index) == 0)
		{
			continue;
		}

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

		if(gObj[index].Type != OBJECT_MONSTER)
		{
			gObjDel(index);
			continue;
		}

		if(this->m_ObjCommonMonster.AddObj(index) == 0)
		{
			gObjDel(index);
			continue;
		}
	}
}

void CCrywolf::RemoveCrywolfCommonMonster() // OK
{
	for(int n=0;n < this->m_ObjCommonMonster.m_ObjCount;n++)
	{
		gObjDel(this->m_ObjCommonMonster.m_ObjIndex[n]);
	}

	this->m_ObjCommonMonster.Reset();
}

void CCrywolf::NotifyCrywolfCurrentState() // OK
{
	PMSG_CRYWOLF_INFO_SEND pMsg;

	pMsg.header.set(0xBD,0x00,sizeof(pMsg));

	pMsg.OccupationState = this->GetOccupationState();

	pMsg.CrywolfState = this->GetCrywolfState();

	gCrywolfUtil.SendCrywolfUserAnyData((BYTE*)&pMsg,sizeof(pMsg));
}

void CCrywolf::NotifyCrywolfStateLeftTime() // OK
{
	static DWORD CrywolfLeftTimeTickCount = 0;

	if((GetTickCount()-CrywolfLeftTimeTickCount) < 20000)
	{
		return;
	}

	CrywolfLeftTimeTickCount = GetTickCount();

	PMSG_CRYWOLF_LEFT_TIME_SEND pMsg;

	pMsg.header.set(0xBD,0x04,sizeof(pMsg));

	pMsg.hour = ((this->m_StateTimeInfo[this->GetCrywolfState()].GetLeftTime()/1000)/60)/24;

	pMsg.minute = ((this->m_StateTimeInfo[this->GetCrywolfState()].GetLeftTime()/1000)/60);

	gCrywolfUtil.SendCrywolfUserAnyData((BYTE*)&pMsg,sizeof(pMsg));
}

void CCrywolf::NotifyCrywolfStatueAndAltarInfo() // OK
{
	static DWORD CrywolfStatueAndAltarInfoTickCount = 0;

	if((GetTickCount()-CrywolfStatueAndAltarInfoTickCount) < 2000)
	{
		return;
	}

	CrywolfStatueAndAltarInfoTickCount = GetTickCount();

	PMSG_CRYWOLF_STATUE_ALTAR_INFO_SEND pMsg;

	pMsg.header.set(0xBD,0x02,sizeof(pMsg));

	pMsg.CrywolfStatueHP = gCrywolfStatue.m_Shield.GetHPPercentage();

	pMsg.AltarState1 = gCrywolfAltar.GetAltarRemainContractCount(205) | (gCrywolfAltar.GetAltarState(205) << 4);

	pMsg.AltarState2 = gCrywolfAltar.GetAltarRemainContractCount(206) | (gCrywolfAltar.GetAltarState(206) << 4);

	pMsg.AltarState3 = gCrywolfAltar.GetAltarRemainContractCount(207) | (gCrywolfAltar.GetAltarState(207) << 4);

	pMsg.AltarState4 = gCrywolfAltar.GetAltarRemainContractCount(208) | (gCrywolfAltar.GetAltarState(208) << 4);

	pMsg.AltarState5 = gCrywolfAltar.GetAltarRemainContractCount(209) | (gCrywolfAltar.GetAltarState(209) << 4);

	gCrywolfUtil.SendCrywolfUserAnyData((BYTE*)&pMsg,sizeof(pMsg));
}

void CCrywolf::NotifyCrywolfBossMonsterInfo() // OK
{
	static DWORD CrywolfBossMonsterInfoTickCount = 0;

	if((GetTickCount()-CrywolfBossMonsterInfoTickCount) < 5000)
	{
		return;
	}

	CrywolfBossMonsterInfoTickCount = GetTickCount();

	PMSG_CRYWOLF_BOSS_MONSTER_INFO_SEND pMsg;

	pMsg.header.set(0xBD,0x05,sizeof(pMsg));

	pMsg.BalgassHP = 0xFFFFFFFF;

	pMsg.DarkElfCount = 0;

	for(int n=OBJECT_START_MONSTER;n < MAX_OBJECT_MONSTER;n++)
	{
		if(gObjIsConnected(n) == 0 || gObj[n].Map != MAP_CRYWOLF || gObj[n].Live == 0)
		{
			continue;
		}

		if(gObj[n].Class == 340)
		{
			pMsg.DarkElfCount++;
		}
		else if(gObj[n].Class == 349)
		{
			pMsg.BalgassHP = (DWORD)((gObj[n].MaxLife==0)?0:((gObj[n].Life*100)/gObj[n].MaxLife));
		}
	}

	gCrywolfUtil.SendCrywolfUserAnyData((BYTE*)&pMsg,sizeof(pMsg));
}

void CCrywolf::NotifyCrywolfStageEffectOnOff(int state) // OK
{
	PMSG_CRYWOLF_STAGE_EFFECT_SEND pMsg;

	pMsg.header.set(0xBD,0x06,sizeof(pMsg));

	pMsg.state = state;

	gCrywolfUtil.SendCrywolfUserAnyData((BYTE*)&pMsg,sizeof(pMsg));
}

void CCrywolf::NotifyCrywolfPersonalRank() // OK
{
	PMSG_CRYWOLF_RANK_SEND pMsg;

	pMsg.header.set(0xBD,0x07,sizeof(pMsg));

	pMsg.rank = 0;

	pMsg.experience = 0;

	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnected(n) != 0 && gObj[n].Type == OBJECT_USER && gObj[n].Map == MAP_CRYWOLF)
		{
			pMsg.rank = this->GetUserRank(n);

			pMsg.experience = this->GetUserExperience(n,pMsg.rank);

			DataSend(n,(BYTE*)&pMsg,pMsg.header.size);

			this->GiveUserRewardExperience(n,pMsg.experience);

			gNotice.GCNoticeSend(n,1,0,0,0,0,0,gMessage.GetMessage(86),pMsg.rank);

			gNotice.GCNoticeSend(n,1,0,0,0,0,0,gMessage.GetMessage(87),pMsg.experience);
		}
	}
}

void CCrywolf::NotifyCrywolfHeroList() // OK
{
	std::vector<LPOBJ> CrywolfHeroList;

	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnected(n) != 0 && gObj[n].Type == OBJECT_USER && gObj[n].Map == MAP_CRYWOLF)
		{
			CrywolfHeroList.push_back(&gObj[n]);
		}
	}

	std::sort(CrywolfHeroList.begin(),CrywolfHeroList.end(),CCrywolfUtil::CrywolfAllUserScoreSort);

	BYTE send[256];

	PMSG_CRYWOLF_RANK_LIST_SEND pMsg;

	pMsg.header.set(0xBD,0x08,0);

	int size = sizeof(pMsg);

	pMsg.count = 0;

	PMSG_CRYWOLF_RANK_LIST info;

	for(std::vector<LPOBJ>::iterator it=CrywolfHeroList.begin();it != CrywolfHeroList.end() && pMsg.count < MAX_CRYWOLF_RANK;it++)
	{
		info.rank = pMsg.count;

		memcpy(info.name,(*it)->Name,sizeof(info.name));

		info.score = (*it)->CrywolfMVPScore;

		info.Class = (BYTE)(*it)->Class;

		this->GiveUserRewardItem((*it)->Index);

		memcpy(&send[size],&info,sizeof(info));
		size += sizeof(info);

		pMsg.count++;
	}

	pMsg.header.size = size;

	memcpy(send,&pMsg,sizeof(pMsg));

	gCrywolfUtil.SendCrywolfUserAnyData(send,size);
}

void CCrywolf::CGCrywolfInfoRecv(int aIndex) // OK
{
	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	PMSG_CRYWOLF_INFO_SEND pMsg;

	pMsg.header.set(0xBD,0x00,sizeof(pMsg));

	pMsg.OccupationState = gCrywolfSync.GetOccupationState();

	pMsg.CrywolfState = gCrywolfSync.GetCrywolfState();

	DataSend(aIndex,(BYTE*)&pMsg,sizeof(pMsg));
}

void CCrywolf::CGCrywolfAltarContractRecv(PMSG_CRYWOLF_ALTAR_CONTRACT_RECV* lpMsg,int aIndex) // OK
{
	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	int bIndex = MAKE_NUMBERW(lpMsg->index[0],lpMsg->index[1]);

	if(OBJECT_RANGE(bIndex) == 0 || (gObj[bIndex].Class < 205 || gObj[bIndex].Class > 209))
	{
		return;
	}

	PMSG_CRYWOLF_ALTAR_CONTRACT_SEND pMsg;

	pMsg.header.set(0xBD,0x03,sizeof(pMsg));

	pMsg.result = 0;

	pMsg.state = gCrywolfAltar.GetAltarState(gObj[bIndex].Class);

	pMsg.index[0] = SET_NUMBERHB(bIndex);

	pMsg.index[1] = SET_NUMBERLB(bIndex);

	if(pMsg.state == 0 && gObj[aIndex].Class == CLASS_FE && gObj[aIndex].Level >= 260)
	{
		if(gCrywolfAltar.SetAltarUserIndex(bIndex,gObj[bIndex].Class,aIndex) != 0)
		{
			pMsg.result = 1;
		}
	}
	else
	{
		gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(105),260);
	}

	DataSend(aIndex,(BYTE*)&pMsg,sizeof(pMsg));
}

void CCrywolf::CGCrywolfChaosRateRecv(int aIndex) // OK
{
	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	PMSG_CRYWOLF_CHAOS_RATE_SEND pMsg;

	pMsg.header.set(0xBD,0x09,sizeof(pMsg));

	pMsg.rate = gCrywolfSync.GetPlusChaosRate();

	DataSend(aIndex,(BYTE*)&pMsg,sizeof(pMsg));
}

void CCrywolf::GiveUserRewardExperience(int aIndex,int AddExperience) // OK
{
	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(gEffectManager.CheckEffect(&gObj[aIndex],EFFECT_SEAL_OF_SUSTENANCE1) != 0 || gEffectManager.CheckEffect(&gObj[aIndex],EFFECT_SEAL_OF_SUSTENANCE2) != 0)
	{
		return;
	}

	if(gObjectManager.CharacterLevelUp(&gObj[aIndex],AddExperience,gServerInfo.m_MaxLevelUpEvent,EXPERIENCE_CRYWOLF) != 0)
	{
		return;
	}

	GCRewardExperienceSend(aIndex,AddExperience);
}

void CCrywolf::GiveUserRewardItem(int aIndex) // OK
{
	if(this->GetOccupationState() == 1)
	{
		return;
	}

	gItemBagManager.DropItemBySpecialValue(ITEM_BAG_SPECIAL_CRYWOLF,-1,-1,&gObj[aIndex],gObj[aIndex].Map,gObj[aIndex].X,gObj[aIndex].Y);
}

void CCrywolf::CrywolfMonsterDieProc(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	int aIndex = gObjMonsterGetTopHitDamageUser(lpObj);

	if(OBJECT_RANGE(aIndex) != 0)
	{
		lpTarget = &gObj[aIndex];
	}

	if(this->GetCrywolfState() == CRYWOLF_STATE_START && lpObj->Map == MAP_CRYWOLF && lpObj->CurrentAI != 0)
	{
		if(lpObj->Class == 340 && gObjIsConnected(lpTarget->Index) != 0)
		{
			gCrywolfUtil.SendCrywolfUserAnyMsg(0,gMessage.GetMessage(90),lpObj->GroupNumber,lpTarget->Name);
		}

		if(lpObj->Class == 349 && gObjIsConnected(lpTarget->Index) == 0)
		{
			gCrywolfUtil.SendCrywolfUserAnyMsg(0,gMessage.GetMessage(89));
		}

		if(lpObj->Class == 349 && gObjIsConnected(lpTarget->Index) != 0)
		{
			gCrywolfUtil.SendCrywolfUserAnyMsg(0,gMessage.GetMessage(88),lpTarget->Name);
		}

		if(gObjIsConnected(lpTarget->Index) != 0)
		{
			gNotice.GCNoticeSend(lpTarget->Index,1,0,0,0,0,0,gMessage.GetMessage(91),this->GetUserScore(lpTarget->Index,lpObj->Index,0));
		}
	}
}

void CCrywolf::LoadCrywolfMapAttr(char* path,int state) // OK
{
	if(state < 0 || state >= MAX_CRYWOLF_OCCUPATION_STATE)
	{
		return;
	}

	this->m_CrywolfMapAttr[state].Load(path,MAP_CRYWOLF);
}

void CCrywolf::SetDBDataLoad(int loaded) // OK
{
	this->m_DBDataLoadOK = loaded;
}

void CCrywolf::SetCrywolfMapAttr(int state) // OK
{
	if(state < 0 || state >= MAX_CRYWOLF_OCCUPATION_STATE)
	{
		return;
	}

	if(this->m_CrywolfMapAttr[state].m_MapNumber != MAP_CRYWOLF)
	{
		return;
	}

	memcpy(gMap[MAP_CRYWOLF].m_MapAttr,this->m_CrywolfMapAttr[state].m_MapAttr,(this->m_CrywolfMapAttr[state].m_width*this->m_CrywolfMapAttr[state].m_height));
}

void CCrywolf::SetCrywolfState(int state) // OK
{
	this->m_CrywolfState = state;
}

void CCrywolf::SetOccupationState(int state) // OK
{
	this->m_OccupationState = state;
}

void CCrywolf::SetCrywolfCommonNPC(int state) // OK
{
	for(int n=0;n < this->m_ObjCommonNPC.m_ObjCount;n++)
	{
		if(gObjIsConnected(this->m_ObjCommonNPC.m_ObjIndex[n]) == 0)
		{
			continue;
		}

		if(state == 1 || state == 2)
		{
			gEffectManager.AddEffect(&gObj[this->m_ObjCommonNPC.m_ObjIndex[n]],0,EFFECT_CRYWOLF_STATE7,0,0,0,0,0);
		}
		else if(state == 0)
		{
			gEffectManager.DelEffect(&gObj[this->m_ObjCommonNPC.m_ObjIndex[n]],EFFECT_CRYWOLF_STATE7);
		}
	}
}

void CCrywolf::SetCrywolfStateAppliedTime(int state) // OK
{
	this->m_StateTimeInfo[state].SetAppliedTime();
}

void CCrywolf::SetCrywolfBossIndex(int index) // OK
{
	this->m_BossIndex = index;
}

int CCrywolf::GetCrywolfState() // OK
{
	return this->m_CrywolfState;
}

int CCrywolf::GetOccupationState() // OK
{
	return this->m_OccupationState;
}

int CCrywolf::GetUserRank(int aIndex) // OK
{
	int rank = 0;

	for(int n=0;n < MAX_CRYWOLF_RANK;n++)
	{
		if(gObj[aIndex].CrywolfMVPScore >= this->m_MVPRankScoreTable[n])
		{
			rank = n;
		}
	}

	return rank;
}

int CCrywolf::GetUserExperience(int aIndex,int rank) // OK
{
	int experience = this->m_MVPRankExpTable[rank];

	if(this->GetOccupationState() == 1)
	{
		experience = (experience*10)/100;
	}

	return (experience*gServerInfo.m_AddEventExperienceRate[gObj[aIndex].AccountLevel]);
}

int CCrywolf::GetUserScore(int aIndex,int bIndex,int type) // OK
{
	int AddMVPScore = ((type==6)?this->m_MVPScoreTable[6]:0);

	if(OBJECT_RANGE(bIndex) != 0)
	{
		switch(gObj[bIndex].Class)
		{
			case 340:
				AddMVPScore = this->m_MVPScoreTable[1];
				break;
			case 341:
				AddMVPScore = this->m_MVPScoreTable[3];
				break;
			case 344:
				AddMVPScore = this->m_MVPScoreTable[2];
				break;
			case 345:
				AddMVPScore = this->m_MVPScoreTable[4];
				break;
			case 348:
				AddMVPScore = this->m_MVPScoreTable[5];
				break;
			case 349:
				AddMVPScore = this->m_MVPScoreTable[0];
				break;
		}
	}

	gObj[aIndex].CrywolfMVPScore += AddMVPScore;

	return gObj[aIndex].CrywolfMVPScore;
}
