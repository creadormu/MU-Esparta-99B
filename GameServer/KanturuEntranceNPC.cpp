// KanturuEntranceNPC.cpp: implementation of the CKanturuEntranceNPC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KanturuEntranceNPC.h"
#include "Kanturu.h"
#include "KanturuBattleUserMng.h"
#include "KanturuUtil.h"
#include "Message.h"
#include "Notice.h"
#include "Util.h"

CKanturuEntranceNPC gKanturuEntranceNPC;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKanturuEntranceNPC::CKanturuEntranceNPC() // OK
{

}

CKanturuEntranceNPC::~CKanturuEntranceNPC() // OK
{

}

void CKanturuEntranceNPC::CGKanturuEnterInfoRecv(int aIndex) // OK
{
	PMSG_KANTURU_ENTER_INFO_SEND pMsg;

	pMsg.header.set(0xD1,0x00,sizeof(pMsg));

	pMsg.state = gKanturu.GetKanturuState();

	pMsg.DetailState = gKanturu.GetKanturuDetailState();

	pMsg.result = gKanturu.CheckCanEnterKanturuBattle();

	pMsg.count = gKanturuBattleUserMng.GetUserCount();

	pMsg.time = gKanturu.GetRemainTime();

	DataSend(aIndex,(BYTE*)&pMsg,sizeof(pMsg));
}

void CKanturuEntranceNPC::CGKanturuEnterRecv(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(gKanturu.GetKanturuState() == KANTURU_STATE_BATTLE_OF_MAYA && gKanturu.CheckEnterKanturu(aIndex) != 0)
	{
		gObjMoveGate(aIndex,133);
		gKanturuBattleUserMng.AddUserData(aIndex);
		gKanturuUtil.NotifyKanturuState(aIndex,gKanturu.GetKanturuState(),gKanturu.GetKanturuDetailState());
		gNotice.GCNoticeSend(aIndex,0,0,0,0,0,0,gMessage.GetMessage(63));
		return;
	}

	if(gKanturu.GetKanturuState() == KANTURU_STATE_TOWER_OF_REFINEMENT && gKanturu.CheckEnterKanturuRefinery(aIndex) != 0)
	{
		gObjMoveGate(aIndex,135);
		gKanturuUtil.NotifyKanturuState(aIndex,gKanturu.GetKanturuState(),gKanturu.GetKanturuDetailState());
		return;
	}
}
