#include <windows.h>
#include <assert.h>
#include "../uilayout.h"

int Layout_GetName			(lua_State* L);
int Layout_GetParent		(lua_State* L);
int Layout_GetSize			(lua_State* L);
int Layout_GetRealSize		(lua_State* L);
int Layout_GetWidth			(lua_State* L);
int Layout_GetHeight		(lua_State* L);
int Layout_GetPos			(lua_State* L);
int Layout_GetTop			(lua_State* L);
int Layout_GetBottom		(lua_State* L);
int Layout_GetLeft			(lua_State* L);
int Layout_GetRight			(lua_State* L);
int Layout_GetCenter		(lua_State* L);
int Layout_SetSize			(lua_State* L);
int Layout_SetWidth			(lua_State* L);
int Layout_SetHeight		(lua_State* L);
int Layout_SetPos			(lua_State* L);
int Layout_SetAnchor		(lua_State* L);
int Layout_ClearAllAnchors	(lua_State* L);
int Layout_GetAnchor		(lua_State* L);
int Layout_GetAnchorOffset	(lua_State* L);
int Layout_IsVisible		(lua_State* L);
int Layout_Show				(lua_State* L);
int Layout_Hide				(lua_State* L);
int Layout_SetColor			(lua_State* L);
int Layout_GetColor			(lua_State* L);
int Layout_SetAlpha			(lua_State* L);
int Layout_GetAlpha			(lua_State* L);
int Layout_Moving			(lua_State* L);
int	Layout_SetScale			(lua_State* L);
int	Layout_GetScale			(lua_State* L);
int	Layout_GetRealScale		(lua_State* L);

// ----------------------------------------------------------------------------------
static const struct luaL_reg g_layoutTable [] = {
	{"GetName",			Layout_GetName},
	{"GetParent",		Layout_GetParent},
	{"GetSize",			Layout_GetSize},
	{"GetRealSize",		Layout_GetRealSize},
	{"GetWidth",		Layout_GetWidth},
	{"GetHeight",		Layout_GetHeight},
	{"GetPos",			Layout_GetPos},
	{"GetTop",			Layout_GetTop},
	{"GetBottom",		Layout_GetBottom},
	{"GetLeft",			Layout_GetLeft},	
	{"GetRight",		Layout_GetRight},
	{"GetCenter",		Layout_GetCenter},
	{"SetSize",			Layout_SetSize},
	{"SetWidth",		Layout_SetWidth},
	{"SetHeight",		Layout_SetHeight},
	{"SetPos",			Layout_SetPos},
	{"SetAnchor",		Layout_SetAnchor},
	{"ClearAllAnchors",	Layout_ClearAllAnchors},
	{"GetAnchor",		Layout_GetAnchor},
	{"GetAnchorOffset",	Layout_GetAnchorOffset},
	{"IsVisible",		Layout_IsVisible},
	{"Show",			Layout_Show},
	{"Hide",			Layout_Hide},
	{"SetColor",		Layout_SetColor},
	{"GetColor",		Layout_GetColor},
	{"SetAlpha",		Layout_SetAlpha},
	{"GetAlpha",		Layout_GetAlpha},
	{"Moving",			Layout_Moving},
	{"SetScale",		Layout_SetScale},
	{"GetScale",		Layout_GetScale},
	{"GetRealScale",	Layout_GetRealScale},
	{NULL, NULL}
};

// ----------------------------------------------------------------------------------
void CUiLuaLayout::OpenLib(lua_State* L)
{
	luaL_newmetatable(L, UI_LUA_TYPE_LAYOUT);		// create new metatable
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);							// metatable.__index = metatable
	luaL_openlib(L, NULL, g_layoutTable, 0);
}

// ----------------------------------------------------------------------------------
CUiLayout* CUiLuaLayout::CheckLayoutTable(lua_State* L, int num)
{
	if (num < 0)
		num = lua_gettop(L) - num + 1;

	luaL_checktype(L, num, LUA_TTABLE);
	lua_pushstring(L, UI_LUA_POINTER);
	lua_rawget(L, num);
	if (lua_isnil(L, -1))
		lua_error(L);
	CUiLayout* layout = (CUiLayout*)lua_touserdata(L, -1);
	luaL_argcheck(L, layout, 1, "expected");
	lua_pop(L, 1);
	return layout;
}

// ----------------------------------------------------------------------------------
int CUiLuaLayout::SetMetaTable(lua_State* L)
{
	if (lua_istable(L, -1))
	{
		luaL_getmetatable(L, UI_LUA_TYPE_LAYOUT);
		lua_setmetatable(L, -2);
		return 1;
	}
	return 0;
}

// ----------------------------------------------------------------------------------
int CUiLuaLayout::NewTable(lua_State* L, CUiLayout* layout)
{
	assert(layout);

	lua_pushlightuserdata(L, layout);
	lua_rawget(L, LUA_REGISTRYINDEX);
	if (!lua_isnil(L, -1))						// 檢查值是否存在
		return 0;
	lua_pop(L, 1);
	lua_newtable(L);
	lua_pushlightuserdata(L, layout);
	lua_pushvalue(L, -2);
	lua_rawset(L, LUA_REGISTRYINDEX);
	lua_pushstring(L, UI_LUA_POINTER);			// 建立指標索引
	lua_pushlightuserdata(L, layout);			// 儲存指標位置
	lua_settable(L, -3);
	return 1;
}

// ----------------------------------------------------------------------------------
void CUiLuaLayout::DeleteTable(lua_State* L, CUiLayout* layout)
{
	PushLayoutTable(L, layout);
	if ( !lua_isnil(L, -1) )
	{
		// Clean index
		lua_pushlightuserdata(L, layout);
		lua_pushnil(L);
		lua_rawset(L, LUA_REGISTRYINDEX);

		// Clean Table pointer
		lua_pushstring(L, UI_LUA_POINTER);			// 建立指標索引
		lua_pushnil(L);
		lua_settable(L, -3);
	}
	lua_pop(L, 1);
}

// ----------------------------------------------------------------------------------
void CUiLuaLayout::PushLayoutTable(lua_State* L, CUiLayout* layout)
{	
	lua_pushlightuserdata(L, layout);
	lua_rawget(L, LUA_REGISTRYINDEX);
}

// ----------------------------------------------------------------------------------
CUiAnchor CUiLuaLayout::GetAnchor(lua_State* L, CUiLayout* layout)
{
	CUiAnchor anchor;
	anchor.m_point = CUiAnchor::GetAnchorPoint(luaL_checkstring(L, 2));
	for ( int i = 3; i <= lua_gettop(L); i++ )
	{
		switch(i)
		{
		case 3:
			anchor.m_relativePoint = CUiAnchor::GetAnchorPoint(luaL_checkstring(L, 3));
			break;

		case 4:
			switch (lua_type(L, 4))
			{
			case LUA_TTABLE:
				anchor.m_relativeTo = CUiLuaLayout::CheckLayoutTable(L, 4);
				break;
			case LUA_TSTRING:
				anchor.m_relativeTo = layout->FindByName(layout->TransName(luaL_checkstring(L, 4)));
				break;
			default:
				lua_error(L);
				break;
			}
			break;

		case 5:
			if ( lua_isnumber(L, 5) )
				anchor.m_offset.m_x = (float)lua_tonumber(L, 5);
			break;

		case 6:
			if ( lua_isnumber(L, 6) )
				anchor.m_offset.m_y = (float)lua_tonumber(L, 6);
			break;
		}
	}
	return anchor;
}

// ----------------------------------------------------------------------------------
CUiColor CUiLuaLayout::GetColor(lua_State* L, int top)
{
	CUiColor color(1.0f, 1.0f, 1.0f, 1.0f);

	int count = 0;
	for ( int i = top; i <= lua_gettop(L); i++ )
	{
		switch (count)
		{
		case 0:
			color.m_r = (float)luaL_checknumber(L, i);
			break;

		case 1:
			color.m_g = (float)luaL_checknumber(L, i);
			break;

		case 2:
			color.m_b = (float)luaL_checknumber(L, i);
			break;

		case 3:
			color.m_a = (float)luaL_checknumber(L, i);
			break;
		}
		count++;
	}
	return color;
}

// ----------------------------------------------------------------------------------
CUiRectangle CUiLuaLayout::GetRectangle(lua_State* L, int top)
{
	CUiRectangle rect(0.0f, 0.0f, 0.0f, 0.0f);
	for ( int i = top; i <= lua_gettop(L); i++ )
	{
		switch (i)
		{
		case 2:		// left
			rect.m_left = (float)luaL_checknumber(L, i);
			break;

		case 3:		// right
			rect.m_right = (float)luaL_checknumber(L, i);
			break;

		case 4:		// top
			rect.m_top = (float)luaL_checknumber(L, i);
			break;

		case 5:		// bottom
			rect.m_bottom = (float)luaL_checknumber(L, i);
			break;
		}		
	}
	return rect;
}

// ----------------------------------------------------------------------------------
int Layout_GetName(lua_State* L)
{
	CUiLayout* layout = CUiLuaLayout::CheckLayoutTable(L, 1);
	const char* name = layout->GetName();
	if (name)
	{
		lua_pushstring(L, name);
		return 1;
	}
	return 0;
}

// ----------------------------------------------------------------------------------
int Layout_GetParent(lua_State* L)
{	
	CUiLayout* layout = CUiLuaLayout::CheckLayoutTable(L, 1);
	CUiLuaLayout::PushLayoutTable(L, layout->GetParent());
	return 1;
}

// ----------------------------------------------------------------------------------
int Layout_GetWidth(lua_State* L)
{
	CUiLayout* layout = CUiLuaLayout::CheckLayoutTable(L, 1);
	lua_pushnumber(L, layout->GetWidth());
	return 1;
}

// ----------------------------------------------------------------------------------
int Layout_GetHeight(lua_State* L)
{
	CUiLayout* layout = CUiLuaLayout::CheckLayoutTable(L, 1);
	lua_pushnumber(L, layout->GetHeight());
	return 1;
}

// ----------------------------------------------------------------------------------
int Layout_GetSize(lua_State* L)
{
	CUiLayout* layout = CUiLuaLayout::CheckLayoutTable(L, 1);
	CUiSize size = layout->GetSize();
	lua_pushnumber(L, size.m_x);
	lua_pushnumber(L, size.m_y);
	return 2;
}

// ----------------------------------------------------------------------------------
int Layout_GetRealSize(lua_State* L)
{
	CUiLayout* layout = CUiLuaLayout::CheckLayoutTable(L, 1);
	CUiSize size = layout->GetRealSize();
	lua_pushnumber(L, size.m_x);
	lua_pushnumber(L, size.m_y);
	return 2;
}

// ----------------------------------------------------------------------------------
int Layout_GetPos(lua_State* L)
{
	CUiLayout* layout = CUiLuaLayout::CheckLayoutTable(L, 1);
	CUiPoint pos = layout->GetPos();
	lua_pushnumber(L, pos.m_x);
	lua_pushnumber(L, pos.m_y);
	return 2;
}

// ----------------------------------------------------------------------------------
int Layout_GetTop(lua_State* L)
{
	CUiLayout* layout = CUiLuaLayout::CheckLayoutTable(L, 1);
	CUiPoint point = layout->GetPaintPos();
	lua_pushnumber(L, point.m_y);
	return 1;
}

// ----------------------------------------------------------------------------------
int Layout_GetBottom(lua_State* L)
{
	CUiLayout* layout = CUiLuaLayout::CheckLayoutTable(L, 1);
	CUiSize size = layout->GetRealSize();
	CUiPoint point = layout->GetPaintPos();
	lua_pushnumber(L, point.m_y + size.m_y);
	return 1;
}

// ----------------------------------------------------------------------------------
int Layout_GetLeft(lua_State* L)
{
	CUiLayout* layout = CUiLuaLayout::CheckLayoutTable(L, 1);
	CUiPoint point = layout->GetPaintPos();
	lua_pushnumber(L, point.m_x);
	return 1;
}

// ----------------------------------------------------------------------------------
int Layout_GetRight(lua_State* L)
{
	CUiLayout* layout = CUiLuaLayout::CheckLayoutTable(L, 1);
	CUiSize size = layout->GetRealSize();
	CUiPoint point = layout->GetPaintPos();
	lua_pushnumber(L, point.m_x + size.m_x);
	return 1;
}

// ----------------------------------------------------------------------------------
int Layout_GetCenter(lua_State* L)
{
	CUiLayout* layout = CUiLuaLayout::CheckLayoutTable(L, 1);
	CUiSize size = layout->GetRealSize();
	CUiPoint point = layout->GetPaintPos();	
	lua_pushnumber(L, point.m_x + size.m_x / 2.0f);
	lua_pushnumber(L, point.m_y + size.m_y / 2.0f);
	return 2;
}

// ----------------------------------------------------------------------------------
int Layout_SetSize(lua_State* L)
{
	CUiLayout* layout = CUiLuaLayout::CheckLayoutTable(L, 1);
	CUiSize size;
	size.m_x = (float)luaL_checknumber(L, 2);
	size.m_y = (float)luaL_checknumber(L, 3);
	layout->SetSize(size);
	return 0;
}

// ----------------------------------------------------------------------------------
int Layout_SetWidth(lua_State* L)
{
	CUiLayout* layout = CUiLuaLayout::CheckLayoutTable(L, 1);
	layout->SetWidth( (float)luaL_checknumber(L, 2) );
	return 0;
}

// ----------------------------------------------------------------------------------
int Layout_SetHeight(lua_State* L)
{
	CUiLayout* layout = CUiLuaLayout::CheckLayoutTable(L, 1);
	layout->SetHeight( (float)luaL_checknumber(L, 2) );
	return 0;
}

// ----------------------------------------------------------------------------------
int Layout_SetPos(lua_State* L)
{
	CUiLayout* layout = CUiLuaLayout::CheckLayoutTable(L, 1);
	CUiPoint pos;
	pos.m_x = (float)luaL_checknumber(L, 2);
	pos.m_y = (float)luaL_checknumber(L, 3);
	layout->SetPos(pos);
	return 0;
}

// ----------------------------------------------------------------------------------
// stack index 1 CUiLayout Table
// stack index 2 point mode
// stack index 3 relative point mode
// stack index 4 to object name
// stack index 5 x offset 
// stack index 6 y offset
int Layout_SetAnchor(lua_State* L)
{	
	CUiLayout* layout = CUiLuaLayout::CheckLayoutTable(L, 1);
	layout->SetAnchor( CUiLuaLayout::GetAnchor(L, layout) );
	return 0;
}

// ----------------------------------------------------------------------------------
int Layout_ClearAllAnchors(lua_State* L)
{
	CUiLayout* layout = CUiLuaLayout::CheckLayoutTable(L, 1);
	layout->ClearAllAnchors();
	return 0;
}

// ----------------------------------------------------------------------------------
int Layout_GetAnchor(lua_State* L)
{
	CUiLayout* layout = CUiLuaLayout::CheckLayoutTable(L, 1);
	CUiLayout* relativeTo = NULL;
	const char* point = "";
	const char* relativePoint = "";
	float x = 0.0f;
	float y = 0.0f;

	int index = 0;
	if ( lua_isnumber(L, 2) ) {
		index = (int)(max(lua_tonumber(L, 2), 0));
	}

	if ( index < layout->GetAnchorsSize() )
	{
		CUiAnchor& anchor = layout->GetAnchor(index);		
		point			= anchor.GetAnchorPointStr(anchor.GetPos());
		relativePoint	= anchor.GetAnchorPointStr(anchor.GetRelativePoint());
		relativeTo		= anchor.m_relativeTo;
		x				= anchor.m_offset.m_x;
		y				= anchor.m_offset.m_y;
	}
	lua_pushstring(L, point);
	lua_pushstring(L, relativePoint);
	CUiLuaLayout::PushLayoutTable(L, relativeTo);
	lua_pushnumber(L, x);
	lua_pushnumber(L, y);
	return 5;
}

// ----------------------------------------------------------------------------------
int Layout_GetAnchorOffset(lua_State* L)
{
	CUiLayout* layout = CUiLuaLayout::CheckLayoutTable(L, 1);
	float x = 0.0f;
	float y = 0.0f;

	int index = 0;
	if ( lua_isnumber(L, 2) ) {
		index = (int)(max(lua_tonumber(L, 2), 0));
	}

	if ( index < layout->GetAnchorsSize() )
	{
		CUiAnchor& anchor = layout->GetAnchor(index);
		x = anchor.m_offset.m_x;
		y = anchor.m_offset.m_y;
	}
	lua_pushnumber(L, x);
	lua_pushnumber(L, y);
	return 2;
}

// ----------------------------------------------------------------------------------
int Layout_IsVisible(lua_State* L)
{
	CUiLayout* layout = CUiLuaLayout::CheckLayoutTable(L, 1);
	lua_pushboolean(L, layout->IsVisible(lua_toboolean(L, 2) != 0));
	return 1;
}

// ----------------------------------------------------------------------------------
int Layout_Show(lua_State* L)
{
	CUiLayout* layout = CUiLuaLayout::CheckLayoutTable(L, 1);
	layout->SetVisible(true);
	return 0;
}

// ----------------------------------------------------------------------------------
int Layout_Hide(lua_State* L)
{
	CUiLayout* layout = CUiLuaLayout::CheckLayoutTable(L, 1);
	layout->SetVisible(false);
	return 0;
}

// ----------------------------------------------------------------------------------
int Layout_SetColor(lua_State* L)
{
	CUiLayout* layout = CUiLuaLayout::CheckLayoutTable(L, 1);
	layout->SetColor((float)luaL_checknumber(L, 2), (float)luaL_checknumber(L, 3), (float)luaL_checknumber(L, 4));
	return 0;
}

// ----------------------------------------------------------------------------------
int Layout_GetColor(lua_State* L)
{
	CUiLayout* layout = CUiLuaLayout::CheckLayoutTable(L, 1);
	CUiColor color = layout->GetColor();
	lua_pushnumber(L, color.m_r);
	lua_pushnumber(L, color.m_g);
	lua_pushnumber(L, color.m_b);
	return 3;
}

// ----------------------------------------------------------------------------------
int Layout_SetAlpha(lua_State* L)
{
	CUiLayout* layout = CUiLuaLayout::CheckLayoutTable(L, 1);
	layout->SetAlpha((float)luaL_checknumber(L, 2));
	return 0;
}

// ----------------------------------------------------------------------------------
int Layout_GetAlpha(lua_State* L)
{
	CUiLayout* layout = CUiLuaLayout::CheckLayoutTable(L, 1);
	lua_pushnumber(L, layout->GetAlpha());
	return 1;
}

// ----------------------------------------------------------------------------------
int Layout_Moving(lua_State* L)
{
	CUiLayout* layout = CUiLuaLayout::CheckLayoutTable(L, 1);
	float x = 0.0f;
	float y = 0.0f;

	if ( lua_isnumber(L, 2) )
		x = (float)lua_tonumber(L, 2);
	if ( lua_isnumber(L, 3) )
		y = (float)lua_tonumber(L, 3);

	layout->Moving(x, y);
	return 0;
}

// ----------------------------------------------------------------------------------
int	Layout_SetScale(lua_State* L)
{
	CUiLayout* layout = CUiLuaLayout::CheckLayoutTable(L, 1);
	if (layout == NULL)
		lua_error(L);

	layout->SetScale( (float)luaL_checknumber(L, 2) );
	return 0;
}

// ----------------------------------------------------------------------------------
int	Layout_GetScale(lua_State* L)
{
	CUiLayout* layout = CUiLuaLayout::CheckLayoutTable(L, 1);
	if (layout == NULL)
		lua_error(L);

	lua_pushnumber(L, layout->GetScale());
	return 1;
}

// ----------------------------------------------------------------------------------
int	Layout_GetRealScale		(lua_State* L)
{
	CUiLayout* layout = CUiLuaLayout::CheckLayoutTable(L, 1);
	if (layout == NULL)
		lua_error(L);

	lua_pushnumber(L, layout->GetRealScale());
	return 1;
}