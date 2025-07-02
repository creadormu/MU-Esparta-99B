// EventSpawnStage.h: interface for the CEventSpawnStage class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

struct EVENT_SPAWN_STAGE_INFO
{
	int Event;
	int Level;
	int Stage;
	int MonsterClass;
	int MaxRegenTime;
};

class CEventSpawnStage
{
public:
	CEventSpawnStage();
	virtual ~CEventSpawnStage();
	void Load(char* path);
public:
	std::vector<EVENT_SPAWN_STAGE_INFO> m_EventSpawnStageInfo;
};

extern CEventSpawnStage gEventSpawnStage;