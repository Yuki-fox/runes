#include "xmlui.h"

// ----------------------------------------------------------------------------
CXmlScrollFrame::CXmlScrollFrame(CXmlUi* pXmlUi, CXmlLayout* pParent)
	: CXmlFrame(pXmlUi, pParent)
{
	m_type				= eXmlUiType_ScrollFrame;
	m_pScrollChild		= NULL;
}

// ----------------------------------------------------------------------------
CXmlScrollFrame::~CXmlScrollFrame()
{
	XML_SAFE_DLETE(m_pScrollChild);
}

// ----------------------------------------------------------------------------
bool CXmlScrollFrame::LoadNodeElement(const DOMNode* node)
{
	if ( CXmlFrame::LoadNodeElement(node) ) {
		return true;
	}

	const XMLCh* nodeName = node->getNodeName();
	if ( XMLString::compareString(nodeName, L"ScrollChild") == 0 ) {
		LoadXmlScrollChild(node);
		return true;
	}
	return false;

}

// ----------------------------------------------------------------------------
void CXmlScrollFrame::Inherit(CXmlLayout* pXmlLayout)
{
	CXmlFrame::Inherit(pXmlLayout);
	CXmlScrollFrame* pXmlScrollFrame = dynamic_cast<CXmlScrollFrame*>(pXmlLayout);

	if (pXmlScrollFrame)
	{
		if ( pXmlScrollFrame->m_pScrollChild )
		{
			XML_SAFE_DLETE(m_pScrollChild);
			m_pScrollChild = CreateInheritFrame(pXmlScrollFrame->m_pScrollChild);
			m_pScrollChild->Inherit(pXmlScrollFrame->m_pScrollChild);
		}
	}
}

// ----------------------------------------------------------------------------
void CXmlScrollFrame::LoadXmlScrollChild(const DOMNode* node)
{
	if (m_pXmlUi == NULL)
		return;

	// Element
	for ( DOMNode *child = node->getFirstChild(); child != 0; child = child->getNextSibling() ) {
		if ( child->getNodeType() == DOMNode::ELEMENT_NODE ) {
			CXmlFrame* pXmlFrame = m_pXmlUi->CreateXmlFrames(child->getNodeName(), this);
			if ( pXmlFrame ) {
				if ( pXmlFrame->LoadFromDOMNode(child) ) {
					// 子元件虛擬狀態需和父元件一致
					pXmlFrame->m_parent		= this;
					pXmlFrame->m_isVirtual	= m_isVirtual;
					m_pScrollChild			= pXmlFrame;
				}
				else
					XML_SAFE_DLETE(pXmlFrame);				
			}
			break;
		}
	}
}