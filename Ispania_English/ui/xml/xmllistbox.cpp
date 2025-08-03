#include "xmllistbox.h"

// ----------------------------------------------------------------------------
CXmlListBox::CXmlListBox(CXmlUi* pXmlUi, CXmlLayout* pParent)
	: CXmlFrame(pXmlUi, pParent)
{
	m_type				= eXmlUiType_ListBox;

	m_fontString		= NULL;
	m_fadeDuration		= 3.0f;
	m_insertMode		= eInsertMode_Bottom;
}

// ----------------------------------------------------------------------------
CXmlListBox::~CXmlListBox()
{
	XML_SAFE_DLETE(m_fontString);
}

// ----------------------------------------------------------------------------
int CXmlListBox::XmlValueToInsertMode(const wchar_t* valueStr)
{
	if ( valueStr )
	{
		if ( XMLString::compareString(valueStr, L"TOP") == 0 )
			return eInsertMode_Top;
		else if ( XMLString::compareString(valueStr, L"BOTTOM") == 0 )
			return eInsertMode_Bottom;
	}
	return eInsertMode_Bottom;
}

// ----------------------------------------------------------------------------
void CXmlListBox::LoadNodeAttribute(const DOMNamedNodeMap* nodeMap)
{
	CXmlFrame::LoadNodeAttribute(nodeMap);

	DOMNode* attr;
	if ( (attr = nodeMap->getNamedItem(L"fadeDuration")) != NULL ) {
		m_fadeDuration = (float)_wtof(attr->getNodeValue());
	}
	if ( (attr = nodeMap->getNamedItem(L"insertMode")) != NULL ) {
		m_insertMode = XmlValueToInsertMode(attr->getNodeValue());
	}
}

// ----------------------------------------------------------------------------
bool CXmlListBox::LoadNodeElement(const DOMNode* node)
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
void CXmlListBox::Inherit(CXmlLayout* pXmlLayout)
{
	CXmlFrame::Inherit(pXmlLayout);
	CXmlListBox* pXmlListBox = dynamic_cast<CXmlListBox*>(pXmlLayout);
	if (pXmlListBox)
	{
		if (pXmlListBox->m_fontString)
		{
			XML_SAFE_DLETE(m_fontString);
			m_fontString = new CXmlFontString(m_pXmlUi, this);
			m_fontString->Inherit(pXmlListBox->m_fontString);
		}		
		m_textInsets		= pXmlListBox->m_textInsets;
		m_fadeDuration		= pXmlListBox->m_fadeDuration;
		m_insertMode		= pXmlListBox->m_insertMode;
	}
}