
// Kalima.cpp: implementation of the CKalima class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Kalima.h"
#include "CrywolfSync.h"
#include "EventEntryLevel.h"
#include "Map.h"
#include "MemScript.h"
#include "Monster.h"
#include "ObjectManager.h"
#include "Party.h"
#include "Util.h"
#include "Viewport.h"

CKalima gKalima;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKalima::CKalima() // OK
{
	
}

CKalima::~CKalima() // OK
{
	
}

void CKalima::Load(char* path) // OK
{
	CMemScript* lpMemScript = new CMemScript;

	if(lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR,path);
		return;
	}

	if(lpMemScript->SetBuffer(path) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	memset(this->m_KalimaPortal,0,sizeof(sizeof(this->m_KalimaPortal)));

	try
	{
		while (true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			if(strcmp("end",lpMemScript->GetString()) == 0)
			{
				break;
			}

			int level = lpMemScript->GetNumber();

			int value = lpMemScript->GetAsNumber();

			if(CHECK_RANGE(level,6) != 0)
			{
				this->m_KalimaPortal[level] = value;
			}
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CKalima::MainProc(LPOBJ lpObj) // OK
{
	if(lpObj->Live == 0 || lpObj->State != OBJECT_PLAYING)
	{
		return;
	}

	if(gObjIsConnectedGP(lpObj->SummonIndex) == 0)
	{
		GCUserDieSend(lpObj,lpObj->Index,0,0);
		gObjDel(lpObj->Index);
		return;
	}

	if((GetTickCount()-lpObj->RegenTime) < 2000)
	{
		return;
	}

	if(gObj[lpObj->SummonIndex].DieRegen != 0)
	{
		this->DeleteKalimaPortal(lpObj->SummonIndex);
		return;
	}

	if(lpObj->KalimaGateEnterCount >= 5)
	{
		this->DeleteKalimaPortal(lpObj->SummonIndex);
		return;
	}

	if((GetTickCount()-lpObj->RegenTime) >= 180000)
	{
		this->DeleteKalimaPortal(lpObj->SummonIndex);
		return;
	}

	for(int n=0;n < MAX_VIEWPORT;n++)
	{
		if(lpObj->VpPlayer2[n].state == VIEWPORT_NONE || OBJECT_RANGE(lpObj->VpPlayer2[n].index) == 0 || lpObj->VpPlayer2[n].type != OBJECT_USER || gObj[lpObj->VpPlayer2[n].index].Live == 0)
		{
			continue;
		}

		if(gViewport.CheckViewportObjectPosition(lpObj->Index,gObj[lpObj->VpPlayer2[n].index].Map,gObj[lpObj->VpPlayer2[n].index].X,gObj[lpObj->VpPlayer2[n].index].Y,2) == 0)
		{
			continue;
		}

		if(lpObj->SummonIndex != lpObj->VpPlayer2[n].index)
		{
			if(OBJECT_RANGE(gObj[lpObj->VpPlayer2[n].index].PartyNumber) == 0)
			{
				continue;
			}

			if(gObj[lpObj->SummonIndex].PartyNumber != gObj[lpObj->VpPlayer2[n].index].PartyNumber)
			{
				continue;
			}
		}

		switch(lpObj->KalimaGateLevel)
		{
			case 0:
				gObjMoveGate(lpObj->VpPlayer2[n].index,88);
				break;
			case 1:
				gObjMoveGate(lpObj->VpPlayer2[n].index,89);
				break;
			case 2:
				gObjMoveGate(lpObj->VpPlayer2[n].index,90);
				break;
			case 3:
				gObjMoveGate(lpObj->VpPlayer2[n].index,91);
				break;
			case 4:
				gObjMoveGate(lpObj->VpPlayer2[n].index,92);
				break;
			case 5:
				gObjMoveGate(lpObj->VpPlayer2[n].index,93);
				break;
			case 6:
				gObjMoveGate(lpObj->VpPlayer2[n].index,116);
				break;
		}

		if((lpObj->KalimaGateEnterCount++) >= 5)
		{
			return;
		}
	}
}

bool CKalima::CreateKalimaPortal(LPOBJ lpObj,int level,int x,int y) // OK
{
	level -= 1;

	if(CHECK_RANGE(level,7) == 0)
	{
		return 0;
	}

	if(lpObj->Map == MAP_ICARUS || KALIMA_MAP_RANGE(lpObj->Map) != 0 || DS_MAP_RANGE(lpObj->Map) != 0 || BC_MAP_RANGE(lpObj->Map) != 0 || CC_MAP_RANGE(lpObj->Map) != 0)
	{
		return 0;
	}

	#if(GAMESERVER_UPDATE>=301)

	if(IT_MAP_RANGE(lpObj->Map) != 0)
	{
		return 0;
	}

	#endif

	#if(GAMESERVER_UPDATE>=603)

	if(DA_MAP_RANGE(lpObj->Map) != 0 || DG_MAP_RANGE(lpObj->Map) != 0 || IG_MAP_RANGE(lpObj->Map) != 0)
	{
		return 0;
	}

	#endif

	#if(GAMESERVER_UPDATE>=803)

	if(CF_MAP_RANGE(lpObj->Map) != 0 || AB_MAP_RANGE(lpObj->Map) != 0)
	{
		return 0;
	}

	#endif

	if(gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,1) != 0)
	{
		return 0;
	}

	if(lpObj->KalimaGateExist != 0)
	{
		return 0;
	}

	int px = x;
	int py = y;

	if(gMap[lpObj->Map].CheckAttr(px,py,255) != 0)
	{
		if(gObjGetRandomFreeLocation(lpObj->Map,&px,&py,3,3,50) == 0)
		{
			return 0;
		}
	}

	int index = gObjAddMonster(lpObj->Map);

	if(OBJECT_RANGE(index) == 0)
	{
		return 0;
	}

	LPOBJ lpMonster = &gObj[index];

	lpMonster->PosNum = -1;
	lpMonster->X = px;
	lpMonster->Y = py;
	lpMonster->TX = px;
	lpMonster->TY = py;
	lpMonster->OldX = px;
	lpMonster->OldY = py;
	lpMonster->StartX = px;
	lpMonster->StartY = py;
	lpMonster->Dir = 1;
	lpMonster->Map = lpObj->Map;

	if(gObjSetMonster(index,(152+level)) == 0)
	{
		gObjDel(index);
		return 0;
	}

	lpMonster->MoveRange = 0;
	lpMonster->DieRegen = 0;
	lpMonster->RegenTime = GetTickCount();
	lpMonster->MaxRegenTime = 0;
	lpMonster->Attribute = ATTRIBUTE_KALIMA;
	lpMonster->SummonIndex = lpObj->Index;
	lpMonster->KalimaGateEnterCount = 0;
	lpMonster->KalimaGateLevel = level;

	lpObj->KalimaGateExist = 1;
	lpObj->KalimaGateIndex = index;

	return 1;
}

void CKalima::CreateKalimaNextPortal(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	if(gObjIsConnectedGP(lpObj->Index) == 0)
	{
		return;
	}

	int level = 0;

	switch(lpTarget->Class)
	{
		case 161:
			level = 1;
			break;
		case 181:
			level = 2;
			break;
		case 189:
			level = 3;
			break;
		case 197:
			level = 4;
			break;
		case 267:
			level = 5;
			break;
		case 338:
			level = 6;
			break;
	}

	if(this->m_KalimaPortal[level-1] == 0)
	{
		return;
	}

	if(lpObj->KalimaGateExist != 0)
	{
		return;
	}

	int px = lpTarget->X;
	int py = lpTarget->Y;

	if(gMap[lpObj->Map].CheckAttr(px,py,255) != 0)
	{
		if(gObjGetRandomFreeLocation(lpObj->Map,&px,&py,3,3,50) == 0)
		{
			return;
		}
	}

	int index = gObjAddMonster(lpObj->Map);

	if(OBJECT_RANGE(index) == 0)
	{
		return;
	}

	LPOBJ lpMonster = &gObj[index];

	lpMonster->PosNum = -1;
	lpMonster->X = px;
	lpMonster->Y = py;
	lpMonster->TX = px;
	lpMonster->TY = py;
	lpMonster->OldX = px;
	lpMonster->OldY = py;
	lpMonster->StartX = px;
	lpMonster->StartY = py;
	lpMonster->Dir = 1;
	lpMonster->Map = lpObj->Map;

	if(gObjSetMonster(index,(152+level)) == 0)
	{
		gObjDel(index);
		return;
	}

	lpMonster->MoveRange = 0;
	lpMonster->DieRegen = 0;
	lpMonster->RegenTime = GetTickCount();
	lpMonster->MaxRegenTime = 0;
	lpMonster->Attribute = ATTRIBUTE_KALIMA;
	lpMonster->SummonIndex = lpObj->Index;
	lpMonster->KalimaGateEnterCount = 0;
	lpMonster->KalimaGateLevel = level;

	lpObj->KalimaGateExist = 1;
	lpObj->KalimaGateIndex = index;
}

bool CKalima::DeleteKalimaPortal(int aIndex) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	int index = gObj[aIndex].KalimaGateIndex;

	if(OBJECT_RANGE(index) == 0)
	{
		return 0;
	}

	if(gObj[index].Live == 0)
	{
		return 0;
	}

	if(gObj[index].Attribute != ATTRIBUTE_KALIMA)
	{
		return 0;
	}

	GCUserDieSend(&gObj[index],index,0,0);

	gObjDel(index);

	gObj[aIndex].KalimaGateExist = 0;
	gObj[aIndex].KalimaGateIndex = -1;

	return 1;
}

void CKalima::KalimaRefillHP(LPOBJ lpObj) // OK
{
	if(lpObj->Live == 0)
	{
		return;
	}

	if(gCrywolfSync.CheckApplyPenalty() != 0 && gCrywolfSync.GetOccupationState() == 1 && gCrywolfSync.GetKundunHPRefillState() != 0)
	{
		if((GetTickCount()-lpObj->RefillTime) >= 300000)
		{
			lpObj->RefillTime = GetTickCount();

			lpObj->Life += (lpObj->MaxLife/4);

			lpObj->Life = (lpObj->Life>lpObj->MaxLife)?lpObj->MaxLife:lpObj->Life;
		}
	}
}