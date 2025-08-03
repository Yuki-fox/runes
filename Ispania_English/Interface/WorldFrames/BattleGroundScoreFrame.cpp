#include "../mainproc/GameMain.h"
#include "BattleGroundScoreFrame.h"
#include "../../netwaker_member/Net_BattleGround/NetCli_BattleGround_Child.h"

CBattleGroundScoreFrame*	g_pBattleGroundScoreFrame = NULL;
int LuaFunc_GetBattleGroundType(lua_State* L);

CBattleGroundScoreFrame::CBattleGroundScoreFrame(CInterface* object)
: CInterfaceBase(object)
{
	g_pBattleGroundScoreFrame = this;
}

CBattleGroundScoreFrame::~CBattleGroundScoreFrame()
{
	g_pBattleGroundScoreFrame = NULL;	
}

void CBattleGroundScoreFrame::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{
		lua_register(L, "GetBattleGroundType",	LuaFunc_GetBattleGroundType);
	}
}

void CBattleGroundScoreFrame::UpdateScore(StructBattleGroundValue* pData)
{
	if (pData == NULL)
	{
		return;
	}

	lua_State* L = GetLuaStateWorld();

	lua_pushnumber(L, pData->iTeamScore[0]);
	lua_setglobal(L, "arg1");
	lua_pushnumber(L, pData->iTeamScore[1]);
	lua_setglobal(L, "arg2");
	lua_pushnumber(L, pData->iTeamScore[2]);
	lua_setglobal(L, "arg3");
	lua_pushnumber(L, pData->iTeamScore[3]);
	lua_setglobal(L, "arg4");

	SendWorldEvent(UiEvent_UpdateBattleGroundCampScore);
}

int LuaFunc_GetBattleGroundType(lua_State* L)
{
	if (g_pGameMain)
	{
		int BattleGroundType = g_pGameMain->GetBattleGroundType();
		lua_pushnumber( L, BattleGroundType );
		return 1;
	}

	return 0;
}