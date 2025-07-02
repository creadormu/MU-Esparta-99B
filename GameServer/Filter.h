// Filter.h: interface for the CFilter class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

struct FILTER_INFO
{
	char Text[32];
};

class CFilter
{
public:
	CFilter();
	virtual ~CFilter();
	void Load(char* path);
	bool CheckSyntax(char* text);
private:
	std::map<std::string,FILTER_INFO> m_FilterInfo;
};

extern CFilter gFilter;
