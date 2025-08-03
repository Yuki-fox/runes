#include <windows.h>
#include "../uimessageframe.h"

int MessageFrame_AddMessage(lua_State* L);
int MessageFrame_GetLines(lua_State* L);
int MessageFrame_GetPosition(lua_State* L);
int MessageFrame_SetPosition(lua_State* L);
int	MessageFrame_ResetTime(lua_State* L);
int MessageFrame_ScrollDown(lua_State* L);
int MessageFrame_ScrollUp(lua_State* L);
int MessageFrame_ScrollBottom(lua_State* L);
int MessageFrame_ScrollTop(lua_State* L);
int MessageFrame_ClearText(lua_State* L);
int MessageFrame_SetFontSize(lua_State* L);
int MessageFrame_EnableHyperlink(lua_State* L);
int MessageFrame_SetLineBytes(lua_State* L);
int MessageFrame_GetPageLines(lua_State* L);
int MessageFrame_GetFontString(lua_State* L);

// ----------------------------------------------------------------------------------
static const struct luaL_reg g_messageframeTable [] = {
	{"AddMessage",			MessageFrame_AddMessage},
	{"GetLines",			MessageFrame_GetLines},
	{"GetPosition",			MessageFrame_GetPosition},
	{"SetPosition",			MessageFrame_SetPosition},
	{"ResetTime",			MessageFrame_ResetTime},
	{"ScrollDown",			MessageFrame_ScrollDown},
	{"ScrollUp",			MessageFrame_ScrollUp},
	{"ScrollBottom",		MessageFrame_ScrollBottom},
	{"ScrollTop",			MessageFrame_ScrollTop},
	{"ClearText",			MessageFrame_ClearText},
	{"SetFontSize",			MessageFrame_SetFontSize},
	{"EnableHyperlink",		MessageFrame_EnableHyperlink},
	{"SetLineBytes",		MessageFrame_SetLineBytes},
	{"GetPageLines",		MessageFrame_GetPageLines},
	{"GetFontString",		MessageFrame_GetFontString},
	{NULL, NULL}
};

// ----------------------------------------------------------------------------------
void CUiLuaMessageFrame::OpenLib(lua_State* L)
{
	luaL_newmetatable(L, UI_LUA_TYPE_MESSAGEFRAME);		// create new metatable
	luaL_getmetatable(L, UI_LUA_TYPE_FRAME);			// inherit layout
	lua_setmetatable(L, -2);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);
	luaL_openlib(L, NULL, g_messageframeTable, 0);
}

// ----------------------------------------------------------------------------------
int	CUiLuaMessageFrame::SetMetaTable(lua_State* L)
{
	if (lua_istable(L, -1))
	{
		luaL_getmetatable(L, UI_LUA_TYPE_MESSAGEFRAME);
		lua_setmetatable(L, -2);
		return 1;
	}
	return 0;
}

// ----------------------------------------------------------------------------------
int MessageFrame_AddMessage(lua_State* L)
{
	CUiMessageFrame* messageframe = dynamic_cast<CUiMessageFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (messageframe == NULL)
		lua_error(L);

	CUiColor color(1.0f, 1.0f, 1.0f, 1.0f);
	const char* text = (lua_isstring(L, 2)) ? lua_tostring(L, 2) : "";	
	if ( lua_isnumber(L, 3) == false ) {
		CUiFontString* fontString = messageframe->GetFontString();
		if ( fontString )
			color = fontString->GetColor();
	} else {
		color = CUiLuaLayout::GetColor(L, 3);
	}

	messageframe->AddMessage(text, color.GetColor());
	return 0;
}

// ----------------------------------------------------------------------------------
int MessageFrame_GetLines(lua_State* L)
{
	CUiMessageFrame* messageframe = dynamic_cast<CUiMessageFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (messageframe == NULL)
		lua_error(L);

	lua_pushnumber(L, messageframe->GetSizeLines());
	return 1;
}

// ----------------------------------------------------------------------------------
int MessageFrame_GetPosition(lua_State* L)
{
	CUiMessageFrame* messageframe = dynamic_cast<CUiMessageFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (messageframe == NULL)
		lua_error(L);

	lua_pushnumber(L, messageframe->GetLastLine());
	return 1;
}

// ----------------------------------------------------------------------------------
int MessageFrame_SetPosition(lua_State* L)
{
	CUiMessageFrame* messageframe = dynamic_cast<CUiMessageFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (messageframe == NULL)
		lua_error(L);

	messageframe->ResetTime();
	messageframe->SetLastLine(luaL_checkint(L, 2));
	return 0;
}

// ----------------------------------------------------------------------------------
int	MessageFrame_ResetTime(lua_State* L)
{
	CUiMessageFrame* messageframe = dynamic_cast<CUiMessageFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (messageframe == NULL)
		lua_error(L);

	messageframe->ResetTime();
	return 0;
}

// ----------------------------------------------------------------------------------
int MessageFrame_ScrollDown(lua_State* L)
{
	CUiMessageFrame* messageframe = dynamic_cast<CUiMessageFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (messageframe == NULL)
		lua_error(L);

	messageframe->ScrollDown();
	return 0;
}

// ----------------------------------------------------------------------------------
int MessageFrame_ScrollUp(lua_State* L)
{
	CUiMessageFrame* messageframe = dynamic_cast<CUiMessageFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (messageframe == NULL)
		lua_error(L);

	messageframe->ScrollUp();
	return 0;
}

// ----------------------------------------------------------------------------------
int MessageFrame_ScrollBottom(lua_State* L)
{
	CUiMessageFrame* messageframe = dynamic_cast<CUiMessageFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (messageframe == NULL)
		lua_error(L);

	messageframe->ScrollBottom();
	return 0;
}
// ----------------------------------------------------------------------------------
int MessageFrame_ScrollTop(lua_State* L)
{
	CUiMessageFrame* messageframe = dynamic_cast<CUiMessageFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (messageframe == NULL)
		lua_error(L);

	messageframe->ScrollTop();
	return 0;
}
// ----------------------------------------------------------------------------------
int MessageFrame_ClearText(lua_State* L)
{
	CUiMessageFrame* messageframe = dynamic_cast<CUiMessageFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (messageframe == NULL)
		lua_error(L);

	messageframe->ClearText();
	return 0;
}
// ----------------------------------------------------------------------------------
int MessageFrame_SetFontSize(lua_State* L)
{
	CUiMessageFrame* messageframe = dynamic_cast<CUiMessageFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (messageframe == NULL)
		lua_error(L);

	// 取得設定字型大小
	int fontSize = luaL_checkint(L, 2);

	CUiFontString* fontString = messageframe->GetFontString();
	if ( fontString )
	{
		CRuFontType* oldFontType = fontString->GetFontType();
		if ( oldFontType )
		{
			CRuFontType* pFontType = CRuFontType::GetFontType(oldFontType->GetFontName(), fontSize, oldFontType->GetWeight(), oldFontType->GetOutLine());
			if ( pFontType )
			{
				fontString->SetFontType(pFontType);
				messageframe->ResetFontString();
			}
		}
	}
	return 0;
}
// ----------------------------------------------------------------------------------
int MessageFrame_EnableHyperlink(lua_State* L)
{
	CUiMessageFrame* messageframe = dynamic_cast<CUiMessageFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (messageframe == NULL)
		lua_error(L);

	messageframe->EnableHyperlink(lua_toboolean(L, 2) != 0);
	return 0;
}
// ----------------------------------------------------------------------------------
int MessageFrame_SetLineBytes(lua_State* L)
{
	CUiMessageFrame* messageframe = dynamic_cast<CUiMessageFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (messageframe == NULL)
		lua_error(L);

	messageframe->SetLineBytes(luaL_checkint(L, 2));
	return 0;
}
// ----------------------------------------------------------------------------------
int MessageFrame_GetPageLines(lua_State* L)
{
	CUiMessageFrame* messageframe = dynamic_cast<CUiMessageFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (messageframe == NULL)
		lua_error(L);

	lua_pushnumber(L, messageframe->GetPageLines());
	return 1;
}

// ----------------------------------------------------------------------------------
int MessageFrame_GetFontString(lua_State* L)
{
	CUiMessageFrame* messageframe = dynamic_cast<CUiMessageFrame*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (messageframe == NULL)
		lua_error(L);

	CUiLuaLayout::PushLayoutTable(L, messageframe->GetFontString());
	return 1;
}