#include "xmlslider.h"

// ----------------------------------------------------------------------------
CXmlSlider::CXmlSlider(CXmlUi* pXmlUi, CXmlLayout* pParent)
	: CXmlFrame(pXmlUi, pParent)
{
	m_type				= eXmlUiType_Slider;

	m_thumbTexture		= NULL;
	m_orientation		= eOrientation_Vertical;
	m_minValue			= 0.0f;
	m_maxValue			= 0.0f;
	m_defaultValue		= 0.0f;
	m_smallStep			= 0.0f;
	m_largeStep			= 0.0f;
}

// ----------------------------------------------------------------------------
CXmlSlider::~CXmlSlider()
{
	XML_SAFE_DLETE(m_thumbTexture);
}

// ----------------------------------------------------------------------------
void CXmlSlider::LoadNodeAttribute(const DOMNamedNodeMap* nodeMap)
{
	CXmlFrame::LoadNodeAttribute(nodeMap);

	DOMNode* attr;
	if ( (attr = nodeMap->getNamedItem(L"orientation")) != NULL ) {
		const XMLCh* temp = attr->getNodeValue();
		if ( XMLString::compareString(temp, L"HORIZONTAL") == 0 )
			m_orientation = eOrientation_Horizontal;
		else if ( XMLString::compareString(temp, L"VERTICAL") == 0 )
			m_orientation = eOrientation_Vertical;
	}
	if ( (attr = nodeMap->getNamedItem(L"minValue")) != NULL ) {
		m_minValue = (float)_wtof(attr->getNodeValue());
	}
	if ( (attr = nodeMap->getNamedItem(L"maxValue")) != NULL ) {
		m_maxValue = (float)_wtof(attr->getNodeValue());
	}
	if ( (attr = nodeMap->getNamedItem(L"defaultValue")) != NULL ) {
		m_defaultValue = (float)_wtof(attr->getNodeValue());
	}
	if ( (attr = nodeMap->getNamedItem(L"smallStep")) != NULL ) {
		m_smallStep = (float)_wtof(attr->getNodeValue());
	}
	if ( (attr = nodeMap->getNamedItem(L"largeStep")) != NULL ) {
		m_largeStep = (float)_wtof(attr->getNodeValue());
	}
}

// ----------------------------------------------------------------------------
bool CXmlSlider::LoadNodeElement(const DOMNode* node)
{
	if ( CXmlFrame::LoadNodeElement(node) ) {
		return true;
	}

	const XMLCh* nodeName = node->getNodeName();
	if ( XMLString::compareString(nodeName, L"ThumbTexture") == 0 ) {
		XML_SAFE_DLETE(m_thumbTexture);
		m_thumbTexture = new CXmlTexture(m_pXmlUi, this);
		m_thumbTexture->LoadFromDOMNode(node);
		return true;
	}
	return false;
}

// ----------------------------------------------------------------------------
void CXmlSlider::Inherit(CXmlLayout* pXmlLayout)
{
	CXmlFrame::Inherit(pXmlLayout);
	CXmlSlider* pXmlSlider = dynamic_cast<CXmlSlider*>(pXmlLayout);
	if (pXmlSlider)
	{
		if (pXmlSlider->m_thumbTexture)
		{
			XML_SAFE_DLETE(m_thumbTexture);
			m_thumbTexture = new CXmlTexture(m_pXmlUi, this);
			m_thumbTexture->Inherit(pXmlSlider->m_thumbTexture);
		}

		m_orientation		= pXmlSlider->m_orientation;		
		m_minValue			= pXmlSlider->m_minValue;
		m_maxValue			= pXmlSlider->m_maxValue;
		m_defaultValue		= pXmlSlider->m_defaultValue;
		m_smallStep			= pXmlSlider->m_smallStep;
		m_largeStep			= pXmlSlider->m_largeStep;
	}
}