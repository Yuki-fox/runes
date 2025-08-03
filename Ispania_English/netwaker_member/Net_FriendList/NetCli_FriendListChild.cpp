#include "../../mainproc/GameMain.h"
#include "../../Interface/WorldFrames/ChatFrame.h"
#include "NetCli_FriendListChild.h"
#include "../../Interface/Debug/DebugMessage.h"
#include "../../interface/Socal/UI_Socal.h"
#include "../net_channel/NetCli_Channel.h"



//map< string, int >			NetCli_FriendListChild::m_mapFolderID;
//map< string, FriendInfo >		NetCli_FriendListChild::m_mapFriendInfo;
//-----------------------------------------------------------------------------
bool NetCli_FriendListChild::_Init()
{
	if (This)
		return false;

	This = new NetCli_FriendListChild;
	return NetCli_FriendList::_Init();
}

//-----------------------------------------------------------------------------
bool NetCli_FriendListChild::_Release()
{
	NetCli_FriendList::_Release();
	if (This)
	{
		delete This;
		This = NULL;
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////

void NetCli_FriendListChild::OnServerDisConnect		( int ServerID )
{

}

void NetCli_FriendListChild::RL_FixFriendInfo		( FriendListTypeENUM Type , int Pos , BaseFriendStruct& Info )
{
	g_pUi_Socal->RL_FixFriendInfo( Type, Pos, Info );
}

void NetCli_FriendListChild::RL_InsertFriendResult	( FriendListTypeENUM Type , int Pos , bool Result )
{
	if( !Result )
	{
		//error 不應該發生
		return;
	}

	//if( Type == EM_FriendListType_Friend )
	//{
	//	g_pInterface->SendWorldEvent( UiEvent_UpdateFriend );
	//}
	//else if( Type == EM_FriendListType_BadFriend )
	//{
	//	
	//}
	//else if( Type == EM_FriendListType_HateFriend )
	//{
	//	
	//}
	//else
	//{
	//	//error
	//	return;	
	//}

}

void NetCli_FriendListChild::RL_EraseFriendResult	( FriendListTypeENUM Type , int DBID , bool Result )
{
	//g_pInterface->SendWorldEvent(UiEvent_ResetFriend);

	if( !Result )
	{
		//error 不應該發生
		return;
	}

	//if( Type == EM_FriendListType_Friend )
	//{
	//	g_pInterface->SendWorldEvent( UiEvent_ResetFriend );
	//}
	//else if( Type == EM_FriendListType_BadFriend )
	//{

	//}
	//else if( Type == EM_FriendListType_HateFriend )
	//{

	//}
	//else
	//{
	//	//error
	//	return;	
	//}

}

void NetCli_FriendListChild::RL_ModifyFriendResult	( FriendListTypeENUM Type , int DBID , bool Result )
{

}

void NetCli_FriendListChild::RChat_PlayerOnlineInfo( int PlayerDBID , char* RoleName , Race_ENUM Race , Voc_ENUM Voc , Voc_ENUM Voc_Sub , Sex_ENUM Sex
													, int LV , int LV_Sub , int GuildID , int TitleID  )
{
	g_pUi_Socal->RChat_PlayerOnlineInfo( PlayerDBID , RoleName , Voc , Voc_Sub , Sex
										, LV , LV_Sub , GuildID , TitleID  );
}

void NetCli_FriendListChild::RChat_PlayerOfflineInfo( int PlayerDBID )
{
	g_pUi_Socal->RChat_PlayerOfflineInfo( PlayerDBID );
}

void NetCli_FriendListChild::RL_FriendListInfo_Zip( FriendListStruct& FriendList )
{
	RoleDataEx*			pRole		= CNetGlobal::RoleData();
	pRole->PlayerSelfData->FriendList = FriendList;

	g_pUi_Socal->FriendListLoadOK();
	
}
void NetCli_FriendListChild::RL_SearchFindPartyPlayerResult( vector<SearchFindPartyPlayerStruct>& PlayerInfo )
{
 	g_pUi_Socal->RL_SearchFindPartyPlayerResult( PlayerInfo );
}

//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
/*
NetCli_FriendListChild::NetCli_FriendListChild()
{

}
//-----------------------------------------------------------------------------
void NetCli_FriendListChild::RChat_BefriendRequest	( char* FriendName )
{
	char Buf[1024];
	sprintf( Buf , "[好友要請] : 好友 = %s" , FriendName );
	g_pDebugMessage->DebugOutput( 5, Buf );
	
	CUI_Socal::BeFriendRequest( FriendName );
}
//-----------------------------------------------------------------------------
void NetCli_FriendListChild::RL_AddFriend			( char* FriendName )
{
	char Buf[1024];
	sprintf( Buf , "[好友加入] : 好友 = %s" , FriendName );
	g_pDebugMessage->DebugOutput( 5, Buf );
	//g_pGameMain->SendSystemMsgEx( g_ObjectData->GetString( "SOCIAL_S_ADDFRIEND_SUCCESS" ) , FriendName );

	
	_RoleData.SelfData.FriendList.AddFriend( FriendName , "" , "" );
	
	int iFriendID = NetCli_FriendListChild::GetFriendID( FriendName );

	lua_State* L = g_pGameMain->GetInterface()->GetUi()->GetLuaState();

	// Friend ID
		lua_pushnumber( L, iFriendID );
		lua_setglobal( L, "arg1" );

	// Folder ID

		lua_pushnumber( L, 1 );
		lua_setglobal( L, "arg2" );

	// Friend Name
		lua_pushstring( L, (const char*)FriendName );
		lua_setglobal( L, "arg3" );


	// 加進資料
		NetCli_FriendListChild::ResetFolderInfo();

		// 分析好友資訊
		for( int i = 0 ; i < _MAX_FRIENDLIST_COUNT_ ; i++ )
		{
			BaseFriendStruct* pFriend = &( _RoleData.SelfData.FriendList.FriendList[i] );

			if( strcmp( pFriend->Name.Begin(), FriendName ) == 0 )
			{
				// 建立資料
				StructFriendInfo Info;

				Info.bOnline		= false;
				Info.Note			= pFriend->Note.Begin();

				CUI_Socal::m_mapFriendInfo[ pFriend->Name.Begin() ] = Info;
			}
		}

	g_pInterface->SendWorldEvent( UiEvent_AddFriend );
	//g_pGameMain->GetInterface()->GetUi()->SendEvent( UiEvent_ResetFriend );
}
//-----------------------------------------------------------------------------
void NetCli_FriendListChild::RL_DelFriend			( char* FriendName, bool bBackGround )
{
	char Buf[1024];
	sprintf( Buf , "[好友移除] : 好友 = %s" , FriendName );
	g_pDebugMessage->DebugOutput( 5, Buf );

	_RoleData.SelfData.FriendList.DelFriend( FriendName );
	
	CUI_Socal::m_mapFriendInfo.erase( FriendName );

	NetCli_FriendListChild::ResetFolderInfo();

	// 分析好友資訊
	for( int i = 0 ; i < _MAX_FRIENDLIST_COUNT_ ; i++ )
	{
		BaseFriendStruct* pFriend = &( _RoleData.SelfData.FriendList.FriendList[i] );

		if( strcmp( pFriend->Name.Begin(), FriendName ) == 0 )
		{
			CUI_Socal::m_mapFriendInfo.erase( pFriend->Name.Begin() );
		}
	}

	if( bBackGround )
	{
		g_pGameMain->GetInterface()->GetUi()->SendEvent( UiEvent_ResetFriend );
	}
	else
	{
		lua_State* L = g_pUi_Socal->GetLuaStateWorld();
		lua_pushstring( L, (const char*)FriendName );
		lua_setglobal( L, "arg1" );
		g_pInterface->SendWorldEvent( UiEvent_DelFriend );
	}

}

//-----------------------------------------------------------------------------
void NetCli_FriendListChild::RL_FriendList			( FriendListStruct& FriendList )
{
	_RoleData.SelfData.FriendList = FriendList;
	
	//NetCli_FriendListChild::ResetInfo();
	NetCli_FriendListChild::ResetFolderInfo();
	NetCli_FriendListChild::ResetFriendInfo();	
	NetCli_FriendListChild::ResetBadFriendInfo();

	g_pInterface->SendWorldEvent( UiEvent_ResetFriend );
}
//-----------------------------------------------------------------------------
//void NetCli_FriendListChild::RChat_PlayerOnline		( char* RoleName , char* Note , int ZoneID , LookStruct& Look , Voc_ENUM Voc , Voc_ENUM Voc_Sub , int LV , int LV_Sub , Sex_ENUM Sex )
void NetCli_FriendListChild::RChat_PlayerOnline		( OnlinePlayerInfoStruct& FriendInfo )
{
	char Buf[1024];

	if( CUI_Socal::m_mapFriendInfo.find( FriendInfo.Name.Begin() ) != CUI_Socal::m_mapFriendInfo.end() )
	{
		//sprintf( Buf , "[好友上線] : 好友 = %s ZoneID = %d Note = %s" , RoleName  , (FriendInfo.ZoneID / 1000) , "NO NOTE" );
		sprintf( Buf , "[好友上線] : 好友 = %s ZoneID = %d Note = %s" , FriendInfo.Name.Begin()  , (FriendInfo.ZoneID % 1000) , "NO NOTE" );
		g_pDebugMessage->DebugOutput( 5, Buf );

		StructFriendInfo	info		= CUI_Socal::m_mapFriendInfo[ FriendInfo.Name.Begin() ];

		info.bOnline			= true;
		info.ZoneID				= FriendInfo.ZoneID;
		info.Voc				= FriendInfo.Voc;
		info.VocLV				= FriendInfo.LV;
		info.Voc_Sub			= FriendInfo.Voc_Sub;
		info.Voc_SubLV			= FriendInfo.LV_Sub;
		info.Sex				= FriendInfo.Sex;
		info.TitleID			= FriendInfo.TitleID;
		//info.Note				= FriendInfo.Note;
		info.GuildID			= FriendInfo.GuildID;
		info.Introduection		= FriendInfo.Note.Begin();
		info.TitleStr			= FriendInfo.TitleStr.Begin();

		CUI_Socal::m_mapFriendInfo[ FriendInfo.Name.Begin() ] = info;
			
		int iFriendID = GetFriendID( FriendInfo.Name.Begin() );

		if( iFriendID != -1 )
		{
			BaseFriendStruct* pFriend = &( _RoleData.SelfData.FriendList.FriendList[ ( iFriendID - 1 ) ] );

			//pFriend->Name			= RoleName;
			//pFriend->Note			= Note;
		}

		

		// Friend Name
		lua_State* L = g_pUi_Socal->GetLuaStateWorld();
		lua_pushstring( L, FriendInfo.Name.Begin()  );
		lua_setglobal( L, "arg1" );

		g_pInterface->SendWorldEvent( UiEvent_OnlineFriend );

		{
			



			const char *name=FriendInfo.Name.Begin();
			
			g_pChatFrame->HaveNewWhisper(name,NULL,2);

		}

	


	}
	else
	{
		//不是好友 通知對方 移除
		NetCli_Channel::S_CtoC_IAmNotYourFriend( FriendInfo.Name , g_pGameMain->GetPlayerName() );
	}
}
//-----------------------------------------------------------------------------
void NetCli_FriendListChild::RChat_PlayerOffline	( char* RoleName )
{
	char Buf[1024];

	map< string, StructFriendInfo >::iterator it = CUI_Socal::m_mapFriendInfo.find( RoleName );


	if( it != CUI_Socal::m_mapFriendInfo.end() )
	{
		sprintf( Buf , "[好友下線] : 好友 = %s" , RoleName );
		g_pDebugMessage->DebugOutput( 5, Buf );

		//StructFriendInfo info = CUI_Socal::m_mapFriendInfo[ RoleName ];
		//info.bOnline = false;
		//CUI_Socal::m_mapFriendInfo[ RoleName ] = info;

		it->second.bOnline = false;

		g_pInterface->SendWorldEvent( UiEvent_ResetFriend );
	}
	else
	{
		//不是好友 通知對方 移除
		//NetCli_Channel::S_CtoC_IAmNotYourFriend( FriendInfo.Name , g_pGameMain->GetPlayerName() );
	}
}
//-----------------------------------------------------------------------------
void NetCli_FriendListChild::RChat_ModifyInfo		( OnlinePlayerInfoStruct& FriendInfo )
{
	char Buf[1024];
	sprintf( Buf , "[好友資料更新] : 好友 = %s" , FriendInfo.Name.Begin() ); //, ZoneID , Note );
	g_pDebugMessage->DebugOutput( 5, Buf );
	
	StructFriendInfo	info		= CUI_Socal::m_mapFriendInfo[ FriendInfo.Name.Begin() ];

	info.ZoneID				= FriendInfo.ZoneID;
	info.Voc				= FriendInfo.Voc;
	info.VocLV				= FriendInfo.LV;
	info.Voc_Sub			= FriendInfo.Voc_Sub;
	info.Voc_SubLV			= FriendInfo.LV_Sub;
	info.Sex				= FriendInfo.Sex;
	info.Introduection		= FriendInfo.Note.Begin();
	info.TitleID			= FriendInfo.TitleID;

	info.GuildID			= FriendInfo.GuildID;
	info.Introduection		= FriendInfo.Note.Begin();
	info.TitleStr			= FriendInfo.TitleStr.Begin();

	CUI_Socal::m_mapFriendInfo[ FriendInfo.Name.Begin() ] = info;

	int iFriendID = GetFriendID( FriendInfo.Name.Begin() );

	if( iFriendID != -1 )
	{

		BaseFriendStruct* pFriend = &( _RoleData.SelfData.FriendList.FriendList[ ( iFriendID - 1 ) ] );

		pFriend->Name			= FriendInfo.Name.Begin();
		//pFriend->Note			= FriendInfo.Note.Begin();
	}
	
	// 更新資料
	g_pInterface->SendWorldEvent( UiEvent_ResetFriend );
}
//-----------------------------------------------------------------------------
void NetCli_FriendListChild::OnServerDisConnect		( int ServerID )
{
	//Chat 斷線
	if( ServerID != _ServerNetID.m_iChat )
		return;

	for( int i = 0 ; i < _MAX_FRIENDLIST_COUNT_ ; i++ )
	{
		if( _RoleData.SelfData.FriendList.FriendList[i].IsEmpty() )
			continue;
		This->RChat_PlayerOffline( (char*)_RoleData.SelfData.FriendList.FriendList[i].Name.Begin() );
	}

}
//-----------------------------------------------------------------------------
int NetCli_FriendListChild::GetFriendID			( const char* RoleName )
{
	for( int i = 0 ; i < _MAX_FRIENDLIST_COUNT_ ; i++ )
	{
		if( strcmp( _RoleData.SelfData.FriendList.FriendList[i].Name.Begin(), RoleName ) == 0 )
		{
			return i+1;
		}
	}
	return -1;
}
//-----------------------------------------------------------------------------
void NetCli_FriendListChild::ResetFolderInfo()
{
	CUI_Socal::m_mapFolderID.clear();

	// 分析目錄及好友, 通知 Friend Frame 更新資料

	// 分析目錄結構
	int iFolderID = 2;
	for( int i = 0 ; i < _MAX_FRIENDLIST_COUNT_ ; i++ )
	{
		if( _RoleData.SelfData.FriendList.FriendList[i].IsEmpty() == false )
		{
			string sGroupName = _RoleData.SelfData.FriendList.FriendList[i].Group.Begin();

			if( strcmp( sGroupName.c_str(), "" ) == 0 || strcmp( sGroupName.c_str(), g_ObjectData->GetString( "SOCIAL_S_DEFAULTGROUP" ) ) == 0 )
			{
				StructFriendFolder folderInfo;

				folderInfo.iID		= 1;
				folderInfo.iExpand	= 1;

				_RoleData.SelfData.FriendList.FriendList[i].Group = g_ObjectData->GetString( "SOCIAL_S_DEFAULTGROUP" );

				CUI_Socal::m_mapFolderID[ _RoleData.SelfData.FriendList.FriendList[i].Group.Begin() ] = folderInfo;
			}
			else
			{
				StructFriendFolder	folderInfo;
				string				sGroupName = _RoleData.SelfData.FriendList.FriendList[i].Group.Begin();

				if( CUI_Socal::m_mapFolderID.find( sGroupName.c_str() ) == CUI_Socal::m_mapFolderID.end() )
				{					
					folderInfo.iID		= iFolderID;
					folderInfo.iExpand	= 1;

					CUI_Socal::m_mapFolderID[ sGroupName.c_str() ] = folderInfo;

					iFolderID++;

				}
				else
				{
					folderInfo.iID		= 1;
					folderInfo.iExpand	= 1;

					CUI_Socal::m_mapFolderID[ sGroupName.c_str() ] = folderInfo;
				}



				//NetCli_FriendListChild::m_mapFolderID[ _RoleData.SelfData.FriendList.FriendList[i].Group.Begin() ] = iFolderID;


			}
		}
	}
}
//-----------------------------------------------------------------------------
void NetCli_FriendListChild::ResetFriendInfo()
{

	CUI_Socal::m_mapFriendInfo.clear();
	//CUI_Socal::m_mapBadFriendInfo.clear();

	//NetCli_FriendListChild::ResetFolderInfo();

	// 分析目錄及好友, 通知 Friend Frame 更新資料
	// 分析好友資訊
	for( int i = 0 ; i < _MAX_FRIENDLIST_COUNT_ ; i++ )
	{
		BaseFriendStruct* pFriend = &( _RoleData.SelfData.FriendList.FriendList[i] );

		if( pFriend->IsEmpty() == false )
		{
			//lua_State* L = g_pGameMain->GetInterface()->GetUi()->GetLuaState();

			// 建立資料
			StructFriendInfo Info;

			Info.bOnline		= false;
			//Info.Introduection	= pFriend->Note.Begin();
			Info.Note			= pFriend->Note.Begin();

			CUI_Socal::m_mapFriendInfo[ pFriend->Name.Begin() ] = Info;

			// Friend ID
			//lua_pushnumber( L, ( i+1 ) );
			//lua_setglobal( L, "arg1" );

			// Folder ID
			int iFolderID = -1;
			//map< string, int >::iterator it = NetCli_FriendListChild::m_mapFolderID.find( pFriend->Group.Begin() );

			map< string, StructFriendFolder >::iterator it;

			it = CUI_Socal::m_mapFolderID.find( pFriend->Group.Begin() );

			if( it != CUI_Socal::m_mapFolderID.end() )
			{
				iFolderID = ( it->second ).iID;
			}
			else
			{
				iFolderID = 1;			
			}

			//lua_pushnumber( L, iFolderID );
			//lua_setglobal( L, "arg2" );

			//g_pGameMain->GetInterface()->GetUi()->SendEvent( UiEvent_AddFriend );
		}
	}



}
//-----------------------------------------------------------------------------
void NetCli_FriendListChild::ResetBadFriendInfo	()
{
	CUI_Socal::m_mapBadFriendInfo.clear();

	// 建立壞朋友清單
	for( int i = 0 ; i < _MAX_FRIENDLIST_COUNT_ ; i++ )
	{
		//BaseFriendStruct* pFriend = &( _RoleData.SelfData.FriendList.BadFriendList[i] );
		const char* pszName	= _RoleData.SelfData.FriendList.BadFriendList[i].Begin();

		if( strlen( pszName ) != 0 )
		{
			//lua_State* L = g_pGameMain->GetInterface()->GetUi()->GetLuaState();

			// 建立資料
			StructFriendInfo Info;

			Info.bOnline		= false;

			CUI_Socal::m_mapBadFriendInfo[ pszName ] = Info;
		}
	}


}
//-----------------------------------------------------------------------------
void NetCli_FriendListChild::RL_SearchFindPartyPlayerResult( vector<SearchFindPartyPlayerStruct>& PlayerInfo )
{
	// 測試用
	//for( unsigned i = 0 ; i < PlayerInfo.size() ; i++ )
	//{
	//	char Buf[1024];
	//	sprintf( Buf , "[搜尋組隊玩家] : Name = %s ZoneID = %d " , PlayerInfo[i].Name.Begin() , PlayerInfo[i].ZoneID );
	//	g_pDebugMessage->DebugOutput( 5, Buf );
	//}

	

	// 測試推假的
		CUI_Socal::m_vecSearchResult.clear();
		CUI_Socal::m_vecSearchResult = PlayerInfo;

	g_pInterface->SendWorldEvent( UiEvent_SearchGroupResult );
}
//-----------------------------------------------------------------------------
void NetCli_FriendListChild::RL_AddBadFriendResult	( const char* szFriendName, bool bResult )
{
	if( bResult )
	{
		_RoleData.SelfData.FriendList.AddBadFriend( (char*)szFriendName );

		// 加進資料
		//NetCli_FriendListChild::ResetInfo();
		NetCli_FriendListChild::ResetBadFriendInfo();

		g_pInterface->SendWorldEvent( UiEvent_RESET_BAD_FRIEND );

  		g_pGameMain->SendSystemMsgEx( g_ObjectData->GetString( "SOCIAL_S_ADDBADFRIEND_SUCCESS" ) , szFriendName );
 	}
	else
	{
		g_pGameMain->SendSystemMsgEx( g_ObjectData->GetString( "SOCIAL_S_ADDBADFRIEND_FAILED" ) , szFriendName );
	}

}
//-----------------------------------------------------------------------------
void NetCli_FriendListChild::RL_DelBadFriendResult	( const char* szFriendName, bool bResult )
{
	_RoleData.SelfData.FriendList.DelBadFriend( (char*)szFriendName );

	NetCli_FriendListChild::ResetBadFriendInfo();

	g_pInterface->SendWorldEvent( UiEvent_RESET_BAD_FRIEND );
}
//-----------------------------------------------------------------------------
void NetCli_FriendListChild::RL_BefriendAnswer ( int iResult, const char* FriendName )
{
	switch( iResult )
	{
	case 0:
		{

			char Buf[1024];
			sprintf( Buf , g_ObjectData->GetString( "SOCIAL_S_ADDFRIEND_NOOWNER" ) );
			g_pChatFrame->SendMsgEvent( CHAT_MSG_SYSTEM, Buf, "" );			
			g_pGameMain->SendSystemMsg( Buf );
		} break;

	case 1:
		{

			g_pGameMain->SendSystemMsgEx( g_ObjectData->GetString( "SOCIAL_S_ADDFRIEND_REFUSE" ) , FriendName );
		} break;

	}

}
*/
