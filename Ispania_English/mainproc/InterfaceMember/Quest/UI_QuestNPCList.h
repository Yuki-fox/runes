#pragma once

#include "../../GameMain.h"

#include "../Control_Utility.h"
#include <map>
#include <string>


#define Register_UILua_QuestNPCList    \
	g_pLuaScript->RegisterFunc(	"GetQuestSelectButtonMode",	int (int),		CUI_QuestNPCList::GetQuestSelectButtonMode	);\
	g_pLuaScript->RegisterFunc(	"SelectQuest",				void (int),		CUI_QuestNPCList::SelectQuest	);\
	g_pLuaScript->RegisterFunc(	"OnLoad_QuestNPCList",		void (),		CUI_QuestNPCList::OnLoad_QuestNPCList	);\


/*
	界面主要用途:
		a. 秀出 NPC Quest 先前導引
		b. 秀出 Quest 清單
*/

// Globals
//-------------------------------------------------------------------
class	CUI_QuestNPCList;
extern	CUI_QuestNPCList							*g_pUi_QuestNPCList;
//-------------------------------------------------------------------


class CUI_QuestNPCList : public CControlInteractUI
{
public:
	CUI_QuestNPCList(void);
	~CUI_QuestNPCList(void);

	static	void			Initial						();
	static	void			Release						();
	static	void			OnLoad_QuestNPCList			();

	//---------------------------------------------------------------------
	static	void			ReSet						();

	static	void			Set_InteractNPC				( CRoleSprite* pRole ) { m_InteractNPC = pRole; }
	static	void			Set_NpcName					( const char* pszNpcName );
	static	void			Add_NpcText					( const char* pszNpcText );

	static	void			Add_Available_Quest			( int iQuestID, const char* pszQuestName );
	static	void			Add_Current_Quest			( int iQuestID, const char* pszQuestName );
	static	void			Add_Complete_Quest			( int iQuestID, const char* pszQuestName );

	static	void			Show						();
	
	static	void			SelectQuest					( int iID );
	static	int				GetQuestSelectButtonMode	( int iID );



	virtual void			OnLeave_InteractRange		( CControlInteractUI* pUI, CRoleSprite* pRoleObj );

	static vector< int >	m_vecQuestID;
	static vector< int >	m_vecQuestMode;

	static CLayout*			m_pQuestNPCList;
	static CButton*			m_pQuestSelectButton		[ 10 ];
	static CFontString*		m_pNPC_Text;				//QuestListFrame_NPC_GreetingText
	static CFontString*		m_pPanelTitle;

	static int				m_iQuestIndex;
	static CRoleSprite*		m_InteractNPC;


};

