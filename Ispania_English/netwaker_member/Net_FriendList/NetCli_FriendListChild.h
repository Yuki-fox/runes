#pragma once

#include "NetCli_FriendList.h"


/*
struct FriendList {
	string		group;
	string		note;
	int			zoneId;
	LookStruct	semblance;
	Voc_ENUM	voc;
	Voc_ENUM	vocSub;
	Sex_ENUM	sex;
};

struct FriendInfo
{
	FriendInfo() { bOnline = false; }
	
	bool		bOnline;
	int			ZoneID;
	LookStruct	Look;
	Voc_ENUM	Voc;
	int			VocLV;
	Voc_ENUM	Voc_Sub;
	int			Voc_SubLV;
	Sex_ENUM	Sex;
	string		Introduection;
};
*/

class NetCli_FriendListChild : public NetCli_FriendList
{
public:	
	static	bool _Init();
	static	bool _Release();

public:

	virtual void OnServerDisConnect		( int ServerID );

	virtual void RL_InsertFriendResult	( FriendListTypeENUM Type , int Pos , bool Result );
	virtual void RL_EraseFriendResult	( FriendListTypeENUM Type , int DBID , bool Result );
	virtual void RL_ModifyFriendResult	( FriendListTypeENUM Type , int DBID , bool Result );

	virtual void RChat_PlayerOnlineInfo(  int PlayerDBID , char* RoleName , Race_ENUM Race , Voc_ENUM Voc , Voc_ENUM Voc_Sub , Sex_ENUM Sex
										, int LV , int LV_Sub , int GuildID , int TitleID );
	virtual void RChat_PlayerOfflineInfo( int PlayerDBID );

	virtual void RL_FriendListInfo_Zip( FriendListStruct& FriendList );
	virtual void RL_FixFriendInfo		( FriendListTypeENUM Type , int Pos , BaseFriendStruct& Info );	
	virtual void RL_SearchFindPartyPlayerResult( vector<SearchFindPartyPlayerStruct>& PlayerInfo );
	/*
	virtual void RChat_BefriendRequest	( char* FriendName );
	virtual void RL_AddFriend			( char* FriendName );
	virtual void RL_DelFriend			( char* FriendName, bool bBackGround );
	virtual void RL_FriendList			( FriendListStruct& FriendList );
	virtual void RL_AddBadFriendResult	( const char* szFriendName, bool bResult );
	virtual void RL_DelBadFriendResult	( const char* szFriendName, bool bResult );

	virtual void RChat_PlayerOnline		( OnlinePlayerInfoStruct& Info );
	virtual void RChat_PlayerOffline	( char* RoleName );
	virtual void RChat_ModifyInfo		( OnlinePlayerInfoStruct& Info );
	
	virtual void RL_BefriendAnswer		( int iResult, const char* FriendName );

	///////////////////////////////////////////////////////////////////////////////

	static int		GetFriendID			( const char* RoleName );

	
	static void		ResetFolderInfo		();
	static void		ResetFriendInfo		();
	static void		ResetBadFriendInfo	();

	//static map< string, int >			m_mapFolderID;
	//static map< string, FriendInfo >	m_mapFriendInfo;
	*/
};

