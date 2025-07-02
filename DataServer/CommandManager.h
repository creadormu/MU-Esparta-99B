// CommandManager.h: interface for the CCommandManager class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "DataServerProtocol.h"

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_COMMAND_RESET_RECV
{
	PSBMSG_HEAD header; // C1:0F:00
	WORD index;
	char account[11];
	char name[11];
};

struct SDHP_COMMAND_MASTER_RESET_RECV
{
	PSBMSG_HEAD header; // C1:0F:01
	WORD index;
	char account[11];
	char name[11];
};

struct SDHP_COMMAND_DIVORCE_RECV
{
	PSBMSG_HEAD header; // C1:0F:02
	int index;
	char name[11];
};

struct SDHP_COMMAND_GIFT_RECV
{
	PSBMSG_HEAD header; // C1:0F:03
	WORD index;
	char Name[11];
	WORD GiftIndex;
	DWORD MaxCount;
};

struct SDHP_COMMAND_RENAME_RECV
{
	PSBMSG_HEAD header; // C1:0F:04
	WORD index;
	char account[11];
	char name[11];
	char newname[11];
};

struct SDHP_COMMAND_BAN_ACCOUNT_RECV
{
	PSBMSG_HEAD header; // C1:0F:05
	WORD index;
	char account[11];
	int BlocCode;
};

struct SDHP_COMMAND_BAN_CHARACTER_RECV
{
	PSBMSG_HEAD header; // C1:0F:06
	WORD index;
	char name[11];
	int CtlCode;
};

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_COMMAND_RESET_SEND
{
	PSBMSG_HEAD header; // C1:0F:00
	WORD index;
	char account[11];
	char name[11];
	UINT ResetDay;
	UINT ResetWek;
	UINT ResetMon;
};

struct SDHP_COMMAND_MASTER_RESET_SEND
{
	PSBMSG_HEAD header; // C1:0F:01
	WORD index;
	char account[11];
	char name[11];
	UINT MasterResetDay;
	UINT MasterResetWek;
	UINT MasterResetMon;
};

struct SDHP_COMMAND_GIFT_SEND
{
	PSBMSG_HEAD header; // C1:0F:03
	WORD index;
	char Name[11];
	WORD GiftIndex;
	BYTE Result;
};

struct SDHP_COMMAND_RENAME_SEND
{
	PSBMSG_HEAD header; // C1:0F:04
	WORD index;
	char account[11];
	char name[11];
	char newname[11];
	BYTE result;
};

struct SDHP_COMMAND_BAN_ACCOUNT_SEND
{
	PSBMSG_HEAD header; // C1:0F:05
	WORD index;
	char account[11];
	int BlocCode;
	BYTE result;
};

struct SDHP_COMMAND_BAN_CHARACTER_SEND
{
	PSBMSG_HEAD header; // C1:0F:06
	WORD index;
	char name[11];
	int CtlCode;
	BYTE result;
};

//**********************************************//
//**********************************************//
//**********************************************//

class CCommandManager
{
public:
	CCommandManager();
	virtual ~CCommandManager();
	void GDCommandResetRecv(SDHP_COMMAND_RESET_RECV* lpMsg,int index);
	void GDCommandMasterResetRecv(SDHP_COMMAND_MASTER_RESET_RECV* lpMsg,int index);
	void GDCommandDivorceRecv(SDHP_COMMAND_DIVORCE_RECV* lpMsg,int index);
	void GDCommandGiftRecv(SDHP_COMMAND_GIFT_RECV* lpMsg,int index);
	void GDCommandRenameRecv(SDHP_COMMAND_RENAME_RECV* lpMsg,int index);
	void GDCommandBanAccountRecv(SDHP_COMMAND_BAN_ACCOUNT_RECV* lpMsg,int index);
	void GDCommandBanCharacterRecv(SDHP_COMMAND_BAN_CHARACTER_RECV* lpMsg,int index);
};

extern CCommandManager gCommandManager;
