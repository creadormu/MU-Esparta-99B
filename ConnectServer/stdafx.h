#pragma once

#define WIN32_LEAN_AND_MEAN

#define _WIN32_WINNT _WIN32_WINNT_WINXP

#define PROTECT_START _asm _emit 0xEB _asm _emit 0x0E _asm _emit 0x14 _asm _emit 0x28 _asm _emit 0xCD _asm _emit 0xDB _asm _emit 0x50 _asm _emit 0x63 _asm _emit 0xAF _asm _emit 0x22 _asm _emit 0x75 _asm _emit 0xEE _asm _emit 0xB5 _asm _emit 0xD8 _asm _emit 0x43 _asm _emit 0x72
#define PROTECT_FINAL _asm _emit 0xEB _asm _emit 0x0E _asm _emit 0x37 _asm _emit 0xF2 _asm _emit 0xCD _asm _emit 0xDB _asm _emit 0x50 _asm _emit 0x63 _asm _emit 0xAF _asm _emit 0x22 _asm _emit 0x75 _asm _emit 0xEE _asm _emit 0xB5 _asm _emit 0xD8 _asm _emit 0x85 _asm _emit 0x61
#define CRASH_APPLICATION_MACRO _asm _emit 0xFF _asm _emit 0x25 _asm _emit 0x00 _asm _emit 0x00 _asm _emit 0x00 _asm _emit 0x00

#define CONNECTSERVER_VERSION "[ 2.0.4 ] ConnectServer (EX%03d-%s) [%s]"

#ifndef CONNECTSERVER_UPDATE
#define CONNECTSERVER_UPDATE 803
#endif

#ifndef CONNECTSERVER_LANGUAGE
#define CONNECTSERVER_LANGUAGE 1
#endif

#ifndef PROTECT_STATE
#define PROTECT_STATE 0
#endif

#if(CONNECTSERVER_LANGUAGE==0)
#define PROTOCOL_NAME "KOR"
#elif(CONNECTSERVER_LANGUAGE==1)
#define PROTOCOL_NAME "ENG"
#elif(CONNECTSERVER_LANGUAGE==2)
#define PROTOCOL_NAME "JPN"
#elif(CONNECTSERVER_LANGUAGE==3)
#define PROTOCOL_NAME "CHS"
#elif(CONNECTSERVER_LANGUAGE==4) 
#define PROTOCOL_NAME "TAI"
#elif(CONNECTSERVER_LANGUAGE==5) 
#define PROTOCOL_NAME "PHI"
#endif

// System Include
#include <windows.h>
#include <winsock2.h>
#include <iostream>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <time.h>
#include <math.h>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <Rpc.h>
#include <dbghelp.h>
#include <Psapi.h>
#include <CommCtrl.h>

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"Rpcrt4.lib")
#pragma comment(lib,"dbghelp.lib")
#pragma comment(lib,"Psapi.lib")

extern int MaxConnectionPerIP;
extern int MaxPacketPerSecond;
extern int MaxConnectionIdle;
extern char CustomerName[32];
extern WORD CustomerLicenseId;

typedef unsigned __int64 QWORD;