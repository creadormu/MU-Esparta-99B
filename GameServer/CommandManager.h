// CommandManager.h: interface for the CCommandManager class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Protocol.h"
#include "User.h"

enum eCommandNumber
{
	COMMAND_MOVE = 0,
	COMMAND_GUILD_WAR = 1,
	COMMAND_BATTLE_SOCCER = 2,
	COMMAND_REQUEST = 3,
	COMMAND_POST = 4,
	COMMAND_ADD_POINT1 = 5,
	COMMAND_ADD_POINT2 = 6,
	COMMAND_ADD_POINT3 = 7,
	COMMAND_ADD_POINT4 = 8,
	COMMAND_ADD_POINT5 = 9,
	COMMAND_PK_CLEAR = 10,
	COMMAND_MONEY = 11,
	COMMAND_CHANGE = 12,
	COMMAND_WARE = 13,
	COMMAND_RESET = 14,
	COMMAND_MASTER_RESET = 15,
	COMMAND_CLEAR_INVENTORY = 16,
	COMMAND_REDISTRIBUTE = 17,
	COMMAND_TREE_RESET = 18,
	COMMAND_RENAME = 19,
	COMMAND_DCFRIEND = 20,
	COMMAND_OFFHELPER = 21,
	COMMAND_CUSTOM_STORE = 50,
	COMMAND_CUSTOM_STORE_OFFLINE = 51,
	COMMAND_CUSTOM_ATTACK = 52,
	COMMAND_CUSTOM_ATTACK_OFFLINE = 53,
	COMMAND_CUSTOM_GIFT = 54,
	COMMAND_CUSTOM_MARRIAGE1 = 55,
	COMMAND_CUSTOM_MARRIAGE2 = 56,
	COMMAND_CUSTOM_MARRIAGE3 = 57,
	COMMAND_CUSTOM_MARRIAGE4 = 58,
	COMMAND_CUSTOM_BUY_VIP = 60,
	COMMAND_CUSTOM_JEWEL_PACK = 61,
	COMMAND_CUSTOM_JEWEL_UNPACK = 62,
	COMMAND_CUSTOM_PICK = 63,
	COMMAND_CUSTOM_PICK_ADD = 64,
	COMMAND_CUSTOM_PICK_CLEAR = 65,
	COMMAND_GM_MOVE = 100,
	COMMAND_GM_POST = 101,
	COMMAND_TRACK = 102,
	COMMAND_TRACE = 103,
	COMMAND_DISCONNECT = 104,
	COMMAND_FIREWORKS = 105,
	COMMAND_MAKE = 106,
	COMMAND_DROP = 107,
	COMMAND_SKIN = 108,
	COMMAND_SET_MONEY = 109,
	COMMAND_HIDE = 110,
	COMMAND_EDIT = 111,
	COMMAND_SET_COIN = 112,
	COMMAND_SET_PK = 113,
	COMMAND_SET_VIP = 114,
	COMMAND_MOVE_ALL = 115,
	COMMAND_MOVE_GUILD = 116,
	COMMAND_MAKE_MOB = 117,
	COMMAND_MAKE_SET = 118,
	COMMAND_BAN_ACC = 119,
	COMMAND_BAN_CHAR = 120,
	COMMAND_BAN_CHAT = 121,
	COMMAND_UNBAN_ACC = 122,
	COMMAND_UNBAN_CHAR = 123,
	COMMAND_UNBAN_CHAT = 124,
	COMMAND_BC_START = 125,
	COMMAND_DS_START = 126,
	COMMAND_CC_START = 127,
	COMMAND_IT_START = 128,
	COMMAND_SET_BUFF = 129,
};

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_COMMAND_RESET_RECV
{
	PSBMSG_HEAD header; // C1:0F:00
	WORD index;
	char account[11];
	char name[11];
	UINT ResetDay;
	UINT ResetWek;
	UINT ResetMon;
};

struct SDHP_COMMAND_MASTER_RESET_RECV
{
	PSBMSG_HEAD header; // C1:0F:01
	WORD index;
	char account[11];
	char name[11];
	UINT MasterResetDay;
	UINT MasterResetWek;
	UINT MasterResetMon;
};

struct SDHP_COMMAND_RENAME_RECV
{
	PSBMSG_HEAD header; // C1:0F:04
	WORD index;
	char account[11];
	char name[11];
	char newname[11];
	BYTE result;
};

struct SDHP_COMMAND_BAN_ACCOUNT_RECV
{
	PSBMSG_HEAD header; // C1:0F:05
	WORD index;
	char account[11];
	int BlocCode;
	BYTE result;
};

struct SDHP_COMMAND_BAN_CHARACTER_RECV
{
	PSBMSG_HEAD header; // C1:0F:06
	WORD index;
	char name[11];
	int CtlCode;
	BYTE result;
};

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_COMMAND_RESET_SEND
{
	PSBMSG_HEAD header; // C1:0F:00
	WORD index;
	char account[11];
	char name[11];
};

struct SDHP_COMMAND_MASTER_RESET_SEND
{
	PSBMSG_HEAD header; // C1:0F:01
	WORD index;
	char account[11];
	char name[11];
};

struct SDHP_COMMAND_RENAME_SEND
{
	PSBMSG_HEAD header; // C1:0F:04
	WORD index;
	char account[11];
	char name[11];
	char newname[11];
};

struct SDHP_COMMAND_BAN_ACCOUNT_SEND
{
	PSBMSG_HEAD header; // C1:0F:05
	WORD index;
	char account[11];
	int BlocCode;
};

struct SDHP_COMMAND_BAN_CHARACTER_SEND
{
	PSBMSG_HEAD header; // C1:0F:06
	WORD index;
	char name[11];
	int CtlCode;
};

//**********************************************//
//**********************************************//
//**********************************************//

struct COMMAND_LIST_INFO
{
	int Index;
	int Switch[MAX_ACCOUNT_LEVEL];
	DWORD Delay;
	int GMLevel;
	int MinLevel;
	int MaxLevel;
	int MinReset;
	int MaxReset;
	DWORD Money[MAX_ACCOUNT_LEVEL];
	DWORD Coin1[MAX_ACCOUNT_LEVEL];
	DWORD Coin2[MAX_ACCOUNT_LEVEL];
	DWORD Coin3[MAX_ACCOUNT_LEVEL];
	char Command[32];
};

class CCommandManager
{
public:
	CCommandManager();
	virtual ~CCommandManager();
	
	void Load(char* path);
	void MainProc();
	long GetNumber(char* arg,int pos);
	void GetString(char* arg,char* out,int size,int pos);
	bool GetCommandCode(int Index,COMMAND_LIST_INFO* lpInfo);
	bool GetCommandCode(char* label,COMMAND_LIST_INFO* lpInfo);
	void ManagementCore(LPOBJ lpObj,char* message);
	void DiscountRequirement(LPOBJ lpObj,int index);
	void CommandMove(LPOBJ lpObj,char* arg);
	void CommandGuildWar(LPOBJ lpObj,char* arg);
	void CommandBattleSoccer(LPOBJ lpObj,char* arg);
	void CommandRequest(LPOBJ lpObj,char* arg);
	void CommandPost(LPOBJ lpObj,char* arg);
	void CommandAddPoint(LPOBJ lpObj,char* arg,int type);
	void CommandAddPointAuto(LPOBJ lpObj,char* arg,int type);
	void CommandPKClear(LPOBJ lpObj,char* syntax);
	void CommandMoney(LPOBJ lpObj,char* arg);
	void CommandChange(LPOBJ lpObj);
	void CommandWare(LPOBJ lpObj,char* arg);
	void CommandReset(LPOBJ lpObj,char* arg);
	void CommandResetAuto(LPOBJ lpObj,char* arg);
	void CommandMasterReset(LPOBJ lpObj,char* arg);
	void CommandClearInventory(LPOBJ lpObj);
	void CommandRedistribute(LPOBJ lpObj);
	void CommandTreeReset(LPOBJ lpObj,char* arg);
	void CommandRename(LPOBJ lpObj,char* arg);
	void CommandDCFriend(LPOBJ lpObj,char* arg);
	void CommandGMMove(LPOBJ lpObj,char* arg);
	void CommandGMPost(LPOBJ lpObj,char* arg);
	void CommandTrack(LPOBJ lpObj,char* arg);
	void CommandTrace(LPOBJ lpObj,char* arg);
	void CommandDisconnect(LPOBJ lpObj,char* arg);
	void CommandFireworks(LPOBJ lpObj);
	void CommandMake(LPOBJ lpObj,char* arg);
	void CommandDrop(LPOBJ lpObj,char* arg);
	void CommandSkin(LPOBJ lpObj,char* arg);
	void CommandSetMoney(LPOBJ lpObj,char* arg);
	void CommandHide(LPOBJ lpObj);
	void CommandEdit(LPOBJ lpObj,char* arg);
	void CommandSetCoin(LPOBJ lpObj,char* arg);
	void CommandSetPK(LPOBJ lpObj,char* arg);
	void CommandSetVip(LPOBJ lpObj,char* arg);
	void CommandMoveAll(LPOBJ lpObj,char* arg);
	void CommandMoveGuild(LPOBJ lpObj,char* arg);
	void CommandMakeMob(LPOBJ lpObj,char* arg);
	void CommandMakeSet(LPOBJ lpObj,char* arg);
	void CommandBanAcc(LPOBJ lpObj,char* arg);
	void CommandBanChar(LPOBJ lpObj,char* arg);
	void CommandBanChat(LPOBJ lpObj,char* arg);
	void CommandUnbanAcc(LPOBJ lpObj,char* arg);
	void CommandUnbanChar(LPOBJ lpObj,char* arg);
	void CommandUnbanChat(LPOBJ lpObj,char* arg);
	void CommandBCStart(LPOBJ lpObj);
	void CommandDSStart(LPOBJ lpObj);
	void CommandCCStart(LPOBJ lpObj);
	void CommandITStart(LPOBJ lpObj);
	void CommandSetBuff(LPOBJ lpObj,char* arg);
	void CommandAddPointAutoProc(LPOBJ lpObj);
	void CommandResetAutoProc(LPOBJ lpObj);
	void DGCommandResetRecv(SDHP_COMMAND_RESET_RECV* lpMsg);
	void DGCommandMasterResetRecv(SDHP_COMMAND_MASTER_RESET_RECV* lpMsg);
	void DGCommandRenameRecv(SDHP_COMMAND_RENAME_RECV* lpMsg);
	void DGCommandBanAccountRecv(SDHP_COMMAND_BAN_ACCOUNT_RECV* lpMsg);
	void DGCommandBanCharacterRecv(SDHP_COMMAND_BAN_CHARACTER_RECV* lpMsg);
private:
	std::map<int,COMMAND_LIST_INFO> m_CommandListInfo;
};

extern CCommandManager gCommandManager;
