#include "../mainproc/GameMain.h"
#include "WorldMap.h"
#include "../../netwaker_member/NetWakerClientInc.h"

#include "../WorldFrames/NpcTrackFrame.h"
#include "../Quest/UI_QuestBook.h"


CWorldMapFrame* g_pWorldMapFrame = NULL;

static float g_fWorldMapIconSize = 16.0f;
static float g_fWorldMapSelfIconSize = 32.0f;

void UiCallback_WorldMap_OnPaint(CUiFrame* frame, INT64 lParam, INT64 rParam);
void UiCallback_AreaMap_OnPaint(CUiFrame* frame, INT64 lParam, INT64 rParam);


void UiCallback_WorldMapIconFrame_OnMouseEnter(CUiFrame* frame, INT64 lParam, INT64 rParam);
void UiCallback_WorldMapIconFrame_OnMouseLeave(CUiFrame* frame, INT64 lParam, INT64 rParam);
void UiCallback_WorldMapIconFrame_OnDoubleClick(CUiFrame* frame, INT64 lParam, INT64 rParam);
void UiCallback_WorldMapIconFrame_OnClick(CUiFrame* frame, INT64 lParam, INT64 rParam);


int	LuaFunc_InitializeWorldMap       ( lua_State* L );
int	LuaFunc_InitializeAreaMap        ( lua_State* L );
int LuaFunc_GetPlayerWorldMapPos     ( lua_State* L ); //玩家在目前地圖的相對位置
int LuaFunc_GetCurrentWorldMapID	 ( lua_State* L );
int luaFunc_SetWorldMapID			 ( lua_State* L );
int luaFunc_SetAreaMapID			 ( lua_State* L );

int luaFunc_SetWorldMapPingPosition  ( lua_State* L );
int luaFunc_GetWorldMapPingPosition  ( lua_State* L );
int luaFunc_GetWorldMapPingMapID	 ( lua_State* L );

int luaFunc_WorldMap_GetWorldCount   ( lua_State* L );
int LuaFunc_WorldMap_GetWorld        ( lua_State* L );

int luaFunc_WorldMap_GetMapCount     ( lua_State* L );
int luaFunc_WorldMap_GetMap          ( lua_State* L );

int luaFunc_WorldMap_SearchQuestNpc  ( lua_State* L );
int luaFunc_WorldMap_ClearQuestNpc   ( lua_State* L );

int LuaFunc_WorldMap_SetShowQuestNpc ( lua_State* L );
int LuaFunc_WorldMap_SetShowParty    ( lua_State* L );
int LuaFunc_WorldMap_SetShowTrackNpc ( lua_State* L );

int luaFunc_GetZoneEnglishName       ( lua_State* L ); 
int luaFunc_GetZoneLocalName         ( lua_State* L ); 

int luaFunc_WorldMap_AddIcon         ( lua_State* L );
int luaFunc_WorldMap_ClearIcon       ( lua_State* L ); 

int luaFunc_GetIconTextureRect       ( lua_State* L ); 

int luaFunc_WorldMap_AutoMove		 ( lua_State* L );

int luaFunc_WorldMap_AutoMoveByNpcID ( lua_State* L );
//int luaFunc_



// ----------------------------------------------------------------------------
CWorldMapFrame::CWorldMapFrame(CInterface* object): CInterfaceBase(object)
{
	g_pWorldMapFrame = this;
   
   	m_PingMapID = 0;

	m_TrackTeammate.resize( _MAX_PARTY_COUNT_ );
	m_TrackNPC.resize( DF_TRACKINFO_MAX );

	m_bGmMode = false;

	m_WorldMapView.eType = CWorldMapView::WMV_WorldMap;
	m_AreaMapView.eType = CWorldMapView::WMV_AreaMap;


	m_SearchQuestNpc_bIgnoreLV = false;
	m_SearchQuestNpc_DirtyTime = -1;

	m_SearchQuestNpc_ShowFlag[0] = true;
	m_SearchQuestNpc_ShowFlag[1] = true;
	m_SearchQuestNpc_ShowFlag[2] = true;
	m_SearchQuestNpc_ShowFlag[3] = true;
}

// ----------------------------------------------------------------------------
CWorldMapFrame::~CWorldMapFrame()
{
	g_pWorldMapFrame = NULL;
}

// ----------------------------------------------------------------------------
void CWorldMapFrame::RegisterFunc()
{

	lua_State* L = GetLuaStateWorld();
	if (L)
	{
		lua_register( L , "InitializeWorldMap"        , LuaFunc_InitializeWorldMap        );
		lua_register( L , "InitializeAreaMap"         , LuaFunc_InitializeAreaMap         );
   		lua_register( L , "GetPlayerWorldMapPos"      , LuaFunc_GetPlayerWorldMapPos      );
		lua_register( L , "GetCurrentWorldMapID"      , LuaFunc_GetCurrentWorldMapID      );
		lua_register( L , "SetWorldMapID"             , luaFunc_SetWorldMapID             );
		lua_register( L , "SetAreaMapID"              , luaFunc_SetAreaMapID              );
		lua_register( L , "SetWorldMapPingPosition"   , luaFunc_SetWorldMapPingPosition   );
		lua_register( L , "GetWorldMapPingPosition"   , luaFunc_GetWorldMapPingPosition   );
 		lua_register( L , "GetWorldMapPingMapID"	  , luaFunc_GetWorldMapPingMapID      );
   		lua_register( L , "WorldMap_GetWorldCount"    , luaFunc_WorldMap_GetWorldCount    );
		lua_register( L , "WorldMap_GetWorld"         , LuaFunc_WorldMap_GetWorld         );

		lua_register( L , "WorldMap_GetMapCount"      , luaFunc_WorldMap_GetMapCount );
		lua_register( L , "WorldMap_GetMap"           , luaFunc_WorldMap_GetMap      );

		lua_register( L , "WorldMap_SearchQuestNpc"   , luaFunc_WorldMap_SearchQuestNpc   );
		lua_register( L , "WorldMap_ClearQuestNpc"    , luaFunc_WorldMap_ClearQuestNpc    );

		lua_register( L , "WorldMap_SetShowQuestNpc"  , LuaFunc_WorldMap_SetShowQuestNpc  );
		lua_register( L , "WorldMap_SetShowParty"     , LuaFunc_WorldMap_SetShowParty     );
		lua_register( L , "WorldMap_SetShowTrackNpc"  , LuaFunc_WorldMap_SetShowTrackNpc  );

		lua_register( L , "GetZoneEnglishName"        , luaFunc_GetZoneEnglishName        );
		lua_register( L , "GetZoneLocalName"          , luaFunc_GetZoneLocalName          );

		lua_register( L , "WorldMap_AddIcon"          , luaFunc_WorldMap_AddIcon          );
		lua_register( L , "WorldMap_ClearIcon"        , luaFunc_WorldMap_ClearIcon        );

 		lua_register( L , "GetIconTextureRect"        , luaFunc_GetIconTextureRect        );

  		lua_register( L , "WorldMap_AutoMove"         , luaFunc_WorldMap_AutoMove         );

  		lua_register( L , "WorldMap_AutoMoveByNpcID"  , luaFunc_WorldMap_AutoMoveByNpcID  );


	}
}



// ----------------------------------------------------------------------------
void CWorldMapFrame::InitializeWorldMap( const char* pViewName , const char* pPlayerCursor )
{
	m_WorldMapView.Init( pViewName, pPlayerCursor );
}

// ----------------------------------------------------------------------------
void CWorldMapFrame::InitializeAreaMap( const char* pViewName , const char* pPlayerCursor )
{
	m_AreaMapView.Init( pViewName, pPlayerCursor );
}

// ----------------------------------------------------------------------------
void CWorldMapFrame::DestroyFrames()
{
	m_WorldMapView.Fini();
	m_AreaMapView.Fini();
}

// ----------------------------------------------------------------------------
void CWorldMapFrame::Update( float elapsedTime )
{
	//更新隊友位置

	static DWORD dwLastUpdate = 0;
	if( GetTickCount() > dwLastUpdate )
	{
		dwLastUpdate = GetTickCount() + 200;

		bool isParty = false;
		bool isRaid = false;

		if( g_pGameMain->IsBattleGroundZone() )
		{
			isRaid = true;
		}
		else
		{
			PartyBaseInfoStruct& rPartyInfo = NetCli_PartyChild::GetPartyInfo();
			if( rPartyInfo.PartyID != -1 )
			{
				isParty = true;
			}

			if( rPartyInfo.Type == EM_PartyType_Raid )
			{
				isRaid = true;
			}
		}

		string strType;
		int count;
		if( isRaid )
		{
			strType = "raid%d";
			count = 36;
		}
		else
		{
  			strType = "party%d";
			count = 5;
		}


		CRoleSprite* sprite;
		int zoneID;
		GameObjDbStruct* ZoneDB;

		char unit[32];
		for( int i = 0 ; i < count ; ++i )
		{
			sprintf( unit, strType.c_str(), i + 1 );

			sprite = g_pGameMain->GetUnitSprite( unit );

			if( NULL == sprite )
			{
				m_TrackTeammate[i].show = false;
				continue;
			}

			if( sprite == g_pGameMain->GetPlayer() )
			{
				m_TrackTeammate[i].show = false;
				continue;
			}

			zoneID = ( sprite->GetIdentify() >> 16 ) % _DEF_ZONE_BASE_COUNT_ ;
			ZoneDB = CNetGlobal::GetObj(  zoneID + Def_ObjectClass_Zone );
			if( ZoneDB == NULL )
			{
				m_TrackTeammate[i].show = false;
				continue;
			}

			CRuVector3 pos = sprite->GetPos();
			if( pos.x == 0 && pos.z == 0 )
			{
				m_TrackTeammate[i].show = false;
				continue;				
			}

			m_TrackTeammate[i].show = true;
			m_TrackTeammate[i].coord.wdbFile = ZoneDB->Zone.MapFile ;
			m_TrackTeammate[i].coord.PosX = pos.x;
			m_TrackTeammate[i].coord.PosZ = pos.z;
			m_TrackTeammate[i].strName = sprite->GetName();
			m_TrackTeammate[i].r = 1.0f;
			m_TrackTeammate[i].g = 1.0f;
			m_TrackTeammate[i].b = 1.0f;
			m_TrackTeammate[i].size = 16;
			m_TrackTeammate[i].id = -1;
			m_TrackTeammate[i].trackArrow = true;

			Voc_ENUM eVoc = sprite->GetVocation();

			switch( eVoc )
			{
			case EM_Vocation_Warrior    : m_TrackTeammate[i].icontype = EM_MapIconType_Warrior    ; break; // 戰士
			case EM_Vocation_Ranger     : m_TrackTeammate[i].icontype = EM_MapIconType_Ranger     ; break; // 遊俠
			case EM_Vocation_Rogue      : m_TrackTeammate[i].icontype = EM_MapIconType_Rogue      ; break; // 盜賊
			case EM_Vocation_Wizard     : m_TrackTeammate[i].icontype = EM_MapIconType_Wizard     ; break; // 法師
			case EM_Vocation_Priest     : m_TrackTeammate[i].icontype = EM_MapIconType_Priest     ; break; // 僧侶
			case EM_Vocation_Knight     : m_TrackTeammate[i].icontype = EM_MapIconType_Knight     ; break; // 騎士
			case EM_Vocation_RuneDancer : m_TrackTeammate[i].icontype = EM_MapIconType_RuneDancer ; break; // 言靈使
			case EM_Vocation_Druid      : m_TrackTeammate[i].icontype = EM_MapIconType_Druid      ; break; // 德魯伊
			case EM_Vocation_Harpsyn    : m_TrackTeammate[i].icontype = EM_MapIconType_Harpsyn    ; break; // 靈能者
			case EM_Vocation_Psyron     : m_TrackTeammate[i].icontype = EM_MapIconType_Psyron     ; break; // 符文工匠
			case EM_Vocation_Duelist	: m_TrackTeammate[i].icontype = EM_MapIconType_Duelist   ; break; // 鬥棋士
			default:
				m_TrackTeammate[i].icontype = EM_MapIconType_NPC  ;
			}

			//有敵意 可能是決鬥中
			if( sprite->GetCanAttackState() && sprite->GetEnemyState() ) 
			{
				m_TrackTeammate[i].show = false;
			}
		}

		for( int i = count ; i < _MAX_PARTY_COUNT_ ; ++i )
		{
			m_TrackTeammate[i].show = false;
		}
 	}


	//m_SearchQuestNpc_bIgnoreLV = bIgnoreLV;
	if( m_SearchQuestNpc_DirtyTime >= 0 && timeGetTime() > (DWORD)( m_SearchQuestNpc_DirtyTime + 1000 ) )
	{
		m_SearchQuestNpc_DirtyTime = -1;

		//m_SearchQuestNpc_DirtyTime = g_pGameMain->GetGameTime();
		g_pWorldMapFrame->ClearTrackQuestNPC();

		set<int> DisplayedNpcs;

  		vector<int> QuestNpcs[5];
		int IconTypes[5] = { 
			EM_MapIconType_FinishedQuest , 
			EM_MapIconType_NewQuest      , 
			EM_MapIconType_TrustQuest    , 
			EM_MapIconType_UnfinishQuest ,
			EM_MapIconType_PublicQuest
		};

		map< int, structQuestTrackIcon > mapNPC;

		RoleDataEx* pRole = CNetGlobal::RoleData();


		//vector<GameObjDbStructEx*>&	rQuestDetailTable = g_ObjectData->_QuestDetailTable;

		GameObjDbStructEx* pQuestDetail;
		GameObjDbStructEx* pObjNPC;

		int NpcID;
		int QuestID;
		int GotQuestIndex;
		bool bLuaCheck;

		int QuestDetail_Begin = Def_ObjectClass_QuestDetail;
		int QuestDetail_End = Def_ObjectClass_QuestDetail + Def_ObjectClass_QuestDetail_Total;

		//int count = rQuestDetailTable.size();
		for( int i = QuestDetail_Begin ; i < QuestDetail_End ; ++i )
		{
			pQuestDetail = g_ObjectData->GetObj( i );

			if( !pQuestDetail )
				continue;

			QuestID = pQuestDetail->GUID;

			//有沒有任務
			GotQuestIndex = pRole->CheckQuestList_IsGotQuest( QuestID );
			if( GotQuestIndex != -1 )
			{
					if( pQuestDetail->QuestDetail.iRequest_QuestID != 0 )
					{
						NpcID = pQuestDetail->QuestDetail.iRequest_QuestID;
					}
					else
					{
						NpcID = pQuestDetail->QuestDetail.iQuestProvider[0];
					}

					pObjNPC = g_ObjectData->GetObj( NpcID );

					if( CNpcTrackFrame::IsNpcVisible( pObjNPC, true ) )
					{

						if( pRole->CheckQuest_IsComplete( GotQuestIndex, QuestID, &( pQuestDetail->QuestDetail ) ) == EM_QUESTCHECK_RESULT_OK )
						{
							//任務完成
							QuestNpcs[0].push_back( NpcID );

							if( (bool)pQuestDetail->QuestDetail.iCheck_Loop == true )
							{
								if( pQuestDetail->QuestDetail.iQuestGroup == 3 )
									SetMapIncon( mapNPC, NpcID, 0, EM_MapIconType_PublicQuestDone, 0 );
								else
									SetMapIncon( mapNPC, NpcID, 0, EM_MapIconType_RepeatQuestDone, 0 );
							}
							else
							{
								SetMapIncon( mapNPC, NpcID, 0, EM_MapIconType_FinishedQuest, 0 );
							}
 						}
						else
						{
							//任務未完成
							QuestNpcs[3].push_back( NpcID );
							SetMapIncon( mapNPC, NpcID, 4, EM_MapIconType_UnfinishQuest, 3 );
						}
					}
			}
			else
			{
				//沒有該任務
				if( pRole->CheckQuest_IsAvailable( QuestID, &pQuestDetail->QuestDetail, m_SearchQuestNpc_bIgnoreLV ) == EM_QUESTCHECK_RESULT_OK )
				{
	  				bLuaCheck = true;
					// 檢查有無 LUA QUESTBEGIN
					if( strlen( pQuestDetail->QuestDetail.szScript_OnBegin ) != 0 )
					{
						bLuaCheck = g_pQuestBook->CheckQuestBeginLua( pQuestDetail->QuestDetail.szScript_OnBegin );
					}

					if( bLuaCheck )
					{

						int iGroupID = -1;

						NpcID		= pQuestDetail->QuestDetail.iQuestProvider[0];
						pObjNPC		= g_ObjectData->GetObj( NpcID );
						
						if( CNpcTrackFrame::IsNpcVisible( pObjNPC, true ) )
						{
							if( pQuestDetail->QuestDetail.iQuestGroup == 3 ) // 公眾
							{
								iGroupID = 4;
								SetMapIncon( mapNPC, NpcID, 2, EM_MapIconType_PublicQuest, 2 );
							}
							else
							if( pQuestDetail->QuestDetail.iControlType != 0 || 
								pQuestDetail->QuestDetail.iQuestGroup != 0 ||
								pQuestDetail->QuestDetail.iCheck_Loop != 0 )
							{
								//委託任務
								//QuestNpcs[2].push_back( NpcID );
								iGroupID = 2;
								SetMapIncon( mapNPC, NpcID, 3, EM_MapIconType_TrustQuest, 2);
							}
							else										
							if( pRole->CheckQuestHistory_IsComplete( QuestID ) == true )	// 檢查角色是否已完成該任務
							{
								continue;
							}
							else
							{
								// 一般任務可接
								iGroupID = 1;
								SetMapIncon( mapNPC, NpcID, 1, EM_MapIconType_NewQuest, 1 );
							}

							//新任務
							QuestNpcs[ iGroupID ].push_back( NpcID );
							
						}	
					}
				}
			}
		}

		const WorldMapInfoStruct* pWorldMapInfo;
		vector<int>::iterator itPos;
		vector<int>::iterator itEnd;

		/*
		for( int i = 0 ; i < 4 ; ++i )
		{
			if( !m_SearchQuestNpc_ShowFlag[i] )
				continue;

			itPos = QuestNpcs[i].begin();
			itEnd = QuestNpcs[i].end();

			IconType = IconTypes[i];

			for( ; itPos != itEnd ; ++itPos )
			{
				NpcID = *itPos;
				if( DisplayedNpcs.find( NpcID )	!= DisplayedNpcs.end() )
					continue;

				pObjNPC = g_ObjectData->GetObj( NpcID );
				pWorldMapInfo = g_ObjectData->GetWorldMapInfoByID( pObjNPC->NPC.ZoneID );

				g_pWorldMapFrame->AddTrackQuestNPC( 
					pWorldMapInfo->wdbFile, 
					(float)pObjNPC->NPC.X, 
					(float)pObjNPC->NPC.Z, 
					pObjNPC->GetName(),
					pObjNPC->GUID,
					IconType );

				DisplayedNpcs.insert( NpcID );
			}
		}
		*/


		for( map< int, structQuestTrackIcon >::iterator it = mapNPC.begin(); it != mapNPC.end(); it++ )
		{
			structQuestTrackIcon Info = it->second;

			if( !m_SearchQuestNpc_ShowFlag[ Info.iType ] )
				continue;

			//itPos = QuestNpcs[i].begin();
			//itEnd = QuestNpcs[i].end();

			//IconType = IconTypes[i];

			//for( ; itPos != itEnd ; ++itPos )
			{
				//NpcID = *itPos;
				//if( DisplayedNpcs.find( NpcID )	!= DisplayedNpcs.end() )
				//	continue;

				pObjNPC = g_ObjectData->GetObj( Info.iNPCID );

				if( pObjNPC != NULL )
				{
					pWorldMapInfo = g_ObjectData->GetWorldMapInfoByID( pObjNPC->NPC.ZoneID );

					g_pWorldMapFrame->AddTrackQuestNPC( 
						pWorldMapInfo->wdbFile, 
						(float)pObjNPC->NPC.X, 
						(float)pObjNPC->NPC.Z, 
						pObjNPC->GetName(),
						pObjNPC->GUID,
						Info.iIcon );
				}

				//DisplayedNpcs.insert( NpcID );
			}



		}

	}

	//m_WorldMapView.Update( elapsedTime );
	//m_AreaMapView.Update( elapsedTime );
}

// ----------------------------------------------------------------------------
void CWorldMapFrame::ChangeZone()
{
	std::set< int >::iterator itPos = m_ChangeZoneClearGroupIDs.begin();
	std::set< int >::iterator itEnd = m_ChangeZoneClearGroupIDs.end();

	for( ; itPos != itEnd ; ++itPos )
	{
		ClearMapIcon( *itPos );
	}
}

// ----------------------------------------------------------------------------
void CWorldMapFrame::SetPingPosition( const SMapCoordinate& rMapCoord )
{
	//m_PingInfo.strPinger = g_pGameMain->GetPlayerName();
	//m_PingInfo.coord = rZoneCoord;

	//發出訊息給隊友
	PartyStateInfo ePSInfo = NetCli_PartyChild::GetPartyState();
	if( ePSInfo == ePartyState_Leader ||
		ePSInfo == ePartyState_Member )
	{
		PartyBaseInfoStruct& rInfo = NetCli_PartyChild::GetPartyInfo();
		NetCli_Channel::SendToCliMapPing( 
			EM_GroupObjectType_Party, rInfo.PartyID, g_pGameMain->GetPlayerName(),
			rMapCoord.MapID, rMapCoord.PosX , rMapCoord.PosZ );
	}
	else
	{
		R_CtoC_MapPing( g_pGameMain->GetPlayerName() , rMapCoord.MapID, rMapCoord.PosX , rMapCoord.PosZ );
	}

}

const SWorldCoordinate& CWorldMapFrame::GetPingPosition()
{
	return m_WorldCoordPing;
}

void CWorldMapFrame::R_CtoC_MapPing( const char* pPingerName, int MapID, float fX, float fZ )
{
	SMapCoordinate coord;
	coord.MapID = MapID;
	coord.PosX = fX;
	coord.PosZ = fZ;
 
	if( false == Func_TransformCoordinate_MapToWorld( coord, m_WorldCoordPing ) )
		return ;
  
	m_PingMapID = MapID;

	lua_State* L = m_interface->GetUi()->GetLuaState();
   
	//忽略自己的訊息
	if( 0 == strcmp( pPingerName , g_pGameMain->GetPlayerName() ) )
	{
		lua_pushnil(L);
		lua_setglobal( L, "PingerName" );

		lua_pushnil(L);
		lua_setglobal( L, "PingZone" );

		lua_pushnil(L);
		lua_setglobal( L, "PingZoneX" );

		lua_pushnil(L);
		lua_setglobal( L, "PingZoneY" );
	}
	else
	{
		lua_pushstring( L, pPingerName );
		lua_setglobal( L, "PingerName" );

		lua_pushstring( L, Func_GetLocalMapName(MapID) );
		lua_setglobal( L, "PingZone" );

		lua_pushnumber( L, fX );
		lua_setglobal( L, "PingZoneX" );

		lua_pushnumber( L, fZ );
		lua_setglobal( L, "PingZoneY" );
	}

	SendWorldEvent( UiEvent_MapPing );

}

// ----------------------------------------------------------------------------
void CWorldMapFrame::ShowTrackTarget( int Index, const char* wdbFile, float PosX, float PosZ, const char* szName, int IconType, float r, float g, float b )
{
	if( Index < 0 || Index > (int)m_TrackNPC.size() )
	{
		return;
	}

	m_TrackNPC[Index].show           = true         ;
	m_TrackNPC[Index].coord.wdbFile  = wdbFile      ;
	m_TrackNPC[Index].coord.PosX     = PosX         ;
	m_TrackNPC[Index].coord.PosZ     = PosZ         ;
	m_TrackNPC[Index].strName        = szName       ;
	m_TrackNPC[Index].icontype       = IconType     ;
	m_TrackNPC[Index].r              = r            ;
	m_TrackNPC[Index].g              = g            ;
	m_TrackNPC[Index].b              = b            ;

  	m_TrackNPC[Index].size           = 16           ;
  	m_TrackNPC[Index].trackArrow     = true         ;
 		
	m_TrackNPC[Index].id             = -1           ;


	m_WorldMapView.Dirty();
	m_AreaMapView.Dirty();
}

// ----------------------------------------------------------------------------
void CWorldMapFrame::HideTrackTarget( int Index )
{
	if( Index < 0 || Index > (int)m_TrackNPC.size() )
	{
		return;
	}
	m_TrackNPC[Index].show = false;

	m_WorldMapView.Dirty();
	m_AreaMapView.Dirty();
}

// ----------------------------------------------------------------------------
void CWorldMapFrame::AddTrackQuestNPC( const char* wdbFile, float PosX, float PosZ, const char* szName, int DBID, int IconType, float r, float g, float b )
{
	SMapTrackInfo Info;

	Info.coord.wdbFile = wdbFile;
	Info.coord.PosX = PosX;
	Info.coord.PosZ = PosZ;
	Info.r = r;
	Info.g = g; 
	Info.b = b;
	Info.icontype = IconType;
	Info.size = 16;
	Info.trackArrow = true;
	if(szName)
	{
		Info.strName = szName;
	}
	else
	{
		Info.strName = "";
	}
	Info.id = DBID;
	Info.show = true;
	m_QuestNpcInfo.push_back( Info );

	m_WorldMapView.Dirty();
	m_AreaMapView.Dirty();
}

// ----------------------------------------------------------------------------
void CWorldMapFrame::ClearTrackQuestNPC()
{
	m_QuestNpcInfo.clear();

	m_WorldMapView.Dirty();
	m_AreaMapView.Dirty();
}

// ----------------------------------------------------------------------------
void CWorldMapFrame::AddMapIcon( int groupId, const char* wdbFile, float PosX, float PosZ, const char* szName, int IconType, float r , float g , float b , bool trackArrow, int id )
{
	bool bFind = false;
	if( id != -1 )
	{
		std::vector< SMapTrackInfo >& table = m_MapIcon[groupId];
		std::vector< SMapTrackInfo >::iterator itPos = table.begin();
		std::vector< SMapTrackInfo >::iterator itEnd = table.end();

		for( ; itPos != itEnd ; ++itPos )
		{
			SMapTrackInfo& Info = (*itPos);
			if( Info.id == id )
			{
				Info.coord.wdbFile = wdbFile;
				Info.coord.PosX = PosX;
				Info.coord.PosZ = PosZ;
				Info.r = r;
				Info.g = g; 
				Info.b = b;
				Info.icontype = IconType;
				Info.size = 16;
				Info.trackArrow = trackArrow;
				if(szName)
				{
					Info.strName = szName;
				}
				else
				{
					Info.strName = "";
				}
				Info.id = -1;
				Info.show = true;

				bFind = true;

				break;
			}
		}
	}

	if( false == bFind )
	{

		SMapTrackInfo Info;

		Info.coord.wdbFile = wdbFile;
		Info.coord.PosX = PosX;
		Info.coord.PosZ = PosZ;
		Info.r = r;
		Info.g = g; 
		Info.b = b;
		Info.icontype = IconType;
		Info.size = 16;
		Info.trackArrow = trackArrow;
		if(szName)
		{
			Info.strName = szName;
		}
		else
		{
			Info.strName = "";
		}
		Info.id = id;
		Info.show = true;

		std::vector< SMapTrackInfo >& table = m_MapIcon[groupId];

		table.push_back( Info );
	}

	m_WorldMapView.Dirty();
	m_AreaMapView.Dirty();

}

// ----------------------------------------------------------------------------
void CWorldMapFrame::AddMapIcon( int groupId, int zoneID, float PosX, float PosZ, const char* szName, int IconType, float r, float g, float b, bool trackArrow, int id )
{
	int mapID = zoneID % _DEF_ZONE_BASE_COUNT_ ;
	const WorldMapInfoStruct* pWorldMapInfo = g_ObjectData->GetWorldMapInfoByID( mapID );

	if( NULL == pWorldMapInfo )
	{
		return ;
	}

	AddMapIcon( groupId, pWorldMapInfo->wdbFile, PosX, PosZ, szName, IconType, r, g, b, trackArrow, id );
}

// ----------------------------------------------------------------------------
void CWorldMapFrame::ClearMapIcon( int groupId, int id )
{
	if( id != -1 )
	{
		std::vector< SMapTrackInfo >& table = m_MapIcon[groupId];
		std::vector< SMapTrackInfo >::iterator itPos = table.begin();
		std::vector< SMapTrackInfo >::iterator itEnd = table.end();

		for( ; itPos != itEnd ; ++itPos )
		{
			SMapTrackInfo& Info = (*itPos);
			if( Info.id == id )
			{
				table.erase( itPos );
				return;
			}
		}

		return;
	}
	m_MapIcon.erase( groupId );
}

// ----------------------------------------------------------------------------
void CWorldMapFrame::ClearMapIconWhenChangeZone( int groupId )
{
	m_ChangeZoneClearGroupIDs.insert( groupId );
}

// ----------------------------------------------------------------------------
void CWorldMapFrame::AddIcon( const char* wdbFile , float PosX , float PosZ , float r, float g, float b, int IconType, float size, char* szName, int id )
{
	SMapTrackInfo Info;

	Info.coord.wdbFile = wdbFile;
	Info.coord.PosX = PosX;
	Info.coord.PosZ = PosZ;
	Info.r = r;
	Info.g = g; 
	Info.b = b;
	Info.icontype = IconType;
	Info.size = size;
	if(szName)
	{
		Info.strName = szName;
	}
	else
	{
		Info.strName = "";
	}
	Info.id = id;
	Info.show = true;
	m_InsideTrackInfo.push_back( Info );

	m_WorldMapView.Dirty();
	m_AreaMapView.Dirty();

	m_bGmMode = true;
}

// ----------------------------------------------------------------------------
void CWorldMapFrame::ClearIcon()
{
	m_InsideTrackInfo.clear();

	m_WorldMapView.Dirty();
	m_AreaMapView.Dirty();
}


#define DF_MapMarkGroupID 20091229

//設定Map 的 Icon & Tips
void CWorldMapFrame::RL_SetMapMark( int MarkID, float PosX, float PosZ, const char* szName, int IconType )
{
	ClearMapIconWhenChangeZone( DF_MapMarkGroupID );
	//int WorldMapID = g_pGameMain->GetWorldMapID();
	int WorldMapID = g_pGameMain->GetZoneID();
	AddMapIcon( DF_MapMarkGroupID, WorldMapID, PosX, PosZ, szName, IconType, 1.0f, 1.0f, 1.0f , false, MarkID );	
}

void CWorldMapFrame::RL_ClsMapMark( int MarkID )
{
	ClearMapIcon( DF_MapMarkGroupID, MarkID );
}

// ----------------------------------------------------------------------------
//
//
// ----------------------------------------------------------------------------
CWorldMapView::CWorldMapView()
{
	m_pUiFrameWorldMap = NULL;
	m_pUiTextureSelf = NULL;

	m_WorldMapID = 0;

	m_StaticIconFrameCount = 0;
	m_bStaticIconFrameListDirty = false;

	m_bShowQuestNpc = true;
	m_bShowParty = true;
	m_bShowTrackNpc = true;

	eType = WMV_WorldMap;
}

void CWorldMapView::Init( const char* pViewName , const char* pPlayerCursor )
{
	m_FrameInfoMap.clear();
	m_IconFrameList.clear();
	m_StaticIconFrameCount = 0;
	m_bStaticIconFrameListDirty = true;

	m_pUiFrameWorldMap = dynamic_cast<CUiFrame*>( g_pGameMain->GetInterface()->GetUiWorld()->FindObject( pViewName ) );	
	m_pUiTextureSelf   = dynamic_cast<CUiTexture*>( g_pGameMain->GetInterface()->GetUiWorld()->FindObject( pPlayerCursor ) );	

	if ( !m_pUiFrameWorldMap || !m_pUiTextureSelf )
		return ;

	if( eType == WMV_WorldMap )
	{
		m_pUiFrameWorldMap->m_pfOnPaint = UiCallback_WorldMap_OnPaint;
	}
	else if( eType == WMV_AreaMap )
	{
		m_pUiFrameWorldMap->m_pfOnPaint = UiCallback_AreaMap_OnPaint;
	}
}

void CWorldMapView::Fini()
{
	m_pUiFrameWorldMap = NULL;
	m_pUiTextureSelf = NULL;

	m_StaticIconFrameCount = 0;
	m_bStaticIconFrameListDirty = true;

	m_FrameInfoMap.clear();
	m_IconFrameList.clear();
}

void CWorldMapView::SetWorldMapID( int WorldMapID )
{
	m_WorldMapID = WorldMapID;
	Dirty();
}

void CWorldMapView::Dirty()
{
	m_bStaticIconFrameListDirty = true;
	m_FrameInfoMap.clear();
}

void CWorldMapView::SetShowQuestNpc( bool bShow )
{
	m_bShowQuestNpc = bShow;
	Dirty();
}

void CWorldMapView::SetShowParty( bool bShow )
{
	m_bShowParty = bShow;
	Dirty();
}

void CWorldMapView::SetShowTrackNpc( bool bShow )
{
	m_bShowTrackNpc = bShow;
	Dirty();
}

void CWorldMapView::Update()
{
	const WorldMapInfoStruct* pWorldMapInfo = g_ObjectData->GetWorldMapInfoByID( m_WorldMapID );
	CRoleSprite* pPlayer = g_pGameMain->GetPlayer();

	if( !pWorldMapInfo || !pPlayer || !m_pUiFrameWorldMap || !m_pUiTextureSelf )
		return;

	if( !m_pUiFrameWorldMap->IsVisible() )
	{
		return;
	}

	// 計算座標
	CUiSize FrameSize = m_pUiFrameWorldMap->GetSize();
	CRuVector3 pos = pPlayer->GetPos();
	CUiAnchor anchor;
	CUiPoint   Point;

	float left   = (float)pWorldMapInfo->LX; 
	float top    = (float)pWorldMapInfo->LZ; 
	float right  = (float)pWorldMapInfo->RX;
	float bottom = (float)pWorldMapInfo->RZ;

	const WorldMapInfoStruct* pWorldMapInfoPlayer = g_ObjectData->GetWorldMapInfoByID( g_pGameMain->GetWorldMapID() );

	if( !pWorldMapInfoPlayer )
	{
		return;
	}
	
	// 設定角色游標座標
	if( strcmp( pWorldMapInfo->wdbFile, pWorldMapInfoPlayer->wdbFile ) == 0 &&		
		pos.m_x > left && pos.m_x < right  && 
		pos.m_z < top  && pos.m_z > bottom )
	{
		Point.m_x = ( pos.m_x - left ) / ( right - left ) * FrameSize.m_x ;
		Point.m_y = ( pos.m_z - top  ) / ( bottom - top ) * FrameSize.m_y ;

		anchor.m_relativeTo = m_pUiFrameWorldMap;
		anchor.m_offset.m_x = Point.m_x;	
		anchor.m_offset.m_y = Point.m_y;
		anchor.m_point = CUiAnchor::eAnchor_Center;
		anchor.m_relativePoint = CUiAnchor::eAnchor_TopLeft;


		float size = g_fWorldMapSelfIconSize * m_pUiFrameWorldMap->GetHeight() / g_pGameMain->GetUIHeight();
		size = ( size * 2 + g_fWorldMapSelfIconSize ) / 3;
		m_pUiTextureSelf->SetSize( CUiSize(size,size) );

		m_pUiTextureSelf->ClearAllAnchors();
		m_pUiTextureSelf->SetAnchor( anchor );

		// 設定角色游標方向
		CRuVector3 direction = pPlayer->GetDirection();

		//float angle = asin( direction.m_x );
		float angle = asin( direction.m_x / sqrtf ( direction.m_x * direction.m_x + direction.m_z * direction.m_z ) );
		if( direction.z < 0.0f )
		{
			angle = ruPI - angle ;
		}

		m_pUiTextureSelf->SetRotate( angle );

		m_pUiTextureSelf->SetVisible( true );
	}
	else
	{
		m_pUiTextureSelf->SetVisible( false );
	}


	//大小改變
	{
		if( m_pUiFrameWorldMap )
		{
			if( m_oldSize != m_pUiFrameWorldMap->GetSize() )
			{
				m_oldSize = m_pUiFrameWorldMap->GetSize();
				m_bStaticIconFrameListDirty = true;
			}
		}
	}


	if( m_bStaticIconFrameListDirty )
	{
		m_bStaticIconFrameListDirty = false;


		//m_TrackNPC
		m_FrameInfoMap.clear();

		m_StaticIconFrameCount = 0;

		if( m_bShowTrackNpc )
			UpdateStaticIconFrameList( g_pWorldMapFrame->m_TrackNPC );
		
		if( m_bShowQuestNpc )
			UpdateStaticIconFrameList( g_pWorldMapFrame->m_QuestNpcInfo );
		
		UpdateStaticIconFrameList( g_pWorldMapFrame->m_InsideTrackInfo );

		std::map< int , std::vector< SMapTrackInfo > >::iterator itPos = g_pWorldMapFrame->m_MapIcon.begin();
		std::map< int , std::vector< SMapTrackInfo > >::iterator itEnd = g_pWorldMapFrame->m_MapIcon.end();

		for( ; itPos != itEnd ; ++itPos )
		{
			UpdateStaticIconFrameList( itPos->second );
		}


		int i = m_StaticIconFrameCount;
		int count = (int)m_IconFrameList.size();
		for( ;  i < count ; ++i )
		{
			SWorldMapIconFrame& rIconFrame = m_IconFrameList[i];
			rIconFrame.pFrme->SetVisible( false );
		}
	}

	if( m_bShowParty )
	{
		//追蹤隊友
		int _temp = m_StaticIconFrameCount;
		UpdateStaticIconFrameList( g_pWorldMapFrame->m_TrackTeammate );

		int i = m_StaticIconFrameCount;
		int count = (int)m_IconFrameList.size();
		for( ;  i < count ; ++i )
		{
			SWorldMapIconFrame& rIconFrame = m_IconFrameList[i];
			assert( rIconFrame.pFrme );
			if( rIconFrame.pFrme && rIconFrame.pFrme->IsVisible() )
			{
				rIconFrame.pFrme->SetVisible( false );
			}
		}

		m_StaticIconFrameCount = _temp;
	}
}

void CWorldMapView::UpdateStaticIconFrameList( std::vector< SMapTrackInfo >& rTable )
{

	const WorldMapInfoStruct* pWorldMapInfo = g_ObjectData->GetWorldMapInfoByID( m_WorldMapID );
	CRoleSprite* pPlayer = g_pGameMain->GetPlayer();

	if( !pWorldMapInfo || !pPlayer || !m_pUiFrameWorldMap || !m_pUiTextureSelf )
		return;

	CUiSize FrameSize = m_pUiFrameWorldMap->GetSize();
	CRuVector3 pos = pPlayer->GetPos();
	CUiAnchor anchor;
	CUiPoint Point;

	float left   = (float)pWorldMapInfo->LX; 
	float top    = (float)pWorldMapInfo->LZ; 
	float right  = (float)pWorldMapInfo->RX;
	float bottom = (float)pWorldMapInfo->RZ;

	CUiFrame* pUiFrame = NULL;		
	CUiTexture* pUiTexture = NULL;


	int frameCount = m_IconFrameList.size();
	int frameIndex = (int)m_StaticIconFrameCount;

	int infoIndex;
	int infoCount = (int)rTable.size();
	for( infoIndex = 0 ; infoIndex < infoCount ; ++infoIndex )
	{
		SMapTrackInfo& Info = rTable[infoIndex];
		if( Info.show &&
			Info.coord.wdbFile == pWorldMapInfo->wdbFile &&
			Info.coord.PosX > left && Info.coord.PosX < right && 
			Info.coord.PosZ < top  && Info.coord.PosZ > bottom )
		{
			if( frameIndex == frameCount )
			{
				// 底板
				pUiFrame = (CUiFrame*)g_pInterface->GetUiWorld()->CreateObject(CUiLayout::eUiType_Frame, "", m_pUiFrameWorldMap );
				pUiFrame->SetFrameStrata(eUIFrame_Strata_Medium);
				pUiFrame->SetAnchorSize(CUiSize(16.0f, 16.0f));
				pUiFrame->SetAnchor(CUiAnchor());
				pUiFrame->SetMouseEnable( true );
				pUiFrame->m_pfOnMouseEnter  = UiCallback_WorldMapIconFrame_OnMouseEnter;
				pUiFrame->m_pfOnMouseLeave  = UiCallback_WorldMapIconFrame_OnMouseLeave;
				pUiFrame->m_pfOnDoubleClick = UiCallback_WorldMapIconFrame_OnDoubleClick;
				pUiFrame->m_pfOnClick       = UiCallback_WorldMapIconFrame_OnClick;
				pUiFrame->RegisterForClicks( CUiFrame::eMouse_Flag_LButton | CUiFrame::eMouse_Flag_RButton );

				// 背景圖形
				pUiTexture = (CUiTexture*)g_pInterface->GetUiWorld()->CreateObject(CUiLayout::eUiType_Texture, "", pUiFrame);
				pUiTexture->SetAnchorSize(CUiSize(16.0f, 16.0f));
				pUiTexture->SetAnchor(CUiAnchor());
				pUiTexture->SetFile( g_szIconTextureName );
				pUiFrame->SetLayers(CUiFrame::eLayerType_Background, pUiTexture);


				m_IconFrameList.push_back( SWorldMapIconFrame( pUiFrame , pUiTexture ) );
				frameCount = (int)m_IconFrameList.size();
			}

			SWorldMapIconFrame& rIconFrame = m_IconFrameList[frameIndex];
			++frameIndex;

			pUiFrame = rIconFrame.pFrme;		
			pUiTexture = rIconFrame.pTexture;

			Point.m_x = ( Info.coord.PosX - left ) / ( right - left ) * FrameSize.m_x ;
			Point.m_y = ( Info.coord.PosZ - top  ) / ( bottom - top ) * FrameSize.m_y ;	

			anchor.m_relativeTo = m_pUiFrameWorldMap;
			anchor.m_offset.m_x = Point.m_x;	
			anchor.m_offset.m_y = Point.m_y;
			anchor.m_point = CUiAnchor::eAnchor_Center;
			anchor.m_relativePoint = CUiAnchor::eAnchor_TopLeft;

			//float size = Info.size * m_pUiFrameWorldMap->GetHeight() / g_pGameMain->GetUIHeight();
			float size = g_fWorldMapIconSize * m_pUiFrameWorldMap->GetHeight() / g_pGameMain->GetUIHeight();
			size = ( size * 2 + g_fWorldMapIconSize ) / 3;
			//pUiTexture->SetFile( Func_GetMapIconFrameName( (EMapIconType)Info.icontype ) );
			CRuRectangle rect;
			Func_GetMapIconTextureInfo( (EMapIconType)Info.icontype, rect );
			pUiTexture->SetTexCoords( 1 , &rect );

			pUiTexture->SetColor( Info.r , Info.g , Info.b );
			pUiTexture->SetSize( CUiSize(size,size) );

			pUiFrame->SetSize( CUiSize(size,size) );
			pUiFrame->ClearAllAnchors();
			pUiFrame->SetAnchor( anchor );				
			pUiFrame->SetVisible( true );

			m_FrameInfoMap[pUiFrame] = &Info;
		}
	}

	m_StaticIconFrameCount = frameIndex;

}

bool CWorldMapView::IconFrame_OnMouseEnter( CUiFrame* frame )
{
	if( !m_pUiFrameWorldMap )
	{
		return false;
	}

	std::map< CUiFrame* , SMapTrackInfo* >::iterator it = m_FrameInfoMap.find( frame );
	if( it == m_FrameInfoMap.end() )
	{
		return false;
	}

	SMapTrackInfo* pInfo = it->second;


	CUiTooltip* pTooltip = (CUiTooltip*)g_pGameMain->GetInterface()->GetUi()->FindObject( "GameTooltip" );

	if( !pTooltip )
		return false;

	pTooltip->ClearLines();

	SMapCoordinate MapCoord;
	MapCoord.MapID = m_WorldMapID;

	if( false == Func_TransformCoordinate_WorldToMap( pInfo->coord, MapCoord ) )
		return false;

	char tt[256];

	if( pInfo->strName.size() > 0 )
	{
		sprintf( tt, "%s ( %.1f , %.1f )", pInfo->strName.c_str(), MapCoord.PosX * 100 , MapCoord.PosZ * 100 );
	}
	else
	{
		sprintf( tt, "( %.1f , %.1f )", MapCoord.PosX * 100 , MapCoord.PosZ * 100 );
	}

	pTooltip->AddLine( tt, 1.0f, 1.0f, 1.0f );

	CUiAnchor anchor;
	anchor.m_relativeTo = m_pUiFrameWorldMap;
	anchor.m_offset.m_x = 0.0f;	
	anchor.m_offset.m_y = 0.0f;
	anchor.m_point = CUiAnchor::eAnchor_BottomRight;
	anchor.m_relativePoint = CUiAnchor::eAnchor_BottomRight;
	pTooltip->ClearAllAnchors();
	pTooltip->SetAnchor( anchor );

	pTooltip->SetVisible( true );

	return true;
}

bool CWorldMapView::IconFrame_OnMouseLeave( CUiFrame* frame )
{
	std::map< CUiFrame* , SMapTrackInfo* >::iterator it = m_FrameInfoMap.find( frame );
	if( it == m_FrameInfoMap.end() )
	{
		return false;
	}
	SMapTrackInfo* pInfo = it->second;

	CUiTooltip* pTooltip = (CUiTooltip*)g_pGameMain->GetInterface()->GetUi()->FindObject( "GameTooltip" );
	if( !pTooltip )
	{
		return false;
	}
	pTooltip->SetVisible( false );

	return true;
}

bool CWorldMapView::IconFrame_OnDoubleClick( CUiFrame* frame )
{
	char szCommand[256];

	std::map< CUiFrame* , SMapTrackInfo* >::iterator it = m_FrameInfoMap.find( frame );
	if( it == m_FrameInfoMap.end() )
	{
		return false;
	}
	SMapTrackInfo* pInfo = it->second;

	if( g_pWorldMapFrame->m_bGmMode && pInfo->id != -1 )
	{
		sprintf( szCommand , "gotoid %d" , pInfo->id );
		NetCli_Talk::GMCommand( szCommand );
	}
	return true;
}

bool CWorldMapView::IconFrame_OnClick( CUiFrame* frame )
{
	std::map< CUiFrame* , SMapTrackInfo* >::iterator it = m_FrameInfoMap.find( frame );
	if( it == m_FrameInfoMap.end() )
	{
		return false;
	}

	SMapTrackInfo* pInfo = it->second;

	if( pInfo->id == -1 )
	{
		return false;
	}

	lua_State* L = g_pWorldMapFrame->GetLuaStateWorld();

	lua_pushnumber( L, pInfo->id );
	lua_setglobal( L, "NpcID" );

	lua_pushstring( L, pInfo->strName.c_str() );
	lua_setglobal( L, "NpcName" );

	g_pWorldMapFrame->SendWorldEvent( UiEvent_WorldMapIconClick );

	return true;
}

void UiCallback_WorldMap_OnPaint(CUiFrame* frame, INT64 lParam, INT64 rParam)
{
	g_pWorldMapFrame->m_WorldMapView.Update();
}

void UiCallback_AreaMap_OnPaint(CUiFrame* frame, INT64 lParam, INT64 rParam)
{
	g_pWorldMapFrame->m_AreaMapView.Update();
}

// ----------------------------------------------------------------------------
void UiCallback_WorldMapIconFrame_OnMouseEnter(CUiFrame* frame, INT64 lParam, INT64 rParam)
{
	if( g_pWorldMapFrame->m_WorldMapView.IconFrame_OnMouseEnter( frame ) )
	{
		return;
	}
	
	if( g_pWorldMapFrame->m_AreaMapView.IconFrame_OnMouseEnter( frame ) )
	{
		return;
	}
}

// ----------------------------------------------------------------------------
void UiCallback_WorldMapIconFrame_OnMouseLeave(CUiFrame* frame, INT64 lParam, INT64 rParam)
{

	if( g_pWorldMapFrame->m_WorldMapView.IconFrame_OnMouseLeave( frame ) )
	{
		return;
	}

	if( g_pWorldMapFrame->m_AreaMapView.IconFrame_OnMouseLeave( frame ) )
	{
		return;
	}
}

// ----------------------------------------------------------------------------
void UiCallback_WorldMapIconFrame_OnDoubleClick(CUiFrame* frame, INT64 lParam, INT64 rParam)
{
	if( g_pWorldMapFrame->m_WorldMapView.IconFrame_OnDoubleClick( frame ) )
	{
		return;
	}

	if( g_pWorldMapFrame->m_AreaMapView.IconFrame_OnDoubleClick( frame ) )
	{
		return;
	}
}

void UiCallback_WorldMapIconFrame_OnClick(CUiFrame* frame, INT64 lParam, INT64 rParam)
{
	if( g_pWorldMapFrame->m_WorldMapView.IconFrame_OnClick( frame ) )
	{
		return;
	}

	if( g_pWorldMapFrame->m_AreaMapView.IconFrame_OnClick( frame ) )
	{
		return;
	}
}

// ----------------------------------------------------------------------------
int LuaFunc_InitializeWorldMap( lua_State* L )
{
	//int n = lua_gettop(L);

	const char* pViewName     = luaL_checkstring( L, 1 );
	const char* pPlayerCursor = luaL_checkstring( L, 2 );

	g_pWorldMapFrame->InitializeWorldMap( pViewName , pPlayerCursor );

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_InitializeAreaMap( lua_State* L )
{
	const char* pViewName     = luaL_checkstring( L, 1 );
	const char* pPlayerCursor = luaL_checkstring( L, 2 );

	g_pWorldMapFrame->InitializeAreaMap( pViewName , pPlayerCursor );

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetPlayerWorldMapPos( lua_State* L )
{
	int id = 0;
	
	if( lua_isnil( L, 1 ) || lua_isnone( L, 1 ) )
	{
		id = g_pGameMain->GetWorldMapID();
	}
	else
	{
		id = (int)luaL_checknumber( L, 1 );	
	}

	const WorldMapInfoStruct* pWorldMapInfo = g_ObjectData->GetWorldMapInfoByID( id );
	CRoleSprite* pPlayer = g_pGameMain->GetPlayer();

	if( !pWorldMapInfo || !pPlayer )
		return 0 ;

	CRuVector3 pos = pPlayer->GetPos();
	float left   = (float)pWorldMapInfo->LX; 
	float top    = (float)pWorldMapInfo->LZ; 
	float right  = (float)pWorldMapInfo->RX;
	float bottom = (float)pWorldMapInfo->RZ;

	// 角色游標

	//超出地圖範圍時 顯示負值 或 大於1
	//if( pos.m_x > left && pos.m_x < right  && 
	//	pos.m_z < top  && pos.m_z > bottom )
	//{
	//	pos.m_x = ( pos.m_x - left ) / ( right - left );
	//	pos.m_z = ( pos.m_z - top  ) / ( bottom - top );
	//	lua_pushnumber( L, pos.m_x	);
	//	lua_pushnumber( L, pos.m_z	);
	//	return 2;
	//}
	//return 0;

	pos.m_x = ( pos.m_x - left ) / ( right - left );
	pos.m_z = ( pos.m_z - top  ) / ( bottom - top );
	lua_pushnumber( L, pos.m_x	);
	lua_pushnumber( L, pos.m_z	);
	return 2;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetCurrentWorldMapID( lua_State* L )
{
	int id = g_pGameMain->GetWorldMapID();
	lua_pushnumber( L, id );
	return 1;
}

// ----------------------------------------------------------------------------
int luaFunc_SetWorldMapID( lua_State* L )
{
	int WorldMapID = (int)luaL_checknumber( L, 1 );
	g_pWorldMapFrame->m_WorldMapView.SetWorldMapID( WorldMapID );
	return 0;
}

// ----------------------------------------------------------------------------
int luaFunc_SetAreaMapID( lua_State* L )
{
	int WorldMapID = (int)luaL_checknumber( L, 1 );
	g_pWorldMapFrame->m_AreaMapView.SetWorldMapID( WorldMapID );
	return 0;
}

// ----------------------------------------------------------------------------
int luaFunc_SetWorldMapPingPosition ( lua_State* L )
{
	SMapCoordinate MapCoord;

	MapCoord.MapID = (int)luaL_checknumber( L, 1 );
	MapCoord.PosX  = (float)luaL_checknumber( L, 2 );
	MapCoord.PosZ  = (float)luaL_checknumber( L, 3 );

	g_pWorldMapFrame->SetPingPosition( MapCoord );

	return 0;
}

// ----------------------------------------------------------------------------
int luaFunc_GetWorldMapPingPosition ( lua_State* L )
{
	const SWorldCoordinate& rWorldCoord = g_pWorldMapFrame->GetPingPosition();
	
	//轉到目前的地圖座標

	SMapCoordinate MapCoord;
	MapCoord.MapID = (int)luaL_checknumber( L, 1 );
	if( Func_TransformCoordinate_WorldToMap( rWorldCoord, MapCoord ) )
	{
		lua_pushnumber( L, MapCoord.PosX );
		lua_pushnumber( L, MapCoord.PosZ );
		return 2;
	}

	return 0;
}

// ----------------------------------------------------------------------------
int luaFunc_GetWorldMapPingMapID( lua_State* L )
{
	lua_pushnumber( L, g_pWorldMapFrame->GetPingMapID() );
	return 1;
}


// ----------------------------------------------------------------------------
int luaFunc_WorldMap_GetWorldCount( lua_State* L )
{
	int count  = g_ObjectData->GetWorldMapInfoTableCount();
	lua_pushnumber( L, count );
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_WorldMap_GetWorld( lua_State* L )
{
	int index = (int)luaL_checknumber( L, 1 ) - 1;
	SWorldMapInfoTable& rWorldMapInfoTable = g_ObjectData->GetWorldMapInfoTableByIndex( index );

	if( rWorldMapInfoTable.WorldZone )
	{
		lua_pushstring( L, Func_GetLocalMapName( rWorldMapInfoTable.WorldZone->MapName ) );
		lua_pushnumber( L, rWorldMapInfoTable.WorldZone->MapID );
		return 2;
	}
 	return 0;
}


static map<string,int> s_MapType;


// ----------------------------------------------------------------------------
int luaFunc_WorldMap_GetMapCount( lua_State* L )
{
	if( s_MapType.empty() )
	{
		s_MapType[ "world"            ] = EM_WorldMapIType_World            ;
		s_MapType[ "zone"             ] = EM_WorldMapIType_Zone             ;
		s_MapType[ "city"             ] = EM_WorldMapIType_City             ;
		s_MapType[ "instance"         ] = EM_WorldMapIType_Instance         ;
		s_MapType[ "worldinstance"    ] = EM_WorldMapIType_WorldInstance    ;
  		s_MapType[ "businessinstance" ] = EM_WorldMapIType_BusinessInstance ;
		s_MapType[ "battleground"     ] = EM_WorldMapIType_Battleground     ;
		s_MapType[ "other"            ] = EM_WorldMapIType_Other            ;
	}
	
	int index = (int)luaL_checknumber( L, 1 ) - 1;
	string strType = luaL_checkstring( L, 2 );

	map<string,int>::iterator itPos = s_MapType.find( strType );
	if( itPos == s_MapType.end() )
	{
		return 0;
	}

	int type = itPos->second;

 	SWorldMapInfoTable& rWorldMapInfoTable = g_ObjectData->GetWorldMapInfoTableByIndex( index );
	vector<const WorldMapInfoStruct*>& mapTable = rWorldMapInfoTable.MapTables[type];

	lua_pushnumber( L, (lua_Number)mapTable.size() );
	return 1;
}

// ----------------------------------------------------------------------------
int luaFunc_WorldMap_GetMap( lua_State* L )
{
	int worldIndex = (int)luaL_checknumber( L, 1 ) - 1;
	string strType = luaL_checkstring( L, 2 );
	int mapIndex = (int)luaL_checknumber( L, 3 ) - 1;


	map<string,int>::iterator itPos = s_MapType.find( strType );
	if( itPos == s_MapType.end() )
	{
		return 0;
	}

	int type = itPos->second;

	SWorldMapInfoTable& rWorldMapInfoTable = g_ObjectData->GetWorldMapInfoTableByIndex( worldIndex );
	vector<const WorldMapInfoStruct*>& mapTable = rWorldMapInfoTable.MapTables[type];

	if( mapIndex >= 0 && mapIndex < (int)mapTable.size() )
	{
		const WorldMapInfoStruct* pObj = mapTable[ mapIndex ];
		lua_pushstring( L, Func_GetLocalMapName( pObj->MapName ) );
		lua_pushnumber( L, pObj->MapID );
		return 2;
	}
	return 0;
}

// ----------------------------------------------------------------------------
int luaFunc_WorldMap_SearchQuestNpc( lua_State* L )
{
	g_pWorldMapFrame->m_SearchQuestNpc_bIgnoreLV   = lua_toboolean(L, 1) == 0 ? false : true;
	g_pWorldMapFrame->m_SearchQuestNpc_ShowFlag[0] = lua_toboolean(L, 2) == 0 ? false : true;
	g_pWorldMapFrame->m_SearchQuestNpc_ShowFlag[1] = lua_toboolean(L, 3) == 0 ? false : true;
	g_pWorldMapFrame->m_SearchQuestNpc_ShowFlag[2] = lua_toboolean(L, 4) == 0 ? false : true;
	g_pWorldMapFrame->m_SearchQuestNpc_ShowFlag[3] = lua_toboolean(L, 5) == 0 ? false : true;

	if( g_pWorldMapFrame->m_SearchQuestNpc_DirtyTime <= 0 )
	{
		g_pWorldMapFrame->m_SearchQuestNpc_DirtyTime = (int)timeGetTime();
	}

	return 0;
}

// ----------------------------------------------------------------------------
int luaFunc_WorldMap_ClearQuestNpc( lua_State* L )
{
	g_pWorldMapFrame->ClearTrackQuestNPC( );
	return 0;
}

int LuaFunc_WorldMap_SetShowQuestNpc ( lua_State* L )
{
	const char* sztype = luaL_checkstring( L , 1 );
	bool bShow = lua_toboolean( L, 2 ) == 0 ? false : true;
	if( 0 == stricmp( sztype , "worldmap" ) )
	{
		g_pWorldMapFrame->m_WorldMapView.SetShowQuestNpc( bShow );
	}
	else
	{
		g_pWorldMapFrame->m_AreaMapView.SetShowQuestNpc( bShow );
	}
	return 0;
}

int LuaFunc_WorldMap_SetShowParty    ( lua_State* L )
{
	const char* sztype = luaL_checkstring( L , 1 );
	bool bShow = lua_toboolean( L, 2 ) == 0 ? false : true;
	if( 0 == stricmp( sztype , "worldmap" ) )
	{
		g_pWorldMapFrame->m_WorldMapView.SetShowParty( bShow );
	}
	else
	{
		g_pWorldMapFrame->m_AreaMapView.SetShowParty( bShow );
	}
	return 0;
}

int LuaFunc_WorldMap_SetShowTrackNpc ( lua_State* L )
{
	const char* sztype = luaL_checkstring( L , 1 );
	bool bShow = lua_toboolean( L, 2 ) == 0 ? false : true;
	if( 0 == stricmp( sztype , "worldmap" ) )
	{
		g_pWorldMapFrame->m_WorldMapView.SetShowTrackNpc( bShow );
	}
	else
	{
		g_pWorldMapFrame->m_AreaMapView.SetShowTrackNpc( bShow );
	}

	return 0;
}

// ----------------------------------------------------------------------------
int luaFunc_GetZoneEnglishName( lua_State* L )
{
 	int mapID = (int)luaL_checknumber( L, 1 );

	const WorldMapInfoStruct* pMapInfo = g_ObjectData->GetWorldMapInfoByID( mapID );

	if( !pMapInfo )
	{
		return 0;
	}

	lua_pushstring( L, pMapInfo->MapName );

	return 1;
}



int luaFunc_GetZoneLocalName( lua_State* L )
{
	int mapID = (int)luaL_checknumber( L, 1 );

	const WorldMapInfoStruct* pMapInfo = g_ObjectData->GetWorldMapInfoByID( mapID );

	if( !pMapInfo )
	{
		return 0;
	}

	lua_pushstring( L, Func_GetLocalMapName( pMapInfo->MapName ) );

	return 1;
}


int luaFunc_WorldMap_AddIcon( lua_State* L )
{
	const char* str = luaL_checkstring( L , 1 );
	float PosX  = (float)luaL_checknumber( L, 2 );
	float PosZ  = (float)luaL_checknumber( L, 3 );

	float r = (float)luaL_checknumber( L, 4 );
	float g = (float)luaL_checknumber( L, 5 );
	float b = (float)luaL_checknumber( L, 6 );
	int IconType = (int)luaL_checknumber( L, 7 );
 
	g_pWorldMapFrame->AddIcon( str , PosX , PosZ , r , g , b , IconType );
	return 0;
}

int luaFunc_WorldMap_ClearIcon( lua_State* L )
{
	g_pWorldMapFrame->ClearIcon();
	return 0;
}

int luaFunc_GetIconTextureRect( lua_State* L )
{
	int _type = 0;
	if( !lua_isnil( L, 1 ) )
	{
		_type = luaL_checkint( L, 1 );
	}
 
	CRuRectangle rect;
	if( Func_GetMapIconTextureInfo( (EMapIconType)_type, rect ) )
	{
		lua_pushstring( L, g_szIconTextureName );
		lua_pushnumber( L, rect.m_left   );
		lua_pushnumber( L, rect.m_top    );
		lua_pushnumber( L, rect.m_right  );
		lua_pushnumber( L, rect.m_bottom );
	}
	else
	{
		lua_pushstring( L, "" );
		lua_pushnumber( L, 0  );
		lua_pushnumber( L, 0  );
		lua_pushnumber( L, 0  );
		lua_pushnumber( L, 0  );
	}
 
	return 5;
}



int luaFunc_WorldMap_AutoMove( lua_State* L )
{
	SMapCoordinate MapCoord;

	MapCoord.MapID = (int)luaL_checknumber( L, 1 );
	MapCoord.PosX  = (float)luaL_checknumber( L, 2 );
	MapCoord.PosZ  = (float)luaL_checknumber( L, 3 );

	//g_pWorldMapFrame->SetPingPosition( MapCoord );

	SWorldCoordinate WorldCoord;
	if( false == Func_TransformCoordinate_MapToWorld( MapCoord, WorldCoord ) )
	{
		return 0;
	}

	CRoleSprite* pSprite = g_pGameMain->GetPlayer();

	if( NULL == pSprite )
	{
		return 0;
	}

	float y = pSprite->GetPos().y;


	CRuVector3 destination( WorldCoord.PosX , y , WorldCoord.PosZ ); 

	if( false == g_pRuneDev->FindHeight( destination ) )
	{
		g_pGameMain->ClearPathMove();
  		g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString( "SYS_WORLDMAP_AUTOMOVE_FAIL2" ) );
		return 0;
	}

	if( false == g_pGameMain->PathMoveTo( destination) )
	{
		//g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString( "SYS_WORLDMAP_AUTOMOVE_FAIL" ) );		
	}
	
	return 0;
}


int luaFunc_WorldMap_AutoMoveByNpcID ( lua_State* L )
{
	int NpcID = (int)luaL_checknumber( L, 1 );

	GameObjDbStructEx* ObjDB = g_ObjectData->GetObj( NpcID );

	if( false == ObjDB->IsNPC() )
	{
		return 0;
	}

	CRuVector3 destination((float)ObjDB->NPC.X , (float)ObjDB->NPC.Y , (float)ObjDB->NPC.Z); 

   	if( false == g_pGameMain->PathMoveTo( destination) )
	{
		//g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString( "SYS_WORLDMAP_AUTOMOVE_FAIL" ) );		
	}
	return 0;
}
//-------------------------------------------------------------------------------------------------
void CWorldMapFrame::SetMapIncon( map< int, structQuestTrackIcon >& mapNPC, int iNPCID, int iProirity, int iIcon, int iType )
{
	if( mapNPC.find( iNPCID ) == mapNPC.end() )
	{
		structQuestTrackIcon Info;

		Info.iIcon			= iIcon;
		Info.iNPCID			= iNPCID;
		Info.iProirity		= iProirity;
		Info.iType			= iType;

		mapNPC[ iNPCID ]		= Info;
	}
	else
	{
		structQuestTrackIcon Info = mapNPC[ iNPCID ];

		if( Info.iProirity > iProirity )
		{
			Info.iIcon		= iIcon;
			Info.iProirity	= iProirity;
			Info.iType		= iType;
		}

		mapNPC[ iNPCID ]		= Info;
	}
}



