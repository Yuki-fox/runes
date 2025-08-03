#pragma once

#include "../interface.h"

#include "MapCommon.h"
#include "WorldMap.h"

class CMinimapFrame;
extern CMinimapFrame* g_pMinimapFrame;


class CMinimapFrame : public CInterfaceBase
{
public:
	CMinimapFrame(CInterface* object);
	virtual ~CMinimapFrame();

	void				InitializeMinimap( const char* pViewName , const char* pMaskName, const char* pPlayerCursor, const char* pDuelRange, const char* pIconName, int nIconCount );

	void				RenderMinimap(CUiFrame* frame, int lParam, int rParam);

	virtual	void		RegisterFunc();
	virtual	void		BuildFrames();
	virtual void		DestroyFrames();

	const char*			GetMinimapIconText( int IconID );
	
	void				SetPingPosition( const CUiPoint& rPoint );
	bool                GetPingPosition( CUiPoint& rPoint );

	//-------------------------------------------------------------------
	int					m_IsVisible;
	
	float				m_fZoomValue;

	bool				m_bShowOption[ EM_MapIconType_END ];
public:
	void ShowDuelRange( float fPosX , float fPosZ , float fRange );
 	void HideDuelRange();

	float m_DuelRange_fPosX;
	float m_DuelRange_fPosZ;
	float m_DuelRange_fRange;

protected:
	void UpdateMiniMap( int iVisible );
	void UpdateTrack( SMapTrackInfo* pInfo );

protected:

	struct sMinimapIconInfo
	{
		CUiFrame*	pUiFrame;
		CUiTexture*	pUiTexture;
		string		strName;
	};


	CUiFrame*           m_pUiFrameView;
	CUiTexture*         m_pUiTextureSelf;
	CUiTexture*         m_pUiDuelRange;	//¨M°«½d³ò
	sMinimapIconInfo*	m_pIconInfos;
	
	int					m_nIconCount;
	int					m_nIconUsedCount;

	bool				m_bInited;

	int					m_Range;

};


