// Friend.h: interface for the CFriend class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "DataServerProtocol.h"

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_FRIEND_LIST_RECV
{
	PSBMSG_HEAD header; // C1:B0:00
	WORD aIndex;
	char Name[11];
	BYTE server;
};

struct SDHP_FRIEND_REQUEST_RECV
{
	PSBMSG_HEAD header; // C1:B0:01
	WORD aIndex;
	char Name[11];
	char Target[11];
};

struct SDHP_FRIEND_RESULT_RECV
{
	PSBMSG_HEAD header; // C1:B0:02
	BYTE result;
	WORD aIndex;
	char Name[11];
	char Target[11];
};

struct SDHP_FRIEND_DELETE_RECV
{
	PSBMSG_HEAD header; // C1:B0:03
	WORD aIndex;
	char Name[11];
	char Target[11];
};

struct SDHP_FRIEND_MAIL_RECV
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

struct SDHP_FRIEND_MAIL_LIST_RECV
{
	PSBMSG_HEAD header; // C1:B0:05
	WORD aIndex;
	char Name[11];
};

struct SDHP_FRIEND_MAIL_READ_RECV
{
	PSBMSG_HEAD header; // C1:B0:06
	WORD aIndex;
	WORD index;
	char Name[11];
};

struct SDHP_FRIEND_MAIL_DELETE_RECV
{
	PSBMSG_HEAD header; // C1:B0:07
	WORD aIndex;
	WORD index;
	char Name[11];
};

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_FRIEND_LIST_SEND
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

struct SDHP_FRIEND_RESULT_SEND
{
	PSBMSG_HEAD header; // C1:B0:01
	WORD aIndex;
	char Name[11];
	char Target[11];
};

struct SDHP_FRIEND_STATE_SEND
{
	PSBMSG_HEAD header; // C1:B0:02
	WORD aIndex;
	char Name[11];
	char Target[11];
	BYTE server;
};

struct SDHP_FRIEND_REQUEST_SEND
{
	PSBMSG_HEAD header; // C1:B0:03
	WORD aIndex;
	BYTE result;
	char Name[11];
	char Target[11];
	BYTE server;
};

struct SDHP_FRIEND_DELETE_SEND
{
	PSBMSG_HEAD header; // C1:B0:04
	WORD aIndex;
	BYTE result;
	char Name[11];
	char Target[11];
};

struct SDHP_FRIEND_MAIL_RESULT_SEND
{
	PSBMSG_HEAD header; // C1:B0:05
	WORD aIndex;
	char Name[11];
	BYTE result;
	DWORD window;
};

struct SDHP_FRIEND_MAIL_LIST_SEND
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

struct SDHP_FRIEND_MAIL_READ_SEND
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

struct SDHP_FRIEND_MAIL_DELETE_SEND
{
	PSBMSG_HEAD header; // C1:B0:08
	BYTE result;
	WORD index;
	WORD aIndex;
	char Name[11];
};

//**********************************************//
//**********************************************//
//**********************************************//

class CFriend
{
public:
	CFriend();
	virtual ~CFriend();
	void GDFriendListRecv(SDHP_FRIEND_LIST_RECV* lpMsg,int index);
	void GDFriendRequestRecv(SDHP_FRIEND_REQUEST_RECV* lpMsg,int index);
	void GDFriendResultRecv(SDHP_FRIEND_RESULT_RECV* lpMsg,int index);
	void GDFriendDeleteRecv(SDHP_FRIEND_DELETE_RECV* lpMsg,int index);
	void GDFriendMailRecv(SDHP_FRIEND_MAIL_RECV* lpMsg,int index);
	void GDFriendMailListRecv(SDHP_FRIEND_MAIL_LIST_RECV* lpMsg,int index);
	void GDFriendMailReadRecv(SDHP_FRIEND_MAIL_READ_RECV* lpMsg,int index);
	void GDFriendMailDeleteRecv(SDHP_FRIEND_MAIL_DELETE_RECV* lpMsg,int index);
	void DGFriendResultSend(int index,DWORD userid,WORD aIndex,char* name);
	void DGFriendStateSend(char* name,int state);
	void DGFriendMailListSend(int index,DWORD userid,WORD aIndex,char* name);
};

extern CFriend gFriend;