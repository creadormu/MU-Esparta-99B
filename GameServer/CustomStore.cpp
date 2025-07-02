// CustomStore.cpp: implementation of the CCustomStore class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomStore.h"
#include "CashShop.h"
#include "CommandManager.h"
#include "DSProtocol.h"
#include "GameMain.h"
#include "Log.h"
#include "Map.h"
#include "MapManager.h"
#include "MasterSkillTree.h"
#include "Message.h"
#include "MuCashShop.h"
#include "Notice.h"
#include "PcPoint.h"
#include "ServerInfo.h"
#include "SocketManager.h"
#include "Util.h"

CCustomStore gCustomStore;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomStore::CCustomStore() // OK
{
	memset(this->m_CustomStoreMapList,0,sizeof(this->m_CustomStoreMapList));
}

CCustomStore::~CCustomStore() // OK
{

}

void CCustomStore::ReadCustomStoreInfo(char* section,char* path) // OK
{
	this->m_CustomStoreSwitch = GetPrivateProfileInt(section,"CustomStoreSwitch",0,path);

	this->m_CustomStoreMapZone = GetPrivateProfileInt(section,"CustomStoreMapZone",0,path);

	if(this->m_CustomStoreMapZone != -1)
	{
		memset(this->m_CustomStoreMapList[0],0,sizeof(this->m_CustomStoreMapList[0]));

		char buff[256] = {0};

		GetPrivateProfileString(section,"CustomStoreMapZone","",buff,sizeof(buff),path);

		char* next_token;

		char* token = strtok_s(buff,",",&next_token);
	
		while(token != 0)
		{
			if(gMapManager.IsValidMap(atoi(token)) != 0)
			{
				this->m_CustomStoreMapList[0][atoi(token)] = 1;
			}

			token = strtok_s(0,",",&next_token);
		}
	}

	GetPrivateProfileString(section,"CustomStoreCommandJoBSyntax","",this->m_CustomStoreCommandJoBSyntax,sizeof(this->m_CustomStoreCommandJoBSyntax),path);
	
	GetPrivateProfileString(section,"CustomStoreCommandJoSSyntax","",this->m_CustomStoreCommandJoSSyntax,sizeof(this->m_CustomStoreCommandJoSSyntax),path);
	
	GetPrivateProfileString(section,"CustomStoreCommandJoLSyntax","",this->m_CustomStoreCommandJoLSyntax,sizeof(this->m_CustomStoreCommandJoLSyntax),path);
	
	GetPrivateProfileString(section,"CustomStoreCommandJoCSyntax","",this->m_CustomStoreCommandJoCSyntax,sizeof(this->m_CustomStoreCommandJoCSyntax),path);
	
	GetPrivateProfileString(section,"CustomStoreCommandJCRSyntax","",this->m_CustomStoreCommandJCRSyntax,sizeof(this->m_CustomStoreCommandJCRSyntax),path);
	
	GetPrivateProfileString(section,"CustomStoreCommandJoGSyntax","",this->m_CustomStoreCommandJoGSyntax,sizeof(this->m_CustomStoreCommandJoGSyntax),path);
	
	#if(GAMESERVER_UPDATE>=201)
	GetPrivateProfileString(section,"CustomStoreCommandJoHSyntax","",this->m_CustomStoreCommandJoHSyntax,sizeof(this->m_CustomStoreCommandJoHSyntax),path);
	#endif
	#if(GAMESERVER_SHOP>=1)
	GetPrivateProfileString(section,"CustomStoreCommandCoin1Syntax","",this->m_CustomStoreCommandCoin1Syntax,sizeof(this->m_CustomStoreCommandCoin1Syntax),path);
	#endif
	#if(GAMESERVER_SHOP==3)
	GetPrivateProfileString(section,"CustomStoreCommandCoin2Syntax","",this->m_CustomStoreCommandCoin2Syntax,sizeof(this->m_CustomStoreCommandCoin2Syntax),path);
	GetPrivateProfileString(section,"CustomStoreCommandCoin3Syntax","",this->m_CustomStoreCommandCoin3Syntax,sizeof(this->m_CustomStoreCommandCoin3Syntax),path);
	#endif

	this->m_CustomStoreOfflineSwitch = GetPrivateProfileInt(section,"CustomStoreOfflineSwitch",0,path);

	#if(GAMESERVER_SHOP>=1)
	this->m_CustomStoreOfflineCoinGain = GetPrivateProfileInt(section,"CustomStoreOfflineCoinGain",0,path);
	#endif

	this->m_CustomStoreOfflineMapZone = GetPrivateProfileInt(section,"CustomStoreOfflineMapZone",0,path);

	if(this->m_CustomStoreOfflineMapZone != -1)
	{
		memset(this->m_CustomStoreMapList[1],0,sizeof(this->m_CustomStoreMapList[1]));

		char buff[256] = {0};

		GetPrivateProfileString(section,"CustomStoreOfflineMapZone","",buff,sizeof(buff),path);

		char* next_token;

		char* token = strtok_s(buff,",",&next_token);
	
		while(token != 0)
		{
			if(gMapManager.IsValidMap(atoi(token)) != 0)
			{
				this->m_CustomStoreMapList[1][atoi(token)] = 1;
			}

			token = strtok_s(0,",",&next_token);
		}
	}
}

bool CCustomStore::CommandCustomStore(LPOBJ lpObj,char* arg) // OK
{
	if(this->m_CustomStoreSwitch == 0)
	{
		return 0;
	}

	if(lpObj->Interface.use != 0)
	{
		return 0;
	}

	char mode[32] = {0};

	gCommandManager.GetString(arg,mode,sizeof(mode),0);

	if(strcmp(mode,"") != 0)
	{
		if(strcmp(mode,this->m_CustomStoreCommandJoBSyntax) == 0)
		{
			return this->OpenCustomStore(lpObj,0);
		}
		else if(strcmp(mode,this->m_CustomStoreCommandJoSSyntax) == 0)
		{
			return this->OpenCustomStore(lpObj,1);
		}
		else if(strcmp(mode,this->m_CustomStoreCommandJoLSyntax) == 0)
		{
			return this->OpenCustomStore(lpObj,2);
		}
		else if(strcmp(mode,this->m_CustomStoreCommandJoCSyntax) == 0)
		{
			return this->OpenCustomStore(lpObj,3);
		}
		if(strcmp(mode,this->m_CustomStoreCommandJCRSyntax) == 0)
		{
			return this->OpenCustomStore(lpObj,4);
		}
		else if(strcmp(mode,this->m_CustomStoreCommandJoGSyntax) == 0)
		{
			return this->OpenCustomStore(lpObj,5);
		}
		#if(GAMESERVER_UPDATE>=201)
		else if(strcmp(mode,this->m_CustomStoreCommandJoHSyntax) == 0)
		{
			return this->OpenCustomStore(lpObj,6);
		}
		#endif
		#if(GAMESERVER_SHOP>=1)
		else if(strcmp(mode,this->m_CustomStoreCommandCoin1Syntax) == 0)
		{
			return this->OpenCustomStore(lpObj,7);
		}
		#endif
		#if(GAMESERVER_SHOP==3)
		else if(strcmp(mode,this->m_CustomStoreCommandCoin2Syntax) == 0)
		{
			return this->OpenCustomStore(lpObj,8);
		}
		else if(strcmp(mode,this->m_CustomStoreCommandCoin3Syntax) == 0)
		{
			return this->OpenCustomStore(lpObj,9);
		}
		#endif
	}
	else
	{
		gPersonalShop.CGPShopCloseRecv(lpObj->Index);
	}

	return 0;
}

bool CCustomStore::CommandCustomStoreOffline(LPOBJ lpObj) // OK
{
	if(this->m_CustomStoreOfflineSwitch == 0)
	{
		return 0;
	}

	if(lpObj->Interface.use != 0)
	{
		return 0;
	}

	if(lpObj->PShopOpen == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(540));
		return 0;
	}

	if(this->m_CustomStoreOfflineMapZone != -1 && this->m_CustomStoreMapList[1][lpObj->Map] == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(574));
		return 0;
	}

	if(gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,1) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(541));
		return 0;
	}

	if(CC_MAP_RANGE(lpObj->Map) != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(574));
		return 0;
	}

	#if(GAMESERVER_UPDATE>=301)

	if(IT_MAP_RANGE(lpObj->Map) != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(574));
		return 0;
	}

	#endif

	#if(GAMESERVER_UPDATE>=803)

	if(CF_MAP_RANGE(lpObj->Map) != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(574));
		return 0;
	}

	#endif

	lpObj->Socket = INVALID_SOCKET;

	lpObj->PShopCustomOffline = 1;

	lpObj->PShopCustomOfflineTime = 0;

	closesocket(lpObj->PerSocketContext->Socket);

	return 1;
}

bool CCustomStore::OpenCustomStore(LPOBJ lpObj,int type) // OK
{
	if(gMapManager.GetMapPShopEnable(lpObj->Map) == 0)
	{
		return 0;
	}

	if(this->m_CustomStoreMapZone != -1 && this->m_CustomStoreMapList[0][lpObj->Map] == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(574));
		return 0;
	}

	if(gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,1) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(541));
		return 0;
	}

	if(gPersonalShop.CheckPersonalShopOpen(lpObj->Index) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(542));
		return 0;
	}

	if(lpObj->Level <= 5)
	{
		gPersonalShop.GCPShopOpenSend(lpObj->Index,2);
		return 0;
	}

	lpObj->PShopOpen = 1;

	lpObj->PShopCustom = 1;

	lpObj->PShopCustomType = type;

	lpObj->PShopCustomOffline = 0;

	lpObj->PShopCustomOfflineTime = 0;

	switch(type)
	{
		case 0:
			memcpy(lpObj->PShopText,gMessage.GetMessage(543),sizeof(lpObj->PShopText));
			break;
		case 1:
			memcpy(lpObj->PShopText,gMessage.GetMessage(544),sizeof(lpObj->PShopText));
			break;
		case 2:
			memcpy(lpObj->PShopText,gMessage.GetMessage(545),sizeof(lpObj->PShopText));
			break;
		case 3:
			memcpy(lpObj->PShopText,gMessage.GetMessage(546),sizeof(lpObj->PShopText));
			break;
		case 4:
			memcpy(lpObj->PShopText,gMessage.GetMessage(547),sizeof(lpObj->PShopText));
			break;
		case 5:
			memcpy(lpObj->PShopText,gMessage.GetMessage(548),sizeof(lpObj->PShopText));
			break;
		case 6:
			memcpy(lpObj->PShopText,gMessage.GetMessage(549),sizeof(lpObj->PShopText));
			break;
		case 7:
			memcpy(lpObj->PShopText,gMessage.GetMessage(550),sizeof(lpObj->PShopText));
			break;
		case 8:
			memcpy(lpObj->PShopText,gMessage.GetMessage(551),sizeof(lpObj->PShopText));
			break;
		case 9:
			memcpy(lpObj->PShopText,gMessage.GetMessage(552),sizeof(lpObj->PShopText));
			break;
	}

	PMSG_PSHOP_TEXT_CHANGE_SEND pMsg;

	pMsg.header.set(0x3F,0x10,sizeof(pMsg));

	pMsg.index[0] = SET_NUMBERHB(lpObj->Index);

	pMsg.index[1] = SET_NUMBERLB(lpObj->Index);

	memcpy(pMsg.text,lpObj->PShopText,sizeof(pMsg.text));

	memcpy(pMsg.name,lpObj->Name,sizeof(pMsg.name));

	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);

	MsgSendV2(lpObj,(BYTE*)&pMsg,pMsg.header.size);

	return 1;
}

bool CCustomStore::OnPShopOpen(LPOBJ lpObj) // OK
{
	return ((lpObj->PShopCustom==0)?0:1);
}

void CCustomStore::OnPShopClose(LPOBJ lpObj) // OK
{
	if(lpObj->PShopCustom != 0)
	{
		lpObj->PShopCustom = 0;
		lpObj->PShopCustomType = 0;
	}

	if(lpObj->PShopCustomOffline == 1)
	{
		lpObj->PShopCustomOffline = 2;
		lpObj->PShopCustomOfflineTime = 5;
	}
}

void CCustomStore::OnPShopSecondProc(LPOBJ lpObj) // OK
{
	if(lpObj->PShopCustomOffline != 0)
	{
		if(lpObj->PShopCustomOffline == 2)
		{
			if((--lpObj->PShopCustomOfflineTime) == 0)
			{
				gObjDel(lpObj->Index);
				lpObj->PShopCustomOffline = 0;
				lpObj->PShopCustomOfflineTime = 0;
			}
		}

		lpObj->ConnectTickCount = GetTickCount();
		lpObj->OnlineRewardTime = ((this->m_CustomStoreOfflineCoinGain==0)?GetTickCount():lpObj->OnlineRewardTime);
	}
}

void CCustomStore::OnPShopAlreadyConnected(LPOBJ lpObj) // OK
{
	if(lpObj->PShopCustomOffline != 0)
	{
		gObjDel(lpObj->Index);
		lpObj->PShopCustomOffline = 0;
		lpObj->PShopCustomOfflineTime = 0;
	}
}

void CCustomStore::OnPShopItemList(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	if(lpTarget->PShopCustom != 0)
	{
		switch(lpTarget->PShopCustomType)
		{
			case 0:
				gNotice.GCNoticeSend(lpObj->Index,0,0,0,0,0,0,gMessage.GetMessage(553));
				break;
			case 1:
				gNotice.GCNoticeSend(lpObj->Index,0,0,0,0,0,0,gMessage.GetMessage(554));
				break;
			case 2:
				gNotice.GCNoticeSend(lpObj->Index,0,0,0,0,0,0,gMessage.GetMessage(555));
				break;
			case 3:
				gNotice.GCNoticeSend(lpObj->Index,0,0,0,0,0,0,gMessage.GetMessage(556));
				break;
			case 4:
				gNotice.GCNoticeSend(lpObj->Index,0,0,0,0,0,0,gMessage.GetMessage(557));
				break;
			case 5:
				gNotice.GCNoticeSend(lpObj->Index,0,0,0,0,0,0,gMessage.GetMessage(558));
				break;
			case 6:
				gNotice.GCNoticeSend(lpObj->Index,0,0,0,0,0,0,gMessage.GetMessage(559));
				break;
			case 7:
				gNotice.GCNoticeSend(lpObj->Index,0,0,0,0,0,0,gMessage.GetMessage(560));
				break;
			case 8:
				gNotice.GCNoticeSend(lpObj->Index,0,0,0,0,0,0,gMessage.GetMessage(561));
				break;
			case 9:
				gNotice.GCNoticeSend(lpObj->Index,0,0,0,0,0,0,gMessage.GetMessage(562));
				break;
		}
	}
}

bool CCustomStore::OnPShopBuyItemRecv(PMSG_PSHOP_BUY_ITEM_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return 1;
	}

	int bIndex = MAKE_NUMBERW(lpMsg->index[0],lpMsg->index[1]);

	if(gObjIsConnectedGP(bIndex) == 0)
	{
		return 1;
	}

	LPOBJ lpTarget = &gObj[bIndex];

	if(lpTarget->PShopOpen == 0)
	{
		gPersonalShop.GCPShopBuyItemSend(aIndex,bIndex,0,3);
		return 1;
	}

	if(lpTarget->PShopCustom == 0)
	{
		return 0;
	}

	if(lpTarget->PShopTransaction != 0)
	{
		gPersonalShop.GCPShopBuyItemSend(aIndex,bIndex,0,4);
		return 1;
	}

	if(INVENTORY_SHOP_RANGE(lpMsg->slot) == 0)
	{
		gPersonalShop.GCPShopBuyItemSend(aIndex,bIndex,0,5);
		return 1;
	}

	gObjFixInventoryPointer(aIndex);

	if(lpObj->Transaction == 1)
	{
		gPersonalShop.GCPShopBuyItemSend(aIndex,bIndex,0,6);
		return 1;
	}

	char name[11] = {0};

	memcpy(name,lpMsg->name,sizeof(lpMsg->name));
	
	if(strcmp(name,lpTarget->Name) != 0)
	{
		gPersonalShop.GCPShopBuyItemSend(aIndex,bIndex,0,6);
		return 1;
	}

	if(lpTarget->Inventory[lpMsg->slot].IsItem() == 0)
	{
		gPersonalShop.GCPShopBuyItemSend(aIndex,bIndex,0,6);
		return 1;
	}

	if(lpTarget->Inventory[lpMsg->slot].m_PShopValue <= 0)
	{
		gPersonalShop.GCPShopBuyItemSend(aIndex,bIndex,0,6);
		return 1;
	}

	if(lpTarget->PShopCustomType == 7 || lpTarget->PShopCustomType == 8 || lpTarget->PShopCustomType == 9)
	{
		#if(GAMESERVER_SHOP==1)
		gPcPoint.GDPcPointRecievePointSend(aIndex,(DWORD)&CCustomStore::OnPShopBuyItemCallbackRecv,(DWORD)&gObj[bIndex],lpMsg->slot);
		#elif(GAMESERVER_SHOP==2)
		gMuCashShop.GDMuCashPointRecievePointSend(aIndex,(DWORD)&CCustomStore::OnPShopBuyItemCallbackRecv,(DWORD)&gObj[bIndex],lpMsg->slot);
		#elif(GAMESERVER_SHOP==3)
		gCashShop.GDCashShopRecievePointSend(aIndex,(DWORD)&CCustomStore::OnPShopBuyItemCallbackRecv,(DWORD)&gObj[bIndex],lpMsg->slot);
		#endif

		return 1;
	}

	int PShopJoBValue = ((lpTarget->PShopCustomType==0)?lpTarget->Inventory[lpMsg->slot].m_PShopValue:0);

	int PShopJoSValue = ((lpTarget->PShopCustomType==1)?lpTarget->Inventory[lpMsg->slot].m_PShopValue:0);

	int PShopJoLValue = ((lpTarget->PShopCustomType==2)?lpTarget->Inventory[lpMsg->slot].m_PShopValue:0);

	int PShopJoCValue = ((lpTarget->PShopCustomType==3)?lpTarget->Inventory[lpMsg->slot].m_PShopValue:0);

	int PShopJCRValue = ((lpTarget->PShopCustomType==4)?lpTarget->Inventory[lpMsg->slot].m_PShopValue:0);
	
	int PShopJoGValue = ((lpTarget->PShopCustomType==5)?lpTarget->Inventory[lpMsg->slot].m_PShopValue:0);
	
	int PShopJoHValue = ((lpTarget->PShopCustomType==6)?lpTarget->Inventory[lpMsg->slot].m_PShopValue:0);

	int RequireJewelCount[7] = {0};

	int PaymentJewelCount[7] = {0};

	int RequireJewelTable[7][4] = {0};

	int PaymentJewelTable[7][4] = {0};

	gPersonalShop.GetRequireJewelCount(lpObj,&RequireJewelCount[0],RequireJewelTable[0],0,PShopJoBValue);

	gPersonalShop.GetRequireJewelCount(lpObj,&RequireJewelCount[1],RequireJewelTable[1],1,PShopJoSValue);

	gPersonalShop.GetRequireJewelCount(lpObj,&RequireJewelCount[2],RequireJewelTable[2],2,PShopJoLValue);

	gPersonalShop.GetRequireJewelCount(lpObj,&RequireJewelCount[3],RequireJewelTable[3],3,PShopJoCValue);

	gPersonalShop.GetRequireJewelCount(lpObj,&RequireJewelCount[4],RequireJewelTable[4],4,PShopJCRValue);

	gPersonalShop.GetRequireJewelCount(lpObj,&RequireJewelCount[5],RequireJewelTable[5],5,PShopJoGValue);

	gPersonalShop.GetRequireJewelCount(lpObj,&RequireJewelCount[6],RequireJewelTable[6],6,PShopJoHValue);

	gPersonalShop.GetPaymentJewelCount(lpTarget,&PaymentJewelCount[0],PaymentJewelTable[0],0,PShopJoBValue);

	gPersonalShop.GetPaymentJewelCount(lpTarget,&PaymentJewelCount[1],PaymentJewelTable[1],1,PShopJoSValue);

	gPersonalShop.GetPaymentJewelCount(lpTarget,&PaymentJewelCount[2],PaymentJewelTable[2],2,PShopJoLValue);

	gPersonalShop.GetPaymentJewelCount(lpTarget,&PaymentJewelCount[3],PaymentJewelTable[3],3,PShopJoCValue);

	gPersonalShop.GetPaymentJewelCount(lpTarget,&PaymentJewelCount[4],PaymentJewelTable[4],4,PShopJCRValue);

	gPersonalShop.GetPaymentJewelCount(lpTarget,&PaymentJewelCount[5],PaymentJewelTable[5],5,PShopJoGValue);

	gPersonalShop.GetPaymentJewelCount(lpTarget,&PaymentJewelCount[6],PaymentJewelTable[6],6,PShopJoHValue);


	if(RequireJewelCount[0] < PShopJoBValue)
	{
		gPersonalShop.GCPShopBuyItemSend(aIndex,bIndex,0,7);
		return 1;
	}

	if(RequireJewelCount[1] < PShopJoSValue)
	{
		gPersonalShop.GCPShopBuyItemSend(aIndex,bIndex,0,7);
		return 1;
	}

	if(RequireJewelCount[2] < PShopJoLValue)
	{
		gPersonalShop.GCPShopBuyItemSend(aIndex,bIndex,0,7);
		return 1;
	}

	if(RequireJewelCount[3] < PShopJoCValue)
	{
		gPersonalShop.GCPShopBuyItemSend(aIndex,bIndex,0,7);
		return 1;
	}

	if(RequireJewelCount[4] < PShopJCRValue)
	{
		gPersonalShop.GCPShopBuyItemSend(aIndex,bIndex,0,7);
		return 1;
	}

	if(RequireJewelCount[5] < PShopJoGValue)
	{
		gPersonalShop.GCPShopBuyItemSend(aIndex,bIndex,0,7);
		return 1;
	}

	if(RequireJewelCount[6] < PShopJoHValue)
	{
		gPersonalShop.GCPShopBuyItemSend(aIndex,bIndex,0,7);
		return 1;
	}

	if(PShopJoBValue > 0 && RequireJewelTable[0][0] == 0 && RequireJewelTable[0][1] == 0 && RequireJewelTable[0][2] == 0 && RequireJewelTable[0][3] == 0)
	{
		gPersonalShop.GCPShopBuyItemSend(aIndex,bIndex,0,7);
		return 1;
	}

	if(PShopJoSValue > 0 && RequireJewelTable[1][0] == 0 && RequireJewelTable[1][1] == 0 && RequireJewelTable[1][2] == 0 && RequireJewelTable[1][3] == 0)
	{
		gPersonalShop.GCPShopBuyItemSend(aIndex,bIndex,0,7);
		return 1;
	}

	if(PShopJoLValue > 0 && RequireJewelTable[2][0] == 0 && RequireJewelTable[2][1] == 0 && RequireJewelTable[2][2] == 0 && RequireJewelTable[2][3] == 0)
	{
		gPersonalShop.GCPShopBuyItemSend(aIndex,bIndex,0,7);
		return 1;
	}

	if(PShopJoCValue > 0 && RequireJewelTable[3][0] == 0 && RequireJewelTable[3][1] == 0 && RequireJewelTable[3][2] == 0 && RequireJewelTable[3][3] == 0)
	{
		gPersonalShop.GCPShopBuyItemSend(aIndex,bIndex,0,7);
		return 1;
	}

	if(PShopJCRValue > 0 && RequireJewelTable[4][0] == 0 && RequireJewelTable[4][1] == 0 && RequireJewelTable[4][2] == 0 && RequireJewelTable[4][3] == 0)
	{
		gPersonalShop.GCPShopBuyItemSend(aIndex,bIndex,0,7);
		return 1;
	}

	if(PShopJoGValue > 0 && RequireJewelTable[5][0] == 0 && RequireJewelTable[5][1] == 0 && RequireJewelTable[5][2] == 0 && RequireJewelTable[5][3] == 0)
	{
		gPersonalShop.GCPShopBuyItemSend(aIndex,bIndex,0,7);
		return 1;
	}

	if(PShopJoHValue > 0 && RequireJewelTable[6][0] == 0 && RequireJewelTable[6][1] == 0 && RequireJewelTable[6][2] == 0 && RequireJewelTable[6][3] == 0)
	{
		gPersonalShop.GCPShopBuyItemSend(aIndex,bIndex,0,7);
		return 1;
	}

	if(gItemManager.GetInventoryEmptySlotCount(lpTarget) < (PaymentJewelCount[0]+PaymentJewelCount[1]+PaymentJewelCount[2]))
	{
		gPersonalShop.GCPShopBuyItemSend(aIndex,bIndex,0,7);
		return 1;
	}

	lpTarget->PShopTransaction = 1;

	BYTE result = gItemManager.InventoryInsertItem(aIndex,lpTarget->Inventory[lpMsg->slot]);

	if(result == 0xFF)
	{
		gPersonalShop.GCPShopBuyItemSend(aIndex,bIndex,0,8);
		return 1;
	}

	gLog.Output(LOG_TRADE,"[SellPesonalShopItem][%s][%s] - (Account: %s,Name: %s,Value: %d,JoBValue: %d,JoSValue: %d,JoLValue: %d,JoCValue: %d,JCrValue: %d,JoGValue: %d,JoHValue: %d,Index: %04d,Level: %02d,Serial: %08X,Option1: %01d,Option2: %01d,Option3: %01d,NewOption: %03d,JewelOfHarmonyOption: %03d,ItemOptionEx: %03d,SocketOption: %03d,%03d,%03d,%03d,%03d)",lpTarget->Account,lpTarget->Name,lpObj->Account,0,PShopJoBValue,PShopJoSValue,PShopJoLValue,PShopJoCValue,PShopJCRValue,PShopJoGValue,PShopJoHValue,lpTarget->Inventory[lpMsg->slot].m_Index,lpTarget->Inventory[lpMsg->slot].m_Level,lpTarget->Inventory[lpMsg->slot].m_Serial,lpTarget->Inventory[lpMsg->slot].m_Option1,lpTarget->Inventory[lpMsg->slot].m_Option2,lpTarget->Inventory[lpMsg->slot].m_Option3,lpTarget->Inventory[lpMsg->slot].m_NewOption,lpTarget->Inventory[lpMsg->slot].m_JewelOfHarmonyOption,lpTarget->Inventory[lpMsg->slot].m_ItemOptionEx,lpTarget->Inventory[lpMsg->slot].m_SocketOption[0],lpTarget->Inventory[lpMsg->slot].m_SocketOption[1],lpTarget->Inventory[lpMsg->slot].m_SocketOption[2],lpTarget->Inventory[lpMsg->slot].m_SocketOption[3],lpTarget->Inventory[lpMsg->slot].m_SocketOption[4]);

	gPersonalShop.SetRequireJewelCount(lpObj,RequireJewelTable[0],0);

	gPersonalShop.SetRequireJewelCount(lpObj,RequireJewelTable[1],1);

	gPersonalShop.SetRequireJewelCount(lpObj,RequireJewelTable[2],2);

	gPersonalShop.SetRequireJewelCount(lpObj,RequireJewelTable[3],3);

	gPersonalShop.SetRequireJewelCount(lpObj,RequireJewelTable[4],4);

	gPersonalShop.SetRequireJewelCount(lpObj,RequireJewelTable[5],5);

	gPersonalShop.SetRequireJewelCount(lpObj,RequireJewelTable[6],6);

	gPersonalShop.GCPShopBuyItemSend(aIndex,bIndex,result,1);

	GDCharacterInfoSaveSend(aIndex);

	gPersonalShop.SetPaymentJewelCount(lpTarget,PaymentJewelTable[0],0);

	gPersonalShop.SetPaymentJewelCount(lpTarget,PaymentJewelTable[1],1);

	gPersonalShop.SetPaymentJewelCount(lpTarget,PaymentJewelTable[2],2);

	gPersonalShop.SetPaymentJewelCount(lpTarget,PaymentJewelTable[3],3);

	gPersonalShop.SetPaymentJewelCount(lpTarget,PaymentJewelTable[4],4);

	gPersonalShop.SetPaymentJewelCount(lpTarget,PaymentJewelTable[5],5);

	gPersonalShop.SetPaymentJewelCount(lpTarget,PaymentJewelTable[6],6);

	gPersonalShop.GCPShopSellItemSend(bIndex,aIndex,lpMsg->slot);

	gItemManager.InventoryDelItem(bIndex,lpMsg->slot);
	gItemManager.GCItemDeleteSend(bIndex,lpMsg->slot,1);

	GDCharacterInfoSaveSend(bIndex);

	if(gPersonalShop.CheckPersonalShop(bIndex) == 0)
	{
		lpTarget->PShopItemChange = 1;
	}
	else
	{
		lpTarget->PShopOpen = 0;
		memset(lpTarget->PShopText,0,sizeof(lpTarget->PShopText));
		gPersonalShop.GCPShopCloseSend(bIndex,1);
		this->OnPShopClose(lpTarget);
	}

	lpTarget->PShopTransaction = 0;

	return 1;
}

void CCustomStore::OnPShopBuyItemCallbackRecv(LPOBJ lpObj,LPOBJ lpTarget,DWORD slot,DWORD Coin1,DWORD Coin2,DWORD Coin3) // OK
{
	if(gObjIsConnectedGP(lpTarget->Index) == 0)
	{
		gPersonalShop.GCPShopBuyItemSend(lpObj->Index,lpTarget->Index,0,3);
		return;
	}

	if(lpTarget->PShopOpen == 0)
	{
		gPersonalShop.GCPShopBuyItemSend(lpObj->Index,lpTarget->Index,0,3);
		return;
	}

	if(lpTarget->PShopTransaction != 0)
	{
		gPersonalShop.GCPShopBuyItemSend(lpObj->Index,lpTarget->Index,0,4);
		return;
	}

	if(lpTarget->Inventory[slot].IsItem() == 0)
	{
		gPersonalShop.GCPShopBuyItemSend(lpObj->Index,lpTarget->Index,0,6);
		return;
	}

	DWORD PShopCoin1Value = ((lpTarget->PShopCustomType==7)?lpTarget->Inventory[slot].m_PShopValue:0);

	DWORD PShopCoin2Value = ((lpTarget->PShopCustomType==8)?lpTarget->Inventory[slot].m_PShopValue:0);

	DWORD PShopCoin3Value = ((lpTarget->PShopCustomType==9)?lpTarget->Inventory[slot].m_PShopValue:0);

	#if(GAMESERVER_SHOP==3)

	if(Coin1 < PShopCoin1Value)
	{
		gPersonalShop.GCPShopBuyItemSend(lpObj->Index,lpTarget->Index,0,7);
		return;
	}

	if(Coin2 < PShopCoin2Value)
	{
		gPersonalShop.GCPShopBuyItemSend(lpObj->Index,lpTarget->Index,0,7);
		return;
	}

	if(Coin3 < PShopCoin3Value)
	{
		gPersonalShop.GCPShopBuyItemSend(lpObj->Index,lpTarget->Index,0,7);
		return;
	}

	#else

	if(Coin3 < PShopCoin1Value)
	{
		gPersonalShop.GCPShopBuyItemSend(lpObj->Index,lpTarget->Index,0,7);
		return;
	}

	#if(GAMESERVER_SHOP==1)
	
		if((int)(lpTarget->Coin1+PShopCoin1Value) > gServerInfo.m_PcPointMaxPoint)
		{
			gPersonalShop.GCPShopBuyItemSend(lpObj->Index,lpTarget->Index,0,7);
			return;
		}

	#elif(GAMESERVER_SHOP==2)

		if((int)(lpTarget->Coin1+PShopCoin1Value) > gServerInfo.m_CashShopMaxPoint)
		{
			gPersonalShop.GCPShopBuyItemSend(lpObj->Index,lpTarget->Index,0,7);
			return;
		}

	#endif

	#endif

	lpTarget->PShopTransaction = 1;

	BYTE result = gItemManager.InventoryInsertItem(lpObj->Index,lpTarget->Inventory[slot]);

	if(result == 0xFF)
	{
		gPersonalShop.GCPShopBuyItemSend(lpObj->Index,lpTarget->Index,0,8);
		return;
	}

	gLog.Output(LOG_TRADE,"[SellPesonalShopItem][%s][%s] - (Account: %s,Name: %s,Value: %d,Coin1: %d,Coin2: %d,Coin3: %d,Index: %04d,Level: %02d,Serial: %08X,Option1: %01d,Option2: %01d,Option3: %01d,NewOption: %03d,JewelOfHarmonyOption: %03d,ItemOptionEx: %03d,SocketOption: %03d,%03d,%03d,%03d,%03d)",lpTarget->Account,lpTarget->Name,lpObj->Account,0,PShopCoin1Value,PShopCoin2Value,PShopCoin3Value,lpTarget->Inventory[slot].m_Index,lpTarget->Inventory[slot].m_Level,lpTarget->Inventory[slot].m_Serial,lpTarget->Inventory[slot].m_Option1,lpTarget->Inventory[slot].m_Option2,lpTarget->Inventory[slot].m_Option3,lpTarget->Inventory[slot].m_NewOption,lpTarget->Inventory[slot].m_JewelOfHarmonyOption,lpTarget->Inventory[slot].m_ItemOptionEx,lpTarget->Inventory[slot].m_SocketOption[0],lpTarget->Inventory[slot].m_SocketOption[1],lpTarget->Inventory[slot].m_SocketOption[2],lpTarget->Inventory[slot].m_SocketOption[3],lpTarget->Inventory[slot].m_SocketOption[4]);

	gObjCoinSub(lpObj->Index,PShopCoin1Value,PShopCoin2Value,PShopCoin3Value);

	gObjCoinAdd(lpTarget->Index,PShopCoin1Value,PShopCoin2Value,PShopCoin3Value);

	gPersonalShop.GCPShopBuyItemSend(lpObj->Index,lpTarget->Index,result,1);

	GDCharacterInfoSaveSend(lpObj->Index);

	gPersonalShop.GCPShopSellItemSend(lpTarget->Index,lpObj->Index,slot);

	gItemManager.InventoryDelItem(lpTarget->Index,(BYTE)slot);
	gItemManager.GCItemDeleteSend(lpTarget->Index,(BYTE)slot,1);

	GDCharacterInfoSaveSend(lpTarget->Index);

	if(gPersonalShop.CheckPersonalShop(lpTarget->Index) == 0)
	{
		lpTarget->PShopItemChange = 1;
	}
	else
	{
		lpTarget->PShopOpen = 0;
		memset(lpTarget->PShopText,0,sizeof(lpTarget->PShopText));
		gPersonalShop.GCPShopCloseSend(lpTarget->Index,1);
		gCustomStore.OnPShopClose(lpTarget);
	}

	lpTarget->PShopTransaction = 0;
}