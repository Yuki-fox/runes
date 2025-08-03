#include <windows.h>
#include "../uicheckbutton.h"

int CheckButton_GetCheckedTexture(lua_State* L);
int CheckButton_GetDisabledTexture(lua_State* L);
int CheckButton_SetChecked(lua_State* L);
int CheckButton_IsChecked(lua_State* L);

// ----------------------------------------------------------------------------------
static const struct luaL_reg g_checkbuttonTable [] = {
	{"GetCheckedTexture", CheckButton_GetCheckedTexture},
	{"GetDisabledCheckTexture", CheckButton_GetDisabledTexture},
	{"SetChecked", CheckButton_SetChecked},
	{"IsChecked", CheckButton_IsChecked},
	{NULL, NULL}
};

// ----------------------------------------------------------------------------------
void CUiLuaCheckButton::OpenLib(lua_State* L)
{
	luaL_newmetatable(L, UI_LUA_TYPE_CHECKBUTTON);		// create new metatable
	luaL_getmetatable(L, UI_LUA_TYPE_BUTTON);			// inherit layout
	lua_setmetatable(L, -2);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);
	luaL_openlib(L, NULL, g_checkbuttonTable, 0);
}

// ----------------------------------------------------------------------------------
int	CUiLuaCheckButton::SetMetaTable(lua_State* L)
{
	if (lua_istable(L, -1))
	{
		luaL_getmetatable(L, UI_LUA_TYPE_CHECKBUTTON);
		lua_setmetatable(L, -2);
		return 1;
	}
	return 0;
}

// ----------------------------------------------------------------------------------
int CheckButton_GetCheckedTexture(lua_State* L)
{
	CUiCheckButton* checkbutton = dynamic_cast<CUiCheckButton*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (checkbutton == NULL)
		lua_error(L);
	CUiLuaLayout::PushLayoutTable(L, checkbutton->GetCheckedTexture());
	return 1;
}

// ----------------------------------------------------------------------------------
int CheckButton_GetDisabledTexture(lua_State* L)
{
	CUiCheckButton* checkbutton = dynamic_cast<CUiCheckButton*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (checkbutton == NULL)
		lua_error(L);
	CUiLuaLayout::PushLayoutTable(L, checkbutton->GetDisabledCheckedTexture());
	return 1;
}

// ----------------------------------------------------------------------------------
int CheckButton_SetChecked(lua_State* L)
{
	CUiCheckButton* checkbutton = dynamic_cast<CUiCheckButton*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (checkbutton == NULL)
		lua_error(L);
	checkbutton->SetChecked(lua_toboolean(L, 2) != 0);
	return 0;
}

// ----------------------------------------------------------------------------------
int CheckButton_IsChecked(lua_State* L)
{
	CUiCheckButton* checkbutton = dynamic_cast<CUiCheckButton*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (checkbutton == NULL)
		lua_error(L);
	lua_pushboolean(L, checkbutton->IsChecked());	
	return 1;
}