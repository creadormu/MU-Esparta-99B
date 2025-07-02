#include "stdafx.h"
#include "DSProtocol.h"
#include "AcheronGuardian.h"
#include "ArcaBattle.h"
#include "BloodCastle.h"
#include "CashShop.h"
#include "CastleSiege.h"
#include "CastleSiegeSync.h"
#include "ChaosBox.h"
#include "ChaosCastle.h"
#include "ChaosCastleFinal.h"
#include "CommandManager.h"
#include "Crywolf.h"
#include "CrywolfSync.h"
#include "CustomGift.h"
#include "CustomMonsterKillCount.h"
#include "CustomPick.h"
#include "CustomStore.h"
#include "CustomWing.h"
#include "DefaultClassFreebies.h"
#include "EventInventory.h"
#include "Filter.h"
#include "Friend.h"
#include "Fruit.h"
#include "GameMain.h"
#include "GameMaster.h"
#include "GensSystem.h"
#include "Guild.h"
#include "GuildManager.h"
#include "GuildMatching.h"
#include "GoldenArcher.h"
#include "Helper.h"
#include "IllusionTemple.h"
#include "InvasionManager.h"
#include "ItemBagManager.h"
#include "JSProtocol.h"
#include "Log.h"
#include "LuckyCoin.h"
#include "LuckyItem.h"
#include "Map.h"
#include "MapManager.h"
#include "MapServerManager.h"
#include "MasterSkillTree.h"
#include "Message.h"
#include "MuCashShop.h"
#include "MuRummy.h"
#include "MuunSystem.h"
#include "Notice.h"
#include "NpcTalk.h"
#include "ObjectManager.h"
#include "Party.h"
#include "PartyMatching.h"
#include "PcPoint.h"
#include "PersonalShop.h"
#include "Quest.h"
#include "QuestWorld.h"
#include "Reconnect.h"
#include "ScriptLoader.h"
#include "ServerInfo.h"
#include "Util.h"
#include "Viewport.h"
#include "Warehouse.h"
#include "FakeOnline.h"


void DataServerProtocolCore(BYTE head,BYTE* lpMsg,int size) // OK
{
	PROTECT_START

	switch(head)
	{
		case 0x01:
			DGCharacterListRecv((SDHP_CHARACTER_LIST_RECV*)lpMsg);
			break;
		case 0x02:
			DGCharacterCreateRecv((SDHP_CHARACTER_CREATE_RECV*)lpMsg);
			break;
		case 0x03:
			DGCharacterDeleteRecv((SDHP_CHARACTER_DELETE_RECV*)lpMsg);
			break;
		case 0x04:
			DGCharacterInfoRecv((SDHP_CHARACTER_INFO_RECV*)lpMsg);
			break;
		case 0x05:
			switch(((lpMsg[0]==0xC1)?lpMsg[3]:lpMsg[4]))
			{
				case 0x00:
					gWarehouse.DGWarehouseItemRecv((SDHP_WAREHOUSE_ITEM_RECV*)lpMsg);
					break;
				case 0x01:
					gWarehouse.DGWarehouseFreeRecv((SDHP_WAREHOUSE_FREE_RECV*)lpMsg);
					break;
			}
			break;
		case 0x07:
			DGCreateItemRecv((SDHP_CREATE_ITEM_RECV*)lpMsg);
			break;
		case 0x08:
			DGOptionDataRecv((SDHP_OPTION_DATA_RECV*)lpMsg);
			break;
		case 0x09:
			DGPetItemInfoRecv((SDHP_PET_ITEM_INFO_RECV*)lpMsg);
			break;
		case 0x0A:
			#if(GAMESERVER_UPDATE>=401)
			DGCharacterNameCheckRecv((SDHP_CHARACTER_NAME_CHECK_RECV*)lpMsg);
			#endif
			break;
		case 0x0B:
			#if(GAMESERVER_UPDATE>=401)
			DGCharacterNameChangeRecv((SDHP_CHARACTER_NAME_CHANGE_RECV*)lpMsg);
			#endif
			break;
		case 0x0C:
			switch(((lpMsg[0]==0xC1)?lpMsg[3]:lpMsg[4]))
			{
				case 0x00:
					#if(GAMESERVER_UPDATE>=301)
					gQuest.DGQuestKillCountRecv((SDHP_QUEST_KILL_COUNT_RECV*)lpMsg);
					#endif
					break;
			}
			break;
		case 0x0D:
			switch(((lpMsg[0]==0xC1)?lpMsg[3]:lpMsg[4]))
			{
				case 0x00:
					#if(GAMESERVER_UPDATE>=401)
					gMasterSkillTree.DGMasterSkillTreeRecv((SDHP_MASTER_SKILL_TREE_RECV*)lpMsg);
					#endif
					break;
			}
			break;
		case 0x0E:
			switch(((lpMsg[0]==0xC1)?lpMsg[3]:lpMsg[4]))
			{
				case 0x00:
					#if(GAMESERVER_UPDATE>=202)
					gNpcTalk.DGNpcLeoTheHelperRecv((SDHP_NPC_LEO_THE_HELPER_RECV*)lpMsg);
					#endif
					break;
				case 0x01:
					#if(GAMESERVER_UPDATE>=401)
					gNpcTalk.DGNpcSantaClausRecv((SDHP_NPC_SANTA_CLAUS_RECV*)lpMsg);
					#endif
					break;
			}
			break;
		case 0x0F:
			switch(((lpMsg[0]==0xC1)?lpMsg[3]:lpMsg[4]))
			{
				case 0x00:
					gCommandManager.DGCommandResetRecv((SDHP_COMMAND_RESET_RECV*)lpMsg);
					break;
				case 0x01:
					gCommandManager.DGCommandMasterResetRecv((SDHP_COMMAND_MASTER_RESET_RECV*)lpMsg);
					break;
				case 0x03:
					gCustomGift.DGCommandGiftRecv((SDHP_COMMAND_GIFT_RECV*)lpMsg);
					break;
				case 0x04:
					gCommandManager.DGCommandRenameRecv((SDHP_COMMAND_RENAME_RECV*)lpMsg);
					break;
				case 0x05:
					gCommandManager.DGCommandBanAccountRecv((SDHP_COMMAND_BAN_ACCOUNT_RECV*)lpMsg);
					break;
				case 0x06:
					gCommandManager.DGCommandBanCharacterRecv((SDHP_COMMAND_BAN_CHARACTER_RECV*)lpMsg);
					break;
			}
			break;
		case 0x10:
			switch(((lpMsg[0]==0xC1)?lpMsg[3]:lpMsg[4]))
			{
				case 0x00:
					#if(GAMESERVER_UPDATE>=501)
					gQuestWorld.DGQuestWorldRecv((SDHP_QUEST_WORLD_RECV*)lpMsg);
					#endif
					break;
			}
			break;
		case 0x11:
			switch(((lpMsg[0]==0xC1)?lpMsg[3]:lpMsg[4]))
			{
				case 0x00:
					#if(GAMESERVER_UPDATE>=501)
					gGensSystem.DGGensSystemInsertRecv((SDHP_GENS_SYSTEM_INSERT_RECV*)lpMsg);
					#endif
					break;
				case 0x01:
					#if(GAMESERVER_UPDATE>=501)
					gGensSystem.DGGensSystemDeleteRecv((SDHP_GENS_SYSTEM_DELETE_RECV*)lpMsg);
					#endif
					break;
				case 0x02:
					#if(GAMESERVER_UPDATE>=501)
					gGensSystem.DGGensSystemMemberRecv((SDHP_GENS_SYSTEM_MEMBER_RECV*)lpMsg);
					#endif
					break;
				case 0x03:
					#if(GAMESERVER_UPDATE>=501)
					gGensSystem.DGGensSystemUpdateRecv((SDHP_GENS_SYSTEM_UPDATE_RECV*)lpMsg);
					#endif
					break;
				case 0x04:
					#if(GAMESERVER_UPDATE>=501)
					gGensSystem.DGGensSystemRewardRecv((SDHP_GENS_SYSTEM_REWARD_RECV*)lpMsg);
					#endif
					break;
				case 0x70:
					#if(GAMESERVER_UPDATE>=501)
					gGensSystem.DGGensSystemCreateRecv((SDHP_GENS_SYSTEM_CREATE_RECV*)lpMsg);
					#endif
					break;
			}
			break;
		case 0x12:
			switch(((lpMsg[0]==0xC1)?lpMsg[3]:lpMsg[4]))
			{
				case 0x00:
					#if(GAMESERVER_UPDATE>=802)
					gMuRummy.GDAnsCardInfo((_tagPMSG_ANS_MURUMMY_SELECT_DS*)lpMsg);
					#endif
					break;
			}
			break;
		case 0x13:
			switch(((lpMsg[0]==0xC1)?lpMsg[3]:lpMsg[4]))
			{
				case 0x00:
					gCustomPick.DGCustomPickRecv((SDHP_CUSTOM_PICK_RECV*)lpMsg);
					break;
			}
			break;
		case 0x14:
			#if(GAMESERVER_UPDATE>=801)
			gAcheronGuardian.DGAcheronGuardianSyncRecv((SDHP_ACHERON_GUARDIAN_SYNC_RECV*)lpMsg);
			#endif
			break;
		case 0x17:
			switch(((lpMsg[0]==0xC1)?lpMsg[3]:lpMsg[4]))
			{
				case 0x00:
					#if(GAMESERVER_UPDATE>=603)
					gHelper.DGHelperDataRecv((SDHP_HELPER_DATA_RECV*)lpMsg);
					#endif
					break;
			}
			break;
		case 0x18:
			switch(((lpMsg[0]==0xC1)?lpMsg[3]:lpMsg[4]))
			{
				case 0x00:
					#if(GAMESERVER_SHOP==3)
					gCashShop.DGCashShopPointRecv((SDHP_CASH_SHOP_POINT_RECV*)lpMsg);
					#endif
					break;
				case 0x01:
					#if(GAMESERVER_SHOP==3)
					gCashShop.DGCashShopItemBuyRecv((SDHP_CASH_SHOP_ITEM_BUY_RECV*)lpMsg);
					#endif
					break;
				case 0x02:
					#if(GAMESERVER_SHOP==3)
					gCashShop.DGCashShopItemGifRecv((SDHP_CASH_SHOP_ITEM_GIF_RECV*)lpMsg);
					#endif
					break;
				case 0x03:
					#if(GAMESERVER_SHOP==3)
					gCashShop.DGCashShopItemNumRecv((SDHP_CASH_SHOP_ITEM_NUM_RECV*)lpMsg);
					#endif
					break;
				case 0x04:
					#if(GAMESERVER_SHOP==3)
					gCashShop.DGCashShopItemUseRecv((SDHP_CASH_SHOP_ITEM_USE_RECV*)lpMsg);
					#endif
					break;
				case 0x05:
					#if(GAMESERVER_SHOP==3)
					gCashShop.DGCashShopPeriodicItemRecv((SDHP_CASH_SHOP_PERIODIC_ITEM_RECV*)lpMsg);
					#endif
					break;
				case 0x06:
					#if(GAMESERVER_SHOP==3)
					gCashShop.DGCashShopRecievePointRecv((SDHP_CASH_SHOP_RECIEVE_POINT_RECV*)lpMsg);
					#endif
					break;
			}
			break;
		case 0x19:
			switch(((lpMsg[0]==0xC1)?lpMsg[3]:lpMsg[4]))
			{
				case 0x00:
					#if(GAMESERVER_SHOP==1)
					gPcPoint.DGPcPointPointRecv((SDHP_PC_POINT_POINT_RECV*)lpMsg);
					#endif
					break;
				case 0x01:
					#if(GAMESERVER_SHOP==1)
					gPcPoint.DGPcPointItemBuyRecv((SDHP_PC_POINT_ITEM_BUY_RECV*)lpMsg);
					#endif
					break;
				case 0x02:
					#if(GAMESERVER_SHOP==1)
					gPcPoint.DGPcPointRecievePointRecv((SDHP_PC_POINT_RECIEVE_POINT_RECV*)lpMsg);
					#endif
					break;
			}
			break;
		case 0x1A:
			switch(((lpMsg[0]==0xC1)?lpMsg[3]:lpMsg[4]))
			{
				case 0x00:
					#if(GAMESERVER_UPDATE>=402)
					gLuckyCoin.DGLuckyCoinCountRecv((SDHP_LUCKY_COIN_COUNT_RECV*)lpMsg);
					#endif
					break;
				case 0x01:
					#if(GAMESERVER_UPDATE>=402)
					gLuckyCoin.DGLuckyCoinRegisterRecv((SDHP_LUCKY_COIN_REGISTER_RECV*)lpMsg);
					#endif
					break;
				case 0x02:
					#if(GAMESERVER_UPDATE>=402)
					gLuckyCoin.DGLuckyCoinExchangeRecv((SDHP_LUCKY_COIN_EXCHANGE_RECV*)lpMsg);
					#endif
					break;
			}
			break;
		case 0x1B:
			switch(((lpMsg[0]==0xC1)?lpMsg[3]:lpMsg[4]))
			{
				case 0x00:
					#if(GAMESERVER_UPDATE>=803)
					gChaosCastleFinal.GCChaosCastleFinalRankingSend((SDHP_CHAOS_CASTLE_RANKING_RECV*)lpMsg);
					#endif
					break;
			}
			break;
		case 0x1C:
			switch(((lpMsg[0]==0xC1)?lpMsg[3]:lpMsg[4]))
			{
				case 0x00:
					#if(GAMESERVER_SHOP==2)
					gMuCashShop.DGMuCashShopPointRecv((SDHP_MU_CASH_SHOP_POINT_RECV*)lpMsg);
					#endif
					break;
				case 0x01:
					#if(GAMESERVER_SHOP==2)
					gMuCashShop.DGMuCashShopItemBuyRecv((SDHP_MU_CASH_SHOP_ITEM_BUY_RECV*)lpMsg);
					#endif
					break;
				case 0x02:
					#if(GAMESERVER_SHOP==2)
					gMuCashShop.DGMuCashShopRecievePointRecv((SDHP_MU_CASH_SHOP_RECIEVE_POINT_RECV*)lpMsg);
					#endif
					break;
			}
			break;
		case 0x1D:
			switch(((lpMsg[0]==0xC1)?lpMsg[3]:lpMsg[4]))
			{
				case 0x00:
					#if(GAMESERVER_UPDATE>=702)
					gArcaBattle.DGArcaBattleMasterRegisterRecv((SDHP_ARCA_BATTLE_MASTER_REGISTER_RESULT_RECV*)lpMsg);
					#endif
					break;
				case 0x01:
					#if(GAMESERVER_UPDATE>=702)
					gArcaBattle.DGArcaBattleMemberRegisterRecv((SDHP_ARCA_BATTLE_MEMBER_REGISTER_RESULT_RECV*)lpMsg);
					#endif
					break;
				case 0x02:
					#if(GAMESERVER_UPDATE>=702)
					gArcaBattle.DGArcaBattleEnterRecv((SDHP_ARCA_BATTLE_ENTER_RECV*)lpMsg);
					#endif
					break;
				case 0x03:
					#if(GAMESERVER_UPDATE>=702)
					gArcaBattle.DGArcaBattleMemberCountRecv((SDHP_ARCA_BATTLE_MEMBER_COUNT_RECV*)lpMsg);
					#endif
					break;
				case 0x04:
					#if(GAMESERVER_UPDATE>=702)
					gArcaBattle.DGArcaBattleRegisterMarkRecv((SDHP_ARCA_BATTLE_REGISTER_MARK_RECV*)lpMsg);
					#endif
					break;
				case 0x05:
					#if(GAMESERVER_UPDATE>=702)
					gArcaBattle.DGArcaBattleMarkRankRecv((SDHP_ARCA_BATTLE_MARK_RANK_RECV*)lpMsg);
					#endif
					break;
			}
			break;
		case 0x1E:
			#if(GAMESERVER_UPDATE>=201)
			DGCrywolfSyncRecv((SDHP_CRYWOLF_SYNC_RECV*)lpMsg);
			#endif
			break;
		case 0x1F:
			#if(GAMESERVER_UPDATE>=201)
			DGCrywolfInfoRecv((SDHP_CRYWOLF_INFO_RECV*)lpMsg);
			#endif
			break;
		case 0x20:
			DGGlobalPostRecv((SDHP_GLOBAL_POST_RECV*)lpMsg);
			break;
		case 0x21:
			DGGlobalNoticeRecv((SDHP_GLOBAL_NOTICE_RECV*)lpMsg);
			break;
		case 0x22:
			switch(((lpMsg[0]==0xC1)?lpMsg[3]:lpMsg[4]))
			{
				case 0x00:
					#if(GAMESERVER_UPDATE>=602)
					gLuckyItem.DGLuckyItemRecv((SDHP_LUCKY_ITEM_RECV*)lpMsg);
					#endif
					break;
			}
			break;
		case 0x23:
			switch(((lpMsg[0]==0xC1)?lpMsg[3]:lpMsg[4]))
			{
				case 0x00:
					#if(GAMESERVER_UPDATE>=701)
					gPentagramSystem.DGPentagramJewelInfoRecv((SDHP_PENTAGRAM_JEWEL_INFO_RECV*)lpMsg);
					#endif
					break;
			}
			break;
		case 0x24:
			#if(GAMESERVER_UPDATE>=801)
			DGSNSDataRecv((SDHP_SNS_DATA_RECV*)lpMsg);
			#endif
			break;
		case 0x25:
			switch(((lpMsg[0]==0xC1)?lpMsg[3]:lpMsg[4]))
			{
				case 0x00:
					#if(GAMESERVER_UPDATE>=802)
					gPersonalShop.DGPShopItemValueRecv((SDHP_PSHOP_ITEM_VALUE_RECV*)lpMsg);
					#endif
					break;
			}
			break;
		case 0x26:
			switch(((lpMsg[0]==0xC1)?lpMsg[3]:lpMsg[4]))
			{
				case 0x00:
					#if(GAMESERVER_UPDATE>=802)
					gEventInventory.DGEventInventoryRecv((SDHP_EVENT_INVENTORY_RECV*)lpMsg);
					#endif
					break;
			}
			break;
		case 0x27:
			switch(((lpMsg[0]==0xC1)?lpMsg[3]:lpMsg[4]))
			{
				case 0x00:
					#if(GAMESERVER_UPDATE>=803)
					gMuunSystem.DGMuunInventoryRecv((SDHP_MUUN_INVENTORY_RECV*)lpMsg);
					#endif
					break;
			}
			break;
		case 0x28:
			switch(((lpMsg[0]==0xC1)?lpMsg[3]:lpMsg[4]))
			{
				case 0x00:
					#if(GAMESERVER_UPDATE>=801)
					gGuildMatching.DGGuildMatchingListRecv((SDHP_GUILD_MATCHING_LIST_RECV*)lpMsg);
					#endif
					break;
				case 0x02:
					#if(GAMESERVER_UPDATE>=801)
					gGuildMatching.DGGuildMatchingInsertRecv((SDHP_GUILD_MATCHING_INSERT_RECV*)lpMsg);
					#endif
					break;
				case 0x03:
					#if(GAMESERVER_UPDATE>=801)
					gGuildMatching.DGGuildMatchingCancelRecv((SDHP_GUILD_MATCHING_CANCEL_RECV*)lpMsg);
					#endif
					break;
				case 0x04:
					#if(GAMESERVER_UPDATE>=801)
					gGuildMatching.DGGuildMatchingJoinInsertRecv((SDHP_GUILD_MATCHING_JOIN_INSERT_RECV*)lpMsg);
					#endif
					break;
				case 0x05:
					#if(GAMESERVER_UPDATE>=801)
					gGuildMatching.DGGuildMatchingJoinCancelRecv((SDHP_GUILD_MATCHING_JOIN_CANCEL_RECV*)lpMsg);
					#endif
					break;
				case 0x06:
					#if(GAMESERVER_UPDATE>=801)
					gGuildMatching.DGGuildMatchingJoinAcceptRecv((SDHP_GUILD_MATCHING_JOIN_ACCEPT_RECV*)lpMsg);
					#endif
					break;
				case 0x07:
					#if(GAMESERVER_UPDATE>=801)
					gGuildMatching.DGGuildMatchingJoinListRecv((SDHP_GUILD_MATCHING_JOIN_LIST_RECV*)lpMsg);
					#endif
					break;
				case 0x08:
					#if(GAMESERVER_UPDATE>=801)
					gGuildMatching.DGGuildMatchingJoinInfoRecv((SDHP_GUILD_MATCHING_JOIN_INFO_RECV*)lpMsg);
					#endif
					break;
				case 0x09:
					#if(GAMESERVER_UPDATE>=801)
					gGuildMatching.DGGuildMatchingNotifyRecv((SDHP_GUILD_MATCHING_NOTIFY_RECV*)lpMsg);
					#endif
					break;
				case 0x0A:
					#if(GAMESERVER_UPDATE>=801)
					gGuildMatching.DGGuildMatchingNotifyMasterRecv((SDHP_GUILD_MATCHING_NOTIFY_MASTER_RECV*)lpMsg);
					#endif
					break;
			}
			break;
		case 0x29:
			switch(((lpMsg[0]==0xC1)?lpMsg[3]:lpMsg[4]))
			{
				case 0x00:
					#if(GAMESERVER_UPDATE>=801)
					gPartyMatching.DGPartyMatchingInsertRecv((SDHP_PARTY_MATCHING_INSERT_RECV*)lpMsg);
					#endif
					break;
				case 0x01:
					#if(GAMESERVER_UPDATE>=801)
					gPartyMatching.DGPartyMatchingListRecv((SDHP_PARTY_MATCHING_LIST_RECV*)lpMsg);
					#endif
					break;
				case 0x02:
					#if(GAMESERVER_UPDATE>=801)
					gPartyMatching.DGPartyMatchingJoinInsertRecv((SDHP_PARTY_MATCHING_JOIN_INSERT_RECV*)lpMsg);
					#endif
					break;
				case 0x03:
					#if(GAMESERVER_UPDATE>=801)
					gPartyMatching.DGPartyMatchingJoinInfoRecv((SDHP_PARTY_MATCHING_JOIN_INFO_RECV*)lpMsg);
					#endif
					break;
				case 0x04:
					#if(GAMESERVER_UPDATE>=801)
					gPartyMatching.DGPartyMatchingJoinListRecv((SDHP_PARTY_MATCHING_JOIN_LIST_RECV*)lpMsg);
					#endif
					break;
				case 0x05:
					#if(GAMESERVER_UPDATE>=801)
					gPartyMatching.DGPartyMatchingJoinAcceptRecv((SDHP_PARTY_MATCHING_JOIN_ACCEPT_RECV*)lpMsg);
					#endif
					break;
				case 0x06:
					#if(GAMESERVER_UPDATE>=801)
					gPartyMatching.DGPartyMatchingJoinCancelRecv((SDHP_PARTY_MATCHING_JOIN_CANCEL_RECV*)lpMsg);
					#endif
					break;
				case 0x07:
					#if(GAMESERVER_UPDATE>=801)
					gPartyMatching.DGPartyMatchingNotifyRecv((SDHP_PARTY_MATCHING_NOTIFY_RECV*)lpMsg);
					#endif
					break;
				case 0x08:
					#if(GAMESERVER_UPDATE>=801)
					gPartyMatching.DGPartyMatchingNotifyLeaderRecv((SDHP_PARTY_MATCHING_NOTIFY_LEADER_RECV*)lpMsg);
					#endif
					break;
			}
			break;
		case 0x50:
			gCustomMonsterKillCount.DGMonsterKillCountRecv((SDHP_MONSTER_KILL_COUNT_RECV*)lpMsg);
			break;
		case 0x72:
			DGGlobalWhisperRecv((SDHP_GLOBAL_WHISPER_RECV*)lpMsg);
			break;
		case 0x73:
			DGGlobalWhisperEchoRecv((SDHP_GLOBAL_WHISPER_ECHO_RECV*)lpMsg);
			break;
		case 0x80:
			switch(((lpMsg[0]==0xC1)?lpMsg[3]:lpMsg[4]))
			{
				case 0x01:
					GS_DGAnsOwnerGuildMaster(lpMsg);
					break;
				case 0x03:
					GS_DGAnsCastleNpcBuy(lpMsg);
					break;
				case 0x04:
					GS_DGAnsCastleNpcRepair(lpMsg);
					break;
				case 0x05:
					GS_DGAnsCastleNpcUpgrade(lpMsg);
					break;
				case 0x06:
					GS_DGAnsTaxInfo(lpMsg);
					break;
				case 0x07:
					GS_DGAnsTaxRateChange(lpMsg);
					break;
				case 0x08:
					GS_DGAnsCastleMoneyChange(lpMsg);
					break;
				case 0x0A:
					GS_DGAnsGuildMarkRegInfo(lpMsg);
					break;
				case 0x0D:
					GS_DGAnsRegAttackGuild(lpMsg);
					break;
				case 0x0E:
					GS_DGAnsRestartCastleState(lpMsg);
					break;
				case 0x0F:
					GS_DGAnsMapSvrMsgMultiCast(lpMsg);
					break;
				case 0x10:
					GS_DGAnsRegGuildMark(lpMsg);
					break;
				case 0x12:
					GS_DGAnsGuildSetGiveUp(lpMsg);
					break;
				case 0x17:
					GS_DGAnsCastleStateSync(lpMsg);
					break;
				case 0x18:
					GS_DGAnsCastleTributeMoney(lpMsg);
					break;
				case 0x19:
					GS_DGAnsResetCastleTaxInfo(lpMsg);
					break;
			}
			break;
		case 0x81:
			GS_DGAnsCastleInitData(lpMsg);
			break;
		case 0x83:
			GS_DGAnsAllGuildMarkRegInfo(lpMsg);
			break;
		case 0x84:
			GS_DGAnsFirstCreateNPC(lpMsg);
			break;
		case 0x85:
			GS_DGAnsCalcREgGuildList(lpMsg);
			break;
		case 0x86:
			GS_DGAnsCsGulidUnionInfo(lpMsg);
			break;
		case 0x87:
			GS_DGAnsCsSaveTotalGuildInfo(lpMsg);
			break;
		case 0x88:
			GS_DGAnsCsLoadTotalGuildInfo(lpMsg);
			break;
		case 0x94:
			switch(((lpMsg[0]==0xC1)?lpMsg[3]:lpMsg[4]))
			{
				case 0x00:
					#if(GAMESERVER_UPDATE<=603)
					gGoldenArcher.DGGoldenArcherCountRecv((SDHP_GOLDEN_ARCHER_COUNT_RECV*)lpMsg);
					#endif
					break;
			}
			break;
		case 0xA0:
			switch(((lpMsg[0]==0xC1)?lpMsg[3]:lpMsg[4]))
			{
				case 0x00:
					gGuild.DGGuildCreateRecv((SDHP_GUILD_CREATE_RECV*)lpMsg);
					break;
				case 0x01:
					gGuild.DGGuildDeleteRecv((SDHP_GUILD_DELETE_RECV*)lpMsg);
					break;
				case 0x02:
					gGuild.DGGuildMemberAddRecv((SDHP_GUILD_MEMBER_ADD_RECV*)lpMsg);
					break;
				case 0x03:
					gGuild.DGGuildMemberDelRecv((SDHP_GUILD_MEMBER_DEL_RECV*)lpMsg);
					break;
				case 0x04:
					gGuild.DGGuildMemberUpdateRecv((SDHP_GUILD_MEMBER_UPDATE_RECV*)lpMsg);
					break;
				case 0x05:
					gGuild.DGGuildAssignStatusRecv((SDHP_GUILD_ASSIGN_STATUS_RECV*)lpMsg);
					break;
				case 0x06:
					gGuild.DGGuildAssignTypeRecv((SDHP_GUILD_ASSIGN_TYPE_RECV*)lpMsg);
					break;
				case 0x07:
					gGuild.DGGuildNoticeRecv((SDHP_GUILD_NOTICE_RECV*)lpMsg);
					break;
				case 0x08:
					gGuild.DGGuildPostRecv((SDHP_GUILD_POST_RECV*)lpMsg);
					break;
				case 0x09:
					gGuild.DGGuildScoreRecv((SDHP_GUILD_SCORE_RECV*)lpMsg);
					break;
				case 0x0A:
					gGuild.DGGuildInfoRecv((SDHP_GUILD_INFO_RECV*)lpMsg);
					break;
				case 0x0B:
					gGuild.DGUnionInfoRecv((SDHP_UNION_INFO_RECV*)lpMsg);
					break;
				case 0x0C:
					gGuild.DGUnionUpdateRecv((SDHP_UNION_UPDATE_RECV*)lpMsg);
					break;
				case 0x0D:
					gGuild.DGGuildUnionInfoRecv((SDHP_GUILD_UNION_INFO_RECV*)lpMsg);
					break;
				case 0x0E:
					gGuild.DGGuildUnionJoinRecv((SDHP_GUILD_UNION_JOIN_RECV*)lpMsg);
					break;				
				case 0x0F:
					gGuild.DGGuildUnionBreakRecv((SDHP_GUILD_UNION_BREAK_RECV*)lpMsg);
					break;
				case 0x10:
					gGuild.DGGuildAllianceKickRecv((SDHP_GUILD_ALLIANCE_KICK_RECV*)lpMsg);
					break;
				}
			break;
		case 0xB0:
			switch(((lpMsg[0]==0xC1)?lpMsg[3]:lpMsg[4]))
			{
				case 0x00:
					gFriend.DGFriendListRecv((SDHP_FRIEND_LIST_RECV*)lpMsg);
					break;
				case 0x01:
					gFriend.DGFriendResultRecv((SDHP_FRIEND_RESULT_RECV*)lpMsg);
					break;
				case 0x02:
					gFriend.DGFriendStateRecv((SDHP_FRIEND_STATE_RECV*)lpMsg);
					break;
				case 0x03:
					gFriend.DGFriendRequestRecv((SDHP_FRIEND_REQUEST_RECV*)lpMsg);
					break;
				case 0x04:
					gFriend.DGFriendDeleteRecv((SDHP_FRIEND_DELETE_RECV*)lpMsg);
					break;
				case 0x05:
					gFriend.DGFriendMailResultRecv((SDHP_FRIEND_MAIL_RESULT_RECV*)lpMsg);
					break;
				case 0x06:
					gFriend.DGFriendMailListRecv((SDHP_FRIEND_MAIL_LIST_RECV*)lpMsg);
					break;
				case 0x07:
					gFriend.DGFriendMailReadRecv((SDHP_FRIEND_MAIL_READ_RECV*)lpMsg);
					break;
				case 0x08:
					gFriend.DGFriendMailDeleteRecv((SDHP_FRIEND_MAIL_DELETE_RECV*)lpMsg);
					break;
			}
			break;
	}

	PROTECT_FINAL
}

void DGCharacterListRecv(SDHP_CHARACTER_LIST_RECV* lpMsg) // OK
{
	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGCharacterListRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	BYTE send[256];

	PMSG_CHARACTER_LIST_SEND pMsg;

	pMsg.header.setE(0xF3,0x00,0);

	int size = sizeof(pMsg);

	pMsg.MoveCnt = lpMsg->MoveCnt;

	pMsg.count = 0;

	#if(GAMESERVER_UPDATE>=602)

	pMsg.ExtWarehouse = lpMsg->ExtWarehouse;

	#endif

	PMSG_CHARACTER_LIST info;

	WORD CharacterCreationLevel = 0;

	for(int n=0;n < lpMsg->count;n++)
	{
		SDHP_CHARACTER_LIST* lpInfo = (SDHP_CHARACTER_LIST*)(((BYTE*)lpMsg)+sizeof(SDHP_CHARACTER_LIST_RECV)+(sizeof(SDHP_CHARACTER_LIST)*n));

		info.slot = lpInfo->slot;

		memcpy(info.Name,lpInfo->name,sizeof(info.Name));

		info.Level = lpInfo->level;

		CharacterCreationLevel = ((info.Level>CharacterCreationLevel)?info.Level:CharacterCreationLevel);

		info.CtlCode = lpInfo->CtlCode;

		#pragma region CHAR_SET_CALC

		memset(info.CharSet,0,sizeof(info.CharSet));

		info.CharSet[0] = (lpInfo->Class%16)*16;
		info.CharSet[0] -= (info.CharSet[0]/32);
		info.CharSet[0] += (lpInfo->Class/16)*32;

		WORD TempInventory[9];

		for(int i=0;i < 9;i++)
		{
			if(i == 0 || i == 1)
			{
				if(lpInfo->Inventory[0+(i*5)] == 0xFF && (lpInfo->Inventory[2+(i*5)] & 0x80) == 0x80 && (lpInfo->Inventory[4+(i*5)] & 0xF0) == 0xF0)
				{
					TempInventory[i] = 0xFFFF;
				}
				else
				{
					TempInventory[i] = (lpInfo->Inventory[0+(i*5)]+((lpInfo->Inventory[2+(i*5)] & 0x80)*2)+((lpInfo->Inventory[4+(i*5)] & 0xF0)*32));
				}
			}
			else
			{
				if(lpInfo->Inventory[0+(i*5)] == 0xFF && (lpInfo->Inventory[2+(i*5)] & 0x80) == 0x80 && (lpInfo->Inventory[4+(i*5)] & 0xF0) == 0xF0)
				{
					TempInventory[i] = (MAX_ITEM_TYPE-1);
				}
				else
				{
					TempInventory[i] = (lpInfo->Inventory[0+(i*5)]+((lpInfo->Inventory[2+(i*5)] & 0x80)*2)+((lpInfo->Inventory[4+(i*5)] & 0xF0)*32))%MAX_ITEM_TYPE;
				}
			}
		}
		
		info.CharSet[1] = TempInventory[INVENTORY_SLOT_WEAPON1]%256;
		info.CharSet[12] |= (TempInventory[INVENTORY_SLOT_WEAPON1]/16) & 0xF0;

		info.CharSet[2] = TempInventory[INVENTORY_SLOT_WEAPON2]%256;
		info.CharSet[13] |= (TempInventory[INVENTORY_SLOT_WEAPON2]/16) & 0xF0;

		info.CharSet[3] |= (TempInventory[INVENTORY_SLOT_HELM] & 0x0F) << 4;
		info.CharSet[9] |= (TempInventory[INVENTORY_SLOT_HELM] & 0x10) << 3;
		info.CharSet[13] |= (TempInventory[INVENTORY_SLOT_HELM] & 0x1E0) >> 5;

		info.CharSet[3] |= (TempInventory[INVENTORY_SLOT_ARMOR] & 0x0F);
		info.CharSet[9] |= (TempInventory[INVENTORY_SLOT_ARMOR] & 0x10) << 2;
		info.CharSet[14] |= (TempInventory[INVENTORY_SLOT_ARMOR] & 0x1E0) >> 1;

		info.CharSet[4] |= (TempInventory[INVENTORY_SLOT_PANTS] & 0x0F) << 4;
		info.CharSet[9] |= (TempInventory[INVENTORY_SLOT_PANTS] & 0x10) << 1;
		info.CharSet[14] |= (TempInventory[INVENTORY_SLOT_PANTS] & 0x1E0) >> 5;

		info.CharSet[4] |= (TempInventory[INVENTORY_SLOT_GLOVES] & 0x0F);
		info.CharSet[9] |= (TempInventory[INVENTORY_SLOT_GLOVES] & 0x10);
		info.CharSet[15] |= (TempInventory[INVENTORY_SLOT_GLOVES] & 0x1E0) >> 1;

		info.CharSet[5] |= (TempInventory[INVENTORY_SLOT_BOOTS] & 0x0F) << 4;
		info.CharSet[9] |= (TempInventory[INVENTORY_SLOT_BOOTS] & 0x10) >> 1;
		info.CharSet[15] |= (TempInventory[INVENTORY_SLOT_BOOTS] & 0x1E0) >> 5;

		int level = 0;

		BYTE table[7] = {1,0,6,5,4,3,2};

		for(int i=0;i < 7;i++)
		{
			if(TempInventory[i] != (MAX_ITEM_TYPE-1) && TempInventory[i] != 0xFFFF)
			{
				level |= ((((lpInfo->Inventory[1+(i*5)]/8) & 0x0F)-1)/2) << (i*3);

				info.CharSet[10] |= ((lpInfo->Inventory[2+(i*5)] & 0x3F)?2:0) << table[i];
				info.CharSet[11] |= ((lpInfo->Inventory[3+(i*5)] & 0x03)?2:0) << table[i];
			}
		}

		info.CharSet[6] = level >> 16;
		info.CharSet[7] = level >> 8;
		info.CharSet[8] = level;

		#if(GAMESERVER_UPDATE>=601)

		if(TempInventory[INVENTORY_SLOT_WING] >= 0 && TempInventory[INVENTORY_SLOT_WING] <= 2)
		{
			info.CharSet[5] |= 4;
			info.CharSet[9] |= TempInventory[INVENTORY_SLOT_WING]+1;
		}
		else if(TempInventory[INVENTORY_SLOT_WING] >= 3 && TempInventory[INVENTORY_SLOT_WING] <= 6)
		{
			info.CharSet[5] |= 8;
			info.CharSet[9] |= TempInventory[INVENTORY_SLOT_WING]-2;
		}
		else if(TempInventory[INVENTORY_SLOT_WING] == 30)
		{
			info.CharSet[5] |= 8;
			info.CharSet[9] |= 5;
		}
		else if(TempInventory[INVENTORY_SLOT_WING] >= 36 && TempInventory[INVENTORY_SLOT_WING] <= 40)
		{
			info.CharSet[5] |= 12;
			info.CharSet[9] |= TempInventory[INVENTORY_SLOT_WING]-35;
		}
		else if(TempInventory[INVENTORY_SLOT_WING] == 41)
		{
			info.CharSet[5] |= 4;
			info.CharSet[9] |= 4;
		}
		else if(TempInventory[INVENTORY_SLOT_WING] == 42)
		{
			info.CharSet[5] |= 8;
			info.CharSet[9] |= 6;
		}
		else if(TempInventory[INVENTORY_SLOT_WING] == 43)
		{
			info.CharSet[5] |= 12;
			info.CharSet[9] |= 6;
		}
		else if(TempInventory[INVENTORY_SLOT_WING] == 49)
		{
			info.CharSet[5] |= 8;
			info.CharSet[9] |= 7;
		}
		else if(TempInventory[INVENTORY_SLOT_WING] == 50)
		{
			info.CharSet[5] |= 12;
			info.CharSet[9] |= 7;
		}
		else if(TempInventory[INVENTORY_SLOT_WING] >= 130 && TempInventory[INVENTORY_SLOT_WING] <= 135)
		{
			info.CharSet[5] |= 12;
			info.CharSet[17] |= (TempInventory[INVENTORY_SLOT_WING]-129) << 5;
		}
		else if(TempInventory[INVENTORY_SLOT_WING] >= 262 && TempInventory[INVENTORY_SLOT_WING] <= 265)
		{
			info.CharSet[5] |= 8;
			info.CharSet[16] |= (TempInventory[INVENTORY_SLOT_WING]-261) << 2;
		}
		else if(TempInventory[INVENTORY_SLOT_WING] == 266)
		{
			info.CharSet[5] |= 4;
			info.CharSet[9] |= 5;
		}
		else if(TempInventory[INVENTORY_SLOT_WING] == 267)
		{
			info.CharSet[5] |= 4;
			info.CharSet[9] |= 6;
		}
		else if(gCustomWing.CheckCustomWingByItem(GET_ITEM(12,TempInventory[INVENTORY_SLOT_WING])) != 0)
		{
			info.CharSet[5] |= 12;
			info.CharSet[17] |= (gCustomWing.GetCustomWingIndex(GET_ITEM(12,TempInventory[INVENTORY_SLOT_WING]))+1) << 1;
		}

		#else

		if(TempInventory[INVENTORY_SLOT_WING] == (MAX_ITEM_TYPE-1))
		{
			info.CharSet[5] |= 12;
		}
		else if(TempInventory[INVENTORY_SLOT_WING] >= 0 && TempInventory[INVENTORY_SLOT_WING] <= 2)
		{
			info.CharSet[5] |= TempInventory[INVENTORY_SLOT_WING] << 2;
		}
		else if(TempInventory[INVENTORY_SLOT_WING] >= 3 && TempInventory[INVENTORY_SLOT_WING] <= 6)
		{
			info.CharSet[5] |= 12;
			info.CharSet[9] |= TempInventory[INVENTORY_SLOT_WING]-2;
		}
		else if(TempInventory[INVENTORY_SLOT_WING] == 30)
		{
			info.CharSet[5] |= 12;
			info.CharSet[9] |= 5;
		}
		#if(GAMESERVER_UPDATE>=301)
		else if(TempInventory[INVENTORY_SLOT_WING] >= 36 && TempInventory[INVENTORY_SLOT_WING] <= 40)
		{
			info.CharSet[5] |= 12;
			info.CharSet[16] |= (TempInventory[INVENTORY_SLOT_WING]-35) << 2;
		}
		#endif
		#if(GAMESERVER_UPDATE>=401)
		else if(TempInventory[INVENTORY_SLOT_WING] == 41)
		{
			info.CharSet[5] |= 12;
			info.CharSet[9] |= 6;
		}
		else if(TempInventory[INVENTORY_SLOT_WING] == 42)
		{
			info.CharSet[5] |= 12;
			info.CharSet[16] |= 28;
		}
		else if(TempInventory[INVENTORY_SLOT_WING] == 43)
		{
			info.CharSet[5] |= 12;
			info.CharSet[16] |= 24;
		}
		#endif
		else if(gCustomWing.CheckCustomWingByItem(GET_ITEM(12,TempInventory[INVENTORY_SLOT_WING])) != 0)
		{
			info.CharSet[5] |= 12;
			info.CharSet[17] |= (gCustomWing.GetCustomWingIndex(GET_ITEM(12,TempInventory[INVENTORY_SLOT_WING]))+1) << 1;
		}

		#endif

		if(TempInventory[INVENTORY_SLOT_HELPER] == (MAX_ITEM_TYPE-1))
		{
			info.CharSet[5] |= 3;
		}
		else if(TempInventory[INVENTORY_SLOT_HELPER] >= 0 && TempInventory[INVENTORY_SLOT_HELPER] <= 2)
		{
			info.CharSet[5] |= TempInventory[INVENTORY_SLOT_HELPER];
		}
		else if(TempInventory[INVENTORY_SLOT_HELPER] == 3)
		{
			info.CharSet[5] |= 3;
			info.CharSet[10] |= 1;
		}
		else if(TempInventory[INVENTORY_SLOT_HELPER] == 4)
		{
			info.CharSet[5] |= 3;
			info.CharSet[12] |= 1;
		}
		#if(GAMESERVER_UPDATE>=201)
		else if(TempInventory[INVENTORY_SLOT_HELPER] == 37)
		{
			info.CharSet[5] |= 3;
			info.CharSet[10] &= 0xFE;
			info.CharSet[12] &= 0xFE;
			info.CharSet[12] |= 4;

			if((lpInfo->Inventory[42] & 1) != 0)
			{
				info.CharSet[16] |= 1;
			}
			else if((lpInfo->Inventory[42] & 2) != 0)
			{
				info.CharSet[16] |= 2;
			}
			else if((lpInfo->Inventory[42] & 4) != 0)
			{
				info.CharSet[17] |= 1;
			}
		}
		#endif
		#if(GAMESERVER_UPDATE>=401)
		else if(TempInventory[INVENTORY_SLOT_HELPER] == 64 || TempInventory[INVENTORY_SLOT_HELPER] == 65 || TempInventory[INVENTORY_SLOT_HELPER] == 67)
		{
			info.CharSet[16] |= (TempInventory[INVENTORY_SLOT_HELPER]-63) << 5;
		}
		#endif
		#if(GAMESERVER_UPDATE>=601)
		else if(TempInventory[INVENTORY_SLOT_HELPER] == 80)
		{
			info.CharSet[16] |= 0xE0;
		}
		else if(TempInventory[INVENTORY_SLOT_HELPER] == 106)
		{
			info.CharSet[16] |= 0xA0;
		}
		else if(TempInventory[INVENTORY_SLOT_HELPER] == 123)
		{
			info.CharSet[16] |= 0x60;
		}
		#endif

		#pragma endregion

		info.GuildStatus = lpInfo->GuildStatus;

		memcpy(&send[size],&info,sizeof(info));
		size += sizeof(info);

		pMsg.count++;
	}

	#if(GAMESERVER_UPDATE>=201)

	pMsg.ClassCode = 0;

	if((gServerInfo.m_MGCreateType == 0 && CharacterCreationLevel >= gServerInfo.m_MGCreateLevel[gObj[lpMsg->index].AccountLevel]) || (gServerInfo.m_MGCreateType != 0 && (lpMsg->ExtClass & CARD_CODE_MG) != 0))
	{
		pMsg.ClassCode |= CARD_CODE_MG;
	}

	if((gServerInfo.m_DLCreateType == 0 && CharacterCreationLevel >= gServerInfo.m_DLCreateLevel[gObj[lpMsg->index].AccountLevel]) || (gServerInfo.m_DLCreateType != 0 && (lpMsg->ExtClass & CARD_CODE_DL) != 0))
	{
		pMsg.ClassCode |= CARD_CODE_DL;
	}

	if((gServerInfo.m_SUCreateType == 0 && CharacterCreationLevel >= gServerInfo.m_SUCreateLevel[gObj[lpMsg->index].AccountLevel]) || (gServerInfo.m_SUCreateType != 0 && (lpMsg->ExtClass & CARD_CODE_SU) != 0))
	{
		pMsg.ClassCode |= CARD_CODE_SU;
	}

	if((gServerInfo.m_RFCreateType == 0 && CharacterCreationLevel >= gServerInfo.m_RFCreateLevel[gObj[lpMsg->index].AccountLevel]) || (gServerInfo.m_RFCreateType != 0 && (lpMsg->ExtClass & CARD_CODE_RF) != 0))
	{
		pMsg.ClassCode |= CARD_CODE_RF;
	}

	gObj[lpMsg->index].ClassCode = pMsg.ClassCode;

	#else

	pMsg.ClassCode = 2;

	gObj[lpMsg->index].ClassCode = 0;

	if(CharacterCreationLevel >= gServerInfo.m_MGCreateLevel[gObj[lpMsg->index].AccountLevel])
	{
		pMsg.ClassCode = 3;

		gObj[lpMsg->index].ClassCode |= CARD_CODE_MG;
	}

	if(CharacterCreationLevel >= gServerInfo.m_DLCreateLevel[gObj[lpMsg->index].AccountLevel])
	{
		pMsg.ClassCode = 4;

		gObj[lpMsg->index].ClassCode |= CARD_CODE_DL;
	}

	#endif

	#if(GAMESERVER_UPDATE>=602)

	gObj[lpMsg->index].ExtWarehouse = pMsg.ExtWarehouse;

	#endif

	pMsg.header.size = size;

	memcpy(send,&pMsg,sizeof(pMsg));

	DataSend(lpMsg->index,send,size);

	#if(GAMESERVER_UPDATE>=201)

	GCCharacterCreationEnableSend(lpMsg->index,0,gObj[lpMsg->index].ClassCode);
	
	#else

	GCCharacterCreationEnableSend(lpMsg->index,0,pMsg.ClassCode);

	#endif
}

void DGCharacterCreateRecv(SDHP_CHARACTER_CREATE_RECV* lpMsg) // OK
{
	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGCharacterCreateRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	PMSG_CHARACTER_CREATE_SEND pMsg;

	pMsg.header.set(0xF3,0x01,sizeof(pMsg));

	pMsg.result = lpMsg->result;

	memcpy(pMsg.name,lpMsg->name,sizeof(pMsg.name));

	pMsg.slot = lpMsg->slot;

	pMsg.level = lpMsg->level;

	pMsg.Class = (lpMsg->Class%16)*16;
	pMsg.Class -= (pMsg.Class/32);
	pMsg.Class += (lpMsg->Class/16)*32;

	memcpy(pMsg.equipment,lpMsg->equipment,sizeof(pMsg.equipment));

	DataSend(lpMsg->index,(BYTE*)&pMsg,pMsg.header.size);
}

void DGCharacterDeleteRecv(SDHP_CHARACTER_DELETE_RECV* lpMsg) // OK
{
	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGCharacterDeleteRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	PMSG_CHARACTER_DELETE_SEND pMsg;

	pMsg.header.set(0xF3,0x02,sizeof(pMsg));

	pMsg.result = lpMsg->result;
	
	DataSend(lpMsg->index,(BYTE*)&pMsg,pMsg.header.size);
}

void DGCharacterInfoRecv(SDHP_CHARACTER_INFO_RECV* lpMsg) // OK
{
	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGCharacterInfoRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	if(lpMsg->result == 0)
	{
		CloseClient(lpMsg->index);
		return;
	}

	if((lpMsg->CtlCode & 1) != 0)
	{
		CloseClient(lpMsg->index);
		return;
	}

	if(gObjectManager.CharacterInfoSet((BYTE*)lpMsg,lpMsg->index) == 0)
	{
		CloseClient(lpMsg->index);
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	if(lpObj->MapServerMoveRequest == 0)
	{
		if((lpObj->NextServerCode=gMapServerManager.CheckMapServerMove(lpObj->Index,lpObj->Map,lpObj->LastServerCode)) != gServerInfo.m_ServerCode)
		{
			if(lpObj->NextServerCode == -1)
			{
				CloseClient(lpObj->Index);
				return;
			}
			else
			{
				GJMapServerMoveSend(lpObj->Index,lpObj->NextServerCode,lpObj->Map,(BYTE)lpObj->X,(BYTE)lpObj->Y);
				return;
			}
		}
	}

	PMSG_CHARACTER_INFO_SEND pMsg;

	pMsg.header.setE(0xF3,0x03,sizeof(pMsg));

	pMsg.X = (BYTE)lpObj->X;
	pMsg.Y = (BYTE)lpObj->Y;
	pMsg.Map = lpObj->Map;
	pMsg.Dir = lpObj->Dir;

	
	#if(GAMESERVER_UPDATE>=401)

	if(gMasterSkillTree.CheckMasterLevel(lpObj) == 0)
	{
		pMsg.Experience[0] = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERHDW(lpObj->Experience)));
		pMsg.Experience[1] = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERHDW(lpObj->Experience)));
		pMsg.Experience[2] = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERHDW(lpObj->Experience)));
		pMsg.Experience[3] = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERHDW(lpObj->Experience)));
		pMsg.Experience[4] = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERLDW(lpObj->Experience)));
		pMsg.Experience[5] = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERLDW(lpObj->Experience)));
		pMsg.Experience[6] = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERLDW(lpObj->Experience)));
		pMsg.Experience[7] = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERLDW(lpObj->Experience)));
		pMsg.NextExperience[0] = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERHDW(lpObj->NextExperience)));
		pMsg.NextExperience[1] = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERHDW(lpObj->NextExperience)));
		pMsg.NextExperience[2] = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERHDW(lpObj->NextExperience)));
		pMsg.NextExperience[3] = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERHDW(lpObj->NextExperience)));
		pMsg.NextExperience[4] = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERLDW(lpObj->NextExperience)));
		pMsg.NextExperience[5] = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERLDW(lpObj->NextExperience)));
		pMsg.NextExperience[6] = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERLDW(lpObj->NextExperience)));
		pMsg.NextExperience[7] = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERLDW(lpObj->NextExperience)));
	}
	else
	{
		pMsg.Experience[0] = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERHDW(lpObj->MasterExperience)));
		pMsg.Experience[1] = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERHDW(lpObj->MasterExperience)));
		pMsg.Experience[2] = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERHDW(lpObj->MasterExperience)));
		pMsg.Experience[3] = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERHDW(lpObj->MasterExperience)));
		pMsg.Experience[4] = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERLDW(lpObj->MasterExperience)));
		pMsg.Experience[5] = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERLDW(lpObj->MasterExperience)));
		pMsg.Experience[6] = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERLDW(lpObj->MasterExperience)));
		pMsg.Experience[7] = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERLDW(lpObj->MasterExperience)));
		pMsg.NextExperience[0] = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERHDW(lpObj->MasterNextExperience)));
		pMsg.NextExperience[1] = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERHDW(lpObj->MasterNextExperience)));
		pMsg.NextExperience[2] = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERHDW(lpObj->MasterNextExperience)));
		pMsg.NextExperience[3] = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERHDW(lpObj->MasterNextExperience)));
		pMsg.NextExperience[4] = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERLDW(lpObj->MasterNextExperience)));
		pMsg.NextExperience[5] = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERLDW(lpObj->MasterNextExperience)));
		pMsg.NextExperience[6] = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERLDW(lpObj->MasterNextExperience)));
		pMsg.NextExperience[7] = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERLDW(lpObj->MasterNextExperience)));
	}

	#else

	pMsg.Experience = lpObj->Experience;
	pMsg.NextExperience = lpObj->NextExperience;

	#endif

	pMsg.LevelUpPoint = lpObj->LevelUpPoint;
	pMsg.Strength = GET_MAX_WORD_VALUE(lpObj->Strength);
	pMsg.Dexterity = GET_MAX_WORD_VALUE(lpObj->Dexterity);
	pMsg.Vitality = GET_MAX_WORD_VALUE(lpObj->Vitality);
	pMsg.Energy = GET_MAX_WORD_VALUE(lpObj->Energy);
	pMsg.Life = GET_MAX_WORD_VALUE(lpObj->Life);
	pMsg.MaxLife = GET_MAX_WORD_VALUE((lpObj->MaxLife+lpObj->AddLife));
	pMsg.Mana = GET_MAX_WORD_VALUE(lpObj->Mana);
	pMsg.MaxMana = GET_MAX_WORD_VALUE((lpObj->MaxMana+lpObj->AddMana));
	#if(GAMESERVER_UPDATE>=201)
	pMsg.Shield = GET_MAX_WORD_VALUE(lpObj->Shield);
	pMsg.MaxShield = GET_MAX_WORD_VALUE((lpObj->MaxShield+lpObj->AddShield));
	#endif
	pMsg.BP = GET_MAX_WORD_VALUE(lpObj->BP);
	pMsg.MaxBP = GET_MAX_WORD_VALUE((lpObj->MaxBP+lpObj->AddBP));
	pMsg.Money = lpObj->Money;
	pMsg.PKLevel = lpObj->PKLevel;
	pMsg.CtlCode = lpMsg->CtlCode;
	pMsg.FruitAddPoint = lpObj->FruitAddPoint;
	pMsg.MaxFruitAddPoint = gFruit.GetMaxFruitPoint(lpObj);
	pMsg.Leadership = GET_MAX_WORD_VALUE(lpObj->Leadership);
	pMsg.FruitSubPoint = lpObj->FruitSubPoint;
	pMsg.MaxFruitSubPoint = gFruit.GetMaxFruitPoint(lpObj);
	#if(GAMESERVER_UPDATE>=602)
	pMsg.ExtInventory = lpObj->ExtInventory;
	#endif

	#if(GAMESERVER_EXTRA==1)
	pMsg.ViewReset = (DWORD)(lpObj->Reset);
	pMsg.ViewPoint = (DWORD)(lpObj->LevelUpPoint);
	pMsg.ViewCurHP = (DWORD)(lpObj->Life);
	pMsg.ViewMaxHP = (DWORD)(lpObj->MaxLife+lpObj->AddLife);
	pMsg.ViewCurMP = (DWORD)(lpObj->Mana);
	pMsg.ViewMaxMP = (DWORD)(lpObj->MaxMana+lpObj->AddMana);
	pMsg.ViewCurBP = (DWORD)(lpObj->BP);
	pMsg.ViewMaxBP = (DWORD)(lpObj->MaxBP+lpObj->AddBP);
	#if(GAMESERVER_UPDATE>=201)
	pMsg.ViewCurSD = (DWORD)(lpObj->Shield);
	pMsg.ViewMaxSD = (DWORD)(lpObj->MaxShield+lpObj->AddShield);
	#endif
	pMsg.ViewStrength = lpObj->Strength;
	pMsg.ViewDexterity = lpObj->Dexterity;
	pMsg.ViewVitality = lpObj->Vitality;
	pMsg.ViewEnergy = lpObj->Energy;
	pMsg.ViewLeadership = lpObj->Leadership;
	pMsg.Class = lpObj->Class;
	#endif

	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);

	GDConnectCharacterSend(lpObj->Index);

	GDOptionDataSend(lpObj->Index);

	gFriend.CGFriendListRecv(lpObj->Index);

	GDPetItemInfoSend(lpObj->Index,0);

	#if(GAMESERVER_UPDATE>=301)

	gQuest.GDQuestKillCountSend(lpObj->Index);

	#endif

	#if(GAMESERVER_UPDATE>=401)

	gMasterSkillTree.GDMasterSkillTreeSend(lpObj->Index);

	#endif

	#if(GAMESERVER_UPDATE>=501)

	gQuestWorld.GDQuestWorldSend(lpObj->Index);

	gGensSystem.GDGensSystemMemberSend(lpObj->Index);

	#endif

	#if(GAMESERVER_UPDATE>=603)

	gHelper.GDHelperDataSend(lpObj->Index);

	#endif

	#if(GAMESERVER_SHOP==3)

	gCashShop.GDCashShopPeriodicItemSend(lpObj->Index);

	gCashShop.CGCashShopPointRecv(lpObj->Index);

	#endif

	#if(GAMESERVER_SHOP==1)

	gPcPoint.GDPcPointPointSend(lpObj->Index);

	#endif

	#if(GAMESERVER_UPDATE>=602)

	gLuckyItem.GDLuckyItemSend(lpObj->Index);

	#endif

	#if(GAMESERVER_UPDATE>=701)

	gPentagramSystem.GDPentagramJewelInfoSend(lpObj->Index,PENTAGRAM_JEWEL_TYPE_INVENTORY);

	gPentagramSystem.GDPentagramJewelInfoSend(lpObj->Index,PENTAGRAM_JEWEL_TYPE_WAREHOUSE);

	#endif

	#if(GAMESERVER_UPDATE>=801)

	GDSNSDataSend(lpObj->Index);

	#endif

	#if(GAMESERVER_UPDATE>=802)

	gPersonalShop.GDPShopItemValueSend(lpObj->Index);

	gEventInventory.GDEventInventorySend(lpObj->Index);

	gMuRummy.GDReqCardInfo(lpObj);

	#endif

	#if(GAMESERVER_UPDATE>=803)

	gMuunSystem.GDMuunInventorySend(lpObj->Index);

	#endif

	gItemManager.GCItemListSend(lpObj->Index);

	gSkillManager.GCSkillListSend(lpObj,0);

	gQuest.GCQuestInfoSend(lpObj->Index);

	#if(GAMESERVER_SHOP==3)

	gCashShop.GCCashShopInitSend(lpObj);

	#endif

	#if(GAMESERVER_UPDATE>=603)

	gHelper.GCHelperStartSend(lpObj->Index,0,0,1);

	#endif

	GCNewCharacterInfoSend(lpObj);

	GCNewGensBattleInfoSend(lpObj);

	gReconnect.GetReconnectInfo(lpObj);

	gSkillManager.SkillChangeUse(lpObj->Index);

	gObjViewportListProtocolCreate(lpObj);

	gObjectManager.CharacterUpdateMapEffect(lpObj);

	gDefaultClassFreebies.GetCharacterFreebies(lpObj,lpMsg->IsNewChar);
	
	gScriptLoader.OnCharacterEntry(lpObj->Index);

	gInvasionManager.CheckEvent(lpObj);

	lpObj->MapServerMoveRequest = 0;

	gEffectManager.GeneratePeriodicEffectList(lpObj);

	gCustomPick.GDCustomPickSend(lpObj->Index);

	gLog.Output(LOG_CONNECT,"[ObjectManager][%d] AddCharacterInfo [%s] [%s][%s]",lpObj->Index,lpObj->Name,lpObj->IpAddr,lpObj->HardwareId);


#if USE_FAKE_ONLINE == TRUE
    //=========== Fake ONline
    OFFEXP_DATA* info = s_FakeOnline.GetOffExpInfo(lpObj);
    if (info != 0 && lpObj->Socket == INVALID_SOCKET)
    {
        lpObj->IsFakeOnline = true;
        lpObj->IsFakePVPMode = info->PVPMode;
        lpObj->IsFakeMoveRange = info->MoveRange;
        lpObj->IsFakePartyMode = info->PartyMode;

        lpObj->SkillBasicID = info->SkillID;
        lpObj->HuntingRange = (DWORD)(info->PhamViTrain);
        lpObj->DistanceMin = (DWORD)(info->TimeReturn);
        lpObj->m_OfflineMoveDelay = GetTickCount();//Time di chuyen

        if (info->UseBuffs[0] > 0) {
            lpObj->BuffOn = 1;
            //if(info->BuffParty == 1)
            {
                lpObj->PartyModeOn = 1;    //Buff Heal Elf
                lpObj->PartyModeBuffOn = 1; //Buff Cho Party
            }
            lpObj->BuffSkill[0] = info->UseBuffs[0];
            lpObj->BuffSkill[1] = info->UseBuffs[1];
            lpObj->BuffSkill[2] = info->UseBuffs[2];
        }

        lpObj->RecoveryPotionOn = 1;
        lpObj->RecoveryPotionPercent = 50;

        lpObj->DistanceLongOn = 1;
        lpObj->DistanceReturnOn = 1;

        lpObj->m_OfflineCoordX = info->MapX;
        lpObj->m_OfflineCoordY = info->MapY;
        lpObj->m_OfflineTimeResetMove = GetTickCount();
        lpObj->AttackCustomDelay = GetTickCount();

        lpObj->ObtainRepairOn = 1; //TU Sua do

        if (info->TuNhatItem == 1) {
            lpObj->ObtainRange = (DWORD)(8);
            lpObj->ObtainPickSelected = 1;
            lpObj->ObtainPickJewels = 1;
            lpObj->ObtainPickMoney = 1;
            lpObj->ObtainPickExcellent = 1;
            lpObj->ObtainPickAncient = 1;
        }
        //==Set Pass Party Tu Dong==//
        if (lpObj->IsFakePartyMode == 1) {
            lpObj->Option |= 1;
            lpObj->Option |= 2;
            lpObj->Option |= 4;
        }

        if (info->TuDongReset == 1 && lpObj->AutoResetEnable == 0) {
            lpObj->AutoResetEnable = 1;

        } /*else if(info->TuDongReset == 2 && lpObj->AutoResetVipEnable == 0) {
          lpObj->AutoResetVipEnable = 1;
          }*/
        LogAdd(LOG_RED, "[FAKE ONLINE][Obj][%d] AddCharacterInfo (%s) OK", lpObj->Index, lpObj->Name);
    }
#endif


}

void DGCreateItemRecv(SDHP_CREATE_ITEM_RECV* lpMsg) // OK
{
	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGCreateItemRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	if(lpMsg->Map != 0xFF && lpMsg->Map != 0xFE && lpMsg->Map != 0xED && lpMsg->Map != 0xEC && lpMsg->Map != 0xEB && gMapManager.IsValidMap(lpMsg->Map) == 0)
	{
		return;
	}

	if(lpMsg->Map == 0xFE || lpMsg->Map == 0xFF)
	{
		if((lpMsg->Map == 0xFE && lpObj->Interface.type != INTERFACE_TRAINER) || (lpMsg->Map == 0xFF && lpObj->Interface.type != INTERFACE_CHAOS_BOX))
		{
			return;
		}

		CItem item;

		item.m_Level = lpMsg->Level;

		item.m_Serial = lpMsg->Serial;

		if(lpMsg->ItemIndex == GET_ITEM(13,18) || lpMsg->ItemIndex == GET_ITEM(13,37) || lpMsg->ItemIndex == GET_ITEM(13,51) || lpMsg->ItemIndex == GET_ITEM(14,7) || lpMsg->ItemIndex == GET_ITEM(14,19) || lpMsg->ItemIndex == GET_ITEM(14,109))
		{
			item.m_Durability = (float)lpMsg->Dur;
		}
		else
		{
			item.m_Durability = (float)gItemManager.GetItemDurability(lpMsg->ItemIndex,lpMsg->Level,lpMsg->NewOption,lpMsg->SetOption);
		}

		item.Convert(lpMsg->ItemIndex,lpMsg->Option1,lpMsg->Option2,lpMsg->Option3,lpMsg->NewOption,lpMsg->SetOption,lpMsg->JewelOfHarmonyOption,lpMsg->ItemOptionEx,lpMsg->SocketOption,lpMsg->SocketOptionBonus);

		if(lpMsg->ItemIndex == GET_ITEM(13,4) || lpMsg->ItemIndex == GET_ITEM(13,5))
		{
			item.SetPetItemInfo(1,0);
		}

		item.m_IsPeriodicItem = ((lpMsg->Duration>0)?1:0);

		item.m_LoadPeriodicItem = ((lpMsg->Duration>0)?1:0);

		item.m_PeriodicItemTime = ((lpMsg->Duration>0)?lpMsg->Duration:0);

		gChaosBox.ChaosBoxInit(lpObj);

		gItemManager.ChaosBoxAddItem(lpObj->Index,item,0);

		gChaosBox.GCChaosMixSend(lpObj->Index,((lpMsg->Map==0xFF)?1:100),&item);
	}
	else if(lpMsg->Map == 0xEB)
	{
		CItem item;

		item.m_Level = lpMsg->Level;

		item.m_Serial = lpMsg->Serial;

		item.m_Durability = lpMsg->Dur;

		item.Convert(lpMsg->ItemIndex,lpMsg->Option1,lpMsg->Option2,lpMsg->Option3,lpMsg->NewOption,lpMsg->SetOption,lpMsg->JewelOfHarmonyOption,lpMsg->ItemOptionEx,lpMsg->SocketOption,lpMsg->SocketOptionBonus);

		if(lpMsg->ItemIndex == GET_ITEM(13,4) || lpMsg->ItemIndex == GET_ITEM(13,5))
		{
			item.SetPetItemInfo(1,0);
		}

		item.m_IsPeriodicItem = ((lpMsg->Duration>0)?1:0);

		item.m_LoadPeriodicItem = ((lpMsg->Duration>0)?1:0);

		item.m_PeriodicItemTime = ((lpMsg->Duration>0)?lpMsg->Duration:0);

		BYTE slot = gItemManager.InventoryInsertItem(lpObj->Index,item);

		if(slot != 0xFF)
		{
			gItemManager.GCItemModifySend(lpObj->Index,slot);
			if(lpMsg->Duration > 0){gCashShop.GCCashShopPeriodicItemSend(lpObj->Index,item.m_Index,slot,item.m_PeriodicItemTime);}
		}
	}
	else if(lpMsg->Map == 0xEC)
	{
		CItem item;

		item.m_Level = lpMsg->Level;

		item.m_Serial = lpMsg->Serial;

		item.m_Durability = lpMsg->Dur;

		item.Convert(lpMsg->ItemIndex,lpMsg->Option1,lpMsg->Option2,lpMsg->Option3,lpMsg->NewOption,lpMsg->SetOption,lpMsg->JewelOfHarmonyOption,lpMsg->ItemOptionEx,lpMsg->SocketOption,lpMsg->SocketOptionBonus);

		item.m_IsPeriodicItem = ((lpMsg->Duration>0)?1:0);

		item.m_LoadPeriodicItem = ((lpMsg->Duration>0)?1:0);

		item.m_PeriodicItemTime = ((lpMsg->Duration>0)?lpMsg->Duration:0);

		BYTE slot = gEventInventory.EventInventoryInsertItem(lpObj->Index,item);

		if(slot != 0xFF){gEventInventory.GCEventItemModifySend(lpObj->Index,slot);}
	}
	else if(lpMsg->Map == 0xED)
	{
		CItem item;

		item.m_Level = lpMsg->Level;

		item.m_Serial = lpMsg->Serial;

		item.m_Durability = lpMsg->Dur;

		item.Convert(lpMsg->ItemIndex,lpMsg->Option1,lpMsg->Option2,lpMsg->Option3,lpMsg->NewOption,lpMsg->SetOption,lpMsg->JewelOfHarmonyOption,lpMsg->ItemOptionEx,lpMsg->SocketOption,lpMsg->SocketOptionBonus);

		item.m_IsPeriodicItem = ((lpMsg->Duration>0)?1:0);

		item.m_LoadPeriodicItem = ((lpMsg->Duration>0)?1:0);

		item.m_PeriodicItemTime = ((lpMsg->Duration>0)?lpMsg->Duration:0);

		BYTE slot = gMuunSystem.MuunInventoryInsertItem(lpObj->Index,item);

		if(slot != 0xFF){gMuunSystem.GCMuunItemModifySend(lpObj->Index,slot);}
	}
	else
	{
		if(gMap[lpMsg->Map].MonsterItemDrop(lpMsg->ItemIndex,lpMsg->Level,lpMsg->Dur,lpMsg->X,lpMsg->Y,lpMsg->Option1,lpMsg->Option2,lpMsg->Option3,lpMsg->NewOption,lpMsg->SetOption,lpMsg->LootIndex,lpMsg->Serial,lpMsg->JewelOfHarmonyOption,lpMsg->ItemOptionEx,lpMsg->SocketOption,lpMsg->SocketOptionBonus,lpMsg->Duration) != 0)
		{
			if(BC_MAP_RANGE(lpMsg->Map) != 0)
			{
				gBloodCastle.SetEventItemSerial(lpMsg->Map,lpMsg->ItemIndex,lpMsg->Serial);
			}

			#if(GAMESERVER_UPDATE>=301)

			if(IT_MAP_RANGE(lpMsg->Map) != 0)
			{
				gIllusionTemple.SetEventItemSerial(lpMsg->Map,lpMsg->ItemIndex,lpMsg->Serial);
			}

			#endif
		}
	}
}

void DGPetItemInfoRecv(SDHP_PET_ITEM_INFO_RECV* lpMsg) // OK
{
	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGPetItemInfoRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	if(lpMsg->type == 0)
	{
		for(int n=0;n < lpMsg->count;n++)
		{
			SDHP_PET_ITEM_INFO1* lpInfo = (SDHP_PET_ITEM_INFO1*)(((BYTE*)lpMsg)+sizeof(SDHP_PET_ITEM_INFO_RECV)+(sizeof(SDHP_PET_ITEM_INFO1)*n));

			if(INVENTORY_RANGE(lpInfo->slot) == 0)
			{
				continue;
			}

			if(lpObj->Inventory[lpInfo->slot].IsItem() == 0)
			{
				continue;
			}

			if(lpObj->Inventory[lpInfo->slot].m_Serial == lpInfo->serial)
			{
				lpObj->Inventory[lpInfo->slot].SetPetItemInfo(lpInfo->level,lpInfo->experience);

				if(INVENTORY_WEAR_RANGE(lpInfo->slot) != 0)
				{
					gObjectManager.CharacterCalcAttribute(lpObj->Index);
				}
			}
		}
	}
	
	if(lpMsg->type == 1)
	{
		for(int n=0;n < lpMsg->count;n++)
		{
			SDHP_PET_ITEM_INFO1* lpInfo = (SDHP_PET_ITEM_INFO1*)(((BYTE*)lpMsg)+sizeof(SDHP_PET_ITEM_INFO_RECV)+(sizeof(SDHP_PET_ITEM_INFO1)*n));

			if(WAREHOUSE_RANGE(lpInfo->slot) == 0)
			{
				continue;
			}

			if(lpObj->Warehouse[lpInfo->slot].IsItem() == 0)
			{
				continue;
			}

			if(lpObj->Warehouse[lpInfo->slot].m_Serial == lpInfo->serial)
			{
				lpObj->Warehouse[lpInfo->slot].SetPetItemInfo(lpInfo->level,lpInfo->experience);
			}
		}
	}
}

void DGOptionDataRecv(SDHP_OPTION_DATA_RECV* lpMsg) // OK
{
	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGOptionDataRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->index];

	#if(GAMESERVER_UPDATE>=701)

	lpObj->ChangeSkin = lpMsg->ChangeSkin;

	gObjViewportListProtocolCreate(lpObj);

	gObjectManager.CharacterUpdateMapEffect(lpObj);

	#endif

	PMSG_OPTION_DATA_SEND pMsg;

	pMsg.header.set(0xF3,0x30,sizeof(pMsg));

	memcpy(pMsg.SkillKey,lpMsg->SkillKey,sizeof(pMsg.SkillKey));

	pMsg.GameOption = lpMsg->GameOption;

	pMsg.QKey = lpMsg->QKey;

	pMsg.WKey = lpMsg->WKey;

	pMsg.EKey = lpMsg->EKey;

	pMsg.ChatWindow = lpMsg->ChatWindow;

	#if(GAMESERVER_UPDATE>=401)

	pMsg.RKey = lpMsg->RKey;

	pMsg.QWERLevel = lpMsg->QWERLevel;

	#endif

	DataSend(lpMsg->index,(BYTE*)&pMsg,pMsg.header.size);
}

void DGCharacterNameCheckRecv(SDHP_CHARACTER_NAME_CHECK_RECV* lpMsg) // OK
{
	#if(GAMESERVER_UPDATE>=401)

	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGCharacterNameCheckRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	PMSG_CHARACTER_NAME_CHECK_SEND pMsg;
	
	pMsg.header.set(0xF3,0x15,sizeof(pMsg));

	memcpy(pMsg.name,lpMsg->name,sizeof(pMsg.name));

	pMsg.result = lpMsg->result;

	DataSend(lpMsg->index,(BYTE*)&pMsg,pMsg.header.size);

	#endif
}

void DGCharacterNameChangeRecv(SDHP_CHARACTER_NAME_CHANGE_RECV* lpMsg) // OK
{
	#if(GAMESERVER_UPDATE>=401)

	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGCharacterNameChangeRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	PMSG_CHARACTER_NAME_CHANGE_SEND pMsg;

	pMsg.header.set(0xF3,0x16,sizeof(pMsg));

	memcpy(pMsg.OldName,lpMsg->OldName,sizeof(pMsg.OldName));

	memcpy(pMsg.NewName,lpMsg->NewName,sizeof(pMsg.NewName));

	pMsg.result = lpMsg->result;

	DataSend(lpMsg->index,(BYTE*)&pMsg,pMsg.header.size);

	#endif
}

void DGCrywolfSyncRecv(SDHP_CRYWOLF_SYNC_RECV* lpMsg) // OK
{
	#if(GAMESERVER_UPDATE>=201)

	if(gMapServerManager.GetMapServerGroup() != lpMsg->MapServerGroup)
	{
		return;
	}

	if(gCrywolfSync.GetCrywolfState() == lpMsg->CrywolfState && gCrywolfSync.GetOccupationState() == lpMsg->OccupationState)
	{
		return;
	}

	gCrywolfSync.SetCrywolfState(lpMsg->CrywolfState);

	gCrywolfSync.SetOccupationState(lpMsg->OccupationState);

	LogAdd(LOG_BLACK,"[Crywolf] Sync Occupation: (%d) State: (%d)",lpMsg->OccupationState,lpMsg->CrywolfState);
	
	#endif
}

void DGCrywolfInfoRecv(SDHP_CRYWOLF_INFO_RECV* lpMsg) // OK
{
	#if(GAMESERVER_UPDATE>=201)

	if(gMapServerManager.GetMapServerGroup() != lpMsg->MapServerGroup)
	{
		return;
	}

	if(lpMsg->OccupationState == 0 || lpMsg->OccupationState == 1 || lpMsg->OccupationState == 2)
	{
		gCrywolf.ApplyCrywolfDBInfo(lpMsg->OccupationState);
	}
	else
	{
		LogAdd(LOG_RED,"[Crywolf] Incorrect DB OccupationState!!!");
	}

	#endif
}

void DGGlobalPostRecv(SDHP_GLOBAL_POST_RECV* lpMsg) // OK
{
	if(gMapServerManager.GetMapServerGroup() != lpMsg->MapServerGroup)
	{
		return;
	}

	switch(lpMsg->type)
	{
		case 0:
			PostMessage1(lpMsg->name,gMessage.GetMessage(323),lpMsg->message);
			break;
		case 1:
			PostMessage2(lpMsg->name,gMessage.GetMessage(323),lpMsg->message);
			break;
		case 2:
			PostMessage3(lpMsg->name,gMessage.GetMessage(323),lpMsg->message);
			break;
		case 3:
			PostMessage4(lpMsg->name,gMessage.GetMessage(323),lpMsg->message);
			break;
	}
}

void DGGlobalNoticeRecv(SDHP_GLOBAL_NOTICE_RECV* lpMsg) // OK
{
	if(gMapServerManager.GetMapServerGroup() != lpMsg->MapServerGroup)
	{
		return;
	}

	gNotice.GCNoticeSendToAll(lpMsg->type,lpMsg->count,lpMsg->opacity,lpMsg->delay,lpMsg->color,lpMsg->speed,"%s",lpMsg->message);
}

void DGSNSDataRecv(SDHP_SNS_DATA_RECV* lpMsg) // OK
{
	#if(GAMESERVER_UPDATE>=801)

	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGSNSDataRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	PMSG_SNS_DATA_SEND pMsg;

	pMsg.header.set(0xFB,sizeof(pMsg));

	pMsg.result = lpMsg->result;

	memcpy(pMsg.data,lpMsg->data,sizeof(pMsg.data));

	DataSend(lpMsg->index,(BYTE*)&pMsg,sizeof(pMsg));

	#endif
}

void DGGlobalWhisperRecv(SDHP_GLOBAL_WHISPER_RECV* lpMsg) // OK
{
	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGGlobalWhisperRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	if(lpMsg->result == 0)
	{
		GCServerMsgSend(lpMsg->index,0);
		return;
	}

	lpMsg->message[(sizeof(lpMsg->message)-1)] = ((strlen(lpMsg->message)>(sizeof(lpMsg->message)-1))?0:lpMsg->message[(sizeof(lpMsg->message)-1)]);

	gLog.Output(LOG_CHAT,"[Whisper][%s][%s] - (Name: %s,Message: %s)",lpMsg->account,lpMsg->name,lpMsg->TargetName,lpMsg->message);
}

void DGGlobalWhisperEchoRecv(SDHP_GLOBAL_WHISPER_ECHO_RECV* lpMsg) // OK
{
	if(gObjIsAccountValid(lpMsg->index,lpMsg->account) == 0)
	{
		LogAdd(LOG_RED,"[DGGlobalWhisperEchoRecv] Invalid Account [%d](%s)",lpMsg->index,lpMsg->account);
		CloseClient(lpMsg->index);
		return;
	}

	lpMsg->message[(sizeof(lpMsg->message)-1)] = ((strlen(lpMsg->message)>(sizeof(lpMsg->message)-1))?0:lpMsg->message[(sizeof(lpMsg->message)-1)]);

	if(gFilter.CheckSyntax(lpMsg->message) != 0)
	{
		gNotice.GCNoticeSend(lpMsg->index,1,0,0,0,0,0,gMessage.GetMessage(723));
		return;
	}

	GCChatWhisperSend(lpMsg->index,lpMsg->SourceName,lpMsg->message);
}

void GDServerInfoSend() // OK
{
	SDHP_DATA_SERVER_INFO_SEND pMsg;

	pMsg.header.set(0x00,sizeof(pMsg));

	pMsg.type = 1;

	pMsg.ServerPort = (WORD)gServerInfo.m_ServerPort;

	strcpy_s(pMsg.ServerName,gServerInfo.m_ServerName);

	pMsg.ServerCode = (WORD)gServerInfo.m_ServerCode;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,-1);
}

void GDCharacterListSend(int aIndex) // OK
{
	if(gObjIsAccountValid(aIndex,gObj[aIndex].Account) == 0)
	{
		return;
	}

	SDHP_CHARACTER_LIST_SEND pMsg;

	pMsg.header.set(0x01,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);
}

void GDCharacterCreateSend(int aIndex,char* name,BYTE Class) // OK
{
	if(gObjIsAccountValid(aIndex,gObj[aIndex].Account) == 0)
	{
		return;
	}

	SDHP_CHARACTER_CREATE_SEND pMsg;

	pMsg.header.set(0x02,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

	memcpy(pMsg.name,name,sizeof(pMsg.name));

	pMsg.Class = Class;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);
}

void GDCharacterDeleteSend(int aIndex,char* name) // OK
{
	if(gObjIsAccountValid(aIndex,gObj[aIndex].Account) == 0)
	{
		return;
	}

	SDHP_CHARACTER_DELETE_SEND pMsg;

	pMsg.header.set(0x03,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

	memcpy(pMsg.name,name,sizeof(pMsg.name));

	pMsg.guild = 0;

	if(gObj[aIndex].Guild != 0)
	{
		if(strcmp(gObj[aIndex].Name,gObj[aIndex].Guild->Master) == 0)
		{
			pMsg.guild = 1; // Guild Master
		}
		else
		{
			pMsg.guild = 2; // Guild Member
		}

		memcpy(pMsg.GuildName,gObj[aIndex].Guild->Name,sizeof(pMsg.GuildName));
	}

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);
}

void GDCharacterInfoSend(int aIndex,char* name) // OK
{
	if(gObjIsAccountValid(aIndex,gObj[aIndex].Account) == 0)
	{
		return;
	}

	SDHP_CHARACTER_INFO_SEND pMsg;

	pMsg.header.set(0x04,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

	memcpy(pMsg.name,name,sizeof(pMsg.name));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);
}

void GDCreateItemSend(int aIndex,BYTE map,BYTE x,BYTE y,int index,BYTE level,BYTE dur,BYTE Option1,BYTE Option2,BYTE Option3,int LootIndex,BYTE NewOption,BYTE SetOption,BYTE JewelOfHarmonyOption,BYTE ItemOptionEx,BYTE SocketOption[MAX_SOCKET_OPTION],BYTE SocketOptionBonus,DWORD duration) // OK
{
	if(gObjIsAccountValid(aIndex,gObj[aIndex].Account) == 0)
	{
		return;
	}
	
	if(index == GET_ITEM(12,65) || index == GET_ITEM(12,105) || index == GET_ITEM(12,111) || index == GET_ITEM(12,117) || index == GET_ITEM(12,123) || index == GET_ITEM(12,129))
	{
		level = 2;
	}

	SDHP_CREATE_ITEM_SEND pMsg;

	pMsg.header.set(0x07,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

	pMsg.X = x;
	pMsg.Y = y;
	pMsg.Map = ((map==0xEB)?((gEventInventory.IsEventItem(index)==0)?((gMuunSystem.IsMuunItem(index)==0)?((gMuunSystem.IsMuunUtil(index)==0)?map:0xED):0xED):0xEC):map);
	pMsg.ItemIndex = index;
	pMsg.Level = level;
	pMsg.Dur = ((dur==0)?gItemManager.GetItemDurability(index,level,NewOption,SetOption):dur);
	pMsg.Option1 = ((gItemManager.GetItemSkill(index) == 0) ? 0 : Option1);
	pMsg.Option2 = Option2;
	pMsg.Option3 = Option3;
	pMsg.NewOption = NewOption;
	pMsg.LootIndex = LootIndex;
	pMsg.SetOption = SetOption;
	pMsg.JewelOfHarmonyOption = JewelOfHarmonyOption;
	pMsg.ItemOptionEx = ItemOptionEx;
	pMsg.SocketOptionBonus = SocketOptionBonus;
	pMsg.Duration = duration;

	if(SocketOption == 0)
	{
		memset(pMsg.SocketOption,0xFF,MAX_SOCKET_OPTION);
	}
	else
	{
		memcpy(pMsg.SocketOption,SocketOption,MAX_SOCKET_OPTION);
	}

	#if(GAMESERVER_UPDATE>=701)

	if(gPentagramSystem.IsPentagramItem(index) != 0 || gPentagramSystem.IsPentagramJewel(index) != 0 || gPentagramSystem.IsPentagramMithril(index) != 0)
	{
		pMsg.SocketOptionBonus = ((SocketOptionBonus==0xFF)?((gPentagramSystem.IsPentagramMithril(index)==0)?(((GetLargeRand()%5)+1) | 16):((GetLargeRand()%5)+1)):SocketOptionBonus);
	}
	else
	{
		pMsg.SocketOptionBonus = SocketOptionBonus;
	}

	#endif

	#if(GAMESERVER_UPDATE>=803)

	gMuunSystem.GetMuunItemSocketOption(index,&pMsg.Level,&pMsg.SocketOption[0],&pMsg.SocketOptionBonus);

	gMuunSystem.GetMuunUtilSocketOption(index,&pMsg.Level,&pMsg.SocketOption[0],&pMsg.SocketOptionBonus);

	#endif

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);
}

void GDOptionDataSend(int aIndex) // OK
{
	if(gObjIsAccountValid(aIndex,gObj[aIndex].Account) == 0)
	{
		return;
	}

	SDHP_OPTION_DATA_SEND pMsg;

	pMsg.header.set(0x08,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

	memcpy(pMsg.name,gObj[aIndex].Name,sizeof(pMsg.name));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);
}

void GDPetItemInfoSend(int aIndex,int type) // OK
{
	if(gObjIsAccountValid(aIndex,gObj[aIndex].Account) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	BYTE send[4096];

	SDHP_PET_ITEM_INFO_SEND pMsg;

	pMsg.header.set(0x09,0);

	int size = sizeof(pMsg);

	pMsg.index = aIndex;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	pMsg.type = type;

	pMsg.count = 0;

	SDHP_PET_ITEM_INFO2 info;

	if(type == 0)
	{
		for(int n=0;n < INVENTORY_SIZE;n++)
		{
			if(lpObj->Inventory[n].IsItem() == 0)
			{
				continue;
			}

			if(lpObj->Inventory[n].m_Index == GET_ITEM(13,4) || lpObj->Inventory[n].m_Index == GET_ITEM(13,5)) // Dark Horse,Dark Spirit
			{
				info.slot = n;

				info.serial = lpObj->Inventory[n].m_Serial;

				memcpy(&send[size],&info,sizeof(info));
				size += sizeof(info);

				pMsg.count++;
			}
		}
	}
	else if(type == 1)
	{
		for(int n=0;n < WAREHOUSE_SIZE;n++)
		{
			if(lpObj->Warehouse[n].IsItem() == 0)
			{
				continue;
			}

			if(lpObj->Warehouse[n].m_Index == GET_ITEM(13,4) || lpObj->Warehouse[n].m_Index == GET_ITEM(13,5)) // Dark Horse,Dark Spirit
			{
				info.slot = n;

				info.serial = lpObj->Warehouse[n].m_Serial;

				memcpy(&send[size],&info,sizeof(info));
				size += sizeof(info);

				pMsg.count++;
			}
		}
	}

	if(pMsg.count > 0)
	{
		pMsg.header.size[0] = SET_NUMBERHB(size);
		pMsg.header.size[1] = SET_NUMBERLB(size);

		memcpy(send,&pMsg,sizeof(pMsg));

		gDataServerConnection.DataSend(send,size,aIndex);
	}
}

void GDCharacterNameCheckSend(int aIndex,char* name) // OK
{
	#if(GAMESERVER_UPDATE>=401)

	if(gObjIsAccountValid(aIndex,gObj[aIndex].Account) == 0)
	{
		return;
	}

	SDHP_CHARACTER_NAME_CHECK_SEND pMsg;

	pMsg.header.set(0x0A,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

	memcpy(pMsg.name,name,sizeof(pMsg.name));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);

	#endif
}

void GDCharacterNameChangeSend(int aIndex,char* OldName,char* NewName) // OK
{
	#if(GAMESERVER_UPDATE>=401)

	if(gObjIsAccountValid(aIndex,gObj[aIndex].Account) == 0)
	{
		return;
	}

	SDHP_CHARACTER_NAME_CHANGE_SEND pMsg;

	pMsg.header.set(0x0B,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

	memcpy(pMsg.OldName,OldName,sizeof(pMsg.OldName));

	memcpy(pMsg.NewName,NewName,sizeof(pMsg.NewName));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);

	#endif
}

void GDCrywolfSyncSend(int MapServerGroup,int CrywolfState,int OccupationState) // OK
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	#if(GAMESERVER_UPDATE>=201)

	SDHP_CRYWOLF_SYNC_SEND pMsg;

	pMsg.header.set(0x1E,sizeof(pMsg));

	pMsg.MapServerGroup = MapServerGroup;

	pMsg.CrywolfState = CrywolfState;

	pMsg.OccupationState = OccupationState;

	gDataServerConnection.DataSend((BYTE*)&pMsg,sizeof(pMsg),-1);

	#endif
}

void GDCrywolfInfoSend(int MapServerGroup) // OK
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	#if(GAMESERVER_UPDATE>=201)

	SDHP_CRYWOLF_INFO_SEND pMsg;

	pMsg.header.set(0x1F,sizeof(pMsg));

	pMsg.MapServerGroup = MapServerGroup;

	gDataServerConnection.DataSend((BYTE*)&pMsg,sizeof(pMsg),-1);

	#endif
}

void GDGlobalPostSend(int MapServerGroup,BYTE type,char* name,char* message) // OK
{
	SDHP_GLOBAL_POST_SEND pMsg;

	pMsg.header.set(0x20,sizeof(pMsg));

	pMsg.MapServerGroup = MapServerGroup;

	pMsg.type = type;

	memcpy(pMsg.name,name,sizeof(pMsg.name));

	memcpy(pMsg.message,message,sizeof(pMsg.message));

	gDataServerConnection.DataSend((BYTE*)&pMsg,sizeof(pMsg),-1);
}

void GDGlobalNoticeSend(int MapServerGroup,BYTE type,BYTE count,BYTE opacity,WORD delay,DWORD color,BYTE speed,char* message) // OK
{
	SDHP_GLOBAL_NOTICE_SEND pMsg;

	pMsg.header.set(0x21,sizeof(pMsg));

	pMsg.MapServerGroup = MapServerGroup;

	pMsg.type = type;

	pMsg.count = count;

	pMsg.opacity = opacity;

	pMsg.delay = delay;

	pMsg.color = color;

	pMsg.speed = speed;

	memcpy(pMsg.message,message,sizeof(pMsg.message));

	gDataServerConnection.DataSend((BYTE*)&pMsg,sizeof(pMsg),-1);
}

void GDSNSDataSend(int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=801)

	if(gObjIsAccountValid(aIndex,gObj[aIndex].Account) == 0)
	{
		return;
	}

	SDHP_SNS_DATA_SEND pMsg;

	pMsg.header.set(0x24,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

	memcpy(pMsg.name,gObj[aIndex].Name,sizeof(pMsg.name));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);

	#endif
}

void GDCharacterInfoSaveSend(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->MapServerMoveQuit != 0)
	{
		return;
	}

	SDHP_CHARACTER_INFO_SAVE_SEND pMsg;

	pMsg.header.set(0x30,sizeof(pMsg));

	pMsg.index = lpObj->Index;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpObj->Name,sizeof(pMsg.name));

	pMsg.Level = lpObj->Level;
	pMsg.Class = lpObj->DBClass;
	pMsg.LevelUpPoint = lpObj->LevelUpPoint;
	pMsg.Experience = lpObj->Experience;
	pMsg.Money = lpObj->Money;
	pMsg.Strength = lpObj->Strength;
	pMsg.Dexterity = lpObj->Dexterity;
	pMsg.Vitality = lpObj->Vitality;
	pMsg.Energy = lpObj->Energy;
	pMsg.Leadership = lpObj->Leadership;
	pMsg.Life = (DWORD)lpObj->Life;
	pMsg.MaxLife = (DWORD)lpObj->MaxLife;
	pMsg.Mana = (DWORD)lpObj->Mana;
	pMsg.MaxMana = (DWORD)lpObj->MaxMana;
	pMsg.BP = (DWORD)lpObj->BP;
	pMsg.MaxBP = (DWORD)lpObj->MaxBP;
	#if(GAMESERVER_UPDATE>=201)
	pMsg.Shield = (DWORD)lpObj->Shield;
	pMsg.MaxShield = (DWORD)lpObj->MaxShield;
	#endif

	for(int n=0;n < INVENTORY_SIZE;n++){gItemManager.DBItemByteConvert(pMsg.Inventory[n],&lpObj->Inventory[n]);}

	for(int n=0;n < MAX_SKILL_LIST;n++){gSkillManager.SkillByteConvert(pMsg.Skill[n],&lpObj->Skill[n]);}

	pMsg.Map = lpObj->Map;
	pMsg.X = (BYTE)lpObj->X;
	pMsg.Y = (BYTE)lpObj->Y;
	pMsg.Dir = lpObj->Dir;
	pMsg.PKCount = lpObj->PKCount;
	pMsg.PKLevel = lpObj->PKLevel;
	pMsg.PKTime = lpObj->PKTime;

	memcpy(pMsg.Quest,lpObj->Quest,sizeof(pMsg.Quest));

	pMsg.ChatLimitTime = lpObj->ChatLimitTime;

	pMsg.FruitAddPoint = lpObj->FruitAddPoint;
	pMsg.FruitSubPoint = lpObj->FruitSubPoint;

	for(int n=0;n < MAX_EFFECT_LIST;n++){gEffectManager.EffectByteConvert(pMsg.Effect[n],&lpObj->Effect[n]);}

	#if(GAMESERVER_UPDATE>=602)
	pMsg.ExtInventory = lpObj->ExtInventory;
	pMsg.ExtWarehouse = lpObj->ExtWarehouse;
	#endif

	pMsg.Married = lpObj->Married;
	memcpy(pMsg.MarryName,lpObj->MarryName,sizeof(pMsg.MarryName));

	pMsg.BCCount = lpObj->BCCount;
	pMsg.CCCount = lpObj->CCCount;
	pMsg.DSCount = lpObj->DSCount;
	pMsg.DGCount = lpObj->DGCount;
	pMsg.ITCount = lpObj->ITCount;
	pMsg.IGCount = lpObj->IGCount;

	gDataServerConnection.DataSend((BYTE*)&pMsg,sizeof(pMsg),aIndex);

	GDPetItemInfoSaveSend(aIndex,0);

	gWarehouse.GDWarehouseItemSaveSend(aIndex);

	#if(GAMESERVER_UPDATE>=301)

	gQuest.GDQuestKillCountSaveSend(aIndex);
	
	#endif

	#if(GAMESERVER_UPDATE>=401)

	gMasterSkillTree.GDMasterSkillTreeSaveSend(aIndex);

	#endif

	#if(GAMESERVER_UPDATE>=501)

	gQuestWorld.GDQuestWorldSaveSend(aIndex);

	#endif

	#if(GAMESERVER_SHOP==3)

	gCashShop.GDCashShopPeriodicItemSaveSend(aIndex);

	#endif

	#if(GAMESERVER_UPDATE>=602)

	gLuckyItem.GDLuckyItemSaveSend(aIndex);

	#endif

	#if(GAMESERVER_UPDATE>=701)

	gPentagramSystem.GDPentagramJewelInfoSaveSend(aIndex,PENTAGRAM_JEWEL_TYPE_INVENTORY);

	gPentagramSystem.GDPentagramJewelInfoSaveSend(aIndex,PENTAGRAM_JEWEL_TYPE_WAREHOUSE);

	#endif

	#if(GAMESERVER_UPDATE>=802)

	gPersonalShop.GDPShopItemValueSaveSend(aIndex);

	gEventInventory.GDEventInventorySaveSend(aIndex);

	gMuRummy.GDReqMuRummyInfoUpdate(lpObj);

	#endif

	#if(GAMESERVER_UPDATE>=803)

	gMuunSystem.GDMuunInventorySaveSend(aIndex);

	#endif

	gCustomPick.GDCustomPickSaveSend(aIndex);
}

void GDInventoryItemSaveSend(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	SDHP_INVENTORY_ITEM_SAVE_SEND pMsg;

	pMsg.header.set(0x31,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpObj->Name,sizeof(pMsg.name));

	for(int n=0;n < INVENTORY_SIZE;n++){gItemManager.DBItemByteConvert(pMsg.Inventory[n],&lpObj->Inventory[n]);}

	gDataServerConnection.DataSend((BYTE*)&pMsg,sizeof(pMsg),aIndex);
}

void GDOptionDataSaveSend(int aIndex,BYTE* SkillKey,BYTE GameOption,BYTE QKey,BYTE WKey,BYTE EKey,BYTE ChatWindow,BYTE RKey,DWORD QWERLevel) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	SDHP_OPTION_DATA_SAVE_SEND pMsg;

	pMsg.header.set(0x33,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpObj->Name,sizeof(pMsg.name));

	memcpy(pMsg.SkillKey,SkillKey,sizeof(pMsg.SkillKey));

	pMsg.GameOption = GameOption;

	pMsg.QKey = QKey;

	pMsg.WKey = WKey;

	pMsg.EKey = EKey;

	pMsg.ChatWindow = ChatWindow;

	#if(GAMESERVER_UPDATE>=401)

	pMsg.RKey = RKey;

	pMsg.QWERLevel = QWERLevel;

	#endif

	#if(GAMESERVER_UPDATE>=701)

	pMsg.ChangeSkin = lpObj->ChangeSkin;

	#endif

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);
}

void GDPetItemInfoSaveSend(int aIndex,int type) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	BYTE send[4096];

	SDHP_PET_ITEM_INFO_SAVE_SEND pMsg;

	pMsg.header.set(0x34,0);

	int size = sizeof(pMsg);

	pMsg.index = aIndex;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	pMsg.count = 0;

	SDHP_PET_ITEM_INFO_SAVE info;

	if(type == 0)
	{
		for(int n=0;n < INVENTORY_SIZE;n++)
		{
			if(lpObj->Inventory[n].IsItem() == 0)
			{
				continue;
			}

			if(lpObj->Inventory[n].m_Index == GET_ITEM(13,4) || lpObj->Inventory[n].m_Index == GET_ITEM(13,5)) // Dark Horse,Dark Spirit
			{
				if(lpObj->Inventory[n].m_LoadPetItemInfo != 0)
				{
					info.serial = lpObj->Inventory[n].m_Serial;

					info.level = lpObj->Inventory[n].m_PetItemLevel;

					info.experience = lpObj->Inventory[n].m_PetItemExp;

					memcpy(&send[size],&info,sizeof(info));
					size += sizeof(info);

					pMsg.count++;
				}
			}
		}
	}
	else if(type == 1)
	{
		for(int n=0;n < WAREHOUSE_SIZE;n++)
		{
			if(lpObj->Warehouse[n].IsItem() == 0)
			{
				continue;
			}

			if(lpObj->Warehouse[n].m_Index == GET_ITEM(13,4) || lpObj->Warehouse[n].m_Index == GET_ITEM(13,5)) // Dark Horse,Dark Spirit
			{
				if(lpObj->Warehouse[n].m_LoadPetItemInfo != 0)
				{
					info.serial = lpObj->Warehouse[n].m_Serial;

					info.level = lpObj->Warehouse[n].m_PetItemLevel;

					info.experience = lpObj->Warehouse[n].m_PetItemExp;

					memcpy(&send[size],&info,sizeof(info));
					size += sizeof(info);

					pMsg.count++;
				}
			}
		}
	}

	if(pMsg.count > 0)
	{
		pMsg.header.size[0] = SET_NUMBERHB(size);
		pMsg.header.size[1] = SET_NUMBERLB(size);

		memcpy(send,&pMsg,sizeof(pMsg));

		gDataServerConnection.DataSend(send,size,aIndex);
	}
}

void GDResetInfoSaveSend(int aIndex,int ResetDay,int ResetWek,int ResetMon) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	SDHP_RESET_INFO_SAVE_SEND pMsg;

	pMsg.header.set(0x39,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpObj->Name,sizeof(pMsg.name));

	pMsg.Reset = lpObj->Reset;

	pMsg.ResetDay = ResetDay;

	pMsg.ResetWek = ResetWek;

	pMsg.ResetMon = ResetMon;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);
}

void GDMasterResetInfoSaveSend(int aIndex,int MasterResetDay,int MasterResetWek,int MasterResetMon) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	SDHP_MASTER_RESET_INFO_SAVE_SEND pMsg;

	pMsg.header.set(0x3A,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpObj->Name,sizeof(pMsg.name));

	pMsg.Reset = lpObj->Reset;

	pMsg.MasterReset = lpObj->MasterReset;

	pMsg.MasterResetDay = MasterResetDay;

	pMsg.MasterResetWek = MasterResetWek;

	pMsg.MasterResetMon = MasterResetMon;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);
}

void GDRankingDuelSaveSend(int aIndex,DWORD WinScore,DWORD LoseScore) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	SDHP_RANKING_DUEL_SAVE_SEND pMsg;

	pMsg.header.set(0x3C,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpObj->Name,sizeof(pMsg.name));

	pMsg.WinScore = WinScore;

	pMsg.LoseScore = LoseScore;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);
}

void GDRankingBloodCastleSaveSend(int aIndex,DWORD Score) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	SDHP_RANKING_BLOOD_CASTLE_SAVE_SEND pMsg;

	pMsg.header.set(0x3D,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpObj->Name,sizeof(pMsg.name));

	pMsg.score = Score;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);
}

void GDRankingChaosCastleSaveSend(int aIndex,DWORD Score) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	SDHP_RANKING_CHAOS_CASTLE_SAVE_SEND pMsg;

	pMsg.header.set(0x3E,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpObj->Name,sizeof(pMsg.name));

	pMsg.score = Score;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);
}

void GDRankingDevilSquareSaveSend(int aIndex,DWORD Score) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	SDHP_RANKING_DEVIL_SQUARE_SAVE_SEND pMsg;

	pMsg.header.set(0x3F,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpObj->Name,sizeof(pMsg.name));

	pMsg.score = Score;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);
}

void GDRankingIllusionTempleSaveSend(int aIndex,DWORD Score) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	SDHP_RANKING_ILLUSION_TEMPLE_SAVE_SEND pMsg;

	pMsg.header.set(0x40,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,lpObj->Account,sizeof(pMsg.account));

	memcpy(pMsg.name,lpObj->Name,sizeof(pMsg.name));

	pMsg.score = Score;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);
}

void GDCreationCardSaveSend(int aIndex,BYTE ExtClass) // OK
{
	SDHP_CREATION_CARD_SAVE_SEND pMsg;

	pMsg.header.set(0x42,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

	pMsg.ExtClass = ExtClass;

	gDataServerConnection.DataSend((BYTE*)&pMsg,sizeof(pMsg),aIndex);
}

void GDCrywolfInfoSaveSend(int MapServerGroup,int CrywolfState,int OccupationState) // OK
{
	#if(GAMESERVER_UPDATE>=201)

	SDHP_CRYWOLF_INFO_SAVE_SEND pMsg;

	pMsg.header.set(0x49,sizeof(pMsg));

	pMsg.MapServerGroup = MapServerGroup;

	pMsg.CrywolfState = CrywolfState;

	pMsg.OccupationState = OccupationState;

	gDataServerConnection.DataSend((BYTE*)&pMsg,sizeof(pMsg),-1);

	#endif
}

void GDSNSDataSaveSend(int aIndex,BYTE* data) // OK
{
	#if(GAMESERVER_UPDATE>=801)

	SDHP_SNS_DATA_SAVE_SEND pMsg;

	pMsg.header.set(0x4E,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

	memcpy(pMsg.name,gObj[aIndex].Name,sizeof(pMsg.account));

	memcpy(pMsg.data,data,sizeof(pMsg.data));

	gDataServerConnection.DataSend((BYTE*)&pMsg,sizeof(pMsg),aIndex);

	#endif
}

void GDConnectCharacterSend(int aIndex) // OK
{
	SDHP_CONNECT_CHARACTER_SEND pMsg;

	pMsg.header.set(0x70,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

	memcpy(pMsg.name,gObj[aIndex].Name,sizeof(pMsg.name));

	gDataServerConnection.DataSend((BYTE*)&pMsg,sizeof(pMsg),aIndex);
}

void GDDisconnectCharacterSend(int aIndex) // OK
{
	SDHP_DISCONNECT_CHARACTER_SEND pMsg;

	pMsg.header.set(0x71,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

	memcpy(pMsg.name,gObj[aIndex].Name,sizeof(pMsg.name));

	gDataServerConnection.DataSend((BYTE*)&pMsg,sizeof(pMsg),aIndex);
}

void GDGlobalWhisperSend(int aIndex,char* TargetName,char* message) // OK
{
	SDHP_GLOBAL_WHISPER_SEND pMsg;

	pMsg.header.set(0x72,sizeof(pMsg));

	pMsg.index = aIndex;

	memcpy(pMsg.account,gObj[aIndex].Account,sizeof(pMsg.account));

	memcpy(pMsg.name,gObj[aIndex].Name,sizeof(pMsg.name));

	memcpy(pMsg.TargetName,TargetName,sizeof(pMsg.TargetName));

	memcpy(pMsg.message,message,sizeof(pMsg.message));

	gDataServerConnection.DataSend((BYTE*)&pMsg,sizeof(pMsg),aIndex);
}

//**************************************************************************//
// RAW FUNCTIONS ***********************************************************//
//**************************************************************************//

void GS_GDReqCastleTotalInfo(int iMapSvrGroup,int iCastleEventCycle)
{
	if( iMapSvrGroup < 0 )
	{
		return;
	}

	CSP_REQ_CASTLEDATA pMsg;

	pMsg.h.set(0x80,0x00,sizeof(CSP_REQ_CASTLEDATA));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iCastleEventCycle = iCastleEventCycle;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.h.size,-1);
}

void GS_GDReqOwnerGuildMaster(int iMapSvrGroup,int aIndex)
{
	if( iMapSvrGroup < 0 )
	{
		return;
	}

	if( OBJECT_RANGE(aIndex) == FALSE )
	{
		return;
	}


	CSP_REQ_OWNERGUILDMASTER pMsg;

	pMsg.h.set(0x80,0x01,sizeof(CSP_REQ_OWNERGUILDMASTER));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iIndex = aIndex;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.h.size,aIndex);
}

void GS_GDReqCastleNpcBuy(int iMapSvrGroup,int aIndex,int iNpcNumber,int iNpcIndex,int iNpcDfLevel,int iNpcRgLevel,int iNpcMaxHP,int iNpcHP,BYTE btNpcX,BYTE btNpcY,BYTE btNpcDIR,int iBuyCost)
{
	if( iMapSvrGroup < 0 )
	{
		return;
	}

	if( OBJECT_RANGE(aIndex) == FALSE )
	{
		return;
	}


	CSP_REQ_NPCBUY pMsg;

	pMsg.h.set(0x80,0x03,sizeof(CSP_REQ_NPCBUY));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iIndex = aIndex;
	pMsg.iNpcNumber = iNpcNumber;
	pMsg.iNpcIndex = iNpcIndex;
	pMsg.iNpcDfLevel = iNpcDfLevel;
	pMsg.iNpcRgLevel = iNpcRgLevel;
	pMsg.iNpcMaxHp = iNpcMaxHP;
	pMsg.iNpcHp = iNpcHP;
	pMsg.btNpcX = btNpcX;
	pMsg.btNpcY = btNpcY;
	pMsg.btNpcDIR = btNpcDIR;
	pMsg.iBuyCost = iBuyCost;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.h.size,aIndex);
}

void GS_GDReqCastleNpcRepair(int iMapSvrGroup,int aIndex,int iNpcNumber,int iNpcIndex,int iRepairCost)
{
	if( iMapSvrGroup < 0 )
	{
		return;
	}

	if( OBJECT_RANGE(aIndex) == FALSE )
	{
		return;
	}


	CSP_REQ_NPCREPAIR pMsg;

	pMsg.h.set(0x80,0x04,sizeof(CSP_REQ_NPCREPAIR));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iIndex = aIndex;
	pMsg.iNpcNumber = iNpcNumber;
	pMsg.iNpcIndex = iNpcIndex;
	pMsg.iRepairCost = iRepairCost;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.h.size,aIndex);
}

void GS_GDReqCastleNpcUpgrade(int iMapSvrGroup,int aIndex,int iNpcNumber,int iNpcIndex,int iNpcUpType,int iNpcUpValue,int iNpcUpIndex)
{
	if( iMapSvrGroup < 0 )
	{
		return;
	}

	if( OBJECT_RANGE(aIndex) == FALSE )
	{
		return;
	}


	CSP_REQ_NPCUPGRADE pMsg;

	pMsg.h.set(0x80,0x05,sizeof(CSP_REQ_NPCUPGRADE));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iIndex = aIndex;
	pMsg.iNpcNumber = iNpcNumber;
	pMsg.iNpcIndex = iNpcIndex;
	pMsg.iNpcUpType = iNpcUpType;
	pMsg.iNpcUpValue = iNpcUpValue;
	pMsg.iNpcUpIndex = iNpcUpIndex;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.h.size,aIndex);
}

void GS_GDReqTaxInfo(int iMapSvrGroup,int aIndex)
{
	if( iMapSvrGroup < 0 )
	{
		return;
	}

	if( OBJECT_RANGE(aIndex) == FALSE )
	{
		return;
	}


	CSP_REQ_TAXINFO pMsg;

	pMsg.h.set(0x80,0x06,sizeof(CSP_REQ_TAXINFO));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iIndex = aIndex;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.h.size,aIndex);
}

void GS_GDReqTaxRateChange( int iMapSvrGroup,int aIndex,int iTaxType,int iTaxRate)
{
	if( iMapSvrGroup < 0 )
	{
		return;
	}

	if( OBJECT_RANGE(aIndex) == FALSE )
	{
		return;
	}


	CSP_REQ_TAXRATECHANGE pMsg;

	pMsg.h.set(0x80,0x07,sizeof(CSP_REQ_TAXRATECHANGE));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iIndex = aIndex;
	pMsg.iTaxRate = iTaxRate; 
	pMsg.iTaxKind = iTaxType;
	
	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.h.size,aIndex);
}

void GS_GDReqCastleMoneyChange(int iMapSvrGroup,int aIndex,int iMoneyChange)
{
	if( iMapSvrGroup < 0 )
	{
		return;
	}

	if( OBJECT_RANGE(aIndex) == FALSE )
	{
		return;
	}


	CSP_REQ_MONEYCHANGE pMsg;

	pMsg.h.set(0x80,0x08,sizeof(CSP_REQ_MONEYCHANGE));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iIndex = aIndex;
	pMsg.iMoneyChanged = iMoneyChange;
	
	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.h.size,aIndex);
}

void GS_GDReqSiegeDateChange(int iMapSvrGroup,int aIndex,WORD wStartYear,BYTE btStartMonth,BYTE btStartDay,WORD wEndYear,BYTE btEndMonth,BYTE btEndDay)
{
	if( iMapSvrGroup < 0 )
	{
		return;
	}

	CSP_REQ_SDEDCHANGE pMsg;

	pMsg.h.set(0x80,0x09,sizeof(CSP_REQ_SDEDCHANGE));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iIndex = aIndex;
	pMsg.wStartYear = wStartYear;
	pMsg.btStartMonth = btStartMonth;
	pMsg.btStartDay = btStartDay;
	pMsg.wEndYear = wEndYear;
	pMsg.btEndMonth = btEndMonth;
	pMsg.btEndDay = btEndDay;
	
	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.h.size,aIndex);
}

void GS_GDReqGuildMarkRegInfo(int iMapSvrGroup,int aIndex)
{
	if( iMapSvrGroup < 0 )
	{
		return;
	}

	if( OBJECT_RANGE(aIndex) == FALSE )
	{
		return;
	}


	CSP_REQ_GUILDREGINFO pMsg;

	pMsg.h.set(0x80,0x0A,sizeof(CSP_REQ_GUILDREGINFO));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iIndex = aIndex;
	memcpy(pMsg.szGuildName,gObj[aIndex].GuildName,8);
	
	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.h.size,aIndex);
}

void GS_GDReqSiegeEndedChange(int iMapSvrGroup,int bIsSiegeEnded)
{
	if( iMapSvrGroup < 0 )
	{
		return;
	}


	CSP_REQ_SIEGEENDCHANGE pMsg;

	pMsg.h.set(0x80,0x0B,sizeof(CSP_REQ_SIEGEENDCHANGE));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.bIsSiegeEnded = bIsSiegeEnded;
	
	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.h.size,-1);
}

void GS_GDReqCastleOwnerChange(int iMapSvrGroup,int bIsCastleOccupied,char* lpszGuildName)
{
	if( iMapSvrGroup < 0 )
	{
		return;
	}

	if( lpszGuildName == NULL )
	{
		return;
	}


	CSP_REQ_CASTLEOWNERCHANGE pMsg;

	pMsg.h.set(0x80,0x0C,sizeof(CSP_REQ_CASTLEOWNERCHANGE));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.bIsCastleOccupied = bIsCastleOccupied;
	memcpy(pMsg.szOwnerGuildName,lpszGuildName,8);
	
	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.h.size,-1);
}

void GS_GDReqRegAttackGuild( int iMapSvrGroup,int aIndex)
{
	if( iMapSvrGroup < 0 )
	{
		return;
	}

	if( OBJECT_RANGE(aIndex) == FALSE )
	{
		return;
	}


	CSP_REQ_REGATTACKGUILD pMsg;

	pMsg.h.set(0x80,0x0D,sizeof(CSP_REQ_REGATTACKGUILD));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iIndex = aIndex;
	memcpy(pMsg.szEnemyGuildName,gObj[aIndex].GuildName,8);
	
	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.h.size,aIndex);
}

void GS_GDReqRestartCastleState( int iMapSvrGroup)
{
	if( iMapSvrGroup < 0 )
	{
		return;
	}

	CSP_REQ_CASTLESIEGEEND pMsg;

	pMsg.h.set(0x80,0x0E,sizeof(CSP_REQ_CASTLESIEGEEND));
	pMsg.wMapSvrNum = iMapSvrGroup;
	
	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.h.size,-1);
}

void GS_GDReqMapSvrMsgMultiCast( int iMapSvrGroup,char*lpszMsgText)
{
	if( iMapSvrGroup < 0 )
	{
		return;
	}

	if( lpszMsgText == NULL )
	{
		return;
	}


	CSP_REQ_MAPSVRMULTICAST pMsg;

	pMsg.h.set(0x80,0x0F,sizeof(CSP_REQ_MAPSVRMULTICAST));
	pMsg.wMapSvrNum = iMapSvrGroup;
	strcpy_s(pMsg.szMsgText,lpszMsgText);
	pMsg.szMsgText[127] = 0;
	
	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.h.size,-1);
}

void GS_GDReqRegGuildMark( int iMapSvrGroup,int aIndex,int iItemPos)
{
	if( iMapSvrGroup < 0 )
	{
		return;
	}

	if( OBJECT_RANGE(aIndex) == FALSE )
	{
		return;
	}

	CSP_REQ_GUILDREGMARK pMsg;

	pMsg.h.set(0x80,0x10,sizeof(CSP_REQ_GUILDREGMARK));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iIndex = aIndex;
	pMsg.iItemPos = iItemPos;
	memcpy(&pMsg.szGuildName,gObj[aIndex].GuildName,sizeof(pMsg.szGuildName));
	
	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.h.size,aIndex);
}

void GS_GDReqGuildMarkReset( int iMapSvrGroup,int aIndex,char* lpszGuildName)
{
	if( iMapSvrGroup < 0 )
	{
		return;
	}

	if( OBJECT_RANGE(aIndex) == FALSE )
	{
		return;
	}

	if( lpszGuildName == NULL )
	{
		return;
	}

	CSP_REQ_GUILDRESETMARK pMsg;

	pMsg.h.set(0x80,0x11,sizeof(CSP_REQ_GUILDRESETMARK));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iIndex = aIndex;
	memcpy(pMsg.szGuildName,lpszGuildName,8);
	
	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.h.size,aIndex);
}

void GS_GDReqGuildSetGiveUp(int iMapSvrGroup,int aIndex,int bIsGiveUp)
{
	if( iMapSvrGroup < 0 )
	{
		return;
	}

	if( OBJECT_RANGE(aIndex) == FALSE )
	{
		return;
	}

	CSP_REQ_GUILDSETGIVEUP pMsg;

	pMsg.h.set(0x80,0x12,sizeof(CSP_REQ_GUILDSETGIVEUP));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iIndex = aIndex;
	pMsg.bIsGiveUp = bIsGiveUp;
	memcpy(pMsg.szGuildName,gObj[aIndex].GuildName,8);
	
	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.h.size,aIndex);
}

void GS_GDReqNpcRemove(int iMapSvrGroup,int iNpcNumber,int iNpcIndex)
{
	if( iMapSvrGroup < 0 )
	{
		return;
	}

	CSP_REQ_NPCREMOVE pMsg;

	pMsg.h.set(0x80,0x16,sizeof(CSP_REQ_NPCREMOVE));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iNpcNumber = iNpcNumber;
	pMsg.iNpcIndex = iNpcIndex;
	
	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.h.size,-1);
}

void GS_GDReqCastleStateSync( int iMapSvrGroup,int iCastleState,int iTaxRateChaos,int iTaxRateStore,int iTaxHuntZone,char* lpszOwnerGuild)
{
	if( iMapSvrGroup < 0 )
	{
		return;
	}

	CSP_REQ_CASTLESTATESYNC pMsg;

	pMsg.h.set(0x80,0x17,sizeof(CSP_REQ_CASTLESTATESYNC));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iCastleState = iCastleState;
	pMsg.iTaxRateChaos = iTaxRateChaos;
	pMsg.iTaxRateStore = iTaxRateStore;
	pMsg.iTaxHuntZone = iTaxHuntZone;
	memcpy(pMsg.szOwnerGuildName,lpszOwnerGuild,8);
	
	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.h.size,-1);
}

void GS_GDReqCastleTributeMoney(int iMapSvrGroup,int iCastleTributeMoney)
{
	if( iMapSvrGroup < 0 )
	{
		return;
	}

	if( iCastleTributeMoney < 0 )
	{
		return;
	}

	CSP_REQ_CASTLETRIBUTEMONEY pMsg;

	pMsg.h.set(0x80,0x18,sizeof(CSP_REQ_CASTLETRIBUTEMONEY));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iCastleTributeMoney = iCastleTributeMoney;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.h.size,-1);
}

void GS_GDReqResetCastleTaxInfo( int iMapSvrGroup)
{
	if( iMapSvrGroup < 0 )
	{
		return;
	}

	CSP_REQ_RESETCASTLETAXINFO pMsg;

	pMsg.h.set(0x80,0x19,sizeof(CSP_REQ_RESETCASTLETAXINFO));
	pMsg.wMapSvrNum = iMapSvrGroup;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.h.size,-1);
}

void GS_GDReqResetSiegeGuildInfo( int iMapSvrGroup)
{
	if( iMapSvrGroup < 0 )
	{
		return;
	}

	CSP_REQ_RESETSIEGEGUILDINFO pMsg;

	pMsg.h.set(0x80,0x1A,sizeof(CSP_REQ_RESETSIEGEGUILDINFO));
	pMsg.wMapSvrNum = iMapSvrGroup;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.h.size,-1);
}

void GS_GDReqResetRegSiegeInfo( int iMapSvrGroup)
{
	if( iMapSvrGroup < 0 )
	{
		return;
	}

	CSP_REQ_RESETREGSIEGEINFO pMsg;

	pMsg.h.set(0x80,0x1B,sizeof(CSP_REQ_RESETREGSIEGEINFO));
	pMsg.wMapSvrNum = iMapSvrGroup;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.h.size,-1);
}

void GS_GDReqCastleInitData( int iMapSvrGroup,int iCastleEventCycle)
{
	if( iMapSvrGroup < 0 )
	{
		return;
	}

	CSP_REQ_CSINITDATA pMsg;

	pMsg.h.set(0x81,sizeof(CSP_REQ_CSINITDATA));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iCastleEventCycle = iCastleEventCycle;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.h.size,-1);
}

void GS_GDReqCastleNpcInfo( int iMapSvrGroup,int aIndex)
{
	if( iMapSvrGroup < 0 )
	{
		return;
	}

	CSP_REQ_NPCDATA pMsg;

	pMsg.h.set(0x82,sizeof(CSP_REQ_NPCDATA));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iIndex = aIndex;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.h.size,aIndex);
}

void GS_GDReqAllGuildMarkRegInfo( int iMapSvrGroup,int aIndex)
{
	if( iMapSvrGroup < 0 )
	{
		return;
	}

	CSP_REQ_ALLGUILDREGINFO pMsg;

	pMsg.h.set(0x83,sizeof(CSP_REQ_ALLGUILDREGINFO));
	pMsg.wMapSvrNum = iMapSvrGroup;
	pMsg.iIndex = aIndex;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.h.size,aIndex);
}

void GS_GDReqCalcRegGuildList(int iMapSvrGroup)
{
	if( iMapSvrGroup < 0 )
	{
		return;
	}

	CSP_REQ_CALCREGGUILDLIST pMsg;

	pMsg.h.set(0x85,sizeof(CSP_REQ_CALCREGGUILDLIST));
	pMsg.wMapSvrNum = iMapSvrGroup;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.h.size,-1);
}

void GS_GDReqCsLoadTotalGuildInfo(int iMapSvrGroup)
{
	if( iMapSvrGroup < 0 )
	{
		return;
	}

	CSP_REQ_CSLOADTOTALGUILDINFO pMsg;

	pMsg.h.set(0x88,sizeof(CSP_REQ_CSLOADTOTALGUILDINFO));
	pMsg.wMapSvrNum = iMapSvrGroup;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.h.size,-1);
}

void GS_DGAnsOwnerGuildMaster(LPBYTE lpRecv)
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	CSP_ANS_OWNERGUILDMASTER* lpMsg = (CSP_ANS_OWNERGUILDMASTER*)lpRecv;

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != gMapServerManager.GetMapServerGroup())
	{
		return;
	}

	GCAnsCastleSiegeState(lpMsg->iIndex,lpMsg->iResult,lpMsg->szCastleOwnGuild,lpMsg->szCastleOwnGuildMaster);
}

void GS_DGAnsCastleNpcBuy(LPBYTE lpRecv)
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	CSP_ANS_NPCBUY* lpMsg = (CSP_ANS_NPCBUY*)lpRecv;

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != gMapServerManager.GetMapServerGroup())
	{
		return;
	}

	if(lpMsg->iResult == 1)
	{
		BOOL bAddResult = gCastleSiege.AddDbNPC(lpMsg->iNpcNumber,lpMsg->iNpcIndex);
	
		if( bAddResult == TRUE)
		{
			if(gObjIsConnected(lpMsg->iIndex))
			{
				gObj[lpMsg->iIndex].Money -= lpMsg->iBuyCost;

				if(gObj[lpMsg->iIndex].Money < 0 )
				{
					gObj[lpMsg->iIndex].Money = 0;
				}

				GCMoneySend(lpMsg->iIndex,gObj[lpMsg->iIndex].Money);
			}
		}
	}

	GCAnsNpcBuy(lpMsg->iIndex,lpMsg->iResult,lpMsg->iNpcNumber,lpMsg->iNpcIndex);
}

void GS_DGAnsCastleNpcRepair(LPBYTE lpRecv)
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	CSP_ANS_NPCREPAIR* lpMsg = (CSP_ANS_NPCREPAIR*) lpRecv;

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != gMapServerManager.GetMapServerGroup())
	{
		return;
	}

	if(lpMsg->iResult == 1)
	{
		BOOL bRepairResult =  gCastleSiege.RepairDbNPC(lpMsg->iNpcNumber,lpMsg->iNpcIndex,lpMsg->iNpcHp,lpMsg->iNpcMaxHp);
		if( bRepairResult == TRUE)
		{
			if(gObjIsConnected(lpMsg->iIndex))
			{
				gObj[lpMsg->iIndex].Money -= lpMsg->iRepairCost;

				if(gObj[lpMsg->iIndex].Money < 0 )
				{
					gObj[lpMsg->iIndex].Money = 0;
				}

				GCMoneySend(lpMsg->iIndex,gObj[lpMsg->iIndex].Money);
			}
		}
	}

	GCAnsNpcRepair(lpMsg->iIndex,lpMsg->iResult,lpMsg->iNpcNumber,lpMsg->iNpcIndex,lpMsg->iNpcHp,lpMsg->iNpcMaxHp);
}

void GS_DGAnsCastleNpcUpgrade(LPBYTE lpRecv)
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	CSP_ANS_NPCUPGRADE* lpMsg = (CSP_ANS_NPCUPGRADE*)lpRecv;

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != gMapServerManager.GetMapServerGroup())
	{
		return;
	}

	if(lpMsg->iResult != 0)
	{
		gCastleSiege.UpgradeDbNPC(lpMsg->iIndex,lpMsg->iNpcNumber,lpMsg->iNpcIndex,lpMsg->iNpcUpType,lpMsg->iNpcUpValue,lpMsg->iNpcUpIndex);
	}

	GCAnsNpcUpgrade(lpMsg->iIndex,lpMsg->iResult,lpMsg->iNpcNumber,lpMsg->iNpcIndex,lpMsg->iNpcUpType,lpMsg->iNpcUpValue);
}

void GS_DGAnsTaxInfo(LPBYTE lpRecv)
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	CSP_ANS_TAXINFO* lpMsg = (CSP_ANS_TAXINFO*)lpRecv;

	if(lpMsg == NULL)
		return;
	
	if(lpMsg->wMapSvrNum != gMapServerManager.GetMapServerGroup())
	{
		return;
	}

	if(lpMsg->iResult == TRUE)
	{
		GCAnsTaxMoneyInfo(lpMsg->iIndex,lpMsg->iResult,lpMsg->iTaxRateChaos,lpMsg->iTaxRateStore,lpMsg->i64CastleMoney);
		gCastleSiege.SetCastleMoney(lpMsg->i64CastleMoney);
	}
}

void GS_DGAnsTaxRateChange(LPBYTE lpRecv)
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	CSP_ANS_TAXRATECHANGE* lpMsg = (CSP_ANS_TAXRATECHANGE*)lpRecv;

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != gMapServerManager.GetMapServerGroup())
	{
		return;
	}

	if(lpMsg->iResult == 1)
	{
		gCastleSiege.SetTaxRate(lpMsg->iTaxKind,lpMsg->iTaxRate);
		GCAnsTaxRateChange(lpMsg->iIndex,lpMsg->iResult,lpMsg->iTaxKind,lpMsg->iTaxRate);
	}
}

void GS_DGAnsCastleMoneyChange(LPBYTE lpRecv)
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	CSP_ANS_MONEYCHANGE* lpMsg = (CSP_ANS_MONEYCHANGE*)lpRecv;

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != gMapServerManager.GetMapServerGroup())
	{
		return;
	}

	if(lpMsg->iResult == TRUE)
	{
		if(gObjIsConnected(lpMsg->iIndex))
		{
			if(lpMsg->iMoneyChanged < 0)
			{
				gObj[lpMsg->iIndex].Money -= lpMsg->iMoneyChanged;

				if(gObj[lpMsg->iIndex].Money > MAX_MONEY)
				{
					gObj[lpMsg->iIndex].Money = MAX_MONEY;
				}

				GCMoneySend(lpMsg->iIndex,gObj[lpMsg->iIndex].Money);
			}
		}
		gCastleSiege.SetCastleMoney(lpMsg->i64CastleMoney);
	}
	
	GCAnsMoneyDrawOut(lpMsg->iIndex,lpMsg->iResult,lpMsg->i64CastleMoney);
}

void GS_DGAnsGuildMarkRegInfo(LPBYTE lpRecv)
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	CSP_ANS_GUILDREGINFO* lpMsg = (CSP_ANS_GUILDREGINFO*)lpRecv;

	if(lpMsg == NULL)
		return;
	
	if(lpMsg->wMapSvrNum != gMapServerManager.GetMapServerGroup())
	{
		return;
	}

	GCAnsGuildRegInfo(lpMsg->iIndex,lpMsg->iResult,lpMsg);
}

void GS_DGAnsRegAttackGuild(LPBYTE lpRecv)
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	CSP_ANS_REGATTACKGUILD* lpMsg = (CSP_ANS_REGATTACKGUILD*)lpRecv;

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != gMapServerManager.GetMapServerGroup())
	{
		return;
	}

	GCAnsRegCastleSiege(lpMsg->iIndex,lpMsg->iResult,lpMsg->szEnemyGuildName);
}

void GS_DGAnsRestartCastleState(LPBYTE lpRecv)
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	CSP_ANS_CASTLESIEGEEND* lpMsg = (CSP_ANS_CASTLESIEGEEND*)lpRecv;

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != gMapServerManager.GetMapServerGroup())
	{
		return;
	}
	
	gCastleSiege.ResetCastleCycle();
}

void GS_DGAnsMapSvrMsgMultiCast(LPBYTE lpRecv)
{
	CSP_ANS_MAPSVRMULTICAST*lpMsg = (CSP_ANS_MAPSVRMULTICAST *)lpRecv;

	if( lpMsg == NULL )
	{
		return;
	}

	if( lpMsg->wMapSvrNum != gMapServerManager.GetMapServerGroup() )
	{
		return;
	}

	gNotice.GCNoticeSendToAll(0,0,0,0,0,0,"%s",lpMsg->szMsgText);
}

void GS_DGAnsRegGuildMark(LPBYTE lpRecv)
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	CSP_ANS_GUILDREGMARK* lpMsg = (CSP_ANS_GUILDREGMARK*)lpRecv;
		
	if( lpMsg == NULL )
	{
		return;
	}

	if( lpMsg->wMapSvrNum != gMapServerManager.GetMapServerGroup() )
	{
		return;
	}
	
	GCAnsRegGuildMark(lpMsg->iIndex,lpMsg->iResult,lpMsg);
}

void GS_DGAnsGuildSetGiveUp(LPBYTE lpRecv)
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	CSP_ANS_GUILDSETGIVEUP* lpMsg = (CSP_ANS_GUILDSETGIVEUP*)lpRecv;

	if( lpMsg == NULL )
	{
		return;
	}

	if( lpMsg->wMapSvrNum != gMapServerManager.GetMapServerGroup() )
	{
		return;
	}
	
	GCAnsGiveUpCastleSiege(lpMsg->iIndex,lpMsg->iResult,lpMsg->bIsGiveUp,lpMsg->iRegMarkCount,lpMsg->szGuildName);
}

void GS_DGAnsCastleStateSync(LPBYTE lpRecv) //Identical gs-cs 56
{
	CSP_ANS_CASTLESTATESYNC*lpMsg = (CSP_ANS_CASTLESTATESYNC *)lpRecv;

	if( lpMsg == NULL )
	{
		return;
	}

	if( lpMsg->wMapSvrNum != gMapServerManager.GetMapServerGroup() )
	{
		return;
	}

	gCastleSiegeSync.SetCastleState(lpMsg->iCastleState); 
	gCastleSiegeSync.SetTaxRateChaos(lpMsg->iTaxRateChaos);
	gCastleSiegeSync.SetTaxRateStore(lpMsg->iTaxRateStore);
	gCastleSiegeSync.SetTaxHuntZone(lpMsg->iTaxHuntZone);
	gCastleSiegeSync.SetCastleOwnerGuild(lpMsg->szOwnerGuildName);
}

void GS_DGAnsCastleTributeMoney(LPBYTE lpRecv)
{
	CSP_ANS_CASTLETRIBUTEMONEY*lpMsg = (CSP_ANS_CASTLETRIBUTEMONEY *)lpRecv;

	if( lpMsg == NULL )
	{
		return;
	}

	if( lpMsg->wMapSvrNum != gMapServerManager.GetMapServerGroup() )
	{
		return;
	}
	
	gCastleSiegeSync.ResetTributeMoney();
}

void GS_DGAnsResetCastleTaxInfo(LPBYTE lpRecv)
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	CSP_ANS_RESETCASTLETAXINFO* lpMsg = (CSP_ANS_RESETCASTLETAXINFO*)lpRecv;

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != gMapServerManager.GetMapServerGroup())
	{
		return;
	}

	if(lpMsg->iResult == 1)
	{
		gCastleSiege.ResetCastleTaxInfo();
	}
}

void GS_DGAnsCastleInitData(LPBYTE lpRecv)
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	CSP_ANS_CSINITDATA* lpMsg = (CSP_ANS_CSINITDATA*)lpRecv;
	CSP_CSINITDATA* lpMsgBody = (CSP_CSINITDATA*)(lpRecv+sizeof(CSP_ANS_CSINITDATA));

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != gMapServerManager.GetMapServerGroup())
	{
		return;
	}

	if(gCastleSiege.GetDataLoadState() != 2)
	{
		return;
	}

	gCastleSiege.SetDataLoadState(3);

	if(lpMsg->iResult == FALSE)
	{
		return;
	}
	
	BOOL bRET_VAL = FALSE;

	bRET_VAL = gCastleSiege.SetCastleInitData(lpMsg);

	if(bRET_VAL == FALSE)
	{
		return;
	}

	bRET_VAL = gCastleSiege.SetCastleNpcData(lpMsgBody,lpMsg->iCount);
	
	if(bRET_VAL == FALSE)
	{
		return;
	}

	gCastleSiege.SetDbDataLoadOK(TRUE);

	gCastleSiege.SetDataLoadState(4);

	if(lpMsg->iFirstCreate == 1)
	{
		gCastleSiege.FirstCreateDbNPC();
	}

	gCastleSiege.Init();
}

void GS_DGAnsAllGuildMarkRegInfo(LPBYTE lpRecv)
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	CSP_ANS_ALLGUILDREGINFO* lpMsg = (CSP_ANS_ALLGUILDREGINFO*)lpRecv;
	CSP_GUILDREGINFO* lpMsgBody = (CSP_GUILDREGINFO*)(lpRecv+sizeof(CSP_ANS_ALLGUILDREGINFO));
	char cBUFFER[1668];
	PMSG_ANS_CSREGGUILDLIST* lpMsgSend;
	PMSG_CSREGGUILDLIST* lpMsgSendBody;

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != gMapServerManager.GetMapServerGroup())
	{
		return;
	}

	if(!gObjIsConnected(lpMsg->iIndex))
		return;

	lpMsgSend = (PMSG_ANS_CSREGGUILDLIST*)cBUFFER;
	lpMsgSendBody = (PMSG_CSREGGUILDLIST*)&cBUFFER[12];

	lpMsgSend->iCount = 0;
	lpMsgSend->btResult = lpMsg->iResult;
	
	if(lpMsg->iResult == TRUE)
	{
		lpMsgSend->iCount = lpMsg->iCount;

		for(int i=0;i< lpMsg->iCount;i++)
		{
			lpMsgSendBody[i].btSeqNum = lpMsgBody[i].btRegRank;
			lpMsgSendBody[i].btIsGiveUp = lpMsgBody[i].bIsGiveUp;
			lpMsgSendBody[i].btRegMarks1 = SET_NUMBERHB(SET_NUMBERHW(lpMsgBody[i].iRegMarkCount));
			lpMsgSendBody[i].btRegMarks2 = SET_NUMBERLB(SET_NUMBERHW(lpMsgBody[i].iRegMarkCount));
			lpMsgSendBody[i].btRegMarks3 = SET_NUMBERHB(SET_NUMBERLW(lpMsgBody[i].iRegMarkCount));
			lpMsgSendBody[i].btRegMarks4 = SET_NUMBERLB(SET_NUMBERLW(lpMsgBody[i].iRegMarkCount));
			memcpy(&lpMsgSendBody[i].szGuildName,lpMsgBody[i].szGuildName,sizeof(lpMsgSendBody[i].szGuildName));
		}
	}
	
	lpMsgSend->h.set(0xB4,(lpMsgSend->iCount*sizeof(PMSG_CSREGGUILDLIST)+sizeof(PMSG_ANS_CSREGGUILDLIST)));

	DataSend(lpMsg->iIndex,(LPBYTE)lpMsgSend,(lpMsgSend->iCount*sizeof(PMSG_CSREGGUILDLIST)+sizeof(PMSG_ANS_CSREGGUILDLIST)));
}

void GS_DGAnsFirstCreateNPC(LPBYTE lpRecv)
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	CSP_ANS_NPCSAVEDATA* lpMsg = (CSP_ANS_NPCSAVEDATA*)lpRecv;

	if(lpMsg == NULL)
	return;

	if(lpMsg->wMapSvrNum != gMapServerManager.GetMapServerGroup())
	{
		return;
	}

	if(lpMsg->iResult == TRUE)
	{
		if(gCastleSiege.GetDbNpcCreated() == FALSE)
		{
			gCastleSiege.CreateDbNPC();
			gCastleSiege.SetDbNpcCreated(1);
		}
	}
}

void GS_DGAnsCalcREgGuildList(LPBYTE lpRecv)
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	CSP_ANS_CALCREGGUILDLIST* lpMsg = (CSP_ANS_CALCREGGUILDLIST*)lpRecv;
	CSP_CALCREGGUILDLIST* lpMsgBody = (CSP_CALCREGGUILDLIST*)(lpRecv+sizeof(CSP_ANS_CALCREGGUILDLIST));
	
	if(lpMsg == NULL)
	return;

	if(lpMsg->wMapSvrNum != gMapServerManager.GetMapServerGroup())
	{
		return;
	}

	if(lpMsg->iResult == TRUE)
	{
		gCastleSiege.SetCalcRegGuildList(lpMsgBody,lpMsg->iCount);
		return;
	}
}

void GS_DGAnsCsGulidUnionInfo(LPBYTE lpRecv)
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	CSP_ANS_CSGUILDUNIONINFO* lpMsg = (CSP_ANS_CSGUILDUNIONINFO*)lpRecv;
	CSP_CSGUILDUNIONINFO* lpMsgBody = (CSP_CSGUILDUNIONINFO*)(lpRecv+sizeof(CSP_ANS_CSGUILDUNIONINFO));
	
	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != gMapServerManager.GetMapServerGroup())
	{
		return;
	}

	if(lpMsg->iResult == TRUE)
	{
		gCastleSiege.MakeCsTotalGuildInfo(lpMsgBody,lpMsg->iCount);
		return;
	}
}

void GS_DGAnsCsSaveTotalGuildInfo(LPBYTE lpRecv)
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	CSP_ANS_CSSAVETOTALGUILDINFO* lpMsg = (CSP_ANS_CSSAVETOTALGUILDINFO*)lpRecv;
	
	if(lpMsg == NULL)
	return;

	if(lpMsg->wMapSvrNum != gMapServerManager.GetMapServerGroup())
	{
		return;
	}

	if(lpMsg->iResult == TRUE)
	{
		gCastleSiege.SetIsSiegeGuildList(TRUE);
		return;
	}
	
	gCastleSiege.SetIsSiegeGuildList(FALSE);
}

void GS_DGAnsCsLoadTotalGuildInfo(LPBYTE lpRecv)
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	CSP_ANS_CSLOADTOTALGUILDINFO* lpMsg = (CSP_ANS_CSLOADTOTALGUILDINFO*)lpRecv;
	CSP_CSLOADTOTALGUILDINFO* lpMsgBody = (CSP_CSLOADTOTALGUILDINFO*)(lpRecv+sizeof(CSP_ANS_CSLOADTOTALGUILDINFO));

	if(lpMsg == NULL)
		return;

	if(lpMsg->wMapSvrNum != gMapServerManager.GetMapServerGroup())
	{
		return;
	}

	if(lpMsg->iResult == TRUE)
	{
		gCastleSiege.SetCsTotalGuildInfo(lpMsgBody,lpMsg->iCount);
		return;
	}
}