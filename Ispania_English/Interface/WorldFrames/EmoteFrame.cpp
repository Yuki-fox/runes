
#include "../../mainproc/GameMain.h"
#include "../../netwaker_member/NetWakerClientInc.h"
#include "../WorldFrames/ItemClipboard.h"
#include "../WorldFrames/ChatEdit.h"
#include "../WorldFrames/ChatFrame.h"
#include "EmoteFrame.h"

#define EMOTE_ICON_PATH "EMOTE_ICON_PATH"


#define DF_EMOTE_COLDOWN 1000 //多久時間可以發送一次

CEmoteFrame* g_pEmoteFrame = NULL;


//Emote
int LuaFunc_GetEmoteInfoCount(lua_State* L); //
int LuaFunc_GetEmoteInfo(lua_State* L);      //
int LuaFunc_DoEmote(lua_State* L);           //
int	LuaFunc_DragEmoteItem(lua_State* L);

// ----------------------------------------------------------------------------
CEmoteFrame::CEmoteFrame(CInterface* object)
	: CInterfaceBase(object)
{
	g_pEmoteFrame = this;	

	m_pEmoteItemList = NULL;
	m_EmoteItemCount = 0;
	m_totalTime = 0.0f;

	//m_pIconPath = NULL;

}

// ----------------------------------------------------------------------------
CEmoteFrame::~CEmoteFrame()
{
	g_pEmoteFrame = NULL;

	if( m_pEmoteItemList )
		delete [] m_pEmoteItemList;
}


// ----------------------------------------------------------------------------
void CEmoteFrame::RegisterFunc()
{
	//要先初始化　才能給lua呼叫
	// InitEmoteDate();

	lua_State* L = GetLuaStateWorld();
	if (L)
	{
		lua_register( L, "GetEmoteInfo"      , LuaFunc_GetEmoteInfo      );
		lua_register( L, "GetEmoteInfoCount" , LuaFunc_GetEmoteInfoCount );
		lua_register( L, "GetEmoteInfo"      , LuaFunc_GetEmoteInfo      );
		lua_register( L, "DoEmote"           , LuaFunc_DoEmote           );
		lua_register( L, "DragEmoteItem"     , LuaFunc_DragEmoteItem     );
	}
}

// ----------------------------------------------------------------------------
void CEmoteFrame::Update(float elapsedTime)
{
	m_totalTime -= elapsedTime;
	if ( m_totalTime < 0.0f )
		m_totalTime = elapsedTime;
}

void CEmoteFrame::InitEmoteDate()
{
	char buf[128];

	m_IDEmoteItemMap.clear();
	if( m_pEmoteItemList )
	{
		delete [] m_pEmoteItemList;
		m_pEmoteItemList = NULL;
	}

	vector<MotionTableStruct>& EmoteTable = g_ObjectData->GetEmoteTable();
	m_EmoteItemCount = (int)EmoteTable.size();
	if( m_EmoteItemCount == 0 )
		return;

	m_pEmoteItemList = new SEmoteItem[m_EmoteItemCount];

	int i = 0;

	for( ; i < m_EmoteItemCount ; ++i )
	{
		MotionTableStruct* pEmote = &EmoteTable[i];

		m_pEmoteItemList[i].ID        = pEmote->ID       ;
		m_pEmoteItemList[i].MotionID  = pEmote->MotionID ;
		m_pEmoteItemList[i].pIconName = pEmote->IconStr  ;

		sprintf( buf , "EMOTE_%d_NAME" , pEmote->ID );
		m_pEmoteItemList[i].pName  = g_ObjectData->GetString( buf );

		sprintf( buf , "EMOTE_%d_TOKEN" , pEmote->ID );
		m_pEmoteItemList[i].pToken = g_ObjectData->GetString( buf );

		sprintf( buf , "EMOTE_%d_MSG1" , pEmote->ID );
		m_pEmoteItemList[i].pMsg1  = g_ObjectData->GetString( buf );

		sprintf( buf , "EMOTE_%d_MSG2" , pEmote->ID );
		m_pEmoteItemList[i].pMsg2  = g_ObjectData->GetString( buf );


		m_IDEmoteItemMap[ m_pEmoteItemList[i].ID ] = &m_pEmoteItemList[i] ;
	}

	//m_pIconPath = g_ObjectData->GetString( EMOTE_ICON_PATH );
}

// ----------------------------------------------------------------------------
void CEmoteFrame::PickupEmoteItem( int ID )
{
	SEmoteItem* pItem = GetEmoteItemByID( ID );

	if( !pItem )
		return;

	g_pItemClipboard->Clear();			// 清除資料
	g_pItemClipboard->SetName( pItem->pName );
	g_pItemClipboard->SetFile( GetEmoteIcon( pItem ) );
	g_pItemClipboard->PickupEmote(pItem->ID);

}

void CEmoteFrame::DoEmote( int ID )
{
	// 累積超過20秒
	if ( m_totalTime > 20.0f )
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_EMOTE_COLDOWN"));
		return;
	}
	else
	{
		// 每發送一次加值1秒
		m_totalTime += 1.0f;
	}	

	MotionTableStruct* pInfo = g_ObjectData->GetEmoteInfoByID( ID );

	if( !pInfo )
		return ;

	CRoleSprite* player = g_pGameMain->GetPlayer();
	CRoleSprite* target = g_pGameMain->GetPlayerTarget();

	/*
	if ( player && player->GetAttachParentId() != 0 )
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_DOEMOTE_MOUNT_ERROR"));
		return;
	}
	*/

	if (player&&( player->GetSpellID() != 0 ))
		return;

	//對NPC做表情
	if( target&&(target->GetObjectType() != eSpriteObject_Player ))
	{
		

	}
//	if (target&&player)

	NetCli_Other::S_SendEmote( player->GetIdentify() , target ? target->GetName() : NULL , pInfo->ID );
}

void CEmoteFrame::R_DoEmote( int sourceID , const char* szTargetName, int emoteID )
{
	CRoleSprite* pSourceSprite = g_pGameMain->FindSpriteByID( sourceID ) ;

	TD_IDEmoteItemMap::iterator it = m_IDEmoteItemMap.find( emoteID );
	SEmoteItem* pItem = ( it != m_IDEmoteItemMap.end() ) ? it->second : NULL ;

	if( !pSourceSprite || !pItem )
	{
		//來源對象不在範圍內 離開
		return;
	}

	lua_State* L = GetLuaStateWorld();

	CRoleSprite* player = g_pGameMain->GetPlayer();

	string source = pSourceSprite->GetName() ;
	string target = szTargetName ;

//	if( source == player->GetName() )
//		source = g_ObjectData->GetString( "EMOTE_YOU" );
	
//	if( target == player->GetName() )
//		target = g_ObjectData->GetString( "EMOTE_YOU" );

	char msg[256];

	if( target == "" || source == target )
	{
		sprintf( msg , pItem->pMsg2	, source.c_str() );
	}
	else
	{
		sprintf( msg , pItem->pMsg1	, source.c_str() , target.c_str() );
	}

	lua_pushstring( L, msg );
	lua_setglobal( L, "arg1" );	

	g_pChatFrame->SendMsgEvent( CHAT_MSG_EMOTE , msg, "" );

	pSourceSprite->SetActorState( (CRuFusion_ActorState)pItem->MotionID );


}


int CEmoteFrame::GetEmoteItemCount()
{
	return m_EmoteItemCount;
}

SEmoteItem* CEmoteFrame::GetEmoteItemByIndex( int index )
{
	if( index >= 0 && index < m_EmoteItemCount )
		return &m_pEmoteItemList[index];
	return NULL;
}

SEmoteItem* CEmoteFrame::GetEmoteItemByID( int ID )
{
	TD_IDEmoteItemMap::iterator it = m_IDEmoteItemMap.find( ID );
	return ( it != m_IDEmoteItemMap.end() ) ? it->second : NULL ;
}

const char* CEmoteFrame::GetEmoteIcon( int emoteID )
{
	TD_IDEmoteItemMap::iterator it = m_IDEmoteItemMap.find( emoteID );
	if( it != m_IDEmoteItemMap.end() )
		return GetEmoteIcon( it->second );
	_Temp[0] = '\0';
	return _Temp;
}

const char* CEmoteFrame::GetEmoteIcon( SEmoteItem* pItem )
{
	//sprintf( _Temp , "%s%s" , m_pIconPath , pItem->pIconName );
	sprintf( _Temp , "interface/Icons/%s" , pItem->pIconName );
	return _Temp; 
}


// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
int LuaFunc_GetEmoteInfoCount(lua_State* L)
{
	//vector<MotionTableStruct>&  GetEmoteTable();
	//int							GetEmoteInfoCount();
	//MotionTableStruct*			GetEmoteInfoByIndex( int index );
	//MotionTableStruct*			GetEmoteInfoByID( int ID );
	//extern ObjectDataClass*    g_ObjectData;

	//int count = g_ObjectData->GetEmoteInfoCount();

	lua_pushnumber( L, g_pEmoteFrame->GetEmoteItemCount() );

	return 1;
}


// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
int LuaFunc_GetEmoteInfo(lua_State* L)
{

	int index = luaL_checkint(L, 1);

	SEmoteItem* pItem = g_pEmoteFrame->GetEmoteItemByIndex( index );
	if( !pItem )
		return 0;
	
	lua_pushnumber( L, pItem->ID );
	lua_pushstring( L, pItem->pName );	
	lua_pushstring( L, pItem->pToken );	
	lua_pushstring( L, g_pEmoteFrame->GetEmoteIcon( pItem ) );	

	return 4;
}


// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
int LuaFunc_DoEmote(lua_State* L)
{
	int ID = luaL_checkint(L, 1);

	g_pEmoteFrame->DoEmote( ID );

	return 0;
}

// ----------------------------------------------------------------------------
int	LuaFunc_DragEmoteItem(lua_State* L)
{
	int ID = luaL_checkint(L, 1);

	g_pEmoteFrame->PickupEmoteItem( ID );
	return 0;
}