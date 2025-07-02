// ItemBagEx.h: interface for the CItemBagEx class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "DefaultClassInfo.h"
#include "Item.h"
#include "User.h"

struct ITEM_BAG_EX_ITEM_INFO
{
	int ItemIndex;
	int Level;
	int Grade;
	int Option0;
	int Option1;
	int Option2;
	int Option3;
	int Option4;
	int Option5;
	int Option6;
	int Duration;
};

struct ITEM_BAG_EX_DROP_INFO
{
	int Index;
	int Section;
	int SectionRate;
	int SetDropRate;
	int MinDropCount;
	int MaxDropCount;
	int RequireClass[MAX_CLASS];
};

struct ITEM_BAG_EX_INFO
{
	int Index;
	int DropRate;
	int MapNumber;
	std::vector<ITEM_BAG_EX_DROP_INFO> DropInfo;
};

class CItemBagEx
{
public:
	CItemBagEx();
	void Load(char* path);
	bool GetItem(LPOBJ lpObj,CItem* lpItem);
	bool DropItem(LPOBJ lpObj,int map,int x,int y);
public:
	std::map<int,ITEM_BAG_EX_INFO> m_ItemBagInfo;
	std::map<int,std::vector<ITEM_BAG_EX_ITEM_INFO>> m_ItemBagItemInfo;
	int m_ItemRate;
	int m_MoneyDrop;
	int m_BagUseEffect;
	int m_RewardType;
	int m_RewardCoin1;
	int m_RewardCoin2;
	int m_RewardCoin3;
};