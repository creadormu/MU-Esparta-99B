// Kalima.h: interface for the CKalima class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "User.h"

class CKalima
{
public:
	CKalima();
	virtual ~CKalima();
	void Load(char* path);
	void MainProc(LPOBJ lpObj);
	bool CreateKalimaPortal(LPOBJ lpObj,int level,int x,int y);
	void CreateKalimaNextPortal(LPOBJ lpObj,LPOBJ lpTarget);
	bool DeleteKalimaPortal(int aIndex);
	void KalimaRefillHP(LPOBJ lpObj);
private:
	int m_KalimaPortal[7];
};

extern CKalima gKalima;
