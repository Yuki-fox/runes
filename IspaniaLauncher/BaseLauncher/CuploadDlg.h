
#pragma once

class CUploadDlg : public CDialogImpl<CUploadDlg>
{
public:
	enum { IDD = IDD_UPLOADDLG };
		BEGIN_MSG_MAP(CUploadDlg)
			MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
			COMMAND_ID_HANDLER(IDOK, OnOk)
			COMMAND_ID_HANDLER(IDCANCEL, OnCanecl)

			
		END_MSG_MAP()
		CString m_title;
		CString m_context;
		CString m_ok;
		CString m_cancel;
		CUploadDlg():m_OK(TRUE){}
		~CUploadDlg(){}

		void SetString(CString& title,CString& context,CString& ok ,CString &cancel)
		{
			m_title=title;
			m_context=context;
			m_ok=ok;
			m_cancel=cancel;


		}


		BOOL m_OK;



LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	m_OK=FALSE;

//	CStatic text=GetDlgItem(IDC_STATICLANG);
/*
	if (GetUserDefaultLangID()==0x804)
	{

	
	CString t;
	t.LoadString(IDS_STRING129);

	SetDlgItemText(IDC_STATICLANG,t);
	}
*/	

	SetWindowText(m_title);
	GetDlgItem(IDC_CONTEXT).SetWindowText(m_context);
	GetDlgItem(IDOK).SetWindowText(m_ok);
	GetDlgItem(IDCANCEL).SetWindowText(m_cancel);

	return TRUE;
}
LRESULT OnOk(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	m_OK=TRUE;

	return 0;
}
LRESULT OnCanecl(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(IDCANCEL);
	m_OK=FALSE;

	return 0;
}
};