#pragma once

#include <list>
#include "uiframe.h"

using namespace std;

// --------------------------------------------------------------------------------------
class CUiLuaEditBox
{
public:
	static	void			OpenLib(lua_State* L);
	static	int				SetMetaTable(lua_State* L);
};

// --------------------------------------------------------------------------------------
struct MultiLineInfo
{
	int start;				// ��l�r����ަ�m
	int count;				// �r�����
	float width;			// �Ӧ�e��
};

// --------------------------------------------------------------------------------------
class CUiEditBox : public CUiFrame
{
public:
	CUiEditBox(const char* name, CUiLayout* pParent, CUi* pUi);
	virtual ~CUiEditBox();	
	
	virtual void			SetMouseEnable(bool isEnable)			{ m_state.isMouseEnable = true; }	
	virtual void			SetKeyboardEnable(bool isEnable)		{ m_state.isKeyboardEnable = true; }
	virtual	void			SetFontString(CUiFontString* fontString);
	virtual CUiFontString*	GetFontString()							{ return m_fontString; }
	virtual	void			SetLetters(int letters);
	virtual	void			SetText(const WCHAR* text);
	virtual	void			SetTextA(const char* text);
	virtual	void			SetTextA( const string& str){ SetTextA( str.c_str() ); };
	virtual	void			SetHistoryLines(int value);
	virtual	void			AddHistoryLine(const WCHAR* text);
	virtual	void			AddHistoryLine(const char* text);
	virtual	const WCHAR*	GetHistoryLine(int line);
	virtual	const WCHAR*	GetText()								{ return m_text; }
	virtual bool			AddText(const WCHAR* text);
	virtual bool			AddTextA(const char* text);
	virtual CUiFrame*		GetFrameAtPoint(CUiPoint pt, CUiFrame* pExclude);
	virtual	void			SetNumeric(bool isNumeric)				{ m_isNumeric = isNumeric; }
	virtual	bool			IsNumeric()								{ return m_isNumeric; }
	virtual	void			SetPassword(bool isPassword)			{ m_isPassword = isPassword; }
	virtual	bool			IsPassword()							{ return m_isPassword; }
	virtual	void			SetMultiMode(bool isMulti)				{ m_multiMode = isMulti; }
	virtual	bool			GetMultiMode()							{ return m_multiMode; }	
	virtual	void			SetHighlightColor(DWORD color)			{ m_blockColor = color; }
	virtual	DWORD			GetHighlightColor()						{ return m_blockColor; }
	virtual void			SetCaretColor(DWORD color)				{ m_caretColor = color; }
	virtual int				GetCaretColor()							{ return m_caretColor; }
	virtual	int				GetTextSize()							{ return m_textSize; }
	virtual	int				GetLetters()							{ return m_letters; }	
	virtual void			SetTextInset(CUiRectangle& inset)		{ m_textInset = inset; }
	virtual	void			SetLuaMetaTable(lua_State* l)			{ CUiLuaEditBox::SetMetaTable(l); }
	virtual	void			SetTextColor(CUiColor& color);	
	virtual	void			HighlightText(int start=0, int end=-1);
	virtual	int				GetNumMultiLines();
	virtual	void			InsertKeyboardChar(const wchar_t* ws, int len);
	virtual	void			InsertKeyboardChar(const char* s, int len);
	virtual	void			SetPlaceCaret(int caretPos);
	virtual int				GetMultiLines()							{ return (int)m_multiLine.size(); }
	virtual int				GetMultiDisplayLines()					{ return m_displayMaxLine; }
	virtual int				GetMultiFirstLine()						{ return m_firstLine; }	
	virtual void			SetMultiFirstLine(int line);
	virtual void			SetInputState(bool inputEnable)			{ m_isKeyboardInput = inputEnable; }
	virtual void			SetRTL(bool isRTL)						{ m_isRTL = isRTL; }
	virtual bool			IsRTL()									{ return m_isRTL; }
	virtual int				GetMemSize();

protected:
	virtual	void			PlaceCaret(int caretPos);
	virtual	void			PasteFromClipboard();
	virtual	void			CopyToClipboard();
	virtual	void			DeleteSelectionText();
	virtual	void			ResetCaretBlink();
	virtual	void			AdjustHistory();
	
	virtual	void			UpdateLine();
	virtual	void			CaretToPoint(int caret, int &ln, int &ch);
	virtual	void			PointToPos(int ln, int ch, float &x, float &y);
	virtual	void			PosToPoint(float x, float y, int &ln, int &ch);
	virtual	int				PointToCaret(int ln, int ch);
	virtual	void			PlacePoint(int ln, int ch);

	virtual	bool			InsertChar(int index, WCHAR ch);
	virtual	int				RemoveChar(int index);
	virtual	int				InsertString(int index, const WCHAR *text, int size=-1);
	
	virtual	void			OnUpdate(float elapsedTime);
	virtual	void			OnPaint(float alpha);
	virtual	void			OnResize();
	virtual	void			OnMouseDown(CUiPoint pt, int key);
	virtual	void			OnMouseMove(CUiPoint pt);
	virtual	void			OnChar(WPARAM wParam, LPARAM lParam);
	virtual	void			OnKeyDown(WPARAM wParam, LPARAM lParam);
	virtual	void			OnTextChanged();

protected:
	list<WCHAR*>			m_history;
	int						m_historyLast;
	int						m_historyLines;
	CUiFontString*			m_fontString;
	WCHAR*					m_text;
	CUiRectangle			m_textInset;				// ��r��ܰϰ�	
	DWORD					m_blockColor;				// �����r�I���C��
	DWORD					m_caretColor;				// ����C��
	int 					m_textSize;
	int						m_letters;					// ��J�r���Ӽ�		
	bool					m_multiMode;				// ���Φh��Ҧ�
	bool					m_isNumeric;				// �u���J�Ʀr
	bool					m_isPassword;				// �K�X�Ҧ�
	bool					m_isInsertMode;				// ���J�Ψ��N�Ҧ�
	bool					m_isPaser;					// �O�_�����r��
	bool					m_isKeyboardInput;			// �O�_�i�H��J��r

	float					m_blinkSpeed;
	float					m_lastBlinkTime;
	int						m_firstVisible;
	int						m_selStart;
	int						m_caretPos;	
	bool					m_caretBool;	

	// �h��s��
	vector<MultiLineInfo>	m_multiLine;
	int						m_firstLine;
	int						m_displayMaxLine;
	int						m_ln;
	int						m_ch;
	wchar_t					m_comStr;
	bool					m_isRTL;
};