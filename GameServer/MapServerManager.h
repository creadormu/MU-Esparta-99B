// MapServerManager.h: interface for the CMapServerManager class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Map.h"

#define MAX_MAP_GROUPS 20
#define MAX_MAP_SUBGROUPS 20

class MAP_SERVER_DATA
{
public:
	MAP_SERVER_DATA() // OK
	{
		this->Clear(1);
	}

	void Clear(int value) // OK
	{
		this->Used = 0;

		this->ServerGroup = -1;

		this->ServerCode = -1;

		memset(this->ServerIPAddress,0,sizeof(this->ServerIPAddress));

		this->ServerPort = 0;

		for(int n=0;n < MAX_MAP;n++)
		{
			switch(value)
			{
				case 0:
					this->MapMove[n] = -1;
					break;
				default:
					this->MapMove[n] = -2;
					break;
			}
		}
	}
public:
	int Used;
	int ServerGroup;
	int ServerCode;
	char ServerIPAddress[16];
	int ServerPort;
	int MapMove[MAX_MAP];
};

class CMapServerManager
{
public:
	CMapServerManager();
	virtual ~CMapServerManager();
	void Clear();
	void Load(char* path);
	int GetMapServerGroup();
	bool GetMapServerData(WORD ServerCode,char* IpAddress,WORD* ServerPort);
	bool CheckMapServer(int MapNumber);
	int CheckMapServerMove(int aIndex,int MapNumber,int ServerCode);
public:
	MAP_SERVER_DATA m_MapServerData[MAX_MAP_GROUPS][MAX_MAP_SUBGROUPS];
	int m_MapServerGroup[MAX_MAP_GROUPS];
	std::map<int,MAP_SERVER_DATA*> m_MapServerCode;
	MAP_SERVER_DATA* m_LocalServerData;
};

extern CMapServerManager gMapServerManager;
