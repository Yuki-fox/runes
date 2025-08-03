#pragma once

#include <queue>
#include "PG/PG_Item.h"
#include "../../MainProc/NetGlobal.h"

class NetCli_Item : public CNetGlobal
{
protected:
    //-------------------------------------------------------------------
    static NetCli_Item* This;
    static bool _Init();
    static bool _Release();
    //-------------------------------------------------------------------
    static void _PG_Item_LtoC_GetItemResult        	( int Sockid , int Size , void* Data );   
    static void _PG_Item_LtoC_ClsItemInBuffer      	( int Sockid , int Size , void* Data );   
    static void _PG_Item_LtoC_NewItemInBuffer      	( int Sockid , int Size , void* Data );   
    static void _PG_Item_LtoC_DeleteItem           	( int Sockid , int Size , void* Data );   
    static void _PG_Item_LtoC_ExchangeField        	( int Sockid , int Size , void* Data );   
    static void _PG_Item_LtoC_BodyBankMoneyExchange	( int Sockid , int Size , void* Data );   
    static void _PG_Item_LtoC_FixItemInfo          	( int Sockid , int Size , void* Data );    
    static void _PG_Item_LtoC_FixAllItemInfo       	( int Sockid , int Size , void* Data );   
    static void _PG_Item_LtoC_FixMoney             	( int Sockid , int Size , void* Data );    
    static void _PG_Item_LtoC_MaxItemCount         	( int Sockid , int Size , void* Data );   
    static void _PG_Item_LtoC_OpenBank             	( int Sockid , int Size , void* Data );    
    static void _PG_Item_LtoC_CloseBank            	( int Sockid , int Size , void* Data );    
    static void _PG_Item_LtoC_UseItemResult        	( int Sockid , int Size , void* Data );    
	static void _PG_Item_LtoC_SetKeyItemFlag       	( int Sockid , int Size , void* Data );    
	static void _PG_Item_LtoC_SetTitleFlag       	( int Sockid , int Size , void* Data );    
	static void _PG_Item_LtoC_SetCardFlag       	( int Sockid , int Size , void* Data );    

	static void _PG_Item_LtoC_UseItemToItem_Result	( int Sockid , int Size , void* Data );    
	static void _PG_Item_LtoC_UsePowerLight			( int Sockid , int Size , void* Data );    
	static void _PG_Item_LtoC_EqRefineResult		( int Sockid , int Size , void* Data );    
	static void _PG_Item_LtoC_ItemDissolutionResult ( int Sockid , int Size , void* Data );   
	static void _PG_Item_LtoC_TakeOutCardResult		( int Sockid , int Size , void* Data );   
	static void _PG_Item_LtoC_ApartBodyItemResult	( int Sockid , int Size , void* Data );   
	static void _PG_Item_LtoC_DigHoleResult			( int Sockid , int Size , void* Data );   
	static void _PG_Item_LtoC_EQCombinOpen			( int Sockid , int Size , void* Data );   
	static void _PG_Item_LtoC_EQCombinClose			( int Sockid , int Size , void* Data );   
	static void _PG_Item_LtoC_EQCombinResult		( int Sockid , int Size , void* Data );   

	static void _PG_Item_LtoC_OpenAccountBag			( int Sockid , int Size , void* Data );
	static void _PG_Item_LtoC_AccountBagGetItemResult	( int Sockid , int Size , void* Data );
	static void _PG_Item_LtoC_ItemTimeout				( int Sockid , int Size , void* Data );

	static void _PG_Item_LtoC_MagicBoxResult			( int Sockid , int Size , void* Data );  
	static void _PG_Item_LtoC_UseEq						( int Sockid , int Size , void* Data );  

	static void _PG_Item_LtoC_GamebleItemStateResult	( int Sockid , int Size , void* Data );
	static void _PG_Item_LtoC_GetGamebleItemStateResult	( int Sockid , int Size , void* Data );
	static void _PG_Item_LtoC_DelItemMsg				( int Sockid , int Size , void* Data );

	static void _PG_Item_LtoC_OpenGamebleItemState		( int Sockid , int Size , void* Data );
	static void _PG_Item_LtoC_CloseGamebleItemState		( int Sockid , int Size , void* Data );

	static void _PG_Item_LtoC_OpenMagicStoneExchange	( int Sockid , int Size , void* Data );
	static void _PG_Item_LtoC_MagicStoneExchangeResult	( int Sockid , int Size , void* Data );
	static void _PG_Item_LtoC_DrawOutRuneResult			( int Sockid , int Size , void* Data );

	static void _PG_Item_LtoC_PutCoolClothResult		( int Sockid , int Size , void* Data );
	static void _PG_Item_LtoC_DelCoolClothResult		( int Sockid , int Size , void* Data );
	static void _PG_Item_LtoC_SetCoolSuitResult			( int Sockid , int Size , void* Data );
	static void _PG_Item_LtoC_ShowCoolSuitIndexResult	( int Sockid , int Size , void* Data );
	static void _PG_Item_LtoC_SwapCoolClothResult		( int Sockid , int Size , void* Data );

	static void _PG_Item_LtoC_PutInExBagResult			( int Sockid , int Size , void* Data );
	static void _PG_Item_LtoC_PopExBagResult			( int Sockid , int Size , void* Data );
	static void _PG_Item_LtoC_UseExBagResult			( int Sockid , int Size , void* Data );
	

	static void _PG_Item_LtoC_OpenRare3EqExchangeItem	( int Sockid , int Size , void* Data );
	static void _PG_Item_LtoC_Rare3EqExchangeItemResult	( int Sockid , int Size , void* Data );
	static void _PG_Item_LtoC_ClsAttributeResult		( int Sockid , int Size , void* Data );
	static void _PG_Item_LtoC_MagicBoxAttrDrawOffResult	( int Sockid , int Size , void* Data );
public:    
    NetCli_Item() { This = this; }
	static ItemFieldStruct s_useItemTarget;			// 使用物暫存值
	static ItemFieldStruct s_useItemSource;			// 使用物暫存值
	static queue<int> s_exchangeBody;				// 暫存物品交換

    static void _GetGroundItem                  ( int ItemID , int Pos = -1 );             
    static void _GetBufferItem                  ( ItemFieldStruct Item , int Pos , int Count );
    static void _DelBufferItem                  ( ItemFieldStruct Item );
    static void _DeleteItem_Body                ( ItemFieldStruct Item , int Pos );
    static void _DeleteItem_Bank                ( ItemFieldStruct Item , int Pos );
    static void _DeleteItem_EQ                  ( ItemFieldStruct Item , int Pos );

    static void _ExchangeField_Body             ( ItemFieldStruct Item1 , int Pos1 , ItemFieldStruct Item2 , int Pos2 , int ClientID = 0);
    static void _ExchangeField_Bank             ( ItemFieldStruct Item1 , int Pos1 , ItemFieldStruct Item2 , int Pos2 , int ClientID = 0);
    static void _ExchangeField_BankBody         ( ItemFieldStruct Item1 , int Pos1 , ItemFieldStruct Item2 , int Pos2 , int ClientID = 0);
    static void _ExchangeField_BodyBank         ( ItemFieldStruct Item1 , int Pos1 , ItemFieldStruct Item2 , int Pos2 , int ClientID = 0);

    static void _RequestBodyBankMoneyExchange   ( int Count );
    static void _RequestBufferInfo              ( );
    static void _ApartBodyItemRequest           ( ItemFieldStruct Item , int SrcPos , int DesCount , int DesPos);
    static void _RequestFixAllItemInfo          ( );
    static void _CloseBank                      ();
    static void _UseItem_Body                   ( ItemFieldStruct Item , int Pos , int TargetID , float TargetX = 0 , float TargetY = 0 , float TargetZ = 0 );
    static void _UseItem_Bank                   ( ItemFieldStruct Item , int Pos , int TargetID , float TargetX = 0 , float TargetY = 0 , float TargetZ = 0 );
    static void _UseItem_EQ						( ItemFieldStruct Item , int Pos , int TargetID , float TargetX = 0 , float TargetY = 0 , float TargetZ = 0 );

	static void _UseItemToItem_Body             ( ItemFieldStruct UseItem , int UseItemPos , ItemFieldStruct TargetItem , int TargetItemPos , char* Password = NULL );
	static void _UseItemToItem_Bank             ( ItemFieldStruct UseItem , int UseItemPos , ItemFieldStruct TargetItem , int TargetItemPos , char* Password = NULL );
	static void _UseItemToItem_EQ				( ItemFieldStruct UseItem , int UseItemPos , ItemFieldStruct TargetItem , int TargetItemPos , char* Password = NULL );

	static void _ItemDissolutionRequest			( ItemFieldStruct&	Item , int Pos );

	static void _TakeOutCardRequest				( int CardObjID );


	//要求融合
	static void _EQCombinRequest				( int CombinItem_Ability , int CombinItem_Image , int CheckItemPos );
	static void _EQCombinClose					( );


    virtual void R_GetItemOK                ( ) = 0;   
    virtual void R_GetGroundItemFailed      ( ) = 0;
    virtual void R_GetBufferItemFailed      ( ) = 0;
	virtual void R_GetBufferItemOK			( int ItemID , int Count ) = 0;
	virtual void R_DelBufferItemOK			( int ItemID , int Count ) = 0;


    virtual void R_ClsItemInBuffer          ( ) = 0;   
    virtual void R_NewItemInBuffer          ( ItemFieldStruct& Item ) = 0;   

    virtual void R_DeleteItemOK_Body        ( ItemFieldStruct& Item ) = 0;   
    virtual void R_DeleteItemOK_Bank        ( ItemFieldStruct& Item ) = 0;  
    virtual void R_DeleteItemOK_EQ          ( ItemFieldStruct& Item ) = 0;   
    virtual void R_DeleteItemFailed         ( ) = 0;   

    
    virtual void R_ExchangeFieldOK_Body          ( int ClientID ) = 0;   
    virtual void R_ExchangeFieldOK_Bank          ( int ClientID ) = 0;   
    virtual void R_ExchangeFieldOK_BodyBank      ( int ClientID ) = 0;   
    virtual void R_ExchangeFieldFailed_Body      ( int ClientID ) = 0;   
    virtual void R_ExchangeFieldFailed_Bank      ( int ClientID ) = 0;   
    virtual void R_ExchangeFieldFailed_BodyBank  ( int ClientID ) = 0;   


    virtual void R_BodyBankMoneyExchangeOK    ( ) = 0;   
    virtual void R_BodyBankMoneyExchangeFalse ( ) = 0;   
    virtual void R_FixItemInfo_Body           ( ItemFieldStruct& Item , int Pos ) = 0;    
    virtual void R_FixItemInfo_Bank           ( ItemFieldStruct& Item , int Pos ) = 0;    
    virtual void R_FixItemInfo_EQ             ( ItemFieldStruct& Item , int Pos ) = 0;    

    virtual void R_FixAllItemInfo           ( BodyItemStruct& Body , BankItemStruct& Bank , EQStruct& EQ ) = 0;   
    virtual void R_FixMoney                 ( int BodyMoney , int BankMoney , int BodyMoney_Account , int Money_Bonus ) = 0;    
    virtual void R_MaxItemCount             ( int MaxBodyItem , int MaxBankItem ) = 0;   
    virtual void R_OpenBank                 ( int GItemID ) = 0;    
    virtual void R_CloseBank                ( ) = 0;    
    virtual void R_UseItemOK                ( int Type , int Pos ) = 0;    
    virtual void R_UseItemFailed            ( int Type , int Pos ) = 0;    

	virtual void R_SetKeyItemFlag			( int KeyItemID , bool Value ) = 0;
	virtual void R_SetTitleFlag				( int TitleID , bool Value ) = 0;
	virtual void R_SetCardFlag				( int TitleID , bool Value ) = 0;

	virtual void R_UseEq					( int PlayerGUID , int EqID ) = 0;
	virtual void R_UseItemtoItemResult		( bool Result ) = 0;
	virtual void R_UsePowerLight			( int PlayerGUID , int PowerLightGUID ) = 0;
	virtual void R_EqRefineResult			( ItemFieldStruct&	Item , EqRefineResultENUM Result ) = 0;
	virtual void R_ItemDissolutionResult	( ItemFieldStruct& Item , ItemDissolutionResultENUM Result ) = 0 ;
	virtual void R_TakeOutCardResult		( int CardObjID , bool Result ) = 0;
	virtual void R_ApartBodyItemResult		( bool Result , short PosSrc , short PosDes ) = 0;
	virtual void R_DigHoleResult			( bool Result ) = 0;

	virtual void R_EQCombinOpen				( int TargetNPC , int ItemPos , int Type ) = 0;
	virtual void R_EQCombinClose			( ) = 0;
	virtual void R_EQCombinResult			( bool Result ) = 0;
	virtual void R_ItemTimeout				( ItemTimeoutPosENUM Pos , ItemFieldStruct& Item ) = 0;

	//////////////////////////////////////////////////////////////////////////
	// Account Bag
	//////////////////////////////////////////////////////////////////////////
	static void SL_AccountBagGetItemRequest	( int BodyPos , int ItemDBID );
	static void SL_AccountBagClose			( );
	virtual void R_OpenAccountBag			( int TargetNPC , int MaxCount , int ItemDBID[20] , ItemFieldStruct Item[20] ) = 0;
	virtual void R_AccountBagGetItemResult	( int ItemDBID , int BodyPos , AccountBagGetItemResultENUM Result ) = 0;

	//////////////////////////////////////////////////////////////////////////
	//魔法石熔出與合成
	//////////////////////////////////////////////////////////////////////////
	/*
	static void SL_SmeltEqtoMagicStoneRequest	( int EQPos , int SmeltItemPos , int BoxPos );
	static void SL_RefineMagicStoneRequest		( int MagicStonePos[3] , int BoxPos );
	static void SL_CombinEQandMagicStoneRequest	( int MagicStonePos[3] , int EQPos , int BoxPos );
	static void SL_RuneTransferRequest			( int RunePos[2] , int BoxPos );
	static void SL_RefineRuneRequest			( int RunePos[5] , int BoxPos );

	virtual void R_SmeltEqtoMagicStoneResult	( int EQPos , int SmeltItemPos , int BoxPos , bool Result ) = 0;  
	virtual void R_RefineMagicStoneResult		( int MagicStonePos[3] , int BoxPos , bool Result ) = 0;
	virtual void R_CombinEQandMagicStoneResult	( int MagicStonePos[3] , int EQPos , int BoxPos , bool Result ) = 0;  
	virtual void R_RuneTransferResult			( int RunePos[2] , int BoxPos , bool Result ) = 0;  
	virtual void R_RefineRuneResult				( int RunePos[5] , int BoxPos , bool Result ) = 0;  
	*/
	static void SL_MagicBoxRequest	( );
	virtual void R_MagicBoxResult( ItemFieldStruct& Item , bool Result ) = 0;
	//////////////////////////////////////////////////////////////////////////
	static void SL_GamebleItemState			( int TableID , int ItemPos , ItemFieldStruct& BodyItem , int LockFlag );
	static void SL_GetGamebleItemState		( bool GiveUp );
	static void SL_CloseGamebleItemState	( );
	virtual void R_GamebleItemStateResult	( int TableID , int StatePos , int StateID , GamebleItemStateResultENUM Result ) = 0;
	virtual void R_GetGamebleItemStateResult( bool Result ) = 0;
	virtual void R_OpenGamebleItemState		( int TargetID ) = 0;
	virtual void R_CloseGamebleItemState	( ) = 0;

	//////////////////////////////////////////////////////////////////////////
	//物件被劇情刪除的訊息
	virtual void R_DelItemMsg( ItemFieldStruct& Item ) = 0;
	//////////////////////////////////////////////////////////////////////////
	//通知開啟換屬性的界面
	virtual void R_OpenMagicStoneExchange	( int MagicStoneID , int Lv , int Money[2] ) = 0;
		//回應Client 所選的屬性    Result = true 確定 , false 取消
		static void SL_MagicStoneExchangeResult ( int AbilityID[3] , bool Result );

			virtual void R_MagicStoneExchangeResult	( ItemFieldStruct& Item , MagicStoneExchangeResultENUM Result , int Money[2] ) = 0;
	//////////////////////////////////////////////////////////////////////////
	static void S_DrawOutRuneRequest		( int BodyPos , int RuneID );
		virtual void R_DrawOutRuneResult	( DrawOutRuneResultENUM Result ) = 0;
	//////////////////////////////////////////////////////////////////////////
	//魔法衣櫥
	//////////////////////////////////////////////////////////////////////////
	static void SL_PutCoolClothRequest		( int BodyPos , int ClothPos );	
		virtual void R_PutCoolClothResult		( PutCoolClothResultENUM Result ) = 0;

	static void SL_DelCoolClothRequest		( int Pos , int ImageObjectID );	
		virtual void R_DelCoolClothResult		( bool Result ) = 0;

	static void SL_SetCoolSuitRequest		( int SuitIndexID , CoolSuitBase& Info );	
		virtual void R_SetCoolSuitResult		( bool Result ) = 0;

	static void SL_ShowCoolSuitIndexRequest	( int CoolSuitIndexID );
		virtual void R_ShowCoolSuitIndexResult	( bool Result ) = 0;

	static void SL_SwapCoolClothRequest		( int Pos1 , int Pos2 );
		virtual void R_SwapCoolClothResult		( bool Result ) = 0;
	//////////////////////////////////////////////////////////////////////////
	//特殊背包
	//////////////////////////////////////////////////////////////////////////
	static void SL_PutInExBagRequest		( ExBagTypeENUM ExBagType , int ItemID , int BodyPos );
		virtual void R_PutInExBagResult			( PutInExBagResultENUM Result ) = 0;

	static void SL_PopExBagRequest		( ExBagTypeENUM ExBagType , int ExBagPos , int BodyPos );
		virtual void R_PopExBagResult			( PopExBagResultENUM Result ) = 0;


	static void SL_UseExBagRequest			( ExBagTypeENUM	ExBagType ,	int BagPos );
		virtual void R_UseExBagResult			( bool Result ) = 0;
	//////////////////////////////////////////////////////////////////////////
	//稀有度3的裝備兌換
	//////////////////////////////////////////////////////////////////////////
	virtual void RL_OpenRare3EqExchangeItem	( int TargetNPCID ) = 0;
		static void SL_Rare3EqExchangeItemRequest(  int Type , vector< int > ItemPos );
			virtual void RL_Rare3EqExchangeItemResult( Rare3EqExchangeItemRequestENUM Result ) = 0;
	static void SL_CloseRare3EqExchangeItem( );
	//////////////////////////////////////////////////////////////////////////
	//清除屬性
	static void SL_ClsAttribute( int ItemType , int ItemPos  , int AttributeID );
		virtual void R_ClsAttributeResult( bool Result ) = 0;

	//魔法盒 屬性萃取
	static void SL_MagicBoxAttrDrawOff( );
		virtual void R_MagicBoxAttrDrawOffResult( bool Result ) = 0;
};

