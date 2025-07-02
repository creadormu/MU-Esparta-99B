// MonsterSkillManager.cpp: implementation of the CMonsterSkillManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MonsterSkillManager.h"
#include "Attack.h"
#include "Map.h"
#include "MemScript.h"
#include "MonsterSkillUnit.h"
#include "Protocol.h"
#include "SkillHitBox.h"
#include "SkillManager.h"
#include "Util.h"
#include "Viewport.h"

CMonsterSkillManager gMonsterSkillManager;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMonsterSkillManager::CMonsterSkillManager() // OK
{
	this->DelAllSkillManagerInfo();
}

CMonsterSkillManager::~CMonsterSkillManager() // OK
{

}

void CMonsterSkillManager::Load(char* path) // OK
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

	this->DelAllSkillManagerInfo();

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

					CMonsterSkillInfo info;

					info.MonsterClass = lpMemScript->GetNumber();

					lpMemScript->GetToken();

					info.m_SkillUnitCount = 0;

					for(int n=0;n < MAX_MONSTER_SKILL_UNIT;n++)
					{
						info.m_SkillUnitTypeArray[n] = lpMemScript->GetAsNumber();

						info.m_SkillUnitArray[n] = gMonsterSkillUnit.FindSkillUnitInfo(lpMemScript->GetAsNumber());

						info.m_SkillUnitCount = ((info.m_SkillUnitArray[n]==0)?info.m_SkillUnitCount:(info.m_SkillUnitCount+1));
					}

					if(info.MonsterClass >= 0 && info.MonsterClass < MAX_MONSTER_SKILL_INFO)
					{
						this->m_MonsterSkillInfoArray[info.MonsterClass] = info;
					}
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

	this->m_DataLoad = 1;

	delete lpMemScript;
}

void CMonsterSkillManager::DelAllSkillManagerInfo() // OK
{
	this->m_DataLoad = 0;

	for(int n=0;n < MAX_MONSTER_SKILL_INFO;n++)
	{
		this->m_MonsterSkillInfoArray[n].Reset();
	}
}

bool CMonsterSkillManager::CheckMonsterSkill(int MonsterClass,int MonsterSkillUnitType) // OK
{
	if(MonsterClass < 0 || MonsterClass >= MAX_MONSTER_SKILL_INFO)
	{
		return 0;
	}

	if(this->m_MonsterSkillInfoArray[MonsterClass].IsValid() == 0)
	{
		return 0;
	}

	for(int n=0;n < MAX_MONSTER_SKILL_UNIT;n++)
	{
		if(this->m_MonsterSkillInfoArray[MonsterClass].m_SkillUnitTypeArray[n] == MonsterSkillUnitType)
		{
			return 1;
		}
	}

	return 0;
}

CMonsterSkillUnitInfo* CMonsterSkillManager::FindMonsterSkillUnitInfo(int aIndex,int MonsterSkillUnitType) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(this->m_MonsterSkillInfoArray[lpObj->Class].IsValid() == 0)
	{
		return 0;
	}

	int MonsterSkillUnitTypeTable[MAX_MONSTER_SKILL_UNIT];
	int MonsterSkillUnitTypeCount = 0;

	for(int n=0;n < MAX_MONSTER_SKILL_UNIT;n++)
	{
		if(this->m_MonsterSkillInfoArray[lpObj->Class].m_SkillUnitTypeArray[n] == MonsterSkillUnitType)
		{
			MonsterSkillUnitTypeTable[MonsterSkillUnitTypeCount++] = n;
		}
	}

	return ((MonsterSkillUnitTypeCount==0)?0:this->m_MonsterSkillInfoArray[lpObj->Class].m_SkillUnitArray[MonsterSkillUnitTypeTable[GetLargeRand()%MonsterSkillUnitTypeCount]]);
}

void CMonsterSkillManager::UseMonsterSkill(int aIndex,int bIndex,int MonsterSkillUnitType,int MonsterSkillUnit,CSkill* lpSkill) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(this->m_MonsterSkillInfoArray[lpObj->Class].IsValid() == 0)
	{
		return;
	}

	CMonsterSkillUnitInfo* lpMonsterSkillUnitInfo = ((MonsterSkillUnit==-1)?this->FindMonsterSkillUnitInfo(aIndex,MonsterSkillUnitType):this->m_MonsterSkillInfoArray[lpObj->Class].m_SkillUnitArray[MonsterSkillUnit]);

	if(lpMonsterSkillUnitInfo == 0)
	{
		return;
	}

	switch(lpMonsterSkillUnitInfo->m_UnitTargetType)
	{
		case MSU_TARGET_TYPE_COMMON:
			bIndex = bIndex;
			break;
		case MSU_TARGET_TYPE_SELF:
			bIndex = aIndex;
			break;
	}

	GCMonsterSkillSend(lpObj,&gObj[bIndex],lpMonsterSkillUnitInfo->m_UnitNumber);

	if(lpMonsterSkillUnitInfo->m_UnitScopeType == MSU_SCOPE_TYPE_NONE)
	{
		lpMonsterSkillUnitInfo->RunSkill(aIndex,bIndex);
		this->SpecialMonsterSkillAttack(lpObj,&gObj[bIndex],lpSkill,lpMonsterSkillUnitInfo);
	}
	else
	{
		for(int n=0;n < MAX_VIEWPORT;n++)
		{
			if(lpObj->VpPlayer2[n].state == VIEWPORT_NONE)
			{
				continue;
			}

			int index = lpObj->VpPlayer2[n].index;

			if(gObj[index].Type != OBJECT_USER || gObj[index].Live == 0 || gObj[index].State != OBJECT_PLAYING)
			{
				continue;
			}

			switch(lpMonsterSkillUnitInfo->m_UnitScopeType)
			{
				case MSU_SCOPE_TYPE_AREA:
					if(gObjCalcDistance(lpObj,&gObj[index]) < lpMonsterSkillUnitInfo->m_UnitScopeValue)
					{
						lpMonsterSkillUnitInfo->RunSkill(aIndex,index);
						this->SpecialMonsterSkillAttack(lpObj,&gObj[index],lpSkill,lpMonsterSkillUnitInfo);
					}
					break;
				case MSU_SCOPE_TYPE_HITBOX:
					if(gSkillHitBox.HitCheck(gSkillManager.GetSkillAngle(lpObj->X,lpObj->Y,gObj[bIndex].X,gObj[bIndex].Y),lpObj->X,lpObj->Y,gObj[index].X,gObj[index].Y) != 0)
					{
						lpMonsterSkillUnitInfo->RunSkill(aIndex,index);
						this->SpecialMonsterSkillAttack(lpObj,&gObj[index],lpSkill,lpMonsterSkillUnitInfo);
					}
					break;
			}
		}
	}
}

void CMonsterSkillManager::SpecialMonsterSkillAttack(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill,CMonsterSkillUnitInfo* lpMonsterSkillUnitInfo) // OK
{
	if(lpMonsterSkillUnitInfo->m_UnitTargetType != MSU_TARGET_TYPE_COMMON)
	{
		return;
	}

	if((lpObj->Class >= 454 && lpObj->Class <= 459) || (lpObj->Class >= 504 && lpObj->Class <= 521)|| lpObj->Class == 562 || lpObj->Class == 563 || lpObj->Class == 565)
	{
		gAttack.Attack(lpObj,lpTarget,lpSkill,0,0,0,0,0);
	}
}
