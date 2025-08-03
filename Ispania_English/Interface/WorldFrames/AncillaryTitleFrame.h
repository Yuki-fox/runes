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



// �d�������ƶq
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
	virtual	void			SetATF_UseItem();		 //��z�ŦX���I�]���~���
	virtual	void			ClearATF_UseItem();		 //�M���D�n����ؼ�
	virtual	void			AddATF_UseItem(int Idx); //�[�J����ؼ�
	virtual	void			OkayATF_UseItem(int Idx);
	virtual	void			UpdateATF_UseItem();
	virtual	void			UpdateATF_TitleFrame();


	
protected:
	bool					m_IsVisible;
	int						m_TitleID[4];			//����,�D��,�t��,����
	ATFUseItem				m_ATFUseItem[10];		//�ΨӴ��ɪ����~   //0 ��ܥثe��m�������
};
