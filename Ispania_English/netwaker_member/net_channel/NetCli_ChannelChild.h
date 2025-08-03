#pragma once

#include <map>
#include <string>
#include "NetCli_Channel.h"

using namespace std;

class NetCli_ChannelChild;
extern NetCli_ChannelChild* g_pNetCli_ChannelChild;

class NetCli_ChannelChild : public NetCli_Channel
{
public:
	static bool _Init();
	static bool _Release();
	
	static	int					GetChannelID(const char* name);	
	static	const char*			GetChannelNameByID(int id);
	static	int					GetZoneChannelID(const char* name);
	static	int					ChannelKick(const char* channelName, const char* playerName);
	static	int					SetChannelOwner(const char* channelName, const char* playerName);
	//static  void ClitoCli_Data( const char* Data , int Size ) { This->}

	virtual void R_CliConnectToChat( int ChatSockID );
	//收到Cli 轉送給 Cli的資訊
	virtual void R_ClitoCli_Data( const char* Data , int Size );

	virtual void R_CreateChannelOK( int ChannelID , const char* Name );
	virtual void R_CreateChannelFailed( const char* Name );
	virtual void R_JoinChannelOK( int ChannelID , const char* Name , int ChannelOwnerDBID , const char* ChannelOwnerName );
	virtual void R_JoinChannelFailed( const char* Name );
	virtual void R_LeaveChannelOK( int ChannelID );
	void removeChannel( int ChannelID);
	virtual void R_LeaveChannelFailed( int ChannelID );

	// 加入頻道
	virtual void RChat_AddUserNotify( GroupObjectTypeEnum Type , int ChannelID , char* Name );
	// 刪除頻道
	virtual void RChat_DelUserNotify( GroupObjectTypeEnum Type , int ChannelID , char* Name );
	// 設定頻道權限回應
	virtual void RChat_SetManagerResult( GroupObjectTypeEnum Type , int ChannelID , bool Result );
	// 收到頻道管理者
	virtual void RChat_ChannelOwner( GroupObjectTypeEnum Type , int ChannelID , int OwnerDBID , char* OwnerName );

	virtual void R_ChannelPlayerListResult	( GroupObjectTypeEnum Type , int ChannelID , vector<string> &Name );
	virtual void R_JoinPlayerDisabledResult ( int ChannelID , GroupObjectTypeEnum Type , bool JoinEnabled );	

	//系統頻道列表
	virtual void R_SysChannel( GroupObjectTypeEnum Type , int ChannelID , char* Name );

	//系統時間
	virtual void R_SysTime( int Time , int TimeZone );	

	virtual void RChat_PlayerNotFind( const char* Name );

	//--------------------------------------------------------------------------------------
	virtual void R_CtoC_Test( char* TestData );

	//-------------------------------------------------------------------
	//Cli to Cli轉送點擊地圖的訊息
	virtual void R_CtoC_MapPing( const char* pPingerName, int ZoneID, float fX, float fZ );
   
	//-----------------------------------------------------------------------------------------------------
	//Cli to Cli 要求取得ModleInfo
	virtual void GetModleInfoRequest( const char* pTargetName );
	virtual void R_CtoC_GetModleInfoRequest( const char* pRequesterName );

	//-----------------------------------------------------------------------------------------------------
	//Cli to Cli 傳回取得ModleInfo結果
	virtual void R_CtoC_GetModleInfoResult( int DBID , SModelInfo& ModelInfo );
 
	//-----------------------------------------------------------------------------------------------------
	//Cli to Cli 要求取得好友資訊
	virtual void GiveMeYourFriendInfo( const char* pTargetName );
	virtual void R_CtoC_GiveMeYourFriendInfo( const char* SenderName );

	//-----------------------------------------------------------------------------------------------------
	//Cli to Cli 回報好友資訊
	virtual void R_CtoC_GiveYouMyFriendInfo( const char* pName, SFriendInfo& FriendInfo );
  
	//-----------------------------------------------------------------------------------------------------
	//Cli to Cli 詢問是否是對方的朋友
 	virtual void AmIYourFriend( FriendListTypeENUM FriendListType, const char* AskerName, const char* AnswererName );
	virtual void R_CtoC_AmIYourFriend( FriendListTypeENUM FriendListType, const char* AskerName, const char* AnswererName );

	//-----------------------------------------------------------------------------------------------------
	//Cli to Cli 詢問是否是對方的朋友結果
	virtual void R_CtoC_AmIYourFriendResult( FriendListTypeENUM FriendListType, const char* AskerName, const char* AnswererName, bool Result );

	//-----------------------------------------------------------------------------------------------------
	//Cli to Cli 通知對方被新增或刪除朋友
	virtual void YouAreMyFriend( FriendListTypeENUM FriendListType, const char* Name, bool bYes );

	//-------------------------------------------------------------------
	// Cli to Cli 訊息對方是否有特定物品
	virtual	void ReceiveToCliAskHasItem(const char* SendName, int itemVersion, int itemID);


	//-------------------------------------------------------------------
	// Cli to Cli 訊問玩家是否要當會長
	virtual	void ReceiveToCliAskLeaderChange(const char* SendName);


	//-------------------------------------------------------------------
	// Cli to Cli回報玩家是否要當會長
	virtual	void ReceiveToCliAskLeaderChangeResult(int dbid,bool YesNo);

	//-------------------------------------------------------------------
	//Cli to Cli 轉送玩家是否有特定物品
	virtual	void ReceiveToCliHasItem(int itemVersion, int itemID, bool hasItem);

	//-----------------------------------------------------------------------------------------------------
protected:
	static string s_AskPlayerInfo_Name;
	static int s_AskPlayerInfo_LastTime;
public:
	//-----------------------------------------------------------------------------------------------------
	//查詢玩家資訊
	static void AskPlayerInfo( const char* targetName );

	//-----------------------------------------------------------------------------------------------------
	//Cli to Cli 查詢玩家資訊
	virtual void R_CtoC_AskPlayerInfo( const char* AskerName );

	//-----------------------------------------------------------------------------------------------------
	//Cli to Cli 查詢玩家資訊結果
	virtual void R_CtoC_AskPlayerInfoResult( SAskPlayerInfo& PlayerInfo );

 	//-----------------------------------------------------------------------------------------------------
	virtual	void ReceiveToCliMessage(int messageType, const char* content);

  	//-----------------------------------------------------------------------------------------------------
	// Cli to Cli 設定團隊目標
	virtual void R_CtoC_SetRaidTarget( int TargetID , int IconID ); 
 
	//-----------------------------------------------------------------------------------------------------
	// Cli to Cli 追蹤玩家
	virtual void R_CtoC_TrackPlayer( const char* SenderName ); 

	//-----------------------------------------------------------------------------------------------------
	// Cli to Cli追蹤玩家 回報玩家座標
	virtual void R_CtoC_TrackPlayerResult( const char* Name, int WorldMapID, float fX, float fY, float fZ ); 

	//-----------------------------------------------------------------------------------------------------
	// Cli to Cli 收到玩家訊問邀請共乘坐騎
	virtual void R_CtoC_AskRideMount( const char* Name, int GItemID );

	//-----------------------------------------------------------------------------------------------------
	// Cli to Cli 收到回應塔乘坐騎
	virtual	void R_CtoC_RideMountResult(const char* Name, int answer);
  
 	//-----------------------------------------------------------------------------------------------------
	//邀請加入頻道
	static void ChannelInvite( const char* targetName, const char* channelName, const char* password );

	//-----------------------------------------------------------------------------------------------------
	// Cli to Cli 邀請加入頻道
	virtual void R_CtoC_ChannelInvite( const char* AskerName, const char* channelName, const char* password );




 	static const char *GetChannelLeader(int ChannelID);
protected:
	static	map<string, int>			s_channel;
	static	map<string, int>			s_zoneChannel;
	static	map<int, string>			s_channelLeader;
};

