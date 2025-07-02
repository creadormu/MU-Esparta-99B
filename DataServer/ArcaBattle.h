// ArcaBattle.h: interface for the CArcaBattle class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "DataServerProtocol.h"

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_ARCA_BATTLE_MASTER_REGISTER_RECV
{
	PSBMSG_HEAD header; // C1:1D:00
	WORD index;
	char account[11];
	char GuildName[9];
};

struct SDHP_ARCA_BATTLE_MEMBER_REGISTER_RECV
{
	PSBMSG_HEAD header; // C1:1D:01
	WORD index;
	char account[11];
	char name[11];
	char GuildName[9];
	int MaxGuildMember;
};

struct SDHP_ARCA_BATTLE_ENTER_RECV
{
	PSBMSG_HEAD header; // C1:1D:02
	WORD index;
	char account[11];
	char name[11];
	char GuildName[9];
};

struct SDHP_ARCA_BATTLE_MEMBER_COUNT_RECV
{
	PSBMSG_HEAD header; // C1:1D:03
	WORD index;
	char account[11];
	char GuildName[9];
};

struct SDHP_ARCA_BATTLE_REGISTER_MARK_RECV
{
	PSBMSG_HEAD header; // C1:1D:04
	WORD index;
	char account[11];
	char GuildName[9];
};

struct SDHP_ARCA_BATTLE_MARK_RANK_RECV
{
	PSBMSG_HEAD header; // C1:1D:05
	WORD index;
	char account[11];
	char GuildName[9];
};

struct SDHP_ARCA_BATTLE_MASTER_REGISTER_SAVE_RECV
{
	PSBMSG_HEAD header; // C1:1D:30
	char GuildName[9];
};

struct SDHP_ARCA_BATTLE_MEMBER_REGISTER_SAVE_RECV
{
	PSBMSG_HEAD header; // C1:1D:31
	char GuildName[9];
	char name[11];
};

struct SDHP_ARCA_BATTLE_REGISTER_MARK_SAVE_RECV
{
	PSBMSG_HEAD header; // C1:1D:32
	char GuildName[9];
	DWORD sign;
};

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_ARCA_BATTLE_MASTER_REGISTER_RESULT_SEND
{
	PSBMSG_HEAD header; // C1:1D:00
	WORD index;
	char account[11];
	BYTE result;
};

struct SDHP_ARCA_BATTLE_MEMBER_REGISTER_RESULT_SEND
{
	PSBMSG_HEAD header; // C1:1D:01
	WORD index;
	char account[11];
	BYTE result;
};

struct SDHP_ARCA_BATTLE_ENTER_SEND
{
	PSBMSG_HEAD header; // C1:1D:02
	WORD index;
	char account[11];
	BYTE result;
};

struct SDHP_ARCA_BATTLE_MEMBER_COUNT_SEND
{
	PSBMSG_HEAD header; // C1:1D:03
	WORD index;
	char account[11];
	BYTE result;
	BYTE count;
};

struct SDHP_ARCA_BATTLE_REGISTER_MARK_SEND
{
	PSBMSG_HEAD header; // C1:1D:04
	WORD index;
	char account[11];
	char GuildName[9];
	BYTE result;
	DWORD sign;
};

struct SDHP_ARCA_BATTLE_MARK_RANK_SEND
{
	PSWMSG_HEAD header; // C1:1D:05
	WORD index;
	char account[11];
	BYTE rank;
	DWORD sign;
	BYTE count;
};

struct SDHP_ARCA_BATTLE_MARK_RANK
{
	BYTE rank;
	char GuildName[9];
	DWORD sign;
};

//**********************************************//
//**********************************************//
//**********************************************//

class CArcaBattle
{
public:
	CArcaBattle();
	virtual ~CArcaBattle();
	void GDArcaBattleMasterRegisterRecv(SDHP_ARCA_BATTLE_MASTER_REGISTER_RECV* lpMsg,int index);
	void GDArcaBattleMemberRegisterRecv(SDHP_ARCA_BATTLE_MEMBER_REGISTER_RECV* lpMsg,int index);
	void GDArcaBattleEnterRecv(SDHP_ARCA_BATTLE_ENTER_RECV* lpMsg,int index);
	void GDArcaBattleMemberCountRecv(SDHP_ARCA_BATTLE_MEMBER_COUNT_RECV* lpMsg,int index);
	void GDArcaBattleRegisterMarkRecv(SDHP_ARCA_BATTLE_REGISTER_MARK_RECV* lpMsg,int index);
	void GDArcaBattleMarkRankRecv(SDHP_ARCA_BATTLE_MARK_RANK_RECV* lpMsg,int index);
	void GDArcaBattleMasterRegisterSaveRecv(SDHP_ARCA_BATTLE_MASTER_REGISTER_SAVE_RECV* lpMsg);
	void GDArcaBattleMemberRegisterSaveRecv(SDHP_ARCA_BATTLE_MEMBER_REGISTER_SAVE_RECV* lpMsg);
	void GDArcaBattleRegisterMarkSaveRecv(SDHP_ARCA_BATTLE_REGISTER_MARK_SAVE_RECV* lpMsg);
private:
};

extern CArcaBattle gArcaBattle;