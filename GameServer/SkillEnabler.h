// SkillEnabler.h: interface for the CSkillEnabler class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "User.h"

struct SKILL_ENABLER_INFO
{
	int Index;
	int Map;
	int PvM;
	int PvP;
};

class CSkillEnabler
{
public:
	CSkillEnabler();
	virtual ~CSkillEnabler();
	void Load(char* path);
	bool CheckUseSkill(LPOBJ lpObj,LPOBJ lpTarget,int index);
private:
	std::vector<SKILL_ENABLER_INFO> m_SkillEnabler;
};

extern CSkillEnabler gSkillEnabler;