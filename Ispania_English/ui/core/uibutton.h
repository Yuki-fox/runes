#pragma once

#include "uiframe.h"

// --------------------------------------------------------------------------------------
class CUiLuaButton
{
public:
	static	void			OpenLib(lua_State* L);
	static	int				SetMetaTable(lua_State* L);
};

// --------------------------------------------------------------------------------------
class CUiButton : public CUiFrame
{
public:
	CUiButton(const char* name, CUiLayout* pParent, CUi* pUi);
	virtual ~CUiButton();

	virtual void			SetMouseEnable(bool isEnable)			{ m_state.isMouseEnable = true; }

	virtual	void			SetScrollParent(CUiLayout* layout);
	virtual	void			SetNormalTexture(CUiTexture* texture);
	virtual	void			SetPushedTexture(CUiTexture* texture);
	virtual	void			SetDisabledTexture(CUiTexture* texture);
	virtual	void			SetHighlightTexture(CUiTexture* texture);
	virtual	void			SetNormalText(CUiFontString* fontString);
	virtual	void			SetDisabledText(CUiFontString* fontString);
	virtual	void			SetHighlightText(CUiFontString* fontString);
	virtual	void			SetText(const WCHAR* text);
	virtual	void			SetTextA(const char* text);
	virtual	float			GetTextWidth();
	virtual	float			GetTextHeight();
	virtual void			SetPushTextOffset(CUiPoint& offset)		{ m_pushedTextOffset = offset; }
	virtual	CUiTexture*		GetNormalTexture()						{ return m_normalTexture; }
	virtual	CUiTexture*		GetPushedTexture()						{ return m_pushedTexture; }
	virtual	CUiTexture*		GetDisabledTexture()					{ return m_disabledTexture; }
	virtual	CUiTexture*		GetHighlightTexture()					{ return m_highlightTexture; }
	virtual	CUiFontString*	GetNormalText()							{ return m_normalText; }
	virtual	CUiFontString*	GetDisabledText()						{ return m_disabledText; }
	virtual	CUiFontString*	GetHighlightText()						{ return m_highlightText; }
	virtual CUiPoint		GetPushTextOffset()						{ return m_pushedTextOffset; }
	virtual	void			SetLuaMetaTable(lua_State* l)			{ CUiLuaButton::SetMetaTable(l); }
	virtual	void			LockHighlight()							{ m_lockHighlight = true; }
	virtual	void			UnlockHighlight()						{ m_lockHighlight = false; }
	virtual	void			LockPushed()							{ m_lockPushed = true; }
	virtual	void			UnlockPushed()							{ m_lockPushed = false; }
	virtual	void			SetTextAnchor(CUiAnchor anchor);
	virtual	void			SetTextColor(CUiColor color);
	virtual	void			SetNormalTextColor(CUiColor color)		{ m_normalTextColor = color; }
	virtual	void			SetDisabledTextColor(CUiColor color)	{ m_disabledTextColor = color; }
	virtual	void			SetHighlightTextColor(CUiColor color)	{ m_highlightTextColor = color; }	
	virtual int				GetMemSize();

protected:

	virtual	void			OnUpdate(float elapsedTime);
	virtual	void			OnPaint(float alpha);

protected:
	CUiTexture*				m_normalTexture;
	CUiTexture*				m_pushedTexture;
	CUiTexture*				m_disabledTexture;
	CUiTexture*				m_highlightTexture;
	CUiFontString*			m_normalText;
	CUiFontString*			m_disabledText;
	CUiFontString*			m_highlightText;
	CUiPoint				m_pushedTextOffset;
	CUiAnchor				m_textAnchor;
	CUiColor				m_normalTextColor;
	CUiColor				m_disabledTextColor;
	CUiColor				m_highlightTextColor;
	bool					m_lockHighlight;
	bool					m_lockPushed;
};