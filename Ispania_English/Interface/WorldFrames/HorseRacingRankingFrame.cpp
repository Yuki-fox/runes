#include <list>
#include <algorithm>
#include "../mainproc/GameMain.h"
#include "HorseRacingRankingFrame.h"
#include "../../netwaker_member/Net_BattleGround/NetCli_BattleGround_Child.h"

CHorseRacingRankingFrame*	g_pHorseRacingRankingFrame = NULL;

int LuaFunc_GetHorseRacingRanking(lua_State* L);
int LuaFunc_GetHorseRacingRankingTime(lua_State* L);
int LuaFunc_GetHorseRacingRankingScore(lua_State* L);
int LuaFunc_GetHorseRacingIsPlayerInGame(lua_State* L);
int LuaFunc_ClearHorseRacingPlayersName(lua_State* L);

bool HorseRacing_Campare_Rank_Less(const StructHorseRacingRankingData & m1, const StructHorseRacingRankingData & m2);

CHorseRacingRankingFrame::CHorseRacingRankingFrame(CInterface* object)
: CInterfaceBase(object)
{
	g_pHorseRacingRankingFrame = this;
}

CHorseRacingRankingFrame::~CHorseRacingRankingFrame()
{
	g_pHorseRacingRankingFrame = NULL;	
}

void CHorseRacingRankingFrame::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{
		lua_register(L, "GetHorseRacingRanking",	LuaFunc_GetHorseRacingRanking);
		lua_register(L, "GetHorseRacingRankingTime",	LuaFunc_GetHorseRacingRankingTime);
		lua_register(L, "GetHorseRacingRankingScore",	LuaFunc_GetHorseRacingRankingScore);
		lua_register(L, "GetHorseRacingIsPlayerInGame",	LuaFunc_GetHorseRacingIsPlayerInGame);
		lua_register(L, "ClearHorseRacingPlayersName",	LuaFunc_ClearHorseRacingPlayersName);
	}
}

void CHorseRacingRankingFrame::FindPlayerNameByTeamID(int iTeamID, string& strOut)
{
	strOut = "";
	CNetCli_BattleGround_Child* tmpThis = (CNetCli_BattleGround_Child*)CNetCli_BattleGround::m_pThis;

	for (map< string, StructBattleGroundScore>::iterator it = tmpThis->m_mapBattleGroundScores.begin(); it != tmpThis->m_mapBattleGroundScores.end(); it++)
	{
		if (it->second.iTeamID == iTeamID)
		{
			strOut = it->first;
			break;
		}			
	}
}

void CHorseRacingRankingFrame::UpdateRanking(StructBattleGroundValue* pData)
{
	if (pData == NULL)
	{
		return;
	}

	StructHorseRacingRankingData tmpData;

	m_RankingData.clear();

	string tmpName = "";

	for (int i = 0; i < DF_BG_MAXTEAM; ++i)
	{
		SecureZeroMemory(&tmpData, sizeof(tmpData));
		
		tmpData.iTeamID = i;

		if (pData->iTeamScore[i] > 0)
		{
			tmpData.iRank = pData->iTeamScore[i];
		}
		else
		{
			tmpData.iRank = 255;
		}

		tmpName = "";
		FindPlayerNameByTeamID(i, tmpName);

		//if (tmpName != "")
		//{
		//	m_PlayersName[i] = tmpName;
		//	tmpData.iInGame = 1;
		//}
		//else
		//{
		//	if (m_PlayersName[i] != "")
		//	{
		//		tmpName = m_PlayersName[i];				
		//	}
		//	tmpData.iInGame = 0;
		//}

		sprintf(tmpData.szName, "%s", tmpName.c_str());

		tmpData.iTime = pData->iVar[i + 1];

		tmpData.iInGame = 1;

		m_RankingData.push_back(tmpData);
	}

	sort(m_RankingData.begin(), m_RankingData.end(), HorseRacing_Campare_Rank_Less);

	SendWorldEvent(UiEvent_HorseRacingRanking);
}

void CHorseRacingRankingFrame::FinalRanking(int iWinTeam)
{
	lua_State* L = GetLuaStateWorld();
	lua_pushnumber(L, iWinTeam);
	lua_setglobal(L, "arg1");
	SendWorldEvent(UiEvent_HorseRacingFinalRanking);
}

int LuaFunc_GetHorseRacingRanking(lua_State* L)
{
	int iCount = luaL_checkint( L, 1 );

	if (iCount > DF_BG_MAXTEAM)
	{
		iCount = DF_BG_MAXTEAM;
	}

	if (iCount > g_pHorseRacingRankingFrame->m_RankingData.size())
	{
		iCount = (int)g_pHorseRacingRankingFrame->m_RankingData.size();
	}

	for (int i = 0; i < iCount; ++i)
	{
		lua_pushstring(L, g_pHorseRacingRankingFrame->m_RankingData[i].szName);
	}

	return iCount;
}

int LuaFunc_GetHorseRacingRankingTime(lua_State* L)
{
	int iCount = luaL_checkint( L, 1 );

	if (iCount > DF_BG_MAXTEAM)
	{
		iCount = DF_BG_MAXTEAM;
	}

	if (iCount > g_pHorseRacingRankingFrame->m_RankingData.size())
	{
		iCount = (int)g_pHorseRacingRankingFrame->m_RankingData.size();
	}

	for (int i = 0; i < iCount; ++i)
	{
		lua_pushnumber(L, g_pHorseRacingRankingFrame->m_RankingData[i].iTime);
	}

	return iCount;
}

int LuaFunc_GetHorseRacingRankingScore(lua_State* L)
{
	int iCount = luaL_checkint( L, 1 );

	if (iCount > DF_BG_MAXTEAM)
	{
		iCount = DF_BG_MAXTEAM;
	}

	if (iCount > g_pHorseRacingRankingFrame->m_RankingData.size())
	{
		iCount = (int)g_pHorseRacingRankingFrame->m_RankingData.size();
	}

	for (int i = 0; i < iCount; ++i)
	{
		lua_pushnumber(L, g_pHorseRacingRankingFrame->m_RankingData[i].iRank);
	}

	return iCount;
}

int LuaFunc_GetHorseRacingIsPlayerInGame(lua_State* L)
{
	int iCount = luaL_checkint( L, 1 );

	if (iCount > DF_BG_MAXTEAM)
	{
		iCount = DF_BG_MAXTEAM;
	}

	if (iCount > g_pHorseRacingRankingFrame->m_RankingData.size())
	{
		iCount = (int)g_pHorseRacingRankingFrame->m_RankingData.size();
	}

	for (int i = 0; i < iCount; ++i)
	{
		lua_pushnumber(L, g_pHorseRacingRankingFrame->m_RankingData[i].iInGame);
	}

	return iCount;
}

int LuaFunc_ClearHorseRacingPlayersName(lua_State* L)
{
	//for (int i = 0; i < DF_BG_MAXTEAM; ++i)
	//{
	//	g_pHorseRacingRankingFrame->m_PlayersName[i] = "";
	//}
	return 0;
}


bool HorseRacing_Campare_Rank_Less(const StructHorseRacingRankingData & m1, const StructHorseRacingRankingData & m2)
{
	return m1.iRank < m2.iRank;
}