// ScriptLoader.h: interface for the CScriptLoader class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "CriticalSection.h"

class CScriptLoader
{
public:
	CScriptLoader();
	virtual ~CScriptLoader();
	void Load(char* path);
	void OnReadScript();
	void OnShutScript();
	void OnTimerThread();
	int OnCommandManager(int aIndex,int code,char* arg);
	void OnCommandDone(int aIndex,int code);
	void OnCharacterEntry(int aIndex);
	void OnCharacterClose(int aIndex);
	int OnNpcTalk(int aIndex,int bIndex);
	void OnMonsterDie(int aIndex,int bIndex);
	void OnUserDie(int aIndex,int bIndex);
	void OnUserRespawn(int aIndex,int KillerType);
	int OnCheckUserTarget(int aIndex,int bIndex);
	int OnCheckUserKiller(int aIndex,int bIndex);
	int OnUserItemPick(int aIndex,int slot);
	int OnUserItemDrop(int aIndex,int slot,int x,int y);
	int OnUserItemMove(int aIndex,int aFlag,int aSlot,int bFlag,int bSlot);
private:
	lua_State* m_luaState;
	CCriticalSection m_critical;
};

extern CScriptLoader gScriptLoader;