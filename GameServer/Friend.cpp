// Friend.cpp: implementation of the CFriend class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Friend.h"
#include "GameMain.h"
#include "ServerInfo.h"
#include "Util.h"

CFriend gFriend;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFriend::CFriend() // OK
{

}

CFriend::~CFriend() // OK
{

}

void CFriend::DGFriendListRecv(SDHP_FRIEND_LIST_RECV* lpMsg) // OK
{
	if(gObjIsNameValid(lpMsg->aIndex,lpMsg->Name) == 0)
	{
		return;
	}

	BYTE send[4096];

	PMSG_FRIEND_LIST_SEND pMsg;

	pMsg.header.set(0xC0,0);

	int size = sizeof(pMsg);

	pMsg.MailCount = lpMsg->MailCount;

	pMsg.MailTotal = 50;

	pMsg.count = 0;

	PMSG_FRIEND_LIST info;

	for(int n=0;n < lpMsg->count;n++)
	{
		SDHP_FRIEND_LIST* lpInfo = (SDHP_FRIEND_LIST*)(((BYTE*)lpMsg)+sizeof(SDHP_FRIEND_LIST_RECV)+(sizeof(SDHP_FRIEND_LIST)*n));

		memcpy(info.Name,lpInfo->Name,sizeof(info.Name));

		info.server = lpInfo->server;

		memcpy(&send[size],&info,sizeof(info));
		size += sizeof(info);

		pMsg.count++;
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);

	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send,&pMsg,sizeof(pMsg));

	DataSend(lpMsg->aIndex,send,size);
}

void CFriend::DGFriendResultRecv(SDHP_FRIEND_RESULT_RECV* lpMsg) // OK
{
	if(gObjIsNameValid(lpMsg->aIndex,lpMsg->Name) == 0)
	{
		return;
	}

	PMSG_FRIEND_RESULT_SEND pMsg;

	pMsg.header.set(0xC2,sizeof(pMsg));

	memcpy(pMsg.Name,lpMsg->Target,sizeof(pMsg.Name));

	DataSend(lpMsg->aIndex,(BYTE*)&pMsg,sizeof(pMsg));
}

void CFriend::DGFriendRequestRecv(SDHP_FRIEND_REQUEST_RECV* lpMsg) // OK
{
	if(gObjIsNameValid(lpMsg->aIndex,lpMsg->Name) == 0)
	{
		return;
	}

	this->GCFriendRequestSend(lpMsg->aIndex,lpMsg->server,lpMsg->Target,lpMsg->result);
}

void CFriend::DGFriendDeleteRecv(SDHP_FRIEND_DELETE_RECV* lpMsg) // OK
{
	if(gObjIsNameValid(lpMsg->aIndex,lpMsg->Name) == 0)
	{
		return;
	}

	PMSG_FRIEND_DELETE_SEND pMsg;

	pMsg.header.set(0xC3,sizeof(pMsg));

	pMsg.result = lpMsg->result;

	memcpy(pMsg.Name,lpMsg->Target,sizeof(pMsg.Name));

	DataSend(lpMsg->aIndex,(BYTE*)&pMsg,sizeof(pMsg));
}

void CFriend::DGFriendMailResultRecv(SDHP_FRIEND_MAIL_RESULT_RECV* lpMsg) // OK
{
	if(gObjIsNameValid(lpMsg->aIndex,lpMsg->Name) == 0)
	{
		return;
	}

	this->GCFriendMailResultSend(lpMsg->aIndex,lpMsg->window,lpMsg->result);

	if(lpMsg->result == 1)
	{
		LPOBJ lpObj = &gObj[lpMsg->aIndex];

		if(lpObj->Money >= 1000)
		{
			lpObj->Money -= 1000;
			GCMoneySend(lpObj->Index,lpObj->Money);
		}
	}
}

void CFriend::DGFriendMailReadRecv(SDHP_FRIEND_MAIL_READ_RECV* lpMsg) // OK
{
	if(gObjIsNameValid(lpMsg->aIndex,lpMsg->Name) == 0)
	{
		return;
	}

	PMSG_FRIEND_MAIL_READ_SEND pMsg;

	int size = sizeof(pMsg)-sizeof(pMsg.Mail);

	if(lpMsg->mailsize > sizeof(pMsg.Mail))
	{
		return;
	}

	size += lpMsg->mailsize;

	pMsg.header.setE(0xC7,size);

	pMsg.index = lpMsg->index;

	pMsg.mailsize = lpMsg->mailsize;

	memcpy(pMsg.CharSet,lpMsg->CharSet,sizeof(pMsg.CharSet));

	pMsg.action = lpMsg->action;

	pMsg.direction = lpMsg->direction;

	memcpy(pMsg.Mail,lpMsg->Mail,lpMsg->mailsize);

	DataSend(lpMsg->aIndex,(BYTE*)&pMsg,size);
}

void CFriend::DGFriendMailDeleteRecv(SDHP_FRIEND_MAIL_DELETE_RECV* lpMsg) // OK
{
	if(gObjIsNameValid(lpMsg->aIndex,lpMsg->Name) == 0)
	{
		return;
	}

	PMSG_FRIEND_MAIL_DELETE_SEND pMsg;

	pMsg.header.set(0xC8,sizeof(pMsg));

	pMsg.result = lpMsg->result;

	pMsg.index = lpMsg->index;

	DataSend(lpMsg->aIndex,(BYTE*)&pMsg,sizeof(pMsg));
}

void CFriend::DGFriendMailListRecv(SDHP_FRIEND_MAIL_LIST_RECV* lpMsg) // OK
{
	if(gObjIsNameValid(lpMsg->aIndex,lpMsg->RecvName) == 0)
	{
		return;
	}

	PMSG_FRIEND_MAIL_SEND pMsg;

	pMsg.header.setE(0xC6,sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.Name,lpMsg->SendName,sizeof(pMsg.Name));

	memcpy(pMsg.Date,lpMsg->Date,sizeof(pMsg.Date));

	memcpy(pMsg.Title,lpMsg->Subject,sizeof(pMsg.Title));

	pMsg.read = lpMsg->read;

	DataSend(lpMsg->aIndex,(BYTE*)&pMsg,sizeof(pMsg));
}

void CFriend::DGFriendStateRecv(SDHP_FRIEND_STATE_RECV* lpMsg) // OK
{
	if(gObjIsNameValid(lpMsg->aIndex,lpMsg->Name) == 0)
	{
		return;
	}

	PMSG_FRIEND_STATE_SEND pMsg;

	pMsg.header.set(0xC4,sizeof(pMsg));

	memcpy(pMsg.Name,lpMsg->Target,sizeof(pMsg.Name));

	pMsg.server = lpMsg->server;

	DataSend(lpMsg->aIndex,(BYTE*)&pMsg,sizeof(pMsg));
}

void CFriend::GDFriendMailSend(char* Name,char* Target,char* Title,BYTE direction,BYTE action,BYTE* CharSet,char* Text) // OK
{
	int size = strlen(Text);

	SDHP_FRIEND_MAIL_SEND pMsg;

	pMsg.header.set(0xB0,0x04,(sizeof(pMsg)-(sizeof(pMsg.Mail)-size)));

	pMsg.aIndex = -1;

	pMsg.window = 0;

	memcpy(pMsg.Name,Name,sizeof(pMsg.Name));

	memcpy(pMsg.Target,Target,sizeof(pMsg.Target));

	memcpy(pMsg.Title,Title,sizeof(pMsg.Title));

	pMsg.direction = direction;

	pMsg.action = action;

	pMsg.mailsize = size;

	memcpy(pMsg.CharSet,CharSet,sizeof(pMsg.CharSet));

	memcpy(pMsg.Mail,Text,size);

	gDataServerConnection.DataSend((BYTE*)&pMsg,(sizeof(pMsg)-(sizeof(pMsg.Mail)-size)),-1);
}

void CFriend::CGFriendListRecv(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	SDHP_FRIEND_LIST_SEND pMsg;

	pMsg.header.set(0xB0,0x00,sizeof(pMsg));

	memcpy(pMsg.Name,lpObj->Name,sizeof(pMsg.Name));

	pMsg.aIndex = aIndex;

	pMsg.server = (BYTE)gServerInfo.m_ServerCode;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);
}

void CFriend::CGFriendRequestRecv(PMSG_FRIEND_REQUEST_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	char Name[11] = {0};

	memcpy(Name,lpMsg->Name,sizeof(lpMsg->Name));

	if(strcmp(lpObj->Name,Name) == 0)
	{
		this->GCFriendRequestSend(aIndex,-1,Name,5);
		return;
	}
	
	SDHP_FRIEND_REQUEST_SEND pMsg;

	pMsg.header.set(0xB0,0x01,sizeof(pMsg));

	pMsg.aIndex = aIndex;

	memcpy(pMsg.Name,lpObj->Name,sizeof(pMsg.Name));

	memcpy(pMsg.Target,Name,sizeof(pMsg.Target));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);
}

void CFriend::CGFriendResultRecv(PMSG_FRIEND_RESULT_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	char Name[11] = {0};

	memcpy(Name,lpMsg->Name,sizeof(lpMsg->Name));

	SDHP_FRIEND_RESULT_SEND pMsg;

	pMsg.header.set(0xB0,0x02,sizeof(pMsg));

	pMsg.result = lpMsg->result;

	pMsg.aIndex = aIndex;

	memcpy(pMsg.Name,lpObj->Name,sizeof(pMsg.Name));

	memcpy(pMsg.Target,Name,sizeof(pMsg.Target));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);
}

void CFriend::CGFriendDeleteRecv(PMSG_FRIEND_DELETE_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	char Name[11] = {0};

	memcpy(Name,lpMsg->Name,sizeof(lpMsg->Name));

	SDHP_FRIEND_DELETE_SEND pMsg;

	pMsg.header.set(0xB0,0x03,sizeof(pMsg));

	pMsg.aIndex = aIndex;

	memcpy(pMsg.Name,lpObj->Name,sizeof(pMsg.Name));

	memcpy(pMsg.Target,Name,sizeof(pMsg.Target));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);
}

void CFriend::CGFriendMailRecv(PMSG_FRIEND_MAIL_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	char Name[11] = {0};

	memcpy(Name,lpMsg->Name,sizeof(lpMsg->Name));

	if(lpObj->Money < 1000)
	{
		this->GCFriendMailResultSend(aIndex,lpMsg->window,7);
		return;
	}

	if(lpMsg->mailsize < 0 || lpMsg->mailsize > 1000 || strlen(Name) < 1)
	{
		this->GCFriendMailResultSend(aIndex,lpMsg->window,0);
		return;
	}

	SDHP_FRIEND_MAIL_SEND pMsg;

	int size = lpMsg->mailsize+sizeof(pMsg) - sizeof(pMsg.Mail);

	pMsg.header.set(0xB0,0x04,size);

	pMsg.aIndex = aIndex;

	pMsg.window = lpMsg->window;

	memcpy(pMsg.Name,lpObj->Name,sizeof(pMsg.Name));

	memcpy(pMsg.Target,Name,sizeof(pMsg.Target));

	memcpy(pMsg.Title,lpMsg->Title,sizeof(pMsg.Title));
	
	pMsg.direction = lpMsg->direction;

	pMsg.action = lpMsg->action;

	pMsg.mailsize = lpMsg->mailsize;
	
	memcpy(pMsg.CharSet,lpObj->CharSet,sizeof(pMsg.CharSet));

	memcpy(pMsg.Mail,lpMsg->Mail,lpMsg->mailsize);

	gDataServerConnection.DataSend((BYTE*)&pMsg,size,aIndex);
}

void CFriend::CGFriendMailReadRecv(PMSG_FRIEND_MAIL_READ_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	SDHP_FRIEND_MAIL_READ_SEND pMsg;

	pMsg.header.set(0xB0,0x06,sizeof(pMsg));

	pMsg.aIndex = aIndex;

	pMsg.index = lpMsg->index;

	memcpy(pMsg.Name,lpObj->Name,sizeof(pMsg.Name));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);
}

void CFriend::CGFriendMailDeleteRecv(PMSG_FRIEND_MAIL_DELETE_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	SDHP_FRIEND_MAIL_DELETE_SEND pMsg;

	pMsg.header.set(0xB0,0x07,sizeof(pMsg));

	pMsg.aIndex = aIndex;

	pMsg.index = lpMsg->index;

	memcpy(pMsg.Name,lpObj->Name,sizeof(pMsg.Name));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);
}

void CFriend::CGFriendMailListRecv(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	SDHP_FRIEND_MAIL_LIST_SEND pMsg;

	pMsg.header.set(0xB0,0x05,sizeof(pMsg));

	pMsg.aIndex = aIndex;

	memcpy(pMsg.Name,lpObj->Name,sizeof(pMsg.Name));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);
}

void CFriend::GCFriendRequestSend(int aIndex,int server,char* Name,int result) // OK
{
	PMSG_FRIEND_REQUEST_SEND pMsg;

	pMsg.header.set(0xC1,sizeof(pMsg));

	pMsg.result = result;

	memcpy(pMsg.Name,Name,sizeof(pMsg.Name));

	pMsg.server = server;

	DataSend(aIndex,(BYTE*)&pMsg,sizeof(pMsg));
}

void CFriend::GCFriendMailResultSend(int aIndex,int window,int result) // OK
{
	PMSG_FRIEND_MAIL_RESULT_SEND pMsg;

	pMsg.header.set(0xC5,sizeof(pMsg));

	pMsg.result = result;

	pMsg.window = window;

	DataSend(aIndex,(BYTE*)&pMsg,sizeof(pMsg));
}