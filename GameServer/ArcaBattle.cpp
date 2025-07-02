// ArcaBattle.cpp: implementation of the CArcaBattle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ArcaBattle.h"
#include "EffectManager.h"
#include "GameMain.h"
#include "GensSystem.h"
#include "GuildManager.h"
#include "ItemBagManager.h"
#include "Map.h"
#include "MapServerManager.h"
#include "MemScript.h"
#include "Monster.h"
#include "Notice.h"
#include "NpcTalk.h"
#include "PartyMatching.h"
#include "ServerInfo.h"
#include "ScheduleManager.h"
#include "Util.h"

CArcaBattle gArcaBattle;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CArcaBattle::CArcaBattle() // OK
{

}

CArcaBattle::~CArcaBattle() // OK
{

}

void CArcaBattle::Init() // OK
{
	#if(GAMESERVER_UPDATE>=702)

	if(gServerInfo.m_ArcaBattleEvent == 0)
	{
		this->SetState(AB_STATE_BLANK);
	}
	else
	{
		this->SetState(AB_STATE_EMPTY);
	}

	#endif
}

void CArcaBattle::Load(char* path) // OK
{
	#if(GAMESERVER_UPDATE>=702)

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

	this->m_ArcaBattleStartTime.clear();

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

					ARCA_BATTLE_START_TIME info;

					info.State = lpMemScript->GetNumber();

					info.Year = lpMemScript->GetAsNumber();

					info.Month = lpMemScript->GetAsNumber();

					info.Day = lpMemScript->GetAsNumber();

					info.DayOfWeek = lpMemScript->GetAsNumber();

					info.Hour = lpMemScript->GetAsNumber();

					info.Minute = lpMemScript->GetAsNumber();

					info.Second = lpMemScript->GetAsNumber();

					this->m_ArcaBattleStartTime.push_back(info);
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

	#endif
}

void CArcaBattle::MainProc() // OK
{
	#if(GAMESERVER_UPDATE>=702)

	return;

	if((GetTickCount()-this->m_TickCount) >= 1000)
	{
		this->m_TickCount = GetTickCount();

		this->m_RemainTime = (int)difftime(this->m_TargetTime,time(0));

		switch(this->m_State)
		{
			case AB_STATE_BLANK:
				this->ProcState_BLANK();
				break;
			case AB_STATE_EMPTY:
				this->ProcState_EMPTY();
				break;
			case AB_STATE_SIGN1:
				this->ProcState_SIGN1();
				break;
			case AB_STATE_SIGN2:
				this->ProcState_SIGN2();
				break;
			case AB_STATE_READY:
				this->ProcState_READY();
				break;
			case AB_STATE_PARTY:
				this->ProcState_PARTY();
				break;
			case AB_STATE_START:
				this->ProcState_START();
				break;
			case AB_STATE_CLOSE:
				this->ProcState_CLOSE();
				break;
			case AB_STATE_CLEAN:
				this->ProcState_CLEAN();
				break;
			case AB_STATE_CLEAR:
				this->ProcState_CLEAR();
				break;
		}
	}

	#endif
}

void CArcaBattle::ProcState_BLANK() // OK
{

}

void CArcaBattle::ProcState_EMPTY() // OK
{
	#if(GAMESERVER_UPDATE>=702)

	if(this->m_RemainTime <= 0)
	{
		this->SetState(AB_STATE_SIGN1);
	}

	#endif
}

void CArcaBattle::ProcState_SIGN1() // OK
{
	#if(GAMESERVER_UPDATE>=702)

	if(this->m_RemainTime <= 0)
	{
		this->SetState(AB_STATE_SIGN2);
	}

	#endif
}

void CArcaBattle::ProcState_SIGN2() // OK
{
	#if(GAMESERVER_UPDATE>=702)

	if(this->m_RemainTime <= 0)
	{
		this->SetState(AB_STATE_READY);
	}

	#endif
}

void CArcaBattle::ProcState_READY() // OK
{
	#if(GAMESERVER_UPDATE>=702)

	if(this->m_RemainTime <= 0)
	{
		this->SetState(AB_STATE_PARTY);
	}

	#endif
}

void CArcaBattle::ProcState_PARTY() // OK
{
	#if(GAMESERVER_UPDATE>=702)

	if(this->m_RemainTime <= 0)
	{
		this->SetState(AB_STATE_START);
	}

	#endif
}

void CArcaBattle::ProcState_START() // OK
{
	#if(GAMESERVER_UPDATE>=702)

	if(this->m_RemainTime <= 0)
	{
		this->SetState(AB_STATE_CLOSE);
	}

	#endif
}

void CArcaBattle::ProcState_CLOSE() // OK
{
	#if(GAMESERVER_UPDATE>=702)

	if(this->m_RemainTime <= 0)
	{
		this->SetState(AB_STATE_CLEAN);
	}

	#endif
}

void CArcaBattle::ProcState_CLEAN() // OK
{
	#if(GAMESERVER_UPDATE>=702)

	if(this->m_RemainTime <= 0)
	{
		this->SetState(AB_STATE_CLEAR);
	}

	#endif
}

void CArcaBattle::ProcState_CLEAR() // OK
{
	#if(GAMESERVER_UPDATE>=702)

	if(this->m_RemainTime <= 0)
	{
		this->SetState(AB_STATE_EMPTY);
	}

	#endif
}

void CArcaBattle::SetState(int state) // OK
{
	#if(GAMESERVER_UPDATE>=702)

	switch((this->m_State=state))
	{
		case AB_STATE_BLANK:
			this->SetState_BLANK();
			break;
		case AB_STATE_EMPTY:
			this->SetState_EMPTY();
			break;
		case AB_STATE_SIGN1:
			this->SetState_SIGN1();
			break;
		case AB_STATE_SIGN2:
			this->SetState_SIGN2();
			break;
		case AB_STATE_READY:
			this->SetState_READY();
			break;
		case AB_STATE_PARTY:
			this->SetState_PARTY();
			break;
		case AB_STATE_START:
			this->SetState_START();
			break;
		case AB_STATE_CLOSE:
			this->SetState_CLOSE();
			break;
		case AB_STATE_CLEAN:
			this->SetState_CLEAN();
			break;
		case AB_STATE_CLEAR:
			this->SetState_CLEAR();
			break;
	}

	#endif
}

void CArcaBattle::SetState_BLANK() // OK
{

}

void CArcaBattle::SetState_EMPTY() // OK
{
	#if(GAMESERVER_UPDATE>=702)

	this->CheckSync();

	LogAdd(LOG_BLACK,"[Arca Battle] SetState EMPTY");

	#endif
}

void CArcaBattle::SetState_SIGN1() // OK
{
	#if(GAMESERVER_UPDATE>=702)

	LogAdd(LOG_BLACK,"[Arca Battle] SetState SIGN1");

	#endif
}

void CArcaBattle::SetState_SIGN2() // OK
{
	#if(GAMESERVER_UPDATE>=702)

	LogAdd(LOG_BLACK,"[Arca Battle] SetState SIGN2");

	#endif
}

void CArcaBattle::SetState_READY() // OK
{
	#if(GAMESERVER_UPDATE>=702)

	LogAdd(LOG_BLACK,"[Arca Battle] SetState READY");

	#endif
}

void CArcaBattle::SetState_PARTY() // OK
{
	#if(GAMESERVER_UPDATE>=702)

	LogAdd(LOG_BLACK,"[Arca Battle] SetState PARTY");

	#endif
}

void CArcaBattle::SetState_START() // OK
{
	#if(GAMESERVER_UPDATE>=702)

	LogAdd(LOG_BLACK,"[Arca Battle] SetState START");

	#endif
}

void CArcaBattle::SetState_CLOSE() // OK
{
	#if(GAMESERVER_UPDATE>=702)

	LogAdd(LOG_BLACK,"[Arca Battle] SetState CLOSE");

	#endif
}

void CArcaBattle::SetState_CLEAN() // OK
{
	#if(GAMESERVER_UPDATE>=702)

	LogAdd(LOG_BLACK,"[Arca Battle] SetState CLEAN");

	#endif
}

void CArcaBattle::SetState_CLEAR() // OK
{
	#if(GAMESERVER_UPDATE>=702)

	LogAdd(LOG_BLACK,"[Arca Battle] SetState CLEAR");

	#endif
}

int CArcaBattle::GetState() // OK
{
	#if(GAMESERVER_UPDATE>=702)

	return this->m_State;

	#else

	return 0;

	#endif
}

void CArcaBattle::CheckSync() // OK
{
	#if(GAMESERVER_UPDATE>=702)

	if(this->m_ArcaBattleStartTime.empty() != 0)
	{
		this->SetState(AB_STATE_BLANK);
		return;
	}

	CTime ScheduleTime;

	CScheduleManager ScheduleManager;

	for(std::vector<ARCA_BATTLE_START_TIME>::iterator it=this->m_ArcaBattleStartTime.begin();it != this->m_ArcaBattleStartTime.end();it++)
	{
		ScheduleManager.AddSchedule(it->Year,it->Month,it->Day,it->Hour,it->Minute,it->Second,it->DayOfWeek);
	}

	if(ScheduleManager.GetSchedule(&ScheduleTime) == 0)
	{
		this->SetState(AB_STATE_BLANK);
		return;
	}

	this->m_RemainTime = (int)difftime(ScheduleTime.GetTime(),time(0));

	this->m_TargetTime = (int)ScheduleTime.GetTime();

	LogAdd(LOG_BLACK,"[Arca Battle] Sync Start Time. [%d] min remain",(this->m_RemainTime/60));

	#endif
}

void CArcaBattle::UserDieProc(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	#if(GAMESERVER_UPDATE>=702)

	if(this->GetState() != AB_STATE_START)
	{
		return;
	}

	gEffectManager.AddEffect(lpObj,0,EFFECT_ARCA_BATTLE_PENALTY,90,0,0,0,0);

	#endif
}

int CArcaBattle::MonsterItemDrop(LPOBJ lpMonster) // OK
{
	#if(GAMESERVER_UPDATE>=702)

	int aIndex = gObjMonsterGetTopHitDamageUser(lpMonster);

	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	#endif

	return 0;
}

void CArcaBattle::CGArcaBattleGuildMasterRegisterRecv(int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=702)

	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(this->GetState() != AB_STATE_SIGN1)
	{
		this->GCArcaBattleMasterRegisterResultSend(aIndex,ARCA_BATTLE_RESULT_MASTER_REGISTER_PERIOD_ENDED);
		return;
	}

	if(lpObj->Guild == 0 || lpObj->GuildRank != GUILD_RANK_MASTER)
	{
		this->GCArcaBattleMasterRegisterResultSend(aIndex,ARCA_BATTLE_RESULT_MASTER_REGISTER_ONLY_GUILD_MASTER);
		return;
	}

	if(lpObj->Guild->GetMemberCount() < gServerInfo.m_ArcaBattleMinGuildMember)
	{
		this->GCArcaBattleMasterRegisterResultSend(aIndex,ARCA_BATTLE_RESULT_MASTER_REGISTER_LOW_GUILD_MEMBERS);
		return;
	}

	SDHP_ARCA_BATTLE_MASTER_REGISTER_SEND pMsg;

	pMsg.header.set(0x1D,0x00,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	memcpy(pMsg.GuildName,lpObj->Guild->Name,sizeof(pMsg.GuildName));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,lpObj->Index);

	#endif
}

void CArcaBattle::CGArcaBattleGuildMemberRegisterRecv(int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=702)

	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(this->GetState() != AB_STATE_SIGN2)
	{
		this->GCArcaBattleMemberRegisterResultSend(aIndex,ARCA_BATTLE_RESULT_MEMBER_REGISTER_PERIOD_ENDED);
		return;
	}

	if(lpObj->Guild == 0)
	{
		this->GCArcaBattleMemberRegisterResultSend(aIndex,ARCA_BATTLE_RESULT_MEMBER_REGISTER_NOT_IN_GUILD);
		return;
	}

	if(lpObj->GuildRank == GUILD_RANK_MASTER)
	{
		this->GCArcaBattleMemberRegisterResultSend(aIndex,ARCA_BATTLE_RESULT_MEMBER_REGISTER_ONLY_MEMBERS);
		return;
	}

	SDHP_ARCA_BATTLE_MEMBER_REGISTER_SEND pMsg;

	pMsg.header.set(0x1D,0x01,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpObj->Name,sizeof(pMsg.name));

	memcpy(pMsg.GuildName,lpObj->Guild->Name,sizeof(pMsg.GuildName));

	pMsg.MaxGuildMember = gServerInfo.m_ArcaBattleMaxGuildMember;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,lpObj->Index);

	#endif
}

void CArcaBattle::CGArcaBattleEnterRecv(PMSG_ARCA_BATTLE_ENTER_RESULT_RECV* lpMsg,int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=702)

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->RegenOk > 0)
	{
		return;
	}

	if(lpObj->Teleport != 0)
	{
		return;
	}

	if(this->GetState() != AB_STATE_PARTY && this->GetState() != AB_STATE_START)
	{
		this->GCArcaBattleEnterResultSend(aIndex,ARCA_BATTLE_RESULT_ENTER_PERIOD_ENDED,0);
		return;
	}

	if(lpObj->UsePartyMatching != 0 || lpObj->UsePartyMatchingJoin != 0)
	{
		PMSG_PARTY_MATCHING_ERROR_SEND pMsg;

		pMsg.header.set(0xEF,0x09,sizeof(pMsg));

		pMsg.result = -1;

		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);

		return;
	}

	if(lpMsg->result == 0)
	{
		this->GCArcaBattleEnterResultSend(aIndex,0,53);
		return;
	}

	SDHP_ARCA_BATTLE_ENTER_SEND pMsg;

	pMsg.header.set(0x1D,0x02,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpObj->Name,sizeof(pMsg.name));

	memcpy(pMsg.GuildName,lpObj->Guild->Name,sizeof(pMsg.GuildName));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,lpObj->Index);

	#endif
}

void CArcaBattle::CGArcaBattleTrophiesRecv(int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=702)

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->PShopOpen != 0)
	{
		return;
	}

	lpObj->Interface.use = 1;
	lpObj->Interface.type = INTERFACE_CHAOS_BOX;
	lpObj->Interface.state = 0;

	lpObj->IsChaosMixCompleted = 0;

	gObjInventoryTransaction(lpObj->Index);

	#endif
}

void CArcaBattle::CGArcaBattleMemberCountRecv(int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=702)

	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(lpObj->Guild == 0)
	{
		this->GCArcaBattleMemberCountResultSend(aIndex,ARCA_BATTLE_RESULT_MEMBER_REGISTER_NOT_IN_GUILD,0);
		return;
	}
	
	SDHP_ARCA_BATTLE_MEMBER_COUNT_SEND pMsg;

	pMsg.header.set(0x1D,0x03,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	memcpy(pMsg.GuildName,lpObj->Guild->Name,sizeof(pMsg.GuildName));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,lpObj->Index);

	#endif
}

void CArcaBattle::CGArcaBattleRegisterMarkRecv(int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=702)

	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(lpObj->Guild == 0)
	{
		this->GCArcaBattleRegisterMarkResultSend(aIndex,ARCA_BATTLE_RESULT_MEMBER_REGISTER_NOT_IN_GUILD);
		return;
	}

	if(lpObj->GuildRank != GUILD_RANK_BATTLE_MASTER && lpObj->GuildRank != GUILD_RANK_OFFICE_MASTER && lpObj->GuildRank != GUILD_RANK_MASTER)
	{
		this->GCArcaBattleRegisterMarkResultSend(aIndex,ARCA_BATTLE_RESULT_SIGN_REGISTER_NOT_ALLOWED);
		return;
	}

	if(this->GetState() != AB_STATE_SIGN1 && this->GetState() != AB_STATE_SIGN2)
	{
		this->GCArcaBattleRegisterMarkResultSend(aIndex,ARCA_BATTLE_RESULT_MASTER_REGISTER_PERIOD_ENDED);
		return;
	}

	if(lpObj->Guild->GetMemberCount() < gServerInfo.m_ArcaBattleMinGuildMember)
	{
		this->GCArcaBattleRegisterMarkResultSend(aIndex,ARCA_BATTLE_RESULT_SIGN_REGISTER_LOW_MEMBERS);
		return;
	}

	SDHP_ARCA_BATTLE_REGISTER_MARK_SEND pMsg;

	pMsg.header.set(0x1D,0x04,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	memcpy(pMsg.GuildName,lpObj->Guild->Name,sizeof(pMsg.GuildName));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,lpObj->Index);

	#endif
}

void CArcaBattle::CGArcaBattleMarkRankRecv(int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=702)

	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	SDHP_ARCA_BATTLE_MARK_RANK_SEND pMsg;

	pMsg.header.set(0x1D,0x05,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	if(lpObj->Guild == 0)
	{
		memset(pMsg.GuildName,0,sizeof(pMsg.GuildName));
	}
	else
	{
		memcpy(pMsg.GuildName,lpObj->Guild->Name,sizeof(pMsg.GuildName));
	}

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,lpObj->Index);

	#endif
}

void CArcaBattle::GCArcaBattleMasterRegisterResultSend(int aIndex,int result) // OK
{
	#if(GAMESERVER_UPDATE>=702)

	PMSG_ARCA_BATTLE_MASTER_REGISTER_RESULT_SEND pMsg;

	pMsg.header.set(0xF8,0x31,sizeof(pMsg));

	pMsg.result = result;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);

	#endif
}

void CArcaBattle::GCArcaBattleMemberRegisterResultSend(int aIndex,int result) // OK
{
	#if(GAMESERVER_UPDATE>=702)

	PMSG_ARCA_BATTLE_MEMBER_REGISTER_RESULT_SEND pMsg;

	pMsg.header.set(0xF8,0x33,sizeof(pMsg));

	pMsg.result = result;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);

	#endif
}

void CArcaBattle::GCArcaBattleEnterResultSend(int aIndex, int result, int data) // OK
{
	#if(GAMESERVER_UPDATE>=702)

	PMSG_ARCA_BATTLE_ENTER_RESULT_SEND pMsg;

	pMsg.header.set(0xF8,0x35,sizeof(pMsg));

	pMsg.result = result;

	pMsg.data = data;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);

	#endif
}

void CArcaBattle::GCArcaBattleMemberCountResultSend(int aIndex,int result,int count) // OK
{
	#if(GAMESERVER_UPDATE>=702)

	PMSG_ARCA_BATTLE_MEMBER_COUNT_SEND pMsg;

	pMsg.header.set(0xF8,0x42,sizeof(pMsg));

	pMsg.result = result;

	pMsg.count = count;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);

	#endif
}

void CArcaBattle::GCArcaBattleRegisterMarkResultSend(int aIndex,int result) // OK
{
	#if(GAMESERVER_UPDATE>=702)

	LPOBJ lpObj = &gObj[aIndex];

	PMSG_ARCA_BATTLE_REGISTER_MARK_RESULT_SEND pMsg;

	pMsg.header.set(0xF8,0x47,sizeof(pMsg));

	pMsg.result = result;
	
	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);

	#endif
}

void CArcaBattle::GDArcaBattleMasterRegisterSaveSend(int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=702)

	LPOBJ lpObj = &gObj[aIndex];

	SDHP_ARCA_BATTLE_MASTER_REGISTER_SAVE_SEND pMsg;

	pMsg.header.set(0x1D,0x30,sizeof(pMsg));

	memcpy(pMsg.GuildName,lpObj->Guild->Name,sizeof(pMsg.GuildName));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);

	#endif
}

void CArcaBattle::GDArcaBattleMemberRegisterSaveSend(int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=702)

	LPOBJ lpObj = &gObj[aIndex];

	SDHP_ARCA_BATTLE_MEMBER_REGISTER_SAVE_SEND pMsg;

	pMsg.header.set(0x1D,0x31,sizeof(pMsg));

	memcpy(pMsg.GuildName,lpObj->Guild->Name,sizeof(pMsg.GuildName));

	memcpy(pMsg.name,lpObj->Name,sizeof(pMsg.name));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);

	#endif
}

void CArcaBattle::GDArcaBattleRegisterMarkSend(int aIndex,int sign) // OK
{
	#if(GAMESERVER_UPDATE>=702)

	LPOBJ lpObj = &gObj[aIndex];

	SDHP_ARCA_BATTLE_REGISTER_MARK_SAVE_SEND pMsg;

	pMsg.header.set(0x1D,0x32,sizeof(pMsg));

	memcpy(pMsg.GuildName,lpObj->Guild->Name,sizeof(pMsg.GuildName));

	pMsg.sign = sign;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);

	#endif
}

void CArcaBattle::DGArcaBattleMasterRegisterRecv(SDHP_ARCA_BATTLE_MASTER_REGISTER_RESULT_RECV* lpMsg) // OK
{
	#if(GAMESERVER_UPDATE>=702)

	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGArcaBattleMasterRegisterRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	switch (lpMsg->result)
	{
	case 0:
		this->GCArcaBattleMasterRegisterResultSend(lpMsg->index,ARCA_BATTLE_RESULT_REGISTER_SUCCESS);
		this->GDArcaBattleMasterRegisterSaveSend(lpMsg->index);
		this->GDArcaBattleMemberRegisterSaveSend(lpMsg->index);
		break;
	case 1:
		this->GCArcaBattleMasterRegisterResultSend(lpMsg->index,ARCA_BATTLE_RESULT_MASTER_REGISTER_ALREADY_REGISTERED);
		break;
	}

	#endif
}

void CArcaBattle::DGArcaBattleMemberRegisterRecv(SDHP_ARCA_BATTLE_MEMBER_REGISTER_RESULT_RECV* lpMsg) // OK
{
	#if(GAMESERVER_UPDATE>=702)

	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGArcaBattleMemberRegisterRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	switch (lpMsg->result)
	{
	case 0:
		this->GCArcaBattleMemberRegisterResultSend(lpMsg->index,ARCA_BATTLE_RESULT_REGISTER_SUCCESS);
		this->GDArcaBattleMemberRegisterSaveSend(lpMsg->index);
		break;
	case 1:
		this->GCArcaBattleMemberRegisterResultSend(lpMsg->index,ARCA_BATTLE_RESULT_MEMBER_REGISTER_ALREADY_REGISTERED);
		break;
	case 2:
		this->GCArcaBattleMemberRegisterResultSend(lpMsg->index,ARCA_BATTLE_RESULT_MEMBER_REGISTER_NOT_IN_GUILD);
		break;
	case 3:
		this->GCArcaBattleMemberRegisterResultSend(lpMsg->index,ARCA_BATTLE_RESULT_MEMBER_REGISTER_MAXIMUM_MEMBERS);
		break;
	}

	#endif
}

void CArcaBattle::DGArcaBattleEnterRecv(SDHP_ARCA_BATTLE_ENTER_RECV* lpMsg) // OK
{
	#if(GAMESERVER_UPDATE>=702)

	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGArcaBattleEnterRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	switch (lpMsg->result)
	{
	case 0:
		gObjMoveGate(lpMsg->index,426);
		break;
	case 1:
		this->GCArcaBattleEnterResultSend(lpMsg->index,ARCA_BATTLE_RESULT_ENTER_NOT_REGISTERED,0);
		break;
	}

	#endif
}

void CArcaBattle::DGArcaBattleMemberCountRecv(SDHP_ARCA_BATTLE_MEMBER_COUNT_RECV* lpMsg) // OK
{
	#if(GAMESERVER_UPDATE>=702)

	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGArcaBattleGuildMemberCountRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	switch (lpMsg->result)
	{
	case 0:
		this->GCArcaBattleMemberCountResultSend(lpMsg->index,lpMsg->result,lpMsg->count);
		break;
	case 1:
		this->GCArcaBattleMemberCountResultSend(lpMsg->index,2,lpMsg->count);
		break;
	}

	#endif
}

void CArcaBattle::DGArcaBattleRegisterMarkRecv(SDHP_ARCA_BATTLE_REGISTER_MARK_RECV* lpMsg) // OK
{
	#if(GAMESERVER_UPDATE>=702)

	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGArcaBattleRegisterMarkRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	if(lpMsg->result == 1)
	{
		this->GCArcaBattleRegisterMarkResultSend(lpMsg->index,ARCA_BATTLE_RESULT_ENTER_NOT_REGISTERED);
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	if(lpObj->PShopOpen != 0)
	{
		return;
	}

	lpObj->Interface.use = 1;
	lpObj->Interface.type = INTERFACE_CHAOS_BOX;
	lpObj->Interface.state = 0;

	lpObj->IsChaosMixCompleted = 0;

	gObjInventoryTransaction(lpObj->Index);

	PMSG_ARCA_BATTLE_REGISTER_MARK_SEND pMsg;

	pMsg.header.set(0xF8,0x44,sizeof(pMsg));

	pMsg.sign[0] = SET_NUMBERHB(SET_NUMBERHW(lpMsg->sign));
	pMsg.sign[1] = SET_NUMBERLB(SET_NUMBERHW(lpMsg->sign));
	pMsg.sign[2] = SET_NUMBERHB(SET_NUMBERLW(lpMsg->sign));
	pMsg.sign[3] = SET_NUMBERLB(SET_NUMBERLW(lpMsg->sign));

	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);

	#endif
}

void CArcaBattle::DGArcaBattleMarkRankRecv(SDHP_ARCA_BATTLE_MARK_RANK_RECV* lpMsg) // OK
{
	#if(GAMESERVER_UPDATE>=702)

	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGArcaBattleMarkRankRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	BYTE send[256];

	PMSG_ARCA_BATTLE_MARK_RANK_SEND pMsg;

	pMsg.header.set(0xF8,0x46,0);

	int size = sizeof(pMsg);

	pMsg.rank = lpMsg->rank;

	pMsg.sign[0] = SET_NUMBERHB(SET_NUMBERHW(lpMsg->sign));
	pMsg.sign[1] = SET_NUMBERLB(SET_NUMBERHW(lpMsg->sign));
	pMsg.sign[2] = SET_NUMBERHB(SET_NUMBERLW(lpMsg->sign));
	pMsg.sign[3] = SET_NUMBERLB(SET_NUMBERLW(lpMsg->sign));

	pMsg.count = 0;

	PMSG_ARCA_BATTLE_MARK_RANK info;

	for(int n=0;n < lpMsg->count;n++)
	{
		SDHP_ARCA_BATTLE_MARK_RANK* lpInfo = (SDHP_ARCA_BATTLE_MARK_RANK*)(((BYTE*)lpMsg)+sizeof(SDHP_ARCA_BATTLE_MARK_RANK_RECV)+(sizeof(SDHP_ARCA_BATTLE_MARK_RANK)*n));

		info.rank = lpInfo->rank;

		memcpy(info.GuildName,lpInfo->GuildName,sizeof(info.GuildName));
		
		info.sign[0] = SET_NUMBERHB(SET_NUMBERHW(lpInfo->sign));
		info.sign[1] = SET_NUMBERLB(SET_NUMBERHW(lpInfo->sign));
		info.sign[2] = SET_NUMBERHB(SET_NUMBERLW(lpInfo->sign));
		info.sign[3] = SET_NUMBERLB(SET_NUMBERLW(lpInfo->sign));

		memcpy(&send[size],&info,sizeof(info));
		size += sizeof(info);

		pMsg.count++;
	}

	pMsg.header.size = size;

	memcpy(send,&pMsg,sizeof(pMsg));

	DataSend(lpObj->Index,send,size);

	#endif
}