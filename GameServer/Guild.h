// Guild.h: interface for the CGuild class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Protocol.h"

enum eGuildRelation
{
	GUILD_RELATION_NONE = 0,
	GUILD_RELATION_UNION = 1,
	GUILD_RELATION_RIVAL = 2,
};

enum eGuildWarType
{
	GUILD_WAR_TYPE_NORMAL = 0,
	GUILD_WAR_TYPE_SOCCER = 1,
};

enum eGuildWarState
{
	GUILD_WAR_STATE_NONE = 0,
	GUILD_WAR_STATE_DECLARE = 1,
	GUILD_WAR_STATE_ACTIVE = 2,
};

enum eGuildWarTeam
{
	GUILD_WAR_TEAM_BLUE = 0,
	GUILD_WAR_TEAM_RED = 1,
};

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_GUILD_CREATE_RECV
{
	PSBMSG_HEAD header; // C1:A0:00
	DWORD Index;
	char Name[9];
	char Master[11];
	BYTE Mark[32];
	BYTE type;
	BYTE result;
};

struct SDHP_GUILD_DELETE_RECV
{
	PSBMSG_HEAD header; // C1:A0:01
	char Name[9];
	char Member[11];
	BYTE result;
};

struct SDHP_GUILD_MEMBER_ADD_RECV
{
	PSBMSG_HEAD header; // C1:A0:02
	char Name[9];
	char Member[11];
	int Server;
	BYTE result;
};

struct SDHP_GUILD_MEMBER_DEL_RECV
{
	PSBMSG_HEAD header; // C1:A0:03
	char Name[9];
	char Member[11];
	BYTE result;
};

struct SDHP_GUILD_MEMBER_UPDATE_RECV
{
	PSBMSG_HEAD header; // C1:A0:04
	char Name[9];
	char Member[11];
	WORD Status;
	int Server;
};

struct SDHP_GUILD_ASSIGN_STATUS_RECV
{
	PSBMSG_HEAD header; // C1:A0:05
	char Name[9];
	char Member[11];
	WORD Status;
	BYTE Type;
	BYTE result;
};

struct SDHP_GUILD_ASSIGN_TYPE_RECV
{
	PSBMSG_HEAD header; // C1:A0:06
	char Name[9];
	BYTE Type;
	BYTE result;
};

struct SDHP_GUILD_NOTICE_RECV
{
	PSBMSG_HEAD header; // C1:A0:07
	char Name[9];
	char Notice[60];
};

struct SDHP_GUILD_POST_RECV
{
	PSBMSG_HEAD header; // C1:A0:08
	char Name[9];
	char Member[11];
	BYTE Type;
	char Message[60];
};

struct SDHP_GUILD_SCORE_RECV
{
	PSBMSG_HEAD header; // C1:A0:09
	char Name[9];
	DWORD Score;
};

struct SDHP_GUILD_INFO_RECV
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

struct SDHP_UNION_INFO_RECV
{
	PSWMSG_HEAD header; // C2:A0:0B
	DWORD Index;
	char Name[9];
	BYTE relation;
	BYTE count;
	int list[100];
};

struct SDHP_UNION_UPDATE_RECV
{
	PSWMSG_HEAD header; // C2:A0:0C
	BYTE code;
	BYTE count;
	int list[100];
};

struct SDHP_GUILD_UNION_INFO_RECV
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

struct SDHP_GUILD_UNION_JOIN_RECV
{
	PSBMSG_HEAD header; // C1:A0:0E
	BYTE result;
	BYTE flag;
	WORD index[2];
	BYTE relation;
	DWORD GuildNumber[2];
};

struct SDHP_GUILD_UNION_BREAK_RECV
{
	PSBMSG_HEAD header; // C1:A0:0F
	BYTE result;
	BYTE flag;
	WORD index[2];
	BYTE relation;
	DWORD GuildNumber[2];
};

struct SDHP_GUILD_ALLIANCE_KICK_RECV
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
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_GUILD_CREATE_SEND
{
	PSBMSG_HEAD header; // C1:A0:00
	char Name[9];
	char Master[11];
	BYTE Mark[32];
	BYTE type;
	int Server;
};

struct SDHP_GUILD_DELETE_SEND
{
	PSBMSG_HEAD header; // C1:A0:01
	char Name[9];
	char Member[11];
};

struct SDHP_GUILD_MEMBER_ADD_SEND
{
	PSBMSG_HEAD header; // C1:A0:02
	char Name[9];
	char Member[11];
	int Server;
};

struct SDHP_GUILD_MEMBER_DEL_SEND
{
	PSBMSG_HEAD header; // C1:A0:03
	char Name[9];
	char Member[11];
};

struct SDHP_GUILD_ASSIGN_STATUS_SEND
{
	PSBMSG_HEAD header; // C1:A0:04
	char Name[9];
	char Member[11];
	WORD Status;
	BYTE Type;
};

struct SDHP_GUILD_ASSIGN_TYPE_SEND
{
	PSBMSG_HEAD header; // C1:A0:05
	char Name[9];
	BYTE Type;
};

struct SDHP_GUILD_NOTICE_SEND
{
	PSBMSG_HEAD header; // C1:A0:06
	char Name[9];
	char Notice[60];
};

struct SDHP_GUILD_POST_SEND
{
	PSBMSG_HEAD header; // C1:A0:07
	char Name[9];
	char Member[11];
	BYTE Type;
	char Message[60];
};

struct SDHP_GUILD_SCORE_SEND
{
	PSBMSG_HEAD header; // C1:A0:08
	char Name[9];
	DWORD Score;
};

struct SDHP_GUILD_UNION_GET_SEND
{
	PSBMSG_HEAD header; // C1:A0:09
	char Member[11];
	DWORD Index;
};

struct SDHP_GUILD_UNION_JOIN_SEND
{
	PSBMSG_HEAD header; // C1:A0:0A
	WORD index[2];
	BYTE relation;
	DWORD GuildNumber[2];
};

struct SDHP_GUILD_UNION_BREAK_SEND
{
	PSBMSG_HEAD header; // C1:A0:0B
	WORD index[2];
	BYTE relation;
	DWORD GuildNumber[2];
};

struct SDHP_GUILD_ALLIANCE_KICK_SEND
{
	PSBMSG_HEAD header; // C1:A0:0C
	WORD index;
	BYTE relation;
	char GuildNameA[9];
	char GuildNameB[9];
};

//**********************************************//
//************ Client -> GameServer ************//
//**********************************************//

struct PMSG_GUILD_REQUEST_RECV
{
	PBMSG_HEAD header; // C1:50
	BYTE index[2];
};

struct PMSG_GUILD_RESULT_RECV
{
	PBMSG_HEAD header; // C1:51
	BYTE result;
	BYTE index[2];
};

struct PMSG_GUILD_DELETE_RECV
{
	PBMSG_HEAD header; // C1:53
	char name[10];
	char PersonalCode[10];
};

struct PMSG_GUILD_MASTER_OPEN_RECV
{
	PBMSG_HEAD header; // C1:54
	BYTE result;
};

struct PMSG_GUILD_CREATE_RECV
{
	PBMSG_HEAD header; // C1:55
	BYTE type;
	char GuildName[8];
	BYTE Mark[32];
};

struct PMSG_GUILD_WAR_REQUEST_RESULT_RECV
{
	PBMSG_HEAD header; // C1:61
	BYTE result;
};

struct PMSG_GUILD_VIEWPORT_REQUEST_RECV
{
	PBMSG_HEAD header; // C1:66
	DWORD GuildNumber;
};

struct PMSG_GUILD_ASSIGN_STATUS_RECV
{
	PBMSG_HEAD header; // C1:E1
	BYTE type;
	BYTE status;
	char name[10];
};

struct PMSG_GUILD_ASSIGN_TYPE_RECV
{
	PBMSG_HEAD header; // C1:E2
	BYTE type;
};

struct PMSG_GUILD_UNION_REQUEST_RECV
{
	PBMSG_HEAD header; // C1:E5
	BYTE relation;
	BYTE type;
	BYTE index[2];
};

struct PMSG_GUILD_UNION_RESULT_RECV
{
	PBMSG_HEAD header; // C1:E6
	BYTE relation;
	BYTE type;
	BYTE result;
	BYTE index[2];
};

struct PMSG_GUILD_ALLIANCE_KICK_RECV
{
	PSBMSG_HEAD header; // C1:EB:01
	char GuildName[8];
};

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_GUILD_REQUEST_SEND
{
	PBMSG_HEAD header; // C1:50
	BYTE index[2];
};

struct PMSG_GUILD_RESULT_SEND
{
	PBMSG_HEAD header; // C1:51
	BYTE result;
};

struct PMSG_GUILD_LIST_SEND
{
	PWMSG_HEAD header; // C2:52
	BYTE result;
	BYTE count;
	DWORD TotalScore;
	BYTE score;
	char RivalGuild[9];
	#if(GAMESERVER_UPDATE>=701)
	BYTE RivalCount;
	BYTE RivalMark[32];
	#endif
};

struct PMSG_GUILD_LIST
{
	char name[10];
	BYTE number;
	BYTE connected;
	BYTE status;
};

struct PMSG_GUILD_DELETE_SEND
{
	PBMSG_HEAD header; // C1:53
	BYTE result;
};

struct PMSG_GUILD_CREATE_RESULT_SEND
{
	PBMSG_HEAD header; // C1:56
	BYTE result;
	BYTE type;
};

struct PMSG_GUILD_WAR_REQUEST_RESULT_SEND
{
	PBMSG_HEAD header; // C1:60
	BYTE result;
};

struct PMSG_GUILD_WAR_REQUEST_SEND
{
	PBMSG_HEAD header; // C1:61
	char GuildName[8];
	BYTE type;
};

struct PMSG_GUILD_WAR_START_SEND
{
	PBMSG_HEAD header; // C1:62
	char GuildName[8];
	BYTE type;
	BYTE team;
};

struct PMSG_GUILD_WAR_END_SEND
{
	PBMSG_HEAD header; // C1:63
	BYTE result;
	char GuildName[8];
};

struct PMSG_GUILD_WAR_SCORE_SEND
{
	PBMSG_HEAD header; // C1:64
	BYTE score[2];
	BYTE type;
};

struct PMSG_GUILD_VIEWPORT_SEND
{
	PBMSG_HEAD header; // C1:66
	DWORD GuildNumber;
	BYTE type;
	char UnionName[8];
	char GuildName[8];
	BYTE Mark[32];
};

struct PMSG_GUILD_ASSIGN_STATUS_SEND
{
	PBMSG_HEAD header; // C1:E1
	BYTE type;
	BYTE result;
	char name[10];
};

struct PMSG_GUILD_ASSIGN_TYPE_SEND
{
	PBMSG_HEAD header; // C1:E2
	BYTE type;
	BYTE result;
};

struct PMSG_GUILD_UNION_REQUEST_SEND
{
	PBMSG_HEAD header; // C1:E5
	BYTE relation;
	BYTE type;
	BYTE index[2];
};

struct PMSG_GUILD_UNION_RESULT_SEND
{
	PBMSG_HEAD header; // C1:E6
	BYTE relation;
	BYTE type;
	BYTE result;
	BYTE index[2];
};

struct PMSG_GUILD_UNION_LIST_SEND
{
	PWMSG_HEAD header; // C2:E9
	BYTE count;
	BYTE result;
	BYTE UnionNumber;
	BYTE RivalNumber;
};

struct PMSG_GUILD_UNION_LIST
{
	BYTE count;
	BYTE Mark[32];
	char GuildName[8];
};

struct PMSG_GUILD_ALLIANCE_KICK_RESULT_SEND
{
	PSBMSG_HEAD header; // C1:EB:01
	BYTE result;
	BYTE type;
	BYTE relation;
};

//**********************************************//
//**********************************************//
//**********************************************//

class CGuild
{
public:
	CGuild();
	virtual ~CGuild();
	void DGGuildCreateRecv(SDHP_GUILD_CREATE_RECV* lpMsg);
	void DGGuildDeleteRecv(SDHP_GUILD_DELETE_RECV* lpMsg);
	void DGGuildMemberAddRecv(SDHP_GUILD_MEMBER_ADD_RECV* lpMsg);
	void DGGuildMemberDelRecv(SDHP_GUILD_MEMBER_DEL_RECV* lpMsg);
	void DGGuildMemberUpdateRecv(SDHP_GUILD_MEMBER_UPDATE_RECV* lpMsg);	
	void DGGuildAssignStatusRecv(SDHP_GUILD_ASSIGN_STATUS_RECV* lpMsg);
	void DGGuildAssignTypeRecv(SDHP_GUILD_ASSIGN_TYPE_RECV* lpMsg);
	void DGGuildNoticeRecv(SDHP_GUILD_NOTICE_RECV* lpMsg);
	void DGGuildPostRecv(SDHP_GUILD_POST_RECV* lpMsg);
	void DGGuildScoreRecv(SDHP_GUILD_SCORE_RECV* lpMsg);
	void DGGuildInfoRecv(SDHP_GUILD_INFO_RECV* lpMsg);
	void DGUnionInfoRecv(SDHP_UNION_INFO_RECV* lpMsg);
	void DGUnionUpdateRecv(SDHP_UNION_UPDATE_RECV* lpMsg);
	void DGGuildUnionInfoRecv(SDHP_GUILD_UNION_INFO_RECV* lpMsg);
	void DGGuildUnionJoinRecv(SDHP_GUILD_UNION_JOIN_RECV* lpMsg);
	void DGGuildUnionBreakRecv(SDHP_GUILD_UNION_BREAK_RECV* lpMsg);
	void DGGuildAllianceKickRecv(SDHP_GUILD_ALLIANCE_KICK_RECV* lpMsg);
	void GDGuildMemberJoinSend(int aIndex,char* GuildName,char* Name);
	void GDGuildMemberBreakSend(int aIndex,char* GuildName,char* Name);
	void GDGuildScoreUpdateSend(char* GuildName,int score);
	void GDGuildGlobalChatSend(int type,char* GuildName,char* Name,char* message);
	void GDGuildGlobalNoticeSend(char* GuildName,char* message);
	void CGGuildRequestRecv(PMSG_GUILD_REQUEST_RECV* lpMsg,int aIndex);
	void CGGuildResultRecv(PMSG_GUILD_RESULT_RECV* lpMsg,int aIndex);
	void CGGuildListRecv(int aIndex);
	void CGGuildDeleteRecv(PMSG_GUILD_DELETE_RECV* lpMsg,int aIndex);
	void CGGuildMasterOpenRecv(PMSG_GUILD_MASTER_OPEN_RECV* lpMsg, int aIndex);
	void CGGuildCreateRecv(PMSG_GUILD_CREATE_RECV* lpMsg,int aIndex);
	void CGGuildMasterCancelRecv(int aIndex);
	void CGGuildWarRequestResultRecv(PMSG_GUILD_WAR_REQUEST_RESULT_RECV* lpMsg,int aIndex);
	void CGGuildViewportRequestRecv(PMSG_GUILD_VIEWPORT_REQUEST_RECV* lpMsg,int aIndex);
	void CGGuildAssignStatusRecv(PMSG_GUILD_ASSIGN_STATUS_RECV* lpMsg,int aIndex);
	void CGGuildAssignTypeRecv(PMSG_GUILD_ASSIGN_TYPE_RECV* lpMsg,int aIndex);
	void CGGuildUnionRequestRecv(PMSG_GUILD_UNION_REQUEST_RECV* lpMsg,int aIndex);
	void CGGuildUnionResultRecv(PMSG_GUILD_UNION_RESULT_RECV* lpMsg,int aIndex);
	void CGGuildUnionListRecv(int aIndex);
	void CGGuildAllianceKickRecv(PMSG_GUILD_ALLIANCE_KICK_RECV* lpMsg,int aIndex);
	void GCGuildResultSend(int aIndex,int result);
	void GCGuildDeleteSend(int aIndex,int result);
	void GCGuildCreateResultSend(int aIndex,int result,int type);
	void GCGuildWarResultSend(int aIndex,int result);
	void GCGuildWarRequestSend(int aIndex,char* GuildName,int type);
	void GCGuildWarDeclareSend(int aIndex,char* GuildName,int type,int team);
	void GCGuildWarEndSend(int aIndex,int result,char* GuildName);
	void GCGuildWarScoreSend(int aIndex);
	void GCGuildAssignStatusSend(int aIndex,int type,int result,char* name);
	void GCGuildAssignTypeSend(int aIndex,int type,int result);
	void GCGuildUnionRequestSend(int aIndex,int relation,int type,int bIndex);
	void GCGuildUnionResultSend(int aIndex,int relation,int type,int result,int bIndex);
	void GuildWarDeclareSet(int aIndex,char* GuildName,int type,int team);
	void GuildWarRequestSend(int aIndex,char* GuildName,int type);
	int GetMaxGuildUser(LPOBJ lpObj);
	int GetGuildNameCount(LPOBJ lpObj,char* name);
	int GetGuildBattleMasterCount(LPOBJ lpObj);
	int GetGuildOfficeMasterCount(LPOBJ lpObj);
	int gObjTargetGuildWarCheck(LPOBJ lpObj,LPOBJ lpTarget);
	void gObjGuildWarEndSend(LPOBJ lpObj,int result1,int result2);
	void gObjGuildWarEndSend(GUILD_INFO* lpGuild1, GUILD_INFO* lpGuild2,int result1,int result2);
	void gObjGuildWarEnd(GUILD_INFO* lpGuild1,GUILD_INFO* lpGuild2);
	int gObjGuildWarProc(GUILD_INFO* lpGuild1,GUILD_INFO* lpGuild2,int score);
	int gObjGuildWarCheck(LPOBJ lpObj,LPOBJ lpTargetObj);
	int gObjGuildWarMasterClose(LPOBJ lpObj);
	int gObjGetGuildUnionNumber(LPOBJ lpObj);
	void gObjGetGuildUnionName(LPOBJ lpObj,char* UnionName,int size);
	bool gObjCheckRival(LPOBJ lpObj,LPOBJ lpTarget);
	int gObjGetRelationShip(LPOBJ lpObj,LPOBJ lpTarget);
	void NoticeSendToAll(GUILD_INFO* lpGuild,int type,char* message,...);
	void DataSendToAll(GUILD_INFO* lpGuild,BYTE* lpMsg,int size);
};

extern CGuild gGuild;