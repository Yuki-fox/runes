#include "../../mainproc/GameMain.h"					// For Obj and Role struct
#include "../../netwaker_member/NetWakerClientInc.h"	// For NetGlobal

//#include "../WorldFrames/ItemClipboard.h"
#include "../../netwaker_member/Net_Guild/NetCli_GuildChild.h"
#include "../../netwaker_member/net_channel/NetCli_Channel.h"

#include "../WorldFrames/ChatFrame.h"
#include "../map/MapCommon.h"

#include "../WorldFrames/GuildFrame.h"

#include "../Common/Com_Function.h"


#include "./UI_Socal.h"

#include <stdlib.h>


#define DF_RELATION_GROUP_ID 1000000




//map< string, StructFriendFolder >		CUI_Socal::m_mapFolderID;
//map< int, StructFriendInfo >			CUI_Socal::m_mapFriendInfo;
//map< string, StructFriendInfo >			CUI_Socal::m_mapBadFriendInfo;
vector< SearchFindPartyPlayerStruct >	CUI_Socal::m_vecSearchResult;
// 全域宣告
//-------------------------------------------------------------------
CUI_Socal* g_pUi_Socal = NULL;

//#define DF_Socal_Token_Friend     "Friend"    
//#define DF_Socal_Token_BadFriend  "BadFriend" 
//#define DF_Socal_Token_HateFriend "HateFriend"


const char* DF_Socal_Token_Friend     = "Friend"     ;
const char* DF_Socal_Token_BadFriend  = "BadFriend"  ;
const char* DF_Socal_Token_HateFriend = "HateFriend" ;

//-------------------------------------------------------------------
bool Func_GetSocalUnitType( const char* szType, ESocalUnitType& rType )
{
	if( strcmp( szType , DF_Socal_Token_Friend ) == 0 )
	{
		rType = SUT_Friend;
	}
	else if( strcmp( szType , DF_Socal_Token_BadFriend ) == 0 )
	{
		rType = SUT_BadFriend;
	}
	else if( strcmp( szType , DF_Socal_Token_HateFriend ) == 0 )
	{
		rType = SUT_HateFriend;
	}
	else
	{
		return false;
	}
	return true;
}


//-------------------------------------------------------------------
//int			LuaFunc_GetNumSkill_SkillBook				( lua_State *L );

/*
int			LuaFunc_GetSkillPoint_SkillBook				( lua_State *L );
int			LuaFunc_SkillUp_SkillBook					( lua_State *L );
int			LuaFunc_UseSkill_SkillBook					( lua_State *L );
int			LuaFunc_OpenSkillList_SkillBook				( lua_State *L );
int			LuaFunc_GetSkillInfo_SkillList				( lua_State *L );
int			LuaFunc_GetNumMaxSkillInfo_SkillList		( lua_State *L );


*/

int	LuaFunc_AddFriend				(lua_State* L);
int	LuaFunc_DelFriend				(lua_State* L);


int LuaFunc_GetFriendCount			(lua_State* L);
int	LuaFunc_GetFriendInfo			(lua_State* L);
int LuaFunc_SetFriendGroup			(lua_State* L);

int LuaFunc_GetSocalGroupCount		(lua_State* L);
int	LuaFunc_GetSocalGroupInfo		(lua_State* L);
int	LuaFunc_AddSocalGroup			(lua_State* L);
int	LuaFunc_DelSocalGroup			(lua_State* L);
int LuaFunc_ModifySocalGroupName	(lua_State* L);
int LuaFunc_ModifySocalGroupSort	(lua_State* L);

int LuaFunc_SetFriendTop			(lua_State* L);

int LuaFunc_GetFriendDetailRequest	(lua_State* L);
int LuaFunc_GetFriendDetail			(lua_State* L);

int	LuaFunc_SearchGroupPeople		(lua_State* L);
//int	LuaFunc_SearchGroupFlag			(lua_State* L);
int	LuaFunc_GetSearchGroupResult	(lua_State* L);
 
int LuaFunc_GetSystemString			(lua_State* L);
int luaFunc_SetFriendModel			(lua_State* L);

int luaFunc_IsMyFriend				(lua_State* L);



int luaFunc_AskPlayerInfo			(lua_State* L); 


//-------------------------------------------------------------------
CUI_Socal::CUI_Socal( CInterface* object ) : CInterfaceBase( object )
{
	g_pUi_Socal = this;
	m_IsDisablePVPRule = false;
	m_FriendListLoadOK = false;
}
// --------------------------------------------------------------------------------------
CUI_Socal::~CUI_Socal(void)
{
	g_pUi_Socal	= NULL;
}
// --------------------------------------------------------------------------------------
void CUI_Socal::RegisterFunc(void)
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{
		lua_register(L, "AddFriend",				LuaFunc_AddFriend				);
		lua_register(L, "DelFriend",				LuaFunc_DelFriend				);


 		lua_register(L, "GetFriendCount",			LuaFunc_GetFriendCount			);
		lua_register(L, "GetFriendInfo",			LuaFunc_GetFriendInfo			);

		lua_register(L, "SetFriendGroup",			LuaFunc_SetFriendGroup			);
   		lua_register(L, "SetFriendTop",				LuaFunc_SetFriendTop			);

		lua_register(L, "GetSocalGroupCount",		LuaFunc_GetSocalGroupCount		);
		lua_register(L, "GetSocalGroupInfo",		LuaFunc_GetSocalGroupInfo		);
	
		lua_register(L, "AddSocalGroup",			LuaFunc_AddSocalGroup			);
		lua_register(L, "DelSocalGroup",			LuaFunc_DelSocalGroup			);
  
    	lua_register(L, "ModifySocalGroupName",		LuaFunc_ModifySocalGroupName	);
       	lua_register(L, "ModifySocalGroupSort",		LuaFunc_ModifySocalGroupSort	);
  
		lua_register(L, "GetFriendDetailRequest",	LuaFunc_GetFriendDetailRequest	);
		lua_register(L, "GetFriendDetail",			LuaFunc_GetFriendDetail			);

	
		// 組隊系統
		lua_register(L, "SearchGroupPeople",		LuaFunc_SearchGroupPeople		);

		//lua_register(L, "SearchGroupFlag",			LuaFunc_SearchGroupFlag			);

		// 組隊搜尋
		lua_register(L, "GetSearchGroupResult",		LuaFunc_GetSearchGroupResult	);

		// 取得系統字串
		lua_register(L, "GetSystemString",			LuaFunc_GetSystemString			);

		lua_register(L, "SetFriendModel",			luaFunc_SetFriendModel			);

		lua_register(L, "IsMyFriend",				luaFunc_IsMyFriend				);

		lua_register(L, "AskPlayerInfo",			luaFunc_AskPlayerInfo			);

	}
}



// ----------------------------------------------------------------------------
void CUI_Socal::Update( float elapsedTime )
{
	UpdateAddFriendList();


	std::list<string>::iterator itPos = m_SystemMsgList.begin();
	//std::list<string>::iterator itEnd = m_SystemMsgList.end();

	if( itPos != m_SystemMsgList.end() )
	{
		string& msg = *itPos;
		g_pGameMain->SendSystemChatMsg( msg.c_str() );
		m_SystemMsgList.pop_front();
	}
	
	//for( ; itPos != itEnd ; ++itPos )
	//{
	//	string& msg = *itPos;

	//	g_pGameMain->SendSystemChatMsg( msg.c_str() );
	//}


}

//--------------------------------------------------------------------------------------
void CUI_Socal::DestroyFrames()
{
	m_AddFriendList.clear();
	m_mapFriendDetail.clear();
	m_mapModelInfo.clear();

	//返回腳色選單 清除
	if( !g_pInterface->IsReloadUI() )
	{
		m_PlayerOnlineSet.clear();
	}

	m_FriendListLoadOK = false;
}

//--------------------------------------------------------------------------------------
void CUI_Socal::LeaveWorld()
{
	m_FriendListLoadOK = false;
}

// ----------------------------------------------------------------------------
void CUI_Socal::_AddFriend ( int DBID, const char* Name, int groupID )
{
	FriendListStruct& FriendList = CNetGlobal::RoleData()->PlayerSelfData->FriendList;

	BaseFriendStruct* pList = NULL;
	int MaxCount = 0;
	FriendList.GetListInfo( EM_FriendListType_Friend, &pList, MaxCount );
	if( !pList )
	{
		return ;
	}
	
	//
	if( FriendList.GroupName[0][0] == '\0' )
	{
		strcpy( FriendList.GroupName[0] , g_ObjectData->GetString( "SYS_SOCIAL_DEFAULT_GROUP" ) );
		NetCli_FriendList::SL_FixClientData( _MAX_BAD_FRIENDLIST_COUNT_ + 0 , FriendList.GroupName[0] );
		CNetGlobal::RoleData()->PlayerSelfData->FriendList.GroupSortID[0] = 0;
		NetCli_FriendList::SL_FixGroupSortID( CNetGlobal::RoleData()->PlayerSelfData->FriendList.GroupSortID );
	}


	bool isFind = false;

	int i;
	for( i = 0 ; i < MaxCount ; ++i )
	{
		if( pList[i].DBID == DBID )
		{
			//新增好友失敗，該玩家已在好友名單中
			g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString( "SYS_SOCIAL_ADDFRIEND_FAIL3" ) );
			return;
		}
		if( pList[i].IsEmpty() )
		{
			pList[i].Clear();
			pList[i].DBID = DBID;
			strcpy( pList[i].Name, Name );
			pList[i].GroupID = groupID;

			NetCli_FriendList::SL_InsertFriend( EM_FriendListType_Friend, i, pList[i] );

			g_pNetCli_ChannelChild->YouAreMyFriend( EM_FriendListType_Friend, Name, true );

			isFind = true;
			break;
		}
	}


	if( !isFind )
	{	
		//好友已滿
		g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString( "SYS_SOCIAL_ADDFRIEND_FAIL1" ) );
	}
	else
	{
		g_pGameMain->SendSystemChatMsgEx( g_ObjectData->GetString( "SYS_SOCIAL_ADDFRIEND" ) , Name );
	}
  
}

// ----------------------------------------------------------------------------
void CUI_Socal::_AddBadFriend ( int DBID, const char* Name )
{
	FriendListStruct& FriendList = CNetGlobal::RoleData()->PlayerSelfData->FriendList;

	if( NULL == Name || strlen( Name ) > _MAX_NAMERECORD_SIZE_ - 1 )
	{
		return;
	}
   
	bool isFind = false;
	for( int i = 0 ; i < _MAX_BAD_FRIENDLIST_COUNT_ ; ++i )
	{
		if( stricmp( FriendList.BadFriendList[i], Name ) == 0 )
		{
			//新增黑名單失敗，該玩家已在黑名單中
			g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString( "SYS_SOCIAL_ADDBADFRIEND_FAIL3" ) );
			return;
		}

		if( FriendList.BadFriendList[i][0] == '\0' )
		{
			strcpy( FriendList.BadFriendList[i] , Name );
			NetCli_FriendList::SL_FixClientData( i , FriendList.BadFriendList[i] );

			isFind = true;
			break;
		}
	}

	if( !isFind )
	{	
		//黑名單已滿
		g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString( "SYS_SOCIAL_ADDBADFRIEND_FAIL1" ) );
	}
	else
	{
		g_pGameMain->SendSystemChatMsgEx( g_ObjectData->GetString( "SYS_SOCIAL_ADDBADFRIEND" ) , Name );
	}

}

// ----------------------------------------------------------------------------
void CUI_Socal::_AddHateFriend( int DBID, const char* Name )
{
	FriendListStruct& FriendList = CNetGlobal::RoleData()->PlayerSelfData->FriendList;

	BaseFriendStruct* pList = NULL;
	int MaxCount = 0;
	FriendList.GetListInfo( EM_FriendListType_HateFriend, &pList, MaxCount );
	if( !pList )
	{
		return ;
	}
	
	BaseFriendStruct Info;
	Info.Clear();
	Info.DBID = DBID;
	strcpy( Info.Name, Name );

	for( int i = 0 ; i < MaxCount ; ++i )
	{
		if( pList[i].DBID == Info.DBID  )
		{
			Info = pList[i];
 			//移除
			FriendList.Erase( EM_FriendListType_HateFriend, Info.DBID );
			NetCli_FriendList::SL_EraseFriend( EM_FriendListType_HateFriend, Info.DBID );
			break;
		}
	}
   
	//尋找可以插入的位置
	bool isFind = false;
	for( int i = 0 ; i < MaxCount ; ++i )
	{
		if( pList[i].IsEmpty() || pList[i].top == false  )
		{
			NetCli_FriendList::SL_InsertFriend( EM_FriendListType_HateFriend, i, Info );
			FriendList.Insert( EM_FriendListType_HateFriend, i, Info );
			isFind = true;
			break;
		}
	}

	//已滿
	if( !isFind )
	{
  		g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString( "SYS_SOCIAL_ADDHATEFRIEND_FAIL1" ) );
	}
	else
	{
		g_pGameMain->SendSystemChatMsgEx( g_ObjectData->GetString( "SYS_SOCIAL_ADDHATEFRIEND" ) , Name );
	}
}

// ----------------------------------------------------------------------------
void CUI_Socal::UpdateAddFriendList( )
{
	if( m_AddFriendList.empty() )
	{
		return;
	}
	std::list<SAddFriend>::iterator itPos = m_AddFriendList.begin();
	std::list<SAddFriend>::iterator itEnd = m_AddFriendList.end();

	for( ; itPos != itEnd ; ++ itPos )
	{
		SAddFriend& sAddFriend = *itPos;

		FindDBIDRoleNameStruct* findStruct = NetCli_OtherChild::GetFindDBIDRoleNameStruct( sAddFriend.Name.c_str() );

		if( !findStruct )
			continue;

		if( findStruct->IsFind )
		{
			switch( sAddFriend.Type )
			{
			case SUT_Friend:
				_AddFriend( findStruct->DBID, findStruct->RoleName.c_str(), sAddFriend.GroupID );
				break;
			case SUT_BadFriend:
				_AddBadFriend( findStruct->DBID, findStruct->RoleName.c_str() );
				break;
			case SUT_HateFriend:
				_AddHateFriend( findStruct->DBID, findStruct->RoleName.c_str() );
			    break;
			default:
				//error
			    break;
			}

			g_pInterface->SendWorldEvent( UiEvent_ResetFriend );
		}
		else
		{
			switch( sAddFriend.Type )
			{
			case SUT_Friend:
				g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString( "SYS_SOCIAL_ADDFRIEND_FAIL2" ) );
				break;
			case SUT_BadFriend:
				g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString( "SYS_SOCIAL_ADDBADFRIEND_FAIL2" ) );
				break;
			case SUT_HateFriend:
				g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString( "SYS_SOCIAL_ADDHATEFRIEND_FAIL2" ) );
				break;
			default:
				//error
				break;
			}

		}

		m_AddFriendList.erase( itPos );
		break;
	}
}

void CUI_Socal::SetFriendModel( int nFriendID, const char* szUIModel )
{
/*
	if( nFriendID < 0 && nFriendID >= _MAX_FRIENDLIST_COUNT_ )
	{
		return;
	}

	BaseFriendStruct* pFriend =  &( CNetGlobal::RoleData()->PlayerSelfData->FriendList.FriendList[ nFriendID ] );
	if( pFriend->IsEmpty() )
	{
		return;
	}

	CUiModel* pUiModel = dynamic_cast<CUiModel*>( m_interface->GetUiWorld()->FindObject( szUIModel ) );	
	if( !pUiModel )
	{
		return;
	}

	StructFriendInfo info = m_mapFriendInfo[ pFriend->DBID ];
	if( !info.bOnline )
	{
		pUiModel->SetVisible( false );
		return;	
	}


	pUiModel->CreateEntity( NULL );

	m_FriendModel_FriendName = pFriend->Name;
	m_FriendModel_UIModel    = szUIModel;

	//test
	//SModelInfo ModelInfo;
	//if( GetSelfModelInfo( ModelInfo ) )
	//{
	//	R_FriendModleInfoResult( pFriend->Name.Begin() , ModelInfo );
	//}

	g_pNetCli_ChannelChild->GiveMeYourFriendInfo( pFriend->Name );
*/
}

void CUI_Socal::AddFriend( ESocalUnitType type, const char* name , int groupID )
{
	if( stricmp( g_pGameMain->GetPlayerName() , name ) == 0 )
		return;

	SAddFriend s;
	s.Type = type;
	s.Name = name;
	s.GroupID = groupID;
	m_AddFriendList.push_back(s);
}

void CUI_Socal::DelFriend( ESocalUnitType type, const char* name, RelationTypeENUM relationType )
{

	if( type == SUT_Friend )
	{

		if( relationType == EM_RelationType_Student    || 
			relationType == EM_RelationType_Teacher    ||
			relationType == EM_RelationType_Wife       ||
			relationType == EM_RelationType_Husband    ||
			relationType == EM_RelationType_Lover      ||
			relationType == EM_RelationType_Family     ||
			relationType == EM_RelationType_BestFriend ||
			relationType == EM_RelationType_BadCompany )
		{
			BaseFriendStruct* pList = NULL;
			int MaxCount = 0;
			CNetGlobal::RoleData()->PlayerSelfData->FriendList.GetListInfo( EM_FriendListType_FamilyFriend, &pList, MaxCount );
			if( !pList )
			{
				return ;
			}

			for( int i = 0 ; i < MaxCount ; ++i )
			{
				if( stricmp( pList[i].Name, name ) == 0 )
				{
					NetCli_FriendList::SL_EraseFriend( EM_FriendListType_FamilyFriend, pList[i].DBID );
					CNetGlobal::RoleData()->PlayerSelfData->FriendList.Erase( EM_FriendListType_FamilyFriend, pList[i].DBID );

					g_pNetCli_ChannelChild->YouAreMyFriend( EM_FriendListType_FamilyFriend, name, false );
					break;
				}
			}
		}
		else
		{
			BaseFriendStruct* pList = NULL;
			int MaxCount = 0;
			CNetGlobal::RoleData()->PlayerSelfData->FriendList.GetListInfo( EM_FriendListType_Friend, &pList, MaxCount );
			if( !pList )
			{
				return ;
			}

			for( int i = 0 ; i < MaxCount ; ++i )
			{
				if( stricmp( pList[i].Name, name ) == 0 )
				{
					NetCli_FriendList::SL_EraseFriend( EM_FriendListType_Friend, pList[i].DBID );
					CNetGlobal::RoleData()->PlayerSelfData->FriendList.Erase( EM_FriendListType_Friend, pList[i].DBID );

					g_pNetCli_ChannelChild->YouAreMyFriend( EM_FriendListType_Friend, name, false );
					break;
				}
			}

		}

	}
	else if( type == SUT_BadFriend )
	{
		FriendListStruct& FriendList = CNetGlobal::RoleData()->PlayerSelfData->FriendList;

		int i;
		int end = _MAX_BAD_FRIENDLIST_COUNT_ - 1;
		bool IsFind = false;
		for( i = 0 ; i < end ; i++ )
		{
			if( IsFind && FriendList.BadFriendList[i][0] == '\0' )
				break;

			if( IsFind == false && stricmp( FriendList.BadFriendList[i], name ) != 0 )
				continue;

			IsFind = true;

			NetCli_FriendList::SL_FixClientData( i , FriendList.BadFriendList[i+1] );
			strcpy( FriendList.BadFriendList[i], FriendList.BadFriendList[i+1] );

		}

		if( i == end )
		{
			NetCli_FriendList::SL_FixClientData( i , "" );
			FriendList.BadFriendList[i][0] = '\0';
		}

	}
	else if( type == SUT_HateFriend )
	{
		BaseFriendStruct* pList = NULL;
		int MaxCount = 0;
		CNetGlobal::RoleData()->PlayerSelfData->FriendList.GetListInfo( EM_FriendListType_HateFriend, &pList, MaxCount );
		if( !pList )
		{
			return ;
		}

		for( int i = 0 ; i < MaxCount ; ++i )
		{
			if( stricmp( pList[i].Name, name ) == 0 )
			{
				NetCli_FriendList::SL_EraseFriend( EM_FriendListType_HateFriend, pList[i].DBID );
				CNetGlobal::RoleData()->PlayerSelfData->FriendList.Erase( EM_FriendListType_HateFriend, pList[i].DBID );
				break;
			}
		}
	}

	g_pInterface->SendWorldEvent( UiEvent_ResetFriend );
}

void CUI_Socal::AddSocalGroup( ESocalUnitType type, const char* Name )
{
	if( type != SUT_Friend )
	{
		return;
	}

	FriendListStruct& FriendList = CNetGlobal::RoleData()->PlayerSelfData->FriendList;

	int i;
	for( i = 0 ; i < _MAX_FRIENDLIST_GROUP_COUNT_ ; ++i )
	{
 		if( FriendList.GroupName[i][0] == '\0' )
		{
			strcpy( FriendList.GroupName[i], Name );
			NetCli_FriendList::SL_FixClientData( _MAX_BAD_FRIENDLIST_COUNT_ + i , FriendList.GroupName[i] );
			ModifyGroupSort( type, i, 11 );
			break;
		}
	}

	//群組已滿
}

void CUI_Socal::DelSocalGroup( ESocalUnitType type, int groupID )
{
	if( type != SUT_Friend )
	{
		return;
	}

	if( groupID < 0 || groupID >= _MAX_FRIENDLIST_GROUP_COUNT_ )
	{
		return;
	}

	FriendListStruct& FriendList = CNetGlobal::RoleData()->PlayerSelfData->FriendList;

	
	//將群組下的人搬到0
	BaseFriendStruct* pList = NULL;
	int MaxCount = 0;
	FriendList.GetListInfo( EM_FriendListType_Friend, &pList, MaxCount );
	if( !pList )
	{
		return ;
	}
  	for( int i = 0 ; i < MaxCount ; ++i )
	{
		if( pList[i].GroupID ==  groupID )
		{
			pList[i].GroupID = 0;
			NetCli_FriendList::SL_ModifyFriend( EM_FriendListType_Friend, pList[i] );
		}
	}

  
	FriendList.GroupName[groupID][0] = '\0';
	NetCli_FriendList::SL_FixClientData( _MAX_BAD_FRIENDLIST_COUNT_ + groupID , FriendList.GroupName[groupID] );

	ModifyGroupSort( type, groupID, 10000 );
}

void CUI_Socal::ModifyGroupName( ESocalUnitType type, int groupID, const char* newName )
{
	if( type != SUT_Friend )
	{
		return;
	}
	
	if( groupID < 0 || groupID >= _MAX_FRIENDLIST_GROUP_COUNT_ )
	{
		return;
	}

	if( ( strlen( newName ) + 1 ) > _MAX_NAMERECORD_SIZE_ )
	{
		return;
	}

	FriendListStruct& FriendList = CNetGlobal::RoleData()->PlayerSelfData->FriendList;

	strcpy( FriendList.GroupName[groupID], newName );
	NetCli_FriendList::SL_FixClientData( _MAX_BAD_FRIENDLIST_COUNT_ + groupID , FriendList.GroupName[groupID] );

	g_pInterface->SendWorldEvent( UiEvent_ResetFriend );
}

void CUI_Socal::ModifyGroupSort( ESocalUnitType type, int groupID, int sort )
{
	FriendListStruct& FriendList = CNetGlobal::RoleData()->PlayerSelfData->FriendList;


	multimap<int,int> sortmap;   //sort , index

	int i;
	for( i = 0 ; i < _MAX_FRIENDLIST_GROUP_COUNT_ ; ++i )
	{
		if( FriendList.GroupName[i][0] == '\0' )
		{
			sortmap.insert( multimap<int,int>::value_type( 10000 , i ) );
			continue;
		}

		if( i == groupID )
		{
			sortmap.insert( multimap<int,int>::value_type( sort * 10 - 1 , i ) );
			continue;
		}

		sortmap.insert( multimap<int,int>::value_type( FriendList.GroupSortID[i] * 10 , i ) );
	}

	//重新替所有排序
	int newsort = 0;

	multimap<int,int>::iterator itPos = sortmap.begin();
 	multimap<int,int>::iterator itEnd = sortmap.end();

	for( ; itPos != itEnd ; ++itPos )
	{
		FriendList.GroupSortID[ itPos->second ] = newsort;
		++newsort;
	}

	NetCli_FriendList::SL_FixGroupSortID( FriendList.GroupSortID );
	g_pInterface->SendWorldEvent( UiEvent_ResetFriend );
}

void CUI_Socal::PlayerKillMe( int DBID, const char* Name )
{
	if( m_IsDisablePVPRule )
	{
		return;
	}

	if( strcmp( g_pGameMain->GetPlayerName() , Name ) == 0 )
	{
		return;
	}

	FriendListStruct& FriendList = CNetGlobal::RoleData()->PlayerSelfData->FriendList;

	BaseFriendStruct* pList = NULL;
	int MaxCount = 0;
	FriendList.GetListInfo( EM_FriendListType_HateFriend, &pList, MaxCount );
	if( !pList )
	{
		return ;
	}
  
	for( int i = 0 ; i < MaxCount ; ++i )
	{
		if( pList[i].DBID == DBID  )
		{
			pList[i].KillMeCount++ ;
   			NetCli_FriendList::SL_ModifyFriend( EM_FriendListType_HateFriend, pList[i] );
			g_pInterface->SendWorldEvent( UiEvent_ResetFriend );
   			return;
		}
	}

//	SAddFriend s;
//	s.Type = SUT_HateFriend;
//	s.Name = Name;
//	s.GroupID = 0;
//	m_AddFriendList.push_back(s);
	_AddHateFriend( DBID, Name );

	for( int i = 0 ; i < MaxCount ; ++i )
	{
		if( pList[i].DBID == DBID  )
		{
			pList[i].KillMeCount++ ;
			NetCli_FriendList::SL_ModifyFriend( EM_FriendListType_HateFriend, pList[i] );
			g_pInterface->SendWorldEvent( UiEvent_ResetFriend );
			return;
		}
	}

}

void CUI_Socal::KillPlayer( int DBID, const char* Name )
{
	if( m_IsDisablePVPRule )
	{
		return;
	}

	if( strcmp( g_pGameMain->GetPlayerName() , Name ) == 0 )
		return;

	FriendListStruct& FriendList = CNetGlobal::RoleData()->PlayerSelfData->FriendList;

	BaseFriendStruct* pList = NULL;
	int MaxCount = 0;
	FriendList.GetListInfo( EM_FriendListType_HateFriend, &pList, MaxCount );
	if( !pList )
	{
		return ;
	}

	for( int i = 0 ; i < MaxCount ; ++i )
	{
		if( pList[i].DBID == DBID  )
		{
			pList[i].RevengeCount++ ;

			if( !pList[i].top && pList[i].RevengeCount >= pList[i].KillMeCount )
			{
				//已經報仇 移除仇人
				//SYS_SOCIAL_ADDHATEFRIEND  %s 加入仇人名單
				//SYS_SOCIAL_REVENGED       已對 %s 復仇，從仇人名單移除

				int DBID = pList[i].DBID;
				 
				FriendList.Erase( EM_FriendListType_HateFriend, DBID );
				NetCli_FriendList::SL_EraseFriend( EM_FriendListType_HateFriend, DBID );

 				g_pGameMain->SendSystemChatMsgEx( g_ObjectData->GetString( "SYS_SOCIAL_REVENGED" ) , Name );
			}
			else
			{
 				NetCli_FriendList::SL_ModifyFriend( EM_FriendListType_HateFriend, pList[i] );
			}
			
			g_pInterface->SendWorldEvent( UiEvent_ResetFriend );
			return;
		}
	}
}

const char* PAPERDOLL_PART_NAMES[DF_ModelInfoBoneScale_Max] =
{
	PAPERDOLL_PART_NAME_HEAD		,	
	PAPERDOLL_PART_NAME_HAIR		,	
	PAPERDOLL_PART_NAME_HELMET		,	
	PAPERDOLL_PART_NAME_SHOULDER	,
	PAPERDOLL_PART_NAME_TORSO		,	
	PAPERDOLL_PART_NAME_HAND		,	
	PAPERDOLL_PART_NAME_BELT		,	
	PAPERDOLL_PART_NAME_LEG			,		
	PAPERDOLL_PART_NAME_FOOT		,	
	PAPERDOLL_PART_NAME_BACK			
};

const char* BONE_SCALE_NAMES[DF_ModelInfoBoneScale_Max] =
{
	BONE_SCALE_NAME_BODY	,	
	BONE_SCALE_NAME_HEAD	,	
	BONE_SCALE_NAME_BREASTS	,	
	BONE_SCALE_NAME_CHEST	,
	BONE_SCALE_NAME_PELVIS	,	
	BONE_SCALE_NAME_ARM		,	
	BONE_SCALE_NAME_FOREARM	,	
	BONE_SCALE_NAME_HAND	,		
	BONE_SCALE_NAME_LEGS	,	
	BONE_SCALE_NAME_CALF	,	
	BONE_SCALE_NAME_FOOT			
};

bool CUI_Socal::GetSelfModelInfo( SModelInfo& rModelInfo )
{
	CRoleSprite* pRoleSprite = g_pGameMain->GetPlayer();

	if( NULL == pRoleSprite )
	{
		return false;
	}

	const char* pPartName;

	rModelInfo.actID = pRoleSprite->GetObjectId();

	int i;
	for( i = 0 ; i < DF_ModelInfoPart_Max ; ++i )
	{
		pPartName = pRoleSprite->GetPaperdollPart( PAPERDOLL_PART_NAMES[i] );
		if( !pPartName || strlen(pPartName) > 31 )
		{
			return false;
		}

		strcpy( rModelInfo.Parts[i].name , pPartName );

		rModelInfo.Parts[i].color[0] = 0;
		rModelInfo.Parts[i].color[1] = 0;

		pRoleSprite->GetPaperdollColor(PAPERDOLL_PART_NAMES[i], rModelInfo.Parts[i].color );
	}

	map<string,float>& rBoneScaleInfo = pRoleSprite->GetBoneScaleInfo();
	map<string,float>::iterator it;
	for( i = 0 ; i < DF_ModelInfoBoneScale_Max ; ++i )
	{
		it = rBoneScaleInfo.find( BONE_SCALE_NAMES[i] );

		if( it != rBoneScaleInfo.end() )
		{
			rModelInfo.BoneScales[i] = it->second;
		}
		else
		{
			rModelInfo.BoneScales[i] = 1;	 
		}
	}

	return true;

}

void CUI_Socal::RChat_PlayerOnlineInfo(  int PlayerDBID , char* RoleName , Voc_ENUM Voc , Voc_ENUM Voc_Sub , Sex_ENUM Sex
									   , int LV , int LV_Sub , int GuildID , int TitleID )
{
	NetCli_OtherChild::InsertDBIDRoleName( PlayerDBID, RoleName );

	//不能看到有GM職業的玩加上線
	//避免玩家 加GM好友
	if( Voc == EM_Vocation_GameMaster ||
		Voc_Sub == EM_Vocation_GameMaster )
	{
		return;
	}

	bool OuputOnlineMsg = false;
	if( m_PlayerOnlineSet.find( PlayerDBID ) == m_PlayerOnlineSet.end() )
	{
		OuputOnlineMsg = true;	
	}
	
	m_PlayerOnlineSet.insert(PlayerDBID);

	FriendListStruct& FriendList = CNetGlobal::RoleData()->PlayerSelfData->FriendList;

	FriendListTypeENUM ListTypes[3] = { EM_FriendListType_Friend , EM_FriendListType_HateFriend , EM_FriendListType_FamilyFriend };

	int count = sizeof( ListTypes ) / sizeof( FriendListTypeENUM );

	FriendListTypeENUM listType;
	BaseFriendStruct* pList = NULL;
  	int MaxCount = 0;
	for( int i = 0; i < count ; ++i )
	{
		listType = ListTypes[i];

		FriendList.GetListInfo( listType, &pList, MaxCount );
		if( !pList )
		{
			continue;
		}
		
		for( int i = 0 ; i < MaxCount ; ++i )
		{
			if( pList[i].DBID == PlayerDBID )
			{

				strcpy( pList[i].Name, RoleName );

				//pList[i].Race     = Race    ;	
				pList[i].Voc       = Voc     ;
				pList[i].Voc_Sub   = Voc_Sub ;
				pList[i].Sex       = Sex     ;
				pList[i].JobLv     = LV      ;
				pList[i].JobLv_Sub = LV_Sub  ;
				pList[i].GuildID   = GuildID ;
				pList[i].TitleID   = TitleID ;
				//pList[i].Time      = time( &pList[i].Time  );

				__time32_t _Time;
				_time32( &_Time );
				pList[i].Time =	_Time;

				//修改server資料
				NetCli_FriendList::SL_ModifyFriend( listType, pList[i] );

				switch( listType )
				{
				case EM_FriendListType_Friend:
					//詢問是否是對方的朋友
					g_pNetCli_ChannelChild->AmIYourFriend( EM_FriendListType_Friend, g_pGameMain->GetPlayerName() , RoleName );

					if( OuputOnlineMsg )
					{
						g_pGameMain->SendSystemChatMsgEx( g_ObjectData->GetString( "SYS_SOCIAL_FRIEND_ONLINE" ) , RoleName );
					}
					break;
				case EM_FriendListType_HateFriend:
					if( OuputOnlineMsg )
					{
						g_pGameMain->SendSystemChatMsgEx( g_ObjectData->GetString( "SYS_SOCIAL_HATEFRIEND_ONLINE" ) , RoleName );
					}
					break;
				case EM_FriendListType_FamilyFriend:
					if( OuputOnlineMsg )
					{
						g_pGameMain->SendSystemChatMsgEx( g_ObjectData->GetString( "SYS_SOCIAL_FAMILYFRIEND_ONLINE" ) , RoleName );
					}

					g_pNetCli_ChannelChild->AmIYourFriend( EM_FriendListType_FamilyFriend, g_pGameMain->GetPlayerName() , RoleName );

				    break;
				default:
				    break;
				}

			}
		}
	}

	g_pInterface->SendWorldEvent( UiEvent_ResetFriend );

}

//char* DateTimeStr( int Time )
//{
//	static char DataTime[256];
//	__time32_t NowTime = Time;
//
//	struct tm *gmt;
//	gmt = _localtime32( &NowTime );
//
//	if( gmt == NULL )
//		return "time";
//
//	char DStr[256];
//	char TStr[256];
//	sprintf( DStr , "%d/%02d/%02d" , 1900+ gmt->tm_year , 1 + gmt->tm_mon , gmt->tm_mday );
//	sprintf( TStr , "%02d:%02d:%02d" , gmt->tm_hour , gmt->tm_min, gmt->tm_sec );
//	sprintf


void CUI_Socal::RChat_PlayerOfflineInfo( int PlayerDBID )
{
	if( m_PlayerOnlineSet.find(PlayerDBID) == m_PlayerOnlineSet.end() )
	{
		return;
	}

	m_PlayerOnlineSet.erase(PlayerDBID);



	FriendListStruct& FriendList = CNetGlobal::RoleData()->PlayerSelfData->FriendList;

	FriendListTypeENUM ListTypes[3] = { EM_FriendListType_Friend , EM_FriendListType_HateFriend , EM_FriendListType_FamilyFriend };

	int count = sizeof( ListTypes ) / sizeof( FriendListTypeENUM );

	FriendListTypeENUM listType;
	BaseFriendStruct* pList = NULL;
	int MaxCount = 0;
	for( int i = 0; i < count ; ++i )
	{
		listType = ListTypes[i];

		FriendList.GetListInfo( listType, &pList, MaxCount );
		if( !pList )
		{
			continue;
		}

		for( int i = 0 ; i < MaxCount ; ++i )
		{
			if( pList[i].DBID == PlayerDBID )
			{
				switch( listType )
				{
				case EM_FriendListType_Friend:
					g_pGameMain->SendSystemChatMsgEx( g_ObjectData->GetString( "SYS_SOCIAL_FRIEND_OFFLINE" ) , pList[i].Name );
					break;
				case EM_FriendListType_HateFriend:
					g_pGameMain->SendSystemChatMsgEx( g_ObjectData->GetString( "SYS_SOCIAL_HATEFRIEND_OFFLINE" ) , pList[i].Name );
					break;
				case EM_FriendListType_FamilyFriend:
					g_pGameMain->SendSystemChatMsgEx( g_ObjectData->GetString( "SYS_SOCIAL_FAMILYFRIEND_OFFLINE" ) , pList[i].Name );
					break;
				default:
					break;
				}
 			}
		}
	}
	

	g_pInterface->SendWorldEvent( UiEvent_ResetFriend );
}

void CUI_Socal::_ShowFriendDetail( const char* Name )
{
	//判斷有沒有資料了
	if( m_mapFriendDetail.find( Name ) == m_mapFriendDetail.end() )
	{
		g_pNetCli_ChannelChild->GiveMeYourFriendInfo( Name );
		return;
	}

	lua_State* L = m_interface->GetUi()->GetLuaState();
	lua_pushstring( L, Name );
	lua_setglobal( L, "FriendName" );

	g_pInterface->SendWorldEvent( UiEvent_ShowFriendDetail );

	GetModleInfoRequest( Name );
}

void CUI_Socal::GetModleInfoRequest( const char* pTargetName )
{
	if( m_mapModelInfo.find( pTargetName ) == m_mapModelInfo.end() )
	{
		g_pNetCli_ChannelChild->GetModleInfoRequest( pTargetName );

		return;
	}

	lua_State* L = m_interface->GetUi()->GetLuaState();
	lua_pushstring( L, pTargetName );
	lua_setglobal( L, "PlayerName" );
	g_pInterface->SendWorldEvent( UiEvent_PlayerModelInfoReady );
}

void CUI_Socal::R_CtoC_GetModleInfoResult( int DBID , SModelInfo& ModelInfo )
{
	string Name = NetCli_OtherChild::GetRoleName(DBID);

	if( Name == "" )
	{
		return;
	}


	m_mapModelInfo[Name] = ModelInfo;

	lua_State* L = m_interface->GetUi()->GetLuaState();
	lua_pushstring( L, Name.c_str() );
	lua_setglobal( L, "PlayerName" );
	g_pInterface->SendWorldEvent( UiEvent_PlayerModelInfoReady );
}

void CUI_Socal::ViewFriendInfoRequest( ESocalUnitType type, const char* name )
{
	FriendListStruct& FriendList = CNetGlobal::RoleData()->PlayerSelfData->FriendList;

	if( type == SUT_Friend )
	{

		BaseFriendStruct* pList = NULL;
		int MaxCount = 0;
   
		FriendList.GetListInfo( EM_FriendListType_FamilyFriend, &pList, MaxCount );
		if( !pList )
		{
			return ;
		}

		for( int i = 0 ; i < MaxCount ; ++i )
		{
			if( stricmp( pList[i].Name, name ) == 0 )
			{
				//在不在線上
				if( m_PlayerOnlineSet.find( pList[i].DBID ) == m_PlayerOnlineSet.end() )
				{
					return;
				}

				_ShowFriendDetail( pList[i].Name );
				return;
			}
		}


		FriendList.GetListInfo( EM_FriendListType_Friend, &pList, MaxCount );
		if( !pList )
		{
			return ;
		}

		for( int i = 0 ; i < MaxCount ; ++i )
		{
			if( stricmp( pList[i].Name, name ) == 0 )
			{
				//在不在線上
				if( m_PlayerOnlineSet.find( pList[i].DBID ) == m_PlayerOnlineSet.end() )
				{
					return;
				}

				//需互為好友
				if( m_EachOtherFriendSet.find( pList[i].DBID ) == m_EachOtherFriendSet.end() )
				{
					g_pInterface->SendWorldEvent( UiEvent_ViewFriendAlert );
   					return;
				}

				_ShowFriendDetail( pList[i].Name );
				return;
			}
		}
	}
	else if( type == SUT_BadFriend )
	{
		return;
	}
	else if( type == SUT_HateFriend )
	{
		BaseFriendStruct* pList = NULL;
		int MaxCount = 0;

		FriendList.GetListInfo( EM_FriendListType_HateFriend, &pList, MaxCount );
		if( !pList )
		{
			return ;
		}

		for( int i = 0 ; i < MaxCount ; ++i )
		{
			if( stricmp( pList[i].Name, name ) == 0 )
			{
				//在不在線上
				if( m_PlayerOnlineSet.find( pList[i].DBID ) == m_PlayerOnlineSet.end() )
				{
					return;
				}

				_ShowFriendDetail( pList[i].Name );
				return;
			}
		}
	}
}

void CUI_Socal::R_CtoC_GiveYouMyFriendInfo( const char* pName, SFriendInfo& FriendInfo )
{
	m_mapFriendDetail[pName] = FriendInfo;
 	_ShowFriendDetail( pName );
}

void CUI_Socal::R_CtoC_AmIYourFriendResult( FriendListTypeENUM FriendListType, const char* AskerName, const char* AnswererName, bool Result )
{
	if( FriendListType == EM_FriendListType_Friend )
	{
		bool IsFind = false;
		int DBID = 0;

		BaseFriendStruct* pList = NULL;
		int MaxCount = 0;
		CNetGlobal::RoleData()->PlayerSelfData->FriendList.GetListInfo( EM_FriendListType_Friend, &pList, MaxCount );
		if( !pList )
		{
			return ;
		}

		int i;
		for( i = 0 ; i < MaxCount ; ++i )
		{
			if( stricmp( pList[i].Name, AnswererName ) == 0 )
			{
				DBID = pList[i].DBID;
				IsFind = true;
				break;
			}
		}

		if( IsFind )
		{
			if( Result == !( m_EachOtherFriendSet.find( DBID ) == m_EachOtherFriendSet.end() ) )
			{
				return ;
			}

			if( Result )
			{
				m_EachOtherFriendSet.insert( DBID );
			}
			else
			{
				m_EachOtherFriendSet.erase( DBID );
			}

			g_pInterface->SendWorldEvent( UiEvent_ResetFriend );
		}
	}
	else if( FriendListType == EM_FriendListType_HateFriend )
	{

	}
	else if( FriendListType == EM_FriendListType_FamilyFriend )
	{
		//FriendListTypeENUM FriendListType, const char* AskerName, const char* AnswererName, bool Result

		if( Result ) //關係還在
		{
			return;
		}

		BaseFriendStruct* pList = NULL;
		int MaxCount = 0;
		CNetGlobal::RoleData()->PlayerSelfData->FriendList.GetListInfo( EM_FriendListType_FamilyFriend, &pList, MaxCount );
		if( !pList )
		{
			return ;
		}

		int i;
		for( i = 0 ; i < MaxCount ; ++i )
		{
			if( stricmp( pList[i].Name, AnswererName ) == 0 )
			{
				// ooxx 與你脫離了xx關係

				//pList[i].Relation
				string RelationString = GetRelationString( pList[i].Relation );
				string msg = "SYS_DETACH_RELATION_" + RelationString;

				//轉大寫
				for ( int j = 0; j < msg.length(); ++j )
					msg[j] = ::toupper(msg[j]);

				msg = g_ObjectData->GetString( msg.c_str() );
				msg = ReplaceKeyString( msg, "<name>", AnswererName );

				//g_pGameMain->SendSystemChatMsg( msg.c_str() );

				m_SystemMsgList.push_back( msg );

				//只秀訊息不刪除
				//NetCli_FriendList::SL_EraseFriend( EM_FriendListType_FamilyFriend, pList[i].DBID );
				//CNetGlobal::RoleData()->PlayerSelfData->FriendList.Erase( EM_FriendListType_FamilyFriend, pList[i].DBID );
				//g_pInterface->SendWorldEvent( UiEvent_ResetFriend );

				return ;
			}
		}

	}
	
}

bool CUI_Socal::IsMyFriend( const char* name, FriendListTypeENUM Type )
{
	BaseFriendStruct* pList = NULL;
	int MaxCount = 0;
	CNetGlobal::RoleData()->PlayerSelfData->FriendList.GetListInfo( Type, &pList, MaxCount );
	if( !pList )
	{
		return false;
	}

	int i;
	for( i = 0 ; i < MaxCount ; ++i )
	{
		if( stricmp( pList[i].Name, name ) == 0 )
		{
			return true;
		}
	}

	return false;
}

static bool s_Searching = false;
static int s_lastSearchTime = 0;
// ----------------------------------------------------------------------------
void CUI_Socal::SL_SearchFindPartyPlayerRequest( Voc_ENUM MainJob , Voc_ENUM SubJob , int MinMainJobLv , int MaxMainJobLv , int MinSubJobLv , int MaxSubJobLv )
{
	if( s_Searching )
	{
		return;
	}

	if( timeGetTime() < DWORD( s_lastSearchTime + 1000 ) )
	{
		return;
	}
	s_Searching = true;
 	s_lastSearchTime = timeGetTime();
 	NetCli_FriendListChild::SL_SearchFindPartyPlayerRequest( MainJob , SubJob , MinMainJobLv, MaxMainJobLv, MinSubJobLv , MaxSubJobLv );
}

// ----------------------------------------------------------------------------
void CUI_Socal::RL_SearchFindPartyPlayerResult( vector<SearchFindPartyPlayerStruct>& PlayerInfo )
{
	s_Searching = false;

	CUI_Socal::m_vecSearchResult.clear();

	//過濾掉GM
	//CUI_Socal::m_vecSearchResult = PlayerInfo;

	vector<SearchFindPartyPlayerStruct>::iterator itPos = PlayerInfo.begin();
	vector<SearchFindPartyPlayerStruct>::iterator itEnd = PlayerInfo.end();

	for( ; itPos != itEnd ; ++itPos )
	{
		if( (*itPos).MainJob != EM_Vocation_GameMaster && 
			(*itPos).SubJob != EM_Vocation_GameMaster )
		{
			CUI_Socal::m_vecSearchResult.push_back( *itPos );
		}
	}

	g_pInterface->SendWorldEvent( UiEvent_SearchGroupResult );
}


// ----------------------------------------------------------------------------
void CUI_Socal::RL_FixFriendInfo( FriendListTypeENUM Type , int Pos , BaseFriendStruct& Info )
{
	FriendListStruct& FriendList = CNetGlobal::RoleData()->PlayerSelfData->FriendList;

	BaseFriendStruct* pList = NULL;
	int MaxCount = 0;
	FriendList.GetListInfo( Type, &pList, MaxCount );
	if( !pList )
	{
		return ;
	}

	if( Pos >= 0 && Pos < MaxCount )
	{
		if( pList[Pos].IsEmpty() || pList[Pos].Relation != Info.Relation )
		{
			//
			string strRelation;
			switch( Info.Relation )
			{
			case EM_RelationType_None       : strRelation = "SYS_RELATION_MSG_NONE"      ;  break;
			case EM_RelationType_Stranger   : strRelation = "SYS_RELATION_MSG_STRANGER"  ;  break;
			case EM_RelationType_Friend     : strRelation = "SYS_RELATION_MSG_FRIEND"    ;  break;
			case EM_RelationType_Enemy      : strRelation = "SYS_RELATION_MSG_ENEMY"     ;  break;
			case EM_RelationType_Student    : strRelation = "SYS_RELATION_MSG_STUDENT"   ;  break;
			case EM_RelationType_Teacher    : strRelation = "SYS_RELATION_MSG_TEACHER"   ;  break;
			case EM_RelationType_Wife       : strRelation = "SYS_RELATION_MSG_WIFE"      ;  break;
			case EM_RelationType_Husband    : strRelation = "SYS_RELATION_MSG_HUSBAND"   ;  break;
			case EM_RelationType_Lover      : strRelation = "SYS_RELATION_MSG_LOVER"     ;  break;
			case EM_RelationType_Family     : strRelation = "SYS_RELATION_MSG_FAMILY"    ;  break;
			case EM_RelationType_BestFriend : strRelation = "SYS_RELATION_MSG_BESTFRIEND";  break;
			case EM_RelationType_BadCompany : strRelation = "SYS_RELATION_MSG_BADCOMPANY";  break;
			default:
				strRelation = "None";
			}

			//string strName = NetCli_OtherChild::GetRoleName( Info.DBID );
			string strName = Info.Name;

			strRelation = g_ObjectData->GetString( strRelation.c_str() );
			strRelation = ReplaceKeyString( strRelation, "<name>", strName.c_str() );

			g_pGameMain->SendSystemChatMsg( strRelation.c_str() );

			g_pInterface->SendWorldEvent( UiEvent_ResetFriend );

		}

		pList[Pos] = Info;
	}
}


// ----------------------------------------------------------------------------
string CUI_Socal::GetRelationString( RelationTypeENUM relationType )
{
	string strRelationType;
	switch( relationType )
	{
	case EM_RelationType_None       : strRelationType = "None"      ;  break;
	case EM_RelationType_Stranger   : strRelationType = "Stranger"  ;  break;
	case EM_RelationType_Friend     : strRelationType = "Friend"    ;  break;
	case EM_RelationType_Enemy      : strRelationType = "Enemy"     ;  break;
	case EM_RelationType_Student    : strRelationType = "Student"   ;  break;
	case EM_RelationType_Teacher    : strRelationType = "Teacher"   ;  break;
	case EM_RelationType_Wife       : strRelationType = "Wife"      ;  break;
	case EM_RelationType_Husband    : strRelationType = "Husband"   ;  break;
	case EM_RelationType_Lover      : strRelationType = "Lover"     ;  break;
	case EM_RelationType_Family     : strRelationType = "Family"    ;  break;
	case EM_RelationType_BestFriend : strRelationType = "BestFriend";  break;
	case EM_RelationType_BadCompany : strRelationType = "BadCompany";  break;
	default:
		strRelationType = "None";
	}

	return strRelationType;
}

// ----------------------------------------------------------------------------
RelationTypeENUM CUI_Socal::GetRelationENUM( string& strRelationType )
{
	RelationTypeENUM relationType = EM_RelationType_None;

	if     ( strRelationType == "None"       ){ relationType = EM_RelationType_None      ; }
	else if( strRelationType == "Stranger"   ){ relationType = EM_RelationType_Stranger  ; }
	else if( strRelationType == "Friend"     ){ relationType = EM_RelationType_Friend    ; }
	else if( strRelationType == "Enemy"      ){ relationType = EM_RelationType_Enemy     ; }
	else if( strRelationType == "Student"    ){ relationType = EM_RelationType_Student   ; }
	else if( strRelationType == "Teacher"    ){ relationType = EM_RelationType_Teacher   ; }
	else if( strRelationType == "Wife"       ){ relationType = EM_RelationType_Wife      ; }
	else if( strRelationType == "Husband"    ){ relationType = EM_RelationType_Husband   ; }
	else if( strRelationType == "Lover"      ){ relationType = EM_RelationType_Lover     ; }
	else if( strRelationType == "Family"     ){ relationType = EM_RelationType_Family    ; }
	else if( strRelationType == "BestFriend" ){ relationType = EM_RelationType_BestFriend; }
	else if( strRelationType == "BadCompany" ){ relationType = EM_RelationType_BadCompany; }

	return relationType;
}


// ----------------------------------------------------------------------------
int	LuaFunc_AddFriend(lua_State* L)
{
	const char* cszType  = luaL_checkstring( L, 1 );
	const char* cszName  = luaL_checkstring( L, 2 );
	int groupID = ( lua_isnone( L, 3 ) || lua_isnil( L, 3 ) ) ? 0 : (int)luaL_checknumber( L, 3 );

	ESocalUnitType type;
	if( !Func_GetSocalUnitType( cszType, type ) )
	{
		return 0;
	}

	if( 0 == stricmp( g_pGameMain->GetPlayerName(), cszName ) )
	{
		if( type == SUT_BadFriend )
		{
			//新增黑名單失敗，不能把自己加入黑名單
			g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString( "SYS_SOCIAL_ADDBADFRIEND_FAIL4" ) );
		}
		else
		{
			//新增好友失敗，不能把自己加入好友
			g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString( "SYS_SOCIAL_ADDFRIEND_FAIL4" ) );
		}

		return 0;
	}
	 
	g_pUi_Socal->AddFriend( type, cszName, groupID );

	return 0;
}
// ----------------------------------------------------------------------------
int	LuaFunc_DelFriend				(lua_State* L)
{
	const char* cszType  = luaL_checkstring( L, 1 );
	const char* cszName  = luaL_checkstring( L, 2 );
	string strRelationType  = lua_isnoneornil( L, 3 ) ? "None" : luaL_checkstring( L, 3 );

	ESocalUnitType type;
	if( !Func_GetSocalUnitType( cszType, type ) )
	{
		return 0;
	}

	RelationTypeENUM relationType = g_pUi_Socal->GetRelationENUM( strRelationType );

 	g_pUi_Socal->DelFriend( type, cszName, relationType );

	//g_pInterface->SendWorldEvent(UiEvent_ResetFriend);
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetFriendCount(lua_State* L)
{
	const char* cszType = luaL_checkstring( L, 1 );

	ESocalUnitType type;
	if( !Func_GetSocalUnitType( cszType, type ) )
	{
		return 0;
	}

	if( type == SUT_Friend )
	{
		BaseFriendStruct* pList = NULL;
		int MaxCount = 0;
		CNetGlobal::RoleData()->PlayerSelfData->FriendList.GetListInfo( EM_FriendListType_FamilyFriend, &pList, MaxCount );
		if( !pList )
		{
			return 0;
		}
		int count = 0;
		int i;
		for( i = 0 ; i < MaxCount ; ++i )
		{
			if( pList[i].IsEmpty() == false )
			{
				++count;
			}
		}

		CNetGlobal::RoleData()->PlayerSelfData->FriendList.GetListInfo( EM_FriendListType_Friend, &pList, MaxCount );
		if( !pList )
		{
			return 0;
		}
		for( i = 0 ; i < MaxCount ; ++i )
		{
			if( pList[i].IsEmpty() == false )
			{
				++count;
			}
		}

		lua_pushnumber( L, count );
		return 1;
	}
	else if( type == SUT_BadFriend )
	{
		FriendListStruct& FriendList = CNetGlobal::RoleData()->PlayerSelfData->FriendList;

		int count = _MAX_BAD_FRIENDLIST_COUNT_;
		int i;
		for( i = 0 ; i < _MAX_BAD_FRIENDLIST_COUNT_ ; i++ )
		{
			if( FriendList.BadFriendList[i][0] == '\0' )
			{
				count = i;
				break;
			}
		}

		lua_pushnumber( L, count );
		return 1;
 	}
	else if( type == SUT_HateFriend )
	{
		BaseFriendStruct* pList = NULL;
		int MaxCount = 0;
		CNetGlobal::RoleData()->PlayerSelfData->FriendList.GetListInfo( EM_FriendListType_HateFriend, &pList, MaxCount );
		if( !pList )
		{
			return 0;
		}
		int count = MaxCount;
		int i;
		for( i = 0 ; i < MaxCount ; ++i )
		{
			if( pList[i].IsEmpty() )
			{
				count = i;
				break;
			}
		}

		lua_pushnumber( L, count );
		return 1;
	}


	/*
	BaseFriendStruct* pList = NULL;
	int MaxCount = 0;
	if( !Func_GetFriendList( type, &pList, MaxCount ) )
	{
		//error
		return 0;
	}

	int count = MaxCount;

	int i;
	for( i = 0 ; i < MaxCount ; ++i )
	{
		if( pList[i].IsEmpty() )
		{
			count = i;
			break;
		}
	}

	lua_pushnumber( L, count );
	*/
	return 0;
}



// ----------------------------------------------------------------------------
int	LuaFunc_GetFriendInfo(lua_State* L)
{
	const char* cszType = luaL_checkstring( L, 1 );
	int iFriendIndex = (INT)luaL_checknumber( L, 2 ) - 1; 

	ESocalUnitType type;
	if( !Func_GetSocalUnitType( cszType, type ) )
	{
		return 0;
	}

	int DBID = -1;
	string strName;
	int groupID = 0;
	bool online = false;
	bool eachOther = false;
	bool unmodifiable = false;
	bool top = false;

	int KillMeCount = 0;
	int RevengeCount = 0;

	RelationTypeENUM  RelationType = EM_RelationType_None;
	float RelationLv = 0;

	if( type == SUT_Friend )
	{
		bool IsFind = false;

		BaseFriendStruct* pList = NULL;
		int MaxCount = 0;
		CNetGlobal::RoleData()->PlayerSelfData->FriendList.GetListInfo( EM_FriendListType_FamilyFriend, &pList, MaxCount );
		if( !pList )
		{
			return 0;
		}
		int count = 0;
		int i;
		for( i = 0 ; i < MaxCount ; ++i )
		{
			if( pList[i].IsEmpty() == false )
			{
				if( count == iFriendIndex )
				{
					IsFind = true;
					DBID = pList[i].DBID;
					strName = pList[i].Name;
					unmodifiable = true;

					groupID = DF_RELATION_GROUP_ID;
					eachOther = true;

					RelationType = pList[ iFriendIndex ].Relation; //關係
					RelationLv = pList[ iFriendIndex ].Lv; //關係等級

					break;
				}
				++count;
			}
		}

		if( IsFind == false )
		{

			CNetGlobal::RoleData()->PlayerSelfData->FriendList.GetListInfo( EM_FriendListType_Friend, &pList, MaxCount );
			if( !pList )
			{
				return 0;
			}
			for( i = 0 ; i < MaxCount ; ++i )
			{
				if( pList[i].IsEmpty() == false )
				{
					if( count == iFriendIndex )
					{
						IsFind = true;
						DBID = pList[i].DBID;
						strName = pList[i].Name;
						groupID = pList[i].GroupID;

						eachOther = g_pUi_Socal->m_EachOtherFriendSet.find( DBID ) != g_pUi_Socal->m_EachOtherFriendSet.end();

						RelationType = EM_RelationType_Friend; //pList[ iFriendIndex ].Relation; //關係
						RelationLv = 0; // pList[ iFriendIndex ].Lv; //關係等級
						break;
					}
  					++count;
				}
			}
   		}

		if( IsFind == false )
		{
 			return 0;
		}

		online = g_pUi_Socal->m_PlayerOnlineSet.find( DBID ) != g_pUi_Socal->m_PlayerOnlineSet.end() ;


	}
	else if( type == SUT_BadFriend )
	{
		if( iFriendIndex < 0 || iFriendIndex >= _MAX_BAD_FRIENDLIST_COUNT_ )
		{
			return 0;
		}
		FriendListStruct& FriendList = CNetGlobal::RoleData()->PlayerSelfData->FriendList;
  		strName = FriendList.BadFriendList[iFriendIndex];

		RelationType = EM_RelationType_None; //關係
		RelationLv = 0; //關係等級
	}
	else if( type == SUT_HateFriend )
	{
		BaseFriendStruct* pList = NULL;
		int MaxCount = 0;
		CNetGlobal::RoleData()->PlayerSelfData->FriendList.GetListInfo( EM_FriendListType_HateFriend, &pList, MaxCount );
		if( !pList )
		{
			return 0;
		}
		if( iFriendIndex < 0 || iFriendIndex >= MaxCount )
		{
			return 0;
		}
		DBID = pList[iFriendIndex].DBID;
		strName = pList[ iFriendIndex ].Name;
		top = pList[ iFriendIndex ].top;

		KillMeCount = pList[ iFriendIndex ].KillMeCount ;
		RevengeCount = pList[ iFriendIndex ].RevengeCount ;

 		online = g_pUi_Socal->m_PlayerOnlineSet.find( DBID ) != g_pUi_Socal->m_PlayerOnlineSet.end() ;

		RelationType = EM_RelationType_Enemy; //pList[ iFriendIndex ].Relation; //關係
  		RelationLv = 0; //pList[ iFriendIndex ].Lv; //關係等級
	}

	// FriendName -- 1
	lua_pushstring( L, strName.c_str() );

	// 自定分類名稱 2
	lua_pushnumber( L, groupID );
	
	// Online Status -- 3
	lua_pushboolean( L, online );

	//
	lua_pushboolean( L, eachOther );

	//unmodifiable
   	lua_pushboolean( L, unmodifiable );

	//top
	lua_pushboolean( L, top );

	//KillMeCount
	lua_pushnumber( L, KillMeCount );

	//RevengeCount
	lua_pushnumber( L, RevengeCount );




	//enum RelationTypeENUM
	//{
	//	EM_RelationType_None	    = -1,	//無關係
	//	EM_RelationType_Stranger	,	//莫生人
	//	EM_RelationType_Friend		,	//好友
	//	EM_RelationType_Enemy		,	//仇人
	//	EM_RelationType_Student		,	//學生
	//	EM_RelationType_Teacher		,	//老師
	//	EM_RelationType_Wife		,	//妻子
	//	EM_RelationType_Husband		,	//先生
	//	EM_RelationType_Lover		,	//情人
	//	EM_RelationType_Brother		,	//兄弟
	//	EM_RelationType_Sister		,	//姐妹
	//	EM_RelationType_BestFriend	,	//摯友
	//	EM_RelationType_BadCompany	,	//損友
	//
	//	EM_RelationType_MaxRelation	,
	//};


	string strRelationType = g_pUi_Socal->GetRelationString( RelationType );

	lua_pushstring( L, strRelationType.c_str() );
 	lua_pushnumber( L, RelationLv );

	//pList[i].KillMeCount ;
 	//pList[i].RevengeCount ;

	return 10;

}

// ----------------------------------------------------------------------------
//--SetFriendInfo( szType, szName, szGroupName = nil, szNote = nil )
int LuaFunc_SetFriendGroup(lua_State* L)
{
	const char* szType = luaL_checkstring( L, 1 );
	const char* szName = luaL_checkstring( L, 2 ); 
	int groupID = (int)luaL_checknumber( L, 3 );

	if( groupID < 0 || groupID >= _MAX_FRIENDLIST_GROUP_COUNT_ )
	{
		return 0;
	}

  	ESocalUnitType type;
	if( !Func_GetSocalUnitType( szType, type ) )
	{
		return 0;
	}

	FriendListTypeENUM FriendListType;

	if( type == SUT_Friend )
	{
		FriendListType = EM_FriendListType_Friend;
	}
//	else if( type == SUT_HateFriend )
//	{
//		FriendListType = EM_FriendListType_HateFriend;
//	}
	else
	{
		return 0;
	}
	
	BaseFriendStruct* pList = NULL;
	int MaxCount = 0;
	CNetGlobal::RoleData()->PlayerSelfData->FriendList.GetListInfo( FriendListType, &pList, MaxCount );
	if( !pList )
	{
		return 0;
	}

	for( int i = 0 ; i < MaxCount ; ++i )
	{
		if( stricmp( pList[i].Name, szName ) == 0 )
		{
			pList[i].GroupID = groupID;
			NetCli_FriendList::SL_ModifyFriend( FriendListType, pList[i] );
			break;
		}
	}

	g_pInterface->SendWorldEvent( UiEvent_ResetFriend );

	return 0;
}

 // ----------------------------------------------------------------------------
int LuaFunc_GetSocalGroupCount(lua_State* L)
{
	const char* szType  = luaL_checkstring( L, 1 );

	ESocalUnitType type;
	if( !Func_GetSocalUnitType( szType, type ) )
	{
		return 0;
	}

	if( type != SUT_Friend )
	{
		return 0;		
	}


	FriendListStruct& FriendList = CNetGlobal::RoleData()->PlayerSelfData->FriendList;


	int count = 0;
	int i;

	//有沒有特殊關係
	bool bHaveFamily = false;
	BaseFriendStruct* pList = NULL;
	int MaxCount = 0;
	CNetGlobal::RoleData()->PlayerSelfData->FriendList.GetListInfo( EM_FriendListType_FamilyFriend, &pList, MaxCount );
	if( !pList )
	{
		return 0;
	}
	for( i = 0 ; i < MaxCount ; ++i )
	{
		if( pList[i].IsEmpty() == false )
		{
			//有特殊關係
			bHaveFamily = true;
			break;
		}
	}

	if( bHaveFamily )
	{
		++count;
	}


	if( FriendList.GroupName[0][0] == '\0' )
	{
  		strcpy( FriendList.GroupName[0] , g_ObjectData->GetString( "SYS_SOCIAL_DEFAULT_GROUP" ) );
		NetCli_FriendList::SL_FixClientData( _MAX_BAD_FRIENDLIST_COUNT_ + 0 , FriendList.GroupName[0] );
 		CNetGlobal::RoleData()->PlayerSelfData->FriendList.GroupSortID[0] = 0;
   		NetCli_FriendList::SL_FixGroupSortID( CNetGlobal::RoleData()->PlayerSelfData->FriendList.GroupSortID );
 	}
 
	for( i = 0 ; i < _MAX_FRIENDLIST_GROUP_COUNT_ ; ++i )
	{
		if( FriendList.GroupName[i][0] != '\0' )
		{
			++count;
		}
	}

	lua_pushnumber( L, count );
	return 1;
}
// ----------------------------------------------------------------------------
int	LuaFunc_GetSocalGroupInfo(lua_State* L)
{
	const char* szType  = luaL_checkstring( L, 1 );
	int Index = (INT)luaL_checknumber( L, 2 ) - 1; 
	ESocalUnitType type;
	if( !Func_GetSocalUnitType( szType, type ) )
	{
		return 0;
	}

	if( type != SUT_Friend )
	{
		return 0;		
	}

	FriendListStruct& FriendList = CNetGlobal::RoleData()->PlayerSelfData->FriendList;

	int count = 0;
	int i;

	
	//有沒有特殊關係
	bool bHaveFamily = false;
	BaseFriendStruct* pList = NULL;
	int MaxCount = 0;
	CNetGlobal::RoleData()->PlayerSelfData->FriendList.GetListInfo( EM_FriendListType_FamilyFriend, &pList, MaxCount );
	if( !pList )
	{
		return 0;
	}
	for( i = 0 ; i < MaxCount ; ++i )
	{
		if( pList[i].IsEmpty() == false )
		{
			//有特殊關係
			bHaveFamily = true;
			break;
		}
	}

	if( bHaveFamily )
	{
		if( Index == count )
		{
			lua_pushnumber( L, DF_RELATION_GROUP_ID ); //ID
			lua_pushstring( L, g_ObjectData->GetString( "SYS_SOCIAL_RELATION_GROUP" ) );
			lua_pushnumber( L, -1 );
			return 3;
		}
		++count;
	}


	for( i = 0 ; i < _MAX_FRIENDLIST_GROUP_COUNT_ ; ++i )
	{
		if( FriendList.GroupName[i][0] != '\0' )
		{
			if( Index == count )
			{
				lua_pushnumber( L, i ); //ID
				lua_pushstring( L, FriendList.GroupName[i] );
				lua_pushnumber( L, FriendList.GroupSortID[i] );
				return 3;
			}
			++count;
		}
	}

	return 0;
}

// ----------------------------------------------------------------------------
int	LuaFunc_AddSocalGroup(lua_State* L)
{
	const char* szType  = luaL_checkstring( L, 1 );
	const char* Name = luaL_checkstring( L, 2 );
	ESocalUnitType type;
	if( !Func_GetSocalUnitType( szType, type ) )
	{
		return 0;
	}

	g_pUi_Socal->AddSocalGroup( type, Name );
	return 0;
}

// ----------------------------------------------------------------------------
int	LuaFunc_DelSocalGroup(lua_State* L)
{
	const char* szType  = luaL_checkstring( L, 1 );
	int groupID = (int)luaL_checknumber( L, 2 );
	ESocalUnitType type;
	if( !Func_GetSocalUnitType( szType, type ) )
	{
		return 0;
	}

	if( groupID == 0 )
	{
		return 0;
	}

	g_pUi_Socal->DelSocalGroup( type, groupID );
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_ModifySocalGroupName(lua_State* L)
{
	const char* szType  = luaL_checkstring( L, 1 );
	int groupID = (int)luaL_checknumber( L, 2 );
	const char* NewName = luaL_checkstring( L, 3 );
	ESocalUnitType type;
	if( !Func_GetSocalUnitType( szType, type ) )
	{
		return 0;
	}

	if( groupID == 0 )
	{
		return 0;
	}

	g_pUi_Socal->ModifyGroupName( type, groupID, NewName );

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_ModifySocalGroupSort(lua_State* L)
{
	const char* szType  = luaL_checkstring( L, 1 );
	int groupID = (int)luaL_checknumber( L, 2 );
	int sort = (int)luaL_checknumber( L, 2 );
	ESocalUnitType type;
	if( !Func_GetSocalUnitType( szType, type ) )
	{
		return 0;
	}

	g_pUi_Socal->ModifyGroupSort( type, groupID, sort );

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_SetFriendTop			(lua_State* L)
{
	const char* szType = luaL_checkstring( L, 1 );
	const char* name   = luaL_checkstring( L, 2 ); 
  	bool top           = lua_toboolean(L, 3) == 0 ? false : true; 

	ESocalUnitType type;
	if( !Func_GetSocalUnitType( szType, type ) )
	{
		return 0;
	}
	if( type != SUT_HateFriend )
	{
 		return 0;
	}

	FriendListStruct& FriendList = CNetGlobal::RoleData()->PlayerSelfData->FriendList;


	BaseFriendStruct* pList = NULL;
	int MaxCount = 0;
	FriendList.GetListInfo( EM_FriendListType_HateFriend, &pList, MaxCount );
	if( !pList )
	{
		return 0;
	}
  
	BaseFriendStruct Info;
	Info.Clear();
	for( int i = 0 ; i < MaxCount ; ++i )
	{
		if( stricmp( pList[i].Name , name ) == 0 )
		{
			Info = pList[i];
			Info.top = top;
			break;
		}
	}

	if( Info.IsEmpty() )
	{
		return 0;
	}
	
	if( Info.top )
	{
		NetCli_FriendList::SL_InsertFriend( EM_FriendListType_HateFriend, 0, Info );
  		FriendList.Insert( EM_FriendListType_HateFriend, 0, Info );
	}
	else
	{
 		//移除
		NetCli_FriendList::SL_EraseFriend( EM_FriendListType_HateFriend, Info.DBID );
		FriendList.Erase( EM_FriendListType_HateFriend, Info.DBID );

 		//尋找可以插入的位置
		for( int i = 0 ; i < MaxCount ; ++i )
		{
			if( pList[i].IsEmpty() || pList[i].top == false  )
			{
				NetCli_FriendList::SL_InsertFriend( EM_FriendListType_HateFriend, i, Info );
				FriendList.Insert( EM_FriendListType_HateFriend, i, Info );
				break;
			}
		}
	}
	
	g_pInterface->SendWorldEvent( UiEvent_ResetFriend );
	
	return 0;

}

// ----------------------------------------------------------------------------
int LuaFunc_GetFriendDetailRequest(lua_State* L)
{
	const char* szType = luaL_checkstring( L, 1 );
	const char* name   = luaL_checkstring( L, 2 ); 

	ESocalUnitType type;
	if( !Func_GetSocalUnitType( szType, type ) )
	{
		return 0;
	}

	g_pUi_Socal->ViewFriendInfoRequest( type, name );
 
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetFriendDetail(lua_State* L)
{
	const char* friendName = luaL_checkstring( L, 1 );

	std::map< string, SFriendInfo >::iterator it = g_pUi_Socal->m_mapFriendDetail.find( friendName );

	if( it == g_pUi_Socal->m_mapFriendDetail.end() )
	{
		return 0;
	}

	SFriendInfo& info = it->second;

	/*
	int			MapID;
	int         ZoneID;
	int			TitleID;
	StaticString< _MAX_NAMERECORD_SIZE_ > TitleStr;

	int			GuildID;

	Voc_ENUM	Voc;
	int			VocLV;
	Voc_ENUM	Voc_Sub;
	int			Voc_SubLV;
	Sex_ENUM	Sex;

	SModelInfo  ModelInfo; //角色外觀

	StaticString< _MAX_FRIENDLIST_NOTE_ > Introduection;
	*/

	// 區域名稱 1
	const char* pMapName = Func_GetLocalMapName( info.MapID );
	lua_pushstring( L, pMapName );


	// Title 稱號	-- 2
	if( info.TitleID == 0 )
	{
		lua_pushstring( L, "" );
	}
	else if( info.TitleID == -1 )
	{
		lua_pushstring( L, info.TitleStr );
	}
	else
	{
		GameObjDbStructEx* pTitleObj = g_ObjectData->GetObj( info.TitleID );
		if( pTitleObj )
			lua_pushstring( L, pTitleObj->GetName() );
		else
			lua_pushstring( L, "Unknow" );
	}

 	// 公會名稱 Guild	-- 3
	const char* GuildName = NetCli_GuildChild::GetGuildNameByID( info.GuildID );
	if( NULL == GuildName )
	{
		GuildName = "";
	}
	lua_pushstring( L, GuildName );

	// ClassMain Name	--4 主職名稱
	lua_pushstring( L, g_ObjectData->GetClassNameByID( info.Voc ) );

	// Class1 Level	-- 5 主職等級
	lua_pushnumber( L, info.VocLV );

	// ClassSub Name -- 6 副職名稱
	lua_pushstring( L, g_ObjectData->GetClassNameByID( info.Voc_Sub ) );

	// Class1 Level	-- 7
	lua_pushnumber( L, info.Voc_SubLV );

	// Introduction	-- 8
	lua_pushstring( L, info.Introduection );


	// WorldID	-- 9
	{
		{
			int WorldID = ( info.ZoneID / _DEF_ZONE_BASE_COUNT_ ) + 1;
			char szWorldStr[256];
			//CGameObj
			//GameObjDbStruct* pZoneObj = g_ObjectData->GetObj( iZoneID + 750000 );

			sprintf( szWorldStr, g_ObjectData->GetString("SOCIAL_S_WORLD"), WorldID );

			lua_pushstring( L, szWorldStr );
		}
	}

	return 9;


	/*
	const char* cszType = luaL_checkstring( L, 1 );
	int iFriendIndex = luaL_checknumber( L, 2 ) - 1;   

	FriendListTypeENUM type;
	if( !Func_GetSocalUnitType( cszType, type ) )
	{
	return 0;
	}

	BaseFriendStruct* pList = NULL;
	int MaxCount = 0;
	if( Func_GetFriendList( EM_FriendListType_Friend, &pList, MaxCount ) )
	{
	//error
	return 0;
	}

	if( iFriendIndex < 0 || iFriendIndex >= MaxCount )
	{
	return 0;
	}


	BaseFriendStruct* pFriend =  &( pList[ iFriendIndex ] );

	if( pFriend->IsEmpty() )
	{
	return 0;
	}

	SSocalFriendInfo info = g_pUi_Socal->m_mapFriendInfo[ pFriend->DBID ];

	// FriendName -- 1
	lua_pushstring( L, pFriend->Name );

	// 自定分類名稱 2
	//int iFolderID = -1;

	//map< string, SSocalFriendInfo >::iterator it = g_pUi_Socal->m_mapFolderID.find( pFriend->GroupType );

	//if( it != g_pUi_Socal->m_mapFolderID.end() )
	//{
	//	iFolderID = ( it->second ).iID;
	//}
	//else
	//{
	//	iFolderID = 1;
	//}
	//lua_pushnumber( L, iFolderID );

	lua_pushstring( L, pFriend->GroupType );

	// 區域名稱 3
	{
	const char* mapname = Func_GetLocalMapName( info.FriendInfo.MapID );

	if( mapname )
	lua_pushstring( L, mapname );
	else
	lua_pushstring( L, "UnknowZone" );
	}

	// Title 稱號	-- 4
	if( info.FriendInfo.TitleID == 0 )
	{
	lua_pushstring( L, "" );
	}
	else if( info.FriendInfo.TitleID == 1 )
	{
	lua_pushstring( L, info.FriendInfo.TitleStr );
	}
	else
	{
	GameObjDbStruct* pTitleObj = g_ObjectData->GetObj( info.FriendInfo.TitleID );
	if( pTitleObj )
	lua_pushstring( L, pTitleObj->Name );
	else
	lua_pushstring( L, "Unknow" );
	}


	// 公會名稱 Guild	-- 5
	const char* GuildName = NetCli_GuildChild::GetGuildNameByID( info.FriendInfo.GuildID );
	if( NULL == GuildName )
	{
	GuildName = "";
	}

	lua_pushstring( L, GuildName );

	// ClassMain Name	--6 主職名稱
	lua_pushstring( L, g_ObjectData->GetClassNameByID( info.FriendInfo.Voc ) );

	// Class1 Level	-- 7 主職等級
	lua_pushnumber( L, info.FriendInfo.VocLV );

	// ClassSub Name	-- 8 副職名稱
	lua_pushstring( L, g_ObjectData->GetClassNameByID( info.FriendInfo.Voc_Sub ) );

	// Class1 Level	-- 9
	lua_pushnumber( L, info.FriendInfo.Voc_SubLV );

	// Introduction	-- 10
	lua_pushstring( L, info.Introduection.c_str() );

	// Note	-- 11
	lua_pushstring( L, pFriend->Note );

	// Online Status --12
	lua_pushnumber( L, (int)info.bOnline );

	// WorldID	-- 13
	{
	{
	int WorldID = ( info.FriendInfo.ZoneID / _DEF_ZONE_BASE_COUNT_ ) + 1;
	char szWorldStr[256];
	//CGameObj
	//GameObjDbStruct* pZoneObj = g_ObjectData->GetObj( iZoneID + 750000 );

	sprintf( szWorldStr, g_ObjectData->GetString("SOCIAL_S_WORLD"), WorldID );

	lua_pushstring( L, szWorldStr );
	}
	}

	return 13;
	*/	
	return 0;
}

// ----------------------------------------------------------------------------
int	LuaFunc_SearchGroupPeople		(lua_State* L)
{
	int		iMainVoc		= luaL_checkint( L, 1 );
	int		iMainVocMinLV	= luaL_checkint( L, 2 );
	int		iMainVocMaxLV	= luaL_checkint( L, 3 );

	int		iSubVoc			= luaL_checkint( L, 4 );
	int		iSubVocMinLV	= luaL_checkint( L, 5 );
	int		iSubVocMaxLV	= luaL_checkint( L, 6 );

	g_pUi_Socal->SL_SearchFindPartyPlayerRequest( (Voc_ENUM)iMainVoc , (Voc_ENUM)iSubVoc , iMainVocMinLV, iMainVocMaxLV, iSubVocMinLV , iSubVocMaxLV );
	return 0;
}
//----------------------------------------------------------------------------
//改用	  LuaFunc_SetFindPartyState		LuaFunc_GetFindPartyState
//int	LuaFunc_SearchGroupFlag (lua_State* L)
//{
//	int					iFlag	= luaL_checkint( L, 1 );
//	CRoleSprite*		pSprite	= g_pGameMain->GetPlayer();
//
//	if ( pSprite == NULL )
//	{
//		lua_pushnumber(L, 0);
//		return 1;
//	}
//
//	if( iFlag == -1 )
//	{
//		int iResult = (bool)pSprite->GetFindParty();
//		lua_pushnumber( L, iResult );
//		return 1;	
//	}
//
//
//
//	PlayerStateStruct	flag;
//
//	flag.FindParty	= iFlag != 0;
//	flag.AFK		= pSprite->GetAFK();
//	flag.Dig		= pSprite->GetBotting();
//
//	NetCli_RoleValue::S_SetPlayerAllZoneState( flag );
//
//	return 0;
//}
// ----------------------------------------------------------------------------
int	LuaFunc_GetSearchGroupResult	(lua_State* L)
{
    int iArg = luaL_checkint( L, 1 );

    if (iArg == -1)
    {
        int iSearchResult = (int)CUI_Socal::m_vecSearchResult.size();
        lua_pushnumber( L, iSearchResult );
        return 1;
    }

    // 1 FriendID
    int iIndex = iArg - 1;  // Friend ID

    if (iIndex >= 0 && iIndex < (int)CUI_Socal::m_vecSearchResult.size())
    {
        SearchFindPartyPlayerStruct SearchResult = CUI_Socal::m_vecSearchResult[ iIndex ];

        // 1 Name
        lua_pushstring( L, SearchResult.Name.Begin() );

        // 2 MainClass
        lua_pushstring( L, g_ObjectData->GetClassNameByID( SearchResult.MainJob ) );

        // 3 MainClassLV
        lua_pushnumber( L, SearchResult.MainJobLv );

        // 4 SubClass
        lua_pushstring( L, g_ObjectData->GetClassNameByID( SearchResult.SubJob ) );

        // 5 MainClassLV
        lua_pushnumber( L, SearchResult.SubJobLv );

        // 6 Title
        if ( SearchResult.TitleID == 0 )
        {
            lua_pushstring( L, "" );
        }
        else if ( SearchResult.TitleID == 1 )
        {
            lua_pushstring( L, SearchResult.TitleStr.Begin() );
        }
        else
        {
            GameObjDbStructEx* pTitleObj = g_ObjectData->GetObj( SearchResult.TitleID );
            if ( pTitleObj )
                lua_pushstring( L, pTitleObj->GetName() );
            else
                lua_pushstring( L, "Unknow" );
        }

        // 7 GuildID
        //lua_pushnumber( L, SearchResult.GuildID );
        const char* pGuildName = g_pGuildFrame->GetGuildNameByID( SearchResult.GuildID );
        if ( pGuildName )
	        lua_pushstring( L, pGuildName );
        else
	        lua_pushstring( L, "" );

        // 8 ZoneID
        //------------------------------------------------------------------------------
        {
	        int iZoneID = SearchResult.ZoneID % _DEF_ZONE_BASE_COUNT_;
	        lua_pushstring( L, Func_GetLocalMapName( iZoneID ) );
        }

        // 9 World
        //------------------------------------------------------------------------------
        {
            int WorldID = ( SearchResult.ZoneID / _DEF_ZONE_BASE_COUNT_ ) + 1;
            char szWorldStr[256];
            //CGameObj
            //GameObjDbStruct* pZoneObj = g_ObjectData->GetObj( iZoneID + 750000 );
            sprintf( szWorldStr, g_ObjectData->GetString("SOCIAL_S_WORLD"), WorldID );
            lua_pushstring( L, szWorldStr );
        }

        return 9;
    }

    return 0; // edit by slime 2010/08/03
}

// ----------------------------------------------------------------------------
int LuaFunc_GetSystemString			(lua_State* L)
{
	const char* pszName		= luaL_checkstring( L, 1 );
	lua_pushstring( L, g_ObjectData->GetString( pszName ));

	return 1;
}



// ----------------------------------------------------------------------------
int luaFunc_SetFriendModel(lua_State* L)
{
	int nFriendID  = luaL_checkint( L, 1 ) - 1;
	const char* pszUIModel = luaL_checkstring( L, 2 );

	g_pUi_Socal->SetFriendModel( nFriendID , pszUIModel );

	return 0;
}

// ----------------------------------------------------------------------------
int luaFunc_IsMyFriend(lua_State* L)
{
	const char* name = luaL_checkstring( L, 1 );

	if( g_pUi_Socal->IsMyFriend( name ) )
	{
		lua_pushboolean( L, true );
		return 1;
	}

	return 0;
}

// ----------------------------------------------------------------------------
int luaFunc_AskPlayerInfo(lua_State* L)
{
	const char* name = luaL_checkstring( L, 1 );
	g_pNetCli_ChannelChild->AskPlayerInfo( name );
	return 0;
}





// ----------------------------------------------------------------------------
int Model_SetPlayerModel(lua_State* L)
{
	CUiModel* model = dynamic_cast<CUiModel*>(CUiLuaLayout::CheckLayoutTable(L, 1));
	if (model == NULL)
		lua_error(L);

  	const char* friendName = luaL_checkstring( L, 2 );

	std::map< string, SModelInfo >::iterator it = g_pUi_Socal->m_mapModelInfo.find( friendName );

	if( it == g_pUi_Socal->m_mapModelInfo.end() )
	{
		return 0;
	}

	SModelInfo& rModelInfo = it->second ;

	GameObjDbStruct* pObj = NULL;

	if ( ( pObj = g_ObjectData->GetObj( rModelInfo.actID ) ) == NULL )
		return 0;

	GameObjDbStruct* imageDB = CNetGlobal::GetObj(pObj->ImageID);
	if ( imageDB == NULL )
		return 0;

	model->CreateEntity( imageDB->Image.ACTWorld );
	int i;
	for( i = 0 ; i < DF_ModelInfoPart_Max ; ++i )
	{
		model->SetPaperdollPart(PAPERDOLL_PART_NAMES[i], rModelInfo.Parts[i].name);
		model->SetPaperdollColor(PAPERDOLL_PART_NAMES[i], rModelInfo.Parts[i].color[0], rModelInfo.Parts[i].color[1]);
	}

	for( i = 0 ; i < DF_ModelInfoBoneScale_Max ; ++i )
	{
		model->SetBoneScale( BONE_SCALE_NAMES[i] , rModelInfo.BoneScales[i] );
	}

	//model->SetVisible( true );

	return 0;
}
