// MonsterAIAutomata.h: interface for the CMonsterAIAutomata class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "MonsterAIAutomataInfo.h"

#define MAX_MONSTER_AI_AUTOMATA_INFO 300

class CMonsterAIAutomata
{
public:
	CMonsterAIAutomata();
	virtual ~CMonsterAIAutomata();
	void Load(char* path);
	void DelAllAutomata();
	CMonsterAIAutomataInfo* FindAutomataInfo(int AutomataNumber);
public:
	bool m_DataLoad;
	CMonsterAIAutomataInfo m_MonsterAIAutomataInfoArray[MAX_MONSTER_AI_AUTOMATA_INFO];
};

extern CMonsterAIAutomata gMonsterAIAutomata;
