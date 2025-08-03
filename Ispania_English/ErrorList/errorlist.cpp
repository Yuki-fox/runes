#include <windows.h>
#include <commctrl.h>
#include "../mainproc/GameMain.h"
#include "errorlist.h"

using namespace std;

// Globals
CErrorList*							g_pError		= NULL;
CLogList*							g_pLogList		= NULL;
const char*							g_ErrorWndName	= "ErrorWnd";

LRESULT CALLBACK	ErrorWndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

// ----------------------------------------------------------------------------------
void CErrorList::Initialize(HINSTANCE hInstance, HWND hParent)
{
	if ( g_pError == NULL )
	{
		g_pError = new CErrorList(hInstance, hParent);
		g_pError->Hide();				
	}
}

// ----------------------------------------------------------------------------------
void CErrorList::Shutdown()
{
	if ( g_pError )
	{
		delete g_pError;
		g_pError = NULL;
	}
}

// ----------------------------------------------------------------------------------
CErrorList::CErrorList(HINSTANCE hInstance, HWND hWnd)
{
	m_errorData.clear();
	m_hWnd = NULL;
	m_hList = NULL;

	WNDCLASSEX WndClass;		

	InitCommonControls();

	// Register frame window
	WndClass.cbSize = sizeof(WNDCLASSEX);
	WndClass.style = 0;
	WndClass.lpfnWndProc = ErrorWndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = NULL;
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = g_ErrorWndName;
	WndClass.hIconSm = NULL;
	RegisterClassEx(&WndClass);

	m_hFont = CreateFont(-16,
						 0,
                         0,
						 0,
						 FW_BOLD,
						 false,
						 false,
						 false,
						 DEFAULT_CHARSET,
						 OUT_DEFAULT_PRECIS,
						 CLIP_DEFAULT_PRECIS,
						 DEFAULT_QUALITY,
						 DEFAULT_PITCH | FF_DONTCARE,
						 "·s²Ó©úÅé" );

	if ( (m_hWnd = CreateWindowEx(WS_EX_TOOLWINDOW | WS_EX_DLGMODALFRAME,
								g_ErrorWndName,
								TEXT("Error List"),
#ifdef KALYDO
								// for Kalydo don't show error frame.
								WS_CAPTION | WS_SYSMENU | WS_THICKFRAME,
#else
								WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_VISIBLE,
#endif // KALYDO
								CW_USEDEFAULT,
								CW_USEDEFAULT,
								CW_USEDEFAULT,
								CW_USEDEFAULT,
								hWnd,
								NULL,
								hInstance,
								NULL)) != NULL )
	{
		RECT rect;
		GetClientRect( m_hWnd, &rect );
		
		m_hTreeView = CreateWindowEx( WS_EX_CLIENTEDGE,
								  WC_TREEVIEW,
                                  (LPCTSTR) NULL,
                                  WS_CHILD | WS_VISIBLE,
                                  0,
                                  0,
                                  (rect.right - rect.left)/2,
                                  (rect.bottom - rect.top)/2,
                                  m_hWnd,
								  (HMENU)NULL,
								  hInstance,
								  NULL );

		m_hListView = CreateWindowEx( WS_EX_CLIENTEDGE,
									WC_LISTVIEW,
									(LPCTSTR) NULL,
									WS_CHILD | WS_VISIBLE,
									(rect.right - rect.left)/2,
									0,
									(rect.right - rect.left)/2,
									(rect.bottom - rect.top)/2,
									m_hWnd,
									(HMENU)NULL,
									hInstance,
									NULL );

		m_hList = CreateWindowEx(WS_EX_CLIENTEDGE,
									"listbox",
									(LPCTSTR) NULL,
									WS_CHILD | WS_VISIBLE | WS_VSCROLL,
									0,
									(rect.bottom - rect.top)/2,
									rect.right - rect.left,
									(rect.bottom - rect.top)/2,
									m_hWnd,
									(HMENU)NULL,
									hInstance,
									NULL );
		
		SendMessage( m_hList, WM_SETFONT, (WPARAM)m_hFont, FALSE );
	}
}

// ----------------------------------------------------------------------------------
CErrorList::~CErrorList()
{
	if (m_hFont)
	{
		DeleteObject(m_hFont);
		m_hFont = NULL;
	}
}

// ----------------------------------------------------------------------------------
void CErrorList::AddMessage( int type, int code, const char* format, ... )
{
	static char buf[4096];
	int color;
	bool bDebugList;
	bDebugList=g_pGameMain->IsShowServerDebugMsg();

#ifdef _DEBUG
	bDebugList=true;
#endif
	if ( format == NULL || !bDebugList)
		return;

	va_list args;
	va_start( args, format );
	vsprintf( buf, format, args );
	va_end( args);
	
	//m_errorData.insert( make_pair(type, item) );

	switch ( type )
	{
	case 0:
		color = 0xFFFF1010;
		break;

	case 1:
		color = 0xFFFFFF20;
		break;

	case 2:
		color = 0xFF10FF08;
		break;

	case 3:
		color = 0xFF1010F0;
		break;

	case 4:
		color = 0xFF10EFF0;
		break;

	case 5:
		color = 0xFFDF20FE;
		break;

	default:
		color = 0xFFEFF0F1;
		break;
	}

	{
		char tmp[MAX_PATH];
		SYSTEMTIME time;
		GetLocalTime(&time);
		sprintf(tmp, "[%2d:%2d:%2d] ", time.wHour, time.wMinute, time.wSecond);

		ErrorItem item;
		item.code = code;
		item.str = tmp;
		item.str += buf;
		SendMessage(m_hList, LB_INSERTSTRING, 0, (LPARAM)item.str.c_str());				
	}
}

// ----------------------------------------------------------------------------------
void CErrorList::SetPos(int x, int y)
{
	SetWindowPos( m_hWnd, HWND_NOTOPMOST, x, y, 0, 0, SWP_NOSIZE );
}
// ----------------------------------------------------------------------------------
void CErrorList::SetSize(int x, int y)
{
	SetWindowPos(m_hTreeView, 0, 0, 0, x/2, y/2, SWP_NOACTIVATE);
	SetWindowPos(m_hListView, 0, x/2, 0, x/2, y/2, SWP_NOACTIVATE);
	SetWindowPos(m_hList, 0, 0, y/2, x, y/2, SWP_NOACTIVATE);
}

// ----------------------------------------------------------------------------------
void CErrorList::Show()
{
	ShowWindow(m_hWnd, SW_SHOW);
}

// ----------------------------------------------------------------------------------
void CErrorList::Hide()
{
	ShowWindow(m_hWnd, SW_HIDE);
}

// ----------------------------------------------------------------------------------
LRESULT CALLBACK	ErrorWndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	RECT rect;	

	switch (iMsg)
	{
	case WM_SIZE:
		// Update window properties
		GetClientRect( hWnd, &rect );
		if (g_pError)
			g_pError->SetSize(rect.right - rect.left, rect.bottom - rect.top);
		break;

	case WM_CLOSE:
		g_pError->Hide();
		return TRUE;
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

// ----------------------------------------------------------------------------------
CLogList::CLogList()
{
	m_hFile = INVALID_HANDLE_VALUE;
}

// ----------------------------------------------------------------------------------
void CLogList::Initialize(const char* filename)
{
	if ( g_pLogList == NULL )
		g_pLogList = new CLogList();

	g_pLogList->OpenFile(filename);
}

// ----------------------------------------------------------------------------------
void CLogList::Shutdown()
{
	if ( g_pLogList )
		g_pLogList->CloseFile();
}

// ----------------------------------------------------------------------------------
void CLogList::AddMessage(const char* format, ... )
{
	static char buf[4096];

	if ( format == NULL )
		return;	

	va_list args;
	va_start( args, format );
	vsprintf( buf, format, args );
	va_end( args);

	if ( m_hFile != INVALID_HANDLE_VALUE )
	{
		char tmp[MAX_PATH];
		SYSTEMTIME time;
		GetLocalTime(&time);
		sprintf(tmp, "[%2d:%2d:%2d] ", time.wHour, time.wMinute, time.wSecond);

		DWORD nWritten;	
		WriteFile( m_hFile, tmp, (DWORD)strlen(tmp), &nWritten, NULL ) ;
		WriteFile( m_hFile, buf, (DWORD)strlen(buf), &nWritten, NULL ) ;
		WriteFile( m_hFile, "\r\n", 2, &nWritten, NULL ) ;
		// FlushFileBuffers( m_hFile ) ;
	}
}

// ----------------------------------------------------------------------------------
void CLogList::OpenFile(const char* filename)
{
	m_hFile = ::CreateFile( filename, GENERIC_READ | GENERIC_WRITE , FILE_SHARE_READ , NULL , CREATE_ALWAYS , FILE_ATTRIBUTE_NORMAL , NULL );
}

// ----------------------------------------------------------------------------------
void CLogList::CloseFile()
{
	if ( m_hFile != INVALID_HANDLE_VALUE )
	{
		::CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
	}
}