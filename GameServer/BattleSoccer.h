// BattleSoccer.h: interface for the CBattleSoccer class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Guild.h"
#include "Party.h"
#include "User.h"

#define MAX_BS_TEAM 2
#define MAX_BS_USER 5

enum eBattleSoccerState
{
	BS_STATE_EMPTY = 0,
	BS_STATE_STAND = 1,
	BS_STATE_START = 2,
	BS_STATE_CLEAN = 3,
};

struct BATTLE_SOCCER_USER
{
	int Index;
	bool Master;

	void Reset()
	{
		this->Index = -1;
		this->Master = 0;
	}
};

struct BATTLE_SOCCER_INFO
{
	int Score;
	int PartyNumber;
	GUILD_INFO* Guild;
	BATTLE_SOCCER_USER User[MAX_BS_USER];

	void Reset()
	{
		this->Guild = 0;
		this->PartyNumber = 0;
		this->Score = 0;
			
		for(int n=0;n < MAX_BS_USER;n++)
		{
			this->User->Reset();
		}
	}
};

class CBattleSoccer
{
public:
	CBattleSoccer();
	virtual ~CBattleSoccer();
	void MainProc();
	void ProcState_EMPTY();
	void ProcState_STAND();
	void ProcState_START();
	void ProcState_CLEAN();
	void SetState(int state);
	void SetState_EMPTY();
	void SetState_STAND();
	void SetState_START();
	void SetState_CLEAN();
	int GetState();
	void SetBallIndex(int aIndex);
	void SetSoccerInfo(GUILD_INFO* lpGuild1,GUILD_INFO* lpGuild2);
	bool AddUser(int team,int aIndex);
	bool DelUser(int team,int aIndex);
	BATTLE_SOCCER_USER* GetUser(int aIndex);
	void CleanUser();
	void ClearUser();
	void CheckUser();
	int GetUserCount(int team);
	bool CheckMapPos(LPOBJ lpObj);
	void GetMapRandomPos(short* ox,short* oy);
	void MonsterProc(LPOBJ lpObj);
private:
	int m_BallSoccerIndex;
	int m_State;
	int m_RemainTime;
	int m_TargetTime;
	int m_TickCount;
	BATTLE_SOCCER_INFO m_BattleSoccerInfo[MAX_BS_TEAM];
};

extern CBattleSoccer gBattleSoccer;

static const BYTE gBattleSoccerGround[4] = {54,141,70,180};

static const BYTE gBattleSoccerZones[MAX_BS_TEAM][4] = {{60,181,64,182},{60,139,64,140}};