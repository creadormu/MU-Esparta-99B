// Guild.cpp: implementation of the CGuild class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Guild.h"
#include "CharacterManager.h"
#include "GuildManager.h"
#include "ServerManager.h"
#include "SocketManager.h"
#include "Util.h"

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

void CGuild::GDGuildCreateRecv(SDHP_GUILD_CREATE_RECV* lpMsg,int aIndex) // OK
{
	SDHP_GUILD_CREATE_SEND pMsg;

	pMsg.header.set(0xA0,0x00,sizeof(pMsg));

	pMsg.Index = 0;

	memcpy(pMsg.Name,lpMsg->Name,sizeof(pMsg.Name));

	memcpy(pMsg.Master,lpMsg->Master,sizeof(pMsg.Master));

	memcpy(pMsg.Mark,lpMsg->Mark,sizeof(pMsg.Mark));

	if((pMsg.result=gGuildManager.AddGuild(lpMsg->Name,lpMsg->Master,lpMsg->Mark,lpMsg->type)) != 1)
	{
		gSocketManager.DataSend(aIndex,(BYTE*)&pMsg,sizeof(pMsg));
		return;
	}

	GUILD_INFO* lpGuild = gGuildManager.GetGuildInfo(lpMsg->Name);

	if(lpGuild != 0)
	{
		pMsg.Index = lpGuild->Index;

		this->DataSendToAll((BYTE*)&pMsg,sizeof(pMsg));

		this->MemberConnect(lpMsg->Master,lpMsg->Server);
	}
}

void CGuild::GDGuildDeleteRecv(SDHP_GUILD_DELETE_RECV* lpMsg,int aIndex) // OK
{
	SDHP_GUILD_DELETE_SEND pMsg;

	pMsg.header.set(0xA0,0x01,sizeof(pMsg));

	memcpy(pMsg.Name,lpMsg->Name,sizeof(pMsg.Name));

	memcpy(pMsg.Member,lpMsg->Member,sizeof(pMsg.Member));

	if((pMsg.result=gGuildManager.DelGuild(lpMsg->Name)) != 1)
	{
		gSocketManager.DataSend(aIndex,(BYTE*)&pMsg,sizeof(pMsg));
		return;
	}

	this->DataSendToAll((BYTE*)&pMsg,sizeof(pMsg));
}

void CGuild::GDGuildMemberAddRecv(SDHP_GUILD_MEMBER_ADD_RECV* lpMsg,int aIndex) // OK
{
	SDHP_GUILD_MEMBER_ADD_SEND pMsg;

	pMsg.header.set(0xA0,0x02,sizeof(pMsg));

	memcpy(pMsg.Name,lpMsg->Name,sizeof(pMsg.Name));

	memcpy(pMsg.Member,lpMsg->Member,sizeof(pMsg.Member));

	pMsg.Server = lpMsg->Server;

	if((pMsg.result=gGuildManager.AddGuildMember(lpMsg->Name,lpMsg->Member,0,pMsg.Server)) != 1)
	{
		gSocketManager.DataSend(aIndex,(BYTE*)&pMsg,sizeof(pMsg));
		return;
	}

	this->DataSendToAll((BYTE*)&pMsg,sizeof(pMsg));
}

void CGuild::GDGuildMemberDelRecv(SDHP_GUILD_MEMBER_DEL_RECV* lpMsg,int aIndex) // OK
{
	SDHP_GUILD_MEMBER_DEL_SEND pMsg;

	pMsg.header.set(0xA0,0x03,sizeof(pMsg));

	memcpy(pMsg.Name,lpMsg->Name,sizeof(pMsg.Name));

	memcpy(pMsg.Member,lpMsg->Member,sizeof(pMsg.Member));

	if((pMsg.result=gGuildManager.DelGuildMember(lpMsg->Member)) != 1)
	{
		gSocketManager.DataSend(aIndex,(BYTE*)&pMsg,sizeof(pMsg));
		return;
	}

	this->DataSendToAll((BYTE*)&pMsg,sizeof(pMsg));
}

void CGuild::GDGuildAssignStatusRecv(SDHP_GUILD_ASSIGN_STATUS_RECV* lpMsg) // OK
{
	SDHP_GUILD_ASSIGN_STATUS_SEND pMsg;

	pMsg.header.set(0xA0,0x05,sizeof(pMsg));

	memcpy(pMsg.Name,lpMsg->Name,sizeof(pMsg.Name));

	memcpy(pMsg.Member,lpMsg->Member,sizeof(pMsg.Member));

	pMsg.Status = lpMsg->Status;

	pMsg.Type = lpMsg->Type;

	pMsg.result = gGuildManager.SetGuildMemberStatus(lpMsg->Member,lpMsg->Status);

	this->DataSendToAll((BYTE*)&pMsg,sizeof(pMsg));
}

void CGuild::GDGuildAssignTypeRecv(SDHP_GUILD_ASSIGN_TYPE_RECV* lpMsg) // OK
{
	SDHP_GUILD_ASSIGN_TYPE_SEND pMsg;

	pMsg.header.set(0xA0,0x06,sizeof(pMsg));

	memcpy(pMsg.Name,lpMsg->Name,sizeof(pMsg.Name));

	pMsg.Type = lpMsg->Type;

	pMsg.result = gGuildManager.SetGuildType(lpMsg->Name,lpMsg->Type);

	this->DataSendToAll((BYTE*)&pMsg,sizeof(pMsg));
}

void CGuild::GDGuildNoticeRecv(SDHP_GUILD_NOTICE_RECV* lpMsg) // OK
{
	if(gGuildManager.SetGuildNotice(lpMsg->Name,lpMsg->Notice) != 0)
	{
		SDHP_GUILD_NOTICE_SEND pMsg;

		pMsg.header.set(0xA0,0x07,sizeof(pMsg));

		memcpy(pMsg.Name,lpMsg->Name,sizeof(pMsg.Name));

		memcpy(pMsg.Notice,lpMsg->Notice,sizeof(pMsg.Notice));

		this->DataSendToAll((BYTE*)&pMsg,sizeof(pMsg));
	}
}

void CGuild::GDGuildPostRecv(SDHP_GUILD_POST_RECV* lpMsg) // OK
{
	SDHP_GUILD_POST_SEND pMsg;

	pMsg.header.set(0xA0,0x08,sizeof(pMsg));

	memcpy(pMsg.Name,lpMsg->Name,sizeof(pMsg.Name));

	memcpy(pMsg.Member,lpMsg->Member,sizeof(pMsg.Member));

	pMsg.Type = lpMsg->Type;

	memcpy(pMsg.Message,lpMsg->Message,sizeof(pMsg.Message));

	this->DataSendToAll((BYTE*)&pMsg,sizeof(pMsg));
}

void CGuild::GDGuildScoreRecv(SDHP_GUILD_SCORE_RECV* lpMsg) // OK
{
	if(gGuildManager.SetGuildScore(lpMsg->Name,lpMsg->Score) != 0)
	{
		SDHP_GUILD_SCORE_SEND pMsg;

		pMsg.header.set(0xA0,0x09,sizeof(pMsg));

		memcpy(pMsg.Name,lpMsg->Name,sizeof(pMsg.Name));

		pMsg.Score = lpMsg->Score;

		this->DataSendToAll((BYTE*)&pMsg,sizeof(pMsg));
	}
}

void CGuild::GDGuildUnionGetRecv(SDHP_GUILD_UNION_GET_RECV* lpMsg,int aIndex) // OK
{
	BYTE send[4096];

	SDHP_GUILD_UNION_INFO_SEND pMsg;

	pMsg.header.set(0xA0,0x0D,0);

	int size = sizeof(pMsg);

	memcpy(pMsg.Member,lpMsg->Member,sizeof(pMsg.Member));

	pMsg.Index = 0;

	pMsg.Union = 0;

	pMsg.Rival = 0;

	pMsg.result = 0;

	pMsg.count = 0;

	GUILD_INFO* lpGuildInfo = gGuildManager.GetGuildInfo(lpMsg->Index);

	if(lpGuildInfo != 0)
	{
		DWORD UnionList[MAX_GUILD_UNION];

		DWORD RivalList[MAX_GUILD_RIVAL];

		pMsg.Index = lpGuildInfo->Index;

		pMsg.Union = (BYTE)gGuildManager.GetUnionList(lpGuildInfo->Union,UnionList);

		pMsg.Rival = (BYTE)gGuildManager.GetRivalList(lpGuildInfo->Rival,RivalList);

		pMsg.result = 1;

		for(int n=0;n < pMsg.Union;n++)
		{
			GUILD_INFO* lpGuild = gGuildManager.GetGuildInfo(UnionList[n]);

			if(lpGuild != 0)
			{
				SDHP_GUILD_UNION_INFO info;

				memcpy(info.Name,lpGuild->Name,sizeof(info.Name));

				memcpy(info.Mark,lpGuild->Mark,sizeof(info.Mark));

				info.count = lpGuild->GetMemberCount();

				memcpy(&send[size],&info,sizeof(info));

				size += sizeof(info);

				pMsg.count++;
			}
		}
	}

	pMsg.header.size[0] = SET_NUMBERHB(size);
	pMsg.header.size[1] = SET_NUMBERLB(size);

	memcpy(send,&pMsg,sizeof(pMsg));

	gSocketManager.DataSend(aIndex,send,size);
}

void CGuild::GDGuildUnionJoinRecv(SDHP_GUILD_UNION_JOIN_RECV* lpMsg,int aIndex) // OK
{
	SDHP_GUILD_UNION_JOIN_SEND pMsg;

	pMsg.header.set(0xA0,0x0E,sizeof(pMsg));

	pMsg.result = 0;

	pMsg.flag = 1;

	memcpy(pMsg.index,lpMsg->index,sizeof(pMsg.index));

	pMsg.relation = lpMsg->relation;

	memcpy(pMsg.GuildNumber,lpMsg->GuildNumber,sizeof(pMsg.GuildNumber));

	GUILD_INFO* lpSourceGuildInfo;

	GUILD_INFO* lpTargetGuildInfo;

	if((lpSourceGuildInfo=gGuildManager.GetGuildInfo(lpMsg->GuildNumber[0])) == 0 || (lpTargetGuildInfo=gGuildManager.GetGuildInfo(lpMsg->GuildNumber[1])) == 0)
	{
		gSocketManager.DataSend(aIndex,(BYTE*)&pMsg,sizeof(pMsg));
		return;
	}

	if((pMsg.result=gGuildManager.AddGuildRelationship(lpMsg->GuildNumber[0],lpMsg->GuildNumber[1],lpMsg->relation)) != 1)
	{
		gSocketManager.DataSend(aIndex,(BYTE*)&pMsg,sizeof(pMsg));
		return;
	}

	for(int n=0;n < MAX_SERVER;n++)
	{
		if(gServerManager[n].CheckState() != 0)
		{
			pMsg.flag = ((n== aIndex)?1:0);
			gSocketManager.DataSend(n,(BYTE*)&pMsg,sizeof(pMsg));
		}
	}
}

void CGuild::GDGuildUnionBreakRecv(SDHP_GUILD_UNION_BREAK_RECV* lpMsg,int aIndex) // OK
{
	SDHP_GUILD_UNION_BREAK_SEND pMsg;

	pMsg.header.set(0xA0,0x0F,sizeof(pMsg));

	pMsg.result = 0;

	pMsg.flag = 1;

	memcpy(pMsg.index,lpMsg->index,sizeof(pMsg.index));

	pMsg.relation = lpMsg->relation;

	memcpy(pMsg.GuildNumber,lpMsg->GuildNumber,sizeof(pMsg.GuildNumber));

	if((pMsg.result=gGuildManager.DelGuildRelationship(lpMsg->GuildNumber[0],lpMsg->relation)) != 1)
	{
		gSocketManager.DataSend(aIndex,(BYTE*)&pMsg,sizeof(pMsg));
		return;
	}

	for(int n=0;n < MAX_SERVER;n++)
	{
		if(gServerManager[n].CheckState() != 0)
		{
			pMsg.flag = ((n==aIndex)?1:0);
			gSocketManager.DataSend(n,(BYTE*)&pMsg,sizeof(pMsg));
		}
	}
}

void CGuild::GDGuildAllianceKickRecv(SDHP_GUILD_ALLIANCE_KICK_RECV* lpMsg,int aIndex) // OK
{
	SDHP_GUILD_ALLIANCE_KICK_SEND pMsg;

	pMsg.header.set(0xA0,0x10,sizeof(pMsg));

	pMsg.flag = 1;

	pMsg.index = lpMsg->index;

	pMsg.relation = lpMsg->relation;

	pMsg.result = 0;

	memcpy(pMsg.GuildNameA,lpMsg->GuildNameA,sizeof(pMsg.GuildNameA));

	memcpy(pMsg.GuildNameB,lpMsg->GuildNameB,sizeof(pMsg.GuildNameB));

	if((pMsg.result=gGuildManager.SetGuildRelationship(lpMsg->GuildNameB,lpMsg->GuildNameA)) != 1)
	{
		gSocketManager.DataSend(aIndex,(BYTE*)&pMsg,sizeof(pMsg));
		return;
	}

	for(int n=0;n < MAX_SERVER;n++)
	{
		if(gServerManager[n].CheckState() != 0)
		{
			pMsg.flag = ((n==aIndex)?1:0);
			gSocketManager.DataSend(n,(BYTE*)&pMsg,sizeof(pMsg));
		}
	}
}

void CGuild::GuildInfoSend(int index) // OK
{
	GUILD_INFO* lpGuildInfo = gGuildManager.GetGuildInfo(index);

	if(lpGuildInfo != 0)
	{
		BYTE send[4096];

		SDHP_GUILD_INFO_SEND pMsg;

		pMsg.header.set(0xA0,0x0A,0);

		int size = sizeof(pMsg);

		pMsg.Index = lpGuildInfo->Index;

		memcpy(pMsg.Name,lpGuildInfo->Name,sizeof(pMsg.Name));

		memcpy(pMsg.Master,lpGuildInfo->Master,sizeof(pMsg.Master));

		memcpy(pMsg.Mark,lpGuildInfo->Mark,sizeof(pMsg.Mark));

		pMsg.Score = lpGuildInfo->Score;

		memcpy(pMsg.Notice,lpGuildInfo->Notice,sizeof(pMsg.Notice));

		pMsg.type = lpGuildInfo->Type;

		pMsg.Union = lpGuildInfo->Union;

		pMsg.Rival = lpGuildInfo->Rival;

		pMsg.count = 0;

		SDHP_GUILD_INFO info;

		for(int n=0;n < MAX_GUILD_MEMBER;n++)
		{
			GUILD_MEMBER_INFO* lpGuildMemberInfo = &lpGuildInfo->Members[n];

			if(lpGuildMemberInfo->IsEmpty() == 0)
			{
				memcpy(info.Name,lpGuildMemberInfo->Name,sizeof(info.Name));

				info.Server = lpGuildMemberInfo->Server;

				info.Status = lpGuildMemberInfo->Status;

				memcpy(&send[size],&info,sizeof(info));

				size += sizeof(info);

				pMsg.count++;
			}
		}

		pMsg.header.size[0] = SET_NUMBERHB(size);
		pMsg.header.size[1] = SET_NUMBERLB(size);

		memcpy(send,&pMsg,sizeof(pMsg));

		this->DataSendToAll(send,size);
	}
}

void CGuild::UnionInfoSend(int index,int relation) // OK
{
	GUILD_INFO* lpGuildInfo = gGuildManager.GetGuildInfo(index);

	if(lpGuildInfo != 0)
	{
		SDHP_UNION_INFO_SEND pMsg;

		pMsg.header.set(0xA0,0x0B,sizeof(pMsg));

		pMsg.Index = lpGuildInfo->Index;

		memcpy(pMsg.Name,lpGuildInfo->Name,sizeof(pMsg.Name));

		pMsg.relation = relation;

		if(relation == 1)
		{
			pMsg.count = (BYTE)gGuildManager.GetUnionList(lpGuildInfo->Index,(DWORD*)pMsg.list);
		}
		else if (relation == 2)
		{
			pMsg.count = (BYTE)gGuildManager.GetRivalList(lpGuildInfo->Index,(DWORD*)pMsg.list);
		}

		this->DataSendToAll((BYTE*)&pMsg,sizeof(pMsg));
	}
}

void CGuild::UnionUpdateSend(int code,int count,int* list) // OK
{
	SDHP_UNION_UPDATE_SEND pMsg;

	pMsg.header.set(0xA0,0x0C,sizeof(pMsg));

	pMsg.code = code;

	pMsg.count = count;

	memcpy(pMsg.list,list,(count*sizeof(int)));

	this->DataSendToAll((BYTE*)&pMsg,sizeof(pMsg));
}

void CGuild::MemberConnect(char* member,int server) // OK
{
	GUILD_INFO* lpGuildInfo = gGuildManager.GetMemberGuildInfo(member);

	GUILD_MEMBER_INFO* lpGuildMemberInfo = gGuildManager.GetGuildMemberInfo(member);

	if(lpGuildInfo == 0 || lpGuildMemberInfo == 0)
	{
		return;
	}

	lpGuildMemberInfo->Server = server;

	this->GuildInfoSend(lpGuildInfo->Index);

	this->UnionInfoSend(lpGuildInfo->Union,1);

	this->UnionInfoSend(lpGuildInfo->Rival,2);

	this->MemberUpdate(lpGuildInfo->Name,member,lpGuildMemberInfo->Status,server);
}

void CGuild::MemberDisconnect(char* member) // OK
{
	GUILD_INFO* lpGuildInfo = gGuildManager.GetMemberGuildInfo(member);

	GUILD_MEMBER_INFO* lpGuildMemberInfo = gGuildManager.GetGuildMemberInfo(member);

	if(lpGuildInfo == 0 || lpGuildMemberInfo == 0)
	{
		return;
	}

	lpGuildMemberInfo->Server = -1;

	this->MemberUpdate(lpGuildInfo->Name,member,lpGuildMemberInfo->Status,-1);
}

void CGuild::MemberUpdate(char* name,char* member,int status,int server) // OK
{
	SDHP_GUILD_MEMBER_UPDATE_SEND pMsg;

	pMsg.header.set(0xA0,0x04,sizeof(pMsg));

	memcpy(pMsg.Name,name,sizeof(pMsg.Name));

	memcpy(pMsg.Member,member,sizeof(pMsg.Member));

	pMsg.Server = server;

	pMsg.Status = status;

	this->DataSendToAll((BYTE*)&pMsg,sizeof(pMsg));
}

void CGuild::DataSendToAll(BYTE* lpMsg,int size) // OK
{
	for(int n=0;n < MAX_SERVER;n++)
	{
		if(gServerManager[n].CheckState() != 0)
		{
			gSocketManager.DataSend(n,lpMsg,size);
		}
	}
}