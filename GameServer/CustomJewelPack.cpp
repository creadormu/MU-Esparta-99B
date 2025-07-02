// CustomJewelPack.cpp: implementation of the CCustomJewelPack class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomJewelPack.h"
#include "CommandManager.h"
#include "DSProtocol.h"
#include "ItemManager.h"
#include "Log.h"
#include "Message.h"
#include "Notice.h"
#include "Util.h"

CCustomJewelPack gCustomJewelPack;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomJewelPack::CCustomJewelPack() // OK
{

}

CCustomJewelPack::~CCustomJewelPack() // OK
{

}

void CCustomJewelPack::ReadCustomJewelPackInfo(char* section, char* path) // OK
{
	this->m_CustomJewelPackSwitch = GetPrivateProfileInt(section,"CustomJewelPackSwitch",0,path);

	GetPrivateProfileString(section,"CustomJewelPackCommandJoBSyntax","",this->m_CustomJewelPackCommandJoBSyntax,sizeof(this->m_CustomJewelPackCommandJoBSyntax),path);
	
	GetPrivateProfileString(section,"CustomJewelPackCommandJoSSyntax","",this->m_CustomJewelPackCommandJoSSyntax,sizeof(this->m_CustomJewelPackCommandJoSSyntax),path);
	
	GetPrivateProfileString(section,"CustomJewelPackCommandJoLSyntax","",this->m_CustomJewelPackCommandJoLSyntax,sizeof(this->m_CustomJewelPackCommandJoLSyntax),path);
	
	GetPrivateProfileString(section,"CustomJewelPackCommandJCRSyntax","",this->m_CustomJewelPackCommandJCRSyntax,sizeof(this->m_CustomJewelPackCommandJCRSyntax),path);
	
	GetPrivateProfileString(section,"CustomJewelPackCommandJoGSyntax","",this->m_CustomJewelPackCommandJoGSyntax,sizeof(this->m_CustomJewelPackCommandJoGSyntax),path);
	
	GetPrivateProfileString(section,"CustomJewelPackCommandJoCSyntax","",this->m_CustomJewelPackCommandJoCSyntax,sizeof(this->m_CustomJewelPackCommandJoCSyntax),path);

	#if(GAMESERVER_UPDATE>=201)
	GetPrivateProfileString(section,"CustomJewelPackCommandJoHSyntax","",this->m_CustomJewelPackCommandJoHSyntax,sizeof(this->m_CustomJewelPackCommandJoHSyntax),path);

	GetPrivateProfileString(section,"CustomJewelPackCommandJGMSyntax","",this->m_CustomJewelPackCommandJGMSyntax,sizeof(this->m_CustomJewelPackCommandJGMSyntax),path);
	
	GetPrivateProfileString(section,"CustomJewelPackCommandJLRSyntax","",this->m_CustomJewelPackCommandJLRSyntax,sizeof(this->m_CustomJewelPackCommandJLRSyntax),path);
	
	GetPrivateProfileString(section,"CustomJewelPackCommandJHRSyntax","",this->m_CustomJewelPackCommandJHRSyntax,sizeof(this->m_CustomJewelPackCommandJHRSyntax),path);
	#endif
}

void CCustomJewelPack::CommandCustomJewelPack(LPOBJ lpObj, char* arg) // OK
{
	if(this->m_CustomJewelPackSwitch == 0)
	{
		return;
	}

	if(lpObj->Interface.use != 0 || lpObj->Live == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(387));
		return;
	}

	char mode[32] = {0};

	gCommandManager.GetString(arg,mode,sizeof(mode),0);

	if(strcmp(mode,"") != 0)
	{
		if(strcmp(mode,this->m_CustomJewelPackCommandJoBSyntax) == 0)
		{
			this->JewelPack(lpObj,arg,0);
		}
		else if(strcmp(mode,this->m_CustomJewelPackCommandJoSSyntax) == 0)
		{
			this->JewelPack(lpObj,arg,1);
		}
		else if(strcmp(mode,this->m_CustomJewelPackCommandJoLSyntax) == 0)
		{
			this->JewelPack(lpObj,arg,2);
		}
		else if(strcmp(mode,this->m_CustomJewelPackCommandJCRSyntax) == 0)
		{
			this->JewelPack(lpObj,arg,3);
		}
		else if(strcmp(mode,this->m_CustomJewelPackCommandJoGSyntax) == 0)
		{
			this->JewelPack(lpObj,arg,4);
		}
		else if(strcmp(mode,this->m_CustomJewelPackCommandJoHSyntax) == 0)
		{
			this->JewelPack(lpObj,arg,5);
		}
		else if(strcmp(mode,this->m_CustomJewelPackCommandJoCSyntax) == 0)
		{
			this->JewelPack(lpObj,arg,6);
		}
		else if(strcmp(mode,this->m_CustomJewelPackCommandJGMSyntax) == 0)
		{
			this->JewelPack(lpObj,arg,7);
		}
		else if(strcmp(mode,this->m_CustomJewelPackCommandJLRSyntax) == 0)
		{
			this->JewelPack(lpObj,arg,8);
		}
		else if(strcmp(mode,this->m_CustomJewelPackCommandJHRSyntax) == 0)
		{
			this->JewelPack(lpObj,arg,9);
		}
	}
	else
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(388));
	}
}

void CCustomJewelPack::CommandCustomJewelUnpack(LPOBJ lpObj, char* arg) // OK
{
	if(this->m_CustomJewelPackSwitch == 0)
	{
		return;
	}

	if(lpObj->Interface.use != 0 || lpObj->Live == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(387));
		return;
	}

	char mode[32] = {0};

	gCommandManager.GetString(arg,mode,sizeof(mode),0);

	if(strcmp(mode,"") != 0)
	{
		if(strcmp(mode,this->m_CustomJewelPackCommandJoBSyntax) == 0)
		{
			this->JewelUnpack(lpObj,arg,0);
		}
		else if(strcmp(mode,this->m_CustomJewelPackCommandJoSSyntax) == 0)
		{
			this->JewelUnpack(lpObj,arg,1);
		}
		else if(strcmp(mode,this->m_CustomJewelPackCommandJoLSyntax) == 0)
		{
			this->JewelUnpack(lpObj,arg,2);
		}
		else if(strcmp(mode,this->m_CustomJewelPackCommandJCRSyntax) == 0)
		{
			this->JewelUnpack(lpObj,arg,3);
		}
		else if(strcmp(mode,this->m_CustomJewelPackCommandJoGSyntax) == 0)
		{
			this->JewelUnpack(lpObj,arg,4);
		}
		else if(strcmp(mode,this->m_CustomJewelPackCommandJoHSyntax) == 0)
		{
			this->JewelUnpack(lpObj,arg,5);
		}
		else if(strcmp(mode,this->m_CustomJewelPackCommandJoCSyntax) == 0)
		{
			this->JewelUnpack(lpObj,arg,6);
		}
		else if(strcmp(mode,this->m_CustomJewelPackCommandJGMSyntax) == 0)
		{
			this->JewelUnpack(lpObj,arg,7);
		}
		else if(strcmp(mode,this->m_CustomJewelPackCommandJLRSyntax) == 0)
		{
			this->JewelUnpack(lpObj,arg,8);
		}
		else if(strcmp(mode,this->m_CustomJewelPackCommandJHRSyntax) == 0)
		{
			this->JewelUnpack(lpObj,arg,9);
		}
	}
	else
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(388));
	}
}

void CCustomJewelPack::JewelPack(LPOBJ lpObj,char* arg,int type) // OK
{
	int value = gCommandManager.GetNumber(arg,1);

	if(value != 10 && value != 20 && value != 30)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(389));
		return;
	}

	int count = gItemManager.GetInventoryItemCount(lpObj,this->GetJewelSimpleIndex(type),0);

	if(count == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(390));
		return;
	}

	if(value > count)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(391));
		return;
	}

	gItemManager.DeleteInventoryItemCount(lpObj,this->GetJewelSimpleIndex(type),0,value);

	GDCreateItemSend(lpObj->Index,0xEB,0,0,this->GetJewelBundleIndex(type),((value/10)-1),0,0,0,0,-1,0,0,0,0,0,0xFF,0);

	gLog.Output(LOG_COMMAND,"[CommandCustomJewelPack][Pack][%s][%s] - (ItemIndex: %d, ItemLevel: %d)",lpObj->Account,lpObj->Name,this->GetJewelBundleIndex(type),((value/10)-1));

	gCommandManager.DiscountRequirement(lpObj,COMMAND_CUSTOM_JEWEL_PACK);
}

void CCustomJewelPack::JewelUnpack(LPOBJ lpObj,char* arg,int type) // OK
{
	int value = gCommandManager.GetNumber(arg,1);

	if(value != 10 && value != 20 && value != 30)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(389));
		return;
	}

	int count = gItemManager.GetInventoryItemCount(lpObj,this->GetJewelBundleIndex(type),((value/10)-1));

	if(count == 0)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(390));
		return;
	}

	if(gItemManager.GetInventoryEmptySlotCount(lpObj) < value)
	{
		gNotice.GCNoticeSend(lpObj->Index,1,0,0,0,0,0,gMessage.GetMessage(392));
		return;
	}

	gItemManager.DeleteInventoryItemCount(lpObj,this->GetJewelBundleIndex(type),((value/10)-1),1);

	for(int n=0;n < value;n++)
	{
		GDCreateItemSend(lpObj->Index,0xEB,0,0,this->GetJewelSimpleIndex(type),0,0,0,0,0,-1,0,0,0,0,0,0xFF,0);
	}

	gLog.Output(LOG_COMMAND,"[CommandCustomJewelPack][Unpack][%s][%s] - (ItemIndex: %d, ItemCount: %d)",lpObj->Account,lpObj->Name,this->GetJewelSimpleIndex(type),value);

	gCommandManager.DiscountRequirement(lpObj,COMMAND_CUSTOM_JEWEL_UNPACK);
}

int CCustomJewelPack::GetJewelSimpleIndex(int type) // OK
{
	switch(type)
	{
		case 0:
			return GET_ITEM(14,13);
		case 1:
			return GET_ITEM(14,14);
		case 2:
			return GET_ITEM(14,16);
		case 3:
			return GET_ITEM(14,22);
		case 4:
			return GET_ITEM(14,31);
		case 5:
			return GET_ITEM(14,42);
		case 6:
			return GET_ITEM(12,15);
		case 7:
			return GET_ITEM(14,41);
		case 8:
			return GET_ITEM(14,43);
		case 9:
			return GET_ITEM(14,44);
	}

	return -1;
}

int CCustomJewelPack::GetJewelBundleIndex(int type) // OK
{
	switch(type)
	{
		case 0:
			return GET_ITEM(12,30);
		case 1:
			return GET_ITEM(12,31);
		case 2:
			return GET_ITEM(12,136);
		case 3:
			return GET_ITEM(12,137);
		case 4:
			return GET_ITEM(12,138);
		case 5:
			return GET_ITEM(12,140);
		case 6:
			return GET_ITEM(12,141);
		case 7:
			return GET_ITEM(12,139);
		case 8:
			return GET_ITEM(12,142);
		case 9:
			return GET_ITEM(12,143);
	}

	return -1;
}