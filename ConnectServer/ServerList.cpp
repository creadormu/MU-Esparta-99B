// ServerList.cpp: implementation of the CServerList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ServerList.h"
#include "MemScript.h"
#include "Util.h"

CServerList gServerList;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CServerList::CServerList() // OK
{
	this->m_JoinServerState = 0;

	this->m_JoinServerStateTime = 0;

	this->m_JoinServerQueueSize = 0;

	this->m_ServerListInfo.clear();
}

CServerList::~CServerList() // OK
{

}

void CServerList::Load(char* path) // OK
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

	this->m_GameServersCount = 0;

	this->m_ServerListInfo.clear();

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

			this->m_GameServersList = 0;

			SERVER_LIST_INFO info;

			memset(&info,0,sizeof(info));

			info.ServerCode = lpMemScript->GetNumber();

			strcpy_s(info.ServerName,lpMemScript->GetAsString());

			strcpy_s(info.ServerAddress,lpMemScript->GetAsString());

			info.ServerPort = lpMemScript->GetAsNumber();

			info.ServerShow = ((lpMemScript->GetAsNumber()>0)?1:0);

			this->m_ServerListInfo.insert(std::pair<int,SERVER_LIST_INFO>(info.ServerCode,info));

			this->m_GameServersCount++;
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CServerList::MainProc() // OK
{
	if(this->m_JoinServerState != 0 && (GetTickCount()-this->m_JoinServerStateTime) > 10000)
	{
		this->m_JoinServerState = 0;
		this->m_JoinServerStateTime = 0;
		LogAdd(LOG_RED, "[SocketUDP] JoinServer disconnected");
	}

	for(std::map<int,SERVER_LIST_INFO>::iterator it=this->m_ServerListInfo.begin();it != this->m_ServerListInfo.end();it++)
	{
		if(it->second.ServerState != 0 && (GetTickCount()-it->second.ServerStateTime) > 10000)
		{
			it->second.ServerState = 0;
			it->second.ServerStateTime = 0;
			LogAdd(LOG_RED,"[SocketUDP] GameServer disconnected [%s] [%s:%d][%d]",it->second.ServerName,it->second.ServerAddress,it->second.ServerPort,it->second.ServerCode);
			this->m_GameServersList--;
		}
	}
}

bool CServerList::CheckJoinServerState() // OK
{
	if(this->m_JoinServerState == 0)
	{
		return 0;
	}

	if(this->m_JoinServerQueueSize > MAX_JOIN_SERVER_QUEUE_SIZE)
	{
		return 0;
	}

	return 1;
}

int CServerList::GenerateServerList(BYTE* lpMsg,int* size) // OK
{
	int count = 0;

	PMSG_SERVER_LIST info;

	if(this->CheckJoinServerState() != 0)
	{
		for(std::map<int,SERVER_LIST_INFO>::iterator it=this->m_ServerListInfo.begin();it != this->m_ServerListInfo.end();it++)
		{
			if(it->second.ServerShow != 0 && it->second.ServerState != 0)
			{
				info.ServerCode = it->second.ServerCode;

				info.UserTotal = (BYTE)((it->second.UserCount*100)/it->second.UserTotal);

				info.type = 0xCC;

				memcpy(&lpMsg[(*size)],&info,sizeof(info));
				(*size) += sizeof(info);

				count++;
			}
		}
	}

	return count;
}

int CServerList::GenerateServerListName(BYTE* lpMsg, int* size) // OK
{
	int count = 0;

	PMSG_SERVER_NAME info;

	if(this->CheckJoinServerState() != 0)
	{
		for(std::map<int,SERVER_LIST_INFO>::iterator it=this->m_ServerListInfo.begin();it != this->m_ServerListInfo.end();it++)
		{
			if(it->second.ServerState != 0)
			{
				info.index = it->second.ServerCode;

				strcpy_s(info.Name,it->second.ServerName);

				memcpy(&lpMsg[(*size)],&info,sizeof(info));
				(*size) += sizeof(info);

				count++;
			}
		}
	}

	return count;
}

SERVER_LIST_INFO* CServerList::GetServerListInfo(int ServerCode) // OK
{
	std::map<int,SERVER_LIST_INFO>::iterator it = this->m_ServerListInfo.find(ServerCode);

	if(it == this->m_ServerListInfo.end())
	{
		return 0;
	}
	else
	{
		return &it->second;
	}
}

void CServerList::ServerProtocolCore(BYTE head,BYTE* lpMsg,int size) // OK
{
	switch(head)
	{
		case 0xA1:
			this->GCGameServerLiveRecv((SDHP_GAME_SERVER_LIVE_RECV*)lpMsg);
			break;
		case 0xA2:
			this->JCJoinServerLiveRecv((SDHP_JOIN_SERVER_LIVE_RECV*)lpMsg);
			break;
	}
}

void CServerList::GCGameServerLiveRecv(SDHP_GAME_SERVER_LIVE_RECV* lpMsg) // OK
{
	SERVER_LIST_INFO* lpServerListInfo = this->GetServerListInfo(lpMsg->ServerCode);

	if(lpServerListInfo == 0)
	{
		return;
	}

	if(lpServerListInfo->ServerState == 0)
	{
		LogAdd(LOG_GREEN,"[SocketUDP] GameServer connected [%s] [%s:%d][%d]",lpServerListInfo->ServerName,lpServerListInfo->ServerAddress,lpServerListInfo->ServerPort,lpServerListInfo->ServerCode);
		this->m_GameServersList++;
	}

	lpServerListInfo->ServerState = 1;

	lpServerListInfo->ServerStateTime = GetTickCount();

	lpServerListInfo->UserCount = lpMsg->UserCount;

	lpServerListInfo->UserTotal = lpMsg->UserTotal;
}

void CServerList::JCJoinServerLiveRecv(SDHP_JOIN_SERVER_LIVE_RECV* lpMsg) // OK
{
	if(this->m_JoinServerState == 0)
	{
		LogAdd(LOG_GREEN,"[SocketUDP] JoinServer connected");
	}

	this->m_JoinServerState = 1;

	this->m_JoinServerStateTime = GetTickCount();

	this->m_JoinServerQueueSize = lpMsg->QueueSize;
}
