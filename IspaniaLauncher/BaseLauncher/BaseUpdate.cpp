// BaseUpdate.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <shlwapi.h>
#include <process.h>
#include "Zip/unzip.h"
#include <string>
#include "cupload.h"
#include <shellApi.h>
#include <tlhelp32.h>
#include "cmdlineargs.h"
#include "resource.h"
#include "IniFile2.h"
#include "VersionData.h"
#include "CUploadDlg.h"
//#include "DownloadFileManage.h"
//#include "MD5\MD5Checksum.h"
#include "BaseCore.h"
using namespace std;

#define RUNEDEVINI "RuneDev.ini"
#define FILELISTINI "filelist.ini"
//#include <iostream.h>
int KILL_PROC_BY_NAME(const char *);


BaseCore BaseCore:: g_core;
CString url;
CString password;
IniFile2 runeDev;
IniFile2 g_filelist;
TCHAR WorkDirectory[512];
CString d_account;
CString d_password;
CString PatchURL;
CString PatchPort;
int DeleteDirectory(const CString &refcstrRootDirectory,bool      bDeleteSubdirectories )
{
	static int sIndex=0;
	int index=sIndex;
	sIndex++;

	bool            bSubdirectory = false;       // Flag, indicating whether
	// subdirectories have been found
	HANDLE          hFile;                       // Handle to directory
	CString     strFilePath;                 // Filepath
	CString     strPattern;                  // Pattern
	WIN32_FIND_DATA FileInformation;             // File information


	strPattern = refcstrRootDirectory + "\\*.*";
	hFile = ::FindFirstFile(strPattern, &FileInformation);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			if(FileInformation.cFileName[0] != '.')
			{
				
				strFilePath = refcstrRootDirectory + _T("\\") + FileInformation.cFileName;

				TCHAR temp[512];

				lstrcpy(temp, FileInformation.cFileName);
				if(FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if(bDeleteSubdirectories)
					{
						// Delete subdirectory


						//lstrlwr(temp);
						
							int iRC = DeleteDirectory(strFilePath, bDeleteSubdirectories);
							if(iRC)
								return iRC;
//#ifndef _DEBUG

							if (index==0)
							g_filelist.Delete(temp);
//#endif
						
					}
					else
						bSubdirectory = true;
				}
				else
				{
					// Set file attributes
					int c=3;

					do 
					{
						if (::SetFileAttributes(strFilePath,	FILE_ATTRIBUTE_NORMAL)==TRUE) 
						{

							if(::DeleteFile(strFilePath) == TRUE)
							{
								break;
							}

						}


						Sleep(1000);

						c--;
					}while(c>=0);
					if(c<0)
						return ::GetLastError();		
				}
			}
		} while(::FindNextFile(hFile, &FileInformation) == TRUE);

		// Close handle
		::FindClose(hFile);

		DWORD dwError = ::GetLastError();
		if(dwError != ERROR_NO_MORE_FILES)
			return dwError;
		else
		{
			if(!bSubdirectory)
			{

							


				// Set directory attributes
				if(::SetFileAttributes(refcstrRootDirectory,
					FILE_ATTRIBUTE_NORMAL) == FALSE)
					return ::GetLastError();

				// Delete directory
				if(::RemoveDirectory(refcstrRootDirectory) == FALSE)
					return ::GetLastError();
			}
		}
	}

	return 0;
}

void UploadCrashRpt(CString  refcstrRootDirectory)
{
	static int sInext=0;
	int Index=sInext;
	sInext++;
	WIN32_FIND_DATA FileInformation;             // File information

	CString     strFilePath;                 // Filepath
	CString     strPattern;                  // Pattern

	g_filelist.SetSection("ftp");
	g_filelist.SetStr("IP",url);

	CFileUpload fileupload;
	fileupload.SetTo(url);
	fileupload.SetPassWord(password);

	g_filelist.SetSection("files");
	HANDLE          hFile; 
	//char temp[512];
	strPattern = refcstrRootDirectory+_T("\\*.*");
	hFile = ::FindFirstFile(strPattern, &FileInformation);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			if(FileInformation.cFileName[0] != '.')
			{

				TCHAR temp[512];
				strcpy(temp,FileInformation.cFileName);
				strlwr(temp);
				
				strFilePath = refcstrRootDirectory + _T("\\") + temp;

				if(FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					

					int mask=0;
					if (Index==0)
					 mask=g_filelist.IntDef(temp,0);
					if (mask==0)
					{
						UploadCrashRpt(strFilePath);
						if (Index==0)
						g_filelist.SetInt(temp,1);
					}

				
					//RemoveDirectory(strFilePath.c_str());
					//	DeleteDirectory(strFilePath);

				}
				else
				{
					bool upload=false;
				

					if (temp[0]=='r'&&temp[1]=='a')
					{
						
						if (strstr(temp,_T("zip")))
						{



							upload=true;
						}

					}
					
					if (upload)
					{
						fileupload.SetFile(strFilePath,temp);

						fileupload.Upload();	

						
					}



					
				 
					
				}
			}

		} while(::FindNextFile(hFile, &FileInformation) == TRUE);
		// Close handle
		::FindClose(hFile);

	}


	


	runeDev.SetInt("NeedCheck",1);
	runeDev.Save(RUNEDEVINI);
}


void UpdateVCP2p()
{

	//如過有 update.rar 解開
	if( PathFileExists( VCP2PZIP ) == TRUE )
	{

		//解壓縮		
		HZIP	hz; 
		ZRESULT	zRet;
		hz = OpenZip( VCP2PZIP ,0 );


		ZIPENTRY ze; 
		BOOL makeNewIFO=FALSE;
		GetZipItem(hz,-1,&ze); 
		int numitems=ze.index;
		for (int zi=0; zi<numitems; zi++)
		{ 

			GetZipItem(hz,zi,&ze);
			int count=20;
			int i;
			for (i=0;i<count;i++) 
			{
				KILL_PROC_BY_NAME("VCp2p.exe");
				

				//lstrcpy(temp,ze.name);

				//lstrlwr(temp);
				//	if (strcmp(temp,"download.ifo")==0)
				//	makeNewIFO=TRUE;

				SetFileAttributes( ze.name , FILE_ATTRIBUTE_NORMAL );
				zRet = UnzipItem(hz,zi,ze.name);
				if (zRet!=ZR_FLATE && zRet!=ZR_OK && zRet!= ZR_MORE)
					Sleep(100);
				else

					break;
			} 
			if (i==count)
				MessageBox(NULL,_T("Please reboot computer "),_T("Reboot"),0);

		}
		CloseZip(hz);
		/*
		if (makeNewIFO)
		{
		DownloadFileManage * downloadFileManager = new DownloadFileManage(string("download.ifo") );
		char fileMd5[1024];		
		CMD5Checksum::GetMD5(UpdateRar,fileMd5);
		downloadFileManager->AddDownloadFile( string(UpdateRar ),string( fileMd5 ));
		downloadFileManager->CheckVersion(string("New Version"));
		downloadFileManager->SetNewVersion();
		delete downloadFileManager;		
		}*/		
		DeleteFile( VCP2PZIP );
	}
	



}

void UpdateAndRunUpdate()
{

	//如過有 update.rar 解開
	if( PathFileExists( UpdateRar ) == TRUE )
	{

		//解壓縮		
		HZIP	hz; 
		ZRESULT	zRet;
		hz = OpenZip( UpdateRar ,0 );

		
		ZIPENTRY ze; 
		BOOL makeNewIFO=FALSE;
		GetZipItem(hz,-1,&ze); 
		int numitems=ze.index;
		for (int zi=0; zi<numitems; zi++)
		{ 

			GetZipItem(hz,zi,&ze);
			int count=20;
			int i;
			for (i=0;i<count;i++) 
			{
				KILL_PROC_BY_NAME("ClientUpdate.exe");
				KILL_PROC_BY_NAME("IspaniaUpdate.exe");
				KILL_PROC_BY_NAME("IspaniaUpdate2.exe");

				//lstrcpy(temp,ze.name);

				//lstrlwr(temp);
			//	if (strcmp(temp,"download.ifo")==0)
				//	makeNewIFO=TRUE;

				SetFileAttributes( ze.name , FILE_ATTRIBUTE_NORMAL );
				zRet = UnzipItem(hz,zi,ze.name);
				if (zRet!=ZR_FLATE && zRet!=ZR_OK && zRet!= ZR_MORE)
					Sleep(100);
				else

					break;
			} 
			if (i==count)
				MessageBox(NULL,_T("Please reboot computer "),_T("Reboot"),0);

		}
		CloseZip(hz);
/*
		if (makeNewIFO)
		{
			DownloadFileManage * downloadFileManager = new DownloadFileManage(string("download.ifo") );
			char fileMd5[1024];		
			CMD5Checksum::GetMD5(UpdateRar,fileMd5);
			downloadFileManager->AddDownloadFile( string(UpdateRar ),string( fileMd5 ));
			downloadFileManager->CheckVersion(string("New Version"));
			downloadFileManager->SetNewVersion();
			delete downloadFileManager;		
		}*/		
		DeleteFile( UpdateRar );
	}
	CString pArg1= _T("0963");


	char temp[512];

	if (d_account.GetLength()>0)
	{
			sprintf(temp," %s",(LPCTSTR)d_account);
			pArg1=pArg1+temp;
	}
	if (d_password.GetLength()>0)
	{
		sprintf(temp," %s",(LPCTSTR)d_password);
		pArg1=pArg1+temp;
	}
	if (PatchURL.GetLength()>0)
	{
		sprintf(temp," PatchIP:%s",(LPCTSTR)PatchURL);
		pArg1=pArg1+temp;
	}

	if (PatchPort.GetLength()>0)
	{
		sprintf(temp," PatchPort:%s",(LPCTSTR)PatchPort);
		pArg1=pArg1+temp;
	}



	SHELLEXECUTEINFO ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = _T("open");
	ShExecInfo.lpFile = UpdateExe;		
	ShExecInfo.lpParameters = pArg1;	
	ShExecInfo.lpDirectory = 0;
	ShExecInfo.nShow = SW_SHOWNORMAL;
	ShExecInfo.hInstApp = NULL;	
	ShellExecuteEx(&ShExecInfo);



}

void UpLoadState (CmdLineArgs& args)
{

	url=args[2];
	password=args[3];

	bool bDlg=false;
	if (args.size()>=6)
	{
		if (!strcmp(args[5],"1"))
			bDlg=true;

	}
	bool bOK=!bDlg;
	if (bDlg)
	{

		BaseCore::g_core.m_crashString.SetSection("CrashString");
		CString sReport =BaseCore::g_core.m_crashString.StrDef("title","Crash repot");
		CString sContext =BaseCore::g_core.m_crashString.StrDef("context","Sorry! something is broke.");
		CString sOK =BaseCore::g_core.m_crashString.StrDef("ok","OK");
		CString sCancel =BaseCore::g_core.m_crashString.StrDef("cancel","Cancel");


	
		CUploadDlg dlg;


		dlg.SetString(sReport, sContext, sOK, sCancel);


		if (IDOK ==dlg.DoModal())
		{
			bOK=TRUE;

		}


	}


	UploadCrashRpt(args[4]);

	DeleteDirectory(args[4],true);


}
string UnicodeToLocaliz(string &a)
{

	WCHAR wtemp[512];
	int len=MultiByteToWideChar(CP_UTF8,0, a.c_str(), -1, wtemp, 511);

	wtemp[len]=0;
	char temp[512];

	len=WideCharToMultiByte(CP_ACP, 0,wtemp , -1,temp ,511,NULL,NULL);
	temp[len]=0;

	return temp;


}
int  NormalState (bool  bWrite)
{
int ret=0;
	BaseCore::g_core.Init(PatchURL,PatchPort);

	int needCheck =runeDev.IntDef("NeedCheck",0);
	if (0)
	{

		TCHAR temp[512];
		BaseCore::g_core.m_string.SetSection("BOOT");

		string Tmsg=BaseCore::g_core.m_string.StrDef("MSG_NEEDCHECK_TITLE","檢查");
		string Cmsg=BaseCore::g_core.m_string.StrDef("MSG_NEEDCHECK_MSG","將進行檔案檢查修護");


		BaseCore::g_core.m_VresionIni.SetSection("BOOT");

		string version=BaseCore::g_core.m_ini.StrDef("Current_Version","2.0.0.0");

		Tmsg=UnicodeToLocaliz(Tmsg);
		Cmsg=UnicodeToLocaliz(Cmsg);

		wsprintf(temp,_T("%s"),Cmsg.c_str());
		int retCode	=MessageBox(NULL,temp,Tmsg.c_str(),MB_ICONEXCLAMATION|MB_YESNO);
		if (retCode==IDYES)
		{
			char  pArg1[4096];// _T("0963");

			
			BaseCore::g_core.m_ini.SetSection("BOOT");

			string DownloadURL=BaseCore::g_core.m_ini.StrDef("DownloadURL","http://frogster.vo.llnwd.net/o9/patch/");


			

		


			VersionData ver;
			CString path;

			ver.SetData(version.c_str() ,path);


			wsprintf(pArg1,"%d.%d.%d.%d %s %s",ver.m_version[0],ver.m_version[1],ver.m_version[2],ver.m_version[3],DownloadURL.c_str(),WorkDirectory);


	


			runeDev.SetInt("NeedCheck",0);
			
			runeDev.Save(RUNEDEVINI);


			SHELLEXECUTEINFO ShExecInfo = {0};
			ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
			ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
			ShExecInfo.hwnd = NULL;
			ShExecInfo.lpVerb = _T("open");
			ShExecInfo.lpFile = "VersionChecker.exe";		
			ShExecInfo.lpParameters = pArg1;	
			ShExecInfo.lpDirectory = 0;
			ShExecInfo.nShow = SW_SHOWNORMAL;
			ShExecInfo.hInstApp = NULL;	
			ShellExecuteEx(&ShExecInfo);
			ret=-3;

			return ret;
		}


	}


	if (bWrite)
	{

		TCHAR temp[1024];
		

		BaseCore::g_core.m_string.SetSection("BOOT");

		string Tmsg=BaseCore::g_core.m_string.StrDef("MSG_UAC_TITLE","使用者帳戶控制(UAC)");
		string Cmsg=BaseCore::g_core.m_string.StrDef("MSG_UAC_CANTWRITE","沒有檔案寫入權限,請檢查你的執行身分");



		Tmsg=UnicodeToLocaliz(Tmsg);
		Cmsg=UnicodeToLocaliz(Cmsg);

		wsprintf(temp,_T("%s"),Cmsg.c_str());
		MessageBox(NULL,temp,Tmsg.c_str(),0);
		ret=-1;
		return ret;
	}

	if (BaseCore::g_core.DownloadVersion()==FALSE)
	{

		TCHAR temp[1024];

		BaseCore::g_core.m_string.SetSection(_T("BOOT"));

		string msg=BaseCore::g_core.m_string.StrDef(_T("MSG_NET_ERROR"),_T("連線失敗, 請檢查你的網路或防火牆設定"));

		msg=UnicodeToLocaliz(msg);
		wsprintf(temp,_T("Can't download version.txt\n %s"),msg .c_str());
		MessageBox(NULL,temp,_T("Version"),0);
		ret=-2;
		return ret;
	}

	

BaseCore::g_core.DownloadVcp2p(CString (WorkDirectory)+_T("\\")+CString (VCP2PExe));

BaseCore::g_core.DownloadUpadteZip(CString (WorkDirectory)+_T("\\")+CString (UpdateExe));


return ret;

}  

bool YouCantPass()
{

	TCHAR YouCantPassName[]=_T("YouCantPass.tmp");
	bool ret=false;
	HANDLE file=CreateFile(YouCantPassName,GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 0, NULL ) ;

	if (file == INVALID_HANDLE_VALUE )
		ret=true;

	else if (file)
	{

		CloseHandle(file);

		if (DeleteFile(YouCantPassName) == FALSE)
		{

			ret=true;

		}


	}

	return ret;



}

HANDLE g_hOneInstance; 

// wb1 crashreport:1234 Crashreport
//int _tmain(int argc, _TCHAR* argv[])
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
#ifndef _DEBUG

//	ShowWindow(FindWindowA("ConsoleWindowClass", NULL), false);
#endif



	GetModuleFileName(NULL,WorkDirectory,512);
	int len;
	len=lstrlen(WorkDirectory);
	for (int i=len-1;i>0;--i)
	{

		if (WorkDirectory[i]=='\\')
		{
			WorkDirectory[i]=0;
			break;
		}

	}


	SetCurrentDirectory(WorkDirectory);

	runeDev.SetIniFile(RUNEDEVINI);

	runeDev.SetSection("Crash");

	g_filelist.SetIniFile(FILELISTINI);
	g_filelist.SetSection("files");

	

	bool bWrite=YouCantPass();
		

	WSADATA wsaData;

	int iResult = WSAStartup( MAKEWORD(2,2), &wsaData );
	if ( iResult != NO_ERROR )
		return -1;


	g_hOneInstance = ::CreateMutex(NULL, FALSE, "Lancher.exe");  
	if(GetLastError() == ERROR_ALREADY_EXISTS)  
	{  
		//AfxMessageBox("ALREADY_EXISTS");  
		return -1;  
	}  

	CmdLineArgs args;

	int count=args.size()-1;
	bool bCrash=false;

	if (count>2)
	{
		CString temp=args[1];
		int index=temp.Find("crash");
		if (index==0)
		{
			bCrash=true;
		}
	}
	if (!bCrash)
	{

	
	
		int i=0;
		while(count)
		{
			CString temp=args[1+i];
			int len=temp.GetLength();
			CString str;
			int index=temp.Find(":");
			if (index!=-1)
			{
				str=temp.Left(index);

				if (str.Compare("PatchIP")==0)
					PatchURL=temp.Right(len-(index+1));
				else if (str.Compare("PatchPort")==0)
					PatchPort=temp.Right(len-(index+1));
			}		
			else if (d_account.GetLength()==0)
				d_account=args[1+i];
			else if (d_password.GetLength()==0)
				d_password=args[1+i];
			count--;
			i++;
		}

	}
	
	int ret=0;

	if (bCrash )
	{
		ret=1;

		UpLoadState(args);

		g_filelist.Save(FILELISTINI);
		
	}
	
	else
	{
		ret=NormalState(bWrite);
		if (!ret)
		{

		
			if (BaseCore::g_core.m_GoVCP2P)
				UpdateVCP2p();

			if (BaseCore::g_core.m_GoUpdate)
				UpdateAndRunUpdate();

		}
		
	}

	
	return ret;
}



int KILL_PROC_BY_NAME(LPCTSTR szToTerminate)
// Created: 6/23/2000  (RK)
// Last modified: 3/10/2002  (RK)
// Please report any problems or bugs to kochhar@physiology.wisc.edu
// The latest version of this routine can be found at:
//     http://www.neurophys.wisc.edu/ravi/software/killproc/
// Terminate the process "szToTerminate" if it is currently running
// This works for Win/95/98/ME and also Win/NT/2000/XP
// The process name is case-insensitive, i.e. "notepad.exe" and "NOTEPAD.EXE"
// will both work (for szToTerminate)
// Return codes are as follows:
//   0   = Process was successfully terminated
//   603 = Process was not currently running
//   604 = No permission to terminate process
//   605 = Unable to load PSAPI.DLL
//   602 = Unable to terminate process for some other reason
//   606 = Unable to identify system type
//   607 = Unsupported OS
//   632 = Invalid process name
//   700 = Unable to get procedure address from PSAPI.DLL
//   701 = Unable to get process list, EnumProcesses failed
//   702 = Unable to load KERNEL32.DLL
//   703 = Unable to get procedure address from KERNEL32.DLL
//   704 = CreateToolhelp32Snapshot failed
// Change history:
//   modified 3/8/2002  - Borland-C compatible if BORLANDC is defined as
//                        suggested by Bob Christensen
//   modified 3/10/2002 - Removed memory leaks as suggested by
//					      Jonathan Richard-Brochu (handles to Proc and Snapshot
//                        were not getting closed properly in some cases)
{
	BOOL bResult,bResultm;
	DWORD aiPID[1000],iCb=1000,iNumProc,iV2000=0;
	DWORD iCbneeded,i,iFound=0;
	TCHAR szName[MAX_PATH];
	TCHAR szToTermUpper[MAX_PATH];
	HANDLE hProc,hSnapShot,hSnapShotm;
	OSVERSIONINFO osvi;
	HINSTANCE hInstLib;
	int iLen,iLenP,indx;
	HMODULE hMod;
	PROCESSENTRY32 procentry;      
	MODULEENTRY32 modentry;

	// Transfer Process name into "szToTermUpper" and
	// convert it to upper case
	iLenP=lstrlen(szToTerminate);
	if(iLenP<1 || iLenP>MAX_PATH) return 632;
	for(indx=0;indx<iLenP;indx++)
		szToTermUpper[indx]=toupper(szToTerminate[indx]);
	szToTermUpper[iLenP]=0;

	// PSAPI Function Pointers.
	BOOL (WINAPI *lpfEnumProcesses)( DWORD *, DWORD cb, DWORD * );
	BOOL (WINAPI *lpfEnumProcessModules)( HANDLE, HMODULE *,
		DWORD, LPDWORD );
	DWORD (WINAPI *lpfGetModuleBaseName)( HANDLE, HMODULE,
		LPTSTR, DWORD );

	// ToolHelp Function Pointers.
	HANDLE (WINAPI *lpfCreateToolhelp32Snapshot)(DWORD,DWORD) ;
	BOOL (WINAPI *lpfProcess32First)(HANDLE,LPPROCESSENTRY32) ;
	BOOL (WINAPI *lpfProcess32Next)(HANDLE,LPPROCESSENTRY32) ;
	BOOL (WINAPI *lpfModule32First)(HANDLE,LPMODULEENTRY32) ;
	BOOL (WINAPI *lpfModule32Next)(HANDLE,LPMODULEENTRY32) ;

	// First check what version of Windows we're in
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	bResult=GetVersionEx(&osvi);
	if(!bResult)     // Unable to identify system version
		return 606;

	// At Present we only support Win/NT/2000/XP or Win/9x/ME
	if((osvi.dwPlatformId != VER_PLATFORM_WIN32_NT) &&
		(osvi.dwPlatformId != VER_PLATFORM_WIN32_WINDOWS))
		return 607;

	if(osvi.dwPlatformId==VER_PLATFORM_WIN32_NT)
	{
		// Win/NT or 2000 or XP

		// Load library and get the procedures explicitly. We do
		// this so that we don't have to worry about modules using
		// this code failing to load under Windows 9x, because
		// it can't resolve references to the PSAPI.DLL.
		hInstLib = LoadLibraryA("PSAPI.DLL");
		if(hInstLib == NULL)
			return 605;

		// Get procedure addresses.
		lpfEnumProcesses = (BOOL(WINAPI *)(DWORD *,DWORD,DWORD*))
			GetProcAddress( hInstLib, "EnumProcesses" ) ;
		lpfEnumProcessModules = (BOOL(WINAPI *)(HANDLE, HMODULE *,
			DWORD, LPDWORD)) GetProcAddress( hInstLib,
			"EnumProcessModules" ) ;
		lpfGetModuleBaseName =(DWORD (WINAPI *)(HANDLE, HMODULE,
			LPTSTR, DWORD )) GetProcAddress( hInstLib,
			"GetModuleBaseNameA" ) ;

		if(lpfEnumProcesses == NULL ||
			lpfEnumProcessModules == NULL ||
			lpfGetModuleBaseName == NULL)
		{
			FreeLibrary(hInstLib);
			return 700;
		}

		bResult=lpfEnumProcesses(aiPID,iCb,&iCbneeded);
		if(!bResult)
		{
			// Unable to get process list, EnumProcesses failed
			FreeLibrary(hInstLib);
			return 701;
		}

		// How many processes are there?
		iNumProc=iCbneeded/sizeof(DWORD);

		// Get and match the name of each process
		for(i=0;i<iNumProc;i++)
		{
			// Get the (module) name for this process

			lstrcpy(szName,_T("Unknown"));
			// First, get a handle to the process
			hProc=OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,
				aiPID[i]);
			// Now, get the process name
			if(hProc)
			{
				if(lpfEnumProcessModules(hProc,&hMod,sizeof(hMod),&iCbneeded) )
				{
					iLen=lpfGetModuleBaseName(hProc,hMod,szName,MAX_PATH);
				}
			}
			CloseHandle(hProc);
			// We will match regardless of lower or upper case
#ifdef BORLANDC
			if(strcmp(strupr(szName),szToTermUpper)==0)
#else

			iLenP=lstrlen(szName);
			if(iLenP<1 || iLenP>MAX_PATH) return 632;
			for(indx=0;indx<iLenP;indx++)
				szName[indx]=toupper(szName[indx]);


			if(lstrcmp(szName,szToTermUpper)==0)
#endif
			{
				// Process found, now terminate it
				iFound=1;
				// First open for termination
				hProc=OpenProcess(PROCESS_TERMINATE,FALSE,aiPID[i]);
				if(hProc)
				{
					if(TerminateProcess(hProc,0))
					{
						// process terminated
						CloseHandle(hProc);
						FreeLibrary(hInstLib);
						return 0;
					}
					else
					{
						// Unable to terminate process
						CloseHandle(hProc);
						FreeLibrary(hInstLib);
						return 602;
					}
				}
				else
				{
					// Unable to open process for termination
					FreeLibrary(hInstLib);
					return 604;
				}
			}
		}
	}

	if(osvi.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS)
	{
		// Win/95 or 98 or ME

		hInstLib = LoadLibrary(_T("Kernel32.DLL"));
		if( hInstLib == NULL )
			return 702;

		// Get procedure addresses.
		// We are linking to these functions of Kernel32
		// explicitly, because otherwise a module using
		// this code would fail to load under Windows NT,
		// which does not have the Toolhelp32
		// functions in the Kernel 32.
		lpfCreateToolhelp32Snapshot=
			(HANDLE(WINAPI *)(DWORD,DWORD))
			GetProcAddress( hInstLib,
			"CreateToolhelp32Snapshot" ) ;
		lpfProcess32First=
			(BOOL(WINAPI *)(HANDLE,LPPROCESSENTRY32))
			GetProcAddress( hInstLib, "Process32First" ) ;
		lpfProcess32Next=
			(BOOL(WINAPI *)(HANDLE,LPPROCESSENTRY32))
			GetProcAddress( hInstLib, "Process32Next" ) ;
		lpfModule32First=
			(BOOL(WINAPI *)(HANDLE,LPMODULEENTRY32))
			GetProcAddress( hInstLib, "Module32First" ) ;
		lpfModule32Next=
			(BOOL(WINAPI *)(HANDLE,LPMODULEENTRY32))
			GetProcAddress( hInstLib, "Module32Next" ) ;
		if( lpfProcess32Next == NULL ||
			lpfProcess32First == NULL ||
			lpfModule32Next == NULL ||
			lpfModule32First == NULL ||
			lpfCreateToolhelp32Snapshot == NULL )
		{
			FreeLibrary(hInstLib);
			return 703;
		}

		// The Process32.. and Module32.. routines return names in all uppercase

		// Get a handle to a Toolhelp snapshot of all the systems processes.

		hSnapShot = lpfCreateToolhelp32Snapshot(
			TH32CS_SNAPPROCESS, 0 ) ;
		if( hSnapShot == INVALID_HANDLE_VALUE )
		{
			FreeLibrary(hInstLib);
			return 704;
		}

		// Get the first process' information.
		procentry.dwSize = sizeof(PROCESSENTRY32);
		bResult=lpfProcess32First(hSnapShot,&procentry);

		// While there are processes, keep looping and checking.
		while(bResult)
		{
			// Get a handle to a Toolhelp snapshot of this process.
			hSnapShotm = lpfCreateToolhelp32Snapshot(
				TH32CS_SNAPMODULE, procentry.th32ProcessID) ;
			if( hSnapShotm == INVALID_HANDLE_VALUE )
			{
				CloseHandle(hSnapShot);
				FreeLibrary(hInstLib);
				return 704;
			}
			// Get the module list for this process
			modentry.dwSize=sizeof(MODULEENTRY32);
			bResultm=lpfModule32First(hSnapShotm,&modentry);

			// While there are modules, keep looping and checking
			while(bResultm)
			{
				if(lstrcmp(modentry.szModule,szToTermUpper)==0)
				{
					// Process found, now terminate it
					iFound=1;
					// First open for termination
					hProc=OpenProcess(PROCESS_TERMINATE,FALSE,procentry.th32ProcessID);
					if(hProc)
					{
						if(TerminateProcess(hProc,0))
						{
							// process terminated
							CloseHandle(hSnapShotm);
							CloseHandle(hSnapShot);
							CloseHandle(hProc);
							FreeLibrary(hInstLib);
							return 0;
						}
						else
						{
							// Unable to terminate process
							CloseHandle(hSnapShotm);
							CloseHandle(hSnapShot);
							CloseHandle(hProc);
							FreeLibrary(hInstLib);
							return 602;
						}
					}
					else
					{
						// Unable to open process for termination
						CloseHandle(hSnapShotm);
						CloseHandle(hSnapShot);
						FreeLibrary(hInstLib);
						return 604;
					}
				}
				else
				{  // Look for next modules for this process
					modentry.dwSize=sizeof(MODULEENTRY32);
					bResultm=lpfModule32Next(hSnapShotm,&modentry);
				}
			}

			//Keep looking
			CloseHandle(hSnapShotm);
			procentry.dwSize = sizeof(PROCESSENTRY32);
			bResult = lpfProcess32Next(hSnapShot,&procentry);
		}
		CloseHandle(hSnapShot);
	}
	if(iFound==0)
	{
		FreeLibrary(hInstLib);
		return 603;
	}
	FreeLibrary(hInstLib);
	return 0;
}
