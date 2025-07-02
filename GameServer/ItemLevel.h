// ItemLevel.h: interface for the CItemLevel class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

struct ITEM_LEVEL_INFO
{
	int ItemIndex;
	int Level;
	char Name[50];
};

class CItemLevel
{
public:
	CItemLevel();
	virtual ~CItemLevel();
	void Load(char* path);
	bool CheckItemlevel(int index,int level);
public:
	std::vector<ITEM_LEVEL_INFO> m_ItemLevelInfo;
};

extern CItemLevel gItemLevel;