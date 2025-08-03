



#pragma once
class SQPageWnd;
class  SQFrameWnd: public SQWindowHost
{
public:
	SQFrameWnd(SquirrelObject &so);
	virtual ~SQFrameWnd();
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void SetPage(SQPageWnd* client);
	void Relayout(SQPageWnd* pclient);


protected:
	HWND m_hWndClient;
};