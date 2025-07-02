// ShopManager.cpp: implementation of the CShopManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ShopManager.h"
#include "CastleSiegeSync.h"
#include "GameMaster.h"
#include "MemScript.h"
#include "Monster.h"
#include "NpcTalk.h"
#include "Path.h"
#include "Util.h"

CShopManager gShopManager;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShopManager::CShopManager() // OK
{
	this->m_ShopManagerInfo.clear();
}

CShopManager::~CShopManager() // OK
{

}

void CShopManager::Load(char* path) // OK
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

	this->m_ShopManagerInfo.clear();

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

			SHOP_MANAGER_INFO info;

			memset(&info,0,sizeof(info));

			info.Index = this->m_ShopManagerInfo.size();

			info.MonsterClass = lpMemScript->GetNumber();

			info.MapNumber = lpMemScript->GetAsNumber();

			info.PosX = lpMemScript->GetAsNumber();

			info.PosY = lpMemScript->GetAsNumber();

			info.Dir = lpMemScript->GetAsNumber();

			info.Enable[0] = lpMemScript->GetAsNumber();

			info.Enable[1] = lpMemScript->GetAsNumber();

			info.Enable[2] = lpMemScript->GetAsNumber();

			info.Enable[3] = lpMemScript->GetAsNumber();

			info.GameMasterLevel = lpMemScript->GetAsNumber();

			#if(GAMESERVER_SHOP>=1)

			info.CoinType = lpMemScript->GetAsNumber();

			#endif

			char FilePath[100];

			wsprintf(FilePath,"Shop\\%s.txt",lpMemScript->GetAsString());

			info.Shop.Load(gPath.GetFullPath(FilePath),info.CoinType);

			this->m_ShopManagerInfo.push_back(info);
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CShopManager::ReloadShop() // OK
{
	for(int n=0;n<MAX_OBJECT_MONSTER;n++)
	{
		if(gObj[n].ShopNumber != -1)
		{
			gObjDel(n);
		}
	}

	for(std::vector<SHOP_MANAGER_INFO>::iterator it = this->m_ShopManagerInfo.begin(); it != this->m_ShopManagerInfo.end(); it++)
	{
		if(it->MonsterClass == 492)
		{
			continue;
		}

		int index = gObjAddMonster(it->MapNumber);

		if(OBJECT_RANGE(index) == 0)
		{
			continue;
		}

		LPOBJ lpObj = &gObj[index];

		lpObj->PosNum = -1;
		lpObj->X = it->PosX;
		lpObj->Y = it->PosY;
		lpObj->TX = it->PosX;
		lpObj->TY = it->PosY;
		lpObj->OldX = it->PosX;
		lpObj->OldY = it->PosY;
		lpObj->StartX = it->PosX;
		lpObj->StartY = it->PosY;
		lpObj->Dir = it->Dir;
		lpObj->Map = it->MapNumber;
		lpObj->ShopNumber = this->GetShopNumber(it->MonsterClass,it->MapNumber,it->PosX,it->PosY);

		if(gObjSetMonster(index,it->MonsterClass) == 0)
		{
			gObjDel(index);
			continue;
		}
	}
}

void CShopManager::ReloadShopInterface() // OK
{
	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnectedGP(n) != 0)
		{
			if(gObj[n].Interface.use != 0 && gObj[n].Interface.type == INTERFACE_SHOP)
			{
				gObj[n].Interface.state = 1;

				PMSG_NPC_TALK_SEND pMsg;

				pMsg.header.setE(0x30,sizeof(pMsg));

				pMsg.result = 0;

				DataSend(n,(BYTE*)&pMsg,pMsg.header.size);

				this->GCShopItemListSendByIndex(gObj[n].TargetShopNumber,n);

				this->GCShopItemPriceSendByIndex(gObj[n].TargetShopNumber,n);

				GCTaxInfoSend(n,2,gCastleSiegeSync.GetTaxRateStore(n));
			}
		}
	}
}

short CShopManager::GetShopNumber(int MonsterClass,int MapNumber,int PosX,int PosY) // OK
{
	for(std::vector<SHOP_MANAGER_INFO>::iterator it = this->m_ShopManagerInfo.begin(); it != this->m_ShopManagerInfo.end(); it++)
	{
		if(it->MonsterClass != -1 && it->MonsterClass == MonsterClass && it->MapNumber == MapNumber && it->PosX == PosX && it->PosY == PosY)
		{
			return it->Index;
		}
	}

	return -1;
}

bool CShopManager::GetItemByIndex(int index,CItem* lpItem,int slot,int* type) // OK
{
	for(std::vector<SHOP_MANAGER_INFO>::iterator it = this->m_ShopManagerInfo.begin(); it != this->m_ShopManagerInfo.end(); it++)
	{
		if(it->Index == index)
		{
			(*type) = it->CoinType;
			return it->Shop.GetItem(lpItem,slot);
		}
	}

	return 0;
}


long CShopManager::GetItemCountByIndex(int index) // OK
{
	for(std::vector<SHOP_MANAGER_INFO>::iterator it = this->m_ShopManagerInfo.begin(); it != this->m_ShopManagerInfo.end(); it++)
	{
		if(it->Index == index)
		{
			return it->Shop.GetItemCount();
		}
	}

	return 0;
}

bool CShopManager::CheckShopAccountLevel(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	for(std::vector<SHOP_MANAGER_INFO>::iterator it = this->m_ShopManagerInfo.begin(); it != this->m_ShopManagerInfo.end(); it++)
	{
		if(it->Index == lpNpc->ShopNumber)
		{
			if(it->Enable[lpObj->AccountLevel] != 0)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
	}

	return 0;
}

bool CShopManager::CheckShopGameMasterLevel(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	for(std::vector<SHOP_MANAGER_INFO>::iterator it = this->m_ShopManagerInfo.begin(); it != this->m_ShopManagerInfo.end(); it++)
	{
		if(it->Index == lpNpc->ShopNumber)
		{
			if(it->GameMasterLevel != -1 &&  it->GameMasterLevel > gGameMaster.GetGameMasterLevel(lpObj))
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
	}

	return 0;
}

bool CShopManager::GCShopItemListSendByIndex(int index,int aIndex) // OK
{
	for(std::vector<SHOP_MANAGER_INFO>::iterator it = this->m_ShopManagerInfo.begin(); it != this->m_ShopManagerInfo.end(); it++)
	{
		if(it->Index == index)
		{
			return it->Shop.GCShopItemListSend(aIndex);
		}
	}

	return 0;
}

bool CShopManager::GCShopItemPriceSendByIndex(int index,int aIndex) // OK
{
	for(std::vector<SHOP_MANAGER_INFO>::iterator it = this->m_ShopManagerInfo.begin(); it != this->m_ShopManagerInfo.end(); it++)
	{
		if(it->Index == index)
		{
			return it->Shop.GCShopItemCoinPriceSend(aIndex,it->CoinType);
		}
	}

	return 0;
}