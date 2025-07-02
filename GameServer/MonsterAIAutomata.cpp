// MonsterAIAutomata.cpp: implementation of the CMonsterAIAutomata class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MonsterAIAutomata.h"
#include "MemScript.h"
#include "Util.h"

CMonsterAIAutomata gMonsterAIAutomata;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMonsterAIAutomata::CMonsterAIAutomata() // OK
{
	this->DelAllAutomata();
}

CMonsterAIAutomata::~CMonsterAIAutomata() // OK
{

}

void CMonsterAIAutomata::Load(char* path) // OK
{
	CMemScript* lpMemScript = new CMemScript;

	if(lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR,path);
		return;
	}

	if(lpMemScript->SetBuffer(path) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	this->DelAllAutomata();

	try
	{
		while(true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			int section = lpMemScript->GetNumber();

			while(true)
			{
				if(section == 0)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					CMonsterAIState info;

					int AutomataNumber = lpMemScript->GetNumber();

					strcpy_s(info.m_StateTransitionDesc,lpMemScript->GetAsString());

					info.m_Priority = lpMemScript->GetAsNumber();

					info.m_CurrentState = lpMemScript->GetAsNumber();

					info.m_NextState = lpMemScript->GetAsNumber();

					info.m_TransitionType = lpMemScript->GetAsNumber();

					info.m_TransitionRate = lpMemScript->GetAsNumber();

					info.m_TransitionValueType = lpMemScript->GetAsNumber();

					info.m_TransitionValue = lpMemScript->GetAsNumber();

					info.m_DelayTime = lpMemScript->GetAsNumber();

					if((AutomataNumber >= 0 && AutomataNumber < MAX_MONSTER_AI_AUTOMATA_INFO) && (info.m_CurrentState >= 0 && info.m_CurrentState < MAX_AI_STATE) && (info.m_Priority >= 0 && info.m_Priority < MAX_AI_PRIORITY))
					{
						this->m_MonsterAIAutomataInfoArray[AutomataNumber].m_AutomataNumber = AutomataNumber;
						this->m_MonsterAIAutomataInfoArray[AutomataNumber].m_AIState[info.m_CurrentState][info.m_Priority] = info;
						this->m_MonsterAIAutomataInfoArray[AutomataNumber].m_AIStateTransCount[info.m_CurrentState]++;
					}
				}
				else
				{
					break;
				}
			}
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	this->m_DataLoad = 1;

	delete lpMemScript;
}

void CMonsterAIAutomata::DelAllAutomata() // OK
{
	this->m_DataLoad = 0;

	for(int n=0;n < MAX_MONSTER_AI_AUTOMATA_INFO;n++)
	{
		this->m_MonsterAIAutomataInfoArray[n].Reset();
	}
}

CMonsterAIAutomataInfo* CMonsterAIAutomata::FindAutomataInfo(int AutomataNumber) // OK
{
	if(AutomataNumber < 0 || AutomataNumber >= MAX_MONSTER_AI_AUTOMATA_INFO)
	{
		return 0;
	}

	if(this->m_MonsterAIAutomataInfoArray[AutomataNumber].m_AutomataNumber == AutomataNumber)
	{
		return &this->m_MonsterAIAutomataInfoArray[AutomataNumber];
	}

	return 0;
}
