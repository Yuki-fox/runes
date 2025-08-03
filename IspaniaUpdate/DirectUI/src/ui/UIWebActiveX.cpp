#include "StdAfx.h"

#include <UI\UIWebActiveX.h>

CWebActiveXUI::CWebActiveXUI(SquirrelObject& so):CActiveXUI(so)
{
	CreateControl(_SC("{8856F961-340A-11D0-A96B-00C04FD705A2}"));
}


void CWebActiveXUI::Navigate(LPCTSTR URL)
{

	CComPtr<IWebBrowser2> pWBT;
	HRESULT hr = GetControl ( &pWBT );

	if ( pWBT )
	{
	//	m_webUICtrl.EnableScrollBar(FALSE); 
	//	m_webUICtrl.Enable3DBorder(FALSE);
	//	m_webUICtrl.SetWebBrowser(pWBT);
		
		CComVariant v;  // empty variant
		pWBT->Navigate ( CComBSTR(URL), 			&v, &v, &v, &v );


	
	// CAxHostWindow  
		

	}

}/*
void CWebActiveXUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( _tcscmp(pstrName, _T("web")) == 0 ) 
		Navigate(pstrValue);



	else 
		CActiveXUI::SetAttribute(pstrName, pstrValue);
}
*/

