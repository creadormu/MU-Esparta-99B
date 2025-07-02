// GuildManager.h: interface for the CGuildManager class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#define MAX_GUILD_UNION 5
#define MAX_GUILD_RIVAL 2
#define MAX_GUILD_MEMBER 80

struct GUILD_MEMBER_INFO
{
	void Clear()
	{
		memset(this->Name,0,sizeof(this->Name));

		this->Server = -1;

		this->Status = -1;
	}

	bool IsEmpty()
	{
		return ((this->Name[0]==0)?1:0);
	}

	char Name[11];
	int Status;
	int Server;
};

struct GUILD_INFO
{
	void Clear()
	{
		this->Index = 0;

		this->Union = 0;

		this->Rival = 0;

		this->Score = 0;

		this->Type = 0;

		memset(this->Name,0,sizeof(this->Name));

		memset(this->Master,0,sizeof(this->Master));

		memset(this->Notice,0,sizeof(this->Notice));

		memset(this->Mark,0,sizeof(this->Mark));

		for(int n=0;n < MAX_GUILD_MEMBER;n++){this->Members[n].Clear();}
	}

	BYTE GetMemberCount()
	{
		BYTE count = 0;

		for(int n=0;n < MAX_GUILD_MEMBER;n++)
		{
			if(this->Members[n].IsEmpty() == 0)
			{
				count++;
			}
		}

		return count;
	}

	DWORD Index;
	DWORD Union;
	DWORD Rival;
	DWORD Score;
	BYTE Type;
	char Name[9];
	char Master[11];
	char Notice[60];
	BYTE Mark[32];
	GUILD_MEMBER_INFO Members[MAX_GUILD_MEMBER];
};

class CGuildManager
{
public:
	CGuildManager();
	virtual ~CGuildManager();
	void Init();
	GUILD_INFO* GetGuildInfo(char* szName);
	GUILD_INFO* GetGuildInfo(DWORD dwNumber);
	GUILD_INFO* GetMemberGuildInfo(char* szGuildMember);
	GUILD_MEMBER_INFO* GetGuildMemberInfo(char* szGuildMember);
	BOOL CheckGuildOnCS(char* szGuildName);
	BYTE AddGuild(char* szGuildName,char* szMasterName,BYTE* lpMark,int btType);
	BYTE DelGuild(char* szGuildName);
	BYTE AddGuildMember(char* szGuildName,char* szGuildMember,int btStatus,int btServer);
	BYTE DelGuildMember(char* szGuildMember);
	BYTE AddGuildRelationship(int guild01,int guild02,int relation);
	BYTE DelGuildRelationship(int guild01,int relation);
	BYTE SetGuildRelationship(char* szGuildMember,char* szGuildMaster);
	BYTE SetGuildScore(char* szGuildName,DWORD dwScore);
	BYTE SetGuildNotice(char* szGuildName,char* szNotice);
	BYTE SetGuildType(char* szGuildName,int btType);
	BYTE SetGuildMemberStatus(char* szGuildMember,int btStatus);
	long GetUnionList(DWORD dwUnionNumber,DWORD* lpUnionList);
	long GetRivalList(DWORD dwRivalNumber,DWORD* lpRivalList);
private:
	std::vector<GUILD_INFO> vGuildList;
};

extern CGuildManager gGuildManager;
