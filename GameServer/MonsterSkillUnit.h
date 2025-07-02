// MonsterSkillUnit.h: interface for the CMonsterSkillUnit class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "MonsterSkillUnitInfo.h"

#define MAX_MONSTER_SKILL_UNIT_INFO_ARRAY 200

class CMonsterSkillUnit
{
public:
	CMonsterSkillUnit();
	virtual ~CMonsterSkillUnit();
	void Load(char* path);
	void DelAllSkillUnit();
	CMonsterSkillUnitInfo* FindSkillUnitInfo(int UnitNumber);
public:
	bool m_DataLoad;
	CMonsterSkillUnitInfo m_MonsterSkillUnitInfoArray[MAX_MONSTER_SKILL_UNIT_INFO_ARRAY];
};

extern CMonsterSkillUnit gMonsterSkillUnit;
