#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <stdio.h>
#include <stdarg.h>
#include "xmlui.h"

static const char*  gMemBufId = "RuneWaker";

// ----------------------------------------------------------------------------
CXmlBinding::CXmlBinding()
{
	m_name.clear();
	m_header.clear();
	m_content.clear();
	m_runOnUp = false;
}

// ----------------------------------------------------------------------------
bool CXmlBinding::LoadFromDOMNode(const DOMNode* node)
{
	// Attribute
	if( node->hasAttributes() ) {
		DOMNamedNodeMap *pAttributes = node->getAttributes();
		int nSize = (int)pAttributes->getLength();

		for ( int i = 0; i < nSize; ++i ) {
			DOMAttr* pAttributeNode = (DOMAttr*) pAttributes->item(i);
			if ( XMLString::compareString(pAttributeNode->getName(), L"name") == 0 ) {
				char* temp = XMLString::transcode(pAttributeNode->getValue());
				m_name = temp;
				XMLString::release(&temp);
			}
			if ( XMLString::compareString(pAttributeNode->getName(), L"runOnUp") == 0 ) {				
				m_runOnUp = XmlValueToBoolean(pAttributeNode->getValue());
			}
			if ( XMLString::compareString(pAttributeNode->getName(), L"header") == 0 ) {
				char* temp = XMLString::transcode(pAttributeNode->getValue());
				m_header = temp;
				XMLString::release(&temp);
			}
		}
	}	

	// script content
	char* temp = XMLString::transcode(node->getTextContent());
	m_content = temp;
	XMLString::release(&temp);

	return true;
}

// ----------------------------------------------------------------------------
CXmlFile::CXmlFile(CXmlUi* pXmlUi)
{
	m_pXmlUi = pXmlUi;

	m_luaFilename.clear();
	m_pathname.clear();
	m_filename.clear();
	m_nodes.clear();	
}

// ----------------------------------------------------------------------------
CXmlFile::~CXmlFile()
{
	for (vector<CXmlNode*>::iterator iter = m_nodes.begin(); iter != m_nodes.end(); iter++)
		delete *iter;
	m_nodes.clear();
}

// ----------------------------------------------------------------------------
bool CXmlFile::LoadFile(const string& pathname, const string& filename)
{
	bool resultBool = true;
	XercesDOMParser* parser = new XercesDOMParser();
	parser->setDoSchema(true);						// optional	
	//parser->setDoNamespaces(true);					// optional	
	parser->setValidationScheme(AbstractDOMParser::Val_Always);

	ErrorHandler* errHandler = (ErrorHandler*) new HandlerBase();
	parser->setErrorHandler(errHandler);

	m_pathname = pathname;
	m_filename = filename;

	try {		
		parser->parse(filename.c_str());
	}
	catch (const XMLException& toCatch) {
		wprintf(L"Exception message is: \n%s\n", toCatch.getMessage());
		resultBool = false;
	}
	catch (const DOMException& toCatch) {
		wprintf(L"Exception message is: \n%s\n", toCatch.msg);
		resultBool = false;
	}
	catch (...) {
		printf("[%s]Unexpected Exception \n", filename.c_str());
		resultBool = false;
	}

	if ( resultBool )
	{		
		DOMDocument* document = parser->getDocument();
		DOMElement* element = document->getDocumentElement();

		// Element
		for ( DOMNode *child = element->getFirstChild(); child != 0; child = child->getNextSibling() ) {
			if ( child->getNodeType() == DOMNode::ELEMENT_NODE ) {
				CXmlNode* pXmlNode = NULL;
				const XMLCh* nodeName = child->getNodeName();

				if ( XMLString::compareString(nodeName, L"Script") == 0 )
					pXmlNode = new CXmlScript(pathname);
				else {
					pXmlNode = m_pXmlUi->CreateXmlLayers(nodeName, NULL);
					if ( pXmlNode == NULL )
						pXmlNode = m_pXmlUi->CreateXmlFrames(nodeName, NULL);
				}		

				if ( pXmlNode ) {
					if ( pXmlNode->LoadFromDOMNode(child) )
						m_nodes.push_back(pXmlNode);
					else
						XML_SAFE_DLETE(pXmlNode);
				}
			}
		}
	}	

	delete parser;
	delete errHandler;

	return resultBool;
}

// ----------------------------------------------------------------------------
CXmlUi::CXmlUi()
{	
	m_nameIndex.clear();
	m_bindings.clear();
	m_files.clear();
	m_errorList.clear();
	m_currentLoadFile = NULL;

	m_nodes.clear();
	m_lastNode = 0;
}

// ----------------------------------------------------------------------------
CXmlUi::~CXmlUi()
{
	for ( vector<CXmlFile*>::iterator iter = m_files.begin(); iter != m_files.end(); iter++ ) {
		XML_SAFE_DLETE(*iter);
	}	
	for ( vector<CXmlBinding*>::iterator iter = m_bindings.begin(); iter != m_bindings.end(); iter++ ) {
		XML_SAFE_DLETE(*iter);
	}
	for ( vector<CXmlNode*>::iterator iter = m_nodes.begin(); iter != m_nodes.end(); iter++ ) {
		XML_SAFE_DLETE(*iter);
	}
	m_nodes.clear();
	m_files.clear();
	m_bindings.clear();
	m_nameIndex.clear();
}

// ----------------------------------------------------------------------------
bool CXmlUi::Initialize()
{
	// 初始化 Xerces-c++
	try {
		XMLPlatformUtils::Initialize();
	}
	catch (const XMLException& toCatch) {
		char* message = XMLString::transcode(toCatch.getMessage());
		printf("Error during initialization! :\n%s\n", message );
		XMLString::release(&message);
		return false;
	}
	return true;
}

// ----------------------------------------------------------------------------
void CXmlUi::Release()
{
	XMLPlatformUtils::Terminate();
}

// ----------------------------------------------------------------------------
// 開啟TOC檔案
bool CXmlUi::OpenToc(const char* filename)
{
	FILE *fptr;
	if ( filename == NULL || !CheckTocFile(filename) )
		return false;

	if ( (fptr = fopen( filename, "r" )) == NULL )
		return false;

	string path = GetPathName(string(filename));
	while ( !feof(fptr) )
	{
		string lineStr;		

		// 載入XML檔案
		if ( ReadLine(fptr, lineStr) )
		{
			if ( strncmp(lineStr.c_str(), "##", 2) != 0 )
			{
				m_currentLoadFile = NULL;
				if ( CheckXmlFile(lineStr) )
				{
					m_currentLoadFile = new CXmlFile(this);
					if ( m_currentLoadFile->LoadFile(path, path + lineStr) )
						m_files.push_back(m_currentLoadFile);
					else
						delete m_currentLoadFile;
				}
				else if ( CheckLuaFile(lineStr) )
				{
					m_currentLoadFile = new CXmlFile(this);
					m_currentLoadFile->m_luaFilename = path + lineStr;
					m_files.push_back(m_currentLoadFile);
				}
			}
		}
	}
	fclose( fptr );
	return true;
}

// ----------------------------------------------------------------------------
bool CXmlUi::LoadBindingFromFile(const char* filename)
{
	if ( filename == NULL )
		return false;

	bool resultBool = true;
	XercesDOMParser* parser = new XercesDOMParser();
	ErrorHandler* errHandler = (ErrorHandler*) new HandlerBase();
	parser->setErrorHandler(errHandler);

	try {		
		parser->parse(filename);
	}
	catch (const XMLException& toCatch) {
		char* message = XMLString::transcode(toCatch.getMessage());
		printf("Exception message is: \n%s\n", message);
		XMLString::release(&message);
		resultBool = false;
	}
	catch (const DOMException& toCatch) {
		char* message = XMLString::transcode(toCatch.msg);
		printf("Exception message is: \n%s\n", message);
		XMLString::release(&message);
		resultBool = false;
	}
	catch (...) {
		printf("[%s]Unexpected Exception \n", filename);
		resultBool = false;
	}

	if ( resultBool )
	{		
		DOMDocument* document = parser->getDocument();
		DOMElement* element = document->getDocumentElement();
		if ( XMLString::compareString(element->getTagName(), L"Bindings") == 0 )
		{
			// Element
			for ( DOMNode *node = element->getFirstChild(); node != 0; node = node->getNextSibling() ) {
				if ( node->getNodeType() == DOMNode::ELEMENT_NODE ) {
					if ( XMLString::compareString(node->getNodeName(), L"Binding") == 0 )
					{
						CXmlBinding* pXmlBinding = new CXmlBinding;
						pXmlBinding->LoadFromDOMNode(node);
						m_bindings.push_back(pXmlBinding);
					}
				}
			}
		}
	}	

	delete parser;
	delete errHandler;

	return resultBool;
}

// ----------------------------------------------------------------------------
bool CXmlUi::LoadBindingFromMemory(const void* memory, int size)
{
	MemBufInputSource* memBufIS = new MemBufInputSource((const XMLByte*)memory, size, gMemBufId, false);

	bool resultBool = true;
	XercesDOMParser* parser = new XercesDOMParser();
	ErrorHandler* errHandler = (ErrorHandler*) new HandlerBase();
	parser->setErrorHandler(errHandler);

	try {		
		parser->parse(*memBufIS);
	}
	catch (const XMLException& toCatch) {
		char* message = XMLString::transcode(toCatch.getMessage());
		printf("Exception message is: \n%s\n", message);
		XMLString::release(&message);
		resultBool = false;
	}
	catch (const DOMException& toCatch) {
		char* message = XMLString::transcode(toCatch.msg);
		printf("Exception message is: \n%s\n", message);
		XMLString::release(&message);
		resultBool = false;
	}
	catch (...) {
		printf("Unexpected Exception \n");
		resultBool = false;
	}

	if ( resultBool )
	{		
		DOMDocument* document = parser->getDocument();
		DOMElement* element = document->getDocumentElement();
		if ( XMLString::compareString(element->getTagName(), L"Bindings") == 0 )
		{
			// Element
			for ( DOMNode *node = element->getFirstChild(); node != 0; node = node->getNextSibling() ) {
				if ( node->getNodeType() == DOMNode::ELEMENT_NODE ) {
					if ( XMLString::compareString(node->getNodeName(), L"Binding") == 0 )
					{
						CXmlBinding* pXmlBinding = new CXmlBinding;
						pXmlBinding->LoadFromDOMNode(node);
						m_bindings.push_back(pXmlBinding);
					}
				}
			}
		}
	}	

	delete parser;
	delete errHandler;

	return resultBool;
}

// ----------------------------------------------------------------------------
bool CXmlUi::LoadXmlFromFile(const char* filename)
{
	bool resultBool = true;
	XercesDOMParser* parser = new XercesDOMParser();
	parser->setDoSchema(true);						// optional
	//parser->setDoNamespaces(true);					// optional	
	parser->setValidationScheme(AbstractDOMParser::Val_Always);

	ErrorHandler* errHandler = (ErrorHandler*) new HandlerBase();
	parser->setErrorHandler(errHandler);	

	try {		
		parser->parse(filename);
	}
	catch (const XMLException& toCatch) {
		wprintf(L"Exception message is: \n%s\n", toCatch.getMessage());
		resultBool = false;
	}
	catch (const DOMException& toCatch) {
		wprintf(L"Exception message is: \n%s\n", toCatch.msg);
		resultBool = false;
	}
	catch (...) {
		printf("[%s]Unexpected Exception \n", filename);
		resultBool = false;
	}

	if ( resultBool )
	{
		string pathname = GetPathName(string(filename));

		DOMDocument* document = parser->getDocument();
		DOMElement* element = document->getDocumentElement();

		// Element
		for ( DOMNode *child = element->getFirstChild(); child != 0; child = child->getNextSibling() ) {
			if ( child->getNodeType() == DOMNode::ELEMENT_NODE ) {				
				const XMLCh* nodeName = child->getNodeName();

				CXmlNode* pXmlNode = NULL;
				if ( XMLString::compareString(nodeName, L"Script") == 0 )
					pXmlNode = new CXmlScript(pathname);
				else {
					pXmlNode = CreateXmlLayers(nodeName, NULL);
					if ( pXmlNode == NULL )
						pXmlNode = CreateXmlFrames(nodeName, NULL);
				}		

				if ( pXmlNode ) {
					if ( pXmlNode->LoadFromDOMNode(child) )
						m_nodes.push_back(pXmlNode);
					else
						XML_SAFE_DLETE(pXmlNode);
				}
			}
		}
	}	

	delete parser;
	delete errHandler;

	return resultBool;
}

// ----------------------------------------------------------------------------
bool CXmlUi::LoadXmlFromMemory(const void* memory, int size, const char* filename)
{
	MemBufInputSource* memBufIS = new MemBufInputSource((const XMLByte*)memory, size, gMemBufId, false);

	bool resultBool = true;
	XercesDOMParser* parser = new XercesDOMParser();
	parser->setDoSchema(true);						// optional
	//parser->setDoNamespaces(true);					// optional	
	parser->setValidationScheme(AbstractDOMParser::Val_Always);

	ErrorHandler* errHandler = (ErrorHandler*) new HandlerBase();
	parser->setErrorHandler(errHandler);	

	string pathname = GetPathName(filename);

	try {		
		parser->parse(*memBufIS);
	}
	catch (const XMLException& toCatch) {
		wprintf(L"Exception message is: \n%s\n", toCatch.getMessage());
		resultBool = false;
	}
	catch (const DOMException& toCatch) {
		wprintf(L"Exception message is: \n%s\n", toCatch.msg);
		resultBool = false;
	}
	catch (...) {
		printf("[Memory Data]Unexpected Exception \n");
		resultBool = false;
	}

	if ( resultBool )
	{
		DOMDocument* document = parser->getDocument();
		DOMElement* element = document->getDocumentElement();

		// Element
		for ( DOMNode *child = element->getFirstChild(); child != 0; child = child->getNextSibling() ) {
			if ( child->getNodeType() == DOMNode::ELEMENT_NODE ) {				
				const XMLCh* nodeName = child->getNodeName();

				CXmlNode* pXmlNode = NULL;
				if ( XMLString::compareString(nodeName, L"Script") == 0 )
					pXmlNode = new CXmlScript(pathname);
				else {
					pXmlNode = CreateXmlLayers(nodeName, NULL);
					if ( pXmlNode == NULL )
						pXmlNode = CreateXmlFrames(nodeName, NULL);
				}		

				if ( pXmlNode ) {
					if ( pXmlNode->LoadFromDOMNode(child) )
						m_nodes.push_back(pXmlNode);
					else
						XML_SAFE_DLETE(pXmlNode);
				}
			}
		}
	}	

	delete parser;
	delete errHandler;

	return resultBool;

}

// ----------------------------------------------------------------------------
void CXmlUi::AddName(const char* name, CXmlLayout* layout)
{
	if (name && layout)
	{
		if ( strncmp(name, g_xml_key_parent, strlen(g_xml_key_parent)) != 0 )
		{
			pair<map<string, CXmlLayout*>::iterator, bool> hr;
			hr = m_nameIndex.insert(make_pair(name, layout));

			// 名稱重複
			// assert(hr.second);
			
			if ( !hr.second )
			{
				PushErrorMsg(" %s名稱重複", name);
			}
		}
	}
}

// ----------------------------------------------------------------------------
CXmlLayout* CXmlUi::FindByName(const char* name)
{
	map<string, CXmlLayout*>::iterator iter = m_nameIndex.find(name);
	if (iter != m_nameIndex.end())
		return iter->second;
	return NULL;
}

// ----------------------------------------------------------------------------
CXmlLayout* CXmlUi::CreateXmlLayers(const wchar_t* typeName, CXmlLayout* parent, const char* inherit, const char* name)
{
	CXmlLayout* pXmlLayout = NULL;
	if ( XMLString::compareString(typeName, L"Texture") == 0 )
		pXmlLayout = new CXmlTexture(this, parent);
	else if ( XMLString::compareString(typeName, L"FontString") == 0 )
		pXmlLayout = new CXmlFontString(this, parent);

	if ( pXmlLayout )
	{
		if ( name )
			pXmlLayout->m_name = name;

		if ( inherit )
			pXmlLayout->Inherit(FindByName(inherit));
	}	
	return pXmlLayout;
}

// ----------------------------------------------------------------------------
CXmlFrame* CXmlUi::CreateXmlFrames(const wchar_t* typeName, CXmlLayout* parent, const char* inherit, const char* name)
{
	CXmlFrame* pXmlFrame = NULL;
	if ( XMLString::compareString(typeName, L"Frame") == 0 )
		pXmlFrame = new CXmlFrame(this, parent);
	else if ( XMLString::compareString(typeName, L"Model") == 0 )
		pXmlFrame = new CXmlModel(this, parent);
	else if ( XMLString::compareString(typeName, L"Button") == 0 )
		pXmlFrame = new CXmlButton(this, parent);
	else if ( XMLString::compareString(typeName, L"CheckButton") == 0 )
		pXmlFrame = new CXmlCheckButton(this, parent);
	else if ( XMLString::compareString(typeName, L"RadioButton") == 0 )
		pXmlFrame = new CXmlRadioButton(this, parent);
	else if ( XMLString::compareString(typeName, L"EditBox") == 0 )
		pXmlFrame = new CXmlEditBox(this, parent);
	else if ( XMLString::compareString(typeName, L"Slider") == 0 )
		pXmlFrame = new CXmlSlider(this, parent);
	else if ( XMLString::compareString(typeName, L"StatusBar") == 0 )
		pXmlFrame = new CXmlStatusBar(this, parent);
	else if ( XMLString::compareString(typeName, L"MessageFrame") == 0 )
		pXmlFrame = new CXmlMessageFrame(this, parent);
	else if ( XMLString::compareString(typeName, L"ListBox") == 0 )
		pXmlFrame = new CXmlListBox(this, parent);
	else if ( XMLString::compareString(typeName, L"ScrollFrame") == 0 )
		pXmlFrame = new CXmlScrollFrame(this, parent);
	else if ( XMLString::compareString(typeName, L"GameTooltip") == 0 )
		pXmlFrame = new CXmlTooltip(this, parent);

	if ( pXmlFrame )
	{
		if ( name )
			pXmlFrame->m_name = name;

		if ( inherit )
			pXmlFrame->Inherit(FindByName(inherit));
	}

	return pXmlFrame;
}

// ----------------------------------------------------------------------------
void CXmlUi::PushErrorMsg(const char* format, ...)
{
	string errorStr;
	char buf[4096];

	if ( format == NULL )
		return;
	
	va_list args;
	va_start( args, format );
	vsprintf( buf, format, args );
	va_end( args);

	if ( m_currentLoadFile ) {
		errorStr = m_currentLoadFile->m_filename;
	}
	errorStr += buf;
	m_errorList.push_back(errorStr);
}

// ----------------------------------------------------------------------------
CXmlNode* CXmlUi::GetFirstXmlNode()
{
	m_lastNode = 0;
	return GetNextXmlNode();
}

// ----------------------------------------------------------------------------
CXmlNode* CXmlUi::GetNextXmlNode()
{
	if ( m_nodes.size() > m_lastNode )
		return m_nodes[m_lastNode++];
	return NULL;
}

/*
// ----------------------------------------------------------------------------
bool CXmlUi::LoadFile(const string& pathname, const string& filename)
{
	bool resultBool = true;
	XercesDOMParser* parser = new XercesDOMParser();
	parser->setDoSchema(true);						// optional
	//parser->setDoNamespaces(true);					// optional	
	parser->setValidationScheme(AbstractDOMParser::Val_Always);

	ErrorHandler* errHandler = (ErrorHandler*) new HandlerBase();
	parser->setErrorHandler(errHandler);
	
	try {		
		parser->parse(filename.c_str());
	}
	catch (const XMLException& toCatch) {
		char* message = XMLString::transcode(toCatch.getMessage());
		printf("Exception message is: \n%s\n", message);
		XMLString::release(&message);
		resultBool = false;
	}
	catch (const DOMException& toCatch) {
		char* message = XMLString::transcode(toCatch.msg);
		printf("Exception message is: \n%s\n", message);
		XMLString::release(&message);
		resultBool = false;
	}
	catch (...) {
		printf("[%s]Unexpected Exception \n", filename.c_str());
		resultBool = false;
	}

	if ( resultBool )
	{		
		DOMDocument* document = parser->getDocument();
		DOMElement* element = document->getDocumentElement();
		DOMNode* node = element->getFirstChild();
		while ( node )
		{
			if ( node->getNodeType() == DOMNode::ELEMENT_NODE )
			{
				CXmlLayout* pXmlLayout;
				char* nodeName;

				pXmlLayout = NULL;
				nodeName = XMLString::transcode(node->getNodeName());
				if ( XMLString::compareString(nodeName, "Script") == 0 )
				{
					CXmlScript* pXmlScript = new CXmlScript();
					if ( pXmlScript->LoadFromDOMNode(node) )
					{
						pXmlScript->m_file = pathname + pXmlScript->m_file;
						m_scripts.push_back(pXmlScript);
					}
					else
						XML_SAFE_DLETE(pXmlScript);
				}
				else if ( XMLString::compareString(nodeName, "FontString") == 0 )
				{
					pXmlLayout = new CXmlFontString(this);
				}
				else if ( XMLString::compareString(nodeName, "Texture") == 0 )
				{
					pXmlLayout = new CXmlTexture(this);
				}
				else if ( XMLString::compareString(nodeName, "Frame") == 0 )
				{
					pXmlLayout = new CXmlFrame(this);
				}
				else if ( XMLString::compareString(nodeName, "Model") == 0 )
				{
					pXmlLayout = new CXmlModel(this);
				}
				else if ( XMLString::compareString(nodeName, "Button") == 0 )
				{
					pXmlLayout = new CXmlButton(this);
				}
				else if ( XMLString::compareString(nodeName, "CheckButton") == 0 )
				{
					pXmlLayout = new CXmlCheckButton(this);
				}
				else if ( XMLString::compareString(nodeName, "RadioButton") == 0 )
				{
					pXmlLayout = new CXmlRadioButton(this);
				}
				else if ( XMLString::compareString(nodeName, "Slider") == 0 )
				{
					pXmlLayout = new CXmlSlider(this);
				}
				else if ( XMLString::compareString(nodeName, "StatusBar") == 0 )
				{
					pXmlLayout = new CXmlStatusBar(this);
				}
				else if ( XMLString::compareString(nodeName, "EditBox") == 0 )
				{
					pXmlLayout = new CXmlEditBox(this);
				}
				else if ( XMLString::compareString(nodeName, "ListBox") == 0 )
				{
					pXmlLayout = new CXmlListBox(this);
				}
				else if ( XMLString::compareString(nodeName, "MessageFrame") == 0 )
				{
					pXmlLayout = new CXmlMessageFrame(this);
				}
				else if ( XMLString::compareString(nodeName, "ScrollFrame") == 0 )
				{
					pXmlLayout = new CXmlScrollFrame(this);
				}
				XMLString::release(&nodeName);

				if ( pXmlLayout )
				{
					if ( pXmlLayout->LoadFromDOMNode(node) )
						m_layouts.push_back(pXmlLayout);
					else
						XML_SAFE_DLETE(pXmlLayout);
				}
			}
			node = node->getNextSibling();
		}
	}

	delete parser;
	delete errHandler;

	return resultBool;
}
*/

// ----------------------------------------------------------------------------
bool CXmlUi::ReadLine(FILE *fptr, string& lineStr)
{
	static char tmpStr[1024];
	if ( fgets( tmpStr, 1024, fptr ) )
	{
		char *begin = tmpStr;
		char *end = tmpStr + strlen(tmpStr) - 1;

		while ( *begin && (*begin == '\t' || *begin == ' ') ) begin++;
		while ( end > begin && (*end == '\t' || *end == ' ' || *end == '\n' ) ) end--;

		tmpStr[end - begin + 1] = 0;
		lineStr = tmpStr;
		return true;
	}
	return false;
}

// ----------------------------------------------------------------------------
bool CXmlUi::CheckTocFile(const string& filename)
{
	string ext(GetFileExt(filename));
	_strlwr( (char*)ext.c_str() );
	if ( ext == "toc" )
		return true;
	return false;
}

// ----------------------------------------------------------------------------
bool CXmlUi::CheckXmlFile(const string& filename)
{
	string ext(GetFileExt(filename));
	_strlwr( (char*)ext.c_str() );
	if ( ext == "xml" )
		return true;
	return false;
}

// ----------------------------------------------------------------------------
bool CXmlUi::CheckLuaFile(const string& filename)
{
	string ext(GetFileExt(filename));
	_strlwr( (char*)ext.c_str() );
	if ( ext == "lua" )
		return true;
	return false;
}

// ----------------------------------------------------------------------------
string CXmlUi::GetFileExt(const string& filename)
{
	int len = (int)filename.length();
	int i;
	for (i = len-1; i >= 0; i--)
	{
		if (filename[i] == '.')
		{
			return filename.substr(i+1, len-i-1);
			//return ext;
		}
	}
	return string("");
}

// ----------------------------------------------------------------------------
string CXmlUi::GetPathName(const string& filename)
{
	int len = (int)filename.length();
	int i;
	for (i = len-1; i >= 0; i--)
	{
		if (filename[i] == '\\')
		{
			return filename.substr(0, i+1);
		}
	}
	return string("");
}