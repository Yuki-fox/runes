#pragma once

#include <list>
#include <string>
#include "../xml/xmlui.h"
#include "uiframe.h"
#include "uimodel.h"
#include "uibutton.h"
#include "uicheckbutton.h"
#include "uiradiobutton.h"
#include "uieditbox.h"
#include "uilistbox.h"
#include "uistatusbar.h"
#include "uislider.h"
#include "uimessageframe.h"
#include "uiscrollframe.h"
#include "uitooltip.h"

//#include "UiWebbrowser.h"


using namespace std;

/*
class CXmlNode;
class CXmlLayout;
class CXmlFile;
class CXmlUi;
*/

class CUi
{
public:
	CUi(lua_State* L, HWND hWnd);
	virtual ~CUi();

	struct MouseKey
	{
		CUiFrame*	pFrame;
		float		time;
	};

	struct ltstr
	{
		bool operator () (const char* p1, const char* p2) const
		{
			return strcmp(p1, p2) < 0;
		}
	};

	lua_State*				GetLuaState()						{ return m_luaState; }	
	CXmlUi*					GetXmlUi()							{ return m_xmlUi; }
	bool					MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool					OnChar(wchar_t* ws, int len);	
	void					Update(float elapsedTime, InputState &state);
	void					Render();

	void					SetScreenSize(CUiSize& size)		{ m_pRootFrame->SetSize(size); }
	void					SetScale(float scale);
	float					GetScale()							{ return m_scale; }
	CUiFrame*				GetRoot()							{ return m_pRootFrame; }
	void					SetCapture(CUiFrame* pUiFrame, int key);
	CUiFrame*				GetCapture()						{ return m_pCapture; }
	int						GetCaptureKey()						{ return m_captureKey; }
	void					SetKeyboardFocus(CUiFrame* pUiFrame);
	CUiFrame*				GetKeyboardFocus()					{ return m_pKeyboardFocus; }
	void					SetDragFrame(CUiFrame* pUiFrame)	{ m_pDragFrame = pUiFrame; }
	CUiFrame*				GetDragFrame()						{ return m_pDragFrame; }
	CUiFrame*				GetMouseOver()						{ return m_pMouseOver; }	
	CUiFrame*				GetMouseFocus()						{ return m_pMouseFocus; }
	void					OnReceiveDrag(CUiPoint& pt, CUiFrame* pExclude);
	void					SetMouseDown(int key, CUiFrame* pUiFrame);
	CUiFrame*				GetMouseDown(int key);
	void					SetMouseExclude(CUiFrame* pUiFrame)	{ m_pMouseExclude = pUiFrame; }	
	bool					LoadScriptFile(const char* filename);
	bool					LoadScriptFromBuffer(const char* buffer, int bufferSize, const char* filename=NULL);
	CUiInput*				GetUiInput()						{ return &m_input; }	
	CUiPoint				GetCursorPos()						{ return m_cursorPos; }
	CUiPoint				GetCursorOffset()					{ return m_cursorOffset; }
	CUiPoint				GetCapturePoint()					{ return m_capturePoint; }

	void					InitializeXmlParser(const char* resourcePathname);
	void					ReleaseXmlParser();
	bool					LoadXmlFile(const char* filename);
	bool					LoadXmlBinding(const char* filename);

	void					OpenTocFile(const char* filename, const char* resourcePath);
	void					BuildFrames();
	CUiLayout*				CreateObject(int type, const char* name, CUiLayout* pParent, const char* inherit=NULL);
	CUiLayout*				FindObject(const char* name);
	void					RegisterEvent(const char* event, CUiFrame* pUiFrame);
	void					UnregisterEvent(const char* event, CUiFrame* pUiFrame);
	bool					SendEvent(const char* event);

	void					LockedFrame(CUiFrame* frame);
	void					UnlockedFrame(CUiFrame* frame);

	int						GetTotalMemSize();
	string					GetFontName(const char* fontFilename);

	void					SetCaretPoint(CUiPoint point)						{ m_caretPoint = point; }
	void					SetIMEComStr(bool isComStr)							{ m_IsComStr = isComStr; }
	bool					GetIMEComStr()										{ return m_IsComStr; }

	void					SetUpdateState(bool isUpdate)						{ m_isUpdate = isUpdate; }
	bool					IsUpdate()											{ return m_isUpdate; }
	bool					IsEvent()											{ return m_isEvent; }
	void					ClearCurrentIme();

protected:
	friend class CUiLayout;	
	void					InsertObject(CUiLayout* layout);
	void					DeleteObject(CUiLayout* layout);

	void					LoadXmlFile(CXmlFile* pXmlFile);
	void					LoadXmlScript(CXmlNode* pXmlNode);
	CUiFrame*				LoadXmlFrames(CUiLayout* pUiParent, CXmlLayout* pXmlLayout);
	CUiLayout*				LoadXmlLayers(CUiLayout* pUiParent, CXmlLayout* pXmlLayout);
	void					LoadXmlLayout(CUiLayout* pUiLayout, CXmlLayout* pXmlLayout);	
	void					LoadXmlFontString(CUiLayout* pUiLayout, CXmlLayout* pXmlLayout);
	void					LoadXmlTexture(CUiLayout* pUiLayout, CXmlLayout* pXmlLayout);
	void					LoadXmlFrame(CUiLayout* pUiLayout, CXmlLayout* pXmlLayout);
	void					LoadXmlModel(CUiLayout* pUiLayout, CXmlLayout* pXmlLayout);
	void					LoadXmlButton(CUiLayout* pUiLayout, CXmlLayout* pXmlLayout);
	void					LoadXmlCheckButton(CUiLayout* pUiLayout, CXmlLayout* pXmlLayout);
	void					LoadXmlRadioButton(CUiLayout* pUiLayout, CXmlLayout* pXmlLayout);
	void					LoadXmlSlider(CUiLayout* pUiLayout, CXmlLayout* pXmlLayout);
	void					LoadXmlStatusBar(CUiLayout* pUiLayout, CXmlLayout* pXmlLayout);
	void					LoadXmlEditBox(CUiLayout* pUiLayout, CXmlLayout* pXmlLayout);
	void					LoadXmlListBox(CUiLayout* pUiLayout, CXmlLayout* pXmlLayout);
	void					LoadXmlMessageFrame(CUiLayout* pUiLayout, CXmlLayout* pXmlLayout);
	void					LoadXmlScrollFrame(CUiLayout* pUiLayout, CXmlLayout* pXmlLayout);
	void					LoadXmlTooltip(CUiLayout* pUiLayout, CXmlLayout* pXmlLayout);

	void					DisableIME();
	void					EnableIME();	

protected:
#ifdef KALYDO
	// optimization for reading fonts
	map< string, string >		m_FontNames;
#endif // KALYDO

	//friend class CUiFrame;	
	map<const char*, CUiLayout*, ltstr>		m_object;
	map< string, vector<CUiFrame*> >		m_userEvent;
	HWND					m_hWnd;
	string					m_resourcePath;
	lua_State*				m_luaState;
	list<CUiFrame*>			m_lock;
	CUiInput				m_input;
	CUiFrame*				m_pRootFrame;
	CUiFrame*				m_pCapture;
	CUiFrame*				m_pKeyDownFrame;								// 記錄 WINDOW MSG 按鍵事件, WM_KEYDOWN, WM_CHAR, WM_KEYUP 需要同一元件才能觸發
	int						m_captureKey;
	CXmlUi*					m_xmlUi;	
	int						m_eventLockedCount;
	CUiFrame*				m_pKeyboardFocus;
	CUiFrame*				m_pMouseFocus;
	CUiFrame*				m_pKeyCapture;
	CUiFrame*				m_pMouseOver;
	CUiFrame*				m_pDragFrame;
	CUiFrame*				m_pMouseExclude;
	CUiFrame*				m_mouseDown[MAX_MOUSE_BUTTON_NUM];
	CUiFrame*				m_lastmouseDown[MAX_MOUSE_BUTTON_NUM];
	float					m_mouseDownTime[MAX_MOUSE_BUTTON_NUM];
	float					m_lastmouseDownTime[MAX_MOUSE_BUTTON_NUM];
	float					m_elapsedTime;
	float					m_absoluteTime;
	float					m_scale;
	CUiPoint				m_cursorPos;
	CUiPoint				m_cursorOffset;	
	CUiPoint				m_capturePoint;	
	CUiPoint				m_caretPoint;
	bool					m_isUpdate;
	bool					m_isEvent;
	int						m_CurrentIMESize;	// scott

	// ANSI string buffer
protected:	
	HKL m_hkl;
	DWORD m_ImeProp;
	CHARSETINFO m_csInfo;
	UINT m_CurCP;
	bool m_IsComStr;

#ifdef UNICODE
#else
	enum{
		MAXANSIBUF = 0x20 // 32
	};

	char m_asbText[MAXANSIBUF];
	int m_asbTextLen;

	BOOL asbAddByte(BYTE by);
	BOOL asbSubmit(void);
	BOOL asbClear(void);
#endif

};