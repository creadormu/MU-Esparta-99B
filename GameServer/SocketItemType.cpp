// SocketItemType.cpp: implementation of the CSocketItemType class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SocketItemType.h"
#include "ItemManager.h"
#include "MemScript.h"
#include "Util.h"

CSocketItemType gSocketItemType;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSocketItemType::CSocketItemType() // OK
{
	this->m_SocketItemTypeInfo.clear();
}

CSocketItemType::~CSocketItemType() // OK
{

}

void CSocketItemType::Load(char* path) // OK
{
	#if(GAMESERVER_UPDATE>=401)

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

	this->m_SocketItemTypeInfo.clear();

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

			SOCKET_ITEM_TYPE_INFO info;

			memset(&info,0,sizeof(info));

			info.ItemIndex = SafeGetItem(GET_ITEM(lpMemScript->GetNumber(),lpMemScript->GetAsNumber()));

			info.MaxSocket = lpMemScript->GetAsNumber();

			this->m_SocketItemTypeInfo.insert(std::pair<int,SOCKET_ITEM_TYPE_INFO>(info.ItemIndex,info));
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;

	#endif
}

bool CSocketItemType::CheckSocketItemType(int index) // OK
{
	#if(GAMESERVER_UPDATE>=401)

	std::map<int,SOCKET_ITEM_TYPE_INFO>::iterator it = this->m_SocketItemTypeInfo.find(index);

	if(it == this->m_SocketItemTypeInfo.end())
	{
		return 0;
	}
	else
	{
		return 1;
	}

	#else

	return 0;

	#endif
}

long CSocketItemType::GetSocketItemMaxSocket(int index) // OK
{
	#if(GAMESERVER_UPDATE>=401)

	std::map<int,SOCKET_ITEM_TYPE_INFO>::iterator it = this->m_SocketItemTypeInfo.find(index);

	if(it == this->m_SocketItemTypeInfo.end())
	{
		return 0;
	}
	else
	{
		return it->second.MaxSocket;
	}

	#else

	return 0;

	#endif
}
