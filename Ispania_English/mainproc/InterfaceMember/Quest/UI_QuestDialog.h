#pragma once

#include "../../GameMain.h"

#include <map>
#include <string>

#define Register_UILua_QuestDialog    \
	g_pLuaScript->RegisterFunc(	"OnClick_ProcessQuest",				void ( void ), CUI_QuestDialog::OnClick_ProcessQuest		);\


/*
#define Register_UILua_Quest    \
	g_pLuaScript->RegisterFunc(	"UI_CharStatus_OnLoad",		void (const char*), CUI_CharStatus::UI_CharStatus_OnLoad		);\
	g_pLuaScript->RegisterFunc(	"UI_CharStatus_OnUpdate",	void (), CUI_CharStatus::UI_CharStatus_OnUpdate		);\
*/

/*
	界面主要用途:
		a. 秀出 NPC Quest 先前導引
		b. 秀出 Quest 清單
*/

// Globals
//-------------------------------------------------------------------
class	CUI_QuestDialog;
extern	CUI_QuestDialog							*g_pUi_QuestDialog;
//-------------------------------------------------------------------

class CUI_QuestDialog
{
public:
	CUI_QuestDialog(void);
	~CUI_QuestDialog(void);

	static void				Initial						();
	static void				Release						();

	static void				ReSet						();

	static void				Set_QuestID					( int iQuestID, int iButtonMode );
		// 0 Accept, 1 Complete( Disbale ), 2 Complete, 3 刪除
	static void				Set_NpcName					( const char* pszNpcName	);
	static void				Set_QuestName				( const char* pszQuestName	);
	static void				Set_QuestDesc				( const char* pszQuestDesc	);
	static void				Set_QuestDetail				( const char* pszQuestDetail);
		

	//static void				Add_RequestKill				( int iTargetID, int iTargetVal );
	static void				Add_RequestItem				( int iItemID, int iItemVal );



	
	static void				Add_RewardItem				( const char* pszQuestDesc );

	static void				Add_Current_Quest			( int iQuestID );
	static void				Show						();

	// 讓介面呼叫的 Lua 函式
	static void				OnClick_ProcessQuest					( void );

	//static string			m_sNpcNAme;
	static int				m_iQuestMode;
	static int				m_iQuestID;
	static CLayout*			m_pQuestDialog;
	static CFontString*		m_pQuestFrameNpcNameText;
	static CFontString*		m_pQuestFrame_QuestName;
	static CFontString*		m_pQuestFrame_QuestDesc;
	static CFontString*		m_pQuestFrame_QuestDetail;
	static CButton*			m_pQuestFrame_ProcessButton;
	static CButton*			m_pQuestFrame_LeaveButton;

};

