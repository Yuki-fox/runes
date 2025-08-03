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

class CUI_QuestDetail;
extern CUI_QuestDetail*		g_pQuestDetail;

class CUI_QuestDetail : public CInterfaceBase //{//: // public CControlInteractUI
{
public:
	CUI_QuestDetail	( CInterface* pObject );
	~CUI_QuestDetail	( void );
	
	//------------------------------------------------
	virtual	void			RegisterFunc		();
	//------------------------------------------------
	virtual void			SetQuest			( int iMode, int iIndex, int iQuestID );	// iMode 0 from NPC; 1 Book
	//------------------------------------------------
	virtual int				GetQuestRequest		( int iQuestID );
	//------------------------------------------------
	virtual	void			GetItemQueue		( int iItemID, int iCount );
	//------------------------------------------------
	virtual string			ReplaceKeyWord		( string pSource , bool bHyperlink = false );
	//------------------------------------------------
	virtual	void			InitKeyWorld		();
	//------------------------------------------------
	virtual string			GetLuaStringResult	( const char* pszScript, int iQuestID, int iItemGUID, int iCount, int iMaxCount, int iResult );

	/*
	virtual void			ResetSpeak		();
	virtual void			SetSpeakDetail	( const char* szString );
	virtual void			AddSpeakOption	( const char* szString );
	*/
	//------------------------------------------------
	int						m_iMode;
	int						m_iStatus;
	int						m_iQuestID;
	int						m_iQuestNPC;         //接任務NPC
	int						m_iRequestQuestNPC;	 //回報任務NPC
	int						m_iRewardItemChoiceID;
	bool					m_isComplete;
	string					m_sName;
	string					m_sDesc;
	string					m_sDetail;	

	int						m_iLV;
	int						m_iExp;
	int						m_iTP;
	int						m_iMoney;



	struct	StructQuestItem
	{
		int					iItemID;
		int					iItemVal;
		string				sName;
		string				sIconPath;
	};
	
	vector< StructQuestItem >	m_vecQuestItem[3];	// 1 Available; 2 Uncomplete; 3 Complete


	bool						m_bInitLuaColorKeyWord;
	map< string, string >		m_mapColorKeyWord;

};
