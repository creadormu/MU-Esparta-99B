// CustomMarriage.h: interface for the CCustomMarriage class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "User.h"
#include "Protocol.h"

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_COMMAND_DIVORCE_SEND
{
	PSBMSG_HEAD header; // C1:0F:02
	int index;
	char name[11];
};

//**********************************************//
//**********************************************//
//**********************************************//

struct MARRIAGE_WEDDING_GIFT_INFO
{
	int ItemIndex;
	int Level;
	int Dur;
	int Option1;
	int Option2;
	int Option3;
	int NewOption;
	int SetOption;
	int SocketCount;
	int Duration;
};

class CCustomMarriage
{
public:
	void ReadCustomMarriageInfo(char* section,char* path);
	void Load(char* path);
	bool CheckSex(LPOBJ lpObj);
	bool CheckPosition(LPOBJ lpObj,LPOBJ lpTarget);
	bool CheckWeddingRequerimentItem(LPOBJ lpObj);
	bool CheckDivorceRequerimentItem(LPOBJ lpObj);
	void GiveWeddingGift(LPOBJ lpObj);
	void CommandPropuse(LPOBJ lpObj,char* arg);
	void CommandAccept(LPOBJ lpObj);
	void CommandDivorce(LPOBJ lpObj);
	void CommandTrackMarry(LPOBJ lpObj);
private:
	int m_CustomMarriageSwitch;
	int m_CustomMarriageHomoSexual;
	int m_CustomMarriageAnnounce;
	int m_CustomMarriageMapZone;
	int m_CustomMarriageStartX;
	int m_CustomMarriageStartY;
	int m_CustomMarriageEndX;
	int m_CustomMarriageEndY;
	int m_CustomMarriageTracePK;
	int m_WeddingEnable;
	int m_WeddingItemIndex;
	int m_WeddingItemLevel;
	int m_WeddingItemCount;
	int m_DivorseEnable;
	int m_DivorseItemIndex;
	int m_DivorseItemLevel;
	int m_DivorseItemCount;
	std::vector<MARRIAGE_WEDDING_GIFT_INFO> m_MarriageWeedingGift;
};

extern CCustomMarriage gCustomMarriage;