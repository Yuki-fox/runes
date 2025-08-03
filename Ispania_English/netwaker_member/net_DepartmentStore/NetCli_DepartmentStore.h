#pragma once

#include "PG/PG_DepartmentStore.h"
#include "../../MainProc/NetGlobal.h"

class NetCli_DepartmentStore : public CNetGlobal
{
protected:
    //-------------------------------------------------------------------
    static NetCli_DepartmentStore* This;
    static bool _Init();
    static bool _Release();

	static void _PG_DepartmentStore_LtoC_ShopFunctionResult			( int Sockid , int Size , void* Data );
	static void _PG_DepartmentStore_DtoC_SellType					( int Sockid , int Size , void* Data );	
	static void _PG_DepartmentStore_DtoC_ShopInfo					( int Sockid , int Size , void* Data );	
	static void _PG_DepartmentStore_DtoC_ShopInfo_Zip				( int Sockid , int Size , void* Data );	
	static void _PG_DepartmentStore_LtoC_BuyItemResult				( int Sockid , int Size , void* Data );
	static void _PG_DepartmentStore_LtoC_ImportAccountMoneyResult	( int Sockid , int Size , void* Data );
	static void _PG_DepartmentStore_LtoC_ExchangeItemResult			( int Sockid , int Size , void* Data );
	static void _PG_DepartmentStore_DtoC_RunningMessage_Zip			( int Sockid , int Size , void* Data );
	static void _PG_DepartmentStore_LtoC_MailGiftResult				( int Sockid , int Size , void* Data );
	static void _PG_DepartmentStore_LtoC_SlotMachineInfoChange		( int Sockid , int Size , void* Data );
	static void _PG_DepartmentStore_LtoC_SlotMachineInfo			( int Sockid , int Size , void* Data );
	static void _PG_DepartmentStore_LtoC_SlotMachinePlayResult		( int Sockid , int Size , void* Data );
	static void _PG_DepartmentStore_LtoC_WebBagResult				( int Sockid , int Size , void* Data );
public:    
    NetCli_DepartmentStore() { This = this; }


	static void SL_ShopFunctionRequest	( int FunctionID );
	//要求販賣類型資訊
	static void SL_SellTypeRequest		( );
	static void SL_ShopInfoRequest		( int SellType );
	static void SL_BuyItem				( int ItemGUID , int Pos , int Count , char* Password );

	static void SL_ExchangeItemRequest	( const char* ItemSerial , const char* Password );

	//要求提領帳號幣
	static void SL_ImportAccountMoney	( );

	virtual void RL_ShopFunctionResult	( int FunctionID , bool Result  ) = 0;
	virtual void RD_ShopInfo			( int ItemCount , DepartmentStoreBaseInfoStruct Item[500] ) = 0;
	virtual void RD_SellType			( int Count , int SellType[50] ) = 0;
	virtual void RL_BuyItemResult		( DepartmentStoreBuyItemResultENUM Result ) = 0;
	virtual void RL_ImportAccountMoneyResult( int Money_Account , int Money_Bonus ) = 0;
	virtual void RL_ExchangeItemResult	( ItemFieldStruct& Item , int Money , int MoneyAccount , ItemExchangeResultENUM Result ) = 0;

	//要求商城跑馬燈
	virtual void RD_RunningMessage		( vector<string>& MsgList ) = 0;
	static  void SL_RunningMessageRequest();

	//要求送理
	static  void SL_MailGift			( int ItemGUID , int Count , const char* Password , MailBackGroundTypeENUM BackGroundType 
										, const char* TargetName , const char* Title , const char* Content );
	virtual void RL_MailGiftResult		( CheckMailGiftResultTypeENUM Result ) = 0;

	//////////////////////////////////////////////////////////////////////////
	//通知所有人 老虎機資料有改變
	virtual void RL_SlotMachineInfoChange(  ) = 0;

	//要求老虎機資料
	static void SL_SlotMachineInfoRequest(  );
			//通知老虎機資料
			virtual void RL_SlotMachineInfo( int itemID[] , int itemCount[] ) = 0;
	//投幣
	static void SL_SlotMachinePlay(  );
		//結果
		virtual void RL_SlotMachinePlayResult( SlotMachinePlayResultENUM result , int itemID , int itemCount ) = 0;
			//要求取出獎品
			static void SL_GetSlotMachineItem();

	//要求取出網頁商城的物品
	static void SL_WebBagRequest(  );
	virtual void RL_WebBagResult( int OrgObjID , int Count ) = 0;
};
