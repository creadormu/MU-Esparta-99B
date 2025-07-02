// Friend.cpp: implementation of the CFriend class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Friend.h"
#include "CharacterManager.h"
#include "QueryManager.h"
#include "ServerManager.h"
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

void CFriend::GDFriendListRecv(SDHP_FRIEND_LIST_RECV* lpMsg,int index) // OK
{
	BYTE send[2048];

	SDHP_FRIEND_LIST_SEND pMsg;

	pMsg.header.set(0xB0,0x00,0);

	int size = sizeof(pMsg);

	pMsg.aIndex = lpMsg->aIndex;

	memcpy(pMsg.Name,lpMsg->Name,sizeof(pMsg.Name));

	pMsg.count = 0;

	pMsg.MailCount = 0;

	gQueryManager.ExecQuery("WZ_UserGuidCreate '%s'",lpMsg->Name);

	gQueryManager.Fetch();

	gQueryManager.Close();

	gQueryManager.ExecQuery("SELECT GUID,MemoTotal FROM T_FriendMain WHERE Name='%s'",lpMsg->Name);

	gQueryManager.Fetch();

	DWORD guid = gQueryManager.GetAsInteger("GUID");

	pMsg.MailCount = gQueryManager.GetAsInteger("MemoTotal");

	gQueryManager.Close();

	if(gQueryManager.ExecQuery("SELECT FriendName,Del FROM T_FriendList WHERE GUID=%d",guid) != 0)
	{
		SDHP_FRIEND_LIST FriendList;

		while(gQueryManager.Fetch() != SQL_NO_DATA)
		{
			gQueryManager.GetAsString("FriendName",FriendList.Name,sizeof(FriendList.Name));

			FriendList.server = ((gQueryManager.GetAsInteger("Del")==0)?(BYTE)GetServerCodeByName(FriendList.Name):0xFF);

			memcpy(&send[size],&FriendList,sizeof(FriendList));

			size += sizeof(FriendList);

			pMsg.count++;
		}
	}

	gQueryManager.Close();

	pMsg.header.size[0] = SET_NUMBERHB(size);
	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send,&pMsg,sizeof(pMsg));

	gSocketManager.DataSend(index,send,size);

	DGFriendResultSend(index,guid,lpMsg->aIndex,lpMsg->Name);

	DGFriendMailListSend(index,guid,lpMsg->aIndex,lpMsg->Name);
}

void CFriend::GDFriendRequestRecv(SDHP_FRIEND_REQUEST_RECV* lpMsg,int index) // OK
{
	gQueryManager.ExecQuery("WZ_WaitFriendAdd '%s','%s'",lpMsg->Name,lpMsg->Target);

	gQueryManager.Fetch();

	BYTE result = gQueryManager.GetResult(0);

	gQueryManager.Close();

	if(result == 8)
	{
		SDHP_FRIEND_REQUEST_SEND pMsg;

		pMsg.header.set(0xB0,0x03,sizeof(pMsg));

		pMsg.aIndex = lpMsg->aIndex;

		pMsg.result = 0;

		memcpy(pMsg.Name,lpMsg->Name,sizeof(pMsg.Name));

		memcpy(pMsg.Target,lpMsg->Target,sizeof(pMsg.Target));

		pMsg.server = (BYTE)GetServerCodeByName(lpMsg->Target);

		gQueryManager.ExecQuery("WZ_FriendAdd '%s','%s'",lpMsg->Name,lpMsg->Target);

		gQueryManager.Fetch();

		pMsg.result = gQueryManager.GetResult(0);

		gQueryManager.Close();

		gSocketManager.DataSend(index,(BYTE*)&pMsg,sizeof(pMsg));

		if(pMsg.result == 1)
		{
			CHARACTER_INFO CharacterInfo;

			if(gCharacterManager.GetCharacterInfo(&CharacterInfo,lpMsg->Target) != 0)
			{
				SDHP_FRIEND_STATE_SEND pMsg;

				pMsg.header.set(0xB0,0x02,sizeof(pMsg));

				pMsg.aIndex = CharacterInfo.UserIndex;

				memcpy(pMsg.Name,lpMsg->Target,sizeof(pMsg.Name));

				memcpy(pMsg.Target,lpMsg->Name,sizeof(pMsg.Target));

				pMsg.server = (BYTE)GetServerCodeByName(lpMsg->Target);

				CServerManager* lpServerManager = FindServerByCode(CharacterInfo.GameServerCode);

				if(lpServerManager != 0){gSocketManager.DataSend(lpServerManager->m_index,(BYTE*)&pMsg,sizeof(pMsg));}
			}
		}
	}
	else
	{
		SDHP_FRIEND_REQUEST_SEND pMsg;

		pMsg.header.set(0xB0,0x03,sizeof(pMsg));

		pMsg.aIndex = lpMsg->aIndex;

		pMsg.result = result;

		memcpy(pMsg.Name,lpMsg->Name,sizeof(pMsg.Name));

		memcpy(pMsg.Target,lpMsg->Target,sizeof(pMsg.Target));

		pMsg.server = 0xFF;

		gSocketManager.DataSend(index,(BYTE*)&pMsg,sizeof(pMsg));

		if(pMsg.result == 1)
		{
			CHARACTER_INFO CharacterInfo;

			if(gCharacterManager.GetCharacterInfo(&CharacterInfo,lpMsg->Target) != 0)
			{
				SDHP_FRIEND_RESULT_SEND pMsg;

				pMsg.header.set(0xB0,0x01,sizeof(pMsg));

				pMsg.aIndex = CharacterInfo.UserIndex;

				memcpy(pMsg.Name,lpMsg->Target,sizeof(pMsg.Name));

				memcpy(pMsg.Target,lpMsg->Name,sizeof(pMsg.Target));

				CServerManager* lpServerManager = FindServerByCode(CharacterInfo.GameServerCode);

				if(lpServerManager != 0){gSocketManager.DataSend(lpServerManager->m_index,(BYTE*)&pMsg,sizeof(pMsg));}
			}
		}
	}
}

void CFriend::GDFriendResultRecv(SDHP_FRIEND_RESULT_RECV* lpMsg,int index) // OK
{
	SDHP_FRIEND_REQUEST_SEND pMsg;

	pMsg.header.set(0xB0,0x03,sizeof(pMsg));

	pMsg.aIndex = lpMsg->aIndex;

	pMsg.result = lpMsg->result;

	memcpy(pMsg.Name,lpMsg->Name,sizeof(pMsg.Name));

	memcpy(pMsg.Target,lpMsg->Target,sizeof(pMsg.Target));

	pMsg.server = (BYTE)GetServerCodeByName(lpMsg->Target);

	if(lpMsg->result == 0)
	{
		gQueryManager.ExecQuery("WZ_WaitFriendDel '%s','%s'",lpMsg->Name,lpMsg->Target);

		gQueryManager.Fetch();

		pMsg.result = gQueryManager.GetResult(0);

		gQueryManager.Close();

		gSocketManager.DataSend(index,(BYTE*)&pMsg,sizeof(pMsg));
	}
	else
	{
		gQueryManager.ExecQuery("WZ_FriendAdd '%s','%s'",lpMsg->Name,lpMsg->Target);

		gQueryManager.Fetch();

		pMsg.result = gQueryManager.GetResult(0);

		gQueryManager.Close();

		gSocketManager.DataSend(index,(BYTE*)&pMsg,sizeof(pMsg));

		if(pMsg.result == 1)
		{
			CHARACTER_INFO CharacterInfo;

			if(gCharacterManager.GetCharacterInfo(&CharacterInfo,lpMsg->Target) != 0)
			{
				SDHP_FRIEND_STATE_SEND pMsg;

				pMsg.header.set(0xB0,0x02,sizeof(pMsg));

				pMsg.aIndex = CharacterInfo.UserIndex;

				memcpy(pMsg.Name,lpMsg->Target,sizeof(pMsg.Name));

				memcpy(pMsg.Target,lpMsg->Name,sizeof(pMsg.Target));

				pMsg.server = (BYTE)GetServerCodeByName(lpMsg->Target);

				CServerManager* lpServerManager = FindServerByCode(CharacterInfo.GameServerCode);

				if(lpServerManager != 0){gSocketManager.DataSend(lpServerManager->m_index,(BYTE*)&pMsg,sizeof(pMsg));}
			}
		}
	}
}

void CFriend::GDFriendDeleteRecv(SDHP_FRIEND_DELETE_RECV* lpMsg,int index) // OK
{
	SDHP_FRIEND_DELETE_SEND pMsg;

	pMsg.header.set(0xB0,0x04,sizeof(pMsg));

	pMsg.aIndex = lpMsg->aIndex;

	pMsg.result = 0;

	memcpy(pMsg.Name,lpMsg->Name,sizeof(pMsg.Name));

	memcpy(pMsg.Target,lpMsg->Target,sizeof(pMsg.Target));

	gQueryManager.ExecQuery("WZ_FriendDel '%s','%s'",lpMsg->Name,lpMsg->Target);

	gQueryManager.Fetch();

	pMsg.result = gQueryManager.GetResult(0);

	gQueryManager.Close();

	gSocketManager.DataSend(index,(BYTE*)&pMsg,sizeof(pMsg));

	if(pMsg.result == 1)
	{
		CHARACTER_INFO CharacterInfo;

		if(gCharacterManager.GetCharacterInfo(&CharacterInfo,lpMsg->Target) != 0)
		{
			SDHP_FRIEND_STATE_SEND pMsg;

			pMsg.header.set(0xB0,0x02,sizeof(pMsg));

			pMsg.aIndex = CharacterInfo.UserIndex;

			memcpy(pMsg.Name,lpMsg->Target,sizeof(pMsg.Name));

			memcpy(pMsg.Target,lpMsg->Name,sizeof(pMsg.Target));

			pMsg.server = 0xFF;

			CServerManager* lpServerManager = FindServerByCode(CharacterInfo.GameServerCode);

			if(lpServerManager != 0){gSocketManager.DataSend(lpServerManager->m_index,(BYTE*)&pMsg,sizeof(pMsg));}
		}
	}
}

void CFriend::GDFriendMailRecv(SDHP_FRIEND_MAIL_RECV* lpMsg,int index) // OK
{
	SDHP_FRIEND_MAIL_RESULT_SEND pMsg;

	pMsg.header.set(0xB0,0x05,sizeof(pMsg));

	pMsg.aIndex = lpMsg->aIndex;

	memcpy(pMsg.Name,lpMsg->Name,sizeof(pMsg.Name));

	pMsg.result = 1;

	pMsg.window = lpMsg->window;

	gQueryManager.BindParameterAsString(1, lpMsg->Target, sizeof(lpMsg->Target));

	gQueryManager.BindParameterAsString(2, lpMsg->Title, sizeof(lpMsg->Title));

	gQueryManager.ExecQuery("WZ_WriteMail '%s',?,?,%d,%d", lpMsg->Name, lpMsg->direction, lpMsg->action);

	gQueryManager.Fetch();

	DWORD memo = gQueryManager.GetResult(0);

	DWORD guid = gQueryManager.GetResult(1);

	gQueryManager.Close();

	if(memo <= 10)
	{
		pMsg.result = (BYTE)memo;
		gSocketManager.DataSend(index,(BYTE*)&pMsg,sizeof(pMsg));
		return;
	}

	BYTE MemoBuff[1000];

	memset(MemoBuff,0,sizeof(MemoBuff));

	memcpy(MemoBuff,lpMsg->Mail,lpMsg->mailsize);

	gQueryManager.BindParameterAsBinary(1,MemoBuff,sizeof(MemoBuff));

	gQueryManager.BindParameterAsBinary(2,lpMsg->CharSet,sizeof(lpMsg->CharSet));

	gQueryManager.ExecQuery("UPDATE T_FriendMail SET Memo=?,Photo=? WHERE MemoIndex=%d AND GUID=%d",memo,guid);

	gQueryManager.Close();

	gSocketManager.DataSend(index,(BYTE*)&pMsg,sizeof(pMsg));

	if(pMsg.result == 1)
	{
		CHARACTER_INFO CharacterInfo;

		if(gCharacterManager.GetCharacterInfo(&CharacterInfo,lpMsg->Target) != 0)
		{
			gQueryManager.ExecQuery("SELECT MemoIndex,FriendName,wDate,Subject,bRead FROM T_FriendMail WHERE MemoIndex=%d AND GUID=%d",memo,guid);

			gQueryManager.Fetch();

			SDHP_FRIEND_MAIL_LIST_SEND pMsg;

			pMsg.header.set(0xB0,0x06,sizeof(pMsg));

			pMsg.aIndex = CharacterInfo.UserIndex;

			pMsg.index = gQueryManager.GetAsInteger("MemoIndex");

			gQueryManager.GetAsString("FriendName",pMsg.SendName,sizeof(pMsg.SendName));

			memcpy(pMsg.RecvName,lpMsg->Target,sizeof(pMsg.RecvName));

			gQueryManager.GetAsString("wDate",pMsg.Date,sizeof(pMsg.Date));

			gQueryManager.GetAsString("Subject",pMsg.Subject,sizeof(pMsg.Subject));

			pMsg.read = gQueryManager.GetAsInteger("bRead");

			CServerManager* lpServerManager = FindServerByCode(CharacterInfo.GameServerCode);

			if(lpServerManager != 0){gSocketManager.DataSend(lpServerManager->m_index,(BYTE*)&pMsg,sizeof(pMsg));}

			gQueryManager.Close();
		}
	}
}

void CFriend::GDFriendMailListRecv(SDHP_FRIEND_MAIL_LIST_RECV* lpMsg,int index) // OK
{
	gQueryManager.ExecQuery("SELECT GUID FROM T_FriendMain WHERE Name='%s'",lpMsg->Name);

	gQueryManager.Fetch();

	DWORD guid = gQueryManager.GetAsInteger("GUID");

	gQueryManager.Close();

	DGFriendMailListSend(index,guid,lpMsg->aIndex,lpMsg->Name);
}

void CFriend::GDFriendMailReadRecv(SDHP_FRIEND_MAIL_READ_RECV* lpMsg,int index) // OK
{
	gQueryManager.ExecQuery("SELECT GUID FROM T_FriendMain WHERE Name='%s'",lpMsg->Name);

	gQueryManager.Fetch();

	DWORD guid = gQueryManager.GetAsInteger("GUID");

	gQueryManager.Close();

	SDHP_FRIEND_MAIL_READ_SEND pMsg;

	pMsg.header.set(0xB0,0x07,sizeof(pMsg));

	pMsg.aIndex = lpMsg->aIndex;

	memcpy(pMsg.Name,lpMsg->Name,sizeof(pMsg.Name));

	pMsg.index = lpMsg->index;

	gQueryManager.ExecQuery("SELECT Memo,Photo,Dir,Act FROM T_FriendMail WHERE MemoIndex=%d AND GUID=%d",lpMsg->index,guid);

	gQueryManager.Fetch();

	gQueryManager.GetAsBinary("Photo",pMsg.CharSet,sizeof(pMsg.CharSet));

	pMsg.direction = gQueryManager.GetAsInteger("Dir");

	pMsg.action = gQueryManager.GetAsInteger("Act");

	gQueryManager.GetAsBinary("Memo",(BYTE*)pMsg.Mail,sizeof(pMsg.Mail));

	pMsg.mailsize = strlen(pMsg.Mail);

	gQueryManager.Close();

	gQueryManager.ExecQuery("UPDATE T_FriendMail SET bRead=1 WHERE MemoIndex=%d AND GUID=%d",lpMsg->index,guid);

	gQueryManager.Close();

	gSocketManager.DataSend(index,(BYTE*)&pMsg,sizeof(pMsg));
}

void CFriend::GDFriendMailDeleteRecv(SDHP_FRIEND_MAIL_DELETE_RECV* lpMsg,int index) // OK
{
	SDHP_FRIEND_MAIL_DELETE_SEND pMsg;

	pMsg.header.set(0xB0,0x08,sizeof(pMsg));

	pMsg.result = 0;

	pMsg.index = lpMsg->index;

	pMsg.aIndex = lpMsg->aIndex;

	memcpy(pMsg.Name,lpMsg->Name,sizeof(pMsg.Name));

	gQueryManager.ExecQuery("WZ_DelMail '%s',%d",lpMsg->Name,lpMsg->index);

	gQueryManager.Fetch();

	pMsg.result = gQueryManager.GetResult(0);

	gQueryManager.Close();

	gSocketManager.DataSend(index,(BYTE*)&pMsg,sizeof(pMsg));
}

void CFriend::DGFriendResultSend(int index,DWORD userid,WORD aIndex,char* name) // OK
{
	if(gQueryManager.ExecQuery("SELECT FriendName FROM T_WaitFriend WHERE GUID=%d",userid) != 0)
	{
		while(gQueryManager.Fetch() != SQL_NO_DATA)
		{
			SDHP_FRIEND_RESULT_SEND pMsg;

			pMsg.header.set(0xB0,0x01,sizeof(pMsg));

			pMsg.aIndex = aIndex;

			memcpy(pMsg.Name,name,sizeof(pMsg.Name));

			gQueryManager.GetAsString("FriendName",pMsg.Target,sizeof(pMsg.Target));

			gSocketManager.DataSend(index,(BYTE*)&pMsg,sizeof(pMsg));
		}
	}

	gQueryManager.Close();
}

void CFriend::DGFriendStateSend(char* name,int state)
{
	DWORD guid;

	if(gQueryManager.ExecQuery("SELECT GUID FROM T_FriendMain WHERE Name='%s'",name) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();

		return;
	}
	else
	{
		guid = gQueryManager.GetAsInteger("GUID");

		gQueryManager.Close();
	}

	if(gQueryManager.ExecQuery("SELECT FriendName,Del FROM T_FriendList WHERE GUID=%d",guid,0) != 0)
	{
		while(gQueryManager.Fetch() != SQL_NO_DATA)
		{
			SDHP_FRIEND_STATE_SEND pMsg;

			pMsg.header.set(0xB0,0x02,sizeof(pMsg));

			gQueryManager.GetAsString("FriendName",pMsg.Name,sizeof(pMsg.Name));

			CHARACTER_INFO CharacterInfo;

			if(gCharacterManager.GetCharacterInfo(&CharacterInfo,pMsg.Name) != 0)
			{
				pMsg.aIndex = CharacterInfo.UserIndex;

				memcpy(pMsg.Target,name,sizeof(pMsg.Target));

				pMsg.server = ((state==0)?((gQueryManager.GetAsInteger("Del")==0)?GetServerCodeByName(pMsg.Target):0xFF):0xFF);

				CServerManager* lpServerManager = FindServerByCode(CharacterInfo.GameServerCode);

				if(lpServerManager != 0){gSocketManager.DataSend(lpServerManager->m_index,(BYTE*)&pMsg,sizeof(pMsg));}
			}
		}
	}

	gQueryManager.Close();
}

void CFriend::DGFriendMailListSend(int index,DWORD userid,WORD aIndex,char* name)
{
	if(gQueryManager.ExecQuery("SELECT MemoIndex,FriendName,wDate,Subject,bRead FROM T_FriendMail WHERE GUID=%d",userid) != 0)
	{
		while(gQueryManager.Fetch() != SQL_NO_DATA)
		{
			SDHP_FRIEND_MAIL_LIST_SEND pMsg;

			pMsg.header.set(0xB0,0x06,sizeof(pMsg));

			pMsg.aIndex = aIndex;

			pMsg.index = gQueryManager.GetAsInteger("MemoIndex");

			gQueryManager.GetAsString("FriendName",pMsg.SendName,sizeof(pMsg.SendName));

			memcpy(pMsg.RecvName,name,sizeof(pMsg.RecvName));

			gQueryManager.GetAsString("wDate",pMsg.Date,sizeof(pMsg.Date));

			gQueryManager.GetAsString("Subject",pMsg.Subject,sizeof(pMsg.Subject));

			pMsg.read = gQueryManager.GetAsInteger("bRead");

			gSocketManager.DataSend(index,(BYTE*)&pMsg,sizeof(pMsg));
		}
	}

	gQueryManager.Close();
}