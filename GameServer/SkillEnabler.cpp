// SkillEnabler.cpp: interface for the CSkillEnabler class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillEnabler.h"
#include "MemScript.h"
#include "Util.h"

CSkillEnabler gSkillEnabler;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillEnabler::CSkillEnabler() // OK
{
	this->m_SkillEnabler.clear();
}

CSkillEnabler::~CSkillEnabler() // OK
{

}

void CSkillEnabler::Load(char* path) // OK
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

	this->m_SkillEnabler.clear();

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

			SKILL_ENABLER_INFO info;

			info.Index = lpMemScript->GetNumber();

			info.Map = lpMemScript->GetAsNumber();

			info.PvM = lpMemScript->GetAsNumber();

			info.PvP = lpMemScript->GetAsNumber();

			this->m_SkillEnabler.push_back(info);
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

bool CSkillEnabler::CheckUseSkill(LPOBJ lpObj,LPOBJ lpTarget,int index) // OK
{
	for(std::vector<SKILL_ENABLER_INFO>::iterator it = this->m_SkillEnabler.begin(); it != this->m_SkillEnabler.end(); it++)
	{
		if(it->Index != index)
		{
			continue;
		}

		if(it->Map != -1 && it->Map != lpObj->Map)
		{
			continue;
		}

		if(lpTarget->Type == OBJECT_MONSTER)
		{
			return (it->PvM == 0) ? 0 : 1;
		}
		else if(lpTarget->Type == OBJECT_USER)
		{
			return (it->PvP == 0) ? 0 : 1;
		}
	}

	return 1;
}