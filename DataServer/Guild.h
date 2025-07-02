// Guild.h: interface for the CGuild class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "DataServerProtocol.h"

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_GUILD_CREATE_RECV
{
	PSBMSG_HEAD header; // C1:A0:00
	char Name[9];
	char Master[11];
	BYTE Mark[32];
	BYTE type;
	int Server;
};

struct SDHP_GUILD_DELETE_RECV
{
	PSBMSG_HEAD header; // C1:A0:01
	char Name[9];
	char Member[11];
};

struct SDHP_GUILD_MEMBER_ADD_RECV
{
	PSBMSG_HEAD header; // C1:A0:02
	char Name[9];
	char Member[11];
	int Server;
};

struct SDHP_GUILD_MEMBER_DEL_RECV
{
	PSBMSG_HEAD header; // C1:A0:03
	char Name[9];
	char Member[11];
};

struct SDHP_GUILD_ASSIGN_STATUS_RECV
{
	PSBMSG_HEAD header; // C1:A0:04
	char Name[9];
	char Member[11];
	WORD Status;
	BYTE Type;
};

struct SDHP_GUILD_ASSIGN_TYPE_RECV
{
	PSBMSG_HEAD header; // C1:A0:05
	char Name[9];
	BYTE Type;
};

struct SDHP_GUILD_NOTICE_RECV
{
	PSBMSG_HEAD header; // C1:A0:06
	char Name[9];
	char Notice[60];
};

struct SDHP_GUILD_POST_RECV
{
	PSBMSG_HEAD header; // C1:A0:07
	char Name[9];
	char Member[11];
	BYTE Type;
	char Message[60];
};

struct SDHP_GUILD_SCORE_RECV
{
	PSBMSG_HEAD header; // C1:A0:08
	char Name[9];
	DWORD Score;
};

struct SDHP_GUILD_UNION_GET_RECV
{
	PSBMSG_HEAD header; // C1:A0:09
	char Member[11];
	DWORD Index;
};

struct SDHP_GUILD_UNION_JOIN_RECV
{
	PSBMSG_HEAD header; // C1:A0:0A
	WORD index[2];
	BYTE relation;
	DWORD GuildNumber[2];
};

struct SDHP_GUILD_UNION_BREAK_RECV
{
	PSBMSG_HEAD header; // C1:A0:0B
	WORD index[2];
	BYTE relation;
	DWORD GuildNumber[2];
};

struct SDHP_GUILD_ALLIANCE_KICK_RECV
{
	PSBMSG_HEAD header; // C1:A0:0C
	WORD index;
	BYTE relation;
	char GuildNameA[9];
	char GuildNameB[9];
};

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_GUILD_CREATE_SEND
{
	PSBMSG_HEAD header; // C1:A0:00
	DWORD Index;
	char Name[9];
	char Master[11];
	BYTE Mark[32];
	BYTE type;
	BYTE result;
};

struct SDHP_GUILD_DELETE_SEND
{
	PSBMSG_HEAD header; // C1:A0:01
	char Name[9];
	char Member[11];
	BYTE result;
};

struct SDHP_GUILD_MEMBER_ADD_SEND
{
	PSBMSG_HEAD header; // C1:A0:02
	char Name[9];
	char Member[11];
	int Server;
	BYTE result;
};

struct SDHP_GUILD_MEMBER_DEL_SEND
{
	PSBMSG_HEAD header; // C1:A0:03
	char Name[9];
	char Member[11];
	BYTE result;
};

struct SDHP_GUILD_MEMBER_UPDATE_SEND
{
	PSBMSG_HEAD header; // C1:A0:04
	char Name[9];
	char Member[11];
	WORD Status;
	int Server;
};

struct SDHP_GUILD_ASSIGN_STATUS_SEND
{
	PSBMSG_HEAD header; // C1:A0:05
	char Name[9];
	char Member[11];
	WORD Status;
	BYTE Type;
	BYTE result;
};

struct SDHP_GUILD_ASSIGN_TYPE_SEND
{
	PSBMSG_HEAD header; // C1:A0:06
	char Name[9];
	BYTE Type;
	BYTE result;
};

struct SDHP_GUILD_NOTICE_SEND
{
	PSBMSG_HEAD header; // C1:A0:07
	char Name[9];
	char Notice[60];
};

struct SDHP_GUILD_POST_SEND
{
	PSBMSG_HEAD header; // C1:A0:08
	char Name[9];
	char Member[11];
	BYTE Type;
	char Message[60];
};

struct SDHP_GUILD_SCORE_SEND
{
	PSBMSG_HEAD header; // C1:A0:09
	char Name[9];
	DWORD Score;
};

struct SDHP_GUILD_INFO_SEND
{
	PSWMSG_HEAD header; // C2:A0:0A
	DWORD Index;
	char Name[9];
	char Master[11];
	BYTE Mark[32];
	DWORD Score;
	char Notice[60];
	BYTE type;
	DWORD Union;
	DWORD Rival;
	BYTE count;
};

struct SDHP_GUILD_INFO
{
	char Name[11];
	WORD Status;
	int Server;
};

struct SDHP_UNION_INFO_SEND
{
	PSWMSG_HEAD header; // C2:A0:0B
	DWORD Index;
	char Name[9];
	BYTE relation;
	BYTE count;
	int list[100];
};

struct SDHP_UNION_UPDATE_SEND
{
	PSWMSG_HEAD header; // C2:A0:0C
	BYTE code;
	BYTE count;
	int list[100];
};

struct SDHP_GUILD_UNION_INFO_SEND
{
	PSWMSG_HEAD header; // C2:A0:0D
	char Member[11];
	DWORD Index;
	BYTE Union;
	BYTE Rival;
	BYTE result;
	BYTE count;
};

struct SDHP_GUILD_UNION_INFO
{
	char Name[9];
	BYTE Mark[32];
	BYTE count;
};

struct SDHP_GUILD_UNION_JOIN_SEND
{
	PSBMSG_HEAD header; // C1:A0:0E
	BYTE result;
	BYTE flag;
	WORD index[2];
	BYTE relation;
	DWORD GuildNumber[2];
};

struct SDHP_GUILD_UNION_BREAK_SEND
{
	PSBMSG_HEAD header; // C1:A0:0F
	BYTE result;
	BYTE flag;
	WORD index[2];
	BYTE relation;
	DWORD GuildNumber[2];
};

struct SDHP_GUILD_ALLIANCE_KICK_SEND
{
	PSBMSG_HEAD header; // C1:A0:10
	BYTE flag;
	WORD index;
	BYTE result;
	BYTE relation;
	char GuildNameA[9];
	char GuildNameB[9];
};

//**********************************************//
//**********************************************//
//**********************************************//

class CGuild
{
public:
	CGuild();
	virtual ~CGuild();
	void GDGuildCreateRecv(SDHP_GUILD_CREATE_RECV* lpMsg,int aIndex);
	void GDGuildDeleteRecv(SDHP_GUILD_DELETE_RECV* lpMsg,int aIndex);
	void GDGuildMemberAddRecv(SDHP_GUILD_MEMBER_ADD_RECV* lpMsg,int aIndex);
	void GDGuildMemberDelRecv(SDHP_GUILD_MEMBER_DEL_RECV* lpMsg,int aIndex);
	void GDGuildAssignStatusRecv(SDHP_GUILD_ASSIGN_STATUS_RECV* lpMsg);
	void GDGuildAssignTypeRecv(SDHP_GUILD_ASSIGN_TYPE_RECV* lpMsg);
	void GDGuildNoticeRecv(SDHP_GUILD_NOTICE_RECV* lpMsg);
	void GDGuildPostRecv(SDHP_GUILD_POST_RECV* lpMsg);
	void GDGuildScoreRecv(SDHP_GUILD_SCORE_RECV* lpMsg);
	void GDGuildUnionGetRecv(SDHP_GUILD_UNION_GET_RECV* lpMsg, int aIndex);
	void GDGuildUnionJoinRecv(SDHP_GUILD_UNION_JOIN_RECV* lpMsg,int aIndex);
	void GDGuildUnionBreakRecv(SDHP_GUILD_UNION_BREAK_RECV* lpMsg,int aIndex);
	void GDGuildAllianceKickRecv(SDHP_GUILD_ALLIANCE_KICK_RECV* lpMsg,int aIndex);
	void GuildInfoSend(int index);
	void UnionInfoSend(int index,int relation);
	void UnionUpdateSend(int code,int count,int* list);
	void MemberConnect(char* member,int server);
	void MemberDisconnect(char* member);
	void MemberUpdate(char* name,char* member,int status,int server);
	void DataSendToAll(BYTE* lpMsg,int size);
};

extern CGuild gGuild;