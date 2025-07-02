// ItemBag.cpp: implementation of the CItemBag class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ItemBag.h"
#include "CustomWing.h"
#include "DSProtocol.h"
#include "ExcellentOptionRate.h"
#include "ItemOptionRate.h"
#include "ItemManager.h"
#include "JewelOfHarmonyOption.h"
#include "Log.h"
#include "Map.h"
#include "MemScript.h"
#include "Protocol.h"
#include "ServerInfo.h"
#include "SetItemType.h"
#include "SocketItemType.h"
#include "Util.h"
#include "WingOption.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemBag::CItemBag() // OK
{
	this->Init();
}

CItemBag::~CItemBag() // OK
{

}

void CItemBag::Init() // OK
{
	memset(this->m_EventName,0,sizeof(this->m_EventName));

	this->m_DropZen = 0;
	this->m_ItemDropRate = 0;
	this->m_MinItemDropCount = 0;
	this->m_MaxItemDropCount = 0;
	this->m_SetItemDropRate = 0;
	this->m_ItemDropType = 0;
	this->m_BagUseEffect = 0;
	this->m_count = 0;
}

void CItemBag::Load(char* path) // OK
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

	this->Init();

	try
	{
		while(true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			int section = lpMemScript->GetNumber();

			while(true)
			{
				if(section == 0)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					strcpy_s(this->m_EventName,lpMemScript->GetString());

					this->m_DropZen = lpMemScript->GetAsNumber();

					this->m_ItemDropRate = lpMemScript->GetAsNumber();

					this->m_MinItemDropCount = lpMemScript->GetAsNumber();

					this->m_MaxItemDropCount = lpMemScript->GetAsNumber();

					this->m_SetItemDropRate = lpMemScript->GetAsNumber();

					this->m_ItemDropType = lpMemScript->GetAsNumber();

					this->m_BagUseEffect = lpMemScript->GetAsNumber();
				}
				else if(section == 1)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					ITEM_BAG_INFO info;

					memset(&info,0,sizeof(info));

					info.ItemIndex = SafeGetItem(GET_ITEM(lpMemScript->GetNumber(),lpMemScript->GetAsNumber()));

					info.MinLevel = lpMemScript->GetAsNumber();

					info.MaxLevel = lpMemScript->GetAsNumber();

					info.Option1 = lpMemScript->GetAsNumber();

					info.Option2 = lpMemScript->GetAsNumber();

					info.Option3 = lpMemScript->GetAsNumber();

					info.NewOption = lpMemScript->GetAsNumber();

					info.SetOption = lpMemScript->GetAsNumber();

					#if(GAMESERVER_UPDATE>=401)

					info.SocketOption = lpMemScript->GetAsNumber();

					#endif

					this->SetInfo(info);
				}
				else
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}
				}
			}
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	this->m_ItemBagEx.Load(path);

	delete lpMemScript;
}

void CItemBag::SetInfo(ITEM_BAG_INFO info) // OK
{
	if(this->m_count < 0 || this->m_count >= MAX_BAG_ITEM)
	{
		return;
	}

	this->m_BagInfo[this->m_count++] = info;
}

ITEM_BAG_INFO* CItemBag::GetInfo(int index) // OK
{
	if(index < 0 || index >= this->m_count)
	{
		return 0;
	}

	return &this->m_BagInfo[index];
}

bool CItemBag::GetItem(LPOBJ lpObj,CItem* lpItem) // OK
{
	if(this->m_ItemBagEx.m_ItemBagInfo.empty() == 0)
	{
		return this->m_ItemBagEx.GetItem(lpObj,lpItem);
	}

	if(this->m_count == 0)
	{
		return 0;
	}

	ITEM_BAG_INFO* lpInfo = this->GetInfo(GetLargeRand()%this->m_count);

	if(lpInfo == 0)
	{
		return 0;
	}

	int Option1 = lpInfo->Option1;
	int Option2 = lpInfo->Option2;
	int Option3 = lpInfo->Option3;
	int SetOption = lpInfo->SetOption;
	int SocketSlot = lpInfo->SocketOption;

	if(this->m_ItemDropType != 0)
	{
		if(Option1 != 0)
		{
			Option1 = GetLargeRand()%2;
		}

		if(Option2 != 0)
		{
			Option2 = GetLargeRand()%2;
		}

		if(Option3 != 0)
		{
			Option3 = 1+(GetLargeRand()%gServerInfo.m_MaxItemOption);

			if((GetLargeRand()%100) < (Option3*3))
			{
				Option3 = (gServerInfo.m_MaxItemOption+1)-Option3;
			}
			else
			{
				Option3 = 0;
			}
		}

		if(SetOption != 0)
		{
			SetOption = GetLargeRand()%2;
		}

		if(SocketSlot != 0)
		{
			SocketSlot = GetLargeRand()%5;
		}
	}

	gItemOptionRate.MakeLevelOption(lpInfo->MinLevel,lpInfo->MaxLevel,&lpItem->m_Level);
	gItemOptionRate.MakeNewOption(lpInfo->ItemIndex,this->m_ItemDropType,lpInfo->NewOption,&lpItem->m_NewOption);
	gItemOptionRate.MakeSetOption(lpInfo->ItemIndex,SetOption,&lpItem->m_SetOption);
	gItemOptionRate.MakeSocketSlot(lpInfo->ItemIndex,SocketSlot,&lpItem->m_SocketOption[0]);

	lpItem->Convert(lpInfo->ItemIndex,Option1,Option2,Option3,lpItem->m_NewOption,lpItem->m_SetOption,0,0,lpItem->m_SocketOption,0xFF);

	return 1;
}

bool CItemBag::DropItem(LPOBJ lpObj,int map,int x,int y) // OK
{
	if(this->m_ItemBagEx.m_ItemBagInfo.empty() == 0)
	{
		return this->m_ItemBagEx.DropItem(lpObj,map,x,y);
	}

	int DropCount = (this->m_MinItemDropCount+(GetLargeRand()%((this->m_MaxItemDropCount-this->m_MinItemDropCount)+1)));

	for(int n=0;n < DropCount;n++)
	{
		int px = x;
		int py = y;

		if(DropCount > 1 || (gMap[map].CheckAttr(px,py,4) != 0 || gMap[map].CheckAttr(px,py,8) != 0))
		{
			if(gObjGetRandomFreeLocation(map,&px,&py,2,2,10) == 0)
			{
				px = lpObj->X;
				py = lpObj->Y;
			}
		}

		if((GetLargeRand()%100) >= this->m_ItemDropRate)
		{
			gMap[map].MoneyItemDrop(this->m_DropZen,px,py);
			continue;
		}

		if((GetLargeRand()%10000) < this->m_SetItemDropRate)
		{
			gSetItemType.MakeRandomSetItem(lpObj->Index,map,px,py);
			continue;
		}

		CItem item;

		if(this->GetItem(lpObj,&item) != 0)
		{
			GDCreateItemSend(lpObj->Index,map,px,py,item.m_Index,(BYTE)item.m_Level,0,item.m_Option1,item.m_Option2,item.m_Option3,lpObj->Index,item.m_NewOption,item.m_SetOption,item.m_JewelOfHarmonyOption,item.m_ItemOptionEx,item.m_SocketOption,item.m_SocketOptionBonus,0);
			continue;
		}
	}

	switch(this->m_BagUseEffect)
	{
		case 1:
			GCServerCommandSend(lpObj->Index,0,lpObj->X,lpObj->Y,1);
			break;
		case 2:
			GCServerCommandSend(lpObj->Index,2,lpObj->X,lpObj->Y,1);
			break;
		case 3:
			GCServerCommandSend(lpObj->Index,58,SET_NUMBERHB(lpObj->Index),SET_NUMBERLB(lpObj->Index),1);
			break;
	}

	return 1;
}
