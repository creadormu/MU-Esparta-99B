// ArcaBattle.cpp: implementation of the CArcaBattle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ArcaBattle.h"
#include "QueryManager.h"
#include "ServerManager.h"
#include "SocketManager.h"

CArcaBattle gArcaBattle;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CArcaBattle::CArcaBattle() // OK
{

}

CArcaBattle::~CArcaBattle() // OK
{

}

void CArcaBattle::GDArcaBattleMasterRegisterRecv(SDHP_ARCA_BATTLE_MASTER_REGISTER_RECV* lpMsg,int index) // OK
{
	#if(DATASERVER_UPDATE>=702)

	SDHP_ARCA_BATTLE_MASTER_REGISTER_RESULT_SEND pMsg;

	pMsg.header.set(0x1D,0x00,sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account,lpMsg->account,sizeof(pMsg.account));

	if(gQueryManager.ExecQuery("SELECT * FROM ArcaBattleGuildMaster WHERE GuildName='%s'",lpMsg->GuildName) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();

		pMsg.result = 0;

		gSocketManager.DataSend(index,(BYTE*)&pMsg,sizeof(pMsg));
	}
	else
	{
		gQueryManager.Close();

		pMsg.result = 1;

		gSocketManager.DataSend(index,(BYTE*)&pMsg,sizeof(pMsg));
	}

	#endif
}

void CArcaBattle::GDArcaBattleMemberRegisterRecv(SDHP_ARCA_BATTLE_MEMBER_REGISTER_RECV* lpMsg,int index) // OK
{
	#if(DATASERVER_UPDATE>=702)

	SDHP_ARCA_BATTLE_MEMBER_REGISTER_RESULT_SEND pMsg;

	pMsg.header.set(0x1D,0x01,sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account,lpMsg->account,sizeof(pMsg.account));

	if(gQueryManager.ExecQuery("SELECT * FROM ArcaBattleGuildMaster WHERE GuildName='%s'",lpMsg->GuildName) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();

		pMsg.result = 2;

		gSocketManager.DataSend(index,(BYTE*)&pMsg,sizeof(pMsg));
	}
	else
	{
		gQueryManager.Close();

		if(gQueryManager.ExecQuery("SELECT * FROM ArcaBattleGuildMember WHERE Name='%s' AND GuildName='%s'",lpMsg->name,lpMsg->GuildName) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
		{
			gQueryManager.Close();

			gQueryManager.ExecQuery("SELECT COUNT(*) FROM ArcaBattleGuildMember WHERE GuildName='%s'", lpMsg->GuildName);

			int MemberCount = gQueryManager.GetResult(0);

			gQueryManager.Close();

			pMsg.result = (MemberCount>=lpMsg->MaxGuildMember)?3:0;

			gSocketManager.DataSend(index,(BYTE*)&pMsg,sizeof(pMsg));
		}
		else
		{
			gQueryManager.Close();

			pMsg.result = 1;

			gSocketManager.DataSend(index,(BYTE*)&pMsg,sizeof(pMsg));	
		}
	}

	#endif
}

void CArcaBattle::GDArcaBattleEnterRecv(SDHP_ARCA_BATTLE_ENTER_RECV* lpMsg,int index) // OK
{
	#if(DATASERVER_UPDATE>=702)

	SDHP_ARCA_BATTLE_ENTER_SEND pMsg;

	pMsg.header.set(0x1D,0x02,sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account,lpMsg->account,sizeof(pMsg.account));

	if(gQueryManager.ExecQuery("SELECT * FROM ArcaBattleGuildMember WHERE Name='%s' AND GuildName='%s'",lpMsg->name,lpMsg->GuildName) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();

		pMsg.result = 1;

		gSocketManager.DataSend(index,(BYTE*)&pMsg,sizeof(pMsg));
	}
	else
	{
		gQueryManager.Close();

		pMsg.result = 0;

		gSocketManager.DataSend(index,(BYTE*)&pMsg,sizeof(pMsg));
	}

	#endif
}

void CArcaBattle::GDArcaBattleMemberCountRecv(SDHP_ARCA_BATTLE_MEMBER_COUNT_RECV* lpMsg,int index) // OK
{
	#if(DATASERVER_UPDATE>=702)

	SDHP_ARCA_BATTLE_MEMBER_COUNT_SEND pMsg;

	pMsg.header.set(0x1D,0x03,sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account,lpMsg->account,sizeof(pMsg.account));

	if(gQueryManager.ExecQuery("SELECT COUNT(*) FROM ArcaBattleGuildMember WHERE GuildName='%s'",lpMsg->GuildName) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		pMsg.result = 1;

		pMsg.count = 0;

		gQueryManager.Close();
	}
	else
	{
		pMsg.result = 0;

		pMsg.count = (BYTE)gQueryManager.GetResult(0);

		gQueryManager.Close();
	}

	gSocketManager.DataSend(index,(BYTE*)&pMsg,sizeof(pMsg));

	#endif
}

void CArcaBattle::GDArcaBattleRegisterMarkRecv(SDHP_ARCA_BATTLE_REGISTER_MARK_RECV* lpMsg,int index) // OK
{
	#if(DATASERVER_UPDATE>=702)

	SDHP_ARCA_BATTLE_REGISTER_MARK_SEND pMsg;

	pMsg.header.set(0x1D,0x04,sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account,lpMsg->account,sizeof(pMsg.account));

	memcpy(pMsg.GuildName,lpMsg->GuildName,sizeof(pMsg.GuildName));

	pMsg.result = 0;

	pMsg.sign = 0;

	if(gQueryManager.ExecQuery("SELECT * FROM ArcaBattleGuildMaster WHERE GuildName='%s'",lpMsg->GuildName) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();

		pMsg.result = 1;

		gSocketManager.DataSend(index,(BYTE*)&pMsg,sizeof(pMsg));
	}
	else
	{
		gQueryManager.Close();

		if(gQueryManager.ExecQuery("SELECT * FROM ArcaBattleGuildEmblem WHERE GuildName='%s'",lpMsg->GuildName) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
		{
			gQueryManager.Close();

			gSocketManager.DataSend(index,(BYTE*)&pMsg,sizeof(pMsg));
		}
		else
		{
			pMsg.sign = gQueryManager.GetAsInteger("MarkCount");

			gQueryManager.Close();

			gSocketManager.DataSend(index,(BYTE*)&pMsg,sizeof(pMsg));
		}
	}

	#endif
}

void CArcaBattle::GDArcaBattleMarkRankRecv(SDHP_ARCA_BATTLE_MARK_RANK_RECV* lpMsg,int index) // OK
{
	#if(DATASERVER_UPDATE>=702)

	BYTE send[4096];

	SDHP_ARCA_BATTLE_MARK_RANK_SEND pMsg;

	pMsg.header.set(0x1D,0x05,0);

	int size = sizeof(pMsg);

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account,lpMsg->account,sizeof(pMsg.account));

	if(gQueryManager.ExecQuery("SELECT Ranking,MarkCount FROM (SELECT ROW_NUMBER() OVER(ORDER BY MarkCount DESC) AS Ranking, * FROM ArcaBattleGuildEmblem) AS TABLA WHERE GuildName='%s'",lpMsg->GuildName) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		pMsg.rank = 0;

		pMsg.sign = 0;

		gQueryManager.Close();
	}
	else
	{
		pMsg.rank = gQueryManager.GetAsInteger("Ranking");

		pMsg.sign = gQueryManager.GetAsInteger("MarkCount");

		gQueryManager.Close();
	}

	pMsg.count = 0;

	SDHP_ARCA_BATTLE_MARK_RANK info;

	if(gQueryManager.ExecQuery("SELECT TOP 6 ROW_NUMBER() OVER(ORDER BY MarkCount DESC) AS Ranking,* FROM ArcaBattleGuildEmblem") != 0)
	{
		while (gQueryManager.Fetch() != SQL_NO_DATA)
		{
			info.rank = gQueryManager.GetAsInteger("Ranking");
			
			info.sign = gQueryManager.GetAsInteger("MarkCount");
			
			gQueryManager.GetAsString("GuildName",info.GuildName,sizeof(info.GuildName));

			memcpy(&send[size],&info,sizeof(info));
			size += sizeof(info);

			pMsg.count++;
		}

		gQueryManager.Close();
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);
	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send,&pMsg,sizeof(pMsg));

	gSocketManager.DataSend(index,send,size);

	#endif
}

void CArcaBattle::GDArcaBattleMasterRegisterSaveRecv(SDHP_ARCA_BATTLE_MASTER_REGISTER_SAVE_RECV* lpMsg) // OK
{
	#if(DATASERVER_UPDATE>=702)

	if(gQueryManager.ExecQuery("SELECT * FROM ArcaBattleGuildMaster WHERE GuildName='%s'",lpMsg->GuildName) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();

		gQueryManager.ExecQuery("INSERT INTO ArcaBattleGuildMaster (GuildName) VALUES ('%s')",lpMsg->GuildName);

		gQueryManager.Close();
	}

	#endif
}

void CArcaBattle::GDArcaBattleMemberRegisterSaveRecv(SDHP_ARCA_BATTLE_MEMBER_REGISTER_SAVE_RECV* lpMsg) // OK
{
	#if(DATASERVER_UPDATE>=702)

	if(gQueryManager.ExecQuery("SELECT * FROM ArcaBattleGuildMember WHERE Name='%s' AND GuildName='%s'",lpMsg->name,lpMsg->GuildName) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();

		gQueryManager.ExecQuery("INSERT INTO ArcaBattleGuildMember (Name,GuildName) VALUES ('%s','%s')",lpMsg->name,lpMsg->GuildName);

		gQueryManager.Close();
	}

	#endif
}

void CArcaBattle::GDArcaBattleRegisterMarkSaveRecv(SDHP_ARCA_BATTLE_REGISTER_MARK_SAVE_RECV* lpMsg) // OK
{
	#if(DATASERVER_UPDATE>=702)

	if(gQueryManager.ExecQuery("SELECT * FROM ArcaBattleGuildEmblem WHERE GuildName='%s'",lpMsg->GuildName) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();

		gQueryManager.ExecQuery("INSERT INTO ArcaBattleGuildEmblem (GuildName,MarkCount) VALUES ('%s',%d)",lpMsg->GuildName,lpMsg->sign);

		gQueryManager.Close();
	}
	else
	{
		DWORD MarkCount = gQueryManager.GetAsInteger("MarkCount");

		gQueryManager.Close();

		gQueryManager.ExecQuery("UPDATE ArcaBattleGuildEmblem SET MarkCount=%d WHERE GuildName='%s'",(((MarkCount+lpMsg->sign)>0x7FFFFFFF)?0x7FFFFFFF:(MarkCount+lpMsg->sign)),lpMsg->GuildName);

		gQueryManager.Close();
	}

	#endif
}