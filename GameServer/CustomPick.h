// CustomPick.h: interface for the CCustomPick class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Protocol.h"
#include "Map.h"
#include "User.h"

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_CUSTOM_PICK_RECV
{
	PSWMSG_HEAD header; // C1:13:00
	WORD index;
	char account[11];
	char name[11];
	BYTE data[400];
	int PickMoney;
	int PickJewel;
	int PickExc;
	int PickSet;
};

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_CUSTOM_PICK_SEND
{
	PSBMSG_HEAD header; // C1:13:00
	WORD index;
	char account[11];
	char name[11];
};

struct SDHP_CUSTOM_PICK_SAVE_SEND
{
	PSWMSG_HEAD header; // C1:13:30
	WORD index;
	char account[11];
	char name[11];
	BYTE data[400];
	int PickMoney;
	int PickJewel;
	int PickExc;
	int PickSet;
};

//**********************************************//
//**********************************************//
//**********************************************//

struct CUSTOM_PICK_INFO
{
	int ItemIndex;
	int ItemLevel;
};

class CCustomPick
{
public:
	CCustomPick();
	virtual ~CCustomPick();
	void ReadCustomPickInfo(char* section,char* path);
	void Load(char* path);
	void MainProc(LPOBJ lpObj);
	void CommandCustomPick(LPOBJ lpObj,char* arg);
	void CommandCustomAdd(LPOBJ lpObj,char* arg);
	void CommandCustomClear(LPOBJ lpObj);
	bool CheckItem(int ItemIndex,int ItemLevel);
	void OnPickClose(LPOBJ lpObj);
	void DGCustomPickRecv(SDHP_CUSTOM_PICK_RECV* lpMsg);
	void GDCustomPickSend(int aIndex);
	void GDCustomPickSaveSend(int aIndex);
private:
	int m_CustomPickSwitch;
	int m_CustomPickMapZone;
	int m_CustomPickMapList[MAX_MAP];
	int m_CustomPickMaxTime[MAX_ACCOUNT_LEVEL];
	std::vector<CUSTOM_PICK_INFO> m_CustomPickInfo;
};

extern CCustomPick gCustomPick;