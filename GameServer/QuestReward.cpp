// QuestReward.cpp: implementation of the CQuestReward class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuestReward.h"
#include "EffectManager.h"
#include "DSProtocol.h"
#include "MemScript.h"
#include "ObjectManager.h"
#include "Quest.h"
#include "ServerInfo.h"
#include "SkillManager.h"
#include "Util.h"

CQuestReward gQuestReward;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestReward::CQuestReward() // OK
{
	this->m_QuestRewardInfo.clear();
}

CQuestReward::~CQuestReward() // OK
{

}

void CQuestReward::Load(char* path) // OK
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

	this->m_QuestRewardInfo.clear();

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

			QUEST_REWARD_INFO info;

			info.Sort = lpMemScript->GetNumber();

			info.Type = lpMemScript->GetAsNumber();

			info.Index = lpMemScript->GetAsNumber();

			info.Quantity = lpMemScript->GetAsNumber();

			info.Level = lpMemScript->GetAsNumber();

			info.Option1 = lpMemScript->GetAsNumber();

			info.Option2 = lpMemScript->GetAsNumber();

			info.Option3 = lpMemScript->GetAsNumber();

			info.NewOption = lpMemScript->GetAsNumber();

			info.RequireIndex = lpMemScript->GetAsNumber();

			info.RequireState = lpMemScript->GetAsNumber();

			for(int n=0;n < MAX_CLASS;n++){info.RequireClass[n] = lpMemScript->GetAsNumber();}

			this->m_QuestRewardInfo.push_back(info);
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

bool CQuestReward::CheckQuestRewardRequisite(LPOBJ lpObj,QUEST_REWARD_INFO* lpInfo) // OK
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

void CQuestReward::InsertQuestReward(LPOBJ lpObj,int QuestIndex) // OK
{
	for each(QUEST_REWARD_INFO lpInfo in this->m_QuestRewardInfo)
	{
		if(lpInfo.RequireIndex != QuestIndex)
		{
			continue;
		}

		if(this->CheckQuestRewardRequisite(lpObj,&lpInfo) == 0)
		{
			continue;
		}

		if(lpInfo.Type == QUEST_REWARD_POINT)
		{
			lpObj->LevelUpPoint += lpInfo.Quantity;
			gQuest.GCQuestRewardSend(lpObj->Index,lpInfo.Index,lpInfo.Quantity);
			continue;
		}

		if(lpInfo.Type == QUEST_REWARD_CHANGE1)
		{
			if((lpObj->DBClass%16) < 1)
			{
				lpObj->DBClass = ((lpObj->DBClass/16)*16)+1;
				lpObj->ChangeUp = lpObj->DBClass%16;
			}

			gObjectManager.CharacterMakePreviewCharSet(lpObj->Index);

			BYTE Class = (lpObj->ChangeUp*16);
			Class -= (Class/32);
			Class += (lpObj->Class*32);

			gQuest.GCQuestRewardSend(lpObj->Index,lpInfo.Index,Class);
			continue;
		}

		if(lpInfo.Type == QUEST_REWARD_EXPERIENCE)
		{
			if(gEffectManager.CheckEffect(lpObj,EFFECT_SEAL_OF_SUSTENANCE1) != 0 || gEffectManager.CheckEffect(lpObj,EFFECT_SEAL_OF_SUSTENANCE2) != 0)
			{
				continue;
			}

			if(gObjectManager.CharacterLevelUp(lpObj,(lpInfo.Quantity*gServerInfo.m_AddQuestExperienceRate[lpObj->AccountLevel]),gServerInfo.m_MaxLevelUpQuest,EXPERIENCE_QUEST) != 0)
			{
				continue;
			}

			GCRewardExperienceSend(lpObj->Index,(lpInfo.Quantity*gServerInfo.m_AddQuestExperienceRate[lpObj->AccountLevel]));
			continue;
		}

		if(lpInfo.Type == QUEST_REWARD_HERO)
		{
			if(lpObj->Class == CLASS_FE)
			{
				gSkillManager.GCSkillAddSend(lpObj->Index,gSkillManager.AddSkill(lpObj,SKILL_INFINITY_ARROW,0),SKILL_INFINITY_ARROW,0,0);
			}

			int AddPoint = ((lpObj->Level>gServerInfo.m_PlusStatMinLevel)?(lpObj->Level- gServerInfo.m_PlusStatMinLevel):0)*gServerInfo.m_PlusStatPoint;

			lpObj->LevelUpPoint += AddPoint;

			gQuest.GCQuestRewardSend(lpObj->Index,lpInfo.Index,AddPoint);
			continue;
		}

		if(lpInfo.Type == QUEST_REWARD_MONEY)
		{
			if(gObjCheckMaxMoney(lpObj->Index,lpInfo.Quantity) == 0)
			{
				lpObj->Money = MAX_MONEY;
			}
			else
			{
				lpObj->Money += lpInfo.Quantity;
			}

			GCMoneySend(lpObj->Index,lpObj->Money);
			continue;
		}

		if(lpInfo.Type == QUEST_REWARD_ITEM)
		{
			for(int n=0;n < lpInfo.Quantity;n++)
			{
				GDCreateItemSend(lpObj->Index,0xEB,0,0,lpInfo.Index,lpInfo.Level,0,lpInfo.Option1,lpInfo.Option2,lpInfo.Option3,-1,lpInfo.NewOption,0,0,0,0,0xFF,0);
			}
			continue;
		}

		if(lpInfo.Type == QUEST_REWARD_EFFECT)
		{
			gEffectManager.AddEffect(lpObj,1,lpInfo.Index,lpInfo.Quantity,0,0,0,0);
			continue;
		}

		if(lpInfo.Type == QUEST_REWARD_COMBO)
		{
			gQuest.GCQuestRewardSend(lpObj->Index,lpInfo.Index,lpInfo.Quantity);
			continue;
		}

		if(lpInfo.Type == QUEST_REWARD_CHANGE2)
		{
			if((lpObj->DBClass%16) < 2)
			{
				lpObj->DBClass = ((lpObj->DBClass/16)*16)+2;
				lpObj->ChangeUp = lpObj->DBClass%16;
			}

			gObjectManager.CharacterMakePreviewCharSet(lpObj->Index);

			BYTE Class = (lpObj->ChangeUp*16);
			Class -= (Class/32);
			Class += (lpObj->Class*32);

			gQuest.GCQuestRewardSend(lpObj->Index,lpInfo.Index,Class);
			continue;
		}
	}
}

long CQuestReward::GetQuestRewardPoint(LPOBJ lpObj,int QuestIndex) // OK
{
	long point = 0;

	for each(QUEST_REWARD_INFO lpInfo in this->m_QuestRewardInfo)
	{
		if(lpInfo.RequireIndex != QuestIndex)
		{
			continue;
		}

		if(lpInfo.Type == QUEST_REWARD_POINT)
		{
			point += lpInfo.Quantity;
			continue;
		}
	}

	return point;
}
