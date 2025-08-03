#pragma once


#include "NetCli_Lottery.h"


class NetCli_LotteryChild : public NetCli_Lottery
{
public:	
	static bool _Init();
	static bool _Release();
public:

	virtual void RC_OpenExchangePrize	( int TargetNPCID );
	virtual void RC_ExchangePrizeResult	( bool Result , int Money , int PrizeCount[3] );
	virtual void RC_CloseExchangePrize	( );
	virtual void RC_OpenBuyLottery		( int TargetNPCID );
	virtual void RC_BuyLotteryResult	( bool Result );
	virtual void RC_CloseBuyLottery		( );

	virtual void RD_LotteryInfo			( int Version , int MaxPrizeMoney );
	virtual void RD_LotteryHistoryInfo	( LotteryInfoStruct LastInfo[5]  );
	virtual void RD_RunLottery			( LotteryInfoStruct	Info );
};

