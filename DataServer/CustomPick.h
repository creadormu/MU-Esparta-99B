// CustomPick.h: interface for the CCustomPick class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "DataServerProtocol.h"

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_CUSTOM_PICK_RECV
{
	PSBMSG_HEAD header; // C1:13:00
	WORD index;
	char account[11];
	char name[11];
};

struct SDHP_CUSTOM_PICK_SAVE_RECV
{
	PSWMSG_HEAD header; // C1:13:30
	WORD index;
	char account[11];
	char name[11];
	BYTE data[400];
	int Money;
	int Jewel;
	int ItemExc;
	int ItemSet;
};

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_CUSTOM_PICK_SEND
{
	PSWMSG_HEAD header; // C1:13:00
	WORD index;
	char account[11];
	char name[11];
	BYTE data[400];
	int Money;
	int Jewel;
	int ItemExc;
	int ItemSet;
};

//**********************************************//
//**********************************************//
//**********************************************//

class CCustomPick
{
public:
	CCustomPick();
	virtual ~CCustomPick();
	void GDCustomPickRecv(SDHP_CUSTOM_PICK_RECV* lpMsg,int index);
	void GDCustomPickSaveRecv(SDHP_CUSTOM_PICK_SAVE_RECV* lpMsg);
};

extern CCustomPick gCustomPick;