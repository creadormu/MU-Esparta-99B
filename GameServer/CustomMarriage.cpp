// CustomMarriage.cpp: implementation of the CCustomMarriage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomMarriage.h"
#include "CustomArena.h"
#include "CommandManager.h"
#include "DSProtocol.h"
#include "GameMain.h"
#include "ItemManager.h"
#include "Log.h"
#include "Map.h"
#include "MemScript.h"
#include "Message.h"
#include "Notice.h"
#include "ServerInfo.h"
#include "Util.h"

CCustomMarriage gCustomMarriage;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void CCustomMarriage::ReadCustomMarriageInfo(char* section,char* path) // OK
{
	this->m_CustomMarriageSwitch = GetPrivateProfileInt(section,"CustomMarriageSwitch",0,path);
	
	this->m_CustomMarriageHomoSexual = GetPrivateProfileInt(section,"CustomMarriageHomoSexual",0,path);
	
	this->m_CustomMarriageAnnounce = GetPrivateProfileInt(section,"CustomMarriageAnnounce",0,path);
	
	this->m_CustomMarriageMapZone = GetPrivateProfileInt(section,"CustomMarriageMapZone",0,path);
	
	this->m_CustomMarriageStartX = GetPrivateProfileInt(section,"CustomMarriageStartX",0,path);
	
	this->m_CustomMarriageStartY = GetPrivateProfileInt(section,"CustomMarriageStartY",0,path);
	
	this->m_CustomMarriageEndX = GetPrivateProfileInt(section,"CustomMarriageEndX",0,path);
	
	this->m_CustomMarriageEndY = GetPrivateProfileInt(section,"CustomMarriageEndY",0,path);
	
	this->m_CustomMarriageTracePK = GetPrivateProfileInt(section,"CustomMarriageTracePK",0,path);
}

void CCustomMarriage::Load(char*path) // OK
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

	this->m_MarriageWeedingGift.clear();

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

					this->m_WeddingEnable = lpMemScript->GetNumber();
					
					this->m_WeddingItemIndex = SafeGetItem(GET_ITEM(lpMemScript->GetAsNumber(),lpMemScript->GetAsNumber()));
					
					this->m_WeddingItemLevel = lpMemScript->GetAsNumber();
					
					this->m_WeddingItemCount = lpMemScript->GetAsNumber();
				}
				else if(section == 1)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					this->m_DivorseEnable = lpMemScript->GetNumber();
					
					this->m_DivorseItemIndex = SafeGetItem(GET_ITEM(lpMemScript->GetAsNumber(),lpMemScript->GetAsNumber()));
					
					this->m_DivorseItemLevel = lpMemScript->GetAsNumber();
					
					this->m_DivorseItemCount = lpMemScript->GetAsNumber();
				}
				else if(section == 2)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					MARRIAGE_WEDDING_GIFT_INFO info;

					memset(&info,0,sizeof(info));

					info.ItemIndex = SafeGetItem(GET_ITEM(lpMemScript->GetNumber(),lpMemScript->GetAsNumber()));

					info.Level = lpMemScript->GetAsNumber();

					info.Dur = lpMemScript->GetAsNumber();

					info.Option1 = lpMemScript->GetAsNumber();

					info.Option2 = lpMemScript->GetAsNumber();

					info.Option3 = lpMemScript->GetAsNumber();

					info.NewOption = lpMemScript->GetAsNumber();

					info.SetOption = lpMemScript->GetAsNumber();

					#if(GAMESERVER_UPDATE>=401)

					info.SocketCount = lpMemScript->GetAsNumber();

					info.Duration = lpMemScript->GetAsNumber();

					#endif

					this->m_MarriageWeedingGift.push_back(info);
				}
				else
				{
					break;
				}
			}
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CCustomMarriage::CommandPropuse(LPOBJ lpObj,char* arg) // OK
{
	if(this->m_CustomMarriageSwitch == 0)
	{
		return;
	}

	char name[11] = {0};

	gCommandManager.GetString(arg,name,sizeof(name),0);

	LPOBJ lpTarget = gObjFind(name);

	if(lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(369));
		return;
	}

	if(lpObj->Index == lpTarget->Index)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(650));
		return;
	}

	if(lpObj->Interface.use != 0 || lpTarget->Interface.use != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(651));
		return;
	}

	if(lpObj->Married != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(652),lpObj->MarryName);
		return;
	}

	if(lpTarget->Married != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(653),lpTarget->Name);
		return;
	}

	if(this->CheckSex(lpObj) == this->CheckSex(lpTarget) && this->m_CustomMarriageHomoSexual == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(654));
		return;
	}

	if(this->CheckPosition(lpObj,lpTarget) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(655));
		return;
	}

	if(this->CheckWeddingRequerimentItem(lpObj) == 0 || this->CheckWeddingRequerimentItem(lpTarget) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(656),this->m_WeddingItemCount,gItemManager.GetItemName(this->m_WeddingItemIndex,this->m_WeddingItemLevel));
		return;
	}

	lpTarget->MarryRequested = 1;

	lpTarget->MarryRequestIndex = lpObj->Index;
	
	lpTarget->MarryRequestTime = GetTickCount();

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(657));
	
	gNotice.GCNoticeSend(lpTarget->Index,1,0,0,0,0,0,gMessage.GetMessage(658),lpObj->Name);

	gLog.Output(LOG_COMMAND,"[CommandMarry][%s][%s] Request of marry to [%s][%s]",lpObj->Account,lpObj->Name,lpTarget->Account,lpTarget->Name);

	gCommandManager.DiscountRequirement(lpObj,COMMAND_CUSTOM_MARRIAGE1);
}

void CCustomMarriage::CommandAccept(LPOBJ lpObj) // OK
{
	if(this->m_CustomMarriageSwitch == 0)
	{
		return;
	}

	if(lpObj->MarryRequested == 0)
	{
		return;
	}

	if(GetTickCount() - lpObj->MarryRequestTime > 60000)
	{
		lpObj->MarryRequested = 0;

		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(659));
		return;
	}

	if(lpObj->Interface.use != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(651));
		return;
	}

	LPOBJ lpTarget = &gObj[lpObj->MarryRequestIndex];

	if(lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(660));
		return;
	}

	if(this->CheckPosition(lpObj,lpTarget) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(655));
		return;
	}

	if(this->CheckWeddingRequerimentItem(lpObj) == 0 || this->CheckWeddingRequerimentItem(lpTarget) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(656),this->m_WeddingItemCount,gItemManager.GetItemName(this->m_WeddingItemIndex,this->m_WeddingItemLevel));
		return;
	}

	lpObj->Married = 1;
	
	lpObj->MarryRequestIndex = 0;
	
	lpObj->MarryRequestTime = 0;
	
	strcpy_s(lpObj->MarryName,lpTarget->Name);

	if(this->m_WeddingEnable != 0)
	{
		gItemManager.DeleteInventoryItemCount(lpObj,this->m_WeddingItemIndex,this->m_WeddingItemLevel,this->m_DivorseItemCount);
	}
	
	this->GiveWeddingGift(lpObj);

	GDCharacterInfoSaveSend(lpObj->Index);

	lpTarget->Married = 1;
	
	lpTarget->MarryRequestIndex = 0;
	
	lpTarget->MarryRequestTime = 0;
	
	strcpy_s(lpTarget->MarryName,lpObj->Name);

	if(this->m_WeddingEnable != 0)
	{
		gItemManager.DeleteInventoryItemCount(lpTarget,this->m_WeddingItemIndex,this->m_WeddingItemLevel,this->m_DivorseItemCount);
	}
	
	this->GiveWeddingGift(lpTarget);

	GDCharacterInfoSaveSend(lpTarget->Index);

	for(int n=0;n < 10;n++)
	{
		GCServerCommandSend(lpObj->Index, 0, (lpObj->X + (((GetLargeRand() % 5) * 2) - 4)), (lpObj->Y + (((GetLargeRand() % 5) * 2) - 4)), 1);
	}

	gNotice.GCNoticeSend(lpObj->Index,0,0,0,0,0,0,gMessage.GetMessage(661),lpTarget->Name);
	
	gNotice.GCNoticeSend(lpTarget->Index,0,0,0,0,0,0,gMessage.GetMessage(661),lpObj->Name);

	if(this->m_CustomMarriageAnnounce != 0)
	{
		gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(662),lpObj->Name,lpTarget->Name);
	}

	gLog.Output(LOG_COMMAND,"[CommandMarry][%s][%s] New marriage with [%s][%s]",lpObj->Account,lpObj->Name,lpTarget->Account,lpTarget->Name);

	gCommandManager.DiscountRequirement(lpObj,COMMAND_CUSTOM_MARRIAGE2);
}

void CCustomMarriage::CommandDivorce(LPOBJ lpObj) // OK
{
	if(this->m_CustomMarriageSwitch == 0)
	{
		return;
	}

	if(lpObj->Married == 0)
	{
		return;
	}

	if(lpObj->Interface.use != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(663));
		return;
	}

	if(this->CheckDivorceRequerimentItem(lpObj) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(664),this->m_DivorseItemCount,gItemManager.GetItemName(this->m_DivorseItemIndex,this->m_DivorseItemLevel));
		return;
	}

	LPOBJ lpTarget = gObjFind(lpObj->MarryName);

	if(lpTarget == 0)
	{
		SDHP_COMMAND_DIVORCE_SEND pMsg;

		pMsg.header.set(0x0F,0x02,sizeof(pMsg));

		pMsg.index = lpObj->Index;
		
		memcpy(pMsg.name,lpObj->MarryName,sizeof(pMsg.name));

		gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,lpObj->Index);
	}
	else
	{
		lpTarget->Married = 0;
		
		memset(lpTarget->MarryName,0,sizeof(lpTarget->MarryName));
		
		GDCharacterInfoSaveSend(lpTarget->Index);
		
		gNotice.GCNoticeSend(lpTarget->Index,0,0,0,0,0,0,gMessage.GetMessage(665));
	}

	gLog.Output(LOG_COMMAND,"[CommandDivorce][%s][%s] He divorces to [%s]",lpObj->Account,lpObj->Name,lpObj->MarryName);

	lpObj->Married = 0;
	
	memset(lpObj->MarryName,0,sizeof(lpObj->MarryName));

	if(this->m_DivorseEnable != 0)
	{
		gItemManager.DeleteInventoryItemCount(lpObj,this->m_DivorseItemIndex,this->m_DivorseItemLevel,this->m_DivorseItemCount);
	}

	GDCharacterInfoSaveSend(lpObj->Index);

	gNotice.GCNoticeSend(lpObj->Index,0,0,0,0,0,0,gMessage.GetMessage(665));

	gCommandManager.DiscountRequirement(lpObj,COMMAND_CUSTOM_MARRIAGE3);
}

void CCustomMarriage::CommandTrackMarry(LPOBJ lpObj) // OK
{
	if(this->m_CustomMarriageSwitch == 0)
	{
		return;
	}

	if(lpObj->Married == 0)
	{
		return;
	}

	if(lpObj->Permission[12] != 0)
	{
		return;
	}

	LPOBJ lpTarget = gObjFind(lpObj->MarryName);

	if(lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(666));
		return;
	}

	if(lpObj->Interface.use != 0 || gCustomArena.CheckMap(lpTarget->Map) != 0 || KALIMA_MAP_RANGE(lpTarget->Map) != 0 || DS_MAP_RANGE(lpTarget->Map) != 0 || BC_MAP_RANGE(lpTarget->Map) != 0 || CC_MAP_RANGE(lpTarget->Map) != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(667));
		return;
	}

	#if(GAMESERVER_UPDATE>=301)

	if(IT_MAP_RANGE(lpTarget->Map) != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(667));
		return;
	}

	#endif

	#if(GAMESERVER_UPDATE>=603)

	if(DA_MAP_RANGE(lpTarget->Map) != 0 || DG_MAP_RANGE(lpTarget->Map) != 0 || IG_MAP_RANGE(lpTarget->Map) != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(667));
		return;
	}

	#endif

	#if(GAMESERVER_UPDATE>=803)

	if(CF_MAP_RANGE(lpTarget->Map) != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(667));
		return;
	}

	#endif

	#if(GAMESERVER_UPDATE>=201)
	if((lpTarget->Map == MAP_ICARUS || lpTarget->Map == MAP_KANTURU3) && (lpObj->Inventory[INVENTORY_SLOT_WING].IsItem() == 0 && lpObj->Inventory[INVENTORY_SLOT_HELPER].m_Index != GET_ITEM(13,3) && lpObj->Inventory[INVENTORY_SLOT_HELPER].m_Index != GET_ITEM(13,37))) // Dinorant,Fenrir
	#else
	if(lpTarget->Map == MAP_ICARUS && lpObj->Inventory[INVENTORY_SLOT_WING].IsItem() == 0 && lpObj->Inventory[INVENTORY_SLOT_HELPER].m_Index != GET_ITEM(13,3) && lpObj->Inventory[INVENTORY_SLOT_HELPER].m_Index != GET_ITEM(13,37)) // Dinorant,Fenrir
	#endif
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(667));
		return;
	}

	if(lpObj->PKLevel > 4 && this->m_CustomMarriageTracePK != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(668));
		return;
	}

	gObjTeleport(lpObj->Index,lpTarget->Map,lpTarget->X,lpTarget->Y);

	gLog.Output(LOG_COMMAND,"[CommandMarryTrack][%s][%s] - Follow couple",lpObj->Account,lpObj->Name);

	gCommandManager.DiscountRequirement(lpObj,COMMAND_CUSTOM_MARRIAGE4);
}

bool CCustomMarriage::CheckSex(LPOBJ lpObj) // OK
{
	if(lpObj->Class == CLASS_FE || lpObj->Class == CLASS_SU)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool CCustomMarriage::CheckPosition(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	if(this->m_CustomMarriageMapZone != lpObj->Map || this->m_CustomMarriageMapZone != lpTarget->Map)
	{
		return 0;
	}

	if((lpObj->X < this->m_CustomMarriageStartX || lpObj->X > this->m_CustomMarriageEndX) || (lpObj->Y < this->m_CustomMarriageStartY || lpObj->Y > this->m_CustomMarriageEndY))
	{
		return 0;
	}

	if((lpTarget->X < this->m_CustomMarriageStartX || lpTarget->X > this->m_CustomMarriageEndX) || (lpTarget->Y < this->m_CustomMarriageStartY || lpTarget->Y > this->m_CustomMarriageEndY))
	{
		return 0;
	}

	return 1;
}

bool CCustomMarriage::CheckWeddingRequerimentItem(LPOBJ lpObj) // OK
{
	if(this->m_WeddingEnable == 0)
	{
		return 1;
	}

	if(gItemManager.GetInventoryItemCount(lpObj,this->m_WeddingItemIndex,this->m_WeddingItemLevel) >= this->m_WeddingItemCount)
	{
		return 1;
	}

	return 0;
}

bool CCustomMarriage::CheckDivorceRequerimentItem(LPOBJ lpObj) // OK
{
	if(this->m_DivorseEnable == 0)
	{
		return 1;
	}

	if(gItemManager.GetInventoryItemCount(lpObj,this->m_DivorseItemIndex,this->m_DivorseItemLevel) >= this->m_DivorseItemCount)
	{
		return 1;
	}

	return 0;
}

void CCustomMarriage::GiveWeddingGift(LPOBJ lpObj) // OK
{
	for(std::vector<MARRIAGE_WEDDING_GIFT_INFO>::iterator It = this->m_MarriageWeedingGift.begin(); It != this->m_MarriageWeedingGift.end(); It++)
	{
		BYTE ItemSocketOption[MAX_SOCKET_OPTION] = { 0xFF,0xFF,0xFF,0xFF,0xFF };

		for(int u = 0; u < It->SocketCount && It->SocketCount <= MAX_SOCKET_OPTION; u++)
		{
			ItemSocketOption[u] = 0xFE;
		}

		GDCreateItemSend(lpObj->Index,0xEB,0,0,It->ItemIndex,It->Level,It->Dur,It->Option1,It->Option2,It->Option3,-1,It->NewOption,It->SetOption,0,0,ItemSocketOption,0xFF,((It->Duration>0) ? ((DWORD)time(0)+It->Duration) : 0));
	}
}