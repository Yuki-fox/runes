#pragma once

#include "../interface.h"

//#include "../Control_Utility.h"
#include <set>
#include <map>
#include <vector>
#include <string>

/*
#define Register_UILua_QuestNPCList    \
	g_pLuaScript->RegisterFunc(	"GetQuestSelectButtonMode",	int (int),		CUI_QuestNPCList::GetQuestSelectButtonMode	);\
	g_pLuaScript->RegisterFunc(	"SelectQuest",				void (int),		CUI_QuestNPCList::SelectQuest	);\
	g_pLuaScript->RegisterFunc(	"OnLoad_QuestNPCList",		void (),		CUI_QuestNPCList::OnLoad_QuestNPCList	);\
*/

class CUI_CraftFrame;
extern CUI_CraftFrame*		g_pCraftFrame;

struct StructCraftType
{
	int							iCraftSkillID;
	string						sSkillName;
	//vector< int >	vecItem;
	//multimap< int, int >		mapCraftItem;
	vector<int>					vecCraftItem;
	//vector<string>				vecSubType;
	//set<int>					setSubType;
	vector<int>					vecSubType;
};

class CUI_CraftFrame : public CInterfaceBase //{//: // public CControlInteractUI
{
public:
	CUI_CraftFrame	( CInterface* pObject );
	~CUI_CraftFrame	( void );

	virtual	void		RegisterFunc			();
	virtual	void		Update					(float elapsedTime);
	
	int					GetMaxCraftItem			( int iType, int iSubType, int iIndex );
	int					GetMaxCraftItem			( int iObjGUID );
	bool				CreateCraftItem			( int iObjGUID, int iNumber, int iFrom );
	void				StopCreateCraftItem		();
	int					GetCraftItemGUID		( int iType, int iIndex );

	int					GetCraftItemGUID		( int iRecipteObjID );
	int					GetCraftRequestItemGUID ( int iRecipteObjID, int iCraftRequestItemIndex );

	static int			GetRecipteInfo			( int iCraftType, int iCraftSubType, int iCraftItemIndex );
		// iCraftItemIndex	�ǤJ�ƭ�	�|�o�쪫��
		//					�ǤJ -1		�|�o���`�q

	static void			GetCraftItemList		( int iCraftType, int iCraftSubType, vector<int>*	pvecCraftItem );

	bool				CheckRecipte			( int iRecipteObjID );
	bool				CheckRequestItem		( int iRequestItemID );

	void				ResetCraftInfo			();

	void				PlayCraftSound			( int iCraftType, int iObjID );
	void				StopCraftSound			();

	void				SetCraftItemField		( bool bIsValid, int iItemGUID = 0, int iButtonID = 0 );
	ItemFieldStruct*	GetCraftItemField		();

	int					GetRecipeGUID			( int iCraftObjID );
	void				GetRuneHoleTooltip		( char *szText, int iRecpiteGUID );

	void				OpenCraftUI				( int lifeSkillID );

	float				CaluateCraftTime		( RoleDataEx* pRole, GameObjDbStruct* pObj );



	


	vector< StructCraftType >	m_vecCraftType;

	int							m_iCraftItem;			// �Ҩ�y�����~

	int							m_iCraftItemNumber;		// �ѤU�n��y���ƶq
	int							m_iCraftItemTotal;

	float						m_fTimeToCreate;		// �ѤU�h�֮ɶ��ᰵ�X���~
	int							m_iCraftSound;			// �����y����
		
	ItemFieldStruct				m_itemField;			// �O��Tooltip�n�Ϊ��}���׸�T
	bool						m_bIsValid;				// ����GetCraftItemField�O�_�i�H���o���Ъ��X��

	int							m_iFromQueue;			// �O���Ͳ��n�D�O�_�Ӧ۱Ƶ{����
};
