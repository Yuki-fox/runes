#pragma once

#include <map>
#include <string>

#define DEBUG_OUTPUT_MSG_FRAME			"DebugOutputMessageFrame"

//////////////////////////////////////////////////////////////////////////
// Globals
class	CErrorList;
class	CLogList;

extern	CErrorList					*g_pError;
extern	CLogList					*g_pLogList;
//////////////////////////////////////////////////////////////////////////

struct ErrorItem {
	std::string		str;
	int				code;
};

class CErrorList
{
public:
	CErrorList(HINSTANCE hInstance, HWND hWnd);
	virtual ~CErrorList();

	static	void		Initialize(HINSTANCE hInstance, HWND hParent);
	static	void		Shutdown();
	void				AddMessage( int type, int code, const char* format, ... );
	void				OutputToFile( const char* format, ... )						{}
	void				SetSize(int x, int y);
	void				SetPos(int x, int y);
	void				Show();
	void				Hide();
	static	HWND		GetHandle() { if( g_pError ) return g_pError->m_hWnd; else return NULL; };


protected:
	std::multimap< int, ErrorItem > m_errorData;
	HWND				m_hWnd;
	HWND				m_hTreeView;
	HWND				m_hListView;
	HWND				m_hList;
	HFONT				m_hFont;	
};

class CLogList
{
public:
	CLogList();
	static	void		Initialize(const char* filename);
	static	void		Shutdown();
	void				AddMessage(const char* format, ... );
	void				OpenFile(const char* filename);
	void				CloseFile();

protected:
	HANDLE				m_hFile;
};