// CommandManager.cpp: implementation of the CCommandManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommandManager.h"
#include "BloodCastle.h"
#include "BonusManager.h"
#include "ChaosCastle.h"
#include "CustomAttack.h"
#include "CustomBuyVip.h"
#include "CustomGift.h"
#include "CustomJewelPack.h"
#include "CustomMarriage.h"
#include "CustomPick.h"
#include "CustomStore.h"
#include "DevilSquare.h"
#include "DSProtocol.h"
#include "GameMain.h"
#include "GameMaster.h"
#include "Guild.h"
#include "GuildManager.h"
#include "HelperOffline.h"
#include "IllusionTemple.h"
#include "ItemManager.h"
#include "ItemOptionRate.h"
#include "JSProtocol.h"
#include "Log.h"
#include "MapServerManager.h"
#include "MasterSkillTree.h"
#include "MemScript.h"
#include "Message.h"
#include "Monster.h"
#include "Move.h"
#include "Notice.h"
#include "ObjectManager.h"
#include "Protocol.h"
#include "Quest.h"
#include "QuestReward.h"
#include "ResetLimit.h"
#include "ResetTable.h"
#include "ScriptLoader.h"
#include "ServerInfo.h"
#include "SetItemType.h"
#include "Util.h"

CCommandManager gCommandManager;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCommandManager::CCommandManager() // OK
{
	this->m_CommandListInfo.clear();
}

CCommandManager::~CCommandManager() // OK
{

}

void CCommandManager::Load(char* path) // OK
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

	this->m_CommandListInfo.clear();

	try
	{
		while (true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			COMMAND_LIST_INFO info;

			memset(&info,0,sizeof(info));

			info.Index = lpMemScript->GetNumber();

			info.Switch[0] = lpMemScript->GetAsNumber();

			info.Switch[1] = lpMemScript->GetAsNumber();

			info.Switch[2] = lpMemScript->GetAsNumber();

			info.Switch[3] = lpMemScript->GetAsNumber();

			strcpy_s(info.Command,lpMemScript->GetAsString());

			info.Delay = lpMemScript->GetAsNumber();

			info.GMLevel = lpMemScript->GetAsNumber();

			info.MinLevel = lpMemScript->GetAsNumber();

			info.MaxLevel = lpMemScript->GetAsNumber();

			info.MinReset = lpMemScript->GetAsNumber();

			info.MaxReset = lpMemScript->GetAsNumber();

			info.Money[0] = lpMemScript->GetAsNumber();

			info.Money[1] = lpMemScript->GetAsNumber();

			info.Money[2] = lpMemScript->GetAsNumber();

			info.Money[3] = lpMemScript->GetAsNumber();

			#if(GAMESERVER_SHOP>=1)

			info.Coin1[0] = lpMemScript->GetAsNumber();

			info.Coin1[1] = lpMemScript->GetAsNumber();

			info.Coin1[2] = lpMemScript->GetAsNumber();

			info.Coin1[3] = lpMemScript->GetAsNumber();

			#endif

			#if(GAMESERVER_SHOP==3)

			info.Coin2[0] = lpMemScript->GetAsNumber();

			info.Coin2[1] = lpMemScript->GetAsNumber();

			info.Coin2[2] = lpMemScript->GetAsNumber();

			info.Coin2[3] = lpMemScript->GetAsNumber();

			info.Coin3[0] = lpMemScript->GetAsNumber();

			info.Coin3[1] = lpMemScript->GetAsNumber();

			info.Coin3[2] = lpMemScript->GetAsNumber();

			info.Coin3[3] = lpMemScript->GetAsNumber();

			#endif

			this->m_CommandListInfo.insert(std::pair<int,COMMAND_LIST_INFO>(info.Index,info));
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

void CCommandManager::MainProc() // OK
{
	for(int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
	{
		if(gObjIsConnectedGP(n) == 0)
		{
			continue;
		}

		if(gObj[n].AutoAddPointCount > 0)
		{
			this->CommandAddPointAutoProc(&gObj[n]);
		}

		if(gObj[n].AutoResetEnable != 0)
		{
			this->CommandResetAutoProc(&gObj[n]);
		}
	}
}

long CCommandManager::GetNumber(char* arg,int pos) // OK
{
	int count = 0,p = 0;

	char buffer[60] = {0};

	int len = strlen(arg);

	len = ((len >= sizeof(buffer)) ? (sizeof(buffer) - 1) : len);

	for(int n = 0; n < len; n++)
	{
		if(arg[n] == 0x20)
		{
			count++;
		}
		else if(count == pos)
		{
			buffer[p] = arg[n];
			p++;
		}
	}

	return atoi(buffer);
}

void CCommandManager::GetString(char* arg,char* out,int size,int pos) // OK
{
	int count = 0,p = 0;

	char buffer[60] = {0};

	int len = strlen(arg);

	len = ((len >= sizeof(buffer)) ? (sizeof(buffer) - 1) : len);

	for(int n = 0; n < len; n++)
	{
		if(arg[n] == 0x20)
		{
			count++;
		}
		else if(count == pos)
		{
			buffer[p] = arg[n];
			p++;
		}
	}

	memcpy(out,buffer,(size - 1));
}

bool CCommandManager::GetCommandCode(int Index,COMMAND_LIST_INFO* lpInfo) // OK
{
	std::map<int,COMMAND_LIST_INFO>::iterator it = this->m_CommandListInfo.find(Index);

	if(it != this->m_CommandListInfo.end())
	{
		(*lpInfo) = it->second;
		return 1;
	}

	return 0;
}

bool CCommandManager::GetCommandCode(char* label,COMMAND_LIST_INFO* lpInfo) // OK
{
	for(std::map<int,COMMAND_LIST_INFO>::iterator it = this->m_CommandListInfo.begin(); it != this->m_CommandListInfo.end(); it++)
	{
		if(strcmp(label,it->second.Command) == 0)
		{
			(*lpInfo) = it->second;
			return 1;
		}
	}

	return 0;
}

void CCommandManager::ManagementCore(LPOBJ lpObj,char* message) // OK
{
	char command[32] = {0};

	memset(command,0,sizeof(command));

	this->GetString(message,command,sizeof(command),0);

	char* argument = &message[strlen(command)];

	if(argument[0] == 0x20)
	{
		argument++;
	}

	COMMAND_LIST_INFO lpInfo;

	if(this->GetCommandCode(command,&lpInfo) == 0)
	{
		return;
	}

	if(lpInfo.Switch[lpObj->AccountLevel] == 0)
	{
		return;
	}

	if(lpObj->TradeMoney != 0)
	{
		return;
	}

	DWORD TickCount = (GetTickCount()-lpObj->CommandCheckTime[lpInfo.Index])/1000;

	if(TickCount < lpInfo.Delay)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(300),(lpInfo.Delay-TickCount),lpInfo.Command);
		return;
	}

	if(lpInfo.GMLevel != -1 && lpInfo.GMLevel > gGameMaster.GetGameMasterLevel(lpObj))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(301),lpInfo.Command);
		return;
	}

	if(lpInfo.MinLevel != -1 && lpInfo.MinLevel > lpObj->Level)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(302),lpInfo.MinLevel,lpInfo.Command);
		return;
	}

	if(lpInfo.MaxLevel != -1 && lpInfo.MaxLevel < lpObj->Level)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(303),lpInfo.MaxLevel,lpInfo.Command);
		return;
	}

	if(lpInfo.MinReset != -1 && lpInfo.MinReset > lpObj->Reset)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(304),lpInfo.MinReset,lpInfo.Command);
		return;
	}

	if(lpInfo.MaxReset != -1 && lpInfo.MaxReset < lpObj->Reset)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(305),lpInfo.MaxReset,lpInfo.Command);
		return;
	}

	if(lpInfo.Money[lpObj->AccountLevel] > lpObj->Money)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(306),lpInfo.Money[lpObj->AccountLevel],lpInfo.Command);
		return;
	}

	if(lpObj->Coin1 < lpInfo.Coin1[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(307),lpInfo.Coin1[lpObj->AccountLevel],lpInfo.Command);
		return;
	}

	if(lpObj->Coin2 < lpInfo.Coin2[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(308),lpInfo.Coin2[lpObj->AccountLevel],lpInfo.Command);
		return;
	}

	if(lpObj->Coin3 < lpInfo.Coin3[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(309),lpInfo.Coin3[lpObj->AccountLevel],lpInfo.Command);
		return;
	}

	if(gScriptLoader.OnCommandManager(lpObj->Index,lpInfo.Index,argument) != 0)
	{
		this->DiscountRequirement(lpObj,lpInfo.Index);
		return;
	}

	switch(lpInfo.Index)
	{
		case COMMAND_MOVE:
			this->CommandMove(lpObj,argument);
			break;
		case COMMAND_GUILD_WAR:
			this->CommandGuildWar(lpObj,argument);
			break;
		case COMMAND_BATTLE_SOCCER:
			this->CommandBattleSoccer(lpObj,argument);
			break;
		case COMMAND_REQUEST:
			this->CommandRequest(lpObj,argument);
			break;
		case COMMAND_POST:
			this->CommandPost(lpObj,argument);
			break;
		case COMMAND_ADD_POINT1:
			this->CommandAddPoint(lpObj,argument,0);
			break;
		case COMMAND_ADD_POINT2:
			this->CommandAddPoint(lpObj,argument,1);
			break;
		case COMMAND_ADD_POINT3:
			this->CommandAddPoint(lpObj,argument,2);
			break;
		case COMMAND_ADD_POINT4:
			this->CommandAddPoint(lpObj,argument,3);
			break;
		case COMMAND_ADD_POINT5:
			this->CommandAddPoint(lpObj,argument,4);
			break;
		case COMMAND_PK_CLEAR:
			this->CommandPKClear(lpObj,lpInfo.Command);
			break;
		case COMMAND_MONEY:
			this->CommandMoney(lpObj,argument);
			break;
		case COMMAND_CHANGE:
			this->CommandChange(lpObj);
			break;
		case COMMAND_WARE:
			this->CommandWare(lpObj,argument);
			break;
		case COMMAND_RESET:
			this->CommandReset(lpObj,argument);
			break;
		case COMMAND_MASTER_RESET:
			this->CommandMasterReset(lpObj,argument);
			break;
		case COMMAND_CLEAR_INVENTORY:
			this->CommandClearInventory(lpObj);
			break;
		case COMMAND_REDISTRIBUTE:
			this->CommandRedistribute(lpObj);
			break;
		case COMMAND_TREE_RESET:
			this->CommandTreeReset(lpObj,argument);
			break;
		case COMMAND_RENAME:
			this->CommandRename(lpObj,argument);
			break;
		case COMMAND_DCFRIEND:
			this->CommandDCFriend(lpObj,argument);
			break;
		case COMMAND_OFFHELPER:
			gHelperOffline.CommandHelperOffline(lpObj);
			break;
		case COMMAND_CUSTOM_STORE:
			gCustomStore.CommandCustomStore(lpObj,argument);
			break;
		case COMMAND_CUSTOM_STORE_OFFLINE:
			gCustomStore.CommandCustomStoreOffline(lpObj);
			break;
		case COMMAND_CUSTOM_ATTACK:
			gCustomAttack.CommandCustomAttack(lpObj,argument);
			break;
		case COMMAND_CUSTOM_ATTACK_OFFLINE:
			gCustomAttack.CommandCustomAttackOffline(lpObj);
			break;
		case COMMAND_CUSTOM_GIFT:
			gCustomGift.CommandGift(lpObj,argument);
			break;
		case COMMAND_CUSTOM_MARRIAGE1:
			gCustomMarriage.CommandPropuse(lpObj,argument);
			break;
		case COMMAND_CUSTOM_MARRIAGE2:
			gCustomMarriage.CommandAccept(lpObj);
			break;
		case COMMAND_CUSTOM_MARRIAGE3:
			gCustomMarriage.CommandDivorce(lpObj);
			break;
		case COMMAND_CUSTOM_MARRIAGE4:
			gCustomMarriage.CommandTrackMarry(lpObj);
			break;
		case COMMAND_CUSTOM_BUY_VIP:
			gCustomBuyVip.CommandBuyVip(lpObj,argument);
			break;
		case COMMAND_CUSTOM_JEWEL_PACK:
			gCustomJewelPack.CommandCustomJewelPack(lpObj,argument);
			break;
		case COMMAND_CUSTOM_JEWEL_UNPACK:
			gCustomJewelPack.CommandCustomJewelUnpack(lpObj,argument);
			break;
		case COMMAND_CUSTOM_PICK:
			gCustomPick.CommandCustomPick(lpObj,argument);
			break;
		case COMMAND_CUSTOM_PICK_ADD:
			gCustomPick.CommandCustomAdd(lpObj,argument);
			break;
		case COMMAND_CUSTOM_PICK_CLEAR:
			gCustomPick.CommandCustomClear(lpObj);
			break;
		case COMMAND_GM_MOVE:
			this->CommandGMMove(lpObj,argument);
			break;
		case COMMAND_GM_POST:
			this->CommandGMPost(lpObj,argument);
			break;
		case COMMAND_TRACK:
			this->CommandTrack(lpObj,argument);
			break;
		case COMMAND_TRACE:
			this->CommandTrace(lpObj,argument);
			break;
		case COMMAND_DISCONNECT:
			this->CommandDisconnect(lpObj,argument);
			break;
		case COMMAND_FIREWORKS:
			this->CommandFireworks(lpObj);
			break;
		case COMMAND_MAKE:
			this->CommandMake(lpObj,argument);
			break;
		case COMMAND_DROP:
			this->CommandDrop(lpObj,argument);
			break;
		case COMMAND_SKIN:
			this->CommandSkin(lpObj,argument);
			break;
		case COMMAND_SET_MONEY:
			this->CommandSetMoney(lpObj,argument);
			break;
		case COMMAND_HIDE:
			this->CommandHide(lpObj);
			break;
		case COMMAND_EDIT:
			this->CommandEdit(lpObj,argument);
			break;
		case COMMAND_SET_COIN:
			this->CommandSetCoin(lpObj,argument);
			break;
		case COMMAND_SET_PK:
			this->CommandSetPK(lpObj,argument);
			break;
		case COMMAND_SET_VIP:
			this->CommandSetVip(lpObj,argument);
			break;
		case COMMAND_MOVE_ALL:
			this->CommandMoveAll(lpObj,argument);
			break;
		case COMMAND_MOVE_GUILD:
			this->CommandMoveGuild(lpObj,argument);
			break;
		case COMMAND_MAKE_MOB:
			this->CommandMakeMob(lpObj,argument);
			break;
		case COMMAND_MAKE_SET:
			this->CommandMakeSet(lpObj,argument);
			break;
		case COMMAND_BAN_ACC:
			this->CommandBanAcc(lpObj,argument);
			break;
		case COMMAND_BAN_CHAR:
			this->CommandBanChar(lpObj,argument);
			break;
		case COMMAND_BAN_CHAT:
			this->CommandBanChat(lpObj,argument);
			break;
		case COMMAND_UNBAN_ACC:
			this->CommandUnbanAcc(lpObj,argument);
			break;
		case COMMAND_UNBAN_CHAR:
			this->CommandUnbanChar(lpObj,argument);
			break;
		case COMMAND_UNBAN_CHAT:
			this->CommandUnbanChat(lpObj,argument);
			break;
		case COMMAND_BC_START:
			this->CommandBCStart(lpObj);
			break;
		case COMMAND_DS_START:
			this->CommandDSStart(lpObj);
			break;
		case COMMAND_CC_START:
			this->CommandCCStart(lpObj);
			break;
		case COMMAND_IT_START:
			this->CommandITStart(lpObj);
			break;
		case COMMAND_SET_BUFF:
			this->CommandSetBuff(lpObj,argument);
			break;
	}
}

void CCommandManager::DiscountRequirement(LPOBJ lpObj,int index) // OK
{
	COMMAND_LIST_INFO lpInfo;

	if(this->GetCommandCode(index,&lpInfo) == 0)
	{
		return;
	}

	if(lpInfo.Money[lpObj->AccountLevel] > 0)
	{
		lpObj->Money -= lpInfo.Money[lpObj->AccountLevel];

		GCMoneySend(lpObj->Index,lpObj->Money);
	}

	if(lpInfo.Coin1[lpObj->AccountLevel] > 0 || lpInfo.Coin2[lpObj->AccountLevel] > 0 || lpInfo.Coin3[lpObj->AccountLevel] > 0)
	{
		gObjCoinSub(lpObj->Index,lpInfo.Coin1[lpObj->AccountLevel],lpInfo.Coin2[lpObj->AccountLevel],lpInfo.Coin3[lpObj->AccountLevel]);
	}

	lpObj->CommandCheckTime[lpInfo.Index] = GetTickCount();

	gScriptLoader.OnCommandDone(lpObj->Index,index);
}

void CCommandManager::CommandMove(LPOBJ lpObj,char* arg) // OK
{
	if(lpObj->Permission[12] != 0)
	{
		return;
	}

	MOVE_INFO MoveInfo;

	if(gMove.GetInfoByName(arg,&MoveInfo) != 0)
	{
		gMove.Move(lpObj,MoveInfo.Index);

		this->DiscountRequirement(lpObj,COMMAND_MOVE);
	}
}

void CCommandManager::CommandGuildWar(LPOBJ lpObj,char* arg) // OK
{
	char GuildName[9] = {0};

	this->GetString(arg,GuildName,sizeof(GuildName),0);

	if(strlen(GuildName) >= 1)
	{
		gGuild.GuildWarRequestSend(lpObj->Index,GuildName,0);

		this->DiscountRequirement(lpObj,COMMAND_GUILD_WAR);
	}
}

void CCommandManager::CommandBattleSoccer(LPOBJ lpObj,char* arg) // OK
{
	char GuildName[9] = {0};

	this->GetString(arg,GuildName,sizeof(GuildName),0);

	if(strlen(GuildName) >= 1)
	{
		gGuild.GuildWarRequestSend(lpObj->Index,GuildName,1);

		this->DiscountRequirement(lpObj,COMMAND_BATTLE_SOCCER);
	}
}

void CCommandManager::CommandRequest(LPOBJ lpObj,char* arg) // OK
{
	char mode[5] = {0};

	this->GetString(arg,mode,sizeof(mode),0);

	if(strcmp(mode,"on") == 0)
	{
		lpObj->Option |= 1;
		lpObj->Option |= 2;
		lpObj->Option &= ~4;

		memset(lpObj->AutoPartyPassword,0,sizeof(lpObj->AutoPartyPassword));

		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(320));

		this->DiscountRequirement(lpObj,COMMAND_REQUEST);
	}
	else if(strcmp(mode,"off") == 0)
	{
		lpObj->Option &= ~1;
		lpObj->Option &= ~2;
		lpObj->Option &= ~4;

		memset(lpObj->AutoPartyPassword,0,sizeof(lpObj->AutoPartyPassword));

		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(321));

		this->DiscountRequirement(lpObj,COMMAND_REQUEST);
	}
	else if(strcmp(mode,"auto") == 0)
	{
		lpObj->Option |= 1;
		lpObj->Option |= 2;
		lpObj->Option |= 4;

		memset(lpObj->AutoPartyPassword,0,sizeof(lpObj->AutoPartyPassword));

		this->GetString(arg,lpObj->AutoPartyPassword,sizeof(lpObj->AutoPartyPassword),1);

		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(322));

		this->DiscountRequirement(lpObj,COMMAND_REQUEST);
	}
}

void CCommandManager::CommandPost(LPOBJ lpObj,char* arg) // OK
{
	if(lpObj->ChatLimitTime > 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(269),lpObj->ChatLimitTime);
		return;
	}

	if(gServerInfo.m_CommandPostType == 0)
	{
		PostMessage1(lpObj->Name,gMessage.GetMessage(323),arg);
	}
	else if(gServerInfo.m_CommandPostType == 1)
	{
		PostMessage2(lpObj->Name,gMessage.GetMessage(323),arg);
	}
	else if(gServerInfo.m_CommandPostType == 2)
	{
		PostMessage3(lpObj->Name,gMessage.GetMessage(323),arg);
	}
	else if(gServerInfo.m_CommandPostType == 3)
	{
		PostMessage4(lpObj->Name,gMessage.GetMessage(323),arg);
	}
	else if(gServerInfo.m_CommandPostType == 4)
	{
		GDGlobalPostSend(gMapServerManager.GetMapServerGroup(),0,lpObj->Name,arg);
	}
	else if(gServerInfo.m_CommandPostType == 5)
	{
		GDGlobalPostSend(gMapServerManager.GetMapServerGroup(),1,lpObj->Name,arg);
	}
	else if(gServerInfo.m_CommandPostType == 6)
	{
		GDGlobalPostSend(gMapServerManager.GetMapServerGroup(),2,lpObj->Name,arg);
	}
	else if(gServerInfo.m_CommandPostType == 7)
	{
		GDGlobalPostSend(gMapServerManager.GetMapServerGroup(),3,lpObj->Name,arg);
	}
	else
	{
		return;
	}

	gLog.Output(LOG_COMMAND,"[CommandPost][%s][%s] - (Message: %s)",lpObj->Account,lpObj->Name,arg);

	this->DiscountRequirement(lpObj,COMMAND_POST);
}

void CCommandManager::CommandAddPoint(LPOBJ lpObj,char* arg,int type) // OK
{
	if(lpObj->Class != CLASS_DL && type == 4)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(324));
		return;
	}

	char mode[5] = {0};

	this->GetString(arg,mode,sizeof(mode),0);

	if(strcmp(mode,"auto") == 0)
	{
		this->CommandAddPointAuto(lpObj,arg,type);
		return;
	}

	int amount = this->GetNumber(arg,0);

	if(amount <= 0 || lpObj->LevelUpPoint < amount)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(325));
		return;
	}

	if(gObjectManager.CharacterLevelUpPointAdd(lpObj,type,amount) == 0)
	{
		return;
	}

	GCNewCharacterInfoSend(lpObj);

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(326),amount,lpObj->LevelUpPoint);

	gLog.Output(LOG_COMMAND,"[CommandAddPoint][%s][%s] - (Type: %d,Amount: %d)",lpObj->Account,lpObj->Name,type,amount);

	this->DiscountRequirement(lpObj,(COMMAND_ADD_POINT1+type));
}

void CCommandManager::CommandAddPointAuto(LPOBJ lpObj,char* arg,int type) // OK
{
	int amount = this->GetNumber(arg,1);

	if(amount > 0)
	{
		lpObj->AutoAddPointCount = 0;

		lpObj->AutoAddPointStats[type] = amount;

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[0]>0) ? (lpObj->AutoAddPointCount+1) : lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[1]>0) ? (lpObj->AutoAddPointCount+1) : lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[2]>0) ? (lpObj->AutoAddPointCount+1) : lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[3]>0) ? (lpObj->AutoAddPointCount+1) : lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[4]>0) ? (lpObj->AutoAddPointCount+1) : lpObj->AutoAddPointCount);

		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(327),amount);

		gLog.Output(LOG_COMMAND,"[CommandAddPointAuto][%s][%s] - (Type: %d,Amount: %d)",lpObj->Account,lpObj->Name,type,amount);
	}
	else
	{
		lpObj->AutoAddPointCount = 0;

		lpObj->AutoAddPointStats[type] = 0;

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[0]>0) ? (lpObj->AutoAddPointCount+1) : lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[1]>0) ? (lpObj->AutoAddPointCount+1) : lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[2]>0) ? (lpObj->AutoAddPointCount+1) : lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[3]>0) ? (lpObj->AutoAddPointCount+1) : lpObj->AutoAddPointCount);

		lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[4]>0) ? (lpObj->AutoAddPointCount+1) : lpObj->AutoAddPointCount);

		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(328));
	}

	this->DiscountRequirement(lpObj,(COMMAND_ADD_POINT1+type));
}

void CCommandManager::CommandPKClear(LPOBJ lpObj,char* syntax) // OK
{
	if(lpObj->PKLevel < 4)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(329));
		return;
	}

	if(gServerInfo.m_CommandPKClearType == 0)
	{
		lpObj->PKLevel = 3;

		lpObj->PKCount = 0;

		lpObj->PKTime = 0;

		GCPKLevelSend(lpObj->Index,lpObj->PKLevel);

		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(330));

		gLog.Output(LOG_COMMAND,"[CommandPKClear][%s][%s] - (PKLevel: %d)",lpObj->Account,lpObj->Name,lpObj->PKLevel);

		this->DiscountRequirement(lpObj,COMMAND_PK_CLEAR);
	}
	else
	{
		DWORD money = (DWORD)(gServerInfo.m_CommandPKClearMoney[lpObj->AccountLevel]*lpObj->PKCount);

		if(lpObj->Money < money)
		{
			gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(306),money,syntax);
			return;
		}

		lpObj->PKLevel = 3;

		lpObj->PKCount = 0;

		lpObj->PKTime = 0;

		GCPKLevelSend(lpObj->Index,lpObj->PKLevel);

		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(330));

		gLog.Output(LOG_COMMAND,"[CommandPKClear][%s][%s] - (PKLevel: %d)",lpObj->Account,lpObj->Name,lpObj->PKLevel);

		lpObj->Money -= money;

		GCMoneySend(lpObj->Index,lpObj->Money);
	}
}

void CCommandManager::CommandMoney(LPOBJ lpObj,char* arg) // OK
{
	int money = this->GetNumber(arg,0);

	if(money <= 0)
	{
		lpObj->Money = 0;
	}
	else if(gObjCheckMaxMoney(lpObj->Index,money) == 0)
	{
		lpObj->Money = MAX_MONEY;
	}
	else
	{
		lpObj->Money += money;
	}

	GCMoneySend(lpObj->Index,lpObj->Money);

	gLog.Output(LOG_COMMAND,"[CommandMoney][%s][%s] - (Money: %d)",lpObj->Account,lpObj->Name,money);

	this->DiscountRequirement(lpObj,COMMAND_MONEY);
}

void CCommandManager::CommandChange(LPOBJ lpObj) // OK
{
	int ChangeUp = lpObj->ChangeUp;

	if(lpObj->Class == CLASS_MG || lpObj->Class == CLASS_DL || lpObj->Class == CLASS_RF)
	{
		ChangeUp++;
	}

	if(ChangeUp >= gServerInfo.m_CommandChangeLimit[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(331));
		return;
	}

	for(int n = 0; n <= ((ChangeUp == 0) ? 3 : 6); n++)
	{
		if(gQuest.CheckQuestListState(lpObj,n,QUEST_FINISH) == 0)
		{
			if(n != 3 || lpObj->Class == CLASS_DK)
			{
				gQuest.AddQuestList(lpObj,n,QUEST_ACCEPT);
				gQuestReward.InsertQuestReward(lpObj,n);
				gQuest.AddQuestList(lpObj,n,QUEST_FINISH);
			}
		}
	}

	lpObj->SendQuestInfo = 0;

	gQuest.GCQuestInfoSend(lpObj->Index);

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(332));

	gLog.Output(LOG_COMMAND,"[CommandChange][%s][%s] - (ChangeUp: %d)",lpObj->Account,lpObj->Name,ChangeUp);

	this->DiscountRequirement(lpObj,COMMAND_CHANGE);
}

void CCommandManager::CommandWare(LPOBJ lpObj,char* arg) // OK
{
	if(lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != TELEPORT_NONE || lpObj->LoadWarehouse != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(333));
		return;
	}

	int number = this->GetNumber(arg,0);

	if(number < 0 || number >= gServerInfo.m_CommandWareNumber[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(334));
		return;
	}

	lpObj->WarehouseMoney = 0;

	memset(lpObj->WarehouseMap,0xFF,WAREHOUSE_SIZE);

	for(int n = 0; n < WAREHOUSE_SIZE; n++)
	{
		lpObj->Warehouse[n].Clear();
	}

	lpObj->Interface.use = 0;

	lpObj->Interface.type = INTERFACE_NONE;

	lpObj->Interface.state = 0;

	lpObj->LoadWarehouse = 0;

	lpObj->WarehouseNumber = number;

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(335),number);

	gLog.Output(LOG_COMMAND,"[CommandWare][%s][%s] - (Number: %d)",lpObj->Account,lpObj->Name,number);

	this->DiscountRequirement(lpObj,COMMAND_WARE);
}

void CCommandManager::CommandReset(LPOBJ lpObj,char* arg) // OK
{
	char mode[5] = {0};

	this->GetString(arg,mode,sizeof(mode),0);

	if(strcmp(mode,"auto") == 0)
	{
		this->CommandResetAuto(lpObj,arg);
		return;
	}

	if(lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != TELEPORT_NONE || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(336));
		return;
	}

	if(lpObj->Level < gResetTable.GetResetLevel(lpObj))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(337),gResetTable.GetResetLevel(lpObj));
		return;
	}

	if(lpObj->Money < gResetTable.GetResetMoney(lpObj))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(338),gResetTable.GetResetMoney(lpObj));
		return;
	}

	if(lpObj->Coin1 < gResetTable.GetResetCoin1(lpObj))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(339),gResetTable.GetResetCoin1(lpObj));
		return;
	}

	if(lpObj->Coin2 < gResetTable.GetResetCoin2(lpObj))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(340),gResetTable.GetResetCoin2(lpObj));
		return;
	}

	if(lpObj->Coin3 < gResetTable.GetResetCoin3(lpObj))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(341),gResetTable.GetResetCoin3(lpObj));
		return;
	}

	if(lpObj->Reset >= gServerInfo.m_CommandResetLimit[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(342),gServerInfo.m_CommandResetLimit[lpObj->AccountLevel]);
		return;
	}

	if(gServerInfo.m_CommandResetCheckItem[lpObj->AccountLevel] != 0)
	{
		for(int n = 0; n < INVENTORY_WEAR_SIZE; n++)
		{
			if(lpObj->Inventory[n].IsItem() != 0)
			{
				gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(343));
				return;
			}
		}
	}

	if(((lpObj->CommandManagerTransaction[0] == 0) ? (lpObj->CommandManagerTransaction[0]++) : lpObj->CommandManagerTransaction[0]) != 0)
	{
		return;
	}

	SDHP_COMMAND_RESET_SEND pMsg;

	pMsg.header.set(0x0F,0x00,sizeof(pMsg));

	pMsg.index = lpObj->Index;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpObj->Name,sizeof(pMsg.name));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,lpObj->Index);
}

void CCommandManager::CommandResetAuto(LPOBJ lpObj,char* arg) // OK
{
	if(gServerInfo.m_CommandResetAutoEnable[lpObj->AccountLevel] == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(344));
		return;
	}

	if(lpObj->AutoResetEnable == 0)
	{
		lpObj->AutoResetEnable = 1;

		lpObj->AutoResetStats[0] = this->GetNumber(arg,1);

		lpObj->AutoResetStats[1] = this->GetNumber(arg,2);

		lpObj->AutoResetStats[2] = this->GetNumber(arg,3);

		lpObj->AutoResetStats[3] = this->GetNumber(arg,4);

		lpObj->AutoResetStats[4] = ((lpObj->Class == CLASS_DL) ? this->GetNumber(arg,5) : 0);

		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(345));
	}
	else
	{
		lpObj->AutoResetEnable = 0;

		lpObj->AutoResetStats[0] = 0;

		lpObj->AutoResetStats[1] = 0;

		lpObj->AutoResetStats[2] = 0;

		lpObj->AutoResetStats[3] = 0;

		lpObj->AutoResetStats[4] = 0;

		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(346));
	}
}

void CCommandManager::CommandMasterReset(LPOBJ lpObj,char* arg) // OK
{
	if(lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != TELEPORT_NONE || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(351));
		return;
	}

	if(lpObj->Level < gServerInfo.m_CommandMasterResetLevel[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(352),gServerInfo.m_CommandMasterResetLevel[lpObj->AccountLevel]);
		return;
	}

	if(lpObj->Reset < gServerInfo.m_CommandMasterResetReset[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(353),gServerInfo.m_CommandMasterResetReset[lpObj->AccountLevel]);
		return;
	}

	if(lpObj->Money < ((DWORD)gServerInfo.m_CommandMasterResetMoney[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(354),gServerInfo.m_CommandMasterResetMoney[lpObj->AccountLevel]);
		return;
	}

	if(lpObj->Coin1 < ((DWORD)gServerInfo.m_CommandMasterResetCoin1[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(355),gServerInfo.m_CommandMasterResetCoin1[lpObj->AccountLevel]);
		return;
	}

	if(lpObj->Coin2 < ((DWORD)gServerInfo.m_CommandMasterResetCoin2[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(356),gServerInfo.m_CommandMasterResetCoin2[lpObj->AccountLevel]);
		return;
	}

	if(lpObj->Coin3 < ((DWORD)gServerInfo.m_CommandMasterResetCoin3[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(357),gServerInfo.m_CommandMasterResetCoin3[lpObj->AccountLevel]);
		return;
	}

	if(lpObj->MasterReset >= gServerInfo.m_CommandMasterResetLimit[lpObj->AccountLevel])
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(358),gServerInfo.m_CommandMasterResetLimit[lpObj->AccountLevel]);
		return;
	}

	if(gServerInfo.m_CommandMasterResetCheckItem[lpObj->AccountLevel] != 0)
	{
		for(int n = 0; n < INVENTORY_WEAR_SIZE; n++)
		{
			if(lpObj->Inventory[n].IsItem() != 0)
			{
				gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(359));
				return;
			}
		}
	}

	if(((lpObj->CommandManagerTransaction[0] == 0) ? (lpObj->CommandManagerTransaction[0]++) : lpObj->CommandManagerTransaction[0]) != 0)
	{
		return;
	}

	SDHP_COMMAND_MASTER_RESET_SEND pMsg;

	pMsg.header.set(0x0F,0x01,sizeof(pMsg));

	pMsg.index = lpObj->Index;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpObj->Name,sizeof(pMsg.name));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,lpObj->Index);
}

void CCommandManager::CommandClearInventory(LPOBJ lpObj) // OK
{
	int MaxValue = gItemManager.GetInventoryMaxValue(lpObj);

	for(int n=INVENTORY_WEAR_SIZE;n < MaxValue;n++)
	{
		if(lpObj->Inventory[n].m_Index == GET_ITEM(13,19) || lpObj->Inventory[n].m_Index == GET_ITEM(14,64)) // Absolute Weapon of Archangel ,Cursed Castle Water
		{
			continue;
		}

		gItemManager.InventoryDelItem(lpObj->Index,n);
		gItemManager.GCItemDeleteSend(lpObj->Index,n,1);
	}

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(364));

	gLog.Output(LOG_COMMAND,"[CommandClearInventory][%s][%s] - Inventory cleaned",lpObj->Account,lpObj->Name);

	this->DiscountRequirement(lpObj,COMMAND_CLEAR_INVENTORY);
}

void CCommandManager::CommandRedistribute(LPOBJ lpObj) // OK
{
	int Point = 0;

	int str = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class,0);
	int agi = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class,1);
	int vit = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class,2);
	int ene = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class,3);
	int cmd = gDefaultClassInfo.GetCharacterDefaultStat(lpObj->Class,4);

	Point += (lpObj->Strength - str);
	lpObj->Strength = str;

	Point += (lpObj->Dexterity - agi);
	lpObj->Dexterity = agi;

	Point += (lpObj->Vitality - vit);
	lpObj->Vitality = vit;

	Point += (lpObj->Energy - ene);
	lpObj->Energy = ene;

	if(lpObj->Class == CLASS_DL)
	{
		Point += (lpObj->Leadership - cmd);
		lpObj->Leadership = cmd;
	}

	lpObj->LevelUpPoint += Point;

	GCNewCharacterInfoSend(lpObj);
	GDCharacterInfoSaveSend(lpObj->Index);
	gObjectManager.CharacterCalcAttribute(lpObj->Index);

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(365));

	gLog.Output(LOG_COMMAND,"[CommandRedistribute][%s][%s] - Points restored.",lpObj->Account,lpObj->Name);

	this->DiscountRequirement(lpObj,COMMAND_REDISTRIBUTE);
}

void CCommandManager::CommandTreeReset(LPOBJ lpObj,char* arg) // OK
{
	#if(GAMESERVER_UPDATE>=402)

	int Type = this->GetNumber(arg,0);

	if(gMasterSkillTree.CheckMasterLevel(lpObj) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(366));
		return;
	}

	if(Type < 0 || Type > 3)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(367));
		return;
	}

	if(gMasterSkillTree.ResetMasterSkill(lpObj,Type) == 0)
	{
		return;
	}

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(368));

	gLog.Output(LOG_COMMAND,"[CommandTreeReset][%s][%s] - Tree Reseted(%d).",lpObj->Account,lpObj->Name,Type);

	this->DiscountRequirement(lpObj,COMMAND_TREE_RESET);

	#endif
}

void CCommandManager::CommandRename(LPOBJ lpObj,char* arg) // OK
{
	if(lpObj->Guild != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(712));
		return;
	}

	char name[11] = {0};

	this->GetString(arg,name,sizeof(name),0);

	if(strlen(name) < 4)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(689));
		return;
	}

	if(gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,1) == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(690));
		return;
	}

	if(lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != TELEPORT_NONE || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(691));
		return;
	}

	SDHP_COMMAND_RENAME_SEND pMsg;

	pMsg.header.set(0x0F,0x04,sizeof(pMsg));

	pMsg.index = lpObj->Index;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpObj->Name,sizeof(pMsg.name));

	memcpy(pMsg.newname,name,sizeof(pMsg.newname));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,lpObj->Index);
}

void CCommandManager::CommandDCFriend(LPOBJ lpObj,char* arg) // OK
{
	char name[11] = {0};

	this->GetString(arg,name,sizeof(name),0);

	char password[11] = {0};

	this->GetString(arg,password,sizeof(password),1);

	LPOBJ lpTarget = gObjFind(name);

	if(lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(696));
		return;
	}

	if(strcmp(password,lpTarget->Password) != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(697));
		return;
	}

	gObjUserKill(lpTarget->Index);

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(698));

	gLog.Output(LOG_COMMAND,"[CommandDCFriend][%s][%s] - [%s][%s]",lpObj->Account,lpObj->Name,lpTarget->Account,lpTarget->Name);

	this->DiscountRequirement(lpObj,COMMAND_DCFRIEND);
}

void CCommandManager::CommandGMMove(LPOBJ lpObj,char* arg) // OK
{
	char name[11] = {0};

	this->GetString(arg,name,sizeof(name),0);

	int map = this->GetNumber(arg,1);

	int x = this->GetNumber(arg,2);

	int y = this->GetNumber(arg,3);

	LPOBJ lpTarget = gObjFind(name);

	if(lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(369));
		return;
	}

	gObjTeleport(lpTarget->Index,map,x,y);

	gLog.Output(LOG_COMMAND,"[CommandGMMove][%s][%s] - (Name: %s,Map: %d,X: %d,Y: %d)",lpObj->Account,lpObj->Name,name,map,x,y);

	this->DiscountRequirement(lpObj,COMMAND_GM_MOVE);
}

void CCommandManager::CommandGMPost(LPOBJ lpObj,char* arg) // OK
{
	if(gServerInfo.m_CommandGMNoticeType == 0)
	{
		gNotice.GCNoticeSendToAll(0,0,0,0,0,0,"[%s] %s",lpObj->Name,arg);
	}
	else
	{
		char buff[256];

		wsprintf(buff,"[%s] %s",lpObj->Name,arg);

		GDGlobalNoticeSend(gMapServerManager.GetMapServerGroup(),0,0,0,0,0,0,buff);
	}

	gLog.Output(LOG_COMMAND,"[CommandGMPost][%s][%s] - (Message: %s)",lpObj->Account,lpObj->Name,arg);

	this->DiscountRequirement(lpObj,COMMAND_GM_POST);
}

void CCommandManager::CommandTrack(LPOBJ lpObj,char* arg) // OK
{
	char name[11] = {0};

	this->GetString(arg,name,sizeof(name),0);

	LPOBJ lpTarget = gObjFind(name);

	if(lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(369));
		return;
	}

	gObjTeleport(lpObj->Index,lpTarget->Map,lpTarget->X,lpTarget->Y);

	gLog.Output(LOG_COMMAND,"[CommandTrack][%s][%s] - (Name: %s)",lpObj->Account,lpObj->Name,name);

	this->DiscountRequirement(lpObj,COMMAND_TRACK);
}

void CCommandManager::CommandTrace(LPOBJ lpObj,char* arg) // OK
{
	char name[11] = {0};

	this->GetString(arg,name,sizeof(name),0);

	LPOBJ lpTarget = gObjFind(name);

	if(lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(369));
		return;
	}

	gObjTeleport(lpTarget->Index,lpObj->Map,lpObj->X,lpObj->Y);

	gLog.Output(LOG_COMMAND,"[CommandTrace][%s][%s] - (Name: %s)",lpObj->Account,lpObj->Name,name);

	this->DiscountRequirement(lpObj,COMMAND_TRACE);
}

void CCommandManager::CommandDisconnect(LPOBJ lpObj,char* arg) // OK
{
	char name[11] = {0};

	this->GetString(arg,name,sizeof(name),0);

	LPOBJ lpTarget = gObjFind(name);

	if(lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(369));
		return;
	}

	gObjUserKill(lpTarget->Index);

	gLog.Output(LOG_COMMAND,"[CommandDisconnect][%s][%s] - (Name: %s)",lpObj->Account,lpObj->Name,name);

	this->DiscountRequirement(lpObj,COMMAND_DISCONNECT);
}

void CCommandManager::CommandFireworks(LPOBJ lpObj) // OK
{
	for(int n=0;n < 10;n++)
	{
		GCServerCommandSend(lpObj->Index, 0, (lpObj->X + (((GetLargeRand() % 5) * 2) - 4)), (lpObj->Y + (((GetLargeRand() % 5) * 2) - 4)), 1);
	}

	gLog.Output(LOG_COMMAND,"[CommandFireworks][%s][%s] - (Map: %d,X: %d,Y: %d)",lpObj->Account,lpObj->Name,lpObj->Map,lpObj->X,lpObj->Y);

	this->DiscountRequirement(lpObj,COMMAND_FIREWORKS);
}

void CCommandManager::CommandMake(LPOBJ lpObj,char* arg) // OK
{
	int count = this->GetNumber(arg,0);

	if(count < 0)
	{
		int section = this->GetNumber(arg,1);
		int type = this->GetNumber(arg,2);
		int level = this->GetNumber(arg,3);
		int skill = this->GetNumber(arg,4);
		int luck = this->GetNumber(arg,5);
		int option = this->GetNumber(arg,6);
		int exc = this->GetNumber(arg,7);
		int set = this->GetNumber(arg,8);
		int joh = this->GetNumber(arg,9);
		int o380 = this->GetNumber(arg,10);
		int socket = this->GetNumber(arg,11);

		if(IsValidItem(GET_ITEM(section,type)) == 0)
		{
			return;
		}

		BYTE ItemSocketOption[MAX_SOCKET_OPTION] = { 0xFF,0xFF,0xFF,0xFF,0xFF };

		for(int n = 0; n < socket && socket <= MAX_SOCKET_OPTION; n++) { ItemSocketOption[n] = 0xFE; }

		count -= count*2;

		for(int n = 0; n < count; n++)
		{
			GDCreateItemSend(lpObj->Index,0xEB,0,0,GET_ITEM(section,type),level,0,skill,luck,option,-1,exc,set,joh,(o380>0)?255:0,ItemSocketOption,0xFF,0);
		}

		gLog.Output(LOG_COMMAND,"[CommandMake][%s][%s] - Count: %d (ItemIndex: %02d,%03d,Level: %d,Skill: %d,Luck: %d,Option: %d,Exc: %d,Set: %d,JoH: %d,380: %d,Socket: %d)",lpObj->Account,lpObj->Name,count,section,type,level,skill,luck,option,exc,set,joh,(o380>0) ? 255 : 0,socket);
	}
	else
	{
		int type = this->GetNumber(arg,1);
		int level = this->GetNumber(arg,2);
		int skill = this->GetNumber(arg,3);
		int luck = this->GetNumber(arg,4);
		int option = this->GetNumber(arg,5);
		int exc = this->GetNumber(arg,6);
		int set = this->GetNumber(arg,7);
		int joh = this->GetNumber(arg,8);
		int o380 = this->GetNumber(arg,9);
		int socket = this->GetNumber(arg,10);

		if(IsValidItem(GET_ITEM(count,type)) == 0)
		{
			return;
		}

		BYTE ItemSocketOption[MAX_SOCKET_OPTION] = { 0xFF,0xFF,0xFF,0xFF,0xFF };

		for(int n = 0; n < socket && socket <= MAX_SOCKET_OPTION; n++) { ItemSocketOption[n] = 0xFE; }

		GDCreateItemSend(lpObj->Index,0xEB,0,0,GET_ITEM(count,type),level,0,skill,luck,option,-1,exc,set,joh,(o380>0) ? 255 : 0,ItemSocketOption,0xFF,0);

		gLog.Output(LOG_COMMAND,"[CommandMake][%s][%s] - (ItemIndex: %02d,%03d,Level: %d,Skill: %d,Luck: %d,Option: %d,Exc: %d,Set: %d,JoH: %d,380: %d,Socket: %d)",lpObj->Account,lpObj->Name,count,type,level,skill,luck,option,exc,set,joh,(o380>0) ? 255 : 0,socket);
	}

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(371));

	this->DiscountRequirement(lpObj,COMMAND_MAKE);
}

void CCommandManager::CommandDrop(LPOBJ lpObj,char* arg) // OK
{
	int count = this->GetNumber(arg,0);

	if(count < 0)
	{
		int section = this->GetNumber(arg,1);
		int type = this->GetNumber(arg,2);
		int level = this->GetNumber(arg,3);
		int skill = this->GetNumber(arg,4);
		int luck = this->GetNumber(arg,5);
		int option = this->GetNumber(arg,6);
		int exc = this->GetNumber(arg,7);
		int set = this->GetNumber(arg,8);
		int socket = this->GetNumber(arg,9);

		if(IsValidItem(GET_ITEM(section,type)) == 0)
		{
			return;
		}

		BYTE ItemSocketOption[MAX_SOCKET_OPTION] = { 0xFF,0xFF,0xFF,0xFF,0xFF };

		for(int n = 0; n < socket && socket <= MAX_SOCKET_OPTION; n++) { ItemSocketOption[n] = 0xFE; }

		count -= count*2;

		int px = lpObj->X;
		int py = lpObj->Y;

		for(int n = 0; n < count; n++)
		{
			if(gObjGetRandomFreeLocation(lpObj->Map,&px,&py,2,2,10) == 0)
			{
				px = lpObj->X;
				py = lpObj->Y;
			}

			GDCreateItemSend(lpObj->Index,lpObj->Map,px,py,GET_ITEM(section,type),level,0,skill,luck,option,lpObj->Index,exc,set,0,0,ItemSocketOption,0xFF,0);
		}

		gLog.Output(LOG_COMMAND,"[CommandDrop][%s][%s] - Count: %d (ItemIndex: %02d,%03d,Level: %d,Skill: %d,Luck: %d,Option: %d,Exc: %d,Set: %d,Socket: %d)",lpObj->Account,lpObj->Name,count,section,type,level,skill,luck,option,exc,set,socket);
	}
	else
	{
		int type = this->GetNumber(arg,1);
		int level = this->GetNumber(arg,2);
		int skill = this->GetNumber(arg,3);
		int luck = this->GetNumber(arg,4);
		int option = this->GetNumber(arg,5);
		int exc = this->GetNumber(arg,6);
		int set = this->GetNumber(arg,7);
		int socket = this->GetNumber(arg,8);

		if(IsValidItem(GET_ITEM(count,type)) == 0)
		{
			return;
		}

		BYTE ItemSocketOption[MAX_SOCKET_OPTION] = { 0xFF,0xFF,0xFF,0xFF,0xFF };

		for(int n = 0; n < socket && socket <= MAX_SOCKET_OPTION; n++) { ItemSocketOption[n] = 0xFE; }

		GDCreateItemSend(lpObj->Index,lpObj->Map,(BYTE)lpObj->X,(BYTE)lpObj->Y,GET_ITEM(count,type),level,0,skill,luck,option,lpObj->Index,exc,set,0,0,ItemSocketOption,0xFF,0);

		gLog.Output(LOG_COMMAND,"[CommandDrop][%s][%s] - (ItemIndex: %02d,%03d,Level: %d,Skill: %d,Luck: %d,Option: %d,Exc: %d,Set: %d,Socket: %d)",lpObj->Account,lpObj->Name,count,type,level,skill,luck,option,exc,set,socket);
	}

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(371));

	this->DiscountRequirement(lpObj,COMMAND_DROP);
}

void CCommandManager::CommandSkin(LPOBJ lpObj,char* arg) // OK
{
	char name[11] = {0};

	this->GetString(arg,name,sizeof(name),0);

	LPOBJ lpTarget = gObjFind(name);

	if(lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(369));
		return;
	}

	lpTarget->Change = this->GetNumber(arg,1);

	gObjViewportListProtocolCreate(lpTarget);

	gObjectManager.CharacterUpdateMapEffect(lpTarget);

	gLog.Output(LOG_COMMAND,"[CommandSkin][%s][%s] - (Name: %s,Change: %d)",lpObj->Account,lpObj->Name,name,lpTarget->Change);

	this->DiscountRequirement(lpObj,COMMAND_SKIN);
}

void CCommandManager::CommandSetMoney(LPOBJ lpObj,char* arg) // OK
{
	char name[11] = {0};

	this->GetString(arg,name,sizeof(name),0);

	LPOBJ lpTarget = gObjFind(name);

	if(lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(369));
		return;
	}

	int money = this->GetNumber(arg,1);

	if(money <= 0)
	{
		lpTarget->Money = 0;
	}
	else if(gObjCheckMaxMoney(lpTarget->Index,money) == 0)
	{
		lpTarget->Money = MAX_MONEY;
	}
	else
	{
		lpTarget->Money += money;
	}

	GCMoneySend(lpTarget->Index,lpTarget->Money);

	gLog.Output(LOG_COMMAND,"[CommandSetMoney][%s][%s] - (Name: %s,Money: %d)",lpObj->Account,lpObj->Name,name,money);

	this->DiscountRequirement(lpObj,COMMAND_SET_MONEY);
}

void CCommandManager::CommandHide(LPOBJ lpObj) // OK
{
	if(gEffectManager.CheckEffect(lpObj,EFFECT_INVISIBILITY) == 0)
	{
		gEffectManager.AddEffect(lpObj,0,EFFECT_INVISIBILITY,0,0,0,0,0);

		gObjViewportListProtocolDestroy(lpObj);

		gLog.Output(LOG_COMMAND,"[CommandHide][%s][%s] - (State: %d)",lpObj->Account,lpObj->Name,1);
	}
	else
	{
		gEffectManager.DelEffect(lpObj,EFFECT_INVISIBILITY);

		gObjViewportListProtocolCreate(lpObj);

		gLog.Output(LOG_COMMAND,"[CommandHide][%s][%s] - (State: %d)",lpObj->Account,lpObj->Name,0);
	}

	this->DiscountRequirement(lpObj,COMMAND_HIDE);
}

void CCommandManager::CommandEdit(LPOBJ lpObj,char* arg) // OK
{
	char name[11] = {0};

	this->GetString(arg,name,sizeof(name),0);

	LPOBJ lpTarget = gObjFind(name);

	if(lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(369));
		return;
	}

	int level = this->GetNumber(arg,1);
	int points = this->GetNumber(arg,2);

	int str = this->GetNumber(arg,3);
	int agi = this->GetNumber(arg,4);
	int vit = this->GetNumber(arg,5);
	int ene = this->GetNumber(arg,6);
	int cmd = this->GetNumber(arg,7);

	int MAX_STATS = gServerInfo.m_MaxStatPoint[lpTarget->AccountLevel];

	lpTarget->Level = (level<0)?1:lpTarget->Level+level;
	lpTarget->Level = ((lpTarget->Level>gServerInfo.m_MaxLevel)?gServerInfo.m_MaxLevel:lpTarget->Level);

	lpTarget->LevelUpPoint = (points<0)?0:lpTarget->LevelUpPoint+points;

	lpTarget->Strength = (str<0)?gDefaultClassInfo.GetCharacterDefaultStat(lpTarget->Class,0):lpTarget->Strength+str;
	lpTarget->Strength = ((lpTarget->Strength>MAX_STATS)?MAX_STATS:lpTarget->Strength);

	lpTarget->Dexterity = (agi<0)?gDefaultClassInfo.GetCharacterDefaultStat(lpTarget->Class,1):lpTarget->Dexterity+agi;
	lpTarget->Dexterity = ((lpTarget->Dexterity>MAX_STATS)?MAX_STATS:lpTarget->Dexterity);

	lpTarget->Vitality = (vit<0)?gDefaultClassInfo.GetCharacterDefaultStat(lpTarget->Class,2):lpTarget->Vitality+vit;
	lpTarget->Vitality = ((lpTarget->Vitality>MAX_STATS)?MAX_STATS:lpTarget->Vitality);

	lpTarget->Energy = (ene<0)?gDefaultClassInfo.GetCharacterDefaultStat(lpTarget->Class,3):lpTarget->Energy+ene;
	lpTarget->Energy = ((lpTarget->Energy>MAX_STATS)?MAX_STATS:lpTarget->Energy);

	if(lpTarget->Class==CLASS_DL)
	{
		lpTarget->Leadership = (cmd<0)?gDefaultClassInfo.GetCharacterDefaultStat(lpTarget->Class,4):lpTarget->Leadership+cmd;
		lpTarget->Leadership = ((lpTarget->Leadership>MAX_STATS) ? MAX_STATS : lpTarget->Leadership);
	}

	gObjCalcExperience(lpTarget);
	GCNewCharacterInfoSend(lpTarget);
	GDCharacterInfoSaveSend(lpTarget->Index);
	gObjectManager.CharacterCalcAttribute(lpTarget->Index);

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(372));

	if(lpObj->Index != lpTarget->Index)
	{
		gNotice.GCNoticeSend(lpTarget->Index,1,0,0,0,0,0,gMessage.GetMessage(373),lpObj->Name);
	}

	gLog.Output(LOG_COMMAND,"[CommandGMEdit][%s][%s] CharEdit:[%s][%s] Level[%d] LevelUpPoint[%d] Str[%d] Agi[%d] Vit[%d] Ene[%d] Cmd[%d]",lpObj->Account,lpObj->Name,lpTarget->Account,lpTarget->Name,lpTarget->Level,lpTarget->LevelUpPoint,lpTarget->Strength,lpTarget->Dexterity,lpTarget->Vitality,lpTarget->Energy,lpTarget->Leadership);

	this->DiscountRequirement(lpObj,COMMAND_EDIT);
}

void CCommandManager::CommandSetCoin(LPOBJ lpObj,char* arg) // OK
{
	char name[11] = {0};

	this->GetString(arg,name,sizeof(name),0);

	LPOBJ lpTarget = gObjFind(name);

	if(lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(369));
		return;
	}

	int Coin1 = this->GetNumber(arg,1);
	int Coin2 = this->GetNumber(arg,2);
	int Coin3 = this->GetNumber(arg,3);

	if(Coin1 > 0 || Coin2 > 0 || Coin3 > 0)
	{
		gObjCoinAdd(lpTarget->Index,Coin1,Coin2,Coin3);
		gLog.Output(LOG_COMMAND,"[CommandGMSetCoin][%s][%s] - [%s][%s] Increase Coin1[%d] Coin2[%d] Coin3[%d]",lpObj->Account,lpObj->Name,lpTarget->Account,lpTarget->Name,Coin1,Coin2,Coin3);
	}

	if(Coin1 < 0 || Coin2 < 0 || Coin3 < 0)
	{
		gObjCoinSub(lpTarget->Index,Coin1,Coin2,Coin3);
		gLog.Output(LOG_COMMAND,"[CommandSetCoin][%s][%s] - [%s][%s] Decrease Coin1[%d] Coin2[%d] Coin3[%d]",lpObj->Account,lpObj->Name,lpTarget->Account,lpTarget->Name,abs(Coin1),abs(Coin2),abs(Coin3));
	}

	if(lpObj->Index != lpTarget->Index)
	{
		gNotice.GCNoticeSend(lpTarget->Index,1,0,0,0,0,0,gMessage.GetMessage(374),lpObj->Name);
	}

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(375));

	this->DiscountRequirement(lpObj,COMMAND_SET_COIN);
}

void CCommandManager::CommandSetPK(LPOBJ lpObj,char* arg) // OK
{
	char name[11] = {0};

	this->GetString(arg,name,sizeof(name),0);

	LPOBJ lpTarget = gObjFind(name);

	if(lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(369));
		return;
	}

	int PKLevel = this->GetNumber(arg, 1);

	lpTarget->PKLevel = (PKLevel < 0) ? 0 : (PKLevel > 6) ? 6 : PKLevel;

	switch (lpTarget->PKLevel)
	{
		case 0:
		case 6:
			lpTarget->PKCount = 3;
			lpTarget->PKTime = gServerInfo.m_PKDownRequirePoint3;
			break;
		case 1:
		case 5:
			lpTarget->PKCount = 0;
			lpTarget->PKTime = gServerInfo.m_PKDownRequirePoint2;
			break;
		case 2:
		case 4:
			lpTarget->PKCount = 0;
			lpTarget->PKTime = gServerInfo.m_PKDownRequirePoint1;
			break;
		case 3:
			lpTarget->PKCount = 0;
			lpTarget->PKTime = 0;
			break;
	}

	GCPKLevelSend(lpTarget->Index,lpTarget->PKLevel);

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(376));

	gLog.Output(LOG_COMMAND,"[CommandGMSetPK][%s][%s] - (Set: Name:%s PKLevel: %d)",lpObj->Account,lpObj->Name,lpTarget->Name,lpObj->PKLevel);

	this->DiscountRequirement(lpObj,COMMAND_SET_PK);
}

void CCommandManager::CommandSetVip(LPOBJ lpObj,char* arg) // OK
{
	char name[11] = {0};

	this->GetString(arg,name,sizeof(name),0);

	LPOBJ lpTarget = gObjFind(name);

	if(lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(369));
		return;
	}

	int AccountLevel = this->GetNumber(arg,1);

	int Days = this->GetNumber(arg,2);

	if(Days < 0 || AccountLevel < 0 || AccountLevel > 3)
	{
		Days = 0;
		AccountLevel = 0;
	}

	GJAccountLevelSaveSend(lpTarget->Index,AccountLevel,(Days*86400));
	GJAccountLevelSend(lpTarget->Index);

	if(lpObj->Index != lpTarget->Index)
	{
		gNotice.GCNoticeSend(lpTarget->Index,1,0,0,0,0,0,gMessage.GetMessage(377),lpTarget->AccountExpireDate);
	}

	gNotice.GCNoticeSend(lpTarget->Index,1,0,0,0,0,0,gMessage.GetMessage(378));

	gLog.Output(LOG_COMMAND,"[CommandGMSetVip][%s][%s] - (Account: %s,AccountLevel: %d,Days: %d)",lpObj->Account,lpObj->Name,lpTarget->Name,AccountLevel,Days);

	this->DiscountRequirement(lpObj,COMMAND_SET_VIP);
}

void CCommandManager::CommandMoveAll(LPOBJ lpObj,char* arg) // OK
{
	int map = this->GetNumber(arg,0);
	int x = this->GetNumber(arg,1);
	int y = this->GetNumber(arg,2);

	for(int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
	{
		if(gObjIsConnectedGP(n) != 0)
		{
			gObjTeleport(n,map,x,y);
		}
	}

	gLog.Output(LOG_COMMAND,"[CommandMoveAll][%s][%s] (Map: %d,X: %d,Y: %d)",lpObj->Account,lpObj->Name,map,x,y);

	this->DiscountRequirement(lpObj,COMMAND_MOVE_ALL);
}

void CCommandManager::CommandMoveGuild(LPOBJ lpObj,char* arg) // OK
{
	char name[9] = {0};

	this->GetString(arg,name,sizeof(name),0);
	int map = this->GetNumber(arg,1);
	int x = this->GetNumber(arg,2);
	int y = this->GetNumber(arg,3);

	GUILD_INFO* lpGuild = gGuildManager.GetGuild(name);

	if(lpGuild == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(379));
		return;
	}

	for(int n=0;n < MAX_GUILD_MEMBER;n++)
	{
		if(lpGuild->Member[n].IsValid() != 0)
		{
			gObjTeleport(lpGuild->Member[n].Index,map,x,y);
		}
	}

	gLog.Output(LOG_COMMAND,"[CommandMoveGuild][%s][%s] (Guild: %s Map: %d,X: %d,Y: %d)", lpGuild->Name,lpObj->Account,lpObj->Name,map,x,y);

	this->DiscountRequirement(lpObj,COMMAND_MOVE_GUILD);
}

void CCommandManager::CommandMakeMob(LPOBJ lpObj,char* arg) // OK
{
	int monster = this->GetNumber(arg,0);
	int amount = this->GetNumber(arg,1);
	int map = lpObj->Map;
	int px = lpObj->X;
	int py = lpObj->Y;

	if(gMap[map].CheckAttr(px,py,1) != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(380));
		return;
	}

	amount =(amount<1)?1:(amount>50)?50:amount;	

	for (int i=0;i<amount;i++)
	{
		int index = gObjAddMonster(lpObj->Map);

		if(OBJECT_RANGE(index) == 0)
		{
			gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(381));
			return;
		}

		LPOBJ lpMonster = &gObj[index];

		lpMonster->PosNum = -1;
		lpMonster->X = px;
		lpMonster->Y = py;
		lpMonster->TX = px;
		lpMonster->TY = py;
		lpMonster->OldX = px;
		lpMonster->OldY = py;
		lpMonster->StartX = px;
		lpMonster->StartY = py;
		lpMonster->Dir = GetLargeRand() % 8;
		lpMonster->Map = map;

		if(gObjSetMonster(index,monster) == 0)
		{
			gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(381));
			gObjDel(index);
			return;
		}

		if(lpMonster->Type == OBJECT_NPC)
		{
			gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(382));
			gObjDel(index);
			return;
		}
	}

	gLog.Output(LOG_COMMAND,"[CommandMakeMob][%s][%s] (Mob: %d Amount: %d MapNumber: %d PosX: %d PosY: %d)",lpObj->Account,lpObj->Name,monster,amount,map,px,py);

	this->DiscountRequirement(lpObj,COMMAND_MAKE_MOB);
}

void CCommandManager::CommandMakeSet(LPOBJ lpObj,char* arg) // OK
{
	int type = this->GetNumber(arg,0);
	int level = this->GetNumber(arg,1);
	int luck = this->GetNumber(arg,2);
	int option = this->GetNumber(arg,3);
	int exc = this->GetNumber(arg,4);
	int set = this->GetNumber(arg,5);
	int joh = this->GetNumber(arg,6);
	int o380 = this->GetNumber(arg,7);
	int socket = this->GetNumber(arg,8);

	BYTE ItemSocketOption[MAX_SOCKET_OPTION] = { 0xFF,0xFF,0xFF,0xFF,0xFF };

	for(int n = 0; n < socket && socket <= MAX_SOCKET_OPTION; n++) { ItemSocketOption[n] = 0xFE; }

	for(int i=7;i<12; i++)
	{
		if(IsValidItem(GET_ITEM(i,type)) != 0)
		{
			GDCreateItemSend(lpObj->Index,0xEB,0,0,GET_ITEM(i,type),level,0,0,luck,option,-1,exc,set,joh,(o380>0)?255:0,ItemSocketOption,0xFF,0);
		}
	}
	
	gLog.Output(LOG_COMMAND,"[CommandMakeSet][%s][%s] - (Type: %d,Level: %d,Luck: %d,Option: %d,Exc: %d,Set: %d,JoH: %d,380: %d,Socket: %d)",lpObj->Account,lpObj->Name,type,level,luck,option,exc,set,joh,(o380>0)?255:0,socket);

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(371));

	this->DiscountRequirement(lpObj,COMMAND_MAKE_SET);
}

void CCommandManager::CommandBanAcc(LPOBJ lpObj,char* arg) // OK
{
	char name[11] = {0};

	this->GetString(arg,name,sizeof(name),0);

	LPOBJ lpTarget = gObjFind(name);

	if(lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(369));
		return;
	}

	SDHP_COMMAND_BAN_ACCOUNT_SEND pMsg;

	pMsg.header.set(0x0F,0x05,sizeof(pMsg));

	pMsg.index = lpObj->Index;

	memcpy(pMsg.account,lpTarget->Account,sizeof(pMsg.account));

	pMsg.BlocCode = 1;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,lpObj->Index);

	gNotice.GCNoticeSend(lpTarget->Index,1,0,0,0,0,0,gMessage.GetMessage(685));

	gObjUserKill(lpTarget->Index);

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(686),lpTarget->Name);

	gLog.Output(LOG_COMMAND,"[CommandBanAcc][%s][%s] - [%s][%s]",lpObj->Account,lpObj->Name,lpTarget->Account,lpTarget->Name);

	this->DiscountRequirement(lpObj,COMMAND_BAN_ACC);
}

void CCommandManager::CommandBanChar(LPOBJ lpObj,char* arg) // OK
{
	char name[11] = {0};

	this->GetString(arg,name,sizeof(name),0);

	LPOBJ lpTarget = gObjFind(name);

	if(lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(369));
		return;
	}

	SDHP_COMMAND_BAN_CHARACTER_SEND pMsg;

	pMsg.header.set(0x0F,0x06,sizeof(pMsg));

	pMsg.index = lpObj->Index;

	memcpy(pMsg.name, lpTarget->Name, sizeof(pMsg.name));

	pMsg.CtlCode = 1;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,lpObj->Index);

	gNotice.GCNoticeSend(lpTarget->Index,1,0,0,0,0,0,gMessage.GetMessage(685));

	gObjUserKill(lpTarget->Index);

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(686),lpTarget->Name);

	gLog.Output(LOG_COMMAND,"[CommandBanChar][%s][%s] - [%s][%s]",lpObj->Account,lpObj->Name,lpTarget->Account,lpTarget->Name);

	this->DiscountRequirement(lpObj,COMMAND_BAN_CHAR);
}

void CCommandManager::CommandBanChat(LPOBJ lpObj,char* arg) // OK
{
	char name[11] = {0};

	this->GetString(arg,name,sizeof(name),0);

	int minutes = this->GetNumber(arg, 1);

	if(minutes <= 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(699));
		return;
	}

	LPOBJ lpTarget = gObjFind(name);

	if(lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(369));
		return;
	}

	lpTarget->ChatLimitTime = minutes;

	lpTarget->ChatLimitTimeSec = 0;

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(701),lpTarget->Name,minutes);

	gLog.Output(LOG_COMMAND,"[CommandBanChat][%s][%s] - [%s][%s] (Minutes: %d)",lpObj->Account,lpObj->Name,lpTarget->Account,lpTarget->Name,minutes);

	this->DiscountRequirement(lpObj,COMMAND_BAN_CHAT);
}

void CCommandManager::CommandUnbanAcc(LPOBJ lpObj,char* arg) // OK
{
	char name[11] = {0};

	this->GetString(arg,name,sizeof(name),0);

	if(strlen(name) < 4)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(689));
		return;
	}

	SDHP_COMMAND_BAN_ACCOUNT_SEND pMsg;

	pMsg.header.set(0x0F,0x05,sizeof(pMsg));

	pMsg.index = lpObj->Index;

	memcpy(pMsg.account,name,sizeof(pMsg.account));

	pMsg.BlocCode = 0;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,lpObj->Index);
}

void CCommandManager::CommandUnbanChar(LPOBJ lpObj,char* arg) // OK
{
	char name[11] = {0};

	this->GetString(arg,name,sizeof(name),0);

	if(strlen(name) < 4)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(689));
		return;
	}

	SDHP_COMMAND_BAN_CHARACTER_SEND pMsg;

	pMsg.header.set(0x0F,0x06,sizeof(pMsg));

	pMsg.index = lpObj->Index;

	memcpy(pMsg.name,name,sizeof(pMsg.name));

	pMsg.CtlCode = 0;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,lpObj->Index);
}

void CCommandManager::CommandUnbanChat(LPOBJ lpObj,char* arg) // OK
{
	char name[11] = {0};

	this->GetString(arg,name,sizeof(name),0);

	LPOBJ lpTarget = gObjFind(name);

	if(lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(369));
		return;
	}

	lpTarget->ChatLimitTime = 0;

	lpTarget->ChatLimitTimeSec = 0;

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(700),lpTarget->Name);

	gLog.Output(LOG_COMMAND,"[CommandUnbanChat][%s][%s] - [%s][%s]",lpObj->Account,lpObj->Name,lpTarget->Account,lpTarget->Name);

	this->DiscountRequirement(lpObj,COMMAND_UNBAN_CHAT);
}

void CCommandManager::CommandBCStart(LPOBJ lpObj) // OK
{
	gBloodCastle.ForceStart();

	gLog.Output(LOG_COMMAND,"[CommandBCStart][%s][%s] ForceStart",lpObj->Account,lpObj->Name);

	this->DiscountRequirement(lpObj,COMMAND_BC_START);
}

void CCommandManager::CommandDSStart(LPOBJ lpObj) // OK
{
	gDevilSquare.ForceStart();

	gLog.Output(LOG_COMMAND,"[CommandDSStart][%s][%s] ForceStart",lpObj->Account,lpObj->Name);

	this->DiscountRequirement(lpObj,COMMAND_DS_START);
}

void CCommandManager::CommandCCStart(LPOBJ lpObj) // OK
{
	gChaosCastle.ForceStart();

	gLog.Output(LOG_COMMAND,"[CommandCCStart][%s][%s] ForceStart",lpObj->Account,lpObj->Name);

	this->DiscountRequirement(lpObj,COMMAND_CC_START);
}

void CCommandManager::CommandITStart(LPOBJ lpObj) // OK
{
	#if(GAMESERVER_UPDATE>=301)

	gIllusionTemple.ForceStart();

	gLog.Output(LOG_COMMAND,"[CommandITStart][%s][%s] ForceStart",lpObj->Account,lpObj->Name);

	this->DiscountRequirement(lpObj,COMMAND_IT_START);

	#endif
}

void CCommandManager::CommandSetBuff(LPOBJ lpObj,char* arg) // OK
{
	char name[11] = {0};

	this->GetString(arg,name,sizeof(name),0);

	LPOBJ lpTarget = gObjFind(name);

	if(lpTarget == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(369));
		return;
	}

	int Effect = this->GetNumber(arg,1);
	int Type = this->GetNumber(arg,2);
	int Duration = this->GetNumber(arg,3);
	int Power1 = this->GetNumber(arg,4);
	int Power2 = this->GetNumber(arg,5);
	int Power3 = this->GetNumber(arg,6);
	int Power4 = this->GetNumber(arg,7);

	if(Effect != -1)
	{
		if(gEffectManager.GetInfo(Effect) == 0)
		{
			gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(395));
			return;
		}

		if(gEffectManager.CheckEffect(lpTarget,Effect) == 0)
		{
			gEffectManager.AddEffect(lpTarget,(Type>0)?1:0,Effect,(Type==0)?Duration:(int)(time(0)+Duration),Power1,Power2,Power3,Power4);

			gLog.Output(LOG_COMMAND,"[CommandSetBuff][%s][%s] - AddBuff [%s][%s] (Buff: %d Duration: %d Power1: %d Power2: %d Power3: %d Power4: %d)",lpObj->Account,lpObj->Name,lpTarget->Account,lpTarget->Name,Effect,Duration,Power1,Power2,Power3,Power4);
		}
		else
		{
			gEffectManager.DelEffect(lpTarget,Effect);

			gLog.Output(LOG_COMMAND,"[CommandSetBuff][%s][%s] - DelBuff [%s][%s] (Buff: %d)",lpObj->Account,lpObj->Name,lpTarget->Account,lpTarget->Name,Effect);
		}
	}
	else
	{
		gEffectManager.ClearAllEffect(lpTarget);

		gLog.Output(LOG_COMMAND,"[CommandSetBuff][%s][%s] - DelAllBuff [%s][%s]",lpObj->Account,lpObj->Name,lpTarget->Account,lpTarget->Name);
	}

	this->DiscountRequirement(lpObj,COMMAND_SET_BUFF);
}

void CCommandManager::CommandAddPointAutoProc(LPOBJ lpObj) // OK
{
	if(lpObj->LevelUpPoint == 0 || lpObj->AutoAddPointCount == 0)
	{
		return;
	}

	int AddStatCount = lpObj->AutoAddPointCount;

	int* stat[5] = { &lpObj->Strength,&lpObj->Dexterity,&lpObj->Vitality,&lpObj->Energy,&lpObj->Leadership };

	for(int n = 0; n < 5; n++)
	{
		if(lpObj->AutoAddPointStats[n] > 0)
		{
			if(AddStatCount == 0)
			{
				break;
			}

			int AddStat = lpObj->LevelUpPoint/AddStatCount;

			AddStat = (((lpObj->LevelUpPoint%AddStatCount)>0) ? (AddStat+1) : AddStat);

			AddStat = ((AddStat>lpObj->AutoAddPointStats[n]) ? lpObj->AutoAddPointStats[n] : AddStat);

			AddStat = ((((*stat[n])+AddStat)>gServerInfo.m_MaxStatPoint[lpObj->AccountLevel]) ? (gServerInfo.m_MaxStatPoint[lpObj->AccountLevel] - (*stat[n])) : AddStat);

			AddStatCount--;

			(*stat[n]) += AddStat;

			lpObj->LevelUpPoint -= AddStat;

			lpObj->AutoAddPointStats[n] -= (((*stat[n]) >= gServerInfo.m_MaxStatPoint[lpObj->AccountLevel]) ? lpObj->AutoAddPointStats[n] : AddStat);

			lpObj->AutoAddPointCount = 0;

			lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[0]>0) ? (lpObj->AutoAddPointCount+1) : lpObj->AutoAddPointCount);

			lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[1]>0) ? (lpObj->AutoAddPointCount+1) : lpObj->AutoAddPointCount);

			lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[2]>0) ? (lpObj->AutoAddPointCount+1) : lpObj->AutoAddPointCount);

			lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[3]>0) ? (lpObj->AutoAddPointCount+1) : lpObj->AutoAddPointCount);

			lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[4]>0) ? (lpObj->AutoAddPointCount+1) : lpObj->AutoAddPointCount);
		}
	}

	gObjectManager.CharacterCalcAttribute(lpObj->Index);

	GCNewCharacterInfoSend(lpObj);
}

void CCommandManager::CommandResetAutoProc(LPOBJ lpObj) // OK
{
	if(lpObj->Interface.use != 0 || lpObj->State == OBJECT_DELCMD || lpObj->DieRegen != 0 || lpObj->Teleport != TELEPORT_NONE || lpObj->PShopOpen != 0 || lpObj->SkillSummonPartyTime != 0)
	{
		return;
	}

	if(lpObj->Level < gResetTable.GetResetLevel(lpObj))
	{
		return;
	}

	if(lpObj->Money < gResetTable.GetResetMoney(lpObj))
	{
		return;
	}

	if(lpObj->Coin1 < gResetTable.GetResetCoin1(lpObj))
	{
		return;
	}

	if(lpObj->Coin2 < gResetTable.GetResetCoin2(lpObj))
	{
		return;
	}

	if(lpObj->Coin3 < gResetTable.GetResetCoin3(lpObj))
	{
		return;
	}

	if(lpObj->Reset >= gServerInfo.m_CommandResetLimit[lpObj->AccountLevel])
	{
		return;
	}

	if(gServerInfo.m_CommandResetCheckItem[lpObj->AccountLevel] != 0)
	{
		for(int n = 0; n < INVENTORY_WEAR_SIZE; n++)
		{
			if(lpObj->Inventory[n].IsItem() != 0)
			{
				return;
			}
		}
	}

	if(((lpObj->CommandManagerTransaction[0] == 0) ? (lpObj->CommandManagerTransaction[0]++) : lpObj->CommandManagerTransaction[0]) != 0)
	{
		return;
	}

	SDHP_COMMAND_RESET_SEND pMsg;

	pMsg.header.set(0x0F,0x00,sizeof(pMsg));

	pMsg.index = lpObj->Index;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpObj->Name,sizeof(pMsg.name));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,lpObj->Index);
}

void CCommandManager::DGCommandResetRecv(SDHP_COMMAND_RESET_RECV* lpMsg) // OK
{
	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGCommandResetRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	lpObj->CommandManagerTransaction[0] = 0;

	if(lpMsg->ResetDay >= gResetLimit.GetLimitDay(lpObj))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(347),gResetLimit.GetLimitDay(lpObj));
		return;
	}

	if(lpMsg->ResetWek >= gResetLimit.GetLimitWek(lpObj))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(348),gResetLimit.GetLimitWek(lpObj));
		return;
	}

	if(lpMsg->ResetMon >= gResetLimit.GetLimitMon(lpObj))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(349),gResetLimit.GetLimitMon(lpObj));
		return;
	}

	lpObj->Money -= gResetTable.GetResetMoney(lpObj);

	GCMoneySend(lpObj->Index,lpObj->Money);

	gObjCoinSub(lpObj->Index,gResetTable.GetResetCoin1(lpObj),gResetTable.GetResetCoin2(lpObj),gResetTable.GetResetCoin3(lpObj));

	this->DiscountRequirement(lpObj,COMMAND_RESET);

	lpObj->Level = ((gServerInfo.m_CommandResetStartLevel[lpObj->AccountLevel] == -1) ? (lpObj->Level - gServerInfo.m_CommandResetStartLevel[lpObj->AccountLevel]) : gServerInfo.m_CommandResetStartLevel[lpObj->AccountLevel]);

	lpObj->Experience = gLevelExperience[lpObj->Level - 1];

	lpObj->Reset += gServerInfo.m_CommandResetCount[lpObj->AccountLevel];

	lpMsg->ResetDay += gServerInfo.m_CommandResetCount[lpObj->AccountLevel];

	lpMsg->ResetWek += gServerInfo.m_CommandResetCount[lpObj->AccountLevel];

	lpMsg->ResetMon += gServerInfo.m_CommandResetCount[lpObj->AccountLevel];

	lpObj->AutoAddPointCount = 0;

	lpObj->AutoAddPointStats[0] = 0;

	lpObj->AutoAddPointStats[1] = 0;

	lpObj->AutoAddPointStats[2] = 0;

	lpObj->AutoAddPointStats[3] = 0;

	lpObj->AutoAddPointStats[4] = 0;

	if(gServerInfo.m_CommandResetClearQuest[lpObj->AccountLevel] != 0)
	{
		lpObj->DBClass = ((lpObj->DBClass/16)*16);
		lpObj->ChangeUp = lpObj->DBClass % 16;

		gObjectManager.CharacterMakePreviewCharSet(lpObj->Index);

		BYTE Class = (lpObj->ChangeUp*16);
		Class -= (Class/32);
		Class += (lpObj->Class*32);

		gQuest.GCQuestRewardSend(lpObj->Index,201,Class);

		memset(lpObj->Quest,0xFF,sizeof(lpObj->Quest));

		gQuest.GCQuestInfoSend(lpObj->Index);
	}

	if(gServerInfo.m_CommandResetClearSkill[lpObj->AccountLevel] != 0)
	{
		lpObj->MasterLevel = 0;

		lpObj->MasterPoint = 0;

		for(int n=0;n < MAX_SKILL_LIST;n++)
		{
			lpObj->Skill[n].Clear();
		}

		#if(GAMESERVER_UPDATE>=402)

		for(int n=0;n < MAX_MASTER_SKILL_LIST;n++)
		{
			lpObj->MasterSkill[n].Clear();
		}

		#endif

		gMasterSkillTree.GCMasterSkillListSend(lpObj->Index);

		gSkillManager.GCSkillListSend(lpObj,0);

		gObjectManager.CharacterCalcAttribute(lpObj->Index);

		gMasterSkillTree.GCMasterInfoSend(lpObj);
	}

	if(gServerInfo.m_CommandResetClearParty[lpObj->AccountLevel] != 0)
	{
		if(OBJECT_RANGE(lpObj->PartyNumber) != 0)
		{
			gParty.Destroy(lpObj->PartyNumber);
		}
	}

	if(gServerInfo.m_CommandResetType == 1)
	{
		int point = gResetTable.GetResetPoint(lpObj);

		point = (point*gServerInfo.m_CommandResetPointRate[lpObj->Class])/100;

		point += (lpObj->Level - 1)*gServerInfo.m_LevelUpPoint[lpObj->Class][lpObj->AccountLevel];

		point += ((gQuest.CheckQuestListState(lpObj,2,QUEST_FINISH) == 0) ? 0 : ((lpObj->Level>gServerInfo.m_PlusStatMinLevel) ? ((lpObj->Level - gServerInfo.m_PlusStatMinLevel)*gServerInfo.m_PlusStatPoint) : 0));

		point += gQuest.GetQuestRewardLevelUpPoint(lpObj);

		point += lpObj->FruitAddPoint;

		lpObj->LevelUpPoint = point;

		lpObj->Strength = (gServerInfo.m_CommandResetKeepStrength != 0) ? lpObj->Strength : gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Strength;
		lpObj->Dexterity = (gServerInfo.m_CommandResetKeepDexterity != 0) ? lpObj->Dexterity : gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Dexterity;
		lpObj->Vitality = (gServerInfo.m_CommandResetKeepVitality != 0) ? lpObj->Vitality : gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Vitality;
		lpObj->Energy = (gServerInfo.m_CommandResetKeepEnergy != 0) ? lpObj->Energy : gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Energy;
		lpObj->Leadership = (gServerInfo.m_CommandResetKeepLeadership != 0) ? lpObj->Leadership : gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Leadership;
	}

	gObjectManager.CharacterCalcAttribute(lpObj->Index);

	GCNewCharacterInfoSend(lpObj);

	GDCharacterInfoSaveSend(lpObj->Index);

	GDResetInfoSaveSend(lpObj->Index,lpMsg->ResetDay,lpMsg->ResetWek,lpMsg->ResetMon);

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(350),lpObj->Reset);

	if(gServerInfo.m_CommandResetMove[lpObj->AccountLevel] != 0)
	{
		switch (lpObj->Class)
		{
		case CLASS_DW:
			gObjMoveGate(lpObj->Index,17);
			break;
		case CLASS_DK:
			gObjMoveGate(lpObj->Index,17);
			break;
		case CLASS_FE:
			gObjMoveGate(lpObj->Index,27);
			break;
		case CLASS_MG:
			gObjMoveGate(lpObj->Index,17);
			break;
		case CLASS_DL:
			gObjMoveGate(lpObj->Index,17);
			break;
		case CLASS_SU:
			gObjMoveGate(lpObj->Index,267);
			break;
		case CLASS_RF:
			gObjMoveGate(lpObj->Index,17);
			break;
		}
	}

	lpObj->AutoAddPointStats[0] = lpObj->AutoResetStats[0];

	lpObj->AutoAddPointStats[1] = lpObj->AutoResetStats[1];

	lpObj->AutoAddPointStats[2] = lpObj->AutoResetStats[2];

	lpObj->AutoAddPointStats[3] = lpObj->AutoResetStats[3];

	lpObj->AutoAddPointStats[4] = lpObj->AutoResetStats[4];

	lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[0]>0) ? (lpObj->AutoAddPointCount+1) : lpObj->AutoAddPointCount);

	lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[1]>0) ? (lpObj->AutoAddPointCount+1) : lpObj->AutoAddPointCount);

	lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[2]>0) ? (lpObj->AutoAddPointCount+1) : lpObj->AutoAddPointCount);

	lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[3]>0) ? (lpObj->AutoAddPointCount+1) : lpObj->AutoAddPointCount);

	lpObj->AutoAddPointCount = ((lpObj->AutoAddPointStats[4]>0) ? (lpObj->AutoAddPointCount+1) : lpObj->AutoAddPointCount);

	this->CommandAddPointAutoProc(lpObj);

	gLog.Output(LOG_COMMAND,"[CommandReset][%s][%s] - (Reset: %d)",lpObj->Account,lpObj->Name,lpObj->Reset);
}

void CCommandManager::DGCommandMasterResetRecv(SDHP_COMMAND_MASTER_RESET_RECV* lpMsg) // OK
{
	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGCommandMasterResetRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	lpObj->CommandManagerTransaction[0] = 0;

	if(lpMsg->MasterResetDay >= ((DWORD)gServerInfo.m_CommandMasterResetLimitDay[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(360),gServerInfo.m_CommandMasterResetLimitDay[lpObj->AccountLevel]);
		return;
	}

	if(lpMsg->MasterResetWek >= ((DWORD)gServerInfo.m_CommandMasterResetLimitWek[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(361),gServerInfo.m_CommandMasterResetLimitWek[lpObj->AccountLevel]);
		return;
	}

	if(lpMsg->MasterResetMon >= ((DWORD)gServerInfo.m_CommandMasterResetLimitMon[lpObj->AccountLevel]))
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(362),gServerInfo.m_CommandMasterResetLimitMon[lpObj->AccountLevel]);
		return;
	}

	lpObj->Money -= gServerInfo.m_CommandMasterResetMoney[lpObj->AccountLevel];

	GCMoneySend(lpObj->Index,lpObj->Money);

	gObjCoinSub(lpObj->Index,gServerInfo.m_CommandMasterResetCoin1[lpObj->AccountLevel],gServerInfo.m_CommandMasterResetCoin2[lpObj->AccountLevel],gServerInfo.m_CommandMasterResetCoin3[lpObj->AccountLevel]);

	this->DiscountRequirement(lpObj,COMMAND_MASTER_RESET);

	lpObj->Level = ((gServerInfo.m_CommandMasterResetStartLevel[lpObj->AccountLevel] == -1) ? (lpObj->Level - gServerInfo.m_CommandMasterResetStartLevel[lpObj->AccountLevel]) : gServerInfo.m_CommandMasterResetStartLevel[lpObj->AccountLevel]);

	lpObj->Experience = gLevelExperience[lpObj->Level - 1];

	lpObj->Reset = ((gServerInfo.m_CommandMasterResetStartReset[lpObj->AccountLevel] == -1) ? (lpObj->Reset - gServerInfo.m_CommandMasterResetReset[lpObj->AccountLevel]) : gServerInfo.m_CommandMasterResetStartReset[lpObj->AccountLevel]);

	lpObj->MasterReset += gServerInfo.m_CommandMasterResetCount[lpObj->AccountLevel];

	lpMsg->MasterResetDay += gServerInfo.m_CommandMasterResetCount[lpObj->AccountLevel];

	lpMsg->MasterResetWek += gServerInfo.m_CommandMasterResetCount[lpObj->AccountLevel];

	lpMsg->MasterResetMon += gServerInfo.m_CommandMasterResetCount[lpObj->AccountLevel];

	if(gServerInfo.m_CommandMasterResetClearQuest[lpObj->AccountLevel] != 0)
	{
		lpObj->DBClass = ((lpObj->DBClass/16)*16);
		lpObj->ChangeUp = lpObj->DBClass % 16;

		gObjectManager.CharacterMakePreviewCharSet(lpObj->Index);

		BYTE Class = (lpObj->ChangeUp*16);
		Class -= (Class/32);
		Class += (lpObj->Class*32);

		gQuest.GCQuestRewardSend(lpObj->Index,201,Class);

		memset(lpObj->Quest,0xFF,sizeof(lpObj->Quest));

		gQuest.GCQuestInfoSend(lpObj->Index);
	}

	if(gServerInfo.m_CommandMasterResetClearSkill[lpObj->AccountLevel] != 0)
	{
		lpObj->MasterLevel = 0;

		lpObj->MasterPoint = 0;

		for(int n = 0; n < MAX_SKILL_LIST; n++)
		{
			lpObj->Skill[n].Clear();
		}

		#if(GAMESERVER_UPDATE>=402)

		for(int n = 0; n < MAX_MASTER_SKILL_LIST; n++)
		{
			lpObj->MasterSkill[n].Clear();
		}

		#endif

		gMasterSkillTree.GCMasterSkillListSend(lpObj->Index);

		gSkillManager.GCSkillListSend(lpObj,0);

		gObjectManager.CharacterCalcAttribute(lpObj->Index);

		gMasterSkillTree.GCMasterInfoSend(lpObj);
	}

	if(gServerInfo.m_CommandMasterResetClearParty[lpObj->AccountLevel] != 0)
	{
		if(OBJECT_RANGE(lpObj->PartyNumber) != 0)
		{
			gParty.Destroy(lpObj->PartyNumber);
		}
	}

	if(gServerInfo.m_CommandMasterResetType == 1)
	{
		int point = 0;

		point = gServerInfo.m_CommandMasterResetPoint[lpObj->AccountLevel]*lpObj->MasterReset;

		point = (point*gServerInfo.m_CommandMasterResetPointRate[lpObj->Class])/100;

		point += (lpObj->Level - 1)*gServerInfo.m_LevelUpPoint[lpObj->Class][lpObj->AccountLevel];

		point += ((gQuest.CheckQuestListState(lpObj,2,QUEST_FINISH) == 0) ? 0 : ((lpObj->Level>gServerInfo.m_PlusStatMinLevel) ? ((lpObj->Level - gServerInfo.m_PlusStatMinLevel)*gServerInfo.m_PlusStatPoint) : 0));

		point += gQuest.GetQuestRewardLevelUpPoint(lpObj);

		point += lpObj->FruitAddPoint;

		lpObj->LevelUpPoint = point;

		lpObj->Strength = (gServerInfo.m_CommandMasterResetKeepStrength != 0) ? lpObj->Strength : gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Strength;
		lpObj->Dexterity = (gServerInfo.m_CommandMasterResetKeepDexterity != 0) ? lpObj->Dexterity : gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Dexterity;
		lpObj->Vitality = (gServerInfo.m_CommandMasterResetKeepVitality != 0) ? lpObj->Vitality : gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Vitality;
		lpObj->Energy = (gServerInfo.m_CommandMasterResetKeepEnergy != 0) ? lpObj->Energy : gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Energy;
		lpObj->Leadership = (gServerInfo.m_CommandMasterResetKeepLeadership != 0) ? lpObj->Leadership : gDefaultClassInfo.m_DefaultClassInfo[lpObj->Class].Leadership;
	}

	gObjectManager.CharacterCalcAttribute(lpObj->Index);

	GCNewCharacterInfoSend(lpObj);

	GDCharacterInfoSaveSend(lpObj->Index);

	GDMasterResetInfoSaveSend(lpObj->Index,lpMsg->MasterResetDay,lpMsg->MasterResetWek,lpMsg->MasterResetMon);

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(363),lpObj->MasterReset);

	if(gServerInfo.m_CommandMasterResetMove[lpObj->AccountLevel] != 0)
	{
		switch (lpObj->Class)
		{
		case CLASS_DW:
			gObjMoveGate(lpObj->Index,17);
			break;
		case CLASS_DK:
			gObjMoveGate(lpObj->Index,17);
			break;
		case CLASS_FE:
			gObjMoveGate(lpObj->Index,27);
			break;
		case CLASS_MG:
			gObjMoveGate(lpObj->Index,17);
			break;
		case CLASS_DL:
			gObjMoveGate(lpObj->Index,17);
			break;
		case CLASS_SU:
			gObjMoveGate(lpObj->Index,267);
			break;
		case CLASS_RF:
			gObjMoveGate(lpObj->Index,17);
			break;
		}
	}

	gLog.Output(LOG_COMMAND,"[CommandMasterReset][%s][%s] - (MasterReset: %d)",lpObj->Account,lpObj->Name,lpObj->MasterReset);
}

void CCommandManager::DGCommandRenameRecv(SDHP_COMMAND_RENAME_RECV* lpMsg) // OK
{
	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGCommandRenameRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	if(lpMsg->result == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(693));
		return;
	}
	else if(lpMsg->result == 2)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(692));
		return;
	}

	memcpy(lpObj->Name,lpMsg->newname,sizeof(lpMsg->newname));

	GDCharacterInfoSaveSend(lpObj->Index);

	this->DiscountRequirement(lpObj,COMMAND_RENAME);

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(694));

	if(lpObj->CloseCount <= 0)
	{
		lpObj->CloseType = 1;
		lpObj->CloseCount = 6;
	}

	gLog.Output(LOG_COMMAND,"[CommandRename][%s][%s] - (New name: %s)",lpObj->Account,lpMsg->name,lpMsg->newname);
}

void CCommandManager::DGCommandBanAccountRecv(SDHP_COMMAND_BAN_ACCOUNT_RECV* lpMsg) // OK
{
	LPOBJ lpObj = &gObj[lpMsg->index];

	if(lpMsg->result != 0)
	{
		if(lpMsg->BlocCode == 0)
		{
			gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(687),lpMsg->account);

			gLog.Output(LOG_COMMAND,"[CommandUnbanAcc][%s][%s] - [%s]",lpObj->Account,lpObj->Name,lpMsg->account);

			this->DiscountRequirement(lpObj,COMMAND_UNBAN_ACC);
		}
	}
	else
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(695));
	}
}

void CCommandManager::DGCommandBanCharacterRecv(SDHP_COMMAND_BAN_CHARACTER_RECV* lpMsg) // OK
{
	LPOBJ lpObj = &gObj[lpMsg->index];

	if (lpMsg->result != 0)
	{
		if (lpMsg->CtlCode == 0)
		{
			gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(687),lpMsg->name);
			
			gLog.Output(LOG_COMMAND,"[CommandUnBanChar][%s][%s] - [%s]",lpObj->Account,lpObj->Name,lpMsg->name);

			this->DiscountRequirement(lpObj,COMMAND_UNBAN_CHAR);
		}
	}
	else
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(695));
	}
}