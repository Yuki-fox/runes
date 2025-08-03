#pragma once

#include "xmlbutton.h"

class CXmlCheckButton : public CXmlButton
{
public:
	CXmlCheckButton(CXmlUi* pXmlUi=NULL, CXmlLayout* pParent=NULL);
	virtual ~CXmlCheckButton();

	virtual void				LoadNodeAttribute(const DOMNamedNodeMap* nodeMap);
	virtual	bool				LoadNodeElement(const DOMNode* node);
	virtual	void				Inherit(CXmlLayout* pXmlLayout);

public:
	CXmlTexture*				m_checkedTexture;
	CXmlTexture*				m_disabledCheckedTexture;
	bool						m_checked;
	bool						m_autoChecked;
};