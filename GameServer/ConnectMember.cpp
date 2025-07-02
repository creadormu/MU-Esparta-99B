// ConnectMember.cpp: implementation of the CConnectMember class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ConnectMember.h"
#include "MemScript.h"
#include "Util.h"

CConnectMember gConnectMember;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConnectMember::CConnectMember() // OK
{
	this->m_ConnectMemberInfo.clear();
}

CConnectMember::~CConnectMember() // OK
{

}

void CConnectMember::Load(char* path) // OK
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

	this->m_ConnectMemberInfo.clear();

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

			CONNECT_MEMBER_INFO info;

			memset(&info,0,sizeof(info));

			strcpy_s(info.Account,lpMemScript->GetString());

			this->m_ConnectMemberInfo.insert(std::pair<std::string,CONNECT_MEMBER_INFO>(info.Account,info));
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

bool CConnectMember::CheckAccount(char* Account) // OK
{
	std::map<std::string,CONNECT_MEMBER_INFO>::iterator it = this->m_ConnectMemberInfo.find(Account);

	if(it == this->m_ConnectMemberInfo.end())
	{
		return 0;
	}
	else
	{
		return 1;
	}
}