 #include "NetCli_AC.h"

//-------------------------------------------------------------------
NetCli_AC*    NetCli_AC::This         = NULL;

//-------------------------------------------------------------------
bool NetCli_AC::_Init()
{
	/*
	_Net.RegOnSrvPacketFunction	( EM_PG_AC_DCtoC_SearchItemCountResult		, _PG_AC_DCtoC_SearchItemCountResult	);
	_Net.RegOnSrvPacketFunction	( EM_PG_AC_DCtoC_SearchItemResult			, _PG_AC_DCtoC_SearchItemResult			);
	_Net.RegOnSrvPacketFunction	( EM_PG_AC_DCtoC_FixSearchItemResult		, _PG_AC_DCtoC_FixSearchItemResult		);
	_Net.RegOnSrvPacketFunction	( EM_PG_AC_DCtoC_SelfCompetitive_BuyItem	, _PG_AC_DCtoC_SelfCompetitive_BuyItem	);
	_Net.RegOnSrvPacketFunction	( EM_PG_AC_DCtoC_SelfCompetitive_SellItem	, _PG_AC_DCtoC_SelfCompetitive_SellItem	);
	_Net.RegOnSrvPacketFunction	( EM_PG_AC_DCtoC_SelfCompetitive_ItemDataEnd, _PG_AC_DCtoC_SelfCompetitive_ItemDataEnd	);
	_Net.RegOnSrvPacketFunction	( EM_PG_AC_DCtoC_CancelSellItemResult		, _PG_AC_DCtoC_CancelSellItemResult		);
	_Net.RegOnSrvPacketFunction	( EM_PG_AC_DCtoC_CloseAC					, _PG_AC_DCtoC_CloseAC					);
	_Net.RegOnSrvPacketFunction	( EM_PG_AC_DCtoC_OpenAC						, _PG_AC_DCtoC_OpenAC					);
	_Net.RegOnSrvPacketFunction	( EM_PG_AC_DCtoC_LostCompetitiveItem		, _PG_AC_DCtoC_LostCompetitiveItem		);
	_Net.RegOnSrvPacketFunction	( EM_PG_AC_DCtoC_FixBuyOrderItem			, _PG_AC_DCtoC_FixBuyOrderItem			);
	_Net.RegOnSrvPacketFunction	( EM_PG_AC_DCtoC_FixSellOrderItem			, _PG_AC_DCtoC_FixSellOrderItem			);
	_Net.RegOnSrvPacketFunction	( EM_PG_AC_DCtoC_ItemHistoryListResult		, _PG_AC_DCtoC_ItemHistoryListResult	);
	_Net.RegOnSrvPacketFunction	( EM_PG_AC_LtoC_BuyItemResult				, _PG_AC_LtoC_BuyItemResult				);
	_Net.RegOnSrvPacketFunction	( EM_PG_AC_LtoC_SellItemResult				, _PG_AC_LtoC_SellItemResult			);
	_Net.RegOnSrvPacketFunction	( EM_PG_AC_LtoC_AcAlreadyOpen				, _PG_AC_LtoC_AcAlreadyOpen				);
	_Net.RegOnSrvPacketFunction	( EM_PG_AC_LtoC_SellMoneyResult				, _PG_AC_LtoC_SellMoneyResult			);
*/
	Cli_NetReg( PG_AC_DCtoC_SearchItemCountResult		);
	Cli_NetReg( PG_AC_DCtoC_SearchItemResult			);
	Cli_NetReg( PG_AC_DCtoC_FixSearchItemResult			);
	Cli_NetReg( PG_AC_DCtoC_SelfCompetitive_BuyItem		);
	Cli_NetReg( PG_AC_DCtoC_SelfCompetitive_SellItem	);
	Cli_NetReg( PG_AC_DCtoC_SelfCompetitive_ItemDataEnd	);
	Cli_NetReg( PG_AC_DCtoC_CancelSellItemResult		);
	Cli_NetReg( PG_AC_DCtoC_CloseAC						);
	Cli_NetReg( PG_AC_DCtoC_OpenAC						);
	Cli_NetReg( PG_AC_DCtoC_LostCompetitiveItem			);
	Cli_NetReg( PG_AC_DCtoC_FixBuyOrderItem				);
	Cli_NetReg( PG_AC_DCtoC_FixSellOrderItem			);
	Cli_NetReg( PG_AC_DCtoC_ItemHistoryListResult		);
	Cli_NetReg( PG_AC_LtoC_BuyItemResult				);
	Cli_NetReg( PG_AC_LtoC_SellItemResult				);
	Cli_NetReg( PG_AC_LtoC_AcAlreadyOpen				);
	Cli_NetReg( PG_AC_LtoC_SellMoneyResult				);
	Cli_NetReg( PG_AC_DtoC_AcTradeOk					);

    return true;
}
//-------------------------------------------------------------------
bool NetCli_AC::_Release()
{
    return true;
}
//-------------------------------------------------------------------
void NetCli_AC::_PG_AC_DtoC_AcTradeOk					( int Sockid , int Size , void* Data )
{
	PG_AC_DtoC_AcTradeOk* PG = (PG_AC_DtoC_AcTradeOk*) Data;
	This->RL_AcTradeOk( PG->BuyerName.Begin() , PG->SellerName.Begin() , PG->PriceType , PG->Money , PG->Tax , PG->Item );
}

void NetCli_AC::_PG_AC_LtoC_SellMoneyResult				( int Sockid , int Size , void* Data )
{
	PG_AC_LtoC_SellMoneyResult* PG = (PG_AC_LtoC_SellMoneyResult*) Data;
	This->RL_SellMoneyResult( PG->Result , PG->Item );

}
void NetCli_AC::_PG_AC_LtoC_AcAlreadyOpen				( int Sockid , int Size , void* Data )
{
	PG_AC_LtoC_AcAlreadyOpen* PG = (PG_AC_LtoC_AcAlreadyOpen*) Data;
	This->RL_AcAlreadyOpen( PG->TargtetNPC );
}
void NetCli_AC::_PG_AC_DCtoC_SelfCompetitive_ItemDataEnd					( int Sockid , int Size , void* Data )
{
	PG_AC_DCtoC_SelfCompetitive_ItemDataEnd* PG = (PG_AC_DCtoC_SelfCompetitive_ItemDataEnd*) Data;
	This->RD_SelfCompetitive_ItemDataEnd( );
}

void NetCli_AC::_PG_AC_DCtoC_ItemHistoryListResult		( int Sockid , int Size , void* Data )
{
	vector< DB_ACItemHistoryStruct >  ItemList;
	PG_AC_DCtoC_ItemHistoryListResult* PG = (PG_AC_DCtoC_ItemHistoryListResult*) Data;

	for( int i = 0 ; i < PG->Count ; i++ )
	{
		ItemList.push_back( PG->ItemHistory[i] );
	}
	This->RD_ItemHistoryListResult( ItemList );
}

void NetCli_AC::_PG_AC_DCtoC_CloseAC					( int Sockid , int Size , void* Data )
{
	PG_AC_DCtoC_CloseAC* PG = (PG_AC_DCtoC_CloseAC*) Data;
	This->RD_CloseAC( );
}
void NetCli_AC::_PG_AC_DCtoC_SearchItemCountResult		( int Sockid , int Size , void* Data )
{
	PG_AC_DCtoC_SearchItemCountResult* PG = (PG_AC_DCtoC_SearchItemCountResult*) Data;
	This->RD_SearchItemCountResult( PG->MaxCount , PG->Count , PG->BeginIndex );
}
void NetCli_AC::_PG_AC_DCtoC_SearchItemResult			( int Sockid , int Size , void* Data )	
{
	PG_AC_DCtoC_SearchItemResult* PG = (PG_AC_DCtoC_SearchItemResult*) Data;
	This->RD_SearchItemResult( PG->Index , PG->Data , (char*)PG->SellerName.Begin() , PG->BuyerDBID );
}
void NetCli_AC::_PG_AC_DCtoC_FixSearchItemResult		( int Sockid , int Size , void* Data )	
{
	PG_AC_DCtoC_FixSearchItemResult* PG = (PG_AC_DCtoC_FixSearchItemResult*) Data;
	This->RD_FixSearchItemResult( PG->ACItemDBID , PG->State , PG->NewPrices , PG->Type , PG->BuyerDBID );
}
void NetCli_AC::_PG_AC_DCtoC_SelfCompetitive_BuyItem	( int Sockid , int Size , void* Data )	
{
	PG_AC_DCtoC_SelfCompetitive_BuyItem* PG = (PG_AC_DCtoC_SelfCompetitive_BuyItem*) Data;
	This->RD_SelfCompetitive_BuyItem( PG->Data , (char*)PG->SellerName.Begin() , PG->BuyerDBID );
}
void NetCli_AC::_PG_AC_DCtoC_SelfCompetitive_SellItem	( int Sockid , int Size , void* Data )	
{
	PG_AC_DCtoC_SelfCompetitive_SellItem* PG = (PG_AC_DCtoC_SelfCompetitive_SellItem*) Data;
	This->RD_SelfCompetitive_SellItem( PG->Data , (char*)PG->BuyerName.Begin() );
}

void NetCli_AC::_PG_AC_DCtoC_CancelSellItemResult	( int Sockid , int Size , void* Data )	
{
	PG_AC_DCtoC_CancelSellItemResult* PG = (PG_AC_DCtoC_CancelSellItemResult*) Data;
	This->RD_CancelSellItemResult( PG->ACItemDBID );
}

void NetCli_AC::_PG_AC_DCtoC_OpenAC						( int Sockid , int Size , void* Data )
{
	PG_AC_DCtoC_OpenAC* PG = (PG_AC_DCtoC_OpenAC*) Data;
	This->RD_OpenAC( PG->NpcGUID , PG->Result );
}
void NetCli_AC::_PG_AC_LtoC_BuyItemResult				( int Sockid , int Size , void* Data )	
{
	PG_AC_LtoC_BuyItemResult* PG = (PG_AC_LtoC_BuyItemResult*) Data;
	This->RL_BuyItemResult( PG->Result , PG->ACItemDBID , PG->Prices , PG->Type , PG->State );
}
void NetCli_AC::_PG_AC_LtoC_SellItemResult				( int Sockid , int Size , void* Data )
{
	PG_AC_LtoC_SellItemResult* PG = (PG_AC_LtoC_SellItemResult*) Data;
	This->RL_SellItemResult( PG->Result , PG->Item );
}
void NetCli_AC::_PG_AC_DCtoC_LostCompetitiveItem		( int Sockid , int Size , void* Data )
{
	PG_AC_DCtoC_LostCompetitiveItem* PG = (PG_AC_DCtoC_LostCompetitiveItem*) Data;
	This->RD_LostCompetitiveItem( PG->ACItemDBID , PG->Item );
}
void NetCli_AC::_PG_AC_DCtoC_FixBuyOrderItem			( int Sockid , int Size , void* Data )
{
	PG_AC_DCtoC_FixBuyOrderItem* PG = (PG_AC_DCtoC_FixBuyOrderItem*) Data;
	This->RD_FixBuyOrderItem( PG->ACItemDBID , PG->State , PG->NewPrices , PG->Type , PG->BuyerDBID );
}
void NetCli_AC::_PG_AC_DCtoC_FixSellOrderItem			( int Sockid , int Size , void* Data )
{
	PG_AC_DCtoC_FixSellOrderItem* PG = (PG_AC_DCtoC_FixSellOrderItem*) Data;
	This->RD_FixSellOrderItem( PG->ACItemDBID , PG->State , PG->NewPrices , PG->Type , (char*)PG->BuyerName.Begin() );
}
void NetCli_AC::SL_SearchItem			( ACSearchConditionStruct& Data )
{
	PG_AC_CtoL_SearchItem Send;

	Send.Data = Data;
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_AC::SL_GetNextSearchItem	( )
{
	PG_AC_CtoL_GetNextSearchItem Send;

	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_AC::SL_GetMyACItemInfo		( )
{
	PG_AC_CtoL_GetMyACItemInfo Send;

	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_AC::SL_BuyItem				( int ACItemDBID , int Prices , PriceTypeENUM PricesType , char* Password )
{
	PG_AC_CtoL_BuyItem Send;
	Send.ACItemDBID = ACItemDBID;		//要買的物品
	Send.Prices = Prices;				//所出的價錢(如果 等於直接購買價 代表直接購買)
	Send.Type = PricesType;
	Send.Password = Password;

	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_AC::SL_SellItem				( int BodyPos , ItemFieldStruct& Item , int Prices , int BuyOutPrices , PriceTypeENUM PricesType , int LiveTime )
{
	PG_AC_CtoL_SellItem Send;
	Send.BodyPos = BodyPos;
	Send.Item = Item;					//賣的東西	
	Send.Prices = Prices;				//基本價
	Send.BuyOutPrices = BuyOutPrices;	//直接購買價
	Send.LiveTime = LiveTime;
	Send.Type = PricesType;

	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_AC::SL_CancelSellItem		( int ACItemDBID )
{
	PG_AC_CtoL_CancelSellItem Send;
	Send.ACItemDBID = ACItemDBID;

	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_AC::SL_CloseAC				( )
{
	PG_AC_CtoL_CloseAC Send;
	
	SendToLocal( sizeof(Send) , &Send );
}

void NetCli_AC::SL_ItemHistoryRequest	( int OrgObjID  )
{
	PG_AC_CtoL_ItemHistoryRequest Send;
	Send.OrgObjID = OrgObjID;
//	Send.Inherent = Inherent;

	SendToLocal( sizeof(Send) , &Send );
}

void NetCli_AC::SL_SellMoney			( PriceTypeENUM	 SellType , int	SellMoney ,	int Prices , int BuyOutPrices , int LiveTime , PriceTypeENUM Type , const char* Password )
{
	PG_AC_CtoL_SellMoney Send;
	Send.SellType = SellType;
	Send.SellMoney = SellMoney;

	Send.Prices = Prices;				//基本價
	Send.BuyOutPrices = BuyOutPrices;	//直接購買價
	Send.LiveTime = LiveTime;			//(分鐘計算)
	Send.Type = Type;
	Send.Password = Password;

	SendToLocal( sizeof(Send) , &Send );
}