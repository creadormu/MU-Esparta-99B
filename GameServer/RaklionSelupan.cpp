// RaklionSelupan.cpp: implementation of the CRaklionSelupan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RaklionSelupan.h"
#include "Map.h"
#include "Monster.h"
#include "MonsterSetBase.h"
#include "MonsterSkillManager.h"
#include "Skill.h"
#include "SkillManager.h"
#include "Util.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRaklionSelupan::CRaklionSelupan() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	this->ClearData();

	#endif
}

CRaklionSelupan::~CRaklionSelupan() // OK
{

}

void CRaklionSelupan::ClearData() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	this->m_SelupanObjIndex = -1;
	this->m_BerserkIndex = 0;
	this->m_BossAttackMin = 0;
	this->m_BossAttackMax = 0;

	#endif
}

void CRaklionSelupan::SelupanAct_FirstSkill() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	if(OBJECT_RANGE(this->GetSelupanObjIndex()) == 0)
	{
		return;
	}

	int aIndex = this->GetSelupanObjIndex();

	int bIndex = this->GetSelupanTargetIndex();

	if(OBJECT_RANGE(bIndex) == 0)
	{
		CSkill skill;

		skill.m_level = 0;

		skill.m_skill = SKILL_RAKLION_SELUPAN4;

		skill.m_index = SKILL_RAKLION_SELUPAN4;

		skill.m_DamageMin = 0;

		skill.m_DamageMax = 0;

		gMonsterSkillManager.UseMonsterSkill(aIndex,aIndex,31,0,&skill);
	}
	else
	{
		CSkill skill;

		skill.m_level = 0;

		skill.m_skill = SKILL_RAKLION_SELUPAN4;

		skill.m_index = SKILL_RAKLION_SELUPAN4;

		skill.m_DamageMin = 0;

		skill.m_DamageMax = 0;

		gMonsterSkillManager.UseMonsterSkill(aIndex,bIndex,31,0,&skill);
	}

	#endif
}

void CRaklionSelupan::SelupanAct_PoisonAttack() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	if(OBJECT_RANGE(this->GetSelupanObjIndex()) == 0)
	{
		return;
	}

	int aIndex = this->GetSelupanObjIndex();

	int bIndex = this->GetSelupanTargetIndex();

	if(OBJECT_RANGE(bIndex) != 0)
	{
		CSkill skill;

		skill.m_level = 0;

		skill.m_skill = SKILL_RAKLION_SELUPAN1;

		skill.m_index = SKILL_RAKLION_SELUPAN1;

		skill.m_DamageMin = 0;

		skill.m_DamageMax = 0;

		gMonsterSkillManager.UseMonsterSkill(aIndex,bIndex,1,1,&skill);
	}

	#endif
}

void CRaklionSelupan::SelupanAct_IceStorm() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	if(OBJECT_RANGE(this->GetSelupanObjIndex()) == 0)
	{
		return;
	}

	int aIndex = this->GetSelupanObjIndex();

	int bIndex = this->GetSelupanTargetIndex();

	if(OBJECT_RANGE(bIndex) != 0)
	{
		CSkill skill;

		skill.m_level = 0;

		skill.m_skill = SKILL_RAKLION_SELUPAN2;

		skill.m_index = SKILL_RAKLION_SELUPAN2;

		skill.m_DamageMin = 0;

		skill.m_DamageMax = 0;

		gMonsterSkillManager.UseMonsterSkill(aIndex,bIndex,1,2,&skill);
	}

	#endif
}

void CRaklionSelupan::SelupanAct_IceStrike() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	if(OBJECT_RANGE(this->GetSelupanObjIndex()) == 0)
	{
		return;
	}

	int aIndex = this->GetSelupanObjIndex();

	int bIndex = this->GetSelupanTargetIndex();

	if(OBJECT_RANGE(bIndex) != 0)
	{
		CSkill skill;

		skill.m_level = 0;

		skill.m_skill = SKILL_RAKLION_SELUPAN3;

		skill.m_index = SKILL_RAKLION_SELUPAN3;

		skill.m_DamageMin = 0;

		skill.m_DamageMax = 0;

		gMonsterSkillManager.UseMonsterSkill(aIndex,bIndex,1,3,&skill);
	}

	#endif
}

void CRaklionSelupan::SelupanAct_SummonMonster() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	if(OBJECT_RANGE(this->GetSelupanObjIndex()) == 0)
	{
		return;
	}

	int aIndex = this->GetSelupanObjIndex();

	gMonsterSkillManager.UseMonsterSkill(aIndex,aIndex,30,4,0);

	this->CreateSummonMonster();

	#endif
}

void CRaklionSelupan::SelupanAct_Heal() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	if(OBJECT_RANGE(this->GetSelupanObjIndex()) == 0)
	{
		return;
	}

	int aIndex = this->GetSelupanObjIndex();

	gMonsterSkillManager.UseMonsterSkill(aIndex,aIndex,20,5,0);

	#endif
}

void CRaklionSelupan::SelupanAct_Freeze() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	if(OBJECT_RANGE(this->GetSelupanObjIndex()) == 0)
	{
		return;
	}

	int aIndex = this->GetSelupanObjIndex();

	int bIndex = this->GetSelupanTargetIndex();

	if(OBJECT_RANGE(bIndex) != 0)
	{
		gMonsterSkillManager.UseMonsterSkill(aIndex,bIndex,1,6,0);
	}

	#endif
}

void CRaklionSelupan::SelupanAct_Teleport() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	if(OBJECT_RANGE(this->GetSelupanObjIndex()) == 0)
	{
		return;
	}

	int aIndex = this->GetSelupanObjIndex();

	int bIndex = this->GetSelupanTargetIndex();

	if(OBJECT_RANGE(bIndex) == 0)
	{
		gMonsterSkillManager.UseMonsterSkill(aIndex,aIndex,1,7,0);
	}
	else
	{
		gMonsterSkillManager.UseMonsterSkill(aIndex,bIndex,1,7,0);
	}

	#endif
}

void CRaklionSelupan::SelupanAct_Invincibility() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	if(OBJECT_RANGE(this->GetSelupanObjIndex()) == 0)
	{
		return;
	}

	int aIndex = this->GetSelupanObjIndex();

	gMonsterSkillManager.UseMonsterSkill(aIndex,aIndex,10,8,0);

	#endif
}

void CRaklionSelupan::SelupanAct_BerserkCansel() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	if(OBJECT_RANGE(this->GetSelupanObjIndex()) == 0)
	{
		return;
	}

	int aIndex = this->GetSelupanObjIndex();

	gObj[aIndex].PhysiDamageMin = this->m_BossAttackMin;

	gObj[aIndex].PhysiDamageMax = this->m_BossAttackMax;

	#endif
}

void CRaklionSelupan::SelupanAct_Berserk1() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	if(OBJECT_RANGE(this->GetSelupanObjIndex()) == 0)
	{
		return;
	}

	int aIndex = this->GetSelupanObjIndex();

	gMonsterSkillManager.UseMonsterSkill(aIndex,aIndex,1,9,0);

	this->m_BerserkIndex = 1;

	gObj[aIndex].PhysiDamageMin = this->m_BossAttackMin+(this->m_BerserkIndex*gObj[aIndex].MonsterSkillElementOption.m_SkillElementAttack);

	gObj[aIndex].PhysiDamageMax = this->m_BossAttackMax+(this->m_BerserkIndex*gObj[aIndex].MonsterSkillElementOption.m_SkillElementAttack);

	#endif
}

void CRaklionSelupan::SelupanAct_Berserk2() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	if(OBJECT_RANGE(this->GetSelupanObjIndex()) == 0)
	{
		return;
	}

	int aIndex = this->GetSelupanObjIndex();

	gMonsterSkillManager.UseMonsterSkill(aIndex,aIndex,1,9,0);

	this->m_BerserkIndex = 2;

	gObj[aIndex].PhysiDamageMin = this->m_BossAttackMin+(this->m_BerserkIndex*gObj[aIndex].MonsterSkillElementOption.m_SkillElementAttack);

	gObj[aIndex].PhysiDamageMax = this->m_BossAttackMax+(this->m_BerserkIndex*gObj[aIndex].MonsterSkillElementOption.m_SkillElementAttack);

	#endif
}

void CRaklionSelupan::SelupanAct_Berserk3() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	if(OBJECT_RANGE(this->GetSelupanObjIndex()) == 0)
	{
		return;
	}

	int aIndex = this->GetSelupanObjIndex();

	gMonsterSkillManager.UseMonsterSkill(aIndex,aIndex,1,9,0);

	this->m_BerserkIndex = 3;

	gObj[aIndex].PhysiDamageMin = this->m_BossAttackMin+(this->m_BerserkIndex*gObj[aIndex].MonsterSkillElementOption.m_SkillElementAttack);

	gObj[aIndex].PhysiDamageMax = this->m_BossAttackMax+(this->m_BerserkIndex*gObj[aIndex].MonsterSkillElementOption.m_SkillElementAttack);

	#endif
}

void CRaklionSelupan::SelupanAct_Berserk4() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	if(OBJECT_RANGE(this->GetSelupanObjIndex()) == 0)
	{
		return;
	}

	int aIndex = this->GetSelupanObjIndex();

	gMonsterSkillManager.UseMonsterSkill(aIndex,aIndex,1,9,0);

	this->m_BerserkIndex = 4;

	gObj[aIndex].PhysiDamageMin = this->m_BossAttackMin+(this->m_BerserkIndex*gObj[aIndex].MonsterSkillElementOption.m_SkillElementAttack);

	gObj[aIndex].PhysiDamageMax = this->m_BossAttackMax+(this->m_BerserkIndex*gObj[aIndex].MonsterSkillElementOption.m_SkillElementAttack);

	#endif
}

bool CRaklionSelupan::CreateSelupan() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	for(int n=0;n < gMonsterSetBase.m_count;n++)
	{
		MONSTER_SET_BASE_INFO* lpInfo = &gMonsterSetBase.m_MonsterSetBaseInfo[n];

		if(lpInfo->Type != 4 || lpInfo->MonsterClass != 459 || lpInfo->Map != MAP_RAKLION2)
		{
			continue;
		}

		int index = gObjAddMonster(lpInfo->Map);

		if(OBJECT_RANGE(index) == 0)
		{
			continue;
		}

		if(gObjSetPosMonster(index,n) == 0)
		{
			gObjDel(index);
			continue;
		}

		if(gObjSetMonster(index,lpInfo->MonsterClass) == 0)
		{
			gObjDel(index);
			continue;
		}

		this->SetSelupanObjIndex(index);

		this->m_BossAttackMin = gObj[index].PhysiDamageMin;

		this->m_BossAttackMax = gObj[index].PhysiDamageMax;

		return 1;
	}

	#endif

	return 0;
}

void CRaklionSelupan::DeleteSelupan() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	if(OBJECT_RANGE(this->GetSelupanObjIndex()) != 0)
	{
		gObjDel(this->GetSelupanObjIndex());
		this->SetSelupanObjIndex(-1);
	}

	#endif
}

void CRaklionSelupan::SetSelupanObjIndex(int index) // OK
{
	#if(GAMESERVER_UPDATE>=401)

	this->m_SelupanObjIndex = index;

	#endif
}

void CRaklionSelupan::SetSelupanSkillDelay(int delay) // OK
{
	#if(GAMESERVER_UPDATE>=401)

	if(delay < 0 || delay > 50000)
	{
		return;
	}

	this->m_SelupanSkillDelay = delay;

	#endif
}

void CRaklionSelupan::CreateSummonMonster() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	int index = gObjAddMonster(MAP_RAKLION2);

	if(OBJECT_RANGE(index) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[index];

	int px = gObj[this->GetSelupanObjIndex()].X;
	int py = gObj[this->GetSelupanObjIndex()].Y;

	if(gObjGetRandomFreeLocation(MAP_RAKLION2,&px,&py,2,2,10) == 0)
	{
		px = gObj[this->GetSelupanObjIndex()].X;
		py = gObj[this->GetSelupanObjIndex()].Y;
	}

	lpObj->PosNum = -1;
	lpObj->X = px;
	lpObj->Y = py;
	lpObj->TX = px;
	lpObj->TY = py;
	lpObj->OldX = px;
	lpObj->OldY = py;
	lpObj->StartX = px;
	lpObj->StartY = py;
	lpObj->Dir = 1;
	lpObj->Map = MAP_RAKLION2;

	if(gObjSetMonster(index,(((GetLargeRand()%2)==0)?457:458)) == 0)
	{
		gObjDel(index);
		return;
	}

	#endif
}

void CRaklionSelupan::DeleteSummonMonster() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	for(int n=OBJECT_START_MONSTER;n < MAX_OBJECT_MONSTER;n++)
	{
		if(gObjIsConnected(n) != 0 && gObj[n].Map == MAP_RAKLION2 && (gObj[n].Class == 457 || gObj[n].Class == 458))
		{
			gObjDel(n);
		}
	}

	#endif
}

int CRaklionSelupan::GetSelupanLife() // OK
{
	return ((OBJECT_RANGE(this->GetSelupanObjIndex())==0)?0:(int)gObj[this->GetSelupanObjIndex()].Life);
}

int CRaklionSelupan::GetSelupanMaxLife() // OK
{
	return ((OBJECT_RANGE(this->GetSelupanObjIndex())==0)?0:(int)(gObj[this->GetSelupanObjIndex()].MaxLife+gObj[this->GetSelupanObjIndex()].AddLife));
}

int CRaklionSelupan::GetSelupanObjIndex() // OK
{
	return this->m_SelupanObjIndex;
}

int CRaklionSelupan::GetSelupanTargetIndex() // OK
{
	return ((OBJECT_RANGE(this->GetSelupanObjIndex())==0)?-1:gObj[this->GetSelupanObjIndex()].TargetNumber);
}

int CRaklionSelupan::GetSelupanSkillDelay() // OK
{
	return this->m_SelupanSkillDelay;
}

int CRaklionSelupan::GetBerserkIndex() // OK
{
	return this->m_BerserkIndex;
}
