// KanturuMaya.cpp: implementation of the CKanturuMaya class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KanturuMaya.h"
#include "KanturuUtil.h"
#include "Map.h"
#include "MonsterSkillManager.h"
#include "ObjectManager.h"
#include "User.h"
#include "Util.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKanturuMaya::CKanturuMaya() // OK
{
	this->Init();
}

CKanturuMaya::~CKanturuMaya() // OK
{

}

void CKanturuMaya::Init() // OK
{
	this->m_MayaObjIndex = -1;
	this->m_MayaSkillTime = 0;
	this->m_IceStormCount = 0;
}

void CKanturuMaya::KanturuMayaAct_IceStorm(int rate) // OK
{
	#if(GAMESERVER_UPDATE>=201)

	if((GetLargeRand()%10000) > rate)
	{
		return;
	}

	if(OBJECT_RANGE(this->m_MayaObjIndex) == 0)
	{
		return;
	}

	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnectedGP(n) != 0 && gObj[n].Map == MAP_KANTURU3)
		{
			gKanturuUtil.NotifyKanturuAreaAttack(this->m_MayaObjIndex,n,0);
			gMonsterSkillManager.UseMonsterSkill(this->m_MayaObjIndex,n,31,-1,0);
			this->m_IceStormCount++;
		}
	}

	#endif
}

void CKanturuMaya::KanturuMayaAct_Hands() // OK
{
	#if(GAMESERVER_UPDATE>=201)

	if((GetTickCount()-this->m_MayaSkillTime) < 20000)
	{
		return;
	}

	if(OBJECT_RANGE(this->m_MayaObjIndex) == 0)
	{
		return;
	}

	this->m_MayaSkillTime = GetTickCount();

	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnectedGP(n) != 0 && gObj[n].Map == MAP_KANTURU3)
		{
			gKanturuUtil.NotifyKanturuAreaAttack(this->m_MayaObjIndex,n,1);

			gMonsterSkillManager.UseMonsterSkill(this->m_MayaObjIndex,n,1,-1,0);

			if((gObj[n].Inventory[INVENTORY_SLOT_RING1].m_Index == GET_ITEM(13,38) && gObj[n].Inventory[INVENTORY_SLOT_RING1].m_Durability == 0) || (gObj[n].Inventory[INVENTORY_SLOT_RING2].m_Index == GET_ITEM(13,38) && gObj[n].Inventory[INVENTORY_SLOT_RING2].m_Durability == 0))
			{
				gObj[n].Life = 0;
				gObjectManager.CharacterLifeCheck(&gObj[this->m_MayaObjIndex],&gObj[n],0,0,0,0,0,0,0);
			}
		}
	}

	#endif
}

void CKanturuMaya::SetMayaObjIndex(int aIndex) // OK
{
	this->m_MayaObjIndex = aIndex;
}
