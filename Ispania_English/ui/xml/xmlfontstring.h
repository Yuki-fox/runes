#pragma once

#include "xmllayout.h"

class CXmlShadow : public IXmlBase
{
public:
	CXmlShadow();
	virtual	bool				LoadNodeElement(const DOMNode* node);

public:
	CXmlColor					m_color;
	CXmlDimension				m_offset;
};

class CXmlFontString : public CXmlLayout
{
public:	
	CXmlFontString(CXmlUi* pXmlUi=NULL, CXmlLayout* pParent=NULL);
	virtual ~CXmlFontString();

	enum {
		eFontWeight_Thin		= 0,
		eFontWeight_Normal,
		eFontWeight_Bold,
	};

	enum {
		eJustifyVType_Top		= 0,
		eJustifyVType_Middle	= 1,
		eJustifyVType_Bottom	= 2,
	};

	enum {
		eJustifyHType_Left		= 0,
		eJustifyHType_Center	= 1,
		eJustifyHType_Right		= 2,
	};

	enum {
		eOutline_None			= 0,
		eOutline_Normal			= 1,
		eOutline_Thick			= 2,
	};

	static	int					XmlValueToJustifyVType( const wchar_t* valueStr );
	static	int					XmlValueToJustifyHType( const wchar_t* valueStr );
	static	int					XmlValueToFontWeight( const wchar_t* valueStr );
	static	int					XmlValueToOutline( const wchar_t* valueStr );

	virtual void				LoadNodeAttribute(const DOMNamedNodeMap* nodeMap);
	virtual	bool				LoadNodeElement(const DOMNode* node);
	virtual	void				Inherit(CXmlLayout* pXmlLayout);

public:
	CXmlShadow*					m_pShadow;
	CXmlColor					m_color;
	CXmlValue					m_height;
	string						m_font;
	string						m_text;
	float						m_spacing;
	int							m_outline;
	int							m_weight;
	int							m_bytes;
	int							m_justifyV;
	int							m_justifyH;
};