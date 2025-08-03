#include <windows.h>
#include "../uibutton.h"

int Button_GetNormalTexture(lua_State* L);
int Button_GetPushedTexture(lua_State* L);
int Button_GetDisabledTexture(lua_State* L);
int Button_GetHighlightTexture(lua_State* L);
int Button_SetNormalTexture(lua_State* L);
int Button_SetPushedTexture(lua_State* L);
int Button_SetDisabledTexture(lua_State* L);
int Button_SetHighlightTexture(lua_State* L);

int Button_GetNormalText(lua_State* L);
int Button_GetDisabledText(lua_State* L);
int Button_GetHighlightText(lua_State* L);
int Button_SetText(lua_State* L);
int Button_GetText(lua_State* L);
int Button_GetTextWidth(lua_State* L);
int Button_GetTextHeight(lua_State* L);
int Button_LockHighlight(lua_State* L);
int Button_UnlockHighlight(lua_State* L);
int Button_LockPushed(lua_State* L);
int Button_UnlockPushed(lua_State* L);
int Button_SetTextAnchor(lua_State* L);
int Button_SetTextColor(lua_State* L);
int Button_IsButtonPushed(lua_State* L);

// ----------------------------------------------------------------------------------
static const struct luaL_reg g_buttonTable [] = {
	{"GetNormalTexture",		Button_GetNormalTexture},
	{"SetNormalTexture",		Button_SetNormalTexture},
	{"GetPushedTexture",		Button_GetPushedTexture},
	{"SetPushedTexture",		Button_SetPushedTexture},
	{"GetDisabledTexture",		Button_GetDisabledTexture},
	{"SetDisabledTexture",		Button_SetDisabledTexture},
	{"GetHighlightTexture",		Button_GetHighlightTexture},
	{"SetHighlightTexture",		Button_SetHighlightTexture},
	{"GetNormalText",			Button_GetNormalText},
	{"GetDisabledText",			Button_GetDisabledText},
	{"GetHighlightText",		Button_GetHighlightText},
	{"SetText",					Button_SetText},
	{"GetText",					Button_GetText},
	{"GetTextWidth",			Button_GetTextWidth},
	{"GetTextHeight",			Button_GetTextHeight},
	{"LockHighlight",			Button_LockHighlight},
	{"UnlockHighlight",			Button_UnlockHighlight},
	{"LockPushed",				Button_LockPushed},
	{"UnlockPushed",			Button_UnlockPushed},
	{"SetTextAnchor",			Button_SetTextAnchor},
	{"SetTextColor",			Button_SetTextColor},
	{"IsButtonPushed",			Button_IsButtonPushed},
	{NULL, NULL}
};

// ----------------------------------------------------------------------------------
void CUiLuaButton::OpenLib(lua_State* L)
{
	luaL_newmetatable(L, UI_LUA_TYPE_BUTTON);			// create new metatable
	luaL_getmetatable(L, UI_LUA_TYPE_FRAME);			// inherit layout
	lua_setmetatable(L, -2);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);
	luaL_openlib(L, NULL, g_buttonTable, 0);
}

// ----------------------------------------------------------------------------------
int	CUiLuaButton::SetMetaTable(lua_State* L)
{
	if (lua_istable(L, -1))
	{
		luaL_getmetatable(L, UI_LUA_TYPE_BUTTON);
		lua_setmetatable(L, -2);
		return 1;
	}
	return 0;
}

// ----------------------------------------------------------------------------------
int Button_GetNormalTexture(lua_State* L)
{
	CUiButton* button = dynamic_cast<CUiButton*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (button == NULL)
		lua_error(L);
	CUiLuaLayout::PushLayoutTable(L, button->GetNormalTexture());
	return 1;
}

// ----------------------------------------------------------------------------------
int Button_GetPushedTexture(lua_State* L)
{
	CUiButton* button = dynamic_cast<CUiButton*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (button == NULL)
		lua_error(L);
	CUiLuaLayout::PushLayoutTable(L, button->GetPushedTexture());
	return 1;
}

// ----------------------------------------------------------------------------------
int Button_GetDisabledTexture(lua_State* L)
{
	CUiButton* button = dynamic_cast<CUiButton*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (button == NULL)
		lua_error(L);
	CUiLuaLayout::PushLayoutTable(L, button->GetDisabledTexture());
	return 1;
}

// ----------------------------------------------------------------------------------
int Button_GetHighlightTexture(lua_State* L)
{
	CUiButton* button = dynamic_cast<CUiButton*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (button == NULL)
		lua_error(L);
	CUiLuaLayout::PushLayoutTable(L, button->GetHighlightTexture());
	return 1;
}

// ----------------------------------------------------------------------------------
int Button_GetNormalText(lua_State* L)
{
	CUiButton* button = dynamic_cast<CUiButton*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (button == NULL)
		lua_error(L);
	CUiLuaLayout::PushLayoutTable(L, button->GetNormalText());
	return 1;
}

// ----------------------------------------------------------------------------------
int Button_GetDisabledText(lua_State* L)
{
	CUiButton* button = dynamic_cast<CUiButton*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (button == NULL)
		lua_error(L);
	CUiLuaLayout::PushLayoutTable(L, button->GetDisabledText());
	return 1;
}

// ----------------------------------------------------------------------------------
int Button_GetHighlightText(lua_State* L)
{
	CUiButton* button = dynamic_cast<CUiButton*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (button == NULL)
		lua_error(L);
	CUiLuaLayout::PushLayoutTable(L, button->GetHighlightText());
	return 1;
}

// ----------------------------------------------------------------------------------
int Button_SetText(lua_State* L)
{
	CUiButton* button = dynamic_cast<CUiButton*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (button == NULL)
		lua_error(L);

	const char* text = (lua_isstring(L, 2)) ? lua_tostring(L, 2) : "";
	button->SetTextA(text);
	return 0;
}

// ----------------------------------------------------------------------------------
int Button_GetText(lua_State* L)
{
	CUiButton* button = dynamic_cast<CUiButton*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (button == NULL)
		lua_error(L);

	CUiFontString* normalText = button->GetNormalText();
	if ( normalText ) {
		lua_pushstring(L, normalText->GetText());
		return 1;
	}
	return 0;
}

// ----------------------------------------------------------------------------------
int Button_GetTextWidth(lua_State* L)
{
	CUiButton* button = dynamic_cast<CUiButton*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (button == NULL)
		lua_error(L);
	lua_pushnumber( L, button->GetTextWidth() );
	return 1;
}

// ----------------------------------------------------------------------------------
int Button_GetTextHeight(lua_State* L)
{
	CUiButton* button = dynamic_cast<CUiButton*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (button == NULL)
		lua_error(L);
	lua_pushnumber( L, button->GetTextHeight() );
	return 1;
}

// ----------------------------------------------------------------------------------
int Button_LockHighlight(lua_State* L)
{
	CUiButton* button = dynamic_cast<CUiButton*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (button == NULL)
		lua_error(L);
	button->LockHighlight();
	return 0;
}

// ----------------------------------------------------------------------------------
int Button_UnlockHighlight(lua_State* L)
{
	CUiButton* button = dynamic_cast<CUiButton*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (button == NULL)
		lua_error(L);
	button->UnlockHighlight();
	return 0;
}

// ----------------------------------------------------------------------------------
int Button_LockPushed(lua_State* L)
{
	CUiButton* button = dynamic_cast<CUiButton*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (button == NULL)
		lua_error(L);
	button->LockPushed();
	return 0;
}

// ----------------------------------------------------------------------------------
int Button_UnlockPushed(lua_State* L)
{
	CUiButton* button = dynamic_cast<CUiButton*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (button == NULL)
		lua_error(L);
	button->UnlockPushed();
	return 0;
}

// ----------------------------------------------------------------------------------
// arg1 button this pointer
// arg2 anchor point mode
// arg3 anchor relative point mode
// arg4 anchor x offset
// arg5 anchor y offset
int Button_SetTextAnchor(lua_State* L)
{
	CUiButton* button = dynamic_cast<CUiButton*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (button == NULL)
		lua_error(L);
	button->SetTextAnchor( CUiLuaLayout::GetAnchor(L, button) );
	return 0;
}
// ----------------------------------------------------------------------------------
int Button_SetNormalTexture(lua_State* L)
{
	CUiButton* button = dynamic_cast<CUiButton*>( CUiLuaLayout::CheckLayoutTable(L, 1) );

	CUiTexture* texture = dynamic_cast<CUiTexture*>( CUiLuaLayout::CheckLayoutTable(L, 2) );
	if (button == NULL||texture==NULL)
		lua_error(L);
	button->SetNormalTexture( texture );
	return 0;
}
// ----------------------------------------------------------------------------------
int Button_SetPushedTexture(lua_State* L)
{
	CUiButton* button = dynamic_cast<CUiButton*>( CUiLuaLayout::CheckLayoutTable(L, 1) );

	CUiTexture* texture = dynamic_cast<CUiTexture*>( CUiLuaLayout::CheckLayoutTable(L, 2) );
	if (button == NULL||texture==NULL)
		lua_error(L);
	button->SetPushedTexture( texture );
	return 0;
}
// ----------------------------------------------------------------------------------
int Button_SetDisabledTexture(lua_State* L)
{
	CUiButton* button = dynamic_cast<CUiButton*>( CUiLuaLayout::CheckLayoutTable(L, 1) );

	CUiTexture* texture = dynamic_cast<CUiTexture*>( CUiLuaLayout::CheckLayoutTable(L, 2) );
	if (button == NULL||texture==NULL)
		lua_error(L);
	button->SetDisabledTexture( texture );
	return 0;
}
// ----------------------------------------------------------------------------------
int Button_SetHighlightTexture(lua_State* L)
{
	CUiButton* button = dynamic_cast<CUiButton*>( CUiLuaLayout::CheckLayoutTable(L, 1) );

	CUiTexture* texture = dynamic_cast<CUiTexture*>( CUiLuaLayout::CheckLayoutTable(L, 2) );
	if (button == NULL||texture==NULL)
		lua_error(L);
	button->SetHighlightTexture( texture );
	return 0;
}
// ----------------------------------------------------------------------------------
int Button_SetTextColor(lua_State* L)
{
	CUiButton* button = dynamic_cast<CUiButton*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (button == NULL)
		lua_error(L);

	button->SetTextColor( CUiLuaLayout::GetColor(L, 2) );
	return 0;
}

// ----------------------------------------------------------------------------------
int Button_IsButtonPushed(lua_State* L)
{
	CUiButton* button = dynamic_cast<CUiButton*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (button == NULL)
		lua_error(L);

	int key = 0;
	const char* str = luaL_checkstring(L, 2);
	if (strcmp(str, "LeftButton") == 0)
		key = MOUSE_LBUTTON;
	else if (strcmp(str, "RightButton") == 0)
		key = MOUSE_RBUTTON;
	else if (strcmp(str, "MiddleButton") == 0)
		key = MOUSE_MBUTTON;
	else if (strcmp(str, "OtherButton") == 0)
		key = MOUSE_XBUTTON;
	else
		lua_error(L);

	lua_pushboolean(L, button->GetMouseDown(key) == button);
	return 1;
}