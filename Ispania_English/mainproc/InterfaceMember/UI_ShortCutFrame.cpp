#include "../GameMain.h"
#include "UI_ShortCutFrame.h"
#include "ChatFrame.h"
#include "WorldFrame.h"
#include "BackPackage.h"

//----------------------------------------------------------------------------------------------------
CUI_ShortCutFrame							*g_pUI_ShortCutFrame;
//----------------------------------------------------------------------------------------------------
UIShortCutBaseStruct		CUI_ShortCutFrame::m_Base[10][10];			//快捷機本資料
//----------------------------------------------------------------------------------------------------
CUI_ShortCutFrame::CUI_ShortCutFrame()
{
    if ( g_pUI_ShortCutFrame == NULL )  
    {
        g_pUI_ShortCutFrame = new CUI_ShortCutFrame;
    }
}
CUI_ShortCutFrame::~CUI_ShortCutFrame()
{
	SAFE_DELETE( g_pUI_ShortCutFrame );	
}

void		CUI_ShortCutFrame::Initial()
{

}
void		CUI_ShortCutFrame::Release()
{

}

void		CUI_ShortCutFrame::OnShortCutUpDate( const char* Name , int Page )
{
    
    CCheckButton*       ThisObj;
    CTexture*           Texture;
    RoleDataEx*         Role;
    int                 ID;
    
    char                Buf[256];

    Role    = g_pGameMain->RoleData();
	
    ThisObj = (CCheckButton*)CLayout::GetLayout( Name );

    if( ThisObj == NULL )
        return;

    ID      = ThisObj->GetID();
    UIShortCutBaseStruct& Info = m_Base[Page][ID];

    sprintf( Buf , "%sIcon", Name  );
    Texture = (CTexture*)CLayout::GetLayout( Buf );

    switch( Info.Type )
    {
    case EM_DragDropType_Body     :
    case EM_DragDropType_Bank     :
    case EM_DragDropType_EQ       :
    case EM_DragDropType_Shop     :
    case EM_DragDropType_Tran     :
    case EM_DragDropType_ShortCut :
        break;
    default:
        {
            //ThisObj->SetText(L"");
			if ( Texture )
				Texture->SetFilePath("interface\\Button\\ui-emptyslot-disabled");
        }
        return;
    }

	if ( Texture )
		Texture->SetFilePath(Info.ImgFile.c_str());
    
    // ThisObj->SetTextA( Info.Title.c_str() );
    // Texture->SetFilePath( Info.ImgFile.c_str() );
}

void		CUI_ShortCutFrame::OnClick( const char* Name , int Page )
{
    CCheckButton*       ThisObj;
    int                 ID;

    ThisObj = (CCheckButton*)CLayout::GetLayout( Name );

    if( ThisObj == NULL )
        return;

    ID      = ThisObj->GetID();

    UIShortCutBaseStruct& Info = m_Base[Page][ID];

    g_pChatFrame->SendChatMessage( Info.Macro.c_str() , 2 );
}

void        CUI_ShortCutFrame::OnMouseUpItem( const char* Name , int Page )
{

}
void        CUI_ShortCutFrame::OnMouseDownItem( const char* Name , int Page )
{
    
}
void        CUI_ShortCutFrame::OnMouseDragItem( const char* Name , int Page )
{
    CCheckButton*       ThisObj = (CCheckButton*)CLayout::GetLayout( Name );
    RoleDataEx*         Role;
    int                 ID;
    Role    = g_pGameMain->RoleData();

    if( ThisObj == NULL )
        return;
    ID      = ThisObj->GetID();

    UIShortCutBaseStruct& Info = m_Base[Page][ID];

    if( Info.IsEmpty() )
        return;

    ItemClipboard* pClpInfo = g_pMainMenu->PasteItemClipboard();
    g_pMainMenu->ClearItemClipboard();
    if( pClpInfo != NULL )   
        return;


    ItemClipboard TempCP;
    TempCP.SetShortCut( Info.Title , Info.ImgFile ,  Info.Macro , Info.OrgID , Info.Pos , Info.Version , Info.Type );
    g_pMainMenu->CopyItemClipboard( TempCP );
    
    Info.Clear();

}
void        CUI_ShortCutFrame::OnMouseDropItem( const char* Name , int Page )
{
    CCheckButton*       ThisObj = (CCheckButton*)CLayout::GetLayout( Name );
    RoleDataEx*         Role;
    int                 ID;
    Role    = g_pGameMain->RoleData();

    if( ThisObj == NULL )
        return;
    ID      = ThisObj->GetID();

    UIShortCutBaseStruct& Info = m_Base[Page][ID];
  

    ItemClipboard* pClpInfo = g_pMainMenu->PasteItemClipboard();
    g_pMainMenu->ClearItemClipboard();
    if( pClpInfo == NULL )   
        return;

    ItemClipboard   ClpInfo = *pClpInfo;

    g_pBackPackage->ItemDragClsProc( &ClpInfo );

    
    if( !Info.IsEmpty() )
    {
        ItemClipboard TempCP;
        TempCP.SetShortCut( Info.Title , Info.ImgFile ,  Info.Macro , Info.OrgID , Info.Pos , Info.Version , Info.Type );
        g_pMainMenu->CopyItemClipboard( TempCP );
    }


    if( ( ClpInfo.flag & _Def_DragDrop_Flag_ShortCut_ )  == 0 )
        return;


    switch( ClpInfo.type )
    {
    case EM_DragDropType_Body:
        {
            ItemFieldStruct& SrcItem = *Role->GetBodyItem( ClpInfo.pos );
            if( SrcItem.IsEmpty() )
                return;
            SetItem( Page , ID , SrcItem.OrgObjID , SrcItem.Serial );
        }
        break;
    case EM_DragDropType_Bank:
        {
        } 
        break;
    case EM_DragDropType_EQ:
        {
        }
        break;
    case EM_DragDropType_ShortCut:
        {
            UIShortCutBaseStruct    TempInfo;
            TempInfo.ImgFile = ClpInfo.file;
            TempInfo.Macro = ClpInfo.macro;
            TempInfo.OrgID = ClpInfo.itemID;
            TempInfo.Pos = ClpInfo.pos;
            TempInfo.Title = ClpInfo.name;
            TempInfo.Type = ClpInfo.orgType;
            TempInfo.Version = ClpInfo.version;
            
            SetMacroBase( Page , ID , &TempInfo );
        }
        break;
    }


}

void        CUI_ShortCutFrame::SetMacro( int Page , int Pos , const char* Title , const char* ImgFile , const char* Macro )
{
    UIShortCutBaseStruct& Info = m_Base[Page][Pos];

    Info.ImgFile =  ImgFile;
    Info.Macro = Macro;
    Info.Title = Title;
    Info.Pos = Pos;
    Info.Type = EM_DragDropType_ShortCut;
    
    Info.OrgID  = 0;
    Info.Version = 0;
    Info.Pos = 0;
}
void        CUI_ShortCutFrame::SetItem( int Page , int Pos , int OrgID , int Version )
{
    char    Buf[256];
    GameObjDbStruct*    ItemDB;
    UIShortCutBaseStruct& Info = m_Base[Page][Pos];


    ItemDB = g_pGameMain->GetObjDB( OrgID );
    if( ItemDB == NULL )
        return;

//    sprintf( Buf , "interface/icons/%s" , ItemDB->ImageObj->ACTField );
    Info.ImgFile =  ItemDB->ImageObj->ACTField;

    sprintf( Buf , "/UseItem %%t_id %d %d" , OrgID  , Version );
    Info.Macro = Buf;

    Info.Title = ItemDB->Name;
    Info.Pos = Pos;
    Info.Version = Version;
    Info.Type = EM_DragDropType_Body;
    Info.OrgID = OrgID;
}

void        CUI_ShortCutFrame::SetMacroBase( int Page , int Pos , UIShortCutBaseStruct* Info )
{
    m_Base[Page][Pos] = *Info;
}