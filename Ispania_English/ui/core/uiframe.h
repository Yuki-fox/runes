#pragma once

#include <list>
#include <map>
#include "uifontstring.h"
#include "uitexture.h"
#include "uiinput.h"

using namespace std;
class CUiFrame;

#define DEFAULT_MAX_SIZE_WIDTH			4096
#define DEFAULT_MAX_SIZE_HEIGHT			4096
#define DEFAULT_MIN_SIZE_WIDTH			0
#define DEFAULT_MIN_SIZE_HEIGHT			0

#define UI_LUA_ONLOAD					"__uiLuaOnLoad__"
#define UI_LUA_ONUPDATE					"__uiLuaOnUpdate__"
#define UI_LUA_ONRESIZE					"__uiLuaOnResize__"
#define UI_LUA_ONSHOW					"__uiLuaOnShow__"
#define UI_LUA_ONHIDE					"__uiLuaOnHide__"
#define UI_LUA_ONENABLE					"__uiLuaOnEnable__"
#define UI_LUA_ONDISABLE				"__uiLuaOnDisable__"
#define UI_LUA_ONTABPRESSED				"__uiLuaOnTabPresssed__"
#define UI_LUA_ONENTERPRESSED			"__uiLuaOnEnterPresssed__"
#define UI_LUA_ONESCAPEPRESSED			"__uiLuaOnEscapePresssed__"
#define UI_LUA_ONSPACEPRESSED			"__uiLuaOnSpacePresssed__"
#define UI_LUA_ONARROWUPPRESSED			"__uiLuaOnArrowUpPresssed__"
#define UI_LUA_ONARROWDOWNPRESSED		"__uiLuaOnArrowDownPresssed__"
#define UI_LUA_ONKEYDOWN				"__uiLuaOnKeyDown__"
#define UI_LUA_ONKEYUP					"__uiLuaOnKeyUp__"
#define UI_LUA_ONCHAR					"__uiLuaOnChar__"
#define UI_LUA_ONMOUSEDOWN				"__uiLuaOnMouseDown__"
#define UI_LUA_ONMOUSEUP				"__uiLuaOnMouseUp__"
#define UI_LUA_ONMOUSEWHEEL				"__uiLuaOnMouseWheel__"
#define UI_LUA_ONMOUSEENTER				"__uiLuaOnMouseEnter__"
#define UI_LUA_ONMOUSELEAVE				"__uiLuaOnMouseLeave__"
#define UI_LUA_ONCLICK					"__uiLuaOnClick__"
#define UI_LUA_ONDOUBLECLICK			"__uiLuaOnDoubleClick__"
#define UI_LUA_ONDRAGBEGIN				"__uiLuaOnDragBegin__"
#define UI_LUA_ONDRAGSTOP				"__uiLuaOnDragStop__"
#define UI_LUA_ONRECEIVEDRAG			"__uiLuaOnReceiveDrag__"
#define UI_LUA_ONVALUECHANGED			"__uiLuaOnValueChanged__"
#define UI_LUA_ONTEXTCHANGED			"__uiLuaOnTextChanged__"
#define UI_LUA_ONSCROLLRANGECHANGED		"__uiLuaOnScrollRangeChanged__"
#define UI_LUA_ONVERTICALSCROLL			"__uiLuaOnVerticalScroll__"
#define UI_LUA_ONHORIZONTALSCROLL		"__uiLuaOnHorizontalScroll__"
#define UI_LUA_ONEDITFOCUSGAINED		"__uiLuaOnEditFocusGained__"
#define UI_LUA_ONEDITFOCUSLOST			"__uiLuaOnEditFocusLost__"
#define UI_LUA_ONEDITCURSORCHANGED		"__uiLuaOnEditCursorChanged__"
#define UI_LUA_ONHYPERLINKENTER			"__uiLuaOnHyperlinkEnter__"
#define UI_LUA_ONHYPERLINKLEAVE			"__uiLuaOnHyperlinkLeave__"
#define UI_LUA_ONHYPERLINKCLICK			"__uiLuaOnHyperlinkClick__"
#define UI_LUA_ONEVENT					"__uiLuaOnEvent__"

#define UI_LUA_LBUTTON					"LBUTTON"
#define UI_LUA_RBUTTON					"RBUTTON"
#define UI_LUA_MBUTTON					"MBUTTON"
#define UI_LUA_XBUTTON					"XBUTTON"

#define UI_LUA_KEY_ESC					"ESCAPE"
#define UI_LUA_KEY_MINUS				"MINUS"
#define UI_LUA_KEY_EQUALS				"EQUALS"
#define UI_LUA_KEY_BACKSPACE			"BACKSPACE"
#define UI_LUA_KEY_TAB					"TAB"
#define UI_LUA_KEY_LBRACKET				"LBRACKET"
#define UI_LUA_KEY_RBRACKET				"RBRACKET"
#define UI_LUA_KEY_ENTER				"ENTER"
#define UI_LUA_KEY_CTRL					"CTRL"	
#define UI_LUA_KEY_SHIFT				"SHIFT"
#define UI_LUA_KEY_ALT					"ALT"
#define UI_LUA_KEY_0					"0"
#define UI_LUA_KEY_1					"1"
#define UI_LUA_KEY_2					"2"
#define UI_LUA_KEY_3					"3"
#define UI_LUA_KEY_4					"4"
#define UI_LUA_KEY_5					"5"
#define UI_LUA_KEY_6					"6"
#define UI_LUA_KEY_7					"7"
#define UI_LUA_KEY_8					"8"
#define UI_LUA_KEY_9					"9"
#define UI_LUA_KEY_A					"A"
#define UI_LUA_KEY_B					"B"
#define UI_LUA_KEY_C					"C"
#define UI_LUA_KEY_D					"D"
#define UI_LUA_KEY_E					"E"
#define UI_LUA_KEY_F					"F"
#define UI_LUA_KEY_G					"G"
#define UI_LUA_KEY_H					"H"
#define UI_LUA_KEY_I					"I"
#define UI_LUA_KEY_J					"J"
#define UI_LUA_KEY_K					"K"
#define UI_LUA_KEY_L					"L"
#define UI_LUA_KEY_M					"M"
#define UI_LUA_KEY_N					"N"
#define UI_LUA_KEY_O					"O"
#define UI_LUA_KEY_P					"P"
#define UI_LUA_KEY_Q					"Q"
#define UI_LUA_KEY_R					"R"
#define UI_LUA_KEY_S					"S"
#define UI_LUA_KEY_T					"T"
#define UI_LUA_KEY_U					"U"
#define UI_LUA_KEY_V					"V"
#define UI_LUA_KEY_W					"W"
#define UI_LUA_KEY_X					"X"
#define UI_LUA_KEY_Y					"Y"
#define UI_LUA_KEY_Z					"Z"
#define UI_LUA_KEY_SEMICOLON			"SEMICOLON"
#define UI_LUA_KEY_APOSTROPHE			"APOSTROPHE"
#define UI_LUA_KEY_GRAVE				"GRAVE"
#define UI_LUA_KEY_BACKSLASH			"BACKSLASH"
#define UI_LUA_KEY_COMMA				"COMMA"
#define UI_LUA_KEY_PERIOD				"PERIOD"
#define UI_LUA_KEY_SLASH				"SLASH"
#define UI_LUA_KEY_SPACE				"SPACE"
#define UI_LUA_KEY_CAPSLOCK				"CAPSLOCK"
#define UI_LUA_KEY_F1					"F1"
#define UI_LUA_KEY_F2					"F2"
#define UI_LUA_KEY_F3					"F3"
#define UI_LUA_KEY_F4					"F4"
#define UI_LUA_KEY_F5					"F5"
#define UI_LUA_KEY_F6					"F6"
#define UI_LUA_KEY_F7					"F7"
#define UI_LUA_KEY_F8					"F8"
#define UI_LUA_KEY_F9					"F9"
#define UI_LUA_KEY_F10					"F10"
#define UI_LUA_KEY_F11					"F11"
#define UI_LUA_KEY_F12					"F12"
#define UI_LUA_KEY_SYSRQ				"SYSRQ"
#define UI_LUA_KEY_SCROLLLOCK			"SCROLLLOCK"
#define UI_LUA_KEY_PAUSE				"PAUSE"
#define UI_LUA_KEY_NUMLOCK				"NUMLOCK"
#define UI_LUA_KEY_NUMPAD0				"NUMPAD0"
#define UI_LUA_KEY_NUMPAD1				"NUMPAD1"
#define UI_LUA_KEY_NUMPAD2				"NUMPAD2"
#define UI_LUA_KEY_NUMPAD3				"NUMPAD3"
#define UI_LUA_KEY_NUMPAD4				"NUMPAD4"
#define UI_LUA_KEY_NUMPAD5				"NUMPAD5"
#define UI_LUA_KEY_NUMPAD6				"NUMPAD6"
#define UI_LUA_KEY_NUMPAD7				"NUMPAD7"
#define UI_LUA_KEY_NUMPAD8				"NUMPAD8"
#define UI_LUA_KEY_NUMPAD9				"NUMPAD9"
#define UI_LUA_KEY_ADD					"ADD"
#define UI_LUA_KEY_SUBTRACT				"SUBTRACT"
#define UI_LUA_KEY_DIVIDE				"DIVIDE"
#define UI_LUA_KEY_MULTIPLY				"MULTIPLY"
#define UI_LUA_KEY_DECIMAL				"DECIMAL"
#define UI_LUA_KEY_NUMPADENTER			"NUMPADENTER"
#define UI_LUA_KEY_INSERT				"INSERT"
#define UI_LUA_KEY_DELETE				"DELETE"
#define UI_LUA_KEY_HOME					"HOME"
#define UI_LUA_KEY_END					"END"
#define UI_LUA_KEY_PAGEUP				"PAGEUP"
#define UI_LUA_KEY_PAGEDOWN				"PAGEDOWN"
#define UI_LUA_KEY_UP					"UP"
#define UI_LUA_KEY_DOWN					"DOWN"
#define UI_LUA_KEY_LEFT					"LEFT"
#define UI_LUA_KEY_RIGHT				"RIGHT"
#define UI_LUA_KEY_LWIN					"LWIN"
#define UI_LUA_KEY_RWIN					"RWIN"
#define UI_LUA_KEY_APPS					"APPS"

enum UIFrameStrata
{
	eUIFrame_Strata_Background			= 0,
	eUIFrame_Strata_Low,
	eUIFrame_Strata_Medium,
	eUIFrame_Strata_High,
	eUIFrame_Strata_Dialog,
	eUIFrame_Strata_Tooltip,
};

// ----------------------------------------------------------------------------------
typedef void (*pfUiCallback) (CUiFrame* uiFrame, INT64 lParam, INT64 rParam);

// ----------------------------------------------------------------------------------
void UiOuputError(const char* errorStr);

// ----------------------------------------------------------------------------------
class CUiEventFlag
{
public:
	CUiEventFlag()
	{
		memset(m_date, 0, sizeof(m_date));
	}

	void	SetBits(int index, bool isOn)
	{
		int* value;
		if (index < 32)
			value = m_date;
		else
		{
			index -= 32;
			value = m_date + 1;
		}
		if (isOn)
			*value |= (1 << index);
		else
			*value &= ~(1 << index);
	}

	int	GetBits(int index)
	{
		if (index < 32)
			return m_date[0] & (1 << index);
		else
			return m_date[1] & (1 << (index-32));
	}

	bool	LockEvent(int index)
	{
		if ( GetBits(index) ) return false;
		SetBits(index, true);
		return true;
	}

	void	UnlockEvent(int index)
	{
		SetBits(index, false);
	}

protected:	
	int		m_date[2];
};

// ----------------------------------------------------------------------------------
class CUiLuaFrame
{
public:
	static	void			OpenLib(lua_State* L);
	static	int				SetMetaTable(lua_State* L);
};

// ----------------------------------------------------------------------------------
class CUiFrame : public CUiLayout
{
public:
	CUiFrame(const char* name, CUiLayout* pParent, CUi* pUi);
	virtual ~CUiFrame();

	enum LayerType {
		eLayerType_Background			= 1,
		eLayerType_Border,
		eLayerType_Artwork,
		eLayerType_Overlay,
		eLayerType_Highlight,
	};

	enum EventType{
		eEventType_None					= -1,
		eEventType_Load					= 0,
		eEventType_Update,
		eEventType_Resize,
		eEventType_Show,
		eEventType_Hide,
		eEventType_Enable,
		eEventType_Disable,

		eEventType_TabPressed,
		eEventType_EnterPressed,
		eEventType_EscapePressed,
		eEventType_SpacePressed,
		eEventType_ArrowUpPressed,
		eEventType_ArrowDownPressed,
		eEventType_KeyDown,
		eEventType_KeyUp,
		eEventType_Char,	

		eEventType_MouseDown,
		eEventType_MouseUp,
		eEventType_MouseWheel,
		eEventType_MouseEnter,
		eEventType_MouseLeave,
		eEventType_Click,
		eEventType_DoubleClick,
		eEventType_DragBegin,
		eEventType_DragStop,
		eEventType_ReceiveDrag,

		eEventType_ValueChanged,
		eEventType_TextChanged,

		eEventType_ScrollRangeChanged,
		eEventType_VerticalScroll,
		eEventType_HorizontalScroll,

		eEventType_EditFocusGained,
		eEventType_EditFocusLost,
		eEventType_EditCursorChanged,

		eEventType_HyperlinkEnter,
		eEventType_HyperlinkLeave,
		eEventType_HyperlinkClick,

		eEventType_Event		
	};

	enum {
		eMouse_Flag_LButton				= 0x01,
		eMouse_Flag_RButton				= 0x02,
		eMouse_Flag_MButton				= 0x04,
		eMouse_Flag_XButton				= 0x08,
	};

	static	const char*		GetButtonName(BYTE num);
	static	const char*		GetKeyName(WPARAM wParam);
	static	WPARAM			GetKeyNum(const char* key);

	// virtual functions
	virtual bool			HandleMouse(CUiInput& input);
	virtual	bool			HandleKeyboard(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual	bool			GetKeyboardState()						{ return m_isVisible && m_isEnable && m_state.isKeyboardEnable; }

	virtual void			Update(float elapsedTime);
	virtual	void			Render(float alpha);

	virtual bool			IsVisible(bool checkParent=false);
	virtual void			SetSize(CUiSize size, bool isReAnchor=true);
	virtual void			SetVisible(bool isVisible);	
	virtual void			SetEnable(bool isEnable);
	virtual bool			IsEnable()								{ return m_isEnable; }
	virtual void			SetKeyboardEnable(bool isEnable);
	virtual bool			IsKeyboardEnable()						{ return m_state.isKeyboardEnable; }
	virtual void			SetMouseEnable(bool isEnable);
	virtual bool			IsMouseEnable()							{ return m_state.isMouseEnable; }
	virtual	void			SetMovable(bool isMovable)				{ m_state.isMovable = isMovable; }
	virtual	bool			IsMovable()								{ return m_state.isMovable; }
	virtual	void			SetResizable(bool isResizable)			{ m_state.isResizable = isResizable; }
	virtual	bool			IsResizable()							{ return m_state.isResizable; }	
	virtual	void			StartMoving();
	virtual	bool			IsMoving()								{ return m_state.isMoving; }
	virtual	void			StartSizing(int anchorPoint);
	virtual	bool			IsSizing()								{ return m_state.isSizing; }
	virtual	void			StopMovingOrSizing();
	virtual	void			SetMaxResizeBounds(CUiSize maxSize);
	virtual	void			SetMinResizeBounds(CUiSize minSize);
	virtual	void			SetID(int id)							{ m_id = id; }
	virtual	int				GetID()									{ return m_id; }
	virtual	void			RegisterForDrag(int flag)				{ m_dragSet = flag; }
	virtual	int				GetDragSet()							{ return m_dragSet; }
	virtual	void			RegisterForClicks(int flag)				{ m_clicksSet = flag; }
	virtual	int				GetClicksSet()							{ return m_clicksSet; }
	virtual	void			RegisterEvent(const char* event);
	virtual	void			UnregisterEvent(const char* event);	
	virtual	void			SetFrameStrata(UIFrameStrata strata);
	virtual	UIFrameStrata	GetFrameStrata()						{ return m_frameStrata; }
	virtual	void			SetTopLevel(bool topLevel);
	virtual	bool			GetTopLevel()							{ return m_topLevel; }	
	virtual	void			SetFrameLevel(int level);
	virtual	int				GetFrameLevel()							{ return m_frameLevel; }
	virtual	void			SetFocus();
	virtual	void			ClearFocus();
	virtual	void			SetBackdrop(CTextureBackdrop* pBackdrop);
	virtual	void			SetBackdropTileColor(CUiRGBA& color);
	virtual	void			SetBackdropTileAlpha(float alpha);
	virtual	void			SetBackdropEdgeColor(CUiRGBA& color);
	virtual	void			SetBackdropEdgeAlpha(float alpha);
	
	virtual	CTextureBackdrop*	GetBackdrop()						{ return m_pBackdrop; }		
	virtual	void			SetHitRectInsets(CUiRectangle& rc)		{ m_hitRectInsets = rc; }
	virtual	bool			ContainsPoint(CUiPoint pt);
	virtual CUiFrame*		GetFrameAtPoint(CUiPoint pt, CUiFrame* pExclude);
	virtual CUiFrame*		GetFrameAtKeyboard(CUiFrame* pExclude);
	virtual int				GetScriptEvent(const char* eventType);
	virtual char*			GetEventName(const char* script);
	virtual	void			SetScripts(int type, const char* script);
	

	
	virtual void			SetLayers(int layer, CUiLayout* pUiLayout);
	virtual void			RemoveLayer( CUiLayout* pUiLayout );



	virtual	void			SetLuaMetaTable(lua_State* l)			{ CUiLuaFrame::SetMetaTable(l); }
	virtual	list<CUiFrame*>&	GetChildList()						{ return m_child; }	

	virtual	CUiFrame*		GetMouseDown(int key);	
	virtual	void			SetScrollParent(CUiLayout* layout);
	virtual	void			GetScrollChildRect(CUiRectangle* childRect);

	virtual	void			KillFocus(int flag = -1);
	virtual void			Locked();
	virtual void			Unlocked();
	virtual	void			AdjustFrameLevel();
	virtual void			SetParent(CUiFrame* parent);
	virtual int				GetMemSize();
	virtual void			SetEventFlag(int eventtype,bool eventEnable);

	virtual void			Resume();

public:
	pfUiCallback			m_pfOnPaint;
	pfUiCallback			m_pfOnShow;
	pfUiCallback			m_pfOnHide;
	pfUiCallback			m_pfOnMouseDown;
	pfUiCallback			m_pfOnMouseUp;	
	pfUiCallback			m_pfOnMouseMove;
	pfUiCallback			m_pfOnMouseEnter;
	pfUiCallback			m_pfOnMouseLeave;
	pfUiCallback			m_pfOnMouseWheel;
	pfUiCallback			m_pfOnClick;
	pfUiCallback			m_pfOnDoubleClick;
	pfUiCallback			m_pfOnDragBegin;
	pfUiCallback			m_pfOnDragStop;
	pfUiCallback			m_pfOnReceiveDrag;

protected:
	friend class CUi;	
	friend class CUiScrollFrame;
	virtual	void			CallLuaFunc(lua_State* L, const char* type_key, int narg=0);

	virtual	void			OnLoad();
	virtual	void			OnResize();
	virtual void			OnShow(lua_State* L);
	virtual void			OnHide(lua_State* L);
	virtual void			OnEnalbe(lua_State* L);
	virtual void			OnDisalbe(lua_State* L);
	virtual	void			OnUpdate(float elapsedTime);
	virtual	void			OnPaint(float alpha);
	virtual	void			OnKillFocus(CUiFrame* pUiFrame)					{}
	virtual	void			OnSetFocus(CUiFrame* pUiFrame)					{}

	virtual void			OnMouseLeave(CUiPoint pt);
	virtual void			OnMouseEnter(CUiPoint pt);
	virtual void			OnMouseMove(CUiPoint pt);
	virtual void			OnMouseWheel(CUiPoint pt, int delta);
	virtual void			OnClick(CUiPoint pt, int key);
	virtual void			OnDoubleClick(CUiPoint pt, int key);
	virtual void			OnMouseDown(CUiPoint pt, int key);
	virtual void			OnMouseUp(CUiPoint pt, int key);
	virtual void			OnDragBegin();
	virtual void			OnDragStop();
	virtual	void			OnReceiveDrag();
	
	virtual	void			OnChar(WPARAM wParam, LPARAM lParam);
	virtual void			OnKeyDown(WPARAM wParam, LPARAM lParam);
	virtual void			OnKeyUp(WPARAM wParam, LPARAM lParam);
	virtual	void			OnEnterPressed();
	virtual	void			OnTabPressed();
	virtual	void			OnSpacePressed();
	virtual	void			OnEscapePressed();
	virtual	void			OnArrowUpPressed();
	virtual	void			OnArrowDownPressed();

	virtual	void			OnTextChanged();
	virtual	void			OnValueChanged(float arg1);
	virtual	void			OnVerticalScroll(float arg1);
	virtual	void			OnHorizontalScroll(float arg1);
	virtual	void			OnScrollRangeChanged(float arg1, float arg2);
	virtual	void			OnEvent(const char* event);

	virtual	void			OnEditFocusGained(CUiFrame* lost);
	virtual	void			OnEditFocusLost(CUiFrame* gain);
	virtual void			OnEditCursorChanged(int ch, int ln);

	virtual void			OnHyperlinkEnter(const WCHAR* link);
	virtual void			OnHyperlinkLeave(const WCHAR* link);
	virtual void			OnHyperlinkClick(const WCHAR* link, int key);
	
	virtual	void			EraseChild(CUiFrame *pUiFrame);
	virtual	void			InsertChild(CUiFrame *pUiFrame);	

protected:
	// member values
	multimap<int, CUiLayout*>	m_layers;	
	list<CUiFrame*>			m_child;
	CUiEventFlag			m_eventFlag;
	CUiEventFlag			m_lockEvent;
	CTextureBackdrop*		m_pBackdrop;
	CUiRGBA					m_backdropTileColor;
	CUiRGBA					m_backdropEdgeColor;
	CUiRectangle			m_hitRectInsets;	
	CUiSize					m_maxResizeBounds;
	CUiSize					m_minResizeBounds;
	int						m_id;
	UIFrameStrata			m_frameStrata;
	int						m_frameLevel;	
	int						m_clicksSet;
	int						m_dragSet;
	bool					m_topLevel;
	bool					m_isEnable;
	map<int, string>		m_resume;

	union {
		int					flag;
		struct{			
			bool				isMouseEnable		: 1;
			bool				isKeyboardEnable	: 1;
			bool				isMovable			: 1;
			bool				isResizable			: 1;
			bool				isMoving			: 1;
			bool				isSizing			: 1;
			bool				isLockedChild		: 1;			// 鎖定子元件不能在更新
		};
	}m_state;
	int						m_sizingAnchorPoint;
	int						m_eventCount;
};