// CustomQuest.h: interface for the CCustomQuest class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "DefaultClassInfo.h"

#define MAX_CUSTOM_QUEST_OBJECTIVE_COUNT 5

enum eCustomQuestObjectiveType
{
	CUSTOM_QUEST_OBJECTIVE_ITEM = 1,
	CUSTOM_QUEST_OBJECTIVE_MONEY = 2,
	CUSTOM_QUEST_OBJECTIVE_SKILL = 3,
	CUSTOM_QUEST_OBJECTIVE_BUFF = 4,
	CUSTOM_QUEST_OBJECTIVE_MONSTER = 5,
};

enum eCustomQuestRewardType
{
	CUSTOM_QUEST_REWARD_ITEM = 0,
	CUSTOM_QUEST_REWARD_MONEY = 1,	
	CUSTOM_QUEST_REWARD_EFFECT = 2,
	CUSTOM_QUEST_REWARD_EXPERIENCE = 3,
};

//**********************************************//
//**********************************************//
//**********************************************//

struct CUSTOM_QUEST_LIST
{
	WORD QuestIndex;
	BYTE QuestState;
	WORD ObjectiveCount[MAX_CUSTOM_QUEST_OBJECTIVE_COUNT];
};

struct CUSTOM_QUEST_INFO
{
	int Index;
	int MonsterClass;
	int RequireMinLevel;
	int RequireMaxLevel;
	int RequireMinReset;
	int RequireMaxReset;
	int StartMessage;
	int CompleteMessage;
	int RequireClass[MAX_CLASS];
};

class CCustomQuest
{
public:
	CCustomQuest();
	virtual ~CCustomQuest();
	void Load(char* path);
private:
	std::map<int,CUSTOM_QUEST_INFO> m_CustomQuestInfo;
};

extern CCustomQuest gCustomQuest;