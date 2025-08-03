#include "xmlmodel.h"

// ----------------------------------------------------------------------------
CXmlModel::CXmlModel(CXmlUi* pXmlUi, CXmlLayout* pParent)
: CXmlFrame(pXmlUi, pParent)
{
	m_type				= eXmlUiType_Model;

	m_scale				= 1.0f;
	m_fogNear			= 0.0f;
	m_fogFar			= 1.0f;
}

// ----------------------------------------------------------------------------
CXmlModel::~CXmlModel()
{
}

// ----------------------------------------------------------------------------
void CXmlModel::LoadNodeAttribute(const DOMNamedNodeMap* nodeMap)
{
	CXmlFrame::LoadNodeAttribute(nodeMap);

	DOMNode* attr;
	if ( (attr = nodeMap->getNamedItem(L"file")) != NULL ) {
		char* temp = XMLString::transcode(attr->getNodeValue());
		m_file = temp;
		XMLString::release(&temp);
	}
	if ( (attr = nodeMap->getNamedItem(L"targetBone")) != NULL ) {
		char* temp = XMLString::transcode(attr->getNodeValue());
		m_targetBone = temp;
		XMLString::release(&temp);
	}
	if ( (attr = nodeMap->getNamedItem(L"maskFile")) != NULL ) {
		char* temp = XMLString::transcode(attr->getNodeValue());
		m_maskFile = temp;
		XMLString::release(&temp);
	}
	if ( (attr = nodeMap->getNamedItem(L"scale")) != NULL ) {
		m_scale = (float)_wtof(attr->getNodeValue());
	}
	if ( (attr = nodeMap->getNamedItem(L"fogNear")) != NULL ) {
		m_fogNear = (float)_wtof(attr->getNodeValue());
	}
	if ( (attr = nodeMap->getNamedItem(L"fogFar")) != NULL ) {
		m_fogFar = (float)_wtof(attr->getNodeValue());
	}
}

// ----------------------------------------------------------------------------
bool CXmlModel::LoadNodeElement(const DOMNode* node)
{
	if ( CXmlFrame::LoadNodeElement(node) ) {
		return true;
	}

	const XMLCh* nodeName = node->getNodeName();
	if ( XMLString::compareString(nodeName, L"FogColor") == 0 ) {
		m_fogColor.LoadFromDOMNode(node);
		return true;
	}

	return false;
}

// ----------------------------------------------------------------------------
void CXmlModel::Inherit(CXmlLayout* pXmlLayout)
{
	CXmlFrame::Inherit(pXmlLayout);
	CXmlModel* pXmlModel = dynamic_cast<CXmlModel*>(pXmlLayout);
	if (pXmlModel)
	{
		m_file			= pXmlModel->m_file;
		m_fogColor		= pXmlModel->m_fogColor;
		m_scale			= pXmlModel->m_scale;
		m_fogNear		= pXmlModel->m_fogNear;
		m_fogFar		= pXmlModel->m_fogFar;
		m_targetBone	= pXmlModel->m_targetBone;
	}
}