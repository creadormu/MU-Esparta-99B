// LifeStone.cpp: implementation of the CLifeStone class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LifeStone.h"
#include "CastleSiege.h"
#include "CastleSiegeSync.h"
#include "GuildManager.h"
#include "Map.h"
#include "Message.h"
#include "Monster.h"
#include "MonsterManager.h"
#include "Notice.h"
#include "Protocol.h"
#include "Util.h"

CLifeStone gLifeStone;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLifeStone::CLifeStone() // OK
{

}

CLifeStone::~CLifeStone() // OK
{

}

bool CLifeStone::CreateLifeStone(int aIndex) // OK
{
	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	int px = lpObj->X;
	int py = lpObj->Y;

	if(lpObj->GuildRank != GUILD_RANK_MASTER)
	{
		return 0;
	}

	if(lpObj->Map != MAP_CASTLE_SIEGE || gCastleSiege.GetCastleState() != CASTLESIEGE_STATE_STARTSIEGE)
	{
		gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(122));
		return 0;
	}

	if(lpObj->CsJoinSide < 2)
	{
		gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(123));
		return 0;
	}

	if(lpObj->Guild->LifeStone != 0)
	{
		gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(124));
		return 0;
	}

	if(px > 150 && px < 210 && py > 175 && py < 230)
	{
		gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(125));
		return 0;
	}

	int index = gObjAddMonster(lpObj->Map);

	if(OBJECT_RANGE(index) == 0)
	{
		gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(127));
		return 0;
	}

	LPOBJ lpMonster = &gObj[index];

	lpMonster->PosNum = -1;
	lpMonster->X = px;
	lpMonster->Y = py;
	lpMonster->MTX = px;
	lpMonster->MTY = py;
	lpMonster->TX = px;
	lpMonster->TY = py;
	lpMonster->OldX = px;
	lpMonster->OldY = py;
	lpMonster->StartX = px;
	lpMonster->StartY = py;
	lpMonster->Dir = 1;
	lpMonster->Map = lpObj->Map;

	if(gObjSetMonster(index,278) == 0)
	{
		gObjDel(index);
		return 0;
	}

	lpMonster->MaxRegenTime = 1000;
	lpMonster->RegenTime = GetTickCount();
	lpMonster->CreationState = 0;
	lpMonster->CreatedActivationTime = 0;
	lpMonster->CsNpcType = OBJECT_NPC;
	lpMonster->CsJoinSide = lpObj->CsJoinSide;
	lpMonster->Guild = lpObj->Guild;

	lpObj->Guild->LifeStone = lpMonster;
	lpObj->LifeStoneCount++;

	gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(126));

	return 1;
}

void CLifeStone::DeleteLifeStone(int aIndex) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Guild != 0)
	{
		lpObj->Guild->LifeStone = 0;
	}
}

bool CLifeStone::SetReSpawnUserXY(int aIndex) // OK
{
	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Map != MAP_CASTLE_SIEGE)
	{
		return 0;
	}

	if(lpObj->Guild == 0)
	{
		return 0;
	}

	if(lpObj->Guild->LifeStone == 0)
	{
		return 0;
	}

	LPOBJ lpTarget = lpObj->Guild->LifeStone;

	if(lpTarget->CreatedActivationTime < 60)
	{
		return 0;
	}

	lpObj->Map = MAP_CASTLE_SIEGE;
	lpObj->X = lpTarget->X;
	lpObj->Y = lpTarget->Y;
	
	return 1;
}

void CLifeStone::FindTarget(int aIndex) // OK
{
	if(gObjIsConnected(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	lpObj->CreatedActivationTime++;

	BYTE OldCreationState = lpObj->CreationState;

	lpObj->CreationState = (lpObj->CreatedActivationTime<60) ? (lpObj->CreatedActivationTime / 12) : 5;

	if(OldCreationState != lpObj->CreationState)
	{
		GCCreationStateSend(aIndex,lpObj->CreationState);
	}

	if(lpObj->CreationState < 5)
	{
		return;
	}

	if(lpObj->VPCount < 1)
	{
		return;
	}

	for(int n=0;n < MAX_VIEWPORT;n++)
	{
		if(lpObj->VpPlayer[n].type != OBJECT_USER)
		{
			continue;
		}

		if(OBJECT_RANGE(lpObj->VpPlayer[n].index) == 0)
		{
			continue;
		}

		LPOBJ lpTarget = &gObj[lpObj->VpPlayer[n].index];

		if(lpTarget->Live == 0)
		{
			continue;
		}

		if(lpObj->CsJoinSide != lpTarget->CsJoinSide)
		{
			continue;
		}

		if(gObjCalcDistance(lpObj,lpTarget) > 3)
		{
			continue;
		}

		float MaxLife = (lpTarget->MaxLife+lpTarget->AddLife);

		if(lpTarget->Life < MaxLife)
		{
			lpTarget->Life += (MaxLife/100);

			lpTarget->Life = (lpTarget->Life > MaxLife) ? MaxLife : lpTarget->Life;

			GCLifeSend(lpTarget->Index,0xFF,(int)lpTarget->Life,lpTarget->Shield);
		}

		float MaxMana = (lpTarget->MaxMana+lpTarget->AddMana);

		if(lpTarget->Mana < MaxMana)
		{
			lpTarget->Mana += (MaxMana/100);

			lpTarget->Mana = (lpTarget->Mana > MaxMana) ? MaxMana : lpTarget->Mana;

			GCManaSend(lpTarget->Index,0xFF,(int)lpTarget->Mana,lpTarget->BP);
		}

		int MaxBP = (lpTarget->MaxBP+lpTarget->AddBP);

		if(lpTarget->BP < MaxBP)
		{
			lpTarget->BP += (MaxBP/100);
			
			lpTarget->BP = (lpTarget->BP > MaxBP) ? MaxBP : lpTarget->BP;

			GCManaSend(lpTarget->Index,0xFF,(int)lpTarget->Mana,lpTarget->BP);
		}
	}
}
