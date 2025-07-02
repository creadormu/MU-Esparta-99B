// KanturuTowerOfRefinement.h: interface for the CKanturuTowerOfRefinement class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "KanturuStateInfo.h"

enum eKanturuTowerOfRefinementState
{
	KANTURU_TOWER_OF_REFINEMENT_NONE = 0,
	KANTURU_TOWER_OF_REFINEMENT_REVITALIZATION = 1,
	KANTURU_TOWER_OF_REFINEMENT_NOTIFY_1 = 2,
	KANTURU_TOWER_OF_REFINEMENT_CLOSE = 3,
	KANTURU_TOWER_OF_REFINEMENT_NOTIFY_2 = 4,
	KANTURU_TOWER_OF_REFINEMENT_END = 5,
	KANTURU_TOWER_OF_REFINEMENT_MAX,
};

class CKanturuTowerOfRefinement
{
public:
	CKanturuTowerOfRefinement();
	virtual ~CKanturuTowerOfRefinement();
	void ResetAllData();
	void Load(char* path);
	void MainProc();
	void SetState(int state);
	void SetState_NONE();
	void SetState_REVITALIZATION();
	void SetState_NOTIFY1();
	void SetState_CLOSE();
	void SetState_NOTIFY2();
	void SetState_END();
	void CheckStateTime();
	void SetTowerOfRefinementState(int state);
	void SetEntrancePermit(int permit);
	void SetUseTowerOfRefinement(int use);
	int GetTowerOfRefinementState();
	int GetEntrancePermit();
	int GetUseTowerOfRefinement();
	int GetRemainTime();
private:
	int m_TowerOfRefinementState;
	int m_EntrancePermit;
	int m_IsUseTowerOfRefinement;
	CKanturuStateInfo m_StateInfo[KANTURU_TOWER_OF_REFINEMENT_MAX];
	int m_StateInfoCount;
};
