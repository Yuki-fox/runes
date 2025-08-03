#include "../mainproc/GameMain.h"
#include "BattleGroundStatusFrame.h"
#include "../../netwaker_member/Net_BattleGround/NetCli_BattleGround_Child.h"
#include <algorithm>

typedef bool (*BattleGroundScoreCompareFunc)(const StructBattleGroundScore &, const StructBattleGroundScore& );

enum
{
	BG_COMPARE_NAME_LESS,
	BG_COMPARE_NAME_GREATER,
	BG_COMPARE_KILL_LESS,
	BG_COMPARE_KILL_GREATER,
	BG_COMPARE_DEAD_LESS,
	BG_COMPARE_DEAD_GREATER,
	BG_COMPARE_DAMAGE_LESS,
	BG_COMPARE_DAMAGE_GREATER,
	BG_COMPARE_HEAL_LESS,
	BG_COMPARE_HEAL_GREATER,
	BG_COMPARE_ROLE_VAL1_LESS,
	BG_COMPARE_ROLE_VAL1_GREATER,
	BG_COMPARE_ROLE_VAL2_LESS,
	BG_COMPARE_ROLE_VAL2_GREATER,
	BG_COMPARE_ROLE_VAL3_LESS,
	BG_COMPARE_ROLE_VAL3_GREATER,
	BG_COMPARE_MAX
};

enum
{
	BG_CLASSIFY_ALL,
	BG_CLASSIFY_TEAM1,
	BG_CLASSIFY_TEAM2,
	BG_CLASSIFY_TEAM3,
	BG_CLASSIFY_TEAM4,
	BG_CLASSIFY_TEAM5,
	BG_CLASSIFY_TEAM6,
	BG_CLASSIFY_TEAM7,
	BG_CLASSIFY_TEAM8,
	BG_CLASSIFY_MAX
};

CBattleGroundStatusFrame*	g_pBattleGroundStatusFrame = NULL;

int LuaFunc_BattleGroundStatusFrameOnShow(lua_State* L);
int LuaFunc_BattleGroundStatusFrameOnHide(lua_State* L);
int LuaFunc_BattleGroundStatusFrameTabChage(lua_State* L);
int LuaFunc_BattleGroundStatusFrameSortChage(lua_State* L);
int LuaFunc_GetBattleGroundTeamPlayerNum(lua_State* L);
int LuaFunc_GetBattleGroundPlayerClassName(lua_State* L);
int LuaFunc_GetBattleGroundPlayerScoreInfoNum(lua_State* L);
int LuaFunc_GetBattleGroundPlayerScoreInfo(lua_State* L);
int LuaFunc_GetWinnerTeam(lua_State* L);
int LuaFunc_OpenBattleGroundPlayerScoreFrame(lua_State* L);

bool BG_Campare_Name_Less(const StructBattleGroundScore & s1, const StructBattleGroundScore & s2);
bool BG_Campare_Name_Greater(const StructBattleGroundScore & s1, const StructBattleGroundScore & s2);
bool BG_Campare_KillVal_Less(const StructBattleGroundScore & s1, const StructBattleGroundScore & s2);
bool BG_Campare_KillVal_Greater(const StructBattleGroundScore & s1, const StructBattleGroundScore & s2);
bool BG_Campare_DeadVal_Less(const StructBattleGroundScore & s1, const StructBattleGroundScore & s2);
bool BG_Campare_DeadVal_Greater(const StructBattleGroundScore & s1, const StructBattleGroundScore & s2);
bool BG_Campare_DamageVal_Less(const StructBattleGroundScore & s1, const StructBattleGroundScore & s2);
bool BG_Campare_DamageVal_Greater(const StructBattleGroundScore & s1, const StructBattleGroundScore & s2);
bool BG_Campare_HealVal_Less(const StructBattleGroundScore & s1, const StructBattleGroundScore & s2);
bool BG_Campare_HealVal_Greater(const StructBattleGroundScore & s1, const StructBattleGroundScore & s2);
bool BG_Campare_RoleVal1_Less(const StructBattleGroundScore & s1, const StructBattleGroundScore & s2);
bool BG_Campare_RoleVal1_Greater(const StructBattleGroundScore & s1, const StructBattleGroundScore & s2);
bool BG_Campare_RoleVal2_Less(const StructBattleGroundScore & s1, const StructBattleGroundScore & s2);
bool BG_Campare_RoleVal2_Greater(const StructBattleGroundScore & s1, const StructBattleGroundScore & s2);
bool BG_Campare_RoleVal3_Less(const StructBattleGroundScore & s1, const StructBattleGroundScore & s2);
bool BG_Campare_RoleVal3_Greater(const StructBattleGroundScore & s1, const StructBattleGroundScore & s2);

BattleGroundScoreCompareFunc g_BGCompareFuncs[BG_COMPARE_MAX] = {
	BG_Campare_Name_Less,
	BG_Campare_Name_Greater,
	BG_Campare_KillVal_Less,
	BG_Campare_KillVal_Greater,
	BG_Campare_DeadVal_Less,
	BG_Campare_DeadVal_Greater,
	BG_Campare_DamageVal_Less,
	BG_Campare_DamageVal_Greater,
	BG_Campare_HealVal_Less,
	BG_Campare_HealVal_Greater,
	BG_Campare_RoleVal1_Less,
	BG_Campare_RoleVal1_Greater,
	BG_Campare_RoleVal2_Less,
	BG_Campare_RoleVal2_Greater,
	BG_Campare_RoleVal3_Less,
	BG_Campare_RoleVal3_Greater
};


CBattleGroundStatusFrame::CBattleGroundStatusFrame(CInterface* object)
: CInterfaceBase(object)
{
	g_pBattleGroundStatusFrame = this;
	m_playerScores.clear();
	m_scoreClassify.clear();
	m_Visible = false;
	m_updateTimer = 0;
	m_curSortFunc = 0;
	m_curClassify = 0;
	SecureZeroMemory(m_teamPlayerNum, sizeof(m_teamPlayerNum));
}

CBattleGroundStatusFrame::~CBattleGroundStatusFrame()
{
	g_pBattleGroundStatusFrame = NULL;
}

void CBattleGroundStatusFrame::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{
		lua_register(L, "BattleGroundStatusFrameOnShow"		,	LuaFunc_BattleGroundStatusFrameOnShow);
		lua_register(L, "BattleGroundStatusFrameOnHide"		,	LuaFunc_BattleGroundStatusFrameOnHide);
		lua_register(L, "BattleGroundStatusFrameTabChange"	,	LuaFunc_BattleGroundStatusFrameTabChage);
		lua_register(L, "BattleGroundStatusFrameSortChange"	,	LuaFunc_BattleGroundStatusFrameSortChage);
		lua_register(L, "GetBattleGroundTeamPlayerNum"		,	LuaFunc_GetBattleGroundTeamPlayerNum);
		lua_register(L, "GetBattleGroundPlayerClassName"	,	LuaFunc_GetBattleGroundPlayerClassName);
		lua_register(L, "GetBattleGroundPlayerScoreInfoNum"	,	LuaFunc_GetBattleGroundPlayerScoreInfoNum);
		lua_register(L, "GetBattleGroundPlayerScoreInfo"	,	LuaFunc_GetBattleGroundPlayerScoreInfo);
		lua_register(L, "GetBattleGroundWinnerTeam"			,	LuaFunc_GetWinnerTeam);
		lua_register(L, "OpenBattleGroundPlayerScoreFrame"	,	LuaFunc_OpenBattleGroundPlayerScoreFrame);
	}
}

void CBattleGroundStatusFrame::FrameOnShow()
{
	m_Visible = true;
	CNetCli_BattleGround::GetBattleGroundScoreInfo();
}

void CBattleGroundStatusFrame::FrameOnHide()
{
	m_Visible = false;
	m_updateTimer = 0.0f;
}

void CBattleGroundStatusFrame::Update(float elapsedTime)
{
	int iType = g_pGameMain->GetBattleGroundType();

	if (iType == 0 )
		return;

	//update score info every 5 sec
	m_updateTimer += elapsedTime;
	if ( m_updateTimer < 5.0f )
		return;

	m_updateTimer = 0.0f;

	CNetCli_BattleGround::GetBattleGroundScoreInfo();
}

void CBattleGroundStatusFrame::ClassifyScore(int iClassify)
{
	m_curClassify = iClassify;

	m_scoreClassify.clear();

	if (m_curClassify == BG_CLASSIFY_ALL)
	{
		m_scoreClassify = m_playerScores;
	}
	else
	{
		int iSize = (int)m_playerScores.size();


		for (int i = 0 ; i < iSize; i++)
		{
			if (m_playerScores[i].iTeamID == m_curClassify)
				m_scoreClassify.push_back(m_playerScores[i]);
		}
	}

	sort(m_scoreClassify.begin(), m_scoreClassify.end(), g_BGCompareFuncs[m_curSortFunc]);

	SendWorldEvent(UiEvent_UpdateBattleGroundPlayerScore);
}

void CBattleGroundStatusFrame::ChangeSortFunc(int iSort)
{
	if (iSort < BG_COMPARE_NAME_LESS || iSort > BG_COMPARE_HEAL_GREATER)
		return;

	if (m_curSortFunc == iSort)
		return;

	m_curSortFunc = iSort;

	sort(m_scoreClassify.begin(), m_scoreClassify.end(), g_BGCompareFuncs[m_curSortFunc]);

	SendWorldEvent(UiEvent_UpdateBattleGroundPlayerScore);
}

void CBattleGroundStatusFrame::UpdateScore(int iSrvID, int iTotalRole, StructBattleGroundScore* pScoreArray)
{
	m_playerScores.clear();

	StructBattleGroundScore* pTmp = pScoreArray;

	SecureZeroMemory(m_teamPlayerNum, sizeof(m_teamPlayerNum));

	for (int i = 0 ; i < iTotalRole; i++)
	{
		m_playerScores.push_back(*pTmp);


		int tmpTeamID = 0;

		try
		{
			tmpTeamID = pTmp->iTeamID;
		}
		catch (...)
		{
			tmpTeamID = -1;
		}

		if (tmpTeamID >= 0 && tmpTeamID < MAX_BG_TEAM_CLASSIFY_NUM)
		{
			m_teamPlayerNum[tmpTeamID]++;
			m_teamPlayerNum[0]++;
		}

		pTmp++;
	}

	ClassifyScore(m_curClassify);
}

void CBattleGroundStatusFrame::BattleGroundClose(int iWinTeam)
{
	m_WinTeam = iWinTeam;
	SendWorldEvent(UiEvent_BattleGroundClose);
}

int CBattleGroundStatusFrame::GetPlayerScoreInfoNum()
{
	return (int)m_scoreClassify.size();
}

StructBattleGroundScore* CBattleGroundStatusFrame::GetPlayerScore(int iIndex)
{
	StructBattleGroundScore* Result = NULL;

	int iSize = (int)m_scoreClassify.size();

	if (iIndex >= 0 && iIndex < iSize)
	{
		Result = &m_scoreClassify[iIndex];
	}

	return Result;
}

int LuaFunc_BattleGroundStatusFrameOnShow(lua_State* L)
{
	g_pBattleGroundStatusFrame->FrameOnShow();
	return 0;
}

int LuaFunc_BattleGroundStatusFrameOnHide(lua_State* L)
{
	g_pBattleGroundStatusFrame->FrameOnHide();
	return 0;
}

int LuaFunc_BattleGroundStatusFrameTabChage(lua_State* L)
{
	int iTabIndex = luaL_checkint( L, 1 );
	g_pBattleGroundStatusFrame->ClassifyScore(iTabIndex);
	return 0;
}

int LuaFunc_BattleGroundStatusFrameSortChage(lua_State* L)
{
	int iSortIndex = luaL_checkint( L, 1 );
	g_pBattleGroundStatusFrame->ChangeSortFunc(iSortIndex);
	return 0;
}

int LuaFunc_GetBattleGroundTeamPlayerNum(lua_State* L)
{
	lua_pushnumber(L, g_pBattleGroundStatusFrame->GetTeamPlayerNum(BG_CLASSIFY_TEAM1));
	lua_pushnumber(L, g_pBattleGroundStatusFrame->GetTeamPlayerNum(BG_CLASSIFY_TEAM2));
	lua_pushnumber(L, g_pBattleGroundStatusFrame->GetTeamPlayerNum(BG_CLASSIFY_TEAM3));
	lua_pushnumber(L, g_pBattleGroundStatusFrame->GetTeamPlayerNum(BG_CLASSIFY_TEAM4));
	lua_pushnumber(L, g_pBattleGroundStatusFrame->GetTeamPlayerNum(BG_CLASSIFY_TEAM5));
	lua_pushnumber(L, g_pBattleGroundStatusFrame->GetTeamPlayerNum(BG_CLASSIFY_TEAM6));
	lua_pushnumber(L, g_pBattleGroundStatusFrame->GetTeamPlayerNum(BG_CLASSIFY_TEAM7));
	lua_pushnumber(L, g_pBattleGroundStatusFrame->GetTeamPlayerNum(BG_CLASSIFY_TEAM8));
	return 8;
}

int LuaFunc_GetBattleGroundPlayerClassName(lua_State* L)
{
	int vocId = luaL_checkint( L, 1 );
	lua_pushstring(L, g_ObjectData->GetClassNameByID((Voc_ENUM)vocId));		// Voc
	return 1;
}

int LuaFunc_GetBattleGroundPlayerScoreInfoNum(lua_State* L)
{
	int iCount = g_pBattleGroundStatusFrame->GetPlayerScoreInfoNum();
	lua_pushnumber(L, iCount);
	return 1;
}

int LuaFunc_GetBattleGroundPlayerScoreInfo(lua_State* L)
{
	int iIndex = luaL_checkint( L, 1 );

	StructBattleGroundScore* pScore = g_pBattleGroundStatusFrame->GetPlayerScore(iIndex);

	if (pScore)
	{
		int isPlayer = 0;

		if (pScore->iDBID == g_pGameMain->GetZoneDBID())
		{
			isPlayer = 1;
		}

		lua_pushnumber(L, isPlayer);
		lua_pushnumber(L, pScore->iTeamID);
		lua_pushnumber(L, pScore->iVocID);
		lua_pushnumber(L, pScore->iVocSubID);
		lua_pushnumber(L, pScore->iLV);
		lua_pushnumber(L, pScore->iLVSub);
		lua_pushstring(L, pScore->Name);
		lua_pushnumber(L, pScore->Score.iDamageVal);
		lua_pushnumber(L, pScore->Score.iDeadVal);
		lua_pushnumber(L, pScore->Score.iHealVal);
		lua_pushnumber(L, pScore->Score.iKillVal);
		lua_pushnumber(L, pScore->iRoleVar[0]);  //player val 0
		lua_pushnumber(L, pScore->iRoleVar[1]);  //player val 1
		lua_pushnumber(L, pScore->iRoleVar[2]);  //player val 2

		return 14;
	}

	return 0;
}

int LuaFunc_GetWinnerTeam(lua_State* L)
{
	int iWinTeam = g_pBattleGroundStatusFrame->GetWinTeam();
	lua_pushnumber( L, iWinTeam );
	return 1;
}

int LuaFunc_OpenBattleGroundPlayerScoreFrame(lua_State* L)
{
	g_pBattleGroundStatusFrame->SendWorldEvent(UiEvent_OpenBattleGroundPlayerScoreFrame);
	return 0;
}

bool BG_Campare_Name_Less(const StructBattleGroundScore & s1, const StructBattleGroundScore & s2) {
	string name1 = s1.Name;
	string name2 = s2.Name;
	return name1 < name2;
}

bool BG_Campare_Name_Greater(const StructBattleGroundScore & s1, const StructBattleGroundScore & s2) {
	return BG_Campare_Name_Less(s2, s1);
}

bool BG_Campare_KillVal_Less(const StructBattleGroundScore & s1, const StructBattleGroundScore & s2) {
	return s1.Score.iKillVal < s2.Score.iKillVal;
}

bool BG_Campare_KillVal_Greater(const StructBattleGroundScore & s1, const StructBattleGroundScore & s2) {
	return s1.Score.iKillVal > s2.Score.iKillVal;
}

bool BG_Campare_DeadVal_Less(const StructBattleGroundScore & s1, const StructBattleGroundScore & s2) {
	return s1.Score.iDeadVal < s2.Score.iDeadVal;
}

bool BG_Campare_DeadVal_Greater(const StructBattleGroundScore & s1, const StructBattleGroundScore & s2) {
	return s1.Score.iDeadVal > s2.Score.iDeadVal;
}

bool BG_Campare_DamageVal_Less(const StructBattleGroundScore & s1, const StructBattleGroundScore & s2) {
	return s1.Score.iDamageVal < s2.Score.iDamageVal;
}

bool BG_Campare_DamageVal_Greater(const StructBattleGroundScore & s1, const StructBattleGroundScore & s2) {
	return s1.Score.iDamageVal > s2.Score.iDamageVal;
}

bool BG_Campare_HealVal_Less(const StructBattleGroundScore & s1, const StructBattleGroundScore & s2) {
	return s1.Score.iHealVal < s2.Score.iHealVal;
}

bool BG_Campare_HealVal_Greater(const StructBattleGroundScore & s1, const StructBattleGroundScore & s2) {
	return s1.Score.iHealVal > s2.Score.iHealVal;
}

bool BG_Campare_RoleVal1_Less(const StructBattleGroundScore & s1, const StructBattleGroundScore & s2)
{
	return s1.iRoleVar[0] < s2.iRoleVar[0];
}

bool BG_Campare_RoleVal1_Greater(const StructBattleGroundScore & s1, const StructBattleGroundScore & s2)
{
	return s1.iRoleVar[0] > s2.iRoleVar[0];
}

bool BG_Campare_RoleVal2_Less(const StructBattleGroundScore & s1, const StructBattleGroundScore & s2)
{
	return s1.iRoleVar[1] < s2.iRoleVar[1];
}

bool BG_Campare_RoleVal2_Greater(const StructBattleGroundScore & s1, const StructBattleGroundScore & s2)
{
	return s1.iRoleVar[1] > s2.iRoleVar[1];
}

bool BG_Campare_RoleVal3_Less(const StructBattleGroundScore & s1, const StructBattleGroundScore & s2)
{
	return s1.iRoleVar[2] < s2.iRoleVar[2];
}

bool BG_Campare_RoleVal3_Greater(const StructBattleGroundScore & s1, const StructBattleGroundScore & s2)
{
	return s1.iRoleVar[2] > s2.iRoleVar[2];
}