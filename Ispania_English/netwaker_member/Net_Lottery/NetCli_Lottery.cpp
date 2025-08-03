#include "NetCli_Lottery.h"

//-------------------------------------------------------------------
NetCli_Lottery*    NetCli_Lottery::This         = NULL;
//-------------------------------------------------------------------
bool NetCli_Lottery::_Init()
{
	/*
	_Net.RegOnSrvPacketFunction	( EM_PG_Lottery_LtoC_OpenExchangePrize	       , _PG_Lottery_LtoC_OpenExchangePrize	        	);
	_Net.RegOnSrvPacketFunction	( EM_PG_Lottery_LtoC_ExchangePrizeResult       , _PG_Lottery_LtoC_ExchangePrizeResult        	);
	_Net.RegOnSrvPacketFunction	( EM_PG_Lottery_LtoC_CloseExchangePrize	       , _PG_Lottery_LtoC_CloseExchangePrize	       	);
	_Net.RegOnSrvPacketFunction	( EM_PG_Lottery_LtoC_OpenBuyLottery		       , _PG_Lottery_LtoC_OpenBuyLottery		       	);
	_Net.RegOnSrvPacketFunction	( EM_PG_Lottery_LtoC_BuyLotteryResult	       , _PG_Lottery_LtoC_BuyLotteryResult	        	);
	_Net.RegOnSrvPacketFunction	( EM_PG_Lottery_LtoC_CloseBuyLottery	       , _PG_Lottery_LtoC_CloseBuyLottery	        	);
	_Net.RegOnSrvPacketFunction	( EM_PG_Lottery_DtoC_LotteryInfo		       , _PG_Lottery_DtoC_LotteryInfo		        	);
	_Net.RegOnSrvPacketFunction	( EM_PG_Lottery_DtoC_LotteryHistoryInfo	       , _PG_Lottery_DtoC_LotteryHistoryInfo		   	);
	_Net.RegOnSrvPacketFunction	( EM_PG_Lottery_DtoC_RunLottery			       , _PG_Lottery_DtoC_RunLottery			       	);
	*/

	Cli_NetReg( PG_Lottery_LtoC_OpenExchangePrize	   );
	Cli_NetReg( PG_Lottery_LtoC_ExchangePrizeResult    );
	Cli_NetReg( PG_Lottery_LtoC_CloseExchangePrize	   );
	Cli_NetReg( PG_Lottery_LtoC_OpenBuyLottery		   );
	Cli_NetReg( PG_Lottery_LtoC_BuyLotteryResult	   );
	Cli_NetReg( PG_Lottery_LtoC_CloseBuyLottery		   );
	Cli_NetReg( PG_Lottery_DtoC_LotteryInfo			   );
	Cli_NetReg( PG_Lottery_DtoC_LotteryHistoryInfo	   );
	Cli_NetReg( PG_Lottery_DtoC_RunLottery			   );

    return true;
}
//-------------------------------------------------------------------
bool NetCli_Lottery::_Release()
{
    return true;
}
//-------------------------------------------------------------------
void NetCli_Lottery::_PG_Lottery_LtoC_OpenExchangePrize		( int Sockid , int Size , void* Data )
{
	PG_Lottery_LtoC_OpenExchangePrize* PG = (PG_Lottery_LtoC_OpenExchangePrize*)Data;
	This->RC_OpenExchangePrize( PG->TargetNPCID );

}
void NetCli_Lottery::_PG_Lottery_LtoC_ExchangePrizeResult	( int Sockid , int Size , void* Data )
{
	PG_Lottery_LtoC_ExchangePrizeResult* PG = (PG_Lottery_LtoC_ExchangePrizeResult*)Data;
	This->RC_ExchangePrizeResult( PG->Result ,  PG->Money , PG->PrizeCount );
}
void NetCli_Lottery::_PG_Lottery_LtoC_CloseExchangePrize	( int Sockid , int Size , void* Data )
{
	PG_Lottery_LtoC_CloseExchangePrize* PG = (PG_Lottery_LtoC_CloseExchangePrize*)Data;
	This->RC_CloseExchangePrize( );
}
void NetCli_Lottery::_PG_Lottery_LtoC_OpenBuyLottery		( int Sockid , int Size , void* Data )
{
	PG_Lottery_LtoC_OpenBuyLottery* PG = (PG_Lottery_LtoC_OpenBuyLottery*)Data;
	This->RC_OpenBuyLottery( PG->TargetNPCID );
}
void NetCli_Lottery::_PG_Lottery_LtoC_BuyLotteryResult		( int Sockid , int Size , void* Data )
{
	PG_Lottery_LtoC_BuyLotteryResult* PG = (PG_Lottery_LtoC_BuyLotteryResult*)Data;
	This->RC_BuyLotteryResult( PG->Result );
}
void NetCli_Lottery::_PG_Lottery_LtoC_CloseBuyLottery		( int Sockid , int Size , void* Data )
{
	PG_Lottery_LtoC_CloseBuyLottery* PG = (PG_Lottery_LtoC_CloseBuyLottery*)Data;
	This->RC_CloseBuyLottery( );
}

void NetCli_Lottery::_PG_Lottery_DtoC_LotteryInfo			( int Sockid , int Size , void* Data )
{
	PG_Lottery_DtoC_LotteryInfo* PG = (PG_Lottery_DtoC_LotteryInfo*)Data;
	This->RD_LotteryInfo( PG->Version , PG->MaxPrizeMoney );
}
void NetCli_Lottery::_PG_Lottery_DtoC_LotteryHistoryInfo		( int Sockid , int Size , void* Data )
{
	PG_Lottery_DtoC_LotteryHistoryInfo* PG = (PG_Lottery_DtoC_LotteryHistoryInfo*)Data;
	This->RD_LotteryHistoryInfo( PG->LastInfo );
}
void NetCli_Lottery::_PG_Lottery_DtoC_RunLottery			( int Sockid , int Size , void* Data )
{
	PG_Lottery_DtoC_RunLottery* PG = (PG_Lottery_DtoC_RunLottery*)Data;
	This->RD_RunLottery( PG->Info );
}
//////////////////////////////////////////////////////////////////////////
void NetCli_Lottery::SL_ExchangePrize			( ItemFieldStruct&	Item , int Pos )
{
	PG_Lottery_CtoL_ExchangePrize Send;
	Send.Item = Item;
	Send.Pos = Pos;
	SendToLocal_Base( sizeof( Send ) , &Send );
}
void NetCli_Lottery::SL_CloseExchangePrize		( )
{
	PG_Lottery_CtoL_CloseExchangePrize Send;
	SendToLocal_Base( sizeof( Send ) , &Send );
}
void NetCli_Lottery::SL_BuyLottery				( int Money , int Count , int Number[20]  )
{
	PG_Lottery_CtoL_BuyLottery Send;
	Send.Money = Money;
	Send.Count = Count;
	memcpy( Send.Number , Number , sizeof(Send.Number) );

	SendToLocal_Base( sizeof( Send ) , &Send );

}
void NetCli_Lottery::SL_CloseBuyLottery			( )
{
	PG_Lottery_CtoL_CloseBuyLottery	Send;
	SendToLocal_Base( sizeof( Send ) , &Send );

}
void NetCli_Lottery::SL_LotteryInfoRequest		( )
{
	PG_Lottery_CtoL_LotteryInfoRequest	Send;
	SendToLocal_Base( sizeof( Send ) , &Send );
}

void NetCli_Lottery::SL_LotteryHistoryInfoRequest( )
{
	PG_Lottery_CtoL_LotteryHistoryInfoRequest	Send;
	SendToLocal_Base( sizeof( Send ) , &Send );
}
