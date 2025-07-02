// DefaultClassFreebies.h: interface for the CDefaultClassFreebies class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "User.h"
#include "DefaultClassInfo.h"

struct DEFAULT_CLASS_FREEBIES_BUFF_INFO
{
	int Class;
	int Effect;
	int Type;
	int Power[4];
	int Duration;
};

struct DEFAULT_CLASS_FREEBIES_ITEM_INFO
{
	int Class;
	int ItemIndex;
	int Level;
	int Dur;
	int Option1;
	int Option2;
	int Option3;
	int NewOption;
	int SetOption;
	int SocketCount;
	int Duration;
};

struct DEFAULT_CLASS_FREEBIES_INFO
{
	int Class;
	int Money;
	int LevelUpPoint;
	std::vector<DEFAULT_CLASS_FREEBIES_ITEM_INFO> ItemList;
	std::vector<DEFAULT_CLASS_FREEBIES_BUFF_INFO> BuffList;
};

class CDefaultClassFreebies
{
public:
	CDefaultClassFreebies();
	virtual ~CDefaultClassFreebies();
	void Load(char* path);
	void SetInfo(DEFAULT_CLASS_FREEBIES_INFO info);
	void SetItemInfo(DEFAULT_CLASS_FREEBIES_ITEM_INFO info);
	void SetBuffInfo(DEFAULT_CLASS_FREEBIES_BUFF_INFO info);
	void GetCharacterFreebies(LPOBJ lpObj,int status);
public:
	DEFAULT_CLASS_FREEBIES_INFO m_DefaultClassFreebiesInfo[MAX_CLASS];
};

extern CDefaultClassFreebies gDefaultClassFreebies;