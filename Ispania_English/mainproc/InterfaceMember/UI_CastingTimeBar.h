#pragma once

#include "../GameMain.h"

// 使用或施法時, 顯示目前進度的 BAR
// 有兩種
//	1. 由左至右滿了之後消失
//	2. 由右至左沒了之後消失
//	消失時加 ALPHA 框消失



#define Register_UI_CastingTimeBar    \
	g_pLuaScript->RegisterFunc(	"OnLoad_CastingTimeBar",		void (), CUI_CastingTimeBar::OnLoad_CastingTimeBar		);\


// Globals
//-------------------------------------------------------------------
class	CUI_CastingTimeBar;
extern	CUI_CastingTimeBar							*g_pUi_CastingTimeBar;
//-------------------------------------------------------------------

typedef void (*PF_EVENT_INTERRUPTED)	(void);
typedef void (*PF_EVENT_FINISHED)		(void);

enum	EM_CASTING_MODE
{
	EM_CASTING_MODE_NONE		=	0,
	EM_CASTING_MODE_NORMAL		=	1,
	EM_CASTING_MODE_CHANNEL		=	2,
	EM_CASTING_MODE_FAILED		=	10,
	EM_CASTING_MODE_FINISHED	=	11,
};

class CUI_CastingTimeBar
{
public:
	CUI_CastingTimeBar(void);
	~CUI_CastingTimeBar(void);

	static	void	Initial();
	static	void	Release();
	static	void	OnUpdate				();
	static  void	OnLoad_CastingTimeBar	();

	void			OnUpdate_Mode_Normal	();
	void			OnUpdate_Mode_Finished	();

	//--------------------------------------------------------------------
	void			Start_Mode_Normal		( const char* pszTitle, DWORD dwTotalTime );
		// 不計算與目標的距離, 只要本身動作改變, 即發出事件
	

	void			Start_Mode_Finished		();


	void			Interrupted				();
		// 中斷

	void			AdjustTime				( int iAdjTime );
		// 增加或減少時間

	void			SetCB_Interrupted		( PF_EVENT_INTERRUPTED pfun )	{ m_pfInterrupted	= pfun; }
	void			SetCB_Finished			( PF_EVENT_FINISHED pfun )		{ m_pfFinished		= pfun; }

	void			ReSet					();
	void			SetObjAlpha				( BYTE Alpha );
	void			SetHighlightAlpha		( BYTE Alpha );
	void			SetBackGroundAlpha		( BYTE Alpha );
	

	DWORD							m_pSparkX;
	DWORD							m_pSparkY;

	EM_CASTING_MODE					m_emMode;

	DWORD							m_dwEndTime;
	DWORD							m_dwTotalTime;
	DWORD							m_dwTime;

	DWORD							m_dwLastTime;
	
	DWORD							m_dwLastMotion;

	char							m_szTitle[64];
	
	CLayout*						m_pCastingBar;
	CFontString*					m_pName;
	CStatusBar*						m_pStatusBar;
	CTexture*						m_pSpark;
	CTexture*						m_pBorderHighlight;
	CTexture*						m_pBorder;
	CTexture*						m_pBackGround;
	
	static	CUI_CastingTimeBar*		m_pThis;
	static	PF_EVENT_INTERRUPTED	m_pfInterrupted;
	static	PF_EVENT_FINISHED		m_pfFinished;

	struct UI_OBJ_COLOR
	{	
		union {
			struct { BYTE b, g, r, a; };
			DWORD Col;
		};
	};

};
