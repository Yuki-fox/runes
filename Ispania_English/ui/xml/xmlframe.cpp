/*
#include "xmlframe.h"
#include "xmlmodel.h"
#include "xmlbutton.h"
#include "xmlcheckbutton.h"
#include "xmlradiobutton.h"
#include "xmleditbox.h"
#include "xmlslider.h"
#include "xmlstatusbar.h"
#include "xmlmessageframe.h"
#include "xmllistbox.h"
*/
#include "xmlui.h"

// ----------------------------------------------------------------------------
CXmlBackdrop::CXmlBackdrop()
{	
	m_tileFile.clear();
	m_edgeFile.clear();
	m_isTile			= true;
}

// ----------------------------------------------------------------------------
void CXmlBackdrop::LoadNodeAttribute(const DOMNamedNodeMap* nodeMap)
{
	DOMNode* attr;
	if ( (attr = nodeMap->getNamedItem(L"bgFile")) != NULL ) {
		char* temp = XMLString::transcode(attr->getNodeValue());
		m_tileFile = temp;
		XMLString::release(&temp);
	}
	if ( (attr = nodeMap->getNamedItem(L"edgeFile")) != NULL ) {
		char* temp = XMLString::transcode(attr->getNodeValue());
		m_edgeFile = temp;
		XMLString::release(&temp);
	}
	if ( (attr = nodeMap->getNamedItem(L"tile")) != NULL ) {
		m_isTile = XmlValueToBoolean(attr->getNodeValue());
	}
}

// ----------------------------------------------------------------------------
bool CXmlBackdrop::LoadNodeElement(const DOMNode* node)
{
	if ( XMLString::compareString(node->getNodeName(), L"BackgroundInsets") == 0 ) {
		m_bgInsets.LoadFromDOMNode(node);
		return true;
	}
	if ( XMLString::compareString(node->getNodeName(), L"TileSize") == 0 ) {
		m_tileSize.LoadFromDOMNode(node);
		return true;
	}
	if ( XMLString::compareString(node->getNodeName(), L"EdgeSize") == 0 ) {
		m_edgeSize.LoadFromDOMNode(node);
		return true;
	}
	return false;
}

// ----------------------------------------------------------------------------
CXmlResizeBounds::CXmlResizeBounds()
{	
	m_maxBounds = NULL;
	m_minBounds = NULL;
}

// ----------------------------------------------------------------------------
CXmlResizeBounds::~CXmlResizeBounds()
{
	XML_SAFE_DLETE(m_maxBounds);
	XML_SAFE_DLETE(m_minBounds);
}

// ----------------------------------------------------------------------------
bool CXmlResizeBounds::LoadNodeElement(const DOMNode* node)
{
	if ( XMLString::compareString(node->getNodeName(), L"minResize") == 0 ) {
		XML_SAFE_DLETE(m_minBounds);
		m_minBounds = new CXmlDimension;
		m_minBounds->LoadFromDOMNode(node);
		return true;
	}
	if ( XMLString::compareString(node->getNodeName(), L"maxResize") == 0 ) {
		XML_SAFE_DLETE(m_maxBounds);
		m_maxBounds = new CXmlDimension;
		m_maxBounds->LoadFromDOMNode(node);
		return true;
	}
	return false;

}

// ----------------------------------------------------------------------------
CXmlFrame::CXmlFrame(CXmlUi* pXmlUi, CXmlLayout* pParent)
	: CXmlLayout(pXmlUi, pParent)
{
	m_type				= eXmlUiType_Frame;

	m_layers.clear();
	m_frames.clear();
	m_scripts.clear();	

	m_pBackdrop			= NULL;
	m_pTitleRegion		= NULL;
	m_alpha				= 1.0f;
	m_id				= 0;
	m_frameStrata		= eStrata_Medium;
	m_frameLevel		= 0;
	m_topLevel			= false;
	m_isEnableMouse		= false;
	m_isEnableKeyboard	= false;
	m_isMovable			= false;
	m_isResizable		= false;
}

// ----------------------------------------------------------------------------
CXmlFrame::~CXmlFrame()
{
	XML_SAFE_DLETE( m_pBackdrop );
	XML_SAFE_DLETE( m_pTitleRegion );

	// scripts
	m_scripts.clear();

	// frames
	for ( vector<CXmlFrame*>::iterator iter = m_frames.begin(); iter != m_frames.end(); iter++ )
	{
		delete *iter;
	}
	m_frames.clear();

	// layers
	for ( multimap<int, CXmlLayout*>::iterator iter = m_layers.begin(); iter != m_layers.end(); iter++ )
	{
		delete iter->second;
	}
	m_layers.clear();	
}

// ----------------------------------------------------------------------------
int	CXmlFrame::XmlValueToLayerLevel(const wchar_t* valueStr)
{
	if ( valueStr )
	{
		if ( XMLString::compareString(valueStr, L"BACKGROUND") == 0 )
			return eLayer_Background;
		else if ( XMLString::compareString(valueStr, L"BORDER") == 0 )
			return eLayer_Border;
		else if ( XMLString::compareString(valueStr, L"ARTWORK") == 0 )
			return eLayer_Artwork;
		else if ( XMLString::compareString(valueStr, L"OVERLAY") == 0 )
			return eLayer_Overlay;
		else if ( XMLString::compareString(valueStr, L"HIGHLIGHT") == 0 )
			return eLayer_Highlight;
	}
	return eLayer_Background;
}

// ----------------------------------------------------------------------------
int	CXmlFrame::XmlValueToFrameStrata(const wchar_t* valueStr)
{
	if ( valueStr )
	{
		if ( XMLString::compareString(valueStr, L"BACKGROUND") == 0 )
			return eStrata_Background;
		else if ( XMLString::compareString(valueStr, L"LOW") == 0 )
			return eStrata_Low;
		else if ( XMLString::compareString(valueStr, L"MEDIUM") == 0 )
			return eStrata_Medium;
		else if ( XMLString::compareString(valueStr, L"HIGH") == 0 )
			return eStrata_High;
		else if ( XMLString::compareString(valueStr, L"DIALOG") == 0 )
			return eStrata_Dialog;
		else if ( XMLString::compareString(valueStr, L"TOOLTIP") == 0 )
			return eStrata_Tooltip;
	}
	return eStrata_Background;
}

// ----------------------------------------------------------------------------
void CXmlFrame::LoadNodeAttribute(const DOMNamedNodeMap* nodeMap)
{
	CXmlLayout::LoadNodeAttribute(nodeMap);

	DOMNode* attr;
	if ( (attr = nodeMap->getNamedItem(L"alpha")) != NULL ) {
		m_alpha = (float)_wtof(attr->getNodeValue());
	}
	if ( (attr = nodeMap->getNamedItem(L"movable")) != NULL ) {
		m_isMovable = XmlValueToBoolean(attr->getNodeValue());
	}
	if ( (attr = nodeMap->getNamedItem(L"resizable")) != NULL ) {
		m_isResizable = XmlValueToBoolean(attr->getNodeValue());
	}
	if ( (attr = nodeMap->getNamedItem(L"frameStrata")) != NULL ) {
		m_frameStrata = XmlValueToFrameStrata(attr->getNodeValue());
	}
	if ( (attr = nodeMap->getNamedItem(L"frameLevel")) != NULL ) {
		m_frameLevel = _wtoi(attr->getNodeValue());
	}
	if ( (attr = nodeMap->getNamedItem(L"toplevel")) != NULL ) {
		m_topLevel = XmlValueToBoolean(attr->getNodeValue());
	}
	if ( (attr = nodeMap->getNamedItem(L"id")) != NULL ) {
		m_id = _wtoi(attr->getNodeValue());
	}
	if ( (attr = nodeMap->getNamedItem(L"enableMouse")) != NULL ) {
		m_isEnableMouse = XmlValueToBoolean(attr->getNodeValue());
	}
	if ( (attr = nodeMap->getNamedItem(L"enableKeyboard")) != NULL ) {
		m_isEnableKeyboard = XmlValueToBoolean(attr->getNodeValue());
	}
}

// ----------------------------------------------------------------------------
bool CXmlFrame::LoadNodeElement(const DOMNode* node)
{
	if ( CXmlLayout::LoadNodeElement(node) ) {
		return true;
	}

	const XMLCh* nodeName = node->getNodeName();
	if ( XMLString::compareString(nodeName, L"TitleRegion") == 0 ) {
		XML_SAFE_DLETE( m_pTitleRegion );
		m_pTitleRegion = new CXmlLayout;
		m_pTitleRegion->LoadFromDOMNode(node);
		return true;
	}
	if ( XMLString::compareString(nodeName, L"ResizeBounds") == 0 ) {
		m_resizeBounds.LoadFromDOMNode(node);
		return true;
	}
	if ( XMLString::compareString(nodeName, L"Backdrop") == 0 ) {
		XML_SAFE_DLETE(m_pBackdrop);
		m_pBackdrop = new CXmlBackdrop;
		m_pBackdrop->LoadFromDOMNode(node);
		return true;
	}
	if ( XMLString::compareString(nodeName, L"HitRectInsets") == 0 ) {
		m_hitRectInsets.LoadFromDOMNode(node);
		return true;
	}
	if ( XMLString::compareString(nodeName, L"Layers") == 0 ) {
		LoadXmlLayers(node);
		return true;
	}
	if ( XMLString::compareString(nodeName, L"Frames") == 0 ) {
		LoadXmlFrames(node);
		return true;
	}
	if ( XMLString::compareString(nodeName, L"Scripts") == 0 ) {
		LoadXmlScripts(node);
		return true;
	}
	return false;
}

// ----------------------------------------------------------------------------
void CXmlFrame::Inherit(CXmlLayout* pXmlLayout)
{
	CXmlLayout::Inherit(pXmlLayout);
	CXmlFrame* pXmlFrame = dynamic_cast<CXmlFrame*>(pXmlLayout);
	if (pXmlFrame)
	{
		InheritFrames(pXmlFrame);
		InheritLayers(pXmlFrame);

		m_scripts			= pXmlFrame->m_scripts;
		m_resizeBounds		= pXmlFrame->m_resizeBounds;
		m_hitRectInsets		= pXmlFrame->m_hitRectInsets;
		m_alpha				= pXmlFrame->m_alpha;
		m_id				= pXmlFrame->m_id;
		m_frameStrata		= pXmlFrame->m_frameStrata;
		m_frameLevel		= pXmlFrame->m_frameLevel;
		m_topLevel			= pXmlFrame->m_topLevel;
		m_isEnableMouse		= pXmlFrame->m_isEnableMouse;
		m_isEnableKeyboard	= pXmlFrame->m_isEnableKeyboard;
		m_isMovable			= pXmlFrame->m_isMovable;
		m_isResizable		= pXmlFrame->m_isResizable;

		if (pXmlFrame->m_pBackdrop)
		{
			XML_SAFE_DLETE(m_pBackdrop);
			m_pBackdrop = new CXmlBackdrop;
			*m_pBackdrop = *pXmlFrame->m_pBackdrop;
		}

		if (pXmlFrame->m_pTitleRegion)
		{
			XML_SAFE_DLETE(m_pTitleRegion);
			m_pTitleRegion = new CXmlLayout(m_pXmlUi);
			*m_pTitleRegion = *pXmlFrame->m_pTitleRegion;
		}
	}
}

// ----------------------------------------------------------------------------
CXmlFrame* CXmlFrame::CreateInheritFrame(CXmlFrame* pXmlFrame)
{
	CXmlFrame* pXmlDesc = NULL;
	if (pXmlFrame)
	{
		switch (pXmlFrame->GetType())
		{
		case eXmlUiType_Frame:
			pXmlDesc = new CXmlFrame(m_pXmlUi, this);
			break;
		case eXmlUiType_Model:
			pXmlDesc = new CXmlModel(m_pXmlUi, this);
			break;
		case eXmlUiType_Button:
			pXmlDesc = new CXmlButton(m_pXmlUi, this);
			break;
		case eXmlUiType_CheckButton:
			pXmlDesc = new CXmlCheckButton(m_pXmlUi, this);
			break;
		case eXmlUiType_RadioButton:
			pXmlDesc = new CXmlRadioButton(m_pXmlUi, this);
			break;
		case eXmlUiType_StatusBar:
			pXmlDesc = new CXmlStatusBar(m_pXmlUi, this);
			break;
		case eXmlUiType_Slider:
			pXmlDesc = new CXmlSlider(m_pXmlUi, this);
			break;
		case eXmlUiType_EditBox:
			pXmlDesc = new CXmlEditBox(m_pXmlUi, this);
			break;
		case eXmlUiType_ListBox:
			pXmlDesc = new CXmlListBox(m_pXmlUi, this);
			break;
		case eXmlUiType_MessageFrame:
			pXmlDesc = new CXmlMessageFrame(m_pXmlUi, this);
			break;
		case eXmlUiType_ScrollFrame:
			pXmlDesc = new CXmlScrollFrame(m_pXmlUi, this);
			break;
		}
	}
	return pXmlDesc;
}

// ----------------------------------------------------------------------------
void CXmlFrame::LoadXmlLayers(const DOMNode* node)
{
	// Element
	node = node->getFirstChild();
	while ( node )
	{
		if ( node->getNodeType() == DOMNode::ELEMENT_NODE )
		{
			char* nodeName = XMLString::transcode(node->getNodeName());

			// Layer
			if ( XMLString::compareString(nodeName, "Layer") == 0 )
			{
				LoadXmlLayer(node);
			}			
			XMLString::release(&nodeName);
		}
		node = node->getNextSibling();
	}
}

// ----------------------------------------------------------------------------
void CXmlFrame::LoadXmlLayer(const DOMNode* node)
{
	int level = eLayer_Background;
	if (m_pXmlUi == NULL)
		return;	

	// Attribute
	if( node->hasAttributes() ) {
		DOMNode* attrNode = node->getAttributes()->getNamedItem(L"level");
		if ( attrNode ) {
			level = XmlValueToLayerLevel(attrNode->getNodeValue());
		}
	}

	// Element
	for ( DOMNode *child = node->getFirstChild(); child != 0; child = child->getNextSibling() ) {
		if ( child->getNodeType() == DOMNode::ELEMENT_NODE ) {
			CXmlLayout* pXmlLayout = m_pXmlUi->CreateXmlLayers(child->getNodeName(), this);
			if (pXmlLayout)	{
				if ( pXmlLayout->LoadFromDOMNode(child) ) {
					// 子元件虛擬狀態需和父元件一致
					// pXmlLayout->m_parent	= this;
					// pXmlLayout->m_isVirtual = m_isVirtual;
					m_layers.insert( make_pair(level, pXmlLayout) );
				}
				else
					delete pXmlLayout;
			}
		}
	}
}

// ----------------------------------------------------------------------------
void CXmlFrame::LoadXmlFrames(const DOMNode* node)
{
	if (m_pXmlUi == NULL)
		return;

	// Element
	for ( DOMNode *child = node->getFirstChild(); child != 0; child = child->getNextSibling() ) {
		if ( child->getNodeType() == DOMNode::ELEMENT_NODE ) {
			CXmlFrame* pXmlFrame = m_pXmlUi->CreateXmlFrames(child->getNodeName(), this);
			if ( pXmlFrame ) {
				if ( pXmlFrame->LoadFromDOMNode(child) )	{
					// 子元件虛擬狀態需和父元件一致
					pXmlFrame->m_parent		= this;
					pXmlFrame->m_isVirtual	= m_isVirtual;
					m_frames.push_back(pXmlFrame);
				}
				else
					XML_SAFE_DLETE(pXmlFrame);
			}
		}
	}
}

// ----------------------------------------------------------------------------
void CXmlFrame::LoadXmlScripts(const DOMNode* node)
{
	// Element
	node = node->getFirstChild();
	while ( node ) {
		if ( node->getNodeType() == DOMNode::ELEMENT_NODE ) {
			char* nodeName = XMLString::transcode(node->getNodeName());
			char* nodeContent = XMLString::transcode(node->getTextContent());
			InsertScript(nodeName, nodeContent);
			XMLString::release(&nodeName);
			XMLString::release(&nodeContent);
		}
		node = node->getNextSibling();
	}
}

// ----------------------------------------------------------------------------
void CXmlFrame::InsertScript(const char* script, const char* content)
{
	if ( script == NULL || content == NULL )
		return;

	map<string, string>::iterator iter = m_scripts.find(script);
	if ( iter != m_scripts.end() ) {
		iter->second = content;
	} else {
		m_scripts.insert(make_pair(script, content));
	}
	return;

	// 去除前後空白	
	int st = 0;	
	while ( content[st] && content[st] < ' ' ) {
		st++;
	}
	int ed = (int)strlen(content + st);
	while ( ed >= st ) {
		if ( content[ed] < ' ' )
			ed--;
		else {
			int size = ed + st + 1;
			char* temp = new char[size + 1];
			memcpy(temp, content + st, sizeof(char) * size);
			temp[size] = 0;

			map<string, string>::iterator iter = m_scripts.find(script);
			if ( iter != m_scripts.end() ) {
				iter->second = temp;
			} else {
				m_scripts.insert(make_pair(script, temp));
			}
			XML_SAFE_DELETE_ARRAY(temp);
			break;
		}
	}
}

// ----------------------------------------------------------------------------
void CXmlFrame::InheritFrames(CXmlFrame* pXmlFrame)
{
	vector<CXmlFrame*>::iterator iter = pXmlFrame->m_frames.begin();
	for (; iter != pXmlFrame->m_frames.end(); iter++)
	{
		CXmlFrame* pXmlDesc = CreateInheritFrame(*iter);

		if (pXmlDesc)
		{
			pXmlDesc->Inherit(*iter);
			m_frames.push_back(pXmlDesc);
		}
	}
}

// ----------------------------------------------------------------------------
void CXmlFrame::InheritLayers(CXmlFrame* pXmlFrame)
{
	multimap<int, CXmlLayout*>::iterator iter = pXmlFrame->m_layers.begin();
	for ( ; iter != pXmlFrame->m_layers.end(); iter++ )
	{
		CXmlLayout* pXmlLayout = NULL;
		switch(iter->second->GetType())
		{
		case eXmlUiType_FontString:
			pXmlLayout = new CXmlFontString(m_pXmlUi, this);
			break;
		case eXmlUiType_Texture:
			pXmlLayout = new CXmlTexture(m_pXmlUi, this);
			break;
		}

		if (pXmlLayout)
		{
			pXmlLayout->Inherit(iter->second);
			m_layers.insert( make_pair(iter->first, pXmlLayout) );
		}
	}
}