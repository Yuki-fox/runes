#include "xmlbase.h"

// ----------------------------------------------------------------------------
bool IXmlBase::LoadFromDOMNode(const DOMNode* node)
{
	if ( node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE )
		return false;

	// Attribute
	if( node->hasAttributes() ) {
		LoadNodeAttribute(node->getAttributes());
	}

	// Element
	for ( DOMNode *child = node->getFirstChild(); child != 0; child = child->getNextSibling() ) {
		if ( child->getNodeType() == DOMNode::ELEMENT_NODE ) {
			LoadNodeElement(child);
		}
	}
	return true;
}

// ----------------------------------------------------------------------------
const XMLCh* IXmlBase::GetNodeAttribute( const DOMNamedNodeMap* nodeMap, const XMLCh* name )
{
	DOMNode* attributeNode;
	if ( nodeMap == NULL || name == NULL )
		return NULL;

	attributeNode = nodeMap->getNamedItem(name);
	if ( attributeNode )
		return attributeNode->getNodeValue();
	return NULL;
}

// ----------------------------------------------------------------------------
bool IXmlBase::XmlValueToBoolean( const XMLCh* valueStr )
{
	if ( valueStr && XMLString::compareString(valueStr, L"true") == 0 )
		return true;
	return false;
}

// ----------------------------------------------------------------------------
CXmlNode::CXmlNode()
{
	m_type = eXmlUiType_Node;
}

// ----------------------------------------------------------------------------
CXmlValue::CXmlValue()
{
	m_val = 0.0f;
}

// ----------------------------------------------------------------------------
bool CXmlValue::LoadNodeElement(const DOMNode* node)
{
	if ( XMLString::compareString(node->getNodeName(), L"AbsValue") == 0 ) {
		if( node->hasAttributes() ) {
			int nSize = (int)node->getAttributes()->getLength();
			for ( int i = 0; i < nSize; ++i ) {
				DOMAttr* pAttributeNode = (DOMAttr*) node->getAttributes()->item(i);
				if ( XMLString::compareString(pAttributeNode->getName(), L"val") == 0 ) {
					m_val = (float)_wtoi(pAttributeNode->getValue());
					break;
				}
			}
		}
		return true;
	}
	if ( XMLString::compareString(node->getNodeName(), L"RelValue") == 0 ) {
		if( node->hasAttributes() ) {
			int nSize = (int)node->getAttributes()->getLength();
			for ( int i = 0; i < nSize; ++i ) {
				DOMAttr* pAttributeNode = (DOMAttr*) node->getAttributes()->item(i);
				if ( XMLString::compareString(pAttributeNode->getName(), L"val") == 0 ) {
					m_val = (float)_wtof(pAttributeNode->getValue());
					break;
				}
			}
		}
		return true;
	}
	return false;
}

// ----------------------------------------------------------------------------
CXmlDimension::CXmlDimension()
{
	m_x = 0.0f;
	m_y = 0.0f;
}

// ----------------------------------------------------------------------------
bool CXmlDimension::LoadNodeElement(const DOMNode* node)
{
	const XMLCh* nodeName = node->getNodeName();
	if ( XMLString::compareString(nodeName, L"AbsDimension") == 0 ) {
		if( node->hasAttributes() ) {
			for ( int i = 0; i < node->getAttributes()->getLength(); ++i ) {
				DOMAttr* pAttributeNode = (DOMAttr*) node->getAttributes()->item(i);
				if ( XMLString::compareString(pAttributeNode->getName(), L"x") == 0 ) {
					m_x = (float)_wtoi(pAttributeNode->getValue());
				}
				if ( XMLString::compareString(pAttributeNode->getName(), L"y") == 0 ) {
					m_y = (float)_wtoi(pAttributeNode->getValue());
				}
			}
		}
		return true;
	}
	if ( XMLString::compareString(nodeName, L"RelDimension") == 0 ) {
		if( node->hasAttributes() ) {
			for ( int i = 0; i < node->getAttributes()->getLength(); ++i ) {
				DOMAttr* pAttributeNode = (DOMAttr*) node->getAttributes()->item(i);
				if ( XMLString::compareString(pAttributeNode->getName(), L"x") == 0 ) {
					m_x = (float)_wtof(pAttributeNode->getValue());
				}
				if ( XMLString::compareString(pAttributeNode->getName(), L"y") == 0 ) {
					m_y = (float)_wtof(pAttributeNode->getValue());
				}
			}
		}
		return true;
	}
	return false;
}

// ----------------------------------------------------------------------------
CXmlInset::CXmlInset()
{
	m_left = 0.0f;
	m_top = 0.0f;
	m_right = 0.0f;
	m_bottom = 0.0f;
}

// ----------------------------------------------------------------------------
bool CXmlInset::LoadNodeElement(const DOMNode* node)
{
	const XMLCh* nodeName = node->getNodeName();
	if ( XMLString::compareString(nodeName, L"AbsInset") == 0 ) {
		if( node->hasAttributes() ) {
			int nSize = (int)node->getAttributes()->getLength();
			for ( int i = 0; i < nSize; ++i ) {
				DOMAttr* pAttributeNode = (DOMAttr*) node->getAttributes()->item(i);
				if ( XMLString::compareString(pAttributeNode->getName(), L"left") == 0 ) {
					m_left = (float)_wtoi(pAttributeNode->getValue());
				}
				if ( XMLString::compareString(pAttributeNode->getName(), L"top") == 0 ) {
					m_top = (float)_wtoi(pAttributeNode->getValue());
				}
				if ( XMLString::compareString(pAttributeNode->getName(), L"right") == 0 ) {
					m_right = (float)_wtoi(pAttributeNode->getValue());
				}
				if ( XMLString::compareString(pAttributeNode->getName(), L"bottom") == 0 ) {
					m_bottom = (float)_wtoi(pAttributeNode->getValue());
				}
			}
		}
		return true;
	}
	if ( XMLString::compareString(nodeName, L"RelInset") == 0 ) {
		if( node->hasAttributes() ) {
			int nSize = (int)node->getAttributes()->getLength();
			for ( int i = 0; i < nSize; ++i ) {
				DOMAttr* pAttributeNode = (DOMAttr*) node->getAttributes()->item(i);
				if ( XMLString::compareString(pAttributeNode->getName(), L"left") == 0 ) {
					m_left = (float)_wtof(pAttributeNode->getValue());
				}
				if ( XMLString::compareString(pAttributeNode->getName(), L"top") == 0 ) {
					m_top = (float)_wtof(pAttributeNode->getValue());
				}
				if ( XMLString::compareString(pAttributeNode->getName(), L"right") == 0 ) {
					m_right = (float)_wtof(pAttributeNode->getValue());
				}
				if ( XMLString::compareString(pAttributeNode->getName(), L"bottom") == 0 ) {
					m_bottom = (float)_wtof(pAttributeNode->getValue());
				}
			}
		}
		return true;
	}
	return false;
}

// ----------------------------------------------------------------------------
CXmlColor::CXmlColor()
{
	m_a = 1.0f;
	m_r = 1.0f;
	m_g = 1.0f;
	m_b = 1.0f;
}

// ----------------------------------------------------------------------------
void CXmlColor::LoadNodeAttribute(const DOMNamedNodeMap* nodeMap)
{
	DOMNode* attr;
	if ( (attr = nodeMap->getNamedItem(L"r")) != NULL ) {
		m_r = (float)_wtof(attr->getNodeValue());
	}
	if ( (attr = nodeMap->getNamedItem(L"g")) != NULL ) {
		m_g = (float)_wtof(attr->getNodeValue());
	}
	if ( (attr = nodeMap->getNamedItem(L"b")) != NULL ) {
		m_b = (float)_wtof(attr->getNodeValue());
	}
	if ( (attr = nodeMap->getNamedItem(L"a")) != NULL ) {
		m_a = (float)_wtof(attr->getNodeValue());
	}
}

// ----------------------------------------------------------------------------
char* RemoveSpaceWord(const char* instr, char* outstr, int outsize)
{
	if ( instr == NULL || outsize == NULL )
		return NULL;

	// 去除前後空白	
	int st = 0;	
	while ( instr[st] && instr[st] < ' ' ) {
		st++;
	}
	int ed = (int)strlen(instr + st);
	while ( ed >= st ) {
		if ( instr[ed] < ' ' )
			ed--;
		else {
			int size = ed + st + 1;
			if ( size > outsize - 1 )
				size = outsize - 1;
			memcpy(outstr, instr + st, sizeof(char) * size);
			outstr[size] = 0;
			break;
		}
	}
	return outstr;
}