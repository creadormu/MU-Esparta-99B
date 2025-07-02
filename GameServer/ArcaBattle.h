// ArcaBattle.h: interface for the CArcaBattle class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Protocol.h"
#include "User.h"

#define MAX_ARCA_BATTLE_AURA 3
#define MAX_ARCA_BATTLE_ZONES 5

enum eArcaBattleResult
{
	ARCA_BATTLE_RESULT_REGISTER_SUCCESS = 0,
	ARCA_BATTLE_RESULT_MASTER_REGISTER_ONLY_GUILD_MASTER = 1,
	ARCA_BATTLE_RESULT_MASTER_REGISTER_LOW_GUILD_MEMBERS = 2,
	ARCA_BATTLE_RESULT_MASTER_REGISTER_MAXIMUM_GUILDS = 3,
	ARCA_BATTLE_RESULT_MASTER_REGISTER_ALREADY_REGISTERED = 4,
	ARCA_BATTLE_RESULT_MASTER_REGISTER_PERIOD_ENDED = 5,
	ARCA_BATTLE_RESULT_MASTER_REGISTER_ONLY_TOP6 = 6,
	ARCA_BATTLE_RESULT_MEMBER_REGISTER_NOT_IN_GUILD = 7,
	ARCA_BATTLE_RESULT_MEMBER_REGISTER_ALREADY_REGISTERED = 8,
	ARCA_BATTLE_RESULT_MEMBER_REGISTER_MAXIMUM_MEMBERS = 9,
	ARCA_BATTLE_RESULT_MEMBER_REGISTER_PERIOD_ENDED = 10,
	ARCA_BATTLE_RESULT_MEMBER_REGISTER_ONLY_MEMBERS = 13,
	ARCA_BATTLE_RESULT_MEMBER_REGISTER_ONLY_DURING_PERIOD = 14,
	ARCA_BATTLE_RESULT_ENTER_NOT_REGISTERED = 11,
	ARCA_BATTLE_RESULT_ENTER_PERIOD_ENDED = 12,
	ARCA_BATTLE_RESULT_SIGN_REGISTER_NOT_ALLOWED = 15,
	ARCA_BATTLE_RESULT_SIGN_REGISTER_LOW_MEMBERS = 16,
	ARCA_BATTLE_RESULT_SIGN_REGISTER_HIGH_MEMBERS = 17,
};

enum eArcaBattleState
{
	AB_STATE_BLANK = 0,
	AB_STATE_EMPTY = 1,
	AB_STATE_SIGN1 = 2,
	AB_STATE_SIGN2 = 3,
	AB_STATE_READY = 4,
	AB_STATE_PARTY = 5,
	AB_STATE_START = 6,
	AB_STATE_CLOSE = 7,
	AB_STATE_CLEAN = 8,
	AB_STATE_CLEAR = 9,
};

//**********************************************//
//************ Client -> GameServer ************//
//**********************************************//

struct PMSG_ARCA_BATTLE_ENTER_RESULT_RECV
{
	PSBMSG_HEAD header; // C1:F8:34
	BYTE result;
};

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_ARCA_BATTLE_MASTER_REGISTER_RESULT_SEND
{
	PSBMSG_HEAD header; // C1:F8:31
	BYTE result;
};

struct PMSG_ARCA_BATTLE_MEMBER_REGISTER_RESULT_SEND
{
	PSBMSG_HEAD header; // C1:F8:33
	BYTE result;
};

struct PMSG_ARCA_BATTLE_ENTER_RESULT_SEND
{
	PSBMSG_HEAD header; // C1:F8:35
	BYTE result;
	BYTE data;
};

struct PMSG_ARCA_BATTLE_OBELISK_INFO_SEND
{
	PSBMSG_HEAD header; // C1:F8:37
	BYTE attribute;
	BYTE aura[MAX_ARCA_BATTLE_AURA];
};

struct PMSG_ARCA_BATTLE_STATE_SEND
{
	PSBMSG_HEAD header; // C1:F8:38
	BYTE state;
};

struct PMSG_ARCA_BATTLE_OBELISK_AURA
{
	DWORD x;
	DWORD y;
};

struct PMSG_ARCA_BATTLE_PLAYING_OBELISK
{
	DWORD group;
	DWORD attribute;
	DWORD world;
	DWORD x;
	DWORD y;
	PMSG_ARCA_BATTLE_OBELISK_AURA aura[MAX_ARCA_BATTLE_AURA];
};

struct PMSG_ARCA_BATTLE_PLAY_INFO_SEND
{
	PSBMSG_HEAD header; // C1:F8:39
	WORD GuildGroup;
	PMSG_ARCA_BATTLE_PLAYING_OBELISK obelisk[MAX_ARCA_BATTLE_ZONES];
};

struct PMSG_ARCA_BATTLE_OBELISK_STATE_SEND
{
	PSBMSG_HEAD header; // C1:F8:3A
	BYTE attribute;
	BYTE state;
	BYTE aura;
};

struct PMSG_ARCA_BATTLE_OBELISK_OCCUPY
{
	WORD obelisk;
	char GuildName[9];
};

struct PMSG_ARCA_BATTLE_RESULT_SEND
{
	PSBMSG_HEAD header; // C1:F8:3B
	BYTE result;
	DWORD ContributePoint;
	DWORD KillPoint;
	DWORD RewardExp;
	WORD BootyCnt;
	PMSG_ARCA_BATTLE_OBELISK_OCCUPY obelisk[MAX_ARCA_BATTLE_ZONES];
};

struct PMSG_ARCA_BATTLE_OCCUPY_ZONE_SEND
{
	PSBMSG_HEAD header; // C1:F8:3D
	char GuildName[9];
};

struct PMSG_ARCA_BATTLE_OBELISK_DATA
{
	BYTE life;
	BYTE attribute;
};

struct PMSG_ARCA_BATTLE_OBELISK_LIFE_SEND
{
	PSBMSG_HEAD header; // C1:F8:3F
	PMSG_ARCA_BATTLE_OBELISK_DATA obelisk[MAX_ARCA_BATTLE_ZONES];
};

struct PMSG_ARCA_BATTLE_MEMBER_COUNT_SEND
{
	PSBMSG_HEAD header; // C1:F8:42
	BYTE result;
	BYTE count;
};

struct PMSG_ARCA_BATTLE_REGISTER_MARK_SEND
{
	PSBMSG_HEAD header; // C1:F8:44
	BYTE sign[4];
};

struct PMSG_ARCA_BATTLE_MARK_RANK_SEND
{
	PSBMSG_HEAD header; // C1:F8:46
	BYTE rank;
	BYTE sign[4];
	BYTE count;
};

struct PMSG_ARCA_BATTLE_MARK_RANK
{
	BYTE rank;
	char GuildName[9];
	BYTE sign[4];
};

struct PMSG_ARCA_BATTLE_REGISTER_MARK_RESULT_SEND
{
	PSBMSG_HEAD header; // C1:F8:47
	BYTE result;
};

struct PMSG_ARCA_BATTLE_OBELISK_GUILD_STATE_SEND
{
	PSBMSG_HEAD header; // C1:F8:49
	DWORD zone;
	DWORD attribute;
	BYTE state;
	DWORD GuildIndex;
	char GuildName[9];
};

struct PMSG_ARCA_BATTLE_NOTIFY_TEXT_SEND
{
	PSBMSG_HEAD header; // C1:F8:4A
	DWORD index;
	DWORD value;
};

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_ARCA_BATTLE_MASTER_REGISTER_RESULT_RECV
{
	PSBMSG_HEAD header; // C1:1D:00
	WORD index;
	char account[11];
	BYTE result;
};

struct SDHP_ARCA_BATTLE_MEMBER_REGISTER_RESULT_RECV
{
	PSBMSG_HEAD header; // C1:1D:01
	WORD index;
	char account[11];
	BYTE result;
};

struct SDHP_ARCA_BATTLE_ENTER_RECV
{
	PSBMSG_HEAD header; // C1:1D:02
	WORD index;
	char account[11];
	BYTE result;
};

struct SDHP_ARCA_BATTLE_MEMBER_COUNT_RECV
{
	PSBMSG_HEAD header; // C1:1D:03
	WORD index;
	char account[11];
	BYTE result;
	BYTE count;
};

struct SDHP_ARCA_BATTLE_REGISTER_MARK_RECV
{
	PSBMSG_HEAD header; // C1:1D:04
	WORD index;
	char account[11];
	char GuildName[9];
	BYTE result;
	DWORD sign;
};

struct SDHP_ARCA_BATTLE_MARK_RANK
{
	BYTE rank;
	char GuildName[9];
	DWORD sign;
};

struct SDHP_ARCA_BATTLE_MARK_RANK_RECV
{
	PSWMSG_HEAD header; // C1:1D:05
	WORD index;
	char account[11];
	BYTE rank;
	DWORD sign;
	BYTE count;
};

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_ARCA_BATTLE_MASTER_REGISTER_SEND
{
	PSBMSG_HEAD header; // C1:1D:00
	WORD index;
	char account[11];
	char GuildName[9];
};

struct SDHP_ARCA_BATTLE_MEMBER_REGISTER_SEND
{
	PSBMSG_HEAD header; // C1:1D:01
	WORD index;
	char account[11];
	char name[11];
	char GuildName[9];
	int MaxGuildMember;
};

struct SDHP_ARCA_BATTLE_ENTER_SEND
{
	PSBMSG_HEAD header; // C1:1D:02
	WORD index;
	char account[11];
	char name[11];
	char GuildName[9];
};

struct SDHP_ARCA_BATTLE_MEMBER_COUNT_SEND
{
	PSBMSG_HEAD header; // C1:1D:03
	WORD index;
	char account[11];
	char GuildName[9];
};

struct SDHP_ARCA_BATTLE_REGISTER_MARK_SEND
{
	PSBMSG_HEAD header; // C1:1D:04
	WORD index;
	char account[11];
	char GuildName[9];
};

struct SDHP_ARCA_BATTLE_MARK_RANK_SEND
{
	PSBMSG_HEAD header; // C1:1D:05
	WORD index;
	char account[11];
	char GuildName[9];
};

struct SDHP_ARCA_BATTLE_MASTER_REGISTER_SAVE_SEND
{
	PSBMSG_HEAD header; // C1:1D:30
	char GuildName[9];
};

struct SDHP_ARCA_BATTLE_MEMBER_REGISTER_SAVE_SEND
{
	PSBMSG_HEAD header; // C1:1D:31
	char GuildName[9];
	char name[11];
};

struct SDHP_ARCA_BATTLE_REGISTER_MARK_SAVE_SEND
{
	PSBMSG_HEAD header; // C1:1D:32
	char GuildName[9];
	DWORD sign;
};

//**********************************************//
//**********************************************//
//**********************************************//

struct ARCA_BATTLE_START_TIME
{
	int State;
	int Year;
	int Month;
	int Day;
	int DayOfWeek;
	int Hour;
	int Minute;
	int Second;
};

class CArcaBattle
{
public:
	CArcaBattle();
	virtual ~CArcaBattle();
	void Init();
	void Load(char* path);
	void MainProc();
	void ProcState_BLANK();
	void ProcState_EMPTY();
	void ProcState_SIGN1();
	void ProcState_SIGN2();
	void ProcState_READY();
	void ProcState_PARTY();
	void ProcState_START();
	void ProcState_CLOSE();
	void ProcState_CLEAN();
	void ProcState_CLEAR();
	void SetState(int state);
	void SetState_BLANK();
	void SetState_EMPTY();
	void SetState_SIGN1();
	void SetState_SIGN2();
	void SetState_READY();
	void SetState_PARTY();
	void SetState_START();
	void SetState_CLOSE();
	void SetState_CLEAN();
	void SetState_CLEAR();
	int GetState();
	void CheckSync();
	void UserDieProc(LPOBJ lpObj,LPOBJ lpTarget);
	int MonsterItemDrop(LPOBJ lpMonster);
	void CGArcaBattleGuildMasterRegisterRecv(int aIndex);
	void CGArcaBattleGuildMemberRegisterRecv(int aIndex);
	void CGArcaBattleEnterRecv(PMSG_ARCA_BATTLE_ENTER_RESULT_RECV* lpMsg,int aIndex);
	void CGArcaBattleTrophiesRecv(int aIndex);
	void CGArcaBattleMemberCountRecv(int aIndex);
	void CGArcaBattleRegisterMarkRecv(int aIndex);
	void CGArcaBattleMarkRankRecv(int aIndex);
	void GCArcaBattleMasterRegisterResultSend(int aIndex,int result);
	void GCArcaBattleMemberRegisterResultSend(int aIndex,int result);
	void GCArcaBattleEnterResultSend(int aIndex,int result,int data);
	void GCArcaBattleMemberCountResultSend(int aIndex,int result,int count);
	void GCArcaBattleRegisterMarkResultSend(int aIndex,int result);
	void GDArcaBattleMasterRegisterSaveSend(int aIndex);
	void GDArcaBattleMemberRegisterSaveSend(int aIndex);
	void GDArcaBattleRegisterMarkSend(int aIndex,int sign);
	void DGArcaBattleMasterRegisterRecv(SDHP_ARCA_BATTLE_MASTER_REGISTER_RESULT_RECV* lpMsg);
	void DGArcaBattleMemberRegisterRecv(SDHP_ARCA_BATTLE_MEMBER_REGISTER_RESULT_RECV* lpMsg);
	void DGArcaBattleEnterRecv(SDHP_ARCA_BATTLE_ENTER_RECV* lpMsg);
	void DGArcaBattleMemberCountRecv(SDHP_ARCA_BATTLE_MEMBER_COUNT_RECV* lpMsg);
	void DGArcaBattleRegisterMarkRecv(SDHP_ARCA_BATTLE_REGISTER_MARK_RECV* lpMsg);
	void DGArcaBattleMarkRankRecv(SDHP_ARCA_BATTLE_MARK_RANK_RECV* lpMsg);
private:
	int m_State;
	int m_RemainTime;
	int m_TargetTime;
	int m_TickCount;
	std::vector<ARCA_BATTLE_START_TIME> m_ArcaBattleStartTime;
};

extern CArcaBattle gArcaBattle;