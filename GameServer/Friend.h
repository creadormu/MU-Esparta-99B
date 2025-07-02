// Friend.h: interface for the CFriend class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Protocol.h"

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_FRIEND_LIST_RECV
{
	PSWMSG_HEAD header; // C1:B0:00
	WORD aIndex;
	char Name[11];
	BYTE count;
	BYTE MailCount;
};

struct SDHP_FRIEND_LIST
{
	char Name[11];
	BYTE server;
};

struct SDHP_FRIEND_RESULT_RECV
{
	PSBMSG_HEAD header; // C1:B0:01
	WORD aIndex;
	char Name[11];
	char Target[11];
};

struct SDHP_FRIEND_STATE_RECV
{
	PSBMSG_HEAD header; // C1:B0:02
	WORD aIndex;
	char Name[11];
	char Target[11];
	BYTE server;
};

struct SDHP_FRIEND_REQUEST_RECV
{
	PSBMSG_HEAD header; // C1:B0:03
	WORD aIndex;
	BYTE result;
	char Name[11];
	char Target[11];
	BYTE server;
};

struct SDHP_FRIEND_DELETE_RECV
{
	PSBMSG_HEAD header; // C1:B0:04
	WORD aIndex;
	BYTE result;
	char Name[11];
	char Target[11];
};

struct SDHP_FRIEND_MAIL_RESULT_RECV
{
	PSBMSG_HEAD header; // C1:B0:05
	WORD aIndex;
	char Name[11];
	BYTE result;
	DWORD window;
};

struct SDHP_FRIEND_MAIL_LIST_RECV
{
	PSWMSG_HEAD header; // C1:B0:06
	WORD aIndex;
	WORD index;
	char SendName[11];
	char RecvName[11];
	char Date[30];
	char Subject[32];
	BYTE read;
};

struct SDHP_FRIEND_MAIL_READ_RECV
{
	PSWMSG_HEAD header; // C1:B0:07
	WORD aIndex;
	char Name[11];
	WORD index;
	WORD mailsize;
	BYTE CharSet[18];
	BYTE direction;
	BYTE action;
	char Mail[1000];
};

struct SDHP_FRIEND_MAIL_DELETE_RECV
{
	PSBMSG_HEAD header; // C1:B0:08
	BYTE result;
	WORD index;
	WORD aIndex;
	char Name[11];
};

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_FRIEND_LIST_SEND
{
	PSBMSG_HEAD header; // C1:B0:00
	WORD aIndex;
	char Name[11];
	BYTE server;
};

struct SDHP_FRIEND_REQUEST_SEND
{
	PSBMSG_HEAD header; // C1:B0:01
	WORD aIndex;
	char Name[11];
	char Target[11];
};

struct SDHP_FRIEND_RESULT_SEND
{
	PSBMSG_HEAD header; // C1:B0:02
	BYTE result;
	WORD aIndex;
	char Name[11];
	char Target[11];
};

struct SDHP_FRIEND_DELETE_SEND
{
	PSBMSG_HEAD header; // C1:B0:03
	WORD aIndex;
	char Name[11];
	char Target[11];
};

struct SDHP_FRIEND_MAIL_SEND
{
	PSWMSG_HEAD header; // C1:B0:04
	WORD aIndex;
	DWORD window;
	char Name[11];
	char Target[11];
	char Title[32];
	BYTE direction;
	BYTE action;
	WORD mailsize;
	BYTE CharSet[18];
	char Mail[1000];
};

struct SDHP_FRIEND_MAIL_LIST_SEND
{
	PSBMSG_HEAD header; // C1:B0:05
	WORD aIndex;
	char Name[11];
};

struct SDHP_FRIEND_MAIL_READ_SEND
{
	PSBMSG_HEAD header; // C1:B0:06
	WORD aIndex;
	WORD index;
	char Name[11];
};

struct SDHP_FRIEND_MAIL_DELETE_SEND
{
	PSBMSG_HEAD header; // C1:B0:07
	WORD aIndex;
	WORD index;
	char Name[11];
};

//**********************************************//
//************ Client -> GameServer ************//
//**********************************************//

struct PMSG_FRIEND_REQUEST_RECV
{
	PBMSG_HEAD header; // C1:C1
	char Name[10];
};

struct PMSG_FRIEND_RESULT_RECV
{
	PBMSG_HEAD header; // C1:C2
	BYTE result;
	char Name[10];
};

struct PMSG_FRIEND_DELETE_RECV
{
	PBMSG_HEAD header; // C1:C3
	char Name[10];
};

struct PMSG_FRIEND_MAIL_RECV
{
	PWMSG_HEAD header; // C1:C5
	DWORD window;
	char Name[10];
	#if(GAMESERVER_UPDATE>=603)
	char Title[60];
	#else
	char Title[32];
	#endif
	BYTE action;
	BYTE direction;
	WORD mailsize;
	char Mail[1000];
};

struct PMSG_FRIEND_MAIL_READ_RECV
{
	PBMSG_HEAD header; // C1:C7
	WORD index;
};

struct PMSG_FRIEND_MAIL_DELETE_RECV
{
	PBMSG_HEAD header; // C1:C8
	WORD index;
};

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_FRIEND_LIST_SEND
{
	PWMSG_HEAD header; // C1:C0
	BYTE MailCount;
	BYTE MailTotal;
	BYTE count;
};

struct PMSG_FRIEND_LIST
{
	char Name[10];
	BYTE server;
};

struct PMSG_FRIEND_REQUEST_SEND
{
	PBMSG_HEAD header; // C1:C1
	BYTE result;
	char Name[10];
	BYTE server;
};

struct PMSG_FRIEND_RESULT_SEND
{
	PBMSG_HEAD header; // C1:C2
	char Name[10];
};

struct PMSG_FRIEND_DELETE_SEND
{
	PBMSG_HEAD header; // C1:C3
	BYTE result;
	char Name[10];
};

struct PMSG_FRIEND_STATE_SEND
{
	PBMSG_HEAD header; // C1:C4
	char Name[10];
	BYTE server;
};

struct PMSG_FRIEND_MAIL_RESULT_SEND
{
	PBMSG_HEAD header; // C1:C5
	BYTE result;
	DWORD window;
};

struct PMSG_FRIEND_MAIL_SEND
{
	PBMSG_HEAD header; // C1:C6
	WORD index;
	char Name[10];
	char Date[30];
	#if(GAMESERVER_UPDATE>=603)
	char Title[60];
	#else
	char Title[32];
	#endif
	BYTE read;
};

struct PMSG_FRIEND_MAIL_READ_SEND
{
	PWMSG_HEAD header; // C1:C7
	WORD index;
	WORD mailsize;
	BYTE CharSet[18];
	BYTE action;
	BYTE direction;
	char Mail[1000];
};

struct PMSG_FRIEND_MAIL_DELETE_SEND
{
	PBMSG_HEAD header; // C1:C8
	BYTE result;
	WORD index;
};

//**********************************************//
//**********************************************//
//**********************************************//

class CFriend
{
public:
	CFriend();
	virtual ~CFriend();
	void DGFriendListRecv(SDHP_FRIEND_LIST_RECV* lpMsg);
	void DGFriendResultRecv(SDHP_FRIEND_RESULT_RECV* lpMsg);
	void DGFriendRequestRecv(SDHP_FRIEND_REQUEST_RECV* lpMsg);
	void DGFriendDeleteRecv(SDHP_FRIEND_DELETE_RECV* lpMsg);
	void DGFriendMailResultRecv(SDHP_FRIEND_MAIL_RESULT_RECV* lpMsg);
	void DGFriendMailReadRecv(SDHP_FRIEND_MAIL_READ_RECV* lpMsg);
	void DGFriendMailDeleteRecv(SDHP_FRIEND_MAIL_DELETE_RECV* lpMsg);
	void DGFriendMailListRecv(SDHP_FRIEND_MAIL_LIST_RECV* lpMsg);
	void DGFriendStateRecv(SDHP_FRIEND_STATE_RECV* lpMsg);
	void GDFriendMailSend(char* Name,char* Target,char* Title,BYTE direction,BYTE action,BYTE* CharSet,char* Text);
	void CGFriendListRecv(int aIndex);
	void CGFriendRequestRecv(PMSG_FRIEND_REQUEST_RECV*lpMsg,int aIndex);
	void CGFriendResultRecv(PMSG_FRIEND_RESULT_RECV* lpMsg, int aIndex);
	void CGFriendDeleteRecv(PMSG_FRIEND_DELETE_RECV* lpMsg, int aIndex);
	void CGFriendMailRecv(PMSG_FRIEND_MAIL_RECV* lpMsg, int aIndex);
	void CGFriendMailReadRecv(PMSG_FRIEND_MAIL_READ_RECV* lpMsg, int aIndex);
	void CGFriendMailDeleteRecv(PMSG_FRIEND_MAIL_DELETE_RECV* lpMsg, int aIndex);
	void CGFriendMailListRecv(int aIndex);
	void GCFriendRequestSend(int aIndex,int server,char* Name,int result);
	void GCFriendMailResultSend(int aIndex,int window,int result);
};

extern CFriend gFriend;