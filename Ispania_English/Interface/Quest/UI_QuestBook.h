#pragma once

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

class CUI_QuestBook;
extern CUI_QuestBook*		g_pQuestBook;

struct Struct_QuestNPC
{
	StaticString< _MAX_NAMERECORD_SIZE_ >	Name;
	float	x,y,z;
};

class CUI_QuestBook : public CInterfaceBase //{//: // public CControlInteractUI
{
public:
	CUI_QuestBook	( CInterface* pObject );
	~CUI_QuestBook	( void );

	virtual	void				RegisterFunc			();	
	virtual bool				CheckQuestBeginLua		( const char* pszScript );
	virtual string				GetLuaStringResult		( const char* pszScript );
	virtual int					GetQuestGUID			( int iIndex );

	virtual	void				Save();	
	virtual	void				Load();
	virtual void				SaveClientData();
	virtual void				LoadClientData(int type, int size, char* data);

	virtual void				DestroyFrames();			// UI ROOT 將被刪除

	virtual void				UpdateQuestItem(int iQuestID);			//Server指令刪除任務時,檢查是否存在追蹤 並刪除
	vector< int >				m_vecQuestID;
	set< int >					m_setTrackID;	            // 紀錄正在追蹤的任務, 記陸內容為 m_vecQuestID 的順序 ( Index )
};
