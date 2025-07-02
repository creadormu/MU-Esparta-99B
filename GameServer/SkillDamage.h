// SkillDamage.h: interface for the CSkillDamage class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Skill.h"

struct SKILL_DAMAGE_INFO
{
	int Index;
	int Damage;
};

class CSkillDamage
{
public:
	CSkillDamage();
	virtual ~CSkillDamage();
	void Load(char* path);
	void SetInfo(SKILL_DAMAGE_INFO info);
	int GetDamage(CSkill* lpSkill,int damage);
private:
	std::map<int,SKILL_DAMAGE_INFO> m_SkillDamageInfo;
};

extern CSkillDamage gSkillDamage;