
#pragma once
#include <windows.h>
enum PSTATE
{
	EM_CheckVersion,
	EM_UpdateFailed,
	EM_Downloading,
	EM_CheckMD5ing,
	EM_ExecuteFile,
	EM_DownloadingOK,
	EM_MD5CheckOK,
	EM_VersionOK,
	EM_UpdateOK

};



enum WM_APPMSG
{
	WM_DLOK=WM_APP+100,
	WM_DLFail,
	WM_MD5OK,
	WM_MD5Fail,
	WM_ExecOK,
	WM_ExecFail,
	WM_UpdateOK,
	WM_RegionError,
	WM_VersionBetter
};

void UpdateIni_Load(TCHAR* IP,TCHAR* port,TCHAR * windowname);
IniFile2& UpdateIni_StringIni(void);
void UpdateIni_Finish(void);

void RuneConfig_Save(void);
float MD5_GetPar(void);
int DLFile_GetNow(void);
//float DLFile_GetNow(void); // GKy 100408 #test
int DLFile_GetMax(void);
double DLFile_GetSpeed(void);
void VC_ExecuteFile(void );
void VC_CheckFileMD5(void );
void VC_SetNewVersion(void );

void VC_PlayGame(TCHAR *account,TCHAR*password);
void VC_RunAgain( TCHAR  *IP, TCHAR* port,TCHAR  *account,TCHAR  *password);
int  VC_RunP2Pdownloader(void); // GKy 100420 for P2P


void VC_SelectZone(int index);
void VC_SetLink(HWND hWnd,const TCHAR * link);
void VC_SetCountryAndLanguage(int  c,int l,TCHAR *windowname);

bool VC_IsFirstInstall(void);
void VC_SetPlayGame(bool te);
void VC_FinisLauncher(void);


UINT  sThreadFunLSC(LPVOID LParam);