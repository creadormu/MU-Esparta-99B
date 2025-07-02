// ResetLimit.h: interface for the CResetLimit class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "User.h"

struct RESET_LIMIT_INFO
{
	int MinReset;
	int MaxReset;
	int LimitDay[MAX_ACCOUNT_LEVEL];
	int LimitWek[MAX_ACCOUNT_LEVEL];
	int LimitMon[MAX_ACCOUNT_LEVEL];
};

class CResetLimit
{
public:
	CResetLimit();
	virtual ~CResetLimit();
	void Load(char* path);
	DWORD GetLimitDay(LPOBJ lpObj);
	DWORD GetLimitWek(LPOBJ lpObj);
	DWORD GetLimitMon(LPOBJ lpObj);
private:
	std::vector<RESET_LIMIT_INFO> m_ResetLimitInfo;
};

extern CResetLimit gResetLimit;
