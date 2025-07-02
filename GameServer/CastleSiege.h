// CastleSiege.h: interface for the CCastleSiege class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Protocol.h"

enum eCastleSiegeState
{
	CASTLESIEGE_STATE_NONE = -1,
	CASTLESIEGE_STATE_IDLE_1 = 0,
	CASTLESIEGE_STATE_REGSIEGE = 1,
	CASTLESIEGE_STATE_IDLE_2 = 2,
	CASTLESIEGE_STATE_REGMARK = 3,
	CASTLESIEGE_STATE_IDLE_3 = 4,
	CASTLESIEGE_STATE_NOTIFY = 5,
	CASTLESIEGE_STATE_READYSIEGE = 6,
	CASTLESIEGE_STATE_STARTSIEGE = 7,
	CASTLESIEGE_STATE_ENDSIEGE = 8,
	CASTLESIEGE_STATE_ENDCYCLE = 9,
};

#define CASTLESIEGE_LEFTTIME_NOTRUNNING -3
#define CASTLESIEGE_LEFTTIME_ENDSIEGE -2
#define CASTLESIEGE_LEFTTIME_ONSIGE -1

#define CASTLESIEGE_DATALOAD_1 1
#define CASTLESIEGE_DATALOAD_2 2
#define CASTLESIEGE_DATALOAD_3 3
#define CASTLESIEGE_DATALOAD_4 4

#define CS_CHECK_UPGRADE(x) ((((x))<0)?0:(((x))>2)?0:1)
#define MACRO1(x) ((x&0xFF)&0xFF)
#define MACRO2(x) ((x&0xFFFF)&0xFFFF)
#define MACRO3(x) (((DWORD)x&0xFFFFFFFF ))
#define MONTH_MACRO(x) ((((x))<0)?0:(((x))>12)?0:1)
#define DAY_MACRO(x) ((((x-1))<0)?0:(((x-1))>30)?0:1)
#define HOUR_MACRO(x) ((((x))<0)?0:(((x))>23)?0:1)
#define MINUTE_MACRO(x) ((((x))<0)?0:(((x))>59)?0:1)
#define XY_MACRO(x) ((((x))<0)?0:(((x))>255)?0:1)

//**********************************************//
//************ Client -> GameServer ************//
//**********************************************//

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_CASTLE_SIEGE_STATE_SEND
{
	PSBMSG_HEAD header; // C1:B2:00
	BYTE result;
	BYTE state;
	BYTE StartYear[2];
	BYTE StartMonth;
	BYTE StartDay;
	BYTE StartHour;
	BYTE StartMinute;
	BYTE EndYear[2];
	BYTE EndMonth;
	BYTE EndDay;
	BYTE EndHour;
	BYTE EndMinute;
	BYTE SiegeStartYear[2];
	BYTE SiegeStartMonth;
	BYTE SiegeStartDay;
	BYTE SiegeStartHour;
	BYTE SiegeStartMinute;
	char GuildName[8];
	char Name[10];
	BYTE RemainTime[4];
};

struct PMSG_CASTLE_SIEGE_REGISTER_GUILD_SEND
{
	PSBMSG_HEAD header; // C1:B2:01
	BYTE result;
	char GuildName[8];
};

struct PMSG_CASTLE_SIEGE_GIVEUP_GUILD_SEND
{
	PSBMSG_HEAD header; // C1:B2:02
	BYTE result;
	BYTE giveup;
	char GuildName[8];
};

struct PMSG_CASTLE_SIEGE_REGISTER_GUILD_INFO_SEND
{
	PSBMSG_HEAD header; // C1:B2:03
	BYTE result;
	char GuildName[8];
	BYTE Mark[4];
	BYTE giveup;
	BYTE ranking;
};

struct PMSG_CASTLE_SIEGE_REGISTER_MARK_SEND
{
	PSBMSG_HEAD header; // C1:B2:04
	BYTE result;
	char GuildName[8];
	BYTE Mark[4];
};

struct PMSG_CASTLE_SIEGE_NPC_BUY_SEND
{
	PSBMSG_HEAD header; // C1:B2:05
	BYTE result;
	DWORD npc;
	DWORD index;
};

struct PMSG_CASTLE_SIEGE_NPC_REPAIR_SEND
{
	PSBMSG_HEAD header; // C1:B2:06
	BYTE result;
	DWORD npc;
	DWORD index;
	DWORD hp;
	DWORD maxhp;
};

struct PMSG_CASTLE_SIEGE_NPC_UPGRADE_SEND
{
	PSBMSG_HEAD header; // C1:B2:07
	BYTE result;
	DWORD npc;
	DWORD index;
	DWORD type;
	DWORD value;
};

struct PMSG_CASTLE_SIEGE_TAX_MONEY_INFO_SEND
{
	PSBMSG_HEAD header; // C1:B2:08
	BYTE result;
	BYTE tax_rate_chaos;
	BYTE tax_rate_store;
	BYTE money[8];
};

struct PMSG_CASTLE_SIEGE_TAX_RATE_INFO_SEND
{
	PSBMSG_HEAD header; // C1:B2:09
	BYTE result;
	BYTE tax_type;
	BYTE tax_rate[4];
};

struct PMSG_CASTLE_SIEGE_MONEY_OUT_SEND
{
	PSBMSG_HEAD header; // C1:B2:10
	BYTE result;
	BYTE money[8];
};

struct PMSG_CASTLE_SIEGE_GATE_STATE_SEND
{
	PSBMSG_HEAD header; // C1:B2:11
	BYTE result;
	BYTE index[2];
};

struct PMSG_CASTLE_SIEGE_GATE_OPERATE_SEND
{
	PSBMSG_HEAD header; // C1:B2:12
	BYTE result;
	BYTE operation;
	BYTE index[2];
};

struct PMSG_CASTLE_SIEGE_GATE_OPERATE_STATE_SEND
{
	PSBMSG_HEAD header; // C1:B2:13
	BYTE operation;
	BYTE index[2];
};

struct PMSG_CASTLE_SIEGE_SWITCH_STATE_SEND
{
	PSBMSG_HEAD header; // C1:B2:14
	BYTE index[2];
	BYTE target[2];
	BYTE state;
};

struct PMSG_CASTLE_SIEGE_CROWN_ACCESS_SEND
{
	PSBMSG_HEAD header; // C1:B2:15
	BYTE state;
	DWORD time;
};

struct PMSG_CASTLE_SIEGE_CROWN_STATE_SEND
{
	PSBMSG_HEAD header; // C1:B2:16
	BYTE state;
};

struct PMSG_CASTLE_SIEGE_START_STATE_SEND
{
	PSBMSG_HEAD header; // C1:B2:17
	BYTE state;
};

struct PMSG_CASTLE_SIEGE_NOTIFY_PROGRESS_SEND
{
	PSBMSG_HEAD header; // C1:B2:18
	BYTE state;
	char GuildName[8];
};

struct PMSG_CASTLE_SIEGE_JOIN_SIDE_SEND
{
	PSBMSG_HEAD header; // C1:B2:19
	BYTE side;
};

struct PMSG_CASTLE_SIEGE_MINI_MAP_OPEN_SEND
{
	PSBMSG_HEAD header; // C1:B2:1B
	BYTE result;
};

struct PMSG_CASTLE_SIEGE_COMMAND_REQUEST_SEND
{
	PSBMSG_HEAD header; // C1:B2:1D
	BYTE team;
	BYTE x;
	BYTE y;
	BYTE command;
};

struct PMSG_CASTLE_SIEGE_REMAIN_TIME_SEND
{
	PSBMSG_HEAD header; // C1:B2:1E
	BYTE hour;
	BYTE minute;
};

struct PMSG_CASTLE_SIEGE_ENABLE_HUNT_SEND
{
	PSBMSG_HEAD header; // C1:B2:1F
	BYTE result;
	BYTE allow;
};

struct PMSG_CASTLE_SIEGE_SWITCH_INFO_SEND
{
	PSBMSG_HEAD header; // C1:B2:20
	BYTE index[2];
	BYTE state;
	BYTE join;
	char GuildName[8];
	char Name[10];
};

struct PMSG_CASTLE_SIEGE_NPC_LIST_SEND
{
	PWMSG_HEAD header; // C1:B3
	BYTE result;
	DWORD count;
};

struct PMSG_CASTLE_SIEGE_NPC_LIST
{
	DWORD number;
	DWORD index;
	DWORD deflevel;
	DWORD regenlevel;
	DWORD maxhp;
	DWORD hp;
	BYTE x;
	BYTE y;
	BYTE live;
};

struct PMSG_CASTLE_SIEGE_MINI_MAP_SINGLE_SEND
{
	PWMSG_HEAD header; // C1:B6
	DWORD count;
};

struct PMSG_CASTLE_SIEGE_MINI_MAP_SINGLE
{
	BYTE x;
	BYTE y;
};

struct PMSG_CASTLE_SIEGE_MINI_MAP_SEND
{
	PWMSG_HEAD header; // C1:BB
	DWORD count;
};

struct PMSG_CASTLE_SIEGE_MINI_MAP
{
	BYTE npc;
	BYTE x;
	BYTE y;
};

//**********************************************//
//**********************************************//
//**********************************************//

struct CSP_REQ_NPCUPDATEDATA
{
	PWMSG_HEAD h;
	WORD wMapSvrNum;
	int iCount;
};

struct CSP_NPCUPDATEDATA
{
	int iNpcNumber;
	int iNpcIndex;
	int iNpcDfLevel;
	int iNpcRgLevel;
	int iNpcMaxHp;
	int iNpcHp;
	BYTE btNpcX;
	BYTE btNpcY;
	BYTE btNpcDIR;
};

struct CSP_REQ_NPCSAVEDATA
{
	PWMSG_HEAD h;
	WORD wMapSvrNum;
	int iCount;
};

struct CSP_NPCSAVEDATA
{
	int iNpcNumber;
	int iNpcIndex;
	int iNpcDfLevel;
	int iNpcRgLevel;
	int iNpcMaxHp;
	int iNpcHp;
	BYTE btNpcX;
	BYTE btNpcY;
	BYTE btNpcDIR;
};

struct CSP_REQ_CSGUILDUNIONINFO
{
    PWMSG_HEAD h;
    WORD wMapSvrNum;
    int iCount;
};

struct CSP_REQ_CSSAVETOTALGUILDINFO
{
	PWMSG_HEAD h;
	WORD wMapSvrNum;
	int iCount;
};

struct CSP_CSSAVETOTALGUILDINFO
{
	char szGuildName[8];
	int iCsGuildID;
	int iGuildInvolved;
	#if(GAMESERVER_UPDATE>=205)
	int iGuildScore; //season 2.5 add-on
	#endif
};

//**********************************************//
//**********************************************//
//**********************************************//

struct _CS_SCHEDULE_DATA
{
	_CS_SCHEDULE_DATA() // OK
	{
		this->Clear();
	}

	virtual ~_CS_SCHEDULE_DATA() // OK
	{

	}

	void Clear() // OK
	{
		this->m_bIN_USE = 0;
		this->m_iSTATE = -1;
		this->m_iADD_DAY = -1;
		this->m_iADD_HOUR = -1; 
		this->m_iADD_MIN = -1;
		this->m_iGAP_SEC = 0;
	}

	int m_bIN_USE; // 4
	int m_iSTATE; // 8
	int m_iADD_DAY; // C
	int m_iADD_HOUR; // 10
	int m_iADD_MIN; // 14
	int m_iGAP_SEC; // 18
};

struct _CS_NPC_DATA
{
	_CS_NPC_DATA() // OK
	{
		this->Clear();
		this->m_iCS_GATE_LEVER_INDEX = -1;
	}

	virtual ~_CS_NPC_DATA() // OK
	{

	}

	void Clear() // OK
	{
		this->m_bIN_USE = 0;
		this->m_iNPC_NUM = -1;
		this->m_iNPC_INDEX = -1;
		this->m_iNPC_LIVE = 0;
		this->m_iNPC_AUTH = -1;
		this->m_bNPC_DBSAVE = 0;
		this->m_iNPC_SIDE = 0;
		this->m_iNPC_OBJINDEX = -1;
		this->m_iNPC_DF_LEVEL = 0;
		this->m_iNPC_RG_LEVEL = 0;
		this->m_iNPC_MAXHP = 0;
		this->m_iNPC_HP = 0;
		this->m_iNPC_SX = -1;
		this->m_iNPC_SY = -1;
		this->m_iNPC_DX = -1;
		this->m_iNPC_DY = -1;
		this->m_iNPC_DIR = -1;
		this->m_iNPC_BASE_DF_LEVEL = 0;
		this->m_iNPC_BASE_RG_LEVEL = 0;
		this->m_iNPC_BASE_MAXHP = 0;
		this->m_iNPC_BASE_HP = 0;
		this->m_iNPC_BASE_SX = -1;
		this->m_iNPC_BASE_SY = -1;
		this->m_iNPC_BASE_DX = -1;
		this->m_iNPC_BASE_DY = -1;
		this->m_iNPC_BASE_DIR = -1;
		this->m_iCsNpcExistVal = 0;
	}

	void SetBaseValue() // OK
	{
		this->m_iNPC_DF_LEVEL = this->m_iNPC_BASE_DF_LEVEL;
		this->m_iNPC_RG_LEVEL = this->m_iNPC_BASE_RG_LEVEL;
		this->m_iNPC_MAXHP = this->m_iNPC_BASE_MAXHP;
		this->m_iNPC_HP = this->m_iNPC_BASE_HP;
		this->m_iNPC_SX = this->m_iNPC_BASE_SX;
		this->m_iNPC_SY = this->m_iNPC_BASE_SY;
		this->m_iNPC_DX = this->m_iNPC_BASE_DX;
		this->m_iNPC_DY = this->m_iNPC_BASE_DY;
		this->m_iNPC_DIR = this->m_iNPC_BASE_DIR;
	}

	int m_bIN_USE; // 4
	int m_iNPC_NUM; // 8
	int m_iNPC_INDEX; // C
	int m_bNPC_DBSAVE; // 10
	int m_iNPC_SIDE; // 14
	int m_iNPC_OBJINDEX; // 18
	int m_iNPC_DF_LEVEL; // 1C
	int m_iNPC_RG_LEVEL; // 20
	int m_iNPC_MAXHP; // 24
	int m_iNPC_HP; // 28
	int m_iNPC_SX; // 2C
	int m_iNPC_SY; // 30
	int m_iNPC_DX; // 34
	int m_iNPC_DY; // 38
	int m_iNPC_DIR; // 3C
	int m_iNPC_BASE_DF_LEVEL; // 40
	int m_iNPC_BASE_RG_LEVEL; // 44
	int m_iNPC_BASE_MAXHP; // 48
	int m_iNPC_BASE_HP; // 4C
	int m_iNPC_BASE_SX; // 50
	int m_iNPC_BASE_SY; // 54
	int m_iNPC_BASE_DX; // 58
	int m_iNPC_BASE_DY; // 5C
	int m_iNPC_BASE_DIR; // 60
	int m_iCS_GATE_LEVER_INDEX; // 64
	int m_iNPC_LIVE; // 68
	int m_iNPC_AUTH; // 6C

	union
	{
		struct
		{
			BYTE m_btCsNpcExistVal1; // 70
			BYTE m_btCsNpcExistVal2; // 71
			BYTE m_btCsNpcExistVal3; // 72
			BYTE m_btCsNpcExistVal4; // 73
		};

		int m_iCsNpcExistVal; // 74
	};
};

struct _CS_REG_GUILD_DATA
{
	_CS_REG_GUILD_DATA() // OK
	{
		this->Clear();
	}

	virtual ~_CS_REG_GUILD_DATA() // OK
	{

	}

	void Clear() // OK
	{
		this->m_bIN_USE = 0;
		this->m_strGuildName = "";
		this->m_iRegMarkCount = 0;
		this->m_iGuildMemberCount = 0;
		this->m_iGuildMasterLevel = 0;
		this->m_iSeqNum = 0;
		this->m_i64TotolScore = 0;
	}

	int m_bIN_USE; // 4
	std::basic_string<char> m_strGuildName; // 8
	int m_iRegMarkCount; // C
	int m_iGuildMemberCount; // 10
	int m_iGuildMasterLevel; // 14
	int m_iSeqNum; // 18
	__int64 m_i64TotolScore; // 1C
};

struct _CS_TOTAL_GUILD_DATA
{
	_CS_TOTAL_GUILD_DATA() // OK
	{
		this->Clear();
	}

	virtual ~_CS_TOTAL_GUILD_DATA() // OK
	{

	}

	void Clear() // OK
	{
		this->m_bIN_USE = 0;
		this->m_iCsGuildID = 0;
		this->m_bGuildInvolved = 0;
		this->m_iGuildScore = 0;
	}

	int m_bIN_USE; // 4
	int m_iCsGuildID; // 8
	int m_bGuildInvolved; // C
	int m_iGuildScore; // 10
};

struct _CS_MINIMAP_DATA
{
	_CS_MINIMAP_DATA() // OK
	{
		this->Clear();
	}

	virtual ~_CS_MINIMAP_DATA() // OK
	{

	}

	void Clear() // OK
	{
		this->m_iGuildMasterIndex = -1;
		memset(this->m_szGuildName,0,sizeof(this->m_szGuildName));
		memset(this->m_stMiniMapPoint,0,sizeof(this->m_stMiniMapPoint));
		this->m_iMiniMapPointCount = 0;
	}

	int m_iGuildMasterIndex; // 4
	char m_szGuildName[16]; // 8
	int m_iMiniMapPointCount; // 18
	PMSG_CASTLE_SIEGE_MINI_MAP_SINGLE m_stMiniMapPoint[1000]; // 1C
};

class CCastleSiege
{
public:
	CCastleSiege();
	virtual ~CCastleSiege();
	void Clear();
	int Ready(int iMapSvrGroup);
	int LoadPreFixData(char*lpszFileName);
	int LoadData(char*lpszFileName);
	int DataRequest();
	int Init();
	void MainProc();
	static BYTE ScheduleStateCompFunc(_CS_SCHEDULE_DATA & A,_CS_SCHEDULE_DATA & B);
	int GetDataLoadState(){return m_iCastleDataLoadState;}
	void SetDataLoadState(int iDataLoadState){m_iCastleDataLoadState = iDataLoadState;}
	void SetDbDataLoadOK(BOOL bDbDataLoadOK){m_bDbDataLoadOK = bDbDataLoadOK;}
	int FirstCreateDbNPC();
	int SetCastleInitData(CSP_ANS_CSINITDATA*lpMsg);
	int SetCastleNpcData(CSP_CSINITDATA*lpMsg,int iCOUNT);
	int CheckSync();
	void GetNextDay(SYSTEMTIME*st,int iAfterDay,int iAfterHour,int iAfterMin,int iAfterSec);
	int GetStateGapSec(int iCastleSiegeState);
	void SetState(int iCastleSiegeState,BOOL bSetRemainMsec);
	void SetState_NONE(BOOL bSetRemainMsec);
	void SetState_IDLE_1(BOOL bSetRemainMsec);
	void SetState_REGSIEGE(BOOL bSetRemainMsec);
	void SetState_IDLE_2(BOOL bSetRemainMsec);
	void SetState_REGMARK(BOOL bSetRemainMsec);
	void SetState_IDLE_3(BOOL bSetRemainMsec);
	void SetState_NOTIFY(BOOL bSetRemainMsec);
	void SetState_READYSIEGE(BOOL bSetRemainMsec);
	void SetState_STARTSIEGE(BOOL bSetRemainMsec);
	void SetState_ENDSIEGE(BOOL bSetRemainMsec);
	void SetState_ENDCYCLE(BOOL bSetRemainMsec);
	void ProcState_NONE();
	void ProcState_IDLE_1();
	void ProcState_REGSIEGE();
	void ProcState_IDLE_2();
	void ProcState_REGMARK();
	void ProcState_IDLE_3();
	void ProcState_NOTIFY();
	void ProcState_READYSIEGE();
	void ProcState_STARTSIEGE();
	void ProcState_ENDSIEGE();
	void ProcState_ENDCYCLE();
	int GetCastleState(){return m_iCastleSiegeState;}
	SYSTEMTIME GetCastleLeftSiegeDate() {return m_tmLeftCastleSiegeDate;}
	int CalcCastleLeftSiegeDate();
	int GetCastleStateTerm(SYSTEMTIME*tmStateStartDate,SYSTEMTIME*tmStateEndDate);
	int GetCurRemainSec();
	void ClearDbNPC();
	void ClearNonDbNPC();
	void ClearNonDbNPC_MidWin();
	void ClearAllNPC();
	void CreateDbNPC();
	void CreateNonDbNPC(BOOL bDelFirst);
	int CheckAddDbNPC(int iIndex,int iNpcType,int iNpcIndex,BYTE & btResult);
	int AddDbNPC(int iNpcType,int iNpcIndex);
	int RepairDbNPC(int iNpcType,int iNpcIndex,int iNpcHP,int iNpcMaxHP);
	int PayForUpgradeDbNpc(int iIndex,int iNpcType,int iNpcIndex,int iNpcUpType,int iNpcUpValue,int iNpcUpIndex);
	int UpgradeDbNPC(int iIndex,int iNpcType,int iNpcIndex,int iNpcUpType,int iNpcUpValue,int iNpcUpIndex);
	int UpgradeDbNPC_DFLEVEL(int iNpcType,int iNpcIndex,int iNpcDfLevel);
	int UpgradeDbNPC_RGLEVEL(int iNpcType,int  iNpcIndex,int iNpcRgLevel);
	int UpgradeDbNPC_MAXHP(int iNpcType,int iNpcIndex,int iNpcMaxHP);
	void StoreDbNpc();
	int DelNPC(int iIndex,int iNpcType,int iMonsterExistVal,BOOL bDbSave);
	void SendNpcStateList(int iIndex,int iNpcType);
	int GetNpcData(int iNpcType,int iNpcIndex,_CS_NPC_DATA & pRetNpcData);
	void AdjustDbNpcLevel();
	void SendCsGateStateViewPort(int iGateIndex,BYTE btOperate);
	void CreateAllCsGateLever();
	int CreateCsGateLever(int iX,int iY);
	int LinkCsGateLever(int iLeverIndex,int iGateIndex);
	int CheckLeverAlive(int iLeverIndex);
	int CheckCsGateAlive(int iGateIndex);
	void CheckCsDbNpcAlive();
	void SetCrownIndex(int iIndex){m_iCrownIndex = iIndex;}
	void NotifyCrownState(BYTE btState);
	BOOL GetRegCrownAvailable(){return m_bRegCrownAvailable;}
	void SetRegCrownAvailable(BOOL bRegCrownAvailable){m_bRegCrownAvailable = bRegCrownAvailable;}
	int GetCastleTowerAccessable(){return m_bCastleTowerAccessable;}
	void SetCastleTowerAccessable(BOOL bTowerAccessable){m_bCastleTowerAccessable = bTowerAccessable;}
	int DelGemOfDefend(int iIndex,int iNeedGemOfDefend);
	void SetTaxRate(int iTaxType,int iTaxRate);
	int CheckCastleHasMoney(__int64 i64CastleMoney);
	__int64 GetCastleMoney() { return m_i64CastleMoney; }
	void SetCastleMoney(__int64 i64CastleMoney);
	void ResetCastleTaxInfo();
	void SetIsSiegeGuildList(BOOL bIsSiegGuildList){m_btIsSiegeGuildList = bIsSiegGuildList;}
	void SendCastleStateSync();
	BOOL GetDbNpcCreated(){return m_bDbNpcCreated;}
	void SetDbNpcCreated(BOOL bDbNpcCreated){m_bDbNpcCreated = bDbNpcCreated;}
	int CheckGuildOwnCastle(char*lpszGuildName);
	void ResetCastleCycle();
	void SetGateBlockState(int iX,int iY,int iGateState);
	char*GetCastleOwnerGuild(){return m_szCastleOwnerGuild;}
	BOOL GetHuntZoneEnter(){return m_bEnterHuntZone;}
	void SetHuntZoneEnter(BOOL bEnterHuntZone){m_bEnterHuntZone = bEnterHuntZone;}
	void CheckBuildCsGuildInfo();
	void SetCalcRegGuildList(CSP_CALCREGGUILDLIST*lpMsg,int iCOUNT);
	void MakeCsTotalGuildInfo(CSP_CSGUILDUNIONINFO*lpMsg,int iCOUNT);
	void SaveCsTotalGuildInfo();
	void SetCsTotalGuildInfo(CSP_CSLOADTOTALGUILDINFO*lpMsg,int iCOUNT);
	int GetCsJoinSide(char*lpszGuildName,BYTE*btCsJoinSide,bool*bCsGuildInvolved);
	void SetAllUserCsJoinSide();
	void ResetAllUserCsJoinSide();
	void NotifySelfCsJoinSide(int iIndex);
	int OperateGate(int iNpcIndex,BOOL bOpenType);
	int OperateGate(int iObjIndex,int iMonsterExistVal,BOOL bOpenType);
	int CheckAttackGuildExist();
	int CheckCastleOwnerMember(int iIndex);
	int CheckCastleOwnerUnionMember(int iIndex);
	int CheckGuardianStatueExist();
	void ChangeWinnerGuild(int iCsJoinSide);
	int CheckMiddleWinnerGuild();
	int CheckCastleSiegeResult();
	int CheckUnionGuildMaster(int iIndex);
	void SetCrownUserIndex(int iIndex){this->m_iCastleCrownAccessUser = iIndex;}
	int GetCrownUserIndex(){return m_iCastleCrownAccessUser;}
	void ResetCrownUserIndex(){m_iCastleCrownAccessUser=-1;}
	int GetCrownAccessUserX(){return m_btCastleCrownAccessUserX;}
	int GetCrownAccessUserY(){return m_btCastleCrownAccessUserY;}
	void SetCrownAccessUserX(BYTE iX){m_btCastleCrownAccessUserX = iX;}
	void SetCrownAccessUserY(BYTE iY){m_btCastleCrownAccessUserY = iY;}
	void SetCrownSwitchUserIndex(int iMonsterClass,int iIndex);
	int GetCrownSwitchUserIndex(int iMonsterClass);
	void ResetCrownSwitchUserIndex(int iMonsterClass);
	void SetCrownAccessTickCount();
	void ResetCrownAccessTickCount();
	int CheckOverlapCsMarks(int iIndex);
	int GetCsAttkGuildList(PMSG_CSATTKGUILDLIST*lpMsgBody,int & iCount);
	void CheckTeleportMagicAxisY(int x,int y,BYTE* ty);
	void NotifyCsSelfLeftTime(int iIndex);
	void CheckReviveNonDbNPC();
	void CheckReviveGuardianStatue();
	void ReSpawnAllUser();
	void ReSpawnEnemyUser(int bRefreshOwnerUser);
	void NotifyAllUserCsStartState(BYTE btStartState);
	void NotifyAllUserCsProgState(BYTE btProgState,char*lpszGuildName);
	void ClearCastleTowerBarrier();
	void SetAllCastleGateState(BOOL bOpenType);
	void AddMiniMapDataReqUser(int iIndex);
	void DelMiniMapDataReqUser(int iIndex);
	void OperateMiniMapWork();
	void SendMapServerGroupMsg(char* lpszMsg);
	void SendAllUserAnyData(LPBYTE lpMsg,int iSize);
	void SendAllUserAnyMsg(char* lpMsg,int iSize);
	void SendCsUserAnyData(LPBYTE lpMsg,int iSize);
	void SendCsUserAnyMsg(char*lpszMsg);
	static BYTE RegGuildScoreCompFunc(_CS_REG_GUILD_DATA & A,_CS_REG_GUILD_DATA & B);
	void CreateDbNPC_INS();
	DWORD GetCrownAccessTickCount(){return m_dwCrownAccessTime;}
	void NotifyCrownSwitchInfo(int iIndex);
	void GetStateDate(int iCastleSiegeState,SYSTEMTIME* st);
public:
	int m_bCastleSiegeEnable; // 8
	int m_bFileDataLoadOK; // C
	int m_bDbDataLoadOK; // 10
	int m_bDoRun; // 14
	int m_iMapSvrGroup; // 18
	int m_iCastleDataLoadState; // 1C
	DWORD m_dwDbDataLoadTickCount; // 20
	int m_iCastleSiegeCycle; // 24
	int m_iCastleSiegeState; // 28
	SYSTEMTIME m_tmSiegeEndSchedule; // 2C
	int m_bFixCastleCycleStartDate; // 3C
	SYSTEMTIME m_tmFixCastleCycleStartDate; // 40
	int m_iFixCastleSpecificState; // 50
	int m_bFixCastleStateStartDate; // 54
	SYSTEMTIME m_tmFixCastleStateStartDate; // 58
	int m_iCS_REMAIN_MSEC; // 68
	int m_iCS_TARGET_MSEC;
	int m_iCS_TICK_COUNT; // 6C
	SYSTEMTIME m_tmStateStartDate; // 70
	SYSTEMTIME m_tmLeftCastleSiegeDate; // 80
	DWORD m_dwNPC_DBSAVE_TICK_COUNT; // 90
	DWORD m_dwCALC_LEFTSIEGE_TICK_COUNT; // 94
	DWORD m_dwEVENT_MSG_TICK_COUNT; // 98
	DWORD m_dwCHECK_GATE_ALIVE_COUNT; // 9C
	DWORD m_dwCS_JOINSIDE_REFRESH_TICK_COUNT; // A0
	DWORD m_dwCS_JOINSIDE_REFRESH_TOTAL_COUNT; // A4
	DWORD m_dwCS_MINIMAP_SEND_TICK_COUNT; // A8
	DWORD m_dwCS_LEFTTIME_TICK_COUNT; // AC
	DWORD m_dwCS_STARTTIME_TICK_COUNT; // B0
	SYSTEMTIME m_tmStartDate; // B4
	SYSTEMTIME m_tmEndDate; // C4
	BYTE m_btIsSiegeGuildList; // D4
	BYTE m_btIsSiegeEnded; // D5
	BYTE m_btIsCastleOccupied; // D6
	char m_szCastleOwnerGuild[16]; // D7
	__int64 m_i64CastleMoney; // E8
	int m_iTaxRateChaos; // F0
	int m_iTaxRateStore; // F4
	int m_iTaxHuntZone; // F8
	int m_bEnterHuntZone; // FC
	std::vector<_CS_SCHEDULE_DATA> m_vtScheduleData; // 100
	std::vector<_CS_NPC_DATA> m_vtNpcData; // 128
	int m_bDbNpcCreated; // 150
	std::map<std::string,_CS_TOTAL_GUILD_DATA> m_mapCsBasicGuildInfo; // 154
	std::map<std::string,_CS_TOTAL_GUILD_DATA> m_mapCsTotalGuildInfo; // 17C
	int m_bCsBasicGuildInfoLoadOK; // 1A4
	int m_bCsTotalGuildInfoLoadOK; // 1A8
	std::map<int,_CS_MINIMAP_DATA> m_mapMiniMapData; // 1AC
	std::vector<int> m_vtMiniMapReqUser; // 1D4
	int m_bCastleTowerAccessable; // 1E4
	int m_bRegCrownAvailable; // 1E8
	int m_iCrownIndex; // 1EC
	int m_iCastleCrownAccessUser; // 1F0
	BYTE m_btCastleCrownAccessUserX; // 1F4
	BYTE m_btCastleCrownAccessUserY; // 1F5
	int m_iCastleSwitchAccessUser1; // 1F8
	int m_iCastleSwitchAccessUser2; // 1FC
	DWORD m_dwCrownAccessTime; // 200
	char m_szMiddleWinnerGuild[16]; // 204
};

extern CCastleSiege gCastleSiege;

static int g_iNpcUpDfLevel_CGATE[3][3] = {
	1,	2,	3000000,
	2,	3,	3000000,
	3,	4,	3000000
};

static int g_iNpcUpDfLevel_CSTATUE[3][3] = {
	1,	3,	3000000,
	2,	5,	3000000,
	3,	7,	3000000,
};

static int g_iNpcUpMaxHP_CGATE[0x3][3] = {
	#if(GAMESERVER_UPDATE>=401)
	2500000,	2,	1000000,
	3500000,	3,	1000000,
	5200000,	4,	1000000,
	#else
	800000,		2,	1000000,
	1500000,	3,	1000000,
	3000000,	4,	1000000,
	#endif
};

static int g_iNpcUpMaxHP_CSTATUE[3][3] = {
	#if(GAMESERVER_UPDATE>=401)
	2200000,	3,	1000000,
	3400000,	5,	1000000,
	5000000,	7,	1000000
	#else
	750000,		3,	1000000,
	1300000,	5,	1000000,
	2500000,	7,	1000000
	#endif
};

static int g_iNpcUpRgLevel_CSTATUE[3][3] = {
	1,	3,	5000000,
	2,	5,	5000000,
	3,	7,	5000000
};

static int g_iCsUserReSpawnArea[3][4] = {
	35,		17,		144,	48,
	74,		144,	115,	154,
	35,		17,		144,	48
};

static int g_iNpcDefense_CGATE[4] = {
	100,180,300,520
};
static int g_iNpcDefense_CSTATUE[4] = {
	80,180,340,550
};