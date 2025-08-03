#pragma once

#include "xmlframe.h"

class CXmlMessageFrame : public CXmlFrame
{
public:	
	CXmlMessageFrame(CXmlUi* pXmlUi=NULL, CXmlLayout* pParent=NULL);
	virtual ~CXmlMessageFrame();
	
	virtual void				LoadNodeAttribute(const DOMNamedNodeMap* nodeMap);
	virtual	bool				LoadNodeElement(const DOMNode* node);
	virtual	void				Inherit(CXmlLayout* pXmlLayout);

public:
	CXmlFontString*				m_fontString;
	CXmlInset					m_textInsets;
	bool						m_isFade;
	float						m_fadeDuration;
	float						m_displayDuration;
	int							m_maxLines;
};