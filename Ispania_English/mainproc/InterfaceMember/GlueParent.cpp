#include "GlueParent.h"

// --------------------------------------------------------------------------------------
// Globals
CGlueParent*						g_pGlueParent = NULL;

// --------------------------------------------------------------------------------------
void CGlueParent::Initial()
{
	if ( g_pGlueParent == NULL )
	{
		g_pGlueParent = new CGlueParent;

		CFrame::RegisterEventString( _Def_UIEvent_SetGlueScreen_ );
	}
}

// --------------------------------------------------------------------------------------
void CGlueParent::Release()
{
	SAFE_DELETE( g_pGlueParent );
}

// --------------------------------------------------------------------------------------
CGlueParent::CGlueParent()
{
}

// --------------------------------------------------------------------------------------
CGlueParent::~CGlueParent()
{
}

// --------------------------------------------------------------------------------------
void RegisterGlueParentFunc()
{
	g_pLuaScript->Register("QuitGame"				, QuitGame);	
	g_pLuaScript->Register("getglobal"				, getglobal);
	g_pLuaScript->Register("GetCursorPos"			, GetCursorPos);
	g_pLuaScript->Register("GetOldCursorPos"		, GetOldCursorPos);
	g_pLuaScript->Register("SetCurrentScreen"		, SetCurrentScreen);
	g_pLuaScript->Register("GetBackPackageMoney"	, GetBackPackageMoney);
}

// --------------------------------------------------------------------------------------
int QuitGame(lua_State *L)
{
	g_pGameMain->Exit();
	return 0;
}

// --------------------------------------------------------------------------------------
int getglobal(lua_State *L)
{
	luaStack l(L);
	if ( l.GetParamCount() >= 1 )
	{
		CLayout* pLayout = CLayout::GetLayout( l.GetParam<const char*>(1) );
		if ( pLayout )
		{
			switch ( pLayout->GetType() )
			{
			case eLT_FontString:
				l.Push( (CFontString*)pLayout );
				break;
			case eLT_Texture:
				l.Push( (CTexture*)pLayout );
				break;
			case eLT_Frame:
				l.Push( (CFrame*)pLayout );
				break;
			case eLT_Model:
				l.Push( (CModel*)pLayout );
				break;
			case eLT_Button:
				l.Push( (CButton*)pLayout );
				break;
			case eLT_CheckButton:
				l.Push( (CCheckButton*)pLayout );
				break;
			case eLT_RadioButton:
				l.Push( (CRadioButton*)pLayout );
				break;
			case eLT_StatusBar:
				l.Push( (CStatusBar*)pLayout );
				break;
			case eLT_Slider:
				l.Push( (CSlider*)pLayout );
				break;
			case eLT_EditBox:
				l.Push( (CEditBox*)pLayout );
				break;
			case eLT_MessageFrame:
				l.Push( (CMessageFrame*)pLayout );
				break;
			}

			return 1;
		}
	}

	return 0;
}

// --------------------------------------------------------------------------------------
int	GetCursorPos(lua_State *L)
{
	luaStack l(L);
	CInput* pInput = CFrame::GetInput();
	l.Push( pInput->GetXPos() );
	l.Push( pInput->GetYPos() );
	return 2;
}

// --------------------------------------------------------------------------------------
int	GetOldCursorPos(lua_State *L)
{
	luaStack l(L);
	CInput* pInput = CFrame::GetInput();
	l.Push( pInput->GetXOldPos() );
	l.Push( pInput->GetYOldPos() );
	return 2;
}

// --------------------------------------------------------------------------------------
int	SetCurrentScreen(lua_State *L)
{
	luaStack l(L);
	if ( l.GetParamCount() >= 1 )
		g_pGlueParent->SetCurrentScreen( l.GetParam<const char*>(1) );
	return 0;
}
// --------------------------------------------------------------------------------------
int GetBackPackageMoney(lua_State *L)
{
	luaStack l(L);

	RoleDataEx* Role = g_pGameMain->RoleData();
	if ( Role )
	{
		int gold = Role->BaseData.Body.Money / 10000;
		int sliver = (Role->BaseData.Body.Money % 10000) / 100;
		int copper = (Role->BaseData.Body.Money % 100);
		l.Push( gold );
		l.Push( sliver );
		l.Push( copper );
		return 3;
	}
	return 0;
}