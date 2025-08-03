#pragma  once
/////////////////////////////////////////////////////////////////////////////////////
//


class UILIB_API CWindowHost 
{
public:
	CWindowHost();
	virtual ~CWindowHost(){}


	HWND GetHWND() const;
	operator HWND() const;

	bool RegisterWindowClass(void);
	bool RegisterSuperclass();

	HWND Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, const RECT rc, HMENU hMenu = NULL);
	HWND Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT, HMENU hMenu = NULL);
	HWND Subclass(HWND hWnd);
	void Unsubclass();
	void ShowWindow(bool bShow = true, bool bTakeFocus = true);

	void ShowMiniWindow(bool bShow /*= true*/, bool bTakeFocus /*= false*/);
	bool ShowModal();
	void Close();
	void CenterWindow();
	void SetTimer(int id,int time);
	void KillTimer(int id);
	void SetIcon(UINT nRes);

	void SetWindowPos(short x,short y,bool draw);

	void EndDialog(int ret);
	int m_nWidth;
	int m_nHeight;

	bool m_bDrag;

protected:
	virtual LPCTSTR GetWindowClassName()  = 0;
	virtual LPCTSTR GetSuperClassName() const;
	virtual UINT GetClassStyle() ;

	LRESULT SendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);
	LRESULT PostMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);
	void ResizeClient(int cx = -1, int cy = -1);

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void OnFinalMessage(HWND hWnd);

	static LRESULT CALLBACK __WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK __ControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:

	
	HWND m_hWnd;
	WNDPROC m_OldWndProc;
	bool m_bSubclassed;
};