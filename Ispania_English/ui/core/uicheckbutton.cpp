#include <windows.h>
#include "uicheckbutton.h"

// ----------------------------------------------------------------------------------
CUiCheckButton::CUiCheckButton(const char* name, CUiLayout* pParent, CUi* pUi)
	: CUiButton(name, pParent, pUi)
{
	m_uiType			= eUiType_CheckButton;

	m_checkedTexture	= NULL;
	m_disabledCheckedTexture	= NULL;
	m_isAutoCheck		= true;
	m_isChecked			= false;
}

// ----------------------------------------------------------------------------------
CUiCheckButton::~CUiCheckButton()
{
	UI_SAFE_DELETE(m_checkedTexture);
	UI_SAFE_DELETE(m_disabledCheckedTexture);
}

// ----------------------------------------------------------------------------------
void CUiCheckButton::SetScrollParent(CUiLayout* layout)
{
	if ( m_checkedTexture )
		m_checkedTexture->SetScrollParent(layout);
	if ( m_disabledCheckedTexture )
		m_disabledCheckedTexture->SetScrollParent(layout);

	CUiButton::SetScrollParent(layout);
}

// ----------------------------------------------------------------------------------
void CUiCheckButton::SetCheckedTexture(CUiTexture* texture)
{
	UI_SAFE_DELETE(m_checkedTexture);
	m_checkedTexture = texture;
}

// ----------------------------------------------------------------------------------
void CUiCheckButton::SetDisabledCheckedTexture(CUiTexture* texture)
{
	UI_SAFE_DELETE(m_disabledCheckedTexture);
	m_disabledCheckedTexture = texture;
}

// ----------------------------------------------------------------------------------
void CUiCheckButton::SetChecked(bool isCheck)
{
	if ( m_isChecked != isCheck )
	{
		m_isChecked = isCheck;
		// onValueChanged
	}
}

// ----------------------------------------------------------------------------------
int CUiCheckButton::GetMemSize()
{
	int size = CUiButton::GetMemSize() + sizeof(CUiCheckButton) - sizeof(CUiButton);

	if ( m_checkedTexture )
		size += m_checkedTexture->GetMemSize();
	if ( m_disabledCheckedTexture )
		size += m_disabledCheckedTexture->GetMemSize();	

	return size;
}

// ----------------------------------------------------------------------------------
void CUiCheckButton::OnUpdate(float elapsedTime)
{
	if (m_checkedTexture)
		m_checkedTexture->Update(elapsedTime);
	if (m_disabledCheckedTexture)
		m_disabledCheckedTexture->Update(elapsedTime);

	CUiButton::OnUpdate(elapsedTime);
}

// ----------------------------------------------------------------------------------
void CUiCheckButton::OnPaint(float alpha)
{
	CUiButton::OnPaint(alpha);

	if ( m_isChecked )
	{
		if ( m_isEnable )
		{
			if ( m_checkedTexture )
				m_checkedTexture->Render(alpha);
		}
		else
		{
			if (m_disabledCheckedTexture)
				m_disabledCheckedTexture->Render(alpha);
		}
	}
}

// ----------------------------------------------------------------------------------
void CUiCheckButton::OnClick(CUiPoint pt, int key)
{
	if ( m_isAutoCheck )
		SetChecked(!m_isChecked);
	CUiButton::OnClick(pt, key);
}