#pragma once

#include "../interface.h"
#include "../../netwaker_member/net_item/NetCli_Item.h"
#include <vector>
using namespace std;
class CBagFrame;
extern CBagFrame*			g_pBagFrame;

struct LockedPendingSlot {
	int slot;					// �I�]����m
	int destType;				// �ؼ�����
	int destIndex;				// �ؼЦ�m
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
	bool				Use(int index, bool fromBag=false);	// �ϥ�
	bool				UseItemByName(const char* name);	// �ϥ�
	bool				Cut(int index);						// �ŤU
	void				Paste(int index);					// �K�W
	int					GetIndex(int objectID);
	int					GetIndex(const char* name);
	void				Locked(int index);
	void				Unlocked(int index);
	int					FindIndexByName(const char* name);
	int					SearchItem(int createTime, int serial);
	void				UpdateItem(int pos=-1);

	virtual	void				UpdateCooldown(int ColdownClass, int ColdownType);

	// �I�]���~�磌�~�ϥ�
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
	int							m_pickupItemIndex;				// �������~�s��
	int							m_useIndex;						// �O�����~�磌�~�s��
	int							m_useToLocation;				// ���~��a�ϥ�
	
	bool						m_lockQueue;					// �O�_���~������
	bool						m_waitQueueUpdate;	
	float						m_lockedQueueTime;				// ����~���t�ɶ�(�קK�ʥ]�L�ɥ��^��)
	int							m_lockedPos;					// ����m
	int                         m_ItemMargePos;

	int							m_unlockSlot;					// �I�]�Ҧb���
	int							m_unlockType;					// �ؼ�����(0�I�] 1�Ȧ� 2�˳�)
	int							m_unlockPos;					// �ؼЦ�m
	string						m_password;						// ����K�X

	bool						m_updateIndex;					// ��s�I�]���ޭ�	
	vector<int>					m_garbage;						// �U������ޭ�
	int							m_itemPos[_MAX_BODY_COUNT_];	// �̫��s��ܪ��A
	int							m_billdin;						// �����B�N��

public:
	int							m_splitBagItemIndex;			// ���磌�~�s��
	int							m_splitBagItemCount;			// ���磌�~�ƶq


public:

	enum EGambleStep
	{
		GS_NoItem  = 0,
		GS_Ready   ,
		GS_Roll    , //���FRoll ���ݫʥ]
		GS_Rolling ,
		GS_Stop    , //���F���� ���ݫʥ]
 		GS_Stoping ,
 		//GS_Stoped  , //Result
		GS_Result  ,
 		GS_Exchange, //�洫��
	};

	EGambleStep m_eGambleStep;

	//Gamble
	int m_GambleBagItemIndex;                //


	int m_Gamble_AttributeLv   ; //���~��������
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



//��w�]�]���~
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
	//�Nitem���Clipboard �ñNm_index = -1
	void Cut();
};
