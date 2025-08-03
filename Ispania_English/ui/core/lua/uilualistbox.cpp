#include <windows.h>
#include "../uilistbox.h"

int ListBox_Clear(lua_State* L);
int ListBox_InsertItem(lua_State* L);
int ListBox_RemoveItem(lua_State* L);
int ListBox_SetTopItem(lua_State* L);
int ListBox_GetTopItem(lua_State* L);
int ListBox_GetItemSize(lua_State* L);
int ListBox_SetItemChecked(lua_State* L);
int ListBox_IsItemChecked(lua_State* L);
int ListBox_GetCurrentItem(lua_State* L);
int ListBox_SetItemHeight(lua_State* L);
int ListBox_GetItemHeight(lua_State* L);

// ----------------------------------------------------------------------------------
static const struct luaL_reg g_listboxTable [] = {	
	{"Clear",				ListBox_Clear},
	{"InsertItem",			ListBox_InsertItem},
	{"RemoveItem",			ListBox_RemoveItem},
	{"SetTopItem",			ListBox_SetTopItem},
	{"GetTopItem",			ListBox_GetTopItem},
	{"GetItemSize",			ListBox_GetItemSize},
	{"SetItemChecked",		ListBox_SetItemChecked},
	{"IsItemChecked",		ListBox_IsItemChecked},
	{"GetCurrentItem",		ListBox_GetCurrentItem},
	{"SetItemHeight",		ListBox_SetItemHeight},
	{"GetItemHeight",		ListBox_GetItemHeight},
	{NULL, NULL}
};

// ----------------------------------------------------------------------------------
void CUiLuaListBox::OpenLib(lua_State* L)
{
	luaL_newmetatable(L, UI_LUA_TYPE_LISTBOX);			// create new metatable
	luaL_getmetatable(L, UI_LUA_TYPE_FRAME);			// inherit layout
	lua_setmetatable(L, -2);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);
	luaL_openlib(L, NULL, g_listboxTable, 0);
}

// ----------------------------------------------------------------------------------
int	CUiLuaListBox::SetMetaTable(lua_State* L)
{
	if (lua_istable(L, -1))
	{
		luaL_getmetatable(L, UI_LUA_TYPE_LISTBOX);
		lua_setmetatable(L, -2);
		return 1;
	}
	return 0;
}

// ----------------------------------------------------------------------------------
int ListBox_Clear(lua_State* L)
{
	CUiListBox* listbox = dynamic_cast<CUiListBox*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (listbox == NULL)
		lua_error(L);

	listbox->ClearAllItem();
	return 0;
}

// ----------------------------------------------------------------------------------
// arg1 項目字串
// arg2 插入位置,有預設值可以不填(0插入最前頭, -1插入最後一筆)
int ListBox_InsertItem(lua_State* L)
{
	CUiListBox* listbox = dynamic_cast<CUiListBox*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (listbox == NULL)
		lua_error(L);
	
	const char* text = luaL_checkstring(L, 2);
	if (text)
	{
		int pos = -1;		
		if (lua_isnumber(L, 3))
			pos = (int)lua_tonumber(L, 3);

		WCHAR* tmp = TransToWChar(text);
		listbox->InsertItem(pos, tmp);
		UI_SAFE_DELETE_ARRAY(tmp);
	}

	return 0;
}

// ----------------------------------------------------------------------------------
// arg1 要刪除項目索引
int ListBox_RemoveItem(lua_State* L)
{
	CUiListBox* listbox = dynamic_cast<CUiListBox*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (listbox == NULL)
		lua_error(L);

	listbox->RemoveItem(luaL_checkint(L, 2));
	return 0;
}

// ----------------------------------------------------------------------------------
int ListBox_SetTopItem(lua_State* L)
{
	CUiListBox* listbox = dynamic_cast<CUiListBox*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (listbox == NULL)
		lua_error(L);

	listbox->SetTopItem(luaL_checkint(L, 2));
	return 0;
}

// ----------------------------------------------------------------------------------
int ListBox_GetTopItem(lua_State* L)
{
	CUiListBox* listbox = dynamic_cast<CUiListBox*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (listbox == NULL)
		lua_error(L);

	lua_pushnumber(L, listbox->GetTopItem());
	return 1;
}

// ----------------------------------------------------------------------------------
int ListBox_GetItemSize(lua_State* L)
{
	CUiListBox* listbox = dynamic_cast<CUiListBox*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (listbox == NULL)
		lua_error(L);

	lua_pushnumber(L, listbox->GetListSize());
	return 1;
}

// ----------------------------------------------------------------------------------
// 
int ListBox_SetItemChecked(lua_State* L)
{
	CUiListBox* listbox = dynamic_cast<CUiListBox*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (listbox == NULL)
		lua_error(L);

	luaL_checktype(L, 3, LUA_TBOOLEAN);	
	listbox->SetItemChecked(luaL_checkint(L, 2), lua_toboolean(L, 3)!=0);
	return 0;
}

// ----------------------------------------------------------------------------------
int ListBox_IsItemChecked(lua_State* L)
{
	CUiListBox* listbox = dynamic_cast<CUiListBox*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (listbox == NULL)
		lua_error(L);

	lua_pushboolean(L, listbox->IsItemChecked(luaL_checkint(L, 2)));
	return 1;
}

// ----------------------------------------------------------------------------------
int ListBox_GetCurrentItem(lua_State* L)
{
	CUiListBox* listbox = dynamic_cast<CUiListBox*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (listbox == NULL)
		lua_error(L);

	lua_pushnumber(L, listbox->GetCurrentItem());
	return 1;
}

// ----------------------------------------------------------------------------------
int ListBox_SetItemHeight(lua_State* L)
{
	CUiListBox* listbox = dynamic_cast<CUiListBox*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (listbox == NULL)
		lua_error(L);

	listbox->SetItemHeight((float)luaL_checknumber(L, 2));
	return 0;
}

// ----------------------------------------------------------------------------------
int ListBox_GetItemHeight(lua_State* L)
{
	CUiListBox* listbox = dynamic_cast<CUiListBox*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (listbox == NULL)
		lua_error(L);

	lua_pushnumber(L, listbox->GetItemHeight());
	return 1;
}