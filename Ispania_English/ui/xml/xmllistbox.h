#pragma once

#include "xmlframe.h"

class CXmlListBox : public CXmlFrame
{
public:
	CXmlListBox(CXmlUi* pXmlUi=NULL, CXmlLayout* pParent=NULL);
	virtual ~CXmlListBox();	

	enum 
	{
		eInsertMode_Top			= 0,
		eInsertMode_Bottom		= 1,
	};

	static	int					XmlValueToInsertMode(const wchar_t* valueStr);

	virtual void				LoadNodeAttribute(const DOMNamedNodeMap* nodeMap);
	virtual	bool				LoadNodeElement(const DOMNode* node);
	virtual	void				Inherit(CXmlLayout* pXmlLayout);

public:
	CXmlFontString*				m_fontString;
	CXmlInset					m_textInsets;
	float						m_fadeDuration;
	int							m_insertMode;
};