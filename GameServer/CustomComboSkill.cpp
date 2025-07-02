// CustomComboSkill.cpp: implementation of the CCustomComboSkill class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomComboSkill.h"
#include "DefaultClassInfo.h"
#include "MemScript.h"
#include "Util.h"

CCustomComboSkill gCustomComboSkill;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomComboSkill::CCustomComboSkill() // OK
{
	for(int i=0;i<MAX_CLASS;i++)
	{
		this->m_ComboSkillInfo[i].clear();
	}
}

CCustomComboSkill::~CCustomComboSkill() // OK
{

}

void CCustomComboSkill::Load(char* path) // OK
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

	for(int i=0;i<MAX_CLASS;i++)
	{
		this->m_ComboSkillInfo[i].clear();
	}

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

			COMBO_SKILL_INFO info;

			info.Class = lpMemScript->GetNumber();

			info.Index = lpMemScript->GetAsNumber();

			info.Stage = lpMemScript->GetAsNumber();

			info.Delay = lpMemScript->GetAsNumber();

			this->SetInfo(info);
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CCustomComboSkill::SetInfo(COMBO_SKILL_INFO info) // OK
{
	if(CHECK_RANGE(info.Class,MAX_CLASS) == 0)
	{
		return;
	}

	std::map<int,COMBO_SKILL_INFO>::iterator it = this->m_ComboSkillInfo[info.Class].find(info.Index);

	if(it == this->m_ComboSkillInfo[info.Class].end())
	{
		this->m_ComboSkillInfo[info.Class].insert(std::pair<int,COMBO_SKILL_INFO>(info.Index,info));
	}
}

int CCustomComboSkill::CheckSkill(int Class,WORD Skill,int* delay) // OK
{
	std::map<int,COMBO_SKILL_INFO>::iterator it = this->m_ComboSkillInfo[Class].find(Skill);

	if(it == this->m_ComboSkillInfo[Class].end())
	{
		return -1;
	}

	(*delay) = it->second.Delay;

	return it->second.Stage;
}