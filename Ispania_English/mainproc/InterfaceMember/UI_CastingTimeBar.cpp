#include "../GameMain.h"
#include "WorldFrame.h"
#include ".\Ui_CastingTimeBar.h"


// 全域宣告
//-------------------------------------------------------------------
CUI_CastingTimeBar*		g_pUi_CastingTimeBar = NULL;

CUI_CastingTimeBar*		CUI_CastingTimeBar::m_pThis			= NULL;
PF_EVENT_INTERRUPTED	CUI_CastingTimeBar::m_pfInterrupted	= NULL;
PF_EVENT_FINISHED		CUI_CastingTimeBar::m_pfFinished	= NULL;

//-------------------------------------------------------------------

#define DF_CASTING_ALPHA_OUT_STEP	13
#define DF_CASTING_FADE_OUT_STEP	50


CUI_CastingTimeBar::CUI_CastingTimeBar(void)
{
	m_dwTotalTime	= 0;
	m_emMode		= EM_CASTING_MODE_NONE;

}

CUI_CastingTimeBar::~CUI_CastingTimeBar(void)
{

}

void CUI_CastingTimeBar::Initial()
{
	if ( g_pUi_CastingTimeBar == NULL )
	{
		g_pUi_CastingTimeBar	= new CUI_CastingTimeBar;
		// Register Func
		//g_pLuaScript->RegisterFunc(	"UI_CharStatus_OnLoad",		void (const char*), UI_CharStatus_OnLoad		);
		//g_pLuaScript->RegisterFunc(	"UI_CharStatus_OnUpdate",	void (void),		UI_CharStatus_OnUpdate	);
		

	}
}

//--------------------------------------------------------------------------------------
void CUI_CastingTimeBar::Release()
{
	SAFE_DELETE( g_pUi_CastingTimeBar );	
}
//--------------------------------------------------------------------------------------
void CUI_CastingTimeBar::OnUpdate()
{
	if( m_pThis == NULL || m_pThis->m_emMode == EM_CASTING_MODE_NONE )
		return;
	
	switch( m_pThis->m_emMode )
	{
	case EM_CASTING_MODE_NORMAL:	
		m_pThis->OnUpdate_Mode_Normal(); break;
	case EM_CASTING_MODE_FINISHED:
		m_pThis->OnUpdate_Mode_Finished(); break;
	}



}
//--------------------------------------------------------------------------------------
void CUI_CastingTimeBar::OnUpdate_Mode_Finished()
{

	int Alpha;
	int	FlashAlpha;
	UI_OBJ_COLOR Color;
	
	if( m_pBorder )
	{
		Color.Col	= m_pBorder->GetVertexColor();
		Alpha		= (int)Color.a;

		Color.Col	= m_pBorderHighlight->GetVertexColor();
		FlashAlpha	= (int)Color.a;

		Alpha		= Alpha - DF_CASTING_ALPHA_OUT_STEP;
		FlashAlpha	= FlashAlpha - DF_CASTING_FADE_OUT_STEP;

		if( FlashAlpha < 0 )
		{
			FlashAlpha = 0;
		}

		if( Alpha < 0 )
		{
			// 完成
			m_emMode = EM_CASTING_MODE_NONE;
			m_pCastingBar->Hide();
		}
		else
		{
			SetObjAlpha( Alpha );
			SetBackGroundAlpha( ( Alpha / 2 ) );
			SetHighlightAlpha( FlashAlpha );
			//SetHighlightAlpha( 255 );

		}
	}
}
//--------------------------------------------------------------------------------------
void CUI_CastingTimeBar::SetObjAlpha				( BYTE Alpha )
{
	UI_OBJ_COLOR Color;

	// Title
	if( m_pName )
	{
		Color.Col	= m_pName->GetColor();
		Color.a		= Alpha;
		m_pName->SetColor( (DWORD) Color.Col );
	}

	// Border
	if( m_pBorder )
	{
		Color.Col	= m_pBorder->GetVertexColor();
		Color.a		= Alpha;
		m_pBorder->SetVertexColor( (int) Color.Col );
	}

	// Spark
	if( m_pSpark )
	{
		Color.Col	= m_pSpark->GetVertexColor();
		Color.a		= Alpha;
		m_pSpark->SetVertexColor( (int) Color.Col );
	}

	// StatusBar
	if( m_pStatusBar )
	{
		Color.Col	= m_pStatusBar->GetBarColor();
		Color.a		= Alpha;
		m_pStatusBar->SetBarColor( (int) Color.Col );
	}
}
//--------------------------------------------------------------------------------------
void CUI_CastingTimeBar::SetHighlightAlpha ( BYTE Alpha )
{
	UI_OBJ_COLOR Color;

	// BorderHighlight
	if( m_pBorderHighlight )
	{
		Color.Col	= m_pBorderHighlight->GetVertexColor();
		Color.a		= Alpha;
		m_pBorderHighlight->SetVertexColor( (int) Color.Col );
	}
}
//--------------------------------------------------------------------------------------
void CUI_CastingTimeBar::SetBackGroundAlpha		( BYTE Alpha )
{	
	UI_OBJ_COLOR Color;

	// BorderHighlight
	if( m_pBackGround )
	{
		Color.Col	= m_pBackGround->GetVertexColor();
		Color.a		= Alpha;
		m_pBackGround->SetVertexColor( (int) Color.Col );
	}
}
//--------------------------------------------------------------------------------------
void CUI_CastingTimeBar::OnUpdate_Mode_Normal ()
{
	DWORD dwTime	= GetTickCount() - m_dwLastTime;
	m_dwTime		= m_dwTime + dwTime;

	m_pStatusBar->SetValue( m_dwTime );

	m_dwLastTime	= GetTickCount();

	if( m_dwTime >= m_dwTotalTime )
	{
		// Finish
		if( m_pfFinished )
			m_pfFinished();

		Start_Mode_Finished();
	}

	if( m_dwLastMotion != g_pWorldFrame->GetPlayer()->GetAction() )
	{
		if( m_pfInterrupted )
			m_pfInterrupted();
	}

	int		iPos	= (int)( ( (float)m_dwTime / (float)m_dwTotalTime ) * 195 );
	
	//m_pSpark->SetPos( iPos, 0 );

	int		iX		= iPos - 16;
	int		iY		= m_pSpark->GetYPos();

	m_pSpark->SetPos( iX, iY );


}
//--------------------------------------------------------------------------------------
void CUI_CastingTimeBar::Start_Mode_Finished		()
{		
	UI_OBJ_COLOR Color;

	// 設定完成模式
		m_emMode	= EM_CASTING_MODE_FINISHED;

		Color.a		= 255;
		Color.r		= 0; 
		Color.g		= 255;
		Color.b		= 0;

		m_pStatusBar->SetBarColor( Color.Col );
		m_pStatusBar->SetValue( m_dwTotalTime );	// fill Status Bar to full
		m_pSpark->Hide();							// Hide Spark
		m_pThis->m_pBorderHighlight->Show();		// Show Highlight around border

	// 將 ALPHA 設滿
		
		Color.Col	= m_pThis->m_pBorderHighlight->GetVertexColor();
		Color.a		= 255;
		m_pThis->m_pBorderHighlight->SetVertexColor( (int)Color.Col );

	// 




}
//--------------------------------------------------------------------------------------
void CUI_CastingTimeBar::OnLoad_CastingTimeBar()
{
	if( m_pThis == NULL )
	{
		m_pThis						= g_pUi_CastingTimeBar;
		m_pThis->m_pCastingBar		= CLayout::GetLayout( "UI_CastingTimeBarFrame" );
		m_pThis->m_pName			= (CFontString*) CLayout::GetLayout( "UI_CastingTimeBarFrame_Text" );
		m_pThis->m_pStatusBar		= (CStatusBar*) CLayout::GetLayout( "UI_CastingTimeBarFrame_StatusBar" );
		m_pThis->m_pSpark			= (CTexture*) CLayout::GetLayout( "UI_CastingTimeBarFrame_Spark" );
		m_pThis->m_pBorderHighlight	= (CTexture*) CLayout::GetLayout( "UI_CastingTimeBarFrame_BorderHighlight" );
		m_pThis->m_pBorder			= (CTexture*) CLayout::GetLayout( "UI_CastingTimeBarFrame_Border" );
		m_pThis->m_pBackGround		= (CTexture*) CLayout::GetLayout( "UI_CastingTimeBarFrame_BackGround" );

		//m_pThis->m_pStatusBar->SetMaxValue( 100 );
		//m_pThis->m_pStatusBar->SetValue( 50 );
		//m_pThis->StartMode_1( NULL, 100000 );

		// 計算 Spark 的位置

		
	}

}
//--------------------------------------------------------------------------------------
void CUI_CastingTimeBar::ReSet()
{
	if( m_pThis )
	{
		// 設定所有元件的 ALPHA 為 255
			SetObjAlpha( 255 );
			SetHighlightAlpha( 255 );
			SetBackGroundAlpha( 125 );

			if( m_pCastingBar )
			{
				m_pCastingBar->Show();
			}
			
			// Title
			if( m_pName )
			{
				m_pName->Show();
			}

			// Border
			if( m_pBorder )
			{
				m_pBorder->Show();
			}

			// Spark
			if( m_pSpark )
			{
				m_pSpark->Show();
			}

			// StatusBar
			if( m_pStatusBar )
			{
				UI_OBJ_COLOR Color;

				Color.a		= 255;
				Color.r		= 255; 
				Color.g		= 178;
				Color.b		= 0;

				m_pStatusBar->SetBarColor( Color.Col );
				m_pStatusBar->Show();
			}

			// BorderHighlight
			if( m_pBorderHighlight )
			{
				m_pBorderHighlight->Hide();
			}
	}
}
//--------------------------------------------------------------------------------------
void CUI_CastingTimeBar::Start_Mode_Normal ( const char* pszTitle, DWORD dwTotalTime )
{
	ReSet();

	strcpy( m_szTitle , pszTitle );

	m_dwTotalTime	= dwTotalTime;
	m_dwLastTime	= GetTickCount();
	m_dwEndTime		= m_dwLastTime + dwTotalTime;

	m_emMode		= EM_CASTING_MODE_NORMAL;
	m_dwTime		= 0;
	m_dwLastMotion	= g_pWorldFrame->GetPlayer()->GetAction();
	
	if( m_pStatusBar )
	{
		m_pStatusBar->SetMaxValue( dwTotalTime );
		m_pStatusBar->SetValue( 0 );
	}
}
//--------------------------------------------------------------------------------------
void CUI_CastingTimeBar::AdjustTime				( int iAdjTime )
{
	m_dwTime += iAdjTime;
}