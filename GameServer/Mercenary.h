// Mercenary.h: interface for the CMercenary class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CMercenary
{
public:
	CMercenary();
	virtual ~CMercenary();
	bool CreateMercenary(int aIndex,int MonsterClass,int x,int y);
	void DeleteMercenary(int aIndex);
	void FindTarget(int aIndex);
private:
	int m_MercenaryCount;
};

extern CMercenary gMercenary;
