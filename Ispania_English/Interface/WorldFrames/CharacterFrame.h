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
		short type;					// �ӷ�����0 �I�] 1�Ȧ�	3 �Ыήe��
		short index;				// �Ҧb���޽s��	
		int ContainerID;			// �Ыήe���ϥ� �e��ID
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
	
	void UpdateTitleInfo();   //��s�ٸ���T
	void ClearTitleInfo();    //�M���ٸ���T 
	int GetTitleCount();      //���o�ٸ��ƶq
	int GetTitleMaxLv();	  //���o�̰��ٸ�����
	sTitleInfo* GetTitleInfoByIndex( int index ); //���o�ٸ���T

	//�Ÿ�
	void SetCoolSuitIndex( int index );
	void PickupCoolSuit( int index );
	void CheckSlotTab();

	void CoolSuitSetupChanged();

	string GetCoolSuitPageTexture( int PageID );

protected:
	map<int, EquipPendingSlot>	m_pendingSolt;
	int							m_pickupItemIndex;
	bool						m_secondEquip;						// �O�_���ĤG�M�˳�
	bool						m_swapLocked;						//
	vector<sTitleInfo>			m_TitleInfos;
	bool						m_bTitleRequested;

public:
	char						m_SlotTab[5];
	int m_slotIndex;
	//�Ÿ� �]�w��
	CoolSuitBase m_CoolSuitSetupTemp;
	bool m_bCoolSuitSetuping;

	int m_CoolClothBag_Pickup_Pending_Pos;
	int m_CoolClothBag_Pickup_Pending_Index;
	int m_CoolClothBag_Pickup_Pending_ObjectID;

	//�@���� �����a����s�����
	int m_CoolSuitIndex;

};


