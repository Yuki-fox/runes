#include ".\UI_QuestDialog.h"


// 全域宣告
//-------------------------------------------------------------------
CUI_QuestDialog				*g_pUi_QuestDialog = NULL;

int					CUI_QuestDialog::m_iQuestMode				= 0;
int					CUI_QuestDialog::m_iQuestID					= 0;
CLayout*			CUI_QuestDialog::m_pQuestDialog				= NULL;
CFontString*		CUI_QuestDialog::m_pQuestFrameNpcNameText	= NULL;
CFontString*		CUI_QuestDialog::m_pQuestFrame_QuestName	= NULL;
CFontString*		CUI_QuestDialog::m_pQuestFrame_QuestDesc	= NULL;
CFontString*		CUI_QuestDialog::m_pQuestFrame_QuestDetail	= NULL;
CButton*			CUI_QuestDialog::m_pQuestFrame_ProcessButton= NULL;
CButton*			CUI_QuestDialog::m_pQuestFrame_LeaveButton	= NULL;

//-------------------------------------------------------------------
CUI_QuestDialog::CUI_QuestDialog(void)
{

}
// --------------------------------------------------------------------------------------
CUI_QuestDialog::~CUI_QuestDialog(void)
{

}
// --------------------------------------------------------------------------------------
void CUI_QuestDialog::Initial()
{
	if ( g_pUi_QuestDialog == NULL )
	{
		g_pUi_QuestDialog = new CUI_QuestDialog;
	}
}
// --------------------------------------------------------------------------------------
void CUI_QuestDialog::Release()
{
	SAFE_DELETE( g_pUi_QuestDialog );	
}
// --------------------------------------------------------------------------------------
void CUI_QuestDialog::ReSet()
{
	if( m_pQuestDialog == NULL )
	{
		m_pQuestDialog				= CLayout::GetLayout( "UI_QuestDetail" );
		m_pQuestFrameNpcNameText	= (CFontString*) CLayout::GetLayout( "UI_QuestDetail_Panel_Name" );
		m_pQuestFrame_QuestName		= (CFontString*) CLayout::GetLayout( "UI_QuestDetail_QuestName" );
		m_pQuestFrame_QuestDesc		= (CFontString*) CLayout::GetLayout( "UI_QuestDetail_QuestDesc" );
		m_pQuestFrame_QuestDetail	= (CFontString*) CLayout::GetLayout( "UI_QuestDetail_QuestDetail" );
		
		m_pQuestFrame_ProcessButton	= (CButton*) CLayout::GetLayout( "QuestFrameProcessButton" );
		m_pQuestFrame_LeaveButton	= (CButton*) CLayout::GetLayout( "QuestFrameDeclineButton" );
	}
}

void CUI_QuestDialog::Set_NpcName ( const char* pszNpcName	)
{
	if( m_pQuestFrameNpcNameText )
	{
		m_pQuestFrameNpcNameText->SetTextA( pszNpcName );
	}
}

void CUI_QuestDialog::Set_QuestName				( const char* pszQuestName	)
{
	if( m_pQuestFrame_QuestName )
	{
		m_pQuestFrame_QuestName->SetTextA( pszQuestName );
	}
}

void CUI_QuestDialog::Set_QuestDesc				( const char* pszQuestDesc	)
{
	if( m_pQuestFrame_QuestDesc )
	{
		m_pQuestFrame_QuestDesc->SetTextA( pszQuestDesc );
	}
}

void CUI_QuestDialog::Set_QuestDetail				( const char* pszQuestDetail)
{
	if( m_pQuestFrame_QuestDetail )
	{
		m_pQuestFrame_QuestDetail->SetTextA( pszQuestDetail );
	}
}

void CUI_QuestDialog::Show()
{
	if( m_pQuestDialog )
	{
		m_pQuestDialog->Show();
	}
}

void CUI_QuestDialog::OnClick_ProcessQuest ( void )
{
	if( m_pQuestDialog )
	{
		m_pQuestDialog->Hide();

		switch( m_iQuestMode ) 
		{
		case 0: // Accept
			{
				g_pGameMain->AcceptQuest( m_iQuestID );
			} break;
		case 2: // Complete
			{
				g_pGameMain->CompleteQuest( m_iQuestID );
			} break;
		case 3: // Deltete
			{
				RoleDataEx*				pRole			= g_pGameMain->RoleData();

				for( int index = 0; index < _MAX_QUEST_COUNT_; index++ )
				{
					int iQuestID = pRole->BaseData.Quest.iQuest[ index ];
					if( iQuestID == m_iQuestID )
					{
						g_pGameMain->DeleteQuest( index, m_iQuestID );
						break;
					}
				}
			} break;
		}
	}
}

void CUI_QuestDialog::Set_QuestID ( int iQuestID, int iButtonMode )
{ 
	if( m_pQuestDialog )
	{
		m_iQuestID		= iQuestID; 
		m_iQuestMode	= iButtonMode;

		switch( m_iQuestMode ) 
		{
		case 0:	// Accept
			{
				m_pQuestFrame_ProcessButton->SetTextA( "接受" );
				m_pQuestFrame_LeaveButton->SetTextA( "拒絕" );
				m_pQuestFrame_ProcessButton->Enable();
			} break;
		case 1: // Complete ( Unable )
			{
				m_pQuestFrame_ProcessButton->SetTextA( "完成" );
				m_pQuestFrame_LeaveButton->SetTextA( "離開" );
				m_pQuestFrame_ProcessButton->Disable();
			} break;
		case 2:	// Complete
			{
				m_pQuestFrame_ProcessButton->SetTextA( "完成" );
				m_pQuestFrame_LeaveButton->SetTextA( "離開" );
				m_pQuestFrame_ProcessButton->Enable();
			} break;
		case 3: // Delete
			{
				m_pQuestFrame_ProcessButton->SetTextA( "刪除" );
				m_pQuestFrame_LeaveButton->SetTextA( "離開" );
				m_pQuestFrame_ProcessButton->Enable();
			}
			break;
		}
	}
}
