// CustomMonsterKillCount.h: interface for the CCustomMonsterKillCount class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "User.h"
#include "Protocol.h"

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_MONSTER_KILL_COUNT_RECV
{
	PBMSG_HEAD header; // C1:50
	WORD Index;
	char Account[11];
	char Name[11];
	WORD MonsterClass;
	WORD Count;
};

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_MONSTER_KILL_COUNT_SEND
{
	PBMSG_HEAD header; // C1:50
	WORD Index;
	char Account[11];
	char Name[11];
	WORD MonsterClass;
};

//**********************************************//
//**********************************************//
//**********************************************//

struct MONSTER_KILL_ITEM_LIST
{
	int Index;
	int ItemIndex;
	int Level;
	int Dur;
	int Option1;
	int Option2;
	int Option3;
	int NewOption;
	int SetOption;
	int SocketCount;
	int Duration;
};

struct MONSTER_KILL_COUNT_INFO
{
	int Index;
	int MonsterClass;
	int Enable[MAX_ACCOUNT_LEVEL];
	int KillCount;
	int Money;
	int Coin1;
	int Coin2;
	int Coin3;
	char Message[128];
	std::vector<MONSTER_KILL_ITEM_LIST> ItemList;
};

class CCustomMonsterKillCount
{
public:
	CCustomMonsterKillCount();
	virtual ~CCustomMonsterKillCount();
	void Load(char* path);
	void SetInfo(MONSTER_KILL_COUNT_INFO info);
	void SetItemInfo(MONSTER_KILL_ITEM_LIST info);
	void CheckMonsterKill(LPOBJ lpObj,LPOBJ lpTarget);
	void DGMonsterKillCountRecv(SDHP_MONSTER_KILL_COUNT_RECV* lpMsg);
private:
	std::map<int,MONSTER_KILL_COUNT_INFO> m_MonsterKillCountInfo;
};

extern CCustomMonsterKillCount gCustomMonsterKillCount;