#include "../../mainproc/GameMain.h"
#include "NpcTrackFrame.h"

#include "../Map/MapCommon.h"
#include "../Map/WorldMap.h"

#include "../mainproc/NetGlobal.h"

#include "../Quest/UI_QuestBook.h"

#include "../netwaker_member/net_channel/NetCli_ChannelChild.h"


CNpcTrackFrame* g_pNpcTrackFrame = NULL;

void UiCallback_NpcTrackWorldMap_OnPaint(CUiFrame* frame, INT64 lParam, INT64 rParam);

int	LuaFunc_NpcTrack_Initialize     ( lua_State* L );
int	LuaFunc_NpcTrack_SetMapID       ( lua_State* L );
int LuaFunc_NpcTrack_SearchNpc      ( lua_State* L );
int LuaFunc_NpcTrack_SearchNpcByDBID( lua_State* L );
int LuaFunc_NpcTrack_SearchQuestNpc ( lua_State* L );
int LuaFunc_NpcTrack_GetNpc         ( lua_State* L );
int LuaFunc_NpcTrack_SetTarget      ( lua_State* L );

int LuaFunc_NpcTrack_GetMax         ( lua_State* L );
int LuaFunc_NpcTrack_SetMax         ( lua_State* L );


int LuaFunc_SetTrackPlayerName		( lua_State* L );


int g_NpcTrackMax = 5; //目前沒資料 先暫時

//-------------------------------------------------------------------
CNpcTrackFrame::CNpcTrackFrame( CInterface* object ) : CInterfaceBase( object )
{
	g_pNpcTrackFrame = this;

	m_pUiFrameWorldMap = NULL;
	m_pUiTextureSelf = NULL;

	m_MapID = -1;



	m_TargetInfos;
}

// --------------------------------------------------------------------------------------
CNpcTrackFrame::~CNpcTrackFrame(void)
{
	g_pNpcTrackFrame = NULL;
}

//--------------------------------------------------------------------------------------
void CNpcTrackFrame::RegisterFunc(void)
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{
		lua_register( L , "NpcTrack_Initialize"      , LuaFunc_NpcTrack_Initialize      );
  		lua_register( L , "NpcTrack_SetMapID"        , LuaFunc_NpcTrack_SetMapID        );
		lua_register( L , "NpcTrack_SearchNpc"       , LuaFunc_NpcTrack_SearchNpc       );
  		lua_register( L , "NpcTrack_SearchNpcByDBID" , LuaFunc_NpcTrack_SearchNpcByDBID );
		lua_register( L , "NpcTrack_GetNpc"          , LuaFunc_NpcTrack_GetNpc          );
		lua_register( L , "NpcTrack_SearchQuestNpc"  , LuaFunc_NpcTrack_SearchQuestNpc  );
  		lua_register( L , "NpcTrack_SetTarget"       , LuaFunc_NpcTrack_SetTarget       );
  		lua_register( L , "NpcTrack_GetMax"          , LuaFunc_NpcTrack_GetMax          );
	  	lua_register( L , "NpcTrack_SetMax"          , LuaFunc_NpcTrack_SetMax          );
		
  	  	lua_register( L , "SetTrackPlayerName"       , LuaFunc_SetTrackPlayerName       );

	}
}
//--------------------------------------------------------------------------------------
void CNpcTrackFrame::Update( float elapsedTime )
{
	/*

	static float s_time = 0.0f;
	s_time += elapsedTime;
	static float s_delay = 0.1f;

	if( m_TrackPlayerName.length() > 0 && s_time > s_delay )
	{
		s_time = 0.0f;

		NetCli_Channel::S_CtoC_TrackPlayer( m_TrackPlayerName.c_str() , g_pGameMain->GetPlayerName() );
	}
	*/

	UpdateTargetInfos();
}

//--------------------------------------------------------------------------------------
void CNpcTrackFrame::ChangeZone()
{
	m_bTargetInfodirty = true;
}

//--------------------------------------------------------------------------------------
void CNpcTrackFrame::UpdateTargetInfos()
{
	CRoleSprite* pRoleSpritePlayer = g_pGameMain->GetPlayer();
	const WorldMapInfoStruct* pWorldMapInfoPlayer = g_ObjectData->GetWorldMapInfoByID( g_pGameMain->GetWorldMapID() );

	if( NULL == pRoleSpritePlayer || NULL == pWorldMapInfoPlayer )
	{
		return;
	}

	CRuVector3 vecPos = pRoleSpritePlayer->GetPos();

	const WorldMapInfoStruct* pWorldMapInfoTarget;
	GameObjDbStructEx* pObjDBNpc;
	CRuVector3 vecNpcPos;
		
	//世界搜尋的箭頭，希望走到定位後自動消失。
	for( int index = 0 ; index < DF_NpcTrackTargetInfoMax ; ++index )
	{
		pObjDBNpc = g_ObjectData->GetObj( m_TargetInfos[index].DBID );
		if( pObjDBNpc )
		{
			pWorldMapInfoTarget = g_ObjectData->GetWorldMapInfoByID( pObjDBNpc->NPC.ZoneID );
		 	if( NULL == pWorldMapInfoTarget || 
				NULL == pWorldMapInfoPlayer ||
				0 != strcmp( pWorldMapInfoTarget->wdbFile , pWorldMapInfoPlayer->wdbFile ) )
			{
				continue;
			}

   			vecNpcPos = CRuVector3( (float)pObjDBNpc->NPC.X , (float)pObjDBNpc->NPC.Y , (float)pObjDBNpc->NPC.Z );

			vecNpcPos -= vecPos;

			if( vecNpcPos.Magnitude() <= 50.0f )
			{
				lua_State* L = GetLuaState();

				lua_pushnumber( L, index + 1 );
				lua_setglobal( L, "arg1" );

			  	SendWorldEvent( "NPCTRACK_CLEAR_TARGET" );
				
				break; //上面事件會改變m_TargetInfos 所以停止迴圈
 				//m_TargetInfos[index].DBID = -1;
	 			//m_bTargetInfodirty = true;
			}
		}
	}
 
	if( !m_bTargetInfodirty )
	{
		return ;
	}
  
	for( int index = 0 ; index < DF_NpcTrackTargetInfoMax ; ++index )
	{
		pObjDBNpc = g_ObjectData->GetObj( m_TargetInfos[index].DBID );
		if( NULL == pObjDBNpc )
		{
			pRoleSpritePlayer->HideCompass( index );
			g_pWorldMapFrame->HideTrackTarget( index );
			continue ;
		}

		pWorldMapInfoTarget = g_ObjectData->GetWorldMapInfoByID( pObjDBNpc->NPC.ZoneID );

		if( NULL == pWorldMapInfoTarget || 
			NULL == pWorldMapInfoPlayer ||
			0 != strcmp( pWorldMapInfoTarget->wdbFile , pWorldMapInfoPlayer->wdbFile ) )
		{
			pRoleSpritePlayer->HideCompass( index );
			g_pWorldMapFrame->HideTrackTarget( index );
			return;
		}

		CRuColor4 rucolor(1.0f, m_TargetInfos[index].r, m_TargetInfos[index].g, m_TargetInfos[index].b);
		vecNpcPos = CRuVector3( (float)pObjDBNpc->NPC.X , (float)pObjDBNpc->NPC.Y , (float)pObjDBNpc->NPC.Z );

		pRoleSpritePlayer->CreateCompass( index, "model\\item\\ui_object\\target_direction\\act_target_direction.ros", rucolor.GetARGB() );
		pRoleSpritePlayer->SetCompassParameters( index, vecNpcPos );
		pRoleSpritePlayer->ShowCompass( index );	
		//BOOL							CreateCompass(INT32 id, const char *resourceName, RUCOLOR color);
		//BOOL							SetCompassParameters(INT32 id, const CRuVector3 &destPt);
		//BOOL							HideCompass(INT32 id);
		//BOOL							ShowCompass(INT32 id);

		g_pWorldMapFrame->ShowTrackTarget( index, pWorldMapInfoTarget->wdbFile , (float)pObjDBNpc->NPC.X , (float)pObjDBNpc->NPC.Z , 
			pObjDBNpc->GetName(), EM_MapIconType_TrackNpc, m_TargetInfos[index].r, m_TargetInfos[index].g, m_TargetInfos[index].b );

 	}

	m_bTargetInfodirty = false;

}

//--------------------------------------------------------------------------------------
bool IsInMap( float fPosX , float fPosZ , int MapID )
{
	const WorldMapInfoStruct* pWorldMapInfo = g_ObjectData->GetWorldMapInfoByID( MapID );
	if( !pWorldMapInfo )
	{
		return false ;
	}

	SWorldCoordinate WorldCoord;
	SMapCoordinate  MapCoord;

	WorldCoord.wdbFile = pWorldMapInfo->wdbFile;
	WorldCoord.PosX = fPosX ;
	WorldCoord.PosZ = fPosZ ;

	MapCoord.MapID = MapID;

	if( false == Func_TransformCoordinate_WorldToMap( WorldCoord, MapCoord ) )
	{
		return false;
	}
	return true;
}

//--------------------------------------------------------------------------------------
void CNpcTrackFrame::Initialize( const char* pViewName, const char* pPlayerTexture )
{
	m_pUiFrameWorldMap = (CUiFrame*)g_pGameMain->GetInterface()->GetUiWorld()->FindObject( pViewName );	
	m_pUiTextureSelf   = (CUiTexture*)g_pGameMain->GetInterface()->GetUiWorld()->FindObject( pPlayerTexture );

	if ( !m_pUiFrameWorldMap || !m_pUiTextureSelf )
		return ;

	if( m_pUiFrameWorldMap )
	{
		m_pUiFrameWorldMap->m_pfOnPaint = UiCallback_NpcTrackWorldMap_OnPaint;
	}
	
	int NPC_Begin = Def_ObjectClass_NPC + 1;
	int NPC_End = Def_ObjectClass_QuestNPC - 1;
	int QuestNPC_Begin = Def_ObjectClass_QuestNPC;
	int QuestNPC_End = Def_ObjectClass_Item - 1;

	/*
	GameObjDbStructEx* ObjDB;
  	int i;
  	//尋找目前ZONE所有NPC
	for( i = NPC_Begin ; i < NPC_End ; ++i )
	{
		ObjDB = g_ObjectData->GetObj( i );
		if( !ObjDB )
			continue;

		if( ObjDB->NPC.ZoneID == -1 )
			continue;

		if( ObjDB->NPC.ZoneID == 2 && IsInMap( ObjDB->NPC.X , ObjDB->NPC.Z , 10000 ) )
		{
			ObjDB->NPC.ZoneID = 10000;
 		}
	}

	for( i = QuestNPC_Begin ; i < QuestNPC_End ; ++i )
	{
		ObjDB = g_ObjectData->GetObj( i );
		if( !ObjDB )
			continue;

		if( ObjDB->NPC.ZoneID == -1 )
			continue;

		if( ObjDB->NPC.ZoneID == 2 && IsInMap( ObjDB->NPC.X , ObjDB->NPC.Z , 10000 ) )
		{
			ObjDB->NPC.ZoneID = 10000;
		}
	}
	*/
}

//--------------------------------------------------------------------------------------
void CNpcTrackFrame::RenderWorldMap( CUiFrame* frame, int lParam, int rParam )
{
	if( !m_pUiFrameWorldMap || !m_pUiTextureSelf )
	{
		return;
	}
	
	CRoleSprite* pPlayer = g_pGameMain->GetPlayer();
	if( !pPlayer )
	{
		return;
	}

	const WorldMapInfoStruct* pWorldMapInfo = g_ObjectData->GetWorldMapInfoByID( m_MapID );
	const WorldMapInfoStruct* pWorldMapInfoPlayer = g_ObjectData->GetWorldMapInfoByID( g_pGameMain->GetZoneID() );

	if( NULL == pWorldMapInfo || 
		NULL == pWorldMapInfoPlayer ||
		0 != strcmp( pWorldMapInfo->wdbFile , pWorldMapInfoPlayer->wdbFile ) )
	{
		m_pUiTextureSelf->SetVisible( false );
		return;
	}


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
int CNpcTrackFrame::SearchNpc( const char* pName )
{
	m_TrackNpcInfoTable.clear();

	int NPC_Begin = Def_ObjectClass_NPC + 1;
	int NPC_End = Def_ObjectClass_QuestNPC - 1;
	int QuestNPC_Begin = Def_ObjectClass_QuestNPC;
	int QuestNPC_End = Def_ObjectClass_Item - 1;

	GameObjDbStructEx* ObjDB;

	int i;
	if( !pName || pName[0] == '\0' )
	{
		//尋找目前ZONE所有NPC
		//int ZoneID = g_pGameMain->GetZoneID();
		int ZoneID = g_pGameMain->GetWorldMapID();

		for( i = NPC_Begin ; i < NPC_End ; ++i )
		{
			ObjDB = g_ObjectData->GetObj( i );
			if( !ObjDB || !ObjDB->NPC.IsShowNpcPos )
				continue;

			if( ObjDB->NPC.ZoneID == ZoneID )
			{
				PushTrackNpcInfo( ObjDB, false );
			}
		}

		for( i = QuestNPC_Begin ; i < QuestNPC_End ; ++i )
		{
			ObjDB = g_ObjectData->GetObj( i );
			if( !ObjDB || !ObjDB->NPC.IsShowNpcPos )
				continue;

			if( ObjDB->NPC.ZoneID == ZoneID )
			{
				PushTrackNpcInfo( ObjDB, false );
			}
		}
	}
	else
	{
		char *pdest;
		for( i = NPC_Begin ; i < NPC_End ; ++i )
		{
			ObjDB = g_ObjectData->GetObj( i );
			if( !ObjDB || !ObjDB->NPC.IsShowNpcPos )
				continue;

			if( ObjDB->NPC.ZoneID <= 0 )
				continue;

			pdest = strstr( ObjDB->GetName(), pName );
			if( pdest || !ObjDB->NPC.IsShowNpcPos )
			{
				PushTrackNpcInfo( ObjDB, false );
			}
		}

		for( i = QuestNPC_Begin ; i < QuestNPC_End ; ++i )
		{
			ObjDB = g_ObjectData->GetObj( i );
			if( !ObjDB || !ObjDB->NPC.IsShowNpcPos )
				continue;

			if( ObjDB->NPC.ZoneID <= 0 )
				continue;

			pdest = strstr( ObjDB->GetName(), pName );
			if( pdest )
			{
				PushTrackNpcInfo( ObjDB, false );
			}
		}
	}

	return (int)m_TrackNpcInfoTable.size();
}

int CNpcTrackFrame::SearchNpcByDBID( int DBID )
{
	m_TrackNpcInfoTable.clear();

	int NPC_Begin = Def_ObjectClass_NPC + 1;
	int NPC_End = Def_ObjectClass_QuestNPC - 1;
	int QuestNPC_Begin = Def_ObjectClass_QuestNPC;
	int QuestNPC_End = Def_ObjectClass_Item - 1;

	if( ( DBID >= NPC_Begin && DBID < NPC_End ) || ( DBID >= QuestNPC_Begin && DBID < QuestNPC_End ) )
	{
		GameObjDbStructEx* ObjDB = g_ObjectData->GetObj( DBID );

		if( ObjDB )
		{
			PushTrackNpcInfo( ObjDB, false );
		}
	}

	return (int)m_TrackNpcInfoTable.size();
}

int CNpcTrackFrame::SearchQuestNpc( bool bIgnoreLV, bool bSameZone, bool sendWorldMap )
{
	m_TrackNpcInfoTable.clear();

	RoleDataEx* pRole = CNetGlobal::RoleData();

	int ZoneID = g_pGameMain->GetWorldMapID();

	int QuestNPC_Begin = Def_ObjectClass_QuestNPC;
	int QuestNPC_End = Def_ObjectClass_Item - 1;

	GameObjDbStructEx* pObjNPC;
	GameObjDbStructEx* pObjQuest;

	int iQuestID;
	int* pQuestList;
	GameObjectQuestDetail *pQuest;

	int i,n;
	for( i = QuestNPC_Begin ; i < QuestNPC_End ; ++i )
	{
		pObjNPC = g_ObjectData->GetObj( i );
		if( !pObjNPC )
			continue;

		if( !pObjNPC->NPC.IsShowNpcPos )
   			continue;

		if( pObjNPC->NPC.ZoneID <= 0 )
			continue;

		if( bSameZone && pObjNPC->NPC.ZoneID != ZoneID )
			continue;

		pQuestList = pObjNPC->NPC.iQuestID;
		
		for( n = 0; n < _MAX_NPC_QUEST_COUNT_; ++n )
		{
			iQuestID = pQuestList[ n ];
			if( iQuestID == 0 )
			{
				break;
			}

			// 檢查角色是否已完成該任務
			if( pRole->CheckQuestHistory_IsComplete( iQuestID ) == true )
			{
				continue;
			}

			pObjQuest = g_ObjectData->GetObj( iQuestID );
			if( NULL == pObjQuest )
			{
				continue;
			}

			pQuest = &( pObjQuest->QuestDetail );
   
			// 檢查 Client 身上有無該項任務
			if( pRole->CheckQuestList_IsGotQuest( iQuestID ) != -1 )
			{
				continue;
			}
			
			if( pRole->CheckQuest_IsAvailable( iQuestID, pQuest, bIgnoreLV ) == EM_QUESTCHECK_RESULT_OK )
			{
				bool bLuaCheck = true;
				// 檢查有無 LUA QUESTBEGIN
				if( strlen( pQuest->szScript_OnBegin ) != 0 )
				{
					bLuaCheck = g_pQuestBook->CheckQuestBeginLua( pQuest->szScript_OnBegin );
				}

				if( bLuaCheck )
				{
					PushTrackNpcInfo( pObjNPC, true );
					break;		
				}

			}
		}
	}

 	if( sendWorldMap )
	{
		g_pWorldMapFrame->ClearTrackQuestNPC();



		//完成 待接 每日待接 未完成

		set<int>  completeNPCs;

		int NpcID;

		RoleDataEx* pRole = CNetGlobal::RoleData();
		for( int index = 0; index < _MAX_QUEST_COUNT_; index++ )
		{
			int iQuestID = pRole->PlayerBaseData->Quest.QuestLog[ index ].iQuestID;
			GameObjDbStruct* pObj = g_ObjectData->GetObj( iQuestID );

			if( !pObj )
				continue;

			if( pRole->CheckQuest_IsComplete( index, iQuestID, &( pObj->QuestDetail ) ) == EM_QUESTCHECK_RESULT_OK )
			{
				//任務完成
				//pObj->QuestDetail.iQuestProvider[5];

				NpcID = pObj->QuestDetail.iQuestProvider[0];

				pObjNPC = g_ObjectData->GetObj( i );
				PushTrackNpcInfo( pObjNPC, true );

				completeNPCs.insert( NpcID );

			}
			else
			{
				//任務未完成

			}

		}

		int count = (int)m_TrackNpcInfoTable.size();

		for( i = 0 ; i < count ; ++i )
		{
			const STrackNpcInfo& rInfo = m_TrackNpcInfoTable[i];

			const WorldMapInfoStruct* pWorldMapInfoTarget = g_ObjectData->GetWorldMapInfoByID( rInfo.pObjDB->NPC.ZoneID );

			g_pWorldMapFrame->AddTrackQuestNPC( 
				pWorldMapInfoTarget->wdbFile, 
				(float)rInfo.pObjDB->NPC.X, 
				(float)rInfo.pObjDB->NPC.Z, 
				rInfo.pObjDB->GetName(),
				rInfo.pObjDB->GUID,
				EM_MapIconType_NewQuest );

		}
	}
	return (int)m_TrackNpcInfoTable.size();
}

//--------------------------------------------------------------------------------------
STrackNpcInfo* CNpcTrackFrame::GetNpc( int index )
{
	if( index >= 0 && index < (int)m_TrackNpcInfoTable.size() )
	{
		return &m_TrackNpcInfoTable[index];
	}
	return NULL;
}

//--------------------------------------------------------------------------------------
void CNpcTrackFrame::SetTarget( int index , int DBID , float r, float g, float b )
{
	if( index < 0 || index >= DF_NpcTrackTargetInfoMax )
	{
		return;
	}

	m_TargetInfos[index].DBID = DBID;
	m_TargetInfos[index].r = r;
	m_TargetInfos[index].g = g;
	m_TargetInfos[index].b = b;

	m_bTargetInfodirty = true;

}


void CNpcTrackFrame::R_CtoC_TrackPlayerResult( const char* Name, int WorldMapID, float fX, float fY, float fZ )
{
	_SetTarget( 9, Name, WorldMapID, fX, fY, fZ, 1.0, 0.0, 0.0 );
}

bool CNpcTrackFrame::IsNpcVisible( GameObjDbStructEx* pObjDB, bool bCheckQuest )
{
	if( !pObjDB || !pObjDB->NPC.IsShowNpcPos || pObjDB->NPC.ZoneID <= 0 )
		return false;

	const WorldMapInfoStruct* pWorldMapInfo = g_ObjectData->GetWorldMapInfoByID( pObjDB->NPC.ZoneID );
	if( !pWorldMapInfo )
	{
		return false;
	}

	//EM_WorldMapIType_None           = 0 , //不使用
	//EM_WorldMapIType_World          = 1 , //大陸
	//EM_WorldMapIType_Zone           = 2 , //地區
	//EM_WorldMapIType_City           = 3 , //城市
	//EM_WorldMapIType_Instance       = 4 , //副本
	//EM_WorldMapIType_Battleground   = 5 , //戰場

	if( pWorldMapInfo->Type == EM_WorldMapIType_World         ||
		pWorldMapInfo->Type == EM_WorldMapIType_Zone          ||
		pWorldMapInfo->Type == EM_WorldMapIType_City          ||
		pWorldMapInfo->Type == EM_WorldMapIType_WorldInstance ||
		g_pGameMain->IsShowServerDebugMsg()                   )
	{

	}
	else
	{
		return false;
	}

	// 執行本地顯示劇情
	if( pObjDB->Classification == EM_ObjectClass_QuestNPC )
	{
		if( strlen( pObjDB->NPC.szLuaDisplayScript ) != 0 )
		{
			if( g_pQuestBook->CheckQuestBeginLua( pObjDB->NPC.szLuaDisplayScript ) == true )
			{					
				return false;
			}
		}

		if( bCheckQuest )
		{
			GameObjDbStructEx* pObjQuest;
			GameObjectQuestDetail *pQuest;

			RoleDataEx* pRole = CNetGlobal::RoleData();

			int iQuestID;
			int* pQuestList;

			pQuestList = pObjDB->NPC.iQuestID;

			for( int n = 0; n < _MAX_NPC_QUEST_COUNT_; ++n )
			{
				iQuestID = pQuestList[ n ];
				if( iQuestID == 0 )
				{
					break;
				}

				pObjQuest = g_ObjectData->GetObj( iQuestID );
				if( NULL == pObjQuest )
				{
					continue;
				}

				pQuest = &( pObjQuest->QuestDetail );

				// 檢查有無 LUA QUESTBEGIN
				if( strlen( pQuest->szScript_OnBegin ) != 0 )
				{
					if( false == g_pQuestBook->CheckQuestBeginLua( pQuest->szScript_OnBegin ) )
					{
						return false;
					}
				}
			}
		}
	}

	return true;
}

void CNpcTrackFrame::_SetTarget( int index, const char* Name, int WorldMapID, float fX, float fY, float fZ, float r , float g , float b )
{
	if( index < 0 || index >= 10 )
	{
		return;
	}

	CRoleSprite* pRoleSpritePlayer = g_pGameMain->GetPlayer();

	if( NULL == pRoleSpritePlayer )
	{
		return;
	}

	const WorldMapInfoStruct* pWorldMapInfoTarget = g_ObjectData->GetWorldMapInfoByID( WorldMapID );
	const WorldMapInfoStruct* pWorldMapInfoPlayer = g_ObjectData->GetWorldMapInfoByID( g_pGameMain->GetWorldMapID() );

	if( NULL == pWorldMapInfoTarget || 
		NULL == pWorldMapInfoPlayer ||
		0 != strcmp( pWorldMapInfoTarget->wdbFile , pWorldMapInfoPlayer->wdbFile ) )
	{
		pRoleSpritePlayer->HideCompass( index );
		g_pWorldMapFrame->HideTrackTarget( index );
		return;
	}

	CRuColor4 rucolor(1.0f, r, g, b);
	CRuVector3 pos( fX, fY, fZ );

	pRoleSpritePlayer->CreateCompass(index, "model\\item\\ui_object\\target_direction\\act_target_direction.ros", rucolor.GetARGB() );
	pRoleSpritePlayer->SetCompassParameters(index, pos);
	pRoleSpritePlayer->ShowCompass(index);	
	//BOOL							CreateCompass(INT32 id, const char *resourceName, RUCOLOR color);
	//BOOL							SetCompassParameters(INT32 id, const CRuVector3 &destPt);
	//BOOL							HideCompass(INT32 id);
	//BOOL							ShowCompass(INT32 id);

	g_pWorldMapFrame->ShowTrackTarget( index, pWorldMapInfoTarget->wdbFile , fX , fZ ,Name, EM_MapIconType_TrackNpc, r, g, b );

}

void CNpcTrackFrame::_ClearTarget( int index )
{
	if( index < 0 || index >= DF_NpcTrackTargetInfoMax )
	{
		return;
	}

	m_bTargetInfodirty = true;

	m_TargetInfos[index].DBID = -1;
}

//--------------------------------------------------------------------------------------
void CNpcTrackFrame::PushTrackNpcInfo( GameObjDbStructEx* pObjDB, bool bCheckQuest )
{
	if( !IsNpcVisible( pObjDB, bCheckQuest ) )
	{
		return;
	}

	const WorldMapInfoStruct* pWorldMapInfo = g_ObjectData->GetWorldMapInfoByID( pObjDB->NPC.ZoneID );
	
	SWorldCoordinate WorldCoord;
	SMapCoordinate  MapCoord;

	WorldCoord.wdbFile = pWorldMapInfo->wdbFile;
	WorldCoord.PosX = (float)pObjDB->NPC.X ;
	WorldCoord.PosZ = (float)pObjDB->NPC.Z ;

	MapCoord.MapID = pObjDB->NPC.ZoneID;

	if( false == Func_TransformCoordinate_WorldToMap( WorldCoord, MapCoord ) )
	{
		return;
	}	

	STrackNpcInfo TrackNpcInfo ;
	TrackNpcInfo.pObjDB = pObjDB;
	TrackNpcInfo.MapID = MapCoord.MapID;
	TrackNpcInfo.PosX  = MapCoord.PosX ;
	TrackNpcInfo.PosZ  = MapCoord.PosZ ;

	m_TrackNpcInfoTable.push_back( TrackNpcInfo );
}

//--------------------------------------------------------------------------------------
void UiCallback_NpcTrackWorldMap_OnPaint(CUiFrame* frame, INT64 lParam, INT64 rParam)
{
	g_pNpcTrackFrame->RenderWorldMap( frame , (int)lParam , (int)rParam );	
}

//--------------------------------------------------------------------------------------
int	LuaFunc_NpcTrack_Initialize( lua_State* L )
{
	const char* pViewName      = luaL_checkstring( L, 1 );
	const char* pPlayerTexture = luaL_checkstring( L, 2 );

	g_pNpcTrackFrame->Initialize( pViewName, pPlayerTexture );
	return 0;
}

int	LuaFunc_NpcTrack_SetMapID( lua_State* L )
{
	g_pNpcTrackFrame->m_MapID = (int)luaL_checknumber( L, 1 );
	return 0;
}

//--------------------------------------------------------------------------------------
int LuaFunc_NpcTrack_SearchNpc( lua_State* L )
{
	const char* pName = luaL_checkstring( L, 1 );
	int count = g_pNpcTrackFrame->SearchNpc( pName );
	lua_pushnumber( L, count );
	return 1;
}

//--------------------------------------------------------------------------------------
int LuaFunc_NpcTrack_SearchNpcByDBID( lua_State* L )
{
	int DBID = luaL_checkint( L, 1 );
	int count = g_pNpcTrackFrame->SearchNpcByDBID( DBID );
	lua_pushnumber( L, count );
	return 1;
}

//--------------------------------------------------------------------------------------
int LuaFunc_NpcTrack_SearchQuestNpc( lua_State* L )
{
	bool bIgnoreLV = lua_toboolean(L, 1) == 0 ? false : true;
	bool bSameZone = lua_toboolean(L, 2) == 0 ? false : true;
	int count = g_pNpcTrackFrame->SearchQuestNpc( bIgnoreLV, bSameZone );
	lua_pushnumber( L, count );
	return 1;
}

//--------------------------------------------------------------------------------------
int LuaFunc_NpcTrack_GetNpc( lua_State* L )
{
	int index = (int)luaL_checknumber( L, 1 ) - 1;
	STrackNpcInfo* pInfo = g_pNpcTrackFrame->GetNpc( index );
	if( !pInfo )
	{
		return 0;
	}

	GameObjDbStructEx* pObjDB = pInfo->pObjDB ;

	if( !pObjDB )
	{
		return 0;
	}

	lua_pushnumber( L, pObjDB->GUID );		// ID
	lua_pushstring( L, pObjDB->GetName() );		// NPC 名稱
	lua_pushnumber( L, pInfo->MapID );
	lua_pushnumber( L, pInfo->PosX  );
	lua_pushnumber( L, pInfo->PosZ  );

	return 5;
}

//--------------------------------------------------------------------------------------
int LuaFunc_NpcTrack_SetTarget( lua_State* L )
{
	int n = lua_gettop(L);
	int index = (int)luaL_checknumber( L, 1 ) - 1;
 	int DBID  = (int)luaL_checknumber( L, 2 );
	
	if( n == 2)
	{
		g_pNpcTrackFrame->SetTarget( index, DBID );
		return 0;
	}

	if( index >= g_NpcTrackMax )
	{
		g_pNpcTrackFrame->SetTarget( index, -1 );
		return 0;	
	}

	float r = (float)luaL_checknumber( L, 3 );
	float g = (float)luaL_checknumber( L, 4 );
	float b = (float)luaL_checknumber( L, 5 );

	g_pNpcTrackFrame->SetTarget( index, DBID, r, g, b );

  	return 0;	
}

//--------------------------------------------------------------------------------------
int LuaFunc_NpcTrack_GetMax( lua_State* L )
{
	lua_pushnumber( L, g_NpcTrackMax );		// ID
	return 1;
}

//--------------------------------------------------------------------------------------
int LuaFunc_NpcTrack_SetMax( lua_State* L )
{
	int Max = (int)luaL_checknumber( L, 1 );
	g_NpcTrackMax = Max;

	if( g_NpcTrackMax < 1 )
		g_NpcTrackMax = 1;

	if( g_NpcTrackMax > 10 )
		g_NpcTrackMax = 10;

	return 0;
}

//--------------------------------------------------------------------------------------
int LuaFunc_SetTrackPlayerName( lua_State* L )
{
  	const char* Name = luaL_checkstring( L, 1 );


	g_pNpcTrackFrame->m_TrackPlayerName = Name;

	return 0;
}
















