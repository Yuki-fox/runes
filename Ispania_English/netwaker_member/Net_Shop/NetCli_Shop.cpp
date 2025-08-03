#include "NetCli_Shop.h"
#include "../../MainProc/NetGlobal.h"
//-------------------------------------------------------------------
NetCli_Shop*    NetCli_Shop::This         = NULL;

//-------------------------------------------------------------------
bool NetCli_Shop::_Init()
{
	/*
	_Net.RegOnSrvPacketFunction	( EM_PG_Shop_LtoC_ShopOpen                      , _PG_Shop_LtoC_ShopOpen                        );
	_Net.RegOnSrvPacketFunction	( EM_PG_Shop_LtoC_ShopClose                     , _PG_Shop_LtoC_ShopClose                       );
	_Net.RegOnSrvPacketFunction	( EM_PG_Shop_LtoC_BuyItemResult                 , _PG_Shop_LtoC_BuyItemResult                   );
	_Net.RegOnSrvPacketFunction	( EM_PG_Shop_LtoC_SelectSellItemResult          , _PG_Shop_LtoC_SelectSellItemResult            );
	_Net.RegOnSrvPacketFunction	( EM_PG_Shop_LtoC_RepairWeaponResult            , _PG_Shop_LtoC_RepairWeaponResult              );
	_Net.RegOnSrvPacketFunction	( EM_PG_Shop_LtoC_AddSellItemRecord	          	, _PG_Shop_LtoC_AddSellItemRecord	            );
	_Net.RegOnSrvPacketFunction	( EM_PG_Shop_LtoC_DelSellItemRecord	          	, _PG_Shop_LtoC_DelSellItemRecord	            );
	_Net.RegOnSrvPacketFunction	( EM_PG_Shop_LtoC_FixShopItemCount	          	, _PG_Shop_LtoC_FixShopItemCount	            );
	_Net.RegOnSrvPacketFunction	( EM_PG_Shop_LtoC_AccountMoneyShopBuyListResult	, _PG_Shop_LtoC_AccountMoneyShopBuyListResult	);
	_Net.RegOnSrvPacketFunction	( EM_PG_Shop_LtoC_FixSellItemBackup				, _PG_Shop_LtoC_FixSellItemBackup				);
*/

	Cli_NetReg( PG_Shop_LtoC_ShopOpen                      	);
	Cli_NetReg( PG_Shop_LtoC_ShopClose                     	);
	Cli_NetReg( PG_Shop_LtoC_BuyItemResult                 	);
	Cli_NetReg( PG_Shop_LtoC_SelectSellItemResult          	);
	Cli_NetReg( PG_Shop_LtoC_RepairWeaponResult            	);
	Cli_NetReg( PG_Shop_LtoC_AddSellItemRecord	          	);
	Cli_NetReg( PG_Shop_LtoC_DelSellItemRecord	          	);
	Cli_NetReg( PG_Shop_LtoC_FixShopItemCount	          	);
	Cli_NetReg( PG_Shop_LtoC_AccountMoneyShopBuyListResult	);
	Cli_NetReg( PG_Shop_LtoC_FixSellItemBackup				);

	return true;
}
//-------------------------------------------------------------------
bool NetCli_Shop::_Release()
{
    return true;
}
//-------------------------------------------------------------------
void NetCli_Shop::_PG_Shop_LtoC_FixSellItemBackup			( int Sockid , int Size , void* Data )
{
	PG_Shop_LtoC_FixSellItemBackup*  PG = (PG_Shop_LtoC_FixSellItemBackup*)Data;
	This->R_FixSellItemBackup( PG->SellItemBackup );
}
void NetCli_Shop::_PG_Shop_LtoC_AccountMoneyShopBuyListResult( int Sockid , int Size , void* Data )
{
	PG_Shop_LtoC_AccountMoneyShopBuyListResult*  PG = (PG_Shop_LtoC_AccountMoneyShopBuyListResult*)Data;
	This->R_AccountMoneyShopBuyListResult( PG->Result );
}

/*
void NetCli_Shop::_PG_Shop_LtoC_AccountMoneyShopResult  ( int Sockid , int Size , void* Data )
{
	PG_Shop_LtoC_AccountMoneyShopResult*  PG = (PG_Shop_LtoC_AccountMoneyShopResult*)Data;
	This->R_AccountMoneyShopResult( PG->OrgObjID , PG->Result );
}
*/

void NetCli_Shop::_PG_Shop_LtoC_AddSellItemRecord	  ( int Sockid , int Size , void* Data )
{
	PG_Shop_LtoC_AddSellItemRecord*  PG = (PG_Shop_LtoC_AddSellItemRecord*)Data;
	This->R_AddSellItemRecord( PG->Info );
}
void NetCli_Shop::_PG_Shop_LtoC_DelSellItemRecord	  ( int Sockid , int Size , void* Data )
{
	PG_Shop_LtoC_DelSellItemRecord*  PG = (PG_Shop_LtoC_DelSellItemRecord*)Data;
	This->R_DelSellItemRecord( PG->EarsePos );
}
void NetCli_Shop::_PG_Shop_LtoC_FixShopItemCount	  ( int Sockid , int Size , void* Data )
{
	PG_Shop_LtoC_FixShopItemCount*  PG = (PG_Shop_LtoC_FixShopItemCount*)Data;
	This->R_FixShopItemCount( PG->TraderID , PG->ShopObjID , PG->Pos , PG->ItemCount );
}

void NetCli_Shop::_PG_Shop_LtoC_ShopOpen            ( int Sockid , int Size , void* Data )
{
    PG_Shop_LtoC_ShopOpen*  PG = (PG_Shop_LtoC_ShopOpen*)Data;
    This->R_ShopOpen( PG->TraderID , PG->ShopObjID , PG->ItemCount );
}
void NetCli_Shop::_PG_Shop_LtoC_ShopClose           ( int Sockid , int Size , void* Data )
{
    PG_Shop_LtoC_ShopClose*  PG = (PG_Shop_LtoC_ShopClose*)Data;
    This->R_ShopClose( );
}
void NetCli_Shop::_PG_Shop_LtoC_BuyItemResult       ( int Sockid , int Size , void* Data )
{
    PG_Shop_LtoC_BuyItemResult*  PG = (PG_Shop_LtoC_BuyItemResult*)Data;
    This->R_BuyItemResult( PG->TraderID , PG->OrgObjID , PG->Count , PG->Result , PG->PriceType , PG->Money );
}
void NetCli_Shop::_PG_Shop_LtoC_SelectSellItemResult( int Sockid , int Size , void* Data )
{
    PG_Shop_LtoC_SelectSellItemResult*  PG = (PG_Shop_LtoC_SelectSellItemResult*)Data;
    This->R_SelectSellItemResult( PG->TraderID , PG->FieldID , PG->Result  );
}
void NetCli_Shop::_PG_Shop_LtoC_RepairWeaponResult  ( int Sockid , int Size , void* Data )
{
    PG_Shop_LtoC_RepairWeaponResult*  PG = (PG_Shop_LtoC_RepairWeaponResult*)Data;
    This->R_RepairWeaponResult( PG->TraderID , PG->Pos , PG->IsWeapon , PG->Result );
}

void NetCli_Shop::S_ShopCloseNotify        ( )
{
    PG_Shop_CtoL_ShopCloseNotify Send;

    SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Shop::S_BuyItemRequest         ( int OrgObjID , int Count ,int pos )
{
    PG_Shop_CtoL_BuyItemRequest Send;
    Send.OrgObjID = OrgObjID;
    Send.Count    = Count;
	Send.Pos = pos;
    SendToLocal( sizeof(Send) , &Send );
}

void NetCli_Shop::S_SelectSellItemRequest  ( int FieldID , ItemFieldStruct* SellItem )
{
    PG_Shop_CtoL_SelectSellItemRequest Send;
    Send.FieldID = FieldID;
    Send.SellItem = *SellItem;
    SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Shop::S_RepairWeaponRequest    ( int Pos , bool IsWeapon )
{
    PG_Shop_CtoL_RepairWeaponRequest Send;

    Send.Pos = Pos;
    Send.IsWeapon = IsWeapon;
    SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Shop::S_BuySellItemRecord     ( SellItemFieldStruct& Info )
{
	PG_Shop_CtoL_BuySellItemRecord Send;

	Send.Info = Info;

	SendToLocal( sizeof(Send) , &Send );
}
/*
//要求購買一組點數商品
void NetCli_Shop::S_AccountMoneyShop		( int OrgObjID , int Money_Account )
{
	PG_Shop_CtoL_AccountMoneyShop Send;
	Send.OrgObjID = OrgObjID;
	Send.Money_Account = Money_Account;
	
	SendToLocal( sizeof(Send) , &Send );
}
*/
void NetCli_Shop::S_AccountMoneyShopBuyList	( int TotalCost , AccountMoneyBuyStruct	BuyList[20] )
{
	PG_Shop_CtoL_AccountMoneyShopBuyList Send;
	Send.TotalCost = TotalCost;
	memcpy( Send.BuyList , BuyList , sizeof(Send.BuyList) );
	SendToLocal( sizeof(Send) , &Send );
}