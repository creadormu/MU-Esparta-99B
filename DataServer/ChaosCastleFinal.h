// ChaosCastleFinal.h: interface for the CChaosCastleFinal class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "DataServerProtocol.h"

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_CHAOS_CASTLE_RANKING_RECV
{
	PSBMSG_HEAD header; // C1:1B:00
	WORD index;
};

struct SDHP_CHAOS_CASTLE_RANKING_SAVE_RECV
{
	PSBMSG_HEAD header; // C1:1B:01
	char name[11];
	DWORD Point;
};

struct SDHP_CHAOS_CASTLE_RANKING_CLEAR_RECV
{
	PSBMSG_HEAD header; // C1:1B:02
};

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_CHAOS_CASTLE_RANKING_SEND
{
	PSWMSG_HEAD header; // C1:1B:00
	WORD index;
	BYTE count;
};

struct SDHP_CHAOS_CASTLE_RANKING
{
	BYTE Rank;
	DWORD Point;
	char name[11];
};

//**********************************************//
//**********************************************//
//**********************************************//

class CChaosCastleFinal
{
public:
	CChaosCastleFinal();
	virtual ~CChaosCastleFinal();
	void GDChaosCastleFinalRankingRecv(SDHP_CHAOS_CASTLE_RANKING_RECV* lpMsg, int index);
	void GDChaosCastleFinalRankingSaveRecv(SDHP_CHAOS_CASTLE_RANKING_SAVE_RECV* lpMsg);
	void GDChaosCastleFinalRankingClearRecv(SDHP_CHAOS_CASTLE_RANKING_CLEAR_RECV* lpMsg);
};

extern CChaosCastleFinal gChaosCastleFinal;