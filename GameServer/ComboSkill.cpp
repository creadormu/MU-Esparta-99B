// ComboSkill.cpp: implementation of the CComboSkill class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ComboSkill.h"
#include "CustomComboSkill.h"
#include "SkillManager.h"
#include "Util.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void CComboSkill::Init() // OK
{
	this->m_time = 0;
	this->m_skill[0] = 0xFFFF;
	this->m_skill[1] = 0xFFFF;
	this->m_index = -1;
}

bool CComboSkill::CheckCombo(int Class,WORD skill) // OK
{
	int delay = 3000;
	int type = gCustomComboSkill.CheckSkill(Class,skill,&delay);

	if(type == -1)
	{
		this->Init();
		return 0;
	}

	if(type == 0)
	{
		this->m_time = GetTickCount()+delay;
		this->m_skill[0] = skill;
		this->m_index = 0;
		return 0;
	}

	if(type == 1)
	{
		if(this->m_time < GetTickCount())
		{
			this->Init();
			return 0;
		}

		if(this->m_skill[0] == 0xFFFF)
		{
			this->Init();
			return 0;
		}

		if(this->m_index == 0)
		{
			this->m_time = GetTickCount()+delay;
			this->m_skill[1] = skill;
			this->m_index = 1;
			return 0;
		}
		
		if(this->m_index == 1 && this->m_skill[1] != skill)
		{
			this->Init();
			return 1;
		}
	}

	this->Init();
	return 0;
}