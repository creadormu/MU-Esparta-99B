// MuCashShop.h: interface for the CMuCashShop class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "DataServerProtocol.h"

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_MU_CASH_SHOP_POINT_RECV
{
	PSBMSG_HEAD header; // C1:1C:00
	WORD index;
	char account[11];
};

struct SDHP_MU_CASH_SHOP_ITEM_BUY_RECV
{
	PSBMSG_HEAD header; // C1:1C:01
	WORD index;
	char account[11];
	DWORD SlotIndex;
	BYTE CategoryIndex;
};

struct SDHP_MU_CASH_SHOP_RECIEVE_POINT_RECV
{
	PSBMSG_HEAD header; // C1:1C:02
	WORD index;
	char account[11];
	DWORD CallbackFunc;
	DWORD CallbackArg1;
	DWORD CallbackArg2;
};

struct SDHP_MU_CASH_SHOP_ADD_POINT_SAVE_RECV
{
	PSBMSG_HEAD header; // C1:1C:30
	WORD index;
	char account[11];
	DWORD AddPcPoint;
	DWORD MaxPcPoint;
};

struct SDHP_MU_CASH_SHOP_SUB_POINT_SAVE_RECV
{
	PSBMSG_HEAD header; // C1:1C:31
	WORD index;
	char account[11];
	DWORD SubPcPoint;
	DWORD MaxPcPoint;
};

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_MU_CASH_SHOP_POINT_SEND
{
	PSBMSG_HEAD header; // C1:1C:00
	WORD index;
	char account[11];
	BYTE result;
	DWORD PcPoint;
};

struct SDHP_MU_CASH_SHOP_ITEM_BUY_SEND
{
	PSBMSG_HEAD header; // C1:1C:01
	WORD index;
	char account[11];
	BYTE result;
	DWORD SlotIndex;
	BYTE CategoryIndex;
	DWORD PcPoint;
};

struct SDHP_MU_CASH_SHOP_RECIEVE_POINT_SEND
{
	PSBMSG_HEAD header; // C1:1C:02
	WORD index;
	char account[11];
	DWORD CallbackFunc;
	DWORD CallbackArg1;
	DWORD CallbackArg2;
	DWORD PcPoint;
};

//**********************************************//
//**********************************************//
//**********************************************//

class CMuCashShop
{
public:
	CMuCashShop();
	virtual ~CMuCashShop();
	void GDMuCashShopPointRecv(SDHP_MU_CASH_SHOP_POINT_RECV* lpMsg,int index);
	void GDMuCashShopItemBuyRecv(SDHP_MU_CASH_SHOP_ITEM_BUY_RECV* lpMsg,int index);
	void GDMuCashShopRecievePointRecv(SDHP_MU_CASH_SHOP_RECIEVE_POINT_RECV* lpMsg,int index);
	void GDMuCashShopAddPointSaveRecv(SDHP_MU_CASH_SHOP_ADD_POINT_SAVE_RECV* lpMsg);
	void GDMuCashShopSubPointSaveRecv(SDHP_MU_CASH_SHOP_SUB_POINT_SAVE_RECV* lpMsg);
};

extern CMuCashShop gMuCashShop;