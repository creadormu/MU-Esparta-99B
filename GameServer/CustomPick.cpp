// CustomPick.cpp: implementation of the CCustomPick class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomPick.h"
#include "CommandManager.h"
#include "GameMain.h"
#include "ItemLevel.h"
#include "ItemManager.h"
#include "Log.h"
#include "Map.h"
#include "MapManager.h"
#include "MemScript.h"
#include "Message.h"
#include "Notice.h"
#include "Util.h"
#include "Viewport.h"

CCustomPick gCustomPick;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomPick::CCustomPick() // OK
{
	this->m_CustomPickInfo.clear();
}

CCustomPick::~CCustomPick() // OK
{

}

void CCustomPick::ReadCustomPickInfo(char* section,char* path) // OK
{
	this->m_CustomPickSwitch = GetPrivateProfileInt(section,"CustomPickSwitch",0,path);

	this->m_CustomPickMapZone = GetPrivateProfileInt(section,"CustomPickMapZone",0,path);

	if(this->m_CustomPickMapZone != -1)
	{
		memset(this->m_CustomPickMapList,0,sizeof(this->m_CustomPickMapList));

		char buff[256] = {0};

		GetPrivateProfileString(section,"CustomPickMapZone","",buff,sizeof(buff),path);

		char* next_token;

		char* token = strtok_s(buff,",",&next_token);
	
		while(token != 0)
		{
			if(gMapManager.IsValidMap(atoi(token)) != 0)
			{
				this->m_CustomPickMapList[atoi(token)] = 1;
			}

			token = strtok_s(0,",",&next_token);
		}
	}

	this->m_CustomPickMaxTime[0] = GetPrivateProfileInt(section,"CustomPickMaxTime_AL0",0,path);

	this->m_CustomPickMaxTime[1] = GetPrivateProfileInt(section,"CustomPickMaxTime_AL1",0,path);

	this->m_CustomPickMaxTime[2] = GetPrivateProfileInt(section,"CustomPickMaxTime_AL2",0,path);

	this->m_CustomPickMaxTime[3] = GetPrivateProfileInt(section,"CustomPickMaxTime_AL3",0,path);
}

void CCustomPick::Load(char* path) // OK
{
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

	this->m_CustomPickInfo.clear();

	try
	{
		while(true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			if(strcmp("end",lpMemScript->GetString()) == 0)
			{
				break;
			}

			CUSTOM_PICK_INFO info;

			info.ItemIndex = SafeGetItem(GET_ITEM(lpMemScript->GetNumber(),lpMemScript->GetAsNumber()));

			info.ItemLevel = lpMemScript->GetAsNumber();

			this->m_CustomPickInfo.push_back(info);
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CCustomPick::MainProc(LPOBJ lpObj) // OK
{
	if(lpObj->Pick.Started == 0)
	{
		return;
	}

	if(this->m_CustomPickMaxTime[lpObj->AccountLevel] != 0 && (GetTickCount()-((DWORD)lpObj->Pick.Timer)) >= ((DWORD)this->m_CustomPickMaxTime[lpObj->AccountLevel]*1000))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(458));
		this->OnPickClose(lpObj);
		return;
	}

	if(gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,1) != 0)
	{
		this->OnPickClose(lpObj);
		return;
	}

	if(this->m_CustomPickMapZone != -1 && this->m_CustomPickMapList[lpObj->Map] == 0)
	{
		this->OnPickClose(lpObj);
		return;
	}

	for(int n=0;n<MAX_MAP_ITEM;n++)
	{
		CMapItem* lpItem = &gMap[lpObj->Map].m_Item[n];

		if(lpItem->IsItem() == 0)
		{
			continue;
		}

		if((GetTickCount()-lpItem->m_CreateTime) < 1000)
		{
			continue;
		}

		if(this->CheckItem(lpItem->m_Index,lpItem->m_Level) != 0)
		{
			continue;
		}

		if(gViewport.CheckViewportObjectPosition(lpObj->Index,lpObj->Map,lpItem->m_X,lpItem->m_Y,5) != 0)
		{
			PMSG_ITEM_GET_RECV pMsg;

			pMsg.header.set(0x22,sizeof(pMsg));

			pMsg.index[0] = SET_NUMBERHB(n);
			pMsg.index[1] = SET_NUMBERLB(n);

			if(lpObj->Pick.PickJewel != 0 && gItemManager.IsJewelItem(lpItem->m_Index) != 0)
			{
				gItemManager.CGItemGetRecv(&pMsg,lpObj->Index);
				continue;
			}
				
			if(lpObj->Pick.PickSet != 0 && lpItem->IsSetItem() != 0)
			{
				gItemManager.CGItemGetRecv(&pMsg,lpObj->Index);
				continue;
			}

			if(lpObj->Pick.PickExc != 0 && lpItem->IsExcItem() != 0)
			{
				gItemManager.CGItemGetRecv(&pMsg,lpObj->Index);
				continue;
			}

			if(lpObj->Pick.PickMoney != 0 && lpItem->m_Index == GET_ITEM(14,15))
			{
				gItemManager.CGItemGetRecv(&pMsg,lpObj->Index);
				continue;
			}

			if(lpObj->Pick.ItemCount > 0)
			{
				char buff[256] = { 0 };

				memcpy(buff,CharToLower(gItemManager.GetItemName(lpItem->m_Index,lpItem->m_Level)),sizeof(buff));

				if(lpItem->m_Level > 0 && gItemLevel.CheckItemlevel(lpItem->m_Index,lpItem->m_Level) == 0)
				{
					wsprintf(buff,"%s +%d",buff,lpItem->m_Level);
				}

				if(lpItem->m_Option1 != 0)
				{
					wsprintf(buff,"%s +skill",buff);
				}

				if(lpItem->m_Option2 != 0)
				{
					wsprintf(buff,"%s +luck",buff);
				}

				if(lpItem->m_Option3 != 0)
				{
					wsprintf(buff,"%s +option",buff);
				}

				if(lpItem->IsSocketItem() != 0)
				{
					wsprintf(buff,"%s +socket%x",buff,GetSocketOptionCount(lpItem->m_SocketOption));
				}

				for(int i=0;i < 20;i++) 
				{ 
					if(strlen(lpObj->Pick.ItemList[i]) < 1)
					{
						continue;
					}

					if(strstr(buff,lpObj->Pick.ItemList[i]) != 0)
					{
						gItemManager.CGItemGetRecv(&pMsg,lpObj->Index);
						break;
					}
				}
			}
		}
	};
}

void CCustomPick::CommandCustomPick(LPOBJ lpObj,char* arg) // OK
{
	if(this->m_CustomPickSwitch == 0)
	{
		return;
	}

	if(lpObj->Pick.Started != 0)
	{
		this->OnPickClose(lpObj);
		return;
	}

	if(gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,1) != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(450));
		return;
	}

	if(this->m_CustomPickMapZone != -1 && this->m_CustomPickMapList[lpObj->Map] == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(547));
		return;
	}

	if(strcmp(arg,"") != 0)
	{
		lpObj->Pick.PickMoney = gCommandManager.GetNumber(arg,0);

		lpObj->Pick.PickJewel = gCommandManager.GetNumber(arg,1);

		lpObj->Pick.PickExc = gCommandManager.GetNumber(arg,2);

		lpObj->Pick.PickSet = gCommandManager.GetNumber(arg,3);
	}

	if(lpObj->Pick.ItemCount == 0 && lpObj->Pick.PickMoney == 0 && lpObj->Pick.PickJewel == 0 && lpObj->Pick.PickExc == 0 && lpObj->Pick.PickSet == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(451));
		return;
	}

	lpObj->Pick.Started = 1;

	lpObj->Pick.Timer = GetTickCount();

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(452),lpObj->Pick.ItemCount,lpObj->Pick.PickMoney,lpObj->Pick.PickJewel,lpObj->Pick.PickExc,lpObj->Pick.PickSet);

	gLog.Output(LOG_COMMAND,"[CommandCustomPick][%s][%s] - Active (ItemCount: %d, Zen: %d, Jewel: %d, Exc: %d, Set: %d)",lpObj->Account,lpObj->Name,lpObj->Pick.ItemCount,lpObj->Pick.PickMoney,lpObj->Pick.PickJewel,lpObj->Pick.PickExc,lpObj->Pick.PickSet);

	gCommandManager.DiscountRequirement(lpObj,COMMAND_CUSTOM_PICK);
}

void CCustomPick::CommandCustomAdd(LPOBJ lpObj,char* arg) // OK
{
	if(this->m_CustomPickSwitch == 0)
	{
		return;
	}

	if(strcmp(arg,"") == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(454));
		return;
	}

	char syntax[20] = {0};

	gCommandManager.GetString(arg,syntax,sizeof(syntax),0);

	if(strlen(syntax) < 3)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(459));
		return;
	}

	if(lpObj->Pick.ItemCount >= 20)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(460));
		return;
	}

	strcpy_s(lpObj->Pick.ItemList[lpObj->Pick.ItemCount],CharToLower(syntax));

	lpObj->Pick.ItemCount++;
	
	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(455),lpObj->Pick.ItemCount,syntax);

	gLog.Output(LOG_COMMAND,"[CommandCustomPickAdd][%s][%s] - Set (Slot: %d Text: %d)",lpObj->Account,lpObj->Name,lpObj->Pick.ItemCount,syntax);

	gCommandManager.DiscountRequirement(lpObj,COMMAND_CUSTOM_PICK_ADD);
}

void CCustomPick::CommandCustomClear(LPOBJ lpObj) // OK
{
	if(this->m_CustomPickSwitch == 0)
	{
		return;
	}

	this->OnPickClose(lpObj);

	lpObj->Pick.ItemCount = 0;

	memset(lpObj->Pick.ItemList,0,sizeof(lpObj->Pick.ItemList));

	lpObj->Pick.PickMoney = 0;
	
	lpObj->Pick.PickJewel = 0;

	lpObj->Pick.PickExc = 0;
	
	lpObj->Pick.PickSet = 0;

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(456));

	gLog.Output(LOG_COMMAND,"[CommandCustomPickClear][%s][%s] - Reset config",lpObj->Account,lpObj->Name);

	gCommandManager.DiscountRequirement(lpObj,COMMAND_CUSTOM_PICK_CLEAR);
}

bool CCustomPick::CheckItem(int ItemIndex,int ItemLevel) // OK
{
	for each(CUSTOM_PICK_INFO lpInfo in this->m_CustomPickInfo)
	{
		if(lpInfo.ItemIndex != ItemIndex)
		{
			continue;
		}

		if(lpInfo.ItemLevel != -1 && lpInfo.ItemLevel != ItemLevel)
		{
			continue;
		}

		return 1;
	}

	return 0;
}

void CCustomPick::OnPickClose(LPOBJ lpObj) // OK
{
	if(lpObj->Pick.Started != 0)
	{
		lpObj->Pick.Started = 0;
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(453));
	}
}

void CCustomPick::DGCustomPickRecv(SDHP_CUSTOM_PICK_RECV* lpMsg) // OK
{
	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGCustomPickRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	memcpy(lpObj->Pick.ItemList,lpMsg->data,sizeof(lpMsg->data));

	for(int i=0;i < 20;i++)
	{
		if(strcmp(lpObj->Pick.ItemList[i],"") != 0)
		{
			lpObj->Pick.ItemCount++;
		}
	}

	lpObj->Pick.PickMoney = lpMsg->PickMoney;

	lpObj->Pick.PickJewel = lpMsg->PickJewel;

	lpObj->Pick.PickExc = lpMsg->PickExc;

	lpObj->Pick.PickSet = lpMsg->PickSet;
}

void CCustomPick::GDCustomPickSend(int aIndex) // OK
{
	if(gObjIsAccountValid(aIndex,gObj[aIndex].Account) == 0)
	{
		return;
	}

	SDHP_CUSTOM_PICK_SEND pMsg;

	pMsg.header.set(0x13,0x00,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

	memcpy(pMsg.name,gObj[aIndex].Name,sizeof(pMsg.name));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);
}

void CCustomPick::GDCustomPickSaveSend(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	SDHP_CUSTOM_PICK_SAVE_SEND pMsg;

	pMsg.header.set(0x13,0x30,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpObj->Name,sizeof(pMsg.name));

	memcpy(pMsg.data,lpObj->Pick.ItemList,sizeof(pMsg.data));

	pMsg.PickMoney = lpObj->Pick.PickMoney;

	pMsg.PickJewel = lpObj->Pick.PickJewel;

	pMsg.PickExc = lpObj->Pick.PickExc;

	pMsg.PickSet = lpObj->Pick.PickSet;

	gDataServerConnection.DataSend((BYTE*)&pMsg,sizeof(pMsg),aIndex);
}