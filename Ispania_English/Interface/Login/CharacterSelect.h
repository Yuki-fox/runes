#pragma once

#include <map>
#include "../interface.h"

using namespace std;

class CCharacterSelect;
extern CCharacterSelect*		g_pCharacterSelect;

class CRoleSprite;
class CCharacterSelect : public CInterfaceBase
{
public:
	CCharacterSelect(CInterface* object);
	virtual ~CCharacterSelect();	

	virtual	void				RegisterFunc();
	virtual	void				Open();
	virtual	void				Close();	

	virtual	void				LoadCharacterStart();
	virtual	void				LoadCharacterEnd();
	virtual	void				LoadCharacterEvent( int iEvent, int iValue );
	
	virtual	void				SetSelected(int index);
	virtual	int					GetSelected()							{ return m_selected; }		
	virtual	CRoleSprite*		GetRoleSprite()							{ return m_sprite; }
	virtual	CRoleSprite*		GetCharacterSprite()					{ return m_sprite; }
	virtual	void				UpdateCharacterList();
	virtual	void				UpdateSprite();
	virtual	void				UpdateParalleZone();

	virtual	void				UpdateCamera();
	virtual void				SetPromptState(bool state)				{ m_boPrompt = state; }
	virtual bool				GetPromptState()						{ return m_boPrompt; }

protected:
	CRoleSprite*				m_sprite;
	int							m_selected;				// 目前選擇角色
	bool						m_waitQueue;
	bool						m_boPrompt;				// 在登入遊戲的創角/選角畫面(進入遊戲之前)；對玩家的公告說明。內容大概是告知玩家："只要有合理懷疑玩家有任何不當作弊行為；私訊是會被提出來檢閱的。"
};