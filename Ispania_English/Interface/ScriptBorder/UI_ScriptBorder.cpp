#include "../../mainproc/GameMain.h"
//#include "../../netwaker_member/NetWakerClientInc.h"
//#include "Net_PK/NetCli_PKChild.h"
#include "../../netwaker_member/net_other/NetCli_OtherChild.h"
#include "../../netwaker_member/Net_Script/NetCli_Script_Child.h"
#include "../Quest//UI_QuestDetail.h"

#include "UI_ScriptBorder.h"

CUI_ScriptBorder*		g_pScriptBorder	= NULL;
//-------------------------------------------------------------------
int			LuaFunc_GetBorderPage						( lua_State *L );
int			LuaFunc_AcceptBorderQuest					( lua_State *L );
int			LuaFunc_CloseBorder							( lua_State *L );

int			LuaFunc_ClickRequestDialogButton			( lua_State *L );
int			LuaFunc_ClickServerInputDialogButton		( lua_State *L );

//-------------------------------------------------------------------
CUI_ScriptBorder::CUI_ScriptBorder( CInterface* object ) : CInterfaceBase( object )
{
	g_pScriptBorder = this;
}
// --------------------------------------------------------------------------------------
CUI_ScriptBorder::~CUI_ScriptBorder(void)
{
	g_pScriptBorder = NULL;
}
//--------------------------------------------------------------------------------------
void CUI_ScriptBorder::RegisterFunc(void)
{
	lua_State* L = GetLuaStateWorld();
	if ( L )
	{
		lua_register( L, "GetBorderPage",							LuaFunc_GetBorderPage						);
		lua_register( L, "AcceptBorderQuest",						LuaFunc_AcceptBorderQuest					);
		lua_register( L, "CloseBorder",								LuaFunc_CloseBorder							);

		lua_register( L, "ClickRequestDialogButton",				LuaFunc_ClickRequestDialogButton			);
		lua_register( L, "ClickServerInputDialogButton",			LuaFunc_ClickServerInputDialogButton		);

	}
}
//--------------------------------------------------------------------------------------
void CUI_ScriptBorder::Clear()
{
	m_vecBorderPage.clear();
	m_sTexture.clear();
	m_sTitle.clear();
	m_iBorderQuestID = 0;
}
//--------------------------------------------------------------------------------------
void CUI_ScriptBorder::ShowBorder()
{
	lua_State* L = m_interface->GetLuaState();

	if( m_iBorderQuestID == 0 )
	{
		lua_pushnumber( L, 0 );
		lua_setglobal( L, "arg1" );
	}
	else
	{
		lua_pushnumber( L, 1 );
		lua_setglobal( L, "arg1" );
	}

	lua_pushnumber( L, (lua_Number)m_vecBorderPage.size() );
	lua_setglobal( L, "arg2" );

	lua_pushstring( L, m_sTitle.c_str() );
	lua_setglobal( L, "arg3" );

	lua_pushstring( L, m_sTexture.c_str() );
	lua_setglobal( L, "arg4" );

	m_interface->SendWorldEvent( UiEvent_ShowScriptBorder );
}
//--------------------------------------------------------------------------------------
int	LuaFunc_GetBorderPage ( lua_State *L )
{
	int iPageID	= luaL_checkint( L, 1 );

	if( iPageID >= 0 && iPageID < g_pScriptBorder->m_vecBorderPage.size() )
	{
		string str = g_pQuestDetail->ReplaceKeyWord( g_pScriptBorder->m_vecBorderPage[ iPageID ] );
		lua_pushstring( L, str.c_str() );
		return 1;
	}
	return 0;
}
//--------------------------------------------------------------------------------------
int	LuaFunc_AcceptBorderQuest ( lua_State *L )
{
	if( g_pScriptBorder->m_iBorderQuestID != 0 )
	{
		CNetCli_Script_Child::AcceptBorderQuest();
	}
	return 0;
}
//--------------------------------------------------------------------------------------
int LuaFunc_CloseBorder( lua_State *L )
{
	CNetCli_Script_Child::CloseBorder();
	return 0;
}
//--------------------------------------------------------------------------------------
int	LuaFunc_ClickRequestDialogButton			( lua_State *L )
{
	int iValue	= luaL_checkint( L, 1 );

	if( NetCli_OtherChild::This->m_pfuncCliSpeakDialog == NULL )
	{
		NetCli_Other::S_DialogSelectID( iValue );
	}
	else
	{
		NetCli_OtherChild::This->m_pfuncCliSpeakDialog( iValue, NetCli_OtherChild:: m_pCliSpeakDialogContext );
	}


	
	return 0;
}
//--------------------------------------------------------------------------------------
int	LuaFunc_ClickServerInputDialogButton			( lua_State *L )
{
	int				iValue			= luaL_checkint( L, 1 );
	const char*		pszInputer		= luaL_checkstring( L, 2 );
	int				iCheckResoult	= g_pGameMain->CheckPatternString( pszInputer );

	NetCli_Other::S_DialogInput( iValue, pszInputer, iCheckResoult );
	return 0;
}

