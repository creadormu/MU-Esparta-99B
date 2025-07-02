// EventSpawnStage.cpp: interface for the CEventSpawnStage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EventSpawnStage.h"
#include "MemScript.h"
#include "Util.h"

CEventSpawnStage gEventSpawnStage;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEventSpawnStage::CEventSpawnStage() // OK
{
	this->m_EventSpawnStageInfo.clear();
}

CEventSpawnStage::~CEventSpawnStage() // OK
{

}

void CEventSpawnStage::Load(char* path) // OK
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

	this->m_EventSpawnStageInfo.clear();

	try
	{
		while (true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			int Event = lpMemScript->GetNumber();

			while (true)
			{
				if(strcmp("end",lpMemScript->GetAsString()) == 0)
				{
					break;
				}

				EVENT_SPAWN_STAGE_INFO info;

				info.Event = Event;

				info.Level = lpMemScript->GetNumber();

				info.Stage = lpMemScript->GetAsNumber();

				info.MonsterClass = lpMemScript->GetAsNumber();

				info.MaxRegenTime = lpMemScript->GetAsNumber();

				this->m_EventSpawnStageInfo.push_back(info);
			}
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}