// DamageTable.cpp: interface for the CDamageTable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DamageTable.h"
#include "Attack.h"
#include "MemScript.h"
#include "Util.h"

CDamageTable gDamageTable;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDamageTable::CDamageTable() // OK
{
	this->m_MonsterDamageInfo.clear();

	this->m_PlayerDamageInfo.clear();
}

CDamageTable::~CDamageTable() // OK
{

}

void CDamageTable::Load(char* path) // OK
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

	this->m_MonsterDamageInfo.clear();

	this->m_PlayerDamageInfo.clear();

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

					MONSTER_DAMAGE_TABLE info;

					info.MapNumber = lpMemScript->GetNumber();

					info.MinLevel = lpMemScript->GetAsNumber();

					info.MaxLevel = lpMemScript->GetAsNumber();

					info.MinReset = lpMemScript->GetAsNumber();

					info.MaxReset = lpMemScript->GetAsNumber();

					info.MinDefense = lpMemScript->GetAsNumber();

					info.MaxDefense = lpMemScript->GetAsNumber();

					info.DamageRate = lpMemScript->GetAsNumber();

					this->m_MonsterDamageInfo.push_back(info);
				}
				else if(section == 1)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					PLAYER_DAMAGE_TABLE info;

					info.MapNumber = lpMemScript->GetNumber();

					info.MinLevel = lpMemScript->GetAsNumber();

					info.MaxLevel = lpMemScript->GetAsNumber();

					info.MinReset = lpMemScript->GetAsNumber();

					info.MaxReset = lpMemScript->GetAsNumber();

					info.MonsterIndex = lpMemScript->GetAsNumber();

					info.MonsterMinLevel = lpMemScript->GetAsNumber();

					info.MonsterMaxLevel = lpMemScript->GetAsNumber();

					info.DamageRate = lpMemScript->GetAsNumber();

					this->m_PlayerDamageInfo.push_back(info);
				}
				else
				{
					break;
				}
			}
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

int CDamageTable::GetMonsterDamage(LPOBJ lpObj) // OK
{
	DWORD defense = 0;

	gAttack.GetPreviewDefense(lpObj,&defense);

	for(std::vector<MONSTER_DAMAGE_TABLE>::iterator it= this->m_MonsterDamageInfo.begin();it != this->m_MonsterDamageInfo.end();it++)
	{
		if(it->MapNumber != -1 && it->MapNumber != lpObj->Map)
		{
			continue;
		}

		if(it->MinLevel != -1 && it->MinLevel > lpObj->Level)
		{
			continue;
		}

		if(it->MaxLevel != -1 && it->MaxLevel < lpObj->Level)
		{
			continue;
		}

		if(it->MinReset != -1 && it->MinReset > lpObj->Reset)
		{
			continue;
		}

		if(it->MaxReset != -1 && it->MaxReset < lpObj->Reset)
		{
			continue;
		}

		if(it->MinDefense != -1 && it->MinDefense > (int)defense)
		{
			continue;
		}

		if(it->MaxDefense != -1 && it->MaxDefense < (int)defense)
		{
			continue;
		}

		return it->DamageRate;
	}

	return 100;
}

int CDamageTable::GetPlayerDamage(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	for(std::vector<PLAYER_DAMAGE_TABLE>::iterator it= this->m_PlayerDamageInfo.begin();it != this->m_PlayerDamageInfo.end();it++)
	{
		if(it->MapNumber != -1 && it->MapNumber != lpObj->Map)
		{
			continue;
		}

		if(it->MonsterIndex != -1 && it->MonsterIndex != lpTarget->Class)
		{
			continue;
		}

		if(it->MinLevel != -1 && it->MinLevel > lpObj->Level)
		{
			continue;
		}

		if(it->MaxLevel != -1 && it->MaxLevel < lpObj->Level)
		{
			continue;
		}

		if(it->MinReset != -1 && it->MinReset > lpObj->Reset)
		{
			continue;
		}

		if(it->MaxReset != -1 && it->MaxReset < lpObj->Reset)
		{
			continue;
		}

		if(it->MonsterMinLevel != -1 && it->MonsterMinLevel > lpTarget->Level)
		{
			continue;
		}

		if(it->MonsterMaxLevel != -1 && it->MonsterMaxLevel < lpTarget->Level)
		{
			continue;
		}

		return it->DamageRate;
	}

	return 100;
}