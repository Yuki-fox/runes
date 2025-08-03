#pragma	   once

#include <string>
#include "../GameMain.h"

// **********************************************************************************************
// Globals
class	CGlueParent;

extern	CGlueParent					*g_pGlueParent;

// **********************************************************************************************

// functions
void	RegisterGlueParentFunc		();
int		QuitGame					(lua_State *L);
int		getglobal					(lua_State *L);
int		GetCursorPos				(lua_State *L);
int		GetOldCursorPos				(lua_State *L);
int		SetCurrentScreen			(lua_State *L);
int		GetBackPackageMoney			(lua_State *L);

using namespace std;

class CGlueParent
{
public:
	CGlueParent();
	~CGlueParent();

	static void				Initial();
	static void				Release();
	
	void					SetCurrentScreen( const char* name )	{ m_currentScreen = name; }
	const char*				GetCurrentScreen()						{ return m_currentScreen.c_str(); }

protected:
	string					m_currentScreen;
};




