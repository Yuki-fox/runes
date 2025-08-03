#include "../mainproc/GameMain.h"
#include "BattleGroundQueueFrame.h"
#include "../../netwaker_member/Net_BattleGround/NetCli_BattleGround_Child.h"
#include "../../netwaker_member/net_party/NetCli_PartyChild.h"
#include <algorithm>



int LuaFunc_UpdateQueueFrameInfo(lua_State* L);
int LuaFunc_JoinBattleGround(lua_State* L);
int LuaFunc_JoinBattleGroundWithTeam(lua_State* L);
int LuaFunc_ReturnBattleGroundType(lua_State* L);
int LuaFunc_GetBattleGroundResultType(lua_State* L);


CBattleGroundQueueFrame* g_pBattleGroundQueueFrame = NULL;

CBattleGroundQueueFrame::CBattleGroundQueueFrame(CInterface* object)
: CInterfaceBase(object)
{
	g_pBattleGroundQueueFrame = this;
}
CBattleGroundQueueFrame::~CBattleGroundQueueFrame()
{
	g_pBattleGroundQueueFrame = NULL;
}

void CBattleGroundQueueFrame::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{
		lua_register(L, "UpdateQueueFrameInfo" , LuaFunc_UpdateQueueFrameInfo);
		lua_register(L, "JoinBattleGround" , LuaFunc_JoinBattleGround);
		lua_register(L, "JoinBattleGroundWithTeam" , LuaFunc_JoinBattleGroundWithTeam);
		lua_register(L, "ReturnBattleGroundType" , LuaFunc_ReturnBattleGroundType);
		lua_register(L, "GetBattleGroundResultType" , LuaFunc_GetBattleGroundResultType);
	}
}

void CBattleGroundQueueFrame::Update(float elapsedTime)
{
	static float lastTime = 0.0f;
	lastTime += elapsedTime;
	if ( lastTime < 0.5f )
		return;

	lastTime = 0.0f;
	queuecount = 0;

	GetJoinBGTeamDBID();

	map< int, StructBattleGroundQueueStatus >& mapStatus = CNetCli_BattleGround_Child::GetQueseStatusMap();
	map< int, StructBattleGroundQueueStatus >::iterator it ;

	for (it= mapStatus.begin(); it != mapStatus.end(); ++it)
	{
		iType = 0;
		emQueueStatus = 0;

		queuecount+=1;
		emQueueStatus = it->second.emQueueStatus;
		dwTimeExcepted = it->second.dwTimeExcepted;
		iType = it->second.iType;
		dwTimeToLeaveQueue = it->second.dwTimeToLeaveQueue - timeGetTime();
		dwTimeToLeaveQueue = dwTimeToLeaveQueue/1000;

		//sprintf(BGN, "%s", CNetCli_BattleGround_Child::GetBattleGroundName(it->first));
		//if(iType != 350)
			SendWorldEvent(UiEvent_UpdateQueueFrameInfo);
	}	
	if( queuecount == 0 )
	{
		SendWorldEvent(UiEvent_ClearAllQueueStatus);
	}
	queuecount=0;	
}

int LuaFunc_UpdateQueueFrameInfo(lua_State* L)
{
	//lua_pushstring(L,g_pBattleGroundQueueFrame->BGN);
	//lua_pushinteger(L,g_pBattleGroundQueueFrame->queuecount);
	lua_pushinteger(L,g_pBattleGroundQueueFrame->iType);
	lua_pushinteger(L,g_pBattleGroundQueueFrame->emQueueStatus);
	lua_pushinteger(L,g_pBattleGroundQueueFrame->dwTimeExcepted);
	lua_pushinteger(L,g_pBattleGroundQueueFrame->dwTimeToLeaveQueue);
	lua_pushinteger(L,g_pBattleGroundQueueFrame->m_IsLeaderSelf);
	return 5;
}

int LuaFunc_JoinBattleGround(lua_State* L)
{
	int iBGType = luaL_checkint( L, 1 );

	CNetCli_BattleGround::JoinBattleGround( 0, 0, -1, iBGType, NULL );

	return 0;
}

int LuaFunc_JoinBattleGroundWithTeam(lua_State* L)
{
	int iBGType = luaL_checkint( L, 1 );

	if (g_pBattleGroundQueueFrame->m_IsLeaderSelf == 1)
	{
		CNetCli_BattleGround::JoinBattleGround( 0, 0, g_pBattleGroundQueueFrame->m_partypeoplecount, iBGType, g_pBattleGroundQueueFrame->m_TeamDBID );
	}

	return 0;
}

void CBattleGroundQueueFrame::GetBattleGroundResult(int BGType)//取得戰場列表
{
	m_IsAllowJoinWithTeam = 0;

	GameObjDbStruct* ZoneObj = CNetGlobal::GetObj(BGType + Def_ObjectClass_Zone);
	if(ZoneObj == NULL)
		return;

	m_IsAllowJoinWithTeam = ZoneObj->Zone.IsAllowTeamJoin;
	resultType = BGType;
	GetJoinBGTeamDBID();

	SendWorldEvent(UiEvent_GotBattleGroundQueueResult);
}

int LuaFunc_ReturnBattleGroundType(lua_State* L)
{
	lua_pushinteger(L,g_pBattleGroundQueueFrame->resultType);
	lua_pushinteger(L,g_pBattleGroundQueueFrame->m_IsAllowJoinWithTeam);
	lua_pushinteger(L,g_pBattleGroundQueueFrame->m_IsLeaderSelf);
	return 3;
}

int LuaFunc_GetBattleGroundResultType(lua_State* L)
{
	int l_DBID = g_pGameMain->GetDBID();
	CNetCli_BattleGround::GetBattleGroundList( l_DBID );
	g_pBattleGroundQueueFrame->FlagOfOpenInterface = true;
	return 0;
}

void CBattleGroundQueueFrame::GetJoinBGTeamDBID()
{
	//SecureZeroMemory(m_TeamDBID,sizeof(m_TeamDBID));
	for (int i=0; i<=36; i++)
	{
		m_TeamDBID[i] = -1;
	}
	m_LeaderDBID = -1;
	m_IsLeaderSelf = 0;
	m_partypeoplecount = 0;

	// 沒有隊伍
	if ( NetCli_PartyChild::GetPartyState() != ePartyState_Leader && NetCli_PartyChild::GetPartyState() != ePartyState_Member )
		return;

	int i = 0;
	int j = 1;

	while (i<36)
	{
		PartyMemberInfo* memberInfo = NetCli_PartyChild::GetRaidMember(i);
		//if ( memberInfo == NULL )
		//	break;
		if ( memberInfo != NULL )
		{
			CRoleSprite* sprite = (CRoleSprite*)(memberInfo->sprite);

			if ( strcmp(sprite->GetName(), NetCli_PartyChild::GetPartyLeaderName()) == 0 )//取得隊長的DBID
			{
				m_LeaderDBID = memberInfo->DBID;
				m_TeamDBID[0] = memberInfo->DBID;
				m_partypeoplecount+=1;

				if (m_TeamDBID[0] != g_pGameMain->GetDBID())
				{
					m_IsLeaderSelf = 0;
				}
				else
				{
					m_IsLeaderSelf = 1;
				}
			}
			else
			{
				m_TeamDBID[j] = memberInfo->DBID;//取得團隊其他成員DBID
				m_partypeoplecount+=1;
				j++;
			}
		}
		i++;
	}
}