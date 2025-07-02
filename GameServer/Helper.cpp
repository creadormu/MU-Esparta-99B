// Helper.cpp: implementation of the CHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Helper.h"
#include "CastleSiege.h"
#include "GameMain.h"
#include "Map.h"
#include "MapManager.h"
#include "Message.h"
#include "Notice.h"
#include "ServerInfo.h"
#include "Util.h"

CHelper gHelper;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHelper::CHelper() // OK
{

}

CHelper::~CHelper() // OK
{

}

void CHelper::MainProc() // OK
{
	#if(GAMESERVER_UPDATE>=603)

	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObjIsConnected(n) == 0)
		{
			continue;
		}

		LPOBJ lpObj = &gObj[n];

		if(lpObj->Helper.Started == 0 || lpObj->Helper.Offline != 0)
		{
			continue;
		}

		if(gMapManager.GetMapHelperEnable(lpObj->Map) == 0)
		{
			this->DisableHelper(lpObj,1);
			continue;
		}

		if(gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,1) != 0)
		{
			this->DisableHelper(lpObj,1);
			continue;
		}

		DWORD time = GetTickCount();

		if((time-lpObj->Helper.TimerMoney) >= ((DWORD)(gServerInfo.m_HelperActiveDelay*60000)))
		{
			DWORD money = (lpObj->Level+lpObj->MasterLevel)*gServerInfo.m_HelperActiveMoney[lpObj->Helper.Stage];

			if(lpObj->Money < money)
			{
				this->DisableHelper(lpObj,2);
				continue;
			}

			lpObj->Helper.TimerMoney = time;

			lpObj->Money -= money;

			GCMoneySend(lpObj->Index,lpObj->Money);

			this->GCHelperStartSend(lpObj->Index,((time-lpObj->Helper.TimerStage)/60000),money,0);
		}

		if((time-lpObj->Helper.TimerStage) > 12000000)
		{
			lpObj->Helper.TimerMoney = time;

			lpObj->Helper.TimerStage = time;

			if((++lpObj->Helper.Stage) >= MAX_HELPER_STAGE)
			{
				this->DisableHelper(lpObj,1);
				continue;
			}
		}
	}

	#endif
}

void CHelper::EnableHelper(LPOBJ lpObj) // OK
{
	#if(GAMESERVER_UPDATE>=603)

	if(lpObj->Helper.Started != 0)
	{
		return;
	}

	if(gMapManager.GetMapHelperEnable(lpObj->Map) == 0)
	{
		return;
	}

	if(gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,1) != 0)
	{
		return;
	}

	if(lpObj->Level < gServerInfo.m_HelperActiveLevel)
	{
		return;
	}

	DWORD money = (lpObj->Level+lpObj->MasterLevel)*gServerInfo.m_HelperActiveMoney[0];

	if(lpObj->Money > money)
	{
		lpObj->Money -= money;

		GCMoneySend(lpObj->Index,lpObj->Money);

		lpObj->Helper.Stage = 0;

		lpObj->Helper.TimerMoney = GetTickCount();

		lpObj->Helper.TimerStage = GetTickCount();

		lpObj->Helper.StartX = lpObj->X;

		lpObj->Helper.StartY = lpObj->Y;

		lpObj->Helper.Started = 1;

		this->GCHelperStartSend(lpObj->Index,0,money,0);
	}
	else
	{
		this->GCHelperStartSend(lpObj->Index,0,0,2);
	}

	#endif
}

void CHelper::DisableHelper(LPOBJ lpObj,int result) // OK
{
	#if(GAMESERVER_UPDATE>=603)

	if(lpObj->Helper.Started == 0)
	{
		return;
	}

	lpObj->Helper.Started = 0;

	this->GCHelperStartSend(lpObj->Index,0,0,result);

	#endif
}

void CHelper::HelperInfoData(int aIndex,HELPER_INFO* lpMsg) // OK
{
	#if(GAMESERVER_UPDATE>=603)

	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	lpObj->Helper.Range[0] = lpMsg->Range & 0x0F;
	lpObj->Helper.MoveTime = lpMsg->MovementTime;
	lpObj->Helper.Skill[0] = lpMsg->SkillAttack1;
	lpObj->Helper.Skill[1] = lpMsg->SkillAttack2;
	lpObj->Helper.Skill[2] = lpMsg->SkillAttack3;
	lpObj->Helper.Delay[0] = lpMsg->SkillDelay1;
	lpObj->Helper.Delay[1] = lpMsg->SkillDelay2;
	lpObj->Helper.Buff[0] = lpMsg->SkillBuff[0];
	lpObj->Helper.Buff[1] = lpMsg->SkillBuff[1];
	lpObj->Helper.Buff[2] = lpMsg->SkillBuff[2];
	lpObj->Helper.PotPercent = 10*(lpMsg->PercentFlag & 0x0F);
	lpObj->Helper.HealPercent = 10*((lpMsg->PercentFlag >> 4) & 0x0F);
	lpObj->Helper.DrainPercent = 10*((lpMsg->PercentFlag >> 8) & 0x0F);
	lpObj->Helper.PartyHealPercent = 10*((lpMsg->PercentFlag >> 12) & 0x0F);
	lpObj->Helper.PartyBuffTime = lpMsg->BuffPartyTime;
	
	lpObj->Helper.Range[1] = (lpMsg->Range >> 4) & 0x0F;
	lpObj->Helper.PickJewel = (lpMsg->PickFlag >> 3) & 1;
	lpObj->Helper.PickExc = (lpMsg->PickFlag >> 4) & 1;
	lpObj->Helper.PickSet = (lpMsg->PickFlag >> 5) & 1;
	lpObj->Helper.PickMoney = (lpMsg->PickFlag >> 6) & 1;
	lpObj->Helper.PickExtra = (lpMsg->PickFlag >> 7) & 1;

	memset(lpObj->Helper.ItemList,0,sizeof(lpObj->Helper.ItemList));
	
	for(int n=0;n < MAX_HELPER_ITEM;n++) 
	{
		memcpy(lpObj->Helper.ItemList[n],CharToLower(lpMsg->ItemList[n]),sizeof(lpObj->Helper.ItemList[n]));
	}

	lpObj->Helper.AutoPotion = lpMsg->OptionFlag & 1;
	lpObj->Helper.AutoHeal = (lpMsg->OptionFlag >> 1) & 1;
	lpObj->Helper.AutoDrainLife = (lpMsg->OptionFlag >> 2) & 1;
	lpObj->Helper.LongDistance = (lpMsg->OptionFlag >> 3) & 1;
	lpObj->Helper.OriginalPosition = (lpMsg->OptionFlag >> 4) & 1;
	lpObj->Helper.Combo = (lpMsg->OptionFlag >> 5) & 1;
	lpObj->Helper.Party = (lpMsg->OptionFlag >> 6) & 1;
	lpObj->Helper.PartyAutoHeal = (lpMsg->OptionFlag >> 7) & 1;
	lpObj->Helper.PartyAutoBuff = (lpMsg->OptionFlag >> 8) & 1;
	lpObj->Helper.DarkSpirit = (lpMsg->OptionFlag >> 9) & 1;
	lpObj->Helper.AutoBuff = (lpMsg->OptionFlag >> 10) & 1;
	lpObj->Helper.SkillDelay[0] = (lpMsg->OptionFlag >> 11) & 1;
	lpObj->Helper.SkillCondition[0] = (lpMsg->OptionFlag >> 12) & 1;
	lpObj->Helper.SkillPreCon[0] = ((lpMsg->OptionFlag >> 13) & 1) % 2;
	lpObj->Helper.SkillSubCon[0] = ((lpMsg->OptionFlag >> 8) >> 6) % 4;
	lpObj->Helper.SkillDelay[1] = (lpMsg->OptionFlag >> 16) & 1;
	lpObj->Helper.SkillCondition[1] = (lpMsg->OptionFlag >> 17) & 1;
	lpObj->Helper.SkillPreCon[1] = ((lpMsg->OptionFlag >> 18) & 1) % 2;
	lpObj->Helper.SkillSubCon[1] = ((lpMsg->OptionFlag >> 13) >> 6) % 4;
	lpObj->Helper.RepairItem = (lpMsg->OptionFlag >> 21) & 1;
	lpObj->Helper.PickAllItem = (lpMsg->OptionFlag >> 22) & 1;
	lpObj->Helper.PickSelected = (lpMsg->OptionFlag >> 23) & 1;
	lpObj->Helper.DarkSpiritAuto = (lpMsg->OptionFlag >> 24) & 1;
	lpObj->Helper.DarkSpiritAttack = (lpMsg->OptionFlag >> 25) & 1;
	lpObj->Helper.AutoAcceptFriend = (lpMsg->OptionFlag >> 26) & 1;
	lpObj->Helper.AutoAcceptGuild = (lpMsg->OptionFlag >> 27) & 1;
	lpObj->Helper.PotionElite = (lpMsg->OptionFlag >> 28) & 1;
	lpObj->Helper.ShortDistance = (lpMsg->OptionFlag >> 29) & 1;
	lpObj->Helper.RegularAttack = (lpMsg->OptionFlag >> 30) & 1;

	#endif
}

void CHelper::CGHelperDataRecv(PMSG_HELPER_DATA_RECV* lpMsg,int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=603)

	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnected(aIndex) == 0)
	{
		return;
	}

	this->GDHelperDataSaveSend(aIndex,lpMsg->data);

	this->HelperInfoData(aIndex,(HELPER_INFO*)lpMsg->data);

	#endif
}

void CHelper::CGHelperStartRecv(PMSG_HELPER_START_RECV* lpMsg,int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=603)

	if(gServerInfo.m_HelperSwitch == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(lpObj->Attack.Started != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(571));
		return;
	}

	if(lpMsg->type == 0)
	{
		this->EnableHelper(lpObj);
	}
	else
	{
		lpObj->Helper.Started = 0;

		this->GCHelperStartSend(aIndex,0,0,1);
	}

	#endif
}

void CHelper::GCHelperStartSend(int aIndex,int time,int money,int result) // OK
{
	#if(GAMESERVER_UPDATE>=603)

	PMSG_HELPER_START_SEND pMsg;

	pMsg.header.set(0xBF,0x51,sizeof(pMsg));

	pMsg.time = time;

	pMsg.money = money;

	pMsg.result = result;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);

	#endif
}

void CHelper::DGHelperDataRecv(SDHP_HELPER_DATA_RECV* lpMsg) // OK
{
	#if(GAMESERVER_UPDATE>=603)

	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGHelperDataRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	PMSG_HELPER_DATA_SEND pMsg;

	pMsg.header.set(0xAE,sizeof(pMsg));

	pMsg.result = lpMsg->result;

	memcpy(pMsg.data,lpMsg->data,sizeof(pMsg.data));

	DataSend(lpMsg->index,(BYTE*)&pMsg,sizeof(pMsg));

	this->HelperInfoData(lpMsg->index,(HELPER_INFO*)lpMsg->data);

	#endif
}

void CHelper::GDHelperDataSend(int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=603)

	if(gObjIsAccountValid(aIndex,gObj[aIndex].Account) == 0)
	{
		return;
	}

	SDHP_HELPER_DATA_SEND pMsg;

	pMsg.header.set(0x17,0x00,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

	memcpy(pMsg.name,gObj[aIndex].Name,sizeof(pMsg.name));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);

	#endif
}

void CHelper::GDHelperDataSaveSend(int aIndex,BYTE* data) // OK
{
	#if(GAMESERVER_UPDATE>=603)

	SDHP_HELPER_DATA_SAVE_SEND pMsg;

	pMsg.header.set(0x17,0x30,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

	memcpy(pMsg.name,gObj[aIndex].Name,sizeof(pMsg.name));

	memcpy(pMsg.data,data,sizeof(pMsg.data));

	gDataServerConnection.DataSend((BYTE*)&pMsg,sizeof(pMsg),aIndex);

	#endif
}