#include <windows.h>
#include "uiscrollframe.h"

// ----------------------------------------------------------------------------------
CUiScrollFrame::CUiScrollFrame(const char* name, CUiLayout* pParent, CUi* pUi)
	: CUiFrame(name, pParent, pUi)
{
	m_uiType			= eUiType_ScrollFrame;

	m_pScrollChild		= NULL;
	m_scrollRange.m_x	= 0.0f;
	m_scrollRange.m_y	= 0.0f;
}

// ----------------------------------------------------------------------------------
CUiScrollFrame::~CUiScrollFrame()
{
	UI_SAFE_DELETE(m_pScrollChild);
}

// ----------------------------------------------------------------------------------
void CUiScrollFrame::Update(float elapsedTime)
{
	if (!m_isEnable || !m_isVisible)
		return;

	if (m_pScrollChild)
		m_pScrollChild->Update(elapsedTime);

	CUiFrame::Update(elapsedTime);
}

// ----------------------------------------------------------------------------------
void CUiScrollFrame::Render(float alpha)
{
	if ( !m_isVisible )
		return;	

	CUiFrame::Render(alpha);

	if ( m_pScrollChild )
	{
		CRuRectangle scissorRect;
		CUiRectangle rect;

		m_pScrollChild->GetRectangle(&rect);
		//GetRectangle(&rect);
		scissorRect.Set(rect.m_left, rect.m_top, rect.m_right, rect.m_bottom);

		g_ruUIRenderer->SetScissorRect(&scissorRect);
		m_pScrollChild->Render(alpha);
		g_ruUIRenderer->SetScissorRect(NULL);
	}
}

// ----------------------------------------------------------------------------------
CUiFrame* CUiScrollFrame::GetFrameAtPoint(CUiPoint pt, CUiFrame* pExclude)
{
	CUiFrame* pFocus = NULL;
	if (m_isVisible && m_isEnable)
	{
		if ( m_pScrollChild && m_pScrollChild->ContainsPoint(pt) )
		{
			if ( (pFocus = m_pScrollChild->GetFrameAtPoint(pt, pExclude)) != NULL )
				return pFocus;
		}

		for (list<CUiFrame*>::reverse_iterator iter = m_child.rbegin(); iter != m_child.rend(); iter++ )
		{
			if ((pFocus = (*iter)->GetFrameAtPoint(pt , pExclude)) != NULL)
				return pFocus;
		}

		if ( m_state.isMouseEnable && this != pExclude && ContainsPoint(pt) )
			return this;
	}
	return pFocus;
}

// ----------------------------------------------------------------------------------
CUiFrame* CUiScrollFrame::GetFrameAtKeyboard(CUiFrame* pExclude)
{
	CUiFrame* pFocus = NULL;
	if (m_isVisible && m_isEnable)
	{
		if (m_pScrollChild && (pFocus = m_pScrollChild->GetFrameAtKeyboard(pExclude)) != NULL)
			return pFocus;

		for (list<CUiFrame*>::reverse_iterator iter = m_child.rbegin(); iter != m_child.rend(); iter++)
		{
			if ( (pFocus = (*iter)->GetFrameAtKeyboard(pExclude)) != NULL )
				return pFocus;
		}

		if (m_state.isKeyboardEnable && m_uiType != eUiType_EditBox && this != pExclude)
			return this;
	}
	return pFocus;
}

// ----------------------------------------------------------------------------------
void CUiScrollFrame::SetScrollChild(CUiFrame* pFrame)
{
	UI_SAFE_DELETE(m_pScrollChild);

	m_pScrollChild = pFrame;

	if (m_pScrollChild)
	{
		EraseChild(m_pScrollChild);					// 從子元件列表移除

		m_pScrollChild->SetScrollParent(m_pScrollChild);
	}
}

// ----------------------------------------------------------------------------------
void CUiScrollFrame::SetVerticalScroll(float value)
{
	value = max(value, 0);
	if ( m_pScrollChild )
	{
		CUiPoint offset = m_pScrollChild->GetScrollOffset();
		if ( offset.m_y != value )
		{
			offset.m_y = value;
			m_pScrollChild->SetScrollOffset(offset);
			OnVerticalScroll(offset.m_y);
		}
	}
}

// ----------------------------------------------------------------------------------
void CUiScrollFrame::SetHorizontalScroll(float value)
{
	value = max(value, 0);
	if ( m_pScrollChild )
	{
		CUiPoint offset = m_pScrollChild->GetScrollOffset();
		if ( offset.m_x != value )
		{
			offset.m_x = value;
			m_pScrollChild->SetScrollOffset(offset);
			OnHorizontalScroll(offset.m_x);
		}
	}
}

// ----------------------------------------------------------------------------------
void CUiScrollFrame::SetScrollRange(float x, float y, bool forceUpdate)
{
	bool update = forceUpdate;
	if ( x < 0 )
		x = 0.0f;
	if ( y < 0 )
		y = 0.0f;

	if ( x != m_scrollRange.m_x || y != m_scrollRange.m_y )
	{
		m_scrollRange.m_x = x;
		m_scrollRange.m_y = y;

		update = true;
	}

	if ( update )
	{
		// OnScrollRangeChanged
		OnScrollRangeChanged(m_scrollRange.m_x, m_scrollRange.m_y);
	}
}

// ----------------------------------------------------------------------------------
void CUiScrollFrame::UpdateScrollChildRect()
{
	if ( m_pScrollChild )
	{
		CUiRectangle childRect, rect;
		m_pScrollChild->GetRectangle(&rect, false);

		childRect = rect;
		m_pScrollChild->GetScrollChildRect(&childRect);

		float scale = m_pScrollChild->GetRealScale();
		float width = (childRect.RectWidth() - rect.RectWidth())/scale;
		float height = (childRect.RectHeight() - rect.RectHeight())/scale;
		SetScrollRange(width, height, true);
	}
}

// ----------------------------------------------------------------------------------
void CUiScrollFrame::InsertChild(CUiFrame *pUiFrame)
{
	if ( pUiFrame != m_pScrollChild )
		CUiFrame::InsertChild(pUiFrame);
}

// ----------------------------------------------------------------------------------
void CUiScrollFrame::OnLoad()
{
	if ( m_pScrollChild )
		m_pScrollChild->OnLoad();

	CUiFrame::OnLoad();
}

// ----------------------------------------------------------------------------------
void CUiScrollFrame::KillFocus(int flag)
{
	if ( m_pScrollChild )
	{
		m_pScrollChild->KillFocus(flag);
	}

	CUiFrame::KillFocus(flag);
}

// ----------------------------------------------------------------------------------
int CUiScrollFrame::GetMemSize()
{
	int size = CUiFrame::GetMemSize() + sizeof(CUiScrollFrame) - sizeof(CUiFrame);

	return size;
}