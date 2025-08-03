#pragma once

#include "../interface.h"



class CQuestTrackFrame;
extern CQuestTrackFrame* g_pQuestTrackFrame;

class CQuestTrackFrame : public CInterfaceBase
{
public:
	CQuestTrackFrame( CInterface* pObject );
	virtual~CQuestTrackFrame( void );

	virtual	void RegisterFunc();	
	virtual void Initialize( const char* pViewName, const char* pPlayerTexture );

	void RenderWorldMap( CUiFrame* frame, int lParam, int rParam );

public:
	CUiFrame*    m_pUiFrameWorldMap;
	CUiTexture*  m_pUiTextureSelf;
 
	string     m_strTrackName;
	CRuVector3 m_vecTrackPositioln;


};
