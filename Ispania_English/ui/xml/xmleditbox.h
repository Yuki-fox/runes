#pragma once

#include "xmlframe.h"

class CXmlEditBox : public CXmlFrame
{
public:
	CXmlEditBox(CXmlUi* pXmlUi=NULL, CXmlLayout* pParent=NULL);
	virtual ~CXmlEditBox();	
	
	virtual void				LoadNodeAttribute(const DOMNamedNodeMap* nodeMap);
	virtual	bool				LoadNodeElement(const DOMNode* node);
	virtual	void				Inherit(CXmlLayout* pXmlLayout);

public:
	CXmlFontString*				m_fontString;
	CXmlInset					m_textInsets;
	CXmlColor					m_highlightColor;
	int							m_letters;
	int							m_historyLines;
	bool						m_isNumeric;
	bool						m_isPassword;
	bool						m_isMultiLine;
	bool						m_isAutoFocus;
};