// CustomMonster.cpp: implementation of the CCustomMonster class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomMonster.h"
#include "DSProtocol.h"
#include "MemScript.h"
#include "Message.h"
#include "Monster.h"
#include "Notice.h"
#include "Util.h"

CCustomMonster gCustomMonster;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomMonster::CCustomMonster() // OK
{
	this->m_CustomMonsterInfo.clear();
}

CCustomMonster::~CCustomMonster() // OK
{

}

void CCustomMonster::Load(char* path) // OK
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

	this->m_CustomMonsterInfo.clear();

	try
	{
		while(true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			if(strcmp("end",lpMemScript->GetString()) == 0)
			{
				break;
			}

			CUSTOM_MONSTER_INFO info;

			memset(&info,0,sizeof(info));

			info.MonsterClass = lpMemScript->GetNumber();

			info.MapNumber = lpMemScript->GetAsNumber();

			info.MaxLife = lpMemScript->GetAsNumber();

			info.DamageMin = lpMemScript->GetAsNumber();

			info.DamageMax = lpMemScript->GetAsNumber();

			info.Defense = lpMemScript->GetAsNumber();

			info.AttackRate = lpMemScript->GetAsNumber();

			info.DefenseRate = lpMemScript->GetAsNumber();

			info.ExperienceRate = lpMemScript->GetAsNumber();

			info.KillMessage = lpMemScript->GetAsNumber();

			info.RewardMessage = lpMemScript->GetAsNumber();

			#if(GAMESERVER_SHOP>=1)

			info.RewardCoin1 = lpMemScript->GetAsNumber();

			#endif

			#if(GAMESERVER_SHOP==3)

			info.RewardCoin2 = lpMemScript->GetAsNumber();

			info.RewardCoin3 = lpMemScript->GetAsNumber();

			#endif

			this->m_CustomMonsterInfo.push_back(info);
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CCustomMonster::SetCustomMonsterInfo(LPOBJ lpObj) // OK
{
	CUSTOM_MONSTER_INFO CustomMonsterInfo;

	if(this->GetCustomMonsterInfo(lpObj->Class,lpObj->Map,&CustomMonsterInfo) == 0)
	{
		return;
	}

	lpObj->Life = (float)((__int64)lpObj->Life*((CustomMonsterInfo.MaxLife==-1)?100:CustomMonsterInfo.MaxLife))/100;

	lpObj->MaxLife = (float)((__int64)lpObj->MaxLife*((CustomMonsterInfo.MaxLife==-1)?100:CustomMonsterInfo.MaxLife))/100;

	lpObj->ScriptMaxLife = (float)((__int64)lpObj->ScriptMaxLife*((CustomMonsterInfo.MaxLife==-1)?100:CustomMonsterInfo.MaxLife))/100;

	lpObj->PhysiDamageMin = ((__int64)lpObj->PhysiDamageMin*((CustomMonsterInfo.DamageMin==-1)?100:CustomMonsterInfo.DamageMin))/100;

	lpObj->PhysiDamageMax = ((__int64)lpObj->PhysiDamageMax*((CustomMonsterInfo.DamageMax==-1)?100:CustomMonsterInfo.DamageMax))/100;

	lpObj->Defense = ((__int64)lpObj->Defense*((CustomMonsterInfo.Defense==-1)?100:CustomMonsterInfo.Defense))/100;

	lpObj->AttackSuccessRate = ((__int64)lpObj->AttackSuccessRate*((CustomMonsterInfo.AttackRate==-1)?100:CustomMonsterInfo.AttackRate))/100;

	lpObj->DefenseSuccessRate = ((__int64)lpObj->DefenseSuccessRate*((CustomMonsterInfo.DefenseRate==-1)?100:CustomMonsterInfo.DefenseRate))/100;

	#if(GAMESERVER_UPDATE>=701)

	lpObj->ElementalDefense = ((__int64)lpObj->ElementalDefense*((CustomMonsterInfo.Defense==-1)?100:CustomMonsterInfo.Defense))/100;

	lpObj->ElementalDamageMin = ((__int64)lpObj->ElementalDamageMin*((CustomMonsterInfo.DamageMin==-1)?100:CustomMonsterInfo.DamageMin))/100;

	lpObj->ElementalDamageMax = ((__int64)lpObj->ElementalDamageMax*((CustomMonsterInfo.DamageMax==-1)?100:CustomMonsterInfo.DamageMax))/100;

	lpObj->ElementalAttackSuccessRate = ((__int64)lpObj->ElementalAttackSuccessRate*((CustomMonsterInfo.AttackRate==-1)?100:CustomMonsterInfo.AttackRate))/100;

	lpObj->ElementalDefenseSuccessRate = ((__int64)lpObj->ElementalDefenseSuccessRate*((CustomMonsterInfo.DefenseRate==-1)?100:CustomMonsterInfo.DefenseRate))/100;

	#endif
}

void CCustomMonster::MonsterDieProc(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	CUSTOM_MONSTER_INFO CustomMonsterInfo;

	if(this->GetCustomMonsterInfo(lpObj->Class,lpObj->Map,&CustomMonsterInfo) == 0)
	{
		return;
	}

	int aIndex = gObjMonsterGetTopHitDamageUser(lpObj);

	if(OBJECT_RANGE(aIndex) != 0)
	{
		lpTarget = &gObj[aIndex];
	}

	if(CustomMonsterInfo.KillMessage != -1)
	{
		gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(CustomMonsterInfo.KillMessage),lpTarget->Name);
	}

	if(CustomMonsterInfo.RewardMessage != -1)
	{
		gNotice.GCNoticeSend(lpTarget->Index,0,0,0,0,0,0,gMessage.GetMessage(CustomMonsterInfo.RewardMessage),CustomMonsterInfo.RewardCoin1,CustomMonsterInfo.RewardCoin2,CustomMonsterInfo.RewardCoin3);
	}

	if(CustomMonsterInfo.RewardCoin1 > 0 || CustomMonsterInfo.RewardCoin2 > 0 || CustomMonsterInfo.RewardCoin3 > 0)
	{
		gObjCoinAdd(lpTarget->Index,CustomMonsterInfo.RewardCoin1,CustomMonsterInfo.RewardCoin2,CustomMonsterInfo.RewardCoin3);
	}
}

long CCustomMonster::GetCustomMonsterExperienceRate(int index,int map) // OK
{
	CUSTOM_MONSTER_INFO CustomMonsterInfo;

	if(this->GetCustomMonsterInfo(index,map,&CustomMonsterInfo) == 0)
	{
		return 100;
	}
	else
	{
		return ((CustomMonsterInfo.ExperienceRate==-1)?100:CustomMonsterInfo.ExperienceRate);
	}
}

long CCustomMonster::GetCustomMonsterMasterExperienceRate(int index,int map) // OK
{
	CUSTOM_MONSTER_INFO CustomMonsterInfo;

	if(this->GetCustomMonsterInfo(index,map,&CustomMonsterInfo) == 0)
	{
		return 100;
	}
	else
	{
		return ((CustomMonsterInfo.ExperienceRate ==-1)?100:CustomMonsterInfo.ExperienceRate);
	}
}

bool CCustomMonster::GetCustomMonsterInfo(int index,int map,CUSTOM_MONSTER_INFO* lpInfo) // OK
{
	for(std::vector<CUSTOM_MONSTER_INFO>::iterator it=this->m_CustomMonsterInfo.begin();it != this->m_CustomMonsterInfo.end();it++)
	{
		if(it->MonsterClass == index && (it->MapNumber == -1 || it->MapNumber == map))
		{
			(*lpInfo) = (*it);
			return 1;
		}
	}

	return 0;
}