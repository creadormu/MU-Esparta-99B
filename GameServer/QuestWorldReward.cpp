// QuestWorldReward.cpp: implementation of the CQuestWorldReward class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuestWorldReward.h"
#include "DSProtocol.h"
#include "EffectManager.h"
#include "GensSystem.h"
#include "MemScript.h"
#include "ObjectManager.h"
#include "QuestWorld.h"
#include "RandomManager.h"
#include "ServerInfo.h"
#include "Util.h"

CQuestWorldReward gQuestWorldReward;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestWorldReward::CQuestWorldReward() // OK
{
	#if(GAMESERVER_UPDATE>=501)

	this->m_QuestWorldRewardInfo.clear();

	#endif
}

CQuestWorldReward::~CQuestWorldReward() // OK
{

}

void CQuestWorldReward::Load(char* path) // OK
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

	this->m_QuestWorldRewardInfo.clear();

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

				QUEST_WORLD_REWARD_INFO info;

				info.RequireGroup = section+1;

				info.Number = lpMemScript->GetNumber();

				info.Type = lpMemScript->GetAsNumber();

				info.Index = lpMemScript->GetAsNumber();

				info.Value = lpMemScript->GetAsNumber();

				info.Level = lpMemScript->GetAsNumber();

				info.Option1 = lpMemScript->GetAsNumber();

				info.Option2 = lpMemScript->GetAsNumber();

				info.Option3 = lpMemScript->GetAsNumber();

				info.NewOption = lpMemScript->GetAsNumber();

				info.ItemSortRate = lpMemScript->GetAsNumber();

				info.RequireIndex = lpMemScript->GetAsNumber();

				info.RequireState = lpMemScript->GetAsNumber();

				for(int n=0;n < MAX_CLASS;n++){info.RequireClass[n] = lpMemScript->GetAsNumber();}

				this->m_QuestWorldRewardInfo.push_back(info);
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

bool CQuestWorldReward::CheckQuestWorldRewardRequisite(LPOBJ lpObj,QUEST_WORLD_REWARD_INFO* lpInfo) // OK
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

void CQuestWorldReward::InsertQuestWorldReward(LPOBJ lpObj,int QuestIndex,int QuestGroup) // OK
{
	#if(GAMESERVER_UPDATE>=501)

	CRandomManager RandomManager;

	for(size_t n=0;n < this->m_QuestWorldRewardInfo.size();n++)
	{
		QUEST_WORLD_REWARD_INFO* lpInfo = &this->m_QuestWorldRewardInfo[n];

		if(lpInfo->RequireIndex != QuestIndex || lpInfo->RequireGroup != QuestGroup)
		{
			continue;
		}

		if(this->CheckQuestWorldRewardRequisite(lpObj,lpInfo) == 0)
		{
			continue;
		}

		if(lpInfo->ItemSortRate != -1)
		{
			RandomManager.AddElement((int)(&(*lpInfo)),lpInfo->ItemSortRate);
			continue;
		}

		if(lpInfo->Type == QUEST_WORLD_REWARD_EXPERIENCE)
		{
			if(gEffectManager.CheckEffect(lpObj,EFFECT_SEAL_OF_SUSTENANCE1) != 0 || gEffectManager.CheckEffect(lpObj,EFFECT_SEAL_OF_SUSTENANCE2) != 0)
			{
				continue;
			}

			if(gObjectManager.CharacterLevelUp(lpObj,(lpInfo->Value*gServerInfo.m_AddQuestExperienceRate[lpObj->AccountLevel]),gServerInfo.m_MaxLevelUpQuest,EXPERIENCE_QUEST) != 0)
			{
				continue;
			}

			GCRewardExperienceSend(lpObj->Index,(lpInfo->Value*gServerInfo.m_AddQuestExperienceRate[lpObj->AccountLevel]));
		}
		
		if(lpInfo->Type == QUEST_WORLD_REWARD_MONEY)
		{
			if(gObjCheckMaxMoney(lpObj->Index,lpInfo->Value) == 0)
			{
				lpObj->Money = MAX_MONEY;
			}
			else
			{
				lpObj->Money += lpInfo->Value;
			}

			GCMoneySend(lpObj->Index,lpObj->Money);
		}
		
		if(lpInfo->Type == QUEST_WORLD_REWARD_ITEM)
		{
			for(int n=0;n < lpInfo->Value;n++)
			{
				GDCreateItemSend(lpObj->Index,0xEB,0,0,lpInfo->Index,lpInfo->Level,0,lpInfo->Option1,lpInfo->Option2,lpInfo->Option3,-1,lpInfo->NewOption,0,0,0,0,0xFF,0);
			}
		}
		
		if(lpInfo->Type == QUEST_WORLD_REWARD_EFFECT)
		{
			gEffectManager.AddEffect(lpObj,1,lpInfo->Index,lpInfo->Value,0,0,0,0);
		}
		
		if(lpInfo->Type == QUEST_WORLD_REWARD_CONTRIBUTION)
		{
			lpObj->GensContribution += lpInfo->Value;
			gGensSystem.GDGensSystemUpdateSend(lpObj->Index);
		}
	}

	QUEST_WORLD_REWARD_INFO* lpInfo;

	if(RandomManager.GetRandomElement((int*)&lpInfo) != 0)
	{
		if(lpInfo->Type == QUEST_WORLD_REWARD_EXPERIENCE)
		{
			if(gEffectManager.CheckEffect(lpObj,EFFECT_SEAL_OF_SUSTENANCE1) != 0 || gEffectManager.CheckEffect(lpObj,EFFECT_SEAL_OF_SUSTENANCE2) != 0)
			{
				return;
			}

			if(gObjectManager.CharacterLevelUp(lpObj,(lpInfo->Value*gServerInfo.m_AddQuestExperienceRate[lpObj->AccountLevel]),gServerInfo.m_MaxLevelUpQuest,EXPERIENCE_QUEST) != 0)
			{
				return;
			}

			GCRewardExperienceSend(lpObj->Index,(lpInfo->Value*gServerInfo.m_AddQuestExperienceRate[lpObj->AccountLevel]));
		}

		if(lpInfo->Type == QUEST_WORLD_REWARD_MONEY)
		{
			if(gObjCheckMaxMoney(lpObj->Index,lpInfo->Value) == 0)
			{
				lpObj->Money = MAX_MONEY;
			}
			else
			{
				lpObj->Money += lpInfo->Value;
			}

			GCMoneySend(lpObj->Index,lpObj->Money);
		}
		
		if(lpInfo->Type == QUEST_WORLD_REWARD_ITEM)
		{
			for(int n=0;n < lpInfo->Value;n++)
			{
				GDCreateItemSend(lpObj->Index,0xEB,0,0,lpInfo->Index,lpInfo->Level,0,lpInfo->Option1,lpInfo->Option2,lpInfo->Option3,-1,lpInfo->NewOption,0,0,0,0,0xFF,0);
			}
		}
		
		if(lpInfo->Type == QUEST_WORLD_REWARD_EFFECT)
		{
			gEffectManager.AddEffect(lpObj,1,lpInfo->Index,lpInfo->Value,0,0,0,0);
		}
		
		if(lpInfo->Type == QUEST_WORLD_REWARD_CONTRIBUTION)
		{
			lpObj->GensContribution += lpInfo->Value;
			gGensSystem.GDGensSystemUpdateSend(lpObj->Index);
		}
	}

	#endif
}

void CQuestWorldReward::GCQuestWorldDetailSend(LPOBJ lpObj,int QuestIndex,int QuestGroup,BYTE protocol) // OK
{
	#if(GAMESERVER_UPDATE>=501)

	PMSG_QUEST_WORLD_DETAIL_SEND pMsg;

	pMsg.header.set(0xF6,protocol,sizeof(pMsg));

	pMsg.QuestIndex = QuestIndex;

	pMsg.QuestGroup = QuestGroup;

	pMsg.ObjectiveCount = 0;

	pMsg.RewardCount = 0;

	pMsg.result = 0;

	for(int n=0;n < MAX_QUEST_WORLD_OBJECTIVE_COUNT;n++)
	{
		if(gQuestWorldObjective.GetQuestWorldObjective(lpObj,QuestIndex,QuestGroup,&pMsg.QuestObjective[n],n) == 0)
		{
			memset(&pMsg.QuestObjective[n],0,sizeof(pMsg.QuestObjective[n]));
		}
		else
		{
			pMsg.ObjectiveCount++;
		}

		memset(&pMsg.QuestReward[n],0,sizeof(pMsg.QuestReward[n]));
	}

	for each(QUEST_WORLD_REWARD_INFO lpInfo in this->m_QuestWorldRewardInfo)
	{
		if(lpInfo.RequireIndex != QuestIndex || lpInfo.RequireGroup != QuestGroup)
		{
			continue;
		}

		if(this->CheckQuestWorldRewardRequisite(lpObj,&lpInfo) == 0)
		{
			continue;
		}

		pMsg.QuestReward[pMsg.RewardCount].type = lpInfo.Type;

		pMsg.QuestReward[pMsg.RewardCount].index = lpInfo.Index;

		pMsg.QuestReward[pMsg.RewardCount].value = lpInfo.Value;

		if(lpInfo.Type == QUEST_WORLD_REWARD_ITEM)
		{
			CItem item;

			item.m_Level = lpInfo.Level;

			item.Convert(lpInfo.Index,lpInfo.Option1,lpInfo.Option2,lpInfo.Option3,lpInfo.NewOption,0,0,0,0,0xFF);

			gItemManager.ItemByteConvert(pMsg.QuestReward[pMsg.RewardCount].ItemInfo,item);
		}

		if(lpInfo.ItemSortRate != -1)
		{
			pMsg.QuestReward[pMsg.RewardCount].type |= 0x20;
			pMsg.result++;
		}

		if((++pMsg.RewardCount) >= MAX_QUEST_WORLD_OBJECTIVE_COUNT)
		{
			break;
		}
	}

	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);

	#endif
}