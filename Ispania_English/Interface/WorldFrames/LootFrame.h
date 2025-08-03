#pragma once

#include <set>
#include "../interface.h"

class CLootFrame;
extern CLootFrame*				g_pLootFrame;

#define NUM_GROUP_LOOT_ITEM			6

using namespace std;

struct AssignItemInfo {
	string playerName;
	float waitTime;
	int playerDBID;
	int itemID;
	int itemVersion;
};

class CLootFrame : public CInterfaceBase
{
public:
	CLootFrame(CInterface* object);
	virtual ~CLootFrame();

	virtual	void				RegisterFunc();
	virtual	void				BuildFrames();
	virtual	void				Update(float elapsedTime);
	virtual	bool				OpenNewLootItem(int itemVersion);
	virtual	void				DeleteLootItem(int itemVersion);
	virtual	void				RefreshAssignItem();

	void						SendOpenBooty( int GItemID );
	void						OpenBooty(bool isResult, int bootyID);
	void						CloseBooty(bool onScript = false);
	void						UpdateBooty();
	int							GetBootyID()							{ return m_bootyID; }
	void						InsertUniqueAssignItem(const char* name, int playerDBID, int itemVersion, int itemID);
	void						ReceiveUniqueAssugnItem(int itemVersion, int itemID, bool hasItem);

protected:
	set<int>					m_groupLootItem;
	bool						m_sendBooty;
	int							m_bootyID;
	vector<AssignItemInfo>		m_uniqueAssignItem;					// 物品唯一指派給某人需要等待確認對方是否擁有此物品
};