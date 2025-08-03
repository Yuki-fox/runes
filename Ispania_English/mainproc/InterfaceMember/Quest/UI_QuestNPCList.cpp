#include ".\UI_QuestNPCList.h"
#include "../../netmember/NetScript.h"

// 全域宣告
//-------------------------------------------------------------------
CUI_QuestNPCList			*g_pUi_QuestNPCList = NULL;

CRoleSprite*			CUI_QuestNPCList::m_InteractNPC;

CButton*				CUI_QuestNPCList::m_pQuestSelectButton		[ 10 ];
//CButton*				CUI_QuestNPCList::m_pAvailableQuestButton		[ 5 ];
//CButton*				CUI_QuestNPCList::m_pCurrentQuestButton		[ 5 ];


CFontString*			CUI_QuestNPCList::m_pPanelTitle;
CFontString*			CUI_QuestNPCList::m_pNPC_Text;				//QuestListFrame_NPC_GreetingText

int						CUI_QuestNPCList::m_iQuestIndex = 0;

CLayout*				CUI_QuestNPCList::m_pQuestNPCList = NULL;

vector< int >			CUI_QuestNPCList::m_vecQuestID;
vector< int >			CUI_QuestNPCList::m_vecQuestMode;


//-------------------------------------------------------------------
CUI_QuestNPCList::CUI_QuestNPCList(void)
{

}
// --------------------------------------------------------------------------------------
CUI_QuestNPCList::~CUI_QuestNPCList(void)
{

}
// --------------------------------------------------------------------------------------
void CUI_QuestNPCList::Initial()
{
	if ( g_pUi_QuestNPCList == NULL )
	{
		g_pUi_QuestNPCList = new CUI_QuestNPCList;
	}
}
// --------------------------------------------------------------------------------------
void CUI_QuestNPCList::Release()
{
	SAFE_DELETE( g_pUi_QuestNPCList );	
}
//--------------------------------------------------------------------------------------
void CUI_QuestNPCList::Add_NpcText					( const char* pszNpcText )
{
	if( m_pQuestNPCList )
	{ 
		m_pNPC_Text->SetTextA( pszNpcText );
	}
}
//--------------------------------------------------------------------------------------
void CUI_QuestNPCList::Add_Available_Quest			( int iQuestID, const char* pszQuestName )
{
	if( m_pQuestNPCList )
	{ 
		int Index = m_vecQuestID.size();
		m_vecQuestID.push_back( iQuestID );
		m_vecQuestMode.push_back( 0 );
		//m_pAvailableQuestButton_Text[ m_iQuestIndex ]->SetTextA( pszQuestName );;

		m_pQuestSelectButton[ Index ]->Show();
		m_pQuestSelectButton[ Index ]->SetTextA( pszQuestName );
	}
}
//--------------------------------------------------------------------------------------
void CUI_QuestNPCList::Add_Current_Quest			( int iQuestID, const char* pszQuestName )
{
	if( m_pQuestNPCList )
	{
		int Index = m_vecQuestID.size();
		m_vecQuestID.push_back( iQuestID );
		m_vecQuestMode.push_back( 1 );
		//m_pAvailableQuestButton_Text[ m_iQuestIndex ]->SetTextA( pszQuestName );;
		m_pQuestSelectButton[ Index ]->Show();
		m_pQuestSelectButton[ Index ]->SetTextA( pszQuestName );
	}
}
//--------------------------------------------------------------------------------------
void CUI_QuestNPCList::Add_Complete_Quest			( int iQuestID, const char* pszQuestName )
{
	if( m_pQuestNPCList )
	{ 
		char szQuestName[64];
		sprintf( szQuestName, "%s ( Complete )", pszQuestName );

		int Index = m_vecQuestID.size();
		m_vecQuestID.push_back( iQuestID );
		m_vecQuestMode.push_back( 2 );
		//m_pAvailableQuestButton_Text[ m_iQuestIndex ]->SetTextA( pszQuestName );;
		m_pQuestSelectButton[ Index ]->Show();
		m_pQuestSelectButton[ Index ]->SetTextA( szQuestName );
	}
}
//--------------------------------------------------------------------------------------
void CUI_QuestNPCList::Show						()
{
	if( m_pQuestNPCList )
	{
		int Index = m_vecQuestID.size();
		m_vecQuestID.push_back( 0 );
		m_vecQuestMode.push_back( -1 );

		//m_pAvailableQuestButton_Text[ m_iQuestIndex ]->SetTextA( pszQuestName );;
		m_pQuestSelectButton[ Index ]->Show();
		m_pQuestSelectButton[ Index ]->SetTextA( "大爺現在沒空!!下次再找你聊天." );
	}

	if( m_pQuestNPCList )
	{
		SetInteractRangeOn( g_pUi_QuestNPCList, m_InteractNPC );
		m_pQuestNPCList->Show();
	}
}
//--------------------------------------------------------------------------------------
void CUI_QuestNPCList::ReSet						()
{

	m_iQuestIndex = 0;

	m_vecQuestID.clear();
	m_vecQuestMode.clear();	

	if( m_pQuestNPCList  )
	{
		for( int Index = 0; Index < 10; Index++ )
		{
			m_pQuestSelectButton[ Index ]->Hide();
		}
	}
}

void CUI_QuestNPCList::Set_NpcName ( const char* pszNpcName )
{
	if( m_pPanelTitle )
		m_pPanelTitle->SetTextA( pszNpcName );
}

void CUI_QuestNPCList::SelectQuest					( int iID )
{
	m_pQuestNPCList->Hide();

	if( iID <= m_vecQuestID.size() )
	{
		int iQuestID	= m_vecQuestID[ ( iID - 1 ) ];
		int	iQuestMode	= m_vecQuestMode[ ( iID - 1 ) ];

		if( iQuestMode != -1 ) 
			g_pGameMain->OnQuestDetail( iQuestID, iQuestMode );
	}
}
//--------------------------------------------------------------------------------------
void CUI_QuestNPCList::OnLeave_InteractRange		( CControlInteractUI* pUI, CRoleSprite* pRoleObj )
{
	if( m_pQuestNPCList )
		m_pQuestNPCList->Hide();
}
//--------------------------------------------------------------------------------------
void CUI_QuestNPCList::OnLoad_QuestNPCList		()
{
	if( m_pQuestNPCList == NULL )
	{
		m_pQuestNPCList = CLayout::GetLayout( "UI_QuestNPCList" );

		if( m_pQuestNPCList ) 
		{			
			char szObjName[ 255 ];

			for( int Index = 0; Index <= 10; Index++ )
			{
				sprintf( szObjName,			"QuestSelectButton_%d", ( Index + 1 ) );
				m_pQuestSelectButton[ Index ]	= (CButton*)CLayout::GetLayout( szObjName );
				m_pQuestSelectButton[ Index ]->Hide();
			}

			m_pNPC_Text			= (CFontString*)CLayout::GetLayout( "UI_QuestNPCList_Text" );
			m_pPanelTitle		= (CFontString*)CLayout::GetLayout( "UI_QuestNPCList_Panel_Name" );
		}

	}
}
//--------------------------------------------------------------------------------------
int	CUI_QuestNPCList::GetQuestSelectButtonMode				( int iID )
{
	if( iID <= m_vecQuestID.size() )
	{
		int	iQuestMode	= m_vecQuestMode[ ( iID - 1 ) ];

		return iQuestMode;
	}
	else
		return -1;

}