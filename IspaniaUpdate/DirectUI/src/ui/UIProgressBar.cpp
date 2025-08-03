#include "StdAfx.h"

#include "DirectUI.h"

CProgressBarUI::CProgressBarUI(SquirrelObject&so):CControlUI(so), m_maxValue(100),m_Value(0),m_image(NULL)
{
	m_cxyFixed.cx=120;
	m_cxyFixed.cy=40;

}
CProgressBarUI::~CProgressBarUI()
{
	if (m_image)
		::DeleteObject(m_image);
}
void  CProgressBarUI::SetWidth(int cxWidth)
{
	m_cxyFixed.cx=cxWidth;

}
void  CProgressBarUI::SetHeight(int cyHeight)
{
	m_cxyFixed.cy=cyHeight;

}


void CProgressBarUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( _tcscmp(pstrName, _T("width")) == 0 ) 
		SetWidth(_ttoi(pstrValue));
	else if( _tcscmp(pstrName, _T("height")) == 0 )
		SetHeight(_ttoi(pstrValue));

	else if (_tcscmp(pstrName, _T("image")) == 0)
		SetImage(pstrValue);

		

	else 
		CControlUI::SetAttribute(pstrName, pstrValue);
}
int CProgressBarUI::GetValue(void)
{

	return m_Value;

}
void CProgressBarUI::SetValue(int Value)
{

	m_Value=Value;


	if (m_Value>m_maxValue)
		m_Value=	m_maxValue;

	//if( m_pManager != NULL ) 
	//	m_pManager->SendNotify(this, _T("changed"));

	Invalidate();
}
void CProgressBarUI::SetMax(int MaxValue)
{
	
	m_maxValue=MaxValue;

}

void CProgressBarUI::SetImage(LPCTSTR imageFile)
{


	m_image=(HBITMAP) DirectUI::LoadImage(imageFile,0,0,0);




}


void CProgressBarUI::DoPaint(HDC hDC, const RECT& rcPaint)
{
	RECT rc =
	{
		m_rcItem.left,
		m_rcItem.top, 
		m_rcItem.right,
		m_rcItem.bottom
	};


	::InflateRect(&rc, -1, -1);

	int l=rc.right-rc.left;

	int t=m_maxValue;
	float a=float(m_Value)/t;

	rc.right=rc.left+a*l;


	
	

	if(!m_image)

	CBlueRenderEngineUI::DoPaintGradient(hDC, m_pManager, rc,RGB(0,10,20), RGB(255,0,240), false, 128/a);
	else 
	{
		BITMAP BitmapInfo;
		::GetObject(m_image, sizeof(BITMAP), &BitmapInfo);
		RECT bitRc;
		bitRc.left=0;
		bitRc.top=0;
		bitRc.bottom=BitmapInfo.bmHeight;
		bitRc.right=BitmapInfo.bmWidth;

		CBlueRenderEngineUI::DoPaintBitmap(hDC, m_pManager, m_image, rc,bitRc);

	}


	
}

UINT CProgressBarUI::GetControlFlags() const
{
	return UIFLAG_TABSTOP;
}

LPCTSTR CProgressBarUI::GetClass() const 
{

	return _T("ProgressBarUI");
}
SIZE CProgressBarUI::EstimateSize(SIZE szAvailable)
{
	return m_cxyFixed;

}