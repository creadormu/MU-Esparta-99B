// ItemLoot.cpp: implementation of the CItemLoot class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ItemLoot.h"
#include "ItemManager.h"
#include "MemScript.h"
#include "Util.h"

CItemLoot gItemLoot;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemLoot::CItemLoot() // OK
{
	this->m_ItemLootInfo.clear();
}

CItemLoot::~CItemLoot() // OK
{

}

void CItemLoot::Load(char* path) // OK
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

	this->m_ItemLootInfo.clear();

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

			ITEM_LOOT_INFO info;

			info.ItemIndex = SafeGetItem(GET_ITEM(lpMemScript->GetNumber(),lpMemScript->GetAsNumber()));

			info.Level = lpMemScript->GetAsNumber();

			info.AllowLoot = lpMemScript->GetAsNumber();

			this->m_ItemLootInfo.push_back(info);
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

bool CItemLoot::CheckItemAllowLoot(int index,int level) // OK
{
	for(std::vector<ITEM_LOOT_INFO>::iterator it = this->m_ItemLootInfo.begin(); it != this->m_ItemLootInfo.end(); it++)
	{
		if(it->ItemIndex != index)
		{
			continue;
		}

		if(it->Level != -1 && it->Level != level)
		{
			continue;
		}

		return (it->AllowLoot == 0) ? 0 : 1;
	}

	return 1;
}