// CannonTower.cpp: implementation of the CCannonTower class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CannonTower.h"
#include "SkillManager.h"
#include "Util.h"
#include "Viewport.h"

CCannonTower gCannonTower;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCannonTower::CCannonTower() // OK
{

}

CCannonTower::~CCannonTower() // OK
{

}

void CCannonTower::FindTarget(int aIndex) // OK
{
	if((GetLargeRand()%2) != 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	BYTE send[256];

	PMSG_MULTI_SKILL_ATTACK_RECV pMsg;

	pMsg.header.set(PROTOCOL_CODE4,sizeof(pMsg));

	int size = sizeof(pMsg);

	#if(GAMESERVER_UPDATE>=701)

	pMsg.skillH = SET_NUMBERHB(0);

	pMsg.skillL = SET_NUMBERLB(0);

	#elif(GAMESERVER_UPDATE>=401)

	pMsg.skill[0] = SET_NUMBERHB(0);

	pMsg.skill[1] = SET_NUMBERLB(0);

	#else

	pMsg.skill = 0;

	#endif

	pMsg.x = (BYTE)lpObj->X;

	pMsg.y = (BYTE)lpObj->Y;

	pMsg.serial = 0;

	pMsg.count = 0;

	PMSG_MULTI_SKILL_ATTACK info;

	for(int n=0;n < MAX_VIEWPORT;n++)
	{
		if(lpObj->VpPlayer2[n].state != VIEWPORT_SEND)
		{
			continue;
		}

		if(lpObj->VpPlayer2[n].type != OBJECT_USER)
		{
			continue;
		}

		if(OBJECT_RANGE(lpObj->VpPlayer2[n].index) == 0)
		{
			continue;
		}

		LPOBJ lpTarget = &gObj[lpObj->VpPlayer2[n].index];

		if(lpTarget->CsJoinSide == 1)
		{
			continue;
		}

		if(gObjCalcDistance(lpObj,lpTarget) >= 7)
		{
			continue;
		}

		#if(GAMESERVER_UPDATE>=701)

		info.indexH = SET_NUMBERHB(lpTarget->Index);

		info.indexL = SET_NUMBERLB(lpTarget->Index);

		#else

		info.index[0] = SET_NUMBERHB(lpTarget->Index);

		info.index[1] = SET_NUMBERLB(lpTarget->Index);

		#endif

		info.MagicKey = 0;

		memcpy(&send[size],&info,sizeof(info));
		size += sizeof(info);

		pMsg.count++;
	}

	if(pMsg.count > 0)
	{
		pMsg.header.size = size;

		memcpy(send,&pMsg,sizeof(pMsg));

		gSkillManager.CGMultiSkillAttackRecv((PMSG_MULTI_SKILL_ATTACK_RECV*)send,aIndex,1);

		gSkillManager.GCDurationSkillAttackSend(lpObj,SKILL_MONSTER_AREA_ATTACK,(BYTE)lpObj->X,(BYTE)lpObj->Y,0);
	}
}