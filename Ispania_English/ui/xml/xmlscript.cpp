#include "xmlscript.h"

// ----------------------------------------------------------------------------
CXmlScript::CXmlScript(const string& pathname)
{
	m_type			= eXmlUiType_Script;
	m_pathname		= pathname;

	m_content.clear();
}

// ----------------------------------------------------------------------------
bool CXmlScript::LoadFromDOMNode(const DOMNode* node)
{
	if ( node == NULL || node->getNodeType() != DOMNode::ELEMENT_NODE )
		return false;

	// Attribute
	if( node->hasAttributes() ) {
		DOMNode* attrNode = node->getAttributes()->getNamedItem(L"file");
		if ( attrNode ) {
			char* temp = XMLString::transcode(attrNode->getNodeValue());
			m_file = m_pathname + temp;
			XMLString::release(&temp);
		}
	}

	// script content
	char* temp = XMLString::transcode(node->getTextContent());
	m_content = temp;
	XMLString::release(&temp);

	return true;
}