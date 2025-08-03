#include "NetCli_InstanceChild.h"
#include "../mainproc/GameMain.h"
#include "../../Interface/WorldFrames/ChatFrame.h"
#include "../../Interface/WorldFrames/PartyFrame.h"
#include "../../Interface/Map/MapCommon.h"
#include "../net_party/NetCli_PartyChild.h"
#include "../tools/Common/Com_Function.h"
#include "../../Interface/WorldFrames/WorldBattleGround.h"


//-----------------------------------------------------------------------------
bool NetCli_InstanceChild::_Init()
{
	if (This)
		return false;

	This = new NetCli_InstanceChild;
	return Net_Instance::_Init();
}

//-----------------------------------------------------------------------------
bool NetCli_InstanceChild::_Release()
{
	Net_Instance::_Release();

	if (This)
	{
		delete This;
		This = NULL;
	}
	return true;
}

//-----------------------------------------------------------------------------
// 通知玩家進入副本重置時間
void NetCli_InstanceChild::R_ResetTime( int Time , int InstanceSaveID , int ValueKeyID , bool IsLogin )
{
	if ( InstanceSaveID >= 0 && InstanceSaveID < _MAX_INSTANCE_SETTING_COUNT_ )
	{
		char temp[512];
		int remainTime = Time - (int)g_pGameMain->GetGameTime();

		// 取得副本名稱
		string instanceName;
		sprintf_s(temp, 512, "InstanceKey%02d", InstanceSaveID);
		int zoneId = g_ObjectData->GetSysKeyValue(temp);
		instanceName = Func_GetLocalMapName( zoneId );

		// 取得副本剩於重置時間
		string resetTime;		
		int day, hour, minute;
		bool showAll = false;
		day = remainTime / 86400;
		hour = (remainTime % 86400) / 3600;
		minute = ((remainTime % 86400) % 3600) / 60;

		if ( IsLogin )
		{
			if ( day > 0 )
			{
				sprintf_s(temp, 512, "%d %s ", day, g_ObjectData->GetString("SYS_DAY"));
				resetTime += temp;
				showAll = true;
			}
			if ( hour > 0 || showAll )
			{
				sprintf_s(temp, 512, "%d %s ", hour, g_ObjectData->GetString("SYS_HOUR"));
				resetTime += temp;
				showAll = true;
			}
			if ( minute > 0 || showAll )
			{
				sprintf_s(temp, 512, "%d %s", minute, g_ObjectData->GetString("SYS_MINUTE"));
				resetTime += temp;
				showAll = true;
			}

			if ( remainTime > 0 )
			{
				g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_WELCOME_INSTANCE"), instanceName.c_str(), resetTime.c_str());
			}
		}
		else
		{
			string msg = ReplaceKeyString(g_ObjectData->GetString("SYS_INSTANCE_SAVE"), "<INSTANCENAME>", instanceName);
			g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", msg.c_str());

			CNetGlobal::RoleData()->PlayerBaseData->InstanceSetting.KeyID[InstanceSaveID] = ValueKeyID;
			CNetGlobal::RoleData()->PlayerBaseData->InstanceSetting.LiveTime[InstanceSaveID] = Time;

			g_pChatFrame->SendWorldEvent("INSTANCE_RESET");
		}
	}


	//g_pChatFrame->SendChatMessageEx()
}

void NetCli_InstanceChild::R_JoinInstanceRequest( )
{
	g_pGameMain->RequestInstanceRecord();
}

// 副本重置
void NetCli_InstanceChild::R_InstanceReset( int InstanceKey )
{
	if ( InstanceKey >= 0 && InstanceKey < _MAX_INSTANCE_SETTING_COUNT_ )
	{
		// 取得副本名稱
		char temp[512];
		string instanceName;
		sprintf_s(temp, 512, "InstanceKey%02d", InstanceKey);
		int zoneId = g_ObjectData->GetSysKeyValue(temp);
		instanceName = Func_GetLocalMapName( zoneId );
		
		string msg = ReplaceKeyString(g_ObjectData->GetString("SYS_INSTANCE_REST"), "<INSTANCENAME>", instanceName);
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", msg.c_str());

		// 
		// CNetGlobal::RoleData()->PlayerBaseData->InstanceSetting.KeyID[InstanceKey] = -1;
	}

	g_pChatFrame->SendWorldEvent("INSTANCE_RESET");
}

// 已經有人進入副本通知其他隊員
void NetCli_InstanceChild::R_InstanceCreate( int PlayerDBID , int ZoneID , int Key )
{
	PartyMemberInfo* info = NetCli_PartyChild::GetGroupMemberByDBID(PlayerDBID);
	if ( info )
	{
		CRoleSprite* sprite = (CRoleSprite*)info->sprite;
		string instanceName = Func_GetLocalMapName( ZoneID );

		string msg;
		if ( Key >= 0 )
		{
			msg = ReplaceKeyString(g_ObjectData->GetString("SYS_INSTANCE_INSIDE"), "<PLAYERNAME>", sprite->GetName());
			msg = ReplaceKeyString(msg, "<INSTANCENAME>", instanceName);
			msg = ReplaceKeyString(msg, "<INSTANCEID>", Key);
		}
		else
		{
			msg = ReplaceKeyString(g_ObjectData->GetString("SYS_INSTANCE_INSIDE01"), "<PLAYERNAME>", sprite->GetName());
			msg = ReplaceKeyString(msg, "<INSTANCENAME>", instanceName);
		}

		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", msg.c_str());
	}	
}

void NetCli_InstanceChild::R_FixInstanceExtend( int ZoneID , int ExtendTime , int LiveTime , int KeyID )
{
	g_pPartyFrame->RL_FixInstanceExtend(ZoneID, ExtendTime, LiveTime, KeyID);
}

// 取得副本時間
void NetCli_InstanceChild::R_InstanceRestTime( int ZoneID , int ExtendTime , int LiveTime , int KeyID )
{
	g_pPartyFrame->RL_InstanceRestTime(ZoneID, ExtendTime, LiveTime, KeyID);
}

void NetCli_InstanceChild::R_GetWorldVarResult( const char* pszVarName, int iVarValue )
{
	g_pWorldBattleGround->R_GetWorldVarResult(pszVarName, iVarValue);
}