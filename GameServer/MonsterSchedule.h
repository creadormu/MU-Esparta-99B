// MonsterSchedule.h: interface for the CMonsterSchedule class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "User.h"

#define MAX_MONSTER_SCHEDULE 100

enum eMonsterScheduleState
{
	MONSTER_SCHEDULE_STATE_BLANK = 0,
	MONSTER_SCHEDULE_STATE_EMPTY = 1,
	MONSTER_SCHEDULE_STATE_START = 2,
};

struct MONSTER_SCHEDULE_START_TIME
{
	int Year;
	int Month;
	int Day;
	int DayOfWeek;
	int Hour;
	int Minute;
	int Second;
};

struct MONSTER_SCHEDULE_VALUE_INFO
{
	int Index;
	int MapNumber;
	int PosX;
	int PosY;
};

struct MONSTER_SCHEDULE_INFO
{
	int Index;
	int State;
	int RemainTime;
	int TargetTime;
	int TickCount;
	int StartMessage;
	int FinalMessage;
	int NpcMessage;
	int EnableTime;
	std::vector<MONSTER_SCHEDULE_START_TIME> StartTime;
	std::vector<MONSTER_SCHEDULE_VALUE_INFO> ValueInfo;
};

class CMonsterSchedule
{
public:
	CMonsterSchedule();
	virtual ~CMonsterSchedule();
	void Init();
	void Load(char* path);
	void MainProc();
	void ProcState_BLANK(MONSTER_SCHEDULE_INFO* lpInfo);
	void ProcState_EMPTY(MONSTER_SCHEDULE_INFO* lpInfo);
	void ProcState_START(MONSTER_SCHEDULE_INFO* lpInfo);
	void SetState(MONSTER_SCHEDULE_INFO* lpInfo,int state);
	void SetState_BLANK(MONSTER_SCHEDULE_INFO* lpInfo);
	void SetState_EMPTY(MONSTER_SCHEDULE_INFO* lpInfo);
	void SetState_START(MONSTER_SCHEDULE_INFO* lpInfo);
	void CheckSync(MONSTER_SCHEDULE_INFO* lpInfo);
	int CheckEnableNpc(LPOBJ lpObj,LPOBJ lpNpc);
private:
	MONSTER_SCHEDULE_INFO m_MonsterScheduleInfo[MAX_MONSTER_SCHEDULE];
};

extern CMonsterSchedule gMonsterSchedule;