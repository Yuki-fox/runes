#include "NetCli_DepartmentStore.h"
#include "../../MainProc/NetGlobal.h"
//-------------------------------------------------------------------
NetCli_DepartmentStore*    NetCli_DepartmentStore::This         = NULL;

//-------------------------------------------------------------------
bool NetCli_DepartmentStore::_Init()
{
	Cli_NetReg( PG_DepartmentStore_LtoC_ShopFunctionResult  		);
	Cli_NetReg( PG_DepartmentStore_DtoC_SellType		    		);
	Cli_NetReg( PG_DepartmentStore_DtoC_ShopInfo		    		);
	Cli_NetReg( PG_DepartmentStore_DtoC_ShopInfo_Zip	    		);
	Cli_NetReg( PG_DepartmentStore_LtoC_BuyItemResult	    		);
	Cli_NetReg( PG_DepartmentStore_LtoC_ImportAccountMoneyResult	);
	Cli_NetReg( PG_DepartmentStore_LtoC_ExchangeItemResult			);
	Cli_NetReg( PG_DepartmentStore_DtoC_RunningMessage_Zip			);
	Cli_NetReg( PG_DepartmentStore_LtoC_MailGiftResult				);
	Cli_NetReg( PG_DepartmentStore_LtoC_SlotMachineInfoChange		);
	Cli_NetReg( PG_DepartmentStore_LtoC_SlotMachineInfo				);
	Cli_NetReg( PG_DepartmentStore_LtoC_SlotMachinePlayResult		);
	Cli_NetReg( PG_DepartmentStore_LtoC_WebBagResult				);
/*
	_Net.RegOnSrvPacketFunction	( EM_PG_DepartmentStore_LtoC_ShopFunctionResult  	 , _PG_DepartmentStore_LtoC_ShopFunctionResult  	 );
	_Net.RegOnSrvPacketFunction	( EM_PG_DepartmentStore_DtoC_SellType		    	 , _PG_DepartmentStore_DtoC_SellType		    	 );
	_Net.RegOnSrvPacketFunction	( EM_PG_DepartmentStore_DtoC_ShopInfo		    	 , _PG_DepartmentStore_DtoC_ShopInfo		    	 );
	_Net.RegOnSrvPacketFunction	( EM_PG_DepartmentStore_DtoC_ShopInfo_Zip	    	 , _PG_DepartmentStore_DtoC_ShopInfo_Zip	    	 );
	_Net.RegOnSrvPacketFunction	( EM_PG_DepartmentStore_LtoC_BuyItemResult	    	 , _PG_DepartmentStore_LtoC_BuyItemResult	    	 );
	_Net.RegOnSrvPacketFunction	( EM_PG_DepartmentStore_LtoC_ImportAccountMoneyResult, _PG_DepartmentStore_LtoC_ImportAccountMoneyResult );
	_Net.RegOnSrvPacketFunction	( EM_PG_DepartmentStore_LtoC_ExchangeItemResult		 , _PG_DepartmentStore_LtoC_ExchangeItemResult		 );
	_Net.RegOnSrvPacketFunction	( EM_PG_DepartmentStore_DtoC_RunningMessage_Zip		 , _PG_DepartmentStore_DtoC_RunningMessage_Zip		 );
	_Net.RegOnSrvPacketFunction	( EM_PG_DepartmentStore_LtoC_MailGiftResult			 , _PG_DepartmentStore_LtoC_MailGiftResult			 );
	*/
    return true;
}
//-------------------------------------------------------------------
bool NetCli_DepartmentStore::_Release()
{
    return true;
}
//-------------------------------------------------------------------
void NetCli_DepartmentStore::_PG_DepartmentStore_LtoC_SlotMachineInfoChange(  int Sockid , int size , void* data )
{
	PG_DepartmentStore_LtoC_SlotMachineInfoChange* pg = (PG_DepartmentStore_LtoC_SlotMachineInfoChange*)data;
	This->RL_SlotMachineInfoChange(  );
}
void NetCli_DepartmentStore::_PG_DepartmentStore_LtoC_SlotMachineInfo(  int Sockid , int size , void* data )
{
	PG_DepartmentStore_LtoC_SlotMachineInfo* pg = (PG_DepartmentStore_LtoC_SlotMachineInfo*)data;
	This->RL_SlotMachineInfo( pg->ItemID , pg->ItemCount );
}
void NetCli_DepartmentStore::_PG_DepartmentStore_LtoC_SlotMachinePlayResult(  int Sockid , int size , void* data )
{
	PG_DepartmentStore_LtoC_SlotMachinePlayResult* pg = (PG_DepartmentStore_LtoC_SlotMachinePlayResult*)data;
	This->RL_SlotMachinePlayResult( pg->Result , pg->ItemID , pg->ItemCount );
}
void NetCli_DepartmentStore::_PG_DepartmentStore_LtoC_WebBagResult( int Sockid , int Size , void* Data )
{
	PG_DepartmentStore_LtoC_WebBagResult* pg = (PG_DepartmentStore_LtoC_WebBagResult*)Data;
	This->RL_WebBagResult( pg->OrgObjID , pg->Count );
}
void NetCli_DepartmentStore::_PG_DepartmentStore_DtoC_ShopInfo_Zip	( int Sockid , int Size , void* Data )
{
	PG_DepartmentStore_DtoC_ShopInfo_Zip* PG = (PG_DepartmentStore_DtoC_ShopInfo_Zip*)Data;
	DepartmentStoreBaseInfoStruct SellItem[1000];

	MyLzoClass myZip;
	int RetSize = myZip.Decode( PG->Data , PG->DataSize , sizeof(SellItem) , (char*)SellItem );

	This->RD_ShopInfo( PG->ItemCount , SellItem );

	//This->RL_MailGiftResult( PG->Result );
}
void NetCli_DepartmentStore::_PG_DepartmentStore_LtoC_MailGiftResult( int Sockid , int Size , void* Data )
{
	PG_DepartmentStore_LtoC_MailGiftResult* PG = (PG_DepartmentStore_LtoC_MailGiftResult*)Data;
	This->RL_MailGiftResult( PG->Result );
}



void NetCli_DepartmentStore::_PG_DepartmentStore_DtoC_RunningMessage_Zip( int Sockid , int Size , void* Data )
{
	PG_DepartmentStore_DtoC_RunningMessage_Zip* PG = (PG_DepartmentStore_DtoC_RunningMessage_Zip*)Data;

	vector<string> MsgList;
	char	DataBuf[0x10000];

	BaseRoleData	BaseData;
	MyLzoClass myZip;
	int RetSize = myZip.Decode( PG->Data , PG->Size , sizeof(DataBuf) , DataBuf );

	if( RetSize != 0 )
	{
		char* pStr = DataBuf;
		for( int i = 0 ; i < PG->MsgCount ; i++ )
		{
			MsgList.push_back( pStr );
			pStr += ( MsgList.back().length() + 1 );
		}
	}

	This->RD_RunningMessage( MsgList );

}
void NetCli_DepartmentStore::_PG_DepartmentStore_LtoC_ExchangeItemResult( int Sockid , int Size , void* Data )
{
	PG_DepartmentStore_LtoC_ExchangeItemResult* PG = (PG_DepartmentStore_LtoC_ExchangeItemResult*)Data;
	This->RL_ExchangeItemResult( PG->Item ,PG->Money , PG->MoneyAccount , PG->Result );
}
void NetCli_DepartmentStore::_PG_DepartmentStore_LtoC_ImportAccountMoneyResult	( int Sockid , int Size , void* Data )
{
	PG_DepartmentStore_LtoC_ImportAccountMoneyResult* PG = (PG_DepartmentStore_LtoC_ImportAccountMoneyResult*)Data;
	This->RL_ImportAccountMoneyResult( PG->Money_Account , PG->Money_Bonus );
}
void NetCli_DepartmentStore::_PG_DepartmentStore_LtoC_ShopFunctionResult	( int Sockid , int Size , void* Data )
{
	PG_DepartmentStore_LtoC_ShopFunctionResult* PG = (PG_DepartmentStore_LtoC_ShopFunctionResult*)Data;
	This->RL_ShopFunctionResult( PG->FunctionID , PG->Result );
}
void NetCli_DepartmentStore::_PG_DepartmentStore_DtoC_SellType				( int Sockid , int Size , void* Data )	
{
	PG_DepartmentStore_DtoC_SellType* PG = (PG_DepartmentStore_DtoC_SellType*)Data;
	This->RD_SellType( PG->ItemCount , PG->SellType );
}
void NetCli_DepartmentStore::_PG_DepartmentStore_DtoC_ShopInfo				( int Sockid , int Size , void* Data )	
{
	PG_DepartmentStore_DtoC_ShopInfo* PG = (PG_DepartmentStore_DtoC_ShopInfo*)Data;
	This->RD_ShopInfo( PG->ItemCount , PG->Item );
}
void NetCli_DepartmentStore::_PG_DepartmentStore_LtoC_BuyItemResult			( int Sockid , int Size , void* Data )
{
	PG_DepartmentStore_LtoC_BuyItemResult* PG = (PG_DepartmentStore_LtoC_BuyItemResult*)Data;
	This->RL_BuyItemResult( PG->Result );
}

void NetCli_DepartmentStore::SL_ShopFunctionRequest	( int FunctionID )
{
	PG_DepartmentStore_CtoL_ShopFunctionRequest Send;
	Send.FunctionID = FunctionID;
	SendToLocal( sizeof( Send ), &Send );
}
void NetCli_DepartmentStore::SL_SellTypeRequest		( )
{
	PG_DepartmentStore_CtoL_SellTypeRequest Send;
	SendToLocal( sizeof( Send ), &Send );
}
void NetCli_DepartmentStore::SL_ShopInfoRequest		( int SellType )
{
	PG_DepartmentStore_CtoL_ShopInfoRequest Send;
	Send.SellType = SellType;
	SendToLocal( sizeof( Send ), &Send );
}
void NetCli_DepartmentStore::SL_BuyItem				( int ItemGUID , int Pos , int Count , char* Password )
{
	PG_DepartmentStore_CtoL_BuyItem	 Send;
	Send.ItemGUID = ItemGUID;
	Send.Pos = Pos;
	Send.Count = Count;
	Send.Password = Password;
	SendToLocal( sizeof( Send ), &Send );
}

void NetCli_DepartmentStore::SL_ImportAccountMoney	( )
{
	PG_DepartmentStore_CtoL_ImportAccountMoney	 Send;	
	SendToLocal( sizeof( Send ), &Send );
}

void NetCli_DepartmentStore::SL_ExchangeItemRequest	( const char* ItemSerial , const char* Password )
{
	PG_DepartmentStore_CtoL_ExchangeItemRequest Send;
	strncpy( Send.ItemSerial , ItemSerial , sizeof( Send.ItemSerial ) );
	strncpy( Send.Password , Password , sizeof( Send.Password ) );
	SendToLocal( sizeof( Send ), &Send );
}

void NetCli_DepartmentStore::SL_RunningMessageRequest()
{
	PG_DepartmentStore_CtoL_RunningMessageRequest Send;
	SendToLocal( sizeof( Send ), &Send );
}

void NetCli_DepartmentStore::SL_MailGift			( int ItemGUID , int Count , const char* Password , MailBackGroundTypeENUM BackGroundType 
													, const char* TargetName , const char* Title , const char* Content )
{
	PG_DepartmentStore_CtoL_MailGift Send;
	
	Send.ItemGUID	= ItemGUID;		//物品索引號
	Send.Count		= Count;		//購買數量
	Send.Password	= Password;	

	//信件資訊
	Send.BackGroundType	= BackGroundType;

	Send.TargetName = TargetName;
	Send.Title		= Title;	
	Send.Content	= Content;

	SendToLocal( sizeof( Send ), &Send );
}

//Client -> Local 要求老虎機資料
void NetCli_DepartmentStore::SL_SlotMachineInfoRequest(  )
{
	PG_DepartmentStore_CtoL_SlotMachineInfoRequest Send;
	SendToLocal( sizeof( Send ), &Send );
}
//Client -> Local 投幣
void NetCli_DepartmentStore::SL_SlotMachinePlay(  )
{
	PG_DepartmentStore_CtoL_SlotMachinePlay Send;
	SendToLocal( sizeof( Send ), &Send );
}
//要求取出獎品
void NetCli_DepartmentStore::SL_GetSlotMachineItem()
{
	PG_DepartmentStore_CtoL_GetSlotMachineItem Send;
	SendToLocal( sizeof( Send ), &Send );
}
//要求取出網頁商城的物品
void NetCli_DepartmentStore::SL_WebBagRequest(  )
{
	PG_DepartmentStore_CtoL_WebBagRequest Send;
	SendToLocal( sizeof( Send ), &Send );
}