#pragma once

#include "../interface.h"
#include "../../netwaker_member/net_item/NetCli_Item.h"
#include <vector>
using namespace std;
class CBagFrame;
extern CBagFrame*			g_pBagFrame;

struct LockedPendingSlot {
	int slot;					// 背包欄位位置
	int destType;				// 目標類型
	int destIndex;				// 目標位置
};





class CBagFrame : public CInterfaceBase
{
public:
	CBagFrame(CInterface* object);
	virtual ~CBagFrame();

	int					AssignQueueItemToBag();
	void				ReceiveQueueItemToBag(int ItemID);
	void				GiveNewItem();
	void				InsertItemQueueItem();
	void				ReflashItemQueueTime();
	void				ClearAllQueueItem();

	void				RegisterFunc();
	void				BuildFrames();
	void				DestroyFrames();
	void				Update(float elapsedTime);
	void				Close();
	bool				Use(int index, bool fromBag=false);	// 使用
	bool				UseItemByName(const char* name);	// 使用
	bool				Cut(int index);						// 剪下
	void				Paste(int index);					// 貼上
	int					GetIndex(int objectID);
	int					GetIndex(const char* name);
	void				Locked(int index);
	void				Unlocked(int index);
	int					FindIndexByName(const char* name);
	int					SearchItem(int createTime, int serial);
	void				UpdateItem(int pos=-1);

	virtual	void				UpdateCooldown(int ColdownClass, int ColdownType);

	// 背包物品對物品使用
	void				SetUseItem(int index);
	bool				UseItemToItem(int targetType, int index);
	void				UseItemCancel();	
	void				BagItemUpdate(void);		
	int					GetUseItemIndex()					{ return m_useIndex; }
	void				ClearPickupItemIndex()				{ m_pickupItemIndex = -1; }	

	int					GetUseItemToLocationID()			{ return m_useToLocation; }
	void				CancelUseItemToLocation();

	virtual	bool				IsLockedQueue()						{ return m_lockQueue; }
	virtual void				SetQueueLocked(bool locked)			{ m_lockQueue = locked; }
	
	virtual void				UnlockPendingItem(const char* password);
	virtual void				CancelUnlockedItem();
	virtual const char*			GetPassword()						{ return m_password.c_str(); }

	void OpenItemMarge(int npc, int pos , int Type);
	void CloseItemMarge(void);
	void ItemMargeResult(bool Result);
	void MargeItemApply(int a,int b);
	void MargeItemClose(void);
	void MagicBoxAttrDrawOffResult(bool Result);

	void						ClearGarbageItems();
	int							GetGarbageMaxItems()				{ return (int)m_garbage.size(); }
	int							GetGarbageItemIndex(int index);

	bool						InsertGarbageItem(int bagIndex, bool ignoreCursorItem=false);
	bool						RemoveGarbageItem(int bagIndex);
	bool						DeleteGarbageItems();
	void						UpdateGarbageItem();

	void						SortItems(int page);
	void						InsertSortItem(int index);
	void						DeleteSortItem(int index);

	//virtual	void				UpdateBagItem();	


	void						ChangeZone();
	void						RefreshCoin();
	int							GetBilldinCount()					{ return m_billdin; }

protected:
	float						m_time;
	int							m_pickupItemIndex;				// 拿取物品編號
	int							m_useIndex;						// 記錄物品對物品編號
	int							m_useToLocation;				// 物品對地使用
	
	bool						m_lockQueue;					// 是否物品分類中
	bool						m_waitQueueUpdate;	
	float						m_lockedQueueTime;				// 鎖住物品分配時間(避免封包過時未回應)
	int							m_lockedPos;					// 鎖住位置
	int                         m_ItemMargePos;

	int							m_unlockSlot;					// 背包所在欄位
	int							m_unlockType;					// 目標類型(0背包 1銀行 2裝備)
	int							m_unlockPos;					// 目標位置
	string						m_password;						// 解鎖密碼

	bool						m_updateIndex;					// 更新背包索引值	
	vector<int>					m_garbage;						// 垃圾桶索引值
	int							m_itemPos[_MAX_BODY_COUNT_];	// 最後更新顯示狀態
	int							m_billdin;						// 必爾丁代幣

public:
	int							m_splitBagItemIndex;			// 分堆物品編號
	int							m_splitBagItemCount;			// 分堆物品數量


public:

	enum EGambleStep
	{
		GS_NoItem  = 0,
		GS_Ready   ,
		GS_Roll    , //按了Roll 等待封包
		GS_Rolling ,
		GS_Stop    , //按了停止 等待封包
 		GS_Stoping ,
 		//GS_Stoped  , //Result
		GS_Result  ,
 		GS_Exchange, //交換中
	};

	EGambleStep m_eGambleStep;

	//Gamble
	int m_GambleBagItemIndex;                //


	int m_Gamble_AttributeLv   ; //物品平均等級
	int m_Gamble_FirstTimeCost ;
	int m_Gamble_ContinueCost  ;


	//bool m_Gamble_IsContinue; //
	vector<int> m_GamblePrizes;

  	int m_GamblePrizesIndex;
   	int m_GambleAbilityIndex;

	int m_GambleLockFlag;

	bool m_isGiveUp;

	int m_GambleManage;

	bool Gamble_ItemChanged( int index );
  
	void Gamble_SetBagItemIndex( int index );
	void Gamble_SetGambleTableIndex( int index );

	void Gamble_PickupItem( );
	void Gamble_ClearItem( );
 	void Gamble_GiveUp( );

	int m_Gamble_TableID;
	int m_Gamble_StatePos;
	int m_Gamble_StateID;
	GamebleItemStateResultENUM m_Gamble_Result;

	void Gamble_Roll( );
 	void Gamble_Stop( );

	void R_GamebleItemStateResult( int TableID , int StatePos , int StateID , GamebleItemStateResultENUM Result );
	void R_GetGamebleItemStateResult( bool Result );

	void R_OpenGamebleItemState( int TargetID );
	void R_CloseGamebleItemState( );

};



//鎖定包包物品
class CBagItemIndex
{
	int m_index;
public:
	CBagItemIndex();
	CBagItemIndex( int index );
	~CBagItemIndex();
public:
	//operators
	//CBagItemIndex& operator = ( int index );
	void operator = ( int index );
	bool operator == ( int index ) const;
	operator int() const;
public:
	//將item放到Clipboard 並將m_index = -1
	void Cut();
};
