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

	virtual void				DestroyFrames();			// UI ROOT �N�Q�R��

	virtual void				UpdateQuestItem(int iQuestID);			//Server���O�R�����Ȯ�,�ˬd�O�_�s�b�l�� �çR��
	vector< int >				m_vecQuestID;
	set< int >					m_setTrackID;	            // �������b�l�ܪ�����, �O�����e�� m_vecQuestID ������ ( Index )
};
