// WingOption.h: interface for the CWingOption class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

struct WING_OPTION_INFO
{
	int ItemIndex;
	int DefenseConstA;
	int IncDamageConstA;
	int IncDamageConstB;
	int DecDamageConstA;
	int DecDamageConstB;
	int MaxNewOption;
};

class CWingOption
{
public:
	CWingOption();
	virtual ~CWingOption();
	void Load(char* path);
	bool CheckWingByItem(int ItemIndex);
	int GetWingDefense(int ItemIndex,int ItemLevel);
	int GetWingIncDamage(int ItemIndex,int ItemLevel);
	int GetWingDecDamage(int ItemIndex,int ItemLevel);
	int GetWingMaxNewOption(int ItemIndex);
private:
	std::map<int,WING_OPTION_INFO> m_WingOptionInfo;
};

extern CWingOption gWingOption;