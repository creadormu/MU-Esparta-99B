// Kanturu.h: interface for the CKanturu class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "KanturuBattleOfMaya.h"
#include "KanturuBattleOfNightmare.h"
#include "KanturuBattleStanby.h"
#include "KanturuStateInfo.h"
#include "KanturuTowerOfRefinement.h"
#include "Map.h"

#define MAX_KANTURU_MAP_LEVEL 2

enum eKanturuState
{
	KANTURU_STATE_NONE = 0,
	KANTURU_STATE_BATTLE_STANDBY = 1,
	KANTURU_STATE_BATTLE_OF_MAYA = 2,
	KANTURU_STATE_BATTLE_OF_NIGHTMARE = 3,
	KANTURU_STATE_TOWER_OF_REFINEMENT = 4,
	KANTURU_STATE_END = 5,
	KANTURU_STATE_MAX
};

class CKanturu
{
public:
	CKanturu();
	virtual ~CKanturu();
	void ResetAllData();
	void Load(char* path);
	void MainProc();
	void ProcState_NONE();
	void ProcState_BATTLE_STANDBY();
	void ProcState_BATTLE_OF_MAYA();
	void ProcState_BATTLE_OF_NIGHTMARE();
	void ProcState_TOWER_OF_REFINEMENT();
	void ProcState_END();
	void SetState(int state);
	void SetState_NONE();
	void SetState_BATTLE_STANDBY();
	void SetState_BATTLE_OF_MAYA();
	void SetState_BATTLE_OF_NIGHTMARE();
	void SetState_TOWER_OF_REFINEMENT();
	void SetState_END();
	void CheckStateTime();
	bool CheckEnterKanturu(int aIndex);
	bool CheckEnterKanturuRefinery(int aIndex);
	bool CheckCanEnterKanturuBattle();
	bool CheckEquipmentMoonStone(int aIndex);
	void KanturuMonsterDieProc(LPOBJ lpObj,LPOBJ lpTarget);
	void LoadKanturuMapAttr(char* path,int level);
	void SetKanturuMapAttr(int level);
	void SetKanturuEnable(int enable);
	void SetKanturuState(int state);
	void SetEnableCheckMoonStone(int enable);
	int GetKanturuEnable();
	int GetKanturuState();
	int GetKanturuDetailState();
	int GetRemainTime();
	int GetEnableCheckMoonStone();
	bool CheckInEvent();
private:
	int m_KanturuEnable;
	int m_KanturuState;
	int m_IsSucccess;
	CKanturuStateInfo m_StateInfo[KANTURU_STATE_MAX];
	int m_StateInfoCount;
	CKanturuBattleStanby m_BattleStanby;
	CKanturuBattleOfMaya m_BattleOfMaya;
	CKanturuBattleOfNightmare m_BattleOfNightmare;
	CKanturuTowerOfRefinement m_TowerOfRefinement;
	CMap m_KanturuMap[KANTURU_STATE_MAX];
	int m_EnableCheckMoonStone;
	int m_KanturuBattleCounter;
	int m_KanturuBattleDate;
};

extern CKanturu gKanturu;
