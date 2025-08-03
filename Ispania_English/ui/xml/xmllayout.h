#pragma once

#include <string>
#include <vector>
#include "xmlbase.h"

using namespace std;

// ----------------------------------------------------------------------------
class CXmlAnchor : public IXmlBase
{
public:
	CXmlAnchor();

	enum
	{
		eAnchor_Null					= 0,
		eAnchor_TopLeft					= 1,
		eAnchor_Top						= 2,
		eAnchor_TopRight				= 3,
		eAnchor_Left					= 4,
		eAnchor_Center					= 5,
		eAnchor_Right					= 6,
		eAnchor_BottomLeft				= 7,
		eAnchor_Bottom					= 8,
		eAnchor_BottomRight				= 9
	};

	static	int					XmlValueToAnchorPoint(const XMLCh* valueStr);

	virtual void				LoadNodeAttribute(const DOMNamedNodeMap* nodeMap);
	virtual	bool				LoadNodeElement(const DOMNode* node);

public:
	string						m_relativeTo;
	CXmlDimension				m_offset;
	int							m_point;
	int							m_relativePoint;
};

// ----------------------------------------------------------------------------
class CXmlAnchors : public IXmlBase
{
public:
	CXmlAnchors();
	virtual	bool				LoadNodeElement(const DOMNode* node);

public:
	vector<CXmlAnchor>			m_vector;
};

// ----------------------------------------------------------------------------
class CXmlUi;
class CXmlLayout : public CXmlNode
{
public:
	CXmlLayout(CXmlUi* pUi=NULL, CXmlLayout* pParent=NULL);
	virtual ~CXmlLayout();
	
	virtual void				LoadNodeAttribute(const DOMNamedNodeMap* nodeMap);
	virtual	bool				LoadNodeElement(const DOMNode* node);

	virtual	void				Inherit(CXmlLayout* pXmlLayout);
	virtual const char*			GetName()							{ return m_name.c_str(); }
	virtual	bool				IsVirtual()							{ return m_isVirtual; }
	virtual	void				ChangeName(string &str);

public:
	CXmlUi*						m_pXmlUi;
	
	CXmlLayout*					m_parent;
	string						m_name;					// ¤¸¥ó¦WºÙ
	CXmlAnchors					m_anchors;
	CXmlDimension				m_size;
	bool						m_isVirtual;			//
	bool						m_isVisible;
};

extern const char* g_xml_key_parent;