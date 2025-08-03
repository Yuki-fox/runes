#include "../netwaker_member/NetWakerClientInc.h"
#include "../tools/Common/Com_Function.h"
#include "../../mainproc/GameMain.h"
#include "../Map/MapCommon.h"
#include "ChatFrame.h"
#include "UnitFrame.h"
#include "PartyFrame.h"

CPartyFrame*					g_pPartyFrame = NULL;

int	LuaFunc_InviteToParty		(lua_State* L);
int LuaFunc_InviteByName		(lua_State* L);
int	LuaFunc_UninviteFromParty	(lua_State* L);
int	LuaFunc_UninviteByName		(lua_State* L);
int	LuaFunc_PromoteToPartyLeader(lua_State* L);
int LuaFunc_LeaveParty			(lua_State* L);
int	LuaFunc_GetPartyMember		(lua_State* L);
int LuaFunc_GetPartyLeaderIndex	(lua_State* L);
int LuaFunc_IsPartyLeader		(lua_State* L);
int LuaFunc_GetNumPartyMembers	(lua_State* L);
int LuaFunc_InPartyByName		(lua_State* L);
int LuaFunc_InRaidByName		(lua_State* L);


// 團隊
int LuaFunc_GetRaidMember		(lua_State* L);
int LuaFunc_SwitchToRaid		(lua_State* L);
int LuaFunc_SwithRaidAssistant	(lua_State* L);

int LuaFunc_SwithRaidMainTank	 (lua_State* L);
int LuaFunc_SwithRaidMainAttacker(lua_State* L);

int LuaFunc_RaidDegrade			(lua_State* L);	 //降級


int LuaFunc_MoveRaidMember		(lua_State* L);
int LuaFunc_GetNumRaidMembers	(lua_State* L);
int LuaFunc_GetRaidLeaderIndex	(lua_State* L);

int LuaFunc_AcceptGroup			(lua_State* L);
int LuaFunc_DeclineGroup		(lua_State* L);
int LuaFunc_IsAssigner			(lua_State* L);
int LuaFunc_IsRaidLeader		(lua_State* L);
int LuaFunc_IsRaidAssistant		(lua_State* L);
int LuaFunc_IsRaidMainTank		(lua_State* L);
int LuaFunc_IsRaidMainAttack	(lua_State* L);
int LuaFunc_GetLeaderChannelID	(lua_State* L);

// 戰場團隊
int LuaFunc_AcceptBattleGroup	(lua_State* L);
int LuaFunc_DeclineBattleGroup	(lua_State* L);

int LuaFunc_ResetInstance		(lua_State* L);
int LuaFunc_SetInstanceLevel	(lua_State* L);
int LuaFunc_GetInstanceLevel	(lua_State* L);

// ----------------------------------------------------------------------------
CPartyFrame::CPartyFrame(CInterface* object)
	: CInterfaceBase(object)
{
	g_pPartyFrame = this;

	m_resetInstanceCount = 0;
	m_resetInstanceTime = 0.0f;
	m_resetInstanceLock = false;
}

// ----------------------------------------------------------------------------
CPartyFrame::~CPartyFrame()
{
	g_pPartyFrame = NULL;
}

// ----------------------------------------------------------------------------
void CPartyFrame::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{
		lua_register( L, "InviteToParty",			LuaFunc_InviteToParty );
		lua_register( L, "InviteByName",			LuaFunc_InviteByName );		
		lua_register( L, "UninviteFromParty",		LuaFunc_UninviteFromParty );
		lua_register( L, "UninviteByName",			LuaFunc_UninviteByName );
		lua_register( L, "PromoteToPartyLeader",	LuaFunc_PromoteToPartyLeader );
		lua_register( L, "LeaveParty",				LuaFunc_LeaveParty );
		lua_register( L, "GetPartyMember",			LuaFunc_GetPartyMember );
		lua_register( L, "GetPartyLeaderIndex",		LuaFunc_GetPartyLeaderIndex );
		lua_register( L, "IsPartyLeader",			LuaFunc_IsPartyLeader );
		lua_register( L, "GetNumPartyMembers",		LuaFunc_GetNumPartyMembers );
		lua_register( L, "GetNumRaidMembers",		LuaFunc_GetNumRaidMembers );
		lua_register( L, "InPartyByName",			LuaFunc_InPartyByName );
		lua_register( L, "InRaidByName",			LuaFunc_InRaidByName );

		lua_register( L, "GetRaidMember",			LuaFunc_GetRaidMember );
		lua_register( L, "SwitchToRaid",			LuaFunc_SwitchToRaid );
		lua_register( L, "SwithRaidAssistant",		LuaFunc_SwithRaidAssistant );
  		lua_register( L, "SwithRaidMainTank",		LuaFunc_SwithRaidMainTank );
		lua_register( L, "SwithRaidMainAttacker",	LuaFunc_SwithRaidMainAttacker );
 		lua_register( L, "RaidDegrade",				LuaFunc_RaidDegrade );

		lua_register( L, "MoveRaidMember",			LuaFunc_MoveRaidMember );
		lua_register( L, "GetRaidLeaderIndex",		LuaFunc_GetRaidLeaderIndex );
   
		lua_register( L, "AcceptGroup",				LuaFunc_AcceptGroup );
		lua_register( L, "DeclineGroup",			LuaFunc_DeclineGroup );

		lua_register( L, "IsAssigner",				LuaFunc_IsAssigner );
		lua_register( L, "IsRaidLeader",			LuaFunc_IsRaidLeader );
		lua_register( L, "IsRaidAssistant",			LuaFunc_IsRaidAssistant );
		lua_register( L, "IsRaidMainTank",			LuaFunc_IsRaidMainTank );
		lua_register( L, "IsRaidMainAttack",		LuaFunc_IsRaidMainAttack );

		lua_register( L, "GetLeaderChannelID",		LuaFunc_GetLeaderChannelID );

		lua_register( L, "AcceptBattleGroup",		LuaFunc_AcceptBattleGroup );
		lua_register( L, "DeclineBattleGroup",		LuaFunc_DeclineBattleGroup );

		lua_register( L, "ResetInstance",			LuaFunc_ResetInstance );
		lua_register( L, "SetInstanceLevel",		LuaFunc_SetInstanceLevel );
		lua_register( L, "GetInstanceLevel",		LuaFunc_GetInstanceLevel );
	}
}

// ----------------------------------------------------------------------------
void CPartyFrame::Update(float elapsedTime)
{
	if ( m_resetInstanceLock )
	{
		m_resetInstanceTime -= elapsedTime;
		if ( m_resetInstanceTime < 0.1f )
		{
			m_resetInstanceLock = false;
			m_resetInstanceTime = 0.0f;
			m_resetInstanceCount = 0;

			g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_RESET_ALL_INSTANCE_FAILED"));
		}
	}	
}

// ----------------------------------------------------------------------------
void CPartyFrame::RefreshPartyMember()
{
	SendWorldEvent( UiEvent_PartyMemberChanged );
}

// ----------------------------------------------------------------------------
void CPartyFrame::RefreshPartyLeader()
{
	SendWorldEvent( UiEvent_PartyLeaderChanged );
}

// ----------------------------------------------------------------------------
// 解除戰場組隊
void CPartyFrame::ClearBattleGroup()
{	
	NetCli_PartyChild::ClearBattleGroup();
}

// ----------------------------------------------------------------------------
void CPartyFrame::SendResetInstance()
{
	if ( m_resetInstanceLock )
		return;

	if ( NetCli_PartyChild::GetPartyState() != ePartyState_Leader )
		return;

	m_resetInstanceCount = 0;
	m_resetInstanceFailed.clear();

	// 重置所有副本(1 - 300)
	for ( int i = 1; i < 300; i++ )
	{
		GameObjDbStructEx* zoneDB = g_ObjectData->GetObj(Def_ObjectClass_Zone + i);
		if ( zoneDB && zoneDB->Zone.IsPrivateZone &&
			 (zoneDB->Zone.PrivateZoneType == EM_PrivateZoneType_Party || zoneDB->Zone.PrivateZoneType == EM_PrivateZoneType_Raid || zoneDB->Zone.PrivateZoneType == EM_PrivateZoneType_Private_Party) )
		{
			NetCli_Other::SL_ResetInstanceRequest(i);
			m_resetInstanceCount++;
		}		
	}

	m_resetInstanceLock = true;
	m_resetInstanceTime = 10.0f;	
}

// ----------------------------------------------------------------------------
void CPartyFrame::RequestInstanceRestTime(bool isForce)
{
	if ( !m_instanceServerInfo.empty() && !isForce )
		return;

	char temp[512];	

	m_instanceServerInfo.clear();
	for ( int i = 0; i < 100; i++ )
	{
		// 取得副本編號
		sprintf_s(temp, 512, "InstanceKey%02d", i);
		int zoneId = g_ObjectData->GetSysKeyValue(temp);

		if ( zoneId > 0 )
		{
			InstanceServerInfo info;
			info.extendTime = 0;
			info.liveTime = 0;
			info.keyId = -1;
			info.instanceKey = i;
			info.requestFrameTime = g_pGameMain->GetFrameTime();
			m_instanceServerInfo[zoneId] = info;
			Net_Instance::SL_InstanceRestTimeRequest(zoneId, i);			
		}
	}
}

// ----------------------------------------------------------------------------
void CPartyFrame::SwitchExtendInstanceTime(int instanceIndex)
{
	if ( instanceIndex < 0 || instanceIndex >= _MAX_INSTANCE_SETTING_COUNT_ )
		return;

	/*
	if ( CNetGlobal::RoleData()->PlayerBaseData->InstanceSetting.KeyID[instanceIndex] <= 0 )
	{
		return;
	}
	*/

	for ( map<int, InstanceServerInfo>::iterator iter = m_instanceServerInfo.begin(); iter != m_instanceServerInfo.end(); iter++ )
	{
		if ( iter->second.instanceKey == instanceIndex )
		{
			if ( iter->second.keyId == -1 )
			{
				string instanceName = Func_GetLocalMapName( iter->first );
				string temp = g_ObjectData->GetString("SYS_INSTANCE_CLOSED");
				string msg = ReplaceKeyString(temp, "<INSTANCENAME>", instanceName);
				g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", msg.c_str());
				return;
			}
			// 如果區域沒開,離上次要求超過10秒在跟SERVER要求一次副本資訊
			else if ( iter->second.extendTime == 0 )
			{
				//string instanceName = Func_GetLocalMapName( iter->first );
				//string temp = g_ObjectData->GetString("SYS_INSTANCE_EXTEND_ERROR");
				//string msg = ReplaceKeyString(temp, "<INSTANCENAME>", instanceName);
				//g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", msg.c_str());

				if ( g_pGameMain->GetFrameTime() > iter->second.requestFrameTime + 10.0 )
				{
					iter->second.requestFrameTime = g_pGameMain->GetFrameTime();
					Net_Instance::SL_InstanceRestTimeRequest(iter->first, instanceIndex);
				}
				return;
			}

			if ( CNetGlobal::RoleData()->PlayerBaseData->InstanceSetting.ExtendTime[instanceIndex] != iter->second.extendTime )
				Net_Instance::SL_InstanceExtend(iter->first, iter->second.extendTime, instanceIndex);
			else
				Net_Instance::SL_InstanceExtend(iter->first, 0, instanceIndex);
			break;
		}
	}	
}

// ----------------------------------------------------------------------------
void CPartyFrame::RL_InstanceRestTime( int ZoneID , int ExtendTime , int LiveTime , int KeyID )
{
	map<int, InstanceServerInfo>::iterator iter = m_instanceServerInfo.find(ZoneID);
	if ( iter != m_instanceServerInfo.end() )
	{
		iter->second.extendTime = ExtendTime;
		iter->second.liveTime = LiveTime;
		iter->second.keyId = KeyID;

		// Key ID 不相同
		if ( CNetGlobal::RoleData()->PlayerBaseData->InstanceSetting.KeyID[iter->second.instanceKey] != KeyID )
		{
			// 清除副本進度
			Net_Instance::SL_InstanceExtend(ZoneID, 0, iter->second.instanceKey);
		}
		
		/*
		if ( KeyID > 0 &&
			CNetGlobal::RoleData()->PlayerBaseData->InstanceSetting.LiveTime[iter->second.instanceKey] != LiveTime )
		{
			// 重設副本時間
			Net_Instance::SL_InstanceExtend(ZoneID, 0, iter->second.instanceKey);
		}
		*/
		
		// 目前延長時間不等於目前重置與下次重置時間,可能原因
		// 1.已經過了兩次CD
		// 2.重置時間有改變
		if ( KeyID > 0 &&
			//CNetGlobal::RoleData()->PlayerBaseData->InstanceSetting.LiveTime[iter->second.instanceKey] != LiveTime &&
			//CNetGlobal::RoleData()->PlayerBaseData->InstanceSetting.ExtendTime[iter->second.instanceKey] != LiveTime &&
			CNetGlobal::RoleData()->PlayerBaseData->InstanceSetting.ExtendTime[iter->second.instanceKey] != ExtendTime &&
			CNetGlobal::RoleData()->PlayerBaseData->InstanceSetting.ExtendTime[iter->second.instanceKey] != LiveTime )
		{
			// 清除延長時間
			Net_Instance::SL_InstanceExtend(ZoneID, 0, iter->second.instanceKey);
		}
	}
}

// ----------------------------------------------------------------------------
void CPartyFrame::RL_FixInstanceExtend( int ZoneID , int ExtendTime , int LiveTime , int KeyID )
{
	map<int, InstanceServerInfo>::iterator iter = m_instanceServerInfo.find(ZoneID);
	if ( iter != m_instanceServerInfo.end() )
	{
		iter->second.keyId = KeyID;

		if ( CNetGlobal::RoleData()->PlayerBaseData->InstanceSetting.KeyID[iter->second.instanceKey] > 0 && KeyID <= 0 )
		{
			string instanceName = Func_GetLocalMapName( ZoneID );
			string temp = g_ObjectData->GetString("SYS_INSTANCE_EXTEND_FAILED");
			string msg = ReplaceKeyString(temp, "<INSTANCENAME>", instanceName);
			g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", msg.c_str());
		}

		CNetGlobal::RoleData()->PlayerBaseData->InstanceSetting.KeyID[iter->second.instanceKey] = KeyID;
		CNetGlobal::RoleData()->PlayerBaseData->InstanceSetting.LiveTime[iter->second.instanceKey] = LiveTime;
		CNetGlobal::RoleData()->PlayerBaseData->InstanceSetting.ExtendTime[iter->second.instanceKey] = ExtendTime;

		SendWorldEvent("INSTANCE_RESET");
	}
}

// ----------------------------------------------------------------------------
void CPartyFrame::RL_ResetInstanceResult(bool boResult)
{
	if ( boResult )
	{
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_RESET_INSTANCE_SUCCEEDED"));
	}
	else
	{
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_RESET_INSTANCE_FAILED"));
	}

	m_resetInstanceLock = false;
	m_resetInstanceTime = 0.0f;
}

// ----------------------------------------------------------------------------
void CPartyFrame::RL_ResetInstanceInfo(int zoneID, bool boResult)
{
	m_resetInstanceCount--;

	if ( boResult )
	{
	}
	else
	{
		GameObjDbStructEx* zoneDB = g_ObjectData->GetObj(Def_ObjectClass_Zone + zoneID);
		if ( zoneDB )
		{
			g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_RESET_INSTANCE_FAILED"), zoneDB->GetName());
		}	

		m_resetInstanceFailed.push_back(zoneID);
	}

	if ( m_resetInstanceCount == 0 )
	{
		if ( m_resetInstanceFailed.empty() )
		{
			g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_RESET_ALL_INSTANCE_SUCCEEDED"));
		}

		m_resetInstanceLock = false;
		m_resetInstanceTime = 0.0f;
	}
}

// ----------------------------------------------------------------------------
int	LuaFunc_InviteToParty(lua_State* L)
{
	CRoleSprite* sprite = g_pGameMain->GetUnitSprite(luaL_checkstring(L, 1));
	if (sprite)
	{
		// 戰場內(戰場外組隊模式不同)
		if ( g_pGameMain->IsBattleGroundZone() )
		{
			if ( sprite->GetEnemyState() )
			{
				g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_INVITE_ERR_ENEMY"));
			}
			else
			{
				NetCli_PartyChild::InviteBattleGroup(sprite->GetDBID());
			}
		}
		else
		{
			NetCli_PartyChild::InviteToParty(sprite->GetName());
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_InviteByName(lua_State* L)
{
	if ( g_pGameMain->IsBattleGroundZone() )
	{
		CRoleSprite* sprite = g_pGameMain->FindBattleMemberByName(luaL_checkstring(L, 1));
		if ( sprite && sprite->GetEnemyState() )
		{
			g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_INVITE_ERR_ENEMY"));
		}
		else
		{
			NetCli_PartyChild::InviteBattleGroup((sprite) ? sprite->GetDBID() : -1);
		}
	}
	else
	{
		NetCli_PartyChild::InviteToParty(luaL_checkstring(L, 1));
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_UninviteFromParty(lua_State* L)
{
	CRoleSprite* sprite = g_pGameMain->GetUnitSprite(luaL_checkstring(L, 1));
	if (sprite)
	{
		// 戰場內(戰場外組隊模式不同)
		if ( g_pGameMain->IsBattleGroundZone() )
		{
			NetCli_PartyChild::UninviteBattleMember(sprite->GetDBID());
		}
		else
		{
			NetCli_PartyChild::KickMember(sprite->GetName());
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------
int	LuaFunc_UninviteByName(lua_State* L)
{
	if ( g_pGameMain->IsBattleGroundZone() )
	{
		CRoleSprite* sprite = g_pGameMain->FindBattleMemberByName(luaL_checkstring(L, 1));
		if ( sprite )
		{
			NetCli_PartyChild::UninviteBattleMember(sprite->GetDBID());
		}
	}
	else
	{
		NetCli_PartyChild::KickMember( luaL_checkstring(L, 1) );
	}
	return 0;
}

// ----------------------------------------------------------------------------
int	LuaFunc_PromoteToPartyLeader(lua_State* L)
{
	CRoleSprite* sprite = g_pGameMain->GetUnitSprite(luaL_checkstring(L, 1));
	if ( sprite && sprite->GetDBID() > 0 )
	{
		if ( g_pGameMain->IsBattleGroundZone() )
		{
			NetCli_PartyChild::PromoteToBattleLeader(sprite->GetDBID());
		}
		else
		{
			NetCli_PartyChild::PromoteToLeader(sprite->GetName());
		}
	}	
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_LeaveParty(lua_State* L)
{
	if ( g_pGameMain->IsBattleGroundZone() )
	{
		NetCli_PartyChild::UninviteBattleMember(g_pGameMain->GetZoneDBID(), false);
	}
	else
	{
		NetCli_PartyChild::KickMember(g_pGameMain->GetPlayerName());
	}
	return 0;
}

// ----------------------------------------------------------------------------
int	LuaFunc_GetPartyMember(lua_State* L)
{
	if ( g_pGameMain->IsBattleGroundZone() )
	{
		BattleGroundPartyMember* member = NetCli_PartyChild::GetBattlePartyMember( luaL_checkint(L, 1) - 1 );
		if ( member )
		{
			const char* name = "";

			CRoleSprite* sprite = g_pGameMain->FindBettleMemberByDBID(member->DBID);
			if ( sprite )
			{
				name = sprite->GetName();
			}

			lua_pushstring(L, name);					// 名稱
			lua_pushboolean(L, true);					// 是否在線上
			return 2;
		}
	}
	else
	{
		PartyMemberInfo* member = NetCli_PartyChild::GetPartyMember( luaL_checkint(L, 1) - 1 );
		if ( member )
		{
			CRoleSprite* sprite = (CRoleSprite*)(member->sprite);
			lua_pushstring(L, sprite->GetName());
			lua_pushboolean(L, member->GItemID != -1);			// 是否在線上
			return 2;
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetPartyLeaderIndex(lua_State* L)
{
	if ( g_pGameMain->IsBattleGroundZone() )
	{
		lua_pushnumber(L, NetCli_PartyChild::GetBattleLeaderPartyNo() + 1);
	}
	else
	{
		lua_pushnumber(L, NetCli_PartyChild::GetPartyLeaderIndex() + 1);
	}
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_IsPartyLeader(lua_State* L)
{
	if ( g_pGameMain->IsBattleGroundZone() )
	{
		lua_pushboolean(L, NetCli_PartyChild::s_BattleState == ePartyState_Leader);
	}
	else
	{
		lua_pushboolean(L, NetCli_PartyChild::s_state == ePartyState_Leader);
	}
	return 1;
}

// ----------------------------------------------------------------------------
// 取得隊伍隊員人數
int LuaFunc_GetNumPartyMembers(lua_State* L)
{
	int size = 0;
	if ( g_pGameMain->IsBattleGroundZone() )
	{
		size = (int)NetCli_PartyChild::s_BattleParty.size();
	}
	else
	{
		size = (int)NetCli_PartyChild::s_party.size();
	}

	if ( size > 0 )
		lua_pushnumber(L, size + 1);
	else
		lua_pushnumber(L, size);
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetRaidMember(lua_State* L)
{
	if ( g_pGameMain->IsBattleGroundZone() )
	{
		BattleGroundPartyMember* member = NetCli_PartyChild::GetBattleRaidMember( luaL_checkint(L, 1) - 1 );
		if ( member )
		{
			const char* name = "";

			CRoleSprite* sprite = g_pGameMain->FindBettleMemberByDBID(member->DBID);
			if ( sprite )
			{
				name = sprite->GetName();
			}

			lua_pushstring(L, name);
			lua_pushboolean(L, true);							// 是否在線上
			lua_pushboolean(L, false);							// 助理
			return 3;
		}
	}
	else
	{
		PartyMemberInfo* member = NetCli_PartyChild::GetRaidMember( luaL_checkint(L, 1) - 1 );
		if ( member )
		{
			CRoleSprite* sprite = (CRoleSprite*)(member->sprite);
			lua_pushstring(L, sprite->GetName());
			lua_pushboolean(L, member->GItemID != -1);			// 是否在線上
			lua_pushboolean(L, member->assistant);				// 助理
			return 3;
		}
	}		
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_SwitchToRaid(lua_State* L)
{
	if ( g_pGameMain->IsBattleGroundZone() )
	{
	}
	else
	{
		bool result = NetCli_PartyChild::SwitchToRaid();
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_SwithRaidAssistant(lua_State* L)
{
	if ( g_pGameMain->IsBattleGroundZone() )
	{
	}
	else
	{
		bool result = NetCli_PartyChild::SwitchRaidAssistant(luaL_checkint(L, 1) - 1, (lua_toboolean(L, 2) != 0));
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_SwithRaidMainTank	 (lua_State* L)
{
	if ( g_pGameMain->IsBattleGroundZone() )
	{
	}
	else
	{
		bool result = NetCli_PartyChild::SwitchRaidMainTank(luaL_checkint(L, 1) - 1, (lua_toboolean(L, 2) != 0));
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_SwithRaidMainAttacker(lua_State* L)
{
	if ( g_pGameMain->IsBattleGroundZone() )
	{
	}
	else
	{
		bool result = NetCli_PartyChild::SwitchRaidMainAttack(luaL_checkint(L, 1) - 1, (lua_toboolean(L, 2) != 0));
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_RaidDegrade(lua_State* L)
{
	if ( g_pGameMain->IsBattleGroundZone() )
	{
	}
	else
	{
		int index = luaL_checkint(L, 1) - 1; 

		// 	PartyMemberInfo* member = NetCli_PartyChild::GetRaidMember( index );
		//	if ( !member )
		//	{
		//		return 0;
		// 	}	

		NetCli_PartyChild::RaidDegrade( index );
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_MoveRaidMember(lua_State* L)
{
	if ( g_pGameMain->IsBattleGroundZone() )
	{
		NetCli_PartyChild::MoveBattleMember(luaL_checkint(L, 1) - 1, luaL_checkint(L, 2) - 1);
	}
	else
	{
		bool result = NetCli_PartyChild::MoveRaidMember(luaL_checkint(L, 1) - 1, luaL_checkint(L, 2) - 1);
	}
	return 0;
}

// ----------------------------------------------------------------------------
// 取得團隊隊員人數
int LuaFunc_GetNumRaidMembers(lua_State* L)
{
	int size = 0;
	if ( g_pGameMain->IsBattleGroundZone() )
	{
		size = (int)NetCli_PartyChild::s_BattleMember.size();
	}
	else
	{
		if( NetCli_PartyChild::s_info.Type == EM_PartyType_Raid )
		{
			size = (int)NetCli_PartyChild::s_member.size();
		}
	}

	lua_pushnumber(L, size);
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_InPartyByName(lua_State* L)
{
	lua_pushboolean(L, NetCli_PartyChild::InParty(luaL_checkstring(L, 1)));
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_InRaidByName(lua_State* L)
{
	lua_pushboolean(L, NetCli_PartyChild::InGroup(luaL_checkstring(L, 1)));
	return 1;
}

// ----------------------------------------------------------------------------
// 取得團隊隊長索引值
int LuaFunc_GetRaidLeaderIndex	(lua_State* L)
{
	if ( g_pGameMain->IsBattleGroundZone() )
	{
		lua_pushnumber(L, NetCli_PartyChild::GetBattleLeaderRaidNo() + 1);	
	}
	else
	{
		lua_pushnumber(L, NetCli_PartyChild::GetRaidLeaderIndex() + 1);	
	}
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_AcceptGroup(lua_State* L)
{
	NetCli_PartyChild::InviteAnswerState(true);
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_DeclineGroup(lua_State* L)
{
	NetCli_PartyChild::InviteAnswerState(false);
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_IsAssigner(lua_State* L)
{
	lua_pushboolean(L, NetCli_PartyChild::IsAssigner());
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_IsRaidLeader(lua_State* L)
{
	lua_pushboolean(L, NetCli_PartyChild::s_state == ePartyState_Leader);
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_IsRaidAssistant(lua_State* L)
{
	PartyMemberInfo* member = NetCli_PartyChild::GetGroupMemberByDBID( g_pGameMain->GetDBID() );
 	if ( member )
	{
		lua_pushboolean( L, member->assistant );			// 助理
		return 1;
	}	
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_IsRaidMainTank(lua_State* L)
{
	PartyMemberInfo* member = NetCli_PartyChild::GetGroupMemberByDBID( g_pGameMain->GetDBID() );
	if ( member )
	{
		lua_pushboolean( L, member->tank );					// 坦克
		return 1;
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_IsRaidMainAttack	(lua_State* L)
{
	PartyMemberInfo* member = NetCli_PartyChild::GetGroupMemberByDBID( g_pGameMain->GetDBID() );
	if ( member )
	{
		lua_pushboolean( L, member->attack );				// 攻擊手
		return 1;
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetLeaderChannelID	(lua_State* L)
{
	lua_pushnumber(L, NetCli_PartyChild::GetLeaderChannelID() + 1);
	return 1;	
}

// ----------------------------------------------------------------------------
int LuaFunc_AcceptBattleGroup(lua_State* L)
{
	NetCli_PartyChild::AcceptBattleGroup();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_DeclineBattleGroup(lua_State* L)
{
	NetCli_PartyChild::DeclineBattleGroup();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_ResetInstance(lua_State* L)
{
	return 0;
	g_pPartyFrame->SendResetInstance();	
}

// ----------------------------------------------------------------------------
int LuaFunc_SetInstanceLevel(lua_State* L)
{
	const char* level = luaL_checkstring(L, 1);
	if ( strcmp(level, "easy") == 0 )
	{
		if ( NetCli_PartyChild::GetPartyState() == ePartyState_Solo || NetCli_PartyChild::GetPartyState() == ePartyState_Receive )
		{
			NetCli_OtherChild::SL_SetRoleValue(EM_RoleValue_ClinetSetting, 0);
		}
		else
		{
			NetCli_PartyChild::SetInstanceLevel(0);
		}
	}
	else if ( strcmp(level, "normal") == 0 )
	{
		if ( NetCli_PartyChild::GetPartyState() == ePartyState_Solo || NetCli_PartyChild::GetPartyState() == ePartyState_Receive )
		{
			NetCli_OtherChild::SL_SetRoleValue(EM_RoleValue_ClinetSetting, 1);
		}
		else
		{
			NetCli_PartyChild::SetInstanceLevel(1);
		}		
	}
	else if ( strcmp(level, "hard") == 0 )
	{
		if ( NetCli_PartyChild::GetPartyState() == ePartyState_Solo || NetCli_PartyChild::GetPartyState() == ePartyState_Receive )
		{
			NetCli_OtherChild::SL_SetRoleValue(EM_RoleValue_ClinetSetting, 2);
		}
		else
		{
			NetCli_PartyChild::SetInstanceLevel(2);
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetInstanceLevel(lua_State* L)
{
	int InstanceLv = 0;
	if ( NetCli_PartyChild::GetPartyState() == ePartyState_Solo || NetCli_PartyChild::GetPartyState() == ePartyState_Receive )
	{
		InstanceLv = (int)CNetGlobal::RoleData()->PlayerBaseData->ClinetSetting[0];
	}
	else
	{
		InstanceLv = NetCli_PartyChild::GetPartyInfo().InstanceLv;
	}
	
	switch ( InstanceLv )
	{
	case 0:
		lua_pushstring(L, "easy");
		break;

	case 1:
		lua_pushstring(L, "normal");
		break;

	case 2:
		lua_pushstring(L, "hard");
		break;

	default:
		lua_pushstring(L, "unknow");
		break;
	}
	return 1;	
}