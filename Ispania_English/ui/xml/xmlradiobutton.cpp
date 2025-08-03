#include "xmlradiobutton.h"

// ----------------------------------------------------------------------------
CXmlRadioButton::CXmlRadioButton(CXmlUi* pXmlUi, CXmlLayout* pParent)
	: CXmlCheckButton(pXmlUi, pParent)
{
	m_type				= eXmlUiType_RadioButton;

	m_groupId			= 0;
}

// ----------------------------------------------------------------------------
CXmlRadioButton::~CXmlRadioButton()
{
}

// ----------------------------------------------------------------------------
void CXmlRadioButton::LoadNodeAttribute(const DOMNamedNodeMap* nodeMap)
{
	CXmlCheckButton::LoadNodeAttribute(nodeMap);

	DOMNode* attr;
	if ( (attr = nodeMap->getNamedItem(L"groupId")) != NULL ) {
		m_groupId = _wtoi(attr->getNodeValue());
	}
}

// ----------------------------------------------------------------------------
void CXmlRadioButton::Inherit(CXmlLayout* pXmlLayout)
{
	CXmlCheckButton::Inherit(pXmlLayout);
	CXmlRadioButton* pXmlRadioButton = dynamic_cast<CXmlRadioButton*>(pXmlLayout);
	if (pXmlRadioButton)
	{
		m_groupId = pXmlRadioButton->m_groupId;
	}
	
}