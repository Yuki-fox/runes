#include "../mainproc/GameMain.h"
#include "../../netwaker_member/Net_Other/NetCli_OtherChild.h"
#include "GmNotificationFrame.h"

CGmNotificationFrame*	g_pGmNotificationFrame = NULL;

int LuaFunc_GmNotification(lua_State* L);

// ----------------------------------------------------------------------------
CGmNotificationFrame::CGmNotificationFrame(CInterface* object)
: CInterfaceBase(object)
{
	g_pGmNotificationFrame = this;
}

// ----------------------------------------------------------------------------
CGmNotificationFrame::~CGmNotificationFrame()
{
	g_pGmNotificationFrame = NULL;
}

// ----------------------------------------------------------------------------
void CGmNotificationFrame::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{
		lua_register(L, "GmNotification", LuaFunc_GmNotification);
	}
}

void CGmNotificationFrame::GmNotification(const char* pszSubject, const char* pszContent, const char* pszClassification)
{
	NetCli_Other::SL_GmNotification(pszSubject, pszContent, pszClassification);
}

int LuaFunc_GmNotification(lua_State* L)
{
	if (L)
	{
		const char* subject			= luaL_checkstring(L, 1);
		const char* content			= lua_isstring(L,2) ? lua_tostring(L, 2) : "";
		const char* classification	= luaL_checkstring(L, 3);
		g_pGmNotificationFrame->GmNotification(subject, content, classification);
	}
	return 0;
}