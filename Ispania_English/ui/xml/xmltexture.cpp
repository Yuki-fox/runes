#include "xmltexture.h"

// ----------------------------------------------------------------------------
CXmlTexCoords::CXmlTexCoords()
{
	m_left = m_top = m_right = m_bottom = 0.0f;
}

// ----------------------------------------------------------------------------
void CXmlTexCoords::LoadNodeAttribute(const DOMNamedNodeMap* nodeMap)
{
	DOMNode* attr;
	if ( (attr = nodeMap->getNamedItem(L"left")) != NULL ) {
		m_left = (float)_wtof(attr->getNodeValue());
	}
	if ( (attr = nodeMap->getNamedItem(L"top")) != NULL ) {
		m_top = (float)_wtof(attr->getNodeValue());
	}
	if ( (attr = nodeMap->getNamedItem(L"right")) != NULL ) {
		m_right = (float)_wtof(attr->getNodeValue());
	}
	if ( (attr = nodeMap->getNamedItem(L"bottom")) != NULL ) {
		m_bottom = (float)_wtof(attr->getNodeValue());
	}
}

// ----------------------------------------------------------------------------
CXmlTexture::CXmlTexture(CXmlUi* pXmlUi, CXmlLayout* pParent)
	: CXmlLayout(pXmlUi, pParent)
{
	m_type				= eXmlUiType_Texture;

	m_file.clear();
	m_texCoords.clear();
	m_alphaMode			= eAlphaMode_Blend;
	m_animationSpeed	= 0.0f;
}

// ----------------------------------------------------------------------------
CXmlTexture::~CXmlTexture()
{
}

// ----------------------------------------------------------------------------
int	CXmlTexture::XmlValueToAlphaMode( const wchar_t* valueStr )
{
	if ( valueStr )
	{
		if ( XMLString::compareString(valueStr, L"DISABLE") == 0 )
			return eAlphaMode_Disable;
		else if ( XMLString::compareString(valueStr, L"BLEND") == 0 )
			return eAlphaMode_Blend;
		else if ( XMLString::compareString(valueStr, L"ADD") == 0 )
			return eAlphaMode_Add;
	}
	return eAlphaMode_Blend;
}

// ----------------------------------------------------------------------------
void CXmlTexture::LoadNodeAttribute(const DOMNamedNodeMap* nodeMap)
{
	CXmlLayout::LoadNodeAttribute(nodeMap);

	DOMNode* attr;
	if ( (attr = nodeMap->getNamedItem(L"file")) != NULL ) {
		char* temp = XMLString::transcode(attr->getNodeValue());
		m_file = temp;
		XMLString::release(&temp);
	}	
	if ( (attr = nodeMap->getNamedItem(L"alphaMode")) != NULL ) {
		m_alphaMode = XmlValueToAlphaMode(attr->getNodeValue());
	}
	if ( (attr = nodeMap->getNamedItem(L"animationSpeed")) != NULL ) {	
		m_animationSpeed = (float)_wtof(attr->getNodeValue());
	}
}

// ----------------------------------------------------------------------------
bool CXmlTexture::LoadNodeElement(const DOMNode* node)
{
	if ( CXmlLayout::LoadNodeElement(node) ) {
		return true;
	}

	const XMLCh* nodeName = node->getNodeName();
	if ( XMLString::compareString(nodeName, L"TexCoords") == 0 ) {
		CXmlTexCoords texCoords;
		if ( texCoords.LoadFromDOMNode(node) ) {
			m_texCoords.push_back(texCoords);
		}		
		return true;
	}
	if ( XMLString::compareString(nodeName, L"Color") == 0 ) {
		m_color.LoadFromDOMNode(node);
	}
	return false;
}

// ----------------------------------------------------------------------------
void CXmlTexture::Inherit(CXmlLayout* pXmlLayout)
{
	CXmlLayout::Inherit(pXmlLayout);
	CXmlTexture* pXmlTexture = dynamic_cast<CXmlTexture*>(pXmlLayout);
	if (pXmlTexture)
	{
		m_file				= pXmlTexture->m_file;
		m_texCoords			= pXmlTexture->m_texCoords;
		m_color				= pXmlTexture->m_color;
		m_alphaMode			= pXmlTexture->m_alphaMode;
		m_animationSpeed	= pXmlTexture->m_animationSpeed;
	}
}