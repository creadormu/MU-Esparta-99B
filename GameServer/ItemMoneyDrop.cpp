// ItemMoneyDrop.cpp: implementation of the CItemMoneyDrop class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ItemMoneyDrop.h"
#include "BonusManager.h"
#include "CrywolfSync.h"
#include "DSProtocol.h"
#include "Map.h"
#include "MapManager.h"
#include "MemScript.h"
#include "Monster.h"
#include "RandomManager.h"
#include "ServerInfo.h"
#include "Util.h"

CItemMoneyDrop gItemMoneyDrop;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemMoneyDrop::CItemMoneyDrop() // OK
{
	this->m_ItemMoneyDropInfo.clear();
}

CItemMoneyDrop::~CItemMoneyDrop() // OK
{

}

void CItemMoneyDrop::Load(char* path) // OK
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

	this->m_ItemMoneyDropInfo.clear();

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

			ITEM_MONEY_DROP_INFO info;

			memset(&info,0,sizeof(info));

			info.MapNumber = lpMemScript->GetNumber();

			info.MonsterClass = lpMemScript->GetAsNumber();

			info.MonsterLevelMin = lpMemScript->GetAsNumber();

			info.MonsterLevelMax = lpMemScript->GetAsNumber();

			info.AccountLevel = lpMemScript->GetAsNumber();

			info.MoneyAmount = lpMemScript->GetAsNumber();

			info.MoneyDropRate = lpMemScript->GetAsNumber();

			this->m_ItemMoneyDropInfo.push_back(info);
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

int CItemMoneyDrop::DropMoney(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	if(gMapManager.GetMapItemDrop(lpTarget->Map) == 0)
	{
		return 0;
	}

	CRandomManager RandomManager;

	for(std::vector<ITEM_MONEY_DROP_INFO>::iterator it=this->m_ItemMoneyDropInfo.begin();it != this->m_ItemMoneyDropInfo.end();it++)
	{
		int DropRate;

		if(it->MapNumber != -1 && it->MapNumber != lpObj->Map)
		{
			continue;
		}

		if(it->MonsterClass != -1 && it->MonsterClass != lpObj->Class)
		{
			continue;
		}

		if(it->MonsterLevelMin != -1 && it->MonsterLevelMin > lpObj->Level)
		{
			continue;
		}

		if(it->MonsterLevelMax != -1 && it->MonsterLevelMax < lpObj->Level)
		{
			continue;
		}

		if(it->AccountLevel != -1 && it->AccountLevel != lpTarget->AccountLevel)
		{
			continue;
		}

		if((DropRate=it->MoneyDropRate) == -1 || (GetLargeRand()%1000000) < (DropRate=it->MoneyDropRate))
		{
			int rate = (1000000/((DropRate==-1)?1000000:DropRate));

			RandomManager.AddElement((int)(&(*it)),rate);
		}
	}

	ITEM_MONEY_DROP_INFO* lpItemDropInfo;

	if(RandomManager.GetRandomElement((int*)&lpItemDropInfo) == 0)
	{
		return 0;
	}
	else
	{
		int Amount = (lpItemDropInfo->MoneyAmount==-1)?((lpObj->Money==0)?1:lpObj->Money):lpItemDropInfo->MoneyAmount;

		Amount = (Amount*lpTarget->MoneyAmountDropRate)/100;

		gMap[lpObj->Map].MoneyItemDrop((Amount>0)?Amount:1,lpObj->X,lpObj->Y);

		return 1;
	}
}