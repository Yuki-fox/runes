#pragma once

#include "xmlframe.h"

class CXmlModel : public CXmlFrame
{
public:	
	CXmlModel(CXmlUi* pXmlUi=NULL, CXmlLayout* pParent=NULL);
	virtual ~CXmlModel();

	virtual void				LoadNodeAttribute(const DOMNamedNodeMap* nodeMap);
	virtual	bool				LoadNodeElement(const DOMNode* node);
	virtual	void				Inherit(CXmlLayout* pXmlLayout);

public:
	string						m_file;
	string						m_targetBone;
	string						m_maskFile;
	CXmlColor					m_fogColor;
	float						m_scale;
	float						m_fogNear;
	float						m_fogFar;
};