// ChaosCastleFinal.cpp: implementation of the CChaosCastleFinal class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChaosCastleFinal.h"
#include "QueryManager.h"
#include "ServerManager.h"
#include "SocketManager.h"

CChaosCastleFinal gChaosCastleFinal;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChaosCastleFinal::CChaosCastleFinal() // OK
{

}

CChaosCastleFinal::~CChaosCastleFinal() // OK
{

}

void CChaosCastleFinal::GDChaosCastleFinalRankingRecv(SDHP_CHAOS_CASTLE_RANKING_RECV* lpMsg, int index) // OK
{
	#if(DATASERVER_UPDATE>=803)

	BYTE send[4096];

	SDHP_CHAOS_CASTLE_RANKING_SEND pMsg;

	pMsg.header.set(0x1B,0x00,0);

	int size = sizeof(pMsg);

	pMsg.index = lpMsg->index;

	pMsg.count = 0;

	SDHP_CHAOS_CASTLE_RANKING info;

	if(gQueryManager.ExecQuery("SELECT TOP 50 ROW_NUMBER() OVER(ORDER BY Score DESC) AS Ranking,* FROM RankingChaosCastleFinal") != 0)
	{
		while (gQueryManager.Fetch() != SQL_NO_DATA)
		{
			info.Rank = gQueryManager.GetAsInteger("Ranking");

			gQueryManager.GetAsString("Name", info.name, sizeof(info.name));

			info.Point = gQueryManager.GetAsInteger("Score");

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

void CChaosCastleFinal::GDChaosCastleFinalRankingSaveRecv(SDHP_CHAOS_CASTLE_RANKING_SAVE_RECV* lpMsg) // OK
{
	#if(DATASERVER_UPDATE>=803)

	if(gQueryManager.ExecQuery("SELECT Name FROM RankingChaosCastleFinal WHERE Name='%s'",lpMsg->name) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();
		gQueryManager.ExecQuery("INSERT INTO RankingChaosCastleFinal (Name,Score) VALUES ('%s',%d)",lpMsg->name,lpMsg->Point);
		gQueryManager.Close();
	}
	else
	{
		gQueryManager.Close();
		gQueryManager.ExecQuery("UPDATE RankingChaosCastleFinal SET Score=Score+%d WHERE Name='%s'",lpMsg->Point,lpMsg->name);
		gQueryManager.Close();
	}

	#endif
}

void CChaosCastleFinal::GDChaosCastleFinalRankingClearRecv(SDHP_CHAOS_CASTLE_RANKING_CLEAR_RECV* lpMsg) // OK
{
	#if(DATASERVER_UPDATE>=803)

	gQueryManager.ExecQuery("DELETE FROM RankingChaosCastleFinal");
	gQueryManager.Close();

	#endif
}