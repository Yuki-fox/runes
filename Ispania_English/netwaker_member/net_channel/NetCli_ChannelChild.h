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
	//����Cli ��e�� Cli����T
	virtual void R_ClitoCli_Data( const char* Data , int Size );

	virtual void R_CreateChannelOK( int ChannelID , const char* Name );
	virtual void R_CreateChannelFailed( const char* Name );
	virtual void R_JoinChannelOK( int ChannelID , const char* Name , int ChannelOwnerDBID , const char* ChannelOwnerName );
	virtual void R_JoinChannelFailed( const char* Name );
	virtual void R_LeaveChannelOK( int ChannelID );
	void removeChannel( int ChannelID);
	virtual void R_LeaveChannelFailed( int ChannelID );

	// �[�J�W�D
	virtual void RChat_AddUserNotify( GroupObjectTypeEnum Type , int ChannelID , char* Name );
	// �R���W�D
	virtual void RChat_DelUserNotify( GroupObjectTypeEnum Type , int ChannelID , char* Name );
	// �]�w�W�D�v���^��
	virtual void RChat_SetManagerResult( GroupObjectTypeEnum Type , int ChannelID , bool Result );
	// �����W�D�޲z��
	virtual void RChat_ChannelOwner( GroupObjectTypeEnum Type , int ChannelID , int OwnerDBID , char* OwnerName );

	virtual void R_ChannelPlayerListResult	( GroupObjectTypeEnum Type , int ChannelID , vector<string> &Name );
	virtual void R_JoinPlayerDisabledResult ( int ChannelID , GroupObjectTypeEnum Type , bool JoinEnabled );	

	//�t���W�D�C��
	virtual void R_SysChannel( GroupObjectTypeEnum Type , int ChannelID , char* Name );

	//�t�ήɶ�
	virtual void R_SysTime( int Time , int TimeZone );	

	virtual void RChat_PlayerNotFind( const char* Name );

	//--------------------------------------------------------------------------------------
	virtual void R_CtoC_Test( char* TestData );

	//-------------------------------------------------------------------
	//Cli to Cli��e�I���a�Ϫ��T��
	virtual void R_CtoC_MapPing( const char* pPingerName, int ZoneID, float fX, float fZ );
   
	//-----------------------------------------------------------------------------------------------------
	//Cli to Cli �n�D���oModleInfo
	virtual void GetModleInfoRequest( const char* pTargetName );
	virtual void R_CtoC_GetModleInfoRequest( const char* pRequesterName );

	//-----------------------------------------------------------------------------------------------------
	//Cli to Cli �Ǧ^���oModleInfo���G
	virtual void R_CtoC_GetModleInfoResult( int DBID , SModelInfo& ModelInfo );
 
	//-----------------------------------------------------------------------------------------------------
	//Cli to Cli �n�D���o�n�͸�T
	virtual void GiveMeYourFriendInfo( const char* pTargetName );
	virtual void R_CtoC_GiveMeYourFriendInfo( const char* SenderName );

	//-----------------------------------------------------------------------------------------------------
	//Cli to Cli �^���n�͸�T
	virtual void R_CtoC_GiveYouMyFriendInfo( const char* pName, SFriendInfo& FriendInfo );
  
	//-----------------------------------------------------------------------------------------------------
	//Cli to Cli �߰ݬO�_�O��誺�B��
 	virtual void AmIYourFriend( FriendListTypeENUM FriendListType, const char* AskerName, const char* AnswererName );
	virtual void R_CtoC_AmIYourFriend( FriendListTypeENUM FriendListType, const char* AskerName, const char* AnswererName );

	//-----------------------------------------------------------------------------------------------------
	//Cli to Cli �߰ݬO�_�O��誺�B�͵��G
	virtual void R_CtoC_AmIYourFriendResult( FriendListTypeENUM FriendListType, const char* AskerName, const char* AnswererName, bool Result );

	//-----------------------------------------------------------------------------------------------------
	//Cli to Cli �q�����Q�s�W�ΧR���B��
	virtual void YouAreMyFriend( FriendListTypeENUM FriendListType, const char* Name, bool bYes );

	//-------------------------------------------------------------------
	// Cli to Cli �T�����O�_���S�w���~
	virtual	void ReceiveToCliAskHasItem(const char* SendName, int itemVersion, int itemID);


	//-------------------------------------------------------------------
	// Cli to Cli �T�ݪ��a�O�_�n��|��
	virtual	void ReceiveToCliAskLeaderChange(const char* SendName);


	//-------------------------------------------------------------------
	// Cli to Cli�^�����a�O�_�n��|��
	virtual	void ReceiveToCliAskLeaderChangeResult(int dbid,bool YesNo);

	//-------------------------------------------------------------------
	//Cli to Cli ��e���a�O�_���S�w���~
	virtual	void ReceiveToCliHasItem(int itemVersion, int itemID, bool hasItem);

	//-----------------------------------------------------------------------------------------------------
protected:
	static string s_AskPlayerInfo_Name;
	static int s_AskPlayerInfo_LastTime;
public:
	//-----------------------------------------------------------------------------------------------------
	//�d�ߪ��a��T
	static void AskPlayerInfo( const char* targetName );

	//-----------------------------------------------------------------------------------------------------
	//Cli to Cli �d�ߪ��a��T
	virtual void R_CtoC_AskPlayerInfo( const char* AskerName );

	//-----------------------------------------------------------------------------------------------------
	//Cli to Cli �d�ߪ��a��T���G
	virtual void R_CtoC_AskPlayerInfoResult( SAskPlayerInfo& PlayerInfo );

 	//-----------------------------------------------------------------------------------------------------
	virtual	void ReceiveToCliMessage(int messageType, const char* content);

  	//-----------------------------------------------------------------------------------------------------
	// Cli to Cli �]�w�ζ��ؼ�
	virtual void R_CtoC_SetRaidTarget( int TargetID , int IconID ); 
 
	//-----------------------------------------------------------------------------------------------------
	// Cli to Cli �l�ܪ��a
	virtual void R_CtoC_TrackPlayer( const char* SenderName ); 

	//-----------------------------------------------------------------------------------------------------
	// Cli to Cli�l�ܪ��a �^�����a�y��
	virtual void R_CtoC_TrackPlayerResult( const char* Name, int WorldMapID, float fX, float fY, float fZ ); 

	//-----------------------------------------------------------------------------------------------------
	// Cli to Cli ���쪱�a�T���ܽЦ@�����M
	virtual void R_CtoC_AskRideMount( const char* Name, int GItemID );

	//-----------------------------------------------------------------------------------------------------
	// Cli to Cli ����^���𭼧��M
	virtual	void R_CtoC_RideMountResult(const char* Name, int answer);
  
 	//-----------------------------------------------------------------------------------------------------
	//�ܽХ[�J�W�D
	static void ChannelInvite( const char* targetName, const char* channelName, const char* password );

	//-----------------------------------------------------------------------------------------------------
	// Cli to Cli �ܽХ[�J�W�D
	virtual void R_CtoC_ChannelInvite( const char* AskerName, const char* channelName, const char* password );




 	static const char *GetChannelLeader(int ChannelID);
protected:
	static	map<string, int>			s_channel;
	static	map<string, int>			s_zoneChannel;
	static	map<int, string>			s_channelLeader;
};

