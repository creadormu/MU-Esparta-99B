// ChaosCastleFinal.h: interface for the CChaosCastleFinal class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Protocol.h"
#include "User.h"

#define MAX_CCF_USER 70
#define MAX_CCF_MONSTER 100

enum eChaosCastleFinalState
{
	CCF_STATE_BLANK = 0,
	CCF_STATE_EMPTY = 1,
	CCF_STATE_STAND = 2,
	CCF_STATE_START = 3,
	CCF_STATE_CLEAN = 4,
};

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_CHAOS_CASTLE_RANKING_RECV
{
	PSWMSG_HEAD header; // C1:1B:00
	WORD index;
	BYTE count;
};

struct SDHP_CHAOS_CASTLE_RANKING
{
	BYTE Rank;
	DWORD Point;
	char name[11];
};

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_CHAOS_CASTLE_RANKING_SEND
{
	PSBMSG_HEAD header; // C1:1B:00
	WORD index;
};

struct SDHP_CHAOS_CASTLE_RANKING_SAVE_SEND
{
	PSBMSG_HEAD header; // C1:1B:01
	char name[11];
	DWORD Point;
};

struct SDHP_CHAOS_CASTLE_RANKING_CLEAR_SEND
{
	PSBMSG_HEAD header; // C1:1B:02
};

//**********************************************//
//************ Client -> GameServer ************//
//**********************************************//

struct PMSG_CHAOS_CASTLE_FINAL_POSITION_RECV
{
	PSBMSG_HEAD header; // C1:AF:06
	BYTE x;
	BYTE y;
};

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_CHAOS_CASTLE_FINAL_STATE_SEND
{
	PBMSG_HEAD header; // C1:9B
	BYTE state;
	WORD time;
	WORD MaxMonster;
	WORD CurMonster;
	WORD EventItemOwner;
	BYTE EventItemLevel;
};

struct PMSG_CHAOS_CASTLE_FINAL_TIME_SEND
{
	PSWMSG_HEAD header; //  C1:AF:03
	DWORD count;
	BYTE Tryout1;
	BYTE Tryout2;
	BYTE Tryout3;
	BYTE SemiFinal1;
	BYTE SemiFinal2;
	BYTE Final;
	BYTE Type;
};

struct PMSG_CHAOS_CASTLE_FINAL_TIME
{
	DWORD Hour;
	DWORD Minute;
	DWORD Stage1;
	DWORD Stage2;
	DWORD Stage3;
};

struct PMSG_CHAOS_CASTLE_FINAL_ENTER_SEND
{
	PSBMSG_HEAD header; // C1:AF:05
	BYTE result;
	BYTE RemainTime[2];
};

struct PMSG_CHAOS_CASTLE_FINAL_RANKING_SEND
{
	#pragma pack(1)
	PSWMSG_HEAD header; // C1:AF:07
	BYTE Result;
	BYTE count;
	#pragma pack()
};

struct PMSG_CHAOS_CASTLE_FINAL_SCORE_SEND
{
	PSBMSG_HEAD header; // C1:AF:09
	DWORD day;
	DWORD type;
	DWORD Score;
	DWORD MonsterKillCount;
	DWORD UserKillCount;
};

//**********************************************//
//**********************************************//
//**********************************************//

struct CHAOS_CASTLE_FINAL_START_TIME
{
	int Hour;
	int Minute;
	int Second;
	int Stage1;
	int Stage2;
	int Stage3;
};

struct CHAOS_CASTLE_FINAL_DELAY_SCORE
{
	int Delay;
	int Index;
	PMSG_CHAOS_CASTLE_FINAL_SCORE_SEND Score;
};

struct CHAOS_CASTLE_FINAL_USER
{
	int Index;
	int Score;
	int UserKillCount;
	int MonsterKillCount;
	int RewardExperience;
};

class CChaosCastleFinal
{
public:
	CChaosCastleFinal();
	virtual ~CChaosCastleFinal();
	void Init();
	void Load(char* path);
	void MainProc();
	void ProcState_BLANK();
	void ProcState_EMPTY();
	void ProcState_STAND();
	void ProcState_START();
	void ProcState_CLEAN();
	void SetState(int state);
	void SetState_BLANK();
	void SetState_EMPTY();
	void SetState_STAND();
	void SetState_START();
	void SetState_CLEAN();
	void SetStage0();
	void SetStage1();
	void SetStage2();
	void SetStage3();
	void CheckSync();
	int GetState();
	int GetRemainTime();
	bool AddUser(int aIndex);
	bool DelUser(int aIndex);
	CHAOS_CASTLE_FINAL_USER* GetUser(int aIndex);
	void CleanUser();
	void ClearUser();
	void CheckUser();
	int GetUserCount();
	int GetUserWinner();
	bool AddMonster(int aIndex);
	bool DelMonster(int aIndex);
	int* GetMonster(int aIndex);
	void CleanMonster();
	void ClearMonster();
	int GetMonsterCount();
	void StageSpawn(int Stage);
	void SetMonster(int MonsterClass,int MaxRegenTime);
	void SetSafeZone();
	void DelSafeZone();
	void SetHollowZone(int stage);
	void DelHollowZone(int stage);
	void SetCenterHollowZone();
	void DelCenterHollowZone();
	void CheckUserPosition();
	void CheckMonsterPosition();
	void CheckItemPosition();
	void SearchUserAndBlow(int x,int y);
	void BlowUserFromPoint(int aIndex,int x,int y);
	bool GetFreePosition(int* ox,int* oy);
	void UserDieProc(LPOBJ lpObj,LPOBJ lpTarget);
	void MonsterDieProc(LPOBJ lpObj,LPOBJ lpTarget);
	void CheckDelayScore();
	void SendMapServerGroupMsg(char* lpMsg,...);
	void NoticeSendToAll(int type,char* message,...);
	void DataSendToAll(BYTE* lpMsg,int size);
	void CGChaosCastleFinalRankingSend(int aIndex);
	void CGChaosCastleFinalClose(int aIndex);
	void CGChaosCastleFinalEnterRecv(int aIndex);
	void CGChaosCastleFinalPositionRecv(PMSG_CHAOS_CASTLE_FINAL_POSITION_RECV* lpMsg,int aIndex);
	void GCChaosCastleFinalRankingRecv(int aIndex);
	void GCChaosCastleFinalScoreSend(int aIndex,int type);
	void GCChaosCastleFinalDelayScoreSend(int aIndex,int type);
	void GCChaosCastleFinalStateSend(int state);
	void GCChaosCastleFinalSimpleStateSend(int state,int aIndex);
	void GCChaosCastleFinalRankingSend(SDHP_CHAOS_CASTLE_RANKING_RECV* lpMsg);
	void GDChaosCastleFinalScoreSaveSend(int aIndex);
	void GDChaosCastleFinalScoreClearSend();
	int GetDay();
private:
	int m_State;
	int m_RemainTime;
	int m_TargetTime;
	int m_TickCount;
	int m_EnterEnabled;
	int m_MinutesLeft;
	int m_TimeCount;
	int m_Stage;
	int m_MaxMonster;
	int m_WinnerIndex;
	int m_WarningTime;
	int m_NotifyTime;
	int m_EventTime;
	int m_CloseTime;
	int m_Tryout1;
	int m_Tryout2;
	int m_Tryout3;
	int m_SemiFinal1;
	int m_SemiFinal2;
	int m_Final;
	CHAOS_CASTLE_FINAL_USER User[MAX_CCF_USER];
	int MonsterIndex[MAX_CCF_MONSTER];
	std::vector<CHAOS_CASTLE_FINAL_DELAY_SCORE> DelayScore;
	std::vector<CHAOS_CASTLE_FINAL_START_TIME> m_ChaosCastleFinalStartTime;
};

extern CChaosCastleFinal gChaosCastleFinal;

static const int gChaosCastleFinalEnterMoney = 50000000;