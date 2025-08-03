#ifndef _SCENE_H
#define _SCENE_H

#include "../../Rune/Rune.h"

class CScene
{
public:
	CScene();
	~CScene();

	void					Render( CRuCamera *pRuCamera );
	bool					LoadTerrain( const char *resourceName );
	float					TerrainHeightCollisionTest( CRuVector3 &position );

protected:
	void					ReleaseTerrain();

protected:
	CRuOptimalTerrain*		m_pRuTerrain;
};

extern CScene*				g_pScene;

#endif //_SCENE_H