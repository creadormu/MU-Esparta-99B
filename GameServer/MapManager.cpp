// MapManager.cpp: implementation of the CMapManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MapManager.h"
#include "MemScript.h"
#include "ServerInfo.h"
#include "Util.h"

CMapManager gMapManager;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMapManager::CMapManager() // OK
{
	this->m_MapManagerInfo.clear();
}

CMapManager::~CMapManager() // OK
{

}

void CMapManager::Load(char* path) // OK
{
	CMemScript* lpMemScript = new CMemScript;

	if(lpMemScript == 0)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR,path);
		return;
	}

	if(lpMemScript->SetBuffer(path) == 0)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
		delete lpMemScript;
		return;
	}

	this->m_MapManagerInfo.clear();

	try
	{
		while(true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			if(strcmp("end",lpMemScript->GetString()) == 0)
			{
				break;
			}

			MAP_MANAGER_INFO info;

			memset(&info,0,sizeof(info));

			info.Index = lpMemScript->GetNumber();

			info.NonPK = lpMemScript->GetAsNumber();

			info.ViewRange = lpMemScript->GetAsNumber();

			info.ExperienceRate = lpMemScript->GetAsNumber();

			#if(GAMESERVER_UPDATE>=401)

			info.MasterExperienceRate = lpMemScript->GetAsNumber();

			#endif

			info.ItemDropRate = lpMemScript->GetAsNumber();

			info.MoneyDropRate = lpMemScript->GetAsNumber();

			info.ExcItemDropRate = lpMemScript->GetAsNumber();

			info.SetItemDropRate = lpMemScript->GetAsNumber();

			#if(GAMESERVER_UPDATE>=401)

			info.SocketItemDrop = lpMemScript->GetAsNumber();

			#endif

			#if(GAMESERVER_UPDATE>=603)

			info.HelperEnable = lpMemScript->GetAsNumber();

			#endif

			#if(GAMESERVER_UPDATE>=501)

			info.GensBattle = lpMemScript->GetAsNumber();

			#endif

			info.ItemDrop = lpMemScript->GetAsNumber();
			
			info.PKItemDrop = lpMemScript->GetAsNumber();
			
			#if(GAMESERVER_UPDATE<=603)

			info.MonsterHPBar = lpMemScript->GetAsNumber();

			#endif
			
			info.TradeEnable = lpMemScript->GetAsNumber();
			
			info.PShopEnable = lpMemScript->GetAsNumber();
			
			info.ChatEnable = lpMemScript->GetAsNumber();
			
			info.PartyEnable = lpMemScript->GetAsNumber();
			
			info.DuelEnable = lpMemScript->GetAsNumber();

			info.GuildWarEnable = lpMemScript->GetAsNumber();

			info.SpawnGate = lpMemScript->GetAsNumber();

			info.RespawnGate = lpMemScript->GetAsNumber();

			strcpy_s(info.Name,lpMemScript->GetAsString());

			this->m_MapManagerInfo.insert(std::pair<int,MAP_MANAGER_INFO>(info.Index,info));
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

int CMapManager::IsValidMap(int index) // OK
{
	std::map<int,MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if(it != this->m_MapManagerInfo.end())
	{
		return 1;
	}

	return 0;
}

int CMapManager::GetMapNonPK(int index) // OK
{
	std::map<int,MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if(it != this->m_MapManagerInfo.end())
	{
		return ((it->second.NonPK == -1) ? gServerInfo.m_NonPK : ((it->second.NonPK == 1) ? 1 : 0));
	}

	return gServerInfo.m_NonPK;
}

int CMapManager::GetMapNonOutlaw(int index) // OK
{
	std::map<int,MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if(it != this->m_MapManagerInfo.end())
	{
		return ((it->second.NonPK == -1) ? 0 : ((it->second.NonPK == 2) ? 1 : 0));
	}

	return 0;
}

int CMapManager::GetMapViewRange(int index) // OK
{
	std::map<int,MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if(it != this->m_MapManagerInfo.end())
	{
		return ((it->second.ViewRange == -1) ? 12 : it->second.ViewRange);
	}
	
	return 12;
}

int CMapManager::GetMapExperienceRate(int index) // OK
{
	std::map<int,MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if(it != this->m_MapManagerInfo.end())
	{
		return it->second.ExperienceRate;
	}

	return 100;
}

int CMapManager::GetMapMasterExperienceRate(int index) // OK
{
	#if(GAMESERVER_UPDATE>=401)

	std::map<int,MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if(it != this->m_MapManagerInfo.end())
	{
		return it->second.MasterExperienceRate;
	}

	#endif

	return 100;
}

int CMapManager::GetMapItemDropRate(int index) // OK
{
	std::map<int,MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if(it != this->m_MapManagerInfo.end())
	{
		return it->second.ItemDropRate;
		
	}

	return 100;
}

int CMapManager::GetMoneyDropRate(int index) // OK
{
	std::map<int,MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if(it != this->m_MapManagerInfo.end())
	{
		return it->second.MoneyDropRate;
		
	}

	return 100;
}

int CMapManager::GetMapExcItemDropRate(int index) // OK
{
	std::map<int,MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if(it != this->m_MapManagerInfo.end())
	{
		return it->second.ExcItemDropRate;
	}

	return 0;
}

int CMapManager::GetMapSetItemDropRate(int index) // OK
{
	std::map<int,MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if(it != this->m_MapManagerInfo.end())
	{
		return it->second.SetItemDropRate;
	}

	return 0;
}

int CMapManager::GetMapSocketItemDrop(int index) // OK
{
	#if(GAMESERVER_UPDATE>=401)

	std::map<int,MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if(it != this->m_MapManagerInfo.end())
	{
		return it->second.SocketItemDrop;
	}

	#endif

	return 0;
}

int CMapManager::GetMapHelperEnable(int index) // OK
{
	#if(GAMESERVER_UPDATE>=603)

	std::map<int,MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if(it != this->m_MapManagerInfo.end())
	{
		return it->second.HelperEnable;
	}

	#endif

	return 0;
}

int CMapManager::GetMapGensBattle(int index) // OK
{
	#if(GAMESERVER_UPDATE>=501)

	std::map<int,MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if(it != this->m_MapManagerInfo.end())
	{
		return it->second.GensBattle;
	}

	#endif

	return 0;
}

int CMapManager::GetMapItemDrop(int index) // OK
{
	std::map<int,MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if(it != this->m_MapManagerInfo.end())
	{
		return it->second.ItemDrop;
	}

	return 0;
}

int CMapManager::GetMapPKItemDrop(int index) // OK
{
	std::map<int,MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if(it != this->m_MapManagerInfo.end())
	{
		return it->second.PKItemDrop;
	}

	return 0;
}

int CMapManager::GetMapMonsterHPBar(int index) // OK
{
	#if(GAMESERVER_UPDATE<=603)

	std::map<int,MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if(it != this->m_MapManagerInfo.end())
	{
		return ((it->second.MonsterHPBar == -1) ? gServerInfo.m_MonsterHealthBarSwitch : ((it->second.MonsterHPBar == 1) ? 1 : 0));
	}

	return gServerInfo.m_MonsterHealthBarSwitch;

	#else

	return 0;

	#endif
}

int CMapManager::GetMapTradeEnable(int index) // OK
{
	std::map<int,MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if(it != this->m_MapManagerInfo.end())
	{
		return ((it->second.TradeEnable == -1) ? gServerInfo.m_TradeSwitch : ((it->second.TradeEnable == 1) ? 1 : 0));
	}

	return gServerInfo.m_TradeSwitch;
}

int CMapManager::GetMapPShopEnable(int index) // OK
{
	std::map<int,MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if(it != this->m_MapManagerInfo.end())
	{
		return ((it->second.PShopEnable == -1) ? gServerInfo.m_PersonalShopSwitch : ((it->second.PShopEnable == 1) ? 1 : 0));
	}

	return gServerInfo.m_PersonalShopSwitch;
}

int CMapManager::GetMapChatEnable(int index) // OK
{
	std::map<int,MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if(it != this->m_MapManagerInfo.end())
	{
		return it->second.ChatEnable;
	}

	return 0;
}

int CMapManager::GetMapPartyEnable(int index) // OK
{
	std::map<int,MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if(it != this->m_MapManagerInfo.end())
	{
		return it->second.PartyEnable;
	}

	return 0;
}

int CMapManager::GetMapDuelEnable(int index) // OK
{
	std::map<int,MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if(it != this->m_MapManagerInfo.end())
	{
		return ((it->second.DuelEnable == -1) ? gServerInfo.m_DuelSwitch : ((it->second.DuelEnable == 1) ? 1 : 0));
	}

	return gServerInfo.m_DuelSwitch;
}

int CMapManager::GetMapGuildWarEnable(int index) // OK
{
	std::map<int,MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if(it != this->m_MapManagerInfo.end())
	{
		return it->second.GuildWarEnable;
	}

	return 0;
}

int CMapManager::GetSpawnGate(int index) // OK
{
	std::map<int,MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if(it != this->m_MapManagerInfo.end())
	{
		return it->second.SpawnGate;
	}

	return -1;
}

int CMapManager::GetRespawnGate(int index) // OK
{
	std::map<int,MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if(it != this->m_MapManagerInfo.end())
	{
		return it->second.RespawnGate;
	}

	return -1;
}

char* CMapManager::GetMapName(int index) // OK
{
	std::map<int,MAP_MANAGER_INFO>::iterator it = this->m_MapManagerInfo.find(index);

	if(it != this->m_MapManagerInfo.end())
	{
		return it->second.Name;
	}

	return 0;
}