// CustomMix.cpp: implementation of the CCustomMix class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomMix.h"
#include "ItemManager.h"
#include "MemScript.h"
#include "Util.h"

CCustomMix gCustomMix;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomMix::CCustomMix() // OK
{
	this->m_CustomMixInfo.clear();
}

CCustomMix::~CCustomMix() // OK
{

}

void CCustomMix::Load(char* path) // OK
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

	this->m_CustomMixInfo.clear();

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

					CUSTOM_MIX_INFO info;

					info.Index = lpMemScript->GetNumber();

					info.MixMoney = lpMemScript->GetAsNumber();

					info.MixRate[0] = lpMemScript->GetAsNumber();

					info.MixRate[1] = lpMemScript->GetAsNumber();

					info.MixRate[2] = lpMemScript->GetAsNumber();

					info.MixRate[3] = lpMemScript->GetAsNumber();

					strcpy_s(info.Name,lpMemScript->GetAsString());

					this->SetInfo(info);
				}
				else if(section == 1)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					CUSTOM_MIX_GROUP_INFO info;

					info.Index = lpMemScript->GetNumber();

					info.Group = lpMemScript->GetAsNumber();

					info.BagSpecial = lpMemScript->GetAsNumber();

					this->SetGroupInfo(info);
				}
				else if(section == 2)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					CUSTOM_MIX_REQUIRE_ITEM_INFO info;

					info.Index = lpMemScript->GetNumber();

					info.Group = lpMemScript->GetAsNumber();

					info.Count = lpMemScript->GetAsNumber();

					info.ItemIndex = SafeGetItem(GET_ITEM(lpMemScript->GetAsNumber(),lpMemScript->GetAsNumber()));

					info.ItemLevel = lpMemScript->GetAsNumber();

					info.ItemOption1 = lpMemScript->GetAsNumber();

					info.ItemOption2 = lpMemScript->GetAsNumber();

					info.ItemOption3 = lpMemScript->GetAsNumber();

					info.ItemNewOption = lpMemScript->GetAsNumber();

					info.ItemSetOption = lpMemScript->GetAsNumber();

					this->SetItemInfo(info);
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

void CCustomMix::SetInfo(CUSTOM_MIX_INFO info) // OK
{
	std::map<int,CUSTOM_MIX_INFO>::iterator it = this->m_CustomMixInfo.find(info.Index);

	if(it != this->m_CustomMixInfo.end())
	{
		return;
	}

	this->m_CustomMixInfo.insert(std::pair<int,CUSTOM_MIX_INFO>(info.Index,info));
}

void CCustomMix::SetGroupInfo(CUSTOM_MIX_GROUP_INFO info) // OK
{
	std::map<int,CUSTOM_MIX_INFO>::iterator it = this->m_CustomMixInfo.find(info.Index);

	if(it == this->m_CustomMixInfo.end())
	{
		return;
	}

	std::map<int,CUSTOM_MIX_GROUP_INFO>::iterator ir = it->second.MixGroup.find(info.Group);

	if(ir != it->second.MixGroup.end())
	{
		return;
	}

	it->second.MixGroup.insert(std::pair<int,CUSTOM_MIX_GROUP_INFO>(info.Group,info));
}

void CCustomMix::SetItemInfo(CUSTOM_MIX_REQUIRE_ITEM_INFO info) // OK
{
	std::map<int,CUSTOM_MIX_INFO>::iterator it = this->m_CustomMixInfo.find(info.Index);

	if(it == this->m_CustomMixInfo.end())
	{
		return;
	}

	std::map<int,CUSTOM_MIX_GROUP_INFO>::iterator ir = it->second.MixGroup.find(info.Group);

	if(ir == it->second.MixGroup.end())
	{
		return;
	}

	ir->second.RequireItem.insert(std::pair<int,CUSTOM_MIX_REQUIRE_ITEM_INFO>(0,info));
}

CUSTOM_MIX_INFO* CCustomMix::GetInfoMix(int index) // OK
{
	std::map<int,CUSTOM_MIX_INFO>::iterator ir = this->m_CustomMixInfo.find(index);

	if(ir == this->m_CustomMixInfo.end())
	{
		return 0;
	}

	return &ir->second;
}

bool CCustomMix::CheckItem(LPOBJ lpObj,CUSTOM_MIX_REQUIRE_ITEM_INFO* lpInfo) // OK
{
	int ItemCount = 0;

	for(int n = 0; n < CHAOS_BOX_SIZE; n++)
	{
		if(lpObj->ChaosBox[n].IsItem() == 0)
		{
			continue;
		}

		if(lpObj->ChaosBox[n].m_Index != lpInfo->ItemIndex)
		{
			continue;
		}

		if(lpInfo->ItemLevel != -1 && lpInfo->ItemLevel != lpObj->ChaosBox[n].m_Level)
		{
			continue;
		}

		if(lpInfo->ItemOption1 != -1 && lpObj->ChaosBox[n].m_Option1 != lpInfo->ItemOption1)
		{
			continue;
		}

		if(lpInfo->ItemOption2 != -1 && lpObj->ChaosBox[n].m_Option2 != lpInfo->ItemOption2)
		{
			continue;
		}

		if(lpInfo->ItemOption3 != -1 && lpObj->ChaosBox[n].m_Option3 < lpInfo->ItemOption3)
		{
			continue;
		}

		if(lpInfo->ItemNewOption != -1 && (lpInfo->ItemNewOption == 0 && lpObj->ChaosBox[n].IsExcItem() != 0))
		{
			continue;
		}

		if(lpInfo->ItemSetOption != -1 && (lpInfo->ItemSetOption == 0 && lpObj->ChaosBox[n].IsSetItem() != 0))
		{
			continue;
		}

		ItemCount++;
	}

	return (ItemCount == lpInfo->Count) ? 1 : 0;
}

bool CCustomMix::CheckGroup(LPOBJ lpObj,CUSTOM_MIX_GROUP_INFO* lpInfo) // OK
{
	for(std::map<int,CUSTOM_MIX_REQUIRE_ITEM_INFO>::iterator it = lpInfo->RequireItem.begin(); it != lpInfo->RequireItem.end(); it++)
	{
		if(this->CheckItem(lpObj,&it->second) == 0)
		{
			return 0;
		}
	}

	return 1;
}