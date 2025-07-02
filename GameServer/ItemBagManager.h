// ItemBagManager.h: interface for the CItemBagManager class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "ItemBag.h"
#include "ItemBagEx.h"
#include "User.h"

enum eItemBagSpecialValue
{
	ITEM_BAG_SPECIAL_CHAOS_BOX = 0,
	ITEM_BAG_SPECIAL_DEVIL_SQUARE = 1,
	ITEM_BAG_SPECIAL_BLOOD_CASTLE = 2,
	ITEM_BAG_SPECIAL_CHAOS_CASTLE = 3,
	ITEM_BAG_SPECIAL_SENIOR_MIX = 4,
	ITEM_BAG_SPECIAL_CRYWOLF = 5,
	ITEM_BAG_SPECIAL_LEO_HELPER = 6,
	ITEM_BAG_SPECIAL_CHAOS_CARD_MIX = 7,
	ITEM_BAG_SPECIAL_ILLUSION_TEMPLE = 8,
	ITEM_BAG_SPECIAL_CHERRY_BLOSSOM_MIX = 9,
	ITEM_BAG_SPECIAL_SANTA_CLAUS = 10,
	ITEM_BAG_SPECIAL_LUCKY_COIN = 11,
	ITEM_BAG_SPECIAL_DOUBLE_GOER = 12,
	ITEM_BAG_SPECIAL_IMPERIAL_GUARDIAN = 13,
	ITEM_BAG_SPECIAL_ARCA_BATTLE_MIX = 14,
	ITEM_BAG_SPECIAL_ACHERON_GUARDIAN = 15,
	ITEM_BAG_SPECIAL_MU_RUMMY = 16,
	ITEM_BAG_SPECIAL_CHAOS_CASTLE_FINAL = 17,
	ITEM_BAG_SPECIAL_CUSTOM_ARENA = 20,
	ITEM_BAG_SPECIAL_CUSTOM_LUKE_HELPER = 21,
};

struct ITEM_BAG_MANAGER_INFO
{
	int ItemIndex;
	int ItemLevel;
	int MonsterClass;
	int SpecialValue;
	int SpecialLevel;
	int SpecialState;
	char BagPath[MAX_PATH];
	CItemBag ItemBag;
};

class CItemBagManager
{
public:
	CItemBagManager();
	virtual ~CItemBagManager();
	void Load(char* path);
	void LoadEventItemBag();
	bool GetItemBySpecialValue(int SpecialValue,int SpecialLevel,int SpecialState,LPOBJ lpObj,CItem* lpItem);
	bool DropItemByItemIndex(int ItemIndex,int ItemLevel,LPOBJ lpObj,int map,int x,int y);
	bool DropItemByMonsterClass(int MonsterClass,LPOBJ lpObj,int map,int x,int y);
	bool DropItemBySpecialValue(int SpecialValue,int SpecialLevel,int SpecialState,LPOBJ lpObj,int map,int x,int y);
private:
	std::vector<ITEM_BAG_MANAGER_INFO> m_ItemBagManagerInfo;
};

extern CItemBagManager gItemBagManager;