#include "../GameMain.h"
#include "CharacterCreate.h"

// --------------------------------------------------------------------------------------
// Globals
CCharacterCreate*						g_pCharacterCreate = NULL;

// --------------------------------------------------------------------------------------
void CCharacterCreate::Initial()
{
	if ( g_pCharacterCreate == NULL )
	{
		g_pCharacterCreate = new CCharacterCreate;		
	}
}

// --------------------------------------------------------------------------------------
void CCharacterCreate::Release()
{
	SAFE_DELETE( g_pCharacterCreate );
}

// --------------------------------------------------------------------------------------
CCharacterCreate::CCharacterCreate()
{
}

// --------------------------------------------------------------------------------------
CCharacterCreate::~CCharacterCreate()
{
}

// --------------------------------------------------------------------------------------
void RegisterCharacterCreateFunc()
{
	g_pLuaScript->Register( "CreateCharacter",			CreateCharacter );
}

// --------------------------------------------------------------------------------------
int CreateCharacter( lua_State *L )
{
	luaStack l(L);
	if ( l.GetParamCount() >= 1 )
	{
		CreateRoleData role;
        role.SampleRole.init();
		role.SampleRole.RoleName = l.GetParam<const char*>(1);
		role.iRoleBaseID = 1000;
		role.iBodyColor = 2;
		role.iHairColor = 2;
		role.iFaceID = 2000;
		role.iHairID = 2001;
		CNetCli_Login::CreateRole( &role );
	}
	return 0;
}