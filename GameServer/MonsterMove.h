// MonsterMove.h: interface for the CMonsterMove class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

struct MONSTER_MOVE_SPAWN
{
	int Index;
	int MapN;
	int MapX;
	int MapY;
	int Dir;
	int Time;
};

struct MONSTER_MOVE_INFO
{
	int MonsterClass;
	int MonsterIndex;
	int RemainTime;
	int TargetTime;
	int TickCount;
	std::vector<MONSTER_MOVE_SPAWN> SpawnInfo;
};

class CMonsterMove
{
public:
	CMonsterMove();
	virtual ~CMonsterMove();
	void Load(char* path);
	void SetInfo(MONSTER_MOVE_SPAWN info);
	void MainProc();
private:
	std::map<int,MONSTER_MOVE_INFO> m_MonsterMoveInfo;
};

extern CMonsterMove gMonsterMove;
