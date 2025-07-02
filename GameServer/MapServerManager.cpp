// MapServerManager.cpp: implementation of the CMapServerManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MapServerManager.h"
#include "Map.h"
#include "MapManager.h"
#include "MemScript.h"
#include "ServerInfo.h"
#include "Util.h"

CMapServerManager gMapServerManager;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMapServerManager::CMapServerManager() // OK
{
	
}

CMapServerManager::~CMapServerManager() // OK
{

}

void CMapServerManager::Clear()
{
	for(int n=0;n<MAX_MAP_GROUPS;n++)
	{
		this->m_MapServerGroup[n] = 0;

		for(int i=0;i<MAX_MAP_SUBGROUPS;i++)
		{
			this->m_MapServerData[n][i].Clear(1);
		}
	}

	this->m_MapServerCode.clear();

	this->m_LocalServerData = 0;
}

void CMapServerManager::Load(char* path)
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

	this->Clear();

	try
	{
		while(true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}
		
			int section = lpMemScript->GetNumber();

			while(true)
			{
				if(section == 0)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					int ServerCode = lpMemScript->GetNumber();

					int ServerGroup = lpMemScript->GetAsNumber();

					MAP_SERVER_DATA* info = &this->m_MapServerData[ServerGroup][++this->m_MapServerGroup[ServerGroup]];

					info->Clear(lpMemScript->GetAsNumber());
					
					info->Used = 1;
					
					info->ServerCode = ServerCode;
					
					info->ServerGroup = ServerGroup;
					
					strcpy_s(info->ServerIPAddress, lpMemScript->GetAsString());

					info->ServerPort = lpMemScript->GetAsNumber();
					
					this->m_MapServerCode.insert(std::pair<int,MAP_SERVER_DATA*>(ServerCode,info));
				}
				else if(section == 1)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					int ServerCode = lpMemScript->GetNumber();

					int NoMove = lpMemScript->GetAsNumber();
					
					int MapNumber = lpMemScript->GetAsNumber();

					int NextServerCode = lpMemScript->GetAsNumber();

					std::map<int,MAP_SERVER_DATA*>::iterator it = this->m_MapServerCode.find(ServerCode);

					if(it == this->m_MapServerCode.end() || it->second->Used == 0 || it->second->ServerCode != ServerCode)
					{
						return;
					}

					it->second->MapMove[MapNumber] = ((NoMove == 1) ? -2 : NextServerCode);
				}
				else
				{
					break;
				}
			}
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;

	std::map<int,MAP_SERVER_DATA*>::iterator it = this->m_MapServerCode.find(gServerInfo.m_ServerCode);

	if(it != this->m_MapServerCode.end())
	{
		this->m_LocalServerData = it->second;
	}
}

int CMapServerManager::GetMapServerGroup() // OK
{
	if(this->m_LocalServerData == 0)
	{
		return -1;
	}

	return this->m_LocalServerData->ServerGroup;
}

bool CMapServerManager::GetMapServerData(WORD ServerCode,char* IpAddress,WORD* ServerPort) // OK
{
	if(IpAddress == 0 || ServerPort == 0)
	{
		return 0;
	}

	std::map<int,MAP_SERVER_DATA*>::iterator it = this->m_MapServerCode.find(ServerCode);

	if(it != this->m_MapServerCode.end())
	{
		strcpy_s(IpAddress,sizeof(it->second->ServerIPAddress),it->second->ServerIPAddress);

		(*ServerPort) = it->second->ServerPort;

		return 1;
	}

	return 0;
}

bool CMapServerManager::CheckMapServer(int MapNumber) // OK
{
	if(gMapManager.IsValidMap(MapNumber) == 0)
	{
		return 0;
	}

	if(this->m_LocalServerData == 0)
	{
		return 0;
	}

	if(this->m_LocalServerData->MapMove[MapNumber] != -2)
	{
		return 0;
	}

	return 1;
}

int CMapServerManager::CheckMapServerMove(int aIndex,int MapNumber,int ServerCode) // OK
{
	if (gObjIsConnected(aIndex) == 0)
	{
		return gServerInfo.m_ServerCode;
	}

	if(gMapManager.IsValidMap(MapNumber) == 0)
	{
		return gServerInfo.m_ServerCode;
	}

	if(this->m_LocalServerData == 0)
	{
		return gServerInfo.m_ServerCode;
	}

	int MapMoveInfo = this->m_LocalServerData->MapMove[MapNumber];

	if(MapMoveInfo == -1 && ServerCode != -1)
	{
		std::map<int,MAP_SERVER_DATA*>::iterator it = this->m_MapServerCode.find(ServerCode);

		if(it != this->m_MapServerCode.end() && it->second->MapMove[MapNumber] == -2)
		{
			return ServerCode;
		}
	}
	else if(MapMoveInfo == -2)
	{
		return gServerInfo.m_ServerCode;
	}
	else if(MapMoveInfo >= 0)
	{
		std::map<int,MAP_SERVER_DATA*>::iterator it = this->m_MapServerCode.find(MapMoveInfo);

		if(it != this->m_MapServerCode.end() && it->second->Used == 1 && it->second->MapMove[MapNumber] == -2)
		{
			return it->second->ServerCode;
		}
	}

	return gServerInfo.m_ServerCode;
}
