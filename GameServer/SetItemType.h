// SetItemType.h: interface for the CSetItemType class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#define MAX_SET_ITEM_OPTION_INDEX 2

struct SET_ITEM_TYPE_INFO
{
	int Index;
	int StatType;
	int OptionIndex[MAX_SET_ITEM_OPTION_INDEX];
	int DropRate;
};

class CSetItemType
{
public:
	CSetItemType();
	virtual ~CSetItemType();
	void Load(char* path);
	SET_ITEM_TYPE_INFO* GetInfo(int index);
	bool CheckSetItemType(int index);
	int GetSetItemOptionIndex(int index,int number);
	int GetSetItemStatType(int index);
	int GetRandomSetItem();
	void MakeRandomSetItem(int aIndex,int map,int x,int y);
private:
	std::map<int,SET_ITEM_TYPE_INFO> m_SetItemTypeInfo;
};

extern CSetItemType gSetItemType;
