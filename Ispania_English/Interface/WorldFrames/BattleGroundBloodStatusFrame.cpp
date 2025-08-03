#include "../mainproc/GameMain.h"
#include "BattleGroundBloodStatusFrame.h"
#include "../../netwaker_member/Net_BattleGround/NetCli_BattleGround_Child.h"
#include "../../netwaker_member/Net_Other/NetCli_OtherChild.h"
#include "../../netwaker_member/Net_Script/NetCli_Script_Child.h"
#include "../Map/Minimap.h"

CBattleGroundBloodStatusFrame*	g_pBattleGroundBloodStatusFrame = NULL;
//int LuaFunc_BattleGroundScoresInfo(lua_State* L);
//int LuaFunc_GetBattleGroundtVar(lua_State* L);

CBattleGroundBloodStatusFrame::CBattleGroundBloodStatusFrame(CInterface* object)
: CInterfaceBase(object)
{
	g_pBattleGroundBloodStatusFrame=this;
}

CBattleGroundBloodStatusFrame::~CBattleGroundBloodStatusFrame()
{
	g_pBattleGroundBloodStatusFrame = NULL;
}

void CBattleGroundBloodStatusFrame::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();

	if (L)
	{
		//	lua_register(L, "BattleGroundScoresInfo" ,	LuaFunc_BattleGroundScoresInfo);
	}
}


void CBattleGroundBloodStatusFrame::UpdateTowerInfo(StructBattleGroundValue* pData){
	if (pData == NULL)
	{
		return;
	}


	lua_State* L = GetLuaStateWorld();

	lua_pushnumber(L, pData->iVar[1]);
	lua_setglobal(L, "arg1");
	lua_pushnumber(L, pData->iVar[2]);
	lua_setglobal(L, "arg2");
	lua_pushnumber(L, pData->iTeamScore[0]);
	lua_setglobal(L, "arg3");
	lua_pushnumber(L, pData->iTeamScore[1]);
	lua_setglobal(L, "arg4");


	SendWorldEvent(UiEvent_UpdateBattleGroundToweriVar);
}









/*int LuaFunc_BattleGroundScoresInfo(lua_State* L)
{
	int index = luaL_checkint(L,1) - 1;

	CNetCli_BattleGround_Child* pCNetCli_BattleGround_Child = (CNetCli_BattleGround_Child*)CNetCli_BattleGround::m_pThis;


	map< string, StructBattleGroundScore >::iterator itPos = pCNetCli_BattleGround_Child->m_mapBattleGroundScores.begin();

	int i;
	for( i = 0 ; i < index ; ++i  )
	{
		if( itPos != pCNetCli_BattleGround_Child->m_mapBattleGroundScores.end() )
		{
			++itPos;
		}
	}

	if( itPos == pCNetCli_BattleGround_Child->m_mapBattleGroundScores.end() )
	{
		return 0;
	}

	StructBattleGroundScore& rinfo = itPos->second;
	//StructBattleGroundScore* rinfo = &itPos->second;

	lua_pushstring(L, rinfo.Name);	
	lua_pushnumber(L, rinfo.iRoleVar[0]);
	lua_pushnumber(L, rinfo.iRoleVar[1]);

	return 3;
}

*/