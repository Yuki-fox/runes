#pragma once
#include "../interface.h"
struct LocationSaveStruct;

#define TB_ENABLE_BATTLE 1
#define TB_STATE_BATTLE 1
#define TB_ENABLE_COOLDOWN 2
#define TB_STATE_BOOKMARK 2
#define TB_STATE_TELEPORT 4
#define TB_STATE_OPENGATE 8
#define TB_STATE_OPENCHANNEL 16

#define TB_COOLDOWN_COUNT 3.0f
class CUI_TeleportBook :public CInterfaceBase 
{
public:
	CUI_TeleportBook(CInterface* object);
	int m_flag;
	float m_CoolDown;
	int m_state;
	int m_TeleportPos;
void				Update(float elapsedTime)	;

	bool CanUseTeleport(int pos);
	int CanUseTeleport1(int pos);

	void RegisterFunc();	
	int findFreeSlot();
	LocationSaveStruct*  GetTeleportInfo(int index );
	void GetTeleportItem(bool&save,bool &use1,bool &use2,bool &use3,int& savePos,int& UesPos1,int& UesPos2,int& UesPos3);
	void AddTeleportRequest(int TeleportPos );
	void AddTeleportResult( int TeleportPos , LocationSaveStruct& Info , bool Result );
	void ReportNeedItem(int index);

	void UseTeleportResult	( int Type , int ItemPos , int TeleportPos , bool Result );
	void ModifyTeleportResult	( int TeleportPos , LocationSaveStruct& Info , bool Result );
	void SwapTeleportResult	( int TeleportPos[2] , bool Result );
	void DelTeleportResult	( int TeleportPos , bool Result );
	void DelTeleportRequest	( int TeleportPos );
	void Teleport(int type,int pos);

	void DoBookMarkCasting(int TeleportPos );
	void DoTelePortCasting(int index );

	void DoOpenGateCasting(int index );

	//bool IsCasting(void);
	bool BreakCasting(void);
	void DoOpenChannelCasting(int index );

	void GetItemName(int index,string &name);

	void ChangeSlot(int pos1,int pos2);
	void PickupItem(int index);
	void GetItemIcon(int icon,char *file);
	LocationSaveStruct* GetItem(int index);
	void EditTeleportNoteRequest(int index,const char*note);

	void SendUpdateEvent(void);

	static void GetItemName(LocationSaveStruct*,char *str);
	
};

extern CUI_TeleportBook*		g_pTeleportBook;