#include "xmllayout.h"
#include "xmlui.h"

const char* g_xml_key_parent = "$parent";

// ----------------------------------------------------------------------------
CXmlAnchor::CXmlAnchor()
{
	m_relativeTo.clear();
	m_point				= eAnchor_Null;
	m_relativePoint		= eAnchor_Null;
}

// ----------------------------------------------------------------------------
int	CXmlAnchor::XmlValueToAnchorPoint( const XMLCh* valueStr )
{
	if ( valueStr )
	{
		if ( XMLString::compareString(valueStr, L"TOPLEFT") == 0 )
			return eAnchor_TopLeft;
		else if ( XMLString::compareString(valueStr, L"TOP") == 0 )
			return eAnchor_Top;
		else if ( XMLString::compareString(valueStr, L"TOPRIGHT") == 0 )
			return eAnchor_TopRight;
		else if ( XMLString::compareString(valueStr, L"LEFT") == 0 )
			return eAnchor_Left;
		else if ( XMLString::compareString(valueStr, L"CENTER") == 0 )
			return eAnchor_Center;
		else if ( XMLString::compareString(valueStr, L"RIGHT") == 0 )
			return eAnchor_Right;
		else if ( XMLString::compareString(valueStr, L"BOTTOMLEFT") == 0 )
			return eAnchor_BottomLeft;
		else if ( XMLString::compareString(valueStr, L"BOTTOM") == 0 )
			return eAnchor_Bottom;
		else if ( XMLString::compareString(valueStr, L"BOTTOMRIGHT") == 0 )
			return eAnchor_BottomRight;
	}
	return eAnchor_Null;
}

// ----------------------------------------------------------------------------
void CXmlAnchor::LoadNodeAttribute(const DOMNamedNodeMap* nodeMap)
{
	DOMNode* attr;
	if ( (attr = nodeMap->getNamedItem(L"point")) != NULL ) {
		m_point = XmlValueToAnchorPoint(attr->getNodeValue());
	}
	if ( (attr = nodeMap->getNamedItem(L"relativeTo")) != NULL ) {
		char* temp = XMLString::transcode(attr->getNodeValue());
		m_relativeTo = temp;
		XMLString::release(&temp);
	}
	if ( (attr = nodeMap->getNamedItem(L"relativePoint")) != NULL ) {
		m_relativePoint = XmlValueToAnchorPoint(attr->getNodeValue());
	}
}

// ----------------------------------------------------------------------------
bool CXmlAnchor::LoadNodeElement(const DOMNode* node)
{
	const XMLCh* nodeName = node->getNodeName();
	if ( XMLString::compareString(nodeName, L"Offset") == 0 ) {
		m_offset.LoadFromDOMNode(node);
		return true;
	}
	return false;
}

// ----------------------------------------------------------------------------
CXmlAnchors::CXmlAnchors()
{
	m_vector.clear();
}

// ----------------------------------------------------------------------------
bool CXmlAnchors::LoadNodeElement(const DOMNode* node)
{
	const XMLCh* nodeName = node->getNodeName();
	if ( XMLString::compareString(nodeName, L"Anchor") == 0 ) {
		CXmlAnchor anchor;
		anchor.LoadFromDOMNode(node);
		m_vector.push_back(anchor);
		return true;
	}
	return false;
}

// ----------------------------------------------------------------------------
CXmlLayout::CXmlLayout(CXmlUi* pUi, CXmlLayout* pParent)
{
	m_type				= eXmlUiType_Layout;	

	m_isVisible			= true;
	m_isVirtual			= false;
	m_parent			= pParent;	
	m_pXmlUi			= pUi;
}

// ----------------------------------------------------------------------------
CXmlLayout::~CXmlLayout()
{
}

// ----------------------------------------------------------------------------
void CXmlLayout::LoadNodeAttribute(const DOMNamedNodeMap* nodeMap)
{
	DOMNode* attr;
	if ( (attr = nodeMap->getNamedItem(L"virtual")) != NULL ) {
		m_isVirtual = XmlValueToBoolean(attr->getNodeValue());
	}
	if ( (attr = nodeMap->getNamedItem(L"name")) != NULL ) {
		char* temp = XMLString::transcode(attr->getNodeValue());
		m_name = temp;
		XMLString::release(&temp);
	}
	if ( (attr = nodeMap->getNamedItem(L"parent")) != NULL && m_pXmlUi ) {
		char* temp = XMLString::transcode(attr->getNodeValue());
		m_parent = m_pXmlUi->FindByName(temp);
		XMLString::release(&temp);
	}
	if ( m_parent ) {
		m_isVirtual = m_parent->m_isVirtual;
	}

	if ( (attr = nodeMap->getNamedItem(L"inherits")) != NULL && m_pXmlUi ) {
		char* inherit = XMLString::transcode(attr->getNodeValue());
		CXmlLayout* pXmlLayout = m_pXmlUi->FindByName(inherit);
		if ( pXmlLayout == NULL ) {
			// �~�Ӫ��󤣦s�b
			m_pXmlUi->PushErrorMsg("�~�Ӫ��󤣦s�b[%s]", inherit);
		} else if ( !pXmlLayout->m_isVirtual ) {
			// �����~�ӫD��������
			m_pXmlUi->PushErrorMsg("�����~�ӫD��������[%s]", inherit);
		} else {
			// �~��
			Inherit(pXmlLayout);
		}
		XMLString::release(&inherit);
	}
	if ( (attr = nodeMap->getNamedItem(L"hidden")) != NULL ) {
		m_isVisible = !XmlValueToBoolean(attr->getNodeValue());
	}

	// �D��������,�إ߯u��W��
	if ( !m_isVirtual )
		ChangeName(m_name);

	// �[�J���ު�
	if ( m_pXmlUi && !m_name.empty() )
		m_pXmlUi->AddName(m_name.c_str(), this);
}

// ----------------------------------------------------------------------------
bool CXmlLayout::LoadNodeElement(const DOMNode* node)
{
	const XMLCh* nodeName = node->getNodeName();

	// Size
	if ( XMLString::compareString(nodeName, L"Size") == 0 ) {
		m_size.LoadFromDOMNode(node);
		return true;
	}
	// Anchor
	if ( XMLString::compareString(nodeName, L"Anchors") == 0 ) {
		m_anchors.LoadFromDOMNode(node);

		// �Y���͹���h�ݱN�W�ٰ��m��
		if (!m_isVirtual)
		{
			vector<CXmlAnchor>& anchors = m_anchors.m_vector;
			for (vector<CXmlAnchor>::iterator iter = anchors.begin(); iter != anchors.end(); iter++)
				ChangeName((*iter).m_relativeTo);
		}
		return true;
	}

	return false;
}

// ----------------------------------------------------------------------------
void CXmlLayout::Inherit(CXmlLayout* pXmlLayout)
{
	if (pXmlLayout)
	{		
		m_anchors	= pXmlLayout->m_anchors;
		m_size		= pXmlLayout->m_size;
		m_isVisible = pXmlLayout->m_isVisible;
		
		if (m_parent)
			m_isVirtual = m_parent->m_isVirtual;

		// �Y�O�����W�٫h�N�W���~��
		if ( strncmp(pXmlLayout->m_name.c_str(), g_xml_key_parent, strlen(g_xml_key_parent)) == 0 )
			m_name = pXmlLayout->m_name;

		if (!m_isVirtual)
		{
			ChangeName(m_name);

			vector<CXmlAnchor>& anchors = m_anchors.m_vector;
			for (vector<CXmlAnchor>::iterator iter = anchors.begin(); iter != anchors.end(); iter++)
				ChangeName((*iter).m_relativeTo);
		}
	}
}

// ----------------------------------------------------------------------------
void CXmlLayout::ChangeName(string &str)
{
	if ( strncmp(str.c_str(), g_xml_key_parent, strlen(g_xml_key_parent)) == 0 )
	{
		const char* tmp = str.c_str() + strlen(g_xml_key_parent);
		if (m_parent)
			str = m_parent->m_name + tmp;
		else
			str = tmp;
	}
}