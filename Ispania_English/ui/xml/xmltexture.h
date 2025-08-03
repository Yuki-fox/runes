#pragma once

#include <vector>
#include "xmllayout.h"

class CXmlTexCoords : public IXmlBase
{
public:    
	CXmlTexCoords();
	virtual void				LoadNodeAttribute(const DOMNamedNodeMap* nodeMap);

public:
	float						m_left, m_top, m_right, m_bottom;
};

class CXmlTexture : public CXmlLayout
{
public:
	CXmlTexture(CXmlUi* pXmlUi=NULL, CXmlLayout* pParent=NULL);
	virtual ~CXmlTexture();

	enum {
		eAlphaMode_Disable		= 0,
		eAlphaMode_Blend,
		eAlphaMode_Add
	};

	int							XmlValueToAlphaMode(const wchar_t* valueStr);
	virtual void				LoadNodeAttribute(const DOMNamedNodeMap* nodeMap);
	virtual	bool				LoadNodeElement(const DOMNode* node);
	virtual	void				Inherit(CXmlLayout* pXmlLayout);

public:
	string						m_file;
	vector<CXmlTexCoords>		m_texCoords;			// tu,tv¶b®y¼Ð
	CXmlColor					m_color;
	int							m_alphaMode;
	float						m_animationSpeed;
};