#include <windows.h>
#include <tchar.h>
#include "rolesprite.h"

//-------------------------------------------------------------------------------------------------
CRoleSprite::CRoleSprite()
{
	m_pRuEntity			= NULL;

	m_refCount			= 0;
	m_identify			= 0;
	m_walkSpeed			= 0;
	m_enableBool		= TRUE;
	m_visibleBool		= TRUE;
	m_behavior			= RSB_STAND;

	m_direction.Set( 0.0f, 0.0f, -1.0f );
	m_nowPos.Set( 0.0f, 0.0f, 0.0f );
	m_tarPos.Set( 0.0f, 0.0f, 0.0f );

	memset( m_nameStr, 0, sizeof(m_nameStr) );
	m_hp				= 0;
	m_mp				= 0;
	m_maxHp				= 0;
	m_maxMp				= 0;
	m_level				= 0;
	m_modelRate			= 0;
}

//-------------------------------------------------------------------------------------------------
CRoleSprite::~CRoleSprite()
{
	Release3DObject();
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::Update( FLOAT elapsedTime, CRuOptimalTerrain* pRuTerrain )
{
	if ( m_tarPos != m_nowPos )
	{
		if ( m_moveNeedTime - elapsedTime <= 0.0f )
			m_nowPos = m_tarPos;
		else
		{
			m_nowPos.m_x += (m_tarPos.m_x - m_nowPos.m_x) / m_moveNeedTime * elapsedTime;
			m_nowPos.m_z += (m_tarPos.m_z - m_nowPos.m_z) / m_moveNeedTime * elapsedTime;
			m_moveNeedTime -= elapsedTime;
		}

		// 設定角色位置
		SetRuEntifyPosition( m_nowPos );
	}

	if ( m_pRuEntity )
	{
		// 設定 Model 矩陣轉向所面對方向
		CRuVector3 defaultOrientation(0.0f, 0.0f, -1.0f);		// 角色靜止時的方向

		// 角色定位，讓角色定在正確的高度以及方向
		RuEntity_Anchor( m_pRuEntity, pRuTerrain, defaultOrientation, m_direction );

		m_pRuEntity->Update( elapsedTime );
	}	

	if ( m_pRuEntity )
	{
		// 設定 Model 矩陣轉向所面對方向
		CRuVector3 defaultOrientation(0.0f, 0.0f, 1.0f);		// 角色靜止時的方向

		float cos2Phi = DotProduct(defaultOrientation, m_direction);

		if(cos2Phi == 1.0f)
		{
			m_pRuEntity->GetRootFrame()->GetRotation().Set(0.0f, 0.0f, 0.0f, -1.0f);
		}
		else
		{
			CRuVector3 sXt = CrossProduct(defaultOrientation, m_direction);
			float sin2Phi = sXt.Magnitude();

			if(sin2Phi == 0.0f)
			{
				m_pRuEntity->GetRootFrame()->GetRotation().Set(0.0f, 0.0f, 0.0f, -1.0f);
			}
			else
			{
				CRuVector3 u = sXt / sXt.Magnitude();

				float sinPhi = sqrt((1.0f + cos2Phi) / 2.0f);
				float cosPhi = sqrt((1.0f - cos2Phi) / 2.0f);

				m_pRuEntity->GetRootFrame()->GetRotation().Set(u.x * sinPhi, u.y * sinPhi, u.z * sinPhi, cosPhi);
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::Render()
{
	if ( m_pRuEntity )
		g_ruSceneRenderer->Scene_Submit( m_pRuEntity );		// 繪製角色
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::SetName( LPCWSTR nameStr )
{
	wcscpy( m_nameStr, nameStr );
}

//-------------------------------------------------------------------------------------------------
bool CRoleSprite::LoadCharacter( const char *resourceName )
{
	CRuEntity* playerEntity;
	
	if ( (playerEntity = g_ruResourceManager->LoadEntity(resourceName)) == NULL )
		return false;

	Release3DObject();

	// 確認物件是ACT物件
	if( playerEntity->GetType().IsSubClassOf( CRuACTEntity::Type ) )
	{
		m_pRuEntity = static_cast<CRuACTEntity *>(playerEntity);

		// 在ACT物件內找尋紙娃娃
		CRuStack<CRuEntity *> entityStack;
		entityStack.Push(m_pRuEntity->GetRootFrame());

		while( !entityStack.IsEmpty() )
		{
			CRuEntity *curEntity = entityStack.Pop();

			if( curEntity->GetType().IsSubClassOf(CRuEntity_Container::Type) )
			{
				CRuEntity_Container *container = static_cast<CRuEntity_Container *>(curEntity);
				for( INT32 i = 0; i < container->GetNumEntities(); ++i )
					entityStack.Push( container->GetEntity(i) );
			}
			else if(curEntity->GetType().IsSubClassOf(CRuPaperdoll::Type))
			{
				CRuPaperdoll *paperdoll = static_cast<CRuPaperdoll *>(curEntity);

				// 找到紙娃娃, 設定配件
				paperdoll->SetComponent("Head",		"default");
				paperdoll->SetComponent("Torso",	"magcloth");
				paperdoll->SetComponent("Leg",		"leather");
				paperdoll->SetComponent("Hair",		"default");
				paperdoll->SetComponent("Cape",		"default");
				paperdoll->SetComponent("Foot",		"cloth");
				paperdoll->SetComponent("Glove",	"cloth");

				// 建立紙娃娃模型
				paperdoll->BuildPaperdoll();
			}
		}

		// 播放一號動作
		m_pRuEntity->PlayMotion(1);

		return true;
	}

	Release3DObject();
	return false;
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::SetPos( CRuVector3 &pos )
{
	m_nowPos = pos;

	// 設定角色位置
	SetRuEntifyPosition( m_nowPos );

	MoveTo( m_nowPos );
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::MoveTo( CRuVector3 &pos )
{	
	m_tarPos = pos;
	m_moveNeedTime = 0.0f;

	if ( m_tarPos != m_nowPos )
	{
		CRuVector3 diff( m_tarPos.m_x - m_nowPos.m_x, 0.0f, m_tarPos.m_z - m_nowPos.m_z );
		m_moveNeedTime = diff.Magnitude() / m_walkSpeed;
	}
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::Move( RoleFace roleFace, FLOAT elapsedTime )
{
	CRuVector3 newPos, sideVector;
	switch ( roleFace )
	{
	case ROLE_FACE_ALONG:
		newPos = m_nowPos + m_direction * elapsedTime * m_walkSpeed;
		break;

	case ROLE_FACE_BACK:
		newPos = m_nowPos - m_direction * elapsedTime * m_walkSpeed;
		break;

	case ROLE_FACE_LEFT:
		sideVector = CrossProduct( m_direction, CRuVector3(0.0f, 1.0f, 0.0f) );
		newPos = m_nowPos + sideVector * elapsedTime * m_walkSpeed;
		break;

	case ROLE_FACE_RIGHT:
		sideVector = CrossProduct( m_direction, CRuVector3(0.0f, 1.0f, 0.0f) );
		newPos = m_nowPos - sideVector * elapsedTime * m_walkSpeed;
		break;

	default:
        return;
	}

	SetPos( newPos );
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::Release3DObject()
{
	if ( m_pRuEntity )
	{
		m_pRuEntity->Release();
		m_pRuEntity = NULL;
	}
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::SetRuEntifyPosition( CRuVector3 &position )
{
	// 角色高度由地表測試求出
	if ( g_pScene )
		position.y = g_pScene->TerrainHeightCollisionTest( position );
	m_pRuEntity->GetRootFrame()->GetTranslation().Set( position );
}
