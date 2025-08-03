
#include "../GameMain.h"
#include "WorldFrame.h"
#include ".\ui_CharheadFrame.h"


// 全域宣告
//-------------------------------------------------------------------
CUI_CharHeadFrame*		g_pUi_CharHeadFrame						= NULL;

/*
CFontString*		CUI_CharHeadFrame::m_pFont_RoleName		= NULL;
CFontString*		CUI_CharHeadFrame::m_pFont_RoleHP		= NULL;
CFontString*		CUI_CharHeadFrame::m_pFont_RoleMP		= NULL;
					
CFontString*		CUI_CharHeadFrame::m_pFont_TargetName	= NULL;
CFontString*		CUI_CharHeadFrame::m_pFont_TargetLV		= NULL;
CFontString*		CUI_CharHeadFrame::m_pFont_TargetHP		= NULL;
CFontString*		CUI_CharHeadFrame::m_pFont_TargetMP		= NULL;
CStatusBar*			CUI_CharHeadFrame::m_pBar_TargetHP		= NULL;
CStatusBar*			CUI_CharHeadFrame::m_pBar_TargetMP		= NULL;

CLayout*			CUI_CharHeadFrame::m_pTargetFrame		= NULL;
CLayout*			CUI_CharHeadFrame::m_pCharFrame			= NULL;		// 人物狀態欄
*/

_HEADFREAMINFO		CUI_CharHeadFrame::m_RoleInfo;
_HEADFREAMINFO		CUI_CharHeadFrame::m_TargetRoleInfo;

CRoleSprite*		CUI_CharHeadFrame::m_pTargetRole		= NULL;

//-------------------------------------------------------------------
CUI_CharHeadFrame::CUI_CharHeadFrame(void)
{
	m_RoleInfo.m_pFrame			= NULL;
	m_TargetRoleInfo.m_pFrame	= NULL;

	CNetGlobal::Schedular()->Push( OnTimeUpdate , 50 , NULL , NULL );
}

CUI_CharHeadFrame::~CUI_CharHeadFrame(void)
{
}

void CUI_CharHeadFrame::Initial()
{
	if ( g_pUi_CharHeadFrame == NULL )
	{
		g_pUi_CharHeadFrame = new CUI_CharHeadFrame;
	}
}
// --------------------------------------------------------------------------------------
int	CUI_CharHeadFrame::OnTimeUpdate( SchedularInfo* Obj , void* InputClass )
{
	char szOut[64];
	CNetGlobal::Schedular()->Push( OnTimeUpdate , 50 , NULL , NULL );

	CRoleSprite* pRole		= g_pWorldFrame->GetPlayer();

	if( m_RoleInfo.m_pFrame && pRole )
	{
		// 更新 HP 及 MP
				
		if( m_RoleInfo.m_pName )
		{
			m_RoleInfo.m_pName->SetTextA(  pRole->GetName() );
		}

		if( m_RoleInfo.m_pLV )
		{
			sprintf( szOut, "%d", pRole->GetLevel() );
			m_RoleInfo.m_pLV->SetTextA( szOut );
		}

		if( m_RoleInfo.m_pHP_Bar )
		{
			m_RoleInfo.m_pHP_Bar ->SetMaxValue( pRole->GetMaxHP() );
			m_RoleInfo.m_pHP_Bar ->SetValue( pRole->GetHP() );
		}

		if( m_RoleInfo.m_pMP_Bar )
		{
			m_RoleInfo.m_pMP_Bar->SetMaxValue( pRole->GetMaxMP() );
			m_RoleInfo.m_pMP_Bar->SetValue( pRole->GetMP() );
		}

		if( m_RoleInfo.m_pSP_Bar )
		{
			m_RoleInfo.m_pSP_Bar->SetMaxValue( pRole->GetMaxMP() );
			m_RoleInfo.m_pSP_Bar->SetValue( pRole->GetMP() );
		}
	}

	if( m_TargetRoleInfo.m_pFrame )
	{
		// 檢查更新
		CRoleSprite* pTargetRole = g_pWorldFrame->GetTargetPlayer();
		
		if( pTargetRole )
		{
			if( m_TargetRoleInfo.m_pFrame->IsVisible() == false )
				m_TargetRoleInfo.m_pFrame->Show();

			if( m_pTargetRole != pTargetRole )
			{
				m_pTargetRole = pTargetRole;
			}

			

			if( m_TargetRoleInfo.m_pName )
			{
				m_TargetRoleInfo.m_pName->SetTextA(  m_pTargetRole->GetName() );
			}

			if( m_TargetRoleInfo.m_pLV )
			{
				sprintf( szOut, "%d", m_pTargetRole->GetLevel() );
				m_TargetRoleInfo.m_pLV->SetTextA( szOut );
			}

			if( m_TargetRoleInfo.m_pHP_Bar )
			{
				m_TargetRoleInfo.m_pHP_Bar ->SetMaxValue( m_pTargetRole->GetMaxHP() );
				m_TargetRoleInfo.m_pHP_Bar ->SetValue( m_pTargetRole->GetHP() );
			}

			if( m_TargetRoleInfo.m_pMP_Bar )
			{
				m_TargetRoleInfo.m_pMP_Bar->SetMaxValue( m_pTargetRole->GetMaxMP() );
				m_TargetRoleInfo.m_pMP_Bar->SetValue( m_pTargetRole->GetMP() );
			}

			if( m_TargetRoleInfo.m_pSP_Bar )
			{
				m_TargetRoleInfo.m_pSP_Bar->SetMaxValue( m_pTargetRole->GetMaxMP() );
				m_TargetRoleInfo.m_pSP_Bar->SetValue( m_pTargetRole->GetMP() );
			}
		}
		else
		{
			m_pTargetRole = NULL;
			m_TargetRoleInfo.m_pFrame->Hide();
		}



	}

	return 0;
}

// --------------------------------------------------------------------------------------
void CUI_CharHeadFrame::Release()
{
	SAFE_DELETE( g_pUi_CharHeadFrame );	
}
//-------------------------------------------------------------------
void CUI_CharHeadFrame::Update ()
{


}
//-------------------------------------------------------------------
void CUI_CharHeadFrame::OnLoad_CharHeadFrame()
{
	m_RoleInfo.m_pFrame			= CLayout::GetLayout( "UI_RoleHeadFrame" );
	m_RoleInfo.m_pName			= (CFontString*)CLayout::GetLayout( "UI_RoleHeadFrame_Border_Name"		);
	m_RoleInfo.m_pLV			= (CFontString*)CLayout::GetLayout( "UI_RoleHeadFrame_Border_LVTEXT"	);

	m_RoleInfo.m_pHP_Bar		= (CStatusBar*)CLayout::GetLayout(	"UI_RoleHeadFrame_HPBAR"			);
	m_RoleInfo.m_pMP_Bar		= (CStatusBar*)CLayout::GetLayout(	"UI_RoleHeadFrame_MPBAR"			);
	m_RoleInfo.m_pSP_Bar		= (CStatusBar*)CLayout::GetLayout(	"UI_RoleHeadFrame_SPBAR"			);
	
}
//-------------------------------------------------------------------
void CUI_CharHeadFrame::OnLoad_TargetHeadFrame()
{
	m_TargetRoleInfo.m_pFrame 	= CLayout::GetLayout( "UI_TargetHeadFrame" );

	m_TargetRoleInfo.m_pName	= (CFontString*)CLayout::GetLayout( "UI_TargetHeadFrame_Border_Name"		);	
	m_TargetRoleInfo.m_pLV		= (CFontString*)CLayout::GetLayout( "UI_TargetHeadFrame_Border_LVTEXT"		);
	m_TargetRoleInfo.m_pHP_Bar	= (CStatusBar*)CLayout::GetLayout(	"UI_TargetHeadFrame_HPBAR"				);
	m_TargetRoleInfo.m_pMP_Bar	= (CStatusBar*)CLayout::GetLayout(	"UI_TargetHeadFrame_MPBAR"				);
	m_TargetRoleInfo.m_pSP_Bar	= (CStatusBar*)CLayout::GetLayout(	"UI_TargetHeadFrame_SPBAR"				);

}

