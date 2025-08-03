#include <windows.h>
#include "../uiframe.h"
#include "../Luatable.h"
int Frame_Locked(lua_State* L);
int Frame_Unlocked(lua_State* L);
int Frame_Enable(lua_State* L);
int Frame_Disable(lua_State* L);
int Frame_IsEnable(lua_State* L);
int Frame_SetID(lua_State* L);
int Frame_GetID(lua_State* L);
//int	Frame_SetScale(lua_State* L);
//int Frame_SetAlpha(lua_State* L);

int Frame_SetLayers(lua_State* L);

int Frame_RegisterEvent(lua_State* L);
int Frame_UnregisterEvent(lua_State* L);
int Frame_RegisterForDrag(lua_State* L);
int Frame_RegisterForClicks(lua_State* L);
int Frame_SetBackdrop(lua_State* L);
int Frame_SetBackdropTileColor(lua_State* L);
int Frame_SetBackdropTileAlpha(lua_State* L);
int Frame_SetBackdropEdgeColor(lua_State* L);
int Frame_SetBackdropEdgeAlpha(lua_State* L);
int Frame_StartMoving(lua_State* L);
int Frame_StartSizing(lua_State* L);
int Frame_StopMovingOrSizing(lua_State* L);
int Frame_SetFrameStrata(lua_State* L);
int Frame_SetFrameLevel(lua_State* L);
int Frame_SetKeyboardEnable(lua_State* L);
int Frame_ResetFrameOrder(lua_State* L);
int Frame_SetParent(lua_State* L);

int Frame_SetScripts(lua_State* L);
int Frame_SetMouseEnable(lua_State* L);


// ----------------------------------------------------------------------------------
static const struct luaL_reg g_frameTable [] = {
	{"Locked",					Frame_Locked},
	{"Unlocked",				Frame_Unlocked},
	{"Enable",					Frame_Enable},
	{"Disable",					Frame_Disable},
	{"IsEnable",				Frame_IsEnable},
	{"SetID",					Frame_SetID},
	{"GetID",					Frame_GetID},
	//{"SetScale",				Frame_SetScale},
	//{"SetAlpha",				Frame_SetAlpha},
	{"SetLayers",				Frame_SetLayers},
	
	{"RegisterEvent",			Frame_RegisterEvent},
	{"UnregisterEvent",			Frame_UnregisterEvent},
	{"RegisterForDrag",			Frame_RegisterForDrag},
	{"RegisterForClicks",		Frame_RegisterForClicks},
	{"SetBackdrop",				Frame_SetBackdrop},	
	{"SetBackdropTileColor",	Frame_SetBackdropTileColor},
	{"SetBackdropTileAlpha",	Frame_SetBackdropTileAlpha},
	{"SetBackdropEdgeColor",	Frame_SetBackdropEdgeColor},
	{"SetBackdropEdgeAlpha",	Frame_SetBackdropEdgeAlpha},
	{"StartMoving",				Frame_StartMoving},
	{"StartSizing",				Frame_StartSizing},
	{"StopMovingOrSizing",		Frame_StopMovingOrSizing},
	{"SetFrameStrata",			Frame_SetFrameStrata},
	{"SetFrameLevel",			Frame_SetFrameLevel},
	{"SetKeyboardEnable",		Frame_SetKeyboardEnable},
	{"ResetFrameOrder",			Frame_ResetFrameOrder},
	{"SetParent",				Frame_SetParent},
	{"SetScripts",				Frame_SetScripts},
	{"SetMouseEnable",			Frame_SetMouseEnable},


	
	
	{NULL, NULL}
};

// ----------------------------------------------------------------------------------
void CUiLuaFrame::OpenLib(lua_State* L)
{
	luaL_newmetatable(L, UI_LUA_TYPE_FRAME);			// create new metatable
	luaL_getmetatable(L, UI_LUA_TYPE_LAYOUT);			// inherit layout
	lua_setmetatable(L, -2);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);
	luaL_openlib(L, NULL, g_frameTable, 0);
}

// ----------------------------------------------------------------------------------
int	CUiLuaFrame::SetMetaTable(lua_State* L)
{
	if (lua_istable(L, -1))
	{
		luaL_getmetatable(L, UI_LUA_TYPE_FRAME);
		lua_setmetatable(L, -2);
		return 1;
	}
	return 0;
}

// ----------------------------------------------------------------------------------
int Frame_Locked(lua_State* L)
{
	CUiFrame* frame = dynamic_cast<CUiFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (frame == NULL)
		lua_error(L);

	frame->Locked();	
	return 0;
}

// ----------------------------------------------------------------------------------
int Frame_Unlocked(lua_State* L)
{
	CUiFrame* frame = dynamic_cast<CUiFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (frame == NULL)
		lua_error(L);

	frame->Unlocked();
	return 0;
}

// ----------------------------------------------------------------------------------
int Frame_Enable(lua_State* L)
{
	CUiFrame* frame = dynamic_cast<CUiFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (frame == NULL)
		lua_error(L);

	frame->SetEnable(true);
	return 0;
}

// ----------------------------------------------------------------------------------
int Frame_Disable(lua_State* L)
{
	CUiFrame* frame = dynamic_cast<CUiFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (frame == NULL)
		lua_error(L);

	frame->SetEnable(false);
	return 0;
}

// ----------------------------------------------------------------------------------
int Frame_IsEnable(lua_State* L)
{
	CUiFrame* frame = dynamic_cast<CUiFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (frame == NULL)
		lua_error(L);

	lua_pushboolean(L, frame->IsEnable());
	return 1;
}

// ----------------------------------------------------------------------------------
int Frame_SetID(lua_State* L)
{
	CUiFrame* frame = dynamic_cast<CUiFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (frame == NULL)
		lua_error(L);

	frame->SetID(luaL_checkint(L, 2));
	return 0;
}

// ----------------------------------------------------------------------------------
int Frame_GetID(lua_State* L)
{
	CUiFrame* frame = dynamic_cast<CUiFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (frame == NULL)
		lua_error(L);

	lua_pushnumber(L, frame->GetID());
	return 1;
}

/*
// ----------------------------------------------------------------------------------
int	Frame_SetScale(lua_State* L)
{
	CUiFrame* frame = dynamic_cast<CUiFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (frame == NULL)
		lua_error(L);

	frame->SetScale( (float)luaL_checknumber(L, 2) );
	return 0;
}
*/

/*
// ----------------------------------------------------------------------------------
int Frame_SetAlpha(lua_State* L)
{
	CUiFrame* frame = dynamic_cast<CUiFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (frame == NULL)
		lua_error(L);

	frame->SetAlpha( (float)luaL_checknumber(L, 2) );
	return 0;
}
*/

// ----------------------------------------------------------------------------------
int Frame_RegisterForDrag(lua_State* L)
{
	int size = lua_gettop(L);
	CUiFrame* frame = dynamic_cast<CUiFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (frame == NULL)
		lua_error(L);
	
	int flag = 0;	
	for ( int i = 2; i <= size; i++ )
	{
		const char* key = luaL_checkstring(L, i);
		if (strcmp(key, "LeftButton") == 0)
			flag |= CUiFrame::eMouse_Flag_LButton;
		else if (strcmp(key, "RightButton") == 0)
			flag |= CUiFrame::eMouse_Flag_RButton;
		else if (strcmp(key, "MiddleButton") == 0)
			flag |= CUiFrame::eMouse_Flag_MButton;
		else if (strcmp(key, "OtherButton") == 0)
			flag |= CUiFrame::eMouse_Flag_XButton;
		else
			lua_error(L);
	}

	frame->RegisterForDrag(flag);
	return 0;
}

// ----------------------------------------------------------------------------------
int Frame_RegisterForClicks(lua_State* L)
{
	CUiFrame* frame = dynamic_cast<CUiFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (frame == NULL)
		lua_error(L);

	int flag = 0;
	int size = lua_gettop(L);
	for ( int i = 2; i <= size; i++ )
	{
		const char* key = luaL_checkstring(L, i);
		if (strcmp(key, "LeftButton") == 0)
			flag |= CUiFrame::eMouse_Flag_LButton;
		else if (strcmp(key, "RightButton") == 0)
			flag |= CUiFrame::eMouse_Flag_RButton;
		else if (strcmp(key, "MiddleButton") == 0)
			flag |= CUiFrame::eMouse_Flag_MButton;
		else if (strcmp(key, "OtherButton") == 0)
			flag |= CUiFrame::eMouse_Flag_XButton;
		else
		{
			const char* name = frame->GetName();
			if (name == NULL) name = "";
			luaL_error(L, "[%s]RegisterForClicks argument %s is invalid", name, key);
		}
	}

	frame->RegisterForClicks(flag);
	return 0;
}

// ----------------------------------------------------------------------------------
int Frame_RegisterEvent(lua_State* L)
{
	CUiFrame* frame = dynamic_cast<CUiFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (frame == NULL)
		lua_error(L);

	frame->RegisterEvent(luaL_checkstring(L, 2));
	return 0;
}

// ----------------------------------------------------------------------------------
int Frame_UnregisterEvent(lua_State* L)
{
	CUiFrame* frame = dynamic_cast<CUiFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (frame == NULL)
		lua_error(L);

	frame->UnregisterEvent(luaL_checkstring(L, 2));
	return 0;
}
int Frame_SetLayers(lua_State* L)
{
	CUiFrame* frame = dynamic_cast<CUiFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	int layer=luaL_checkint(L, 2);
	CUiLayout* layout = dynamic_cast<CUiLayout*>( CUiLuaLayout::CheckLayoutTable(L, 3) );
	if (frame && layout )
	{

		frame->SetLayers(layer,layout);
		

	}
	else
		lua_error(L);


	return 0;
}
int Frame_SetBackdrop(lua_State* L)
{
	CUiFrame* frame = dynamic_cast<CUiFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (frame == NULL)
		lua_error(L);
	CLuaTable backdroptab(L);


	CTextureBackdrop backdrop;


	backdrop.SetEdgeFileName(backdroptab.GetString("edgeFile","Interface\\Tooltips\\Tooltip-Background").c_str());
	backdrop.SetTileFileName(backdroptab.GetString("bgFile","Interface\\Tooltips\\Tooltip-Border").c_str());


	CRuRectangle rc;
	{
		CLuaTable rcTab=backdroptab.SubTable("BackgroundInsets");
		rc.m_left	= (float)rcTab.GetInt("left",0);
		rc.m_right	= (float)rcTab.GetInt("right",0);
		rc.m_top	= (float)rcTab.GetInt("top",0);
		rc.m_bottom	= (float)rcTab.GetInt("bottom",0);
		backdrop.SetBackgroundOffset(rc);

	}
	backdrop.SetEdgeSize((float)backdroptab.GetInt("EdgeSize",16));
	backdrop.SetTileSize((float)backdroptab.GetInt("TileSize",16));
	

	frame->SetBackdrop(&backdrop);
	return 0;
}
// ----------------------------------------------------------------------------------
int Frame_SetBackdropTileColor(lua_State* L)
{
	CUiFrame* frame = dynamic_cast<CUiFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (frame == NULL)
		lua_error(L);

	CUiColor color = CUiLuaLayout::GetColor(L, 2);
	frame->SetBackdropTileColor( CUiRGBA(color.GetColor()) );
	return 0;
}

// ----------------------------------------------------------------------------------
int Frame_SetBackdropTileAlpha(lua_State* L)
{
	CUiFrame* frame = dynamic_cast<CUiFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (frame == NULL)
		lua_error(L);

	if ( lua_isnumber(L, 2) )
		frame->SetBackdropTileAlpha((float)lua_tonumber(L, 2));
	return 0;
}

// ----------------------------------------------------------------------------------
int Frame_SetBackdropEdgeColor(lua_State* L)
{
	CUiFrame* frame = dynamic_cast<CUiFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (frame == NULL)
		lua_error(L);

	CUiColor color = CUiLuaLayout::GetColor(L, 2);
	frame->SetBackdropEdgeColor( CUiRGBA(color.GetColor()) );
	return 0;
}

// ----------------------------------------------------------------------------------
int Frame_SetBackdropEdgeAlpha(lua_State* L)
{
	CUiFrame* frame = dynamic_cast<CUiFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (frame == NULL)
		lua_error(L);

	if ( lua_isnumber(L, 2) )
		frame->SetBackdropEdgeAlpha((float)lua_tonumber(L, 2));
	return 0;
}

// ----------------------------------------------------------------------------------
int Frame_StartMoving(lua_State* L)
{
	CUiFrame* frame = dynamic_cast<CUiFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (frame == NULL)
		lua_error(L);
	frame->StartMoving();
	return 0;
}

// ----------------------------------------------------------------------------------
int Frame_StartSizing(lua_State* L)
{
	CUiFrame* frame = dynamic_cast<CUiFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (frame == NULL)
		lua_error(L);	
	frame->StartSizing( CUiAnchor::GetAnchorPoint(luaL_checkstring(L, 2)) );
	return 0;
}

// ----------------------------------------------------------------------------------
int Frame_StopMovingOrSizing(lua_State* L)
{
	CUiFrame* frame = dynamic_cast<CUiFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (frame == NULL)
		lua_error(L);
	frame->StopMovingOrSizing();
	return 0;
}

// ----------------------------------------------------------------------------------
int Frame_SetFrameStrata(lua_State* L)
{
	CUiFrame* frame = dynamic_cast<CUiFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (frame == NULL)
		lua_error(L);
	else
	{
		const char* strata = luaL_checkstring(L, 2);
		if ( strata )
		{
			if ( strcmp(strata, "BACKGROUND") == 0 )
				frame->SetFrameStrata(eUIFrame_Strata_Background);
			else if ( strcmp(strata, "LOW") == 0 )
				frame->SetFrameStrata(eUIFrame_Strata_Low);
			else if ( strcmp(strata, "MEDIUM") == 0 )
				frame->SetFrameStrata(eUIFrame_Strata_Medium);
			else if ( strcmp(strata, "HIGH") == 0 )
				frame->SetFrameStrata(eUIFrame_Strata_High);
			else if ( strcmp(strata, "DIALOG") == 0 )
				frame->SetFrameStrata(eUIFrame_Strata_Dialog);
			else if ( strcmp(strata, "TOOLTIP") == 0 )
				frame->SetFrameStrata(eUIFrame_Strata_Tooltip);
			else
				lua_error(L);
		}
	}

	return 0;
}

// ----------------------------------------------------------------------------------
int Frame_SetFrameLevel(lua_State* L)
{
	CUiFrame* frame = dynamic_cast<CUiFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (frame == NULL)
		lua_error(L);
	frame->SetFrameLevel(luaL_checkint(L, 2));
	return 0;
}

// ----------------------------------------------------------------------------------
int Frame_SetKeyboardEnable(lua_State* L)
{
	CUiFrame* frame = dynamic_cast<CUiFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (frame == NULL)
		lua_error(L);
	frame->SetKeyboardEnable(lua_toboolean(L, 2) != 0);
	return 0;
}

// ----------------------------------------------------------------------------------
int Frame_ResetFrameOrder(lua_State* L)
{
	CUiFrame* frame = dynamic_cast<CUiFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (frame == NULL)
		lua_error(L);
	frame->AdjustFrameLevel();
	return 0;
}

// ----------------------------------------------------------------------------------
int Frame_SetParent(lua_State* L)
{
	CUiFrame* frame = dynamic_cast<CUiFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (frame == NULL)
		lua_error(L);

	frame->SetParent( dynamic_cast<CUiFrame*>( CUiLuaLayout::CheckLayoutTable(L, 2) ) );
	return 0;
}

int Frame_SetScripts(lua_State* L)
{

	CUiFrame* frame = dynamic_cast<CUiFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (frame == NULL)
		lua_error(L);
	const char * EventScriptName=luaL_checkstring(L, 2);
	int type=frame->GetScriptEvent(EventScriptName);	

	const char * func=luaL_checkstring(L, 3);

	frame->SetScripts(type,func);
	return 0;

}
int Frame_SetMouseEnable(lua_State* L)
{

	CUiFrame* frame = dynamic_cast<CUiFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (frame == NULL)
		lua_error(L);

	frame->SetMouseEnable(lua_toboolean(L, 2) != 0);
	return 0;

}

