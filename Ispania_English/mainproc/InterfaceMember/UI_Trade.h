#pragma once
#include "../GameMain.h"
#include "MainMenu.h"

#define Register_UI_Trade    \
    g_pLuaScript->RegisterFunc( "OnLoadTradeMenu"       , void ( const char* )  , CUI_Trade::OnLoadTradeMenu );\
    g_pLuaScript->RegisterFunc( "UpDateTradeItem"       , void ( const char*    , bool ), CUI_Trade::UpDateTradeItem );\
    g_pLuaScript->RegisterFunc( "GetSelectTradeOKFlage" , bool ( bool )         , CUI_Trade::GetSelectTradeOKFlage );\
    g_pLuaScript->RegisterFunc( "GetTradeOKFlage"       , bool ( bool )         , CUI_Trade::GetTradeOKFlage );\
    g_pLuaScript->RegisterFunc( "SelectTradeOK"         , void (  )             , CUI_Trade::SelectTradeOK );\
    g_pLuaScript->RegisterFunc( "CancelTrade"           , void (  )             , CUI_Trade::CancelTrade );\
    g_pLuaScript->RegisterFunc( "OnLoadTradeButton"     , void ( const char* )  , CUI_Trade::OnLoadTradeButton );\
    g_pLuaScript->RegisterFunc( "Trade"                 , void (  )             , CUI_Trade::Trade );\
    g_pLuaScript->RegisterFunc( "SetTradeMoney"         , void (  )             , CUI_Trade::SetTradeMoney );\
    g_pLuaScript->RegisterFunc( "GetMyTradeMoney"       , int (  )              , CUI_Trade::GetMyTradeMoney );\
    g_pLuaScript->RegisterFunc( "GetTargetTradeMoney"   , int (  )              , CUI_Trade::GetTargetTradeMoney );

// **********************************************************************************************
// Globals
class	CUI_Trade;

extern	CUI_Trade							*g_pUI_Trade;

// **********************************************************************************************
class CUI_Trade
{

    static	ItemFieldStruct	    _MyTradeItem[ _MAX_TRADE_ITEM_COUNT_ ];
    static	ItemFieldStruct	    _TargetTradeItem[ _MAX_TRADE_ITEM_COUNT_ ];

    static  bool                _IsMyTradeSelectOK;
    static  bool                _IsTargetTradeSelectOK;

    static  bool                _IsMyTradeOK;
  //  static  bool                _IsTargetTradeOK;

    static  CFrame*             _Win;

    static  CFrame*             _MyTradeSelectOK;
    static  CFrame*             _TargetTradeSelectOK;

    static  CFrame*             _MyTradeOK;
//    static  CLayout*            _TargetTradeOK;

    static  int                 _TradeID;

    static  int                 _Pre_PutItemPos;
    static  ItemFieldStruct     _Pre_PutItem;
    static  int                 _Pre_TargetID;

    static  int                 _MyTradeMoney;
    static  int                 _TargetTradeMoney;
public:
	CUI_Trade();
	virtual ~CUI_Trade();

	static	void		Initial();
	static	void		Release();	

    static  bool        IsVisible() { return (_Win !=NULL && _Win->IsVisible() ); } ;

    //更新自己的交易物品
    static  void        UpDateMyTardeItemInfo( );

    //設定對方的交易物品
    static  void        SetTargetTradeItem( int Pos , ItemFieldStruct& Item );
    
    //開啟交易視窗
    static  void        OpenTradeMenu( int TradeID );
    static  void        CloseTradeMenu( );

    static  void        IsMyTradeSelectOK( bool Flag );
    static  void        IsTargetTradeSelectOK( bool Flag );
    static  void        IsMyTradeOK( bool Flag );
    static  void        UpdateButtonState();

    static  void        SetMyTradeMoney( int Value );
    static  void        SetTargetTradeMoney( int Value );


    //設定金錢的 CallBack
    static void         SetMoney_CB( void* Obj , int Value );
    //----------------------------------------------------------------------------------
    // ItemPos = -1 表示沒有要求放物品
    static  bool        TradeRequest( int TargetID , int ItemPos = -1  );
    //----------------------------------------------------------------------------------
    //LUA
    static  void        OnLoadTradeMenu( const char* Name );

    static  void        OnLoadTradeButton( const char* Name );

    //更新交易物品 
    static  void        UpDateTradeItem( const char* Name , bool IsMe );
    //取得是否交易OK
    static  bool        GetSelectTradeOKFlage( bool IsMe );
    static  bool        GetTradeOKFlage( bool IsMe );
    
    //案下確定交易物品項目(非交易)
    static  void        SelectTradeOK( );
    //交易
    static  void        Trade( );

    //取消交易
    static  void        CancelTrade( );

    //設定交易金錢
    static  void        SetTradeMoney( );

    static  int         GetMyTradeMoney();

    static  int         GetTargetTradeMoney();

};



