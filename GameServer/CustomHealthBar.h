// CustomHealthBar.h: interface for the CCustomHealthBar class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

struct MONSTER_HEALTH_BAR_INFO
{
	int MonsterIndex;
	int MapNumber;
	int BarSwitch;
};

class CCustomHealthBar
{
public:
	void Load(char* path);
	int CheckHealthBar(int MonsterIndex,int MapNumber);
private:
	std::vector<MONSTER_HEALTH_BAR_INFO> m_MonsterHealthBarInfo;
};

extern CCustomHealthBar gCustomHealthBar;