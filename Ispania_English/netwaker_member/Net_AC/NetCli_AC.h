#pragma once

#include "PG/PG_AC.h"
#include "../../MainProc/NetGlobal.h"

class NetCli_AC : public CNetGlobal
{
protected:
    //-------------------------------------------------------------------
    static NetCli_AC* This;
    static bool _Init();
    static bool _Release();
    //-------------------------------------------------------------------
	static void _PG_AC_DCtoC_SearchItemCountResult		( int Sockid , int Size , void* Data );
	static void _PG_AC_DCtoC_SearchItemResult			( int Sockid , int Size , void* Data );	
	static void _PG_AC_DCtoC_FixSearchItemResult		( int Sockid , int Size , void* Data );	
	static void _PG_AC_DCtoC_SelfCompetitive_BuyItem	( int Sockid , int Size , void* Data );	
	static void _PG_AC_DCtoC_SelfCompetitive_SellItem	( int Sockid , int Size , void* Data );	
	static void _PG_AC_DCtoC_SelfCompetitive_ItemDataEnd( int Sockid , int Size , void* Data );	
	static void _PG_AC_DCtoC_CancelSellItemResult		( int Sockid , int Size , void* Data );	
	static void _PG_AC_DCtoC_CloseAC					( int Sockid , int Size , void* Data );	
	static void _PG_AC_DCtoC_OpenAC						( int Sockid , int Size , void* Data );
	static void _PG_AC_DCtoC_LostCompetitiveItem		( int Sockid , int Size , void* Data );
	static void _PG_AC_DCtoC_FixBuyOrderItem			( int Sockid , int Size , void* Data );
	static void _PG_AC_DCtoC_FixSellOrderItem			( int Sockid , int Size , void* Data );
	static void _PG_AC_DCtoC_ItemHistoryListResult		( int Sockid , int Size , void* Data );

	static void _PG_AC_LtoC_BuyItemResult				( int Sockid , int Size , void* Data );	
	static void _PG_AC_LtoC_SellItemResult				( int Sockid , int Size , void* Data );
	static void _PG_AC_LtoC_AcAlreadyOpen				( int Sockid , int Size , void* Data );
	static void _PG_AC_LtoC_SellMoneyResult				( int Sockid , int Size , void* Data );
	static void _PG_AC_DtoC_AcTradeOk					( int Sockid , int Size , void* Data );
public:    
    NetCli_AC() { This = this; }

	virtual void RD_SearchItemCountResult	( int MaxCount , int Count , int BeginIndex ) = 0;
	virtual void RD_SearchItemResult		( int Index , ACItemInfoStruct& Data , char* SellerName , int BuyerDBID) = 0;
	virtual void RD_FixSearchItemResult		( int ACItemDBID , ACItemStateENUM State , int NewPrices , PriceTypeENUM Type , int BuyerDBID ) = 0;
	virtual void RD_SelfCompetitive_BuyItem	( ACItemInfoStruct& Data , char* SellerName , int BuyerDBID  ) = 0;
	virtual void RD_SelfCompetitive_SellItem( ACItemInfoStruct& Data , char* BuyerName ) = 0;
	virtual void RD_SelfCompetitive_ItemDataEnd(  ) = 0;
	virtual void RD_CancelSellItemResult	( int ACItemDBID ) = 0;
	virtual void RD_CloseAC					( ) = 0;
	virtual void RD_OpenAC					( int NpcGUID , bool Result ) = 0;
	virtual void RD_LostCompetitiveItem		( int ACItemDBID , ItemFieldStruct& Item ) = 0;
	virtual void RD_FixBuyOrderItem			( int ACItemDBID , ACItemStateENUM State , int NewPrices , PriceTypeENUM PricesType , int BuyerDBID ) = 0;
	virtual void RD_FixSellOrderItem		( int ACItemDBID , ACItemStateENUM State , int NewPrices , PriceTypeENUM PricesType , char* BuyerName ) = 0;
	virtual void RD_ItemHistoryListResult	( vector< DB_ACItemHistoryStruct >& ItemList ) = 0;

	virtual void RL_BuyItemResult			( bool Result , int ACItemDBID , int Prices , PriceTypeENUM PricesType , ACItemStateENUM State) = 0;
	virtual void RL_SellItemResult			( ACSellItemResultENUM Result , ACItemInfoStruct& Item ) = 0;
	virtual void RL_AcAlreadyOpen			( int TareTargtetNPC ) = 0;
	virtual void RL_SellMoneyResult			( SellMoneyResultENUM Result  , ACItemInfoStruct& Item ) = 0;
	virtual void RL_AcTradeOk				( const char* BuyerName , const char* SellerName , int PriceType , int Money , int Tax , ItemFieldStruct& Item ) = 0;

	static void SL_SearchItem			( ACSearchConditionStruct& Data );
	static void SL_GetNextSearchItem	( );
	static void SL_GetMyACItemInfo		( );
	static void SL_BuyItem				( int ACItemDBID , int Prices , PriceTypeENUM PricesType , char* Password );
	static void SL_SellItem				( int BodyPos , ItemFieldStruct& Item , int Prices , int BuyOutPrices , PriceTypeENUM PricesType , int LiveTime );
	static void SL_CancelSellItem		( int ACItemDBID );
	static void SL_CloseAC				( );
	static void SL_ItemHistoryRequest	( int OrgObjID );
	static void SL_SellMoney			( PriceTypeENUM	 SellType , int	SellMoney ,	int Prices , int BuyOutPrices , int LiveTime , PriceTypeENUM PriceType , const char* Password );

};

