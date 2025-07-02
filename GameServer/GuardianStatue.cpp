// GuardianStatue.cpp: implementation of the CGuardianStatue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GuardianStatue.h"
#include "Protocol.h"

CGuardianStatue gGuardianStatue;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGuardianStatue::CGuardianStatue() // OK
{

}

CGuardianStatue::~CGuardianStatue() // OK
{

}

void CGuardianStatue::FindTarget(int aIndex) // OK
{
	if(gObjIsConnected(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

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

		if(lpTarget->CsJoinSide != 1)
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