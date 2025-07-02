// CustomGift.h: interface for the CCustomGift class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "User.h"
#include "Protocol.h"

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_COMMAND_GIFT_SEND
{
	PSBMSG_HEAD header; // C1:0F:03
	WORD index;
	char Name[11];
	WORD GiftIndex;
	DWORD MaxCount;
};

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_COMMAND_GIFT_RECV
{
	PSBMSG_HEAD header; // C1:0F:03
	WORD index;
	char Name[11];
	WORD GiftIndex;
	BYTE Result;
};

//**********************************************//
//**********************************************//
//**********************************************//

struct GIFT_LIST_ITEM_INFO
{
	int Index;
	int ItemIndex;
	int Level;
	int Dur;
	int Option1;
	int Option2;
	int Option3;
	int NewOption;
	int SetOption;
	int SocketCount;
	int Duration;
};

struct GIFT_LIST_INFO
{
	int Index;
	int Class;
	int Enable[MAX_ACCOUNT_LEVEL];
	int MinLevel;
	int MaxLevel;
	int MinReset;
	int MaxReset;
	int MaxCount;
	int LevelUpPoint;
	DWORD Money;
	int Coin1;
	int Coin2;
	int Coin3;
	char Code[32];
	std::vector<GIFT_LIST_ITEM_INFO> ItemList;
};

class CCustomGift
{
public:
	CCustomGift();
	virtual ~CCustomGift();
	void ReadCustomGiftInfo(char* section,char* path);
	void Load(char* path);
	void SetInfo(GIFT_LIST_INFO info);
	void SetItemInfo(GIFT_LIST_ITEM_INFO info);
	bool CommandGift(LPOBJ lpObj,char* arg);
	void DGCommandGiftRecv(SDHP_COMMAND_GIFT_RECV* lpMsg);
private:
	int m_CustomGiftSwitch;
	std::map<int,GIFT_LIST_INFO> m_GiftListInfo;
};

extern CCustomGift gCustomGift;