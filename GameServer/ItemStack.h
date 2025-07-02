// ItemStack.h: interface for the CItemStack class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

struct ITEM_STACK_INFO
{
	int ItemIndex;
	int Level;
	int MaxStack;
	int CreateItemIndex;
};

class CItemStack
{
public:
	CItemStack();
	virtual ~CItemStack();
	void Load(char* path);
	bool CheckItemStack(int index,int level);
	int GetItemMaxStack(int index,int level);
	int GetCreateItemIndex(int index);
private:
	std::vector<ITEM_STACK_INFO> m_ItemStackInfo;
};

extern CItemStack gItemStack;
