#pragma once

#include "../interface.h"

class CStoreFrame;
extern CStoreFrame*				g_pStoreFrame;

#define MAX_STORE_ITEM			30

class CStoreFrame : public CInterfaceBase
{
public:
	CStoreFrame(CInterface* object);
	virtual ~CStoreFrame();

	enum {
		eStore_Type_Buy				= 0,
		eStore_Type_Sell			= 1,
	};

	virtual	void				RegisterFunc();
	virtual	void				Update(float elapsedTime);
	virtual	void				Open(int m_npcId, int storeId, int* itemCount);	
	virtual	void				Close();
	virtual	bool				IsOpen()						{ return m_shopId != 0; }
	virtual	void				UpdateBuy(int index, int count);
	virtual	void				UpdateSell();
	virtual	void				Cut(int state, int index);
	virtual	void				Paste(int state, int index);
	virtual	void				Buy(int state, int index, int count, bool immediately=false);
	virtual	void				Sell(int pos);

	virtual	int					GetShopId()						{ return m_shopId; }
	virtual	int					GetNpcId()						{ return m_npcId; }
	virtual int					GetShopItemIndex(int index);
	virtual	int					GetItemId(int index);
	virtual	int					GetItemStock(int index);
	virtual	int					GetCount()						{ return m_count; }
	virtual	bool				GetCanFix();
	virtual	void				SetRepairState(bool enable)		{ m_repairEnable = enable; }
	virtual	bool				GetRepairState()				{ return m_repairEnable; }

	virtual	void				ReceiveClose();
protected:
	virtual void				AddBuyItem(int index, int id, int count, bool immediately=false);
	virtual void				SendBuyItem();

protected:
	bool						m_repairEnable;	
	bool						m_receiveClose;
	int							m_shopId;						// 商店物品資料
	int							m_npcId;						// 開啟NPC編號
	int							m_count;						// 記錄總販賣種類數量
	int							m_index[MAX_STORE_ITEM];		// 物品在資料庫索引值
	int							m_itemCount[MAX_STORE_ITEM];	// 物品販賣數量

	int							m_queueID;
	int							m_queueCount;
	int							m_queueIndex;
	float						m_lastSendBuyTime;
};