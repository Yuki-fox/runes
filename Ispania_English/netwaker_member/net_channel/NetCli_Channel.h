#pragma once

#include "PG/PG_ChannelBase.h"
#include "../../MainProc/NetGlobal.h"

#define OnClitoCliPacketFunctionBase     boost::function<void ( void* data , int Size ) >

struct ClitoCliCallBackStruct
{
	OnClitoCliPacketFunctionBase		Fun;	//CallBack函式
	int									Max;	//最大封包
	int									Min;	//最小封包

	ClitoCliCallBackStruct()
	{
		Fun = NULL;
		Max = 0xffff;
		Min = 0;			 
	};
};

#define  ClitoCli_NetReg( PGName ) NetCli_Channel::RegOnClitoCliPacketFunction( EM_##PGName , _##PGName , sizeof(PGName) , sizeof(PGName) )

class NetCli_Channel : public CNetGlobal
{
protected:
    //-------------------------------------------------------------------
    static NetCli_Channel* This;
    static bool _Init();
    static bool _Release();
    //-------------------------------------------------------------------

	static void _PG_ChannelBase_LtoC_CreateChannelResult		( int Sockid , int Size , void* Data );
	static void _PG_ChannelBase_LtoC_JoinChannelResult			( int Sockid , int Size , void* Data );
	static void _PG_ChannelBase_LtoC_LeaveChannelResult			( int Sockid , int Size , void* Data );
	static void _PG_ChannelBase_ChattoC_ClientConnectToChat		( int Sockid , int Size , void* Data );
	static void _PG_ChannelBase_ChattoC_DataTransmitCliToCli	( int Sockid , int Size , void* Data );

	static void _PG_ChannelBase_ChattoC_ChannelPlayerListResult	( int Sockid , int Size , void* Data );
	static void _PG_ChannelBase_ChattoC_JoinPlayerDisabledResult( int Sockid , int Size , void* Data );

	static void _PG_ChannelBase_ChattoC_SysChannel				( int Sockid , int Size , void* Data );
	static void _PG_ChannelBase_ChattoAll_SystemTime			( int Sockid , int Size , void* Data );

	static void _PG_ChannelBase_ChattoC_AddUserNotify			( int Sockid , int Size , void* Data );
	static void _PG_ChannelBase_ChattoC_DelUserNotify			( int Sockid , int Size , void* Data );
		
	static void _PG_ChannelBase_ChattoC_SetManagerResult		( int Sockid , int Size , void* Data );
	static void _PG_ChannelBase_ChattoC_ChannelOwner			( int Sockid , int Size , void* Data );
	static void _PG_ChannelBase_ChattoC_PlayerNotFind			( int Sockid , int Size , void* Data );
	//-------------------------------------------------------------------
	//Cli to Cli 轉送測試封包
	static void _PG_CtoC_Test( void *Data , int Size );

	//-------------------------------------------------------------------
	//Cli to Cli轉送點擊地圖的訊息
	static void _PG_CtoC_MapPing( void *Data , int Size );

	//-----------------------------------------------------------------------------------------------------
	//Cli to Cli 要求取得ModleInfo
	static void _PG_CtoC_GetModleInfoRequest( void *Data , int Size );

	//-----------------------------------------------------------------------------------------------------
	//Cli to Cli 傳回取得ModleInfo結果
	static void _PG_CtoC_GetModleInfoResult( void *Data , int Size );

	//-----------------------------------------------------------------------------------------------------
	//Cli to Cli 要求取得好友資訊
	static void _PG_CtoC_GiveMeYourFriendInfo( void *Data , int Size );

	//-----------------------------------------------------------------------------------------------------
	//Cli to Cli 回報好友資訊
	static void _PG_CtoC_GiveYouMyFriendInfo( void *Data , int Size );

	//-----------------------------------------------------------------------------------------------------
	//Cli to Cli 詢問是否是對方的朋友
	static void _PG_CtoC_AmIYourFriend( void *Data , int Size );

	//-----------------------------------------------------------------------------------------------------
	//Cli to Cli 詢問是否是對方的朋友結果
	static void _PG_CtoC_AmIYourFriendResult( void *Data , int Size );

	//-----------------------------------------------------------------------------------------------------
	//Cli to Cli 查詢玩家資訊
	static void _PG_CtoC_AskPlayerInfo( void *Data , int Size );

	//-----------------------------------------------------------------------------------------------------
	//Cli to Cli 查詢玩家資訊結果
	static void _PG_CtoC_AskPlayerInfoResult( void *Data , int Size );
 
	//-----------------------------------------------------------------------------------------------------
	//Cli to Cli 傳送訊息
	static void _PG_CtoC_Message( void *Data , int Size );
 
	//-----------------------------------------------------------------------------------------------------
	// Cli to Cli 設定團隊目標
	static void _PG_CtoC_SetRaidTarget( void *Data , int Size );
   
	//-----------------------------------------------------------------------------------------------------
	// Cli to Cli 追蹤玩家
	static void _PG_CtoC_TrackPlayer( void *Data , int Size );

	//-----------------------------------------------------------------------------------------------------
	// Cli to Cli 追蹤玩家 回報玩家座標
	static void _PG_CtoC_TrackPlayerResult( void *Data , int Size );

	//-----------------------------------------------------------------------------------------------------
	// Cli to Cli 追蹤玩家 回報玩家座標
	static void _PG_CtoC_AskHasItem( void *Data , int Size );

	//-----------------------------------------------------------------------------------------------------
	// Cli to Cli 訊問玩家是否要當會長

	static void _PG_CtoC_LeaderChange( void *Data , int Size );
	static void _PG_CtoC_LeaderChangeResult( void *Data , int Size );

	//-----------------------------------------------------------------------------------------------------
	// Cli to Cli 追蹤玩家 回報玩家座標
	static void _PG_CtoC_HasItemResult( void *Data , int Size );

	//-----------------------------------------------------------------------------------------------------
	// Cli to Cli 
	static void _PG_CtoC_AskRideMount( void *Data , int Size );
	static void _PG_CtoC_RideMountResult( void *Data , int Size );

	//-----------------------------------------------------------------------------------------------------
	// Cli to Cli 邀請加入頻道
	static void _PG_CtoC_ChannelInvite( void *Data , int Size );

	//Client To Client 的封包處理
	static vector< ClitoCliCallBackStruct >		_ClitoCliPacketFunctionList;
public:    
    NetCli_Channel() { This = this; }

	//註冊Cli to Cli 的封包處理函式
	static bool	RegOnClitoCliPacketFunction ( CliToCliPGCommandEnum PGid , OnClitoCliPacketFunctionBase  Func , int MinSize , int MaxSize );

	static void S_CreateChannel				( const char* ChannelName , const char* Password );
	static void S_JoinChannel				( const char* ChannelName , const char* Password , bool IsForceJoin );
	static void S_LeaveChannel				( GroupObjectTypeEnum Type , int ChannelID );
	static bool S_DataTransmitToCli_Group	( GroupObjectTypeEnum Type , int ChannelID , const void* Data , int DataSize );
	static bool S_DataTransmitToCli_Name	( char* Name , const void* Data , int DataSize );

	static void S_ChannelPlayerList		( GroupObjectTypeEnum Type , int ChannelID );
	static void S_JoinPlayerDisabled	( GroupObjectTypeEnum Type , int ChannelID , bool JoinEnabled );
	static void S_KickPlayer			( GroupObjectTypeEnum Type , int ChannelID , char* PlayerName );
	static void S_SetManager			( GroupObjectTypeEnum Type , int ChannelID , char* PlayerName );

	//區域頻道的ID
	static void S_ChannelZoneID			( int MapZoneID );

	virtual void R_CliConnectToChat( int ChatSockID ) = 0 ;
	//收到Cli 轉送給 Cli的資訊
	virtual void R_ClitoCli_Data( const char* Data , int Size ) = 0;

	virtual void R_CreateChannelOK(  int ChannelID , const char* Name ) = 0;
	virtual void R_CreateChannelFailed( const char* Name ) = 0;
	virtual void R_JoinChannelOK( int ChannelID , const char* Name , int ChannelOwnerDBID , const char* ChannelOwnerName ) = 0;
	virtual void R_JoinChannelFailed( const char* Name  ) = 0;
	virtual void R_LeaveChannelOK( int ChannelID ) = 0;
	virtual void R_LeaveChannelFailed( int ChannelID ) = 0;

	virtual void R_ChannelPlayerListResult	( GroupObjectTypeEnum Type , int ChannelID , vector<string> &Name ) = 0;
	virtual void R_JoinPlayerDisabledResult ( int ChannelID , GroupObjectTypeEnum Type , bool JoinEnabled ) = 0;

	//系統頻道列表
	virtual void R_SysChannel( GroupObjectTypeEnum Type , int ChannelID , char* Name ) = 0;

	virtual void R_SysTime( int Time , int TimeZone ) = 0;

	virtual void RChat_AddUserNotify( GroupObjectTypeEnum Type , int ChannelID , char* Name ) = 0;
	virtual void RChat_DelUserNotify( GroupObjectTypeEnum Type , int ChannelID , char* Name ) = 0;

	virtual void RChat_SetManagerResult( GroupObjectTypeEnum Type , int ChannelID , bool Result ) = 0;
	virtual void RChat_ChannelOwner( GroupObjectTypeEnum Type , int ChannelID , int OwnerDBID , char* OwnerName ) = 0;

	virtual void RChat_PlayerNotFind( const char* Name ) = 0;
	//------------------------------------------------------------------------------------------------------
	// Cli to Cli 測試
	static void SendToCliTest( char* Name , char* TestStr );
	virtual void R_CtoC_Test( char* TestData ) = 0;

	//-----------------------------------------------------------------------------------------------------
	//Cli to Cli 要求取得ModleInfo
	virtual void R_CtoC_GetModleInfoRequest( const char* pRequesterName ) = 0;

	//-----------------------------------------------------------------------------------------------------
	//Cli to Cli 傳回取得ModleInfo結果
	virtual void R_CtoC_GetModleInfoResult( int DBID , SModelInfo& ModelInfo ) = 0;
	
	//-------------------------------------------------------------------
	//Cli to Cli轉送點擊小地圖的訊息
	static void SendToCliMapPing( GroupObjectTypeEnum Type, int ChannelID, const char* pPingerName, int ZoneID, float fX, float fZ );
	virtual void R_CtoC_MapPing( const char* pPingerName, int ZoneID, float fX, float fZ ) = 0;

	//-----------------------------------------------------------------------------------------------------
	//Cli to Cli 要求取得好友資訊
	virtual void R_CtoC_GiveMeYourFriendInfo( const char* pRequesterName ) = 0;

	//-----------------------------------------------------------------------------------------------------
	//Cli to Cli 回報好友資訊
  	virtual void R_CtoC_GiveYouMyFriendInfo( const char* pName, SFriendInfo& FriendInfo ) = 0;

  	//-----------------------------------------------------------------------------------------------------
	//Cli to Cli 詢問是否是對方的朋友
	virtual void R_CtoC_AmIYourFriend( FriendListTypeENUM FriendListType, const char* AskerName, const char* AnswererName ) = 0;

	//-----------------------------------------------------------------------------------------------------
	//Cli to Cli 詢問是否是對方的朋友結果
	virtual void R_CtoC_AmIYourFriendResult( FriendListTypeENUM FriendListType, const char* AskerName, const char* AnswererName, bool Result ) = 0;

	//-----------------------------------------------------------------------------------------------------
	//Cli to Cli 查詢玩家資訊
	virtual void R_CtoC_AskPlayerInfo( const char* AskerName ) = 0;

	//-----------------------------------------------------------------------------------------------------
	//Cli to Cli 查詢玩家資訊結果
	virtual void R_CtoC_AskPlayerInfoResult( SAskPlayerInfo& PlayerInfo ) = 0;

	//-----------------------------------------------------------------------------------------------------
	static	void SendToCliMessage(const char* targetName, int messageType, const char* content);
	virtual	void ReceiveToCliMessage(int messageType, const char* content) = 0;

	//-------------------------------------------------------------------
	//Cli to Cli 查詢玩家是否有此物品
	static	void SendToCliAskHasItem(const char* targetName, const char* senderName, int itemVersion, int itemID);
	virtual	void ReceiveToCliAskHasItem(const char* SendName, int itemVersion, int itemID) = 0;


	//-------------------------------------------------------------------
	// Cli to Cli 訊問玩家是否要當會長
	static	void SendToCliAskLeaderChange(const char* targetName,const char* SendName);
	virtual	void ReceiveToCliAskLeaderChange(const char* SendName) = 0;
	//-------------------------------------------------------------------
	// Cli to Cli 回應玩家是否要當會長
	static	void SendToCliAskLeaderChangeResult(const char* targetName ,int DBID,bool YesNo);
	virtual	void ReceiveToCliAskLeaderChangeResult(int DBID,bool YesNo) = 0;
	
	

	//-------------------------------------------------------------------
	//Cli to Cli 轉送玩家是否有此物品
	static	void SendToCliHasItem(const char* targetName, int itemVersion, int itemDB, bool hasItem); 
	virtual	void ReceiveToCliHasItem(int itemVersion, int itemID, bool hasItem) = 0;

	//-----------------------------------------------------------------------------------------------------
	// Cli to Cli 設定團隊目標
	static  void S_CtoC_SetRaidTarget( GroupObjectTypeEnum Type, int ChannelID , int TargetID , int IconID );
	virtual void R_CtoC_SetRaidTarget( int TargetID , int IconID ) = 0; 
   

	//-----------------------------------------------------------------------------------------------------
	// Cli to Cli 追蹤玩家
	static  void S_CtoC_TrackPlayer( const char* TargetName , const char* SenderName );
	virtual void R_CtoC_TrackPlayer( const char* SenderName ) = 0; 
   
 	//-----------------------------------------------------------------------------------------------------
	// Cli to Cli追蹤玩家 回報玩家座標
	virtual void R_CtoC_TrackPlayerResult( const char* Name, int ZoneID, float fX, float fY, float fZ ) = 0; 	

	//-----------------------------------------------------------------------------------------------------
	// Cli to Cli訊問是否願意塔乘坐騎
	static	void S_CtoC_AskRideMount(const char* targetName, const char* senderName, int GItemID);
	virtual	void R_CtoC_AskRideMount(const char* Name, int GItemID) = 0;

	//-----------------------------------------------------------------------------------------------------
	// Cli to Cli回應是否共乘
	static	void S_CtoC_RideMountResult(const char* targetName, const char* senderName, int answer);
	virtual	void R_CtoC_RideMountResult(const char* Name, int answer) = 0;

	//-----------------------------------------------------------------------------------------------------
	// Cli to Cli 邀請加入頻道
	virtual void R_CtoC_ChannelInvite( const char* AskerName, const char* channelName, const char* password ) = 0;

};

