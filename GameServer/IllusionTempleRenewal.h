// IllusionTempleRenewal.h: interface for the CIllusionTempleRenewal class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "IllusionTemple.h"
#include "MapItem.h"
#include "Protocol.h"
#include "User.h"

enum eIllusionTempleRenewalState
{
	ITR_STATE_BLANK = 0,
	ITR_STATE_EMPTY = 1,
	ITR_STATE_STAND = 2,
	ITR_STATE_START = 3,
	ITR_STATE_CLEAN = 4,
};

enum eIllusionTempleRenewalTeam
{
	ITR_TEAM_NONE = -1,
	ITR_TEAM_Y = 0,
	ITR_TEAM_B = 1,
};

//**********************************************//
//************ Client -> GameServer ************//
//**********************************************//

struct PMSG_ILLUSION_TEMPLE_RENEWAL_SKILL_RECV
{
	PSBMSG_HEAD header; // C1:BF:02
	BYTE skill[2];
	BYTE index[2];
	BYTE dir;
};

struct PMSG_ILLUSION_TEMPLE_RENEWAL_STORAGE_RECV
{
	PSBMSG_HEAD header; // C1:BF:6A
	BYTE index[2];
	WORD Class;
	BYTE status[2];
};

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_ILLUSION_TEMPLE_RENEWAL_REWARD_SCORE_SEND // OK
{
	PSBMSG_HEAD header; // C1:BF:04
	BYTE score[2];
	BYTE count;
	BYTE winner;
};

struct PMSG_ILLUSION_TEMPLE_RENEWAL_REWARD_SCORE // OK
{
	char name[10];
	BYTE map;
	BYTE team;
	BYTE Class;
	BYTE Point;
};

struct PMSG_ILLUSION_TEMPLE_RENEWAL_CHANGE_SCENARIO_SEND
{
	PSBMSG_HEAD header; // C1:BF:60
	BYTE level;
	BYTE state;
};

struct PMSG_ILLUSION_TEMPLE_RENEWAL_EVENT_ITEM_OWNER_SEND // OK
{
	PSBMSG_HEAD header; // C1:BF:61
	WORD index;
	char name[10];
	BYTE state;
};

struct PMSG_ILLUSION_TEMPLE_RENEWAL_BATTLE_INFO_SEND // OK
{
	PSBMSG_HEAD header; // C1:BF:62
	WORD time;
	BYTE score[2];
	BYTE team;
};

struct PMSG_ILLUSION_TEMPLE_RENEWAL_DEL_SKILL_SEND // OK
{
	PSBMSG_HEAD header; // C1:BF:63
	BYTE skill[2];
	WORD index;
};

struct PMSG_ILLUSION_TEMPLE_RENEWAL_ADD_SKILL_SEND // OK
{
	PSBMSG_HEAD header; // C1:BF:67
	BYTE result;
	BYTE skill[2];
	WORD index;
	WORD target;
};

struct PMSG_ILLUSION_TEMPLE_RENEWAL_RESULT_SEND // OK
{
	PSBMSG_HEAD header; // C1:BF:6A
	BYTE result;
	BYTE index[2];
	WORD Class;
	BYTE status;
};

struct PMSG_ILLUSION_TEMPLE_RENEWAL_ENTER_SEND // OK
{
	PSBMSG_HEAD header; // C1:BF:70
	BYTE result;
	char name[11];
};

//**********************************************//
//**********************************************//
//**********************************************//

struct ILLUSION_TEMPLE_RENEWAL_START_TIME
{
	int Year;
	int Month;
	int Day;
	int DayOfWeek;
	int Hour;
	int Minute;
	int Second;
};

struct ILLUSION_TEMPLE_RENEWAL_USER
{
	int Index;
	int Team;
	int Point;
	int Score;
};

struct ILLUSION_TEMPLE_RENEWAL_LEVEL
{
	int Level;
	int State;
	int Map;
	int RemainTime;
	int TargetTime;
	int TickCount;
	int EnterEnabled;
	int MinutesLeft;
	int TimeCount;
	int Score[MAX_IT_TEAM];
	int EventItemSerial;
	int EventItemNumber;
	int EventItemOwner;
	int EventItemLevel;
	int EventItemStatus;
	int PartyNumber[MAX_IT_TEAM];
	int WinnerTeam;
	ILLUSION_TEMPLE_RENEWAL_USER User[MAX_IT_USER];
	int StoneStatueIndex;
	int MonsterIndex[MAX_IT_MONSTER];
};

class CIllusionTempleRenewal
{
public:
	CIllusionTempleRenewal();
	virtual ~CIllusionTempleRenewal();
	void Init();
	void Load(char* path);
	void MainProc();
	void ProcState_BLANK(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel);
	void ProcState_EMPTY(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel);
	void ProcState_STAND(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel);
	void ProcState_START(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel);
	void ProcState_CLEAN(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel);
	void SetState(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel,int state);
	void SetState_BLANK(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel);
	void SetState_EMPTY(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel);
	void SetState_STAND(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel);
	void SetState_START(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel);
	void SetState_CLEAN(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel);
	void CheckSync(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel);
	int GetState(int level);
	int GetRemainTime(int level);
	int GetEnterEnabled(int level);
	int GetEnteredUserCount(int level);
	bool AddUser(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel,int aIndex);
	bool DelUser(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel,int aIndex);
	ILLUSION_TEMPLE_RENEWAL_USER* GetUser(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel,int aIndex);
	void CleanUser(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel);
	void ClearUser(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel);
	void CheckUser(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel);
	int GetUserCount(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel,int team);
	int GetUserJoinTeam(LPOBJ lpObj);
	bool GetUserRespawnLocation(LPOBJ lpObj,int* gate,int* map,int* x,int* y,int* dir,int* level);
	void SearchUserDropEventItem(int aIndex);
	void SearchUserDeleteEventItem(int aIndex);
	bool AddMonster(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel,int aIndex);
	bool DelMonster(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel,int aIndex);
	int* GetMonster(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel,int aIndex);
	void CleanMonster(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel);
	void ClearMonster(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel);
	int GetMonsterCount(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel);
	void SetStoneStatue(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel);
	void StageSpawn(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel,int Stage);
	void SetMonster(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel,int MonsterClass,int MaxRegenTime);
	bool CheckEventItemSerial(int map,CMapItem* lpItem);
	void SetEventItemSerial(int map,int index,int serial);
	void DropEventItem(int map,int number,int aIndex);
	void GetEventItem(int map,int aIndex,CMapItem* lpItem);
	void DestroyEventItem(int map,CMapItem* lpItem);
	void UserDieProc(LPOBJ lpObj,LPOBJ lpTarget);
	void MonsterDieProc(LPOBJ lpObj,LPOBJ lpTarget);
	bool CheckPlayerTarget(LPOBJ lpObj,LPOBJ lpTarget);
	void NoticeSendToAll(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel,int type,char* message,...);
	void DataSendToAll(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel,BYTE* lpMsg,int size);
	void CGIllusionTempleRenewalSkillRecv(PMSG_ILLUSION_TEMPLE_RENEWAL_SKILL_RECV* lpMsg,int aIndex);
	void CGIllusionTempleRenewalButtonClickRecv(int aIndex);
	void CGIllusionTempleRenewalStorageRecv(PMSG_ILLUSION_TEMPLE_RENEWAL_STORAGE_RECV* lpMsg,int aIndex);
	void CGIllusionTempleRenewalEnterRecv(int aIndex);
	void GCIllusionTempleRenewalRewardScoreSend(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel);
	void GCIllusionTempleRenewalChangeScenarioSend(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel,int state,int aIndex);
	void GCIllusionTempleRenewalEventItemOwnerSend(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel);
	void GCIllusionTempleRenewalBattleInfoSend(ILLUSION_TEMPLE_RENEWAL_LEVEL* lpLevel);
	void GCIllusionTempleRenewalDelSkillSend(int aIndex,int skill);
	void GCIllusionTempleRenewalAddSkillSend(int aIndex,int result,int skill,int bIndex);
	void GCIllusionTempleRenewalResultSend(int aIndex,int bIndex, BYTE result, BYTE status);
	void ForceStart();
private:
	ILLUSION_TEMPLE_RENEWAL_LEVEL m_IllusionTempleRenewalLevel[MAX_IT_LEVEL];
	int m_WarningTime;
	int m_NotifyTime;
	int m_EventTime;
	int m_CloseTime;
	std::vector<ILLUSION_TEMPLE_RENEWAL_START_TIME> m_IllusionTempleRenewalStartTime;
};

extern CIllusionTempleRenewal gIllusionTempleRenewal;