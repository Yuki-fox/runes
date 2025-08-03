/////////////////////////////////////////////////////////////////////////////////////
//
//

#include "StdAfx.h"




CWindowHost::CWindowHost() :
m_hWnd(NULL), 
m_OldWndProc(::DefWindowProc), 
m_bSubclassed(false),m_nHeight(0),m_nWidth(0),m_bDrag(0)

{
}


HWND CWindowHost::GetHWND() const 
{ 
	return m_hWnd; 
}

void CWindowHost::KillTimer(int id)
{
	::KillTimer(m_hWnd, id); 
}
void CWindowHost::SetTimer(int id,int time)
{
	::SetTimer(m_hWnd, id, time, NULL); 
}
UINT CWindowHost::GetClassStyle() 
{
	return 0;
}

LPCTSTR CWindowHost::GetSuperClassName() const
{
	return NULL;
}

CWindowHost::operator HWND() const
{
	return m_hWnd;
}

HWND CWindowHost::Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, const RECT rc, HMENU hMenu)
{
	return Create(hwndParent, pstrName, dwStyle, dwExStyle, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, hMenu);
}

HWND CWindowHost::Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, int x, int y, int cx, int cy, HMENU hMenu)
{
	m_nWidth=cx>0? cx:0;
	m_nHeight=cy>0? cy:0;
	LPCTSTR superClassName=GetSuperClassName();
	bool  bRegisterOk;

	if (superClassName)
		bRegisterOk=RegisterSuperclass();
	else
		bRegisterOk=RegisterWindowClass();

	if( !bRegisterOk) 
		return NULL;	

	m_hWnd = ::CreateWindowEx(
		dwExStyle, 
		GetWindowClassName(), 
		pstrName,
		dwStyle,
		x, y, cx,cy, 
		hwndParent, 
		hMenu, 
		CPaintManagerUI::GetResourceInstance(), 
		this);
	ASSERT(m_hWnd!=NULL);
	return m_hWnd;
}

HWND CWindowHost::Subclass(HWND hWnd)
{
	ASSERT(::IsWindow(hWnd));
	ASSERT(m_hWnd==NULL);
	m_OldWndProc =    ((WNDPROC)SetWindowLongPtr((hWnd), GWLP_WNDPROC, (LPARAM)(WNDPROC)(__WndProc)));
		//SubclassWindow(hWnd, __WndProc);
	if( m_OldWndProc == NULL ) return NULL;
	m_bSubclassed = true;
	m_hWnd = hWnd;
	return m_hWnd;
}

void CWindowHost::Unsubclass()
{
	ASSERT(::IsWindow(m_hWnd));
	if( !::IsWindow(m_hWnd) )
		return;
	if( !m_bSubclassed ) 
		return;
	 SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LPARAM)(WNDPROC)(m_OldWndProc));
	//SubclassWindow(m_hWnd, m_OldWndProc);
	m_OldWndProc = ::DefWindowProc;
	m_bSubclassed = false;
}

void CWindowHost::ShowWindow(bool bShow /*= true*/, bool bTakeFocus /*= false*/)
{
	ASSERT(::IsWindow(m_hWnd));
	if( !::IsWindow(m_hWnd) ) return;
	::ShowWindow(m_hWnd, bShow ? (bTakeFocus ? SW_SHOWNORMAL : SW_SHOWNOACTIVATE) : SW_HIDE);
}
void CWindowHost::ShowMiniWindow(bool bShow /*= true*/, bool bTakeFocus /*= false*/)
{
	ASSERT(::IsWindow(m_hWnd));
	if( !::IsWindow(m_hWnd) ) return;
	::ShowWindow(m_hWnd,SW_MINIMIZE);
}



bool CWindowHost::ShowModal()
{
	ASSERT(::IsWindow(m_hWnd));
	HWND hWndParent = GetWindowOwner(m_hWnd);
	::ShowWindow(m_hWnd, SW_SHOWNORMAL);
	::EnableWindow(hWndParent, FALSE);
	MSG msg = { 0 };
	while( ::IsWindow(m_hWnd) && ::GetMessage(&msg, NULL, 0, 0) ) 
	{
		if( msg.message == WM_CLOSE )
		{
			::EnableWindow(hWndParent, TRUE);
			::SetFocus(hWndParent);
		}
		if( !CPaintManagerUI::TranslateMessage(&msg) ) 
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		if( msg.message == WM_QUIT )
			break;
	}
	::EnableWindow(hWndParent, TRUE);
	::SetFocus(hWndParent);
	if( msg.message == WM_QUIT ) 
		::PostQuitMessage(msg.wParam);
	return true;
}

void CWindowHost::Close()
{
	ASSERT(::IsWindow(m_hWnd));
	if( !::IsWindow(m_hWnd) )
		return;
	PostMessage(WM_CLOSE);
}
void  CWindowHost::EndDialog(int ret)
{

	::DestroyWindow(m_hWnd);
} 
void CWindowHost::SetWindowPos(short x,short y,bool draw)
{

	INT flag=SWP_NOSIZE;
	//if (!draw)
	//	flag|=SWP_NOREDRAW;

	RECT rc;
	GetWindowRect(m_hWnd,&rc);
	

	int sx=rc.left+x;
	int sy=rc.top+y;
	if (x>-300 || y>-300)



	::SetWindowPos(m_hWnd,NULL,sx,sy,0,0,flag);
	
	
	m_bDrag=draw;
}

void CWindowHost::CenterWindow()
{
	ASSERT(::IsWindow(m_hWnd));
	ASSERT((GetWindowStyle(m_hWnd)&WS_CHILD)==0);
	RECT rcDlg = { 0 };
	::GetWindowRect(m_hWnd, &rcDlg);
	RECT rcArea = { 0 };
	RECT rcCenter = { 0 };
	HWND hWndParent = ::GetParent(m_hWnd);
	HWND hWndCenter = ::GetWindowOwner(m_hWnd);
	::SystemParametersInfo(SPI_GETWORKAREA, NULL, &rcArea, NULL);
	if( hWndCenter == NULL )
		rcCenter = rcArea; 
	else 
		::GetWindowRect(hWndCenter, &rcCenter);

	int DlgWidth = rcDlg.right - rcDlg.left;
	int DlgHeight = rcDlg.bottom - rcDlg.top;

	// Find dialog's upper left based on rcCenter
	int xLeft = (rcCenter.left + rcCenter.right) / 2 - DlgWidth / 2;
	int yTop = (rcCenter.top + rcCenter.bottom) / 2 - DlgHeight / 2;

	// The dialog is outside the screen, move it inside
	if( xLeft < rcArea.left )
		xLeft = rcArea.left;
	else if( xLeft + DlgWidth > rcArea.right ) 
		xLeft = rcArea.right - DlgWidth;
	if( yTop < rcArea.top ) 
		yTop = rcArea.top;
	else if( yTop + DlgHeight > rcArea.bottom ) 
		yTop = rcArea.bottom - DlgHeight;
	::SetWindowPos(m_hWnd, NULL, xLeft, yTop, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void CWindowHost::SetIcon(UINT nRes)
{
	HICON hIcon = (HICON)::LoadImage(CPaintManagerUI::GetResourceInstance(), MAKEINTRESOURCE(nRes), IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	ASSERT(hIcon);
	::SendMessage(m_hWnd, WM_SETICON, (WPARAM) TRUE, (LPARAM) hIcon);
	hIcon = (HICON)::LoadImage(CPaintManagerUI::GetResourceInstance(), MAKEINTRESOURCE(nRes), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	ASSERT(hIcon);
	::SendMessage(m_hWnd, WM_SETICON, (WPARAM) FALSE, (LPARAM) hIcon);
}

bool CWindowHost::RegisterWindowClass(void)
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = GetClassStyle();
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.lpfnWndProc = CWindowHost::__WndProc;
	wc.hInstance = CPaintManagerUI::GetResourceInstance();
	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;//(HBRUSH) GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = GetWindowClassName();
	ATOM ret = ::RegisterClassEx(&wc);
	ASSERT(ret!=NULL || ::GetLastError()==ERROR_CLASS_ALREADY_EXISTS);
	return ret != NULL || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS;
}

bool CWindowHost::RegisterSuperclass()
{
	// Get the class information from an existing
	// window so we can subclass it later on...
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	if( !::GetClassInfoEx(NULL, GetSuperClassName(), &wc) ) 
	{
		if( !::GetClassInfoEx(CPaintManagerUI::GetResourceInstance(), GetSuperClassName(), &wc) ) 
		{
			ASSERT(!"Unable to locate window class");
			return NULL;
		}
	}
	m_OldWndProc = wc.lpfnWndProc;
	wc.lpfnWndProc = CWindowHost::__ControlProc;
	wc.hInstance = CPaintManagerUI::GetResourceInstance();
	wc.lpszClassName = GetWindowClassName();
	ATOM ret = ::RegisterClassEx(&wc);
	ASSERT(ret!=NULL || ::GetLastError()==ERROR_CLASS_ALREADY_EXISTS);
	return ret != NULL || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS;
}

LRESULT CALLBACK CWindowHost::__WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CWindowHost* pThis = NULL;
	if( uMsg == WM_NCCREATE ) 
	{
		LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
		pThis = static_cast<CWindowHost*>(lpcs->lpCreateParams);
		pThis->m_hWnd = hWnd;
		::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LPARAM>(pThis));
	} 
	else 
	{
		pThis = reinterpret_cast<CWindowHost*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
		if( uMsg == WM_NCDESTROY && pThis != NULL ) 
		{
			LRESULT lRes = ::CallWindowProc(pThis->m_OldWndProc, hWnd, uMsg, wParam, lParam);
			::SetWindowLongPtr(pThis->m_hWnd, GWLP_USERDATA, 0L);
			if( pThis->m_bSubclassed ) 
				pThis->Unsubclass();
			pThis->m_hWnd = NULL;
			pThis->OnFinalMessage(hWnd);
			return lRes;
		}
	}
	if( pThis != NULL ) 
	{
		return pThis->HandleMessage(uMsg, wParam, lParam);
	} 
	else 
	{
		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

LRESULT CALLBACK CWindowHost::__ControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CWindowHost* pThis = NULL;
	if( uMsg == WM_NCCREATE ) 
	{
		LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
		pThis = static_cast<CWindowHost*>(lpcs->lpCreateParams);
		::SetProp(hWnd, _T("WndX"), (HANDLE) pThis);
		pThis->m_hWnd = hWnd;
	} 
	else 
	{
		pThis = reinterpret_cast<CWindowHost*>(::GetProp(hWnd, _T("WndX") ));
		if( uMsg == WM_NCDESTROY && pThis != NULL )
		{
			LRESULT lRes = ::CallWindowProc(pThis->m_OldWndProc, hWnd, uMsg, wParam, lParam);
			if( pThis->m_bSubclassed ) 
				pThis->Unsubclass();
			::SetProp(hWnd, _T("WndX"), NULL);
			pThis->m_hWnd = NULL;
			pThis->OnFinalMessage(hWnd);
			return lRes;
		}
	}
	if( pThis != NULL ) 
	{
		return pThis->HandleMessage(uMsg, wParam, lParam);
	} 
	else 
	{
		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

LRESULT CWindowHost::SendMessage(UINT uMsg, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/)
{
	ASSERT(::IsWindow(m_hWnd));
	return ::SendMessage(m_hWnd, uMsg, wParam, lParam);
} 

LRESULT CWindowHost::PostMessage(UINT uMsg, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/)
{
	ASSERT(::IsWindow(m_hWnd));
	return ::PostMessage(m_hWnd, uMsg, wParam, lParam);
}

void CWindowHost::ResizeClient(int cx /*= -1*/, int cy /*= -1*/)
{
	ASSERT(::IsWindow(m_hWnd));
	RECT rc = { 0 };
	if( !::GetClientRect(m_hWnd, &rc))
		return;
	if( cx != -1 )
		rc.right = cx;
	if( cy != -1 )
		rc.bottom = cy;
	if( !::AdjustWindowRectEx(&rc, GetWindowStyle(m_hWnd), (!(GetWindowStyle(m_hWnd) & WS_CHILD) && (::GetMenu(m_hWnd) != NULL)), GetWindowExStyle(m_hWnd)) ) 
		return;
	UINT uFlags = SWP_NOZORDER | SWP_NOMOVE;
	::SetWindowPos(m_hWnd, NULL, 0, 0, rc.right - rc.left, rc.bottom - rc.top, uFlags);
}

LRESULT CWindowHost::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	
	return ::CallWindowProc(m_OldWndProc, m_hWnd, uMsg, wParam, lParam);
}

void CWindowHost::OnFinalMessage(HWND /*hWnd*/)
{
}