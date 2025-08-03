#pragma once

#include "../interface.h"

// ************************************************************************************************************************************************************

class CGM_Config;
extern CGM_Config*	g_pGM_Config;

// ************************************************************************************************************************************************************

class CGM_Config : public CInterfaceBase
{
protected:
	CUiFrame*					m_UI_Win;

	CUiCheckButton*				m_UI_ShowHiddenCheck;

	BOOL						m_visibleOnLastUpdate;

public:
								CGM_Config(CInterface* object);
	virtual						~CGM_Config();

	virtual	void				RegisterFunc();
	virtual	bool				IsVisible();

	virtual	void				OnLoad();
	virtual	void				OnUpdate();

	virtual	void				OnApply();
	virtual	void				OnClose();
};

// ************************************************************************************************************************************************************
