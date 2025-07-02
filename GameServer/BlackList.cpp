// BlackList.cpp: implementation of the CBlackList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BlackList.h"
#include "MemScript.h"
#include "Util.h"

CBlackList gBlackList;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBlackList::CBlackList() // OK
{
	this->m_BlackListIpInfo.clear();

	this->m_BlackListHidInfo.clear();
}

CBlackList::~CBlackList() // OK
{

}

void CBlackList::Load(char* path) // OK
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

	this->m_BlackListIpInfo.clear();

	this->m_BlackListHidInfo.clear();

	try
	{
		while(true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}
		
			int section = lpMemScript->GetNumber();

			while(true)
			{
				if(section == 0)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					BLACKLIST_IP_INFO info;

					memset(&info,0,sizeof(info));

					strcpy_s(info.IpAddress,lpMemScript->GetString());

					this->m_BlackListIpInfo.insert(std::pair<std::string,BLACKLIST_IP_INFO>(std::string(info.IpAddress),info));
				}
				else if(section == 1)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					BLACKLIST_HID_INFO info;

					memset(&info,0,sizeof(info));

					strcpy_s(info.HardwareId,lpMemScript->GetString());

					this->m_BlackListHidInfo.insert(std::pair<std::string,BLACKLIST_HID_INFO>(std::string(info.HardwareId),info));
				}
				else
				{
					break;
				}
			}
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

bool CBlackList::CheckIpAddress(char* IpAddress) // OK
{
	std::map<std::string,BLACKLIST_IP_INFO>::iterator it = this->m_BlackListIpInfo.find(std::string(IpAddress));

	if(it == this->m_BlackListIpInfo.end())
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool CBlackList::CheckHardwareId(char* HardwareId) // OK
{
	std::map<std::string,BLACKLIST_HID_INFO>::iterator it = this->m_BlackListHidInfo.find(std::string(HardwareId));

	if(it == this->m_BlackListHidInfo.end())
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
