#pragma once

#if(GAMESERVER_UPDATE>=701)
#include "..\\..\\..\\Util\\mapm\\M_APM.h"
#endif
#include "User.h"

#if(GAMESERVER_LANGUAGE==0)
#define PROTOCOL_NAME "KOR"
#define PROTOCOL_CODE1 0xD3
#define PROTOCOL_CODE2 0xD7
#define PROTOCOL_CODE3 0xDF
#define PROTOCOL_CODE4 0x10
#elif(GAMESERVER_LANGUAGE==1)
#define PROTOCOL_NAME "ENG"
#define PROTOCOL_CODE1 0xD4
#define PROTOCOL_CODE2 0x11
#define PROTOCOL_CODE3 0x15
#define PROTOCOL_CODE4 0xDB
#elif(GAMESERVER_LANGUAGE==2)
#define PROTOCOL_NAME "JPN"
#define PROTOCOL_CODE1 0x1D
#define PROTOCOL_CODE2 0xDC
#define PROTOCOL_CODE3 0xD6
#define PROTOCOL_CODE4 0xD7
#elif(GAMESERVER_LANGUAGE==3)
#define PROTOCOL_NAME "CHS"
#define PROTOCOL_CODE1 0xD7
#define PROTOCOL_CODE2 0xD9
#define PROTOCOL_CODE3 0xD2
#define PROTOCOL_CODE4 0x1D
#elif(GAMESERVER_LANGUAGE==4) 
#define PROTOCOL_NAME "TAI"
#define PROTOCOL_CODE1 0xD6
#define PROTOCOL_CODE2 0xDD
#define PROTOCOL_CODE3 0xDF
#define PROTOCOL_CODE4 0xD2
#elif(GAMESERVER_LANGUAGE==5) 
#define PROTOCOL_NAME "PHI"
#define PROTOCOL_CODE1 0xDD
#define PROTOCOL_CODE2 0xD6
#define PROTOCOL_CODE3 0xDF
#define PROTOCOL_CODE4 0x11
#endif

#define SET_NUMBERHB(x) ((BYTE)((DWORD)(x)>>(DWORD)8))
#define SET_NUMBERLB(x) ((BYTE)((DWORD)(x)&0xFF))
#define SET_NUMBERHW(x) ((WORD)((DWORD)(x)>>(DWORD)16))
#define SET_NUMBERLW(x) ((WORD)((DWORD)(x)&0xFFFF))
#define SET_NUMBERHDW(x) ((DWORD)((QWORD)(x)>>(QWORD)32))
#define SET_NUMBERLDW(x) ((DWORD)((QWORD)(x)&0xFFFFFFFF))

#define MAKE_NUMBERW(x,y) ((WORD)(((BYTE)((y)&0xFF))|((BYTE)((x)&0xFF)<<8)))
#define MAKE_NUMBERDW(x,y) ((DWORD)(((WORD)((y)&0xFFFF))|((WORD)((x)&0xFFFF)<<16)))
#define MAKE_NUMBERQW(x,y) ((QWORD)(((DWORD)((y)&0xFFFFFFFF))|((DWORD)((x)&0xFFFFFFFF)<<32)))

#define MAKE_PACKET(x) (BYTE*)&x,x.header.size

//**********************************************//
//************ Packet Base *********************//
//**********************************************//

struct PBMSG_HEAD
{
	void set(BYTE head,BYTE size) // OK
	{
		this->type = 0xC1;
		this->size = size;
		this->head = head;
	}

	void setE(BYTE head,BYTE size) // OK
	{
		this->type = 0xC3;
		this->size = size;
		this->head = head;
	}

	BYTE type;
	BYTE size;
	BYTE head;
};

struct PSBMSG_HEAD
{
	void set(BYTE head,BYTE subh,BYTE size) // OK
	{
		this->type = 0xC1;
		this->size = size;
		this->head = head;
		this->subh = subh;
	}

	void setE(BYTE head,BYTE subh,BYTE size) // OK
	{
		this->type = 0xC3;
		this->size = size;
		this->head = head;
		this->subh = subh;
	}

	BYTE type;
	BYTE size;
	BYTE head;
	BYTE subh;
};

struct PWMSG_HEAD
{
	void set(BYTE head,WORD size) // OK
	{
		this->type = 0xC2;
		this->size[0] = SET_NUMBERHB(size);
		this->size[1] = SET_NUMBERLB(size);
		this->head = head;
	}

	void setE(BYTE head,WORD size) // OK
	{
		this->type = 0xC4;
		this->size[0] = SET_NUMBERHB(size);
		this->size[1] = SET_NUMBERLB(size);
		this->head = head;
	}

	BYTE type;
	BYTE size[2];
	BYTE head;
};

struct PSWMSG_HEAD
{
	void set(BYTE head,BYTE subh,WORD size) // OK
	{
		this->type = 0xC2;
		this->size[0] = SET_NUMBERHB(size);
		this->size[1] = SET_NUMBERLB(size);
		this->head = head;
		this->subh = subh;
	}

	void setE(BYTE head,BYTE subh,WORD size) // OK
	{
		this->type = 0xC4;
		this->size[0] = SET_NUMBERHB(size);
		this->size[1] = SET_NUMBERLB(size);
		this->head = head;
		this->subh = subh;
	}

	BYTE type;
	BYTE size[2];
	BYTE head;
	BYTE subh;
};

//**********************************************//
//************ Client -> GameServer ************//
//**********************************************//

struct PMSG_CHAT_RECV
{
	PBMSG_HEAD header; // C1:00
	char name[10];
	char message[60];
};

struct PMSG_CHAT_WHISPER_RECV
{
	PBMSG_HEAD header; // C1:02
	char name[10];
	char message[60];
};

struct PMSG_MAIN_CHECK_RECV
{
	PBMSG_HEAD header; // C1:03
	DWORD key;
};

struct PMSG_LIVE_CLIENT_RECV
{
	PBMSG_HEAD header; // C1:0E
	DWORD TickCount;
	WORD PhysiSpeed;
	WORD MagicSpeed;
};

struct PMSG_POSITION_RECV
{
	PBMSG_HEAD header; // C1:[PROTOCOL_CODE3]
	BYTE x;
	BYTE y;
};

struct PMSG_ACTION_RECV
{
	PBMSG_HEAD header; // C1:18
	BYTE dir;
	BYTE action;
	BYTE index[2];
};

struct PMSG_EVENT_REMAIN_TIME_RECV
{
	PBMSG_HEAD header; // C1:91
	BYTE EventType;
	BYTE ItemLevel;
};

struct PMSG_PET_ITEM_COMMAND_RECV
{
	PBMSG_HEAD header; // C1:A7
	BYTE type;
	BYTE command;
	BYTE index[2];
};

struct PMSG_PET_ITEM_INFO_RECV
{
	PBMSG_HEAD header; // C1:A9
	BYTE type;
	BYTE flag;
	BYTE slot;
};

struct PMSG_MAP_SERVER_MOVE_AUTH_RECV
{
	PSBMSG_HEAD header; // C1:B1:01
	char account[12];
	char name[12];
	DWORD AuthCode1;
	DWORD AuthCode2;
	DWORD AuthCode3;
	DWORD AuthCode4;
	DWORD TickCount;
	BYTE ClientVersion[5];
	BYTE ClientSerial[16];
};

struct PMSG_FRIEND_MESSAGE_RECV
{
	PWMSG_HEAD header; // C1:C5
	DWORD guid;
	char name[10];
	char subject[60];
	BYTE dir;
	BYTE action;
	WORD size;
	char text[1000];
};

struct PMSG_MOVE_RECV
{
	PBMSG_HEAD header; // C1:[PROTOCOL_CODE1]
	BYTE x;
	BYTE y;
	BYTE path[8];
};

struct PMSG_CONNECT_ACCOUNT_RECV
{
	#pragma pack(1)
	PSBMSG_HEAD header; // C3:F1:01
	char account[10];
	#if(GAMESERVER_UPDATE>=406)
	char password[20];
	#elif(GAMESERVER_UPDATE>=401)
	char password[12];
	#else
	char password[10];
	#endif
	DWORD TickCount;
	BYTE ClientVersion[5];
	BYTE ClientSerial[16];
	#if(GAMESERVER_UPDATE>=803)
	BYTE LanguageCode;
	#endif
	#pragma pack()
};

struct PMSG_CLOSE_CLIENT_RECV
{
	PSBMSG_HEAD header; // C1:F1:02
	BYTE type;
};

struct PMSG_CHARACTER_CREATE_RECV
{
	PSBMSG_HEAD header; // C1:F3:01
	char name[10];
	BYTE Class;
};

struct PMSG_CHARACTER_DELETE_RECV
{
	PSBMSG_HEAD header; // C1:F3:02
	char name[10];
	char PersonalCode[10];
};

struct PMSG_CHARACTER_INFO_RECV
{
	PSBMSG_HEAD header; // C1:F3:03
	char name[10];
};

struct PMSG_LEVEL_UP_POINT_RECV
{
	PSBMSG_HEAD header; // C1:F3:06
	BYTE type;
};

struct PMSG_CHARACTER_NAME_CHECK_RECV
{
	PSBMSG_HEAD header; // C1:F3:15
	char name[10];
};

struct PMSG_CHARACTER_NAME_CHANGE_RECV
{
	PSBMSG_HEAD header; // C1:F3:16
	char OldName[10];
	char NewName[10];
};

struct PMSG_OPTION_CHANGE_SKIN_RECV
{
	PSBMSG_HEAD header; // C1:F3:21
	BYTE ChangeSkin;
};

struct PMSG_OPTION_DATA_RECV
{
	#pragma pack(1)
	PSBMSG_HEAD header; // C1:F3:30
	#if(GAMESERVER_UPDATE>=401)
	BYTE SkillKey[20];
	#else
	BYTE SkillKey[10];
	#endif
	BYTE GameOption;
	BYTE QKey;
	BYTE WKey;
	BYTE EKey;
	BYTE ChatWindow;
	#if(GAMESERVER_UPDATE>=401)
	BYTE RKey;
	DWORD QWERLevel;
	#endif
	#pragma pack()
};

struct PMSG_CLIENT_SECURITY_BREACH_RECV
{
	PSBMSG_HEAD header; // C1:F3:31
	BYTE code[4];
};

struct PMSG_SNS_DATA_RECV
{
	PWMSG_HEAD header; // C1:FB
	BYTE result;
	BYTE data[256];
};

struct PMSG_SNS_DATA_LOG_RECV
{
	PBMSG_HEAD header; // C1:FC
	BYTE code[3];
};

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_CHAT_SEND
{
	PBMSG_HEAD header; // C1:00
	char name[10];
	char message[60];
};

struct PMSG_CHAT_TARGET_SEND
{
	PBMSG_HEAD header; // C1:01
	BYTE index[2];
	char message[60];
};

struct PMSG_CHAT_WHISPER_SEND
{
	PBMSG_HEAD header; // C1:02
	char name[10];
	char message[60];
};

struct PMSG_MAIN_CHECK_SEND
{
	PBMSG_HEAD header; // C1:03
	WORD key;
};

struct PMSG_EVENT_STATE_SEND
{
	PBMSG_HEAD header; // C1:0B
	BYTE state;
	BYTE event;
};

struct PMSG_SERVER_MSG_SEND
{
	PBMSG_HEAD header; // C1:0C
	BYTE MsgNumber;
};

struct PMSG_WEATHER_SEND
{
	PBMSG_HEAD header; // C1:0F
	BYTE weather;
};

struct PMSG_DAMAGE_SEND
{
	PBMSG_HEAD header; // C1:[PROTOCOL_CODE2]
	BYTE index[2];
	BYTE damage[2];
	#if(GAMESERVER_UPDATE>=701)
	BYTE type[2];
	#else
	BYTE type;
	#endif
	#if(GAMESERVER_UPDATE>=201)
	BYTE ShieldDamage[2];
	#endif
	#if(GAMESERVER_UPDATE>=701)
	BYTE attribute;
	#endif
	#if(GAMESERVER_EXTRA==1)
	DWORD ViewCurHP;
	#if(GAMESERVER_UPDATE>=201)
	DWORD ViewCurSD;
	#endif
	DWORD ViewDamageHP;
	#if(GAMESERVER_UPDATE>=201)
	DWORD ViewDamageSD;
	#endif
	#endif
};

struct PMSG_POSITION_SEND
{
	PBMSG_HEAD header; // C1:[PROTOCOL_CODE3]
	BYTE index[2];
	BYTE x;
	BYTE y;
};

struct PMSG_MONSTER_DIE_SEND
{
	PBMSG_HEAD header; // C1:16
	BYTE index[2];
	BYTE experience[2];
	BYTE damage[2];
	#if(GAMESERVER_EXTRA==1)
	DWORD ViewDamageHP;
	#endif
};

struct PMSG_USER_DIE_SEND
{
	PBMSG_HEAD header; // C1:17
	BYTE index[2];
	#if(GAMESERVER_UPDATE>=401)
	BYTE skill[2];
	#else
	BYTE skill;
	#endif
	BYTE killer[2];
};

struct PMSG_ACTION_SEND
{
	PBMSG_HEAD header; // C1:18
	BYTE index[2];
	BYTE dir;
	BYTE action;
	BYTE target[2];
};

struct PMSG_LIFE_SEND
{
	PBMSG_HEAD header; // C1:26
	BYTE type;
	BYTE life[2];
	BYTE flag;
	#if(GAMESERVER_UPDATE>=201)
	BYTE shield[2];
	#endif
	#if(GAMESERVER_EXTRA==1)
	DWORD ViewHP;
	#if(GAMESERVER_UPDATE>=201)
	DWORD ViewSD;
	#endif
	#endif
};

struct PMSG_MANA_SEND
{
	PBMSG_HEAD header; // C1:27
	BYTE type;
	BYTE mana[2];
	BYTE bp[2];
	#if(GAMESERVER_EXTRA==1)
	DWORD ViewMP;
	DWORD ViewBP;
	#endif
};

struct PMSG_ITEM_SPECIAL_TIME_SEND
{
	PBMSG_HEAD header; // C3:29
	BYTE number;
	WORD time;
};

struct PMSG_MAP_ATTR_SEND
{
	PBMSG_HEAD header; // C1:46
	BYTE type;
	BYTE attr;
	BYTE flag;
	BYTE count;
};

struct PMSG_MAP_ATTR
{
	BYTE x;
	BYTE y;
	BYTE tx;
	BYTE ty;
};

struct PMSG_PARTY_ITEM_INFO_SEND
{
	PBMSG_HEAD header; // C1:47
	BYTE index[2];
	DWORD ItemInfo;
	BYTE level;
};

struct PMSG_EFFECT_INFO_SEND
{
	PBMSG_HEAD header; // C1:48
	BYTE index[2];
	BYTE effect;
};

struct PMSG_MONSTER_SKILL_SEND
{
	PBMSG_HEAD header; // C1:69
	#if(GAMESERVER_UPDATE>=401)
	BYTE skill[2];
	#else
	BYTE skill;
	#endif
	WORD index;
	WORD target;
};

struct PMSG_EVENT_REMAIN_TIME_SEND
{
	PBMSG_HEAD header; // C1:91
	BYTE EventType;
	BYTE RemainTimeH;
	BYTE EnteredUser;
	BYTE RemainTimeL;
};

struct PMSG_TIME_COUNT_SEND
{
	PBMSG_HEAD header; // C1:92
	BYTE type;
};

struct PMSG_REWARD_EXPERIENCE_SEND
{
	PBMSG_HEAD header; // C1:9C
	BYTE index[2];
	WORD experience[2];
	BYTE damage[2];
	#if(GAMESERVER_EXTRA==1)
	DWORD ViewDamageHP;
	DWORD ViewExperience;
	DWORD ViewNextExperience;
	#endif
};

struct PMSG_EVENT_ENTER_COUNT_SEND
{
	PBMSG_HEAD header; // C1:9F
	BYTE EventType;
	BYTE EnterCount;
};

struct PMSG_PET_ITEM_INFO_SEND
{
	PBMSG_HEAD header; // C1:A9
	BYTE type;
	BYTE flag;
	BYTE slot;
	BYTE level;
	UINT experience;
	#if(GAMESERVER_UPDATE>=401)
	BYTE durability;
	#endif
};

struct PMSG_MAP_SERVER_MOVE_SEND
{
	PSBMSG_HEAD header; // C1:B1:00
	char IpAddress[16];
	WORD ServerPort;
	WORD ServerCode;
	DWORD AuthCode1;
	DWORD AuthCode2;
	DWORD AuthCode3;
	DWORD AuthCode4;
	#if(GAMESERVER_EXTRA==1)
	WORD NextServerCode;
	#endif
};

struct PMSG_MAP_SERVER_MOVE_AUTH_SEND
{
	PSBMSG_HEAD header; // C1:B1:01
	BYTE result;
};

struct PMSG_TAX_INFO_SEND
{
	PSBMSG_HEAD header; // C1:B2:1A
	BYTE type;
	BYTE rate;
};

struct PMSG_KILL_COUNT_SEND
{
	PSBMSG_HEAD header; // C1:B8:01
	BYTE count;
};

struct PMSG_CREATION_STATE_SEND
{
	PSBMSG_HEAD header; // C1:B9:01
	BYTE index[2];
	BYTE state;
};

struct PMSG_MONSTER_AREA_SKILL_SEND
{
	PSBMSG_HEAD header; // C1:BD:0C
	BYTE MonsterClass[2];
	BYTE sx;
	BYTE sy;
	BYTE tx;
	BYTE ty;
};

struct PMSG_MOVE_SEND
{
	PBMSG_HEAD header; // C1:[PROTOCOL_CODE1]
	BYTE index[2];
	BYTE x;
	BYTE y;
	BYTE dir;
};

struct PMSG_ELEMENTAL_DAMAGE_SEND
{
	PBMSG_HEAD header; // C1:D8
	BYTE index[2];
	BYTE attribute;
	DWORD damage;
	#if(GAMESERVER_EXTRA==1)
	DWORD ViewCurHP;
	#if(GAMESERVER_UPDATE>=201)
	DWORD ViewCurSD;
	#endif
	#endif
};

struct PMSG_CHARACTER_CREATION_ENABLE_SEND
{
	PBMSG_HEAD header; // C1:DE
	BYTE flag;
	BYTE result;
	#if(GAMESERVER_EXTRA==1)
	WORD ExperienceMultiplierConstA;
	WORD ExperienceMultiplierConstB;
	WORD CharacterMaxLevel;
	WORD CharacterDeleteMaxLevel;
	#endif
};

struct PMSG_LIFE_UPDATE_SEND
{
	PSBMSG_HEAD header; // C1:EC:10
	BYTE index[2];
	BYTE MaxHP[4];
	BYTE CurHP[4];
};

struct PMSG_CHARACTER_ATTACK_SPEED_SEND
{
	PSBMSG_HEAD header; // C1:EC:30
	DWORD PhysiSpeed;
	DWORD MagicSpeed;
};

struct PMSG_ENTER_EVENT_MAP_ERROR_SEND
{
	PSBMSG_HEAD header; // C1:EF:09
	DWORD result;
};

struct PMSG_CONNECT_CLIENT_SEND
{
	PSBMSG_HEAD header; // C1:F1:00
	BYTE result;
	BYTE index[2];
	BYTE ClientVersion[5];
	#if(GAMESERVER_EXTRA==1)
	WORD ServerCode;
	#endif
};

struct PMSG_CONNECT_ACCOUNT_SEND
{
	PSBMSG_HEAD header; // C1:F1:01
	BYTE result;
};

struct PMSG_CLOSE_CLIENT_SEND
{
	PSBMSG_HEAD header; // C1:F1:02
	BYTE result;
};

struct PMSG_CHARACTER_LIST_SEND
{
	PSBMSG_HEAD header; // C1:F3:00
	BYTE ClassCode;
	BYTE MoveCnt;
	BYTE count;
	#if(GAMESERVER_UPDATE>=602)
	BYTE ExtWarehouse;
	#endif
};

struct PMSG_CHARACTER_LIST
{
	BYTE slot;
	char Name[10];
	WORD Level;
	BYTE CtlCode;
	BYTE CharSet[18];
	BYTE GuildStatus;
};

struct PMSG_CHARACTER_CREATE_SEND
{
	PSBMSG_HEAD header; // C1:F3:01
	BYTE result;
	char name[10];
	BYTE slot;
	WORD level;
	BYTE Class;
	BYTE equipment[24];
};

struct PMSG_CHARACTER_DELETE_SEND
{
	PSBMSG_HEAD header; // C1:F3:02
	BYTE result;
};

struct PMSG_CHARACTER_INFO_SEND
{
	PSBMSG_HEAD header; // C3:F3:03
	BYTE X;
	BYTE Y;
	BYTE Map;
	BYTE Dir;
	#if(GAMESERVER_UPDATE>=401)
	BYTE Experience[8];
	BYTE NextExperience[8];
	#else
	DWORD Experience;
	DWORD NextExperience;
	#endif
	WORD LevelUpPoint;
	WORD Strength;
	WORD Dexterity;
	WORD Vitality;
	WORD Energy;
	WORD Life;
	WORD MaxLife;
	WORD Mana;
	WORD MaxMana;
	#if(GAMESERVER_UPDATE>=201)
	WORD Shield;
	WORD MaxShield;
	#endif
	WORD BP;
	WORD MaxBP;
	DWORD Money;
	BYTE PKLevel;
	BYTE CtlCode;
	WORD FruitAddPoint;
	WORD MaxFruitAddPoint;
	WORD Leadership;
	WORD FruitSubPoint;
	WORD MaxFruitSubPoint;
	#if(GAMESERVER_UPDATE>=602)
	BYTE ExtInventory;
	#endif
	#if(GAMESERVER_EXTRA==1)
	DWORD ViewReset;
	DWORD ViewPoint;
	DWORD ViewCurHP;
	DWORD ViewMaxHP;
	DWORD ViewCurMP;
	DWORD ViewMaxMP;
	DWORD ViewCurBP;
	DWORD ViewMaxBP;
	#if(GAMESERVER_UPDATE>=201)
	DWORD ViewCurSD;
	DWORD ViewMaxSD;
	#endif
	DWORD ViewStrength;
	DWORD ViewDexterity;
	DWORD ViewVitality;
	DWORD ViewEnergy;
	DWORD ViewLeadership;
	WORD Class;
	#endif
};

struct PMSG_CHARACTER_REGEN_SEND
{
	PSBMSG_HEAD header; // C3:F3:04
	BYTE X;
	BYTE Y;
	BYTE Map;
	BYTE Dir;
	WORD Life;
	WORD Mana;
	#if(GAMESERVER_UPDATE>=201)
	WORD Shield;
	#endif
	WORD BP;
	#if(GAMESERVER_UPDATE>=401)
	BYTE Experience[8];
	#else
	DWORD Experience;
	#endif
	DWORD Money;
	#if(GAMESERVER_EXTRA==1)
	DWORD ViewCurHP;
	DWORD ViewCurMP;
	DWORD ViewCurBP;
	#if(GAMESERVER_UPDATE>=201)
	DWORD ViewCurSD;
	#endif
	#endif
};

struct PMSG_LEVEL_UP_SEND
{
	PSBMSG_HEAD header; // C1:F3:05
	WORD Level;
	WORD LevelUpPoint;
	WORD MaxLife;
	WORD MaxMana;
	#if(GAMESERVER_UPDATE>=201)
	WORD MaxShield;
	#endif
	WORD MaxBP;
	WORD FruitAddPoint;
	WORD MaxFruitAddPoint;
	WORD FruitSubPoint;
	WORD MaxFruitSubPoint;
	#if(GAMESERVER_EXTRA==1)
	DWORD ViewPoint;
	DWORD ViewMaxHP;
	DWORD ViewMaxMP;
	DWORD ViewMaxBP;
	#if(GAMESERVER_UPDATE>=201)
	DWORD ViewMaxSD;
	#endif
	DWORD ViewExperience;
	DWORD ViewNextExperience;
	#endif
};

struct PMSG_LEVEL_UP_POINT_SEND
{
	PSBMSG_HEAD header; // C1:F3:06
	BYTE result;
	WORD MaxLifeAndMana;
	#if(GAMESERVER_UPDATE>=201)
	WORD MaxShield;
	#endif
	WORD MaxBP;
	#if(GAMESERVER_EXTRA==1)
	DWORD ViewPoint;
	DWORD ViewMaxHP;
	DWORD ViewMaxMP;
	DWORD ViewMaxBP;
	#if(GAMESERVER_UPDATE>=201)
	DWORD ViewMaxSD;
	#endif
	DWORD ViewStrength;
	DWORD ViewDexterity;
	DWORD ViewVitality;
	DWORD ViewEnergy;
	DWORD ViewLeadership;
	#endif
};

struct PMSG_MONSTER_DAMAGE_SEND
{
	PSBMSG_HEAD header; // C1:F3:07
	BYTE damage[2];
	#if(GAMESERVER_UPDATE>=201)
	BYTE ShieldDamage[2];
	#endif
	#if(GAMESERVER_EXTRA==1)
	DWORD ViewCurHP;
	#if(GAMESERVER_UPDATE>=201)
	DWORD ViewCurSD;
	#endif
	DWORD ViewDamageHP;
	#if(GAMESERVER_UPDATE>=201)
	DWORD ViewDamageSD;
	#endif
	#endif
};

struct PMSG_PK_LEVEL_SEND
{
	PSBMSG_HEAD header; // C1:F3:08
	BYTE index[2];
	BYTE PKLevel;
};

struct PMSG_CHARACTER_NAME_CHECK_SEND
{
	PSBMSG_HEAD header; // C1:F3:15
	char name[10];
	BYTE result;
};

struct PMSG_CHARACTER_NAME_CHANGE_SEND
{
	PSBMSG_HEAD header; // C1:F3:16
	char OldName[10];
	char NewName[10];
	BYTE result;
};

struct PMSG_SUMMON_LIFE_SEND
{
	PSBMSG_HEAD header; // C1:F3:20
	BYTE life;
};

struct PMSG_TIME_VIEW_SEND
{
	PSBMSG_HEAD header; // C1:F3:22
	WORD time;
};

struct PMSG_OPTION_DATA_SEND
{
	#pragma pack(1)
	PSBMSG_HEAD header; // C1:F3:30
	#if(GAMESERVER_UPDATE>=401)
	BYTE SkillKey[20];
	#else
	BYTE SkillKey[10];
	#endif
	BYTE GameOption;
	BYTE QKey;
	BYTE WKey;
	BYTE EKey;
	BYTE ChatWindow;
	#if(GAMESERVER_UPDATE>=401)
	BYTE RKey;
	DWORD QWERLevel;
	#endif
	#pragma pack()
};

struct PMSG_MATH_AUTHENTICATOR_SEND
{
	PSBMSG_HEAD header; // C1:F3:32
	DWORD function;
	float value;
	char result[64];
};

struct PMSG_SERVER_COMMAND_SEND
{
	PSBMSG_HEAD header; // C1:F3:40
	BYTE type;
	BYTE x;
	BYTE y;
};

struct PMSG_ACHERON_ENTER_SEND
{
	PSBMSG_HEAD header; // C1:F8:[21:4C]
	BYTE result;
};

struct PMSG_SNS_DATA_SEND
{
	PWMSG_HEAD header; // C1:FB
	BYTE result;
	BYTE data[256];
};

struct PMSG_NEW_CHARACTER_INFO_SEND
{
	PSBMSG_HEAD header; // C1:F3:E0
	WORD Level;
	WORD LevelUpPoint;
	DWORD Experience;
	DWORD NextExperience;
	WORD Strength;
	WORD Dexterity;
	WORD Vitality;
	WORD Energy;
	WORD Leadership;
	WORD Life;
	WORD MaxLife;
	WORD Mana;
	WORD MaxMana;
	WORD BP;
	WORD MaxBP;
	#if(GAMESERVER_UPDATE>=201)
	WORD Shield;
	WORD MaxShield;
	#endif
	WORD FruitAddPoint;
	WORD MaxFruitAddPoint;
	WORD FruitSubPoint;
	WORD MaxFruitSubPoint;
	//EXTRA
	DWORD ViewReset;
	DWORD ViewPoint;
	DWORD ViewCurHP;
	DWORD ViewMaxHP;
	DWORD ViewCurMP;
	DWORD ViewMaxMP;
	DWORD ViewCurBP;
	DWORD ViewMaxBP;
	#if(GAMESERVER_UPDATE>=201)
	DWORD ViewCurSD;
	DWORD ViewMaxSD;
	#endif
	DWORD ViewStrength;
	DWORD ViewDexterity;
	DWORD ViewVitality;
	DWORD ViewEnergy;
	DWORD ViewLeadership;
};

struct PMSG_NEW_CHARACTER_CALC_SEND
{
	PSBMSG_HEAD header; // C1:F3:E1
	DWORD ViewCurHP;
	DWORD ViewMaxHP;
	DWORD ViewCurMP;
	DWORD ViewMaxMP;
	DWORD ViewCurBP;
	DWORD ViewMaxBP;
	#if(GAMESERVER_UPDATE>=201)
	DWORD ViewCurSD;
	DWORD ViewMaxSD;
	#endif
	DWORD ViewAddStrength;
	DWORD ViewAddDexterity;
	DWORD ViewAddVitality;
	DWORD ViewAddEnergy;
	DWORD ViewAddLeadership;
	DWORD ViewPhysiDamageMin;
	DWORD ViewPhysiDamageMax;
	DWORD ViewMagicDamageMin;
	DWORD ViewMagicDamageMax;
	DWORD ViewCurseDamageMin;
	DWORD ViewCurseDamageMax;
	DWORD ViewMulPhysiDamage;
	DWORD ViewDivPhysiDamage;
	DWORD ViewMulMagicDamage;
	DWORD ViewDivMagicDamage;
	DWORD ViewMulCurseDamage;
	DWORD ViewDivCurseDamage;
	DWORD ViewMagicDamageRate;
	DWORD ViewCurseDamageRate;
	DWORD ViewPhysiSpeed;
	DWORD ViewMagicSpeed;
	DWORD ViewAttackSuccessRate;
	DWORD ViewAttackSuccessRatePvP;
	DWORD ViewDefense;
	DWORD ViewDefenseSuccessRate;
	DWORD ViewDefenseSuccessRatePvP;
	DWORD ViewDamageMultiplier;
	DWORD ViewRFDamageMultiplierA;
	DWORD ViewRFDamageMultiplierB;
	DWORD ViewRFDamageMultiplierC;
	DWORD ViewDarkSpiritAttackDamageMin;
	DWORD ViewDarkSpiritAttackDamageMax;
	DWORD ViewDarkSpiritAttackSpeed;
	DWORD ViewDarkSpiritAttackSuccessRate;
};

struct PMSG_NEW_HEALTH_BAR_SEND
{
	PSWMSG_HEAD header; // C2:F3:E2
	BYTE count;
};

struct PMSG_NEW_HEALTH_BAR
{
	WORD index;
	BYTE type;
	BYTE rate;
};

struct PMSG_NEW_GENS_BATTLE_INFO_SEND
{
	PSBMSG_HEAD header; // C1:F3:E3
	BYTE GensBattleMapCount;
	BYTE GensMoveIndexCount;
	BYTE GensBattleMap[120];
	BYTE GensMoveIndex[120];
};

struct PMSG_NEW_MESSAGE_SEND
{
	PSBMSG_HEAD header; // C1:F3:[E4:E5]
	BYTE type;
	BYTE color;
	char message[128];
};

struct PMSG_WINDOW_NAME_SEND
{
	PSBMSG_HEAD header; // C1:F3:E8
	char title[64];
};

//**********************************************//
//**********************************************//
//**********************************************//

void ProtocolCore(BYTE head,BYTE* lpMsg,int size,int aIndex,int encrypt,int serial);
void CGChatRecv(PMSG_CHAT_RECV* lpMsg,int aIndex);
void CGChatWhisperRecv(PMSG_CHAT_WHISPER_RECV* lpMsg,int aIndex);
void CGMainCheckRecv(PMSG_MAIN_CHECK_RECV* lpMsg,int aIndex);
void CGLiveClientRecv(PMSG_LIVE_CLIENT_RECV* lpMsg,int aIndex);
void CGPositionRecv(PMSG_POSITION_RECV* lpMsg,int aIndex);
void CGActionRecv(PMSG_ACTION_RECV* lpMsg,int aIndex);
void CGEventRemainTimeRecv(PMSG_EVENT_REMAIN_TIME_RECV* lpMsg,int aIndex);
void CGPetItemCommandRecv(PMSG_PET_ITEM_COMMAND_RECV* lpMsg,int aIndex);
void CGPetItemInfoRecv(PMSG_PET_ITEM_INFO_RECV* lpMsg,int aIndex);
void CGMapServerMoveAuthRecv(PMSG_MAP_SERVER_MOVE_AUTH_RECV* lpMsg,int aIndex);
void CGMoveRecv(PMSG_MOVE_RECV* lpMsg,int aIndex);
void CGConnectAccountRecv(PMSG_CONNECT_ACCOUNT_RECV* lpMsg,int aIndex);
void CGCloseClientRecv(PMSG_CLOSE_CLIENT_RECV* lpMsg,int aIndex);
void CGCharacterListRecv(int aIndex);
void CGCharacterCreateRecv(PMSG_CHARACTER_CREATE_RECV* lpMsg,int aIndex);
void CGCharacterDeleteRecv(PMSG_CHARACTER_DELETE_RECV* lpMsg,int aIndex);
void CGCharacterInfoRecv(PMSG_CHARACTER_INFO_RECV* lpMsg,int aIndex);
void CGLevelUpPointRecv(PMSG_LEVEL_UP_POINT_RECV* lpMsg,int aIndex);
void CGCharacterMoveViewportEnableRecv(int aIndex);
void CGCharacterNameCheckRecv(PMSG_CHARACTER_NAME_CHECK_RECV* lpMsg,int aIndex);
void CGCharacterNameChangeRecv(PMSG_CHARACTER_NAME_CHANGE_RECV* lpMsg,int aIndex);
void CGOptionChangeSkinRecv(PMSG_OPTION_CHANGE_SKIN_RECV* lpMsg,int aIndex);
void CGOptionDataRecv(PMSG_OPTION_DATA_RECV* lpMsg,int aIndex);
void CGClientSecurityBreachRecv(PMSG_CLIENT_SECURITY_BREACH_RECV* lpMsg,int aIndex);
void CGAcheronEnterRecv(int aIndex);
void CGAcheronGuardianEnterRecv(int aIndex);
void CGSNSDataRecv(PMSG_SNS_DATA_RECV* lpMsg,int aIndex);
void CGSNSDataLogRecv(PMSG_SNS_DATA_LOG_RECV* lpMsg,int aIndex);
void GCChatTargetSend(LPOBJ lpObj,int aIndex,char* message);
void GCChatWhisperSend(int aIndex,char* name,char* message);
void GCMainCheckSend(int aIndex);
void GCEventStateSend(int aIndex,BYTE state,BYTE event);
void GCEventStateSendToAll(int Map,BYTE state,BYTE event);
void GCServerMsgSend(int aIndex,BYTE msg);
void GCWeatherSend(int aIndex,BYTE weather);
void GCDamageSend(int aIndex,int bIndex,BYTE flag,int damage,int type,int ShieldDamage);
void GCUserDieSend(LPOBJ lpObj,int aIndex,int skill,int bIndex);
void GCActionSend(LPOBJ lpObj,BYTE action,int aIndex,int bIndex);
void GCMoneySend(int aIndex,DWORD money);
void GCLifeSend(int aIndex,BYTE type,int life,int shield);
void GCManaSend(int aIndex,BYTE type,int mana,int bp);
void GCItemUseSpecialTimeSend(int aIndex,BYTE number,int time);
void GCMapAttrSend(int aIndex,BYTE type,BYTE attr,BYTE flag,BYTE count,PMSG_MAP_ATTR* lpInfo);
void GCPartyItemInfoSend(int aIndex,CItem* lpItem);
void GCEffectInfoSend(int aIndex,BYTE effect);
void GCGuildMasterQuestionSend(int aIndex);
void GCMonsterSkillSend(LPOBJ lpObj,LPOBJ lpTarget,int skill);
void GCMonsterDieSend(int aIndex,int bIndex,int experience,int damage,BYTE flag);
void GCRewardExperienceSend(int aIndex,int experience);
void GCEventEnterCountSend(int aIndex,BYTE EventType,BYTE EnterCount);
void GCPetItemInfoSend(int aIndex,BYTE type,BYTE flag,BYTE slot,BYTE level,DWORD experience,BYTE durability);
void GCMapServerMoveAuthSend(int aIndex,BYTE result);
void GCTaxInfoSend(int aIndex,BYTE type,BYTE rate);
void GCCreationStateSend(int aIndex,BYTE state);
void GCMonsterAreaSkillSend(int aIndex,int MonsterClass,int sx,int sy,int tx,int ty);
void GCElementalDamageSend(int aIndex,int bIndex,BYTE attribute,int damage);
void GCCharacterCreationEnableSend(int aIndex,BYTE flag,BYTE result);
void GCLifeUpdateSend(LPOBJ lpObj);
void GCCharacterAttackSpeedSend(LPOBJ lpObj);
void GCConnectClientSend(int aIndex,BYTE result);
void GCConnectAccountSend(int aIndex,BYTE result);
void GCConnectAccountSend(int aIndex,BYTE result,SOCKET socket);
void GCCloseClientSend(int aIndex,BYTE result);
void GCCharacterRegenSend(LPOBJ lpObj);
void GCLevelUpSend(LPOBJ lpObj);
void GCMonsterDamageSend(int aIndex,int damage,int ShieldDamage);
void GCPKLevelSend(int aIndex,int PKLevel);
void GCSummonLifeSend(int aIndex,int life,int MaxLife);
void GCTimeViewSend(int aIndex,int time);
void GCMathAuthenticatorSend(int aIndex);
void GCServerCommandSend(int aIndex,int type,int x,int y,int Viewport);
void GCNewCharacterInfoSend(LPOBJ lpObj);
void GCNewCharacterCalcSend(LPOBJ lpObj);
void GCNewHealthBarSend(LPOBJ lpObj);
void GCNewGensBattleInfoSend(LPOBJ lpObj);
void GCNewMessageSend(int aIndex,int type,int color,char* message,...);
void GCWindowsNameSend(int aIndex,char* title);

//**************************************************************************//
// RAW FUNCTIONS ***********************************************************//
//**************************************************************************//

struct PMSG_REQ_GIVEUPCASTLESIEGE
{
	PSBMSG_HEAD h;
	BYTE btGiveUp;
};

struct CSP_ANS_GUILDREGINFO
{
	PSBMSG_HEAD h;
	int iResult;	// 4
	WORD wMapSvrNum;	// 8
	int iIndex;	// C
	char szGuildName[8];	// 10
	int iRegMarkCount;	// 18
	BYTE bIsGiveUp;	// 1C
	BYTE btRegRank;	// 1D
};

struct PMSG_REQ_REGGUILDMARK
{
	PSBMSG_HEAD h;
	BYTE btItemPos;
};

struct CSP_ANS_GUILDREGMARK
{
	struct PSBMSG_HEAD h;
	int iResult;
	WORD wMapSvrNum;
	int iIndex;
	char szGuildName[8];
	int iItemPos;
	int iRegMarkCount;
};

struct PMSG_REQ_NPCBUY
{
	PSBMSG_HEAD h;
	int iNpcNumber;
	int iNpcIndex;
};

struct PMSG_REQ_NPCREPAIR
{
	PSBMSG_HEAD h;
	int iNpcNumber;
	int iNpcIndex;
};

struct PMSG_REQ_NPCUPGRADE
{
	PSBMSG_HEAD h;
	int iNpcNumber;
	int iNpcIndex;
	int iNpcUpType;
	int iNpcUpValue;
};

struct PMSG_REQ_TAXRATECHANGE
{
	PSBMSG_HEAD h;
	BYTE btTaxType;
	BYTE btTaxRate1;
	BYTE btTaxRate2;
	BYTE btTaxRate3;
	BYTE btTaxRate4;
};

struct PMSG_REQ_MONEYDRAWOUT
{
	PSBMSG_HEAD h;
	BYTE btMoney1;
	BYTE btMoney2;
	BYTE btMoney3;
	BYTE btMoney4;
};

struct PMSG_REQ_CSGATEOPERATE
{
	PSBMSG_HEAD h;
	BYTE btOperate;
	BYTE btIndex1;
	BYTE btIndex2;
};

struct PMSG_REQ_CSCOMMAND
{
	PSBMSG_HEAD h;
	BYTE btTeam;
	BYTE btX;
	BYTE btY;
	BYTE btCommand;
};

struct PMSG_REQ_CSHUNTZONEENTER
{
	PSBMSG_HEAD h;
	BYTE btHuntZoneEnter;
};

struct PMSG_REQ_NPCDBLIST
{
	PBMSG_HEAD h;
	BYTE btMonsterCode;
};

struct PMSG_ANS_CSATTKGUILDLIST
{
	PWMSG_HEAD h;
	BYTE btResult;
	int iCount;
};

struct PMSG_ANS_MOVE_TO_CASTLE_HUNTZONE
{
	PSBMSG_HEAD h; // +0x0(0x4)
	BYTE btResult; // +0x4(0x1)
};

struct PMSG_ANS_GUILDMARK_OF_CASTLEOWNER
{
	PSBMSG_HEAD h;	// C1:B9:02
	BYTE GuildMarkOfCastleOwner[32];	// 4
};

struct CSP_ANS_CSINITDATA
{
	PWMSG_HEAD h;	// 0
	int iResult;		// 4
	WORD wMapSvrNum;	// 8
	WORD wStartYear;	// a
	BYTE btStartMonth;	// c
	BYTE btStartDay;	// d
	WORD wEndYear; // e
	BYTE btEndMonth; // 10
	BYTE btEndDay;	// 11
	BYTE btIsSiegeGuildList;	// 12
	BYTE btIsSiegeEnded;	// 13
	BYTE btIsCastleOccupied;	// 14
	char szCastleOwnGuild[8]; // 15
	__int64 i64CastleMoney; // 20
	int iTaxRateChaos; // 28
	int iTaxRateStore; // 2c
	int iTaxHuntZone; // 30
	int iFirstCreate; // 34
	int iCount; // 38
};

struct CSP_CSINITDATA
{
	int iNpcNumber; // 0
	int iNpcIndex; // 4
	int iNpcDfLevel; // 8
	int iNpcRgLevel; // c
	int iNpcMaxHp; // 10
	int iNpcHp; // 14
	BYTE btNpcX; // 18
	BYTE btNpcY; // 19
	BYTE btNpcDIR; // 1a
};

struct CSP_CALCREGGUILDLIST
{
	char szGuildName[8]; // 0
	int iRegMarkCount; // 8
	int iGuildMemberCount; // c
	int iGuildMasterLevel; // 10
	int iSeqNum; // 14
};

struct CSP_CSGUILDUNIONINFO
{
	char szGuildName[8]; // 0
	int iCsGuildID; // 8
};

struct CSP_CSLOADTOTALGUILDINFO
{
	char szGuildName[8]; // 0
	int iCsGuildID; // 8
	int iGuildInvolved; // c
	#if(GAMESERVER_UPDATE>=205)
	int iGuildScore; //10 season 2.5 add-on
	#endif
};

struct PMSG_CSATTKGUILDLIST
{
	BYTE btCsJoinSide; // 0
	BYTE btGuildInvolved; // 1
	char szGuildName[8]; // 2
	#if(GAMESERVER_UPDATE>=205)
	int iGuildScore; //A season 2.5 add-on
	#endif
};

struct PMSG_REQ_MOVE_TO_CASTLE_HUNTZONE
{
	PSBMSG_HEAD h;
	int iPrice;
};

struct PMSG_ANS_GUARD_IN_CASTLE_HUNTZONE
{
	PSBMSG_HEAD h;
	BYTE btResult;
	BYTE btUsable;
	int iCurrentPrice;
	int iMaxPrice;
	int iUnitOfPrice;
};

void CGReqCastleSiegeState(int iIndex);
void GCAnsCastleSiegeState(int iIndex,int iResult,char* lpszGuildName,char* lpszGuildMaster);
void CGReqRegCastleSiege(int iIndex);
void GCAnsRegCastleSiege(int iIndex,int iResult,char* lpszGuildName);
void CGReqGiveUpCastleSiege(PMSG_REQ_GIVEUPCASTLESIEGE* lpMsg,int iIndex);
void GCAnsGiveUpCastleSiege(int iIndex,int iResult,int bGiveUp,int iMarkCount,char* lpszGuildName);
void CGReqGuildRegInfo(int iIndex);
void GCAnsGuildRegInfo(int iIndex,int iResult,CSP_ANS_GUILDREGINFO* lpMsgResult);
void CGReqRegGuildMark(PMSG_REQ_REGGUILDMARK* lpMsg,int iIndex);
void GCAnsRegGuildMark(int iIndex,int iResult,CSP_ANS_GUILDREGMARK* lpMsgResult);
void CGReqNpcBuy(PMSG_REQ_NPCBUY* lpMsg,int iIndex);
void GCAnsNpcBuy(int iIndex,int iResult,int iNpcNumber,int iNpcIndex);
void CGReqNpcRepair(PMSG_REQ_NPCREPAIR* lpMsg,int iIndex);
void GCAnsNpcRepair(int iIndex,int iResult,int iNpcNumber,int iNpcIndex,int iNpcHP,int iNpcMaxHP);
void CGReqNpcUpgrade(PMSG_REQ_NPCUPGRADE* lpMsg,int iIndex);
void GCAnsNpcUpgrade(int iIndex,int iResult,int iNpcNumber,int iNpcIndex,int iNpcUpType,int iNpcUpValue);
void CGReqTaxMoneyInfo(int iIndex);
void GCAnsTaxMoneyInfo(int iIndex,int iResult,BYTE btTaxRateChaos,BYTE btTaxRateStore,__int64 i64Money);
void CGReqTaxRateChange(PMSG_REQ_TAXRATECHANGE* lpMsg,int iIndex);
void GCAnsTaxRateChange(int iIndex,int iResult,BYTE btTaxType,int iTaxRate);
void CGReqMoneyDrawOut(PMSG_REQ_MONEYDRAWOUT* lpMsg,int iIndex);
void GCAnsMoneyDrawOut(int iIndex,int iResult,__int64 i64Money);
void GCAnsCsGateState(int iIndex,int iResult,int iGateIndex);
void CGReqCsGateOperate(PMSG_REQ_CSGATEOPERATE* lpMsg,int iIndex);
void GCAnsCsGateOperate(int iIndex,int iResult,int iGateIndex,int iGateOperate);
void GCAnsCsGateCurState(int iIndex,int iGateIndex,int iGateOperate);
void GCAnsCsAccessSwitchState(int iIndex,int iSwitchIndex,int iSwitchUserIndex,BYTE btSwitchState);
void GCAnsCsAccessCrownState(int iIndex,BYTE btCrownState);
void GCAnsCsNotifyStart(int iIndex,BYTE btStartState);
void GCAnsCsNotifyProgress(int iIndex,BYTE btCastleSiegeState,char* lpszGuildName);
void CGReqCsMiniMapData(int iIndex);
void GCAnsCsMiniMapData(int iIndex,BYTE btResult);
void CGReqStopCsMiniMapData(int iIndex);
void CGReqCsSendCommand(PMSG_REQ_CSCOMMAND* lpMsg,int iIndex);
void GCAnsCsSendCommand(int iCsJoinSide,BYTE btTeam,BYTE btX,BYTE btY,BYTE btCommand);
void GCAnsCsLeftTimeAlarm(BYTE btHour,BYTE btMinute);
void GCAnsSelfCsLeftTimeAlarm(int iIndex,BYTE btHour,BYTE btMinute);
void CGReqCsSetEnterHuntZone(PMSG_REQ_CSHUNTZONEENTER* lpMsg,int iIndex);
void GCAnsCsSetEnterHuntZone(int iIndex,BYTE btResult,BYTE btEnterHuntZone);
void CGReqNpcDbList(PMSG_REQ_NPCDBLIST* lpMsg,int iIndex);
void CGReqCsRegGuildList(int iIndex);
void CGReqCsAttkGuildList(int iIndex);
void CGReqGuildMarkOfCastleOwner(int iIndex);
void CGReqCastleHuntZoneEntrance(PMSG_REQ_MOVE_TO_CASTLE_HUNTZONE*aRecv,int iIndex);