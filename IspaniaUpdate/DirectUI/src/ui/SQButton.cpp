#include "StdAfx.h"
#include <UI\SQButton.h>
#include "DirectUI.h"
SQButtonUI::SQButtonUI(SquirrelObject&so):CButtonUI(so),m_cyHeight(10)
{
	int i;
	for (i=0;i<BS_COUNT;i++)
	{
		m_image[i]=NULL;
		

	}
}
 SQButtonUI::~SQButtonUI()
{
	int i;
	for (i=0;i<BS_COUNT;i++)
	{
		if (m_image[i])
			::DeleteObject(m_image[i]);

	}


}
SIZE SQButtonUI::EstimateSize(SIZE /*szAvailable*/)
{

	return CSize(m_cxWidth,m_cyHeight);
}
LPCTSTR SQButtonUI::GetClass() const
{
	return _T("SQButton");
}
void SQButtonUI::SetImage(int state,LPCTSTR pstrBitmap)
{
	if (m_image[state])
		::DeleteObject(m_image[state]);

	m_image[state]=(HBITMAP) DirectUI::LoadImage(pstrBitmap,0,0,0);


}
void SQButtonUI::DoPaint(HDC hDC, const RECT& rcPaint)
{
 

	// Draw button
	UINT uState = 0;
	if( IsFocused() ) 
		uState |= UISTATE_FOCUSED;
	if( !IsEnabled() ) 
		uState |= UISTATE_DISABLED;

	CBlueRenderEngineUI::DoPaintButton2(hDC, m_pManager, m_rcItem, m_image,  m_uButtonState | uState);

	

}

void SQButtonUI::SetHeight(int height)
{
	m_cyHeight=height;
}

void SQButtonUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( _tcscmp(pstrName, _T("normal")) == 0 )
		SetImage(BS_NORMAL,pstrValue);
	else if( _tcscmp(pstrName, _T("click")) == 0 )
		SetImage(BS_PUSHED,pstrValue);
	else if( _tcscmp(pstrName, _T("disable")) == 0 )
		SetImage(BS_DISABLE,pstrValue);
	else if( _tcscmp(pstrName, _T("hover")) == 0 )
		SetImage(BS_HOVER,pstrValue);
	else
	CButtonUI::SetAttribute(pstrName,pstrValue);
	

}