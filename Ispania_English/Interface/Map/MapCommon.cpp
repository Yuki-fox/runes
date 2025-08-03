
#include "../mainproc/GameMain.h"
#include "MapCommon.h"
#include "../../netwaker_member/NetWakerClientInc.h"

#include "../../tools/Common/Com_Function.h"


const char* g_szIconTextureName = "Interface\\Minimap\\MinimapIcon";


#define DF_SetIcon( name, id ) \
	MapIconTextureIndex[ EM_MapIconType_##name ] = id;\
	NameMapIconTypeMap[#name] = EM_MapIconType_##name;

class CMapIconTextureInfo
{
public:
	CMapIconTextureInfo()
	{
		MapIconTextureIndex[EM_MapIconType_None             ] = 0 ;  //
		MapIconTextureIndex[EM_MapIconType_Bank             ] = 1 ;
		MapIconTextureIndex[EM_MapIconType_MailBox          ] = 2 ;
		MapIconTextureIndex[EM_MapIconType_FlightPoint      ] = 3 ;
		MapIconTextureIndex[EM_MapIconType_AuctionHouse     ] = 4 ;
		MapIconTextureIndex[EM_MapIconType_Inn              ] = 5 ;
		MapIconTextureIndex[EM_MapIconType_Weapon           ] = 6 ;
		MapIconTextureIndex[EM_MapIconType_Armor            ] = 7 ;
		MapIconTextureIndex[EM_MapIconType_Magic            ] = 8 ;
		MapIconTextureIndex[EM_MapIconType_Mat              ] = 9 ;
		MapIconTextureIndex[EM_MapIconType_Shop             ] = 10;
		MapIconTextureIndex[EM_MapIconType_Potion           ] = 11;
		MapIconTextureIndex[EM_MapIconType_Food             ] = 12;
		MapIconTextureIndex[EM_MapIconType_Recipe           ] = 13;
		MapIconTextureIndex[EM_MapIconType_Mine             ] = 14;
		MapIconTextureIndex[EM_MapIconType_Wood             ] = 15;
		MapIconTextureIndex[EM_MapIconType_Herb             ] = 16;
		MapIconTextureIndex[EM_MapIconType_Fish             ] = 17;
		MapIconTextureIndex[EM_MapIconType_Knight           ] = 18;
		MapIconTextureIndex[EM_MapIconType_Priest           ] = 19;
		MapIconTextureIndex[EM_MapIconType_Ranger           ] = 20;
		MapIconTextureIndex[EM_MapIconType_Rogue            ] = 21;
		MapIconTextureIndex[EM_MapIconType_Warrior          ] = 22;
		MapIconTextureIndex[EM_MapIconType_Wizard           ] = 23;
		MapIconTextureIndex[EM_MapIconType_TrackArrow       ] = 24;
		MapIconTextureIndex[EM_MapIconType_Flag             ] = 25;
		MapIconTextureIndex[EM_MapIconType_NewQuest         ] = 26;
		MapIconTextureIndex[EM_MapIconType_FinishedQuest    ] = 27;
		MapIconTextureIndex[EM_MapIconType_UnfinishQuest    ] = 28;
		MapIconTextureIndex[EM_MapIconType_RepeatQuestDone  ] = 29;
		MapIconTextureIndex[EM_MapIconType_TrustQuest       ] = 30;
		MapIconTextureIndex[EM_MapIconType_RepeatQuest      ] = 30;
		MapIconTextureIndex[EM_MapIconType_GrayQuest        ] = 31;
		MapIconTextureIndex[EM_MapIconType_GuildMan         ] = 33;
		MapIconTextureIndex[EM_MapIconType_LotteryMan       ] = 34;
		MapIconTextureIndex[EM_MapIconType_VocationMan      ] = 35;
		MapIconTextureIndex[EM_MapIconType_HouseMan         ] = 36;
		MapIconTextureIndex[EM_MapIconType_QuestNPC         ] = 37;
		MapIconTextureIndex[EM_MapIconType_QuestOBJ         ] = 38;
		MapIconTextureIndex[EM_MapIconType_TrackNpcArrow    ] = 39;
		MapIconTextureIndex[EM_MapIconType_TrackNpc         ] = 40;
		MapIconTextureIndex[EM_MapIconType_NPC              ] = 40;
		MapIconTextureIndex[EM_MapIconType_Player           ] = 40;		
		MapIconTextureIndex[EM_MapIconType_Speak            ] = 41;
		MapIconTextureIndex[EM_MapIconType_BlueTower        ] = 42;
		MapIconTextureIndex[EM_MapIconType_RedTower         ] = 43;
		MapIconTextureIndex[EM_MapIconType_GrayTower        ] = 44;
		MapIconTextureIndex[EM_MapIconType_Tomb		        ] = 45;		
		MapIconTextureIndex[EM_MapIconType_BGTeam1	        ] = 46;
		MapIconTextureIndex[EM_MapIconType_BGTeam2	        ] = 47;
		MapIconTextureIndex[EM_MapIconType_BGTeam3	        ] = 48;
		MapIconTextureIndex[EM_MapIconType_BGTeam4	        ] = 49;
		MapIconTextureIndex[EM_MapIconType_BGTeam5	        ] = 50;
		MapIconTextureIndex[EM_MapIconType_BGTeam6	        ] = 51;
		MapIconTextureIndex[EM_MapIconType_BGTeam7	        ] = 52;
		MapIconTextureIndex[EM_MapIconType_BGTeam8	        ] = 53;
		MapIconTextureIndex[EM_MapIconType_RuneDancer	    ] = 54;
		MapIconTextureIndex[EM_MapIconType_Druid            ] = 55;
  		MapIconTextureIndex[EM_MapIconType_Fighting         ] = 56;

		MapIconTextureIndex[EM_MapIconType_PublicQuest      ] = 57;
		MapIconTextureIndex[EM_MapIconType_PublicQuestDone  ] = 58;
		MapIconTextureIndex[EM_MapIconType_Harpsyn		    ] = 59;
		MapIconTextureIndex[EM_MapIconType_Psyron			] = 60;		
		MapIconTextureIndex[EM_MapIconType_Duelist     	    ] = 61;

		MapIconTextureIndex[EM_MapIconType_temp4     	    ] = 0;
		MapIconTextureIndex[EM_MapIconType_temp5     	    ] = 0;
		MapIconTextureIndex[EM_MapIconType_temp6     	    ] = 0;
		MapIconTextureIndex[EM_MapIconType_temp7     	    ] = 0;
		MapIconTextureIndex[EM_MapIconType_temp8     	    ] = 0;
		MapIconTextureIndex[EM_MapIconType_temp9     	    ] = 0;
		MapIconTextureIndex[EM_MapIconType_temp10    	    ] = 0;

	}
	
	int MapIconTextureIndex[EM_MapIconType_END];
	std::map<std::string,EMapIconType> NameMapIconTypeMap;


	bool GetMapIconTextureInfo( EMapIconType eID, CRuRectangle& rect )
	{
		if( eID < 0 || eID >= EM_MapIconType_END )
		{
			rect.m_left   = 0.0f;
			rect.m_right  = 0.0f;
			rect.m_top    = 0.0f;
			rect.m_bottom = 0.0f;
			return false;
		}

		int index = MapIconTextureIndex[eID];
		if( index == 0 )
		{
			rect.m_left   = 0.0f;
			rect.m_right  = 0.0f;
			rect.m_top    = 0.0f;
			rect.m_bottom = 0.0f;
			return false;
		}

		index = index - 1;

		rect.m_left = ( (float)index * 17.0f ) / 2048.0f;
		rect.m_right = ( (float)index * 17.0f + 16.0f ) / 2048.0f;
		rect.m_top = 0.0f;
		rect.m_bottom = 1.0f;
		return true;
	}

};


//
bool Func_GetMapIconTextureInfo( EMapIconType eID, CRuRectangle& rect )
{
	static CMapIconTextureInfo g_MapIconTextureInfo;
	return g_MapIconTextureInfo.GetMapIconTextureInfo( eID, rect );
}

const char* Func_GetLocalMapName( const char* pMapName )
{
	const char* name = g_ObjectData->GetString( strupr( Com_FormatString( "ZONE_%s" , pMapName ) ) );
	if ( strlen(name) <= 0 )
		name = pMapName;

	return name;
}

const char* Func_GetLocalMapName( int MapID )
{
	if ( MapID == 400 )
	{
		return Func_GetLocalMapName( "default" );
	}

	const WorldMapInfoStruct* pWorldMapInfo = g_ObjectData->GetWorldMapInfoByID( MapID );

	if( NULL == pWorldMapInfo )
	{
		OutputDebugString( Com_FormatString( "Func_GetLocalMapName error , can't find WorldMapInfoStruct mapid = %d" , MapID ) );
		//return Com_FormatString( "WorldMap%d" , MapID );
		return "";
	}

	return Func_GetLocalMapName( pWorldMapInfo->MapName );
}

bool Func_TransformCoordinate_MapToWorld( const SMapCoordinate& rMapCoord, SWorldCoordinate& rWorldCoord )
{
	const WorldMapInfoStruct* pWorldMapInfo = g_ObjectData->GetWorldMapInfoByID( rMapCoord.MapID );

	if( NULL == pWorldMapInfo )
	{
		OutputDebugString( Com_FormatString( "Func_TransformCoordinate_MapToWorld error , can't find WorldMapInfoStruct mapid = %d" , rMapCoord.MapID ) );
		return false;
	}

	float left   = (float)pWorldMapInfo->LX; 
	float top    = (float)pWorldMapInfo->LZ; 
	float right  = (float)pWorldMapInfo->RX;
	float bottom = (float)pWorldMapInfo->RZ;

	rWorldCoord.wdbFile = pWorldMapInfo->wdbFile;
	rWorldCoord.PosX = ( right - left ) * rMapCoord.PosX + left ;
	rWorldCoord.PosZ = ( bottom - top ) * rMapCoord.PosZ + top  ;

	return true;
}

bool Func_TransformCoordinate_WorldToMap( const SWorldCoordinate& rWorldCoord, SMapCoordinate& rMapCoord )
{
	const WorldMapInfoStruct* pWorldMapInfo = g_ObjectData->GetWorldMapInfoByID( rMapCoord.MapID );
	if( NULL == pWorldMapInfo )
	{
  		OutputDebugString( Com_FormatString( "Func_TransformCoordinate_WorldToZone error , can't find WorldMapInfoStruct mapid = %d" , rMapCoord.MapID ) );
		return false;
	}

	if( rWorldCoord.wdbFile != pWorldMapInfo->wdbFile )
	{
 		return false;
	}

	float left   = (float)pWorldMapInfo->LX; 
	float top    = (float)pWorldMapInfo->LZ; 
	float right  = (float)pWorldMapInfo->RX;
	float bottom = (float)pWorldMapInfo->RZ;

	rMapCoord.PosX = ( rWorldCoord.PosX - left ) / ( right - left );
	rMapCoord.PosZ = ( rWorldCoord.PosZ - top  ) / ( bottom - top );

	//在地圖邊緣的 往內移 避免看不到
	if( rMapCoord.PosX < 0.01f && rMapCoord.PosX > -0.2f )
		rMapCoord.PosX = 0.01f;

	if( rMapCoord.PosX > 0.99f && rMapCoord.PosX < 1.2f )
		rMapCoord.PosX = 0.99f;

	if( rMapCoord.PosZ < 0.01f && rMapCoord.PosZ > -0.2f )
		rMapCoord.PosZ = 0.01f;

	if( rMapCoord.PosZ > 0.99f && rMapCoord.PosZ < 1.2f )
		rMapCoord.PosZ = 0.99f;


	if( rMapCoord.PosX >= 0.0f && rMapCoord.PosX <= 1.0f &&
		rMapCoord.PosZ >= 0.0f && rMapCoord.PosZ <= 1.0f )
	{
		return true;
	}

	return false;
}

//目前會有問題
//
//bool Func_TransformCoordinate_WorldToZone( const SWorldCoordinate& rWorldCoord , SZoneCoordinate& rZoneCoord   )
//{
//	map< int , SWorldZoneStruct >& rTable = g_ObjectData->GetWorldZoneTable();
//	map< int , SWorldZoneStruct >::iterator it = rTable.find( rWorldCoord.strMapName );
//
//	if( it == rTable.end() )
//		return false;
//
//	GameObjDbStructEx* obj;
//	list<GameObjDbStructEx*>& rlist = it->second;
//	list<GameObjDbStructEx*>::iterator itPos = rlist.begin();
//	list<GameObjDbStructEx*>::iterator itEnd = rlist.end();
//
//	for( ; itPos != itEnd ; ++itPos )
//	{
//		obj = (*itPos);
//
//		if( rWorldCoord.PosX >= obj->Zone.WorldMapX1 && 
//			rWorldCoord.PosX <= obj->Zone.WorldMapX2 && 
//			rWorldCoord.PosZ >= obj->Zone.WorldMapY1 && 
//			rWorldCoord.PosZ <= obj->Zone.WorldMapY2 )
//		{
//
//			rZoneCoord.ZoneID = obj->GUID - Def_ObjectClass_Zone;
//			rZoneCoord.PosX = ( rWorldCoord.PosX - obj->Zone.WorldMapX1 ) / ( obj->Zone.WorldMapX2 - obj->Zone.WorldMapX1 ) ;
//			rZoneCoord.PosZ = ( rWorldCoord.PosZ - obj->Zone.WorldMapY1 ) / ( obj->Zone.WorldMapY2 - obj->Zone.WorldMapY1 ) ;
//			
//			return true;
//		}
//	}
//
//	return false;
//}





//因為Zone.CtrlMaxX 不一定有初始化 所以不能使用
//始用Zone.WorldMapX1 可能會有Zone跟Zone有重複的範圍
//所以放棄此做法
/*
int Func_GetZoneIDByPosition( const char* pMapName , float fX , float fZ )
{
	map< string , list<GameObjDbStructEx*> >& rTable = g_ObjectData->GetMapZoneTable();
	map< string , list<GameObjDbStructEx*> >::iterator it = rTable.find( pMapName );

	if( it == rTable.end() )
		return -1;

	GameObjDbStructEx* obj;
	list<GameObjDbStructEx*>& rlist = it->second;
	list<GameObjDbStructEx*>::iterator itPos = rlist.begin();
	list<GameObjDbStructEx*>::iterator itEnd = rlist.end();

	for( ; itPos != itEnd ; ++itPos )
	{
		obj = (*itPos);
		obj->Zone;
		//因為obj->Zone.CtrlMaxX 不一定有初始化 所以不能使用
		//if( fX <= obj->Zone.CtrlMaxX && 
		//	fX >= obj->Zone.CtrlMinX && 
		//	fZ <= obj->Zone.CtrlMaxZ && 
		//	fZ >= obj->Zone.CtrlMinZ )
		if( fX >= obj->Zone.WorldMapX1 && 
			fX <= obj->Zone.WorldMapY1 && 
			fZ >= obj->Zone.WorldMapY1 && 
			fZ <= obj->Zone.WorldMapY2 )
		{
			return obj->GUID;
		}
	}
	
	return -1;
}
*/




//void SetMapTrackInfo( const char* szMapName, REAL PosX, REAL PosZ, const char* szTargetName, int nIconID );
//void ClearMapTrackInfo();
//void SetMapPingInfo( CUiPoint& rPoint );


































