// CustomMix.h: interface for the CCustomMix class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "User.h"

struct CUSTOM_MIX_REQUIRE_ITEM_INFO
{
	int Index;
	int Group;
	int Count;
	int ItemIndex;
	int ItemLevel;
	int ItemOption1;
	int ItemOption2;
	int ItemOption3;
	int ItemNewOption;
	int ItemSetOption;
};

struct CUSTOM_MIX_GROUP_INFO
{
	int Index;
	int Group;
	int BagSpecial;
	std::map<int,CUSTOM_MIX_REQUIRE_ITEM_INFO> RequireItem;
};

struct CUSTOM_MIX_INFO
{
	int Index;
	int MixMoney;
	int MixRate[MAX_ACCOUNT_LEVEL];
	char Name[32];
	std::map<int,CUSTOM_MIX_GROUP_INFO> MixGroup;
};

class CCustomMix
{
public:
	CCustomMix();
	virtual ~CCustomMix();
	void Load(char* path);
	void SetInfo(CUSTOM_MIX_INFO info);
	void SetGroupInfo(CUSTOM_MIX_GROUP_INFO info);
	void SetItemInfo(CUSTOM_MIX_REQUIRE_ITEM_INFO info);
	bool CheckItem(LPOBJ lpObj,CUSTOM_MIX_REQUIRE_ITEM_INFO* lpInfo);
	bool CheckGroup(LPOBJ lpObj,CUSTOM_MIX_GROUP_INFO* lpInfo);
	CUSTOM_MIX_INFO* GetInfoMix(int index);
public:
	std::map<int,CUSTOM_MIX_INFO> m_CustomMixInfo;
};

extern CCustomMix gCustomMix;