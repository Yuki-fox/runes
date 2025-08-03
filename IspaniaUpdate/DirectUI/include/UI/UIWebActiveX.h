#ifndef _MING_WEBACTIVEXUI_H
#define _MING_WEBACTIVEXUI_H
#pragma once
/////////////////////////////////////////////////////////////////////////////////////
//

class UILIB_API CWebActiveXUI : public CActiveXUI
{
	
public:
	CWebActiveXUI(SquirrelObject& so);

	virtual ~CWebActiveXUI(){}
	
 void Navigate(LPCTSTR URL);


	//void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

protected:
	//CWebUIController m_webUICtrl;
	
};
#endif//_MING_WEBACTIVEXUI_H