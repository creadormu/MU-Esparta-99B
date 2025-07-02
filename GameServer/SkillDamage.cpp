// SkillDamage.cpp: interface for the CSkillDamage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillDamage.h"
#include "MemScript.h"
#include "Util.h"

CSkillDamage gSkillDamage;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillDamage::CSkillDamage() // OK
{
	this->m_SkillDamageInfo.clear();
}

CSkillDamage::~CSkillDamage() // OK
{

}

void CSkillDamage::Load(char* path) // OK
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

	this->m_SkillDamageInfo.clear();

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

			SKILL_DAMAGE_INFO info;

			info.Index = lpMemScript->GetNumber();

			info.Damage = lpMemScript->GetAsNumber();

			this->SetInfo(info);
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CSkillDamage::SetInfo(SKILL_DAMAGE_INFO info) // OK
{
	std::map<int,SKILL_DAMAGE_INFO>::iterator it = this->m_SkillDamageInfo.find(info.Index);

	if(it != this->m_SkillDamageInfo.end())
	{
		return;
	}

	this->m_SkillDamageInfo.insert(std::pair<int,SKILL_DAMAGE_INFO>(info.Index,info));
}

int CSkillDamage::GetDamage(CSkill* lpSkill,int damage) // OK
{
	if(lpSkill != 0)
	{
		std::map<int,SKILL_DAMAGE_INFO>::iterator it = this->m_SkillDamageInfo.find(lpSkill->m_index);

		if(it != this->m_SkillDamageInfo.end())
		{
			damage = (damage*it->second.Damage)/100;
		}
	}
	
	return damage;
}