#pragma once

#include "xmlframe.h"

class CXmlScrollFrame : public CXmlFrame
{
public:	
	CXmlScrollFrame(CXmlUi* pXmlUi=NULL, CXmlLayout* pParent=NULL);
	virtual ~CXmlScrollFrame();

	virtual	bool				LoadNodeElement(const DOMNode* node);
	virtual	void				Inherit(CXmlLayout* pXmlLayout);
protected:
	virtual	void				LoadXmlScrollChild(const DOMNode* node);
public:
	CXmlFrame*					m_pScrollChild;
};