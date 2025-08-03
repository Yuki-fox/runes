#include ".\UI_QuestList.h"
#include "../../netmember/NetScript.h"

// 全域宣告
//-------------------------------------------------------------------
CUI_QuestList			*g_pUi_QuestList = NULL;

CUI_QuestList*			CUI_QuestList::m_pThis					= NULL;
/*
CRoleSprite*			CUI_QuestList::m_InteractNPC;
vector< int >			CUI_QuestList::m_vecAvailableQuest;
vector< int >			CUI_QuestList::m_vecCurrentQuest;
vector< string >		CUI_QuestList::m_vecAvailableQuestName;
vector< string >		CUI_QuestList::m_vecCurrentQuestName;

//CButton*				m_pQuestSelectButton		[ 10 ];
//CButton*				CUI_QuestList::m_pAvailableQuestButton		[ 5 ];
//CButton*				CUI_QuestList::m_pCurrentQuestButton		[ 5 ];

CFontString*			CUI_QuestList::m_pCurrentQuestButton_Text	[ 5 ];
CFontString*			CUI_QuestList::m_pQuestFrameNpcNameText;
CFontString*			CUI_QuestList::m_pNPC_Text;				//QuestListFrame_NPC_GreetingText

int						CUI_QuestList::m_iQuestIndex = 0;

CLayout*				CUI_QuestList::m_pQuestList = NULL;
*/
//-------------------------------------------------------------------
CUI_QuestList::CUI_QuestList(void)
{
	m_pQuestList = NULL;

}
// --------------------------------------------------------------------------------------
CUI_QuestList::~CUI_QuestList(void)
{

}
// --------------------------------------------------------------------------------------
void CUI_QuestList::Initial()
{
	if ( g_pUi_QuestList == NULL )
	{
		g_pUi_QuestList = new CUI_QuestList;
		m_pThis			= g_pUi_QuestList;
	}
}
// --------------------------------------------------------------------------------------
void CUI_QuestList::Release()
{
	SAFE_DELETE( g_pUi_QuestList );	
}
//--------------------------------------------------------------------------------------
void CUI_QuestList::OnLoad_QuestListBook ()
{
	if( m_pThis )
	{
		if( m_pThis->m_pQuestList == NULL )
		{
			m_pThis->m_pQuestList = CLayout::GetLayout( "UI_QuestListBook" );

			if( m_pThis->m_pQuestList && m_pThis ) 
			{			
				char szObjName[ 255 ];

				for( int Index = 0; Index < 10; Index++ )
				{
					sprintf( szObjName,			"QuestListSelectButton_%d", ( Index + 1 ) );
					m_pThis->m_pQuestSelectButton[ Index ]	= (CButton*)CLayout::GetLayout( szObjName );
					if( m_pThis->m_pQuestSelectButton[ Index ] )
					{
						m_pThis->m_pQuestSelectButton[ Index ]->Hide();
					}
				}
			}
		}
	}
}
//--------------------------------------------------------------------------------------
void CUI_QuestList::OnShow_QuestListBook ()
{
	RoleDataEx*				pRole			= g_pGameMain->RoleData();

	m_pThis->m_vecQuestID.clear();
	
	char szObjName[ 255 ];
	for( int Index = 0; Index < 10; Index++ )
	{
		sprintf( szObjName,			"QuestListSelectButton_%d", ( Index + 1 ) );
		m_pThis->m_pQuestSelectButton[ Index ]	= (CButton*)CLayout::GetLayout( szObjName );
		if( m_pThis->m_pQuestSelectButton[ Index ] )
			m_pThis->m_pQuestSelectButton[ Index ]->Hide();
	}

	for( int index = 0; index < _MAX_QUEST_COUNT_; index++ )
	{
		int iQuestID = pRole->BaseData.Quest.iQuest[ index ];
		if( iQuestID != 0 )
		{
			// 取得任務名稱
			GameObjDbStruct* pQuestObj			= g_ObjectData->GetObj( iQuestID );

			if( pQuestObj != 0 )
			{
				if( m_pThis->m_pQuestSelectButton[ index ] )
				{
					m_pThis->m_pQuestSelectButton[ index ]->SetTextA( pQuestObj->Name );
					m_pThis->m_pQuestSelectButton[ index ]->Show();
					m_pThis->m_vecQuestID.push_back( iQuestID );
				}
			}
		}
	}
}
//--------------------------------------------------------------------------------------
void CUI_QuestList::OnClick_QuestList_Button	( int iID )
{
	if( iID <= m_pThis->m_vecQuestID.size() )
	{
		int iQuestID = m_pThis->m_vecQuestID[ ( iID - 1 )];
		g_pGameMain->OnQuestDetail( iQuestID, 3 );
	}
}
//--------------------------------------------------------------------------------------
void CUI_QuestList::ReSet ()
{
	if( m_pThis )
	{
		if( m_pThis->m_pQuestList )
		{
			if( m_pThis->m_pQuestList->IsVisible() == true )
			{
				OnShow_QuestListBook ();
			}
		}
	}
}
//--------------------------------------------------------------------------------------
/*
void CUI_QuestList::Add_NpcText					( const char* pszNpcText )
{
	if( m_pQuestList )
	{ 
		m_pNPC_Text->SetTextA( pszNpcText );
	}
}
//--------------------------------------------------------------------------------------
void CUI_QuestList::Add_Available_Quest			( int iQuestID, const char* pszQuestName )
{
	if( m_pQuestList )
	{ 
		int Index = m_vecAvailableQuest.size();
		m_vecAvailableQuest.push_back( iQuestID );
		//m_pAvailableQuestButton_Text[ m_iQuestIndex ]->SetTextA( pszQuestName );;
		m_pAvailableQuestButton[ Index ]->Show();
		m_pAvailableQuestButton[ Index ]->SetTextA( pszQuestName );
		m_pAvailableQuestButton[ Index ]->SetID( ( Index + 1 ) );
	}
}
//--------------------------------------------------------------------------------------
void CUI_QuestList::Add_Current_Quest			( int iQuestID, const char* pszQuestName )
{
	if( m_pQuestList )
	{
		int Index = m_vecCurrentQuest.size();
		m_vecCurrentQuest.push_back( iQuestID );
		//m_pAvailableQuestButton_Text[ m_iQuestIndex ]->SetTextA( pszQuestName );;
		m_pCurrentQuestButton[ Index ]->Show();
		m_pCurrentQuestButton[ Index ]->SetTextA( pszQuestName );
		m_pCurrentQuestButton[ Index ]->SetID( ( Index + 101 ) );
	}
}
//--------------------------------------------------------------------------------------
void CUI_QuestList::Add_Complete_Quest			( int iQuestID, const char* pszQuestName )
{
	if( m_pQuestList )
	{ 
		char szQuestName[64];
		sprintf( szQuestName, "%s ( Complete )", pszQuestName );

		int Index = m_vecCurrentQuest.size();
		m_vecCurrentQuest.push_back( iQuestID );
		//m_pAvailableQuestButton_Text[ m_iQuestIndex ]->SetTextA( pszQuestName );;
		m_pCurrentQuestButton[ Index ]->Show();
		m_pCurrentQuestButton[ Index ]->SetTextA( szQuestName );
		m_pCurrentQuestButton[ Index ]->SetID( ( Index + 201 ) );
	}
}
//--------------------------------------------------------------------------------------
void CUI_QuestList::Show						()
{
	if( m_pQuestList )
	{
		SetInteractRangeOn( g_pUi_QuestList, m_InteractNPC );
		m_pQuestList->Show();
	}
}
//--------------------------------------------------------------------------------------
void CUI_QuestList::ReSet						()
{

	m_iQuestIndex = 0;

	// First Time Loading
	//-----------------------------------------------------------------------------------------------
	if( m_pQuestList == NULL )
	{
		m_pQuestList = CLayout::GetLayout( "UI_NPCQuestListFrame" );

		if( m_pQuestList ) 
		{			
			char szObjName[ 255 ];
		
			for( int Index = 0; Index < 10; Index++ )
			{
				sprintf( szObjName,			"QuestSelectButton%d", ( Index + 1 ) );
				m_pAvailableQuestButton[ Index ]	= (CButton*)CLayout::GetLayout( szObjName );
				m_pAvailableQuestButton[ Index ]->Hide();
			}

			for( int Index = 0; Index < 5; Index++ )
			{
				sprintf( szObjName,			"CurrentQuestTitleButton%d", ( Index + 1 ) );
				m_pCurrentQuestButton[ Index ]		= (CButton*)CLayout::GetLayout( szObjName );
				m_pCurrentQuestButton[ Index ]->Hide();
			}

			m_pNPC_Text								= (CFontString*)CLayout::GetLayout( "QuestListFrame_NPC_GreetingText" );
			m_pQuestFrameNpcNameText				= (CFontString*)CLayout::GetLayout( "QuestListFrame_NpcFrame_NpcNameText" );
		}

	}
	//-----------------------------------------------------------------------------------------------

	m_vecAvailableQuest.clear();
	m_vecAvailableQuestName.clear();
	m_vecCurrentQuest.clear();	
	m_vecCurrentQuestName.clear();

	if( m_pQuestList  )
	{
		for( int Index = 0; Index < 5; Index++ )
		{
			m_pAvailableQuestButton[ Index ]->Hide();
			m_pCurrentQuestButton[ Index ]->Hide();
			//m_pCurrentQuestButton[ Index ]->Hide();
		}
	}
}

void CUI_QuestList::Set_NpcName ( const char* pszNpcName )
{
	if( m_pQuestList )
		m_pQuestFrameNpcNameText->SetTextA( pszNpcName );
}

void CUI_QuestList::SelectQuest					( int iID )
{
	m_pQuestList->Hide();

	if( iID < 100 )
	{
		// Available_Quest		
		if( iID <= m_vecAvailableQuest.size() )
		{
			int iQuestID = m_vecAvailableQuest[ ( iID - 1 ) ];

			// 呼叫 QuestDetail 秀出完整的界面
			g_pGameMain->OnQuestDetail( iQuestID, 0 );
		}
	}
	else
	{
		if( iID < 200 )
		{
			// Current Quest
			int index = iID - 101;

			if( index <= m_vecCurrentQuest.size() )
			{
				int iQuestID = m_vecCurrentQuest[ index ];
				g_pGameMain->OnQuestDetail( iQuestID, 1 );
			}
		}
		else
		{
			int index = iID - 201;

			if( index <= m_vecCurrentQuest.size() )
			{
				int iQuestID = m_vecCurrentQuest[ index ];
				g_pGameMain->OnQuestDetail( iQuestID, 2 );
			}
		}
	}
}
//--------------------------------------------------------------------------------------
void CUI_QuestList::OnLeave_InteractRange		( CControlInteractUI* pUI, CRoleSprite* pRoleObj )
{
	if( m_pQuestList )
		m_pQuestList->Hide();
}
*/