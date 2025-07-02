// DropEvent.h: interface for the CDropEvent class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "User.h"

#define MAX_DROP_EVENT 30

enum eDropEventState
{
	DROP_EVENT_STATE_BLANK = 0,
	DROP_EVENT_STATE_EMPTY = 1,
	DROP_EVENT_STATE_START = 2,
};

struct DROP_EVENT_START_TIME
{
	int Year;
	int Month;
	int Day;
	int DayOfWeek;
	int Hour;
	int Minute;
	int Second;
};

struct DROP_EVENT_ITEM_INFO
{
	int ItemIndex;
	int Level;
	int Durability;
	int Option0;
	int Option1;
	int Option2;
	int Option3;
	int Option4;
	int Option5;
	int Option6;
	int Duration;
	int MapNumber;
	int MonsterClass;
	int MonsterLevelMin;
	int MonsterLevelMax;
	int AccountLevel;
	int DropRate;
};

struct DROP_EVENT_INFO
{
	int Index;
	int State;
	int RemainTime;
	int TargetTime;
	int TickCount;
	int StartMessage;
	int FinalMessage;
	int DropEventTime;
	std::vector<DROP_EVENT_START_TIME> StartTime;
	std::vector<DROP_EVENT_ITEM_INFO> ItemInfo;
};

class CDropEvent
{
public:
	CDropEvent();
	virtual ~CDropEvent();
	void Init();
	void Load(char* path);
	void MainProc();
	void ProcState_BLANK(DROP_EVENT_INFO* lpInfo);
	void ProcState_EMPTY(DROP_EVENT_INFO* lpInfo);
	void ProcState_START(DROP_EVENT_INFO* lpInfo);
	void SetState(DROP_EVENT_INFO* lpInfo,int state);
	void SetState_BLANK(DROP_EVENT_INFO* lpInfo);
	void SetState_EMPTY(DROP_EVENT_INFO* lpInfo);
	void SetState_START(DROP_EVENT_INFO* lpInfo);
	void CheckSync(DROP_EVENT_INFO* lpInfo);
	int GetState(int index);
	int GetRemainTime(int index);
	int DropItem(LPOBJ lpObj,LPOBJ lpTarget);
	int GetItemDropRate(LPOBJ lpObj,LPOBJ lpTarget,int ItemIndex,int DropRate);
private:
	DROP_EVENT_INFO m_EventDropInfo[MAX_DROP_EVENT];
};

extern CDropEvent gDropEvent;