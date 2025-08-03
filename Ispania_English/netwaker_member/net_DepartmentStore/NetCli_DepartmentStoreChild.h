#pragma once

#include "NetCli_DepartmentStore.h"

class NetCli_DepartmentStoreChild : public NetCli_DepartmentStore
{
public:
	static bool _Init();
	static bool _Release();

	virtual void RL_ShopFunctionResult		( int FunctionID , bool Result  );
	virtual void RD_SellType				( int Count , int SellType[50] );
	virtual void RD_ShopInfo				( int ItemCount , DepartmentStoreBaseInfoStruct Item[500] );
	virtual void RL_BuyItemResult			( DepartmentStoreBuyItemResultENUM Result );
	virtual void RL_ImportAccountMoneyResult( int Money_Account , int Money_Bonus );
	virtual void RL_ExchangeItemResult		( ItemFieldStruct& Item , int Money , int MoneyAccount , ItemExchangeResultENUM Result );
	virtual void RD_RunningMessage			( vector<string>& MsgList );
	virtual void RL_MailGiftResult			( CheckMailGiftResultTypeENUM Result );

	virtual void RL_SlotMachineInfoChange	(  );
	virtual void RL_SlotMachineInfo			( int itemID[] , int itemCount[] );
	virtual void RL_SlotMachinePlayResult	( SlotMachinePlayResultENUM result , int itemID , int itemCount );
	virtual void RL_WebBagResult			( int OrgObjID , int Count );

public:
	static bool g_OnTimeWebBagRequest;

protected:
	static int OnTimeWebBagRequest(SchedularInfo* obj, void* inputclass);
};
