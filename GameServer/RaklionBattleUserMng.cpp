// Raklion.cpp: implementation of the CRaklion class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RaklionBattleUserMng.h"
#include "Map.h"
#include "Raklion.h"
#include "RaklionUtil.h"
#include "User.h"
#include "Util.h"

CRaklionBattleUserMng gRaklionBattleUserMng;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRaklionBattleUserMng::CRaklionBattleUserMng() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	this->ResetAllData();

	#endif
}

CRaklionBattleUserMng::~CRaklionBattleUserMng() // OK
{

}

void CRaklionBattleUserMng::ResetAllData() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	this->m_UserInfo.clear();

	#endif
}

void CRaklionBattleUserMng::AddUserData(int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=401)

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	this->m_UserInfo.push_back(aIndex);

	gRaklionUtil.NotifyRaklionState(aIndex,gRaklion.GetRaklionState(),gRaklion.GetRaklionDetailState());

	#endif
}

void CRaklionBattleUserMng::DelUserData(int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=401)

	if(OBJECT_RANGE(aIndex) == 0)
	{
		return;
	}

	for(int n=0;n < this->GetBattleUserCount();n++)
	{
		if(this->m_UserInfo[n] == aIndex)
		{
			this->m_UserInfo.erase((this->m_UserInfo.begin()+n));
			return;
		}
	}

	#endif
}

void CRaklionBattleUserMng::CheckUserState() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	for(int n=0;n < this->GetBattleUserCount();n++)
	{
		if(gObjIsConnected(this->m_UserInfo[n]) == 0)
		{
			this->DelUserData(this->m_UserInfo[n]);
			continue;
		}

		if(gObj[this->m_UserInfo[n]].Map != MAP_RAKLION2)
		{
			this->DelUserData(this->m_UserInfo[n]);
			continue;
		}
	}

	#endif
}

void CRaklionBattleUserMng::CheckUserOnRaklionBossMap() // OK
{
	#if(GAMESERVER_UPDATE>=401)

	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnectedGP(n) != 0 && gObj[n].Map == MAP_RAKLION2 && (gObj[n].Authority & 32) == 0)
		{
			if(this->IsBattleUser(n) == 0)
			{
				gObjMoveGate(n,286);
			}
		}
	}

	#endif
}

bool CRaklionBattleUserMng::IsBattleUser(int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=401)

	for(int n=0;n < this->GetBattleUserCount();n++)
	{
		if(this->m_UserInfo[n] == aIndex)
		{
			return 1;
		}
	}

	#endif

	return 0;
}

bool CRaklionBattleUserMng::CheckBattleUsers() // OK
{
	return this->m_UserInfo.empty();
}

int CRaklionBattleUserMng::GetBattleUserCount() // OK
{
	return this->m_UserInfo.size();
}

int CRaklionBattleUserMng::GetBattleUserIndex(int index) // OK
{
	return this->m_UserInfo[index];
}
