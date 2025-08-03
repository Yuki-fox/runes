#pragma once

#include "../interface.h"

#include <map>
#include <string>

class CUI_ScriptBorder;
extern CUI_ScriptBorder*		g_pScriptBorder;

class CUI_ScriptBorder : public CInterfaceBase //{//: // public CControlInteractUI
{
public:
	CUI_ScriptBorder				( CInterface* pObject );
	~CUI_ScriptBorder				( void );

	virtual	void					RegisterFunc	();

	virtual void					Clear();
	virtual	void					ShowBorder();	

	vector< string >				m_vecBorderPage;
	string							m_sTexture;
	string							m_sTitle;
	int								m_iBorderQuestID;
};
