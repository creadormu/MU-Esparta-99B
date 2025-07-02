// DamageTable.h: interface for the CDamageTable class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "User.h"

struct MONSTER_DAMAGE_TABLE
{
	int MapNumber;
	int MinLevel;
	int MaxLevel;
	int MinReset;
	int MaxReset;
	int MinDefense;
	int MaxDefense;
	int DamageRate;
};

struct PLAYER_DAMAGE_TABLE
{
	int MapNumber;
	int MinLevel;
	int MaxLevel;
	int MinReset;
	int MaxReset;
	int MonsterIndex;
	int MonsterMinLevel;
	int MonsterMaxLevel;
	int DamageRate;
};

class CDamageTable
{
public:
	CDamageTable();
	virtual ~CDamageTable();
	void Load(char* path);
	int GetMonsterDamage(LPOBJ lpObj);
	int GetPlayerDamage(LPOBJ lpObj,LPOBJ lpTarget);
private:
	std::vector<MONSTER_DAMAGE_TABLE> m_MonsterDamageInfo;
	std::vector<PLAYER_DAMAGE_TABLE> m_PlayerDamageInfo;
};

extern CDamageTable gDamageTable;