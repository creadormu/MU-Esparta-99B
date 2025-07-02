// ItemOptionManager.cpp: implementation of the CItemOptionManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ItemOptionRate.h"
#include "CustomWing.h"
#include "ExcellentOptionRate.h"
#include "ItemManager.h"
#include "MemScript.h"
#include "PentagramSystem.h"
#include "RandomManager.h"
#include "SetItemType.h"
#include "SocketItemType.h"
#include "Util.h"
#include "WingOption.h"

CItemOptionRate gItemOptionRate;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemOptionRate::CItemOptionRate() // OK
{
	this->m_ItemOption0RateInfo.clear();

	this->m_ItemOption1RateInfo.clear();

	this->m_ItemOption2RateInfo.clear();

	this->m_ItemOption3RateInfo.clear();

	this->m_ItemOption4RateInfo.clear();

	this->m_ItemOption5RateInfo.clear();

	this->m_ItemOption6RateInfo.clear();
}

CItemOptionRate::~CItemOptionRate() // OK
{

}

void CItemOptionRate::Load(char* path) // OK
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

	this->m_ItemOption0RateInfo.clear();

	this->m_ItemOption1RateInfo.clear();

	this->m_ItemOption2RateInfo.clear();

	this->m_ItemOption3RateInfo.clear();

	this->m_ItemOption4RateInfo.clear();

	this->m_ItemOption5RateInfo.clear();

	this->m_ItemOption6RateInfo.clear();

	try
	{
		while(true)
		{
			if(lpMemScript->GetToken() == TOKEN_END)
			{
				break;
			}

			int section = lpMemScript->GetNumber();

			while(true)
			{
				if(section == 0)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					ITEM_OPTION0_RATE_INFO info;

					info.Index = lpMemScript->GetNumber();

					for(int n=0;n < MAX_ITEM_OPTION0_RATE;n++)
					{
						info.Rate[n] = lpMemScript->GetAsNumber();
					}

					this->m_ItemOption0RateInfo.insert(std::pair<int,ITEM_OPTION0_RATE_INFO>(info.Index,info));
				}
				else if(section == 1)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					ITEM_OPTION1_RATE_INFO info;

					info.Index = lpMemScript->GetNumber();

					for(int n=0;n < MAX_ITEM_OPTION1_RATE;n++)
					{
						info.Rate[n] = lpMemScript->GetAsNumber();
					}

					this->m_ItemOption1RateInfo.insert(std::pair<int,ITEM_OPTION1_RATE_INFO>(info.Index,info));
				}
				else if(section == 2)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					ITEM_OPTION2_RATE_INFO info;

					info.Index = lpMemScript->GetNumber();

					for(int n=0;n < MAX_ITEM_OPTION2_RATE;n++)
					{
						info.Rate[n] = lpMemScript->GetAsNumber();
					}

					this->m_ItemOption2RateInfo.insert(std::pair<int,ITEM_OPTION2_RATE_INFO>(info.Index,info));
				}
				else if(section == 3)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					ITEM_OPTION3_RATE_INFO info;

					info.Index = lpMemScript->GetNumber();

					for(int n=0;n < MAX_ITEM_OPTION3_RATE;n++)
					{
						info.Rate[n] = lpMemScript->GetAsNumber();
					}

					this->m_ItemOption3RateInfo.insert(std::pair<int,ITEM_OPTION3_RATE_INFO>(info.Index,info));
				}
				else if(section == 4)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					ITEM_OPTION4_RATE_INFO info;

					info.Index = lpMemScript->GetNumber();

					for(int n=0;n < MAX_ITEM_OPTION4_RATE;n++)
					{
						info.Rate[n] = lpMemScript->GetAsNumber();
					}

					this->m_ItemOption4RateInfo.insert(std::pair<int,ITEM_OPTION4_RATE_INFO>(info.Index,info));
				}
				else if(section == 5)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					ITEM_OPTION5_RATE_INFO info;

					info.Index = lpMemScript->GetNumber();

					for(int n=0;n < MAX_ITEM_OPTION5_RATE;n++)
					{
						info.Rate[n] = lpMemScript->GetAsNumber();
					}

					this->m_ItemOption5RateInfo.insert(std::pair<int,ITEM_OPTION5_RATE_INFO>(info.Index,info));
				}
				#if(GAMESERVER_UPDATE>=401)
				else if(section == 6)
				{
					if(strcmp("end",lpMemScript->GetAsString()) == 0)
					{
						break;
					}

					ITEM_OPTION6_RATE_INFO info;

					info.Index = lpMemScript->GetNumber();

					for(int n=0;n < MAX_ITEM_OPTION6_RATE;n++)
					{
						info.Rate[n] = lpMemScript->GetAsNumber();
					}

					this->m_ItemOption6RateInfo.insert(std::pair<int,ITEM_OPTION6_RATE_INFO>(info.Index,info));
				}
				#endif
				else
				{
					break;
				}
			}
		}
	}
	catch(...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

bool CItemOptionRate::GetItemOption0(int index,BYTE* option) // OK
{
	std::map<int,ITEM_OPTION0_RATE_INFO>::iterator it = this->m_ItemOption0RateInfo.find(index);

	if(it == this->m_ItemOption0RateInfo.end())
	{
		return 0;
	}

	CRandomManager RandomManager;

	for(int n=0;n < MAX_ITEM_OPTION0_RATE;n++)
	{
		RandomManager.AddElement(n,it->second.Rate[n]);
	}

	return RandomManager.GetRandomElement(option);
}

bool CItemOptionRate::GetItemOption1(int index,BYTE* option) // OK
{
	std::map<int,ITEM_OPTION1_RATE_INFO>::iterator it = this->m_ItemOption1RateInfo.find(index);

	if(it == this->m_ItemOption1RateInfo.end())
	{
		return 0;
	}

	CRandomManager RandomManager;

	for(int n=0;n < MAX_ITEM_OPTION1_RATE;n++)
	{
		RandomManager.AddElement(n,it->second.Rate[n]);
	}

	return RandomManager.GetRandomElement(option);
}

bool CItemOptionRate::GetItemOption2(int index,BYTE* option) // OK
{
	std::map<int,ITEM_OPTION2_RATE_INFO>::iterator it = this->m_ItemOption2RateInfo.find(index);

	if(it == this->m_ItemOption2RateInfo.end())
	{
		return 0;
	}

	CRandomManager RandomManager;

	for(int n=0;n < MAX_ITEM_OPTION2_RATE;n++)
	{
		RandomManager.AddElement(n,it->second.Rate[n]);
	}

	return RandomManager.GetRandomElement(option);
}

bool CItemOptionRate::GetItemOption3(int index,BYTE* option) // OK
{
	std::map<int,ITEM_OPTION3_RATE_INFO>::iterator it = this->m_ItemOption3RateInfo.find(index);

	if(it == this->m_ItemOption3RateInfo.end())
	{
		return 0;
	}

	CRandomManager RandomManager;

	for(int n=0;n < MAX_ITEM_OPTION3_RATE;n++)
	{
		RandomManager.AddElement(n,it->second.Rate[n]);
	}

	return RandomManager.GetRandomElement(option);
}

bool CItemOptionRate::GetItemOption4(int index,BYTE* option) // OK
{
	std::map<int,ITEM_OPTION4_RATE_INFO>::iterator it = this->m_ItemOption4RateInfo.find(index);

	if(it == this->m_ItemOption4RateInfo.end())
	{
		return 0;
	}

	CRandomManager RandomManager;

	for(int n=0;n < MAX_ITEM_OPTION4_RATE;n++)
	{
		RandomManager.AddElement(n,it->second.Rate[n]);
	}

	return RandomManager.GetRandomElement(option);
}

bool CItemOptionRate::GetItemOption5(int index,BYTE* option) // OK
{
	std::map<int,ITEM_OPTION5_RATE_INFO>::iterator it = this->m_ItemOption5RateInfo.find(index);

	if(it == this->m_ItemOption5RateInfo.end())
	{
		return 0;
	}

	CRandomManager RandomManager;

	for(int n=0;n < MAX_ITEM_OPTION5_RATE;n++)
	{
		RandomManager.AddElement(n,it->second.Rate[n]);
	}

	return RandomManager.GetRandomElement(option);
}

bool CItemOptionRate::GetItemOption6(int index,BYTE* option) // OK
{
	#if(GAMESERVER_UPDATE>=401)
	
	std::map<int,ITEM_OPTION6_RATE_INFO>::iterator it = this->m_ItemOption6RateInfo.find(index);

	if(it == this->m_ItemOption6RateInfo.end())
	{
		return 0;
	}

	CRandomManager RandomManager;

	for(int n=0;n < MAX_ITEM_OPTION6_RATE;n++)
	{
		RandomManager.AddElement(n,it->second.Rate[n]);
	}

	return RandomManager.GetRandomElement(option);
	
	#else
	
	return 0;

	#endif
}

void CItemOptionRate::MakeLevelOption(int MinLevel,int MaxLevel,short* level) // OK
{
	(*level) = 0;

	if(MinLevel >= MaxLevel)
	{
		(*level) = MinLevel;
		return;
	}

	(*level) = (short)(MinLevel+(GetLargeRand()%((MaxLevel-MinLevel)+1)));
}

void CItemOptionRate::MakeNewOption(int ItemIndex,int random,int value,BYTE* option) // OK
{
	(*option) = 0;

	int MaxNewOption = MAX_EXC_OPTION;

	if(ItemIndex == GET_ITEM(13,3) || ItemIndex == GET_ITEM(13,37)) // Dinorant,Fenrir
	{
		MaxNewOption = 3;
	}
	else if(gWingOption.CheckWingByItem(ItemIndex) != 0)
	{
		MaxNewOption = gWingOption.GetWingMaxNewOption(ItemIndex);
	}
	else if(gCustomWing.CheckCustomWingByItem(ItemIndex) != 0)
	{
		MaxNewOption = 4;
	}

	if(random == 0)
	{
		if(((*option) & value) == 0)
		{
			(*option) |= value;
		}
	}
	else
	{
		if(random == 2)
		{
			value = GetLargeRand()%value;
		}

		int count = 0;

		while(true)
		{
			if(count >= value)
			{
				break;
			}

			if(count >= MaxNewOption)
			{
				break;
			}

			int value = gExcellentOptionRate.GetNewOption(ItemIndex);

			if(((*option) & value) == 0)
			{
				(*option) |= value;
				count++;
			}
		}
	}
}

bool CItemOptionRate::MakeSetOption(int ItemIndex,int value,BYTE* option) // OK
{
	if(gSetItemType.CheckSetItemType(ItemIndex) == 0 || value == 0)
	{
		(*option) = 0;
		return 0;
	}

	(*option) = 0;

	if(value == 1 || value == 2)
	{
		BYTE OptionTable[MAX_SET_ITEM_OPTION_INDEX];
		int OptionCount = 0;

		for(int n=0;n < MAX_SET_ITEM_OPTION_INDEX;n++)
		{
			if(gSetItemType.GetSetItemOptionIndex(ItemIndex,n) > 0)
			{
				OptionTable[OptionCount++] = (n==0)?((value==1)?5:9):((value==2)?6:10);
			}
		}

		(*option) = ((OptionCount==0)?0:OptionTable[GetLargeRand()%OptionCount]);
	}
	else if(value == 3 || value == 4)
	{
		if(gSetItemType.GetSetItemOptionIndex(ItemIndex,0) > 0)
		{
			(*option) = ((value==3)?5:9);
		}
	}
	else if(value == 5 || value == 6)
	{
		if(gSetItemType.GetSetItemOptionIndex(ItemIndex,1) > 0)
		{
			(*option) = ((value==5)?6:10);
		}
	}

	return 1;
}

void CItemOptionRate::MakeSocketSlot(int ItemIndex,int value,BYTE* option) // OK
{
	memset(option,0xFF,MAX_SOCKET_OPTION);

	if(value == 0)
	{
		return;
	}

	int count = 0;

	for(int n=0;n < MAX_SOCKET_OPTION;n++)
	{
		if(count >= value)
		{
			return;
		}

		if(option[n] == 0xFF)
		{
			option[n] = 0xFE;
			count++;
		}
	}
}

bool CItemOptionRate::MakeSocketOption(int ItemIndex,int value,BYTE* option) // OK
{
	#if(GAMESERVER_UPDATE>=401)

	if((gSocketItemType.CheckSocketItemType(ItemIndex) == 0 && gPentagramSystem.IsPentagramItem(ItemIndex) == 0) || value == 0 || value == 0xFF)
	{
		memset(option,0xFF,MAX_SOCKET_OPTION);
		return 0;
	}

	if(gSocketItemType.CheckSocketItemType(ItemIndex) == 0)
	{
		for(int n=0;n < ((value>MAX_SOCKET_OPTION)?MAX_SOCKET_OPTION:value);n++)
		{
			option[n] = 0xFE;
		}
	}
	else
	{
		for(int n=0;n < ((value>gSocketItemType.GetSocketItemMaxSocket(ItemIndex))?gSocketItemType.GetSocketItemMaxSocket(ItemIndex):value);n++)
		{
			option[n] = 0xFE;
		}
	}

	return 1;

	#else
	
	memset(option,0xFF,MAX_SOCKET_OPTION);

	return 0;

	#endif
}
