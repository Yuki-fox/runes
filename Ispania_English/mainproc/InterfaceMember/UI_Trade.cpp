#include "../GameMain.h"
#include "UI_Trade.h"
#include "UI_Calculate.h"

// **********************************************************************************************
// Globals
CUI_Trade*			g_pUI_Trade = NULL;

ItemFieldStruct	    CUI_Trade::_MyTradeItem[ _MAX_TRADE_ITEM_COUNT_ ];
ItemFieldStruct	    CUI_Trade::_TargetTradeItem[ _MAX_TRADE_ITEM_COUNT_ ];

bool                CUI_Trade::_IsMyTradeOK;
bool                CUI_Trade::_IsMyTradeSelectOK;
bool                CUI_Trade::_IsTargetTradeSelectOK;

CFrame*             CUI_Trade::_MyTradeSelectOK = NULL;
CFrame*             CUI_Trade::_TargetTradeSelectOK = NULL;
CFrame*             CUI_Trade::_MyTradeOK = NULL;

CFrame*             CUI_Trade::_Win = NULL;
int                 CUI_Trade::_TradeID;

int                 CUI_Trade::_Pre_PutItemPos = -1;
ItemFieldStruct     CUI_Trade::_Pre_PutItem;
int                 CUI_Trade::_Pre_TargetID = -1;
int                 CUI_Trade::_MyTradeMoney = -1; 
int                 CUI_Trade::_TargetTradeMoney =-1;
// **********************************************************************************************
// --------------------------------------------------------------------------------------
void CUI_Trade::Initial()
{
	if ( g_pUI_Trade == NULL )
	{
		g_pUI_Trade = new CUI_Trade;
    }
}

// --------------------------------------------------------------------------------------
void CUI_Trade::Release()
{
	SAFE_DELETE( g_pUI_Trade );	
}

// --------------------------------------------------------------------------------------
CUI_Trade::CUI_Trade()
{

}

// --------------------------------------------------------------------------------------
CUI_Trade::~CUI_Trade()
{
}
// --------------------------------------------------------------------------------------
//更新自己的交易物品
void    CUI_Trade::UpDateMyTardeItemInfo( )
{
    RoleDataEx* Role    = g_pGameMain->RoleData();
    for( int i = 0 ; i < _MAX_BODY_COUNT_ ; i++ )
    {
        ItemFieldStruct& Item = Role->BaseData.Body.Item[i];
        if( Item.Pos >= EM_TRADE_ITEM1 && Item.Pos <= EM_TRADE_ITEM25 )
        {
            _MyTradeItem[ Item.Pos - EM_TRADE_ITEM1 ] = Item;
        }       
    }
}

//設定對方的交易物品
void    CUI_Trade::SetTargetTradeItem( int Pos , ItemFieldStruct& Item )
{
    _TargetTradeItem[ Pos ] = Item;
    RoleDataEx* Role    = g_pGameMain->RoleData();
    Role->TempData.UpdateInfo.Trade = true;

}

//開啟交易視窗
void    CUI_Trade::OpenTradeMenu( int TradeID )
{
    RoleDataEx* Role    = g_pGameMain->RoleData();
    for( int i = 0 ; i < _MAX_TRADE_ITEM_COUNT_ ; i++ )
    {
        _MyTradeItem[i].Init();
        _TargetTradeItem[i].Init();
    }

    _IsMyTradeOK             = false;
    _IsMyTradeSelectOK       = false;
    _IsTargetTradeSelectOK   = false;
    _MyTradeMoney            = 0;
    _TargetTradeMoney        = 0;



    UpdateButtonState();

    _TradeID                 = TradeID;

    g_pGameMain->RoleData()->TempData.UpdateInfo.Body = true;
    _Win->Show();
    

    if( _Pre_TargetID == TradeID )
    {
        ItemFieldStruct*    SrcItem = Role->GetBodyItem( _Pre_PutItemPos );
        if( SrcItem != NULL && *SrcItem == _Pre_PutItem )
        {
            NetCli_Trade::S_PutItem( 0 , _Pre_PutItemPos , *SrcItem );
            SrcItem->Pos = EM_ItemState_CliDisabled;
        }      
    }
    else
    {
        _Pre_PutItemPos = -1;
        _Pre_TargetID = -1;
    }

    
}

//更新交易物品 
void    CUI_Trade::UpDateTradeItem( const char* Name , bool IsMe )
{
    CCheckButton*       ThisObj;
    CTexture*           Texture;
    RoleDataEx*         Role;
    int                 ID;
    GameObjDbStruct*    ItemDB;
    char                Buf[256];
    ItemFieldStruct     Item;
    Role    = g_pGameMain->RoleData();

    ThisObj = (CCheckButton*)CLayout::GetLayout( Name );

    if( ThisObj == NULL )
        return;

    ID      = ThisObj->GetID();

    if( IsMe == true )
        Item = _MyTradeItem[ ID ];
    else
        Item = _TargetTradeItem[ ID ];

    ItemDB = g_pGameMain->GetObjDB( Item.OrgObjID );

    sprintf( Buf , "%s_Texture", Name  );
    Texture = (CTexture*)CLayout::GetLayout( Buf );

    if( ItemDB == NULL )
    {
        ThisObj->SetTextA( "" );
        Texture->SetVisible( false );
    }
    else
    {
        Texture->SetVisible( true );
        sprintf( Buf , "%d %s" , Item.Count , ItemDB->Name );
        ThisObj->SetTextA( Buf );
//        sprintf( Buf , "interface/icons/%s" , ItemDB->ImageObj->ACTField );
        Texture->SetFilePath( ItemDB->ImageObj->ACTField );
    }  
}


//取得是否交易OK
bool        CUI_Trade::GetSelectTradeOKFlage( bool IsMe )
{
    if( IsMe )
        return _IsMyTradeSelectOK;

    return _IsTargetTradeSelectOK;
}

bool        CUI_Trade::GetTradeOKFlage( bool IsMe )
{
    if( IsMe )
        return _IsMyTradeOK;

    return false;
}

//案下確定交易物品項目(非交易)
void    CUI_Trade::SelectTradeOK( )
{
    NetCli_Trade::S_ItemOK();
    IsMyTradeSelectOK( true );
//    _IsMyTradeSelectOK = true;
//    _MyTradeSelectOK->SetEnable( false );
}

//交易
void    CUI_Trade::Trade( )
{
     NetCli_Trade::S_FinalOK();
     _IsMyTradeOK = true;
    _MyTradeOK->SetEnable( false );   
}


void    CUI_Trade::OnLoadTradeMenu( const char* Name )
{
    _Win = (CFrame*)CLayout::GetLayout( Name );
}

void    CUI_Trade::CloseTradeMenu( )
{
    _Win->Hide();
}

//取消交易
void    CUI_Trade::CancelTrade( )
{
    NetCli_Trade::S_StopTrade( _TradeID );
    _TradeID = -1;
    _Win->Hide();
}

void    CUI_Trade::OnLoadTradeButton( const char* Name )
{
    CFrame*       ThisObj;
    //Role    = g_pGameMain->RoleData();

    ThisObj = (CFrame*)CLayout::GetLayout( Name );

    if( ThisObj == NULL )
        return;
 
    switch( ThisObj->GetID() )
    {
    case 0:
        _MyTradeSelectOK = ThisObj;
        break;
    case 1:
        _TargetTradeSelectOK = ThisObj;
        break;
    case 2:
        _MyTradeOK = ThisObj;
        break;
/*    case 3:
        _TargetTradeOK = ThisObj;
        break;*/
    }
}

void    CUI_Trade::IsMyTradeSelectOK( bool Flag )
{
    _IsMyTradeSelectOK = Flag;    
    UpdateButtonState();
}
void    CUI_Trade::IsTargetTradeSelectOK( bool Flag )
{
    _IsTargetTradeSelectOK = Flag;
    UpdateButtonState();
}   

void    CUI_Trade::IsMyTradeOK( bool Flag )
{
    _IsMyTradeOK = Flag;
    UpdateButtonState();
}

void    CUI_Trade::UpdateButtonState()
{

    if( _IsMyTradeSelectOK == true )
    {
        _MyTradeSelectOK->SetEnable( false );
    }
    else
    {
        _MyTradeSelectOK->SetEnable( true );
    }
    if( _IsTargetTradeSelectOK == true )
    {
        _TargetTradeSelectOK->SetEnable( false );
    }
    else
    {
        _TargetTradeSelectOK->SetEnable( true );
    }
    if( _IsMyTradeSelectOK && _IsTargetTradeSelectOK )
    {
        _MyTradeOK->SetEnable( true );
    }
    else
    {
        _MyTradeOK->SetEnable( false );
    }
}

// ItemPos = -1 表示沒有要求放物品
bool    CUI_Trade::TradeRequest( int TargetID , int ItemPos  )
{
    RoleDataEx* Role    = g_pGameMain->RoleData();
    if( _Win->IsVisible() )
    {
        if( TargetID == _TradeID && ItemPos != -1 )
        {
            ItemFieldStruct*    SrcItem = Role->GetBodyItem( ItemPos );
            if( SrcItem == NULL && SrcItem->IsEmpty() )
                return false;
            for( int i = 0 ; i < _MAX_TRADE_ITEM_COUNT_ ; i ++ )
            {
                if( _MyTradeItem[ i ].IsEmpty() )
                {
                    NetCli_Trade::S_PutItem( i , ItemPos , *SrcItem );

                    SrcItem->Pos = EM_ItemState_CliDisabled;
//                    _IsOnItemCmdProc  = true;            
                    break;
                }
            }
        }
        return false;
    }

    if( ItemPos != -1 )
    {
        ItemFieldStruct*    SrcItem = Role->GetBodyItem( ItemPos );
        if( SrcItem == NULL )
            return false;
        _Pre_PutItemPos = ItemPos;
        _Pre_PutItem = *SrcItem;
    }
    else
        _Pre_PutItemPos = -1;

    _Pre_TargetID = TargetID;
    NetCli_Trade::S_RequestTrade( TargetID );

    return true;
}

//設定交易金錢
void   CUI_Trade::SetTradeMoney( )
{
    RoleDataEx* Role    = g_pGameMain->RoleData();
    if( _IsMyTradeSelectOK == true || Role->BodyMoeny() <= 0)
        return;

    g_pUI_Calculate->Open( Role->BodyMoeny() , NULL , SetMoney_CB );
}

//設定金錢的 CallBack
void   CUI_Trade::SetMoney_CB( void* Obj , int Value )
{
    NetCli_Trade::S_PutMoney( Value );
}

int    CUI_Trade::GetMyTradeMoney()
{
    return _MyTradeMoney;
}

int    CUI_Trade::GetTargetTradeMoney()
{
    return _TargetTradeMoney;
}

void   CUI_Trade::SetMyTradeMoney( int Value )
{
    _MyTradeMoney = Value;
}
void   CUI_Trade::SetTargetTradeMoney( int Value )
{
    _TargetTradeMoney = Value;
}
