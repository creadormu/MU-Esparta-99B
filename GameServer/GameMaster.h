// GameMaster.h: interface for the CGameMaster class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "User.h"

struct GAME_MASTER_INFO
{
	char Account[11];
	char Name[11];
	int Level;
};

class CGameMaster
{
public:
	CGameMaster();
	virtual ~CGameMaster();
	void Load(char* path);
	int GetGameMasterLevel(LPOBJ lpObj);
private:
	std::map<std::string,GAME_MASTER_INFO> m_GameMasterInfo;
};

extern CGameMaster gGameMaster;