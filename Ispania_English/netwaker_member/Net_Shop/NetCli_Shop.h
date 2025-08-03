#pragma once

#include "PG/PG_Shop.h"
#include "../../MainProc/NetGlobal.h"

class NetCli_Shop : public CNetGlobal
{
protected:
    //-------------------------------------------------------------------
    static NetCli_Shop* This;
    static bool _Init();
    static bool _Release();
    //-------------------------------------------------------------------

    static void _PG_Shop_LtoC_ShopOpen            		( int Sockid , int Size , void* Data ); 
    static void _PG_Shop_LtoC_ShopClose           		( int Sockid , int Size , void* Data );
    static void _PG_Shop_LtoC_BuyItemResult       		( int Sockid , int Size , void* Data );
    static void _PG_Shop_LtoC_SelectSellItemResult		( int Sockid , int Size , void* Data );
    static void _PG_Shop_LtoC_RepairWeaponResult  		( int Sockid , int Size , void* Data );

	static void _PG_Shop_LtoC_AddSellItemRecord	  		( int Sockid , int Size , void* Data );
	static void _PG_Shop_LtoC_DelSellItemRecord	  		( int Sockid , int Size , void* Data );
	static void _PG_Shop_LtoC_FixShopItemCount	  		( int Sockid , int Size , void* Data );

//	static void _PG_Shop_LtoC_AccountMoneyShopResult	( int Sockid , int Size , void* Data );
	static void _PG_Shop_LtoC_AccountMoneyShopBuyListResult( int Sockid , int Size , void* Data );
	static void _PG_Shop_LtoC_FixSellItemBackup			( int Sockid , int Size , void* Data );
public:    
    NetCli_Shop() { This = this; }

    static void S_ShopCloseNotify        ( );
    static void S_BuyItemRequest         (  int OrgObjID , int Count , int pos );
    static void S_SelectSellItemRequest  (  int FieldID , ItemFieldStruct* SellItem );
	//如果 Pos = -1 and IsWeapon = true 表示修理所有裝備
    static void S_RepairWeaponRequest    (  int Pos , bool IsWeapon );

	//買回物品
	static void S_BuySellItemRecord     ( SellItemFieldStruct& Info );

	//要求購買一組點數商品
//	static void S_AccountMoneyShop		( int OrgObjID , int Money_Account );
	static void S_AccountMoneyShopBuyList	( int TotalCost , AccountMoneyBuyStruct	BuyList[20] );

    virtual void R_ShopOpen            	( int TraderID , int ShopObjID , int ItemCount[30] ) = 0;  
    virtual void R_ShopClose           	( ) = 0;
    virtual void R_BuyItemResult       	( int TraderID , int OrgObjID , int Count , bool Result , PriceTypeENUM PriceType[2] , int Money[2] ) = 0;
    virtual void R_SelectSellItemResult	( int TraderID , int FieldID , bool Result ) = 0;    	
    virtual void R_RepairWeaponResult  	( int TraderID , int Pos , bool IsWeapon , bool Result ) = 0;

	virtual void R_AddSellItemRecord	( SellItemFieldStruct& Info ) = 0;
	virtual void R_DelSellItemRecord	( int EarsePos ) = 0;
	virtual void R_FixShopItemCount		( int TraderID , int ShopObjID , int Pos , int ItemCount ) = 0;
	//購買商城的結果
//	virtual void R_AccountMoneyShopResult( int OrgObjID , AccountMoneyShopResultENUM Result ) = 0;;
	virtual void R_AccountMoneyShopBuyListResult( AccountMoneyShopResultENUM Result ) = 0;
	virtual void R_FixSellItemBackup( StaticVector< SellItemFieldStruct , _MAX_SELL_ITEM_SAVE_COUNT_ >&	SellItemBackup ) = 0;
};

