#pragma once

#include "../interface.h"

//************************************************************************************************************************************************************
class CQuestTrackNew;
extern CQuestTrackNew* g_pQuestTrackNew;
//************************************************************************************************************************************************************

class CQuestTrackNew : public CInterfaceBase
{

protected:
	CUiFrame*					m_UI_Win;

	CUiSlider*					m_UI_TypefaceFont;

	CUiCheckButton*				m_UI_QuestinsPutTopCheck;
	CUiCheckButton*				m_UI_QuestTrackAutomaticCheck;
		
	BOOL						m_visibleOnLastUpdate;
public:
								CQuestTrackNew( CInterface* pObject );
	virtual						~CQuestTrackNew( void );

	virtual	void				RegisterFunc();
	virtual	bool				IsVisible();


};
