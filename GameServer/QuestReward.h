// QuestReward.h: interface for the CQuestReward class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "DefaultClassInfo.h"
#include "User.h"

enum eQuestRewardType
{
	QUEST_REWARD_NONE = 0,
	QUEST_REWARD_POINT = 1,
	QUEST_REWARD_CHANGE1 = 2,
	QUEST_REWARD_EXPERIENCE = 3,
	QUEST_REWARD_HERO = 4,
	QUEST_REWARD_MONEY = 5,
	QUEST_REWARD_ITEM = 6,
	QUEST_REWARD_EFFECT = 7,
	QUEST_REWARD_COMBO = 8,
	QUEST_REWARD_CHANGE2 = 16,
};

struct QUEST_REWARD_INFO
{
	int Sort;
	int Type;
	int Index;
	int Quantity;
	int Level;
	int Option1;
	int Option2;
	int Option3;
	int NewOption;
	int RequireIndex;
	int RequireState;
	int RequireClass[MAX_CLASS];
};

class CQuestReward
{
public:
	CQuestReward();
	virtual ~CQuestReward();
	void Load(char* path);
	bool CheckQuestRewardRequisite(LPOBJ lpObj,QUEST_REWARD_INFO* lpInfo);
	void InsertQuestReward(LPOBJ lpObj,int QuestIndex);
	long GetQuestRewardPoint(LPOBJ lpObj,int QuestIndex);
private:
	std::vector<QUEST_REWARD_INFO> m_QuestRewardInfo;
};

extern CQuestReward gQuestReward;
