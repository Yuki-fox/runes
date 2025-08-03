#include <windows.h>
#include "uislider.h"

// ----------------------------------------------------------------------------------
CUiSlider::CUiSlider(const char* name, CUiLayout* pParent, CUi* pUi)
	: CUiFrame(name, pParent, pUi)
{
	m_uiType			= eUiType_Slider;

	m_pThumbTexture		= NULL;
	m_orientation		= eOrientation_Vertical;
	m_valueStepMode		= eValueStep_Float;
	m_value				= 0.0f;
	m_maxValue			= 0.0f;
	m_minValue			= 0.0f;
	
	m_isThumbPressed	= false;
}

// ----------------------------------------------------------------------------------
CUiSlider::~CUiSlider()
{
	UI_SAFE_DELETE(m_pThumbTexture);
}

// ----------------------------------------------------------------------------------
void CUiSlider::SetThumbTexture(CUiTexture* texture)
{
	UI_SAFE_DELETE(m_pThumbTexture);
	m_pThumbTexture = texture;
}

// ----------------------------------------------------------------------------------
void CUiSlider::SetMaxValue(float maxValue)
{
	if ( m_valueStepMode == eValueStep_Int )
	{
		float f = floor(maxValue);
		maxValue = (f + 0.5f < maxValue) ? f + 1.0f : f;
	}

	if ( m_maxValue != maxValue )
	{
		m_maxValue = maxValue;
		SetValueInternal(m_value, false);

		// OnScrollRangeChanged
		OnScrollRangeChanged(m_minValue, m_maxValue);
	}
}

// ----------------------------------------------------------------------------------
void CUiSlider::SetMinValue(float minValue)
{
	if ( m_valueStepMode == eValueStep_Int )
	{
		float f = floor(minValue);
		minValue = (f + 0.5f < minValue) ? f + 1.0f : f;
	}

	if ( m_minValue != minValue )
	{
		m_minValue = minValue;
		SetValueInternal(m_value);

		// OnScrollRangeChanged
		OnScrollRangeChanged(m_minValue, m_maxValue);
	}
}

// ----------------------------------------------------------------------------------
void CUiSlider::OnUpdate(float elapsedTime)
{
	// CUiFrame::OnUpdate(elapsedTime);
	if (m_pThumbTexture)
		m_pThumbTexture->Update(elapsedTime);
}

// ----------------------------------------------------------------------------------
void CUiSlider::OnPaint(float alpha)
{
	CUiRectangle rect;
	if ( GetThumbRect(rect) )
	{
		CUiPoint pos = GetPaintPos();
		m_pThumbTexture->Render(alpha, rect.m_left + pos.m_x, rect.m_top + pos.m_y, 1.0f, 1.0f);
	}
}

// ----------------------------------------------------------------------------------
void CUiSlider::OnMouseDown(CUiPoint pt, int key)
{
	CUiPoint cursorPt = pt - GetPaintPos();
	CUiRectangle rect;

	switch (key)
	{
	case MOUSE_LBUTTON:
		if ( GetThumbRect(rect) )
		{
			switch (m_orientation)
			{
			case eOrientation_Horizontal:
				if ( cursorPt.m_x < rect.m_left || cursorPt.m_x > rect.m_right )
					SetValueInternal( ValueFromPos(pt) );
				else
				{
					m_isThumbPressed = true;
					m_pressedOffset.m_x = rect.m_left - cursorPt.m_x;
					m_pressedOffset.m_y = rect.m_top - cursorPt.m_y;
				}
				break;

			case eOrientation_Vertical:
				if ( cursorPt.m_y < rect.m_top || cursorPt.m_y > rect.m_bottom )
					SetValueInternal( ValueFromPos(pt) );
				else
				{
					m_isThumbPressed = true;
					m_pressedOffset.m_x = rect.m_left - cursorPt.m_x;
					m_pressedOffset.m_y = rect.m_top - cursorPt.m_y;
				}
				break;
			}
		}
		break;
	}

	CUiFrame::OnMouseDown(pt, key);
}

// ----------------------------------------------------------------------------------
void CUiSlider::OnMouseUp(CUiPoint pt, int key)
{
	switch (key)
	{
	case MOUSE_LBUTTON:
		if ( m_isThumbPressed )
		{
			m_isThumbPressed = false;
			m_pressedOffset.m_x = 0.0f;
			m_pressedOffset.m_y = 0.0f;
		}
		break;
	}
	CUiFrame::OnMouseUp(pt, key);
}

// ----------------------------------------------------------------------------------
void CUiSlider::OnMouseMove(CUiPoint pt)
{
	if (m_isThumbPressed)
	{
		float sliderValue = ValueFromPos(pt + m_pressedOffset);		
		SetValueInternal( sliderValue );
	}

	// CUiFrame::OnMouseMove(pt);
}

// ----------------------------------------------------------------------------------
void CUiSlider::SetValueInternal(float value, bool force)
{
	if ( m_valueStepMode == eValueStep_Int )
	{
		float f = floor(value);
		value = (f + 0.5f < value) ? f + 1.0f : f;
	}

	// Clamp to range
	value = max(m_minValue, value);
	value = min(m_maxValue, value);

	if( force == false && value == m_value )
		return;

	m_value = value;
	OnValueChanged(m_value);
}

// ----------------------------------------------------------------------------------
float CUiSlider::ValueFromPos(CUiPoint& pt)
{
	if (m_pThumbTexture)
	{
		CUiSize size = GetRealSize();
		CUiSize thumbSize = m_pThumbTexture->GetRealSize();
		CUiPoint screenPt = GetPaintPos();

		switch (m_orientation)
		{
		case eOrientation_Horizontal:
			if (size.m_x != thumbSize.m_x)
				return m_minValue + (m_maxValue - m_minValue) * (pt.m_x - screenPt.m_x) / (size.m_x - thumbSize.m_x);
			break;
				
		case eOrientation_Vertical:
			if (size.m_y != thumbSize.m_y)
				return m_minValue + (m_maxValue - m_minValue) * (pt.m_y - screenPt.m_y) / (size.m_y - thumbSize.m_y);
			break;
		}
	}
	return m_minValue;
}

// ----------------------------------------------------------------------------------
bool CUiSlider::GetThumbRect(CUiRectangle &rect)
{
	if (m_pThumbTexture == NULL || m_maxValue == m_minValue)
		return false;

	CUiSize size = GetRealSize();
	CUiSize thumbSize = m_pThumbTexture->GetRealSize();
	switch (m_orientation)
	{
	case eOrientation_Horizontal:
		rect.m_left	= (m_value - m_minValue) * (size.m_x - thumbSize.m_x) / (m_maxValue - m_minValue);		
		rect.m_top	= (size.m_y - thumbSize.m_y) / 2.0f;
		break;

	case eOrientation_Vertical:
		rect.m_left	= (size.m_x - thumbSize.m_x) / 2.0f;
		rect.m_top	= (m_value - m_minValue) * (size.m_y - thumbSize.m_y) / (m_maxValue - m_minValue);
		break;
	}

	rect.m_right	= rect.m_left + thumbSize.m_x;
	rect.m_bottom	= rect.m_top + thumbSize.m_y;

	return true;
}

// ----------------------------------------------------------------------------------
int CUiSlider::GetMemSize()
{
	int size = CUiFrame::GetMemSize() + sizeof(CUiSlider) - sizeof(CUiFrame);

	if ( m_pThumbTexture )
		size += m_pThumbTexture->GetMemSize();

	return size;
}