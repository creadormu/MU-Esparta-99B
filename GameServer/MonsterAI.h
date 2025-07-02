// MonsterAI.h: interface for the CMonsterAI class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "User.h"

class CMonsterAI
{
public:
	CMonsterAI();
	virtual ~CMonsterAI();
	void MonsterAIProc();
	bool RunAI(int aIndex,int MonsterClass);
	void MonsterStateMsgProc(int aIndex);
	void ProcessStateMsg(LPOBJ lpObj,int code,int aIndex,int SubCode);
	bool UpdateCurrentAIUnit(int aIndex);
	void MonsterMoveProc();
	void MonsterMove(int aIndex);
};

extern CMonsterAI gMonsterAI;