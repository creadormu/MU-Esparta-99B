// MuCashShop.cpp: implementation of the CMuCashShop class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MuCashShop.h"
#include "DSProtocol.h"
#include "EffectManager.h"
#include "GameMain.h"
#include "Item.h"
#include "Log.h"
#include "Map.h"
#include "MemScript.h"
#include "ServerInfo.h"
#include "Util.h"

CMuCashShop gMuCashShop;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMuCashShop::CMuCashShop() // OK
{
	#if(GAMESERVER_SHOP==2)

	memset(this->m_CashShopProductCount,0,sizeof(this->m_CashShopProductCount));

	memset(this->m_CashShopProductInfo,0,sizeof(this->m_CashShopProductInfo));
	
	memset(this->m_CashItemList,0,sizeof(this->m_CashItemList));

	#endif
}

CMuCashShop::~CMuCashShop() // OK
{

}

void CMuCashShop::Load(char* path) // OK
{
	#if(GAMESERVER_SHOP==2)

	CMemScript* lpMemScript = new CMemScript;

	if(lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR,path);
		return;
	}

	if(lpMemScript->SetBuffer(path) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	memset(this->m_CashShopProductInfo,0,sizeof(this->m_CashShopProductInfo));
	
	memset(this->m_CashShopProductCount,0,sizeof(this->m_CashShopProductCount));
	
	memset(this->m_CashItemList,0,sizeof(this->m_CashItemList));

	try
	{
		while (true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			if(strcmp("end",lpMemScript->GetString()) == 0)
			{
				break;
			}

			MU_CASH_SHOP_ITEM_INFO info;

			memset(&info,0,sizeof(info));

			info.Active = 1;

			info.CategoryIndex = lpMemScript->GetNumber();

			info.Flag = lpMemScript->GetAsNumber();

			info.ItemIndex = SafeGetItem(GET_ITEM(lpMemScript->GetAsNumber(),lpMemScript->GetAsNumber()));

			info.ItemLevel = lpMemScript->GetAsNumber();

			info.ItemDurability = lpMemScript->GetAsNumber();

			info.ItemOption1 = lpMemScript->GetAsNumber();

			info.ItemOption2 = lpMemScript->GetAsNumber();

			info.ItemOption3 = lpMemScript->GetAsNumber();

			info.ItemNewOption = lpMemScript->GetAsNumber();

			info.ItemSetOption = lpMemScript->GetAsNumber();

			info.ItemHarmonyOption = lpMemScript->GetAsNumber();

			info.ItemOptionEx = lpMemScript->GetAsNumber();

			info.ItemDuration = lpMemScript->GetAsNumber();

			info.CoinValue = lpMemScript->GetAsNumber();

			if(this->m_CashShopProductCount[info.CategoryIndex] >= MAX_MU_CASH_SHOP_ITEM)
			{
				return;
			}

			this->m_CashShopProductInfo[info.CategoryIndex][this->m_CashShopProductCount[info.CategoryIndex]++] = info;
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;

	this->RegenItemList();

	#endif
}

void CMuCashShop::RegenItemList() // OK
{
	#if(GAMESERVER_SHOP==2)

	for(int i=0;i<MAX_MU_CASH_SHOP_CATEGORY;i++)
	{
		if(this->m_CashShopProductCount[i] == 0)
		{
			continue;
		}

		int PageCount = (this->m_CashShopProductCount[i]/MAX_MU_CASH_SHOP_PAGE_ITEM);

		for(int n=0;n<=PageCount;n++)
		{
			PMSG_MU_CASH_SHOP_ITEM_SEND* lpInfo = &this->m_CashItemList[i][n];

			lpInfo->CategoryIndex = i;
			lpInfo->PageIndex = n;
			lpInfo->ItemCount = ((n<PageCount)?MAX_MU_CASH_SHOP_PAGE_ITEM:(this->m_CashShopProductCount[i]-MAX_MU_CASH_SHOP_PAGE_ITEM*n));

			for(int j=0;j<lpInfo->ItemCount;j++)
			{
				int ItemCount = (MAX_MU_CASH_SHOP_PAGE_ITEM*n+j);

				MU_CASH_SHOP_ITEM_INFO lpItem = this->m_CashShopProductInfo[i][ItemCount];

				lpInfo->ItemInfo[j].Slot = (ItemCount+1);
				lpInfo->ItemInfo[j].Category = i;
				lpInfo->ItemInfo[j].Durability = (BYTE)((lpItem.ItemDurability==0)?gItemManager.GetItemDurability(lpItem.ItemIndex,lpItem.ItemLevel,lpItem.ItemNewOption,lpItem.ItemSetOption):lpItem.ItemDurability);
				lpInfo->ItemInfo[j].SellRate = 0;
				lpInfo->ItemInfo[j].Value = lpItem.CoinValue;
				lpInfo->ItemInfo[j].Flag = lpItem.Flag;
				lpInfo->ItemInfo[j].Duration = (lpItem.ItemDuration/60);

				CItem Item;
				
				Item.m_Level = lpItem.ItemLevel;
				Item.m_Durability = (float)gItemManager.GetItemDurability(lpItem.ItemIndex,lpItem.ItemLevel,lpItem.ItemNewOption,lpItem.ItemSetOption);
				
				Item.Convert(lpItem.ItemIndex,lpItem.ItemOption1,lpItem.ItemOption2,lpItem.ItemOption3,lpItem.ItemNewOption,lpItem.ItemSetOption,lpItem.ItemHarmonyOption,lpItem.ItemOptionEx,0,0xFF);
		
				gItemManager.ItemByteConvert(lpInfo->ItemInfo[j].ItemInfo,Item);
			}
		}
	}

	#endif
}

void CMuCashShop::CGMuCashShopItemRecv(PMSG_MU_CASH_SHOP_ITEM_RECV* lpMsg,int aIndex) // OK
{
	#if(GAMESERVER_SHOP==2)

	if(gServerInfo.m_CashShopSwitch == 0)
	{
		return;
	}

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	PMSG_MU_CASH_SHOP_ITEM_SEND pMsg = this->m_CashItemList[lpMsg->CategoryIndex][lpMsg->PageIndex];
	
	pMsg.header.set(0xF5,0x06,sizeof(pMsg));
	
	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);

	#endif
}

void CMuCashShop::CGMuCashShopOpenRecv(PMSG_MU_CASH_SHOP_OPEN_RECV* lpMsg,int aIndex) // OK
{
	#if(GAMESERVER_SHOP==2)

	if(gServerInfo.m_CashShopSwitch == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	PMSG_MU_CASH_SHOP_OPEN_SEND pMsg;

	pMsg.header.set(0xF5,0x02,sizeof(pMsg));

	pMsg.Result = 0;

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(gItemManager.CheckItemInChaosBox(lpObj) != 0 || gItemManager.CheckItemInTrade(lpObj) != 0)
	{
		return;
	}

	if(lpObj->Permission[10] != 0)
	{
		pMsg.Result = 6;
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	if(gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,1) == 0)
	{
		pMsg.Result = 6;
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	if(lpMsg->OpenType == 0)
	{
		lpObj->Interface.use = 0;
		lpObj->Interface.type = INTERFACE_NONE;
		lpObj->Interface.state = 0;
	}
	else
	{
		if(lpObj->Interface.use == 0)
		{
			lpObj->Interface.use = 1;
			lpObj->Interface.type = INTERFACE_CASH_SHOP;
			lpObj->Interface.state = 0;
			this->GDMuCashPointSend(aIndex);
		}
		else
		{
			pMsg.Result = 8;
		}
	}

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);

	#endif
}

void CMuCashShop::CGMuCashShopItemBuyRecv(PMSG_MU_CASH_SHOP_BUY_RECV* lpMsg,int aIndex) // OK
{
	#if(GAMESERVER_SHOP==2)

	if(gServerInfo.m_CashShopSwitch == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(((lpObj->CashShopTransaction[1]==0)?(lpObj->CashShopTransaction[1]++):lpObj->CashShopTransaction[1]) != 0)
	{
		return;
	}

	SDHP_MU_CASH_SHOP_ITEM_BUY_SEND pMsg;

	pMsg.header.set(0x1C,0x01,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	pMsg.SlotIndex = lpMsg->SlotIndex;

	pMsg.CategoryIndex = lpMsg->CategoryIndex;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);

	#endif
}

void CMuCashShop::DGMuCashShopPointRecv(SDHP_MU_CASH_SHOP_POINT_RECV* lpMsg) // OK
{
	#if(GAMESERVER_SHOP==2)

	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGMuCashShopPointRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	lpObj->CashShopTransaction[0] = 0;

	lpObj->Coin1 = lpMsg->Coin;
	
	lpObj->Coin2 = 0;
	
	lpObj->Coin3 = 0;

	PMSG_MU_CASH_SHOP_POINT_SEND pMsg;

	pMsg.header.set(0xF5,0x04,sizeof(pMsg));

	pMsg.Point = lpMsg->Coin;

	DataSend(lpMsg->index,(BYTE*)&pMsg,pMsg.header.size);

	#endif
}

void CMuCashShop::DGMuCashShopItemBuyRecv(SDHP_MU_CASH_SHOP_ITEM_BUY_RECV* lpMsg) // OK
{
	#if(GAMESERVER_SHOP==2)

	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGMuCashShopItemBuyRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	lpObj->CashShopTransaction[1] = 0;

	PMSG_MU_CASH_SHOP_BUY_SEND pMsg;

	pMsg.header.set(0xF5,0x08,sizeof(pMsg));

	pMsg.result = 0;

	if(lpMsg->result != 0)
	{
		pMsg.result = 7;
		DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	if(lpObj->Interface.use == 0 || lpObj->Interface.type != INTERFACE_CASH_SHOP)
	{
		pMsg.result = 7;
		DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	gObjFixInventoryPointer(lpObj->Index);

	if(lpObj->Transaction == 1)
	{
		pMsg.result = 7;
		DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	MU_CASH_SHOP_ITEM_INFO lpProductInfo = this->m_CashShopProductInfo[lpMsg->CategoryIndex-1][lpMsg->SlotIndex-1];

	if(lpProductInfo.Active != 1)
	{
		pMsg.result = 6;
		DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	if(lpMsg->Coin < (DWORD)lpProductInfo.CoinValue)
	{
		pMsg.result = 1;
		DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	EFFECT_INFO* lpInfo = gEffectManager.GetInfoByItem(lpProductInfo.ItemIndex);

	if(lpInfo == 0)
	{
		if(gItemManager.CheckItemInventorySpace(lpObj,lpProductInfo.ItemIndex) == 0)
		{
			pMsg.result = 7;
			DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
			return;
		}

		GDCreateItemSend(lpObj->Index,0xEB,0,0,lpProductInfo.ItemIndex,lpProductInfo.ItemLevel,lpProductInfo.ItemDurability,lpProductInfo.ItemOption1,lpProductInfo.ItemOption2,lpProductInfo.ItemOption3,-1,lpProductInfo.ItemNewOption,lpProductInfo.ItemSetOption,lpProductInfo.ItemHarmonyOption,lpProductInfo.ItemOptionEx,0,0,((lpProductInfo.ItemDuration>0)?((DWORD)time(0)+lpProductInfo.ItemDuration):0));
	}
	else
	{
		if(gEffectManager.GetEffect(lpObj,lpInfo->Index) != 0)
		{
			pMsg.result = 7;
			DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
			return;
		}

		gEffectManager.AddEffect(lpObj,1,lpInfo->Index,(int)(time(0)+lpProductInfo.ItemDuration),0,0,0,0);
	}

	gLog.Output(LOG_CASH_SHOP,"[CashShopItemBuy][ProductInfo][%s][%s] - (Category: %d,CoinValue: %d,ItemIndex: %d,ItemLevel: %d,ItemDurability: %d,ItemOption1: %d,ItemOption2: %d,ItemOption3: %d,ItemNewOption: %d,ItemSetOption: %d,ItemHarmonyOption: %d,ItemOptionEx: %d,ItemDuration: %d)",lpObj->Account,lpObj->Name,lpProductInfo.CategoryIndex,lpProductInfo.CoinValue,lpProductInfo.ItemIndex,lpProductInfo.ItemLevel,lpProductInfo.ItemDurability,lpProductInfo.ItemOption1,lpProductInfo.ItemOption2,lpProductInfo.ItemOption3,lpProductInfo.ItemNewOption,lpProductInfo.ItemSetOption,lpProductInfo.ItemHarmonyOption,lpProductInfo.ItemOptionEx,lpProductInfo.ItemDuration);

	gObjCoinSub(lpObj->Index,lpProductInfo.CoinValue,0,0);
	
	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);

	#endif
}

void CMuCashShop::DGMuCashShopRecievePointRecv(SDHP_MU_CASH_SHOP_RECIEVE_POINT_RECV* lpMsg) // OK
{
	#if(GAMESERVER_SHOP==2)

	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGMuCashShopRecievePointRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	((void(*)(LPOBJ,DWORD,DWORD,DWORD,DWORD,DWORD))lpMsg->CallbackFunc)(lpObj,lpMsg->CallbackArg1,lpMsg->CallbackArg2,0,0,lpMsg->Coin);

	#endif
}

void CMuCashShop::GDMuCashPointSend(int aIndex) // OK
{
	#if(GAMESERVER_SHOP==2)

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(gObjIsAccountValid(aIndex,gObj[aIndex].Account) == 0)
	{
		return;
	}

	if(((gObj[aIndex].CashShopTransaction[0]==0)?(gObj[aIndex].CashShopTransaction[0]++):gObj[aIndex].CashShopTransaction[0]) != 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	lpObj->Coin1 = 0;

	lpObj->Coin2 = 0;

	lpObj->Coin3 = 0;

	SDHP_MU_CASH_SHOP_POINT_SEND pMsg;

	pMsg.header.set(0x1C,0x00,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);

	#endif
}

void CMuCashShop::GDMuCashPointRecievePointSend(int aIndex,DWORD CallbackFunc,DWORD CallbackArg1,DWORD CallbackArg2) // OK
{
	#if(GAMESERVER_SHOP==2)

	if(gObjIsAccountValid(aIndex,gObj[aIndex].Account) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	SDHP_MU_CASH_SHOP_RECIEVE_POINT_SEND pMsg;

	pMsg.header.set(0x1C,0x02,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	pMsg.CallbackFunc = CallbackFunc;

	pMsg.CallbackArg1 = CallbackArg1;

	pMsg.CallbackArg2 = CallbackArg2;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);

	#endif
}

void CMuCashShop::GDMuCashPointAddPointSaveSend(int aIndex,DWORD AddCoin) // OK
{
	#if(GAMESERVER_SHOP==2)

	SDHP_MU_CASH_SHOP_ADD_POINT_SAVE_SEND pMsg;

	pMsg.header.set(0x1C,0x30,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

	pMsg.AddCoin = AddCoin;

	pMsg.MaxCoin = gServerInfo.m_CashShopMaxPoint;

	gDataServerConnection.DataSend((BYTE*)&pMsg,sizeof(pMsg),aIndex);

	this->GDMuCashPointSend(aIndex);

	#endif
}

void CMuCashShop::GDMuCashPointSubPointSaveSend(int aIndex,DWORD SubCoin) // OK
{
	#if(GAMESERVER_SHOP==2)

	SDHP_MU_CASH_SHOP_SUB_POINT_SAVE_SEND pMsg;

	pMsg.header.set(0x1C,0x31,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

	pMsg.SubCoin = SubCoin;

	pMsg.MaxCoin = gServerInfo.m_CashShopMaxPoint;

	gDataServerConnection.DataSend((BYTE*)&pMsg,sizeof(pMsg),aIndex);

	this->GDMuCashPointSend(aIndex);

	#endif
}