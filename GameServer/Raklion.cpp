// Raklion.cpp: implementation of the CRaklion class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Raklion.h"
#include "Map.h"
#include "MemScript.h"
#include "Message.h"
#include "Monster.h"
#include "MonsterSetBase.h"
#include "Notice.h"
#include "RaklionBattleUserMng.h"
#include "RaklionUtil.h"
#include "ServerInfo.h"
#include "Util.h"

CRaklion gRaklion;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRaklion::CRaklion() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	this->ClearData();

	#endif
}

CRaklion::~CRaklion() // OK
{

}

void CRaklion::ClearData() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	this->m_IsComplete = 0;
	this->m_AppearanceMSec = 0;
	this->m_BossZoneCloseMSec = 0;
	this->m_BossZoneOpenMSec = 0;
	this->m_BossEggHalfChecked = 0;
	this->m_BossEggCount = 0;
	this->m_BossEggCountTotal = 0;

	#endif
}

void CRaklion::Load(char* path) // OK
{
	#if(GAMESERVER_UPDATE>=401)

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

					this->m_AppearanceDelay = lpMemScript->GetNumber();

					this->m_BossZoneCloseDelay = lpMemScript->GetAsNumber();

					this->m_BossZoneOpenDelay = lpMemScript->GetAsNumber();
				}
				else if(section == 1)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					this->m_BossEggHalf = lpMemScript->GetNumber();

					this->m_BossEggMax = lpMemScript->GetAsNumber();
				}
				else if(section == 2)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					this->m_RaklionBattleOfSelupan.SetSelupanSkillDelay(lpMemScript->GetNumber());
				}
				else if(section == 3)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					this->m_RaklionBattleOfSelupan.SetPatternCondition(0,lpMemScript->GetNumber());

					this->m_RaklionBattleOfSelupan.SetPatternCondition(1,lpMemScript->GetAsNumber());

					this->m_RaklionBattleOfSelupan.SetPatternCondition(2,lpMemScript->GetAsNumber());

					this->m_RaklionBattleOfSelupan.SetPatternCondition(3,lpMemScript->GetAsNumber());

					this->m_RaklionBattleOfSelupan.SetPatternCondition(4,lpMemScript->GetAsNumber());

					this->m_RaklionBattleOfSelupan.SetPatternCondition(5,lpMemScript->GetAsNumber());
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

void CRaklion::MainProc() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	if(gServerInfo.m_RaklionEvent == 0)
	{
		return;
	}

	gRaklionBattleUserMng.CheckUserOnRaklionBossMap();

	gRaklionBattleUserMng.CheckUserState();

	switch(this->m_RaklionState)
	{
		case RAKLION_STATE_IDLE:
			this->ProcState_IDLE();
			break;
		case RAKLION_STATE_NOTIFY1:
			this->ProcState_NOTIFY1();
			break;
		case RAKLION_STATE_STANDBY:
			this->ProcState_STANDBY();
			break;
		case RAKLION_STATE_NOTIFY2:
			this->ProcState_NOTIFY2();
			break;
		case RAKLION_STATE_READY:
			this->ProcState_READY();
			break;
		case RAKLION_STATE_START_BATTLE:
			this->ProcState_START_BATTLE();
			break;
		case RAKLION_STATE_NOTIFY3:
			this->ProcState_NOTIFY3();
			break;
		case RAKLION_STATE_CLOSE_DOOR:
			this->ProcState_CLOSE_DOOR();
			break;
		case RAKLION_STATE_ALL_USER_DIE:
			this->ProcState_ALL_USER_DIE();
			break;
		case RAKLION_STATE_NOTIFY4:
			this->ProcState_NOTIFY4();
			break;
		case RAKLION_STATE_END:
			this->ProcState_END();
			break;
	}

	#endif
}

void CRaklion::ProcState_IDLE() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	if(this->m_BossEggCountTotal > 0 && this->GetBossEggCount() == 0)
	{
		this->SetState(RAKLION_STATE_STANDBY);
		return;
	}

	if(this->m_BossEggHalfChecked == 0 && this->GetBossEggCount() <= this->m_BossEggHalf)
	{
		this->m_BossEggHalfChecked = 1;
		this->SetState(RAKLION_STATE_NOTIFY1);
	}

	#endif
}

void CRaklion::ProcState_NOTIFY1() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	this->SetState(RAKLION_STATE_IDLE);

	#endif
}

void CRaklion::ProcState_STANDBY() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	if(((DWORD)this->m_AppearanceDelay*1000) <= (GetTickCount()-this->m_AppearanceMSec))
	{
		this->SetState(RAKLION_STATE_NOTIFY2);
		return;
	}

	#endif
}

void CRaklion::ProcState_NOTIFY2() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	this->SetState(RAKLION_STATE_READY);

	#endif
}

void CRaklion::ProcState_READY() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	this->SetState(RAKLION_STATE_START_BATTLE);

	#endif
}

void CRaklion::ProcState_START_BATTLE() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	if(((DWORD)this->m_BossZoneCloseDelay*1000) <= (GetTickCount()-this->m_BossZoneCloseMSec))
	{
		this->SetState(RAKLION_STATE_NOTIFY3);
		return;
	}

	this->m_RaklionBattleOfSelupan.SelupanLifeCheck();

	this->m_RaklionBattleOfSelupan.Run();

	#endif
}

void CRaklion::ProcState_NOTIFY3() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	this->m_RaklionBattleOfSelupan.SelupanLifeCheck();

	this->m_RaklionBattleOfSelupan.Run();

	this->SetState(RAKLION_STATE_CLOSE_DOOR);

	#endif
}

void CRaklion::ProcState_CLOSE_DOOR() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	if(gRaklionBattleUserMng.CheckBattleUsers() != 0)
	{
		this->m_RaklionBattleOfSelupan.SetSuccessValue(0);
		this->SetState(RAKLION_STATE_ALL_USER_DIE);
		return;
	}

	if(this->m_RaklionBattleOfSelupan.GetBattleOfSelupanState() == SELUPAN_STATE_DIE)
	{
		this->m_RaklionBattleOfSelupan.SetSuccessValue(1);
		this->SetState(RAKLION_STATE_NOTIFY4);
		return;
	}

	this->m_RaklionBattleOfSelupan.SelupanLifeCheck();

	this->m_RaklionBattleOfSelupan.Run();

	#endif
}

void CRaklion::ProcState_ALL_USER_DIE() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	this->SetState(RAKLION_STATE_NOTIFY4);

	#endif
}

void CRaklion::ProcState_NOTIFY4() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	if(((DWORD)this->m_BossZoneOpenDelay*1000) <= (GetTickCount()-this->m_BossZoneOpenMSec))
	{
		this->SetState(RAKLION_STATE_END);
		return;
	}

	#endif
}

void CRaklion::ProcState_END() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	this->SetState(RAKLION_STATE_IDLE);

	#endif
}

void CRaklion::SetState(int state) // OK
{
	#if(GAMESERVER_UPDATE>=401)

	switch(state)
	{
		case RAKLION_STATE_IDLE:
			this->SetState_IDLE();
			break;
		case RAKLION_STATE_NOTIFY1:
			this->SetState_NOTIFY1();
			break;
		case RAKLION_STATE_STANDBY:
			this->SetState_STANDBY();
			break;
		case RAKLION_STATE_NOTIFY2:
			this->SetState_NOTIFY2();
			break;
		case RAKLION_STATE_READY:
			this->SetState_READY();
			break;
		case RAKLION_STATE_START_BATTLE:
			this->SetState_START_BATTLE();
			break;
		case RAKLION_STATE_NOTIFY3:
			this->SetState_NOTIFY3();
			break;
		case RAKLION_STATE_CLOSE_DOOR:
			this->SetState_CLOSE_DOOR();
			break;
		case RAKLION_STATE_ALL_USER_DIE:
			this->SetState_ALL_USER_DIE();
			break;
		case RAKLION_STATE_NOTIFY4:
			this->SetState_NOTIFY4();
			break;
		case RAKLION_STATE_END:
			this->SetState_END();
			break;
	}

	gRaklionUtil.NotifyRaklionChangeState(this->GetRaklionState(),this->m_RaklionBattleOfSelupan.GetBattleOfSelupanState());

	#endif
}

void CRaklion::SetState_IDLE() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	LogAdd(LOG_BLACK,"[Raklion] SetState IDLE");

	this->SetRaklionState(RAKLION_STATE_IDLE);

	#endif
}

void CRaklion::SetState_NOTIFY1() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	this->SetRaklionState(RAKLION_STATE_NOTIFY1);

	#endif
}

void CRaklion::SetState_STANDBY() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	this->m_AppearanceMSec = GetTickCount();

	this->SetRaklionState(RAKLION_STATE_STANDBY);

	#endif
}

void CRaklion::SetState_NOTIFY2() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(140),(((this->m_BossZoneCloseDelay/60)==0)?1:(this->m_BossZoneCloseDelay/60)));

	this->SetRaklionState(RAKLION_STATE_NOTIFY2);

	#endif
}

void CRaklion::SetState_READY() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	this->m_RaklionBattleOfSelupan.SetState(SELUPAN_STATE_STANDBY);

	this->SetRaklionState(RAKLION_STATE_READY);

	#endif
}

void CRaklion::SetState_START_BATTLE() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	LogAdd(LOG_BLACK,"[Raklion] SetState START");

	this->m_BossZoneCloseMSec = GetTickCount();

	this->SetRaklionState(RAKLION_STATE_START_BATTLE);

	#endif
}

void CRaklion::SetState_NOTIFY3() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(141));

	this->SetRaklionState(RAKLION_STATE_NOTIFY3);

	#endif
}

void CRaklion::SetState_CLOSE_DOOR() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	this->SetRaklionState(RAKLION_STATE_CLOSE_DOOR);

	#endif
}

void CRaklion::SetState_ALL_USER_DIE() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	this->m_RaklionBattleOfSelupan.DeleteSelupan();

	this->m_RaklionBattleOfSelupan.DeleteSummonMonster();

	this->SetRaklionState(RAKLION_STATE_ALL_USER_DIE);

	#endif
}

void CRaklion::SetState_NOTIFY4() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	this->m_BossZoneOpenMSec = GetTickCount();

	gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(143),(((this->m_BossZoneOpenDelay/60)==0)?1:(this->m_BossZoneOpenDelay/60)));

	gRaklionUtil.NotifyRaklionResult(this->m_RaklionBattleOfSelupan.GetSuccessValue());

	this->SetRaklionState(RAKLION_STATE_NOTIFY4);

	#endif
}

void CRaklion::SetState_END() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	LogAdd(LOG_BLACK,"[Raklion] SetState END");

	this->BossEggDeleteAll();

	this->m_RaklionBattleOfSelupan.DeleteSelupan();

	this->m_RaklionBattleOfSelupan.DeleteSummonMonster();

	this->m_RaklionBattleOfSelupan.ResetAllData();

	this->ClearData();

	gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(144));

	this->BossEggRegenAll();

	this->SetRaklionState(RAKLION_STATE_END);

	#endif
}

void CRaklion::SetRaklionState(int state) // OK
{
	#if(GAMESERVER_UPDATE>=401)

	this->m_RaklionState = state;

	#endif
}

void CRaklion::SetRaklionDetailState(int state) // OK
{
	#if(GAMESERVER_UPDATE>=401)

	this->m_RaklionBattleOfSelupan.SetBattleOfSelupanState(state);

	#endif
}

void CRaklion::SelupanDie(int index) // OK
{
	#if(GAMESERVER_UPDATE>=401)

	if(OBJECT_RANGE(this->m_RaklionBattleOfSelupan.GetSelupanObjIndex()) == 0)
	{
		return;
	}

	if(this->m_RaklionBattleOfSelupan.GetSelupanObjIndex() != index)
	{
		return;
	}

	this->m_RaklionBattleOfSelupan.SetSelupanObjIndex(-1);

	#endif
}

void CRaklion::RaklionMonsterDieProc(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	#if(GAMESERVER_UPDATE>=401)

	int aIndex = gObjMonsterGetTopHitDamageUser(lpObj);

	if(OBJECT_RANGE(aIndex) != 0)
	{
		lpTarget = &gObj[aIndex];
	}

	if(lpObj->Class == 459)
	{
		this->SelupanDie(lpObj->Index);
		gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(142),lpTarget->Name);
	}

	if(lpObj->Class >= 460 && lpObj->Class <= 462)
	{
		this->BossEggDie();
		gRaklionUtil.NotifyRaklionObjectCount(this->GetBossEggCount(),0);
	}

	#endif
}

bool CRaklion::BossEggRegen() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	if(this->m_BossEggCount >= this->m_BossEggMax)
	{
		return 0;
	}

	this->m_BossEggCount++;
	return 1;

	#else

	return 0;

	#endif
}

bool CRaklion::BossEggDie() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	if(this->m_BossEggCount <= 0)
	{
		return 0;
	}

	this->m_BossEggCount--;
	return 1;

	#else

	return 0;

	#endif
}

void CRaklion::BossEggRegenAll() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	for(int n=0;n < gMonsterSetBase.m_count;n++)
	{
		MONSTER_SET_BASE_INFO* lpInfo = &gMonsterSetBase.m_MonsterSetBaseInfo[n];

		if(lpInfo->Type != 4 || (lpInfo->MonsterClass != 460 && lpInfo->MonsterClass != 461 && lpInfo->MonsterClass != 462) || lpInfo->Map != MAP_RAKLION2)
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

		if(this->BossEggRegen() == 0)
		{
			gObjDel(index);
			continue;
		}
	}

	this->m_BossEggCountTotal = this->GetBossEggCount();

	#endif
}

void CRaklion::BossEggDeleteAll() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	for(int n=OBJECT_START_MONSTER;n < MAX_OBJECT_MONSTER;n++)
	{
		if(gObjIsConnected(n) != 0 && gObj[n].Map == MAP_RAKLION2 && (gObj[n].Class == 460 || gObj[n].Class == 461 || gObj[n].Class == 462))
		{
			gObjDel(n);
			this->BossEggDie();
		}
	}

	this->m_BossEggCountTotal = this->GetBossEggCount();

	#endif
}

int CRaklion::GetRaklionState() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	return this->m_RaklionState;

	#else

	return 0;

	#endif
}

int CRaklion::GetRaklionDetailState() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	return this->m_RaklionBattleOfSelupan.GetBattleOfSelupanState();

	#else

	return 0;

	#endif
}

int CRaklion::GetBossEggCount() // OK
{
	int count = 0;

	#if(GAMESERVER_UPDATE>=401)

	for(int n=OBJECT_START_MONSTER;n < MAX_OBJECT_MONSTER;n++)
	{
		if(gObjIsConnected(n) != 0 && gObj[n].Map == MAP_RAKLION2 && (gObj[n].Class == 460 || gObj[n].Class == 461 || gObj[n].Class == 462))
		{
			count++;
		}
	}

	#endif

	return count;
}
