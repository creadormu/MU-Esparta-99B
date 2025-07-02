// CommandManager.cpp: implementation of the CCommandManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommandManager.h"
#include "BadSyntax.h"
#include "QueryManager.h"
#include "SocketManager.h"
#include "Util.h"

CCommandManager gCommandManager;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCommandManager::CCommandManager() // OK
{

}

CCommandManager::~CCommandManager() // OK
{

}

void CCommandManager::GDCommandResetRecv(SDHP_COMMAND_RESET_RECV* lpMsg,int index) // OK
{
	SDHP_COMMAND_RESET_SEND pMsg;

	pMsg.header.set(0x0F,0x00,sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account,lpMsg->account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpMsg->name,sizeof(pMsg.name));

	gQueryManager.ExecQuery("EXEC WZ_GetResetInfo '%s','%s'",lpMsg->account,lpMsg->name);

	gQueryManager.Fetch();

	pMsg.ResetDay = gQueryManager.GetAsInteger("ResetDay");

	pMsg.ResetWek = gQueryManager.GetAsInteger("ResetWek");

	pMsg.ResetMon = gQueryManager.GetAsInteger("ResetMon");

	gQueryManager.Close();

	gSocketManager.DataSend(index,(BYTE*)&pMsg,pMsg.header.size);
}

void CCommandManager::GDCommandMasterResetRecv(SDHP_COMMAND_MASTER_RESET_RECV* lpMsg,int index) // OK
{
	SDHP_COMMAND_MASTER_RESET_SEND pMsg;

	pMsg.header.set(0x0F,0x01,sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account,lpMsg->account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpMsg->name,sizeof(pMsg.name));

	gQueryManager.ExecQuery("EXEC WZ_GetMasterResetInfo '%s','%s'",lpMsg->account,lpMsg->name);

	gQueryManager.Fetch();

	pMsg.MasterResetDay = gQueryManager.GetAsInteger("MasterResetDay");

	pMsg.MasterResetWek = gQueryManager.GetAsInteger("MasterResetWek");

	pMsg.MasterResetMon = gQueryManager.GetAsInteger("MasterResetMon");

	gQueryManager.Close();

	gSocketManager.DataSend(index,(BYTE*)&pMsg,pMsg.header.size);
}

void CCommandManager::GDCommandDivorceRecv(SDHP_COMMAND_DIVORCE_RECV* lpMsg, int index) // OK
{
	gQueryManager.ExecQuery("UPDATE Character SET Married=0, MarryCouple=NULL WHERE Name='%s'", lpMsg->name);

	gQueryManager.Close();
}

void CCommandManager::GDCommandGiftRecv(SDHP_COMMAND_GIFT_RECV* lpMsg, int index) // OK
{
	SDHP_COMMAND_GIFT_SEND pMsg;

	pMsg.header.set(0x0F, 0x03, sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.Name, lpMsg->Name, sizeof(pMsg.Name));

	pMsg.GiftIndex = lpMsg->GiftIndex;

	pMsg.Result = 0;

	DWORD Count = 0;

	if (gQueryManager.ExecQuery("SELECT * FROM GiftData WHERE Name='%s' and [Index]='%d'", lpMsg->Name, lpMsg->GiftIndex) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();

		gQueryManager.ExecQuery("INSERT INTO GiftData (Name,[Index],Count) VALUES ('%s',%d,0)", lpMsg->Name, lpMsg->GiftIndex,Count);

		gQueryManager.Close();
	}
	else
	{
		Count = gQueryManager.GetAsInteger("Count");

		gQueryManager.Close();
	}

	if (lpMsg->MaxCount == -1 || Count < lpMsg->MaxCount)
	{
		pMsg.Result = 1;

		gQueryManager.ExecQuery("UPDATE GiftData Set Count=Count+1 WHERE Name='%s' and [Index]='%d'",lpMsg->Name,lpMsg->GiftIndex);

		gQueryManager.Close();
	}

	gSocketManager.DataSend(index, (BYTE*)&pMsg, pMsg.header.size);
}

void CCommandManager::GDCommandRenameRecv(SDHP_COMMAND_RENAME_RECV* lpMsg, int index) // OK
{
	SDHP_COMMAND_RENAME_SEND pMsg;

	pMsg.header.set(0x0F,0x04,sizeof(pMsg));

	pMsg.index = lpMsg->index;

	pMsg.result = 0;

	memcpy(pMsg.account,lpMsg->account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpMsg->name,sizeof(pMsg.name));

	memcpy(pMsg.newname,lpMsg->newname,sizeof(pMsg.newname));

	if(CheckTextSyntax(lpMsg->newname,sizeof(lpMsg->newname)) == 0 || gBadSyntax.CheckSyntax(lpMsg->newname) == 0)
	{
		pMsg.result = 2;
	}

	if(pMsg.result == 0)
	{
		if(gQueryManager.ExecQuery("EXEC WZ_RenameCharacter '%s','%s','%s'",lpMsg->account,lpMsg->name,lpMsg->newname) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
		{
			gQueryManager.Close();
		}
		else
		{
			pMsg.result = gQueryManager.GetResult(0);

			gQueryManager.Close();
		}
	}

	gSocketManager.DataSend(index,(BYTE*)&pMsg,pMsg.header.size);
}

void CCommandManager::GDCommandBanAccountRecv(SDHP_COMMAND_BAN_ACCOUNT_RECV* lpMsg, int index) // OK
{
	SDHP_COMMAND_BAN_ACCOUNT_SEND pMsg;

	pMsg.header.set(0x0F,0x05,sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account,lpMsg->account,sizeof(pMsg.account));

	pMsg.BlocCode = lpMsg->BlocCode;

	if(gQueryManager.ExecQuery("EXEC WZ_BanAccount '%s','%d'",lpMsg->account,lpMsg->BlocCode) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		pMsg.result = 0;

		gQueryManager.Close();
	}
	else
	{
		pMsg.result = gQueryManager.GetResult(0);

		gQueryManager.Close();
	}

	gSocketManager.DataSend(index,(BYTE*)&pMsg,pMsg.header.size);
}

void CCommandManager::GDCommandBanCharacterRecv(SDHP_COMMAND_BAN_CHARACTER_RECV* lpMsg, int index) // OK
{
	SDHP_COMMAND_BAN_CHARACTER_SEND pMsg;

	pMsg.header.set(0x0F,0x06,sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.name,lpMsg->name,sizeof(pMsg.name));

	pMsg.CtlCode = lpMsg->CtlCode;

	if(gQueryManager.ExecQuery("EXEC WZ_BanCharacter '%s','%d'",lpMsg->name,lpMsg->CtlCode) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		pMsg.result = 0;

		gQueryManager.Close();
	}
	else
	{
		pMsg.result = gQueryManager.GetResult(0);

		gQueryManager.Close();
	}

	gSocketManager.DataSend(index,(BYTE*)&pMsg,pMsg.header.size);
}