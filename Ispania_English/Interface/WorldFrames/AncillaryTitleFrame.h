#pragma once
#include "../interface.h"
#include <string>
#include <map>
#include <vector>
#include "BagFrame.h"
#include "../CraftFrame/UI_CraftFrame.h"
#include "../../netwaker_member/net_other/NetCli_OtherChild.h"
//#include "../CraftFrame/UI_CraftFrame.h"

//******************************************************************************
class CAncillaryTitleFrame;
extern CAncillaryTitleFrame*	g_pAncillaryTitleFrame;



// 寵物食物數量
struct ATFUseItem
{
	int count;
	int ID; 
	int pos;
};



using namespace std;

class CAncillaryTitleFrame : public CInterfaceBase
{
public:
	CAncillaryTitleFrame( CInterface* pObject );
	virtual				~CAncillaryTitleFrame();

	virtual	void			RegisterFunc();	
	virtual	void			Open();	
	virtual	void			Close();	
	virtual	void			Update(float elapsedTime);

	virtual	void			SetATFItem();
	virtual void			R_SetNewTitleSysResult( bool Result );


	virtual int 			GetATFItem( int Idx ){ return m_TitleID[Idx] ;};

	virtual	void			SetATFUseItemID(int Idx,int GUID){ g_pAncillaryTitleFrame->m_ATFUseItem[Idx].ID = GUID;};
	virtual	void			SetATFUseItemCount(int Idx,int Count){ g_pAncillaryTitleFrame->m_ATFUseItem[Idx].count = Count;};
	virtual	void			SetATFUseItemPos(int Idx,int Pos){ g_pAncillaryTitleFrame->m_ATFUseItem[Idx].pos = Pos;};

	virtual int 			GetATFUseItemPos( int idx ){ return g_pAncillaryTitleFrame->m_ATFUseItem[idx].pos;};
	virtual	int				GetATFUseItemID(int Idx){ return g_pAncillaryTitleFrame->m_ATFUseItem[Idx].ID;};
	virtual	int				GetATFUseItemCount(int Idx){ return g_pAncillaryTitleFrame->m_ATFUseItem[Idx].count;};


	virtual void			R_NewTitleSys_UseItemResult( NewTitleSysUseItemResultENUM Result );
	virtual	void			SetATF_UseItem();		 //整理符合的背包物品資料
	virtual	void			ClearATF_UseItem();		 //清掉主要選取目標
	virtual	void			AddATF_UseItem(int Idx); //加入選取目標
	virtual	void			OkayATF_UseItem(int Idx);
	virtual	void			UpdateATF_UseItem();
	virtual	void			UpdateATF_TitleFrame();


	
protected:
	bool					m_IsVisible;
	int						m_TitleID[4];			//任務,挑戰,系統,活動
	ATFUseItem				m_ATFUseItem[10];		//用來提升的物品   //0 表示目前位置內的資料
};
