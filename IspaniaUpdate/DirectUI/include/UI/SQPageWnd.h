
#include <sqplus.h>


#pragma once



class SQPageWnd : public SQWindowHost
{
public:
	SQPageWnd(SquirrelObject &so);

	virtual ~SQPageWnd()
	{
		//m_pm=0;

	}
	
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL SetBackGround(LPCTSTR szFileName,short red,short green,short blue);
void SetBackGroundRegion(bool v);

	void Init();
	void OnPrepareAnimation();

	LPCTSTR GetDialogResource();

	
public:
	
	CPaintManagerUI m_pm;

	

	HRGN m_hRegion;
	HBITMAP m_hBitmap;


	int m_nxPos;
	int m_nyPos;
};