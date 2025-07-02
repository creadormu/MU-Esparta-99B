// BonusManager.h: interface for the CBonusManager class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "User.h"

#define MAX_BONUS 30

enum eBonusState
{
	BONUS_STATE_BLANK = 0,
	BONUS_STATE_EMPTY = 1,
	BONUS_STATE_START = 2,
};

enum eBonusIndex
{
	BONUS_INDEX_EXPERIENCE_RATE = 0,
	BONUS_INDEX_MASTER_EXPERIENCE_RATE = 1,
	BONUS_INDEX_ITEM_DROP_RATE = 2,
	BONUS_INDEX_EXC_ITEM_DROP_RATE = 3,
	BONUS_INDEX_SET_ITEM_DROP_RATE = 4,
	BONUS_INDEX_COIN1 = 5,
	BONUS_INDEX_COIN2 = 6,
	BONUS_INDEX_COIN3 = 7,
	BONUS_INDEX_CHAOSMIX_RATE = 8,
};

struct BONUS_START_TIME
{
	int Year;
	int Month;
	int Day;
	int DayOfWeek;
	int Hour;
	int Minute;
	int Second;
};

struct BONUS_VALUE_INFO
{
	int BonusIndex;
	int BonusValue[MAX_ACCOUNT_LEVEL];
	int MapNumber;
};

struct BONUS_INFO
{
	int Index;
	int State;
	int RemainTime;
	int TargetTime;
	int TickCount;
	int StartMessage;
	int FinalMessage;
	int BonusTime;
	std::vector<BONUS_START_TIME> StartTime;
	std::vector<BONUS_VALUE_INFO> ValueInfo;
};

class CBonusManager
{
public:
	CBonusManager();
	virtual ~CBonusManager();
	void Init();
	void Load(char* path);
	void MainProc();
	void ProcState_BLANK(BONUS_INFO* lpInfo);
	void ProcState_EMPTY(BONUS_INFO* lpInfo);
	void ProcState_START(BONUS_INFO* lpInfo);
	void SetState(BONUS_INFO* lpInfo,int state);
	void SetState_BLANK(BONUS_INFO* lpInfo);
	void SetState_EMPTY(BONUS_INFO* lpInfo);
	void SetState_START(BONUS_INFO* lpInfo);
	void CheckSync(BONUS_INFO* lpInfo);
	int GetBonusValue(LPOBJ lpObj,int BonusIndex,int BonusValue);
private:
	BONUS_INFO m_BonusInfo[MAX_BONUS];
};

extern CBonusManager gBonusManager;
