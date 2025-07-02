// ResetLimit.cpp: implementation of the CResetLimit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ResetLimit.h"
#include "MemScript.h"
#include "ServerInfo.h"
#include "Util.h"

CResetLimit gResetLimit;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CResetLimit::CResetLimit() // OK
{
	this->m_ResetLimitInfo.clear();
}

CResetLimit::~CResetLimit() // OK
{

}

void CResetLimit::Load(char* path) // OK
{
	CMemScript* lpMemScript = new CMemScript;

	if(lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR,path);
		return;
	}

	if(lpMemScript->SetBuffer(path) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	this->m_ResetLimitInfo.clear();

	try
	{
		while (true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			if(strcmp("end",lpMemScript->GetString()) == 0)
			{
				break;
			}

			RESET_LIMIT_INFO info;

			memset(&info,0,sizeof(info));

			info.MinReset = lpMemScript->GetNumber();

			info.MaxReset = lpMemScript->GetAsNumber();

			info.LimitDay[0] = lpMemScript->GetAsNumber();

			info.LimitDay[1] = lpMemScript->GetAsNumber();

			info.LimitDay[2] = lpMemScript->GetAsNumber();

			info.LimitDay[3] = lpMemScript->GetAsNumber();

			info.LimitWek[0] = lpMemScript->GetAsNumber();

			info.LimitWek[1] = lpMemScript->GetAsNumber();

			info.LimitWek[2] = lpMemScript->GetAsNumber();

			info.LimitWek[3] = lpMemScript->GetAsNumber();

			info.LimitMon[0] = lpMemScript->GetAsNumber();

			info.LimitMon[1] = lpMemScript->GetAsNumber();

			info.LimitMon[2] = lpMemScript->GetAsNumber();

			info.LimitMon[3] = lpMemScript->GetAsNumber();

			this->m_ResetLimitInfo.push_back(info);
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

DWORD CResetLimit::GetLimitDay(LPOBJ lpObj) // OK
{
	for(std::vector<RESET_LIMIT_INFO>::iterator it = this->m_ResetLimitInfo.begin(); it != this->m_ResetLimitInfo.end(); it++)
	{
		if((lpObj->Reset+1) >= it->MinReset && (lpObj->Reset+1) <= it->MaxReset)
		{
			if(it->LimitDay[lpObj->AccountLevel] == -1)
			{
				return gServerInfo.m_CommandResetLimitDay[lpObj->AccountLevel];
			}
			else
			{
				return it->LimitDay[lpObj->AccountLevel];
			}
		}
	}

	return gServerInfo.m_CommandResetLimitDay[lpObj->AccountLevel];
}

DWORD CResetLimit::GetLimitWek(LPOBJ lpObj) // OK
{
	for(std::vector<RESET_LIMIT_INFO>::iterator it = this->m_ResetLimitInfo.begin(); it != this->m_ResetLimitInfo.end(); it++)
	{
		if((lpObj->Reset+1) >= it->MinReset && (lpObj->Reset+1) <= it->MaxReset)
		{
			if(it->LimitWek[lpObj->AccountLevel] == -1)
			{
				return gServerInfo.m_CommandResetLimitWek[lpObj->AccountLevel];
			}
			else
			{
				return it->LimitWek[lpObj->AccountLevel];
			}
		}
	}

	return gServerInfo.m_CommandResetLimitWek[lpObj->AccountLevel];
}

DWORD CResetLimit::GetLimitMon(LPOBJ lpObj) // OK
{
	for(std::vector<RESET_LIMIT_INFO>::iterator it = this->m_ResetLimitInfo.begin(); it != this->m_ResetLimitInfo.end(); it++)
	{
		if((lpObj->Reset+1) >= it->MinReset && (lpObj->Reset+1) <= it->MaxReset)
		{
			if(it->LimitMon[lpObj->AccountLevel] == -1)
			{
				return gServerInfo.m_CommandResetLimitMon[lpObj->AccountLevel];
			}
			else
			{
				return it->LimitMon[lpObj->AccountLevel];
			}
		}
	}

	return gServerInfo.m_CommandResetLimitMon[lpObj->AccountLevel];
}