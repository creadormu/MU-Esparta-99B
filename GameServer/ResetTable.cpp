// ResetTable.cpp: implementation of the CResetTable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ResetTable.h"
#include "MemScript.h"
#include "ServerInfo.h"
#include "Util.h"

CResetTable gResetTable;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CResetTable::CResetTable() // OK
{
	this->m_ResetTableInfo.clear();
}

CResetTable::~CResetTable() // OK
{

}

void CResetTable::Load(char* path) // OK
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

	this->m_ResetTableInfo.clear();

	try
	{
		while(true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			if(strcmp("end",lpMemScript->GetString()) == 0)
			{
				break;
			}

			RESET_TABLE_INFO info;

			memset(&info,0,sizeof(info));

			info.MinReset = lpMemScript->GetNumber();

			info.MaxReset = lpMemScript->GetAsNumber();

			info.Level[0] = lpMemScript->GetAsNumber();

			info.Level[1] = lpMemScript->GetAsNumber();

			info.Level[2] = lpMemScript->GetAsNumber();

			info.Level[3] = lpMemScript->GetAsNumber();

			info.Money[0] = lpMemScript->GetAsNumber();

			info.Money[1] = lpMemScript->GetAsNumber();

			info.Money[2] = lpMemScript->GetAsNumber();

			info.Money[3] = lpMemScript->GetAsNumber();

			info.Point[0] = lpMemScript->GetAsNumber();

			info.Point[1] = lpMemScript->GetAsNumber();

			info.Point[2] = lpMemScript->GetAsNumber();

			info.Point[3] = lpMemScript->GetAsNumber();

			#if(GAMESERVER_SHOP>=1)

			info.Coin1[0] = lpMemScript->GetAsNumber();

			info.Coin1[1] = lpMemScript->GetAsNumber();

			info.Coin1[2] = lpMemScript->GetAsNumber();

			info.Coin1[3] = lpMemScript->GetAsNumber();

			#endif

			#if(GAMESERVER_SHOP==3)

			info.Coin2[0] = lpMemScript->GetAsNumber();

			info.Coin2[1] = lpMemScript->GetAsNumber();

			info.Coin2[2] = lpMemScript->GetAsNumber();

			info.Coin2[3] = lpMemScript->GetAsNumber();

			info.Coin3[0] = lpMemScript->GetAsNumber();

			info.Coin3[1] = lpMemScript->GetAsNumber();

			info.Coin3[2] = lpMemScript->GetAsNumber();

			info.Coin3[3] = lpMemScript->GetAsNumber();

			#endif

			this->m_ResetTableInfo.push_back(info);
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

int CResetTable::GetResetLevel(LPOBJ lpObj) // OK
{
	for(std::vector<RESET_TABLE_INFO>::iterator it=this->m_ResetTableInfo.begin();it != this->m_ResetTableInfo.end();it++)
	{
		if((lpObj->Reset+1) >= it->MinReset && (lpObj->Reset+1) <= it->MaxReset)
		{
			if(it->Level[lpObj->AccountLevel] == -1)
			{
				return gServerInfo.m_CommandResetLevel[lpObj->AccountLevel];
			}
			else
			{
				return it->Level[lpObj->AccountLevel];
			}
		}
	}

	return gServerInfo.m_CommandResetLevel[lpObj->AccountLevel];
}

DWORD CResetTable::GetResetMoney(LPOBJ lpObj) // OK
{
	for(std::vector<RESET_TABLE_INFO>::iterator it=this->m_ResetTableInfo.begin();it != this->m_ResetTableInfo.end();it++)
	{
		if((lpObj->Reset+1) >= it->MinReset && (lpObj->Reset+1) <= it->MaxReset)
		{
			if(it->Money[lpObj->AccountLevel] == -1)
			{
				return gServerInfo.m_CommandResetMoney[lpObj->AccountLevel];
			}
			else
			{
				return it->Money[lpObj->AccountLevel];
			}
		}
	}

	return gServerInfo.m_CommandResetMoney[lpObj->AccountLevel];
}

int CResetTable::GetResetPoint(LPOBJ lpObj) // OK
{
	int point = 0;

	for(int n=1;n <= lpObj->Reset;n++)
	{
		int AddPoint = gServerInfo.m_CommandResetPoint[lpObj->AccountLevel];

		for(std::vector<RESET_TABLE_INFO>::iterator it=this->m_ResetTableInfo.begin();it != this->m_ResetTableInfo.end();it++)
		{
			if(n >= it->MinReset && n <= it->MaxReset)
			{
				if(it->Point[lpObj->AccountLevel] == -1)
				{
					AddPoint = gServerInfo.m_CommandResetPoint[lpObj->AccountLevel];
					break;
				}
				else
				{
					AddPoint = it->Point[lpObj->AccountLevel];
					break;
				}
			}
		}

		point += AddPoint;
	}

	return point;
}

DWORD CResetTable::GetResetCoin1(LPOBJ lpObj) // OK
{
	#if(GAMESERVER_SHOP>=1)

	for(std::vector<RESET_TABLE_INFO>::iterator it = this->m_ResetTableInfo.begin(); it != this->m_ResetTableInfo.end(); it++)
	{
		if((lpObj->Reset+1) >= it->MinReset && (lpObj->Reset+1) <= it->MaxReset)
		{
			if(it->Coin1[lpObj->AccountLevel] == -1)
			{
				return gServerInfo.m_CommandResetCoin1[lpObj->AccountLevel];
			}
			else
			{
				return it->Coin1[lpObj->AccountLevel];
			}
		}
	}

	return gServerInfo.m_CommandResetCoin1[lpObj->AccountLevel];

	#else

	return 0;

	#endif
}

DWORD CResetTable::GetResetCoin2(LPOBJ lpObj) // OK
{
	#if(GAMESERVER_SHOP==3)

	for(std::vector<RESET_TABLE_INFO>::iterator it = this->m_ResetTableInfo.begin(); it != this->m_ResetTableInfo.end(); it++)
	{
		if((lpObj->Reset+1) >= it->MinReset && (lpObj->Reset+1) <= it->MaxReset)
		{
			if(it->Coin2[lpObj->AccountLevel] == -1)
			{
				return gServerInfo.m_CommandResetCoin2[lpObj->AccountLevel];
			}
			else
			{
				return it->Coin2[lpObj->AccountLevel];
			}
		}
	}

	return gServerInfo.m_CommandResetCoin2[lpObj->AccountLevel];
	
	#else

	return 0;
	
	#endif
}

DWORD CResetTable::GetResetCoin3(LPOBJ lpObj) // OK
{
	#if(GAMESERVER_SHOP==3)

	for(std::vector<RESET_TABLE_INFO>::iterator it = this->m_ResetTableInfo.begin(); it != this->m_ResetTableInfo.end(); it++)
	{
		if((lpObj->Reset+1) >= it->MinReset && (lpObj->Reset+1) <= it->MaxReset)
		{
			if(it->Coin3[lpObj->AccountLevel] == -1)
			{
				return gServerInfo.m_CommandResetCoin3[lpObj->AccountLevel];
			}
			else
			{
				return it->Coin3[lpObj->AccountLevel];
			}
		}
	}

	return gServerInfo.m_CommandResetCoin3[lpObj->AccountLevel];
	
	#else
	
	return 0;

	#endif
}