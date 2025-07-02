// ExcellentOptionRate.h: interface for the CExcellentOptionRate class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

struct EXCELLENT_ITEM_OPTION_INFO
{
	int Index;
	int ItemMinIndex;
	int ItemMaxIndex;
	char Name[32];
	int Rate;
};

class CExcellentOptionRate
{
public:
	CExcellentOptionRate();
	virtual ~CExcellentOptionRate();
	void Load(char* path);
	int GetNewOption(int ItemIndex);
private:
	std::vector<EXCELLENT_ITEM_OPTION_INFO> m_ExcellentItemOptionInfo;
};

extern CExcellentOptionRate gExcellentOptionRate;