// Kanturu.cpp: implementation of the CKanturu class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Kanturu.h"
#include "KanturuBattleUserMng.h"
#include "KanturuMonsterMng.h"
#include "KanturuUtil.h"
#include "MemScript.h"
#include "Monster.h"
#include "Path.h"
#include "ServerInfo.h"
#include "User.h"
#include "Util.h"

CKanturu gKanturu;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKanturu::CKanturu() // OK
{
	#if(GAMESERVER_UPDATE>=201)

	this->SetKanturuEnable(0);

	this->SetKanturuState(KANTURU_STATE_NONE);

	this->SetEnableCheckMoonStone(0);

	this->ResetAllData();

	#endif
}

CKanturu::~CKanturu() // OK
{

}

void CKanturu::ResetAllData() // OK
{
	#if(GAMESERVER_UPDATE>=201)

	for(int n=0;n < KANTURU_STATE_MAX;n++)
	{
		this->m_StateInfo[n].ResetTimeInfo();
	}

	this->m_StateInfoCount = 0;

	#endif
}

void CKanturu::Load(char* path) // OK
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
				if(section == 0)
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
				else if(section == 10)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					this->SetEnableCheckMoonStone(lpMemScript->GetNumber());
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

	this->m_BattleStanby.Load(path);

	this->m_BattleOfMaya.Load(path);

	this->m_BattleOfNightmare.Load(path);

	this->m_TowerOfRefinement.Load(path);

	delete lpMemScript;

	#endif
}

void CKanturu::MainProc() // OK
{
	#if(GAMESERVER_UPDATE>=201)

	if(gServerInfo.m_KanturuEvent == 0)
	{
		return;
	}

	this->CheckStateTime();

	switch(this->m_KanturuState)
	{
		case KANTURU_STATE_NONE:
			this->ProcState_NONE();
			break;
		case KANTURU_STATE_BATTLE_STANDBY:
			this->ProcState_BATTLE_STANDBY();
			break;
		case KANTURU_STATE_BATTLE_OF_MAYA:
			this->ProcState_BATTLE_OF_MAYA();
			break;
		case KANTURU_STATE_BATTLE_OF_NIGHTMARE:
			this->ProcState_BATTLE_OF_NIGHTMARE();
			break;
		case KANTURU_STATE_TOWER_OF_REFINEMENT:
			this->ProcState_TOWER_OF_REFINEMENT();
			break;
		case KANTURU_STATE_END:
			this->ProcState_END();
			break;
	}

	#endif
}

void CKanturu::ProcState_NONE() // OK
{
	#if(GAMESERVER_UPDATE>=201)

	this->SetState(KANTURU_STATE_BATTLE_STANDBY);

	#endif
}

void CKanturu::ProcState_BATTLE_STANDBY() // OK
{
	#if(GAMESERVER_UPDATE>=201)

	if(this->m_BattleStanby.GetBattleStanbyState() == KANTURU_STANBY_END)
	{
		this->m_BattleStanby.SetState_NONE();
		this->SetState(KANTURU_STATE_BATTLE_OF_MAYA);
		return;
	}

	this->m_BattleStanby.MainProc();

	#endif
}

void CKanturu::ProcState_BATTLE_OF_MAYA() // OK
{
	#if(GAMESERVER_UPDATE>=201)

	if(this->m_BattleOfMaya.GetBattleOfMayaState() == KANTURU_MAYA_ENDCYCLE)
	{
		this->m_BattleOfMaya.SetState_NONE();
		this->SetState(((this->m_BattleOfMaya.GetSuccessValue()==0)?KANTURU_STATE_END:KANTURU_STATE_BATTLE_OF_NIGHTMARE));
		return;
	}

	this->m_BattleOfMaya.MainProc();

	#endif
}

void CKanturu::ProcState_BATTLE_OF_NIGHTMARE() // OK
{
	#if(GAMESERVER_UPDATE>=201)

	if(this->m_BattleOfNightmare.GetBattleOfNightmareState() == KANTURU_NIGHTMARE_ENDCYCLE)
	{
		this->m_BattleOfNightmare.SetState_NONE();
		this->SetState(((this->m_BattleOfNightmare.GetSuccessValue()==0)?KANTURU_STATE_END:KANTURU_STATE_TOWER_OF_REFINEMENT));
		return;
	}

	this->m_BattleOfNightmare.MainProc();

	#endif
}

void CKanturu::ProcState_TOWER_OF_REFINEMENT() // OK
{
	#if(GAMESERVER_UPDATE>=201)

	if(this->m_TowerOfRefinement.GetTowerOfRefinementState() == KANTURU_TOWER_OF_REFINEMENT_END)
	{
		this->m_TowerOfRefinement.SetState_NONE();
		this->SetState(KANTURU_STATE_END);
		return;
	}

	this->m_TowerOfRefinement.MainProc();

	#endif
}

void CKanturu::ProcState_END() // OK
{
	#if(GAMESERVER_UPDATE>=201)

	this->SetState(KANTURU_STATE_NONE);

	#endif
}

void CKanturu::SetState(int state) // OK
{
	#if(GAMESERVER_UPDATE>=201)

	this->m_StateInfo[state].SetConditionAppliedTime();

	gKanturuMonsterMng.ResetRegenMonsterObjData();

	this->SetKanturuMapAttr(0);

	switch(state)
	{
		case KANTURU_STATE_NONE:
			this->SetState_NONE();
			break;
		case KANTURU_STATE_BATTLE_STANDBY:
			this->SetState_BATTLE_STANDBY();
			break;
		case KANTURU_STATE_BATTLE_OF_MAYA:
			this->SetState_BATTLE_OF_MAYA();
			break;
		case KANTURU_STATE_BATTLE_OF_NIGHTMARE:
			this->SetState_BATTLE_OF_NIGHTMARE();
			break;
		case KANTURU_STATE_TOWER_OF_REFINEMENT:
			this->SetState_TOWER_OF_REFINEMENT();
			break;
		case KANTURU_STATE_END:
			this->SetState_END();
			break;
	}

	#endif
}

void CKanturu::SetState_NONE() // OK
{
	#if(GAMESERVER_UPDATE>=201)

	this->SetKanturuState(KANTURU_STATE_NONE);

	#endif
}

void CKanturu::SetState_BATTLE_STANDBY() // OK
{
	#if(GAMESERVER_UPDATE>=201)

	LogAdd(LOG_BLACK,"[Kanturu] SetState STAND");

	gKanturuBattleUserMng.ResetAllData();

	this->SetKanturuState(KANTURU_STATE_BATTLE_STANDBY);

	this->m_BattleStanby.SetState(KANTURU_STANBY_START);

	#endif
}

void CKanturu::SetState_BATTLE_OF_MAYA() // OK
{
	#if(GAMESERVER_UPDATE>=201)

	this->SetKanturuState(KANTURU_STATE_BATTLE_OF_MAYA);

	this->m_BattleOfMaya.SetState(KANTURU_MAYA_STANDBY_1);

	#endif
}

void CKanturu::SetState_BATTLE_OF_NIGHTMARE() // OK
{
	#if(GAMESERVER_UPDATE>=201)

	this->SetKanturuState(KANTURU_STATE_BATTLE_OF_NIGHTMARE);

	this->m_BattleOfNightmare.SetState(KANTURU_NIGHTMARE_IDLE);

	#endif
}

void CKanturu::SetState_TOWER_OF_REFINEMENT() // OK
{
	#if(GAMESERVER_UPDATE>=201)

	this->SetKanturuState(KANTURU_STATE_TOWER_OF_REFINEMENT);

	this->m_TowerOfRefinement.SetState(KANTURU_TOWER_OF_REFINEMENT_REVITALIZATION);

	this->SetKanturuMapAttr(1);

	#endif
}

void CKanturu::SetState_END() // OK
{
	#if(GAMESERVER_UPDATE>=201)

	LogAdd(LOG_BLACK,"[Kanturu] SetState END");

	this->SetKanturuState(KANTURU_STATE_END);

	gKanturuBattleUserMng.ResetAllData();

	#endif
}

void CKanturu::CheckStateTime() // OK
{
	#if(GAMESERVER_UPDATE>=201)

	if(this->GetKanturuState() != KANTURU_STATE_NONE && this->m_StateInfo[this->GetKanturuState()].GetCondition() == 1 && this->m_StateInfo[this->GetKanturuState()].IsTimeOut() != 0)
	{
		this->SetState(((this->GetKanturuState()==KANTURU_STATE_END)?KANTURU_STATE_NONE:(this->GetKanturuState()+1)));
	}

	#endif
}

bool CKanturu::CheckEnterKanturu(int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=201)

	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return 0;
	}

	if(lpObj->Map != MAP_KANTURU2)
	{
		gKanturuUtil.NotifyKanturuEnter(aIndex,4);
		return 0;
	}

	if(this->GetKanturuState() != KANTURU_STATE_BATTLE_OF_MAYA)
	{
		gKanturuUtil.NotifyKanturuEnter(aIndex,3);
		return 0;
	}

	if(this->m_BattleOfMaya.GetEntrancePermit() == 0)
	{
		gKanturuUtil.NotifyKanturuEnter(aIndex,3);
		return 0;
	}

	if(gKanturuBattleUserMng.IsOverMaxUser() != 0)
	{
		gKanturuUtil.NotifyKanturuEnter(aIndex,1);
		return 0;
	}

	if(this->CheckEquipmentMoonStone(aIndex) == 0)
	{
		gKanturuUtil.NotifyKanturuEnter(aIndex,2);
		return 0;
	}

	if(lpObj->Inventory[INVENTORY_SLOT_HELPER].m_Index == GET_ITEM(13,2))
	{
		gKanturuUtil.NotifyKanturuEnter(aIndex,5);
		return 0;
	}

	if(lpObj->Change >= 0)
	{
		gKanturuUtil.NotifyKanturuEnter(aIndex,6);
		return 0;
	}

	if(lpObj->Inventory[INVENTORY_SLOT_WING].IsItem() == 0 && lpObj->Inventory[INVENTORY_SLOT_HELPER].m_Index != GET_ITEM(13,3) && lpObj->Inventory[INVENTORY_SLOT_HELPER].m_Index != GET_ITEM(13,37))
	{
		gKanturuUtil.NotifyKanturuEnter(aIndex,7);
		return 0;
	}

	return 1;

	#else

	return 0;

	#endif
}

bool CKanturu::CheckEnterKanturuRefinery(int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=201)

	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return 0;
	}

	if(lpObj->Map != MAP_KANTURU2)
	{
		gKanturuUtil.NotifyKanturuEnter(aIndex,4);
		return 0;
	}

	if(this->GetKanturuState() != KANTURU_STATE_TOWER_OF_REFINEMENT)
	{
		gKanturuUtil.NotifyKanturuEnter(aIndex,3);
		return 0;
	}

	if(this->m_TowerOfRefinement.GetEntrancePermit() == 0)
	{
		gKanturuUtil.NotifyKanturuEnter(aIndex,3);
		return 0;
	}

	return 1;

	#else

	return 0;

	#endif
}

bool CKanturu::CheckCanEnterKanturuBattle() // OK
{
	#if(GAMESERVER_UPDATE>=201)

	if(this->GetKanturuState() == KANTURU_STATE_BATTLE_OF_MAYA && this->m_BattleOfMaya.GetEntrancePermit() != 0)
	{
		return 1;
	}

	if(this->GetKanturuState() == KANTURU_STATE_TOWER_OF_REFINEMENT && this->m_TowerOfRefinement.GetEntrancePermit() != 0 && this->m_TowerOfRefinement.GetUseTowerOfRefinement() != 0)
	{
		return 1;
	}

	#endif

	return 0;
}

bool CKanturu::CheckEquipmentMoonStone(int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=201)

	LPOBJ lpObj = &gObj[aIndex];

	if(this->GetEnableCheckMoonStone() == 0)
	{
		return 1;
	}

	if(lpObj->Inventory[INVENTORY_SLOT_RING1].IsItem() != 0 && lpObj->Inventory[INVENTORY_SLOT_RING1].m_Index == GET_ITEM(13,38) && lpObj->Inventory[INVENTORY_SLOT_RING1].m_Durability > 0)
	{
		return 1;
	}

	if(lpObj->Inventory[INVENTORY_SLOT_RING2].IsItem() != 0 && lpObj->Inventory[INVENTORY_SLOT_RING2].m_Index == GET_ITEM(13,38) && lpObj->Inventory[INVENTORY_SLOT_RING2].m_Durability > 0)
	{
		return 1;
	}

	#endif

	return 0;
}

void CKanturu::KanturuMonsterDieProc(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	#if(GAMESERVER_UPDATE>=201)

	int aIndex = gObjMonsterGetTopHitDamageUser(lpObj);

	if(OBJECT_RANGE(aIndex) != 0)
	{
		lpTarget = &gObj[aIndex];
	}

	gKanturuMonsterMng.MonsterDie(lpObj->Index);

	#endif
}

void CKanturu::LoadKanturuMapAttr(char* path,int level) // OK
{
	#if(GAMESERVER_UPDATE>=201)

	if(level < 0 || level >= MAX_KANTURU_MAP_LEVEL)
	{
		return;
	}

	this->m_KanturuMap[level].Load(path,MAP_KANTURU3);

	#endif
}

void CKanturu::SetKanturuMapAttr(int level) // OK
{
	#if(GAMESERVER_UPDATE>=201)

	if(level < 0 || level >= MAX_KANTURU_MAP_LEVEL)
	{
		return;
	}

	memcpy(gMap[MAP_KANTURU3].m_MapAttr,this->m_KanturuMap[level].m_MapAttr,(this->m_KanturuMap[level].m_width*this->m_KanturuMap[level].m_height));

	#endif
}

void CKanturu::SetKanturuEnable(int enable) // OK
{
	#if(GAMESERVER_UPDATE>=201)

	this->m_KanturuEnable = enable;

	#endif
}

void CKanturu::SetKanturuState(int state) // OK
{
	#if(GAMESERVER_UPDATE>=201)

	this->m_KanturuState = state;

	#endif
}

void CKanturu::SetEnableCheckMoonStone(int enable) // OK
{
	#if(GAMESERVER_UPDATE>=201)

	this->m_EnableCheckMoonStone = enable;

	#endif
}

int CKanturu::GetKanturuEnable() // OK
{
	#if(GAMESERVER_UPDATE>=201)

	return this->m_KanturuEnable;

	#else 

	return 0;

	#endif
}

int CKanturu::GetKanturuState() // OK
{
	#if(GAMESERVER_UPDATE>=201)

	return this->m_KanturuState;

	#else 

	return 0;

	#endif
}

int CKanturu::GetKanturuDetailState() // OK
{
	#if(GAMESERVER_UPDATE>=201)

	switch(this->GetKanturuState())
	{
		case KANTURU_STATE_BATTLE_STANDBY:
			return this->m_BattleStanby.GetBattleStanbyState();
			break;
		case KANTURU_STATE_BATTLE_OF_MAYA:
			return this->m_BattleOfMaya.GetBattleOfMayaState();
			break;
		case KANTURU_STATE_BATTLE_OF_NIGHTMARE:
			return this->m_BattleOfNightmare.GetBattleOfNightmareState();
			break;
		case KANTURU_STATE_TOWER_OF_REFINEMENT:
			return this->m_TowerOfRefinement.GetTowerOfRefinementState();
			break;
	}

	#endif

	return KANTURU_STATE_NONE;
}

int CKanturu::GetRemainTime() // OK
{
	#if(GAMESERVER_UPDATE>=201)

	switch(this->GetKanturuState())
	{
		case KANTURU_STATE_BATTLE_STANDBY:
			return this->m_BattleStanby.GetRemainTime();
			break;
		case KANTURU_STATE_BATTLE_OF_MAYA:
			return this->m_BattleOfMaya.GetRemainTime();
			break;
		case KANTURU_STATE_BATTLE_OF_NIGHTMARE:
			return this->m_BattleOfNightmare.GetRemainTime();
			break;
		case KANTURU_STATE_TOWER_OF_REFINEMENT:
			return this->m_TowerOfRefinement.GetRemainTime();
			break;
	}

	#endif

	return 0;
}

int CKanturu::GetEnableCheckMoonStone() // OK
{
	#if(GAMESERVER_UPDATE>=201)

	return this->m_EnableCheckMoonStone;

	#else

	return 0;

	#endif
}

bool CKanturu::CheckInEvent() // OK
{
	#if(GAMESERVER_UPDATE>=201)

	switch(this->GetKanturuState())
	{
		case KANTURU_STATE_BATTLE_OF_MAYA:
		{
			switch (this->GetKanturuDetailState())
			{
				case KANTURU_MAYA_START_1:
				case KANTURU_MAYA_1:
				case KANTURU_MAYA_START_2:
				case KANTURU_MAYA_2:
				case KANTURU_MAYA_START_3:
				case KANTURU_MAYA_3:
					return 1;
			}
		}
		break;
		case KANTURU_STATE_BATTLE_OF_NIGHTMARE:
		{
			if(this->GetKanturuDetailState() == KANTURU_NIGHTMARE_START)
			{
				return 1;
			}
		}
		break;
	}

	#endif

	return 0;
}
