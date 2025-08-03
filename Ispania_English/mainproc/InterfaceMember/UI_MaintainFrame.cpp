#include "../GameMain.h"
#include "UI_MaintainFrame.h"
#include "ChatFrame.h"
#include "WorldFrame.h"

CFrame*             CUI_MaintainFrame::_UI_Win          = NULL;
CFontString*        CUI_MaintainFrame::_UI_Title        = NULL;
CEditBox*           CUI_MaintainFrame::_UI_ObjID        = NULL;
CEditBox*           CUI_MaintainFrame::_UI_OrgName      = NULL;
CEditBox*           CUI_MaintainFrame::_UI_GItemID      = NULL;
CEditBox*           CUI_MaintainFrame::_UI_Name         = NULL;
CEditBox*           CUI_MaintainFrame::_UI_ObjClass     = NULL;
CEditBox*           CUI_MaintainFrame::_UI_PID          = NULL;
CEditBox*           CUI_MaintainFrame::_UI_AutoPlot     = NULL;
CEditBox*           CUI_MaintainFrame::_UI_QuestID      = NULL;
CEditBox*           CUI_MaintainFrame::_UI_PosX         = NULL;
CEditBox*           CUI_MaintainFrame::_UI_PosY         = NULL;
CEditBox*           CUI_MaintainFrame::_UI_PosZ         = NULL;
CEditBox*           CUI_MaintainFrame::_UI_PosDir       = NULL;
CButton*            CUI_MaintainFrame::_UI_PosReset     = NULL;
CEditBox*           CUI_MaintainFrame::_UI_MacroName    = NULL;
CButton*            CUI_MaintainFrame::_UI_MacroCreate  = NULL;
CFontString*        CUI_MaintainFrame::_UI_Macro        = NULL;
CButton*            CUI_MaintainFrame::_UI_Accept       = NULL;
CButton*            CUI_MaintainFrame::_UI_Cencel       = NULL;
CCheckButton*       CUI_MaintainFrame::_UI_CheckMode[32];
int                 CUI_MaintainFrame::_GItemID         = -1;
//----------------------------------------------------------------------------------------------------
CUI_MaintainFrame							*g_pUI_MaintainFrame;
//----------------------------------------------------------------------------------------------------
CUI_MaintainFrame::CUI_MaintainFrame()
{
    if ( g_pUI_MaintainFrame == NULL )  
    {
        g_pUI_MaintainFrame = new CUI_MaintainFrame;
    }
}
CUI_MaintainFrame::~CUI_MaintainFrame()
{
	SAFE_DELETE( g_pUI_MaintainFrame );	
}

void		CUI_MaintainFrame::Initial()
{

}
void		CUI_MaintainFrame::Release()
{

}

void        CUI_MaintainFrame::OnLoad( )
{
    _UI_Win         = (CFrame*     )CLayout::GetLayout( "MaintainInterface" );
    _UI_Title       = (CFontString*)CLayout::GetLayout( "MaintainInterface1Title" );
    _UI_ObjID       = (CEditBox*   )CLayout::GetLayout( "MT_OrgObjIDEdit" );
    _UI_OrgName     = (CEditBox*   )CLayout::GetLayout( "MT_OrgNameEdit" );
    _UI_GItemID     = (CEditBox*   )CLayout::GetLayout( "MT_GItemIDEdit" );
    _UI_Name        = (CEditBox*   )CLayout::GetLayout( "MT_NameEdit" );
    _UI_ObjClass    = (CEditBox*   )CLayout::GetLayout( "MT_PlotTypeEdit" );
    _UI_PID         = (CEditBox*   )CLayout::GetLayout( "MT_PIDEdit" );
    _UI_AutoPlot    = (CEditBox*   )CLayout::GetLayout( "MT_PlotNameEdit" );
    _UI_QuestID     = (CEditBox*   )CLayout::GetLayout( "MT_QuestIDEdit" );
    _UI_PosX        = (CEditBox*   )CLayout::GetLayout( "MT_PositionXEdit" );
    _UI_PosY        = (CEditBox*   )CLayout::GetLayout( "MT_PositionYEdit" );
    _UI_PosZ        = (CEditBox*   )CLayout::GetLayout( "MT_PositionZEdit" );
    _UI_PosDir      = (CEditBox*   )CLayout::GetLayout( "MT_PositionDirEdit" );
    _UI_PosReset    = (CButton*    )CLayout::GetLayout( "MT_SetPosition" );
    _UI_MacroName   = (CEditBox*   )CLayout::GetLayout( "MT_MacroNameEdit" );
    _UI_MacroCreate = (CButton*    )CLayout::GetLayout( "MT_CreateMacro" );
    _UI_Macro       = (CFontString*)CLayout::GetLayout( "MT_MacroStr" );
    _UI_Accept      = (CButton*    )CLayout::GetLayout( "MT_Accept" );
    _UI_Cencel      = (CButton*    )CLayout::GetLayout( "MT_Cencel" );

    char Buf[256];
    for( int i = 0 ; i < 32 ; i++ )
    {
        sprintf( Buf , "MT_Mode%d" , i + 1  );
        _UI_CheckMode[i] = (CCheckButton*)CLayout::GetLayout( Buf );
    }
    
    if(     _UI_Win         ==  NULL       
         || _UI_Title       ==  NULL   
         || _UI_ObjID       ==  NULL   
         || _UI_OrgName     ==  NULL   
         || _UI_GItemID     ==  NULL   
         || _UI_Name        ==  NULL   
         || _UI_ObjClass    ==  NULL   
         || _UI_PID         ==  NULL   
         || _UI_AutoPlot    ==  NULL   
         || _UI_QuestID     ==  NULL   
         || _UI_PosX        ==  NULL   
         || _UI_PosY        ==  NULL   
         || _UI_PosZ        ==  NULL   
         || _UI_PosDir      ==  NULL   
         || _UI_PosReset    ==  NULL   
         || _UI_MacroName   ==  NULL   
         || _UI_MacroCreate ==  NULL   
         || _UI_Macro       ==  NULL   
         || _UI_Accept      ==  NULL   
         || _UI_Cencel      ==  NULL   )
    {
        _UI_Win = NULL;
    }
}

bool        CUI_MaintainFrame::Open( )
{
    if( _UI_Win != NULL )
    {
        if( _UI_Win->IsVisible() )
            return false;
        _UI_GItemID->SetTextA("");
        _UI_Title->SetTextA( "件立物件" );
        _GItemID = -1;
        _UI_Win->Show( );
        OnResetPos();
        return true;
    }
    return false;
}

bool        CUI_MaintainFrame::ModifyOpen( ModifyNPCInfo*  Info )
{
    if( _UI_Win != NULL )
    {
        _UI_Win->Show( );

        SetModifyInfo( Info );
        return true;
    }
    return false;
}

bool        CUI_MaintainFrame::Hide( )
{
    if( _UI_Win != NULL )
    {
        _UI_Win->Hide( );
        _GItemID = -1;
        return true;
    }
    return false;
}

void        CUI_MaintainFrame::OnAcceptClick( )
{
    if( _GItemID == -1 )
        g_pChatFrame->SendChatMessage( _UI_Macro->GetTextA() , 2 );
    else
    {
        ModifyNPCInfo Info;
        GetModifyInfo( &Info );
        Net_GSrvConnect::ModyNPC( &Info );
    }
    _UI_Win->Hide();
}

void        CUI_MaintainFrame::OnResetPos( )
{
    CRoleSprite* pSprite = g_pWorldFrame->GetPlayer(  );
    if( pSprite == NULL )
    {
        return;
    }
    CRuVector3 direction = pSprite->GetDirection();
    CRuVector3 nowPos = pSprite->GetPos() - g_offsetPosstion;

    float theta = 0.0f;

    // 取得角色所朝方向
    theta = acos( direction.Dot( CRuVector3(1.0f, 0.0f, 0.0f) ) );
    if ( direction.m_z > 0.0f )
        theta = PI + PI - theta;

    // 轉換為角度
    theta = theta * 180.0f / PI;

    char    Buf[256];
    sprintf( Buf , "%d" , int(nowPos.m_x) );
    _UI_PosX->SetTextA( Buf );
    sprintf( Buf , "%d" , int(nowPos.m_y) );
    _UI_PosY->SetTextA( Buf );  
    sprintf( Buf , "%d" , int(nowPos.m_z) );
    _UI_PosZ->SetTextA( Buf );
    sprintf( Buf , "%d" , int(theta) );
    _UI_PosDir->SetTextA( Buf );

}

void        CUI_MaintainFrame::OnUpdate( )
{
    static  int LOrgID = 0;
    char    Buf[1024];
    int     OrgID = 0;
    int     QuestID = 0;
    int     Mode = 0;
    int     PID = 0;

    OrgID = atol( _UI_ObjID->GetTextA() );

    GameObjDbStructEx*    OrgObjDB =    g_pGameMain->GetObjDB( OrgID );
    if( OrgObjDB == NULL )
    {
        _UI_OrgName->SetTextA( "無" );
    }
    else
    {
        _UI_OrgName->SetTextA( OrgObjDB->Name );
    }

    if(     LOrgID != OrgID 
        &&  OrgObjDB != NULL 
        &&  _GItemID == -1 )
    {        
        ObjectModeStruct  InitMode;
/*
        InitMode._Mode = 0;
        switch( OrgObjDB->Classification )
        {
        case EM_ObjectClass_NPC:
            InitMode.InitNpcMode();
            break;
        case EM_ObjectClass_Item:
            InitMode.InitItemMode();
            break;
        }
*/		
		InitMode = OrgObjDB->GetModeFlag();

        for( int i = 0 ; i < 32 ; i++ )
        {   
            if( _UI_CheckMode[i] == NULL )            
                continue;
            if( TestBit( InitMode._Mode , i ) == false )
                _UI_CheckMode[i]->SetState( true );         
            else
                _UI_CheckMode[i]->SetState( false );
        }
    }
    LOrgID = OrgID;

    for( int i = 0 ; i < 32 ; i++ )
    {
        if( _UI_CheckMode[i] == NULL )
            continue;
        if( _UI_CheckMode[i]->GetState() == true )
            continue;

        SetBit( Mode , i );
    }

    QuestID = atol( _UI_QuestID->GetTextA() );
    PID     = atol( _UI_PID->GetTextA() );

    //CreateObject OrgObjID,x,y,z,dir,Count,AutoPlot,ObjName,ClassName,QuestID,Mode
    const char* NameStr;
    const char* AutoPlotStr = _UI_AutoPlot->GetTextA();
    const char* ClassNameStr = _UI_ObjClass->GetTextA();
    if( strlen( _UI_Name->GetTextA() ) == 0 )
        NameStr = _UI_OrgName->GetTextA();
    else
        NameStr = _UI_Name->GetTextA();
    
    if( strlen( NameStr ) == 0  )
        NameStr = "-";
    if( strlen( AutoPlotStr ) == 0  )
        AutoPlotStr = "-";
    if( strlen( ClassNameStr ) == 0  )
        ClassNameStr = "-";


    sprintf( Buf , "/? CreateObj %d,%s,0,0,0,0,1,%s,%s,%d,0x%x,%d" 
                    , OrgID , NameStr , AutoPlotStr , ClassNameStr ,  QuestID , Mode , PID );

    _UI_Macro->SetTextA( Buf );

}

void        CUI_MaintainFrame::SetModifyInfo( ModifyNPCInfo* Info )
{
    _GItemID = Info->GItemID;
    _UI_Title->SetTextA( "修改物件" );
    _UI_ObjID->SetTextA( IntToStr( Info->OrgObjID ) );
    _UI_OrgName->SetTextA( "" );
    _UI_GItemID->SetTextA( IntToStr( Info->GItemID ) );
    _UI_Name->SetTextA( Info->Name.Begin() );
    _UI_ObjClass->SetTextA( Info->ClassName.Begin() );
    _UI_PID->SetTextA( IntToStr( Info->PID )  );
    _UI_AutoPlot->SetTextA( Info->AutoPlot.Begin() );
    _UI_QuestID->SetTextA( IntToStr( Info->QuestID )  );
    _UI_PosX->SetTextA( IntToStr( Info->X )  );
    _UI_PosY->SetTextA( IntToStr( Info->Y )  );
    _UI_PosZ->SetTextA( IntToStr( Info->Z )  );
    _UI_PosDir->SetTextA( IntToStr( Info->Dir )  );
    
    for( int i = 0 ; i < 32 ; i++ )
    {   
        if( _UI_CheckMode[i] == NULL )            
            continue;
        if( TestBit( Info->Mode , i ) == false )
            _UI_CheckMode[i]->SetState( true );         
        else
            _UI_CheckMode[i]->SetState( false );
    }
}

void        CUI_MaintainFrame::GetModifyInfo( ModifyNPCInfo* Info )
{

    Info->GItemID = _GItemID;
    Info->OrgObjID = atol( _UI_ObjID->GetTextA() );	
    Info->X = atol( _UI_PosX->GetTextA() );
    Info->Y = atol( _UI_PosY->GetTextA() );			
    Info->Z = atol( _UI_PosZ->GetTextA());			
    Info->Dir = atol( _UI_PosDir->GetTextA() );			    
    Info->QuestID = atol( _UI_QuestID->GetTextA() );

    Info->PID = atol( _UI_PID->GetTextA() );

    Info->Name      = _UI_Name->GetTextA();
    if( Info->Name.Size() == 0 )
        Info->Name = _UI_OrgName->GetTextA();
    Info->AutoPlot  = _UI_AutoPlot->GetTextA();;
    Info->ClassName = _UI_ObjClass->GetTextA();;        

    Info->Mode = 0;
    for( int i = 0 ; i < 32 ; i++ )
    {
        if( _UI_CheckMode[i] == NULL )
            continue;
        if( _UI_CheckMode[i]->GetState() == true )
            continue;

        SetBit( Info->Mode , i );
    }

}

void        CUI_MaintainFrame::OnCreateMacro( )
{
    ItemClipboard ClpInfo;    
    ClpInfo.SetMacro( _UI_MacroName->GetTextA() , "\\interface\\Icons\\Ability_Hunter_EagleEye" , _UI_Macro->GetTextA() );

    g_pMainMenu->CopyItemClipboard( ClpInfo );
}