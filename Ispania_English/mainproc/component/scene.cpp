#include <windows.h>
#include "scene.h"

// Golbal
CScene*					g_pScene = NULL;

//-------------------------------------------------------------------------------------------------
CScene::CScene()
{
	g_pScene = this;
	m_pRuTerrain = NULL;
}

//-------------------------------------------------------------------------------------------------
CScene::~CScene()
{
	g_pScene = NULL;
	ReleaseTerrain();	
}

//-------------------------------------------------------------------------------------------------
void CScene::Render( CRuCamera *pRuCamera )
{
	if( m_pRuTerrain )
	{
		// �]�w�a�����Y
		m_pRuTerrain->SetCamera( pRuCamera );

		// ø�s�a��
		g_ruSceneRenderer->Scene_Submit( m_pRuTerrain );

		// g_terrain->DumpDebugInfo();
	}		
}

//-------------------------------------------------------------------------------------------------
// ���J�a���T
bool CScene::LoadTerrain( const char* resourceName )
{
	ReleaseTerrain();

	CRuOptimalTerrain_DataSource_File *terrainDS = ruNEW CRuOptimalTerrain_DataSource_File();

	// Ū���a���ƨӷ�
	if( terrainDS->Open(resourceName) )
	{
		// �إߦa��޲z����
		m_pRuTerrain = ruNEW CRuOptimalTerrain();
		m_pRuTerrain->Initialize( 0, 0 );
		m_pRuTerrain->AttachDataSource(terrainDS);
	}

	terrainDS->Release();

	return (m_pRuTerrain) ? true : false;
}

//-------------------------------------------------------------------------------------------------
// �a��I������,�D�X�a���צ�m
float CScene::TerrainHeightCollisionTest( CRuVector3 &position )
{	
	if ( m_pRuTerrain == NULL )
		return 0.0f;

	// �յۻP�a���I��,��X��m
	float height = 0.0f;
	int attempts = 0;
	bool placeFound = FALSE;

	// ���ոI���̦h�Q��,�C���S���I����N�Ⱚ�ש��W��
	while( !placeFound && attempts < 10 )
	{
		// Build collision query
		CRuCollisionQuery_Ray rayQuery;
		rayQuery.m_colRay.Origin().Set( position.x, position.y + 5.0f * attempts, position.z );
		rayQuery.m_colRay.Direction().Set( 0.0f, -1.0f, 0.0f );

		// Ray-cast into terrain
		m_pRuTerrain->QueryCollision( &rayQuery );

		if( rayQuery.m_colPoints->Count() > 0 )
		{
			// Sort the results using insertion sort
			g_ruInsertionSort.Sort( rayQuery.m_colDistances->GetArray(), rayQuery.m_colDistances->Count() );
			UINT32 *sortedIndices = g_ruInsertionSort.GetSortedIndices();

			// Set the pc's position to the nearest collision point
			height = rayQuery.m_colPoints->Get( sortedIndices[0] ).y;

			placeFound = TRUE;
		}
		++attempts;
	}

	return height;
}

//-------------------------------------------------------------------------------------------------
void CScene::ReleaseTerrain()
{
	if ( m_pRuTerrain )
	{
		m_pRuTerrain->Release();
		m_pRuTerrain = NULL;
	}
}
