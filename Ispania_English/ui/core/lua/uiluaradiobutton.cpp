#include <windows.h>
#include "../uiradiobutton.h"

int RadioButton_GetGroupId(lua_State* L);

// ----------------------------------------------------------------------------------
static const struct luaL_reg g_radiobuttonTable [] = {
	{"GetGroupId", RadioButton_GetGroupId},
	{NULL, NULL}
};

// ----------------------------------------------------------------------------------
void CUiLuaRadioButton::OpenLib(lua_State* L)
{
	luaL_newmetatable(L, UI_LUA_TYPE_RADIOBUTTON);		// create new metatable
	luaL_getmetatable(L, UI_LUA_TYPE_CHECKBUTTON);		// inherit layout
	lua_setmetatable(L, -2);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);
	luaL_openlib(L, NULL, g_radiobuttonTable, 0);
}

// ----------------------------------------------------------------------------------
int	CUiLuaRadioButton::SetMetaTable(lua_State* L)
{
	if (lua_istable(L, -1))
	{
		luaL_getmetatable(L, UI_LUA_TYPE_RADIOBUTTON);
		lua_setmetatable(L, -2);
		return 1;
	}
	return 0;
}

// ----------------------------------------------------------------------------------
int RadioButton_GetGroupId(lua_State* L)
{
	CUiRadioButton* radiobutton = dynamic_cast<CUiRadioButton*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (radiobutton == NULL)
		lua_error(L);
	lua_pushnumber(L, radiobutton->GetGroupId());
	return 1;
}