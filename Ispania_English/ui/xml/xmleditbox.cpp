#include "xmleditbox.h"

// ----------------------------------------------------------------------------
CXmlEditBox::CXmlEditBox(CXmlUi* pXmlUi, CXmlLayout* pParent)
	: CXmlFrame(pXmlUi, pParent)
{
	m_type				= eXmlUiType_EditBox;

	m_fontString		= NULL;
	m_letters			= 255;
	m_historyLines		= 0;
	m_isNumeric			= false;
	m_isPassword		= false;
	m_isMultiLine		= false;
	m_isAutoFocus		= true;

	m_highlightColor.m_a	= 0.75f;
	m_highlightColor.m_r	= 0.75f;
	m_highlightColor.m_g	= 0.75f;
	m_highlightColor.m_b	= 0.75f;
}

// ----------------------------------------------------------------------------
CXmlEditBox::~CXmlEditBox()
{
	XML_SAFE_DLETE(m_fontString);
}

// ----------------------------------------------------------------------------
void CXmlEditBox::LoadNodeAttribute(const DOMNamedNodeMap* nodeMap)
{
	CXmlFrame::LoadNodeAttribute(nodeMap);

	DOMNode* attr;
	if ( (attr = nodeMap->getNamedItem(L"letters")) != NULL ) {
		m_letters = _wtoi(attr->getNodeValue());
	}	
	if ( (attr = nodeMap->getNamedItem(L"historyLines")) != NULL ) {
		m_historyLines = _wtoi(attr->getNodeValue());
	}
	if ( (attr = nodeMap->getNamedItem(L"numeric")) != NULL ) {
		m_isNumeric = XmlValueToBoolean(attr->getNodeValue());
	}
	if ( (attr = nodeMap->getNamedItem(L"password")) != NULL ) {
		m_isPassword = XmlValueToBoolean(attr->getNodeValue());
	}
	if ( (attr = nodeMap->getNamedItem(L"multiLine")) != NULL ) {
		m_isMultiLine = XmlValueToBoolean(attr->getNodeValue());
	}
	if ( (attr = nodeMap->getNamedItem(L"autoFocus")) != NULL ) {
		m_isAutoFocus = XmlValueToBoolean(attr->getNodeValue());
	}
}

// ----------------------------------------------------------------------------
bool CXmlEditBox::LoadNodeElement(const DOMNode* node)
{
	if ( CXmlFrame::LoadNodeElement(node) ) {
		return true;
	}

	const XMLCh* nodeName = node->getNodeName();
	if ( XMLString::compareString(nodeName, L"FontString") == 0 ) {
		XML_SAFE_DLETE(m_fontString);
		m_fontString = new CXmlFontString(m_pXmlUi, this);
		m_fontString->LoadFromDOMNode(node);
		return true;
	}
	if ( XMLString::compareString(nodeName, L"HighlightColor") == 0 ) {
		m_highlightColor.LoadFromDOMNode(node);
		return true;
	}
	if ( XMLString::compareString(nodeName, L"TextInsets") == 0 ) {
		m_textInsets.LoadFromDOMNode(node);
		return true;
	}
	return false;
}

// ----------------------------------------------------------------------------
void CXmlEditBox::Inherit(CXmlLayout* pXmlLayout)
{
	CXmlFrame::Inherit(pXmlLayout);
	CXmlEditBox* pXmlEditBox = dynamic_cast<CXmlEditBox*>(pXmlLayout);
	if (pXmlEditBox)
	{
		if (pXmlEditBox->m_fontString)
		{
			XML_SAFE_DLETE(m_fontString);
			m_fontString = new CXmlFontString(m_pXmlUi, this);
			m_fontString->Inherit(pXmlEditBox->m_fontString);
		}
				
		m_textInsets		= pXmlEditBox->m_textInsets;
		m_highlightColor	= pXmlEditBox->m_highlightColor;
		m_historyLines		= pXmlEditBox->m_historyLines;
		m_letters			= pXmlEditBox->m_letters;
		m_isNumeric			= pXmlEditBox->m_isNumeric;
		m_isPassword		= pXmlEditBox->m_isPassword;
		m_isMultiLine		= pXmlEditBox->m_isMultiLine;
		m_isAutoFocus		= pXmlEditBox->m_isAutoFocus;
	}
}