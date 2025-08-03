#include <windows.h>
#include "../uieditbox.h"

int EditBox_SetFocus(lua_State* L);
int EditBox_ClearFocus(lua_State* L);
int EditBox_GetText(lua_State* L);
int EditBox_SetText(lua_State* L);
int EditBox_SetNumber(lua_State* L);
int EditBox_GetNumber(lua_State* L);
int EditBox_SetTextColor(lua_State* L);
int EditBox_SetTextInset(lua_State* L);
int EditBox_SetCaretColor(lua_State* L);
int EditBox_GetCaretColor(lua_State* L);
int EditBox_AddHistoryLine(lua_State* L);
int EditBox_HighlightText(lua_State* L);
int EditBox_InsertChar(lua_State* L);
int EditBox_InsertText(lua_State* L);
int EditBox_SetPasswordMode(lua_State* L);
int EditBox_SetPlaceCaret(lua_State* L);
int EditBox_GetMultiMaxLines(lua_State* L);
int EditBox_GetMultiRangeLines(lua_State* L);
int EditBox_GetMultiFirstLine(lua_State* L);
int EditBox_SetMultiFirstLine(lua_State* L);
int EditBox_SetKeyboardInputEnable(lua_State* L);
int EditBox_SetRTL(lua_State* L);

// ----------------------------------------------------------------------------------
static const struct luaL_reg g_editboxTable [] = {
	{"SetFocus",				EditBox_SetFocus},
	{"ClearFocus",				EditBox_ClearFocus},
	{"GetText",					EditBox_GetText},
	{"SetText",					EditBox_SetText},
	{"SetNumber",				EditBox_SetNumber},
	{"GetNumber",				EditBox_GetNumber},
	{"SetTextColor",			EditBox_SetTextColor},
	{"SetTextInset",			EditBox_SetTextInset},
	{"SetCaretColor",			EditBox_SetCaretColor},
	{"GetCaretColor",			EditBox_GetCaretColor},
	{"AddHistoryLine",			EditBox_AddHistoryLine},
	{"HighlightText",			EditBox_HighlightText},
	{"InsertChar",				EditBox_InsertChar},
	{"InsertText",				EditBox_InsertText},
	{"SetPasswordMode",			EditBox_SetPasswordMode},
	{"SetPlaceCaret",			EditBox_SetPlaceCaret},	
	{"GetMultiMaxLines",		EditBox_GetMultiMaxLines},
	{"GetMultiRangeLines",		EditBox_GetMultiRangeLines},
	{"GetMultiFirstLine",		EditBox_GetMultiFirstLine},
	{"SetMultiFirstLine",		EditBox_SetMultiFirstLine},
	{"SetKeyboardInputEnable",	EditBox_SetKeyboardInputEnable},
	{"SetRTL",					EditBox_SetRTL},
	{NULL, NULL}
};

// ----------------------------------------------------------------------------------
void CUiLuaEditBox::OpenLib(lua_State* L)
{
	luaL_newmetatable(L, UI_LUA_TYPE_EDITBOX);			// create new metatable
	luaL_getmetatable(L, UI_LUA_TYPE_FRAME);			// inherit layout
	lua_setmetatable(L, -2);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);
	luaL_openlib(L, NULL, g_editboxTable, 0);
}

// ----------------------------------------------------------------------------------
int	CUiLuaEditBox::SetMetaTable(lua_State* L)
{
	if (lua_istable(L, -1))
	{
		luaL_getmetatable(L, UI_LUA_TYPE_EDITBOX);
		lua_setmetatable(L, -2);
		return 1;
	}
	return 0;
}

// ----------------------------------------------------------------------------------
int EditBox_SetFocus(lua_State* L)
{
	CUiEditBox* editbox = dynamic_cast<CUiEditBox*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (editbox == NULL)
		lua_error(L);

	editbox->SetFocus();
	return 0;
}

// ----------------------------------------------------------------------------------
int EditBox_ClearFocus(lua_State* L)
{
	CUiEditBox* editbox = dynamic_cast<CUiEditBox*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (editbox == NULL)
		lua_error(L);

	editbox->ClearFocus();
	return 0;
}

// ----------------------------------------------------------------------------------
int EditBox_GetText(lua_State* L)
{
	CUiEditBox* editbox = dynamic_cast<CUiEditBox*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (editbox == NULL)
		lua_error(L);

	lua_pushstring(L, editbox->GetText());
	return 1;
}

// ----------------------------------------------------------------------------------
int EditBox_SetText(lua_State* L)
{
	CUiEditBox* editbox = dynamic_cast<CUiEditBox*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (editbox == NULL)
		lua_error(L);

	const char* text = (lua_isstring(L, 2)) ? lua_tostring(L, 2) : "";
	editbox->SetTextA(text);
	return 0;
}

// ----------------------------------------------------------------------------------
int EditBox_SetNumber(lua_State* L)
{
	CUiEditBox* editbox = dynamic_cast<CUiEditBox*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (editbox == NULL)
		lua_error(L);

	if ( lua_isstring(L, 2) ) {
		WCHAR* ws = Utf8ToWChar(lua_tostring(L, 2));
		editbox->SetText(ws);
		delete [] ws;
	}
	return 0;
}

// ----------------------------------------------------------------------------------
int EditBox_GetNumber(lua_State* L)
{
	CUiEditBox* editbox = dynamic_cast<CUiEditBox*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (editbox == NULL)
		lua_error(L);

	lua_pushnumber(L, _wtof(editbox->GetText()));
	return 1;
}

// ----------------------------------------------------------------------------------
int EditBox_SetTextColor(lua_State* L)
{
	CUiEditBox* editbox = dynamic_cast<CUiEditBox*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (editbox == NULL)
		lua_error(L);

	editbox->SetTextColor( CUiLuaLayout::GetColor(L, 2) );
	return 0;
}

// ----------------------------------------------------------------------------------
int EditBox_SetTextInset(lua_State* L)
{
	CUiEditBox* editbox = dynamic_cast<CUiEditBox*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (editbox == NULL)
		lua_error(L);

	editbox->SetTextInset( CUiLuaLayout::GetRectangle(L, 2) );
	return 0;
}

// ----------------------------------------------------------------------------------
int EditBox_SetCaretColor(lua_State* L)
{
	CUiEditBox* editbox = dynamic_cast<CUiEditBox*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (editbox == NULL)
		lua_error(L);

	editbox->SetCaretColor( CUiLuaLayout::GetColor(L, 2).GetColor() );
	return 0;
}

// ----------------------------------------------------------------------------------
int EditBox_GetCaretColor(lua_State* L)
{
	CUiEditBox* editbox = dynamic_cast<CUiEditBox*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (editbox == NULL)
		lua_error(L);	

	CUiColor color = editbox->GetCaretColor();
	lua_pushnumber(L, color.m_r);
	lua_pushnumber(L, color.m_g);
	lua_pushnumber(L, color.m_b);
	return 3;
}

// ----------------------------------------------------------------------------------
int EditBox_AddHistoryLine(lua_State* L)
{
	CUiEditBox* editbox = dynamic_cast<CUiEditBox*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (editbox == NULL)
		lua_error(L);

	editbox->AddHistoryLine( luaL_checkstring(L, 2) );
	return 0;
}

// ----------------------------------------------------------------------------------
int EditBox_HighlightText(lua_State* L)
{
	CUiEditBox* editbox = dynamic_cast<CUiEditBox*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (editbox == NULL)
		lua_error(L);

	if ( lua_gettop(L) > 1 )
		editbox->HighlightText(luaL_checkint(L, 2), luaL_checkint(L, 3));
	else
		editbox->HighlightText();
	return 0;
}

// ----------------------------------------------------------------------------------
int EditBox_InsertChar(lua_State* L)
{
	CUiEditBox* editbox = dynamic_cast<CUiEditBox*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (editbox == NULL)
		lua_error(L);

	if ( lua_isstring(L, 2) ) {
		editbox->InsertKeyboardChar(lua_tostring(L, 2), -1);
	}
	return 0;
}

// ----------------------------------------------------------------------------------
int EditBox_InsertText(lua_State* L)
{
	CUiEditBox* editbox = dynamic_cast<CUiEditBox*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (editbox == NULL)
		lua_error(L);

	if ( lua_isstring(L, 2) ) {
		editbox->AddTextA(lua_tostring(L, 2));
	}
	return 0;
}

// ----------------------------------------------------------------------------------
int EditBox_SetPasswordMode(lua_State* L)
{
	CUiEditBox* editbox = dynamic_cast<CUiEditBox*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (editbox == NULL)
		lua_error(L);

	editbox->SetPassword(lua_toboolean(L, 2) != 0);
	return 0;
}
// ----------------------------------------------------------------------------------
int EditBox_SetPlaceCaret(lua_State* L)
{
	CUiEditBox* editbox = dynamic_cast<CUiEditBox*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (editbox == NULL)
		lua_error(L);

	editbox->SetPlaceCaret((int)lua_tonumber(L, 2));
	return 0;
}
// ----------------------------------------------------------------------------------
int EditBox_GetMultiMaxLines(lua_State* L)
{
	CUiEditBox* editbox = dynamic_cast<CUiEditBox*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (editbox == NULL)
		lua_error(L);

	lua_pushnumber(L, editbox->GetMultiLines());
	return 1;
}
// ----------------------------------------------------------------------------------
int EditBox_GetMultiRangeLines(lua_State* L)
{
	CUiEditBox* editbox = dynamic_cast<CUiEditBox*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (editbox == NULL)
		lua_error(L);

	lua_pushnumber(L, editbox->GetMultiDisplayLines());
	return 1;
}
// ----------------------------------------------------------------------------------
int EditBox_GetMultiFirstLine(lua_State* L)
{
	CUiEditBox* editbox = dynamic_cast<CUiEditBox*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (editbox == NULL)
		lua_error(L);

	lua_pushnumber(L, editbox->GetMultiFirstLine());
	return 1;
}
// ----------------------------------------------------------------------------------
int EditBox_SetMultiFirstLine(lua_State* L)
{
	CUiEditBox* editbox = dynamic_cast<CUiEditBox*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (editbox == NULL)
		lua_error(L);
	editbox->SetMultiFirstLine(luaL_checkint(L, 2));
	return 0;
}
// ----------------------------------------------------------------------------------
int EditBox_SetKeyboardInputEnable(lua_State* L)
{
	CUiEditBox* editbox = dynamic_cast<CUiEditBox*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (editbox == NULL)
		lua_error(L);
	editbox->SetInputState(lua_toboolean(L, 2) != 0);
	return 0;
}

// ----------------------------------------------------------------------------------
int EditBox_SetRTL(lua_State* L)
{
	CUiEditBox* editbox = dynamic_cast<CUiEditBox*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (editbox == NULL)
		lua_error(L);
	editbox->SetRTL(lua_toboolean(L, 2) != 0);
	return 0;
}