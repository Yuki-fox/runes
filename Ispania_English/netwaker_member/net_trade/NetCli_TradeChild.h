#pragma once

#include "NetCli_Trade.h"

#define PLAYER_TRADE_MAX_ITEM				8

class NetCli_TradeChild : public NetCli_Trade
{
public:	
	static	bool _Init();
	static	bool _Release();

public:
	static	void				ClearItem();
	static	ItemFieldStruct*	GetTradeItem(int index);

	virtual void R_RequestTrade          ( int TargetID );
	virtual void R_OpenTrade             ( char* Name , int TargetID );
	virtual void R_TargetPutItem         ( int Pos , ItemFieldStruct& Item );
	virtual void R_TargetPutMoney        ( int Money , int Money_Account );
	virtual void R_OwnerPutMoney         ( int Money , int Money_Account );
//	virtual void R_TargetItemOK          ( );
//	virtual void R_CancelItemOK          ( );
//	virtual void R_FinalOKEnable         ( );
//	virtual void R_TargetFinalOK         ( );
	virtual void R_ClearTradeItem        ( );
	virtual void R_TradeItem_OK          ( );
	virtual void R_TradeItem_Failed      ( );
	virtual void R_PutItem_OK            ( );
	virtual void R_PutItem_Failed        ( );
	virtual void R_RequestTradeError     ( );
	virtual void R_DisagreeTrade         ( int ClientResult );	
	virtual void R_StopTrade			 ( int TargetID );
	virtual void R_TradeState			 ( bool TradeOK , bool FinalOK , bool TargetTradeOK , bool TargetFinalOK );
	virtual void R_CancelTardeRequest	 ( int TargetID );
	virtual void R_FinalOKFailed		 ( FinalOKFailedENUM Result );
protected:
	static	ItemFieldStruct		s_tradeItem[PLAYER_TRADE_MAX_ITEM];
};

