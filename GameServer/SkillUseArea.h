// SkillUseArea.h: interface for the CSkillUseArea class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "User.h"

struct SKILL_USE_INFO
{
	int Index;
};

struct SKILL_USE_AREA_INFO
{
	int Map;
	int X;
	int Y;
	int TX;
	int TY;
};

class CSkillUseArea
{
public:
	CSkillUseArea();
	virtual ~CSkillUseArea();
	void Load(char* path);
	bool CheckUseSkill(LPOBJ lpObj,int Index);
private:
	std::map<int,SKILL_USE_INFO> m_SkillUseInfo;
	std::vector<SKILL_USE_AREA_INFO> m_SkillUseAreaInfo;
};

extern CSkillUseArea gSkillUseArea;