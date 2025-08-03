#pragma once

#include "xmlframe.h"

class CXmlSlider : public CXmlFrame
{
public:	
	CXmlSlider(CXmlUi* pXmlUi=NULL, CXmlLayout* pParent=NULL);
	virtual ~CXmlSlider();	

	enum
	{
		eOrientation_Vertical	= 0,
		eOrientation_Horizontal	= 1,
	};

	virtual void				LoadNodeAttribute(const DOMNamedNodeMap* nodeMap);
	virtual	bool				LoadNodeElement(const DOMNode* node);
	virtual	void				Inherit(CXmlLayout* pXmlLayout);

public:
	CXmlTexture*				m_thumbTexture;
	int							m_orientation;
	float						m_minValue;
	float						m_maxValue;
	float						m_defaultValue;
	float						m_smallStep;
	float						m_largeStep;	
};