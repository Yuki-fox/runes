#include "../mainproc/GameMain.h"
#include "NetCli_ChannelChild.h"
#include "NetCli_ChannelChild.h"
#include "../Net_FriendList/NetCli_FriendList.h"
#include "../Net_Script/NetCli_Script_Child.h"
#include "../Interface/WorldFrames/NpcTrackFrame.h"
#include "../Interface/WorldFrames/ChatFrame.h"
#include "../Interface/WorldFrames/LootFrame.h"
#include "../Interface/Map/Worldmap.h"
#include "../Interface/Socal/UI_Socal.h"
#include "../Interface/WorldFrames/RaidTargetFrame.h"
#include "../Interface/WorldFrames/GameControl.h"
#include "../../Interface/Debug/DebugMessage.h"
#include "../../tools/Common/Com_Function.h"

#include "../Interface/WorldFrames/GuildFrame.h"


NetCli_ChannelChild* g_pNetCli_ChannelChild = NULL;

map<string, int>			NetCli_ChannelChild::s_channel;
map<string, int>			NetCli_ChannelChild::s_zoneChannel;
map<int, string>			NetCli_ChannelChild::s_channelLeader;

string NetCli_ChannelChild::s_AskPlayerInfo_Name;
int NetCli_ChannelChild::s_AskPlayerInfo_LastTime;

//-----------------------------------------------------------------------------
bool NetCli_ChannelChild::_Init()
{
	if (This)
		return false;
	s_channel.clear();
	s_zoneChannel.clear();
	s_channelLeader.clear();

	s_AskPlayerInfo_Name = "-1";
	s_AskPlayerInfo_LastTime = -1;

	g_pNetCli_ChannelChild = new NetCli_ChannelChild;

	This = g_pNetCli_ChannelChild;

	return NetCli_Channel::_Init();

}

//-----------------------------------------------------------------------------
bool NetCli_ChannelChild::_Release()
{
	NetCli_Channel::_Release();
	if (This)
	{
		delete This;
		This = NULL;
	}

	s_channel.clear();
	s_zoneChannel.clear();
	s_channelLeader.clear();

	g_pNetCli_ChannelChild = NULL;
	return true;
}
//-----------------------------------------------------------------------------
// arg1 頻道名稱
// return 傳回頻道代碼,　若失敗則傳回 -1
int	NetCli_ChannelChild::GetChannelID(const char* name)
{
	map<string, int>::iterator iter = s_channel.find(name);
	if ( iter != s_channel.end() )
		return iter->second;
	return -1;
}
//-----------------------------------------------------------------------------
const char* NetCli_ChannelChild::GetChannelNameByID(int id)
{
	//std::string ret = ""; // GKy 100419 add for solving crash, obsolete
	for ( map<string, int>::iterator iter = s_channel.begin(); iter != s_channel.end(); iter++ )
	{
		if ( iter->second == id )
			return (iter->first).c_str();
	}
	return NULL; // GKy 100419 need keep for existed referecne.
	//return ret.c_str(); // GKy 100419 obsolete.
}
//-----------------------------------------------------------------------------
int NetCli_ChannelChild::GetZoneChannelID(const char* name)
{
	map<string, int>::iterator iter = s_zoneChannel.find(name);
	if ( iter != s_zoneChannel.end() )
		return iter->second;
	return -1;
}
//-----------------------------------------------------------------------------
int	NetCli_ChannelChild::ChannelKick(const char* channelName, const char* playerName)
{
	int channel = GetChannelID(channelName);	
	if ( channel == -1 )
		return -1;				// 頻道不存在

	map<int, string>::iterator iter = s_channelLeader.find(channel);
	if ( iter == s_channelLeader.end() )
		return -1;				// 頻道不存在

	// 檢查是否為頻道管理者
	if ( strcmp((iter->second).c_str(), _RoleData.BaseData.RoleName.Begin()) != 0 )
		return -2;	
	
	S_KickPlayer(EM_GroupObjectType_PlayerChannel, channel, (char*)playerName);
	return 0;
}
//-----------------------------------------------------------------------------
int	NetCli_ChannelChild::SetChannelOwner(const char* channelName, const char* playerName)
{
	int channel = GetChannelID(channelName);	
	if ( channel == -1 )
		return -1;				// 頻道不存在

	map<int, string>::iterator iter = s_channelLeader.find(channel);
	if ( iter == s_channelLeader.end() )
		return -1;				// 頻道不存在

	// 檢查是否為頻道管理者
	if ( strcmp((iter->second).c_str(), _RoleData.BaseData.RoleName.Begin()) != 0 )
		return -2;	

	// 自己已經是管理者
	if ( strcmp(playerName, _RoleData.BaseData.RoleName.Begin()) == 0 )
		return -3;

	S_SetManager(EM_GroupObjectType_PlayerChannel, channel, (char*)playerName);
	return 0;
}
//-----------------------------------------------------------------------------
void NetCli_ChannelChild::R_CliConnectToChat( int ChatSockID )
{
	SetSrvID_Chat( ChatSockID );
}
//-----------------------------------------------------------------------------
void NetCli_ChannelChild::R_ClitoCli_Data( const char* Data , int Size )
{
	ClitoCliPacketBaseStruct* PG = (ClitoCliPacketBaseStruct*)Data;

	if( PG->Command >= (int)_ClitoCliPacketFunctionList.size() )
	{
		TRACE2( "PG->Command >= _ClitoCliPacketFunctionList.size() PG->Command=%dSize=%d" , PG->Command , Size );
		return;
	}
	
	ClitoCliCallBackStruct& Func = _ClitoCliPacketFunctionList[ PG->Command ];

	if( Func.Fun == NULL )
	{
		TRACE2( "Func.Fun == NULL  PG->Command=%dSize=%d" , PG->Command , Size );
		return;
	}

	if( Func.Min > Size || Func.Max < Size )
	{
		TRACE2( " Func.Min > Size || Func.Max < Size  PG->Command=%d  Size=%d" , PG->Command , Size );
		return;
	}

	Func.Fun( (void*)Data , Size );

}
//-----------------------------------------------------------------------------
void NetCli_ChannelChild::R_CreateChannelOK( int ChannelID , const char* Name )
{
	/*
	const char* keyStr = g_ObjectData->GetString("SYS_CREATE_CHANNEL_SUCCESS");
	if ( keyStr[0] > 0 ) {
		int channel = g_pChatFrame->GetChannelID(Name);
		g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", keyStr, channel, Name);
	}
	*/

	g_pChatFrame->CreateChannelOK(Name,ChannelID);

	g_pDebugMessage->DebugOutput(0, "Create Channel %s(%d)", Name, ChannelID);
	
}
//-----------------------------------------------------------------------------
void NetCli_ChannelChild::R_CreateChannelFailed( const char* Name )
{
	const char* keyStr = g_ObjectData->GetString("SYS_CREATE_CHANNEL_FAILED");
	if ( keyStr[0] > 0 ) {
		g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", keyStr, Name);
	}
	
	g_pDebugMessage->DebugOutput(0, "Create Channel Failed!! %s" , Name);	
	g_pChatFrame->LeaveChannelByName(Name, true);
}
//-----------------------------------------------------------------------------
void NetCli_ChannelChild::R_JoinChannelOK( int ChannelID , const char* Name , int ChannelOwnerDBID , const char* ChannelOwnerName )
{
	g_pDebugMessage->DebugOutput(0, "Join Channel %s(%d)", Name ,ChannelID);

	/*
	const char* keyStr = g_ObjectData->GetString("SYS_JOIN_CHANNEL_SUCCESS");
	if ( keyStr[0] > 0 ) {
		int channel = g_pChatFrame->GetChannelID(Name);
		g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", keyStr, channel, Name);
	}
	*/
	
	// 加入頻道
	s_channel.insert( make_pair(Name, ChannelID) );	

	// 頻道管理者
	s_channelLeader[ChannelID] = ChannelOwnerName;
	// 回應頻道加入
	g_pChatFrame->ReceiveJoinChannel( Name );

	// 顯示自己加入頻道
	RChat_AddUserNotify(EM_GroupObjectType_PlayerChannel, ChannelID, (char*)(_RoleData.BaseData.RoleName.Begin()));

	// 顯示管理者名稱
	RChat_ChannelOwner(EM_GroupObjectType_PlayerChannel, ChannelID, ChannelOwnerDBID, (char*)ChannelOwnerName);		
}
//-----------------------------------------------------------------------------
void NetCli_ChannelChild::R_JoinChannelFailed( const char* Name )
{
	const char* keyStr = g_ObjectData->GetString("SYS_JOIN_CHANNEL_FAILED");
	if ( keyStr[0] > 0 ) {
		g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", keyStr, Name);
	}

	g_pDebugMessage->DebugOutput(0, "Join Channel Failed!! %s", Name);
	g_pChatFrame->LeaveChannelByName(Name, true);
}

void NetCli_ChannelChild::removeChannel( int ChannelID)
{

	for ( map<string, int>::iterator iter = s_channel.begin(); iter != s_channel.end(); iter++ )
	{
		if ( iter->second == ChannelID )
		{
			g_pDebugMessage->DebugOutput(0, "Leave Channel %d.%s", iter->second, iter->first.c_str());			

			string channelName = iter->first;		// 暫時記錄頻道名稱			

		

			// 通知界面做更新(移除頻道)
			g_pChatFrame->LeaveChannelByName(channelName.c_str(), true);

			s_channel.erase(iter);
			s_channelLeader.erase(ChannelID);
			return;
		}
	}

	g_pDebugMessage->DebugOutput(0, "[ERROR!!]Leave Channel %d, Not Exist", ChannelID);
}
//-----------------------------------------------------------------------------
void NetCli_ChannelChild::R_LeaveChannelOK( int ChannelID )
{
	// 顯示自己離開頻道
	RChat_DelUserNotify(EM_GroupObjectType_PlayerChannel, ChannelID, (char*)(_RoleData.BaseData.RoleName.Begin()));			

	removeChannel(ChannelID);


	
}
//-----------------------------------------------------------------------------
void NetCli_ChannelChild::R_LeaveChannelFailed( int ChannelID )
{
	g_pDebugMessage->DebugOutput(0, "Leave %d Failed", ChannelID);
}
//-----------------------------------------------------------------------------
void NetCli_ChannelChild::R_ChannelPlayerListResult	( GroupObjectTypeEnum Type , int ChannelID , vector<string> &Name )
{
	g_pChatFrame->R_ChannelPlayerListResult( Type , ChannelID , Name );
}

//-----------------------------------------------------------------------------
void NetCli_ChannelChild::R_JoinPlayerDisabledResult ( int ChannelID , GroupObjectTypeEnum Type , bool JoinEnabled )
{
	char Buf[1024];
	if( JoinEnabled )
		sprintf( Buf , "Public'sChannel %d" , ChannelID  );
	else
		sprintf( Buf , "Private's Channel %d" , ChannelID  );
	g_pChatFrame->AddMessage( 5, Buf );
}
//-----------------------------------------------------------------------------
//系統頻道列表
void NetCli_ChannelChild::R_SysChannel( GroupObjectTypeEnum Type , int ChannelID , char* Name )
{
	char Buf[1024];
	switch( Type )
	{
	case EM_GroupObjectType_SysChannel:
		sprintf( Buf , "System Channel ID = %d Name = %s" , ChannelID , Name );
		break;
	case EM_GroupObjectType_SysZoneChannel:
		sprintf( Buf , "Zone Channel ID = %d Name = %s" , ChannelID , Name );
		s_zoneChannel.insert( make_pair(Name, ChannelID) );
		break;
	}
	// g_pChatFrame->AddMessage( 5, Buf );
}
//-----------------------------------------------------------------------------
void NetCli_ChannelChild::R_SysTime( int Time , int TimeZone )
{
	g_pGameMain->SetGameTime(Time,TimeZone);
}

//-----------------------------------------------------------------------------
void NetCli_ChannelChild::RChat_PlayerNotFind( const char* Name )
{
	if( s_AskPlayerInfo_Name == Name )
	{
		g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString( "SYS_ASK_PLAYER_INFO_NOT_FIND" ) );
	 	s_AskPlayerInfo_LastTime = -1;
		s_AskPlayerInfo_Name = "-1";
	}
}

//-----------------------------------------------------------------------------
void NetCli_ChannelChild::R_CtoC_Test( char* TestData )
{
	g_pChatFrame->AddMessage( 5, TestData );
}
//-------------------------------------------------------------------
//Cli to Cli轉送點擊地圖的訊息
void NetCli_ChannelChild::R_CtoC_MapPing( const char* pPingerName, int ZoneID, float fX, float fZ )
{   
	g_pWorldMapFrame->R_CtoC_MapPing( pPingerName , ZoneID , fX , fZ );
}

//-----------------------------------------------------------------------------------------------------
//Cli to Cli 要求取得ModleInfo
void NetCli_ChannelChild::GetModleInfoRequest( const char* pTargetName )
{
	PG_CtoC_GetModleInfoRequest Send;
	Send.RequesterName = g_pGameMain->GetPlayerName();

	S_DataTransmitToCli_Name( (char*)pTargetName , &Send , sizeof(Send) );
}

void NetCli_ChannelChild::R_CtoC_GetModleInfoRequest( const char* pRequesterName )
{
	PG_CtoC_GetModleInfoResult Send;
	if( g_pUi_Socal->GetSelfModelInfo( Send.ModelInfo ) )
	{
		//Send.Name = g_pGameMain->GetPlayerName();
		Send.DBID = g_pGameMain->GetDBID();
		S_DataTransmitToCli_Name( (char*)pRequesterName , &Send , sizeof(Send) );
	}
}

//-----------------------------------------------------------------------------------------------------
//Cli to Cli 傳回取得ModleInfo結果
void NetCli_ChannelChild::R_CtoC_GetModleInfoResult( int DBID , SModelInfo& ModelInfo )
{
	g_pUi_Socal->R_CtoC_GetModleInfoResult( DBID , ModelInfo );
}

//-----------------------------------------------------------------------------------------------------
//Cli to Cli 要求取得好友資訊
void NetCli_ChannelChild::GiveMeYourFriendInfo( const char* pTargetName )
{
	PG_CtoC_GiveMeYourFriendInfo Send;
	Send.SenderName = g_pGameMain->GetPlayerName();
	S_DataTransmitToCli_Name( (char*)pTargetName , &Send , sizeof(Send) );
}

void NetCli_ChannelChild::R_CtoC_GiveMeYourFriendInfo( const char* SenderName )
{
	//SModelInfo ModelInfo;
	//if( !g_pUi_Socal->GetSelfModelInfo( ModelInfo ) )
	//{
	//	return ;
	//}

	RoleDataEx*	pRole = CNetGlobal::RoleData();

	PG_CtoC_GiveYouMyFriendInfo Send;
	
	Send.Name                     = g_pGameMain->GetPlayerName();
	Send.FriendInfo.MapID         = g_pGameMain->GetWorldMapID();
	Send.FriendInfo.ZoneID        = g_pGameMain->GetZoneID();
	Send.FriendInfo.TitleID       = pRole->BaseData.TitleID;
	Send.FriendInfo.TitleStr      = pRole->PlayerBaseData->TitleStr;
	Send.FriendInfo.GuildID       = pRole->GuildID();
	Send.FriendInfo.Voc           = pRole->BaseData.Voc;
	Send.FriendInfo.VocLV         = pRole->Level();
	Send.FriendInfo.Voc_Sub       = pRole->BaseData.Voc_Sub;
	Send.FriendInfo.Voc_SubLV     = pRole->Level_Sub();
	Send.FriendInfo.Sex           = pRole->BaseData.Sex;
	//Send.FriendInfo.ModelInfo     = ModelInfo;
  	Send.FriendInfo.Introduection = pRole->PlayerSelfData->FriendList.Note.Begin();

	
 	S_DataTransmitToCli_Name( (char*)SenderName , &Send , sizeof(Send) );
}

//-----------------------------------------------------------------------------------------------------
//Cli to Cli 回報好友資訊
void NetCli_ChannelChild::R_CtoC_GiveYouMyFriendInfo( const char* pName, SFriendInfo& FriendInfo )
{
	g_pUi_Socal->R_CtoC_GiveYouMyFriendInfo( pName , FriendInfo );
}

//-----------------------------------------------------------------------------------------------------
//Cli to Cli 詢問是否是對方的朋友
void NetCli_ChannelChild::AmIYourFriend( FriendListTypeENUM FriendListType, const char* AskerName, const char* AnswererName )
{
	PG_CtoC_AmIYourFriend Send;
	Send.AskerName = AskerName;
	Send.AnswererName = AnswererName;
 	Send.FriendListType = FriendListType;
	S_DataTransmitToCli_Name( (char*)AnswererName , &Send , sizeof(Send) );
}

void NetCli_ChannelChild::R_CtoC_AmIYourFriend( FriendListTypeENUM FriendListType, const char* AskerName, const char* AnswererName )
{
	if( !g_pUi_Socal->IsFriendListLoadOK() )
	{
		//名單還沒收到 不回應
		return;
	}

	bool bResult = g_pUi_Socal->IsMyFriend( AskerName, FriendListType );

  	PG_CtoC_AmIYourFriendResult Send;
	Send.AskerName = AskerName;
	Send.AnswererName = AnswererName;
	Send.Result = bResult;
	Send.FriendListType = FriendListType;
	S_DataTransmitToCli_Name( (char*)AskerName , &Send , sizeof(Send) );
}

//-----------------------------------------------------------------------------------------------------
//Cli to Cli 詢問是否是對方的朋友結果
void NetCli_ChannelChild::R_CtoC_AmIYourFriendResult( FriendListTypeENUM FriendListType, const char* AskerName, const char* AnswererName, bool Result )
{
	g_pUi_Socal->R_CtoC_AmIYourFriendResult( FriendListType, AskerName, AnswererName, Result );
}

//-----------------------------------------------------------------------------------------------------
//Cli to Cli 通知對方被新增或刪除朋友
void NetCli_ChannelChild::YouAreMyFriend( FriendListTypeENUM FriendListType, const char* Name, bool bYes )
{
	PG_CtoC_AmIYourFriendResult Send;
	Send.AskerName = Name ;
	Send.AnswererName = g_pGameMain->GetPlayerName();
	Send.FriendListType = FriendListType;
	Send.Result = bYes;
 	S_DataTransmitToCli_Name( (char*)Name , &Send , sizeof(Send) );
}
//-----------------------------------------------------------------------------------------------------
void NetCli_ChannelChild::ReceiveToCliAskLeaderChangeResult(int dbid,bool YesNo)
{
		if ( g_pGuildFrame )
	g_pGuildFrame->AskLeaderChangeResult(dbid,YesNo);
}
//-----------------------------------------------------------------------------------------------------
void NetCli_ChannelChild::ReceiveToCliAskLeaderChange(const char* SendName)
{
	if ( g_pGuildFrame )
		g_pGuildFrame->AskLeaderChange(SendName);
}
//-----------------------------------------------------------------------------------------------------
void NetCli_ChannelChild::ReceiveToCliAskHasItem(const char* SendName, int itemVersion, int itemID)
{
	if ( RoleData() )
	{
		PG_CtoC_HasItemResult Send;
		Send.hasItem = RoleData()->CheckItemInBody(itemID);
		Send.itemID = itemID;
		Send.itemVersion = itemVersion;
		S_DataTransmitToCli_Name( (char*)SendName , &Send , sizeof(Send) );
	}	
}

//-------------------------------------------------------------------
//Cli to Cli 轉送玩家是否有特定物品
void NetCli_ChannelChild::ReceiveToCliHasItem(int itemVersion, int itemID, bool hasItem)
{
	g_pLootFrame->ReceiveUniqueAssugnItem(itemVersion, itemID, hasItem);
	//g_pLootFrame->ReceiveHasItem(int itemID, hasItem);
}

//-----------------------------------------------------------------------------------------------------
//查詢玩家資訊
void NetCli_ChannelChild::AskPlayerInfo( const char* targetName )
{
	if( timeGetTime() < DWORD(s_AskPlayerInfo_LastTime + 3000)  )
	{
		return;
	}

	s_AskPlayerInfo_LastTime = timeGetTime();
	s_AskPlayerInfo_Name = targetName;

	PG_CtoC_AskPlayerInfo Send;
	Send.SenderName = g_pGameMain->GetPlayerName();
	S_DataTransmitToCli_Name( (char*)targetName , &Send , sizeof(Send) );
}

//-----------------------------------------------------------------------------------------------------
//Cli to Cli 查詢玩家資訊
void NetCli_ChannelChild::R_CtoC_AskPlayerInfo( const char* AskerName )
{	
	RoleDataEx*	pRole = CNetGlobal::RoleData();

	if ( pRole == NULL )
		return;

	PG_CtoC_AskPlayerInfoResult Send;

	Send.PlayerInfo.Name      = g_pGameMain->GetPlayerName();
	Send.PlayerInfo.MapID     = g_pGameMain->GetWorldMapID();
	Send.PlayerInfo.GuildID   = pRole->GuildID();
	Send.PlayerInfo.Voc       = pRole->BaseData.Voc;
	Send.PlayerInfo.VocLV     = pRole->Level();
	Send.PlayerInfo.Voc_Sub   = pRole->BaseData.Voc_Sub;
	Send.PlayerInfo.Voc_SubLV = pRole->Level_Sub();
	Send.PlayerInfo.Sex       = pRole->BaseData.Sex;
  	Send.PlayerInfo.Race      = pRole->BaseData.Race;

	S_DataTransmitToCli_Name( (char*)AskerName , &Send , sizeof(Send) );
}

//-----------------------------------------------------------------------------------------------------
//Cli to Cli 查詢玩家資訊結果
void NetCli_ChannelChild::R_CtoC_AskPlayerInfoResult( SAskPlayerInfo& PlayerInfo )
{
	string temp;

	s_AskPlayerInfo_LastTime = -1;
	s_AskPlayerInfo_Name = "-1";

	// 查詢GM顯示目標不存在
	if ( PlayerInfo.Voc < EM_Vocation_Warrior )
	{
		g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString( "SYS_ASK_PLAYER_INFO_NOT_FIND" ) );
		return;
	}	

	//SYS_ASK_PLAYER_INFO_FORMAT <player><guild><voc><subvoc><map><race><sex>
	string msg = g_ObjectData->GetString( "SYS_ASK_PLAYER_INFO_FORMAT" );

	msg = ReplaceKeyString( msg, "<player>", PlayerInfo.Name.Begin() );

	const char* szGuildName = g_pGuildFrame->GetGuildNameByID( PlayerInfo.GuildID );
	string GuildName;
	
	if( szGuildName )
		GuildName = szGuildName;

	if( GuildName.length() > 0 )
	{
		temp = g_ObjectData->GetString( "SYS_ASK_PLAYER_INFO_GUILD_FORMAT" );
		temp = ReplaceKeyString( temp, "<guild>", GuildName );
		msg = ReplaceKeyString( msg, "<guild>", temp );
	}
	else
	{
		msg = ReplaceKeyString( msg, "<guild>", "" );
	}
	
	string Voc = g_ObjectData->GetClassNameByID( PlayerInfo.Voc );
	if( Voc.length() > 0 )
	{
		temp = g_ObjectData->GetString( "SYS_ASK_PLAYER_INFO_VOC_FORMAT" );
		temp = ReplaceKeyString( temp, "<voc>", Voc );
		temp = ReplaceKeyString( temp, "<lv>", PlayerInfo.VocLV );
 		msg = ReplaceKeyString( msg, "<voc>", temp );
	}
	else
	{
 		msg = ReplaceKeyString( msg, "<voc>", "" );
	}

	string Voc_Sub = g_ObjectData->GetClassNameByID( PlayerInfo.Voc_Sub );
	if( Voc_Sub.length() > 0 )
	{
		temp = g_ObjectData->GetString( "SYS_ASK_PLAYER_INFO_VOC_FORMAT" );
		temp = ReplaceKeyString( temp, "<voc>", Voc_Sub );
		temp = ReplaceKeyString( temp, "<lv>", PlayerInfo.Voc_SubLV );
		msg = ReplaceKeyString( msg, "<subvoc>", temp );
	}
	else
	{
		msg = ReplaceKeyString( msg, "<subvoc>", "" );
	}
 	
	const WorldMapInfoStruct* pWorldMapInfo = g_ObjectData->GetWorldMapInfoByID( PlayerInfo.MapID );
	if( pWorldMapInfo )
	{
 		msg = ReplaceKeyString( msg, "<map>", Func_GetLocalMapName( pWorldMapInfo->MapName ) );
	}
	else
	{
		msg = ReplaceKeyString( msg, "<map>", "" );
	}

	msg = ReplaceKeyString( msg, "<race>", g_ObjectData->GetString( Com_FormatString( "SYS_RACE_%d", PlayerInfo.Race ) ) );

	if ( PlayerInfo.Sex == EM_Sex_Boy )
	{
		msg = ReplaceKeyString( msg, "<sex>", g_ObjectData->GetString("SYS_SEX_BOY") );
	}
	else// if (PlayerInfo.Sex == EM_Sex_Girl )
	{
		msg = ReplaceKeyString( msg, "<sex>", g_ObjectData->GetString("SYS_SEX_GIRL") );
	}

	g_pGameMain->SendSystemChatMsg( msg.c_str() );

}

//-----------------------------------------------------------------------------------------------------
void NetCli_ChannelChild::ReceiveToCliMessage(int messageType, const char* content)
{
	string replaceString;
	switch(messageType)
	{	
	case 0:				// 一般顏色
		replaceString = CNetCli_Script_Child::ReplaceSystemKeyword(content);
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", replaceString.c_str());
		break;
	}
}

//-----------------------------------------------------------------------------------------------------
// Cli to Cli 設定團隊目標
void NetCli_ChannelChild::R_CtoC_SetRaidTarget( int TargetID , int IconID )
{
	g_pRaidTargetFrame->R_SetRaidTarget( TargetID, IconID );
}

//-----------------------------------------------------------------------------------------------------
// Cli to Cli 追蹤玩家
void NetCli_ChannelChild::R_CtoC_TrackPlayer( const char* SenderName )
{

	CRoleSprite* pPlayer = g_pGameMain->GetPlayer();

	if( !pPlayer )
		return;

	// 計算座標
	CRuVector3 pos = pPlayer->GetPos();
	
	//int MapID = g_pGameMain->GetWorldMapID(); 
	
	//const WorldMapInfoStruct* pWorldMapInfoPlayer = g_ObjectData->GetWorldMapInfoByID( MapID );

	//SWorldCoordinate WorldCoord;
	//SMapCoordinate MapCoord;

	//WorldCoord.wdbFile = pWorldMapInfoPlayer->wdbFile;
	//WorldCoord.PosX = pos.x;
	//WorldCoord.PosZ = pos.z;

	//MapCoord.MapID = MapID;

	//Func_TransformCoordinate_WorldToMap( WorldCoord, MapCoord );
	
	PG_CtoC_TrackPlayerResult Send;

	Send.TargetName = g_pGameMain->GetPlayerName();	
	Send.WorldMapID = g_pGameMain->GetWorldMapID();
  
	Send.fX = pos.x;
	Send.fY = pos.y;
	Send.fZ = pos.z;

	S_DataTransmitToCli_Name( (char*)SenderName , &Send , sizeof(Send) );
}

//-----------------------------------------------------------------------------------------------------
// Cli to Cli追蹤玩家 回報玩家座標
void NetCli_ChannelChild::R_CtoC_TrackPlayerResult( const char* Name, int WorldMapID, float fX, float fY, float fZ )
{
//	const WorldMapInfoStruct* pWorldMapInfoPlayer = g_ObjectData->GetWorldMapInfoByID( WorldMapID );
	g_pNpcTrackFrame->R_CtoC_TrackPlayerResult( Name, WorldMapID, fX, fY, fZ  );
}

//-----------------------------------------------------------------------------------------------------
// 收到訊問共乘
void NetCli_ChannelChild::R_CtoC_AskRideMount(const char* Name, int GItemID)
{
	g_pGameControl->R_InviteMount(Name, GItemID);
}

//-----------------------------------------------------------------------------------------------------
// Cli to Cli 收到回應塔乘坐騎
void NetCli_ChannelChild::R_CtoC_RideMountResult(const char* Name, int answer)
{
	switch (answer)
	{
	case 1:
		g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_MOUNT_TARGET_DECLINE"), Name);
		break;

	case 2:
		g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_MOUNT_TARGET_BUSY"), Name);
		break;
	}
}


//-----------------------------------------------------------------------------------------------------
//邀請加入頻道
void NetCli_ChannelChild::ChannelInvite( const char* targetName, const char* channelName, const char* password )
{
	PG_CtoC_ChannelInvite Send;
	Send.SenderName = g_pGameMain->GetPlayerName();
	Send.ChannelName = channelName; 
	Send.ChannelPass = password;
	S_DataTransmitToCli_Name( (char*)targetName , &Send , sizeof(Send) );
}

//-----------------------------------------------------------------------------------------------------
// Cli to Cli 邀請加入頻道
void NetCli_ChannelChild::R_CtoC_ChannelInvite( const char* AskerName, const char* channelName, const char* password )
{
	g_pChatFrame->R_CtoC_ChannelInvite( AskerName, channelName, password );	
}

//-----------------------------------------------------------------------------------------------------
// 加入頻道
void NetCli_ChannelChild::RChat_AddUserNotify( GroupObjectTypeEnum Type , int ChannelID , char* Name )
{
	g_pDebugMessage->DebugOutput(0, "RChat_AddUserNotify Type=%d ChannelID=%d Name=%s", Type , ChannelID , Name );

	switch (Type)
	{
	case EM_GroupObjectType_SysChannel:			//系統
		break;

	case EM_GroupObjectType_SysZoneChannel:		//每個區域都有的
		break;

	case EM_GroupObjectType_PlayerChannel:		//玩家的
		{
			// 搜尋頻道是否存在
			const char* channelName = GetChannelNameByID(ChannelID);
			if ( channelName != NULL )
			{
				char content[512];
				char channelType[512];
				sprintf_s(channelType, 512, "%s%s", CHAT_MSG_CHANNEL, channelName);
				sprintf_s(content, 512, "%s %s", Name, g_ObjectData->GetString("SYS_JOIN_CHANNEL"));
				g_pChatFrame->SendChatMessage(channelType, "", content);
				g_pChatFrame->AddChannelMember(channelName, ChannelID,Name);
			}
		}
		break;
	case EM_GroupObjectType_Party:				//隊伍
		break;
	case EM_GroupObjectType_Guild:				//工會
		break;
	}
}

//-----------------------------------------------------------------------------------------------------
// 刪除頻道
void NetCli_ChannelChild::RChat_DelUserNotify( GroupObjectTypeEnum Type , int ChannelID , char* Name )
{
	g_pDebugMessage->DebugOutput(0, "RChat_DelUserNotify Type=%d ChannelID=%d Name=%s", Type , ChannelID , Name );

	switch (Type)
	{
	case EM_GroupObjectType_SysChannel:			//系統
		break;

	case EM_GroupObjectType_SysZoneChannel:		//每個區域都有的
		break;

	case EM_GroupObjectType_PlayerChannel:		//玩家的
		{
			// 搜尋頻道是否存在
			const char* channelName = GetChannelNameByID(ChannelID);
			if ( channelName != NULL )
			{
				char content[512];
				char channelType[512];
				sprintf_s(channelType, 512, "%s%s", CHAT_MSG_CHANNEL, channelName);
				sprintf_s(content, 512, "%s %s", Name, g_ObjectData->GetString("SYS_LEAVE_CHANNEL"));
				g_pChatFrame->SendChatMessage(channelType, "", content);
				g_pChatFrame->DelChannelMember(channelName,ChannelID,Name);
			}
		}
		break;
	case EM_GroupObjectType_Party:				//隊伍
		break;
	case EM_GroupObjectType_Guild:				//工會
		break;
	}
}

//-----------------------------------------------------------------------------------------------------
// 設定頻道權限回應
void NetCli_ChannelChild::RChat_SetManagerResult( GroupObjectTypeEnum Type , int ChannelID , bool Result )
{
}

//-----------------------------------------------------------------------------------------------------
// 收到頻道管理者
void NetCli_ChannelChild::RChat_ChannelOwner( GroupObjectTypeEnum Type , int ChannelID , int OwnerDBID , char* OwnerName )
{
	g_pDebugMessage->DebugOutput(0, "RChat_ChannelOwner Type=%d ChannelID=%d OwnerName=%s", Type , ChannelID , OwnerName );

	switch (Type)
	{
	case EM_GroupObjectType_SysChannel:			//系統
		break;

	case EM_GroupObjectType_SysZoneChannel:		//每個區域都有的
		break;

	case EM_GroupObjectType_PlayerChannel:		//玩家的
		{
			// 搜尋頻道是否存在
			const char* channelName = GetChannelNameByID(ChannelID);
			if ( channelName != NULL )
			{
				char content[512];
				char channelType[512];
				sprintf_s(channelType, 512, "%s%s", CHAT_MSG_CHANNEL, channelName);
				sprintf_s(content, 512, g_ObjectData->GetString("SYS_CHANNEL_LEADER"), OwnerName);
				g_pChatFrame->SendChatMessage(channelType, "", content);

				g_pChatFrame->ChangeChannelOwner(channelName,ChannelID,OwnerName);
			}

			s_channelLeader[ChannelID] = OwnerName;

			
		}
		break;
	case EM_GroupObjectType_Party:				//隊伍
		break;
	case EM_GroupObjectType_Guild:				//工會
		break;
	}
}

//--------------------------------------------------------
const char *NetCli_ChannelChild::GetChannelLeader(int ChannelID)
{
	const char *ChannelLeaderName=NULL;


	map<int, string>::iterator iter = s_channelLeader.find(ChannelID);
	if ( iter != s_channelLeader.end() )
	


		ChannelLeaderName=s_channelLeader[ChannelID].c_str();


	return ChannelLeaderName;



}