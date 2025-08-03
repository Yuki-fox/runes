#pragma once

#include <stdio.h>
#include <vector>
#include <map>
#include "xmlscript.h"
#include "xmlfontstring.h"
#include "xmltexture.h"
#include "xmlframe.h"
#include "xmlmodel.h"
#include "xmlbutton.h"
#include "xmlcheckbutton.h"
#include "xmlradiobutton.h"
#include "xmlslider.h"
#include "xmlstatusbar.h"
#include "xmleditbox.h"
#include "xmllistbox.h"
#include "xmlmessageframe.h"
#include "xmlscrollframe.h"
#include "xmlTooltip.h"
//#include "xmlWebbrowser.h"

using namespace std;

class CXmlUi;
class CXmlFile;

class CXmlBinding : public IXmlBase
{
public:
	CXmlBinding();

	virtual	bool			LoadFromDOMNode(const DOMNode* node);
public:
	string					m_name;
	string					m_header;
	string					m_content;
	bool					m_runOnUp;
};

class CXmlFile
{
public:
	CXmlFile(CXmlUi* pXmlUi);
	~CXmlFile();

	virtual	bool			LoadFile(const string& pathname, const string& filename);	
public:
	CXmlUi*					m_pXmlUi;
	vector<CXmlNode*>		m_nodes;
	string					m_luaFilename;
	string					m_pathname;
	string					m_filename;
};

class CXmlUi
{
public:	
	CXmlUi();
	virtual ~CXmlUi();
	static	bool			Initialize();
	static	void			Release();

	virtual	bool			OpenToc(const char* filename);
	virtual	bool			LoadBindingFromFile(const char* filename);
	virtual bool			LoadBindingFromMemory(const void* memory, int size);
	virtual bool			LoadXmlFromFile(const char* filename);
	virtual bool			LoadXmlFromMemory(const void* memory, int size, const char* filename=NULL);	
	virtual	void			AddName(const char* name, CXmlLayout* layout);
	virtual CXmlLayout*		FindByName(const char* name);
	virtual	CXmlLayout*		CreateXmlLayers(const wchar_t* typeName, CXmlLayout* parent, const char* inherit=NULL, const char* name=NULL);
	virtual	CXmlFrame*		CreateXmlFrames(const wchar_t* typeName, CXmlLayout* parent, const char* inherit=NULL, const char* name=NULL);
	virtual void			PushErrorMsg(const char* format, ...);

	virtual CXmlNode*		GetFirstXmlNode();
	virtual CXmlNode*		GetNextXmlNode();

	virtual	string			GetFileExt(const string& filename);
	virtual	string			GetPathName(const string& filename);

protected:	
	//virtual	bool			LoadFile(const string& pathname, const string& filename);
	virtual	bool			ReadLine(FILE *fptr, string& lineStr);
	virtual	bool			CheckTocFile(const string& filename);
	virtual	bool			CheckXmlFile(const string& filename);
	virtual	bool			CheckLuaFile(const string& filename);	

public:
	map<string, CXmlLayout*>	m_nameIndex;
	vector<CXmlBinding*>		m_bindings;
	vector<CXmlFile*>			m_files;
	vector<string>				m_errorList;
	CXmlFile*					m_currentLoadFile;
	string						m_filename;

	vector<CXmlNode*>			m_nodes;
	int							m_lastNode;
};

