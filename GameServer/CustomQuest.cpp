// CustomQuest.cpp: implementation of the CCustomQuest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomQuest.h"
#include "MemScript.h"
#include "Util.h"

CCustomQuest gCustomQuest;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomQuest::CCustomQuest() // OK
{
	this->m_CustomQuestInfo.clear();
}

CCustomQuest::~CCustomQuest() // OK
{

}

void CCustomQuest::Load(char* path) // OK
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

	this->m_CustomQuestInfo.clear();

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

					CUSTOM_QUEST_INFO info;

					info.Index = lpMemScript->GetNumber();

					info.MonsterClass = lpMemScript->GetAsNumber();

					info.RequireMinLevel = lpMemScript->GetAsNumber();

					info.RequireMaxLevel = lpMemScript->GetAsNumber();

					info.RequireMinReset = lpMemScript->GetAsNumber();

					info.RequireMaxReset = lpMemScript->GetAsNumber();

					info.StartMessage = lpMemScript->GetAsNumber();

					info.CompleteMessage = lpMemScript->GetAsNumber();

					for(int n=0;n < MAX_CLASS; n++){info.RequireClass[n] = lpMemScript->GetAsNumber();}

					this->m_CustomQuestInfo.insert(std::pair<int,CUSTOM_QUEST_INFO>(info.Index,info));
				}
				else if(section == 1)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}
				}
				else if(section == 2)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
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

	delete lpMemScript;
}