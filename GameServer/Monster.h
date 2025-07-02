#pragma once

#include "Protocol.h"
#include "User.h"

void gObjMonsterDieGiveItem(LPOBJ lpObj,LPOBJ lpTarget);
bool gObjSetPosMonster(int aIndex,int number);
bool gObjSetMonster(int aIndex,int MonsterClass);
bool gObjMonsterRegen(LPOBJ lpObj);
bool gObjMonsterMoveCheck(LPOBJ lpObj,int tx,int ty);
void gObjMonsterInitHitDamage(LPOBJ lpObj);
void gObjMonsterSetHitDamage(LPOBJ lpObj,int aIndex,int damage);
int gObjMonsterDelHitDamageUser(LPOBJ lpObj);
int gObjMonsterGetTopHitDamageUser(LPOBJ lpObj);
int gObjMonsterGetTopHitDamageParty(LPOBJ lpObj,int PartyNumber,int* TopHitDamageUser);
int gObjAddMonster(int map);
int gObjAddSummon();
int gObjSummonSetEnemy(LPOBJ lpObj,int aIndex);
void gObjSummonKill(int aIndex);
void gObjMonsterMagicAttack(LPOBJ lpObj,int skill);
void gObjMonsterBeattackRecv(BYTE* aRecv,int aIndex);
void gObjMonsterAttack(LPOBJ lpObj,LPOBJ lpTarget);
void gObjMonsterDie(LPOBJ lpObj,LPOBJ lpTarget);
void gObjMonsterStateProc(LPOBJ lpObj,int code,int aIndex,int SubCode);
void gObjTrapFindTarget(LPOBJ lpObj);
void gObjMonsterUpdateProc(LPOBJ lpObj);
void gObjMonsterReactionProc(LPOBJ lpObj);
void gObjMonsterGetXYToPatrol(LPOBJ lpObj);
bool gObjMonsterGetXYToChase(LPOBJ lpObj);
void gObjMonsterGetXYToOwner(LPOBJ lpObj);
void gObjMonsterFindTarget(LPOBJ lpObj);
void gObjMonsterFindPathToMove(LPOBJ lpObj);
void gObjMonsterProcessHit(LPOBJ lpObj,int aIndex);