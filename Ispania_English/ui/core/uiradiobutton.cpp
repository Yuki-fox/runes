#include <windows.h>
#include "uiradiobutton.h"

// ----------------------------------------------------------------------------------
CUiRadioButton::CUiRadioButton(const char* name, CUiLayout* pParent, CUi* pUi)
	: CUiCheckButton(name, pParent, pUi)
{
	m_uiType			= eUiType_RadioButton;

	m_groupId			= 0;
}

// ----------------------------------------------------------------------------------
CUiRadioButton::~CUiRadioButton()
{
}

// ----------------------------------------------------------------------------------
void CUiRadioButton::SetChecked(bool isCheck)
{
	if ( m_isChecked && isCheck )
		return;

	CUiFrame* frame = dynamic_cast<CUiFrame*>(m_pParent);
	// clear all radio checked
	if ( frame && isCheck )
	{
		list<CUiFrame*>& child = frame->GetChildList();
		for ( list<CUiFrame*>::iterator iter = child.begin(); iter != child.end(); iter++ )
		{
			CUiRadioButton* radioButton = dynamic_cast<CUiRadioButton*>(*iter);
			if (radioButton)
			{
				if (radioButton->m_groupId == m_groupId )
					radioButton->m_isChecked = false;
			}
		}
	}

	CUiCheckButton::SetChecked(isCheck);
}

// ----------------------------------------------------------------------------------
int CUiRadioButton::GetMemSize()
{
	int size = CUiCheckButton::GetMemSize() + sizeof(CUiRadioButton) - sizeof(CUiCheckButton);

	return size;
}

// ----------------------------------------------------------------------------------
void CUiRadioButton::OnClick(CUiPoint pt, int key)
{
	if ( m_isAutoCheck )
		SetChecked(true);
	CUiButton::OnClick(pt, key);
}