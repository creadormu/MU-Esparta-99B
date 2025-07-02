// KanturuBattleOfMaya.h: interface for the CKanturuBattleOfMaya class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "KanturuStateInfo.h"
#include "KanturuMaya.h"

#define KANTURU_MAYA_AI_CHANGE_TIME 4
#define KANTURU_MAYA_TIME_INFO 3
#define KANTURU_MAYA_GROUP_NUMBER 3

enum eKanturuMayaState
{
	KANTURU_MAYA_NONE = 0,
	KANTURU_MAYA_STANDBY_1 = 1,
	KANTURU_MAYA_NOTIFY_1 = 2,
	KANTURU_MAYA_START_1 = 3,
	KANTURU_MAYA_1 = 4,
	KANTURU_MAYA_END_1 = 5,
	KANTURU_MAYA_ENDCYCLE_1 = 6,
	KANTURU_MAYA_STANDBY_2 = 7,
	KANTURU_MAYA_START_2 = 8,
	KANTURU_MAYA_2 = 9,
	KANTURU_MAYA_END_2 = 10,
	KANTURU_MAYA_ENDCYCLE_2 = 11,
	KANTURU_MAYA_STANDBY_3 = 12,
	KANTURU_MAYA_START_3 = 13,
	KANTURU_MAYA_3 = 14,
	KANTURU_MAYA_END_3 = 15,
	KANTURU_MAYA_ENDCYCLE_3 = 16,
	KANTURU_MAYA_END = 17,
	KANTURU_MAYA_ENDCYCLE = 18,
	KANTURU_MAYA_MAX,
};

class CKanturuBattleOfMaya
{
public:
	CKanturuBattleOfMaya();
	virtual ~CKanturuBattleOfMaya();
	void ResetAllData();
	void Load(char* path);
	void MainProc();
	void ProcState_NONE();
	void ProcState_STANBY1();
	void ProcState_NOTIFY1();
	void ProcState_START1();
	void ProcState_MAYA1();
	void ProcState_END_MAYA1();
	void ProcState_ENDCYCLE_MAYA1();
	void ProcState_STANBY2();
	void ProcState_START2();
	void ProcState_MAYA2();
	void ProcState_END_MAYA2();
	void ProcState_ENDCYCLE_MAYA2();
	void ProcState_STANBY3();
	void ProcState_START3();
	void ProcState_MAYA3();
	void ProcState_END_MAYA3();
	void ProcState_ENDCYCLE_MAYA3();
	void ProcState_END();
	void ProcState_ENDCYCLE();
	void SetState(int state);
	void SetState_NONE();
	void SetState_STANBY1();
	void SetState_NOTIFY1();
	void SetState_START1();
	void SetState_MAYA1();
	void SetState_END_MAYA1();
	void SetState_ENDCYCLE_MAYA1();
	void SetState_STANBY2();
	void SetState_START2();
	void SetState_MAYA2();
	void SetState_END_MAYA2();
	void SetState_ENDCYCLE_MAYA2();
	void SetState_STANBY3();
	void SetState_START3();
	void SetState_MAYA3();
	void SetState_END_MAYA3();
	void SetState_ENDCYCLE_MAYA3();
	void SetState_END();
	void SetState_ENDCYCLE();
	void CheckStateTime();
	void CheckMayaBattleTime();
	void CheckMayaBattleUser();
	void ResetAIMonster();
	void SetAIMonsterGroup(int GroupNumber);
	void ChangeAI(int GroupNumber);
	void SetBattleOfMayaState(int state);
	void SetBattleSceneState(int scene);
	void SetEntrancePermit(int permit);
	void SetSuccessValue(int success);
	void SetSceneSuccessValue(int success);
	int GetBattleOfMayaState();
	int GetBattleSceneState();
	int GetRemainTime();
	int GetEntrancePermit();
	int GetSuccessValue();
	int GetSceneSuccessValue();
private:
	int m_BattleOfMayaState;
	int m_SceneState;
	CKanturuStateInfo m_BattleTimeInfo[KANTURU_MAYA_TIME_INFO];
	int m_BattleTimeInfoCount;
	CKanturuStateInfo m_StateInfo[KANTURU_MAYA_MAX];
	int m_StateInfoCount;
	int m_EntrancePermit;
	int m_IsSucccess;
	int m_IsSceneSuccess;
	CKanturuMaya m_KanturuMaya;
	int m_MayaHandDieTimeCounter;
	int m_MayaLeftHandObjIndex;
	int m_MayaRightHandObjIndex;
	int m_MayaHandAIChangeTime[KANTURU_MAYA_AI_CHANGE_TIME];
	int m_MayaHandAIChangeTimeCount;
	int m_MayaHandAIAppliedTime;
	int m_MayaHandCurrentAI;
	int m_MayaHandGroupNumber[KANTURU_MAYA_GROUP_NUMBER];
	int m_MayaHAndGroupNumberCount;
	int m_MayaIceStormUsingRate;
};
