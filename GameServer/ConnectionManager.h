// ConnectionManager.h: interface for the CConnectionManager class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Protocol.h"

//**********************************************//
//************ Client -> GameServer ************//
//**********************************************//

struct PMSG_HARDWARE_ID_INFO_RECV
{
	PSBMSG_HEAD header; // C1:F3:09
	char HardwareId[45];
};

//**********************************************//
//**********************************************//
//**********************************************//

struct HARDWARE_ID_INFO
{
	char HardwareId[45];
	int count;
};

struct IP_ADDRESS_INFO
{
	char IpAddress[16];
	int count;
};

class CConnectionManager
{
public:
	CConnectionManager();
	virtual ~CConnectionManager();
	bool CheckHardwareId(char* HardwareId);
	void InsertHardwareId(char* HardwareId);
	void RemoveHardwareId(char* HardwareId);
	bool CheckIpAddress(char* IpAddress);
	void InsertIpAddress(char* IpAddress);
	void RemoveIpAddress(char* IpAddress);
	void CGHardwareIdRecv(PMSG_HARDWARE_ID_INFO_RECV* lpMsg, int aIndex);
private:
	std::map<std::string,HARDWARE_ID_INFO> m_HardwareIdInfo;
	std::map<std::string,IP_ADDRESS_INFO> m_IpAddressInfo;
};

extern CConnectionManager gConnectionManager;