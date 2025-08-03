#pragma once

#include <string>
#include "xmlbase.h"

using namespace std;

class CXmlScript : public CXmlNode
{
public:
	CXmlScript(const string& pathname);
	virtual	bool				LoadFromDOMNode(const DOMNode* node);
	virtual const char*			GetFile()							{ return m_file.c_str(); }
	virtual	const char*			GetContent()						{ return m_content.c_str(); }

public:
	string						m_pathname;
	string						m_file;
	string						m_content;
};