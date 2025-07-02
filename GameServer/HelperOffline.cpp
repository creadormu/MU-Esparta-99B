// HelperOffline.cpp: implementation of the CHelperOffline class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HelperOffline.h"
#include "Attack.h"
#include "CustomAttack.h"
#include "EffectManager.h"
#include "Helper.h"
#include "ItemLevel.h"
#include "ItemManager.h"
#include "Kalima.h"
#include "Map.h"
#include "MasterSkillTree.h"
#include "MemScript.h"
#include "Message.h"
#include "Monster.h"
#include "Notice.h"
#include "ObjectManager.h"
#include "Party.h"
#include "ServerInfo.h"
#include "SkillManager.h"
#include "SocketManager.h"
#include "Util.h"
#include "Viewport.h"

CHelperOffline gHelperOffline;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHelperOffline::CHelperOffline() // OK
{
	
}

CHelperOffline::~CHelperOffline() // OK
{

}

void CHelperOffline::Load(char* path) // OK
{
	#if(GAMESERVER_UPDATE>=603)

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

	this->m_HelperSkillInfo.clear();

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

			HELPER_SKILL_INFO info;

			info.Skill = lpMemScript->GetNumber();

			info.Group = lpMemScript->GetAsNumber();

			info.Value = lpMemScript->GetAsNumber();

			info.BaseSpeed = (int)(lpMemScript->GetAsFloatNumber()*1000);

			info.MainSpeed = (int)(lpMemScript->GetAsFloatNumber()*1000);

			this->SetInfo(info);
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;

	#endif
}

void CHelperOffline::SetInfo(HELPER_SKILL_INFO info) // OK
{
	#if(GAMESERVER_UPDATE>=603)

	std::map<int,HELPER_SKILL_INFO>::iterator it = this->m_HelperSkillInfo.find(info.Skill);

	if(it != this->m_HelperSkillInfo.end())
	{
		return;
	}

	this->m_HelperSkillInfo.insert(std::pair<int,HELPER_SKILL_INFO>(info.Skill,info));

	#endif
}

HELPER_SKILL_INFO* CHelperOffline::GetInfo(int index) // OK
{
	#if(GAMESERVER_UPDATE>=603)

	std::map<int,HELPER_SKILL_INFO>::iterator it = this->m_HelperSkillInfo.find(index);

	if(it == this->m_HelperSkillInfo.end())
	{
		return 0;
	}

	return &it->second;

	#else

	return 0;

	#endif
}

void CHelperOffline::MainProc(LPOBJ lpObj) // OK
{
	#if(GAMESERVER_UPDATE>=603)

	if(lpObj->Helper.Offline == 0)
	{
		return;
	}

	if(lpObj->Live == 0)
	{
		return;
	}

	if(gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,1) != 0)
	{
		this->HelperOfflineClose(lpObj);
		return;
	}

	if(gServerInfo.m_HelperOfflineMaxTimeLimit[lpObj->AccountLevel] > 0)
	{
		if((GetTickCount()-lpObj->Helper.TimerOffline) >= (DWORD)(gServerInfo.m_HelperOfflineMaxTimeLimit[lpObj->AccountLevel]*60000))
		{
			this->HelperOfflineClose(lpObj);
			return;
		}
	}

	this->HelperOfflineAutoPotion(lpObj);

	this->HelperOfflineAutoBuff(lpObj);

	this->HelperOfflineAutoPick(lpObj);

	this->HelperOfflineAutoPetPick(lpObj);

	this->HelperOfflineAutoReload(lpObj);

	this->HelperOfflineAutoRepair(lpObj);

	if(GetTickCount() < lpObj->Helper.TimerSkill)
	{
		return;
	}

	if(lpObj->ComboSkill.m_index != -1)
	{
		if(lpObj->ComboSkill.m_time < GetTickCount())
		{
			lpObj->ComboSkill.Init();
		}
	}
	
	if(lpObj->Helper.OriginalPosition != 0)
	{
		if((GetTickCount()-lpObj->Helper.TimerSkill) > 3000)
		{
			if((GetTickCount()-lpObj->Helper.TimerMove) > (DWORD)(lpObj->Helper.MoveTime*1000))
			{
				lpObj->Helper.TimerMove = GetTickCount();

				if(lpObj->Helper.StartX != lpObj->X || lpObj->Helper.StartY != lpObj->Y)
				{
					this->HelperOfflineMove(lpObj,lpObj->Helper.StartX,lpObj->Helper.StartY);

					return;
				}
			}
		}
	}

	this->HelperOfflineAutoBuffParty(lpObj);
	
	this->HelperOfflineAutoAttack(lpObj);

	#endif
}

void CHelperOffline::SecondProc(LPOBJ lpObj) // OK
{
	#if(GAMESERVER_UPDATE>=603)

	if(lpObj->Helper.Offline != 0)
	{
		lpObj->ConnectTickCount = GetTickCount();
		lpObj->OnlineRewardTime = ((gServerInfo.m_HelperOfflineCoinGain==0)?GetTickCount():lpObj->OnlineRewardTime);
	}

	#endif
}

void CHelperOffline::CommandHelperOffline(LPOBJ lpObj) // OK
{
	#if(GAMESERVER_UPDATE>=603)

	if(gServerInfo.m_HelperOfflineSwitch == 0)
	{
		return;
	}
	
	if(lpObj->Helper.Started == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(570));
		return;
	}

	lpObj->Socket = INVALID_SOCKET;

	lpObj->Helper.Offline = 1;

	lpObj->Helper.TimerOffline = GetTickCount();

	closesocket(lpObj->PerSocketContext->Socket);

	#endif
}

void CHelperOffline::HelperOfflineClose(LPOBJ lpObj) // OK
{
	#if(GAMESERVER_UPDATE>=603)

	if(lpObj->Helper.Offline != 0)
	{
		gObjDel(lpObj->Index);
		lpObj->Helper.Offline = 0;
	}

	#endif
}

void CHelperOffline::HelperOfflineAutoPotion(LPOBJ lpObj) // OK
{
	#if(GAMESERVER_UPDATE>=603)

	if(lpObj->Helper.AutoPotion == 0)
	{
		return;
	}

	PMSG_ITEM_USE_RECV pMsg;

	pMsg.header.set(0x26,sizeof(pMsg));

	pMsg.SourceSlot = 0xFF;

	pMsg.TargetSlot = 0xFF;

	pMsg.type = 0;

	for(int n=0;n < 20;n++)
	{
		if(((lpObj->Life*100)/(lpObj->MaxLife+lpObj->AddLife)) < lpObj->Helper.PotPercent)
		{
			pMsg.SourceSlot = ((pMsg.SourceSlot==0xFF)?gItemManager.GetInventoryItemSlot(lpObj,GET_ITEM(14,3),-1):pMsg.SourceSlot);

			pMsg.SourceSlot = ((pMsg.SourceSlot==0xFF)?gItemManager.GetInventoryItemSlot(lpObj,GET_ITEM(14,2),-1):pMsg.SourceSlot);

			pMsg.SourceSlot = ((pMsg.SourceSlot==0xFF)?gItemManager.GetInventoryItemSlot(lpObj,GET_ITEM(14,1),-1):pMsg.SourceSlot);

			pMsg.SourceSlot = ((pMsg.SourceSlot==0xFF)?gItemManager.GetInventoryItemSlot(lpObj,GET_ITEM(14,0),-1):pMsg.SourceSlot);

			if(pMsg.SourceSlot == 0xFF) 
			{ 
				return;
			}

			gItemManager.CGItemUseRecv(&pMsg,lpObj->Index);
		}
		else
		{
			return;
		}
	}

	#endif
}

void CHelperOffline::HelperOfflineAutoBuff(LPOBJ lpObj) // OK
{
	#if(GAMESERVER_UPDATE>=603)

	if(gServerInfo.m_HelperOfflineBuffEnable[lpObj->AccountLevel] == 0)
	{
		return;
	}

	if(lpObj->Helper.AutoBuff != 0)
	{
		for(int n=0;n < 3;n++)
		{
			CSkill* lpSkill = this->HelperOfflineGetSkill(lpObj,lpObj->Helper.Buff[n]);
			
			if(lpSkill != 0 && gEffectManager.CheckEffect(lpObj,gSkillManager.GetSkillEffect(lpSkill->m_index)) == 0)
			{
				this->HelperOfflineUseSkill(lpObj,lpObj,lpSkill);
			}
		}
	}

	if(lpObj->Class == CLASS_FE && lpObj->Helper.AutoHeal != 0)
	{
		CSkill* lpSkill = this->HelperOfflineGetSkill(lpObj,SKILL_HEAL);

		if(lpSkill != 0 && ((lpObj->Life*100)/(lpObj->MaxLife+lpObj->AddLife)) < lpObj->Helper.HealPercent)
		{
			this->HelperOfflineUseSkill(lpObj,lpObj,lpSkill);
		}
	}

	if(lpObj->Class == CLASS_SU && lpObj->Helper.AutoDrainLife != 0)
	{
		CSkill* lpSkill = this->HelperOfflineGetSkill(lpObj,SKILL_DRAIN_LIFE);

		if(lpSkill != 0 && ((lpObj->Life*100)/(lpObj->MaxLife+lpObj->AddLife)) < lpObj->Helper.DrainPercent)
		{
			this->HelperOfflineUseSkill(lpObj,lpObj,lpSkill);
		}
	}

	#endif
}

void CHelperOffline::HelperOfflineAutoPick(LPOBJ lpObj) // OK
{
	#if(GAMESERVER_UPDATE>=603)

	if(gServerInfo.m_HelperOfflinePickEnable[lpObj->AccountLevel] == 0)
	{
		return;
	}

	if(lpObj->Helper.PickAllItem == 0 && lpObj->Helper.PickSelected == 0)
	{
		return;
	}

	for(int n=0;n<MAX_MAP_ITEM;n++)
	{
		CMapItem* lpItem = &gMap[lpObj->Map].m_Item[n];

		if(lpItem->IsItem() == 0)
		{
			continue;
		}

		if(gViewport.CheckViewportObjectPosition(lpObj->Index,lpObj->Map,lpItem->m_X,lpItem->m_Y,lpObj->Helper.Range[1]) != 0)
		{
			PMSG_ITEM_GET_RECV pMsg;

			pMsg.header.set(0x22,sizeof(pMsg));

			pMsg.index[0] = SET_NUMBERHB(n);
			pMsg.index[1] = SET_NUMBERLB(n);

			if(lpObj->Helper.PickAllItem != 0 && lpItem->m_Index != GET_ITEM(14,15))
			{
				gItemManager.CGItemGetRecv(&pMsg,lpObj->Index);
				continue;
			}

			if(lpObj->Helper.PickSelected != 0)
			{
				if(lpObj->Helper.PickJewel != 0 && gItemManager.IsJewelItem(lpItem->m_Index) != 0)
				{
					gItemManager.CGItemGetRecv(&pMsg,lpObj->Index);
					continue;
				}
				
				if(lpObj->Helper.PickSet != 0 && lpItem->IsSetItem() != 0)
				{
					gItemManager.CGItemGetRecv(&pMsg,lpObj->Index);
					continue;
				}

				if(lpObj->Helper.PickExc != 0 && lpItem->IsExcItem() != 0)
				{
					gItemManager.CGItemGetRecv(&pMsg,lpObj->Index);
					continue;
				}

				if(lpObj->Helper.PickMoney != 0 && lpItem->m_Index == GET_ITEM(14,15))
				{
					gItemManager.CGItemGetRecv(&pMsg,lpObj->Index);
					continue;
				}

				if(lpObj->Helper.PickExtra != 0)
				{
					char buff[256] = { 0 };

					memcpy(buff,CharToLower(gItemManager.GetItemName(lpItem->m_Index,lpItem->m_Level)),sizeof(buff));

					if(lpItem->m_Level > 0 && gItemLevel.CheckItemlevel(lpItem->m_Index,lpItem->m_Level) == 0)
					{
						wsprintf(buff,"%s +%d",buff,lpItem->m_Level);
					}

					if(lpItem->m_Option1 != 0)
					{
						wsprintf(buff,"%s +skill",buff);
					}

					if(lpItem->m_Option2 != 0)
					{
						wsprintf(buff,"%s +luck",buff);
					}

					if(lpItem->m_Option3 != 0)
					{
						wsprintf(buff,"%s +option",buff);
					}

					if(lpItem->IsSocketItem() != 0)
					{
						wsprintf(buff,"%s +socket%x",buff,GetSocketOptionCount(lpItem->m_SocketOption));
					}

					for(int i=0;i < MAX_HELPER_ITEM;i++) 
					{ 
						if(strlen(lpObj->Helper.ItemList[i]) < 1)
						{
							continue;
						}

						if(strstr(buff,lpObj->Helper.ItemList[i]) != 0)
						{
							gItemManager.CGItemGetRecv(&pMsg,lpObj->Index);
							break;
						}
					}
				}
			}
		}
	}

	#endif
}

void CHelperOffline::HelperOfflineAutoPetPick(LPOBJ lpObj) // OK
{
	#if(GAMESERVER_UPDATE>=603)

	if(gServerInfo.m_HelperOfflinePickEnable[lpObj->AccountLevel] == 0)
	{
		return;
	}

	if(lpObj->Inventory[INVENTORY_SLOT_HELPER].IsItem() == 0 || lpObj->Inventory[INVENTORY_SLOT_HELPER].m_Durability < 1)
	{
		return;
	}

	if(lpObj->Inventory[INVENTORY_SLOT_HELPER].m_Index != GET_ITEM(13,67) && lpObj->Inventory[INVENTORY_SLOT_HELPER].m_Index != GET_ITEM(13,80) && lpObj->Inventory[INVENTORY_SLOT_HELPER].m_Index != GET_ITEM(13,106) && lpObj->Inventory[INVENTORY_SLOT_HELPER].m_Index != GET_ITEM(13,123)) // Rudolf, Panda, Unicorn, Skeleton
	{
		return;
	}

	for(int n=0;n < MAX_MAP_ITEM;n++)
	{
		if(gMap[lpObj->Map].m_Item[n].IsItem() == 0)
		{
			continue;
		}

		if(gMap[lpObj->Map].m_Item[n].m_Index != GET_ITEM(14,15))
		{
			continue;
		}

		if(gViewport.CheckViewportObjectPosition(lpObj->Index,lpObj->Map,gMap[lpObj->Map].m_Item[n].m_X,gMap[lpObj->Map].m_Item[n].m_Y,4) != 0)
		{
			PMSG_ITEM_GET_RECV pMsg;

			pMsg.header.set(0x22, sizeof(pMsg));

			pMsg.index[0] = SET_NUMBERHB(n);
			pMsg.index[1] = SET_NUMBERLB(n);

			gItemManager.CGItemGetRecv(&pMsg,lpObj->Index);

			continue;
		}
	}

	#endif
}

void CHelperOffline::HelperOfflineAutoReload(LPOBJ lpObj) // OK
{
	#if(GAMESERVER_UPDATE>=603)

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

	#endif
}

void CHelperOffline::HelperOfflineAutoRepair(LPOBJ lpObj) // OK
{
	#if(GAMESERVER_UPDATE>=603)

	if(lpObj->Helper.RepairItem == 0)
	{
		return;
	}

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

		if(gItemManager.RepairItem(lpObj,&lpObj->Inventory[n],n,0) != 0)
		{
			gObjectManager.CharacterCalcAttribute(lpObj->Index);
		}
	}

	#endif
}

void CHelperOffline::HelperOfflineAutoBuffParty(LPOBJ lpObj) // OK
{
	#if(GAMESERVER_UPDATE>=603)

	if(gServerInfo.m_HelperOfflineBuffEnable[lpObj->AccountLevel] == 0)
	{
		return;
	}

	if(OBJECT_RANGE(lpObj->PartyNumber) == 0)
	{
		return;
	}

	if(lpObj->Helper.Party == 0)
	{
		return;
	}

	if(lpObj->Class == CLASS_FE && lpObj->Helper.PartyAutoHeal != 0)
	{
		CSkill* lpSkill = this->HelperOfflineGetSkill(lpObj,SKILL_HEAL);

		if(lpSkill != 0)
		{
			for(int n=0;n < MAX_PARTY_USER;n++)
			{
				int bIndex = gParty.m_PartyInfo[lpObj->PartyNumber].Index[n];

				if(OBJECT_RANGE(bIndex) == 0)
				{
					continue;
				}

				if(lpObj->Index == bIndex)
				{
					continue;
				}

				LPOBJ lpTarget = &gObj[bIndex];

				if(lpTarget->Live == 0 || lpTarget->State != OBJECT_PLAYING || lpTarget->Map != lpObj->Map)
				{
					continue;
				}

				if(((lpTarget->Life*100)/(lpTarget->MaxLife+lpTarget->AddLife)) < lpObj->Helper.PartyHealPercent)
				{
					this->HelperOfflineUseSkill(lpObj,lpTarget,lpSkill);
				}
			}
		}
	}

	if(lpObj->Helper.PartyAutoBuff != 0)
	{
		if((GetTickCount()-lpObj->Helper.TimerBuffParty) >= (DWORD)(lpObj->Helper.PartyBuffTime*1000))
		{
			lpObj->Helper.TimerBuffParty = GetTickCount();

			for(int n=0;n < 3;n++)
			{
				CSkill* lpSkill = this->HelperOfflineGetSkill(lpObj,lpObj->Helper.Buff[n]);
			
				if(lpSkill != 0)
				{
					if(lpSkill->m_skill == SKILL_GREATER_LIFE || lpSkill->m_skill == SKILL_GREATER_CRITICAL_DAMAGE || lpSkill->m_skill == SKILL_INFINITY_ARROW || lpSkill->m_skill == SKILL_SWORD_POWER || lpSkill->m_skill == SKILL_MAGIC_CIRCLE || lpSkill->m_skill == SKILL_GREATER_IGNORE_DEFENSE_RATE || lpSkill->m_skill == SKILL_FITNESS || lpSkill->m_skill == SKILL_GREATER_DEFENSE_SUCCESS_RATE || lpSkill->m_skill == SKILL_BLOOD_HOWLING)
					{
						continue;
					}

					for(int n=0;n < MAX_PARTY_USER;n++)
					{
						int bIndex = gParty.m_PartyInfo[lpObj->PartyNumber].Index[n];

						if(OBJECT_RANGE(bIndex) == 0)
						{
							continue;
						}

						if(lpObj->Index == bIndex)
						{
							continue;
						}

						LPOBJ lpTarget = &gObj[bIndex];

						if(lpTarget->Live == 0 || lpTarget->State != OBJECT_PLAYING || lpTarget->Map != lpObj->Map)
						{
							continue;
						}

						if(gEffectManager.CheckEffect(lpTarget,gSkillManager.GetSkillEffect(lpSkill->m_index)) == 0)
						{
							this->HelperOfflineUseSkill(lpObj,lpTarget,lpSkill);
						}
					}
				}
			}
		}
	}

	#endif
}

void CHelperOffline::HelperOfflineAutoAttack(LPOBJ lpObj) // OK
{
	#if(GAMESERVER_UPDATE>=603)

	for(int n=0;n < 2;++n)
	{
		CSkill* lpSkill = this->HelperOfflineGetSkill(lpObj,lpObj->Helper.Skill[n+1]);

		if(lpSkill == 0 || lpSkill->m_skill == SKILL_HEAL)
		{
			continue;
		}

		if(lpObj->Helper.Combo != 0)
		{
			if(lpObj->ComboSkill.m_index == 0 || lpObj->ComboSkill.m_index == 1)
			{
				if(lpObj->ComboSkill.m_skill[lpObj->ComboSkill.m_index] != lpSkill->m_index)
				{
					if(this->HelperOfflineFindTarget(lpObj,0,0) != 0)
					{
						this->HelperOfflineUseSkill(lpObj,&gObj[lpObj->TargetNumber],lpSkill);
					}
				}
			}

			continue;
		}

		if(lpObj->Helper.SkillDelay[n] == 0 && lpObj->Helper.SkillCondition[n] == 0)
		{
			continue;
		}

		if(lpObj->Helper.SkillCondition[n] != 0)
		{
			if(lpObj->Helper.SkillSubCon[n] == 3 && this->HelperOfflineCountTarget(lpObj) < 6)
			{
				continue;
			}
			else if(lpObj->Helper.SkillSubCon[n] == 2 && this->HelperOfflineCountTarget(lpObj) < 5)
			{
				continue;
			}
			else if(lpObj->Helper.SkillSubCon[n] == 1 && this->HelperOfflineCountTarget(lpObj) < 4)
			{
				continue;
			}
			else if(lpObj->Helper.SkillSubCon[n] == 0 && this->HelperOfflineCountTarget(lpObj) < 3)
			{
				continue;
			}
		}

		if(lpObj->Helper.SkillDelay[n] != 0)
		{
			if((GetTickCount()-lpObj->Helper.TimerDelay[n]) < (DWORD)(lpObj->Helper.Delay[n]*1000))
			{
				continue;
			}
		}

		if(this->HelperOfflineFindTarget(lpObj,lpObj->Helper.SkillPreCon[n],0) != 0)
		{
			if(this->HelperOfflineUseSkill(lpObj,&gObj[lpObj->TargetNumber],lpSkill) != 0)
			{
				lpObj->Helper.TimerDelay[n] = GetTickCount();
			}
		}
	}

	if(lpObj->Helper.Combo == 0 || lpObj->ComboSkill.m_index == -1)
	{
		CSkill* lpSkill = this->HelperOfflineGetSkill(lpObj,lpObj->Helper.Skill[0]);

		if(lpSkill != 0 && lpSkill->m_skill != SKILL_HEAL)
		{
			if(this->HelperOfflineFindTarget(lpObj,0,0) != 0)
			{
				this->HelperOfflineUseSkill(lpObj,&gObj[lpObj->TargetNumber],lpSkill);
			}
		}
	}

	if(lpObj->Helper.RegularAttack != 0)
	{
		if(this->HelperOfflineFindTarget(lpObj,0,0) != 0)
		{
			if(gObjCalcDistance(lpObj,&gObj[lpObj->TargetNumber]) > 2)
			{
				this->HelperOfflineMove(lpObj,gObj[lpObj->TargetNumber].X,gObj[lpObj->TargetNumber].Y);

				return;
			}

			lpObj->Helper.TimerSkill = GetTickCount()+500;

			PMSG_ATTACK_RECV pMsg;

			pMsg.header.set(PROTOCOL_CODE2,sizeof(pMsg));

			pMsg.index[0] = SET_NUMBERHB(lpObj->TargetNumber);

			pMsg.index[1] = SET_NUMBERLB(lpObj->TargetNumber);

			pMsg.action = ACTION_ATTACK1;

			pMsg.dir = GetPathPacketDirPos(gObj[lpObj->TargetNumber].X-lpObj->X,gObj[lpObj->TargetNumber].Y-lpObj->Y);

			gAttack.CGAttackRecv(&pMsg,lpObj->Index);
		}
	}

	#endif
}

bool CHelperOffline::HelperOfflinePotionMP(LPOBJ lpObj,int SkillNumber) // OK
{
	#if(GAMESERVER_UPDATE>=603)

	if(gSkillManager.CheckSkillMana(lpObj,SkillNumber) != 0)
	{
		return 1;
	}

	PMSG_ITEM_USE_RECV pMsg;

	pMsg.header.set(0x26,sizeof(pMsg));

	pMsg.SourceSlot = 0xFF;

	pMsg.TargetSlot = 0xFF;

	pMsg.type = 0;

	for(int n=0;n < 20;n++)
	{
		if(gSkillManager.CheckSkillMana(lpObj,SkillNumber) == 0)
		{
			pMsg.SourceSlot = ((pMsg.SourceSlot==0xFF)?gItemManager.GetInventoryItemSlot(lpObj,GET_ITEM(14,4),-1):pMsg.SourceSlot);

			pMsg.SourceSlot = ((pMsg.SourceSlot==0xFF)?gItemManager.GetInventoryItemSlot(lpObj,GET_ITEM(14,5),-1):pMsg.SourceSlot);

			pMsg.SourceSlot = ((pMsg.SourceSlot==0xFF)?gItemManager.GetInventoryItemSlot(lpObj,GET_ITEM(14,6),-1):pMsg.SourceSlot);

			if(lpObj->Helper.PotionElite != 0){ pMsg.SourceSlot = ((pMsg.SourceSlot==0xFF)?gItemManager.GetInventoryItemSlot(lpObj,GET_ITEM(14,71),-1):pMsg.SourceSlot); }

			if(pMsg.SourceSlot == 0xFF) 
			{ 
				break;
			}

			gItemManager.CGItemUseRecv(&pMsg,lpObj->Index);
		}
		else
		{
			return 1;
		}
	}

	return 0;

	#endif
}

void CHelperOffline::HelperOfflineMove(LPOBJ lpObj,int x,int y) // OK
{
	#if(GAMESERVER_UPDATE>=603)

	if(gEffectManager.CheckStunEffect(lpObj) != 0 || gEffectManager.CheckImmobilizeEffect(lpObj) != 0)
	{
		return;
	}

	PATH_INFO path;

	BYTE bPath[8];

	if(gMap[lpObj->Map].PathFinding2(lpObj->X,lpObj->Y,x,y,&path) != 0)
	{
		short tx;
		short ty;
		short sx = lpObj->X;
		short sy = lpObj->Y;
		BYTE pos = 0;

		memset(bPath,0,sizeof(bPath));
		bPath[0] = (lpObj->Dir*16)+((path.PathNum-1)&0x0F);
		
		for(int n=1;n < path.PathNum;++n)
		{
			tx = path.PathX[n];
			ty = path.PathY[n];
			pos = GetPathPacketDirPos(tx-sx,ty-sy);
			sx = tx;
			sy = ty;

			if((n%2) == 1)
			{
				bPath[(n+1)/2] = (pos*16);
			}
			else
			{
				bPath[(n+1)/2] |= pos;
			}
		}

		bPath[0] &= 0x0F;
		bPath[0] |= pos << 4;

		PMSG_MOVE_RECV pMsg;

		pMsg.header.set(PROTOCOL_CODE1,sizeof(pMsg));

		pMsg.x = (BYTE)lpObj->X;

		pMsg.y = (BYTE)lpObj->Y;

		memcpy(pMsg.path,bPath,sizeof(pMsg.path));

		CGMoveRecv((PMSG_MOVE_RECV*)&pMsg,lpObj->Index);
	}

	#endif
}

int CHelperOffline::HelperOfflineCountTarget(LPOBJ lpObj) // OK
{
	#if(GAMESERVER_UPDATE>=603)

	int count = 0;

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

		if(gObjCalcDistance(lpObj,&gObj[lpObj->VpPlayer2[n].index]) >= lpObj->Helper.Range[0])
		{
			continue;
		}

		count++;
	}

	return count;

	#else

	return 0;

	#endif
}

int CHelperOffline::HelperOfflineFindTarget(LPOBJ lpObj,int self,int another) // OK
{
	#if(GAMESERVER_UPDATE>=603)

	lpObj->TargetNumber = -1;

	int NearestDistance = lpObj->Helper.Range[0];

	for(int n=0;n < MAX_VIEWPORT;n++)
	{
		if(lpObj->VpPlayer2[n].state == VIEWPORT_NONE || OBJECT_RANGE(lpObj->VpPlayer2[n].index) == 0 || lpObj->VpPlayer2[n].type != OBJECT_MONSTER)
		{
			continue;
		}

		if(another != 0 && lpObj->VpPlayer2[n].index == lpObj->TargetNumber)
		{
			continue;
		}

		if(gMap[lpObj->Map].CheckWall(lpObj->X,lpObj->Y,gObj[lpObj->VpPlayer2[n].index].X,gObj[lpObj->VpPlayer2[n].index].Y) == 0)
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

		if(self != 0 && gObj[lpObj->VpPlayer2[n].index].TargetNumber != lpObj->Index)
		{
			continue;
		}

		NearestDistance = gObjCalcDistance(lpObj,&gObj[lpObj->VpPlayer2[n].index]);

		lpObj->TargetNumber = lpObj->VpPlayer2[n].index;
	}

	return ((lpObj->TargetNumber==-1)?0:1);

	#else

	return 0;

	#endif
}

CSkill* CHelperOffline::HelperOfflineGetSkill(LPOBJ lpObj,int SkillNumber) // OK
{
	#if(GAMESERVER_UPDATE>=603)

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

	#else

	return 0;

	#endif
}

bool CHelperOffline::HelperOfflineUseSkill(LPOBJ lpObj,LPOBJ lpTarget,CSkill* lpSkill)// OK
{
	#if(GAMESERVER_UPDATE>=603)

	if(lpSkill == 0)
	{
		return 0;
	}

	HELPER_SKILL_INFO* lpInfo = this->GetInfo(lpSkill->m_skill);

	if(lpInfo == 0)
	{
		return 0;
	}

	if(GetTickCount() < lpObj->Helper.TimerSkill)
	{
		return 0;
	}

	if(gSkillManager.CheckSkillBP(lpObj,lpSkill->m_index) == 0)
	{
		return 0;
	}

	if(this->HelperOfflinePotionMP(lpObj,lpSkill->m_index) == 0)
	{
		return 0;
	}

	int range = (((range=gSkillManager.GetSkillRange(lpSkill->m_index))==0)?1:range);

	if(lpObj->Helper.ShortDistance != 0)
	{
		range = 2;
	}

	if(gObjCalcDistance(lpObj,lpTarget) > range)
	{
		this->HelperOfflineMove(lpObj,lpTarget->X,lpTarget->Y);

		return 0;
	}

	if(gMap[lpObj->Map].CheckWall(lpObj->X,lpObj->Y,lpTarget->X,lpTarget->Y) == 0)
	{
		this->HelperOfflineFindTarget(lpObj,0,1);
		return 0;
	}

	switch(lpInfo->Group)
	{
		case 0:
		case 1:
			gCustomAttack.CustomAttackSkillAttack(lpObj,lpTarget->Index,lpInfo->Skill);
			break;
		case 2:
			gCustomAttack.CustomAttackMultilAttack(lpObj,lpTarget->Index,lpInfo->Skill);
			break;
		case 3:
			gCustomAttack.CustomAttackDurationlAttack(lpObj,lpTarget->Index,lpInfo->Skill);
			break;
		case 4:
			gCustomAttack.CustomAttackRageAttack(lpObj,lpTarget->Index,lpInfo->Skill);
			break;
	}

	DWORD SkillDelay = gSkillManager.GetSkillDelay(lpSkill->m_index);

	if(SkillDelay > 0)
	{
		lpObj->Helper.TimerSkill = GetTickCount()+SkillDelay;
	}
	else
	{
		lpObj->Helper.TimerSkill = GetTickCount()+(DWORD)((lpInfo->MainSpeed*lpObj->PhysiSpeed)>lpInfo->BaseSpeed?0:lpInfo->BaseSpeed-(lpInfo->MainSpeed*lpObj->PhysiSpeed));
	}

	return 1;

	#else

	return 0;

	#endif
}