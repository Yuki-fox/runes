#pragma once

#include "xmlcheckbutton.h"

class CXmlRadioButton : public CXmlCheckButton
{
public:
	CXmlRadioButton(CXmlUi* pXmlUi=NULL, CXmlLayout* pParent=NULL);
	virtual ~CXmlRadioButton();
	
	virtual void				LoadNodeAttribute(const DOMNamedNodeMap* nodeMap);
	virtual	void				Inherit(CXmlLayout* pXmlLayout);

public:
	int							m_groupId;
};