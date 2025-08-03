#include <windows.h>
#include "uistatusbar.h"

// ----------------------------------------------------------------------------------
CUiStatusBar::CUiStatusBar(const char* name, CUiLayout* pParent, CUi* pUi)
	:CUiFrame(name, pParent, pUi)
{
	m_uiType			= eUiType_StatusBar;

	m_barColor.m_a		= 1.0f;
	m_barColor.m_r		= 1.0f;
	m_barColor.m_g		= 1.0f;
	m_barColor.m_b		= 1.0f;
	m_pBarTexture		= NULL;
	m_minValue			= 0.0f;
	m_maxValue			= 0.0f;
	m_stepValue			= 0.0f;
	m_value				= 0.0f;
	m_reverse			= false;
}

// ----------------------------------------------------------------------------------
CUiStatusBar::~CUiStatusBar()
{
	UI_SAFE_DELETE(m_pBarTexture);
}

// ----------------------------------------------------------------------------------
void CUiStatusBar::SetScrollParent(CUiLayout* layout)
{
	if ( m_pBarTexture )
		m_pBarTexture->SetScrollParent(layout);

	CUiFrame::SetScrollParent(layout);
}

// ----------------------------------------------------------------------------------
void CUiStatusBar::SetMaxValue(float maxValue)
{
	if ( m_maxValue != maxValue )
	{
		m_maxValue = maxValue;
		SetValueInternal( m_value );
	}
}

// ----------------------------------------------------------------------------------
void CUiStatusBar::SetMinValue(float minValue)
{
	if ( m_minValue != minValue )
	{
		m_minValue = minValue;
		SetValueInternal( m_value );
	}
}

// ----------------------------------------------------------------------------------
void CUiStatusBar::SetBarTexture(CUiTexture* texture)
{
	UI_SAFE_DELETE(m_pBarTexture);
	m_pBarTexture = texture;
	UpdateBarTexture();
}

// ----------------------------------------------------------------------------------
void CUiStatusBar::SetReverse(bool reverse)
{
	if ( m_reverse != reverse )
	{
		m_reverse = reverse;

		UpdateBarTexture();
	}
}

// ----------------------------------------------------------------------------------
void CUiStatusBar::OnUpdate(float elapsedTime)
{
	CUiFrame::OnUpdate(elapsedTime);
	if (m_pBarTexture)
		m_pBarTexture->Update(elapsedTime);
}

// ----------------------------------------------------------------------------------
void CUiStatusBar::OnPaint(float alpha)
{
	//CUiFrame::OnPaint();

	if ( m_pBarTexture && m_barColor.m_a > 0.05f )
	{
		float value = (m_maxValue - m_minValue <= 0.0f) ? 0.0f : (m_value - m_minValue) / (m_maxValue - m_minValue);
		m_pBarTexture->SetColor(m_barColor.m_r, m_barColor.m_g, m_barColor.m_b);
		m_pBarTexture->SetAlpha(m_barColor.m_a);
		m_pBarTexture->Render(alpha, value , 1.0f);
	}
}

// ----------------------------------------------------------------------------------
void CUiStatusBar::SetValueInternal(float value)
{
	// Clamp to range
	value = max(m_minValue, value);
	value = min(m_maxValue, value);

	if( value == m_value )
		return;

	m_value = value;
	OnValueChanged(m_value);
	// OnValueChanged	
}

// ----------------------------------------------------------------------------------
void CUiStatusBar::UpdateBarTexture()
{
	if ( m_pBarTexture == NULL )
		return;

	m_pBarTexture->SetReverseState(m_reverse);
	m_pBarTexture->SetAnchorSize(CUiSize(0.0f, 0.0f));
	m_pBarTexture->ClearAllAnchors();
	
	CUiAnchor anchor;
	anchor.m_relativeTo = this;
	m_pBarTexture->SetAnchor(anchor);
}

// ----------------------------------------------------------------------------------
int	CUiStatusBar::GetMemSize()
{
	int size = CUiFrame::GetMemSize() + sizeof(CUiStatusBar) - sizeof(CUiFrame);

	if ( m_pBarTexture )
		size += m_pBarTexture->GetMemSize();

	return size;
}
