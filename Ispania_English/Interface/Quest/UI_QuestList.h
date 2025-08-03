#pragma once

//	界面主要用途:
//		a. 顯示 NPC 任務對話
//		b. 秀出 Quest 清單( 包含判斷可接, 不可接, 已完成 )

#include "../interface.h"

//#include "../Control_Utility.h"
#include <map>
#include <vector>
#include <string>

/*
#define Register_UILua_QuestNPCList    \
	g_pLuaScript->RegisterFunc(	"GetQuestSelectButtonMode",	int (int),		CUI_QuestNPCList::GetQuestSelectButtonMode	);\
	g_pLuaScript->RegisterFunc(	"SelectQuest",				void (int),		CUI_QuestNPCList::SelectQuest	);\
	g_pLuaScript->RegisterFunc(	"OnLoad_QuestNPCList",		void (),		CUI_QuestNPCList::OnLoad_QuestNPCList	);\
*/

class CUI_QuestList;
extern CUI_QuestList*		g_pQuestList;

class CUI_QuestList : public CInterfaceBase //{//: // public CControlInteractUI
{
public:
	CUI_QuestList	( CInterface* pObject );
	~CUI_QuestList	( void );

	//static	void		Initial						();
	//static	void		Release						();
	//static	void		OnLoad_QuestNPCList			();
	virtual	void			RegisterFunc();	
	virtual	void			Update(float elapsedTime);
	//---------------------------------------------------------------------
	//static	int				LuaFunc_OnLoad_QuestList	( lua_State *L );
	//---------------------------------------------------------------------
	virtual void			SetQuestNPC					( int iObjID );
	virtual int				GetQuestNPC					() { return m_iQuestNPC; }
	virtual void			Clear						();

	//virtual	void			ReSet						();

	//virtual	void			Set_InteractNPC				( CRoleSprite* pRole ) { m_InteractNPC = pRole; }
	/*
	virtual	void			Set_NpcName					( const char* pszNpcName );
	virtual	void			Add_NpcText					( const char* pszNpcText );

	virtual	void			Add_Available_Quest			( int iQuestID, const char* pszQuestName );
	virtual	void			Add_Current_Quest			( int iQuestID, const char* pszQuestName );
	virtual	void			Add_Complete_Quest			( int iQuestID, const char* pszQuestName );

	virtual	void			Show						();
	
	virtual	void			SelectQuest					( int iID );
	virtual	int				GetQuestSelectButtonMode	( int iID );
	*/

	//virtual void			OnLeave_InteractRange		( CControlInteractUI* pUI, CRoleSprite* pRoleObj );
	/*
	vector< int >			m_vecQuestID;
	vector< int >			m_vecQuestMode;

	CUiLayout*				m_pQuestNPCList;
	CUiButton*				m_pQuestSelectButton		[ 10 ];
	CUiFontString*			m_pNPC_Text;				//QuestListFrame_NPC_GreetingText
	CUiFontString*			m_pPanelTitle;
	int						m_iQuestIndex;
	*/

	int						m_iQuestNPC;
	vector< int >			m_vecQuestID[3];	// 1 Available ; 2 UnComplete; 3 Complete

	string					m_sText;
	//CRoleSprite*			m_InteractNPC;
};
