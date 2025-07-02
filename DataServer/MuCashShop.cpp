// MuCashShop.cpp: implementation of the CMuCashShop class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MuCashShop.h"
#include "QueryManager.h"
#include "SocketManager.h"

CMuCashShop gMuCashShop;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMuCashShop::CMuCashShop() // OK
{

}

CMuCashShop::~CMuCashShop() // OK
{

}

void CMuCashShop::GDMuCashShopPointRecv(SDHP_MU_CASH_SHOP_POINT_RECV* lpMsg,int index) // OK
{
	#if(DATASERVER_SHOP==2 || DATASERVER_SHOP==4)

	SDHP_MU_CASH_SHOP_POINT_SEND pMsg;

	pMsg.header.set(0x1C,0x00,sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account,lpMsg->account,sizeof(pMsg.account));

	pMsg.result = 0;

	if(gQueryManager.ExecQuery("SELECT * FROM PcPointData WHERE AccountID='%s'",lpMsg->account) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();

		if(gQueryManager.ExecQuery("INSERT INTO PcPointData (AccountID,PcPoint) VALUES ('%s',0)",lpMsg->account) == 0)
		{
			gQueryManager.Close();

			pMsg.result = 1;
		}
		else
		{
			gQueryManager.Close();

			pMsg.PcPoint = 0;
		}
	}
	else
	{
		pMsg.PcPoint = gQueryManager.GetAsInteger("PcPoint");

		gQueryManager.Close();
	}

	gSocketManager.DataSend(index,(BYTE*)&pMsg,sizeof(pMsg));

	#endif
}

void CMuCashShop::GDMuCashShopItemBuyRecv(SDHP_MU_CASH_SHOP_ITEM_BUY_RECV* lpMsg,int index) // OK
{
	#if(DATASERVER_SHOP==2 || DATASERVER_SHOP==4)

	SDHP_MU_CASH_SHOP_ITEM_BUY_SEND pMsg;

	pMsg.header.set(0x1C,0x01,sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account,lpMsg->account,sizeof(pMsg.account));

	pMsg.result = 0;

	pMsg.SlotIndex = lpMsg->SlotIndex;

	pMsg.CategoryIndex = lpMsg->CategoryIndex;

	if(gQueryManager.ExecQuery("SELECT * FROM PcPointData WHERE AccountID='%s'",lpMsg->account) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();

		pMsg.result = 1;
	}
	else
	{
		pMsg.PcPoint = gQueryManager.GetAsInteger("PcPoint");

		gQueryManager.Close();
	}

	gSocketManager.DataSend(index,(BYTE*)&pMsg,sizeof(pMsg));

	#endif
}

void CMuCashShop::GDMuCashShopRecievePointRecv(SDHP_MU_CASH_SHOP_RECIEVE_POINT_RECV* lpMsg,int index) // OK
{
	#if(DATASERVER_SHOP==2 || DATASERVER_SHOP==4)

	SDHP_MU_CASH_SHOP_RECIEVE_POINT_SEND pMsg;

	pMsg.header.set(0x1C,0x02,sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account,lpMsg->account,sizeof(pMsg.account));

	pMsg.CallbackFunc = lpMsg->CallbackFunc;

	pMsg.CallbackArg1 = lpMsg->CallbackArg1;

	pMsg.CallbackArg2 = lpMsg->CallbackArg2;

	if(gQueryManager.ExecQuery("SELECT * FROM PcPointData WHERE AccountID='%s'",lpMsg->account) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();

		pMsg.PcPoint = 0;
	}
	else
	{
		pMsg.PcPoint = gQueryManager.GetAsInteger("PcPoint");

		gQueryManager.Close();
	}

	gSocketManager.DataSend(index,(BYTE*)&pMsg,sizeof(pMsg));

	#endif
}

void CMuCashShop::GDMuCashShopAddPointSaveRecv(SDHP_MU_CASH_SHOP_ADD_POINT_SAVE_RECV* lpMsg) // OK
{
	#if(DATASERVER_SHOP==2 || DATASERVER_SHOP==4)

	if(gQueryManager.ExecQuery("SELECT PcPoint FROM PcPointData WHERE AccountID='%s'",lpMsg->account) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();

		gQueryManager.ExecQuery("INSERT INTO PcPointData (AccountID,PcPoint) VALUES ('%s',%d)",lpMsg->account,((lpMsg->AddPcPoint>lpMsg->MaxPcPoint)?lpMsg->MaxPcPoint:lpMsg->AddPcPoint));

		gQueryManager.Close();
	}
	else
	{
		DWORD PcPoint = gQueryManager.GetAsInteger("PcPoint");

		gQueryManager.Close();

		gQueryManager.ExecQuery("UPDATE PcPointData SET PcPoint=%d WHERE AccountID='%s'",(((PcPoint+lpMsg->AddPcPoint)>lpMsg->MaxPcPoint)?lpMsg->MaxPcPoint:(PcPoint+lpMsg->AddPcPoint)),lpMsg->account);

		gQueryManager.Close();
	}

	#endif
}

void CMuCashShop::GDMuCashShopSubPointSaveRecv(SDHP_MU_CASH_SHOP_SUB_POINT_SAVE_RECV* lpMsg) // OK
{
	#if(DATASERVER_SHOP==2 || DATASERVER_SHOP==4)

	if(gQueryManager.ExecQuery("SELECT PcPoint FROM PcPointData WHERE AccountID='%s'",lpMsg->account) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();

		gQueryManager.ExecQuery("INSERT INTO PcPointData (AccountID,PcPoint) VALUES ('%s',%d)",lpMsg->account,0,0,0);

		gQueryManager.Close();
	}
	else
	{
		DWORD PcPoint = gQueryManager.GetAsInteger("PcPoint");

		gQueryManager.Close();

		gQueryManager.ExecQuery("UPDATE PcPointData SET PcPoint=%d WHERE AccountID='%s'",((lpMsg->SubPcPoint>PcPoint)?0:(PcPoint-lpMsg->SubPcPoint)),lpMsg->account);

		gQueryManager.Close();
	}

	#endif
}