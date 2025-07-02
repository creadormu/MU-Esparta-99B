// JewelOfHarmonyType.h: interface for the CJewelOfHarmonyType class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Item.h"

struct JEWEL_OF_HARMONY_TYPE_INFO
{
	int ItemIndex;
	int Level;
};

class CJewelOfHarmonyType
{
public:
	CJewelOfHarmonyType();
	virtual ~CJewelOfHarmonyType();
	void Load(char* path);
	JEWEL_OF_HARMONY_TYPE_INFO* GetInfo(int index);
	bool CheckJewelOfHarmonyItemType(CItem* lpItem);
private:
	std::map<int,JEWEL_OF_HARMONY_TYPE_INFO> m_JewelOfHarmonyTypeInfo;
};

extern CJewelOfHarmonyType gJewelOfHarmonyType;
