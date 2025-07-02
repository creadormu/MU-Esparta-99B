#include "stdafx.h"
#include "Protocol.h"
#include "AcheronGuardian.h"
#include "ArcaBattle.h"
#include "Attack.h"
#include "BloodCastle.h"
#include "CashShop.h"
#include "CastleSiege.h"
#include "CastleSiegeSync.h"
#include "CastleSiegeWeapon.h"
#include "ChaosBox.h"
#include "ChaosCastle.h"
#include "ChaosCastleFinal.h"
#include "CommandManager.h"
#include "ConnectionManager.h"
#include "Crywolf.h"
#include "CustomAttack.h"
#include "CustomHealthBar.h"
#include "DarkSpirit.h"
#include "DefaultClassInfo.h"
#include "DevilSquare.h"
#include "DoubleGoer.h"
#include "DSProtocol.h"
#include "Duel.h"
#include "EffectManager.h"
#include "EventEntryLevel.h"
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
#include "HackPacketCheck.h"
#include "Helper.h"
#include "IllusionTemple.h"
#include "IllusionTempleRenewal.h"
#include "ImperialGuardian.h"
#include "InventoryEquipment.h"
#include "ItemManager.h"
#include "JewelMix.h"
#include "JSProtocol.h"
#include "KanturuEntranceNPC.h"
#include "Log.h"
#include "LuckyCoin.h"
#include "Map.h"
#include "MapManager.h"
#include "MapServerManager.h"
#include "MasterSkillTree.h"
#include "Message.h"
#include "MiniMap.h"
#include "MiningSystem.h"
#include "Move.h"
#include "MuCashShop.h"
#include "MuRummy.h"
#include "MuunSystem.h"
#include "Notice.h"
#include "NpcTalk.h"
#include "ObjectManager.h"
#include "Party.h"
#include "PartyMatching.h"
#include "PcPoint.h"
#include "PentagramSystem.h"
#include "PersonalShop.h"
#include "Quest.h"
#include "QuestWorld.h"
#include "ServerInfo.h"
#include "SkillManager.h"
#include "Trade.h"
#include "Util.h"
#include "Viewport.h"
#include "Warehouse.h"
#include "FakeOnline.h"

void ProtocolCore(BYTE head,BYTE* lpMsg,int size,int aIndex,int encrypt,int serial) // OK
{
	PROTECT_START

	if(gObj[aIndex].Type == OBJECT_USER && gHackPacketCheck.CheckPacketHack(aIndex,head,((lpMsg[0]==0xC1)?lpMsg[3]:lpMsg[4]),encrypt,serial) == 0)
	{
		return;
	}

	switch(head)
	{
		case 0x00:
			CGChatRecv((PMSG_CHAT_RECV*)lpMsg,aIndex);
			break;
		case 0x02:
			CGChatWhisperRecv((PMSG_CHAT_WHISPER_RECV*)lpMsg,aIndex);
			break;
		case 0x03:
			CGMainCheckRecv((PMSG_MAIN_CHECK_RECV*)lpMsg,aIndex);
			break;
		case 0x04:
			gCustomAttack.CustomAttackClose(aIndex);
			break;
		case 0x0E:
			CGLiveClientRecv((PMSG_LIVE_CLIENT_RECV*)lpMsg,aIndex);
			break;
		case PROTOCOL_CODE2:
			gAttack.CGAttackRecv((PMSG_ATTACK_RECV*)lpMsg,aIndex);
			break;
		case PROTOCOL_CODE3:
			CGPositionRecv((PMSG_POSITION_RECV*)lpMsg,aIndex);
			break;
		case 0x18:
			CGActionRecv((PMSG_ACTION_RECV*)lpMsg,aIndex);
			break;
		case 0x19:
			gSkillManager.CGSkillAttackRecv((PMSG_SKILL_ATTACK_RECV*)lpMsg,aIndex);
			break;
		case 0x1B:
			gSkillManager.CGSkillCancelRecv((PMSG_SKILL_CANCEL_RECV*)lpMsg,aIndex);
			break;
		case 0x1C:
			gMove.CGTeleportRecv((PMSG_TELEPORT_RECV*)lpMsg,aIndex);
			break;
		case 0x1E:
			gSkillManager.CGDurationSkillAttackRecv((PMSG_DURATION_SKILL_ATTACK_RECV*)lpMsg,aIndex);
			break;
		case 0x22:
			gItemManager.CGItemGetRecv((PMSG_ITEM_GET_RECV*)lpMsg,aIndex);
			break;
		case 0x23:
			gItemManager.CGItemDropRecv((PMSG_ITEM_DROP_RECV*)lpMsg,aIndex);
			break;
		case 0x24:
			gItemManager.CGItemMoveRecv((PMSG_ITEM_MOVE_RECV*)lpMsg,aIndex);
			break;
		case 0x26:
			gItemManager.CGItemUseRecv((PMSG_ITEM_USE_RECV*)lpMsg,aIndex);
			break;
		case 0x30:
			gNpcTalk.CGNpcTalkRecv((PMSG_NPC_TALK_RECV*)lpMsg,aIndex);
			break;
		case 0x31:
			gNpcTalk.CGNpcTalkCloseRecv(aIndex);
			break;
		case 0x32:
			gItemManager.CGItemBuyRecv((PMSG_ITEM_BUY_RECV*)lpMsg,aIndex);
			break;
		case 0x33:
			gItemManager.CGItemSellRecv((PMSG_ITEM_SELL_RECV*)lpMsg,aIndex);
			break;
		case 0x34:
			gItemManager.CGItemRepairRecv((PMSG_ITEM_REPAIR_RECV*)lpMsg,aIndex);
			break;
		case 0x36:
			gTrade.CGTradeRequestRecv((PMSG_TRADE_REQUEST_RECV*)lpMsg,aIndex);
			break;
		case 0x37:
			gTrade.CGTradeResponseRecv((PMSG_TRADE_RESPONSE_RECV*)lpMsg,aIndex);
			break;
		case 0x3A:
			gTrade.CGTradeMoneyRecv((PMSG_TRADE_MONEY_RECV*)lpMsg,aIndex);
			break;
		case 0x3C:
			gTrade.CGTradeOkButtonRecv((PMSG_TRADE_OK_BUTTON_RECV*)lpMsg,aIndex);
			break;
		case 0x3D:
			gTrade.CGTradeCancelButtonRecv(aIndex);
			break;
		case 0x3F:
			switch(lpMsg[3])
			{
				case 0x01:
					gPersonalShop.CGPShopSetItemPriceRecv((PMSG_PSHOP_SET_ITEM_PRICE_RECV*)lpMsg,aIndex);
					break;
				case 0x02:
					gPersonalShop.CGPShopOpenRecv((PMSG_PSHOP_OPEN_RECV*)lpMsg,aIndex);
					break;
				case 0x03:
					gPersonalShop.CGPShopCloseRecv(aIndex);
					break;
				case 0x05:
					gPersonalShop.CGPShopItemListRecv((PMSG_PSHOP_ITEM_LIST_RECV*)lpMsg,aIndex);
					break;
				case 0x06:
					gPersonalShop.CGPShopBuyItemRecv((PMSG_PSHOP_BUY_ITEM_RECV*)lpMsg,aIndex);
					break;
				case 0x07:
					gPersonalShop.CGPShopLeaveRecv((PMSG_PSHOP_LEAVE_RECV*)lpMsg,aIndex);
					break;
			}
			break;
		case 0x40:
			gParty.CGPartyRequestRecv((PMSG_PARTY_REQUEST_RECV*)lpMsg,aIndex);
			break;
		case 0x41:
			gParty.CGPartyRequestResultRecv((PMSG_PARTY_REQUEST_RESULT_RECV*)lpMsg,aIndex);
			break;
		case 0x42:
			gParty.CGPartyListRecv(aIndex);
			break;
		case 0x43:
			gParty.CGPartyDelMemberRecv((PMSG_PARTY_DEL_MEMBER_RECV*)lpMsg,aIndex);
			break;
		case 0x4A:
			#if(GAMESERVER_UPDATE>=601)
			gSkillManager.CGRageFighterSkillAttackRecv((PMSG_RAGE_FIGHTER_SKILL_ATTACK_RECV*)lpMsg,aIndex);
			#endif
			break;
		case 0x4B:
			#if(GAMESERVER_UPDATE>=601)
			gSkillManager.CGSkillDarkSideRecv((PMSG_SKILL_DARK_SIDE_RECV*)lpMsg,aIndex);
			#endif
			break;
		case 0x4C:
			switch(lpMsg[3])
			{
				case 0x00:
					#if(GAMESERVER_UPDATE>=801)
					gMiningSystem.CGMiningStartRecv((PMSG_MINING_START_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x01:
					#if(GAMESERVER_UPDATE>=801)
					gMiningSystem.CGMiningSuccessRecv((PMSG_MINING_SUCCESS_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x03:
					#if(GAMESERVER_UPDATE>=801)
					gMiningSystem.CGMiningFailureRecv((PMSG_MINING_FAILURE_RECV*)lpMsg,aIndex);
					#endif
					break;
			}
			break;
		case 0x4D:
			switch(lpMsg[3])
			{
				case 0x00:
					#if(GAMESERVER_UPDATE>=802)
					gEventInventory.CGEventItemGetRecv((PMSG_EVENT_ITEM_GET_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x01:
					#if(GAMESERVER_UPDATE>=802)
					gEventInventory.CGEventItemDropRecv((PMSG_EVENT_ITEM_DROP_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x0F:
					#if(GAMESERVER_UPDATE>=802)
					gEventInventory.CGEventInventoryOpenRecv((PMSG_EVENT_INVENTORY_OPEN_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x10:
					#if(GAMESERVER_UPDATE>=802)
					gMuRummy.CGReqMuRummyStart((_tagPMSG_REQ_MURUMMY_INFO*)lpMsg,aIndex);
					#endif
					break;
				case 0x11:
					#if(GAMESERVER_UPDATE>=802)
					gMuRummy.CGReqCardReveal((_tagPMSG_REQ_REVEAL_CARD*)lpMsg,aIndex);
					#endif
					break;
				case 0x12:
					#if(GAMESERVER_UPDATE>=802)
					gMuRummy.CGReqCardMove((_tagPMSG_REQ_CARD_MOVE*)lpMsg,aIndex);
					#endif
					break;
				case 0x13:
					#if(GAMESERVER_UPDATE>=802)
					gMuRummy.CGReqCardReMove((_tagPMSG_REQ_CARD_REMOVE*)lpMsg,aIndex);
					#endif
					break;
				case 0x14:
					#if(GAMESERVER_UPDATE>=802)
					gMuRummy.CGReqCardMatch((_tagPMSG_REQ_CARD_MATCH*)lpMsg,aIndex);
					#endif
					break;
				case 0x15:
					#if(GAMESERVER_UPDATE>=802)
					gMuRummy.CGReqMuRummyEnd((_tagPMSG_REQ_MURUMMY_END*)lpMsg,aIndex);
					#endif
					break;
			}
			break;
		case 0x4E:
			switch(lpMsg[3])
			{
				case 0x00:
					#if(GAMESERVER_UPDATE>=803)
					gMuunSystem.CGMuunItemGetRecv((PMSG_MUUN_ITEM_GET_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x08:
					#if(GAMESERVER_UPDATE>=803)
					gMuunSystem.CGMuunItemUseRecv((PMSG_MUUN_ITEM_USE_RECV*)lpMsg,aIndex);
					#endif
				case 0x09:
					#if(GAMESERVER_UPDATE>=803)
					gMuunSystem.CGMuunItemSellRecv((PMSG_MUUN_ITEM_SELL_RECV*)lpMsg,aIndex);
					#endif
					break;
			}
			break;
		case 0x50:
			gGuild.CGGuildRequestRecv((PMSG_GUILD_REQUEST_RECV*)lpMsg,aIndex);
			break;
		case 0x51:
			gGuild.CGGuildResultRecv((PMSG_GUILD_RESULT_RECV*)lpMsg,aIndex);
			break;
		case 0x52:
			gGuild.CGGuildListRecv(aIndex);
			break;
		case 0x53:
			gGuild.CGGuildDeleteRecv((PMSG_GUILD_DELETE_RECV*)lpMsg,aIndex);
			break;
		case 0x54:
			gGuild.CGGuildMasterOpenRecv((PMSG_GUILD_MASTER_OPEN_RECV*)lpMsg,aIndex);
			break;
		case 0x55:
			gGuild.CGGuildCreateRecv((PMSG_GUILD_CREATE_RECV*)lpMsg,aIndex);
			break;
		case 0x57:
			gGuild.CGGuildMasterCancelRecv(aIndex);
			break;
		case 0x61:
			gGuild.CGGuildWarRequestResultRecv((PMSG_GUILD_WAR_REQUEST_RESULT_RECV*)lpMsg,aIndex);
			break;
		case 0x66:
			gGuild.CGGuildViewportRequestRecv((PMSG_GUILD_VIEWPORT_REQUEST_RECV*)lpMsg,aIndex);
			break;
		case 0x81:
			gWarehouse.CGWarehouseMoneyRecv((PMSG_WAREHOUSE_MONEY_RECV*)lpMsg,aIndex);
			break;
		case 0x82:
			gWarehouse.CGWarehouseClose(aIndex);
			break;
		case 0x83:
			gWarehouse.CGWarehousePasswordRecv((PMSG_WAREHOUSE_PASSWORD_RECV*)lpMsg,aIndex);
			break;
		case 0x86:
			gChaosBox.CGChaosMixRecv((PMSG_CHAOS_MIX_RECV*)lpMsg,aIndex);
			break;
		case 0x87:
			gChaosBox.CGChaosMixCloseRecv(aIndex);
			break;
		case 0x88:
			gChaosBox.CGChaosMixRateRecv((PMSG_CHAOS_MIX_RATE_RECV*)lpMsg,aIndex);
			break;
		case 0x8E:
			#if(GAMESERVER_UPDATE>=501)
			gMove.CGTeleportMoveRecv((PMSG_TELEPORT_MOVE_RECV*)lpMsg,aIndex);
			#endif
			break;
		case 0x90:
			gDevilSquare.CGDevilSquareEnterRecv((PMSG_DEVIL_SQUARE_ENTER_RECV*)lpMsg,aIndex);
			break;
		case 0x91:
			CGEventRemainTimeRecv((PMSG_EVENT_REMAIN_TIME_RECV*)lpMsg,aIndex);
			break;
		case 0x95:
			#if(GAMESERVER_UPDATE<=603)
			gGoldenArcher.CGGoldenArcherRegisterRecv((PMSG_GOLDEN_ARCHER_COUNT_RECV*)lpMsg,aIndex);
			#endif
			break;
		case 0x97:
			#if(GAMESERVER_UPDATE<=603)
			gGoldenArcher.CGGoldenArcherCloseRecv(aIndex);
			#endif
			break;
		case 0x9A:
			gBloodCastle.CGBloodCastleEnterRecv((PMSG_BLOOD_CASTLE_ENTER_RECV*)lpMsg,aIndex);
			break;
		case 0xA0:
			gQuest.CGQuestInfoRecv(aIndex);
			break;
		case 0xA2:
			gQuest.CGQuestStateRecv((PMSG_QUEST_STATE_RECV*)lpMsg,aIndex);
			break;
		case 0xA7:
			CGPetItemCommandRecv((PMSG_PET_ITEM_COMMAND_RECV*)lpMsg,aIndex);
			break;
		case 0xA9:
			CGPetItemInfoRecv((PMSG_PET_ITEM_INFO_RECV*)lpMsg,aIndex);
			break;
		case 0xAA:
			#if(GAMESERVER_UPDATE>=402)
			switch(lpMsg[3])
			{
				case 0x01:
					gDuel.CGDuelStartRecv((PMSG_DUEL_START_RECV*)lpMsg,aIndex);
					break;
				case 0x02:
					gDuel.CGDuelOkRecv((PMSG_DUEL_OK_RECV*)lpMsg,aIndex);
					break;
				case 0x03:
					gDuel.CGDuelEndRecv(aIndex);
					break;
				case 0x07:
					gDuel.CGDuelWatchRecv((PMSG_DUEL_WATCH_RECV*)lpMsg,aIndex);
					break;
				case 0x09:
					gDuel.CGDuelButtonRecv((PMSG_DUEL_BUTTON_RECV*)lpMsg,aIndex);
					break;
			}
			#else
			gDuel.CGDuelStartRecv((PMSG_DUEL_START_RECV*)lpMsg,aIndex);
			#endif
			break;
		case 0xAB:
			#if(GAMESERVER_UPDATE<=401)
			gDuel.CGDuelEndRecv(aIndex);
			#endif
			break;
		case 0xAC:
			#if(GAMESERVER_UPDATE<=401)
			gDuel.CGDuelOkRecv((PMSG_DUEL_OK_RECV*)lpMsg,aIndex);
			#endif
			break;
		case 0xAE:
			#if(GAMESERVER_UPDATE>=603)
			gHelper.CGHelperDataRecv((PMSG_HELPER_DATA_RECV*)lpMsg,aIndex);
			#endif
			break;
		case 0xAF:
			switch(lpMsg[3])
			{
				case 0x01:
					gChaosCastle.CGChaosCastleEnterRecv((PMSG_CHAOS_CASTLE_ENTER_RECV*)lpMsg,aIndex);
					break;
				case 0x02:
					gChaosCastle.CGChaosCastlePositionRecv((PMSG_CHAOS_CASTLE_POSITION_RECV*)lpMsg,aIndex);
					break;
				case 0x03:
					#if(GAMESERVER_UPDATE>=803)
					gChaosCastleFinal.CGChaosCastleFinalRankingSend(aIndex);
					#endif
					break;
				case 0x05:
					#if(GAMESERVER_UPDATE>=803)
					gChaosCastleFinal.CGChaosCastleFinalEnterRecv(aIndex);
					#endif
					break;
				case 0x06:
					#if(GAMESERVER_UPDATE>=803)
					gChaosCastleFinal.CGChaosCastleFinalPositionRecv((PMSG_CHAOS_CASTLE_FINAL_POSITION_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x07:
					#if(GAMESERVER_UPDATE>=803)
					gChaosCastleFinal.GCChaosCastleFinalRankingRecv(aIndex);
					#endif
					break;
				case 0x08:
					#if(GAMESERVER_UPDATE>=803)
					gChaosCastleFinal.CGChaosCastleFinalClose(aIndex);
					#endif
					break;
			}
			break;
		case 0xB0:
			gSkillManager.CGSkillTeleportAllyRecv((PMSG_SKILL_TELEPORT_ALLY_RECV*)lpMsg,aIndex);
			break;
		case 0xB1:
			switch(lpMsg[3])
			{
				case 0x01:
					CGMapServerMoveAuthRecv((PMSG_MAP_SERVER_MOVE_AUTH_RECV*)lpMsg,aIndex);
					break;
			}
			break;
		case 0xB2:
			switch(lpMsg[3])
			{
				case 0x00:
					CGReqCastleSiegeState(aIndex);
					break;
				case 0x01:
					CGReqRegCastleSiege(aIndex);
					break;
				case 0x02:
					CGReqGiveUpCastleSiege((PMSG_REQ_GIVEUPCASTLESIEGE*)lpMsg,aIndex);
					break;
				case 0x03:
					CGReqGuildRegInfo(aIndex);
					break;
				case 0x04:
					CGReqRegGuildMark((PMSG_REQ_REGGUILDMARK*)lpMsg,aIndex);
					break;
				case 0x05:
					CGReqNpcBuy((PMSG_REQ_NPCBUY*)lpMsg,aIndex);
					break;
				case 0x06:
					CGReqNpcRepair((PMSG_REQ_NPCREPAIR*)lpMsg,aIndex);
					break;
				case 0x07:
					CGReqNpcUpgrade((PMSG_REQ_NPCUPGRADE*)lpMsg,aIndex);
					break;
				case 0x08:
					CGReqTaxMoneyInfo(aIndex);
					break;
				case 0x09:
					CGReqTaxRateChange((PMSG_REQ_TAXRATECHANGE*)lpMsg,aIndex);
					break;
				case 0x10:
					CGReqMoneyDrawOut((PMSG_REQ_MONEYDRAWOUT*)lpMsg,aIndex);
					break;
				case 0x12:
					CGReqCsGateOperate((PMSG_REQ_CSGATEOPERATE*)lpMsg,aIndex);
					break;
				case 0x1B:
					CGReqCsMiniMapData(aIndex);
					break;
				case 0x1C:
					CGReqStopCsMiniMapData(aIndex);
					break;
				case 0x1D:
					CGReqCsSendCommand((PMSG_REQ_CSCOMMAND*)lpMsg,aIndex);
					break;
				case 0x1F:
					CGReqCsSetEnterHuntZone((PMSG_REQ_CSHUNTZONEENTER*)lpMsg,aIndex);
					break;
			}
			break;
		case 0xB3:
			CGReqNpcDbList((PMSG_REQ_NPCDBLIST*)lpMsg,aIndex);
			break;
		case 0xB4:
			CGReqCsRegGuildList(aIndex);
			break;
		case 0xB5:
			CGReqCsAttkGuildList(aIndex);
			break;
		case 0xB7:
			switch(lpMsg[3])
			{
				case 0x01:
					gCastleSiegeWeapon.CGCastleSiegeWeaponUseRecv((PMSG_CASTLE_SIEGE_WEAPON_USE_RECV*)lpMsg,aIndex);
					break;
				case 0x04:
					gCastleSiegeWeapon.CGCastleSiegeWeaponDamageRecv((PMSG_CASTLE_SIEGE_WEAPON_DAMAGE_RECV*)lpMsg,aIndex);
					break;
			}
			break;
		case 0xB9:
			switch(lpMsg[3])
			{
				case 0x02:
					CGReqGuildMarkOfCastleOwner(aIndex);
					break;
				case 0x05:
					CGReqCastleHuntZoneEntrance((PMSG_REQ_MOVE_TO_CASTLE_HUNTZONE*)lpMsg,aIndex);
					break;
			}
			break;
		case 0xBC:
			switch(lpMsg[3])
			{
				case 0x00:
					gJewelMix.CGJewelMixRecv((PMSG_JEWEL_MIX_RECV*)lpMsg,aIndex);
					break;
				case 0x01:
					gJewelMix.CGJewelUnMixRecv((PMSG_JEWEL_UNMIX_RECV*)lpMsg,aIndex);
					break;
			}
			break;
		case 0xBD:
			switch(lpMsg[3])
			{
				case 0x00:
					gCrywolf.CGCrywolfInfoRecv(aIndex);
					break;
				case 0x03:
					gCrywolf.CGCrywolfAltarContractRecv((PMSG_CRYWOLF_ALTAR_CONTRACT_RECV*)lpMsg,aIndex);
					break;
				case 0x09:
					gCrywolf.CGCrywolfChaosRateRecv(aIndex);
					break;
			}
			break;
		case 0xBF:
			switch(lpMsg[3])
			{
				case 0x00:
					#if(GAMESERVER_UPDATE>=301 && GAMESERVER_UPDATE<=603)
					gIllusionTemple.CGIllusionTempleEnterRecv((PMSG_ILLUSION_TEMPLE_ENTER_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x02:
					#if(GAMESERVER_UPDATE>=301 && GAMESERVER_UPDATE<=603)
					gIllusionTemple.CGIllusionTempleSkillRecv((PMSG_ILLUSION_TEMPLE_SKILL_RECV*)lpMsg,aIndex);
					#elif(GAMESERVER_UPDATE >= 803)
					gIllusionTempleRenewal.CGIllusionTempleRenewalSkillRecv((PMSG_ILLUSION_TEMPLE_RENEWAL_SKILL_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x05:
					#if(GAMESERVER_UPDATE>=301 && GAMESERVER_UPDATE<=603)
					gIllusionTemple.CGIllusionTempleButtonClickRecv(aIndex);
					#elif(GAMESERVER_UPDATE >= 803)
					gIllusionTempleRenewal.CGIllusionTempleRenewalButtonClickRecv(aIndex);
					#endif
					break;
				case 0x0B:
					#if(GAMESERVER_UPDATE>=402)
					gLuckyCoin.CGLuckyCoinCountRecv((PMSG_LUCKY_COIN_COUNT_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x0C:
					#if(GAMESERVER_UPDATE>=402)
					gLuckyCoin.CGLuckyCoinRegisterRecv((PMSG_LUCKY_COIN_REGISTER_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x0D:
					#if(GAMESERVER_UPDATE>=402)
					gLuckyCoin.CGLuckyCoinExchangeRecv((PMSG_LUCKY_COIN_EXCHANGE_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x0E:
					#if(GAMESERVER_UPDATE>=501)
					gDoubleGoer.CGDoubleGoerEnterRecv((PMSG_DOUBLE_GOER_ENTER_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x17:
					#if(GAMESERVER_UPDATE>=501)
					gNpcTalk.CGNpcJuliaRecv(aIndex);
					#endif
					break;
				case 0x20:
					#if(GAMESERVER_UPDATE>=501)
					gInventoryEquipment.CGInventoryEquipmentRecv((PMSG_INVENTORY_EQUIPMENT_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x51:
					#if(GAMESERVER_UPDATE>=603)
					gHelper.CGHelperStartRecv((PMSG_HELPER_START_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x6A:
					#if(GAMESERVER_UPDATE>=803)
					gIllusionTempleRenewal.CGIllusionTempleRenewalStorageRecv((PMSG_ILLUSION_TEMPLE_RENEWAL_STORAGE_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x70:
					#if(GAMESERVER_UPDATE>=803)
					gIllusionTempleRenewal.CGIllusionTempleRenewalEnterRecv(aIndex);
					#endif
					break;
				case 0x71:
					#if(GAMESERVER_UPDATE>=803)
					//Accept Enter
					#endif
					break;
			}
			break;
		case 0xC0:
			gFriend.CGFriendListRecv(aIndex);
			break;
		case 0xC1:
			gFriend.CGFriendRequestRecv((PMSG_FRIEND_REQUEST_RECV*)lpMsg,aIndex);
			break;
		case 0xC2:
			gFriend.CGFriendResultRecv((PMSG_FRIEND_RESULT_RECV*)lpMsg,aIndex);
			break;
		case 0xC3:
			gFriend.CGFriendDeleteRecv((PMSG_FRIEND_DELETE_RECV*)lpMsg,aIndex);
			break;
		case 0xC5:
			gFriend.CGFriendMailRecv((PMSG_FRIEND_MAIL_RECV*)lpMsg,aIndex);
			break;
		case 0xC7:
			gFriend.CGFriendMailReadRecv((PMSG_FRIEND_MAIL_READ_RECV*)lpMsg,aIndex);
			break;
		case 0xC8:
			gFriend.CGFriendMailDeleteRecv((PMSG_FRIEND_MAIL_DELETE_RECV*)lpMsg,aIndex);
			break;
		case 0xC9:
			gFriend.CGFriendMailListRecv(aIndex);
			break;
		case 0xD0:
			switch(lpMsg[3])
			{
				case 0x05:
					#if(GAMESERVER_SHOP==1)
					gPcPoint.CGPcPointItemBuyRecv((PMSG_PC_POINT_ITEM_BUY_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x06:
					#if(GAMESERVER_SHOP==1)
					gPcPoint.CGPcPointOpenRecv((PMSG_PC_POINT_OPEN_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x07:
					#if(GAMESERVER_UPDATE>=301)
					gQuest.CGQuestNpcWarewolfRecv(aIndex);
					#endif
					break;
				case 0x08:
					#if(GAMESERVER_UPDATE>=301)
					gQuest.CGQuestNpcKeeperRecv(aIndex);
					#endif
					break;
				case 0x09:
					#if(GAMESERVER_UPDATE>=202)
					gNpcTalk.CGNpcLeoTheHelperRecv(aIndex);
					#endif
					break;
				case 0x0A:
					#if(GAMESERVER_UPDATE>=401)
					gNpcTalk.CGNpcSnowmanRecv(aIndex);
					#endif
					break;
				case 0x10:
					#if(GAMESERVER_UPDATE>=401)
					gNpcTalk.CGNpcSantaClausRecv(aIndex);
					#endif
					break;
			}
			break;
		case 0xD1:
			switch(lpMsg[3])
			{
				case 0x00:
					gKanturuEntranceNPC.CGKanturuEnterInfoRecv(aIndex);
					break;
				case 0x01:
					gKanturuEntranceNPC.CGKanturuEnterRecv(aIndex);
					break;
			}
			break;
		#if(GAMESERVER_SHOP==3)
		case 0xD2:
			switch(lpMsg[3])
			{
				case 0x01:
					#if(GAMESERVER_SHOP==3)
					gCashShop.CGCashShopPointRecv(aIndex);
					#endif
					break;
				case 0x02:
					#if(GAMESERVER_SHOP==3)
					gCashShop.CGCashShopOpenRecv((PMSG_CASH_SHOP_OPEN_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x03:
					#if(GAMESERVER_SHOP==3)
					gCashShop.CGCashShopItemBuyRecv((PMSG_CASH_SHOP_ITEM_BUY_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x04:
					#if(GAMESERVER_SHOP==3)
					gCashShop.CGCashShopItemGifRecv((PMSG_CASH_SHOP_ITEM_GIF_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x05:
					#if(GAMESERVER_SHOP==3)
					gCashShop.CGCashShopItemNumRecv((PMSG_CASH_SHOP_ITEM_NUM_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x0B:
					#if(GAMESERVER_SHOP==3)
					gCashShop.CGCashShopItemUseRecv((PMSG_CASH_SHOP_ITEM_USE_RECV*)lpMsg,aIndex);
					#endif
					break;
			}
			break;
		#endif
		case PROTOCOL_CODE1:
			CGMoveRecv((PMSG_MOVE_RECV*)lpMsg,aIndex);
			break;
		case PROTOCOL_CODE4:
			gSkillManager.CGMultiSkillAttackRecv((PMSG_MULTI_SKILL_ATTACK_RECV*)lpMsg,aIndex,0);
			break;
		case 0xE1:
			gGuild.CGGuildAssignStatusRecv((PMSG_GUILD_ASSIGN_STATUS_RECV*)lpMsg,aIndex);
			break;
		case 0xE2:
			gGuild.CGGuildAssignTypeRecv((PMSG_GUILD_ASSIGN_TYPE_RECV*)lpMsg,aIndex);
			break;
		case 0xE5:
			gGuild.CGGuildUnionRequestRecv((PMSG_GUILD_UNION_REQUEST_RECV*)lpMsg,aIndex);
			break;
		case 0xE6:
			gGuild.CGGuildUnionResultRecv((PMSG_GUILD_UNION_RESULT_RECV*)lpMsg,aIndex);
			break;
		case 0xE7:
			switch(lpMsg[3])
			{
				case 0x01:
					#if(GAMESERVER_UPDATE>=802)
					gMiniMap.CGMiniMapStartPartyInfoRecv(aIndex);
					#endif
					break;
				case 0x02:
					#if(GAMESERVER_UPDATE>=802)
					gMiniMap.CGMiniMapClosePartyInfoRecv(aIndex);
					#endif
					break;
				case 0x03:
					#if(GAMESERVER_UPDATE>=802)
					gMiniMap.CGMiniMapInfoRecv((PMSG_MINI_MAP_INFO_RECV*)lpMsg,aIndex);
					#endif
					break;
			}
			break;
		case 0xE9:
			gGuild.CGGuildUnionListRecv(aIndex);
			break;
		case 0xEB:
			gGuild.CGGuildAllianceKickRecv((PMSG_GUILD_ALLIANCE_KICK_RECV*)lpMsg,aIndex);
			break;
		case 0xEC:
			switch(lpMsg[3])
			{
				case 0x00:
					#if(GAMESERVER_UPDATE>=701)
					gPentagramSystem.CGPentagramJewelInsertRecv((PMSG_PENTAGRAM_JEWEL_INSERT_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x01:
					#if(GAMESERVER_UPDATE>=701)
					gPentagramSystem.CGPentagramJewelRemoveRecv((PMSG_PENTAGRAM_JEWEL_REMOVE_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x02:
					#if(GAMESERVER_UPDATE>=701)
					gPentagramSystem.CGPentagramJewelRefineRecv((PMSG_PENTAGRAM_JEWEL_REFINE_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x03:
					#if(GAMESERVER_UPDATE>=701)
					gPentagramSystem.CGPentagramJewelUpgradeRecv((PMSG_PENTAGRAM_JEWEL_UPGRADE_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x31:
					#if(GAMESERVER_UPDATE>=802)
					gPersonalShop.CGPShopSearchRecv((PMSG_PSHOP_SEARCH_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x33:
					#if(GAMESERVER_UPDATE>=802)
					gPersonalShop.CGPShopSearchLogRecv((PMSG_PSHOP_SEARCH_LOG_RECV*)lpMsg,aIndex);
					#endif
					break;
			}
			break;
		case 0xED:
			switch(lpMsg[3])
			{
				case 0x00:
					#if(GAMESERVER_UPDATE>=801)
					gGuildMatching.CGGuildMatchingListRecv((PMSG_GUILD_MATCHING_LIST_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x01:
					#if(GAMESERVER_UPDATE>=801)
					gGuildMatching.CGGuildMatchingListSearchRecv((PMSG_GUILD_MATCHING_LIST_SEARCH_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x02:
					#if(GAMESERVER_UPDATE>=801)
					gGuildMatching.CGGuildMatchingInsertRecv((PMSG_GUILD_MATCHING_INSERT_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x03:
					#if(GAMESERVER_UPDATE>=801)
					gGuildMatching.CGGuildMatchingCancelRecv((PMSG_GUILD_MATCHING_CANCEL_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x04:
					#if(GAMESERVER_UPDATE>=801)
					gGuildMatching.CGGuildMatchingJoinInsertRecv((PMSG_GUILD_MATCHING_JOIN_INSERT_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x05:
					#if(GAMESERVER_UPDATE>=801)
					gGuildMatching.CGGuildMatchingJoinCancelRecv((PMSG_GUILD_MATCHING_JOIN_CANCEL_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x06:
					#if(GAMESERVER_UPDATE>=801)
					gGuildMatching.CGGuildMatchingJoinAcceptRecv((PMSG_GUILD_MATCHING_JOIN_ACCEPT_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x07:
					#if(GAMESERVER_UPDATE>=801)
					gGuildMatching.CGGuildMatchingJoinListRecv((PMSG_GUILD_MATCHING_JOIN_LIST_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x08:
					#if(GAMESERVER_UPDATE>=801)
					gGuildMatching.CGGuildMatchingJoinInfoRecv((PMSG_GUILD_MATCHING_JOIN_INFO_RECV*)lpMsg,aIndex);
					#endif
					break;
			}
			break;
		case 0xEF:
			switch(lpMsg[3])
			{
				case 0x00:
					#if(GAMESERVER_UPDATE>=801)
					gPartyMatching.CGPartyMatchingInsertRecv((PMSG_PARTY_MATCHING_INSERT_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x01:
					#if(GAMESERVER_UPDATE>=801)
					gPartyMatching.CGPartyMatchingListRecv((PMSG_PARTY_MATCHING_LIST_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x02:
					#if(GAMESERVER_UPDATE>=801)
					gPartyMatching.CGPartyMatchingJoinInsertRecv((PMSG_PARTY_MATCHING_JOIN_INSERT_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x03:
					#if(GAMESERVER_UPDATE>=801)
					gPartyMatching.CGPartyMatchingJoinInfoRecv((PMSG_PARTY_MATCHING_JOIN_INFO_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x04:
					#if(GAMESERVER_UPDATE>=801)
					gPartyMatching.CGPartyMatchingJoinListRecv((PMSG_PARTY_MATCHING_JOIN_LIST_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x05:
					#if(GAMESERVER_UPDATE>=801)
					gPartyMatching.CGPartyMatchingJoinAcceptRecv((PMSG_PARTY_MATCHING_JOIN_ACCEPT_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x06:
					#if(GAMESERVER_UPDATE>=801)
					gPartyMatching.CGPartyMatchingJoinCancelRecv((PMSG_PARTY_MATCHING_JOIN_CANCEL_RECV*)lpMsg,aIndex);
					#endif
					break;
			}
			break;
		case 0xF1:
			switch(lpMsg[3])
			{
				case 0x01:
					CGConnectAccountRecv((PMSG_CONNECT_ACCOUNT_RECV*)lpMsg,aIndex);
					break;
				case 0x02:
					CGCloseClientRecv((PMSG_CLOSE_CLIENT_RECV*)lpMsg,aIndex);
					break;
			}
			break;
		case 0xF3:
			switch(lpMsg[3])
			{
				case 0x00:
					CGCharacterListRecv(aIndex);
					break;
				case 0x01:
					CGCharacterCreateRecv((PMSG_CHARACTER_CREATE_RECV*)lpMsg,aIndex);
					break;
				case 0x02:
					CGCharacterDeleteRecv((PMSG_CHARACTER_DELETE_RECV*)lpMsg,aIndex);
					break;
				case 0x03:
					CGCharacterInfoRecv((PMSG_CHARACTER_INFO_RECV*)lpMsg,aIndex);
					break;
				case 0x06:
					CGLevelUpPointRecv((PMSG_LEVEL_UP_POINT_RECV*)lpMsg,aIndex);
					break;
				case 0x09:
					gConnectionManager.CGHardwareIdRecv((PMSG_HARDWARE_ID_INFO_RECV*)lpMsg,aIndex);
					break;
				case 0x12:
					CGCharacterMoveViewportEnableRecv(aIndex);
					break;
				case 0x15:
					#if(GAMESERVER_UPDATE>=401)
					CGCharacterNameCheckRecv((PMSG_CHARACTER_NAME_CHECK_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x16:
					#if(GAMESERVER_UPDATE>=401)
					CGCharacterNameChangeRecv((PMSG_CHARACTER_NAME_CHANGE_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x21:
					#if(GAMESERVER_UPDATE>=701)
					CGOptionChangeSkinRecv((PMSG_OPTION_CHANGE_SKIN_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x30:
					CGOptionDataRecv((PMSG_OPTION_DATA_RECV*)lpMsg,aIndex);
					break;
				case 0x31:
					#if(GAMESERVER_UPDATE>=701)
					CGClientSecurityBreachRecv((PMSG_CLIENT_SECURITY_BREACH_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x52:
					#if(GAMESERVER_UPDATE>=401)
					gMasterSkillTree.CGMasterSkillRecv((PMSG_MASTER_SKILL_RECV*)lpMsg,aIndex);
					#endif
					break;
			}
			break;
		#if(GAMESERVER_SHOP==2)
		case 0xF5:
			switch(lpMsg[3])
			{
				case 0x01:
					#if(GAMESERVER_SHOP==2)
					gMuCashShop.CGMuCashShopOpenRecv((PMSG_MU_CASH_SHOP_OPEN_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x03:
					#if(GAMESERVER_SHOP==2)
					gMuCashShop.GDMuCashPointSend(aIndex);
					#endif
					break;
				case 0x05:
					#if(GAMESERVER_SHOP==2)
					gMuCashShop.CGMuCashShopItemRecv((PMSG_MU_CASH_SHOP_ITEM_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x07:
					#if(GAMESERVER_SHOP==2)
					gMuCashShop.CGMuCashShopItemBuyRecv((PMSG_MU_CASH_SHOP_BUY_RECV*)lpMsg,aIndex);
					#endif
					break;
			}
			break;
		#endif
		case 0xF6:
			switch(lpMsg[3])
			{
				case 0x0A:
					#if(GAMESERVER_UPDATE>=501)
					gQuestWorld.CGQuestWorldDialogRecv((PMSG_QUEST_WORLD_DIALOG_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x0B:
					#if(GAMESERVER_UPDATE>=501)
					gQuestWorld.CGQuestWorldAcceptRecv((PMSG_QUEST_WORLD_ACCEPT_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x0D:
					#if(GAMESERVER_UPDATE>=501)
					gQuestWorld.CGQuestWorldFinishRecv((PMSG_QUEST_WORLD_FINISH_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x0F:
					#if(GAMESERVER_UPDATE>=501)
					gQuestWorld.CGQuestWorldCancelRecv((PMSG_QUEST_WORLD_CANCEL_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x10:
					#if(GAMESERVER_UPDATE>=501)
					gQuestWorld.CGQuestWorldButtonRecv((PMSG_QUEST_WORLD_BUTTON_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x1B:
					#if(GAMESERVER_UPDATE>=501)
					gQuestWorld.CGQuestWorldDetailRecv((PMSG_QUEST_WORLD_DETAIL_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x30:
					#if(GAMESERVER_UPDATE>=501)
					gQuestWorld.CGQuestWorldListRecv(aIndex);
					#endif
					break;
				case 0x31:
					#if(GAMESERVER_UPDATE>=501)
					gQuestWorld.CGQuestWorldNpcElfBufferRecv(aIndex);
					#endif
					break;
			}
			break;
		case 0xF7:
			switch(lpMsg[3])
			{
				case 0x01:
					#if(GAMESERVER_UPDATE>=501)
					gImperialGuardian.CGImperialGuardianEnterRecv((PMSG_IMPERIAL_GUARDIAN_ENTER_RECV*)lpMsg,aIndex);
					#endif
					break;
			}
			break;
		case 0xF8:
			switch(lpMsg[3])
			{
				case 0x01:
					#if(GAMESERVER_UPDATE>=501)
					gGensSystem.CGGensSystemInsertRecv((PMSG_GENS_SYSTEM_INSERT_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x03:
					#if(GAMESERVER_UPDATE>=501)
					gGensSystem.CGGensSystemDeleteRecv((PMSG_GENS_SYSTEM_DELETE_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x09:
					#if(GAMESERVER_UPDATE>=501)
					gGensSystem.CGGensSystemRewardRecv((PMSG_GENS_SYSTEM_REWARD_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x0B:
					#if(GAMESERVER_UPDATE>=501)
					gGensSystem.CGGensSystemMemberRecv((PMSG_GENS_SYSTEM_MEMBER_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x20:
					#if(GAMESERVER_UPDATE>=701)
					CGAcheronEnterRecv(aIndex);
					#endif
					break;
				case 0x30:
					#if(GAMESERVER_UPDATE>=702)
					gArcaBattle.CGArcaBattleGuildMasterRegisterRecv(aIndex);
					#endif
					break;
				case 0x32:
					#if(GAMESERVER_UPDATE>=702)
					gArcaBattle.CGArcaBattleGuildMemberRegisterRecv(aIndex);
					#endif
					break;
				case 0x34:
					#if(GAMESERVER_UPDATE>=702)
					gArcaBattle.CGArcaBattleEnterRecv((PMSG_ARCA_BATTLE_ENTER_RESULT_RECV*)lpMsg,aIndex);
					#endif
					break;
				case 0x36:
					#if(GAMESERVER_UPDATE>=702)
					gArcaBattle.CGArcaBattleTrophiesRecv(aIndex);
					#endif
					break;
				case 0x41:
					#if(GAMESERVER_UPDATE>=702)
					gArcaBattle.CGArcaBattleMemberCountRecv(aIndex);
					#endif
					break;
				case 0x43:
					#if(GAMESERVER_UPDATE>=702)
					gArcaBattle.CGArcaBattleRegisterMarkRecv(aIndex);
					#endif
					break;
				case 0x45:
					#if(GAMESERVER_UPDATE>=702)
					gArcaBattle.CGArcaBattleMarkRankRecv(aIndex);
					#endif
					break;
				case 0x4B:
					#if(GAMESERVER_UPDATE>=801)
					CGAcheronGuardianEnterRecv(aIndex);
					#endif
					break;
			}
			break;
		case 0xFB:
			#if(GAMESERVER_UPDATE>=801)
			CGSNSDataRecv((PMSG_SNS_DATA_RECV*)lpMsg,aIndex);
			#endif
			break;
		case 0xFC:
			#if(GAMESERVER_UPDATE>=801)
			CGSNSDataLogRecv((PMSG_SNS_DATA_LOG_RECV*)lpMsg,aIndex);
			#endif
			break;
	}

	PROTECT_FINAL
}

void CGChatRecv(PMSG_CHAT_RECV* lpMsg,int aIndex) // OK
{
	if(gMapManager.GetMapChatEnable(gObj[aIndex].Map) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	char name[11] = {0};

	memcpy(name,lpMsg->name,sizeof(lpMsg->name));

	if(strcmp(name,lpObj->Name) != 0)
	{
		return;
	}

	lpMsg->message[(sizeof(lpMsg->message)-1)] = 0;

	gLog.Output(LOG_CHAT,"[General][%s][%s] - (Message: %s)",lpObj->Account,lpObj->Name,lpMsg->message);

	if(gFilter.CheckSyntax(lpMsg->message) != 0)
	{
		gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(723));
		return;
	}

	if(lpMsg->message[0] == '/')
	{
		gCommandManager.ManagementCore(lpObj,lpMsg->message);
		return;
	}

	if(lpObj->ChatLimitTime > 0)
	{
		gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(269),lpObj->ChatLimitTime);
		return;
	}

	if((lpObj->Penalty & 2) != 0)
	{
		return;
	}

	if(lpObj->Permission[11] != 0)
	{
		return;
	}

	PMSG_CHAT_SEND pMsg;

	pMsg.header.set(0x00,sizeof(pMsg));

	memcpy(pMsg.name,lpMsg->name,sizeof(pMsg.name));

	memcpy(pMsg.message,lpMsg->message,sizeof(pMsg.message));

	if(lpMsg->message[0] == '!' && gGameMaster.GetGameMasterLevel(lpObj) != -1)
	{
		GDGlobalNoticeSend(gMapServerManager.GetMapServerGroup(),0,0,0,0,0,0,&lpMsg->message[1]);
	}
	else if(lpMsg->message[0] == '~')
	{
		if(OBJECT_RANGE(lpObj->PartyNumber) != 0)
		{
			for(int n=0;n < MAX_PARTY_USER;n++)
			{
				if(OBJECT_RANGE(gParty.m_PartyInfo[lpObj->PartyNumber].Index[n]) != 0)
				{
					DataSend(gParty.m_PartyInfo[lpObj->PartyNumber].Index[n],(BYTE*)&pMsg,pMsg.header.size);
				}
			}
		}
	}
	else if(lpMsg->message[0] == '@')
	{
		if(lpObj->Guild != 0)
		{
			if(lpMsg->message[1] == '>')
			{
				if(lpObj->GuildRank == GUILD_RANK_MASTER)
				{
					gGuild.GDGuildGlobalNoticeSend(lpObj->Guild->Name,&lpMsg->message[2]);
				}
			}
			else if(lpMsg->message[1] == '@')
			{
				if(lpObj->Guild->GuildUnion != 0)
				{
					gGuild.GDGuildGlobalChatSend(1,lpObj->Guild->Name,lpObj->Name,lpMsg->message);
				}
			}
			else
			{
				gGuild.GDGuildGlobalChatSend(0,lpObj->Guild->Name,lpObj->Name,lpMsg->message);
			}
		}
	}
	#if(GAMESERVER_UPDATE>=501)
	else if(lpMsg->message[0] == '$')
	{
		if(lpObj->GensFamily != 0)
		{
			for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
			{
				if(gObj[n].GensFamily == lpObj->GensFamily)
				{
					DataSend(n,(BYTE*)&pMsg,pMsg.header.size);
				}
			}
		}
	}
	#endif
	else
	{
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
		MsgSendV2(lpObj,(BYTE*)&pMsg,pMsg.header.size);
	}
}

void CGChatWhisperRecv(PMSG_CHAT_WHISPER_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	char name[11] = {0};

	memcpy(name,lpMsg->name,sizeof(lpMsg->name));

	if(lpObj->ChatLimitTime > 0)
	{
		gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(269),lpObj->ChatLimitTime);
		return;
	}

	if((lpObj->Penalty & 2) != 0)
	{
		return;
	}

	LPOBJ lpTarget = gObjFind(name);

	if(lpTarget == 0)
	{
		GDGlobalWhisperSend(aIndex,name,lpMsg->message);
		return;
	}

	if(aIndex == lpTarget->Index)
	{
		gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(270));
		return;
	}

	lpMsg->message[(sizeof(lpMsg->message)-1)] = 0;

	gLog.Output(LOG_CHAT,"[Whisper][%s][%s] - (Name: %s,Message: %s)",lpObj->Account,lpObj->Name,name,lpMsg->message);

	if(gFilter.CheckSyntax(lpMsg->message) != 0)
	{
		gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(723));
		return;
	}

	GCChatWhisperSend(lpTarget->Index,lpObj->Name,lpMsg->message);
}

void CGMainCheckRecv(PMSG_MAIN_CHECK_RECV* lpMsg,int aIndex) // OK
{
	
}

void CGLiveClientRecv(PMSG_LIVE_CLIENT_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Connected != OBJECT_LOGGED && lpObj->Connected != OBJECT_ONLINE)
	{
		return;
	}

	if(gServerInfo.m_CheckLatencyHack != 0 && abs((int)((GetTickCount()-lpObj->ServerTickCount)-(lpMsg->TickCount-lpObj->ClientTickCount))) > gServerInfo.m_CheckLatencyHackTolerance)
	{
		gLog.Output(LOG_HACK,"[%s][%s] Client Latency Hack Detected [%d][%d]",lpObj->Account,lpObj->Name,(GetTickCount()-lpObj->ServerTickCount),(lpMsg->TickCount-lpObj->ClientTickCount));
	
		if(gServerInfo.m_CheckLatencyHackAction == 1)
		{
			gObjUserKill(aIndex);
		}
		else if(gServerInfo.m_CheckLatencyHackAction == 2)
		{
			GCCloseClientSend(aIndex,0);
		}

		return;
	}

	if(lpObj->Connected == OBJECT_ONLINE && gServerInfo.m_CheckSpeedHack != 0 && lpMsg->PhysiSpeed > lpObj->PhysiSpeed && abs((int)(lpObj->PhysiSpeed-lpMsg->PhysiSpeed)) > gServerInfo.m_CheckSpeedHackTolerance)
	{
		gLog.Output(LOG_HACK,"[%s][%s] Client Speed Hack Detected [%d][%d]",lpObj->Account,lpObj->Name,lpObj->PhysiSpeed,lpMsg->PhysiSpeed);

		if(gServerInfo.m_CheckSpeedHackAction == 1)
		{
			gObjUserKill(aIndex);
		}
		else if(gServerInfo.m_CheckSpeedHackAction == 2)
		{
			GCCloseClientSend(aIndex,0);
		}

		return;
	}

	if(lpObj->Connected == OBJECT_ONLINE && gServerInfo.m_CheckSpeedHack != 0 && lpMsg->MagicSpeed > lpObj->MagicSpeed && abs((int)(lpObj->MagicSpeed-lpMsg->MagicSpeed)) > gServerInfo.m_CheckSpeedHackTolerance)
	{
		gLog.Output(LOG_HACK,"[%s][%s] Client Speed Hack Detected [%d][%d]",lpObj->Account,lpObj->Name,lpObj->MagicSpeed,lpMsg->MagicSpeed);

		if(gServerInfo.m_CheckSpeedHackAction == 1)
		{
			gObjUserKill(aIndex);
		}
		else if(gServerInfo.m_CheckSpeedHackAction == 2)
		{
			GCCloseClientSend(aIndex,0);
		}

		return;
	}

	lpObj->ConnectTickCount = GetTickCount();
}

void CGPositionRecv(PMSG_POSITION_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGS(aIndex) == 0)
	{
		return;
	}

	if(lpObj->Teleport != TELEPORT_NONE)
	{
		return;
	}

	if(gObjCheckMapTile(lpObj,2) != 0)
	{
		return;
	}

	#if(GAMESERVER_UPDATE>=803)

	if((CC_MAP_RANGE(lpObj->Map) != 0 || CF_MAP_RANGE(lpObj->Map) != 0) && (GetTickCount()-lpObj->ChaosCastleBlowTime) < 1000)
	{
		return;
	}

	#else

	if(CC_MAP_RANGE(lpObj->Map) != 0 && (GetTickCount()-lpObj->ChaosCastleBlowTime) < 1000)
	{
		return;
	}

	#endif

	if(lpMsg->x < (lpObj->X-15) || lpMsg->x > (lpObj->X+15) || lpMsg->y < (lpObj->Y-15) || lpMsg->y > (lpObj->Y+15))
	{
		return;
	}

	gMap[lpObj->Map].DelStandAttr(lpObj->OldX,lpObj->OldY);

	lpObj->X = lpMsg->x;
	lpObj->Y = lpMsg->y;
	lpObj->TX = lpMsg->x;
	lpObj->TY = lpMsg->y;
	lpObj->OldX = lpMsg->x;
	lpObj->OldY = lpMsg->y;

	gMap[lpObj->Map].SetStandAttr(lpObj->TX,lpObj->TY);

	PMSG_POSITION_SEND pMsg;

	pMsg.header.set(PROTOCOL_CODE3,sizeof(pMsg));

	pMsg.index[0] = SET_NUMBERHB(aIndex);

	pMsg.index[1] = SET_NUMBERLB(aIndex);

	pMsg.x = (BYTE)lpObj->TX;

	pMsg.y = (BYTE)lpObj->TY;

	if(lpObj->Type == OBJECT_USER)
	{
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
	}

	for(int n=0;n < MAX_VIEWPORT;n++)
	{
		if(lpObj->VpPlayer2[n].type == OBJECT_USER)
		{
			if(lpObj->VpPlayer2[n].state != OBJECT_EMPTY && lpObj->VpPlayer2[n].state != OBJECT_DIECMD && lpObj->VpPlayer2[n].state != OBJECT_DIED)
			{
				DataSend(lpObj->VpPlayer2[n].index,(BYTE*)&pMsg,pMsg.header.size);
			}
		}
	}
}

void CGActionRecv(PMSG_ACTION_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	lpObj->Dir = lpMsg->dir;

	lpObj->ActionNumber = lpMsg->action;

	if(lpObj->ActionNumber == ACTION_SIT1)
	{
		lpObj->ViewState = VIEW_STATE_ACTION_1;
		lpObj->Rest = lpObj->ActionNumber;
	}
	else if(lpObj->ActionNumber == ACTION_POSE1)
	{
		lpObj->ViewState = VIEW_STATE_ACTION_2;
		lpObj->Rest = lpObj->ActionNumber;
	}
	else if(lpObj->ActionNumber == ACTION_HEALING1)
	{
		lpObj->ViewState = VIEW_STATE_ACTION_3;
		lpObj->Rest = lpObj->ActionNumber;
	}

	PMSG_ACTION_SEND pMsg;

	pMsg.header.set(0x18,sizeof(pMsg));

	pMsg.index[0] = SET_NUMBERHB(aIndex);

	pMsg.index[1] = SET_NUMBERLB(aIndex);

	pMsg.dir = lpMsg->dir;

	pMsg.action = lpMsg->action;

	pMsg.target[0] = lpMsg->index[0];

	pMsg.target[1] = lpMsg->index[1];
	
	for(int n=0;n < MAX_VIEWPORT;n++)
	{
		if(lpObj->VpPlayer2[n].type == OBJECT_USER)
		{
			if(lpObj->VpPlayer2[n].state != OBJECT_EMPTY && lpObj->VpPlayer2[n].state != OBJECT_DIECMD && lpObj->VpPlayer2[n].state != OBJECT_DIED)
			{
				DataSend(lpObj->VpPlayer2[n].index,(BYTE*)&pMsg,pMsg.header.size);
			}
		}
	}
}

void CGEventRemainTimeRecv(PMSG_EVENT_REMAIN_TIME_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	PMSG_EVENT_REMAIN_TIME_SEND pMsg;

	pMsg.header.set(0x91,sizeof(pMsg));

	pMsg.EventType = lpMsg->EventType;

	pMsg.RemainTimeH = 0;

	pMsg.EnteredUser = 0;

	pMsg.RemainTimeL = 0;

	switch(lpMsg->EventType)
	{
		case 1:
		{
			lpMsg->ItemLevel = gEventEntryLevel.GetDSLevel(&gObj[aIndex]);

			if(DS_LEVEL_RANGE(lpMsg->ItemLevel) != 0)
			{
				if(gDevilSquare.GetState(lpMsg->ItemLevel) == DS_STATE_EMPTY)
				{
					if(gDevilSquare.GetEnterEnabled(lpMsg->ItemLevel) == 0)
					{
						pMsg.RemainTimeH = gDevilSquare.GetRemainTime(lpMsg->ItemLevel);
					}
					else
					{
						pMsg.EnteredUser = gDevilSquare.GetEnteredUserCount(lpMsg->ItemLevel);
					}
				}
				else
				{
					pMsg.RemainTimeH = gDevilSquare.GetRemainTime(lpMsg->ItemLevel);
				}
			}
			else
			{
				GCNewMessageSend(aIndex,0,0,"%s",gMessage.GetMessage(35));
				return;
			}
		}break;
		case 2:
		{
			lpMsg->ItemLevel = gEventEntryLevel.GetBCLevel(&gObj[aIndex]);

			if(BC_LEVEL_RANGE(lpMsg->ItemLevel) != 0)
			{
				if(gBloodCastle.GetState(lpMsg->ItemLevel) == BC_STATE_EMPTY)
				{
					if(gBloodCastle.GetEnterEnabled(lpMsg->ItemLevel) == 0)
					{
						pMsg.RemainTimeH = gBloodCastle.GetRemainTime(lpMsg->ItemLevel);
					}
					else
					{
						pMsg.EnteredUser = gBloodCastle.GetEnteredUserCount(lpMsg->ItemLevel);
					}
				}
				else
				{
					pMsg.RemainTimeH = gBloodCastle.GetRemainTime(lpMsg->ItemLevel);
				}
			}
			else
			{
				GCNewMessageSend(aIndex,0,0,"%s",gMessage.GetMessage(7));
				return;
			}
		}break;
		case 4:
		{
			lpMsg->ItemLevel = gEventEntryLevel.GetCCLevel(&gObj[aIndex]);

			if(CC_LEVEL_RANGE(lpMsg->ItemLevel) != 0)
			{
				if(gChaosCastle.GetState(lpMsg->ItemLevel) == CC_STATE_EMPTY)
				{
					if(gChaosCastle.GetEnterEnabled(lpMsg->ItemLevel) == 0)
					{
						pMsg.RemainTimeL = gChaosCastle.GetRemainTime(lpMsg->ItemLevel);
					}
					else
					{
						pMsg.EnteredUser = gChaosCastle.GetEnteredUserCount(lpMsg->ItemLevel);
					}
				}
				else
				{
					pMsg.RemainTimeL = gChaosCastle.GetRemainTime(lpMsg->ItemLevel);
				}
			}
			else
			{
				GCNewMessageSend(aIndex,0,0,"%s",gMessage.GetMessage(22));
				return;
			}
		}break;
		case 5:
		{
			lpMsg->ItemLevel = gEventEntryLevel.GetITLevel(&gObj[aIndex]);

			if(IT_LEVEL_RANGE(lpMsg->ItemLevel) != 0)
			{
				if(gIllusionTemple.GetState(lpMsg->ItemLevel) == IT_STATE_EMPTY)
				{
					if(gIllusionTemple.GetEnterEnabled(lpMsg->ItemLevel) == 0)
					{
						pMsg.RemainTimeH = gIllusionTemple.GetRemainTime(lpMsg->ItemLevel);
					}
					else
					{
						pMsg.EnteredUser = gIllusionTemple.GetEnteredUserCount(lpMsg->ItemLevel);
					}
				}
				else
				{
					pMsg.RemainTimeH = gIllusionTemple.GetRemainTime(lpMsg->ItemLevel);
				}
			}
			else
			{
				GCNewMessageSend(aIndex,0,0,"%s",gMessage.GetMessage(45));
				return;
			}
		}break;
	}

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void CGPetItemCommandRecv(PMSG_PET_ITEM_COMMAND_RECV* lpMsg,int aIndex) // OK
{
	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(lpMsg->type != 0)
	{
		return;
	}

	int bIndex = MAKE_NUMBERW(lpMsg->index[0],lpMsg->index[1]);

	gDarkSpirit[aIndex].ChangeCommand(lpMsg->command,bIndex);
}

void CGPetItemInfoRecv(PMSG_PET_ITEM_INFO_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(lpMsg->type != 0 && lpMsg->type != 1)
	{
		return;
	}

	if(lpMsg->flag == 0) // Inventory
	{
		if(INVENTORY_RANGE(lpMsg->slot) == 0)
		{
			return;
		}

		GCPetItemInfoSend(aIndex,lpMsg->type,lpMsg->flag,lpMsg->slot,lpObj->Inventory[lpMsg->slot].m_PetItemLevel,lpObj->Inventory[lpMsg->slot].m_PetItemExp,(BYTE)lpObj->Inventory[lpMsg->slot].m_Durability);
	}
	else if(lpMsg->flag == 1) // Warehouse
	{
		if(WAREHOUSE_RANGE(lpMsg->slot) == 0)
		{
			return;
		}

		if(lpObj->LoadWarehouse == 0)
		{
			return;
		}

		GCPetItemInfoSend(aIndex,lpMsg->type,lpMsg->flag,lpMsg->slot,lpObj->Warehouse[lpMsg->slot].m_PetItemLevel,lpObj->Warehouse[lpMsg->slot].m_PetItemExp,(BYTE)lpObj->Warehouse[lpMsg->slot].m_Durability);
	}
	else if(lpMsg->flag == 2) // Trade
	{
		if(TRADE_RANGE(lpMsg->slot) == 0)
		{
			return;
		}

		if(lpObj->Interface.use == 0 || lpObj->Interface.type != INTERFACE_TRADE || lpObj->Interface.state == 0)
		{
			return;
		}

		GCPetItemInfoSend(aIndex,lpMsg->type,lpMsg->flag,lpMsg->slot,lpObj->Trade[lpMsg->slot].m_PetItemLevel,lpObj->Trade[lpMsg->slot].m_PetItemExp,(BYTE)lpObj->Trade[lpMsg->slot].m_Durability);
	}
	else if(lpMsg->flag == 3) // Target Trade
	{
		if(TRADE_RANGE(lpMsg->slot) == 0)
		{
			return;
		}

		if(lpObj->Interface.use == 0 || lpObj->Interface.type != INTERFACE_TRADE || lpObj->Interface.state == 0)
		{
			return;
		}

		if(OBJECT_RANGE(lpObj->TargetNumber) == 0)
		{
			return;
		}

		LPOBJ lpTarget = &gObj[lpObj->TargetNumber];

		if(lpTarget->Interface.use == 0 || lpTarget->Interface.type != INTERFACE_TRADE || lpTarget->Interface.state == 0)
		{
			return;
		}

		GCPetItemInfoSend(aIndex,lpMsg->type,lpMsg->flag,lpMsg->slot,lpTarget->Trade[lpMsg->slot].m_PetItemLevel,lpTarget->Trade[lpMsg->slot].m_PetItemExp,(BYTE)lpTarget->Trade[lpMsg->slot].m_Durability);
	}
	else if(lpMsg->flag == 4) // Chaos Box
	{
		if(CHAOS_BOX_RANGE(lpMsg->slot) == 0)
		{
			return;
		}

		GCPetItemInfoSend(aIndex,lpMsg->type,lpMsg->flag,lpMsg->slot,lpObj->ChaosBox[lpMsg->slot].m_PetItemLevel,lpObj->ChaosBox[lpMsg->slot].m_PetItemExp,(BYTE)lpObj->ChaosBox[lpMsg->slot].m_Durability);
	}
	else if(lpMsg->flag == 5) // Target Personal Shop
	{
		if(INVENTORY_SHOP_RANGE(lpMsg->slot) == 0)
		{
			return;
		}

		if(OBJECT_RANGE(lpObj->PShopDealerIndex) == 0)
		{
			return;
		}

		LPOBJ lpTarget = &gObj[lpObj->PShopDealerIndex];

		GCPetItemInfoSend(aIndex,lpMsg->type,lpMsg->flag,lpMsg->slot,lpTarget->Inventory[lpMsg->slot].m_PetItemLevel,lpTarget->Inventory[lpMsg->slot].m_PetItemExp,(BYTE)lpTarget->Inventory[lpMsg->slot].m_Durability);
	}
}

void CGMapServerMoveAuthRecv(PMSG_MAP_SERVER_MOVE_AUTH_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Connected != OBJECT_CONNECTED)
	{
		CloseClient(aIndex);
		return;
	}

	if(memcmp(gServerInfo.m_ServerVersion,lpMsg->ClientVersion,sizeof(lpMsg->ClientVersion)) != 0)
	{
		GCMapServerMoveAuthSend(aIndex,6);
		return;
	}

	if(memcmp(gServerInfo.m_ServerSerial,lpMsg->ClientSerial,sizeof(lpMsg->ClientSerial)) != 0)
	{
		GCMapServerMoveAuthSend(aIndex,6);
		return;
	}

	if(lpObj->LoginMessageSend == 0)
	{
		lpObj->LoginMessageSend++;
		lpObj->LoginMessageCount++;
		lpObj->ConnectTickCount = GetTickCount();
		lpObj->ClientTickCount = lpMsg->TickCount;
		lpObj->ServerTickCount = GetTickCount();

		char account[11] = {0};

		PacketArgumentDecrypt(account,lpMsg->account,sizeof(account));

		lpObj->MapServerMoveRequest = 1;

		GJMapServerMoveAuthSend(aIndex,account,lpMsg->name,lpMsg->AuthCode1,lpMsg->AuthCode2,lpMsg->AuthCode3,lpMsg->AuthCode4);
	}
}

void CGMoveRecv(PMSG_MOVE_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGS(aIndex) == 0)
	{
		return;
	}

	if(lpObj->RegenOk > 0)
	{
		return;
	}

	if(lpObj->Teleport != TELEPORT_NONE)
	{
		lpObj->LastReset = 1;
		return;
	}

	if(gObjCheckMapTile(lpObj,1) != 0)
	{
		return;
	}

	if(lpObj->Type == OBJECT_USER && lpObj->Helper.Offline == 0)
	{
		if(gServerInfo.m_CheckMoveHack == 1)
		{
			if(lpObj->LastReset != 0 || ((GetTickCount()-lpObj->LastMoveTime) > (DWORD)gServerInfo.m_CheckMoveHackMaxDelay))
			{
				lpObj->LastMoveTime = GetTickCount();
				lpObj->LastX = lpObj->TX;
				lpObj->LastY = lpObj->TY;
				lpObj->LastReset = 0;
			}

			int Contador = abs((lpObj->LastX-lpObj->TX)+(lpObj->LastY-lpObj->TY));

			if(Contador > gServerInfo.m_CheckMoveHackMaxCount)
			{
				gLog.Output(LOG_HACK,"[HackCheckMoveMaxDelay][%s][%s] MaxCount error (Count: [%d][%d])",lpObj->Account,lpObj->Name,Contador,gServerInfo.m_CheckMoveHackMaxCount);

				if(gServerInfo.m_CheckMoveHackAction == 1)
				{
					gObjUserKill(aIndex);
				}
				else if(gServerInfo.m_CheckMoveHackAction == 2)
				{
					GCCloseClientSend(aIndex,0);
				}

				return;
			}
		}
	}

	if(gEffectManager.CheckStunEffect(lpObj) != 0 || gEffectManager.CheckImmobilizeEffect(lpObj) != 0)
	{
		return;
	}

	if(lpObj->SkillSummonPartyTime != 0)
	{
		lpObj->SkillSummonPartyTime = 0;
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(272));
	}

	lpObj->Dir = lpMsg->path[0] >> 4;
	lpObj->Rest = 0;
	lpObj->PathCur = 0;
	lpObj->PathCount = lpMsg->path[0] & 0x0F;

	memset(lpObj->PathX,0,sizeof(lpObj->PathX));

	memset(lpObj->PathY,0,sizeof(lpObj->PathY));

	memset(lpObj->PathOri,0,sizeof(lpObj->PathOri));

	lpObj->TX = lpMsg->x;
	lpObj->TY = lpMsg->y;
	lpObj->PathCur = ((lpObj->PathCount>0)?1:0);
	lpObj->PathCount = ((lpObj->PathCount>0)?(lpObj->PathCount+1):lpObj->PathCount);
	lpObj->PathStartEnd = 1;
	lpObj->PathX[0] = lpMsg->x;
	lpObj->PathY[0] = lpMsg->y;
	lpObj->PathDir[0] = lpObj->Dir;

	for(int n=1;n < lpObj->PathCount;n++)
	{
		if((n%2) == 0)
		{
			lpObj->TX = lpObj->PathX[n-1]+RoadPathTable[((lpMsg->path[((n+1)/2)] & 0x0F)*2)+0];
			lpObj->TY = lpObj->PathY[n-1]+RoadPathTable[((lpMsg->path[((n+1)/2)] & 0x0F)*2)+1];
			lpObj->PathX[n] = lpObj->PathX[n-1]+RoadPathTable[((lpMsg->path[((n+1)/2)] & 0x0F)*2)+0];
			lpObj->PathY[n] = lpObj->PathY[n-1]+RoadPathTable[((lpMsg->path[((n+1)/2)] & 0x0F)*2)+1];
			lpObj->PathOri[n-1] = lpMsg->path[((n+1)/2)] & 0x0F;
			lpObj->PathDir[n+0] = lpMsg->path[((n+1)/2)] & 0x0F;
		}
		else
		{
			lpObj->TX = lpObj->PathX[n-1]+RoadPathTable[((lpMsg->path[((n+1)/2)]/0x10)*2)+0];
			lpObj->TY = lpObj->PathY[n-1]+RoadPathTable[((lpMsg->path[((n+1)/2)]/0x10)*2)+1];
			lpObj->PathX[n] = lpObj->PathX[n-1]+RoadPathTable[((lpMsg->path[((n+1)/2)]/0x10)*2)+0];
			lpObj->PathY[n] = lpObj->PathY[n-1]+RoadPathTable[((lpMsg->path[((n+1)/2)]/0x10)*2)+1];
			lpObj->PathOri[n-1] = lpMsg->path[((n+1)/2)]/0x10;
			lpObj->PathDir[n+0] = lpMsg->path[((n+1)/2)]/0x10;
		}
	}

	if(lpObj->TX < (lpObj->X-15) || lpObj->TX > (lpObj->X+15) || lpObj->TY < (lpObj->Y-15) || lpObj->TY > (lpObj->Y+15))
	{
		lpObj->PathCur = 0;
		lpObj->PathCount = 0;
		lpObj->PathStartEnd = 0;
		memset(lpObj->PathX,0,sizeof(lpObj->PathX));
		memset(lpObj->PathY,0,sizeof(lpObj->PathY));
		memset(lpObj->PathOri,0,sizeof(lpObj->PathOri));
		gObjSetPosition(lpObj->Index,lpObj->X,lpObj->Y);
		return;
	}
	
	if(lpObj->PathCount > 0 && (gMap[lpObj->Map].CheckAttr(lpObj->TX,lpObj->TY,4) != 0 || gMap[lpObj->Map].CheckAttr(lpObj->TX,lpObj->TY,8) != 0))
	{
		lpObj->PathCur = 0;
		lpObj->PathCount = 0;
		lpObj->PathStartEnd = 0;
		memset(lpObj->PathX,0,sizeof(lpObj->PathX));
		memset(lpObj->PathY,0,sizeof(lpObj->PathY));
		memset(lpObj->PathOri,0,sizeof(lpObj->PathOri));
		gObjSetPosition(lpObj->Index,lpObj->X,lpObj->Y);
		return;
	}

	gMap[lpObj->Map].DelStandAttr(lpObj->OldX,lpObj->OldY);

	lpObj->X = lpMsg->x;
	lpObj->Y = lpMsg->y;
	lpObj->TX = lpObj->TX;
	lpObj->TY = lpObj->TY;
	lpObj->OldX = lpObj->TX;
	lpObj->OldY = lpObj->TY;
	lpObj->ViewState = VIEW_STATE_NONE;

	gMap[lpObj->Map].SetStandAttr(lpObj->TX,lpObj->TY);

	PMSG_MOVE_SEND pMsg;

	pMsg.header.set(PROTOCOL_CODE1,sizeof(pMsg));

	pMsg.index[0] = SET_NUMBERHB(aIndex);

	pMsg.index[1] = SET_NUMBERLB(aIndex);

	pMsg.x = (BYTE)lpObj->TX;

	pMsg.y = (BYTE)lpObj->TY;

	pMsg.dir = lpObj->Dir << 4;

	if(lpObj->Type == OBJECT_USER)
	{
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
	}

	for(int n=0;n < MAX_VIEWPORT;n++)
	{
		if(lpObj->VpPlayer2[n].type == OBJECT_USER)
		{
			if(lpObj->VpPlayer2[n].state != OBJECT_EMPTY && lpObj->VpPlayer2[n].state != OBJECT_DIECMD && lpObj->VpPlayer2[n].state != OBJECT_DIED)
			{
				DataSend(lpObj->VpPlayer2[n].index,(BYTE*)&pMsg,pMsg.header.size);
			}
		}
	}
}

void CGConnectAccountRecv(PMSG_CONNECT_ACCOUNT_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Connected != OBJECT_CONNECTED)
	{
		CloseClient(aIndex);
		return;
	}

	if(memcmp(gServerInfo.m_ServerVersion,lpMsg->ClientVersion,sizeof(lpMsg->ClientVersion)) != 0)
	{
		GCConnectAccountSend(aIndex,6);
		return;
	}

	if(memcmp(gServerInfo.m_ServerSerial,lpMsg->ClientSerial,sizeof(lpMsg->ClientSerial)) != 0)
	{
		GCConnectAccountSend(aIndex,6);
		return;
	}


						//========================================================================
					//FakeOnline_EMU
#if USE_FAKE_ONLINE == TRUE
					if (sObj->IsFakeOnline == 1)
					{
						s_FakeOnline.OnAttackAlreadyConnected(&gObj[i]);
						gObjDel(i); // them fix van thanh
					}
#endif
					//========================================================================

	if(lpObj->LoginMessageSend == 0)
	{
		lpObj->LoginMessageSend++;
		lpObj->LoginMessageCount++;
		lpObj->ConnectTickCount = GetTickCount();
		lpObj->ClientTickCount = lpMsg->TickCount;
		lpObj->ServerTickCount = GetTickCount();

		char account[11] = {0};

		PacketArgumentDecrypt(account,lpMsg->account,(sizeof(account)-1));

		char password[11] = {0};

		PacketArgumentDecrypt(password,lpMsg->password,(sizeof(password)-1));

		lpObj->MapServerMoveRequest = 0;
		lpObj->LastServerCode = -1;
		lpObj->DestMap = -1;
		lpObj->DestX = 0;
		lpObj->DestY = 0;

		memcpy(lpObj->Password,password,sizeof(lpObj->Password));

		GJConnectAccountSend(aIndex,account,password,lpObj->IpAddr);
	}
}

void CGCloseClientRecv(PMSG_CLOSE_CLIENT_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Connected != OBJECT_LOGGED && lpObj->Connected != OBJECT_ONLINE)
	{
		return;
	}

	gObjectManager.CharacterGameCloseSet(aIndex,lpMsg->type);
}

void CGCharacterListRecv(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Connected != OBJECT_LOGGED)
	{
		return;
	}

	GDCharacterListSend(aIndex);
}

void CGCharacterCreateRecv(PMSG_CHARACTER_CREATE_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Connected != OBJECT_LOGGED)
	{
		return;
	}

	PMSG_CHARACTER_CREATE_SEND pMsg;

	pMsg.header.set(0xF3,0x01,sizeof(pMsg));

	pMsg.result = 0;

	memcpy(pMsg.name,lpMsg->name,sizeof(pMsg.name));

	if(gServerInfo.m_CharacterCreateSwitch == 0)
	{
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	if(lpMsg->Class != DB_CLASS_DW && lpMsg->Class != DB_CLASS_DK && lpMsg->Class != DB_CLASS_FE && lpMsg->Class != DB_CLASS_MG && lpMsg->Class != DB_CLASS_DL && lpMsg->Class != DB_CLASS_SU && lpMsg->Class != DB_CLASS_RF)
	{
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	if(lpMsg->Class == DB_CLASS_SU && (lpObj->ClassCode & CARD_CODE_SU) == 0)
	{
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	if(lpMsg->Class == DB_CLASS_MG && (lpObj->ClassCode & CARD_CODE_MG) == 0)
	{
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	if(lpMsg->Class == DB_CLASS_DL && (lpObj->ClassCode & CARD_CODE_DL) == 0)
	{
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	if(lpMsg->Class == DB_CLASS_RF && (lpObj->ClassCode & CARD_CODE_RF) == 0)
	{
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	char name[11] = {0};

	memcpy(name,lpMsg->name,sizeof(lpMsg->name));

	GDCharacterCreateSend(aIndex,name,lpMsg->Class);
}

void CGCharacterDeleteRecv(PMSG_CHARACTER_DELETE_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Connected != OBJECT_LOGGED)
	{
		return;
	}

	if(lpObj->EnableDelCharacter == 0)
	{
		return;
	}

	PMSG_CHARACTER_DELETE_SEND pMsg;

	pMsg.header.set(0xF3,0x02,sizeof(pMsg));

	pMsg.result = 0;

	if(gServerInfo.m_CharacterDeleteSwitch == 0)
	{
		pMsg.result = 2;
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	if(gObjCheckPersonalCode(aIndex,lpMsg->PersonalCode) == 0)
	{
		pMsg.result = 2;
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	char name[11] = {0};

	memcpy(name,lpMsg->name,sizeof(lpMsg->name));

	GDCharacterDeleteSend(aIndex,name);
}

void CGCharacterInfoRecv(PMSG_CHARACTER_INFO_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Connected != OBJECT_LOGGED)
	{
		return;
	}

	lpObj->EnableDelCharacter = 0;
	
	char name[11] = {0};

	memcpy(name,lpMsg->name,sizeof(lpMsg->name));

	GDCharacterInfoSend(aIndex,name);
}

void CGLevelUpPointRecv(PMSG_LEVEL_UP_POINT_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	PMSG_LEVEL_UP_POINT_SEND pMsg;

	pMsg.header.set(0xF3,0x06,sizeof(pMsg));

	pMsg.result = 0;
	
	if(gObjectManager.CharacterLevelUpPointAdd(lpObj,lpMsg->type,1) != 0)
	{
		pMsg.result = 16+lpMsg->type;

		switch(lpMsg->type)
		{
			case 2: // Vitality
				pMsg.MaxLifeAndMana = GET_MAX_WORD_VALUE((lpObj->MaxLife+lpObj->AddLife));
				break;
			case 3: // Energy
				pMsg.MaxLifeAndMana = GET_MAX_WORD_VALUE((lpObj->MaxMana+lpObj->AddMana));
				break;
		}

		pMsg.MaxBP = GET_MAX_WORD_VALUE((lpObj->MaxBP+lpObj->AddBP));

		#if(GAMESERVER_UPDATE>=201)
		pMsg.MaxShield = GET_MAX_WORD_VALUE((lpObj->MaxShield+lpObj->AddShield));
		#endif

		#if(GAMESERVER_EXTRA==1)
		pMsg.ViewPoint = (DWORD)(lpObj->LevelUpPoint);
		pMsg.ViewMaxHP = (DWORD)(lpObj->MaxLife+lpObj->AddLife);
		pMsg.ViewMaxMP = (DWORD)(lpObj->MaxMana+lpObj->AddMana);
		pMsg.ViewMaxBP = (DWORD)(lpObj->MaxBP+lpObj->AddBP);
		#if(GAMESERVER_UPDATE>=201)
		pMsg.ViewMaxSD = (DWORD)(lpObj->MaxShield+lpObj->AddShield);
		#endif
		pMsg.ViewStrength = lpObj->Strength;
		pMsg.ViewDexterity = lpObj->Dexterity;
		pMsg.ViewVitality = lpObj->Vitality;
		pMsg.ViewEnergy = lpObj->Energy;
		pMsg.ViewLeadership = lpObj->Leadership;
		#endif
	}

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void CGCharacterMoveViewportEnableRecv(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	lpObj->RegenOk = ((lpObj->RegenOk==1)?2:lpObj->RegenOk);
}

void CGCharacterNameCheckRecv(PMSG_CHARACTER_NAME_CHECK_RECV* lpMsg,int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=401)

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Connected != OBJECT_LOGGED)
	{
		return;
	}

	char name[11] = {0};

	memcpy(name,lpMsg->name,sizeof(lpMsg->name));

	GDCharacterNameCheckSend(aIndex,name);

	#endif
}

void CGCharacterNameChangeRecv(PMSG_CHARACTER_NAME_CHANGE_RECV* lpMsg,int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=401)

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Connected != OBJECT_LOGGED)
	{
		return;
	}

	char OldName[11] = {0};

	char NewName[11] = {0};

	memcpy(OldName,lpMsg->OldName,sizeof(lpMsg->OldName));

	memcpy(NewName,lpMsg->NewName,sizeof(lpMsg->NewName));

	GDCharacterNameChangeSend(aIndex,OldName,NewName);

	#endif
}

void CGOptionChangeSkinRecv(PMSG_OPTION_CHANGE_SKIN_RECV* lpMsg,int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=701)

	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnected(aIndex) == 0)
	{
		return;
	}

	lpObj->ChangeSkin = lpMsg->ChangeSkin;

	gObjViewportListProtocolCreate(lpObj);

	gObjectManager.CharacterUpdateMapEffect(lpObj);

	#endif
}

void CGOptionDataRecv(PMSG_OPTION_DATA_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnected(aIndex) == 0)
	{
		return;
	}

	#if(GAMESERVER_UPDATE>=401)
	GDOptionDataSaveSend(aIndex,lpMsg->SkillKey,lpMsg->GameOption,lpMsg->QKey,lpMsg->WKey,lpMsg->EKey,lpMsg->ChatWindow,lpMsg->RKey,lpMsg->QWERLevel);
	#else
	GDOptionDataSaveSend(aIndex,lpMsg->SkillKey,lpMsg->GameOption,lpMsg->QKey,lpMsg->WKey,lpMsg->EKey,lpMsg->ChatWindow,0,0);
	#endif
}

void CGClientSecurityBreachRecv(PMSG_CLIENT_SECURITY_BREACH_RECV* lpMsg,int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=701)

	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnected(aIndex) == 0)
	{
		return;
	}

	gLog.Output(LOG_HACK,"[ClientSecurityBreach][%s][%s] (Code: %d)",lpObj->Account,lpObj->Name,MAKE_NUMBERDW(MAKE_NUMBERW(lpMsg->code[0],lpMsg->code[1]),MAKE_NUMBERW(lpMsg->code[2],lpMsg->code[3])));

	#endif
}

void CGAcheronEnterRecv(int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=701)

	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	PMSG_ACHERON_ENTER_SEND pMsg;

	pMsg.header.set(0xF8,0x21,sizeof(pMsg));

	if(gItemManager.GetInventoryItemCount(lpObj,GET_ITEM(13,146),0) == 0)
	{
		pMsg.result = 1;
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	gItemManager.DeleteInventoryItemCount(lpObj,GET_ITEM(13,146),0,1);

	gObjMoveGate(aIndex,417);

	#endif
}

void CGAcheronGuardianEnterRecv(int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=801)

	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(lpObj->Teleport != 0)
	{
		return;
	}

	if(lpObj->UsePartyMatching != 0 || lpObj->UsePartyMatchingJoin != 0)
	{
		PMSG_PARTY_MATCHING_ERROR_SEND pMsg;

		pMsg.header.set(0xEF,0x09,sizeof(pMsg));

		pMsg.result = -1;

		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);

		return;
	}

	if(gAcheronGuardian.GetSyncState() != AG_STATE_START)
	{
		PMSG_ACHERON_ENTER_SEND pMsg;

		pMsg.header.set(0xF8,0x4C,sizeof(pMsg));

		pMsg.result = 1;

		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
		return;
	}

	gObjMoveGate(aIndex,426);

	#endif
}

void CGSNSDataRecv(PMSG_SNS_DATA_RECV* lpMsg,int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=801)

	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	GDSNSDataSaveSend(aIndex,lpMsg->data);

	#endif
}

void CGSNSDataLogRecv(PMSG_SNS_DATA_LOG_RECV* lpMsg,int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=801)

	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	LogAdd(LOG_BLACK,"[SNSDataLog][%s][%s] - (Code: %d/%d/%d)",lpObj->Account,lpObj->Name,lpMsg->code[0],lpMsg->code[1],lpMsg->code[2]);

	#endif
}

void GCChatTargetSend(LPOBJ lpObj,int aIndex,char* message) // OK
{
	int size = strlen(message);

	size = ((size>MAX_CHAT_MESSAGE_SIZE)?MAX_CHAT_MESSAGE_SIZE:size);

	PMSG_CHAT_TARGET_SEND pMsg;

	pMsg.header.set(0x01,(sizeof(pMsg)-(sizeof(pMsg.message)-(size+1))));

	pMsg.index[0] = SET_NUMBERHB(aIndex);

	pMsg.index[1] = SET_NUMBERLB(aIndex);

	memcpy(pMsg.message,message,size);

	pMsg.message[size] = 0;

	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
}

void GCChatWhisperSend(int aIndex,char* name,char* message) // OK
{
	int size = strlen(message);

	size = ((size>MAX_CHAT_MESSAGE_SIZE)?MAX_CHAT_MESSAGE_SIZE:size);

	PMSG_CHAT_WHISPER_SEND pMsg;

	pMsg.header.set(0x02,(sizeof(pMsg)-(sizeof(pMsg.message)-(size+1))));

	memcpy(pMsg.name,name,sizeof(pMsg.name));

	memcpy(pMsg.message,message,size);

	pMsg.message[size] = 0;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void GCMainCheckSend(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	PMSG_MAIN_CHECK_SEND pMsg;

	pMsg.header.set(0x03,sizeof(pMsg));

	pMsg.key = (((GetLargeRand()%64)*16) | ((512 & 0x3F0)*64) | ((512 & 0x0F)) ^ 0xB479);

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void GCEventStateSend(int aIndex,BYTE state,BYTE event) // OK
{
	PMSG_EVENT_STATE_SEND pMsg;

	pMsg.header.set(0x0B,sizeof(pMsg));

	pMsg.state = state;
	
	pMsg.event = event;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void GCEventStateSendToAll(int Map,BYTE state,BYTE event) // OK
{
	PMSG_EVENT_STATE_SEND pMsg;

	pMsg.header.set(0x0B,sizeof(pMsg));

	pMsg.state = state;

	pMsg.event = event;

	for(int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
	{
		if(gObjIsConnectedGP(n) == 0)
		{
			continue;
		}

		if(Map == -1 || gObj[n].Map == Map)
		{
			DataSend(n,(BYTE*)&pMsg,pMsg.header.size);
		}
	}
}

void GCServerMsgSend(int aIndex,BYTE msg) // OK
{
	PMSG_SERVER_MSG_SEND pMsg;

	pMsg.header.set(0x0C,sizeof(pMsg));

	pMsg.MsgNumber = msg;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void GCWeatherSend(int aIndex,BYTE weather) // OK
{
	PMSG_WEATHER_SEND pMsg;

	pMsg.header.set(0x0F,sizeof(pMsg));
	
	pMsg.weather = weather;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void GCDamageSend(int aIndex,int bIndex,BYTE flag,int damage,int type,int ShieldDamage) // OK
{
	PMSG_DAMAGE_SEND pMsg;

	pMsg.header.set(PROTOCOL_CODE2,sizeof(pMsg));

	pMsg.index[0] = SET_NUMBERHB(bIndex) | (flag*0x80);
	pMsg.index[1] = SET_NUMBERLB(bIndex);

	pMsg.damage[0] = SET_NUMBERHB(GET_MAX_WORD_VALUE(damage));
	pMsg.damage[1] = SET_NUMBERLB(GET_MAX_WORD_VALUE(damage));

	#if(GAMESERVER_UPDATE>=701)

	pMsg.type[0] = SET_NUMBERHB(type);
	pMsg.type[1] = SET_NUMBERLB(type);

	#else

	pMsg.type = type;

	#endif

	#if(GAMESERVER_UPDATE>=201)
	
	pMsg.ShieldDamage[0] = SET_NUMBERHB(GET_MAX_WORD_VALUE(ShieldDamage));
	pMsg.ShieldDamage[1] = SET_NUMBERLB(GET_MAX_WORD_VALUE(ShieldDamage));
	
	#endif

	#if(GAMESERVER_UPDATE>=701)

	pMsg.attribute = 0;

	#endif

	#if(GAMESERVER_EXTRA==1)
	pMsg.ViewCurHP = (DWORD)gObj[bIndex].Life;
	#if(GAMESERVER_UPDATE>=201)
	pMsg.ViewCurSD = (DWORD)gObj[bIndex].Shield;
	#endif
	pMsg.ViewDamageHP = damage;
	#if(GAMESERVER_UPDATE>=201)
	pMsg.ViewDamageSD = ShieldDamage;
	#endif
	#endif

	if(gObj[aIndex].Type == OBJECT_USER)
	{
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
	}

	if(gObj[bIndex].Type == OBJECT_USER)
	{
		DataSend(bIndex,(BYTE*)&pMsg,pMsg.header.size);
	}
}

void GCUserDieSend(LPOBJ lpObj,int aIndex,int skill,int bIndex) // OK
{
	PMSG_USER_DIE_SEND pMsg;

	pMsg.header.set(0x17,sizeof(pMsg));

	pMsg.index[0] = SET_NUMBERHB(aIndex);
	pMsg.index[1] = SET_NUMBERLB(aIndex);

	#if(GAMESERVER_UPDATE>=401)
	pMsg.skill[0] = SET_NUMBERHB(skill);
	pMsg.skill[1] = SET_NUMBERLB(skill);
	#else
	pMsg.skill = skill;
	#endif

	pMsg.killer[0] = SET_NUMBERHB(bIndex);
	pMsg.killer[1] = SET_NUMBERLB(bIndex);

	if(lpObj->Type == OBJECT_USER)
	{
		DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
	}

	MsgSendV2(lpObj,(BYTE*)&pMsg,pMsg.header.size);
}

void GCActionSend(LPOBJ lpObj,BYTE action,int aIndex,int bIndex) // OK
{
	PMSG_ACTION_SEND pMsg;

	pMsg.header.set(0x18,sizeof(pMsg));

	pMsg.index[0] = SET_NUMBERHB(aIndex);

	pMsg.index[1] = SET_NUMBERLB(aIndex);

	pMsg.dir = lpObj->Dir;

	pMsg.action = action;

	pMsg.target[0] = SET_NUMBERHB(bIndex);

	pMsg.target[1] = SET_NUMBERLB(bIndex);

	MsgSendV2(lpObj,(BYTE*)&pMsg,pMsg.header.size);
}

void GCMoneySend(int aIndex,DWORD money) // OK
{
	PMSG_ITEM_GET_SEND pMsg;

	pMsg.header.setE(0x22,sizeof(pMsg));

	pMsg.result = 0xFE;

	memset(pMsg.ItemInfo,0,sizeof(pMsg.ItemInfo));

	pMsg.ItemInfo[0] = SET_NUMBERHB(SET_NUMBERHW(money));
	pMsg.ItemInfo[1] = SET_NUMBERLB(SET_NUMBERHW(money));
	pMsg.ItemInfo[2] = SET_NUMBERHB(SET_NUMBERLW(money));
	pMsg.ItemInfo[3] = SET_NUMBERLB(SET_NUMBERLW(money));

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void GCLifeSend(int aIndex,BYTE type,int life,int shield) // OK
{
	PMSG_LIFE_SEND pMsg;

	pMsg.header.set(0x26,sizeof(pMsg));

	pMsg.type = type;

	pMsg.life[0] = SET_NUMBERHB(GET_MAX_WORD_VALUE(life));
	pMsg.life[1] = SET_NUMBERLB(GET_MAX_WORD_VALUE(life));

	pMsg.flag = 0;

	#if(GAMESERVER_UPDATE>=201)
	pMsg.shield[0] = SET_NUMBERHB(GET_MAX_WORD_VALUE(shield));
	pMsg.shield[1] = SET_NUMBERLB(GET_MAX_WORD_VALUE(shield));
	#endif

	#if(GAMESERVER_EXTRA==1)
	pMsg.ViewHP = life;
	#if(GAMESERVER_UPDATE>=201)
	pMsg.ViewSD = shield;
	#endif
	#endif

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void GCManaSend(int aIndex,BYTE type,int mana,int bp) // OK
{
	PMSG_MANA_SEND pMsg;

	pMsg.header.set(0x27,sizeof(pMsg));

	pMsg.type = type;

	pMsg.mana[0] = SET_NUMBERHB(GET_MAX_WORD_VALUE(mana));
	pMsg.mana[1] = SET_NUMBERLB(GET_MAX_WORD_VALUE(mana));

	pMsg.bp[0] = SET_NUMBERHB(GET_MAX_WORD_VALUE(bp));
	pMsg.bp[1] = SET_NUMBERLB(GET_MAX_WORD_VALUE(bp));

	#if(GAMESERVER_EXTRA==1)
	pMsg.ViewMP = mana;
	pMsg.ViewBP = bp;
	#endif

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void GCItemUseSpecialTimeSend(int aIndex,BYTE number,int time) // OK
{
	PMSG_ITEM_SPECIAL_TIME_SEND pMsg;

	pMsg.header.setE(0x29,sizeof(pMsg));

	pMsg.number = number;

	pMsg.time = time;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void GCMapAttrSend(int aIndex,BYTE type,BYTE attr,BYTE flag,BYTE count,PMSG_MAP_ATTR* lpInfo) // OK
{
	BYTE send[256];

	PMSG_MAP_ATTR_SEND pMsg;

	pMsg.header.set(0x46,0);

	int size = sizeof(pMsg);

	pMsg.type = type;

	pMsg.attr = attr;

	pMsg.flag = flag;

	pMsg.count = count;

	PMSG_MAP_ATTR info;

	for(int n=0;n < count;n++)
	{
		info.x = lpInfo[n].x;
		info.y = lpInfo[n].y;
		info.tx = lpInfo[n].tx;
		info.ty = lpInfo[n].ty;

		memcpy(&send[size],&info,sizeof(info));
		size += sizeof(info);
	}

	pMsg.header.size = size;

	memcpy(send,&pMsg,sizeof(pMsg));

	DataSend(aIndex,send,size);
}

void GCPartyItemInfoSend(int aIndex,CItem* lpItem) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(OBJECT_RANGE(lpObj->PartyNumber) == 0)
	{
		return;
	}

	PMSG_PARTY_ITEM_INFO_SEND pMsg;

	pMsg.header.set(0x47,sizeof(pMsg));

	pMsg.index[0] = SET_NUMBERHB(aIndex);
	pMsg.index[1] = SET_NUMBERLB(aIndex);

	pMsg.ItemInfo = lpItem->m_Index & 0x1FFF;

	if(lpItem->m_Option1 != 0)
	{
		pMsg.ItemInfo |= 0x2000;
	}

	if(lpItem->m_Option2 != 0)
	{
		pMsg.ItemInfo |= 0x4000;
	}

	if(lpItem->m_Option3 != 0)
	{
		pMsg.ItemInfo |= 0x8000;
	}

	if(lpItem->IsExcItem() != 0)
	{
		pMsg.ItemInfo |= 0x10000;
	}

	if(lpItem->IsSetItem() != 0)
	{
		pMsg.ItemInfo |= 0x20000;
	}

	if(lpItem->Is380Item() != 0)
	{
		pMsg.ItemInfo |= 0x40000;
	}

	pMsg.level = (BYTE)lpItem->m_Level;

	for(int n=0;n < MAX_PARTY_USER;n++)
	{
		int index = gParty.m_PartyInfo[lpObj->PartyNumber].Index[n];

		if(OBJECT_RANGE(index) != 0)
		{
			DataSend(index,(BYTE*)&pMsg,pMsg.header.size);
		}
	}
}

void GCEffectInfoSend(int aIndex,BYTE effect) // OK
{
	PMSG_EFFECT_INFO_SEND pMsg;

	pMsg.header.set(0x48,sizeof(pMsg));

	pMsg.index[0] = SET_NUMBERHB(aIndex);
	pMsg.index[1] = SET_NUMBERLB(aIndex);

	pMsg.effect = effect;

	if(effect == 17)
	{
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
	}

	MsgSendV2(&gObj[aIndex],(BYTE*)&pMsg,pMsg.header.size);
}

void GCGuildMasterQuestionSend(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Interface.use != 0)
	{
		return;
	}

	PBMSG_HEAD pMsg;

	pMsg.set(0x54,sizeof(pMsg));

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.size);
}

void GCMonsterSkillSend(LPOBJ lpObj,LPOBJ lpTarget,int skill) // OK
{
	PMSG_MONSTER_SKILL_SEND pMsg;

	pMsg.header.set(0x69,sizeof(pMsg));

	#if(GAMESERVER_UPDATE>=401)
	pMsg.skill[0] = SET_NUMBERHB(skill);
	pMsg.skill[1] = SET_NUMBERLB(skill);
	#else
	pMsg.skill = skill;
	#endif

	pMsg.index = lpObj->Index;

	pMsg.target = lpTarget->Index;

	if(lpObj->Index == OBJECT_USER)
	{
		DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
	}

	MsgSendV2(lpObj,(BYTE*)&pMsg,pMsg.header.size);
}

void GCMonsterDieSend(int aIndex,int bIndex,int experience,int damage,BYTE flag) // OK
{
	PMSG_REWARD_EXPERIENCE_SEND pMsg;

	pMsg.header.setE(0x9C,sizeof(pMsg));

	pMsg.index[0] = SET_NUMBERHB(bIndex) | (flag*0x80);
	pMsg.index[1] = SET_NUMBERLB(bIndex);

	pMsg.experience[0] = SET_NUMBERHW(experience);
	pMsg.experience[1] = SET_NUMBERLW(experience);

	pMsg.damage[0] = SET_NUMBERHB(GET_MAX_WORD_VALUE(damage));
	pMsg.damage[1] = SET_NUMBERLB(GET_MAX_WORD_VALUE(damage));

	#if(GAMESERVER_EXTRA==1)
	pMsg.ViewDamageHP = damage;
	pMsg.ViewExperience = gObj[aIndex].Experience;
	pMsg.ViewNextExperience = gObj[aIndex].NextExperience;
	#endif

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void GCRewardExperienceSend(int aIndex,int experience) // OK
{
	PMSG_REWARD_EXPERIENCE_SEND pMsg;

	pMsg.header.setE(0x9C,sizeof(pMsg));

	pMsg.index[0] = 0xFF;
	pMsg.index[1] = 0xFF;

	pMsg.experience[0] = SET_NUMBERHW(experience);
	pMsg.experience[1] = SET_NUMBERLW(experience);

	pMsg.damage[0] = 0;
	pMsg.damage[1] = 0;

	#if(GAMESERVER_EXTRA==1)
	pMsg.ViewDamageHP = 0;
	pMsg.ViewExperience = gObj[aIndex].Experience;
	pMsg.ViewNextExperience = gObj[aIndex].NextExperience;
	#endif

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void GCEventEnterCountSend(int aIndex,BYTE EventType,BYTE EnterCount) // OK
{
	PMSG_EVENT_ENTER_COUNT_SEND pMsg;

	pMsg.header.setE(0x9F,sizeof(pMsg));

	pMsg.EventType = EventType;

	pMsg.EnterCount = EnterCount;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void GCPetItemInfoSend(int aIndex,BYTE type,BYTE flag,BYTE slot,BYTE level,DWORD experience,BYTE durability) // OK
{
	PMSG_PET_ITEM_INFO_SEND pMsg;

	pMsg.header.set(0xA9,sizeof(pMsg));

	pMsg.type = type;

	pMsg.flag = flag;

	pMsg.slot = slot;

	pMsg.level = level;

	pMsg.experience = experience;

	#if(GAMESERVER_UPDATE>=401)

	pMsg.durability = durability;

	#endif

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void GCMapServerMoveAuthSend(int aIndex,BYTE result) // OK
{
	PMSG_MAP_SERVER_MOVE_AUTH_SEND pMsg;

	pMsg.header.set(0xB1,0x01,sizeof(pMsg));

	pMsg.result = result;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void GCTaxInfoSend(int aIndex,BYTE type,BYTE rate) // OK
{
	PMSG_TAX_INFO_SEND pMsg;

	pMsg.header.set(0xB2,0x1A,sizeof(pMsg));

	pMsg.type = type;

	pMsg.rate = rate;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void GCCreationStateSend(int aIndex,BYTE state) // OK
{
	PMSG_CREATION_STATE_SEND pMsg;

	pMsg.header.set(0xB9,0x01,sizeof(pMsg));

	pMsg.index[0] = SET_NUMBERHB(aIndex);

	pMsg.index[1] = SET_NUMBERLB(aIndex);

	pMsg.state = state;

	MsgSendV2(&gObj[aIndex],(BYTE*)&pMsg,pMsg.header.size);
}

void GCMonsterAreaSkillSend(int aIndex,int MonsterClass,int sx,int sy,int tx,int ty) // OK
{
	PMSG_MONSTER_AREA_SKILL_SEND pMsg;

	pMsg.header.set(0xBD,0x0C,sizeof(pMsg));

	pMsg.MonsterClass[0] = SET_NUMBERHB(MonsterClass);

	pMsg.MonsterClass[1] = SET_NUMBERLB(MonsterClass);

	pMsg.sx = sx;

	pMsg.sy = sy;

	pMsg.tx = tx;

	pMsg.ty = ty;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void GCElementalDamageSend(int aIndex,int bIndex,BYTE attribute,int damage) // OK
{
	#if(GAMESERVER_UPDATE>=701)

	PMSG_ELEMENTAL_DAMAGE_SEND pMsg;

	pMsg.header.set(0xD8,sizeof(pMsg));

	pMsg.index[0] = SET_NUMBERHB(bIndex);

	pMsg.index[1] = SET_NUMBERLB(bIndex);

	pMsg.attribute = attribute;

	pMsg.damage = damage;

	#if(GAMESERVER_EXTRA==1)
	pMsg.ViewCurHP = (DWORD)gObj[bIndex].Life;
	pMsg.ViewCurSD = (DWORD)gObj[bIndex].Shield;
	#endif

	if(gObj[aIndex].Type == OBJECT_USER)
	{
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
	}

	if(gObj[bIndex].Type == OBJECT_USER)
	{
		DataSend(bIndex,(BYTE*)&pMsg,pMsg.header.size);
	}

	#endif
}

void GCCharacterCreationEnableSend(int aIndex,BYTE flag,BYTE result) // OK
{
	PMSG_CHARACTER_CREATION_ENABLE_SEND pMsg;

	pMsg.header.set(0xDE,sizeof(pMsg));

	pMsg.flag = flag;
	pMsg.result = result;

	#if(GAMESERVER_EXTRA==1)
	pMsg.ExperienceMultiplierConstA = gServerInfo.m_ExperienceMultiplierConstA;
	pMsg.ExperienceMultiplierConstB = gServerInfo.m_ExperienceMultiplierConstB;
	pMsg.CharacterMaxLevel = gServerInfo.m_MaxLevel;
	pMsg.CharacterDeleteMaxLevel = gServerInfo.m_CharacterDeleteMaxLevel;
	#endif

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void GCLifeUpdateSend(LPOBJ lpObj) // OK
{
	#if(GAMESERVER_UPDATE>=701)

	if(lpObj->Type != OBJECT_MONSTER)
	{
		return;
	}

	PMSG_LIFE_UPDATE_SEND pMsg;

	pMsg.header.set(0xEC,0x10,sizeof(pMsg));

	pMsg.index[0] = SET_NUMBERHB(lpObj->Index);
	pMsg.index[1] = SET_NUMBERLB(lpObj->Index);

	pMsg.MaxHP[0] = SET_NUMBERHB(SET_NUMBERHW((lpObj->MaxLife+lpObj->AddLife)));
	pMsg.MaxHP[1] = SET_NUMBERHB(SET_NUMBERLW((lpObj->MaxLife+lpObj->AddLife)));
	pMsg.MaxHP[2] = SET_NUMBERLB(SET_NUMBERHW((lpObj->MaxLife+lpObj->AddLife)));
	pMsg.MaxHP[3] = SET_NUMBERLB(SET_NUMBERLW((lpObj->MaxLife+lpObj->AddLife)));

	pMsg.CurHP[0] = SET_NUMBERHB(SET_NUMBERHW((lpObj->Life)));
	pMsg.CurHP[1] = SET_NUMBERHB(SET_NUMBERLW((lpObj->Life)));
	pMsg.CurHP[2] = SET_NUMBERLB(SET_NUMBERHW((lpObj->Life)));
	pMsg.CurHP[3] = SET_NUMBERLB(SET_NUMBERLW((lpObj->Life)));

	MsgSendV2(lpObj,(BYTE*)&pMsg,pMsg.header.size);

	#endif
}

void GCCharacterAttackSpeedSend(LPOBJ lpObj) // OK
{
	#if(GAMESERVER_UPDATE>=801)

	PMSG_CHARACTER_ATTACK_SPEED_SEND pMsg;

	pMsg.header.set(0xEC,0x30,sizeof(pMsg));

	pMsg.PhysiSpeed = lpObj->PhysiSpeed;

	pMsg.MagicSpeed = lpObj->MagicSpeed;

	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);

	#endif
}

void GCConnectClientSend(int aIndex,BYTE result) // OK
{
	PMSG_CONNECT_CLIENT_SEND pMsg;

	pMsg.header.set(0xF1,0x00,sizeof(pMsg));

	pMsg.result = result;

	pMsg.index[0] = SET_NUMBERHB(aIndex);

	pMsg.index[1] = SET_NUMBERLB(aIndex);

	pMsg.ClientVersion[0] = gServerInfo.m_ServerVersion[0];

	pMsg.ClientVersion[1] = gServerInfo.m_ServerVersion[1];

	pMsg.ClientVersion[2] = gServerInfo.m_ServerVersion[2];

	pMsg.ClientVersion[3] = gServerInfo.m_ServerVersion[3];

	pMsg.ClientVersion[4] = gServerInfo.m_ServerVersion[4];

	#if(GAMESERVER_EXTRA==1)
	
	pMsg.ServerCode = gServerInfo.m_ServerCode;

	#endif

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);

	gObj[aIndex].ConnectTickCount = GetTickCount();
}

void GCConnectAccountSend(int aIndex,BYTE result) // OK
{
	PMSG_CONNECT_ACCOUNT_SEND pMsg;

	pMsg.header.set(0xF1,0x01,sizeof(pMsg));

	pMsg.result = result;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void GCConnectAccountSend(int aIndex,BYTE result,SOCKET socket) // OK
{
	PMSG_CONNECT_ACCOUNT_SEND pMsg;

	pMsg.header.set(0xF1,0x01,sizeof(pMsg));

	pMsg.result = result;

	DataSendSocket(socket,(BYTE*)&pMsg,pMsg.header.size);
}

void GCCloseClientSend(int aIndex,BYTE result) // OK
{
	PMSG_CLOSE_CLIENT_SEND pMsg;

	pMsg.header.setE(0xF1,0x02,sizeof(pMsg));

	pMsg.result = result;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void GCCharacterRegenSend(LPOBJ lpObj) // OK
{
	PMSG_CHARACTER_REGEN_SEND pMsg;

	pMsg.header.setE(0xF3,0x04,sizeof(pMsg));

	pMsg.X = (BYTE)lpObj->X;

	pMsg.Y = (BYTE)lpObj->Y;

	pMsg.Map = lpObj->Map;

	pMsg.Dir = lpObj->Dir;

	pMsg.Life = GET_MAX_WORD_VALUE(lpObj->Life);

	pMsg.Mana = GET_MAX_WORD_VALUE(lpObj->Mana);
	
	#if(GAMESERVER_UPDATE>=201)
	
	pMsg.Shield = GET_MAX_WORD_VALUE(lpObj->Shield);

	#endif

	pMsg.BP = GET_MAX_WORD_VALUE(lpObj->BP);

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
	}

	#else

	pMsg.Experience = lpObj->Experience;

	#endif

	pMsg.Money = lpObj->Money;

	#if(GAMESERVER_EXTRA==1)
	pMsg.ViewCurHP = (DWORD)(lpObj->Life);
	pMsg.ViewCurMP = (DWORD)(lpObj->Mana);
	pMsg.ViewCurBP = (DWORD)(lpObj->BP);
	#if(GAMESERVER_UPDATE>=201)
	pMsg.ViewCurSD = (DWORD)(lpObj->Shield);
	#endif
	#endif

	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
}

void GCLevelUpSend(LPOBJ lpObj) // OK
{
	PMSG_LEVEL_UP_SEND pMsg;

	pMsg.header.set(0xF3,0x05,sizeof(pMsg));

	pMsg.Level = lpObj->Level;

	pMsg.LevelUpPoint = lpObj->LevelUpPoint;

	pMsg.MaxLife = GET_MAX_WORD_VALUE((lpObj->MaxLife+lpObj->AddLife));

	pMsg.MaxMana = GET_MAX_WORD_VALUE((lpObj->MaxMana+lpObj->AddMana));

	#if(GAMESERVER_UPDATE>=201)
	pMsg.MaxShield = GET_MAX_WORD_VALUE((lpObj->MaxShield+lpObj->AddShield));
	#endif

	pMsg.MaxBP = GET_MAX_WORD_VALUE((lpObj->MaxBP+lpObj->AddBP));

	pMsg.FruitAddPoint = lpObj->FruitAddPoint;

	pMsg.MaxFruitAddPoint = gFruit.GetMaxFruitPoint(lpObj);

	pMsg.FruitSubPoint = lpObj->FruitSubPoint;

	pMsg.MaxFruitSubPoint = gFruit.GetMaxFruitPoint(lpObj);

	#if(GAMESERVER_EXTRA==1)
	pMsg.ViewPoint = (DWORD)(lpObj->LevelUpPoint);
	pMsg.ViewMaxHP = (DWORD)(lpObj->MaxLife+lpObj->AddLife);
	pMsg.ViewMaxMP = (DWORD)(lpObj->MaxMana+lpObj->AddMana);
	pMsg.ViewMaxBP = (DWORD)(lpObj->MaxBP+lpObj->AddBP);
	#if(GAMESERVER_UPDATE>=201)
	pMsg.ViewMaxSD = (DWORD)(lpObj->MaxShield+lpObj->AddShield);
	#endif
	pMsg.ViewExperience = lpObj->Experience;
	pMsg.ViewNextExperience = lpObj->NextExperience;
	#endif

	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);

	GCEffectInfoSend(lpObj->Index,16);
}

void GCMonsterDamageSend(int aIndex,int damage,int ShieldDamage) // OK
{
	PMSG_MONSTER_DAMAGE_SEND pMsg;

	pMsg.header.set(0xF3,0x07,sizeof(pMsg));

	pMsg.damage[0] = SET_NUMBERHB(GET_MAX_WORD_VALUE(damage));

	pMsg.damage[1] = SET_NUMBERLB(GET_MAX_WORD_VALUE(damage));

	#if(GAMESERVER_UPDATE>=201)
	
	pMsg.ShieldDamage[0] = SET_NUMBERHB(GET_MAX_WORD_VALUE(ShieldDamage));

	pMsg.ShieldDamage[1] = SET_NUMBERLB(GET_MAX_WORD_VALUE(ShieldDamage));
	
	#endif

	#if(GAMESERVER_EXTRA==1)
	pMsg.ViewCurHP = (DWORD)gObj[aIndex].Life;
	#if(GAMESERVER_UPDATE>=201)
	pMsg.ViewCurSD = (DWORD)gObj[aIndex].Shield;
	#endif
	pMsg.ViewDamageHP = damage;
	#if(GAMESERVER_UPDATE>=201)
	pMsg.ViewDamageSD = ShieldDamage;
	#endif
	#endif

	if(gObj[aIndex].Type == OBJECT_USER)
	{
		DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
	}
}

void GCPKLevelSend(int aIndex,int PKLevel) // OK
{
	PMSG_PK_LEVEL_SEND pMsg;

	pMsg.header.set(0xF3,0x08,sizeof(pMsg));

	pMsg.index[0] = SET_NUMBERHB(aIndex);
	pMsg.index[1] = SET_NUMBERLB(aIndex);

	pMsg.PKLevel = PKLevel;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
	MsgSendV2(&gObj[aIndex],(BYTE*)&pMsg,pMsg.header.size);
}

void GCSummonLifeSend(int aIndex,int life,int MaxLife) // OK
{
	if(MaxLife <= 0)
	{
		return;
	}

	PMSG_SUMMON_LIFE_SEND pMsg;

	pMsg.header.set(0xF3,0x20,sizeof(pMsg));

	pMsg.life = (life*100)/MaxLife;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void GCTimeViewSend(int aIndex,int time) // OK
{
	PMSG_TIME_VIEW_SEND pMsg;

	pMsg.header.set(0xF3,0x22,sizeof(pMsg));

	pMsg.time = time;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void GCMathAuthenticatorSend(int aIndex) // OK
{
	#if(GAMESERVER_UPDATE>=701)

	PMSG_MATH_AUTHENTICATOR_SEND pMsg;

	pMsg.header.set(0xF3,0x32,sizeof(pMsg));

	pMsg.function = GetLargeRand()%6;

	pMsg.value = (float)(1+(GetLargeRand()%9));

	memset(pMsg.result,0,sizeof(pMsg.result));

	MAPM mapm = pMsg.value;

	switch(pMsg.function)
	{
		case 0:
			mapm = mapm.sin();
			break;
		case 1:
			mapm = mapm.cos();
			break;
		case 2:
			mapm = mapm.tan();
			break;
		case 3:
			mapm = mapm.sqrt();
			break;
		case 4:
			mapm = mapm.log();
			break;
		case 5:
			mapm = mapm.exp();
			break;
	}

	mapm.toString(pMsg.result,5);

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);

	#endif
}

void GCServerCommandSend(int aIndex,int type,int x,int y,int Viewport) // OK
{
	PMSG_SERVER_COMMAND_SEND pMsg;

	pMsg.header.set(0xF3,0x40,sizeof(pMsg));

	pMsg.type = type;

	pMsg.x = x;

	pMsg.y = y;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);

	if(Viewport != 0){MsgSendV2(&gObj[aIndex],(BYTE*)&pMsg,pMsg.header.size);}
}

void GCNewCharacterInfoSend(LPOBJ lpObj) // OK
{
	#if(GAMESERVER_EXTRA==1)

	PMSG_NEW_CHARACTER_INFO_SEND pMsg;

	pMsg.header.set(0xF3,0xE0,sizeof(pMsg));

	pMsg.Level = lpObj->Level;
	pMsg.LevelUpPoint = lpObj->LevelUpPoint;
	pMsg.Experience = lpObj->Experience;
	pMsg.NextExperience = lpObj->NextExperience;
	pMsg.Strength = GET_MAX_WORD_VALUE(lpObj->Strength);
	pMsg.Dexterity = GET_MAX_WORD_VALUE(lpObj->Dexterity);
	pMsg.Vitality = GET_MAX_WORD_VALUE(lpObj->Vitality);
	pMsg.Energy = GET_MAX_WORD_VALUE(lpObj->Energy);
	pMsg.Leadership = GET_MAX_WORD_VALUE(lpObj->Leadership);
	pMsg.Life = GET_MAX_WORD_VALUE(lpObj->Life);
	pMsg.MaxLife = GET_MAX_WORD_VALUE((lpObj->MaxLife+lpObj->AddLife));
	pMsg.Mana = GET_MAX_WORD_VALUE(lpObj->Mana);
	pMsg.MaxMana = GET_MAX_WORD_VALUE((lpObj->MaxMana+lpObj->AddMana));
	pMsg.BP = GET_MAX_WORD_VALUE(lpObj->BP);
	pMsg.MaxBP = GET_MAX_WORD_VALUE((lpObj->MaxBP+lpObj->AddBP));
	#if(GAMESERVER_UPDATE>=201)
	pMsg.Shield = GET_MAX_WORD_VALUE(lpObj->Shield);
	pMsg.MaxShield = GET_MAX_WORD_VALUE((lpObj->MaxShield+lpObj->AddShield));
	#endif
	pMsg.FruitAddPoint = lpObj->FruitAddPoint;
	pMsg.MaxFruitAddPoint = gFruit.GetMaxFruitPoint(lpObj);
	pMsg.FruitSubPoint = lpObj->FruitSubPoint;
	pMsg.MaxFruitSubPoint = gFruit.GetMaxFruitPoint(lpObj);

	//EXTRA
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

	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);

	#endif
}

void GCNewCharacterCalcSend(LPOBJ lpObj) // OK
{
	#if(GAMESERVER_EXTRA==1)

	PMSG_NEW_CHARACTER_CALC_SEND pMsg;

	pMsg.header.set(0xF3,0xE1,sizeof(pMsg));

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

	pMsg.ViewAddStrength = lpObj->AddStrength;

	pMsg.ViewAddDexterity = lpObj->AddDexterity;

	pMsg.ViewAddVitality = lpObj->AddVitality;

	pMsg.ViewAddEnergy = lpObj->AddEnergy;

	pMsg.ViewAddLeadership = lpObj->AddLeadership;

	gAttack.GetPreviewPhysiDamage(lpObj,&pMsg.ViewPhysiDamageMin,&pMsg.ViewPhysiDamageMax,&pMsg.ViewMulPhysiDamage,&pMsg.ViewDivPhysiDamage);

	gAttack.GetPreviewMagicDamage(lpObj,&pMsg.ViewMagicDamageMin,&pMsg.ViewMagicDamageMax,&pMsg.ViewMulMagicDamage,&pMsg.ViewDivMagicDamage,&pMsg.ViewMagicDamageRate);

	gAttack.GetPreviewCurseDamage(lpObj,&pMsg.ViewCurseDamageMin,&pMsg.ViewCurseDamageMax,&pMsg.ViewMulCurseDamage,&pMsg.ViewDivCurseDamage,&pMsg.ViewCurseDamageRate);

	pMsg.ViewPhysiSpeed = lpObj->PhysiSpeed;

	pMsg.ViewMagicSpeed = lpObj->MagicSpeed;

	pMsg.ViewAttackSuccessRate = lpObj->AttackSuccessRate;

	pMsg.ViewAttackSuccessRate += lpObj->EffectOption.AddAttackSuccessRate;

	pMsg.ViewAttackSuccessRate += (pMsg.ViewAttackSuccessRate*lpObj->EffectOption.MulAttackSuccessRate)/100;

	pMsg.ViewAttackSuccessRate -= (pMsg.ViewAttackSuccessRate*lpObj->EffectOption.DivAttackSuccessRate)/100;

	pMsg.ViewAttackSuccessRatePvP = lpObj->AttackSuccessRatePvP;

	gAttack.GetPreviewDefense(lpObj,&pMsg.ViewDefense);

	pMsg.ViewDefenseSuccessRate = lpObj->DefenseSuccessRate;

	pMsg.ViewDefenseSuccessRate += lpObj->EffectOption.AddDefenseSuccessRate;

	pMsg.ViewDefenseSuccessRate += (pMsg.ViewDefenseSuccessRate*lpObj->EffectOption.MulDefenseSuccessRate)/100;

	pMsg.ViewDefenseSuccessRate -= (pMsg.ViewDefenseSuccessRate*lpObj->EffectOption.DivDefenseSuccessRate)/100;

	pMsg.ViewDefenseSuccessRatePvP = lpObj->DefenseSuccessRatePvP;

	gAttack.GetPreviewDamageMultiplier(lpObj,&pMsg.ViewDamageMultiplier,&pMsg.ViewRFDamageMultiplierA,&pMsg.ViewRFDamageMultiplierB,&pMsg.ViewRFDamageMultiplierC);

	gDarkSpirit[lpObj->Index].GetPreviewAttackDamage(lpObj,&pMsg.ViewDarkSpiritAttackDamageMin,&pMsg.ViewDarkSpiritAttackDamageMax,&pMsg.ViewDarkSpiritAttackSpeed,&pMsg.ViewDarkSpiritAttackSuccessRate);

	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);

	#endif
}

void GCNewHealthBarSend(LPOBJ lpObj) // OK
{
	#if(GAMESERVER_EXTRA==1)

	#if(GAMESERVER_UPDATE<=603)

	if(gMapManager.GetMapMonsterHPBar(lpObj->Map) == 0)
	{
		return;
	}

	BYTE send[4096];

	PMSG_NEW_HEALTH_BAR_SEND pMsg;

	pMsg.header.set(0xF3,0xE2,0);

	int size = sizeof(pMsg);

	pMsg.count = 0;

	PMSG_NEW_HEALTH_BAR info;

	for(int n=0;n < MAX_VIEWPORT;n++)
	{
		if(lpObj->VpPlayer[n].state != VIEWPORT_SEND && lpObj->VpPlayer[n].state != VIEWPORT_WAIT)
		{
			continue;
		}

		if(lpObj->VpPlayer[n].type != OBJECT_MONSTER)
		{
			continue;
		}

		if(OBJECT_RANGE(lpObj->VpPlayer[n].index) == 0)
		{
			continue;
		}

		LPOBJ lpTarget = &gObj[lpObj->VpPlayer[n].index];

		if(lpTarget->Live == 0 || lpTarget->State != OBJECT_PLAYING || OBJECT_RANGE(lpTarget->SummonIndex) != 0)
		{
			continue;
		}

		if(gCustomHealthBar.CheckHealthBar(lpTarget->Class,lpTarget->Map) == 0)
		{
			continue;
		}

		info.index = lpTarget->Index;

		info.type = (BYTE)lpTarget->Type;

		info.rate = (BYTE)((lpTarget->Life*100)/(lpTarget->MaxLife+lpTarget->AddLife));

		memcpy(&send[size],&info,sizeof(info));
		size += sizeof(info);

		pMsg.count++;
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);

	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send,&pMsg,sizeof(pMsg));

	DataSend(lpObj->Index,send,size);

	#endif

	#endif
}

void GCNewGensBattleInfoSend(LPOBJ lpObj) // OK
{
	#if(GAMESERVER_EXTRA==1)

	#if(GAMESERVER_UPDATE>=501)

	PMSG_NEW_GENS_BATTLE_INFO_SEND pMsg;

	pMsg.header.set(0xF3,0xE3,sizeof(pMsg));

	pMsg.GensBattleMapCount = gGensSystem.GetGensBattleMap(pMsg.GensBattleMap);

	pMsg.GensMoveIndexCount = gGensSystem.GetGensMoveIndex(pMsg.GensMoveIndex);

	DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);

	#endif

	#endif
}

void GCNewMessageSend(int aIndex,int type,int color,char* message,...) // OK
{
	#if(GAMESERVER_EXTRA==1)

	char buff[128];

	va_list arg;
	va_start(arg, message);
	vsprintf_s(buff, message, arg);
	va_end(arg);

	PMSG_NEW_MESSAGE_SEND pMsg;

	pMsg.header.set(0xF3,0xE4,sizeof(pMsg));

	pMsg.type = type;

	pMsg.color = color;

	memcpy(pMsg.message,buff,sizeof(pMsg.message));

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);

	#endif
}

void GCWindowsNameSend(int aIndex,char* title) // OK
{
	PMSG_WINDOW_NAME_SEND pMsg;

	pMsg.header.set(0xF3,0xE8,sizeof(pMsg));

	memcpy(pMsg.title,title,sizeof(pMsg.title));

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

//**************************************************************************//
// RAW FUNCTIONS ***********************************************************//
//**************************************************************************//

void CGReqCastleSiegeState(int iIndex) 
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	GS_GDReqOwnerGuildMaster(gMapServerManager.GetMapServerGroup(),iIndex);
}

void GCAnsCastleSiegeState(int iIndex,int iResult,char* lpszGuildName,char* lpszGuildMaster)
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	if(lpszGuildName == 0 || lpszGuildMaster == 0)
	{
		return;
	}

	PMSG_CASTLE_SIEGE_STATE_SEND pMsg;

	pMsg.header.set(0xB2,0x00,sizeof(pMsg));

	pMsg.result = iResult;

	memcpy(pMsg.GuildName,lpszGuildName,sizeof(pMsg.GuildName));
	memcpy(pMsg.Name,lpszGuildMaster,sizeof(pMsg.Name));

	SYSTEMTIME tmStateStartDate = {0};
	SYSTEMTIME tmStateEndDate = {0};
	SYSTEMTIME tmSiegeStartDate = {0};

	int iCastleState = gCastleSiege.GetCastleState();
	int bRETVAL = gCastleSiege.GetCastleStateTerm(&tmStateStartDate,&tmStateEndDate);
	tmSiegeStartDate = gCastleSiege.GetCastleLeftSiegeDate();

	if( bRETVAL == FALSE )
	{
		iCastleState = -1;
	}

	int iStateLeftSec = gCastleSiege.GetCurRemainSec();


	pMsg.state = iCastleState;

	pMsg.StartYear[0] = SET_NUMBERHB(tmStateStartDate.wYear);
	pMsg.StartYear[1] = SET_NUMBERLB(tmStateStartDate.wYear);
	pMsg.StartMonth = (BYTE)tmStateStartDate.wMonth;
	pMsg.StartDay   = (BYTE)tmStateStartDate.wDay;
	pMsg.StartHour  = (BYTE)tmStateStartDate.wHour;
	pMsg.StartMinute= (BYTE)tmStateStartDate.wMinute;

	pMsg.EndYear[0] = SET_NUMBERHB(tmStateEndDate.wYear);
	pMsg.EndYear[1] = SET_NUMBERLB(tmStateEndDate.wYear);
	pMsg.EndMonth = (BYTE)tmStateEndDate.wMonth;
	pMsg.EndDay   = (BYTE)tmStateEndDate.wDay;
	pMsg.EndHour  = (BYTE)tmStateEndDate.wHour;
	pMsg.EndMinute= (BYTE)tmStateEndDate.wMinute;

	pMsg.SiegeStartYear[0] = SET_NUMBERHB(tmSiegeStartDate.wYear);
	pMsg.SiegeStartYear[1] = SET_NUMBERLB(tmSiegeStartDate.wYear);
	pMsg.SiegeStartMonth = (BYTE)tmSiegeStartDate.wMonth;
	pMsg.SiegeStartDay   = (BYTE)tmSiegeStartDate.wDay;
	pMsg.SiegeStartHour  = (BYTE)tmSiegeStartDate.wHour;
	pMsg.SiegeStartMinute= (BYTE)tmSiegeStartDate.wMinute;

	pMsg.RemainTime[0] = SET_NUMBERHB(SET_NUMBERHW(iStateLeftSec));
	pMsg.RemainTime[1] = SET_NUMBERLB(SET_NUMBERHW(iStateLeftSec));
	pMsg.RemainTime[2] = SET_NUMBERHB(SET_NUMBERLW(iStateLeftSec));
	pMsg.RemainTime[3] = SET_NUMBERLB(SET_NUMBERLW(iStateLeftSec));

	DataSend(iIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void CGReqRegCastleSiege(int iIndex) 
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	if( gCastleSiege.GetCastleState() != CASTLESIEGE_STATE_REGSIEGE )
	{
		GCAnsRegCastleSiege(iIndex,7,"");
		return;
	}
	if( strcmp(gObj[iIndex].GuildName,"") == 0 )
	{
		GCAnsRegCastleSiege(iIndex,6,"");
		return;
	}
	if( gCastleSiege.CheckUnionGuildMaster(iIndex) == FALSE )
	{
		GCAnsRegCastleSiege(iIndex,0,"");
	}
	else if( (gCastleSiege.CheckCastleOwnerMember(iIndex) != FALSE) || 
		(gCastleSiege.CheckCastleOwnerUnionMember(iIndex) != FALSE) )
	{
		GCAnsRegCastleSiege(iIndex,3,"");
	}
	else
	{
		GS_GDReqRegAttackGuild(gMapServerManager.GetMapServerGroup(),iIndex);
	}
}

void GCAnsRegCastleSiege(int iIndex,int iResult,char* lpszGuildName)
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	if( lpszGuildName == NULL )
	{
		return;
	}

	if( OBJECT_RANGE(iIndex) == FALSE )
	{
		return;
	}

	PMSG_CASTLE_SIEGE_REGISTER_GUILD_SEND pMsg;

	pMsg.header.set(0xB2,0x01,sizeof(pMsg));
	pMsg.result = iResult;
	memcpy(pMsg.GuildName,lpszGuildName,sizeof(pMsg.GuildName));

	DataSend(iIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void CGReqGiveUpCastleSiege(PMSG_REQ_GIVEUPCASTLESIEGE*lpMsg,int iIndex) 
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	if( (gCastleSiege.GetCastleState() < CASTLESIEGE_STATE_REGSIEGE) || (gCastleSiege.GetCastleState() > CASTLESIEGE_STATE_REGMARK) ) //Fixed
	{
		GCAnsGiveUpCastleSiege(iIndex,3,0,0,"");
		return;
	}

	if( strcmp(gObj[iIndex].GuildName,"") == 0 )
	{
		return;
	}
	
	if( gCastleSiege.CheckUnionGuildMaster(iIndex) == FALSE )
	{
		return;
	}

	GS_GDReqGuildSetGiveUp(gMapServerManager.GetMapServerGroup(),iIndex,lpMsg->btGiveUp);
}

void GCAnsGiveUpCastleSiege(int iIndex,int iResult,int bGiveUp,int iMarkCount,char* lpszGuildName)
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	if( lpszGuildName == NULL )
	{
		return;
	}

	if( OBJECT_RANGE(iIndex) == FALSE )
	{
		return;
	}

	PMSG_CASTLE_SIEGE_GIVEUP_GUILD_SEND pMsg;

	pMsg.header.set(0xB2,0x02,sizeof(pMsg));
	pMsg.result = iResult;
	pMsg.giveup = bGiveUp;

	memcpy(pMsg.GuildName,lpszGuildName,sizeof(pMsg.GuildName));

	if( (pMsg.result == 1) && (iMarkCount > 0) )
	{
		gObj[iIndex].Money += (iMarkCount*3000);

		if( gObj[iIndex].Money > MAX_MONEY ) 
		{
			gObj[iIndex].Money = MAX_MONEY;
		}
		GCMoneySend(iIndex,gObj[iIndex].Money);
	}

	DataSend(iIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void CGReqGuildRegInfo(int iIndex) 
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}
	
	if( strcmp(gObj[iIndex].GuildName,"") == 0 )
	{
		return;
	}

	GS_GDReqGuildMarkRegInfo(gMapServerManager.GetMapServerGroup(),iIndex);
}

void GCAnsGuildRegInfo(int iIndex,int iResult,CSP_ANS_GUILDREGINFO* lpMsgResult)
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	if( lpMsgResult == NULL )
	{
		return;
	}

	if( OBJECT_RANGE(iIndex) == FALSE )
	{
		return;
	}

	PMSG_CASTLE_SIEGE_REGISTER_GUILD_INFO_SEND pMsg;

	pMsg.header.set(0xB2,0x03,sizeof(pMsg));

	pMsg.result = iResult;
	pMsg.Mark[0] = SET_NUMBERHB(SET_NUMBERHW(lpMsgResult->iRegMarkCount));
	pMsg.Mark[1] = SET_NUMBERLB(SET_NUMBERHW(lpMsgResult->iRegMarkCount));
	pMsg.Mark[2] = SET_NUMBERHB(SET_NUMBERLW(lpMsgResult->iRegMarkCount));
	pMsg.Mark[3] = SET_NUMBERLB(SET_NUMBERLW(lpMsgResult->iRegMarkCount));
	pMsg.ranking = lpMsgResult->btRegRank;
	pMsg.giveup = lpMsgResult->bIsGiveUp;
	memcpy(pMsg.GuildName,lpMsgResult->szGuildName,sizeof(pMsg.GuildName));

	DataSend(iIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void CGReqRegGuildMark(PMSG_REQ_REGGUILDMARK*lpMsg,int iIndex) 
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	if( lpMsg == 0 )
	{
		return;
	}
	
	if( OBJECT_RANGE(iIndex) == 0 )
	{
		return;
	}
	
	if( gObj[iIndex].UseEventServer == 1 )
	{
		return;
	}

	gObj[iIndex].UseEventServer = 1;

	if( gCastleSiege.GetCastleState() != CASTLESIEGE_STATE_REGMARK )
	{
		gObj[iIndex].UseEventServer = 0;
		return;
	}
	if( strcmp( gObj[iIndex].GuildName,"") == 0)
	{
		gObj[iIndex].UseEventServer = 0;
		return;
	}
	
	if(lpMsg->btItemPos < 0 || lpMsg->btItemPos >= 64)
	{
		PMSG_CASTLE_SIEGE_REGISTER_MARK_SEND pMsg;
		pMsg.header.set(0xB2,0x04,sizeof(pMsg));
		pMsg.result = 3;
		DataSend(iIndex,(BYTE*)&pMsg,pMsg.header.size);
		gObj[iIndex].UseEventServer = 0;
	}
	else
	{
		int iItemPos = (lpMsg->btItemPos+12);

		if( gObj[iIndex].Inventory[iItemPos].IsItem() == 0 )
		{
			PMSG_CASTLE_SIEGE_REGISTER_MARK_SEND pMsg;
			pMsg.header.set(0xB2,0x04,sizeof(pMsg));
			pMsg.result = 3;
			DataSend(iIndex,(BYTE*)&pMsg,pMsg.header.size);
			gObj[iIndex].UseEventServer = 0;
		}
		else
		{
			if( gObj[iIndex].Inventory[iItemPos].m_Index != GET_ITEM(14,21) || 
				gObj[iIndex].Inventory[iItemPos].m_Level != 3 || 
				gObj[iIndex].Inventory[iItemPos].m_Durability <= 0.0f )
			{
				PMSG_CASTLE_SIEGE_REGISTER_MARK_SEND pMsg;
				pMsg.header.set(0xB2,0x04,sizeof(pMsg));
				pMsg.result = 3;
				DataSend(iIndex,(BYTE*)&pMsg,pMsg.header.size);
				gObj[iIndex].UseEventServer = 0;
			}
			else
			{
				GS_GDReqRegGuildMark(gMapServerManager.GetMapServerGroup(),iIndex,iItemPos);
			}
		}
	}
}

void GCAnsRegGuildMark(int iIndex,int iResult,CSP_ANS_GUILDREGMARK* lpMsgResult) 
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	if( lpMsgResult == 0 )
	{
		return;
	}

	if( OBJECT_RANGE(iIndex) == 0 )
	{
		return;
	}

	if( gObj[iIndex].Inventory[lpMsgResult->iItemPos].IsItem() == 0 )
	{	
		PMSG_CASTLE_SIEGE_REGISTER_MARK_SEND pMsg;

		pMsg.header.set(0xB2,0x04,sizeof(pMsg));
		pMsg.result = 3;
		DataSend(iIndex,(BYTE*)&pMsg,pMsg.header.size);
		gObj[iIndex].UseEventServer = 0;
	}
	else
	{
		if( gObj[iIndex].Inventory[lpMsgResult->iItemPos].m_Index != GET_ITEM(14,21) && 
			gObj[iIndex].Inventory[lpMsgResult->iItemPos].m_Level == 3 || 
			gObj[iIndex].Inventory[lpMsgResult->iItemPos].m_Durability <= 0.0f ) 
		{
			PMSG_CASTLE_SIEGE_REGISTER_MARK_SEND pMsg;

			pMsg.header.set(0xB2,0x04,sizeof(pMsg));
			pMsg.result = 3;
			DataSend(iIndex,(BYTE*)&pMsg,pMsg.header.size);
			gObj[iIndex].UseEventServer = 0;
		}
		else
		{
			if( (gObj[iIndex].Inventory[lpMsgResult->iItemPos].m_Durability -= 1.0) > 0.0f )
			{
				gItemManager.GCItemDurSend(iIndex,lpMsgResult->iItemPos,(BYTE)gObj[iIndex].Inventory[lpMsgResult->iItemPos].m_Durability,0);
			}
			else
			{
				gItemManager.InventoryDelItem(iIndex,lpMsgResult->iItemPos);
				gItemManager.GCItemDeleteSend(iIndex,lpMsgResult->iItemPos,1);
			}
			
			PMSG_CASTLE_SIEGE_REGISTER_MARK_SEND pMsg;

			pMsg.header.set(0xB2,0x04,sizeof(pMsg));
			pMsg.result = iResult;
			pMsg.Mark[0] = SET_NUMBERHB(SET_NUMBERHW(lpMsgResult->iRegMarkCount));
			pMsg.Mark[1] = SET_NUMBERLB(SET_NUMBERHW(lpMsgResult->iRegMarkCount));
			pMsg.Mark[2] = SET_NUMBERHB(SET_NUMBERLW(lpMsgResult->iRegMarkCount));
			pMsg.Mark[3] = SET_NUMBERLB(SET_NUMBERLW(lpMsgResult->iRegMarkCount));
			memcpy(pMsg.GuildName,lpMsgResult->szGuildName,sizeof(pMsg.GuildName));
			DataSend(iIndex,(BYTE*)&pMsg,pMsg.header.size);
			gObj[iIndex].UseEventServer = 0;
			
		}
	}
}

void CGReqNpcBuy(PMSG_REQ_NPCBUY*lpMsg,int iIndex) 
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	if( lpMsg == NULL )
	{
		return;
	}

	if( ::gObjIsConnected(iIndex) == FALSE )
	{
		return;
	}

	if( strcmp(gObj[iIndex].GuildName,"") == 0 )
	{
		return;
	}

	if( gCastleSiege.CheckGuildOwnCastle(gObj[iIndex].GuildName) != 0 && (gObj[iIndex].GuildRank == GUILD_RANK_MASTER && gObj[iIndex].GuildRank == GUILD_RANK_OFFICE_MASTER) )
	{
		BYTE btResult = 0;

		BOOL bRETVAL = gCastleSiege.CheckAddDbNPC(iIndex,lpMsg->iNpcNumber,lpMsg->iNpcIndex,btResult);

		if( bRETVAL == FALSE )
		{
			GCAnsNpcBuy(iIndex,btResult,lpMsg->iNpcNumber,lpMsg->iNpcIndex);
		}
	}
}

void GCAnsNpcBuy(int iIndex,int iResult,int iNpcNumber,int iNpcIndex) 
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}
	
	PMSG_CASTLE_SIEGE_NPC_BUY_SEND pMsg;

	pMsg.header.set(0xB2,0x05,sizeof(pMsg));
	pMsg.result = iResult;
	pMsg.npc = iNpcNumber;
	pMsg.index = iNpcIndex;
	DataSend(iIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void CGReqNpcRepair(PMSG_REQ_NPCREPAIR*lpMsg,int iIndex) 
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	if( lpMsg == NULL )
	{
		return;
	}

	if( ::gObjIsConnected(iIndex) == FALSE )
	{
		return;
	}

	if( strcmp(gObj[iIndex].GuildName,"") == 0 )
	{
		return;
	}

	if( (gCastleSiege.CheckGuildOwnCastle(gObj[iIndex].GuildName) == FALSE) || (gObj[iIndex].GuildRank != GUILD_RANK_MASTER) && (gObj[iIndex].GuildRank != GUILD_RANK_OFFICE_MASTER) )
	{
		return;
	}

	_CS_NPC_DATA pNpcData;
	BOOL bRETVAL;
	int iNpcIndex;
	int iRepairCost;

	bRETVAL = gCastleSiege.GetNpcData(lpMsg->iNpcNumber,lpMsg->iNpcIndex,pNpcData);

	if( bRETVAL == FALSE )
	{
		GCAnsNpcRepair(iIndex,0,lpMsg->iNpcNumber,lpMsg->iNpcIndex,0,0);
		return;
	}
	else
	{
		iNpcIndex = pNpcData.m_iNPC_OBJINDEX;

		if( gObjIsConnected(iNpcIndex) == FALSE )
		{
			GCAnsNpcRepair(iIndex,0,lpMsg->iNpcNumber,lpMsg->iNpcIndex,0,0);
			return;
		}
		else if( gObj[iNpcIndex].Live == 0 )
		{
			GCAnsNpcRepair(iIndex,0,lpMsg->iNpcNumber,lpMsg->iNpcIndex,0,0);
			return;
		}
		else
		{
			if( (gObj[iNpcIndex].MaxLife == gObj[iNpcIndex].Life) || (gObj[iNpcIndex].MaxLife < gObj[iNpcIndex].Life) )
			{
				gObj[iNpcIndex].Life = gObj[iNpcIndex].MaxLife;
				GCAnsNpcRepair(iIndex,0,lpMsg->iNpcNumber,lpMsg->iNpcIndex,0,0);
				return;
			}
			else
			{
				iRepairCost = 0;

				switch( lpMsg->iNpcNumber )
				{
					case 277:
						iRepairCost	= (int)(float((gObj[iNpcIndex].MaxLife - gObj[iNpcIndex].Life)*0.5f)+float(gObj[iNpcIndex].CsNpcDfLevel*1000000));
						break;
					case 283:
						iRepairCost	= (int)((gObj[iNpcIndex].MaxLife - gObj[iNpcIndex].Life)* 0.3f+float((gObj[iNpcIndex].CsNpcDfLevel+gObj[iNpcIndex].CsNpcRgLevel)*1000000));
						break;
					default:
						GCAnsNpcRepair(iIndex,0,lpMsg->iNpcNumber,lpMsg->iNpcIndex,0,0);
						return;
				}

				if( gObj[iIndex].Money < ((DWORD)iRepairCost) )
				{
					GCAnsNpcRepair(iIndex,3,lpMsg->iNpcNumber,lpMsg->iNpcIndex,0,0);
					return;
				}
				else
				{
					GS_GDReqCastleNpcRepair(gMapServerManager.GetMapServerGroup(),iIndex,lpMsg->iNpcNumber,lpMsg->iNpcIndex,iRepairCost);
					return;
				}
			}
		}
	}
}

void GCAnsNpcRepair(int iIndex,int iResult,int iNpcNumber,int iNpcIndex,int iNpcHP,int iNpcMaxHP) 
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	PMSG_CASTLE_SIEGE_NPC_REPAIR_SEND pMsg;

	pMsg.header.set(0xB2,0x06,sizeof(pMsg));
	pMsg.result = iResult;
	pMsg.npc = iNpcNumber;
	pMsg.index = iNpcIndex;
	pMsg.hp = iNpcHP;
	pMsg.maxhp = iNpcMaxHP;

	DataSend(iIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void CGReqNpcUpgrade(PMSG_REQ_NPCUPGRADE*lpMsg,int iIndex) 
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	if( lpMsg == NULL )
	{
		return;
	}

	if( ::gObjIsConnected(iIndex) == FALSE )
	{
		return;
	}

	int iNEED_GEMOFDEFEND = 0; 
	int	iNEED_MONEY = 0;
	int	iCUR_UPLVL = 0;
	int	iNXT_UPLVL = 0; 
	int	iNXT_UPVALUE = 0;
	int	bENABLE_UPGRADE = FALSE;

	if( strcmp(gObj[iIndex].GuildName,"") == 0 )
		return;

	if( (gCastleSiege.CheckGuildOwnCastle(gObj[iIndex].GuildName) == FALSE) || (gObj[iIndex].GuildRank != GUILD_RANK_MASTER) )
	{
		return;
	}

	switch( lpMsg->iNpcNumber )
	{
		case 277:
		{
			_CS_NPC_DATA m_NpcData;
			BOOL bResult = gCastleSiege.GetNpcData(lpMsg->iNpcNumber,lpMsg->iNpcIndex,m_NpcData);

			if( bResult )
			{
				switch( lpMsg->iNpcUpType )
				{
					case 1:
					{
						iCUR_UPLVL = m_NpcData.m_iNPC_DF_LEVEL;
						iNXT_UPLVL = lpMsg->iNpcUpValue;

						if( (iNXT_UPLVL >= 1) && (iNXT_UPLVL <= 3) && (iNXT_UPLVL == (iCUR_UPLVL+1)) )
						{
							bENABLE_UPGRADE = TRUE;
							iNXT_UPVALUE = iNXT_UPLVL;
							iNEED_GEMOFDEFEND =  g_iNpcUpDfLevel_CGATE[(iNXT_UPLVL-1)][1];
							iNEED_MONEY = g_iNpcUpDfLevel_CGATE[(iNXT_UPLVL-1)][2];
						}
					}
					break;

					case 3:
					{
						iCUR_UPLVL = 0;

						for( int i = 0; i < 3; i++ )
						{
							if( m_NpcData.m_iNPC_MAXHP >= *g_iNpcUpMaxHP_CGATE[i] )
							{
								iCUR_UPLVL = (i+1);
							}
						}
						iNXT_UPLVL = 0;

						for(int i = 0; i < 3; i++ )
						{
							if( lpMsg->iNpcUpValue >= *g_iNpcUpMaxHP_CGATE[i] )
							{
								iNXT_UPLVL = (i+1);
							}
						}

						if( (iNXT_UPLVL >= 1) && (iNXT_UPLVL <= 3) && (iNXT_UPLVL == (iCUR_UPLVL+1)) )
						{
							bENABLE_UPGRADE = TRUE;
							iNXT_UPVALUE = g_iNpcUpMaxHP_CGATE[(iNXT_UPLVL-1)][0];
							iNEED_GEMOFDEFEND = g_iNpcUpMaxHP_CGATE[(iNXT_UPLVL-1)][1];
							iNEED_MONEY = g_iNpcUpMaxHP_CGATE[(iNXT_UPLVL-1)][2];
						}
					}
					break;
					default:
						{
							GCAnsNpcUpgrade(iIndex,5,lpMsg->iNpcNumber,lpMsg->iNpcIndex,lpMsg->iNpcUpType,lpMsg->iNpcUpValue);
							return;
						}
						break;
				}
			}
			else
			{
				GCAnsNpcUpgrade(iIndex,7,lpMsg->iNpcNumber,lpMsg->iNpcIndex,lpMsg->iNpcUpType,lpMsg->iNpcUpValue);
				return;
			}
		}
		break;
		case 283:
		{
			_CS_NPC_DATA m_NpcData;

			BOOL bResult;
			
			bResult	= gCastleSiege.GetNpcData(lpMsg->iNpcNumber,lpMsg->iNpcIndex,m_NpcData);

			if( bResult )
			{
				switch( lpMsg->iNpcUpType )
				{
					case 1:
						{
							iCUR_UPLVL = m_NpcData.m_iNPC_DF_LEVEL;
							iNXT_UPLVL = lpMsg->iNpcUpValue;

							if( (iNXT_UPLVL >= 1) && (iNXT_UPLVL <= 3) && (iNXT_UPLVL == (iCUR_UPLVL+1)) )
							{
								bENABLE_UPGRADE = TRUE;
								iNXT_UPVALUE = iNXT_UPLVL;
								iNEED_GEMOFDEFEND = g_iNpcUpDfLevel_CSTATUE[(iNXT_UPLVL-1)][1];
								iNEED_MONEY = g_iNpcUpDfLevel_CSTATUE[(iNXT_UPLVL-1)][2];
							}
						}
						break;
					case 2:
						{
							iCUR_UPLVL = m_NpcData.m_iNPC_RG_LEVEL;
							iNXT_UPLVL = lpMsg->iNpcUpValue;
							
							if( (iNXT_UPLVL >= 1) && (iNXT_UPLVL <= 3) && (iNXT_UPLVL == (iCUR_UPLVL+1)) )
							{
								bENABLE_UPGRADE = TRUE;
								iNXT_UPVALUE = iNXT_UPLVL;
								iNEED_GEMOFDEFEND = g_iNpcUpRgLevel_CSTATUE[(iNXT_UPLVL-1)][1];
								iNEED_MONEY = g_iNpcUpRgLevel_CSTATUE[(iNXT_UPLVL-1)][2];
							}
						}
						break;
					case 3:
						{
							iCUR_UPLVL = 0;

							for( int i = 0; i < 3; i++ )
							{
								if( m_NpcData.m_iNPC_MAXHP >= *g_iNpcUpMaxHP_CSTATUE[i] )
								{
									iCUR_UPLVL = (i+1);
								}
							}

							iNXT_UPLVL = 0;

							for(int i = 0; i < 3; i++ )
							{
								if( lpMsg->iNpcUpValue >= *g_iNpcUpMaxHP_CSTATUE[i] )
								{
									iNXT_UPLVL = (i+1);
								}
							}

							if( (iNXT_UPLVL >= 1) && (iNXT_UPLVL <= 3) && (iNXT_UPLVL == (iCUR_UPLVL+1)) )
							{
								bENABLE_UPGRADE = TRUE;
								iNXT_UPVALUE = g_iNpcUpMaxHP_CSTATUE[(iNXT_UPLVL-1)][0];
								iNEED_GEMOFDEFEND = g_iNpcUpMaxHP_CSTATUE[(iNXT_UPLVL-1)][1];
								iNEED_MONEY = g_iNpcUpMaxHP_CSTATUE[(iNXT_UPLVL-1)][2];
							}
						}
						break;
					default:
						{
							GCAnsNpcUpgrade(iIndex,5,lpMsg->iNpcNumber,lpMsg->iNpcIndex,lpMsg->iNpcUpType,lpMsg->iNpcUpValue);
							return;
						}
						break;
				}
			}
			else
			{
				GCAnsNpcUpgrade(iIndex,7,lpMsg->iNpcNumber,lpMsg->iNpcIndex,lpMsg->iNpcUpType,lpMsg->iNpcUpValue);		
				return;
			}
		}
		break;
	}

	int iGEMOFDEFEND_COUNT;
	int x;
	int bRETVAL;

	if( bENABLE_UPGRADE == FALSE )
	{
		GCAnsNpcUpgrade(iIndex,6,lpMsg->iNpcNumber,lpMsg->iNpcIndex,lpMsg->iNpcUpType,lpMsg->iNpcUpValue);		
	}
	else 
	{
		if( gObj[iIndex].Money < ((DWORD)iNEED_MONEY) )
		{
			GCAnsNpcUpgrade(iIndex,3,lpMsg->iNpcNumber,lpMsg->iNpcIndex,lpMsg->iNpcUpType,lpMsg->iNpcUpValue);
		}
		else
		{
			for( iGEMOFDEFEND_COUNT = 0,x = 0; x < INVENTORY_MAIN_SIZE; x++ )
			{
				if( gObj[iIndex].Inventory[x].IsItem() == TRUE )
				{
					if( gObj[iIndex].Inventory[x].m_Index == GET_ITEM(14,31) )
					{
						iGEMOFDEFEND_COUNT += 1;
					}
				}
			}

			if( iGEMOFDEFEND_COUNT < iNEED_GEMOFDEFEND )
			{
				GCAnsNpcUpgrade(iIndex,4,lpMsg->iNpcNumber,lpMsg->iNpcIndex,lpMsg->iNpcUpType,lpMsg->iNpcUpValue);
			}
			else
			{
				bRETVAL = gCastleSiege.PayForUpgradeDbNpc(iIndex,lpMsg->iNpcNumber,lpMsg->iNpcIndex,lpMsg->iNpcUpType,lpMsg->iNpcUpValue,(iNXT_UPLVL-1));
				
				if( bRETVAL == FALSE )
				{
				}
				else
				{
					gObj[iIndex].IsCastleNPCUpgradeCompleted = TRUE;

					GS_GDReqCastleNpcUpgrade(gMapServerManager.GetMapServerGroup(),iIndex,lpMsg->iNpcNumber,lpMsg->iNpcIndex,lpMsg->iNpcUpType,iNXT_UPVALUE,(iNXT_UPLVL-1));
				}	
			}
		}

	}
}

void GCAnsNpcUpgrade(int iIndex,int iResult,int iNpcNumber,int iNpcIndex,int iNpcUpType,int iNpcUpValue) 
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	PMSG_CASTLE_SIEGE_NPC_UPGRADE_SEND pMsg;

	pMsg.header.set(0xB2,0x07,sizeof(pMsg));
	pMsg.result = iResult;
	pMsg.npc = iNpcNumber;
	pMsg.index = iNpcIndex;
	pMsg.type = iNpcUpType;
	pMsg.value = iNpcUpValue;

	DataSend(iIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void CGReqTaxMoneyInfo(int iIndex) 
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	if( strcmp(gObj[iIndex].GuildName,"") == 0 )
	{
		return;
	}
	
	if( (gCastleSiege.CheckGuildOwnCastle(gObj[iIndex].GuildName) != 0) && gObj[iIndex].GuildRank != GUILD_RANK_MASTER)
	{
		GS_GDReqTaxInfo(gMapServerManager.GetMapServerGroup(), iIndex);
	}
}

void GCAnsTaxMoneyInfo(int iIndex,int iResult,BYTE btTaxRateChaos,BYTE btTaxRateStore,__int64 i64Money) 
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	PMSG_CASTLE_SIEGE_TAX_MONEY_INFO_SEND pMsg;

	pMsg.header.set(0xB2,0x08,sizeof(pMsg));
	pMsg.result = iResult;
	pMsg.tax_rate_chaos = btTaxRateChaos;
	pMsg.tax_rate_store = btTaxRateStore;
	pMsg.money[0] = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERHDW(i64Money)));
	pMsg.money[1] = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERHDW(i64Money)));
	pMsg.money[2] = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERHDW(i64Money)));
	pMsg.money[3] = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERHDW(i64Money)));
	pMsg.money[4] = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERLDW(i64Money)));
	pMsg.money[5] = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERLDW(i64Money)));
	pMsg.money[6] = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERLDW(i64Money)));
	pMsg.money[7] = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERLDW(i64Money)));

	DataSend(iIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void CGReqTaxRateChange(PMSG_REQ_TAXRATECHANGE*lpMsg,int iIndex) 
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	int iMaxTaxRate;
	int iTaxRate;

	if( lpMsg == NULL )
	{
		return;
	}

	if( strcmp(gObj[iIndex].GuildName,"") == 0 )
	{
		return;
	}

	if( (gCastleSiege.CheckGuildOwnCastle(gObj[iIndex].GuildName) != 0) && gObj[iIndex].GuildRank != GUILD_RANK_MASTER) 
	{
		iMaxTaxRate = 0;

		switch( lpMsg->btTaxType )
		{
		case 1:
			iMaxTaxRate = 3;
			break;
		case 2:
			iMaxTaxRate = 3;
			break;
		case 3:
			iMaxTaxRate = 300000;
			break;
		}

		iTaxRate = MAKE_NUMBERDW(MAKE_NUMBERW(lpMsg->btTaxRate1,lpMsg->btTaxRate2),MAKE_NUMBERW(lpMsg->btTaxRate3,lpMsg->btTaxRate4)); 

		if( (iTaxRate < 0) || (iTaxRate > iMaxTaxRate) )
		{
			GCAnsTaxRateChange(iIndex,0,0,0);
		}
		else
		{
			GS_GDReqTaxRateChange(gMapServerManager.GetMapServerGroup(),iIndex,lpMsg->btTaxType,iTaxRate);
		}
	}
}

void GCAnsTaxRateChange(int iIndex,int iResult,BYTE btTaxType,int iTaxRate) 
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	PMSG_CASTLE_SIEGE_TAX_RATE_INFO_SEND pMsg;

	pMsg.header.set(0xB2,0x09,sizeof(pMsg));
	pMsg.result = iResult;
	pMsg.tax_type = btTaxType;
	pMsg.tax_rate[0] = SET_NUMBERHB(SET_NUMBERHW(iTaxRate));
	pMsg.tax_rate[1] = SET_NUMBERLB(SET_NUMBERHW(iTaxRate));
	pMsg.tax_rate[2] = SET_NUMBERHB(SET_NUMBERLW(iTaxRate));
	pMsg.tax_rate[3] = SET_NUMBERLB(SET_NUMBERLW(iTaxRate));
	DataSend(iIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void CGReqMoneyDrawOut(PMSG_REQ_MONEYDRAWOUT*lpMsg,int iIndex) 
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	int iMoneyChange;
	BOOL bRETVAL;

	if( lpMsg == NULL )
	{
		return;
	}

	if( strcmp(gObj[iIndex].GuildName,"") == 0 )
	{
		return;
	}
	
	if( (gCastleSiege.CheckGuildOwnCastle(gObj[iIndex].GuildName) != 0) && gObj[iIndex].GuildRank != GUILD_RANK_MASTER)
	{
		iMoneyChange = MAKE_NUMBERDW(MAKE_NUMBERW(lpMsg->btMoney4,lpMsg->btMoney3),MAKE_NUMBERW(lpMsg->btMoney2,lpMsg->btMoney1));

		if( iMoneyChange <= 0 )
		{
			GCAnsMoneyDrawOut(iIndex,0,0);
		}
		else
		{
			bRETVAL = gCastleSiege.CheckCastleHasMoney(iMoneyChange);

			if( bRETVAL == FALSE )
			{
				GCAnsMoneyDrawOut(iIndex,0,0);
			}
			else
			{
				if( (gObj[iIndex].Money+iMoneyChange) > MAX_MONEY )
				{
					GCAnsMoneyDrawOut(iIndex,0,0);
				}
				else
				{
					GS_GDReqCastleMoneyChange(gMapServerManager.GetMapServerGroup(),iIndex,-iMoneyChange);
				}
			}
		}
	}
}

void GCAnsMoneyDrawOut(int iIndex,int iResult,__int64 i64Money) 
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	PMSG_CASTLE_SIEGE_MONEY_OUT_SEND pMsg;

	pMsg.header.set(0xB2,0x10,sizeof(pMsg));
	pMsg.result = iResult;
	pMsg.money[0] = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERHDW(i64Money)));
	pMsg.money[1] = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERHDW(i64Money)));
	pMsg.money[2] = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERHDW(i64Money)));
	pMsg.money[3] = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERHDW(i64Money)));
	pMsg.money[4] = SET_NUMBERHB(SET_NUMBERHW(SET_NUMBERLDW(i64Money)));
	pMsg.money[5] = SET_NUMBERLB(SET_NUMBERHW(SET_NUMBERLDW(i64Money)));
	pMsg.money[6] = SET_NUMBERHB(SET_NUMBERLW(SET_NUMBERLDW(i64Money)));
	pMsg.money[7] = SET_NUMBERLB(SET_NUMBERLW(SET_NUMBERLDW(i64Money)));
	
	DataSend(iIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void GCAnsCsGateState(int iIndex,int iResult,int iGateIndex) 
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	PMSG_CASTLE_SIEGE_GATE_STATE_SEND pMsg;

	pMsg.header.set(0xB2,0x11,sizeof(pMsg));
	pMsg.result = iResult;
	pMsg.index[0] = SET_NUMBERHB(SET_NUMBERLW(iGateIndex));
	pMsg.index[1] = SET_NUMBERLB(SET_NUMBERLW(iGateIndex));
	DataSend(iIndex,(BYTE*)&pMsg, pMsg.header.size);
}

void CGReqCsGateOperate(PMSG_REQ_CSGATEOPERATE*lpMsg,int iIndex) 
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	int iGateIndex;

	if( strcmp(gObj[iIndex].GuildName,"") == 0 )
	{
		return;
	}
	
	if( gCastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
	{
		if( gObj[iIndex].CsJoinSide != 1 )
		{
			return;
		}
	}
	else
	{
		if( (gCastleSiege.CheckCastleOwnerMember(iIndex) == FALSE) && (gCastleSiege.CheckCastleOwnerUnionMember(iIndex) == FALSE) )
		{
			return;
		}
	}
	
	iGateIndex = MAKE_NUMBERW(lpMsg->btIndex1,lpMsg->btIndex2);

	if( gCastleSiege.CheckCsGateAlive(iGateIndex) == TRUE ) 
	{
		int iOperateGate = gCastleSiege.OperateGate(iGateIndex,gObj[iGateIndex].CsNpcExistVal,lpMsg->btOperate);

		if( iOperateGate == 0 )
		{
			GCAnsCsGateOperate(iIndex,0,-1,0);
		}
		else
		{
			GCAnsCsGateOperate(iIndex,1,iGateIndex,lpMsg->btOperate);
			gCastleSiege.SendCsGateStateViewPort(iGateIndex,lpMsg->btOperate);
		}
	}
	else
	{
		GCAnsCsGateOperate(iIndex,2,-1,0);
	}
}

void GCAnsCsGateOperate(int iIndex,int iResult,int iGateIndex,int iGateOperate) 
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	PMSG_CASTLE_SIEGE_GATE_OPERATE_SEND pMsg;

	pMsg.header.set(0xB2,0x12,sizeof(pMsg));
	pMsg.result = iResult;
	pMsg.operation = iGateOperate;
	pMsg.index[0] = SET_NUMBERHB(SET_NUMBERLW(iGateIndex));
	pMsg.index[1] = SET_NUMBERLB(SET_NUMBERLW(iGateIndex));
	DataSend(iIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void GCAnsCsGateCurState(int iIndex,int iGateIndex,int iGateOperate) 
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	PMSG_CASTLE_SIEGE_GATE_OPERATE_STATE_SEND pMsg;

	pMsg.header.set(0xB2,0x13,sizeof(pMsg));
	pMsg.operation = iGateOperate;
	pMsg.index[0] = SET_NUMBERHB(SET_NUMBERLW(iGateIndex));
	pMsg.index[1] = SET_NUMBERLB(SET_NUMBERLW(iGateIndex));
	DataSend(iIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void GCAnsCsAccessSwitchState(int iIndex,int iSwitchIndex,int iSwitchUserIndex,BYTE btSwitchState) 
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	PMSG_CASTLE_SIEGE_SWITCH_STATE_SEND pMsg;

	pMsg.header.set(0xB2,0x14,sizeof(pMsg));
	pMsg.index[0] = SET_NUMBERHB(SET_NUMBERLW(iSwitchIndex));
	pMsg.index[1] = SET_NUMBERLB(SET_NUMBERLW(iSwitchIndex));
	pMsg.target[0] = SET_NUMBERHB(SET_NUMBERLW(iSwitchUserIndex));
	pMsg.target[1] = SET_NUMBERLB(SET_NUMBERLW(iSwitchUserIndex));
	pMsg.state = btSwitchState;
	DataSend(iIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void GCAnsCsAccessCrownState(int aIndex,BYTE state)
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	#if(GAMESERVER_UPDATE<=301)
	int AccumulatedCrownAccessTime = 31000;
	#else
	int AccumulatedCrownAccessTime = 61000;
	#endif

	switch (state)
	{
		case 0:
		{
			if(lpObj->AccumulatedCrownAccessTime < 0 || lpObj->AccumulatedCrownAccessTime > AccumulatedCrownAccessTime)
			{
				lpObj->AccumulatedCrownAccessTime = 0;
			}
		}
		break;
		case 1:
		{
			lpObj->AccumulatedCrownAccessTime = 0;
		}
		break;
		case 2:
		{
			lpObj->AccumulatedCrownAccessTime += GetTickCount() - gCastleSiege.GetCrownAccessTickCount();
			lpObj->AccumulatedCrownAccessTime -= gServerInfo.m_CastleSiegeLowerAccumulatedTimeValue;

			if(lpObj->AccumulatedCrownAccessTime < 0 || lpObj->AccumulatedCrownAccessTime > AccumulatedCrownAccessTime)
			{
				lpObj->AccumulatedCrownAccessTime = 0;
			}
		}
		break;
	}

	PMSG_CASTLE_SIEGE_CROWN_ACCESS_SEND pMsg;

	pMsg.header.set(0xB2,0x15,sizeof(pMsg));
	pMsg.state = state;
	pMsg.time = lpObj->AccumulatedCrownAccessTime;
	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void GCAnsCsNotifyStart(int iIndex,BYTE btStartState) 
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	PMSG_CASTLE_SIEGE_START_STATE_SEND pMsg;

	pMsg.header.set(0xB2,0x17,sizeof(pMsg));
	pMsg.state = btStartState;
	DataSend(iIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void GCAnsCsNotifyProgress(int iIndex,BYTE btCastleSiegeState,char* lpszGuildName) 
{
	if( lpszGuildName == NULL )
	{
		return;
	}

	PMSG_CASTLE_SIEGE_NOTIFY_PROGRESS_SEND pMsg;

	pMsg.header.set(0xB2,0x18,sizeof(pMsg));
	pMsg.state = btCastleSiegeState;
	memset(pMsg.GuildName,0,sizeof(pMsg.GuildName));
	memcpy(pMsg.GuildName,lpszGuildName,sizeof(pMsg.GuildName));

	DataSend(iIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void CGReqCsMiniMapData(int iIndex) 
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	if( gObjIsConnected(iIndex) == FALSE )
	{
		return;
	}

	if( (gObj[iIndex].CsJoinSide <= 0) || ( gCastleSiege.CheckUnionGuildMaster(iIndex) == FALSE ) )
	{
		GCAnsCsMiniMapData(iIndex,3);
	}
	else if( gCastleSiege.GetCastleState() != CASTLESIEGE_STATE_STARTSIEGE )
	{
		GCAnsCsMiniMapData(iIndex,2);
	}
	else
	{
		gCastleSiege.AddMiniMapDataReqUser(iIndex);
		GCAnsCsMiniMapData(iIndex,1);
	}
}

void GCAnsCsMiniMapData(int iIndex,BYTE btResult) 
{
	PMSG_CASTLE_SIEGE_MINI_MAP_OPEN_SEND pMsg;

	pMsg.header.set(0xB2,0x1B,sizeof(pMsg));
	pMsg.result = btResult;
	DataSend(iIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void CGReqStopCsMiniMapData(int iIndex) 
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	if( gObjIsConnected(iIndex) == FALSE )
	{
		return;
	}

	gCastleSiege.DelMiniMapDataReqUser(iIndex);
}

void CGReqCsSendCommand(PMSG_REQ_CSCOMMAND*lpMsg,int iIndex) 
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	if( gObjIsConnected(iIndex) == FALSE )
	{
		return;
	}

	if( gObj[iIndex].CsJoinSide <= 0 ||	gCastleSiege.CheckUnionGuildMaster(iIndex) == FALSE )
	{
		return;
	}

	if( gCastleSiege.GetCastleState() != CASTLESIEGE_STATE_STARTSIEGE )
	{
		return;
	}

	GCAnsCsSendCommand(gObj[iIndex].CsJoinSide,lpMsg->btTeam,lpMsg->btX,lpMsg->btY,lpMsg->btCommand);
}

void GCAnsCsSendCommand(int iCsJoinSize,BYTE btTeam,BYTE btX,BYTE btY,BYTE btCommand) 
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	PMSG_CASTLE_SIEGE_COMMAND_REQUEST_SEND pMsg;

	pMsg.header.set(0xB2,0x1D,sizeof(pMsg));
	pMsg.team = btTeam;
	pMsg.x = btX;
	pMsg.y = btY;
	pMsg.command = btCommand;
	
	for( int iIndex = OBJECT_START_USER; iIndex < MAX_OBJECT; iIndex++ )
	{
		if( gObjIsConnected(iIndex) == FALSE ) continue;

		if( (gObj[iIndex].CsJoinSide == iCsJoinSize) && (gObj[iIndex].Map == MAP_CASTLE_SIEGE) )
		{
			DataSend(iIndex,(BYTE*)&pMsg,pMsg.header.size);
		}
	}
}

void GCAnsCsLeftTimeAlarm(BYTE btHour,BYTE btMinute) 
{
	PMSG_CASTLE_SIEGE_REMAIN_TIME_SEND pMsg;

	pMsg.header.set(0xB2,0x1E,sizeof(pMsg));
	pMsg.hour = btHour;
	pMsg.minute = btMinute;

	for(int iIndex = OBJECT_START_USER;iIndex < MAX_OBJECT;iIndex++)
	{
		if( !gObjIsConnected(iIndex) )
		{
			continue;
		}

		DataSend(iIndex,(BYTE*)&pMsg,pMsg.header.size);
	}

}

void GCAnsSelfCsLeftTimeAlarm(int iIndex,BYTE btHour,BYTE btMinute) 
{
	if (!gObjIsConnected(iIndex))
	{
		return;
	}

	PMSG_CASTLE_SIEGE_REMAIN_TIME_SEND pMsg;

	pMsg.header.set(0xB2,0x1E,sizeof(pMsg));
	pMsg.hour = btHour;
	pMsg.minute = btMinute;

	DataSend(iIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void CGReqCsSetEnterHuntZone(PMSG_REQ_CSHUNTZONEENTER*lpMsg,int iIndex) 
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	if( gObjIsConnected(iIndex) == FALSE )
	{
		return;
	}
	
	if( gCastleSiege.CheckCastleOwnerMember(iIndex) == FALSE )
	{
		GCAnsCsSetEnterHuntZone(iIndex,2,lpMsg->btHuntZoneEnter);
	}
	else if( gCastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
	{
		GCAnsCsSetEnterHuntZone(iIndex,0,lpMsg->btHuntZoneEnter);
	}
	else
	{
		gCastleSiege.SetHuntZoneEnter(lpMsg->btHuntZoneEnter);
		GCAnsCsSetEnterHuntZone(iIndex,1,lpMsg->btHuntZoneEnter);
	}
}

void GCAnsCsSetEnterHuntZone(int iIndex,BYTE btResult,BYTE btEnterHuntZone) 
{
	PMSG_CASTLE_SIEGE_ENABLE_HUNT_SEND pMsg;

	pMsg.header.set(0xB2,0x1F,sizeof(pMsg));
	pMsg.result = btResult;
	pMsg.allow = btEnterHuntZone;

	DataSend(iIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void CGReqNpcDbList(PMSG_REQ_NPCDBLIST*lpMsg,int iIndex) 
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	PMSG_CASTLE_SIEGE_NPC_LIST_SEND pResult;

	if( lpMsg == NULL )
	{
		return;
	}

	if( strcmp(gObj[iIndex].GuildName,"") == 0 )
	{
		return;
	}
	
	if( gCastleSiege.CheckGuildOwnCastle(gObj[iIndex].GuildName) == 0 || ( (gObj[iIndex].GuildRank != GUILD_RANK_OFFICE_MASTER) && (gObj[iIndex].GuildRank != GUILD_RANK_MASTER) ) )
	{
		pResult.header.set(0xB3,sizeof(pResult));
		pResult.count = 0;
		pResult.result = 2;
		DataSend(iIndex,(BYTE*)&pResult,sizeof(pResult));
	}
	else
	{
		switch( lpMsg->btMonsterCode )
		{
		case 1:
			gCastleSiege.SendNpcStateList(iIndex,277);
			break;
		case 2:
			gCastleSiege.SendNpcStateList(iIndex,283);
			break;
		}
	}
}

void CGReqCsRegGuildList(int iIndex) 
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	GS_GDReqAllGuildMarkRegInfo(gMapServerManager.GetMapServerGroup(),iIndex);
}

void CGReqCsAttkGuildList(int iIndex) 
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	char cBUFFER[1625];
	PMSG_ANS_CSATTKGUILDLIST* lpMsgSend;
	PMSG_CSATTKGUILDLIST* lpMsgSendBody;
	int iCount;
	
	lpMsgSend = (PMSG_ANS_CSATTKGUILDLIST*)(cBUFFER);
	lpMsgSendBody = (PMSG_CSATTKGUILDLIST*)(cBUFFER+sizeof(PMSG_ANS_CSATTKGUILDLIST));

	iCount = 0;

	lpMsgSend->btResult = gCastleSiege.GetCsAttkGuildList(lpMsgSendBody,iCount);
	lpMsgSend->iCount = iCount;

	if( lpMsgSend->iCount < 0 )	 lpMsgSend->iCount = 0;
	if( lpMsgSend->iCount > 100 )lpMsgSend->iCount = 100;

	lpMsgSend->h.set( 0xB5,((lpMsgSend->iCount*sizeof(PMSG_CSATTKGUILDLIST))+sizeof(PMSG_ANS_CSATTKGUILDLIST)));
	DataSend(iIndex,(LPBYTE)lpMsgSend,((lpMsgSend->iCount*sizeof(PMSG_CSATTKGUILDLIST))+sizeof(PMSG_ANS_CSATTKGUILDLIST)));
}

void CGReqGuildMarkOfCastleOwner(int iIndex) 
{
	if( !OBJECT_RANGE(iIndex) )
	{
		return;
	}

	LPOBJ lpObj = &gObj[iIndex];
	char szGuildName[9] = {0};

	memcpy(szGuildName,gCastleSiegeSync.GetCastleOwnerGuild(),8);

	if( szGuildName )
	{
		GUILD_INFO* lpGuild = gGuildManager.GetGuild(szGuildName);

		if( lpGuild != NULL )
		{
			PMSG_ANS_GUILDMARK_OF_CASTLEOWNER pMsg = {0};

			pMsg.h.set(0xB9,0x02,sizeof(pMsg));
			memcpy(pMsg.GuildMarkOfCastleOwner,lpGuild->Mark,sizeof(pMsg.GuildMarkOfCastleOwner));

			DataSend(iIndex,(LPBYTE)&pMsg,sizeof(pMsg));
		}
	}
}

void CGReqCastleHuntZoneEntrance(PMSG_REQ_MOVE_TO_CASTLE_HUNTZONE*aRecv,int iIndex) 
{
	if(gServerInfo.m_ServerType != 1)
	{
		return;
	}

	if( !OBJECT_RANGE(iIndex) )
	{
		return;
	}
	
	LPOBJ lpObj = &gObj[iIndex];

	PMSG_ANS_MOVE_TO_CASTLE_HUNTZONE pMsg = {0};

	pMsg.h.set(0xB9,0x05,sizeof(pMsg));

	pMsg.btResult = 0;

	int iEnterTaxMoney = 0;

	iEnterTaxMoney = gCastleSiegeSync.GetTaxHuntZone(lpObj->Index,TRUE);

	if( iEnterTaxMoney < 0)
	{
		iEnterTaxMoney = 0;
	}

	BOOL bPermission = FALSE;

	if( gCastleSiege.GetHuntZoneEnter() )
	{
		bPermission = TRUE;
	}

	if( lpObj->Guild )
	{
		if( gCastleSiege.CheckCastleOwnerMember(lpObj->Index) )
		{
			bPermission = TRUE;
		}

		if( gCastleSiege.CheckCastleOwnerUnionMember(lpObj->Index) )
		{
			bPermission = TRUE;
		}
	}
	else
	{
		if( gCastleSiege.GetHuntZoneEnter() )
		{
			bPermission = TRUE;
		}
	}

	if( bPermission == TRUE )
	{
		if( lpObj->Money >= ((DWORD)iEnterTaxMoney) )
		{
			if( lpObj->Map == MAP_CASTLE_SIEGE )
			{
				if( gObjMoveGate(lpObj->Index,95) == TRUE )
				{
					pMsg.btResult = 1;
					int iOldMoney = lpObj->Money;

					lpObj->Money -= iEnterTaxMoney;

					gCastleSiegeSync.AddTributeMoney(iEnterTaxMoney);
					
					if( lpObj->Money < 0)
					{
						lpObj->Money = 0;
					}
					
					GCMoneySend(lpObj->Index,lpObj->Money);
				}
			}
		}
	}

	DataSend(iIndex,(LPBYTE)&pMsg,sizeof(pMsg));
}