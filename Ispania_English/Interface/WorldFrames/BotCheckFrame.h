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
	CUiFrame*					m_botCheckFrame;				// 原件外框
	CUiFontString*				m_explainText;					// 說明文字
	CUiFontString*				m_checkSerialText;				// 認証碼
	CUiEditBox*					m_editbox;						// 輸入框
	CUiButton*					m_buttonOkey;					// 確定
};