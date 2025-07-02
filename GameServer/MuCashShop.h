// MuCashShop.h: interface for the CMuCashShop class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Protocol.h"
#include "ItemManager.h"

#define MAX_MU_CASH_SHOP_CATEGORY 13
#define MAX_MU_CASH_SHOP_PAGE 15
#define MAX_MU_CASH_SHOP_PAGE_ITEM 9
#define MAX_MU_CASH_SHOP_ITEM (MAX_MU_CASH_SHOP_PAGE*MAX_MU_CASH_SHOP_PAGE_ITEM)

//**********************************************//
//************ Client -> GameServer ************//
//**********************************************//

struct PMSG_MU_CASH_SHOP_OPEN_RECV
{
	#pragma pack(1)
	PSBMSG_HEAD header; // C1:F5:01
	BYTE OpenType;
	BYTE OpenAlready;
	#pragma pack()
};

struct PMSG_MU_CASH_SHOP_ITEM_RECV
{
	#pragma pack(1)
	PSBMSG_HEAD header; // C1:F5:05
	BYTE CategoryIndex;
	BYTE PageIndex;
	BYTE PageLoaded;
	#pragma pack()
};

struct PMSG_MU_CASH_SHOP_BUY_RECV
{
	#pragma pack(1)
	PSBMSG_HEAD header; // C1:F5:07
	DWORD SlotIndex;
	BYTE CategoryIndex;
	#pragma pack()
};

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_MU_CASH_SHOP_OPEN_SEND
{
	#pragma pack(1)
	PSBMSG_HEAD header; // C1:F5:02
	BYTE Result;
	#pragma pack()
};

struct PMSG_MU_CASH_SHOP_POINT_SEND
{
	#pragma pack(1)
	PSBMSG_HEAD header; // C1:F5:04
	DWORD Point;
	#pragma pack()
};

struct PMSG_MU_CASH_SHOP_ITEM_SEND
{
	#pragma pack(1)
	PSBMSG_HEAD header; // C1:F5:06
	BYTE CategoryIndex;
	BYTE PageIndex;
	BYTE ItemCount;
	struct
	{
		DWORD Slot;
		BYTE Category;
		BYTE Durability;
		BYTE SellRate;
		WORD Value;
		BYTE Flag;
		BYTE ItemInfo[MAX_ITEM_INFO];
		DWORD Duration;
	}ItemInfo[MAX_MU_CASH_SHOP_PAGE_ITEM];
	#pragma pack()
};

struct PMSG_MU_CASH_SHOP_BUY_SEND
{
	#pragma pack(1)
	PSBMSG_HEAD header; // C1:F5:08
	BYTE result;
	#pragma pack()
};

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_MU_CASH_SHOP_POINT_RECV
{
	PSBMSG_HEAD header; // C1:1C:00
	WORD index;
	char account[11];
	BYTE result;
	DWORD Coin;
};

struct SDHP_MU_CASH_SHOP_ITEM_BUY_RECV
{
	PSBMSG_HEAD header; // C1:1C:01
	WORD index;
	char account[11];
	BYTE result;
	DWORD SlotIndex;
	BYTE CategoryIndex;
	DWORD Coin;
};

struct SDHP_MU_CASH_SHOP_RECIEVE_POINT_RECV
{
	PSBMSG_HEAD header; // C1:1C:02
	WORD index;
	char account[11];
	DWORD CallbackFunc;
	DWORD CallbackArg1;
	DWORD CallbackArg2;
	DWORD Coin;
};

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_MU_CASH_SHOP_POINT_SEND
{
	PSBMSG_HEAD header; // C1:1C:00
	WORD index;
	char account[11];
};

struct SDHP_MU_CASH_SHOP_ITEM_BUY_SEND
{
	PSBMSG_HEAD header; // C1:1C:01
	WORD index;
	char account[11];
	DWORD SlotIndex;
	BYTE CategoryIndex;
};

struct SDHP_MU_CASH_SHOP_RECIEVE_POINT_SEND
{
	PSBMSG_HEAD header; // C1:1C:02
	WORD index;
	char account[11];
	DWORD CallbackFunc;
	DWORD CallbackArg1;
	DWORD CallbackArg2;
};

struct SDHP_MU_CASH_SHOP_ADD_POINT_SAVE_SEND
{
	PSBMSG_HEAD header; // C1:1C:30
	WORD index;
	char account[11];
	DWORD AddCoin;
	DWORD MaxCoin;
};

struct SDHP_MU_CASH_SHOP_SUB_POINT_SAVE_SEND
{
	PSBMSG_HEAD header; // C1:1C:31
	WORD index;
	char account[11];
	DWORD SubCoin;
	DWORD MaxCoin;
};

//**********************************************//
//**********************************************//
//**********************************************//

struct MU_CASH_SHOP_ITEM_INFO
{
	int Active;
	int CategoryIndex;
	int Flag;
	int ItemIndex;
	int ItemLevel;
	int ItemDurability;
	int ItemOption1;
	int ItemOption2;
	int ItemOption3;
	int ItemNewOption;
	int ItemSetOption;
	int ItemHarmonyOption;
	int ItemOptionEx;
	int ItemDuration;
	int CoinValue;
};

class CMuCashShop
{
public:
	CMuCashShop();
	virtual ~CMuCashShop();
	void Load(char* path);
	void RegenItemList();
	void CGMuCashShopItemRecv(PMSG_MU_CASH_SHOP_ITEM_RECV* lpMsg,int aIndex);
	void CGMuCashShopOpenRecv(PMSG_MU_CASH_SHOP_OPEN_RECV* lpMsg,int aIndex);
	void CGMuCashShopItemBuyRecv(PMSG_MU_CASH_SHOP_BUY_RECV* lpMsg,int aIndex);
	void DGMuCashShopPointRecv(SDHP_MU_CASH_SHOP_POINT_RECV* lpMsg);
	void DGMuCashShopItemBuyRecv(SDHP_MU_CASH_SHOP_ITEM_BUY_RECV* lpMsg);
	void DGMuCashShopRecievePointRecv(SDHP_MU_CASH_SHOP_RECIEVE_POINT_RECV* lpMsg);
	void GDMuCashPointSend(int aIndex);
	void GDMuCashPointRecievePointSend(int aIndex,DWORD CallbackFunc,DWORD CallbackArg1,DWORD CallbackArg2);
	void GDMuCashPointAddPointSaveSend(int aIndex,DWORD AddCoin);
	void GDMuCashPointSubPointSaveSend(int aIndex,DWORD SubCoin);
private:
	int m_CashShopProductCount[MAX_MU_CASH_SHOP_CATEGORY];
	MU_CASH_SHOP_ITEM_INFO m_CashShopProductInfo[MAX_MU_CASH_SHOP_CATEGORY][MAX_MU_CASH_SHOP_ITEM];
	PMSG_MU_CASH_SHOP_ITEM_SEND m_CashItemList[MAX_MU_CASH_SHOP_CATEGORY][MAX_MU_CASH_SHOP_PAGE];
};

extern CMuCashShop gMuCashShop;