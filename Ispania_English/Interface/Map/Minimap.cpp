#include "../mainproc/GameMain.h"
#include "Minimap.h"
#include "WorldMap.h"
#include "../../netwaker_member/NetWakerClientInc.h"

#include "../../tools/Common/Com_Function.h"


//#define DF_MINIMAP_TEXTURE_TRACKARROW "Interface\\Minimap\\MinimapIcon_TrackArrow"
//const char* DF_MINIMAP_TEXTURE_TRACKARROW = "Interface\\Minimap\\MinimapIcon_TrackArrow" ;


CMinimapFrame* g_pMinimapFrame = NULL;


void UiCallback_Minimap_OnPaint(CUiFrame* frame, INT64 lParam, INT64 rParam);

int LuaFunc_InitializeMiniMap		( lua_State* L );         
int LuaFunc_GetZoneName				( lua_State* L );
int LuaFunc_SetMiniMapVisible		( lua_State* L );
int LuaFunc_SetMiniMapZoomValue		( lua_State* L );
int LuaFunc_GetMinimapIconText		( lua_State* L );
int LuaFunc_GetCurrentGameTime		( lua_State* L );
int LuaFunc_GetTickCount			( lua_State* L );
int LuaFunc_GetMinimapShowOption	( lua_State* L );
int LuaFunc_SetMinimapShowOption	( lua_State* L );

int luaFunc_SetMinimapPingPosition	( lua_State* L );
int luaFunc_GetMinimapPingPosition  ( lua_State* L );

// ----------------------------------------------------------------------------
CMinimapFrame::CMinimapFrame(CInterface* object)
	: CInterfaceBase(object)
{
	g_pMinimapFrame = this;


	m_IsVisible = 1;
	m_fZoomValue = 1.0;

	m_pUiFrameView       = NULL;
	m_pUiTextureSelf     = NULL;

	// TODO 這裡應該要讀檔案, 看玩家的設定

	for( int i=0; i< (int)EM_MapIconType_END; i++ )
	{
		m_bShowOption[i] = true;
	}


	//m_bShowOption[ EM_MapIconType_Mine ]		= false;
	//m_bShowOption[ EM_MapIconType_Herb ]		= false;
	//m_bShowOption[ EM_MapIconType_Wood ]		= false;

	m_bShowOption[ EM_MapIconType_None ]		= false;


	m_pIconInfos = NULL;
	m_nIconCount = 0;
	m_nIconUsedCount = 0;

	m_bInited = false;

}

// ----------------------------------------------------------------------------
CMinimapFrame::~CMinimapFrame()
{
	g_pMinimapFrame = NULL;

	if( m_pIconInfos )
		delete [] m_pIconInfos;
}

// ----------------------------------------------------------------------------
void CMinimapFrame::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{
		lua_register( L, "InitializeMiniMap",			LuaFunc_InitializeMiniMap		);
		lua_register( L, "GetZoneName",					LuaFunc_GetZoneName				);
		lua_register( L, "SetMiniMapVisible",			LuaFunc_SetMiniMapVisible		);
		lua_register( L, "SetMiniMapZoomValue",			LuaFunc_SetMiniMapZoomValue		);
		lua_register( L, "GetMinimapIconText",			LuaFunc_GetMinimapIconText		);
		lua_register( L, "GetCurrentGameTime",			LuaFunc_GetCurrentGameTime		);
		lua_register( L, "GetTickCount",				LuaFunc_GetTickCount			);
		lua_register( L, "GetMinimapShowOption",		LuaFunc_GetMinimapShowOption	);
		lua_register( L, "SetMinimapShowOption",		LuaFunc_SetMinimapShowOption	);
		lua_register( L, "SetMinimapPingPosition",		luaFunc_SetMinimapPingPosition	);
		lua_register( L, "GetMinimapPingPosition",		luaFunc_GetMinimapPingPosition	);
	}
}

// ----------------------------------------------------------------------------
void CMinimapFrame::InitializeMinimap( const char* pViewName , const char* pMaskName, const char* pPlayerCursor, const char* pDuelRange, const char* pIconName, int nIconCount )
{
	if( !( pViewName && pMaskName && pPlayerCursor && pIconName && nIconCount ) )
		return ;

	m_pUiFrameView   = dynamic_cast<CUiFrame*>( m_interface->GetUiWorld()->FindObject( pViewName ) );
	m_pUiTextureSelf = dynamic_cast<CUiTexture*>( m_interface->GetUiWorld()->FindObject( pPlayerCursor ) );
	m_pUiDuelRange   = dynamic_cast<CUiTexture*>( m_interface->GetUiWorld()->FindObject( pDuelRange ) );

	if ( !m_pUiFrameView || !m_pUiTextureSelf || !m_pUiDuelRange )
		return ;

	if( m_pIconInfos )
		delete [] m_pIconInfos;

	m_pIconInfos = new sMinimapIconInfo[nIconCount];

	sMinimapIconInfo* pIconInfo;

	char szFrameName[MAX_PATH];
	char szTextureName[MAX_PATH];
	for( int i = 0; i < nIconCount; ++i )
	{
		pIconInfo = &m_pIconInfos[i];
		sprintf( szFrameName, "%s%d", pIconName , ( i + 1 ) );
		pIconInfo->pUiFrame = dynamic_cast<CUiFrame*>(m_interface->GetUiWorld()->FindObject( szFrameName ) );

		sprintf( szTextureName, "%s_Texture", szFrameName );
		pIconInfo->pUiTexture = dynamic_cast<CUiTexture*>(m_interface->GetUiWorld()->FindObject( szTextureName ) );

		if( !pIconInfo->pUiFrame || !pIconInfo->pUiTexture )
			return;

		pIconInfo->pUiTexture->SetFile( g_szIconTextureName ); 
	}

	m_nIconCount = nIconCount;

	m_pUiFrameView->m_pfOnPaint = UiCallback_Minimap_OnPaint;
	//m_pUiFrameView->m_pfOnClick = UiCallback_Minimap_OnClick;

	//m_pUiFrameView->SetMouseEnable(true);

	g_pRuneDev->SetMiniMapMaskFile( pMaskName );
	g_pRuneDev->SetMiniMapMask();

	m_Range = (int)m_pUiFrameView->GetSize().m_y / 2;

	CUiAnchor	anchor;
	anchor.m_relativeTo = m_pUiFrameView;
	anchor.m_offset.m_x = 0;	
	anchor.m_offset.m_y = 0;
	anchor.m_point = CUiAnchor::eAnchor_Center;
	anchor.m_relativePoint = CUiAnchor::eAnchor_Center;

	m_pUiTextureSelf->ClearAllAnchors();
	m_pUiTextureSelf->SetAnchor( anchor );

	m_pUiDuelRange->SetMaskFile( "" );
	m_pUiDuelRange->SetVisible( false );

	m_bInited = true;
}

// ----------------------------------------------------------------------------
void CMinimapFrame::RenderMinimap(CUiFrame* frame, int lParam, int rParam)
{
	if( !m_bInited )
		return;

	CRuFusion_Minimap*	minimap = g_pGameMain->GetFusionMinimap();
	CRoleSprite*		player	= g_pGameMain->GetPlayer();

	static DWORD dwLastUpdate	= 0;

	if ( minimap )
	{
		CRuRectangle	rViewport;
		CRuVector2		Origin;

		CUiPoint ViewPos   = m_pUiFrameView->GetPos();
		CUiSize  ViewSize  = m_pUiFrameView->GetRealSize();

		rViewport.m_left   = ViewPos.m_x;
		rViewport.m_right  = ViewPos.m_x + ViewSize.m_x;

		rViewport.m_top    = ViewPos.m_y;
		rViewport.m_bottom = ViewPos.m_y + ViewSize.m_y;

		Origin.m_x         = ( ViewSize.m_x )/2;
		Origin.m_y         = ( ViewSize.m_y )/2;


		minimap->SetMinimapViewport( rViewport );		// 顯示區域大小
		minimap->SetMinimapOrigin( Origin );		    // 中心點位置
		minimap->SetZoom( m_fZoomValue );
		minimap->SetMinimapLayer( static_cast<INT32>(g_pGameMain->GetActiveZoneType()) );	// 設定小地圖層次

		if ( player )
			minimap->SetPlayerPosition(player->GetPos());
		
		if ( m_IsVisible != 0 )
			minimap->Display();

	}


	if( player && GetTickCount() > dwLastUpdate )
	{
		UpdateMiniMap( m_IsVisible );
		dwLastUpdate = GetTickCount() + 40;	// 下次處理時間
	}

}

// ----------------------------------------------------------------------------
void CMinimapFrame::ShowDuelRange( float fPosX , float fPosZ , float fRange )
{
	if( !m_pUiDuelRange )
		return;
	m_DuelRange_fPosX = fPosX;
	m_DuelRange_fPosZ = fPosZ;
	m_DuelRange_fRange = fRange;

	m_pUiDuelRange->SetVisible(true);
}

// ----------------------------------------------------------------------------
void CMinimapFrame::HideDuelRange()
{
	if( !m_pUiDuelRange )
		return;
	m_pUiDuelRange->SetVisible(false);
}

//-----------------------------------------------------------------------------
bool GetNPCQuestTooltipText( string& str, GameObjDbStructEx* ObjDb, int iQuestIndex )
{
	RoleDataEx* pRole = CNetGlobal::RoleData();
	int iQuestID = pRole->PlayerBaseData->Quest.QuestLog[iQuestIndex].iQuestID;

	GameObjDbStructEx* pQuestObj = g_ObjectData->GetObj( iQuestID );
	GameObjectQuestDetail* pQuestDetail	= NULL;
	if( NULL == pQuestObj )
	{
		return false;
	}

	pQuestDetail = &( pQuestObj->QuestDetail );
	if( NULL == pQuestDetail )
	{
		return false;
	}

	if( pRole->CheckQuest_IsComplete( iQuestIndex, iQuestID, pQuestDetail ) == EM_QUESTCHECK_RESULT_OK )
	{
		return false;
	}

	string temp;
	string temp2;

	temp += pQuestObj->GetName();//  pQuestDetail->szQuest_Name;

	int i;

	//殺怪任務
	for( i = 0 ; i < _MAX_SIZE_QUEST_ITEM ; ++i )
	{
		if( pQuestDetail->iRequest_KillID[i] == ObjDb->GUID )
		{
			//int count = pRole->PlayerBaseData->Quest.iQuestKillCount[ iQuestIndex ][ i ];
			int count = pRole->PlayerBaseData->Quest.QuestLog[iQuestIndex].iQuestKillCount[i];

 			int max = pQuestDetail->iRequest_KillVal[ i ];

			if( max > 0 )
			{
				temp2 = Com_FormatString( "( %d / %d )", count, max );
				temp += temp2;
			}

			break;
		}
	}

	//殺怪掉寶
	for( i = 0 ; i < _MAX_SIZE_QUEST_CONDITION ; ++i )
	{
		if( pQuestDetail->iProcess_KilltoGetItem_ObjID[i] == ObjDb->GUID )
		{
			int ItemID = pQuestDetail->iProcess_KilltoGetItem_GetItem[i];
			GameObjDbStructEx* itemDB = g_ObjectData->GetObj( ItemID );

			if ( itemDB )
			{
				int count = pRole->Sc_GetItemNumFromRole( ItemID );
				int max = 0;

				for( int ItemIndex = 0 ; ItemIndex < _MAX_SIZE_QUEST_CONDITION ; ++ItemIndex )
				{
					if( pQuestDetail->iRequest_ItemID[ItemIndex] == 0 )
					{
						break;
					}
					if( pQuestDetail->iRequest_ItemID[ItemIndex] == ItemID )
					{
						max = pQuestDetail->iRequest_ItemVal[ItemIndex];
						break;
					}
				}

				if( max > 0 )
				{
					temp2 = Com_FormatString( " %s ( %d / %d )",itemDB->GetName(), count, max );
					temp += temp2;

				}
			}			
		}
	}	
   
	//點東西掉寶
	for( i = 0 ; i < _MAX_SIZE_QUEST_CONDITION ; ++i )
	{
		if( pQuestDetail->iProcess_ClickToGetItem_ObjID[i] == ObjDb->GUID )
		{
			int ItemID = pQuestDetail->iProcess_ClickToGetItem_GetItem[i];
			GameObjDbStructEx* itemDB = g_ObjectData->GetObj( ItemID );

			if ( itemDB )
			{
				int count = pRole->Sc_GetItemNumFromRole( ItemID );
				int max = 0;

				for( int ItemIndex = 0 ; ItemIndex < _MAX_SIZE_QUEST_CONDITION ; ++ItemIndex )
				{
					if( pQuestDetail->iRequest_ItemID[ItemIndex] == 0 )
					{
						break;
					}
					if( pQuestDetail->iRequest_ItemID[ItemIndex] == ItemID )
					{
						max = pQuestDetail->iRequest_ItemVal[ItemIndex];
						break;
					}
				}

				if( max > 0 )
				{
					temp2 = Com_FormatString( " %s ( %d / %d )",itemDB->GetName(), count, max );
					temp += temp2;
				}
			}
			break;
		}
	}
  
	str += temp;
	return true;
}


// ----------------------------------------------------------------------------
bool GetNPCAllQuestTooltipText( string& str, GameObjDbStructEx* ObjDb )
{
	RoleDataEx* pRole = CNetGlobal::RoleData();

	if( !ObjDb )
	{
		return false;
	}

	if( !( ObjDb->IsNPC() || ObjDb->IsQuestNPC() ) )
	{
		return false;
	}

	//換行
	int line = 0;

	int iQuestID = 0;
	for( int iQuestIndex = 0; iQuestIndex < _MAX_QUEST_COUNT_ ; ++iQuestIndex )
	{

		for( int i = 0 ; i < _MAX_QUESTLINK_COUNT ; ++i )
		{
			//if( ObjDb->NPC.pQuestLink[i] == NULL )
			if( ObjDb->NPC.iQuestLinkObj[i] == 0 )
			{
				break;
			}
   
  			iQuestID = ObjDb->NPC.iQuestLinkObj[i];

			if( iQuestID == pRole->PlayerBaseData->Quest.QuestLog[iQuestIndex].iQuestID )
			{
				string temp;
				if( GetNPCQuestTooltipText( temp, ObjDb, iQuestIndex ) )
				{
					if( line > 0 )
					{
						str += "\n";
					}
					str += temp;
					++line;
					break;
				}
			}
		}

	}

	if( line > 0 )
		return true;

	return false;
}

// ----------------------------------------------------------------------------
bool CheckNPCQuestLink( GameObjDbStructEx* ObjDb )
{
	RoleDataEx* pRole = CNetGlobal::RoleData();

	bool bUnComplete = false;
	int iQuestID	= 0;
	for( int i = 0 ; i < _MAX_QUEST_COUNT_ ; ++i )
	{
		int iQuestID = pRole->PlayerBaseData->Quest.QuestLog[ i ].iQuestID;
		GameObjDbStruct* pQuestDb = g_ObjectData->GetObj( iQuestID );
		if ( pQuestDb == NULL )
			continue;

		GameObjectQuestDetail* pQuest = &( pQuestDb->QuestDetail );
		for( int j = 0 ; j < _MAX_QUESTLINK_COUNT; ++j )
		{
			//if( ObjDb->NPC.pQuestLink[j] == NULL )
			if( ObjDb->NPC.iQuestLinkObj[j] == 0 )
			{
				continue;
			}

			//if( iQuestID == ObjDb->NPC.pQuestLink[j]->GUID )
			if( iQuestID == ObjDb->NPC.iQuestLinkObj[j] )
			{
				if( pRole->CheckQuest_IsComplete( i, iQuestID, pQuest ) != EM_QUESTCHECK_RESULT_OK )
				{
					return true;
				}
			}
		}
	}

	return false;
}


// ----------------------------------------------------------------------------
void CMinimapFrame::UpdateMiniMap( int iVisible )
{
	if( !m_bInited )
		return;

	multimap < float, CRoleSprite* >	mapRoleDistance;
	
	map<int, CRoleSprite*>&		spriteMap			= g_pRuneDev->GetSpriteMap();
	CRoleSprite*				pPlayer				= g_pGameMain->GetPlayer();
	RoleDataEx*					pRole				= CNetGlobal::RoleData();
	CRuFusion_Minimap*			pMinimap			= g_pGameMain->GetFusionMinimap();
	CRoleSprite*				pObj				= NULL;
	
	CRuVector3					dir;
	float						magnitude;

	if( NULL == pMinimap )
	{
		return;
	}


	float fWorldRange = pMinimap->MinimapRadiusToWorldRadius( (REAL)m_Range );

	const char* pTextureName = NULL;

	// 暫時測試 標出前 10 距離近的

	CRuVector3	Pos;
	CUiPoint	IconPos;
	CUiAnchor	anchor;


	// ----------------------------------------------------------------------------
	//m_pUiTextureSelf

	CRuVector3 direction = pPlayer->GetDirection();

	//float angle = acos(DotProduct(CRuVector3(0.0f, 0.0f, 1.0f), direction));
	//REAL tripleProduct = TripleProduct_Scalar(CRuVector3(0.0f, 1.0f, 0.0f), direction, CRuVector3(0.0f, 0.0f, 1.0f));

	//if(tripleProduct < 0.0f)
	//{
	//	angle = ruPI * 2.0f - angle;
	//}
	//m_pUiTextureSelf->SetRotate( -angle );


	//angle = asin( direction.m_x / sqrtf ( direction.m_x * direction.m_x + direction.m_z * direction.m_z ) );
	//上式因為direction 是正規化向量 故可改成 asin( direction.m_x / 1 ); => asin( direction.m_x );
	//float angle = asin( direction.m_x );

	//direction 不是正規化向量 = =
   	float angle = asin( direction.m_x / sqrtf ( direction.m_x * direction.m_x + direction.m_z * direction.m_z ) );

	if( direction.z < 0.0f )
	{
		angle = ruPI - angle ;
	}

	m_pUiTextureSelf->SetRotate( angle );

	// ----------------------------------------------------------------------------	
	//重置icon
	m_nIconUsedCount = 0;
	for( int i = 0; i < m_nIconCount; ++i )
	{
		m_pIconInfos[i].pUiFrame->SetVisible( false );
	}

	if( iVisible == 0 )
	{
		m_pUiTextureSelf->SetVisible( false );
		return;
	}

	//UpdateTrack();


	// ----------------------------------------------------------------------------
	m_pUiTextureSelf->SetVisible( true );

	map<int, CRoleSprite*>::iterator itPos = spriteMap.begin();
	map<int, CRoleSprite*>::iterator itEnd = spriteMap.end();

	for ( ; itPos != itEnd; ++itPos )
	{
		pObj	= itPos->second;

		dir		= pPlayer->GetPos() - pObj->GetPos();
		dir.m_y = 0;
		magnitude = dir.Magnitude();	

		if( magnitude >= fWorldRange ||	pPlayer == pObj || false == pObj->IsVisible() )
		{
			continue;
		}
		else
		{
			// 加入判斷檢查物件是否顯示

			int iIconID = pObj->GetQuestMinimapIcon();

			//任務是否已完成了
			if(	iIconID == EM_MapIconType_QuestNPC ||
				iIconID == EM_MapIconType_QuestOBJ )
			{
				//怪物死了
				if( pObj->GetHP() == 0 )
				{
					continue;	
				}

		 		GameObjDbStructEx* ObjDb = g_ObjectData->GetObj( pObj->GetObjectId() );

				if( ObjDb == NULL || !CheckNPCQuestLink( ObjDb ) )
				{
					continue;
				}
			}

			if( iIconID != EM_MapIconType_NewQuest      		&&  // 新任務
				iIconID != EM_MapIconType_UnfinishQuest 		&&  // 未完成任務
				iIconID != EM_MapIconType_FinishedQuest 		&&  // 完成任務
				iIconID != EM_MapIconType_TrustQuest    		&&   // 委託任務
				iIconID != EM_MapIconType_QuestNPC				&&
				iIconID	!= EM_MapIconType_GrayQuest				&&
				iIconID	!= EM_MapIconType_RepeatQuest			&&
				iIconID	!= EM_MapIconType_RepeatQuestDone		&&
				iIconID != EM_MapIconType_QuestOBJ				&&
				iIconID != EM_MapIconType_PublicQuest			&&
				iIconID != EM_MapIconType_PublicQuestDone							
				)
			{
				iIconID = pObj->GetMinimapIcon();
			}
  
			if( iIconID == EM_MapIconType_TrustQuest )
			{
				iIconID = EM_MapIconType_NewQuest;
			}
			else if( 
				iIconID == EM_MapIconType_Weapon ||	// 武器屋
				iIconID == EM_MapIconType_Armor  ||	// 防具屋			
				iIconID == EM_MapIconType_Potion ||	// 藥水店			
				iIconID == EM_MapIconType_Food   ||	// 飲食店			
				iIconID == EM_MapIconType_Magic  ||	// 魔法商店			
				iIconID == EM_MapIconType_Mat    ||	// 材料行			
				iIconID == EM_MapIconType_Shop   ||	// 雜貨店
				iIconID == EM_MapIconType_Fish   ||	// 漁具店			
				iIconID == EM_MapIconType_Recipe )	// 配方
			{
				iIconID = EM_MapIconType_Shop;
			}
	 	
			if( iIconID == EM_MapIconType_None ||
				m_bShowOption[ iIconID ] == false )
				continue;

			mapRoleDistance.insert( pair< float, CRoleSprite*> ( magnitude, pObj ) );		
		}
	}

	if( m_nIconUsedCount >= m_nIconCount )
		return;

	sMinimapIconInfo* pIconInfo;
		
	// 設定範圍內物件的圖點
	CRuRectangle rect;

	for( multimap < float, CRoleSprite* >::iterator it = mapRoleDistance.begin(); it != mapRoleDistance.end(); it++ )
	{
		pIconInfo = &m_pIconInfos[m_nIconUsedCount];

		pObj = it->second;
		Pos  = pMinimap->WorldToMinimap( pObj->GetPos() );

		EMapIconType eID = (EMapIconType)pObj->GetQuestMinimapIcon();
		if( eID == EM_MapIconType_None )
			 eID = (EMapIconType)pObj->GetMinimapIcon();

		//不能打的任務NPC
		if( eID == EM_MapIconType_QuestNPC && false == pObj->GetCanAttackState() )
		{
			eID = EM_MapIconType_QuestOBJ;
		}
   		
		Func_GetMapIconTextureInfo( eID, rect );
		pIconInfo->pUiTexture->SetTexCoords( 1 , &rect );
		pIconInfo->pUiTexture->SetColor( 1.0f, 1.0f, 1.0f );
		pIconInfo->pUiTexture->SetRotate( 0 );
	
		//pIconInfo->pUiFrame->SetID( pObj->GetIdentify() );
		//pIconInfo->pUiFrame->SetPos( IconPos );

		anchor.m_relativeTo = m_pUiFrameView;
		anchor.m_offset.m_x = Pos.m_x;	
		anchor.m_offset.m_y = Pos.m_y;
		anchor.m_point = CUiAnchor::eAnchor_Center;
		anchor.m_relativePoint = CUiAnchor::eAnchor_Center;

		pIconInfo->pUiFrame->ClearAllAnchors();
		pIconInfo->pUiFrame->SetAnchor( anchor );

		pIconInfo->pUiFrame->SetVisible( true );

		//pIconInfo->strName = pObj->GetName();
		//顯示任務連結

		string str = pObj->GetName();

		int iIconID = pObj->GetQuestMinimapIcon();

		if(	iIconID == EM_MapIconType_QuestNPC ||
			iIconID == EM_MapIconType_QuestOBJ )
		{
			string temp;
			GameObjDbStructEx* ObjDb = g_ObjectData->GetObj( pObj->GetObjectId() );
			if( GetNPCAllQuestTooltipText( temp, ObjDb ) )
			{
				str += "\n";
				str += temp;
			}
		}

		pIconInfo->strName = str;

		++m_nIconUsedCount;
	
		if( m_nIconUsedCount == m_nIconCount )
			break;
	}

	// ----------------------------------------------------------------------------
	//更新追蹤
	{
		int DBZoneID = g_pGameMain->GetZoneID() % _DEF_ZONE_BASE_COUNT_ + Def_ObjectClass_Zone ;
		GameObjDbStruct* zoneDB = CNetGlobal::GetObj( DBZoneID );

		SMapTrackInfo* pTrackInfo;
		int i;
		//追蹤隊友
		for( i = 0 ; i < (int)g_pWorldMapFrame->m_TrackTeammate.size() ; ++i )
		{
			if( m_nIconUsedCount >= m_nIconCount ) 
				break;

			pTrackInfo = &g_pWorldMapFrame->m_TrackTeammate[i];

			if( pTrackInfo->show && 
				pTrackInfo->coord.wdbFile == zoneDB->Zone.MapFile )
			{
				UpdateTrack( pTrackInfo );
			}		
		}

		//追蹤NPC
		for( i = 0 ; i < (int)g_pWorldMapFrame->m_TrackNPC.size() ; ++i )
		{
			if( m_nIconUsedCount >= m_nIconCount ) 
				break;

			pTrackInfo = &g_pWorldMapFrame->m_TrackNPC[i];

			if( pTrackInfo->show && 
				pTrackInfo->coord.wdbFile == zoneDB->Zone.MapFile )
			{
				UpdateTrack( pTrackInfo );
			}		
		}

		//AddMapIcon
		std::map< int , std::vector< SMapTrackInfo > >::iterator itPos = g_pWorldMapFrame->m_MapIcon.begin();
		std::map< int , std::vector< SMapTrackInfo > >::iterator itEnd = g_pWorldMapFrame->m_MapIcon.end();

		for( ; itPos != itEnd ; ++itPos )
		{
			std::vector< SMapTrackInfo >& table = itPos->second ;


			for( i = 0 ; i < (int)table.size() ; ++i )
			{
				if( m_nIconUsedCount >= m_nIconCount ) 
					break;

				pTrackInfo = &table[i];

				if( pTrackInfo->show && 
					pTrackInfo->coord.wdbFile == zoneDB->Zone.MapFile )
				{
					UpdateTrack( pTrackInfo );
				}		
			}

		}









 	}
 
	//決鬥範圍
	if( m_pUiDuelRange->IsVisible() )
	{
		//float fSize = m_DuelRange_fRange  * ( 1.0f /  MinimapRadiusToWorldRadius( 1.0f ) );
		float fSize = m_DuelRange_fRange / pMinimap->MinimapRadiusToWorldRadius( 1.0f );
		fSize = fSize * 2.0f; //半徑轉直徑

		Pos = pMinimap->WorldToMinimap(  CRuVector3( m_DuelRange_fPosX , 0.0 , m_DuelRange_fPosZ ) );

		anchor.m_relativeTo = m_pUiFrameView;
		anchor.m_offset.m_x = Pos.m_x;	
		anchor.m_offset.m_y = Pos.m_y;
		anchor.m_point = CUiAnchor::eAnchor_Center;
		anchor.m_relativePoint = CUiAnchor::eAnchor_Center;

		m_pUiDuelRange->ClearAllAnchors();
		m_pUiDuelRange->SetAnchor( anchor );
		m_pUiDuelRange->SetSize( CUiSize( fSize , fSize ) );
 	}

}

// ----------------------------------------------------------------------------
void CMinimapFrame::UpdateTrack( SMapTrackInfo* pInfo )
{
	EMapIconType eID;
	float angle;
	sMinimapIconInfo* pIconInfo;

	CRuFusion_Minimap* pMinimap = g_pGameMain->GetFusionMinimap();
	pIconInfo = &m_pIconInfos[m_nIconUsedCount];

	CRuVector3 dir;
	float      fMagnitude;

	dir.m_x = pInfo->coord.PosX;
	dir.m_y = 0.0f;
	dir.m_z = pInfo->coord.PosZ;

	dir = pMinimap->WorldToMinimap( dir );

	static int s_RangeDelta	= 18;
   
	if( dir.Magnitude() >= m_Range )
	{
		//沒有追蹤箭頭
		if( !pInfo->trackArrow )
		{
			return;
		}


		dir.Normalize();
		angle = asin( dir.m_x );

		if( dir.y > 0.0f )
		{
			angle = ruPI - angle ;
		}

  		//內縮一點
		fMagnitude= (REAL)(m_Range - s_RangeDelta);
		dir = dir * fMagnitude;

		if( pInfo->icontype == EM_MapIconType_TrackNpc )
		{
			eID = EM_MapIconType_TrackNpcArrow;
  			pIconInfo->pUiTexture->SetColor( pInfo->r, pInfo->g, pInfo->b );
		}
		else
		{
			eID = EM_MapIconType_TrackArrow; 
	 		pIconInfo->pUiTexture->SetColor( 1.0f, 1.0f, 1.0f );
		}
	}
	else
	{
		angle = 0.0f;
		eID = (EMapIconType)pInfo->icontype;

		pIconInfo->pUiTexture->SetColor( pInfo->r, pInfo->g, pInfo->b );
	}

	CRuRectangle rect;
	Func_GetMapIconTextureInfo( eID, rect );
	pIconInfo->pUiTexture->SetTexCoords( 1 , &rect );

	pIconInfo->pUiTexture->SetRotate( angle );

	CUiAnchor anchor;
	anchor.m_relativeTo = m_pUiFrameView;
	anchor.m_offset.m_x = dir.m_x;	
	anchor.m_offset.m_y = dir.m_y;
	anchor.m_point = CUiAnchor::eAnchor_Center;
	anchor.m_relativePoint = CUiAnchor::eAnchor_Center;

	pIconInfo->pUiFrame->ClearAllAnchors();
	pIconInfo->pUiFrame->SetAnchor( anchor );
	pIconInfo->pUiFrame->SetVisible( true );

	pIconInfo->strName = pInfo->strName;

	++m_nIconUsedCount;

}

// ----------------------------------------------------------------------------
void CMinimapFrame::BuildFrames()
{
	// 註冊繪出小地圖函式
	// 先取得CUIFrame* 指標,在設定繪出函式
}

// ----------------------------------------------------------------------------
void CMinimapFrame::DestroyFrames()
{
	m_pUiFrameView		= NULL;
	m_pUiTextureSelf	= NULL;
	m_pUiDuelRange		= NULL;
}

// ----------------------------------------------------------------------------
const char* CMinimapFrame::GetMinimapIconText( int IconID )
{
	for( int i = 0; i < m_nIconCount; ++i )
	{
		if( IconID == m_pIconInfos[i].pUiFrame->GetID() )
		{
			return m_pIconInfos[i].strName.c_str();
		}
	}
	return NULL;
}

// ----------------------------------------------------------------------------
void CMinimapFrame::SetPingPosition( const CUiPoint& rPoint )
{
	const WorldMapInfoStruct* pWorldMapInfo = g_ObjectData->GetWorldMapInfoByID( g_pGameMain->GetWorldMapID() );
	if( !pWorldMapInfo )
		return;
 
	CRuFusion_Minimap* pMinimap = g_pGameMain->GetFusionMinimap();
	CRuVector3 position( rPoint.m_x , rPoint.m_y , 0.0 );
	position = pMinimap->MinimapToWorld( position );

 	SMapCoordinate MapCoord;
	MapCoord.MapID = pWorldMapInfo->MapID ;

	MapCoord.PosX = ( position.m_x - pWorldMapInfo->LX ) / ( pWorldMapInfo->RX - pWorldMapInfo->LX ) ;
	MapCoord.PosZ = ( position.m_z - pWorldMapInfo->LZ ) / ( pWorldMapInfo->RZ - pWorldMapInfo->LZ ) ;

  	g_pWorldMapFrame->SetPingPosition( MapCoord );
}

// ----------------------------------------------------------------------------
bool CMinimapFrame::GetPingPosition( CUiPoint& rPoint )
{
	const WorldMapInfoStruct* pWorldMapInfo = g_ObjectData->GetWorldMapInfoByID( g_pGameMain->GetWorldMapID() );
	if( !pWorldMapInfo )
		return false;

	const SWorldCoordinate& rWorldCoord = g_pWorldMapFrame->GetPingPosition();

 
 	if( rWorldCoord.wdbFile != pWorldMapInfo->wdbFile )
	{
		rPoint.m_x = 0;
		rPoint.m_y = 0;
		return false;
	}

	//轉小地圖座標
	CRuFusion_Minimap* pMinimap = g_pGameMain->GetFusionMinimap();
	CRuVector3 position = pMinimap->WorldToMinimap( CRuVector3( rWorldCoord.PosX , 0.0f , rWorldCoord.PosZ ) );
	rPoint.m_x = position.m_x ;
	rPoint.m_y = position.m_y ;

	return true;
}

// ----------------------------------------------------------------------------
void UiCallback_Minimap_OnPaint(CUiFrame* frame, INT64 lParam, INT64 rParam)
{
	g_pMinimapFrame->RenderMinimap( frame, (int)lParam, (int)rParam);
}

// ----------------------------------------------------------------------------
//void UiCallback_Minimap_OnClick(CUiFrame* frame, int lParam, int rParam)
//{
//	g_pMinimapFrame->ClickMinimap( frame, lParam, rParam);
//}

// ----------------------------------------------------------------------------
int LuaFunc_InitializeMiniMap( lua_State* L )
{
	int n = lua_gettop(L);
	if( n != 6 )
	{
		return 0;
	}

	const char* pViewName     = luaL_checkstring( L, 1 );
	const char* pMaskName     = luaL_checkstring( L, 2 );
	const char* pPlayerCursor = luaL_checkstring( L, 3 );
	const char* pDuelRange    = luaL_checkstring( L, 4 );
	const char* pIconName     = luaL_checkstring( L, 5 );
	int         nIconCount    = (int)luaL_checknumber( L, 6 );

	g_pMinimapFrame->InitializeMinimap( pViewName , pMaskName, pPlayerCursor, pDuelRange, pIconName, nIconCount );

	return 0;
}
// ----------------------------------------------------------------------------
int	LuaFunc_GetZoneName			( lua_State* L )
{
	const char* szName = Func_GetLocalMapName( g_pGameMain->GetZoneName() );
	lua_pushstring( L, szName );
	return 1;
}
// ----------------------------------------------------------------------------
int	LuaFunc_SetMiniMapVisible	( lua_State* L )
{
	int iVisible = luaL_checkint( L, 1 );
	
	if( iVisible != 0 )
		g_pMinimapFrame->m_IsVisible = 1;
	else
		g_pMinimapFrame->m_IsVisible = 0;

	return 0;
}

// ----------------------------------------------------------------------------
int	LuaFunc_SetMiniMapZoomValue	( lua_State* L )
{
	float fZoomValue = (float)luaL_checknumber( L, 1 );
	g_pMinimapFrame->m_fZoomValue = fZoomValue;
	return 0;
}

// ----------------------------------------------------------------------------
int	LuaFunc_GetMinimapIconText	( lua_State* L )
{
	int iID  = (int)luaL_checknumber( L, 1 );

	const char* pText = g_pMinimapFrame->GetMinimapIconText( iID );

	if( pText )
	{
		lua_pushstring( L, pText );
		return 1;
	}

	return 0;
}
// ----------------------------------------------------------------------------
int	LuaFunc_GetCurrentGameTime	( lua_State* L )
{
	int iTime = g_pGameMain->GetCurrentGameTime();
	lua_pushnumber( L, iTime );

	return 1;
}
// ----------------------------------------------------------------------------
int	LuaFunc_GetTickCount		( lua_State* L )
{
	int iTime = (int)GetTickCount();
	lua_pushnumber( L, iTime );
	return 1;
}
// ----------------------------------------------------------------------------
int	LuaFunc_GetMinimapShowOption	( lua_State* L )
{
	int iID = (int)luaL_checknumber( L, 1 );

	if( iID >= 0 && iID < EM_MapIconType_END )
	{
		if( g_pMinimapFrame->m_bShowOption[ iID ] )
		{
			lua_pushnumber( L, 1 );
			return 1;
		}
		else
			return 0;
	}
	
	return 0;
}

// ----------------------------------------------------------------------------
int	LuaFunc_SetMinimapShowOption	( lua_State* L )
{
	int iID	 = (int)luaL_checknumber( L, 1 );
	int iSet = (int)luaL_checknumber( L, 2 );

	if( iID >= 0 && iID < EM_MapIconType_END )
	{
		g_pMinimapFrame->m_bShowOption[ iID ] = ( iSet == 1 ) ? true : false;
	}

	return 0;
}

// ----------------------------------------------------------------------------
int luaFunc_SetMinimapPingPosition( lua_State* L )
{
	CUiPoint point;
	point.m_x = (float)luaL_checknumber( L, 1 );
	point.m_y = (float)luaL_checknumber( L, 2 );
	g_pMinimapFrame->SetPingPosition( point );

	return 0;
}

// ----------------------------------------------------------------------------
int luaFunc_GetMinimapPingPosition( lua_State* L )
{
	CUiPoint Point; 
	if( g_pMinimapFrame->GetPingPosition( Point ) )
	{
		lua_pushnumber( L, Point.m_x );
		lua_pushnumber( L, Point.m_y );
		return 2;
	}
	return 0;
}



















