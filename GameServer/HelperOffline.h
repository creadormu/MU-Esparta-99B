// HelperOffline.h: interface for the CHelperOffline class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "DefaultClassInfo.h"
#include "Map.h"
#include "User.h"

struct HELPER_SKILL_INFO
{
	int Skill;
	int Group;
	int Value;
	int BaseSpeed;
	int MainSpeed;
};

class CHelperOffline
{
public:
	CHelperOffline();
	virtual ~CHelperOffline();
	void Load(char* path);
	void SetInfo(HELPER_SKILL_INFO info);
	HELPER_SKILL_INFO* GetInfo(int index);
	void MainProc(LPOBJ lpObj);
	void SecondProc(LPOBJ lpObj);
	void CommandHelperOffline(LPOBJ lpObj);
	void HelperOfflineClose(LPOBJ lpObj);
	void HelperOfflineAutoPotion(LPOBJ lpObj);
	void HelperOfflineAutoBuff(LPOBJ lpObj);
	void HelperOfflineAutoPick(LPOBJ lpObj);
	void HelperOfflineAutoPetPick(LPOBJ lpObj);
	void HelperOfflineAutoReload(LPOBJ lpObj);
	void HelperOfflineAutoRepair(LPOBJ lpObj);
	void HelperOfflineAutoBuffParty(LPOBJ lpObj);
	void HelperOfflineAutoAttack(LPOBJ lpObj);
	bool HelperOfflinePotionMP(LPOBJ lpObj,int SkillNumber);
	void HelperOfflineMove(LPOBJ lpObj,int x,int y);
	int HelperOfflineCountTarget(LPOBJ lpObj);
	int HelperOfflineFindTarget(LPOBJ lpObj,int self,int another);
	CSkill* HelperOfflineGetSkill(LPOBJ lpObj,int SkillNumber);
	bool HelperOfflineUseSkill(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill);
private:
	std::map<int,HELPER_SKILL_INFO> m_HelperSkillInfo;
};

extern CHelperOffline gHelperOffline;