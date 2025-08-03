#include <windows.h>
#include "uiframe.h"
#include "ui.h"

// ----------------------------------------------------------------------------------
CUiFrame::CUiFrame(const char* name, CUiLayout* pParent, CUi* pUi)
	: CUiLayout(name, pParent, pUi)
{
	m_uiType			= eUiType_Frame;

	m_layers.clear();
	m_child.clear();
	
	m_pBackdrop				= NULL;
	m_id					= 0;
	m_frameLevel			= 0;	
	m_frameStrata			= eUIFrame_Strata_Medium;
	m_clicksSet				= eMouse_Flag_LButton;
	m_dragSet				= eMouse_Flag_LButton;
	m_topLevel				= false;
	m_isEnable				= true;
	m_state.flag			= 0;
	m_sizingAnchorPoint		= 0;
	m_eventCount			= 0;
	m_maxResizeBounds.m_x	= DEFAULT_MAX_SIZE_WIDTH;
	m_maxResizeBounds.m_y	= DEFAULT_MAX_SIZE_HEIGHT;
	m_minResizeBounds.m_x	= DEFAULT_MIN_SIZE_WIDTH;
	m_minResizeBounds.m_y	= DEFAULT_MIN_SIZE_HEIGHT;

	m_backdropTileColor.SetColor( 0xFFFFFFFF );
	m_backdropEdgeColor.SetColor( 0xFFFFFFFF );

	m_pfOnPaint			= NULL;
	m_pfOnShow			= NULL;
	m_pfOnHide			= NULL;
	m_pfOnMouseDown		= NULL;
	m_pfOnMouseUp		= NULL;	
	m_pfOnMouseMove		= NULL;
	m_pfOnMouseEnter	= NULL;
	m_pfOnMouseLeave	= NULL;
	m_pfOnMouseWheel	= NULL;
	m_pfOnClick			= NULL;
	m_pfOnDoubleClick	= NULL;
	m_pfOnDragBegin		= NULL;
	m_pfOnDragStop		= NULL;
	m_pfOnReceiveDrag	= NULL;

	if (pParent)
	{
		CUiFrame* pUiFrame = dynamic_cast<CUiFrame*>(pParent);
		if (pUiFrame)
			pUiFrame->InsertChild(this);
	}
}

// ----------------------------------------------------------------------------------
CUiFrame::~CUiFrame()
{	
	// 通知父元件移除自己
	if (m_pParent)
	{
		CUiFrame* pUiFrame = dynamic_cast<CUiFrame*>(m_pParent);
		if ( pUiFrame )
			pUiFrame->EraseChild(this);
		m_pParent = NULL;
	}

	// 刪除所有子元件
	for ( list<CUiFrame*>::iterator iter = m_child.begin(); iter != m_child.end(); iter++ )
	{
		(*iter)->m_pParent = NULL;
		delete (*iter);
	}
	m_child.clear();	
	
	// 刪除所有資源物件
	for ( multimap<int, CUiLayout*>::iterator iter = m_layers.begin(); iter != m_layers.end(); iter++ )
		delete iter->second;
	m_layers.clear();
	
	// 刪除背景物件
	UI_SAFE_DELETE(m_pBackdrop);
	
	// 釋放所有事件資料
}

// ----------------------------------------------------------------------------------
bool CUiFrame::HandleMouse(CUiInput& input)
{
	CUiPoint pt((float)input.GetXPos(), (float)input.GetYPos());
	//bool isMouseIn = ContainsPoint(pt);

	/*
	if (!m_isEnable || !m_isVisible || !m_state.isMouseEnable || m_pUi == NULL)
		return false;
	*/
	if ( !m_isEnable || !m_isVisible || m_pUi == NULL )
		return false;

	// 滑鼠軸向有做移動
	if ( input.GetXDelta() != 0.0f || input.GetYDelta() != 0.0f )
		OnMouseMove(pt);

	// 滾輪滾動
	int wheelDelta = (int)input.GetZDelta();
	if ( input.GetZDelta() != 0 )
		OnMouseWheel(pt, wheelDelta);

	// 按鈕狀態
	for (int key = 0; key < MAX_MOUSE_BUTTON_NUM; key++)
	{
		if (input.GetButtonDown(key))
		{
			if ( m_pUi->GetCapture() == NULL )
				m_pUi->SetCapture(this, key);

			m_pUi->SetMouseDown(key, this);
			OnMouseDown(pt, key);
		}

		if (input.GetButtonUp(key))
		{
			// 如果目前為拖曳狀態
			if ( m_pUi->GetDragFrame() == this )
			{
				//m_pUi->OnReceiveDrag(pt, this);
				m_pUi->OnReceiveDrag(pt, NULL);
				OnDragStop();
			}

			// 當按下與放開在同一元件時觸發
			if ( m_pUi->GetMouseDown(key) == this )
			{
				if ( ContainsPoint(pt) && m_clicksSet & (0x01 << key) )
					OnClick(pt, key);
			}

			// 解除鎖定狀態
			if ( m_pUi->GetCapture() && key == m_pUi->GetCaptureKey() )
				m_pUi->SetCapture(NULL, 0);

			// OnMouseUp(key);
		}
	}

	return true;
}

// ----------------------------------------------------------------------------------
bool CUiFrame::HandleKeyboard(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CHAR:
		OnChar(wParam, lParam);
		switch ( wParam )
		{
			// Enter
		case L'\r':
			OnEnterPressed();
			break;

			// Tab
		case L'\t':
			OnTabPressed();
			break;

			// Space
		case L' ':
			OnSpacePressed();
			break;

			// Escape
		case 27:
			OnEscapePressed();
			break;
		}
		break;

	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		OnKeyDown(wParam, lParam);
		break;

	case WM_KEYUP:
	case WM_SYSKEYUP:
		OnKeyUp(wParam, lParam);
		break;
	}

	return true;
}

// ----------------------------------------------------------------------------------
void CUiFrame::Update(float elapsedTime)
{
	CUiPoint cursorOffset = m_pUi->GetCursorOffset();

	if (!m_isEnable || !m_isVisible)
		return;

	if ( m_state.isMoving )
		Moving( cursorOffset.m_x, cursorOffset.m_y );

	if ( m_state.isSizing )
		Sizing( m_sizingAnchorPoint, cursorOffset.m_x, cursorOffset.m_y );

	// 繪出LAYERS物件
	for ( multimap<int, CUiLayout*>::iterator iter = m_layers.begin(); iter != m_layers.end(); iter++ )
		(iter->second)->Update(elapsedTime);

	OnUpdate(elapsedTime);	

	m_state.isLockedChild = true;


	// 更新子元件
	for ( list<CUiFrame*>::iterator iter = m_child.begin(); iter != m_child.end(); iter++ )
		(*iter)->Update(elapsedTime);
	
	m_state.isLockedChild = false;
}

// ----------------------------------------------------------------------------------
void CUiFrame::Render(float alpha)
{
	if ( !m_isVisible )
		return;

	alpha *= m_color.m_a;

	// 繪出背景
	if (m_pBackdrop)
	{
		CUiRectangle rc;		
		GetRectangle(&rc);

		CUiRGBA tileColor = m_backdropTileColor;
		CUiRGBA edgeColor = m_backdropEdgeColor;
		tileColor.SetAlphaScale(alpha);
		edgeColor.SetAlphaScale(alpha);

		CRuRectangle drawRect(rc.m_left, rc.m_top, rc.m_right, rc.m_bottom);
		m_pBackdrop->Draw(drawRect, tileColor.m_value , edgeColor.m_value);
	}

	// 繪出LAYERS物件(Background, Border)
	multimap<int, CUiLayout*>::iterator iter = m_layers.begin();
	while (iter != m_layers.end())
	{
		if ( iter->first > eLayerType_Border )
			break;
		else
		{
			(iter->second)->Render(alpha);
			iter++;
		}
	}	

	// 繪出自己
	OnPaint(alpha);

	// 繪出LAYERS物件(Artwork, Overlay, Highlight)
	for ( ; iter != m_layers.end(); iter++ )
		(iter->second)->Render(alpha);

	// 繪出子元件
	for ( list<CUiFrame*>::iterator iter = m_child.begin(); iter != m_child.end(); iter++ )
		(*iter)->Render(alpha);
}

// ----------------------------------------------------------------------------------
bool CUiFrame::IsVisible(bool checkParent)
{
	if ( !m_isVisible )
		return false;

	if ( checkParent && m_pParent )
		return m_pParent->IsVisible(true);	

	return true;
}

// ----------------------------------------------------------------------------------
void CUiFrame::SetSize(CUiSize size, bool isReAnchor)
{
	if ( size.m_x > m_maxResizeBounds.m_x )
		size.m_x = m_maxResizeBounds.m_x;
	else if ( size.m_x < m_minResizeBounds.m_x )
		size.m_x = m_minResizeBounds.m_x;

	if ( size.m_y > m_maxResizeBounds.m_y )
		size.m_y = m_maxResizeBounds.m_y;
	else if ( size.m_y < m_minResizeBounds.m_y )
		size.m_y = m_minResizeBounds.m_y;

	if (size != m_size)
	{
		m_size = size;
		OnResize();

		if (isReAnchor)
			ResetAnchors();			// 重新定位
	}
}

// ----------------------------------------------------------------------------------
void CUiFrame::SetVisible(bool isVisible)
{
	if (m_isVisible != isVisible)
	{
		CUiLayout::SetVisible(isVisible);
		if ( m_isVisible ) {
			bool parentShow = true;

			CUiLayout* parent = m_pParent;
			while (parent) {
				if ( parent->IsVisible() ) {
					parent = parent->GetParent();
				} else {
					parentShow = false;
					break;
				}
			}

			if ( parentShow )
				OnShow( GetLuaState() );
		} else {
			OnHide( GetLuaState() );
		}
	}
}

// ----------------------------------------------------------------------------------
void CUiFrame::SetEnable(bool isEnable)
{
	if ( m_isEnable != isEnable )
	{
		m_isEnable = isEnable;
		if ( m_isEnable )
		{
			OnEnalbe( GetLuaState() );
		}
		else
		{
			OnDisalbe( GetLuaState() );
		}
	}
}

// ----------------------------------------------------------------------------------
void CUiFrame::SetKeyboardEnable(bool isEnable)
{
	m_state.isKeyboardEnable = isEnable;
	if ( !m_state.isKeyboardEnable && m_pUi && m_pUi->GetKeyboardFocus() == this )
		m_pUi->SetKeyboardFocus(NULL);
}

// ----------------------------------------------------------------------------------
void CUiFrame::SetMouseEnable(bool isEnable)
{
	m_state.isMouseEnable = isEnable;
	if ( !m_state.isMouseEnable && m_pUi && m_pUi->GetCapture() == this )
		m_pUi->SetCapture(NULL, 0);
}

// ----------------------------------------------------------------------------------
void CUiFrame::RegisterEvent(const char* event)
{
	if (m_pUi)
		m_pUi->RegisterEvent(event, this);
}

// ----------------------------------------------------------------------------------
void CUiFrame::UnregisterEvent(const char* event)
{
	if (m_pUi)
		m_pUi->UnregisterEvent(event, this);
}

// ----------------------------------------------------------------------------------
void CUiFrame::SetFrameStrata(UIFrameStrata strata)
{
	if ( m_frameStrata != strata )
	{
		m_frameStrata = strata;
		AdjustFrameLevel();
	}
}

// ----------------------------------------------------------------------------------
void CUiFrame::SetTopLevel(bool topLevel)
{
	if ( m_topLevel != topLevel )
	{
		m_topLevel = topLevel;
		AdjustFrameLevel();
	}
}

// ----------------------------------------------------------------------------------
void CUiFrame::SetFrameLevel(int level)
{
	if (m_frameLevel != level)
	{
		m_frameLevel = level;
		AdjustFrameLevel();
	}
}

// ----------------------------------------------------------------------------------
void CUiFrame::SetFocus()
{
	if (m_pUi)
		m_pUi->SetKeyboardFocus(this);
}

// ----------------------------------------------------------------------------------
void CUiFrame::ClearFocus()
{
	if (m_pUi && m_pUi->GetKeyboardFocus() == this)
		m_pUi->SetKeyboardFocus(NULL);
}

// ----------------------------------------------------------------------------------
void CUiFrame::SetBackdrop(CTextureBackdrop* pBackdrop)
{
	UI_SAFE_DELETE(m_pBackdrop);
	if (pBackdrop)
	{
		m_pBackdrop = new CTextureBackdrop;
		*m_pBackdrop = *pBackdrop;
	}
}

// ----------------------------------------------------------------------------------
void CUiFrame::SetBackdropTileColor(CUiRGBA& color)
{
	m_backdropTileColor.m_r = color.m_r;
	m_backdropTileColor.m_g = color.m_g;
	m_backdropTileColor.m_b = color.m_b;
}

// ----------------------------------------------------------------------------------
void CUiFrame::SetBackdropTileAlpha(float alpha)
{
	m_backdropTileColor.m_a = (unsigned char)(min(max(alpha, 0.0f), 1.0f) * 255);
}

// ----------------------------------------------------------------------------------
void CUiFrame::SetBackdropEdgeColor(CUiRGBA& color)
{
	m_backdropEdgeColor.m_r = color.m_r;
	m_backdropEdgeColor.m_g = color.m_g;
	m_backdropEdgeColor.m_b = color.m_b;
}

// ----------------------------------------------------------------------------------
void CUiFrame::SetBackdropEdgeAlpha(float alpha)
{
	m_backdropEdgeColor.m_a = (unsigned char)(min(max(alpha, 0.0f), 1.0f) * 255);
}

// ----------------------------------------------------------------------------------
void CUiFrame::StartMoving()
{
	m_state.isMoving = true;
}

// ----------------------------------------------------------------------------------
void CUiFrame::StartSizing(int anchorPoint)
{
	m_state.isSizing = true;
	m_sizingAnchorPoint = anchorPoint;
}

// ----------------------------------------------------------------------------------
void CUiFrame::StopMovingOrSizing()
{
	m_state.isMoving = false;
	m_state.isSizing = false;
}

// ----------------------------------------------------------------------------------
void CUiFrame::SetMaxResizeBounds(CUiSize maxSize)
{
	m_maxResizeBounds = maxSize;
	if ( m_maxResizeBounds.m_x > DEFAULT_MAX_SIZE_WIDTH )
		m_maxResizeBounds.m_x = DEFAULT_MAX_SIZE_WIDTH;
	if ( m_maxResizeBounds.m_x > DEFAULT_MAX_SIZE_HEIGHT )
		m_maxResizeBounds.m_y = DEFAULT_MAX_SIZE_HEIGHT;
}

// ----------------------------------------------------------------------------------
void CUiFrame::SetMinResizeBounds(CUiSize minSize)
{
	m_minResizeBounds = minSize;
	if ( m_minResizeBounds.m_x < DEFAULT_MIN_SIZE_WIDTH )
		m_minResizeBounds.m_x = DEFAULT_MIN_SIZE_WIDTH;
	if ( m_minResizeBounds.m_y < DEFAULT_MIN_SIZE_HEIGHT )
		m_minResizeBounds.m_y = DEFAULT_MIN_SIZE_HEIGHT;
}

// ----------------------------------------------------------------------------------
bool CUiFrame::ContainsPoint(CUiPoint pt)
{
	CUiRectangle rect;	
	GetRectangle(&rect);
	float scale = GetRealScale();
	if ( pt.m_x < rect.m_left + m_hitRectInsets.m_left*scale )
		return false;
	else if ( pt.m_y < rect.m_top + m_hitRectInsets.m_top*scale )
		return false;
	else if ( pt.m_x > rect.m_right - m_hitRectInsets.m_right*scale )
		return false;
	else if ( pt.m_y > rect.m_bottom - m_hitRectInsets.m_bottom*scale )
		return false;

	return true;
}

// ----------------------------------------------------------------------------------
CUiFrame* CUiFrame::GetFrameAtPoint(CUiPoint pt, CUiFrame* pExclude)
{
	if (m_isVisible /*&& m_isEnable*/)
	{
		for (list<CUiFrame*>::reverse_iterator iter = m_child.rbegin(); iter != m_child.rend(); iter++ )
		{
			CUiFrame *pFocus;
			if ( (pFocus = (*iter)->GetFrameAtPoint(pt , pExclude)) != NULL )
				return pFocus;
		}

		if ( m_state.isMouseEnable && this != pExclude && ContainsPoint(pt) )
			return this;
	}	
	return NULL;
}

// ----------------------------------------------------------------------------------
CUiFrame* CUiFrame::GetFrameAtKeyboard(CUiFrame* pExclude)
{
	if (m_isVisible && m_isEnable)
	{
		for (list<CUiFrame*>::reverse_iterator iter = m_child.rbegin(); iter != m_child.rend(); iter++)
		{
			CUiFrame* pFocus;
			if ( (pFocus = (*iter)->GetFrameAtKeyboard(pExclude)) != NULL )
				return pFocus;
		}

		if (m_state.isKeyboardEnable && m_uiType != eUiType_EditBox && this != pExclude)
			return this;
	}
	return NULL;
}

// ----------------------------------------------------------------------------------
int CUiFrame::GetScriptEvent(const char* eventType)
{
	if ( strcmp(eventType, "OnLoad") == 0 ) {
		return eEventType_Load;
	} else if ( strcmp(eventType, "OnUpdate") == 0 ) {
		return eEventType_Update;
	} else if ( strcmp(eventType, "OnSizeChanged") == 0 ) {
		return eEventType_Resize;
	} else if ( strcmp(eventType, "OnShow") == 0 ) {
		return eEventType_Show;
	} else if ( strcmp(eventType, "OnHide") == 0 ) {
		return eEventType_Hide;
	} else if ( strcmp(eventType, "OnEnable") == 0 ) {
		return eEventType_Enable;
	} else if ( strcmp(eventType, "OnDisable") == 0 ) {
		return eEventType_Disable;
	} else if ( strcmp(eventType, "OnTabPressed") == 0 ) {
		return eEventType_TabPressed;
	} else if ( strcmp(eventType, "OnEnterPressed") == 0 ) {
		return eEventType_EnterPressed;
	} else if ( strcmp(eventType, "OnEscapePressed") == 0 ) {
		return eEventType_EscapePressed;
	} else if ( strcmp(eventType, "OnSpacePressed") == 0 ) {
		return eEventType_SpacePressed;
	} else if ( strcmp(eventType, "OnArrowUpPressed") == 0 ) {
		return eEventType_ArrowUpPressed;
	} else if ( strcmp(eventType, "OnArrowDownPressed") == 0 ) {
		return eEventType_ArrowDownPressed;
	} else if ( strcmp(eventType, "OnKeyDown") == 0 ) {
		return eEventType_KeyDown;
	} else if ( strcmp(eventType, "OnKeyUp") == 0 ) {
		return eEventType_KeyUp;
	} else if ( strcmp(eventType, "OnChar") == 0 ) {
		return eEventType_Char;
	} else if ( strcmp(eventType, "OnMouseDown") == 0 ) {
		return eEventType_MouseDown;
	} else if ( strcmp(eventType, "OnMouseUp") == 0 ) {
		return eEventType_MouseUp;
	} else if ( strcmp(eventType, "OnClick") == 0 ) {
		return eEventType_Click;
	} else if ( strcmp(eventType, "OnDoubleClick") == 0 ) {
		return eEventType_DoubleClick;
	} else if ( strcmp(eventType, "OnMouseWheel") == 0 ) {
		return eEventType_MouseWheel;
	} else if ( strcmp(eventType, "OnEnter") == 0 ) {
		return eEventType_MouseEnter;
	} else if ( strcmp(eventType, "OnLeave") == 0 ) {
		return eEventType_MouseLeave;
	} else if ( strcmp(eventType, "OnDragStart") == 0 ) {
		return eEventType_DragBegin;
	} else if ( strcmp(eventType, "OnDragStop") == 0 ) {
		return eEventType_DragStop;
	} else if ( strcmp(eventType, "OnReceiveDrag") == 0 ) {
		return eEventType_ReceiveDrag;
	} else if ( strcmp(eventType, "OnValueChanged") == 0 ) {
		return eEventType_ValueChanged;
	} else if ( strcmp(eventType, "OnTextChanged") == 0 ) {
		return eEventType_TextChanged;
	} else if ( strcmp(eventType, "OnScrollRangeChanged") == 0 ) {
		return eEventType_ScrollRangeChanged;
	} else if ( strcmp(eventType, "OnVerticalScroll") == 0 ) {
		return eEventType_VerticalScroll;
	} else if ( strcmp(eventType, "OnHorizontalScroll") == 0 ) {
		return eEventType_HorizontalScroll;
	} else if ( strcmp(eventType, "OnEditFocusGained") == 0 ) {
		return eEventType_EditFocusGained;
	} else if ( strcmp(eventType, "OnEditFocusLost") == 0 ) {
		return eEventType_EditFocusLost;
	} else if ( strcmp(eventType, "OnCursorChanged") == 0 ) {
		return eEventType_EditCursorChanged;
	} else if ( strcmp(eventType, "OnHyperlinkEnter") == 0 ) {
		return eEventType_HyperlinkEnter;
	} else if ( strcmp(eventType, "OnHyperlinkLeave") == 0 ) {
		return eEventType_HyperlinkLeave;
	} else if ( strcmp(eventType, "OnHyperlinkClick") == 0 ) {
		return eEventType_HyperlinkClick;
	} else if ( strcmp(eventType, "OnEvent") == 0 ) {
		return eEventType_Event;
	}
	return eEventType_None;
}

// ----------------------------------------------------------------------------------
char* CUiFrame::GetEventName(const char* script)
{
	if ( strcmp(script, UI_LUA_ONLOAD) == 0 )
		return "OnLoad";
	else if ( strcmp(script, UI_LUA_ONUPDATE) == 0 ) 
		return "OnUpdate";
	else if ( strcmp(script, UI_LUA_ONRESIZE) == 0 ) 
		return "OnSizeChanged";
	else if ( strcmp(script, UI_LUA_ONSHOW) == 0 ) 
		return "OnShow";
	else if ( strcmp(script, UI_LUA_ONHIDE) == 0 ) 
		return "OnHide";
	else if ( strcmp(script, UI_LUA_ONENABLE) == 0 ) 
		return "OnEnable";
	else if ( strcmp(script, UI_LUA_ONDISABLE) == 0 ) 
		return "OnDisable";
	else if ( strcmp(script, UI_LUA_ONTABPRESSED) == 0 ) 
		return "OnTabPressed";
	else if ( strcmp(script, UI_LUA_ONENTERPRESSED) == 0 ) 
		return "OnEscapePressed";
	else if ( strcmp(script, UI_LUA_ONESCAPEPRESSED) == 0 ) 
		return "OnEscapePressed";
	else if ( strcmp(script, UI_LUA_ONSPACEPRESSED) == 0 ) 
		return "OnSpacePressed";
	else if ( strcmp(script, UI_LUA_ONARROWUPPRESSED) == 0 ) 
		return "OnArrowUpPressed";
	else if ( strcmp(script, UI_LUA_ONARROWDOWNPRESSED) == 0 ) 
		return "OnArrowDownPressed";
	else if ( strcmp(script, UI_LUA_ONKEYDOWN) == 0 ) 
		return "OnKeyDown";
	else if ( strcmp(script, UI_LUA_ONKEYUP) == 0 ) 
		return "OnKeyUp";
	else if ( strcmp(script, UI_LUA_ONCHAR) == 0 ) 
		return "OnChar";
	else if ( strcmp(script, UI_LUA_ONMOUSEDOWN) == 0 ) 
		return "OnMouseDown";
	else if ( strcmp(script, UI_LUA_ONMOUSEUP) == 0 ) 
		return "OnMouseUp";
	else if ( strcmp(script, UI_LUA_ONMOUSEWHEEL) == 0 )
		return "OnMouseWheel";
	else if ( strcmp(script, UI_LUA_ONMOUSEENTER) == 0 )
		return "OnEnter";
	else if ( strcmp(script, UI_LUA_ONMOUSELEAVE) == 0 )
		return "OnLeave";
	else if ( strcmp(script, UI_LUA_ONCLICK) == 0 )
		return "OnClick";
	else if ( strcmp(script, UI_LUA_ONDOUBLECLICK) == 0 )
		return "OnDoubleClick";
	else if ( strcmp(script, UI_LUA_ONDRAGBEGIN) == 0 )
		return "OnDragStart";
	else if ( strcmp(script, UI_LUA_ONDRAGSTOP) == 0 )
		return "OnDragStop";
	else if ( strcmp(script, UI_LUA_ONRECEIVEDRAG) == 0 )
		return "OnReceiveDrag";
	else if ( strcmp(script, UI_LUA_ONVALUECHANGED) == 0 )
		return "OnValueChanged";
	else if ( strcmp(script, UI_LUA_ONTEXTCHANGED) == 0 )
		return "OnTextChanged";
	else if ( strcmp(script, UI_LUA_ONSCROLLRANGECHANGED) == 0 )
		return "OnScrollRangeChanged";
	else if ( strcmp(script, UI_LUA_ONVERTICALSCROLL) == 0 )
		return "OnVerticalScroll";
	else if ( strcmp(script, UI_LUA_ONHORIZONTALSCROLL) == 0 )
		return "OnHorizontalScroll";
	else if ( strcmp(script, UI_LUA_ONEDITFOCUSGAINED) == 0 )
		return "OnEditFocusGained";
	else if ( strcmp(script, UI_LUA_ONEDITFOCUSLOST) == 0 )
		return "OnEditFocusLost";
	else if ( strcmp(script, UI_LUA_ONEDITCURSORCHANGED) == 0 )
		return "OnCursorChanged";
	else if ( strcmp(script, UI_LUA_ONHYPERLINKENTER) == 0 )
		return "OnHyperlinkEnter";
	else if ( strcmp(script, UI_LUA_ONHYPERLINKLEAVE) == 0 )
		return "OnHyperlinkLeave";
	else if ( strcmp(script, UI_LUA_ONHYPERLINKCLICK) == 0 )
		return "OnHyperlinkClick";
	else if ( strcmp(script, UI_LUA_ONEVENT) == 0 )
		return "OnEvent";
	else
		return "Unknow Event";
}




// ----------------------------------------------------------------------------------
void CUiFrame::SetScripts(int type, const char* script)
{
	lua_State* L = GetLuaState();
	if (L == NULL || script == NULL)
		return;

	if ( lua_checkresume(L) )
	{
		m_resume[type] = script;
		return;
	}

	// first push this table
	CUiLuaLayout::PushLayoutTable(L, this);
	if (!lua_isnil(L, -1))
	{
		const char* func_narg = NULL;
		const char* table_key = NULL;
		switch(type)
		{
		case eEventType_Load:
			func_narg = "this";
			table_key = UI_LUA_ONLOAD;
			break;

		case eEventType_Update:
			func_narg = "this, elapsedTime";
			table_key = UI_LUA_ONUPDATE;
			break;

		case eEventType_Resize:
			func_narg = "this";
			table_key = UI_LUA_ONRESIZE;
			break;

		case eEventType_Show:
			func_narg = "this";
			table_key = UI_LUA_ONSHOW;
			break;

		case eEventType_Hide:
			func_narg = "this";
			table_key = UI_LUA_ONHIDE;
			break;

		case eEventType_Enable:
			func_narg = "this";
			table_key = UI_LUA_ONENABLE;
			break;

		case eEventType_Disable:
			func_narg = "this";
			table_key = UI_LUA_ONDISABLE;
			break;

		case eEventType_TabPressed:
			func_narg = "this";
			table_key = UI_LUA_ONTABPRESSED;
			break;

		case eEventType_EnterPressed:
			func_narg = "this";
			table_key = UI_LUA_ONENTERPRESSED;
			break;

		case eEventType_EscapePressed:
			func_narg = "this";
			table_key = UI_LUA_ONESCAPEPRESSED;
			break;

		case eEventType_SpacePressed:
			func_narg = "this";
			table_key = UI_LUA_ONSPACEPRESSED;
			break;

		case eEventType_ArrowUpPressed:
			func_narg = "this";
			table_key = UI_LUA_ONARROWUPPRESSED;
			break;

		case eEventType_ArrowDownPressed:
			func_narg = "this";
			table_key = UI_LUA_ONARROWDOWNPRESSED;
			break;

		case eEventType_KeyDown:
			func_narg = "this, key";
			table_key = UI_LUA_ONKEYDOWN;
			break;

		case eEventType_KeyUp:
			func_narg = "this, key";
			table_key = UI_LUA_ONKEYUP;
			break;

		case eEventType_Char:
			func_narg = "this, char";
			table_key = UI_LUA_ONCHAR;
			break;

		case eEventType_MouseDown:
			func_narg = "this, key";
			table_key = UI_LUA_ONMOUSEDOWN;
			break;

		case eEventType_MouseUp:
			func_narg = "this, key";
			table_key = UI_LUA_ONMOUSEUP;
			break;

		case eEventType_MouseWheel:
			func_narg = "this, delta";
			table_key = UI_LUA_ONMOUSEWHEEL;
			break;

		case eEventType_MouseEnter:
			func_narg = "this";
			table_key = UI_LUA_ONMOUSEENTER;
			break;

		case eEventType_MouseLeave:
			func_narg = "this";
			table_key = UI_LUA_ONMOUSELEAVE;
			break;

		case eEventType_Click:
			func_narg = "this, key";
			table_key = UI_LUA_ONCLICK;
			break;

		case eEventType_DoubleClick:
			func_narg = "this, key";
			table_key = UI_LUA_ONDOUBLECLICK;
			break;

		case eEventType_DragBegin:
			func_narg = "this";
			table_key = UI_LUA_ONDRAGBEGIN;
			break;

		case eEventType_DragStop:
			func_narg = "this";
			table_key = UI_LUA_ONDRAGSTOP;
			break;

		case eEventType_ReceiveDrag:
			func_narg = "this";
			table_key = UI_LUA_ONRECEIVEDRAG;
			break;

		case eEventType_ValueChanged:
			func_narg = "this, arg1";
			table_key = UI_LUA_ONVALUECHANGED;
			break;

		case eEventType_TextChanged:
			func_narg = "this";
			table_key = UI_LUA_ONTEXTCHANGED;
			break;

		case eEventType_ScrollRangeChanged:
			func_narg = "this, arg1, arg2";
			table_key = UI_LUA_ONSCROLLRANGECHANGED;
			break;

		case eEventType_VerticalScroll:
			func_narg = "this, arg1";
			table_key = UI_LUA_ONVERTICALSCROLL;
			break;

		case eEventType_HorizontalScroll:
			func_narg = "this, arg1";
			table_key = UI_LUA_ONHORIZONTALSCROLL;
			break;

		case eEventType_EditFocusGained:
			func_narg = "this";
			table_key = UI_LUA_ONEDITFOCUSGAINED;
			break;

		case eEventType_EditFocusLost:
			func_narg = "this";
			table_key = UI_LUA_ONEDITFOCUSLOST;
			break;

		case eEventType_EditCursorChanged:
			func_narg = "this, arg1, arg2, arg3, arg4";
			table_key = UI_LUA_ONEDITCURSORCHANGED;
			break;

		case eEventType_HyperlinkEnter:
			func_narg = "this, link";
			table_key = UI_LUA_ONHYPERLINKENTER;
			break;

		case eEventType_HyperlinkLeave:
			func_narg = "this, link";
			table_key = UI_LUA_ONHYPERLINKLEAVE;
			break;

		case eEventType_HyperlinkClick:
			func_narg = "this, link, key";
			table_key = UI_LUA_ONHYPERLINKCLICK;
			break;

		case eEventType_Event:
			func_narg = "this, event";
			table_key = UI_LUA_ONEVENT;
			break;

		default:
			lua_pop(L, 1);
			return;
		}

		char* tmp = new char[strlen(script) + 127];
		sprintf(tmp, "function %s (%s)\n%s\nend", table_key, func_narg, script);
		if (luaL_loadbuffer(L, tmp, strlen(tmp), 0) != 0 || lua_resume(L, 0) != 0 )
		{
			const char* err = lua_tostring(L, -1);
			lua_pop(L, 1);
			UiOuputError(err);
		}
		else
		{
			lua_getglobal(L, table_key);
			if (!lua_isnil(L, -1))
			{
				lua_pushstring(L, table_key);
				lua_insert(L, -2);
				lua_rawset(L, -3);
				lua_pushnil(L);
				lua_setglobal(L, table_key);
			}
			else
				lua_pop(L, 1);
		}
		m_eventFlag.SetBits(type, true);
		delete [] tmp;

		/*
		if (func)
		{
			lua_pushstring(L, table_key);
			lua_pushcfunction(L, func);
			lua_rawset(L, -3);
		}
		*/
	}
	lua_pop(L, 1);
}

void CUiFrame::SetEventFlag(int eventtype,bool eventEnable)
{

	m_eventFlag.SetBits(eventtype, eventEnable);
}

// ----------------------------------------------------------------------------------
void CUiFrame::Resume()
{
	for ( list<CUiFrame*>::iterator iter = m_child.begin(); iter != m_child.end(); iter++ )
		(*iter)->Resume();

	for ( map<int, string>::iterator iter = m_resume.begin(); iter != m_resume.end(); iter++ )
		SetScripts(iter->first, iter->second.c_str());

	m_resume.clear();

	OnLoad();
}
// ----------------------------------------------------------------------------------
void CUiFrame::SetLayers(int layer, CUiLayout* pUiLayout)
{
	if ( pUiLayout == NULL )
		return;

	// 如果元件已經建立,移除重新插入
	for ( multimap<int, CUiLayout*>::iterator iter = m_layers.begin(); iter != m_layers.begin(); iter++ )
	{
		if ( iter->second == pUiLayout )
		{
			m_layers.erase(iter);
			break;
		}
	}

	if (pUiLayout)
		m_layers.insert(make_pair(layer, pUiLayout));
}


// ---------------------------------------------------------------------------------
void CUiFrame::RemoveLayer( CUiLayout* pUiLayout )
{
	if ( pUiLayout == NULL )
		return;

	// 如果元件已經建立,移除重新插入
	for ( multimap<int, CUiLayout*>::iterator iter = m_layers.begin(); iter != m_layers.end(); iter++ )
	{
		if ( iter->second == pUiLayout )
		{
			OutputDebugStringA( "remove\n" );
			m_layers.erase(iter);
			break;
		}
	}
}
// ----------------------------------------------------------------------------------
CUiFrame* CUiFrame::GetMouseDown(int key)
{
	return m_pUi->GetMouseDown(key);
}

// ----------------------------------------------------------------------------------
void CUiFrame::SetScrollParent(CUiLayout* layout)
{
	CUiLayout::SetScrollParent(layout);

	// LAYERS物件
	for ( multimap<int, CUiLayout*>::iterator iter = m_layers.begin(); iter != m_layers.end(); iter++ )
		(iter->second)->SetScrollParent(layout);

	// 子元件
	for ( list<CUiFrame*>::iterator iter = m_child.begin(); iter != m_child.end(); iter++ )
		(*iter)->SetScrollParent(layout);
}

// ----------------------------------------------------------------------------------
void CUiFrame::GetScrollChildRect(CUiRectangle* childRect)
{
	if (m_isVisible && childRect)
	{
		CUiRectangle rect;

		GetRectangle(&rect, false);
		childRect->Collect(rect);

		// LAYERS物件
		for ( multimap<int, CUiLayout*>::iterator iter = m_layers.begin(); iter != m_layers.end(); iter++ )
		{			
			CUiLayout* layout = iter->second;
			if ( layout->IsVisible() ) {
				layout->GetRectangle(&rect, false);
				childRect->Collect(rect);
			}
		}

		// 子元件
		for ( list<CUiFrame*>::iterator iter = m_child.begin(); iter != m_child.end(); iter++ )
			(*iter)->GetScrollChildRect(childRect);
	}
}

// ----------------------------------------------------------------------------------
const char* CUiFrame::GetButtonName(BYTE num)
{
	switch (num)
	{
	case MOUSE_LBUTTON:
		return UI_LUA_LBUTTON;

	case MOUSE_RBUTTON:
		return UI_LUA_RBUTTON;

	case MOUSE_MBUTTON:
		return UI_LUA_MBUTTON;

	case MOUSE_XBUTTON:
		return UI_LUA_XBUTTON;
	}
	return NULL;
}

// ----------------------------------------------------------------------------------
const char* CUiFrame::GetKeyName(WPARAM wParam)
{	
	switch (wParam)
	{
	case VK_BACK:
		return "BACKSPACE";
	case VK_TAB:
		return "TAB";
	case VK_CONTROL:
		return "CTRL";
	case VK_MENU:
		return "ALT";
	case VK_SHIFT:
		return "SHIFT";
	case VK_ESCAPE:
		return "ESCAPE";
	case VK_RETURN:
		return "ENTER";
	case VK_SPACE:
		return "SPACE";
	case '0':
		return "0";
	case '1':
		return "1";
	case '2':
		return "2";
	case '3':
		return "3";
	case '4':
		return "4";
	case '5':
		return "5";
	case '6':
		return "6";
	case '7':
		return "7";
	case '8':
		return "8";
	case '9':
		return "9";
	case 'A':
		return "A";
	case 'B':
		return "B";
	case 'C':
		return "C";
	case 'D':
		return "D";
	case 'E':
		return "E";
	case 'F':
		return "F";
	case 'G':
		return "G";
	case 'H':
		return "H";
	case 'I':
		return "I";
	case 'J':
		return "J";
	case 'K':
		return "K";
	case 'L':
		return "L";
	case 'M':
		return "M";
	case 'N':
		return "N";
	case 'O':
		return "O";
	case 'P':
		return "P";
	case 'Q':
		return "Q";
	case 'R':
		return "R";
	case 'S':
		return "S";
	case 'T':
		return "T";
	case 'U':
		return "U";
	case 'V':
		return "V";
	case 'W':
		return "W";
	case 'X':
		return "X";
	case 'Y':
		return "Y";
	case 'Z':
		return "Z";
	case VK_F1:
		return "F1";
	case VK_F2:
		return "F2";
	case VK_F3:
		return "F3";
	case VK_F4:
		return "F4";
	case VK_F5:
		return "F5";
	case VK_F6:
		return "F6";
	case VK_F7:
		return "F7";
	case VK_F8:
		return "F8";
	case VK_F9:
		return "F9";
	case VK_F10:
		return "F10";
	case VK_F11:
		return "F11";
	case VK_F12:
		return "F12";
	case VK_NUMLOCK:
		return "NUMLOCK";
	case VK_NUMPAD0:
		return "NUMPAD0";
	case VK_NUMPAD1:
		return "NUMPAD1";
	case VK_NUMPAD2:
		return "NUMPAD2";
	case VK_NUMPAD3:
		return "NUMPAD3";
	case VK_NUMPAD4:
		return "NUMPAD4";
	case VK_NUMPAD5:
		return "NUMPAD5";
	case VK_NUMPAD6:
		return "NUMPAD6";
	case VK_NUMPAD7:
		return "NUMPAD7";
	case VK_NUMPAD8:
		return "NUMPAD8";
	case VK_NUMPAD9:
		return "NUMPAD9";
	case VK_ADD:
		return "ADD";
	case VK_SUBTRACT:
		return "SUBTRACT";
	case VK_DIVIDE:
		return "DIVIDE";
	case VK_MULTIPLY:
		return "MULTIPLY";
	case VK_DECIMAL:
		return "DECIMAL";
	case VK_INSERT:
		return "INSERT";
	case VK_DELETE:
		return "DELETE";
	case VK_HOME:
		return "HOME";
	case VK_END:
		return "END";
	case VK_UP:
		return "UP";
	case VK_DOWN:
		return "DOWN";
	case VK_LEFT:
		return "LEFT";
	case VK_RIGHT:
		return "RIGHT";
	case VK_LWIN:
		return "LWIN";
	case VK_RWIN:
		return "RWIN";
	case VK_APPS:
		return "APPS";
	case VK_OEM_PLUS:		// '='
		return "=";
	case VK_OEM_MINUS:		// '-'
		return "-";
	case VK_OEM_COMMA:		// ',' any country
		return ",";
	case VK_OEM_PERIOD:		// '.' any country
		return ".";
	case VK_OEM_1:			// ';:' for US
		return ";";
	case VK_OEM_2:			// '/?' for US
		return "/";
	case VK_OEM_3:			// '`'   //fuming 8-14-2007
		return "`";
	case VK_OEM_4:			//  '[{' for US
		return "[";
	case VK_OEM_5:			//  '\|' for US
		return "\\";
	case VK_OEM_6:			//  ']}' for US
		return "]";
	case VK_OEM_7:			//  ''"' for US
		return "'";
	case VK_SNAPSHOT:		// PtrScr/SysRq
		return "PRTSCR";
	case VK_SCROLL:			// Scroll Lock
		return "SCROLL";
	case VK_PAUSE:			// Pause/Break
		return "PAUSE";
// fuming 8-10-2007
	case VK_PRIOR:			// PAGEUP
		return "PAGEUP";
	case VK_NEXT:			// PAGEDOWN
		return "PAGEDOWN";
	}
	return NULL;
}

// ----------------------------------------------------------------------------------
WPARAM CUiFrame::GetKeyNum(const char* key)
{
	if ( key )
	{
		int len = (int)strlen(key);
		if ( len == 1 ) // 0~9 A~Z  fuming 8-14-2007
		{
			if ( key[0] >= '0' && key[0] <= '9') 
				return key[0];
			else if (key[0] >= 'A' &&key[0] <= 'Z')
				return key[0];
			else if (key[0] >= 'a' &&key[0] <= 'z')
				return key[0] - 32;
		}
		else if ( len < 4 && (key[0] == 'F' || key[0] == 'f') )
		{
			int num = atoi(key + 1);
			if ( num >= 1 && num <= 12 )
				return VK_F1 + num - 1;
		}
		else if ( strnicmp("NUMPAD", key, 6) == 0 )
		{
			if ( key[6] >= '0' && key[6] <= '9' )
				return VK_NUMPAD0 + key[6] - '0';
		}		
		else if ( stricmp(key, "TAB") == 0 )
			return VK_TAB;
		else if ( stricmp(key, "ESCAPE") == 0 )
			return VK_ESCAPE;
		else if ( stricmp(key, "ENTER") == 0 )//fuming 8-10-2007
			return VK_RETURN;
		else if ( stricmp(key, "CTRL") == 0 )
			return VK_CONTROL;
		else if ( stricmp(key, "ALT") == 0 )
			return VK_MENU;
		else if ( stricmp(key, "SHIFT") == 0 )
			return VK_SHIFT;
		else if ( stricmp(key, "SPACE") == 0 )
			return VK_SPACE;
		else if ( stricmp(key, "UP") == 0 )
			return VK_UP;
		else if ( stricmp(key, "DOWN") == 0 )
			return VK_DOWN;
		else if ( stricmp(key, "LEFT") == 0 )
			return VK_LEFT;
		else if ( stricmp(key, "RIGHT") == 0 )
			return VK_RIGHT;
		else if ( stricmp(key, "INSERT") == 0 )
			return VK_INSERT;
		else if ( stricmp(key, "DELETE") == 0 )
			return VK_DELETE;
		else if ( stricmp(key, "HOME") == 0 )
			return VK_HOME;
		else if ( stricmp(key, "END") == 0 )
			return VK_END;
		else if ( stricmp(key, "BACKSPACE") == 0 )
			return VK_BACK;
		else if ( stricmp(key, "ADD") == 0 )
			return VK_ADD;
		else if ( stricmp(key, "SUBTRACT") == 0 )
			return VK_SUBTRACT;
		else if ( stricmp(key, "DIVIDE") == 0 )
			return VK_DIVIDE;
		else if ( stricmp(key, "MULTIPLY") == 0 )
			return VK_MULTIPLY;
		else if ( stricmp(key, "DECIMAL") == 0 )
			return VK_DECIMAL;
		else if ( stricmp(key, "LWIN") == 0 )
			return VK_LWIN;
		else if ( stricmp(key, "RWIN") == 0 )
			return VK_RWIN;
		else if ( stricmp(key, "APPS") == 0 )
			return VK_APPS;
		// fuming 8-14-2007 "/" "`" "\" 不能轉換123
		//else if ( stricmp(key, "-") == 0 )
		if ( stricmp(key, "-") == 0 )
			return VK_OEM_MINUS;
		else if ( stricmp(key, "=") == 0 )
			return VK_OEM_PLUS;
		else if ( stricmp(key, ",") == 0 )
			return VK_OEM_COMMA;
		else if ( stricmp(key, ".") == 0 )
			return VK_OEM_PERIOD;
		else if ( stricmp(key, ";") == 0 )
			return VK_OEM_1;
		else if ( stricmp(key, "/") == 0 )
			return VK_OEM_2;
		else if ( stricmp(key, "`") == 0 )
			return VK_OEM_3;
		else if ( stricmp(key, "[") == 0 )
			return VK_OEM_4;
		else if ( stricmp(key, "\\") == 0 )
			return VK_OEM_5;
		else if ( stricmp(key, "]") == 0 )
			return VK_OEM_6;
		else if ( stricmp(key, "'") == 0 )
			return VK_OEM_7;
		else if ( stricmp(key, "PRTSCR") == 0 )
			return VK_SNAPSHOT;
		else if ( stricmp(key, "SCROLL") == 0 )
			return VK_SCROLL;
		else if ( stricmp(key, "PAUSE") == 0 )
			return VK_PAUSE;
		// fuming 8-10-2007
		else if ( stricmp(key, "PAGEUP") == 0 )
			return VK_PRIOR;
		else if ( stricmp(key, "PAGEDOWN") == 0 )
			return VK_NEXT;
	}	
	return 0;
}

// ----------------------------------------------------------------------------------
void CUiFrame::CallLuaFunc(lua_State* L, const char* type_key, int narg)
{
	assert(L);

	int pop_count = narg + 1;
	CUiLuaLayout::PushLayoutTable(L, this);
	if (!lua_isnil(L, -1))
	{
		pop_count++;
		lua_pushstring(L, type_key);
		lua_rawget(L, -2);
		if (!lua_isnil(L, -1))
		{
			lua_insert(L, -(2 + narg));					// move function
			lua_insert(L, -(1 + narg));					// move table
			if (lua_pcall(L, 1+narg, 0, 0) != 0)
			{
				char temp[1024];
				sprintf_s(temp, 1024, "call %s's %s, line: %s", m_name, GetEventName(type_key), lua_tostring(L, -1));
				lua_pop(L, 1);
				UiOuputError(temp);
			}
			pop_count -= 2 + narg;
		}
	}
	lua_pop(L, pop_count);
}

// ----------------------------------------------------------------------------------
void CUiFrame::OnLoad()
{
	if ( m_lockEvent.LockEvent(eEventType_Load) == false )
		return;

	for (list<CUiFrame*>::reverse_iterator iter = m_child.rbegin(); iter != m_child.rend(); iter++ )
		(*iter)->OnLoad();	

	lua_State* L = GetLuaState();
	if (L && m_eventFlag.GetBits(eEventType_Load))
		CallLuaFunc(L, UI_LUA_ONLOAD, 0);

	m_lockEvent.UnlockEvent(eEventType_Load);
}

// ----------------------------------------------------------------------------------
void CUiFrame::OnResize()
{	
	if ( m_lockEvent.LockEvent(eEventType_Resize) == false )
		return;

	lua_State* L = GetLuaState();
	if (L && m_eventFlag.GetBits(eEventType_Resize))
	{
		CallLuaFunc(L, UI_LUA_ONRESIZE, 0);
	}

	m_lockEvent.UnlockEvent(eEventType_Resize);
}

// ----------------------------------------------------------------------------------
void CUiFrame::OnShow(lua_State* L)
{
	if ( m_lockEvent.LockEvent(eEventType_Show) == false )
		return;

	for (list<CUiFrame*>::reverse_iterator iter = m_child.rbegin(); iter != m_child.rend(); iter++ )
	{
		if ( (*iter)->m_isVisible )
			(*iter)->OnShow(L);
	}
	if (m_eventFlag.GetBits(eEventType_Show))
		CallLuaFunc(L, UI_LUA_ONSHOW, 0);
	if (m_pfOnShow)
		m_pfOnShow(this, 0, 0);	

	m_lockEvent.UnlockEvent(eEventType_Show);
}

// ----------------------------------------------------------------------------------
void CUiFrame::OnHide(lua_State* L)
{
	if ( m_lockEvent.LockEvent(eEventType_Hide) == false )
		return;

	/*
	for (list<CUiFrame*>::reverse_iterator iter = m_child.rbegin(); iter != m_child.rend(); iter++ )
		(*iter)->OnHide(L);
	*/
	
	// 取消元件下面所有子元件的鎖定狀熊
	KillFocus();

	if (m_eventFlag.GetBits(eEventType_Hide))
		CallLuaFunc(L, UI_LUA_ONHIDE, 0);
	if (m_pfOnHide)
		m_pfOnHide(this, 0, 0);

	m_lockEvent.UnlockEvent(eEventType_Hide);
}

// ----------------------------------------------------------------------------------
void CUiFrame::OnEnalbe(lua_State* L)
{
	if ( m_lockEvent.LockEvent(eEventType_Enable) == false )
		return;
	
	if (m_eventFlag.GetBits(eEventType_Enable))
		CallLuaFunc(L, UI_LUA_ONENABLE, 0);

	m_lockEvent.UnlockEvent(eEventType_Enable);
}

// ----------------------------------------------------------------------------------
void CUiFrame::OnDisalbe(lua_State* L)
{
	if ( m_lockEvent.LockEvent(eEventType_Disable) == false )
		return;

	// 取消元件下面所有子元件的鎖定狀熊
	KillFocus();

	if (m_eventFlag.GetBits(eEventType_Disable))
		CallLuaFunc(L, UI_LUA_ONDISABLE, 0);

	m_lockEvent.UnlockEvent(eEventType_Disable);
}

// ----------------------------------------------------------------------------------
void CUiFrame::OnUpdate(float elapsedTime)
{
	if ( m_lockEvent.LockEvent(eEventType_Update) == false )
		return;

	lua_State* L = GetLuaState();
	if (L && m_eventFlag.GetBits(eEventType_Update))
	{
		lua_pushnumber(L, elapsedTime);
		CallLuaFunc(L, UI_LUA_ONUPDATE, 1);
	}

	m_lockEvent.UnlockEvent(eEventType_Update);
}

// ----------------------------------------------------------------------------------
void CUiFrame::OnPaint(float alpha)
{
	if ( m_pfOnPaint )
		m_pfOnPaint(this, (int)alpha * 100, 0);
}

// ----------------------------------------------------------------------------------
void CUiFrame::OnMouseLeave(CUiPoint pt)
{
	if ( m_lockEvent.LockEvent(eEventType_MouseLeave) == false )
		return;

	lua_State* L = GetLuaState();
	if (L && m_eventFlag.GetBits(eEventType_MouseLeave))
	{
		CallLuaFunc(L, UI_LUA_ONMOUSELEAVE, 0);
	}
	if (m_pfOnMouseLeave)
		m_pfOnMouseLeave(this, 0, 0);

	m_lockEvent.UnlockEvent(eEventType_MouseLeave);
}

// ----------------------------------------------------------------------------------
void CUiFrame::OnMouseEnter(CUiPoint pt)
{
	if ( m_lockEvent.LockEvent(eEventType_MouseEnter) == false )
		return;

	lua_State* L = GetLuaState();
	if (L && m_eventFlag.GetBits(eEventType_MouseEnter))
	{
		CallLuaFunc(L, UI_LUA_ONMOUSEENTER, 0);
	}
	if (m_pfOnMouseEnter)
		m_pfOnMouseEnter(this, 0, 0);

	m_lockEvent.UnlockEvent(eEventType_MouseEnter);
}

// ----------------------------------------------------------------------------------
void CUiFrame::OnMouseMove(CUiPoint pt)
{
	/*
	if ( m_isMovable && m_pUi->GetDragFrame() == this )
	{
		CUiPoint pt = GetPos();
		pt.m_x += input->GetXPos() - input->GetXOldPos();
		pt.m_y += input->GetYPos() - input->GetYOldPos();
		SetPos(pt);
	}
	*/

	// 若沒有元件在拖曳狀態,檢查元件自己是否可以觸發拖曳事件
	if (m_pUi->GetDragFrame() == NULL && m_pUi->GetCapture() == this)
	{
		int key = m_pUi->GetCaptureKey();		
		if ( m_dragSet & (0x01 << key) && m_pUi->GetMouseDown(key) )
		{
			CUiPoint offset = pt - m_pUi->GetCapturePoint();
			if ( abs(offset.m_x) > 4.0f || abs(offset.m_y) > 4.0f )
			{
				m_pUi->SetDragFrame( this );
				OnDragBegin();
			}
		}
	}	
	if (m_pfOnMouseMove)
		m_pfOnMouseMove(this, 0, 0);
}

// ----------------------------------------------------------------------------------
void CUiFrame::OnMouseWheel(CUiPoint pt, int delta)
{
	if ( m_lockEvent.LockEvent(eEventType_MouseWheel) == false )
		return;

	lua_State* L = GetLuaState();
	if (L && m_eventFlag.GetBits(eEventType_MouseWheel))
	{
		lua_pushnumber(L, delta);
		CallLuaFunc(L, UI_LUA_ONMOUSEWHEEL, 1);
	}
	if ( m_pfOnMouseWheel )
		m_pfOnMouseWheel(this, delta, 0);

	m_lockEvent.UnlockEvent(eEventType_MouseWheel);
}

// ----------------------------------------------------------------------------------
void CUiFrame::OnClick(CUiPoint pt, int key)
{
	if ( m_lockEvent.LockEvent(eEventType_Click) == false )
		return;

	lua_State* L = GetLuaState();
	if (L && m_eventFlag.GetBits(eEventType_Click))
	{
		lua_pushstring(L, GetButtonName(key));
		CallLuaFunc(L, UI_LUA_ONCLICK, 1);
	}
	if ( m_pfOnClick )
		m_pfOnClick(this, key, (INT64)&pt);

	m_lockEvent.UnlockEvent(eEventType_Click);
}

// ----------------------------------------------------------------------------------
void CUiFrame::OnDoubleClick(CUiPoint pt, int key)
{
	if ( m_lockEvent.LockEvent(eEventType_DoubleClick) == false )
		return;

	lua_State* L = GetLuaState();
	if (L && m_eventFlag.GetBits(eEventType_DoubleClick))
	{
		lua_pushstring(L, GetButtonName(key));
		CallLuaFunc(L, UI_LUA_ONDOUBLECLICK, 1);
	}
	if ( m_pfOnDoubleClick )
		m_pfOnDoubleClick(this, key, (INT64)&pt);

	m_lockEvent.UnlockEvent(eEventType_DoubleClick);
}

// ----------------------------------------------------------------------------------
void CUiFrame::OnMouseDown(CUiPoint pt, int key)
{
	if ( m_lockEvent.LockEvent(eEventType_MouseDown) == false )
		return;

	lua_State* L = GetLuaState();
	if (L && m_eventFlag.GetBits(eEventType_MouseDown))
	{
		lua_pushstring(L, GetButtonName(key));
		CallLuaFunc(L, UI_LUA_ONMOUSEDOWN, 1);
	}
	if ( m_pfOnMouseDown )
		m_pfOnMouseDown(this, key, (INT64)&pt);

	m_lockEvent.UnlockEvent(eEventType_MouseDown);
}

// ----------------------------------------------------------------------------------
void CUiFrame::OnMouseUp(CUiPoint pt, int key)
{
	if ( m_lockEvent.LockEvent(eEventType_MouseUp) == false )
		return;

	lua_State* L = GetLuaState();
	if (L && m_eventFlag.GetBits(eEventType_MouseUp))
	{
		lua_pushstring(L, GetButtonName(key));
		CallLuaFunc(L, UI_LUA_ONMOUSEUP, 1);
	}
	if ( m_pfOnMouseUp )
		m_pfOnMouseUp(this, key, (INT64)&pt);

	m_lockEvent.UnlockEvent(eEventType_MouseUp);
}

// ----------------------------------------------------------------------------------
void CUiFrame::OnDragBegin()
{
	if ( m_lockEvent.LockEvent(eEventType_DragBegin) == false )
		return;

	lua_State* L = GetLuaState();
	if (L && m_eventFlag.GetBits(eEventType_DragBegin))
	{		
		CallLuaFunc(L, UI_LUA_ONDRAGBEGIN, 0);
	}
	if ( m_pfOnDragBegin )
		m_pfOnDragBegin(this, 0, 0);

	m_lockEvent.UnlockEvent(eEventType_DragBegin);
}

// ----------------------------------------------------------------------------------
void CUiFrame::OnDragStop()
{
	if ( m_lockEvent.LockEvent(eEventType_DragStop) == false )
		return;

	m_pUi->SetDragFrame(NULL);

	lua_State* L = GetLuaState();
	if (L && m_eventFlag.GetBits(eEventType_DragStop))
	{		
		CallLuaFunc(L, UI_LUA_ONDRAGSTOP, 0);
	}
	if ( m_pfOnDragStop )
		m_pfOnDragStop(this, 0, 0);

	m_lockEvent.UnlockEvent(eEventType_DragStop);
}

// ----------------------------------------------------------------------------------
void CUiFrame::OnReceiveDrag()
{
	if ( m_lockEvent.LockEvent(eEventType_ReceiveDrag) == false )
		return;

	lua_State* L = GetLuaState();
	if (L && m_eventFlag.GetBits(eEventType_ReceiveDrag))
	{		
		CallLuaFunc(L, UI_LUA_ONRECEIVEDRAG, 0);
	}
	if ( m_pfOnReceiveDrag )
		m_pfOnReceiveDrag(this, 0, 0);

	m_lockEvent.UnlockEvent(eEventType_ReceiveDrag);
}

// ----------------------------------------------------------------------------------
void CUiFrame::OnChar(WPARAM wParam, LPARAM lParam)
{
	if ( m_lockEvent.LockEvent(eEventType_Char) == false )
		return;

	lua_State* L = GetLuaState();
	if (L && m_eventFlag.GetBits(eEventType_Char))
	{		
		lua_pushstring(L, GetKeyName(wParam));
		CallLuaFunc(L, UI_LUA_ONCHAR, 1);
	}

	m_lockEvent.UnlockEvent(eEventType_Char);
}

// ----------------------------------------------------------------------------------
void CUiFrame::OnKeyDown(WPARAM wParam, LPARAM lParam)
{
	if ( m_lockEvent.LockEvent(eEventType_KeyDown) == false )
		return;

	lua_State* L = GetLuaState();
	if (L && m_eventFlag.GetBits(eEventType_KeyDown))
	{		
		lua_pushstring(L, GetKeyName(wParam));
		CallLuaFunc(L, UI_LUA_ONKEYDOWN, 1);
	}

	m_lockEvent.UnlockEvent(eEventType_KeyDown);
}

// ----------------------------------------------------------------------------------
void CUiFrame::OnKeyUp(WPARAM wParam, LPARAM lParam)
{
	if ( m_lockEvent.LockEvent(eEventType_KeyUp) == false )
		return;

	lua_State* L = GetLuaState();
	if (L && m_eventFlag.GetBits(eEventType_KeyUp))
	{		
		lua_pushstring(L, GetKeyName(wParam));
		CallLuaFunc(L, UI_LUA_ONKEYUP, 1);
	}

	m_lockEvent.UnlockEvent(eEventType_KeyUp);
}

// ----------------------------------------------------------------------------------
void CUiFrame::OnEnterPressed()
{
	if ( m_lockEvent.LockEvent(eEventType_EnterPressed) == false )
		return;

	lua_State* L = GetLuaState();
	if (L && m_eventFlag.GetBits(eEventType_EnterPressed))
	{
		CallLuaFunc(L, UI_LUA_ONENTERPRESSED, 0);
	}

	m_lockEvent.UnlockEvent(eEventType_EnterPressed);
}

// ----------------------------------------------------------------------------------
void CUiFrame::OnTabPressed()
{
	if ( m_lockEvent.LockEvent(eEventType_TabPressed) == false )
		return;

	lua_State* L = GetLuaState();
	if (L && m_eventFlag.GetBits(eEventType_TabPressed))
	{		
		CallLuaFunc(L, UI_LUA_ONTABPRESSED, 0);
	}

	m_lockEvent.UnlockEvent(eEventType_TabPressed);
}

// ----------------------------------------------------------------------------------
void CUiFrame::OnSpacePressed()
{
	if ( m_lockEvent.LockEvent(eEventType_SpacePressed) == false )
		return;

	lua_State* L = GetLuaState();
	if (L && m_eventFlag.GetBits(eEventType_SpacePressed))
	{		
		CallLuaFunc(L, UI_LUA_ONSPACEPRESSED, 0);
	}

	m_lockEvent.UnlockEvent(eEventType_SpacePressed);
}

// ----------------------------------------------------------------------------------
void CUiFrame::OnEscapePressed()
{
	if ( m_lockEvent.LockEvent(eEventType_EscapePressed) == false )
		return;

	lua_State* L = GetLuaState();
	if (L && m_eventFlag.GetBits(eEventType_EscapePressed))
	{		
		CallLuaFunc(L, UI_LUA_ONESCAPEPRESSED, 0);
	}

	m_lockEvent.UnlockEvent(eEventType_EscapePressed);
}

// ----------------------------------------------------------------------------------
void CUiFrame::OnArrowUpPressed()
{
	if ( m_lockEvent.LockEvent(eEventType_ArrowUpPressed) == false )
		return;

	lua_State* L = GetLuaState();
	if (L && m_eventFlag.GetBits(eEventType_ArrowUpPressed))
	{		
		CallLuaFunc(L, UI_LUA_ONARROWUPPRESSED, 0);
	}

	m_lockEvent.UnlockEvent(eEventType_ArrowUpPressed);
}

// ----------------------------------------------------------------------------------
void CUiFrame::OnArrowDownPressed()
{
	if ( m_lockEvent.LockEvent(eEventType_ArrowDownPressed) == false )
		return;

	lua_State* L = GetLuaState();
	if (L && m_eventFlag.GetBits(eEventType_ArrowDownPressed))
	{		
		CallLuaFunc(L, UI_LUA_ONARROWDOWNPRESSED, 0);
	}

	m_lockEvent.UnlockEvent(eEventType_ArrowDownPressed);
}

// ----------------------------------------------------------------------------------
void CUiFrame::OnTextChanged()
{
	if ( m_lockEvent.LockEvent(eEventType_TextChanged) == false )
		return;

	lua_State* L = GetLuaState();
	if (L && m_eventFlag.GetBits(eEventType_TextChanged))
	{
		CallLuaFunc(L, UI_LUA_ONTEXTCHANGED, 0);
	}

	m_lockEvent.UnlockEvent(eEventType_TextChanged);
}

// ----------------------------------------------------------------------------------
void CUiFrame::OnValueChanged(float arg1)
{
	if ( m_lockEvent.LockEvent(eEventType_ValueChanged) == false )
		return;

	lua_State* L = GetLuaState();
	if (L && m_eventFlag.GetBits(eEventType_ValueChanged))
	{
		lua_pushnumber(L, arg1);
		CallLuaFunc(L, UI_LUA_ONVALUECHANGED, 1);
	}

	m_lockEvent.UnlockEvent(eEventType_ValueChanged);
}

// ----------------------------------------------------------------------------------
void CUiFrame::OnVerticalScroll(float arg1)
{
	if ( m_lockEvent.LockEvent(eEventType_VerticalScroll) == false )
		return;

	lua_State* L = GetLuaState();
	if (L && m_eventFlag.GetBits(eEventType_VerticalScroll))
	{
		lua_pushnumber(L, arg1);
		CallLuaFunc(L, UI_LUA_ONVERTICALSCROLL, 1);
	}

	m_lockEvent.UnlockEvent(eEventType_VerticalScroll);
}

// ----------------------------------------------------------------------------------
void CUiFrame::OnHorizontalScroll(float arg1)
{
	if ( m_lockEvent.LockEvent(eEventType_HorizontalScroll) == false )
		return;

	lua_State* L = GetLuaState();
	if (L && m_eventFlag.GetBits(eEventType_HorizontalScroll))
	{
		lua_pushnumber(L, arg1);
		CallLuaFunc(L, UI_LUA_ONHORIZONTALSCROLL, 1);
	}

	m_lockEvent.UnlockEvent(eEventType_HorizontalScroll);
}

// ----------------------------------------------------------------------------------
void CUiFrame::OnScrollRangeChanged(float arg1, float arg2)
{
	if ( m_lockEvent.LockEvent(eEventType_ScrollRangeChanged) == false )
		return;

	lua_State* L = GetLuaState();
	if (L && m_eventFlag.GetBits(eEventType_ScrollRangeChanged))
	{
		lua_pushnumber(L, arg1);
		lua_pushnumber(L, arg2);
		CallLuaFunc(L, UI_LUA_ONSCROLLRANGECHANGED, 2);
	}

	m_lockEvent.UnlockEvent(eEventType_ScrollRangeChanged);
}

// ----------------------------------------------------------------------------------
void CUiFrame::OnEvent(const char* event)
{
	/*
	if ( m_lockEvent.LockEvent(eEventType_Event) == false )
		return;
	*/

	m_eventCount++;
	if ( m_eventCount > 10 )
		return;

	lua_State* L = GetLuaState();
	if (L && m_eventFlag.GetBits(eEventType_Event))
	{
        lua_pushstring(L, event);
		CallLuaFunc(L, UI_LUA_ONEVENT, 1);
	}

	m_lockEvent.UnlockEvent(eEventType_Event);
	m_eventCount = 0;
}

// ----------------------------------------------------------------------------------
void CUiFrame::OnEditFocusGained(CUiFrame* lost)
{	
	if ( m_lockEvent.LockEvent(eEventType_EditFocusGained) == false )
		return;

	lua_State* L = GetLuaState();
	if ( L && m_eventFlag.GetBits(eEventType_EditFocusGained) ) {
		//CUiLuaLayout::PushLayoutTable(L, lost);
		CallLuaFunc(L, UI_LUA_ONEDITFOCUSGAINED, 0);
	}

	m_lockEvent.UnlockEvent(eEventType_EditFocusGained);
}

// ----------------------------------------------------------------------------------
void CUiFrame::OnEditFocusLost(CUiFrame* gain)
{
	if ( m_lockEvent.LockEvent(eEventType_EditFocusLost) == false )
		return;

	lua_State* L = GetLuaState();
	if ( L && m_eventFlag.GetBits(eEventType_EditFocusLost) ) {
		//CUiLuaLayout::PushLayoutTable(L, gain);
		CallLuaFunc(L, UI_LUA_ONEDITFOCUSLOST, 0);
	}

	m_lockEvent.UnlockEvent(eEventType_EditFocusLost);
}

// ----------------------------------------------------------------------------------
// x, y 游標位置
void CUiFrame::OnEditCursorChanged(int ch, int ln)
{
	if ( m_lockEvent.LockEvent(eEventType_EditCursorChanged) == false )
		return;

	lua_State* L = GetLuaState();
	if ( L && m_eventFlag.GetBits(eEventType_EditCursorChanged) ) {
		lua_pushnumber(L, ch);
		lua_pushnumber(L, ln);
		CallLuaFunc(L, UI_LUA_ONEDITCURSORCHANGED, 2);
	}

	m_lockEvent.UnlockEvent(eEventType_EditCursorChanged);
}

// ----------------------------------------------------------------------------------
void CUiFrame::OnHyperlinkEnter(const WCHAR* link)
{
	if ( m_lockEvent.LockEvent(eEventType_HyperlinkEnter) == false )
		return;

	lua_State* L = GetLuaState();
	if ( L && m_eventFlag.GetBits(eEventType_HyperlinkEnter) ) {
		lua_pushstring(L, link);
		CallLuaFunc(L, UI_LUA_ONHYPERLINKENTER, 1);
	}

	m_lockEvent.UnlockEvent(eEventType_HyperlinkEnter);
}

// ----------------------------------------------------------------------------------
void CUiFrame::OnHyperlinkLeave(const WCHAR* link)
{
	if ( m_lockEvent.LockEvent(eEventType_HyperlinkLeave) == false )
		return;

	lua_State* L = GetLuaState();
	if ( L && m_eventFlag.GetBits(eEventType_HyperlinkLeave) ) {
		lua_pushstring(L, link);
		CallLuaFunc(L, UI_LUA_ONHYPERLINKLEAVE, 1);
	}

	m_lockEvent.UnlockEvent(eEventType_HyperlinkLeave);
}

// ----------------------------------------------------------------------------------
void CUiFrame::OnHyperlinkClick(const WCHAR* link, int key)
{
	if ( m_lockEvent.LockEvent(eEventType_HyperlinkClick) == false )
		return;

	lua_State* L = GetLuaState();
	if ( L && m_eventFlag.GetBits(eEventType_HyperlinkClick) ) {
		lua_pushstring(L, link);
		lua_pushstring(L, GetButtonName(key));
		CallLuaFunc(L, UI_LUA_ONHYPERLINKCLICK, 2);
	}

	m_lockEvent.UnlockEvent(eEventType_HyperlinkClick);
}

// ----------------------------------------------------------------------------------
void CUiFrame::KillFocus(int flag)
{
	if ( m_pUi == NULL )
		return;

	if ( (flag & 0x01) && m_pUi->GetKeyboardFocus() == this )
		m_pUi->SetKeyboardFocus(NULL);
	if ( (flag & 0x02) && m_pUi->GetCapture() == this )
		m_pUi->SetCapture(NULL, 0);
	if ( m_pUi->GetDragFrame() == this )
		OnDragStop();
	for ( list<CUiFrame*>::iterator iter = m_child.begin(); iter != m_child.end(); iter++ )
		(*iter)->KillFocus(flag);
}

// ----------------------------------------------------------------------------------
void CUiFrame::Locked()
{
	if ( m_pUi ) {
		m_pUi->LockedFrame(this);
	}
}

// ----------------------------------------------------------------------------------
void CUiFrame::Unlocked()
{
	if ( m_pUi ) {
		m_pUi->UnlockedFrame(this);
	}
}

// ----------------------------------------------------------------------------------
void CUiFrame::EraseChild(CUiFrame *pUiFrame)
{
	assert( pUiFrame );
	for ( list<CUiFrame*>::iterator iter = m_child.begin(); iter != m_child.end(); iter++ )
	{
		if ( (*iter) == pUiFrame )
		{
			m_child.erase( iter );
			break;
		}
	}
}

// ----------------------------------------------------------------------------------
void CUiFrame::InsertChild(CUiFrame *pUiFrame)
{
	assert( pUiFrame );

	bool isInsert = false;

	// 找尋適當插入點
	for ( list<CUiFrame*>::iterator iter = m_child.begin(); iter != m_child.end(); iter++ )
	{
		CUiFrame* frame = *iter;
		if ( frame->m_frameStrata < pUiFrame->m_frameStrata )
			continue;

		if ( frame->m_frameStrata > pUiFrame->m_frameStrata || frame->m_topLevel || frame->m_frameLevel > pUiFrame->m_frameLevel )
		{
			m_child.insert(iter, pUiFrame);
			isInsert = true;
			break;
		}
	}

	if ( !isInsert )
		m_child.push_back(pUiFrame);
}

// ----------------------------------------------------------------------------------
void CUiFrame::AdjustFrameLevel()
{
	// 更新中不能更換子元件位置
	if ( m_state.isLockedChild )
		return;

	CUiFrame* pParent = dynamic_cast<CUiFrame*>(m_pParent);
	if (pParent && !pParent->m_state.isLockedChild)
	{
		pParent->EraseChild(this);
		pParent->InsertChild(this);
		pParent->AdjustFrameLevel();
	}
}

// ----------------------------------------------------------------------------------
void CUiFrame::SetParent(CUiFrame* parent)
{
	if ( m_pUi == NULL || parent == NULL )
		return;		

	// 檢測新父元件是否存在
	switch ( parent->GetUiType() )
	{
	case eUiType_Frame:
	case eUiType_Model:
	case eUiType_Button:
	case eUiType_CheckButton:
	case eUiType_RadioButton:
	case eUiType_StatusBar:
	case eUiType_Slider:
	case eUiType_EditBox:
	case eUiType_ListBox:
	case eUiType_MessageFrame:
	case eUiType_ScrollFrame:
	case eUiType_Tooltip:
		break;

	default:
		return;
	}

	// 移除舊的
	CUiFrame* pParent = dynamic_cast<CUiFrame*>(m_pParent);
	if ( pParent )
		pParent->EraseChild(this);

	// 加入新的
	parent->InsertChild(this);
}

// ----------------------------------------------------------------------------------
int CUiFrame::GetMemSize()
{
	int size = CUiLayout::GetMemSize() + sizeof(CUiFrame) - sizeof(CUiLayout);

	size += CUiLayout::GetMemSize();
	for ( list<CUiFrame*>::iterator iter = m_child.begin(); iter != m_child.end(); iter++ )
		size += (*iter)->GetMemSize();
	for ( multimap<int, CUiLayout*>::iterator iter = m_layers.begin(); iter != m_layers.end(); iter++ )
		size += (iter->second)->GetMemSize();

	size += sizeof(CUiFrame*) * (int)m_child.size() + sizeof(m_child);
	size += sizeof(CUiLayout*) * (int)m_layers.size() + sizeof(m_layers);
	size += sizeof(m_eventFlag);
	size += sizeof(m_pBackdrop);
	if ( m_pBackdrop )
	{
		CTextureImage texture;
		size += sizeof(CTextureBackdrop);

		texture = m_pBackdrop->GetTileTexture();
		if ( texture.GetFileName() )
			size += (int)strlen(texture.GetFileName()) + 1;

		texture = m_pBackdrop->GetEdgeTexture();
		if ( texture.GetFileName() )
			size += (int)strlen(texture.GetFileName()) + 1;
	}

	size += sizeof(m_backdropTileColor);
	size += sizeof(m_backdropEdgeColor);
	size += sizeof(m_hitRectInsets);
	size += sizeof(m_maxResizeBounds);
	size += sizeof(m_minResizeBounds);
	size += sizeof(m_id);
	size += sizeof(m_frameStrata);
	size += sizeof(m_frameLevel);
	size += sizeof(m_clicksSet);
	size += sizeof(m_dragSet);
	size += sizeof(m_topLevel);
	size += sizeof(m_isEnable);
	size += sizeof(m_state);
	size += sizeof(m_sizingAnchorPoint);

	return size;
}