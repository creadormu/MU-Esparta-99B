#pragma once

#include "ServerDisplayer.h"

void PacketArgumentEncrypt(char* out_buff,char* in_buff,int size);
void ErrorMessageBox(char* message,...);
void LogAdd(eLogColor color,char* text,...);
void ConnectServerTimeoutProc();
int GetUserCount();
int GetFreeClientIndex();
int SearchFreeClientIndex(int* index,int MinIndex,int MaxIndex,DWORD MinTime);
char* CharReverse(char* text);

extern int gClientCount;
