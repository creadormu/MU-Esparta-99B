// Map.h: interface for the CMap class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "MapItem.h"
#include "MapPath.h"


#define MAX_MAP 100
#define MAX_MAP_ITEM 300
#define MAX_MAP_WIDTH 256
#define MAX_MAP_HEIGHT 256

#define MAP_ITEM_RANGE(x) (((x)<0)?0:((x)>=MAX_MAP_ITEM)?0:1)
#define KALIMA_MAP_RANGE(x) (((x)<MAP_KALIMA1)?0:((x)>MAP_KALIMA6)?(((x)==MAP_KALIMA7)?1:0):1)
#define DS_MAP_RANGE(x) (((x)==MAP_DEVIL_SQUARE1)?1:((x)==MAP_DEVIL_SQUARE2)?1:0)
#if(GAMESERVER_UPDATE>=401)
#define BC_MAP_RANGE(x) (((x)<MAP_BLOOD_CASTLE1)?0:((x)>MAP_BLOOD_CASTLE7)?(((x)==MAP_BLOOD_CASTLE8)?1:0):1)
#define CC_MAP_RANGE(x) (((x)<MAP_CHAOS_CASTLE1)?0:((x)>MAP_CHAOS_CASTLE6)?(((x)==MAP_CHAOS_CASTLE7)?1:0):1)
#define IT_MAP_RANGE(x) (((x)<MAP_ILLUSION_TEMPLE1)?0:((x)>MAP_ILLUSION_TEMPLE6)?0:1)
#else
#define BC_MAP_RANGE(x) (((x)<MAP_BLOOD_CASTLE1)?0:((x)>MAP_BLOOD_CASTLE7)?0:1)
#define CC_MAP_RANGE(x) (((x)<MAP_CHAOS_CASTLE1)?0:((x)>MAP_CHAOS_CASTLE6)?0:1)
#if(GAMESERVER_UPDATE>=301)
#define IT_MAP_RANGE(x) (((x)<MAP_ILLUSION_TEMPLE1)?0:((x)>MAP_ILLUSION_TEMPLE5)?0:1)
#endif
#endif
#if(GAMESERVER_UPDATE>=603)
#define DA_MAP_RANGE(x) (((x)==MAP_DUEL_ARENA)?1:0)
#define DG_MAP_RANGE(x) (((x)<MAP_DOUBLE_GOER1)?0:((x)>MAP_DOUBLE_GOER4)?0:1)
#define IG_MAP_RANGE(x) (((x)<MAP_IMPERIAL_GUARDIAN1)?0:((x)>MAP_IMPERIAL_GUARDIAN4)?0:1)
#endif
#if(GAMESERVER_UPDATE>=803)
#define CF_MAP_RANGE(x) (((x)==MAP_CHAOS_CASTLE_FINAL)?1:0)
#define AB_MAP_RANGE(x) (((x)==MAP_ACHERON2)?1:((x)==MAP_DEVENTER2)?1:0)
#endif

enum eMapNumber
{
	MAP_LORENCIA = 0,
	MAP_DUNGEON = 1,
	MAP_DEVIAS = 2,
	MAP_NORIA = 3,
	MAP_LOST_TOWER = 4,
	MAP_RESERVED1 = 5,
	MAP_ARENA = 6,
	MAP_ATLANS = 7,
	MAP_TARKAN = 8,
	MAP_DEVIL_SQUARE1 = 9,
	MAP_ICARUS = 10,
	MAP_BLOOD_CASTLE1 = 11,
	MAP_BLOOD_CASTLE2 = 12,
	MAP_BLOOD_CASTLE3 = 13,
	MAP_BLOOD_CASTLE4 = 14,
	MAP_BLOOD_CASTLE5 = 15,
	MAP_BLOOD_CASTLE6 = 16,
	MAP_BLOOD_CASTLE7 = 17,
	MAP_CHAOS_CASTLE1 = 18,
	MAP_CHAOS_CASTLE2 = 19,
	MAP_CHAOS_CASTLE3 = 20,
	MAP_CHAOS_CASTLE4 = 21,
	MAP_CHAOS_CASTLE5 = 22,
	MAP_CHAOS_CASTLE6 = 23,
	MAP_KALIMA1 = 24,
	MAP_KALIMA2 = 25,
	MAP_KALIMA3 = 26,
	MAP_KALIMA4 = 27,
	MAP_KALIMA5 = 28,
	MAP_KALIMA6 = 29,
	MAP_CASTLE_SIEGE = 30,
	MAP_LAND_OF_TRIALS = 31,
	MAP_DEVIL_SQUARE2 = 32,
	MAP_AIDA = 33,
	MAP_CRYWOLF = 34,
	MAP_RESERVED2 = 35,
	MAP_KALIMA7 = 36,
	#if(GAMESERVER_UPDATE>=201)
	MAP_KANTURU1 = 37,
	MAP_KANTURU2 = 38,
	MAP_KANTURU3 = 39,
	#endif
	MAP_SILENT = 40,
	#if(GAMESERVER_UPDATE>=301)
	MAP_BARRACKS = 41,
	MAP_REFUGE = 42,
	MAP_ILLUSION_TEMPLE1 = 45,
	MAP_ILLUSION_TEMPLE2 = 46,
	MAP_ILLUSION_TEMPLE3 = 47,
	MAP_ILLUSION_TEMPLE4 = 48,
	MAP_ILLUSION_TEMPLE5 = 49,
	#endif
	#if(GAMESERVER_UPDATE>=401)
	MAP_ILLUSION_TEMPLE6 = 50,
	MAP_ELBELAND = 51,
	MAP_BLOOD_CASTLE8 = 52,
	MAP_CHAOS_CASTLE7 = 53,
	MAP_SWAMP_OF_CALMNESS = 56,
	MAP_RAKLION1 = 57,
	MAP_RAKLION2 = 58,
	MAP_SANTA_TOWN = 62,
	#endif
	#if(GAMESERVER_UPDATE>=603)
	MAP_VULCANUS = 63,
	MAP_DUEL_ARENA = 64,
	MAP_DOUBLE_GOER1 = 65,
	MAP_DOUBLE_GOER2 = 66,
	MAP_DOUBLE_GOER3 = 67,
	MAP_DOUBLE_GOER4 = 68,
	MAP_IMPERIAL_GUARDIAN1 = 69,
	MAP_IMPERIAL_GUARDIAN2 = 70,
	MAP_IMPERIAL_GUARDIAN3 = 71,
	MAP_IMPERIAL_GUARDIAN4 = 72,
	MAP_LOREN_MARKET = 79,
	MAP_KARUTAN1 = 80,
	MAP_KARUTAN2 = 81,
	#endif
	#if(GAMESERVER_UPDATE>=803)
	MAP_ACHERON1 = 91,
	MAP_ACHERON2 = 92,
	MAP_DEVENTER1 = 95,
	MAP_DEVENTER2 = 96,
	MAP_CHAOS_CASTLE_FINAL = 97,
	MAP_ILLUSION_TEMPLE_FINAL1 = 98,
	MAP_ILLUSION_TEMPLE_FINAL2 = 99,
	#endif
};

class CMap
{
public:
	CMap();
	virtual ~CMap();
	void Load(char* path,int map);
	BYTE GetAttr(int x,int y);
	bool CheckAttr(int x,int y,BYTE attr);
	void SetAttr(int x,int y,BYTE attr);
	void DelAttr(int x,int y,BYTE attr);
	bool CheckStandAttr(int x,int y);
	void SetStandAttr(int x,int y);
	void DelStandAttr(int x,int y);
	BYTE GetWeather();
	void SetWeather(BYTE weather,BYTE variation);
	void WeatherVariationProcess();
	void WeatherAllSend(BYTE weather);
	void GetMapRandomPos(short* ox,short* oy,int size);
	bool MoneyItemDrop(int money,int x,int y);
	bool MonsterItemDrop(int index,int level,float dur,int x,int y,BYTE Option1,BYTE Option2,BYTE Option3,BYTE NewOption,BYTE SetOption,int aIndex,DWORD serial,BYTE JewelOfHarmonyOption,BYTE ItemOptionEx,BYTE SocketOption[MAX_SOCKET_OPTION],BYTE SocketOptionBonus,DWORD duration);
	bool ItemDrop(int index,int level,float dur,int x,int y,BYTE Option1,BYTE Option2,BYTE Option3,BYTE NewOption,BYTE SetOption,DWORD serial,int aIndex,int PetLevel,int PetExp,BYTE JewelOfHarmonyOption,BYTE ItemOptionEx,BYTE SocketOption[MAX_SOCKET_OPTION],BYTE SocketOptionBonus,DWORD duration);
	bool CheckItemGive(int aIndex,int index);
	void ItemGive(int aIndex,int index);
	void StateSetDestroy();
	BOOL CheckWall(int sx,int sy,int tx,int ty);
	BYTE CheckWall2(int sx,int sy,int tx,int ty);
	bool PathFinding2(int sx,int sy,int tx,int ty,PATH_INFO* path);
	bool PathFinding3(int sx,int sy,int tx,int ty,PATH_INFO* path);
	bool PathFinding4(int sx,int sy,int tx,int ty,PATH_INFO* path);
public:
	BYTE m_Weather;
	BYTE m_WeatherVariation;
	DWORD m_WeatherTimer;
	DWORD m_NextWeatherTimer;
	BYTE* m_MapAttr;
	int m_width;
	int m_height;
	CMapPath* m_path;
	CMapItem m_Item[MAX_MAP_ITEM];
	int m_MapNumber;
	int m_ItemCount;
};

extern CMap gMap[MAX_MAP];
