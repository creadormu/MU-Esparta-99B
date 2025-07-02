// CustomJewel.cpp: implementation of the CCustomJewel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomJewel.h"
#include "CustomWing.h"
#include "ExcellentOptionRate.h"
#include "ItemManager.h"
#include "MemScript.h"
#include "ObjectManager.h"
#include "ServerInfo.h"
#include "SetItemType.h"
#include "SocketItemType.h"
#include "Util.h"
#include "WingOption.h"

CCustomJewel gCustomJewel;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomJewel::CCustomJewel() // OK
{
	this->Init();
}

CCustomJewel::~CCustomJewel() // OK
{

}

void CCustomJewel::Init() // OK
{
	for(int n=0;n < MAX_CUSTOM_JEWEL;n++)
	{
		this->m_CustomJewelInfo[n].Index = -1;
	}
}

void CCustomJewel::Load(char* path) // OK
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

					CUSTOM_JEWEL_INFO info;

					memset(&info,0,sizeof(info));

					info.Index = lpMemScript->GetNumber();

					info.ItemIndex = SafeGetItem(GET_ITEM(lpMemScript->GetAsNumber(),lpMemScript->GetAsNumber()));

					info.EnableSlotWeapon = lpMemScript->GetAsNumber();

					info.EnableSlotArmor = lpMemScript->GetAsNumber();

					info.EnableSlotWing = lpMemScript->GetAsNumber();

					info.Rate[0] = lpMemScript->GetAsNumber();

					info.Rate[1] = lpMemScript->GetAsNumber();

					info.Rate[2] = lpMemScript->GetAsNumber();

					info.Rate[3] = lpMemScript->GetAsNumber();

					info.SalePrice = lpMemScript->GetAsNumber();

					this->SetInfo(info);
				}
				else if(section == 1)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					CUSTOM_JEWEL_REQUIRE_INFO info;

					memset(&info,0,sizeof(info));

					info.Index = lpMemScript->GetNumber();

					info.MinItemLevel = lpMemScript->GetAsNumber();

					info.MaxItemLevel = lpMemScript->GetAsNumber();

					info.MaxItemOption1 = lpMemScript->GetAsNumber();

					info.MaxItemOption2 = lpMemScript->GetAsNumber();

					info.MinItemOption3 = lpMemScript->GetAsNumber();

					info.MaxItemOption3 = lpMemScript->GetAsNumber();

					info.MinItemNewOption = lpMemScript->GetAsNumber();

					info.MaxItemNewOption = lpMemScript->GetAsNumber();

					info.MaxItemSetOption = lpMemScript->GetAsNumber();

					#if(GAMESERVER_UPDATE>=401)

					info.MinItemSocketOption = lpMemScript->GetAsNumber();

					info.MaxItemSocketOption = lpMemScript->GetAsNumber();

					#endif

					this->SetRequireInfo(info);
				}
				else if(section == 2)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					CUSTOM_JEWEL_SUCCESS_INFO info;

					memset(&info,0,sizeof(info));

					info.Index = lpMemScript->GetNumber();

					info.Level = lpMemScript->GetAsNumber();

					info.Option1 = lpMemScript->GetAsNumber();

					info.Option2 = lpMemScript->GetAsNumber();

					info.Option3 = lpMemScript->GetAsNumber();

					info.NewOption = lpMemScript->GetAsNumber();

					info.SetOption = lpMemScript->GetAsNumber();

					#if(GAMESERVER_UPDATE>=401)

					info.SocketOption = lpMemScript->GetAsNumber();

					#endif

					this->SetSuccessInfo(info);
				}
				else if(section == 3)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					CUSTOM_JEWEL_FAILURE_INFO info;

					memset(&info,0,sizeof(info));

					info.Index = lpMemScript->GetNumber();

					info.Level = lpMemScript->GetAsNumber();

					info.Option1 = lpMemScript->GetAsNumber();

					info.Option2 = lpMemScript->GetAsNumber();

					info.Option3 = lpMemScript->GetAsNumber();

					info.NewOption = lpMemScript->GetAsNumber();

					info.SetOption = lpMemScript->GetAsNumber();

					#if(GAMESERVER_UPDATE>=401)

					info.SocketOption = lpMemScript->GetAsNumber();

					#endif

					this->SetFailureInfo(info);
				}
				else
				{
					break;
				}
			}
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CCustomJewel::SetInfo(CUSTOM_JEWEL_INFO info) // OK
{
	if(info.Index < 0 || info.Index >= MAX_CUSTOM_JEWEL)
	{
		return;
	}

	this->m_CustomJewelInfo[info.Index] = info;
}

void CCustomJewel::SetRequireInfo(CUSTOM_JEWEL_REQUIRE_INFO info) // OK
{
	if(info.Index < 0 || info.Index >= MAX_CUSTOM_JEWEL)
	{
		return;
	}

	this->m_CustomJewelInfo[info.Index].RequireInfo = info;
}

void CCustomJewel::SetSuccessInfo(CUSTOM_JEWEL_SUCCESS_INFO info) // OK
{
	if(info.Index < 0 || info.Index >= MAX_CUSTOM_JEWEL)
	{
		return;
	}

	this->m_CustomJewelInfo[info.Index].SuccessInfo = info;
}

void CCustomJewel::SetFailureInfo(CUSTOM_JEWEL_FAILURE_INFO info) // OK
{
	if(info.Index < 0 || info.Index >= MAX_CUSTOM_JEWEL)
	{
		return;
	}

	this->m_CustomJewelInfo[info.Index].FailureInfo = info;
}

CUSTOM_JEWEL_INFO* CCustomJewel::GetInfo(int index) // OK
{
	if(index < 0 || index >= MAX_CUSTOM_JEWEL)
	{
		return 0;
	}

	if(this->m_CustomJewelInfo[index].Index != index)
	{
		return 0;
	}

	return &this->m_CustomJewelInfo[index];
}

CUSTOM_JEWEL_INFO* CCustomJewel::GetInfoByItem(int ItemIndex) // OK
{
	for(int n=0;n < MAX_CUSTOM_JEWEL;n++)
	{
		CUSTOM_JEWEL_INFO* lpInfo = this->GetInfo(n);

		if(lpInfo == 0)
		{
			continue;
		}

		if(lpInfo->ItemIndex == ItemIndex)
		{
			return lpInfo;
		}
	}

	return 0;
}

CUSTOM_JEWEL_SUCCESS_INFO* CCustomJewel::GetSuccessInfo(int ItemIndex) // OK
{
	CUSTOM_JEWEL_INFO* lpInfo = this->GetInfoByItem(ItemIndex);

	if(lpInfo == 0)
	{
		return 0;
	}

	return &lpInfo->SuccessInfo;
}

CUSTOM_JEWEL_FAILURE_INFO* CCustomJewel::GetFailureInfo(int ItemIndex) // OK
{
	CUSTOM_JEWEL_INFO* lpInfo = this->GetInfoByItem(ItemIndex);

	if(lpInfo == 0)
	{
		return 0;
	}

	return &lpInfo->FailureInfo;
}

bool CCustomJewel::CheckCustomJewel(int index) // OK
{
	if(this->GetInfo(index) != 0)
	{
		return 1;
	}

	return 0;
}

bool CCustomJewel::CheckCustomJewelByItem(int ItemIndex) // OK
{
	if(this->GetInfoByItem(ItemIndex) != 0)
	{
		return 1;
	}

	return 0;
}

bool CCustomJewel::CheckCustomJewelApplyItem(int ItemIndex,CItem* lpItem) // OK
{
	CUSTOM_JEWEL_INFO* lpInfo = this->GetInfoByItem(ItemIndex);

	if(lpInfo == 0)
	{
		return 0;
	}

	if(lpItem->IsLuckyItem() != 0)
	{
		return 0;
	}

	if(lpItem->IsJewelOfHarmonyItem() != 0 && (lpInfo->RequireInfo.MinItemSocketOption > 0 || lpInfo->RequireInfo.MaxItemSocketOption > 0))
	{
		return 0;
	}

	if(lpItem->m_Index == GET_ITEM(4,7) || lpItem->m_Index == GET_ITEM(4,15))
	{
		return 0;
	}

	if(lpItem->m_Index > GET_ITEM(12,6) && (lpItem->m_Index < GET_ITEM(12,36) || lpItem->m_Index > GET_ITEM(12,43)) && lpItem->m_Index != GET_ITEM(12,49) && lpItem->m_Index != GET_ITEM(12,50) && (lpItem->m_Index < GET_ITEM(12,262) || lpItem->m_Index > GET_ITEM(12,265)) && lpItem->m_Index != GET_ITEM(13,30) && gCustomWing.CheckCustomWingByItem(lpItem->m_Index) == 0)
	{
		return 0;
	}

	if(lpInfo->EnableSlotWeapon == 0 && (lpItem->m_Index >= GET_ITEM(0,0) && lpItem->m_Index < GET_ITEM(6,0)))
	{
		return 0;
	}

	if(lpInfo->EnableSlotArmor == 0 && (lpItem->m_Index >= GET_ITEM(6,0) && lpItem->m_Index < GET_ITEM(12,0)))
	{
		return 0;
	}

	if(lpInfo->EnableSlotWing == 0 && ((lpItem->m_Index >= GET_ITEM(12,0) && lpItem->m_Index <= GET_ITEM(12,6)) || (lpItem->m_Index >= GET_ITEM(12,36) && lpItem->m_Index <= GET_ITEM(12,43)) || lpItem->m_Index == GET_ITEM(12,49) || lpItem->m_Index == GET_ITEM(12,50) || (lpItem->m_Index >= GET_ITEM(12,262) && lpItem->m_Index <= GET_ITEM(12,265)) || lpItem->m_Index == GET_ITEM(13,30) || gCustomWing.CheckCustomWingByItem(lpItem->m_Index) != 0))
	{
		return 0;
	}

	if(lpInfo->RequireInfo.MinItemLevel != -1 && lpItem->m_Level < lpInfo->RequireInfo.MinItemLevel)
	{
		return 0;
	}

	if(lpInfo->RequireInfo.MaxItemLevel != -1 && lpItem->m_Level > lpInfo->RequireInfo.MaxItemLevel)
	{
		return 0;
	}

	if(lpInfo->RequireInfo.MaxItemOption1 != -1 && lpItem->m_Option1 > lpInfo->RequireInfo.MaxItemOption1)
	{
		return 0;
	}

	if(lpInfo->RequireInfo.MaxItemOption2 != -1 && lpItem->m_Option2 > lpInfo->RequireInfo.MaxItemOption2)
	{
		return 0;
	}

	if(lpInfo->RequireInfo.MinItemOption3 != -1 && lpItem->m_Option3 < lpInfo->RequireInfo.MinItemOption3)
	{
		return 0;
	}

	if(lpInfo->RequireInfo.MaxItemOption3 != -1 && lpItem->m_Option3 > lpInfo->RequireInfo.MaxItemOption3)
	{
		return 0;
	}

	if(this->CheckCustomJewelApplyItemNewOption(lpInfo,lpItem) == 0)
	{
		return 0;
	}

	if(this->CheckCustomJewelApplyItemSetOption(lpInfo,lpItem) == 0)
	{
		return 0;
	}

	if(this->CheckCustomJewelApplyItemSocketOption(lpInfo,lpItem) == 0)
	{
		return 0;
	}

	return 1;
}

bool CCustomJewel::CheckCustomJewelApplyItemNewOption(CUSTOM_JEWEL_INFO* lpInfo,CItem* lpItem) // OK
{
	if(lpInfo->RequireInfo.MinItemNewOption != -1 && GetNewOptionCount(lpItem->m_NewOption) < lpInfo->RequireInfo.MinItemNewOption)
	{
		return 0;
	}

	if(lpInfo->RequireInfo.MaxItemNewOption != -1 && lpInfo->RequireInfo.MaxItemNewOption > 10 && (lpItem->m_NewOption & (lpInfo->RequireInfo.MaxItemNewOption-10)) != 0)
	{
		return 0;
	}

	if(lpInfo->RequireInfo.MaxItemNewOption != -1 && lpInfo->RequireInfo.MaxItemNewOption < 10 && GetNewOptionCount(lpItem->m_NewOption) > lpInfo->RequireInfo.MaxItemNewOption)
	{
		return 0;
	}

	return 1;
}

bool CCustomJewel::CheckCustomJewelApplyItemSetOption(CUSTOM_JEWEL_INFO* lpInfo,CItem* lpItem) // OK
{
	if(lpInfo->RequireInfo.MaxItemSetOption != -1 && (lpItem->IsSetItem() != 0 || ((lpInfo->RequireInfo.MaxItemSetOption & 1) != 0 && gSetItemType.GetSetItemOptionIndex(lpItem->m_Index,0) <= 0)))
	{
		return 0;
	}

	if(lpInfo->RequireInfo.MaxItemSetOption != -1 && (lpItem->IsSetItem() != 0 || ((lpInfo->RequireInfo.MaxItemSetOption & 2) != 0 && gSetItemType.GetSetItemOptionIndex(lpItem->m_Index,1) <= 0)))
	{
		return 0;
	}

	return 1;
}

bool CCustomJewel::CheckCustomJewelApplyItemSocketOption(CUSTOM_JEWEL_INFO* lpInfo,CItem* lpItem) // OK
{
	if(lpInfo->RequireInfo.MinItemSocketOption != -1 && ((lpInfo->RequireInfo.MinItemSocketOption == 0 && gSocketItemType.CheckSocketItemType(lpItem->m_Index) != 0) || (lpInfo->RequireInfo.MinItemSocketOption != 0 && (gSocketItemType.CheckSocketItemType(lpItem->m_Index) == 0 || GetSocketOptionCount(lpItem->m_SocketOption) < lpInfo->RequireInfo.MinItemSocketOption))))
	{
		return 0;
	}

	if(lpInfo->RequireInfo.MaxItemSocketOption != -1 && ((lpInfo->RequireInfo.MaxItemSocketOption == 0 && gSocketItemType.CheckSocketItemType(lpItem->m_Index) != 0) || (lpInfo->RequireInfo.MaxItemSocketOption != 0 && (gSocketItemType.CheckSocketItemType(lpItem->m_Index) == 0 || GetSocketOptionCount(lpItem->m_SocketOption) > lpInfo->RequireInfo.MaxItemSocketOption))))
	{
		return 0;
	}

	return 1;
}

int CCustomJewel::GetCustomJewelSuccessRate(int ItemIndex,int AccountLevel) // OK
{
	CUSTOM_JEWEL_INFO* lpInfo = this->GetInfoByItem(ItemIndex);

	if(lpInfo == 0)
	{
		return 0;
	}

	return lpInfo->Rate[AccountLevel];
}

int CCustomJewel::GetCustomJewelSalePrice(int ItemIndex) // OK
{
	CUSTOM_JEWEL_INFO* lpInfo = this->GetInfoByItem(ItemIndex);

	if(lpInfo == 0)
	{
		return 0;
	}

	return lpInfo->SalePrice;
}

int CCustomJewel::GetCustomJewelNewOption(CItem* lpItem,int value,int type) // OK
{
	int count = 0;

	int MaxNewOption = MAX_EXC_OPTION;

	if(lpItem->m_Index == GET_ITEM(13,3) || lpItem->m_Index == GET_ITEM(13,37)) // Dinorant,Fenrir
	{
		MaxNewOption = 3;
	}
	else if(gWingOption.CheckWingByItem(lpItem->m_Index) != 0)
	{
		MaxNewOption = gWingOption.GetWingMaxNewOption(lpItem->m_Index);
	}
	else if(gCustomWing.CheckCustomWingByItem(lpItem->m_Index) != 0)
	{
		MaxNewOption = 4;
	}

	if(type != 0)
	{
		while(true)
		{
			if(count >= value || GetNewOptionCount(lpItem->m_NewOption) >= MaxNewOption)
			{
				break;
			}

			int option = gExcellentOptionRate.GetNewOption(lpItem->m_Index);

			if((lpItem->m_NewOption & option) == 0)
			{
				lpItem->m_NewOption |= option;
				count++;
			}
		}
	}
	else
	{
		if(value >= GetNewOptionCount(lpItem->m_NewOption))
		{
			return (lpItem->m_NewOption = 0);
		}

		while(true)
		{
			if(count == value)
			{
				break;
			}

			int option = 1 << (GetLargeRand()%MaxNewOption);

			if((lpItem->m_NewOption & option) != 0)
			{
				lpItem->m_NewOption ^= option;
				count++;
			}
		}
	}

	return count;
}

int CCustomJewel::GetCustomJewelSetOption(CItem* lpItem,int value,int type) // OK
{
	BYTE SetIndex = lpItem->m_SetOption & 3;
	BYTE SetValue = lpItem->m_SetOption & 12;

	if(type != 0)
	{
		if(SetIndex == 0)
		{
			for(int n=0;n < MAX_SET_ITEM_OPTION_INDEX;n++)
			{
				if((value & (1 << n)) != 0 && gSetItemType.GetSetItemOptionIndex(lpItem->m_Index,n) > 0)
				{
					SetIndex = ((SetIndex==0)?(n+1):(((GetLargeRand()%2)==0)?(n+1):SetIndex));
				}
			}
		}

		if(SetValue == 0 && SetIndex > 0 && (value & 4) != 0)
		{
			SetValue = 4;
		}

		if(SetValue == 4 && SetIndex > 0 && (value & 8) != 0)
		{
			SetValue = 8;
		}

		return ((lpItem->m_SetOption)=(SetIndex+SetValue));
	}
	else
	{
		if(SetIndex != 0 && value > 0)
		{
			return (lpItem->m_SetOption = 0);
		}

		return 0;
	}
}

int CCustomJewel::GetCustomJewelSocketOption(CItem* lpItem,int value,int type) // OK
{
	int count = 0;

	if (type != 0)
	{
		for(int n=0;n < MAX_SOCKET_OPTION;n++)
		{
			if(count < value && lpItem->m_SocketOption[n] == 0xFF)
			{
				lpItem->m_SocketOption[n] = 0xFE;
				count++;
			}
		}
	}
	else
	{
		for(int n=0;n < MAX_SOCKET_OPTION;n++)
		{
			if(count < value && lpItem->m_SocketOption[n] != 0xFF)
			{
				lpItem->m_SocketOption[n] = 0xFF;
				count++;
			}
		}
	}

	return count;
}
