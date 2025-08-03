#include "xmlmessageframe.h"

// ----------------------------------------------------------------------------
CXmlMessageFrame::CXmlMessageFrame(CXmlUi* pXmlUi, CXmlLayout* pParent)
	: CXmlFrame(pXmlUi, pParent)
{
	m_type				= eXmlUiType_MessageFrame;

	m_fontString		= NULL;
	m_isFade			= true;
	m_fadeDuration		= 3.0f;
	m_displayDuration	= 10.0f;
	m_maxLines			= 10;
}

// ----------------------------------------------------------------------------
CXmlMessageFrame::~CXmlMessageFrame()
{
	XML_SAFE_DLETE(m_fontString);
}

// ----------------------------------------------------------------------------
void CXmlMessageFrame::LoadNodeAttribute(const DOMNamedNodeMap* nodeMap)
{
	CXmlFrame::LoadNodeAttribute(nodeMap);
	DOMNode* attr;
	if ( (attr = nodeMap->getNamedItem(L"fade")) != NULL ) {
		m_isFade = XmlValueToBoolean(attr->getNodeValue());
	}
	if ( (attr = nodeMap->getNamedItem(L"fadeDuration")) != NULL ) {
		m_fadeDuration = (float)_wtof(attr->getNodeValue());
	}
	if ( (attr = nodeMap->getNamedItem(L"displayDuration")) != NULL ) {
		m_displayDuration = (float)_wtof(attr->getNodeValue());
	}
	if ( (attr = nodeMap->getNamedItem(L"maxLines")) != NULL ) {
		m_maxLines = _wtoi(attr->getNodeValue());
	}
}

// ----------------------------------------------------------------------------
bool CXmlMessageFrame::LoadNodeElement(const DOMNode* node)
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
	if ( XMLString::compareString(nodeName, L"TextInsets") == 0 ) {
		m_textInsets.LoadFromDOMNode(node);
		return true;
	}

	return false;
}

// ----------------------------------------------------------------------------
void CXmlMessageFrame::Inherit(CXmlLayout* pXmlLayout)
{
	CXmlFrame::Inherit(pXmlLayout);
	CXmlMessageFrame* pXmlMessageFrame = dynamic_cast<CXmlMessageFrame*>(pXmlLayout);
	if (pXmlMessageFrame)
	{
		if (pXmlMessageFrame->m_fontString)
		{
			XML_SAFE_DLETE(m_fontString);
			m_fontString = new CXmlFontString(m_pXmlUi, this);
			m_fontString->Inherit(pXmlMessageFrame->m_fontString);
		}

		m_textInsets		= pXmlMessageFrame->m_textInsets;
		m_isFade			= pXmlMessageFrame->m_isFade;
		m_fadeDuration		= pXmlMessageFrame->m_fadeDuration;
		m_displayDuration	= pXmlMessageFrame->m_displayDuration;
		m_maxLines			= pXmlMessageFrame->m_maxLines;
	}
}