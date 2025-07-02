// SetItemType.cpp: implementation of the CSetItemType class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SetItemType.h"
#include "DSProtocol.h"
#include "ItemManager.h"
#include "ItemOptionRate.h"
#include "Log.h"
#include "MapRateInfo.h"
#include "MemScript.h"
#include "RandomManager.h"
#include "ServerInfo.h"
#include "Util.h"

CSetItemType gSetItemType;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSetItemType::CSetItemType() // OK
{
	this->m_SetItemTypeInfo.clear();
}

CSetItemType::~CSetItemType() // OK
{

}

void CSetItemType::Load(char* path) // OK
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

	this->m_SetItemTypeInfo.clear();

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

			SET_ITEM_TYPE_INFO info;

			memset(&info,0,sizeof(info));

			info.Index = SafeGetItem(GET_ITEM(lpMemScript->GetNumber(),lpMemScript->GetAsNumber()));

			info.StatType = lpMemScript->GetAsNumber();

			for(int n = 0; n < MAX_SET_ITEM_OPTION_INDEX; n++)
			{
				info.OptionIndex[n] = lpMemScript->GetAsNumber();
			}

			info.DropRate = lpMemScript->GetAsNumber();

			this->m_SetItemTypeInfo.insert(std::pair<int,SET_ITEM_TYPE_INFO>(info.Index,info));
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

SET_ITEM_TYPE_INFO* CSetItemType::GetInfo(int index) // OK
{
	std::map<int,SET_ITEM_TYPE_INFO>::iterator it = this->m_SetItemTypeInfo.find(index);

	if(it == this->m_SetItemTypeInfo.end())
	{
		return 0;
	}
	else
	{
		return &it->second;
	}
}

bool CSetItemType::CheckSetItemType(int index) // OK
{
	return ((this->GetInfo(index) == 0) ? 0 : 1);
}

int CSetItemType::GetSetItemOptionIndex(int index,int number) // OK
{
	SET_ITEM_TYPE_INFO* lpInfo = this->GetInfo(index);

	if(lpInfo == 0)
	{
		return -1;
	}

	if(number < 0 || number >= MAX_SET_ITEM_OPTION_INDEX)
	{
		return -1;
	}

	return lpInfo->OptionIndex[number];
}

int CSetItemType::GetSetItemStatType(int index) // OK
{
	SET_ITEM_TYPE_INFO* lpInfo = this->GetInfo(index);

	if(lpInfo == 0)
	{
		return -1;
	}

	return lpInfo->StatType;
}

int CSetItemType::GetRandomSetItem() // OK
{
	CRandomManager RandomManager;

	WORD ItemIndex = -1;

	for(std::map<int,SET_ITEM_TYPE_INFO>::iterator it = this->m_SetItemTypeInfo.begin(); it != this->m_SetItemTypeInfo.end(); it++)
	{
		ITEM_INFO ItemInfo;

		if(gItemManager.GetInfo(it->second.Index,&ItemInfo) != 0 && ItemInfo.DropItem != 0)
		{
			RandomManager.AddElement(it->second.Index,it->second.DropRate);
		}
	}

	RandomManager.GetRandomElement(&ItemIndex);

	return ItemIndex;
}

void CSetItemType::MakeRandomSetItem(int aIndex,int map,int x,int y) // OK
{
	WORD ItemIndex = this->GetRandomSetItem();

	if(ItemIndex == -1)
	{
		return;
	}

	BYTE ItemLevel = 0;
	BYTE ItemOption1 = 0;
	BYTE ItemOption2 = 0;
	BYTE ItemOption3 = 0;
	BYTE ItemNewOption = 0;
	BYTE ItemSetOption = 0;
	BYTE ItemSocketOption[MAX_SOCKET_OPTION] = { 0xFF,0xFF,0xFF,0xFF,0xFF };

	gItemOptionRate.GetItemOption0(gMapRateInfo.GetDropOption0(map,DROP_TYPE_ANCIENT),&ItemLevel);

	gItemOptionRate.GetItemOption1(gMapRateInfo.GetDropOption1(map,DROP_TYPE_ANCIENT),&ItemOption1);

	gItemOptionRate.GetItemOption2(gMapRateInfo.GetDropOption2(map,DROP_TYPE_ANCIENT),&ItemOption2);

	gItemOptionRate.GetItemOption3(gMapRateInfo.GetDropOption3(map,DROP_TYPE_ANCIENT),&ItemOption3);

	gItemOptionRate.GetItemOption4(gMapRateInfo.GetDropOption4(map,DROP_TYPE_ANCIENT),&ItemNewOption);

	gItemOptionRate.GetItemOption5(gMapRateInfo.GetDropOption5(map,DROP_TYPE_ANCIENT),&ItemSetOption);

	gItemOptionRate.GetItemOption6(gMapRateInfo.GetDropOption6(map,DROP_TYPE_ANCIENT),&ItemSocketOption[0]);

	gItemOptionRate.MakeNewOption(ItemIndex,1,ItemNewOption,&ItemNewOption);

	gItemOptionRate.MakeSetOption(ItemIndex,ItemSetOption,&ItemSetOption);

	gItemOptionRate.MakeSocketOption(ItemIndex,ItemSocketOption[0],&ItemSocketOption[0]);

	GDCreateItemSend(aIndex,map,x,y,ItemIndex,ItemLevel,0,ItemOption1,ItemOption2,ItemOption3,aIndex,ItemNewOption,ItemSetOption,0,0,ItemSocketOption,0xFF,0);
}
