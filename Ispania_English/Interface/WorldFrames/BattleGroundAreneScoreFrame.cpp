#include "../mainproc/GameMain.h"
#include "BattleGroundAreneScoreFrame.h"
#include "../../netwaker_member/Net_BattleGround/NetCli_BattleGround_Child.h"
#include "../../netwaker_member/Net_Other/NetCli_OtherChild.h"

int LuaFunc_GetArenaScoreCareerTotal(lua_State* L);
int LuaFunc_GetArenaScoreWeekSimple(lua_State* L);
int LuaFunc_ShowDetailArenaInfo(lua_State* L);
int LuaFunc_GetDetailArenaInfo(lua_State* L);
int LuaFunc_SwitchTimeUnit(lua_State* L);
int LuaFunc_GetTimeUnit(lua_State* L);
int LuaFunc_GetArenaMemberNum(lua_State* L);
int LuaFunc_GetArenaMemberInfo(lua_State* L);

CBattleGroundArenaScoreFrame*	g_pBattleGroundArenaScoreFrame = NULL;

CBattleGroundArenaScoreFrame::CBattleGroundArenaScoreFrame(CInterface* object)
: CInterfaceBase(object)
{
	g_pBattleGroundArenaScoreFrame = this;

	SecureZeroMemory(m_ArenaInfos, sizeof(m_ArenaInfos));

	m_TimeToUpdate = 0;

	m_ArenaInfos[0].iArenaType = BG_ARENA_TYPE_1_VS_1;
	m_ArenaInfos[0].iTeamGUID = 1;
	m_ArenaInfos[0].ArenaInfo.iWeekJoinCount = 10;
	m_ArenaInfos[0].ArenaInfo.iWeekLoseCount = 4;
	m_ArenaInfos[0].ArenaInfo.iWeekWinCount  = 6;

	m_ArenaInfos[2].iArenaType = BG_ARENA_TYPE_6_VS_6;
	m_ArenaInfos[2].iTeamGUID = 3;
	m_ArenaInfos[2].ArenaInfo.iWeekJoinCount = 20;
	m_ArenaInfos[2].ArenaInfo.iWeekLoseCount = 12;
	m_ArenaInfos[2].ArenaInfo.iWeekWinCount  = 8;
}

CBattleGroundArenaScoreFrame::~CBattleGroundArenaScoreFrame()
{

}

void CBattleGroundArenaScoreFrame::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{
		lua_register(L, "GetArenaScoreCareerTotal"  ,	LuaFunc_GetArenaScoreCareerTotal);
		lua_register(L, "GetArenaScoreWeekSimple"   ,	LuaFunc_GetArenaScoreWeekSimple);
		lua_register(L, "ShowDetailArenaInfo"	    ,	LuaFunc_ShowDetailArenaInfo);
		lua_register(L, "GetDetailArenaInfo"		,	LuaFunc_GetDetailArenaInfo);
		lua_register(L, "SwitchArenaInfoTimeUint"   ,	LuaFunc_SwitchTimeUnit);
		lua_register(L, "GetArenaInfoTimeUint"      ,	LuaFunc_GetTimeUnit);
		lua_register(L, "GetArenaMemberNum"         ,	LuaFunc_GetArenaMemberNum);
		lua_register(L, "GetArenaMemberInfo"        ,	LuaFunc_GetArenaMemberInfo);
	}
}

void CBattleGroundArenaScoreFrame::Update(float elapsedTime)
{
	if (m_TimeToUpdate > 0)
	{
		m_TimeToUpdate -= elapsedTime;

		if (m_TimeToUpdate <= 0)
		{
			g_pBattleGroundArenaScoreFrame->SendWorldEvent(UiEvent_UpdateCharArenaScoreDetailFrame);
			m_TimeToUpdate = 0;			
		}
	}
}

void CBattleGroundArenaScoreFrame::UpdateArenaData(int iArenaType, int iTeamGUID, StructArenaInfo* pArenaInfo)
{
	if (pArenaInfo == NULL)
	{
		return;
	}

	if (iArenaType < 0 || iArenaType > 3)
	{
		return;
	}

	m_ArenaInfos[iArenaType].iArenaType = iArenaType;
	m_ArenaInfos[iArenaType].iTeamGUID  = iTeamGUID;
	m_ArenaInfos[iArenaType].ArenaInfo  = (*pArenaInfo);
}

int CBattleGroundArenaScoreFrame::GetCarrerTotalWinCount()
{
	int Result = 0;
	for (int i = 0; i < BG_ARENA_TYPE_MAX; ++i)
	{
		if (m_ArenaInfos[i].iArenaType > 0)
		{
			Result += m_ArenaInfos[i].ArenaInfo.iLifeWinCount;
		}		
	}

	return Result;
}

int CBattleGroundArenaScoreFrame::GetCarrerTotalLoseCount()
{
	int Result = 0;
	for (int i = 0; i < BG_ARENA_TYPE_MAX; ++i)
	{
		if (m_ArenaInfos[i].iArenaType > 0)
		{
			Result += m_ArenaInfos[i].ArenaInfo.iLifeLoseCount;
		}		
	}

	return Result;
}

int LuaFunc_GetArenaScoreCareerTotal(lua_State* L)
{
	int iTotalWin   = g_pBattleGroundArenaScoreFrame->GetCarrerTotalWinCount();
	int iTotalLose  = g_pBattleGroundArenaScoreFrame->GetCarrerTotalLoseCount();
	int iWinProb    = 0;
	int iArenaPoint = 0;

	int iTotalCount = iTotalWin + iTotalLose;
	if (iTotalCount > 0)
	{
		iWinProb = iTotalWin / iTotalCount;
	}
	
	lua_pushnumber( L, iTotalWin);
	lua_pushnumber( L, iTotalLose);
	lua_pushnumber( L, iWinProb);
	lua_pushnumber( L, iArenaPoint);

	return 4;
}

int LuaFunc_GetArenaScoreWeekSimple(lua_State* L)
{	
	int iIndex = luaL_checkint( L, 1 ) - 1;

	if (iIndex < 0 || iIndex >= BG_ARENA_TYPE_MAX)
	{
		return 0;
	}

	StructArenaInfoEx* pInfo = g_pBattleGroundArenaScoreFrame->GetInfo(iIndex);

	int iArenaType = pInfo->iArenaType;
	int iTeamGUID  = pInfo->iTeamGUID;
	int iPlayCount = pInfo->ArenaInfo.iWeekJoinCount; //need to change
	int iWinCount  = pInfo->ArenaInfo.iWeekWinCount;
	int iLoseCount = pInfo->ArenaInfo.iWeekLoseCount;
	int iJoinCount = pInfo->ArenaInfo.iWeekJoinCount;
	int iAreaPoint = (int)pInfo->ArenaInfo.fArenaPoint;

	lua_pushnumber( L, iArenaType);
	lua_pushnumber( L, iTeamGUID);
	lua_pushnumber( L, iPlayCount);
	lua_pushnumber( L, iWinCount);
	lua_pushnumber( L, iLoseCount);
	lua_pushnumber( L, iJoinCount);
	lua_pushnumber( L, iAreaPoint);

	return 7;
}

int LuaFunc_ShowDetailArenaInfo(lua_State* L)
{
	int iType = luaL_checkint( L, 1 );

	g_pBattleGroundArenaScoreFrame->SetActiveDetailArenaType(iType - 1);

	g_pBattleGroundArenaScoreFrame->SendWorldEvent(UiEvent_OpenCharArenaScoreDetailFrame);

	return 0;
}

int LuaFunc_GetDetailArenaInfo(lua_State* L)
{		
	int  iIndex    = g_pBattleGroundArenaScoreFrame->GetActiveDetailArenaType();
	int  iTimeUnit = g_pBattleGroundArenaScoreFrame->GetTimeUnit();

	StructArenaInfoEx* pInfo = g_pBattleGroundArenaScoreFrame->GetInfo(iIndex);

	int iArenaType				= pInfo->iArenaType;
	int iTeamGUID				= pInfo->iTeamGUID;

	int iTotalWin				= 0;
	int iTotalLose				= 0;
	int iArenaPoint				= 0;
	int iPlayCountThis			= 0;
	int iWinCountThis			= 0;
	int iLoseCountThis			= 0;
	int iJoinCountThis			= 0;
	int iPlayCountLast			= 0;
	int iWinCountLast			= 0;
	int iLoseCountLast			= 0;
	int iJoinCountLast			= 0;
	
	int iRankLast				= 0;
	int iTotalDefeatCarrer		= 0;
	int iTotalDefeatedCarrer	= 0;
	int iJoinCount2Carrer		= 0;

	int iTotalDefeatLast		= 0;
	int iTotalDefeatedLast		= 0;
	int iJoinCount2Last			= 0;

	//weekly
	if (iTimeUnit == 0)
	{
		iTotalWin				= 0;
		iTotalLose				= 0;
		iArenaPoint				= 0;
		iPlayCountThis			= pInfo->ArenaInfo.iWeekJoinCount;
		iWinCountThis			= pInfo->ArenaInfo.iWeekWinCount;
		iLoseCountThis			= pInfo->ArenaInfo.iWeekLoseCount;
		iJoinCountThis			= pInfo->ArenaInfo.iWeekJoinCount;
		iPlayCountLast			= pInfo->ArenaInfo.iWeekJoinCount;
		iWinCountLast			= pInfo->ArenaInfo.iWeekWinCount;
		iLoseCountLast			= pInfo->ArenaInfo.iWeekLoseCount;
		iJoinCountLast			= 0;

		iRankLast				= 0;
		iTotalDefeatCarrer		= 0;
		iTotalDefeatedCarrer	= 0;
		iJoinCount2Carrer		= 0;

		iTotalDefeatLast		= 0;
		iTotalDefeatedLast		= 0;
		iJoinCount2Last			= 0;
	}
	//monthly
	else if (iTimeUnit == 1)
	{
		iTotalWin				= 0;
		iTotalLose				= 0;
		iArenaPoint				= 0;
		iPlayCountThis			= pInfo->ArenaInfo.iWeekJoinCount;
		iWinCountThis			= pInfo->ArenaInfo.iWeekWinCount;
		iLoseCountThis			= pInfo->ArenaInfo.iWeekLoseCount;
		iJoinCountThis			= pInfo->ArenaInfo.iWeekJoinCount;
		iPlayCountLast			= pInfo->ArenaInfo.iWeekJoinCount;
		iWinCountLast			= pInfo->ArenaInfo.iWeekWinCount;
		iLoseCountLast			= pInfo->ArenaInfo.iWeekLoseCount;
		iJoinCountLast			= 0;

		iRankLast				= 0;
		iTotalDefeatCarrer		= 0;
		iTotalDefeatedCarrer	= 0;
		iJoinCount2Carrer		= 0;

		iTotalDefeatLast		= 0;
		iTotalDefeatedLast		= 0;
		iJoinCount2Last			= 0;
	}


	lua_pushnumber( L, iArenaType);
	lua_pushnumber( L, iTeamGUID);
	lua_pushnumber( L, iTotalWin);
	lua_pushnumber( L, iTotalLose);
	lua_pushnumber( L, iArenaPoint);
	lua_pushnumber( L, iPlayCountThis);
	lua_pushnumber( L, iWinCountThis);
	lua_pushnumber( L, iLoseCountThis);
	lua_pushnumber( L, iJoinCountThis);
	lua_pushnumber( L, iPlayCountLast);
	lua_pushnumber( L, iWinCountLast);
	lua_pushnumber( L, iLoseCountLast);
	lua_pushnumber( L, iJoinCountLast);
	lua_pushnumber( L, iRankLast);
	lua_pushnumber( L, iTotalDefeatCarrer);
	lua_pushnumber( L, iTotalDefeatedCarrer);
	lua_pushnumber( L, iJoinCount2Carrer);
	lua_pushnumber( L, iTotalDefeatLast);
	lua_pushnumber( L, iTotalDefeatedLast);
	lua_pushnumber( L, iJoinCount2Last);

	return 20;
}

int LuaFunc_SwitchTimeUnit(lua_State* L)
{
	int iTimeUnit = g_pBattleGroundArenaScoreFrame->GetTimeUnit();

	if (iTimeUnit == 0)
	{
		iTimeUnit = 1;
	}
	else if(iTimeUnit == 1)
	{
		iTimeUnit = 0;
	}

	g_pBattleGroundArenaScoreFrame->SetTimeUnit(iTimeUnit);

	lua_pushnumber( L, iTimeUnit);

	return 1;
}

int LuaFunc_GetTimeUnit(lua_State* L)
{
	int iTimeUnit = g_pBattleGroundArenaScoreFrame->GetTimeUnit();

	lua_pushnumber( L, iTimeUnit);

	return 1;
}

int LuaFunc_GetArenaMemberNum(lua_State* L)
{
	int iTypeIndex = g_pBattleGroundArenaScoreFrame->GetActiveDetailArenaType();
	StructArenaInfoEx* pInfo = g_pBattleGroundArenaScoreFrame->GetInfo(iTypeIndex);

	int iMemberNum = pInfo->ArenaInfo.iNumTeamMember;

	lua_pushnumber( L, iMemberNum);

	return 1;
}

int LuaFunc_GetArenaMemberInfo(lua_State* L)
{
	int iTypeIndex = g_pBattleGroundArenaScoreFrame->GetActiveDetailArenaType();
	StructArenaInfoEx* pInfo = g_pBattleGroundArenaScoreFrame->GetInfo(iTypeIndex);

	int iMemberIndex = luaL_checkint( L, 1 ) - 1;

	string strMemberName = "";
	int iDBID            = pInfo->ArenaInfo.TeamMember[iMemberIndex].iDBID;
	int iWeekWinCount    = pInfo->ArenaInfo.TeamMember[iMemberIndex].iWeekWinCount;
	int iWeekLoseCount   = pInfo->ArenaInfo.TeamMember[iMemberIndex].iWeekLoseCount;
	int iWeekJoinCount   = 0;

	FindDBIDRoleNameStruct* pFindStruct = NetCli_OtherChild::GetFindDBIDRoleNameStruct( iDBID );

	if( pFindStruct && pFindStruct->IsFind )
	{
		strMemberName = pFindStruct->RoleName;
	}
	else
	{
		g_pBattleGroundArenaScoreFrame->Update_After_n_Sec(3.0f);
	}

	lua_pushstring( L, strMemberName.c_str());
	lua_pushnumber( L, iWeekWinCount);
	lua_pushnumber( L, iWeekLoseCount);
	lua_pushnumber( L, iWeekJoinCount);

	return 4;
}