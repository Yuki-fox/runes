
#include "StdAfx.h"
#include "RegionCreator.h"
#include "UI\MinG_UI.h"

#include <shlwapi.h>
#include "DirectUI.h"

SQPageWnd::SQPageWnd(SquirrelObject &so):SQWindowHost(so),m_hBitmap(NULL),m_hRegion(NULL)
{
	//m_pm=new CPaintManagerUI();

}

void SQPageWnd:: SetBackGroundRegion(bool v)
{
	if (v)
	{
		if (m_hRegion)
		{

			m_pm.SetWindowRgn(m_hRegion);
		}
	}
	else
	{

		m_pm.SetWindowRgn(NULL);
	}
}


BOOL SQPageWnd::SetBackGround(LPCTSTR szFileName,short red,short green,short blue)
{

	TCHAR temp[512];

	wsprintf(temp,_SC("skin\\%s"),szFileName);
	int len=lstrlen(temp);
	for (int i=len-1;i>0;--i)
	{
		if (temp[i]=='.')
		{
			temp[i]=0;
			break;
		}
	}
	
	lstrcat(temp,_T(".rgn"));
	CRegionCreator RegionCreator(temp);
	



	HBITMAP hBmp=DirectUI::LoadImage(szFileName,red,green,blue);


	if (!hBmp)
		return FALSE;

	if (m_hRegion)
		::DeleteObject(m_hRegion);
	
	m_hRegion=RegionCreator.CreateRegionFromBitmap(hBmp, RGB(red,green,blue),m_nWidth,m_nHeight);


	if (m_hBitmap)
	::DeleteObject(m_hBitmap);


	m_hBitmap=hBmp;

	BITMAP bm;

	GetObject(m_hBitmap, sizeof(bm), &bm);
	


	m_pm.SetMinMaxInfo(m_nWidth,m_nHeight);

	//::MoveWindow(m_hWnd, m_nxPos, m_nyPos, w, h, TRUE);
	//::SetWindowPos(m_hWnd,NULL,0,0,m_nBitmapWidth,m_nBitmapHeight,SWP_NOMOVE|SWP_NOREDRAW );
	

	m_pm.	SetBackGround(m_hBitmap,m_nWidth,m_nHeight);
	
	m_pm.UpdateLayout();
	
	return TRUE;
}


LPCTSTR SQPageWnd::GetDialogResource()
{
	LPCTSTR ret=NULL;
	LPTSTR newBuff=NULL;

	HSQUIRRELVM v=SquirrelVM::GetVMPtr();
	SquirrelObject   func   = m_Instance.GetValue(_T("GetDialogResource"));
	if (func.GetType()==OT_CLOSURE)
	{
		sq_pushobject(v,func.GetObjectHandle());
		sq_pushobject(v,m_Instance.GetObjectHandle());
		SQPLUS_CHECK_FNCALL(sq_call(v,1,SQTrue,SQ_CALL_RAISE_ERROR));

		ret=	SqPlus::GetRet(	SqPlus::TypeWrapper<LPCTSTR>(),v,-1);	

		int len=lstrlen(ret);
		newBuff=new TCHAR[len+1];
		memcpy((void *)newBuff,ret,sizeof(TCHAR)*len);
		newBuff[len]=0;
	}
	return newBuff;
}




void SQPageWnd::Init()
{
	
	ResizeClient(m_nWidth, m_nHeight);
	m_pm.SetMinMaxInfo(m_nWidth,m_nHeight);
}

LRESULT SQPageWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch (uMsg)
	{
	case WM_CREATE:
		{
			
			m_pm.Init(m_hWnd);
			LPCTSTR ScriptBuffer=GetDialogResource();

			if (ScriptBuffer)
			{
				CControlUI* pRoot = DirectUI_DialogBuilder(ScriptBuffer);
				//ASSERT(pRoot && "Failed to parse XML");
				m_pm.AttachDialog(pRoot);
				delete []ScriptBuffer;
			}
			m_pm.AddNotifier(this);
			Init();
		}
		return 0;
	}
	
	LRESULT lRes = 0;
	if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) )
		return lRes;

	return SQHandleMessage(uMsg, wParam, lParam);
}
void SQPageWnd::OnPrepareAnimation()
{
}