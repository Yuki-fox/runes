#include <windows.h>
#include "../uifontstring.h"

int FontString_SetText(lua_State* L);
int FontString_GetText(lua_State* L);
//int FontString_SetColor(lua_State* L);

int FontString_SetFont(lua_State* L);
int FontString_SetFontSize(lua_State* L);
int FontString_HideLastDot(lua_State* L);
int FontString_IsDrawDot(lua_State* L);
int FontString_SetJustifyHType(lua_State* L);

int FontString_GetDisplayWidth(lua_State* L);
int FontString_SetRTL(lua_State* L);
int FontString_GetPaintLines(lua_State* L);

// ----------------------------------------------------------------------------------
static const struct luaL_reg g_fontstringTable [] = {	
	{"SetText",			FontString_SetText},
	{"GetText",			FontString_GetText},
	//{"SetColor",		FontString_SetColor},
	
	{"SetFont",			FontString_SetFont},
	{"SetFontSize",		FontString_SetFontSize},
	{"HideLastDot",		FontString_HideLastDot},	
	{"IsDrawDot",		FontString_IsDrawDot},
	{"SetJustifyHType",	FontString_SetJustifyHType},

	{"GetDisplayWidth",	FontString_GetDisplayWidth},
	{"SetRTL",			FontString_SetRTL},
	{"GetPaintLines",	FontString_GetPaintLines},
	{NULL, NULL}
};	

// ----------------------------------------------------------------------------------
void CUiLuaFontString::OpenLib(lua_State* L)
{
	luaL_newmetatable(L, UI_LUA_TYPE_FONTSTRING);		// create new metatable
	luaL_getmetatable(L, UI_LUA_TYPE_LAYOUT);			// inherit layout
	lua_setmetatable(L, -2);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);
	luaL_openlib(L, NULL, g_fontstringTable, 0);
}

// ----------------------------------------------------------------------------------
int	CUiLuaFontString::SetMetaTable(lua_State* L)
{
	if (lua_istable(L, -1))
	{
		luaL_getmetatable(L, UI_LUA_TYPE_FONTSTRING);
		lua_setmetatable(L, -2);
		return 1;
	}
	return 0;
}

// ----------------------------------------------------------------------------------
int FontString_SetText(lua_State* L)
{
	CUiFontString* font = dynamic_cast<CUiFontString*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (font == NULL)
		lua_error(L);
	
	const char* text = (lua_isstring(L, 2)) ? lua_tostring(L, 2) : "";
	font->SetTextA(text);
	return 0;
}

// ----------------------------------------------------------------------------------
int FontString_GetText(lua_State* L)
{
	CUiFontString* font = dynamic_cast<CUiFontString*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (font == NULL)
		lua_error(L);

	lua_pushstring(L, font->GetText());
	return 1;
}
/*
// ----------------------------------------------------------------------------------
// arg2 red
// arg3 green
// arg3 blue
int FontString_SetColor(lua_State* L)
{
	CUiFontString* fontString = dynamic_cast<CUiFontString*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (fontString == NULL)
		lua_error(L);

	int red = luaL_checkint(L, 2);
	int green = luaL_checkint(L, 3);
	int blue = luaL_checkint(L, 4);


	fontString->SetColor(red,green,blue);


return 0;
}
*/
// ----------------------------------------------------------------------------------
// arg2 字型名稱
// arg3 字型大小
// arg4 字型寬度 (預設 ruFONTWEIGHT_NORMAL)
// arg5 字型外框線 (預設 ruFONTOUTLINE_NONE)
int FontString_SetFont(lua_State* L)
{
	CUiFontString* fontString = dynamic_cast<CUiFontString*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (fontString == NULL)
		lua_error(L);

	const char* fontFile = luaL_checkstring(L, 2);
	int fontSize = luaL_checkint(L, 3);
	RuFontWeight fontWeight = ruFONTWEIGHT_NORMAL;
	RuFontOutline fontOutline = ruFONTOUTLINE_NONE;

	if ( lua_isstring(L, 4) )
	{
		const char* weight = lua_tostring(L, 4);
		if ( strcmp(weight, "THIN") == 0 )
			fontWeight = ruFONTWEIGHT_THIN;
		else if ( strcmp(weight, "NORMAL") == 0 )
			fontWeight = ruFONTWEIGHT_NORMAL;
		else if ( strcmp(weight, "BOLD") == 0 )
			fontWeight = ruFONTWEIGHT_BOLD;
	}

	if ( lua_isstring(L, 5) )
	{
		const char* outline = lua_tostring(L, 5);
		if ( strcmp(outline, "NONE") == 0 )
			fontOutline = ruFONTOUTLINE_NONE;
		else if ( strcmp(outline, "NORMAL") == 0 )
			fontOutline = ruFONTOUTLINE_NORMAL;
		else if ( strcmp(outline, "THICK") == 0 )
			fontOutline = ruFONTOUTLINE_THICK;
	}

	lua_pushboolean( L, fontString->CreateFontType(fontFile, fontSize, fontWeight, fontOutline) );
	return 1;
}

// ----------------------------------------------------------------------------------
int FontString_SetFontSize(lua_State* L)
{
	CUiFontString* fontString = dynamic_cast<CUiFontString*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (fontString == NULL)
		lua_error(L);

	// 取得設定字型大小
	int fontSize = luaL_checkint(L, 2);
	CRuFontType* oldFontType = fontString->GetFontType();
	if ( oldFontType )
	{
		CRuFontType* pFontType = CRuFontType::GetFontType(oldFontType->GetFontName(), fontSize, oldFontType->GetWeight(), oldFontType->GetOutLine());
		if ( pFontType )
		{
			fontString->SetFontType(pFontType);
		}
	}
	return 0;

}

// ----------------------------------------------------------------------------------
int FontString_HideLastDot(lua_State* L)
{
	CUiFontString* fontString = dynamic_cast<CUiFontString*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (fontString == NULL)
		lua_error(L);

	fontString->HideLastDot(true);
	return 0;
}

// ----------------------------------------------------------------------------------
int FontString_IsDrawDot(lua_State* L)
{
	CUiFontString* fontString = dynamic_cast<CUiFontString*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (fontString == NULL)
		lua_error(L);

	lua_pushboolean(L, fontString->IsDrawDot());
	return 1;
}

// ----------------------------------------------------------------------------------
int FontString_SetJustifyHType(lua_State* L)
{
	CUiFontString* fontString = dynamic_cast<CUiFontString*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (fontString == NULL)
		lua_error(L);

	const char* key = luaL_checkstring(L, 2);
	int justifyType;
	if ( strcmp(key, "LEFT") == 0 )
		justifyType = CUiFontString::eJustifyH_Left;
	else if ( strcmp(key, "RIGHT") == 0 )
		justifyType = CUiFontString::eJustifyH_Right;
	else if ( strcmp(key, "CENTER") == 0 )
		justifyType = CUiFontString::eJustifyH_Center;
	else
		return 0;

	fontString->SetJustifyHType(justifyType);
	return 0;
}

// ----------------------------------------------------------------------------------
int FontString_GetDisplayWidth(lua_State* L)
{
	CUiFontString* fontString = dynamic_cast<CUiFontString*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (fontString == NULL)
		lua_error(L);

	lua_pushnumber(L, fontString->GetDisplayWidth(1));
	return 1;
}

// ----------------------------------------------------------------------------------
int FontString_SetRTL(lua_State* L)
{
	CUiFontString* fontString = dynamic_cast<CUiFontString*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (fontString == NULL)
		lua_error(L);

	fontString->SetRTL(lua_toboolean(L, 2) != 0);
	return 0;
}

// ----------------------------------------------------------------------------------
int FontString_GetPaintLines(lua_State* L)
{
	CUiFontString* fontString = dynamic_cast<CUiFontString*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (fontString == NULL)
		lua_error(L);

	lua_pushnumber(L, fontString->GetPaintLines());
	return 1;
}