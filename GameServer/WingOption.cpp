// CustomWing.cpp: implementation of the CCustomWing class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WingOption.h"
#include "ItemManager.h"
#include "MemScript.h"
#include "Util.h"

CWingOption gWingOption;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWingOption::CWingOption() // OK
{
	this->m_WingOptionInfo.clear();
}

CWingOption::~CWingOption() // OK
{

}


void CWingOption::Load(char* path) // OK
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

	this->m_WingOptionInfo.clear();

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

			WING_OPTION_INFO info;

			memset(&info,0,sizeof(info));

			info.ItemIndex = SafeGetItem(GET_ITEM(lpMemScript->GetNumber(),lpMemScript->GetAsNumber()));

			info.DefenseConstA = lpMemScript->GetAsNumber();

			info.IncDamageConstA = lpMemScript->GetAsNumber();

			info.IncDamageConstB = lpMemScript->GetAsNumber();

			info.DecDamageConstA = lpMemScript->GetAsNumber();

			info.DecDamageConstB = lpMemScript->GetAsNumber();

			info.MaxNewOption = lpMemScript->GetAsNumber();

			this->m_WingOptionInfo.insert(std::pair<int,WING_OPTION_INFO>(info.ItemIndex,info));
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

bool CWingOption::CheckWingByItem(int ItemIndex) // OK
{
	std::map<int, WING_OPTION_INFO>::iterator it = this->m_WingOptionInfo.find(ItemIndex);

	if(it == this->m_WingOptionInfo.end())
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int CWingOption::GetWingDefense(int ItemIndex,int ItemLevel) // OK
{
	std::map<int, WING_OPTION_INFO>::iterator it = this->m_WingOptionInfo.find(ItemIndex);

	if(it == this->m_WingOptionInfo.end())
	{
		return 0;
	}

	return (it->second.DefenseConstA*ItemLevel);
}

int CWingOption::GetWingIncDamage(int ItemIndex,int ItemLevel) // OK
{
	std::map<int, WING_OPTION_INFO>::iterator it = this->m_WingOptionInfo.find(ItemIndex);

	if(it == this->m_WingOptionInfo.end())
	{
		return 100;
	}

	return (it->second.IncDamageConstA+(ItemLevel*it->second.IncDamageConstB));
}

int CWingOption::GetWingDecDamage(int ItemIndex,int ItemLevel) // OK
{
	std::map<int, WING_OPTION_INFO>::iterator it = this->m_WingOptionInfo.find(ItemIndex);

	if(it == this->m_WingOptionInfo.end())
	{
		return 100;
	}

	return (it->second.DecDamageConstA-(ItemLevel*it->second.DecDamageConstB));
}

int CWingOption::GetWingMaxNewOption(int ItemIndex) // OK
{
	std::map<int, WING_OPTION_INFO>::iterator it = this->m_WingOptionInfo.find(ItemIndex);

	if(it == this->m_WingOptionInfo.end())
	{
		return 0;
	}

	return (it->second.MaxNewOption > MAX_EXC_OPTION) ? MAX_EXC_OPTION : it->second.MaxNewOption;
}