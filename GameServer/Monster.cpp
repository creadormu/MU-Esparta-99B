#include "stdafx.h"
#include "Monster.h"
#include "AcheronGuardian.h"
#include "ArcaBattle.h"
#include "Attack.h"
#include "BattleSoccer.h"
#include "BloodCastle.h"
#include "BonusManager.h"
#include "CastleSiege.h"
#include "ChaosCastle.h"
#include "CriticalSection.h"
#include "Crywolf.h"
#include "CrywolfSync.h"
#include "CustomMonster.h"
#include "CustomMonsterKillCount.h"
#include "DevilSquare.h"
#include "DropEvent.h"
#include "DSProtocol.h"
#include "EffectManager.h"
#include "IllusionTemple.h"
#include "ImperialGuardian.h"
#include "ItemBagManager.h"
#include "ItemDrop.h"
#include "ItemMoneyDrop.h"
#include "ItemOptionRate.h"
#include "JewelOfHarmonyOption.h"
#include "Kalima.h"
#include "LifeStone.h"
#include "Map.h"
#include "MapManager.h"
#include "MapRateInfo.h"
#include "MapServerManager.h"
#include "MemoryAllocator.h"
#include "Mercenary.h"
#include "MonsterAIUtil.h"
#include "MonsterManager.h"
#include "MonsterSetBase.h"
#include "MonsterSkillManager.h"
#include "ObjectManager.h"
#include "Party.h"
#include "Quest.h"
#include "QuestObjective.h"
#include "QuestWorldObjective.h"
#include "Raklion.h"
#include "ServerInfo.h"
#include "SetItemType.h"
#include "ShopManager.h"
#include "SkillManager.h"
#include "SocketItemOption.h"
#include "SocketItemType.h"
#include "Util.h"
#include "Viewport.h"

void gObjMonsterDieGiveItem(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	int aIndex = gObjMonsterGetTopHitDamageUser(lpObj);

	if(OBJECT_RANGE(aIndex) != 0)
	{
		lpTarget = &gObj[aIndex];
	}

	if(OBJECT_RANGE(lpObj->SummonIndex) != 0)
	{
		return;
	}

	gCustomMonsterKillCount.CheckMonsterKill(lpObj,lpTarget);

	if(lpObj->Class == 132 || lpObj->Class == 133 || lpObj->Class == 134) // Statue of Saint
	{
		return;
	}

	if(lpObj->Class == 247 || lpObj->Class == 249 || lpObj->Class == 277 || lpObj->Class == 278 || lpObj->Class == 283 || lpObj->Class == 288 || lpObj->Class == 460 || lpObj->Class == 461 || lpObj->Class == 462 || lpObj->Class == 524 || lpObj->Class == 525 || lpObj->Class == 527 || lpObj->Class == 528 || lpObj->Class == 605 || lpObj->Class == 606 || lpObj->Class == 607)
	{
		return;
	}

	if(lpObj->Class == 476) // Cursed Santa
	{
		gEffectManager.AddEffect(lpTarget,0,EFFECT_CHRISTMAS1,1800,(45+(lpObj->Level/3)),(50+(lpObj->Level/5)),0,0);
		return;
	}

	if(lpObj->Class == 504) // Gaion
	{
		gImperialGuardian.DropMainBossItem(lpObj,lpTarget);
		return;
	}

	if(lpObj->Class == 505) // Destler
	{
		gImperialGuardian.DropSideBossItem(lpObj,lpTarget);
		return;
	}

	if(lpObj->Class == 506) // Vermont
	{
		gImperialGuardian.DropSideBossItem(lpObj,lpTarget);
		return;
	}

	if(lpObj->Class == 507) // Kato
	{
		gImperialGuardian.DropSideBossItem(lpObj,lpTarget);
		return;
	}

	if(lpObj->Class == 508) // Galia
	{
		gImperialGuardian.DropSideBossItem(lpObj,lpTarget);
		return;
	}

	if(lpObj->Class == 509) // Erkanne
	{
		gImperialGuardian.DropSideBossItem(lpObj,lpTarget);
		return;
	}

	if(lpObj->Class == 510) // Raymond
	{
		gImperialGuardian.DropSideBossItem(lpObj,lpTarget);
		return;
	}

	if(lpObj->Class == 511) // Jerint
	{
		gImperialGuardian.DropSideBossItem(lpObj,lpTarget);
		return;
	}

	if(gArcaBattle.MonsterItemDrop(lpObj) != 0)
	{
		return;
	}

	if(lpObj->Class == 627 || lpObj->Class == 628 || lpObj->Class == 629 || lpObj->Class == 630 || lpObj->Class == 631) // Cursed Fire Tower, Cursed Water Tower, Cursed Earth Tower,Cursed Wind Tower,Cursed Darkness Tower
	{
		gAcheronGuardian.DropTowerItem(lpObj,lpTarget);
		return;
	}

	if(gItemBagManager.DropItemByMonsterClass(lpObj->Class,lpTarget,lpObj->Map,lpObj->X,lpObj->Y) != 0)
	{
		return;
	}

	if(gQuestObjective.MonsterItemDrop(lpObj) != 0)
	{
		return;
	}

	if(gQuestWorldObjective.MonsterItemDrop(lpObj) != 0)
	{
		return;
	}

	if(gDropEvent.DropItem(lpObj,lpTarget) != 0)
	{
		return;
	}

	if(gItemDrop.DropItem(lpObj,lpTarget) != 0)
	{
		return;
	}

	if(gItemMoneyDrop.DropMoney(lpObj,lpTarget) != 0)
	{
		return;
	}

	#if(GAMESERVER_UPDATE>=803)

	if(CC_MAP_RANGE(lpObj->Map) != 0 || CF_MAP_RANGE(lpObj->Map) != 0)
	{
		return;
	}

	#else

	if(CC_MAP_RANGE(lpObj->Map) != 0)
	{
		return;
	}

	#endif

	int DropIndex = -1;

	int DropLevel = lpObj->Level;

	int ExcItemDrop = 0;

	int ItemDropRate = gServerInfo.m_ItemDropRate[lpTarget->AccountLevel];

	ItemDropRate = (ItemDropRate*(lpTarget->ItemDropRate+lpTarget->EffectOption.AddItemDropRate))/100;

	ItemDropRate = (ItemDropRate*gMapManager.GetMapItemDropRate(lpTarget->Map))/100;

	ItemDropRate = (ItemDropRate*gBonusManager.GetBonusValue(lpTarget,BONUS_INDEX_ITEM_DROP_RATE,100))/100;

	int ExcItemDropRate = gMapManager.GetMapExcItemDropRate(lpTarget->Map);

	ExcItemDropRate = gBonusManager.GetBonusValue(lpTarget,BONUS_INDEX_EXC_ITEM_DROP_RATE,ExcItemDropRate);

	#if(GAMESERVER_UPDATE>=803)

	int SetItemDropRate = (gMapManager.GetMapSetItemDropRate(lpTarget->Map)+((lpTarget->Map==MAP_ACHERON1)?(100*lpTarget->EffectOption.AddAcheronSetItemDropRate):0));

	#else

	int SetItemDropRate = gMapManager.GetMapSetItemDropRate(lpTarget->Map);

	#endif

	SetItemDropRate = gBonusManager.GetBonusValue(lpTarget,BONUS_INDEX_SET_ITEM_DROP_RATE,SetItemDropRate);

	if(lpObj->Level >= 25 && (GetLargeRand()%1000000) < SetItemDropRate)
	{
		gSetItemType.MakeRandomSetItem(lpTarget->Index,lpObj->Map,lpObj->X,lpObj->Y);
		return;
	}

	if(lpObj->Level >= 25 && (GetLargeRand()%1000000) < ExcItemDropRate)
	{
		DropLevel -= 25;
		ExcItemDrop = 1;
	}

	if((DropIndex=gMonsterManager.GetMonsterItem(DropLevel,ExcItemDrop,gMapManager.GetMapSocketItemDrop(lpObj->Map))) >= GET_ITEM(0,0) && (GetLargeRand()%((lpObj->ItemRate==0)?1:lpObj->ItemRate)) < ItemDropRate)
	{
		WORD ItemIndex = DropIndex;
		BYTE ItemLevel = 0;
		BYTE ItemOption1 = 0;
		BYTE ItemOption2 = 0;
		BYTE ItemOption3 = 0;
		BYTE ItemNewOption = 0;
		BYTE ItemSetOption = 0;
		BYTE ItemSocketOption[MAX_SOCKET_OPTION] = {0xFF,0xFF,0xFF,0xFF,0xFF};

		gItemOptionRate.GetItemOption0(gMapRateInfo.GetDropOption0(lpObj->Map,((ExcItemDrop==0)?DROP_TYPE_COMMON:DROP_TYPE_EXCELLENT)),&ItemLevel);

		gItemOptionRate.GetItemOption1(gMapRateInfo.GetDropOption1(lpObj->Map,((ExcItemDrop==0)?DROP_TYPE_COMMON:DROP_TYPE_EXCELLENT)),&ItemOption1);

		gItemOptionRate.GetItemOption2(gMapRateInfo.GetDropOption2(lpObj->Map,((ExcItemDrop==0)?DROP_TYPE_COMMON:DROP_TYPE_EXCELLENT)),&ItemOption2);

		gItemOptionRate.GetItemOption3(gMapRateInfo.GetDropOption3(lpObj->Map,((ExcItemDrop==0)?DROP_TYPE_COMMON:DROP_TYPE_EXCELLENT)),&ItemOption3);

		gItemOptionRate.GetItemOption4(gMapRateInfo.GetDropOption4(lpObj->Map,((ExcItemDrop==0)?DROP_TYPE_COMMON:DROP_TYPE_EXCELLENT)),&ItemNewOption);

		gItemOptionRate.GetItemOption5(gMapRateInfo.GetDropOption5(lpObj->Map,((ExcItemDrop==0)?DROP_TYPE_COMMON:DROP_TYPE_EXCELLENT)),&ItemSetOption);

		gItemOptionRate.GetItemOption6(gMapRateInfo.GetDropOption6(lpObj->Map,((gItemManager.GetItemTwoHand(ItemIndex)==0)?DROP_TYPE_SOCKET1:DROP_TYPE_SOCKET2)),&ItemSocketOption[0]);

		ItemLevel = ((ItemIndex>=GET_ITEM(12,0))?0:ItemLevel);

		ItemOption1 = ((ItemIndex>=GET_ITEM(12,0))?0:ItemOption1);

		ItemOption2 = ((ItemIndex>=GET_ITEM(12,0))?0:ItemOption2);

		ItemNewOption = ((ItemIndex>=GET_ITEM(12,0))?0:ItemNewOption);

		gItemOptionRate.MakeNewOption(ItemIndex,1,ItemNewOption,&ItemNewOption);

		gItemOptionRate.MakeSetOption(ItemIndex,ItemSetOption,&ItemSetOption);

		gItemOptionRate.MakeSocketOption(ItemIndex,ItemSocketOption[0],&ItemSocketOption[0]);

		GDCreateItemSend(lpTarget->Index,lpObj->Map,(BYTE)lpObj->X,(BYTE)lpObj->Y,ItemIndex,ItemLevel,0,ItemOption1,ItemOption2,ItemOption3,lpTarget->Index,ItemNewOption,ItemSetOption,0,0,ItemSocketOption,0xFF,0);
	}
	else if(lpObj->Money > 0 && (GetLargeRand()%((lpObj->MoneyRate==0)?1:lpObj->MoneyRate)) < (gMapManager.GetMoneyDropRate(lpObj->Map)/10))
	{
		int money = ((__int64)lpObj->Money*gServerInfo.m_MoneyAmountDropRate[lpTarget->AccountLevel])/100;

		money = ((__int64)money*lpTarget->MoneyAmountDropRate)/100;

		money = ((money>0)?money:1);

		gMap[lpObj->Map].MoneyItemDrop(money,lpObj->X,lpObj->Y);
	}
}

bool gObjSetPosMonster(int aIndex,int number) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(number < 0 || number > MAX_MSB_MONSTER)
	{
		return 0;
	}

	MONSTER_SET_BASE_INFO* lpInfo = &gMonsterSetBase.m_MonsterSetBaseInfo[number];

	short x,y;

	if(gMonsterSetBase.GetPosition(number,lpInfo->Map,&x,&y) == 0)
	{
		return 0;
	}

	lpObj->PosNum = number;
	lpObj->X = x;
	lpObj->Y = y;
	lpObj->TX = lpObj->X;
	lpObj->TY = lpObj->Y;
	lpObj->OldX = lpObj->X;
	lpObj->OldY = lpObj->Y;
	lpObj->StartX = (BYTE)lpObj->X;
	lpObj->StartY = (BYTE)lpObj->Y;
	lpObj->Dir = lpInfo->Dir;
	lpObj->Map = lpInfo->Map;
	return 1;
}

bool gObjSetMonster(int aIndex,int MonsterClass) // OK
{
	if(OBJECT_RANGE(aIndex) == 0)
	{
		return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	lpObj->ConnectTickCount = GetTickCount();
	lpObj->ShopNumber = gShopManager.GetShopNumber(MonsterClass,lpObj->Map,lpObj->X,lpObj->Y);
	lpObj->TargetNumber = -1;
	lpObj->SummonIndex = -1;
	lpObj->LastAttackerID = -1;
	lpObj->Connected = OBJECT_ONLINE;
	lpObj->Live = 1;
	lpObj->State = OBJECT_CREATE;
	lpObj->DieRegen = 0;
	lpObj->Class = MonsterClass;

	if(MonsterClass == 200)
	{
		gBattleSoccer.SetBallIndex(aIndex);
	}

	MONSTER_INFO* lpInfo = gMonsterManager.GetInfo(MonsterClass);

	if(lpInfo == 0)
	{
		return 0;
	}

	if(lpInfo->Type == 0)
	{
		lpObj->Type = OBJECT_MONSTER;
	}
	else
	{
		lpObj->Type = OBJECT_NPC;
	}

	memcpy(lpObj->Name,lpInfo->Name,sizeof(lpObj->Name));
	memcpy(lpObj->MonsterName,lpInfo->Name,sizeof(lpObj->MonsterName));

	lpObj->Level = lpInfo->Level;
	lpObj->PhysiSpeed = lpInfo->AttackSpeed;
	lpObj->PhysiDamageMin = lpInfo->DamageMin;
	lpObj->PhysiDamageMax = lpInfo->DamageMax;
	lpObj->Defense = lpInfo->Defense;
	lpObj->MagicDefense = lpInfo->MagicDefense;
	lpObj->AttackSuccessRate = lpInfo->AttackRate;
	lpObj->DefenseSuccessRate = lpInfo->DefenseRate;
	lpObj->Life = (float)lpInfo->Life;
	lpObj->MaxLife = (float)lpInfo->Life;
	lpObj->Mana = (float)lpInfo->Mana;
	lpObj->MaxMana = (float)lpInfo->Mana;
	lpObj->MoveRange = lpInfo->MoveRange;
	lpObj->MoveSpeed = lpInfo->MoveSpeed;
	lpObj->MaxRegenTime = lpInfo->RegenTime*1000;
	lpObj->AttackRange = lpInfo->AttackRange;
	lpObj->ViewRange = lpInfo->ViewRange;
	lpObj->Attribute = lpInfo->Attribute;
	lpObj->AttackType = lpInfo->AttackType;
	lpObj->ItemRate = lpInfo->ItemRate;
	lpObj->MoneyRate = lpInfo->MoneyRate;
	lpObj->Resistance[0] = ((lpInfo->Resistance[0]>255)?255:lpInfo->Resistance[0]);
	lpObj->Resistance[1] = ((lpInfo->Resistance[1]>255)?255:lpInfo->Resistance[1]);
	lpObj->Resistance[2] = ((lpInfo->Resistance[2]>255)?255:lpInfo->Resistance[2]);
	lpObj->Resistance[3] = ((lpInfo->Resistance[3]>255)?255:lpInfo->Resistance[3]);
	lpObj->Resistance[4] = ((lpInfo->Resistance[4]>255)?255:lpInfo->Resistance[4]);
	lpObj->Resistance[5] = ((lpInfo->Resistance[5]>255)?255:lpInfo->Resistance[5]);
	lpObj->Resistance[6] = ((lpInfo->Resistance[6]>255)?255:lpInfo->Resistance[6]);
	lpObj->ScriptMaxLife = (float)lpInfo->ScriptLife;
	lpObj->BasicAI = 0;
	lpObj->CurrentAI = 0;
	lpObj->CurrentAIState = 0;
	lpObj->LastAIRunTime = 0;
	lpObj->GroupNumber = 0;
	lpObj->SubGroupNumber = 0;
	lpObj->GroupMemberGuid = -1;
	lpObj->RegenType = 0;

	lpObj->Agro.ResetAll();

	#if(GAMESERVER_UPDATE>=701)
	lpObj->ElementalAttribute = ((lpInfo->ElementalAttribute==6)?((GetLargeRand()%5)+1):lpInfo->ElementalAttribute);
	lpObj->ElementalPattern = lpInfo->ElementalPattern;
	lpObj->ElementalDefense = lpInfo->ElementalDefense;
	lpObj->ElementalDamageMin = lpInfo->ElementalDamageMin;
	lpObj->ElementalDamageMax = lpInfo->ElementalDamageMax;
	lpObj->ElementalAttackSuccessRate = lpInfo->ElementalAttackRate;
	lpObj->ElementalDefenseSuccessRate = lpInfo->ElementalDefenseRate;
	#endif

	gObjSetInventory1Pointer(lpObj);

	gCustomMonster.SetCustomMonsterInfo(lpObj);

	if(lpObj->AttackType != 0)
	{
		if(lpObj->AttackType == 150)
		{
			gSkillManager.AddSkill(lpObj,SKILL_MONSTER_AREA_ATTACK,0);
		}
		else
		{
			gSkillManager.AddSkill(lpObj,lpObj->AttackType,0);
		}

		if(MonsterClass == 66 || MonsterClass == 73) // Cursed King,Drakan
		{
			gSkillManager.AddSkill(lpObj,SKILL_ENERGY_BALL,0);
		}

		if(MonsterClass == 77) // Phoenix of Darkness
		{
			gSkillManager.AddSkill(lpObj,SKILL_LIGHTNING,0);
		}

		if(MonsterClass == 89 || MonsterClass == 95 || MonsterClass == 112 || MonsterClass == 118 || MonsterClass == 124 || MonsterClass == 130 || MonsterClass == 143 || MonsterClass == 433) // Spirit Sorcerer
		{
			gSkillManager.AddSkill(lpObj,SKILL_LIGHTNING,0);
		}

		if(MonsterClass == 144 || MonsterClass == 174 || MonsterClass == 182 || MonsterClass == 190 || MonsterClass == 260 || MonsterClass == 268 && MonsterClass == 331) // Kalima Death Angel
		{
			gSkillManager.AddSkill(lpObj,SKILL_ENERGY_BALL,0);
		}

		if(MonsterClass == 145 || MonsterClass == 175 || MonsterClass == 183 || MonsterClass == 191 || MonsterClass == 261 || MonsterClass == 269 && MonsterClass == 332) // Kalima Death Centurion
		{
			gSkillManager.AddSkill(lpObj,SKILL_ENERGY_BALL,0);
		}

		if(MonsterClass == 146 || MonsterClass == 176 || MonsterClass == 184 || MonsterClass == 192 || MonsterClass == 262 || MonsterClass == 270 && MonsterClass == 333) // Kalima Bloody Soldier
		{
			gSkillManager.AddSkill(lpObj,SKILL_ENERGY_BALL,0);
		}

		if(MonsterClass == 147 || MonsterClass == 177 || MonsterClass == 185 || MonsterClass == 193 || MonsterClass == 263 || MonsterClass == 271 && MonsterClass == 334) // Kalima Aegis
		{
			gSkillManager.AddSkill(lpObj,SKILL_ENERGY_BALL,0);
		}

		if(MonsterClass == 148 || MonsterClass == 178 || MonsterClass == 186 || MonsterClass == 194 || MonsterClass == 264 || MonsterClass == 272 && MonsterClass == 335) // Kalima Lord Centurion
		{
			gSkillManager.AddSkill(lpObj,SKILL_ENERGY_BALL,0);
		}

		if(MonsterClass == 149 || MonsterClass == 179 || MonsterClass == 187 || MonsterClass == 195 || MonsterClass == 265 || MonsterClass == 273 && MonsterClass == 336) // Kalima Necron
		{
			gSkillManager.AddSkill(lpObj,SKILL_POISON,0);
			gSkillManager.AddSkill(lpObj,SKILL_ENERGY_BALL,0);
		}

		if(MonsterClass == 160 || MonsterClass == 180 || MonsterClass == 188 || MonsterClass == 196 || MonsterClass == 266 || MonsterClass == 274 && MonsterClass == 337) // Kalima Schriker
		{
			gSkillManager.AddSkill(lpObj,SKILL_ENERGY_BALL,0);
		}

		if(MonsterClass == 161 || MonsterClass == 181 || MonsterClass == 189 || MonsterClass == 197 || MonsterClass == 267 || MonsterClass == 275 && MonsterClass == 338) // Shadow of Kundun
		{
			gSkillManager.AddSkill(lpObj,SKILL_POISON,0);
			gSkillManager.AddSkill(lpObj,SKILL_ENERGY_BALL,0);
			gSkillManager.AddSkill(lpObj,SKILL_FIRE_SLASH,0);
			gSkillManager.AddSkill(lpObj,SKILL_MONSTER_SUMMON,0);
			gSkillManager.AddSkill(lpObj,SKILL_MAGIC_DAMAGE_IMMUNITY,0);
			gSkillManager.AddSkill(lpObj,SKILL_PHYSI_DAMAGE_IMMUNITY,0);
		}

		if(MonsterClass == 163 || MonsterClass == 165 || MonsterClass == 167 || MonsterClass == 169 || MonsterClass == 171 || MonsterClass == 173 || MonsterClass == 427) // Chaos Castle Wizard
		{
			gSkillManager.AddSkill(lpObj,SKILL_ENERGY_BALL,0);
		}

		if(MonsterClass == 529 || MonsterClass == 530)
		{
			gSkillManager.AddSkill(lpObj,SKILL_MONSTER_AREA_ATTACK,0);
		}

		if(MonsterClass == 533)
		{
			gSkillManager.AddSkill(lpObj,SKILL_SELF_EXPLOSION,0);
		}

		if(MonsterClass == 534)
		{
			gSkillManager.AddSkill(lpObj,SKILL_FIVE_SHOT,0);
		}

		if(MonsterClass == 535)
		{
			gSkillManager.AddSkill(lpObj,SKILL_TWISTING_SLASH,0);
		}

		if(MonsterClass == 536)
		{
			gSkillManager.AddSkill(lpObj,SKILL_ICE_STORM,0);
		}

		if(MonsterClass == 537)
		{
			gSkillManager.AddSkill(lpObj,SKILL_POWER_SLASH,0);
		}

		if(MonsterClass == 538)
		{
			gSkillManager.AddSkill(lpObj,SKILL_EARTHQUAKE,0);
		}

		if(MonsterClass == 539)
		{
			gSkillManager.AddSkill(lpObj,SKILL_RED_STORM,0);
		}

		if(MonsterClass == 561)
		{
			gSkillManager.AddSkill(lpObj,SKILL_EVIL_SPIRIT,0);
			gSkillManager.AddSkill(lpObj,SKILL_DECAY,0);
			gSkillManager.AddSkill(lpObj,SKILL_BIRDS,0);
		}
	}

	gMap[lpObj->Map].SetStandAttr(lpObj->X,lpObj->Y);
	lpObj->OldX = lpObj->X;
	lpObj->OldY = lpObj->Y;
	return 1;
}

bool gObjMonsterRegen(LPOBJ lpObj) // OK
{
	gObjClearViewport(lpObj);

	lpObj->TargetNumber = -1;
	lpObj->LastAttackerID = -1;
	lpObj->NextActionTime = 5000;
	lpObj->ActionState.Reset();

	if(lpObj->Attribute == ATTRIBUTE_KALIMA)
	{
		lpObj->Live = 1;
		lpObj->DieRegen = 0;
		gObjClearViewport(lpObj);
		gObjViewportListCreate(lpObj->Index);
		gObjViewportListProtocolCreate(lpObj);
		return 0;
	}

	if(lpObj->Attribute == ATTRIBUTE_CASTLE || lpObj->Attribute == ATTRIBUTE_ACHERON || lpObj->Attribute == ATTRIBUTE_INVASION)
	{
		int px = lpObj->X;
		int py = lpObj->Y;

		if(gObjGetRandomFreeLocation(lpObj->Map,&px,&py,5,5,30) == 0)
		{
			lpObj->Live = 0;
			lpObj->State = OBJECT_DYING;
			lpObj->DieRegen = 1;
			lpObj->RegenTime = GetTickCount();
			return 0;
		}

		lpObj->X = px;
		lpObj->Y = py;
	}
	else
	{
		if(gMonsterSetBase.GetPosition(lpObj->PosNum,lpObj->Map,&lpObj->X,&lpObj->Y) == 0)
		{
			lpObj->Live = 0;
			lpObj->State = OBJECT_DYING;
			lpObj->DieRegen = 1;
			lpObj->RegenTime = GetTickCount();
			return 0;
		}
	}

	lpObj->MTX = lpObj->X;
	lpObj->MTY = lpObj->Y;
	lpObj->TX = lpObj->X;
	lpObj->TY = lpObj->Y;
	lpObj->StartX = (BYTE)lpObj->X;
	lpObj->StartY = (BYTE)lpObj->Y;
	lpObj->PathCur = 0;
	lpObj->PathCount = 0;
	lpObj->PathStartEnd = 0;

	gObjMonsterInitHitDamage(lpObj);

	if(lpObj->Map != MAP_CRYWOLF || (gCrywolfSync.GetCrywolfState() != CRYWOLF_STATE_NOTIFY_2 && gCrywolfSync.GetCrywolfState() != CRYWOLF_STATE_READY && gCrywolfSync.GetCrywolfState() != CRYWOLF_STATE_START && gCrywolfSync.GetCrywolfState() != CRYWOLF_STATE_END))
	{
		if(gCrywolfSync.CheckApplyBenefit() != 0 && gCrywolfSync.GetOccupationState() == 0)
		{
			lpObj->Life = (lpObj->ScriptMaxLife*gCrywolfSync.GetMonHPBenefitRate())/100;
			lpObj->MaxLife = (lpObj->ScriptMaxLife*gCrywolfSync.GetMonHPBenefitRate())/100;
		}
	}

	return 1;
}

bool gObjMonsterMoveCheck(LPOBJ lpObj,int tx,int ty) // OK
{
	if(lpObj->ActionState.Emotion == EMOTION_ATTACK)
	{
		return 1;
	}

	if(lpObj->PosNum == -1 && lpObj->MoveRange != 0)
	{
		if(sqrt((pow((float)(tx-lpObj->StartX),(float)2)+pow((float)(ty-lpObj->StartY),(float)2))) > lpObj->MoveRange)
		{
			return 0;
		}

		return 1;
	}

	#if(GAMESERVER_UPDATE>=201)

	if(lpObj->PosNum == -1 && lpObj->Map == MAP_KANTURU3)
	{
		if(sqrt((pow((float)(tx-lpObj->StartX),(float)2)+pow((float)(ty-lpObj->StartY),(float)2))) > 30)
		{
			return 0;
		}
	}
	else
	{
		if(lpObj->PosNum == -1)
		{
			return 0;
		}

		if(gMonsterSetBase.m_MonsterSetBaseInfo[lpObj->PosNum].Dis == 0)
		{
			return 0;
		}

		if(sqrt((pow((float)(tx-lpObj->StartX),(float)2)+pow((float)(ty-lpObj->StartY),(float)2))) > gMonsterSetBase.m_MonsterSetBaseInfo[lpObj->PosNum].Dis)
		{
			return 0;
		}
	}

	#else

	if(lpObj->PosNum == -1)
	{
		return 0;
	}

	if(gMonsterSetBase.m_MonsterSetBaseInfo[lpObj->PosNum].Dis == 0)
	{
		return 0;
	}

	if(sqrt((pow((float)(tx-lpObj->StartX),(float)2)+pow((float)(ty-lpObj->StartY),(float)2))) > gMonsterSetBase.m_MonsterSetBaseInfo[lpObj->PosNum].Dis)
	{
		return 0;
	}

	#endif

	return 1;
}

void gObjMonsterInitHitDamage(LPOBJ lpObj) // OK
{
	for(int n=0;n < MAX_HIT_DAMAGE;n++)
	{
		lpObj->HitDamage[n].index = -1;
		lpObj->HitDamage[n].damage = 0;
		lpObj->HitDamage[n].time = 0;
	}

	lpObj->HitDamageCount = 0;
}

void gObjMonsterSetHitDamage(LPOBJ lpObj,int aIndex,int damage) // OK
{
	int HitDamageIndex = -1;

	for(int n=0;n < MAX_HIT_DAMAGE;n++)
	{
		if(OBJECT_RANGE(lpObj->HitDamage[n].index) == 0)
		{
			HitDamageIndex = n;
			continue;
		}

		if(lpObj->HitDamage[n].index == aIndex)
		{
			lpObj->HitDamage[n].damage = (((lpObj->HitDamage[n].damage+damage)>lpObj->MaxLife)?(int)lpObj->MaxLife:(lpObj->HitDamage[n].damage+damage));
			lpObj->HitDamage[n].time = GetTickCount();
			return;
		}
	}

	if(CHECK_RANGE(HitDamageIndex,MAX_HIT_DAMAGE) != 0 || OBJECT_RANGE(HitDamageIndex=gObjMonsterDelHitDamageUser(lpObj)) != 0)
	{
		lpObj->HitDamage[HitDamageIndex].index = aIndex;

		lpObj->HitDamage[HitDamageIndex].damage = ((damage>lpObj->MaxLife)?(int)lpObj->MaxLife:damage);

		lpObj->HitDamage[HitDamageIndex].time = GetTickCount();

		lpObj->HitDamageCount++;
	}
}

int gObjMonsterDelHitDamageUser(LPOBJ lpObj) // OK
{
	int HitDamageIndex = -1;

	for(int n=0;n < MAX_HIT_DAMAGE;n++)
	{
		if(OBJECT_RANGE(lpObj->HitDamage[n].index) == 0)
		{
			continue;
		}

		int result = 0;

		if(gObjIsConnected(lpObj->HitDamage[n].index) == 0)
		{
			result = 1;
		}
		else if((GetTickCount()-lpObj->HitDamage[n].time) > (DWORD)(gServerInfo.m_MonsterGetTopHitDamageUserMaxTime*1000))
		{
			result = 1;
		}
		else if(lpObj->Map != gObj[lpObj->HitDamage[n].index].Map)
		{
			result = 1;
		}
		else if(gObjCalcDistance(lpObj,&gObj[lpObj->HitDamage[n].index]) > 20)
		{
			result = 1;
		}

		if(result != 0)
		{
			lpObj->HitDamage[n].index = -1;
			lpObj->HitDamageCount--;
			HitDamageIndex = n;
		}
	}

	return HitDamageIndex;
}

int gObjMonsterGetTopHitDamageUser(LPOBJ lpObj) // OK
{
	int TopHitDamageUser = -1;
	int TopHitDamage = 0;
	int PartyTopHitDamageUser = -1;
	int PartyTopHitDamage = 0;

	for(int n=0;n < MAX_HIT_DAMAGE;n++)
	{
		if(gObjIsConnected(lpObj->HitDamage[n].index) == 0)
		{
			continue;
		}

		if((GetTickCount()-lpObj->HitDamage[n].time) > (DWORD)(gServerInfo.m_MonsterGetTopHitDamageUserMaxTime*1000))
		{
			continue;
		}

		if(lpObj->Map != gObj[lpObj->HitDamage[n].index].Map)
		{
			continue;
		}

		if(gObjCalcDistance(lpObj,&gObj[lpObj->HitDamage[n].index]) > 20)
		{
			continue;
		}

		if(OBJECT_RANGE(gObj[lpObj->HitDamage[n].index].PartyNumber) == 0)
		{
			if(lpObj->HitDamage[n].damage > TopHitDamage)
			{
				TopHitDamageUser = lpObj->HitDamage[n].index;
				TopHitDamage = lpObj->HitDamage[n].damage;
			}
		}
		else
		{
			if((PartyTopHitDamage=gObjMonsterGetTopHitDamageParty(lpObj,gObj[lpObj->HitDamage[n].index].PartyNumber,&PartyTopHitDamageUser)) > TopHitDamage)
			{
				TopHitDamageUser = PartyTopHitDamageUser;
				TopHitDamage = PartyTopHitDamage;
			}
		}
	}

	return TopHitDamageUser;
}

int gObjMonsterGetTopHitDamageParty(LPOBJ lpObj,int PartyNumber,int* TopHitDamageUser) // OK
{
	int TopHitDamage = 0;
	int TotalHitDamage = 0;

	for(int n=0;n < MAX_HIT_DAMAGE;n++)
	{
		if(gObjIsConnected(lpObj->HitDamage[n].index) == 0)
		{
			continue;
		}

		if((GetTickCount()-lpObj->HitDamage[n].time) > (DWORD)(gServerInfo.m_MonsterGetTopHitDamageUserMaxTime*1000))
		{
			continue;
		}

		if(lpObj->Map != gObj[lpObj->HitDamage[n].index].Map)
		{
			continue;
		}

		if(gObjCalcDistance(lpObj,&gObj[lpObj->HitDamage[n].index]) > 20)
		{
			continue;
		}

		for(int i=0;i < MAX_PARTY_USER;i++)
		{
			if(gParty.m_PartyInfo[PartyNumber].Index[i] == lpObj->HitDamage[n].index)
			{
				if(lpObj->HitDamage[n].damage > TopHitDamage)
				{
					(*TopHitDamageUser) = lpObj->HitDamage[n].index;
					TopHitDamage = lpObj->HitDamage[n].damage;
					TotalHitDamage += lpObj->HitDamage[n].damage;
				}
				else
				{
					(*TopHitDamageUser) = (*TopHitDamageUser);
					TopHitDamage = TopHitDamage;
					TotalHitDamage += lpObj->HitDamage[n].damage;
				}
			}
		}
	}

	return TotalHitDamage;
}

int gObjAddMonster(int map) // OK
{
	static CCriticalSection critical;

	critical.lock();

	int index = -1;
	int count = gObjMonCount;

	if(gMapServerManager.CheckMapServer(map) == 0)
	{
		critical.unlock();
		return -1;
	}

	if(gMemoryAllocator.GetMemoryAllocatorFree(&index,OBJECT_START_MONSTER,MAX_OBJECT_MONSTER,10000) != 0)
	{
		gObjAllocData(index);
		gObjCharZeroSet(index);
		gObj[index].Index = index;
		gObj[index].Connected = OBJECT_CONNECTED;
		critical.unlock();
		return index;
	}

	for(int n=OBJECT_START_MONSTER;n < MAX_OBJECT_MONSTER;n++)
	{
		if(gObj[count].Connected == OBJECT_OFFLINE)
		{
			if(gObjAllocData(count) == 0)
			{
				break;
			}

			gObjCharZeroSet(count);
			gObj[count].Index = count;
			gObj[count].Connected = OBJECT_CONNECTED;
			critical.unlock();
			return count;
		}
		else
		{
			count = (((++count)>=MAX_OBJECT_MONSTER)?OBJECT_START_MONSTER:count);
		}
	}

	critical.unlock();
	return -1;
}

int gObjAddSummon() // OK
{
	static CCriticalSection critical;

	critical.lock();

	int index = -1;
	int count = gObjCallMonCount;

	if(gMemoryAllocator.GetMemoryAllocatorFree(&index,MAX_OBJECT_MONSTER,OBJECT_START_USER,10000) != 0)
	{
		gObjAllocData(index);
		gObjCharZeroSet(index);
		gObj[index].Index = index;
		gObj[index].Connected = OBJECT_CONNECTED;
		critical.unlock();
		return index;
	}

	for(int n=MAX_OBJECT_MONSTER;n < OBJECT_START_USER;n++)
	{
		if(gObj[count].Connected == OBJECT_OFFLINE)
		{
			if(gObjAllocData(count) == 0)
			{
				break;
			}

			gObjCharZeroSet(count);
			gObj[count].Index = count;
			gObj[count].Connected = OBJECT_CONNECTED;
			critical.unlock();
			return count;
		}
		else
		{
			count = (((++count)>=OBJECT_START_USER)?MAX_OBJECT_MONSTER:count);
		}
	}

	critical.unlock();
	return -1;
}

int gObjSummonSetEnemy(LPOBJ lpObj,int aIndex) // OK
{
	if(lpObj->Type != OBJECT_USER)
	{
		return -1;
	}

	LPOBJ lpSummon = &gObj[lpObj->SummonIndex];

	if(OBJECT_RANGE(lpSummon->TargetNumber) != 0)
	{
		return -1;
	}

	if(gObjIsConnected(aIndex) == 0 || aIndex == lpObj->Index || aIndex == lpSummon->Index)
	{
		return -1;
	}

	lpSummon->TargetNumber = aIndex;
	lpSummon->ActionState.Emotion = EMOTION_ATTACK;
	lpSummon->ActionState.EmotionCount = 30;
	return aIndex;
}

void gObjSummonKill(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(OBJECT_RANGE(lpObj->SummonIndex) == 0)
	{
		return;
	}

	LPOBJ lpSummon = &gObj[lpObj->SummonIndex];

	if(lpSummon->Type != OBJECT_MONSTER || lpSummon->SummonIndex != aIndex)
	{
		return;
	}

	if(gObjIsConnected(lpSummon->Index) != 0)
	{
		lpSummon->SummonIndex = -1;
	}

	lpObj->SummonIndex = -1;

	gObjDel(lpSummon->Index);
}

void gObjMonsterMagicAttack(LPOBJ lpObj,int skill) // OK
{
	CSkill* lpSkill = &lpObj->Skill[skill];

	if(lpSkill->IsSkill() == 0)
	{
		return;
	}

	gSkillManager.GCDurationSkillAttackSend(lpObj,lpSkill->m_index,(BYTE)lpObj->X,(BYTE)lpObj->Y,0);

	BYTE send[256];

	PMSG_MULTI_SKILL_ATTACK_RECV pMsg;

	pMsg.header.set(PROTOCOL_CODE4,sizeof(pMsg));

	int size = sizeof(pMsg);

	#if(GAMESERVER_UPDATE>=701)

	pMsg.skillH = SET_NUMBERHB(0);

	pMsg.skillL = SET_NUMBERLB(0);

	#elif(GAMESERVER_UPDATE>=401)

	pMsg.skill[0] = SET_NUMBERHB(0);

	pMsg.skill[1] = SET_NUMBERLB(0);

	#else

	pMsg.skill = 0;

	#endif

	pMsg.x = (BYTE)lpObj->X;

	pMsg.y = (BYTE)lpObj->Y;

	pMsg.serial = 0;

	pMsg.count = 0;

	PMSG_MULTI_SKILL_ATTACK info;

	for(int n=0;n < MAX_VIEWPORT;n++)
	{
		if(lpObj->VpPlayer2[n].state == VIEWPORT_NONE || OBJECT_RANGE(lpObj->VpPlayer2[n].index) == 0 || lpObj->VpPlayer2[n].type != OBJECT_USER)
		{
			continue;
		}

		if(gObjCalcDistance(lpObj,&gObj[lpObj->VpPlayer2[n].index]) >= 6)
		{
			continue;
		}

		#if(GAMESERVER_UPDATE>=701)

		info.indexH = SET_NUMBERHB(lpObj->VpPlayer2[n].index);

		info.indexL = SET_NUMBERLB(lpObj->VpPlayer2[n].index);

		#else

		info.index[0] = SET_NUMBERHB(lpObj->VpPlayer2[n].index);

		info.index[1] = SET_NUMBERLB(lpObj->VpPlayer2[n].index);

		#endif

		info.MagicKey = 0;

		memcpy(&send[size],&info,sizeof(info));
		size += sizeof(info);

		if(CHECK_SKILL_ATTACK_EXTENDED_COUNT(pMsg.count) == 0)
		{
			break;
		}
	}

	pMsg.header.size = size;

	memcpy(send,&pMsg,sizeof(pMsg));

	if(lpObj->Class == 161 || lpObj->Class == 181 || lpObj->Class == 189 || lpObj->Class == 197 || lpObj->Class == 267 || lpObj->Class == 275 || lpObj->Class == 338)
	{
		gObjMonsterBeattackRecv(send,lpObj->Index);
	}
	else
	{
		gSkillManager.CGMultiSkillAttackRecv((PMSG_MULTI_SKILL_ATTACK_RECV*)send,lpObj->Index,1);
	}
}

void gObjMonsterBeattackRecv(BYTE* aRecv,int aIndex) // OK
{
	PMSG_MULTI_SKILL_ATTACK_RECV* lpMsg = (PMSG_MULTI_SKILL_ATTACK_RECV*)aRecv;

	#if(GAMESERVER_UPDATE>=701)

	CSkill* lpSkill = &gObj[aIndex].Skill[MAKE_NUMBERW(lpMsg->skillH,lpMsg->skillL)];

	#elif(GAMESERVER_UPDATE>=401)

	CSkill* lpSkill = &gObj[aIndex].Skill[MAKE_NUMBERW(lpMsg->skill[0],lpMsg->skill[1])];

	#else

	CSkill* lpSkill = &gObj[aIndex].Skill[lpMsg->skill];

	#endif

	if(lpSkill->IsSkill() == 0)
	{
		return;
	}

	for(int n=0;n < lpMsg->count;n++)
	{
		PMSG_MULTI_SKILL_ATTACK* lpInfo = (PMSG_MULTI_SKILL_ATTACK*)(((BYTE*)lpMsg)+sizeof(PMSG_MULTI_SKILL_ATTACK_RECV)+(sizeof(PMSG_MULTI_SKILL_ATTACK)*n));

		#if(GAMESERVER_UPDATE>=701)

		int bIndex = MAKE_NUMBERW(lpInfo->indexH,lpInfo->indexL);

		#else

		int bIndex = MAKE_NUMBERW(lpInfo->index[0],lpInfo->index[1]);

		#endif

		if(OBJECT_RANGE(bIndex) == 0)
		{
			continue;
		}

		gAttack.Attack(&gObj[aIndex],&gObj[bIndex],lpSkill,1,1,0,0,0);
	}
}

void gObjMonsterAttack(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	if(lpObj->Class == 66 || lpObj->Class == 73 || lpObj->Class == 77)
	{
		PMSG_SKILL_ATTACK_RECV pMsg;

		pMsg.header.set(0x19,sizeof(pMsg));

		#if(GAMESERVER_UPDATE>=701)

		pMsg.skillH = SET_NUMBERHB(1);

		pMsg.skillL = SET_NUMBERLB(1);

		pMsg.indexH = SET_NUMBERHB(lpObj->TargetNumber);

		pMsg.indexL = SET_NUMBERLB(lpObj->TargetNumber);

		#else

		#if(GAMESERVER_UPDATE>=401)

		pMsg.skill[0] = SET_NUMBERHB(1);

		pMsg.skill[1] = SET_NUMBERLB(1);

		#else

		pMsg.skill = 1;

		#endif

		pMsg.index[0] = SET_NUMBERHB(lpObj->TargetNumber);

		pMsg.index[1] = SET_NUMBERLB(lpObj->TargetNumber);

		#endif

		pMsg.dis = 0;

		gSkillManager.CGSkillAttackRecv(&pMsg,lpObj->Index);
	}
	else if(lpObj->Class == 89 || lpObj->Class == 95 || lpObj->Class == 112 || lpObj->Class == 118 || lpObj->Class == 124 || lpObj->Class == 130 || lpObj->Class == 143 || lpObj->Class == 433)
	{
		PMSG_SKILL_ATTACK_RECV pMsg;

		pMsg.header.set(0x19,sizeof(pMsg));

		#if(GAMESERVER_UPDATE>=701)

		pMsg.skillH = SET_NUMBERHB(1);

		pMsg.skillL = SET_NUMBERLB(1);

		pMsg.indexH = SET_NUMBERHB(lpObj->TargetNumber);

		pMsg.indexL = SET_NUMBERLB(lpObj->TargetNumber);

		#else

		#if(GAMESERVER_UPDATE>=401)

		pMsg.skill[0] = SET_NUMBERHB(1);

		pMsg.skill[1] = SET_NUMBERLB(1);

		#else

		pMsg.skill = 1;

		#endif

		pMsg.index[0] = SET_NUMBERHB(lpObj->TargetNumber);

		pMsg.index[1] = SET_NUMBERLB(lpObj->TargetNumber);

		#endif

		pMsg.dis = 0;

		gSkillManager.CGSkillAttackRecv(&pMsg,lpObj->Index);
	}
	else if(lpObj->Class == 144 || lpObj->Class == 174 || lpObj->Class == 182 || lpObj->Class == 190 || lpObj->Class == 260 || lpObj->Class == 268)
	{
		if((GetLargeRand()%2) == 0)
		{
			PMSG_SKILL_ATTACK_RECV pMsg;

			pMsg.header.set(0x19,sizeof(pMsg));

			#if(GAMESERVER_UPDATE>=701)

			pMsg.skillH = SET_NUMBERHB(0);

			pMsg.skillL = SET_NUMBERLB(0);

			pMsg.indexH = SET_NUMBERHB(lpObj->TargetNumber);

			pMsg.indexL = SET_NUMBERLB(lpObj->TargetNumber);

			#else

			#if(GAMESERVER_UPDATE>=401)

			pMsg.skill[0] = SET_NUMBERHB(0);

			pMsg.skill[1] = SET_NUMBERLB(0);

			#else

			pMsg.skill = 0;

			#endif

			pMsg.index[0] = SET_NUMBERHB(lpObj->TargetNumber);

			pMsg.index[1] = SET_NUMBERLB(lpObj->TargetNumber);

			#endif

			pMsg.dis = 0;

			gSkillManager.CGSkillAttackRecv(&pMsg,lpObj->Index);
		}
		else
		{
			PMSG_ATTACK_RECV pMsg;

			pMsg.header.set(PROTOCOL_CODE2,sizeof(pMsg));

			pMsg.index[0] = SET_NUMBERHB(lpObj->TargetNumber);

			pMsg.index[1] = SET_NUMBERLB(lpObj->TargetNumber);

			pMsg.action = ACTION_ATTACK1;

			pMsg.dir = lpObj->Dir;

			gAttack.CGAttackRecv(&pMsg,lpObj->Index);
		}
	}
	else if(lpObj->Class == 145 || lpObj->Class == 146 || lpObj->Class == 147 || lpObj->Class == 148 || lpObj->Class == 160 || lpObj->Class == 175 || lpObj->Class == 176 || lpObj->Class == 177 || lpObj->Class == 178 || lpObj->Class == 180 || lpObj->Class == 183 || lpObj->Class == 184 || lpObj->Class == 185 || lpObj->Class == 186 || lpObj->Class == 188 || lpObj->Class == 191 || lpObj->Class == 192 || lpObj->Class == 193 || lpObj->Class == 194 || lpObj->Class == 196 || lpObj->Class == 261 || lpObj->Class == 262 || lpObj->Class == 263 || lpObj->Class == 264 || lpObj->Class == 266 || lpObj->Class == 269 || lpObj->Class == 270 || lpObj->Class == 271 || lpObj->Class == 272 || lpObj->Class == 274)
	{
		if((GetLargeRand()%2) == 0)
		{
			PMSG_SKILL_ATTACK_RECV pMsg;

			pMsg.header.set(0x19,sizeof(pMsg));

			#if(GAMESERVER_UPDATE>=701)

			pMsg.skillH = SET_NUMBERHB(0);

			pMsg.skillL = SET_NUMBERLB(0);

			pMsg.indexH = SET_NUMBERHB(lpObj->TargetNumber);

			pMsg.indexL = SET_NUMBERLB(lpObj->TargetNumber);

			#else

			#if(GAMESERVER_UPDATE>=401)

			pMsg.skill[0] = SET_NUMBERHB(0);

			pMsg.skill[1] = SET_NUMBERLB(0);

			#else

			pMsg.skill = 0;

			#endif

			pMsg.index[0] = SET_NUMBERHB(lpObj->TargetNumber);

			pMsg.index[1] = SET_NUMBERLB(lpObj->TargetNumber);

			#endif

			pMsg.dis = 0;

			gSkillManager.CGSkillAttackRecv(&pMsg,lpObj->Index);
		}
		else
		{
			PMSG_ATTACK_RECV pMsg;

			pMsg.header.set(PROTOCOL_CODE2,sizeof(pMsg));

			pMsg.index[0] = SET_NUMBERHB(lpObj->TargetNumber);

			pMsg.index[1] = SET_NUMBERLB(lpObj->TargetNumber);

			pMsg.action = ACTION_ATTACK1;

			pMsg.dir = lpObj->Dir;

			gAttack.CGAttackRecv(&pMsg,lpObj->Index);
		}
	}
	else if(lpObj->Class == 149 || lpObj->Class == 179 || lpObj->Class == 187 || lpObj->Class == 195 || lpObj->Class == 265 || lpObj->Class == 273)
	{
		PMSG_SKILL_ATTACK_RECV pMsg;

		pMsg.header.set(0x19,sizeof(pMsg));

		#if(GAMESERVER_UPDATE>=701)

		pMsg.skillH = SET_NUMBERHB(0);

		pMsg.skillL = SET_NUMBERLB((0+(GetLargeRand()%2)));

		pMsg.indexH = SET_NUMBERHB(lpObj->TargetNumber);

		pMsg.indexL = SET_NUMBERLB(lpObj->TargetNumber);

		#else

		#if(GAMESERVER_UPDATE>=401)

		pMsg.skill[0] = SET_NUMBERHB(0);

		pMsg.skill[1] = SET_NUMBERLB((0+(GetLargeRand()%2)));

		#else

		pMsg.skill = (0+(GetLargeRand()%2));

		#endif

		pMsg.index[0] = SET_NUMBERHB(lpObj->TargetNumber);

		pMsg.index[1] = SET_NUMBERLB(lpObj->TargetNumber);

		#endif

		pMsg.dis = 0;

		gSkillManager.CGSkillAttackRecv(&pMsg,lpObj->Index);
	}
	else if(lpObj->Class == 161 || lpObj->Class == 181 || lpObj->Class == 189 || lpObj->Class == 197 || lpObj->Class == 267 || lpObj->Class == 275 || lpObj->Class == 338)
	{
		PMSG_SKILL_ATTACK_RECV pMsg;

		pMsg.header.set(0x19,sizeof(pMsg));

		#if(GAMESERVER_UPDATE>=701)

		pMsg.skillH = SET_NUMBERHB(0);

		pMsg.skillL = SET_NUMBERLB((1+(GetLargeRand()%6)));

		pMsg.indexH = SET_NUMBERHB(lpObj->TargetNumber);

		pMsg.indexL = SET_NUMBERLB(lpObj->TargetNumber);

		#else

		#if(GAMESERVER_UPDATE>=401)

		pMsg.skill[0] = SET_NUMBERHB(0);

		pMsg.skill[1] = SET_NUMBERLB((1+(GetLargeRand()%6)));

		#else

		pMsg.skill = (1+(GetLargeRand()%6));

		#endif

		pMsg.index[0] = SET_NUMBERHB(lpObj->TargetNumber);

		pMsg.index[1] = SET_NUMBERLB(lpObj->TargetNumber);

		#endif

		pMsg.dis = 0;

		gSkillManager.CGSkillAttackRecv(&pMsg,lpObj->Index);

		gObjMonsterMagicAttack(lpObj,0);
	}
	else if(lpObj->Class == 163 || lpObj->Class == 165 || lpObj->Class == 167 || lpObj->Class == 169 || lpObj->Class == 171 || lpObj->Class == 173 || lpObj->Class == 427)
	{
		PMSG_SKILL_ATTACK_RECV pMsg;

		pMsg.header.set(0x19,sizeof(pMsg));

		#if(GAMESERVER_UPDATE>=701)

		pMsg.skillH = SET_NUMBERHB(0);

		pMsg.skillL = SET_NUMBERLB(0);

		pMsg.indexH = SET_NUMBERHB(lpObj->TargetNumber);

		pMsg.indexL = SET_NUMBERLB(lpObj->TargetNumber);

		#else

		#if(GAMESERVER_UPDATE>=401)

		pMsg.skill[0] = SET_NUMBERHB(0);

		pMsg.skill[1] = SET_NUMBERLB(0);

		#else

		pMsg.skill = 0;

		#endif

		pMsg.index[0] = SET_NUMBERHB(lpObj->TargetNumber);

		pMsg.index[1] = SET_NUMBERLB(lpObj->TargetNumber);

		#endif

		pMsg.dis = 0;

		gSkillManager.CGSkillAttackRecv(&pMsg,lpObj->Index);
	}
	else if(lpObj->Class == 529 || lpObj->Class == 530 || lpObj->Class == 533 || lpObj->Class == 534 || lpObj->Class == 535 || lpObj->Class == 536 || lpObj->Class == 537 || lpObj->Class == 538 || lpObj->Class == 539)
	{
		PMSG_DURATION_SKILL_ATTACK_RECV pMsg;

		pMsg.header.set(0x1E,sizeof(pMsg));

		#if(GAMESERVER_UPDATE>=701)

		pMsg.skillH = SET_NUMBERHB(0);

		pMsg.skillL = SET_NUMBERLB(0);

		#elif(GAMESERVER_UPDATE>=401)

		pMsg.skill[0] = SET_NUMBERHB(0);

		pMsg.skill[1] = SET_NUMBERLB(0);

		#else

		pMsg.skill = 0;

		#endif

		pMsg.x = (BYTE)gObj[lpObj->TargetNumber].X;

		pMsg.y = (BYTE)gObj[lpObj->TargetNumber].Y;

		pMsg.dir = (gSkillManager.GetSkillAngle(gObj[lpObj->TargetNumber].X,gObj[lpObj->TargetNumber].Y,lpObj->X,lpObj->Y)*255)/360;

		pMsg.dis = 0;

		pMsg.angle = (gSkillManager.GetSkillAngle(lpObj->X,lpObj->Y,gObj[lpObj->TargetNumber].X,gObj[lpObj->TargetNumber].Y)*255)/360;

		#if(GAMESERVER_UPDATE>=803)

		pMsg.indexH = SET_NUMBERHB(lpObj->TargetNumber);

		pMsg.indexL = SET_NUMBERLB(lpObj->TargetNumber);

		#else

		pMsg.index[0] = SET_NUMBERHB(lpObj->TargetNumber);

		pMsg.index[1] = SET_NUMBERLB(lpObj->TargetNumber);

		#endif

		pMsg.MagicKey = 0;

		gSkillManager.CGDurationSkillAttackRecv(&pMsg,lpObj->Index);
	}
	else if(lpObj->Class == 561)
	{
		PMSG_DURATION_SKILL_ATTACK_RECV pMsg;

		pMsg.header.set(0x1E,sizeof(pMsg));

		#if(GAMESERVER_UPDATE>=701)

		pMsg.skillH = SET_NUMBERHB(0);

		pMsg.skillL = SET_NUMBERLB((((GetLargeRand()%100)>=25)?1:0));

		#elif(GAMESERVER_UPDATE>=401)

		pMsg.skill[0] = SET_NUMBERHB(0);

		pMsg.skill[1] = SET_NUMBERLB((((GetLargeRand()%100)>=25)?1:0));

		#else

		pMsg.skill = (((GetLargeRand()%100)>=25)?1:0);

		#endif

		pMsg.x = (BYTE)gObj[lpObj->TargetNumber].X;

		pMsg.y = (BYTE)gObj[lpObj->TargetNumber].Y;

		pMsg.dir = (gSkillManager.GetSkillAngle(gObj[lpObj->TargetNumber].X,gObj[lpObj->TargetNumber].Y,lpObj->X,lpObj->Y)*255)/360;

		pMsg.dis = 0;

		pMsg.angle = (gSkillManager.GetSkillAngle(lpObj->X,lpObj->Y,gObj[lpObj->TargetNumber].X,gObj[lpObj->TargetNumber].Y)*255)/360;

		#if(GAMESERVER_UPDATE>=803)

		pMsg.indexH = SET_NUMBERHB(lpObj->TargetNumber);

		pMsg.indexL = SET_NUMBERLB(lpObj->TargetNumber);

		#else

		pMsg.index[0] = SET_NUMBERHB(lpObj->TargetNumber);

		pMsg.index[1] = SET_NUMBERLB(lpObj->TargetNumber);

		#endif

		pMsg.MagicKey = 0;

		gSkillManager.CGDurationSkillAttackRecv(&pMsg,lpObj->Index);
	}
	else
	{
		if(lpObj->AttackType == 0 || lpObj->AttackType >= 100)
		{
			PMSG_ATTACK_RECV pMsg;

			pMsg.header.set(PROTOCOL_CODE2,sizeof(pMsg));

			pMsg.index[0] = SET_NUMBERHB(lpObj->TargetNumber);

			pMsg.index[1] = SET_NUMBERLB(lpObj->TargetNumber);

			pMsg.action = ACTION_ATTACK1;

			pMsg.dir = lpObj->Dir;

			gAttack.CGAttackRecv(&pMsg,lpObj->Index);
		}
		else
		{
			PMSG_SKILL_ATTACK_RECV pMsg;

			pMsg.header.set(0x19,sizeof(pMsg));

			#if(GAMESERVER_UPDATE>=701)

			pMsg.skillH = SET_NUMBERHB(0);

			pMsg.skillL = SET_NUMBERLB(0);

			pMsg.indexH = SET_NUMBERHB(lpObj->TargetNumber);

			pMsg.indexL = SET_NUMBERLB(lpObj->TargetNumber);

			#else

			#if(GAMESERVER_UPDATE>=401)

			pMsg.skill[0] = SET_NUMBERHB(0);

			pMsg.skill[1] = SET_NUMBERLB(0);

			#else

			pMsg.skill = 0;

			#endif

			pMsg.index[0] = SET_NUMBERHB(lpObj->TargetNumber);

			pMsg.index[1] = SET_NUMBERLB(lpObj->TargetNumber);

			#endif

			pMsg.dis = 0;

			gSkillManager.CGSkillAttackRecv(&pMsg,lpObj->Index);
		}
	}
}

void gObjMonsterDie(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	if(lpObj->CsNpcType != 0)
	{
		switch(lpObj->CsNpcType)
		{
			case 1:
				gCastleSiege.DelNPC(lpObj->Index,lpObj->Class,lpObj->CsNpcExistVal,1);
				break;
			case 2:
				gCastleSiege.DelNPC(lpObj->Index,lpObj->Class,lpObj->CsNpcExistVal,0);
				break;
			case 3:
				gCastleSiege.DelNPC(lpObj->Index,lpObj->Class,lpObj->CsNpcExistVal,0);
				break;
		}

		if(lpObj->Class == 278)
		{
			gLifeStone.DeleteLifeStone(lpObj->Index);
		}

		if(lpObj->Class == 286 || lpObj->Class == 287)
		{
			gMercenary.DeleteMercenary(lpObj->Index);
		}

		gObjDel(lpObj->Index);
	}

	if(KALIMA_MAP_RANGE(lpObj->Map) != 0)
	{
		if(lpObj->Class == 161 || lpObj->Class == 181 || lpObj->Class == 189 || lpObj->Class == 197 || lpObj->Class == 267 || lpObj->Class == 338)
		{
			gKalima.CreateKalimaNextPortal(lpTarget,lpObj);
		}
	}

	gObjMonsterDieGiveItem(lpObj,lpTarget);

	lpObj->NextActionTime = 500;

	if(OBJECT_RANGE(lpObj->SummonIndex) != 0)
	{
		gObjSummonKill(lpObj->SummonIndex);
	}
}

void gObjMonsterStateProc(LPOBJ lpObj,int code,int aIndex,int SubCode) // OK
{
	switch(code)
	{
		case 0:
			gObjMonsterProcessHit(lpObj,aIndex);
			break;
		case 1:
			gObjMonsterDie(lpObj,&gObj[aIndex]);
			break;
		case 2:
			if(gObj[aIndex].Live != 0)
			{
				gObjBackSpring(lpObj,&gObj[aIndex]);
			}
			break;
		case 3:
			lpObj->TargetNumber = -1;
			lpObj->LastAttackerID = -1;
			lpObj->NextActionTime = 1000;
			lpObj->ActionState.Attack = 0;
			lpObj->ActionState.Move = 0;
			lpObj->ActionState.Emotion = EMOTION_REST;
			break;
		case 4:
			lpObj->ActionState.Emotion = EMOTION_ESCAPE;
			lpObj->ActionState.EmotionCount = 1;
			break;
		case 6:
			if(gObj[aIndex].Live != 0)
			{
				gObjBackSpring2(lpObj,&gObj[aIndex],2);
			}
			break;
		case 7:
			if(gObj[aIndex].Live != 0)
			{
				gObjBackSpring2(lpObj,&gObj[aIndex],3);
			}
			break;
		case 55:
			if(gObj[aIndex].Live != 0)
			{
				gAttack.Attack(lpObj,&gObj[aIndex],0,0,0,0,0,0);
			}
			break;
		case 56:
			if(gObjCheckResistance(&gObj[aIndex],1) == 0)
			{
				gEffectManager.AddEffect(&gObj[aIndex],0,EFFECT_POISON,SubCode,lpObj->Index,2,3,0);
			}
			break;
		case 57:
			if(gObj[aIndex].Live != 0)
			{
				gObjBackSpring2(&gObj[aIndex],lpObj,SubCode);
			}
			break;
	}
}

void gObjTrapFindTarget(LPOBJ lpObj) // OK
{
	int IndexTable[MAX_VIEWPORT];
	int IndexCount = 0;

	for(int n=0;n < MAX_VIEWPORT;n++)
	{
		if(lpObj->VpPlayer2[n].state == VIEWPORT_NONE || OBJECT_RANGE(lpObj->VpPlayer2[n].index) == 0 || lpObj->VpPlayer2[n].type != OBJECT_USER)
		{
			continue;
		}

		if((gObj[lpObj->VpPlayer2[n].index].Authority & 32) != 0 || gEffectManager.CheckEffect(&gObj[lpObj->VpPlayer2[n].index],EFFECT_INVISIBILITY) != 0)
		{
			continue;
		}

		if(gSkillManager.CheckSkillTarget(lpObj,lpObj->VpPlayer2[n].index,-1,lpObj->VpPlayer2[n].type) == 0)
		{
			continue;
		}

		if(gViewport.CheckViewportObjectPosition(lpObj->Index,gObj[lpObj->VpPlayer2[n].index].Map,gObj[lpObj->VpPlayer2[n].index].X,gObj[lpObj->VpPlayer2[n].index].Y,lpObj->AttackRange) == 0)
		{
			continue;
		}

		IndexTable[IndexCount++] = lpObj->VpPlayer2[n].index;

		if(lpObj->AttackRange == 0)
		{
			break;
		}
	}

	lpObj->TargetNumber = ((IndexCount==0)?-1:IndexTable[GetLargeRand()%IndexCount]);

	if(OBJECT_RANGE(lpObj->TargetNumber) != 0)
	{
		lpObj->ActionState.Attack = 1;
		lpObj->NextActionTime = lpObj->PhysiSpeed;
	}
	else
	{
		lpObj->NextActionTime = lpObj->MoveSpeed;
	}
}

void gObjMonsterUpdateProc(LPOBJ lpObj) // OK
{
	if(lpObj->Live == 0 || lpObj->State != OBJECT_PLAYING)
	{
		return;
	}

	if((GetTickCount()-lpObj->CurActionTime) < (lpObj->NextActionTime+lpObj->DelayActionTime))
	{
		return;
	}

	lpObj->CurActionTime = GetTickCount();

	if(lpObj->Attribute == ATTRIBUTE_KALIMA || lpObj->Class == 131 || lpObj->Class == 132 || lpObj->Class == 133 || lpObj->Class == 134 || lpObj->Class == 278 || lpObj->Class == 283 || lpObj->Class == 288)
	{
		return;
	}

	if((lpObj->Class >= 100 && lpObj->Class <= 110) || lpObj->Class == 523) // Trap
	{
		gObjTrapFindTarget(lpObj);
	}
	else if(lpObj->Class == 200) // Soccer Ball
	{
		gBattleSoccer.MonsterProc(lpObj);
		return;
	}
	else if(lpObj->Class == 286 || lpObj->Class == 287) //Archer, Spearman
	{
		gMercenary.FindTarget(lpObj->Index);
	}
	else
	{
		gObjMonsterReactionProc(lpObj);
	}

	if(lpObj->Attribute == ATTRIBUTE_SUMMONED && OBJECT_RANGE(lpObj->SummonIndex) != 0)
	{
		LPOBJ lpSummon = &gObj[lpObj->SummonIndex];

		if(lpObj->Map != lpSummon->Map || gObjCalcDistance(lpObj,lpSummon) >= lpObj->MoveRange)
		{
			gObjTeleportMagicUse(lpObj->Index,lpSummon->X+1,lpSummon->Y);
			lpObj->Map = lpSummon->Map;
			return;
		}
	}

	if(lpObj->ActionState.Move == 1)
	{
		gObjMonsterFindPathToMove(lpObj);
		lpObj->ActionState.Move = 0 ;
		return;		
	}

	if(lpObj->ActionState.Attack == 1)
	{
		if(lpObj->Class == 459) // Selupan
		{
			return;
		}

		if(gMonsterSkillManager.CheckMonsterSkill(lpObj->Class,0) == 0)
		{
			if(OBJECT_RANGE(lpObj->TargetNumber) == 0 || gObj[lpObj->TargetNumber].Connected != OBJECT_ONLINE || gObj[lpObj->TargetNumber].CloseCount > 0 || gObj[lpObj->TargetNumber].Live == 0 || gObj[lpObj->TargetNumber].Teleport != TELEPORT_NONE)
			{
				lpObj->TargetNumber = -1;
				lpObj->LastAttackerID = -1;
				lpObj->NextActionTime = 1000;
				lpObj->ActionState.Attack = 0;
				lpObj->ActionState.Move = 0;
				lpObj->ActionState.Emotion = EMOTION_REST;
				return;
			}

			lpObj->Dir = GetPathPacketDirPos((gObj[lpObj->TargetNumber].X-lpObj->X),(gObj[lpObj->TargetNumber].Y-lpObj->Y));

			if(lpObj->AttackType == 50 || (lpObj->AttackType >= 100 && (GetLargeRand()%5) == 0))
			{
				gObjMonsterMagicAttack(lpObj,0);
			}
			else
			{
				gObjMonsterAttack(lpObj,&gObj[lpObj->TargetNumber]);
			}

			lpObj->ActionState.Attack = 0;
		}
		else
		{
			if(OBJECT_RANGE(lpObj->TargetNumber) == 0 || gObj[lpObj->TargetNumber].Connected != OBJECT_ONLINE || gObj[lpObj->TargetNumber].CloseCount > 0 || gObj[lpObj->TargetNumber].Live == 0 || gObj[lpObj->TargetNumber].Teleport != TELEPORT_NONE)
			{
				lpObj->TargetNumber = -1;
				lpObj->LastAttackerID = -1;
				lpObj->NextActionTime = 1000;
				lpObj->ActionState.Attack = 0;
				lpObj->ActionState.Move = 0;
				lpObj->ActionState.Emotion = EMOTION_REST;
				return;
			}

			if((GetLargeRand()%4) == 0)
			{
				lpObj->Dir = GetPathPacketDirPos((gObj[lpObj->TargetNumber].X-lpObj->X),(gObj[lpObj->TargetNumber].Y-lpObj->Y));

				PMSG_ATTACK_RECV pMsg;

				pMsg.header.set(PROTOCOL_CODE2,sizeof(pMsg));

				pMsg.index[0] = SET_NUMBERHB(lpObj->TargetNumber);

				pMsg.index[1] = SET_NUMBERLB(lpObj->TargetNumber);

				pMsg.action = ACTION_ATTACK1;

				pMsg.dir = lpObj->Dir;

				gAttack.CGAttackRecv(&pMsg,lpObj->Index);

				lpObj->ActionState.Attack = 0;
			}
			else
			{
				lpObj->Dir = GetPathPacketDirPos((gObj[lpObj->TargetNumber].X-lpObj->X),(gObj[lpObj->TargetNumber].Y-lpObj->Y));

				gMonsterSkillManager.UseMonsterSkill(lpObj->Index,lpObj->TargetNumber,0,-1,0);

				lpObj->ActionState.Attack = 0;
			}
		}
	}
}

void gObjMonsterReactionProc(LPOBJ lpObj) // OK
{
	if(OBJECT_RANGE(lpObj->TargetNumber) == 0)
	{
		lpObj->ActionState.Emotion = EMOTION_REST;
	}

	if(gEffectManager.CheckEffect(lpObj,EFFECT_SLEEP) != 0)
	{
		return;
	}

	if(lpObj->ActionState.Emotion == EMOTION_REST)
	{
		if(lpObj->Attribute != ATTRIBUTE_NONE)
		{
			if(lpObj->ActionState.Attack == 1)
			{
				lpObj->ActionState.Attack = 0;
				lpObj->TargetNumber = -1;
				lpObj->NextActionTime = 500;
			}
			
			int action_code = lpObj->Attribute == ATTRIBUTE_SUMMONED ? 1 : (GetLargeRand()%2);

			if(action_code == 0)
			{
				lpObj->NextActionTime = 500;
			}
			else if(lpObj->MoveRange > 0 && gMonsterAIUtil.CheckMovingCondition(lpObj) != 0)
			{
				if(lpObj->Attribute != ATTRIBUTE_SUMMONED)
				{
					gObjMonsterGetXYToPatrol(lpObj);
				}
				else
				{
					gObjMonsterGetXYToOwner(lpObj);
				}
			}
			
			gObjMonsterFindTarget(lpObj);

			if(OBJECT_RANGE(lpObj->TargetNumber) != 0)
			{
				lpObj->ActionState.EmotionCount = 30;
				lpObj->ActionState.Emotion = EMOTION_ATTACK;
			}
		}
	}
	else if(lpObj->ActionState.Emotion == EMOTION_ATTACK)
	{
		lpObj->ActionState.Update();

		if(OBJECT_RANGE(lpObj->TargetNumber) != 0 && lpObj->PathStartEnd == 0)
		{
			if(gObjCalcDistance(lpObj,&gObj[lpObj->TargetNumber]) <= (lpObj->AttackType>=100?(lpObj->AttackRange+2):lpObj->AttackRange))
			{
				if(gMap[lpObj->Map].CheckWall(lpObj->X,lpObj->Y,gObj[lpObj->TargetNumber].X,gObj[lpObj->TargetNumber].Y) != 0)
				{
					if(gMap[lpObj->Map].CheckAttr(gObj[lpObj->TargetNumber].X,gObj[lpObj->TargetNumber].Y,1) == 0)
					{
						lpObj->ActionState.Attack = 1;
					}
					else
					{
						lpObj->TargetNumber = -1;
						lpObj->ActionState.EmotionCount = 30;
						lpObj->ActionState.Emotion = EMOTION_ATTACK;
					}

					lpObj->Dir = GetPathPacketDirPos(gObj[lpObj->TargetNumber].X-lpObj->X,gObj[lpObj->TargetNumber].Y-lpObj->Y);
					lpObj->NextActionTime = lpObj->PhysiSpeed;
				}
				else
				{
					lpObj->TargetNumber = -1;
					lpObj->ActionState.Attack = 0;
					lpObj->NextActionTime = 500;
					lpObj->ActionState.Emotion = EMOTION_REST;
					lpObj->ActionState.Move = 1;
				}
			}
			else
			{
				if(gObjMonsterGetXYToChase(lpObj) != 0)
				{
					if(gMap[lpObj->Map].CheckWall(lpObj->X,lpObj->Y,lpObj->MTX,lpObj->MTY) != 0)
					{
						lpObj->ActionState.Move = 1;
						lpObj->NextActionTime = 400;
						lpObj->Dir = GetPathPacketDirPos(gObj[lpObj->TargetNumber].X-lpObj->X,gObj[lpObj->TargetNumber].Y-lpObj->Y);
					}
					else
					{
						gObjMonsterGetXYToPatrol(lpObj);
						lpObj->ActionState.Emotion = EMOTION_ESCAPE;
						lpObj->ActionState.EmotionCount = 10;
					}
				}
				else
				{
					gObjMonsterGetXYToPatrol(lpObj);
				}
			}
		}
	}
	else if(lpObj->ActionState.Emotion == EMOTION_MOVE)
	{
		lpObj->ActionState.Update();
		lpObj->ActionState.Move = 1;
		lpObj->NextActionTime = 800;

		if(OBJECT_RANGE(lpObj->TargetNumber) != 0)
		{
			int tdir = GetPathPacketDirPos(gObj[lpObj->TargetNumber].X-lpObj->X,gObj[lpObj->TargetNumber].Y-lpObj->Y)*2;
			lpObj->MTX += RoadPathTable[tdir]*(-3);
			lpObj->MTY += RoadPathTable[tdir+1]*(-3);
		}
	}
	else if(lpObj->ActionState.Emotion == EMOTION_ESCAPE)
	{
		lpObj->ActionState.Update();
		lpObj->ActionState.Move = 0;
		lpObj->ActionState.Attack = 0;
		lpObj->NextActionTime = 400;
	}
}

void gObjMonsterGetXYToPatrol(LPOBJ lpObj) // OK
{
	if(gMonsterAIUtil.CheckMovingCondition(lpObj) == 0)
	{
		return;
	}

	lpObj->NextActionTime = 1000;

	for(int n=0;n < 10;n++)
	{
		int tx = (lpObj->X-lpObj->MoveRange)+(GetLargeRand()%((lpObj->MoveRange*2)+1));
		int ty = (lpObj->Y-lpObj->MoveRange)+(GetLargeRand()%((lpObj->MoveRange*2)+1));

		if(gObjMonsterMoveCheck(lpObj,tx,ty) != 0)
		{
			if(lpObj->Class == 247 || lpObj->Class == 249)
			{
				if(gMap[lpObj->Map].CheckAttr(tx,ty,2) == 0 && gMap[lpObj->Map].CheckAttr(tx,ty,4) == 0 && gMap[lpObj->Map].CheckAttr(tx,ty,8) == 0)
				{
					lpObj->MTX = tx;
					lpObj->MTY = ty;
					lpObj->ActionState.Move = 1;
					lpObj->NextActionTime = 3000;
					return;
				}
			}
			else
			{
				if(gMap[lpObj->Map].CheckAttr(tx,ty,1) == 0 && gMap[lpObj->Map].CheckAttr(tx,ty,2) == 0 && gMap[lpObj->Map].CheckAttr(tx,ty,4) == 0 && gMap[lpObj->Map].CheckAttr(tx,ty,8) == 0)
				{
					lpObj->ActionState.Emotion = EMOTION_REST;
					lpObj->MTX = tx;
					lpObj->MTY = ty;
					lpObj->ActionState.Move = 1;
					lpObj->NextActionTime = 500;
					lpObj->TargetNumber = -1;
					lpObj->ActionState.Attack = 0;
					return;
				}
			}
		}
	}
}

bool gObjMonsterGetXYToChase(LPOBJ lpObj) // OK
{
	if(gMonsterAIUtil.CheckMovingCondition(lpObj) == 0)
	{
		return 0;
	}

	if(gMonsterAIUtil.FindMonViewportObj2(lpObj->Index,lpObj->TargetNumber) == 0)
	{
		return 0;
	}

	if(OBJECT_RANGE(lpObj->TargetNumber) == 0 || gObj[lpObj->TargetNumber].Teleport != TELEPORT_NONE)
	{
		return 0;
	}

	int sx = gObj[lpObj->TargetNumber].X;
	int sy = gObj[lpObj->TargetNumber].Y;
	int tx = gObj[lpObj->TargetNumber].X;
	int ty = gObj[lpObj->TargetNumber].Y;
	int dis = (lpObj->AttackType>=100?(lpObj->AttackRange+2):lpObj->AttackRange);

	if(lpObj->X < tx)
	{
		sx -= dis;
	}

	if(lpObj->X > tx)
	{
		sx += dis;
	}

	if(lpObj->Y < ty)
	{
		sy -= dis;
	}

	if(lpObj->Y > ty)
	{
		sy += dis;
	}

	int PathValue = GetPathPacketDirPos((gObj[lpObj->TargetNumber].X-sx),(gObj[lpObj->TargetNumber].Y-sy))*2;

	if(gMap[lpObj->Map].CheckStandAttr(sx,sy) == 0)
	{
		for(int n=0;n < (MAX_ROAD_PATH_TABLE/2);n++)
		{
			tx = gObj[lpObj->TargetNumber].X+RoadPathTable[PathValue++];
			ty = gObj[lpObj->TargetNumber].Y+RoadPathTable[PathValue++];

			if(gObjMonsterMoveCheck(lpObj,tx,ty) != 0)
			{
				if(gMap[lpObj->Map].CheckAttr(tx,ty,1) == 0 && gMap[lpObj->Map].CheckAttr(tx,ty,2) == 0 && gMap[lpObj->Map].CheckAttr(tx,ty,4) == 0 && gMap[lpObj->Map].CheckAttr(tx,ty,8) == 0)
				{
					lpObj->MTX = tx;
					lpObj->MTY = ty;
					return 1;
				}
			}

			PathValue = ((PathValue>=MAX_ROAD_PATH_TABLE)?0:PathValue);
		}
	}
	else
	{
		if(gObjMonsterMoveCheck(lpObj,sx,sy) != 0)
		{
			if(gMap[lpObj->Map].CheckAttr(sx,sy,1) == 0 && gMap[lpObj->Map].CheckAttr(sx,sy,2) == 0 && gMap[lpObj->Map].CheckAttr(sx,sy,4) == 0 && gMap[lpObj->Map].CheckAttr(sx,sy,8) == 0)
			{
				lpObj->MTX = sx;
				lpObj->MTY = sy;
				return 1;
			}
		}
	}

	return 0;
}

void gObjMonsterGetXYToOwner(LPOBJ lpObj) // OK
{
	if(OBJECT_RANGE(lpObj->SummonIndex) == 0)
	{
		return;
	}

	if(gObj[lpObj->SummonIndex].Rest != 0)
	{
		return;
	}

	int sx = gObj[lpObj->SummonIndex].X;
	int sy = gObj[lpObj->SummonIndex].Y;
	int tx = gObj[lpObj->SummonIndex].X;
	int ty = gObj[lpObj->SummonIndex].Y;
	int dis = (lpObj->AttackType>=100?(lpObj->AttackRange+2):lpObj->AttackRange);

	if(lpObj->X < tx)
	{
		sx -= dis;
	}

	if(lpObj->X > tx)
	{
		sx += dis;
	}

	if(lpObj->Y < ty)
	{
		sy -= dis;
	}

	if(lpObj->Y > ty)
	{
		sy += dis;
	}

	int PathValue = GetPathPacketDirPos((gObj[lpObj->SummonIndex].X-sx),(gObj[lpObj->SummonIndex].Y-sy))*2;

	if(gMap[lpObj->Map].CheckStandAttr(sx,sy) == 0)
	{
		for(int n=0;n < (MAX_ROAD_PATH_TABLE/2);n++)
		{
			tx = gObj[lpObj->SummonIndex].X+RoadPathTable[PathValue++];
			ty = gObj[lpObj->SummonIndex].Y+RoadPathTable[PathValue++];

			if(gMap[lpObj->Map].CheckAttr(tx,ty,2) == 0 && gMap[lpObj->Map].CheckAttr(tx,ty,4) == 0 && gMap[lpObj->Map].CheckAttr(tx,ty,8) == 0)
			{
				lpObj->MTX = tx;
				lpObj->MTY = ty;
				lpObj->ActionState.Move = 1;
				lpObj->NextActionTime = 1000;
				return;
			}

			PathValue = ((PathValue>=MAX_ROAD_PATH_TABLE)?0:PathValue);
		}
	}
	else
	{
		if(gMap[lpObj->Map].CheckAttr(sx,sy,2) == 0 && gMap[lpObj->Map].CheckAttr(sx,sy,4) == 0 && gMap[lpObj->Map].CheckAttr(sx,sy,8) == 0)
		{
			lpObj->MTX = sx;
			lpObj->MTY = sy;
			lpObj->ActionState.Move = 1;
			lpObj->NextActionTime = 1000;
			return ;
		}
	}
}

void gObjMonsterFindTarget(LPOBJ lpObj) // OK
{
	int IndexTable[MAX_VIEWPORT];
	int IndexCount = 0;

	for(int n=0;n < MAX_VIEWPORT;n++)
	{
		if(lpObj->VpPlayer2[n].state == VIEWPORT_NONE || OBJECT_RANGE(lpObj->VpPlayer2[n].index) == 0)
		{
			continue;
		}

		if((gObj[lpObj->VpPlayer2[n].index].Authority & 32) != 0 || gEffectManager.CheckEffect(&gObj[lpObj->VpPlayer2[n].index],EFFECT_INVISIBILITY) != 0)
		{
			continue;
		}

		if(gViewport.CheckViewportObjectPosition(lpObj->Index,gObj[lpObj->VpPlayer2[n].index].Map,gObj[lpObj->VpPlayer2[n].index].X,gObj[lpObj->VpPlayer2[n].index].Y,lpObj->ViewRange) == 0)
		{
			continue;
		}

		if(lpObj->Attribute == ATTRIBUTE_SUMMONED && lpObj->VpPlayer2[n].type == OBJECT_USER)
		{
			continue;
		}

		if((lpObj->Class == 247 || lpObj->Class == 249) && gObj[lpObj->VpPlayer2[n].index].PKLevel > 4)
		{
			IndexTable[IndexCount++] = lpObj->VpPlayer2[n].index;
			continue;
		}

		if(gSkillManager.CheckSkillTarget(lpObj,lpObj->VpPlayer2[n].index,-1,lpObj->VpPlayer2[n].type) == 0)
		{
			continue;
		}

		IndexTable[IndexCount++] = lpObj->VpPlayer2[n].index;
	}

	lpObj->TargetNumber = ((IndexCount==0)?-1:IndexTable[GetLargeRand()%IndexCount]);
}

void gObjMonsterFindPathToMove(LPOBJ lpObj) // OK
{
	if(gEffectManager.CheckStunEffect(lpObj) != 0 || gEffectManager.CheckImmobilizeEffect(lpObj) != 0)
	{
		return;
	}

	PATH_INFO path;

	BYTE bPath[8];

	if(gMap[lpObj->Map].PathFinding2(lpObj->X,lpObj->Y,lpObj->MTX,lpObj->MTY,&path) != 0)
	{
		short sx = lpObj->X;
		short sy = lpObj->Y;
		BYTE pos = 0;

		memset(bPath,0,sizeof(bPath));
		bPath[0] = (lpObj->Dir*16)+((path.PathNum-1)&0x0F);
		
		for(int n=1;n < path.PathNum;++n)
		{
			pos = GetPathPacketDirPos(path.PathX[n]-sx,path.PathY[n]-sy);
			sx = path.PathX[n];
			sy = path.PathY[n];

			if((n%2) == 1)
			{
				bPath[(n+1)/2] = (pos*16);
			}
			else
			{
				bPath[(n+1)/2] |= pos;
			}
		}

		bPath[0] &= 0x0F;
		bPath[0] |= pos << 4;

		PMSG_MOVE_RECV pMsg;

		pMsg.header.set(PROTOCOL_CODE1,sizeof(pMsg));

		pMsg.x = (BYTE)lpObj->X;

		pMsg.y = (BYTE)lpObj->Y;

		memcpy(pMsg.path,bPath,sizeof(pMsg.path));

		CGMoveRecv((PMSG_MOVE_RECV*)&pMsg,lpObj->Index);
	}
}

void gObjMonsterProcessHit(LPOBJ lpObj,int aIndex) // OK
{
	if(lpObj->Attribute == ATTRIBUTE_NONE)
	{
		return;
	}

	if(OBJECT_RANGE(aIndex) == 0 || gObj[aIndex].Live == 0 || gObj[aIndex].State != OBJECT_PLAYING)
	{
		return;
	}

	if(lpObj->TargetNumber == aIndex)
	{
		return;
	}

	if(gMap[lpObj->Map].CheckWall(lpObj->X,lpObj->Y,gObj[aIndex].X,gObj[aIndex].Y) == 0)
	{
		return;
	}

	if(gMap[gObj[aIndex].Map].CheckAttr(gObj[aIndex].X,gObj[aIndex].Y,1) != 0)
	{
		return;
	}

	if(lpObj->ActionState.Emotion == EMOTION_REST)
	{
		lpObj->ActionState.Emotion = EMOTION_ATTACK;
		lpObj->ActionState.EmotionCount = 10;
	}
	else if(lpObj->ActionState.Emotion == EMOTION_ATTACK)
	{
		lpObj->ActionState.EmotionCount = 10;
	}

	if(lpObj->ActionState.Attack == 0)
	{
		if(gObjCalcDistance(lpObj,&gObj[aIndex]) <= (lpObj->AttackType>=100?(lpObj->AttackRange+2):lpObj->AttackRange))
		{
			if(gObj[aIndex].Attribute == ATTRIBUTE_SUMMONED || (GetLargeRand()%100) < 90)
			{
				lpObj->TargetNumber = aIndex;
			}
		}
		else
		{
			if(OBJECT_RANGE(lpObj->TargetNumber) == 0)
			{
				lpObj->TargetNumber = aIndex;
			}
		}
	}
	else
	{
		if((GetLargeRand()%2) == 1)
		{
			lpObj->ActionState.Attack = 1;
			lpObj->TargetNumber = aIndex;
		}
	}
}
