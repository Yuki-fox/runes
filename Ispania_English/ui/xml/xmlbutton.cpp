#include "xmlbutton.h"

// ----------------------------------------------------------------------------
CXmlButton::CXmlButton(CXmlUi* pXmlUi, CXmlLayout* pParent)
	: CXmlFrame(pXmlUi, pParent)
{
	m_type				= eXmlUiType_Button;

	m_normalTexture		= NULL;
	m_pushedTexture		= NULL;
	m_disabledTexture	= NULL;
	m_highlightTexture	= NULL;
	m_normalText		= NULL;
	m_disabledText		= NULL;
	m_highlightText		= NULL;

	m_text.clear();
}

// ----------------------------------------------------------------------------
CXmlButton::~CXmlButton()
{
	XML_SAFE_DLETE(m_normalTexture);
	XML_SAFE_DLETE(m_pushedTexture);
	XML_SAFE_DLETE(m_disabledTexture);
	XML_SAFE_DLETE(m_highlightTexture);
	XML_SAFE_DLETE(m_normalText);
	XML_SAFE_DLETE(m_disabledText);
	XML_SAFE_DLETE(m_highlightText);
}

// ----------------------------------------------------------------------------
void CXmlButton::LoadNodeAttribute(const DOMNamedNodeMap* nodeMap)
{
	CXmlFrame::LoadNodeAttribute(nodeMap);

	DOMNode* attr;
	if ( (attr = nodeMap->getNamedItem(L"text")) != NULL ) {
		char* temp = XMLString::transcode(attr->getNodeValue());
		m_text = temp;
		XMLString::release(&temp);
	}
}

// ----------------------------------------------------------------------------
bool CXmlButton::LoadNodeElement(const DOMNode* node)
{
	if ( CXmlFrame::LoadNodeElement(node) ) {
		return true;
	}

	const XMLCh* nodeName = node->getNodeName();
	if ( XMLString::compareString(nodeName, L"NormalTexture") == 0 ) {
		XML_SAFE_DLETE(m_normalTexture);
		m_normalTexture = new CXmlTexture(m_pXmlUi, this);
		m_normalTexture->LoadFromDOMNode(node);
		return true;
	}
	if ( XMLString::compareString(nodeName, L"PushedTexture") == 0 ) {
		XML_SAFE_DLETE(m_pushedTexture);
		m_pushedTexture = new CXmlTexture(m_pXmlUi, this);
		m_pushedTexture->LoadFromDOMNode(node);
		return true;
	}
	if ( XMLString::compareString(nodeName, L"DisabledTexture") == 0 ) {
		XML_SAFE_DLETE(m_disabledTexture);
		m_disabledTexture = new CXmlTexture(m_pXmlUi, this);
		m_disabledTexture->LoadFromDOMNode(node);
		return true;
	}
	if ( XMLString::compareString(nodeName, L"HighlightTexture") == 0 ) {
		XML_SAFE_DLETE(m_highlightTexture);
		m_highlightTexture = new CXmlTexture(m_pXmlUi, this);
		m_highlightTexture->LoadFromDOMNode(node);
		return true;
	}
	if ( XMLString::compareString(nodeName, L"NormalText") == 0 ) {
		XML_SAFE_DLETE(m_normalText);
		m_normalText = new CXmlFontString(m_pXmlUi, this);		
		m_normalText->LoadFromDOMNode(node);
		DefaultFontString(m_normalText);
		return true;
	}
	if ( XMLString::compareString(nodeName, L"HighlightText") == 0 ) {
		XML_SAFE_DLETE(m_highlightText);
		m_highlightText = new CXmlFontString(m_pXmlUi, this);
		m_highlightText->LoadFromDOMNode(node);
		DefaultFontString(m_highlightText);
		return true;
	}
	if ( XMLString::compareString(nodeName, L"DisabledText") == 0 ) {
		XML_SAFE_DLETE(m_disabledText);
		m_disabledText = new CXmlFontString(m_pXmlUi, this);
		m_disabledText->LoadFromDOMNode(node);
		DefaultFontString(m_disabledText);
		return true;
	}
	if ( XMLString::compareString(nodeName, L"PushedTextOffset") == 0 ) {
		m_pushedTextOffset.LoadFromDOMNode(node);
		return true;
	}
	return false;
}

// ----------------------------------------------------------------------------
void CXmlButton::DefaultFontString(CXmlFontString* fontString)
{
	if ( fontString->m_anchors.m_vector.empty() )
	{
		CXmlAnchor anchor;
		anchor.m_point = CXmlAnchor::eAnchor_Center;
		fontString->m_anchors.m_vector.push_back(anchor);
	}	
}

// ----------------------------------------------------------------------------
void CXmlButton::Inherit(CXmlLayout* pXmlLayout)
{
	CXmlFrame::Inherit(pXmlLayout);
	CXmlButton* pXmlButton = dynamic_cast<CXmlButton*>(pXmlLayout);
	if (pXmlButton)
	{
		m_text				= pXmlButton->m_text;
		m_pushedTextOffset	= pXmlButton->m_pushedTextOffset;

		if (pXmlButton->m_normalTexture)
		{
			XML_SAFE_DLETE(m_normalTexture);
			m_normalTexture = new CXmlTexture(m_pXmlUi, this);
			m_normalTexture->Inherit(pXmlButton->m_normalTexture);
		}
		if (pXmlButton->m_pushedTexture)
		{
			XML_SAFE_DLETE(m_pushedTexture);
			m_pushedTexture = new CXmlTexture(m_pXmlUi, this);
			m_pushedTexture->Inherit(pXmlButton->m_pushedTexture);
		}
		if (pXmlButton->m_disabledTexture)
		{
			XML_SAFE_DLETE(m_disabledTexture);
			m_disabledTexture = new CXmlTexture(m_pXmlUi, this);
			m_disabledTexture->Inherit(pXmlButton->m_disabledTexture);
		}
		if (pXmlButton->m_highlightTexture)
		{
			XML_SAFE_DLETE(m_highlightTexture);
			m_highlightTexture = new CXmlTexture(m_pXmlUi, this);
			m_highlightTexture->Inherit(pXmlButton->m_highlightTexture);
		}
		if (pXmlButton->m_normalText)
		{
			XML_SAFE_DLETE(m_normalText);
			m_normalText = new CXmlFontString(m_pXmlUi, this);
			m_normalText->Inherit(pXmlButton->m_normalText);
		}
		if (pXmlButton->m_disabledText)
		{
			XML_SAFE_DLETE(m_disabledText);
			m_disabledText = new CXmlFontString(m_pXmlUi, this);
			m_disabledText->Inherit(pXmlButton->m_disabledText);
		}
		if (pXmlButton->m_highlightText)
		{
			XML_SAFE_DLETE(m_highlightText);
			m_highlightText = new CXmlFontString(m_pXmlUi, this);
			m_highlightText->Inherit(pXmlButton->m_highlightText);
		}
	}
}