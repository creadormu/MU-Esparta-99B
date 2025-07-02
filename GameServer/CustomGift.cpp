// CustomGift.cpp: implementation of the CCustomGift class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomGift.h"
#include "CommandManager.h"
#include "DSProtocol.h"
#include "GameMain.h"
#include "ItemManager.h"
#include "Log.h"
#include "MemScript.h"
#include "Message.h"
#include "Notice.h"
#include "Util.h"

CCustomGift gCustomGift;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomGift::CCustomGift() // OK
{
	this->m_GiftListInfo.clear();
}

CCustomGift::~CCustomGift() // OK
{

}

void CCustomGift::ReadCustomGiftInfo(char* section,char* path) // OK
{
	this->m_CustomGiftSwitch = GetPrivateProfileInt(section,"CustomGiftSwitch",0,path);
}

void CCustomGift::Load(char* path) // OK
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

	this->m_GiftListInfo.clear();

	try
	{
		while (true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			int section = lpMemScript->GetNumber();

			while (true)
			{
				if(section == 0)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					GIFT_LIST_INFO info;

					memset(&info,0,sizeof(info));

					info.Index = lpMemScript->GetNumber();

					info.Class = lpMemScript->GetAsNumber();

					info.Enable[0] = lpMemScript->GetAsNumber();

					info.Enable[1] = lpMemScript->GetAsNumber();

					info.Enable[2] = lpMemScript->GetAsNumber();

					info.Enable[3] = lpMemScript->GetAsNumber();

					info.MinLevel = lpMemScript->GetAsNumber();

					info.MaxLevel = lpMemScript->GetAsNumber();

					info.MinReset = lpMemScript->GetAsNumber();
					
					info.MaxReset = lpMemScript->GetAsNumber();
					
					info.MaxCount = lpMemScript->GetAsNumber();
					
					info.LevelUpPoint = lpMemScript->GetAsNumber();
					
					info.Money = lpMemScript->GetAsNumber();
					
					#if(GAMESERVER_SHOP>=1)

					info.Coin1 = lpMemScript->GetAsNumber();

					#endif
					
					#if(GAMESERVER_SHOP==3)

					info.Coin2 = lpMemScript->GetAsNumber();
					
					info.Coin3 = lpMemScript->GetAsNumber();

					#endif

					strcpy_s(info.Code,lpMemScript->GetAsString());

					this->SetInfo(info);
				}
				else if(section == 1)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					GIFT_LIST_ITEM_INFO info;

					memset(&info,0,sizeof(info));

					info.Index = lpMemScript->GetNumber();
					
					info.ItemIndex = SafeGetItem(GET_ITEM(lpMemScript->GetAsNumber(),lpMemScript->GetAsNumber()));
					
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

					this->SetItemInfo(info);
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

void CCustomGift::SetInfo(GIFT_LIST_INFO info) // OK
{
	std::map<int,GIFT_LIST_INFO>::iterator it = this->m_GiftListInfo.find(info.Index);

	if(it != this->m_GiftListInfo.end())
	{
		return;
	}

	this->m_GiftListInfo.insert(std::pair<int,GIFT_LIST_INFO>(info.Index,info));
}

void CCustomGift::SetItemInfo(GIFT_LIST_ITEM_INFO info) // OK
{
	std::map<int,GIFT_LIST_INFO>::iterator it = this->m_GiftListInfo.find(info.Index);

	if(it == this->m_GiftListInfo.end())
	{
		return;
	}

	it->second.ItemList.push_back(info);
}

bool CCustomGift::CommandGift(LPOBJ lpObj,char* arg) // OK
{
	if(this->m_CustomGiftSwitch == 0)
	{
		return 0;
	}

	char mode[32] = {0};

	gCommandManager.GetString(arg,mode,sizeof(mode),0);

	for(std::map<int,GIFT_LIST_INFO>::iterator it = this->m_GiftListInfo.begin(); it != this->m_GiftListInfo.end(); it++)
	{
		if(strcmp(mode,it->second.Code) != 0)
		{
			continue;
		}

		if(it->second.Class != -1 && lpObj->Class != it->second.Class)
		{
			continue;
		}

		if(it->second.Enable[lpObj->AccountLevel] == 0)
		{
			continue;
		}

		if(it->second.MinLevel != -1 && lpObj->Level < it->second.MinLevel)
		{
			gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(606));
			return 0;
		}

		if(it->second.MaxLevel != -1 && lpObj->Level > it->second.MaxLevel)
		{
			gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(607));
			return 0;
		}

		if(it->second.MinReset != -1 && lpObj->Reset < it->second.MinReset)
		{
			gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(608));
			return 0;
		}

		if(it->second.MaxReset != -1 && lpObj->Reset > it->second.MaxReset)
		{
			gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(609));
			return 0;
		}

		SDHP_COMMAND_GIFT_SEND pMsg;

		pMsg.header.set(0x0F,0x03,sizeof(pMsg));

		pMsg.index = lpObj->Index;
		memcpy(pMsg.Name,lpObj->Name,sizeof(pMsg.Name));
		pMsg.GiftIndex = it->second.Index;
		pMsg.MaxCount = it->second.MaxCount;

		gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,lpObj->Index);

		return 1;
	}

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(605));

	return 0;
}

void CCustomGift::DGCommandGiftRecv(SDHP_COMMAND_GIFT_RECV* lpMsg) // OK
{
	if(gObjIsConnected(lpMsg->index) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	if(lpMsg->Result == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(611));
		return;
	}

	std::map<int,GIFT_LIST_INFO>::iterator it = this->m_GiftListInfo.find(lpMsg->GiftIndex);


	if(it != this->m_GiftListInfo.end())
	{
		if(it->second.LevelUpPoint > 0)
		{
			lpObj->LevelUpPoint += it->second.LevelUpPoint;
			GCNewCharacterInfoSend(lpObj);
			GDCharacterInfoSaveSend(lpObj->Index);
		}

		if(it->second.Money > 0)
		{
			if(gObjCheckMaxMoney(lpObj->Index,it->second.Money) == 0)
			{
				lpObj->Money = MAX_MONEY;
			}
			else
			{
				lpObj->Money += it->second.Money;
			}

			GCMoneySend(lpObj->Index,lpObj->Money);
		}

		if(it->second.Coin1 > 0 || it->second.Coin2 > 0 || it->second.Coin3 > 0)
		{
			gObjCoinAdd(lpObj->Index,it->second.Coin1,it->second.Coin2,it->second.Coin3);
			gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(183),it->second.Coin1,it->second.Coin2,it->second.Coin3);
		}

		int px = lpObj->X;
		int py = lpObj->Y;

		for(std::vector<GIFT_LIST_ITEM_INFO>::iterator ir = it->second.ItemList.begin(); ir != it->second.ItemList.end(); ir++)
		{
			BYTE ItemSocketOption[MAX_SOCKET_OPTION] = { 0xFF,0xFF,0xFF,0xFF,0xFF };

			for(int u = 0; u < ir->SocketCount && ir->SocketCount <= MAX_SOCKET_OPTION; u++)
			{
				ItemSocketOption[u] = 0xFE;
			}

			if(gObjGetRandomFreeLocation(lpObj->Map,&px,&py,2,2,10) == 0)
			{
				px = lpObj->X;
				py = lpObj->Y;
			}

			GDCreateItemSend(lpObj->Index,lpObj->Map,px,py,ir->ItemIndex,ir->Level,ir->Dur,ir->Option1,ir->Option2,ir->Option3,lpObj->Index,ir->NewOption,ir->SetOption,0,0,ItemSocketOption,0xFF,((ir->Duration>0) ? ((DWORD)time(0)+ir->Duration) : 0));
		}

		GCServerCommandSend(lpObj->Index,0,lpObj->X,lpObj->Y,1);

		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(612));

		gLog.Output(LOG_COMMAND,"[CommandGift][%s][%s] - (Active Gift: %d %s)",lpObj->Account,lpObj->Name,it->second.Index,it->second.Code);
	}
}