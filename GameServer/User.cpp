#include "stdafx.h"
#include "User.h"
#include "380ItemOption.h"
#include "AcheronGuardian.h"
#include "ArcaBattle.h"
#include "Attack.h"
#include "BattleSoccer.h"
#include "BloodCastle.h"
#include "BonusManager.h"
#include "CannonTower.h"
#include "CashShop.h"
#include "CastleDeep.h"
#include "CastleSiege.h"
#include "CastleSiegeCrown.h"
#include "CastleSiegeCrownSwitch.h"
#include "CastleSiegeSync.h"
#include "CastleSiegeWeapon.h"
#include "ChaosBox.h"
#include "ChaosCastle.h"
#include "ChaosCastleFinal.h"
#include "CommandManager.h"
#include "ConnectionManager.h"
#include "Crywolf.h"
#include "CrywolfAltar.h"
#include "CrywolfSync.h"
#include "CustomArena.h"
#include "CustomAttack.h"
#include "CustomEventDrop.h"
#include "CustomLukeHelper.h"
#include "CustomStore.h"
#include "DarkSpirit.h"
#include "DefaultClassInfo.h"
#include "DevilSquare.h"
#include "DoubleGoer.h"
#include "DropEvent.h"
#include "DSProtocol.h"
#include "Duel.h"
#include "EffectManager.h"
#include "EventInventory.h"
#include "GameMain.h"
#include "Gate.h"
#include "GensSystem.h"
#include "GuardianStatue.h"
#include "Guild.h"
#include "GuildManager.h"
#include "GuildMatching.h"
#include "HackPacketCheck.h"
#include "Helper.h"
#include "HelperOffline.h"
#include "IllusionTemple.h"
#include "ImperialGuardian.h"
#include "InvasionManager.h"
#include "InventoryEquipment.h"
#include "JewelOfHarmonyOption.h"
#include "JSProtocol.h"
#include "Kalima.h"
#include "Kanturu.h"
#include "LifeStone.h"
#include "Log.h"
#include "Map.h"
#include "MapManager.h"
#include "MapServerManager.h"
#include "MasterSkillTree.h"
#include "MemoryAllocator.h"
#include "Message.h"
#include "MiniMap.h"
#include "Monster.h"
#include "MonsterMove.h"
#include "MonsterSchedule.h"
#include "MonsterSkillManager.h"
#include "MossMerchant.h"
#include "Move.h"
#include "MoveSummon.h"
#include "MuCashShop.h"
#include "MuRummy.h"
#include "MuunSystem.h"
#include "Notice.h"
#include "ObjectManager.h"
#include "OnlineReward.h"
#include "Party.h"
#include "PartyMatching.h"
#include "PcPoint.h"
#include "PentagramSystem.h"
#include "PersonalShop.h"
#include "Quest.h"
#include "QuestWorld.h"
#include "Raklion.h"
#include "RaklionBattleUserMng.h"
#include "RaklionUtil.h"
#include "RandomManager.h"
#include "Reconnect.h"
#include "ScriptLoader.h"
#include "SerialCheck.h"
#include "ServerInfo.h"
#include "SetItemOption.h"
#include "SkillManager.h"
#include "SocketItemOption.h"
#include "Trade.h"
#include "Union.h"
#include "Util.h"
#include "Viewport.h"
#include "Warehouse.h"
#include "FakeOnline.h"

int gObjCount;
int gObjMonCount;
int gObjCallMonCount;
int gObjTotalUser;
int gObjTotalMonster;
int gObjTotalOffline;
int gCloseMsg;
int gCloseMsgTime;
int gCloseServer;

OBJECTSTRUCT_HEADER gObj;

MESSAGE_STATE_MACHINE_HEADER gSMMsg;

MESSAGE_STATE_ATTACK_MACHINE_HEADER gSMAttackProcMsg;

DWORD gLevelExperience[MAX_CHARACTER_LEVEL+1];

DWORD gPetExperience[MAX_CHARACTER_LEVEL+1];

//**************************************************************************//
// OBJECT MAIN FUNCTIONS ***************************************************//
//**************************************************************************//
void gObjEventRunProc() // OK
{
	#if(GAMESERVER_UPDATE>=801)

	gAcheronGuardian.MainProc();

	#endif

	#if(GAMESERVER_UPDATE>=702)

	gArcaBattle.MainProc();

	#endif

	gBattleSoccer.MainProc();

	gBloodCastle.MainProc();

	gBonusManager.MainProc();

	gCastleDeep.MainProc();

	gCastleSiege.MainProc();

	gCastleSiegeWeapon.MainProc();

	gChaosCastle.MainProc();

	gCrywolf.MainProc();

	#if(GAMESERVER_UPDATE>=803)

	gChaosCastleFinal.MainProc();

	#endif

	gCustomArena.MainProc();

	gCustomEventDrop.MainProc();

	gCustomLukeHelper.MainProc();

	gDevilSquare.MainProc();

	#if(GAMESERVER_UPDATE>=501)

	gDoubleGoer.MainProc();

	#endif

	gDropEvent.MainProc();

	#if(GAMESERVER_UPDATE>=402)

	gDuel.MainProc();

	#endif

	#if(GAMESERVER_UPDATE>=301 && GAMESERVER_UPDATE<=603)

	gIllusionTemple.MainProc();

	#endif

	#if(GAMESERVER_UPDATE>=501)

	gImperialGuardian.MainProc();

	#endif

	gInvasionManager.MainProc();

	gKanturu.MainProc();

	gMonsterSchedule.MainProc();

	#if(GAMESERVER_UPDATE>=402)

	gMossMerchant.MainProc();

	#endif

	#if(GAMESERVER_UPDATE>=401)

	gRaklion.MainProc();

	#endif
}

void gObjViewportProc() // OK
{
	for(int n = 0; n < MAX_OBJECT; n++)
	{
		gObjectManager.ObjectSetStateCreate(n);
	}

	for(int n = 0; n < MAX_OBJECT; n++)
	{
		gObjViewportListDestroy(n);
	}

	for(int n = 0; n < MAX_OBJECT; n++)
	{
		gObjViewportListCreate(n);
	}

	for(int n = 0; n < MAX_OBJECT; n++)
	{
		gObjViewportListProtocol(n);
	}

	for(int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
	{
		gPersonalShop.GCPShopViewportSend(n);
	}

	for(int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
	{
		gObj[n].PShopItemChange = 0;
	}

	for(int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
	{
		gObjViewportUnionUpdate(n);
	}

	gObjectManager.ObjectSetStateProc();
}

void gObjFirstProc() // OK
{
	for(int n = 0; n < MAX_MAP; n++)
	{
		gMap[n].WeatherVariationProcess();
	}

	gObjSecondProc();

	#if(GAMESERVER_SHOP==3)

	gCashShop.MainProc();

	#endif

	gCommandManager.MainProc();

	gDuel.CheckDuelUser();

	gEffectManager.MainProc();

	#if(GAMESERVER_UPDATE>=603)

	gHelper.MainProc();

	#endif

	#if(GAMESERVER_UPDATE>=501)

	gInventoryEquipment.MainProc();

	#endif

	#if(GAMESERVER_UPDATE>=803)

	gMuunSystem.MainProc();

	#endif

	gNotice.MainProc();

	gReconnect.MainProc();

	gCastleSiegeSync.AdjustTributeMoney();

	gCrywolf.CrywolfSecondAct();

	if(gCastleSiege.GetDataLoadState() == CASTLESIEGE_DATALOAD_2)
	{
		gCastleSiege.DataRequest();
	}

	gCastleSiege.SendCastleStateSync();

	gMonsterMove.MainProc();

	#if(GAMESERVER_SHOP>=1)

	gOnlineReward.MainProc();

	#endif
}

void gObjCloseProc() // OK
{
	if(gCloseMsg != 0)
	{
		if((--gCloseMsgTime) <= 1)
		{
			if(gCloseMsgTime == 1)
			{
				gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(280));
				LogAdd(LOG_RED,gMessage.GetMessage(280));
			}
		}
		else
		{
			if((gCloseMsgTime % 10) == 0)
			{
				gNotice.GCNoticeSendToAll(0,0,0,0,0,0,gMessage.GetMessage(281),gCloseMsgTime);
				LogAdd(LOG_RED,gMessage.GetMessage(281),gCloseMsgTime);
			}
		}

		if(gCloseMsgTime < 0)
		{
			gCloseMsg = 0;
			gCloseMsgTime = 0;
			gObjAllDisconnect();
			gCloseServer = 1;
		}
	}
}

void gObjCountProc() // OK
{
	int TotalUser = 0;

	int TotalMonster = 0;

	int TotalOffline = 0;

	for(int n = 0; n < MAX_OBJECT; n++)
	{
		if(gObj[n].Connected != OBJECT_OFFLINE)
		{
			if(gObj[n].Type == OBJECT_USER)
			{
				TotalUser++;

				if(gObj[n].Attack.Offline != 0 || gObj[n].Helper.Offline != 0 || gObj[n].PShopCustomOffline != 0)
				{
					TotalOffline++;
				}
			}
			else
			{
				TotalMonster++;
			}
		}
	}

	gObjTotalUser = TotalUser;

	gObjTotalMonster = TotalMonster;

	gObjTotalOffline = TotalOffline;
}

void gObjAccountLevelProc() // OK
{
	for(int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
	{
		if(gObjIsConnectedGP(n) != 0)
		{
			GJAccountLevelSend(n);
		}
	}
}

void gObjMathAuthenticatorProc() // OK
{
#if(GAMESERVER_UPDATE>=701)

	for(int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
	{
		if(gObjIsConnectedGP(n) != 0)
		{
			GCMathAuthenticatorSend(n);
		}
	}

#endif
}

//**************************************************************************//
// OBJECT BASE FUNCTIONS ***************************************************//
//**************************************************************************//
void gObjInit() // OK
{
	gObjCount = OBJECT_START_USER;

	gObjMonCount = OBJECT_START_MONSTER;

	gObjCallMonCount = MAX_OBJECT_MONSTER;

	memset(gObj.CommonStruct,0,sizeof(OBJECTSTRUCT));

	gMemoryAllocator.m_TempMemoryAllocatorInfo.Alloc();

	gMemoryAllocator.BindMemoryAllocatorInfo(OBJECT_START_USER,gMemoryAllocator.m_TempMemoryAllocatorInfo);
}

void gObjAllDisconnect() // OK
{
	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObj[n].Connected != OBJECT_OFFLINE && gObj[n].Type == OBJECT_USER)
		{
			CloseClient(n);

			gCustomAttack.AttackOfflineClose(&gObj[n]);

			gCustomStore.OnPShopAlreadyConnected(&gObj[n]);

			gHelperOffline.HelperOfflineClose(&gObj[n]);

						//================================================================
			//FakeOnline_EMU
#if USE_FAKE_ONLINE	== TRUE
			s_FakeOnline.OnAttackAlreadyConnected(&gObj[n]);
#endif
			//================================================================
		}
	}
}

void gObjOfflineDisconnect() // OK
{
	for(int n=OBJECT_START_USER;n < MAX_OBJECT;n++)
	{
		if(gObj[n].Connected != OBJECT_OFFLINE && gObj[n].Type == OBJECT_USER)
		{
			if(gObj[n].Attack.Offline != 0 || gObj[n].Helper.Offline != 0 || gObj[n].PShopCustomOffline != 0)
			{
				CloseClient(n);

				gCustomAttack.AttackOfflineClose(&gObj[n]);

				gCustomStore.OnPShopAlreadyConnected(&gObj[n]);

				gHelperOffline.HelperOfflineClose(&gObj[n]);
			}
		}
	}
}

void gObjSetExperienceTable() // OK
{
	gLevelExperience[0] = 0;
	gPetExperience[0] = 0;
	gPetExperience[1] = 0;

	DWORD over = 1;

	for(int n=1;n <= MAX_CHARACTER_LEVEL;n++)
	{
		gLevelExperience[n] = (((n+9)*n)*n)*gServerInfo.m_ExperienceMultiplierConstA;

		if(n > 2)
		{
			gPetExperience[n] = ((((n+10)*n)*n)*n)*gServerInfo.m_PetExperienceMultiplierConstA;
		}

		if(n > 255)
		{
			gLevelExperience[n] += (((over+9)*over)*over)*gServerInfo.m_ExperienceMultiplierConstB;
			over++;
		}
	}
}

void gObjCharZeroSet(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	lpObj->CloseCount = -1;
	lpObj->CloseType = -1;
	lpObj->EnableDelCharacter = 1;
	lpObj->TimeCount = 0;
	lpObj->Live = 0;

	memset(lpObj->Name,0,sizeof(lpObj->Name));

	lpObj->Level = 0;
	lpObj->LevelUpPoint = 0;
	lpObj->FruitAddPoint = 0;
	lpObj->FruitSubPoint = 0;
	lpObj->Money = 0;
	lpObj->Strength = 0;
	lpObj->Dexterity = 0;
	lpObj->Vitality = 0;
	lpObj->Energy = 0;
	lpObj->Leadership = 0;
	lpObj->AddStrength = 0;
	lpObj->AddDexterity = 0;
	lpObj->AddVitality = 0;
	lpObj->AddEnergy = 0;
	lpObj->AddLeadership = 0;
	lpObj->Life = 0;
	lpObj->MaxLife = 0;
	lpObj->AddLife = 0;
	lpObj->ScriptMaxLife = 0;
	lpObj->Mana = 0;
	lpObj->MaxMana = 0;
	lpObj->AddMana = 0;
	lpObj->BP;
	lpObj->MaxBP = 0;
	lpObj->AddBP = 0;
	lpObj->Shield = 0;
	lpObj->MaxShield = 0;
	lpObj->AddShield = 0;
	lpObj->ChatLimitTime = 0;
	lpObj->ChatLimitTimeSec = 0;
	lpObj->PKLevel = 0;
	lpObj->PKCount = 0;
	lpObj->PKTime = 0;
	lpObj->X = 0;
	lpObj->Y = 0;
	lpObj->TX = 0;
	lpObj->TY = 0;
	lpObj->MTX = 0;
	lpObj->MTY = 0;
	lpObj->OldX = 0;
	lpObj->OldY = 0;
	lpObj->StartX = 0;
	lpObj->StartY = 0;
	lpObj->Dir = 0;
	lpObj->Map = 0;
	lpObj->DeathMap = 0;
	lpObj->DeathX = 0;
	lpObj->DeathY = 0;
	lpObj->PathCount = 0;
	lpObj->PathCur = 0;
	lpObj->PathStartEnd = 0;
	lpObj->Authority = 0;
	lpObj->AuthorityCode = 0;
	lpObj->Penalty = 0;
	lpObj->State = OBJECT_EMPTY;
	lpObj->Rest = 0;
	lpObj->ViewState = VIEW_STATE_NONE;
	lpObj->DieRegen = 0;
	lpObj->RegenOk = 0;
	lpObj->RegenMapNumber = 0;
	lpObj->RegenMapX = 0;
	lpObj->RegenMapY = 0;
	lpObj->RegenTime = 0;
	lpObj->PosNum = -1;
	lpObj->DelayLevel = 0;
	lpObj->DrinkSpeed = 0;
	lpObj->DrinkSpeedLastTime = 0;
	lpObj->DrinkDamage = 0;
	lpObj->DrinkDamageLastTime = 0;
	lpObj->MonsterDeleteTime = 0;
	lpObj->KalimaGateExist = 0;
	lpObj->KalimaGateIndex = -1;
	lpObj->KalimaGateEnterCount = 0;
	lpObj->KalimaGateLevel = -1;
	lpObj->AttackObj = 0;
	lpObj->AttackerKilled = 0;

	for(int n = 0; n < MAX_SELF_DEFENSE; n++)
	{
		lpObj->SelfDefense[n] = -1;
	}

	memset(lpObj->SelfDefenseTime,0,sizeof(lpObj->SelfDefenseTime));

	lpObj->MySelfDefenseTime = 0;
	lpObj->PartyNumber = -1;
	lpObj->PartyTargetUser = -1;
	
	lpObj->Guild = 0;
	memset(lpObj->GuildName,0,sizeof(lpObj->GuildName));
	lpObj->GuildIndex = 0;
	lpObj->GuildRank = -1;

	lpObj->SummonIndex = -1;
	lpObj->Change = -1;
	lpObj->TargetNumber = -1;
	lpObj->TargetShopNumber = -1;
	lpObj->LastAttackerID = -1;
	lpObj->MagicDefense = 0;
	lpObj->Attribute = ATTRIBUTE_NONE;
	lpObj->MoveRange = 0;
	lpObj->InvasionIndex = -1;
	lpObj->MultiSkillIndex = 0;
	lpObj->MultiSkillCount = 0;
	lpObj->RageFighterSkillIndex = 0;
	lpObj->RageFighterSkillCount = 0;
	lpObj->RageFighterSkillTarget = 10000;

	for(int n = 0; n < MAX_SKILL_LIST; n++)
	{
		lpObj->Skill[n].Clear();
	}

	memset(lpObj->CharSet,0,sizeof(lpObj->CharSet));

	memset(lpObj->Resistance,0,sizeof(lpObj->Resistance));

	memset(lpObj->AddResistance,0,sizeof(lpObj->AddResistance));

	gObjClearViewport(lpObj);

	gObjMonsterInitHitDamage(lpObj);

	lpObj->Interface.use = 0;
	lpObj->Interface.type = INTERFACE_NONE;
	lpObj->Interface.state = 0;
	lpObj->InterfaceTime = 0;
	lpObj->Transaction = 0;

	gObjSetInventory1Pointer(lpObj);

	gObjSetEventInventory1Pointer(lpObj);

	gObjClearPlayerOption(lpObj);

	lpObj->TradeMoney = 0;
	lpObj->TradeOk = 0;
	lpObj->WarehouseCount = 0;
	lpObj->WarehousePW = 0;
	lpObj->WarehouseLock = -1;
	lpObj->WarehouseMoney = 0;
	lpObj->WarehouseSave = 0;
	lpObj->ChaosMoney = 0;
	lpObj->ChaosSuccessRate = 0;
	lpObj->ChaosLock = 0;
#if(GAMESERVER_UPDATE>=802)
	lpObj->LoadEventInventory = 0;
	lpObj->MuunItemStatus[0] = 0;
	lpObj->MuunItemStatus[1] = 0;
	lpObj->LoadMuunInventory = 0;
#endif
	lpObj->Option = 3;
	lpObj->ChaosCastleBlowTime = 0;
	lpObj->DuelUserReserved = -1;
	lpObj->DuelUserRequested = -1;
	lpObj->DuelUser = -1;
	lpObj->DuelScore = 0;
	lpObj->DuelTickCount = 0;

	memset(lpObj->PShopText,0,sizeof(lpObj->PShopText));

	memset(lpObj->PShopDealerName,0,sizeof(lpObj->PShopDealerName));

	memset(lpObj->VpPShopPlayer,0,sizeof(lpObj->VpPShopPlayer));

	lpObj->PShopOpen = 0;
	lpObj->PShopTransaction = 0;
	lpObj->PShopItemChange = 0;
	lpObj->PShopRedrawAbs = 0;
	lpObj->PShopWantDeal = 0;
	lpObj->PShopDealerIndex = -1;
	lpObj->PShopCustom = 0;
	lpObj->PShopCustomType = 0;
	lpObj->PShopCustomOffline = 0;
	lpObj->PShopCustomOfflineTime = 0;
	lpObj->VpPShopPlayerCount = 0;
	lpObj->IsInBattleGround = 0;
	lpObj->UseEventServer = 0;
	lpObj->LoadWarehouse = 0;

	memset(lpObj->Quest,0xFF,sizeof(lpObj->Quest));

	lpObj->SendQuestInfo = 0;
	lpObj->CheckLifeTime = 0;
	lpObj->LastReset = 0;
	lpObj->LastX = 0;
	lpObj->LastY = 0;
	lpObj->LastTeleportTime = 0;
	lpObj->SkillNovaState = 0;
	lpObj->SkillNovaCount = 0;
	lpObj->SkillNovaTime = 0;
	lpObj->ReqWarehouseOpen = 0;
	lpObj->IsFullSetItem = 0;
	lpObj->SkillSummonPartyTime = 0;
	lpObj->SkillSummonPartyMap = 0;
	lpObj->SkillSummonPartyX = 0;
	lpObj->SkillSummonPartyY = 0;
	lpObj->IsChaosMixCompleted = 0;
	lpObj->SkillLongSpearChange = 0;
	lpObj->CharSaveTime = 0;
	#if(GAMESERVER_UPDATE>=301)
	lpObj->LoadQuestKillCount = 0;
	lpObj->QuestKillCountIndex = -1;
	#endif
	lpObj->LoadMasterLevel = 0;
	lpObj->MasterLevel = 0;
	lpObj->MasterPoint = 0;
	lpObj->MasterExperience = 0;
	lpObj->MasterNextExperience = 0;

	for(int n = 0; n < MAX_EFFECT_LIST; n++)
	{
		lpObj->Effect[n].Clear();
	}

	lpObj->ExtInventory = 0;
	lpObj->WarehouseNumber = 0;
	lpObj->AutoAddPointCount = 0;
	lpObj->AutoResetEnable = 0;

	memset(lpObj->AutoPartyPassword,0,sizeof(lpObj->AutoPartyPassword));

	memset(lpObj->AutoAddPointStats,0,sizeof(lpObj->AutoAddPointStats));

	memset(lpObj->AutoResetStats,0,sizeof(lpObj->AutoResetStats));

	lpObj->MiniMapState = 0;
	lpObj->MiniMapValue = -1;
	lpObj->MiningStage = 0;
	lpObj->MiningIndex = -1;
	lpObj->UseGuildMatching = 0;
	lpObj->UseGuildMatchingJoin = 0;
	lpObj->UsePartyMatching = 0;
	lpObj->UsePartyMatchingJoin = 0;

	memset(lpObj->CashShopTransaction,0,sizeof(lpObj->CashShopTransaction));

	memset(lpObj->CommandManagerTransaction,0,sizeof(lpObj->CommandManagerTransaction));

	memset(lpObj->LuckyCoinTransaction,0,sizeof(lpObj->LuckyCoinTransaction));

	lpObj->GoldenArcherTransaction = 0;

	memset(lpObj->PcPointTransaction,0,sizeof(lpObj->PcPointTransaction));

	lpObj->TradeOkTime = 0;
	lpObj->PotionTime = 0;
	lpObj->ComboTime = 0;
	lpObj->HPAutoRecuperationTime = 0;
	lpObj->MPAutoRecuperationTime = 0;
	lpObj->BPAutoRecuperationTime = 0;
	lpObj->SDAutoRecuperationTime = 0;
	lpObj->OnlineRewardTime = GetTickCount();
	lpObj->Reset = 0;
	lpObj->MasterReset = 0;
	lpObj->ChangeSkin = 0;
	#if(GAMESERVER_UPDATE>=501)
	lpObj->LoadQuestWorld = 0;
	lpObj->QuestWorldMonsterClass = -1;
	lpObj->LoadGens = 0;
	lpObj->GensFamily = 0;
	lpObj->GensRank = 0;
	lpObj->GensSymbol = 0;
	lpObj->GensContribution = 0;
	lpObj->GensNextContribution = 0;
	#endif

	memset(&lpObj->EffectOption,0,sizeof(lpObj->EffectOption));

	gObjClearSpecialOption(lpObj);

	lpObj->HPRecoveryCount = 0;
	lpObj->MPRecoveryCount = 0;
	lpObj->BPRecoveryCount = 0;
	lpObj->SDRecoveryCount = 0;
	lpObj->ResurrectionTalismanActive = 0;
	lpObj->ResurrectionTalismanMap = 0;
	lpObj->ResurrectionTalismanX = 0;
	lpObj->ResurrectionTalismanY = 0;
	lpObj->MobilityTalismanActive = 0;
	lpObj->MobilityTalismanMap = 0;
	lpObj->MobilityTalismanX = 0;
	lpObj->MobilityTalismanY = 0;
	lpObj->MapServerMoveQuit = 0;
	lpObj->CsNpcExistVal = 0;
	lpObj->CsNpcType = 0;
	lpObj->CsGateOpen = 0;
	lpObj->CsGateLeverLinkIndex = -1;
	lpObj->CsNpcDfLevel = 0;
	lpObj->CsNpcRgLevel = 0;
	lpObj->CsJoinSide = 0;
	lpObj->CsGuildInvolved = 0;
	lpObj->CsSeniorMix = 0;
	lpObj->IsCastleNPCUpgradeCompleted = 0;
	lpObj->CsSiegeWeaponState = 0;
	lpObj->CsWeaponIndex = -1;
	lpObj->KillCount = 0;
	lpObj->AccumulatedDamage = 0;
	lpObj->LifeStoneCount = 0;
	lpObj->CreationState = 0;
	lpObj->CreatedActivationTime = 0;

	lpObj->MonsterSkillElementOption.Reset();

	lpObj->Agro.ResetAll();

	lpObj->BasicAI = 0;
	lpObj->CurrentAI = 0;
	lpObj->CurrentAIState = 0;
	lpObj->LastAIRunTime = 0;
	lpObj->GroupNumber = 0;
	lpObj->SubGroupNumber = 0;
	lpObj->GroupMemberGuid = -1;
	lpObj->RegenType = 0;
	lpObj->LastAutomataRuntime = 0;
	lpObj->LastAutomataDelay = 0;
	lpObj->AccumulatedCrownAccessTime = 0;
	lpObj->CrywolfMVPScore = 0;
	lpObj->RefillTime = GetTickCount();
	lpObj->LastCheckTick = 0;

	for(int n = 0; n < MAX_MONSTER_SEND_MSG; n++)
	{
		gSMMsg[aIndex][n].Clear();
	}

	for(int n = 0; n < MAX_MONSTER_SEND_ATTACK_MSG; n++)
	{
		gSMAttackProcMsg[aIndex][n].Clear();
	}

	lpObj->Married = 0;
	lpObj->MarryRequested = 0;
	lpObj->MarryRequestIndex = -1;
	lpObj->MarryRequestTime = 0;

	memset(lpObj->MarryName,0,sizeof(lpObj->MarryName));

	for(int n = 0; n < MAX_SKILL; n++)
	{
		lpObj->HackSkillCount[n] = 0;
		lpObj->HackSkillDelay[n] = 0;
	}

	lpObj->BCCount = 0;
	lpObj->CCCount = 0;
	lpObj->DSCount = 0;
	lpObj->DGCount = 0;
	lpObj->ITCount = 0;
	lpObj->IGCount = 0;

	memset(lpObj->Permission,0,sizeof(lpObj->Permission));

	lpObj->Helper.Started = 0;
	lpObj->Helper.Offline = 0;

	lpObj->Attack.Started = 0;
	lpObj->Attack.Offline = 0;

	lpObj->Pick.Started = 0;
	lpObj->Pick.ItemCount = 0;
}

void gObjClearPlayerOption(LPOBJ lpObj) // OK
{
	if(lpObj->Type != OBJECT_USER)
	{
		return;
	}

	for(int n = 0; n < INVENTORY_SIZE; n++)
	{
		lpObj->Inventory[n].Clear();
	}

	for(int n = 0; n < INVENTORY_SIZE; n++)
	{
		lpObj->Inventory1[n].Clear();
	}

	for(int n = 0; n < INVENTORY_SIZE; n++)
	{
		lpObj->Inventory2[n].Clear();
	}

	for(int n = 0; n < WAREHOUSE_SIZE; n++)
	{
		lpObj->Warehouse[n].Clear();
	}

#if(GAMESERVER_UPDATE>=802)

	for(int n = 0; n < EVENT_INVENTORY_SIZE; n++)
	{
		lpObj->EventInventory[n].Clear();
	}

	for(int n = 0; n < EVENT_INVENTORY_SIZE; n++)
	{
		lpObj->EventInventory1[n].Clear();
	}

	for(int n = 0; n < EVENT_INVENTORY_SIZE; n++)
	{
		lpObj->EventInventory2[n].Clear();
	}

#endif

#if(GAMESERVER_UPDATE>=803)

	for(int n = 0; n < MUUN_INVENTORY_SIZE; n++)
	{
		lpObj->MuunInventory[n].Clear();
	}

#endif

#if(GAMESERVER_UPDATE>=802)

	lpObj->MuRummyInfo->Clear();

#endif

	gTrade.ClearTrade(lpObj);

	memset(lpObj->InventoryMap,0xFF,INVENTORY_SIZE);

	memset(lpObj->InventoryMap1,0xFF,INVENTORY_SIZE);

	memset(lpObj->InventoryMap2,0xFF,INVENTORY_SIZE);

	memset(lpObj->WarehouseMap,0xFF,WAREHOUSE_SIZE);

#if(GAMESERVER_UPDATE>=802)

	memset(lpObj->EventInventoryMap,0xFF,EVENT_INVENTORY_SIZE);

	memset(lpObj->EventInventoryMap1,0xFF,EVENT_INVENTORY_SIZE);

	memset(lpObj->EventInventoryMap2,0xFF,EVENT_INVENTORY_SIZE);

	memset(lpObj->MuunInventoryMap,0xFF,MUUN_INVENTORY_SIZE);

#endif

#if(GAMESERVER_UPDATE>=402)
	
	for(int n = 0; n < MAX_MASTER_SKILL_LIST; n++)
	{
		lpObj->MasterSkill[n].Clear();
	}

#endif

#if(GAMESERVER_UPDATE>=301)

	for(int n = 0; n < MAX_QUEST_KILL_COUNT; n++)
	{
		lpObj->QuestKillCount[n].Clear();
	}

#endif

#if(GAMESERVER_UPDATE>=501)

	for(int n = 0; n < MAX_QUEST_WORLD_LIST; n++)
	{
		memset(&lpObj->QuestWorldList[n],0xFF,sizeof(lpObj->QuestWorldList[n]));
	}

	for(int n = 0; n < MAX_GENS_SYSTEM_VICTIM; n++)
	{
		lpObj->GensVictimList[n].Reset();
	}

#endif

#if(GAMESERVER_UPDATE>=701)

	for(int n = 0; n < MAX_PENTAGRAM_JEWEL_INFO; n++)
	{
		lpObj->PentagramJewelInfo_Inventory[n].Clear();
	}

	for(int n = 0; n < MAX_PENTAGRAM_JEWEL_INFO; n++)
	{
		lpObj->PentagramJewelInfo_Warehouse[n].Clear();
	}

#endif

	for(int n = 0; n < MAX_SKILL; n++)
	{
		memset(&lpObj->SkillDelay[n],0,sizeof(lpObj->SkillDelay[n]));
	}

	for(int n = 0; n < MAX_HACK_PACKET_INFO; n++)
	{
		memset(&lpObj->HackPacketDelay[n],0,sizeof(lpObj->HackPacketDelay[n]));
	}

	for(int n = 0; n < MAX_HACK_PACKET_INFO; n++)
	{
		memset(&lpObj->HackPacketCount[n],0,sizeof(lpObj->HackPacketCount[n]));
	}
}

void gObjClearSpecialOption(LPOBJ lpObj) // OK
{
#if(GAMESERVER_UPDATE>=701)

	memset(&lpObj->PentagramOption,0,sizeof(lpObj->PentagramOption));

	memset(&lpObj->PentagramJewelOption,0,sizeof(lpObj->PentagramJewelOption));

#endif

	lpObj->ArmorSetBonus = 0;
	lpObj->SkillDamageBonus = 0;
	lpObj->DoubleDamageRate = 0;
	lpObj->TripleDamageRate = 0;
	lpObj->IgnoreDefenseRate = 0;
	lpObj->IgnoreShieldGaugeRate = 0;
	lpObj->CriticalDamageRate = 0;
	lpObj->CriticalDamage = 0;
	lpObj->ExcellentDamageRate = 0;
	lpObj->ExcellentDamage = 0;
	lpObj->ResistDoubleDamageRate = 0;
	lpObj->ResistIgnoreDefenseRate = 0;
	lpObj->ResistIgnoreShieldGaugeRate = 0;
	lpObj->ResistCriticalDamageRate = 0;
	lpObj->ResistExcellentDamageRate = 0;
	lpObj->ResistStunRate = 0;
	lpObj->ExperienceRate = 100;
	lpObj->MasterExperienceRate = 100;
	lpObj->ItemDropRate = 100;
	lpObj->MoneyAmountDropRate = 100;
	lpObj->HPRecovery = 0;
	lpObj->MPRecovery = 0;
	lpObj->BPRecovery = 2;
	lpObj->SDRecovery = 0;
	lpObj->HPRecoveryRate = 0;
	lpObj->MPRecoveryRate = 0;
	lpObj->BPRecoveryRate = 0;
	lpObj->SDRecoveryRate = 0;
	lpObj->SDRecoveryType = 0;
	lpObj->MPConsumptionRate = 100;
	lpObj->BPConsumptionRate = 100;
	lpObj->ShieldGaugeRate = gServerInfo.m_ShieldGaugeRate;
	lpObj->DecreaseShieldGaugeRate = 0;
	lpObj->DamagePvP = 0;
	lpObj->DefensePvP = 0;
	lpObj->AttackSuccessRatePvP = 0;
	lpObj->DefenseSuccessRatePvP = 0;
	lpObj->ShieldDamageReduction = gServerInfo.m_DefenseConstA;
	lpObj->ShieldDamageReductionTime = 0;

	memset(lpObj->DamageReduction,0,sizeof(lpObj->DamageReduction));

	lpObj->DamageReflect = 0;
	lpObj->HuntHP = 0;
	lpObj->HuntMP = 0;
	lpObj->HuntBP = 0;
	lpObj->HuntSD = 0;
	lpObj->WeaponDurabilityRate = 100;
	lpObj->ArmorDurabilityRate = 100;
	lpObj->WingDurabilityRate = 100;
	lpObj->GuardianDurabilityRate = 100;
	lpObj->PendantDurabilityRate = 100;
	lpObj->RingDurabilityRate = 100;
	lpObj->PetDurabilityRate = 100;
	lpObj->FullDamageReflectRate = 0;
	lpObj->DefensiveFullHPRestoreRate = 0;
	lpObj->DefensiveFullMPRestoreRate = 0;
	lpObj->DefensiveFullSDRestoreRate = 0;
	lpObj->DefensiveFullBPRestoreRate = 0;
	lpObj->OffensiveFullHPRestoreRate = 0;
	lpObj->OffensiveFullMPRestoreRate = 0;
	lpObj->OffensiveFullSDRestoreRate = 0;
	lpObj->OffensiveFullBPRestoreRate = 0;
}

void gObjCalcExperience(LPOBJ lpObj) // OK
{
	lpObj->Experience = ((lpObj->Experience<gLevelExperience[(lpObj->Level - 1)]) ? gLevelExperience[(lpObj->Level - 1)] : lpObj->Experience);

	lpObj->NextExperience = gLevelExperience[((lpObj->Level >= MAX_CHARACTER_LEVEL) ? MAX_CHARACTER_LEVEL : lpObj->Level)];

	gMasterSkillTree.CalcMasterLevelNextExperience(lpObj);
}

bool gObjAllocData(int aIndex) // OK
{
	CMemoryAllocatorInfo MemoryAllocatorInfo;

	if(gMemoryAllocator.GetMemoryAllocatorInfo(&MemoryAllocatorInfo,aIndex) == 0)
	{
		gObj.ObjectStruct[aIndex] = new OBJECTSTRUCT;

		memset(gObj.ObjectStruct[aIndex],0,sizeof(OBJECTSTRUCT));

		InitializeCriticalSection(&gObj.ObjectStruct[aIndex]->PShopTrade);

		MemoryAllocatorInfo.m_Index = aIndex;

		MemoryAllocatorInfo.m_Active = 1;

		MemoryAllocatorInfo.m_ActiveTime = GetTickCount();

		MemoryAllocatorInfo.Alloc();

		gMemoryAllocator.InsertMemoryAllocatorInfo(MemoryAllocatorInfo);

		gMemoryAllocator.BindMemoryAllocatorInfo(aIndex,MemoryAllocatorInfo);

		if(OBJECT_MONSTER_RANGE(aIndex) != 0) { gObjMonCount = (((++gObjMonCount) >= MAX_OBJECT_MONSTER) ? OBJECT_START_MONSTER : gObjMonCount); }

		if(OBJECT_SUMMON_RANGE(aIndex) != 0) { gObjCallMonCount = (((++gObjCallMonCount) >= OBJECT_START_USER) ? MAX_OBJECT_MONSTER : gObjCallMonCount); }

		if(OBJECT_USER_RANGE(aIndex) != 0) { gObjCount = (((++gObjCount) >= MAX_OBJECT) ? OBJECT_START_USER : gObjCount); }
	}
	else
	{
		MemoryAllocatorInfo.m_Index = aIndex;

		MemoryAllocatorInfo.m_Active = 1;

		MemoryAllocatorInfo.m_ActiveTime = GetTickCount();

		gMemoryAllocator.InsertMemoryAllocatorInfo(MemoryAllocatorInfo);

		gMemoryAllocator.BindMemoryAllocatorInfo(aIndex,MemoryAllocatorInfo);
	}

	return 1;
}

void gObjFreeData(int aIndex) // OK
{
	CMemoryAllocatorInfo MemoryAllocatorInfo;

	if(gMemoryAllocator.GetMemoryAllocatorInfo(&MemoryAllocatorInfo,aIndex) != 0)
	{
		MemoryAllocatorInfo.m_Index = aIndex;

		MemoryAllocatorInfo.m_Active = 0;

		MemoryAllocatorInfo.m_ActiveTime = GetTickCount();

		gMemoryAllocator.InsertMemoryAllocatorInfo(MemoryAllocatorInfo);
	}
}

short gObjAddSearch(SOCKET socket,char* IpAddress) // OK
{
	int index = -1;
	int count = gObjCount;

	if(gCloseServer != 0)
	{
		GCConnectAccountSend(0,2,socket);
		return -1;
	}

	if(gObjTotalUser >= gServerInfo.m_ServerMaxUserNumber)
	{
		GCConnectAccountSend(0,4,socket);
		return -1;
	}

	if(gMemoryAllocator.GetMemoryAllocatorFree(&index,OBJECT_START_USER,MAX_OBJECT,10000) != 0)
	{
		return index;
	}

	for(int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
	{
		if(gObj[count].Connected == OBJECT_OFFLINE)
		{
			return count;
		}
		else
		{
			count = (((++count) >= MAX_OBJECT) ? OBJECT_START_USER : count);
		}
	}

	return -1;
}

short gObjAdd(SOCKET socket,char* IpAddress,int aIndex) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return -1;
	}

	if(gObj[aIndex].Connected != OBJECT_OFFLINE)
	{
		return -1;
	}

	if(gObjAllocData(aIndex) == 0)
	{
		return -1;
	}

	LPOBJ lpObj = &gObj[aIndex];

	gObjCharZeroSet(aIndex);

	lpObj->Index = aIndex;
	lpObj->Connected = OBJECT_CONNECTED;
	lpObj->LoginMessageSend = 0;
	lpObj->LoginMessageCount = 0;
	lpObj->Socket = socket;

	lpObj->PacketTime = GetTickCount();
	lpObj->PacketCount = 0;
	lpObj->ClientVerify = 0;

	strcpy_s(lpObj->IpAddr,IpAddress);

	lpObj->AutoSaveTime = GetTickCount();
	lpObj->ConnectTickCount = GetTickCount();
	lpObj->Type = OBJECT_USER;
	lpObj->ExtWarehouse = 0;

	memset(lpObj->Account,0,sizeof(lpObj->Account));

	gSerialCheck[aIndex].Init();

	gConnectionManager.InsertIpAddress(lpObj->IpAddr);

	gLog.Output(LOG_CONNECT,"[ObjectManager][%d] AddClient [%s]",aIndex,lpObj->IpAddr);

	return aIndex;
}

short gObjDel(int aIndex) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return -1;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Connected == OBJECT_OFFLINE)
	{
		return -1;
	}

	if(gMapManager.IsValidMap(lpObj->Map) != 0)
	{
		gMap[lpObj->Map].DelStandAttr(lpObj->X,lpObj->Y);
		gMap[lpObj->Map].DelStandAttr(lpObj->OldX,lpObj->OldY);
	}

	if(lpObj->Type == OBJECT_USER)
	{
		if(lpObj->MapServerMoveQuit == 0)
		{
			gObjectManager.CharacterGameClose(aIndex);

			GJDisconnectAccountSend(aIndex,lpObj->Account,lpObj->IpAddr);
		}

		if(lpObj->Account[0] != 0) 
		{ 
			gLog.Output(LOG_CONNECT,"[ObjectManager][%d] DelAccountInfo [%s] [%s][%s]",aIndex,lpObj->Account,lpObj->IpAddr,lpObj->HardwareId);
		}

		gLog.Output(LOG_CONNECT,"[ObjectManager][%d] DelClient [%s]",aIndex,lpObj->IpAddr);

		memset(lpObj->Account,0,sizeof(lpObj->Account));

		memset(lpObj->PersonalCode,0,sizeof(lpObj->PersonalCode));

		gConnectionManager.RemoveIpAddress(lpObj->IpAddr);

		gConnectionManager.RemoveHardwareId(lpObj->HardwareId);
	}

	lpObj->Connected = OBJECT_OFFLINE;

	gObjFreeData(aIndex);

	return aIndex;
}

LPOBJ gObjFind(char* name) // OK
{
	for(int n = OBJECT_START_USER; n < MAX_OBJECT; n++)
	{
		if(gObjIsConnectedGP(n) != 0 && strcmp(gObj[n].Name,name) == 0)
		{
			return &gObj[n];
		}
	}

	return 0;
}

int gObjCalcDistance(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	return (int)sqrt(pow(((float)lpObj->X-(float)lpTarget->X),2)+pow(((float)lpObj->Y-(float)lpTarget->Y),2));
}

int gObjCalcDistance(LPOBJ lpObj,int x,int y) // OK
{
	return (int)sqrt(pow(((float)lpObj->X-(float)x),2)+pow(((float)lpObj->Y-(float)y),2));
}

bool gObjGetRandomFreeLocation(int map,int* ox,int* oy,int tx,int ty,int count) // OK
{
	int x = (*ox);
	int y = (*oy);

	tx = ((tx<1)?1:tx);
	ty = ((ty<1)?1:ty);

	for(int n=0;n<count;n++)
	{
		(*ox) = ((GetLargeRand()%(tx+1))*((GetLargeRand()%2==0)?-1:1))+x;
		(*oy) = ((GetLargeRand()%(ty+1))*((GetLargeRand()%2==0)?-1:1))+y;

		if(gMap[map].CheckAttr((*ox),(*oy),4) == 0 && gMap[map].CheckAttr((*ox),(*oy),8) == 0)
		{
			return 1;
		}
	}

	return 0;
}

bool gObjGetRandomMonsterLocation(int map,int* ox,int* oy,int tx,int ty,int count) // OK
{
	if(gMapManager.IsValidMap(map) != 0)
	{
		int x = (*ox);
		int y = (*oy);

		tx = ((tx<1)?1:tx);
		ty = ((ty<1)?1:ty);

		for(int n=0;n<count;n++)
		{
			(*ox) = ((GetLargeRand()%(tx+1))*((GetLargeRand()%2==0)?-1:1))+x;
			(*oy) = ((GetLargeRand()%(ty+1))*((GetLargeRand()%2==0)?-1:1))+y;

			if(gMap[map].CheckAttr((*ox),(*oy),1) == 0 && gMap[map].CheckAttr((*ox),(*oy),4) == 0 && gMap[map].CheckAttr((*ox),(*oy),8) == 0)
			{
				return 1;
			}
		}
	}

	return 0;
}

//**************************************************************************//
// OBJECT CHECK FUNCTIONS **************************************************//
//**************************************************************************//
bool gObjIsConnected(int aIndex) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Connected != OBJECT_ONLINE)
	{
		return 0;
	}

	return 1;
}

bool gObjIsConnectedGP(int aIndex) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Connected != OBJECT_ONLINE)
	{
		return 0;
	}

	if(lpObj->Type != OBJECT_USER || (lpObj->CloseCount > 0 || lpObj->MapServerMoveQuit != 0))
	{
		return 0;
	}

	return 1;
}

bool gObjIsConnectedGS(int aIndex) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Connected != OBJECT_ONLINE)
	{
		return 0;
	}

	if(lpObj->Type == OBJECT_USER && (lpObj->CloseCount > 0 || lpObj->MapServerMoveQuit != 0))
	{
		return 0;
	}

	return 1;
}

bool gObjIsNameValid(int aIndex,char* name) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Connected == OBJECT_OFFLINE)
	{
		return 0;
	}

	if(strcmp(name,"") == 0)
	{
		return 0;
	}

	if(strcmp(lpObj->Name,name) != 0)
	{
		return 0;
	}

	return 1;
}

bool gObjIsAccountValid(int aIndex,char* account) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Connected == OBJECT_OFFLINE)
	{
		return 0;
	}

	if(strcmp(account,"") == 0)
	{
		return 0;
	}

	if(strcmp(lpObj->Account,account) != 0)
	{
		return 0;
	}

	return 1;
}

bool gObjIsChangeSkin(int aIndex) // OK
{
	if(gObj[aIndex].Change < 0)
	{
		return 0;
	}

	if(gObj[aIndex].Change == gServerInfo.m_TransformationRing1 || gObj[aIndex].Change == gServerInfo.m_TransformationRing2 || gObj[aIndex].Change == gServerInfo.m_TransformationRing3 || gObj[aIndex].Change == gServerInfo.m_TransformationRing4 || gObj[aIndex].Change == gServerInfo.m_TransformationRing5 || gObj[aIndex].Change == gServerInfo.m_TransformationRing6)
	{
		return ((gObj[aIndex].ChangeSkin == 0) ? 1 : 0);
	}

	if(gObj[aIndex].Change == 372 || gObj[aIndex].Change == 373 || gObj[aIndex].Change == 374 || gObj[aIndex].Change == 378 || gObj[aIndex].Change == 477 || gObj[aIndex].Change == 503 || gObj[aIndex].Change == 548 || gObj[aIndex].Change == 616 || gObj[aIndex].Change == 617 || gObj[aIndex].Change == 625 || gObj[aIndex].Change == 626 || gObj[aIndex].Change == 642)
	{
		return ((gObj[aIndex].ChangeSkin == 0) ? 1 : 0);
	}

	return 1;
}

bool gObjCheckMaxMoney(int aIndex,DWORD AddMoney) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	if(((QWORD)gObj[aIndex].Money+(QWORD)AddMoney) > (QWORD)MAX_MONEY)
	{
		return 0;
	}

	return 1;
}

bool gObjCheckPersonalCode(int aIndex,char* PersonalCode) // OK
{
	if(gServerInfo.m_PersonalCodeCheck == 0)
	{
		return 1;
	}

	if(strncmp(PersonalCode,&gObj[aIndex].PersonalCode[6],7) == 0)
	{
		return 1;
	}

	return 0;
}

bool gObjCheckResistance(LPOBJ lpObj,int type) // OK
{
	BYTE resist = lpObj->Resistance[type];

	if(resist == 0xFF)
	{
		return 1;
	}

	if((lpObj->Authority & 32) != 0 && (lpObj->Inventory[INVENTORY_SLOT_RING1].m_Index == GET_ITEM(13,42) || lpObj->Inventory[INVENTORY_SLOT_RING2].m_Index == GET_ITEM(13,42)))
	{
		return 1;
	}

	if(gEffectManager.CheckEffect(lpObj,EFFECT_IRON_DEFENSE) != 0 || gEffectManager.CheckEffect(lpObj,EFFECT_IRON_DEFENSE_IMPROVED) != 0)
	{
		return 1;
	}

	if((type == 0 || type == 2) && gEffectManager.CheckEffect(lpObj,EFFECT_SOUL_POTION) != 0)
	{
		resist += (resist*50)/100;
	}

	if((GetLargeRand() % (resist+1)) == 0)
	{
		return 0;
	}

	return 1;
}

bool gObjCheckTeleportArea(int aIndex,int x,int y) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Live == 0 || lpObj->Teleport != TELEPORT_NONE)
	{
		return 0;
	}

	if(x < (lpObj->X - 8) || x >(lpObj->X+8) || y < (lpObj->Y - 8) || y >(lpObj->Y+8))
	{
		return 0;
	}

	if(gMap[lpObj->Map].CheckAttr(lpObj->X,lpObj->Y,1) != 0 || gMap[lpObj->Map].CheckAttr(x,y,1) != 0)
	{
		return 0;
	}

	return 1;
}

bool gObjCheckMapTile(LPOBJ lpObj,int type) // OK
{
	if(lpObj->Type != OBJECT_USER)
	{
		return 0;
	}

	if(CC_MAP_RANGE(lpObj->Map) != 0 && gChaosCastle.GetState(GET_CC_LEVEL(lpObj->Map)) == CC_STATE_START)
	{
		return 0;
	}

	#if(GAMESERVER_UPDATE>=803)

	if(CF_MAP_RANGE(lpObj->Map) != 0 && gChaosCastleFinal.GetState() == CCF_STATE_START)
	{
		return 0;
	}

	#endif

	for(int x = 0; x < 3; x++)
	{
		for(int y = 0; y < 3; y++)
		{
			if(gMap[lpObj->Map].CheckAttr((lpObj->X+x),(lpObj->Y+y),4) == 0 && gMap[lpObj->Map].CheckAttr((lpObj->X+x),(lpObj->Y+y),8) == 0)
			{
				return 0;
			}
		}
	}

	switch(lpObj->Class)
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

	return 1;
}
void gObjCoinAdd(int aIndex,int Coin1,int Coin2,int Coin3) // OK
{
	#if(GAMESERVER_SHOP==1)
	gPcPoint.GDPcPointAddPointSaveSend(aIndex,Coin1);
	#endif

	#if(GAMESERVER_SHOP==2)
	gMuCashShop.GDMuCashPointAddPointSaveSend(aIndex,Coin1);
	#endif

	#if(GAMESERVER_SHOP==3)
	gCashShop.GDCashShopAddPointSaveSend(aIndex,0,Coin1,Coin2,Coin3);
	#endif
}

void gObjCoinSub(int aIndex,int Coin1,int Coin2,int Coin3) // OK
{
	#if(GAMESERVER_SHOP==1)
	gPcPoint.GDPcPointSubPointSaveSend(aIndex,Coin1);
	#endif

	#if(GAMESERVER_SHOP==2)
	gMuCashShop.GDMuCashPointSubPointSaveSend(aIndex,Coin1);
	#endif

	#if(GAMESERVER_SHOP==3)
	gCashShop.GDCashShopSubPointSaveSend(aIndex,0,Coin1,Coin2,Coin3);
	#endif
}

//**************************************************************************//
// ITEM TRANSACTION FUNCTIONS **********************************************//
//**************************************************************************//
bool gObjFixInventoryPointer(int aIndex) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Inventory == lpObj->Inventory1)
	{
		return 1;
	}

	if(lpObj->Inventory == lpObj->Inventory2)
	{
		if(lpObj->Transaction == 1)
		{
			return 0;
		}
		else
		{
			for(int n = 0; n < INVENTORY_SIZE; n++)
			{
				lpObj->Inventory2[n].Clear();
			}
		}
	}

	gObjSetInventory1Pointer(lpObj);
	return 0;
}

void gObjSetInventory1Pointer(LPOBJ lpObj) // OK
{
	lpObj->Inventory = lpObj->Inventory1;
	lpObj->InventoryMap = lpObj->InventoryMap1;
}

void gObjSetInventory2Pointer(LPOBJ lpObj) // OK
{
	lpObj->Inventory = lpObj->Inventory2;
	lpObj->InventoryMap = lpObj->InventoryMap2;
}

bool gObjFixEventInventoryPointer(int aIndex) // OK
{
#if(GAMESERVER_UPDATE>=802)

	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->EventInventory == lpObj->EventInventory1)
	{
		return 1;
	}

	if(lpObj->EventInventory == lpObj->EventInventory2)
	{
		if(lpObj->Transaction == 1)
		{
			return 0;
		}
		else
		{
			for(int n = 0; n < EVENT_INVENTORY_SIZE; n++)
			{
				lpObj->EventInventory2[n].Clear();
			}
		}
	}

	gObjSetEventInventory1Pointer(lpObj);
	return 0;

#else

	return 1;

#endif
}

void gObjSetEventInventory1Pointer(LPOBJ lpObj) // OK
{
#if(GAMESERVER_UPDATE>=802)

	lpObj->EventInventory = lpObj->EventInventory1;
	lpObj->EventInventoryMap = lpObj->EventInventoryMap1;

#endif
}

void gObjSetEventInventory2Pointer(LPOBJ lpObj) // OK
{
#if(GAMESERVER_UPDATE>=802)

	lpObj->EventInventory = lpObj->EventInventory2;
	lpObj->EventInventoryMap = lpObj->EventInventoryMap2;

#endif
}

bool gObjInventoryTransaction(int aIndex) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Transaction == 1)
	{
		return 0;
	}

	for(int n = 0; n < MAX_SKILL_LIST; n++)
	{
		lpObj->SkillBackup[n] = lpObj->Skill[n];
	}

	for(int n = 0; n < INVENTORY_SIZE; n++)
	{
		lpObj->Inventory2[n] = lpObj->Inventory1[n];
	}

#if(GAMESERVER_UPDATE>=802)

	for(int n = 0; n < EVENT_INVENTORY_SIZE; n++)
	{
		lpObj->EventInventory2[n] = lpObj->EventInventory1[n];
	}

#endif

	memcpy(lpObj->InventoryMap2,lpObj->InventoryMap1,INVENTORY_SIZE);

#if(GAMESERVER_UPDATE>=802)

	memcpy(lpObj->EventInventoryMap2,lpObj->EventInventoryMap1,EVENT_INVENTORY_SIZE);

#endif

	gObjSetInventory2Pointer(lpObj);

	gObjSetEventInventory2Pointer(lpObj);

	lpObj->Transaction = 1;
	return 1;
}

bool gObjInventoryCommit(int aIndex) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Transaction != 1)
	{
		return 0;
	}

	for(int n = 0; n < INVENTORY_SIZE; n++)
	{
		lpObj->Inventory1[n] = lpObj->Inventory2[n];
	}

#if(GAMESERVER_UPDATE>=802)

	for(int n = 0; n < EVENT_INVENTORY_SIZE; n++)
	{
		lpObj->EventInventory1[n] = lpObj->EventInventory2[n];
	}

#endif

	memcpy(lpObj->InventoryMap1,lpObj->InventoryMap2,INVENTORY_SIZE);

#if(GAMESERVER_UPDATE>=802)

	memcpy(lpObj->EventInventoryMap1,lpObj->EventInventoryMap2,EVENT_INVENTORY_SIZE);

#endif

	gObjSetInventory1Pointer(lpObj);

	gObjSetEventInventory1Pointer(lpObj);

	lpObj->Transaction = 2;
	return 1;
}

bool gObjInventoryRollback(int aIndex) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Transaction != 1)
	{
		return 0;
	}

	for(int n = 0; n < MAX_SKILL_LIST; n++)
	{
		lpObj->Skill[n] = lpObj->SkillBackup[n];
	}

	for(int n = 0; n < INVENTORY_SIZE; n++)
	{
		lpObj->Inventory2[n].Clear();
	}

	#if(GAMESERVER_UPDATE>=802)

	for(int n = 0; n < EVENT_INVENTORY_SIZE; n++)
	{
		lpObj->EventInventory2[n].Clear();
	}

	#endif

	gObjSetInventory1Pointer(lpObj);

	gObjSetEventInventory1Pointer(lpObj);

	lpObj->Transaction = 3;
	return 1;
}
//**************************************************************************//
// VIEWPORT FUNCTIONS ******************************************************//
//**************************************************************************//
void gObjSetViewport(int aIndex,int state) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	for(int n = 0; n < MAX_VIEWPORT; n++)
	{
		if(lpObj->VpPlayer[n].state == state)
		{
			switch(state)
			{
				case VIEWPORT_SEND:
					lpObj->VpPlayer[n].state = VIEWPORT_WAIT;
					break;
				case VIEWPORT_DESTROY:
					lpObj->VpPlayer[n].state = VIEWPORT_NONE;
					lpObj->VpPlayer[n].index = -1;
					lpObj->VPCount--;
					break;
			}
		}

		if(lpObj->VpPlayerItem[n].state == state)
		{
			switch(state)
			{
				case VIEWPORT_SEND:
					lpObj->VpPlayerItem[n].state = VIEWPORT_WAIT;
					break;
				case VIEWPORT_DESTROY:
					lpObj->VpPlayerItem[n].state = VIEWPORT_NONE;
					lpObj->VpPlayerItem[n].index = -1;
					lpObj->VPCountItem--;
					break;
			}
		}
	}
}

void gObjClearViewport(LPOBJ lpObj) // OK
{
	for(int n = 0; n < MAX_VIEWPORT; n++)
	{
		lpObj->VpPlayer[n].state = VIEWPORT_NONE;
		lpObj->VpPlayer[n].index = -1;
		lpObj->VpPlayer2[n].state = VIEWPORT_NONE;
		lpObj->VpPlayer2[n].index = -1;
		lpObj->VpPlayerItem[n].state = VIEWPORT_NONE;
		lpObj->VpPlayerItem[n].index = -1;
	}

	lpObj->VPCount = 0;
	lpObj->VPCount2 = 0;
	lpObj->VPCountItem = 0;
}

void gObjViewportListProtocolDestroy(LPOBJ lpObj) // OK
{
	gViewport.GCViewportSimpleDestroySend(lpObj);
}

void gObjViewportListProtocolCreate(LPOBJ lpObj) // OK
{
	if(lpObj->Type == OBJECT_USER)
	{
		gViewport.GCViewportSimplePlayerSend(lpObj);
		gViewport.GCViewportSimpleChangeSend(lpObj);
		gViewport.GCViewportSimpleGuildSend(lpObj);
		gViewport.GCViewportSimpleGensSystemSend(lpObj);
	}
	else
	{
		gViewport.GCViewportSimpleMonsterSend(lpObj);
		gViewport.GCViewportSimpleSummonSend(lpObj);
	}
}

void gObjViewportListProtocol(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Connected != OBJECT_ONLINE)
	{
		return;
	}

	if(lpObj->Type == OBJECT_USER)
	{
		gViewport.GCViewportDestroySend(aIndex);
		gViewport.GCViewportDestroyItemSend(aIndex);
	}

	gObjSetViewport(aIndex,VIEWPORT_DESTROY);

	if(lpObj->Type == OBJECT_USER)
	{
		gViewport.GCViewportPlayerSend(aIndex);
		gViewport.GCViewportMonsterSend(aIndex);
		gViewport.GCViewportSummonSend(aIndex);
		gViewport.GCViewportItemSend(aIndex);
		gViewport.GCViewportChangeSend(aIndex);
		gViewport.GCViewportGuildSend(aIndex);
		gViewport.GCViewportUnionSend(aIndex);
		gViewport.GCViewportGensSystemSend(aIndex);
	}

	gObjSetViewport(aIndex,VIEWPORT_SEND);
}

void gObjViewportListDestroy(int aIndex) // OK
{
	if(gObjIsConnected(aIndex) == 0)
	{
		return;
	}

	gViewport.DestroyViewportPlayer1(aIndex);

	gViewport.DestroyViewportPlayer2(aIndex);

	gViewport.DestroyViewportMonster1(aIndex);

	gViewport.DestroyViewportMonster2(aIndex);

	gViewport.DestroyViewportItem(aIndex);
}

void gObjViewportListCreate(int aIndex) // OK
{
	if(gObjIsConnected(aIndex) == 0)
	{
		return;
	}

	if(gObj[aIndex].RegenOk > 0)
	{
		return;
	}

	gViewport.CreateViewportPlayer(aIndex);

	gViewport.CreateViewportMonster(aIndex);

	gViewport.CreateViewportItem(aIndex);
}

void gObjViewportUnionUpdate(int aIndex) // OK
{
	if(gObjIsConnected(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Guild == 0 || lpObj->Type != OBJECT_USER  || lpObj->RegenOk > 0 || lpObj->CloseCount > -1)
	{
		return;
	}

	gViewport.GCViewportSimpleUnionSend(lpObj);
}
//**************************************************************************//
// USER FUNCTIONS **********************************************************//
//**************************************************************************//
void gObjSetKillCount(int aIndex,int type) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(type == 0)
	{
		lpObj->KillCount = 0;
	}

	if(type == 1 && lpObj->KillCount < 255)
	{
		lpObj->KillCount++;
	}

	if(type == 2 && lpObj->KillCount > 0)
	{
		lpObj->KillCount--;
	}

	PMSG_KILL_COUNT_SEND pMsg;

	pMsg.header.set(0xB8,0x01,sizeof(pMsg));

	pMsg.count = lpObj->KillCount;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void gObjTeleportMagicUse(int aIndex,int x,int y) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Teleport != TELEPORT_NONE)
	{
		return;
	}

	lpObj->TeleportTime = GetTickCount();
	lpObj->PathCount = 0;
	lpObj->Teleport = TELEPORT_STANDBY;
	lpObj->ViewState = VIEW_STATE_TELEPORT;
	lpObj->X = x;
	lpObj->Y = y;
	lpObj->TX = x;
	lpObj->TY = y;

	gMap[lpObj->Map].DelStandAttr(lpObj->OldX,lpObj->OldY);
	gMap[lpObj->Map].SetStandAttr(lpObj->TX,lpObj->TY);

	lpObj->OldX = lpObj->TX;
	lpObj->OldY = lpObj->TY;

	gObjViewportListProtocolDestroy(lpObj);
}

void gObjInterfaceCheckTime(LPOBJ lpObj) // OK
{
	if(lpObj->Interface.use == 0)
	{
		return;
	}

	if((GetTickCount() - lpObj->InterfaceTime) < 5000)
	{
		return;
	}

	if(lpObj->Interface.type == INTERFACE_TRADE)
	{
		if(lpObj->Interface.state == 0)
		{
			if(OBJECT_RANGE(lpObj->TargetNumber) != 0)
			{
				gObj[lpObj->TargetNumber].Interface.use = 0;
				gObj[lpObj->TargetNumber].Interface.type = INTERFACE_NONE;
				gObj[lpObj->TargetNumber].Interface.state = 0;
				gObj[lpObj->TargetNumber].TargetNumber = -1;

				gTrade.GCTradeResultSend(lpObj->TargetNumber,3);
			}

			lpObj->Interface.use = 0;
			lpObj->Interface.type = INTERFACE_NONE;
			lpObj->Interface.state = 0;
			lpObj->TargetNumber = -1;

			gTrade.GCTradeResultSend(lpObj->Index,3);
		}
	}

	if(lpObj->Interface.type == INTERFACE_PARTY)
	{
		if(lpObj->Interface.state == 0)
		{
			if(OBJECT_RANGE(lpObj->TargetNumber) != 0)
			{
				gObj[lpObj->TargetNumber].Interface.use = 0;
				gObj[lpObj->TargetNumber].Interface.type = INTERFACE_NONE;
				gObj[lpObj->TargetNumber].Interface.state = 0;
				gObj[lpObj->TargetNumber].TargetNumber = -1;
				gObj[lpObj->TargetNumber].PartyTargetUser = -1;

				gParty.GCPartyResultSend(lpObj->TargetNumber,0);
			}

			lpObj->Interface.use = 0;
			lpObj->Interface.type = INTERFACE_NONE;
			lpObj->Interface.state = 0;
			lpObj->TargetNumber = -1;
			lpObj->PartyTargetUser = -1;

			gParty.GCPartyResultSend(lpObj->Index,0);
		}
	}

	if(lpObj->Interface.type == INTERFACE_GUILD_REQUEST)
	{
		if(lpObj->Interface.state == 0)
		{
			if(OBJECT_RANGE(lpObj->TargetNumber) != 0)
			{
				gObj[lpObj->TargetNumber].Interface.use = 0;
				gObj[lpObj->TargetNumber].Interface.type = INTERFACE_NONE;
				gObj[lpObj->TargetNumber].Interface.state = 0;
				gObj[lpObj->TargetNumber].TargetNumber = -1;

				gGuild.GCGuildResultSend(lpObj->TargetNumber,0);
			}

			lpObj->Interface.use = 0;
			lpObj->Interface.type = INTERFACE_NONE;
			lpObj->Interface.state = 0;
			lpObj->TargetNumber = -1;

			gGuild.GCGuildResultSend(lpObj->Index,0);
		}
	}

	lpObj->InterfaceTime = GetTickCount();
}

void gObjSkillNovaCheckTime(LPOBJ lpObj) // OK
{
	if(lpObj->SkillNovaState == 0)
	{
		return;
	}

	if((GetTickCount() - lpObj->SkillNovaTime) < 500)
	{
		return;
	}

	lpObj->SkillNovaTime = GetTickCount();

	if((++lpObj->SkillNovaCount) == 12)
	{
		CSkill* lpSkill = gSkillManager.GetSkill(lpObj,SKILL_NOVA);

		if(lpSkill != 0)
		{
			gSkillManager.RunningSkill(lpObj->Index,0,lpSkill,0,0,0,0);
		}
	}
	else
	{
		PMSG_SKILL_NOVA_SEND pMsg;

		pMsg.header.set(0xBA,sizeof(pMsg));

		pMsg.index[0] = SET_NUMBERHB(lpObj->Index);

		pMsg.index[1] = SET_NUMBERLB(lpObj->Index);

		pMsg.type = SKILL_NOVA;

		pMsg.count = lpObj->SkillNovaCount;

		DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
		MsgSendV2(lpObj,(BYTE*)&pMsg,pMsg.header.size);
	}
}

void gObjPKDownCheckTime(LPOBJ lpObj,int time,int type) // OK
{
	if(lpObj->PKLevel == 3)
	{
		return;
	}

	int PKLevel = 3;

	if(lpObj->PKLevel < 3)
	{
		if(type == 0)
		{
			lpObj->PKTime -= time;
		}

		if(lpObj->PKTime > gServerInfo.m_PKDownRequirePoint2)
		{
			PKLevel = 0;
		}
		else if(lpObj->PKTime > gServerInfo.m_PKDownRequirePoint1)
		{
			PKLevel = 1;
		}
		else if(lpObj->PKTime > 0)
		{
			PKLevel = 2;
		}
	}
	else
	{
		lpObj->PKTime -= time;

		if(lpObj->PKTime > gServerInfo.m_PKDownRequirePoint2)
		{
			PKLevel = 6;
		}
		else if(lpObj->PKTime > gServerInfo.m_PKDownRequirePoint1)
		{
			PKLevel = 5;
		}
		else if(lpObj->PKTime > 0)
		{
			PKLevel = 4;
		}
	}

	if (lpObj->PKLevel == PKLevel)
	{
		return;
	}

	lpObj->PKLevel = PKLevel;

	GCPKLevelSend(lpObj->Index,lpObj->PKLevel);
}

void gObjUserDie(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	if(lpObj->Type != OBJECT_USER)
	{
		return;
	}

	#if USE_FAKE_ONLINE == TRUE
	if (lpTarget->Type == OBJECT_USER && lpObj->IsFakeOnline == true) //Neu bi nguoi kill
	{
		lpObj->IsFakeRegen = false;
		s_FakeOnline.PostChatMSG(lpObj);
	}
	#endif


	if(OBJECT_RANGE(lpObj->TargetNumber) != 0)
	{
		gTrade.ResetTrade(lpObj->TargetNumber);
		gTrade.GCTradeResultSend(lpObj->TargetNumber,0);
		gTrade.ResetTrade(lpObj->Index);
	}

	if(gDuel.CheckDuel(lpObj,lpTarget) == 0 && lpObj->PKLevel > 4)
	{
		char buff[256];

		wsprintf(buff,gMessage.GetMessage(383),lpObj->Name,lpTarget->Name);

		switch(gServerInfo.m_PKDeathAnnounce)
		{
			case 1:
				PostMessage1("",buff,0);
				break;
			case 2:
				PostMessage2("",buff,0);
				break;
			case 3:
				PostMessage3("",buff,0);
				break;
			case 4:
				PostMessage4("",buff,0);
				break;
		}
	}

	gObjSetKillCount(lpObj->Index,0);

	gObjKillerItemDrop(lpObj,lpTarget);

	if(gCustomArena.CheckMap(lpObj->Map) != 0)
	{
		gCustomArena.UserDieProc(lpObj,lpTarget);
		return;
	}
	else if(DS_MAP_RANGE(lpObj->Map) != 0)
	{
		gDevilSquare.UserDieProc(lpObj,lpTarget);
		return;
	}
	else if(BC_MAP_RANGE(lpObj->Map) != 0)
	{
		gBloodCastle.UserDieProc(lpObj,lpTarget);
		return;
	}
	else if(CC_MAP_RANGE(lpObj->Map) != 0)
	{
		gChaosCastle.UserDieProc(lpObj,lpTarget);
		return;
	}
	#if(GAMESERVER_UPDATE>=301)
	else if(IT_MAP_RANGE(lpObj->Map) != 0)
	{
		gIllusionTemple.UserDieProc(lpObj,lpTarget);
		return;
	}
	#endif
	#if(GAMESERVER_UPDATE>=603)
	else if(DG_MAP_RANGE(lpObj->Map) != 0)
	{
		gDoubleGoer.UserDieProc(lpObj,lpTarget);
		return;
	}
	else if(IG_MAP_RANGE(lpObj->Map) != 0)
	{
		gImperialGuardian.UserDieProc(lpObj,lpTarget);
		return;
	}
	#endif
	#if(GAMESERVER_UPDATE>=803)
	else if(CF_MAP_RANGE(lpObj->Map) != 0)
	{
		gChaosCastleFinal.UserDieProc(lpObj,lpTarget);
		return;
	}
	else if(AB_MAP_RANGE(lpObj->Map) != 0)
	{
		gArcaBattle.UserDieProc(lpObj,lpTarget);
		return;
	}
	#endif

	if(gGuild.gObjTargetGuildWarCheck(lpObj,lpTarget) != 0)
	{
		return;
	}

	gDuel.UserDieProc(lpObj,lpTarget);

	gGensSystem.UserDieProc(lpObj,lpTarget);
}

void gObjKillerItemDrop(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	if(gDuel.CheckDuel(lpObj,lpTarget) != 0)
	{
		return;
	}

	if(gMapManager.GetMapPKItemDrop(lpObj->Map) == 0)
	{
		return;
	}

	if(lpObj->PKLevel > 3)
	{
		int rate = 0;

		if(lpObj->PKLevel == 4)
		{
			rate = (lpTarget->Type == OBJECT_USER) ? gServerInfo.m_PKItemDropRatePvP1 : gServerInfo.m_PKItemDropRatePvM1;
		}
		else if (lpObj->PKLevel == 5)
		{
			rate = (lpTarget->Type == OBJECT_USER) ? gServerInfo.m_PKItemDropRatePvP2 : gServerInfo.m_PKItemDropRatePvM2;
		}
		else
		{
			rate = (lpTarget->Type == OBJECT_USER) ? gServerInfo.m_PKItemDropRatePvP3 : gServerInfo.m_PKItemDropRatePvM3;
		}

		if((GetLargeRand()%100) < rate)
		{
			CRandomManager RandomManager;

			for(int n=0;n<INVENTORY_WEAR_SIZE;n++)
			{
				CItem* lpItem = &lpObj->Inventory[n];

				if(lpItem->IsItem() == 0 || lpItem->m_IsValidItem == 0)
				{
					continue;
				}

				if(lpItem->m_Index == GET_ITEM(4,7) || lpItem->m_Index == GET_ITEM(4,15)) // Bolt, Arrow
				{
					continue;
				}

				if(lpItem->m_Level > gServerInfo.m_PKItemDropMaxLevel)
				{
					continue;
				}

				if(n == 7 && gServerInfo.m_PKItemDropWing == 0)
				{
					continue;
				}

				if(n == 8 && gServerInfo.m_PKItemDropPet == 0)
				{
					continue;
				}

				if(n != 7 && lpItem->IsExcItem() != 0 && GetNewOptionCount(lpItem->m_NewOption) > gServerInfo.m_PKItemDropExc)
				{
					continue;
				}

				if(lpItem->IsSetItem() != 0 && gServerInfo.m_PKItemDropSet == 0)
				{
					continue;
				}

				if(lpItem->IsJewelOfHarmonyItem() != 0 && gServerInfo.m_PKItemDropJoH == 0)
				{
					continue;
				}

				if(lpItem->Is380Item() != 0 && gServerInfo.m_PKItemDrop380 == 0)
				{
					continue;
				}

				RandomManager.AddElement(n,1);
			}

			int slot;

			if(RandomManager.GetRandomElement(&slot) == 0)
			{
				return;
			}
			else
			{
				PMSG_ITEM_DROP_SEND pMsg;

				pMsg.header.set(0x23,sizeof(pMsg));

				pMsg.result = 0;

				pMsg.slot = slot;

				CItem* lpItem = &lpObj->Inventory[slot];

				if(gMap[lpObj->Map].ItemDrop(lpItem->m_Index,lpItem->m_Level,lpItem->m_Durability,lpObj->X,lpObj->Y,lpItem->m_Option1,lpItem->m_Option2,lpItem->m_Option3,lpItem->m_NewOption,lpItem->m_SetOption,lpItem->m_Serial,lpObj->Index,lpItem->m_PetItemLevel,lpItem->m_PetItemExp,lpItem->m_JewelOfHarmonyOption,lpItem->m_ItemOptionEx,lpItem->m_SocketOption,lpItem->m_SocketOptionBonus,lpItem->m_PeriodicItemTime) != 0)
				{
					pMsg.result = 1;
					gItemManager.InventoryDelItem(lpObj->Index,slot);
					gItemManager.UpdateInventoryViewport(lpObj->Index,slot);
				}

				DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size);
			}
		}
	}
}

void gObjPlayerKiller(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	if(lpObj->Type != OBJECT_USER || lpTarget->Type != OBJECT_USER)
	{
		return;
	}

	if(gScriptLoader.OnCheckUserKiller(lpObj->Index,lpTarget->Index) == 0)
	{
		return;
	}

	if(lpObj->Authority == 32 || lpTarget->Authority == 32)
	{
		return;
	}

	if(gDuel.CheckDuel(lpObj,lpTarget) != 0)
	{
		return;
	}

	if(gGuild.gObjGetRelationShip(lpObj,lpTarget) == 2)
	{
		return;
	}

	if(gGuild.gObjTargetGuildWarCheck(lpObj,lpTarget) != 0)
	{
		return;
	}

	if(gGensSystem.CheckGens(lpObj,lpTarget) != 0)
	{
		return;
	}

	if(lpObj->Map == MAP_CASTLE_SIEGE)
	{
		if(gCastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE && lpObj->CsJoinSide != 0)
		{
			return;
		}
		else if(lpObj->Y > 113)
		{
			if(gCastleSiege.CheckCastleOwnerUnionMember(lpObj->Index) != 0)
			{
				return;
			}
		}

		if(lpTarget->PKLevel >= 6)
		{
			return;
		}
	}

	if(gCustomArena.CheckMap(lpObj->Map) != 0 && gCustomArena.CheckMap(lpTarget->Map) != 0)
	{
		return;
	}

	if(CC_MAP_RANGE(lpObj->Map) != 0 && CC_MAP_RANGE(lpTarget->Map) != 0)
	{
		return;
	}

	#if(GAMESERVER_UPDATE>=301)

	if(IT_MAP_RANGE(lpObj->Map) != 0 && IT_MAP_RANGE(lpTarget->Map) != 0)
	{
		return;
	}

	#endif

	#if(GAMESERVER_UPDATE>=803)

	if(AB_MAP_RANGE(lpObj->Map) != 0 && AB_MAP_RANGE(lpTarget->Map) != 0)
	{
		return;
	}

	if(CF_MAP_RANGE(lpObj->Map) != 0 && CF_MAP_RANGE(lpTarget->Map) != 0)
	{
		return;
	}

	#endif

	if(gMapManager.GetMapNonOutlaw(lpObj->Map) != 0)
	{
		return;
	}

	for(int n = 0; n < MAX_SELF_DEFENSE; n++)
	{
		if(OBJECT_RANGE(lpTarget->SelfDefense[n]) != 0)
		{
			if(lpObj->Index == lpTarget->SelfDefense[n])
			{
				return;
			}
		}
	}

	int PKLevel = 3;

	if(lpObj->PKLevel > 3)
	{
		if(lpTarget->PKLevel > 4)
		{
			return;
		}

		lpObj->PKCount++;

		if(lpObj->PKLevel == 4)
		{
			lpObj->PKTime = gServerInfo.m_PKDownRequirePoint2;
		}
		else if(lpObj->PKLevel == 5)
		{
			lpObj->PKTime = gServerInfo.m_PKDownRequirePoint3;
		}
		else
		{
			lpObj->PKTime += gServerInfo.m_PKDownRequirePoint4;
		}

		if(lpObj->PKTime > gServerInfo.m_PKDownRequirePoint2)
		{
			lpObj->PKLevel = 6;
		}
		else if(lpObj->PKTime > gServerInfo.m_PKDownRequirePoint1)
		{
			lpObj->PKLevel = 5;
		}
		else if(lpObj->PKTime > 0)
		{
			lpObj->PKLevel = 4;
		}
	}
	else
	{
		if(lpTarget->PKLevel < 5)
		{
			if(lpObj->PKLevel <= 3)
			{
				lpObj->PKLevel = 3;
				lpObj->PKCount = 0;
			}

			lpObj->PKCount++;

			if(lpObj->PKLevel == 3)
			{
				lpObj->PKTime = gServerInfo.m_PKDownRequirePoint1;
			}
			else if(lpObj->PKLevel == 4)
			{
				lpObj->PKTime = gServerInfo.m_PKDownRequirePoint2;
			}
			else if(lpObj->PKLevel == 5)
			{
				lpObj->PKTime = gServerInfo.m_PKDownRequirePoint3;
			}
			else
			{
				lpObj->PKTime += gServerInfo.m_PKDownRequirePoint4;
			}

			if(lpObj->PKTime > gServerInfo.m_PKDownRequirePoint2)
			{
				lpObj->PKLevel = 6;
			}
			else if(lpObj->PKTime > gServerInfo.m_PKDownRequirePoint1)
			{
				lpObj->PKLevel = 5;
			}
			else if(lpObj->PKTime > 0)
			{
				lpObj->PKLevel = 4;
			}
		}
		else
		{
			lpObj->PKCount++;

			if(lpObj->PKLevel == 3)
			{
				lpObj->PKTime = gServerInfo.m_PKDownRequirePoint1;
			}
			else if(lpObj->PKLevel == 2)
			{
				lpObj->PKTime = gServerInfo.m_PKDownRequirePoint2;
			}
			else if (lpObj->PKLevel == 1)
			{
				lpObj->PKTime = gServerInfo.m_PKDownRequirePoint3;
			}
			else
			{
				lpObj->PKTime += gServerInfo.m_PKDownRequirePoint4;
			}

			if(lpObj->PKTime > gServerInfo.m_PKDownRequirePoint2)
			{
				lpObj->PKLevel = 0;
			}
			else if(lpObj->PKTime > gServerInfo.m_PKDownRequirePoint1)
			{
				lpObj->PKLevel = 1;
			}
			else if(lpObj->PKTime > 0)
			{
				lpObj->PKLevel = 2;
			}
		}
	}

	GCPKLevelSend(lpObj->Index,lpObj->PKLevel);
}

int gObjMoveGate(int aIndex,int gate) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Type == OBJECT_USER)
	{
		gDarkSpirit[aIndex].SetMode(DARK_SPIRIT_MODE_NORMAL,-1);
	}

	if(lpObj->SkillSummonPartyTime != 0)
	{
		lpObj->SkillSummonPartyTime = 0;
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(272));
	}

	if(BC_MAP_RANGE(lpObj->Map) != 0)
	{
		if(gBloodCastle.GetState(GET_BC_LEVEL(lpObj->Map)) == BC_STATE_START)
		{
			gBloodCastle.SearchUserDropEventItem(aIndex);
		}
		else
		{
			gBloodCastle.SearchUserDeleteEventItem(aIndex);
		}
	}

	#if(GAMESERVER_UPDATE>=301)
	
	if(IT_MAP_RANGE(lpObj->Map) != 0)
	{
		if(gIllusionTemple.GetState(GET_IT_LEVEL(lpObj->Map)) == IT_STATE_START)
		{
			gIllusionTemple.SearchUserDropEventItem(aIndex);
		}
		else
		{
			gIllusionTemple.SearchUserDeleteEventItem(aIndex);
		}
	}

	#endif

	if(lpObj->RegenOk != 0 || gGate.IsGate(gate) == 0)
	{
		goto ERROR_JUMP;
	}

	int TargetGate,map,x,y,dir,level;

	if(gGate.GetGate(gate,&TargetGate,&map,&x,&y,&dir,&level) == 0)
	{
		goto ERROR_JUMP;
	}

	if(gMapManager.IsValidMap(map) == 0)
	{
		goto ERROR_JUMP;
	}

	if(lpObj->PShopOpen != 0 && ((lpObj->Map == MAP_CASTLE_SIEGE && map == MAP_LORENCIA) || (lpObj->Map == MAP_LORENCIA && map == MAP_CASTLE_SIEGE)))
	{
		goto ERROR_JUMP;
	}

	#if(GAMESERVER_UPDATE>=501)
	
	if(lpObj->GensFamily == 0 && gMapManager.GetMapGensBattle(map) != 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(229));
		goto ERROR_JUMP;
	}

	#endif

	if(BC_MAP_RANGE(map) != 0 || CC_MAP_RANGE(map) != 0 || DS_MAP_RANGE(map) != 0)
	{
		gEffectManager.DelEffect(lpObj,EFFECT_ELF_BUFFER);
	}

	#if(GAMESERVER_UPDATE>=301)
	
	if(IT_MAP_RANGE(map) != 0)
	{
		gEffectManager.DelEffect(lpObj,EFFECT_ELF_BUFFER);
	}

	#endif

	#if(GAMESERVER_UPDATE>=603)

	if(DG_MAP_RANGE(map) != 0 || IG_MAP_RANGE(map) != 0)
	{
		gEffectManager.DelEffect(lpObj,EFFECT_ELF_BUFFER);
	}

	#endif

	#if(GAMESERVER_UPDATE>=803)

	if(CF_MAP_RANGE(map) != 0)
	{
		gEffectManager.DelEffect(lpObj,EFFECT_ELF_BUFFER);
	}

	#endif

	if(TargetGate == 97)
	{
		if(gCastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
		{
			if(lpObj->CsJoinSide != 1 && gCastleSiege.GetCastleTowerAccessable() == 0)
			{
				goto ERROR_JUMP;
			}

			if(lpObj->CsJoinSide == 0)
			{
				goto ERROR_JUMP;
			}
		}
		else
		{
			if(gCastleSiege.CheckCastleOwnerMember(lpObj->Index) == 0 && gCastleSiege.CheckCastleOwnerUnionMember(lpObj->Index) == 0)
			{
				goto ERROR_JUMP;
			}
		}
	}

	if(map == MAP_ATLANS && lpObj->Inventory[INVENTORY_SLOT_HELPER].IsItem() != 0 && (lpObj->Inventory[INVENTORY_SLOT_HELPER].m_Index == GET_ITEM(13,2) || lpObj->Inventory[INVENTORY_SLOT_HELPER].m_Index == GET_ITEM(13,3))) // Uniria,Dinorant
	{
		gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(274));
		goto ERROR_JUMP;
	}

	#if(GAMESERVER_UPDATE>=201)

	if((map == MAP_ICARUS || map == MAP_KANTURU3) && lpObj->Inventory[INVENTORY_SLOT_WING].IsItem() == 0 && lpObj->Inventory[INVENTORY_SLOT_HELPER].m_Index != GET_ITEM(13,3) && lpObj->Inventory[INVENTORY_SLOT_HELPER].m_Index != GET_ITEM(13,37))
	{
		gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(275));
		goto ERROR_JUMP;
	}

	#else

	if(map == MAP_ICARUS && lpObj->Inventory[INVENTORY_SLOT_WING].IsItem() == 0 && lpObj->Inventory[INVENTORY_SLOT_HELPER].m_Index != GET_ITEM(13,3) && lpObj->Inventory[INVENTORY_SLOT_HELPER].m_Index != GET_ITEM(13,37))
	{
		gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(275));
		goto ERROR_JUMP;
	}

	#endif

	if(KALIMA_MAP_RANGE(lpObj->Map) == 0 && lpObj->Map != map)
	{
		gKalima.DeleteKalimaPortal(aIndex);
	}

	if(KALIMA_MAP_RANGE(lpObj->Map) != 0 && lpObj->Map != map)
	{
		gKalima.DeleteKalimaPortal(aIndex);
	}

	#if(GAMESERVER_UPDATE>=401)

	if(map == MAP_RAKLION2)
	{
		if(gRaklion.GetRaklionState() == RAKLION_STATE_CLOSE_DOOR || gRaklion.GetRaklionState() == RAKLION_STATE_ALL_USER_DIE || gRaklion.GetRaklionState() == RAKLION_STATE_NOTIFY4 || gRaklion.GetRaklionState() == RAKLION_STATE_END)
		{
			gNotice.GCNoticeSend(aIndex,1,0,0,0,0,0,gMessage.GetMessage(145));
			goto ERROR_JUMP;
		}
		else
		{
			if(gate == 290)
			{
				gRaklionBattleUserMng.AddUserData(aIndex);
			}
		}
	}

	if(map == MAP_RAKLION1)
	{
		if(gate == 292)
		{
			if(gRaklion.GetRaklionState() == RAKLION_STATE_CLOSE_DOOR || gRaklion.GetRaklionState() == RAKLION_STATE_ALL_USER_DIE || gRaklion.GetRaklionState() == RAKLION_STATE_NOTIFY4 || gRaklion.GetRaklionState() == RAKLION_STATE_END)
			{
				goto ERROR_JUMP;
			}
			else
			{
				gRaklionBattleUserMng.DelUserData(aIndex);
			}
		}
		else
		{
			gRaklionUtil.NotifyRaklionState(aIndex,gRaklion.GetRaklionState(),gRaklion.GetRaklionDetailState());
		}
	}

	#endif

	if(lpObj->Interface.use != 0 || lpObj->DieRegen != 0)
	{
		lpObj->State = OBJECT_DELCMD;
		lpObj->RegenOk = 1;
		gMove.GCTeleportSend(aIndex,gate,lpObj->Map,(BYTE)lpObj->X,(BYTE)lpObj->Y,lpObj->Dir);
		return 0;
	}

	lpObj->State = OBJECT_DELCMD;

	if(lpObj->Map != map)
	{
		short NextServerCode = gMapServerManager.CheckMapServerMove(aIndex,map,lpObj->LastServerCode);

		if(NextServerCode != gServerInfo.m_ServerCode)
		{
			if(NextServerCode == -1)
			{
				return 0;
			}

			GJMapServerMoveSend(aIndex,NextServerCode,map,x,y);
			return 1;
		}
	}

	lpObj->X = x;
	lpObj->Y = y;
	lpObj->TX = x;
	lpObj->TY = y;
	lpObj->Map = map;
	lpObj->Dir = dir;
	lpObj->PathCount = 0;
	lpObj->Teleport = TELEPORT_NONE;
	lpObj->ViewState = VIEW_STATE_NONE;
	lpObj->MiniMapState = 0;
	lpObj->MiniMapValue = -1;

	gHelper.DisableHelper(lpObj,1);

	gObjClearViewport(lpObj);

	gMove.GCTeleportSend(aIndex,gate,lpObj->Map,(BYTE)lpObj->X,(BYTE)lpObj->Y,lpObj->Dir);

	gObjViewportListProtocolCreate(lpObj);

	gObjectManager.CharacterUpdateMapEffect(lpObj);

	gInvasionManager.CheckEvent(lpObj);

	lpObj->RegenMapNumber = lpObj->Map;
	lpObj->RegenMapX = (BYTE)lpObj->X;
	lpObj->RegenMapY = (BYTE)lpObj->Y;
	lpObj->RegenOk = 1;

	if(lpObj->Type == OBJECT_USER)
	{
		lpObj->LastTeleportTime = 10;
	}

	return 1;

ERROR_JUMP:

	gObjClearViewport(lpObj);

	gMove.GCTeleportSend(aIndex,gate,lpObj->Map,(BYTE)lpObj->X,(BYTE)lpObj->Y,lpObj->Dir);

	gObjViewportListProtocolCreate(lpObj);

	gObjectManager.CharacterUpdateMapEffect(lpObj);

	gInvasionManager.CheckEvent(lpObj);

	lpObj->RegenMapNumber = lpObj->Map;
	lpObj->RegenMapX = (BYTE)lpObj->X;
	lpObj->RegenMapY = (BYTE)lpObj->Y;
	lpObj->RegenOk = 1;
	return 0;
}

void gObjTeleport(int aIndex,int map,int x,int y) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return;
	}

	if(gMapManager.IsValidMap(map) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	lpObj->State = OBJECT_DELCMD;

	if(lpObj->Map != map)
	{
		short NextServerCode = gMapServerManager.CheckMapServerMove(aIndex,map,lpObj->LastServerCode);

		if(NextServerCode != gServerInfo.m_ServerCode)
		{
			if(NextServerCode == -1)
			{
				return;
			}

			GJMapServerMoveSend(aIndex,NextServerCode,map,x,y);
			return;
		}
	}

	if(lpObj->Interface.use != 0 && lpObj->Interface.type == INTERFACE_TRADE)
	{
		gTrade.CGTradeCancelButtonRecv(aIndex);
	}

	if(lpObj->Interface.use != 0 && lpObj->Interface.type == INTERFACE_WAREHOUSE)
	{
		gWarehouse.CGWarehouseClose(aIndex);
	}

	lpObj->X = x;
	lpObj->Y = y;
	lpObj->TX = x;
	lpObj->TY = y;
	lpObj->Map = map;
	lpObj->PathCount = 0;
	lpObj->Teleport = TELEPORT_NONE;
	lpObj->ViewState = VIEW_STATE_NONE;
	lpObj->MiniMapState = 0;
	lpObj->MiniMapValue = -1;

	gHelper.DisableHelper(lpObj,1);

	gObjViewportListProtocolDestroy(lpObj);

	gMove.GCTeleportSend(aIndex,1,lpObj->Map,(BYTE)lpObj->X,(BYTE)lpObj->Y,lpObj->Dir);

	gObjViewportListProtocolCreate(lpObj);

	gObjClearViewport(lpObj);

	gObjectManager.CharacterUpdateMapEffect(lpObj);

	gInvasionManager.CheckEvent(lpObj);

	lpObj->RegenMapNumber = lpObj->Map;
	lpObj->RegenMapX = (BYTE)lpObj->X;
	lpObj->RegenMapY = (BYTE)lpObj->Y;
	lpObj->RegenOk = 1;
}

void gObjSummonAlly(LPOBJ lpObj,int map,int x,int y) // OK
{
	lpObj->SkillSummonPartyTime = 0;
	lpObj->SkillSummonPartyMap = 0;
	lpObj->SkillSummonPartyX = 0;
	lpObj->SkillSummonPartyY = 0;

	if(lpObj->Map == map)
	{
		gSkillManager.GCSkillAttackSend(lpObj,SKILL_TELEPORT,lpObj->Index,1);
		gObjTeleportMagicUse(lpObj->Index,x,y);
		return;
	}

	lpObj->X = x;
	lpObj->Y = y;
	lpObj->TX = x;
	lpObj->TY = y;
	lpObj->Map = map;
	lpObj->PathCount = 0;
	lpObj->Teleport = TELEPORT_NONE;
	lpObj->ViewState = VIEW_STATE_NONE;
	lpObj->MiniMapState = 0;
	lpObj->MiniMapValue = -1;

	gHelper.DisableHelper(lpObj,1);

	gObjViewportListProtocolDestroy(lpObj);

	gMove.GCTeleportSend(lpObj->Index,-1,lpObj->Map,(BYTE)lpObj->X,(BYTE)lpObj->Y,lpObj->Dir);

	gObjViewportListProtocolCreate(lpObj);

	gObjClearViewport(lpObj);

	gObjectManager.CharacterUpdateMapEffect(lpObj);

	gInvasionManager.CheckEvent(lpObj);

	lpObj->RegenMapNumber = lpObj->Map;
	lpObj->RegenMapX = (BYTE)lpObj->X;
	lpObj->RegenMapY = (BYTE)lpObj->Y;
	lpObj->RegenOk = 1;
}

void gObjSkillUseProc(LPOBJ lpObj) // OK
{
	CMonsterSkillElementOption::CheckSkillElementOptionProc(lpObj);

	if(lpObj->Type == OBJECT_USER && lpObj->SkillSummonPartyTime > 0)
	{
		if((--lpObj->SkillSummonPartyTime) == 0)
		{
			if(gMoveSummon.CheckMoveSummon(lpObj,lpObj->SkillSummonPartyMap,lpObj->SkillSummonPartyX,lpObj->SkillSummonPartyY) == 0)
			{
				gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(272));
			}
			else
			{
				gObjSummonAlly(lpObj,lpObj->SkillSummonPartyMap,lpObj->SkillSummonPartyX,lpObj->SkillSummonPartyY);
			}
		}
		else
		{
			gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(273),lpObj->SkillSummonPartyTime);
		}
	}

	if(lpObj->Type == OBJECT_USER && lpObj->DrinkSpeed > 0)
	{
		if(GetTickCount() >= lpObj->DrinkSpeedLastTime)
		{
			lpObj->DrinkSpeed = 0;
			lpObj->DrinkSpeedLastTime = 0;

			gObjectManager.CharacterCalcAttribute(lpObj->Index);
		}
	}

	if(lpObj->Type == OBJECT_USER && lpObj->DrinkDamage > 0)
	{
		if(GetTickCount() >= lpObj->DrinkDamageLastTime)
		{
			lpObj->DrinkDamage = 0;
			lpObj->DrinkDamageLastTime = 0;

			gObjectManager.CharacterCalcAttribute(lpObj->Index);
		}
	}
}

void gObjUserKill(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Attack.Offline != 0 || lpObj->Helper.Offline != 0 || lpObj->PShopCustomOffline != 0)
	{
		CloseClient(aIndex);

		gCustomAttack.AttackOfflineClose(lpObj);

		gCustomStore.OnPShopAlreadyConnected(lpObj);

		gHelperOffline.HelperOfflineClose(lpObj);

		return;
	}

	if(lpObj->CloseCount <= 0)
	{
		lpObj->CloseType = 0;
		lpObj->CloseCount = 6;
	}
}

int gObjInventorySearchSerialNumber(LPOBJ lpObj,DWORD serial) // OK
{
	int count = 0;

	for(int n = 0; n < INVENTORY_SIZE; n++)
	{
		if(lpObj->Inventory[n].m_Serial != 0 && lpObj->Inventory[n].m_Serial == serial && (count++) > 0)
		{
			gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(282));
			gObjUserKill(lpObj->Index);
			return 0;
		}
	}

	return 1;
}

int gObjWarehouseSearchSerialNumber(LPOBJ lpObj,DWORD serial) // OK
{
	int count = 0;

	for(int n = 0; n < WAREHOUSE_SIZE; n++)
	{
		if(lpObj->Warehouse[n].m_Serial != 0 && lpObj->Warehouse[n].m_Serial == serial && (count++) > 0)
		{
			gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(282));
			gObjUserKill(lpObj->Index);
			return 0;
		}
	}

	return 1;
}

void gObjAddMsgSend(LPOBJ lpObj,int MsgCode,int SendUser,int SubCode) // OK
{
	for(int n = 0; n < MAX_MONSTER_SEND_MSG; n++)
	{
		if(gSMMsg[lpObj->Index][n].MsgCode == -1)
		{
			if(gSMMsg.ObjectStruct[lpObj->Index] == gSMMsg.CommonStruct) { gSMMsg.ObjectStruct[lpObj->Index] = new MESSAGE_STATE_MACHINE_COMMON; }

			if(gSMMsg.ObjectStruct[lpObj->Index]->ObjectStruct[n] == gSMMsg.ObjectStruct[lpObj->Index]->CommonStruct) { gSMMsg.ObjectStruct[lpObj->Index]->ObjectStruct[n] = new MESSAGE_STATE_MACHINE; }

			gSMMsg[lpObj->Index][n].MsgCode = MsgCode;
			gSMMsg[lpObj->Index][n].MsgTime = GetTickCount();
			gSMMsg[lpObj->Index][n].SendUser = SendUser;
			gSMMsg[lpObj->Index][n].SubCode = SubCode;
			return;
		}
	}
}

void gObjAddMsgSendDelay(LPOBJ lpObj,int MsgCode,int SendUser,int MsgTimeDelay,int SubCode) // OK
{
	for(int n = 0; n < MAX_MONSTER_SEND_MSG; n++)
	{
		if(gSMMsg[lpObj->Index][n].MsgCode == -1)
		{
			if(gSMMsg.ObjectStruct[lpObj->Index] == gSMMsg.CommonStruct) { gSMMsg.ObjectStruct[lpObj->Index] = new MESSAGE_STATE_MACHINE_COMMON; }

			if(gSMMsg.ObjectStruct[lpObj->Index]->ObjectStruct[n] == gSMMsg.ObjectStruct[lpObj->Index]->CommonStruct) { gSMMsg.ObjectStruct[lpObj->Index]->ObjectStruct[n] = new MESSAGE_STATE_MACHINE; }

			gSMMsg[lpObj->Index][n].MsgCode = MsgCode;
			gSMMsg[lpObj->Index][n].MsgTime = GetTickCount()+MsgTimeDelay;
			gSMMsg[lpObj->Index][n].SendUser = SendUser;
			gSMMsg[lpObj->Index][n].SubCode = SubCode;
			return;
		}
	}
}

void gObjAddAttackProcMsgSendDelay(LPOBJ lpObj,int MsgCode,int SendUser,int MsgTimeDelay,int SubCode,int SubCode2) // OK
{
	for(int n = 0; n < MAX_MONSTER_SEND_ATTACK_MSG; n++)
	{
		if(gSMAttackProcMsg[lpObj->Index][n].MsgCode == -1)
		{
			if(gSMAttackProcMsg.ObjectStruct[lpObj->Index] == gSMAttackProcMsg.CommonStruct) { gSMAttackProcMsg.ObjectStruct[lpObj->Index] = new MESSAGE_STATE_ATTACK_MACHINE_COMMON; }

			if(gSMAttackProcMsg.ObjectStruct[lpObj->Index]->ObjectStruct[n] == gSMAttackProcMsg.ObjectStruct[lpObj->Index]->CommonStruct) { gSMAttackProcMsg.ObjectStruct[lpObj->Index]->ObjectStruct[n] = new MESSAGE_STATE_ATTACK_MACHINE; }

			gSMAttackProcMsg[lpObj->Index][n].MsgCode = MsgCode;
			gSMAttackProcMsg[lpObj->Index][n].MsgTime = GetTickCount()+MsgTimeDelay;
			gSMAttackProcMsg[lpObj->Index][n].SendUser = SendUser;
			gSMAttackProcMsg[lpObj->Index][n].SubCode = SubCode;
			gSMAttackProcMsg[lpObj->Index][n].SubCode2 = SubCode2;
			return;
		}
	}
}

//**************************************************************************//
// OTHERS FUNCTIONS ********************************************************//
//**************************************************************************//

void gObjSecondProc() // OK
{
	for(int n=0;n < MAX_OBJECT;n++)
	{
		LPOBJ lpObj = &gObj[n];

		if(lpObj->Connected > OBJECT_LOGGED)
		{
			if(lpObj->MapServerMoveQuit == 1)
			{
				if(GetTickCount() - lpObj->MapServerMoveQuitTickCount > 30000)
				{
					gObjDel(lpObj->Index);
					continue;
				}
			}

			gObjSkillUseProc(lpObj);

			if(lpObj->Type == OBJECT_MONSTER)
			{
				if(lpObj->Class == 275)
				{
					gKalima.KalimaRefillHP(lpObj);
					continue;
				}
				
				if(lpObj->Class == 278)
				{
					gLifeStone.FindTarget(lpObj->Index);
					continue;
				}

				if(lpObj->Class == 283)
				{
					gGuardianStatue.FindTarget(lpObj->Index);
					continue;
				}

				if(lpObj->Class == 288)
				{
					gCannonTower.FindTarget(lpObj->Index);
					continue;
				}

				if(lpObj->Attribute == ATTRIBUTE_KALIMA)
				{
					gKalima.MainProc(lpObj);
					continue;
				}

				if(lpObj->MonsterDeleteTime != 0 && GetTickCount() >= lpObj->MonsterDeleteTime)
				{
					gObjDel(lpObj->Index);
					continue;
				}
			}
			if(lpObj->Type == OBJECT_NPC)
			{
				if( (lpObj->Class < 204)?FALSE:(lpObj->Class > 209)?FALSE:TRUE)
				{
					gCrywolf.CrywolfNpcAct(lpObj->Index);
				}
				
				if(lpObj->Class == 216)
				{
					gCastleSiegeCrown.CastleSiegeCrownAct(lpObj->Index);
					continue;
				}
				
				if(lpObj->Class == 217 || lpObj->Class == 218)
				{
					gCastleSiegeCrownSwitch.CastleSiegeCrownSwitchAct(lpObj->Index);
					continue;
				}

				if(lpObj->Class == 221 || lpObj->Class == 222)
				{
					gCastleSiegeWeapon.CastleSiegeWeaponAct(lpObj->Index);
				}
			}

			if(lpObj->Type == OBJECT_USER)
			{
				if(lpObj->Inventory[INVENTORY_SLOT_WING].IsItem() == 0 && lpObj->Inventory[INVENTORY_SLOT_HELPER].m_Index != GET_ITEM(13,3) && lpObj->Inventory[INVENTORY_SLOT_HELPER].m_Index != GET_ITEM(13,37))
				{
					if(lpObj->Map == MAP_ICARUS)
					{
						gObjMoveGate(lpObj->Index,22);
					}

					#if(GAMESERVER_UPDATE>=201)
					
					if(lpObj->Map == MAP_KANTURU3 && gKanturu.GetKanturuState() < KANTURU_STATE_BATTLE_OF_NIGHTMARE)
					{
						gObjMoveGate(lpObj->Index,22);
					}

					#endif
				}

				gCustomAttack.SecondProc(lpObj);

								//============================================================================================
				//FakeOnline_EMU
#if USE_FAKE_ONLINE == TRUE
				s_FakeOnline.FakeAttackProc(lpObj); //Attack, Di Chuyen, Move 
#endif
				//============================================================================================


				gCustomStore.OnPShopSecondProc(lpObj);

				gHelperOffline.SecondProc(lpObj);

				gObjCheckMapTile(lpObj,3);

				GCNewHealthBarSend(lpObj);

				if(lpObj->ChatLimitTime > 0)
				{
					lpObj->ChatLimitTimeSec++;

					if(lpObj->ChatLimitTimeSec > 60)
					{
						lpObj->ChatLimitTimeSec = 0;
						lpObj->ChatLimitTime--;

						if(lpObj->ChatLimitTime < 1)
						{
							lpObj->ChatLimitTime = 0;
							gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(283));
						}
					}
				}

				if(lpObj->Map == MAP_CASTLE_SIEGE && gCastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
				{
					if((lpObj->X < 150 || lpObj->X > 200) || (lpObj->Y < 175 || lpObj->Y > 225))
					{
						if(lpObj->AccumulatedCrownAccessTime > 0)
						{
							lpObj->AccumulatedCrownAccessTime = (((lpObj->AccumulatedCrownAccessTime-gServerInfo.m_CastleSiegeDecayAccumulatedTimeValue)<0)?0:(lpObj->AccumulatedCrownAccessTime-gServerInfo.m_CastleSiegeDecayAccumulatedTimeValue));
						}
					}
				}

				gObjectManager.CharacterAutoRecuperation(lpObj);

				if(lpObj->Type == OBJECT_USER && lpObj->LastTeleportTime > 0)
				{
					lpObj->LastTeleportTime--;
				}

				gObjDelayLifeCheck(n);

				gObjectManager.CharacterItemDurationDown(lpObj);

				if(lpObj->PartyNumber >= 0)
				{
					gParty.GCPartyLifeSend(lpObj->PartyNumber);

				#if(GAMESERVER_UPDATE>=802)

					gMiniMap.GCMiniMapPartyInfoSend(lpObj->Index);

					#endif
				}

				#if(GAMESERVER_UPDATE>=801)

				gGuildMatching.GDGuildMatchingInsertSaveSend(lpObj->Index);

				gPartyMatching.GDPartyMatchingInsertSaveSend(lpObj->Index);

				#endif

				if(lpObj->Map == MAP_ARENA)
				{
					if(gBattleSoccer.CheckMapPos(lpObj) != 0)
					{
						gObjMoveGate(lpObj->Index,17);
					}
				}
			}
		}

		if(lpObj->Connected >= OBJECT_LOGGED && lpObj->Type == OBJECT_USER &&	lpObj->CloseCount > 0)
		{
			if(lpObj->CloseCount == 1)
			{
				if(lpObj->CloseType == 1)
				{
					if(gObjectManager.CharacterGameClose(lpObj->Index) == 1)
					{
						GCCloseClientSend(lpObj->Index,1);
					}
				}
				else if(lpObj->CloseType == 0)
				{
					GCCloseClientSend(lpObj->Index,0);
				}
				else if(lpObj->CloseType == 2)
				{
					GCCloseClientSend(lpObj->Index,2);
				}
				else
				{
					GCCloseClientSend(lpObj->Index,2);
				}
			}
			else
			{
				gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(289),(lpObj->CloseCount-1));
			}

			lpObj->CloseCount--;
		}

		if(lpObj->Connected > OBJECT_LOGGED && lpObj->Type == OBJECT_USER)
		{
			if(GetTickCount()-lpObj->AutoSaveTime > 600000)
			{
				GDCharacterInfoSaveSend(lpObj->Index);
				lpObj->AutoSaveTime = GetTickCount();
			}

			gObjPKDownCheckTime(lpObj,gServerInfo.m_PKDownPlusTimePoint,0);
			gObjInterfaceCheckTime(lpObj);
			gObjTimeCheckSelfDefense(lpObj);
		}

		if(lpObj->Connected > OBJECT_OFFLINE && lpObj->Type == OBJECT_USER)
		{
			if((GetTickCount()-lpObj->PacketTime) >= 1000)
			{
				lpObj->PacketCount = 0;
				lpObj->PacketTime = GetTickCount();
			}

			if(lpObj->ClientVerify == 0 && (GetTickCount()-lpObj->ConnectTickCount) > (DWORD)gServerInfo.m_MaxTimeConnectionVerify*1000)
			{
				gLog.Output(LOG_CONNECT,"[ObjectManager][%d] CloseClient [%s] Reason: Not verified",n,lpObj->IpAddr);
				CloseClient(n);
			}

			if((GetTickCount()-lpObj->ConnectTickCount) >= DWORD(gServerInfo.m_MaxConnectionIdle*1000))
			{
				gLog.Output(LOG_CONNECT,"[ObjectManager][%d] CloseClient [%s] Reason: Timeout",n,lpObj->IpAddr);
				CloseClient(n);
			}
		}
	}
}

void gObjDelayLifeCheck(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->CheckLifeTime > 0)
	{
		lpObj->CheckLifeTime--;

		if(lpObj->CheckLifeTime <= 0)
		{
			lpObj->CheckLifeTime = 0;

			if(lpObj->Life < 0)
			{
				lpObj->Life = 0;
			}

			if(lpObj->AttackObj != 0)
			{
				gObjectManager.CharacterLifeCheck(lpObj->AttackObj,lpObj,0,1,0,0,0,0,0);
			}
		}
	}
}

bool gObjBackSpring(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	LPOBJ lpTargetObj = lpTarget;

	if(gMapManager.IsValidMap(lpObj->Map) == 0)
	{
		return 0;
	}

	if(lpObj->Type == OBJECT_USER)
	{
		if(lpObj->Teleport != TELEPORT_NONE)
		{
			return 0;
		}
	}

	if(lpObj->Type == OBJECT_USER)
	{
		if((lpObj->Authority &0x02) == 2)
		{
			return 0;
		}
	}

	if (lpObj->Class >= 131 && lpObj->Class <= 134)
	{
		return 0;
	}

	if (lpObj->Class >= 523 && lpObj->Class <= 528)
	{
		return 0;
	}

	if (lpObj->Class >= 598 && lpObj->Class <= 602)
	{
		return 0;
	}

	if (lpObj->Class >= 627 && lpObj->Class <= 631)
	{
		return 0;
	}

	if (lpObj->Class == 275 || lpObj->Class == 657 || lpObj->Class == 673)
	{
		return 0;
	}

	if((lpObj->Class >= 215 && lpObj->Class <= 219) || lpObj->Class == 277 || lpObj->Class == 278 || lpObj->Class == 283 || lpObj->Class == 288)
	{
		return 0;
	}

	if(gCastleSiege.GetCrownUserIndex() == lpObj->Index)
	{
		if(lpObj->Inventory[INVENTORY_SLOT_HELPER].m_Index == GET_ITEM(13,4))
		{
			return 0;
		}
	}
	
	if(gCrywolf.GetCrywolfState() == 4 || gCrywolf.GetCrywolfState() == 3)
	{
		if(lpObj->Type == OBJECT_USER)
		{
			for(int i=205;i<=209;i++)
			{
				int iAltarIndex = gCrywolfAltar.GetAltarUserIndex(i);
				
				if(iAltarIndex != -1)
				{
					if(iAltarIndex == lpObj->Index)
					{
						return 0;
					}
				}
			}
		}
	}

	if((lpObj->Class >= 204 && lpObj->Class <= 209) || lpObj->Class == 348)
	{
		return 0;
	}

	if((lpObj->Class >= 459 && lpObj->Class <= 462))
	{
		return 0;
	}

	int tdir;

	if((GetLargeRand()%3) == 0)
	{
		if(lpTargetObj->Dir < 4)
		{
			tdir = lpTargetObj->Dir+4;
		}
		else
		{
			tdir = lpTargetObj->Dir-4;
		}

		tdir *= 2;
	}
	else
	{
		tdir = lpTargetObj->Dir*2;
	}

	int x = lpObj->X;
	int y = lpObj->Y;

	x += RoadPathTable[tdir];
	y += RoadPathTable[1+tdir];

	BYTE attr = gMap[lpObj->Map].GetAttr(x,y);

	if((attr & 1) == 1 || (attr & 2) == 2 || (attr & 4) == 4 || (attr & 8) == 8 || (attr & 16) == 16)
	{
		return 0;
	}

	PMSG_POSITION_RECV pMsg;

	pMsg.header.set(PROTOCOL_CODE3,sizeof(pMsg));

	pMsg.x = x;

	pMsg.y = y;

	lpObj->Rest = 0;
	gMap[lpObj->Map].DelStandAttr(lpObj->OldX,lpObj->OldY);
	gMap[lpObj->Map].SetStandAttr(x,y);
	lpObj->OldX = x;
	lpObj->OldY = y;
	CGPositionRecv(&pMsg,lpObj->Index);

	if(lpObj->Type == OBJECT_USER)
	{
		lpObj->PathCount = 0;
	}

	return 1;
}

bool BackSpringCheck(int & x,int & y,int & dir,BYTE map)
{
	BYTE attr;
	int tx = x;
	int ty = y;
	int tdir = dir/2;

	tx = tx+RoadPathTable[dir];
	ty = ty+RoadPathTable[1+dir];

	attr = gMap[map].GetAttr(tx,ty);

	if((attr & 1) == 1 ||
		(attr & 4) == 4 ||
		(attr & 8) == 8
		|| (attr & 16) == 16
		)
	{
		tdir += 4;

		if(tdir > 7)
		{
			tdir -= 8;
		}

		dir = tdir*2;

		return 0;
	}

	x = tx;
	y = ty;

	return 1;
}

bool gObjBackSpring2(LPOBJ lpObj,LPOBJ lpTargetObj,int count)
{
	int tdir;

	if(gMapManager.IsValidMap(lpObj->Map) == 0)
	{
		return 0;
	}

	if(lpObj->Type == OBJECT_USER)
	{
		if(lpObj->Teleport != TELEPORT_NONE)
		{
			return 0;
		}
	}
	
	if(lpObj->Type == OBJECT_USER)
	{
		if((lpObj->Authority &0x02) == 2)
		{
			return 0;
		}
	}

	if (lpObj->Class == 287 || lpObj->Class == 286)
	{
		return 0;
	}

	if (lpObj->Class >= 131 && lpObj->Class <= 134)
	{
		return 0;
	}

	if (lpObj->Class >= 523 && lpObj->Class <= 528)
	{
		return 0;
	}

	if (lpObj->Class >= 598 && lpObj->Class <= 602)
	{
		return 0;
	}

	if (lpObj->Class >= 627 && lpObj->Class <= 631)
	{
		return 0;
	}

	if (lpObj->Class == 275 || lpObj->Class == 657 || lpObj->Class == 673)
	{
		return 0;
	}

	if( lpObj->Class == 277 || lpObj->Class == 278 || lpObj->Class == 283 || lpObj->Class == 288 || (lpObj->Class >= 215 && lpObj->Class <= 219))
	{
		return 0;
	}
	
	if(gCastleSiege.GetCrownUserIndex() == lpObj->Index)
	{
		if(lpObj->Inventory[INVENTORY_SLOT_HELPER].m_Index == GET_ITEM(13,4))
		{
			return 0;
		}
	}
	
	if(gCrywolf.GetCrywolfState() == 4 || gCrywolf.GetCrywolfState() == 3)
	{
		if(lpObj->Type == OBJECT_USER)
		{
			for(int i=205;i<=209;i++)
			{
				int iAltarIndex = gCrywolfAltar.GetAltarUserIndex(i);
				
				if(iAltarIndex != -1)
				{
					if(iAltarIndex == lpObj->Index)
					{
						return 0;
					}
				}
			}
		}
	}
	
	if((lpObj->Class >= 204 && lpObj->Class <= 209) || lpObj->Class == 348)
	{
		return 0;
	}

	if((lpObj->Class >= 459 && lpObj->Class <= 462))
	{
		return 0;
	}

	tdir = GetPathPacketDirPos(lpObj->X - lpTargetObj->X,lpObj->Y - lpTargetObj->Y)*2;

	int x = lpObj->X;
	int y = lpObj->Y;

	for(int n = 0; n<count; n++)
	{
		if(n >= 2)
		{
			if(lpObj->Class == 4 || lpObj->Class == 3 || lpObj->Class == 1)
			{
				BackSpringCheck(x,y,tdir,lpObj->Map);
			}
		}
		else
		{
			BackSpringCheck(x,y,tdir,lpObj->Map);
		}
	}

	PMSG_POSITION_RECV pMsg;

	pMsg.header.set(PROTOCOL_CODE3,sizeof(pMsg));

	pMsg.x = x;

	pMsg.y = y;

	lpObj->Rest = 0;
	gMap[lpObj->Map].DelStandAttr(lpObj->OldX,lpObj->OldY);
	gMap[lpObj->Map].SetStandAttr(x,y);
	lpObj->OldX = x;
	lpObj->OldY = y;
	CGPositionRecv(&pMsg,lpObj->Index);

	return 1;
}

bool gObjIsSelfDefense(LPOBJ lpObj,int aTargetIndex)
{
	if(OBJECT_RANGE(aTargetIndex) == 0)
	{
		return 0;
	}

	for(int n = 0; n<MAX_SELF_DEFENSE; n++)
	{
		if(lpObj->SelfDefense[n] >= 0)
		{
			if(lpObj->SelfDefense[n] == aTargetIndex)
			{
				return 1;
			}
		}
	}

	return 0;
}

void gObjCheckSelfDefense(LPOBJ lpObj,int aTargetIndex)
{
	if(OBJECT_RANGE(aTargetIndex) == 0)
	{
		return;
	}

	if(gObj[aTargetIndex].PKLevel > 4)
	{
		return;
	}

	int iPartyNumber = lpObj->PartyNumber;

	for(int n = 0; n < MAX_SELF_DEFENSE; n++)
	{
		if(gObj[aTargetIndex].SelfDefense[n] >= 0)
		{
			if(gObj[aTargetIndex].SelfDefense[n] == lpObj->Index)
			{
				return;
			}

			if(iPartyNumber >= 0)
			{
				if(gObj[gObj[aTargetIndex].SelfDefense[n]].PartyNumber == iPartyNumber)
				{
					return;
				}
			}
		}
	}

	int blank = -1;

	for(int n = 0; n < MAX_SELF_DEFENSE; n++)
	{
		if(lpObj->SelfDefense[n] >= 0)
		{
			if(lpObj->SelfDefense[n] == aTargetIndex)
			{
				lpObj->SelfDefenseTime[n] = GetTickCount()+60000;
				return;
			}
		}
		else
		{
			blank = n;
		}
	}

	if(blank < 0)
	{
		return;
	}

	lpObj->MySelfDefenseTime = GetTickCount();
	lpObj->SelfDefense[blank] = aTargetIndex;
	lpObj->SelfDefenseTime[blank] = GetTickCount()+60000;

	char szTemp[64];

	wsprintf(szTemp,gMessage.GetMessage(284),lpObj->Name,gObj[aTargetIndex].Name);

	iPartyNumber = gObj[aTargetIndex].PartyNumber;

	if(iPartyNumber >= 0)
	{
		for(int n = 0; n < MAX_PARTY_USER; n++)
		{
			if(gParty.m_PartyInfo[iPartyNumber].Index[n] >= 0)
			{
				gNotice.GCNoticeSend(gParty.m_PartyInfo[iPartyNumber].Index[n],1,0,0,0,0,0,szTemp);
			}
		}
	}
	else
	{
		gNotice.GCNoticeSend(aTargetIndex,1,0,0,0,0,0,szTemp);
	}

	gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,szTemp);
}

void gObjTimeCheckSelfDefense(LPOBJ lpObj) // OK
{
	char buff[256];

	for(int n=0;n < MAX_SELF_DEFENSE;n++)
	{
		if(lpObj->SelfDefense[n] >= 0)
		{
			if(GetTickCount() > lpObj->SelfDefenseTime[n])
			{
				wsprintf(buff,gMessage.GetMessage(285),lpObj->Name);
				gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,buff);
				gNotice.GCNoticeSend(lpObj->SelfDefense[n],1,0,0,0,0,0,buff);
				lpObj->SelfDefense[n] = -1;
			}
		}
	}
}

void gObjSetPosition(int aIndex,int x,int y) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	PMSG_POSITION_RECV pMove;

	pMove.header.set(PROTOCOL_CODE3,sizeof(pMove));

	pMove.x = x;
	pMove.y = y;

	lpObj->Rest = 0;

	gMap[lpObj->Map].DelStandAttr(lpObj->OldX,lpObj->OldY);
	gMap[lpObj->Map].SetStandAttr(x,y);

	lpObj->OldX = x;
	lpObj->OldY = y;

	CGPositionRecv(&pMove,lpObj->Index);
}

void gObjAuthorityCodeSet(LPOBJ lpObj) // OK
{
	if((lpObj->Authority&1) == 1)
	{
		return;
	}

	if((lpObj->Authority&2)== 2)
	{
		lpObj->AuthorityCode = -1;
		return;
	}

	if((lpObj->Authority&4) == 4)
	{
		return;
	}

	if((lpObj->Authority&8) == 8)
	{
		lpObj->AuthorityCode |= 1;
		lpObj->AuthorityCode |= 2;
		lpObj->AuthorityCode |= 4;
		lpObj->AuthorityCode |= 8;
		lpObj->AuthorityCode |= 16;
		lpObj->AuthorityCode |= 32;
		return;
	}

	if((lpObj->Authority&16) == 16)
	{
		return;
	}

	if((lpObj->Authority&32) == 32)
	{
		lpObj->AuthorityCode |= 1;
		lpObj->AuthorityCode |= 2;
		lpObj->AuthorityCode |= 4;
		lpObj->AuthorityCode |= 8;
		lpObj->AuthorityCode |= 16;
		lpObj->AuthorityCode |= 32;
		return;
	}
}

void gObjUseDrink(LPOBJ lpObj,int level) // OK
{
	switch (level)
	{
		case 1: //Olive of Love
			GCItemUseSpecialTimeSend(lpObj->Index,0,gServerInfo.m_OliveOfLoveIncSpeedTime);
			lpObj->DrinkSpeed = gServerInfo.m_OliveOfLoveIncSpeed;
			lpObj->DrinkSpeedLastTime = GetTickCount()+(gServerInfo.m_OliveOfLoveIncSpeedTime*1000);
			gObjectManager.CharacterCalcAttribute(lpObj->Index);
			break;
		case 2: // Remedy of Love
			GCItemUseSpecialTimeSend(lpObj->Index,1,gServerInfo.m_RemedyOfLoveIncDamageTime);
			lpObj->DrinkDamage = gServerInfo.m_RemedyOfLoveIncDamage;
			lpObj->DrinkDamageLastTime = GetTickCount()+(gServerInfo.m_RemedyOfLoveIncDamageTime*1000);
			gObjectManager.CharacterCalcAttribute(lpObj->Index);
			break;
		default: //ALE+0
			GCItemUseSpecialTimeSend(lpObj->Index,0,gServerInfo.m_AleIncSpeedTime);
			lpObj->DrinkSpeed = gServerInfo.m_AleIncSpeed;
			lpObj->DrinkSpeedLastTime = GetTickCount()+(gServerInfo.m_AleIncSpeedTime*1000);
			gObjectManager.CharacterCalcAttribute(lpObj->Index);
			break;
	}
}

void gObjCustomLogPlusChaosMix(LPOBJ lpObj,int type,int index) // OK
{
	if(type != MAX_ITEM_LEVEL-11)
	{
		return;
	}

	if(gServerInfo.m_ChaosMixPlusItemAnnounce != 0)
	{
		char buff[256];

		wsprintf(buff,gMessage.GetMessage(500),lpObj->Name,gItemManager.GetItemName(index,(type+10)),(type+10));

		if(gServerInfo.m_ChaosMixPlusItemAnnounce == 1)
		{
			gNotice.GCNoticeSendToAll(0,0,0,0,0,0,buff);
		}
		else
		{
			GDGlobalNoticeSend(gMapServerManager.GetMapServerGroup(),0,0,0,0,0,0,buff);
		}
	}
}

bool gObjCheckAutoParty(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	if((lpTarget->Option & 4) == 0)
	{
		return 0;
	}

	if(lpTarget->AutoPartyPassword[0] != 0 && strcmp(lpObj->AutoPartyPassword,lpTarget->AutoPartyPassword) != 0)
	{
		return 0;
	}

	return 1;
}

void gObjEventEntrySaveSend(LPOBJ lpObj,int type) // OK
{
	switch(type)
	{
		case 0:
			lpObj->BCCount++;
			break;
		case 1:
			lpObj->CCCount++;
			break;
		case 2:
			lpObj->DSCount++;
			break;
		case 3:
			lpObj->DGCount++;
			break;
		case 4:
			lpObj->ITCount++;
			break;
		case 5:
			lpObj->IGCount++;
			break;
	}

	GDCharacterInfoSaveSend(lpObj->Index);
}