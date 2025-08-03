#include "../../mainproc/GameMain.h"
#include "../../netwaker_member/NetWakerClientInc.h"

#include "QuestTrack.h"
#include "../../interface/quest/UI_QuestDetail.h"
#include "../../interface/quest/UI_QuestList.h"
#include "../../interface/quest/UI_Questbook.h"

CQuestTrackFrame* g_pQuestTrackFrame = NULL;

void UiCallback_QuestTrackWorldMap_OnPaint(CUiFrame* frame, INT64 lParam, INT64 rParam);



int	LuaFunc_InitializeQuestTrackFrame( lua_State* L );

int LuaFunc_GetTargetSource(lua_State* L);

int LuaFunc_Daily_count(lua_State* L);
int LuaFunc_IsDailyQuest(lua_State* L);
int LuaFunc_QuestName(lua_State* L);
int LuaFunc_GetQuestId(lua_State* L);
//-------------------------------------------------------------------
CQuestTrackFrame::CQuestTrackFrame( CInterface* object ) : CInterfaceBase( object )
{
	g_pQuestTrackFrame = this;

	m_pUiFrameWorldMap = NULL;
	m_pUiTextureSelf = NULL;
}
// --------------------------------------------------------------------------------------
CQuestTrackFrame::~CQuestTrackFrame(void)
{
	g_pQuestTrackFrame = NULL;
}
//--------------------------------------------------------------------------------------
void CQuestTrackFrame::RegisterFunc(void)
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{
		lua_register( L, "InitializeQuestTrackFrame" ,	LuaFunc_InitializeQuestTrackFrame	);
		lua_register( L, "GetTargetSource",				LuaFunc_GetTargetSource				);
		lua_register( L, "Daily_count",					LuaFunc_Daily_count					);
		lua_register( L, "IsDailyQuest",				LuaFunc_IsDailyQuest				);
		lua_register( L, "QuestName",					LuaFunc_QuestName					);
		lua_register( L, "GetQuestId",					LuaFunc_GetQuestId					);

		
	}
}

//--------------------------------------------------------------------------------------
void CQuestTrackFrame::Initialize( const char* pViewName, const char* pPlayerTexture )
{
	m_pUiFrameWorldMap = (CUiFrame*)g_pGameMain->GetInterface()->GetUiWorld()->FindObject( pViewName );	
	m_pUiTextureSelf   = (CUiTexture*)g_pGameMain->GetInterface()->GetUiWorld()->FindObject( pPlayerTexture );

	if ( !m_pUiFrameWorldMap || !m_pUiTextureSelf )
		return ;

	if( m_pUiFrameWorldMap )
	{
		m_pUiFrameWorldMap->m_pfOnPaint = UiCallback_QuestTrackWorldMap_OnPaint;
	}
}

//--------------------------------------------------------------------------------------
void CQuestTrackFrame::RenderWorldMap( CUiFrame* frame, int lParam, int rParam )
{
	const WorldMapInfoStruct* pWorldMapInfo = g_ObjectData->GetWorldMapInfoByID( g_pGameMain->GetWorldMapID() );

	CRoleSprite* pPlayer = g_pGameMain->GetPlayer();

	if( !pWorldMapInfo || !pPlayer || !m_pUiFrameWorldMap || !m_pUiTextureSelf )
		return;

	// 計算座標
	CUiSize FrameSize = m_pUiFrameWorldMap->GetSize();
	CRuVector3 pos;
	CUiAnchor anchor;
	CUiPoint   Point;

	float left   = (float)pWorldMapInfo->LX; 
	float top    = (float)pWorldMapInfo->LZ; 
	float right  = (float)pWorldMapInfo->RX;
	float bottom = (float)pWorldMapInfo->RZ;

	// 設定角色游標座標
	pos = pPlayer->GetPos();
	if( pos.m_x > left && pos.m_x < right  && 
		pos.m_z < top  && pos.m_z > bottom )
	{
		m_pUiTextureSelf->SetVisible( true );

		Point.m_x = ( pos.m_x - left ) / ( right - left ) * FrameSize.m_x ;
		Point.m_y = ( pos.m_z - top  ) / ( bottom - top ) * FrameSize.m_y ;

		anchor.m_relativeTo = m_pUiFrameWorldMap;
		anchor.m_offset.m_x = Point.m_x;	
		anchor.m_offset.m_y = Point.m_y;
		anchor.m_point = CUiAnchor::eAnchor_Center;
		anchor.m_relativePoint = CUiAnchor::eAnchor_TopLeft;

		m_pUiTextureSelf->ClearAllAnchors();
		m_pUiTextureSelf->SetAnchor( anchor );

		// 設定角色游標方向
		CRuVector3 direction = pPlayer->GetDirection();

		float angle = asin( direction.m_x );
		if( direction.z < 0.0f )
		{
			angle = ruPI - angle ;
		}

		m_pUiTextureSelf->SetRotate( angle );
	}
	else
	{
		m_pUiTextureSelf->SetVisible( false );
	}

}

//--------------------------------------------------------------------------------------
void UiCallback_QuestTrackWorldMap_OnPaint(CUiFrame* frame, INT64 lParam, INT64 rParam)
{
	g_pQuestTrackFrame->RenderWorldMap( frame , (int)lParam , (int)rParam );	
}

//--------------------------------------------------------------------------------------
int LuaFunc_InitializeQuestTrackFrame( lua_State* L )
{
	int n = lua_gettop(L);
	if( n != 2 )
	{
		return 0;
	}

	const char* pViewName      = luaL_checkstring( L, 1 );
	const char* pPlayerTexture = luaL_checkstring( L, 2 );

	g_pQuestTrackFrame->Initialize( pViewName, pPlayerTexture );

	return 0;
}


//--------------------------------------------------------------------------------------------

int LuaFunc_GetTargetSource(lua_State* L)
{
	int	iIndex = luaL_checkint( L, 1 );
	int iItemID_count = luaL_checkint( L, 2 );
	int i = luaL_checkint( L, 3 );
	int	iQuestGUID	= g_pQuestBook->GetQuestGUID( ( iIndex - 1 ) );
	GameObjDbStructEx*	pObj = g_ObjectData->GetObj( iQuestGUID );   //GameObjDbStructEx NO.91 ObjectStructEx.h

	
	//int j , k = 0;
	//char szPreString[256];	
	string strStr1,strStr2;


	//char szKeyWord[256];
	//int iItemID;

	int iGUID					= pObj->QuestDetail.iRequest_KillID[ i ];
	GameObjDbStructEx* pRequest	= g_ObjectData->GetObj( iGUID );
	int retr1 ,retr2,retr3;
	int QT_stop = 0;

	retr1 = pObj->QuestDetail.iProcess_KilltoGetItem_ObjID[i]; //任務目標		
	retr2 = pObj->QuestDetail.iProcess_KilltoGetItem_GetItem[i]; //任務需求
	retr3 = pObj->QuestDetail.iProcess_ClickToGetItem_GetItem[i]; //點選會得到任務需求

	if(pObj->QuestDetail.iProcess_KilltoGetItem_GetItem[i+1] == NULL)
	{
		QT_stop = 1;		
	}
//	iProcess_KilltoGetItem_ObjID  
//	iProcess_ClickToGetItem_ObjID


	GameObjDbStructEx* itemDB1 = g_ObjectData->GetObj( retr1 );
	if(itemDB1)
	{
		strStr1 = itemDB1->GetName();
	}
	
	GameObjDbStructEx* itemDB2 = g_ObjectData->GetObj( retr3 );
	if(itemDB2)
	{
		strStr2 = itemDB2->GetName();
	}

	lua_pushnumber(L, retr1);
	lua_pushnumber(L, retr2);
	lua_pushnumber(L, retr3);

	lua_pushstring(L, strStr1.c_str() );
	lua_pushstring(L, strStr2.c_str() );

	lua_pushnumber(L, QT_stop);
	
	return 6;
}

//--------------------------------------------------------------------------------------------
int LuaFunc_Daily_count(lua_State* L)
{
	RoleDataEx*			pRole		= CNetGlobal::RoleData();
	int Daily_count_finish = pRole->PlayerBaseData->Quest.iQuestGroup[ 1 ] ; 
	int Daily_count = 10;
	lua_pushnumber( L, Daily_count_finish);
	lua_pushnumber( L, Daily_count);
	return 2;
}

//--------------------------------------------------------------------------------------------
int LuaFunc_IsDailyQuest(lua_State* L) //判斷是否是每日任務
{
	int iIndex		= luaL_checkint( L, 1 ) - 1;
	int	iQuestID	= g_pQuestBook->m_vecQuestID[ iIndex ];
	GameObjDbStructEx*	pObj		= g_ObjectData->GetObj( iQuestID );
	int lua_bDaily_num = 0;
	if(pObj){
		if( pObj->QuestDetail.iControlType != 0 || pObj->QuestDetail.iQuestGroup != 0 )
		{
			lua_bDaily_num = 1;     // 1代表每日任務
		}
	}
	lua_pushnumber( L, lua_bDaily_num );	
	return 1;
}
//--------------------------------------------------------------------------------------------
int LuaFunc_QuestName(lua_State* L) //回傳任務名稱
{
	int iIndex		= luaL_checkint( L, 1 ) - 1;
	int	iQuestID	= g_pQuestBook->m_vecQuestID[ iIndex ];
	GameObjDbStructEx*	pObj		= g_ObjectData->GetObj( iQuestID );

	char szQuestName[256];
	if(pObj) 
	{
		strcpy( szQuestName, pObj->GetName());
		lua_pushstring( L, szQuestName	);
		return 1;
	}
	return 0;
}
//--------------------------------------------------------------------------------------------
int LuaFunc_GetQuestId(lua_State* L)
{
	int iIndex		= luaL_checkint( L, 1 ) - 1;	
	int	iQuestID	= g_pQuestBook->GetQuestGUID(iIndex);
	GameObjDbStructEx*	pObj		= g_ObjectData->GetObj( iQuestID );
	if(pObj)
	{
		lua_pushnumber( L, iQuestID );	
		return 1;
	}
	return 0;
}
//--------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
