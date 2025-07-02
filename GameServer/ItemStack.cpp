// ItemStack.cpp: implementation of the CItemStack class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ItemStack.h"
#include "ItemManager.h"
#include "MemScript.h"
#include "Util.h"

CItemStack gItemStack;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemStack::CItemStack() // OK
{
	this->m_ItemStackInfo.clear();
}

CItemStack::~CItemStack() // OK
{

}

void CItemStack::Load(char* path) // OK
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

	this->m_ItemStackInfo.clear();

	try
	{
		while (true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			int section = lpMemScript->GetNumber();

			while (true)
			{
				if(section == 0)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					ITEM_STACK_INFO info;

					info.ItemIndex = SafeGetItem(GET_ITEM(lpMemScript->GetNumber(),lpMemScript->GetAsNumber()));

					info.Level = lpMemScript->GetAsNumber();

					info.MaxStack = lpMemScript->GetAsNumber();

					info.CreateItemIndex = -1;

					this->m_ItemStackInfo.push_back(info);
				}
				else if(section == 1)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					ITEM_STACK_INFO info;

					info.ItemIndex = SafeGetItem(GET_ITEM(lpMemScript->GetNumber(),lpMemScript->GetAsNumber()));

					info.Level = -1;

					info.MaxStack = lpMemScript->GetAsNumber();

					info.CreateItemIndex = SafeGetItem(GET_ITEM(lpMemScript->GetAsNumber(),lpMemScript->GetAsNumber()));

					this->m_ItemStackInfo.push_back(info);
				}
				else
				{
					break;
				}
			}
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

bool CItemStack::CheckItemStack(int index,int level) // OK
{
	for each(ITEM_STACK_INFO lpInfo in this->m_ItemStackInfo)
	{
		if(lpInfo.ItemIndex != index)
		{
			continue;
		}

		if(lpInfo.Level != -1 && lpInfo.Level != level)
		{
			continue;
		}

		return 1;
	}

	return 0;
}

int CItemStack::GetItemMaxStack(int index,int level) // OK
{
	for each(ITEM_STACK_INFO lpInfo in this->m_ItemStackInfo)
	{
		if(lpInfo.ItemIndex != index)
		{
			continue;
		}

		if(lpInfo.Level != -1 && lpInfo.Level != level)
		{
			continue;
		}

		return lpInfo.MaxStack;
	}
	
	return 0;
}

int CItemStack::GetCreateItemIndex(int index) // OK
{
	for each(ITEM_STACK_INFO lpInfo in this->m_ItemStackInfo)
	{
		if(lpInfo.ItemIndex != index)
		{
			continue;
		}

		return lpInfo.CreateItemIndex;
	}

	return -1;
}