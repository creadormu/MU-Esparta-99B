// AcheronGuardian.h: interface for the CAcheronGuardian class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Map.h"
#include "Protocol.h"
#include "User.h"

enum eAcheronGuardianState
{
	AG_STATE_BLANK = 0,
	AG_STATE_EMPTY = 1,
	AG_STATE_START = 2,
	AG_STATE_CLEAN = 3,
};

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_ACHERON_GUARDIAN_SYNC_RECV
{
	PBMSG_HEAD header; // C1:14
	WORD MapServerGroup;
	DWORD AcheronState;
};

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_ACHERON_GUARDIAN_SYNC_SEND
{
	PBMSG_HEAD header; // C1:14
	WORD MapServerGroup;
	DWORD AcheronState;
};

//**********************************************//
//**********************************************//
//**********************************************//

struct ACHERON_GUARDIAN_START_TIME
{
	int Year;
	int Month;
	int Day;
	int DayOfWeek;
	int Hour;
	int Minute;
	int Second;
};

struct ACHERON_GUARDIAN_GROUP
{
	int Index;
	int ObeliskMapN;
	int ObeliskMapX;
	int ObeliskMapY;
	int Rate;
};

struct ACHERON_GUARDIAN_SPAWN
{
	int Index;
	int Group;
	int Minute;
};

struct ACHERON_GUARDIAN_MONSTER
{
	int Index;
	int Group;
	int MonsterClass;
	int Count;
	int X;
	int Y;
	int TX;
	int TY;
};

class CAcheronGuardian
{
public:
	CAcheronGuardian();
	virtual ~CAcheronGuardian();
	void Init();
	void Load(char* path);
	void MainProc();
	void ProcState_BLANK();
	void ProcState_EMPTY();
	void ProcState_START();
	void ProcState_CLEAN();
	void SetState(int state);
	void SetState_BLANK();
	void SetState_EMPTY();
	void SetState_START();
	void SetState_CLEAN();
	void CheckSync();
	int GetState();
	int GetSyncState();
	void ClearUser();
	void ClearMonster();
	void AddMonster(int stage,int group);
	void AddObelisk();
	void DropTowerItem(LPOBJ lpObj, LPOBJ lpTarget);
	void DataSendToAll(BYTE* lpMsg, int size);
	void LoadAcheronMapAttr(char* path,int map,int state);
	void SetAcheronMapAttr(int state);
	void GDAcheronGuardianSyncSend(int state);
	void DGAcheronGuardianSyncRecv(SDHP_ACHERON_GUARDIAN_SYNC_RECV* lpMsg);
	void ForceStart();
public:
	int m_State;
	int m_SyncState;
	int m_RemainTime;
	int m_TargetTime;
	int m_TickCount;
	int m_TimeNotify;
	int m_Stage;
	int m_Group;
	int m_Map;
	int m_ObeliskX;
	int m_ObeliskY;
	std::vector<ACHERON_GUARDIAN_START_TIME> m_AcheronGuardianStartTime;
	std::vector<ACHERON_GUARDIAN_GROUP> m_AcheronGuardianGroup;
	std::vector<ACHERON_GUARDIAN_SPAWN> m_AcheronGuardianSpawn;
	std::vector<ACHERON_GUARDIAN_MONSTER> m_AcheronGuardianMonster;
	CMap m_AcheronMapAttr[2][2];
};

extern CAcheronGuardian gAcheronGuardian;
