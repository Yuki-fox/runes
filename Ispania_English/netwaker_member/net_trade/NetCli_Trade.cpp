#include "NetCli_Trade.h"

//-------------------------------------------------------------------
NetCli_Trade*    NetCli_Trade::This         = NULL;

//-------------------------------------------------------------------
bool NetCli_Trade::_Init()
{
	/*
    _Net.RegOnSrvPacketFunction	( EM_PG_Trade_LtoC_RequestTrade       	, _PG_Trade_LtoC_RequestTrade       );
    _Net.RegOnSrvPacketFunction	( EM_PG_Trade_LtoC_OpenTrade          	, _PG_Trade_LtoC_OpenTrade          );
    _Net.RegOnSrvPacketFunction	( EM_PG_Trade_LtoC_TargetPutItem      	, _PG_Trade_LtoC_TargetPutItem      );
    _Net.RegOnSrvPacketFunction	( EM_PG_Trade_LtoC_TargetPutMoney     	, _PG_Trade_LtoC_TargetPutMoney     );
    _Net.RegOnSrvPacketFunction	( EM_PG_Trade_LtoC_OwnerPutMoney      	, _PG_Trade_LtoC_OwnerPutMoney      );
    _Net.RegOnSrvPacketFunction	( EM_PG_Trade_LtoC_ClearTradeItem     	, _PG_Trade_LtoC_ClearTradeItem     );
    _Net.RegOnSrvPacketFunction	( EM_PG_Trade_LtoC_TradeItemResult    	, _PG_Trade_LtoC_TradeItemResult    );
    _Net.RegOnSrvPacketFunction	( EM_PG_Trade_LtoC_PutItemResult      	, _PG_Trade_LtoC_PutItemResult      );
	_Net.RegOnSrvPacketFunction	( EM_PG_Trade_LtoC_RequestTradeError  	, _PG_Trade_LtoC_RequestTradeError  );
	_Net.RegOnSrvPacketFunction	( EM_PG_Trade_LtoC_DisagreeTrade      	, _PG_Trade_LtoC_DisagreeTrade      );
	_Net.RegOnSrvPacketFunction	( EM_PG_Trade_LtoC_StopTrade	      	, _PG_Trade_LtoC_StopTrade			);
	_Net.RegOnSrvPacketFunction	( EM_PG_Trade_LtoC_TradeState	      	, _PG_Trade_LtoC_TradeState			);
	_Net.RegOnSrvPacketFunction	( EM_PG_Trade_LtoC_CancelTradeRequest 	, _PG_Trade_LtoC_CancelTardeRequest	);
	_Net.RegOnSrvPacketFunction	( EM_PG_Trade_LtoC_FinalOKFailed		, _PG_Trade_LtoC_FinalOKFailed		);
	*/

	Cli_NetReg( PG_Trade_LtoC_RequestTrade       	);
	Cli_NetReg( PG_Trade_LtoC_OpenTrade          	);
	Cli_NetReg( PG_Trade_LtoC_TargetPutItem      	);
	Cli_NetReg( PG_Trade_LtoC_TargetPutMoney     	);
	Cli_NetReg( PG_Trade_LtoC_OwnerPutMoney      	);
	Cli_NetReg( PG_Trade_LtoC_ClearTradeItem     	);
	Cli_NetReg( PG_Trade_LtoC_TradeItemResult    	);
	Cli_NetReg( PG_Trade_LtoC_PutItemResult      	);
	Cli_NetReg( PG_Trade_LtoC_RequestTradeError  	);
	Cli_NetReg( PG_Trade_LtoC_DisagreeTrade      	);
	Cli_NetReg( PG_Trade_LtoC_StopTrade	      		);
	Cli_NetReg( PG_Trade_LtoC_TradeState	      	);
	Cli_NetReg( PG_Trade_LtoC_CancelTradeRequest 	);
	Cli_NetReg( PG_Trade_LtoC_FinalOKFailed			);
								  
    return true;
}
//-------------------------------------------------------------------
bool NetCli_Trade::_Release()
{
    return true;
}
//-------------------------------------------------------------------
void NetCli_Trade::_PG_Trade_LtoC_FinalOKFailed		( int Sockid , int Size , void* Data )
{
	PG_Trade_LtoC_FinalOKFailed* PG = (PG_Trade_LtoC_FinalOKFailed*)Data;
	This->R_FinalOKFailed( PG->Result );
}
void NetCli_Trade::_PG_Trade_LtoC_CancelTradeRequest	( int Sockid , int Size , void* Data )
{
	PG_Trade_LtoC_CancelTradeRequest* PG = (PG_Trade_LtoC_CancelTradeRequest*)Data;
	This->R_CancelTardeRequest( PG->TargetID );
}
void NetCli_Trade::_PG_Trade_LtoC_TradeState	    ( int Sockid , int Size , void* Data )
{
	PG_Trade_LtoC_TradeState* PG = (PG_Trade_LtoC_TradeState*)Data;
	This->R_TradeState( PG->TradeOK , PG->FinalOK , PG->TargetTradeOK , PG->TargetFinalOK );
}
void NetCli_Trade::_PG_Trade_LtoC_StopTrade	    ( int Sockid , int Size , void* Data )
{
	PG_Trade_LtoC_StopTrade* PG = (PG_Trade_LtoC_StopTrade*)Data;
	This->R_StopTrade( PG->TargetID );
}

void NetCli_Trade::_PG_Trade_LtoC_PutItemResult       ( int Sockid , int Size , void* Data )
{
    PG_Trade_LtoC_PutItemResult* PG = (PG_Trade_LtoC_PutItemResult*)Data;
    if( PG->Result == true )
    {
        This->R_PutItem_OK();
    }
    else
    {
        This->R_PutItem_Failed();
    }
}

void NetCli_Trade::_PG_Trade_LtoC_RequestTrade     ( int Sockid , int Size , void* Data )
{
    PG_Trade_LtoC_RequestTrade* PG = (PG_Trade_LtoC_RequestTrade*)Data;
    This->R_RequestTrade( PG->TargetID );
}
void NetCli_Trade::_PG_Trade_LtoC_OpenTrade        ( int Sockid , int Size , void* Data )
{
    PG_Trade_LtoC_OpenTrade* PG = (PG_Trade_LtoC_OpenTrade*)Data;
    This->R_OpenTrade( PG->Name , PG->TargetID );
}
void NetCli_Trade::_PG_Trade_LtoC_TargetPutItem    ( int Sockid , int Size , void* Data )
{
    PG_Trade_LtoC_TargetPutItem* PG = (PG_Trade_LtoC_TargetPutItem*)Data;
    This->R_TargetPutItem( PG->Pos , PG->Item );
}
void NetCli_Trade::_PG_Trade_LtoC_TargetPutMoney   ( int Sockid , int Size , void* Data )
{
    PG_Trade_LtoC_TargetPutMoney* PG = (PG_Trade_LtoC_TargetPutMoney*)Data;
    This->R_TargetPutMoney( PG->Money , PG->Money_Account );
}
void NetCli_Trade::_PG_Trade_LtoC_OwnerPutMoney    ( int Sockid , int Size , void* Data )
{
    PG_Trade_LtoC_OwnerPutMoney* PG = (PG_Trade_LtoC_OwnerPutMoney*)Data;
    This->R_OwnerPutMoney( PG->Money , PG->Money_Account );
}
/*
void NetCli_Trade::_PG_Trade_LtoC_TargetItemOK     ( int Sockid , int Size , void* Data )
{
    PG_Trade_LtoC_TargetItemOK* PG = (PG_Trade_LtoC_TargetItemOK*)Data;
    This->R_TargetItemOK( );
}
void NetCli_Trade::_PG_Trade_LtoC_CancelItemOK     ( int Sockid , int Size , void* Data )
{
    PG_Trade_LtoC_CancelItemOK* PG = (PG_Trade_LtoC_CancelItemOK*)Data;
    This->R_CancelItemOK( );
}
void NetCli_Trade::_PG_Trade_LtoC_FinalOKEnable    ( int Sockid , int Size , void* Data )
{
    PG_Trade_LtoC_FinalOKEnable* PG = (PG_Trade_LtoC_FinalOKEnable*)Data;
    This->R_FinalOKEnable();
}
void NetCli_Trade::_PG_Trade_LtoC_TargetFinalOK    ( int Sockid , int Size , void* Data )
{
    PG_Trade_LtoC_TargetFinalOK* PG = (PG_Trade_LtoC_TargetFinalOK*)Data;
    This->R_TargetFinalOK( );
}
*/
void NetCli_Trade::_PG_Trade_LtoC_ClearTradeItem   ( int Sockid , int Size , void* Data )
{
    PG_Trade_LtoC_ClearTradeItem* PG = (PG_Trade_LtoC_ClearTradeItem*)Data;
    This->R_ClearTradeItem();
}
void NetCli_Trade::_PG_Trade_LtoC_TradeItemResult  ( int Sockid , int Size , void* Data )
{
    PG_Trade_LtoC_TradeItemResult* PG = (PG_Trade_LtoC_TradeItemResult*)Data;
    if( PG->Result )
        This->R_TradeItem_OK();
    else
        This->R_TradeItem_Failed();
}
void NetCli_Trade::_PG_Trade_LtoC_RequestTradeError( int Sockid , int Size , void* Data )
{
	PG_Trade_LtoC_RequestTradeError* PG = (PG_Trade_LtoC_RequestTradeError*)Data;
	This->R_RequestTradeError( );
}
void NetCli_Trade::_PG_Trade_LtoC_DisagreeTrade    ( int Sockid , int Size , void* Data )
{
	PG_Trade_LtoC_DisagreeTrade* PG = (PG_Trade_LtoC_DisagreeTrade*)Data;
	This->R_DisagreeTrade( PG->ClientResult );
}



void NetCli_Trade::S_RequestTrade   ( int TargetID )
{
    PG_Trade_CtoL_RequestTrade Send;
    Send.TargetID = TargetID;
    SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Trade::S_AgreeTrade     ( int TargetID ,bool Result , int ClientResult )
{
    PG_Trade_CtoL_AgreeTrade Send;
    Send.TargetID = TargetID;
	Send.Result = Result;
	Send.ClientResult = ClientResult;
    SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Trade::S_PutItem        ( int Pos , int ItemPos , ItemFieldStruct& Item )
{
    PG_Trade_CtoL_PutItem Send;
    Send.Pos = Pos;
    Send.ItemPos = ItemPos;
    Send.Item = Item;
    SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Trade::S_PutMoney       ( int Money , int Money_Account )
{
    PG_Trade_CtoL_PutMoney Send;
    Send.Money = Money;
	Send.Money_Account = Money_Account;
    SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Trade::S_ItemOK         ( )
{
    PG_Trade_CtoL_ItemOK Send;
    SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Trade::S_FinalOK        ( const char* Password )
{
    PG_Trade_CtoL_FinalOK Send;
	Send.Password = Password;
    SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Trade::S_StopTrade      ( int TargetID )
{
    PG_Trade_CtoL_StopTrade Send;
    Send.TargetID = TargetID;
    SendToLocal( sizeof(Send) , &Send );
}

void NetCli_Trade::S_CancelTardeRequest( int TargetID )
{
	PG_Trade_CtoL_CancelTradeRequest Send;
	Send.TargetID = TargetID;
	SendToLocal( sizeof(Send) , &Send );
}