// CannonTower.h: interface for the CCannonTower class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CCannonTower
{
public:
	CCannonTower();
	virtual ~CCannonTower();
	void FindTarget(int aIndex);
};

extern CCannonTower gCannonTower;
