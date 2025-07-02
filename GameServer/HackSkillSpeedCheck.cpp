// HackSkillSpeedCheck.cpp: implementation of the CHackSkillSpeedCheck class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HackSkillSpeedCheck.h"
#include "MemScript.h"
#include "Message.h"
#include "Notice.h"
#include "ServerInfo.h"
#include "Log.h"
#include "Util.h"

CHackSkillSpeedCheck gHackSkillSpeedCheck;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHackSkillSpeedCheck::CHackSkillSpeedCheck() // OK
{
	this->m_HackSkillSpeedCheckInfo.clear();
}

CHackSkillSpeedCheck::~CHackSkillSpeedCheck() // OK
{

}

void CHackSkillSpeedCheck::Load(char* path) // OK
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

	this->m_HackSkillSpeedCheckInfo.clear();

	try
	{
		while(true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			if(strcmp("end",lpMemScript->GetString()) == 0)
			{
				break;
			}

			HACK_SKILL_SPEED_CHECK_INFO info;

			info.Index = lpMemScript->GetNumber();

			info.Class = lpMemScript->GetAsNumber();
			
			info.MinLevel = lpMemScript->GetAsNumber();
			
			info.MaxLevel = lpMemScript->GetAsNumber();
			
			info.MinReset = lpMemScript->GetAsNumber();
			
			info.MaxReset = lpMemScript->GetAsNumber();

			info.MinSpeed = lpMemScript->GetAsNumber();

			info.MaxSpeed = lpMemScript->GetAsNumber();

			info.MaxDelay = lpMemScript->GetAsNumber();

			info.MinCount = lpMemScript->GetAsNumber();

			info.MaxCount = lpMemScript->GetAsNumber();

			info.Action = lpMemScript->GetAsNumber();

			this->m_HackSkillSpeedCheckInfo.push_back(info);
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

bool CHackSkillSpeedCheck::CheckSkillSpeed(int aIndex,int index) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	int Speed = ((lpObj->Class == CLASS_DW)?lpObj->MagicSpeed:lpObj->PhysiSpeed);

	for each(HACK_SKILL_SPEED_CHECK_INFO lpInfo in this->m_HackSkillSpeedCheckInfo)
	{
		if(lpInfo.Index != index)
		{
			continue;
		}

		if(lpInfo.Class != -1 && lpInfo.Class != lpObj->Class)
		{
			continue;
		}

		if(lpInfo.MinLevel != -1 && lpInfo.MinLevel > lpObj->Level)
		{
			continue;
		}

		if(lpInfo.MaxLevel != -1 && lpInfo.MaxLevel < lpObj->Level)
		{
			continue;
		}

		if(lpInfo.MinReset != -1 && lpInfo.MinReset > lpObj->Reset)
		{
			continue;
		}

		if(lpInfo.MaxReset != -1 && lpInfo.MaxReset < lpObj->Reset)
		{
			continue;
		}

		if(lpInfo.MinSpeed != -1 && lpInfo.MinSpeed > Speed)
		{
			continue;
		}

		if(lpInfo.MaxSpeed != -1 && lpInfo.MaxSpeed < Speed)
		{
			continue;
		}

		if(this->CheckSkillMaxDelay(aIndex,index,lpInfo.MaxDelay) == 0)
		{
			gObj[aIndex].HackSkillDelay[index] = GetTickCount();
			gObj[aIndex].HackSkillCount[index] = 0;
		}

		if(this->CheckSkillMinCount(aIndex,index,lpInfo.MinCount) == 0)
		{
			if(this->CheckSkillMaxCount(aIndex,index,lpInfo.MaxCount) == 0)
			{
				gLog.Output(LOG_HACK,"[HackSkillSpeedCheck][%s][%s] Skill count error (Index: %d, Speed: %d, Count: [%d][%d], Action: %d)",gObj[aIndex].Account,gObj[aIndex].Name,index,Speed,gObj[aIndex].HackSkillCount[index],lpInfo.MaxCount,lpInfo.Action);

				if(lpInfo.Action == 1)
				{
					GCNewMessageSend(aIndex,0,0,gMessage.GetMessage(719));
				}
				else if(lpInfo.Action == 2)
				{
					gObjUserKill(aIndex);
				}

				return 0;
			}
		}
	}

	return 1;
}

bool CHackSkillSpeedCheck::CheckSkillMaxDelay(int aIndex,int index,int MaxDelay) // OK
{
	return ((MaxDelay==-1)?1:(((GetTickCount()-gObj[aIndex].HackSkillDelay[index])>=((DWORD)MaxDelay))?0:1));
}

bool CHackSkillSpeedCheck::CheckSkillMinCount(int aIndex,int index,int MinCount) // OK
{
	return ((MinCount==-1)?1:(((++gObj[aIndex].HackSkillCount[index])>((DWORD)MinCount))?0:1));
}

bool CHackSkillSpeedCheck::CheckSkillMaxCount(int aIndex,int index,int MaxCount) // OK
{
	return ((MaxCount==-1)?1:((gObj[aIndex].HackSkillCount[index]>((DWORD)MaxCount))?0:1));
}