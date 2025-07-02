// CustomStore.h: interface for the CCustomStore class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "PersonalShop.h"
#include "Map.h"
#include "User.h"

class CCustomStore
{
public:
	CCustomStore();
	virtual ~CCustomStore();
	void ReadCustomStoreInfo(char* section,char* path);
	bool CommandCustomStore(LPOBJ lpObj,char* arg);
	bool CommandCustomStoreOffline(LPOBJ lpObj);
	bool OpenCustomStore(LPOBJ lpObj,int type);
	bool OnPShopOpen(LPOBJ lpObj);
	void OnPShopClose(LPOBJ lpObj);
	void OnPShopSecondProc(LPOBJ lpObj);
	void OnPShopAlreadyConnected(LPOBJ lpObj);
	void OnPShopItemList(LPOBJ lpObj,LPOBJ lpTarget);
	bool OnPShopBuyItemRecv(PMSG_PSHOP_BUY_ITEM_RECV* lpMsg,int aIndex);
	static void OnPShopBuyItemCallbackRecv(LPOBJ lpObj,LPOBJ lpTarget,DWORD slot,DWORD Coin1,DWORD Coin2,DWORD Coin3);
public:
	int m_CustomStoreSwitch;
	int m_CustomStoreMapZone;
	int m_CustomStoreMapList[2][MAX_MAP];
	char m_CustomStoreCommandJoBSyntax[32]; //bless
	char m_CustomStoreCommandJoSSyntax[32]; //soul
	char m_CustomStoreCommandJoLSyntax[32]; //Life
	char m_CustomStoreCommandJoCSyntax[32]; //Chaos
	char m_CustomStoreCommandJCRSyntax[32]; //creation
	char m_CustomStoreCommandJoGSyntax[32]; //guardian
	char m_CustomStoreCommandJoHSyntax[32]; //harmony
	char m_CustomStoreCommandCoin1Syntax[32];
	char m_CustomStoreCommandCoin2Syntax[32];
	char m_CustomStoreCommandCoin3Syntax[32];
	int m_CustomStoreOfflineSwitch;
	int m_CustomStoreOfflineCoinGain;
	int m_CustomStoreOfflineMapZone;
};

extern CCustomStore gCustomStore;
