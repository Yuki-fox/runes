#pragma once

#include "../interface.h"

class CBotCheckFrame;
extern CBotCheckFrame*			g_pBotCheckFrame;

class CBotCheckFrame : public CInterfaceBase
{
public:
	CBotCheckFrame(CInterface* object);
	virtual ~CBotCheckFrame();

	virtual	void				RegisterFunc();
	virtual void				BuildFrames();
	virtual void				DestroyFrames();

	virtual void				Open();
	virtual void				Close();
	virtual void				CheckText();
protected:

	CRuFontType*				m_fontType;
	CUiFrame*					m_botCheckFrame;				// ���~��
	CUiFontString*				m_explainText;					// ������r
	CUiFontString*				m_checkSerialText;				// �{���X
	CUiEditBox*					m_editbox;						// ��J��
	CUiButton*					m_buttonOkey;					// �T�w
};