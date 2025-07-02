// GoldenArcher.cpp: implementation of the CGoldenArcher class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GoldenArcher.h"
#include "DSProtocol.h"
#include "GameMain.h"
#include "ItemManager.h"
#include "MemScript.h"
#include "Util.h"

CGoldenArcher gGoldenArcher;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void CGoldenArcher::CGGoldenArcherCountRecv(int aIndex) // OK
{
	#if(GAMESERVER_UPDATE<=603)

	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(((lpObj->GoldenArcherTransaction == 0)?(lpObj->GoldenArcherTransaction++):lpObj->GoldenArcherTransaction) != 0)
	{
		return;
	}

	SDHP_GOLDEN_ARCHER_COUNT_SEND pMsg;

	pMsg.header.set(0x94,0x00,sizeof(pMsg));

	pMsg.index = lpObj->Index;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,lpObj->Index);

	#endif
}

void CGoldenArcher::CGGoldenArcherRegisterRecv(PMSG_GOLDEN_ARCHER_COUNT_RECV* lpMsg,int aIndex) // OK
{
	#if(GAMESERVER_UPDATE<=603)

	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(gItemManager.GetInventoryItemCount(lpObj,GET_ITEM(14,21),0) != 0)
	{
		gItemManager.DeleteInventoryItemCount(lpObj,GET_ITEM(14,21),0,1);

		this->GDGoldenArcherAddCountSaveSend(aIndex,1);

		this->CGGoldenArcherCountRecv(aIndex);
	}
	
	#endif
}

void CGoldenArcher::CGGoldenArcherCloseRecv(int aIndex) // OK
{
	#if(GAMESERVER_UPDATE<=603)

	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(lpObj->Interface.use == 0 || lpObj->Interface.type == INTERFACE_NONE || lpObj->Interface.type != INTERFACE_GOLDEN_ARCHER)
	{
		return;
	}

	lpObj->Interface.use = 0;
	lpObj->Interface.type = INTERFACE_NONE;
	lpObj->Interface.state = 0;

	#endif
}

void CGoldenArcher::DGGoldenArcherCountRecv(SDHP_GOLDEN_ARCHER_COUNT_RECV* lpMsg) // OK
{
	#if(GAMESERVER_UPDATE<=603)

	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGGoldenArcherCountRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	lpObj->GoldenArcherTransaction = 0;

	PMSG_GOLDEN_ARCHER_COUNT_SEND pMsg;

	pMsg.header.set(0x94,sizeof(pMsg));

	pMsg.type = 0;

	pMsg.count = lpMsg->count;

	memset(pMsg.code,0,sizeof(pMsg.code));

	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);

	#endif
}

void CGoldenArcher::GDGoldenArcherAddCountSaveSend(int aIndex, DWORD count) // OK
{
	#if(GAMESERVER_UPDATE<=603)

	SDHP_GOLDEN_ARCHER_ADD_COUNT_SAVE_SEND pMsg;

	pMsg.header.set(0x94,0x30,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

	pMsg.count = count;

	gDataServerConnection.DataSend((BYTE*)&pMsg,sizeof(pMsg),aIndex);

	#endif
}