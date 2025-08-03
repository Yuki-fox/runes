#pragma once

#include "../interface.h"
#include "../../netwaker_member/net_lottery/NetCli_LotteryChild.h"


class CLotteryFrame;
extern CLotteryFrame* g_pLotteryFrame;


class CLotteryFrame : public CInterfaceBase
{
public:
	CLotteryFrame(CInterface* object);
	virtual ~CLotteryFrame();

	virtual void RegisterFunc();

	virtual void Update( float elapsedTime );

	virtual void RC_OpenExchangePrize	( int TargetNPCID );
	virtual void RC_ExchangePrizeResult	( bool Result , int Money , int PrizeCount[3] );
	virtual void RC_CloseExchangePrize	( );
	
	virtual void RC_OpenBuyLottery		( int TargetNPCID );
	virtual void RC_BuyLotteryResult	( bool Result );
	virtual void RC_CloseBuyLottery		( );

	virtual void RD_LotteryInfo			( int Version , int MaxPrizeMoney );
	virtual void RD_LotteryHistoryInfo	( LotteryInfoStruct LastInfo[5]  );
	virtual void RD_RunLottery			( LotteryInfoStruct	Info );
public:

	void BuyLottery();

public:

	virtual void ExchangeLottery();
	virtual void ConfirmExchange();
	virtual void CancelExchange();
	virtual void ExchangeSlotChanged();

	ItemFieldStruct m_itemPickupExchange;
	int m_bagpos;
public:
	//0	close
	//1	update
	//2	open

	enum ELotteryFrameMode
	{
		LFM_CLOSE  = 0,
		LFM_UPDATE = 1,
 		LFM_OPEN   = 2,
	};

	ELotteryFrameMode m_BuyLottery_Mode;
	int m_BuyLottery_NPCID;
 	int m_BuyLottery_Version;
	int m_BuyLottery_MaxPrizeMoney;
 
public:
	ELotteryFrameMode m_ExchangePrize_Mode;
	int m_ExchangePrize_NPCID;
	LotteryInfoStruct m_ExchangePrize_LastInfo[5];

public:
	int m_Number[20];

};


