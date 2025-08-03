#include "../mainproc/GameMain.h"
#include "BattleGroundOptionButtonFrame.h"
#include "../../netwaker_member/Net_BattleGround/NetCli_BattleGround_Child.h"
#include "../../netwaker_member/Net_Other/NetCli_OtherChild.h"
#include "../../netwaker_member/Net_Script/NetCli_Script_Child.h"
#include "../../netwaker_member/net_party/NetCli_PartyChild.h"
#include "../Map/Minimap.h"
#include "ChatFrame.h"


CBattleGroundOptionButtonFrame*	g_pBattleGroundOptionButtonFrame = NULL;

int LuaFunc_EnterBattleGround(lua_State* L);
int LuaFunc_LeaveBattleGroundWaitQueue(lua_State* L);
int LuaFunc_LeaveBattleGround(lua_State* L);
int LuaFunc_ChangeDungeon(lua_State* L);
int LuaFunc_GetBattleGroundName(lua_State* L);
int LuaFunc_GetTimeInBattleGroundQueue(lua_State* L);
int LuaFunc_GetBattleGroundQueueStatusNum(lua_State* L);
int LuaFunc_GetBattleGroundQueueStatus(lua_State* L);
int LuaFunc_DlgCancelEnterBattleGround(lua_State* L);

CBattleGroundOptionButtonFrame::CBattleGroundOptionButtonFrame(CInterface* object)
: CInterfaceBase(object)
{
	g_pBattleGroundOptionButtonFrame = this;
	SecureZeroMemory(m_bDlgFlag, sizeof(m_bDlgFlag));
}

CBattleGroundOptionButtonFrame::~CBattleGroundOptionButtonFrame()
{
	g_pBattleGroundOptionButtonFrame = NULL;
}

void CBattleGroundOptionButtonFrame::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{
		lua_register(L, "EnterBattleGround"				,	LuaFunc_EnterBattleGround);
		lua_register(L, "LeaveBattleGroundWaitQueue"	,	LuaFunc_LeaveBattleGroundWaitQueue);
		lua_register(L, "LeaveBattleGround"				,	LuaFunc_LeaveBattleGround);
		lua_register(L, "ChangeDungeon"					,	LuaFunc_ChangeDungeon);
		lua_register(L, "GetBattleGroundName"			,	LuaFunc_GetBattleGroundName);
		lua_register(L, "GetTimeInBattleGroundQueue"	,	LuaFunc_GetTimeInBattleGroundQueue);
		lua_register(L, "GetBattleGroundQueueStatusNum"	,	LuaFunc_GetBattleGroundQueueStatusNum);
		lua_register(L, "GetBattleGroundQueueStatus"	,	LuaFunc_GetBattleGroundQueueStatus);
		lua_register(L, "DlgCancelEnterBattleGround"	,	LuaFunc_DlgCancelEnterBattleGround);
	}
}

void CBattleGroundOptionButtonFrame::Update(float elapsedTime)
{
	static float lastTime = 0.0f;
	lastTime += elapsedTime;
	if ( lastTime < 0.5f )
		return;

	lastTime = 0.0f;

	char Msg[128];

	map< int, StructBattleGroundQueueStatus >& mapStatus = CNetCli_BattleGround_Child::GetQueseStatusMap();
	map< int, StructBattleGroundQueueStatus >::iterator it;
		
	DWORD nowTick = timeGetTime();

	int iType = g_pGameMain->GetBattleGroundType();

	// 檢查是不是在戰場內, 在戰場內自動移除
	it = mapStatus.find(iType);
	if (it!= mapStatus.end())
	{
		mapStatus.erase(it);
	}

	bool bHaveTicket = false;

	//檢查是否已到離開佇列時間
	for (it = mapStatus.begin(); it != mapStatus.end(); ++it)
	{
		if (it->second.dwTimeToLeaveQueue > 0)
		{
			if (nowTick > it->second.dwTimeToLeaveQueue)
			{
				if((it->first % 1000) > 400)
				{
					sprintf(Msg, g_ObjectData->GetString("BG_PLAYER_LEAVE_QUEUE"), CNetCli_BattleGround_Child::GetBattleGroundName(it->second.iType));
					g_pGameMain->SendSystemMsg(Msg);
					g_pChatFrame->SendMsgEvent( CHAT_MSG_SYSTEM, Msg, "" );

					// 時間到移除
					it = mapStatus.erase(it);

					if (it == mapStatus.end())
					{
						break;
					}
				}								
			}
		}

		if ((it->second.emQueueStatus == EM_BATTLEGROUNDSTATUS_GOT_TICKET_WAIT_ENTER) || (it->second.emQueueStatus == EM_BATTLEGROUNDSTATUS_GOT_TICKET_WAIT_ENTER_HIDE) )
		{
			bHaveTicket = true;
		}
	}

	//dialog status check
	lua_State* L = GetLuaStateWorld();
	for (int i = 0; i < MAX_ENTER_BG_QUERY_DLG_NUM; ++i)
	{
		if (m_bDlgFlag[i] > 0)
		{
			it = mapStatus.find(m_bDlgFlag[i]);

			if (it != mapStatus.end())
			{
				if ((it->second.emQueueStatus != EM_BATTLEGROUNDSTATUS_GOT_TICKET_WAIT_ENTER) && (it->second.emQueueStatus != EM_BATTLEGROUNDSTATUS_GOT_TICKET_WAIT_ENTER_HIDE))
				{
					m_bDlgFlag[i] = 0;
					lua_pushnumber( L, i + 1);
					lua_setglobal(L, "arg1");
					SendWorldEvent(UiEvent_CloseBattleGroundDialog);
				}
				else if(((it->second.iType % 1000) < 400) && (CBattleGroundOptionButtonFrame::IsLeader() == 1))
				{
					//世界戰場隊長不秀進入戰場介面，所以如果是繼任隊長，就把介面給關閉，並送出LeaveBattleGroundRemote。
					m_bDlgFlag[i] = 0;
					lua_pushnumber( L, i + 1);
					lua_setglobal(L, "arg1");
					SendWorldEvent(UiEvent_CloseBattleGroundDialog);

					CNetCli_BattleGround::LeaveBattleGroundRemote(g_pGameMain->GetDBID(), it->second.iType);
					mapStatus.erase(it);
				}
			}
			else
			{
				if((m_bDlgFlag[i] % 1000) < 400)
				{
					CNetCli_BattleGround::LeaveBattleGroundRemote(g_pGameMain->GetDBID(), m_bDlgFlag[i]);
				}

				m_bDlgFlag[i] = 0;
				lua_pushnumber( L, i + 1);
				lua_setglobal(L, "arg1");
				SendWorldEvent(UiEvent_CloseBattleGroundDialog);				
			}
		}
	}

	//
	if (!bHaveTicket)
	{
		StopFlashBGOptionButton();
	}
}

void CBattleGroundOptionButtonFrame::ShowEnterBGQueryDialog(int iType, int iRoomID)
{
	char szBG[1024];
	bool bHideSureButton = false;
	string strRoomName = "";
	string strContent = "";
	int iHideSureButton = 0;
	int iIndex = 0;

	//-----------------------------------------------
	//Room Name
	SecureZeroMemory(szBG, sizeof(szBG));

	if ( (iType == 350) || (iType == 410) )
	{
		sprintf(szBG, "%s", CNetCli_BattleGround_Child::GetBattleGroundName(iType));
	}
	else
	{
		sprintf(szBG, "%s %d", CNetCli_BattleGround_Child::GetBattleGroundName(iType), iRoomID);
	}

	strRoomName  = szBG;
	//-----------------------------------------------

	//-----------------------------------------------
	//Content
	SecureZeroMemory(szBG, sizeof(szBG));
	sprintf(szBG, g_ObjectData->GetString("BG_DLG_ASK_ENTER_BG_ROOM"), strRoomName.c_str());

	strContent = szBG;
	//-----------------------------------------------

	//-----------------------------------------------
	//玩家在死亡 or 戰鬥中, hide sure button
	CRoleSprite* player = g_pGameMain->GetPlayer();
	if (player)
	{		
		if (player->GetDeadState() || player->GetCombatState())
		{
			bHideSureButton = true;
		}
	}
	//-----------------------------------------------

	for (int i = 0; i < MAX_ENTER_BG_QUERY_DLG_NUM; ++i)
	{
		//we find a exsit dialog
		if (m_bDlgFlag[i] == iType)
		{
			return;
		}

		if (m_bDlgFlag[i] == 0)
		{
			m_bDlgFlag[i] = iType;
			iIndex = i + 1;
			break;
		}
	}

	if (bHideSureButton)
	{
		iHideSureButton = 1;
	}
//----------------------------------------------------世界戰場用----------------------------------------------------------------
	int IsLeaderSelf = 0;
	int IsTeam = 0;

	IsTeam = CBattleGroundOptionButtonFrame::IsTeam();
	IsLeaderSelf = CBattleGroundOptionButtonFrame::IsLeader();
//-----------------------------------------------------------------------------------------------------------------------------
	lua_State* L = GetLuaStateWorld();
	lua_pushstring( L, strContent.c_str());
	lua_setglobal(L, "arg1");
	lua_pushnumber( L, iType);
	lua_setglobal(L, "arg2");
	lua_pushnumber( L, iIndex);
	lua_setglobal(L, "arg3");
	lua_pushnumber( L, iHideSureButton);
	lua_setglobal(L, "arg4");
	lua_pushstring( L, CNetCli_Script_Child::ReplaceSystemKeyword( g_ObjectData->GetString("BG_DLG_SURE_ENTER_ROOM") ).c_str());
	lua_setglobal(L, "arg5");
	if((iType % 1000) > 400)
		lua_pushstring( L, CNetCli_Script_Child::ReplaceSystemKeyword( g_ObjectData->GetString("BG_DLG_HIDE_TICKET") ).c_str());
	else
		lua_pushstring( L, CNetCli_Script_Child::ReplaceSystemKeyword( g_ObjectData->GetString("BG_DLG_CANCEL_JOIN_ROOM") ).c_str());
	lua_setglobal(L, "arg6");
	lua_pushnumber( L, IsLeaderSelf);
	lua_setglobal(L, "arg7");
	lua_pushnumber( L, IsTeam);
	lua_setglobal(L, "arg8");

	SendWorldEvent(UiEvent_OpenBattleGroundDialog);
}

void CBattleGroundOptionButtonFrame::StartFlashBGOptionButton()
{
	SendWorldEvent(UiEvent_StartFlashBattleGroundOptionButton);
}

void CBattleGroundOptionButtonFrame::StopFlashBGOptionButton()
{
	SendWorldEvent(UiEvent_StopFlashBattleGroundOptionButton);
}

int CBattleGroundOptionButtonFrame::IsTeam()
{
	if ( NetCli_PartyChild::GetPartyState() != ePartyState_Leader && NetCli_PartyChild::GetPartyState() != ePartyState_Member )
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int CBattleGroundOptionButtonFrame::IsLeader()
{
	int i = 0;

	while (true)
	{
		if(IsTeam == 0 || i >= 36)
			break;

		PartyMemberInfo* memberInfo = NetCli_PartyChild::GetRaidMember(i);

		if(memberInfo == NULL)
		{
			i++;
			continue;
		}			

		CRoleSprite* sprite = (CRoleSprite*)(memberInfo->sprite);

		if ( strcmp(sprite->GetName(), NetCli_PartyChild::GetPartyLeaderName()) == 0 )//取得隊長的DBID
		{
			int iTeamDBID = memberInfo->DBID;

			if (iTeamDBID != g_pGameMain->GetDBID())
			{
				return 0;
			}
			else
			{
				return 1;
			}

			break;
		}

		i++;
	}
	return 0;
}

void CBattleGroundOptionButtonFrame::LeaveBattleGroundGroup(int iZoneID)
{
	if((iZoneID % 1000) == 190)
		NetCli_PartyChild::KickMember(g_pGameMain->GetPlayerName());
}

int LuaFunc_EnterBattleGround(lua_State* L)
{	
	int iBattleGroundType = luaL_checkint( L, 1 );

	map< int, StructBattleGroundQueueStatus >& mapStatus = CNetCli_BattleGround_Child::GetQueseStatusMap();

	map< int, StructBattleGroundQueueStatus >::iterator it = mapStatus.find(iBattleGroundType);

	if (it != mapStatus.end())
	{
		CRoleSprite* player = g_pGameMain->GetPlayer();

		if (player)
		{

			if ( (g_pGameMain->GetZoneID() % 1000) > 100)
			{
				g_pGameMain->SendSystemMsg(g_ObjectData->GetString("BG_MSG_IN_INDEPENDENT_ZONE_NOT_ALLOW_ENTER"));
				g_pChatFrame->SendMsgEvent( CHAT_MSG_SYSTEM, g_ObjectData->GetString("BG_MSG_IN_INDEPENDENT_ZONE_NOT_ALLOW_ENTER"), "" );
				return 0;
			}

			//玩家已死亡 
			if (player->GetDeadState())
			{
				g_pGameMain->SendSystemMsg(g_ObjectData->GetString("BG_MSG_YOU_ARE_DEAD_NOT_ALLOW_ENTER"));
				g_pChatFrame->SendMsgEvent( CHAT_MSG_SYSTEM, g_ObjectData->GetString("BG_MSG_YOU_ARE_DEAD_NOT_ALLOW_ENTER"), "" );
				return 0;
			}
			//玩家戰鬥中
			else if ( player->GetCombatState())
			{
				g_pGameMain->SendSystemMsg(g_ObjectData->GetString("BG_MSG_COMBATING_NOT_ALLOW_ENTER"));
				g_pChatFrame->SendMsgEvent( CHAT_MSG_SYSTEM, g_ObjectData->GetString("BG_MSG_COMBATING_NOT_ALLOW_ENTER"), "" );
				return 0;
			}
		}

		// return if ticket has been sent
		if (it->second.emQueueStatus == EM_BATTLEGROUNDSTATUS_TICKET_SENT)
		{
			return 0;
		}

		//CNetCli_BattleGround::EnterBattleGround(0, iBattleGroundType, (*it).second.iRoomID, (*it).second.iTeamID);
		CNetCli_BattleGround::EnterBattleGround(iBattleGroundType, (*it).second.iRoomID, (*it).second.iTeamID);

		it->second.emQueueStatus = EM_BATTLEGROUNDSTATUS_TICKET_SENT;
	}

	return 0;
}

int LuaFunc_LeaveBattleGroundWaitQueue(lua_State* L)
{
	int iType = luaL_checkint( L, 1 );

	map< int, StructBattleGroundQueueStatus >& mapStatus = CNetCli_BattleGround_Child::GetQueseStatusMap();

	map< int, StructBattleGroundQueueStatus >::iterator it = mapStatus.find(iType);

	if (it != mapStatus.end())
	{
		mapStatus.erase(it);
	}

	CNetCli_BattleGround::LeaveBattleGroundWaitQueue(g_pGameMain->GetDBID(), iType);

 	if((iType % 1000) < 400)
 		return 0;

	char Msg[128];

	sprintf(Msg, g_ObjectData->GetString("BG_PLAYER_LEAVE_QUEUE"), CNetCli_BattleGround_Child::GetBattleGroundName(iType));
	g_pGameMain->SendSystemMsg(Msg);
	g_pChatFrame->SendMsgEvent( CHAT_MSG_SYSTEM, Msg, "" );

	return 0;
}

int LuaFunc_LeaveBattleGround(lua_State* L)
{
	int iType = g_pGameMain->GetBattleGroundType();
	map< int, StructBattleGroundQueueStatus >& mapStatus = CNetCli_BattleGround_Child::GetQueseStatusMap();

	map< int, StructBattleGroundQueueStatus >::iterator it = mapStatus.find(iType);

	if (it != mapStatus.end())
	{
		mapStatus.erase(it);
	}
	g_pGameMain->SetCombatState(false);
	CNetCli_BattleGround::LeaveBattleGround();
	return 0;
}

int LuaFunc_ChangeDungeon(lua_State* L)
{
	//CNetCli_BattleGround::GetBattleGround()
	return 0;
}

int LuaFunc_GetBattleGroundName(lua_State* L)
{
	int iType = luaL_checkint( L, 1 );

	lua_pushstring( L, CNetCli_BattleGround_Child::GetBattleGroundName(iType));
	return 1;
}

int LuaFunc_GetTimeInBattleGroundQueue(lua_State* L)
{
	int iType = luaL_checkint( L, 1 );

	DWORD dwTick = 0;

	map< int, StructBattleGroundQueueStatus >& mapStatus = CNetCli_BattleGround_Child::GetQueseStatusMap();
	map< int, StructBattleGroundQueueStatus >::iterator it = mapStatus.find(iType);

	if (it != mapStatus.end())
	{
		dwTick = timeGetTime() - (*it).second.dwTimeToEnterQueue;
	}

	lua_pushnumber( L, dwTick);
	return 1;
}

int LuaFunc_GetBattleGroundQueueStatusNum(lua_State* L)
{
	map< int, StructBattleGroundQueueStatus >& mapStatus = CNetCli_BattleGround_Child::GetQueseStatusMap();

	int iSize = (int)mapStatus.size();

	lua_pushnumber( L, iSize);
	return 1;
}

int LuaFunc_GetBattleGroundQueueStatus(lua_State* L)
{
	int iStatus	 = 0;
	int iType    = 0;
	DWORD dwTimeEnter = 0;
	DWORD dwTimeExcepted = 0;
	DWORD dwTimeToLeave = 0;

	int iCurBGType = luaL_checkint( L, 1 );

	map< int, StructBattleGroundQueueStatus >& mapStatus = CNetCli_BattleGround_Child::GetQueseStatusMap();
	map< int, StructBattleGroundQueueStatus >::iterator it;

	//Get First Item
	if (iCurBGType == 0)
	{
		it = mapStatus.begin();

		if (it != mapStatus.end())
		{
			iStatus		   = (*it).second.emQueueStatus;
			iType		   = (*it).second.iType;
			dwTimeEnter    = (*it).second.dwTimeToEnterQueue;
			dwTimeExcepted = (*it).second.dwTimeExcepted;
			dwTimeToLeave  = (*it).second.dwTimeToLeaveQueue;
		}
	}
	else 
	{
		it = mapStatus.find(iCurBGType);

		if (it != mapStatus.end())
		{
			it++; //we need to get next item

			if (it != mapStatus.end())
			{
				iStatus		   = (*it).second.emQueueStatus;
				iType          = (*it).second.iType;
				dwTimeEnter    = (*it).second.dwTimeToEnterQueue;
				dwTimeExcepted = (*it).second.dwTimeExcepted;
				dwTimeToLeave  = (*it).second.dwTimeToLeaveQueue;
			}
		}
	}

	lua_pushnumber( L, iStatus);
	lua_pushnumber( L, iType);
	lua_pushnumber( L, dwTimeExcepted);
	lua_pushnumber( L, dwTimeToLeave);
	lua_pushnumber( L, dwTimeEnter);

	return 5;
}

int LuaFunc_DlgCancelEnterBattleGround(lua_State* L)
{
	int iIndex = luaL_checkint( L, 1 );

	int iType = g_pBattleGroundOptionButtonFrame->m_bDlgFlag[iIndex];

	map< int, StructBattleGroundQueueStatus >& mapStatus = CNetCli_BattleGround_Child::GetQueseStatusMap();
	map< int, StructBattleGroundQueueStatus >::iterator it = mapStatus.find(iType);

	if (it != mapStatus.end() && it->second.emQueueStatus != EM_BATTLEGROUNDSTATUS_TICKET_SENT)
	{
		it->second.emQueueStatus = EM_BATTLEGROUNDSTATUS_GOT_TICKET_WAIT_ENTER_HIDE;

		if ( !(NetCli_PartyChild::GetPartyState() != ePartyState_Leader && NetCli_PartyChild::GetPartyState() != ePartyState_Member) )
		{
			int IsLeaderSelf = 0;
			int IsTeam = 0;

			IsTeam = CBattleGroundOptionButtonFrame::IsTeam();
			IsLeaderSelf = CBattleGroundOptionButtonFrame::IsLeader();

			if((iType % 1000) < 400 && IsLeaderSelf == 0)
			{
				CNetCli_BattleGround::LeaveBattleGroundRemote(g_pGameMain->GetDBID(), iType);

				mapStatus.erase(it);
			}	
			
		}
		
	}

	g_pBattleGroundOptionButtonFrame->m_bDlgFlag[iIndex] = 0;

	return 0;
}