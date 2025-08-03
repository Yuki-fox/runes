#include "xmlcheckbutton.h"

// ----------------------------------------------------------------------------
CXmlCheckButton::CXmlCheckButton(CXmlUi* pXmlUi, CXmlLayout* pParent)
	: CXmlButton(pXmlUi, pParent)
{
	m_type				= eXmlUiType_CheckButton;

	m_checkedTexture	= NULL;
	m_disabledCheckedTexture = NULL;
	m_checked			= false;
	m_autoChecked		= true;
}

// ----------------------------------------------------------------------------
CXmlCheckButton::~CXmlCheckButton()
{
	XML_SAFE_DLETE(m_checkedTexture);
	XML_SAFE_DLETE(m_disabledCheckedTexture);
}

// ----------------------------------------------------------------------------
void CXmlCheckButton::LoadNodeAttribute(const DOMNamedNodeMap* nodeMap)
{
	CXmlButton::LoadNodeAttribute(nodeMap);

	DOMNode* attr;
	if ( (attr = nodeMap->getNamedItem(L"checked")) != NULL ) {
		m_checked = XmlValueToBoolean(attr->getNodeValue());
	}
	if ( (attr = nodeMap->getNamedItem(L"autoChecked")) != NULL ) {
		m_autoChecked = XmlValueToBoolean(attr->getNodeValue());
	}
}

// ----------------------------------------------------------------------------
bool CXmlCheckButton::LoadNodeElement(const DOMNode* node)
{
	if ( CXmlButton::LoadNodeElement(node) ) {
		return true;
	}

	const XMLCh* nodeName = node->getNodeName();
	if ( XMLString::compareString(nodeName, L"CheckedTexture") == 0 ) {
		XML_SAFE_DLETE(m_checkedTexture);
		m_checkedTexture = new CXmlTexture(m_pXmlUi, this);
		m_checkedTexture->LoadFromDOMNode(node);
		return true;
	}
	if ( XMLString::compareString(nodeName, L"DisabledCheckedTexture") == 0 ) {
		XML_SAFE_DLETE(m_disabledCheckedTexture);
		m_disabledCheckedTexture = new CXmlTexture(m_pXmlUi, this);
		m_disabledCheckedTexture->LoadFromDOMNode(node);
		return true;
	}
	return false;
}

// ----------------------------------------------------------------------------
void CXmlCheckButton::Inherit(CXmlLayout* pXmlLayout)
{
	CXmlButton::Inherit(pXmlLayout);
	CXmlCheckButton* pXmlCheckButton = dynamic_cast<CXmlCheckButton*>(pXmlLayout);
	if (pXmlCheckButton)
	{
		m_checked			= pXmlCheckButton->m_checked;
		m_autoChecked		= pXmlCheckButton->m_autoChecked;

		if (pXmlCheckButton->m_checkedTexture)
		{
			XML_SAFE_DLETE(m_checkedTexture);
			m_checkedTexture = new CXmlTexture(m_pXmlUi, this);
			m_checkedTexture->Inherit(pXmlCheckButton->m_checkedTexture);
		}
		if (pXmlCheckButton->m_disabledCheckedTexture)
		{
			XML_SAFE_DLETE(m_disabledCheckedTexture);
			m_disabledCheckedTexture = new CXmlTexture(m_pXmlUi, this);
			m_disabledCheckedTexture->Inherit(pXmlCheckButton->m_disabledCheckedTexture);
		}
	}
}