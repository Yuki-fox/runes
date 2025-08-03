#pragma once

#include "PG/PG_Lottery.h"
#include "../../MainProc/NetGlobal.h"

class NetCli_Lottery : public CNetGlobal
{
protected:
    //-------------------------------------------------------------------
    static NetCli_Lottery* This;
    static bool _Init();
    static bool _Release();

	static void _PG_Lottery_LtoC_OpenExchangePrize		( int Sockid , int Size , void* Data );
	static void _PG_Lottery_LtoC_ExchangePrizeResult	( int Sockid , int Size , void* Data );	
	static void _PG_Lottery_LtoC_CloseExchangePrize		( int Sockid , int Size , void* Data );
	static void _PG_Lottery_LtoC_OpenBuyLottery			( int Sockid , int Size , void* Data );
	static void _PG_Lottery_LtoC_BuyLotteryResult		( int Sockid , int Size , void* Data );	
	static void _PG_Lottery_LtoC_CloseBuyLottery		( int Sockid , int Size , void* Data );

	static void _PG_Lottery_DtoC_LotteryInfo			( int Sockid , int Size , void* Data );	
	static void _PG_Lottery_DtoC_LotteryHistoryInfo		( int Sockid , int Size , void* Data );	
	static void _PG_Lottery_DtoC_RunLottery				( int Sockid , int Size , void* Data );

public:    
    NetCli_Lottery() { This = this; }


	static void SL_ExchangePrize			( ItemFieldStruct&	Item , int Pos );
	static void SL_CloseExchangePrize		( );
	static void SL_BuyLottery				( int Money , int Count , int Number[20] );
	static void SL_CloseBuyLottery			( );
	static void SL_LotteryInfoRequest		( );
	static void SL_LotteryHistoryInfoRequest( );


	virtual void RC_OpenExchangePrize	( int TargetNPCID ) = 0;
	virtual void RC_ExchangePrizeResult	( bool Result , int Money , int PrizeCount[3] ) = 0;
	virtual void RC_CloseExchangePrize	( ) = 0;
	virtual void RC_OpenBuyLottery		( int TargetNPCID ) = 0;
	virtual void RC_BuyLotteryResult	( bool Result ) = 0;
	virtual void RC_CloseBuyLottery		( ) = 0;

	virtual void RD_LotteryInfo			( int Version , int MaxPrizeMoney ) = 0;
	virtual void RD_LotteryHistoryInfo	( LotteryInfoStruct LastInfo[5]  ) = 0;
	virtual void RD_RunLottery			( LotteryInfoStruct	Info ) = 0;
};

