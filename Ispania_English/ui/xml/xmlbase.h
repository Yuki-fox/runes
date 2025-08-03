#pragma once

#include <stdio.h>
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>
#include <xercesc/dom/DOMAttr.hpp>
#include <xercesc/util/XMLString.hpp>

XERCES_CPP_NAMESPACE_USE

#define XML_SAFE_DLETE(x)				if (x) { delete x; x = NULL; }
#define XML_SAFE_DELETE_ARRAY(x)		if (x) { delete [] x; x = NULL; }

enum XmlUiType {
	eXmlUiType_Node				= 0,
	eXmlUiType_Layout			,
	eXmlUiType_Script			,
	eXmlUiType_FontString		,
	eXmlUiType_Texture			,
	eXmlUiType_Frame			,
	eXmlUiType_Model			,
	eXmlUiType_Button			,
	eXmlUiType_CheckButton		,
	eXmlUiType_RadioButton		,
	eXmlUiType_StatusBar		,
	eXmlUiType_Slider			,
	eXmlUiType_EditBox			,
	eXmlUiType_ListBox			,
	eXmlUiType_MessageFrame		,
	eXmlUiType_ScrollFrame		,
	eXmlUiType_Tooltip			,
};

class IXmlBase
{
public:
	virtual	bool				LoadFromDOMNode(const DOMNode* node);	
	virtual void				LoadNodeAttribute(const DOMNamedNodeMap* nodeMap)			{}
	virtual	bool				LoadNodeElement(const DOMNode* node)						{ return false; }

	static	const XMLCh*		GetNodeAttribute(const DOMNamedNodeMap* nodeMap, const XMLCh* name);
	static	bool				XmlValueToBoolean(const XMLCh* valueStr);
};

class CXmlNode : public IXmlBase
{
public:
	CXmlNode();	
	virtual XmlUiType			GetType()								{ return m_type; }

public:
	XmlUiType					m_type;
};

class CXmlValue : public IXmlBase
{
public:
	CXmlValue();	
	virtual	bool				LoadNodeElement(const DOMNode* node);

public:
	float						m_val;
};

class CXmlDimension : public IXmlBase
{
public:
	CXmlDimension();
	virtual	bool				LoadNodeElement(const DOMNode* node);

public:
	float						m_x, m_y;
};

class CXmlInset : public IXmlBase
{
public:
	CXmlInset();
	virtual	bool				LoadNodeElement(const DOMNode* node);

public:
	float						m_left, m_top, m_right, m_bottom;
};

class CXmlColor : public IXmlBase
{
public:
	CXmlColor();
	virtual void				LoadNodeAttribute(const DOMNamedNodeMap* nodeMap);

public:
	float						m_a, m_r, m_g, m_b;
};

char* RemoveSpaceWord(const char* instr, char* outstr, int outsize);