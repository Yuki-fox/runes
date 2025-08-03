#include "../GameMain.h"
#include "MainMenu.h"

void    ItemClipboard::SetItem_Body( std::string Name , std::string ImgFile , int ItemID , int Pos , int Version  )
{

    file    = ImgFile;
    name    = Name;
    pos     = Pos;
    orgType = type    = EM_DragDropType_Body;
    flag    = _Def_DragDrop_Flag_Item_ | _Def_DragDrop_Flag_ShortCut_;
//    macro   = Macro;
    itemID  = ItemID;
    version = Version;
}

void    ItemClipboard::SetItem_Bank( std::string Name , std::string ImgFile , int ItemID , int Pos , int Version )
{

    file    = ImgFile;
    name    = Name;
    pos     = Pos;
    orgType = type    = EM_DragDropType_Bank;
    flag    = _Def_DragDrop_Flag_Item_ | _Def_DragDrop_Flag_ShortCut_;
//    macro   = Macro;
    itemID  = ItemID;
    version = Version;
}

void    ItemClipboard::SetItem_EQ( std::string Name , std::string ImgFile , int ItemID , int Pos , int Version )
{

    file    = ImgFile;
    name    = Name;
    pos     = Pos;
    orgType = type    = EM_DragDropType_EQ;
    flag    = _Def_DragDrop_Flag_Item_ | _Def_DragDrop_Flag_ShortCut_;
//    macro   = Macro;
    itemID  = ItemID;
    version = Version;
}

void    ItemClipboard::SetShortCut( std::string Name , std::string ImgFile  , std::string Macro , int ItemID, int Pos , int Version , DragDropTypeEnum OrgType  )
{
    file    = ImgFile;
    name    = Name;
    type    = EM_DragDropType_ShortCut;
    flag    = _Def_DragDrop_Flag_ShortCut_;
    macro   = Macro;
    itemID  = ItemID;
    version = Version;
    pos     = Pos;
    orgType = OrgType;
}
void    ItemClipboard::SetMacro    ( std::string Name , std::string ImgFile  , std::string Macro )
{
    file    = ImgFile;
    name    = Name;
    orgType = type    = EM_DragDropType_ShortCut;
    flag    = _Def_DragDrop_Flag_ShortCut_;
    macro   = Macro;
    itemID  = 0;
    version = 0;
    pos     = 0;
}
// **********************************************************************************************
// Globals

CMainMenu*							g_pMainMenu = NULL;
// **********************************************************************************************

// --------------------------------------------------------------------------------------
void CMainMenu::Initial()
{
	if ( g_pMainMenu == NULL )
	{
		g_pMainMenu = new CMainMenu;

		// Register Func
        CFrame::RegisterEventString( _Def_UIEvent_OnDrag_ );
        CFrame::RegisterEventString( _Def_UIEvent_OnDrop_ );
	}
}

// --------------------------------------------------------------------------------------
void CMainMenu::Release()
{
	SAFE_DELETE( g_pMainMenu );	
}

// --------------------------------------------------------------------------------------
CMainMenu::CMainMenu()
{	
	m_haveItemBuf = false;
}

// --------------------------------------------------------------------------------------
CMainMenu::~CMainMenu()
{
}

// --------------------------------------------------------------------------------------
ItemClipboard* CMainMenu::PasteItemClipboard()
{
	if ( !m_haveItemBuf )
		return NULL;
	return &m_itemBuf;
}

// --------------------------------------------------------------------------------------
void CMainMenu::CopyItemClipboard( ItemClipboard& itemData )
{
	m_itemBuf = itemData;
	m_haveItemBuf = true;
	g_pGameMain->SetCursorIcon( CURSOR_DRAG_ITEM );
	g_pGameMain->SetDragCursorIcon( m_itemBuf.file.c_str() );

    CFrame::SendEventString( _Def_UIEvent_OnDrag_, 0, 0 );
}

// --------------------------------------------------------------------------------------
void CMainMenu::ClearItemClipboard()
{
	m_haveItemBuf = false;
	g_pGameMain->ClearDragCursorIcon();
	g_pGameMain->SetCursorIcon( CURSOR_NORMAL_POINT );

    CFrame::SendEventString( _Def_UIEvent_OnDrop_, 0, 0 );
}
	