#pragma once

#include "xmlframe.h"

class CXmlStatusBar : public CXmlFrame
{
public:
	CXmlStatusBar(CXmlUi* pXmlUi=NULL, CXmlLayout* pParent=NULL);
	virtual ~CXmlStatusBar();
	
	virtual void				LoadNodeAttribute(const DOMNamedNodeMap* nodeMap);
	virtual	bool				LoadNodeElement(const DOMNode* node);
	virtual	void				Inherit(CXmlLayout* pXmlLayout);

public:
	CXmlTexture*				m_barTexture;
	CXmlColor					m_barColor;
	float						m_minValue;
	float						m_maxValue;
	float						m_defaultValue;
	bool						m_reverse;
};