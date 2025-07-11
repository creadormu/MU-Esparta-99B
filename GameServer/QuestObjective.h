// QuestObjective.h: interface for the CQuestObjective class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "DefaultClassInfo.h"
#include "User.h"

enum eQuestObjectiveType
{
	QUEST_OBJECTIVE_NONE = 0,
	QUEST_OBJECTIVE_ITEM = 1,
	QUEST_OBJECTIVE_MONEY = 2,
	QUEST_OBJECTIVE_MONSTER = 4,
};

struct QUEST_KILL_COUNT
{
	void Clear() // OK
	{
		this->MonsterClass = -1;
		this->KillCount = 0;
	}

	int MonsterClass;
	int KillCount;
};

struct QUEST_OBJECTIVE_INFO
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
	int MapNumber;
	int DropMinLevel;
	int DropMaxLevel;
	int ItemDropRate;
	int RequireIndex;
	int RequireState;
	int RequireClass[MAX_CLASS];
};

class CQuestObjective
{
public:
	CQuestObjective();
	virtual ~CQuestObjective();
	void Load(char* path);
	int GetQuestObjectiveCount(LPOBJ lpObj,QUEST_OBJECTIVE_INFO* lpInfo);
	int GetQuestObjectiveKillCount(LPOBJ lpObj,QUEST_OBJECTIVE_INFO* lpInfo);
	bool CheckQuestObjectiveRequisite(LPOBJ lpObj,QUEST_OBJECTIVE_INFO* lpInfo);
	bool CheckQuestObjective(LPOBJ lpObj,int QuestIndex);
	bool CheckQuestObjectiveItemCount(LPOBJ lpObj,int ItemIndex,int ItemLevel);
	void AddQuestObjectiveKillCount(LPOBJ lpObj,QUEST_OBJECTIVE_INFO* lpInfo);
	void InitQuestObjectiveKillCount(LPOBJ lpObj,int QuestIndex);
	void RemoveQuestObjective(LPOBJ lpObj,int QuestIndex);
	bool MonsterItemDrop(LPOBJ lpMonster);
	bool MonsterItemDropParty(LPOBJ lpMonster,int PartyNumber);
	void MonsterKill(LPOBJ lpMonster);
	void MonsterKillParty(LPOBJ lpMonster,int PartyNumber);
private:
	std::vector<QUEST_OBJECTIVE_INFO> m_QuestObjectiveInfo;
};

extern CQuestObjective gQuestObjective;
