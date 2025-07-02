// ExcellentOptionRate.cpp: implementation of the CExcellentOptionRate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ExcellentOptionRate.h"
#include "ItemManager.h"
#include "MemScript.h"
#include "RandomManager.h"
#include "Util.h"

CExcellentOptionRate gExcellentOptionRate;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExcellentOptionRate::CExcellentOptionRate() // OK
{
	this->m_ExcellentItemOptionInfo.clear();
}

CExcellentOptionRate::~CExcellentOptionRate() // OK
{

}

void CExcellentOptionRate::Load(char* path) // OK
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

	this->m_ExcellentItemOptionInfo.clear();

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

			EXCELLENT_ITEM_OPTION_INFO info;

			info.Index = lpMemScript->GetNumber();

			info.ItemMinIndex = SafeGetItem(GET_ITEM(lpMemScript->GetAsNumber(),lpMemScript->GetAsNumber()));

			info.ItemMaxIndex = SafeGetItem(GET_ITEM(lpMemScript->GetAsNumber(),lpMemScript->GetAsNumber()));

			strcpy_s(info.Name,lpMemScript->GetAsString());

			info.Rate = lpMemScript->GetAsNumber();

			this->m_ExcellentItemOptionInfo.push_back(info);
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

int CExcellentOptionRate::GetNewOption(int ItemIndex)
{
	int value = 0;

	CRandomManager RandomManager;

	for(std::vector<EXCELLENT_ITEM_OPTION_INFO>::iterator it = this->m_ExcellentItemOptionInfo.begin(); it != this->m_ExcellentItemOptionInfo.end(); it++)
	{
		if(it->ItemMinIndex != -1 && it->ItemMinIndex > ItemIndex)
		{
			continue;
		}

		if(it->ItemMaxIndex != -1 && it->ItemMaxIndex < ItemIndex)
		{
			continue;
		}

		RandomManager.AddElement(it->Index,it->Rate);
	}

	RandomManager.GetRandomElement(&value);

	return value;
}