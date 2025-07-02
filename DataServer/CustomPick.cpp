// CustomPick.cpp: implementation of the CCustomPick class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomPick.h"
#include "QueryManager.h"
#include "SocketManager.h"

CCustomPick gCustomPick;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomPick::CCustomPick() // OK
{

}

CCustomPick::~CCustomPick() // OK
{

}

void CCustomPick::GDCustomPickRecv(SDHP_CUSTOM_PICK_RECV* lpMsg,int index) // OK
{
	SDHP_CUSTOM_PICK_SEND pMsg;

	pMsg.header.set(0x13,0x00,sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account,lpMsg->account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpMsg->name,sizeof(pMsg.name));

	if(gQueryManager.ExecQuery("SELECT * FROM CustomPick WHERE Name='%s'",lpMsg->name) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();

		memset(pMsg.data,0,sizeof(pMsg.data));

		pMsg.Money = 0;

		pMsg.Jewel = 0;

		pMsg.ItemExc = 0;

		pMsg.ItemSet = 0;
	}
	else
	{
		gQueryManager.GetAsBinary("CustomPickList",pMsg.data,sizeof(pMsg.data));

		pMsg.Money = gQueryManager.GetAsInteger("Money");

		pMsg.Jewel = gQueryManager.GetAsInteger("Jewel");

		pMsg.ItemExc = gQueryManager.GetAsInteger("ItemExc");

		pMsg.ItemSet = gQueryManager.GetAsInteger("ItemSet");

		gQueryManager.Close();
	}

	gSocketManager.DataSend(index,(BYTE*)&pMsg,sizeof(pMsg));
}

void CCustomPick::GDCustomPickSaveRecv(SDHP_CUSTOM_PICK_SAVE_RECV* lpMsg) // OK
{
	if(gQueryManager.ExecQuery("SELECT Name FROM CustomPick WHERE Name='%s'",lpMsg->name) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();
		gQueryManager.BindParameterAsBinary(1,lpMsg->data,sizeof(lpMsg->data));
		gQueryManager.ExecQuery("INSERT INTO CustomPick (Name,CustomPickList,Money,Jewel,ItemExc,ItemSet) VALUES ('%s',?,%d,%d,%d,%d)",lpMsg->name,lpMsg->Money,lpMsg->Jewel,lpMsg->ItemExc,lpMsg->ItemSet);
		gQueryManager.Close();
	}
	else
	{
		gQueryManager.Close();
		gQueryManager.BindParameterAsBinary(1,lpMsg->data,sizeof(lpMsg->data));
		gQueryManager.ExecQuery("UPDATE CustomPick SET CustomPickList=?, Money=%d, Jewel=%d, ItemExc=%d, ItemSet=%d  WHERE Name='%s'",lpMsg->Money,lpMsg->Jewel,lpMsg->ItemExc,lpMsg->ItemSet,lpMsg->name);
		gQueryManager.Close();
	}
}
