#include <sqplus.h>


#pragma once


class SQWindowHost : public CWindowHost, public INotifyUI
{
public:
	SQWindowHost(SquirrelObject &so);
	virtual ~SQWindowHost();
	void OnFinalMessage(HWND /*hWnd*/);

	LPCTSTR GetWindowClassName() ;
	UINT GetClassStyle() ;// { return UI_CLASSSTYLE_FRAME; };
	void Notify(TNotifyUI& msg);
	LRESULT SQHandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);





	
	SquirrelObject m_Notify;
	SquirrelObject m_Instance;
protected:

	
	
	

	
};