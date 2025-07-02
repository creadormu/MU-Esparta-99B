// GiftSystem.cpp: interface for the CGiftSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomBuyVip.h"
#include "CommandManager.h"
#include "JSProtocol.h"
#include "Log.h"
#include "MemScript.h"
#include "Message.h"
#include "Notice.h"
#include "Util.h"

CCustomBuyVip gCustomBuyVip;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomBuyVip::CCustomBuyVip() // OK
{

}

CCustomBuyVip::~CCustomBuyVip() // OK
{

}

void CCustomBuyVip::Load(char* path) // OK
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

	this->m_CustomBuyVipInfo.clear();

	try
	{
		while (true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			if(strcmp("end",lpMemScript->GetString()) == 0)
			{
				break;
			}

			CUSTOM_BUY_VIP_INFO info;

			memset(&info,0,sizeof(info));

			info.AccountLevel = lpMemScript->GetNumber();

			info.NewAccountLevel = lpMemScript->GetAsNumber();
			
			info.Days = lpMemScript->GetAsNumber();
			
			info.Money = lpMemScript->GetAsNumber();
			
			#if(GAMESERVER_SHOP>=1)

			info.Coin1 = lpMemScript->GetAsNumber();
			
			#endif

			#if(GAMESERVER_SHOP==3)

			info.Coin2 = lpMemScript->GetAsNumber();
			
			info.Coin3 = lpMemScript->GetAsNumber();

			#endif
			
			strcpy_s(info.Code,lpMemScript->GetAsString());

			this->m_CustomBuyVipInfo.insert(std::pair<std::string,CUSTOM_BUY_VIP_INFO>(info.Code,info));
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CCustomBuyVip::CommandBuyVip(LPOBJ lpObj,char* arg) // OK
{
	char plan[32] = {0};

	gCommandManager.GetString(arg,plan,sizeof(plan),0);

	std::map<std::string,CUSTOM_BUY_VIP_INFO>::iterator it = this->m_CustomBuyVipInfo.find(plan);

	if(it == this->m_CustomBuyVipInfo.end())
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(674));
		return;
	}

	if(it->second.AccountLevel != -1 && it->second.AccountLevel > lpObj->AccountLevel)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(675));
		return;
	}

	if(it->second.Money > lpObj->Money)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(676),it->second.Money);
		return;
	}

	if(it->second.Coin1 > lpObj->Coin1)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(677),it->second.Coin1);
		return;
	}

	if(it->second.Coin2 > lpObj->Coin2)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(678),it->second.Coin2);
		return;
	}

	if(it->second.Coin3 > lpObj->Coin3)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(679),it->second.Coin3);
		return;
	}

	if(it->second.Money > 0)
	{
		lpObj->Money -= it->second.Money;
		GCMoneySend(lpObj->Index,lpObj->Money);
	}

	if(it->second.Coin1 > 0 || it->second.Coin2 > 0 || it->second.Coin3 > 0)
	{
		gObjCoinSub(lpObj->Index,it->second.Coin1,it->second.Coin2,it->second.Coin3);
	}
	
	GJAccountLevelSaveSend(lpObj->Index,it->second.NewAccountLevel,(it->second.Days*86400));
	GJAccountLevelSend(lpObj->Index);

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(680));

	gLog.Output(LOG_COMMAND,"[CommandBuyVip][%s][%s] - (Adquired plan: %s)",lpObj->Account,lpObj->Name,plan);

	gCommandManager.DiscountRequirement(lpObj,COMMAND_CUSTOM_BUY_VIP);
}