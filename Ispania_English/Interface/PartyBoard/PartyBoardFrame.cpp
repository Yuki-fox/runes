#include "../../mainproc/GameMain.h"
#include "../../netwaker_member/NetWakerClientInc.h"

#include "PartyBoardFrame.h"

CPartyBoardFrame*		g_pPartyBoardFrame	= NULL;
static int				s_AskPartyJoin_LastTime = 0;
//-------------------------------------------------------------------
int			LuaFunc_PostPartyAd				( lua_State *L );
int			LuaFunc_GetPartyBoardInfo		( lua_State *L );
int			LuaFunc_GetMyPBPostIndex		( lua_State *L );
int			LuaFunc_DeleteMyPBPost			( lua_State *L );
int			LuaFunc_SendPartyJoinMsg		( lua_State *L );
int			LuaFunc_PartyJoinResult			( lua_State *L );
int			LuaFunc_PostPeoplewareAd		( lua_State *L );
int			LuaFunc_PB_SetRaidInfo			( lua_State *L );
int			LuaFunc_PB_GetRaidInfo			( lua_State *L );
int			LuaFunc_PB_GetRaidTooltipInfo	( lua_State *L );
int			LuaFunc_PB_UpdateMemberNum		( lua_State *L );
int			LuaFunc_PB_UpdateVocAndLevel	( lua_State *L );
int			LuaFunc_GetMyPBPostType			( lua_State *L );
int			LuaFunc_PB_GetVocInfo			( lua_State *L );
int			LuaFunc_PB_GetServerLvLimit		( lua_State *L );

//-------------------------------------------------------------------
CPartyBoardFrame::CPartyBoardFrame( CInterface* object ) : CInterfaceBase( object )
{
	g_pPartyBoardFrame = this;
	m_vecPartyPosts.clear();
	m_vecPeoplewarePosts.clear();
	m_AskerName.clear();
	m_mapRaidInfo.clear();
	m_myPostType = ePartyBoard_NoAd;
}
// --------------------------------------------------------------------------------------
CPartyBoardFrame::~CPartyBoardFrame(void)
{
	g_pPartyBoardFrame = NULL;
}
//--------------------------------------------------------------------------------------
void CPartyBoardFrame::RegisterFunc(void)
{
	lua_State* l = GetLuaStateWorld();
	if (l)
	{
		lua_register( l, "PostPartyAd",				LuaFunc_PostPartyAd			);	// 發佈組隊招募
		lua_register( l, "GetPartyBoardInfo",		LuaFunc_GetPartyBoardInfo	);	// 取得組隊招募文數量及內容
		lua_register( l, "GetMyPBPostIndex",		LuaFunc_GetMyPBPostIndex	);	// 取得自己的留言索引(無留言時回傳0)
		lua_register( l, "DeleteMyPBPost",			LuaFunc_DeleteMyPBPost		);	// 取消組隊招募留言
		lua_register( l, "SendPartyJoinMsg",		LuaFunc_SendPartyJoinMsg	);	// 傳送申請加入訊息給指定名稱的玩家
		lua_register( l, "PartyJoinResult",			LuaFunc_PartyJoinResult		);	// 拒絕別人申請加入隊伍的要求
		lua_register( l, "PostPeoplewareAd",		LuaFunc_PostPeoplewareAd	);	// 發佈人力資源
		lua_register( l, "PB_SetRaidInfo",			LuaFunc_PB_SetRaidInfo		);	// 用index取得團隊資訊並儲存
		lua_register( l, "PB_GetRaidInfo",			LuaFunc_PB_GetRaidInfo		);	// 用memberID取得之前儲存的團隊資訊
		lua_register( l, "PB_GetRaidTooltipInfo",	LuaFunc_PB_GetRaidTooltipInfo	);	// 用memberID取得之前儲存的團隊資訊(Tooltip用)
		lua_register( l, "PB_UpdateMemberNum",		LuaFunc_PB_UpdateMemberNum	);	// 更新留言要顯示的目前團隊成員數
		lua_register( l, "PB_UpdateVocAndLevel",	LuaFunc_PB_UpdateVocAndLevel);	// 更新留言要顯示的玩家職業與等級
		lua_register( l, "GetMyPBPostType",			LuaFunc_GetMyPBPostType		);	// 取得自己是否有留言、留言在哪個板的資訊
		lua_register( l, "PB_GetVocInfo",			LuaFunc_PB_GetVocInfo		);	// 取得人力資源留言板的職業資訊
		lua_register( l, "PB_GetServerLvLimit",		LuaFunc_PB_GetServerLvLimit	);	// 取得Server設定的玩家等級上限
	}
}

void CPartyBoardFrame::DestroyFrames()
{
	ClearList();
}
void CPartyBoardFrame::ClearList()
{
	m_vecPartyPosts.clear();
	m_vecPeoplewarePosts.clear();
	m_AskerName.clear();
	m_mapRaidInfo.clear();
	m_myPostType = ePartyBoard_NoAd;
	SendWorldEvent( "PARTY_BOARD_POST_DELETED" );
}

void CPartyBoardFrame::AddPost( int iPlayerDBID , int iDataSize , char* szData )
{
	EPartyBoardPostType Type;
	memcpy( &Type, szData, sizeof( EPartyBoardPostType ) );

	switch( Type )
	{
	case ePartyBoard_PartyAd:
		{
			SPartyBoardPartyAd post;
			memcpy( &post, szData + sizeof( EPartyBoardPostType ), sizeof( SPartyBoardPartyAd ) );

			vector<pair<int, SPartyBoardPartyAd>>::iterator it;
			for( it = m_vecPartyPosts.begin(); it != m_vecPartyPosts.end(); it++ )
				if( (*it).first == iPlayerDBID )
				{
					(*it) = pair<int, SPartyBoardPartyAd>( iPlayerDBID, post );
					g_pPartyBoardFrame->SendWorldEvent( "PARTY_BOARD_REFRESH" );
					return;
				}

			m_vecPartyPosts.push_back( pair<int, SPartyBoardPartyAd>( iPlayerDBID, post ) );
		}
		break;
	case ePartyBoard_PeoplewareAd:
		{
			SPartyBoardPeoplewareAd post;
			memcpy( &post, szData + sizeof( EPartyBoardPostType ), sizeof( SPartyBoardPeoplewareAd ) );

			vector<pair<int, SPartyBoardPeoplewareAd>>::iterator it;
			for( it = m_vecPeoplewarePosts.begin(); it != m_vecPeoplewarePosts.end(); it++ )
				if( (*it).first == iPlayerDBID )
				{
					(*it) = pair<int, SPartyBoardPeoplewareAd>( iPlayerDBID, post );
					g_pPartyBoardFrame->SendWorldEvent( "PARTY_BOARD_REFRESH" );
					return;
				}

			m_vecPeoplewarePosts.push_back( pair<int, SPartyBoardPeoplewareAd>( iPlayerDBID, post ) );
		}
		break;
	}

	g_pPartyBoardFrame->SendWorldEvent( "PARTY_BOARD_REFRESH" );
}

void CPartyBoardFrame::DeletePost( int iPlayerDBID )
{
	if( iPlayerDBID == g_pGameMain->GetDBID() )
	{
		m_myPostType = ePartyBoard_NoAd;
		g_pPartyBoardFrame->SendWorldEvent( "PARTY_BOARD_POST_DELETED" );
	}

	vector<pair<int, SPartyBoardPartyAd>>::iterator it;
	for( it = m_vecPartyPosts.begin(); it != m_vecPartyPosts.end(); it++ )
		if( (*it).first == iPlayerDBID )
		{
			m_vecPartyPosts.erase( it );
			g_pPartyBoardFrame->SendWorldEvent( "PARTY_BOARD_REFRESH" );
			return;
		}

	vector<pair<int, SPartyBoardPeoplewareAd>>::iterator it2;
	for( it2 = m_vecPeoplewarePosts.begin(); it2 != m_vecPeoplewarePosts.end(); it2++ )
		if( (*it2).first == iPlayerDBID )
		{
			m_vecPeoplewarePosts.erase( it2 );
			g_pPartyBoardFrame->SendWorldEvent( "PARTY_BOARD_REFRESH" );
			return;
		}
}

int CPartyBoardFrame::GetPostNum( int iTab )
{
	switch( iTab )
	{
	case 1:
		return (int)m_vecPartyPosts.size();
	case 2:
		return (int)m_vecPeoplewarePosts.size();
	}

	return 0;	
}

int CPartyBoardFrame::GetPoster( int iTab, int iIndex )
{
	switch( iTab )
	{
	case 1:
		return m_vecPartyPosts[iIndex].first;
	case 2:
		return m_vecPeoplewarePosts[iIndex].first;
	}

	return 0;
}

SPartyBoardPartyAd CPartyBoardFrame::GetPartyPost( int iIndex )
{
	return m_vecPartyPosts[iIndex].second;
}

SPartyBoardPeoplewareAd CPartyBoardFrame::GetPeoplewarePost( int iIndex )
{
	return m_vecPeoplewarePosts[iIndex].second;
}

void CPartyBoardFrame::PartyJoinResult( bool bYesNo )
{
	if( !m_AskerName.empty() )
	{
		char *szAskerName = (char*)m_AskerName.c_str();
		NetCli_PartyChild::AskJoinToPartyResult( szAskerName, bYesNo );
		m_AskerName.clear();
	}	
}

void CPartyBoardFrame::SetAskerName( string AskerName )
{
	m_AskerName = AskerName;
}

string CPartyBoardFrame::GetAskerName()
{
	return m_AskerName;
}

void CPartyBoardFrame::AnalyzeRaidInfo( int iLeaderDBID, vector< PartyMemberInfoStruct > & List )
{
	for( int i = 0; i < List.size(); ++i )
	{
		int iPartyNo = List[i].PartyNo;

		SPartyBoardRaidInfo info;
		info.Name		= List[i].Name;
		info.isOnline	= ( List[i].GItemID != -1 );
		info.isLeader	= ( List[i].DBID == iLeaderDBID );
		info.Voc		= List[i].Voc;
		info.Voc_Sub	= List[i].Voc_Sub;
		info.Level		= List[i].Level;
		info.Level_Sub	= List[i].Level_Sub;
		info.Sex		= List[i].Sex;

		m_mapRaidInfo[iPartyNo] = info;
	}

	SendWorldEvent( "PB_RAIDINFO_REFRESH" );
}

bool CPartyBoardFrame::HasRaidMemberInfo ( int iMemberID )
{
	if( m_mapRaidInfo.find( iMemberID ) != m_mapRaidInfo.end() )
		return true;

	return false;
}

SPartyBoardRaidInfo CPartyBoardFrame::GetRaidMemberInfo ( int iMemberID )
{
	return m_mapRaidInfo[iMemberID];
}

void CPartyBoardFrame::UpdateLeaderName( string oldLeaderName )
{
	SPartyBoardPartyAd *pPost = NULL;

	int iNum = (int)m_vecPartyPosts.size();

	for( int i = 0; i < iNum; ++i )
	{
		string posterName = m_vecPartyPosts[i].second.PosterName.Begin();

		if( posterName.compare( oldLeaderName ) == 0 )
		{
			pPost = &(m_vecPartyPosts[i].second);
			m_vecPartyPosts[i].second.PosterName = g_pGameMain->GetPlayerName();
			break;
		}
	}

	if( pPost == NULL )
		return;

	if( m_myPostType == ePartyBoard_NoAd )
	{
		pPost->byMemberNumNow = (BYTE)NetCli_PartyChild::s_member.size();

		EPartyBoardPostType Type = ePartyBoard_PartyAd;

		int		size		= sizeof( SPartyBoardPartyAd ) + sizeof( EPartyBoardPostType );
		char	*data		= new char[size];

		memcpy( data, &Type, sizeof( EPartyBoardPostType ) );
		memcpy( data + sizeof( EPartyBoardPostType ), pPost, sizeof( SPartyBoardPartyAd ) );

		NetCli_Talk::SChat_RegisterBillBoardInfo( size, data );

		delete [] data;

		m_myPostType = ePartyBoard_PartyAd;
	}	

	char* name = _strdup( oldLeaderName.c_str() );

	NetCli_PartyChild::DeletePBPost( name );

	free( name );	// delete [] name;會被中斷
}

int CPartyBoardFrame::GetMyPostIndex()
{
	int iNum = GetPostNum( (int)m_myPostType );

	for( int i = 0; i < iNum; ++i )
	{
		int iPlayerDBID = GetPoster( (int)m_myPostType, i );

		if( iPlayerDBID == g_pGameMain->GetDBID() )
		{
			return i;
		}
	}

	return -1;
}

bool CPartyBoardFrame::IsPostForRaid()
{
	if( m_myPostType == ePartyBoard_PartyAd )
	{
		int iIndex = GetMyPostIndex();
		SPartyBoardPartyAd ad = GetPartyPost( iIndex );

		if( ad.PartyMode == 1 )
			return true;
	}

	return false;
}

int LuaFunc_PostPartyAd( lua_State *L )
{
	// 不能同時在組隊招募留言板及人力資源留言板留言
	if( g_pPartyBoardFrame->GetMyPostType() == ePartyBoard_PeoplewareAd )
		return 0;

	// 不是單身也不是隊長不能留言
	if( NetCli_PartyChild::s_state != ePartyState_Leader && NetCli_PartyChild::s_member.size() != 0 )
		return 0;

	SPartyBoardPartyAd post;

	int		iTarget			= luaL_checkint( L, 1 );
	int		iArea			= luaL_checkint( L, 2 );
	int		iAreaID			= luaL_checkint( L, 3 );
	string	AreaName		= luaL_checkstring( L, 4 );
	int		iPartyMode		= luaL_checkint( L, 5 );
	int		iAssignMode		= luaL_checkint( L, 6 );
	int		iMemberNum		= luaL_checkint( L, 7 );
	int		iLvLimitMode	= luaL_checkint( L, 8 );
	int		iLowLvLimit		= luaL_checkint( L, 9 );
	int		iDutyAttack		= luaL_checkint( L, 10 );
	int		iDutyDefend		= luaL_checkint( L, 11 );
	int		iDutyCure		= luaL_checkint( L, 12 );
	int		iDutyNoLimit	= luaL_checkint( L, 13 );
	string	Comment			= luaL_checkstring( L, 14 );

	// 已有所需隊員數也不用留言了
	if( NetCli_PartyChild::s_member.size() > iMemberNum )
		return 0;

	CRoleSprite* player = g_pGameMain->GetPlayer();

	post.PosterName			= player->GetName();
	post.AreaName			= AreaName.c_str();
	post.Comment			= Comment.c_str();
	post.byTarget			= (BYTE)iTarget;
	post.iAreaID			= iAreaID;
	post.byArea				= (BYTE)iArea;
	post.PartyMode			= (BYTE)iPartyMode;
	post.AssignMode			= (BYTE)iAssignMode;
	post.byMemberNumNeed	= (BYTE)iMemberNum;
	post.byMemberNumNow		= (BYTE)NetCli_PartyChild::s_member.size();
	post.LvLimitMode		= (BYTE)iLvLimitMode;
	post.sLowLvLimit		= (short)iLowLvLimit;
	post.byDutyAttack		= (BYTE)iDutyAttack;
	post.byDutyDefend		= (BYTE)iDutyDefend;
	post.byDutyCure			= (BYTE)iDutyCure;
	post.byDutyNoLimit		= (BYTE)iDutyNoLimit;	
	
	EPartyBoardPostType Type = ePartyBoard_PartyAd;

	int		size		= sizeof( SPartyBoardPartyAd ) + sizeof( EPartyBoardPostType );
	char	*data		= new char[size];

	memcpy( data, &Type, sizeof( EPartyBoardPostType ) );
	memcpy( data + sizeof( EPartyBoardPostType ), &post, sizeof( SPartyBoardPartyAd ) );

	NetCli_Talk::SChat_RegisterBillBoardInfo( size, data );

	delete [] data;

	g_pPartyBoardFrame->SetMyPostType( ePartyBoard_PartyAd );

	return 0;
}

int LuaFunc_GetPartyBoardInfo( lua_State *L )
{
	int iTab = luaL_checkint( L, 1 );

	if( iTab == 1 )
	{
		int iIndex = luaL_checkint( L, 2 );

		if( iIndex == -1 )
		{
			lua_pushnumber( L, g_pPartyBoardFrame->GetPostNum( iTab ) );
			return 1;
		}
		else if( iIndex > 0 && iIndex <= g_pPartyBoardFrame->GetPostNum( iTab ) )
		{
			SPartyBoardPartyAd post = g_pPartyBoardFrame->GetPartyPost( iIndex - 1 );

			lua_pushnumber( L, post.LvLimitMode );
			lua_pushnumber( L, post.sLowLvLimit );
			lua_pushstring( L, post.PosterName.Begin() );
			lua_pushnumber( L, post.byMemberNumNeed );
			lua_pushnumber( L, post.byMemberNumNow );
			lua_pushnumber( L, post.PartyMode );
			lua_pushnumber( L, post.AssignMode );
			lua_pushnumber( L, post.byDutyAttack );
			lua_pushnumber( L, post.byDutyDefend );
			lua_pushnumber( L, post.byDutyCure );
			lua_pushnumber( L, post.byDutyNoLimit );
			lua_pushnumber( L, post.byTarget );
			lua_pushnumber( L, post.byArea );
			lua_pushnumber( L, post.iAreaID );
			lua_pushstring( L, post.AreaName.Begin() );
			lua_pushstring( L, post.Comment.Begin() );

			return 16;
		}
	}
	else if( iTab == 2 )
	{
		int iIndex = luaL_checkint( L, 2 );

		if( iIndex == -1 )
		{
			lua_pushnumber( L, g_pPartyBoardFrame->GetPostNum( iTab ) );
			return 1;
		}
		else if( iIndex > 0 && iIndex <= g_pPartyBoardFrame->GetPostNum( iTab ) )
		{
			SPartyBoardPeoplewareAd post = g_pPartyBoardFrame->GetPeoplewarePost( iIndex - 1 );

			lua_pushnumber( L, post.Voc );
			lua_pushnumber( L, post.VocLV );
			lua_pushnumber( L, post.Voc_Sub );
			lua_pushnumber( L, post.Voc_SubLV );
			lua_pushstring( L, post.PosterName.Begin() );
			lua_pushnumber( L, post.byActAs );
			for( int i = 0; i < 3; ++i )
			{
				lua_pushnumber( L, post.byTarget[i] );
				lua_pushnumber( L, post.byArea[i] );
				lua_pushnumber( L, post.iAreaID[i] );
				lua_pushstring( L, post.AreaName[i].Begin() );
			}			
			lua_pushstring( L, post.Comment.Begin() );

			return 19;
		}
	}	

	return 0;
}

int LuaFunc_GetMyPBPostIndex( lua_State *L )
{
	int iIndex = g_pPartyBoardFrame->GetMyPostIndex();

	lua_pushnumber( L, iIndex + 1 );

	return 1;
}

int LuaFunc_DeleteMyPBPost( lua_State *L )
{
	NetCli_Talk::SChat_BillBoardLiveTime( 0 );

	return 0;
}

int LuaFunc_SendPartyJoinMsg( lua_State *L )
{
	if( timeGetTime() < DWORD(s_AskPartyJoin_LastTime + 5000) )
		return 0;

	s_AskPartyJoin_LastTime = timeGetTime();

	char* szLeaderName = (char*)luaL_checkstring( L, 1 );

	NetCli_PartyChild::AskJoinToParty( szLeaderName );

	return 0;
}

int LuaFunc_PartyJoinResult( lua_State *L )
{
	bool bYesNo = lua_toboolean( L, 1 ) == 1 ? true : false;

	g_pPartyBoardFrame->PartyJoinResult( bYesNo );

	return 0;
}

int LuaFunc_PostPeoplewareAd( lua_State *L )
{
	// 不能同時在組隊招募留言板及人力資源留言板留言
	if( g_pPartyBoardFrame->GetMyPostType() == ePartyBoard_PartyAd )
		return 0;

	// 不是單身不能留言
	if( NetCli_PartyChild::s_member.size() != 0 )
		return 0;

	SPartyBoardPeoplewareAd post;
	int		iTarget[3];
	int		iArea[3];
	int		iAreaID[3];
	string	AreaName[3];


	for( int i = 0; i < 3; ++i )
	{
		iTarget[i]		= luaL_checkint( L, 1 + ( i * 4 ) );
		iArea[i]		= luaL_checkint( L, 2 + ( i * 4 ) );
		iAreaID[i]		= luaL_checkint( L, 3 + ( i * 4 ) );
		AreaName[i]		= luaL_checkstring( L, 4 + ( i * 4 ) );
	}

	string	Comment		= luaL_checkstring( L, 13 );
	int		iActAs		= luaL_checkint( L, 14 );

	CRoleSprite* player = g_pGameMain->GetPlayer();

	post.PosterName		= player->GetName();

	for( int i = 0; i < 3; ++i )
	{
		post.byTarget[i]	= (BYTE)iTarget[i];
		post.iAreaID[i]		= iAreaID[i];
		post.byArea[i]		= (BYTE)iArea[i];
		post.AreaName[i]	= AreaName[i].c_str();
	}

	post.Comment		= Comment.c_str();
	post.byActAs		= (BYTE)iActAs;

	RoleDataEx*	pRole = CNetGlobal::RoleData();

	post.Voc			= pRole->BaseData.Voc;
	post.VocLV			= (short)pRole->Level();
	post.Voc_Sub		= pRole->BaseData.Voc_Sub;
	post.Voc_SubLV		= (short)pRole->Level_Sub();

	EPartyBoardPostType Type = ePartyBoard_PeoplewareAd;

	int		size		= sizeof( SPartyBoardPeoplewareAd ) + sizeof( EPartyBoardPostType );
	char	*data		= new char[size];

	memcpy( data, &Type, sizeof( EPartyBoardPostType ) );
	memcpy( data + sizeof( EPartyBoardPostType ), &post, sizeof( SPartyBoardPeoplewareAd ) );

	NetCli_Talk::SChat_RegisterBillBoardInfo( size, data );

	delete [] data;

	g_pPartyBoardFrame->SetMyPostType( ePartyBoard_PeoplewareAd );

	return 0;
}

int LuaFunc_PB_SetRaidInfo( lua_State *L )
{
	int iIndex = luaL_checkint( L, 1 ) - 1;

	NetCli_Party::SL_GetMemberInfo( g_pPartyBoardFrame->GetPoster( 1, iIndex ) );

	return 0;
}

int LuaFunc_PB_GetRaidInfo( lua_State *L )
{
	int iPartyNo = luaL_checkint( L, 1 ) - 1;

	if( g_pPartyBoardFrame->HasRaidMemberInfo( iPartyNo ) )
	{
		SPartyBoardRaidInfo info = g_pPartyBoardFrame->GetRaidMemberInfo( iPartyNo );

		lua_pushstring( L, info.Name.Begin() );
		lua_pushboolean( L, info.isLeader );
		lua_pushboolean( L, info.isOnline );

		return 3;
	}

	return 0;	
}

int LuaFunc_PB_GetRaidTooltipInfo( lua_State *L )
{
	int iPartyNo = luaL_checkint( L, 1 ) - 1;

	if( g_pPartyBoardFrame->HasRaidMemberInfo( iPartyNo ) )
	{
		SPartyBoardRaidInfo info = g_pPartyBoardFrame->GetRaidMemberInfo( iPartyNo );

		lua_pushstring( L, info.Name.Begin() );
		lua_pushnil( L );	// Race
		lua_pushstring( L, g_ObjectData->GetClassNameByID( (Voc_ENUM)info.Voc ) );
		lua_pushstring( L, g_ObjectData->GetClassNameByID( (Voc_ENUM)info.Voc_Sub ) );
		lua_pushnumber( L, info.Level );
		lua_pushnumber( L, info.Level_Sub );

		return 6;
	}

	return 0;	
}

int LuaFunc_PB_UpdateMemberNum( lua_State *L )
{
	// 沒在組隊招募留言板留言則不用更新團隊成員人數
	if( g_pPartyBoardFrame->GetMyPostType() != ePartyBoard_PartyAd )
		return 0;

	int iTab = (int)g_pPartyBoardFrame->GetMyPostType();

	int iNum = g_pPartyBoardFrame->GetPostNum( iTab );

	for( int i = 0; i < iNum; ++i )
	{
		int iPlayerDBID = g_pPartyBoardFrame->GetPoster( iTab, i );

		if( iPlayerDBID == g_pGameMain->GetDBID() )
		{
			SPartyBoardPartyAd post = g_pPartyBoardFrame->GetPartyPost( i );

			// 人數夠了就下架
			if( post.byMemberNumNeed < NetCli_PartyChild::s_member.size() )
			{
				NetCli_Talk::SChat_BillBoardLiveTime( 0 );
				return 0;
			}

			post.byMemberNumNow = (BYTE)NetCli_PartyChild::s_member.size();

			EPartyBoardPostType Type = ePartyBoard_PartyAd;

			int		size		= sizeof( SPartyBoardPartyAd ) + sizeof( EPartyBoardPostType );
			char	*data		= new char[size];

			memcpy( data, &Type, sizeof( EPartyBoardPostType ) );
			memcpy( data + sizeof( EPartyBoardPostType ), &post, sizeof( SPartyBoardPartyAd ) );

			NetCli_Talk::SChat_RegisterBillBoardInfo( size, data );

			delete [] data;

			break;
		}
	}

	return 0;
}

int LuaFunc_PB_UpdateVocAndLevel( lua_State *L )
{
	// 沒在人力資源留言板留言則不用更新留言者職業、等級
	if( g_pPartyBoardFrame->GetMyPostType() != ePartyBoard_PeoplewareAd )
		return 0;

	int iTab = (int)g_pPartyBoardFrame->GetMyPostType();

	int iNum = g_pPartyBoardFrame->GetPostNum( iTab );

	for( int i = 0; i < iNum; ++i )
	{
		int iPlayerDBID = g_pPartyBoardFrame->GetPoster( iTab, i );

		if( iPlayerDBID == g_pGameMain->GetDBID() && g_pGameMain->GetPlayer() )
		{
			SPartyBoardPeoplewareAd post = g_pPartyBoardFrame->GetPeoplewarePost( i );

			CRoleSprite* pSprite = g_pGameMain->GetPlayer();

			post.Voc			= pSprite->GetVocation();
			post.VocLV			= (short)pSprite->GetLevel();
			post.Voc_Sub		= pSprite->GetVocationSub();
			post.Voc_SubLV		= (short)pSprite->GetLevelSub();

			EPartyBoardPostType Type = ePartyBoard_PeoplewareAd;

			int		size		= sizeof( SPartyBoardPeoplewareAd ) + sizeof( EPartyBoardPostType );
			char	*data		= new char[size];

			memcpy( data, &Type, sizeof( EPartyBoardPostType ) );
			memcpy( data + sizeof( EPartyBoardPostType ), &post, sizeof( SPartyBoardPeoplewareAd ) );

			NetCli_Talk::SChat_RegisterBillBoardInfo( size, data );

			delete [] data;

			break;
		}
	}

	return 0;
}

int LuaFunc_GetMyPBPostType( lua_State *L )
{
	int iType = (int)g_pPartyBoardFrame->GetMyPostType();

	lua_pushnumber( L, iType );

	return 1;
}

int	LuaFunc_PB_GetVocInfo( lua_State *L )
{
	int iIndex = luaL_checkint( L, 1 );

	if( iIndex > 0 && iIndex <= g_pPartyBoardFrame->GetPostNum( 2 ) )
	{
		SPartyBoardPeoplewareAd post = g_pPartyBoardFrame->GetPeoplewarePost( iIndex - 1 );

		lua_pushstring( L, g_ObjectData->GetClassNameByID( post.Voc ) );
		lua_pushnumber( L, post.VocLV );
		lua_pushstring( L, g_ObjectData->GetClassNameByID( post.Voc_Sub ) );
		lua_pushnumber( L, post.Voc_SubLV );

		return 4;
	}

	return 0;
}

int LuaFunc_PB_GetServerLvLimit( lua_State *L )
{
	PlayerBaseRoleData* pBaseData = CNetGlobal::RoleData()->PlayerBaseData;

	lua_pushnumber( L, pBaseData->MaxLv );

	return 1;
}