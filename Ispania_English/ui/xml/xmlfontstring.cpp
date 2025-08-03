#include "xmlfontstring.h"

// ----------------------------------------------------------------------------
CXmlShadow::CXmlShadow()
{
}

// ----------------------------------------------------------------------------
bool CXmlShadow::LoadNodeElement(const DOMNode* node)
{
	const XMLCh* nodeName = node->getNodeName();
	if ( XMLString::compareString(nodeName, L"Color") == 0 ) {
		m_color.LoadFromDOMNode(node);
		return true;
	}
	if ( XMLString::compareString(nodeName, L"Offset") == 0 ) {
		m_offset.LoadFromDOMNode(node);
		return true;
	}
	return false;
}

// ----------------------------------------------------------------------------
CXmlFontString::CXmlFontString(CXmlUi* pXmlUi, CXmlLayout* pParent)
	: CXmlLayout(pXmlUi, pParent)
{
	m_type				= eXmlUiType_FontString;

	m_font.clear();
	m_text.clear();
	m_pShadow			= NULL;
	m_outline			= eOutline_Normal;
	m_weight			= eFontWeight_Normal;
	m_bytes				= 255;
	m_spacing			= 0.0f;
	m_justifyV			= eJustifyVType_Middle;
	m_justifyH			= eJustifyHType_Center;
}

// ----------------------------------------------------------------------------
CXmlFontString::~CXmlFontString()
{
	XML_SAFE_DLETE(m_pShadow);
}

// ----------------------------------------------------------------------------
int CXmlFontString::XmlValueToJustifyVType( const wchar_t* valueStr )
{
	if ( valueStr )
	{
		if ( XMLString::compareString(valueStr, L"TOP") == 0 )
			return eJustifyVType_Top;
		else if ( XMLString::compareString(valueStr, L"MIDDLE") == 0 )
			return eJustifyVType_Middle;
		else if ( XMLString::compareString(valueStr, L"BOTTOM") == 0 )
			return eJustifyVType_Bottom;
	}
	return eJustifyVType_Top;
}

// ----------------------------------------------------------------------------
int CXmlFontString::XmlValueToJustifyHType( const wchar_t* valueStr )
{
	if ( valueStr )
	{
		if ( XMLString::compareString(valueStr, L"LEFT") == 0 )
			return eJustifyHType_Left;
		else if ( XMLString::compareString(valueStr, L"CENTER") == 0 )
			return eJustifyHType_Center;
		else if ( XMLString::compareString(valueStr, L"RIGHT") == 0 )
			return eJustifyHType_Right;
	}
	return eJustifyVType_Top;
}

// ----------------------------------------------------------------------------
int	CXmlFontString::XmlValueToFontWeight( const wchar_t* valueStr )
{
	if ( valueStr )
	{
		if ( XMLString::compareString(valueStr, L"THIN") == 0 )
			return eFontWeight_Thin;
		else if ( XMLString::compareString(valueStr, L"NORMAL") == 0 )
			return eFontWeight_Normal;
		else if ( XMLString::compareString(valueStr, L"BOLD") == 0 )
			return eFontWeight_Bold;
	}
	return eFontWeight_Normal;
}

// ----------------------------------------------------------------------------
int	CXmlFontString::XmlValueToOutline( const wchar_t* valueStr )
{
	if ( valueStr )
	{
		if ( XMLString::compareString(valueStr, L"NONE") == 0 )
			return eOutline_None;
		else if ( XMLString::compareString(valueStr, L"NORMAL") == 0 )
			return eOutline_Normal;
		else if ( XMLString::compareString(valueStr, L"THICK") == 0 )
			return eOutline_Thick;
	}
	return eOutline_Normal;
}

// ----------------------------------------------------------------------------
void CXmlFontString::LoadNodeAttribute(const DOMNamedNodeMap* nodeMap)
{
	CXmlLayout::LoadNodeAttribute(nodeMap);

	DOMNode* attr;
	if ( (attr = nodeMap->getNamedItem(L"font")) != NULL ) {
		char* temp = XMLString::transcode(attr->getNodeValue());
		m_font = temp;
		XMLString::release(&temp);
	}
	if ( (attr = nodeMap->getNamedItem(L"text")) != NULL ) {
		char* temp = XMLString::transcode(attr->getNodeValue());
		m_text = temp;
		XMLString::release(&temp);
	}
	if ( (attr = nodeMap->getNamedItem(L"bytes")) != NULL ) {
		m_bytes = _wtoi(attr->getNodeValue());
	}
	if ( (attr = nodeMap->getNamedItem(L"spacing")) != NULL ) {
		m_spacing = (float)_wtof(attr->getNodeValue());
	}
	if ( (attr = nodeMap->getNamedItem(L"outline")) != NULL ) {
		m_outline = XmlValueToOutline(attr->getNodeValue());
	}
	if ( (attr = nodeMap->getNamedItem(L"weight")) != NULL ) {
		m_weight = XmlValueToFontWeight(attr->getNodeValue());
	}
	if ( (attr = nodeMap->getNamedItem(L"justifyV")) != NULL ) {
		m_justifyV = XmlValueToJustifyVType(attr->getNodeValue());
	}
	if ( (attr = nodeMap->getNamedItem(L"justifyH")) != NULL ) {
		m_justifyH = XmlValueToJustifyHType(attr->getNodeValue());
	}
}

// ----------------------------------------------------------------------------
bool CXmlFontString::LoadNodeElement(const DOMNode* node)
{
	if ( CXmlLayout::LoadNodeElement(node) ) {
		return true;
	}

	const XMLCh* nodeName = node->getNodeName();
	if ( XMLString::compareString(nodeName, L"Color") == 0 ) {
		m_color.LoadFromDOMNode(node);
		return true;
	}
	if ( XMLString::compareString(nodeName, L"FontHeight") == 0 ) {
		m_height.LoadFromDOMNode(node);
		return true;
	}
	if ( XMLString::compareString(nodeName, L"Shadow") == 0 ) {
		XML_SAFE_DLETE( m_pShadow );
		m_pShadow = new CXmlShadow;
		m_pShadow->LoadFromDOMNode(node);
		return true;
	}
	return false;
}

// ----------------------------------------------------------------------------
void CXmlFontString::Inherit(CXmlLayout* pXmlLayout)
{
	CXmlLayout::Inherit(pXmlLayout);
	CXmlFontString* pXmlFontString = dynamic_cast<CXmlFontString*>(pXmlLayout);

	if (pXmlFontString)
	{
		if (pXmlFontString->m_pShadow)
		{
			XML_SAFE_DLETE( m_pShadow );
			m_pShadow = new CXmlShadow;
			*m_pShadow = *pXmlFontString->m_pShadow;
		}
		
		m_color			= pXmlFontString->m_color;
		m_height		= pXmlFontString->m_height;
		m_font			= pXmlFontString->m_font;
		m_text			= pXmlFontString->m_text;
		m_outline		= pXmlFontString->m_outline;
		m_weight		= pXmlFontString->m_weight;	
		m_bytes			= pXmlFontString->m_bytes;
		m_spacing		= pXmlFontString->m_spacing;
		m_justifyV		= pXmlFontString->m_justifyV;
		m_justifyH		= pXmlFontString->m_justifyH;
	}
}