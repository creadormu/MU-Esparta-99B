// BattleSoccer.cpp: implementation of the CBattleSoccer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleSoccer.h"
#include "Guild.h"
#include "GuildManager.h"
#include "Map.h"
#include "MapManager.h"
#include "Monster.h"
#include "Party.h"
#include "Util.h"

CBattleSoccer gBattleSoccer;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBattleSoccer::CBattleSoccer() // OK
{
	this->m_State = BS_STATE_EMPTY;
	this->m_RemainTime = 0;
	this->m_TargetTime = 0;
	this->m_TickCount = GetTickCount();

	for(int n=0;n < MAX_BS_TEAM;n++)
	{
		this->m_BattleSoccerInfo[n].Reset();
	}

	this->m_BallSoccerIndex = -1;

	this->CleanUser();
}

CBattleSoccer::~CBattleSoccer() // OK
{
	
}

void CBattleSoccer::MainProc() // OK
{
	if((GetTickCount()-this->m_TickCount) >= 1000)
	{
		this->m_TickCount = GetTickCount();

		this->m_RemainTime = (int)difftime(this->m_TargetTime,time(0));

		switch(this->m_State)
		{
			case BS_STATE_EMPTY:
				this->ProcState_EMPTY();
				break;
			case BS_STATE_STAND:
				this->ProcState_STAND();
				break;
			case BS_STATE_START:
				this->ProcState_START();
				break;
			case BS_STATE_CLEAN:
				this->ProcState_CLEAN();
				break;
		}
	}
}

void CBattleSoccer::ProcState_EMPTY() // OK
{

}

void CBattleSoccer::ProcState_STAND() // OK
{
	this->CheckUser();

	if(this->GetUserCount(GUILD_WAR_TEAM_BLUE) == 0 || this->GetUserCount(GUILD_WAR_TEAM_RED) == 0)
	{
		this->SetState(BS_STATE_CLEAN);
		return;
	}

	if(this->m_RemainTime <= 0)
	{
		this->SetState(BS_STATE_START);
	}
}

void CBattleSoccer::ProcState_START() // OK
{
	this->CheckUser();

	if(this->GetUserCount(GUILD_WAR_TEAM_BLUE) == 0 || this->GetUserCount(GUILD_WAR_TEAM_RED) == 0)
	{
		this->SetState(BS_STATE_CLEAN);
		return;
	}

	if(this->m_RemainTime <= 0)
	{
		this->SetState(BS_STATE_CLEAN);
	}
}

void CBattleSoccer::ProcState_CLEAN() // OK
{
	this->CheckUser();

	if(this->m_RemainTime <= 0)
	{
		this->SetState(BS_STATE_EMPTY);
	}
}

void CBattleSoccer::SetState(int state) // OK
{
	if(OBJECT_RANGE(this->m_BallSoccerIndex) != 0)
	{
		gObjMonsterRegen(&gObj[this->m_BallSoccerIndex]);
	}

	switch((this->m_State=state))
	{
		case BS_STATE_EMPTY:
			this->SetState_EMPTY();
			break;
		case BS_STATE_STAND:
			this->SetState_STAND();
			break;
		case BS_STATE_START:
			this->SetState_START();
			break;
		case BS_STATE_CLEAN:
			this->SetState_CLEAN();
			break;
	}
}

void CBattleSoccer::SetState_EMPTY() // OK
{
	this->m_RemainTime = 0;
	this->m_TargetTime = 0;
	this->m_TickCount = GetTickCount();

	this->ClearUser();

	LogAdd(LOG_BLACK,"[Battle Soccer] SetState EMPTY");
}

void CBattleSoccer::SetState_STAND() // OK
{
	this->m_RemainTime = 10;

	this->m_TargetTime = (int)(time(0)+this->m_RemainTime);

	LogAdd(LOG_BLACK,"[Battle Soccer] SetState STAND");
}

void CBattleSoccer::SetState_START() // OK
{
	this->m_RemainTime = 900;

	this->m_TargetTime = (int)(time(0)+this->m_RemainTime);

	LogAdd(LOG_BLACK,"[Battle Soccer] SetState START [%s] vs [%s]",this->m_BattleSoccerInfo[0].Guild->Name,this->m_BattleSoccerInfo[1].Guild->Name);
}

void CBattleSoccer::SetState_CLEAN() // OK
{
	GUILD_INFO* lpGuild1 = this->m_BattleSoccerInfo[0].Guild;

	GUILD_INFO* lpGuild2 = this->m_BattleSoccerInfo[1].Guild;

	gGuild.gObjGuildWarEndSend(lpGuild1,lpGuild2,1,0);
	
	gGuild.gObjGuildWarEnd(lpGuild1,lpGuild2);

	this->m_RemainTime = 10;

	this->m_TargetTime = (int)(time(0)+this->m_RemainTime);

	LogAdd(LOG_BLACK,"[Battle Soccer] SetState CLEAN");
}

int CBattleSoccer::GetState() // OK
{
	return this->m_State;
}

void CBattleSoccer::SetBallIndex(int aIndex) // OK
{
	this->m_BallSoccerIndex = aIndex;
}

void CBattleSoccer::SetSoccerInfo(GUILD_INFO* lpGuild1,GUILD_INFO* lpGuild2) // OK
{
	this->m_BattleSoccerInfo[GUILD_WAR_TEAM_BLUE].Guild = lpGuild1;

	this->m_BattleSoccerInfo[GUILD_WAR_TEAM_RED].Guild = lpGuild2;

	for(int n=0;n < MAX_GUILD_MEMBER;n++)
	{
		if(lpGuild1->Member[n].IsValid() != 0)
		{
			this->AddUser(GUILD_WAR_TEAM_BLUE,lpGuild1->Member[n].Index);
		}

		if(lpGuild2->Member[n].IsValid() != 0)
		{
			this->AddUser(GUILD_WAR_TEAM_RED,lpGuild2->Member[n].Index);
		}
	}

	this->SetState(BS_STATE_STAND);
}

bool CBattleSoccer::AddUser(int team,int aIndex) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	if(this->GetUser(aIndex) != 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	BATTLE_SOCCER_INFO* lpInfo = &this->m_BattleSoccerInfo[team];

	for(int n=0;n < MAX_BS_USER;n++)
	{
		if(OBJECT_RANGE(lpInfo->User[n].Index) != 0)
		{
			continue;
		}

		lpInfo->User[n].Index = aIndex;
		lpInfo->User[n].Master = (lpObj->GuildRank == GUILD_RANK_MASTER)?1:0;
		
		return 1;
	}

	return 0;
}

bool CBattleSoccer::DelUser(int team,int aIndex) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	BATTLE_SOCCER_USER* lpUser = this->GetUser(aIndex);

	if(lpUser == 0)
	{
		return 0;
	}

	BATTLE_SOCCER_INFO* lpInfo = &this->m_BattleSoccerInfo[team];

	if(OBJECT_RANGE(lpInfo->PartyNumber) != 0)
	{
		gParty.DelMember(lpInfo->PartyNumber,aIndex);
		lpInfo->PartyNumber = ((gParty.IsParty(lpInfo->PartyNumber)==0)?-1:lpInfo->PartyNumber);
	}

	lpUser->Reset();

	return 1;
}

BATTLE_SOCCER_USER* CBattleSoccer::GetUser(int aIndex) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	for(int n=0;n < MAX_BS_TEAM;n++)
	{
		for(int i=0;i < MAX_BS_USER;i++)
		{
			if(this->m_BattleSoccerInfo[n].User[i].Index == aIndex)
			{
				return &this->m_BattleSoccerInfo[n].User[i];
			}
		}
	}

	return 0;
}

void CBattleSoccer::CleanUser() // OK
{
	for(int n=0;n < MAX_BS_TEAM;n++)
	{
		for(int i=0;i < MAX_BS_USER;i++)
		{
			this->m_BattleSoccerInfo[n].User[i].Reset();
		}
	}
}

void CBattleSoccer::ClearUser() // OK
{
	for(int n=0;n < MAX_BS_TEAM;n++)
	{
		for(int i=0;i < MAX_BS_USER;i++)
		{
			if(OBJECT_RANGE(this->m_BattleSoccerInfo[n].User[i].Index) == 0)
			{
				continue;
			}

			gObjMoveGate(this->m_BattleSoccerInfo[n].User[i].Index,17);

			this->m_BattleSoccerInfo[n].User[i].Reset();
		}
	}
}

void CBattleSoccer::CheckUser() // OK
{
	for(int n=0;n < MAX_BS_TEAM;n++)
	{
		for(int i=0;i < MAX_BS_USER;i++)
		{
			if(OBJECT_RANGE(this->m_BattleSoccerInfo[n].User[i].Index) == 0)
			{
				continue;
			}

			if(gObjIsConnected(this->m_BattleSoccerInfo[n].User[i].Index) == 0)
			{
				this->DelUser(n,this->m_BattleSoccerInfo[n].User[i].Index);
				continue;
			}

			if(gObj[this->m_BattleSoccerInfo[n].User[i].Index].Map != MAP_ARENA)
			{
				this->DelUser(n,this->m_BattleSoccerInfo[n].User[i].Index);
				continue;
			}

			int px = gObj[this->m_BattleSoccerInfo[n].User[i].Index].X;

			int py = gObj[this->m_BattleSoccerInfo[n].User[i].Index].Y;

			if(px < gBattleSoccerZones[n][0] && px > gBattleSoccerZones[n][2] && py < gBattleSoccerZones[n][1] && py > gBattleSoccerZones[n][3])
			{
				this->DelUser(n,this->m_BattleSoccerInfo[n].User[i].Index);
				continue;
			}
		}
	}
}

int CBattleSoccer::GetUserCount(int team) // OK
{
	int count = 0;

	for(int i=0;i < MAX_BS_USER;i++)
	{
		if(OBJECT_RANGE(this->m_BattleSoccerInfo[team].User[i].Index) != 0)
		{
			count++;
		}
	}

	return count;
}

bool CBattleSoccer::CheckMapPos(LPOBJ lpObj) // OK
{
	if(lpObj->X >= gBattleSoccerGround[0] && lpObj->X <= gBattleSoccerGround[2] && lpObj->Y >= gBattleSoccerGround[1] && lpObj->Y <= gBattleSoccerGround[3])
	{
		if(this->m_State == BS_STATE_EMPTY)
		{
			return 1;
		}

		BATTLE_SOCCER_USER* lpUser = this->GetUser(lpObj->Index);

		if(lpUser == 0)
		{
			return 1;
		}
	}

	return 0;
}

void CBattleSoccer::GetMapRandomPos(short* ox, short* oy) // OK
{
	int px = (gBattleSoccerGround[2]-gBattleSoccerGround[0]);

	int py = (gBattleSoccerGround[3]-gBattleSoccerGround[1]);

	(*ox) = (short)gBattleSoccerGround[0]+(short)(GetLargeRand()%((px<1)?1:px));

	(*oy) = (short)gBattleSoccerGround[1]+(short)(GetLargeRand()%((py<1)?1:py));
}

void CBattleSoccer::MonsterProc(LPOBJ lpObj) // OK
{
	if(this->m_State != BS_STATE_START)
	{
		return;
	}

	for(int n=0;n < MAX_BS_TEAM;n++)
	{
		BATTLE_SOCCER_INFO* lpInfo1 = &this->m_BattleSoccerInfo[n];

		BATTLE_SOCCER_INFO* lpInfo2 = &this->m_BattleSoccerInfo[n^1];

		if(lpObj->X >= gBattleSoccerZones[n][0] && lpObj->X <= gBattleSoccerZones[n][2] && lpObj->Y >= gBattleSoccerZones[n][1] && lpObj->Y <= gBattleSoccerZones[n][3])
		{
			gObjMonsterRegen(lpObj);

			if(gGuild.gObjGuildWarProc(lpInfo1->Guild,lpInfo2->Guild,20) == 1)
			{
				if(lpInfo1->Guild->WarType == GUILD_WAR_TYPE_SOCCER)
				{
					this->SetState(BS_STATE_CLEAN);
					gGuild.gObjGuildWarEndSend(lpInfo1->Guild,lpInfo2->Guild,1,0);
					gGuild.gObjGuildWarEnd(lpInfo1->Guild,lpInfo2->Guild);
				}
			}
		}
	}
}