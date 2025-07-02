// SkillUseArea.cpp: interface for the CSkillUseArea class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillUseArea.h"
#include "MemScript.h"
#include "Util.h"

CSkillUseArea gSkillUseArea;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillUseArea::CSkillUseArea() // OK
{
	this->m_SkillUseInfo.clear();

	this->m_SkillUseAreaInfo.clear();
}

CSkillUseArea::~CSkillUseArea() // OK
{

}

void CSkillUseArea::Load(char* path) // OK
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

	this->m_SkillUseInfo.clear();

	this->m_SkillUseAreaInfo.clear();

	try
	{
		while (true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			int section = lpMemScript->GetNumber();

			while (true)
			{
				if(section == 0)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					SKILL_USE_INFO info;

					info.Index = lpMemScript->GetNumber();

					this->m_SkillUseInfo.insert(std::pair<int,SKILL_USE_INFO>(info.Index,info));
				}
				else if(section == 1)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					SKILL_USE_AREA_INFO info;

					info.Map = lpMemScript->GetNumber();

					info.X = lpMemScript->GetAsNumber();

					info.Y = lpMemScript->GetAsNumber();

					info.TX = lpMemScript->GetAsNumber();

					info.TY = lpMemScript->GetAsNumber();

					this->m_SkillUseAreaInfo.push_back(info);
				}
				else
				{
					break;
				}
			}
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

bool CSkillUseArea::CheckUseSkill(LPOBJ lpObj,int Index) // OK
{
	std::map<int,SKILL_USE_INFO>::iterator it = this->m_SkillUseInfo.find(Index);

	if(it == this->m_SkillUseInfo.end())
	{
		return 0;
	}

	for(std::vector<SKILL_USE_AREA_INFO>::iterator it = this->m_SkillUseAreaInfo.begin(); it != this->m_SkillUseAreaInfo.end(); it++)
	{
		if(it->Map != lpObj->Map)
		{
			continue;
		}

		if((lpObj->X < it->X || lpObj->X > it->TX) || (lpObj->Y < it->Y || lpObj->Y > it->TY))
		{
			continue;
		}

		return 1;
	}

	return 0;
}