#include "stdafx.h"
#include "resource.h"
#include "ProGressBarDlg.h"

#include "baseCore.h"

#include "DownloadThread_File.h"



CProgressBarDLG::CProgressBarDLG():m_thread(NULL)
{


}
LRESULT CProgressBarDLG::OnInitDialog(HWND hwndFocus, LPARAM lParam)
{
	CenterWindow();
	SetFileName(UpdateExe);



	CProgressBarCtrl  ProgressBar=GetDlgItem(IDC_PROGRESS1);

	ProgressBar.SetRange(0,100);
	ProgressBar.SetPos(0);


	//SetTitleText(m_core->GetDestPakName());

	if (m_thread)
	m_thread->start();
	
	//

	
	

	return 0;
}


void CProgressBarDLG::SetDate(void)
{



}

void CProgressBarDLG::SetFileName(LPCTSTR file)
{

	CWindow fileT=GetDlgItem(IDC_FILENAME);


	fileT.SetWindowText(file);

}
void CProgressBarDLG::SetProgress(float par)
{

	CWindow ParT=GetDlgItem(IDC_PAR);
	TCHAR temp[512];
	wsprintf(temp,_T("%d%%"),(int)par);
	ParT.SetWindowText(temp);

	CProgressBarCtrl  ProgressBar=GetDlgItem(IDC_PROGRESS1);

	ProgressBar.SetPos(par);

}

void CProgressBarDLG::SetTitleText(LPCTSTR file)
{


	TCHAR temp[512];
	wsprintf(temp,_T("download ...%s"),file);


	SetWindowText(temp);

}