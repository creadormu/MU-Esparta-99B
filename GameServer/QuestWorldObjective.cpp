// QuestWorldObjective.cpp: implementation of the CQuestWorldObjective class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuestWorldObjective.h"
#include "DSProtocol.h"
#include "GensSystem.h"
#include "MemScript.h"
#include "Monster.h"
#include "Notice.h"
#include "Party.h"
#include "QuestWorld.h"
#include "ServerInfo.h"
#include "SkillManager.h"
#include "Util.h"

CQuestWorldObjective gQuestWorldObjective;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestWorldObjective::CQuestWorldObjective() // OK
{
	#if(GAMESERVER_UPDATE>=501)

	this->m_QuestWorldObjectiveInfo.clear();

	#endif
}

CQuestWorldObjective::~CQuestWorldObjective() // OK
{

}

void CQuestWorldObjective::Load(char* path) // OK
{
	#if(GAMESERVER_UPDATE>=501)

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

	this->m_QuestWorldObjectiveInfo.clear();

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
				if(strcmp("end",lpMemScript->GetAsString()) == 0)
				{
					break;
				}

				QUEST_WORLD_OBJECTIVE_INFO info;

				info.RequireGroup = section+1;

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

				this->m_QuestWorldObjectiveInfo.push_back(info);
			}
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;

#endif
}

int CQuestWorldObjective::GetQuestWorldObjectiveCount(LPOBJ lpObj,QUEST_WORLD_OBJECTIVE_INFO* lpInfo) // OK
{
	#if(GAMESERVER_UPDATE>=501)

	switch(lpInfo->Type)
	{
		case QUEST_WORLD_OBJECTIVE_MONSTER:
			return this->GetQuestWorldObjectiveKillCount(lpObj,lpInfo);
		case QUEST_WORLD_OBJECTIVE_SKILL:
			return ((gSkillManager.GetSkill(lpObj,lpInfo->Index)==0)?0:1);
		case QUEST_WORLD_OBJECTIVE_ITEM:
			return gItemManager.GetInventoryItemCount(lpObj,lpInfo->Index,lpInfo->Level);
		case QUEST_WORLD_OBJECTIVE_LEVEL:
			return lpObj->Level;
		case QUEST_WORLD_OBJECTIVE_SPECIAL:
			return this->GetQuestWorldObjectiveKillCount(lpObj,lpInfo);
		case QUEST_WORLD_OBJECTIVE_BUFF:
			return ((gEffectManager.CheckEffect(lpObj,lpInfo->Index)==0)?0:1);
		case QUEST_WORLD_OBJECTIVE_CHAOS_CASTLE_PLAYER_KILL:
			return this->GetQuestWorldObjectiveKillCount(lpObj,lpInfo);
		case QUEST_WORLD_OBJECTIVE_CHAOS_CASTLE_MONSTER_KILL:
			return this->GetQuestWorldObjectiveKillCount(lpObj,lpInfo);
		case QUEST_WORLD_OBJECTIVE_BLOOD_CASTLE_GATE_DESTROY:
			return this->GetQuestWorldObjectiveKillCount(lpObj,lpInfo);
		case QUEST_WORLD_OBJECTIVE_BLOOD_CASTLE_CLEAR:
			return this->GetQuestWorldObjectiveKillCount(lpObj,lpInfo);
		case QUEST_WORLD_OBJECTIVE_CHAOS_CASTLE_CLEAR:
			return this->GetQuestWorldObjectiveKillCount(lpObj,lpInfo);
		case QUEST_WORLD_OBJECTIVE_DEVIL_SQUARE_CLEAR:
			return this->GetQuestWorldObjectiveKillCount(lpObj,lpInfo);
		case QUEST_WORLD_OBJECTIVE_ILLUSION_TEMPLE_CLEAR:
			return this->GetQuestWorldObjectiveKillCount(lpObj,lpInfo);
		case QUEST_WORLD_OBJECTIVE_DEVIL_SQUARE_POINTS:
			return this->GetQuestWorldObjectiveKillCount(lpObj,lpInfo);
		case QUEST_WORLD_OBJECTIVE_MONEY:
			return lpObj->Money;
		case QUEST_WORLD_OBJECTIVE_CONTRIBUTION:
			return lpObj->GensContribution;
		case QUEST_WORLD_OBJECTIVE_NPC:
			return this->GetQuestWorldObjectiveKillCount(lpObj,lpInfo);
	}

	#endif

	return 0;
}

int CQuestWorldObjective::GetQuestWorldObjectiveKillCount(LPOBJ lpObj,QUEST_WORLD_OBJECTIVE_INFO* lpInfo) // OK
{
	#if(GAMESERVER_UPDATE>=501)

	QUEST_WORLD_LIST* lpQuestWorldList = gQuestWorld.GetQuestWorldList(lpObj,lpInfo->RequireIndex,lpInfo->RequireGroup);

	if(lpQuestWorldList != 0)
	{
		return lpQuestWorldList->ObjectiveCount[lpInfo->Sort];
	}

	#endif

	return 0;
}

bool CQuestWorldObjective::GetQuestWorldObjective(LPOBJ lpObj,int QuestIndex,int QuestGroup,QUEST_WORLD_OBJECTIVE* lpObjective,int Sort) // OK
{
	#if(GAMESERVER_UPDATE>=501)

	for each(QUEST_WORLD_OBJECTIVE_INFO lpInfo in this->m_QuestWorldObjectiveInfo)
	{
		if(lpInfo.RequireIndex != QuestIndex)
		{
			continue;
		}

		if(lpInfo.RequireGroup != QuestGroup)
		{
			continue;
		}

		if(this->CheckQuestWorldObjectiveRequisite(lpObj,&lpInfo) == 0)
		{
			continue;
		}

		if(lpInfo.Sort != Sort)
		{
			continue;
		}

		lpObjective->type = lpInfo.Type;

		lpObjective->index = lpInfo.Index;

		lpObjective->value = lpInfo.Quantity;

		lpObjective->count = this->GetQuestWorldObjectiveCount(lpObj,&lpInfo);

		if(lpInfo.Type == QUEST_WORLD_OBJECTIVE_ITEM)
		{
			CItem item;

			item.m_Level = lpInfo.Level;

			item.Convert(lpInfo.Index,lpInfo.Option1,lpInfo.Option2,lpInfo.Option3,lpInfo.NewOption,0,0,0,0,0xFF);

			gItemManager.ItemByteConvert(lpObjective->ItemInfo,item);
		}

		return 1;
	}

	#endif

	return 0;
}

bool CQuestWorldObjective::CheckQuestWorldObjectiveRequisite(LPOBJ lpObj,QUEST_WORLD_OBJECTIVE_INFO* lpInfo) // OK
{
	#if(GAMESERVER_UPDATE>=501)

	if(lpInfo->RequireIndex != -1 && gQuestWorld.CheckQuestWorldListState(lpObj,lpInfo->RequireIndex,lpInfo->RequireGroup,lpInfo->RequireState) == 0)
	{
		return 0;
	}

	if(lpInfo->RequireClass[lpObj->Class] == 0 || lpInfo->RequireClass[lpObj->Class] > (lpObj->ChangeUp+1))
	{
		return 0;
	}

	return 1;

	#else

	return 0;

	#endif
}

bool CQuestWorldObjective::CheckQuestWorldObjective(LPOBJ lpObj,int QuestIndex,int QuestGroup) // OK
{
	#if(GAMESERVER_UPDATE>=501)

	for each(QUEST_WORLD_OBJECTIVE_INFO lpInfo in this->m_QuestWorldObjectiveInfo)
	{
		if(lpInfo.RequireIndex != QuestIndex || lpInfo.RequireGroup != QuestGroup)
		{
			continue;
		}

		if(this->CheckQuestWorldObjectiveRequisite(lpObj,&lpInfo) == 0)
		{
			continue;
		}

		if(this->GetQuestWorldObjectiveCount(lpObj,&lpInfo) < lpInfo.Quantity)
		{
			return 0;
		}
	}

	return 1;

	#else

	return 0;

	#endif
}

bool CQuestWorldObjective::CheckQuestWorldObjectiveItemCount(LPOBJ lpObj,int ItemIndex,int ItemLevel) // OK
{
	#if(GAMESERVER_UPDATE>=501)

	for each(QUEST_WORLD_OBJECTIVE_INFO lpInfo in this->m_QuestWorldObjectiveInfo)
	{
		if(this->CheckQuestWorldObjectiveRequisite(lpObj,&lpInfo) == 0)
		{
			continue;
		}

		if(lpInfo.Type != QUEST_WORLD_OBJECTIVE_ITEM)
		{
			continue;
		}

		if(lpInfo.Index == ItemIndex && lpInfo.Level == ItemLevel && lpInfo.Quantity <= this->GetQuestWorldObjectiveCount(lpObj,&lpInfo))
		{
			return 0;
		}
	}

	#endif

	return 1;
}

void CQuestWorldObjective::AddQuestWorldObjectiveKillCount(LPOBJ lpObj,QUEST_WORLD_OBJECTIVE_INFO* lpInfo) // OK
{
	#if(GAMESERVER_UPDATE>=501)

	QUEST_WORLD_LIST* lpQuestWorldList = gQuestWorld.GetQuestWorldList(lpObj,lpInfo->RequireIndex,lpInfo->RequireGroup);

	if(lpQuestWorldList != 0)
	{
		lpQuestWorldList->ObjectiveCount[lpInfo->Sort]++;
	}

	#endif
}

void CQuestWorldObjective::RemoveQuestWorldObjective(LPOBJ lpObj,int QuestIndex,int QuestGroup) // OK
{
	#if(GAMESERVER_UPDATE>=501)

	for each(QUEST_WORLD_OBJECTIVE_INFO lpInfo in this->m_QuestWorldObjectiveInfo)
	{
		if(lpInfo.RequireIndex != QuestIndex || lpInfo.RequireGroup != QuestGroup)
		{
			continue;
		}

		if(lpInfo.Type == QUEST_WORLD_OBJECTIVE_ITEM)
		{
			gItemManager.DeleteInventoryItemCount(lpObj,lpInfo.Index,lpInfo.Level,lpInfo.Quantity);
			continue;
		}

		if(lpInfo.Type == QUEST_WORLD_OBJECTIVE_MONEY)
		{
			lpObj->Money -= lpInfo.Quantity;
			GCMoneySend(lpObj->Index,lpObj->Money);
			continue;
		}
	}

	#endif
}

void CQuestWorldObjective::PressButton(LPOBJ lpObj,int QuestIndex,int QuestGroup) // OK
{
	#if(GAMESERVER_UPDATE>=501)

	for each(QUEST_WORLD_OBJECTIVE_INFO lpInfo in this->m_QuestWorldObjectiveInfo)
	{
		if(lpInfo.RequireIndex != QuestIndex || lpInfo.RequireGroup != QuestGroup)
		{
			continue;
		}

		if(this->CheckQuestWorldObjectiveRequisite(lpObj,&lpInfo) == 0)
		{
			continue;
		}

		if(lpInfo.Type != QUEST_WORLD_OBJECTIVE_SPECIAL)
		{
			continue;
		}

		if(lpInfo.Quantity > this->GetQuestWorldObjectiveCount(lpObj,&lpInfo))
		{
			this->AddQuestWorldObjectiveKillCount(lpObj,&lpInfo);
			return;
		}
	}

	#endif
}

bool CQuestWorldObjective::MonsterItemDrop(LPOBJ lpMonster) // OK
{
	#if(GAMESERVER_UPDATE>=501)

	int aIndex = gObjMonsterGetTopHitDamageUser(lpMonster);

	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(OBJECT_RANGE(lpObj->PartyNumber) != 0 && gServerInfo.m_QuestWorldMonsterItemDropParty != 0)
	{
		return this->MonsterItemDropParty(lpMonster,lpObj->PartyNumber);
	}

	for each(QUEST_WORLD_OBJECTIVE_INFO lpInfo in this->m_QuestWorldObjectiveInfo)
	{
		if(this->CheckQuestWorldObjectiveRequisite(lpObj,&lpInfo) == 0)
		{
			continue;
		}

		if(lpInfo.Type != QUEST_WORLD_OBJECTIVE_ITEM)
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

		if(lpInfo.ItemDropRate > (GetLargeRand()%10000) && lpInfo.Quantity > this->GetQuestWorldObjectiveCount(lpObj,&lpInfo))
		{
			GDCreateItemSend(aIndex,lpMonster->Map,(BYTE)lpMonster->X,(BYTE)lpMonster->Y,lpInfo.Index,lpInfo.Level,0,lpInfo.Option1,lpInfo.Option2,lpInfo.Option3,aIndex,lpInfo.NewOption,0,0,0,0,0xFF,0);
			return 1;
		}
	}

	#endif

	return 0;
}

bool CQuestWorldObjective::MonsterItemDropParty(LPOBJ lpMonster,int PartyNumber) // OK
{
	#if(GAMESERVER_UPDATE>=501)

	for(int n=0;n < MAX_PARTY_USER;n++)
	{
		int aIndex = gParty.m_PartyInfo[PartyNumber].Index[n];

		if(OBJECT_RANGE(aIndex) == 0)
		{
			continue;
		}

		LPOBJ lpObj = &gObj[aIndex];

		for each(QUEST_WORLD_OBJECTIVE_INFO lpInfo in this->m_QuestWorldObjectiveInfo)
		{
			if(this->CheckQuestWorldObjectiveRequisite(lpObj,&lpInfo) == 0)
			{
				continue;
			}

			if(lpInfo.Type != QUEST_WORLD_OBJECTIVE_ITEM)
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

			if(lpInfo.ItemDropRate > (GetLargeRand()%10000) && lpInfo.Quantity > this->GetQuestWorldObjectiveCount(lpObj,&lpInfo))
			{
				GDCreateItemSend(aIndex,lpMonster->Map,(BYTE)lpMonster->X,(BYTE)lpMonster->Y,lpInfo.Index,lpInfo.Level,0,lpInfo.Option1,lpInfo.Option2,lpInfo.Option3,aIndex,lpInfo.NewOption,0,0,0,0,0xFF,0);
				return 1;
			}
		}
	}

	#endif

	return 0;
}

void CQuestWorldObjective::MonsterKill(LPOBJ lpMonster) // OK
{
	#if(GAMESERVER_UPDATE>=501)

	int aIndex = gObjMonsterGetTopHitDamageUser(lpMonster);

	if(OBJECT_RANGE(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(OBJECT_RANGE(lpObj->PartyNumber) != 0 && gServerInfo.m_QuestWorldMonsterKillParty != 0)
	{
		return this->MonsterKillParty(lpMonster,lpObj->PartyNumber);
	}

	for each(QUEST_WORLD_OBJECTIVE_INFO lpInfo in this->m_QuestWorldObjectiveInfo)
	{
		if(this->CheckQuestWorldObjectiveRequisite(lpObj,&lpInfo) == 0)
		{
			continue;
		}

		if(lpInfo.Type != QUEST_WORLD_OBJECTIVE_MONSTER)
		{
			continue;
		}

		if(lpInfo.MapNumber != -1 && lpInfo.MapNumber != lpMonster->Map)
		{
			continue;
		}

		if(lpInfo.Index == lpMonster->Class && lpInfo.Quantity > this->GetQuestWorldObjectiveCount(lpObj,&lpInfo))
		{
			this->AddQuestWorldObjectiveKillCount(lpObj,&lpInfo);
			return;
		}
	}

	#endif
}

void CQuestWorldObjective::MonsterKillParty(LPOBJ lpMonster,int PartyNumber) // OK
{
	#if(GAMESERVER_UPDATE>=501)

	for(int n = 0; n < MAX_PARTY_USER; n++)
	{
		int aIndex = gParty.m_PartyInfo[PartyNumber].Index[n];

		if(OBJECT_RANGE(aIndex) == 0)
		{
			continue;
		}

		LPOBJ lpObj = &gObj[aIndex];

		for each(QUEST_WORLD_OBJECTIVE_INFO lpInfo in this->m_QuestWorldObjectiveInfo)
		{
			if(this->CheckQuestWorldObjectiveRequisite(lpObj,&lpInfo) == 0)
			{
				continue;
			}

			if(lpInfo.Type != QUEST_WORLD_OBJECTIVE_MONSTER)
			{
				continue;
			}

			if(lpInfo.MapNumber != -1 && lpInfo.MapNumber != lpMonster->Map)
			{
				continue;
			}

			if(lpInfo.Index == lpMonster->Class && lpInfo.Quantity > this->GetQuestWorldObjectiveCount(lpObj,&lpInfo))
			{
				this->AddQuestWorldObjectiveKillCount(lpObj,&lpInfo);
				return;
			}
		}
	}

	#endif
}

void CQuestWorldObjective::QuestWorldObjectiveSpecial(LPOBJ lpObj,int type,int index) // OK
{
	#if(GAMESERVER_UPDATE>=501)

	for each(QUEST_WORLD_OBJECTIVE_INFO lpInfo in this->m_QuestWorldObjectiveInfo)
	{
		if(this->CheckQuestWorldObjectiveRequisite(lpObj,&lpInfo) == 0)
		{
			continue;
		}

		if(lpInfo.Type != type)
		{
			continue;
		}

		if(lpInfo.MapNumber != -1 && lpInfo.MapNumber != lpObj->Map)
		{
			continue;
		}

		if(lpInfo.Type == QUEST_WORLD_OBJECTIVE_NPC && lpInfo.Index == 0)
		{
			if(lpObj->GensFamily == GENS_FAMILY_VARNERT && index != 543)
			{
				continue;
			}
			
			if(lpObj->GensFamily == GENS_FAMILY_DUPRIAN && index != 544)
			{
				continue;
			}
		}
		else
		{
			if(index != -1 && lpInfo.Index != index)
			{
				continue;
			}
		}

		if(lpInfo.Quantity > this->GetQuestWorldObjectiveCount(lpObj,&lpInfo))
		{
			this->AddQuestWorldObjectiveKillCount(lpObj,&lpInfo);
			return;
		}
	}

	#endif
}