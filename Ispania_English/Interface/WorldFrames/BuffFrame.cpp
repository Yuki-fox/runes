#include "../../netwaker_member/NetWakerClientInc.h"
#include "../../mainproc/GameMain.h"
#include "BuffFrame.h"

CBuffFrame*					g_pBuffFrame = NULL;

int	LuaFunc_GetPlayerBuff			(lua_State* L);
int	LuaFunc_GetPlayerBuffTexture	(lua_State* L);
int	LuaFunc_GetPlayerBuffLeftTime	(lua_State* L);
int	LuaFunc_GetTargetBuff			(lua_State* L);
int	LuaFunc_GetTargetBuffTexture	(lua_State* L);
int	LuaFunc_GetPartyBuff			(lua_State* L);
int	LuaFunc_CancelPlayerBuff		(lua_State* L);

// ----------------------------------------------------------------------------
CBuffFrame::CBuffFrame(CInterface* object)
	: CInterfaceBase(object)
{
	g_pBuffFrame = this;
}

// ----------------------------------------------------------------------------
CBuffFrame::~CBuffFrame()
{
	g_pBuffFrame = NULL;
}

// ----------------------------------------------------------------------------
void CBuffFrame::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{
		lua_register( L, "GetPlayerBuff",			LuaFunc_GetPlayerBuff );
		lua_register( L, "GetPlayerBuffTexture",	LuaFunc_GetPlayerBuffTexture );
		lua_register( L, "GetPlayerBuffLeftTime",	LuaFunc_GetPlayerBuffLeftTime );		
		lua_register( L, "GetTargetBuff",			LuaFunc_GetTargetBuff );
		lua_register( L, "GetTargetBuffTexture",	LuaFunc_GetTargetBuffTexture );
		lua_register( L, "GetPartyBuff",			LuaFunc_GetPartyBuff );
		lua_register( L, "CancelPlayerBuff",		LuaFunc_CancelPlayerBuff );
	}
}

// ----------------------------------------------------------------------------
int	LuaFunc_GetPlayerBuff(lua_State* L)
{
	int index = -1;
	int id = luaL_checkint(L, 1) - 1;
	const char* filter = luaL_checkstring(L, 2);

	CRoleSprite* player = g_pGameMain->GetPlayer();
	if ( player )
	{
		if ( strcmp(filter, "ALL") == 0 )
		{
			if ( player->GetBuffInfo(id) )
				index = id;
		}
		else if ( strcmp(filter, "HELPFUL") == 0 )
		{
			index = player->GetBuffHelpfulIndex(id);
		}
		else if ( strcmp(filter, "HARMFUL") == 0 )
		{
			index = player->GetBuffHarmfulIndex(id);
		}
	}
	lua_pushnumber(L, index);
	return 1;
}

// ----------------------------------------------------------------------------
int	LuaFunc_GetPlayerBuffTexture(lua_State* L)
{
	CRoleSprite* player = g_pGameMain->GetPlayer();
	if ( player )
	{
		RoleBuffInfo* buff = player->GetBuffInfo(luaL_checkint(L, 1));
		if ( buff )
		{
			GameObjDbStructEx* itemDB = g_ObjectData->GetObj(buff->id);
			if ( itemDB )
			{
				GameObjDbStructEx* imageDB = CNetGlobal::GetObj(itemDB->ImageID);
				if ( imageDB )
				{
					lua_pushstring( L, imageDB->Image.ACTField );
					return 1;
				}
			}
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------
int	LuaFunc_GetPlayerBuffLeftTime(lua_State* L)
{
	int time = 0;
	CRoleSprite* player = g_pGameMain->GetPlayer();
	if ( player )
	{
		RoleBuffInfo* buff = player->GetBuffInfo(luaL_checkint(L, 1));
		if ( buff )
		{
			time = (int)buff->time;
		}
	}
	lua_pushnumber(L, time);
	return 1;
}

// ----------------------------------------------------------------------------
int	LuaFunc_GetTargetBuff(lua_State* L)
{
	int index = -1;
	int id = luaL_checkint(L, 1);
	const char* filter = luaL_checkstring(L, 2);

	CRoleSprite* player = g_pGameMain->GetPlayerTarget();
	if ( player )
	{
		if ( strcmp(filter, "ALL") == 0 )
		{
			if ( player->GetBuffInfo(id) )
				index = id;
		}
		else if ( strcmp(filter, "HELPFUL") == 0 )
		{
			index = player->GetBuffHelpfulIndex(id);
		}
		else if ( strcmp(filter, "HARMFUL") == 0 )
		{
			index = player->GetBuffHarmfulIndex(id);
		}
	}
	lua_pushnumber(L, index);
	return 1;
}

// ----------------------------------------------------------------------------
int	LuaFunc_GetTargetBuffTexture(lua_State* L)
{
	CRoleSprite* player = g_pGameMain->GetPlayerTarget();
	if ( player )
	{
		RoleBuffInfo* buff = player->GetBuffInfo(luaL_checkint(L, 1));
		if ( buff )
		{
			GameObjDbStructEx* itemDB = g_ObjectData->GetObj(buff->id);
			if ( itemDB )
			{
				GameObjDbStructEx* imageDB = g_ObjectData->GetObj(itemDB->ImageID);
				if ( imageDB )
				{
					lua_pushstring( L, imageDB->Image.ACTField );
					return 1;
				}
			}
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------
int	LuaFunc_GetPartyBuff(lua_State* L)
{
	return 0;
}

// ----------------------------------------------------------------------------
int	LuaFunc_CancelPlayerBuff(lua_State* L)
{
	CRoleSprite* player = g_pGameMain->GetPlayer();
	if ( player )
	{
		int index = player->GetBuffHelpfulIndex(luaL_checkint(L, 1) - 1);
		RoleBuffInfo* buff = player->GetBuffInfo(index);
		if ( buff )
		{
			NetCli_Magic::CancelAssistMagic(buff->id);
		}
	}
	return 0;
}