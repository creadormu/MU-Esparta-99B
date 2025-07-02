// HackSkillSpeedCheck.h: interface for the CHackSkillSpeedCheck class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

struct HACK_SKILL_SPEED_CHECK_INFO
{
	int Index;
	int Class;
	int MinLevel;
	int MaxLevel;
	int MinReset;
	int MaxReset;
	int MinSpeed;
	int MaxSpeed;
	int MaxDelay;
	int MinCount;
	int MaxCount;
	int Action;
};

class CHackSkillSpeedCheck
{
public:
	CHackSkillSpeedCheck();
	virtual ~CHackSkillSpeedCheck();
	void Load(char* path);
	bool CheckSkillSpeed(int aIndex,int index);
	bool CheckSkillMaxDelay(int aIndex,int index,int MaxDelay);
	bool CheckSkillMinCount(int aIndex,int index,int MinCount);
	bool CheckSkillMaxCount(int aIndex,int index,int MaxCount);
private:
	std::vector<HACK_SKILL_SPEED_CHECK_INFO> m_HackSkillSpeedCheckInfo;
};

extern CHackSkillSpeedCheck gHackSkillSpeedCheck;