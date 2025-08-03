#include "xmlstatusbar.h"

// ----------------------------------------------------------------------------
CXmlStatusBar::CXmlStatusBar(CXmlUi* pXmlUi, CXmlLayout* pParent)
	: CXmlFrame(pXmlUi, pParent)
{
	m_type				= eXmlUiType_StatusBar;

	m_barTexture		= NULL;
	m_minValue			= 0;
	m_maxValue			= 0;
	m_defaultValue		= 0;
	m_reverse			= false;
}

// ----------------------------------------------------------------------------
CXmlStatusBar::~CXmlStatusBar()
{
	XML_SAFE_DLETE(m_barTexture);
}

// ----------------------------------------------------------------------------
void CXmlStatusBar::LoadNodeAttribute(const DOMNamedNodeMap* nodeMap)
{
	CXmlFrame::LoadNodeAttribute(nodeMap);

	DOMNode* attr;
	if ( (attr = nodeMap->getNamedItem(L"minValue")) != NULL ) {
		m_minValue = (float)_wtof(attr->getNodeValue());
	}
	if ( (attr = nodeMap->getNamedItem(L"maxValue")) != NULL ) {
		m_maxValue = (float)_wtof(attr->getNodeValue());
	}
	if ( (attr = nodeMap->getNamedItem(L"defaultValue")) != NULL ) {
		m_defaultValue = (float)_wtof(attr->getNodeValue());
	}
	if ( (attr = nodeMap->getNamedItem(L"reverse")) != NULL ) {
		m_reverse = XmlValueToBoolean(attr->getNodeValue());
	}
}

// ----------------------------------------------------------------------------
bool CXmlStatusBar::LoadNodeElement(const DOMNode* node)
{
	if ( CXmlFrame::LoadNodeElement(node) ) {
		return true;
	}

	const XMLCh* nodeName = node->getNodeName();
	if ( XMLString::compareString(nodeName, L"BarTexture") == 0 ) {
		XML_SAFE_DLETE(m_barTexture);
		m_barTexture = new CXmlTexture(m_pXmlUi, this);
		m_barTexture->LoadFromDOMNode(node);
		return true;
	}
	if ( XMLString::compareString(nodeName, L"BarColor") == 0 ) {
		m_barColor.LoadFromDOMNode(node);
		return true;
	}
	return false;
}

// ----------------------------------------------------------------------------
void CXmlStatusBar::Inherit(CXmlLayout* pXmlLayout)
{
	CXmlFrame::Inherit(pXmlLayout);
	CXmlStatusBar* pXmlStatusBar = dynamic_cast<CXmlStatusBar*>(pXmlLayout);
	if (pXmlStatusBar)
	{
		if (pXmlStatusBar->m_barTexture)
		{
			XML_SAFE_DLETE(m_barTexture);
			m_barTexture = new CXmlTexture(m_pXmlUi, this);
			m_barTexture->Inherit(pXmlStatusBar->m_barTexture);
		}

		m_barColor		= pXmlStatusBar->m_barColor;
		m_minValue		= pXmlStatusBar->m_minValue;
		m_maxValue		= pXmlStatusBar->m_maxValue;
		m_defaultValue	= pXmlStatusBar->m_defaultValue;
		m_reverse		= pXmlStatusBar->m_reverse;
	}
}