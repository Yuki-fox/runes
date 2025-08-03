#include "NetCli_Channel.h"

//-------------------------------------------------------------------
NetCli_Channel*    NetCli_Channel::This         = NULL;
vector< ClitoCliCallBackStruct >		NetCli_Channel::_ClitoCliPacketFunctionList;
//-------------------------------------------------------------------
bool NetCli_Channel::_Init()
{
	/*
	_Net.RegOnSrvPacketFunction	( EM_PG_ChannelBase_LtoC_CreateChannelResult		, _PG_ChannelBase_LtoC_CreateChannelResult	 );
	_Net.RegOnSrvPacketFunction	( EM_PG_ChannelBase_LtoC_JoinChannelResult			 , _PG_ChannelBase_LtoC_JoinChannelResult		 );
	_Net.RegOnSrvPacketFunction	( EM_PG_ChannelBase_LtoC_LeaveChannelResult			 , _PG_ChannelBase_LtoC_LeaveChannelResult		 );
	_Net.RegOnSrvPacketFunction	( EM_PG_ChannelBase_ChattoC_ClientConnectToChat		, _PG_ChannelBase_ChattoC_ClientConnectToChat	 );
	_Net.RegOnSrvPacketFunction	( EM_PG_ChannelBase_ChattoC_DataTransmitCliToCli	, _PG_ChannelBase_ChattoC_DataTransmitCliToCli	 );
	_Net.RegOnSrvPacketFunction	( EM_PG_ChannelBase_ChattoC_ChannelPlayerListResult	, _PG_ChannelBase_ChattoC_ChannelPlayerListResult	 );
	_Net.RegOnSrvPacketFunction	( EM_PG_ChannelBase_ChattoC_JoinPlayerDisabledResult, _PG_ChannelBase_ChattoC_JoinPlayerDisabledResult	 );
	_Net.RegOnSrvPacketFunction	( EM_PG_ChannelBase_ChattoC_SysChannel				, _PG_ChannelBase_ChattoC_SysChannel	 );
	_Net.RegOnSrvPacketFunction	( EM_PG_ChannelBase_ChattoAll_SystemTime			, _PG_ChannelBase_ChattoAll_SystemTime	 );
	_Net.RegOnSrvPacketFunction	( EM_PG_ChannelBase_ChattoC_AddUserNotify			, _PG_ChannelBase_ChattoC_AddUserNotify	 );
	_Net.RegOnSrvPacketFunction	( EM_PG_ChannelBase_ChattoC_DelUserNotify			, _PG_ChannelBase_ChattoC_DelUserNotify	 );
	_Net.RegOnSrvPacketFunction	( EM_PG_ChannelBase_ChattoC_SetManagerResult		, _PG_ChannelBase_ChattoC_SetManagerResult	);
	_Net.RegOnSrvPacketFunction	( EM_PG_ChannelBase_ChattoC_ChannelOwner			, _PG_ChannelBase_ChattoC_ChannelOwner		);
*/
	Cli_NetReg( PG_ChannelBase_LtoC_CreateChannelResult			);
	Cli_NetReg( PG_ChannelBase_LtoC_JoinChannelResult			);
	Cli_NetReg( PG_ChannelBase_LtoC_LeaveChannelResult			);
	Cli_NetReg( PG_ChannelBase_ChattoC_ClientConnectToChat		);
	Cli_NetReg( PG_ChannelBase_ChattoC_DataTransmitCliToCli		);
	Cli_NetReg( PG_ChannelBase_ChattoC_ChannelPlayerListResult	);
	Cli_NetReg( PG_ChannelBase_ChattoC_JoinPlayerDisabledResult	);
	Cli_NetReg( PG_ChannelBase_ChattoC_SysChannel				);
	Cli_NetReg( PG_ChannelBase_ChattoAll_SystemTime				);
	Cli_NetReg( PG_ChannelBase_ChattoC_AddUserNotify			);
	Cli_NetReg( PG_ChannelBase_ChattoC_DelUserNotify			);
	Cli_NetReg( PG_ChannelBase_ChattoC_SetManagerResult			);
	Cli_NetReg( PG_ChannelBase_ChattoC_ChannelOwner				);
	Cli_NetReg( PG_ChannelBase_ChattoC_PlayerNotFind			);


	//RegOnClitoCliPacketFunction ( EM_PG_CtoC_Test , _PG_CtoC_Test , sizeof(PG_CtoC_Test) , sizeof(PG_CtoC_Test) );
	ClitoCli_NetReg( PG_CtoC_Test );
	ClitoCli_NetReg( PG_CtoC_MapPing );
	ClitoCli_NetReg( PG_CtoC_GetModleInfoRequest );
	ClitoCli_NetReg( PG_CtoC_GetModleInfoResult );
	ClitoCli_NetReg( PG_CtoC_GiveMeYourFriendInfo );
	ClitoCli_NetReg( PG_CtoC_GiveYouMyFriendInfo );
	ClitoCli_NetReg( PG_CtoC_AmIYourFriend );
	ClitoCli_NetReg( PG_CtoC_AmIYourFriendResult );

	ClitoCli_NetReg( PG_CtoC_AskPlayerInfo );
	ClitoCli_NetReg( PG_CtoC_AskPlayerInfoResult );

	ClitoCli_NetReg( PG_CtoC_Message );	
 	//ClitoCli_NetReg( PG_CtoC_DelFriend );	 //2008-07-10 好友名單從雙向改成單向 不用再通知對方刪除
   	ClitoCli_NetReg( PG_CtoC_SetRaidTarget );	
	ClitoCli_NetReg( PG_CtoC_TrackPlayer );	
	ClitoCli_NetReg( PG_CtoC_TrackPlayerResult );

	ClitoCli_NetReg( PG_CtoC_AskHasItem );
	ClitoCli_NetReg( PG_CtoC_HasItemResult );
	ClitoCli_NetReg( PG_CtoC_AskRideMount );
	ClitoCli_NetReg( PG_CtoC_RideMountResult );


	ClitoCli_NetReg( PG_CtoC_LeaderChange );
	ClitoCli_NetReg( PG_CtoC_LeaderChangeResult );

	ClitoCli_NetReg( PG_CtoC_ChannelInvite );

    return true;
}
//-------------------------------------------------------------------
void NetCli_Channel::_PG_ChannelBase_ChattoC_PlayerNotFind			( int Sockid , int Size , void* Data )
{
	PG_ChannelBase_ChattoC_PlayerNotFind* PG = (PG_ChannelBase_ChattoC_PlayerNotFind*)Data;
	This->RChat_PlayerNotFind( PG->Name.Begin() );
}
void NetCli_Channel::_PG_ChannelBase_ChattoC_SetManagerResult		( int Sockid , int Size , void* Data )
{
	PG_ChannelBase_ChattoC_SetManagerResult* PG = (PG_ChannelBase_ChattoC_SetManagerResult*)Data;
	This->RChat_SetManagerResult( PG->Type , PG->ChannelID , PG->Result );
}
void NetCli_Channel::_PG_ChannelBase_ChattoC_ChannelOwner			( int Sockid , int Size , void* Data )
{
	PG_ChannelBase_ChattoC_ChannelOwner* PG = (PG_ChannelBase_ChattoC_ChannelOwner*)Data;
	This->RChat_ChannelOwner( PG->Type , PG->ChannelID , PG->OwnerDBID , (char*)PG->OwnerName.Begin() );
}

void NetCli_Channel::_PG_ChannelBase_ChattoC_AddUserNotify			( int Sockid , int Size , void* Data )
{
	PG_ChannelBase_ChattoC_AddUserNotify* PG = (PG_ChannelBase_ChattoC_AddUserNotify*)Data;
	This->RChat_AddUserNotify( PG->Type , PG->ChannelID , (char*)PG->Name.Begin() );
}
void NetCli_Channel::_PG_ChannelBase_ChattoC_DelUserNotify			( int Sockid , int Size , void* Data )
{
	PG_ChannelBase_ChattoC_DelUserNotify* PG = (PG_ChannelBase_ChattoC_DelUserNotify*)Data;
	This->RChat_DelUserNotify( PG->Type , PG->ChannelID , (char*)PG->Name.Begin() );
}

//Cli to Cli 轉送測試封包
void NetCli_Channel::_PG_CtoC_Test( void *Data , int Size )
{
	PG_CtoC_Test* PG = (PG_CtoC_Test*)Data;
	This->R_CtoC_Test( PG->TestData );
}

//-------------------------------------------------------------------
//Cli to Cli轉送點擊地圖的訊息
void NetCli_Channel::_PG_CtoC_MapPing( void *Data , int Size )
{
	PG_CtoC_MapPing* PG = (PG_CtoC_MapPing*)Data;
	This->R_CtoC_MapPing( PG->PingerName.Begin() , PG->ZoneID , PG->fX , PG->fZ );
}

//-----------------------------------------------------------------------------------------------------
//Cli to Cli 要求取得ModleInfo
void NetCli_Channel::_PG_CtoC_GetModleInfoRequest( void *Data , int Size )
{
	PG_CtoC_GetModleInfoRequest* PG = (PG_CtoC_GetModleInfoRequest*)Data;
	This->R_CtoC_GetModleInfoRequest( PG->RequesterName.Begin() );
}

//-----------------------------------------------------------------------------------------------------
//Cli to Cli 傳回取得ModleInfo結果
void NetCli_Channel::_PG_CtoC_GetModleInfoResult( void *Data , int Size )
{
	PG_CtoC_GetModleInfoResult* PG = (PG_CtoC_GetModleInfoResult*)Data;
	This->R_CtoC_GetModleInfoResult( PG->DBID , PG->ModelInfo );
}

//-----------------------------------------------------------------------------------------------------
//Cli to Cli 要求取得好友資訊
void NetCli_Channel::_PG_CtoC_GiveMeYourFriendInfo( void *Data , int Size )
{
	PG_CtoC_GiveMeYourFriendInfo* PG = (PG_CtoC_GiveMeYourFriendInfo*)Data;
	This->R_CtoC_GiveMeYourFriendInfo( PG->SenderName.Begin() );
}

//-----------------------------------------------------------------------------------------------------
//Cli to Cli 回報好友資訊
void NetCli_Channel::_PG_CtoC_GiveYouMyFriendInfo( void *Data , int Size )
{
	PG_CtoC_GiveYouMyFriendInfo* PG = (PG_CtoC_GiveYouMyFriendInfo*)Data;
	This->R_CtoC_GiveYouMyFriendInfo( PG->Name , PG->FriendInfo );
}

//-----------------------------------------------------------------------------------------------------
//Cli to Cli 詢問是否是對方的朋友
void NetCli_Channel::_PG_CtoC_AmIYourFriend( void *Data , int Size )
{
	PG_CtoC_AmIYourFriend* PG = (PG_CtoC_AmIYourFriend*)Data;
	This->R_CtoC_AmIYourFriend( PG->FriendListType, PG->AskerName, PG->AnswererName );
}

//-----------------------------------------------------------------------------------------------------
//Cli to Cli 詢問是否是對方的朋友結果
void NetCli_Channel::_PG_CtoC_AmIYourFriendResult( void *Data , int Size )
{
	PG_CtoC_AmIYourFriendResult* PG = (PG_CtoC_AmIYourFriendResult*)Data;
	This->R_CtoC_AmIYourFriendResult( PG->FriendListType, PG->AskerName, PG->AnswererName, PG->Result );
}

//-----------------------------------------------------------------------------------------------------
//Cli to Cli 查詢玩家資訊
void NetCli_Channel::_PG_CtoC_AskPlayerInfo( void *Data , int Size )
{
	PG_CtoC_AskPlayerInfo* PG = (PG_CtoC_AskPlayerInfo*)Data;
	This->R_CtoC_AskPlayerInfo( PG->SenderName );
}

//-----------------------------------------------------------------------------------------------------
//Cli to Cli 查詢玩家資訊結果
void NetCli_Channel::_PG_CtoC_AskPlayerInfoResult( void *Data , int Size )
{
	PG_CtoC_AskPlayerInfoResult* PG = (PG_CtoC_AskPlayerInfoResult*)Data;
	This->R_CtoC_AskPlayerInfoResult( PG->PlayerInfo );
}

//-----------------------------------------------------------------------------------------------------
//Cli to Cli 傳送訊息
void NetCli_Channel::_PG_CtoC_Message(void *Data, int Size)
{
	PG_CtoC_Message* PG = (PG_CtoC_Message*)Data;
	This->ReceiveToCliMessage(PG->messageType, PG->Content);
}

//-----------------------------------------------------------------------------------------------------
// Cli to Cli 設定團隊目標
void NetCli_Channel::_PG_CtoC_SetRaidTarget( void *Data , int Size )
{
	PG_CtoC_SetRaidTarget* PG = (PG_CtoC_SetRaidTarget*)Data;
	This->R_CtoC_SetRaidTarget( PG->TargetID , PG->IconID );
}

//-----------------------------------------------------------------------------------------------------
// Cli to Cli 追蹤玩家
void NetCli_Channel::_PG_CtoC_TrackPlayer( void *Data , int Size )
{
	PG_CtoC_TrackPlayer* PG = (PG_CtoC_TrackPlayer*)Data;
	This->R_CtoC_TrackPlayer( PG->SenderName );
}

//-----------------------------------------------------------------------------------------------------
// Cli to Cli 追蹤玩家 回報玩家座標
void NetCli_Channel::_PG_CtoC_TrackPlayerResult( void *Data , int Size )
{
	PG_CtoC_TrackPlayerResult* PG = (PG_CtoC_TrackPlayerResult*)Data;
	This->R_CtoC_TrackPlayerResult( PG->TargetName, PG->WorldMapID, PG->fX, PG->fY, PG->fZ );
}

//-----------------------------------------------------------------------------------------------------
// Cli to Cli 追蹤玩家 回報玩家座標
void NetCli_Channel::_PG_CtoC_AskHasItem( void *Data , int Size )
{
	PG_CtoC_AskHasItem* PG = (PG_CtoC_AskHasItem*)Data;
	This->ReceiveToCliAskHasItem( PG->SenderName, PG->itemVersion, PG->itemID);
}
//-----------------------------------------------------------------------------------------------------
//Cli to Cli 訊問玩家是否要當會長
void NetCli_Channel::_PG_CtoC_LeaderChange( void *Data , int Size )
{
	PG_CtoC_LeaderChange* PG = (PG_CtoC_LeaderChange*)Data;
	This->ReceiveToCliAskLeaderChange( PG->SenderName);
}
//-----------------------------------------------------------------------------------------------------
//Cli to Cli 回應玩家是否要當會長
void NetCli_Channel::_PG_CtoC_LeaderChangeResult( void *Data , int Size )
{
	PG_CtoC_LeaderChangeResult* PG = (PG_CtoC_LeaderChangeResult*)Data;
	This->ReceiveToCliAskLeaderChangeResult( PG->DBID, PG->YesNo);
}
//-----------------------------------------------------------------------------------------------------
// Cli to Cli 追蹤玩家 回報玩家座標
void NetCli_Channel::_PG_CtoC_HasItemResult( void *Data , int Size )
{
	PG_CtoC_HasItemResult* PG = (PG_CtoC_HasItemResult*)Data;
	This->ReceiveToCliHasItem( PG->itemVersion, PG->itemID, PG->hasItem );
}

//-----------------------------------------------------------------------------------------------------
// Cli to Cli 訊問是否要共乘坐騎
void NetCli_Channel::_PG_CtoC_AskRideMount( void *Data , int Size )
{
	PG_CtoC_AskRideMount* PG = (PG_CtoC_AskRideMount*)Data;
	This->R_CtoC_AskRideMount( PG->SenderName, PG->GItemID );
}

//-----------------------------------------------------------------------------------------------------
// Cli to Cli 回應是否要共乘坐騎
void NetCli_Channel::_PG_CtoC_RideMountResult( void *Data , int Size )
{
	PG_CtoC_RideMountResult* PG = (PG_CtoC_RideMountResult*)Data;
	This->R_CtoC_RideMountResult( PG->name, PG->answer );
}

//-----------------------------------------------------------------------------------------------------
// Cli to Cli 邀請加入頻道
void NetCli_Channel::_PG_CtoC_ChannelInvite( void *Data , int Size )
{
	PG_CtoC_ChannelInvite* PG = (PG_CtoC_ChannelInvite*)Data;
	This->R_CtoC_ChannelInvite( PG->SenderName, PG->ChannelName, PG->ChannelPass );
}

bool NetCli_Channel::_Release()
{
    return true;
}

void NetCli_Channel::_PG_ChannelBase_ChattoAll_SystemTime			( int Sockid , int Size , void* Data )
{
	PG_ChannelBase_ChattoAll_SystemTime* PG = (PG_ChannelBase_ChattoAll_SystemTime*)Data;
	This->R_SysTime( PG->Time , PG->TimeZone );
}
void NetCli_Channel::_PG_ChannelBase_ChattoC_SysChannel				( int Sockid , int Size , void* Data )
{
	PG_ChannelBase_ChattoC_SysChannel* PG = (PG_ChannelBase_ChattoC_SysChannel*)Data;
	This->R_SysChannel( PG->Type , PG->ChannelID , (char*)PG->Name.Begin() );
}

void NetCli_Channel::_PG_ChannelBase_ChattoC_ChannelPlayerListResult	( int Sockid , int Size , void* Data )
{
	PG_ChannelBase_ChattoC_ChannelPlayerListResult* PG = (PG_ChannelBase_ChattoC_ChannelPlayerListResult*)Data;

	vector<string> Name;

	for( int i = 0 ; i < PG->Count ; i++ )
	{
		Name.push_back( PG->PlayerNameList[i].Begin() );
	}

	This->R_ChannelPlayerListResult( PG->Type , PG->ChannelID , Name );

}
void NetCli_Channel::_PG_ChannelBase_ChattoC_JoinPlayerDisabledResult( int Sockid , int Size , void* Data )
{
	PG_ChannelBase_ChattoC_JoinPlayerDisabledResult* PG = (PG_ChannelBase_ChattoC_JoinPlayerDisabledResult*)Data;

	This->R_JoinPlayerDisabledResult( PG->ChannelID , PG->Type , PG->JoinEnabled );
}


void NetCli_Channel::_PG_ChannelBase_LtoC_CreateChannelResult	( int Sockid , int Size , void* Data )
{
	PG_ChannelBase_LtoC_CreateChannelResult* PG = (PG_ChannelBase_LtoC_CreateChannelResult*)Data;
	if( PG->Result == true )
	{
		This->R_CreateChannelOK( PG->ChannelID , PG->ChannelName.Begin() );
	}
	else
	{
		This->R_CreateChannelFailed( PG->ChannelName.Begin() );
	}
}
void NetCli_Channel::_PG_ChannelBase_LtoC_JoinChannelResult	( int Sockid , int Size , void* Data )
{
	PG_ChannelBase_LtoC_JoinChannelResult* PG = (PG_ChannelBase_LtoC_JoinChannelResult*)Data;
	if( PG->Result == true )
	{
		This->R_JoinChannelOK( PG->ChannelID , PG->ChannelName.Begin() , PG->ChannelOwnerDBID , PG->ChannelOwnerName.Begin() );
	}
	else
	{
		This->R_JoinChannelFailed( PG->ChannelName.Begin()  );
	}
}
void NetCli_Channel::_PG_ChannelBase_LtoC_LeaveChannelResult	( int Sockid , int Size , void* Data )
{
	PG_ChannelBase_LtoC_LeaveChannelResult* PG = (PG_ChannelBase_LtoC_LeaveChannelResult*)Data;
	if( PG->Result == true )
	{
		This->R_LeaveChannelOK( PG->ChannelID );
	}
	else
	{
		This->R_LeaveChannelFailed( PG->ChannelID  );
	}
}
void NetCli_Channel::_PG_ChannelBase_ChattoC_DataTransmitCliToCli	( int Sockid , int Size , void* Data )
{
	PG_ChannelBase_ChattoC_DataTransmitCliToCli* PG = (PG_ChannelBase_ChattoC_DataTransmitCliToCli*)Data;
	This->R_ClitoCli_Data( PG->Data , PG->DataSize );
}


void NetCli_Channel::_PG_ChannelBase_ChattoC_ClientConnectToChat( int Sockid , int Size , void* Data )
{
	This->R_CliConnectToChat( Sockid );
}

void NetCli_Channel::S_CreateChannel			( const char* ChannelName , const char* Password )
{
	PG_ChannelBase_CtoL_CreateChannel Send;
	Send.ChannelName    = ChannelName;
	Send.Password		= Password;

	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Channel::S_JoinChannel				( const char* ChannelName , const char* Password, bool IsForceJoin )
{
	PG_ChannelBase_CtoL_JoinChannel Send;
	Send.ChannelName = ChannelName;
	Send.Password = Password;
	Send.IsForceJoin = IsForceJoin;
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Channel::S_LeaveChannel				( GroupObjectTypeEnum Type  , int ChannelID )
{
	PG_ChannelBase_CtoL_LeaveChannel Send;
	Send.ChannelID= ChannelID;
	Send.Type = Type;
	
	SendToLocal( sizeof(Send) , &Send );
}
bool NetCli_Channel::S_DataTransmitToCli_Group	( GroupObjectTypeEnum Type , int ChannelID , const void* Data , int DataSize )
{
	PG_ChannelBase_CtoChat_DataTransmitToCli_Group	Send;

	if( DataSize > sizeof( Send.Data ) )
		return false;

	memcpy( Send.Data , Data , DataSize );
	Send.DataSize = DataSize;
	Send.Type = Type;
	Send.ChannelID = ChannelID;

	SendToChat(  sizeof(Send) , &Send );

	return true;
}
bool NetCli_Channel::S_DataTransmitToCli_Name	( char* Name , const void* Data , int DataSize )
{
	PG_ChannelBase_CtoChat_DataTransmitToCli_Name Send;

	if( DataSize > sizeof( Send.Data ) )
		return false;

	memcpy( Send.Data , Data , DataSize );
	Send.DataSize = DataSize;
	Send.Name = Name;
	SendToChat(  sizeof(Send) , &Send );

	return true;
}

void NetCli_Channel::S_ChannelPlayerList	( GroupObjectTypeEnum Type , int ChannelID )
{
	PG_ChannelBase_CtoChat_ChannelPlayerList Send;
	Send.Type = Type;
	Send.ChannelID = ChannelID;
	SendToChat(  sizeof(Send) , &Send );
}
void NetCli_Channel::S_JoinPlayerDisabled	( GroupObjectTypeEnum Type , int ChannelID , bool JoinEnabled )
{
	PG_ChannelBase_CtoChat_JoinPlayerDisabled Send;
	Send.Type = Type;
	Send.ChannelID = ChannelID;
	Send.JoinEnabled = JoinEnabled;
	SendToChat(  sizeof(Send) , &Send );
}
void NetCli_Channel::S_KickPlayer			( GroupObjectTypeEnum Type , int ChannelID , char* PlayerName )
{
	PG_ChannelBase_CtoChat_KickPlayer Send;
	Send.Type = Type;
	Send.ChannelID = ChannelID;
	Send.PlayerName = PlayerName;
	SendToChat(  sizeof(Send) , &Send );
}
void NetCli_Channel::S_SetManager			( GroupObjectTypeEnum Type , int ChannelID , char* PlayerName )
{
	PG_ChannelBase_CtoChat_SetManager Send;
	Send.Type = Type;
	Send.ChannelID = ChannelID;
	Send.PlayerName = PlayerName;
	SendToChat(  sizeof(Send) , &Send );
}
//區域頻道的ID
void NetCli_Channel::S_ChannelZoneID			( int MapZoneID )
{
	PG_ChannelBase_CtoChat_ZoneChannelID Send;
	Send.MapZoneID = MapZoneID;
	SendToChat(  sizeof(Send) , &Send );
}

//註冊Cli to Cli 的封包處理函式
bool NetCli_Channel::RegOnClitoCliPacketFunction ( CliToCliPGCommandEnum PGid , OnClitoCliPacketFunctionBase  Func , int MinSize , int MaxSize )
{
	if( PGid >= EM_CliToCliPGCommandEnum_MAX )
	{
		TRACE1( "PGid >= EM_CliToCliPGCommandEnum_MAX PGid = %d" , PGid );
		return false;
	}

	ClitoCliCallBackStruct Temp;

	while( PGid >= (int)_ClitoCliPacketFunctionList.size() )
		_ClitoCliPacketFunctionList.push_back( Temp );

	if( _ClitoCliPacketFunctionList[ PGid ].Fun != NULL )
	{
		TRACE1( "_ClitoCliPacketFunctionList[ PGid ].Fun PGid = %d" , PGid );
		return false;
	}

	Temp.Fun = Func;
	Temp.Max = MaxSize;
	Temp.Min = MinSize;

	_ClitoCliPacketFunctionList[ PGid ] = Temp;

	return true;
}

void NetCli_Channel::SendToCliTest( char* Name , char* TestStr )
{
	PG_CtoC_Test Send;
	strncpy( Send.TestData , TestStr , sizeof(Send.TestData) );
	S_DataTransmitToCli_Name	( Name , &Send , sizeof(Send) );
}


//-------------------------------------------------------------------
//Cli to Cli轉送點擊地圖的訊息
void NetCli_Channel::SendToCliMapPing( GroupObjectTypeEnum Type , int ChannelID , const char* pPingerName , int ZoneID , float fX , float fZ )
{
	PG_CtoC_MapPing Send;

	Send.PingerName = pPingerName;
	Send.ZoneID     = ZoneID;
	Send.fX         = fX  ;
	Send.fZ         = fZ  ;

	S_DataTransmitToCli_Group( Type, ChannelID, &Send, sizeof(Send) );
}

//-------------------------------------------------------------------
//Cli to Cli 轉換訊息
void NetCli_Channel::SendToCliMessage(const char* targetName, int messageType, const char* content)
{
	PG_CtoC_Message Send;
	Send.messageType = messageType;
	Send.SenderName = "";
	strncpy(Send.Content, content, sizeof(Send.Content));
	S_DataTransmitToCli_Name( (char*)targetName , &Send , sizeof(Send) );
}

//-------------------------------------------------------------------
void NetCli_Channel::SendToCliAskHasItem(const char* targetName, const char* senderName, int itemVersion, int itemID)
{
	PG_CtoC_AskHasItem Send;
	Send.SenderName = senderName;
	Send.itemVersion = itemVersion;
	Send.itemID = itemID;
	S_DataTransmitToCli_Name( (char*)targetName , &Send , sizeof(Send) );
}
//-------------------------------------------------------------------
void NetCli_Channel::SendToCliAskLeaderChange(const char* targetName, const char* senderName)
{
	PG_CtoC_LeaderChange Send;
	Send.SenderName = senderName;
	
	S_DataTransmitToCli_Name( (char*)targetName , &Send , sizeof(Send) );
}
//-------------------------------------------------------------------
void NetCli_Channel::SendToCliAskLeaderChangeResult(const char* targetName,int DBID,bool YesNo)
{
	PG_CtoC_LeaderChangeResult Send;
	Send.DBID=DBID;
	Send.YesNo = YesNo;

	S_DataTransmitToCli_Name( (char*)targetName , &Send , sizeof(Send) );
}

//-------------------------------------------------------------------
void NetCli_Channel::SendToCliHasItem(const char* targetName, int itemVersion, int itemDB, bool hasItem)
{
	PG_CtoC_HasItemResult Send;
	Send.hasItem = hasItem;
	Send.itemVersion = itemVersion;
	Send.itemID = itemDB;
	S_DataTransmitToCli_Name( (char*)targetName , &Send , sizeof(Send) );
}

//-------------------------------------------------------------------
//Cli to Cli 轉換訊息
void NetCli_Channel::S_CtoC_SetRaidTarget( GroupObjectTypeEnum Type, int ChannelID, int TargetID, int IconID )
{
	PG_CtoC_SetRaidTarget Send;

	Send.TargetID = TargetID ;
	Send.IconID   = IconID   ;
 
	S_DataTransmitToCli_Group( Type, ChannelID, &Send, sizeof(Send) );
}

//-----------------------------------------------------------------------------------------------------
// Cli to Cli 追蹤玩家
void NetCli_Channel::S_CtoC_TrackPlayer( const char* TargetName , const char* SenderName )
{
	PG_CtoC_TrackPlayer Send;

	Send.SenderName = SenderName ;
	S_DataTransmitToCli_Name( (char*)TargetName, &Send, sizeof(Send) );
}

//-----------------------------------------------------------------------------------------------------
// Cli to Cli 訊問是否願意塔乘坐騎
void NetCli_Channel::S_CtoC_AskRideMount(const char* targetName, const char* senderName, int GItemID)
{
	PG_CtoC_AskRideMount Send;
	Send.SenderName = senderName;	
	Send.GItemID = GItemID;
	S_DataTransmitToCli_Name( (char*)targetName , &Send , sizeof(Send) );
}

//-----------------------------------------------------------------------------------------------------
// Cli to Cli 回應是否共乘
void NetCli_Channel::S_CtoC_RideMountResult(const char* targetName, const char* senderName, int answer)
{
	PG_CtoC_RideMountResult Send;
	Send.name = senderName;
	Send.answer = answer;
	S_DataTransmitToCli_Name( (char*)targetName , &Send , sizeof(Send) );
}