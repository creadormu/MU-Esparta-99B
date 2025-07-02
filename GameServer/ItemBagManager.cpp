// ItemBagManager.cpp: implementation of the CItemBagManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ItemBagManager.h"
#include "ItemManager.h"
#include "Log.h"
#include "MemScript.h"
#include "Path.h"
#include "Util.h"

CItemBagManager gItemBagManager;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemBagManager::CItemBagManager() // OK
{
	this->m_ItemBagManagerInfo.clear();
}

CItemBagManager::~CItemBagManager() // OK
{

}

void CItemBagManager::Load(char* path) // OK
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

	this->m_ItemBagManagerInfo.clear();

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

			ITEM_BAG_MANAGER_INFO info;

			info.ItemIndex = lpMemScript->GetNumber();

			info.ItemLevel = lpMemScript->GetAsNumber();

			info.MonsterClass = lpMemScript->GetAsNumber();

			info.SpecialValue = lpMemScript->GetAsNumber();

			info.SpecialLevel = lpMemScript->GetAsNumber();

			info.SpecialState = lpMemScript->GetAsNumber();

			strcpy_s(info.BagPath,lpMemScript->GetAsString());

			this->m_ItemBagManagerInfo.push_back(info);
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CItemBagManager::LoadEventItemBag() // OK
{
	for(std::vector<ITEM_BAG_MANAGER_INFO>::iterator it = this->m_ItemBagManagerInfo.begin(); it != this->m_ItemBagManagerInfo.end(); it++)
	{
		char path[MAX_PATH];

		wsprintf(path,"EventItemBag\\%s.txt",it->BagPath);

		it->ItemBag.Load(gPath.GetFullPath(path));
	}
}

bool CItemBagManager::GetItemBySpecialValue(int SpecialValue,int SpecialLevel,int SpecialState,LPOBJ lpObj,CItem* lpItem) // OK
{
	for(std::vector<ITEM_BAG_MANAGER_INFO>::iterator it = this->m_ItemBagManagerInfo.begin(); it != this->m_ItemBagManagerInfo.end(); it++)
	{
		if(it->SpecialValue == SpecialValue && (it->SpecialLevel == -1 || it->SpecialLevel == SpecialLevel) && (it->SpecialState == -1 || it->SpecialState == SpecialState))
		{
			return it->ItemBag.GetItem(lpObj,lpItem);
		}
	}

	return 0;
}

bool CItemBagManager::DropItemByItemIndex(int ItemIndex,int ItemLevel,LPOBJ lpObj,int map,int x,int y) // OK
{
	for(std::vector<ITEM_BAG_MANAGER_INFO>::iterator it = this->m_ItemBagManagerInfo.begin(); it != this->m_ItemBagManagerInfo.end(); it++)
	{
		if(it->ItemIndex == ItemIndex && (it->ItemLevel == -1 || it->ItemLevel == ItemLevel))
		{
			return it->ItemBag.DropItem(lpObj,map,x,y);
		}
	}

	return 0;
}

bool CItemBagManager::DropItemByMonsterClass(int MonsterClass,LPOBJ lpObj,int map,int x,int y) // OK
{
	for(std::vector<ITEM_BAG_MANAGER_INFO>::iterator it = this->m_ItemBagManagerInfo.begin(); it != this->m_ItemBagManagerInfo.end(); it++)
	{
		if(it->MonsterClass == MonsterClass)
		{
			return it->ItemBag.DropItem(lpObj,map,x,y);
		}
	}
	
	return 0;
}

bool CItemBagManager::DropItemBySpecialValue(int SpecialValue,int SpecialLevel,int SpecialState,LPOBJ lpObj,int map,int x,int y) // OK
{
	for(std::vector<ITEM_BAG_MANAGER_INFO>::iterator it = this->m_ItemBagManagerInfo.begin(); it != this->m_ItemBagManagerInfo.end(); it++)
	{
		if(it->SpecialValue == SpecialValue && (it->SpecialLevel == -1 || it->SpecialLevel == SpecialLevel) && (it->SpecialState == -1 || it->SpecialState == SpecialState))
		{
			return it->ItemBag.DropItem(lpObj,map,x,y);
		}
	}

	return 0;
}