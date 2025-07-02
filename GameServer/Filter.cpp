// Filter.cpp: implementation of the CFilter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Filter.h"
#include "MemScript.h"
#include "Util.h"

CFilter gFilter;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFilter::CFilter() // OK
{
	this->m_FilterInfo.clear();
}

CFilter::~CFilter() // OK
{

}

void CFilter::Load(char* path) // OK
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

	this->m_FilterInfo.clear();

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

			FILTER_INFO info;

			memset(&info,0,sizeof(info));

			strcpy_s(info.Text,CharToLower(lpMemScript->GetString()));

			this->m_FilterInfo.insert(std::pair<std::string,FILTER_INFO>(info.Text,info));
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

bool CFilter::CheckSyntax(char* text) // OK
{
	char* buff = CharToLower(text);

	for(std::map<std::string,FILTER_INFO>::iterator it=this->m_FilterInfo.begin();it != this->m_FilterInfo.end();it++)
	{
		if(strstr(buff,it->second.Text) != 0)
		{
			return 1;
		}
	}

	return 0;
}
