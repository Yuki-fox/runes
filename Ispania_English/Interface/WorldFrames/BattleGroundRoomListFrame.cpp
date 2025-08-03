#include "../../mainproc/GameMain.h"
#include "BattleGroundRoomListFrame.h"
#include "../../netwaker_member/Net_BattleGround/NetCli_BattleGround_Child.h"

CBattleGroundRoomListFrame*	g_pBattleGroundRoomListFrame = NULL;

int LuaFunc_SelectBattleGroundRoom(lua_State* L);
int LuaFunc_GetBattleGroundRoomNum(lua_State* L);
int LuaFunc_GetBattleGroundRoomName(lua_State* L);

CBattleGroundRoomListFrame::CBattleGroundRoomListFrame(CInterface* object)
: CInterfaceBase(object)
{
	g_pBattleGroundRoomListFrame = this;
	m_vecRoom.clear();
}

CBattleGroundRoomListFrame::~CBattleGroundRoomListFrame()
{
	g_pBattleGroundRoomListFrame = NULL;
}

void CBattleGroundRoomListFrame::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{
		lua_register(L, "SelectBattleGroundRoom"	,	LuaFunc_SelectBattleGroundRoom);
		lua_register(L, "GetBattleGroundRoomNum"	,	LuaFunc_GetBattleGroundRoomNum);
		lua_register(L, "GetBattleGroundRoomName"	,	LuaFunc_GetBattleGroundRoomName);
	}
}

void CBattleGroundRoomListFrame::Open()
{
	SendWorldEvent(UiEvent_OpenBattleGroundRoomListFrame);
}

void CBattleGroundRoomListFrame::Close()
{
	SendWorldEvent(UiEvent_CloseBattleGroundRoomListFrame);
}

void CBattleGroundRoomListFrame::UpdateRoomList(int iType, vector< structBattleGroundResult > vecRoom)
{
	structBattleGroundResult bgRoom;
	SecureZeroMemory(&bgRoom, sizeof(bgRoom));

	bgRoom.iRoomID = 0;
	bgRoom.szName  = g_ObjectData->GetString( "BG_ROOM_LIST_FRAME_AUTO_SELECT" );

	m_vecRoom.clear();

	m_vecRoom.push_back(bgRoom);

	for (vector< structBattleGroundResult >::iterator it = vecRoom.begin(); it != vecRoom.end(); it++)
	{
		bgRoom = *it;

		if (bgRoom.szName == "")
		{
			char bgRoomName [64];
			sprintf(bgRoomName, "%s ", g_ObjectData->GetZoneNameByID(iType), bgRoom.iRoomID);
			bgRoom.szName = bgRoomName;
		}

		m_vecRoom.push_back((*it));
	}

	m_iType = iType;

	SendWorldEvent(UiEvent_UpdateBattleGroundRoomListFrame);
}

int LuaFunc_SelectBattleGroundRoom(lua_State* L)
{
	int iIndex = luaL_checkint( L, 1 );

	CRoleSprite* target = g_pGameMain->GetPlayerTarget();
	RoleDataEx* role = CNetGlobal::RoleData();

	if( target != NULL )
	{
		int iRoomNum = (int)g_pBattleGroundRoomListFrame->m_vecRoom.size();

		if ((iIndex >= 0) && (iIndex < iRoomNum))
		{
			CNetCli_BattleGround::JoinBattleGround( target->GetIdentify(), g_pBattleGroundRoomListFrame->m_vecRoom[iIndex].iRoomID, role->PartyID(), g_pBattleGroundRoomListFrame->m_iType, NULL );
		}
	}	
	return 0;
}

int LuaFunc_GetBattleGroundRoomNum(lua_State* L)
{
	lua_pushnumber(L, (lua_Number)g_pBattleGroundRoomListFrame->m_vecRoom.size());
	return 1;
}

int LuaFunc_GetBattleGroundRoomName(lua_State* L)
{
	int iIndex = luaL_checkint( L, 1 );

	if (iIndex == -1)
	{
		
		//lua_pushstring(L, g_ObjectData->GetZoneNameByID(g_pBattleGroundRoomListFrame->m_iType));
		lua_pushstring(L, CNetCli_BattleGround_Child::GetBattleGroundName(g_pBattleGroundRoomListFrame->m_iType));
		return 1;
	}

	int iRoomNum = (int)g_pBattleGroundRoomListFrame->m_vecRoom.size();

	if ((iIndex >= 0) && (iIndex < iRoomNum))
	{
		lua_pushstring(L, g_pBattleGroundRoomListFrame->m_vecRoom[iIndex].szName.Begin());
		return 1;
	}

	return 0;
}