#pragma once

#include <vector>
#include "../interface.h"
//#include "NetCli_DepartmentStore.h"
class CMerchantFrame;
extern CMerchantFrame*				g_pMerchantFrame;

using namespace std;
/*

struct ShoppingCartItem
{
	bool m_update;
	int m_type;

	int objectID;
	int price;
	int stock;
	int index;						// 在資料庫點數商品索引值
};*/
struct DepartmentStoreBaseInfoStruct;

enum TopupStatusENUM
{
	EM_TopupStatus_StandBy	  ,
	EM_TopupStatus_WaitSession,
	EM_TopupStatus_WaitOpen	  ,
};

enum ZShopStatusENUM
{
	EM_ZShopStatus_StandBy	  ,
	EM_ZShopStatus_WaitSession,
	EM_ZShopStatus_WaitOpen	  ,
};

class CMerchantFrame : public CInterfaceBase
{
public:
	CMerchantFrame(CInterface* object);
	virtual ~CMerchantFrame();

public:
	virtual	void RegisterFunc();
	virtual	void Update(float elapsedTime);
	virtual	int	 GetItemNums()					{ return m_itemNums; }
	virtual void SetFilterType( int type );
	virtual int	 GetFilterType();

public:
	void SaveClientData();// 要求 Server 儲存資料
	void LoadClientData(int type, int size, char* data);// 取得由 Server 讀取資料	

public:
	vector<DepartmentStoreBaseInfoStruct*>& GetNowItemList();
	DepartmentStoreBaseInfoStruct* GetItemInfo(int index,int GUID);
	void BuildFrames(void);
	void DestroyFrames(void);
	void LeaveWorld();
	void SaveGamebarLog();
	void ImportAccountMoneyResult(int Runes);
	void RunningMessage( vector<string>& MsgList );
	void MailGiftResult( int Result );
	void SlotMachinePlayResult( int result , int  itemDbid,int count );
	void SlotMachineInfoChange();
	void SlotMachineInfo( int itemID[] , int itemCount[] );
	void ShopInfo(int ItemCount , DepartmentStoreBaseInfoStruct Item[500] );
	void ItemType(int TypeCount , int * TypeList );
	void BuyItemResult(int  Result);
	void setDisableSendGift(bool IsDisableSendGift );
	void AddGameBarLogs();
	void ChangeZone();

public:
	int	m_GameBarCost;

protected:
	bool m_bDirty;
	bool m_itemUpdate;
	int	 m_itemNums;
	int	 m_FilterType;
	
protected:
	TopupStatusENUM m_TopupStatus;
	unsigned long	m_TopupWaitTick;
	std::string		m_TouupUrl;

	ZShopStatusENUM m_ZShopStatus;
	unsigned long	m_ZShopWaitTick;

public:
	TopupStatusENUM GetTopupStatus()											{return m_TopupStatus;}
	void			TopupOpenSession(const char* url);
	void			TopupOpenSessionResult(int sessiontype, int sessionid);
	void			TopupOpenWebPage();

public:
	ZShopStatusENUM GetZShopStatus()											{return m_ZShopStatus;}
	void			ZShopOpenSession();
	void			ZShopOpenSessionResult(int sessiontype, int sessionid);
	void			ZShopOpenWebPage();
};