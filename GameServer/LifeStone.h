// LifeStone.h: interface for the CLifeStone class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CLifeStone
{
public:
	CLifeStone();
	virtual ~CLifeStone();
	bool CreateLifeStone(int aIndex);
	void DeleteLifeStone(int aIndex);
	bool SetReSpawnUserXY(int aIndex);
	void FindTarget(int aIndex);
};

extern CLifeStone gLifeStone;
