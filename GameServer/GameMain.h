#pragma once

#include "Connection.h"
#include "Protocol.h"

struct SDHP_GAME_SERVER_LIVE_SEND
{
	PBMSG_HEAD header; // C1:A1
	DWORD ServerCode;
	DWORD UserCount;
	DWORD UserTotal;
};

void GameMainInit(HWND hwnd);
void ConnectServerInfoSend();
bool JoinServerConnect(DWORD wMsg);
bool DataServerConnect(DWORD wMsg);
bool JoinServerReconnect(HWND hwnd,DWORD wMsg);
bool DataServerReconnect(HWND hwnd,DWORD wMsg);
void JoinServerMsgProc(WPARAM wParam,LPARAM lParam);
void DataServerMsgProc(WPARAM wParam,LPARAM lParam);
void CALLBACK QueueTimerCallback(PVOID lpParameter,BOOLEAN TimerOrWaitFired);

extern CConnection gJoinServerConnection;
extern CConnection gDataServerConnection;
