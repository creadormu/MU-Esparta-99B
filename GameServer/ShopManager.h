// ShopManager.h: interface for the CShopManager class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Shop.h"

#define MAX_SHOP 100

#define SHOP_RANGE(x) (((x)<0)?0:((x)>=MAX_SHOP)?0:1)

struct SHOP_MANAGER_INFO
{
	int Index;
	int MonsterClass;
	int MapNumber;
	int PosX;
	int PosY;
	int Dir;
	int Enable[MAX_ACCOUNT_LEVEL];
	int GameMasterLevel;
	int CoinType;
	CShop Shop;
};

class CShopManager
{
public:
	CShopManager();
	virtual ~CShopManager();
	void Load(char* path);
	void ReloadShop();
	void ReloadShopInterface();
	short GetShopNumber(int MonsterClass,int MapNumber,int PosX,int PosY);
	bool GetItemByIndex(int index,CItem* lpItem,int slot,int* type);
	long GetItemCountByIndex(int index);
	bool CheckShopAccountLevel(LPOBJ lpNpc,LPOBJ lpObj);
	bool CheckShopGameMasterLevel(LPOBJ lpNpc,LPOBJ lpObj);
	bool GCShopItemListSendByIndex(int index,int aIndex);
	bool GCShopItemPriceSendByIndex(int index,int aIndex);
public:
	std::vector<SHOP_MANAGER_INFO> m_ShopManagerInfo;
};

extern CShopManager gShopManager;