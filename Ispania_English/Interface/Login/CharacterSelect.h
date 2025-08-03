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
	int							m_selected;				// �ثe��ܨ���
	bool						m_waitQueue;
	bool						m_boPrompt;				// �b�n�J�C�����Ш�/�﨤�e��(�i�J�C�����e)�F�缾�a�����i�����C���e�j���O�i�����a�G"�u�n���X�z�h�ê��a�����󤣷�@���欰�F�p�T�O�|�Q���X���˾\���C"
};