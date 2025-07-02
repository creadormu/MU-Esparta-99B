// CustomAttack.cpp: implementation of the CCustomAttack class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomAttack.h"
#include "CommandManager.h"
#include "CustomArena.h"
#include "EffectManager.h"
#include "Map.h"
#include "MapManager.h"
#include "MasterSkillTree.h"
#include "MemScript.h"
#include "Message.h"
#include "Notice.h"
#include "ObjectManager.h"
#include "Party.h"
#include "SkillManager.h"
#include "SocketManager.h"
#include "Viewport.h"
#include "Util.h"
#include "FakeOnline.h"

CCustomAttack gCustomAttack;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomAttack::CCustomAttack() // OK
{
	memset(this->m_CustomAttackMapList,0,sizeof(this->m_CustomAttackMapList));

	this->m_CustomAttackSpeedInfo.clear();

	for(int n = 0;n < MAX_CLASS;n++)
	{
		this->m_CustomAttackSkillInfo[n].clear();
	}
}

CCustomAttack::~CCustomAttack() // OK
{

}

void CCustomAttack::MainProc(LPOBJ lpObj) // OK
{
	if(lpObj->Attack.Started == 0)
	{
		return;
	}

	if(lpObj->Attack.Offline != 0)
	{
		if(this->m_CustomAttackOfflineMaxTimeLimit[lpObj->AccountLevel] > 0)
		{
			if((GetTickCount()-lpObj->Attack.Timer) >= (DWORD)(this->m_CustomAttackOfflineMaxTimeLimit[lpObj->AccountLevel]*60000))
			{
				this->AttackOfflineClose(lpObj);
				return;
			}
		}

		if(this->m_CustomAttackOfflineKeepEnable[lpObj->AccountLevel] == 0)
		{
			if(gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,1) != 0)
			{
				this->AttackOfflineClose(lpObj);
				return;
			}

			if(gViewport.CheckViewportObjectPosition(lpObj->Index,lpObj->Attack.StartMap,lpObj->Attack.StartX,lpObj->Attack.StartY,5) == 0)
			{
				this->AttackOfflineClose(lpObj);
				return;
			}

			if((this->m_CustomAttackMapZone != -1 && this->m_CustomAttackMapList[lpObj->Map] == 0) || gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,1) != 0)
			{
				this->AttackOfflineClose(lpObj);
				return;
			}
		}
	}
	else
	{
		if(this->m_CustomAttackMaxTimeLimit[lpObj->AccountLevel] > 0)
		{
			if((GetTickCount()-lpObj->Attack.Timer) >= (DWORD)(this->m_CustomAttackMaxTimeLimit[lpObj->AccountLevel]*60000))
			{
				lpObj->Attack.Started = 0;
				GCNewMessageSend(lpObj->Index,1,2,gMessage.GetMessage(578));
				this->CustomAttackSend(lpObj->Index);
				return;
			}
		}

		if((this->m_CustomAttackMapZone != -1 && this->m_CustomAttackMapList[lpObj->Map] == 0) || gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,1) != 0)
		{
			lpObj->Attack.Started = 0;
			GCNewMessageSend(lpObj->Index,1,2,gMessage.GetMessage(572));
			this->CustomAttackSend(lpObj->Index);
			return;
		}

		if(gViewport.CheckViewportObjectPosition(lpObj->Index,lpObj->Attack.StartMap,lpObj->Attack.StartX,lpObj->Attack.StartY,5) == 0)
		{
			lpObj->Attack.Started = 0;
			GCNewMessageSend(lpObj->Index,1,2,gMessage.GetMessage(572));
			this->CustomAttackSend(lpObj->Index);
			return;
		}
	}

	this->CustomAttackAutoPotion(lpObj);

	this->CustomAttackAutoRepair(lpObj);

	this->CustomAttackAutoReload(lpObj);

	this->CustomAttackAutoBuff(lpObj);

	this->CustomAttackAutoAttack(lpObj);
}

void CCustomAttack::SecondProc(LPOBJ lpObj) // OK
{
	if(lpObj->Attack.Offline != 0)
	{
		lpObj->ConnectTickCount = GetTickCount();

		lpObj->OnlineRewardTime = ((this->m_CustomAttackOfflineCoinGain[lpObj->AccountLevel]==0)?GetTickCount():lpObj->OnlineRewardTime);
	}
}

void CCustomAttack::AttackOfflineClose(LPOBJ lpObj) // OK
{
	if(lpObj->Attack.Offline != 0)
	{
		gObjDel(lpObj->Index);
		lpObj->Attack.Started = 0;
		lpObj->Attack.Offline = 0;
	}
}

void CCustomAttack::ReadCustomAttackInfo(char* section,char* path) // OK
{
	this->m_CustomAttackSwitch = GetPrivateProfileInt(section,"CustomAttackSwitch",0,path);

	this->m_CustomAttackMapZone = GetPrivateProfileInt(section,"CustomAttackMapZone",0,path);

	if(this->m_CustomAttackMapZone != -1)
	{
		memset(this->m_CustomAttackMapList,0,sizeof(this->m_CustomAttackMapList));

		char buff[256] = {0};

		GetPrivateProfileString(section,"CustomAttackMapZone","",buff,sizeof(buff),path);

		char* next_token;
		char* token = strtok_s(buff,",",&next_token);
	
		while(token != 0)
		{
			if(gMapManager.IsValidMap(atoi(token)) != 0)
			{
				this->m_CustomAttackMapList[atoi(token)] = 1;
			}

			token = strtok_s(0,",",&next_token);
		}
	}

	this->m_CustomAttackBuffEnable[0] = GetPrivateProfileInt(section,"CustomAttackBuffEnable_AL0",0,path);
	this->m_CustomAttackBuffEnable[1] = GetPrivateProfileInt(section,"CustomAttackBuffEnable_AL1",0,path);
	this->m_CustomAttackBuffEnable[2] = GetPrivateProfileInt(section,"CustomAttackBuffEnable_AL2",0,path);
	this->m_CustomAttackBuffEnable[3] = GetPrivateProfileInt(section,"CustomAttackBuffEnable_AL3",0,path);

	this->m_CustomAttackMaxTimeLimit[0] = GetPrivateProfileInt(section,"CustomAttackMaxTimeLimit_AL0",0,path);
	this->m_CustomAttackMaxTimeLimit[1] = GetPrivateProfileInt(section,"CustomAttackMaxTimeLimit_AL1",0,path);
	this->m_CustomAttackMaxTimeLimit[2] = GetPrivateProfileInt(section,"CustomAttackMaxTimeLimit_AL2",0,path);
	this->m_CustomAttackMaxTimeLimit[3] = GetPrivateProfileInt(section,"CustomAttackMaxTimeLimit_AL3",0,path);

	this->m_CustomAttackOfflineSwitch = GetPrivateProfileInt(section,"CustomAttackOfflineSwitch",0,path);

	#if(GAMESERVER_SHOP>=1)

	this->m_CustomAttackOfflineCoinGain[0] = GetPrivateProfileInt(section,"CustomAttackOfflineCoinGain_AL0",0,path);
	this->m_CustomAttackOfflineCoinGain[1] = GetPrivateProfileInt(section,"CustomAttackOfflineCoinGain_AL1",0,path);
	this->m_CustomAttackOfflineCoinGain[2] = GetPrivateProfileInt(section,"CustomAttackOfflineCoinGain_AL2",0,path);
	this->m_CustomAttackOfflineCoinGain[3] = GetPrivateProfileInt(section,"CustomAttackOfflineCoinGain_AL3",0,path);

	#endif

	this->m_CustomAttackOfflineBuffEnable[0] = GetPrivateProfileInt(section,"CustomAttackOfflineBuffEnable_AL0",0,path);
	this->m_CustomAttackOfflineBuffEnable[1] = GetPrivateProfileInt(section,"CustomAttackOfflineBuffEnable_AL1",0,path);
	this->m_CustomAttackOfflineBuffEnable[2] = GetPrivateProfileInt(section,"CustomAttackOfflineBuffEnable_AL2",0,path);
	this->m_CustomAttackOfflineBuffEnable[3] = GetPrivateProfileInt(section,"CustomAttackOfflineBuffEnable_AL3",0,path);

	this->m_CustomAttackOfflineKeepEnable[0] = GetPrivateProfileInt(section,"CustomAttackOfflineKeepEnable_AL0",0,path);
	this->m_CustomAttackOfflineKeepEnable[1] = GetPrivateProfileInt(section,"CustomAttackOfflineKeepEnable_AL1",0,path);
	this->m_CustomAttackOfflineKeepEnable[2] = GetPrivateProfileInt(section,"CustomAttackOfflineKeepEnable_AL2",0,path);
	this->m_CustomAttackOfflineKeepEnable[3] = GetPrivateProfileInt(section,"CustomAttackOfflineKeepEnable_AL3",0,path);

	this->m_CustomAttackOfflineMaxTimeLimit[0] = GetPrivateProfileInt(section,"CustomAttackOfflineMaxTimeLimit_AL0",0,path);
	this->m_CustomAttackOfflineMaxTimeLimit[1] = GetPrivateProfileInt(section,"CustomAttackOfflineMaxTimeLimit_AL1",0,path);
	this->m_CustomAttackOfflineMaxTimeLimit[2] = GetPrivateProfileInt(section,"CustomAttackOfflineMaxTimeLimit_AL2",0,path);
	this->m_CustomAttackOfflineMaxTimeLimit[3] = GetPrivateProfileInt(section,"CustomAttackOfflineMaxTimeLimit_AL3",0,path);
}

void CCustomAttack::Load(char* path) // OK
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

	this->m_CustomAttackSpeedInfo.clear();

	for(int n = 0;n < MAX_CLASS;n++)
	{
		this->m_CustomAttackSkillInfo[n].clear();
	}

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

					CUSTOM_ATTACK_SPEED_INFO info;

					info.Class = lpMemScript->GetNumber();

					info.MinReset = lpMemScript->GetAsNumber();

					info.MaxReset = lpMemScript->GetAsNumber();

					info.SpeedRate = lpMemScript->GetAsNumber();

					this->m_CustomAttackSpeedInfo.push_back(info);
				}
				else if(section == 1)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					CUSTOM_ATTACK_SKILL_INFO info;

					info.Class = lpMemScript->GetNumber();

					info.Skill = lpMemScript->GetAsNumber();

					info.Group = lpMemScript->GetAsNumber();

					info.Value = lpMemScript->GetAsNumber();

					info.BaseSpeed = (int)(lpMemScript->GetAsFloatNumber()*1000);

					info.MainSpeed = (int)(lpMemScript->GetAsFloatNumber()*1000);

					this->SetInfo(info);
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

void CCustomAttack::SetInfo(CUSTOM_ATTACK_SKILL_INFO info) // OK
{
	if(CHECK_RANGE(info.Class,MAX_CLASS) == 0)
	{
		return;
	}

	std::map<int,CUSTOM_ATTACK_SKILL_INFO>::iterator it = this->m_CustomAttackSkillInfo[info.Class].find(info.Skill);

	if(it != this->m_CustomAttackSkillInfo[info.Class].end())
	{
		return;
	}

	this->m_CustomAttackSkillInfo[info.Class].insert(std::pair<int,CUSTOM_ATTACK_SKILL_INFO>(info.Skill,info));
}

int CCustomAttack::GetAttackSpeed(LPOBJ lpObj) // OK
{
	for(std::vector<CUSTOM_ATTACK_SPEED_INFO>::iterator it=this->m_CustomAttackSpeedInfo.begin(); it != this->m_CustomAttackSpeedInfo.end(); it++)
	{
		if(it->Class != lpObj->Class)
		{
			continue;
		}

		if(it->MinReset != -1 && it->MinReset > lpObj->Reset)
		{
			continue;
		}

		if(it->MaxReset != -1 && it->MaxReset < lpObj->Reset)
		{
			continue;
		}

		return it->SpeedRate;
	}

	return 100;
}

CUSTOM_ATTACK_SKILL_INFO* CCustomAttack::GetInfo(int Class,int SkillNumber) // OK
{
	std::map<int,CUSTOM_ATTACK_SKILL_INFO>::iterator it = this->m_CustomAttackSkillInfo[Class].find(SkillNumber);

	if(it == this->m_CustomAttackSkillInfo[Class].end())
	{
		return 0;
	}

	return &it->second;
}

int CCustomAttack::GetSkilLAuto(LPOBJ lpObj) // OK
{
	for(std::map<int,CUSTOM_ATTACK_SKILL_INFO>::iterator it=this->m_CustomAttackSkillInfo[lpObj->Class].begin();it != this->m_CustomAttackSkillInfo[lpObj->Class].end();it++)
	{
		CSkill* lpSkill = this->CustomAttackFindSkill(lpObj,it->second.Skill);

		if(lpSkill == 0)
		{
			continue;
		}

		return it->second.Skill;
	}

	return 0;
}

void CCustomAttack::CustomAttackSend(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	PMSG_CUSTOM_ATTACK_SEND pMsg;

	pMsg.header.set(0xF3,0xEB,sizeof(pMsg));

	pMsg.Started = lpObj->Attack.Started;

	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
}

void CCustomAttack::CustomAttackClose(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(lpObj->Attack.Started == 0)
	{
		return;
	}

	lpObj->Attack.Started = 0;
	GCNewMessageSend(lpObj->Index,1,2,gMessage.GetMessage(572));
}

void CCustomAttack::CommandCustomAttack(LPOBJ lpObj,char* arg) // OK
{
	if(this->m_CustomAttackSwitch == 0)
	{
		return;
	}
	
	if(lpObj->Helper.Started != 0)
	{
		GCNewMessageSend(lpObj->Index,1,2,gMessage.GetMessage(573));
		return;
	}

	if((this->m_CustomAttackMapZone != -1 && this->m_CustomAttackMapList[lpObj->Map] == 0) || gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,1) != 0)
	{
		GCNewMessageSend(lpObj->Index,1,2,gMessage.GetMessage(574));
		return;
	}

	lpObj->Attack.Started ^= 1;

	if(lpObj->Attack.Started == 0)
	{
		GCNewMessageSend(lpObj->Index,1,2,gMessage.GetMessage(572));
		return;
	}
	else
	{
		if(strcmp(arg,"") != 0)
		{
			lpObj->Attack.AutoAttack = gCommandManager.GetNumber(arg,0);

			lpObj->Attack.AutoBuff = gCommandManager.GetNumber(arg,1);

			lpObj->Attack.SkillIndex = gCommandManager.GetNumber(arg,2);
		}
		else
		{
			lpObj->Attack.AutoAttack = 1;

			lpObj->Attack.AutoBuff = this->m_CustomAttackBuffEnable[lpObj->AccountLevel];

			lpObj->Attack.SkillIndex = this->GetSkilLAuto(lpObj);
		}

		lpObj->Attack.StartMap = lpObj->Map;

		lpObj->Attack.StartX = lpObj->X;

		lpObj->Attack.StartY = lpObj->Y;

		lpObj->Attack.TimerSkill = GetTickCount();
	}

	this->CustomAttackSend(lpObj->Index);

	if(lpObj->Attack.AutoAttack == 0 && lpObj->Attack.AutoBuff == 0 && lpObj->Attack.SkillIndex == 0)
	{
		GCNewMessageSend(lpObj->Index,1,2,gMessage.GetMessage(575));
		return;
	}

	lpObj->Attack.Timer = GetTickCount();

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(576));
}

void CCustomAttack::CommandCustomAttackOffline(LPOBJ lpObj) // OK
{
	
#if USE_FAKE_ONLINE == TRUE
s_FakeOnline.FakeAttackProc(lpObj); //Attack, Di Chuyen, Move
#endif

	if(this->m_CustomAttackOfflineSwitch == 0)
	{
		return;
	}

	if(lpObj->Attack.Started == 0)
	{
		GCNewMessageSend(lpObj->Index,1,2,gMessage.GetMessage(577));
		return;
	}

	lpObj->Socket = INVALID_SOCKET;

	lpObj->Attack.Offline = 1;

	lpObj->Attack.Timer = GetTickCount();

	closesocket(lpObj->PerSocketContext->Socket);
}

void CCustomAttack::CustomAttackAutoPotion(LPOBJ lpObj) // OK
{
	PMSG_ITEM_USE_RECV pMsg;

	pMsg.header.set(0x26,sizeof(pMsg));

	pMsg.SourceSlot = 0xFF;

	pMsg.TargetSlot = 0xFF;

	pMsg.type = 0;

	for(int n=0;n < 10;n++)
	{
		if(((lpObj->Life*100)/(lpObj->MaxLife+lpObj->AddLife)) < 50)
		{
			pMsg.SourceSlot = ((pMsg.SourceSlot==0xFF)?gItemManager.GetInventoryItemSlot(lpObj,GET_ITEM(14,3),-1):pMsg.SourceSlot);

			pMsg.SourceSlot = ((pMsg.SourceSlot==0xFF)?gItemManager.GetInventoryItemSlot(lpObj,GET_ITEM(14,2),-1):pMsg.SourceSlot);

			pMsg.SourceSlot = ((pMsg.SourceSlot==0xFF)?gItemManager.GetInventoryItemSlot(lpObj,GET_ITEM(14,1),-1):pMsg.SourceSlot);

			pMsg.SourceSlot = ((pMsg.SourceSlot==0xFF)?gItemManager.GetInventoryItemSlot(lpObj,GET_ITEM(14,0),-1):pMsg.SourceSlot);

			if(INVENTORY_FULL_RANGE(pMsg.SourceSlot) != 0)
			{
				gItemManager.CGItemUseRecv(&pMsg,lpObj->Index);
			}
		}

		if(((lpObj->Mana*100)/(lpObj->MaxMana+lpObj->AddMana)) < 50)
		{
			pMsg.SourceSlot = ((pMsg.SourceSlot==0xFF)?gItemManager.GetInventoryItemSlot(lpObj,GET_ITEM(14,6),-1):pMsg.SourceSlot);

			pMsg.SourceSlot = ((pMsg.SourceSlot==0xFF)?gItemManager.GetInventoryItemSlot(lpObj,GET_ITEM(14,5),-1):pMsg.SourceSlot);

			pMsg.SourceSlot = ((pMsg.SourceSlot==0xFF)?gItemManager.GetInventoryItemSlot(lpObj,GET_ITEM(14,4),-1):pMsg.SourceSlot);

			if(INVENTORY_FULL_RANGE(pMsg.SourceSlot) != 0)
			{
				gItemManager.CGItemUseRecv(&pMsg,lpObj->Index);
			}
		}
	}
}

void CCustomAttack::CustomAttackAutoRepair(LPOBJ lpObj) // OK
{
	PMSG_ITEM_REPAIR_SEND pMsg;

	pMsg.header.set(0x34,sizeof(pMsg));

	pMsg.money = 0;

	for(int n=0;n < INVENTORY_WEAR_SIZE;n++)
	{
		if(lpObj->Inventory[n].IsItem() == 0)
		{
			continue;
		}

		if(lpObj->Inventory[n].m_Durability >= lpObj->Inventory[n].m_BaseDurability)
		{
			continue;
		}

		pMsg.money = gItemManager.RepairItem(lpObj,&lpObj->Inventory[n],n,0);

		if(pMsg.money != 0)
		{
			DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
			
			gObjectManager.CharacterCalcAttribute(lpObj->Index);
		}
	}
}

void CCustomAttack::CustomAttackAutoReload(LPOBJ lpObj) // OK
{
	if(lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index >= GET_ITEM(4,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index < GET_ITEM(5,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index != GET_ITEM(4,15) && lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Slot == 0)
	{
		if(lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Index != GET_ITEM(4,7) || lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Durability < 1)
		{
			if(gItemManager.GetInventoryItemCount(lpObj,GET_ITEM(4,7),-1) > 0)
			{
				PMSG_ITEM_MOVE_RECV pMsg;

				pMsg.header.set(0x24,sizeof(pMsg));

				pMsg.SourceFlag = 0;

				pMsg.SourceSlot = gItemManager.GetInventoryItemSlot(lpObj,GET_ITEM(4,7),-1);

				pMsg.TargetFlag = 0;

				pMsg.TargetSlot = 1;

				if(INVENTORY_FULL_RANGE(pMsg.TargetSlot) != 0) 
				{
					gItemManager.CGItemMoveRecv(&pMsg,lpObj->Index);
					gItemManager.InventoryDelItem(lpObj->Index,pMsg.SourceSlot);
					gItemManager.GCItemDeleteSend(lpObj->Index,pMsg.SourceSlot,1);
				}
			}
		}
	}

	if(lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Index >= GET_ITEM(4,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Index < GET_ITEM(5,0) && lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Index != GET_ITEM(4,7) && lpObj->Inventory[INVENTORY_SLOT_WEAPON2].m_Slot == 1)
	{
		if(lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Index != GET_ITEM(4,15) || lpObj->Inventory[INVENTORY_SLOT_WEAPON1].m_Durability < 1)
		{
			if(gItemManager.GetInventoryItemCount(lpObj,GET_ITEM(4,15),-1) > 0)
			{
				PMSG_ITEM_MOVE_RECV pMsg;

				pMsg.header.set(0x24,sizeof(pMsg));

				pMsg.SourceFlag = 0;

				pMsg.SourceSlot = gItemManager.GetInventoryItemSlot(lpObj,GET_ITEM(4,15),-1);

				pMsg.TargetFlag = 0;

				pMsg.TargetSlot = 0;

				if(INVENTORY_FULL_RANGE(pMsg.TargetSlot) != 0)
				{ 
					gItemManager.CGItemMoveRecv(&pMsg,lpObj->Index);
					gItemManager.InventoryDelItem(lpObj->Index,pMsg.SourceSlot);
					gItemManager.GCItemDeleteSend(lpObj->Index,pMsg.SourceSlot,1);
				}
			}
		}
	}
}

void CCustomAttack::CustomAttackAutoBuff(LPOBJ lpObj) // OK
{
	if(lpObj->Attack.Offline == 0 && this->m_CustomAttackBuffEnable[lpObj->AccountLevel] == 0)
	{
		return;
	}

	if(lpObj->Attack.Offline != 0 && this->m_CustomAttackOfflineBuffEnable[lpObj->AccountLevel] == 0)
	{
		return;
	}

	if(lpObj->Attack.AutoBuff == 0)
	{ 
		return;
	}

	for(std::map<int,CUSTOM_ATTACK_SKILL_INFO>::iterator it=this->m_CustomAttackSkillInfo[lpObj->Class].begin();it != this->m_CustomAttackSkillInfo[lpObj->Class].end();it++)
	{
		if(it->second.Group != 0)
		{
			continue;
		}
		
		CSkill* lpSkill = this->CustomAttackFindSkill(lpObj,it->second.Skill);

		if(lpSkill == 0)
		{
			continue;
		}

		if(gSkillManager.CheckSkillMana(lpObj,lpSkill->m_index) == 0)
		{
			continue;
		}

		if(gSkillManager.CheckSkillBP(lpObj,lpSkill->m_index) == 0)
		{
			continue;
		}

		if(it->second.Value == 0)
		{
			if(gEffectManager.CheckEffect(lpObj,gSkillManager.GetSkillEffect(lpSkill->m_index)) == 0)
			{
				this->CustomAttackUseSkill(lpObj,lpObj,&it->second,lpSkill);
				continue;
			}
		}
		else
		{
			if(OBJECT_RANGE(lpObj->PartyNumber) == 0)
			{
				if(gEffectManager.CheckEffect(lpObj,gSkillManager.GetSkillEffect(lpSkill->m_index)) == 0)
				{
					this->CustomAttackUseSkill(lpObj,lpObj,&it->second,lpSkill);
					continue;
				}
			}
			else
			{
				for(int n=0;n < MAX_PARTY_USER;n++)
				{
					int bIndex = gParty.m_PartyInfo[lpObj->PartyNumber].Index[n];

					if(OBJECT_RANGE(bIndex) == 0)
					{
						continue;
					}

					LPOBJ lpTarget = &gObj[bIndex];

					if(lpTarget->Live == 0 || lpTarget->State != OBJECT_PLAYING || lpTarget->Map != lpObj->Map)
					{
						continue;
					}

					if(gObjCalcDistance(lpObj,lpTarget) > gSkillManager.GetSkillRange(lpSkill->m_index))
					{
						continue;
					}

					if(gEffectManager.CheckEffect(lpTarget,gSkillManager.GetSkillEffect(lpSkill->m_index)) == 0)
					{
						this->CustomAttackUseSkill(lpObj,lpTarget,&it->second,lpSkill);
						continue;
					}
				}
			}
		}
	}
}

void CCustomAttack::CustomAttackAutoAttack(LPOBJ lpObj) // OK
{
	if(lpObj->Attack.AutoAttack == 0)
	{
		return;
	}

	CUSTOM_ATTACK_SKILL_INFO* lpInfo = this->GetInfo(lpObj->Class,lpObj->Attack.SkillIndex);

	if(lpInfo == 0)
	{
		return;
	}

	CSkill* lpSkill = this->CustomAttackFindSkill(lpObj,lpObj->Attack.SkillIndex);

	if(lpSkill != 0)
	{
		if(GetTickCount() < lpObj->Attack.TimerSkill)
		{
			return;
		}

		if(gSkillManager.CheckSkillMana(lpObj,lpSkill->m_index) == 0)
		{
			return;
		}

		if(gSkillManager.CheckSkillBP(lpObj,lpSkill->m_index) == 0)
		{
			return;
		}

		if(this->CustomAttackFindTarget(lpObj,lpSkill->m_index) != 0)
		{
			this->CustomAttackUseSkill(lpObj,&gObj[lpObj->TargetNumber],lpInfo,lpSkill);
		}
	}
}

int CCustomAttack::CustomAttackFindTarget(LPOBJ lpObj,int SkillNumber) // OK
{
	int NearestDistance = 100;

	for(int n=0;n < MAX_VIEWPORT;n++)
	{
		if(lpObj->VpPlayer2[n].state == VIEWPORT_NONE || OBJECT_RANGE(lpObj->VpPlayer2[n].index) == 0 || lpObj->VpPlayer2[n].type != OBJECT_MONSTER)
		{
			continue;
		}

		if(gSkillManager.CheckSkillTarget(lpObj,lpObj->VpPlayer2[n].index,-1,lpObj->VpPlayer2[n].type) == 0)
		{
			continue;
		}

		if(gObjCalcDistance(lpObj,&gObj[lpObj->VpPlayer2[n].index]) >= NearestDistance)
		{
			continue;
		}

		if(gSkillManager.CheckSkillRange(lpObj,SkillNumber,lpObj->X,lpObj->Y,gObj[lpObj->VpPlayer2[n].index].X,gObj[lpObj->VpPlayer2[n].index].Y) != 0)
		{
			lpObj->TargetNumber = lpObj->VpPlayer2[n].index;
			NearestDistance = gObjCalcDistance(lpObj,&gObj[lpObj->VpPlayer2[n].index]);
			continue;
		}

		if(gSkillManager.CheckSkillRadio(SkillNumber,lpObj->X,lpObj->Y,gObj[lpObj->VpPlayer2[n].index].X,gObj[lpObj->VpPlayer2[n].index].Y) != 0)
		{
			lpObj->TargetNumber = lpObj->VpPlayer2[n].index;
			NearestDistance = gObjCalcDistance(lpObj,&gObj[lpObj->VpPlayer2[n].index]);
			continue;
		}
	}

	return ((NearestDistance==100)?0:1);
}

CSkill* CCustomAttack::CustomAttackFindSkill(LPOBJ lpObj,int SkillNumber) // OK
{
	CSkill* lpSkill = gSkillManager.GetMasterSkill(lpObj,SkillNumber);

	if(lpSkill != 0)
	{
		return lpSkill;
	}

	lpSkill = gSkillManager.GetSkill(lpObj,SkillNumber);

	if(lpSkill != 0)
	{
		return lpSkill;
	}

	return 0;
}

void CCustomAttack::CustomAttackUseSkill(LPOBJ lpObj,LPOBJ lpTarget,CUSTOM_ATTACK_SKILL_INFO* lpInfo,CSkill* lpSkill) // OK
{
	if(lpInfo == 0 || lpSkill == 0)
	{
		return;
	}

	if(lpSkill->m_skill == SKILL_HEAL && ((lpTarget->Life*100)/(lpTarget->MaxLife+lpTarget->AddLife)) > 60)
	{
		return;
	}

	if(GetTickCount() < lpObj->Attack.TimerSkill)
	{
		return;
	}

	if(gSkillManager.CheckSkillMana(lpObj,lpSkill->m_index) == 0)
	{
		return;
	}

	if(gSkillManager.CheckSkillBP(lpObj,lpSkill->m_index) == 0)
	{
		return;
	}

	switch(lpInfo->Group)
	{
		case 2:
			this->CustomAttackMultilAttack(lpObj,lpTarget->Index,lpInfo->Skill);
			break;
		case 3:
			this->CustomAttackDurationlAttack(lpObj,lpTarget->Index,lpInfo->Skill);
			break;
		case 4:
			this->CustomAttackRageAttack(lpObj,lpTarget->Index,lpInfo->Skill);
			break;
		default:
			this->CustomAttackSkillAttack(lpObj,lpTarget->Index,lpInfo->Skill);
			break;
	}

	DWORD SkillDelay = gSkillManager.GetSkillDelay(lpSkill->m_index);

	if(SkillDelay > 0)
	{
		lpObj->Attack.TimerSkill = GetTickCount()+SkillDelay;
	}
	else
	{
		int BaseSpeed = (lpInfo->BaseSpeed*this->GetAttackSpeed(lpObj))/100;

		lpObj->Attack.TimerSkill = GetTickCount()+(DWORD)((lpInfo->MainSpeed*lpObj->PhysiSpeed)>BaseSpeed?0:BaseSpeed-(lpInfo->MainSpeed*lpObj->PhysiSpeed));
	}
}

void CCustomAttack::CustomAttackSkillAttack(LPOBJ lpObj,int aIndex,int SkillNumber) // OK
{
	PMSG_SKILL_ATTACK_RECV pMsg;

	pMsg.header.set(0x19,sizeof(pMsg));

	#if(GAMESERVER_UPDATE>=701)

	pMsg.skillH = SET_NUMBERHB(SkillNumber);

	pMsg.skillL = SET_NUMBERLB(SkillNumber);

	pMsg.indexH = SET_NUMBERHB(aIndex);

	pMsg.indexL = SET_NUMBERLB(aIndex);

	#else

	#if(GAMESERVER_UPDATE>=401)

	pMsg.skill[0] = SET_NUMBERHB(SkillNumber);

	pMsg.skill[1] = SET_NUMBERLB(SkillNumber);

	#else

	pMsg.skill = SkillNumber;

	#endif

	pMsg.index[0] = SET_NUMBERHB(aIndex);

	pMsg.index[1] = SET_NUMBERLB(aIndex);

	#endif

	pMsg.dis = 0;

	gSkillManager.CGSkillAttackRecv(&pMsg,lpObj->Index);
}

void CCustomAttack::CustomAttackMultilAttack(LPOBJ lpObj,int aIndex,int SkillNumber) // OK
{
	this->CustomAttackDurationlAttack(lpObj,aIndex,SkillNumber);

	BYTE send[256];

	PMSG_MULTI_SKILL_ATTACK_RECV pMsg;

	pMsg.header.set(PROTOCOL_CODE4,sizeof(pMsg));

	int size = sizeof(pMsg);

	#if(GAMESERVER_UPDATE>=701)

	pMsg.skillH = SET_NUMBERHB(SkillNumber);

	pMsg.skillL = SET_NUMBERLB(SkillNumber);

	#elif(GAMESERVER_UPDATE>=401)

	pMsg.skill[0] = SET_NUMBERHB(SkillNumber);

	pMsg.skill[1] = SET_NUMBERLB(SkillNumber);

	#else

	pMsg.skill = SkillNumber;

	#endif

	pMsg.x = (BYTE)lpObj->X;

	pMsg.y = (BYTE)lpObj->Y;

	pMsg.serial = 0;

	pMsg.count = 0;

	PMSG_MULTI_SKILL_ATTACK info;

	for(int n=0;n < MAX_VIEWPORT;n++)
	{
		if(lpObj->VpPlayer2[n].state == VIEWPORT_NONE || OBJECT_RANGE(lpObj->VpPlayer2[n].index) == 0 || lpObj->VpPlayer2[n].type != OBJECT_MONSTER)
		{
			continue;
		}

		int index = lpObj->VpPlayer2[n].index;

		if(gSkillManager.CheckSkillTarget(lpObj,index,aIndex,lpObj->VpPlayer2[n].type) == 0)
		{
			continue;
		}

		if(gSkillManager.CheckSkillRadio(SkillNumber,lpObj->X,lpObj->Y,gObj[index].X,gObj[index].Y) == 0)
		{
			continue;
		}

		#if(GAMESERVER_UPDATE>=701)

		info.indexH = SET_NUMBERHB(index);

		info.indexL = SET_NUMBERLB(index);

		#else

		info.index[0] = SET_NUMBERHB(index);

		info.index[1] = SET_NUMBERLB(index);

		#endif

		info.MagicKey = 0;

		memcpy(&send[size],&info,sizeof(info));
		size += sizeof(info);

		if(CHECK_SKILL_ATTACK_COUNT(pMsg.count) == 0)
		{
			break;
		}
	}

	pMsg.header.size = size;

	memcpy(send,&pMsg,sizeof(pMsg));

	gSkillManager.CGMultiSkillAttackRecv((PMSG_MULTI_SKILL_ATTACK_RECV*)send,lpObj->Index,0);
}

void CCustomAttack::CustomAttackDurationlAttack(LPOBJ lpObj,int aIndex,int SkillNumber) // OK
{
	PMSG_DURATION_SKILL_ATTACK_RECV pMsg;

	pMsg.header.set(0x1E,sizeof(pMsg));

	#if(GAMESERVER_UPDATE>=701)

	pMsg.skillH = SET_NUMBERHB(SkillNumber);

	pMsg.skillL = SET_NUMBERLB(SkillNumber);

	#elif(GAMESERVER_UPDATE>=401)

	pMsg.skill[0] = SET_NUMBERHB(SkillNumber);

	pMsg.skill[1] = SET_NUMBERLB(SkillNumber);

	#else

	pMsg.skill = SkillNumber;

	#endif

	pMsg.x = (BYTE)gObj[aIndex].X;

	pMsg.y = (BYTE)gObj[aIndex].Y;

	pMsg.dir = (gSkillManager.GetSkillAngle(gObj[aIndex].X,gObj[aIndex].Y,lpObj->X,lpObj->Y)*255)/360;

	pMsg.dis = 0;

	pMsg.angle = (gSkillManager.GetSkillAngle(lpObj->X,lpObj->Y,gObj[aIndex].X,gObj[aIndex].Y)*255)/360;

	#if(GAMESERVER_UPDATE>=803)

	pMsg.indexH = SET_NUMBERHB(aIndex);

	pMsg.indexL = SET_NUMBERLB(aIndex);

	#else

	pMsg.index[0] = SET_NUMBERHB(aIndex);

	pMsg.index[1] = SET_NUMBERLB(aIndex);

	#endif

	pMsg.MagicKey = 0;

	gSkillManager.CGDurationSkillAttackRecv(&pMsg,lpObj->Index);
}

void CCustomAttack::CustomAttackRageAttack(LPOBJ lpObj,int aIndex,int SkillNumber) // OK
{
	if(SkillNumber == SKILL_DARK_SIDE)
	{
		PMSG_SKILL_DARK_SIDE_RECV pMsg;

		pMsg.header.set(0x4B,sizeof(pMsg));

		pMsg.skill[0] = SET_NUMBERHB(SkillNumber);

		pMsg.skill[1] = SET_NUMBERLB(SkillNumber);

		pMsg.index[0] = SET_NUMBERHB(aIndex);

		pMsg.index[1] = SET_NUMBERLB(aIndex);

		gSkillManager.CGSkillDarkSideRecv(&pMsg,lpObj->Index);
	}
	
	PMSG_RAGE_FIGHTER_SKILL_ATTACK_RECV pMsg;

	pMsg.header.set(0x4A,sizeof(pMsg));

	#if(GAMESERVER_UPDATE>=701)

	pMsg.indexH = SET_NUMBERHB(aIndex);

	pMsg.indexL = SET_NUMBERLB(aIndex);

	pMsg.skillH = SET_NUMBERHB(SkillNumber);

	pMsg.skillL = SET_NUMBERLB(SkillNumber);

	#else

	pMsg.skill[0] = SET_NUMBERHB(SkillNumber);

	pMsg.skill[1] = SET_NUMBERLB(SkillNumber);

	pMsg.index[0] = SET_NUMBERHB(aIndex);

	pMsg.index[1] = SET_NUMBERLB(aIndex);

	#endif

	pMsg.dis = 0;

	gSkillManager.CGRageFighterSkillAttackRecv(&pMsg,lpObj->Index);
}