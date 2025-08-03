#include <windows.h>
#include "sprite.h"
#include "../mainproc/NetGlobal.h"
#include "../ErrorList/errorlist.h"

#define MAX_ACTION_EVENTS				1000

//-----------------------------------------------------------------------------
set<IBaseSprite*>				IBaseSprite::s_containers;				// 
CRuFontType*					IBaseSprite::s_pFontType;				// 字型資料
CRuOptimalTerrain*				IBaseSprite::s_pRuTerrain;				// 地表資訊
CRuGlobalEventManager*			IBaseSprite::s_pRuEventListener;		// 動作事件
CRuFusion_Mime_Template*		IBaseSprite::s_pRuMimeTemplate;
CRuFusion_Mime_Template*		IBaseSprite::s_pRuMimeTemplate_Deprecated;
SpriteMsgProc					IBaseSprite::s_procFunc;
HideEffectFunc					IBaseSprite::s_hideEffectFunc = NULL;	// 隱藏特效
bool							IBaseSprite::s_gm_ShowHidden = false;
INT32							IBaseSprite::s_probe_SpriteHouseKeeping = -1;
CRuPaperdollCache*				IBaseSprite::s_paperdollCache = NULL;			// Paperdoll cache manager
CRuArrayList<IBaseSprite *>		IBaseSprite::s_colSprites;
CRuArrayList<IBaseSprite *>		IBaseSprite::s_plotColSprites;
CRuContainer_Collision*			IBaseSprite::s_collisionContainer = NULL;
int								IBaseSprite::s_localPlayerId = 0;
float							IBaseSprite::s_fontScale = 1.0f;
bool							IBaseSprite::s_isNoBlood = true;

bool							g_baseSpriteLockDelete	 = false;
CRuArrayList<IBaseSprite *>		g_baseSpriteList;

//-----------------------------------------------------------------------------
void IBaseSprite::Initialize()
{
	s_containers.clear();	

	// 事件觸發控制物件
	s_pRuEventListener				= ruNEW CRuGlobalEventManager;

	// 設定傀儡控制物件
	s_pRuMimeTemplate				= ruNEW CRuFusion_Mime_Template();
	s_pRuMimeTemplate_Deprecated	= ruNEW CRuFusion_Mime_Template();
	s_pRuMimeTemplate_Deprecated->InitializeDeprecatedBoneRotators();
	s_pRuMimeTemplate_Deprecated->InitializeDeprecatedBoneScalers();

	s_pFontType = NULL;
	s_pRuTerrain = NULL;
	s_procFunc = NULL;	

	// Create collision container
	s_collisionContainer = ruNEW CRuContainer_Collision();
}

//-----------------------------------------------------------------------------
void IBaseSprite::Shutdown()
{
	// Delete all existing sprites
	set<IBaseSprite*>::iterator iter;
	while ( (iter = s_containers.begin()) != s_containers.end() )
		delete *iter;

	s_colSprites.Clear();
	s_plotColSprites.Clear();

	// Release collision container
	ruSAFE_RELEASE(s_collisionContainer);

	// Release global event listener
	if ( s_pRuEventListener )
	{
		delete s_pRuEventListener;
		s_pRuEventListener = NULL;
	}	

	// Release mime templates
	ruSAFE_RELEASE(s_pRuMimeTemplate);
	ruSAFE_RELEASE(s_pRuMimeTemplate_Deprecated);

	s_pFontType = NULL;
	s_pRuTerrain = NULL;
	s_procFunc = NULL;
}

//-----------------------------------------------------------------------------
void IBaseSprite::ReleaseAll()
{
	// Delete all existing sprites
	set<IBaseSprite*>::iterator iter;
	while ( (iter = s_containers.begin()) != s_containers.end() )
		delete *iter;

	s_colSprites.Clear();
	s_plotColSprites.Clear();
	s_pRuTerrain = NULL;

	// Clear instance manager cache
	g_ruInstanceManager->ClearCache();

	// Flush resource manager cache now
	g_ruResourceManager->GarbageCollect();
}

//-----------------------------------------------------------------------------
void InsertSpriteUpdateList(CRuArrayList<IBaseSprite *>& spriteUpdateList, IBaseSprite* curSprite)
{
	// 優先加入父元件
	if ( curSprite->GetParentMount() )
	{
		InsertSpriteUpdateList(spriteUpdateList, curSprite->GetParentMount());
	}

	// 排除已經加入物件
	for ( int i = 0; i < spriteUpdateList.Count(); i++ )
	{
		if ( curSprite == spriteUpdateList[i] )
			return;
	}

	spriteUpdateList.Add(curSprite);
}

//-----------------------------------------------------------------------------
void IBaseSprite::UpdateAll( float elapsedTime )
{
	if(s_probe_SpriteHouseKeeping == -1)
	{
		s_probe_SpriteHouseKeeping = g_ruProfiler->CreateProbe("Update Sprite (HK)");
	}

	RUPROFILE_SAMPLE_BEGIN(s_probe_SpriteHouseKeeping, 0);

	// Retrieve relevant properties for use during update
	RuPropertyType propertyValue;

	if(g_ruEngineSettings->GetSetting("gm:show hidden", propertyValue))
	{
		s_gm_ShowHidden = boost::get<BOOL>(propertyValue) ? true : false;
	}

	// Update shadow display mode for all sprites
	UpdateShadowMode();

	// Update event listener
	s_pRuEventListener->Update( elapsedTime );

	// Run one pass through to locate sprites which have enabled plot collision trigger or inter-object collision
	s_colSprites.Clear();
	s_plotColSprites.Clear();	

	// 刪除自解物件
	vector<IBaseSprite*> removeList;
	set<IBaseSprite*>::iterator removeIter = s_containers.begin();
	while ( removeIter != s_containers.end() )
	{
		IBaseSprite *sprite = *removeIter;

		if ( sprite->IsDestruct() && sprite->GetDestructTime() < elapsedTime )
		{
			removeList.push_back(sprite);
			// removeIter = s_containers.erase(removeIter);

			removeIter++;
		}
		else
		{
			removeIter++;
		}
	}

	for ( vector<IBaseSprite*>::iterator iter = removeList.begin(); iter != removeList.end(); iter++ )
		delete (*iter);

	// Refresh parent mounts
	for ( set<IBaseSprite*>::iterator iter = s_containers.begin(); iter != s_containers.end(); iter++ )
	{
		IBaseSprite* sprite = *iter;

		if((*iter)->CollisionEnabled())
		{
			//CRuEntity *colEntity = (*iter)->GetCurrentEntity();
			CRuEntity *colEntity = (*iter)->GetRuEntity();
			if(colEntity)
			{
				// Insert entity into collision container
				s_collisionContainer->InsertCollisionObject(colEntity, colEntity->GetWorldBounds(), TRUE);

				// Set entity as a valid decal target
				colEntity->SetOption(ruENTITYOPTION_DECALTARGET, TRUE);
			}

			// Insert into list of collision sprites
			s_colSprites.Add(*iter);

		}

		// Insert into list of plot collision sprites if plot collision trigger is enabled
		if((*iter)->PlotCollisionTriggerEnabled())
		{
			s_plotColSprites.Add(*iter);
		}
	}

	// Insert terrain as a collision object
	s_collisionContainer->InsertCollisionObject(s_pRuTerrain, s_pRuTerrain->GetWorldBounds(), TRUE);

	// Update the collision container
	s_collisionContainer->Update(elapsedTime);

	RUPROFILE_SAMPLE_END(s_probe_SpriteHouseKeeping, 0);

	// Build an ordered sprite update list based on parent mount relations
	g_baseSpriteList.Clear();

	// Refresh parent mounts	
	for ( set<IBaseSprite*>::iterator iter = s_containers.begin(); iter != s_containers.end(); iter++ )
	{
		IBaseSprite *sprite = *iter;

		// Find and set parent mount
		if( sprite->GetServerMove() == true )
		{
			IBaseSprite *parentMount = sprite->FindMountCandidate();
			sprite->SetParentMount(parentMount);
		}

		InsertSpriteUpdateList( g_baseSpriteList, sprite );
	}

	// Update sprites
	for(INT32 i = 0; i < g_baseSpriteList.Count(); ++i)
	{
		IBaseSprite *sprite = g_baseSpriteList[i];

		// 指標被移除
		if ( s_containers.find(sprite) == s_containers.end() )
			continue;

		// Move sprite with parent mount
		sprite->MoveWithParentMount();
		
		if ( sprite->GetPauseTime() < 0.00001f )
		{
			// Update destruction time
			if ( sprite->IsDestruct() )
				sprite->SetDestructTime(sprite->GetDestructTime() - elapsedTime);

			// 
			sprite->Update( elapsedTime );
			sprite->UpdateLinkedSprites();
		}
		else
		{
			sprite->SetPauseTime(sprite->GetPauseTime() - elapsedTime);
		}
	}
}

//-----------------------------------------------------------------------------
void IBaseSprite::UpdateShadowMode()
{
	RuPropertyType propertyValue;

	// Update shadow type based on the shadow detail level for all sprites
	if(g_ruEngineSettings->GetSetting("shadow:detail level", propertyValue))
	{
		INT32 shadowDetailLevel = boost::get<INT32>(propertyValue);

		switch(shadowDetailLevel)
		{
			default:
			case 0:		// No shadows
				{
					set<IBaseSprite*> &spriteContainer = IBaseSprite::GetContainer();
					for(set<IBaseSprite*>::iterator iter = spriteContainer.begin(); iter != spriteContainer.end(); iter++)
					{
						(*iter)->SetShadowType(eSpriteShadowType_None);
					}
				}

				break;

			case 1:		// All dot shadows
				{
					set<IBaseSprite*> &spriteContainer = IBaseSprite::GetContainer();
					for(set<IBaseSprite*>::iterator iter = spriteContainer.begin(); iter != spriteContainer.end(); iter++)
					{
						switch((*iter)->GetObjectType())
						{
							default:
							case eSpriteObject_Unknown:
							case eSpriteObject_Effect:
								(*iter)->SetShadowType(eSpriteShadowType_None);
								break;

							case eSpriteObject_Player:
								if ( (*iter)->GetObjectId() == DUMMY_ID )
									(*iter)->SetShadowType(eSpriteShadowType_None);
								else								
									(*iter)->SetShadowType(eSpriteShadowType_Dot);
								break;

							case eSpriteObject_NPC:
							case eSpriteObject_QuestNPC:
								{
									GameObjDbStructEx *pObj = CNetGlobal::GetObj((*iter)->GetObjectId());
									if ( pObj )
									{
										GameObjDbStructEx* imageDB = CNetGlobal::GetObj(pObj->ImageID);
										if ( imageDB && imageDB->Image.IsShowShadow )
										{
											(*iter)->SetShadowType(eSpriteShadowType_Dot);
										}
										else
										{
											(*iter)->SetShadowType(eSpriteShadowType_None);
										}
									}
								}
								break;
						}
					}
				}

				break;

			case 2:		// Only players have real shadows
				{
					set<IBaseSprite*> &spriteContainer = IBaseSprite::GetContainer();
					for(set<IBaseSprite*>::iterator iter = spriteContainer.begin(); iter != spriteContainer.end(); iter++)
					{
						switch((*iter)->GetObjectType())
						{
							default:
							case eSpriteObject_Unknown:
							case eSpriteObject_Effect:
								(*iter)->SetShadowType(eSpriteShadowType_None);
								break;

							case eSpriteObject_Player:
								if ( (*iter)->GetObjectId() == DUMMY_ID )
									(*iter)->SetShadowType(eSpriteShadowType_None);
								else								
									(*iter)->SetShadowType(eSpriteShadowType_Projected);
								break;

							case eSpriteObject_NPC:
							case eSpriteObject_QuestNPC:
								{
									GameObjDbStructEx *pObj = CNetGlobal::GetObj((*iter)->GetObjectId());
									if ( pObj )
									{
										GameObjDbStructEx* imageDB = CNetGlobal::GetObj(pObj->ImageID);
										if ( imageDB && imageDB->Image.IsShowShadow )
										{
											(*iter)->SetShadowType(eSpriteShadowType_Dot);
										}
										else
										{
											(*iter)->SetShadowType(eSpriteShadowType_None);
										}
									}
								}
								break;
						}
					}
				}

				break;

			case 3:		// Everybody has real shadows
				{
					set<IBaseSprite*> &spriteContainer = IBaseSprite::GetContainer();
					for(set<IBaseSprite*>::iterator iter = spriteContainer.begin(); iter != spriteContainer.end(); iter++)
					{
						switch((*iter)->GetObjectType())
						{
							default:
							case eSpriteObject_Unknown:
							case eSpriteObject_Effect:
								(*iter)->SetShadowType(eSpriteShadowType_None);
								break;

							case eSpriteObject_Player:
								if ( (*iter)->GetObjectId() == DUMMY_ID )
									(*iter)->SetShadowType(eSpriteShadowType_None);
								else								
									(*iter)->SetShadowType(eSpriteShadowType_Projected);
								break;

							case eSpriteObject_NPC:
							case eSpriteObject_QuestNPC:
								{
									GameObjDbStructEx *pObj = CNetGlobal::GetObj((*iter)->GetObjectId());
									if ( pObj )
									{
										GameObjDbStructEx* imageDB = CNetGlobal::GetObj(pObj->ImageID);
										if ( imageDB && imageDB->Image.IsShowShadow )
										{
											(*iter)->SetShadowType(eSpriteShadowType_Projected);
										}
										else
										{
											(*iter)->SetShadowType(eSpriteShadowType_None);
										}
									}
								}
								break;
						}
					}
				}

				break;
		}
	}
}

//-----------------------------------------------------------------------------
void IBaseSprite::RenderAll()
{
	// Update sprites
	for(INT32 i = 0; i < g_baseSpriteList.Count(); ++i)
	{
		IBaseSprite *sprite = g_baseSpriteList[i];

		// 指標被移除
		if ( s_containers.find(sprite) == s_containers.end() )
			continue;

		if ( sprite->GetPauseTime() < 0.000001f && sprite->CheckRenderState() )
		{
			sprite->Render();
		}
	}
}

//-----------------------------------------------------------------------------
void IBaseSprite::SetFontType( CRuFontType* pFontType )
{
	s_pFontType = pFontType;
}

//-----------------------------------------------------------------------------
void IBaseSprite::SetRuTerrain( CRuOptimalTerrain* pRuTerrain )
{
	s_pRuTerrain = pRuTerrain;
}

//-----------------------------------------------------------------------------
CRuContainer_Collision *IBaseSprite::GetCollisionContainer()
{
	return s_collisionContainer;
}

//-----------------------------------------------------------------------------
bool IBaseSprite::CheckedSpriteIsLife( IBaseSprite* pBaseSprite )
{
	return ( s_containers.find( pBaseSprite ) != s_containers.end() );
}

//-----------------------------------------------------------------------------
const char* IBaseSprite::GetAttachPointName( int point )
{
	switch ( point )
	{
	case eAttach_Top:
		return ATTACH_POINT_TOP;

	case eAttach_Down:
		return ATTACH_POINT_DOWN;

	case eAttach_Hit_Point1:
		return ATTACH_POINT_HIT01;

	case eAttach_Hit_Point2:
		return ATTACH_POINT_HIT02;

	case eAttach_Back_Polearm:
		return ATTACH_POINT_BACK_STAFF;

	case eAttach_Back_Shield:
		return ATTACH_POINT_BACK_SHIELD;

	case eAttach_Back_Sword:
		return ATTACH_POINT_BACK_SWORD;

	case eAttach_Name_Title:
		return ATTACH_POINT_NAME_TITLE;

	case eAttach_Hand_Shield:
		return ATTACH_POINT_HAND_SHIELD;

	case eAttach_Left_Hand:
		return ATTACH_POINT_LEFT_HAND;

	case eAttach_Right_Hand:
		return ATTACH_POINT_RIGHT_HAND;

	case eAttach_Left_Foot:
		return ATTACH_POINT_LEFT_FOOT;

	case eAttach_Right_Foot:
		return ATTACH_POINT_RIGHT_FOOT;

	case eAttach_Left_Weapon:
		return ATTACH_POINT_LEFT_WEAPON;

	case eAttach_Right_Weapon:
		return ATTACH_POINT_RIGHT_WEAPON;

	case eAttach_Shoot:
		return ATTACH_POINT_SHOOT;

	case eAttach_Mouth:
		return ATTACH_POINT_MOUTH;

	case eAttach_Freeze_Down:
		return ATTACH_POINT_FREEZE_DOWN;

	case eAttach_Freeze_Hit:
		return ATTACH_POINT_FREEZE_HIT;

	case eAttach_Weapon_Effect01:
		return ATTACH_POINT_WEAPON_EFFECT01;

	case eAttach_Weapon_Effect02:
		return ATTACH_POINT_WEAPON_EFFECT02;

	case eAttach_Weapon_Effect03:
		return ATTACH_POINT_WEAPON_EFFECT03;
	}
	return "";
}

//-----------------------------------------------------------------------------
int	IBaseSprite::GetAttachPointEnum( const char* point )
{
	if ( point == NULL || strlen(point) < 1 )
		return eAttach_Null;

	if ( strcmp(point, ATTACH_POINT_TOP) == 0 )
		return eAttach_Top;
	else if ( strcmp(point, ATTACH_POINT_DOWN) == 0 )
		return eAttach_Down;
	else if ( strcmp(point, ATTACH_POINT_HIT01) == 0 )
		return eAttach_Hit_Point1;
	else if ( strcmp(point, ATTACH_POINT_HIT02) == 0 )
		return eAttach_Hit_Point2;
	else if ( strcmp(point, ATTACH_POINT_BACK_STAFF) == 0 )
		return eAttach_Back_Polearm;
	else if ( strcmp(point, ATTACH_POINT_BACK_SHIELD) == 0 )
		return eAttach_Back_Shield;
	else if ( strcmp(point, ATTACH_POINT_BACK_SWORD) == 0 )
		return eAttach_Back_Sword;
	else if ( strcmp(point, ATTACH_POINT_NAME_TITLE) == 0 )
		return eAttach_Name_Title;
	else if ( strcmp(point, ATTACH_POINT_HAND_SHIELD) == 0 )
		return eAttach_Hand_Shield;
	else if ( strcmp(point, ATTACH_POINT_LEFT_HAND) == 0 )
		return eAttach_Left_Hand;
	else if ( strcmp(point, ATTACH_POINT_RIGHT_HAND) == 0 )
		return eAttach_Right_Hand;
	else if ( strcmp(point, ATTACH_POINT_LEFT_FOOT) == 0 )
		return eAttach_Left_Foot;
	else if ( strcmp(point, ATTACH_POINT_RIGHT_FOOT) == 0 )
		return eAttach_Right_Foot;
	else if ( strcmp(point, ATTACH_POINT_LEFT_WEAPON) == 0 )
		return eAttach_Left_Weapon;
	else if ( strcmp(point, ATTACH_POINT_RIGHT_WEAPON) == 0 )
		return eAttach_Right_Weapon;
	else if ( strcmp(point, ATTACH_POINT_SHOOT) == 0 )
		return eAttach_Shoot;
	else if ( strcmp(point, ATTACH_POINT_MOUTH) == 0 )
		return eAttach_Mouth;
	else if ( strcmp(point, ATTACH_POINT_FREEZE_DOWN) == 0 )
		return eAttach_Freeze_Down;
	else if ( strcmp(point, ATTACH_POINT_FREEZE_HIT) == 0 )
		return eAttach_Freeze_Hit;
	else if ( strcmp(point, ATTACH_POINT_WEAPON_EFFECT01) == 0 )
		return eAttach_Weapon_Effect01;
	else if ( strcmp(point, ATTACH_POINT_WEAPON_EFFECT02) == 0 )
		return eAttach_Weapon_Effect02;
	else if ( strcmp(point, ATTACH_POINT_WEAPON_EFFECT03) == 0 )
		return eAttach_Weapon_Effect03;

	return eAttach_Null;
}

//-----------------------------------------------------------------------------
void IBaseSprite::SetRuEntityDirection( CRuEntity* pRuEntity, CRuVector3& dir, float entityScale )
{
	// 讓 Entity 在正確的方向
	if ( pRuEntity )
	{
		RuEntity_Orient( pRuEntity, CRuVector3( 0.0f, 0.0f, -1.0f), dir, entityScale );
	}
}

//-----------------------------------------------------------------------------
bool IBaseSprite::TerrainTest( CRuVector3& pt1, CRuVector3& pt2, CRuVector3 *newPos )
{
	CRuCollisionQuery_Segment segmentQuery;

	segmentQuery.m_colSegment.SetFromPoints( pt1, pt2 );

	// Ray-cast into terrain
	s_collisionContainer->QueryCollision(&segmentQuery);

	if(segmentQuery.m_colPoints->Count() > 0)
	{
		// Sort the results using insertion sort
		RuGlobalInsertionSort().Sort(segmentQuery.m_colDistances->GetArray(), segmentQuery.m_colDistances->Count());
		UINT32 *sortedIndices = RuGlobalInsertionSort().GetSortedIndices();

		// Store new y position
		if ( newPos )
			*newPos = segmentQuery.m_colPoints->Get(sortedIndices[0]);

		return true;
	}

	return false;
}
//-----------------------------------------------------------------------------
bool IBaseSprite::TerrainTestHight(CRuVector3& pos, float& y, float testRange)
{
	REAL h = RuEntity_FindAnchorPoint(pos, s_collisionContainer, 5.0f, 1.0f, testRange);

	if (h != -FLT_MAX)
	{
		y = h;
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
bool IBaseSprite::TestSphere( CRuVector3& pos, bool isJump )
{
	if(	RuCollision_TestSphere(CRuSphere(pos + CRuVector3(0.0f, 10.0f, 0.0f), 2.0f), s_collisionContainer) ||
		RuCollision_TestSphere(CRuSphere(pos + CRuVector3(0.0f, 16.0f, 0.0f), 2.0f), s_collisionContainer) )
	{
		return false;
	}

	if ( isJump && RuCollision_TestSphere(CRuSphere(pos + CRuVector3(0.0f, 1.0f, 0.0f), 1.0f), s_collisionContainer) )
		return false;

	return true;
}

//-----------------------------------------------------------------------------
bool IBaseSprite::ModifyPosOnTerrain( CRuVector3& pos )
{	
	CRuVector3 curPos = pos;
	float h = 4.0f;
	
	for ( int i = 0; i < 6; i++ )
	{
		float y;

		curPos.m_y += h;
		if ( TerrainTestHight( curPos, y, 50.0f ) )
		{
			pos.m_y = y;
			return true;
		}

		h += 4.0f;
	}
	return false;
}

//-----------------------------------------------------------------------------
void IBaseSprite::SetFontScale(float scale)
{
	s_fontScale = scale;
}

//-----------------------------------------------------------------------------
void IBaseSprite::SetBloodDisable(bool isDisable)
{
	s_isNoBlood = isDisable;
}

//-----------------------------------------------------------------------------
void IBaseSprite::ResetAllCollisionData()
{
	set<IBaseSprite*> &spriteContainer = IBaseSprite::GetContainer();
	for(set<IBaseSprite*>::iterator iter = spriteContainer.begin(); iter != spriteContainer.end(); iter++)
	{
		CRuFusion_Actor* pRuActor = NULL;

		switch((*iter)->GetObjectType())
		{
			default:
			case eSpriteObject_Unknown:
			case eSpriteObject_Effect:
				break;

			case eSpriteObject_Player:
				pRuActor = static_cast<CRoleSprite *>(*iter)->GetRuActor();
				if ( pRuActor )
					pRuActor->GetSojourner()->ResetCollisionData();
				break;

			case eSpriteObject_NPC:
			case eSpriteObject_QuestNPC:
				pRuActor = static_cast<CRoleSprite *>(*iter)->GetRuActor();
				if ( pRuActor )
					pRuActor->GetSojourner()->ResetCollisionData();
				break;
		}
	}
}

//-----------------------------------------------------------------------------
void IBaseSprite::Insert( IBaseSprite* pSprite )
{
	s_containers.insert( pSprite );

	if (s_procFunc)
		s_procFunc(pSprite, eSpriteMsg_Create, 0, 0);
}

//-----------------------------------------------------------------------------
void IBaseSprite::Erase(IBaseSprite* pSprite, bool force)
{
	for ( set<IBaseSprite*>::iterator iter = s_containers.begin(); iter != s_containers.end(); iter++ )
	{
		(*iter)->DetachSprite(pSprite);
		(*iter)->ReceiveDestruct(pSprite);
	}

	if ( s_procFunc )
		s_procFunc(pSprite, eSpriteMsg_Delete, 0, 0);	

	// Remove sprite from collision sprites list
	for(INT32 i = 0; i < s_colSprites.Count(); ++i)
	{
		if(s_colSprites[i] == pSprite)
		{
			s_colSprites.RemoveAt(i);
			break;
		}
	}

	// Remove sprite from plot collision sprites list
	for(INT32 i = 0; i < s_plotColSprites.Count(); ++i)
	{
		if(s_plotColSprites[i] == pSprite)
		{
			s_plotColSprites.RemoveAt(i);
			break;
		}
	}

	if ( pSprite )
	{
		// 解除物件之間連結(部份指標位置也需要清除)
		pSprite->SeverSpriteLinks();
	}

	// 刪除在容器內的指標
	s_containers.erase(pSprite);
}

/*
//-----------------------------------------------------------------------------
IBaseSprite::IBaseSprite()
{
	IBaseSprite(true)	
}
*/

//-----------------------------------------------------------------------------
IBaseSprite::IBaseSprite(bool isAddToContainer)
{
	m_classType			= eSpriteClass_Base;

	m_pRuEntity			= NULL;
	m_pRuTempEntity		= NULL;
	m_pRuFloorEntity	= NULL;
	m_pRuDecalShadow	= ruNEW CRuEntity_DecalShadow();
	m_objectId			= 0;
	m_objectType		= eSpriteObject_Unknown;

	m_isVisible			= true;
	m_isImplementVisible	= true;
	m_enableCollision	= false;
	m_enablePlotColTrigger = false;
	m_id				= 0;
//	m_shadowType		= eSpriteShadowType_None;
	m_shadowType		= eSpriteShadowType_Projected;

	m_nowPos.Set( 0.0f, 0.0f, 0.0f );
	m_direction.Set( 0.0f, 0.0f, -1.0f );

	m_moveSpeed			= 1.0f;

	m_orientingAxis.Set(0.0f, 1.0f, 0.0f);
	m_targetOrientingAxis.Set(0.0f, 1.0f, 0.0f);
	m_rebaseDirection = false;
	m_orientToSurfaceNormal = false;
	m_orientToSurfaceForword = false;

	m_isDestruct		= false;
	m_destructTime		= 0.0f;
	m_pauseTime			= 0.0f;

	m_scale				= 1.0f;
	m_appnedScale		= 1.0f;

	m_linkParent		= NULL;
	m_linkControls		= false;

	m_parentMount		= NULL;
	m_lastParentMount	= NULL;

	m_linkShadow		= false;
	m_proxyLoadCompleted	= false;
	m_dyeFXMaskRequiresLoad = false;

	m_disableUpdateEntity = false;
	m_aboveWater		= false;
	m_isHorse			= false;
	m_isServerMove		= true;

	if ( isAddToContainer )
		Insert( this );	
}

//-----------------------------------------------------------------------------
IBaseSprite::~IBaseSprite()
{
	ruSAFE_RELEASE(m_pRuDecalShadow);

	// 刪除所有附加物件(一般都為特效物件)
	for ( map<IBaseSprite*, int>::iterator iter = m_attachSprite.begin(); iter != m_attachSprite.end(); iter++ )
	{
		CRuEntity* entity = iter->first->GetRuEntity();

		if ( entity )
		{
			entity->DetachFromParent();
		}
	}	
	m_attachSprite.clear();

	// 清除關連指標
	for ( set<IBaseSprite*>::iterator iter = s_containers.begin(); iter != s_containers.end(); iter++ )
	{
		if ( (*iter)->m_parentMount == this )
			(*iter)->m_parentMount = NULL;
	}
	
	Erase(this);

	// 解除物件之間連結(部份指標位置也需要清除)
	// SeverSpriteLinks();

	// 清除暫存物件
	ReleaseTempEntity();

	// 清除地板物件
	ReleaseFloorEntity();

	ReleaseEntity();

	// Release compasses
	for(INT32 i = 0; i < m_compassList.Count(); ++i)
	{
		ruSAFE_RELEASE(m_compassList[i].m_compassEntity);
	}
}

//-----------------------------------------------------------------------------
BOOL IBaseSprite::HandleProxyLoadComplete(RuEventArgs *eventArgs)
{
	// Update caster class flags
	SetCasterClass();

	if(m_pRuEntity)
	{		
		// Play Idle Motion
		// PlayMotion("stand_idle01");	
		PlayMotion("idle");			

		// Play persistent motion
		RuACT_PlayPersistentMotion(m_pRuEntity, FALSE);

		// Enable alpha-Z
		RuEntity_EnableAlphaZ(m_pRuEntity);
	}

	m_proxyLoadCompleted = true;

	return TRUE;
}

//-----------------------------------------------------------------------------
void IBaseSprite::Update( float elapsedTime )
{
/*
	// DEBUG CODE
	if(m_pRuEntity)
	{
		CRuEntity *paperdoll = RuEntity_FindFirstPaperdoll(m_pRuEntity);

		if(s_pRuTerrain && paperdoll && paperdoll->GetType() == CRuPaperdoll::Type)
		{
			// Only do orientation if we have moved more than a specific distance threshold
			CRuVector3 curPos = m_pRuEntity->GetTranslation(ruFRAMETYPE_LOCAL);
			if(m_lastOrientingAxisCheckPos.m_x == 0.0f || (curPos - m_lastOrientingAxisCheckPos).Magnitude() > 0.5f)
			{
				CRuVector3 restingNormal = RuEntity_FindRestingNormal(curPos, s_pRuTerrain);

				m_lastOrientingAxisCheckPos = curPos;
				this->SetOrientingAxis(restingNormal);
			}
		}
	}
*/

	if ( m_disableUpdateEntity && m_proxyLoadCompleted )
		return;

	// If orient to surface normal is enabled, update the orienting axis
	if (m_pRuEntity)
	{
		if (m_orientToSurfaceForword)
		{
			CRuVector3 curPos = m_pRuEntity->GetTranslation(ruFRAMETYPE_LOCAL);
			if(m_lastOrientingAxisCheckPos.m_x == 0.0f || (curPos - m_lastOrientingAxisCheckPos).Magnitude() > 5.0f)
			{
				CRuVector3 position[2];
				position[0] = curPos - (m_direction * 1.2f);
				position[1] = curPos + (m_direction * 1.5f);
				if ( TerrainTestHight(position[0], position[0].m_y, 10) && TerrainTestHight(position[1], position[1].m_y, 10) )
				{
					CRuVector3 nextDirection = position[0] - position[1];
					if ( nextDirection.Magnitude() > 0.0001f ) 
					{
						CRuVector3 rightVector = CrossProduct(nextDirection, CRuVector3(0.0f, 1.0f, 0.0f));
						CRuVector3 upVector;
						if( rightVector.Magnitude() > ruEPSILON )
						{
							rightVector.Normalize();
							upVector = CrossProduct(rightVector, nextDirection);
							if ( upVector.Magnitude() > ruEPSILON )
							{
								upVector.Normalize();
								SetOrientingAxis(upVector);
							}
						}
					}
				}
				

				/*
				CRuVector3 nextDirection;
				float y;
				float nearDot = 0.2f;
				for ( int i = 0; i < 5; i++ )
				{
					CRuVector3 position = curPos + (m_direction * 1.2f);
					if ( TerrainTestHight(position, y, 10) )
					{
						position.m_y = y;
						nextDirection = curPos - position;
						if ( nextDirection.Magnitude() > 0.0001f )
						{
							float tha =	m_direction.Dot(nextDirection.Normalize());
							if ( tha > nearDot )
							{
								nearDot = tha;
							}
						}
					}
				}

				if ( nearDot > 0.2f )
				{
					CRuVector3 rightVector = CrossProduct(nextDirection, CRuVector3(0.0f, 1.0f, 0.0f));
					CRuVector3 upVector;
					if( rightVector.Magnitude() > ruEPSILON )
					{
						rightVector.Normalize();

						upVector = CrossProduct(rightVector, nextDirection);
						if ( upVector.Magnitude() > ruEPSILON )
						{
							if ( m_objectId == 102760 ) {
								g_pError->AddMessage(0, 0, "SetOrientingAxis (%.2f,%.2f,%.2f) upVector(%.2f,%.2f,%.2f)",
									m_orientingAxis.m_x, m_orientingAxis.m_y, m_orientingAxis.m_z,
									upVector.m_x, upVector.m_y, upVector.m_z );
							}

							upVector.Normalize();
							this->SetOrientingAxis(upVector);
						}
					}
				}
				*/

				m_lastOrientingAxisCheckPos = curPos;
			}
		}
		else if (m_orientToSurfaceNormal)
		{
			// Only do orientation if we have moved more than a specific distance threshold
			CRuVector3 curPos = m_pRuEntity->GetTranslation(ruFRAMETYPE_LOCAL);
			if(m_lastOrientingAxisCheckPos.m_x == 0.0f || (curPos - m_lastOrientingAxisCheckPos).Magnitude() > 0.5f)
			{
				CRuVector3 restingNormal = RuEntity_FindRestingNormal(curPos, s_collisionContainer);

				m_lastOrientingAxisCheckPos = curPos;
				this->SetOrientingAxis(restingNormal);
			}
		}
	}
	
	// Interpolate orienting axis movement arc
	REAL w1 = min(1.0f, elapsedTime * 20.0f);
	REAL w0 = 1.0f - w1;
	m_orientingAxis = m_orientingAxis * w0 + m_targetOrientingAxis * w1;	

	// If decal shadow is enabled, check that the decal shadow is initialized
	if (m_linkParent && m_linkShadow)
	{
		// Remove self from the caster list of the locally owned decal shadow object
		m_pRuDecalShadow->RemoveCaster(m_pRuEntity);
		m_pRuDecalShadow->SetReceiver(NULL);
		
		// Insert self into caster list of the link parent's decal shadow object
		if ( m_linkParent->m_pRuDecalShadow )
			m_linkParent->m_pRuDecalShadow->AddCaster(m_pRuEntity, FALSE);
	}
	else
	{
		CRuEntity* ruEntity = GetCurrentEntity();
		if (ruEntity && s_pRuTerrain)
		{
			m_pRuDecalShadow->AddCaster(ruEntity, TRUE);
			m_pRuDecalShadow->SetReceiver(this->GetCollisionContainer());

			REAL dotShadowSize = min(30.0f, ruEntity->GetLocalBounds().Radius() / 15.0f * 5.0f);
			m_pRuDecalShadow->SetDotShadowSize(dotShadowSize);
		}
	}

	m_pRuDecalShadow->SetShadowMode(static_cast<RuDecalShadowMode>(ruDECALSHADOWMODE_NONE + m_shadowType));	

	// Floor Entity
	if (m_pRuFloorEntity)
	{
		FindFloorParentEntity();
		// m_pRuFloorEntity->Update(elapsedTime);
	}

	// Update temporary entity
	if (m_pRuTempEntity)
		m_pRuTempEntity->Update(elapsedTime * 1.0f);

	// Update primary entity
	if (m_pRuEntity )
	{
		// Update entity only if it is not attached to some other entity
		if ( m_pRuEntity->GetParent() == NULL )
			m_pRuEntity->Update(elapsedTime * 1.0f);
	}

	// Update decal shadow
	if (m_pRuDecalShadow)
	{
		m_pRuDecalShadow->Update(elapsedTime * 1.0f);
	}

	// Update compass entities
	for(INT32 i = 0; i < m_compassList.Count(); ++i)
	{
		if(m_compassList[i].m_compassEntity)
		{
			m_compassList[i].m_compassEntity->Update(elapsedTime);
		}
	}
}

//-----------------------------------------------------------------------------
void IBaseSprite::Render()
{
	if (m_pRuTempEntity)
		g_ruDefaultRenderContext->Submit(m_pRuTempEntity);
	else if (m_pRuEntity)
		g_ruDefaultRenderContext->Submit(m_pRuEntity);
	if(m_pRuDecalShadow)
		g_ruDefaultRenderContext->Submit(m_pRuDecalShadow);

	// Render compasses
	for(INT32 i = 0; i < m_compassList.Count(); ++i)
	{
		if(m_compassList[i].m_compassEntity && m_compassList[i].m_compassEntity->GetOption(ruENTITYOPTION_HIDDEN) == FALSE)
		{
			// Reset compass transform
			m_compassList[i].m_compassEntity->ResetAllTransforms();

			// Calculate direction to target
			CRuVector3 dirVec = m_compassList[i].m_compassTarget - this->GetPos();
			if(dirVec.Magnitude() > ruEPSILON)
			{
				dirVec.Normalize();
			}
			else
			{
				dirVec.Set(1.0f, 0.0f, 0.0f);
			}

			// Perform initial orientation
			CRuQuaternion rotQuat;
			rotQuat.RotateIntoVector(dirVec, CRuVector3(0.0f, 1.0f, 0.0f));
			m_compassList[i].m_compassEntity->Rotate(rotQuat);

			// Align arrow up axis with world up while constraining rotation to arrow forward axis
			CRuVector3 forwardVec(0.0f, 1.0f, 0.0f);
			rotQuat.TransformVector(forwardVec);

			CRuVector3 upDir(0.0f, 0.0f, 1.0f);
			rotQuat.TransformVector(upDir);

			CRuQuaternion fixRotQuat;
			fixRotQuat.RotateIntoVector(CRuVector3(0.0f, 1.0f, 0.0f), upDir, forwardVec);

			m_compassList[i].m_compassEntity->Rotate(fixRotQuat);

			// Offset compass to a position around the sprite center
			CRuVector3 posOffset = this->GetPos() + dirVec * 10.0f + CRuVector3(0.0f, 10.0f, 0.0f);
			m_compassList[i].m_compassEntity->Translate(posOffset);

			// Submit for rendering
			g_ruDefaultRenderContext->Submit(m_compassList[i].m_compassEntity);
		}
	}
}

//-----------------------------------------------------------------------------
void IBaseSprite::SetVisible(bool show)
{
	m_isVisible = show;
}

//-----------------------------------------------------------------------------
bool IBaseSprite::CheckRenderState()
{
	return (m_isVisible && m_isImplementVisible) || s_gm_ShowHidden;
}

//-----------------------------------------------------------------------------
bool IBaseSprite::SetObjectId( int id, bool deferredLoad )
{
	GameObjDbStruct* pObj = NULL;

	if ( m_objectId == id )
		return false;	

	// Get object data
	if ( (pObj = CNetGlobal::GetObj( id )) == NULL )
		return false;

	// Get Image object data
	GameObjDbStruct* imageDB = CNetGlobal::GetObj(pObj->ImageID);
	if ( imageDB == NULL )
		return false;	

	// Set sprite type and type-related options
	switch ( pObj->Classification )
	{
	case EM_ObjectClass_Player:
		SetObjectType(eSpriteObject_Player);
		break;

	case EM_ObjectClass_NPC:
		SetObjectType(eSpriteObject_NPC);
		break;

	case EM_ObjectClass_QuestNPC:
		SetObjectType(eSpriteObject_QuestNPC);
		break;

	case EM_ObjectClass_Mine:
		SetObjectType(eSpriteObject_QuestNPC);
		break;

	case EM_ObjectClass_MagicBase:
	case EM_ObjectClass_MagicCollect:
		if(m_pRuEntity)
		{
			m_pRuEntity->SetOption(ruENTITYOPTION_NOMOTIONPROPAGATE, TRUE);
		}
		SetObjectType(eSpriteObject_Effect);
		PlayMotion("idle");
		break;
	}

	// Set object scale
	SetScale(imageDB->Image.ModelSize / 100.0f, eSpriteScale_World);

	// Set object alpha
	SetAlpha(imageDB->Image.Alpha / 255.0f);

	// Load entity
	if ( !LoadRuEntity(imageDB->Image.ACTWorld, deferredLoad ) )
		return false;

	// Play Idle Motion
	PlayMotion("idle");	

	// Store object ID
	m_objectId = id;

	return true;
}

//-----------------------------------------------------------------------------
void IBaseSprite::SetCasterClass()
{
	if(m_pRuEntity)
	{
		switch(GetObjectType())
		{
			case eSpriteObject_Player:
			case eSpriteObject_NPC:
			case eSpriteObject_QuestNPC:
				RuEntity_SetOptionWithTypeFilter(m_pRuEntity, ruENTITYOPTION_CASTERCLASS, 1, CRuModel::Type());
				RuEntity_SetOptionWithTypeFilter(m_pRuEntity, ruENTITYOPTION_CASTERCLASS, 1, CRuCompositeModel::Type());
				break;
		}
	}

	if(m_pRuTempEntity)
	{
		switch(GetObjectType())
		{
			case eSpriteObject_Player:
			case eSpriteObject_NPC:
			case eSpriteObject_QuestNPC:
				RuEntity_SetOptionWithTypeFilter(m_pRuTempEntity, ruENTITYOPTION_CASTERCLASS, 1, CRuModel::Type());
				RuEntity_SetOptionWithTypeFilter(m_pRuTempEntity, ruENTITYOPTION_CASTERCLASS, 1, CRuCompositeModel::Type());
				break;
		}
	}
}

//-----------------------------------------------------------------------------
bool IBaseSprite::LoadRuEntity(const char* actName, bool deferredLoad, float scale)
{
	if ( actName == NULL || actName[0] == NULL )
		return false;
	
	char tempString[512];
	if ( s_isNoBlood )
	{
		strcpy(tempString, actName);
		strlwr(tempString);

		char *ext = strstr(tempString, ".ros");
		if ( ext != NULL )
		{
			*ext = 0;
		}

		strcat(tempString, "_noblood.ros");

		if ( g_ruResourceManager->FileExists(tempString) )
		{
			actName = tempString;
		}
	}

	ReleaseEntity();

	if(deferredLoad)
	{
		// Mark proxy as not having completed load
		m_proxyLoadCompleted = false;

		// Create entity as a proxy entity and register a listener for load completion
		m_pRuEntity = ruNEW CRuEntity_Proxy();
		static_cast<CRuEntity_Proxy *>(m_pRuEntity)->Event_LoadComplete().RegisterHandler(m_pRuEntity->GetGUID(), boost::bind(&IBaseSprite::HandleProxyLoadComplete, this, _1));

		// Create paperdoll placeholder
		CRuPaperdoll_Placeholder *paperdollPlaceholder = ruNEW CRuPaperdoll_Placeholder();
		m_pRuEntity->AddChild(paperdollPlaceholder);
		ruSAFE_RELEASE(paperdollPlaceholder);

		// Create link frames
		CRuLinkFrame *linkFrame;

		linkFrame = ruNEW CRuLinkFrame();
		linkFrame->SetLinkName("p_down");
		m_pRuEntity->AddChild(linkFrame);
		ruSAFE_RELEASE(linkFrame);

		linkFrame = ruNEW CRuLinkFrame();
		linkFrame->SetLinkName("p_shoot");
		m_pRuEntity->AddChild(linkFrame);
		ruSAFE_RELEASE(linkFrame);

		linkFrame = ruNEW CRuLinkFrame();
		linkFrame->SetLinkName("p_name_title");
		m_pRuEntity->AddChild(linkFrame);
		ruSAFE_RELEASE(linkFrame);

		linkFrame = ruNEW CRuLinkFrame();
		linkFrame->SetLinkName("p_back_gun");
		m_pRuEntity->AddChild(linkFrame);
		ruSAFE_RELEASE(linkFrame);

		linkFrame = ruNEW CRuLinkFrame();
		linkFrame->SetLinkName("p_back_bow");
		m_pRuEntity->AddChild(linkFrame);
		ruSAFE_RELEASE(linkFrame);

		linkFrame = ruNEW CRuLinkFrame();
		linkFrame->SetLinkName("p_back_staff");
		m_pRuEntity->AddChild(linkFrame);
		ruSAFE_RELEASE(linkFrame);

		linkFrame = ruNEW CRuLinkFrame();
		linkFrame->SetLinkName("p_hit_point_01");
		m_pRuEntity->AddChild(linkFrame);
		ruSAFE_RELEASE(linkFrame);

		linkFrame = ruNEW CRuLinkFrame();
		linkFrame->SetLinkName("p_hit_point_02");
		m_pRuEntity->AddChild(linkFrame);
		ruSAFE_RELEASE(linkFrame);

		linkFrame = ruNEW CRuLinkFrame();
		linkFrame->SetLinkName("p_back_shield");
		m_pRuEntity->AddChild(linkFrame);
		ruSAFE_RELEASE(linkFrame);

		linkFrame = ruNEW CRuLinkFrame();
		linkFrame->SetLinkName("p_back_sword");
		m_pRuEntity->AddChild(linkFrame);
		ruSAFE_RELEASE(linkFrame);

		linkFrame = ruNEW CRuLinkFrame();
		linkFrame->SetLinkName("p_right_weapon");
		m_pRuEntity->AddChild(linkFrame);
		ruSAFE_RELEASE(linkFrame);

		linkFrame = ruNEW CRuLinkFrame();
		linkFrame->SetLinkName("p_left_weapon");
		m_pRuEntity->AddChild(linkFrame);
		ruSAFE_RELEASE(linkFrame);

		linkFrame = ruNEW CRuLinkFrame();
		linkFrame->SetLinkName("p_mouth");
		m_pRuEntity->AddChild(linkFrame);
		ruSAFE_RELEASE(linkFrame);

		linkFrame = ruNEW CRuLinkFrame();
		linkFrame->SetLinkName("p_top");
		m_pRuEntity->AddChild(linkFrame);
		ruSAFE_RELEASE(linkFrame);

		linkFrame = ruNEW CRuLinkFrame();
		linkFrame->SetLinkName("p_left_foot");
		m_pRuEntity->AddChild(linkFrame);
		ruSAFE_RELEASE(linkFrame);

		linkFrame = ruNEW CRuLinkFrame();
		linkFrame->SetLinkName("p_right_foot");
		m_pRuEntity->AddChild(linkFrame);
		ruSAFE_RELEASE(linkFrame);

		linkFrame = ruNEW CRuLinkFrame();
		linkFrame->SetLinkName("p_hand_shield");
		m_pRuEntity->AddChild(linkFrame);
		ruSAFE_RELEASE(linkFrame);

		linkFrame = ruNEW CRuLinkFrame();
		linkFrame->SetLinkName("p_right_hand");
		m_pRuEntity->AddChild(linkFrame);
		ruSAFE_RELEASE(linkFrame);

		linkFrame = ruNEW CRuLinkFrame();
		linkFrame->SetLinkName("p_left_hand");
		m_pRuEntity->AddChild(linkFrame);
		ruSAFE_RELEASE(linkFrame);

		// Perform entity load by proxy
		static_cast<CRuEntity_Proxy *>(m_pRuEntity)->LoadEntity(actName);

		// 設定初始面向
		SetDirection(CRuVector3(0.0f, 0.0f, -1.0f), true);

		// 註冊事件處理器
		s_pRuEventListener->RegisterEventHandlers(m_pRuEntity, reinterpret_cast<PTRVALUE>(this));

		return true;
	}
	else
	{
		if ( m_pRuEntity = g_ruResourceManager->LoadEntity(actName) )
		{
			// Manually call proxy load complete
			HandleProxyLoadComplete(NULL);

			// Scale
			SetScale(scale);

			// 設定初始面向
			SetDirection(CRuVector3(0.0f, 0.0f, -1.0f), true);

			// 註冊事件處理器
			s_pRuEventListener->RegisterEventHandlers(m_pRuEntity, reinterpret_cast<PTRVALUE>(this));

			// Play persistent motion
			RuACT_PlayPersistentMotion(m_pRuEntity, FALSE);

			return true;
		}
	}

	return false;
}

//-----------------------------------------------------------------------------
bool IBaseSprite::LoadRuTempEntity(const char* actName)
{
	ReleaseTempEntity();
	if ( m_pRuTempEntity = g_ruResourceManager->LoadEntity(actName) )
	{
		// Setup caster class
		SetCasterClass();

		// 移除上次影子資料
		if ( m_pRuDecalShadow )
		{
			m_pRuDecalShadow->RemoveCaster(m_pRuEntity);
			//m_pRuDecalShadow->AddCaster(m_pRuTempEntity);
		}

		// 設定目前座標值
		m_pRuTempEntity->SetTranslation(m_nowPos);

		// 設定初始面向
		SetRuEntityDirection(m_pRuTempEntity, m_direction, m_scale);

		// 註冊事件處理器
		s_pRuEventListener->RegisterEventHandlers(m_pRuTempEntity, reinterpret_cast<PTRVALUE>(this));	

		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
bool IBaseSprite::CreateFloorEntity(const char* actName)
{
	ReleaseFloorEntity();	

	if ( m_pRuFloorEntity = g_ruResourceManager->LoadEntity(actName) )
	{
		RuACT_PlayMotionEx(m_pRuFloorEntity, -1, "idle", 0.1f, false, 1.0f, 0.0f, 0.0f, GetMoveSpeed());

		FindFloorParentEntity();

		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
void IBaseSprite::ReleaseFloorEntity()
{
	if (m_pRuFloorEntity)
	{
		// 自父元件移除
		m_pRuFloorEntity->DetachFromParent();

		// 釋放自己
		ruSAFE_RELEASE(m_pRuFloorEntity);
	}
}

//-----------------------------------------------------------------------------
void IBaseSprite::FindFloorParentEntity()
{
	if ( m_pRuFloorEntity == NULL )
		return;

	IBaseSprite* sprite = this;
	while ( sprite->GetLinkParent() )
		sprite = sprite->GetLinkParent();

	CRuEntity* pRuEntity = (sprite->GetRuTempEntity()) ? sprite->GetRuTempEntity() : sprite->GetRuEntity();
	if ( pRuEntity && m_pRuFloorEntity->GetParent() != pRuEntity ) {
		RuEntity_Attach_BFS( pRuEntity, m_pRuFloorEntity, ATTACH_POINT_DOWN );
	}
}

//-----------------------------------------------------------------------------
void IBaseSprite::MovePos( CRuVector3 &pos )
{
	m_nowPos = m_nowPos + pos;
	if ( m_pRuEntity )
		m_pRuEntity->SetTranslation( m_nowPos );
}

//-----------------------------------------------------------------------------
void IBaseSprite::SetPos( CRuVector3 &pos )
{
	m_nowPos = pos;
	if ( m_pRuEntity )
		m_pRuEntity->SetTranslation( pos );
	if ( m_pRuTempEntity )
		m_pRuTempEntity->SetTranslation( pos );	
}

//-----------------------------------------------------------------------------
void IBaseSprite::SetDirection( CRuVector3 direction, bool IsForce, bool IsInitial )
{
	if ( IsSame(direction.m_x, 0.0f) && IsSame(direction.m_z, 0.0f) )
		return;
	
	direction.m_y = 0.0f;
	if ( m_direction != direction.Normalize() || IsForce )
	{
		m_direction = direction;
		SetRuEntityDirection(m_pRuEntity, m_direction, m_scale);
	}
}

//-----------------------------------------------------------------------------
void IBaseSprite::SetOrientingAxis(const CRuVector3 &vec, bool force)
{
	m_targetOrientingAxis = vec;
	if ( m_targetOrientingAxis.Magnitude() > 0.00001f )
		m_targetOrientingAxis.Normalize();

	if(force)
	{
		m_orientingAxis = m_targetOrientingAxis;
	}
}

//-----------------------------------------------------------------------------
void IBaseSprite::UpdatePositionDirectionFromEntity()
{
	// Update position
	m_nowPos = m_pRuEntity->GetTranslation();

	// Update direction
	CRuVector3 dir(0.0f, 0.0f, -1.0f);
	m_pRuEntity->GetWorldTransform().TransformVector(dir);
	dir.Normalize();

	// Apply direction change
	SetDirection(dir);
}

//-----------------------------------------------------------------------------
void IBaseSprite::UpdateOrientation()
{
}

//-----------------------------------------------------------------------------
void IBaseSprite::SetOrientMode(CRuFusion_Actor::OrientMode orientMode)
{
}

//-----------------------------------------------------------------------------
void IBaseSprite::SetScale(float scale, SpriteScaleType type)
{
	switch (type)
	{
	case eSpriteScale_Local:
		scale *= m_scale;
		break;

	case eSpriteScale_World:
		break;
	}	

	if (m_scale != scale)
	{
		m_scale = scale;
		UpdateScale();
	}
}

//-----------------------------------------------------------------------------
void IBaseSprite::AppnedScale(float scale)
{
	if ( m_appnedScale != scale )
	{
		m_appnedScale = scale;
		UpdateScale();
	}
}

//-----------------------------------------------------------------------------
void IBaseSprite::UpdateScale()
{
	if ( m_pRuEntity )
	{
		m_pRuEntity->ResetAllTransforms();
		m_pRuEntity->Scale(m_scale * m_appnedScale, ruTRANSFORMOP_PRECONCAT);
		m_pRuEntity->SetTranslation(m_nowPos);
	}
}

//-----------------------------------------------------------------------------
float IBaseSprite::GetEntityWorldScale()
{	
	if ( m_pRuEntity == NULL )
		return 1.0f;

	return m_pRuEntity->GetScale(ruFRAMETYPE_WORLD);
}

//-----------------------------------------------------------------------------
void IBaseSprite::ClearDestruct()
{
	m_isDestruct = false;
	m_destructTime = 0.0f;
}

//-----------------------------------------------------------------------------
void IBaseSprite::SetDestructTime( float time )
{
	m_isDestruct = true;
	m_destructTime = time;
}

//-----------------------------------------------------------------------------
CRuEntity* IBaseSprite::FindLinkEntity( int point )
{
	if ( point != eAttach_Null )
	{
		if ( m_pRuTempEntity )
			return RuEntity_FindLinkFrame( m_pRuTempEntity, GetAttachPointName(point), TRUE );
		else if ( m_pRuEntity ) 
			return RuEntity_FindLinkFrame( m_pRuEntity, GetAttachPointName(point), TRUE );
	}
	return NULL;
}

//-----------------------------------------------------------------------------
bool IBaseSprite::GetAttachPos(int point, CRuVector3& pos)
{
	CRuEntity* pRuEntity1 = NULL;
	CRuEntity* pRuEntity2 = NULL;
	pos = m_nowPos;

	switch (point)
	{
	case eAttach_Null:
	case eAttach_Freeze_Down:
		break;

	case eAttach_Freeze_Hit:
		pRuEntity1 = FindLinkEntity(eAttach_Hit_Point1);
		pRuEntity2 = FindLinkEntity(eAttach_Hit_Point2);
		if ( pRuEntity1 && pRuEntity2 )
		{
			CRuVector3 hit1Point, hit2Point;
			pRuEntity1->GetWorldTransform().TransformPoint(hit1Point);
			pRuEntity2->GetWorldTransform().TransformPoint(hit2Point);
			pos.m_x = (hit1Point.m_x + hit2Point.m_x) / 2.0f;
			pos.m_y = (hit1Point.m_y + hit2Point.m_y) / 2.0f;
			pos.m_z = (hit1Point.m_z + hit2Point.m_z) / 2.0f;
		}
		break;

	default:
		if ( (pRuEntity1 = FindLinkEntity(point)) != NULL )
		{
			pos.Set(0.0f, 0.0f, 0.0f);
			pRuEntity1->GetWorldTransform().TransformPoint(pos);
		}
		break;

	}
	return true;
}

//-----------------------------------------------------------------------------
CRuVector3 IBaseSprite::GetAttachPos( const char *attachPointName )
{
	CRuVector3 attachPos(this->GetPos());

	CRuEntity* ruEntity = GetCurrentEntity();
	if(ruEntity && attachPointName)
	{
		// Find attachment link frame
		CRuEntity *attachmentFrame = RuEntity_FindLinkFrame(ruEntity, attachPointName, TRUE);

		if(attachmentFrame)
		{
			attachPos = attachmentFrame->GetTranslation(ruFRAMETYPE_WORLD);
		}
	}

	return attachPos;
}

//-----------------------------------------------------------------------------
CRuVector3 IBaseSprite::GetAttachDir( const char *attachPointName )
{
	CRuVector3 attachDir(0.0f, 1.0f, 0.0f);

	if(m_pRuEntity)
	{
		// Find attachment link frame
		CRuEntity *attachmentFrame = RuEntity_FindLinkFrame(m_pRuEntity, attachPointName, TRUE);

		if(attachmentFrame)
		{
			CRuQuaternion attachRotQuat = attachmentFrame->GetRotation(ruFRAMETYPE_WORLD);
			attachRotQuat.TransformVector(attachDir);
		}
	}

	return attachDir;
}

//-----------------------------------------------------------------------------
void IBaseSprite::ReleaseEntity()
{
	// 清除影子關連
	if ( m_linkParent && m_linkShadow )
	{
		if(m_linkParent->m_pRuDecalShadow)
		{
			m_linkParent->m_pRuDecalShadow->RemoveCaster(m_pRuEntity);
		}
	}

	if ( m_pRuEntity )
	{
		if(m_pRuDecalShadow)
		{
			m_pRuDecalShadow->RemoveAllCasters();
		}

		// s_pRuEventListener->RemoveEntity( m_pRuEntity );
		m_pRuEntity->Release();
		m_pRuEntity = NULL;
		m_objectId = 0;
	}
}

//-----------------------------------------------------------------------------
void IBaseSprite::ReleaseTempEntity()
{
	if ( m_pRuTempEntity )
		m_pRuDecalShadow->RemoveCaster(m_pRuTempEntity);

	ruSAFE_RELEASE(m_pRuTempEntity);
}

//-----------------------------------------------------------------------------
bool IBaseSprite::PlayMotion(const char* motionStr, CRuACT_ActionEvents *actionEventsOut)
{
	if ( m_pRuEntity == NULL )
		return false;

	if ( !RuACT_HasMotion(m_pRuEntity, motionStr) )
		return false;

	// Play persistent motion
	RuACT_PlayPersistentMotion(m_pRuEntity, FALSE);

	if ( !RuACT_PlayMotionEx(m_pRuEntity, -1, motionStr, 0.1f, false, 1.0f, 0.0f, 0.0f, GetMoveSpeed(), actionEventsOut) )
		return false;

	return true;
}

//-----------------------------------------------------------------------------
bool IBaseSprite::HasMotion(const char* motionStr)
{
	return ( RuACT_HasMotion(m_pRuEntity, motionStr) ) ? true : false;
}

//-----------------------------------------------------------------------------
void IBaseSprite::EnableCelHighlight(RUCOLOR color)
{
	if(m_pRuEntity)
	{
		RuEntity_EnableCelHighlight(m_pRuEntity, color);
	}

	if(m_pRuTempEntity)
	{
		RuEntity_EnableCelHighlight(m_pRuTempEntity, color);
	}
}

//-----------------------------------------------------------------------------
void IBaseSprite::DisableCelHighlight()
{
	if(m_pRuEntity)
	{
		RuEntity_DisableCelHighlight(m_pRuEntity);
	}

	if(m_pRuTempEntity)
	{
		RuEntity_DisableCelHighlight(m_pRuTempEntity);
	}
}

//-----------------------------------------------------------------------------
bool IBaseSprite::AnchorSprite(float testLimit)
{
	CRuVector3 spritePosition = this->GetPos();

	spritePosition.m_y = RuEntity_FindRestingHeight(spritePosition, s_collisionContainer, testLimit);

	this->SetPos(spritePosition);

	return true;
}

//-----------------------------------------------------------------------------
void IBaseSprite::LinkSprite(IBaseSprite *linkedSprite, const char *attachPointName, bool linkControls, bool linkShadow)
{
	// Unlink any previous links
	if(linkedSprite->m_linkParent)
	{
		linkedSprite->m_linkParent->UnlinkSprite(linkedSprite);
	}

	// Establish sprite link
	linkedSprite->m_linkParent = this;
	linkedSprite->m_linkAttachPointName = attachPointName;
	linkedSprite->m_linkControls = linkControls;
	linkedSprite->m_linkShadow = linkShadow;
	m_linkedSprites.Add(linkedSprite);
	
	linkedSprite->m_linkParentLastPosition = linkedSprite->m_linkAttachPointName ? this->GetAttachPos(linkedSprite->m_linkAttachPointName) :this->GetPos();	

	// If a link attachment point was specified, move the linked sprite to the attachment point position
	if(linkedSprite->m_linkAttachPointName)
	{
		linkedSprite->SetPos(this->GetAttachPos(linkedSprite->m_linkAttachPointName));
	}
}

//-----------------------------------------------------------------------------
void IBaseSprite::UnlinkSprite(IBaseSprite *linkedSprite)
{
	// Remove sprite from linked sprite list
	for(INT32 i = 0; i < m_linkedSprites.Count(); ++i)
	{
		if(m_linkedSprites[i] == linkedSprite)
		{
			if ( linkedSprite->GetRuEntity() )
				linkedSprite->GetRuEntity()->DetachFromParent();

			if(m_pRuDecalShadow)
			{
				// Clean up shadow links
				if(linkedSprite->m_linkShadow)
				{
					m_pRuDecalShadow->RemoveCaster(linkedSprite->GetRuEntity());
				}
			}

			m_linkedSprites.RemoveAt(i);
			break;
		}
	}

	// Reset link parent
	linkedSprite->m_linkParent = NULL;
	linkedSprite->m_linkAttachPointName = NULL;
	linkedSprite->m_linkControls = false;
	linkedSprite->m_linkShadow = false;
}

//-----------------------------------------------------------------------------
void IBaseSprite::UnlinkChildSprite()
{
	if ( GetRuEntity() )
	{
		GetRuEntity()->DetachFromParent();
	}

	m_linkParent = NULL;
	m_linkAttachPointName = NULL;
	m_linkControls = false;
	m_linkShadow = false;
}

//-----------------------------------------------------------------------------
void IBaseSprite::SeverSpriteLinks()
{
	// Unlink children
	for(INT32 i = 0; i < m_linkedSprites.Count(); ++i)
	{
		if(m_pRuDecalShadow)
		{
			// Clean up shadow links
			if(m_linkedSprites[i]->m_linkShadow)
			{
				m_pRuDecalShadow->RemoveCaster(m_linkedSprites[i]->GetRuEntity());
			}
		}

		m_linkedSprites[i]->SetPos(m_nowPos);
		m_linkedSprites[i]->UnlinkChildSprite();
	}

	m_linkedSprites.Clear();

	// Unlink from parent
	if(m_linkParent)
	{
		m_linkParent->UnlinkSprite(this);
	}	
}

//-----------------------------------------------------------------------------
IBaseSprite *IBaseSprite::GetLinkParent()
{
	return m_linkParent;
}

//-----------------------------------------------------------------------------
bool IBaseSprite::SpriteControlsAreLinked()
{
	if(m_linkParent && m_linkControls)
	{
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
void IBaseSprite::UpdateLinkedSprites()
{
	for(INT32 i = 0; i < m_linkedSprites.Count(); ++i)
	{
		CRuVector3 linkPos = m_linkedSprites[i]->m_linkAttachPointName ? this->GetAttachPos(m_linkedSprites[i]->m_linkAttachPointName) : this->GetPos();
		CRuVector3 linkDir = m_linkedSprites[i]->m_linkAttachPointName ? this->GetAttachDir(m_linkedSprites[i]->m_linkAttachPointName) : CRuVector3(0.0f, 1.0f, 0.0f);

		CRuVector3 positionOffset = linkPos - m_linkedSprites[i]->m_linkParentLastPosition;
		m_linkedSprites[i]->MovePos(positionOffset);
		m_linkedSprites[i]->SetDirection(this->GetDirection());
		m_linkedSprites[i]->SetOrientingAxis(linkDir, true);
		m_linkedSprites[i]->m_linkParentLastPosition = linkPos;

		m_linkedSprites[i]->UpdateOrientation();
	}
}

//-----------------------------------------------------------------------------
IBaseSprite *IBaseSprite::FindMountCandidate()
{
	if( !GetRuEntity() )
	{
		return NULL;
	}

	REAL nearestMountPointDist = FLT_MAX;
	IBaseSprite *nearestMountSprite = NULL;

	REAL mountDistThreshold = 25.0f;
	CRuVector3 sprPos = this->GetPos() + CRuVector3(0.0f, 2.0f, 0.0f);

	for(INT32 i = 0; i < s_colSprites.Count(); ++i)
	{
		// Do not perform mounted check against self
		if(s_colSprites[i] == this || s_colSprites[i]->GetServerMove() == false )
			continue;

		CRuEntity *sprEntity = s_colSprites[i]->GetRuEntity();

		if(sprEntity)
		{
			// Perform collision query against current candidate sprite
			CRuSegment3 colTestSeg(sprPos, sprPos - CRuVector3(0.0f, mountDistThreshold, 0.0f));

			CRuCollisionQuery_Segment_Nearest segQuery;

			segQuery.m_colSegment = colTestSeg;
			segQuery.m_exclusionMask = RuWorld::ruTRIFLAGS_WATER | RuWorld::ruTRIFLAGS_FOLIAGE;
			segQuery.m_ignoreBackface = FALSE;
			segQuery.m_ignoreCollisionFlag = FALSE;

			sprEntity->QueryCollision(&segQuery);

			// Keep the closest candidate sprite
			if(segQuery.m_positiveHit && segQuery.m_collisionT < nearestMountPointDist)
			{
				nearestMountPointDist = segQuery.m_collisionT;
				nearestMountSprite = s_colSprites[i];
			}
		}
	}
/*
	// Debug
	if(nearestMountSprite)
	{
		char outputStr[1024];
		sprintf(outputStr, "Mounted Sprite: %x\n", reinterpret_cast<PTRVALUE>(nearestMountSprite));
		OutputDebugString(outputStr);
	}
*/
	return nearestMountSprite;
}

//-----------------------------------------------------------------------------
void IBaseSprite::SetParentMount(IBaseSprite *parentMount)
{
	IBaseSprite* previous = parentMount;
	while ( previous )
	{
		if ( previous == this )
		{
			parentMount = NULL;
			break;
		}
		previous = previous->GetParentMount();
	}

	m_parentMount = parentMount;
}

//-----------------------------------------------------------------------------
void IBaseSprite::MoveWithParentMount()
{
	// 記錄遞迴的每個節點
	// static set<IBaseSprite*> s_recursionParentMount;
	// static int s_recursionParentMountCount = 0;	

	if ( !GetRuEntity() )
	{
		SetParentMount(NULL);
	}

	if(m_parentMount)
	{
		//set<IBaseSprite*>::iterator iter = s_recursionParentMount.find(this);
		//if ( iter != s_recursionParentMount.end() )
		//	return;

		//s_recursionParentMountCount++;
		//s_recursionParentMount.insert(this);

		// Recursively invoke on parent mount sprite
		m_parentMount->MoveWithParentMount();

		CRuEntity *parentEntity = m_parentMount->GetRuEntity();
		CRuEntity *childEntity = this->GetRuEntity();

		if(m_parentMount == m_lastParentMount && m_linkParent == NULL)
		{
			CRuVector3 offset = parentEntity->GetTranslation(ruFRAMETYPE_WORLD) - m_lastParentMountPosition;
			childEntity->Translate(offset);

			// Update cached sprite position
			m_nowPos = childEntity->GetTranslation();
		}

		// Save parent mount information
		m_lastParentMount = m_parentMount;
		m_lastParentMountPosition = parentEntity->GetTranslation(ruFRAMETYPE_WORLD);

		//s_recursionParentMountCount--;
		//if ( s_recursionParentMountCount == 0 )
		//{
		//	s_recursionParentMount.clear();
		//}
	}
	else
	{
		m_lastParentMount = NULL;
	}	
}

//-----------------------------------------------------------------------------
void IBaseSprite::UpdateChildSprite()
{
	// return;

	// 搜尋所有物件如果設定父元件為自己一起更新坐標
	for ( set<IBaseSprite*>::iterator iter = s_containers.begin(); iter != s_containers.end(); iter++ )
	{
		if ( (*iter)->GetParentMount() == this )
		{
			(*iter)->MoveWithParentMount();
		}
	}
}

//-----------------------------------------------------------------------------
BOOL IBaseSprite::CreateCompass(INT32 id, const char *resourceName, RUCOLOR color)
{
	// Make sure there is no other compass with the specified ID
	for(INT32 i = 0; i < m_compassList.Count(); ++i)
	{
		if(m_compassList[i].m_id == id)
		{
			return FALSE;
		}
	}

	// Insert compass descriptor
	INT32 compassIdx = m_compassList.Count();
	m_compassList.Add(CompassDesc());

	m_compassList[compassIdx].m_id = id;
	m_compassList[compassIdx].m_compassEntity = g_ruResourceManager->LoadEntity(resourceName);
	m_compassList[compassIdx].m_compassTarget.Set(0.0f, 0.0f, 0.0f);
	m_compassList[compassIdx].m_compassColor = color;

	// If the resource load failed, abort the creation process
	if(m_compassList[compassIdx].m_compassEntity == NULL)
	{
		m_compassList.RemoveAt(compassIdx);
		return FALSE;
	}

	// Attach color controller
	CRuController_GlobalColor *colorController = ruNEW CRuController_GlobalColor();
	colorController->SetStartColor(CRuColor4(color));
	colorController->SetEndColor(CRuColor4(color));
	colorController->SetTarget(m_compassList[compassIdx].m_compassEntity);
	m_compassList[compassIdx].m_compassEntity->AddChild(colorController);
	ruSAFE_RELEASE(colorController);

	return TRUE;
}

//-----------------------------------------------------------------------------
BOOL IBaseSprite::SetCompassParameters(INT32 id, const CRuVector3 &destPt)
{
	// Find the specified compass
	for(INT32 i = 0; i < m_compassList.Count(); ++i)
	{
		if(m_compassList[i].m_id == id)
		{
			m_compassList[i].m_compassTarget = destPt;
			return TRUE;
		}
	}

	return FALSE;
}

//-----------------------------------------------------------------------------
BOOL IBaseSprite::HideCompass(INT32 id)
{
	// Find the specified compass
	for(INT32 i = 0; i < m_compassList.Count(); ++i)
	{
		if(m_compassList[i].m_id == id)
		{
			if(m_compassList[i].m_compassEntity)
			{
				m_compassList[i].m_compassEntity->SetOption(ruENTITYOPTION_HIDDEN, TRUE);
			}

			return TRUE;
		}
	}

	return FALSE;
}

//-----------------------------------------------------------------------------
BOOL IBaseSprite::ShowCompass(INT32 id)
{
	// Find the specified compass
	for(INT32 i = 0; i < m_compassList.Count(); ++i)
	{
		if(m_compassList[i].m_id == id)
		{
			if(m_compassList[i].m_compassEntity)
			{
				m_compassList[i].m_compassEntity->SetOption(ruENTITYOPTION_HIDDEN, FALSE);
			}

			return TRUE;
		}
	}

	return FALSE;
}

//-----------------------------------------------------------------------------
void IBaseSprite::HideAllCompasses()
{
	for(INT32 i = 0; i < m_compassList.Count(); ++i)
	{
		if(m_compassList[i].m_compassEntity)
		{
			m_compassList[i].m_compassEntity->SetOption(ruENTITYOPTION_HIDDEN, TRUE);
		}
	}
}

//-----------------------------------------------------------------------------
void IBaseSprite::ShowAllCompasses()
{
	for(INT32 i = 0; i < m_compassList.Count(); ++i)
	{
		if(m_compassList[i].m_compassEntity)
		{
			m_compassList[i].m_compassEntity->SetOption(ruENTITYOPTION_HIDDEN, FALSE);
		}
	}
}

//-----------------------------------------------------------------------------
CRuGlobalEventManager *IBaseSprite::GetGlobalEventManager()
{
	return s_pRuEventListener;
}

//-----------------------------------------------------------------------------
bool IBaseSprite::IsMotionComplete()
{
	if ( m_pRuEntity == NULL )
		return true;
	return (RuACT_IsMotionComplete(m_pRuEntity, 0xFFFFFFFF, TRUE)) ? true : false;
}

//-----------------------------------------------------------------------------
void IBaseSprite::AddOnEntity( bool isAttach, CRuEntity* pRuEntity, SpriteAttachPoint point )
{
	if ( m_pRuEntity == NULL || pRuEntity == NULL )	
		return;

	if ( isAttach )
		RuEntity_Attach_BFS( m_pRuEntity, pRuEntity, GetAttachPointName(point) );
	else
	{
		pRuEntity->DetachFromParent();
		//RuEntity_Detach( m_pRuEntity, pRuEntity, GetAttachPointName(point) );
	}
}

//-----------------------------------------------------------------------------
SpriteMoveDirection IBaseSprite::ComputeDirection( CRuVector3 forward )
{
	if ( IsSame(forward.m_x, 0.0f) && IsSame(forward.m_z, 0.0f) )
		return eMove_NULL;

	forward.m_y = 0.0f;
	forward.Normalize();
	float theta = forward.Dot( m_direction );

	// 大於157.5度 , 後退
	if ( theta < -0.92388 )
		return eMove_Back;
	// 介於 112.5 ~ 157.5 度 , 斜後退
	else if ( theta < -0.38268 )			
	{
		if ( TripleProduct_Scalar( m_direction, forward, CRuVector3(0.0f, 1.0f, 0.0f) ) > 0.0f )
			return eMove_Back_Left;
		else
			return eMove_Back_Right;
	}
	// 介於 67.5 ~ 112.5 度 , 側移
	else if ( theta < 0.38268 )
	{
		if ( TripleProduct_Scalar( m_direction, forward, CRuVector3(0.0f, 1.0f, 0.0f) ) > 0.0f )
			return eMove_Side_Right;
		else
			return eMove_Side_Left;
	}
	// 介於 22.5 ~ 67.5 度 , 斜前進
	else if ( theta < 0.92388 )
	{
		if ( TripleProduct_Scalar( m_direction, forward, CRuVector3(0.0f, 1.0f, 0.0f) ) > 0.0f )
			return eMove_Forward_Right;
		else
			return eMove_Forward_Left;
	}
	// 小於 22.5 , 前進
	else
		return eMove_Forward;
}

//-----------------------------------------------------------------------------
// 對Y軸旋轉方向向量
CRuVector3 IBaseSprite::RotateDirection( float delta )
{	
	float c = cos(delta);
	float s = sin(delta);
	return CRuVector3(m_direction.m_x * c - m_direction.m_z * s, 0.0f, m_direction.m_x * s + m_direction.m_z * c);
}

//-----------------------------------------------------------------------------
// 是否相同朝向
bool IBaseSprite::CheckSameForward(CRuVector3 forward)
{
	if (forward.m_x == 0.0f && forward.m_y == 0.0f && forward.m_z == 0.0f)
		return true;
	forward.Normalize();
	return (forward.Dot(m_direction) > 0);
}

//-----------------------------------------------------------------------------
bool IBaseSprite::AttachSprite(IBaseSprite* sprite, int point)
{
	if ( sprite )
	{
		CRuEntity* childEntity = sprite->GetRuEntity();
		CRuEntity* parentEntity = GetCurrentEntity();

		if ( parentEntity )
		{
			if ( RuEntity_Attach_BFS(parentEntity, childEntity, GetAttachPointName(point)) )
			{
				// 記錄目前已經 Attach Entity, 在變身時需要做替換
				m_attachSprite.insert(make_pair(sprite, point));

				return true;
			}
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
bool IBaseSprite::DetachSprite(IBaseSprite* sprite)
{
	map<IBaseSprite*, int>::iterator iter = m_attachSprite.find(sprite);
	if ( iter != m_attachSprite.end() )
	{
		CRuEntity* childEntity = iter->first->GetRuEntity();
		if ( childEntity )
		{
			BOOL ret = childEntity->DetachFromParent();

			m_attachSprite.erase(iter);

			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
void IBaseSprite::TransAttachSprite(CRuEntity* newEntity)
{
	if ( newEntity )
	{
		for ( map<IBaseSprite*, int>::iterator iter = m_attachSprite.begin(); iter != m_attachSprite.end(); iter++ )
		{
			CRuEntity* entity = iter->first->GetRuEntity();

			if ( entity )
			{
				entity->DetachFromParent();

				RuEntity_Attach_BFS(newEntity, entity, GetAttachPointName(iter->second));
			}
		}
	}	
}

//-----------------------------------------------------------------------------
void IBaseSprite::SetAboveWaterState(bool enable)
{
	if ( m_linkParent )
		m_linkParent->SetAboveWaterState(enable);
	m_aboveWater = enable;
}