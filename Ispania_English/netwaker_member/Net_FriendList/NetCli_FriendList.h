#pragma once

#include "PG/PG_FriendList.h"
#include "../../MainProc/NetGlobal.h"

class NetCli_FriendList : public CNetGlobal
{
protected:
    //-------------------------------------------------------------------
    static	NetCli_FriendList* This;
    static	bool _Init();
    static	bool _Release();	
    //-------------------------------------------------------------------
	/*
	static void _PG_Friend_ChattoC_BefriendRequest	( int Sockid , int Size , void* Data );	
	static void _PG_Friend_LtoC_AddFriend			( int Sockid , int Size , void* Data );		
	static void _PG_Friend_LtoC_DelFriend			( int Sockid , int Size , void* Data );		
	static void _PG_Friend_LtoC_FriendList			( int Sockid , int Size , void* Data );	


	static void _PG_Friend_ChattoC_PlayerOnline		( int Sockid , int Size , void* Data );
	static void _PG_Friend_ChattoC_PlayerOffline	( int Sockid , int Size , void* Data );
	static void _PG_Friend_ChattoC_ModifyInfo		( int Sockid , int Size , void* Data );
	static void _PG_Friend_LtoC_AddBadFriendResult	( int Sockid , int Size , void* Data );
	static void _PG_Friend_LtoC_DelBadFriendResult	( int Sockid , int Size , void* Data );
	
	static void _PG_Friend_CtoL_DelBadFriend		( int Sockid , int Size , void* Data );	
	static void _PG_Friend_ChattoC_BefriendAnswer	( int Sockid , int Size , void* Data );	
*/
	static void _PG_FriendList_LtoC_InsertFriendResult	( int Sockid , int Size , void* Data );	
	static void _PG_FriendList_ChattoC_PlayerOnlineInfo	( int Sockid , int Size , void* Data );	
	static void _PG_FriendList_ChattoC_PlayerOfflineInfo( int Sockid , int Size , void* Data );	
	static void _PG_FriendList_LtoC_EraseFriendResult	( int Sockid , int Size , void* Data );	
	static void _PG_FriendList_LtoC_ModifyFriendResult	( int Sockid , int Size , void* Data );	
	static void _PG_FriendList_LtoC_FriendListInfo_Zip	( int Sockid , int Size , void* Data );	
	static void _PG_FriendList_LtoC_FixFriendInfo		( int Sockid , int Size , void* Data );	
	static void _PG_Friend_LtoC_SearchFindPartyPlayerResult( int Sockid , int Size , void* Data );

	static void		OnEvent_ServerDisConnect		( int ServerID );
public:    
    NetCli_FriendList() { This = this; }

	static	NetCli_FriendList*	GetThis()						{ return This; }

	/*
	//要求和某人建立好友
	static void SChat_BefriendRequest	( char* Name );	
	static void SChat_BefriendResult	( char* Name , bool Result );
	static void SL_DelFriend			( char* Name , bool bBackGound = false );

	static void SL_ModifyMyInfo			( char* Note );
	static void SL_AddBadFriend			( char* Name, bool bCheckRole );
	static void SL_DelBadFriend			( char* Name );
	static void	SL_SetBadFriendInfo		( const char* Name, const char* Note );
	static void SL_SetFriendInfo		( const char* Name , const char* Group , const char* Note );


	virtual void RChat_BefriendRequest	( char* FriendName ) = 0;
	virtual void RL_AddFriend			( char* FriendName ) = 0;
	virtual void RL_DelFriend			( char* FriendName, bool bBackGound ) = 0;
	virtual void RL_FriendList			( FriendListStruct& FriendList ) = 0;

	virtual void RL_AddBadFriendResult	( const char* szFriendName, bool bResult ) = 0;
	virtual void RL_DelBadFriendResult	( const char* szFriendName, bool bResult ) = 0;
	

	virtual void RChat_PlayerOnline		( OnlinePlayerInfoStruct& FriendInfo ) = 0;
	virtual void RChat_PlayerOffline	( char* RoleName ) = 0;
	virtual void RChat_ModifyInfo		( OnlinePlayerInfoStruct& FriendInfo ) = 0;


	virtual void RL_BefriendAnswer		( int iResult, const char* FriendName ) = 0;

	
*/

	static void SL_InsertFriend			( FriendListTypeENUM Type , int Pos , BaseFriendStruct& Info );
	static void SL_EraseFriend			( FriendListTypeENUM Type , int DBID );
	static void SL_ModifyFriend			( FriendListTypeENUM Type , BaseFriendStruct& Info );
	static void SL_ModifyNote			( const char* Note );
	static void SL_FixClientData		( int Pos , char* CliData );
	static void SL_FixGroupSortID		( char	GroupSortID[ _MAX_FRIENDLIST_GROUP_COUNT_ ] );
	static void SL_SearchFindPartyPlayerRequest( Voc_ENUM MainJob , Voc_ENUM SubJob , int MinMainJobLv , int MaxMainJobLv , int MinSubJobLv , int MaxSubJobLv );

	virtual void OnServerDisConnect		( int ServerID ) = 0;
	virtual void RL_InsertFriendResult	( FriendListTypeENUM Type , int Pos , bool Result ) = 0;
	virtual void RL_EraseFriendResult	( FriendListTypeENUM Type , int DBID , bool Result ) = 0;
	virtual void RL_ModifyFriendResult	( FriendListTypeENUM Type , int DBID , bool Result ) = 0;

	virtual void RChat_PlayerOnlineInfo(  int PlayerDBID , char* RoleName , Race_ENUM Race , Voc_ENUM Voc , Voc_ENUM Voc_Sub , Sex_ENUM Sex
										, int LV , int LV_Sub , int GuildID , int TitleID ) = 0;
	virtual void RChat_PlayerOfflineInfo( int PlayerDBID ) = 0;

	virtual void RL_FriendListInfo_Zip	( FriendListStruct& FriendList ) = 0;
	virtual void RL_FixFriendInfo		( FriendListTypeENUM Type , int Pos , BaseFriendStruct& Info ) = 0;	
	virtual void RL_SearchFindPartyPlayerResult( vector<SearchFindPartyPlayerStruct>& PlayerInfo ) = 0;
};

