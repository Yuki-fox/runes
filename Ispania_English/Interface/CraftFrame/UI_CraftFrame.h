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
		// iCraftItemIndex	傳入數值	會得到物件
		//					傳入 -1		會得到總量

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

	int							m_iCraftItem;			// 所制造的物品

	int							m_iCraftItemNumber;		// 剩下要制造的數量
	int							m_iCraftItemTotal;

	float						m_fTimeToCreate;		// 剩下多少時間後做出物品
	int							m_iCraftSound;			// 播放制造音效
		
	ItemFieldStruct				m_itemField;			// 記錄Tooltip要用的稀有度資訊
	bool						m_bIsValid;				// 做為GetCraftItemField是否可以取得指標的旗標

	int							m_iFromQueue;			// 記錄生產要求是否來自排程介面
};
