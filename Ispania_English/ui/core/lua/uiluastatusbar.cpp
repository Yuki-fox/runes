#include <windows.h>
#include "../uistatusbar.h"

int StatusBar_SetMaxValue(lua_State* L);
int StatusBar_GetMaxValue(lua_State* L);
int StatusBar_SetMinValue(lua_State* L);
int StatusBar_GetMinValue(lua_State* L);
int StatusBar_SetValue(lua_State* L);
int StatusBar_GetValue(lua_State* L);
int StatusBar_SetBarColor(lua_State* L);
int StatusBar_GetBarColor(lua_State* L);

// ----------------------------------------------------------------------------------
static const struct luaL_reg g_statusbarTable [] = {
	{"SetMaxValue",			StatusBar_SetMaxValue},
	{"GetMaxValue",			StatusBar_GetMaxValue},
	{"SetMinValue",			StatusBar_SetMinValue},
	{"GetMinValue",			StatusBar_GetMinValue},	
	{"SetValue",			StatusBar_SetValue},
	{"GetValue",			StatusBar_GetValue},
	{"SetBarColor",			StatusBar_SetBarColor},
	{"GetBarColor",			StatusBar_GetBarColor},
	{NULL, NULL}
};

// ----------------------------------------------------------------------------------
void CUiLuaStatusBar::OpenLib(lua_State* L)
{
	luaL_newmetatable(L, UI_LUA_TYPE_STATUSBAR);		// create new metatable
	luaL_getmetatable(L, UI_LUA_TYPE_FRAME);			// inherit layout
	lua_setmetatable(L, -2);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);
	luaL_openlib(L, NULL, g_statusbarTable, 0);
}

// ----------------------------------------------------------------------------------
int	CUiLuaStatusBar::SetMetaTable(lua_State* L)
{
	if (lua_istable(L, -1))
	{
		luaL_getmetatable(L, UI_LUA_TYPE_STATUSBAR);
		lua_setmetatable(L, -2);
		return 1;
	}
	return 0;
}

// ----------------------------------------------------------------------------------
int StatusBar_SetMaxValue(lua_State* L)
{
	CUiStatusBar* statusbar = dynamic_cast<CUiStatusBar*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (statusbar == NULL)
		lua_error(L);

	statusbar->SetMaxValue((float)luaL_checknumber(L, 2));
	return 0;
}

// ----------------------------------------------------------------------------------
int StatusBar_GetMaxValue(lua_State* L)
{
	CUiStatusBar* statusbar = dynamic_cast<CUiStatusBar*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (statusbar == NULL)
		lua_error(L);

	lua_pushnumber(L, statusbar->GetMaxValue());
	return 1;
}

// ----------------------------------------------------------------------------------
int StatusBar_SetMinValue(lua_State* L)
{
	CUiStatusBar* statusbar = dynamic_cast<CUiStatusBar*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (statusbar == NULL)
		lua_error(L);

	statusbar->SetMinValue((float)luaL_checknumber(L, 2));
	return 0;
}

// ----------------------------------------------------------------------------------
int StatusBar_GetMinValue(lua_State* L)
{
	CUiStatusBar* statusbar = dynamic_cast<CUiStatusBar*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (statusbar == NULL)
		lua_error(L);

	lua_pushnumber(L, statusbar->GetMinValue());
	return 1;
}

// ----------------------------------------------------------------------------------
int StatusBar_SetValue(lua_State* L)
{
	CUiStatusBar* statusbar = dynamic_cast<CUiStatusBar*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (statusbar == NULL)
		lua_error(L);

	statusbar->SetValue((float)luaL_checknumber(L, 2));
	return 0;
}

// ----------------------------------------------------------------------------------
int StatusBar_GetValue(lua_State* L)
{
	CUiStatusBar* statusbar = dynamic_cast<CUiStatusBar*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (statusbar == NULL)
		lua_error(L);

	lua_pushnumber(L, statusbar->GetValue());
	return 1;
}

// ----------------------------------------------------------------------------------
int StatusBar_SetBarColor(lua_State* L)
{
	CUiStatusBar* statusbar = dynamic_cast<CUiStatusBar*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (statusbar == NULL)
		lua_error(L);

	CUiColor color = CUiLuaLayout::GetColor(L, 2);
	statusbar->SetBarColor(color);
	return 0;
}

// ----------------------------------------------------------------------------------
int StatusBar_GetBarColor(lua_State* L)
{
	CUiStatusBar* statusbar = dynamic_cast<CUiStatusBar*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (statusbar == NULL)
		lua_error(L);

	CUiColor color = statusbar->GetBarColor();
	lua_pushnumber(L, color.m_r);
	lua_pushnumber(L, color.m_g);
	lua_pushnumber(L, color.m_b);
	lua_pushnumber(L, color.m_a);
	return 4;
}