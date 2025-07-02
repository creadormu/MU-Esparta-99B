// ServerInfo.cpp: implementation of the CServerInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ServerInfo.h"
#include "380ItemOption.h"
#include "380ItemType.h"
#include "AcheronGuardian.h"
#include "ArcaBattle.h"
#include "BlackList.h"
#include "BloodCastle.h"
#include "BonusManager.h"
#include "CashShop.h"
#include "CastleDeep.h"
#include "CastleSiege.h"
#include "ChaosCastle.h"
#include "ChaosCastleFinal.h"
#include "CommandManager.h"
#include "ConnectMember.h"
#include "Crywolf.h"
#include "CrywolfSync.h"
#include "CustomArena.h"
#include "CustomAttack.h"
#include "CustomBuyVip.h"
#include "CustomComboSkill.h"
#include "CustomEventDrop.h"
#include "CustomGift.h"
#include "CustomHealthBar.h"
#include "CustomJewel.h"
#include "CustomJewelPack.h"
#include "CustomLukeHelper.h"
#include "CustomMarriage.h"
#include "CustomMix.h"
#include "CustomMonster.h"
#include "CustomMonsterKillCount.h"
#include "CustomPick.h"
#include "CustomQuest.h"
#include "CustomStore.h"
#include "CustomWing.h"
#include "DamageTable.h"
#include "DefaultClassFreebies.h"
#include "DefaultClassInfo.h"
#include "DevilSquare.h"
#include "DoubleGoer.h"
#include "DropEvent.h"
#include "EffectManager.h"
#include "EventEntryLevel.h"
#include "EventSpawnStage.h"
#include "ExcellentOptionRate.h"
#include "ExperienceTable.h"
#include "Filter.h"
#include "GameMaster.h"
#include "Gate.h"
#include "GoldenArcher.h"
#include "HackPacketCheck.h"
#include "HackSkillSpeedCheck.h"
#include "HelperOffline.h"
#include "IllusionTemple.h"
#include "ImperialGuardian.h"
#include "InvasionManager.h"
#include "ItemBagManager.h"
#include "ItemDrop.h"
#include "ItemLevel.h"
#include "ItemLoot.h"
#include "ItemMoneyDrop.h"
#include "ItemMove.h"
#include "ItemOption.h"
#include "ItemOptionRate.h"
#include "ItemStack.h"
#include "ItemValue.h"
#include "JewelOfHarmonyOption.h"
#include "JewelOfHarmonyType.h"
#include "Kalima.h"
#include "Kanturu.h"
#include "LuckyItem.h"
#include "MapManager.h"
#include "MapRateInfo.h"
#include "MapServerManager.h"
#include "MasterSkillTree.h"
#include "Message.h"
#include "MiniMap.h"
#include "MiningSystem.h"
#include "MonsterAIAutomata.h"
#include "MonsterAIElement.h"
#include "MonsterAIGroup.h"
#include "MonsterAIRule.h"
#include "MonsterAIUnit.h"
#include "MonsterManager.h"
#include "MonsterMove.h"
#include "MonsterSchedule.h"
#include "MonsterSetBase.h"
#include "MonsterSkillElement.h"
#include "MonsterSkillManager.h"
#include "MonsterSkillUnit.h"
#include "MossMerchant.h"
#include "Move.h"
#include "MoveSummon.h"
#include "MuCashShop.h"
#include "MuunSystem.h"
#include "Notice.h"
#include "NpcTalk.h"
#include "OnlineReward.h"
#include "PacketManager.h"
#include "Path.h"
#include "PcPoint.h"
#include "PentagramSystem.h"
#include "Quest.h"
#include "QuestObjective.h"
#include "QuestReward.h"
#include "QuestWorld.h"
#include "QuestWorldObjective.h"
#include "QuestWorldReward.h"
#include "Raklion.h"
#include "ResetLimit.h"
#include "ResetTable.h"
#include "ScriptLoader.h"
#include "SetItemOption.h"
#include "SetItemType.h"
#include "ShopManager.h"
#include "SkillDamage.h"
#include "SkillEnabler.h"
#include "SkillHitBox.h"
#include "SkillManager.h"
#include "SkillUseArea.h"
#include "SocketItemOption.h"
#include "SocketItemType.h"
#include "SummonScroll.h"
#include "Util.h"
#include "WingOption.h"

WORD CustomerLicenseId;

CServerInfo gServerInfo;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CServerInfo::CServerInfo() // OK
{

}

CServerInfo::~CServerInfo() // OK
{

}

void CServerInfo::ReadInit() // OK
{
	gPath.SetMainPath(MAIN_PATH);

	this->ReadBlackListInfo();

	this->ReadChaosMixInfo();

	this->ReadCharacterInfo();

	this->ReadCommandInfo();

	this->ReadCommonInfo();

	this->ReadMapServerInfo();

	this->ReadCustomInfo();

	this->ReadEventInfo();

	this->ReadEventItemBagInfo();

	this->ReadHackInfo();

	this->ReadItemInfo();

	this->ReadCashShopInfo();

	this->ReadMapInfo();

	this->ReadMonsterInfo();

	this->ReadMoveInfo();

	this->ReadQuestInfo();

	this->ReadScriptInfo();

	this->ReadShopInfo();

	this->ReadSkillInfo();

	this->ReadUtilInfo();
}

void CServerInfo::ReadBlackListInfo() // OK
{
	gBlackList.Load(gPath.GetFullPath("BlackList.txt"));
}

void CServerInfo::ReadCashShopInfo() // OK
{
	this->ReadCashShopInfo("GameServerInfo",".\\Data\\GameServerInfo - Common.dat");

	#if(GAMESERVER_SHOP==3)

	gCashShop.LoadPackage(gPath.GetFullPath("CashShop\\CashShopPackage.txt"));

	gCashShop.LoadProduct(gPath.GetFullPath("CashShop\\CashShopProduct.txt"));

	#endif

	#if(GAMESERVER_SHOP==2)

	gMuCashShop.Load(gPath.GetFullPath("CashShop.txt"));

	#endif
}

void CServerInfo::ReadChaosMixInfo() // OK
{
	this->ReadChaosMixInfo("GameServerInfo",".\\Data\\GameServerInfo - ChaosMix.dat");
}

void CServerInfo::ReadCharacterInfo() // OK
{
	this->ReadCharacterInfo("GameServerInfo",".\\Data\\GameServerInfo - Character.dat");

	gDefaultClassFreebies.Load(gPath.GetFullPath("Character\\DefaultClassFreebies.txt"));

	gDefaultClassInfo.Load(gPath.GetFullPath("Character\\DefaultClassInfo.txt"));
}

void CServerInfo::ReadCommandInfo() // OK
{
	this->ReadCommandInfo("GameServerInfo",".\\Data\\GameServerInfo - Command.dat");

	gCommandManager.Load(gPath.GetFullPath("CommandManager.txt"));
}

void CServerInfo::ReadCommonInfo() // OK
{
	this->ReadCommonInfo("GameServerInfo",".\\Data\\GameServerInfo - Common.dat");

	gConnectMember.Load(gPath.GetFullPath("ConnectMember.txt"));

	gMapManager.Load(gPath.GetFullPath("MapManager.txt"));

	gMapRateInfo.Load(gPath.GetFullPath("MapRateInfo.txt"));

	gMessage.Load(gPath.GetFullPath("Message.txt"));

	#if(GAMESERVER_UPDATE>=802)

	gMiniMap.Load(gPath.GetFullPath("MiniMap.txt"));

	#endif

	#if(GAMESERVER_UPDATE>=801)

	gMiningSystem.Load(gPath.GetFullPath("Mining.txt"));

	#endif
}

void CServerInfo::ReadCustomInfo() // OK
{
	gCustomArena.ReadCustomArenaInfo("GameServerInfo",".\\Data\\GameServerInfo - Custom.dat");

	gCustomAttack.ReadCustomAttackInfo("GameServerInfo",".\\Data\\GameServerInfo - Custom.dat");

	gCustomEventDrop.ReadCustomEventDropInfo("GameServerInfo",".\\Data\\GameServerInfo - Custom.dat");

	gCustomGift.ReadCustomGiftInfo("GameServerInfo",".\\Data\\GameServerInfo - Custom.dat");

	gCustomJewelPack.ReadCustomJewelPackInfo("GameServerInfo",".\\Data\\GameServerInfo - Custom.dat");

	gCustomLukeHelper.ReadCustomLukeHelperInfo("GameServerInfo",".\\Data\\GameServerInfo - Custom.dat");

	gCustomMarriage.ReadCustomMarriageInfo("GameServerInfo",".\\Data\\GameServerInfo - Custom.dat");

	gCustomPick.ReadCustomPickInfo("GameServerInfo",".\\Data\\GameServerInfo - Custom.dat");

	gCustomStore.ReadCustomStoreInfo("GameServerInfo",".\\Data\\GameServerInfo - Custom.dat");

	gCustomArena.Load(gPath.GetFullPath("Custom\\CustomArena.txt"));

	gCustomAttack.Load(gPath.GetFullPath("Custom\\CustomAttack.txt"));

	gCustomBuyVip.Load(gPath.GetFullPath("Custom\\CustomBuyVip.txt"));

	gCustomEventDrop.Load(gPath.GetFullPath("Custom\\CustomEventDrop.txt"));

	gCustomGift.Load(gPath.GetFullPath("Custom\\CustomGift.txt"));

	gCustomHealthBar.Load(gPath.GetFullPath("Custom\\CustomHealthBar.txt"));

	gCustomJewel.Load(gPath.GetFullPath("Custom\\CustomJewel.txt"));

	gCustomLukeHelper.Load(gPath.GetFullPath("Custom\\CustomLukeHelper.txt"));

	gCustomMarriage.Load(gPath.GetFullPath("Custom\\CustomMarriage.txt"));

	#if(GAMESERVER_UPDATE>=301)

	gCustomMix.Load(gPath.GetFullPath("Custom\\CustomMix.txt"));

	#endif

	gCustomMonster.Load(gPath.GetFullPath("Custom\\CustomMonster.txt"));

	gCustomMonsterKillCount.Load(gPath.GetFullPath("Custom\\CustomMonsterKillCount.txt"));

	gCustomPick.Load(gPath.GetFullPath("Custom\\CustomPick.txt"));

	//gCustomQuest.Load(gPath.GetFullPath("Custom\\CustomQuest.txt"));

	gCustomWing.Load(gPath.GetFullPath("Custom\\CustomWing.txt"));
}

void CServerInfo::ReadEventInfo() // OK
{
	this->ReadEventInfo("GameServerInfo",".\\Data\\GameServerInfo - Event.dat");

	#if(GAMESERVER_UPDATE>=801)

	gAcheronGuardian.Load(gPath.GetFullPath("Event\\AcheronGuardian.dat"));

	#endif

	#if(GAMESERVER_UPDATE>=702)

	gArcaBattle.Load(gPath.GetFullPath("Event\\ArcaBattle.dat"));

	#endif

	gBloodCastle.Load(gPath.GetFullPath("Event\\BloodCastle.dat"));

	gBonusManager.Load(gPath.GetFullPath("Event\\BonusManager.dat"));

	gCastleDeep.Load(gPath.GetFullPath("Event\\CastleDeepEvent.dat"));

	if(this->m_ServerType == 1 && gCastleSiege.Ready(gMapServerManager.GetMapServerGroup()) != 0)
	{
		if(gCastleSiege.LoadData(gPath.GetFullPath("Event\\CastleSiege.dat")) != 0)
		{
			gCastleSiege.LoadPreFixData(".\\Data\\GameServerInfo - Event.dat");
			gCastleSiege.SetDataLoadState(CASTLESIEGE_DATALOAD_2);
		}
	}

	gChaosCastle.Load(gPath.GetFullPath("Event\\ChaosCastle.dat"));

	#if(GAMESERVER_UPDATE>=803)
	
	gChaosCastleFinal.Load(gPath.GetFullPath("Event\\ChaosCastleSurvival.dat"));
	
	#endif

	#if(GAMESERVER_UPDATE>=201)

	gCrywolf.Load(gPath.GetFullPath("Event\\Crywolf.dat"));

	#endif

	gDevilSquare.Load(gPath.GetFullPath("Event\\DevilSquare.dat"));

	#if(GAMESERVER_UPDATE>=501)

	gDoubleGoer.Load(gPath.GetFullPath("Event\\DoubleGoer.dat"));

	#endif

	gDropEvent.Load(gPath.GetFullPath("Event\\DropEvent.dat"));

	gEventEntryLevel.Load(gPath.GetFullPath("Event\\EventEntryLevel.dat"));

	gEventSpawnStage.Load(gPath.GetFullPath("Event\\EventStageSpawn.dat"));

	#if(GAMESERVER_UPDATE>=301 && GAMESERVER_UPDATE<=603)

	gIllusionTemple.Load(gPath.GetFullPath("Event\\IllusionTemple.dat"));

	#endif

	#if(GAMESERVER_UPDATE>=501)

	gImperialGuardian.Load(gPath.GetFullPath("Event\\ImperialGuardian.dat"));

	#endif

	gInvasionManager.Load(gPath.GetFullPath("Event\\InvasionManager.dat"));

	gKalima.Load(gPath.GetFullPath("Event\\Kalima.dat"));

	#if(GAMESERVER_UPDATE>=201)

	gKanturu.Load(gPath.GetFullPath("Event\\Kanturu.dat"));

	#endif

	#if(GAMESERVER_UPDATE>=402)

	gMossMerchant.Load(gPath.GetFullPath("Event\\MossMerchant.dat"));

	#endif

	#if(GAMESERVER_UPDATE>=401)

	gRaklion.Load(gPath.GetFullPath("Event\\Raklion.dat"));

	#endif

	#if(GAMESERVER_UPDATE>=801)

	gSummonScroll.Load(gPath.GetFullPath("Event\\SummonScroll.dat"));

	#endif
}

void CServerInfo::ReadEventItemBagInfo() // OK
{
	gItemBagManager.Load(gPath.GetFullPath("EventItemBagManager.txt"));

	gItemBagManager.LoadEventItemBag();
}

void CServerInfo::ReadHackInfo() // OK
{
	this->ReadHackInfo("GameServerInfo",".\\Data\\GameServerInfo - Common.dat");
	
	gHackPacketCheck.Load(gPath.GetFullPath("Hack\\HackPacketCheck.txt"));

	gHackSkillSpeedCheck.Load(gPath.GetFullPath("Hack\\HackSkillSpeedCheck.txt"));

	gPacketManager.LoadEncryptionKey(gPath.GetFullPath("Hack\\Enc2.dat"));

	gPacketManager.LoadDecryptionKey(gPath.GetFullPath("Hack\\Dec1.dat"));
}

void CServerInfo::ReadItemInfo() // OK
{
	this->ReadItemInfo("GameServerInfo",".\\Data\\GameServerInfo - Item.dat");

	gItemManager.Load(gPath.GetFullPath("Item\\Item.txt"));

	#if(GAMESERVER_UPDATE>=201)

	g380ItemOption.Load(gPath.GetFullPath("Item\\380ItemOption.txt"));

	g380ItemType.Load(gPath.GetFullPath("Item\\380ItemType.txt"));

	#endif

	gExcellentOptionRate.Load(gPath.GetFullPath("Item\\ExcellentOptionRate.txt"));

	gItemDrop.Load(gPath.GetFullPath("Item\\ItemDrop.txt"));

	gItemLevel.Load(gPath.GetFullPath("Item\\ItemLevel.txt"));

	gItemLoot.Load(gPath.GetFullPath("Item\\ItemLoot.txt"));

	gItemMoneyDrop.Load(gPath.GetFullPath("Item\\ItemMoneyDrop.txt"));

	gItemMove.Load(gPath.GetFullPath("Item\\ItemMove.txt"));

	gItemOption.Load(gPath.GetFullPath("Item\\ItemOption.txt"));

	gItemOptionRate.Load(gPath.GetFullPath("Item\\ItemOptionRate.txt"));

	gItemStack.Load(gPath.GetFullPath("Item\\ItemStack.txt"));

	gItemValue.Load(gPath.GetFullPath("Item\\ItemValue.txt"));

	#if(GAMESERVER_UPDATE>=201)

	gJewelOfHarmonyOption.Load(gPath.GetFullPath("Item\\JewelOfHarmonyOption.txt"));

	gJewelOfHarmonyType.Load(gPath.GetFullPath("Item\\JewelOfHarmonyType.txt"));

	#endif

	#if(GAMESERVER_UPDATE>=602)

	gLuckyItem.Load(gPath.GetFullPath("Item\\LuckyItem.txt"));

	#endif

	#if(GAMESERVER_UPDATE>=803)

	gMuunSystem.Load(gPath.GetFullPath("Item\\MuunOption.txt"));

	#endif

	#if(GAMESERVER_UPDATE>=701)

	gPentagramSystem.Load(gPath.GetFullPath("Item\\PentagramOption.txt"));

	gPentagramSystem.LoadJewel(gPath.GetFullPath("Item\\PentagramJewelOption.txt"));

	gPentagramSystem.LoadMixRate(gPath.GetFullPath("Item\\PentagramJewelMixRate.txt"));

	#endif

	gSetItemOption.Load(gPath.GetFullPath("Item\\SetItemOption.txt"));

	gSetItemType.Load(gPath.GetFullPath("Item\\SetItemType.txt"));

	#if(GAMESERVER_UPDATE>=401)

	gSocketItemOption.Load(gPath.GetFullPath("Item\\SocketItemOption.txt"));

	gSocketItemType.Load(gPath.GetFullPath("Item\\SocketItemType.txt"));

	#endif

	gWingOption.Load(gPath.GetFullPath("Item\\WingOption.txt"));
}

void CServerInfo::ReadMapInfo() // OK
{
	FILE* file;

	char path[256] = {0};

	char temp[256] = {0};

	for(int n=0;n < MAX_MAP;n++)
	{
		wsprintf(temp,"Terrain\\terrain%d.att",(n+1));

		if(fopen_s(&file,gPath.GetFullPath(temp),"r") == 0)
		{
			strcpy_s(path,temp);
			fclose(file);
		}

		gMap[n].Load(gPath.GetFullPath(path),n);
	}	

	#if(GAMESERVER_UPDATE>=201)

	gCrywolf.LoadCrywolfMapAttr(gPath.GetFullPath("Terrain\\terrain35_PEACE.att"),0);

	gCrywolf.LoadCrywolfMapAttr(gPath.GetFullPath("Terrain\\terrain35_OCCUPIED.att"),1);

	gCrywolf.LoadCrywolfMapAttr(gPath.GetFullPath("Terrain\\terrain35_WAR.att"),2);

	gKanturu.LoadKanturuMapAttr(gPath.GetFullPath("Terrain\\terrain40_CLOSE.att"),0);

	gKanturu.LoadKanturuMapAttr(gPath.GetFullPath("Terrain\\terrain40_OPEN.att"),1);

	#endif

	#if(GAMESERVER_UPDATE>=702)

	gAcheronGuardian.LoadAcheronMapAttr(gPath.GetFullPath("Terrain\\terrain93_CLOSE.att"),0,0);

	gAcheronGuardian.LoadAcheronMapAttr(gPath.GetFullPath("Terrain\\terrain93_OPEN.att"),0,1);

	gAcheronGuardian.LoadAcheronMapAttr(gPath.GetFullPath("Terrain\\terrain97_CLOSE.att"),1,0);

	gAcheronGuardian.LoadAcheronMapAttr(gPath.GetFullPath("Terrain\\terrain97_OPEN.att"),1,1);

	#endif
}

void CServerInfo::ReadMapServerInfo() // OK
{
	gMapServerManager.Load(gPath.GetFullPath("MapServerInfo.txt"));
}

void CServerInfo::ReadMonsterInfo() // OK
{
	gMonsterAIAutomata.Load(gPath.GetFullPath("Monster\\AI\\MonsterAIAutomata.txt"));

	CMonsterAIElement::LoadData(gPath.GetFullPath("Monster\\AI\\MonsterAIElement.txt"));

	CMonsterAIElement::m_MonsterAIMovePath[MAP_CRYWOLF].LoadData(gPath.GetFullPath("Monster\\AI\\MonsterAIMovePath.txt"));

	CMonsterAIGroup::LoadData(gPath.GetFullPath("Monster\\AI\\MonsterAIGroup.txt"));

	CMonsterAIRule::LoadData(gPath.GetFullPath("Monster\\AI\\MonsterAIRule.txt"));

	CMonsterAIUnit::LoadData(gPath.GetFullPath("Monster\\AI\\MonsterAIUnit.txt"));

	gMonsterManager.Load(gPath.GetFullPath("Monster\\MonsterList.txt"));

	gMonsterMove.Load(gPath.GetFullPath("Monster\\MonsterMove.txt"));

	gMonsterSchedule.Load(gPath.GetFullPath("Monster\\MonsterSchedule.txt"));

	gMonsterSchedule.Init();

	gMonsterSetBase.LoadSpawn();

	gShopManager.ReloadShop();

	gMonsterSkillElement.Load(gPath.GetFullPath("Monster\\Skill\\MonsterSkillElement.txt"));

	gMonsterSkillUnit.Load(gPath.GetFullPath("Monster\\Skill\\MonsterSkillUnit.txt"));

	gMonsterSkillManager.Load(gPath.GetFullPath("Monster\\Skill\\MonsterSkill.txt"));

	gNpcTalk.Load(gPath.GetFullPath("Monster\\NpcTalk.txt"));
}

void CServerInfo::ReadMoveInfo() // OK
{
	gGate.Load(gPath.GetFullPath("Move\\Gate.txt"));

	gMove.Load(gPath.GetFullPath("Move\\Move.txt"));

	gMoveSummon.Load(gPath.GetFullPath("Move\\MoveSummon.txt"));
}

void CServerInfo::ReadQuestInfo() // OK
{
	gQuest.Load(gPath.GetFullPath("Quest\\Quest.txt"));

	gQuestObjective.Load(gPath.GetFullPath("Quest\\QuestObjective.txt"));

	gQuestReward.Load(gPath.GetFullPath("Quest\\QuestReward.txt"));

	#if(GAMESERVER_UPDATE>=501)

	gQuestWorld.Load(gPath.GetFullPath("QuestWorld\\QuestWorld.txt"));

	gQuestWorldObjective.Load(gPath.GetFullPath("QuestWorld\\QuestWorldObjective.txt"));

	gQuestWorldReward.Load(gPath.GetFullPath("QuestWorld\\QuestWorldReward.txt"));

	#endif
}

void CServerInfo::ReadScriptInfo() // OK
{
	gScriptLoader.Load(gPath.GetFullPath("Script\\ScriptMain.lua"));

	gScriptLoader.OnShutScript();

	gScriptLoader.OnReadScript();
}

void CServerInfo::ReadShopInfo() // OK
{
	#if(GAMESERVER_SHOP==1)

	gPcPoint.Load(gPath.GetFullPath("PcPoint.txt"));

	gPcPoint.ReloadShopInterface();

	#endif

	gShopManager.Load(gPath.GetFullPath("ShopManager.txt"));

	gShopManager.ReloadShop();

	gShopManager.ReloadShopInterface();
}

void CServerInfo::ReadSkillInfo() // OK
{
	this->ReadSkillInfo("GameServerInfo",".\\Data\\GameServerInfo - Skill.dat");

	gCustomComboSkill.Load(gPath.GetFullPath("Skill\\ComboSkill.txt"));

	gEffectManager.Load(gPath.GetFullPath("Skill\\EffectList.txt"));

	gHelperOffline.Load(gPath.GetFullPath("Skill\\HelperSkill.txt"));

	#if(GAMESERVER_UPDATE>=401)

	gMasterSkillTree.Load(gPath.GetFullPath("Skill\\MasterSkillTree.txt"));

	#endif

	gSkillDamage.Load(gPath.GetFullPath("Skill\\SkillDamage.txt"));

	gSkillHitBox.Load(gPath.GetFullPath("Skill\\SkillElect.hit"));

	gSkillEnabler.Load(gPath.GetFullPath("Skill\\SkillEnabler.txt"));

	gSkillManager.Load(gPath.GetFullPath("Skill\\SkillList.txt"));

	gSkillUseArea.Load(gPath.GetFullPath("Skill\\SkillUseArea.txt"));
}

void CServerInfo::ReadUtilInfo() // OK
{
	gDamageTable.Load(gPath.GetFullPath("Util\\DamageTable.txt"));

	gExperienceTable.Load(gPath.GetFullPath("Util\\ExperienceTable.txt"));

	gFilter.Load(gPath.GetFullPath("Util\\Filter.txt"));

	gGameMaster.Load(gPath.GetFullPath("Util\\GameMaster.txt"));

	gNotice.Load(gPath.GetFullPath("Util\\Notice.txt"));

	#if(GAMESERVER_SHOP>=1)

	gOnlineReward.Load(gPath.GetFullPath("Util\\OnlineReward.txt"));

	#endif

	gResetLimit.Load(gPath.GetFullPath("Util\\ResetLimit.txt"));

	gResetTable.Load(gPath.GetFullPath("Util\\ResetTable.txt"));
}

void CServerInfo::WriteItemInfo() // OK
{
	gItemManager.GetItemBmd(gPath.GetFullPath("Item\\Item.bmd"));
}

void CServerInfo::ReloadMonsterInfo() // OK
{
	for(int n=0;n < MAX_OBJECT_MONSTER;n++)
	{
		if(gObj[n].Type != OBJECT_MONSTER && gObj[n].Type != OBJECT_NPC)
		{
			continue;
		}

		if(gObj[n].CsNpcType != 0)
		{
			continue;
		}

		gObjDel(n);
	}

	gObjMonCount = 0;

	gCrywolf.m_ObjCommonNPC.Reset();

	gCrywolf.m_ObjSpecialNPC.Reset();

	gCrywolf.m_ObjCommonMonster.Reset();

	gCrywolf.m_ObjSpecialMonster.Reset();

	this->ReadMonsterInfo();

	gMonsterManager.SetMonsterData();
}

//->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

void CServerInfo::ReadStartupInfo(char* section,char* path) // OK
{
	char buff[256] = {0};

	CustomerLicenseId = GetPrivateProfileInt(section,"CustomerLicenseId",0,path);

	GetPrivateProfileString(section,"ServerName","",this->m_ServerName,sizeof(this->m_ServerName),path);

	this->m_ServerCode = GetPrivateProfileInt(section,"ServerCode",0,path);

	this->m_ServerType = GetPrivateProfileInt(section,"ServerType",0,path);

	this->m_ServerLock = GetPrivateProfileInt(section,"ServerLock",0,path);

	this->m_ServerPort = GetPrivateProfileInt(section,"ServerPort",0,path);

	wsprintf(this->m_ServerMutex,"SSeMU_MUTEX_%d",this->m_ServerCode);

	GetPrivateProfileString(section,"ServerVersion","",buff,sizeof(buff),path);

	this->m_ServerVersion[0] = buff[0];

	this->m_ServerVersion[1] = buff[2];

	this->m_ServerVersion[2] = buff[3];

	this->m_ServerVersion[3] = buff[5];

	this->m_ServerVersion[4] = buff[6];

	this->m_ServerVersion[5] = 0;

	GetPrivateProfileString(section,"ServerSerial","",this->m_ServerSerial,sizeof(this->m_ServerSerial),path);

	this->m_ServerMaxUserNumber = GetPrivateProfileInt(section,"ServerMaxUserNumber",0,path);

	this->m_ServerMaxUserNumber = ((this->m_ServerMaxUserNumber>gGameServerMaxAccount[4]) ? gGameServerMaxAccount[4] : this->m_ServerMaxUserNumber);

	this->m_ServerEncDecKey1 = GetPrivateProfileInt(section,"ServerEncDecKey1",0,path);

	this->m_ServerEncDecKey2 = GetPrivateProfileInt(section,"ServerEncDecKey2",0,path);

	GetPrivateProfileString(section,"DataServerAddress","",this->m_DataServerAddress,sizeof(this->m_DataServerAddress),path);
	this->m_DataServerPort = GetPrivateProfileInt(section,"DataServerPort",0,path);

	GetPrivateProfileString(section,"JoinServerAddress","",this->m_JoinServerAddress,sizeof(this->m_JoinServerAddress),path);
	this->m_JoinServerPort = GetPrivateProfileInt(section,"JoinServerPort",0,path);

	GetPrivateProfileString(section,"ConnectServerAddress","",this->m_ConnectServerAddress,sizeof(this->m_ConnectServerAddress),path);
	this->m_ConnectServerPort = GetPrivateProfileInt(section,"ConnectServerPort",0,path);

	this->m_WriteChatLog = GetPrivateProfileInt(section,"WriteChatLog",0,path);
	this->m_WriteCommandLog = GetPrivateProfileInt(section,"WriteCommandLog",0,path);
	this->m_WriteTradeLog = GetPrivateProfileInt(section,"WriteTradeLog",0,path);
	this->m_WriteConnectLog = GetPrivateProfileInt(section,"WriteConnectLog",0,path);
	this->m_WriteHackLog = GetPrivateProfileInt(section,"WriteHackLog",0,path);
	this->m_WriteCashShopLog = GetPrivateProfileInt(section,"WriteCashShopLog",0,path);
	this->m_WriteChaosMixLog = GetPrivateProfileInt(section,"WriteChaosMixLog",0,path);
	this->m_WriteScriptLog = GetPrivateProfileInt(section,"WriteScriptLog",0,path);

	this->m_ExperienceMultiplierConstA = GetPrivateProfileInt(section,"ExperienceMultiplierConstA",0,path);
	this->m_ExperienceMultiplierConstB = GetPrivateProfileInt(section,"ExperienceMultiplierConstB",0,path);
	this->m_PetExperienceMultiplierConstA = GetPrivateProfileInt(section,"PetExperienceMultiplierConstA",0,path);

	this->m_MaxLevel = GetPrivateProfileInt(section,"MaxLevel",0,path);
	this->m_MaxLevel = ((this->m_MaxLevel > MAX_CHARACTER_LEVEL) ? MAX_CHARACTER_LEVEL : this->m_MaxLevel);

	this->m_PetMaxLevel = GetPrivateProfileInt(section,"MaxPetLevel",0,path);
	this->m_PetMaxLevel = ((this->m_PetMaxLevel > MAX_CHARACTER_LEVEL) ? MAX_CHARACTER_LEVEL : this->m_PetMaxLevel);
}

void CServerInfo::ReadCashShopInfo(char* section,char* path) // OK
{
	#if(GAMESERVER_UPDATE>=501)

	this->m_CashShopSwitch = GetPrivateProfileInt(section,"CashShopSwitch",0,path);

	this->m_CashShopScriptVersion1 = GetPrivateProfileInt(section,"CashShopScriptVersion1",0,path);
	this->m_CashShopScriptVersion2 = GetPrivateProfileInt(section,"CashShopScriptVersion2",0,path);
	this->m_CashShopScriptVersion3 = GetPrivateProfileInt(section,"CashShopScriptVersion3",0,path);

	this->m_CashShopBannerVersion1 = GetPrivateProfileInt(section,"CashShopBannerVersion1",0,path);
	this->m_CashShopBannerVersion2 = GetPrivateProfileInt(section,"CashShopBannerVersion2",0,path);
	this->m_CashShopBannerVersion3 = GetPrivateProfileInt(section,"CashShopBannerVersion3",0,path);

	#endif
}

void CServerInfo::ReadChaosMixInfo(char* section,char* path) // OK
{
	this->m_ChaosItemMixRate[0] = GetPrivateProfileInt(section,"ChaosItemMixRate_AL0",0,path);
	this->m_ChaosItemMixRate[1] = GetPrivateProfileInt(section,"ChaosItemMixRate_AL1",0,path);
	this->m_ChaosItemMixRate[2] = GetPrivateProfileInt(section,"ChaosItemMixRate_AL2",0,path);
	this->m_ChaosItemMixRate[3] = GetPrivateProfileInt(section,"ChaosItemMixRate_AL3",0,path);

	this->m_DevilSquareMixRate1[0] = GetPrivateProfileInt(section,"DevilSquareMixRate1_AL0",0,path);
	this->m_DevilSquareMixRate1[1] = GetPrivateProfileInt(section,"DevilSquareMixRate1_AL1",0,path);
	this->m_DevilSquareMixRate1[2] = GetPrivateProfileInt(section,"DevilSquareMixRate1_AL2",0,path);
	this->m_DevilSquareMixRate1[3] = GetPrivateProfileInt(section,"DevilSquareMixRate1_AL3",0,path);

	this->m_DevilSquareMixRate2[0] = GetPrivateProfileInt(section,"DevilSquareMixRate2_AL0",0,path);
	this->m_DevilSquareMixRate2[1] = GetPrivateProfileInt(section,"DevilSquareMixRate2_AL1",0,path);
	this->m_DevilSquareMixRate2[2] = GetPrivateProfileInt(section,"DevilSquareMixRate2_AL2",0,path);
	this->m_DevilSquareMixRate2[3] = GetPrivateProfileInt(section,"DevilSquareMixRate2_AL3",0,path);
	
	this->m_DevilSquareMixRate3[0] = GetPrivateProfileInt(section,"DevilSquareMixRate3_AL0",0,path);
	this->m_DevilSquareMixRate3[1] = GetPrivateProfileInt(section,"DevilSquareMixRate3_AL1",0,path);
	this->m_DevilSquareMixRate3[2] = GetPrivateProfileInt(section,"DevilSquareMixRate3_AL2",0,path);
	this->m_DevilSquareMixRate3[3] = GetPrivateProfileInt(section,"DevilSquareMixRate3_AL3",0,path);

	this->m_DevilSquareMixRate4[0] = GetPrivateProfileInt(section,"DevilSquareMixRate4_AL0",0,path);
	this->m_DevilSquareMixRate4[1] = GetPrivateProfileInt(section,"DevilSquareMixRate4_AL1",0,path);
	this->m_DevilSquareMixRate4[2] = GetPrivateProfileInt(section,"DevilSquareMixRate4_AL2",0,path);
	this->m_DevilSquareMixRate4[3] = GetPrivateProfileInt(section,"DevilSquareMixRate4_AL3",0,path);

	this->m_DevilSquareMixRate5[0] = GetPrivateProfileInt(section,"DevilSquareMixRate5_AL0",0,path);
	this->m_DevilSquareMixRate5[1] = GetPrivateProfileInt(section,"DevilSquareMixRate5_AL1",0,path);
	this->m_DevilSquareMixRate5[2] = GetPrivateProfileInt(section,"DevilSquareMixRate5_AL2",0,path);
	this->m_DevilSquareMixRate5[3] = GetPrivateProfileInt(section,"DevilSquareMixRate5_AL3",0,path);

	this->m_DevilSquareMixRate6[0] = GetPrivateProfileInt(section,"DevilSquareMixRate6_AL0",0,path);
	this->m_DevilSquareMixRate6[1] = GetPrivateProfileInt(section,"DevilSquareMixRate6_AL1",0,path);
	this->m_DevilSquareMixRate6[2] = GetPrivateProfileInt(section,"DevilSquareMixRate6_AL2",0,path);
	this->m_DevilSquareMixRate6[3] = GetPrivateProfileInt(section,"DevilSquareMixRate6_AL3",0,path);

	#if(GAMESERVER_UPDATE>=401)

	this->m_DevilSquareMixRate7[0] = GetPrivateProfileInt(section,"DevilSquareMixRate7_AL0",0,path);
	this->m_DevilSquareMixRate7[1] = GetPrivateProfileInt(section,"DevilSquareMixRate7_AL1",0,path);
	this->m_DevilSquareMixRate7[2] = GetPrivateProfileInt(section,"DevilSquareMixRate7_AL2",0,path);
	this->m_DevilSquareMixRate7[3] = GetPrivateProfileInt(section,"DevilSquareMixRate7_AL3",0,path);

	#endif

	this->m_PlusCommonItemLevelMixRate[0][0] = GetPrivateProfileInt(section,"PlusCommonItemLevelMixRate1_AL0",0,path);
	this->m_PlusCommonItemLevelMixRate[0][1] = GetPrivateProfileInt(section,"PlusCommonItemLevelMixRate1_AL1",0,path);
	this->m_PlusCommonItemLevelMixRate[0][2] = GetPrivateProfileInt(section,"PlusCommonItemLevelMixRate1_AL2",0,path);
	this->m_PlusCommonItemLevelMixRate[0][3] = GetPrivateProfileInt(section,"PlusCommonItemLevelMixRate1_AL3",0,path);

	this->m_PlusExcSetItemLevelMixRate[0][0] = GetPrivateProfileInt(section,"PlusExcSetItemLevelMixRate1_AL0",0,path);
	this->m_PlusExcSetItemLevelMixRate[0][1] = GetPrivateProfileInt(section,"PlusExcSetItemLevelMixRate1_AL1",0,path);
	this->m_PlusExcSetItemLevelMixRate[0][2] = GetPrivateProfileInt(section,"PlusExcSetItemLevelMixRate1_AL2",0,path);
	this->m_PlusExcSetItemLevelMixRate[0][3] = GetPrivateProfileInt(section,"PlusExcSetItemLevelMixRate1_AL3",0,path);

	#if(GAMESERVER_UPDATE>=401)

	this->m_PlusSocketItemLevelMixRate[0][0] = GetPrivateProfileInt(section,"PlusSocketItemLevelMixRate1_AL0",0,path);
	this->m_PlusSocketItemLevelMixRate[0][1] = GetPrivateProfileInt(section,"PlusSocketItemLevelMixRate1_AL1",0,path);
	this->m_PlusSocketItemLevelMixRate[0][2] = GetPrivateProfileInt(section,"PlusSocketItemLevelMixRate1_AL2",0,path);
	this->m_PlusSocketItemLevelMixRate[0][3] = GetPrivateProfileInt(section,"PlusSocketItemLevelMixRate1_AL3",0,path);

	#endif

	this->m_PlusCommonItemLevelMixRate[1][0] = GetPrivateProfileInt(section,"PlusCommonItemLevelMixRate2_AL0",0,path);
	this->m_PlusCommonItemLevelMixRate[1][1] = GetPrivateProfileInt(section,"PlusCommonItemLevelMixRate2_AL1",0,path);
	this->m_PlusCommonItemLevelMixRate[1][2] = GetPrivateProfileInt(section,"PlusCommonItemLevelMixRate2_AL2",0,path);
	this->m_PlusCommonItemLevelMixRate[1][3] = GetPrivateProfileInt(section,"PlusCommonItemLevelMixRate2_AL3",0,path);

	this->m_PlusExcSetItemLevelMixRate[1][0] = GetPrivateProfileInt(section,"PlusExcSetItemLevelMixRate2_AL0",0,path);
	this->m_PlusExcSetItemLevelMixRate[1][1] = GetPrivateProfileInt(section,"PlusExcSetItemLevelMixRate2_AL1",0,path);
	this->m_PlusExcSetItemLevelMixRate[1][2] = GetPrivateProfileInt(section,"PlusExcSetItemLevelMixRate2_AL2",0,path);
	this->m_PlusExcSetItemLevelMixRate[1][3] = GetPrivateProfileInt(section,"PlusExcSetItemLevelMixRate2_AL3",0,path);

	#if(GAMESERVER_UPDATE>=401)

	this->m_PlusSocketItemLevelMixRate[1][0] = GetPrivateProfileInt(section,"PlusSocketItemLevelMixRate2_AL0",0,path);
	this->m_PlusSocketItemLevelMixRate[1][1] = GetPrivateProfileInt(section,"PlusSocketItemLevelMixRate2_AL1",0,path);
	this->m_PlusSocketItemLevelMixRate[1][2] = GetPrivateProfileInt(section,"PlusSocketItemLevelMixRate2_AL2",0,path);
	this->m_PlusSocketItemLevelMixRate[1][3] = GetPrivateProfileInt(section,"PlusSocketItemLevelMixRate2_AL3",0,path);

	#endif

	this->m_PlusCommonItemLevelMixRate[2][0] = GetPrivateProfileInt(section,"PlusCommonItemLevelMixRate3_AL0",0,path);
	this->m_PlusCommonItemLevelMixRate[2][1] = GetPrivateProfileInt(section,"PlusCommonItemLevelMixRate3_AL1",0,path);
	this->m_PlusCommonItemLevelMixRate[2][2] = GetPrivateProfileInt(section,"PlusCommonItemLevelMixRate3_AL2",0,path);
	this->m_PlusCommonItemLevelMixRate[2][3] = GetPrivateProfileInt(section,"PlusCommonItemLevelMixRate3_AL3",0,path);

	this->m_PlusExcSetItemLevelMixRate[2][0] = GetPrivateProfileInt(section,"PlusExcSetItemLevelMixRate3_AL0",0,path);
	this->m_PlusExcSetItemLevelMixRate[2][1] = GetPrivateProfileInt(section,"PlusExcSetItemLevelMixRate3_AL1",0,path);
	this->m_PlusExcSetItemLevelMixRate[2][2] = GetPrivateProfileInt(section,"PlusExcSetItemLevelMixRate3_AL2",0,path);
	this->m_PlusExcSetItemLevelMixRate[2][3] = GetPrivateProfileInt(section,"PlusExcSetItemLevelMixRate3_AL3",0,path);

	#if(GAMESERVER_UPDATE>=401)

	this->m_PlusSocketItemLevelMixRate[2][0] = GetPrivateProfileInt(section,"PlusSocketItemLevelMixRate3_AL0",0,path);
	this->m_PlusSocketItemLevelMixRate[2][1] = GetPrivateProfileInt(section,"PlusSocketItemLevelMixRate3_AL1",0,path);
	this->m_PlusSocketItemLevelMixRate[2][2] = GetPrivateProfileInt(section,"PlusSocketItemLevelMixRate3_AL2",0,path);
	this->m_PlusSocketItemLevelMixRate[2][3] = GetPrivateProfileInt(section,"PlusSocketItemLevelMixRate3_AL3",0,path);

	#endif

	this->m_PlusCommonItemLevelMixRate[3][0] = GetPrivateProfileInt(section,"PlusCommonItemLevelMixRate4_AL0",0,path);
	this->m_PlusCommonItemLevelMixRate[3][1] = GetPrivateProfileInt(section,"PlusCommonItemLevelMixRate4_AL1",0,path);
	this->m_PlusCommonItemLevelMixRate[3][2] = GetPrivateProfileInt(section,"PlusCommonItemLevelMixRate4_AL2",0,path);
	this->m_PlusCommonItemLevelMixRate[3][3] = GetPrivateProfileInt(section,"PlusCommonItemLevelMixRate4_AL3",0,path);

	this->m_PlusExcSetItemLevelMixRate[3][0] = GetPrivateProfileInt(section,"PlusExcSetItemLevelMixRate4_AL0",0,path);
	this->m_PlusExcSetItemLevelMixRate[3][1] = GetPrivateProfileInt(section,"PlusExcSetItemLevelMixRate4_AL1",0,path);
	this->m_PlusExcSetItemLevelMixRate[3][2] = GetPrivateProfileInt(section,"PlusExcSetItemLevelMixRate4_AL2",0,path);
	this->m_PlusExcSetItemLevelMixRate[3][3] = GetPrivateProfileInt(section,"PlusExcSetItemLevelMixRate4_AL3",0,path);

	#if(GAMESERVER_UPDATE>=401)

	this->m_PlusSocketItemLevelMixRate[3][0] = GetPrivateProfileInt(section,"PlusSocketItemLevelMixRate4_AL0",0,path);
	this->m_PlusSocketItemLevelMixRate[3][1] = GetPrivateProfileInt(section,"PlusSocketItemLevelMixRate4_AL1",0,path);
	this->m_PlusSocketItemLevelMixRate[3][2] = GetPrivateProfileInt(section,"PlusSocketItemLevelMixRate4_AL2",0,path);
	this->m_PlusSocketItemLevelMixRate[3][3] = GetPrivateProfileInt(section,"PlusSocketItemLevelMixRate4_AL3",0,path);

	#endif

	#if(GAMESERVER_UPDATE>=501)

	this->m_PlusCommonItemLevelMixRate[4][0] = GetPrivateProfileInt(section,"PlusCommonItemLevelMixRate5_AL0",0,path);
	this->m_PlusCommonItemLevelMixRate[4][1] = GetPrivateProfileInt(section,"PlusCommonItemLevelMixRate5_AL1",0,path);
	this->m_PlusCommonItemLevelMixRate[4][2] = GetPrivateProfileInt(section,"PlusCommonItemLevelMixRate5_AL2",0,path);
	this->m_PlusCommonItemLevelMixRate[4][3] = GetPrivateProfileInt(section,"PlusCommonItemLevelMixRate5_AL3",0,path);

	this->m_PlusExcSetItemLevelMixRate[4][0] = GetPrivateProfileInt(section,"PlusExcSetItemLevelMixRate5_AL0",0,path);
	this->m_PlusExcSetItemLevelMixRate[4][1] = GetPrivateProfileInt(section,"PlusExcSetItemLevelMixRate5_AL1",0,path);
	this->m_PlusExcSetItemLevelMixRate[4][2] = GetPrivateProfileInt(section,"PlusExcSetItemLevelMixRate5_AL2",0,path);
	this->m_PlusExcSetItemLevelMixRate[4][3] = GetPrivateProfileInt(section,"PlusExcSetItemLevelMixRate5_AL3",0,path);

	this->m_PlusSocketItemLevelMixRate[4][0] = GetPrivateProfileInt(section,"PlusSocketItemLevelMixRate5_AL0",0,path);
	this->m_PlusSocketItemLevelMixRate[4][1] = GetPrivateProfileInt(section,"PlusSocketItemLevelMixRate5_AL1",0,path);
	this->m_PlusSocketItemLevelMixRate[4][2] = GetPrivateProfileInt(section,"PlusSocketItemLevelMixRate5_AL2",0,path);
	this->m_PlusSocketItemLevelMixRate[4][3] = GetPrivateProfileInt(section,"PlusSocketItemLevelMixRate5_AL3",0,path);

	this->m_PlusCommonItemLevelMixRate[5][0] = GetPrivateProfileInt(section,"PlusCommonItemLevelMixRate6_AL0",0,path);
	this->m_PlusCommonItemLevelMixRate[5][1] = GetPrivateProfileInt(section,"PlusCommonItemLevelMixRate6_AL1",0,path);
	this->m_PlusCommonItemLevelMixRate[5][2] = GetPrivateProfileInt(section,"PlusCommonItemLevelMixRate6_AL2",0,path);
	this->m_PlusCommonItemLevelMixRate[5][3] = GetPrivateProfileInt(section,"PlusCommonItemLevelMixRate6_AL3",0,path);

	this->m_PlusExcSetItemLevelMixRate[5][0] = GetPrivateProfileInt(section,"PlusExcSetItemLevelMixRate6_AL0",0,path);
	this->m_PlusExcSetItemLevelMixRate[5][1] = GetPrivateProfileInt(section,"PlusExcSetItemLevelMixRate6_AL1",0,path);
	this->m_PlusExcSetItemLevelMixRate[5][2] = GetPrivateProfileInt(section,"PlusExcSetItemLevelMixRate6_AL2",0,path);
	this->m_PlusExcSetItemLevelMixRate[5][3] = GetPrivateProfileInt(section,"PlusExcSetItemLevelMixRate6_AL3",0,path);

	this->m_PlusSocketItemLevelMixRate[5][0] = GetPrivateProfileInt(section,"PlusSocketItemLevelMixRate6_AL0",0,path);
	this->m_PlusSocketItemLevelMixRate[5][1] = GetPrivateProfileInt(section,"PlusSocketItemLevelMixRate6_AL1",0,path);
	this->m_PlusSocketItemLevelMixRate[5][2] = GetPrivateProfileInt(section,"PlusSocketItemLevelMixRate6_AL2",0,path);
	this->m_PlusSocketItemLevelMixRate[5][3] = GetPrivateProfileInt(section,"PlusSocketItemLevelMixRate6_AL3",0,path);

	#endif

	this->m_DinorantMixRate[0] = GetPrivateProfileInt(section,"DinorantMixRate_AL0",0,path);
	this->m_DinorantMixRate[1] = GetPrivateProfileInt(section,"DinorantMixRate_AL1",0,path);
	this->m_DinorantMixRate[2] = GetPrivateProfileInt(section,"DinorantMixRate_AL2",0,path);
	this->m_DinorantMixRate[3] = GetPrivateProfileInt(section,"DinorantMixRate_AL3",0,path);

	this->m_FruitMixRate[0] = GetPrivateProfileInt(section,"FruitMixRate_AL0",0,path);
	this->m_FruitMixRate[1] = GetPrivateProfileInt(section,"FruitMixRate_AL1",0,path);
	this->m_FruitMixRate[2] = GetPrivateProfileInt(section,"FruitMixRate_AL2",0,path);
	this->m_FruitMixRate[3] = GetPrivateProfileInt(section,"FruitMixRate_AL3",0,path);

	this->m_Wing2MixRate[0] = GetPrivateProfileInt(section,"Wing2MixRate_AL0",0,path);
	this->m_Wing2MixRate[1] = GetPrivateProfileInt(section,"Wing2MixRate_AL1",0,path);
	this->m_Wing2MixRate[2] = GetPrivateProfileInt(section,"Wing2MixRate_AL2",0,path);
	this->m_Wing2MixRate[3] = GetPrivateProfileInt(section,"Wing2MixRate_AL3",0,path);

	this->m_BloodCastleMixRate1[0] = GetPrivateProfileInt(section,"BloodCastleMixRate1_AL0",0,path);
	this->m_BloodCastleMixRate1[1] = GetPrivateProfileInt(section,"BloodCastleMixRate1_AL1",0,path);
	this->m_BloodCastleMixRate1[2] = GetPrivateProfileInt(section,"BloodCastleMixRate1_AL2",0,path);
	this->m_BloodCastleMixRate1[3] = GetPrivateProfileInt(section,"BloodCastleMixRate1_AL3",0,path);

	this->m_BloodCastleMixRate2[0] = GetPrivateProfileInt(section,"BloodCastleMixRate2_AL0",0,path);
	this->m_BloodCastleMixRate2[1] = GetPrivateProfileInt(section,"BloodCastleMixRate2_AL1",0,path);
	this->m_BloodCastleMixRate2[2] = GetPrivateProfileInt(section,"BloodCastleMixRate2_AL2",0,path);
	this->m_BloodCastleMixRate2[3] = GetPrivateProfileInt(section,"BloodCastleMixRate2_AL3",0,path);

	this->m_BloodCastleMixRate3[0] = GetPrivateProfileInt(section,"BloodCastleMixRate3_AL0",0,path);
	this->m_BloodCastleMixRate3[1] = GetPrivateProfileInt(section,"BloodCastleMixRate3_AL1",0,path);
	this->m_BloodCastleMixRate3[2] = GetPrivateProfileInt(section,"BloodCastleMixRate3_AL2",0,path);
	this->m_BloodCastleMixRate3[3] = GetPrivateProfileInt(section,"BloodCastleMixRate3_AL3",0,path);

	this->m_BloodCastleMixRate4[0] = GetPrivateProfileInt(section,"BloodCastleMixRate4_AL0",0,path);
	this->m_BloodCastleMixRate4[1] = GetPrivateProfileInt(section,"BloodCastleMixRate4_AL1",0,path);
	this->m_BloodCastleMixRate4[2] = GetPrivateProfileInt(section,"BloodCastleMixRate4_AL2",0,path);
	this->m_BloodCastleMixRate4[3] = GetPrivateProfileInt(section,"BloodCastleMixRate4_AL3",0,path);

	this->m_BloodCastleMixRate5[0] = GetPrivateProfileInt(section,"BloodCastleMixRate5_AL0",0,path);
	this->m_BloodCastleMixRate5[1] = GetPrivateProfileInt(section,"BloodCastleMixRate5_AL1",0,path);
	this->m_BloodCastleMixRate5[2] = GetPrivateProfileInt(section,"BloodCastleMixRate5_AL2",0,path);
	this->m_BloodCastleMixRate5[3] = GetPrivateProfileInt(section,"BloodCastleMixRate5_AL3",0,path);

	this->m_BloodCastleMixRate6[0] = GetPrivateProfileInt(section,"BloodCastleMixRate6_AL0",0,path);
	this->m_BloodCastleMixRate6[1] = GetPrivateProfileInt(section,"BloodCastleMixRate6_AL1",0,path);
	this->m_BloodCastleMixRate6[2] = GetPrivateProfileInt(section,"BloodCastleMixRate6_AL2",0,path);
	this->m_BloodCastleMixRate6[3] = GetPrivateProfileInt(section,"BloodCastleMixRate6_AL3",0,path);

	this->m_BloodCastleMixRate7[0] = GetPrivateProfileInt(section,"BloodCastleMixRate7_AL0",0,path);
	this->m_BloodCastleMixRate7[1] = GetPrivateProfileInt(section,"BloodCastleMixRate7_AL1",0,path);
	this->m_BloodCastleMixRate7[2] = GetPrivateProfileInt(section,"BloodCastleMixRate7_AL2",0,path);
	this->m_BloodCastleMixRate7[3] = GetPrivateProfileInt(section,"BloodCastleMixRate7_AL3",0,path);

	#if(GAMESERVER_UPDATE>=401)

	this->m_BloodCastleMixRate8[0] = GetPrivateProfileInt(section,"BloodCastleMixRate8_AL0",0,path);
	this->m_BloodCastleMixRate8[1] = GetPrivateProfileInt(section,"BloodCastleMixRate8_AL1",0,path);
	this->m_BloodCastleMixRate8[2] = GetPrivateProfileInt(section,"BloodCastleMixRate8_AL2",0,path);
	this->m_BloodCastleMixRate8[3] = GetPrivateProfileInt(section,"BloodCastleMixRate8_AL3",0,path);

	#endif

	this->m_Wing1MixRate[0] = GetPrivateProfileInt(section,"Wing1MixRate_AL0",0,path);
	this->m_Wing1MixRate[1] = GetPrivateProfileInt(section,"Wing1MixRate_AL1",0,path);
	this->m_Wing1MixRate[2] = GetPrivateProfileInt(section,"Wing1MixRate_AL2",0,path);
	this->m_Wing1MixRate[3] = GetPrivateProfileInt(section,"Wing1MixRate_AL3",0,path);

	this->m_PetMixRate[0] = GetPrivateProfileInt(section,"PetMixRate_AL0",0,path);
	this->m_PetMixRate[1] = GetPrivateProfileInt(section,"PetMixRate_AL1",0,path);
	this->m_PetMixRate[2] = GetPrivateProfileInt(section,"PetMixRate_AL2",0,path);
	this->m_PetMixRate[3] = GetPrivateProfileInt(section,"PetMixRate_AL3",0,path);

	#if(GAMESERVER_UPDATE>=201)

	this->m_PieceOfHornMixRate[0] = GetPrivateProfileInt(section,"PieceOfHornMixRate_AL0",0,path);
	this->m_PieceOfHornMixRate[1] = GetPrivateProfileInt(section,"PieceOfHornMixRate_AL1",0,path);
	this->m_PieceOfHornMixRate[2] = GetPrivateProfileInt(section,"PieceOfHornMixRate_AL2",0,path);
	this->m_PieceOfHornMixRate[3] = GetPrivateProfileInt(section,"PieceOfHornMixRate_AL3",0,path);

	this->m_BrokenHornMixRate[0] = GetPrivateProfileInt(section,"BrokenHornMixRate_AL0",0,path);
	this->m_BrokenHornMixRate[1] = GetPrivateProfileInt(section,"BrokenHornMixRate_AL1",0,path);
	this->m_BrokenHornMixRate[2] = GetPrivateProfileInt(section,"BrokenHornMixRate_AL2",0,path);
	this->m_BrokenHornMixRate[3] = GetPrivateProfileInt(section,"BrokenHornMixRate_AL3",0,path);

	this->m_HornOfFenrirMixRate[0] = GetPrivateProfileInt(section,"HornOfFenrirMixRate_AL0",0,path);
	this->m_HornOfFenrirMixRate[1] = GetPrivateProfileInt(section,"HornOfFenrirMixRate_AL1",0,path);
	this->m_HornOfFenrirMixRate[2] = GetPrivateProfileInt(section,"HornOfFenrirMixRate_AL2",0,path);
	this->m_HornOfFenrirMixRate[3] = GetPrivateProfileInt(section,"HornOfFenrirMixRate_AL3",0,path);

	this->m_HornOfFenrirUpgradeMixRate[0] = GetPrivateProfileInt(section,"HornOfFenrirUpgradeMixRate_AL0",0,path);
	this->m_HornOfFenrirUpgradeMixRate[1] = GetPrivateProfileInt(section,"HornOfFenrirUpgradeMixRate_AL1",0,path);
	this->m_HornOfFenrirUpgradeMixRate[2] = GetPrivateProfileInt(section,"HornOfFenrirUpgradeMixRate_AL2",0,path);
	this->m_HornOfFenrirUpgradeMixRate[3] = GetPrivateProfileInt(section,"HornOfFenrirUpgradeMixRate_AL3",0,path);

	#endif

	#if(GAMESERVER_UPDATE>=201)

	this->m_ShieldPotionMixRate[0][0] = GetPrivateProfileInt(section,"ShieldPotionMixRate1_AL0",0,path);
	this->m_ShieldPotionMixRate[0][1] = GetPrivateProfileInt(section,"ShieldPotionMixRate1_AL1",0,path);
	this->m_ShieldPotionMixRate[0][2] = GetPrivateProfileInt(section,"ShieldPotionMixRate1_AL2",0,path);
	this->m_ShieldPotionMixRate[0][3] = GetPrivateProfileInt(section,"ShieldPotionMixRate1_AL3",0,path);

	this->m_ShieldPotionMixRate[1][0] = GetPrivateProfileInt(section,"ShieldPotionMixRate2_AL0",0,path);
	this->m_ShieldPotionMixRate[1][1] = GetPrivateProfileInt(section,"ShieldPotionMixRate2_AL1",0,path);
	this->m_ShieldPotionMixRate[1][2] = GetPrivateProfileInt(section,"ShieldPotionMixRate2_AL2",0,path);
	this->m_ShieldPotionMixRate[1][3] = GetPrivateProfileInt(section,"ShieldPotionMixRate2_AL3",0,path);

	this->m_ShieldPotionMixRate[2][0] = GetPrivateProfileInt(section,"ShieldPotionMixRate3_AL0",0,path);
	this->m_ShieldPotionMixRate[2][1] = GetPrivateProfileInt(section,"ShieldPotionMixRate3_AL1",0,path);
	this->m_ShieldPotionMixRate[2][2] = GetPrivateProfileInt(section,"ShieldPotionMixRate3_AL2",0,path);
	this->m_ShieldPotionMixRate[2][3] = GetPrivateProfileInt(section,"ShieldPotionMixRate3_AL3",0,path);

	this->m_JewelOfHarmonyItemPurityMixRate[0] = GetPrivateProfileInt(section,"JewelOfHarmonyItemPurityMixRate_AL0",0,path);
	this->m_JewelOfHarmonyItemPurityMixRate[1] = GetPrivateProfileInt(section,"JewelOfHarmonyItemPurityMixRate_AL1",0,path);
	this->m_JewelOfHarmonyItemPurityMixRate[2] = GetPrivateProfileInt(section,"JewelOfHarmonyItemPurityMixRate_AL2",0,path);
	this->m_JewelOfHarmonyItemPurityMixRate[3] = GetPrivateProfileInt(section,"JewelOfHarmonyItemPurityMixRate_AL3",0,path);

	this->m_JewelOfHarmonyItemSmeltMixRate1[0] = GetPrivateProfileInt(section,"JewelOfHarmonyItemSmeltMixRate1_AL0",0,path);
	this->m_JewelOfHarmonyItemSmeltMixRate1[1] = GetPrivateProfileInt(section,"JewelOfHarmonyItemSmeltMixRate1_AL1",0,path);
	this->m_JewelOfHarmonyItemSmeltMixRate1[2] = GetPrivateProfileInt(section,"JewelOfHarmonyItemSmeltMixRate1_AL2",0,path);
	this->m_JewelOfHarmonyItemSmeltMixRate1[3] = GetPrivateProfileInt(section,"JewelOfHarmonyItemSmeltMixRate1_AL3",0,path);

	this->m_JewelOfHarmonyItemSmeltMixRate2[0] = GetPrivateProfileInt(section,"JewelOfHarmonyItemSmeltMixRate2_AL0",0,path);
	this->m_JewelOfHarmonyItemSmeltMixRate2[1] = GetPrivateProfileInt(section,"JewelOfHarmonyItemSmeltMixRate2_AL1",0,path);
	this->m_JewelOfHarmonyItemSmeltMixRate2[2] = GetPrivateProfileInt(section,"JewelOfHarmonyItemSmeltMixRate2_AL2",0,path);
	this->m_JewelOfHarmonyItemSmeltMixRate2[3] = GetPrivateProfileInt(section,"JewelOfHarmonyItemSmeltMixRate2_AL3",0,path);

	this->m_JewelOfHarmonyItemRestoreMixRate[0] = GetPrivateProfileInt(section,"JewelOfHarmonyItemRestoreMixRate_AL0",0,path);
	this->m_JewelOfHarmonyItemRestoreMixRate[1] = GetPrivateProfileInt(section,"JewelOfHarmonyItemRestoreMixRate_AL1",0,path);
	this->m_JewelOfHarmonyItemRestoreMixRate[2] = GetPrivateProfileInt(section,"JewelOfHarmonyItemRestoreMixRate_AL2",0,path);
	this->m_JewelOfHarmonyItemRestoreMixRate[3] = GetPrivateProfileInt(section,"JewelOfHarmonyItemRestoreMixRate_AL3",0,path);

	#endif

	#if(GAMESERVER_UPDATE>=201)

	this->m_Item380MixRate1[0] = GetPrivateProfileInt(section,"Item380MixRate1_AL0",0,path);
	this->m_Item380MixRate1[1] = GetPrivateProfileInt(section,"Item380MixRate1_AL1",0,path);
	this->m_Item380MixRate1[2] = GetPrivateProfileInt(section,"Item380MixRate1_AL2",0,path);
	this->m_Item380MixRate1[3] = GetPrivateProfileInt(section,"Item380MixRate1_AL3",0,path);

	this->m_Item380MixRate2[0] = GetPrivateProfileInt(section,"Item380MixRate2_AL0",0,path);
	this->m_Item380MixRate2[1] = GetPrivateProfileInt(section,"Item380MixRate2_AL1",0,path);
	this->m_Item380MixRate2[2] = GetPrivateProfileInt(section,"Item380MixRate2_AL2",0,path);
	this->m_Item380MixRate2[3] = GetPrivateProfileInt(section,"Item380MixRate2_AL3",0,path);

	#endif

	#if(GAMESERVER_UPDATE>=301)

	this->m_IllusionTempleMixRate1[0] = GetPrivateProfileInt(section,"IllusionTempleMixRate1_AL0",0,path);
	this->m_IllusionTempleMixRate1[1] = GetPrivateProfileInt(section,"IllusionTempleMixRate1_AL1",0,path);
	this->m_IllusionTempleMixRate1[2] = GetPrivateProfileInt(section,"IllusionTempleMixRate1_AL2",0,path);
	this->m_IllusionTempleMixRate1[3] = GetPrivateProfileInt(section,"IllusionTempleMixRate1_AL3",0,path);

	this->m_IllusionTempleMixRate2[0] = GetPrivateProfileInt(section,"IllusionTempleMixRate2_AL0",0,path);
	this->m_IllusionTempleMixRate2[1] = GetPrivateProfileInt(section,"IllusionTempleMixRate2_AL1",0,path);
	this->m_IllusionTempleMixRate2[2] = GetPrivateProfileInt(section,"IllusionTempleMixRate2_AL2",0,path);
	this->m_IllusionTempleMixRate2[3] = GetPrivateProfileInt(section,"IllusionTempleMixRate2_AL3",0,path);

	this->m_IllusionTempleMixRate3[0] = GetPrivateProfileInt(section,"IllusionTempleMixRate3_AL0",0,path);
	this->m_IllusionTempleMixRate3[1] = GetPrivateProfileInt(section,"IllusionTempleMixRate3_AL1",0,path);
	this->m_IllusionTempleMixRate3[2] = GetPrivateProfileInt(section,"IllusionTempleMixRate3_AL2",0,path);
	this->m_IllusionTempleMixRate3[3] = GetPrivateProfileInt(section,"IllusionTempleMixRate3_AL3",0,path);

	this->m_IllusionTempleMixRate4[0] = GetPrivateProfileInt(section,"IllusionTempleMixRate4_AL0",0,path);
	this->m_IllusionTempleMixRate4[1] = GetPrivateProfileInt(section,"IllusionTempleMixRate4_AL1",0,path);
	this->m_IllusionTempleMixRate4[2] = GetPrivateProfileInt(section,"IllusionTempleMixRate4_AL2",0,path);
	this->m_IllusionTempleMixRate4[3] = GetPrivateProfileInt(section,"IllusionTempleMixRate4_AL3",0,path);

	this->m_IllusionTempleMixRate5[0] = GetPrivateProfileInt(section,"IllusionTempleMixRate5_AL0",0,path);
	this->m_IllusionTempleMixRate5[1] = GetPrivateProfileInt(section,"IllusionTempleMixRate5_AL1",0,path);
	this->m_IllusionTempleMixRate5[2] = GetPrivateProfileInt(section,"IllusionTempleMixRate5_AL2",0,path);
	this->m_IllusionTempleMixRate5[3] = GetPrivateProfileInt(section,"IllusionTempleMixRate5_AL3",0,path);

	#if(GAMESERVER_UPDATE>=401)

	this->m_IllusionTempleMixRate6[0] = GetPrivateProfileInt(section,"IllusionTempleMixRate6_AL0",0,path);
	this->m_IllusionTempleMixRate6[1] = GetPrivateProfileInt(section,"IllusionTempleMixRate6_AL1",0,path);
	this->m_IllusionTempleMixRate6[2] = GetPrivateProfileInt(section,"IllusionTempleMixRate6_AL2",0,path);
	this->m_IllusionTempleMixRate6[3] = GetPrivateProfileInt(section,"IllusionTempleMixRate6_AL3",0,path);

	#endif

	#endif

	#if(GAMESERVER_UPDATE>=301)

	this->m_FeatherOfCondorMixRate[0] = GetPrivateProfileInt(section,"FeatherOfCondorMixRate_AL0",0,path);
	this->m_FeatherOfCondorMixRate[1] = GetPrivateProfileInt(section,"FeatherOfCondorMixRate_AL1",0,path);
	this->m_FeatherOfCondorMixRate[2] = GetPrivateProfileInt(section,"FeatherOfCondorMixRate_AL2",0,path);
	this->m_FeatherOfCondorMixRate[3] = GetPrivateProfileInt(section,"FeatherOfCondorMixRate_AL3",0,path);

	this->m_Wing3MixRate[0] = GetPrivateProfileInt(section,"Wing3MixRate_AL0",0,path);
	this->m_Wing3MixRate[1] = GetPrivateProfileInt(section,"Wing3MixRate_AL1",0,path);
	this->m_Wing3MixRate[2] = GetPrivateProfileInt(section,"Wing3MixRate_AL2",0,path);
	this->m_Wing3MixRate[3] = GetPrivateProfileInt(section,"Wing3MixRate_AL3",0,path);

	#endif

	#if(GAMESERVER_UPDATE>=401)

	this->m_SocketItemCreateSeedMixRate[0] = GetPrivateProfileInt(section,"SocketItemCreateSeedMixRate_AL0",0,path);
	this->m_SocketItemCreateSeedMixRate[1] = GetPrivateProfileInt(section,"SocketItemCreateSeedMixRate_AL1",0,path);
	this->m_SocketItemCreateSeedMixRate[2] = GetPrivateProfileInt(section,"SocketItemCreateSeedMixRate_AL2",0,path);
	this->m_SocketItemCreateSeedMixRate[3] = GetPrivateProfileInt(section,"SocketItemCreateSeedMixRate_AL3",0,path);

	this->m_SocketItemCreateSeedSphereMixRate[0] = GetPrivateProfileInt(section,"SocketItemCreateSeedSphereMixRate_AL0",0,path);
	this->m_SocketItemCreateSeedSphereMixRate[1] = GetPrivateProfileInt(section,"SocketItemCreateSeedSphereMixRate_AL1",0,path);
	this->m_SocketItemCreateSeedSphereMixRate[2] = GetPrivateProfileInt(section,"SocketItemCreateSeedSphereMixRate_AL2",0,path);
	this->m_SocketItemCreateSeedSphereMixRate[3] = GetPrivateProfileInt(section,"SocketItemCreateSeedSphereMixRate_AL3",0,path);

	#endif

	#if(GAMESERVER_UPDATE>=801)

	this->m_SummonScrollMixRate1[0] = GetPrivateProfileInt(section,"SummonScrollMixRate1_AL0",0,path);
	this->m_SummonScrollMixRate1[1] = GetPrivateProfileInt(section,"SummonScrollMixRate1_AL1",0,path);
	this->m_SummonScrollMixRate1[2] = GetPrivateProfileInt(section,"SummonScrollMixRate1_AL2",0,path);
	this->m_SummonScrollMixRate1[3] = GetPrivateProfileInt(section,"SummonScrollMixRate1_AL3",0,path);

	this->m_SummonScrollMixRate2[0] = GetPrivateProfileInt(section,"SummonScrollMixRate2_AL0",0,path);
	this->m_SummonScrollMixRate2[1] = GetPrivateProfileInt(section,"SummonScrollMixRate2_AL1",0,path);
	this->m_SummonScrollMixRate2[2] = GetPrivateProfileInt(section,"SummonScrollMixRate2_AL2",0,path);
	this->m_SummonScrollMixRate2[3] = GetPrivateProfileInt(section,"SummonScrollMixRate2_AL3",0,path);

	this->m_SummonScrollMixRate3[0] = GetPrivateProfileInt(section,"SummonScrollMixRate3_AL0",0,path);
	this->m_SummonScrollMixRate3[1] = GetPrivateProfileInt(section,"SummonScrollMixRate3_AL1",0,path);
	this->m_SummonScrollMixRate3[2] = GetPrivateProfileInt(section,"SummonScrollMixRate3_AL2",0,path);
	this->m_SummonScrollMixRate3[3] = GetPrivateProfileInt(section,"SummonScrollMixRate3_AL3",0,path);

	#endif

	#if(GAMESERVER_UPDATE>=602)

	this->m_LuckyItemRefineMixRate1[0] = GetPrivateProfileInt(section,"LuckyItemRefineMixRate1_AL0",0,path);
	this->m_LuckyItemRefineMixRate1[1] = GetPrivateProfileInt(section,"LuckyItemRefineMixRate1_AL1",0,path);
	this->m_LuckyItemRefineMixRate1[2] = GetPrivateProfileInt(section,"LuckyItemRefineMixRate1_AL2",0,path);
	this->m_LuckyItemRefineMixRate1[3] = GetPrivateProfileInt(section,"LuckyItemRefineMixRate1_AL3",0,path);

	this->m_LuckyItemRefineMixRate2[0] = GetPrivateProfileInt(section,"LuckyItemRefineMixRate2_AL0",0,path);
	this->m_LuckyItemRefineMixRate2[1] = GetPrivateProfileInt(section,"LuckyItemRefineMixRate2_AL1",0,path);
	this->m_LuckyItemRefineMixRate2[2] = GetPrivateProfileInt(section,"LuckyItemRefineMixRate2_AL2",0,path);
	this->m_LuckyItemRefineMixRate2[3] = GetPrivateProfileInt(section,"LuckyItemRefineMixRate2_AL3",0,path);

	#endif

	#if(GAMESERVER_UPDATE>=701)

	this->m_MonsterWingMixRate[0] = GetPrivateProfileInt(section,"MonsterWingMixRate_AL0",0,path);
	this->m_MonsterWingMixRate[1] = GetPrivateProfileInt(section,"MonsterWingMixRate_AL1",0,path);
	this->m_MonsterWingMixRate[2] = GetPrivateProfileInt(section,"MonsterWingMixRate_AL2",0,path);
	this->m_MonsterWingMixRate[3] = GetPrivateProfileInt(section,"MonsterWingMixRate_AL3",0,path);

	this->m_SocketWeaponMixRate[0] = GetPrivateProfileInt(section,"SocketWeaponMixRate_AL0",0,path);
	this->m_SocketWeaponMixRate[1] = GetPrivateProfileInt(section,"SocketWeaponMixRate_AL1",0,path);
	this->m_SocketWeaponMixRate[2] = GetPrivateProfileInt(section,"SocketWeaponMixRate_AL2",0,path);
	this->m_SocketWeaponMixRate[3] = GetPrivateProfileInt(section,"SocketWeaponMixRate_AL3",0,path);

	this->m_PentagramMithrilMixRate[0] = GetPrivateProfileInt(section,"PentagramMithrilMixRate_AL0",0,path);
	this->m_PentagramMithrilMixRate[1] = GetPrivateProfileInt(section,"PentagramMithrilMixRate_AL1",0,path);
	this->m_PentagramMithrilMixRate[2] = GetPrivateProfileInt(section,"PentagramMithrilMixRate_AL2",0,path);
	this->m_PentagramMithrilMixRate[3] = GetPrivateProfileInt(section,"PentagramMithrilMixRate_AL3",0,path);

	this->m_PentagramElixirMixRate[0] = GetPrivateProfileInt(section,"PentagramElixirMixRate_AL0",0,path);
	this->m_PentagramElixirMixRate[1] = GetPrivateProfileInt(section,"PentagramElixirMixRate_AL1",0,path);
	this->m_PentagramElixirMixRate[2] = GetPrivateProfileInt(section,"PentagramElixirMixRate_AL2",0,path);
	this->m_PentagramElixirMixRate[3] = GetPrivateProfileInt(section,"PentagramElixirMixRate_AL3",0,path);

	this->m_PentagramJewelMixRate[0] = GetPrivateProfileInt(section,"PentagramJewelMixRate_AL0",0,path);
	this->m_PentagramJewelMixRate[1] = GetPrivateProfileInt(section,"PentagramJewelMixRate_AL1",0,path);
	this->m_PentagramJewelMixRate[2] = GetPrivateProfileInt(section,"PentagramJewelMixRate_AL2",0,path);
	this->m_PentagramJewelMixRate[3] = GetPrivateProfileInt(section,"PentagramJewelMixRate_AL3",0,path);

	this->m_PentagramDecompositeMixRate[0] = GetPrivateProfileInt(section,"PentagramDecompositeMixRate_AL0",0,path);
	this->m_PentagramDecompositeMixRate[1] = GetPrivateProfileInt(section,"PentagramDecompositeMixRate_AL1",0,path);
	this->m_PentagramDecompositeMixRate[2] = GetPrivateProfileInt(section,"PentagramDecompositeMixRate_AL2",0,path);
	this->m_PentagramDecompositeMixRate[3] = GetPrivateProfileInt(section,"PentagramDecompositeMixRate_AL3",0,path);

	this->m_TrophiesMixRate[0][0] = GetPrivateProfileInt(section,"TrophiesMixRate1_AL0",0,path);
	this->m_TrophiesMixRate[0][1] = GetPrivateProfileInt(section,"TrophiesMixRate1_AL1",0,path);
	this->m_TrophiesMixRate[0][2] = GetPrivateProfileInt(section,"TrophiesMixRate1_AL2",0,path);
	this->m_TrophiesMixRate[0][3] = GetPrivateProfileInt(section,"TrophiesMixRate1_AL3",0,path);

	this->m_TrophiesMixRate[1][0] = GetPrivateProfileInt(section,"TrophiesMixRate2_AL0",0,path);
	this->m_TrophiesMixRate[1][1] = GetPrivateProfileInt(section,"TrophiesMixRate2_AL1",0,path);
	this->m_TrophiesMixRate[1][2] = GetPrivateProfileInt(section,"TrophiesMixRate2_AL2",0,path);
	this->m_TrophiesMixRate[1][3] = GetPrivateProfileInt(section,"TrophiesMixRate2_AL3",0,path);

	this->m_TrophiesMixRate[2][0] = GetPrivateProfileInt(section,"TrophiesMixRate3_AL0",0,path);
	this->m_TrophiesMixRate[2][1] = GetPrivateProfileInt(section,"TrophiesMixRate3_AL1",0,path);
	this->m_TrophiesMixRate[2][2] = GetPrivateProfileInt(section,"TrophiesMixRate3_AL2",0,path);
	this->m_TrophiesMixRate[2][3] = GetPrivateProfileInt(section,"TrophiesMixRate3_AL3",0,path);

	this->m_TrophiesMixRate[3][0] = GetPrivateProfileInt(section,"TrophiesMixRate4_AL0",0,path);
	this->m_TrophiesMixRate[3][1] = GetPrivateProfileInt(section,"TrophiesMixRate4_AL1",0,path);
	this->m_TrophiesMixRate[3][2] = GetPrivateProfileInt(section,"TrophiesMixRate4_AL2",0,path);
	this->m_TrophiesMixRate[3][3] = GetPrivateProfileInt(section,"TrophiesMixRate4_AL3",0,path);

	this->m_TrophiesMixRate[4][0] = GetPrivateProfileInt(section,"TrophiesMixRate5_AL0",0,path);
	this->m_TrophiesMixRate[4][1] = GetPrivateProfileInt(section,"TrophiesMixRate5_AL1",0,path);
	this->m_TrophiesMixRate[4][2] = GetPrivateProfileInt(section,"TrophiesMixRate5_AL2",0,path);
	this->m_TrophiesMixRate[4][3] = GetPrivateProfileInt(section,"TrophiesMixRate5_AL3",0,path);

	this->m_TrophiesMixRate[5][0] = GetPrivateProfileInt(section,"TrophiesMixRate6_AL0",0,path);
	this->m_TrophiesMixRate[5][1] = GetPrivateProfileInt(section,"TrophiesMixRate6_AL1",0,path);
	this->m_TrophiesMixRate[5][2] = GetPrivateProfileInt(section,"TrophiesMixRate6_AL2",0,path);
	this->m_TrophiesMixRate[5][3] = GetPrivateProfileInt(section,"TrophiesMixRate6_AL3",0,path);

	this->m_TrophiesMixRate[6][0] = GetPrivateProfileInt(section,"TrophiesMixRate7_AL0",0,path);
	this->m_TrophiesMixRate[6][1] = GetPrivateProfileInt(section,"TrophiesMixRate7_AL1",0,path);
	this->m_TrophiesMixRate[6][2] = GetPrivateProfileInt(section,"TrophiesMixRate7_AL2",0,path);
	this->m_TrophiesMixRate[6][3] = GetPrivateProfileInt(section,"TrophiesMixRate7_AL3",0,path);

	this->m_TrophiesMixRate[7][0] = GetPrivateProfileInt(section,"TrophiesMixRate8_AL0",0,path);
	this->m_TrophiesMixRate[7][1] = GetPrivateProfileInt(section,"TrophiesMixRate8_AL1",0,path);
	this->m_TrophiesMixRate[7][2] = GetPrivateProfileInt(section,"TrophiesMixRate8_AL2",0,path);
	this->m_TrophiesMixRate[7][3] = GetPrivateProfileInt(section,"TrophiesMixRate8_AL3",0,path);

	this->m_TrophiesMixRate[8][0] = GetPrivateProfileInt(section,"TrophiesMixRate9_AL0",0,path);
	this->m_TrophiesMixRate[8][1] = GetPrivateProfileInt(section,"TrophiesMixRate9_AL1",0,path);
	this->m_TrophiesMixRate[8][2] = GetPrivateProfileInt(section,"TrophiesMixRate9_AL2",0,path);
	this->m_TrophiesMixRate[8][3] = GetPrivateProfileInt(section,"TrophiesMixRate9_AL3",0,path);

	this->m_TrophiesMixRate[9][0] = GetPrivateProfileInt(section,"TrophiesMixRate10_AL0",0,path);
	this->m_TrophiesMixRate[9][1] = GetPrivateProfileInt(section,"TrophiesMixRate10_AL1",0,path);
	this->m_TrophiesMixRate[9][2] = GetPrivateProfileInt(section,"TrophiesMixRate10_AL2",0,path);
	this->m_TrophiesMixRate[9][3] = GetPrivateProfileInt(section,"TrophiesMixRate10_AL3",0,path);

	#endif
}

void CServerInfo::ReadCharacterInfo(char* section,char* path) // OK
{
	this->m_DuelDamageRate = GetPrivateProfileInt(section,"DuelDamageRate",0,path);

	#if(GAMESERVER_UPDATE>=501)

	this->m_GensDamageRate = GetPrivateProfileInt(section,"GensDamageRate",0,path);

	#endif

	this->m_CustomArenaDamageRate = GetPrivateProfileInt(section,"CustomArenaDamageRate",0,path);

	this->m_ChaosCastleDamageRate = GetPrivateProfileInt(section,"ChaosCastleDamageRate",0,path);

	#if(GAMESERVER_UPDATE>=701)

	this->m_ArcaBattleDamageRate1 = GetPrivateProfileInt(section,"ArcaBattleDamageRate1",0,path);

	this->m_ArcaBattleDamageRate2 = GetPrivateProfileInt(section,"ArcaBattleDamageRate2",0,path);

	#endif

	#if(GAMESERVER_UPDATE>=301)

	this->m_IllusionTempleDamageRate = GetPrivateProfileInt(section,"IllusionTempleDamageRate",0,path);

	#endif

	this->m_CastleSiegeDamageRate1 = GetPrivateProfileInt(section,"CastleSiegeDamageRate1",0,path);
	this->m_CastleSiegeDamageRate2 = GetPrivateProfileInt(section,"CastleSiegeDamageRate2",0,path);
	this->m_CastleSiegeDamageRate3 = GetPrivateProfileInt(section,"CastleSiegeDamageRate3",0,path);

	this->m_GeneralDamageRatePvP = GetPrivateProfileInt(section,"GeneralDamageRatePvP",0,path);
	this->m_GeneralDamageRatePvM = GetPrivateProfileInt(section,"GeneralDamageRatePvM",0,path);

	this->m_ReflectDamageRatePvP = GetPrivateProfileInt(section,"ReflectDamageRatePvP",0,path);
	this->m_ReflectDamageRatePvM = GetPrivateProfileInt(section,"ReflectDamageRatePvM",0,path);

	this->m_DamageRatePvP[CLASS_DW] = GetPrivateProfileInt(section,"DWDamageRatePvP",0,path);
	this->m_DamageRatePvP[CLASS_DK] = GetPrivateProfileInt(section,"DKDamageRatePvP",0,path);
	this->m_DamageRatePvP[CLASS_FE] = GetPrivateProfileInt(section,"FEDamageRatePvP",0,path);
	this->m_DamageRatePvP[CLASS_MG] = GetPrivateProfileInt(section,"MGDamageRatePvP",0,path);
	this->m_DamageRatePvP[CLASS_DL] = GetPrivateProfileInt(section,"DLDamageRatePvP",0,path);
	#if(GAMESERVER_UPDATE>=401)
	this->m_DamageRatePvP[CLASS_SU] = GetPrivateProfileInt(section,"SUDamageRatePvP",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_DamageRatePvP[CLASS_RF] = GetPrivateProfileInt(section,"RFDamageRatePvP",0,path);
	#endif

	this->m_DamageRatePvM[CLASS_DW] = GetPrivateProfileInt(section,"DWDamageRatePvM",0,path);
	this->m_DamageRatePvM[CLASS_DK] = GetPrivateProfileInt(section,"DKDamageRatePvM",0,path);
	this->m_DamageRatePvM[CLASS_FE] = GetPrivateProfileInt(section,"FEDamageRatePvM",0,path);
	this->m_DamageRatePvM[CLASS_MG] = GetPrivateProfileInt(section,"MGDamageRatePvM",0,path);
	this->m_DamageRatePvM[CLASS_DL] = GetPrivateProfileInt(section,"DLDamageRatePvM",0,path);
	#if(GAMESERVER_UPDATE>=401)
	this->m_DamageRatePvM[CLASS_SU] = GetPrivateProfileInt(section,"SUDamageRatePvM",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_DamageRatePvM[CLASS_RF] = GetPrivateProfileInt(section,"RFDamageRatePvM",0,path);
	#endif

	this->m_DamageRateTo[CLASS_DW][CLASS_DW] = GetPrivateProfileInt(section,"DWDamageRateToDW",0,path);
	this->m_DamageRateTo[CLASS_DW][CLASS_DK] = GetPrivateProfileInt(section,"DWDamageRateToDK",0,path);
	this->m_DamageRateTo[CLASS_DW][CLASS_FE] = GetPrivateProfileInt(section,"DWDamageRateToFE",0,path);
	this->m_DamageRateTo[CLASS_DW][CLASS_MG] = GetPrivateProfileInt(section,"DWDamageRateToMG",0,path);
	this->m_DamageRateTo[CLASS_DW][CLASS_DL] = GetPrivateProfileInt(section,"DWDamageRateToDL",0,path);
	#if(GAMESERVER_UPDATE>=401)
	this->m_DamageRateTo[CLASS_DW][CLASS_SU] = GetPrivateProfileInt(section,"DWDamageRateToSU",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_DamageRateTo[CLASS_DW][CLASS_RF] = GetPrivateProfileInt(section,"DWDamageRateToRF",0,path);
	#endif

	this->m_DamageRateTo[CLASS_DK][CLASS_DW] = GetPrivateProfileInt(section,"DKDamageRateToDW",0,path);
	this->m_DamageRateTo[CLASS_DK][CLASS_DK] = GetPrivateProfileInt(section,"DKDamageRateToDK",0,path);
	this->m_DamageRateTo[CLASS_DK][CLASS_FE] = GetPrivateProfileInt(section,"DKDamageRateToFE",0,path);
	this->m_DamageRateTo[CLASS_DK][CLASS_MG] = GetPrivateProfileInt(section,"DKDamageRateToMG",0,path);
	this->m_DamageRateTo[CLASS_DK][CLASS_DL] = GetPrivateProfileInt(section,"DKDamageRateToDL",0,path);
	#if(GAMESERVER_UPDATE>=401)
	this->m_DamageRateTo[CLASS_DK][CLASS_SU] = GetPrivateProfileInt(section,"DKDamageRateToSU",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_DamageRateTo[CLASS_DK][CLASS_RF] = GetPrivateProfileInt(section,"DKDamageRateToRF",0,path);
	#endif

	this->m_DamageRateTo[CLASS_FE][CLASS_DW] = GetPrivateProfileInt(section,"FEDamageRateToDW",0,path);
	this->m_DamageRateTo[CLASS_FE][CLASS_DK] = GetPrivateProfileInt(section,"FEDamageRateToDK",0,path);
	this->m_DamageRateTo[CLASS_FE][CLASS_FE] = GetPrivateProfileInt(section,"FEDamageRateToFE",0,path);
	this->m_DamageRateTo[CLASS_FE][CLASS_MG] = GetPrivateProfileInt(section,"FEDamageRateToMG",0,path);
	this->m_DamageRateTo[CLASS_FE][CLASS_DL] = GetPrivateProfileInt(section,"FEDamageRateToDL",0,path);
	#if(GAMESERVER_UPDATE>=401)
	this->m_DamageRateTo[CLASS_FE][CLASS_SU] = GetPrivateProfileInt(section,"FEDamageRateToSU",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_DamageRateTo[CLASS_FE][CLASS_RF] = GetPrivateProfileInt(section,"FEDamageRateToRF",0,path);
	#endif

	this->m_DamageRateTo[CLASS_MG][CLASS_DW] = GetPrivateProfileInt(section,"MGDamageRateToDW",0,path);
	this->m_DamageRateTo[CLASS_MG][CLASS_DK] = GetPrivateProfileInt(section,"MGDamageRateToDK",0,path);
	this->m_DamageRateTo[CLASS_MG][CLASS_FE] = GetPrivateProfileInt(section,"MGDamageRateToFE",0,path);
	this->m_DamageRateTo[CLASS_MG][CLASS_MG] = GetPrivateProfileInt(section,"MGDamageRateToMG",0,path);
	this->m_DamageRateTo[CLASS_MG][CLASS_DL] = GetPrivateProfileInt(section,"MGDamageRateToDL",0,path);
	#if(GAMESERVER_UPDATE>=401)
	this->m_DamageRateTo[CLASS_MG][CLASS_SU] = GetPrivateProfileInt(section,"MGDamageRateToSU",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_DamageRateTo[CLASS_MG][CLASS_RF] = GetPrivateProfileInt(section,"MGDamageRateToRF",0,path);
	#endif

	this->m_DamageRateTo[CLASS_DL][CLASS_DW] = GetPrivateProfileInt(section,"DLDamageRateToDW",0,path);
	this->m_DamageRateTo[CLASS_DL][CLASS_DK] = GetPrivateProfileInt(section,"DLDamageRateToDK",0,path);
	this->m_DamageRateTo[CLASS_DL][CLASS_FE] = GetPrivateProfileInt(section,"DLDamageRateToFE",0,path);
	this->m_DamageRateTo[CLASS_DL][CLASS_MG] = GetPrivateProfileInt(section,"DLDamageRateToMG",0,path);
	this->m_DamageRateTo[CLASS_DL][CLASS_DL] = GetPrivateProfileInt(section,"DLDamageRateToDL",0,path);
	#if(GAMESERVER_UPDATE>=401)
	this->m_DamageRateTo[CLASS_DL][CLASS_SU] = GetPrivateProfileInt(section,"DLDamageRateToSU",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_DamageRateTo[CLASS_DL][CLASS_RF] = GetPrivateProfileInt(section,"DLDamageRateToRF",0,path);
	#endif

	#if(GAMESERVER_UPDATE>=401)
	this->m_DamageRateTo[CLASS_SU][CLASS_DW] = GetPrivateProfileInt(section,"SUDamageRateToDW",0,path);
	this->m_DamageRateTo[CLASS_SU][CLASS_DK] = GetPrivateProfileInt(section,"SUDamageRateToDK",0,path);
	this->m_DamageRateTo[CLASS_SU][CLASS_FE] = GetPrivateProfileInt(section,"SUDamageRateToFE",0,path);
	this->m_DamageRateTo[CLASS_SU][CLASS_MG] = GetPrivateProfileInt(section,"SUDamageRateToMG",0,path);
	this->m_DamageRateTo[CLASS_SU][CLASS_DL] = GetPrivateProfileInt(section,"SUDamageRateToDL",0,path);
	this->m_DamageRateTo[CLASS_SU][CLASS_SU] = GetPrivateProfileInt(section,"SUDamageRateToSU",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_DamageRateTo[CLASS_SU][CLASS_RF] = GetPrivateProfileInt(section,"SUDamageRateToRF",0,path);
	#endif

	#if(GAMESERVER_UPDATE>=601)
	this->m_DamageRateTo[CLASS_RF][CLASS_DW] = GetPrivateProfileInt(section,"RFDamageRateToDW",0,path);
	this->m_DamageRateTo[CLASS_RF][CLASS_DK] = GetPrivateProfileInt(section,"RFDamageRateToDK",0,path);
	this->m_DamageRateTo[CLASS_RF][CLASS_FE] = GetPrivateProfileInt(section,"RFDamageRateToFE",0,path);
	this->m_DamageRateTo[CLASS_RF][CLASS_MG] = GetPrivateProfileInt(section,"RFDamageRateToMG",0,path);
	this->m_DamageRateTo[CLASS_RF][CLASS_DL] = GetPrivateProfileInt(section,"RFDamageRateToDL",0,path);
	this->m_DamageRateTo[CLASS_RF][CLASS_SU] = GetPrivateProfileInt(section,"RFDamageRateToSU",0,path);
	this->m_DamageRateTo[CLASS_RF][CLASS_RF] = GetPrivateProfileInt(section,"RFDamageRateToRF",0,path);
	#endif

	this->m_ReflectDamageRateTo[CLASS_DW][CLASS_DW] = GetPrivateProfileInt(section,"DWReflectDamageRateToDW",0,path);
	this->m_ReflectDamageRateTo[CLASS_DW][CLASS_DK] = GetPrivateProfileInt(section,"DWReflectDamageRateToDK",0,path);
	this->m_ReflectDamageRateTo[CLASS_DW][CLASS_FE] = GetPrivateProfileInt(section,"DWReflectDamageRateToFE",0,path);
	this->m_ReflectDamageRateTo[CLASS_DW][CLASS_MG] = GetPrivateProfileInt(section,"DWReflectDamageRateToMG",0,path);
	this->m_ReflectDamageRateTo[CLASS_DW][CLASS_DL] = GetPrivateProfileInt(section,"DWReflectDamageRateToDL",0,path);
	#if(GAMESERVER_UPDATE>=401)
	this->m_ReflectDamageRateTo[CLASS_DW][CLASS_SU] = GetPrivateProfileInt(section,"DWReflectDamageRateToSU",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_ReflectDamageRateTo[CLASS_DW][CLASS_RF] = GetPrivateProfileInt(section,"DWReflectDamageRateToRF",0,path);
	#endif

	this->m_ReflectDamageRateTo[CLASS_DK][CLASS_DW] = GetPrivateProfileInt(section,"DKReflectDamageRateToDW",0,path);
	this->m_ReflectDamageRateTo[CLASS_DK][CLASS_DK] = GetPrivateProfileInt(section,"DKReflectDamageRateToDK",0,path);
	this->m_ReflectDamageRateTo[CLASS_DK][CLASS_FE] = GetPrivateProfileInt(section,"DKReflectDamageRateToFE",0,path);
	this->m_ReflectDamageRateTo[CLASS_DK][CLASS_MG] = GetPrivateProfileInt(section,"DKReflectDamageRateToMG",0,path);
	this->m_ReflectDamageRateTo[CLASS_DK][CLASS_DL] = GetPrivateProfileInt(section,"DKReflectDamageRateToDL",0,path);
	#if(GAMESERVER_UPDATE>=401)
	this->m_ReflectDamageRateTo[CLASS_DK][CLASS_SU] = GetPrivateProfileInt(section,"DKReflectDamageRateToSU",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_ReflectDamageRateTo[CLASS_DK][CLASS_RF] = GetPrivateProfileInt(section,"DKReflectDamageRateToRF",0,path);
	#endif

	this->m_ReflectDamageRateTo[CLASS_FE][CLASS_DW] = GetPrivateProfileInt(section,"FEReflectDamageRateToDW",0,path);
	this->m_ReflectDamageRateTo[CLASS_FE][CLASS_DK] = GetPrivateProfileInt(section,"FEReflectDamageRateToDK",0,path);
	this->m_ReflectDamageRateTo[CLASS_FE][CLASS_FE] = GetPrivateProfileInt(section,"FEReflectDamageRateToFE",0,path);
	this->m_ReflectDamageRateTo[CLASS_FE][CLASS_MG] = GetPrivateProfileInt(section,"FEReflectDamageRateToMG",0,path);
	this->m_ReflectDamageRateTo[CLASS_FE][CLASS_DL] = GetPrivateProfileInt(section,"FEReflectDamageRateToDL",0,path);
	#if(GAMESERVER_UPDATE>=401)
	this->m_ReflectDamageRateTo[CLASS_FE][CLASS_SU] = GetPrivateProfileInt(section,"FEReflectDamageRateToSU",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_ReflectDamageRateTo[CLASS_FE][CLASS_RF] = GetPrivateProfileInt(section,"FEReflectDamageRateToRF",0,path);
	#endif

	this->m_ReflectDamageRateTo[CLASS_MG][CLASS_DW] = GetPrivateProfileInt(section,"MGReflectDamageRateToDW",0,path);
	this->m_ReflectDamageRateTo[CLASS_MG][CLASS_DK] = GetPrivateProfileInt(section,"MGReflectDamageRateToDK",0,path);
	this->m_ReflectDamageRateTo[CLASS_MG][CLASS_FE] = GetPrivateProfileInt(section,"MGReflectDamageRateToFE",0,path);
	this->m_ReflectDamageRateTo[CLASS_MG][CLASS_MG] = GetPrivateProfileInt(section,"MGReflectDamageRateToMG",0,path);
	this->m_ReflectDamageRateTo[CLASS_MG][CLASS_DL] = GetPrivateProfileInt(section,"MGReflectDamageRateToDL",0,path);
	#if(GAMESERVER_UPDATE>=401)
	this->m_ReflectDamageRateTo[CLASS_MG][CLASS_SU] = GetPrivateProfileInt(section,"MGReflectDamageRateToSU",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_ReflectDamageRateTo[CLASS_MG][CLASS_RF] = GetPrivateProfileInt(section,"MGReflectDamageRateToRF",0,path);
	#endif

	this->m_ReflectDamageRateTo[CLASS_DL][CLASS_DW] = GetPrivateProfileInt(section,"DLReflectDamageRateToDW",0,path);
	this->m_ReflectDamageRateTo[CLASS_DL][CLASS_DK] = GetPrivateProfileInt(section,"DLReflectDamageRateToDK",0,path);
	this->m_ReflectDamageRateTo[CLASS_DL][CLASS_FE] = GetPrivateProfileInt(section,"DLReflectDamageRateToFE",0,path);
	this->m_ReflectDamageRateTo[CLASS_DL][CLASS_MG] = GetPrivateProfileInt(section,"DLReflectDamageRateToMG",0,path);
	this->m_ReflectDamageRateTo[CLASS_DL][CLASS_DL] = GetPrivateProfileInt(section,"DLReflectDamageRateToDL",0,path);
	#if(GAMESERVER_UPDATE>=401)
	this->m_ReflectDamageRateTo[CLASS_DL][CLASS_SU] = GetPrivateProfileInt(section,"DLReflectDamageRateToSU",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_ReflectDamageRateTo[CLASS_DL][CLASS_RF] = GetPrivateProfileInt(section,"DLReflectDamageRateToRF",0,path);
	#endif

	#if(GAMESERVER_UPDATE>=401)
	this->m_ReflectDamageRateTo[CLASS_SU][CLASS_DW] = GetPrivateProfileInt(section,"SUReflectDamageRateToDW",0,path);
	this->m_ReflectDamageRateTo[CLASS_SU][CLASS_DK] = GetPrivateProfileInt(section,"SUReflectDamageRateToDK",0,path);
	this->m_ReflectDamageRateTo[CLASS_SU][CLASS_FE] = GetPrivateProfileInt(section,"SUReflectDamageRateToFE",0,path);
	this->m_ReflectDamageRateTo[CLASS_SU][CLASS_MG] = GetPrivateProfileInt(section,"SUReflectDamageRateToMG",0,path);
	this->m_ReflectDamageRateTo[CLASS_SU][CLASS_DL] = GetPrivateProfileInt(section,"SUReflectDamageRateToDL",0,path);
	this->m_ReflectDamageRateTo[CLASS_SU][CLASS_SU] = GetPrivateProfileInt(section,"SUReflectDamageRateToSU",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_ReflectDamageRateTo[CLASS_SU][CLASS_RF] = GetPrivateProfileInt(section,"SUReflectDamageRateToRF",0,path);
	#endif

	#if(GAMESERVER_UPDATE>=601)
	this->m_ReflectDamageRateTo[CLASS_RF][CLASS_DW] = GetPrivateProfileInt(section,"RFReflectDamageRateToDW",0,path);
	this->m_ReflectDamageRateTo[CLASS_RF][CLASS_DK] = GetPrivateProfileInt(section,"RFReflectDamageRateToDK",0,path);
	this->m_ReflectDamageRateTo[CLASS_RF][CLASS_FE] = GetPrivateProfileInt(section,"RFReflectDamageRateToFE",0,path);
	this->m_ReflectDamageRateTo[CLASS_RF][CLASS_MG] = GetPrivateProfileInt(section,"RFReflectDamageRateToMG",0,path);
	this->m_ReflectDamageRateTo[CLASS_RF][CLASS_DL] = GetPrivateProfileInt(section,"RFReflectDamageRateToDL",0,path);
	this->m_ReflectDamageRateTo[CLASS_RF][CLASS_SU] = GetPrivateProfileInt(section,"RFReflectDamageRateToSU",0,path);
	this->m_ReflectDamageRateTo[CLASS_RF][CLASS_RF] = GetPrivateProfileInt(section,"RFReflectDamageRateToRF",0,path);
	#endif

	this->m_DamageStuckRate[CLASS_DW] = GetPrivateProfileInt(section,"DWDamageStuckRate",0,path);
	this->m_DamageStuckRate[CLASS_DK] = GetPrivateProfileInt(section,"DKDamageStuckRate",0,path);
	this->m_DamageStuckRate[CLASS_FE] = GetPrivateProfileInt(section,"FEDamageStuckRate",0,path);
	this->m_DamageStuckRate[CLASS_MG] = GetPrivateProfileInt(section,"MGDamageStuckRate",0,path);
	this->m_DamageStuckRate[CLASS_DL] = GetPrivateProfileInt(section,"DLDamageStuckRate",0,path);
	#if(GAMESERVER_UPDATE>=401)
	this->m_DamageStuckRate[CLASS_SU] = GetPrivateProfileInt(section,"SUDamageStuckRate",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_DamageStuckRate[CLASS_RF] = GetPrivateProfileInt(section,"RFDamageStuckRate",0,path);
	#endif

	this->m_DamageStuckOnPetUniria = GetPrivateProfileInt(section,"DamageStuckOnPetUniria",0,path);
	this->m_DamageStuckOnPetDinorant = GetPrivateProfileInt(section,"DamageStuckOnPetDinorant",0,path);
	this->m_DamageStuckOnPetDarkHorse = GetPrivateProfileInt(section,"DamageStuckOnPetDarkHorse",0,path);
	#if(GAMESERVER_UPDATE>=201)
	this->m_DamageStuckOnPetFenrir = GetPrivateProfileInt(section,"DamageStuckOnPetFenrir",0,path);
	#endif

	this->m_DKDamageMultiplierConstA = GetPrivateProfileInt(section,"DKDamageMultiplierConstA",0,path);

	this->m_DLDamageMultiplierConstA = GetPrivateProfileInt(section,"DLDamageMultiplierConstA",0,path);

	#if(GAMESERVER_UPDATE>=601)

	this->m_RFDamageMultiplierConstA = GetPrivateProfileInt(section,"RFDamageMultiplierConstA",0,path);
	this->m_RFDamageMultiplierConstB = GetPrivateProfileInt(section,"RFDamageMultiplierConstB",0,path);
	this->m_RFDamageMultiplierConstC = GetPrivateProfileInt(section,"RFDamageMultiplierConstC",0,path);

	#endif

	this->m_DKDamageMultiplierMaxRate = GetPrivateProfileInt(section,"DKDamageMultiplierMaxRate",0,path);

	this->m_DLDamageMultiplierMaxRate = GetPrivateProfileInt(section,"DLDamageMultiplierMaxRate",0,path);

	#if(GAMESERVER_UPDATE>=601)

	this->m_RFDamageMultiplierMaxRate = GetPrivateProfileInt(section,"RFDamageMultiplierMaxRate",0,path);

	#endif

	this->m_DarkSpiritRangeAttackRate = GetPrivateProfileInt(section,"DarkSpiritRangeAttackRate",0,path);

	this->m_DarkSpiritCriticalDamageRate = GetPrivateProfileInt(section,"DarkSpiritCriticalDamageRate",0,path);

	this->m_DarkSpiritExcellentDamageRate = GetPrivateProfileInt(section,"DarkSpiritExcellentDamageRate",0,path);

	this->m_DarkSpiritAttackDamageMinConstA = GetPrivateProfileInt(section,"DarkSpiritAttackDamageMinConstA",0,path);
	this->m_DarkSpiritAttackDamageMinConstB = GetPrivateProfileInt(section,"DarkSpiritAttackDamageMinConstB",0,path);
	this->m_DarkSpiritAttackDamageMinConstC = GetPrivateProfileInt(section,"DarkSpiritAttackDamageMinConstC",0,path);

	this->m_DarkSpiritAttackDamageMaxConstA = GetPrivateProfileInt(section,"DarkSpiritAttackDamageMaxConstA",0,path);
	this->m_DarkSpiritAttackDamageMaxConstB = GetPrivateProfileInt(section,"DarkSpiritAttackDamageMaxConstB",0,path);
	this->m_DarkSpiritAttackDamageMaxConstC = GetPrivateProfileInt(section,"DarkSpiritAttackDamageMaxConstC",0,path);

	this->m_DarkSpiritAttackSpeedConstA = GetPrivateProfileInt(section,"DarkSpiritAttackSpeedConstA",0,path);
	this->m_DarkSpiritAttackSpeedConstB = GetPrivateProfileInt(section,"DarkSpiritAttackSpeedConstB",0,path);
	this->m_DarkSpiritAttackSpeedConstC = GetPrivateProfileInt(section,"DarkSpiritAttackSpeedConstC",0,path);
	this->m_DarkSpiritAttackSpeedConstD = GetPrivateProfileInt(section,"DarkSpiritAttackSpeedConstD",0,path);

	this->m_DarkSpiritAttackSuccessRateConstA = GetPrivateProfileInt(section,"DarkSpiritAttackSuccessRateConstA",0,path);
	this->m_DarkSpiritAttackSuccessRateConstB = GetPrivateProfileInt(section,"DarkSpiritAttackSuccessRateConstB",0,path);
	this->m_DarkSpiritAttackSuccessRateConstC = GetPrivateProfileInt(section,"DarkSpiritAttackSuccessRateConstC",0,path);

	this->m_ComboDamageConstA[CLASS_DW] = GetPrivateProfileInt(section,"DWComboDamageConstA",0,path);
	this->m_ComboDamageConstB[CLASS_DW] = GetPrivateProfileInt(section,"DWComboDamageConstB",0,path);
	this->m_ComboDamageConstC[CLASS_DW] = GetPrivateProfileInt(section,"DWComboDamageConstC",0,path);
	this->m_ComboDamageConstA[CLASS_DK] = GetPrivateProfileInt(section,"DKComboDamageConstA",0,path);
	this->m_ComboDamageConstB[CLASS_DK] = GetPrivateProfileInt(section,"DKComboDamageConstB",0,path);
	this->m_ComboDamageConstC[CLASS_DK] = GetPrivateProfileInt(section,"DKComboDamageConstC",0,path);
	this->m_ComboDamageConstA[CLASS_FE] = GetPrivateProfileInt(section,"FEComboDamageConstA",0,path);
	this->m_ComboDamageConstB[CLASS_FE] = GetPrivateProfileInt(section,"FEComboDamageConstB",0,path);
	this->m_ComboDamageConstC[CLASS_FE] = GetPrivateProfileInt(section,"FEComboDamageConstC",0,path);
	this->m_ComboDamageConstA[CLASS_MG] = GetPrivateProfileInt(section,"MGComboDamageConstA",0,path);
	this->m_ComboDamageConstB[CLASS_MG] = GetPrivateProfileInt(section,"MGComboDamageConstB",0,path);
	this->m_ComboDamageConstC[CLASS_MG] = GetPrivateProfileInt(section,"MGComboDamageConstC",0,path);
	this->m_ComboDamageConstA[CLASS_DL] = GetPrivateProfileInt(section,"DLComboDamageConstA",0,path);
	this->m_ComboDamageConstB[CLASS_DL] = GetPrivateProfileInt(section,"DLComboDamageConstB",0,path);
	this->m_ComboDamageConstC[CLASS_DL] = GetPrivateProfileInt(section,"DLComboDamageConstC",0,path);
	#if(GAMESERVER_UPDATE>=401)
	this->m_ComboDamageConstA[CLASS_SU] = GetPrivateProfileInt(section,"SUComboDamageConstA",0,path);
	this->m_ComboDamageConstB[CLASS_SU] = GetPrivateProfileInt(section,"SUComboDamageConstB",0,path);
	this->m_ComboDamageConstC[CLASS_SU] = GetPrivateProfileInt(section,"SUComboDamageConstC",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_ComboDamageConstA[CLASS_RF] = GetPrivateProfileInt(section,"RFComboDamageConstA",0,path);
	this->m_ComboDamageConstB[CLASS_RF] = GetPrivateProfileInt(section,"RFComboDamageConstB",0,path);
	this->m_ComboDamageConstC[CLASS_RF] = GetPrivateProfileInt(section,"RFComboDamageConstC",0,path);
	#endif

	this->m_EarthquakeDamageConstA = GetPrivateProfileInt(section,"EarthquakeDamageConstA",0,path);
	this->m_EarthquakeDamageConstB = GetPrivateProfileInt(section,"EarthquakeDamageConstB",0,path);
	this->m_EarthquakeDamageConstC = GetPrivateProfileInt(section,"EarthquakeDamageConstC",0,path);

	this->m_ElectricSparkDamageConstA = GetPrivateProfileInt(section,"ElectricSparkDamageConstA",0,path);
	this->m_ElectricSparkDamageConstB = GetPrivateProfileInt(section,"ElectricSparkDamageConstB",0,path);

	this->m_DLSkillDamageConstA = GetPrivateProfileInt(section,"DLSkillDamageConstA",0,path);
	this->m_DLSkillDamageConstB = GetPrivateProfileInt(section,"DLSkillDamageConstB",0,path);

	this->m_NovaDamageConstA = GetPrivateProfileInt(section,"NovaDamageConstA",0,path);
	this->m_NovaDamageConstB = GetPrivateProfileInt(section,"NovaDamageConstB",0,path);
	this->m_NovaDamageConstC = GetPrivateProfileInt(section,"NovaDamageConstC",0,path);

	this->m_HPRecoveryRate[CLASS_DW] = GetPrivateProfileInt(section,"DWHPRecoveryRate",0,path);
	this->m_HPRecoveryRate[CLASS_DK] = GetPrivateProfileInt(section,"DKHPRecoveryRate",0,path);
	this->m_HPRecoveryRate[CLASS_FE] = GetPrivateProfileInt(section,"FEHPRecoveryRate",0,path);
	this->m_HPRecoveryRate[CLASS_MG] = GetPrivateProfileInt(section,"MGHPRecoveryRate",0,path);
	this->m_HPRecoveryRate[CLASS_DL] = GetPrivateProfileInt(section,"DLHPRecoveryRate",0,path);
	#if(GAMESERVER_UPDATE>=401)
	this->m_HPRecoveryRate[CLASS_SU] = GetPrivateProfileInt(section,"SUHPRecoveryRate",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_HPRecoveryRate[CLASS_RF] = GetPrivateProfileInt(section,"RFHPRecoveryRate",0,path);
	#endif

	this->m_MPRecoveryRate[CLASS_DW] = GetPrivateProfileInt(section,"DWMPRecoveryRate",0,path);
	this->m_MPRecoveryRate[CLASS_DK] = GetPrivateProfileInt(section,"DKMPRecoveryRate",0,path);
	this->m_MPRecoveryRate[CLASS_FE] = GetPrivateProfileInt(section,"FEMPRecoveryRate",0,path);
	this->m_MPRecoveryRate[CLASS_MG] = GetPrivateProfileInt(section,"MGMPRecoveryRate",0,path);
	this->m_MPRecoveryRate[CLASS_DL] = GetPrivateProfileInt(section,"DLMPRecoveryRate",0,path);
	#if(GAMESERVER_UPDATE>=401)
	this->m_MPRecoveryRate[CLASS_SU] = GetPrivateProfileInt(section,"SUMPRecoveryRate",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_MPRecoveryRate[CLASS_RF] = GetPrivateProfileInt(section,"RFMPRecoveryRate",0,path);
	#endif

	this->m_BPRecoveryRate[CLASS_DW] = GetPrivateProfileInt(section,"DWBPRecoveryRate",0,path);
	this->m_BPRecoveryRate[CLASS_DK] = GetPrivateProfileInt(section,"DKBPRecoveryRate",0,path);
	this->m_BPRecoveryRate[CLASS_FE] = GetPrivateProfileInt(section,"FEBPRecoveryRate",0,path);
	this->m_BPRecoveryRate[CLASS_MG] = GetPrivateProfileInt(section,"MGBPRecoveryRate",0,path);
	this->m_BPRecoveryRate[CLASS_DL] = GetPrivateProfileInt(section,"DLBPRecoveryRate",0,path);
	#if(GAMESERVER_UPDATE>=401)
	this->m_BPRecoveryRate[CLASS_SU] = GetPrivateProfileInt(section,"SUBPRecoveryRate",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_BPRecoveryRate[CLASS_RF] = GetPrivateProfileInt(section,"RFBPRecoveryRate",0,path);
	#endif

	#if(GAMESERVER_UPDATE>=201)
	this->m_SDRecoveryRate[CLASS_DW] = GetPrivateProfileInt(section,"DWSDRecoveryRate",0,path);
	this->m_SDRecoveryRate[CLASS_DK] = GetPrivateProfileInt(section,"DKSDRecoveryRate",0,path);
	this->m_SDRecoveryRate[CLASS_FE] = GetPrivateProfileInt(section,"FESDRecoveryRate",0,path);
	this->m_SDRecoveryRate[CLASS_MG] = GetPrivateProfileInt(section,"MGSDRecoveryRate",0,path);
	this->m_SDRecoveryRate[CLASS_DL] = GetPrivateProfileInt(section,"DLSDRecoveryRate",0,path);
	#if(GAMESERVER_UPDATE>=401)
	this->m_SDRecoveryRate[CLASS_SU] = GetPrivateProfileInt(section,"SUSDRecoveryRate",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_SDRecoveryRate[CLASS_RF] = GetPrivateProfileInt(section,"RFSDRecoveryRate",0,path);
	#endif
	#endif

	#if(GAMESERVER_UPDATE>=201)

	this->m_DWPlasmaStormDamageConstA = GetPrivateProfileInt(section,"DWPlasmaStormDamageConstA",0,path);
	this->m_DWPlasmaStormDamageConstB = GetPrivateProfileInt(section,"DWPlasmaStormDamageConstB",0,path);
	this->m_DWPlasmaStormDamageConstC = GetPrivateProfileInt(section,"DWPlasmaStormDamageConstC",0,path);
	this->m_DWPlasmaStormDamageConstD = GetPrivateProfileInt(section,"DWPlasmaStormDamageConstD",0,path);

	this->m_DKPlasmaStormDamageConstA = GetPrivateProfileInt(section,"DKPlasmaStormDamageConstA",0,path);
	this->m_DKPlasmaStormDamageConstB = GetPrivateProfileInt(section,"DKPlasmaStormDamageConstB",0,path);
	this->m_DKPlasmaStormDamageConstC = GetPrivateProfileInt(section,"DKPlasmaStormDamageConstC",0,path);
	this->m_DKPlasmaStormDamageConstD = GetPrivateProfileInt(section,"DKPlasmaStormDamageConstD",0,path);

	this->m_FEPlasmaStormDamageConstA = GetPrivateProfileInt(section,"FEPlasmaStormDamageConstA",0,path);
	this->m_FEPlasmaStormDamageConstB = GetPrivateProfileInt(section,"FEPlasmaStormDamageConstB",0,path);
	this->m_FEPlasmaStormDamageConstC = GetPrivateProfileInt(section,"FEPlasmaStormDamageConstC",0,path);
	this->m_FEPlasmaStormDamageConstD = GetPrivateProfileInt(section,"FEPlasmaStormDamageConstD",0,path);

	this->m_MGPlasmaStormDamageConstA = GetPrivateProfileInt(section,"MGPlasmaStormDamageConstA",0,path);
	this->m_MGPlasmaStormDamageConstB = GetPrivateProfileInt(section,"MGPlasmaStormDamageConstB",0,path);
	this->m_MGPlasmaStormDamageConstC = GetPrivateProfileInt(section,"MGPlasmaStormDamageConstC",0,path);
	this->m_MGPlasmaStormDamageConstD = GetPrivateProfileInt(section,"MGPlasmaStormDamageConstD",0,path);

	this->m_DLPlasmaStormDamageConstA = GetPrivateProfileInt(section,"DLPlasmaStormDamageConstA",0,path);
	this->m_DLPlasmaStormDamageConstB = GetPrivateProfileInt(section,"DLPlasmaStormDamageConstB",0,path);
	this->m_DLPlasmaStormDamageConstC = GetPrivateProfileInt(section,"DLPlasmaStormDamageConstC",0,path);
	this->m_DLPlasmaStormDamageConstD = GetPrivateProfileInt(section,"DLPlasmaStormDamageConstD",0,path);
	this->m_DLPlasmaStormDamageConstE = GetPrivateProfileInt(section,"DLPlasmaStormDamageConstE",0,path);

	#if(GAMESERVER_UPDATE>=401)

	this->m_SUPlasmaStormDamageConstA = GetPrivateProfileInt(section,"SUPlasmaStormDamageConstA",0,path);
	this->m_SUPlasmaStormDamageConstB = GetPrivateProfileInt(section,"SUPlasmaStormDamageConstB",0,path);
	this->m_SUPlasmaStormDamageConstC = GetPrivateProfileInt(section,"SUPlasmaStormDamageConstC",0,path);
	this->m_SUPlasmaStormDamageConstD = GetPrivateProfileInt(section,"SUPlasmaStormDamageConstD",0,path);

	#endif

	#if(GAMESERVER_UPDATE>=601)

	this->m_RFPlasmaStormDamageConstA = GetPrivateProfileInt(section,"RFPlasmaStormDamageConstA",0,path);
	this->m_RFPlasmaStormDamageConstB = GetPrivateProfileInt(section,"RFPlasmaStormDamageConstB",0,path);
	this->m_RFPlasmaStormDamageConstC = GetPrivateProfileInt(section,"RFPlasmaStormDamageConstC",0,path);
	this->m_RFPlasmaStormDamageConstD = GetPrivateProfileInt(section,"RFPlasmaStormDamageConstD",0,path);

	#endif

	#endif

	this->m_DWPhysiDamageMinConstA = GetPrivateProfileInt(section,"DWPhysiDamageMinConstA",0,path);
	this->m_DWPhysiDamageMaxConstA = GetPrivateProfileInt(section,"DWPhysiDamageMaxConstA",0,path);
	
	this->m_DWMagicDamageMinConstA = GetPrivateProfileInt(section,"DWMagicDamageMinConstA",0,path);
	this->m_DWMagicDamageMaxConstA = GetPrivateProfileInt(section,"DWMagicDamageMaxConstA",0,path);

	this->m_DKPhysiDamageMinConstA = GetPrivateProfileInt(section,"DKPhysiDamageMinConstA",0,path);
	this->m_DKPhysiDamageMaxConstA = GetPrivateProfileInt(section,"DKPhysiDamageMaxConstA",0,path);
	
	this->m_DKMagicDamageMinConstA = GetPrivateProfileInt(section,"DKMagicDamageMinConstA",0,path);
	this->m_DKMagicDamageMaxConstA = GetPrivateProfileInt(section,"DKMagicDamageMaxConstA",0,path);

	this->m_FEPhysiDamageMinConstA = GetPrivateProfileInt(section,"FEPhysiDamageMinConstA",0,path);
	this->m_FEPhysiDamageMaxConstA = GetPrivateProfileInt(section,"FEPhysiDamageMaxConstA",0,path);

	this->m_FEPhysiDamageMinBowConstA = GetPrivateProfileInt(section,"FEPhysiDamageMinBowConstA",0,path);
	this->m_FEPhysiDamageMinBowConstB = GetPrivateProfileInt(section,"FEPhysiDamageMinBowConstB",0,path);
	
	this->m_FEPhysiDamageMaxBowConstA = GetPrivateProfileInt(section,"FEPhysiDamageMaxBowConstA",0,path);
	this->m_FEPhysiDamageMaxBowConstB = GetPrivateProfileInt(section,"FEPhysiDamageMaxBowConstB",0,path);

	this->m_FEMagicDamageMinConstA = GetPrivateProfileInt(section,"FEMagicDamageMinConstA",0,path);
	this->m_FEMagicDamageMaxConstA = GetPrivateProfileInt(section,"FEMagicDamageMaxConstA",0,path);

	this->m_MGPhysiDamageMinConstA = GetPrivateProfileInt(section,"MGPhysiDamageMinConstA",0,path);
	this->m_MGPhysiDamageMinConstB = GetPrivateProfileInt(section,"MGPhysiDamageMinConstB",0,path);

	this->m_MGPhysiDamageMaxConstA = GetPrivateProfileInt(section,"MGPhysiDamageMaxConstA",0,path);
	this->m_MGPhysiDamageMaxConstB = GetPrivateProfileInt(section,"MGPhysiDamageMaxConstB",0,path);

	this->m_MGMagicDamageMinConstA = GetPrivateProfileInt(section,"MGMagicDamageMinConstA",0,path);
	this->m_MGMagicDamageMaxConstA = GetPrivateProfileInt(section,"MGMagicDamageMaxConstA",0,path);

	this->m_DLPhysiDamageMinConstA = GetPrivateProfileInt(section,"DLPhysiDamageMinConstA",0,path);
	this->m_DLPhysiDamageMinConstB = GetPrivateProfileInt(section,"DLPhysiDamageMinConstB",0,path);

	this->m_DLPhysiDamageMaxConstA = GetPrivateProfileInt(section,"DLPhysiDamageMaxConstA",0,path);
	this->m_DLPhysiDamageMaxConstB = GetPrivateProfileInt(section,"DLPhysiDamageMaxConstB",0,path);

	this->m_DLMagicDamageMinConstA = GetPrivateProfileInt(section,"DLMagicDamageMinConstA",0,path);
	this->m_DLMagicDamageMaxConstA = GetPrivateProfileInt(section,"DLMagicDamageMaxConstA",0,path);

	#if(GAMESERVER_UPDATE>=401)

	this->m_SUPhysiDamageMinConstA = GetPrivateProfileInt(section,"SUPhysiDamageMinConstA",0,path);
	this->m_SUPhysiDamageMaxConstA = GetPrivateProfileInt(section,"SUPhysiDamageMaxConstA",0,path);

	this->m_SUMagicDamageMinConstA = GetPrivateProfileInt(section,"SUMagicDamageMinConstA",0,path);
	this->m_SUMagicDamageMaxConstA = GetPrivateProfileInt(section,"SUMagicDamageMaxConstA",0,path);

	#endif

	#if(GAMESERVER_UPDATE>=601)

	this->m_RFPhysiDamageMinConstA = GetPrivateProfileInt(section,"RFPhysiDamageMinConstA",0,path);
	this->m_RFPhysiDamageMinConstB = GetPrivateProfileInt(section,"RFPhysiDamageMinConstB",0,path);

	this->m_RFPhysiDamageMaxConstA = GetPrivateProfileInt(section,"RFPhysiDamageMaxConstA",0,path);
	this->m_RFPhysiDamageMaxConstB = GetPrivateProfileInt(section,"RFPhysiDamageMaxConstB",0,path);

	this->m_RFMagicDamageMinConstA = GetPrivateProfileInt(section,"RFMagicDamageMinConstA",0,path);
	this->m_RFMagicDamageMaxConstA = GetPrivateProfileInt(section,"RFMagicDamageMaxConstA",0,path);

	#endif

	this->m_DWAttackSuccessRateConstA = GetPrivateProfileInt(section,"DWAttackSuccessRateConstA",0,path);
	this->m_DWAttackSuccessRateConstB = GetPrivateProfileInt(section,"DWAttackSuccessRateConstB",0,path);
	this->m_DWAttackSuccessRateConstC = GetPrivateProfileInt(section,"DWAttackSuccessRateConstC",0,path);
	this->m_DWAttackSuccessRateConstD = GetPrivateProfileInt(section,"DWAttackSuccessRateConstD",0,path);

	this->m_DKAttackSuccessRateConstA = GetPrivateProfileInt(section,"DKAttackSuccessRateConstA",0,path);
	this->m_DKAttackSuccessRateConstB = GetPrivateProfileInt(section,"DKAttackSuccessRateConstB",0,path);
	this->m_DKAttackSuccessRateConstC = GetPrivateProfileInt(section,"DKAttackSuccessRateConstC",0,path);
	this->m_DKAttackSuccessRateConstD = GetPrivateProfileInt(section,"DKAttackSuccessRateConstD",0,path);

	this->m_FEAttackSuccessRateConstA = GetPrivateProfileInt(section,"FEAttackSuccessRateConstA",0,path);
	this->m_FEAttackSuccessRateConstB = GetPrivateProfileInt(section,"FEAttackSuccessRateConstB",0,path);
	this->m_FEAttackSuccessRateConstC = GetPrivateProfileInt(section,"FEAttackSuccessRateConstC",0,path);
	this->m_FEAttackSuccessRateConstD = GetPrivateProfileInt(section,"FEAttackSuccessRateConstD",0,path);

	this->m_MGAttackSuccessRateConstA = GetPrivateProfileInt(section,"MGAttackSuccessRateConstA",0,path);
	this->m_MGAttackSuccessRateConstB = GetPrivateProfileInt(section,"MGAttackSuccessRateConstB",0,path);
	this->m_MGAttackSuccessRateConstC = GetPrivateProfileInt(section,"MGAttackSuccessRateConstC",0,path);
	this->m_MGAttackSuccessRateConstD = GetPrivateProfileInt(section,"MGAttackSuccessRateConstD",0,path);

	this->m_DLAttackSuccessRateConstA = GetPrivateProfileInt(section,"DLAttackSuccessRateConstA",0,path);
	this->m_DLAttackSuccessRateConstB = GetPrivateProfileInt(section,"DLAttackSuccessRateConstB",0,path);
	this->m_DLAttackSuccessRateConstC = GetPrivateProfileInt(section,"DLAttackSuccessRateConstC",0,path);
	this->m_DLAttackSuccessRateConstD = GetPrivateProfileInt(section,"DLAttackSuccessRateConstD",0,path);
	this->m_DLAttackSuccessRateConstE = GetPrivateProfileInt(section,"DLAttackSuccessRateConstE",0,path);

	#if(GAMESERVER_UPDATE>=401)

	this->m_SUAttackSuccessRateConstA = GetPrivateProfileInt(section,"SUAttackSuccessRateConstA",0,path);
	this->m_SUAttackSuccessRateConstB = GetPrivateProfileInt(section,"SUAttackSuccessRateConstB",0,path);
	this->m_SUAttackSuccessRateConstC = GetPrivateProfileInt(section,"SUAttackSuccessRateConstC",0,path);
	this->m_SUAttackSuccessRateConstD = GetPrivateProfileInt(section,"SUAttackSuccessRateConstD",0,path);

	#endif

	#if(GAMESERVER_UPDATE>=601)

	this->m_RFAttackSuccessRateConstA = GetPrivateProfileInt(section,"RFAttackSuccessRateConstA",0,path);
	this->m_RFAttackSuccessRateConstB = GetPrivateProfileInt(section,"RFAttackSuccessRateConstB",0,path);
	this->m_RFAttackSuccessRateConstC = GetPrivateProfileInt(section,"RFAttackSuccessRateConstC",0,path);
	this->m_RFAttackSuccessRateConstD = GetPrivateProfileInt(section,"RFAttackSuccessRateConstD",0,path);

	#endif

	this->m_DWAttackSuccessRatePvPConstA = GetPrivateProfileInt(section,"DWAttackSuccessRatePvPConstA",0,path);
	this->m_DWAttackSuccessRatePvPConstB = GetPrivateProfileInt(section,"DWAttackSuccessRatePvPConstB",0,path);
	this->m_DWAttackSuccessRatePvPConstC = GetPrivateProfileInt(section,"DWAttackSuccessRatePvPConstC",0,path);
	this->m_DWAttackSuccessRatePvPConstD = GetPrivateProfileInt(section,"DWAttackSuccessRatePvPConstD",0,path);

	this->m_DKAttackSuccessRatePvPConstA = GetPrivateProfileInt(section,"DKAttackSuccessRatePvPConstA",0,path);
	this->m_DKAttackSuccessRatePvPConstB = GetPrivateProfileInt(section,"DKAttackSuccessRatePvPConstB",0,path);
	this->m_DKAttackSuccessRatePvPConstC = GetPrivateProfileInt(section,"DKAttackSuccessRatePvPConstC",0,path);
	this->m_DKAttackSuccessRatePvPConstD = GetPrivateProfileInt(section,"DKAttackSuccessRatePvPConstD",0,path);

	this->m_FEAttackSuccessRatePvPConstA = GetPrivateProfileInt(section,"FEAttackSuccessRatePvPConstA",0,path);
	this->m_FEAttackSuccessRatePvPConstB = GetPrivateProfileInt(section,"FEAttackSuccessRatePvPConstB",0,path);
	this->m_FEAttackSuccessRatePvPConstC = GetPrivateProfileInt(section,"FEAttackSuccessRatePvPConstC",0,path);
	this->m_FEAttackSuccessRatePvPConstD = GetPrivateProfileInt(section,"FEAttackSuccessRatePvPConstD",0,path);

	this->m_MGAttackSuccessRatePvPConstA = GetPrivateProfileInt(section,"MGAttackSuccessRatePvPConstA",0,path);
	this->m_MGAttackSuccessRatePvPConstB = GetPrivateProfileInt(section,"MGAttackSuccessRatePvPConstB",0,path);
	this->m_MGAttackSuccessRatePvPConstC = GetPrivateProfileInt(section,"MGAttackSuccessRatePvPConstC",0,path);
	this->m_MGAttackSuccessRatePvPConstD = GetPrivateProfileInt(section,"MGAttackSuccessRatePvPConstD",0,path);

	this->m_DLAttackSuccessRatePvPConstA = GetPrivateProfileInt(section,"DLAttackSuccessRatePvPConstA",0,path);
	this->m_DLAttackSuccessRatePvPConstB = GetPrivateProfileInt(section,"DLAttackSuccessRatePvPConstB",0,path);
	this->m_DLAttackSuccessRatePvPConstC = GetPrivateProfileInt(section,"DLAttackSuccessRatePvPConstC",0,path);
	this->m_DLAttackSuccessRatePvPConstD = GetPrivateProfileInt(section,"DLAttackSuccessRatePvPConstD",0,path);

	#if(GAMESERVER_UPDATE>=401)

	this->m_SUAttackSuccessRatePvPConstA = GetPrivateProfileInt(section,"SUAttackSuccessRatePvPConstA",0,path);
	this->m_SUAttackSuccessRatePvPConstB = GetPrivateProfileInt(section,"SUAttackSuccessRatePvPConstB",0,path);
	this->m_SUAttackSuccessRatePvPConstC = GetPrivateProfileInt(section,"SUAttackSuccessRatePvPConstC",0,path);
	this->m_SUAttackSuccessRatePvPConstD = GetPrivateProfileInt(section,"SUAttackSuccessRatePvPConstD",0,path);

	#endif

	#if(GAMESERVER_UPDATE>=601)

	this->m_RFAttackSuccessRatePvPConstA = GetPrivateProfileInt(section,"RFAttackSuccessRatePvPConstA",0,path);
	this->m_RFAttackSuccessRatePvPConstB = GetPrivateProfileInt(section,"RFAttackSuccessRatePvPConstB",0,path);
	this->m_RFAttackSuccessRatePvPConstC = GetPrivateProfileInt(section,"RFAttackSuccessRatePvPConstC",0,path);
	this->m_RFAttackSuccessRatePvPConstD = GetPrivateProfileInt(section,"RFAttackSuccessRatePvPConstD",0,path);

	#endif

	this->m_DWPhysiSpeedConstA = GetPrivateProfileInt(section,"DWPhysiSpeedConstA",0,path);
	this->m_DWMagicSpeedConstA = GetPrivateProfileInt(section,"DWMagicSpeedConstA",0,path);

	this->m_DKPhysiSpeedConstA = GetPrivateProfileInt(section,"DKPhysiSpeedConstA",0,path);
	this->m_DKMagicSpeedConstA = GetPrivateProfileInt(section,"DKMagicSpeedConstA",0,path);

	this->m_FEPhysiSpeedConstA = GetPrivateProfileInt(section,"FEPhysiSpeedConstA",0,path);
	this->m_FEMagicSpeedConstA = GetPrivateProfileInt(section,"FEMagicSpeedConstA",0,path);

	this->m_MGPhysiSpeedConstA = GetPrivateProfileInt(section,"MGPhysiSpeedConstA",0,path);
	this->m_MGMagicSpeedConstA = GetPrivateProfileInt(section,"MGMagicSpeedConstA",0,path);

	this->m_DLPhysiSpeedConstA = GetPrivateProfileInt(section,"DLPhysiSpeedConstA",0,path);
	this->m_DLMagicSpeedConstA = GetPrivateProfileInt(section,"DLMagicSpeedConstA",0,path);

	#if(GAMESERVER_UPDATE>=401)

	this->m_SUPhysiSpeedConstA = GetPrivateProfileInt(section,"SUPhysiSpeedConstA",0,path);
	this->m_SUMagicSpeedConstA = GetPrivateProfileInt(section,"SUMagicSpeedConstA",0,path);

	#endif

	#if(GAMESERVER_UPDATE>=601)

	this->m_RFPhysiSpeedConstA = GetPrivateProfileInt(section,"RFPhysiSpeedConstA",0,path);
	this->m_RFMagicSpeedConstA = GetPrivateProfileInt(section,"RFMagicSpeedConstA",0,path);

	#endif

	this->m_DWDefenseSuccessRateConstA = GetPrivateProfileInt(section,"DWDefenseSuccessRateConstA",0,path);
	this->m_DKDefenseSuccessRateConstA = GetPrivateProfileInt(section,"DKDefenseSuccessRateConstA",0,path);

	this->m_FEDefenseSuccessRateConstA = GetPrivateProfileInt(section,"FEDefenseSuccessRateConstA",0,path);

	this->m_MGDefenseSuccessRateConstA = GetPrivateProfileInt(section,"MGDefenseSuccessRateConstA",0,path);

	this->m_DLDefenseSuccessRateConstA = GetPrivateProfileInt(section,"DLDefenseSuccessRateConstA",0,path);

	#if(GAMESERVER_UPDATE>=401)

	this->m_SUDefenseSuccessRateConstA = GetPrivateProfileInt(section,"SUDefenseSuccessRateConstA",0,path);

	#endif

	#if(GAMESERVER_UPDATE>=601)

	this->m_RFDefenseSuccessRateConstA = GetPrivateProfileInt(section,"RFDefenseSuccessRateConstA",0,path);

	#endif

	this->m_DWDefenseSuccessRatePvPConstA = GetPrivateProfileInt(section,"DWDefenseSuccessRatePvPConstA",0,path);
	this->m_DWDefenseSuccessRatePvPConstB = GetPrivateProfileInt(section,"DWDefenseSuccessRatePvPConstB",0,path);
	this->m_DWDefenseSuccessRatePvPConstC = GetPrivateProfileInt(section,"DWDefenseSuccessRatePvPConstC",0,path);

	this->m_DKDefenseSuccessRatePvPConstA = GetPrivateProfileInt(section,"DKDefenseSuccessRatePvPConstA",0,path);
	this->m_DKDefenseSuccessRatePvPConstB = GetPrivateProfileInt(section,"DKDefenseSuccessRatePvPConstB",0,path);
	this->m_DKDefenseSuccessRatePvPConstC = GetPrivateProfileInt(section,"DKDefenseSuccessRatePvPConstC",0,path);

	this->m_FEDefenseSuccessRatePvPConstA = GetPrivateProfileInt(section,"FEDefenseSuccessRatePvPConstA",0,path);
	this->m_FEDefenseSuccessRatePvPConstB = GetPrivateProfileInt(section,"FEDefenseSuccessRatePvPConstB",0,path);
	this->m_FEDefenseSuccessRatePvPConstC = GetPrivateProfileInt(section,"FEDefenseSuccessRatePvPConstC",0,path);

	this->m_MGDefenseSuccessRatePvPConstA = GetPrivateProfileInt(section,"MGDefenseSuccessRatePvPConstA",0,path);
	this->m_MGDefenseSuccessRatePvPConstB = GetPrivateProfileInt(section,"MGDefenseSuccessRatePvPConstB",0,path);
	this->m_MGDefenseSuccessRatePvPConstC = GetPrivateProfileInt(section,"MGDefenseSuccessRatePvPConstC",0,path);

	this->m_DLDefenseSuccessRatePvPConstA = GetPrivateProfileInt(section,"DLDefenseSuccessRatePvPConstA",0,path);
	this->m_DLDefenseSuccessRatePvPConstB = GetPrivateProfileInt(section,"DLDefenseSuccessRatePvPConstB",0,path);
	this->m_DLDefenseSuccessRatePvPConstC = GetPrivateProfileInt(section,"DLDefenseSuccessRatePvPConstC",0,path);

	#if(GAMESERVER_UPDATE>=401)

	this->m_SUDefenseSuccessRatePvPConstA = GetPrivateProfileInt(section,"SUDefenseSuccessRatePvPConstA",0,path);
	this->m_SUDefenseSuccessRatePvPConstB = GetPrivateProfileInt(section,"SUDefenseSuccessRatePvPConstB",0,path);
	this->m_SUDefenseSuccessRatePvPConstC = GetPrivateProfileInt(section,"SUDefenseSuccessRatePvPConstC",0,path);

	#endif

	#if(GAMESERVER_UPDATE>=601)

	this->m_RFDefenseSuccessRatePvPConstA = GetPrivateProfileInt(section,"RFDefenseSuccessRatePvPConstA",0,path);
	this->m_RFDefenseSuccessRatePvPConstB = GetPrivateProfileInt(section,"RFDefenseSuccessRatePvPConstB",0,path);
	this->m_RFDefenseSuccessRatePvPConstC = GetPrivateProfileInt(section,"RFDefenseSuccessRatePvPConstC",0,path);

	#endif

	this->m_DWDefenseConstA = GetPrivateProfileInt(section,"DWDefenseConstA",0,path);
	this->m_DKDefenseConstA = GetPrivateProfileInt(section,"DKDefenseConstA",0,path);
	this->m_FEDefenseConstA = GetPrivateProfileInt(section,"FEDefenseConstA",0,path);
	this->m_MGDefenseConstA = GetPrivateProfileInt(section,"MGDefenseConstA",0,path);
	this->m_DLDefenseConstA = GetPrivateProfileInt(section,"DLDefenseConstA",0,path);
	#if(GAMESERVER_UPDATE>=401)
	this->m_SUDefenseConstA = GetPrivateProfileInt(section,"SUDefenseConstA",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_RFDefenseConstA = GetPrivateProfileInt(section,"RFDefenseConstA",0,path);
	#endif

	#if(GAMESERVER_UPDATE>=701)

	this->m_DuelElementalDamageRate = GetPrivateProfileInt(section,"DuelElementalDamageRate",0,path);

	this->m_GensElementalDamageRate = GetPrivateProfileInt(section,"GensElementalDamageRate",0,path);

	this->m_CustomArenaElementalDamageRate = GetPrivateProfileInt(section,"CustomArenaElementalDamageRate",0,path);

	this->m_ChaosCastleElementalDamageRate = GetPrivateProfileInt(section,"ChaosCastleElementalDamageRate",0,path);

	this->m_IllusionTempleElementalDamageRate = GetPrivateProfileInt(section,"IllusionTempleElementalDamageRate",0,path);

	this->m_CastleSiegeElementalDamageRate1 = GetPrivateProfileInt(section,"CastleSiegeElementalDamageRate1",0,path);
	this->m_CastleSiegeElementalDamageRate2 = GetPrivateProfileInt(section,"CastleSiegeElementalDamageRate2",0,path);

	this->m_GeneralElementalDamageRatePvP = GetPrivateProfileInt(section,"GeneralElementalDamageRatePvP",0,path);
	this->m_GeneralElementalDamageRatePvM = GetPrivateProfileInt(section,"GeneralElementalDamageRatePvM",0,path);

	this->m_ElementalDamageRatePvP[CLASS_DW] = GetPrivateProfileInt(section,"DWElementalDamageRatePvP",0,path);
	this->m_ElementalDamageRatePvP[CLASS_DK] = GetPrivateProfileInt(section,"DKElementalDamageRatePvP",0,path);
	this->m_ElementalDamageRatePvP[CLASS_FE] = GetPrivateProfileInt(section,"FEElementalDamageRatePvP",0,path);
	this->m_ElementalDamageRatePvP[CLASS_MG] = GetPrivateProfileInt(section,"MGElementalDamageRatePvP",0,path);
	this->m_ElementalDamageRatePvP[CLASS_DL] = GetPrivateProfileInt(section,"DLElementalDamageRatePvP",0,path);
	this->m_ElementalDamageRatePvP[CLASS_SU] = GetPrivateProfileInt(section,"SUElementalDamageRatePvP",0,path);
	this->m_ElementalDamageRatePvP[CLASS_RF] = GetPrivateProfileInt(section,"RFElementalDamageRatePvP",0,path);

	this->m_ElementalDamageRatePvM[CLASS_DW] = GetPrivateProfileInt(section,"DWElementalDamageRatePvM",0,path);
	this->m_ElementalDamageRatePvM[CLASS_DK] = GetPrivateProfileInt(section,"DKElementalDamageRatePvM",0,path);
	this->m_ElementalDamageRatePvM[CLASS_FE] = GetPrivateProfileInt(section,"FEElementalDamageRatePvM",0,path);
	this->m_ElementalDamageRatePvM[CLASS_MG] = GetPrivateProfileInt(section,"MGElementalDamageRatePvM",0,path);
	this->m_ElementalDamageRatePvM[CLASS_DL] = GetPrivateProfileInt(section,"DLElementalDamageRatePvM",0,path);
	this->m_ElementalDamageRatePvM[CLASS_SU] = GetPrivateProfileInt(section,"SUElementalDamageRatePvM",0,path);
	this->m_ElementalDamageRatePvM[CLASS_RF] = GetPrivateProfileInt(section,"RFElementalDamageRatePvM",0,path);

	this->m_ElementalDamageRateTo[CLASS_DW][CLASS_DW] = GetPrivateProfileInt(section,"DWElementalDamageRateToDW",0,path);
	this->m_ElementalDamageRateTo[CLASS_DW][CLASS_DK] = GetPrivateProfileInt(section,"DWElementalDamageRateToDK",0,path);
	this->m_ElementalDamageRateTo[CLASS_DW][CLASS_FE] = GetPrivateProfileInt(section,"DWElementalDamageRateToFE",0,path);
	this->m_ElementalDamageRateTo[CLASS_DW][CLASS_MG] = GetPrivateProfileInt(section,"DWElementalDamageRateToMG",0,path);
	this->m_ElementalDamageRateTo[CLASS_DW][CLASS_DL] = GetPrivateProfileInt(section,"DWElementalDamageRateToDL",0,path);
	this->m_ElementalDamageRateTo[CLASS_DW][CLASS_SU] = GetPrivateProfileInt(section,"DWElementalDamageRateToSU",0,path);
	this->m_ElementalDamageRateTo[CLASS_DW][CLASS_RF] = GetPrivateProfileInt(section,"DWElementalDamageRateToRF",0,path);

	this->m_ElementalDamageRateTo[CLASS_DK][CLASS_DW] = GetPrivateProfileInt(section,"DKElementalDamageRateToDW",0,path);
	this->m_ElementalDamageRateTo[CLASS_DK][CLASS_DK] = GetPrivateProfileInt(section,"DKElementalDamageRateToDK",0,path);
	this->m_ElementalDamageRateTo[CLASS_DK][CLASS_FE] = GetPrivateProfileInt(section,"DKElementalDamageRateToFE",0,path);
	this->m_ElementalDamageRateTo[CLASS_DK][CLASS_MG] = GetPrivateProfileInt(section,"DKElementalDamageRateToMG",0,path);
	this->m_ElementalDamageRateTo[CLASS_DK][CLASS_DL] = GetPrivateProfileInt(section,"DKElementalDamageRateToDL",0,path);
	this->m_ElementalDamageRateTo[CLASS_DK][CLASS_SU] = GetPrivateProfileInt(section,"DKElementalDamageRateToSU",0,path);
	this->m_ElementalDamageRateTo[CLASS_DK][CLASS_RF] = GetPrivateProfileInt(section,"DKElementalDamageRateToRF",0,path);

	this->m_ElementalDamageRateTo[CLASS_FE][CLASS_DW] = GetPrivateProfileInt(section,"FEElementalDamageRateToDW",0,path);
	this->m_ElementalDamageRateTo[CLASS_FE][CLASS_DK] = GetPrivateProfileInt(section,"FEElementalDamageRateToDK",0,path);
	this->m_ElementalDamageRateTo[CLASS_FE][CLASS_FE] = GetPrivateProfileInt(section,"FEElementalDamageRateToFE",0,path);
	this->m_ElementalDamageRateTo[CLASS_FE][CLASS_MG] = GetPrivateProfileInt(section,"FEElementalDamageRateToMG",0,path);
	this->m_ElementalDamageRateTo[CLASS_FE][CLASS_DL] = GetPrivateProfileInt(section,"FEElementalDamageRateToDL",0,path);
	this->m_ElementalDamageRateTo[CLASS_FE][CLASS_SU] = GetPrivateProfileInt(section,"FEElementalDamageRateToSU",0,path);
	this->m_ElementalDamageRateTo[CLASS_FE][CLASS_RF] = GetPrivateProfileInt(section,"FEElementalDamageRateToRF",0,path);

	this->m_ElementalDamageRateTo[CLASS_MG][CLASS_DW] = GetPrivateProfileInt(section,"MGElementalDamageRateToDW",0,path);
	this->m_ElementalDamageRateTo[CLASS_MG][CLASS_DK] = GetPrivateProfileInt(section,"MGElementalDamageRateToDK",0,path);
	this->m_ElementalDamageRateTo[CLASS_MG][CLASS_FE] = GetPrivateProfileInt(section,"MGElementalDamageRateToFE",0,path);
	this->m_ElementalDamageRateTo[CLASS_MG][CLASS_MG] = GetPrivateProfileInt(section,"MGElementalDamageRateToMG",0,path);
	this->m_ElementalDamageRateTo[CLASS_MG][CLASS_DL] = GetPrivateProfileInt(section,"MGElementalDamageRateToDL",0,path);
	this->m_ElementalDamageRateTo[CLASS_MG][CLASS_SU] = GetPrivateProfileInt(section,"MGElementalDamageRateToSU",0,path);
	this->m_ElementalDamageRateTo[CLASS_MG][CLASS_RF] = GetPrivateProfileInt(section,"MGElementalDamageRateToRF",0,path);

	this->m_ElementalDamageRateTo[CLASS_DL][CLASS_DW] = GetPrivateProfileInt(section,"DLElementalDamageRateToDW",0,path);
	this->m_ElementalDamageRateTo[CLASS_DL][CLASS_DK] = GetPrivateProfileInt(section,"DLElementalDamageRateToDK",0,path);
	this->m_ElementalDamageRateTo[CLASS_DL][CLASS_FE] = GetPrivateProfileInt(section,"DLElementalDamageRateToFE",0,path);
	this->m_ElementalDamageRateTo[CLASS_DL][CLASS_MG] = GetPrivateProfileInt(section,"DLElementalDamageRateToMG",0,path);
	this->m_ElementalDamageRateTo[CLASS_DL][CLASS_DL] = GetPrivateProfileInt(section,"DLElementalDamageRateToDL",0,path);
	this->m_ElementalDamageRateTo[CLASS_DL][CLASS_SU] = GetPrivateProfileInt(section,"DLElementalDamageRateToSU",0,path);
	this->m_ElementalDamageRateTo[CLASS_DL][CLASS_RF] = GetPrivateProfileInt(section,"DLElementalDamageRateToRF",0,path);

	this->m_ElementalDamageRateTo[CLASS_SU][CLASS_DW] = GetPrivateProfileInt(section,"SUElementalDamageRateToDW",0,path);
	this->m_ElementalDamageRateTo[CLASS_SU][CLASS_DK] = GetPrivateProfileInt(section,"SUElementalDamageRateToDK",0,path);
	this->m_ElementalDamageRateTo[CLASS_SU][CLASS_FE] = GetPrivateProfileInt(section,"SUElementalDamageRateToFE",0,path);
	this->m_ElementalDamageRateTo[CLASS_SU][CLASS_MG] = GetPrivateProfileInt(section,"SUElementalDamageRateToMG",0,path);
	this->m_ElementalDamageRateTo[CLASS_SU][CLASS_DL] = GetPrivateProfileInt(section,"SUElementalDamageRateToDL",0,path);
	this->m_ElementalDamageRateTo[CLASS_SU][CLASS_SU] = GetPrivateProfileInt(section,"SUElementalDamageRateToSU",0,path);
	this->m_ElementalDamageRateTo[CLASS_SU][CLASS_RF] = GetPrivateProfileInt(section,"SUElementalDamageRateToRF",0,path);

	this->m_ElementalDamageRateTo[CLASS_RF][CLASS_DW] = GetPrivateProfileInt(section,"RFElementalDamageRateToDW",0,path);
	this->m_ElementalDamageRateTo[CLASS_RF][CLASS_DK] = GetPrivateProfileInt(section,"RFElementalDamageRateToDK",0,path);
	this->m_ElementalDamageRateTo[CLASS_RF][CLASS_FE] = GetPrivateProfileInt(section,"RFElementalDamageRateToFE",0,path);
	this->m_ElementalDamageRateTo[CLASS_RF][CLASS_MG] = GetPrivateProfileInt(section,"RFElementalDamageRateToMG",0,path);
	this->m_ElementalDamageRateTo[CLASS_RF][CLASS_DL] = GetPrivateProfileInt(section,"RFElementalDamageRateToDL",0,path);
	this->m_ElementalDamageRateTo[CLASS_RF][CLASS_SU] = GetPrivateProfileInt(section,"RFElementalDamageRateToSU",0,path);
	this->m_ElementalDamageRateTo[CLASS_RF][CLASS_RF] = GetPrivateProfileInt(section,"RFElementalDamageRateToRF",0,path);

	this->m_DWElementalDefenseConstA = GetPrivateProfileInt(section,"DWElementalDefenseConstA",0,path);
	this->m_DKElementalDefenseConstA = GetPrivateProfileInt(section,"DKElementalDefenseConstA",0,path);
	this->m_FEElementalDefenseConstA = GetPrivateProfileInt(section,"FEElementalDefenseConstA",0,path);
	this->m_MGElementalDefenseConstA = GetPrivateProfileInt(section,"MGElementalDefenseConstA",0,path);
	this->m_DLElementalDefenseConstA = GetPrivateProfileInt(section,"DLElementalDefenseConstA",0,path);
	this->m_SUElementalDefenseConstA = GetPrivateProfileInt(section,"SUElementalDefenseConstA",0,path);
	this->m_RFElementalDefenseConstA = GetPrivateProfileInt(section,"RFElementalDefenseConstA",0,path);

	this->m_DWElementalDamageMinConstA = GetPrivateProfileInt(section,"DWElementalDamageMinConstA",0,path);
	this->m_DWElementalDamageMaxConstA = GetPrivateProfileInt(section,"DWElementalDamageMaxConstA",0,path);

	this->m_DKElementalDamageMinConstA = GetPrivateProfileInt(section,"DKElementalDamageMinConstA",0,path);
	this->m_DKElementalDamageMaxConstA = GetPrivateProfileInt(section,"DKElementalDamageMaxConstA",0,path);

	this->m_FEElementalDamageMinConstA = GetPrivateProfileInt(section,"FEElementalDamageMinConstA",0,path);
	this->m_FEElementalDamageMinConstB = GetPrivateProfileInt(section,"FEElementalDamageMinConstB",0,path);

	this->m_FEElementalDamageMaxConstA = GetPrivateProfileInt(section,"FEElementalDamageMaxConstA",0,path);
	this->m_FEElementalDamageMaxConstB = GetPrivateProfileInt(section,"FEElementalDamageMaxConstB",0,path);

	this->m_MGElementalDamageMinConstA = GetPrivateProfileInt(section,"MGElementalDamageMinConstA",0,path);
	this->m_MGElementalDamageMinConstB = GetPrivateProfileInt(section,"MGElementalDamageMinConstB",0,path);

	this->m_MGElementalDamageMaxConstA = GetPrivateProfileInt(section,"MGElementalDamageMaxConstA",0,path);
	this->m_MGElementalDamageMaxConstB = GetPrivateProfileInt(section,"MGElementalDamageMaxConstB",0,path);

	this->m_DLElementalDamageMinConstA = GetPrivateProfileInt(section,"DLElementalDamageMinConstA",0,path);
	this->m_DLElementalDamageMinConstB = GetPrivateProfileInt(section,"DLElementalDamageMinConstB",0,path);

	this->m_DLElementalDamageMaxConstA = GetPrivateProfileInt(section,"DLElementalDamageMaxConstA",0,path);
	this->m_DLElementalDamageMaxConstB = GetPrivateProfileInt(section,"DLElementalDamageMaxConstB",0,path);

	this->m_SUElementalDamageMinConstA = GetPrivateProfileInt(section,"SUElementalDamageMinConstA",0,path);
	this->m_SUElementalDamageMaxConstA = GetPrivateProfileInt(section,"SUElementalDamageMaxConstA",0,path);

	this->m_RFElementalDamageMinConstA = GetPrivateProfileInt(section,"RFElementalDamageMinConstA",0,path);
	this->m_RFElementalDamageMinConstB = GetPrivateProfileInt(section,"RFElementalDamageMinConstB",0,path);
	this->m_RFElementalDamageMaxConstA = GetPrivateProfileInt(section,"RFElementalDamageMaxConstA",0,path);
	this->m_RFElementalDamageMaxConstB = GetPrivateProfileInt(section,"RFElementalDamageMaxConstB",0,path);

	this->m_DWElementalAttackSuccessRateConstA = GetPrivateProfileInt(section,"DWElementalAttackSuccessRateConstA",0,path);
	this->m_DWElementalAttackSuccessRateConstB = GetPrivateProfileInt(section,"DWElementalAttackSuccessRateConstB",0,path);
	this->m_DWElementalAttackSuccessRateConstC = GetPrivateProfileInt(section,"DWElementalAttackSuccessRateConstC",0,path);
	this->m_DWElementalAttackSuccessRateConstD = GetPrivateProfileInt(section,"DWElementalAttackSuccessRateConstD",0,path);

	this->m_DKElementalAttackSuccessRateConstA = GetPrivateProfileInt(section,"DKElementalAttackSuccessRateConstA",0,path);
	this->m_DKElementalAttackSuccessRateConstB = GetPrivateProfileInt(section,"DKElementalAttackSuccessRateConstB",0,path);
	this->m_DKElementalAttackSuccessRateConstC = GetPrivateProfileInt(section,"DKElementalAttackSuccessRateConstC",0,path);
	this->m_DKElementalAttackSuccessRateConstD = GetPrivateProfileInt(section,"DKElementalAttackSuccessRateConstD",0,path);

	this->m_FEElementalAttackSuccessRateConstA = GetPrivateProfileInt(section,"FEElementalAttackSuccessRateConstA",0,path);
	this->m_FEElementalAttackSuccessRateConstB = GetPrivateProfileInt(section,"FEElementalAttackSuccessRateConstB",0,path);
	this->m_FEElementalAttackSuccessRateConstC = GetPrivateProfileInt(section,"FEElementalAttackSuccessRateConstC",0,path);
	this->m_FEElementalAttackSuccessRateConstD = GetPrivateProfileInt(section,"FEElementalAttackSuccessRateConstD",0,path);

	this->m_MGElementalAttackSuccessRateConstA = GetPrivateProfileInt(section,"MGElementalAttackSuccessRateConstA",0,path);
	this->m_MGElementalAttackSuccessRateConstB = GetPrivateProfileInt(section,"MGElementalAttackSuccessRateConstB",0,path);
	this->m_MGElementalAttackSuccessRateConstC = GetPrivateProfileInt(section,"MGElementalAttackSuccessRateConstC",0,path);
	this->m_MGElementalAttackSuccessRateConstD = GetPrivateProfileInt(section,"MGElementalAttackSuccessRateConstD",0,path);

	this->m_DLElementalAttackSuccessRateConstA = GetPrivateProfileInt(section,"DLElementalAttackSuccessRateConstA",0,path);
	this->m_DLElementalAttackSuccessRateConstB = GetPrivateProfileInt(section,"DLElementalAttackSuccessRateConstB",0,path);
	this->m_DLElementalAttackSuccessRateConstC = GetPrivateProfileInt(section,"DLElementalAttackSuccessRateConstC",0,path);
	this->m_DLElementalAttackSuccessRateConstD = GetPrivateProfileInt(section,"DLElementalAttackSuccessRateConstD",0,path);

	this->m_SUElementalAttackSuccessRateConstA = GetPrivateProfileInt(section,"SUElementalAttackSuccessRateConstA",0,path);
	this->m_SUElementalAttackSuccessRateConstB = GetPrivateProfileInt(section,"SUElementalAttackSuccessRateConstB",0,path);
	this->m_SUElementalAttackSuccessRateConstC = GetPrivateProfileInt(section,"SUElementalAttackSuccessRateConstC",0,path);
	this->m_SUElementalAttackSuccessRateConstD = GetPrivateProfileInt(section,"SUElementalAttackSuccessRateConstD",0,path);

	this->m_RFElementalAttackSuccessRateConstA = GetPrivateProfileInt(section,"RFElementalAttackSuccessRateConstA",0,path);
	this->m_RFElementalAttackSuccessRateConstB = GetPrivateProfileInt(section,"RFElementalAttackSuccessRateConstB",0,path);
	this->m_RFElementalAttackSuccessRateConstC = GetPrivateProfileInt(section,"RFElementalAttackSuccessRateConstC",0,path);
	this->m_RFElementalAttackSuccessRateConstD = GetPrivateProfileInt(section,"RFElementalAttackSuccessRateConstD",0,path);

	this->m_DWElementalDefenseSuccessRateConstA = GetPrivateProfileInt(section,"DWElementalDefenseSuccessRateConstA",0,path);
	this->m_DKElementalDefenseSuccessRateConstA = GetPrivateProfileInt(section,"DKElementalDefenseSuccessRateConstA",0,path);
	this->m_FEElementalDefenseSuccessRateConstA = GetPrivateProfileInt(section,"FEElementalDefenseSuccessRateConstA",0,path);
	this->m_MGElementalDefenseSuccessRateConstA = GetPrivateProfileInt(section,"MGElementalDefenseSuccessRateConstA",0,path);
	this->m_DLElementalDefenseSuccessRateConstA = GetPrivateProfileInt(section,"DLElementalDefenseSuccessRateConstA",0,path);
	this->m_SUElementalDefenseSuccessRateConstA = GetPrivateProfileInt(section,"SUElementalDefenseSuccessRateConstA",0,path);
	this->m_RFElementalDefenseSuccessRateConstA = GetPrivateProfileInt(section,"RFElementalDefenseSuccessRateConstA",0,path);

	#endif
}

void CServerInfo::ReadCommandInfo(char* section,char* path) // OK
{
	this->m_CommandPostType = GetPrivateProfileInt(section,"CommandPostType",0,path);

	this->m_CommandPKClearType = GetPrivateProfileInt(section,"CommandPKClearType",0,path);
	this->m_CommandPKClearMoney[0] = GetPrivateProfileInt(section,"CommandPKClearMoney_AL0",0,path);
	this->m_CommandPKClearMoney[1] = GetPrivateProfileInt(section,"CommandPKClearMoney_AL1",0,path);
	this->m_CommandPKClearMoney[2] = GetPrivateProfileInt(section,"CommandPKClearMoney_AL2",0,path);
	this->m_CommandPKClearMoney[3] = GetPrivateProfileInt(section,"CommandPKClearMoney_AL3",0,path);

	this->m_CommandAddPointAutoEnable[0] = GetPrivateProfileInt(section,"CommandAddPointAutoEnable_AL0",0,path);
	this->m_CommandAddPointAutoEnable[1] = GetPrivateProfileInt(section,"CommandAddPointAutoEnable_AL1",0,path);
	this->m_CommandAddPointAutoEnable[2] = GetPrivateProfileInt(section,"CommandAddPointAutoEnable_AL2",0,path);
	this->m_CommandAddPointAutoEnable[3] = GetPrivateProfileInt(section,"CommandAddPointAutoEnable_AL3",0,path);

	this->m_CommandChangeLimit[0] = GetPrivateProfileInt(section,"CommandChangeLimit_AL0",0,path);
	this->m_CommandChangeLimit[1] = GetPrivateProfileInt(section,"CommandChangeLimit_AL1",0,path);
	this->m_CommandChangeLimit[2] = GetPrivateProfileInt(section,"CommandChangeLimit_AL2",0,path);
	this->m_CommandChangeLimit[3] = GetPrivateProfileInt(section,"CommandChangeLimit_AL3",0,path);

	this->m_CommandWareNumber[0] = GetPrivateProfileInt(section,"CommandWareNumber_AL0",0,path);
	this->m_CommandWareNumber[1] = GetPrivateProfileInt(section,"CommandWareNumber_AL1",0,path);
	this->m_CommandWareNumber[2] = GetPrivateProfileInt(section,"CommandWareNumber_AL2",0,path);
	this->m_CommandWareNumber[3] = GetPrivateProfileInt(section,"CommandWareNumber_AL3",0,path);

	this->m_CommandResetType = GetPrivateProfileInt(section,"CommandResetType",0,path);

	this->m_CommandResetKeepStrength = GetPrivateProfileInt(section,"CommandResetKeepStrength",0,path);
	this->m_CommandResetKeepDexterity = GetPrivateProfileInt(section,"CommandResetKeepDexterity",0,path);
	this->m_CommandResetKeepVitality = GetPrivateProfileInt(section,"CommandResetKeepVitality",0,path);
	this->m_CommandResetKeepEnergy = GetPrivateProfileInt(section,"CommandResetKeepEnergy",0,path);
	this->m_CommandResetKeepLeadership = GetPrivateProfileInt(section,"CommandResetKeepLeadership",0,path);

	this->m_CommandResetAutoEnable[0] = GetPrivateProfileInt(section,"CommandResetAutoEnable_AL0",0,path);
	this->m_CommandResetAutoEnable[1] = GetPrivateProfileInt(section,"CommandResetAutoEnable_AL1",0,path);
	this->m_CommandResetAutoEnable[2] = GetPrivateProfileInt(section,"CommandResetAutoEnable_AL2",0,path);
	this->m_CommandResetAutoEnable[3] = GetPrivateProfileInt(section,"CommandResetAutoEnable_AL3",0,path);

	this->m_CommandResetCheckItem[0] = GetPrivateProfileInt(section,"CommandResetCheckItem_AL0",0,path);
	this->m_CommandResetCheckItem[1] = GetPrivateProfileInt(section,"CommandResetCheckItem_AL1",0,path);
	this->m_CommandResetCheckItem[2] = GetPrivateProfileInt(section,"CommandResetCheckItem_AL2",0,path);
	this->m_CommandResetCheckItem[3] = GetPrivateProfileInt(section,"CommandResetCheckItem_AL3",0,path);

	this->m_CommandResetMove[0] = GetPrivateProfileInt(section,"CommandResetMove_AL0",0,path);
	this->m_CommandResetMove[1] = GetPrivateProfileInt(section,"CommandResetMove_AL1",0,path);
	this->m_CommandResetMove[2] = GetPrivateProfileInt(section,"CommandResetMove_AL2",0,path);
	this->m_CommandResetMove[3] = GetPrivateProfileInt(section,"CommandResetMove_AL3",0,path);

	this->m_CommandResetClearQuest[0] = GetPrivateProfileInt(section,"CommandResetClearQuest_AL0",0,path);
	this->m_CommandResetClearQuest[1] = GetPrivateProfileInt(section,"CommandResetClearQuest_AL1",0,path);
	this->m_CommandResetClearQuest[2] = GetPrivateProfileInt(section,"CommandResetClearQuest_AL2",0,path);
	this->m_CommandResetClearQuest[3] = GetPrivateProfileInt(section,"CommandResetClearQuest_AL3",0,path);

	this->m_CommandResetClearSkill[0] = GetPrivateProfileInt(section,"CommandResetClearSkill_AL0",0,path);
	this->m_CommandResetClearSkill[1] = GetPrivateProfileInt(section,"CommandResetClearSkill_AL1",0,path);
	this->m_CommandResetClearSkill[2] = GetPrivateProfileInt(section,"CommandResetClearSkill_AL2",0,path);
	this->m_CommandResetClearSkill[3] = GetPrivateProfileInt(section,"CommandResetClearSkill_AL3",0,path);

	this->m_CommandResetClearParty[0] = GetPrivateProfileInt(section,"CommandResetClearParty_AL0",0,path);
	this->m_CommandResetClearParty[1] = GetPrivateProfileInt(section,"CommandResetClearParty_AL1",0,path);
	this->m_CommandResetClearParty[2] = GetPrivateProfileInt(section,"CommandResetClearParty_AL2",0,path);
	this->m_CommandResetClearParty[3] = GetPrivateProfileInt(section,"CommandResetClearParty_AL3",0,path);

	this->m_CommandResetLevel[0] = GetPrivateProfileInt(section,"CommandResetLevel_AL0",0,path);
	this->m_CommandResetLevel[1] = GetPrivateProfileInt(section,"CommandResetLevel_AL1",0,path);
	this->m_CommandResetLevel[2] = GetPrivateProfileInt(section,"CommandResetLevel_AL2",0,path);
	this->m_CommandResetLevel[3] = GetPrivateProfileInt(section,"CommandResetLevel_AL3",0,path);

	this->m_CommandResetMoney[0] = GetPrivateProfileInt(section,"CommandResetMoney_AL0",0,path);
	this->m_CommandResetMoney[1] = GetPrivateProfileInt(section,"CommandResetMoney_AL1",0,path);
	this->m_CommandResetMoney[2] = GetPrivateProfileInt(section,"CommandResetMoney_AL2",0,path);
	this->m_CommandResetMoney[3] = GetPrivateProfileInt(section,"CommandResetMoney_AL3",0,path);

	#if(GAMESERVER_SHOP>=1)

	this->m_CommandResetCoin1[0] = GetPrivateProfileInt(section,"CommandResetCoin1_AL0",0,path);
	this->m_CommandResetCoin1[1] = GetPrivateProfileInt(section,"CommandResetCoin1_AL1",0,path);
	this->m_CommandResetCoin1[2] = GetPrivateProfileInt(section,"CommandResetCoin1_AL2",0,path);
	this->m_CommandResetCoin1[3] = GetPrivateProfileInt(section,"CommandResetCoin1_AL3",0,path);

	#endif

	#if(GAMESERVER_SHOP==3)

	this->m_CommandResetCoin2[0] = GetPrivateProfileInt(section,"CommandResetCoin2_AL0",0,path);
	this->m_CommandResetCoin2[1] = GetPrivateProfileInt(section,"CommandResetCoin2_AL1",0,path);
	this->m_CommandResetCoin2[2] = GetPrivateProfileInt(section,"CommandResetCoin2_AL2",0,path);
	this->m_CommandResetCoin2[3] = GetPrivateProfileInt(section,"CommandResetCoin2_AL3",0,path);

	this->m_CommandResetCoin3[0] = GetPrivateProfileInt(section,"CommandResetCoin3_AL0",0,path);
	this->m_CommandResetCoin3[1] = GetPrivateProfileInt(section,"CommandResetCoin3_AL1",0,path);
	this->m_CommandResetCoin3[2] = GetPrivateProfileInt(section,"CommandResetCoin3_AL2",0,path);
	this->m_CommandResetCoin3[3] = GetPrivateProfileInt(section,"CommandResetCoin3_AL3",0,path);

	#endif

	this->m_CommandResetCount[0] = GetPrivateProfileInt(section,"CommandResetCount_AL0",0,path);
	this->m_CommandResetCount[1] = GetPrivateProfileInt(section,"CommandResetCount_AL1",0,path);
	this->m_CommandResetCount[2] = GetPrivateProfileInt(section,"CommandResetCount_AL2",0,path);
	this->m_CommandResetCount[3] = GetPrivateProfileInt(section,"CommandResetCount_AL3",0,path);

	this->m_CommandResetLimit[0] = GetPrivateProfileInt(section,"CommandResetLimit_AL0",0,path);
	this->m_CommandResetLimit[1] = GetPrivateProfileInt(section,"CommandResetLimit_AL1",0,path);
	this->m_CommandResetLimit[2] = GetPrivateProfileInt(section,"CommandResetLimit_AL2",0,path);
	this->m_CommandResetLimit[3] = GetPrivateProfileInt(section,"CommandResetLimit_AL3",0,path);

	this->m_CommandResetLimitDay[0] = GetPrivateProfileInt(section,"CommandResetLimitDay_AL0",0,path);
	this->m_CommandResetLimitDay[1] = GetPrivateProfileInt(section,"CommandResetLimitDay_AL1",0,path);
	this->m_CommandResetLimitDay[2] = GetPrivateProfileInt(section,"CommandResetLimitDay_AL2",0,path);
	this->m_CommandResetLimitDay[3] = GetPrivateProfileInt(section,"CommandResetLimitDay_AL3",0,path);

	this->m_CommandResetLimitWek[0] = GetPrivateProfileInt(section,"CommandResetLimitWek_AL0",0,path);
	this->m_CommandResetLimitWek[1] = GetPrivateProfileInt(section,"CommandResetLimitWek_AL1",0,path);
	this->m_CommandResetLimitWek[2] = GetPrivateProfileInt(section,"CommandResetLimitWek_AL2",0,path);
	this->m_CommandResetLimitWek[3] = GetPrivateProfileInt(section,"CommandResetLimitWek_AL3",0,path);

	this->m_CommandResetLimitMon[0] = GetPrivateProfileInt(section,"CommandResetLimitMon_AL0",0,path);
	this->m_CommandResetLimitMon[1] = GetPrivateProfileInt(section,"CommandResetLimitMon_AL1",0,path);
	this->m_CommandResetLimitMon[2] = GetPrivateProfileInt(section,"CommandResetLimitMon_AL2",0,path);
	this->m_CommandResetLimitMon[3] = GetPrivateProfileInt(section,"CommandResetLimitMon_AL3",0,path);

	this->m_CommandResetStartLevel[0] = GetPrivateProfileInt(section,"CommandResetStartLevel_AL0",0,path);
	this->m_CommandResetStartLevel[1] = GetPrivateProfileInt(section,"CommandResetStartLevel_AL1",0,path);
	this->m_CommandResetStartLevel[2] = GetPrivateProfileInt(section,"CommandResetStartLevel_AL2",0,path);
	this->m_CommandResetStartLevel[3] = GetPrivateProfileInt(section,"CommandResetStartLevel_AL3",0,path);

	this->m_CommandResetPoint[0] = GetPrivateProfileInt(section,"CommandResetPoint_AL0",0,path);
	this->m_CommandResetPoint[1] = GetPrivateProfileInt(section,"CommandResetPoint_AL1",0,path);
	this->m_CommandResetPoint[2] = GetPrivateProfileInt(section,"CommandResetPoint_AL2",0,path);
	this->m_CommandResetPoint[3] = GetPrivateProfileInt(section,"CommandResetPoint_AL3",0,path);

	this->m_CommandResetPointRate[CLASS_DW] = GetPrivateProfileInt(section,"CommandResetPointRateDW",0,path);
	this->m_CommandResetPointRate[CLASS_DK] = GetPrivateProfileInt(section,"CommandResetPointRateDK",0,path);
	this->m_CommandResetPointRate[CLASS_FE] = GetPrivateProfileInt(section,"CommandResetPointRateFE",0,path);
	this->m_CommandResetPointRate[CLASS_MG] = GetPrivateProfileInt(section,"CommandResetPointRateMG",0,path);
	this->m_CommandResetPointRate[CLASS_DL] = GetPrivateProfileInt(section,"CommandResetPointRateDL",0,path);
	#if(GAMESERVER_UPDATE>=401)
	this->m_CommandResetPointRate[CLASS_SU] = GetPrivateProfileInt(section,"CommandResetPointRateSU",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_CommandResetPointRate[CLASS_RF] = GetPrivateProfileInt(section,"CommandResetPointRateRF",0,path);
	#endif

	this->m_CommandMasterResetType = GetPrivateProfileInt(section,"CommandMasterResetType",0,path);

	this->m_CommandMasterResetKeepStrength = GetPrivateProfileInt(section,"CommandMasterResetKeepStrength",0,path);
	this->m_CommandMasterResetKeepDexterity = GetPrivateProfileInt(section,"CommandMasterResetKeepDexterity",0,path);
	this->m_CommandMasterResetKeepVitality = GetPrivateProfileInt(section,"CommandMasterResetKeepVitality",0,path);
	this->m_CommandMasterResetKeepEnergy = GetPrivateProfileInt(section,"CommandMasterResetKeepEnergy",0,path);
	this->m_CommandMasterResetKeepLeadership = GetPrivateProfileInt(section,"CommandMasterResetKeepLeadership",0,path);

	this->m_CommandMasterResetCheckItem[0] = GetPrivateProfileInt(section,"CommandMasterResetCheckItem_AL0",0,path);
	this->m_CommandMasterResetCheckItem[1] = GetPrivateProfileInt(section,"CommandMasterResetCheckItem_AL1",0,path);
	this->m_CommandMasterResetCheckItem[2] = GetPrivateProfileInt(section,"CommandMasterResetCheckItem_AL2",0,path);
	this->m_CommandMasterResetCheckItem[3] = GetPrivateProfileInt(section,"CommandMasterResetCheckItem_AL3",0,path);

	this->m_CommandMasterResetMove[0] = GetPrivateProfileInt(section,"CommandMasterResetMove_AL0",0,path);
	this->m_CommandMasterResetMove[1] = GetPrivateProfileInt(section,"CommandMasterResetMove_AL1",0,path);
	this->m_CommandMasterResetMove[2] = GetPrivateProfileInt(section,"CommandMasterResetMove_AL2",0,path);
	this->m_CommandMasterResetMove[3] = GetPrivateProfileInt(section,"CommandMasterResetMove_AL3",0,path);

	this->m_CommandMasterResetClearQuest[0] = GetPrivateProfileInt(section,"CommandMasterResetClearQuest_AL0",0,path);
	this->m_CommandMasterResetClearQuest[1] = GetPrivateProfileInt(section,"CommandMasterResetClearQuest_AL1",0,path);
	this->m_CommandMasterResetClearQuest[2] = GetPrivateProfileInt(section,"CommandMasterResetClearQuest_AL2",0,path);
	this->m_CommandMasterResetClearQuest[3] = GetPrivateProfileInt(section,"CommandMasterResetClearQuest_AL3",0,path);

	this->m_CommandMasterResetClearSkill[0] = GetPrivateProfileInt(section,"CommandMasterResetClearSkill_AL0",0,path);
	this->m_CommandMasterResetClearSkill[1] = GetPrivateProfileInt(section,"CommandMasterResetClearSkill_AL1",0,path);
	this->m_CommandMasterResetClearSkill[2] = GetPrivateProfileInt(section,"CommandMasterResetClearSkill_AL2",0,path);
	this->m_CommandMasterResetClearSkill[3] = GetPrivateProfileInt(section,"CommandMasterResetClearSkill_AL3",0,path);

	this->m_CommandMasterResetClearParty[0] = GetPrivateProfileInt(section,"CommandMasterResetClearParty_AL0",0,path);
	this->m_CommandMasterResetClearParty[1] = GetPrivateProfileInt(section,"CommandMasterResetClearParty_AL0",0,path);
	this->m_CommandMasterResetClearParty[2] = GetPrivateProfileInt(section,"CommandMasterResetClearParty_AL0",0,path);
	this->m_CommandMasterResetClearParty[3] = GetPrivateProfileInt(section,"CommandMasterResetClearParty_AL0",0,path);

	this->m_CommandMasterResetLevel[0] = GetPrivateProfileInt(section,"CommandMasterResetLevel_AL0",0,path);
	this->m_CommandMasterResetLevel[1] = GetPrivateProfileInt(section,"CommandMasterResetLevel_AL1",0,path);
	this->m_CommandMasterResetLevel[2] = GetPrivateProfileInt(section,"CommandMasterResetLevel_AL2",0,path);
	this->m_CommandMasterResetLevel[3] = GetPrivateProfileInt(section,"CommandMasterResetLevel_AL3",0,path);

	this->m_CommandMasterResetReset[0] = GetPrivateProfileInt(section,"CommandMasterResetReset_AL0",0,path);
	this->m_CommandMasterResetReset[1] = GetPrivateProfileInt(section,"CommandMasterResetReset_AL1",0,path);
	this->m_CommandMasterResetReset[2] = GetPrivateProfileInt(section,"CommandMasterResetReset_AL2",0,path);
	this->m_CommandMasterResetReset[3] = GetPrivateProfileInt(section,"CommandMasterResetReset_AL3",0,path);

	this->m_CommandMasterResetMoney[0] = GetPrivateProfileInt(section,"CommandMasterResetMoney_AL0",0,path);
	this->m_CommandMasterResetMoney[1] = GetPrivateProfileInt(section,"CommandMasterResetMoney_AL1",0,path);
	this->m_CommandMasterResetMoney[2] = GetPrivateProfileInt(section,"CommandMasterResetMoney_AL2",0,path);
	this->m_CommandMasterResetMoney[3] = GetPrivateProfileInt(section,"CommandMasterResetMoney_AL3",0,path);

	#if(GAMESERVER_SHOP>=1)

	this->m_CommandMasterResetCoin1[0] = GetPrivateProfileInt(section,"CommandMasterResetCoin1_AL0",0,path);
	this->m_CommandMasterResetCoin1[1] = GetPrivateProfileInt(section,"CommandMasterResetCoin1_AL1",0,path);
	this->m_CommandMasterResetCoin1[2] = GetPrivateProfileInt(section,"CommandMasterResetCoin1_AL2",0,path);
	this->m_CommandMasterResetCoin1[3] = GetPrivateProfileInt(section,"CommandMasterResetCoin1_AL3",0,path);

	#endif

	#if(GAMESERVER_SHOP==3)

	this->m_CommandMasterResetCoin2[0] = GetPrivateProfileInt(section,"CommandMasterResetCoin2_AL0",0,path);
	this->m_CommandMasterResetCoin2[1] = GetPrivateProfileInt(section,"CommandMasterResetCoin2_AL1",0,path);
	this->m_CommandMasterResetCoin2[2] = GetPrivateProfileInt(section,"CommandMasterResetCoin2_AL2",0,path);
	this->m_CommandMasterResetCoin2[3] = GetPrivateProfileInt(section,"CommandMasterResetCoin2_AL3",0,path);

	this->m_CommandMasterResetCoin3[0] = GetPrivateProfileInt(section,"CommandMasterResetCoin3_AL0",0,path);
	this->m_CommandMasterResetCoin3[1] = GetPrivateProfileInt(section,"CommandMasterResetCoin3_AL1",0,path);
	this->m_CommandMasterResetCoin3[2] = GetPrivateProfileInt(section,"CommandMasterResetCoin3_AL2",0,path);
	this->m_CommandMasterResetCoin3[3] = GetPrivateProfileInt(section,"CommandMasterResetCoin3_AL3",0,path);

	#endif

	this->m_CommandMasterResetCount[0] = GetPrivateProfileInt(section,"CommandMasterResetCount_AL0",0,path);
	this->m_CommandMasterResetCount[1] = GetPrivateProfileInt(section,"CommandMasterResetCount_AL1",0,path);
	this->m_CommandMasterResetCount[2] = GetPrivateProfileInt(section,"CommandMasterResetCount_AL2",0,path);
	this->m_CommandMasterResetCount[3] = GetPrivateProfileInt(section,"CommandMasterResetCount_AL3",0,path);

	this->m_CommandMasterResetLimit[0] = GetPrivateProfileInt(section,"CommandMasterResetLimit_AL0",0,path);
	this->m_CommandMasterResetLimit[1] = GetPrivateProfileInt(section,"CommandMasterResetLimit_AL1",0,path);
	this->m_CommandMasterResetLimit[2] = GetPrivateProfileInt(section,"CommandMasterResetLimit_AL2",0,path);
	this->m_CommandMasterResetLimit[3] = GetPrivateProfileInt(section,"CommandMasterResetLimit_AL3",0,path);

	this->m_CommandMasterResetLimitDay[0] = GetPrivateProfileInt(section,"CommandMasterResetLimitDay_AL0",0,path);
	this->m_CommandMasterResetLimitDay[1] = GetPrivateProfileInt(section,"CommandMasterResetLimitDay_AL1",0,path);
	this->m_CommandMasterResetLimitDay[2] = GetPrivateProfileInt(section,"CommandMasterResetLimitDay_AL2",0,path);
	this->m_CommandMasterResetLimitDay[3] = GetPrivateProfileInt(section,"CommandMasterResetLimitDay_AL3",0,path);

	this->m_CommandMasterResetLimitWek[0] = GetPrivateProfileInt(section,"CommandMasterResetLimitWek_AL0",0,path);
	this->m_CommandMasterResetLimitWek[1] = GetPrivateProfileInt(section,"CommandMasterResetLimitWek_AL1",0,path);
	this->m_CommandMasterResetLimitWek[2] = GetPrivateProfileInt(section,"CommandMasterResetLimitWek_AL2",0,path);
	this->m_CommandMasterResetLimitWek[3] = GetPrivateProfileInt(section,"CommandMasterResetLimitWek_AL3",0,path);

	this->m_CommandMasterResetLimitMon[0] = GetPrivateProfileInt(section,"CommandMasterResetLimitMon_AL0",0,path);
	this->m_CommandMasterResetLimitMon[1] = GetPrivateProfileInt(section,"CommandMasterResetLimitMon_AL1",0,path);
	this->m_CommandMasterResetLimitMon[2] = GetPrivateProfileInt(section,"CommandMasterResetLimitMon_AL2",0,path);
	this->m_CommandMasterResetLimitMon[3] = GetPrivateProfileInt(section,"CommandMasterResetLimitMon_AL3",0,path);

	this->m_CommandMasterResetStartLevel[0] = GetPrivateProfileInt(section,"CommandMasterResetStartLevel_AL0",0,path);
	this->m_CommandMasterResetStartLevel[1] = GetPrivateProfileInt(section,"CommandMasterResetStartLevel_AL1",0,path);
	this->m_CommandMasterResetStartLevel[2] = GetPrivateProfileInt(section,"CommandMasterResetStartLevel_AL2",0,path);
	this->m_CommandMasterResetStartLevel[3] = GetPrivateProfileInt(section,"CommandMasterResetStartLevel_AL3",0,path);

	this->m_CommandMasterResetStartReset[0] = GetPrivateProfileInt(section,"CommandMasterResetStartReset_AL0",0,path);
	this->m_CommandMasterResetStartReset[1] = GetPrivateProfileInt(section,"CommandMasterResetStartReset_AL1",0,path);
	this->m_CommandMasterResetStartReset[2] = GetPrivateProfileInt(section,"CommandMasterResetStartReset_AL2",0,path);
	this->m_CommandMasterResetStartReset[3] = GetPrivateProfileInt(section,"CommandMasterResetStartReset_AL3",0,path);

	this->m_CommandMasterResetPoint[0] = GetPrivateProfileInt(section,"CommandMasterResetPoint_AL0",0,path);
	this->m_CommandMasterResetPoint[1] = GetPrivateProfileInt(section,"CommandMasterResetPoint_AL1",0,path);
	this->m_CommandMasterResetPoint[2] = GetPrivateProfileInt(section,"CommandMasterResetPoint_AL2",0,path);
	this->m_CommandMasterResetPoint[3] = GetPrivateProfileInt(section,"CommandMasterResetPoint_AL3",0,path);

	this->m_CommandMasterResetPointRate[CLASS_DW] = GetPrivateProfileInt(section,"CommandMasterResetPointRateDW",0,path);
	this->m_CommandMasterResetPointRate[CLASS_DK] = GetPrivateProfileInt(section,"CommandMasterResetPointRateDK",0,path);
	this->m_CommandMasterResetPointRate[CLASS_FE] = GetPrivateProfileInt(section,"CommandMasterResetPointRateFE",0,path);
	this->m_CommandMasterResetPointRate[CLASS_MG] = GetPrivateProfileInt(section,"CommandMasterResetPointRateMG",0,path);
	this->m_CommandMasterResetPointRate[CLASS_DL] = GetPrivateProfileInt(section,"CommandMasterResetPointRateDL",0,path);
	#if(GAMESERVER_UPDATE>=401)
	this->m_CommandMasterResetPointRate[CLASS_SU] = GetPrivateProfileInt(section,"CommandMasterResetPointRateSU",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_CommandMasterResetPointRate[CLASS_RF] = GetPrivateProfileInt(section,"CommandMasterResetPointRateRF",0,path);
	#endif

	this->m_CommandGMNoticeType = GetPrivateProfileInt(section,"CommandGMNoticeType",0,path);
}

void CServerInfo::ReadCommonInfo(char* section,char* path) // OK
{
	this->m_ServerMaxUserNumber = GetPrivateProfileInt(section,"ServerMaxUserNumber",0,path);
	this->m_ServerMaxUserNumber = ((this->m_ServerMaxUserNumber>gGameServerMaxAccount[4]) ? gGameServerMaxAccount[4] : this->m_ServerMaxUserNumber);

	this->m_MaxConnectionIdle = GetPrivateProfileInt(section,"MaxConnectionIdle",0,path);
	this->m_MaxConnectionPerIP = GetPrivateProfileInt(section,"MaxConnectionPerIP",0,path);
	this->m_MaxConnectionPerHID = GetPrivateProfileInt(section,"MaxConnectionPerHID",0,path);
	this->m_MaxPacketPerSecond = GetPrivateProfileInt(section,"MaxPacketPerSecond",0,path);
	this->m_MaxTimeConnectionVerify = GetPrivateProfileInt(section,"MaxTimeConnectionVerify",0,path);

	this->m_ChaosMixPlusItemAnnounce = GetPrivateProfileInt(section,"ChaosMixPlusItemAnnounce",0,path);
	this->m_MaxItemOption = GetPrivateProfileInt(section,"MaxItemOption",0,path);
	this->m_PersonalCodeCheck = GetPrivateProfileInt(section,"PersonalCodeCheck",0,path);
	this->m_ConnectMemberCheck = GetPrivateProfileInt(section,"ConnectMemberCheck",0,path);
	this->m_TeleportAttackCheck = GetPrivateProfileInt(section,"TeleportAttackCheck",0,path);
	this->m_EffectOverwriteMode = GetPrivateProfileInt(section,"EffectOverwriteMode",0,path);
	#if(GAMESERVER_UPDATE>=201)
	this->m_SetItemAcceptHarmonySwitch = GetPrivateProfileInt(section,"SetItemAcceptHarmonySwitch",0,path);
	#endif

	this->m_MonsterMaxLifeRate = GetPrivateProfileInt(section,"MonsterMaxLifeRate",0,path);
	this->m_MonsterDefenseRate = GetPrivateProfileInt(section,"MonsterDefenseRate",0,path);
	this->m_MonsterDefenseSuccessRateRate = GetPrivateProfileInt(section,"MonsterDefenseSuccessRateRate",0,path);
	this->m_MonsterPhysiDamageRate = GetPrivateProfileInt(section,"MonsterPhysiDamageRate",0,path);
	this->m_MonsterAttackSuccessRateRate = GetPrivateProfileInt(section,"MonsterAttackSuccessRateRate",0,path);
	#if(GAMESERVER_UPDATE<=603)
	this->m_MonsterHealthBarSwitch = GetPrivateProfileInt(section,"MonsterHealthBarSwitch",0,path);
	#endif

	this->m_MonsterGetTopHitDamageUserMaxTime = GetPrivateProfileInt(section,"MonsterGetTopHitDamageUserMaxTime",0,path);

	this->m_NonPK = GetPrivateProfileInt(section,"NonPK",0,path);
	this->m_PKLimitFree = GetPrivateProfileInt(section,"PKLimitFree",0,path);
	this->m_PKLimitShop = GetPrivateProfileInt(section,"PKLimitShop",0,path);
	this->m_PKLimitMove = GetPrivateProfileInt(section,"PKLimitMove",0,path);
	this->m_PKLimitMoveSummon = GetPrivateProfileInt(section,"PKLimitMoveSummon",0,path);
	this->m_PKLimitEventEntry = GetPrivateProfileInt(section,"PKLimitEventEntry",0,path);
	this->m_PKDeathAnnounce = GetPrivateProfileInt(section,"PKDeathAnnounce",0,path);
	this->m_PKDownPlusTimePoint = GetPrivateProfileInt(section,"PKDownPlusTimePoint",0,path);
	this->m_PKDownPlusKillPoint = GetPrivateProfileInt(section,"PKDownPlusKillPoint",0,path);
	this->m_PKDownRequirePoint1 = GetPrivateProfileInt(section,"PKDownRequirePoint1",0,path);
	this->m_PKDownRequirePoint2 = GetPrivateProfileInt(section,"PKDownRequirePoint2",0,path);
	this->m_PKDownRequirePoint3 = GetPrivateProfileInt(section,"PKDownRequirePoint3",0,path);
	this->m_PKDownRequirePoint4 = GetPrivateProfileInt(section,"PKDownRequirePoint4",0,path);
	this->m_PKItemDropRatePvP1 = GetPrivateProfileInt(section,"PKItemDropRatePvP1",0,path);
	this->m_PKItemDropRatePvP2 = GetPrivateProfileInt(section,"PKItemDropRatePvP2",0,path);
	this->m_PKItemDropRatePvP3 = GetPrivateProfileInt(section,"PKItemDropRatePvP3",0,path);
	this->m_PKItemDropRatePvM1 = GetPrivateProfileInt(section,"PKItemDropRatePvM1",0,path);
	this->m_PKItemDropRatePvM2 = GetPrivateProfileInt(section,"PKItemDropRatePvM2",0,path);
	this->m_PKItemDropRatePvM3 = GetPrivateProfileInt(section,"PKItemDropRatePvM3",0,path);
	this->m_PKItemDropMaxLevel = GetPrivateProfileInt(section,"PKItemDropMaxLevel",0,path);
	this->m_PKItemDropPet = GetPrivateProfileInt(section,"PKItemDropPet",0,path);
	this->m_PKItemDropWing = GetPrivateProfileInt(section,"PKItemDropWing",0,path);
	this->m_PKItemDropExc = GetPrivateProfileInt(section,"PKItemDropExc",0,path);
	this->m_PKItemDropSet = GetPrivateProfileInt(section,"PKItemDropSet",0,path);
	this->m_PKItemDropJoH = GetPrivateProfileInt(section,"PKItemDropJoH",0,path);
	this->m_PKItemDrop380 = GetPrivateProfileInt(section,"PKItemDrop380",0,path);

	this->m_TradeSwitch = GetPrivateProfileInt(section,"TradeSwitch",0,path);

	this->m_PersonalShopSwitch = GetPrivateProfileInt(section,"PersonalShopSwitch",0,path);
	#if(GAMESERVER_UPDATE>=802)
	this->m_PersonalShopMoneyCommisionRate = GetPrivateProfileInt(section,"PersonalShopMoneyCommisionRate",0,path);
	this->m_PersonalShopJewelCommisionRate = GetPrivateProfileInt(section,"PersonalShopJewelCommisionRate",0,path);
	#endif

	this->m_DuelSwitch = GetPrivateProfileInt(section,"DuelSwitch",0,path);

	#if(GAMESERVER_UPDATE<=401)
	this->m_DuelAnnounceSwitch = GetPrivateProfileInt(section,"DuelAnnounceSwitch",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=402)
	this->m_DuelArenaAnnounceSwitch = GetPrivateProfileInt(section,"DuelArenaAnnounceSwitch",0,path);
	#endif
	this->m_DuelMaxScore = GetPrivateProfileInt(section,"DuelMaxScore",0,path);
	this->m_DuelMaxTime = GetPrivateProfileInt(section,"DuelMaxTime",0,path);

	this->m_GuildCreateSwitch = GetPrivateProfileInt(section,"GuildCreateSwitch",0,path);
	this->m_GuildDeleteSwitch = GetPrivateProfileInt(section,"GuildDeleteSwitch",0,path);
	
	this->m_GuildCreateMinLevel[0] = GetPrivateProfileInt(section,"GuildCreateMinLevel_AL0",0,path);
	this->m_GuildCreateMinLevel[1] = GetPrivateProfileInt(section,"GuildCreateMinLevel_AL1",0,path);
	this->m_GuildCreateMinLevel[2] = GetPrivateProfileInt(section,"GuildCreateMinLevel_AL2",0,path);
	this->m_GuildCreateMinLevel[3] = GetPrivateProfileInt(section,"GuildCreateMinLevel_AL3",0,path);
	
	this->m_GuildCreateMinReset[0] = GetPrivateProfileInt(section,"GuildCreateMinReset_AL0",0,path);
	this->m_GuildCreateMinReset[1] = GetPrivateProfileInt(section,"GuildCreateMinReset_AL1",0,path);
	this->m_GuildCreateMinReset[2] = GetPrivateProfileInt(section,"GuildCreateMinReset_AL2",0,path);
	this->m_GuildCreateMinReset[3] = GetPrivateProfileInt(section,"GuildCreateMinReset_AL3",0,path);
	
	this->m_GuildInsertMaxUser1 = GetPrivateProfileInt(section,"GuildInsertMaxUser1",0,path);
	this->m_GuildInsertMaxUser2 = GetPrivateProfileInt(section,"GuildInsertMaxUser2",0,path);
	this->m_GuildAllianceMinUser = GetPrivateProfileInt(section,"GuildAllianceMinUser",0,path);
	this->m_GuildAllianceMaxCount = GetPrivateProfileInt(section,"GuildAllianceMaxCount",0,path);
	this->m_GuildOwnerDestroyLimit = GetPrivateProfileInt(section,"GuildOwnerDestroyLimit",0,path);

	this->m_ElfBufferMaxLevel[0] = GetPrivateProfileInt(section,"ElfBufferMaxLevel_AL0",0,path);
	this->m_ElfBufferMaxLevel[1] = GetPrivateProfileInt(section,"ElfBufferMaxLevel_AL1",0,path);
	this->m_ElfBufferMaxLevel[2] = GetPrivateProfileInt(section,"ElfBufferMaxLevel_AL2",0,path);
	this->m_ElfBufferMaxLevel[3] = GetPrivateProfileInt(section,"ElfBufferMaxLevel_AL3",0,path);

	this->m_ElfBufferMaxReset[0] = GetPrivateProfileInt(section,"ElfBufferMaxReset_AL0",0,path);
	this->m_ElfBufferMaxReset[1] = GetPrivateProfileInt(section,"ElfBufferMaxReset_AL1",0,path);
	this->m_ElfBufferMaxReset[2] = GetPrivateProfileInt(section,"ElfBufferMaxReset_AL2",0,path);
	this->m_ElfBufferMaxReset[3] = GetPrivateProfileInt(section,"ElfBufferMaxReset_AL3",0,path);

	this->m_ElfBufferDamageConstA = GetPrivateProfileInt(section,"ElfBufferDamageConstA",0,path);
	this->m_ElfBufferDamageConstB = GetPrivateProfileInt(section,"ElfBufferDamageConstB",0,path);
	this->m_ElfBufferDefenseConstA = GetPrivateProfileInt(section,"ElfBufferDefenseConstA",0,path);
	this->m_ElfBufferDefenseConstB = GetPrivateProfileInt(section,"ElfBufferDefenseConstB",0,path);

	this->m_PetExperienceRateDivisor = GetPrivateProfileInt(section,"PetExperienceRateDivisor",0,path);

	this->m_AddExperienceRate[0] = GetPrivateProfileInt(section,"AddExperienceRate_AL0",0,path);
	this->m_AddExperienceRate[1] = GetPrivateProfileInt(section,"AddExperienceRate_AL1",0,path);
	this->m_AddExperienceRate[2] = GetPrivateProfileInt(section,"AddExperienceRate_AL2",0,path);
	this->m_AddExperienceRate[3] = GetPrivateProfileInt(section,"AddExperienceRate_AL3",0,path);

	#if(GAMESERVER_UPDATE>=401)

	this->m_AddMasterExperienceRate[0] = GetPrivateProfileInt(section,"AddMasterExperienceRate_AL0",0,path);
	this->m_AddMasterExperienceRate[1] = GetPrivateProfileInt(section,"AddMasterExperienceRate_AL1",0,path);
	this->m_AddMasterExperienceRate[2] = GetPrivateProfileInt(section,"AddMasterExperienceRate_AL2",0,path);
	this->m_AddMasterExperienceRate[3] = GetPrivateProfileInt(section,"AddMasterExperienceRate_AL3",0,path);

	this->m_MinMasterExperienceMonsterLevel[0] = GetPrivateProfileInt(section,"MinMasterExperienceMonsterLevel_AL0",0,path);
	this->m_MinMasterExperienceMonsterLevel[1] = GetPrivateProfileInt(section,"MinMasterExperienceMonsterLevel_AL1",0,path);
	this->m_MinMasterExperienceMonsterLevel[2] = GetPrivateProfileInt(section,"MinMasterExperienceMonsterLevel_AL2",0,path);
	this->m_MinMasterExperienceMonsterLevel[3] = GetPrivateProfileInt(section,"MinMasterExperienceMonsterLevel_AL3",0,path);

	#endif

	this->m_AddEventExperienceRate[0] = GetPrivateProfileInt(section,"AddEventExperienceRate_AL0",0,path);
	this->m_AddEventExperienceRate[1] = GetPrivateProfileInt(section,"AddEventExperienceRate_AL1",0,path);
	this->m_AddEventExperienceRate[2] = GetPrivateProfileInt(section,"AddEventExperienceRate_AL2",0,path);
	this->m_AddEventExperienceRate[3] = GetPrivateProfileInt(section,"AddEventExperienceRate_AL3",0,path);

	#if(GAMESERVER_UPDATE>=501)

	this->m_AddQuestExperienceRate[0] = GetPrivateProfileInt(section,"AddQuestExperienceRate_AL0",0,path);
	this->m_AddQuestExperienceRate[1] = GetPrivateProfileInt(section,"AddQuestExperienceRate_AL1",0,path);
	this->m_AddQuestExperienceRate[2] = GetPrivateProfileInt(section,"AddQuestExperienceRate_AL2",0,path);
	this->m_AddQuestExperienceRate[3] = GetPrivateProfileInt(section,"AddQuestExperienceRate_AL3",0,path);

	#endif

	this->m_ItemDropTime = GetPrivateProfileInt(section,"ItemDropTime",0,path);

	this->m_ItemDropRate[0] = GetPrivateProfileInt(section,"ItemDropRate_AL0",0,path);
	this->m_ItemDropRate[1] = GetPrivateProfileInt(section,"ItemDropRate_AL1",0,path);
	this->m_ItemDropRate[2] = GetPrivateProfileInt(section,"ItemDropRate_AL2",0,path);
	this->m_ItemDropRate[3] = GetPrivateProfileInt(section,"ItemDropRate_AL3",0,path);

	this->m_MoneyDropTime = GetPrivateProfileInt(section,"MoneyDropTime",0,path);

	this->m_MoneyAmountDropRate[0] = GetPrivateProfileInt(section,"MoneyAmountDropRate_AL0",0,path);
	this->m_MoneyAmountDropRate[1] = GetPrivateProfileInt(section,"MoneyAmountDropRate_AL1",0,path);
	this->m_MoneyAmountDropRate[2] = GetPrivateProfileInt(section,"MoneyAmountDropRate_AL2",0,path);
	this->m_MoneyAmountDropRate[3] = GetPrivateProfileInt(section,"MoneyAmountDropRate_AL3",0,path);

	this->m_WeaponDurabilityRate = GetPrivateProfileInt(section,"WeaponDurabilityRate",0,path);

	this->m_ArmorDurabilityRate = GetPrivateProfileInt(section,"ArmorDurabilityRate",0,path);

	this->m_WingDurabilityRate = GetPrivateProfileInt(section,"WingDurabilityRate",0,path);

	this->m_GuardianDurabilityRate = GetPrivateProfileInt(section,"GuardianDurabilityRate",0,path);

	this->m_PendantDurabilityRate = GetPrivateProfileInt(section,"PendantDurabilityRate",0,path);

	this->m_RingDurabilityRate = GetPrivateProfileInt(section,"RingDurabilityRate",0,path);

	this->m_PetDurabilityRate = GetPrivateProfileInt(section,"PetDurabilityRate",0,path);

	#if(GAMESERVER_UPDATE>=803)

	this->m_MuunDurabilityRate = GetPrivateProfileInt(section,"MuunDurabilityRate",0,path);

	#endif

	#if(GAMESERVER_UPDATE>=603)

	this->m_LuckyMinDurabilityRepair = GetPrivateProfileInt(section,"LuckyMinDurabilityRepair",0,path);

	#endif

	this->m_TradeItemBlock = GetPrivateProfileInt(section,"TradeItemBlock",0,path);

	this->m_TradeItemBlockExc = GetPrivateProfileInt(section,"TradeItemBlockExc",0,path);

	this->m_TradeItemBlockSet = GetPrivateProfileInt(section,"TradeItemBlockSet",0,path);

	#if(GAMESERVER_UPDATE>=201)

	this->m_TradeItemBlockHarmony = GetPrivateProfileInt(section,"TradeItemBlockHarmony",0,path);
	
	#endif

	#if(GAMESERVER_UPDATE>=301)

	this->m_TradeItemBlock380 = GetPrivateProfileInt(section,"TradeItemBlock380",0,path);

	#endif

	#if(GAMESERVER_UPDATE>=602)

	this->m_TradeItemBlockLucky = GetPrivateProfileInt(section,"TradeItemBlockLucky",0,path);

	#endif

	this->m_TradeItemBlockSell = GetPrivateProfileInt(section,"TradeItemBlockSell",0,path);

	#if(GAMESERVER_UPDATE>=602)

	this->m_VaultItemBlockLucky = GetPrivateProfileInt(section,"VaultItemBlockLucky",0,path);

	#endif

	this->m_MaxLevelUp = GetPrivateProfileInt(section,"MaxLevelUp",0,path);

	this->m_MaxLevelUpEvent = GetPrivateProfileInt(section,"MaxLevelUpEvent",0,path);

	#if(GAMESERVER_UPDATE>=501)

	this->m_MaxLevelUpQuest = GetPrivateProfileInt(section,"MaxLevelUpQuest",0,path);

	#endif

	this->m_MaxStatPoint[0] = GetPrivateProfileInt(section,"MaxStatPoint_AL0",0,path);
	this->m_MaxStatPoint[1] = GetPrivateProfileInt(section,"MaxStatPoint_AL1",0,path);
	this->m_MaxStatPoint[2] = GetPrivateProfileInt(section,"MaxStatPoint_AL2",0,path);
	this->m_MaxStatPoint[3] = GetPrivateProfileInt(section,"MaxStatPoint_AL3",0,path);

	this->m_LevelUpPoint[CLASS_DW][0] = GetPrivateProfileInt(section,"DWLevelUpPoint_AL0",0,path);
	this->m_LevelUpPoint[CLASS_DW][1] = GetPrivateProfileInt(section,"DWLevelUpPoint_AL1",0,path);
	this->m_LevelUpPoint[CLASS_DW][2] = GetPrivateProfileInt(section,"DWLevelUpPoint_AL2",0,path);
	this->m_LevelUpPoint[CLASS_DW][3] = GetPrivateProfileInt(section,"DWLevelUpPoint_AL3",0,path);

	this->m_LevelUpPoint[CLASS_DK][0] = GetPrivateProfileInt(section,"DKLevelUpPoint_AL0",0,path);
	this->m_LevelUpPoint[CLASS_DK][1] = GetPrivateProfileInt(section,"DKLevelUpPoint_AL1",0,path);
	this->m_LevelUpPoint[CLASS_DK][2] = GetPrivateProfileInt(section,"DKLevelUpPoint_AL2",0,path);
	this->m_LevelUpPoint[CLASS_DK][3] = GetPrivateProfileInt(section,"DKLevelUpPoint_AL3",0,path);

	this->m_LevelUpPoint[CLASS_FE][0] = GetPrivateProfileInt(section,"FELevelUpPoint_AL0",0,path);
	this->m_LevelUpPoint[CLASS_FE][1] = GetPrivateProfileInt(section,"FELevelUpPoint_AL1",0,path);
	this->m_LevelUpPoint[CLASS_FE][2] = GetPrivateProfileInt(section,"FELevelUpPoint_AL2",0,path);
	this->m_LevelUpPoint[CLASS_FE][3] = GetPrivateProfileInt(section,"FELevelUpPoint_AL3",0,path);

	this->m_LevelUpPoint[CLASS_MG][0] = GetPrivateProfileInt(section,"MGLevelUpPoint_AL0",0,path);
	this->m_LevelUpPoint[CLASS_MG][1] = GetPrivateProfileInt(section,"MGLevelUpPoint_AL1",0,path);
	this->m_LevelUpPoint[CLASS_MG][2] = GetPrivateProfileInt(section,"MGLevelUpPoint_AL2",0,path);
	this->m_LevelUpPoint[CLASS_MG][3] = GetPrivateProfileInt(section,"MGLevelUpPoint_AL3",0,path);

	this->m_LevelUpPoint[CLASS_DL][0] = GetPrivateProfileInt(section,"DLLevelUpPoint_AL0",0,path);
	this->m_LevelUpPoint[CLASS_DL][1] = GetPrivateProfileInt(section,"DLLevelUpPoint_AL1",0,path);
	this->m_LevelUpPoint[CLASS_DL][2] = GetPrivateProfileInt(section,"DLLevelUpPoint_AL2",0,path);
	this->m_LevelUpPoint[CLASS_DL][3] = GetPrivateProfileInt(section,"DLLevelUpPoint_AL3",0,path);

	#if(GAMESERVER_UPDATE>=401)

	this->m_LevelUpPoint[CLASS_SU][0] = GetPrivateProfileInt(section,"SULevelUpPoint_AL0",0,path);
	this->m_LevelUpPoint[CLASS_SU][1] = GetPrivateProfileInt(section,"SULevelUpPoint_AL1",0,path);
	this->m_LevelUpPoint[CLASS_SU][2] = GetPrivateProfileInt(section,"SULevelUpPoint_AL2",0,path);
	this->m_LevelUpPoint[CLASS_SU][3] = GetPrivateProfileInt(section,"SULevelUpPoint_AL3",0,path);

	#endif

	#if(GAMESERVER_UPDATE>=601)

	this->m_LevelUpPoint[CLASS_RF][0] = GetPrivateProfileInt(section,"RFLevelUpPoint_AL0",0,path);
	this->m_LevelUpPoint[CLASS_RF][1] = GetPrivateProfileInt(section,"RFLevelUpPoint_AL1",0,path);
	this->m_LevelUpPoint[CLASS_RF][2] = GetPrivateProfileInt(section,"RFLevelUpPoint_AL2",0,path);
	this->m_LevelUpPoint[CLASS_RF][3] = GetPrivateProfileInt(section,"RFLevelUpPoint_AL3",0,path);

	#endif

	this->m_PlusStatPoint = GetPrivateProfileInt(section,"PlusStatPoint",0,path);

	this->m_PlusStatMinLevel = GetPrivateProfileInt(section,"PlusStatMinLevel",0,path);

	this->m_CharacterCreateSwitch = GetPrivateProfileInt(section,"CharacterCreateSwitch",0,path);

	#if(GAMESERVER_UPDATE>=601)
	this->m_MGCreateType = GetPrivateProfileInt(section,"MGCreateType",0,path);
	this->m_DLCreateType = GetPrivateProfileInt(section,"DLCreateType",0,path);
	this->m_SUCreateType = GetPrivateProfileInt(section,"SUCreateType",0,path);
	this->m_RFCreateType = GetPrivateProfileInt(section,"RFCreateType",0,path);
	#endif

	this->m_MGCreateLevel[0] = GetPrivateProfileInt(section,"MGCreateLevel_AL0",0,path);
	this->m_MGCreateLevel[1] = GetPrivateProfileInt(section,"MGCreateLevel_AL1",0,path);
	this->m_MGCreateLevel[2] = GetPrivateProfileInt(section,"MGCreateLevel_AL2",0,path);
	this->m_MGCreateLevel[3] = GetPrivateProfileInt(section,"MGCreateLevel_AL3",0,path);

	this->m_DLCreateLevel[0] = GetPrivateProfileInt(section,"DLCreateLevel_AL0",0,path);
	this->m_DLCreateLevel[1] = GetPrivateProfileInt(section,"DLCreateLevel_AL1",0,path);
	this->m_DLCreateLevel[2] = GetPrivateProfileInt(section,"DLCreateLevel_AL2",0,path);
	this->m_DLCreateLevel[3] = GetPrivateProfileInt(section,"DLCreateLevel_AL3",0,path);

	#if(GAMESERVER_UPDATE>=401)

	this->m_SUCreateLevel[0] = GetPrivateProfileInt(section,"SUCreateLevel_AL0",0,path);
	this->m_SUCreateLevel[1] = GetPrivateProfileInt(section,"SUCreateLevel_AL1",0,path);
	this->m_SUCreateLevel[2] = GetPrivateProfileInt(section,"SUCreateLevel_AL2",0,path);
	this->m_SUCreateLevel[3] = GetPrivateProfileInt(section,"SUCreateLevel_AL3",0,path);

	#endif

	#if(GAMESERVER_UPDATE>=601)

	this->m_RFCreateLevel[0] = GetPrivateProfileInt(section,"RFCreateLevel_AL0",0,path);
	this->m_RFCreateLevel[1] = GetPrivateProfileInt(section,"RFCreateLevel_AL1",0,path);
	this->m_RFCreateLevel[2] = GetPrivateProfileInt(section,"RFCreateLevel_AL2",0,path);
	this->m_RFCreateLevel[3] = GetPrivateProfileInt(section,"RFCreateLevel_AL3",0,path);

	#endif

	this->m_CharacterDeleteSwitch = GetPrivateProfileInt(section,"CharacterDeleteSwitch",0,path);
	this->m_CharacterDeleteMaxLevel = GetPrivateProfileInt(section,"CharacterDeleteMaxLevel",0,path);

	#if(GAMESERVER_UPDATE>=201)

	this->m_ShieldGaugeRate = GetPrivateProfileInt(section,"ShieldGaugeRate",0,path);

	this->m_ShieldGaugeConstA = GetPrivateProfileInt(section,"ShieldGaugeConstA",0,path);
	this->m_ShieldGaugeConstB = GetPrivateProfileInt(section,"ShieldGaugeConstB",0,path);

	this->m_ShieldGaugeAttackRate = GetPrivateProfileInt(section,"ShieldGaugeAttackRate",0,path);

	this->m_ShieldGaugeAttackComboMiss = GetPrivateProfileInt(section,"ShieldGaugeAttackComboMiss",0,path);

	#endif

	#if(GAMESERVER_UPDATE>=401)

	this->m_MasterSkillTree = GetPrivateProfileInt(section,"MasterSkillTree",0,path);

	this->m_MasterSkillTreePoint = GetPrivateProfileInt(section,"MasterSkillTreePoint",0,path);

	this->m_MasterSkillTreeMaxLevel = GetPrivateProfileInt(section,"MasterSkillTreeMaxLevel",0,path);

	#endif

	#if(GAMESERVER_SHOP==2)

	this->m_CashShopSwitch = GetPrivateProfileInt(section,"CashShopSwitch",0,path);

	this->m_CashShopMaxPoint = GetPrivateProfileInt(section,"CashShopMaxPoint",0,path);

	#endif

	#if(GAMESERVER_SHOP==1)
	
	this->m_PcPointSwitch = GetPrivateProfileInt(section,"PcPointSwitch",0,path);

	this->m_PcPointMaxPoint = GetPrivateProfileInt(section,"PcPointMaxPoint",0,path);

	this->m_PcPointMaxPoint = GET_MAX_WORD_VALUE(this->m_PcPointMaxPoint);

	#endif

	#if(GAMESERVER_UPDATE>=501)

	this->m_GensSystemSwitch = GetPrivateProfileInt(section,"GensSystemSwitch",0,path);

	this->m_GensSystemGuildLock = GetPrivateProfileInt(section,"GensSystemGuildLock",0,path);
	this->m_GensSystemPartyLock = GetPrivateProfileInt(section,"GensSystemPartyLock",0,path);

	this->m_GensSystemInsertMinLevel = GetPrivateProfileInt(section,"GensSystemInsertMinLevel",0,path);

	this->m_GensSystemContributionFloodTime = GetPrivateProfileInt(section,"GensSystemContributionFloodTime",0,path);

	this->m_GensSystemVictimContributionDecrease = GetPrivateProfileInt(section,"GensSystemVictimContributionDecrease",0,path);
	this->m_GensSystemKillerContributionIncrease = GetPrivateProfileInt(section,"GensSystemKillerContributionIncrease",0,path);

	this->m_GensSystemVictimMinContributionDecrease = GetPrivateProfileInt(section,"GensSystemVictimMinContributionDecrease",0,path);
	this->m_GensSystemVictimMaxContributionDecrease = GetPrivateProfileInt(section,"GensSystemVictimMaxContributionDecrease",0,path);

	this->m_GensSystemKillerMinContributionIncrease = GetPrivateProfileInt(section,"GensSystemKillerMinContributionIncrease",0,path);
	this->m_GensSystemKillerMaxContributionIncrease = GetPrivateProfileInt(section,"GensSystemKillerMaxContributionIncrease",0,path);

	this->m_GensSystemStartRewardDay = GetPrivateProfileInt(section,"GensSystemStartRewardDay",0,path);
	this->m_GensSystemFinalRewardDay = GetPrivateProfileInt(section,"GensSystemFinalRewardDay",0,path);

	#endif

	#if(GAMESERVER_UPDATE>=603)

	this->m_HelperSwitch = GetPrivateProfileInt(section,"HelperSwitch",0,path);

	this->m_HelperActiveLevel = GetPrivateProfileInt(section,"HelperActiveLevel",0,path);

	this->m_HelperActiveDelay = GetPrivateProfileInt(section,"HelperActiveDelay",0,path);

	this->m_HelperActiveMoney[0] = GetPrivateProfileInt(section,"HelperActiveMoney1",0,path);
	this->m_HelperActiveMoney[1] = GetPrivateProfileInt(section,"HelperActiveMoney2",0,path);
	this->m_HelperActiveMoney[2] = GetPrivateProfileInt(section,"HelperActiveMoney3",0,path);
	this->m_HelperActiveMoney[3] = GetPrivateProfileInt(section,"HelperActiveMoney4",0,path);
	this->m_HelperActiveMoney[4] = GetPrivateProfileInt(section,"HelperActiveMoney5",0,path);

	this->m_HelperOfflineSwitch = GetPrivateProfileInt(section,"HelperOfflineSwitch",0,path);

	this->m_HelperOfflineCoinGain[0] = GetPrivateProfileInt(section,"HelperOfflineCoinGain_AL0",0,path);
	this->m_HelperOfflineCoinGain[1] = GetPrivateProfileInt(section,"HelperOfflineCoinGain_AL1",0,path);
	this->m_HelperOfflineCoinGain[2] = GetPrivateProfileInt(section,"HelperOfflineCoinGain_AL2",0,path);
	this->m_HelperOfflineCoinGain[3] = GetPrivateProfileInt(section,"HelperOfflineCoinGain_AL3",0,path);

	this->m_HelperOfflineBuffEnable[0] = GetPrivateProfileInt(section,"HelperOfflineBuffEnable_AL0",0,path);
	this->m_HelperOfflineBuffEnable[1] = GetPrivateProfileInt(section,"HelperOfflineBuffEnable_AL1",0,path);
	this->m_HelperOfflineBuffEnable[2] = GetPrivateProfileInt(section,"HelperOfflineBuffEnable_AL2",0,path);
	this->m_HelperOfflineBuffEnable[3] = GetPrivateProfileInt(section,"HelperOfflineBuffEnable_AL3",0,path);

	this->m_HelperOfflinePickEnable[0] = GetPrivateProfileInt(section,"HelperOfflinePickEnable_AL0",0,path);
	this->m_HelperOfflinePickEnable[1] = GetPrivateProfileInt(section,"HelperOfflinePickEnable_AL1",0,path);
	this->m_HelperOfflinePickEnable[2] = GetPrivateProfileInt(section,"HelperOfflinePickEnable_AL2",0,path);
	this->m_HelperOfflinePickEnable[3] = GetPrivateProfileInt(section,"HelperOfflinePickEnable_AL3",0,path);

	this->m_HelperOfflineMaxTimeLimit[0] = GetPrivateProfileInt(section,"HelperOfflineMaxTimeLimit_AL0",0,path);
	this->m_HelperOfflineMaxTimeLimit[1] = GetPrivateProfileInt(section,"HelperOfflineMaxTimeLimit_AL1",0,path);
	this->m_HelperOfflineMaxTimeLimit[2] = GetPrivateProfileInt(section,"HelperOfflineMaxTimeLimit_AL2",0,path);
	this->m_HelperOfflineMaxTimeLimit[3] = GetPrivateProfileInt(section,"HelperOfflineMaxTimeLimit_AL3",0,path);

	#endif

	#if(GAMESERVER_UPDATE>=802)

	this->m_EventInventorySwitch = GetPrivateProfileInt(section,"EventInventorySwitch",0,path);

	this->m_EventInventoryExpireYear = GetPrivateProfileInt(section,"EventInventoryExpireYear",0,path);
	this->m_EventInventoryExpireMonth = GetPrivateProfileInt(section,"EventInventoryExpireMonth",0,path);
	this->m_EventInventoryExpireDay = GetPrivateProfileInt(section,"EventInventoryExpireDay",0,path);

	this->m_MuRummySwitch = GetPrivateProfileInt(section,"MuRummySwitch",0,path);

	this->m_MuRummyRewardScore1 = GetPrivateProfileInt(section,"MuRummyRewardScore1",0,path);
	this->m_MuRummyRewardScore2 = GetPrivateProfileInt(section,"MuRummyRewardScore2",0,path);

	this->m_MuRummyRewardMoneyAmount = GetPrivateProfileInt(section,"MuRummyRewardMoneyAmount",0,path);

	#endif

	this->m_PartyReconnectTime = GetPrivateProfileInt(section,"PartyReconnectTime",0,path);

	this->m_PartyMoneyDistribute = GetPrivateProfileInt(section,"PartyMoneyDistribute",0,path);

	this->m_PartyDisableKillBetweenMembers = GetPrivateProfileInt(section,"PartyDisableKillBetweenMembers",0,path);

	this->m_PartyGeneralExperience[0] = GetPrivateProfileInt(section,"PartyGeneralExperience1",0,path);
	this->m_PartyGeneralExperience[1] = GetPrivateProfileInt(section,"PartyGeneralExperience2",0,path);
	this->m_PartyGeneralExperience[2] = GetPrivateProfileInt(section,"PartyGeneralExperience3",0,path);
	this->m_PartyGeneralExperience[3] = GetPrivateProfileInt(section,"PartyGeneralExperience4",0,path);
	this->m_PartyGeneralExperience[4] = GetPrivateProfileInt(section,"PartyGeneralExperience5",0,path);

	this->m_PartySpecialExperience[0] = GetPrivateProfileInt(section,"PartySpecialExperience1",0,path);
	this->m_PartySpecialExperience[1] = GetPrivateProfileInt(section,"PartySpecialExperience2",0,path);
	this->m_PartySpecialExperience[2] = GetPrivateProfileInt(section,"PartySpecialExperience3",0,path);
	this->m_PartySpecialExperience[3] = GetPrivateProfileInt(section,"PartySpecialExperience4",0,path);
	this->m_PartySpecialExperience[4] = GetPrivateProfileInt(section,"PartySpecialExperience5",0,path);

	this->m_PartyMaxGapLevel = GetPrivateProfileInt(section,"PartyMaxGapLevel",0,path);

	this->m_SoulSuccessRate[0] = GetPrivateProfileInt(section,"SoulSuccessRate_AL0",0,path);
	this->m_SoulSuccessRate[1] = GetPrivateProfileInt(section,"SoulSuccessRate_AL1",0,path);
	this->m_SoulSuccessRate[2] = GetPrivateProfileInt(section,"SoulSuccessRate_AL2",0,path);
	this->m_SoulSuccessRate[3] = GetPrivateProfileInt(section,"SoulSuccessRate_AL3",0,path);

	this->m_LifeSuccessRate[0] = GetPrivateProfileInt(section,"LifeSuccessRate_AL0",0,path);
	this->m_LifeSuccessRate[1] = GetPrivateProfileInt(section,"LifeSuccessRate_AL1",0,path);
	this->m_LifeSuccessRate[2] = GetPrivateProfileInt(section,"LifeSuccessRate_AL2",0,path);
	this->m_LifeSuccessRate[3] = GetPrivateProfileInt(section,"LifeSuccessRate_AL3",0,path);

	#if(GAMESERVER_UPDATE>=201)

	this->m_HarmonySuccessRate[0] = GetPrivateProfileInt(section,"HarmonySuccessRate_AL0",0,path);
	this->m_HarmonySuccessRate[1] = GetPrivateProfileInt(section,"HarmonySuccessRate_AL1",0,path);
	this->m_HarmonySuccessRate[2] = GetPrivateProfileInt(section,"HarmonySuccessRate_AL2",0,path);
	this->m_HarmonySuccessRate[3] = GetPrivateProfileInt(section,"HarmonySuccessRate_AL3",0,path);

	this->m_SmeltStoneSuccessRate1[0] = GetPrivateProfileInt(section,"SmeltStoneSuccessRate1_AL0",0,path);
	this->m_SmeltStoneSuccessRate1[1] = GetPrivateProfileInt(section,"SmeltStoneSuccessRate1_AL1",0,path);
	this->m_SmeltStoneSuccessRate1[2] = GetPrivateProfileInt(section,"SmeltStoneSuccessRate1_AL2",0,path);
	this->m_SmeltStoneSuccessRate1[3] = GetPrivateProfileInt(section,"SmeltStoneSuccessRate1_AL3",0,path);

	this->m_SmeltStoneSuccessRate2[0] = GetPrivateProfileInt(section,"SmeltStoneSuccessRate2_AL0",0,path);
	this->m_SmeltStoneSuccessRate2[1] = GetPrivateProfileInt(section,"SmeltStoneSuccessRate2_AL1",0,path);
	this->m_SmeltStoneSuccessRate2[2] = GetPrivateProfileInt(section,"SmeltStoneSuccessRate2_AL2",0,path);
	this->m_SmeltStoneSuccessRate2[3] = GetPrivateProfileInt(section,"SmeltStoneSuccessRate2_AL3",0,path);

	#endif

	this->m_AddLuckSuccessRate1[0] = GetPrivateProfileInt(section,"AddLuckSuccessRate1_AL0",0,path);
	this->m_AddLuckSuccessRate1[1] = GetPrivateProfileInt(section,"AddLuckSuccessRate1_AL1",0,path);
	this->m_AddLuckSuccessRate1[2] = GetPrivateProfileInt(section,"AddLuckSuccessRate1_AL2",0,path);
	this->m_AddLuckSuccessRate1[3] = GetPrivateProfileInt(section,"AddLuckSuccessRate1_AL3",0,path);

	this->m_AddLuckSuccessRate2[0] = GetPrivateProfileInt(section,"AddLuckSuccessRate2_AL0",0,path);
	this->m_AddLuckSuccessRate2[1] = GetPrivateProfileInt(section,"AddLuckSuccessRate2_AL1",0,path);
	this->m_AddLuckSuccessRate2[2] = GetPrivateProfileInt(section,"AddLuckSuccessRate2_AL2",0,path);
	this->m_AddLuckSuccessRate2[3] = GetPrivateProfileInt(section,"AddLuckSuccessRate2_AL3",0,path);

	this->m_FruitAddPointMin = GetPrivateProfileInt(section,"FruitAddPointMin",0,path);
	this->m_FruitAddPointMax = GetPrivateProfileInt(section,"FruitAddPointMax",0,path);

	this->m_FruitAddPointSuccessRate[0] = GetPrivateProfileInt(section,"FruitAddPointSuccessRate_AL0",0,path);
	this->m_FruitAddPointSuccessRate[1] = GetPrivateProfileInt(section,"FruitAddPointSuccessRate_AL1",0,path);
	this->m_FruitAddPointSuccessRate[2] = GetPrivateProfileInt(section,"FruitAddPointSuccessRate_AL2",0,path);
	this->m_FruitAddPointSuccessRate[3] = GetPrivateProfileInt(section,"FruitAddPointSuccessRate_AL3",0,path);

	this->m_FruitSubPointMin = GetPrivateProfileInt(section,"FruitSubPointMin",0,path);
	this->m_FruitSubPointMax = GetPrivateProfileInt(section,"FruitSubPointMax",0,path);

	this->m_FruitSubPointSuccessRate[0] = GetPrivateProfileInt(section,"FruitSubPointSuccessRate_AL0",0,path);
	this->m_FruitSubPointSuccessRate[1] = GetPrivateProfileInt(section,"FruitSubPointSuccessRate_AL1",0,path);
	this->m_FruitSubPointSuccessRate[2] = GetPrivateProfileInt(section,"FruitSubPointSuccessRate_AL2",0,path);
	this->m_FruitSubPointSuccessRate[3] = GetPrivateProfileInt(section,"FruitSubPointSuccessRate_AL3",0,path);

	this->m_QuestMonsterItemDropParty = GetPrivateProfileInt(section,"QuestMonsterItemDropParty",0,path);
	this->m_QuestMonsterKillParty = GetPrivateProfileInt(section,"QuestMonsterKillParty",0,path);

	#if(GAMESERVER_UPDATE>=501)

	this->m_QuestWorldMonsterItemDropParty = GetPrivateProfileInt(section,"QuestWorldMonsterItemDropParty",0,path);
	this->m_QuestWorldMonsterKillParty = GetPrivateProfileInt(section,"QuestWorldMonsterKillParty",0,path);

	#endif
}

void CServerInfo::ReadEventInfo(char* section,char* path) // OK
{
	#if(GAMESERVER_UPDATE>=701)

	this->m_AcheronGuardianEvent = GetPrivateProfileInt(section,"AcheronGuardianEvent",0,path);

	this->m_AcheronGuardianEventTime = GetPrivateProfileInt(section,"AcheronGuardianEventTime",0,path);

	#endif

	#if(GAMESERVER_UPDATE>=702)

	this->m_ArcaBattleEvent = GetPrivateProfileInt(section,"ArcaBattleEvent",0,path);
	
	this->m_ArcaBattleMinGuildMember = GetPrivateProfileInt(section,"ArcaBattleMinGuildMember",0,path);

	this->m_ArcaBattleMaxGuildMember = GetPrivateProfileInt(section,"ArcaBattleMaxGuildMember",0,path);

	#endif

	this->m_BloodCastleEvent = GetPrivateProfileInt(section,"BloodCastleEvent",0,path);

	this->m_BloodCastleMaxUser = GetPrivateProfileInt(section,"BloodCastleMaxUser",0,path);

	this->m_BloodCastleMaxEntryCount[0] = GetPrivateProfileInt(section,"BloodCastleMaxEntryCount_AL0",0,path);
	this->m_BloodCastleMaxEntryCount[1] = GetPrivateProfileInt(section,"BloodCastleMaxEntryCount_AL1",0,path);
	this->m_BloodCastleMaxEntryCount[2] = GetPrivateProfileInt(section,"BloodCastleMaxEntryCount_AL2",0,path);
	this->m_BloodCastleMaxEntryCount[3] = GetPrivateProfileInt(section,"BloodCastleMaxEntryCount_AL3",0,path);

	this->m_BloodCastleNPCWithoutEntrance = GetPrivateProfileInt(section,"BloodCastleNPCWithoutEntrance",0,path);

	this->m_BonusManagerSwitch = GetPrivateProfileInt(section,"BonusManagerSwitch",0,path);

	this->m_CastleDeepEvent = GetPrivateProfileInt(section,"CastleDeepEvent",0,path);

	this->m_CastleDeepEventTime = GetPrivateProfileInt(section,"CastleDeepEventTime",0,path);

	this->m_CastleSiegeEvent = GetPrivateProfileInt(section,"CastleSiegeEvent",0,path);

	this->m_CastleSiegeCycleStartYear = GetPrivateProfileInt(section,"CastleSiegeCycleStartYear",0,path);
	this->m_CastleSiegeCycleStartMonth = GetPrivateProfileInt(section,"CastleSiegeCycleStartMonth",0,path);
	this->m_CastleSiegeCycleStartDay = GetPrivateProfileInt(section,"CastleSiegeCycleStartDay",0,path);

	this->m_CastleSiegeSpecificState = GetPrivateProfileInt(section,"CastleSiegeSpecificState",0,path);

	this->m_CastleSiegeStateStartYear = GetPrivateProfileInt(section,"CastleSiegeStateStartYear",0,path);
	this->m_CastleSiegeStateStartMonth = GetPrivateProfileInt(section,"CastleSiegeStateStartMonth",0,path);
	this->m_CastleSiegeStateStartDay = GetPrivateProfileInt(section,"CastleSiegeStateStartDay",0,path);

	this->m_CastleSiegeStateStartHour = GetPrivateProfileInt(section,"CastleSiegeStateStartHour",0,path);
	this->m_CastleSiegeStateStartMinute = GetPrivateProfileInt(section,"CastleSiegeStateStartMinute",0,path);

	this->m_CastleSiegeOffensiveWeaponDamage = GetPrivateProfileInt(section,"CastleSiegeOffensiveWeaponDamage",0,path);
	this->m_CastleSiegeDefensiveWeaponDamage = GetPrivateProfileInt(section,"CastleSiegeDefensiveWeaponDamage",0,path);

	this->m_CastleSiegeDecayAccumulatedTimeValue = GetPrivateProfileInt(section,"CastleSiegeDecayAccumulatedTimeValue",0,path);
	this->m_CastleSiegeLowerAccumulatedTimeValue = GetPrivateProfileInt(section,"CastleSiegeLowerAccumulatedTimeValue",0,path);

	this->m_ChaosCastleEvent = GetPrivateProfileInt(section,"ChaosCastleEvent",0,path);

	this->m_ChaosCastleMinUser = GetPrivateProfileInt(section,"ChaosCastleMinUser",0,path);

	this->m_ChaosCastleBlowUserRate = GetPrivateProfileInt(section,"ChaosCastleBlowUserRate",0,path);

	this->m_ChaosCastleMoneyRate = GetPrivateProfileInt(section,"ChaosCastleMoneyRate",0,path);

	this->m_ChaosCastleMaxEntryCount[0] = GetPrivateProfileInt(section,"ChaosCastleMaxEntryCount_AL0",0,path);
	this->m_ChaosCastleMaxEntryCount[1] = GetPrivateProfileInt(section,"ChaosCastleMaxEntryCount_AL1",0,path);
	this->m_ChaosCastleMaxEntryCount[2] = GetPrivateProfileInt(section,"ChaosCastleMaxEntryCount_AL2",0,path);
	this->m_ChaosCastleMaxEntryCount[3] = GetPrivateProfileInt(section,"ChaosCastleMaxEntryCount_AL3",0,path);

	#if(GAMESERVER_UPDATE>=803)

	this->m_ChaosCastleSurvivalEvent = GetPrivateProfileInt(section,"ChaosCastleSurvivalEvent",0,path);
	
	this->m_ChaosCastleSurvivalMinUser = GetPrivateProfileInt(section,"ChaosCastleSurvivalMinUser",0,path);
	
	this->m_ChaosCastleSurvivalMinLevel = GetPrivateProfileInt(section,"ChaosCastleSurvivalMinLevel",0,path);

	this->m_ChaosCastleSurvivalBlowUserRate = GetPrivateProfileInt(section,"ChaosCastleSurvivalBlowUserRate",0,path);

	#endif

	#if(GAMESERVER_UPDATE>=201)

	gCrywolfSync.SetEnableCrywolf((this->m_CrywolfEvent = GetPrivateProfileInt(section,"CrywolfEvent",0,path)));

	gCrywolfSync.SetApplyBenefit((this->m_CrywolfBenefitSwitch = GetPrivateProfileInt(section,"CrywolfBenefitSwitch",0,path)));

	gCrywolfSync.SetPlusChaosRate((this->m_CrywolfBenefitChaosRate = GetPrivateProfileInt(section,"CrywolfBenefitChaosRate",0,path)));

	gCrywolfSync.SetMonHPBenefitRate((this->m_CrywolfBenefitMonsterLife = GetPrivateProfileInt(section,"CrywolfBenefitMonsterLife",0,path)));

	gCrywolfSync.SetApplyPenalty((this->m_CrywolfPenaltySwitch = GetPrivateProfileInt(section,"CrywolfPenaltySwitch",0,path)));

	gCrywolfSync.SetGemDropPenaltiyRate((this->m_CrywolfPenaltyJewelDrop = GetPrivateProfileInt(section,"CrywolfPenaltyJewelDrop",0,path)));

	gCrywolfSync.SetGettingExpPenaltyRate((this->m_CrywolfPenaltyExperience = GetPrivateProfileInt(section,"CrywolfPenaltyExperience",0,path)));

	gCrywolfSync.SetKundunHPRefillState((this->m_CrywolfPenaltyKundunHPRefill = GetPrivateProfileInt(section,"CrywolfPenaltyKundunHPRefill",0,path)));

	#endif

	this->m_DevilSquareEvent = GetPrivateProfileInt(section,"DevilSquareEvent",0,path);

	this->m_DevilSquareMaxUser = GetPrivateProfileInt(section,"DevilSquareMaxUser",0,path);

	this->m_DevilSquareMaxEntryCount[0] = GetPrivateProfileInt(section,"DevilSquareMaxEntryCount_AL0",0,path);
	this->m_DevilSquareMaxEntryCount[1] = GetPrivateProfileInt(section,"DevilSquareMaxEntryCount_AL1",0,path);
	this->m_DevilSquareMaxEntryCount[2] = GetPrivateProfileInt(section,"DevilSquareMaxEntryCount_AL2",0,path);
	this->m_DevilSquareMaxEntryCount[3] = GetPrivateProfileInt(section,"DevilSquareMaxEntryCount_AL3",0,path);

	this->m_DevilSquareNPCWithoutEntrance = GetPrivateProfileInt(section,"DevilSquareNPCWithoutEntrance",0,path);

	#if(GAMESERVER_UPDATE>=501)

	this->m_DoubleGoerEvent = GetPrivateProfileInt(section,"DoubleGoerEvent",0,path);

	this->m_DoubleGoerDifficultRate = GetPrivateProfileInt(section,"DoubleGoerDifficultRate",0,path);

	this->m_DoubleGoerMaxEntryCount[0] = GetPrivateProfileInt(section,"DoubleGoerMaxEntryCount_AL0",0,path);
	this->m_DoubleGoerMaxEntryCount[1] = GetPrivateProfileInt(section,"DoubleGoerMaxEntryCount_AL1",0,path);
	this->m_DoubleGoerMaxEntryCount[2] = GetPrivateProfileInt(section,"DoubleGoerMaxEntryCount_AL2",0,path);
	this->m_DoubleGoerMaxEntryCount[3] = GetPrivateProfileInt(section,"DoubleGoerMaxEntryCount_AL3",0,path);

	#endif

	this->m_DropEventSwitch = GetPrivateProfileInt(section,"DropEventSwitch",0,path);

	#if(GAMESERVER_UPDATE>=301)

	this->m_IllusionTempleEvent = GetPrivateProfileInt(section,"IllusionTempleEvent",0,path);

	this->m_IllusionTempleMinUser = GetPrivateProfileInt(section,"IllusionTempleMinUser",0,path);

	this->m_IllusionTempleRewardFenrirRate = GetPrivateProfileInt(section,"IllusionTempleRewardFenrirRate",0,path);

	this->m_IllusionTempleMaxEntryCount[0] = GetPrivateProfileInt(section,"IllusionTempleMaxEntryCount_AL0",0,path);
	this->m_IllusionTempleMaxEntryCount[1] = GetPrivateProfileInt(section,"IllusionTempleMaxEntryCount_AL1",0,path);
	this->m_IllusionTempleMaxEntryCount[2] = GetPrivateProfileInt(section,"IllusionTempleMaxEntryCount_AL2",0,path);
	this->m_IllusionTempleMaxEntryCount[3] = GetPrivateProfileInt(section,"IllusionTempleMaxEntryCount_AL3",0,path);

	this->m_IllusionTempleNPCWithoutEntrance = GetPrivateProfileInt(section,"IllusionTempleNPCWithoutEntrance",0,path);

	#endif

	#if(GAMESERVER_UPDATE>=501)

	this->m_ImperialGuardianEvent = GetPrivateProfileInt(section,"ImperialGuardianEvent",0,path);

	this->m_ImperialGuardianDifficultRate = GetPrivateProfileInt(section,"ImperialGuardianDifficultRate",0,path);

	this->m_ImperialGuardianRewardExperience1 = GetPrivateProfileInt(section,"ImperialGuardianRewardExperience1",0,path);
	this->m_ImperialGuardianRewardExperience2 = GetPrivateProfileInt(section,"ImperialGuardianRewardExperience2",0,path);

	this->m_ImperialGuardianMaxEntryCount[0] = GetPrivateProfileInt(section,"ImperialGuardianMaxEntryCount_AL0",0,path);
	this->m_ImperialGuardianMaxEntryCount[1] = GetPrivateProfileInt(section,"ImperialGuardianMaxEntryCount_AL1",0,path);
	this->m_ImperialGuardianMaxEntryCount[2] = GetPrivateProfileInt(section,"ImperialGuardianMaxEntryCount_AL2",0,path);
	this->m_ImperialGuardianMaxEntryCount[3] = GetPrivateProfileInt(section,"ImperialGuardianMaxEntryCount_AL3",0,path);

	#endif

	this->m_InvasionManagerSwitch = GetPrivateProfileInt(section,"InvasionManagerSwitch",0,path);

	#if(GAMESERVER_UPDATE>=201)

	this->m_KanturuEvent = GetPrivateProfileInt(section,"KanturuEvent",0,path);

	#endif

	#if(GAMESERVER_UPDATE>=402)

	this->m_MossMerchantEvent = GetPrivateProfileInt(section,"MossMerchantEvent",0,path);

	this->m_MossMerchantEventTime = GetPrivateProfileInt(section,"MossMerchantEventTime",0,path);

	#endif

	#if(GAMESERVER_UPDATE>=401)

	this->m_RaklionEvent = GetPrivateProfileInt(section,"RaklionEvent",0,path);

	#endif
}

void CServerInfo::ReadHackInfo(char* section,char* path) // OK
{
	this->m_CheckSpeedHack = GetPrivateProfileInt(section,"CheckSpeedHack",0,path);
	this->m_CheckSpeedHackTolerance = GetPrivateProfileInt(section,"CheckSpeedHackTolerance",0,path);
	this->m_CheckSpeedHackAction = GetPrivateProfileInt(section,"CheckSpeedHackAction",0,path);

	this->m_CheckLatencyHack = GetPrivateProfileInt(section,"CheckLatencyHack",0,path);
	this->m_CheckLatencyHackTolerance = GetPrivateProfileInt(section,"CheckLatencyHackTolerance",0,path);
	this->m_CheckLatencyHackAction = GetPrivateProfileInt(section,"CheckLatencyHackAction",0,path);

	this->m_CheckAutoPotionHack = GetPrivateProfileInt(section,"CheckAutoPotionHack",0,path);
	this->m_CheckAutoPotionHackTolerance = GetPrivateProfileInt(section,"CheckAutoPotionHackTolerance",0,path);
	this->m_CheckAutoPotionHackAction = GetPrivateProfileInt(section,"CheckAutoPotionHackAction",0,path);

	this->m_CheckAutoComboHack = GetPrivateProfileInt(section,"CheckAutoComboHack",0,path);
	this->m_CheckAutoComboHackTolerance = GetPrivateProfileInt(section,"CheckAutoComboHackTolerance",0,path);
	this->m_CheckAutoComboHackAction = GetPrivateProfileInt(section,"CheckAutoComboHackAction",0,path);

	this->m_CheckMoveHack = GetPrivateProfileInt(section,"CheckMoveHack",0,path);
	this->m_CheckMoveHackMaxDelay = GetPrivateProfileInt(section,"CheckMoveHackMaxDelay",0,path);
	this->m_CheckMoveHackMaxCount = GetPrivateProfileInt(section,"CheckMoveHackMaxCount",0,path);
	this->m_CheckMoveHackAction = GetPrivateProfileInt(section,"CheckMoveHackAction",0,path);
}

void CServerInfo::ReadItemInfo(char* section,char* path) // OK
{
	this->m_TransformationRing1 = GetPrivateProfileInt(section,"TransformationRing1",0,path);
	this->m_TransformationRing2 = GetPrivateProfileInt(section,"TransformationRing2",0,path);
	this->m_TransformationRing3 = GetPrivateProfileInt(section,"TransformationRing3",0,path);
	this->m_TransformationRing4 = GetPrivateProfileInt(section,"TransformationRing4",0,path);
	this->m_TransformationRing5 = GetPrivateProfileInt(section,"TransformationRing5",0,path);
	this->m_TransformationRing6 = GetPrivateProfileInt(section,"TransformationRing6",0,path);

	this->m_SatanIncDamageConstA = GetPrivateProfileInt(section,"SatanIncDamageConstA",0,path);

	this->m_DinorantIncDamageConstA = GetPrivateProfileInt(section,"DinorantIncDamageConstA",0,path);

	#if(GAMESERVER_UPDATE>=201)

	this->m_BlackFenrirIncDamageConstA = GetPrivateProfileInt(section,"BlackFenrirIncDamageConstA",0,path);

	#endif

	#if(GAMESERVER_UPDATE>=401)

	this->m_DemonIncDamageConstA = GetPrivateProfileInt(section,"DemonIncDamageConstA",0,path);

	this->m_SkeletonIncDamageConstA = GetPrivateProfileInt(section,"SkeletonIncDamageConstA",0,path);

	#endif

	this->m_AngelDecDamageConstA = GetPrivateProfileInt(section,"AngelDecDamageConstA",0,path);

	this->m_DinorantDecDamageConstA = GetPrivateProfileInt(section,"DinorantDecDamageConstA",0,path);
	this->m_DinorantDecDamageConstB = GetPrivateProfileInt(section,"DinorantDecDamageConstB",0,path);

	this->m_DarkHorseDecDamageConstA = GetPrivateProfileInt(section,"DarkHorseDecDamageConstA",0,path);
	this->m_DarkHorseDecDamageConstB = GetPrivateProfileInt(section,"DarkHorseDecDamageConstB",0,path);

	#if(GAMESERVER_UPDATE>=201)

	this->m_BlueFenrirDecDamageConstA = GetPrivateProfileInt(section,"BlueFenrirDecDamageConstA",0,path);

	#endif

	#if(GAMESERVER_UPDATE>=401)

	this->m_GuardianSpiritDecDamageConstA = GetPrivateProfileInt(section,"GuardianSpiritDecDamageConstA",0,path);

	#endif

	this->m_ApplePotionRate[CLASS_DW] = GetPrivateProfileInt(section,"DWApplePotionRate",0,path);
	this->m_ApplePotionRate[CLASS_DK] = GetPrivateProfileInt(section,"DKApplePotionRate",0,path);
	this->m_ApplePotionRate[CLASS_FE] = GetPrivateProfileInt(section,"FEApplePotionRate",0,path);
	this->m_ApplePotionRate[CLASS_MG] = GetPrivateProfileInt(section,"MGApplePotionRate",0,path);
	this->m_ApplePotionRate[CLASS_DL] = GetPrivateProfileInt(section,"DLApplePotionRate",0,path);
	#if(GAMESERVER_UPDATE>=401)
	this->m_ApplePotionRate[CLASS_SU] = GetPrivateProfileInt(section,"SUApplePotionRate",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_ApplePotionRate[CLASS_RF] = GetPrivateProfileInt(section,"RFApplePotionRate",0,path);
	#endif

	this->m_SmallLifePotionRate[CLASS_DW] = GetPrivateProfileInt(section,"DWSmallLifePotionRate",0,path);
	this->m_SmallLifePotionRate[CLASS_DK] = GetPrivateProfileInt(section,"DKSmallLifePotionRate",0,path);
	this->m_SmallLifePotionRate[CLASS_FE] = GetPrivateProfileInt(section,"FESmallLifePotionRate",0,path);
	this->m_SmallLifePotionRate[CLASS_MG] = GetPrivateProfileInt(section,"MGSmallLifePotionRate",0,path);
	this->m_SmallLifePotionRate[CLASS_DL] = GetPrivateProfileInt(section,"DLSmallLifePotionRate",0,path);
	#if(GAMESERVER_UPDATE>=401)
	this->m_SmallLifePotionRate[CLASS_SU] = GetPrivateProfileInt(section,"SUSmallLifePotionRate",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_SmallLifePotionRate[CLASS_RF] = GetPrivateProfileInt(section,"RFSmallLifePotionRate",0,path);
	#endif

	this->m_MidleLifePotionRate[CLASS_DW] = GetPrivateProfileInt(section,"DWMidleLifePotionRate",0,path);
	this->m_MidleLifePotionRate[CLASS_DK] = GetPrivateProfileInt(section,"DKMidleLifePotionRate",0,path);
	this->m_MidleLifePotionRate[CLASS_FE] = GetPrivateProfileInt(section,"FEMidleLifePotionRate",0,path);
	this->m_MidleLifePotionRate[CLASS_MG] = GetPrivateProfileInt(section,"MGMidleLifePotionRate",0,path);
	this->m_MidleLifePotionRate[CLASS_DL] = GetPrivateProfileInt(section,"DLMidleLifePotionRate",0,path);
	#if(GAMESERVER_UPDATE>=401)
	this->m_MidleLifePotionRate[CLASS_SU] = GetPrivateProfileInt(section,"SUMidleLifePotionRate",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_MidleLifePotionRate[CLASS_RF] = GetPrivateProfileInt(section,"RFMidleLifePotionRate",0,path);
	#endif

	this->m_LargeLifePotionRate[CLASS_DW] = GetPrivateProfileInt(section,"DWLargeLifePotionRate",0,path);
	this->m_LargeLifePotionRate[CLASS_DK] = GetPrivateProfileInt(section,"DKLargeLifePotionRate",0,path);
	this->m_LargeLifePotionRate[CLASS_FE] = GetPrivateProfileInt(section,"FELargeLifePotionRate",0,path);
	this->m_LargeLifePotionRate[CLASS_MG] = GetPrivateProfileInt(section,"MGLargeLifePotionRate",0,path);
	this->m_LargeLifePotionRate[CLASS_DL] = GetPrivateProfileInt(section,"DLLargeLifePotionRate",0,path);
	#if(GAMESERVER_UPDATE>=401)
	this->m_LargeLifePotionRate[CLASS_SU] = GetPrivateProfileInt(section,"SULargeLifePotionRate",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_LargeLifePotionRate[CLASS_RF] = GetPrivateProfileInt(section,"RFLargeLifePotionRate",0,path);
	#endif

	this->m_SmallManaPotionRate[CLASS_DW] = GetPrivateProfileInt(section,"DWSmallManaPotionRate",0,path);
	this->m_SmallManaPotionRate[CLASS_DK] = GetPrivateProfileInt(section,"DKSmallManaPotionRate",0,path);
	this->m_SmallManaPotionRate[CLASS_FE] = GetPrivateProfileInt(section,"FESmallManaPotionRate",0,path);
	this->m_SmallManaPotionRate[CLASS_MG] = GetPrivateProfileInt(section,"MGSmallManaPotionRate",0,path);
	this->m_SmallManaPotionRate[CLASS_DL] = GetPrivateProfileInt(section,"DLSmallManaPotionRate",0,path);
	#if(GAMESERVER_UPDATE>=401)
	this->m_SmallManaPotionRate[CLASS_SU] = GetPrivateProfileInt(section,"SUSmallManaPotionRate",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_SmallManaPotionRate[CLASS_RF] = GetPrivateProfileInt(section,"RFSmallManaPotionRate",0,path);
	#endif

	this->m_MidleManaPotionRate[CLASS_DW] = GetPrivateProfileInt(section,"DWMidleManaPotionRate",0,path);
	this->m_MidleManaPotionRate[CLASS_DK] = GetPrivateProfileInt(section,"DKMidleManaPotionRate",0,path);
	this->m_MidleManaPotionRate[CLASS_FE] = GetPrivateProfileInt(section,"FEMidleManaPotionRate",0,path);
	this->m_MidleManaPotionRate[CLASS_MG] = GetPrivateProfileInt(section,"MGMidleManaPotionRate",0,path);
	this->m_MidleManaPotionRate[CLASS_DL] = GetPrivateProfileInt(section,"DLMidleManaPotionRate",0,path);
	#if(GAMESERVER_UPDATE>=401)
	this->m_MidleManaPotionRate[CLASS_SU] = GetPrivateProfileInt(section,"SUMidleManaPotionRate",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_MidleManaPotionRate[CLASS_RF] = GetPrivateProfileInt(section,"RFMidleManaPotionRate",0,path);
	#endif

	this->m_LargeManaPotionRate[CLASS_DW] = GetPrivateProfileInt(section,"DWLargeManaPotionRate",0,path);
	this->m_LargeManaPotionRate[CLASS_DK] = GetPrivateProfileInt(section,"DKLargeManaPotionRate",0,path);
	this->m_LargeManaPotionRate[CLASS_FE] = GetPrivateProfileInt(section,"FELargeManaPotionRate",0,path);
	this->m_LargeManaPotionRate[CLASS_MG] = GetPrivateProfileInt(section,"MGLargeManaPotionRate",0,path);
	this->m_LargeManaPotionRate[CLASS_DL] = GetPrivateProfileInt(section,"DLLargeManaPotionRate",0,path);
	#if(GAMESERVER_UPDATE>=401)
	this->m_LargeManaPotionRate[CLASS_SU] = GetPrivateProfileInt(section,"SULargeManaPotionRate",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_LargeManaPotionRate[CLASS_RF] = GetPrivateProfileInt(section,"RFLargeManaPotionRate",0,path);
	#endif

	#if(GAMESERVER_UPDATE>=201)

	this->m_SmallShieldPotionRate[CLASS_DW] = GetPrivateProfileInt(section,"DWSmallShieldPotionRate",0,path);
	this->m_SmallShieldPotionRate[CLASS_DK] = GetPrivateProfileInt(section,"DKSmallShieldPotionRate",0,path);
	this->m_SmallShieldPotionRate[CLASS_FE] = GetPrivateProfileInt(section,"FESmallShieldPotionRate",0,path);
	this->m_SmallShieldPotionRate[CLASS_MG] = GetPrivateProfileInt(section,"MGSmallShieldPotionRate",0,path);
	this->m_SmallShieldPotionRate[CLASS_DL] = GetPrivateProfileInt(section,"DLSmallShieldPotionRate",0,path);
	#if(GAMESERVER_UPDATE>=401)
	this->m_SmallShieldPotionRate[CLASS_SU] = GetPrivateProfileInt(section,"SUSmallShieldPotionRate",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_SmallShieldPotionRate[CLASS_RF] = GetPrivateProfileInt(section,"RFSmallShieldPotionRate",0,path);
	#endif

	this->m_MidleShieldPotionRate[CLASS_DW] = GetPrivateProfileInt(section,"DWMidleShieldPotionRate",0,path);
	this->m_MidleShieldPotionRate[CLASS_DK] = GetPrivateProfileInt(section,"DKMidleShieldPotionRate",0,path);
	this->m_MidleShieldPotionRate[CLASS_FE] = GetPrivateProfileInt(section,"FEMidleShieldPotionRate",0,path);
	this->m_MidleShieldPotionRate[CLASS_MG] = GetPrivateProfileInt(section,"MGMidleShieldPotionRate",0,path);
	this->m_MidleShieldPotionRate[CLASS_DL] = GetPrivateProfileInt(section,"DLMidleShieldPotionRate",0,path);
	#if(GAMESERVER_UPDATE>=401)
	this->m_MidleShieldPotionRate[CLASS_SU] = GetPrivateProfileInt(section,"SUMidleShieldPotionRate",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_MidleShieldPotionRate[CLASS_RF] = GetPrivateProfileInt(section,"RFMidleShieldPotionRate",0,path);
	#endif

	this->m_LargeShieldPotionRate[CLASS_DW] = GetPrivateProfileInt(section,"DWLargeShieldPotionRate",0,path);
	this->m_LargeShieldPotionRate[CLASS_DK] = GetPrivateProfileInt(section,"DKLargeShieldPotionRate",0,path);
	this->m_LargeShieldPotionRate[CLASS_FE] = GetPrivateProfileInt(section,"FELargeShieldPotionRate",0,path);
	this->m_LargeShieldPotionRate[CLASS_MG] = GetPrivateProfileInt(section,"MGLargeShieldPotionRate",0,path);
	this->m_LargeShieldPotionRate[CLASS_DL] = GetPrivateProfileInt(section,"DLLargeShieldPotionRate",0,path);
	#if(GAMESERVER_UPDATE>=401)
	this->m_LargeShieldPotionRate[CLASS_SU] = GetPrivateProfileInt(section,"SULargeShieldPotionRate",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_LargeShieldPotionRate[CLASS_RF] = GetPrivateProfileInt(section,"RFLargeShieldPotionRate",0,path);
	#endif

	this->m_SmallCompoundPotionRate1[CLASS_DW] = GetPrivateProfileInt(section,"DWSmallCompoundPotionRate1",0,path);
	this->m_SmallCompoundPotionRate1[CLASS_DK] = GetPrivateProfileInt(section,"DKSmallCompoundPotionRate1",0,path);
	this->m_SmallCompoundPotionRate1[CLASS_FE] = GetPrivateProfileInt(section,"FESmallCompoundPotionRate1",0,path);
	this->m_SmallCompoundPotionRate1[CLASS_MG] = GetPrivateProfileInt(section,"MGSmallCompoundPotionRate1",0,path);
	this->m_SmallCompoundPotionRate1[CLASS_DL] = GetPrivateProfileInt(section,"DLSmallCompoundPotionRate1",0,path);
	#if(GAMESERVER_UPDATE>=401)
	this->m_SmallCompoundPotionRate1[CLASS_SU] = GetPrivateProfileInt(section,"SUSmallCompoundPotionRate1",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_SmallCompoundPotionRate1[CLASS_RF] = GetPrivateProfileInt(section,"RFSmallCompoundPotionRate1",0,path);
	#endif

	this->m_SmallCompoundPotionRate2[CLASS_DW] = GetPrivateProfileInt(section,"DWSmallCompoundPotionRate2",0,path);
	this->m_SmallCompoundPotionRate2[CLASS_DK] = GetPrivateProfileInt(section,"DKSmallCompoundPotionRate2",0,path);
	this->m_SmallCompoundPotionRate2[CLASS_FE] = GetPrivateProfileInt(section,"FESmallCompoundPotionRate2",0,path);
	this->m_SmallCompoundPotionRate2[CLASS_MG] = GetPrivateProfileInt(section,"MGSmallCompoundPotionRate2",0,path);
	this->m_SmallCompoundPotionRate2[CLASS_DL] = GetPrivateProfileInt(section,"DLSmallCompoundPotionRate2",0,path);
	#if(GAMESERVER_UPDATE>=401)
	this->m_SmallCompoundPotionRate2[CLASS_SU] = GetPrivateProfileInt(section,"SUSmallCompoundPotionRate2",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_SmallCompoundPotionRate2[CLASS_RF] = GetPrivateProfileInt(section,"RFSmallCompoundPotionRate2",0,path);
	#endif


	this->m_MidleCompoundPotionRate1[CLASS_DW] = GetPrivateProfileInt(section,"DWMidleCompoundPotionRate1",0,path);
	this->m_MidleCompoundPotionRate1[CLASS_DK] = GetPrivateProfileInt(section,"DKMidleCompoundPotionRate1",0,path);
	this->m_MidleCompoundPotionRate1[CLASS_FE] = GetPrivateProfileInt(section,"FEMidleCompoundPotionRate1",0,path);
	this->m_MidleCompoundPotionRate1[CLASS_MG] = GetPrivateProfileInt(section,"MGMidleCompoundPotionRate1",0,path);
	this->m_MidleCompoundPotionRate1[CLASS_DL] = GetPrivateProfileInt(section,"DLMidleCompoundPotionRate1",0,path);
	#if(GAMESERVER_UPDATE>=401)
	this->m_MidleCompoundPotionRate1[CLASS_SU] = GetPrivateProfileInt(section,"SUMidleCompoundPotionRate1",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_MidleCompoundPotionRate1[CLASS_RF] = GetPrivateProfileInt(section,"RFMidleCompoundPotionRate1",0,path);
	#endif

	this->m_MidleCompoundPotionRate2[CLASS_DW] = GetPrivateProfileInt(section,"DWMidleCompoundPotionRate2",0,path);
	this->m_MidleCompoundPotionRate2[CLASS_DK] = GetPrivateProfileInt(section,"DKMidleCompoundPotionRate2",0,path);
	this->m_MidleCompoundPotionRate2[CLASS_FE] = GetPrivateProfileInt(section,"FEMidleCompoundPotionRate2",0,path);
	this->m_MidleCompoundPotionRate2[CLASS_MG] = GetPrivateProfileInt(section,"MGMidleCompoundPotionRate2",0,path);
	this->m_MidleCompoundPotionRate2[CLASS_DL] = GetPrivateProfileInt(section,"DLMidleCompoundPotionRate2",0,path);
	#if(GAMESERVER_UPDATE>=401)
	this->m_MidleCompoundPotionRate2[CLASS_SU] = GetPrivateProfileInt(section,"SUMidleCompoundPotionRate2",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_MidleCompoundPotionRate2[CLASS_RF] = GetPrivateProfileInt(section,"RFMidleCompoundPotionRate2",0,path);
	#endif

	this->m_LargeCompoundPotionRate1[CLASS_DW] = GetPrivateProfileInt(section,"DWLargeCompoundPotionRate1",0,path);
	this->m_LargeCompoundPotionRate1[CLASS_DK] = GetPrivateProfileInt(section,"DKLargeCompoundPotionRate1",0,path);
	this->m_LargeCompoundPotionRate1[CLASS_FE] = GetPrivateProfileInt(section,"FELargeCompoundPotionRate1",0,path);
	this->m_LargeCompoundPotionRate1[CLASS_MG] = GetPrivateProfileInt(section,"MGLargeCompoundPotionRate1",0,path);
	this->m_LargeCompoundPotionRate1[CLASS_DL] = GetPrivateProfileInt(section,"DLLargeCompoundPotionRate1",0,path);
	#if(GAMESERVER_UPDATE>=401)
	this->m_LargeCompoundPotionRate1[CLASS_SU] = GetPrivateProfileInt(section,"SULargeCompoundPotionRate1",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_LargeCompoundPotionRate1[CLASS_RF] = GetPrivateProfileInt(section,"RFLargeCompoundPotionRate1",0,path);
	#endif

	this->m_LargeCompoundPotionRate2[CLASS_DW] = GetPrivateProfileInt(section,"DWLargeCompoundPotionRate2",0,path);
	this->m_LargeCompoundPotionRate2[CLASS_DK] = GetPrivateProfileInt(section,"DKLargeCompoundPotionRate2",0,path);
	this->m_LargeCompoundPotionRate2[CLASS_FE] = GetPrivateProfileInt(section,"FELargeCompoundPotionRate2",0,path);
	this->m_LargeCompoundPotionRate2[CLASS_MG] = GetPrivateProfileInt(section,"MGLargeCompoundPotionRate2",0,path);
	this->m_LargeCompoundPotionRate2[CLASS_DL] = GetPrivateProfileInt(section,"DLLargeCompoundPotionRate2",0,path);
	#if(GAMESERVER_UPDATE>=401)
	this->m_LargeCompoundPotionRate2[CLASS_SU] = GetPrivateProfileInt(section,"SULargeCompoundPotionRate2",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_LargeCompoundPotionRate2[CLASS_RF] = GetPrivateProfileInt(section,"RFLargeCompoundPotionRate2",0,path);
	#endif

	#endif

	#if(GAMESERVER_UPDATE>=401)

	this->m_EliteLifePotionRate[CLASS_DW] = GetPrivateProfileInt(section,"DWEliteLifePotionRate",0,path);
	this->m_EliteLifePotionRate[CLASS_DK] = GetPrivateProfileInt(section,"DKEliteLifePotionRate",0,path);
	this->m_EliteLifePotionRate[CLASS_FE] = GetPrivateProfileInt(section,"FEEliteLifePotionRate",0,path);
	this->m_EliteLifePotionRate[CLASS_MG] = GetPrivateProfileInt(section,"MGEliteLifePotionRate",0,path);
	this->m_EliteLifePotionRate[CLASS_DL] = GetPrivateProfileInt(section,"DLEliteLifePotionRate",0,path);
	#if(GAMESERVER_UPDATE>=401)
	this->m_EliteLifePotionRate[CLASS_SU] = GetPrivateProfileInt(section,"SUEliteLifePotionRate",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_EliteLifePotionRate[CLASS_RF] = GetPrivateProfileInt(section,"RFEliteLifePotionRate",0,path);
	#endif

	this->m_EliteManaPotionRate[CLASS_DW] = GetPrivateProfileInt(section,"DWEliteManaPotionRate",0,path);
	this->m_EliteManaPotionRate[CLASS_DK] = GetPrivateProfileInt(section,"DKEliteManaPotionRate",0,path);
	this->m_EliteManaPotionRate[CLASS_FE] = GetPrivateProfileInt(section,"FEEliteManaPotionRate",0,path);
	this->m_EliteManaPotionRate[CLASS_MG] = GetPrivateProfileInt(section,"MGEliteManaPotionRate",0,path);
	this->m_EliteManaPotionRate[CLASS_DL] = GetPrivateProfileInt(section,"DLEliteManaPotionRate",0,path);
	#if(GAMESERVER_UPDATE>=401)
	this->m_EliteManaPotionRate[CLASS_SU] = GetPrivateProfileInt(section,"SUEliteManaPotionRate",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_EliteManaPotionRate[CLASS_RF] = GetPrivateProfileInt(section,"RFEliteManaPotionRate",0,path);
	#endif

	this->m_EliteShieldPotionRate[CLASS_DW] = GetPrivateProfileInt(section,"DWEliteShieldPotionRate",0,path);
	this->m_EliteShieldPotionRate[CLASS_DK] = GetPrivateProfileInt(section,"DKEliteShieldPotionRate",0,path);
	this->m_EliteShieldPotionRate[CLASS_FE] = GetPrivateProfileInt(section,"FEEliteShieldPotionRate",0,path);
	this->m_EliteShieldPotionRate[CLASS_MG] = GetPrivateProfileInt(section,"MGEliteShieldPotionRate",0,path);
	this->m_EliteShieldPotionRate[CLASS_DL] = GetPrivateProfileInt(section,"DLEliteShieldPotionRate",0,path);
	#if(GAMESERVER_UPDATE>=401)
	this->m_EliteShieldPotionRate[CLASS_SU] = GetPrivateProfileInt(section,"SUEliteShieldPotionRate",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_EliteShieldPotionRate[CLASS_RF] = GetPrivateProfileInt(section,"RFEliteShieldPotionRate",0,path);
	#endif

	#endif

	this->m_AleIncSpeed = GetPrivateProfileInt(section,"AleIncSpeed",0,path);
	this->m_AleIncSpeedTime = GetPrivateProfileInt(section,"AleIncSpeedTime",0,path);

	this->m_OliveOfLoveIncSpeed = GetPrivateProfileInt(section,"OliveOfLoveIncSpeed",0,path);
	this->m_OliveOfLoveIncSpeedTime = GetPrivateProfileInt(section,"OliveOfLoveIncSpeedTime",0,path);

	this->m_RemedyOfLoveIncDamage = GetPrivateProfileInt(section,"RemedyOfLoveIncDamage",0,path);
	this->m_RemedyOfLoveIncDamageTime = GetPrivateProfileInt(section,"RemedyOfLoveIncDamageTime",0,path);

	this->m_PotionOfBlessConstA = GetPrivateProfileInt(section,"PotionOfBlessConstA",0,path);
	this->m_PotionOfBlessConstB = GetPrivateProfileInt(section,"PotionOfBlessConstB",0,path);
	this->m_PotionOfBlessTimeConstA = GetPrivateProfileInt(section,"PotionOfBlessTimeConstA",0,path);

	this->m_PotionOfSoulConstA = GetPrivateProfileInt(section,"PotionOfSoulConstA",0,path);
	this->m_PotionOfSoulConstB = GetPrivateProfileInt(section,"PotionOfSoulConstB",0,path);
	this->m_PotionOfSoulTimeConstA = GetPrivateProfileInt(section,"PotionOfSoulTimeConstA",0,path);
}

void CServerInfo::ReadSkillInfo(char* section,char* path) // OK
{
	this->m_ManaShieldConstA = GetPrivateProfileInt(section,"ManaShieldConstA",0,path);
	this->m_ManaShieldConstB = GetPrivateProfileInt(section,"ManaShieldConstB",0,path);
	this->m_ManaShieldConstC = GetPrivateProfileInt(section,"ManaShieldConstC",0,path);

	this->m_ManaShieldRate[CLASS_DW] = GetPrivateProfileInt(section,"ManaShieldRateDW",0,path);
	this->m_ManaShieldRate[CLASS_DK] = GetPrivateProfileInt(section,"ManaShieldRateDK",0,path);
	this->m_ManaShieldRate[CLASS_FE] = GetPrivateProfileInt(section,"ManaShieldRateFE",0,path);
	this->m_ManaShieldRate[CLASS_MG] = GetPrivateProfileInt(section,"ManaShieldRateMG",0,path);
	this->m_ManaShieldRate[CLASS_DL] = GetPrivateProfileInt(section,"ManaShieldRateDL",0,path);
	#if(GAMESERVER_UPDATE>=401)
	this->m_ManaShieldRate[CLASS_SU] = GetPrivateProfileInt(section,"ManaShieldRateSU",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_ManaShieldRate[CLASS_RF] = GetPrivateProfileInt(section,"ManaShieldRateRF",0,path);
	#endif

	this->m_ManaShieldTimeConstA = GetPrivateProfileInt(section,"ManaShieldTimeConstA",0,path);
	this->m_ManaShieldTimeConstB = GetPrivateProfileInt(section,"ManaShieldTimeConstB",0,path);

	this->m_ManaShieldMaxRate = GetPrivateProfileInt(section,"ManaShieldMaxRate",0,path);

	this->m_DefenseConstA = GetPrivateProfileInt(section,"DefenseConstA",0,path);

	this->m_DefenseTimeConstA = GetPrivateProfileInt(section,"DefenseTimeConstA",0,path);

	this->m_HealConstA = GetPrivateProfileInt(section,"HealConstA",0,path);
	this->m_HealConstB = GetPrivateProfileInt(section,"HealConstB",0,path);

	this->m_GreaterDefenseConstA = GetPrivateProfileInt(section,"GreaterDefenseConstA",0,path);
	this->m_GreaterDefenseConstB = GetPrivateProfileInt(section,"GreaterDefenseConstB",0,path);

	this->m_GreaterDefenseRate[CLASS_DW] = GetPrivateProfileInt(section,"GreaterDefenseRateDW",0,path);
	this->m_GreaterDefenseRate[CLASS_DK] = GetPrivateProfileInt(section,"GreaterDefenseRateDK",0,path);
	this->m_GreaterDefenseRate[CLASS_FE] = GetPrivateProfileInt(section,"GreaterDefenseRateFE",0,path);
	this->m_GreaterDefenseRate[CLASS_MG] = GetPrivateProfileInt(section,"GreaterDefenseRateMG",0,path);
	this->m_GreaterDefenseRate[CLASS_DL] = GetPrivateProfileInt(section,"GreaterDefenseRateDL",0,path);
	#if(GAMESERVER_UPDATE>=401)
	this->m_GreaterDefenseRate[CLASS_SU] = GetPrivateProfileInt(section,"GreaterDefenseRateSU",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_GreaterDefenseRate[CLASS_RF] = GetPrivateProfileInt(section,"GreaterDefenseRateRF",0,path);
	#endif

	this->m_GreaterDefenseTimeConstA = GetPrivateProfileInt(section,"GreaterDefenseTimeConstA",0,path);

	this->m_GreaterDamageConstA = GetPrivateProfileInt(section,"GreaterDamageConstA",0,path);
	this->m_GreaterDamageConstB = GetPrivateProfileInt(section,"GreaterDamageConstB",0,path);

	this->m_GreaterDamageRate[CLASS_DW] = GetPrivateProfileInt(section,"GreaterDamageRateDW",0,path);
	this->m_GreaterDamageRate[CLASS_DK] = GetPrivateProfileInt(section,"GreaterDamageRateDK",0,path);
	this->m_GreaterDamageRate[CLASS_FE] = GetPrivateProfileInt(section,"GreaterDamageRateFE",0,path);
	this->m_GreaterDamageRate[CLASS_MG] = GetPrivateProfileInt(section,"GreaterDamageRateMG",0,path);
	this->m_GreaterDamageRate[CLASS_DL] = GetPrivateProfileInt(section,"GreaterDamageRateDL",0,path);
	#if(GAMESERVER_UPDATE>=401)
	this->m_GreaterDamageRate[CLASS_SU] = GetPrivateProfileInt(section,"GreaterDamageRateSU",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_GreaterDamageRate[CLASS_RF] = GetPrivateProfileInt(section,"GreaterDamageRateRF",0,path);
	#endif

	this->m_GreaterDamageTimeConstA = GetPrivateProfileInt(section,"GreaterDamageTimeConstA",0,path);

	this->m_SummonMonster1 = GetPrivateProfileInt(section,"SummonMonster1",0,path);
	this->m_SummonMonster2 = GetPrivateProfileInt(section,"SummonMonster2",0,path);
	this->m_SummonMonster3 = GetPrivateProfileInt(section,"SummonMonster3",0,path);
	this->m_SummonMonster4 = GetPrivateProfileInt(section,"SummonMonster4",0,path);
	this->m_SummonMonster5 = GetPrivateProfileInt(section,"SummonMonster5",0,path);
	this->m_SummonMonster6 = GetPrivateProfileInt(section,"SummonMonster6",0,path);
	this->m_SummonMonster7 = GetPrivateProfileInt(section,"SummonMonster7",0,path);
	#if(GAMESERVER_UPDATE>=701)
	this->m_SummonMonster8 = GetPrivateProfileInt(section,"SummonMonster8",0,path);
	#endif

	this->m_GreaterLifeConstA = GetPrivateProfileInt(section,"GreaterLifeConstA",0,path);
	this->m_GreaterLifeConstB = GetPrivateProfileInt(section,"GreaterLifeConstB",0,path);
	this->m_GreaterLifeConstC = GetPrivateProfileInt(section,"GreaterLifeConstC",0,path);

	this->m_GreaterLifeRate[CLASS_DW] = GetPrivateProfileInt(section,"GreaterLifeRateDW",0,path);
	this->m_GreaterLifeRate[CLASS_DK] = GetPrivateProfileInt(section,"GreaterLifeRateDK",0,path);
	this->m_GreaterLifeRate[CLASS_FE] = GetPrivateProfileInt(section,"GreaterLifeRateFE",0,path);
	this->m_GreaterLifeRate[CLASS_MG] = GetPrivateProfileInt(section,"GreaterLifeRateMG",0,path);
	this->m_GreaterLifeRate[CLASS_DL] = GetPrivateProfileInt(section,"GreaterLifeRateDL",0,path);
	#if(GAMESERVER_UPDATE>=401)
	this->m_GreaterLifeRate[CLASS_SU] = GetPrivateProfileInt(section,"GreaterLifeRateSU",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_GreaterLifeRate[CLASS_RF] = GetPrivateProfileInt(section,"GreaterLifeRateRF",0,path);
	#endif

	this->m_GreaterLifeTimeConstA = GetPrivateProfileInt(section,"GreaterLifeTimeConstA",0,path);
	this->m_GreaterLifeTimeConstB = GetPrivateProfileInt(section,"GreaterLifeTimeConstB",0,path);

	this->m_GreaterLifeMaxRate = GetPrivateProfileInt(section,"GreaterLifeMaxRate",0,path);

	this->m_FireSlashConstA = GetPrivateProfileInt(section,"FireSlashConstA",0,path);
	this->m_FireSlashConstB = GetPrivateProfileInt(section,"FireSlashConstB",0,path);

	this->m_FireSlashTimeConstA = GetPrivateProfileInt(section,"FireSlashTimeConstA",0,path);

	this->m_FireSlashMaxRate = GetPrivateProfileInt(section,"FireSlashMaxRate",0,path);

	this->m_GreaterCriticalDamageConstA = GetPrivateProfileInt(section,"GreaterCriticalDamageConstA",0,path);
	this->m_GreaterCriticalDamageConstB = GetPrivateProfileInt(section,"GreaterCriticalDamageConstB",0,path);

	this->m_GreaterCriticalDamageTimeConstA = GetPrivateProfileInt(section,"GreaterCriticalDamageTimeConstA",0,path);
	this->m_GreaterCriticalDamageTimeConstB = GetPrivateProfileInt(section,"GreaterCriticalDamageTimeConstB",0,path);

	this->m_SternTimeConstA = GetPrivateProfileInt(section,"SternTimeConstA",0,path);

	this->m_GreaterManaConstA = GetPrivateProfileInt(section,"GreaterManaConstA",0,path);

	this->m_GreaterManaTimeConstA = GetPrivateProfileInt(section,"GreaterManaTimeConstA",0,path);

	this->m_InvisibilityTimeConstA = GetPrivateProfileInt(section,"InvisibilityTimeConstA",0,path);

	#if(GAMESERVER_UPDATE>=201)

	this->m_BrandTimeConstA = GetPrivateProfileInt(section,"BrandTimeConstA",0,path);

	this->m_InfinityArrowTimeConstA = GetPrivateProfileInt(section,"InfinityArrowTimeConstA",0,path);

	#endif

	this->m_InfinityArrowSwitch[0] = GetPrivateProfileInt(section,"InfinityArrowSwitch_AL0",0,path);
	this->m_InfinityArrowSwitch[1] = GetPrivateProfileInt(section,"InfinityArrowSwitch_AL1",0,path);
	this->m_InfinityArrowSwitch[2] = GetPrivateProfileInt(section,"InfinityArrowSwitch_AL2",0,path);
	this->m_InfinityArrowSwitch[3] = GetPrivateProfileInt(section,"InfinityArrowSwitch_AL3",0,path);

	this->m_MagicDamageImmunityTimeConstA = GetPrivateProfileInt(section,"MagicDamageImmunityTimeConstA",0,path);

	this->m_PhysiDamageImmunityTimeConstA = GetPrivateProfileInt(section,"PhysiDamageImmunityTimeConstA",0,path);

	#if(GAMESERVER_UPDATE>=401)

	this->m_DrainLifeConstA = GetPrivateProfileInt(section,"DrainLifeConstA",0,path);
	this->m_DrainLifeConstB = GetPrivateProfileInt(section,"DrainLifeConstB",0,path);

	this->m_ReflectDamageConstA = GetPrivateProfileInt(section,"ReflectDamageConstA",0,path);
	this->m_ReflectDamageConstB = GetPrivateProfileInt(section,"ReflectDamageConstB",0,path);

	this->m_ReflectDamageRate[CLASS_DW] = GetPrivateProfileInt(section,"ReflectDamageRateDW",0,path);
	this->m_ReflectDamageRate[CLASS_DK] = GetPrivateProfileInt(section,"ReflectDamageRateDK",0,path);
	this->m_ReflectDamageRate[CLASS_FE] = GetPrivateProfileInt(section,"ReflectDamageRateFE",0,path);
	this->m_ReflectDamageRate[CLASS_MG] = GetPrivateProfileInt(section,"ReflectDamageRateMG",0,path);
	this->m_ReflectDamageRate[CLASS_DL] = GetPrivateProfileInt(section,"ReflectDamageRateDL",0,path);

	#if(GAMESERVER_UPDATE>=401)
	this->m_ReflectDamageRate[CLASS_SU] = GetPrivateProfileInt(section,"ReflectDamageRateSU",0,path);
	#endif
	#if(GAMESERVER_UPDATE>=601)
	this->m_ReflectDamageRate[CLASS_RF] = GetPrivateProfileInt(section,"ReflectDamageRateRF",0,path);
	#endif

	this->m_ReflectDamageTimeConstA = GetPrivateProfileInt(section,"ReflectDamageTimeConstA",0,path);
	this->m_ReflectDamageTimeConstB = GetPrivateProfileInt(section,"ReflectDamageTimeConstB",0,path);

	this->m_ReflectDamageMaxRate = GetPrivateProfileInt(section,"ReflectDamageMaxRate",0,path);

	this->m_SwordPowerConstA = GetPrivateProfileInt(section,"SwordPowerConstA",0,path);
	this->m_SwordPowerConstB = GetPrivateProfileInt(section,"SwordPowerConstB",0,path);

	this->m_SwordPowerTimeConstA = GetPrivateProfileInt(section,"SwordPowerTimeConstA",0,path);
	this->m_SwordPowerTimeConstB = GetPrivateProfileInt(section,"SwordPowerTimeConstB",0,path);

	this->m_SwordPowerMaxRate = GetPrivateProfileInt(section,"SwordPowerMaxRate",0,path);

	this->m_SleepConstA = GetPrivateProfileInt(section,"SleepConstA",0,path);
	this->m_SleepConstB = GetPrivateProfileInt(section,"SleepConstB",0,path);

	this->m_SleepTimeConstA = GetPrivateProfileInt(section,"SleepTimeConstA",0,path);
	this->m_SleepTimeConstB = GetPrivateProfileInt(section,"SleepTimeConstB",0,path);

	this->m_SleepMaxTime = GetPrivateProfileInt(section,"SleepMaxTime",0,path);

	this->m_LesserDefenseConstA = GetPrivateProfileInt(section,"LesserDefenseConstA",0,path);
	this->m_LesserDefenseConstB = GetPrivateProfileInt(section,"LesserDefenseConstB",0,path);
	this->m_LesserDefenseConstC = GetPrivateProfileInt(section,"LesserDefenseConstC",0,path);
	this->m_LesserDefenseConstD = GetPrivateProfileInt(section,"LesserDefenseConstD",0,path);

	this->m_LesserDefenseTimeConstA = GetPrivateProfileInt(section,"LesserDefenseTimeConstA",0,path);
	this->m_LesserDefenseTimeConstB = GetPrivateProfileInt(section,"LesserDefenseTimeConstB",0,path);

	this->m_LesserDefenseMaxRate = GetPrivateProfileInt(section,"LesserDefenseMaxRate",0,path);

	this->m_LesserDamageConstA = GetPrivateProfileInt(section,"LesserDamageConstA",0,path);
	this->m_LesserDamageConstB = GetPrivateProfileInt(section,"LesserDamageConstB",0,path);
	this->m_LesserDamageConstC = GetPrivateProfileInt(section,"LesserDamageConstC",0,path);
	this->m_LesserDamageConstD = GetPrivateProfileInt(section,"LesserDamageConstD",0,path);

	this->m_LesserDamageTimeConstA = GetPrivateProfileInt(section,"LesserDamageTimeConstA",0,path);
	this->m_LesserDamageTimeConstB = GetPrivateProfileInt(section,"LesserDamageTimeConstB",0,path);

	this->m_LesserDamageMaxRate = GetPrivateProfileInt(section,"LesserDamageMaxRate",0,path);

	this->m_SahamuttConstA = GetPrivateProfileInt(section,"SahamuttConstA",0,path);

	this->m_NeilConstA = GetPrivateProfileInt(section,"NeilConstA",0,path);

	this->m_MagicCircleConstA = GetPrivateProfileInt(section,"MagicCircleConstA",0,path);

	this->m_MagicCircleTimeConstA = GetPrivateProfileInt(section,"MagicCircleTimeConstA",0,path);

	this->m_ShieldRecoverConstA = GetPrivateProfileInt(section,"ShieldRecoverConstA",0,path);
	this->m_ShieldRecoverConstB = GetPrivateProfileInt(section,"ShieldRecoverConstB",0,path);

	this->m_DragonSlayerConstA = GetPrivateProfileInt(section,"DragonSlayerConstA",0,path);
	this->m_DragonSlayerConstB = GetPrivateProfileInt(section,"DragonSlayerConstB",0,path);
	this->m_DragonSlayerConstC = GetPrivateProfileInt(section,"DragonSlayerConstC",0,path);
	this->m_DragonSlayerConstD = GetPrivateProfileInt(section,"DragonSlayerConstD",0,path);

	this->m_DragonSlayerMaxRate = GetPrivateProfileInt(section,"DragonSlayerMaxRate",0,path);

	this->m_GreaterIgnoreDefenseRateConstA = GetPrivateProfileInt(section,"GreaterIgnoreDefenseRateConstA",0,path);
	this->m_GreaterIgnoreDefenseRateConstB = GetPrivateProfileInt(section,"GreaterIgnoreDefenseRateConstB",0,path);

	this->m_GreaterIgnoreDefenseRateTimeConstA = GetPrivateProfileInt(section,"GreaterIgnoreDefenseRateTimeConstA",0,path);
	this->m_GreaterIgnoreDefenseRateTimeConstB = GetPrivateProfileInt(section,"GreaterIgnoreDefenseRateTimeConstB",0,path);

	this->m_FitnessConstA = GetPrivateProfileInt(section,"FitnessConstA",0,path);
	this->m_FitnessConstB = GetPrivateProfileInt(section,"FitnessConstB",0,path);

	this->m_FitnessTimeConstA = GetPrivateProfileInt(section,"FitnessTimeConstA",0,path);
	this->m_FitnessTimeConstB = GetPrivateProfileInt(section,"FitnessTimeConstB",0,path);

	this->m_GreaterDefenseSuccessRateConstA = GetPrivateProfileInt(section,"GreaterDefenseSuccessRateConstA",0,path);
	this->m_GreaterDefenseSuccessRateConstB = GetPrivateProfileInt(section,"GreaterDefenseSuccessRateConstB",0,path);

	this->m_GreaterDefenseSuccessRateTimeConstA = GetPrivateProfileInt(section,"GreaterDefenseSuccessRateTimeConstA",0,path);
	this->m_GreaterDefenseSuccessRateTimeConstB = GetPrivateProfileInt(section,"GreaterDefenseSuccessRateTimeConstB",0,path);

	#endif

	#if(GAMESERVER_UPDATE>=701)

	this->m_PartyHealConstA = GetPrivateProfileInt(section,"PartyHealConstA",0,path);
	this->m_PartyHealConstB = GetPrivateProfileInt(section,"PartyHealConstB",0,path);

	this->m_BlessConstA = GetPrivateProfileInt(section,"BlessConstA",0,path);

	this->m_BlessTimeConstA = GetPrivateProfileInt(section,"BlessTimeConstA",0,path);

	this->m_BlindConstA = GetPrivateProfileInt(section,"BlindConstA",0,path);
	this->m_BlindConstB = GetPrivateProfileInt(section,"BlindConstB",0,path);
	this->m_BlindConstC = GetPrivateProfileInt(section,"BlindConstC",0,path);

	this->m_BlindTimeConstA = GetPrivateProfileInt(section,"BlindTimeConstA",0,path);

	this->m_EarthPrisonConstA = GetPrivateProfileInt(section,"EarthPrisonConstA",0,path);

	this->m_IronDefenseConstA = GetPrivateProfileInt(section,"IronDefenseConstA",0,path);
	this->m_IronDefenseConstB = GetPrivateProfileInt(section,"IronDefenseConstB",0,path);

	this->m_IronDefenseTimeConstA = GetPrivateProfileInt(section,"IronDefenseTimeConstA",0,path);

	this->m_BloodHowlingConstA = GetPrivateProfileInt(section,"BloodHowlingConstA",0,path);
	this->m_BloodHowlingConstB = GetPrivateProfileInt(section,"BloodHowlingConstB",0,path);

	this->m_BloodHowlingTimeConstA = GetPrivateProfileInt(section,"BloodHowlingTimeConstA",0,path);

	#endif
}