#include "NetCli_Item.h"

//-------------------------------------------------------------------
NetCli_Item*    NetCli_Item::This         = NULL;

ItemFieldStruct NetCli_Item::s_useItemSource;			// 暫存值
ItemFieldStruct NetCli_Item::s_useItemTarget;			// 暫存值
queue<int>		NetCli_Item::s_exchangeBody;			// 暫存物品交換

//-------------------------------------------------------------------
bool NetCli_Item::_Init()
{
	/*
    _Net.RegOnSrvPacketFunction	( EM_PG_Item_LtoC_GetItemResult         	, _PG_Item_LtoC_GetItemResult          	);
    _Net.RegOnSrvPacketFunction	( EM_PG_Item_LtoC_ClsItemInBuffer       	, _PG_Item_LtoC_ClsItemInBuffer        	);
    _Net.RegOnSrvPacketFunction	( EM_PG_Item_LtoC_NewItemInBuffer       	, _PG_Item_LtoC_NewItemInBuffer        	);
    _Net.RegOnSrvPacketFunction	( EM_PG_Item_LtoC_DeleteItem            	, _PG_Item_LtoC_DeleteItem             	);
    _Net.RegOnSrvPacketFunction	( EM_PG_Item_LtoC_ExchangeField         	, _PG_Item_LtoC_ExchangeField          	);
    _Net.RegOnSrvPacketFunction	( EM_PG_Item_LtoC_BodyBankMoneyExchange 	, _PG_Item_LtoC_BodyBankMoneyExchange  	);
    _Net.RegOnSrvPacketFunction	( EM_PG_Item_LtoC_FixItemInfo           	, _PG_Item_LtoC_FixItemInfo            	);
    _Net.RegOnSrvPacketFunction	( EM_PG_Item_LtoC_FixAllItemInfo        	, _PG_Item_LtoC_FixAllItemInfo         	);
    _Net.RegOnSrvPacketFunction	( EM_PG_Item_LtoC_FixMoney              	, _PG_Item_LtoC_FixMoney               	);
    _Net.RegOnSrvPacketFunction	( EM_PG_Item_LtoC_MaxItemCount          	, _PG_Item_LtoC_MaxItemCount           	);
    _Net.RegOnSrvPacketFunction	( EM_PG_Item_LtoC_OpenBank              	, _PG_Item_LtoC_OpenBank               	);
    _Net.RegOnSrvPacketFunction	( EM_PG_Item_LtoC_CloseBank             	, _PG_Item_LtoC_CloseBank              	);
    _Net.RegOnSrvPacketFunction	( EM_PG_Item_LtoC_UseItemResult         	, _PG_Item_LtoC_UseItemResult          	);
	_Net.RegOnSrvPacketFunction	( EM_PG_Item_LtoC_SetKeyItemFlag        	, _PG_Item_LtoC_SetKeyItemFlag         	);
	_Net.RegOnSrvPacketFunction	( EM_PG_Item_LtoC_SetTitleFlag				, _PG_Item_LtoC_SetTitleFlag         	);
	_Net.RegOnSrvPacketFunction	( EM_PG_Item_LtoC_SetCardFlag				, _PG_Item_LtoC_SetCardFlag         	);
	_Net.RegOnSrvPacketFunction	( EM_PG_Item_LtoC_UseItemToItem_Result  	, _PG_Item_LtoC_UseItemToItem_Result	);
	_Net.RegOnSrvPacketFunction	( EM_PG_Item_LtoC_UsePowerLight				, _PG_Item_LtoC_UsePowerLight			);
	_Net.RegOnSrvPacketFunction	( EM_PG_Item_LtoC_EqRefineResult			, _PG_Item_LtoC_EqRefineResult			);
	_Net.RegOnSrvPacketFunction	( EM_PG_Item_LtoC_ItemDissolutionResult		, _PG_Item_LtoC_ItemDissolutionResult	);
	_Net.RegOnSrvPacketFunction	( EM_PG_Item_LtoC_TakeOutCardResult			, _PG_Item_LtoC_TakeOutCardResult		);
	_Net.RegOnSrvPacketFunction	( EM_PG_Item_LtoC_ApartBodyItemResult		, _PG_Item_LtoC_ApartBodyItemResult		);
	_Net.RegOnSrvPacketFunction	( EM_PG_Item_LtoC_DigHoleResult				, _PG_Item_LtoC_DigHoleResult			);
	_Net.RegOnSrvPacketFunction	( EM_PG_Item_LtoC_EQCombinOpen				, _PG_Item_LtoC_EQCombinOpen			);
	_Net.RegOnSrvPacketFunction	( EM_PG_Item_LtoC_EQCombinClose				, _PG_Item_LtoC_EQCombinClose			);
	_Net.RegOnSrvPacketFunction	( EM_PG_Item_LtoC_EQCombinResult			, _PG_Item_LtoC_EQCombinResult			);
	_Net.RegOnSrvPacketFunction	( EM_PG_Item_LtoC_OpenAccountBag			, _PG_Item_LtoC_OpenAccountBag			);
	_Net.RegOnSrvPacketFunction	( EM_PG_Item_LtoC_AccountBagGetItemResult	, _PG_Item_LtoC_AccountBagGetItemResult	);
	_Net.RegOnSrvPacketFunction	( EM_PG_Item_LtoC_ItemTimeout				, _PG_Item_LtoC_ItemTimeout				);
	_Net.RegOnSrvPacketFunction	( EM_PG_Item_LtoC_MagicBoxResult			, _PG_Item_LtoC_MagicBoxResult			);
*/

	Cli_NetReg( PG_Item_LtoC_GetItemResult         	 );
	Cli_NetReg( PG_Item_LtoC_ClsItemInBuffer       	 );
	Cli_NetReg( PG_Item_LtoC_NewItemInBuffer       	 );
	Cli_NetReg( PG_Item_LtoC_DeleteItem            	 );
	Cli_NetReg( PG_Item_LtoC_ExchangeField         	 );
	Cli_NetReg( PG_Item_LtoC_BodyBankMoneyExchange 	 );
	Cli_NetReg( PG_Item_LtoC_FixItemInfo           	 );
	Cli_NetReg( PG_Item_LtoC_FixAllItemInfo        	 );
	Cli_NetReg( PG_Item_LtoC_FixMoney              	 );
	Cli_NetReg( PG_Item_LtoC_MaxItemCount          	 );
	Cli_NetReg( PG_Item_LtoC_OpenBank              	 );
	Cli_NetReg( PG_Item_LtoC_CloseBank             	 );
	Cli_NetReg( PG_Item_LtoC_UseItemResult         	 );
	Cli_NetReg( PG_Item_LtoC_SetKeyItemFlag        	 );
	Cli_NetReg( PG_Item_LtoC_SetTitleFlag			 );
	Cli_NetReg( PG_Item_LtoC_SetCardFlag			 );
	Cli_NetReg( PG_Item_LtoC_UseItemToItem_Result  	 );
	Cli_NetReg( PG_Item_LtoC_UsePowerLight			 );
	Cli_NetReg( PG_Item_LtoC_EqRefineResult			 );
	Cli_NetReg( PG_Item_LtoC_ItemDissolutionResult	 );
	Cli_NetReg( PG_Item_LtoC_TakeOutCardResult		 );
	Cli_NetReg( PG_Item_LtoC_ApartBodyItemResult	 );
	Cli_NetReg( PG_Item_LtoC_DigHoleResult			 );
	Cli_NetReg( PG_Item_LtoC_EQCombinOpen			 );
	Cli_NetReg( PG_Item_LtoC_EQCombinClose			 );
	Cli_NetReg( PG_Item_LtoC_EQCombinResult			 );
	Cli_NetReg( PG_Item_LtoC_OpenAccountBag			 );
	Cli_NetReg( PG_Item_LtoC_AccountBagGetItemResult );
	Cli_NetReg( PG_Item_LtoC_ItemTimeout			 );
	Cli_NetReg( PG_Item_LtoC_MagicBoxResult			 );
	Cli_NetReg( PG_Item_LtoC_UseEq					 );
	Cli_NetReg( PG_Item_LtoC_GamebleItemStateResult  );
	Cli_NetReg( PG_Item_LtoC_GetGamebleItemStateResult );

	Cli_NetReg( PG_Item_LtoC_OpenGamebleItemState	);
	Cli_NetReg( PG_Item_LtoC_CloseGamebleItemState	);

	Cli_NetReg( PG_Item_LtoC_OpenMagicStoneExchange	);
	Cli_NetReg( PG_Item_LtoC_MagicStoneExchangeResult);
	Cli_NetReg( PG_Item_LtoC_DrawOutRuneResult	);

	Cli_NetReg( PG_Item_LtoC_PutCoolClothResult		);
	Cli_NetReg( PG_Item_LtoC_DelCoolClothResult		);
	Cli_NetReg( PG_Item_LtoC_SetCoolSuitResult		);
	Cli_NetReg( PG_Item_LtoC_ShowCoolSuitIndexResult);
	Cli_NetReg( PG_Item_LtoC_SwapCoolClothResult	);

	Cli_NetReg( PG_Item_LtoC_PutInExBagResult		);
	Cli_NetReg( PG_Item_LtoC_PopExBagResult			);
	Cli_NetReg( PG_Item_LtoC_UseExBagResult			);

	Cli_NetReg( PG_Item_LtoC_OpenRare3EqExchangeItem		);
	Cli_NetReg( PG_Item_LtoC_Rare3EqExchangeItemResult		);

	Cli_NetReg( PG_Item_LtoC_ClsAttributeResult				);
	Cli_NetReg( PG_Item_LtoC_MagicBoxAttrDrawOffResult		);

    return true;
}
//-------------------------------------------------------------------
bool NetCli_Item::_Release()
{
    return true;
}
//-------------------------------------------------------------------
void NetCli_Item::_PG_Item_LtoC_MagicBoxAttrDrawOffResult	( int Sockid , int Size , void* Data )
{
	PG_Item_LtoC_MagicBoxAttrDrawOffResult*  PG = (PG_Item_LtoC_MagicBoxAttrDrawOffResult*)Data;
	This->R_MagicBoxAttrDrawOffResult( PG->Result );
}
void NetCli_Item::_PG_Item_LtoC_ClsAttributeResult		( int Sockid , int Size , void* Data )
{
	PG_Item_LtoC_ClsAttributeResult*  PG = (PG_Item_LtoC_ClsAttributeResult*)Data;
	This->R_ClsAttributeResult( PG->Result );
}
void NetCli_Item::_PG_Item_LtoC_PopExBagResult			( int Sockid , int Size , void* Data )
{
	PG_Item_LtoC_PopExBagResult*  PG = (PG_Item_LtoC_PopExBagResult*)Data;
	This->R_PopExBagResult( PG->Result );
}
void NetCli_Item::_PG_Item_LtoC_OpenRare3EqExchangeItem	( int Sockid , int Size , void* Data )
{
	PG_Item_LtoC_OpenRare3EqExchangeItem*  PG = (PG_Item_LtoC_OpenRare3EqExchangeItem*)Data;
	This->RL_OpenRare3EqExchangeItem( PG->TargetNPCID );
}
void NetCli_Item::_PG_Item_LtoC_Rare3EqExchangeItemResult( int Sockid , int Size , void* Data )
{
	PG_Item_LtoC_Rare3EqExchangeItemResult*  PG = (PG_Item_LtoC_Rare3EqExchangeItemResult*)Data;
	This->RL_Rare3EqExchangeItemResult( PG->Result );
}

void NetCli_Item::_PG_Item_LtoC_PutInExBagResult		( int Sockid , int Size , void* Data )
{
	PG_Item_LtoC_PutInExBagResult*  PG = (PG_Item_LtoC_PutInExBagResult*)Data;
	This->R_PutInExBagResult( PG->Result );
}
void NetCli_Item::_PG_Item_LtoC_UseExBagResult			( int Sockid , int Size , void* Data )
{
	PG_Item_LtoC_UseExBagResult*  PG = (PG_Item_LtoC_UseExBagResult*)Data;
	This->R_UseExBagResult( PG->Result );
}
void NetCli_Item::_PG_Item_LtoC_PutCoolClothResult		( int Sockid , int Size , void* Data )
{
	PG_Item_LtoC_PutCoolClothResult*  PG = (PG_Item_LtoC_PutCoolClothResult*)Data;
	This->R_PutCoolClothResult( PG->Result );
}
void NetCli_Item::_PG_Item_LtoC_DelCoolClothResult		( int Sockid , int Size , void* Data )
{
	PG_Item_LtoC_DelCoolClothResult*  PG = (PG_Item_LtoC_DelCoolClothResult*)Data;
	This->R_DelCoolClothResult( PG->Result );
}
void NetCli_Item::_PG_Item_LtoC_SetCoolSuitResult		( int Sockid , int Size , void* Data )
{
	PG_Item_LtoC_SetCoolSuitResult*  PG = (PG_Item_LtoC_SetCoolSuitResult*)Data;
	This->R_SetCoolSuitResult( PG->Result );
}
void NetCli_Item::_PG_Item_LtoC_ShowCoolSuitIndexResult	( int Sockid , int Size , void* Data )
{
	PG_Item_LtoC_ShowCoolSuitIndexResult*  PG = (PG_Item_LtoC_ShowCoolSuitIndexResult*)Data;
	This->R_ShowCoolSuitIndexResult( PG->Result );
}
void NetCli_Item::_PG_Item_LtoC_SwapCoolClothResult	( int Sockid , int Size , void* Data )
{
	PG_Item_LtoC_SwapCoolClothResult*  PG = (PG_Item_LtoC_SwapCoolClothResult*)Data;
	This->R_SwapCoolClothResult( PG->Result );
}

void NetCli_Item::_PG_Item_LtoC_DrawOutRuneResult		( int Sockid , int Size , void* Data )
{
	PG_Item_LtoC_DrawOutRuneResult*  PG = (PG_Item_LtoC_DrawOutRuneResult*)Data;
	This->R_DrawOutRuneResult( PG->Result );
}

void NetCli_Item::_PG_Item_LtoC_OpenMagicStoneExchange	( int Sockid , int Size , void* Data )
{
	PG_Item_LtoC_OpenMagicStoneExchange*  PG = (PG_Item_LtoC_OpenMagicStoneExchange*)Data;
	This->R_OpenMagicStoneExchange( PG->MagicStoneID , PG->Lv , PG->Money );
}
void NetCli_Item::_PG_Item_LtoC_MagicStoneExchangeResult( int Sockid , int Size , void* Data )
{
	PG_Item_LtoC_MagicStoneExchangeResult*  PG = (PG_Item_LtoC_MagicStoneExchangeResult*)Data;
	This->R_MagicStoneExchangeResult( PG->Item , PG->Result , PG->Money );
}
void NetCli_Item::_PG_Item_LtoC_OpenGamebleItemState	( int Sockid , int Size , void* Data )
{
	PG_Item_LtoC_OpenGamebleItemState*  PG = (PG_Item_LtoC_OpenGamebleItemState*)Data;
	This->R_OpenGamebleItemState( PG->TargetID );
}
void NetCli_Item::_PG_Item_LtoC_CloseGamebleItemState	( int Sockid , int Size , void* Data )
{
	PG_Item_LtoC_CloseGamebleItemState*  PG = (PG_Item_LtoC_CloseGamebleItemState*)Data;
	This->R_CloseGamebleItemState( );
}
void NetCli_Item::_PG_Item_LtoC_DelItemMsg			( int Sockid , int Size , void* Data )
{
	PG_Item_LtoC_DelItemMsg*  PG = (PG_Item_LtoC_DelItemMsg*)Data;
	This->R_DelItemMsg( PG->Item );
}
void NetCli_Item::_PG_Item_LtoC_GamebleItemStateResult		( int Sockid , int Size , void* Data )
{
	PG_Item_LtoC_GamebleItemStateResult*  PG = (PG_Item_LtoC_GamebleItemStateResult*)Data;
	This->R_GamebleItemStateResult( PG->TableID , PG->StatePos , PG->StateID , PG->Result );
}
void NetCli_Item::_PG_Item_LtoC_GetGamebleItemStateResult	( int Sockid , int Size , void* Data )
{
	PG_Item_LtoC_GetGamebleItemStateResult*  PG = (PG_Item_LtoC_GetGamebleItemStateResult*)Data;
	This->R_GetGamebleItemStateResult( PG->Result );
}

void NetCli_Item::_PG_Item_LtoC_UseEq				( int Sockid , int Size , void* Data )
{
	PG_Item_LtoC_UseEq*  PG = (PG_Item_LtoC_UseEq*)Data;
	This->R_UseEq( PG->PlayerGUID , PG->EqID );
}
void NetCli_Item::_PG_Item_LtoC_MagicBoxResult		( int Sockid , int Size , void* Data )
{
	PG_Item_LtoC_MagicBoxResult*  PG = (PG_Item_LtoC_MagicBoxResult*)Data;
	This->R_MagicBoxResult( PG->Item , PG->Result );
}
/*
void NetCli_Item::_PG_Item_LtoC_SmeltEqtoMagicStoneResult	( int Sockid , int Size , void* Data ) 
{
	PG_Item_LtoC_SmeltEqtoMagicStoneResult*  PG = (PG_Item_LtoC_SmeltEqtoMagicStoneResult*)Data;
	This->R_SmeltEqtoMagicStoneResult( PG->EQPos , PG->SmeltItemPos , PG->BoxPos , PG->Result );
}
void NetCli_Item::_PG_Item_LtoC_RefineMagicStoneResult		( int Sockid , int Size , void* Data ) 
{
	PG_Item_LtoC_RefineMagicStoneResult*  PG = (PG_Item_LtoC_RefineMagicStoneResult*)Data;
	This->R_RefineMagicStoneResult( PG->MagicStonePos , PG->BoxPos , PG->Result );
}
void NetCli_Item::_PG_Item_LtoC_CombinEQandMagicStoneResult	( int Sockid , int Size , void* Data ) 
{
	PG_Item_LtoC_CombinEQandMagicStoneResult*  PG = (PG_Item_LtoC_CombinEQandMagicStoneResult*)Data;
	This->R_CombinEQandMagicStoneResult( PG->MagicStonePos , PG->EQPos , PG->BoxPos , PG->Result );
}
void NetCli_Item::_PG_Item_LtoC_RuneTransferResult			( int Sockid , int Size , void* Data ) 
{
	PG_Item_LtoC_RuneTransferResult*  PG = (PG_Item_LtoC_RuneTransferResult*)Data;
	This->R_RuneTransferResult( PG->RunePos , PG->BoxPos , PG->Result );
}
void NetCli_Item::_PG_Item_LtoC_RefineRuneResult			( int Sockid , int Size , void* Data ) 
{
	PG_Item_LtoC_RefineRuneResult*  PG = (PG_Item_LtoC_RefineRuneResult*)Data;
	This->R_RefineRuneResult( PG->RunePos , PG->BoxPos , PG->Result );
}
*/

void NetCli_Item::_PG_Item_LtoC_ItemTimeout			( int Sockid , int Size , void* Data )
{
	PG_Item_LtoC_ItemTimeout*  PG = (PG_Item_LtoC_ItemTimeout*)Data;
	This->R_ItemTimeout( PG->Pos , PG->Item );
}

void NetCli_Item::_PG_Item_LtoC_OpenAccountBag			( int Sockid , int Size , void* Data )
{
	PG_Item_LtoC_OpenAccountBag*  PG = (PG_Item_LtoC_OpenAccountBag*)Data;
	This->R_OpenAccountBag( PG->TargetNPC , PG->MaxCount , PG->ItemDBID , PG->Item );
}
void NetCli_Item::_PG_Item_LtoC_AccountBagGetItemResult	( int Sockid , int Size , void* Data )
{
	PG_Item_LtoC_AccountBagGetItemResult*  PG = (PG_Item_LtoC_AccountBagGetItemResult*)Data;
	This->R_AccountBagGetItemResult( PG->ItemDBID , PG->BodyPos , PG->Result );
}

void NetCli_Item::_PG_Item_LtoC_EQCombinOpen			( int Sockid , int Size , void* Data )
{
	PG_Item_LtoC_EQCombinOpen*  PG = (PG_Item_LtoC_EQCombinOpen*)Data;
	This->R_EQCombinOpen( PG->TargetNPC , PG->ItemPos , PG->Type );
}
void NetCli_Item::_PG_Item_LtoC_EQCombinClose			( int Sockid , int Size , void* Data )
{
	PG_Item_LtoC_EQCombinClose*  PG = (PG_Item_LtoC_EQCombinClose*)Data;
	This->R_EQCombinClose( );
}
void NetCli_Item::_PG_Item_LtoC_EQCombinResult		( int Sockid , int Size , void* Data )
{
	PG_Item_LtoC_EQCombinResult*  PG = (PG_Item_LtoC_EQCombinResult*)Data;
	This->R_EQCombinResult( PG->Result );
}
void NetCli_Item::_PG_Item_LtoC_DigHoleResult			( int Sockid , int Size , void* Data )
{
	PG_Item_LtoC_DigHoleResult*  PG = (PG_Item_LtoC_DigHoleResult*)Data;
	This->R_DigHoleResult( PG->Result );
}
void NetCli_Item::_PG_Item_LtoC_ApartBodyItemResult	( int Sockid , int Size , void* Data )
{
	PG_Item_LtoC_ApartBodyItemResult*  PG = (PG_Item_LtoC_ApartBodyItemResult*)Data;
	This->R_ApartBodyItemResult( PG->Result , PG->PosSrc , PG->PosDes );
}
void NetCli_Item::_PG_Item_LtoC_TakeOutCardResult		( int Sockid , int Size , void* Data )
{
	PG_Item_LtoC_TakeOutCardResult*  PG = (PG_Item_LtoC_TakeOutCardResult*)Data;
	This->R_TakeOutCardResult( PG->CardObjID , PG->Result );
}
void NetCli_Item::_PG_Item_LtoC_ItemDissolutionResult			( int Sockid , int Size , void* Data )
{
	PG_Item_LtoC_ItemDissolutionResult*  PG = (PG_Item_LtoC_ItemDissolutionResult*)Data;
	This->R_ItemDissolutionResult( PG->Item , PG->Result );
}


void NetCli_Item::_PG_Item_LtoC_EqRefineResult			( int Sockid , int Size , void* Data )
{
	PG_Item_LtoC_EqRefineResult*  PG = (PG_Item_LtoC_EqRefineResult*)Data;
	This->R_EqRefineResult( PG->Item , PG->Result );
}

void NetCli_Item::_PG_Item_LtoC_UsePowerLight			( int Sockid , int Size , void* Data )
{
	PG_Item_LtoC_UsePowerLight*  PG = (PG_Item_LtoC_UsePowerLight*)Data;
	This->R_UsePowerLight( PG->PlayerGUID , PG->PowerLightGUID );
}
void NetCli_Item::_PG_Item_LtoC_UseItemToItem_Result            ( int Sockid , int Size , void* Data )
{
	PG_Item_LtoC_UseItemToItem_Result*  PG = (PG_Item_LtoC_UseItemToItem_Result*)Data;

	This->R_UseItemtoItemResult( PG->Result );

	s_useItemSource.Init();
	s_useItemSource.Init();
}

void NetCli_Item::_PG_Item_LtoC_GetItemResult            ( int Sockid , int Size , void* Data )
{
    PG_Item_LtoC_GetItemResult*  PG = (PG_Item_LtoC_GetItemResult*)Data;
    switch( PG->Result )
    {
    case EM_Item_ItemOK:
        This->R_GetItemOK(  );
        break;
    case EM_Item_GetGroundItemFailed:
        This->R_GetGroundItemFailed( );
        break;
    case EM_Item_GetItemInBufferFailed:
        This->R_GetBufferItemFailed( );
        break;
	case EM_Item_GetItemInBufferOK:
		This->R_GetBufferItemOK( PG->ItemID , PG->Count );
		break;
	case EM_Item_DelItemInBufferOK:
		This->R_DelBufferItemOK( PG->ItemID , PG->Count );
		break;
    }

}
void NetCli_Item::_PG_Item_LtoC_SetKeyItemFlag          ( int Sockid , int Size , void* Data )  
{
	PG_Item_LtoC_SetKeyItemFlag*  PG = (PG_Item_LtoC_SetKeyItemFlag*)Data;
	This->R_SetKeyItemFlag( PG->KeyItemID  , PG->Value );
}
void NetCli_Item::_PG_Item_LtoC_SetTitleFlag          ( int Sockid , int Size , void* Data )  
{
	PG_Item_LtoC_SetTitleFlag*  PG = (PG_Item_LtoC_SetTitleFlag*)Data;
	This->R_SetTitleFlag( PG->TitleObjID , PG->Value );
}
void NetCli_Item::_PG_Item_LtoC_SetCardFlag          ( int Sockid , int Size , void* Data )  
{
	PG_Item_LtoC_SetCardFlag*  PG = (PG_Item_LtoC_SetCardFlag*)Data;
	This->R_SetCardFlag( PG->CardObjID , PG->Value );
}
void NetCli_Item::_PG_Item_LtoC_ClsItemInBuffer          ( int Sockid , int Size , void* Data )  
{
    PG_Item_LtoC_ClsItemInBuffer*  PG = (PG_Item_LtoC_ClsItemInBuffer*)Data;
    This->R_ClsItemInBuffer();
}
void NetCli_Item::_PG_Item_LtoC_NewItemInBuffer          ( int Sockid , int Size , void* Data )  
{
    PG_Item_LtoC_NewItemInBuffer*  PG = (PG_Item_LtoC_NewItemInBuffer*)Data;
    This->R_NewItemInBuffer( PG->Item );
}
void NetCli_Item::_PG_Item_LtoC_DeleteItem               ( int Sockid , int Size , void* Data )  
{
    PG_Item_LtoC_DeleteItem*  PG = (PG_Item_LtoC_DeleteItem*)Data;
    switch( PG->Result )
    {
    case EM_Item_ItemOK:
        {
            switch( PG->Type )
            {
                case Def_Item_Pos_Body:
                    This->R_DeleteItemOK_Body( PG->Item );
                    break;
                case Def_Item_Pos_Bank:
                    This->R_DeleteItemOK_Bank( PG->Item );
                    break;
                case Def_Item_Pos_EQ:
                    This->R_DeleteItemOK_EQ( PG->Item );
                    break;
            }
        }
        break;
    case EM_Item_DeleteItemFailed:
        This->R_DeleteItemFailed();
        break;
    }
}
void NetCli_Item::_PG_Item_LtoC_ExchangeField            ( int Sockid , int Size , void* Data )  
{
    PG_Item_LtoC_ExchangeField*  PG = (PG_Item_LtoC_ExchangeField*)Data;
    if(  PG->Result  == EM_Item_ItemOK )
    {
        if(		PG->Type[0] == Def_Item_Pos_Body 
            &&	PG->Type[1] == Def_Item_Pos_Body )
        {
            This->R_ExchangeFieldOK_Body( PG->ClientID );
			s_exchangeBody.pop();
        }
        else if(PG->Type[0] == Def_Item_Pos_Body 
            &&	PG->Type[1] == Def_Item_Pos_Bank )
        {
            This->R_ExchangeFieldOK_BodyBank( PG->ClientID );
        }
        else if(PG->Type[0] == Def_Item_Pos_Bank
            &&	PG->Type[1] == Def_Item_Pos_Body )
        {
            This->R_ExchangeFieldOK_BodyBank( PG->ClientID );
        }
        else if( PG->Type[0] == Def_Item_Pos_Bank 
            &&	 PG->Type[1] == Def_Item_Pos_Bank )
        {
            This->R_ExchangeFieldOK_Bank( PG->ClientID );
        }
    }
    else if( PG->Result  == EM_Item_ExchangeFieldFailed )
    {
        if(		PG->Type[0] == Def_Item_Pos_Body 
            &&	PG->Type[1] == Def_Item_Pos_Body )
        {
            This->R_ExchangeFieldFailed_Body( PG->ClientID );
			s_exchangeBody.pop();
        }
        else if(PG->Type[0] == Def_Item_Pos_Body 
            &&	PG->Type[1] == Def_Item_Pos_Bank )
        {
            This->R_ExchangeFieldFailed_BodyBank( PG->ClientID );
        }
        else if(PG->Type[0] == Def_Item_Pos_Bank
            &&	PG->Type[1] == Def_Item_Pos_Body )
        {
            This->R_ExchangeFieldFailed_BodyBank( PG->ClientID );
        }
        else if( PG->Type[0] == Def_Item_Pos_Bank 
            &&	 PG->Type[1] == Def_Item_Pos_Bank )
        {
            This->R_ExchangeFieldFailed_Bank( PG->ClientID );
        }
    }

}
void NetCli_Item::_PG_Item_LtoC_BodyBankMoneyExchange    ( int Sockid , int Size , void* Data )  
{
    PG_Item_LtoC_BodyBankMoneyExchange* PG = (PG_Item_LtoC_BodyBankMoneyExchange*)Data;
    if( PG->Result == EM_Item_ItemOK )
        This->R_BodyBankMoneyExchangeOK();
    else
        This->R_BodyBankMoneyExchangeFalse();
}
void NetCli_Item::_PG_Item_LtoC_FixItemInfo              ( int Sockid , int Size , void* Data )   
{
    PG_Item_LtoC_FixItemInfo* PG = (PG_Item_LtoC_FixItemInfo*)Data;
    switch( PG->Type )
    {
    case Def_Item_Pos_Body:
        This->R_FixItemInfo_Body( PG->Item , PG->Pos );
        break;
    case Def_Item_Pos_Bank:
        This->R_FixItemInfo_Bank( PG->Item , PG->Pos );
        break;
    case Def_Item_Pos_EQ:
        This->R_FixItemInfo_EQ( PG->Item , PG->Pos );
        break;
    }


}
void NetCli_Item::_PG_Item_LtoC_FixAllItemInfo           ( int Sockid , int Size , void* Data )  
{
    PG_Item_LtoC_FixAllItemInfo* PG = (PG_Item_LtoC_FixAllItemInfo*)Data;
    This->R_FixAllItemInfo( PG->Body , PG->Bank , PG->EQ );
}
void NetCli_Item::_PG_Item_LtoC_FixMoney                 ( int Sockid , int Size , void* Data )   
{
    PG_Item_LtoC_FixMoney* PG = (PG_Item_LtoC_FixMoney*)Data;
    This->R_FixMoney( PG->BodyMoney , PG->BankMoney , PG->BodyMoney_Account , PG->Money_Bonus );
}
void NetCli_Item::_PG_Item_LtoC_MaxItemCount             ( int Sockid , int Size , void* Data )  
{
    PG_Item_LtoC_MaxItemCount* PG = (PG_Item_LtoC_MaxItemCount*)Data;
    This->R_MaxItemCount( PG->MaxBody , PG->MaxBank );
}
void NetCli_Item::_PG_Item_LtoC_OpenBank                 ( int Sockid , int Size , void* Data )   
{
	PG_Item_LtoC_OpenBank* PG = (PG_Item_LtoC_OpenBank*)Data;
	This->R_OpenBank( PG->GItemID );
}

void NetCli_Item::_PG_Item_LtoC_CloseBank                 ( int Sockid , int Size , void* Data )   
{
	PG_Item_LtoC_CloseBank* PG = (PG_Item_LtoC_CloseBank*)Data;
	This->R_CloseBank( );
}

void NetCli_Item::_PG_Item_LtoC_UseItemResult            ( int Sockid , int Size , void* Data )   
{

//	MemoryBlockStruct* TempData = _Net.GetClientData( );

   PG_Item_LtoC_UseItemResult* PG = (PG_Item_LtoC_UseItemResult*)Data; 
   if( PG->Result )
   {
       This->R_UseItemOK( PG->Type , PG->Pos );
   }
   else
   {
       This->R_UseItemFailed( PG->Type , PG->Pos );
   }
   
}
//------------------------------------------------------------------------------------------------------------------------
void NetCli_Item::_GetGroundItem                  ( int ItemID , int Pos )            
{
    PG_Item_CtoL_GetGroundItem Send;
    Send.ItemID = ItemID;
    Send.Pos = Pos;
    SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Item::_GetBufferItem                  ( ItemFieldStruct Item , int Pos , int Count )
{
    PG_Item_CtoL_GetBufferItem Send;
    Send.Item = Item;
    Send.Pos = Pos;
    Send.Count = Count;
    SendToLocal( sizeof(Send) , &Send );
}

void NetCli_Item::_DelBufferItem                  ( ItemFieldStruct Item )
{
    PG_Item_CtoL_GetBufferItem Send;
    Send.Item = Item;
    Send.Pos = -1;
    SendToLocal( sizeof(Send) , &Send );
}

void NetCli_Item::_DeleteItem_Body                     ( ItemFieldStruct Item , int Pos )
{

    PG_Item_CtoL_DeleteItem Send;
    Send.Item = Item;
    Send.Pos = Pos;
    Send.Type = Def_Item_Pos_Body;

    if( Send.Item.Pos == EM_ItemState_CliDisabled )
        Send.Item.Pos = EM_ItemState_NONE;

    SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Item::_DeleteItem_Bank                     ( ItemFieldStruct Item , int Pos )
{
    PG_Item_CtoL_DeleteItem Send;
    Send.Item = Item;
    Send.Pos = Pos;
    Send.Type = Def_Item_Pos_Bank;

    if( Send.Item.Pos == EM_ItemState_CliDisabled )
        Send.Item.Pos = EM_ItemState_NONE;

    SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Item::_DeleteItem_EQ                     ( ItemFieldStruct Item , int Pos )
{

    PG_Item_CtoL_DeleteItem Send;
    Send.Item = Item;
    Send.Pos = Pos;
    Send.Type = Def_Item_Pos_EQ;

    if( Send.Item.Pos == EM_ItemState_CliDisabled )
        Send.Item.Pos = EM_ItemState_NONE;

    SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Item::_ExchangeField_Body                  ( ItemFieldStruct Item1 , int Pos1 , ItemFieldStruct Item2 , int Pos2 , int ClientID )
{
    PG_Item_CtoL_ExchangeField Send;
    Send.Item[0] = Item1;
    Send.Item[1] = Item2;
    Send.Pos[0] = Pos1;
    Send.Pos[1] = Pos2;
    Send.Type[0] = Def_Item_Pos_Body;
    Send.Type[1] = Def_Item_Pos_Body;
	Send.ClientID = ClientID;

    if( Send.Item[0].Pos == EM_ItemState_CliDisabled )
        Send.Item[0].Pos = EM_ItemState_NONE;
    if( Send.Item[1].Pos == EM_ItemState_CliDisabled )
        Send.Item[1].Pos = EM_ItemState_NONE;
	
	s_exchangeBody.push( Pos1 | (Pos2 << 16) );

    SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Item::_ExchangeField_Bank                  ( ItemFieldStruct Item1 , int Pos1 , ItemFieldStruct Item2 , int Pos2 , int ClientID )
{

    PG_Item_CtoL_ExchangeField Send;
    Send.Item[0] = Item1;
    Send.Item[1] = Item2;
    Send.Pos[0] = Pos1;
    Send.Pos[1] = Pos2;
    Send.Type[0] = Def_Item_Pos_Bank;
    Send.Type[1] = Def_Item_Pos_Bank;
	Send.ClientID = ClientID;

    if( Send.Item[0].Pos == EM_ItemState_CliDisabled )
        Send.Item[0].Pos = EM_ItemState_NONE;
    if( Send.Item[1].Pos == EM_ItemState_CliDisabled )
        Send.Item[1].Pos = EM_ItemState_NONE;

    SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Item::_ExchangeField_BankBody                  ( ItemFieldStruct Item1 , int Pos1 , ItemFieldStruct Item2 , int Pos2 , int ClientID )
{
    PG_Item_CtoL_ExchangeField Send;
    Send.Item[0] = Item1;
    Send.Item[1] = Item2;
    Send.Pos[0] = Pos1;
    Send.Pos[1] = Pos2;
    Send.Type[0] = Def_Item_Pos_Bank;
    Send.Type[1] = Def_Item_Pos_Body;
	Send.ClientID = ClientID;

    if( Send.Item[0].Pos == EM_ItemState_CliDisabled )
        Send.Item[0].Pos = EM_ItemState_NONE;
    if( Send.Item[1].Pos == EM_ItemState_CliDisabled )
        Send.Item[1].Pos = EM_ItemState_NONE;

    SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Item::_ExchangeField_BodyBank                  ( ItemFieldStruct Item1 , int Pos1 , ItemFieldStruct Item2 , int Pos2 , int ClientID )
{

    PG_Item_CtoL_ExchangeField Send;
    Send.Item[0] = Item1;
    Send.Item[1] = Item2;
    Send.Pos[0] = Pos1;
    Send.Pos[1] = Pos2;
    Send.Type[0] = Def_Item_Pos_Body;
    Send.Type[1] = Def_Item_Pos_Bank;
	Send.ClientID = ClientID;
    
    if( Send.Item[0].Pos == EM_ItemState_CliDisabled )
        Send.Item[0].Pos = EM_ItemState_NONE;
    if( Send.Item[1].Pos == EM_ItemState_CliDisabled )
        Send.Item[1].Pos = EM_ItemState_NONE;

    SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Item::_RequestBodyBankMoneyExchange   ( int Count )
{
    PG_Item_CtoL_RequestBodyBankMoneyExchange Send;
    Send.Count = Count;
    SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Item::_RequestBufferInfo              ()
{
    PG_Item_CtoL_RequestBufferInfo Send;
    SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Item::_ApartBodyItemRequest           ( ItemFieldStruct Item , int SrcPos , int DesCount , int DesPos )
{
    PG_Item_CtoL_ApartBodyItemRequest Send;
    Send.Item = Item;
    Send.CountDes = DesCount;
    Send.PosDes = DesPos;
    Send.PosSrc = SrcPos;

    if( Send.Item.Pos == EM_ItemState_CliDisabled )
        Send.Item.Pos = EM_ItemState_NONE;

    SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Item::_RequestFixAllItemInfo          ()
{
    PG_Item_CtoL_RequestFixAllItemInfo Send;
    SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Item::_CloseBank                      ()
{
    PG_Item_CtoL_CloseBank Send;
    SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Item::_UseItem_Body                        ( ItemFieldStruct Item , int Pos , int TargetID , float TargetX , float TargetY , float TargetZ )
{
    PG_Item_CtoL_UseItem Send;
    Send.Item = Item;
    Send.Pos = Pos;
    Send.TargetID = TargetID;
    Send.Type = Def_Item_Pos_Body;
	Send.TargetX = TargetX;
	Send.TargetY = TargetY;
	Send.TargetZ = TargetZ;

    if( Send.Item.Pos == EM_ItemState_CliDisabled )
        Send.Item.Pos = EM_ItemState_NONE;
    
//	_Net.PushClientData( "測試" , 5 );

    SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Item::_UseItem_Bank                        ( ItemFieldStruct Item , int Pos , int TargetID , float TargetX , float TargetY , float TargetZ )
{
    PG_Item_CtoL_UseItem Send;
    Send.Item = Item;
    Send.Pos = Pos;
    Send.TargetID = TargetID;
    Send.Type = Def_Item_Pos_Bank;

    if( Send.Item.Pos == EM_ItemState_CliDisabled )
        Send.Item.Pos = EM_ItemState_NONE;

    SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Item::_UseItem_EQ                        ( ItemFieldStruct Item , int Pos , int TargetID , float TargetX , float TargetY , float TargetZ )
{
    PG_Item_CtoL_UseItem Send;
    Send.Item = Item;
    Send.Pos = Pos;
    Send.TargetID = TargetID;
    Send.Type = Def_Item_Pos_EQ;

    if( Send.Item.Pos == EM_ItemState_CliDisabled )
        Send.Item.Pos = EM_ItemState_NONE;

    SendToLocal( sizeof(Send) , &Send );
}

void NetCli_Item::_UseItemToItem_Body( ItemFieldStruct UseItem , int UseItemPos , ItemFieldStruct TargetItem , int TargetItemPos , char* Password )
{
	PG_Item_CtoL_UseItemToItem_Notify Send;

	s_useItemSource = UseItem;
	s_useItemTarget = TargetItem;

	Send.TargetPosType	= 0;		// 0 身體 1 銀行 2 裝備
	Send.UseItemPos = UseItemPos;
	Send.UseItem	= UseItem;
	Send.TargetItemPos = TargetItemPos;
	Send.TargetItem = TargetItem;
	if( Password != NULL )
		Send.Password = Password;

	SendToLocal( sizeof(Send) , &Send );

}
void NetCli_Item::_UseItemToItem_Bank( ItemFieldStruct UseItem , int UseItemPos , ItemFieldStruct TargetItem , int TargetItemPos , char* Password)
{
	PG_Item_CtoL_UseItemToItem_Notify Send;

	s_useItemSource = UseItem;
	s_useItemTarget = TargetItem;

	Send.TargetPosType	= 1;		// 0 身體 1 銀行 2 裝備
	Send.UseItemPos = UseItemPos;
	Send.UseItem	= UseItem;
	Send.TargetItemPos = TargetItemPos;
	Send.TargetItem = TargetItem;
	if( Password != NULL )
		Send.Password = Password;

	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Item::_UseItemToItem_EQ( ItemFieldStruct UseItem , int UseItemPos , ItemFieldStruct TargetItem , int TargetItemPos , char* Password)
{
	PG_Item_CtoL_UseItemToItem_Notify Send;

	s_useItemSource = UseItem;
	s_useItemTarget = TargetItem;

	Send.TargetPosType	= 2;		// 0 身體 1 銀行 2 裝備
	Send.UseItemPos = UseItemPos;
	Send.UseItem	= UseItem;
	Send.TargetItemPos = TargetItemPos;
	Send.TargetItem = TargetItem;

	if( Password != NULL )
		Send.Password = Password;

	SendToLocal( sizeof(Send) , &Send );
}

void NetCli_Item::_ItemDissolutionRequest			( ItemFieldStruct&	Item , int Pos )
{
	PG_Item_CtoL_ItemDissolutionRequest Send;
	Send.Item = Item;
	Send.Pos = Pos;

	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Item::_TakeOutCardRequest				( int CardObjID )
{
	PG_Item_CtoL_TakeOutCardRequest Send;
	Send.CardObjID = CardObjID;

	SendToLocal( sizeof(Send) , &Send );
}
//要求融合
void NetCli_Item::_EQCombinRequest		( int CombinItem_Ability , int CombinItem_Image , int CheckItemPos )
{
	PG_Item_CtoL_EQCombinRequest Send;
	Send.CombinItemPos[0] = CombinItem_Ability;
	Send.CombinItemPos[1] = CombinItem_Image;
	Send.CheckItemPos	  = CheckItemPos;

	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Item::_EQCombinClose		()
{
	PG_Item_CtoL_EQCombinClose Send;
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Item::SL_AccountBagGetItemRequest	( int BodyPos , int ItemDBID )
{
	PG_Item_CtoL_AccountBagGetItemRequest Send;
	Send.BodyPos = BodyPos;
	Send.ItemDBID = ItemDBID;
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Item::SL_AccountBagClose			( )
{
	PG_Item_CtoL_AccountBagClose Send;
	SendToLocal( sizeof(Send) , &Send );
}
/*
void NetCli_Item::SL_SmeltEqtoMagicStoneRequest	( int EQPos , int SmeltItemPos , int BoxPos )
{
	PG_Item_CtoL_SmeltEqtoMagicStoneRequest Send;
	Send.EQPos = EQPos;
	Send.SmeltItemPos = SmeltItemPos;
	Send.BoxPos	= BoxPos;

	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Item::SL_RefineMagicStoneRequest		( int MagicStonePos[3] , int BoxPos )
{
	PG_Item_CtoL_RefineMagicStoneRequest Send;
	Send.MagicStonePos[0] = MagicStonePos[0];
	Send.MagicStonePos[1] = MagicStonePos[1];
	Send.MagicStonePos[2] = MagicStonePos[2];
	Send.BoxPos	  = BoxPos;

	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Item::SL_CombinEQandMagicStoneRequest( int MagicStonePos[3] , int EQPos , int BoxPos )
{
	PG_Item_CtoL_CombinEQandMagicStoneRequest Send;
	Send.MagicStonePos[0] = MagicStonePos[0];
	Send.MagicStonePos[1] = MagicStonePos[1];
	Send.MagicStonePos[2] = MagicStonePos[2];
	Send.EQPos	  = EQPos;
	Send.BoxPos	  = BoxPos;

	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Item::SL_RuneTransferRequest		( int RunePos[2] , int BoxPos )
{
	PG_Item_CtoL_RuneTransferRequest Send;
	Send.RunePos[0] = RunePos[0];
	Send.RunePos[1] = RunePos[1];
	Send.BoxPos	  = BoxPos;

	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Item::SL_RefineRuneRequest			( int RunePos[5] , int BoxPos )
{
	PG_Item_CtoL_RefineRuneRequest Send;
	memcpy( Send.RunePos , RunePos , sizeof(RunePos) );
	Send.BoxPos	  = BoxPos;

	SendToLocal( sizeof(Send) , &Send );
}
*/
void NetCli_Item::SL_MagicBoxRequest	( )
{
	PG_Item_CtoL_MagicBoxRequest Send;
	SendToLocal( sizeof(Send) , &Send );
}

void NetCli_Item::SL_GamebleItemState( int TableID , int ItemPos , ItemFieldStruct& BodyItem , int LockFlag )
{
	PG_Item_CtoL_GamebleItemState Send;
	Send.TableID = TableID;
	Send.ItemPos = ItemPos;
	Send.BodyItem = BodyItem;
	Send.LockFlag = LockFlag;
	SendToLocal( sizeof(Send) , &Send );
}

void NetCli_Item::SL_GetGamebleItemState		( bool GiveUp )
{
	PG_Item_CtoL_GetGamebleItemState Send;
	Send.GiveUp = GiveUp;
	SendToLocal( sizeof(Send) , &Send );
}

void NetCli_Item::SL_CloseGamebleItemState	( )
{
	PG_Item_CtoL_CloseGamebleItemState Send;
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Item::SL_MagicStoneExchangeResult ( int AbilityID[3] , bool Result )
{
	PG_Item_CtoL_MagicStoneExchangeResult Send;
	memcpy( Send.AbilityID , AbilityID , sizeof(Send.AbilityID) );
	Send.Result = Result;
	SendToLocal( sizeof(Send) , &Send );
}

void NetCli_Item::S_DrawOutRuneRequest		( int BodyPos , int RuneID )
{
	PG_Item_CtoL_DrawOutRuneRequest Send;
	Send.BodyPos	= BodyPos;
	Send.RuneID		= RuneID;
	SendToLocal( sizeof(Send) , &Send );
}

void NetCli_Item::SL_PutCoolClothRequest		( int BodyPos , int ClothPos )
{
	PG_Item_CtoL_PutCoolClothRequest Send;
	Send.BodyPos = BodyPos;
	Send.CoolClothPos = ClothPos;
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Item::SL_DelCoolClothRequest		( int Pos , int ImageObjectID )
{
	PG_Item_CtoL_DelCoolClothRequest Send;
	Send.Pos = Pos;
	Send.ImageObjectID = ImageObjectID;
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Item::SL_SetCoolSuitRequest			( int SuitIndexID , CoolSuitBase& Info )
{
	PG_Item_CtoL_SetCoolSuitRequest Send;
	Send.SuitIndexID = SuitIndexID;
	Send.Info = Info;
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Item::SL_ShowCoolSuitIndexRequest	( int CoolSuitIndexID )
{
	PG_Item_CtoL_ShowCoolSuitIndexRequest Send;
	Send.CoolSuitIndexID = CoolSuitIndexID;
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Item::SL_PutInExBagRequest( ExBagTypeENUM ExBagType , int ItemID , int BodyPos )
{
	PG_Item_CtoL_PutInExBagRequest Send;
	Send.ExBagType = ExBagType;
	Send.ItemID = ItemID;
	Send.BodyPos = BodyPos;
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Item::SL_UseExBagRequest( ExBagTypeENUM	ExBagType ,	int BagPos )
{
	PG_Item_CtoL_UseExBagRequest Send;
	Send.ExBagType = ExBagType;
	Send.BagPos = BagPos;
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Item::SL_SwapCoolClothRequest		( int Pos1 , int Pos2 )
{
	PG_Item_CtoL_SwapCoolClothRequest Send;
	Send.Pos[0] = Pos1;
	Send.Pos[1] = Pos2;
	SendToLocal( sizeof(Send) , &Send );
}

void NetCli_Item::SL_Rare3EqExchangeItemRequest( int Type , vector< int > ItemPos )
{
	PG_Item_CtoL_Rare3EqExchangeItemRequest Send;
	memset( Send.ItemPos , -1 , sizeof(Send.ItemPos) );
	for( unsigned int i = 0 ; i < ItemPos.size() ; i++ )
		Send.ItemPos[i] = ItemPos[i];
	Send.Type = Type;	
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Item::SL_CloseRare3EqExchangeItem( )
{
	PG_Item_CtoL_CloseRare3EqExchangeItem Send;
	SendToLocal( sizeof(Send) , &Send );
}

void NetCli_Item::SL_PopExBagRequest		( ExBagTypeENUM ExBagType , int ExBagPos , int BodyPos )
{
	PG_Item_CtoL_PopExBagRequest Send;
	Send.ExBagType = ExBagType;
	Send.ExBagPos = ExBagPos;
	Send.BodyPos = BodyPos;
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Item::SL_ClsAttribute( int ItemType , int ItemPos  , int AttributeID )
{
	PG_Item_CtoL_ClsAttribute Send;
	Send.Type = ItemType;
	Send.Pos = ItemPos;
	Send.AttributeID = AttributeID;
	SendToLocal( sizeof(Send) , &Send );
}
//魔法盒 屬性萃取
void NetCli_Item::SL_MagicBoxAttrDrawOff( )
{
	PG_Item_CtoL_MagicBoxAttrDrawOff Send;

	SendToLocal( sizeof(Send) , &Send );
}
