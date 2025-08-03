#pragma once

#include <map>
#include <vector>
#include "xmlfontstring.h"
#include "xmltexture.h"

using namespace std;

class CXmlBackdrop : public IXmlBase
{
public:
	CXmlBackdrop();

	virtual void				LoadNodeAttribute(const DOMNamedNodeMap* nodeMap);
	virtual	bool				LoadNodeElement(const DOMNode* node);
public:
	CXmlInset					m_bgInsets;
	CXmlValue					m_tileSize;
	CXmlValue					m_edgeSize;
	string						m_tileFile;
	string						m_edgeFile;
	bool						m_isTile;
};

class CXmlResizeBounds : public IXmlBase
{
public:
	CXmlResizeBounds();
	virtual ~CXmlResizeBounds();

	virtual	bool				LoadNodeElement(const DOMNode* node);
public:	
	CXmlDimension*				m_maxBounds;
	CXmlDimension*				m_minBounds;
};

class CXmlFrame : public CXmlLayout
{
public:

	enum {
		eLayer_Background			= 0,
		eLayer_Border,
		eLayer_Artwork,
		eLayer_Overlay,
		eLayer_Highlight
	};

	enum {
		eStrata_Background			= 0,
		eStrata_Low,
		eStrata_Medium,
		eStrata_High,
		eStrata_Dialog,
		eStrata_Tooltip
	};
	
	CXmlFrame(CXmlUi* pXmlUi=NULL, CXmlLayout* pParent=NULL);
	virtual ~CXmlFrame();

	static	int					XmlValueToLayerLevel(const wchar_t* valueStr);
	static	int					XmlValueToFrameStrata(const wchar_t* valueStr);

	virtual void				LoadNodeAttribute(const DOMNamedNodeMap* nodeMap);
	virtual	bool				LoadNodeElement(const DOMNode* node);
	virtual	void				Inherit(CXmlLayout* pXmlLayout);
	virtual	CXmlFrame*			CreateInheritFrame(CXmlFrame* pXmlFrame);
protected:
	// layers
	virtual	void				LoadXmlLayers(const DOMNode* node);
	virtual	void				LoadXmlLayer(const DOMNode* node);

	virtual void				LoadXmlFrames(const DOMNode* node);

	// scripts
	virtual void				LoadXmlScripts(const DOMNode* node);
	virtual void				InsertScript(const char* script, const char* content);
	
	virtual	void				InheritFrames(CXmlFrame* pXmlFrame);	
	virtual	void				InheritLayers(CXmlFrame* pXmlFrame);

public:
	// member values
	multimap<int, CXmlLayout*>	m_layers;
	vector<CXmlFrame*>			m_frames;
	map<string, string>			m_scripts;

	CXmlResizeBounds			m_resizeBounds;
	CXmlInset					m_hitRectInsets;
	CXmlBackdrop*				m_pBackdrop;
	CXmlLayout*					m_pTitleRegion;
	float						m_alpha;
	int							m_id;
	int							m_frameStrata;
	int							m_frameLevel;
	bool						m_topLevel;
	bool						m_isEnableMouse;
	bool						m_isEnableKeyboard;
	bool						m_isMovable;
	bool						m_isResizable;	
};