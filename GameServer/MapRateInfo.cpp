// MapRateInfo.cpp: implementation of the CMapRateInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MapRateInfo.h"
#include "MemScript.h"
#include "Util.h"

CMapRateInfo gMapRateInfo;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMapRateInfo::CMapRateInfo() // OK
{
	this->m_MapRateInfo.clear();
}

CMapRateInfo::~CMapRateInfo() // OK
{

}

void CMapRateInfo::Load(char* path) // OK
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

	this->m_MapRateInfo.clear();

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

			MAP_RATE_INFO info;

			info.Index = lpMemScript->GetNumber();

			info.DropType = lpMemScript->GetAsNumber();

			info.Option0 = lpMemScript->GetAsNumber();

			info.Option1 = lpMemScript->GetAsNumber();

			info.Option2 = lpMemScript->GetAsNumber();

			info.Option3 = lpMemScript->GetAsNumber();

			info.Option4 = lpMemScript->GetAsNumber();

			info.Option5 = lpMemScript->GetAsNumber();

			#if(GAMESERVER_UPDATE>=401)

			info.Option6 = lpMemScript->GetAsNumber();

			#endif

			this->m_MapRateInfo.push_back(info);
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

int CMapRateInfo::GetDropOption0(int index,int type) // OK
{
	for(std::vector<MAP_RATE_INFO>::iterator it=this->m_MapRateInfo.begin();it != this->m_MapRateInfo.end();it++)
	{
		if(it->DropType != type)
		{
			continue;
		}

		if(it->Index != -1 && it->Index != index)
		{
			continue;
		}

		return it->Option0;
	}

	return 0;
}

int CMapRateInfo::GetDropOption1(int index,int type) // OK
{
	for(std::vector<MAP_RATE_INFO>::iterator it=this->m_MapRateInfo.begin();it != this->m_MapRateInfo.end();it++)
	{
		if(it->DropType != type)
		{
			continue;
		}

		if(it->Index != -1 && it->Index != index)
		{
			continue;
		}

		return it->Option1;
	}

	return 0;
}

int CMapRateInfo::GetDropOption2(int index,int type) // OK
{
	for(std::vector<MAP_RATE_INFO>::iterator it=this->m_MapRateInfo.begin();it != this->m_MapRateInfo.end();it++)
	{
		if(it->DropType != type)
		{
			continue;
		}

		if(it->Index != -1 && it->Index != index)
		{
			continue;
		}

		return it->Option2;
	}

	return 0;
}

int CMapRateInfo::GetDropOption3(int index,int type) // OK
{
	for(std::vector<MAP_RATE_INFO>::iterator it=this->m_MapRateInfo.begin();it != this->m_MapRateInfo.end();it++)
	{
		if(it->DropType != type)
		{
			continue;
		}

		if(it->Index != -1 && it->Index != index)
		{
			continue;
		}

		return it->Option3;
	}

	return 0;
}

int CMapRateInfo::GetDropOption4(int index,int type) // OK
{
	for(std::vector<MAP_RATE_INFO>::iterator it=this->m_MapRateInfo.begin();it != this->m_MapRateInfo.end();it++)
	{
		if(it->DropType != type)
		{
			continue;
		}

		if(it->Index != -1 && it->Index != index)
		{
			continue;
		}

		return it->Option4;
	}

	return 0;
}

int CMapRateInfo::GetDropOption5(int index,int type) // OK
{
	for(std::vector<MAP_RATE_INFO>::iterator it=this->m_MapRateInfo.begin();it != this->m_MapRateInfo.end();it++)
	{
		if(it->DropType != type)
		{
			continue;
		}

		if(it->Index != -1 && it->Index != index)
		{
			continue;
		}

		return it->Option5;
	}

	return 0;
}

int CMapRateInfo::GetDropOption6(int index,int type) // OK
{
	#if(GAMESERVER_UPDATE>=401)

	for(std::vector<MAP_RATE_INFO>::iterator it=this->m_MapRateInfo.begin();it != this->m_MapRateInfo.end();it++)
	{
		if(it->DropType != type)
		{
			continue;
		}

		if(it->Index != -1 && it->Index != index)
		{
			continue;
		}

		return it->Option6;
	}

	#endif

	return 0;
}