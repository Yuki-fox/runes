#include <windows.h>
#include "../uiscrollframe.h"

int ScrollFrame_SetVerticalScroll		(lua_State* L);
int ScrollFrame_GetVerticalScroll		(lua_State* L);
int ScrollFrame_SetHorizontalScroll		(lua_State* L);
int ScrollFrame_GetHorizontalScroll		(lua_State* L);
int ScrollFrame_SetScrollRange			(lua_State* L);
int ScrollFrame_GetScrollRange			(lua_State* L);
int ScrollFrame_UpdateScrollChildRect	(lua_State* L);

// ----------------------------------------------------------------------------------
static const struct luaL_reg g_scrollframeTable [] = {
	{"SetVerticalScroll",		ScrollFrame_SetVerticalScroll},
	{"GetVerticalScroll",		ScrollFrame_GetVerticalScroll},
	{"SetHorizontalScroll",		ScrollFrame_SetHorizontalScroll},
	{"GetHorizontalScroll",		ScrollFrame_GetHorizontalScroll},
	{"SetScrollRange",			ScrollFrame_SetScrollRange},
	{"GetScrollRange",			ScrollFrame_GetScrollRange},
	{"UpdateScrollChildRect",	ScrollFrame_UpdateScrollChildRect},
	{NULL, NULL}
};

// ----------------------------------------------------------------------------------
void CUiLuaScrollFrame::OpenLib(lua_State* L)
{
	luaL_newmetatable(L, UI_LUA_TYPE_SCROLLFRAME);		// create new metatable
	luaL_getmetatable(L, UI_LUA_TYPE_FRAME);			// inherit layout
	lua_setmetatable(L, -2);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);
	luaL_openlib(L, NULL, g_scrollframeTable, 0);
}

// ----------------------------------------------------------------------------------
int CUiLuaScrollFrame::SetMetaTable(lua_State* L)
{
	if (lua_istable(L, -1))
	{
		luaL_getmetatable(L, UI_LUA_TYPE_SCROLLFRAME);
		lua_setmetatable(L, -2);
		return 1;
	}
	return 0;
}

// ----------------------------------------------------------------------------------
int ScrollFrame_SetVerticalScroll(lua_State* L)
{
	CUiScrollFrame* scrollframe = dynamic_cast<CUiScrollFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (scrollframe == NULL)
		lua_error(L);

	scrollframe->SetVerticalScroll((float)luaL_checknumber(L, 2));
	return 0;
}

// ----------------------------------------------------------------------------------
int ScrollFrame_GetVerticalScroll(lua_State* L)
{
	CUiScrollFrame* scrollframe = dynamic_cast<CUiScrollFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (scrollframe == NULL)
		lua_error(L);

	//lua_pushnumber(L, scrollframe->GetViewOffset().m_y);
	lua_pushnumber(L, 1);
	return 1;
}

// ----------------------------------------------------------------------------------
int ScrollFrame_SetHorizontalScroll(lua_State* L)
{
	CUiScrollFrame* scrollframe = dynamic_cast<CUiScrollFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (scrollframe == NULL)
		lua_error(L);

	scrollframe->SetHorizontalScroll((float)luaL_checknumber(L, 2));
	return 0;
}

// ----------------------------------------------------------------------------------
int ScrollFrame_GetHorizontalScroll(lua_State* L)
{
	CUiScrollFrame* scrollframe = dynamic_cast<CUiScrollFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (scrollframe == NULL)
		lua_error(L);

	//lua_pushnumber(L, scrollframe->GetViewOffset().m_x);
	lua_pushnumber(L, 1);
	return 1;
}

// ----------------------------------------------------------------------------------
int ScrollFrame_SetScrollRange(lua_State* L)
{
	CUiScrollFrame* scrollframe = dynamic_cast<CUiScrollFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (scrollframe == NULL)
		lua_error(L);

	scrollframe->SetScrollRange((float)luaL_checknumber(L, 2), (float)luaL_checknumber(L, 3));
	return 0;
}

// ----------------------------------------------------------------------------------
int ScrollFrame_GetScrollRange(lua_State* L)
{
	CUiScrollFrame* scrollframe = dynamic_cast<CUiScrollFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (scrollframe == NULL)
		lua_error(L);

	CUiPoint pt = scrollframe->GetScrollRange();
	lua_pushnumber(L, pt.m_x);
	lua_pushnumber(L, pt.m_y);
	return 2;
}

// ----------------------------------------------------------------------------------
int ScrollFrame_UpdateScrollChildRect(lua_State* L)
{
	CUiScrollFrame* scrollframe = dynamic_cast<CUiScrollFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (scrollframe == NULL)
		lua_error(L);

	scrollframe->UpdateScrollChildRect();
	return 0;
}