// CustomLukeHelper.h: interface for the CCustomLukeHelper class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "User.h"

enum eCustomLukeHelperState
{
	CUSTOM_LUKE_HELPER_BLANK = 0,
	CUSTOM_LUKE_HELPER_EMPTY = 1,
	CUSTOM_LUKE_HELPER_START = 2,
};

struct CUSTOM_LUKE_HELPER_RESPAWN
{
	int Index;
	int Map;
	int Class;
	int StartX;
	int StartY;
	int EndX;
	int EndY;
	int BagSpecial;
};

struct CUSTOM_LUKE_HELPER_START_TIME
{
	int Year;
	int Month;
	int Day;
	int DayOfWeek;
	int Hour;
	int Minute;
	int Second;
};

class CCustomLukeHelper
{
public:
	CCustomLukeHelper();
	virtual ~CCustomLukeHelper();
	void Init();
	void ReadCustomLukeHelperInfo(char* section,char* path);
	void Load(char* path);
	CUSTOM_LUKE_HELPER_RESPAWN* GetInfo(int index);
	void MainProc();
	void ProcState_BLANK();
	void ProcState_EMPTY();
	void ProcState_START();
	void SetState(int state);
	void SetState_BLANK();
	void SetState_EMPTY();
	void SetState_START();
	void CheckSync();
	void SetMonster();
	bool NpcLukeHelper(LPOBJ lpNpc,LPOBJ lpObj);
private:
	int m_State;
	int m_RemainTime;
	int m_TargetTime;
	int m_EventTime;
	int m_MinutesLeft;
	int m_TickCount;
	int m_RespawnIndex;
	int m_LukeHelperIndex;
	int m_CustomLukeHelperSwitch;
	std::map<int,CUSTOM_LUKE_HELPER_RESPAWN> m_CustomLukeHelperRespawn;
	std::vector<CUSTOM_LUKE_HELPER_START_TIME> m_CustomLukeHelperStartTime;
};

extern CCustomLukeHelper gCustomLukeHelper;
