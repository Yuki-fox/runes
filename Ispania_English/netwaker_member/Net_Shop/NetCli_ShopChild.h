#pragma once

#include "NetCli_Shop.h"

class NetCli_ShopChild : public NetCli_Shop
{
public:
	static bool _Init();
	static bool _Release();

	virtual void R_ShopOpen            ( int TraderID , int ShopObjID , int ItemCount[30] ); 
	virtual void R_ShopClose           ( );
	virtual void R_BuyItemResult       ( int TraderID , int OrgObjID , int Count , bool Result , PriceTypeENUM PriceType[2] , int Money[2] );
	virtual void R_SelectSellItemResult( int TraderID , int FieldID , bool Result );
	virtual void R_RepairWeaponResult  ( int TraderID , int Pos , bool IsWeapon , bool Result );

	virtual void R_AddSellItemRecord	( SellItemFieldStruct& Info );
	virtual void R_DelSellItemRecord	( int EarsePos );
	virtual void R_FixShopItemCount		( int TraderID , int ShopObjID , int Pos , int ItemCount );
	//購買商城的結果
//	virtual void R_AccountMoneyShopResult( int OrgObjID , AccountMoneyShopResultENUM Result );
	virtual void R_AccountMoneyShopBuyListResult( AccountMoneyShopResultENUM Result );
	virtual void R_FixSellItemBackup( StaticVector< SellItemFieldStruct , _MAX_SELL_ITEM_SAVE_COUNT_ >&	SellItemBackup );
};

