#pragma once

#include "xmlframe.h"

class CXmlButton : public CXmlFrame
{
public:
	CXmlButton(CXmlUi* pXmlUi=NULL, CXmlLayout* pParent=NULL);
	virtual ~CXmlButton();

	virtual void				LoadNodeAttribute(const DOMNamedNodeMap* nodeMap);
	virtual	bool				LoadNodeElement(const DOMNode* node);
	virtual	void				DefaultFontString(CXmlFontString* fontString);
	virtual	void				Inherit(CXmlLayout* pXmlLayout);

public:
	string						m_text;
	CXmlTexture*				m_normalTexture;
	CXmlTexture*				m_pushedTexture;
	CXmlTexture*				m_disabledTexture;
	CXmlTexture*				m_highlightTexture;
	CXmlFontString*				m_normalText;
	CXmlFontString*				m_disabledText;
	CXmlFontString*				m_highlightText;
	CXmlDimension				m_pushedTextOffset;
};