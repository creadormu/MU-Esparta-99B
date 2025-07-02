// QuestObjective.cpp: implementation of the CQuestObjective class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuestObjective.h"
#include "DSProtocol.h"
#include "ItemManager.h"
#include "MemScript.h"
#include "Monster.h"
#include "Notice.h"
#include "Party.h"
#include "Quest.h"
#include "ServerInfo.h"
#include "Util.h"

CQuestObjective gQuestObjective;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestObjective::CQuestObjective() // OK
{
	this->m_QuestObjectiveInfo.clear();
}

CQuestObjective::~CQuestObjective() // OK
{

}

void CQuestObjective::Load(char* path) // OK
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

	this->m_QuestObjectiveInfo.clear();

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

			QUEST_OBJECTIVE_INFO info;

			info.Sort = lpMemScript->GetNumber();

			info.Type = lpMemScript->GetAsNumber();

			info.Index = lpMemScript->GetAsNumber();

			info.Quantity = lpMemScript->GetAsNumber();

			info.Level = lpMemScript->GetAsNumber();

			info.Option1 = lpMemScript->GetAsNumber();

			info.Option2 = lpMemScript->GetAsNumber();

			info.Option3 = lpMemScript->GetAsNumber();

			info.NewOption = lpMemScript->GetAsNumber();

			info.MapNumber = lpMemScript->GetAsNumber();

			info.DropMinLevel = lpMemScript->GetAsNumber();

			info.DropMaxLevel = lpMemScript->GetAsNumber();

			info.ItemDropRate = lpMemScript->GetAsNumber();

			info.RequireIndex = lpMemScript->GetAsNumber();

			info.RequireState = lpMemScript->GetAsNumber();

			for(int n=0;n < MAX_CLASS;n++){info.RequireClass[n] = lpMemScript->GetAsNumber();}

			this->m_QuestObjectiveInfo.push_back(info);
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

int CQuestObjective::GetQuestObjectiveCount(LPOBJ lpObj,QUEST_OBJECTIVE_INFO* lpInfo) // OK
{
	switch(lpInfo->Type)
	{
		case QUEST_OBJECTIVE_ITEM:
			return gItemManager.GetInventoryItemCount(lpObj,lpInfo->Index,lpInfo->Level);
		case QUEST_OBJECTIVE_MONEY:
			return lpObj->Money;
		#if(GAMESERVER_UPDATE>=301)
		case QUEST_OBJECTIVE_MONSTER:
			return this->GetQuestObjectiveKillCount(lpObj, lpInfo);
		#endif
	}

	return 0;
}

int CQuestObjective::GetQuestObjectiveKillCount(LPOBJ lpObj,QUEST_OBJECTIVE_INFO* lpInfo) // OK
{
	#if(GAMESERVER_UPDATE>=301)

	if(lpObj->QuestKillCountIndex != lpInfo->RequireIndex)
	{
		return 0;
	}

	if(lpObj->QuestKillCount[lpInfo->Sort].MonsterClass != lpInfo->Index)
	{
		return 0;
	}

	return lpObj->QuestKillCount[lpInfo->Sort].KillCount;

	#else

	return 0;

	#endif
}

bool CQuestObjective::CheckQuestObjectiveRequisite(LPOBJ lpObj,QUEST_OBJECTIVE_INFO* lpInfo) // OK
{
	if(lpInfo->RequireIndex != -1 && gQuest.CheckQuestListState(lpObj,lpInfo->RequireIndex,lpInfo->RequireState) == 0)
	{
		return 0;
	}

	if(lpInfo->RequireClass[lpObj->Class] == 0 || lpInfo->RequireClass[lpObj->Class] > (lpObj->ChangeUp+1))
	{
		return 0;
	}

	return 1;
}

bool CQuestObjective::CheckQuestObjective(LPOBJ lpObj,int QuestIndex) // OK
{
	for each(QUEST_OBJECTIVE_INFO lpInfo in this->m_QuestObjectiveInfo)
	{
		if(lpInfo.RequireIndex != QuestIndex)
		{
			continue;
		}

		if(this->CheckQuestObjectiveRequisite(lpObj,&lpInfo) == 0)
		{
			continue;
		}

		if(this->GetQuestObjectiveCount(lpObj,&lpInfo) < lpInfo.Quantity)
		{
			return 0;
		}
	}

	return 1;
}

bool CQuestObjective::CheckQuestObjectiveItemCount(LPOBJ lpObj,int ItemIndex,int ItemLevel) // OK
{
	for each(QUEST_OBJECTIVE_INFO lpInfo in this->m_QuestObjectiveInfo)
	{
		if(this->CheckQuestObjectiveRequisite(lpObj,&lpInfo) == 0)
		{
			continue;
		}

		if(lpInfo.Type != QUEST_OBJECTIVE_ITEM)
		{
			continue;
		}

		if(lpInfo.Index == ItemIndex && lpInfo.Level == ItemLevel && lpInfo.Quantity <= this->GetQuestObjectiveCount(lpObj,&lpInfo))
		{
			return 0;
		}
	}

	return 1;
}

void CQuestObjective::AddQuestObjectiveKillCount(LPOBJ lpObj,QUEST_OBJECTIVE_INFO* lpInfo) // OK
{
	#if(GAMESERVER_UPDATE>=301)

	if(lpObj->QuestKillCountIndex != lpInfo->RequireIndex)
	{
		return;
	}

	if(lpObj->QuestKillCount[lpInfo->Sort].MonsterClass != lpInfo->Index)
	{
		return;
	}

	lpObj->QuestKillCount[lpInfo->Sort].KillCount++;

	#endif
}

void CQuestObjective::InitQuestObjectiveKillCount(LPOBJ lpObj,int QuestIndex) // OK
{
	#if(GAMESERVER_UPDATE>=301)

	for each(QUEST_OBJECTIVE_INFO lpInfo in this->m_QuestObjectiveInfo)
	{
		if(lpInfo.RequireIndex != QuestIndex)
		{
			continue;
		}

		if(this->CheckQuestObjectiveRequisite(lpObj,&lpInfo) == 0)
		{
			continue;
		}

		if(lpInfo.Type != QUEST_OBJECTIVE_MONSTER)
		{
			continue;
		}

		lpObj->QuestKillCountIndex = QuestIndex;

		lpObj->QuestKillCount[lpInfo.Sort].MonsterClass = lpInfo.Index;

		lpObj->QuestKillCount[lpInfo.Sort].KillCount = 0;
	}

	#endif
}

void CQuestObjective::RemoveQuestObjective(LPOBJ lpObj,int QuestIndex) // OK
{
	for each(QUEST_OBJECTIVE_INFO lpInfo in this->m_QuestObjectiveInfo)
	{
		if(lpInfo.RequireIndex != QuestIndex)
		{
			continue;
		}

		if(this->CheckQuestObjectiveRequisite(lpObj,&lpInfo) == 0)
		{
			continue;
		}

		if(lpInfo.Type == QUEST_OBJECTIVE_ITEM)
		{
			gItemManager.DeleteInventoryItemCount(lpObj,lpInfo.Index,lpInfo.Level,lpInfo.Quantity);
			continue;
		}

		if(lpInfo.Type == QUEST_OBJECTIVE_MONEY)
		{
			lpObj->Money -= lpInfo.Quantity;
			GCMoneySend(lpObj->Index,lpObj->Money);
			continue;
		}

		#if(GAMESERVER_UPDATE>=301)
		
		if(lpInfo.Type == QUEST_OBJECTIVE_MONSTER)
		{
			lpObj->QuestKillCountIndex = -1;
			lpObj->QuestKillCount[lpInfo.Sort].MonsterClass = -1;
			lpObj->QuestKillCount[lpInfo.Sort].KillCount = 0;
			continue;
		}

		#endif
	}
}

bool CQuestObjective::MonsterItemDrop(LPOBJ lpMonster) // OK
{
	int aIndex = gObjMonsterGetTopHitDamageUser(lpMonster);

	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(OBJECT_RANGE(lpObj->PartyNumber) != 0 && gServerInfo.m_QuestMonsterItemDropParty != 0)
	{
		return this->MonsterItemDropParty(lpMonster,lpObj->PartyNumber);
	}

	for each(QUEST_OBJECTIVE_INFO lpInfo in this->m_QuestObjectiveInfo)
	{
		if(this->CheckQuestObjectiveRequisite(lpObj,&lpInfo) == 0)
		{
			continue;
		}

		if(lpInfo.Type != QUEST_OBJECTIVE_ITEM)
		{
			continue;
		}

		if(lpInfo.MapNumber != -1 && lpInfo.MapNumber != lpMonster->Map)
		{
			continue;
		}

		if(lpInfo.DropMinLevel != -1 && lpInfo.DropMinLevel > lpMonster->Level)
		{
			continue;
		}

		if(lpInfo.DropMinLevel != -1 && lpInfo.DropMaxLevel != -1 && lpInfo.DropMaxLevel < lpMonster->Level)
		{
			continue;
		}

		if(lpInfo.DropMinLevel == -1 && lpInfo.DropMaxLevel != -1 && lpInfo.DropMaxLevel != lpMonster->Class)
		{
			continue;
		}

		if(lpInfo.ItemDropRate > (GetLargeRand()%10000) && lpInfo.Quantity > this->GetQuestObjectiveCount(lpObj,&lpInfo))
		{
			GDCreateItemSend(aIndex,lpMonster->Map,(BYTE)lpMonster->X,(BYTE)lpMonster->Y,lpInfo.Index,lpInfo.Level,lpInfo.Option1,lpInfo.Option2,lpInfo.Option3,0,aIndex,lpInfo.NewOption,0,0,0,0,0xFF,0);
			return 1;
		}
	}

	return 0;
}

bool CQuestObjective::MonsterItemDropParty(LPOBJ lpMonster,int PartyNumber) // OK
{
	for(int n = 0; n < MAX_PARTY_USER; n++)
	{
		int aIndex = gParty.m_PartyInfo[PartyNumber].Index[n];

		if(OBJECT_RANGE(aIndex) == 0)
		{
			continue;
		}

		LPOBJ lpObj = &gObj[aIndex];

		for each(QUEST_OBJECTIVE_INFO lpInfo in this->m_QuestObjectiveInfo)
		{
			if(this->CheckQuestObjectiveRequisite(lpObj,&lpInfo) == 0)
			{
				continue;
			}

			if(lpInfo.Type != QUEST_OBJECTIVE_ITEM)
			{
				continue;
			}

			if(lpInfo.MapNumber != -1 && lpInfo.MapNumber != lpMonster->Map)
			{
				continue;
			}

			if(lpInfo.DropMinLevel != -1 && lpInfo.DropMinLevel > lpMonster->Level)
			{
				continue;
			}

			if(lpInfo.DropMinLevel != -1 && lpInfo.DropMaxLevel != -1 && lpInfo.DropMaxLevel < lpMonster->Level)
			{
				continue;
			}

			if(lpInfo.DropMinLevel == -1 && lpInfo.DropMaxLevel != -1 && lpInfo.DropMaxLevel != lpMonster->Class)
			{
				continue;
			}

			if(lpInfo.ItemDropRate > (GetLargeRand()%10000) && lpInfo.Quantity > this->GetQuestObjectiveCount(lpObj,&lpInfo))
			{
				GDCreateItemSend(aIndex,lpMonster->Map,(BYTE)lpMonster->X,(BYTE)lpMonster->Y,lpInfo.Index,lpInfo.Level,lpInfo.Option1,lpInfo.Option2,lpInfo.Option3,0,aIndex,lpInfo.NewOption,0,0,0,0,0xFF,0);
				return 1;
			}
		}
	}

	return 0;
}

void CQuestObjective::MonsterKill(LPOBJ lpMonster) // OK
{
	int aIndex = gObjMonsterGetTopHitDamageUser(lpMonster);

	if(OBJECT_RANGE(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(OBJECT_RANGE(lpObj->PartyNumber) != 0 && gServerInfo.m_QuestMonsterKillParty != 0)
	{
		return this->MonsterKillParty(lpMonster,lpObj->PartyNumber);
	}

	for each(QUEST_OBJECTIVE_INFO lpInfo in this->m_QuestObjectiveInfo)
	{
		if(this->CheckQuestObjectiveRequisite(lpObj,&lpInfo) == 0)
		{
			continue;
		}

		if(lpInfo.Type != QUEST_OBJECTIVE_MONSTER)
		{
			continue;
		}

		if(lpInfo.MapNumber != -1 && lpInfo.MapNumber != lpMonster->Map)
		{
			continue;
		}

		#if(GAMESERVER_UPDATE>=301)
		
		if(lpInfo.Index == lpMonster->Class && lpInfo.Quantity > this->GetQuestObjectiveCount(lpObj,&lpInfo))
		{
			this->AddQuestObjectiveKillCount(lpObj,&lpInfo);
			gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,"[Quest] %d/%d of %s",this->GetQuestObjectiveCount(lpObj,&lpInfo),lpInfo.Quantity,lpMonster->MonsterName);
			return;
		}

		#endif
	}
}

void CQuestObjective::MonsterKillParty(LPOBJ lpMonster,int PartyNumber) // OK
{
	for(int n = 0; n < MAX_PARTY_USER; n++)
	{
		int aIndex = gParty.m_PartyInfo[PartyNumber].Index[n];

		if(OBJECT_RANGE(aIndex) == 0)
		{
			continue;
		}

		LPOBJ lpObj = &gObj[aIndex];

		for each(QUEST_OBJECTIVE_INFO lpInfo in this->m_QuestObjectiveInfo)
		{
			if(this->CheckQuestObjectiveRequisite(lpObj,&lpInfo) == 0)
			{
				continue;
			}

			if(lpInfo.Type != QUEST_OBJECTIVE_MONSTER)
			{
				continue;
			}

			if(lpInfo.MapNumber != -1 && lpInfo.MapNumber != lpMonster->Map)
			{
				continue;
			}

			#if(GAMESERVER_UPDATE>=301)

			if(lpInfo.Index == lpMonster->Class && lpInfo.Quantity > this->GetQuestObjectiveCount(lpObj,&lpInfo))
			{
				this->AddQuestObjectiveKillCount(lpObj,&lpInfo);
				gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,"[Quest] %d/%d of %s",this->GetQuestObjectiveCount(lpObj,&lpInfo),lpInfo.Quantity,lpMonster->MonsterName);
				return;
			}

			#endif
		}
	}
}