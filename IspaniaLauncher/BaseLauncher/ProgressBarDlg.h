

#pragma once 
#include "CDownLoad.h"
namespace base
{
	class Thread;
}
class CProgressBarDLG : public  CDialogImpl<CProgressBarDLG>,public Iprogress
{
public:
	enum { IDD = IDD_DLCLIENTUPDATE };

	BEGIN_MSG_MAP_EX(CProgressBarDLG)
		MSG_WM_INITDIALOG(OnInitDialog)
	END_MSG_MAP()

	CProgressBarDLG();

	void SetFileName(LPCTSTR file);
	void SetTitleText(LPCTSTR file);
	void SetProgress(float par);
	void SetDate(void);
	base::Thread* m_thread;
	


	LRESULT OnInitDialog(HWND hwndFocus, LPARAM lParam);

};