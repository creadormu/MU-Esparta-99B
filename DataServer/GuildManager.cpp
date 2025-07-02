// GuildManager.cpp: implementation of the CGuildManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GuildManager.h"
#include "Guild.h"
#include "QueryManager.h"
#include "Util.h"

CGuildManager gGuildManager;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGuildManager::CGuildManager()
{

}

CGuildManager::~CGuildManager()
{

}

void CGuildManager::Init()
{
	this->vGuildList.clear();

	if(gQueryManager.ExecQuery("SELECT * FROM Guild") != 0)
	{
		while(gQueryManager.Fetch() != SQL_NO_DATA)
		{
			GUILD_INFO GuildInfo;

			GuildInfo.Clear();

			gQueryManager.GetAsString("G_Name",GuildInfo.Name,sizeof(GuildInfo.Name));

			gQueryManager.GetAsBinary("G_Mark",GuildInfo.Mark,sizeof(GuildInfo.Mark));

			GuildInfo.Score = gQueryManager.GetAsInteger("G_Score");

			gQueryManager.GetAsString("G_Master",GuildInfo.Master,sizeof(GuildInfo.Master));

			gQueryManager.GetAsString("G_Notice",GuildInfo.Notice,sizeof(GuildInfo.Notice));

			GuildInfo.Index = gQueryManager.GetAsInteger("Number");

			GuildInfo.Type = gQueryManager.GetAsInteger("G_Type");

			GuildInfo.Rival = gQueryManager.GetAsInteger("G_Rival");

			GuildInfo.Union = gQueryManager.GetAsInteger("G_Union");

			this->vGuildList.push_back(GuildInfo);
		}
	}

	gQueryManager.Close();

	if(gQueryManager.ExecQuery("SELECT * FROM GuildMember") != 0)
	{
		while(gQueryManager.Fetch() != SQL_NO_DATA)
		{
			char GuildName[9] = {0};

			GUILD_MEMBER_INFO GuildMemberInfo;

			GuildMemberInfo.Clear();

			gQueryManager.GetAsString("Name",GuildMemberInfo.Name,sizeof(GuildMemberInfo.Name));

			gQueryManager.GetAsString("G_Name",GuildName,sizeof(GuildName));

			GuildMemberInfo.Status = gQueryManager.GetAsInteger("G_Status");

			GUILD_INFO* lpGuildInfo = this->GetGuildInfo(GuildName);

			if(lpGuildInfo != 0)
			{
				if(_stricmp(lpGuildInfo->Master,GuildMemberInfo.Name) == 0)
				{
					lpGuildInfo->Members[0] = GuildMemberInfo;
				}
				else
				{
					for(int n=1;n < MAX_GUILD_MEMBER;n++)
					{
						if(lpGuildInfo->Members[n].IsEmpty() != 0)
						{
							lpGuildInfo->Members[n] = GuildMemberInfo;
							break;
						}
					}
				}
			}
		}
	}

	gQueryManager.Close();
}

GUILD_INFO* CGuildManager::GetGuildInfo(char* szName)
{
	for(std::vector<GUILD_INFO>::iterator it=this->vGuildList.begin();it != this->vGuildList.end();it++)
	{
		if(it->Name[0] == szName[0])
		{
			if(_stricmp(it->Name,szName) == 0)
			{
				return it._Ptr;
			}
		}
	}

	return 0;
}

GUILD_INFO* CGuildManager::GetGuildInfo(DWORD dwNumber)
{
	for(std::vector<GUILD_INFO>::iterator it=this->vGuildList.begin();it != this->vGuildList.end();it++)
	{
		if(it->Index == dwNumber)
		{
			return it._Ptr;
		}
	}

	return 0;
}

GUILD_INFO* CGuildManager::GetMemberGuildInfo(char* szGuildMember)
{
	for(std::vector<GUILD_INFO>::iterator it=this->vGuildList.begin();it != this->vGuildList.end();it++)
	{
		for(int n=0;n < MAX_GUILD_MEMBER;n++)
		{
			if(it->Members[n].Name[0] == szGuildMember[0])
			{
				if(_stricmp(it->Members[n].Name,szGuildMember) == 0)
				{
					return it._Ptr;
				}
			}
		}
	}

	return 0;
}

GUILD_MEMBER_INFO* CGuildManager::GetGuildMemberInfo(char* szGuildMember)
{
	for(std::vector<GUILD_INFO>::iterator it=this->vGuildList.begin();it != this->vGuildList.end();it++)
	{
		for(int n=0;n < MAX_GUILD_MEMBER;n++)
		{
			if(it->Members[n].Name[0] == szGuildMember[0])
			{
				if(_stricmp(it->Members[n].Name,szGuildMember) == 0)
				{
					return &it->Members[n];
				}
			}
		}
	}

	return 0;
}

BOOL CGuildManager::CheckGuildOnCS(char* szGuildName)
{
	if(gQueryManager.ExecQuery("WZ_CS_CheckSiegeGuildList '%s'",szGuildName) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();

		return 0;
	}
	else
	{
		gQueryManager.Close();

		return gQueryManager.GetResult(0);
	}
}

BYTE CGuildManager::AddGuild(char* szGuildName,char* szMasterName,BYTE* lpMark,int btType)
{
	GUILD_INFO* lpGuildInfo = this->GetGuildInfo(szGuildName);

	if(lpGuildInfo != 0)
	{
		return 3;
	}

	if(CheckTextSyntax(szGuildName,strlen(szGuildName)) == 0)
	{
		return 4;
	}

	if(gQueryManager.ExecQuery("WZ_GuildCreate '%s','%s'",szGuildName,szMasterName) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();

		return 0;
	}
	else
	{
		if(gQueryManager.GetResult(0) == 0)
		{
			gQueryManager.Close();

			gQueryManager.BindParameterAsBinary(1,lpMark,32);

			gQueryManager.ExecQuery("UPDATE Guild SET G_Mark=?,G_Type=%d,G_Notice='' WHERE G_Name='%s'",btType,szGuildName);

			gQueryManager.Close();

			gQueryManager.ExecQuery("UPDATE GuildMember SET G_Status=%d WHERE Name='%s'",0x80,szMasterName);

			gQueryManager.Close();

			GUILD_INFO GuildInfo;

			GuildInfo.Clear();

			gQueryManager.ExecQuery("SELECT Number FROM Guild WHERE G_Name='%s'",szGuildName);

			gQueryManager.Fetch();

			GuildInfo.Index = gQueryManager.GetAsInteger("Number");

			gQueryManager.Close();

			memcpy(GuildInfo.Name,szGuildName,sizeof(GuildInfo.Name));

			memcpy(GuildInfo.Master,szMasterName,sizeof(GuildInfo.Master));

			GuildInfo.Type = btType;

			memcpy(GuildInfo.Mark,lpMark,sizeof(GuildInfo.Mark));

			memcpy(GuildInfo.Members[0].Name,szMasterName,sizeof(GuildInfo.Members[0].Name));

			GuildInfo.Members[0].Status = 128;

			this->vGuildList.push_back(GuildInfo);

			return 1;
		}
		else
		{
			gQueryManager.Close();

			return 2;
		}
	}
}

BYTE CGuildManager::DelGuild(char* szGuildName)
{
	GUILD_INFO* lpGuildInfo = this->GetGuildInfo(szGuildName);

	if(lpGuildInfo == 0)
	{
		return 3;
	}

	if(this->CheckGuildOnCS(szGuildName) != 0)
	{
		return 2;
	}

	this->DelGuildRelationship(lpGuildInfo->Index,1);

	this->DelGuildRelationship(lpGuildInfo->Index,2);

	lpGuildInfo->Clear();

	gQueryManager.ExecQuery("DELETE FROM Guild WHERE G_Name='%s'",szGuildName);

	gQueryManager.Close();

	gQueryManager.ExecQuery("DELETE FROM GuildMember WHERE G_Name='%s'",szGuildName);

	gQueryManager.Close();

	return 1;
}

BYTE CGuildManager::AddGuildMember(char* szGuildName,char* szGuildMember,int btStatus,int btServer)
{
	GUILD_INFO* lpGuildInfo = this->GetGuildInfo(szGuildName);

	if(lpGuildInfo == 0)
	{
		return 0;
	}

	if(this->GetGuildMemberInfo(szGuildMember) != 0)
	{
		return 3;
	}

	if(gQueryManager.ExecQuery("INSERT INTO GuildMember (Name,G_Name,G_Level,G_Status) VALUES ('%s','%s',%d,%d)",szGuildMember,szGuildName,1,btStatus) == 0)
	{
		gQueryManager.Close();

		return 4;
	}
	else
	{
		gQueryManager.Close();

		GUILD_MEMBER_INFO GuildMemberInfo;

		GuildMemberInfo.Clear();

		memcpy(GuildMemberInfo.Name,szGuildMember,sizeof(GuildMemberInfo.Name));

		GuildMemberInfo.Status = btStatus;

		GuildMemberInfo.Server = btServer;

		for(int n=1;n < MAX_GUILD_MEMBER;n++)
		{
			if(lpGuildInfo->Members[n].IsEmpty() != 0)
			{
				lpGuildInfo->Members[n] = GuildMemberInfo;
				return 1;
			}
		}

		return 4;
	}
}

BYTE CGuildManager::DelGuildMember(char* szGuildMember)
{
	GUILD_MEMBER_INFO* lpGuildMemberInfo = this->GetGuildMemberInfo(szGuildMember);

	if(lpGuildMemberInfo == 0){return 3;}

	gQueryManager.ExecQuery("DELETE FROM GuildMember WHERE Name='%s'",szGuildMember);

	gQueryManager.Close();

	lpGuildMemberInfo->Clear();

	return 1;
}

BYTE CGuildManager::AddGuildRelationship(int guild01,int guild02,int relation)
{
	GUILD_INFO* lpSourceGuild = this->GetGuildInfo(guild01);

	GUILD_INFO* lpTargetGuild = this->GetGuildInfo(guild02);

	if(lpSourceGuild == 0)
	{
		return 0;
	}

	if(lpTargetGuild == 0)
	{
		return 0;
	}

	if(this->CheckGuildOnCS(lpSourceGuild->Name) != 0)
	{
		return 16;
	}

	if(this->CheckGuildOnCS(lpTargetGuild->Name) != 0)
	{
		return 16;
	}

	DWORD UnionGuildNumber[MAX_GUILD_UNION];

	DWORD RivalGuildNumber[MAX_GUILD_RIVAL];

	DWORD UnionGuildAmount = 0;

	DWORD RivalGuildAmount = 0;

	switch(relation)
	{
		case 1:
			if(lpSourceGuild->Union > 0)
			{
				return 0;
			}

			if(lpSourceGuild->Rival > 0)
			{
				return 0;
			}

			if((UnionGuildAmount=this->GetUnionList(lpTargetGuild->Index,UnionGuildNumber)) >= MAX_GUILD_UNION)
			{
				return 0;
			}

			if(gQueryManager.ExecQuery("UPDATE Guild SET G_Union=%d WHERE Number=%d",lpTargetGuild->Index,lpTargetGuild->Index) == 0)
			{
				gQueryManager.Close();
				return 0;
			}
			else
			{
				gQueryManager.Close();
				lpTargetGuild->Union = lpTargetGuild->Index;
			}

			if(gQueryManager.ExecQuery("UPDATE Guild SET G_Union=%d WHERE Number=%d",lpTargetGuild->Index,lpSourceGuild->Index) == 0)
			{
				gQueryManager.Close();
				return 0;
			}
			else
			{
				gQueryManager.Close();
				lpSourceGuild->Union = lpTargetGuild->Index;
			}

			gGuild.UnionInfoSend(lpTargetGuild->Index,1);

			UnionGuildAmount = this->GetUnionList(lpTargetGuild->Index,UnionGuildNumber);

			gGuild.UnionUpdateSend(0,UnionGuildAmount,(int*)UnionGuildNumber);

			return 1;
		case 2:
			if(lpSourceGuild->Rival > 0)
			{
				return 0;
			}

			if(lpTargetGuild->Rival > 0)
			{
				return 0;
			}

			if(lpSourceGuild->Union > 0 && lpSourceGuild->Union != lpSourceGuild->Index)
			{
				return 0;
			}

			if(lpTargetGuild->Union > 0 && lpTargetGuild->Union != lpTargetGuild->Index)
			{
				return 0;
			}

			if(gQueryManager.ExecQuery("UPDATE Guild SET G_Rival=%d WHERE Number=%d",lpSourceGuild->Index,lpTargetGuild->Index) == 0)
			{
				gQueryManager.Close();
				return 0;
			}
			else
			{
				gQueryManager.Close();
				lpTargetGuild->Rival = lpSourceGuild->Index;
			}

			if(gQueryManager.ExecQuery("UPDATE Guild SET G_Rival=%d WHERE Number=%d",lpTargetGuild->Index,lpSourceGuild->Index) == 0)
			{
				gQueryManager.Close();
				return 0;
			}
			else
			{
				gQueryManager.Close();
				lpSourceGuild->Rival = lpTargetGuild->Index;
			}

			gGuild.UnionInfoSend(lpSourceGuild->Index,2);

			gGuild.UnionInfoSend(lpTargetGuild->Index,2);

			RivalGuildNumber[RivalGuildAmount++] = lpSourceGuild->Index;

			RivalGuildNumber[RivalGuildAmount++] = lpTargetGuild->Index;

			gGuild.UnionUpdateSend(0,RivalGuildAmount,(int*)RivalGuildNumber);

			return 1;
	}

	return 0;
}

BYTE CGuildManager::DelGuildRelationship(int guild01,int relation)
{
	GUILD_INFO* lpGuildInfo = this->GetGuildInfo(guild01);

	if(lpGuildInfo == 0)
	{
		return 0;
	}

	GUILD_INFO* lpUnionInfo = 0;

	GUILD_INFO* lpRivalInfo = 0;

	DWORD UnionGuildNumber[MAX_GUILD_UNION];

	DWORD RivalGuildNumber[MAX_GUILD_RIVAL];

	DWORD UnionGuildAmount = 0;

	DWORD RivalGuildAmount = 0;

	switch(relation)
	{
		case 1:
			if(this->CheckGuildOnCS(lpGuildInfo->Name) != 0)
			{
				return 16;
			}

			if(lpGuildInfo->Index == lpGuildInfo->Union)
			{
				return 0;
			}

			if((lpUnionInfo=this->GetGuildInfo(lpGuildInfo->Union)) == 0)
			{
				return 0;
			}

			if((UnionGuildAmount=this->GetUnionList(lpUnionInfo->Index,UnionGuildNumber)) == 0)
			{
				return 0;
			}

			if(UnionGuildAmount == 2)
			{
				if(gQueryManager.ExecQuery("UPDATE Guild SET G_Union=0 WHERE Number IN (%d,%d)",UnionGuildNumber[0],UnionGuildNumber[1]) == 0)
				{
					gQueryManager.Close();
					return 0;
				}
				else
				{
					gQueryManager.Close();
					lpGuildInfo->Union = 0;
					lpUnionInfo->Union = 0;
					gGuild.UnionInfoSend(lpUnionInfo->Index,1);
					gGuild.UnionUpdateSend(16,UnionGuildAmount,(int*)UnionGuildNumber);
					return 1;
				}
			}
			else
			{
				if(gQueryManager.ExecQuery("UPDATE Guild SET G_Union=0 WHERE Number=%d",lpGuildInfo->Index) == 0)
				{
					gQueryManager.Close();
					return 0;
				}
				else
				{
					gQueryManager.Close();
					lpGuildInfo->Union = 0;
					gGuild.UnionInfoSend(lpUnionInfo->Index,1);
					gGuild.UnionUpdateSend(0,UnionGuildAmount,(int*)UnionGuildNumber);
					return 1;
				}
			}

			break;
		case 2:
			if(this->CheckGuildOnCS(lpGuildInfo->Name) != 0)
			{
				return 16;
			}

			if((lpRivalInfo=this->GetGuildInfo(lpGuildInfo->Rival)) == 0)
			{
				return 0;
			}

			if(gQueryManager.ExecQuery("UPDATE Guild SET G_Rival=0 WHERE Number IN (%d,%d)",lpGuildInfo->Index,lpRivalInfo->Index) == 0)
			{
				gQueryManager.Close();
				return 0;
			}
			else
			{
				gQueryManager.Close();
				lpGuildInfo->Rival = 0;
				lpRivalInfo->Rival = 0;
				gGuild.UnionInfoSend(lpGuildInfo->Index,2);
				gGuild.UnionInfoSend(lpRivalInfo->Index,2);
				RivalGuildNumber[RivalGuildAmount++] = lpGuildInfo->Index;
				RivalGuildNumber[RivalGuildAmount++] = lpRivalInfo->Index;
				gGuild.UnionUpdateSend(0,RivalGuildAmount,(int*)RivalGuildNumber);
				return 1;
			}

			break;
	}

	return 0;
}

BYTE CGuildManager::SetGuildRelationship(char* szGuildMember,char* szGuildMaster)
{
	GUILD_INFO* lpSourceGuild = this->GetGuildInfo(szGuildMember);

	GUILD_INFO* lpTargetGuild = this->GetGuildInfo(szGuildMaster);

	if(lpSourceGuild == 0)
	{
		return 0;
	}

	if(lpTargetGuild == 0)
	{
		return 0;
	}

	if(lpSourceGuild->Union != lpTargetGuild->Index)
	{
		return 0;
	}

	if(lpTargetGuild->Union != lpTargetGuild->Index)
	{
		return 0;
	}

	if(this->CheckGuildOnCS(lpSourceGuild->Name) != 0)
	{
		return 16;
	}

	if(this->CheckGuildOnCS(lpTargetGuild->Name) != 0)
	{
		return 16;
	}

	DWORD UnionGuildNumber[MAX_GUILD_UNION];

	DWORD UnionGuildAmount = this->GetUnionList(lpTargetGuild->Index,UnionGuildNumber);

	if(UnionGuildAmount == 2)
	{
		if(gQueryManager.ExecQuery("UPDATE Guild SET G_Union=0 WHERE Number IN (%d,%d)",lpSourceGuild->Index,lpTargetGuild->Index) == 0)
		{
			gQueryManager.Close();

			return 0;
		}
		else
		{
			gQueryManager.Close();

			lpSourceGuild->Union = 0;

			lpTargetGuild->Union = 0;

			gGuild.UnionInfoSend(lpTargetGuild->Index,1);

			gGuild.UnionUpdateSend(16,UnionGuildAmount,(int*)UnionGuildNumber);

			return 1;
		}
	}
	else
	{
		if(gQueryManager.ExecQuery("UPDATE Guild SET G_Union=0 WHERE Number=%d",lpSourceGuild->Index) == 0)
		{
			gQueryManager.Close();

			return 0;
		}
		else
		{
			gQueryManager.Close();

			lpSourceGuild->Union = 0;

			gGuild.UnionInfoSend(lpTargetGuild->Index,1);

			gGuild.UnionUpdateSend(0,UnionGuildAmount,(int*)UnionGuildNumber);

			return 1;
		}
	}
}

BYTE CGuildManager::SetGuildScore(char* szGuildName,DWORD dwScore)
{
	GUILD_INFO* lpGuildInfo = this->GetGuildInfo(szGuildName);

	if(lpGuildInfo == 0){return 0;}

	if(gQueryManager.ExecQuery("UPDATE Guild SET G_Score=%d WHERE G_Name='%s'",dwScore,szGuildName) == 0)
	{
		gQueryManager.Close();

		return 0;
	}
	else
	{
		gQueryManager.Close();

		lpGuildInfo->Score = dwScore;

		return 1;
	}
}

BYTE CGuildManager::SetGuildNotice(char* szGuildName,char* szNotice)
{
	GUILD_INFO* lpGuildInfo = this->GetGuildInfo(szGuildName);

	if(lpGuildInfo == 0){return 0;}

	gQueryManager.BindParameterAsString(1,szNotice,sizeof(lpGuildInfo->Notice));

	if(gQueryManager.ExecQuery("UPDATE Guild SET G_Notice=? WHERE G_Name='%s'",szGuildName) == 0)
	{
		gQueryManager.Close();

		return 0;
	}
	else
	{
		gQueryManager.Close();

		memcpy(lpGuildInfo->Notice,szNotice,sizeof(lpGuildInfo->Notice));

		return 1;
	}
}

BYTE CGuildManager::SetGuildType(char* szGuildName,int btType)
{
	GUILD_INFO* lpGuildInfo = this->GetGuildInfo(szGuildName);

	if(lpGuildInfo == 0){return 0;}

	if(gQueryManager.ExecQuery("UPDATE Guild SET G_Type=%d WHERE G_Name='%s'",btType,szGuildName) == 0)
	{
		gQueryManager.Close();

		return 0;
	}
	else
	{
		gQueryManager.Close();

		lpGuildInfo->Type = btType;

		return 1;
	}
}

BYTE CGuildManager::SetGuildMemberStatus(char* szGuildMember,int btStatus)
{
	GUILD_MEMBER_INFO* lpGuildMemberInfo = this->GetGuildMemberInfo(szGuildMember);

	if(lpGuildMemberInfo == 0){return 0;}

	if(gQueryManager.ExecQuery("UPDATE GuildMember SET G_Status=%d WHERE Name='%s'",btStatus,szGuildMember) == 0)
	{
		gQueryManager.Close();

		return 0;
	}
	else
	{
		gQueryManager.Close();

		lpGuildMemberInfo->Status = btStatus;

		return 1;
	}
}

long CGuildManager::GetUnionList(DWORD dwUnionNumber,DWORD* lpUnionList)
{
	GUILD_INFO* lpGuildInfo = this->GetGuildInfo(dwUnionNumber);

	if(lpGuildInfo == 0)
	{
		return 0;
	}

	if(lpGuildInfo->Union == 0)
	{
		return 0;
	}

	long count = 0;

	lpUnionList[count++] = lpGuildInfo->Index;

	for(std::vector<GUILD_INFO>::iterator it=this->vGuildList.begin();it != this->vGuildList.end();it++)
	{
		if(it->Index != lpGuildInfo->Index && it->Union == lpGuildInfo->Index)
		{
			lpUnionList[count++] = it->Index;
			if(count >= MAX_GUILD_UNION){break;}
		}
	}

	return count;
}

long CGuildManager::GetRivalList(DWORD dwRivalNumber,DWORD* lpRivalList)
{
	GUILD_INFO* lpGuildInfo = this->GetGuildInfo(dwRivalNumber);

	if(lpGuildInfo == 0)
	{
		return 0;
	}

	if(lpGuildInfo->Rival == 0)
	{
		return 0;
	}

	long count = 0;

	lpRivalList[count++] = lpGuildInfo->Rival;

	return count;
}
