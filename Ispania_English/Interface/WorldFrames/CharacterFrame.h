#pragma once

#include <map>
#include <vector>
#include "../interface.h"

class CCharacterFrame;
extern CCharacterFrame*			g_pCharacterFrame;

using namespace std;

struct sTitleInfo
{
	//char* pName;
	int   nID;
	bool  bGeted;
};

typedef union
{
	int val;
	struct {
		short type;					// 來源類型0 背包 1銀行	3 房屋容器
		short index;				// 所在索引編號	
		int ContainerID;			// 房屋容器使用 容器ID
	};
}EquipPendingSlot;

class CCharacterFrame : public CInterfaceBase
{
public:
	CCharacterFrame(CInterface* object);
	virtual ~CCharacterFrame();

	void				RegisterFunc();
	void				BuildFrames();
	void				Update(float elapsedTime);
	void				ChangeZone();

	bool				Cut(int index);
	void				Paste(int index);
	bool				Use(int index);
	void				UpdateCooldown(int ColdownClass, int ColdownType);
	void				EquipPendingItem(int slot);
	void				CancelPendingItem(int slot);
	void				Equip(int slot, int type, int index,int ContainerID = -1);
	void				SendEquip(int slot, int type, int index, int ContainerID);
	void				PendingEquip(int solt, int type, int index, int ContainerID);
	int					SearchEquip(int createTime, int serial);	
	void				SwapEquip(int slot=0);
	void				SwapEquipResult(bool success);
	void				UnlockSwapEquip()							{ m_swapLocked = false ; }
	void				ClearPickupItemIndex()						{ m_pickupItemIndex = -1; }
	void SaveClientData();
	void LoadClientData(int type, int size, char* data);

	//
    void S_SetTitleRequest( int TitleID );
	void R_SetTitle_OK( int TitleID );
    void R_SetTitle_Failed( );
	
	void UpdateTitleInfo();   //更新稱號資訊
	void ClearTitleInfo();    //清除稱號資訊 
	int GetTitleCount();      //取得稱號數量
	int GetTitleMaxLv();	  //取得最高稱號等級
	sTitleInfo* GetTitleInfoByIndex( int index ); //取得稱號資訊

	//酷裝
	void SetCoolSuitIndex( int index );
	void PickupCoolSuit( int index );
	void CheckSlotTab();

	void CoolSuitSetupChanged();

	string GetCoolSuitPageTexture( int PageID );

protected:
	map<int, EquipPendingSlot>	m_pendingSolt;
	int							m_pickupItemIndex;
	bool						m_secondEquip;						// 是否為第二套裝備
	bool						m_swapLocked;						//
	vector<sTitleInfo>			m_TitleInfos;
	bool						m_bTitleRequested;

public:
	char						m_SlotTab[5];
	int m_slotIndex;
	//酷裝 設定用
	CoolSuitBase m_CoolSuitSetupTemp;
	bool m_bCoolSuitSetuping;

	int m_CoolClothBag_Pickup_Pending_Pos;
	int m_CoolClothBag_Pickup_Pending_Index;
	int m_CoolClothBag_Pickup_Pending_ObjectID;

	//作延遲 讓玩家不能連續切頁
	int m_CoolSuitIndex;

};


