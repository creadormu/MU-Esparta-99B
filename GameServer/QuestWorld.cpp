// QuestWorld.cpp: implementation of the CQuestWorld class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuestWorld.h"
#include "DSProtocol.h"
#include "EffectManager.h"
#include "GameMain.h"
#include "MasterSkillTree.h"
#include "MemScript.h"
#include "Monster.h"
#include "ObjectManager.h"
#include "ServerInfo.h"
#include "SkillManager.h"
#include "Util.h"

CQuestWorld gQuestWorld;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestWorld::CQuestWorld() // OK
{
	#if(GAMESERVER_UPDATE>=501)

	this->m_QuestWorldInfo.clear();

	#endif
}

CQuestWorld::~CQuestWorld() // OK
{

}

void CQuestWorld::Load(char* path) // OK
{
	#if(GAMESERVER_UPDATE>=501)

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

	this->m_QuestWorldInfo.clear();

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
				if(strcmp("end",lpMemScript->GetAsString()) == 0)
				{
					break;
				}

				QUEST_WORLD_INFO info;

				info.Group = section+1;

				info.Index = lpMemScript->GetNumber();

				info.Number = lpMemScript->GetAsNumber();

				info.StartType = lpMemScript->GetAsNumber();

				info.MonsterClass = lpMemScript->GetAsNumber();

				info.CurrentState = lpMemScript->GetAsNumber();

				info.NextIndex[1] = lpMemScript->GetAsNumber();

				info.NextIndex[2] = lpMemScript->GetAsNumber();

				info.NextIndex[3] = lpMemScript->GetAsNumber();

				info.RequireIndex = lpMemScript->GetAsNumber();

				info.RequireState = lpMemScript->GetAsNumber();

				info.Start = (info.RequireState == QUEST_WORLD_FINISH) ? 1 : 0;

				info.RequireMinLevel = lpMemScript->GetAsNumber();

				info.RequireMaxLevel = lpMemScript->GetAsNumber();

				info.RequireMinReset = lpMemScript->GetAsNumber();

				info.RequireMaxReset = lpMemScript->GetAsNumber();

				info.RequireFamily = lpMemScript->GetAsNumber();

				info.RequireDoW = lpMemScript->GetAsNumber();

				info.RequireCDW = lpMemScript->GetAsNumber();

				for(int n = 0; n < MAX_CLASS; n++) { info.RequireClass[n] = lpMemScript->GetAsNumber(); }

				this->m_QuestWorldInfo.push_back(info);
			}
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;

	#endif
}

QUEST_WORLD_INFO* CQuestWorld::GetInfoByIndex(LPOBJ lpObj,int QuestIndex,int QuestGroup) // OK
{
	#if(GAMESERVER_UPDATE>=501)

	for(size_t n=0;n < this->m_QuestWorldInfo.size();n++)
	{
		QUEST_WORLD_INFO* lpInfo = &this->m_QuestWorldInfo[n];

		if(lpInfo->Index != QuestIndex || lpInfo->Group != QuestGroup)
		{
			continue;
		}

		if(this->CheckQuestWorldRequisite(lpObj,lpInfo) == 0)
		{
			continue;
		}

		return lpInfo;
	}

	#endif

	return 0;	
}

int CQuestWorld::GetStateByIndex(int QuestIndex,int QuestGroup) // OK
{
	#if(GAMESERVER_UPDATE>=501)

	for each(QUEST_WORLD_INFO lpInfo in this->m_QuestWorldInfo)
	{
		if(lpInfo.Index != QuestIndex || lpInfo.Group != QuestGroup)
		{
			continue;
		}

		return lpInfo.CurrentState;
	}

	#endif

	return 0;
}

bool CQuestWorld::AddQuestWorldList(LPOBJ lpObj,int QuestIndex,int QuestGroup,int QuestNumber,int QuestState,int Start) // OK
{
	#if(GAMESERVER_UPDATE>=501)

	QuestGroup -= 1;

	if(QuestGroup < 0 || QuestGroup >= MAX_QUEST_WORLD_LIST)
	{
		return 0;
	}

	tm timeinfo;

	time_t rawtime;

	time(&rawtime);

	localtime_s(&timeinfo,&rawtime);

	lpObj->QuestWorldList[QuestGroup].QuestIndex = QuestIndex;

	lpObj->QuestWorldList[QuestGroup].QuestStart = ((lpObj->QuestWorldList[QuestGroup].QuestNumber==QuestNumber)?lpObj->QuestWorldList[QuestGroup].QuestStart:QuestIndex);

	lpObj->QuestWorldList[QuestGroup].QuestStart = ((Start==0)?lpObj->QuestWorldList[QuestGroup].QuestStart:QuestIndex);

	lpObj->QuestWorldList[QuestGroup].QuestNumber = QuestNumber;

	lpObj->QuestWorldList[QuestGroup].QuestState = QuestState;

	lpObj->QuestWorldList[QuestGroup].CooldownDay = timeinfo.tm_mday;

	lpObj->QuestWorldList[QuestGroup].CooldownMonth = timeinfo.tm_mon;

	lpObj->QuestWorldList[QuestGroup].CooldownYear = timeinfo.tm_year;

	if(QuestState == QUEST_WORLD_NORMAL || QuestState == QUEST_WORLD_FINISH)
	{
		memset(&lpObj->QuestWorldList[QuestGroup].ObjectiveCount,0x00,sizeof(lpObj->QuestWorldList[QuestGroup].ObjectiveCount));
	}

	return 1;

	#else

	return 0;

	#endif
}

QUEST_WORLD_LIST* CQuestWorld::GetQuestWorldList(LPOBJ lpObj,int QuestIndex,int QuestGroup) // OK
{
	#if(GAMESERVER_UPDATE>=501)

	QuestGroup -= 1;

	if(QuestGroup < 0 || QuestGroup >= MAX_QUEST_WORLD_LIST)
	{
		return 0;
	}

	if(lpObj->QuestWorldList[QuestGroup].QuestIndex != QuestIndex)
	{
		return 0;
	}

	return &lpObj->QuestWorldList[QuestGroup];

	#else

	return 0;

	#endif
}

bool CQuestWorld::CheckQuestWorldRequisite(LPOBJ lpObj,QUEST_WORLD_INFO* lpInfo) // OK
{
	#if(GAMESERVER_UPDATE>=501)

	if(lpInfo->RequireIndex == -1 && lpInfo->Number == lpObj->QuestWorldList[(lpInfo->Group-1)].QuestNumber)
	{
		return 0;
	}

	if(lpInfo->RequireIndex != -1 && this->CheckQuestWorldListState(lpObj,lpInfo->RequireIndex,lpInfo->Group,lpInfo->RequireState) == 0)
	{
		return 0;
	}

	if(lpInfo->RequireMinLevel != -1 && lpInfo->RequireMinLevel > ((gMasterSkillTree.CheckMasterLevel(lpObj) != 0) ? (lpObj->Level+lpObj->MasterLevel) : lpObj->Level))
	{
		return 0;
	}

	if(lpInfo->RequireMaxLevel != -1 && lpInfo->RequireMaxLevel < ((gMasterSkillTree.CheckMasterLevel(lpObj) != 0) ? (lpObj->Level+lpObj->MasterLevel) : lpObj->Level))
	{
		return 0;
	}

	if(lpInfo->RequireMinReset != -1 && lpInfo->RequireMinReset > lpObj->Reset)
	{
		return 0;
	}

	if(lpInfo->RequireMaxReset != -1 && lpInfo->RequireMaxReset < lpObj->Reset)
	{
		return 0;
	}

	if(lpInfo->RequireFamily != -1 && lpInfo->RequireFamily != lpObj->GensFamily)
	{
		return 0;
	}

	if(lpInfo->RequireClass[lpObj->Class] == 0 || lpInfo->RequireClass[lpObj->Class] > (lpObj->ChangeUp+1))
	{
		return 0;
	}

	SYSTEMTIME SystemTime;

	GetLocalTime(&SystemTime);

	if(lpInfo->RequireDoW != -1 && lpInfo->RequireDoW != SystemTime.wDayOfWeek)
	{
		return 0;
	}

	if(lpInfo->RequireCDW != -1)
	{
		tm timeinfo;

		time_t rawtime;

		time(&rawtime);

		localtime_s(&timeinfo,&rawtime);

		tm time_start = {0,0,0,lpObj->QuestWorldList[(lpInfo->Group-1)].CooldownDay,lpObj->QuestWorldList[(lpInfo->Group-1)].CooldownMonth,lpObj->QuestWorldList[(lpInfo->Group-1)].CooldownYear,0,0,0};

		tm time_final = {0,0,0,timeinfo.tm_mday,timeinfo.tm_mon,timeinfo.tm_year,0,0,0};

		time_t make_start = mktime(&time_start);

		time_t make_final = mktime(&time_final);

		if(make_start == -1 || make_final == -1)
		{
			return 0;
		}

		if((difftime(make_final,make_start)/60) < lpInfo->RequireCDW)
		{
			return 0;
		}
	}

	return 1;

	#else

	return 0;

	#endif
}

bool CQuestWorld::CheckQuestWorldListState(LPOBJ lpObj,int QuestIndex,int QuestGroup,int QuestState) // OK
{
	#if(GAMESERVER_UPDATE>=501)

	QUEST_WORLD_LIST* lpQuestWorldList = this->GetQuestWorldList(lpObj,QuestIndex,QuestGroup);

	if(lpQuestWorldList == 0)
	{
		return 0;
	}

	if(lpQuestWorldList->QuestState == QuestState)
	{
		return 1;
	}

	#endif

	return 0;
}

void CQuestWorld::NpcElfBuffer(LPOBJ lpNpc,LPOBJ lpObj) // OK
{
	#if(GAMESERVER_UPDATE>=501)

	lpObj->Interface.use = 1;
	lpObj->Interface.type = INTERFACE_QUEST_WORLD;
	lpObj->Interface.state = 0;
	lpObj->QuestWorldMonsterClass = lpNpc->Class;

	PMSG_QUEST_WORLD_NPC_TALK_SEND pMsg;

	pMsg.header.set(0xF9,0x01,sizeof(pMsg));

	pMsg.MonsterClass = lpNpc->Class;

	pMsg.contribution = 0;

	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);

	#endif
}

bool CQuestWorld::CharacterUseLetter(LPOBJ lpObj) // OK
{
	#if(GAMESERVER_UPDATE>=501)

	for each(QUEST_WORLD_INFO lpInfo in this->m_QuestWorldInfo)
	{
		if(lpInfo.StartType != 2)
		{
			continue;
		}

		if(this->CheckQuestWorldRequisite(lpObj,&lpInfo) == 0)
		{
			continue;
		}

		QUEST_WORLD_LIST* lpQuestWorldList = this->GetQuestWorldList(lpObj,lpInfo.Index,lpInfo.Group);

		if(lpQuestWorldList == 0 && lpInfo.CurrentState == QUEST_WORLD_ACCEPT)
		{
			this->AddQuestWorldList(lpObj,lpInfo.Index,lpInfo.Group,lpInfo.Number,QUEST_WORLD_ACCEPT,0);
			this->GCQuestWorldDetailSend(lpObj,lpInfo.Index,lpInfo.Group,1);
			return 1;
		}
	}

	#endif

	return 0;
}

void CQuestWorld::CGQuestWorldDialogRecv(PMSG_QUEST_WORLD_DIALOG_RECV* lpMsg,int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=501)

	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	QUEST_WORLD_INFO* lpInfo = this->GetInfoByIndex(lpObj,lpMsg->QuestIndex,lpMsg->QuestGroup);

	if(lpInfo == 0)
	{
		return;
	}

	QUEST_WORLD_LIST* lpQuestWorldList = this->GetQuestWorldList(lpObj,lpInfo->Index,lpInfo->Group);

	if(lpQuestWorldList == 0 && lpInfo->CurrentState == QUEST_WORLD_NORMAL)
	{
		this->AddQuestWorldList(lpObj,lpInfo->Index,lpInfo->Group,lpInfo->Number,QUEST_WORLD_NORMAL,lpInfo->Start);
		this->GCQuestWorldAcceptSend(lpObj,lpInfo->Index,lpInfo->Group,lpMsg->QuestOption);
		return;
	}

	if(lpQuestWorldList != 0 && lpQuestWorldList->QuestState == QUEST_WORLD_NORMAL && lpInfo->CurrentState == QUEST_WORLD_NORMAL)
	{
		this->AddQuestWorldList(lpObj,lpInfo->Index,lpInfo->Group,lpInfo->Number,QUEST_WORLD_NORMAL,lpInfo->Start);
		this->GCQuestWorldAcceptSend(lpObj,lpInfo->Index,lpInfo->Group,lpMsg->QuestOption);
		return;
	}

	if(lpQuestWorldList != 0 && lpQuestWorldList->QuestState == QUEST_WORLD_ACCEPT && lpInfo->CurrentState == QUEST_WORLD_ACCEPT)
	{
		this->AddQuestWorldList(lpObj,lpInfo->Index,lpInfo->Group,lpInfo->Number,QUEST_WORLD_ACCEPT,lpInfo->Start);
		this->GCQuestWorldDetailSend(lpObj,lpInfo->Index,lpInfo->Group,lpMsg->QuestOption);
		return;
	}

	if(lpQuestWorldList != 0 && lpQuestWorldList->QuestState == QUEST_WORLD_REJECT && lpInfo->CurrentState == QUEST_WORLD_REJECT)
	{
		this->AddQuestWorldList(lpObj,lpInfo->Index,lpInfo->Group,lpInfo->Number,QUEST_WORLD_REJECT,lpInfo->Start);
		this->GCQuestWorldDetailSend(lpObj,lpInfo->Index,lpInfo->Group,lpMsg->QuestOption);
		return;
	}

	if(lpQuestWorldList != 0 && lpQuestWorldList->QuestState == QUEST_WORLD_FINISH && lpInfo->CurrentState == QUEST_WORLD_NORMAL)
	{
		this->AddQuestWorldList(lpObj,lpQuestWorldList->QuestStart,lpInfo->Group,lpInfo->Number,QUEST_WORLD_NORMAL,lpInfo->Start);
		this->GCQuestWorldAcceptSend(lpObj,lpQuestWorldList->QuestStart,lpInfo->Group,lpMsg->QuestOption);
		return;
	}

	if(lpQuestWorldList != 0 && lpQuestWorldList->QuestState == QUEST_WORLD_FINISH && lpInfo->CurrentState == QUEST_WORLD_ACCEPT)
	{
		this->AddQuestWorldList(lpObj,lpQuestWorldList->QuestStart,lpInfo->Group,lpInfo->Number,QUEST_WORLD_NORMAL,lpInfo->Start);
		this->GCQuestWorldAcceptSend(lpObj,lpQuestWorldList->QuestStart,lpInfo->Group,lpMsg->QuestOption);
		return;
	}

	if(lpQuestWorldList != 0 && lpQuestWorldList->QuestState == QUEST_WORLD_FINISH && lpInfo->CurrentState == QUEST_WORLD_REJECT)
	{
		this->AddQuestWorldList(lpObj,lpQuestWorldList->QuestStart,lpInfo->Group,lpInfo->Number,QUEST_WORLD_NORMAL,lpInfo->Start);
		this->GCQuestWorldAcceptSend(lpObj,lpQuestWorldList->QuestStart,lpInfo->Group,lpMsg->QuestOption);
		return;
	}

	#endif
}

void CQuestWorld::CGQuestWorldAcceptRecv(PMSG_QUEST_WORLD_ACCEPT_RECV* lpMsg,int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=501)

	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	QUEST_WORLD_INFO* lpInfo = this->GetInfoByIndex(lpObj,lpMsg->QuestIndex,lpMsg->QuestGroup);

	if(lpInfo == 0)
	{
		return;
	}

	int NextState = this->GetStateByIndex(lpInfo->NextIndex[lpMsg->QuestOption],lpInfo->Group);

	QUEST_WORLD_LIST* lpQuestWorldList = this->GetQuestWorldList(lpObj,lpInfo->Index,lpInfo->Group);

	if(lpQuestWorldList == 0 && lpInfo->CurrentState == QUEST_WORLD_NORMAL && NextState == QUEST_WORLD_NORMAL)
	{
		this->AddQuestWorldList(lpObj,lpInfo->NextIndex[lpMsg->QuestOption],lpInfo->Group,lpInfo->Number,QUEST_WORLD_NORMAL,lpInfo->Start);
		this->GCQuestWorldAcceptSend(lpObj,lpInfo->NextIndex[lpMsg->QuestOption],lpInfo->Group,lpMsg->QuestOption);
		return;
	}

	if(lpQuestWorldList == 0 && lpInfo->CurrentState == QUEST_WORLD_NORMAL && NextState == QUEST_WORLD_ACCEPT)
	{
		this->AddQuestWorldList(lpObj,lpInfo->NextIndex[lpMsg->QuestOption],lpInfo->Group,lpInfo->Number,QUEST_WORLD_ACCEPT,lpInfo->Start);
		this->GCQuestWorldDetailSend(lpObj,lpInfo->NextIndex[lpMsg->QuestOption],lpInfo->Group,lpMsg->QuestOption);
		return;
	}

	if(lpQuestWorldList == 0 && lpInfo->CurrentState == QUEST_WORLD_NORMAL && NextState == QUEST_WORLD_REJECT)
	{
		this->AddQuestWorldList(lpObj,lpInfo->NextIndex[lpMsg->QuestOption],lpInfo->Group,lpInfo->Number,QUEST_WORLD_REJECT,lpInfo->Start);
		this->GCQuestWorldDetailSend(lpObj,lpInfo->NextIndex[lpMsg->QuestOption],lpInfo->Group,lpMsg->QuestOption);
		return;
	}

	if(lpQuestWorldList != 0 && lpQuestWorldList->QuestState == QUEST_WORLD_NORMAL && lpInfo->CurrentState == QUEST_WORLD_NORMAL && NextState == QUEST_WORLD_NORMAL)
	{
		this->AddQuestWorldList(lpObj,lpInfo->NextIndex[lpMsg->QuestOption],lpInfo->Group,lpInfo->Number,QUEST_WORLD_NORMAL,lpInfo->Start);
		this->GCQuestWorldAcceptSend(lpObj,lpInfo->NextIndex[lpMsg->QuestOption],lpInfo->Group,lpMsg->QuestOption);
		return;
	}

	if(lpQuestWorldList != 0 && lpQuestWorldList->QuestState == QUEST_WORLD_NORMAL && lpInfo->CurrentState == QUEST_WORLD_NORMAL && NextState == QUEST_WORLD_ACCEPT)
	{
		this->AddQuestWorldList(lpObj,lpInfo->NextIndex[lpMsg->QuestOption],lpInfo->Group,lpInfo->Number,QUEST_WORLD_ACCEPT,lpInfo->Start);
		this->GCQuestWorldDetailSend(lpObj,lpInfo->NextIndex[lpMsg->QuestOption],lpInfo->Group,lpMsg->QuestOption);
		return;
	}

	if(lpQuestWorldList != 0 && lpQuestWorldList->QuestState == QUEST_WORLD_NORMAL && lpInfo->CurrentState == QUEST_WORLD_NORMAL && NextState == QUEST_WORLD_REJECT)
	{
		this->AddQuestWorldList(lpObj,lpInfo->NextIndex[lpMsg->QuestOption],lpInfo->Group,lpInfo->Number,QUEST_WORLD_REJECT,lpInfo->Start);
		this->GCQuestWorldDetailSend(lpObj,lpInfo->NextIndex[lpMsg->QuestOption],lpInfo->Group,lpMsg->QuestOption);
		return;
	}

	if(lpQuestWorldList != 0 && lpQuestWorldList->QuestState == QUEST_WORLD_FINISH && lpInfo->CurrentState == QUEST_WORLD_NORMAL && NextState == QUEST_WORLD_NORMAL)
	{
		this->AddQuestWorldList(lpObj,lpInfo->NextIndex[lpMsg->QuestOption],lpInfo->Group,lpInfo->Number,QUEST_WORLD_NORMAL,lpInfo->Start);
		this->GCQuestWorldAcceptSend(lpObj,lpInfo->NextIndex[lpMsg->QuestOption],lpInfo->Group,lpMsg->QuestOption);
		return;
	}

	if(lpQuestWorldList != 0 && lpQuestWorldList->QuestState == QUEST_WORLD_FINISH && lpInfo->CurrentState == QUEST_WORLD_NORMAL && NextState == QUEST_WORLD_ACCEPT)
	{
		this->AddQuestWorldList(lpObj,lpInfo->NextIndex[lpMsg->QuestOption],lpInfo->Group,lpInfo->Number,QUEST_WORLD_ACCEPT,lpInfo->Start);
		this->GCQuestWorldDetailSend(lpObj,lpInfo->NextIndex[lpMsg->QuestOption],lpInfo->Group,lpMsg->QuestOption);
		return;
	}

	if(lpQuestWorldList != 0 && lpQuestWorldList->QuestState == QUEST_WORLD_FINISH && lpInfo->CurrentState == QUEST_WORLD_NORMAL && NextState == QUEST_WORLD_REJECT)
	{
		this->AddQuestWorldList(lpObj,lpInfo->NextIndex[lpMsg->QuestOption],lpInfo->Group,lpInfo->Number,QUEST_WORLD_REJECT,lpInfo->Start);
		this->GCQuestWorldDetailSend(lpObj,lpInfo->NextIndex[lpMsg->QuestOption],lpInfo->Group,lpMsg->QuestOption);
		return;
	}

	#endif
}

void CQuestWorld::CGQuestWorldFinishRecv(PMSG_QUEST_WORLD_FINISH_RECV* lpMsg,int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=501)

	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	QUEST_WORLD_INFO* lpInfo = this->GetInfoByIndex(lpObj,lpMsg->QuestIndex,lpMsg->QuestGroup);

	if(lpInfo == 0)
	{
		return;
	}

	if(gQuestWorldObjective.CheckQuestWorldObjective(lpObj,lpInfo->Index,lpInfo->Group) == 0)
	{
		return;
	}

	QUEST_WORLD_LIST* lpQuestWorldList = this->GetQuestWorldList(lpObj,lpInfo->Index,lpInfo->Group);

	if(lpQuestWorldList != 0 && lpQuestWorldList->QuestState == QUEST_WORLD_ACCEPT && lpInfo->CurrentState == QUEST_WORLD_ACCEPT)
	{
		gQuestWorldObjective.RemoveQuestWorldObjective(lpObj,lpInfo->Index,lpInfo->Group);
		gQuestWorldReward.InsertQuestWorldReward(lpObj,lpInfo->Index,lpInfo->Group);
		this->AddQuestWorldList(lpObj,lpInfo->Index,lpInfo->Group,lpInfo->Number,QUEST_WORLD_FINISH,lpInfo->Start);
		this->GCQuestWorldFinishSend(lpObj,lpInfo->Index,lpInfo->Group,1);
		return;
	}

	if(lpQuestWorldList != 0 && lpQuestWorldList->QuestState == QUEST_WORLD_REJECT && lpInfo->CurrentState == QUEST_WORLD_REJECT)
	{
		gQuestWorldObjective.RemoveQuestWorldObjective(lpObj,lpInfo->Index,lpInfo->Group);
		gQuestWorldReward.InsertQuestWorldReward(lpObj,lpInfo->Index,lpInfo->Group);
		this->AddQuestWorldList(lpObj,lpQuestWorldList->QuestStart,lpInfo->Group,lpInfo->Number,QUEST_WORLD_NORMAL,lpInfo->Start);
		this->GCQuestWorldFinishSend(lpObj,lpInfo->Index,lpInfo->Group,1);
		return;
	}

	#endif
}

void CQuestWorld::CGQuestWorldCancelRecv(PMSG_QUEST_WORLD_CANCEL_RECV* lpMsg,int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=501)

	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	QUEST_WORLD_INFO* lpInfo = this->GetInfoByIndex(lpObj,lpMsg->QuestIndex,lpMsg->QuestGroup);

	if(lpInfo == 0)
	{
		return;
	}

	QUEST_WORLD_LIST* lpQuestWorldList = this->GetQuestWorldList(lpObj,lpInfo->Index,lpInfo->Group);

	this->AddQuestWorldList(lpObj,lpQuestWorldList->QuestStart,lpInfo->Group,lpInfo->Number,QUEST_WORLD_NORMAL,lpInfo->Start);
	
	this->GCQuestWorldCancelSend(lpObj,lpInfo->Index,lpInfo->Group,1);

	#endif
}

void CQuestWorld::CGQuestWorldButtonRecv(PMSG_QUEST_WORLD_BUTTON_RECV* lpMsg,int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=501)

	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	QUEST_WORLD_INFO* lpInfo = this->GetInfoByIndex(lpObj,lpMsg->QuestIndex,lpMsg->QuestGroup);

	if(lpInfo == 0)
	{
		return;
	}

	gQuestWorldObjective.PressButton(lpObj,lpInfo->Index,lpInfo->Group);

	#endif
}

void CQuestWorld::CGQuestWorldDetailRecv(PMSG_QUEST_WORLD_DETAIL_RECV* lpMsg,int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=501)

	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	QUEST_WORLD_INFO* lpInfo = this->GetInfoByIndex(lpObj,lpMsg->QuestIndex,lpMsg->QuestGroup);

	if(lpInfo == 0)
	{
		return;
	}

	QUEST_WORLD_LIST* lpQuestWorldList = this->GetQuestWorldList(lpObj,lpInfo->Index,lpInfo->Group);

	if(lpQuestWorldList == 0)
	{
		return;
	}

	gQuestWorldReward.GCQuestWorldDetailSend(lpObj,lpInfo->Index,lpInfo->Group,0x1B);

	#endif
}

void CQuestWorld::CGQuestWorldListRecv(int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=501)

	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(lpObj->Interface.use == 0 || lpObj->Interface.type != INTERFACE_QUEST_WORLD)
	{
		return;
	}

	BYTE send[256];

	PMSG_QUEST_WORLD_LIST_SEND pMsg;

	pMsg.header.set(0xF6,0x0A,0);

	int size = sizeof(pMsg);

	pMsg.MonsterClass = lpObj->QuestWorldMonsterClass;

	pMsg.count = 0;

	pMsg.result = 0;

	PMSG_QUEST_WORLD_LIST info;

	for each(QUEST_WORLD_INFO lpInfo in this->m_QuestWorldInfo)
	{
		if(lpInfo.StartType != 0)
		{
			continue;
		}

		if(lpInfo.MonsterClass != lpObj->QuestWorldMonsterClass)
		{
			continue;
		}

		if(this->CheckQuestWorldRequisite(lpObj,&lpInfo) == 0)
		{
			continue;
		}

		info.QuestIndex = lpInfo.Index;

		info.QuestGroup = lpInfo.Group;

		memcpy(&send[size],&info,sizeof(info));
		size += sizeof(info);

		pMsg.count++;
	}

	pMsg.header.size = size;

	memcpy(send,&pMsg,sizeof(pMsg));

	DataSend(aIndex,send,size);

	#endif
}

void CQuestWorld::CGQuestWorldNpcElfBufferRecv(int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=501)

	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(lpObj->Interface.use == 0 || lpObj->Interface.type != INTERFACE_QUEST_WORLD)
	{
		return;
	}

	if(lpObj->Level > gServerInfo.m_ElfBufferMaxLevel[lpObj->AccountLevel])
	{
		GCServerCommandSend(lpObj->Index,13,0,0,0);
		return;
	}

	if(lpObj->Reset > gServerInfo.m_ElfBufferMaxReset[lpObj->AccountLevel])
	{
		GCServerCommandSend(lpObj->Index,13,0,0,0);
		return;
	}

	gEffectManager.AddEffect(lpObj,0,EFFECT_ELF_BUFFER,1800,(gServerInfo.m_ElfBufferDamageConstA+(lpObj->Level/gServerInfo.m_ElfBufferDamageConstB)),(gServerInfo.m_ElfBufferDefenseConstA+(lpObj->Level/gServerInfo.m_ElfBufferDefenseConstB)),0,0);

	#endif
}

void CQuestWorld::GCQuestWorldButtonSend(LPOBJ lpObj,int QuestIndex,int QuestGroup,int QuestOption) // OK
{
	#if(GAMESERVER_UPDATE>=501)

	PMSG_QUEST_WORLD_BUTTON_SEND pMsg;

	pMsg.header.set(0xF6,0x03,sizeof(pMsg));

	pMsg.QuestIndex1 = QuestIndex;

	pMsg.QuestGroup1 = QuestGroup;

	for each(QUEST_WORLD_INFO lpInfo in this->m_QuestWorldInfo)
	{
		if(lpInfo.StartType != 1)
		{
			continue;
		}

		if(this->CheckQuestWorldRequisite(lpObj,&lpInfo) == 0)
		{
			continue;
		}

		if(lpObj->QuestWorldList[(lpInfo.Group-1)].QuestNumber != 0xFFFF && lpObj->QuestWorldList[(lpInfo.Group-1)].QuestNumber > lpInfo.Number)
		{
			continue;
		}

		pMsg.QuestIndex2 = lpInfo.Index;

		pMsg.QuestGroup2 = lpInfo.Group;

		DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);

		return;
	}

	#endif
}

void CQuestWorld::GCQuestWorldAcceptSend(LPOBJ lpObj,int QuestIndex,int QuestGroup,int QuestOption) // OK
{
	#if(GAMESERVER_UPDATE>=501)

	PMSG_QUEST_WORLD_ACCEPT_SEND pMsg;

	pMsg.header.set(0xF6,0x0B,sizeof(pMsg));

	pMsg.QuestIndex = QuestIndex;

	pMsg.QuestGroup = QuestGroup;

	pMsg.QuestOption = QuestOption;

	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);

	this->GCQuestWorldButtonSend(lpObj,QuestIndex,QuestGroup,QuestOption);

	#endif
}

void CQuestWorld::GCQuestWorldDetailSend(LPOBJ lpObj,int QuestIndex,int QuestGroup,int QuestOption) // OK
{
	#if(GAMESERVER_UPDATE>=501)

	gQuestWorldReward.GCQuestWorldDetailSend(lpObj,QuestIndex,QuestGroup,0x0C);

	this->GCQuestWorldButtonSend(lpObj,QuestIndex,QuestGroup,QuestOption);

	#endif
}

void CQuestWorld::GCQuestWorldFinishSend(LPOBJ lpObj,int QuestIndex,int QuestGroup,int QuestResult) // OK
{
	#if(GAMESERVER_UPDATE>=501)

	PMSG_QUEST_WORLD_FINISH_SEND pMsg;

	pMsg.header.set(0xF6,0x0D,sizeof(pMsg));

	pMsg.QuestIndex = QuestIndex;

	pMsg.QuestGroup = QuestGroup;

	pMsg.QuestResult = QuestResult;

	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);

	this->GCQuestWorldButtonSend(lpObj,QuestIndex,QuestGroup,QuestResult);

	#endif
}

void CQuestWorld::GCQuestWorldCancelSend(LPOBJ lpObj,int QuestIndex,int QuestGroup,int QuestResult) // OK
{
	#if(GAMESERVER_UPDATE>=501)

	PMSG_QUEST_WORLD_CANCEL_SEND pMsg;

	pMsg.header.set(0xF6,0x0F,sizeof(pMsg));

	pMsg.QuestIndex = QuestIndex;

	pMsg.QuestGroup = QuestGroup;

	pMsg.QuestResult = QuestResult;

	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);

	this->GCQuestWorldButtonSend(lpObj,QuestIndex,QuestGroup,QuestResult);

	#endif
}

void CQuestWorld::GCQuestWorldInterfaceListSend(LPOBJ lpObj) // OK
{
	#if(GAMESERVER_UPDATE>=501)

	BYTE send[256];

	PMSG_QUEST_WORLD_INTERFACE_LIST_SEND pMsg;

	pMsg.header.set(0xF6,0x1A,0);

	int size = sizeof(pMsg);

	pMsg.count = 0;

	PMSG_QUEST_WORLD_INTERFACE_LIST info;

	for(int n=0;n < MAX_QUEST_WORLD_LIST;n++)
	{
		if(lpObj->QuestWorldList[n].QuestIndex == 0xFFFF)
		{
			continue;
		}

		if(lpObj->QuestWorldList[n].QuestState == QUEST_WORLD_FINISH)
		{
			continue;
		}

		info.QuestIndex = lpObj->QuestWorldList[n].QuestIndex;

		info.QuestGroup = n+1;

		memcpy(&send[size],&info,sizeof(info));
		size += sizeof(info);

		pMsg.count++;
	}

	pMsg.header.size = size;

	memcpy(send,&pMsg,sizeof(pMsg));

	DataSend(lpObj->Index,send,size);

	#endif
}

void CQuestWorld::DGQuestWorldRecv(SDHP_QUEST_WORLD_RECV* lpMsg) // OK
{
	#if(GAMESERVER_UPDATE>=501)

	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGQuestWorldRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	if(lpObj->LoadQuestWorld != 0)
	{
		return;
	}

	lpObj->LoadQuestWorld = 1;

	memcpy(lpObj->QuestWorldList,lpMsg->QuestWorldList,sizeof(lpMsg->QuestWorldList));

	this->GCQuestWorldInterfaceListSend(lpObj);

	this->GCQuestWorldButtonSend(lpObj,0,1,0);

	#endif
}

void CQuestWorld::GDQuestWorldSend(int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=501)

	if(gObjIsAccountValid(aIndex,gObj[aIndex].Account) == 0)
	{
		return;
	}

	if(gObj[aIndex].LoadQuestWorld != 0)
	{
		return;
	}

	SDHP_QUEST_WORLD_SEND pMsg;

	pMsg.header.set(0x10,0x00,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

	memcpy(pMsg.name,gObj[aIndex].Name,sizeof(pMsg.name));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);

	#endif
}

void CQuestWorld::GDQuestWorldSaveSend(int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=501)

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->LoadQuestWorld == 0)
	{
		return;
	}

	SDHP_QUEST_WORLD_SAVE_SEND pMsg;

	pMsg.header.set(0x10,0x30,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpObj->Name,sizeof(pMsg.name));

	memcpy(pMsg.QuestWorldList,lpObj->QuestWorldList,sizeof(pMsg.QuestWorldList));

	gDataServerConnection.DataSend((BYTE*)&pMsg,sizeof(pMsg),aIndex);

	#endif
}