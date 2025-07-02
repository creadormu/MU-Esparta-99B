// ComboSkill.h: interface for the CComboSkill class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CComboSkill
{
public:
	void Init();
	bool CheckCombo(int Class,WORD skill);
public:
	DWORD m_time;
	WORD m_skill[2];
	int m_index;
};
