#include "StdAfx.h"

#include <sqplus.h>



SQFrameWnd::SQFrameWnd(SquirrelObject &so):SQWindowHost(so), m_hWndClient(NULL) 
{

}
SQFrameWnd::~SQFrameWnd()
{

	


}

void SQFrameWnd::SetPage(SQPageWnd* pclient)
{
	if( pclient != NULL ) 
	{
		if( m_hWndClient != NULL )
			::PostMessage(m_hWndClient, WM_CLOSE, 0, 0L);
		pclient->m_pm.AddNotifier(this);
		m_hWndClient = pclient->Create(m_hWnd, NULL, UI_WNDSTYLE_CHILD, 0);

		Relayout(pclient);

	}

}
void SQFrameWnd::Relayout(SQPageWnd* pclient)
{

	
	if (pclient)
	{
		
		ResizeClient(pclient->m_nWidth,pclient->m_nHeight);
	}
	
//	if (pclient)
	
	PostMessage(WM_SIZE);
}

LRESULT SQFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch (uMsg)
	{

	case WM_DESTROY:
		::PostQuitMessage(0L);
		break;
	case WM_SETFOCUS:
		::SetFocus(m_hWndClient);
		break;
	case WM_SIZE:
		{
			RECT rcClient;
			::GetClientRect(m_hWnd, &rcClient);
			::MoveWindow(m_hWndClient, rcClient.left, rcClient.top, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, TRUE);
			return 0;
		}
	case WM_PAINT:
		{
			PAINTSTRUCT ps = { 0 };
			::BeginPaint(m_hWnd, &ps);
			::EndPaint(m_hWnd, &ps);

			return 0;
		}
	}



	return SQHandleMessage(uMsg,wParam,lParam);


}