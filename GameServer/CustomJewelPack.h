// CustomJewelPack.h: interface for the CCustomJewelPack class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "User.h"

class CCustomJewelPack
{
public:
	CCustomJewelPack();
	virtual ~CCustomJewelPack();
	void ReadCustomJewelPackInfo(char* section,char* path);
	void CommandCustomJewelPack(LPOBJ lpObj,char* arg);
	void CommandCustomJewelUnpack(LPOBJ lpObj,char* arg);
	void JewelPack(LPOBJ lpObj,char* arg,int type);
	void JewelUnpack(LPOBJ lpObj,char* arg,int type);
	int GetJewelSimpleIndex(int type);
	int GetJewelBundleIndex(int type);
private:
	int m_CustomJewelPackSwitch;
	char m_CustomJewelPackCommandJoBSyntax[32];
	char m_CustomJewelPackCommandJoSSyntax[32];
	char m_CustomJewelPackCommandJoLSyntax[32];
	char m_CustomJewelPackCommandJCRSyntax[32];
	char m_CustomJewelPackCommandJGMSyntax[32];
	char m_CustomJewelPackCommandJoHSyntax[32];
	char m_CustomJewelPackCommandJoCSyntax[32];
	char m_CustomJewelPackCommandJoGSyntax[32];
	char m_CustomJewelPackCommandJLRSyntax[32];
	char m_CustomJewelPackCommandJHRSyntax[32];
};

extern CCustomJewelPack gCustomJewelPack;