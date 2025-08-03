#include <windows.h>
#include "uibutton.h"
#include "ui.h"

// ----------------------------------------------------------------------------------
CUiButton::CUiButton(const char* name, CUiLayout* pParent, CUi* pUi)
	: CUiFrame(name, pParent, pUi)
{
	m_uiType			= eUiType_Button;

	m_state.isMouseEnable = true;

	m_normalTexture		= NULL;
	m_pushedTexture		= NULL;
	m_disabledTexture	= NULL;
	m_highlightTexture	= NULL;
	m_normalText		= NULL;
	m_disabledText		= NULL;
	m_highlightText		= NULL;
	m_lockHighlight		= false;
	m_lockPushed		= false;

	m_normalTextColor.m_a		= 1.0f;
	m_normalTextColor.m_r		= 0.0f;
	m_normalTextColor.m_g		= 0.0f;
	m_normalTextColor.m_b		= 0.0f;
	m_disabledTextColor.m_a		= 1.0f;
	m_disabledTextColor.m_r		= 0.7f;
	m_disabledTextColor.m_g		= 0.7f;
	m_disabledTextColor.m_b		= 0.7f;
	m_highlightTextColor.m_a	= 1.0f;
	m_highlightTextColor.m_r	= 1.0f;
	m_highlightTextColor.m_g	= 1.0f;
	m_highlightTextColor.m_b	= 1.0f;	

	m_textAnchor.m_relativeTo	= this;
	m_textAnchor.m_point		= CUiAnchor::eAnchor_Center;
}

// ----------------------------------------------------------------------------------
CUiButton::~CUiButton()
{
	UI_SAFE_DELETE(m_normalTexture);
	UI_SAFE_DELETE(m_pushedTexture);
	UI_SAFE_DELETE(m_disabledTexture);
	UI_SAFE_DELETE(m_highlightTexture);
	UI_SAFE_DELETE(m_normalText);
	UI_SAFE_DELETE(m_disabledText);
	UI_SAFE_DELETE(m_highlightText);
}

// ----------------------------------------------------------------------------------
void CUiButton::SetScrollParent(CUiLayout* layout)
{
	if ( m_normalTexture )
		m_normalTexture->SetScrollParent(layout);
	if ( m_pushedTexture )
		m_pushedTexture->SetScrollParent(layout);
	if (m_disabledTexture)
		m_disabledTexture->SetScrollParent(layout);
	if (m_highlightTexture)
		m_highlightTexture->SetScrollParent(layout);
	if (m_normalText)
		m_normalText->SetScrollParent(layout);
	if (m_disabledText)
		m_disabledText->SetScrollParent(layout);
	if (m_highlightText)
		m_highlightText->SetScrollParent(layout);

	CUiFrame::SetScrollParent(layout);
}

// ----------------------------------------------------------------------------------
void CUiButton::SetNormalTexture(CUiTexture* texture)
{
	if (texture==m_normalTexture)
		return ;
	UI_SAFE_DELETE(m_normalTexture);
	m_normalTexture = texture;
}

// ----------------------------------------------------------------------------------
void CUiButton::SetPushedTexture(CUiTexture* texture)
{
	if (texture==m_pushedTexture)
		return ;
	UI_SAFE_DELETE(m_pushedTexture);
	m_pushedTexture = texture;
}

// ----------------------------------------------------------------------------------
void CUiButton::SetDisabledTexture(CUiTexture* texture)
{
	if (texture==m_disabledTexture)
		return ;
	UI_SAFE_DELETE(m_disabledTexture);
	m_disabledTexture = texture;
}

// ----------------------------------------------------------------------------------
void CUiButton::SetHighlightTexture(CUiTexture* texture)
{
	if (texture==m_highlightTexture)
		return ;
	UI_SAFE_DELETE(m_highlightTexture);
	m_highlightTexture = texture;
}

// ----------------------------------------------------------------------------------
void CUiButton::SetNormalText(CUiFontString* fontString)
{

	if (fontString==m_normalText)
		return ;
	UI_SAFE_DELETE(m_normalText);
	m_normalText = fontString;
}

// ----------------------------------------------------------------------------------
void CUiButton::SetDisabledText(CUiFontString* fontString)
{
	if (fontString==m_disabledText)
		return ;
	UI_SAFE_DELETE(m_disabledText);
	m_disabledText = fontString;
}

// ----------------------------------------------------------------------------------
void CUiButton::SetHighlightText(CUiFontString* fontString)
{
	if (fontString==m_highlightText)
		return ;
	UI_SAFE_DELETE(m_highlightText);
	m_highlightText = fontString;
}

// ----------------------------------------------------------------------------------
void CUiButton::SetText(const WCHAR* text)
{
	if (text)
	{
		if (m_normalText)
			m_normalText->SetText(text);
		if (m_disabledText)
			m_disabledText->SetText(text);
		if (m_highlightText)
			m_highlightText->SetText(text);
	}
}

// ----------------------------------------------------------------------------------
void CUiButton::SetTextA(const char* text)
{
	//WCHAR* tmp = MultiByteToWChar(text);
	WCHAR* tmp = Utf8ToWChar(text);
	SetText(tmp);
	UI_SAFE_DELETE_ARRAY(tmp);
}

// ----------------------------------------------------------------------------------
float CUiButton::GetTextWidth()
{
	float maxWidth = 0.0f;
	if (m_normalText)
	{
		float width = m_normalText->GetWidth();
		if ( width > maxWidth )
			maxWidth = width;
	}
		
	if (m_disabledText)
	{
		float width = m_disabledText->GetWidth();
		if ( width > maxWidth )
			maxWidth = width;
	}
	if (m_highlightText)
	{
		float width = m_highlightText->GetWidth();
		if ( width > maxWidth )
			maxWidth = width;
	}
	return maxWidth;
}

// ----------------------------------------------------------------------------------
float CUiButton::GetTextHeight()
{
	float maxHeight = 0.0f;
	if (m_normalText)
	{
		float height = m_normalText->GetHeight();
		if ( height > maxHeight )
			maxHeight = height;
	}

	if (m_disabledText)
	{
		float height = m_disabledText->GetHeight();
		if ( height > maxHeight )
			maxHeight = height;
	}
	if (m_highlightText)
	{
		float height = m_highlightText->GetHeight();
		if ( height > maxHeight )
			maxHeight = height;
	}
	return maxHeight;
}

// ----------------------------------------------------------------------------------
void CUiButton::SetTextAnchor(CUiAnchor anchor)
{
	m_textAnchor = anchor;
	m_textAnchor.m_relativeTo = this;

	if (m_normalText)
	{
		m_normalText->ClearAllAnchors();
		m_normalText->SetAnchor(anchor);
	}
	if (m_disabledText)
	{
		m_disabledText->ClearAllAnchors();
		m_disabledText->SetAnchor(anchor);
	}
	if (m_highlightText)
	{
		m_highlightText->ClearAllAnchors();
		m_highlightText->SetAnchor(anchor);
	}
}

// ----------------------------------------------------------------------------------
void CUiButton::SetTextColor(CUiColor color)
{
	if (m_normalText)
		m_normalText->SetColor(color.m_r, color.m_g, color.m_b);
	if (m_disabledText)
		m_disabledText->SetColor(color.m_r, color.m_g, color.m_b);
	if (m_highlightText)
		m_highlightText->SetColor(color.m_r, color.m_g, color.m_b);
}

// ----------------------------------------------------------------------------------
int CUiButton::GetMemSize()
{
	int size = CUiFrame::GetMemSize() + sizeof(CUiButton) - sizeof(CUiFrame);

	if ( m_normalTexture )
		size += m_normalTexture->GetMemSize();
	if ( m_pushedTexture )
		size += m_pushedTexture->GetMemSize();
	if ( m_disabledTexture )
		size += m_disabledTexture->GetMemSize();
	if ( m_highlightTexture )
		size += m_highlightTexture->GetMemSize();
	if ( m_normalText )
		size += m_normalText->GetMemSize();
	if ( m_disabledText )
		size += m_disabledText->GetMemSize();
	if ( m_highlightText )
		size += m_highlightText->GetMemSize();

	return size;
}

// ----------------------------------------------------------------------------------
void CUiButton::OnUpdate(float elapsedTime)
{	
	if (m_normalTexture)
		m_normalTexture->Update(elapsedTime);
	if (m_pushedTexture)
		m_pushedTexture->Update(elapsedTime);
	if (m_disabledTexture)
		m_disabledTexture->Update(elapsedTime);
	if (m_highlightTexture)
		m_highlightTexture->Update(elapsedTime);	

	CUiFrame::OnUpdate(elapsedTime);
}

// ----------------------------------------------------------------------------------
void CUiButton::OnPaint(float alpha)
{
	if ( m_isEnable )
	{
		// Texture
		if ( m_lockPushed || (m_pUi->GetMouseDown(MOUSE_LBUTTON) == this || m_pUi->GetMouseDown(MOUSE_RBUTTON) == this) )
		{
			if (m_pushedTexture)
				m_pushedTexture->Render(alpha);
			else if (m_normalTexture)
				m_normalTexture->Render(alpha);
		}
		else
		{
			if (m_normalTexture)
				m_normalTexture->Render(alpha);
		}
		if ( m_lockHighlight || m_pUi->GetMouseOver() == this )
		{
			if (m_highlightTexture)
				m_highlightTexture->Render(alpha);
		}

		// FontString		
		if ( m_lockPushed || (m_pUi->GetMouseDown(MOUSE_LBUTTON) == this || m_pUi->GetMouseDown(MOUSE_RBUTTON) == this) )
		{
			CUiFontString* fotnString = NULL;
			if ( m_highlightText )
				fotnString = m_highlightText;
			else if ( m_normalText )
				fotnString = m_normalText;

			if ( fotnString )
			{
				CUiPoint pos = fotnString->GetPaintPos();
				fotnString->Render(alpha, pos.m_x + m_pushedTextOffset.m_x, pos.m_y + m_pushedTextOffset.m_y);
			}
		}
		else if ( m_lockHighlight || m_pUi->GetMouseOver() == this )
		{
			if (m_highlightText)
				m_highlightText->Render(alpha);
			else if (m_normalText)
				m_normalText->Render(alpha);
		}
		else
		{
			if (m_normalText)
				m_normalText->Render(alpha);
		}

		/*
		if ( m_pUi->GetMouseDown(MOUSE_LBUTTON) == this )
		{
			if (m_normalText)
			{
				CUiPoint pt = m_normalText->GetPos() + m_pushedTextOffset;
				m_normalText->SetColor(m_normalTextColor.m_r, m_normalTextColor.m_g, m_normalTextColor.m_b);
				m_normalText->Render(alpha, pt.m_x, pt.m_y);
			}
		}
		else if ( m_pUi->GetMouseOver() == this )
		{
			if (m_normalText)
			{				
				m_normalText->SetColor(m_highlightTextColor.m_r, m_highlightTextColor.m_g, m_highlightTextColor.m_b);
				m_normalText->Render(alpha);
			}
		}
		else
		{
			if (m_normalText)
			{
				m_normalText->SetColor(m_normalTextColor.m_r, m_normalTextColor.m_g, m_normalTextColor.m_b);
				m_normalText->Render(alpha);
			}
		}
		*/
	}
	else
	{		
		if (m_disabledTexture)
			m_disabledTexture->Render(alpha);
		if (m_disabledText)
			m_disabledText->Render(alpha);
		else if (m_normalText)
			m_normalText->Render(alpha);
		/*
		if (m_disabledTexture)
			m_disabledTexture->Render(alpha);
		if (m_normalText)
		{
			m_normalText->SetColor(m_disabledTextColor.m_r, m_disabledTextColor.m_g, m_disabledTextColor.m_b);
			m_normalText->Render(alpha);
		}
		*/
	}
}