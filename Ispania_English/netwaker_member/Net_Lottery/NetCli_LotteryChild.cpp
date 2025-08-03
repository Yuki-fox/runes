#include "NetCli_LotteryChild.h"
#include "../../mainproc/GameMain.h"
#include "../Interface/WorldFrames/ChatFrame.h"
#include "../../Interface/Debug/DebugMessage.h"
#include "../../Interface/WorldFrames/LotteryFrame.h"



//-----------------------------------------------------------------------------
bool NetCli_LotteryChild::_Init()
{
	if (This)
		return false;

	This = new NetCli_LotteryChild;
	return NetCli_Lottery::_Init();
}

//-----------------------------------------------------------------------------
bool NetCli_LotteryChild::_Release()
{
	NetCli_Lottery::_Release();
	if (This)
	{
		delete This;
		This = NULL;
	}
	return true;
}


void NetCli_LotteryChild::RC_OpenExchangePrize( int TargetNPCID )
{
	g_pLotteryFrame->RC_OpenExchangePrize( TargetNPCID );
	g_pDebugMessage->DebugOutput( 2, "RC_OpenExchangePrize TargetNPCID = %d" , TargetNPCID );
}

void NetCli_LotteryChild::RC_ExchangePrizeResult( bool Result , int Money , int PrizeCount[3] )
{
	g_pLotteryFrame->RC_ExchangePrizeResult( Result, Money, PrizeCount );
	if( Result == true )
		g_pDebugMessage->DebugOutput( 2, "RC_ExchangePrizeResultOK Money = %d PrizeCount = %d,%d,%d" , Money , PrizeCount[0] , PrizeCount[1] , PrizeCount[2] );
	else
		g_pDebugMessage->DebugOutput( 2, "RC_ExchangePrizeResultFailed" );
}

void NetCli_LotteryChild::RC_CloseExchangePrize( )
{
	g_pLotteryFrame->RC_CloseExchangePrize();
	g_pDebugMessage->DebugOutput( 2, "RC_CloseExchangePrize" );
}

void NetCli_LotteryChild::RC_OpenBuyLottery( int TargetNPCID )
{
	g_pLotteryFrame->RC_OpenBuyLottery( TargetNPCID );
	g_pDebugMessage->DebugOutput( 2, "RC_OpenBuyLottery TargetNPCID = %d" , TargetNPCID );
}

void NetCli_LotteryChild::RC_BuyLotteryResult( bool Result )
{
	g_pLotteryFrame->RC_BuyLotteryResult( Result );
	g_pDebugMessage->DebugOutput( 2, "RC_BuyLotteryResult Result = %d" , Result );
}

void NetCli_LotteryChild::RC_CloseBuyLottery( )
{
	g_pLotteryFrame->RC_CloseBuyLottery();
	g_pDebugMessage->DebugOutput( 2, "RC_CloseBuyLottery" );
}

void NetCli_LotteryChild::RD_LotteryInfo( int Version , int MaxPrizeMoney )
{
	g_pLotteryFrame->RD_LotteryInfo( Version, MaxPrizeMoney );
	g_pDebugMessage->DebugOutput( 2, "RD_LotteryInfo Version=%d MaxPrizeMoney=%d" , Version , MaxPrizeMoney );
}


void NetCli_LotteryChild::RD_LotteryHistoryInfo( LotteryInfoStruct LastInfo[5] )
{
	g_pLotteryFrame->RD_LotteryHistoryInfo( LastInfo );
	g_pDebugMessage->DebugOutput( 2, "RD_LotteryInfo " );
}

void NetCli_LotteryChild::RD_RunLottery( LotteryInfoStruct Info )
{
	g_pLotteryFrame->RD_RunLottery( Info );
	g_pDebugMessage->DebugOutput( 2, "RD_RunLottery Version=%d Num = %d,%d,%d,%d,%d" , Info.Version , Info.Num[0] , Info.Num[1] , Info.Num[2] , Info.Num[3] , Info.Num[4]  );
}