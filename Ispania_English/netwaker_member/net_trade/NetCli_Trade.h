#pragma once

#include "PG/PG_Trade.h"
#include "../../MainProc/NetGlobal.h"

class NetCli_Trade : public CNetGlobal
{
protected:
    //-------------------------------------------------------------------
    static	NetCli_Trade* This;
    static	bool _Init();
    static	bool _Release();	
    //-------------------------------------------------------------------
    static void _PG_Trade_LtoC_RequestTrade     	( int Sockid , int Size , void* Data );
    static void _PG_Trade_LtoC_OpenTrade        	( int Sockid , int Size , void* Data );
    static void _PG_Trade_LtoC_TargetPutItem    	( int Sockid , int Size , void* Data );
    static void _PG_Trade_LtoC_TargetPutMoney   	( int Sockid , int Size , void* Data );
    static void _PG_Trade_LtoC_OwnerPutMoney    	( int Sockid , int Size , void* Data );
//    static void _PG_Trade_LtoC_TargetItemOK   	  ( int Sockid , int Size , void* Data );
//    static void _PG_Trade_LtoC_CancelItemOK   	  ( int Sockid , int Size , void* Data );
//    static void _PG_Trade_LtoC_FinalOKEnable  	  ( int Sockid , int Size , void* Data );
//    static void _PG_Trade_LtoC_TargetFinalOK  	  ( int Sockid , int Size , void* Data );
    static void _PG_Trade_LtoC_ClearTradeItem   	( int Sockid , int Size , void* Data );
    static void _PG_Trade_LtoC_TradeItemResult  	( int Sockid , int Size , void* Data );
    static void _PG_Trade_LtoC_PutItemResult    	( int Sockid , int Size , void* Data );

	static void _PG_Trade_LtoC_RequestTradeError	( int Sockid , int Size , void* Data );
	static void _PG_Trade_LtoC_DisagreeTrade    	( int Sockid , int Size , void* Data );
	static void _PG_Trade_LtoC_StopTrade	    	( int Sockid , int Size , void* Data );

	static void _PG_Trade_LtoC_TradeState	    	( int Sockid , int Size , void* Data );
	static void _PG_Trade_LtoC_CancelTradeRequest	( int Sockid , int Size , void* Data );

	static void _PG_Trade_LtoC_FinalOKFailed		( int Sockid , int Size , void* Data );
public:    
    NetCli_Trade() { This = this; }

	static	NetCli_Trade*	GetThis()						{ return This; }
    static void S_RequestTrade   	( int TargetID );
    static void S_AgreeTrade     	( int TargetID , bool Result , int ClientResult );
    static void S_PutItem        	( int Pos , int ItemPos , ItemFieldStruct& Item );
    static void S_PutMoney       	( int Money , int Money_Account );
    static void S_ItemOK         	( );
    static void S_FinalOK        	( const char* Password );
    static void S_StopTrade      	( int TargetID );
	static void S_CancelTardeRequest( int TargetID );



    virtual void R_RequestTrade          ( int TargetID ) = 0;
    virtual void R_OpenTrade             ( char* Name , int TargetID ) = 0;
    virtual void R_TargetPutItem         ( int Pos , ItemFieldStruct& Item ) = 0;
    virtual void R_TargetPutMoney        ( int Money , int Money_Account ) = 0;
    virtual void R_OwnerPutMoney         ( int Money , int Money_Account ) = 0;
//    virtual void R_TargetItemOK          ( ) = 0;
//    virtual void R_CancelItemOK          ( ) = 0;
//    virtual void R_FinalOKEnable         ( ) = 0;
//    virtual void R_TargetFinalOK         ( ) = 0;
    virtual void R_ClearTradeItem        ( ) = 0;
    virtual void R_TradeItem_OK          ( ) = 0;
    virtual void R_TradeItem_Failed      ( ) = 0;
    virtual void R_PutItem_OK            ( ) = 0;
    virtual void R_PutItem_Failed        ( ) = 0;

	virtual void R_RequestTradeError     ( ) = 0;
	virtual void R_DisagreeTrade         ( int ClientResult ) = 0;
	virtual void R_StopTrade			 ( int TargetID ) = 0;
	virtual void R_TradeState			 ( bool TradeOK , bool FinalOK , bool TargetTradeOK , bool TargetFinalOK ) = 0;
	virtual void R_CancelTardeRequest	 ( int TargetID ) = 0;
	virtual void R_FinalOKFailed		 ( FinalOKFailedENUM Result ) = 0;
};

