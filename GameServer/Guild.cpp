// Guild.cpp: implementation of the CGuild class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Guild.h"
#include "ArcaBattle.h"
#include "BattleSoccer.h"
#include "CastleSiege.h"
#include "CastleSiegeSync.h"
#include "CustomArena.h"
#include "DefaultClassInfo.h"
#include "GameMain.h"
#include "GensSystem.h"
#include "GuildManager.h"
#include "GuildMatching.h"
#include "Map.h"
#include "MapManager.h"
#include "Message.h"
#include "Monster.h"
#include "Notice.h"
#include "ServerInfo.h"
#include "Union.h"
#include "Util.h"
#include "Viewport.h"

CGuild gGuild;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGuild::CGuild() // OK
{

}

CGuild::~CGuild() // OK
{

}

void CGuild::DGGuildCreateRecv(SDHP_GUILD_CREATE_RECV* lpMsg) // OK
{
	if(lpMsg->result == 1)
	{
		gGuildManager.AddGuild(lpMsg->Index,lpMsg->Name,lpMsg->Mark,lpMsg->Master,"",0);
	}

	LPOBJ lpObj = gObjFind(lpMsg->Master);

	if(lpObj != 0)
	{
		this->GCGuildCreateResultSend(lpObj->Index,lpMsg->result,lpMsg->type);

		if(lpMsg->result == 1)
		{
			if(lpObj->Interface.use != 0 && lpObj->Interface.type == INTERFACE_GUILD_CREATE)
			{
				lpObj->Interface.use = 0;
			}

			GUILD_INFO* lpGuild = gGuildManager.GetGuild(lpMsg->Name);

			if(lpGuild != 0)
			{
				lpObj->Guild = lpGuild;
			
				lpObj->GuildIndex = lpMsg->Index;
			
				lpObj->GuildRank = GUILD_RANK_MASTER;
			
				memcpy(lpObj->GuildName,lpMsg->Name,sizeof(lpObj->GuildName));

				gViewport.GCViewportSimpleGuildSend(lpObj);

				gViewport.GCViewportSimpleUnionSend(lpObj);
			}
		}
	}
}

void CGuild::DGGuildDeleteRecv(SDHP_GUILD_DELETE_RECV* lpMsg) // OK
{
	if(lpMsg->result == 1)
	{
		GUILD_INFO* lpGuild = gGuildManager.GetGuild(lpMsg->Name);

		if(lpGuild != 0)
		{
			GUILD_INFO* lpRival = gGuildManager.GetGuild(lpGuild->GuildRival);

			if(lpRival != 0)
			{
				lpRival->GuildRival = GUILD_RELATION_NONE;
			}

			for(int n=0;n < MAX_GUILD_MEMBER;n++)
			{
				if(lpGuild->Member[n].IsValid() != 0)
				{
					LPOBJ lpTarget = &gObj[lpGuild->Member[n].Index];

					lpTarget->Guild = 0;

					lpTarget->GuildIndex = 0;

					lpTarget->GuildRank = GUILD_RANK_NONE;

					memset(lpTarget->GuildName,0,sizeof(lpTarget->GuildName));

					gViewport.DestroyViewportGuild(lpTarget->Index);

					gViewport.GCViewportSimpleUnionSend(lpTarget);

					this->GCGuildDeleteSend(lpTarget->Index,lpMsg->result);
				}
			}

			gUnionManager.DelUnion(lpGuild->Index);

			gGuildManager.DelGuild(lpMsg->Name);
		}
	}
	else
	{
		LPOBJ lpObj = gObjFind(lpMsg->Member);

		if(lpObj != 0)
		{
			this->GCGuildDeleteSend(lpObj->Index,lpMsg->result);
		}
	}
}

void CGuild::DGGuildMemberAddRecv(SDHP_GUILD_MEMBER_ADD_RECV* lpMsg) // OK
{
	LPOBJ lpObj = gObjFind(lpMsg->Member);

	if(lpObj != 0)
	{
		this->GCGuildResultSend(lpObj->Index,lpMsg->result);

		if(lpMsg->result == 1)
		{
			gGuildManager.AddMember(lpMsg->Name,lpMsg->Member,lpObj->Index,GUILD_RANK_MEMBER,lpMsg->Server);

			GUILD_INFO* lpGuild = gGuildManager.GetGuild(lpMsg->Name);

			if(lpGuild != 0)
			{
				lpObj->Guild = lpGuild;

				lpObj->GuildIndex = lpGuild->Index;

				lpObj->GuildRank = GUILD_RANK_MEMBER;

				memcpy(lpObj->GuildName,lpMsg->Name,sizeof(lpObj->GuildName));

				gViewport.GCViewportSimpleGuildSend(lpObj);

				gViewport.GCViewportSimpleUnionSend(lpObj);
			}
		}
	}
	else
	{
		if(lpMsg->result == 1)
		{
			gGuildManager.AddMember(lpMsg->Name,lpMsg->Member,-1,GUILD_RANK_MEMBER,lpMsg->Server);
		}
	}
}

void CGuild::DGGuildMemberDelRecv(SDHP_GUILD_MEMBER_DEL_RECV* lpMsg) // OK
{
	if(lpMsg->result == 1)
	{
		gGuildManager.DelMember(lpMsg->Name,lpMsg->Member);
	}

	LPOBJ lpObj = gObjFind(lpMsg->Member);

	if(lpObj != 0)
	{
		this->GCGuildDeleteSend(lpObj->Index,lpMsg->result);

		if(lpMsg->result == 1)
		{
			lpObj->Guild = 0;
			
			lpObj->GuildIndex = 0;
			
			lpObj->GuildRank = GUILD_RANK_NONE;
			
			memset(lpObj->GuildName,0,sizeof(lpObj->GuildName));

			gViewport.DestroyViewportGuild(lpObj->Index);

			gViewport.GCViewportSimpleUnionSend(lpObj);
		}
	}
}

void CGuild::DGGuildMemberUpdateRecv(SDHP_GUILD_MEMBER_UPDATE_RECV* lpMsg) // OK
{
	LPOBJ lpObj = gObjFind(lpMsg->Member);

	if(lpObj != 0)
	{
		if(gServerInfo.m_ServerType == 1)
		{
			gCastleSiege.GetCsJoinSide(lpMsg->Name,&lpObj->CsJoinSide,&lpObj->CsGuildInvolved);
		
			gCastleSiege.NotifySelfCsJoinSide(lpObj->Index);
		}

		memcpy(lpObj->GuildName,lpMsg->Name,sizeof(lpObj->GuildName));

		GUILD_INFO* lpGuild = gGuildManager.GetGuild(lpMsg->Name);

		if(lpGuild != 0)
		{
			lpObj->Guild = lpGuild;

			lpObj->GuildIndex = lpObj->Guild->Index;

			lpObj->GuildRank = lpMsg->Status;
			
			gGuildManager.ConnectMember(lpMsg->Name,lpObj->Name,lpObj->Index,lpMsg->Status,lpMsg->Server);

			if(strlen(lpObj->Guild->Notice) > 0)
			{
				gNotice.GCNoticeSend(lpObj->Index,2,0,0,0,0,0,"%s",lpObj->Guild->Notice);
			}

			gViewport.GCViewportSimpleGuildSend(lpObj);

			if(lpObj->Guild->WarState != GUILD_WAR_STATE_NONE)
			{
				this->GCGuildWarScoreSend(lpObj->Index);
				
				this->GCGuildWarDeclareSend(lpObj->Index,lpObj->Guild->WarEnemy->Name,lpObj->Guild->WarType,lpObj->Guild->WarTeam);
			}
		}
		else
		{
			if(gServerInfo.m_ServerType == 1)
			{
				lpObj->CsJoinSide = 0;
				lpObj->CsGuildInvolved = 0;
			}
		}
	}

	gGuildManager.SetMemberServer(lpMsg->Name,lpMsg->Member,lpMsg->Server);
}

void CGuild::DGGuildAssignStatusRecv(SDHP_GUILD_ASSIGN_STATUS_RECV* lpMsg) // OK
{
	GUILD_INFO* lpGuild = gGuildManager.GetGuild(lpMsg->Name);

	if(lpGuild != 0)
	{
		LPOBJ lpObj = gObjFind(lpGuild->Master);

		if(lpObj != 0)
		{
			this->GCGuildAssignStatusSend(lpObj->Index,lpMsg->Type,lpMsg->result,lpMsg->Member);
		}

		if(lpMsg->result == 1)
		{
			gGuildManager.SetMemberRank(lpMsg->Name,lpMsg->Member,lpMsg->Status);

			LPOBJ lpMember = gObjFind(lpMsg->Member);

			if(lpMember != 0)
			{
				lpMember->GuildRank = lpMsg->Status;

				gViewport.GCViewportSimpleGuildSend(lpMember);
			}

			if(lpMsg->Status == GUILD_RANK_OFFICE_MASTER)
			{
				this->NoticeSendToAll(lpGuild,0,gMessage.GetMessage(190),lpMsg->Member);
			}
			else if(lpMsg->Status == GUILD_RANK_BATTLE_MASTER)
			{
				this->NoticeSendToAll(lpGuild,0,gMessage.GetMessage(191),lpMsg->Member);
			}
			else
			{
				this->NoticeSendToAll(lpGuild,0,gMessage.GetMessage(192),lpMsg->Member);
			}
		}
	}
}

void CGuild::DGGuildAssignTypeRecv(SDHP_GUILD_ASSIGN_TYPE_RECV* lpMsg) // OK
{
	GUILD_INFO* lpGuild = gGuildManager.GetGuild(lpMsg->Name);

	if(lpGuild != 0)
	{
		LPOBJ lpObj = gObjFind(lpGuild->Master);

		if(lpObj != 0)
		{
			this->GCGuildAssignTypeSend(lpObj->Index,lpMsg->Type,lpMsg->result);
		}

		if(lpMsg->result == 1)
		{
			gGuildManager.SetGuildType(lpMsg->Name,lpMsg->Type);

			this->NoticeSendToAll(lpGuild,0,gMessage.GetMessage(193),lpMsg->Name,lpMsg->Type);

			for(int n=0;n < MAX_GUILD_MEMBER;n++)
			{
				if(lpGuild->Member[n].IsValid() != 0)
				{
					gViewport.GCViewportSimpleGuildSend(&gObj[lpGuild->Member[n].Index]);
				}
			}
		}
	}
}

void CGuild::DGGuildNoticeRecv(SDHP_GUILD_NOTICE_RECV* lpMsg) // OK
{
	GUILD_INFO* lpGuild = gGuildManager.GetGuild(lpMsg->Name);

	if(lpGuild != 0)
	{
		memcpy(lpGuild->Notice,lpMsg->Notice,sizeof(lpGuild->Notice));

		this->NoticeSendToAll(lpGuild,2,"%s",lpMsg->Notice);
	}
}

void CGuild::DGGuildPostRecv(SDHP_GUILD_POST_RECV* lpMsg) // OK
{
	GUILD_INFO* lpGuild = gGuildManager.GetGuild(lpMsg->Name);

	if(lpGuild != 0)
	{
		PMSG_CHAT_SEND pMsg;

		pMsg.header.set(0x00,sizeof(pMsg));

		memcpy(pMsg.name,lpMsg->Member,sizeof(pMsg.name));

		memcpy(pMsg.message,lpMsg->Message,sizeof(pMsg.message));

		if(lpMsg->Type == 0)
		{
			this->DataSendToAll(lpGuild,(BYTE*)&pMsg,pMsg.header.size);
		}
		else
		{
			int UnionCount = 0;

			int UnionGuildList[100] = {0};

			if(gUnionManager.GetGuildUnionMemberList(lpGuild->GuildUnion,UnionCount,UnionGuildList) != 0)
			{
				for(int i=0;i < UnionCount;i++)
				{
					GUILD_INFO* lpTarget = gGuildManager.GetGuild(UnionGuildList[i]);

					this->DataSendToAll(lpTarget,(BYTE*)&pMsg,pMsg.header.size);
				}
			}
		}
	}
}

void CGuild::DGGuildScoreRecv(SDHP_GUILD_SCORE_RECV* lpMsg) // OK
{
	GUILD_INFO* lpGuild = gGuildManager.GetGuild(lpMsg->Name);

	if(lpGuild != 0)
	{
		lpGuild->Score = lpMsg->Score;
	}
}

void CGuild::DGGuildInfoRecv(SDHP_GUILD_INFO_RECV* lpMsg) // OK
{
	gGuildManager.AddGuild(lpMsg->Index,lpMsg->Name,lpMsg->Mark,lpMsg->Master,lpMsg->Notice,lpMsg->Score);
	
	gGuildManager.SetGuildType(lpMsg->Name,lpMsg->type);

	GUILD_INFO*lpGuild = gGuildManager.GetGuild(lpMsg->Index);

	if(lpGuild != 0)
	{
		lpGuild->GuildUnion = lpMsg->Union;

		lpGuild->GuildRival = lpMsg->Rival;

		for(int n=0;n < MAX_GUILD_MEMBER;n++)
		{
			if(lpGuild->Member[n].IsValid() != 0)
			{
				gViewport.GCViewportSimpleGuildSend(&gObj[lpGuild->Member[n].Index]);
			}
		}
	}

	for(int n=0;n < lpMsg->count;n++)
	{
		SDHP_GUILD_INFO* lpInfo = (SDHP_GUILD_INFO*)(((BYTE*)lpMsg)+sizeof(SDHP_GUILD_INFO_RECV)+(sizeof(SDHP_GUILD_INFO)*n));

		if(gGuildManager.AddMember(lpMsg->Name,lpInfo->Name,-1,lpInfo->Status,lpInfo->Server) == 0)
		{
			break;
		}
	}
}

void CGuild::DGUnionInfoRecv(SDHP_UNION_INFO_RECV*lpMsg)
{
	if(lpMsg->relation == GUILD_RELATION_UNION)
	{
		gUnionManager.AddUnion(lpMsg->Index,lpMsg->Name);
		gUnionManager.SetGuildUnionMemberList(lpMsg->Index,lpMsg->count,lpMsg->list);
	}
	else if(lpMsg->relation == GUILD_RELATION_RIVAL)
	{
		gUnionManager.AddUnion(lpMsg->Index,lpMsg->Name);
		gUnionManager.SetGuildRivalMemberList(lpMsg->Index,lpMsg->count,lpMsg->list);
	}
}

void CGuild::DGUnionUpdateRecv(SDHP_UNION_UPDATE_RECV* lpMsg) // OK
{
	if(lpMsg->code == 16)
	{
		GUILD_INFO* lpGuild = gGuildManager.GetGuild(lpMsg->list[0]);

		if(lpGuild != 0)
		{
			lpGuild->GuildUnion = GUILD_RELATION_NONE;

			this->NoticeSendToAll(lpGuild,1,gMessage.GetMessage(201),lpGuild->Name);
		}
	}
}

void CGuild::DGGuildUnionInfoRecv(SDHP_GUILD_UNION_INFO_RECV* lpMsg) // OK
{
	LPOBJ lpObj = gObjFind(lpMsg->Member);

	if(lpObj != 0)
	{
		BYTE send[4096];

		PMSG_GUILD_UNION_LIST_SEND pMsg;

		pMsg.header.set(0xE9,0);

		int size = sizeof(pMsg);

		pMsg.count = 0;

		pMsg.result = lpMsg->result;
	
		pMsg.UnionNumber = lpMsg->Union;

		pMsg.RivalNumber = lpMsg->Rival;

		PMSG_GUILD_UNION_LIST info;

		for(int n=0;n < lpMsg->count;n++)
		{
			SDHP_GUILD_UNION_INFO* lpInfo = (SDHP_GUILD_UNION_INFO*)(((BYTE*)lpMsg)+sizeof(SDHP_GUILD_UNION_INFO_RECV)+(sizeof(SDHP_GUILD_UNION_INFO)*n));

			info.count = lpInfo->count;

			memcpy(info.Mark,lpInfo->Mark,sizeof(info.Mark));

			memcpy(info.GuildName,lpInfo->Name,sizeof(info.GuildName));

			memcpy(&send[size],&info,sizeof(info));

			size += sizeof(info);

			pMsg.count++;
		}

		pMsg.header.size[0] = SET_NUMBERHB(size);

		pMsg.header.size[1] = SET_NUMBERLB(size);

		memcpy(send,&pMsg,sizeof(pMsg));

		DataSend(lpObj->Index,send,size);
	}
}

void CGuild::DGGuildUnionJoinRecv(SDHP_GUILD_UNION_JOIN_RECV* lpMsg) // OK
{
	GUILD_INFO* lpGuildA = gGuildManager.GetGuild(lpMsg->GuildNumber[0]);

	GUILD_INFO* lpGuildB = gGuildManager.GetGuild(lpMsg->GuildNumber[1]);

	if(lpMsg->flag == 1)
	{
		if(lpGuildA == 0 || lpGuildB == 0)
		{
			lpMsg->result = 0;
		}

		this->GCGuildUnionResultSend(lpMsg->index[0],lpMsg->relation,1,lpMsg->result,lpMsg->index[1]);

		this->GCGuildUnionResultSend(lpMsg->index[1],lpMsg->relation,1,lpMsg->result,lpMsg->index[0]);
	}
	else
	{
		if(lpGuildA == 0 || lpGuildB == 0)
		{
			return;
		}
	}

	if (lpMsg->result == 0 || lpMsg->result == 16)
	{
		return;
	}

	if(lpMsg->relation == GUILD_RELATION_UNION)
	{
		lpGuildA->GuildUnion = lpGuildB->Index;
		
		lpGuildB->GuildUnion = lpGuildB->Index;
	}
	else if(lpMsg->relation == GUILD_RELATION_RIVAL)
	{
		lpGuildA->GuildRival = lpGuildB->Index;
		
		lpGuildB->GuildRival = lpGuildA->Index;
	}

	if(lpMsg->relation == GUILD_RELATION_UNION)
	{
		this->NoticeSendToAll(lpGuildA,1,gMessage.GetMessage(194),lpGuildB->Name);

		this->NoticeSendToAll(lpGuildB,1,gMessage.GetMessage(196),lpGuildA->Name,lpGuildB->Name);
	}
	else if(lpMsg->relation == GUILD_RELATION_RIVAL)
	{
		this->NoticeSendToAll(lpGuildA,1,gMessage.GetMessage(195),lpGuildB->Name);
		
		this->NoticeSendToAll(lpGuildB,1,gMessage.GetMessage(195),lpGuildA->Name);
	}
}

void CGuild::DGGuildUnionBreakRecv(SDHP_GUILD_UNION_BREAK_RECV* lpMsg) // OK
{
	GUILD_INFO* lpGuildA = gGuildManager.GetGuild(lpMsg->GuildNumber[0]);

	GUILD_INFO* lpGuildB = gGuildManager.GetGuild(lpMsg->GuildNumber[1]);

	if(lpMsg->flag == 1)
	{
		if(lpMsg->relation == GUILD_RELATION_UNION)
		{
			if(lpGuildA == 0)
			{
				lpMsg->result = 0;
			}

			this->GCGuildUnionResultSend(lpMsg->index[0],lpMsg->relation,2,lpMsg->result,lpMsg->index[1]);
		}
		else if(lpMsg->relation == GUILD_RELATION_RIVAL)
		{
			if(lpGuildA == 0 || lpGuildB == 0)
			{
				lpMsg->result = 0;
			}

			this->GCGuildUnionResultSend(lpMsg->index[0],lpMsg->relation,2,lpMsg->result,lpMsg->index[1]);

			this->GCGuildUnionResultSend(lpMsg->index[1],lpMsg->relation,2,lpMsg->result,lpMsg->index[0]);
		}
	}
	else
	{
		if(lpMsg->relation == GUILD_RELATION_UNION && lpGuildA == 0)
		{
			return;
		}
		else if(lpMsg->relation == GUILD_RELATION_RIVAL && (lpGuildA == 0 || lpGuildB == 0))
		{
			return;
		}
	}

	if(lpMsg->result == 0 || lpMsg->result == 16)
	{
		return;
	}

	if(lpMsg->relation == GUILD_RELATION_UNION)
	{
		lpGuildA->GuildUnion = 0;
	}
	else if(lpMsg->relation == GUILD_RELATION_RIVAL)
	{
		lpGuildA->GuildRival = 0;

		lpGuildB->GuildRival = 0;
	}

	if (lpMsg->relation == GUILD_RELATION_UNION)
	{
		this->NoticeSendToAll(lpGuildA,1,gMessage.GetMessage(197));
	}
	else if(lpMsg->relation == GUILD_RELATION_RIVAL)
	{
		this->NoticeSendToAll(lpGuildA,1,gMessage.GetMessage(200),lpGuildB->Name);

		this->NoticeSendToAll(lpGuildB,1,gMessage.GetMessage(200),lpGuildA->Name);
	}
}

void CGuild::DGGuildAllianceKickRecv(SDHP_GUILD_ALLIANCE_KICK_RECV* lpMsg) // OK
{
	GUILD_INFO* lpGuildA = gGuildManager.GetGuild(lpMsg->GuildNameA);

	GUILD_INFO* lpGuildB = gGuildManager.GetGuild(lpMsg->GuildNameB);

	if(lpMsg->flag == 1)
	{
		if(lpGuildA == 0)
		{
			lpMsg->result = 0;
		}

		PMSG_GUILD_ALLIANCE_KICK_RESULT_SEND pMsg;

		pMsg.header.set(0xEB,0x01,sizeof(pMsg));
		
		pMsg.result = lpMsg->result;
		
		pMsg.type = 2;

		pMsg.relation = GUILD_RELATION_UNION;

		DataSend(lpMsg->index,(BYTE*)&pMsg,sizeof(pMsg));
	}
	else
	{
		if(lpGuildB == 0)
		{
			return;
		}
	}

	if(lpMsg->result == 0 || lpMsg->result == 16)
	{
		return;
	}

	if(lpGuildB != 0)
	{
		lpGuildB->GuildUnion = 0;
	}

	this->NoticeSendToAll(lpGuildA,1,gMessage.GetMessage(198),lpMsg->GuildNameA,lpMsg->GuildNameB);

	this->NoticeSendToAll(lpGuildB,1,gMessage.GetMessage(197));
}

void CGuild::GDGuildMemberJoinSend(int aIndex,char* GuildName,char* Name) // OK
{
	SDHP_GUILD_MEMBER_ADD_SEND pMsg;

	pMsg.header.set(0xA0,0x02,sizeof(pMsg));

	memcpy(pMsg.Name,GuildName,sizeof(pMsg.Name));

	memcpy(pMsg.Member,Name,sizeof(pMsg.Member));

	pMsg.Server = gServerInfo.m_ServerCode;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);
}

void CGuild::GDGuildMemberBreakSend(int aIndex,char* GuildName,char* Name) // OK
{
	SDHP_GUILD_MEMBER_DEL_SEND pMsg;

	pMsg.header.set(0xA0,0x03,sizeof(pMsg));

	memcpy(pMsg.Name,GuildName,sizeof(pMsg.Name));

	memcpy(pMsg.Member,Name,sizeof(pMsg.Member));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);
}

void CGuild::GDGuildScoreUpdateSend(char* GuildName,int score) // OK
{
	SDHP_GUILD_SCORE_SEND pMsg;

	pMsg.header.set(0xA0,0x08,sizeof(pMsg));

	memcpy(pMsg.Name,GuildName,sizeof(pMsg.Name));

	pMsg.Score = score;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,-1);
}

void CGuild::GDGuildGlobalChatSend(int type,char* GuildName,char* Name,char* message) // OK
{
	SDHP_GUILD_POST_SEND pMsg;

	pMsg.header.set(0xA0,0x07,sizeof(pMsg));

	memcpy(pMsg.Name,GuildName,sizeof(pMsg.Name));

	memcpy(pMsg.Member,Name,sizeof(pMsg.Member));

	pMsg.Type = type;

	memcpy(pMsg.Message,message,sizeof(pMsg.Message));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,-1);
}

void CGuild::GDGuildGlobalNoticeSend(char* GuildName,char* message) // OK
{
	SDHP_GUILD_NOTICE_SEND pMsg;

	pMsg.header.set(0xA0,0x06,sizeof(pMsg));

	memcpy(pMsg.Name,GuildName,sizeof(pMsg.Name));

	memcpy(pMsg.Notice,message,sizeof(pMsg.Notice));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,-1);
}

void CGuild::CGGuildRequestRecv(PMSG_GUILD_REQUEST_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	int bIndex = MAKE_NUMBERW(lpMsg->index[0],lpMsg->index[1]);

	if(gObjIsConnectedGP(bIndex) == 0)
	{
		return;
	}

	LPOBJ lpTarget = &gObj[bIndex];

	if((lpTarget->Option & 1) == 0)
	{
		this->GCGuildResultSend(aIndex,0);
		return;
	}

	if(lpTarget->GuildIndex == 0)
	{
		this->GCGuildResultSend(aIndex,4);
		return;
	}

	if(lpTarget->GuildRank != GUILD_RANK_MASTER)
	{
		this->GCGuildResultSend(aIndex,4);
		return;
	}

	if(lpObj->GuildIndex != 0)
	{
		this->GCGuildResultSend(aIndex,5);
		return;
	}

	if(lpObj->Interface.use != 0 || lpTarget->Interface.use != 0)
	{
		this->GCGuildResultSend(aIndex,6);
		return;
	}

	if(lpTarget->Guild->WarState != 0)
	{
		this->GCGuildResultSend(aIndex,6);
		return;
	}

	if(gCastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
	{
		this->GCGuildResultSend(aIndex,6);
		return;
	}

	if(lpObj->Level < 6)
	{
		this->GCGuildResultSend(aIndex,7);
		return;
	}

	if(lpTarget->Guild->GetMemberCount() >= this->GetMaxGuildUser(lpTarget))
	{
		this->GCGuildResultSend(aIndex,2);
		return;
	}

	#if(GAMESERVER_UPDATE>=501)

	if(gServerInfo.m_GensSystemGuildLock != 0 && gObj[aIndex].GensFamily == GENS_FAMILY_NONE)
	{
		this->GCGuildResultSend(aIndex,163);
		return;
	}

	if(gServerInfo.m_GensSystemGuildLock != 0 && gObj[bIndex].GensFamily == GENS_FAMILY_NONE)
	{
		this->GCGuildResultSend(aIndex,161);
		return;
	}

	if(gServerInfo.m_GensSystemGuildLock != 0 && gObj[aIndex].GensFamily != gObj[bIndex].GensFamily)
	{
		this->GCGuildResultSend(aIndex,162);
		return;
	}

	#endif

	if((lpTarget->Helper.Started != 0 || lpTarget->Helper.Offline != 0) && lpTarget->Helper.AutoAcceptGuild != 0)
	{
		gGuildMatching.GDGuildMatchingJoinCancelSend(aIndex,1);

		this->GDGuildMemberJoinSend(aIndex,lpTarget->Guild->Name,lpObj->Name);

		return;
	}

	lpObj->Interface.use = 1;
	lpObj->Interface.type = INTERFACE_GUILD;
	lpObj->Interface.state = 0;
	lpObj->InterfaceTime = GetTickCount();
	lpObj->TargetNumber = bIndex;

	lpTarget->Interface.use = 1;
	lpTarget->Interface.type = INTERFACE_GUILD;
	lpTarget->Interface.state = 0;
	lpTarget->InterfaceTime = GetTickCount();
	lpTarget->TargetNumber = aIndex;

	PMSG_GUILD_REQUEST_SEND pMsg;

	pMsg.header.set(0x50,sizeof(pMsg));

	pMsg.index[0] = SET_NUMBERHB(aIndex);
	pMsg.index[1] = SET_NUMBERLB(aIndex);

	DataSend(bIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void CGuild::CGGuildResultRecv(PMSG_GUILD_RESULT_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	int bIndex = MAKE_NUMBERW(lpMsg->index[0],lpMsg->index[1]);

	if(gObjIsConnectedGP(bIndex) == 0)
	{
		return;
	}

	LPOBJ lpTarget = &gObj[bIndex];

	if(lpObj->Interface.use == 0 || lpObj->Interface.type != INTERFACE_GUILD)
	{
		return;
	}

	if(lpTarget->Interface.use == 0 || lpTarget->Interface.type != INTERFACE_GUILD)
	{
		return;
	}

	if(lpMsg->result == 0)
	{
		this->GCGuildResultSend(bIndex,0);
		goto CLEAR_JUMP;
	}

	if((lpObj->Option & 1) == 0)
	{
		this->GCGuildResultSend(bIndex,0);
		goto CLEAR_JUMP;
	}

	if(lpObj->GuildIndex == 0)
	{
		this->GCGuildResultSend(bIndex,4);
		goto CLEAR_JUMP;
	}

	if(lpObj->GuildRank != GUILD_RANK_MASTER)
	{
		this->GCGuildResultSend(bIndex,4);
		goto CLEAR_JUMP;
	}

	if(lpTarget->GuildIndex != 0)
	{
		this->GCGuildResultSend(bIndex,5);
		goto CLEAR_JUMP;
	}

	if(lpObj->Guild->WarState != 0)
	{
		this->GCGuildResultSend(bIndex,6);
		goto CLEAR_JUMP;
	}

	if(gCastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
	{
		this->GCGuildResultSend(bIndex,6);
		goto CLEAR_JUMP;
	}

	if(lpTarget->Level < 6)
	{
		this->GCGuildResultSend(bIndex,7);
		goto CLEAR_JUMP;
	}

	if(lpObj->Guild->GetMemberCount() >= this->GetMaxGuildUser(lpObj))
	{
		this->GCGuildResultSend(bIndex,2);
		goto CLEAR_JUMP;
	}

	#if(GAMESERVER_UPDATE>=501)

	if(gServerInfo.m_GensSystemGuildLock != 0 && gObj[aIndex].GensFamily == GENS_FAMILY_NONE)
	{
		this->GCGuildResultSend(aIndex,163);
		goto CLEAR_JUMP;
	}

	if(gServerInfo.m_GensSystemGuildLock != 0 && gObj[bIndex].GensFamily == GENS_FAMILY_NONE)
	{
		this->GCGuildResultSend(aIndex,161);
		goto CLEAR_JUMP;
	}

	if(gServerInfo.m_GensSystemGuildLock != 0 && gObj[aIndex].GensFamily != gObj[bIndex].GensFamily)
	{
		this->GCGuildResultSend(aIndex,162);
		goto CLEAR_JUMP;
	}

	#endif

	gGuildMatching.GDGuildMatchingJoinCancelSend(bIndex,1);

	this->GDGuildMemberJoinSend(bIndex,lpObj->Guild->Name,lpTarget->Name);

	CLEAR_JUMP:

	lpObj->Interface.use = 0;
	lpObj->Interface.type = INTERFACE_NONE;
	lpObj->Interface.state = 0;
	lpObj->InterfaceTime = 0;
	lpObj->TargetNumber = -1;

	lpTarget->Interface.use = 0;
	lpTarget->Interface.type = INTERFACE_NONE;
	lpTarget->Interface.state = 0;
	lpTarget->InterfaceTime = 0;
	lpTarget->TargetNumber = -1;
}

void CGuild::CGGuildListRecv(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(lpObj->Guild == 0)
	{
		return;
	}

	BYTE send[2048];

	PMSG_GUILD_LIST_SEND pMsg;

	pMsg.header.set(0x52,0);

	int size = sizeof(pMsg);

	pMsg.result = 1;

	pMsg.count = 0;

	pMsg.TotalScore = lpObj->Guild->Score;

	pMsg.score = lpObj->Guild->WarScore;

	GUILD_INFO* lpRivalGuild = gGuildManager.GetGuild(lpObj->Guild->GuildRival);

	if(lpRivalGuild == 0)
	{
		memset(pMsg.RivalGuild,0,sizeof(pMsg.RivalGuild));

		#if(GAMESERVER_UPDATE>=701)

		pMsg.RivalCount = 0;

		memset(pMsg.RivalMark,0,sizeof(pMsg.RivalMark));

		#endif
	}
	else
	{
		memcpy(pMsg.RivalGuild,lpRivalGuild->Name,sizeof(pMsg.RivalGuild));

		#if(GAMESERVER_UPDATE>=701)

		pMsg.RivalCount = lpRivalGuild->GetMemberCount();

		memcpy(pMsg.RivalMark,lpRivalGuild->Mark,sizeof(pMsg.RivalMark));

		#endif
	}

	PMSG_GUILD_LIST info;

	for(int n=0;n < MAX_GUILD_MEMBER;n++)
	{
		if(lpObj->Guild->Member[n].IsEmpty() != 0)
		{
			continue;
		}

		memcpy(info.name,lpObj->Guild->Member[n].Name,sizeof(info.name));

		info.number = (BYTE)(lpObj->Guild->Member[n].Server);

		info.connected = (lpObj->Guild->Member[n].Server & 0x7F) | ((lpObj->Guild->Member[n].Server!=-1)?0x80:0x00);

		info.status = lpObj->Guild->Member[n].Rank;

		memcpy(&send[size],&info,sizeof(info));
		size += sizeof(info);

		pMsg.count++;
	}

	if(pMsg.count > 0)
	{
		pMsg.header.size[0] = SET_NUMBERHB(size);
		pMsg.header.size[1] = SET_NUMBERLB(size);

		memcpy(send,&pMsg,sizeof(pMsg));

		DataSend(aIndex,send,size);
	}
}

void CGuild::CGGuildDeleteRecv(PMSG_GUILD_DELETE_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(gServerInfo.m_GuildDeleteSwitch == 0)
	{
		this->GCGuildDeleteSend(aIndex,0);
		return;
	}

	if(lpObj->GuildIndex == 0)
	{
		this->GCGuildDeleteSend(aIndex,0);
		return;
	}

	if(gCastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
	{
		this->GCGuildDeleteSend(aIndex,0);
		return;
	}

	char name[11] = {0};

	char PersonalCode[14] = {0};

	memcpy(name,lpMsg->name,sizeof(lpMsg->name));

	memcpy(PersonalCode,lpMsg->PersonalCode,sizeof(lpMsg->PersonalCode));

	if(lpObj->GuildRank == GUILD_RANK_MASTER)
	{
		if(strcmp(lpObj->Name,name) != 0)
		{
			if(gObjCheckPersonalCode(aIndex,PersonalCode) == 0)
			{
				this->GCGuildDeleteSend(aIndex,0);
				return;
			}
			else
			{
				GDGuildMemberBreakSend(aIndex,lpObj->Guild->Name,name);
				return;
			}
		}

		if(gObjCheckPersonalCode(aIndex,PersonalCode) == 0)
		{
			this->GCGuildDeleteSend(aIndex,0);
			return;
		}

		if(gServerInfo.m_GuildOwnerDestroyLimit != 0 && gCastleSiegeSync.CheckCastleOwnerMember(aIndex) != 0)
		{
			this->GCGuildDeleteSend(aIndex,0);
			return;
		}

		gObjGuildWarMasterClose(lpObj);

		gGuildMatching.GDGuildMatchingCancelSend(aIndex,1);

		SDHP_GUILD_DELETE_SEND pMsg;

		pMsg.header.set(0xA0,0x01,sizeof(pMsg));

		memcpy(pMsg.Name,lpObj->Guild->Name,sizeof(pMsg.Name));

		memcpy(pMsg.Member,name,sizeof(pMsg.Member));

		gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);
	}
	else
	{
		if(strcmp(lpObj->Name,name) != 0)
		{
			this->GCGuildDeleteSend(aIndex,0);
			return;
		}

		if(gObjCheckPersonalCode(aIndex,PersonalCode) == 0)
		{
			this->GCGuildDeleteSend(aIndex,0);
			return;
		}

		GDGuildMemberBreakSend(aIndex,lpObj->Guild->Name,name);
	}
}

void CGuild::CGGuildMasterOpenRecv(PMSG_GUILD_MASTER_OPEN_RECV*lpMsg, int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(lpMsg->result == 1)
	{
		if(lpObj->Level < gServerInfo.m_GuildCreateMinLevel[lpObj->AccountLevel])
		{
			goto CLEAR_JUMP;
		}

		if(lpObj->Reset < gServerInfo.m_GuildCreateMinReset[lpObj->AccountLevel])
		{
			goto CLEAR_JUMP;
		}

		PBMSG_HEAD pMsg;

		pMsg.set(0x55,sizeof(pMsg));

		DataSend(aIndex,(BYTE*)&pMsg,pMsg.size); 

		return;
	}
	else
	{
		goto CLEAR_JUMP;
	}

	CLEAR_JUMP:

	if(lpObj->Interface.use != 0 && lpObj->Interface.type == INTERFACE_GUILD_CREATE)
	{
		lpObj->Interface.use = 0;
	}
}

void CGuild::CGGuildCreateRecv(PMSG_GUILD_CREATE_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	char GuildName[9] = {0};

	memcpy(GuildName,lpMsg->GuildName,sizeof(lpMsg->GuildName));

	if(strlen(GuildName) < 3 || strlen(GuildName) > 9)
	{
		this->GCGuildCreateResultSend(aIndex,2,0);
	}
	else
	{
		gGuildMatching.GDGuildMatchingJoinCancelSend(aIndex,1);

		SDHP_GUILD_CREATE_SEND pMsg;

		pMsg.header.set(0xA0,0x00,sizeof(pMsg));

		memcpy(pMsg.Name,GuildName,sizeof(pMsg.Name));

		memcpy(pMsg.Master,lpObj->Name,sizeof(pMsg.Master));

		memcpy(pMsg.Mark,lpMsg->Mark,sizeof(pMsg.Mark));

		pMsg.type = lpMsg->type;

		pMsg.Server = gServerInfo.m_ServerCode;

		gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);
	}

	if(lpObj->Interface.use != 0 && lpObj->Interface.type == INTERFACE_GUILD_CREATE)
	{
		lpObj->Interface.use = 0;
	}
}

void CGuild::CGGuildMasterCancelRecv(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(lpObj->Guild != 0)
	{
		return;
	}

	if(lpObj->Interface.use != 0 && lpObj->Interface.type == INTERFACE_GUILD_CREATE)
	{
		lpObj->Interface.use = 0;
	}
}

void CGuild::CGGuildWarRequestResultRecv(PMSG_GUILD_WAR_REQUEST_RESULT_RECV* lpMsg, int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	GUILD_INFO* lpGuild1 = lpObj->Guild;

	if(lpGuild1 == 0)
	{
		this->GCGuildWarResultSend(aIndex,0);
		return;
	}

	GUILD_INFO* lpGuild2 = lpGuild1->WarEnemy;

	if(lpGuild2 != 0 && lpMsg->result != 0)
	{
		LPOBJ lpTarget = &gObj[lpGuild2->Member[0].Index];

		#if(GAMESERVER_UPDATE>=301)

		if(gCustomArena.CheckMap(lpTarget->Map) != 0 || BC_MAP_RANGE(lpTarget->Map) != 0 || CC_MAP_RANGE(lpTarget->Map) != 0 || DS_MAP_RANGE(lpTarget->Map) != 0 || IT_MAP_RANGE(lpTarget->Map) != 0)
		{
			lpMsg->result = 0;
		}

		#else

		if(gCustomArena.CheckMap(lpTarget->Map) != 0 || BC_MAP_RANGE(lpTarget->Map) != 0 || CC_MAP_RANGE(lpTarget->Map) != 0 || DS_MAP_RANGE(lpTarget->Map) != 0)
		{
			lpMsg->result = 0;
		}

		#endif
	}

	if(lpMsg->result == 0)
	{
		if(lpGuild2 != 0)
		{
			if(lpGuild2->WarDeclare == GUILD_WAR_STATE_DECLARE)
			{
				lpGuild1->WarDeclare = GUILD_WAR_STATE_NONE;

				lpGuild2->WarDeclare = GUILD_WAR_STATE_NONE;

				if(lpGuild2->Member[0].Index >= 0)
				{
					this->GCGuildWarResultSend(lpGuild2->Member[0].Index,6);
				}
			}
		}
	}
	else
	{
		if(lpGuild1->WarDeclare == GUILD_WAR_STATE_DECLARE)
		{
			if(lpGuild2 != 0)
			{
				if(lpGuild2->WarDeclare == GUILD_WAR_STATE_DECLARE)
				{
					if(lpGuild1->WarType == GUILD_WAR_TYPE_SOCCER)
					{
						if(gBattleSoccer.GetState() != BS_STATE_EMPTY)
						{
							lpGuild1->WarDeclare = GUILD_WAR_STATE_NONE;
							lpGuild1->WarState = GUILD_WAR_STATE_NONE;

							lpGuild2->WarDeclare = GUILD_WAR_STATE_NONE;
							lpGuild2->WarState = GUILD_WAR_STATE_NONE;

							this->GCGuildWarResultSend(aIndex,4);

							return;
						}

						lpGuild1->WarScore = 0;
						lpGuild2->WarScore = 0;
						
						gBattleSoccer.SetSoccerInfo(lpGuild1,lpGuild2);
					}

					lpGuild1->WarDeclare = GUILD_WAR_STATE_ACTIVE;
					lpGuild1->WarState = GUILD_WAR_STATE_DECLARE;
					lpGuild1->WarScore = 0;
					lpGuild1->WarTeam = GUILD_WAR_TEAM_BLUE;

					lpGuild2->WarDeclare = GUILD_WAR_STATE_ACTIVE;
					lpGuild2->WarState = GUILD_WAR_STATE_DECLARE;
					lpGuild2->WarScore = 0;
					lpGuild2->WarTeam = GUILD_WAR_TEAM_RED;

					for(int n=0;n < MAX_GUILD_MEMBER;n++)
					{
						if(lpGuild1->Member[n].IsValid() != 0)
						{
							if(n > 0 && lpGuild1->WarType == 1)
							{
								if(gObj[lpGuild1->Member[0].Index].PartyNumber >= 0 && gObj[lpGuild1->Member[0].Index].PartyNumber == gObj[lpGuild1->Member[n].Index].PartyNumber)
								{
									this->GuildWarDeclareSet(lpGuild1->Member[n].Index,lpGuild2->Name,lpGuild1->WarType,lpGuild1->WarTeam);
								}
							}
							else
							{
								this->GuildWarDeclareSet(lpGuild1->Member[n].Index,lpGuild2->Name,lpGuild1->WarType,lpGuild1->WarTeam);
							}
						}
					}

					for(int n=0;n < MAX_GUILD_MEMBER;n++)
					{
						if(lpGuild2->Member[n].IsValid() != 0)
						{
							if(n > 0 && lpGuild1->WarType == 1)
							{
								if(gObj[lpGuild2->Member[0].Index].PartyNumber >= 0 && gObj[lpGuild2->Member[0].Index].PartyNumber == gObj[lpGuild2->Member[n].Index].PartyNumber)
								{
									this->GuildWarDeclareSet(lpGuild2->Member[n].Index,lpGuild1->Name,lpGuild2->WarType,lpGuild2->WarTeam);
								}
							}
							else 
							{
								this->GuildWarDeclareSet(lpGuild2->Member[n].Index,lpGuild1->Name,lpGuild2->WarType,lpGuild2->WarTeam);
							}
						}
					}

					if(lpGuild1->WarType == GUILD_WAR_TYPE_SOCCER)
					{
						gObjAddMsgSendDelay(&gObj[aIndex],5,aIndex,10000,0);

						this->NoticeSendToAll(lpGuild1,1,gMessage.GetMessage(287));

						this->NoticeSendToAll(lpGuild2,1,gMessage.GetMessage(287));					
					}
				}
			}
		}
	}
}

void CGuild::CGGuildViewportRequestRecv(PMSG_GUILD_VIEWPORT_REQUEST_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	GUILD_INFO* lpInfo = gGuildManager.GetGuild(lpMsg->GuildNumber);

	if(lpInfo == 0)
	{
		return;
	}

	PMSG_GUILD_VIEWPORT_SEND pMsg;

	pMsg.header.set(0x66,sizeof(pMsg));

	pMsg.GuildNumber = lpInfo->Index;

	pMsg.type = lpInfo->Type;

	CUnionInfo* lpUnionInfo = gUnionManager.SearchUnion(lpInfo->GuildUnion);

	if(lpUnionInfo == 0)
	{
		memset(pMsg.UnionName,0,sizeof(pMsg.UnionName));
	}
	else
	{
		memcpy(pMsg.UnionName,lpUnionInfo->m_szMasterGuild,sizeof(pMsg.UnionName));
	}

	memcpy(pMsg.GuildName,lpInfo->Name,sizeof(pMsg.GuildName));

	memcpy(pMsg.Mark,lpInfo->Mark,sizeof(pMsg.Mark));

	DataSend(aIndex,(BYTE*)&pMsg,sizeof(pMsg));
}

void CGuild::CGGuildAssignStatusRecv(PMSG_GUILD_ASSIGN_STATUS_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(lpMsg->type != 1 && lpMsg->type != 2 && lpMsg->type != 3)
	{
		return;
	}

	char name[11] = {0};

	memcpy(name,lpMsg->name,sizeof(lpMsg->name));

	if(lpObj->Guild == 0)
	{
		this->GCGuildAssignStatusSend(aIndex,lpMsg->type,16,name);
		return;
	}

	if(strcmp(lpObj->Name,name) == 0)
	{
		this->GCGuildAssignStatusSend(aIndex,lpMsg->type,17,name);
		return;
	}

	if(this->GetGuildNameCount(lpObj,name) == 0)
	{
		this->GCGuildAssignStatusSend(aIndex,lpMsg->type,17,name);
		return;
	}

	if(lpObj->GuildRank != GUILD_RANK_MASTER)
	{
		this->GCGuildAssignStatusSend(aIndex,lpMsg->type,17,name);
		return;
	}

	if(lpMsg->status != GUILD_RANK_MEMBER && lpMsg->status != GUILD_RANK_BATTLE_MASTER && lpMsg->status != GUILD_RANK_OFFICE_MASTER)
	{
		this->GCGuildAssignStatusSend(aIndex,lpMsg->type,18,name);
		return;
	}

	if(lpMsg->status == GUILD_RANK_BATTLE_MASTER && this->GetGuildBattleMasterCount(lpObj) >= (((lpObj->Level+lpObj->MasterLevel)/200)+1))
	{
		this->GCGuildAssignStatusSend(aIndex,lpMsg->type,18,name);
		return;
	}

	if(lpMsg->status == GUILD_RANK_OFFICE_MASTER && this->GetGuildOfficeMasterCount(lpObj) > 0)
	{
		this->GCGuildAssignStatusSend(aIndex,lpMsg->type,18,name);
		return;
	}

	SDHP_GUILD_ASSIGN_STATUS_SEND pMsg;

	pMsg.header.set(0xA0,0x04,sizeof(pMsg));

	memcpy(pMsg.Name,lpObj->Guild->Name,sizeof(pMsg.Name));

	memcpy(pMsg.Member,name,sizeof(pMsg.Member));

	pMsg.Status = lpMsg->status;

	pMsg.Type = lpMsg->type;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);
}

void CGuild::CGGuildAssignTypeRecv(PMSG_GUILD_ASSIGN_TYPE_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}
	
	if(lpObj->Guild == 0)
	{
		this->GCGuildAssignTypeSend(aIndex,lpMsg->type,16);
		return;
	}

	if(lpObj->GuildRank != GUILD_RANK_MASTER)
	{
		this->GCGuildAssignTypeSend(aIndex,lpMsg->type,17);
		return;
	}
	
	SDHP_GUILD_ASSIGN_TYPE_SEND pMsg;

	pMsg.header.set(0xA0,0x05,sizeof(pMsg));

	memcpy(pMsg.Name,lpObj->Guild->Name,sizeof(pMsg.Name));

	pMsg.Type = lpMsg->type;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);
}

void CGuild::CGGuildUnionRequestRecv(PMSG_GUILD_UNION_REQUEST_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	#if(GAMESERVER_UPDATE>=301)

	if(IT_MAP_RANGE(lpObj->Map) != 0)
	{
		return;
	}

	#endif

	int bIndex = MAKE_NUMBERW(lpMsg->index[0],lpMsg->index[1]);

	if(gObjIsConnectedGP(bIndex) == 0)
	{
		this->GCGuildResultSend(aIndex,3);
		return;
	}

	LPOBJ lpTarget = &gObj[bIndex];

	if((lpTarget->Option & 1) == 0)
	{
		return;
	}

	if(gArcaBattle.GetState() > AB_STATE_EMPTY && gArcaBattle.GetState() < AB_STATE_CLEAN)
	{
		this->GCGuildUnionResultSend(aIndex,lpMsg->relation,lpMsg->type,33,bIndex);
		return;
	}

	if(gCastleSiegeSync.GetCastleState() >= CASTLESIEGE_STATE_NOTIFY && gCastleSiegeSync.GetCastleState() <= CASTLESIEGE_STATE_STARTSIEGE)
	{
		this->GCGuildUnionResultSend(aIndex,lpMsg->relation,lpMsg->type,16,bIndex);
		return;
	}

	if(lpObj->Guild == 0 || lpTarget->Guild == 0)
	{
		this->GCGuildResultSend(aIndex,5);
		return;
	}

	if(lpObj->GuildRank != GUILD_RANK_MASTER && lpTarget->GuildRank != GUILD_RANK_MASTER)
	{
		this->GCGuildResultSend(aIndex,4);
		return;
	}

	if(lpObj->Interface.use != 0 || lpTarget->Interface.use != 0)
	{
		this->GCGuildResultSend(aIndex,6);
		return;
	}

	if(lpObj->GuildRank != GUILD_RANK_MASTER || lpTarget->GuildRank != GUILD_RANK_MASTER)
	{
		this->GCGuildUnionResultSend(aIndex,lpMsg->relation,lpMsg->type,17,bIndex);
		return;
	}

	if(lpMsg->type == 1)
	{
		if(lpMsg->relation == GUILD_RELATION_UNION)
		{
			if(lpTarget->Guild->GetMemberCount() < gServerInfo.m_GuildAllianceMinUser)
			{
				this->GCGuildUnionResultSend(aIndex,lpMsg->relation,lpMsg->type,29,bIndex);
				return;
			}

			if(lpTarget->Guild->GuildUnion != 0 && lpTarget->Guild->GuildUnion == lpTarget->Guild->Index)
			{
				if(gUnionManager.GetGuildRelationShipCount(lpTarget->Guild->GuildUnion,1) >= ((gServerInfo.m_GuildAllianceMaxCount>MAX_UNION_MEMBER)?MAX_UNION_MEMBER:gServerInfo.m_GuildAllianceMaxCount) )
				{
					this->GCGuildUnionResultSend(aIndex,lpMsg->relation,lpMsg->type,30,bIndex);
					return;
				}
			}

			if(strcmp(lpObj->GuildName,gCastleSiegeSync.GetCastleOwnerGuild()) == 0)
			{
				this->GCGuildUnionResultSend(aIndex,lpMsg->relation,lpMsg->type,23,bIndex);
				return;
			}

			if(lpObj->Guild->GuildUnion != 0)
			{
				this->GCGuildUnionResultSend(aIndex,lpMsg->relation,lpMsg->type,23,bIndex);
				return;
			}

			if(gObjGetRelationShip(lpObj,lpTarget) == GUILD_RELATION_RIVAL)
			{
				this->GCGuildUnionResultSend(aIndex,lpMsg->relation,lpMsg->type,22,bIndex);
				return;
			}

			if(lpTarget->Guild->GuildUnion && lpTarget->Guild->Index != lpTarget->Guild->GuildUnion)
			{
				this->GCGuildUnionResultSend(aIndex,lpMsg->relation,lpMsg->type,27,bIndex);
				return;
			}
		}
		else if(lpMsg->relation == GUILD_RELATION_RIVAL)
		{
			if(lpObj->Guild->GuildRival != 0 || lpTarget->Guild->GuildRival != 0)
			{
				this->GCGuildUnionResultSend(aIndex,lpMsg->relation,lpMsg->type,24,bIndex);
				return;
			}

			if(gObjGetRelationShip(lpObj,lpTarget) == GUILD_RELATION_UNION)
			{
				this->GCGuildUnionResultSend(aIndex,lpMsg->relation,lpMsg->type,21,bIndex);
				return;
			}
		}
	}
	else if(lpMsg->type == 2)
	{
		if(lpMsg->relation == GUILD_RELATION_UNION)
		{
			if(lpObj->Guild->GuildUnion == 0)
			{
				this->GCGuildUnionResultSend(aIndex,lpMsg->relation,lpMsg->type,25,bIndex);
				return;
			}
		}
		else if(lpMsg->relation == GUILD_RELATION_RIVAL)
		{
			if(lpObj->Guild->GuildRival == 0 || lpTarget->Guild->GuildRival == 0)
			{
				this->GCGuildUnionResultSend(aIndex,lpMsg->relation,lpMsg->type,25,bIndex);
				return;
			}

			if(lpObj->Guild->GuildRival != lpTarget->Guild->Index || lpTarget->Guild->GuildRival != lpObj->Guild->Index)
			{
				this->GCGuildUnionResultSend(aIndex,lpMsg->relation,lpMsg->type,28,bIndex);
				return;
			}
		}
	}		
	
	if(lpMsg->type == 2 && lpMsg->relation == GUILD_RELATION_UNION)
	{
		SDHP_GUILD_UNION_BREAK_SEND pMsg;

		pMsg.header.set(0xA0,0x0B,sizeof(pMsg));

		pMsg.index[0] = aIndex;

		pMsg.index[1] = -1;

		pMsg.relation = lpMsg->relation;

		pMsg.GuildNumber[0] = lpObj->Guild->Index;

		pMsg.GuildNumber[1] = lpObj->Guild->GuildUnion;

		gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);
	}
	else
	{
		lpObj->Interface.use = 1;
		lpObj->Interface.type = INTERFACE_GUILD_REQUEST;
		lpObj->Interface.state = 0;
		lpObj->InterfaceTime = GetTickCount();
		lpObj->TargetNumber = bIndex;

		lpTarget->Interface.use = 1;
		lpTarget->Interface.type = INTERFACE_GUILD_REQUEST;
		lpTarget->Interface.state = 0;
		lpTarget->InterfaceTime = GetTickCount();
		lpTarget->TargetNumber = aIndex;

		this->GCGuildUnionRequestSend(bIndex,lpMsg->relation,lpMsg->type,aIndex);
	}
}

void CGuild::CGGuildUnionResultRecv(PMSG_GUILD_UNION_RESULT_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	int bIndex = MAKE_NUMBERW(lpMsg->index[0],lpMsg->index[1]);

	if(gObjIsConnectedGP(bIndex) == 0)
	{
		this->GCGuildResultSend(aIndex,3);
		return;
	}

	LPOBJ lpTarget = &gObj[bIndex];

	if(lpMsg->result == 1)
	{
		if(lpMsg->type == 1)
		{
			SDHP_GUILD_UNION_JOIN_SEND pMsg;

			pMsg.header.set(0xA0,0x0A,sizeof(pMsg));

			pMsg.index[0] = bIndex;

			pMsg.index[1] = aIndex;

			pMsg.relation = lpMsg->relation;

			pMsg.GuildNumber[0] = lpTarget->Guild->Index;

			pMsg.GuildNumber[1] = lpObj->Guild->Index;

			gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);
		}
		else if(lpMsg->type == 2)
		{
			SDHP_GUILD_UNION_BREAK_SEND pMsg;

			pMsg.header.set(0xA0,0x0B,sizeof(pMsg));

			pMsg.index[0] = aIndex;

			pMsg.index[1] = bIndex;

			pMsg.relation = lpMsg->relation;

			pMsg.GuildNumber[0] = lpObj->Guild->Index;

			pMsg.GuildNumber[1] = lpTarget->Guild->Index;

			gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);
		}
	}
	else
	{
		this->GCGuildUnionResultSend(bIndex,lpMsg->relation,lpMsg->type,32,bIndex);
	}

	lpObj->Interface.use = 0;
	lpObj->Interface.type = INTERFACE_NONE;
	lpObj->Interface.state = 0;
	lpObj->InterfaceTime = 0;
	lpObj->TargetNumber = -1;

	lpTarget->Interface.use = 0;
	lpTarget->Interface.type = INTERFACE_NONE;
	lpTarget->Interface.state = 0;
	lpTarget->InterfaceTime = 0;
	lpTarget->TargetNumber = -1;
}

void CGuild::CGGuildUnionListRecv(int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		this->GCGuildResultSend(aIndex,3);
		return;
	}

	if(lpObj->Guild == 0)
	{
		this->GCGuildResultSend(aIndex,3);
		return;
	}

	if(lpObj->Guild->GuildUnion == 0)
	{
		this->GCGuildResultSend(aIndex,3);
		return;
	}

	SDHP_GUILD_UNION_GET_SEND pMsg;

	pMsg.header.set(0xA0,0x09,sizeof(pMsg));

	memcpy(pMsg.Member,lpObj->Name,sizeof(pMsg.Member));

	pMsg.Index = lpObj->Guild->GuildUnion;

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);
}

void CGuild::CGGuildAllianceKickRecv(PMSG_GUILD_ALLIANCE_KICK_RECV* lpMsg,int aIndex) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		this->GCGuildResultSend(aIndex,3);
		return;
	}

	if(lpObj->Guild == 0)
	{
		this->GCGuildResultSend(aIndex,3);
		return;
	}

	if(lpObj->Guild->GuildUnion == 0)
	{
		this->GCGuildUnionResultSend(aIndex,GUILD_RELATION_UNION,2,25,0);
		return;
	}

	if(lpObj->GuildRank != GUILD_RANK_MASTER || lpObj->Guild->Index != lpObj->Guild->GuildUnion)
	{
		this->GCGuildUnionResultSend(aIndex,GUILD_RELATION_UNION,2,17,0);
		return;
	}

	char GuildName[9] = {0};

	memcpy(GuildName,lpMsg->GuildName,sizeof(lpMsg->GuildName));

	if(strcmp(lpObj->Guild->Name,GuildName) == 0)
	{
		this->GCGuildUnionResultSend(aIndex,GUILD_RELATION_UNION,2,17,0);
		return;
	}

	SDHP_GUILD_ALLIANCE_KICK_SEND pMsg;

	pMsg.header.set(0xA0,0x0C,sizeof(pMsg));

	pMsg.index = aIndex;

	pMsg.relation = GUILD_RELATION_UNION;

	memcpy(pMsg.GuildNameA,lpObj->Guild->Name,sizeof(pMsg.GuildNameA));

	memcpy(pMsg.GuildNameB,GuildName,sizeof(pMsg.GuildNameB));

	gDataServerConnection.DataSend((BYTE*)&pMsg,pMsg.header.size,aIndex);
}

void CGuild::GCGuildResultSend(int aIndex,int result) // OK
{
	PMSG_GUILD_RESULT_SEND pMsg;

	pMsg.header.set(0x51,sizeof(pMsg));

	pMsg.result = result;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void CGuild::GCGuildDeleteSend(int aIndex,int result) // OK
{
	PMSG_GUILD_DELETE_SEND pMsg;

	pMsg.header.set(0x53,sizeof(pMsg));

	pMsg.result = result;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void CGuild::GCGuildCreateResultSend(int aIndex,int result,int type) // OK
{
	PMSG_GUILD_CREATE_RESULT_SEND pMsg;

	pMsg.header.set(0x56,sizeof(pMsg));

	pMsg.result = result;

	pMsg.type = type;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void CGuild::GCGuildWarResultSend(int aIndex,int result) // OK
{
	PMSG_GUILD_WAR_REQUEST_RESULT_SEND pMsg;

	pMsg.header.set(0x60,sizeof(pMsg));

	pMsg.result = result;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void CGuild::GCGuildWarRequestSend(int aIndex,char* GuildName,int type) // OK
{
	PMSG_GUILD_WAR_REQUEST_SEND pMsg;

	pMsg.header.set(0x61,sizeof(pMsg));

	memcpy(pMsg.GuildName,GuildName,sizeof(pMsg.GuildName));

	pMsg.type = type;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void CGuild::GCGuildWarDeclareSend(int aIndex,char* GuildName,int type,int team) // OK
{
	PMSG_GUILD_WAR_START_SEND pMsg;

	pMsg.header.set(0x62,sizeof(pMsg));

	memcpy(pMsg.GuildName,GuildName,sizeof(pMsg.GuildName));

	pMsg.type = type;

	pMsg.team = team;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void CGuild::GCGuildWarEndSend(int aIndex,int result,char* GuildName) // OK
{
	PMSG_GUILD_WAR_END_SEND pMsg;

	pMsg.header.set(0x63,sizeof(pMsg));

	pMsg.result = result;

	memcpy(pMsg.GuildName,GuildName,sizeof(pMsg.GuildName));

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void CGuild::GCGuildWarScoreSend(int aIndex) // OK
{
	PMSG_GUILD_WAR_SCORE_SEND pMsg;

	pMsg.header.set(0x64,sizeof(pMsg));

	pMsg.score[0] = ((gObj[aIndex].Guild==0)?0:gObj[aIndex].Guild->WarScore);

	pMsg.score[1] = ((gObj[aIndex].Guild->WarEnemy==0)?0:gObj[aIndex].Guild->WarEnemy->WarScore);

	pMsg.type = GUILD_WAR_TYPE_NORMAL;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void CGuild::GCGuildAssignStatusSend(int aIndex,int type,int result,char* name) // OK
{
	PMSG_GUILD_ASSIGN_STATUS_SEND pMsg;

	pMsg.header.set(0xE1,sizeof(pMsg));

	pMsg.type = type;

	pMsg.result = result;

	memcpy(pMsg.name,name,sizeof(pMsg.name));

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void CGuild::GCGuildAssignTypeSend(int aIndex,int type,int result) // OK
{
	PMSG_GUILD_ASSIGN_TYPE_SEND pMsg;

	pMsg.header.set(0xE2,sizeof(pMsg));

	pMsg.type = type;

	pMsg.result = result;

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void CGuild::GCGuildUnionRequestSend(int aIndex,int relation,int type,int bIndex) // OK
{
	PMSG_GUILD_UNION_REQUEST_SEND pMsg;

	pMsg.header.set(0xE5,sizeof(pMsg));

	pMsg.relation = relation;

	pMsg.type = type;

	pMsg.index[0] = SET_NUMBERHB(bIndex);

	pMsg.index[1] = SET_NUMBERLB(bIndex);

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void CGuild::GCGuildUnionResultSend(int aIndex,int relation,int type,int result,int bIndex) // OK
{
	PMSG_GUILD_UNION_RESULT_SEND pMsg;

	pMsg.header.set(0xE6,sizeof(pMsg));

	pMsg.relation = relation;

	pMsg.type = type;

	pMsg.result = result;

	pMsg.index[0] = SET_NUMBERHB(bIndex);

	pMsg.index[1] = SET_NUMBERLB(bIndex);

	DataSend(aIndex,(BYTE*)&pMsg,pMsg.header.size);
}

void CGuild::GuildWarDeclareSet(int aIndex,char* GuildName,int type,int team) // OK
{
	if(gObj[aIndex].Guild == 0)
	{
		return;
	}

	this->GCGuildWarDeclareSend(aIndex,GuildName,type,team);

	this->GCGuildWarScoreSend(aIndex);

	if(type == GUILD_WAR_TYPE_SOCCER)
	{
		gObj[aIndex].IsInBattleGround = 1;

		if(gServerInfo.m_PKLimitFree == 0 && gObj[aIndex].PKLevel > 4)
		{
			return;
		}

		if(team == GUILD_WAR_TEAM_BLUE)
		{
			gObjTeleport(aIndex,6,60,153);
		}
		else
		{
			gObjTeleport(aIndex,6,60,168);
		}
	}
}

void CGuild::GuildWarRequestSend(int aIndex,char* GuildName,int type) // OK
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == 0)
	{
		return;
	}

	if(gMapManager.GetMapGuildWarEnable(lpObj->Map) == 0)
	{
		return;
	}

	if(lpObj->Guild == 0)
	{
		this->GCGuildWarResultSend(aIndex,3);
		return;
	}

	if(lpObj->Guild->WarState == GUILD_WAR_STATE_DECLARE || lpObj->Guild->WarDeclare == GUILD_WAR_STATE_DECLARE)
	{
		this->GCGuildWarResultSend(aIndex,4);
		return;
	}

	if(gServerInfo.m_PKLimitFree == 0 && lpObj->PKLevel > 4)
	{
		this->GCGuildWarResultSend(aIndex,4);
		return;
	}

	if(lpObj->GuildRank != GUILD_RANK_MASTER)
	{
		this->GCGuildWarResultSend(aIndex,5);
		return;
	}

	if(!strncmp(lpObj->Guild->Name,GuildName,8))
	{
		return;
	}

	GUILD_INFO* lpInfo = gGuildManager.GetGuild(GuildName);

	if(lpInfo == 0)
	{
		this->GCGuildWarResultSend(aIndex,0);
		return;
	}

	if(lpInfo->WarState == GUILD_WAR_STATE_DECLARE || lpInfo->WarDeclare == GUILD_WAR_STATE_DECLARE)
	{
		this->GCGuildWarResultSend(aIndex,4);
		return;
	}

	if(lpObj->Guild->GuildUnion != 0 && lpObj->Guild->GuildUnion == lpInfo->GuildUnion)
	{
		return;
	}

	LPOBJ lpTarget = gObjFind(lpInfo->Master);

	if(lpTarget == 0)
	{
		this->GCGuildWarResultSend(aIndex,2);
		return;
	}

	if(gServerInfo.m_PKLimitFree == 0 && lpTarget->PKLevel > 4)
	{
		this->GCGuildWarResultSend(aIndex,4);
		return;
	}

	if(gMapManager.GetMapGuildWarEnable(lpTarget->Map) == 0)
	{
		return;
	}

	if((lpTarget->Option & 1) != 1)
	{
		this->GCGuildWarResultSend(aIndex,4);
		return;
	}

	this->GCGuildWarResultSend(aIndex,1);

	this->GCGuildWarRequestSend(lpTarget->Index,lpObj->Guild->Name,type);

	lpObj->Guild->WarDeclare = GUILD_WAR_STATE_DECLARE;
	lpObj->Guild->WarType = type;
	lpObj->Guild->WarEnemy = lpInfo;

	lpInfo->WarDeclare = GUILD_WAR_STATE_DECLARE;
	lpInfo->WarType = type;
	lpInfo->WarEnemy = lpObj->Guild;
}

int CGuild::GetMaxGuildUser(LPOBJ lpObj) // OK
{
	int MaxGuildUser = 0;

	if(lpObj->Class == CLASS_DL)
	{
		MaxGuildUser = ((lpObj->Reset==0)?(((lpObj->Level+lpObj->MasterLevel)/10)+(lpObj->Leadership/10)):gServerInfo.m_GuildInsertMaxUser2);

		MaxGuildUser = ((MaxGuildUser>gServerInfo.m_GuildInsertMaxUser2)?gServerInfo.m_GuildInsertMaxUser2:MaxGuildUser);
	}
	else
	{
		MaxGuildUser = ((lpObj->Reset==0)?((lpObj->Level+lpObj->MasterLevel)/10):gServerInfo.m_GuildInsertMaxUser1);

		MaxGuildUser = ((MaxGuildUser>gServerInfo.m_GuildInsertMaxUser1)?gServerInfo.m_GuildInsertMaxUser1:MaxGuildUser);
	}

	return ((MaxGuildUser> MAX_GUILD_MEMBER)?MAX_GUILD_MEMBER:MaxGuildUser);
}

int CGuild::GetGuildNameCount(LPOBJ lpObj,char* name) // OK
{
	int count = 0;

	for(int n=0;n < MAX_GUILD_MEMBER;n++)
	{
		if(lpObj->Guild->Member[n].IsEmpty() == 0 && strcmp(lpObj->Guild->Member[n].Name,name) == 0)
		{
			count++;
		}
	}

	return count;
}

int CGuild::GetGuildBattleMasterCount(LPOBJ lpObj) // OK
{
	int count = 0;

	for(int n=0;n < MAX_GUILD_MEMBER;n++)
	{
		if(lpObj->Guild->Member[n].IsEmpty() == 0 && lpObj->Guild->Member[n].Rank == GUILD_RANK_BATTLE_MASTER)
		{
			count++;
		}
	}

	return count;
}

int CGuild::GetGuildOfficeMasterCount(LPOBJ lpObj) // OK
{
	int count = 0;

	for(int n=0;n < MAX_GUILD_MEMBER;n++)
	{
		if(lpObj->Guild->Member[n].IsEmpty() == 0 && lpObj->Guild->Member[n].Rank == GUILD_RANK_OFFICE_MASTER)
		{
			count++;
		}
	}

	return count;
}

int CGuild::gObjTargetGuildWarCheck(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	if(lpObj->Type != OBJECT_USER || lpTarget->Type != OBJECT_USER)
	{
		return 0;
	}

	if(lpObj->Guild == 0 || lpTarget->Guild == 0)
	{
		return 0;
	}

	if(lpObj->Guild->WarState != GUILD_WAR_STATE_DECLARE || lpTarget->Guild->WarState != GUILD_WAR_STATE_DECLARE)
	{
		return 0;
	}

	if(strcmp(lpObj->Guild->WarEnemy->Name,lpTarget->Guild->Name))
	{
		return 0;
	}

	return 1;
}

void CGuild::gObjGuildWarEndSend(LPOBJ lpObj,int result1,int result2) // OK
{
	if(lpObj == 0)
	{
		return;
	}

	if(lpObj->Guild == 0)
	{
		return;
	}

	if(lpObj->Guild->WarState != GUILD_WAR_STATE_DECLARE)
	{
		return;
	}

	if(lpObj->Guild->WarEnemy == 0)
	{
		return;
	}

	for(int n=0;n < MAX_GUILD_MEMBER;n++)
	{
		if(lpObj->Guild->Member[n].IsValid() != 0)
		{
			this->GCGuildWarEndSend(lpObj->Guild->Member[n].Index,result1,lpObj->Guild->WarEnemy->Name);
		}
	}

	for(int n=0;n < MAX_GUILD_MEMBER;n++)
	{
		if(lpObj->Guild->WarEnemy->Member[n].IsValid() != 0)
		{
			this->GCGuildWarEndSend(lpObj->Guild->WarEnemy->Member[n].Index,result2,lpObj->Guild->Name);
		}
	}
}

void CGuild::gObjGuildWarEndSend(GUILD_INFO* lpGuild1,GUILD_INFO* lpGuild2,int result1,int result2) // OK
{
	if(lpGuild1 == 0 || lpGuild2 == 0)
	{
		return;
	}

	if(lpGuild1->WarState != GUILD_WAR_STATE_DECLARE || lpGuild2->WarState != GUILD_WAR_STATE_DECLARE)
	{
		return;
	}

	for(int n=0;n < MAX_GUILD_MEMBER;n++)
	{
		if(lpGuild1->Member[n].IsValid() != 0)
		{
			gGuild.GCGuildWarEndSend(lpGuild1->Member[n].Index,result1,lpGuild2->Name);
		}		
	}

	for(int n=0;n < MAX_GUILD_MEMBER;n++)
	{
		if(lpGuild2->Member[n].IsValid() != 0)
		{
			gGuild.GCGuildWarEndSend(lpGuild2->Member[n].Index,result2,lpGuild1->Name);
		}
	}
}

void CGuild::gObjGuildWarEnd(GUILD_INFO* lpGuild1,GUILD_INFO* lpGuild2) // OK
{
	if(lpGuild1 != 0)
	{
		lpGuild1->WarDeclare = GUILD_WAR_STATE_NONE;
		lpGuild1->WarState = GUILD_WAR_STATE_NONE;

		if(lpGuild1->WarEnemy != 0)
		{
			lpGuild1->WarEnemy = 0;
		}
	}

	if(lpGuild2 != 0)
	{
		lpGuild2->WarDeclare = GUILD_WAR_STATE_NONE;
		lpGuild2->WarState = GUILD_WAR_STATE_NONE;

		if(lpGuild2->WarEnemy != 0)
		{
			lpGuild2->WarEnemy = 0;
		}
	}
}

int CGuild::gObjGuildWarProc(GUILD_INFO* lpGuild1,GUILD_INFO* lpGuild2,int score) // OK
{
	if(lpGuild1 == 0 || lpGuild2 == 0)
	{
		return 0;
	}

	int MaxScore = (lpGuild1->WarType==GUILD_WAR_TYPE_SOCCER)?100:20;

	lpGuild1->WarScore += score;

	if(lpGuild1->WarScore >= MaxScore)
	{
		if(lpGuild1->WarScore > MaxScore && lpGuild2->WarScore == 0)
		{
			lpGuild1->Score += 3;
		}
		else if(lpGuild1->WarScore > MaxScore && lpGuild2->WarScore <= 10)
		{
			lpGuild1->Score += 2;
		}
		else
		{
			lpGuild1->Score += 1;
		}

		this->GDGuildScoreUpdateSend(lpGuild1->Name,lpGuild1->Score);

		this->GDGuildScoreUpdateSend(lpGuild2->Name,lpGuild2->Score);

		return 1;
	}

	for(int n=0;n < MAX_GUILD_MEMBER;n++)
	{
		if(lpGuild1->Member[n].IsValid() != 0)
		{
			this->GCGuildWarScoreSend(lpGuild1->Member[n].Index);
		}
	}

	for(int n=0;n < MAX_GUILD_MEMBER;n++)
	{
		if(lpGuild2->Member[n].IsValid() != 0)
		{
			this->GCGuildWarScoreSend(lpGuild2->Member[n].Index);
		}
	}

	return 0;
}

int CGuild::gObjGuildWarCheck(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	if(gObjTargetGuildWarCheck(lpObj,lpTarget) == 0)
	{
		return 0;
	}

	GUILD_INFO* lpGuild1 = lpObj->Guild;

	GUILD_INFO* lpGuild2 = lpTarget->Guild;

	int Score = (strcmp(lpTarget->Name,lpTarget->Guild->Master) == 0) ? 2 : 1;

	if(gObjGuildWarProc(lpGuild1,lpGuild2,Score) == 1)
	{
		if(lpGuild1->WarType == GUILD_WAR_TYPE_SOCCER)
		{
			gObjGuildWarEndSend(lpGuild1,lpGuild2,1,0);
			gObjGuildWarEnd(lpGuild1,lpGuild2);
			gBattleSoccer.SetState(BS_STATE_CLEAN);
		}
		else
		{
			gObjGuildWarEndSend(lpObj,1,0);
			gObjAddMsgSendDelay(lpObj,4,lpObj->Index,2000,0);
		}
	}

	return 1;
}

int CGuild::gObjGuildWarMasterClose(LPOBJ lpObj) // OK
{
	if(lpObj->Guild == 0)
	{
		return 0;
	}

	if(strcmp(lpObj->Guild->Master,lpObj->Name))
	{
		return 0;
	}

	gObjGuildWarEndSend(lpObj,3,2);

	if(lpObj->Guild->WarType == GUILD_WAR_TYPE_SOCCER)
	{
		gBattleSoccer.SetState(BS_STATE_CLEAN);
	}

	gObjGuildWarEnd(lpObj->Guild,lpObj->Guild->WarEnemy);

	return 1;
}

int CGuild::gObjGetGuildUnionNumber(LPOBJ lpObj) // OK
{
	if(lpObj->Guild == 0)
	{
		return 0;
	}

	return (lpObj->Guild->GuildUnion == GUILD_RELATION_NONE) ? lpObj->Guild->Index : lpObj->Guild->GuildUnion;
}

void CGuild::gObjGetGuildUnionName(LPOBJ lpObj,char* UnionName,int size) // OK
{
	memset(UnionName,0,sizeof(UnionName));

	if(lpObj->Guild == 0 || lpObj->Guild->GuildUnion == 0)
	{
		return;
	}

	CUnionInfo* lpUnion = gUnionManager.SearchUnion(lpObj->Guild->GuildUnion);

	if(lpUnion != 0)
	{
		memcpy(UnionName,lpUnion->m_szMasterGuild,size);
	}
}

bool CGuild::gObjCheckRival(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	if(lpObj->Type != OBJECT_USER || lpTarget->Type != OBJECT_USER)
	{
		return 0;
	}

	if(lpObj->Guild == 0 || lpTarget->Guild == 0)
	{
		return 0;
	}

	if(lpObj->Guild->GuildRival == GUILD_RELATION_NONE || lpTarget->Guild->GuildRival == GUILD_RELATION_NONE)
	{
		return 0;
	}

	if(gUnionManager.GetGuildRelationShip((lpObj->Guild->GuildUnion==GUILD_RELATION_NONE) ? lpObj->Guild->Index : lpObj->Guild->GuildUnion,lpTarget->Guild->Index) == 2)
	{
		return 1;
	}

	return 0;
}

int CGuild::gObjGetRelationShip(LPOBJ lpObj,LPOBJ lpTarget) // OK
{
	if(lpObj == 0 || lpTarget == 0)
	{
		return 0;
	}

	if(lpObj->Guild == 0 || lpTarget->Guild == 0)
	{
		return 0;
	}

	return gUnionManager.GetGuildRelationShip((lpObj->Guild->GuildUnion==GUILD_RELATION_NONE) ? lpObj->Guild->Index : lpObj->Guild->GuildUnion,lpTarget->Guild->Index);
}

void CGuild::NoticeSendToAll(GUILD_INFO* lpGuild,int type,char* message,...) // OK
{
	if(lpGuild != 0)
	{
		char buff[256];

		va_list arg;
		va_start(arg,message);
		vsprintf_s(buff,message,arg);
		va_end(arg);

		for(int n=0;n < MAX_GUILD_MEMBER;n++)
		{
			if(lpGuild->Member[n].IsValid() != 0)
			{
				gNotice.GCNoticeSend(lpGuild->Member[n].Index,type,0,0,0,0,0,buff);
			}
		}
	}
}

void CGuild::DataSendToAll(GUILD_INFO* lpGuild,BYTE* lpMsg,int size) // OK
{
	if(lpGuild != 0)
	{
		for(int n=0;n < MAX_GUILD_MEMBER;n++)
		{
			if(lpGuild->Member[n].IsValid() != 0)
			{
				DataSend(lpGuild->Member[n].Index,lpMsg,size);
			}
		}
	}
}