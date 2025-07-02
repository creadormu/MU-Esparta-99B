// KanturuBattleOfMaya.cpp: implementation of the CKanturuBattleOfMaya class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KanturuBattleOfMaya.h"
#include "Kanturu.h"
#include "KanturuBattleUserMng.h"
#include "KanturuMonsterMng.h"
#include "KanturuUtil.h"
#include "MemScript.h"
#include "Message.h"
#include "MonsterAIGroup.h"
#include "Util.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKanturuBattleOfMaya::CKanturuBattleOfMaya() // OK
{
	this->SetBattleOfMayaState(KANTURU_MAYA_NONE);

	this->SetBattleSceneState(-1);

	this->SetEntrancePermit(0);

	this->SetSuccessValue(0);

	this->SetSceneSuccessValue(0);

	this->m_MayaHandDieTimeCounter = 0;

	this->m_MayaLeftHandObjIndex = -1;

	this->m_MayaRightHandObjIndex = -1;

	for(int n=0;n < KANTURU_MAYA_AI_CHANGE_TIME;n++)
	{
		this->m_MayaHandAIChangeTime[n] = -1;
	}

	this->m_MayaHandAIAppliedTime = 0;

	this->m_MayaHandCurrentAI = 0;

	for(int n=0;n < KANTURU_MAYA_GROUP_NUMBER;n++)
	{
		this->m_MayaHandGroupNumber[n] = -1;
	}

	this->ResetAllData();
}

CKanturuBattleOfMaya::~CKanturuBattleOfMaya() // OK
{

}

void CKanturuBattleOfMaya::ResetAllData() // OK
{
	for(int n=0;n < KANTURU_MAYA_TIME_INFO;n++)
	{
		this->m_BattleTimeInfo[n].ResetTimeInfo();
	}

	this->m_BattleTimeInfoCount = 0;

	for(int n=0;n < KANTURU_MAYA_MAX;n++)
	{
		this->m_StateInfo[n].ResetTimeInfo();
	}

	this->m_StateInfoCount = 0;

	this->m_MayaHandAIChangeTimeCount = 0;

	this->m_MayaHAndGroupNumberCount = 0;

	this->m_MayaIceStormUsingRate = 0;
}

void CKanturuBattleOfMaya::Load(char* path) // OK
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

	this->ResetAllData();

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
				if(section == 2)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					int state = lpMemScript->GetNumber();

					int condition = lpMemScript->GetAsNumber();

					int value = lpMemScript->GetAsNumber();

					this->m_BattleTimeInfo[this->m_BattleTimeInfoCount].SetStateInfo(state);

					this->m_BattleTimeInfo[this->m_BattleTimeInfoCount].SetCondition(condition);

					this->m_BattleTimeInfo[this->m_BattleTimeInfoCount].SetValue(((condition==1)?(value*1000):value));

					this->m_BattleTimeInfoCount++;
				}
				else if(section == 3)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					int state = lpMemScript->GetNumber();

					int condition = lpMemScript->GetAsNumber();

					int value = lpMemScript->GetAsNumber();

					this->m_StateInfo[this->m_StateInfoCount].SetStateInfo(state);

					this->m_StateInfo[this->m_StateInfoCount].SetCondition(condition);

					this->m_StateInfo[this->m_StateInfoCount].SetValue(((condition==1)?(value*1000):value));

					this->m_StateInfoCount++;
				}
				else if(section == 4)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					this->m_MayaHandAIChangeTime[this->m_MayaHandAIChangeTimeCount++] = lpMemScript->GetNumber()*1000;
				}
				else if(section == 5)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					this->m_MayaHandGroupNumber[this->m_MayaHAndGroupNumberCount++] = lpMemScript->GetNumber();
				}
				else if(section == 6)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					this->m_MayaIceStormUsingRate = lpMemScript->GetNumber();
				}
				else
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}
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

void CKanturuBattleOfMaya::MainProc() // OK
{
	this->CheckStateTime();

	this->CheckMayaBattleTime();

	this->CheckMayaBattleUser();

	gKanturuBattleUserMng.CheckUserState();

	switch(this->m_BattleOfMayaState)
	{
		case KANTURU_MAYA_NONE:
			this->ProcState_NONE();
			break;
		case KANTURU_MAYA_STANDBY_1:
			this->ProcState_STANBY1();
			break;
		case KANTURU_MAYA_NOTIFY_1:
			this->ProcState_NOTIFY1();
			break;
		case KANTURU_MAYA_START_1:
			this->ProcState_START1();
			break;
		case KANTURU_MAYA_1:
			this->ProcState_MAYA1();
			break;
		case KANTURU_MAYA_END_1:
			this->ProcState_END_MAYA1();
			break;
		case KANTURU_MAYA_ENDCYCLE_1:
			this->ProcState_ENDCYCLE_MAYA1();
			break;
		case KANTURU_MAYA_STANDBY_2:
			this->ProcState_STANBY2();
			break;
		case KANTURU_MAYA_START_2:
			this->ProcState_START2();
			break;
		case KANTURU_MAYA_2:
			this->ProcState_MAYA2();
			break;
		case KANTURU_MAYA_END_2:
			this->ProcState_END_MAYA2();
			break;
		case KANTURU_MAYA_ENDCYCLE_2:
			this->ProcState_ENDCYCLE_MAYA2();
			break;
		case KANTURU_MAYA_STANDBY_3:
			this->ProcState_STANBY3();
			break;
		case KANTURU_MAYA_START_3:
			this->ProcState_START3();
			break;
		case KANTURU_MAYA_3:
			this->ProcState_MAYA3();
			break;
		case KANTURU_MAYA_END_3:
			this->ProcState_END_MAYA3();
			break;
		case KANTURU_MAYA_ENDCYCLE_3:
			this->ProcState_ENDCYCLE_MAYA3();
			break;
		case KANTURU_MAYA_END:
			this->ProcState_END();
			break;
		case KANTURU_MAYA_ENDCYCLE:
			this->ProcState_ENDCYCLE();
			break;
	}
}

void CKanturuBattleOfMaya::ProcState_NONE() // OK
{

}

void CKanturuBattleOfMaya::ProcState_STANBY1() // OK
{

}

void CKanturuBattleOfMaya::ProcState_NOTIFY1() // OK
{

}

void CKanturuBattleOfMaya::ProcState_START1() // OK
{
	if(gKanturuMonsterMng.IsExistAliveMonster() == 0)
	{
		this->SetSuccessValue(1);

		this->SetState(KANTURU_MAYA_1);
	}
	else
	{
		this->m_KanturuMaya.KanturuMayaAct_IceStorm(this->m_MayaIceStormUsingRate);

		gKanturuUtil.NotifyKanturuObjectCount(gKanturuMonsterMng.GetAliveMonsterCount(),gKanturuBattleUserMng.GetUserCount());
	}
}

void CKanturuBattleOfMaya::ProcState_MAYA1() // OK
{
	if(gObj[this->m_MayaLeftHandObjIndex].Live == 0)
	{
		if(this->m_MayaHandDieTimeCounter == 0)
		{
			this->SetSuccessValue(1);

			this->SetSceneSuccessValue(1);

			this->m_MayaHandDieTimeCounter = GetTickCount();

			gKanturuUtil.NotifyKanturuObjectCount(0,gKanturuBattleUserMng.GetUserCount());
		}
		else
		{
			if((GetTickCount()-this->m_MayaHandDieTimeCounter) >= 3000)
			{
				this->SetState(KANTURU_MAYA_END_1);
			}
		}
	}
	else
	{
		this->m_KanturuMaya.KanturuMayaAct_Hands();

		this->ChangeAI(this->m_MayaHandGroupNumber[0]);

		gKanturuUtil.NotifyKanturuObjectCount(1,gKanturuBattleUserMng.GetUserCount());
	}
}

void CKanturuBattleOfMaya::ProcState_END_MAYA1() // OK
{

}

void CKanturuBattleOfMaya::ProcState_ENDCYCLE_MAYA1() // OK
{

}

void CKanturuBattleOfMaya::ProcState_STANBY2() // OK
{

}

void CKanturuBattleOfMaya::ProcState_START2()
{
	if(gKanturuMonsterMng.IsExistAliveMonster() == 0)
	{
		this->SetSuccessValue(1);

		this->SetState(KANTURU_MAYA_2);
	}
	else
	{
		this->m_KanturuMaya.KanturuMayaAct_IceStorm(this->m_MayaIceStormUsingRate);

		gKanturuUtil.NotifyKanturuObjectCount(gKanturuMonsterMng.GetAliveMonsterCount(),gKanturuBattleUserMng.GetUserCount());
	}
}

void CKanturuBattleOfMaya::ProcState_MAYA2()
{
	if(gObj[this->m_MayaRightHandObjIndex].Live == 0)
	{
		if(this->m_MayaHandDieTimeCounter == 0)
		{
			this->SetSuccessValue(1);

			this->SetSceneSuccessValue(1);

			this->m_MayaHandDieTimeCounter = GetTickCount();

			gKanturuUtil.NotifyKanturuObjectCount(0,gKanturuBattleUserMng.GetUserCount());
		}
		else
		{
			if((GetTickCount()-this->m_MayaHandDieTimeCounter) >= 3000)
			{
				this->SetState(KANTURU_MAYA_END_2);
			}
		}
	}
	else
	{
		this->m_KanturuMaya.KanturuMayaAct_Hands();

		this->ChangeAI(this->m_MayaHandGroupNumber[1]);

		gKanturuUtil.NotifyKanturuObjectCount(1,gKanturuBattleUserMng.GetUserCount());
	}
}

void CKanturuBattleOfMaya::ProcState_END_MAYA2() // OK
{

}

void CKanturuBattleOfMaya::ProcState_ENDCYCLE_MAYA2() // OK
{

}

void CKanturuBattleOfMaya::ProcState_STANBY3() // OK
{

}

void CKanturuBattleOfMaya::ProcState_START3() // OK
{
	if(gKanturuMonsterMng.IsExistAliveMonster() == 0)
	{
		this->SetSuccessValue(1);

		this->SetState(KANTURU_MAYA_3);
	}
	else
	{
		this->m_KanturuMaya.KanturuMayaAct_IceStorm(this->m_MayaIceStormUsingRate);

		gKanturuUtil.NotifyKanturuObjectCount(gKanturuMonsterMng.GetAliveMonsterCount(),gKanturuBattleUserMng.GetUserCount());
	}
}

void CKanturuBattleOfMaya::ProcState_MAYA3() // OK
{
	if(gObj[this->m_MayaLeftHandObjIndex].Live == 0 && gObj[this->m_MayaRightHandObjIndex].Live == 0)
	{
		if(this->m_MayaHandDieTimeCounter == 0)
		{
			this->SetSuccessValue(1);

			this->SetSceneSuccessValue(1);

			this->m_MayaHandDieTimeCounter = GetTickCount();

			gKanturuUtil.NotifyKanturuObjectCount(0,gKanturuBattleUserMng.GetUserCount());
		}
		else
		{
			if((GetTickCount()-this->m_MayaHandDieTimeCounter) >= 3000)
			{
				this->SetState(KANTURU_MAYA_END_3);
			}
		}
	}
	else
	{
		this->m_KanturuMaya.KanturuMayaAct_Hands();

		this->ChangeAI(this->m_MayaHandGroupNumber[2]);

		if((gObj[this->m_MayaLeftHandObjIndex].Live == 0 && gObj[this->m_MayaRightHandObjIndex].Live != 0) || (gObj[this->m_MayaLeftHandObjIndex].Live != 0 && gObj[this->m_MayaRightHandObjIndex].Live == 0))
		{
			gKanturuUtil.NotifyKanturuObjectCount(1,gKanturuBattleUserMng.GetUserCount());
		}
		else
		{
			gKanturuUtil.NotifyKanturuObjectCount(2,gKanturuBattleUserMng.GetUserCount());
		}
	}
}

void CKanturuBattleOfMaya::ProcState_END_MAYA3() // OK
{

}

void CKanturuBattleOfMaya::ProcState_ENDCYCLE_MAYA3() // OK
{

}

void CKanturuBattleOfMaya::ProcState_END() // OK
{

}

void CKanturuBattleOfMaya::ProcState_ENDCYCLE() // OK
{

}

void CKanturuBattleOfMaya::SetState(int state) // OK
{
	this->m_StateInfo[state].SetConditionAppliedTime();

	gKanturuMonsterMng.ResetRegenMonsterObjData();

	this->m_KanturuMaya.Init();

	this->ResetAIMonster();

	switch(state)
	{
		case KANTURU_MAYA_NONE:
			this->SetState_NONE();
			break;
		case KANTURU_MAYA_STANDBY_1:
			this->SetState_STANBY1();
			break;
		case KANTURU_MAYA_NOTIFY_1:
			this->SetState_NOTIFY1();
			break;
		case KANTURU_MAYA_START_1:
			this->SetState_START1();
			break;
		case KANTURU_MAYA_1:
			this->SetState_MAYA1();
			break;
		case KANTURU_MAYA_END_1:
			this->SetState_END_MAYA1();
			break;
		case KANTURU_MAYA_ENDCYCLE_1:
			this->SetState_ENDCYCLE_MAYA1();
			break;
		case KANTURU_MAYA_STANDBY_2:
			this->SetState_STANBY2();
			break;
		case KANTURU_MAYA_START_2:
			this->SetState_START2();
			break;
		case KANTURU_MAYA_2:
			this->SetState_MAYA2();
			break;
		case KANTURU_MAYA_END_2:
			this->SetState_END_MAYA2();
			break;
		case KANTURU_MAYA_ENDCYCLE_2:
			this->SetState_ENDCYCLE_MAYA2();
			break;
		case KANTURU_MAYA_STANDBY_3:
			this->SetState_STANBY3();
			break;
		case KANTURU_MAYA_START_3:
			this->SetState_START3();
			break;
		case KANTURU_MAYA_3:
			this->SetState_MAYA3();
			break;
		case KANTURU_MAYA_END_3:
			this->SetState_END_MAYA3();
			break;
		case KANTURU_MAYA_ENDCYCLE_3:
			this->SetState_ENDCYCLE_MAYA3();
			break;
		case KANTURU_MAYA_END:
			this->SetState_END();
			break;
		case KANTURU_MAYA_ENDCYCLE:
			this->SetState_ENDCYCLE();
			break;
	}

	gKanturuUtil.NotifyKanturuChangeState(KANTURU_STATE_BATTLE_OF_MAYA,this->m_BattleOfMayaState);
}

void CKanturuBattleOfMaya::SetState_NONE() // OK
{
	this->SetBattleOfMayaState(KANTURU_MAYA_NONE);

	this->SetEntrancePermit(0);
}

void CKanturuBattleOfMaya::SetState_STANBY1() // OK
{
	this->SetBattleOfMayaState(KANTURU_MAYA_STANDBY_1);

	this->SetEntrancePermit(1);

	gKanturuBattleUserMng.ResetAllData();
}

void CKanturuBattleOfMaya::SetState_NOTIFY1() // OK
{
	this->SetBattleOfMayaState(KANTURU_MAYA_NOTIFY_1);

	this->SetEntrancePermit(0);
}

void CKanturuBattleOfMaya::SetState_START1() // OK
{
	LogAdd(LOG_BLACK,"[Kanturu] SetState START");

	this->SetBattleOfMayaState(KANTURU_MAYA_START_1);

	this->SetBattleSceneState(0);

	this->SetEntrancePermit(0);

	this->SetSuccessValue(0);

	this->SetSceneSuccessValue(0);

	this->m_BattleTimeInfo[this->m_SceneState].SetAppliedTime();

	if(this->m_BattleTimeInfo[this->m_SceneState].GetCondition() == 1)
	{
		gKanturuUtil.NotifyKanturuBattleTime(this->m_BattleTimeInfo[this->m_SceneState].GetValue());
	}

	gKanturuMonsterMng.SetKanturuMonster(0);

	this->m_KanturuMaya.SetMayaObjIndex(gKanturuMonsterMng.GetMayaObjIndex());
}

void CKanturuBattleOfMaya::SetState_MAYA1() // OK
{
	this->SetBattleOfMayaState(KANTURU_MAYA_1);

	this->SetEntrancePermit(0);

	this->SetSuccessValue(0);

	this->SetSceneSuccessValue(0);

	this->m_MayaHandDieTimeCounter = 0;

	gKanturuMonsterMng.SetKanturuMonster(3);

	this->m_KanturuMaya.SetMayaObjIndex(gKanturuMonsterMng.GetMayaObjIndex());

	this->SetAIMonsterGroup(this->m_MayaHandGroupNumber[0]);

	this->m_MayaLeftHandObjIndex = CMonsterAIGroup::FindGroupLeader(this->m_MayaHandGroupNumber[0]);
}

void CKanturuBattleOfMaya::SetState_END_MAYA1() // OK
{
	this->SetBattleOfMayaState(KANTURU_MAYA_END_1);

	this->SetBattleSceneState(-1);

	this->SetEntrancePermit(0);

	this->SetSceneSuccessValue(0);

	gKanturuUtil.NotifyKanturuResult(this->GetSuccessValue());
}

void CKanturuBattleOfMaya::SetState_ENDCYCLE_MAYA1() // OK
{
	this->SetBattleOfMayaState(KANTURU_MAYA_ENDCYCLE_1);

	this->SetEntrancePermit(0);

	gKanturuUtil.SendMsgKanturuBossMapUser(gMessage.GetMessage(64));
}

void CKanturuBattleOfMaya::SetState_STANBY2() // OK
{
	this->SetBattleOfMayaState(KANTURU_MAYA_STANDBY_2);

	this->SetEntrancePermit(1);
}

void CKanturuBattleOfMaya::SetState_START2() // OK
{
	this->SetBattleOfMayaState(KANTURU_MAYA_START_2);

	this->SetBattleSceneState(1);

	this->SetEntrancePermit(0);

	this->SetSuccessValue(0);

	this->SetSceneSuccessValue(0);

	this->m_BattleTimeInfo[this->m_SceneState].SetAppliedTime();

	if(this->m_BattleTimeInfo[this->m_SceneState].GetCondition() == 1)
	{
		gKanturuUtil.NotifyKanturuBattleTime(this->m_BattleTimeInfo[this->m_SceneState].GetValue());
	}

	gKanturuMonsterMng.SetKanturuMonster(0);

	this->m_KanturuMaya.SetMayaObjIndex(gKanturuMonsterMng.GetMayaObjIndex());
}

void CKanturuBattleOfMaya::SetState_MAYA2() // OK
{
	this->SetBattleOfMayaState(KANTURU_MAYA_2);

	this->SetEntrancePermit(0);

	this->SetSuccessValue(0);

	this->SetSceneSuccessValue(0);

	this->m_MayaHandDieTimeCounter = 0;

	gKanturuMonsterMng.SetKanturuMonster(4);

	this->m_KanturuMaya.SetMayaObjIndex(gKanturuMonsterMng.GetMayaObjIndex());

	this->SetAIMonsterGroup(this->m_MayaHandGroupNumber[1]);

	this->m_MayaRightHandObjIndex = CMonsterAIGroup::FindGroupLeader(this->m_MayaHandGroupNumber[1]);
}

void CKanturuBattleOfMaya::SetState_END_MAYA2() // OK
{
	this->SetBattleOfMayaState(KANTURU_MAYA_END_2);

	this->SetBattleSceneState(-1);

	this->SetEntrancePermit(0);

	this->SetSceneSuccessValue(0);

	gKanturuUtil.NotifyKanturuResult(this->GetSuccessValue());
}

void CKanturuBattleOfMaya::SetState_ENDCYCLE_MAYA2() // OK
{
	this->SetBattleOfMayaState(KANTURU_MAYA_ENDCYCLE_2);

	this->SetEntrancePermit(0);

	gKanturuUtil.SendMsgKanturuBossMapUser(gMessage.GetMessage(64));
}

void CKanturuBattleOfMaya::SetState_STANBY3() // OK
{
	this->SetBattleOfMayaState(KANTURU_MAYA_STANDBY_3);

	this->SetEntrancePermit(1);
}

void CKanturuBattleOfMaya::SetState_START3() // OK
{
	this->SetBattleOfMayaState(KANTURU_MAYA_START_3);

	this->SetBattleSceneState(2);

	this->SetEntrancePermit(0);

	this->SetSuccessValue(0);

	this->SetSceneSuccessValue(0);

	this->m_BattleTimeInfo[this->m_SceneState].SetAppliedTime();

	if(this->m_BattleTimeInfo[this->m_SceneState].GetCondition() == 1)
	{
		gKanturuUtil.NotifyKanturuBattleTime(this->m_BattleTimeInfo[this->m_SceneState].GetValue());
	}

	gKanturuMonsterMng.SetKanturuMonster(1);
}

void CKanturuBattleOfMaya::SetState_MAYA3() // OK
{
	this->SetBattleOfMayaState(KANTURU_MAYA_3);

	this->SetEntrancePermit(0);

	this->SetSuccessValue(0);

	this->SetSceneSuccessValue(0);

	this->m_MayaHandDieTimeCounter = 0;

	gKanturuMonsterMng.SetKanturuMonster(5);

	this->m_KanturuMaya.SetMayaObjIndex(gKanturuMonsterMng.GetMayaObjIndex());

	this->SetAIMonsterGroup(this->m_MayaHandGroupNumber[2]);

	this->m_MayaLeftHandObjIndex = CMonsterAIGroup::FindGroupLeader(this->m_MayaHandGroupNumber[2]);

	this->m_MayaRightHandObjIndex = CMonsterAIGroup::FindGroupMemberObjectIndex(this->m_MayaHandGroupNumber[2],1);
}

void CKanturuBattleOfMaya::SetState_END_MAYA3() // OK
{
	this->SetBattleOfMayaState(KANTURU_MAYA_END_3);

	this->SetBattleSceneState(-1);

	this->SetEntrancePermit(0);

	this->SetSceneSuccessValue(0);

	gKanturuUtil.NotifyKanturuResult(this->GetSuccessValue());
}

void CKanturuBattleOfMaya::SetState_ENDCYCLE_MAYA3() // OK
{
	this->SetBattleOfMayaState(KANTURU_MAYA_ENDCYCLE_3);

	this->SetEntrancePermit(0);
}

void CKanturuBattleOfMaya::SetState_END() // OK
{
	this->SetBattleOfMayaState(KANTURU_MAYA_END);

	this->SetBattleSceneState(-1);

	this->SetEntrancePermit(0);

	this->SetSceneSuccessValue(0);

	if(this->GetSuccessValue() == 0)
	{
		gKanturuUtil.SendMsgKanturuBossMapUser(gMessage.GetMessage(67));
	}
	else
	{
		gKanturuUtil.SendMsgKanturuBossMapUser(gMessage.GetMessage(65));
	}

	gKanturuUtil.NotifyKanturuResult(this->GetSuccessValue());
}

void CKanturuBattleOfMaya::SetState_ENDCYCLE() // OK
{
	this->SetBattleOfMayaState(KANTURU_MAYA_ENDCYCLE);

	this->SetEntrancePermit(0);

	if(this->GetSuccessValue() == 0)
	{
		gKanturuBattleUserMng.MoveAllUser(137);
		gKanturuBattleUserMng.ResetAllData();
	}
}

void CKanturuBattleOfMaya::CheckStateTime() // OK
{
	if(this->GetBattleOfMayaState() != KANTURU_MAYA_NONE && this->m_StateInfo[this->GetBattleOfMayaState()].GetCondition() == 1 && this->m_StateInfo[this->GetBattleOfMayaState()].IsTimeOut() != 0)
	{
		this->SetState(((this->GetBattleOfMayaState()==KANTURU_MAYA_ENDCYCLE)?KANTURU_MAYA_NONE:(this->GetBattleOfMayaState()+1)));
	}
}

void CKanturuBattleOfMaya::CheckMayaBattleTime() // OK
{
	if(this->GetBattleSceneState() != -1 && this->m_BattleTimeInfo[this->m_SceneState].GetCondition() == 1 && this->GetSceneSuccessValue() == 0 && this->GetBattleOfMayaState() != KANTURU_MAYA_END && this->m_BattleTimeInfo[this->m_SceneState].IsTimeOut() != 0)
	{
		this->SetSuccessValue(0);

		this->SetSceneSuccessValue(0);

		this->SetState(KANTURU_MAYA_END);
	}
}

void CKanturuBattleOfMaya::CheckMayaBattleUser() // OK
{
	if(this->GetBattleOfMayaState() == KANTURU_MAYA_NOTIFY_1 || this->GetBattleOfMayaState() == KANTURU_MAYA_START_1 || this->GetBattleOfMayaState() == KANTURU_MAYA_1 || this->GetBattleOfMayaState() == KANTURU_MAYA_START_2 || this->GetBattleOfMayaState() == KANTURU_MAYA_2 || this->GetBattleOfMayaState() == KANTURU_MAYA_START_3 || this->GetBattleOfMayaState() == KANTURU_MAYA_3)
	{
		if(gKanturuBattleUserMng.IsEmpty() != 0)
		{
			this->SetSuccessValue(0);

			this->SetSceneSuccessValue(0);

			this->SetState(KANTURU_MAYA_END);
		}
	}
}

void CKanturuBattleOfMaya::ResetAIMonster() // OK
{
	if(OBJECT_RANGE(this->m_MayaLeftHandObjIndex) != 0)
	{
		gObj[this->m_MayaLeftHandObjIndex].Life = 0;
	}

	if(OBJECT_RANGE(this->m_MayaRightHandObjIndex) != 0)
	{
		gObj[this->m_MayaRightHandObjIndex].Life = 0;
	}

	for(int n=0;n < KANTURU_MAYA_AI_CHANGE_TIME;n++)
	{
		CMonsterAIGroup::DelGroupInstance(this->m_MayaHandGroupNumber[n]);
	}

	this->m_MayaLeftHandObjIndex = -1;

	this->m_MayaRightHandObjIndex = -1;

	this->m_MayaHandAIAppliedTime = 0;

	this->m_MayaHandCurrentAI = 0;
}

void CKanturuBattleOfMaya::SetAIMonsterGroup(int GroupNumber) // OK
{
	CMonsterAIGroup::Init(GroupNumber);

	CMonsterAIGroup::ChangeAIOrder(GroupNumber,0);

	this->m_MayaHandAIAppliedTime = GetTickCount();
}

void CKanturuBattleOfMaya::ChangeAI(int GroupNumber) // OK
{
	if((GetTickCount()-this->m_MayaHandAIAppliedTime) >= ((DWORD)this->m_MayaHandAIChangeTime[this->m_MayaHandCurrentAI]))
	{
		CMonsterAIGroup::ChangeAIOrder(GroupNumber,((this->m_MayaHandCurrentAI==KANTURU_MAYA_GROUP_NUMBER)?this->m_MayaHandCurrentAI:(this->m_MayaHandCurrentAI+1)));

		this->m_MayaHandCurrentAI = ((this->m_MayaHandCurrentAI==KANTURU_MAYA_GROUP_NUMBER)?this->m_MayaHandCurrentAI:(this->m_MayaHandCurrentAI+1));

		this->m_MayaHandAIAppliedTime = GetTickCount();
	}
}

void CKanturuBattleOfMaya::SetBattleOfMayaState(int state) // OK
{
	this->m_BattleOfMayaState = state;
}

void CKanturuBattleOfMaya::SetBattleSceneState(int scene) // OK
{
	this->m_SceneState = scene;
}

void CKanturuBattleOfMaya::SetEntrancePermit(int permit) // OK
{
	this->m_EntrancePermit = permit;
}

void CKanturuBattleOfMaya::SetSuccessValue(int success) // OK
{
	this->m_IsSucccess = success;
}

void CKanturuBattleOfMaya::SetSceneSuccessValue(int success) // OK
{
	this->m_IsSceneSuccess = success;
}

int CKanturuBattleOfMaya::GetBattleOfMayaState() // OK
{
	return this->m_BattleOfMayaState;
}

int CKanturuBattleOfMaya::GetBattleSceneState() // OK
{
	return this->m_SceneState;
}

int CKanturuBattleOfMaya::GetRemainTime() // OK
{
	return this->m_StateInfo[this->GetBattleOfMayaState()].GetRemainTime();
}

int CKanturuBattleOfMaya::GetEntrancePermit() // OK
{
	return this->m_EntrancePermit;
}

int CKanturuBattleOfMaya::GetSuccessValue() // OK
{
	return this->m_IsSucccess;
}

int CKanturuBattleOfMaya::GetSceneSuccessValue() // OK
{
	return this->m_IsSceneSuccess;
}
