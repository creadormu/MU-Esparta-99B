// MapRateInfo.h: interface for the CMapRateInfo class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

enum MAP_DROP_TYPE
{
	DROP_TYPE_COMMON = 0,
	DROP_TYPE_EXCELLENT = 1,
	DROP_TYPE_ANCIENT = 2,
	DROP_TYPE_SOCKET1 = 3,
	DROP_TYPE_SOCKET2 = 4,
};

struct MAP_RATE_INFO
{
	int Index;
	int DropType;
	int Option0;
	int Option1;
	int Option2;
	int Option3;
	int Option4;
	int Option5;
	int Option6;
};

class CMapRateInfo
{
public:
	CMapRateInfo();
	virtual ~CMapRateInfo();
	void Load(char* path);
	int GetDropOption0(int index,int type);
	int GetDropOption1(int index,int type);
	int GetDropOption2(int index,int type);
	int GetDropOption3(int index,int type);
	int GetDropOption4(int index,int type);
	int GetDropOption5(int index,int type);
	int GetDropOption6(int index,int type);
public:
	std::vector<MAP_RATE_INFO> m_MapRateInfo;
};

extern CMapRateInfo gMapRateInfo;