#pragma once

#include "ComboSkill.h"
#include "Effect.h"
#include "Item.h"
#include "MonsterAIAgro.h"
#include "MonsterSkillElementOption.h"
#include "Skill.h"

#define MAX_OBJECT 12000
#define MAX_OBJECT_MONSTER 10000
#if(PROTECT_STATE==1)
#define MAX_OBJECT_USER 1000
#else
#define MAX_OBJECT_USER 100
#endif

#define OBJECT_START_MONSTER 0
#define OBJECT_START_USER (MAX_OBJECT-MAX_OBJECT_USER)

#define MAX_SELF_DEFENSE 5
#define MAX_HIT_DAMAGE 40
#define MAX_VIEWPORT 75
#define MAX_MONEY 2000000000
#define MAX_CHARACTER_LEVEL 1000
#define MAX_ACCOUNT_LEVEL 4
#define MAX_DAMAGE_REDUCTION 6
#define MAX_MESSAGE_SIZE 255
#define MAX_CHAT_MESSAGE_SIZE 59
#define MAX_ROAD_PATH_TABLE 16
#define MAX_MONSTER_SEND_MSG 20
#define MAX_MONSTER_SEND_ATTACK_MSG 30

#define OBJECT_RANGE(x) (((x)<0)?0:((x)>=MAX_OBJECT)?0:1)
#define OBJECT_MONSTER_RANGE(x) (((x)<OBJECT_START_MONSTER)?0:((x)>=MAX_OBJECT_MONSTER)?0:1)
#define OBJECT_SUMMON_RANGE(x) (((x)<MAX_OBJECT_MONSTER)?0:((x)>=OBJECT_START_USER)?0:1)
#define OBJECT_USER_RANGE(x) (((x)<OBJECT_START_USER)?0:((x)>=MAX_OBJECT)?0:1)
#define GET_MAX_RESISTANCE(x,y,z) (((((x)>(y))?(x):(y))>(z))?(((x)>(y))?(x):(y)):(z))
#define GET_MAX_WORD_VALUE(x) (((x)>65000)?65000:((WORD)(x)))
#define CHECK_RANGE(x,y) (((x)<0)?0:((x)>=(y))?0:1)

static const int gGameServerMaxAccount[5] = { 20,200,500,MAX_OBJECT_USER,MAX_OBJECT_USER };

enum eObjectConnectState
{
	OBJECT_OFFLINE = 0,
	OBJECT_CONNECTED = 1,
	OBJECT_LOGGED = 2,
	OBJECT_ONLINE = 3,
};

enum eObjectType
{
	OBJECT_NONE = 0,
	OBJECT_USER = 1,
	OBJECT_MONSTER = 2,
	OBJECT_NPC = 3,
	OBJECT_ITEM = 5,
};

enum eObjectState
{
	OBJECT_EMPTY = 0,
	OBJECT_CREATE = 1,
	OBJECT_PLAYING = 2,
	OBJECT_DYING = 4,
	OBJECT_DIECMD = 8,
	OBJECT_DIED = 16,
	OBJECT_DELCMD = 32,
};

enum eInterfaceType
{
	INTERFACE_NONE = 0,
	INTERFACE_TRADE = 1,
	INTERFACE_PARTY = 2,
	INTERFACE_SHOP = 3,
	INTERFACE_GUILD = 4,
	INTERFACE_GUILD_CREATE = 5,
	INTERFACE_WAREHOUSE = 6,
	INTERFACE_CHAOS_BOX = 7,
	INTERFACE_PERSONAL_SHOP = 8,
	INTERFACE_GOLDEN_ARCHER = 9,
	INTERFACE_QUEST = 10,
	INTERFACE_TRANSFER = 11,
	INTERFACE_COMMON = 12,
	INTERFACE_TRAINER = 13,
	INTERFACE_GUILD_REQUEST = 14,
	INTERFACE_CASTLE_SIEGE_WEAPON_OFFENSE = 15,
	INTERFACE_CASTLE_SIEGE_WEAPON_DEFENSE = 16,
	INTERFACE_QUEST_WORLD = 17,
	INTERFACE_CASH_SHOP = 18,
	INTERFACE_MINING = 19,
	INTERFACE_PC_SHOP = 20,
	INTERFACE_CHAOS_CASTLE = 21,
};

enum eActionType
{
	ACTION_ATTACK1 = 120,
	ACTION_ATTACK2 = 121,
	ACTION_STAND1 = 122,
	ACTION_STAND2 = 123,
	ACTION_MOVE1 = 124,
	ACTION_MOVE2 = 125,
	ACTION_DAMAGE1 = 126,
	ACTION_DIE1 = 127,
	ACTION_SIT1 = 128,
	ACTION_POSE1 = 129,
	ACTION_HEALING1 = 130,
	ACTION_GREETING1 = 131,
	ACTION_GOODBYE1 = 132,
	ACTION_CLAP1 = 133,
	ACTION_GESTURE1 = 134,
	ACTION_DIRECTION1 = 135,
	ACTION_UNKNOWN1 = 136,
	ACTION_CRY1 = 137,
	ACTION_CHEER1 = 138,
	ACTION_AWKWARD1 = 139,
	ACTION_SEE1 = 140,
	ACTION_WIN1 = 141,
	ACTION_SMILE1 = 142,
	ACTION_SLEEP1 = 143,
	ACTION_COLD1 = 144,
	ACTION_AGAIN1 = 145,
	ACTION_RESPECT1 = 146,
	ACTION_SALUTE1 = 147,
	ACTION_RUSH1 = 148,
	ACTION_SCISSORS = 149,
	ACTION_ROCK = 150,
	ACTION_PAPER = 151,
	ACTION_HUSTLE = 152,
	ACTION_PROVOCATION = 153,
	ACTION_LOOK_AROUND = 154,
	ACTION_CHEERS = 155,
	ACTION_HANDCLAP = 156,
	ACTION_POINTDANCE = 157,
	ACTION_JACK1 = 158,
	ACTION_JACK2 = 159,
	ACTION_SANTA1_1 = 160,
	ACTION_SANTA1_2 = 161,
	ACTION_SANTA1_3 = 162,
	ACTION_SANTA2_1 = 163,
	ACTION_SANTA2_2 = 164,
	ACTION_SANTA2_3 = 165,
	ACTION_RAGEBUFF_1 = 166,
	ACTION_RAGEBUFF_2 = 167,
	ACTION_PICKAXE = 168,
};

enum eAttributeType
{
	ATTRIBUTE_NONE = 0,
	ATTRIBUTE_SPECIAL = 1,
	ATTRIBUTE_MONSTER = 2,
	ATTRIBUTE_SUMMON = 3,
	ATTRIBUTE_SUMMONED = 4,
	ATTRIBUTE_INVASION = 5,
	ATTRIBUTE_KALIMA = 6,
	ATTRIBUTE_CASTLE = 7,
	ATTRIBUTE_ACHERON = 8,
};

enum eDamageReductionType
{
	DAMAGE_REDUCTION_EXCELLENT_ITEM = 0,
	DAMAGE_REDUCTION_SET_ITEM = 1,
	DAMAGE_REDUCTION_JOH_ITEM = 2,
	DAMAGE_REDUCTION_380_ITEM = 3,
	DAMAGE_REDUCTION_MASTER_SKILL_TREE = 4,
	DAMAGE_REDUCTION_SOCKET_ITEM = 5,
};

enum eEmotionType
{
	EMOTION_REST = 0,
	EMOTION_ATTACK = 1,
	EMOTION_MOVE = 2,
	EMOTION_ESCAPE = 3,
};

enum eExperienceType
{
	EXPERIENCE_COMMON = 0,
	EXPERIENCE_PARTY = 1,
	EXPERIENCE_QUEST = 2,
	EXPERIENCE_DEVIL_SQUARE = 3,
	EXPERIENCE_BLOOD_CASTLE = 4,
	EXPERIENCE_CHAOS_CASTLE = 5,
	EXPERIENCE_CRYWOLF = 6,
	EXPERIENCE_ILLUSION_TEMPLE = 7,
	EXPERIENCE_IMPERIAL_GUARDIAN = 8,
	EXPERIENCE_CHAOS_CASTLE_FINAL = 9,
	EXPERIENCE_ARCA_BATTLE = 10,
};

enum eTeleportStatus
{
	TELEPORT_NONE = 0,
	TELEPORT_STANDBY = 1,
	TELEPORT_READY = 2,
	TELEPORT_UNK = 3
};

enum eViewState
{
	VIEW_STATE_NONE = 0,
	VIEW_STATE_TELEPORT = 1,
	VIEW_STATE_ACTION_1 = 2,
	VIEW_STATE_ACTION_2 = 3,
	VIEW_STATE_ACTION_3 = 4,
};

struct MESSAGE_STATE_MACHINE
{
	MESSAGE_STATE_MACHINE() // OK
	{
		this->Clear();
	}

	void Clear() // OK
	{
		this->MsgCode = -1;
		this->SendUser = -1;
		this->MsgTime = 0;
		this->SubCode = 0;
	}

	int MsgCode;
	int SendUser;
	int MsgTime;
	int SubCode;
};

struct MESSAGE_STATE_MACHINE_COMMON
{
	MESSAGE_STATE_MACHINE_COMMON() // OK
	{
		this->CommonStruct = new MESSAGE_STATE_MACHINE;

		for(int n=0;n < MAX_MONSTER_SEND_MSG;n++){this->ObjectStruct[n] = this->CommonStruct;}
	}

	MESSAGE_STATE_MACHINE& operator[](int index) // OK
	{
		return (*this->ObjectStruct[index]);
	}

	MESSAGE_STATE_MACHINE* CommonStruct;
	MESSAGE_STATE_MACHINE* ObjectStruct[MAX_MONSTER_SEND_MSG];
};

struct MESSAGE_STATE_MACHINE_HEADER
{
	MESSAGE_STATE_MACHINE_HEADER() // OK
	{
		this->CommonStruct = new MESSAGE_STATE_MACHINE_COMMON;

		for(int n=0;n < MAX_OBJECT;n++){this->ObjectStruct[n] = this->CommonStruct;}
	}

	MESSAGE_STATE_MACHINE_COMMON& operator[](int index) // OK
	{
		return (*this->ObjectStruct[index]);
	}

	MESSAGE_STATE_MACHINE_COMMON* CommonStruct;
	MESSAGE_STATE_MACHINE_COMMON* ObjectStruct[MAX_OBJECT];
};

struct MESSAGE_STATE_ATTACK_MACHINE
{
	MESSAGE_STATE_ATTACK_MACHINE() // OK
	{
		this->Clear();
	}

	void Clear() // OK
	{
		this->MsgCode = -1;
		this->SendUser = -1;
		this->MsgTime = 0;
		this->SubCode = 0;
		this->SubCode2 = 0;
	}

	int MsgCode;
	int SendUser;
	int MsgTime;
	int SubCode;
	int SubCode2;
};

struct MESSAGE_STATE_ATTACK_MACHINE_COMMON
{
	MESSAGE_STATE_ATTACK_MACHINE_COMMON() // OK
	{
		this->CommonStruct = new MESSAGE_STATE_ATTACK_MACHINE;

		for(int n=0;n < MAX_MONSTER_SEND_ATTACK_MSG;n++){this->ObjectStruct[n] = this->CommonStruct;}
	}

	MESSAGE_STATE_ATTACK_MACHINE& operator[](int index) // OK
	{
		return (*this->ObjectStruct[index]);
	}

	MESSAGE_STATE_ATTACK_MACHINE* CommonStruct;
	MESSAGE_STATE_ATTACK_MACHINE* ObjectStruct[MAX_MONSTER_SEND_ATTACK_MSG];
};

struct MESSAGE_STATE_ATTACK_MACHINE_HEADER
{
	MESSAGE_STATE_ATTACK_MACHINE_HEADER() // OK
	{
		this->CommonStruct = new MESSAGE_STATE_ATTACK_MACHINE_COMMON;

		for(int n=0;n < MAX_OBJECT;n++){this->ObjectStruct[n] = this->CommonStruct;}
	}

	MESSAGE_STATE_ATTACK_MACHINE_COMMON& operator[](int index) // OK
	{
		return (*this->ObjectStruct[index]);
	}

	MESSAGE_STATE_ATTACK_MACHINE_COMMON* CommonStruct;
	MESSAGE_STATE_ATTACK_MACHINE_COMMON* ObjectStruct[MAX_OBJECT];
};

struct EFFECT_OPTION
{
	int AddPhysiDamage;
	int AddMagicDamage;
	int AddCurseDamage;
	int AddMinPhysiDamage;
	int AddMaxPhysiDamage;
	int AddMinMagicDamage;
	int AddMaxMagicDamage;
	int AddMinCurseDamage;
	int AddMaxCurseDamage;
	int AddAttackSuccessRate;
	int AddPhysiSpeed;
	int AddMagicSpeed;
	int AddDefense;
	int AddDefenseSuccessRate;
	int AddMaxHP;
	int AddMaxMP;
	int AddMaxBP;
	int AddMaxSD;
	int AddStrength;
	int AddDexterity;
	int AddVitality;
	int AddEnergy;
	int AddLeadership;
	int AddDamageReduction;
	int AddDamageReflect;
	int AddCriticalDamageRate;
	int AddCriticalDamage;
	int AddExcellentDamageRate;
	int AddExcellentDamage;
	int AddExperienceRate;
	int AddMasterExperienceRate;
	int AddVolcanoExperienceRate;
	int AddPartyBonusExperienceRate;
	int AddIgnoreDefenseRate;
	int AddItemDropRate;
	int AddAcheronSetItemDropRate;
	int AddHPRecovery;
	int AddMPRecovery;
	int AddBPRecovery;
	int AddSDRecovery;
	int AddHPRecoveryRate;
	int AddMPRecoveryRate;
	int AddBPRecoveryRate;
	int AddSDRecoveryRate;
	int AddSwordPowerDamageRate;
	int AddSwordPowerDefenseRate;
	int SubDefense;
	int MulPhysiDamage;
	int MulMagicDamage;
	int MulCurseDamage;
	int MulAttackSuccessRate;
	int MulPhysiSpeed;
	int MulMagicSpeed;
	int MulDefense;
	int MulDefenseSuccessRate;
	int MulMaxHP;
	int MulMaxMP;
	int MulMaxBP;
	int MulMaxSD;
	int DivPhysiDamage;
	int DivMagicDamage;
	int DivCurseDamage;
	int DivAttackSuccessRate;
	int DivPhysiSpeed;
	int DivMagicSpeed;
	int DivDefense;
	int DivDefenseSuccessRate;
	int DivMaxHP;
	int DivMaxMP;
	int DivMaxBP;
	int DivMaxSD;
	int AddElementalDamage[5];
	int AddElementalDefense[5];
};

struct PENTAGRAM_OPTION
{
	int MulPentagramDamage;
	int MulPentagramDefense;
	int AddElementalCriticalDamageRate;
	int AddElementalDefenseTransferRate;
	int AddElementalAttackTransferRate;
	int MulElementalDefenseSuccessRate;
	int MulElementalAttackSuccessRate;
};

struct PENTAGRAM_JEWEL_OPTION
{
	int AddElementalDamage;
	int AddElementalDamagePvP;
	int AddElementalDamagePvM;
	int AddElementalDamageRange;
	int AddElementalDamageMelee;
	int MulElementalDamagePvP;
	int MulElementalDamagePvM;
	int MulElementalDamageVsFire;
	int MulElementalDamageVsWater;
	int MulElementalDamageVsEarth;
	int MulElementalDamageVsWind;
	int MulElementalDamageVsDark;
	int AddElementalDefense;
	int AddElementalDefensePvP;
	int AddElementalDefensePvM;
	int AddElementalDefenseRange;
	int AddElementalDefenseMelee;
	int MulElementalDefenseVsFire;
	int MulElementalDefenseVsWater;
	int MulElementalDefenseVsEarth;
	int MulElementalDefenseVsWind;
	int MulElementalDefenseVsDark;
	int MulElementalAttackSuccessRate;
	int MulElementalDefenseSuccessRate;
	int AddElementalDamageReductionPvP;
	int AddElementalDamageReductionPvM;
	int AddElementalSlowRate;
	int AddElementalDebuffRate;
	int AddElementalCriticalDamageRatePvP;
	int AddElementalCriticalDamageRatePvM;
	int AddElementalExcellentDamageRatePvP;
	int AddElementalExcellentDamageRatePvM;
};

struct ACTION_STATE
{
	bool Attack;
	bool Move;
	BYTE Emotion;
	BYTE EmotionCount;

	void Reset()
	{
		this->Attack = false;
		this->Move = false;
		this->Emotion = EMOTION_REST;
		this->EmotionCount = 0;
	}

	void Update()
	{
		if(this->EmotionCount > 0)
		{
			this->EmotionCount--;
		}
		else
		{
			this->Emotion = EMOTION_REST;
		}
	}
};

struct VIEWPORT_STRUCT
{
	#pragma pack(1)
	BYTE state;
	short index;
	BYTE type;
	#pragma pack()
};

struct HIT_DAMAGE_STRUCT
{
	int index;
	int damage;
	DWORD time;
};

struct INTERFACE_STATE
{
	DWORD use:2;
	DWORD state:4;
	DWORD type:10;
};

struct HELPER_STRUCT
{
	bool Started;
	bool Offline;
	int Stage;
	short StartX;
	short StartY;
	int Range[2];
	int MoveTime;
	int Skill[3];
	int Delay[2];
	int Buff[3];
	int PotPercent;
	int HealPercent;
	int DrainPercent;
	int PartyHealPercent;
	int PartyBuffTime;
	int PickJewel;
	int PickSet;
	int PickExc;
	int PickMoney;
	int PickExtra;
	char ItemList[12][16];
	int AutoPotion;
	int AutoHeal;
	int AutoDrainLife;
	int LongDistance;
	int OriginalPosition;
	int Combo;
	int Party;
	int PartyAutoHeal;
	int PartyAutoBuff;
	int DarkSpirit;
	int AutoBuff;
	int SkillDelay[2];
	int SkillCondition[2];
	int SkillPreCon[2];
	int SkillSubCon[2];
	int RepairItem;
	int PickAllItem;
	int PickSelected;
	int DarkSpiritAuto;
	int DarkSpiritAttack;
	int AutoAcceptFriend;
	int AutoAcceptGuild;
	int PotionElite;
	int ShortDistance;
	int RegularAttack;
	DWORD TimerMoney;
	DWORD TimerStage;
	DWORD TimerOffline;
	DWORD TimerBuffParty;
	DWORD TimerMove;
	DWORD TimerSkill;
	DWORD TimerDelay[2];
};

struct ATTACK_STRUCT
{
	int Started;
	int AutoAttack;
	int AutoBuff;
	int SkillIndex;
	int Offline;
	DWORD Timer;
	DWORD TimerSkill;
	int StartMap;
	int StartX;
	int StartY;
};

struct PICK_STRUCT 
{
	int Started;
	int ItemCount;
	char ItemList[20][20];
	int PickJewel;
	int PickSet;
	int PickExc;
	int PickMoney;
	DWORD Timer;
};

struct OBJECTSTRUCT
{
	int Index;
	int Connected;
	char LoginMessageSend;
	char LoginMessageCount;
	char CloseCount;
	char CloseType;
	BOOL EnableDelCharacter;
	struct PER_SOCKET_CONTEXT* PerSocketContext;
	SOCKET Socket;
	DWORD PacketTime;
	int PacketCount;
	bool ClientVerify;
	char IpAddr[16];
	char HardwareId[45];
	DWORD AutoSaveTime;
	DWORD ConnectTickCount;
	DWORD ClientTickCount;
	DWORD ServerTickCount;
	DWORD TimeCount;
	WORD Type;
	BYTE Live;
	char Account[11];
	char Name[11];
	char Password[11];
	char MonsterName[32];
	char PersonalCode[14];
	CComboSkill ComboSkill;
	WORD Class;
	BYTE DBClass;
	BYTE ClassCode;
	BYTE ChangeUp;
	short Level;
	int LevelUpPoint;
	WORD FruitAddPoint;
	WORD FruitSubPoint;
	DWORD Experience;
	DWORD NextExperience;
	DWORD Money;
	int Strength;
	int Dexterity;
	int Vitality;
	int Energy;
	int Leadership;
	float Life;
	float MaxLife;
	float ScriptMaxLife;
	float Mana;
	float MaxMana;
	DWORD ChatLimitTime;
	BYTE ChatLimitTimeSec;
	int AddStrength;
	int AddDexterity;
	int AddVitality;
	int AddEnergy;
	int AddLeadership;
	int BP;
	int MaxBP;
	int AddBP;
	float VitalityToLife;
	float EnergyToMana;
	BYTE PKLevel;
	int PKCount;
	int PKTime;
	short X;
	short Y;
	BYTE Dir;
	BYTE Map;
	BYTE DeathMap;
	short DeathX;
	short DeathY;
	int AddLife;
	int AddMana;
	int Shield;
	int MaxShield;
	int AddShield;
	BYTE StartX;
	BYTE StartY;
	short OldX;
	short OldY;
	short TX;
	short TY;
	short MTX;
	short MTY;
	int PathCount;
	int PathCur;
	char PathStartEnd;
	short PathOri[15];
	short PathX[15];
	short PathY[15];
	char PathDir[15];
	DWORD PathTime;
	DWORD Authority;
	DWORD AuthorityCode;
	DWORD Penalty;
	ACTION_STATE ActionState;
	BYTE ActionNumber;
	DWORD CurActionTime;
	DWORD NextActionTime;
	DWORD DelayActionTime;
	DWORD State;
	BYTE Rest;
	char ViewState;
	DWORD LastMoveTime;
	BYTE LastReset;
	short LastX;
	short LastY;
	DWORD LastAttackTime;
	DWORD TeleportTime;
	char Teleport;
	char KillerType;
	char DieRegen;
	char RegenOk;
	BYTE RegenMapNumber;
	BYTE RegenMapX;
	BYTE RegenMapY;
	DWORD RegenTime;
	DWORD MaxRegenTime;
	short PosNum;
	char DelayLevel;
	int DrinkSpeed;
	DWORD DrinkSpeedLastTime;
	int DrinkDamage;
	DWORD DrinkDamageLastTime;
	DWORD MonsterDeleteTime;
	char KalimaGateExist;
	int KalimaGateIndex;
	char KalimaGateEnterCount;
	int KalimaGateLevel;
	OBJECTSTRUCT* AttackObj;
	bool AttackerKilled;
	int SelfDefense[MAX_SELF_DEFENSE];
	DWORD SelfDefenseTime[MAX_SELF_DEFENSE];
	DWORD MySelfDefenseTime;
	int PartyNumber;
	int PartyTargetUser;
	struct GUILD_INFO* Guild;
	char GuildName[11];
	int GuildIndex;
	int GuildRank;
	int SummonIndex;
	int Change;
	short TargetNumber;
	short TargetShopNumber;
	short ShopNumber;
	short LastAttackerID;
	int PhysiDamageMin;
	int PhysiDamageMax;
	int MagicDamageMin;
	int MagicDamageMax;
	int CurseDamageMin;
	int CurseDamageMax;
	int PhysiDamageMaxLeft;
	int PhysiDamageMinLeft;
	int PhysiDamageMaxRight;
	int PhysiDamageMinRight;
	int DKDamageMultiplierRate;
	int DLDamageMultiplierRate;
	int RFDamageMultiplierRate[3];
	int AttackSuccessRate;
	int PhysiSpeed;
	int MagicSpeed;
	int Defense;
	int MagicDefense;
	int DefenseSuccessRate;
	#if(GAMESERVER_UPDATE>=701)
	int ElementalAttribute;
	int ElementalPattern;
	int ElementalDefense;
	int ElementalDamageMin;
	int ElementalDamageMax;
	int ElementalAttackSuccessRate;
	int ElementalDefenseSuccessRate;
	#endif
	short MoveSpeed;
	short MoveRange;
	short AttackRange;
	short AttackType;
	short ViewRange;
	short Attribute;
	short ItemRate;
	short MoneyRate;
	CSkill* SkillBackup;
	CSkill* Skill;
	WORD MultiSkillIndex;
	WORD MultiSkillCount;
	WORD RageFighterSkillIndex;
	WORD RageFighterSkillCount;
	WORD RageFighterSkillTarget;
	BYTE CharSet[18];
	BYTE Resistance[MAX_RESISTANCE_TYPE];
	BYTE AddResistance[MAX_RESISTANCE_TYPE];
	VIEWPORT_STRUCT* VpPlayer;
	VIEWPORT_STRUCT* VpPlayer2;
	VIEWPORT_STRUCT* VpPlayerItem;
	int VPCount;
	int VPCount2;
	int VPCountItem;
	HIT_DAMAGE_STRUCT* HitDamage;
	short HitDamageCount;
	INTERFACE_STATE Interface;
	DWORD InterfaceTime;
	char Transaction;
	CItem* Inventory;
	CItem* Inventory1;
	CItem* Inventory2;
	BYTE* InventoryMap;
	BYTE* InventoryMap1;
	BYTE* InventoryMap2;
	CItem* Trade;
	BYTE* TradeMap;
	int TradeMoney;
	bool TradeOk;
	CItem* Warehouse;
	BYTE* WarehouseMap;
	char WarehouseCount;
	short WarehousePW;
	BYTE WarehouseLock;
	int WarehouseMoney;
	int WarehouseSave;
	CItem* ChaosBox;
	BYTE* ChaosBoxMap;
	int ChaosMoney;
	int ChaosSuccessRate;
	int ChaosLock;
	#if(GAMESERVER_UPDATE>=802)
	int LoadEventInventory;
	CItem* EventInventory;
	CItem* EventInventory1;
	CItem* EventInventory2;
	BYTE* EventInventoryMap;
	BYTE* EventInventoryMap1;
	BYTE* EventInventoryMap2;
	#endif
	#if(GAMESERVER_UPDATE>=803)
	int MuunItemStatus[2];
	int LoadMuunInventory;
	CItem* MuunInventory;
	BYTE* MuunInventoryMap;
	#endif
	DWORD Option;
	int ChaosCastleBlowTime;
	int DuelUserReserved;
	int DuelUserRequested;
	int DuelUser;
	BYTE DuelScore;
	DWORD DuelTickCount;
	bool PShopOpen;
	bool PShopTransaction;
	bool PShopItemChange;
	bool PShopRedrawAbs;
	char PShopText[36];
	bool PShopWantDeal;
	int PShopDealerIndex;
	char PShopDealerName[10];
	int PShopCustom;
	int PShopCustomType;
	int PShopCustomOffline;
	int PShopCustomOfflineTime;
	CRITICAL_SECTION PShopTrade;
	int VpPShopPlayer[MAX_VIEWPORT];
	int VpPShopPlayerCount;
	bool IsInBattleGround;
	bool HaveWeaponInHand;
	bool UseEventServer;
	int LoadWarehouse;
	BYTE Quest[50];
	bool SendQuestInfo;
	int CheckLifeTime;
	BYTE LastTeleportTime;
	BYTE SkillNovaState;
	BYTE SkillNovaCount;
	DWORD SkillNovaTime;
	BYTE ReqWarehouseOpen;
	bool IsFullSetItem;
	WORD SkillSummonPartyTime;
	BYTE SkillSummonPartyMap;
	BYTE SkillSummonPartyX;
	BYTE SkillSummonPartyY;
	bool IsChaosMixCompleted;
	bool SkillLongSpearChange;
	DWORD CharSaveTime;
	#if(GAMESERVER_UPDATE>=301)
	int LoadQuestKillCount;
	int QuestKillCountIndex;
	struct QUEST_KILL_COUNT* QuestKillCount;
	#endif
	int LoadMasterLevel;
	int MasterLevel;
	int MasterPoint;
	#pragma pack(1)
	QWORD MasterExperience;
	QWORD MasterNextExperience;
	#pragma pack()
	#if(GAMESERVER_UPDATE>=402)
	CSkill* MasterSkill;
	#endif
	CEffect* Effect;
	DWORD* SkillDelay;
	DWORD* HackPacketDelay;
	DWORD* HackPacketCount;
	int ExtInventory;
	int ExtWarehouse;
	int WarehouseNumber;
	int AccountLevel;
	char AccountExpireDate[20];
	char AutoPartyPassword[11];
	int AutoAddPointCount;
	int AutoAddPointStats[5];
	int AutoResetEnable;
	int AutoResetStats[5];
	int MiniMapState;
	int MiniMapValue;
	int MiningStage;
	int MiningIndex;
	int UseGuildMatching;
	int UseGuildMatchingJoin;
	int UsePartyMatching;
	int UsePartyMatchingJoin;
	int CashShopTransaction[4];
	int CommandManagerTransaction[1];
	int LuckyCoinTransaction[2];
	int GoldenArcherTransaction;
	int PcPointTransaction[2];
	DWORD TradeOkTime;
	DWORD PotionTime;
	DWORD ComboTime;
	DWORD HPAutoRecuperationTime;
	DWORD MPAutoRecuperationTime;
	DWORD BPAutoRecuperationTime;
	DWORD SDAutoRecuperationTime;
	DWORD OnlineRewardTime;
	int Reset;
	int MasterReset;
	int ChangeSkin;
	#if(GAMESERVER_UPDATE>=501)
	int LoadQuestWorld;
	struct QUEST_WORLD_LIST* QuestWorldList;
	int QuestWorldMonsterClass;
	int LoadGens;
	int GensFamily;
	int GensRank;
	int GensSymbol;
	int GensContribution;
	int GensNextContribution;
	struct GENS_SYSTEM_VICTIM_LIST* GensVictimList;
	#endif
	#if(GAMESERVER_UPDATE>=701)
	struct PENTAGRAM_JEWEL_INFO* PentagramJewelInfo_Inventory;
	struct PENTAGRAM_JEWEL_INFO* PentagramJewelInfo_Warehouse;
	#endif
	#if(GAMESERVER_UPDATE>=802)
	class CMuRummyInfo* MuRummyInfo;
	#endif
	EFFECT_OPTION EffectOption;
	#if(GAMESERVER_UPDATE>=701)
	PENTAGRAM_OPTION PentagramOption;
	PENTAGRAM_JEWEL_OPTION PentagramJewelOption;
	#endif
	int ArmorSetBonus;
	int SkillDamageBonus;
	int DoubleDamageRate;
	int TripleDamageRate;
	int IgnoreDefenseRate;
	int IgnoreShieldGaugeRate;
	int CriticalDamageRate;
	int CriticalDamage;
	int ExcellentDamageRate;
	int ExcellentDamage;
	int ResistDoubleDamageRate;
	int ResistIgnoreDefenseRate;
	int ResistIgnoreShieldGaugeRate;
	int ResistCriticalDamageRate;
	int ResistExcellentDamageRate;
	int ResistStunRate;
	int ExperienceRate;
	int MasterExperienceRate;
	int ItemDropRate;
	int MoneyAmountDropRate;
	int HPRecovery;
	int MPRecovery;
	int BPRecovery;
	int SDRecovery;
	int HPRecoveryRate;
	int MPRecoveryRate;
	int BPRecoveryRate;
	int SDRecoveryRate;
	int SDRecoveryType;
	int HPRecoveryCount;
	int MPRecoveryCount;
	int BPRecoveryCount;
	int SDRecoveryCount;
	int MPConsumptionRate;
	int BPConsumptionRate;
	int ShieldGaugeRate;
	int DecreaseShieldGaugeRate;
	int DamagePvP;
	int DefensePvP;
	int AttackSuccessRatePvP;
	int DefenseSuccessRatePvP;
	int ShieldDamageReduction;
	int ShieldDamageReductionTime;
	int DamageReduction[MAX_DAMAGE_REDUCTION];
	int DamageReflect;
	int HuntHP;
	int HuntMP;
	int HuntBP;
	int HuntSD;
	int WeaponDurabilityRate;
	int ArmorDurabilityRate;
	int WingDurabilityRate;
	int GuardianDurabilityRate;
	int PendantDurabilityRate;
	int RingDurabilityRate;
	int PetDurabilityRate;
	int FullDamageReflectRate;
	int DefensiveFullHPRestoreRate;
	int DefensiveFullMPRestoreRate;
	int DefensiveFullSDRestoreRate;
	int DefensiveFullBPRestoreRate;
	int OffensiveFullHPRestoreRate;
	int OffensiveFullMPRestoreRate;
	int OffensiveFullSDRestoreRate;
	int OffensiveFullBPRestoreRate;
	int ResurrectionTalismanActive;
	int ResurrectionTalismanMap;
	int ResurrectionTalismanX;
	int ResurrectionTalismanY;
	int MobilityTalismanActive;
	int MobilityTalismanMap;
	int MobilityTalismanX;
	int MobilityTalismanY;
	bool MapServerMoveQuit;
	bool MapServerMoveRequest;
	DWORD MapServerMoveQuitTickCount;
	short NextServerCode;
	short LastServerCode;
	short DestMap;
	BYTE DestX;
	BYTE DestY;
	union
	{
		struct
		{
			BYTE CsNpcExistVal1;
			BYTE CsNpcExistVal2;
			BYTE CsNpcExistVal3;
			BYTE CsNpcExistVal4;
		};

		int CsNpcExistVal;
	};
	BYTE CsNpcType;
	BYTE CsGateOpen;
	int CsGateLeverLinkIndex;
	BYTE CsNpcDfLevel;
	BYTE CsNpcRgLevel;
	BYTE CsJoinSide;
	bool CsGuildInvolved;
	bool CsSeniorMix;
	bool IsCastleNPCUpgradeCompleted;
	BYTE CsSiegeWeaponState;
	int CsWeaponIndex;
	BYTE KillCount;
	int AccumulatedDamage;
	BYTE LifeStoneCount;
	BYTE CreationState;
	int CreatedActivationTime;
	int AccumulatedCrownAccessTime;
	CMonsterSkillElementOption MonsterSkillElementOption;
	int BasicAI;
	int CurrentAI;
	int CurrentAIState;
	int LastAIRunTime;
	int GroupNumber;
	int SubGroupNumber;
	int GroupMemberGuid;
	int RegenType;
	CMonsterAIAgro Agro;
	int LastAutomataRuntime;
	int LastAutomataDelay;
	int CrywolfMVPScore;
	DWORD RefillTime;
	DWORD LastCheckTick;
	DWORD CommandCheckTime[200];
	DWORD Coin1;
	DWORD Coin2;
	DWORD Coin3;
	WORD Married;
	char MarryName[11];
	char MarryRequested;
	int MarryRequestIndex;
	DWORD MarryRequestTime;
	DWORD HackSkillDelay[618];
	DWORD HackSkillCount[618];
	WORD BCCount;
	WORD CCCount;
	WORD DSCount;
	WORD DGCount;
	WORD ITCount;
	WORD IGCount;
	BYTE Permission[50];
	HELPER_STRUCT Helper;
	ATTACK_STRUCT Attack;
	PICK_STRUCT Pick;
	int InvasionIndex;
	//FakeOnline_EMU
#if USE_FAKE_ONLINE == TRUE
	bool IsFakeOnline;
	DWORD m_OfflineMoveDelay;
	bool IsFakeRegen;
	int	 IsFakePVPMode;
	int	 IsFakePartyMode;
	int	 IsFakeMoveRange;
	DWORD	 IsFakeTimeLag;
	DWORD FakeBotPartyInviteCooldownTick; // Nuevo nombre único
#endif
	
	int Zen;
	int ChaosBank;
	int BlessBank;
	int SoulBank;
	int LifeBank;
	int CreateonBank;
	int GuardianBank;
	int HarmonyBank;
	int LowStoneBank;
	int HighStoneBank;
	int GemStoneBank;



	BYTE	RecoveryPotionOn;
	BYTE	RecoveryPotionPercent;
	BYTE	RecoveryHealOn;			//-> CLASS_ELF
	BYTE	RecoveryHealPercent;	//-> CLASS_ELF
	BYTE	RecoveryDrainOn;		//-> CLASS_SUMMONER
	BYTE	RecoveryDrainPercent;	//-> CLASS_SUMMONER
	BYTE	DistanceLongOn;
	BYTE	DistanceReturnOn;
	DWORD	DistanceMin;
	DWORD	SkillBasicID;
	BYTE	ComboOn;				//-> CLASS_KNIGHT
	BYTE	PartyModeOn;
	BYTE	PartyModeHealOn;		//-> CLASS_ELF
	BYTE	PartyModeHealPercent;	//-> CLASS_ELF
	BYTE	PartyModeBuffOn;
	BYTE	BuffOn;
	DWORD	BuffSkill[3];
	DWORD	ObtainRange;
	BYTE	ObtainRepairOn;
	BYTE	ObtainPickNear;
	BYTE	ObtainPickSelected;
	BYTE	ObtainPickJewels;
	BYTE	ObtainPickAncient;
	BYTE	ObtainPickMoney;
	BYTE	ObtainPickExcellent;
	BYTE	ObtainPickExtra;
	int		ObtainPickExtraCount;
	char	ObtainPickItemList[12][16];
	DWORD	HuntingRange;
	short m_OfflineCoordX;
	short m_OfflineCoordY;
	DWORD m_OfflineTimeResetMove;
	int AttackCustomDelay;


};

struct OBJECTSTRUCT_HEADER
{
	OBJECTSTRUCT_HEADER() // OK
	{
		this->CommonStruct = new OBJECTSTRUCT;

		for(int n=0;n < MAX_OBJECT;n++){this->ObjectStruct[n] = this->CommonStruct;}
	}

	OBJECTSTRUCT& operator[](int index) // OK
	{
		return (*this->ObjectStruct[index]);
	}

	OBJECTSTRUCT* CommonStruct;
	OBJECTSTRUCT* ObjectStruct[MAX_OBJECT];
};

typedef OBJECTSTRUCT* LPOBJ;

extern OBJECTSTRUCT_HEADER gObj;

extern MESSAGE_STATE_MACHINE_HEADER gSMMsg;

extern MESSAGE_STATE_ATTACK_MACHINE_HEADER gSMAttackProcMsg;

extern DWORD gLevelExperience[MAX_CHARACTER_LEVEL+1];

extern DWORD gPetExperience[MAX_CHARACTER_LEVEL+1];

//**************************************************************************//
// OBJECT MAIN FUNCTIONS ***************************************************//
//**************************************************************************//
void gObjEventRunProc();
void gObjViewportProc();
void gObjFirstProc();
void gObjCloseProc();
void gObjCountProc();
void gObjAccountLevelProc();
void gObjMathAuthenticatorProc();
//**************************************************************************//
// OBJECT BASE FUNCTIONS ***************************************************//
//**************************************************************************//
void gObjInit();
void gObjAllDisconnect();
void gObjOfflineDisconnect();
void gObjSetExperienceTable();
void gObjCharZeroSet(int aIndex);
void gObjClearPlayerOption(LPOBJ lpObj);
void gObjClearSpecialOption(LPOBJ lpObj);
void gObjCalcExperience(LPOBJ lpObj);
bool gObjAllocData(int aIndex);
void gObjFreeData(int aIndex);
short gObjAddSearch(SOCKET socket,char* IpAddress);
short gObjAdd(SOCKET socket,char* IpAddress,int aIndex);
short gObjDel(int aIndex);
LPOBJ gObjFind(char* name);
int gObjCalcDistance(LPOBJ lpObj,LPOBJ lpTarget);
int gObjCalcDistance(LPOBJ lpObj,int x,int y);
bool gObjGetRandomFreeLocation(int map,int* ox,int* oy,int tx,int ty,int count);
bool gObjGetRandomMonsterLocation(int map,int* ox,int* oy,int tx,int ty,int count);
//**************************************************************************//
// OBJECT CHECK FUNCTIONS **************************************************//
//**************************************************************************//
bool gObjIsConnected(int aIndex);
bool gObjIsConnectedGP(int aIndex);
bool gObjIsConnectedGS(int aIndex);
bool gObjIsNameValid(int aIndex,char* name);
bool gObjIsAccountValid(int aIndex,char* account);
bool gObjIsChangeSkin(int aIndex);
bool gObjCheckMaxMoney(int aIndex,DWORD AddMoney);
bool gObjCheckPersonalCode(int aIndex,char* PersonalCode);
bool gObjCheckResistance(LPOBJ lpObj,int type);
bool gObjCheckTeleportArea(int aIndex,int x,int y);
bool gObjCheckMapTile(LPOBJ lpObj,int type);
void gObjCoinAdd(int aIndex,int Coin1,int Coin2,int Coin3);
void gObjCoinSub(int aIndex,int Coin1,int Coin2,int Coin3);
//**************************************************************************//
// ITEM TRANSACTION FUNCTIONS **********************************************//
//**************************************************************************//
bool gObjFixInventoryPointer(int aIndex);
void gObjSetInventory1Pointer(LPOBJ lpObj);
void gObjSetInventory2Pointer(LPOBJ lpObj);
bool gObjFixEventInventoryPointer(int aIndex);
void gObjSetEventInventory1Pointer(LPOBJ lpObj);
void gObjSetEventInventory2Pointer(LPOBJ lpObj);
bool gObjInventoryTransaction(int aIndex);
bool gObjInventoryCommit(int aIndex);
bool gObjInventoryRollback(int aIndex);
//**************************************************************************//
// VIEWPORT FUNCTIONS ******************************************************//
//**************************************************************************//
void gObjSetViewport(int aIndex,int state);
void gObjClearViewport(LPOBJ lpObj);
void gObjViewportListProtocolDestroy(LPOBJ lpObj);
void gObjViewportListProtocolCreate(LPOBJ lpObj);
void gObjViewportListProtocol(int aIndex);
void gObjViewportListDestroy(int aIndex);
void gObjViewportListCreate(int aIndex);
void gObjViewportUnionUpdate(int aIndex);
//**************************************************************************//
// USER FUNCTIONS **********************************************************//
//**************************************************************************//
void gObjSetKillCount(int aIndex,int type);
void gObjTeleportMagicUse(int aIndex,int x,int y);
void gObjInterfaceCheckTime(LPOBJ lpObj);
void gObjSkillNovaCheckTime(LPOBJ lpObj);
void gObjPKDownCheckTime(LPOBJ lpObj,int time,int type);
void gObjUserDie(LPOBJ lpObj,LPOBJ lpTarget);
void gObjKillerItemDrop(LPOBJ lpObj, LPOBJ lpTarget);
void gObjPlayerKiller(LPOBJ lpObj,LPOBJ lpTarget);
int gObjMoveGate(int aIndex,int gate);
void gObjTeleport(int aIndex,int map,int x,int y);
void gObjSummonAlly(LPOBJ lpObj,int map,int x,int y);
void gObjSkillUseProc(LPOBJ lpObj);
void gObjUserKill(int aIndex);
int gObjInventorySearchSerialNumber(LPOBJ lpObj,DWORD serial);
int gObjWarehouseSearchSerialNumber(LPOBJ lpObj,DWORD serial);
void gObjAddMsgSend(LPOBJ lpObj,int MsgCode,int SendUser,int SubCode);
void gObjAddMsgSendDelay(LPOBJ lpObj,int MsgCode,int SendUser,int MsgTimeDelay,int SubCode);
void gObjAddAttackProcMsgSendDelay(LPOBJ lpObj,int MsgCode,int SendUser,int MsgTimeDelay,int SubCode,int SubCode2);
//**************************************************************************//
// OTHERS FUNCTIONS ********************************************************//
//**************************************************************************//
void gObjSecondProc();
void gObjDelayLifeCheck(int aIndex);
bool gObjBackSpring(LPOBJ lpObj,LPOBJ lpTarget);
bool gObjBackSpring2(LPOBJ lpObj,LPOBJ lpTarget,int count);
bool gObjIsSelfDefense(LPOBJ lpObj,int aTargetIndex);
void gObjCheckSelfDefense(LPOBJ lpObj,int aTargetIndex);
void gObjTimeCheckSelfDefense(LPOBJ lpObj);
void gObjSetPosition(int aIndex,int x,int y);
void gObjAuthorityCodeSet(LPOBJ lpObj);
void gObjUseDrink(LPOBJ lpObj,int level);
void gObjCustomLogPlusChaosMix(LPOBJ lpObj,int type,int index);
bool gObjCheckAutoParty(LPOBJ lpObj,LPOBJ lpTarget);
void gObjEventEntrySaveSend(LPOBJ lpObj,int type);

extern int gObjCount;
extern int gObjMonCount;
extern int gObjCallMonCount;
extern int gObjTotalUser;
extern int gObjTotalMonster;
extern int gObjTotalOffline;
extern int gCloseMsg;
extern int gCloseMsgTime;
extern int gCloseServer;