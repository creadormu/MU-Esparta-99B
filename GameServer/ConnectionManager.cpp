// ConnectionManager.cpp: implementation of the CConnectionManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ConnectionManager.h"
#include "BlackList.h"
#include "Log.h"
#include "ServerInfo.h"

CConnectionManager gConnectionManager;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConnectionManager::CConnectionManager() // OK
{
	this->m_HardwareIdInfo.clear();

	this->m_IpAddressInfo.clear();
}

CConnectionManager::~CConnectionManager() // OK
{

}

bool CConnectionManager::CheckHardwareId(char* HardwareId) // OK
{
	if(gServerInfo.m_MaxConnectionPerHID == 0)
	{
		return 0;
	}

	std::map<std::string,HARDWARE_ID_INFO>::iterator lpHardwareId = this->m_HardwareIdInfo.find(HardwareId);

	if(lpHardwareId == this->m_HardwareIdInfo.end())
	{
		return 1;
	}
	else
	{
		return ((lpHardwareId->second.count >= gServerInfo.m_MaxConnectionPerHID)?0:1);
	}
}

void CConnectionManager::InsertHardwareId(char* HardwareId) // OK
{
	std::map<std::string,HARDWARE_ID_INFO>::iterator lpHardwareId = this->m_HardwareIdInfo.find(HardwareId);

	if(lpHardwareId == this->m_HardwareIdInfo.end())
	{
		HARDWARE_ID_INFO info;

		strcpy_s(info.HardwareId,HardwareId);

		info.count = 1;

		this->m_HardwareIdInfo.insert(std::pair<std::string,HARDWARE_ID_INFO>(HardwareId,info));
	}
	else
	{
		lpHardwareId->second.count++;
	}
}

void CConnectionManager::RemoveHardwareId(char* HardwareId) // OK
{
	std::map<std::string,HARDWARE_ID_INFO>::iterator lpHardwareId = this->m_HardwareIdInfo.find(HardwareId);

	if(lpHardwareId != this->m_HardwareIdInfo.end())
	{
		if((--lpHardwareId->second.count) == 0)
		{
			this->m_HardwareIdInfo.erase(lpHardwareId);
		}
	}
}

bool CConnectionManager::CheckIpAddress(char* IpAddress) // OK
{
	if(gServerInfo.m_MaxConnectionPerIP == 0)
	{
		return 0;
	}

	std::map<std::string,IP_ADDRESS_INFO>::iterator lpIpAddress = this->m_IpAddressInfo.find(IpAddress);

	if(lpIpAddress == this->m_IpAddressInfo.end())
	{
		return 1;
	}
	else
	{
		return ((lpIpAddress->second.count >= gServerInfo.m_MaxConnectionPerIP)?0:1);
	}
}

void CConnectionManager::InsertIpAddress(char* IpAddress) // OK
{
	std::map<std::string,IP_ADDRESS_INFO>::iterator lpIpAddress = this->m_IpAddressInfo.find(IpAddress);

	if(lpIpAddress == this->m_IpAddressInfo.end())
	{
		IP_ADDRESS_INFO info;

		strcpy_s(info.IpAddress,IpAddress);

		info.count = 1;

		this->m_IpAddressInfo.insert(std::pair<std::string,IP_ADDRESS_INFO>(IpAddress,info));
	}
	else
	{
		lpIpAddress->second.count++;
	}
}

void CConnectionManager::RemoveIpAddress(char* IpAddress) // OK
{
	std::map<std::string,IP_ADDRESS_INFO>::iterator lpIpAddress = this->m_IpAddressInfo.find(IpAddress);

	if(lpIpAddress != this->m_IpAddressInfo.end())
	{
		if((--lpIpAddress->second.count) == 0)
		{
			this->m_IpAddressInfo.erase(lpIpAddress);
		}
	}
}

void CConnectionManager::CGHardwareIdRecv(PMSG_HARDWARE_ID_INFO_RECV* lpMsg,int aIndex) // OK
{
	char HardwareId[45] = {0};

	memcpy(HardwareId,lpMsg->HardwareId,sizeof(lpMsg->HardwareId));

	if(HardwareId[8] != '-' && HardwareId[17] != '-' && HardwareId[26] != '-' && HardwareId[35] != '-')
	{
		gObjDel(aIndex);
		return;
	}

	if(strlen(HardwareId) < 0 || strlen(HardwareId) > 44 || strlen(HardwareId) != 44)
	{
		gObjDel(aIndex);
		return;
	}
	
	if(gBlackList.CheckHardwareId(HardwareId) == 0)
	{
		gObjDel(aIndex);
		return;
	}

	if(this->CheckHardwareId(HardwareId) == 0)
	{
		gObjDel(aIndex);
		return;
	}

	gObj[aIndex].ClientVerify = 1;

	strcpy_s(gObj[aIndex].HardwareId,HardwareId);

	gLog.Output(LOG_CONNECT,"[ObjectManager][%d] Verified client [%s][%s]",aIndex,gObj[aIndex].IpAddr,gObj[aIndex].HardwareId);

	this->InsertHardwareId(HardwareId);
}