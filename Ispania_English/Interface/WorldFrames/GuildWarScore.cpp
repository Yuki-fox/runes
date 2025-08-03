#include "../../mainproc/GameMain.h"
#include "../../netwaker_member/Net_BG_GuildWar/NetCli_BG_GuildWar.h"

#include "../Map/Minimap.h"
#include "GuildWarScore.h"
#include "../GuildHouses/GuildHouseStatus.h"
#include "../GuildHouses/GuildHousesFrame.h"
#include "../../netwaker_member/Net_Guild/NetCli_GuildChild.h"
#include "GuildFrame.h"


#define UiEvent_GuildWarScore "GUILDWARSCORE"

int LuaFunc_GetGuildHouseWarInfo(lua_State* L);
int LuaFunc_GetGuildHouseGuildName(lua_State* L);
int LuaFunc_GetGuildHouseGuildTime(lua_State* L);
//****************************************************************************
CGuildWarScore*		g_pGuildWarScore = NULL;

//****************************************************************************
//****************************************************************************
//----------------------------------------------------------------------------

CGuildWarScore::CGuildWarScore(CInterface* object)
: CInterfaceBase(object)
{
	g_pGuildWarScore = this;
	m_EnergyB = 0;
	m_EnergyR = 0;
	m_guildIDB = 0;
	m_guildIDR = 0;
	m_guildScoreB = 5000;
	m_guildScoreR = 5000;
	//m_BeginTime = 0;
	//m_EndTime = 0;
}

//----------------------------------------------------------------------------
CGuildWarScore::~CGuildWarScore()
{
	g_pGuildWarScore = NULL;
}

//----------------------------------------------------------------------------
void CGuildWarScore::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();

	if (L)
	{
			lua_register(L, "GetGuildHouseWarInfo",			LuaFunc_GetGuildHouseWarInfo		);
			lua_register(L, "GetGuildHouseGuildName",		LuaFunc_GetGuildHouseGuildName		);
			lua_register(L, "GetGuildHouseGuildTime",		LuaFunc_GetGuildHouseGuildTime		);

	}
}

// ----------------------------------------------------------------------------
void CGuildWarScore::Update(float elapsedTime)
{
	if( m_RemainTime > 0 ){
		m_RemainTime -= elapsedTime;
	}
}
//----------------------------------------------------------------------------
void CGuildWarScore::RBL_GuildScore(GuildHouseWarFightStruct& Info , int Time )
{
	m_GuildInfo = Info;
	//m_BeginTime = m_GuildInfo.BeginTime;
	//m_EndTime = m_GuildInfo.EndTime;

	m_RemainTime = (float)Time;
	m_EnergyB = m_GuildInfo.Base[0].Energy;
	m_EnergyR = m_GuildInfo.Base[1].Energy;
	m_guildIDB = m_GuildInfo.Base[0].GuildID;
	m_guildIDR = m_GuildInfo.Base[1].GuildID;
	m_guildScoreB = m_GuildInfo.Base[0].Score;
	m_guildScoreR = m_GuildInfo.Base[1].Score;

	SendWorldEvent(UiEvent_GuildWarScore);
}

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
int LuaFunc_GetGuildHouseWarInfo(lua_State* L)
{
	lua_pushnumber(L, g_pGuildWarScore->m_EnergyB);
	lua_pushnumber(L, g_pGuildWarScore->m_EnergyR);
	lua_pushnumber(L, g_pGuildWarScore->m_guildScoreB);
	lua_pushnumber(L, g_pGuildWarScore->m_guildScoreR);
	lua_pushnumber(L, g_pGuildWarScore->m_RemainTime);

	//lua_pushnumber(L, g_pGuildWarScore->m_BeginTime);
	//lua_pushnumber(L, g_pGuildWarScore->m_EndTime);
	return 5;
}
//----------------------------------------------------------------------------
int LuaFunc_GetGuildHouseGuildName(lua_State* L)
{   
	GuildHouseWarInfoStruct* GuildHouseWarInfoB = g_pGuildHousesFrame->GetGuildHouseWarInfoStruct( g_pGuildWarScore->m_guildIDB );
	GuildHouseWarInfoStruct* GuildHouseWarInfoR = g_pGuildHousesFrame->GetGuildHouseWarInfoStruct( g_pGuildWarScore->m_guildIDR );

	if( GuildHouseWarInfoB )
	{
		lua_pushstring( L, GuildHouseWarInfoB->GuildName );
	}
	else
	{
		lua_pushstring( L, "" );
	}

	if( GuildHouseWarInfoR )
	{
		lua_pushstring( L, GuildHouseWarInfoR->GuildName );
	}
	else
	{
		lua_pushstring( L, "" );
	}
	return 2;
}
//----------------------------------------------------------------------------
int LuaFunc_GetGuildHouseGuildTime(lua_State* L)
{   
	//int	iTime = g_pGuildWarScore->m_EndTime - g_pGameMain->GetGameTime();
	//g_pGuildWarScore->m_BeginTime;
//	iTime = iTime - g_pGameMain->GetGameTime();
	if( g_pGuildWarScore->m_RemainTime >= 0){
		lua_pushnumber( L, g_pGuildWarScore->m_RemainTime );	  
		return 1;
	}
	return 0;
}							