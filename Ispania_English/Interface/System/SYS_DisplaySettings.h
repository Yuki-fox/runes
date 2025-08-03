#pragma once

#include "../interface.h"

// ************************************************************************************************************************************************************

class CSYS_DisplaySettings;
extern CSYS_DisplaySettings*	g_pSYS_DisplaySettings;

// ************************************************************************************************************************************************************

class CSYS_DisplaySettings : public CInterfaceBase
{
protected:
	CRuArrayList<RuDisplayMode>	m_displayModes;
	BOOL						m_resolutionNeedChange;
	BOOL						m_NeedToBeWindowed;

public:
								CSYS_DisplaySettings(CInterface* object);
	virtual						~CSYS_DisplaySettings();

	virtual	void				RegisterFunc();

	virtual	void				OnShow();
	virtual void				OnApply();

	INT32						GetNumDisplayModes();
	INT32						GetDisplayModeWidth(INT32 idx);
	INT32						GetDisplayModeHeight(INT32 idx);
	INT32						GetDisplayModeRefreshRate(INT32 idx);

	void						SetResolutionNeedChange(BOOL needChange);
	void						SetNeedToBeWindowed(BOOL beWindowed);
};

// ************************************************************************************************************************************************************
