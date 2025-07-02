// KanturuBattleStanby.h: interface for the CKanturuBattleStanby class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "KanturuStateInfo.h"

enum eKanturuStandbyState
{
	KANTURU_STANBY_NONE = 0,
	KANTURU_STANBY_START = 1,
	KANTURU_STANBY_NOTIFY = 2,
	KANTURU_STANBY_END = 3,
	KANTURU_STANBY_MAX,
};

class CKanturuBattleStanby
{
public:
	CKanturuBattleStanby();
	virtual ~CKanturuBattleStanby();
	void ResetAllData();
	void Load(char* path);
	void MainProc();
	void ProcState_NONE();
	void ProcState_START();
	void ProcState_NOTIFY();
	void ProcState_END();
	void SetState(int state);
	void SetState_NONE();
	void SetState_START();
	void SetState_NOTIFY();
	void SetState_END();
	void CheckStateTime();
	void SetBattleStanbyState(int state);
	void SetSuccessValue(int success);
	int GetBattleStanbyState();
	int GetRemainTime();
private:
	int m_BattleStanbyState;
	int m_IsSucccess;
	CKanturuStateInfo m_StateInfo[KANTURU_STANBY_MAX];
	int m_StateInfoCount;
};
