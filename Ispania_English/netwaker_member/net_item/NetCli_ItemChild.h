#pragma once

#include "NetCli_Item.h"

struct NetAccountBagItem
{
	int DBID;						// 物品編號
	ItemFieldStruct item;			// 物品資料
};

class NetCli_ItemChild : public NetCli_Item
{
public:
	static bool _Init();
	static bool _Release();
	static void	_Update(float elapsed);
	static int	AssignBuffrtToBag					( );
	static void	ReplaceMoney						(string description, string money);


	virtual void R_GetItemOK                        ( );   
	virtual void R_GetGroundItemFailed              ( );
	virtual void R_GetBufferItemFailed              ( );
	virtual void R_GetBufferItemOK					( int ItemID , int Count );		
	virtual void R_DelBufferItemOK					( int ItemID , int Count );		


	virtual void R_ClsItemInBuffer                  ( );   
	virtual void R_NewItemInBuffer                  ( ItemFieldStruct& Item );   

	virtual void R_DeleteItemOK_Body                ( ItemFieldStruct& Item );   
	virtual void R_DeleteItemOK_Bank                ( ItemFieldStruct& Item );  
	virtual void R_DeleteItemOK_EQ                  ( ItemFieldStruct& Item );   
	virtual void R_DeleteItemFailed                 ( );
	virtual	void DeleteItemMessage					( ItemFieldStruct& Item );

	virtual void R_ExchangeFieldOK_Body             ( int ClientID );   
	virtual void R_ExchangeFieldOK_Bank             ( int ClientID );   
	virtual void R_ExchangeFieldOK_BodyBank         ( int ClientID );   
	virtual void R_ExchangeFieldFailed_Body         ( int ClientID );   
	virtual void R_ExchangeFieldFailed_Bank         ( int ClientID );   
	virtual void R_ExchangeFieldFailed_BodyBank     ( int ClientID );   

	virtual void R_BodyBankMoneyExchangeOK          ( );   
	virtual void R_BodyBankMoneyExchangeFalse       ( );   
	virtual void R_FixItemInfo_Body                 ( ItemFieldStruct& Item , int Pos );    
	virtual void R_FixItemInfo_Bank                 ( ItemFieldStruct& Item , int Pos );    
	virtual void R_FixItemInfo_EQ                   ( ItemFieldStruct& Item , int Pos );    

	virtual void R_FixAllItemInfo                   ( BodyItemStruct& Body , BankItemStruct& Bank , EQStruct& EQ );   
	virtual void R_FixMoney                         ( int BodyMoney , int BankMoney , int BodyMoney_Account , int Money_Bonus );    
	virtual void R_MaxItemCount                     ( int MaxBodyItem , int MaxBankItem );   
	virtual void R_OpenBank                         ( int GItemID );    
	virtual void R_CloseBank                        ( );    
	virtual void R_UseItemOK                        ( int Type , int Pos );    
	virtual void R_UseItemFailed                    ( int Type , int Pos );
	virtual void R_SetKeyItemFlag					( int KeyItemID , bool Value );
	virtual void R_SetTitleFlag						( int TitleID , bool Value );
	virtual void R_SetCardFlag						( int TitleID , bool Value );
	virtual void R_UseItemtoItemResult				( bool Result );
	virtual void R_UsePowerLight					( int PlayerGUID , int PowerLightGUID );
	virtual void R_EqRefineResult					( ItemFieldStruct&	Item , EqRefineResultENUM Result );
	virtual void R_ItemDissolutionResult			( ItemFieldStruct& Item , ItemDissolutionResultENUM Result );
	virtual void R_TakeOutCardResult				( int CardObjID , bool Result );
	virtual void R_ApartBodyItemResult				( bool	Result , short PosSrc , short PosDes );
	virtual void R_DigHoleResult					( bool Result );

	virtual void R_EQCombinOpen						( int TargetNPC , int ItemPos , int Type );
	virtual void R_EQCombinClose					( );
	virtual void R_EQCombinResult					( bool Result );

	virtual void R_ItemTimeout						( ItemTimeoutPosENUM Pos , ItemFieldStruct& Item );
	virtual void R_UseEq							( int PlayerGUID , int EqID );
	//////////////////////////////////////////////////////////////////////////
	//Account Bag
	//////////////////////////////////////////////////////////////////////////
	virtual void R_OpenAccountBag			( int TargetNPC , int MaxCount , int ItemDBID[20] , ItemFieldStruct Item[20] );
	virtual void R_AccountBagGetItemResult	( int ItemDBID , int BodyPos , AccountBagGetItemResultENUM Result );
	static	void ClearAccountBag			( );
	static	int  GetAccountBagNumItems		( )		{ return s_accountBagItems; }
	static	int	 GetAccountBagNPCID			( )		{ return s_npcID; }

	static	NetAccountBagItem*	GetAccountBagItemInfo(int index);
	//////////////////////////////////////////////////////////////////////////
	//魔法石熔出與合成
	//////////////////////////////////////////////////////////////////////////
	/*
	virtual void R_SmeltEqtoMagicStoneResult	( int EQPos , int SmeltItemPos , int BoxPos , bool Result );  
	virtual void R_RefineMagicStoneResult		( int MagicStonePos[3] , int BoxPos , bool Result );
	virtual void R_CombinEQandMagicStoneResult	( int MagicStonePos[3] , int EQPos , int BoxPos , bool Result );  
	virtual void R_RuneTransferResult			( int RunePos[2] , int BoxPos , bool Result );  
	virtual void R_RefineRuneResult				( int RunePos[5] , int BoxPos , bool Result );  
	*/
	virtual void R_MagicBoxResult( ItemFieldStruct& Item , bool Result );
	//////////////////////////////////////////////////////////////////////////	
	virtual void R_GamebleItemStateResult( int TableID , int StatePos , int StateID , GamebleItemStateResultENUM Result );
	virtual void R_GetGamebleItemStateResult( bool Result );
	virtual void R_OpenGamebleItemState( int TargetID ) ;
	virtual void R_CloseGamebleItemState( );
	//////////////////////////////////////////////////////////////////////////
	//物件被劇情刪除的訊息
	virtual void R_DelItemMsg( ItemFieldStruct& Item );
	//////////////////////////////////////////////////////////////////////////
	virtual void R_OpenMagicStoneExchange	( int MagicStoneID , int Lv , int Money[2] );
	virtual void R_MagicStoneExchangeResult	( ItemFieldStruct& Item , MagicStoneExchangeResultENUM Result , int Money[2] );
	//////////////////////////////////////////////////////////////////////////
	virtual void R_DrawOutRuneResult	( DrawOutRuneResultENUM Result );
	//////////////////////////////////////////////////////////////////////////
	//魔法衣櫥
	//////////////////////////////////////////////////////////////////////////
	static void PutCoolClothRequest			( int BodyPos, int ClothPos, int ObjID );
	virtual void R_PutCoolClothResult		( PutCoolClothResultENUM Result );

	static void DelCoolClothRequest			( int Pos , int ImageObjectID );
	virtual void R_DelCoolClothResult		( bool Result );

	static void SetCoolSuitRequest			( int SuitIndexID , CoolSuitBase& Info );
	virtual void R_SetCoolSuitResult		( bool Result );

 	static void ShowCoolSuitIndexRequest	( int CoolSuitIndexID );
	virtual void R_ShowCoolSuitIndexResult	( bool Result );

	static void SwapCoolClothRequest( int Pos1 , int Pos2 );
	virtual void R_SwapCoolClothResult		( bool Result );
	//////////////////////////////////////////////////////////////////////////
	//特殊背包
	//////////////////////////////////////////////////////////////////////////
	virtual void R_PutInExBagResult			( PutInExBagResultENUM Result );
	virtual void R_PopExBagResult			( PopExBagResultENUM Result );
	virtual void R_UseExBagResult			( bool Result );
	//////////////////////////////////////////////////////////////////////////
	//稀有度3的裝備兌換
	//////////////////////////////////////////////////////////////////////////
	virtual void RL_OpenRare3EqExchangeItem	( int TargetNPCID );
	virtual void RL_Rare3EqExchangeItemResult( Rare3EqExchangeItemRequestENUM Result );
	//////////////////////////////////////////////////////////////////////////
	virtual void R_ClsAttributeResult( bool Result );
	virtual void R_MagicBoxAttrDrawOffResult( bool Result );
public:
	static	bool				s_isLockQueue;
	static	float				s_updateTime;

	// Account Bag
	static	int							s_npcID;					// NPC編號
	static	int							s_accountBagItems;			// 最大最數
	static	vector<NetAccountBagItem>	s_accountBagList;			// 清單列表
};

