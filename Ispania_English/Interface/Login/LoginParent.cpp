#include "../../mainproc/GameMain.h"
#include "AccountLogin.h"
#include "CharacterCreate.h"
#include "CharacterSelect.h"
#include "LoginParent.h"

CLoginParent*				g_pLoginParent = NULL;

int	LuaFunc_QuitGame					(lua_State *L);
int	LuaFunc_SetCurrentScreen			(lua_State *L);
int	LuaFunc_GetVersion					(lua_State *L);

// ----------------------------------------------------------------------------
CLoginParent::CLoginParent(CInterface* object) : CInterfaceBase(object)
{
	m_currentScreen.clear();
	//初始化亂數種子 螢幕小鍵盤的亂數地點
	int time=timeGetTime();
	srand(time);

	g_pLoginParent = this;
}

// ----------------------------------------------------------------------------
CLoginParent::~CLoginParent()
{
	g_pLoginParent = NULL;
}

// ----------------------------------------------------------------------------
void CLoginParent::RegisterFunc()
{
	lua_State* L = GetLuaState();
	if (L)
	{
		lua_register(L, "QuitGame",					LuaFunc_QuitGame);
		lua_register(L, "SetCurrentScreen",			LuaFunc_SetCurrentScreen);
		lua_register(L, "GetVersion",				LuaFunc_GetVersion);
	}
}

// --------------------------------------------------------------------------------------
void CLoginParent::SendLoginScreen(const char* arg)
{
	lua_State* L = m_interface->GetUiLogin()->GetLuaState();
	if ( L )
	{
		lua_pushstring(L, arg);
		lua_setglobal(L, "arg1");
		SendLoginEvent("SET_LOGIN_SCREEN");
	}
}

// ----------------------------------------------------------------------------
int	LuaFunc_QuitGame (lua_State *L)
{
	g_pGameMain->Exit();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_SetCurrentScreen (lua_State *L)
{
	static bool firstLogin = true;

	const char* szCurrentScreen = luaL_checkstring(L, 1);

	if ( strcmp(szCurrentScreen, GLUE_SCREEN_LOGIN) == 0 )
	{
		if ( !firstLogin )
		{
			const char* account = g_pGameMain->GetAccountName();
			const char* password = g_pGameMain->GetAccountPassword();

#ifdef KALYDO
			// we are returning to the login screen, use single sign-on to login
			// we've logged in before so the credentials should already be there
			g_pAccountLogin->SetShouldLogin(true);
			application->loginWithSSO();
#endif // KALYDO
			// 自動傳入帳密
			if ( strcmp(g_pGameMain->GetCountry(), "TW") == 0 )
			{
				if ( strlen(account) > 0 && strlen(password) > 0 )
				{
					g_pAccountLogin->Login(account, password);
				}
			}
		}
	}

	firstLogin = false;

	g_pLoginParent->SetCurrentScreen( szCurrentScreen );
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetVersion(lua_State *L)
{
	if ( g_pGameMain ) {
		lua_pushstring(L, g_pGameMain->GetVersionNO());
	} else {
		lua_pushnil(L);
	}
	return 1;
}