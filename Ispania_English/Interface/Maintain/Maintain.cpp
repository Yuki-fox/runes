#include <stdio.h>
#include "../../mainproc/GameMain.h"
#include "../../netwaker_member/NetWakerClientInc.h"
#include "../WorldFrames/ChatEdit.h"
#include "../WorldFrames/ChatFrame.h"
#include "../Worldframes/MacroFrame.h"
#include "Maintain.h"

int LuaFunc_OnMaintainFrameOnLoad(lua_State* L);
int LuaFunc_OnMaintainFrameOnAccept(lua_State* L);
int LuaFunc_OnMaintainFrameOnResetPos(lua_State* L);
int LuaFunc_OnMaintainFrameOnUpdate(lua_State* L);
int LuaFunc_OnMaintainFrameCreateMacro(lua_State* L);

CMaintain*				g_pMaintain = NULL;

// ----------------------------------------------------------------------------
CMaintain::CMaintain(CInterface* object)
	: CInterfaceBase(object)
{
	_UI_Win				= NULL;
	_UI_Title			= NULL;
	_UI_ObjID			= NULL;
	_UI_OrgName			= NULL;
	_UI_GItemID			= NULL;
	_UI_Name			= NULL;
	_UI_ObjClass		= NULL;
	_UI_PID				= NULL;
	_UI_AutoPlot		= NULL;
	_UI_QuestID			= NULL;
	_UI_PosX			= NULL;
	_UI_PosY			= NULL;
	_UI_PosZ			= NULL;
	_UI_PosDir			= NULL;
	_UI_PosReset		= NULL;	
	_UI_MacroName		= NULL;
	_UI_MacroCreate		= NULL;
	_UI_Macro			= NULL;
	_UI_Accept			= NULL;
	_UI_Cencel			= NULL;
	_GItemID			= -1;
	memset(_UI_CheckMode, 0, sizeof(_UI_CheckMode) );	

	g_pMaintain = this;
}

// ----------------------------------------------------------------------------
CMaintain::~CMaintain()
{
	g_pMaintain = NULL;
}

// ----------------------------------------------------------------------------
void CMaintain::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{
		lua_register(L, "OnLoadMaintainFrame",			LuaFunc_OnMaintainFrameOnLoad);
		lua_register(L, "OnAcceptClickMaintainFrame",	LuaFunc_OnMaintainFrameOnAccept);
		lua_register(L, "OnResetPosMaintainFrame",		LuaFunc_OnMaintainFrameOnResetPos);
		lua_register(L, "OnUpdateMaintainFrame",		LuaFunc_OnMaintainFrameOnUpdate);
		lua_register(L, "CreateMacroMaintainFrame",		LuaFunc_OnMaintainFrameCreateMacro);
	}
}

// ----------------------------------------------------------------------------
void CMaintain::OnLoad()
{
	_UI_Win         = (CUiFrame*     )m_interface->GetUiWorld()->FindObject( "MaintainInterface" );
	_UI_Title       = (CUiFontString*)m_interface->GetUiWorld()->FindObject( "MaintainInterface1Title" );
	_UI_ObjID       = (CUiEditBox*   )m_interface->GetUiWorld()->FindObject( "MT_OrgObjIDEdit" );
	_UI_OrgName     = (CUiEditBox*   )m_interface->GetUiWorld()->FindObject( "MT_OrgNameEdit" );
	_UI_GItemID     = (CUiEditBox*   )m_interface->GetUiWorld()->FindObject( "MT_GItemIDEdit" );
	_UI_Name        = (CUiEditBox*   )m_interface->GetUiWorld()->FindObject( "MT_NameEdit" );
	_UI_ObjClass    = (CUiEditBox*   )m_interface->GetUiWorld()->FindObject( "MT_PlotTypeEdit" );
	_UI_PID         = (CUiEditBox*   )m_interface->GetUiWorld()->FindObject( "MT_PIDEdit" );
	_UI_AutoPlot    = (CUiEditBox*   )m_interface->GetUiWorld()->FindObject( "MT_PlotNameEdit" );
	_UI_QuestID     = (CUiEditBox*   )m_interface->GetUiWorld()->FindObject( "MT_QuestIDEdit" );
	_UI_PosX        = (CUiEditBox*   )m_interface->GetUiWorld()->FindObject( "MT_PositionXEdit" );
	_UI_PosY        = (CUiEditBox*   )m_interface->GetUiWorld()->FindObject( "MT_PositionYEdit" );
	_UI_PosZ        = (CUiEditBox*   )m_interface->GetUiWorld()->FindObject( "MT_PositionZEdit" );
	_UI_PosDir      = (CUiEditBox*   )m_interface->GetUiWorld()->FindObject( "MT_PositionDirEdit" );
	_UI_PosReset    = (CUiButton*    )m_interface->GetUiWorld()->FindObject( "MT_SetPosition" );
	_UI_MacroName   = (CUiEditBox*   )m_interface->GetUiWorld()->FindObject( "MT_MacroNameEdit" );
	_UI_MacroCreate = (CUiButton*    )m_interface->GetUiWorld()->FindObject( "MT_CreateMacro" );
	_UI_Macro       = (CUiFontString*)m_interface->GetUiWorld()->FindObject( "MT_MacroStr" );
	_UI_Accept      = (CUiButton*    )m_interface->GetUiWorld()->FindObject( "MT_Accept" );
	_UI_Cencel      = (CUiButton*    )m_interface->GetUiWorld()->FindObject( "MT_Cencel" );

	char Buf[256];
	for( int i = 0 ; i < 32 ; i++ )
	{
		sprintf( Buf , "MT_Mode%d" , i + 1  );
		_UI_CheckMode[i] = (CUiCheckButton*)m_interface->GetUiWorld()->FindObject( Buf );
	}
}

// ----------------------------------------------------------------------------
bool CMaintain::Open()
{
	if( _UI_Win != NULL )
	{
		if( _UI_Win->IsVisible() )
			return false;

		_UI_GItemID->SetText(L"");
		_GItemID = -1;
		_UI_Win->SetVisible(true);
		OnResetPos();
		return true;
	}
	return false;
}

// ----------------------------------------------------------------------------
bool CMaintain::ModifyOpen( void*  data )
{
	ModifyNPCInfo* Info = (ModifyNPCInfo*)data;
	if( _UI_Win != NULL )
	{
		_UI_Win->SetVisible(true);

		SetModifyInfo( Info );
		return true;
	}
	return false;
}

// ----------------------------------------------------------------------------
bool CMaintain::Hide()
{
	if( _UI_Win != NULL )
	{
		_UI_Win->SetVisible(false);
		_GItemID = -1;
		return true;
	}
	return false;
}

// ----------------------------------------------------------------------------
void CMaintain::OnCreateMacro()
{
	char* CmdStr  = TransToChar( _UI_Macro->GetText() );
	char* NameStr = TransToChar( _UI_Name->GetText() );
	if( NameStr == NULL || NameStr[0] == 0 )
	{
		delete[] NameStr;

		NameStr = TransToChar( _UI_OrgName->GetText() ); 
	}
	if (CmdStr )
	{
		if ( !g_pMacroFrame->CreateMacro(NameStr , 0 , CmdStr) )
		{
			g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, "巨集已滿,無法建立", "");
		}
		
		delete [] CmdStr;
		delete [] NameStr;
		_UI_Win->SetVisible(false);
	}
}

// ----------------------------------------------------------------------------
void CMaintain::OnAcceptClick()
{
	if( _GItemID == -1 )
	{
		char* tmp = (char*)_Cmd.c_str();
		if (tmp)
		{
			g_pChatEdit->SendChatMessage(tmp, 0);
		}
	}
	else
	{
		ModifyNPCInfo Info;
		GetModifyInfo( &Info );
		Net_GSrvConnect::ModyNPC( &Info );
	}
	_UI_Win->SetVisible(false);
}

// ----------------------------------------------------------------------------
void CMaintain::OnResetPos()
{
	CRoleSprite* pSprite = g_pGameMain->GetPlayer();
	if( pSprite == NULL )
	{
		return;
	}
	CRuVector3 direction = pSprite->GetDirection();
	CRuVector3 nowPos = pSprite->GetPos();

	float theta = 0.0f;

	// 取得角色所朝方向
	theta = acos( direction.Dot( CRuVector3(1.0f, 0.0f, 0.0f) ) );
	if ( direction.m_z > 0.0f )
		theta = PI + PI - theta;

	// 轉換為角度
	theta = theta * 180.0f / PI;

	WCHAR buf[256];	
	swprintf(buf, 256, L"%d", int(nowPos.m_x));
	_UI_PosX->SetText( buf );
	swprintf(buf, 256, L"%d", int(nowPos.m_y));
	_UI_PosY->SetText( buf );
	swprintf(buf, 256, L"%d", int(nowPos.m_z));
	_UI_PosZ->SetText( buf );

	swprintf(buf, 256, L"%d", int(theta));
	_UI_PosDir->SetText( buf );


}

// ----------------------------------------------------------------------------
void CMaintain::OnUpdate()
{
	static  int LOrgID = 0;
	WCHAR   buf[1024];
	int     OrgID = 0;
	int     QuestID = 0;
	ObjectModeStruct     Mode;
	int     PID = 0;

	Mode._Mode = 0;


	char* tmp = TransToChar(_UI_ObjID->GetText());
	if (tmp)
	{
		OrgID = atol(tmp);
		delete [] tmp;
	}

	GameObjDbStructEx*    OrgObjDB =    CNetGlobal::GetObj( OrgID );
	if( OrgObjDB == NULL )
	{
		_UI_OrgName->SetTextA( "無" );
	}
	else
	{
		_UI_OrgName->SetTextA( OrgObjDB->GetName() );
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
				_UI_CheckMode[i]->SetChecked( false );
			else
				_UI_CheckMode[i]->SetChecked( true );
		}
	}
	LOrgID = OrgID;

	for( int i = 0 ; i < 32 ; i++ )
	{
		if( _UI_CheckMode[i] == NULL )
			continue;
		if( _UI_CheckMode[i]->IsChecked() == false )
			continue;

		SetBit( Mode._Mode , i );
	}

	Mode.ShowRoleHead = true;
	Mode.CollisionPlot = false;

	tmp = TransToChar(_UI_QuestID->GetText());
	if (tmp)
	{
		QuestID = atol(tmp);
		delete [] tmp;

	}
	tmp = TransToChar(_UI_PID->GetText());
	if (tmp)
	{
		PID = atol(tmp);
		delete tmp;
	}

	//CreateObject OrgObjID,x,y,z,dir,Count,AutoPlot,ObjName,ClassName,QuestID,Mode
	const WCHAR* NameStr;
	const WCHAR* AutoPlotStr = _UI_AutoPlot->GetText();
	const WCHAR* ClassNameStr = _UI_ObjClass->GetText();
	if (wcslen(_UI_Name->GetText()) == 0)
		NameStr = _UI_OrgName->GetText();
	else
		NameStr = _UI_Name->GetText();

	if (wcslen(_UI_Name->GetText()) == 0)
		NameStr = L"-";
	if (wcslen(AutoPlotStr) == 0)
		AutoPlotStr = L"-";
	if (wcslen(ClassNameStr) == 0)
		ClassNameStr = L"-";
	
	swprintf( buf, 1024, L"/cmd ? CreateObj %d,%s,0,0,0,0,1,%s,%s,%d,0x%x,%d" 
		, OrgID , NameStr , AutoPlotStr , ClassNameStr ,  QuestID , Mode , PID );

	_UI_Macro->SetText( buf );

	swprintf( buf, 1024, L"/? CreateObj %d,%s,%s,%s,%s,%s,1,%s,%s,%d,0x%x,%d" 
		, OrgID , NameStr 
		, _UI_PosX->GetText() , _UI_PosY->GetText() , _UI_PosZ->GetText() , _UI_PosDir->GetText()
		, AutoPlotStr , ClassNameStr ,  QuestID , Mode , PID );

	tmp = WCharToUFT8( buf );
	if (tmp)
	{
		_Cmd = tmp;
		delete tmp;
	}	
}

// ----------------------------------------------------------------------------
void CMaintain::SetModifyInfo( void* data )
{
	ModifyNPCInfo* Info = (ModifyNPCInfo*)data;

	_GItemID = Info->GItemID;
//	_UI_Title->SetTextA( "修改物件" );
	_UI_ObjID->SetTextA( IntToStr( Info->OrgObjID ) );
	_UI_OrgName->SetTextA( "" );
	_UI_GItemID->SetTextA( IntToStr( Info->GItemID ) );
	_UI_Name->SetTextA( Info->Name.Begin() );
	_UI_ObjClass->SetTextA( Info->ClassName.Begin() );
	_UI_PID->SetTextA( IntToStr( Info->PID )  );
	_UI_AutoPlot->SetTextA( Info->AutoPlot.Begin() );
	_UI_QuestID->SetTextA( IntToStr( Info->QuestID )  );
	_UI_PosX->SetTextA( IntToStr( int(Info->X) )  );
	_UI_PosY->SetTextA( IntToStr( int(Info->Y) )  );
	_UI_PosZ->SetTextA( IntToStr( int(Info->Z) )  );
	_UI_PosDir->SetTextA( IntToStr( int(Info->Dir) )  );

	for( int i = 0 ; i < 32 ; i++ )
	{   
		if( _UI_CheckMode[i] == NULL )            
			continue;
		if( TestBit( Info->Mode , i ) == false )
			_UI_CheckMode[i]->SetChecked( false );         
		else
			_UI_CheckMode[i]->SetChecked( true );
	}
}

// ----------------------------------------------------------------------------
void CMaintain::GetModifyInfo( void* data )
{
	ObjectModeStruct     Mode;
	ModifyNPCInfo* Info = (ModifyNPCInfo*)data;
	
	Info->vX = Info->vY = Info->vZ = 0;
	Info->GItemID = _GItemID;
	Info->OrgObjID = _wtol( _UI_ObjID->GetText() );
	Info->X = (float)_wtof( _UI_PosX->GetText() );
	Info->Y = (float)_wtof( _UI_PosY->GetText() );
	Info->Z = (float)_wtof( _UI_PosZ->GetText());
	Info->Dir = (float)_wtof( _UI_PosDir->GetText() );
	Info->QuestID = _wtol( _UI_QuestID->GetText() );

	Info->PID = _wtol( _UI_PID->GetText() );

	char* name		= TransToChar(_UI_Name->GetText());	
	char* autoPlot	= TransToChar(_UI_AutoPlot->GetText());
	char* className	= TransToChar(_UI_ObjClass->GetText());

	if (name && name[0])
		Info->Name = name;
	else
	{
		char* tmp = TransToChar(_UI_OrgName->GetText());
		Info->Name = tmp;
		delete [] tmp;
	}
	Info->AutoPlot	= autoPlot;
	Info->ClassName	= className;
	Info->Mode		= 0;

	for( int i = 0 ; i < 32 ; i++ )
	{
		if( _UI_CheckMode[i] == NULL )
			continue;
		if( _UI_CheckMode[i]->IsChecked() == false )
			continue;

		SetBit( Mode._Mode , i );
	}

	Mode.ShowRoleHead = true;
	Mode.CollisionPlot = false;
	
	Info->Mode = Mode._Mode;

	if (name) delete [] name;
	if (autoPlot) delete [] autoPlot;
	if (className) delete [] className;
}

// ----------------------------------------------------------------------------
int LuaFunc_OnMaintainFrameOnLoad(lua_State* L)
{
	g_pMaintain->OnLoad();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_OnMaintainFrameOnAccept(lua_State* L)
{
	g_pMaintain->OnAcceptClick();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_OnMaintainFrameOnResetPos(lua_State* L)
{
	g_pMaintain->OnResetPos();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_OnMaintainFrameOnUpdate(lua_State* L)
{
	g_pMaintain->OnUpdate();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_OnMaintainFrameCreateMacro(lua_State* L)
{
	g_pMaintain->OnCreateMacro();
	return 0;
}