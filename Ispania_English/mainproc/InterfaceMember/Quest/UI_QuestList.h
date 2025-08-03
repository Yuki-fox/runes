#pragma once

#include "../../GameMain.h"

#include "../Control_Utility.h"
#include <map>
#include <string>


#define Register_UILua_QuestList    \
	g_pLuaScript->RegisterFunc(	"OnLoad_QuestListBook",				void ( void ),	CUI_QuestList::OnLoad_QuestListBook		);\
	g_pLuaScript->RegisterFunc(	"OnShow_QuestListBook",				void ( void ),	CUI_QuestList::OnShow_QuestListBook		);\
	g_pLuaScript->RegisterFunc(	"OnClick_QuestList_Button",			void ( int ),	CUI_QuestList::OnClick_QuestList_Button	);\
	

// Globals
//-------------------------------------------------------------------
class	CUI_QuestList;
extern	CUI_QuestList							*g_pUi_QuestList;
//-------------------------------------------------------------------


//class CUI_QuestList : public CControlInteractUI
class CUI_QuestList
{
public:
	CUI_QuestList(void);
	~CUI_QuestList(void);

	static	void			Initial						();
	static	void			Release						();

	// LUA STATIC 
	//---------------------------------------------------------------------
	static	void			OnLoad_QuestListBook		();
	static	void			OnShow_QuestListBook		();
	static	void			OnClick_QuestList_Button	( int iID );

	//---------------------------------------------------------------------
	void					ReSet						();
	vector< int >			m_vecQuestID;
	CLayout*				m_pQuestList;
	CButton*				m_pQuestSelectButton		[ 10 ];

	static CUI_QuestList*	m_pThis;
};

