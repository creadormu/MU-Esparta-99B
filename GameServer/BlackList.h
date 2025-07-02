// BlackList.h: interface for the CBlackList class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

struct BLACKLIST_IP_INFO
{
	char IpAddress[17];
};

struct BLACKLIST_HID_INFO
{
	char HardwareId[45];
};

class CBlackList
{
public:
	CBlackList();
	virtual ~CBlackList();
	void Load(char* path);
	bool CheckIpAddress(char* IpAddress);
	bool CheckHardwareId(char* HardwareId);

public:
	std::map<std::string,BLACKLIST_IP_INFO> m_BlackListIpInfo;
	std::map<std::string,BLACKLIST_HID_INFO> m_BlackListHidInfo;
};

extern CBlackList gBlackList;