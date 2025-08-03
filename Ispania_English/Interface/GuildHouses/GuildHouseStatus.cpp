#include "../mainproc/GameMain.h"
#include "GuildHouseStatus.h"
#include "GuildHousesFrame.h"
#include "../../netwaker_member/Net_BG_GuildWar/NetCli_BG_GuildWarChild.h"
#include <algorithm>

typedef bool (*GuildHouseScoreCompareFunc)( const SGuildHouseWarPlayerInfo&, const SGuildHouseWarPlayerInfo& );

enum
{
	GHS_COMPARE_NAME_LESS,
	GHS_COMPARE_NAME_GREATER,
	GHS_COMPARE_KILL_LESS,
	GHS_COMPARE_KILL_GREATER,
	GHS_COMPARE_DAMAGE_LESS,
	GHS_COMPARE_DAMAGE_GREATER,
	GHS_COMPARE_DEFEND_LESS,
	GHS_COMPARE_DEFEND_GREATER,
	GHS_COMPARE_HEAL_LESS,
	GHS_COMPARE_HEAL_GREATER,
	GHS_COMPARE_MERIT_LESS,
	GHS_COMPARE_MERIT_GREATER,
	GHS_COMPARE_ENERGY_LESS,
	GHS_COMPARE_ENERGY_GREATER,
	GHS_COMPARE_OCCUPY_LESS,
	GHS_COMPARE_OCCUPY_GREATER,
	GHS_COMPARE_SCORE_LESS,
	GHS_COMPARE_SCORE_GREATER,
	GHS_COMPARE_TYPE_LESS,
	GHS_COMPARE_MAX,
};

enum
{
	GHS_TAB_MY_TEAM = 1,
	GHS_TAB_ENEMY_TEAM,
	GHS_TAB_MIX,
};

CGuildHouseWarStatus*	g_pGuildHouseWarStatus = NULL;

int LuaFunc_GuildHouseWarStatusOnShow(lua_State* L);
int LuaFunc_GuildHouseWarStatusOnHide(lua_State* L);
int LuaFunc_GuildHouseWarStatusTabChange(lua_State* L);
int LuaFunc_GuildHouseWarStatusSortChange(lua_State* L);
int LuaFunc_GetGuildHouseWarTeamPlayerNum(lua_State* L);
int LuaFunc_GetGuildHouseWarPlayerScoreInfoNum(lua_State* L);
int LuaFunc_GetGuildHouseWarPlayerScoreInfo(lua_State* L);
int LuaFunc_GetGuildHouseWarWinnerTeam(lua_State* L);
int LuaFunc_OpenGuildHouseWarPlayerScoreFrame(lua_State* L);
int LuaFunc_GetGuildHouseWarMyTeamColor(lua_State* L);
int LuaFunc_GuildHouseWarTest(lua_State* L);
int LuaFunc_GetPlayerWarScore(lua_State* L);

bool GHS_Campare_Name_Less(const SGuildHouseWarPlayerInfo & s1, const SGuildHouseWarPlayerInfo & s2);
bool GHS_Campare_Name_Greater(const SGuildHouseWarPlayerInfo & s1, const SGuildHouseWarPlayerInfo & s2);
bool GHS_Campare_KillVal_Less(const SGuildHouseWarPlayerInfo & s1, const SGuildHouseWarPlayerInfo & s2);
bool GHS_Campare_KillVal_Greater(const SGuildHouseWarPlayerInfo & s1, const SGuildHouseWarPlayerInfo & s2);
bool GHS_Campare_DamageVal_Less(const SGuildHouseWarPlayerInfo & s1, const SGuildHouseWarPlayerInfo & s2);
bool GHS_Campare_DamageVal_Greater(const SGuildHouseWarPlayerInfo & s1, const SGuildHouseWarPlayerInfo & s2);
bool GHS_Campare_DefendVal_Less(const SGuildHouseWarPlayerInfo & s1, const SGuildHouseWarPlayerInfo & s2);
bool GHS_Campare_DefendVal_Greater(const SGuildHouseWarPlayerInfo & s1, const SGuildHouseWarPlayerInfo & s2);
bool GHS_Campare_HealVal_Less(const SGuildHouseWarPlayerInfo & s1, const SGuildHouseWarPlayerInfo & s2);
bool GHS_Campare_HealVal_Greater(const SGuildHouseWarPlayerInfo & s1, const SGuildHouseWarPlayerInfo & s2);
bool GHS_Campare_MeritVal_Less(const SGuildHouseWarPlayerInfo & s1, const SGuildHouseWarPlayerInfo & s2);
bool GHS_Campare_MeritVal_Greater(const SGuildHouseWarPlayerInfo & s1, const SGuildHouseWarPlayerInfo & s2);
bool GHS_Campare_EnergyVal_Less(const SGuildHouseWarPlayerInfo & s1, const SGuildHouseWarPlayerInfo & s2);
bool GHS_Campare_EnergyVal_Greater(const SGuildHouseWarPlayerInfo & s1, const SGuildHouseWarPlayerInfo & s2);
bool GHS_Campare_OccupyVal_Less(const SGuildHouseWarPlayerInfo & s1, const SGuildHouseWarPlayerInfo & s2);
bool GHS_Campare_OccupyVal_Greater(const SGuildHouseWarPlayerInfo & s1, const SGuildHouseWarPlayerInfo & s2);
bool GHS_Campare_ScoreVal_Less(const SGuildHouseWarPlayerInfo & s1, const SGuildHouseWarPlayerInfo & s2);
bool GHS_Campare_ScoreVal_Greater(const SGuildHouseWarPlayerInfo & s1, const SGuildHouseWarPlayerInfo & s2);
bool GHS_Campare_Type_Less(const SGuildHouseWarPlayerInfo & s1, const SGuildHouseWarPlayerInfo & s2);

GuildHouseScoreCompareFunc g_GHSCompareFuncs[GHS_COMPARE_MAX] = {
	GHS_Campare_Name_Less,
	GHS_Campare_Name_Greater,
	GHS_Campare_KillVal_Less,
	GHS_Campare_KillVal_Greater,
	GHS_Campare_DamageVal_Less,
	GHS_Campare_DamageVal_Greater,
	GHS_Campare_DefendVal_Less,
	GHS_Campare_DefendVal_Greater,
	GHS_Campare_HealVal_Less,
	GHS_Campare_HealVal_Greater,
	GHS_Campare_MeritVal_Less,
	GHS_Campare_MeritVal_Greater,
	GHS_Campare_EnergyVal_Less,
	GHS_Campare_EnergyVal_Greater,
	GHS_Campare_OccupyVal_Less,
	GHS_Campare_OccupyVal_Greater,
	GHS_Campare_ScoreVal_Less,
	GHS_Campare_ScoreVal_Greater,
	GHS_Campare_Type_Less,
};

CGuildHouseWarStatus::CGuildHouseWarStatus(CInterface* object)
: CInterfaceBase(object)
{
	g_pGuildHouseWarStatus = this;
	m_playerScores.clear();
	m_scoreClassify.clear();
	m_mapRewardInfo.clear();
	m_Visible = false;
	m_updateTimer = 0;
	m_curSortFunc = 0;
	m_curClassify = GHS_TAB_MY_TEAM;
	SecureZeroMemory(m_teamPlayerNum, sizeof(m_teamPlayerNum));
	m_WinTeam = 0;
	m_iBlueTeamGuildID = 0;
	m_iMyTeamColor = 0;
}

CGuildHouseWarStatus::~CGuildHouseWarStatus()
{
	g_pGuildHouseWarStatus = NULL;
}

void CGuildHouseWarStatus::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{
		lua_register(L, "GuildHouseWarStatusFrameOnShow"		,	LuaFunc_GuildHouseWarStatusOnShow);
		lua_register(L, "GuildHouseWarStatusFrameOnHide"		,	LuaFunc_GuildHouseWarStatusOnHide);
		lua_register(L, "GuildHouseWarStatusFrameTabChange"		,	LuaFunc_GuildHouseWarStatusTabChange);
		lua_register(L, "GuildHouseWarStatusFrameSortChange"	,	LuaFunc_GuildHouseWarStatusSortChange);
		lua_register(L, "GetGuildHouseWarTeamPlayerNum"			,	LuaFunc_GetGuildHouseWarTeamPlayerNum);
		lua_register(L, "GetGuildHouseWarPlayerScoreInfoNum"	,	LuaFunc_GetGuildHouseWarPlayerScoreInfoNum);
		lua_register(L, "GetGuildHouseWarPlayerScoreInfo"		,	LuaFunc_GetGuildHouseWarPlayerScoreInfo);
		lua_register(L, "GetGuildHouseWarWinnerTeam"			,	LuaFunc_GetGuildHouseWarWinnerTeam);
		lua_register(L, "OpenGuildHouseWarPlayerScoreFrame"		,	LuaFunc_OpenGuildHouseWarPlayerScoreFrame);
		lua_register(L, "GetGuildHouseWarMyTeamColor"			,	LuaFunc_GetGuildHouseWarMyTeamColor);
		lua_register(L, "GuildHouseWarTest"						,	LuaFunc_GuildHouseWarTest);
		lua_register(L, "GetPlayerWarScore"						,	LuaFunc_GetPlayerWarScore);
	}
}

void CGuildHouseWarStatus::FrameOnShow()
{
	m_Visible = true;
	//NetCli_BG_GuildWar::SBL_PlayerInfoRequest();
}

void CGuildHouseWarStatus::FrameOnHide()
{
	m_Visible = false;
	m_updateTimer = 0.0f;
}

void CGuildHouseWarStatus::Update(float elapsedTime)
{
	//if( !m_Visible )
	//	return;

	//int ZoneID = g_pGameMain->GetZoneID() % _DEF_ZONE_BASE_COUNT_;

	//if( ZoneID != 402 )
	//	return;

	////update score info every 5 sec
	//m_updateTimer += elapsedTime;
	//if ( m_updateTimer < 5.0f )
	//	return;

	//m_updateTimer = 0.0f;

	//NetCli_BG_GuildWar::SBL_PlayerInfoRequest();
}

void CGuildHouseWarStatus::ClassifyScore(int iClassify)
{
	m_curClassify = iClassify;

	m_scoreClassify.clear();

	if (m_curClassify == GHS_TAB_MIX)
	{
		m_scoreClassify = m_playerScores;
	}
	else
	{
		int iSize = (int)m_playerScores.size();

		for (int i = 0 ; i < iSize; i++)
		{
			if( (m_curClassify == GHS_TAB_MY_TEAM && m_playerScores[i].iTeamColor == m_iMyTeamColor) ||
				(m_curClassify == GHS_TAB_ENEMY_TEAM && m_playerScores[i].iTeamColor != m_iMyTeamColor) )
					m_scoreClassify.push_back(m_playerScores[i]);
		}
	}

	sort(m_scoreClassify.begin(), m_scoreClassify.end(), g_GHSCompareFuncs[m_curSortFunc]);

	SendWorldEvent(UiEvent_UpdateGuildHousePlayerScore);
}

void CGuildHouseWarStatus::ChangeSortFunc(int iSort)
{
	if (iSort < GHS_COMPARE_NAME_LESS || iSort >= GHS_COMPARE_MAX)
		return;

	if (m_curSortFunc == iSort)
		return;

	m_curSortFunc = iSort;

	sort(m_scoreClassify.begin(), m_scoreClassify.end(), g_GHSCompareFuncs[m_curSortFunc]);

	SendWorldEvent(UiEvent_UpdateGuildHousePlayerScore);
}

void CGuildHouseWarStatus::UpdateScore(int iTotalRole, int iIndex, GuildWarPlayerInfoStruct* pScoreArray)
{
	if( iIndex == 0 )
	{
		m_playerScores.clear();
		SecureZeroMemory(m_teamPlayerNum, sizeof(m_teamPlayerNum));
	}

	SGuildHouseWarPlayerInfo info;
	info.Status = *pScoreArray;

	if( info.Status.GuildID == m_iBlueTeamGuildID )
		info.iTeamColor = GHS_COLOR_TEAM_BLUE;
	else
		info.iTeamColor = GHS_COLOR_TEAM_RED;

	if( info.Status.PlayerDBID == g_pGameMain->GetZoneDBID() )
		m_iMyTeamColor = info.iTeamColor;

	m_playerScores.push_back(info);

	if( info.iTeamColor > GHS_COLOR_ALL && info.iTeamColor < GHS_COLOR_MAX )
	{
		m_teamPlayerNum[info.iTeamColor]++;
		m_teamPlayerNum[GHS_COLOR_ALL]++;
	}

	if( iTotalRole == iIndex + 1 )
		ClassifyScore(m_curClassify);
}

void CGuildHouseWarStatus::UpdateFinalInfo(int PlayerDBID, int PrizeType, int Score, int OrderID, Voc_ENUM Voc, Voc_ENUM Voc_Sub, int Level, int Level_Sub)
{
	SGuildHouseWarRewardInfo info;

	info.Voc = Voc;
	info.Voc_Sub = Voc_Sub;
	info.iLevel = Level;
	info.iLevel_Sub = Level_Sub;
	info.iPrizeType = PrizeType;
	info.iScore = Score;
	info.iOrderID = OrderID;

	m_mapRewardInfo[PlayerDBID] = info;

	ClassifyScore(m_curClassify);
}

void CGuildHouseWarStatus::GuildHouseWarClose(int iWinTeam)
{
	m_WinTeam = iWinTeam;
	SendWorldEvent(UiEvent_GuildHouseClose);
}

void CGuildHouseWarStatus::EndWarCalc()
{
	if( m_Info.Base[0].Score > m_Info.Base[1].Score )
		GuildHouseWarClose( GHS_COLOR_TEAM_BLUE );
	else if( m_Info.Base[0].Score < m_Info.Base[1].Score )
		GuildHouseWarClose( GHS_COLOR_TEAM_RED );
	else
		GuildHouseWarClose( GHS_COLOR_MAX );
}

int CGuildHouseWarStatus::GetPlayerScoreInfoNum()
{
	return (int)m_scoreClassify.size();
}

SGuildHouseWarPlayerInfo* CGuildHouseWarStatus::GetPlayerScore(int iIndex)
{
	SGuildHouseWarPlayerInfo* Result = NULL;

	int iSize = (int)m_scoreClassify.size();

	if (iIndex >= 0 && iIndex < iSize)
	{
		Result = &m_scoreClassify[iIndex];
	}

	return Result;
}

SGuildHouseWarRewardInfo* CGuildHouseWarStatus::GetRewardInfo(int iPlayerDBID)
{
	return &m_mapRewardInfo[iPlayerDBID];
}

void CGuildHouseWarStatus::GetGuildScore( GuildHouseWarFightStruct& Info )
{
	// 第一隊是藍的
	m_iBlueTeamGuildID = Info.Base[0].GuildID;
	m_Info = Info;
}

int LuaFunc_GuildHouseWarStatusOnShow(lua_State* L)
{
	g_pGuildHouseWarStatus->FrameOnShow();
	return 0;
}

int LuaFunc_GuildHouseWarStatusOnHide(lua_State* L)
{
	g_pGuildHouseWarStatus->FrameOnHide();
	return 0;
}

int LuaFunc_GuildHouseWarStatusTabChange(lua_State* L)
{
	int iTabIndex = luaL_checkint( L, 1 );
	g_pGuildHouseWarStatus->ClassifyScore(iTabIndex);
	return 0;
}

int LuaFunc_GuildHouseWarStatusSortChange(lua_State* L)
{
	int iSortIndex = luaL_checkint( L, 1 );
	g_pGuildHouseWarStatus->ChangeSortFunc(iSortIndex);
	return 0;
}

int LuaFunc_GetGuildHouseWarTeamPlayerNum(lua_State* L)
{
	lua_pushnumber(L, g_pGuildHouseWarStatus->GetTeamPlayerNum(GHS_COLOR_TEAM_BLUE));
	lua_pushnumber(L, g_pGuildHouseWarStatus->GetTeamPlayerNum(GHS_COLOR_TEAM_RED));
	return 2;
}

int LuaFunc_GetGuildHouseWarPlayerScoreInfoNum(lua_State* L)
{
	int iCount = g_pGuildHouseWarStatus->GetPlayerScoreInfoNum();
	lua_pushnumber(L, iCount);
	return 1;
}

int LuaFunc_GetGuildHouseWarPlayerScoreInfo(lua_State* L)
{
	int iIndex = luaL_checkint( L, 1 );
	int iBoardType = luaL_checkint( L, 2 );

	SGuildHouseWarPlayerInfo* pScore = g_pGuildHouseWarStatus->GetPlayerScore(iIndex);

	if (pScore)
	{
		int isPlayer = 0;

		if (pScore->Status.PlayerDBID == g_pGameMain->GetZoneDBID())
		{
			isPlayer = 1;
		}

		lua_pushnumber(L, isPlayer);
		lua_pushnumber(L, pScore->iTeamColor);
		lua_pushstring(L, pScore->Status.RoleName);

		if( iBoardType == 1 )
		{			
			lua_pushnumber(L, pScore->Status.KillPoint);
			lua_pushnumber(L, pScore->Status.HitPoint);
			lua_pushnumber(L, pScore->Status.DefPoint);
			lua_pushnumber(L, pScore->Status.HealPoint);
			lua_pushnumber(L, pScore->Status.WarScore);
			lua_pushnumber(L, pScore->Status.EnergyPoint);
			lua_pushnumber(L, pScore->Status.OccupyPoint);
			lua_pushnumber(L, pScore->Status.TotalScore);

			return 11;
		}
		else
		{
			SGuildHouseWarRewardInfo* pInfo = g_pGuildHouseWarStatus->GetRewardInfo( pScore->Status.PlayerDBID );
			
			lua_pushnumber(L, pInfo->Voc);
			lua_pushnumber(L, pInfo->iLevel);
			lua_pushnumber(L, pInfo->Voc_Sub);
			lua_pushnumber(L, pInfo->iLevel_Sub);
			lua_pushnumber(L, pInfo->iPrizeType);
			lua_pushnumber(L, pInfo->iScore);
			lua_pushnumber(L, pInfo->iOrderID);

			return 10;
		}		
	}

	return 0;
}

int LuaFunc_GetGuildHouseWarWinnerTeam(lua_State* L)
{
	int iWinTeam = g_pGuildHouseWarStatus->GetWinTeam();
	lua_pushnumber( L, iWinTeam );
	return 1;
}

int LuaFunc_OpenGuildHouseWarPlayerScoreFrame(lua_State* L)
{
	g_pGuildHouseWarStatus->SendWorldEvent(UiEvent_OpenGuildHousePlayerScoreFrame);
	return 0;
}

int LuaFunc_GetGuildHouseWarMyTeamColor(lua_State* L)
{
	int iMyTeamColor = g_pGuildHouseWarStatus->GetMyTeamColor();
	lua_pushnumber( L, iMyTeamColor );
	return 1;
}

int LuaFunc_GuildHouseWarTest(lua_State* L)
{
	GuildWarPlayerInfoStruct *pInfo = new GuildWarPlayerInfoStruct[2];
	pInfo[0].GuildID = 1;
	pInfo[0].PlayerDBID = g_pGameMain->GetZoneDBID();
	pInfo[0].RoleName = "abc";
	pInfo[0].KillPoint = 100;	
	pInfo[0].HitPoint = 200;	
	pInfo[0].DefPoint = 300;	
	pInfo[0].HealPoint = 400;	
	pInfo[0].WarScore = 500;	
	pInfo[0].EnergyPoint = 600;
	pInfo[0].OccupyPoint = 700;
	pInfo[0].TotalScore = 800;
	pInfo[1].GuildID = 2;
	pInfo[1].PlayerDBID = 54321;
	pInfo[1].RoleName = "def";
	pInfo[1].KillPoint = 800;	
	pInfo[1].HitPoint = 700;	
	pInfo[1].DefPoint = 600;	
	pInfo[1].HealPoint = 500;	
	pInfo[1].WarScore = 400;	
	pInfo[1].EnergyPoint = 300;
	pInfo[1].OccupyPoint = 200;
	pInfo[1].TotalScore = 100;

	g_pGuildHouseWarStatus->SetBlueTeamGuildID( 1 );
	g_pGuildHouseWarStatus->UpdateScore( 2, 0, &pInfo[0] );
	g_pGuildHouseWarStatus->UpdateScore( 2, 1, &pInfo[1] );
	g_pGuildHouseWarStatus->UpdateFinalInfo( pInfo[1].PlayerDBID, 1, 100, 2, (Voc_ENUM)1, (Voc_ENUM)2, 16, 13 );
	g_pGuildHouseWarStatus->UpdateFinalInfo( pInfo[0].PlayerDBID, 2, 800, 1, (Voc_ENUM)3, (Voc_ENUM)4, 45, 26 );

	return 0;
}

bool GHS_Campare_Name_Less(const SGuildHouseWarPlayerInfo & s1, const SGuildHouseWarPlayerInfo & s2) {
	string name1 = s1.Status.RoleName;
	string name2 = s2.Status.RoleName;
	return name1 < name2;
}

bool GHS_Campare_Name_Greater(const SGuildHouseWarPlayerInfo & s1, const SGuildHouseWarPlayerInfo & s2) {
	return GHS_Campare_Name_Less(s2, s1);
}

bool GHS_Campare_KillVal_Less(const SGuildHouseWarPlayerInfo & s1, const SGuildHouseWarPlayerInfo & s2) {
	return s1.Status.KillPoint < s2.Status.KillPoint;
}

bool GHS_Campare_KillVal_Greater(const SGuildHouseWarPlayerInfo & s1, const SGuildHouseWarPlayerInfo & s2) {
	return s1.Status.KillPoint > s2.Status.KillPoint;
}

bool GHS_Campare_DamageVal_Less(const SGuildHouseWarPlayerInfo & s1, const SGuildHouseWarPlayerInfo & s2) {
	return s1.Status.HitPoint < s2.Status.HitPoint;
}

bool GHS_Campare_DamageVal_Greater(const SGuildHouseWarPlayerInfo & s1, const SGuildHouseWarPlayerInfo & s2) {
	return s1.Status.HitPoint > s2.Status.HitPoint;
}

bool GHS_Campare_DefendVal_Less(const SGuildHouseWarPlayerInfo & s1, const SGuildHouseWarPlayerInfo & s2)
{
	return s1.Status.DefPoint < s2.Status.DefPoint;
}

bool GHS_Campare_DefendVal_Greater(const SGuildHouseWarPlayerInfo & s1, const SGuildHouseWarPlayerInfo & s2)
{
	return s1.Status.DefPoint > s2.Status.DefPoint;
}

bool GHS_Campare_HealVal_Less(const SGuildHouseWarPlayerInfo & s1, const SGuildHouseWarPlayerInfo & s2) {
	return s1.Status.HealPoint < s2.Status.HealPoint;
}

bool GHS_Campare_HealVal_Greater(const SGuildHouseWarPlayerInfo & s1, const SGuildHouseWarPlayerInfo & s2) {
	return s1.Status.HealPoint > s2.Status.HealPoint;
}

bool GHS_Campare_MeritVal_Less(const SGuildHouseWarPlayerInfo & s1, const SGuildHouseWarPlayerInfo & s2)
{
	return s1.Status.WarScore < s2.Status.WarScore;
}

bool GHS_Campare_MeritVal_Greater(const SGuildHouseWarPlayerInfo & s1, const SGuildHouseWarPlayerInfo & s2)
{
	return s1.Status.WarScore > s2.Status.WarScore;
}

bool GHS_Campare_EnergyVal_Less(const SGuildHouseWarPlayerInfo & s1, const SGuildHouseWarPlayerInfo & s2)
{
	return s1.Status.EnergyPoint < s2.Status.EnergyPoint;
}

bool GHS_Campare_EnergyVal_Greater(const SGuildHouseWarPlayerInfo & s1, const SGuildHouseWarPlayerInfo & s2)
{
	return s1.Status.EnergyPoint > s2.Status.EnergyPoint;
}

bool GHS_Campare_OccupyVal_Less(const SGuildHouseWarPlayerInfo & s1, const SGuildHouseWarPlayerInfo & s2)
{
	return s1.Status.OccupyPoint < s2.Status.OccupyPoint;
}

bool GHS_Campare_OccupyVal_Greater(const SGuildHouseWarPlayerInfo & s1, const SGuildHouseWarPlayerInfo & s2)
{
	return s1.Status.OccupyPoint > s2.Status.OccupyPoint;
}

bool GHS_Campare_ScoreVal_Less(const SGuildHouseWarPlayerInfo & s1, const SGuildHouseWarPlayerInfo & s2)
{
	return s1.Status.TotalScore < s2.Status.TotalScore;
}

bool GHS_Campare_ScoreVal_Greater(const SGuildHouseWarPlayerInfo & s1, const SGuildHouseWarPlayerInfo & s2)
{
	return s1.Status.TotalScore > s2.Status.TotalScore;
}

bool GHS_Campare_Type_Less(const SGuildHouseWarPlayerInfo & s1, const SGuildHouseWarPlayerInfo & s2)
{
	SGuildHouseWarRewardInfo* pInfo1 = g_pGuildHouseWarStatus->GetRewardInfo( s1.Status.PlayerDBID );
	SGuildHouseWarRewardInfo* pInfo2 = g_pGuildHouseWarStatus->GetRewardInfo( s2.Status.PlayerDBID );

	if( pInfo1->iPrizeType == -1 )
		return false;
	else if( pInfo2->iPrizeType == -1 )
		return true;

	if( pInfo1->iPrizeType < pInfo2->iPrizeType || ( pInfo1->iPrizeType == pInfo2->iPrizeType && pInfo1->iOrderID < pInfo2->iOrderID ) )
		return true;
	else
		return false;
}
int LuaFunc_GetPlayerWarScore(lua_State* L)
{
	RoleDataEx*		pRole= CNetGlobal::RoleData();
	int iCountOnBody = pRole->CalBodyItemCount( 206685 , true );		
	if(iCountOnBody)
	{
		lua_pushnumber( L, iCountOnBody );		
		return 1;
	}
	return 0;
}