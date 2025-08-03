#include "../GameMain.h"
#include "BackPackage.h"
#include "WorldFrame.h"

// **********************************************************************************************
// Globals
CBackPackage*							g_pBackPackage = NULL;
// **********************************************************************************************
bool    CBackPackage::_IsOnItemCmdProc = false;
// --------------------------------------------------------------------------------------
void CBackPackage::Initial()
{
	if ( g_pBackPackage == NULL )
	{
		g_pBackPackage = new CBackPackage;
        CFrame::RegisterEventString( _Def_UIEvent_OnItemUpDate_ );
        CFrame::RegisterEventString( _Def_UIEvent_OnBankUpDate_ );
        CFrame::RegisterEventString( _Def_UIEvent_OnEQUpDate_ );
        CFrame::RegisterEventString( _Def_UIEvent_OnTradeUpDate_ );
    }
}

// --------------------------------------------------------------------------------------
void CBackPackage::Release()
{
	SAFE_DELETE( g_pBackPackage );	
}

// --------------------------------------------------------------------------------------
CBackPackage::CBackPackage()
{

    CNetGlobal::Schedular()->Push( _OnTimeUpdate , 1000 , NULL , NULL );    
}

// --------------------------------------------------------------------------------------
CBackPackage::~CBackPackage()
{
}

void        CBackPackage::OnItemUpDate( const char* Name )
{   
    CButton*			ThisObj;
    CTexture*           Texture;
	CFontString*		CountFont;
	CFontString*		StockFont;
    RoleDataEx*         Role;
    int                 ID;
    GameObjDbStruct*    ItemDB;	
    char                Buf[256];

    Role    = g_pGameMain->RoleData();
//    if( Role->TempData.UpdateInfo.Body != true )
//        return;

    ThisObj = (CButton*)CLayout::GetLayout( Name );

    if( ThisObj == NULL )
        return;

    ID      = ThisObj->GetID();
    
    ItemFieldStruct& Item = *Role->GetBodyItem( ID );
    ItemDB = g_pGameMain->GetObjDB( Item.OrgObjID );

    sprintf( Buf , "%sIconTexture", Name  );
    Texture = (CTexture*)CLayout::GetLayout( Buf );
	sprintf( Buf , "%sCount", Name  );
	CountFont = (CFontString*)CLayout::GetLayout( Buf );
	sprintf( Buf , "%sStock", Name  );
	StockFont = (CFontString*)CLayout::GetLayout( Buf );
	
    if( Texture == NULL )
        return;	

    if( ItemDB == NULL )
    {
		if ( Texture )
			Texture->SetFilePath("interface\\Button\\ui-emptyslot-disabled");
		if ( CountFont )
			CountFont->SetText(L"");
		if ( StockFont )
			StockFont->SetText(L"");
			
        //ThisObj->SetTextA( "" );
        //Texture->SetVisible( false );
    }
    else
    {
        //Texture->SetVisible( true );
		sprintf( Buf , "%d" , Item.Count );
		if ( CountFont )
		{
			CountFont->SetVisible( true );
			CountFont->SetTextA( Buf );
		}
		if ( Texture )
			Texture->SetFilePath( ItemDB->ImageObj->ACTField );


        

        //ThisObj->SetTextA( Buf );
        //sprintf( Buf , "interface/icons/%s" , ItemDB->ACTField );
        //Texture->SetFilePath( Buf );

		/*
        Texture->SetFilePath( ItemDB->ImageObj->ACTField );
        int Color = Texture->GetVertexColor() & 0xffffff;        
        if( Item.Pos != EM_ItemState_NONE )
        {            
            Texture->SetVertexColor( Color + 0x7f000000 );
        }
        else
        {
            Texture->SetVertexColor( Color + 0xff000000 );
        }
		*/
    }

	/*
    ItemClipboard* ClpInfo = g_pMainMenu->PasteItemClipboard();
    sprintf( Buf , "%s_HighLight", Name  );
    CTexture* HightLight = (CTexture*)CLayout::GetLayout( Buf );
    HightLight->SetVisible( false ) ;

    if( ClpInfo != NULL  && ClpInfo->type == EM_DragDropType_Body && ClpInfo->pos == ID )
    {
        HightLight->SetVisible( true ) ;
    }
	*/    
}
// --------------------------------------------------------------------------------------
//定時處理
int   CBackPackage::_OnTimeUpdate( SchedularInfo* Obj , void* InputClass )
{
    CNetGlobal::Schedular()->Push( _OnTimeUpdate , 200 , NULL , NULL );    
    int Count;
    GameObjDbStruct*    ItemDB;

    if( _IsOnItemCmdProc  == true )
        return 0;

    RoleDataEx*         Role;
    Role    = g_pGameMain->RoleData();
    if( Role->TempData.ItemTemp.IsEmpty() )
        return 0;

    ItemFieldStruct& Item = *Role->TempData.ItemTemp.Begin();
    ItemDB = g_pGameMain->GetObjDB(  Item.OrgObjID );
    if( ItemDB == NULL)
        return 0;

    int EmptyPos = Role->SearchBodyFirstFit( &Item );
    if( EmptyPos != -1 )
    {
        _IsOnItemCmdProc  = true;
        if( Role->GetBodyItem( EmptyPos )->IsEmpty() )
        {
            Count = Item.Count;
            *Role->GetBodyItem( EmptyPos ) = Item;            
        }
        else
        {
            Count = __min( Item.Count , ItemDB->MaxHeap - Role->GetBodyItem( EmptyPos )->Count );
            Role->GetBodyItem( EmptyPos )->Count += Count;
        }
        Role->GetBodyItem( EmptyPos )->Pos = EM_ItemState_CliDisabled;
        NetCli_Item::_GetBufferItem( *Role->TempData.ItemTemp.Begin() , EmptyPos , Count );
        if( Count == Role->TempData.ItemTemp.Front().Count )
            Role->TempData.ItemTemp.Pop_Front();
        else
            Role->TempData.ItemTemp.Front().Count -= Count;


        _IsOnItemCmdProc = true;
    }
    else 
    {
        if( Role->TempData.ItemTemp.Size() >= 7 )
        {
            g_pError->AddMessage( 2, 0, "丟棄暫存物品 %d", Role->TempData.ItemTemp.Begin()->OrgObjID );
            _IsOnItemCmdProc  = true;
            NetCli_Item::_DelBufferItem( *Role->TempData.ItemTemp.Begin() );
            Role->TempData.ItemTemp.Pop_Front();
            _IsOnItemCmdProc = true;
        }
        
    }
    
    return 0;
}
// --------------------------------------------------------------------------------------
void        CBackPackage::ClsOnItemCmdProc( )
{
    _IsOnItemCmdProc = false;
}
// --------------------------------------------------------------------------------------
void        CBackPackage::OnItemDrag( const char* Name )
{
    CCheckButton*       ThisObj = (CCheckButton*)CLayout::GetLayout( Name );
    RoleDataEx*         Role;
    int                 ID;
    GameObjDbStruct*    ItemDB;
    Role    = g_pGameMain->RoleData();


    if( ThisObj == NULL )
        return;
    ID      = ThisObj->GetID();

    ItemFieldStruct& Item = *Role->GetBodyItem( ID );
    ItemDB = g_pGameMain->GetObjDB( Item.OrgObjID );

    if( Item.Pos != EM_ItemState_NONE )
        return;
    if( ItemDB == NULL )
        return;

    ItemClipboard ClpInfo;    
    //char    ImgFile[256];
//    sprintf( ImgFile , "interface/icons/%s" , ItemDB->ACTField );
    ClpInfo.SetItem_Body( ItemDB->Name , ItemDB->ImageObj->ACTField , ItemDB->GUID , ID , Item.Serial );

    g_pMainMenu->CopyItemClipboard( ClpInfo );
    Item.Pos = EM_ItemState_CliDisabled;

    Role->TempData.UpdateInfo.Body = true;
}
// --------------------------------------------------------------------------------------
void        CBackPackage::UseItem_Body( const char* Name )
{
    CCheckButton*       ThisObj = (CCheckButton*)CLayout::GetLayout( Name );
    RoleDataEx*         Role;
    int                 ID;
    GameObjDbStruct*    ItemDB;
    Role    = g_pGameMain->RoleData();

    if( ThisObj == NULL )
        return;
    ID      = ThisObj->GetID();
    ItemFieldStruct& Item = *Role->GetBodyItem( ID );


    ItemClipboard* ClpInfo = g_pMainMenu->PasteItemClipboard();
    if( ClpInfo != NULL )
        return;

    if( _IsOnItemCmdProc == true )
        return;

    ItemDB = g_pGameMain->GetObjDB( Item.OrgObjID );    

    if( ItemDB == NULL )
        return;

    if( ItemDB->Item.IncMagic_Onuse == 0 )
        return;

    CRoleSprite*		CTarget =	g_pWorldFrame->GetTargetPlayer();
    
    if( CTarget == NULL )
        NetCli_Item::_UseItem_Body( Item , ID , -1 );
    else
        NetCli_Item::_UseItem_Body( Item , ID , CTarget->GetIdentify() );

    Item.Pos = EM_ItemState_CliDisabled;

    _IsOnItemCmdProc  = true;
}
// --------------------------------------------------------------------------------------
void        CBackPackage::UseItem_Bank( const char* Name )
{
    CCheckButton*       ThisObj = (CCheckButton*)CLayout::GetLayout( Name );
    RoleDataEx*         Role;
    int                 ID;
    GameObjDbStruct*    ItemDB;
    Role    = g_pGameMain->RoleData();

    if( ThisObj == NULL )
        return;
    ID      = ThisObj->GetID();
    ItemFieldStruct& Item = *Role->GetBankItem( ID );


    ItemClipboard* ClpInfo = g_pMainMenu->PasteItemClipboard();
    if( ClpInfo != NULL )
        return;

    if( _IsOnItemCmdProc == true )
        return;

    ItemDB = g_pGameMain->GetObjDB( Item.OrgObjID );    

    if( ItemDB == NULL )
        return;

    if( ItemDB->Item.IncMagic_Onuse == 0 )
        return;

    CRoleSprite*		CTarget =	g_pWorldFrame->GetTargetPlayer();

    if( CTarget == NULL )
        NetCli_Item::_UseItem_Bank( Item , ID , -1 );
    else
        NetCli_Item::_UseItem_Bank( Item , ID , CTarget->GetIdentify() );

    Item.Pos = EM_ItemState_CliDisabled;

    _IsOnItemCmdProc  = true;
}
// --------------------------------------------------------------------------------------
void        CBackPackage::UseItem_EQ( const char* Name )
{
    CCheckButton*       ThisObj = (CCheckButton*)CLayout::GetLayout( Name );
    RoleDataEx*         Role;
    int                 ID;
    GameObjDbStruct*    ItemDB;
    Role    = g_pGameMain->RoleData();

    if( ThisObj == NULL )
        return;
    ID      = ThisObj->GetID();
    ItemFieldStruct& Item = *Role->GetEqItem( ID );


    ItemClipboard* ClpInfo = g_pMainMenu->PasteItemClipboard();
    if( ClpInfo != NULL )
        return;

    if( _IsOnItemCmdProc == true )
        return;

    ItemDB = g_pGameMain->GetObjDB( Item.OrgObjID );    

    if( ItemDB == NULL )
        return;

    if( ItemDB->Item.IncMagic_Onuse == 0 )
        return;

    CRoleSprite*		CTarget =	g_pWorldFrame->GetTargetPlayer();

    if( CTarget == NULL )
        NetCli_Item::_UseItem_EQ( Item , ID , -1 );
    else
        NetCli_Item::_UseItem_EQ( Item , ID , CTarget->GetIdentify() );

    Item.Pos = EM_ItemState_CliDisabled;

    _IsOnItemCmdProc  = true;
}
// --------------------------------------------------------------------------------------
void        CBackPackage::OnItemDrop( const char* Name )
{
    CCheckButton*       ThisObj = (CCheckButton*)CLayout::GetLayout( Name );
    RoleDataEx*         Role;
    int                 ID;
    Role    = g_pGameMain->RoleData();
    
    if( ThisObj == NULL )
        return;
    ID      = ThisObj->GetID();
    ItemFieldStruct& Item = *Role->GetBodyItem( ID );


    ItemClipboard* ClpInfo = g_pMainMenu->PasteItemClipboard();
    g_pMainMenu->ClearItemClipboard();
    if( ClpInfo == NULL )
        return;

    ItemDragClsProc( ClpInfo );

    if( ( ClpInfo->flag & _Def_DragDrop_Flag_Item_ )  == 0 )
        return;


    if( _IsOnItemCmdProc  == true )
        return;
      

    switch( ClpInfo->type )
    {
    case EM_DragDropType_Body:
        {
            if( ClpInfo->pos == ID )
            {
                Role->TempData.UpdateInfo.Body = true;
                return;
            }
            
            ItemFieldStruct& SrcItem = *Role->GetBodyItem( ClpInfo->pos );
            NetCli_Item::_ExchangeField_Body(  SrcItem , ClpInfo->pos , Item , ID );
            Item.Pos =SrcItem.Pos = EM_ItemState_CliDisabled;
        }
        break;
    case EM_DragDropType_Bank:
        {
            ItemFieldStruct& SrcItem = *Role->GetBankItem( ClpInfo->pos );
            NetCli_Item::_ExchangeField_BankBody(  SrcItem , ClpInfo->pos , Item , ID );
            Item.Pos =SrcItem.Pos = EM_ItemState_CliDisabled;
        } 
        break;
    case EM_DragDropType_EQ:
        {
            ItemFieldStruct& SrcItem = *Role->GetEqItem( ClpInfo->pos );
            NetCli_EQ::S_SetEQ_Body(  Item , ID , ClpInfo->pos );
            Item.Pos =SrcItem.Pos = EM_ItemState_CliDisabled;
        }
        break;
    }

    _IsOnItemCmdProc  = true;

}
//--------------------------------------------------------------------------------------
bool        CBackPackage::CheckDragEvent( )
{
    if( g_pMainMenu->PasteItemClipboard() == NULL )
        return false;
    return true;
}
//--------------------------------------------------------------------------------------
void        CBackPackage::ItemDragClsProc( ItemClipboard* ClpInfo  )
{
    RoleDataEx*         Role;
    Role    = g_pGameMain->RoleData();

    switch(  ClpInfo->type  )
    {
    case EM_DragDropType_Body:
        {
            ItemFieldStruct& Item = *Role->GetBodyItem( ClpInfo->pos );
            if( Item.Pos == EM_ItemState_CliDisabled )
                Item.Pos = EM_ItemState_NONE;
            Role->TempData.UpdateInfo.Body = true;
            break;
        }
    case EM_DragDropType_Bank:
        {
            ItemFieldStruct& Item = *Role->GetBankItem( ClpInfo->pos );
            if( Item.Pos == EM_ItemState_CliDisabled )
                Item.Pos = EM_ItemState_NONE;
            Role->TempData.UpdateInfo.Bank = true;
            break;
        }
    case EM_DragDropType_EQ:
        {
            ItemFieldStruct& Item = *Role->GetEqItem( ClpInfo->pos );
            if( Item.Pos == EM_ItemState_CliDisabled )
                Item.Pos = EM_ItemState_NONE;
            Role->TempData.UpdateInfo.Eq = true;
            break;
        }
        break;
    }
    
    
}
// --------------------------------------------------------------------------------------
void        CBackPackage::OnBankDrag( const char* Name )
{
    CCheckButton*       ThisObj = (CCheckButton*)CLayout::GetLayout( Name );
    RoleDataEx*         Role;
    int                 ID;
    GameObjDbStruct*    ItemDB;
    Role    = g_pGameMain->RoleData();


    if( ThisObj == NULL )
        return;
    ID      = ThisObj->GetID();

    ItemFieldStruct& Item = *Role->GetBankItem( ID );
    ItemDB = g_pGameMain->GetObjDB( Item.OrgObjID );

    if( Item.Pos != EM_ItemState_NONE )
        return;
    if( ItemDB == NULL )
        return;

    ItemClipboard ClpInfo;    
    //char    ImgFile[256];
    //sprintf( ImgFile , "interface/icons/%s" , ItemDB->ACTField );
    ClpInfo.SetItem_Bank( ItemDB->Name , ItemDB->ImageObj->ACTField , ItemDB->GUID  , ID , Item.Serial  );

    g_pMainMenu->CopyItemClipboard( ClpInfo );
    Item.Pos = EM_ItemState_CliDisabled;
    Role->TempData.UpdateInfo.Bank = true;
}
// --------------------------------------------------------------------------------------
void        CBackPackage::OnBankDrop( const char* Name )
{
    CCheckButton*       ThisObj = (CCheckButton*)CLayout::GetLayout( Name );
    RoleDataEx*         Role;
    int                 ID;
    Role    = g_pGameMain->RoleData();

    if( ThisObj == NULL )
        return;
    ID      = ThisObj->GetID();
    ItemFieldStruct& Item = *Role->GetBankItem( ID );


    ItemClipboard* ClpInfo = g_pMainMenu->PasteItemClipboard();
    g_pMainMenu->ClearItemClipboard();
    if( ClpInfo == NULL )
        return;

    ItemDragClsProc( ClpInfo );
    if( ( ClpInfo->flag & _Def_DragDrop_Flag_Item_ )  == 0 )
        return;

    if( _IsOnItemCmdProc  == true )
        return;


    switch( ClpInfo->type )
    {
    case EM_DragDropType_Body:
        {
            ItemFieldStruct& SrcItem = *Role->GetBodyItem( ClpInfo->pos );
            NetCli_Item::_ExchangeField_BodyBank(  SrcItem , ClpInfo->pos , Item , ID );
            Item.Pos =SrcItem.Pos = EM_ItemState_CliDisabled;
        }
        break;
    case EM_DragDropType_Bank:
        {
            if( ClpInfo->pos == ID )
            {
                Role->TempData.UpdateInfo.Bank = true;
                return;
            }

            ItemFieldStruct& SrcItem = *Role->GetBankItem( ClpInfo->pos );
            NetCli_Item::_ExchangeField_Bank(  SrcItem , ClpInfo->pos , Item , ID );
            Item.Pos =SrcItem.Pos = EM_ItemState_CliDisabled;
            
        }
        break;
    case EM_DragDropType_EQ:
        {
            ItemFieldStruct& SrcItem = *Role->GetEqItem( ClpInfo->pos );
            NetCli_EQ::S_SetEQ_Bank(  Item , ID , ClpInfo->pos );
            Item.Pos =SrcItem.Pos = EM_ItemState_CliDisabled;

        }
        break;
    }

    _IsOnItemCmdProc  = true;

}
// --------------------------------------------------------------------------------------
void        CBackPackage::OnBankUpDate( const char* Name )
{   
    CCheckButton*       ThisObj;
    CTexture*           Texture;
    RoleDataEx*         Role;
    int                 ID;
    GameObjDbStruct*    ItemDB;
    char                Buf[256];

    Role    = g_pGameMain->RoleData();
//    if( Role->TempData.UpdateInfo.Bank == false )
//        return;

    ThisObj = (CCheckButton*)CLayout::GetLayout( Name );
    if( ThisObj == NULL )
        return;

    ID      = ThisObj->GetID();

    ItemFieldStruct& Item = *Role->GetBankItem( ID );
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
//        sprintf( Buf , "interface/icons/%s" , ItemDB->ACTField );
//        Texture->SetFilePath( Buf );
        Texture->SetFilePath( ItemDB->ImageObj->ACTField );
        int Color = Texture->GetVertexColor() & 0xffffff;        
        if( Item.Pos == EM_ItemState_CliDisabled )
        {            
            Texture->SetVertexColor( Color + 0x7f000000 );
        }
        else
        {
            Texture->SetVertexColor( Color + 0xff000000 );
        }
    }

    ItemClipboard* ClpInfo = g_pMainMenu->PasteItemClipboard();
    sprintf( Buf , "%s_HighLight", Name  );
    CTexture* HightLight = (CTexture*)CLayout::GetLayout( Buf );
    HightLight->SetVisible( false ) ;

    if( ClpInfo != NULL  && ClpInfo->type == EM_DragDropType_Bank && ClpInfo->pos == ID )
    {
        HightLight->SetVisible( true ) ;
    }
}
// --------------------------------------------------------------------------------------
void        CBackPackage::OnEQDrag( const char* Name )
{
    CCheckButton*       ThisObj = (CCheckButton*)CLayout::GetLayout( Name );
    RoleDataEx*         Role;
    int                 ID;
    GameObjDbStruct*    ItemDB;
    Role    = g_pGameMain->RoleData();


    if( ThisObj == NULL )
        return;
    ID      = ThisObj->GetID();

    ItemFieldStruct& Item = *Role->GetEqItem( ID );
    ItemDB = g_pGameMain->GetObjDB( Item.OrgObjID );

    if( Item.Pos != EM_ItemState_NONE )
        return;
    if( ItemDB == NULL )
        return;

    ItemClipboard ClpInfo;    
//    char    ImgFile[256];
//    sprintf( ImgFile , "interface/icons/%s" , ItemDB->ACTField );
    ClpInfo.SetItem_EQ( ItemDB->Name , ItemDB->ImageObj->ACTField , ItemDB->GUID , ID , Item.Serial );

    g_pMainMenu->CopyItemClipboard( ClpInfo );
    Item.Pos = EM_ItemState_CliDisabled;
    Role->TempData.UpdateInfo.Eq = true;
}
// --------------------------------------------------------------------------------------
void        CBackPackage::OnEQDrop( const char* Name )
{ 
    CCheckButton*       ThisObj = (CCheckButton*)CLayout::GetLayout( Name );
    RoleDataEx*         Role;
    int                 ID;
    Role    = g_pGameMain->RoleData();

    if( ThisObj == NULL )
        return;
    ID      = ThisObj->GetID();
    ItemFieldStruct& Item = *Role->GetBankItem( ID );


    ItemClipboard* ClpInfo = g_pMainMenu->PasteItemClipboard();
    g_pMainMenu->ClearItemClipboard();
    if( ClpInfo == NULL )
        return;

    ItemDragClsProc( ClpInfo );
    if( ( ClpInfo->flag & _Def_DragDrop_Flag_Item_ )  == 0 )
        return;

    if( _IsOnItemCmdProc  == true )
        return;

    GameObjDbStructEx*	OrgDB = Role->St_GetObjDB( ClpInfo->itemID );

    switch( ClpInfo->type )
    {
    case EM_DragDropType_Body:
        {
            ItemFieldStruct&    SrcItem = *Role->GetBodyItem( ClpInfo->pos );
            
            if(     OrgDB != NULL
                &&  Role->CheckBaseSetEQ( ID , OrgDB ) == true 
                &&  Role->CheckSetEQ( ID , OrgDB ) == false     )
            {
                if( ID == EM_EQWearPos_SecondHand )
                {
                    ItemFieldStruct&    OtherHandItem = *Role->GetEqItem( EM_EQWearPos_MainHand ); 
                    int EmptyPos = Role->SearchBodyEmptyPos();
                    if( EmptyPos == -1 )
                        return;
                    ItemFieldStruct&    EmptyItem = *Role->GetBodyItem( EmptyPos );
                    NetCli_EQ::S_SetEQ_Body(  EmptyItem , EmptyPos , EM_EQWearPos_MainHand );
                    OtherHandItem.Pos = EM_ItemState_CliDisabled;

                }
                else if( ID == EM_EQWearPos_MainHand )
                {
                    ItemFieldStruct&    OtherHandItem = *Role->GetEqItem( EM_EQWearPos_SecondHand ); 
                    int EmptyPos = Role->SearchBodyEmptyPos();
                    if( EmptyPos == -1 )
                        return;
                    ItemFieldStruct&    EmptyItem = *Role->GetBodyItem( EmptyPos );
                    NetCli_EQ::S_SetEQ_Body(  EmptyItem , EmptyPos , EM_EQWearPos_SecondHand );
                    OtherHandItem.Pos = EM_ItemState_CliDisabled;
                }

            }

            NetCli_EQ::S_SetEQ_Body(  SrcItem , ClpInfo->pos , ID );
            Item.Pos =SrcItem.Pos = EM_ItemState_CliDisabled;
            _IsOnItemCmdProc  = true;
        }
        break;
    case EM_DragDropType_Bank:
        {
            ItemFieldStruct&    SrcItem = *Role->GetBankItem( ClpInfo->pos );

            if(     OrgDB != NULL
                &&  Role->CheckBaseSetEQ( ID , OrgDB ) == true 
                &&  Role->CheckSetEQ( ID , OrgDB ) == false     )
            {
                if( ID == EM_EQWearPos_SecondHand )
                {
                    ItemFieldStruct&    OtherHandItem = *Role->GetEqItem( EM_EQWearPos_MainHand ); 
                    int EmptyPos = Role->SearchBankEmptyPos();
                    if( EmptyPos == -1 )
                        return;
                    ItemFieldStruct&    EmptyItem = *Role->GetBankItem( EmptyPos );
                    NetCli_EQ::S_SetEQ_Bank(  EmptyItem , EmptyPos , EM_EQWearPos_MainHand );
                    OtherHandItem.Pos = EM_ItemState_CliDisabled;

                }
                else if( ID == EM_EQWearPos_MainHand )
                {
                    ItemFieldStruct&    OtherHandItem = *Role->GetEqItem( EM_EQWearPos_SecondHand ); 
                    int EmptyPos = Role->SearchBankEmptyPos();
                    if( EmptyPos == -1 )
                        return;
                    ItemFieldStruct&    EmptyItem = *Role->GetBankItem( EmptyPos );
                    NetCli_EQ::S_SetEQ_Bank(  EmptyItem , EmptyPos , EM_EQWearPos_SecondHand );
                    OtherHandItem.Pos = EM_ItemState_CliDisabled;
                }
            }
            NetCli_EQ::S_SetEQ_Bank(  SrcItem , ClpInfo->pos , ID );
            Item.Pos =SrcItem.Pos = EM_ItemState_CliDisabled;
            _IsOnItemCmdProc  = true;

        }
        break;
    case EM_DragDropType_EQ:
        {
            if( ClpInfo->pos == ID )
            {
                Role->TempData.UpdateInfo.Eq = true;
                return;
            }

            ItemFieldStruct&    SrcItem = *Role->GetEqItem( ClpInfo->pos );

            if(     OrgDB != NULL
                &&  Role->CheckBaseSetEQ( ID , OrgDB ) == true 
                &&  Role->CheckEQSwap(  ClpInfo->pos , ID ) == false     )
            {
                
                ItemFieldStruct&    OtherHandItem = *Role->GetEqItem( ID ) ;

                int EmptyPos = Role->SearchBodyEmptyPos();
                if( EmptyPos == -1 )
                    return;
                ItemFieldStruct&    EmptyItem = *Role->GetBodyItem( EmptyPos );
                NetCli_EQ::S_SetEQ_Body(  EmptyItem , EmptyPos , ID );
                OtherHandItem.Pos = EM_ItemState_CliDisabled;
            }

            NetCli_EQ::S_SetEQ_EQ(  SrcItem , ClpInfo->pos , ID );
            Item.Pos =SrcItem.Pos = EM_ItemState_CliDisabled;
            _IsOnItemCmdProc  = true;
        }
        break;
    }

    

}
// --------------------------------------------------------------------------------------
void        CBackPackage::OnEQUpDate( const char* Name )
{   

    CCheckButton*       ThisObj;
    CTexture*           Texture;
    CTexture*           HightLight;
    RoleDataEx*         Role;
    int                 ID;
    GameObjDbStruct*    ItemDB;
    char                Buf[256];

    Role    = g_pGameMain->RoleData();
    ThisObj = (CCheckButton*)CLayout::GetLayout( Name );

    if( ThisObj == NULL )
        return;

    ID      = ThisObj->GetID();

    ItemFieldStruct& Item = *Role->GetEqItem( ID );
    ItemDB = g_pGameMain->GetObjDB( Item.OrgObjID );

//    if( Role->TempData.UpdateInfo.Eq == true )
    {
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
//            sprintf( Buf , "interface/icons/%s" , ItemDB->ACTField );
//            Texture->SetFilePath( Buf );
            Texture->SetFilePath( ItemDB->ImageObj->ACTField );
            int Color = Texture->GetVertexColor() & 0xffffff;        
            if( Item.Pos == EM_ItemState_CliDisabled )
            {            
                Texture->SetVertexColor( Color + 0x7f000000 );
            }
            else
            {
                Texture->SetVertexColor( Color + 0xff000000 );
            }
        }
    }


    
    sprintf( Buf , "%s_HighLight", Name  );
    HightLight = (CTexture*)CLayout::GetLayout( Buf );
    HightLight->SetVisible( false ) ;
    ItemClipboard* ClpInfo = g_pMainMenu->PasteItemClipboard();
    if( ClpInfo != NULL )
    {
        GameObjDbStructEx*	OrgDB = Role->St_GetObjDB( ClpInfo->itemID );
        if( OrgDB  != NULL && Role->CheckBaseSetEQ( ID , OrgDB ) == true )
            HightLight->SetVisible( true ) ;
    }

}
// --------------------------------------------------------------------------------------
void        CBackPackage::BodyMoneyUpdate( const char* Name )
{
    RoleDataEx* Role = g_pGameMain->RoleData();
    CFontString* ThisObj;	
}
// --------------------------------------------------------------------------------------
int			CBackPackage::GetBodyMoney()
{
	RoleDataEx* Role = g_pGameMain->RoleData();
	return Role->BaseData.Body.Money;
}
// --------------------------------------------------------------------------------------
void        CBackPackage::OnTradeDrag( const char* Name )
{
}
// --------------------------------------------------------------------------------------
void        CBackPackage::OnTradeDrop( const char* Name )
{ 
    CCheckButton*       ThisObj = (CCheckButton*)CLayout::GetLayout( Name );
    RoleDataEx*         Role;
    int                 ID;
    Role    = g_pGameMain->RoleData();

    if( ThisObj == NULL )
        return;
    ID      = ThisObj->GetID();
    ItemFieldStruct& Item = *Role->GetBankItem( ID );


    ItemClipboard* ClpInfo = g_pMainMenu->PasteItemClipboard();
    g_pMainMenu->ClearItemClipboard();
    if( ClpInfo == NULL )
        return;

    ItemDragClsProc( ClpInfo );
    if( ( ClpInfo->flag & _Def_DragDrop_Flag_Item_ )  == 0 )
        return;

    if( _IsOnItemCmdProc  == true )
        return;

    GameObjDbStruct*	OrgDB = Role->St_GetObjDB( ClpInfo->itemID );

    switch( ClpInfo->type )
    {
    case EM_DragDropType_Body:
        {
            ItemFieldStruct&    SrcItem = *Role->GetBodyItem( ClpInfo->pos );

            NetCli_Trade::S_PutItem( ID , ClpInfo->pos , SrcItem );

            Item.Pos =SrcItem.Pos = EM_ItemState_CliDisabled;
            _IsOnItemCmdProc  = true;
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
    }

}

const char*	CBackPackage::GetBodyItemName( int index )
{
    RoleDataEx* Role      = g_pGameMain->RoleData();
    ItemFieldStruct& Item = *Role->GetBodyItem( index );

    GameObjDbStruct*	OrgDB = Role->St_GetObjDB( Item.OrgObjID );
    if( OrgDB == NULL )
        return NULL;

    return OrgDB->Name;

}