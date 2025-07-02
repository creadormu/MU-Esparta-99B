// MemScript.cpp: implementation of the CMemScript class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Mercenary.h"
#include "CastleSiege.h"
#include "CastleSiegeSync.h"
#include "EffectManager.h"
#include "GuildManager.h"
#include "Map.h"
#include "Message.h"
#include "Monster.h"
#include "MonsterManager.h"
#include "Notice.h"
#include "Util.h"
#include "Viewport.h"

CMercenary gMercenary;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMercenary::CMercenary() // OK
{
	this->m_MercenaryCount = 0;
}

CMercenary::~CMercenary() // OK
{

}

bool CMercenary::CreateMercenary(int aIndex,int MonsterClass,int x, int y) // OK
{
	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	int px = lpObj->X;
	int py = lpObj->Y;

	if(lpObj->Map != MAP_CASTLE_SIEGE || gCastleSiege.GetCastleState() != CASTLESIEGE_STATE_STARTSIEGE)
	{
		gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(128));
		return 0;
	}

	if(MonsterClass == 286 || MonsterClass == 287)
	{
		if(lpObj->CsJoinSide != 1)
		{
			gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(129));
			return 0;
		}
		if(lpObj->GuildRank != GUILD_RANK_OFFICE_MASTER && lpObj->GuildRank != GUILD_RANK_MASTER)
		{
			gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(130));
			return 0;
		}
	}

	if(this->m_MercenaryCount > 100)
	{
		gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(131));
		return 0;
	}

	int index = gObjAddMonster(lpObj->Map);

	if(OBJECT_RANGE(index) == 0)
	{
		gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(133));
		return 0;
	}

	MONSTER_INFO* lpInfo = gMonsterManager.GetInfo(MonsterClass);

	if(lpInfo == 0)
	{
		gObjDel(index);
		return 0;
	}

	gObjSetMonster(index,MonsterClass);

	LPOBJ lpMonster = &gObj[index];

	lpMonster->Live = 1;
	lpMonster->Life = (float)lpMonster->Life;
	lpMonster->MaxLife = (float)lpMonster->Life;
	lpMonster->PosNum = -1;
	lpMonster->X = x;
	lpMonster->Y = y;
	lpMonster->MTX = x;
	lpMonster->MTY = y;
	lpMonster->TX = x;
	lpMonster->TY = y;
	lpMonster->OldX = x;
	lpMonster->OldY = y;
	lpMonster->StartX = x;
	lpMonster->StartY = y;
	lpMonster->Map = lpObj->Map;
	lpMonster->MoveRange = 0;
	lpMonster->Level = lpMonster->Level;
	lpMonster->Type = OBJECT_MONSTER;
	lpMonster->MaxRegenTime = 1000;
	lpMonster->Dir = 1;
	lpMonster->RegenTime = GetTickCount();
	lpMonster->Attribute = ATTRIBUTE_NONE;
	lpMonster->DieRegen = 0;
	lpMonster->CsNpcType = 2;
	lpMonster->CsJoinSide = 1;

	gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(132));

	this->m_MercenaryCount++;

	return 1;
}

void CMercenary::DeleteMercenary(int aIndex) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return;
	}

	if((--this->m_MercenaryCount) < 0)
	{
		this->m_MercenaryCount = 0;
	}
}

void CMercenary::FindTarget(int aIndex) // OK
{
	if(gObjIsConnected(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	int IndexTable[MAX_VIEWPORT];
	int IndexCount = 0;

	for(int n=0;n < MAX_VIEWPORT;n++)
	{
		if(lpObj->VpPlayer2[n].state == VIEWPORT_NONE || OBJECT_RANGE(lpObj->VpPlayer2[n].index) == 0 || lpObj->VpPlayer2[n].type != OBJECT_USER)
		{
			continue;
		}

		if(gObj[lpObj->VpPlayer2[n].index].CsJoinSide == lpObj->CsJoinSide)
		{
			continue;
		}

		if((gObj[lpObj->VpPlayer2[n].index].Authority & 32) != 0 || gEffectManager.CheckEffect(&gObj[lpObj->VpPlayer2[n].index],EFFECT_INVISIBILITY) != 0)
		{
			continue;
		}

		if(gViewport.CheckViewportObjectPosition(lpObj->Index,gObj[lpObj->VpPlayer2[n].index].Map,gObj[lpObj->VpPlayer2[n].index].X,gObj[lpObj->VpPlayer2[n].index].Y,(lpObj->Class == 286)?5:3) == 0)
		{
			continue;
		}

		IndexTable[IndexCount++] = lpObj->VpPlayer2[n].index;
	}

	lpObj->TargetNumber = ((IndexCount==0)?-1:IndexTable[GetLargeRand()%IndexCount]);

	if(OBJECT_RANGE(lpObj->TargetNumber) != 0)
	{
		lpObj->ActionState.Attack = 1;
		lpObj->NextActionTime = lpObj->PhysiSpeed;
	}
	else
	{
		lpObj->NextActionTime = lpObj->MoveSpeed;
	}
}
