#pragma once

#include "uiframe.h"

// ----------------------------------------------------------------------------------
class CUiLuaMessageFrame
{
public:
	static	void			OpenLib(lua_State* L);
	static	int				SetMetaTable(lua_State* L);
};

struct MessageHyperlinkHit
{
	MessageHyperlinkHit()
	{
		x = 0.0f;
		y = 0.0f;
		font = NULL;
	}

	float x, y;
	CUiFontString* font;
};

// ----------------------------------------------------------------------------------
class CUiMessageFrame : public CUiFrame
{
public:
	CUiMessageFrame(const char* name, CUiLayout* pParent, CUi* pUi);
	virtual ~CUiMessageFrame();	

	virtual	void			SetFontString(CUiFontString* fontString);	
	virtual CUiFontString*	GetFontString()							{ return m_fontString; }
	virtual void			ResetFontString();
	virtual CUiFrame*		GetFrameAtPoint(CUiPoint pt, CUiFrame* pExclude);

	virtual	bool			AddMessage(const WCHAR* text, DWORD color=0);
	virtual	bool			AddMessage(const char* text, DWORD color=0);
	virtual	void			ClearText();
	virtual	void			ResetTime()								{ ResetTextItemTime(); }
	virtual	void			SetMaxLines(int maxLines);
	virtual	int				GetMaxLines()							{ return m_maxLines; }
	virtual	int				GetSizeLines()							{ return m_sizeLines; }
	virtual	void			SetLastLine(int line);
	virtual	int				GetLastLine()							{ return m_lastLine; }
	virtual	void			SetFadeDuration(float time)				{ m_fadeDuration = time; }
	virtual	float			GetFadeDuration()						{ return m_fadeDuration; }
	virtual	void			SetDisplayDuration(float time)			{ m_displayDuration = time; }
	virtual	float			GetDisplayDuration()					{ return m_displayDuration; }
	virtual	void			SetTextInset(CUiRectangle& inset)		{ m_textInset = inset; }
	virtual	void			ScrollDown();
	virtual	void			ScrollUp();
	virtual	void			ScrollBottom();
	virtual	void			ScrollTop();
	virtual void			EnableHyperlink(bool enable);
	virtual void			SetLineBytes(int bytes)					{ m_lineBytes = bytes; }
	virtual int				GetPageLines();

	virtual	void			SetLuaMetaTable(lua_State* l)			{ CUiLuaMessageFrame::SetMetaTable(l); }
	virtual int				GetMemSize();

protected:
	virtual	void			ReleaseText();
	virtual	void			PopItem();
	virtual	void			PushItem(const WCHAR* text, DWORD color);
	virtual	void			ClearTextItemFont();
	virtual	void			ResetTextItemTime();
	virtual void			ReleaseHyperlink();

	virtual	void			OnResize();
	virtual	void			OnUpdate(float elapsedTime);
	virtual	void			OnPaint(float alpha);

	virtual void			OnMouseLeave(CUiPoint pt);
	virtual void			OnMouseEnter(CUiPoint pt);
	virtual void			OnClick(CUiPoint pt, int key);

protected:
	struct TextItem
	{
		CUiFontString*	font;
		WCHAR*			str;
		DWORD			color;
		float			time;
		float			alpha;
	};
	
	TextItem*				m_text;
	CUiFontString*			m_fontString;
	CUiRectangle			m_textInset;				// ��r��ܰϰ�	
	int						m_sizeLines;				// �ثe�̤j���
	int						m_maxLines;					// �̤j�e�q���
	int						m_lastLine;
	int						m_lineBytes;				// ���̤j�r����
	float					m_displayDuration;
	float					m_fadeDuration;

	bool					m_enableHyperlink;			// �O�_�}�� Hyperlink
	bool					m_hyperlinkEnter;			// �W�쵲
	WCHAR					m_hyperlink[1024];			// �W�쵲���e
	vector<MessageHyperlinkHit>		m_hyperlinkHit;
};