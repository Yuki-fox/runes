#include "stdafx.h"
#include "ReadUpdateIni.h"

#include "CDownLoad.h"
#include "VersionData.h"

#include "md5\MD5Checksum.h"
#include <base\base_Thread.h>
#include <shellapi.h>
#include <mmsystem.h>
#include <sqplus.h>
#include <ShlObj.h>


#ifdef _P2P_APPEND
//#include "libtorrentSimpleClient.h" // GKy 100421, using P2P as an individual downloader
#include "libtorrentSimpleClientInterface.h" // GKy 100421
#include "libtorrentSimpleClientInterface_body.h"
#endif

#define DOCUMENTS_SAVE_PATH	_T("OneForAll")

#define UPDATEINF _T("Update.inf")
#define UPDATEINI _T("Update_%s.ini")
#define STRINGINI _T("string_%s.ini")
#define SERVERINI _T("serverlist_%s.ini")
#define VERSIONINI _T("Version.ini")
#define OLDUPDATEINI _T("Update.ini")

#define CONFIGINI _T("client.config.ini")


// GKy 100329 4561L27 ---------------------------------------------------------------------
bool g_bEnableP2P = true;



std::wstring g_wsTorrenFile = _T(""); //"LbT.torrent";
std::wstring g_wsWebSeed = _T("");

int g_iLscUpSpeed = 15000; // GKy 100427 // unit:Bps, default 15kB (15000)
int g_iLscDlSpeed = -1; // -1 == no Limit



#ifdef _P2P_APPEND
libtorrentSimpleClient_info gLscInfo; // GKy 100421

HANDLE g_hP2PdownloaderInstance = NULL; // GKy 100422 for Mutex 


HWND   g_hwndP2Pdownloader = NULL; // GKy 100430


#endif


CHAR wide_to_narrow(WCHAR w)
{
	// simple typecast
	// works because UNICODE incorporates ASCII into itself
	return CHAR(w);
}


std::string  g_fTSTRtoSTR( base::String tStr_ ){

		std::wstring wstr = tStr_.c_str();
		std::string url_seed_; url_seed_.resize(wstr.length());
		std::transform(wstr.begin(), wstr.end(), url_seed_.begin(), wide_to_narrow);

		return url_seed_;
}

////

//
//WCHAR narrow_to_wide(CHAR w)
//{
//	// simple typecast
//	// works because UNICODE incorporates ASCII into itself
//	return WCHAR(w);
//}
//
//
//base::String  g_fSTRtoTSTR( std::string tStr_ ){
//
//		std::string wstr = tStr_.c_str();
//		std::wstring url_seed_; url_seed_.resize(wstr.length());
//		std::transform(wstr.begin(), wstr.end(), url_seed_.begin(), narrow_to_wide);
//
//		return url_seed_;
//}



#if _P2P_APPEND
BOOL   CALLBACK   EnumWindowsProc_vcP2P(HWND   hwnd,LPARAM   lParam)    
{    
  TCHAR    lpWinTitle[256];  

  ::GetWindowText(hwnd,lpWinTitle,256-1);    

  std::wstring  m_strTitle = lpWinTitle;

  //if(m_strTitle.Find("RuneWaker P2P Downloader")!=-1)  
  if( wstring::npos != m_strTitle.find(_T("RuneWaker P2P Downloader")) )
  {  
	  g_hwndP2Pdownloader = hwnd;

	  return  FALSE; 
  }  
  return   TRUE;    
}    
#endif



// GKy 100503 add for close ALL related Wnd.
BOOL   CALLBACK   EnumWindowsProc_vcP2P_close(HWND   hwnd,LPARAM   lParam)    
{    
  TCHAR    lpWinTitle[256];  

  ::GetWindowText(hwnd,lpWinTitle,256-1);    

  std::wstring  m_strTitle = lpWinTitle;

  //if(m_strTitle.Find("RuneWaker P2P Downloader")!=-1)  
  if( wstring::npos != m_strTitle.find(_T("RuneWaker P2P Downloader")) )
  {  
		//g_hwndP2Pdownloader = hwnd;

		if( NULL != hwnd){
			::PostMessage(hwnd, WM_CLOSE, NULL, NULL); // 
		}//if

	  //return  FALSE; 
	  // GKy 100503 need close ALL related window.
  }  
  return   TRUE;    
}    




// -- GKy 100329 4561L27 ---------------------------------------------------------------------



void _SetProgressState(int state);
BOOL runLauncher=FALSE;

BOOL runGame=FALSE;
class internal_Update;
 internal_Update* g_update;

CControlUI *g_child;//=(CControlUI*)obh.GetInstanceUP(0);
float g_md5Par=0;
class MD5_prog :public IMD5_Progress
{
	bool progress_function(double dltotal,	double dlnow);

};
bool MD5_prog ::progress_function(double dltotal,	double dlnow)
{

	if (dltotal>0)
		g_md5Par=(dlnow*100)/dltotal;
	else 
		g_md5Par;

return true;
}


MD5_prog g_MD5_prog;

float MD5_GetPar(void)
{

  if( g_bEnableP2P ){

	// #P2P
	return g_md5Par;


  }else{

	// #ORIGIN

	return g_md5Par;

  }//if( g_bEnableP2P ){


}
void _MessageBox(LPCTSTR title,LPCTSTR context,LPCTSTR type,int buttons)
{
	SqPlus::SquirrelFunction<void> func(_T("_MessageBox"));
	func(title,context,type,buttons);
}
void SetPatchIP( char * ip,int port)
{

}

TCHAR g_clientName[1024];
TCHAR szWinTmpPath[1024];
TCHAR szGameExePath[512];
CPaintManagerUI* g_pManager;
//child->GetManager()->SetTimer(child,100,1000);

void SetCountryAndLanguage(const SQChar* country,const SQChar* language,TCHAR *windowname);

void SetToTable(IniFile2 &ini,LPCTSTR name);
void GetTableToIni(LPCTSTR Section,IniFile2& ini,SquirrelObject &table);

BOOL ExecuteFile(LPCTSTR filename)
{

	TCHAR temp[2048];

	BOOL ret=TRUE;

	wsprintf(temp,_T("%s"),filename);

	bool bWait=true;



	//SetProgressState(EM_ExecuteFile);


	SHELLEXECUTEINFO ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = _T("open");
	ShExecInfo.lpFile = temp;		
	ShExecInfo.lpParameters = NULL;	
	ShExecInfo.lpDirectory = szGameExePath;
	ShExecInfo.nShow = SW_SHOWNORMAL;
	ShExecInfo.hInstApp = NULL;	
	ShellExecuteEx(&ShExecInfo);
	if (bWait)
		WaitForSingleObject(ShExecInfo.hProcess,INFINITE);

	

	DWORD code;
	if (GetExitCodeProcess(ShExecInfo.hProcess,&code))
	{
		if (code==0x0badf00d)
		{
			//RecodeAndNextTime(temp);
			ret=FALSE;


		}
		else if (code!=0)
			ret=FALSE;

	
			

	}
	
	int count=10;
	do 
	{
		
		SetFileAttributes(temp,FILE_ATTRIBUTE_NORMAL);
		Sleep(5000);

		if (!DeleteFile(temp))
		{

			TCHAR* error=_T("error!!");
			DWORD ret=GetLastError();

			switch (ret)
			{
			case ERROR_FILE_NOT_FOUND :
				error=_T("ERROR_FILE_NOT_FOUND");
				break;
			case ERROR_ACCESS_DENIED :
				error=_T("ERROR_FILE_NOT_FOUND");
				break;

			}

			TCHAR temp2[1024];


				wsprintf(temp2,_T("Delete File %s %s"),temp,error);

		//	_MessageBox(_T("Delete File"),temp2,_T("Error"),0);

			Sleep(3500);
		
			count--;
		}
		else
			count=0;


	}while(count>0);

	return ret;
}



class execFile :public base::Thread
{
public:

	execFile();
	virtual ~execFile();
	void run();
	

	
};
execFile::execFile( )
{
}
execFile::~execFile()
{

}
class internal_Update;
class CheckMd5:public base::Thread
{
public:
	CheckMd5(internal_Update* p);
	virtual ~CheckMd5();
	void run();
	internal_Update* m_p;

};
CheckMd5::CheckMd5(internal_Update* p):m_p(p)
{
}
CheckMd5::~CheckMd5()
{


}

class downloadFile :public base::Thread
{
public:
	downloadFile(CFileDownLoad * dl);
	virtual ~downloadFile();
	void run();



	
	
	

	
//data

	CFileDownLoad * m_dl;

	
};
downloadFile::downloadFile(CFileDownLoad * dl):m_dl(dl)
{
}
downloadFile::~downloadFile()
{

	
}

// GKy 100409 0301L321 -------------------------------------------

class cThreadLSC :public base::Thread
{
public:
	cThreadLSC(void);
	virtual ~cThreadLSC();
	void run();
	
	//data
	///
	
};
cThreadLSC::cThreadLSC(void)
{

}
cThreadLSC::~cThreadLSC()
{
	
}
// -- GKy 100409 0301L321 -------------------------------------------


void _SetCheckZone(int index)
{
	SqPlus::SquirrelFunction<void> func(_T("SetCheckZoneDef"));
	func(index);
}

void _SetAllFiles(int files,int start,int end)
{
	SqPlus::SquirrelFunction<void> func(_T("_SetAllFiles"));
	func(files,start,end);


}

LPCTSTR GetString(LPCTSTR Section,LPCTSTR ID)
{
	SqPlus::SquirrelFunction<LPCTSTR> func(_T("GetString"));
	return func(Section,ID);
	

}
void _SetTotalStep(int count)
{
	SqPlus::SquirrelFunction<void> func(_T("_SetTotalStep"));
	func(count);
}

void _ProcPostMessage(LPCTSTR msg)
{

	SqPlus::SquirrelFunction<void> func(_T("_ProcPostMessage"));
	func(msg);
}

void _ProcFileText(LPCTSTR filename)
{
	SqPlus::SquirrelFunction<void> func(_T("_ProcFileText"));
	func(filename);

}

void _SetProgressBarMax(int max)
{

	SqPlus::SquirrelFunction<void> func(_T("_SetProgressBarMax"));
	func(max);

}
void _SetProgressBarPos(int now)
{
	SqPlus::SquirrelFunction<void> func(_T("_SetProgressBarPos"));
	func(now);

}



class internal_Update
{
public:
	char*  m_password;
	IniFile2 m_ini;
	IniFile2 m_versionIni;
	IniFile2 m_stringIni;
	IniFile2 m_serverIni;
	IniFile2 m_runeDevIni;
	IniFile2 m_URLExt;
	IniFile2 m_configIni;
	IniFile2 m_install;
	IniFile2 m_localization;
	IniFile2 m_country;

	base::String m_IP;
	base::String m_port;
	base::String m_windowname;

	bool m_Select;

	IniFile2 m_UpdateListIni;
	base::String m_savePath;
	base::String m_dlPath;

	cThreadLSC* m_tLsc; // GKy 100409
	downloadFile* m_download;
	CheckMd5* m_md5Thread;
	int m_selectZone;


	execFile* m_execfile;

	CFileDownLoad * m_dl;
	int m_state;

public:

	internal_Update();
	virtual ~internal_Update();

	void InitialDL(void);
	BOOL CheckVersion(void);
	void SetNewVersion(void);

	void SelectZone(int index);
	void UpgradeZip(void);


	BOOL ExecFile(void);
	void CheckNeedDLPatch();
	BOOL DLPatchEND(void);
	bool  DownloadFile();
	BOOL CheckFileMD5(void);

	void RunCheckMD5(void);

	void InitialCheckMD5Thread(void);
	void InitialThread(void);
	void InitialExecThread(void);
	void RunDownload(void);
	void RunExecfile(void);
	void Initial(TCHAR* IP,TCHAR* port,TCHAR* windowname);
	void LoadUpdate(TCHAR *windowname);
	void MakePassWord(void);
	void FinisThread(void );
	void DoState(int state);
	void PlayGame(void);

	void Finis(void)
	{
		m_bIsClose=true;
	
	}

	int m_sTime;
	int m_dlsec;
	bool m_bIsClose;
	int m_dlFiles;
	int m_iDonwloadPort;
	int m_iDonwloadPort2;
	char m_MD5[256];
	base::String m_strCurrentVersion;
	base::String m_patchFile;
	base::String m_patchFilePath;
	base::String m_strZipPathURL;
	base::String m_strZipPathURL2;

	base::String m_strProxyPathURL;
	base::String m_clientName;

	base::String m_strOnDownloadFileName;



};

IniFile2& UpdateIni_StringIni(void)
{
return g_update->m_stringIni;
}
void VC_SetCountryAndLanguage(int c,int l,TCHAR *windowname)
{

	SquirrelObject c_tab=SquirrelVM::GetRootTable().GetValue(_T("g_Country"));

	SquirrelObject c_obj=c_tab.GetValue(c);

	const SQChar*id=c_obj.GetString(_SC("id"));

	SquirrelObject l_tab=SquirrelVM::GetRootTable().GetValue(_T("g_Language"));


	SquirrelObject ltabdata=l_tab.GetValue(l);
	const SQChar*lang =ltabdata.GetString(_SC("id"));


	SetCountryAndLanguage(id,lang,windowname);
	g_update->SelectZone(0);

}

internal_Update::internal_Update():m_password(0),m_download(NULL),m_dl(NULL),m_bIsClose(FALSE),m_state(0),m_execfile(0),m_selectZone(0),m_Select(false),m_md5Thread(NULL)
{
	m_tLsc = NULL; // GKy 100409


}
internal_Update::~internal_Update()
{
	if (m_password)
		delete []m_password;
	m_password=NULL;

	if (m_dl)
		delete []m_dl;
	m_dl=NULL;


	m_md5Thread=NULL;

	//if (m_download)
	//	delete m_download;
	m_download=NULL;

	//if (m_execfile)
	//	delete m_execfile;
	m_execfile=NULL;

#ifdef _P2P_APPEND //GKy 100422 P2P
	if ( NULL != m_tLsc){
		delete m_tLsc;
		m_tLsc = NULL;
	}//if
	//gLsc.stop(); // GKy 100420 move to other place
#endif


// GKy 100325 #add P2P
#if _P2P_TEST
	MessageBox(NULL, _T(" gLsc.stop() "), _T("P2P"), MB_OK); // GKy #test
#endif


}

void internal_Update:: UpgradeZip(void)
{
	//m_password=new char[128];

	//MakePassWord();

	//m_password[16]=0;



	//TCHAR tempZip[512];
	//wsprintf(tempZip,_T("%s%s.zip"),(LPCTSTR)m_installDir,_T(UPDATEINF));

	TCHAR oldZip[512];

	wsprintf(oldZip,_T("%s"),UPDATEINF);

	TCHAR newZipx[512];
	wsprintf(newZipx,_T("%s.tmp"),UPDATEINF);
	//string buff;

	//m_Updateini.SaveString(buff);



	HZIP oldhz,newZip;
	ZRESULT ret;

	oldhz=OpenZip(oldZip ,m_password);
	newZip=CreateZip(newZipx, m_password);
	if (newZip)
	{
		//CString name;
		//	name.Format(_T(UPDATEINI),m_country);	
		base::String installname =_T("install.ini");

		if (oldhz)
		{
			ZIPENTRY ze; 

			GetZipItem(oldhz,-1,&ze); 
			int numitems=ze.index;
			for (int zi=0; zi<numitems; zi++)
			{
				GetZipItem(oldhz,zi,&ze);
				if (installname!=ze.name)
				{					
					char* data=new char[ze.unc_size] ;
					DWORD len=ze.unc_size;

					ret=UnzipItem(oldhz,zi,data,len);
					//	if (ret==ZR_OK)
					{

						ret=ZipAdd(newZip, ze.name,  data,(DWORD)len);

					}
					delete []data;					
				}
			}

			CloseZip(oldhz);
		}
		
		SquirrelObject root=SquirrelVM::GetRootTable();
		SquirrelObject sqtable=root.GetValue(_T("Install"));

		GetTableToIni(_T("BOOT"),m_install,sqtable);


		m_install.SetInt(_T("first"),0);
	

		std::string buff;

		m_install.SaveString(buff);
		



		ret=ZipAdd(newZip, (LPCTSTR) _T("install.ini"),   (void*) buff.c_str(),(DWORD)buff.size());

		CloseZip(newZip);

		SetFileAttributes(oldZip,FILE_ATTRIBUTE_NORMAL);

		DeleteFile(oldZip);
		BOOL ret=	MoveFile(newZipx,oldZip);

	}





	//	delete []m_password;
	//	m_password=NULL;
}
void internal_Update::LoadUpdate(TCHAR *windowname)
{
	//¸ÑÀ£ÁY		
	HZIP	hz; 
	ZRESULT	zRet;	
	hz = OpenZip( UPDATEINF ,m_password );

	ZIPENTRY ze; 
	
	TCHAR tempz[1024];
	
	if (hz)
	{
		int index;

		


		base::String select=m_install.StrDef(_T("select"),_T("cn"));

	
		m_localization.SetSection(select.c_str());

		base::String 	country=m_localization.StrDef(_T("country"),_T("cn"));

		m_windowname=m_localization.StrDef(_T("windowname"), DOCUMENTS_SAVE_PATH);

		//m_localization.StrDef(_T("language"),_T("zh-cn"));
		 lstrcpy(windowname,m_windowname.c_str());


		m_runeDevIni.SetSection(_SC("Locale"));
		base::String	lang=m_runeDevIni.StrDef(_SC("Language"),_T("none"));
		if (lang==_T("none"))
		{
			lang=_T("en");
			m_runeDevIni.SetStr(_SC("Language"),_T("en"));


		}
	


		wsprintf(tempz,UPDATEINI,(LPCTSTR)country.c_str());

		zRet=FindZipItem(hz,tempz,true,&index,&ze);
		if (zRet==ZR_OK)
		{
			char* data=new char[ze.unc_size] ;
			DWORD len=ze.unc_size; 

			zRet=UnzipItem(hz,index,data,len);
			if (zRet==ZR_OK || zRet==ZR_MORE)
				m_ini.SetIniData(data,len);
			else
				m_ini.SetIniFile(OLDUPDATEINI);
			delete []data;

			base::String top=m_URLExt.StrDef(_SC("top"),_SC(""));
			if (top.length())
			{
				m_ini.SetStr(_SC("BrowserURLTop"),top.c_str());

			}
			base::String main=m_URLExt.StrDef(_SC("main"),_SC(""));
			if (main.length())
			{
				m_ini.SetStr(_SC("BrowserURL"),main.c_str());

			}



			SetToTable(m_ini,_T("Update"));

		}

		wsprintf(tempz,STRINGINI,(LPCTSTR)lang.c_str());
		zRet=FindZipItem(hz,tempz,true,&index,&ze);
		if (zRet==ZR_OK)
		{
			char* data=new char[ze.unc_size] ;
			DWORD len=ze.unc_size;

			zRet=UnzipItem(hz,index,data,len);
			if (zRet==ZR_OK || zRet==ZR_MORE)
				m_stringIni.SetIniData(data,len);
			else
				m_stringIni.SetIniFile(OLDUPDATEINI);
			delete []data;

			SetToTable(m_stringIni,_T("String"));

		}
		wsprintf(tempz,SERVERINI,(LPCTSTR)country.c_str());

		zRet=FindZipItem(hz,tempz,true,&index,&ze);
		if (zRet==ZR_OK)
		{
			char* data=new char[ze.unc_size] ;
			DWORD len=ze.unc_size;

			zRet=UnzipItem(hz,index,data,len);
			if (zRet==ZR_OK || zRet==ZR_MORE)
				m_serverIni.SetIniData(data,len);
			else
				m_serverIni.SetIniFile(OLDUPDATEINI);
			delete []data;

			SetToTable(m_serverIni,_T("Server"));

		}		

		CloseZip(hz);

		SquirrelObject root=SquirrelVM::GetRootTable();

		SquirrelObject tab=root.GetValue(_T("Localization"));


		tab.SetValue(_T("country"),country.c_str());
		tab.SetValue(_T("language"),lang.c_str());


	}




	



}

void internal_Update::PlayGame(void)
{





} 
base::String GetMyDocumentDirectory()
{
	TCHAR path[4096]={NULL};//_T("c:\\Documents and Settings\\Administrator\\My Documents\\");
	SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, path);

	return path;

}

void internal_Update::InitialDL(void)
                                                                                                                                                                                                      {

	
	IniFile2& updateini=m_ini;
	IniFile2& Versionini=m_versionIni;

	Versionini.SetSection(_T("BOOT"));
	updateini.SetSection(_T("BOOT"));

	m_strCurrentVersion	=  Versionini.Str(_T("Current_Version") ) ;

	m_iDonwloadPort=updateini.Int(_T("DownloadPort"));
	m_iDonwloadPort2=updateini.Int(_T("DownloadPort2"));

	if (m_port.length()>0)
		m_iDonwloadPort=_tstoi(m_port.c_str());


	m_clientName=updateini.Str(_T("ClientExe"));

	m_strZipPathURL=updateini.Str(_T("DownloadURL"));
	m_strZipPathURL2=updateini.Str(_T("DownloadURL2"));


// GKy 100325 #add P2P #test
#if _P2P_TEST
	base::String  str_;
	str_ = _T("ZipPathURL=") + m_strZipPathURL + _T(";ZipPathURL2=") + m_strZipPathURL2;

	MessageBox(NULL, str_.c_str(), _T("P2P"), MB_OK); // GKy #test
#endif


	m_URLExt.SetSection(_T("BOOT"));
	m_strProxyPathURL=m_URLExt.Str(_T("ProxyURL"));

	if (m_IP.length()>0)
		m_strZipPathURL=m_IP;


	IniFile2& updateState=m_UpdateListIni;


	updateState.SetSection(_T("Current Version"));

	base::String version=updateState.Str(_T("Version"));

	base::String path;
	VersionData last;
	VersionData client_now;
	last.SetData(version.c_str(),path);

	client_now.SetData(m_strCurrentVersion.c_str(),path);
	m_dlFiles=(last.m_version[3]-client_now.m_version[3]);
	_SetAllFiles(100,30,80);


	
	GetModuleFileName(NULL,szGameExePath,512);
	int len=lstrlen(szGameExePath);
	for(int i=len-1;i>0;--i)
	{
		if (szGameExePath[i]=='\\')
		{

			szGameExePath[i+1]=0;

			break;
		}

	}


	GetTempPath(sizeof(szWinTmpPath)/sizeof(TCHAR), szWinTmpPath);



	//base::String inTmpPathFile=base::String (szWinTmpPath)+_T("ROM-GameSettings.ini");

	base::String documentDir = GetMyDocumentDirectory();

	if (documentDir.length()>0)
	{
		documentDir =documentDir+_T("\\") + DOCUMENTS_SAVE_PATH + _T("\\");


		if (CreateDirectory(documentDir.c_str(),0))
		{
				SetFileAttributes(documentDir.c_str(),FILE_ATTRIBUTE_HIDDEN) ;

		}

		base::String  DocumentPath=documentDir+_T("GameSettings.ini");
	//	if (PathFileExists(DocumentPath.c_str()))
		{
			IniFile2 GameIni;

			if (path=="th")
			{
				if (client_now.m_version[3]!=1994&&(client_now.m_version[3]<2029))
					GameIni.SetUnicode(1);
			}
			else
			{
				if (client_now.m_version[3]<2029)
					GameIni.SetUnicode(1);
			}			
				
			GameIni.SetIniFile(DocumentPath);
			GameIni.SetSection(_T("Install"));
			GameIni.SetStr(_T("GamePath"),szGameExePath);

			GameIni.Save(DocumentPath.c_str(),true);
		//	GameIni.Save(inTmpPathFile.c_str(),true);
			
		}


	}





}
void internal_Update::SelectZone(int index)
{
  


	m_serverIni.SetSection(_T("TestServer"));
	LPCTSTR xIp=m_serverIni.Str(_T("IP"));
	int  xPort=m_serverIni.Int(_T("Port"));

	LPCTSTR Ip=m_runeDevIni.StrDef(_T("IP"),xIp);
	int  Port=m_runeDevIni.IntDef(_T("Port"),xPort);


	if (index==0&&lstrlen(xIp)==0)
		index=1;
		
		
	m_runeDevIni.SetSection(_T("Server"));


	m_selectZone=index;

	if (index>0) 
	{
		m_runeDevIni.SetSection(_T("Zone"));

		m_runeDevIni.SetInt(_T("Select"),m_selectZone);


		TCHAR Section[512];
		wsprintf(Section,_T("Server%d"),m_selectZone);

		m_serverIni.SetSection(Section);
		Ip=m_serverIni.StrDef(_T("IP"),xIp);
		Port=m_serverIni.IntDef(_T("Port"),xPort);

	}
	else
	{


		if (lstrlen(xIp)>0)
		{
			Ip=xIp;
			Port=xPort;

		}
		
	}
	m_runeDevIni.SetSection(_T("Server"));

	m_runeDevIni.SetStr(_T("Ip"),Ip);
	m_runeDevIni.SetInt(_T("Port"),Port);






	m_runeDevIni.Save(_T("RuneDev.ini"));


}

void internal_Update::MakePassWord(void)
{
	/*
	for (int i=0;i<128;i++)
		m_password[i]=0;

	for (int i=0;i<128;i++)
		m_password[i]=rand()%26+'A';


	m_password[0]='t';
	m_password[3]='v';
	m_password[1]='b';
	m_password[2]='s';
	for (int i=4;i<128;i+=2)
		m_password[i]=rand()%26+'a';
	m_password[1]--;



	for (int i=5;i<128;i+=2)
		m_password[i]=rand()%26+'A';
	m_password[2]--;
	for (int i=6;i<128;i+=3)
		m_password[i]=rand()%10+'0';
	m_password[3]--;
	*/
	strcpy( m_password, "?3np4l_15s_!H3r3" );

}

void SetToTable(IniFile2 &ini,LPCTSTR name)
{
	SquirrelObject root=SquirrelVM::GetRootTable();


	SquirrelObject tab=root.GetValue(name);

	SQObjectType sot = tab.GetType();
	if (sot != OT_TABLE ||sot== OT_NULL)
	{	
		tab=SquirrelVM::CreateTable();
		root.SetValue(name,tab);
	}

	CSimpleIni& _ini=ini.GetIni();
	CSimpleIni::TNamesDepend aSections;

	_ini.GetAllSections(aSections);

	CSimpleIni::TNamesDepend::const_iterator iSection = aSections.begin();
	for ( ; iSection != aSections.end(); ++iSection ) 
	{

		CSimpleIni::TNamesDepend aKeys;		
		_ini.GetAllKeys(iSection->pItem,aKeys);


		SquirrelObject Section=tab.GetValue(iSection->pItem);

		SQObjectType sectionType = Section.GetType();
		if (sectionType != OT_TABLE ||sectionType== OT_NULL)
		{	
			Section=SquirrelVM::CreateTable();
			tab.SetValue(iSection->pItem,Section);
		}

		CSimpleIni:: TNamesDepend::const_iterator iKey = aKeys.begin();
		for ( ; iKey != aKeys.end(); ++iKey) 
		{
			CSimpleIni::TNamesDepend aValues;
			_ini.GetAllValues(iSection->pItem, iKey->pItem, aValues);

			CSimpleIni::TNamesDepend::const_iterator iValue = aValues.begin();
			if (aValues.size()>1)
			{	

				SquirrelObject oarray=SquirrelVM::CreateArray(0);
				for ( ; iValue != aValues.end(); ++iValue) 
				{

					oarray.ArrayAppend(iValue->pItem);
				}
				Section.SetValue( iKey->pItem,oarray);
			}
			else
			{

				if (iValue != aValues.end())
				Section.SetValue( iKey->pItem,iValue->pItem);

			}

		}

	}

	


		
}
void internal_Update::InitialThread(void)
{
	//if (m_download)
	//	m_download=0;

	

	m_download=new downloadFile(m_dl);
}
void internal_Update::InitialCheckMD5Thread(void)
{
	m_md5Thread=new CheckMd5(this);

}

void internal_Update::InitialExecThread(void)
{
	//if (m_execfile)
	//	delete m_execfile;



	m_execfile=new execFile();
}

void internal_Update::FinisThread(void )
{
	
	Finis();

	//timer1->Enabled = false;
	if( m_download) 
	{
		

		for( int i = 0 ; i < 200 ; i++ )
		{
			if(	   m_download->alive() == false  )
			
				break;
			 m_download->detach();
			//m_dl->Break();
			Sleep( 100);

		}
		
		m_download=0;
	}

	if( m_execfile)
	{
		

		for( int i = 0 ; i < 200 ; i++ )
		{
			if(	 m_execfile&&  m_execfile->alive() == false  )
				break;
			Sleep( 100);
		}
		
		m_execfile=0;
	}

	if( m_md5Thread)
	{


		for( int i = 0 ; i < 200 ; i++ )
		{
			if(	 m_md5Thread&&  m_md5Thread->alive() == false  )
				break;
			Sleep( 100);
		}

		m_md5Thread=0;
	}
	if (m_dl)
	{
		
		delete m_dl;
	}
	m_dl=NULL;



	// GKy 100412-9261L1190 ---------------------------------------------
	if( NULL != m_tLsc){
		for( int i = 0 ; i < 200 ; i++ )
		{
			if( m_tLsc && m_tLsc->alive() == false )
				break;

			m_tLsc->detach();

			Sleep( 100);
		}
		m_tLsc=0;
	}

	//---------------------
	// GKy 100429 try to end P2P downloader.
	// GKy 100429 need done after stop the thread m_tLsc. avoid m_tLsc start a new one.

	//EnumWindows((WNDENUMPROC)EnumWindowsProc_vcP2P,NULL);  
	EnumWindows((WNDENUMPROC)EnumWindowsProc_vcP2P_close,NULL);  

	// GKy 100430
	// GKy 100503 change as done in EnumWindowsProc_vcP2P
	//if( NULL != g_hwndP2Pdownloader){
	//	::PostMessage(g_hwndP2Pdownloader, WM_CLOSE, NULL, NULL); // 
	//}//if

	// -- GKy 100412-9261L1190 ---------------------------------------------



}
void internal_Update::RunExecfile(void)
{
	InitialExecThread();
	m_execfile->start();
}
void internal_Update::RunDownload(void)
{

// GKy 100325 #add P2P
#if _P2P_TEST
	MessageBox(NULL, _T(" internal_Update::RunDownload(void) "), _T("P2P"), MB_OK); // GKy #test
#endif


	InitialThread();
	m_download->start();


}

void internal_Update::RunCheckMD5(void)
{
	InitialCheckMD5Thread();
	m_md5Thread->start();


}

void internal_Update::DoState(int state)
{

	switch (state)
	{


	case EM_MD5CheckOK:

		RunExecfile();



		break;
	case EM_UpdateOK:


		break;


	}
}


void internal_Update::Initial(TCHAR* IP,TCHAR* port,TCHAR* windowname)
{
	TCHAR Initemp[1024];

	m_password=new char[128];

	MakePassWord();
	m_password[16]=0;

	m_IP=IP;
	m_port=port;




	


	m_localization.SetUnicode(1);
	m_install.SetUnicode(1);
	m_ini.SetUnicode(1);
	m_versionIni.SetUnicode(1);
	m_stringIni.SetUnicode(1);
	m_serverIni.SetUnicode(1);


	m_country.SetUnicode(1);
	


	m_runeDevIni.SetIniFile( _T("RuneDev.ini") );

	m_runeDevIni.SetSection(_T("Zone"));

	m_selectZone=m_runeDevIni.IntDef(_T("Select"),0);


	m_URLExt.SetIniFile(_SC("www.ini"));


	m_runeDevIni.SetSection(_T("Path"));


	base::String inipath=m_runeDevIni.Str(_T("Resource"));

	


	SetToTable(m_runeDevIni,_T("runeDev"));



	int len=inipath.length();
	if (len>=2)
	{
		int pos=inipath.lastIndexOf(_T('\\'));
		if (pos!=len-1)
			inipath+=_T("\\");


		wsprintf(Initemp,_T("%s%s"),inipath.c_str(),CONFIGINI);
	}
	else
		lstrcpy(Initemp,CONFIGINI);

	m_savePath=Initemp;
	m_configIni.SetIniFile(Initemp);


	m_configIni.SetSection(_T("character detail")); 

	int i=m_configIni.IntDef(_T("paperdoll detail"),0);

	m_configIni.SetInt(_T("paperdoll detail"),6-i);

		m_configIni.SetSection(_T("visual detail")); 

	 i=m_configIni.IntDef(_T("rtlightmap size"),0);

	m_configIni.SetInt(_T("rtlightmap size"),i/128);



	SetToTable(m_configIni,_T("config"));

	TCHAR tempz[512];







base::String select=_SC("sg");
base::String country=_SC("sg");
base::String lang=_SC("en");


 

	{
		//¸ÑÀ£ÁY		
		HZIP	hz; 
		ZRESULT	zRet;	
		hz = OpenZip( UPDATEINF ,m_password );

		ZIPENTRY ze; 

		if (hz)
		{
			
			int index;

			zRet=FindZipItem(hz,_T("Install.ini"),true,&index,&ze);
			if (zRet==ZR_OK)
			{
				char* data=new char[ze.unc_size] ;
				DWORD len=ze.unc_size;

				zRet=UnzipItem(hz,index,data,len);
				if (zRet==ZR_OK || zRet==ZR_MORE)
					m_install.SetIniData(data,len);
				else
					m_install.SetIniFile(_T("Install.ini"));
				delete []data;

				SetToTable(m_install,_T("Install"));
			}
			else
			{
				SquirrelObject root=SquirrelVM::GetRootTable();
				SquirrelObject tab=SquirrelVM::CreateTable();
				root.SetValue(_T("Install"),tab);
				SquirrelObject sc=SquirrelVM::CreateTable();
				tab.SetValue(_T("BOOT"),sc);
				sc.SetValue(_T("first"),1);
				sc.SetValue(_T("select"),_T("eneu"));
			
			}
			zRet=FindZipItem(hz,_T("Localization.ini"),true,&index,&ze);
			if (zRet==ZR_OK)
			{
				char* data=new char[ze.unc_size] ;
				DWORD len=ze.unc_size;

				zRet=UnzipItem(hz,index,data,len);
				if (zRet==ZR_OK || zRet==ZR_MORE)
				{
					m_localization.SetIniData(data,len);
					m_Select=true;

				}
				else
				{
					m_localization.SetIniFile(_T("Localization.ini"));
					m_Select=false;

				}
				delete []data;
				
				SetToTable(m_localization,_T("Localization"));

			}


			m_install.SetSection(_T("BOOT"));

			select=m_install.StrDef(_T("select"),_T("enus"));

			m_localization.SetSection(select.c_str());

			country=m_localization.StrDef(_T("country"),_T("cn"));

			 lang=m_localization.StrDef(_T("language"),_T("zh-cn"));


			 m_runeDevIni.SetSection(_SC("Locale"));
			 base::String	lang2=m_runeDevIni.StrDef(_SC("Language"),_T("none"));
			 if (lang2==_T("none"))
			 {
				
				 m_runeDevIni.SetStr(_SC("Language"),lang.c_str());


			 }
			 else
				 lang=lang2;



			 m_windowname=m_localization.StrDef(_T("windowname"), DOCUMENTS_SAVE_PATH);

			 lstrcpy(windowname,m_windowname.c_str());
			
			wsprintf(tempz,UPDATEINI,(LPCTSTR)country.c_str());
			
			zRet=FindZipItem(hz,tempz,true,&index,&ze);
			if (zRet==ZR_OK)
			{
				char* data=new char[ze.unc_size] ;
				DWORD len=ze.unc_size;

				zRet=UnzipItem(hz,index,data,len);
				if (zRet==ZR_OK || zRet==ZR_MORE)
					m_ini.SetIniData(data,len);
				else
					m_ini.SetIniFile(OLDUPDATEINI);
				delete []data;

				base::String top=m_URLExt.StrDef(_SC("top"),_SC(""));
				if (top.length())
				{
					m_ini.SetStr(_SC("BrowserURLTop"),top.c_str());
				
				}
				base::String main=m_URLExt.StrDef(_SC("main"),_SC(""));
				if (main.length())
				{
					m_ini.SetStr(_SC("BrowserURL"),main.c_str());

				}


				SetToTable(m_ini,_T("Update"));

			}

			zRet=FindZipItem(hz,VERSIONINI,true,&index,&ze);
			if (zRet==ZR_OK)
			{
				char* data=new char[ze.unc_size] ;
				DWORD len=ze.unc_size;

				zRet=UnzipItem(hz,index,data,len);
				if (zRet==ZR_OK || zRet==ZR_MORE)
					m_versionIni.SetIniData(data,len);
				else
					m_versionIni.SetIniFile(OLDUPDATEINI);
				delete []data;

				SetToTable(m_versionIni,_T("Version"));

			}

			wsprintf(tempz,STRINGINI,(LPCTSTR)lang.c_str());
			zRet=FindZipItem(hz,tempz,true,&index,&ze);
			if (zRet==ZR_OK)
			{
				char* data=new char[ze.unc_size] ;
				DWORD len=ze.unc_size;

				zRet=UnzipItem(hz,index,data,len);
				if (zRet==ZR_OK || zRet==ZR_MORE)
					m_stringIni.SetIniData(data,len);
				else
					m_stringIni.SetIniFile(OLDUPDATEINI);
				delete []data;

				SetToTable(m_stringIni,_T("String"));

			}

			wsprintf(tempz,SERVERINI,(LPCTSTR)country.c_str());

			zRet=FindZipItem(hz,tempz,true,&index,&ze);
			if (zRet==ZR_OK)
			{
				char* data=new char[ze.unc_size] ;
				DWORD len=ze.unc_size;

				zRet=UnzipItem(hz,index,data,len);
				if (zRet==ZR_OK || zRet==ZR_MORE)
					m_serverIni.SetIniData(data,len);
				else
					m_serverIni.SetIniFile(OLDUPDATEINI);
				delete []data;

				SetToTable(m_serverIni,_T("Server"));

			}		


			zRet=FindZipItem(hz,_T("country.ini"),true,&index,&ze);
			if (zRet==ZR_OK )
			{
				char* data=new char[ze.unc_size] ;
				DWORD len=ze.unc_size;

				zRet=UnzipItem(hz,index,data,len);
				if (zRet==ZR_OK || zRet==ZR_MORE)
				{
					m_country.SetIniData(data,len);
					m_Select=true;
				}
				else
				{
					m_country.SetIniFile(OLDUPDATEINI);
					m_Select=false;
				}
				delete []data;

				
			}		
			else

			{

				m_country.SetSection(_T("BOOT"));
				m_country.SetStr(_T("list"),_T("eneu:International (English),de:DE/AT/CH (Deutsch),enus:USA (English)"));


			}
			SetToTable(m_country,_T("Country"));



			CloseZip(hz);
		}


		SquirrelObject root=SquirrelVM::GetRootTable();

		SquirrelObject tab=root.GetValue(_T("Localization"));

	
		tab.SetValue(_T("country"),country.c_str());
		tab.SetValue(_T("language"),lang.c_str());

	


	

		m_ini.SetSection(_T("BOOT"));

		 m_dlPath=m_ini.StrDef(_T("ZipPath"),_T("Download"));

		if (m_dlPath.at(m_dlPath.length()-1)!='\\')
			m_dlPath+=_T("\\");


			wsprintf(Initemp,_T("%sVersion.txt"),m_dlPath.c_str());

		

			m_UpdateListIni.SetIniFile(Initemp);
	



	}

		SelectZone(m_selectZone);

	m_dl=new CFileDownLoad();


	

}



void _SetProgressState(int state)
{

	g_update->DoState(state);


	SqPlus::SquirrelFunction<void> func(_T("SetProgressState"));
	func(state);


}
void CheckMd5::run()
{
	int ret=m_p->CheckFileMD5();
	if (!ret) 
	{
		
	}



	g_pManager->KillTimer(g_child,101);

	Sleep(200);

	if (ret>=0)
	{



		g_pManager->PostMessageX(WM_MD5OK);
	}
	else
	{
		static int count=3;
		if (count<0)
			g_pManager->PostMessageX(WM_ExecFail);
		else
			g_pManager->PostMessageX(WM_MD5Fail);
		count--;

		
	}


	//_SetProgressState(EM_DownloadingOK);
}
void downloadFile::run()
{
	int ret=m_dl->DownLoad(3,0);
	if (!ret) 
	{
		int len=lstrlen(m_dl->m_URL2);
		if (len>0)
		{

			ret=m_dl->DownLoad(3,1);
		}

	}
	g_pManager->KillTimer(g_child,100);

	Sleep(200);

	if (ret>=0)
	{

		

		g_pManager->PostMessageX(WM_DLOK);
	}
	else
	{
		

		
		g_pManager->PostMessageX(WM_DLFail);

	}
		
	
//_SetProgressState(EM_DownloadingOK);
}
// GKy 100409 3301L1685 -------------------------------------------------
void cThreadLSC::run()
{
#ifdef _P2P_APPEND

	enum DL_RESULT {
		DL_RESULT_INIT = 0,
		DL_RESULT_OK,
		DL_RESULT_FAIL,

		DL_RESULT_RESTART_AS_NO_P2P, // GKy 100503 add for P2P can`t start case(Like no torrent file)

		DL_RESULT_MAX
	};

	//int ret = 0;
	DL_RESULT ret = DL_RESULT_INIT;

	//float prg = 0.0f;
	do{
		// -1. Check if downloader still alive; GKy 100423 move Later for download complete case
		//////////////////////////////////////////////////////////////////

		// GKy 100503 move to end part, no need Loop always
		//EnumWindows((WNDENUMPROC)EnumWindowsProc_vcP2P,NULL);  


		// 0. Update data
		gLscInfo.IPCread();

		// 2. Check to state, if Complete, do next task
		switch( gLscInfo.getState() ){

			case LSC_TORRENT_STATE_DOWNLOAD_COMPLETE:
				gLscInfo.IPCreset();
				//ret = 1;
				ret = DL_RESULT_OK;
				break;


			case LSC_TORRENT_STATE_OFF: // P2P can`t run, turn off, reStart as old way.
				gLscInfo.IPCreset();
				ret = DL_RESULT_RESTART_AS_NO_P2P;

				// GKy 100503 indeed need delete P2P template file, BUT now only handle no torrent file case, pass.

				break;


			//++++++++++++++++ Case will reStart vcP2P.exe ++++++++++++++++++++++++++++
			default:
			case LSC_TORRENT_STATE_NA: // GKy 100426
			case LSC_TORRENT_STATE_DOWNLOADING:
			case LSC_TORRENT_STATE_LOADING_FILE: // GKy 100428


				// 2.5 Check if downloader still alive
				//////////////////////////////////////////////////////////////////
				// GKy 100422 if find previous existed, need 1.terminate/reStart 2.use it.


				// GKy 100423 try
				if( g_hP2PdownloaderInstance ){
					if( ::CloseHandle(g_hP2PdownloaderInstance) ){
						g_hP2PdownloaderInstance = NULL; // GKy 100423
					}
				}


				////g_hP2PdownloaderInstance = ::CreateMutex(NULL, TRUE, _T("vcP2P")); // bInitialOwner = true for Release
				g_hP2PdownloaderInstance = ::CreateMutex(NULL, FALSE, _T(LSC_VCP2P_MUTEX)); // GKy 100503
				//g_hP2PdownloaderInstance = ::CreateMutex(NULL, FALSE, _T("vcP2P")); // GKy 100423
				if(GetLastError() == ERROR_ALREADY_EXISTS)  
				{  
					//do nothing
					//Sleep(10);
					//getchar(); //#test

				}else{

					//getchar(); //#test

					// GKy 100423 try must Release: --------- 
					if( g_hP2PdownloaderInstance ){

						for( int i=0; i < 10; i++ ){

							//if( ::ReleaseMutex(g_hP2PdownloaderInstance) ){
							if( ::CloseHandle(g_hP2PdownloaderInstance) ){

								g_hP2PdownloaderInstance = NULL; // GKy 100423

								Sleep(200); // GKy 100427 avoid reStart too quickly

								//reStart the App.
								VC_RunP2Pdownloader();

								Sleep(800); // GKy 100503 add for wait downloader start

								break;

							}else{
								// Release FLR

								//Sleep(10);

							}//if

						}//for

					}//if

				}//if

				//////////////////////////////////////////////

				break; // case LSC_TORRENT_STATE_DOWNLOADING:



			case LSC_TORRENT_STATE_ERROR:
				gLscInfo.IPCreset();
				//ret = 2;
				ret = DL_RESULT_FAIL;
				break; // case LSC_TORRENT_STATE_NA:


			//default: // as auto reStart case?..
				//break;

		}//switch

		Sleep(200);

	//}while( ret != 1 );
	//}while( 0 == ret );
	}while( DL_RESULT_INIT == ret );

	


	//////////////////////////////////////////////////////////

	Sleep(200);

	/////////////////////////////

	switch(ret){

		case DL_RESULT_OK:
			g_pManager->PostMessageX(WM_DLOK);
			break;

		case DL_RESULT_FAIL:
			{
				// GKy 100511 try show some Error msg

				////TCHAR temp[1000];
				//LPCTSTR pr =GetString( _T("BOOT"),_T("MSG_DOWNLOAD_FAIL"));
				////wsprintf(temp,pr,m_patchFile.c_str());
				//_MessageBox(_T("Download File"),pr,_T("Error"),0); // 

				_MessageBox(_T("Fatal Error"),GetString( _T("BOOT"),_T("MSG_DOWNLOAD_FAIL") ),_T("Error"),0);


			}

			g_pManager->PostMessageX(WM_DLFail);
			break;


		case DL_RESULT_RESTART_AS_NO_P2P:
			//g_pManager->PostMessageX(WM_DLOK);

			g_bEnableP2P = false;

			// GKy 100511 try remove P2P download template file
			// GKy 100511 try done in vcP2P.exe? have some problem..TBC..

			g_update->RunDownload(); // GKy 100503 start OLD DL process.

			break;


		default:
			break;

	}//switch


#endif //#ifdef _P2P_APPEND

	//////////////////////////////////////////////
	//////////////////////////////////////////////
	//////////////////////////////////////////////


}
// -- GKy 100409 3301L1685 -------------------------------------------------


void execFile::run()
{

	//if (g_update->CheckFileMD5()>0)
	

	if (g_update->DLPatchEND())
	{

		g_pManager->PostMessageX(WM_ExecOK);

		runLauncher=TRUE;
	
	}
	else
	{
		g_pManager->PostMessageX(WM_ExecFail);
		

	}

	
	
	
}

void internal_Update::CheckNeedDLPatch()
{
	BOOL ret=0;

	if(!m_bIsClose)
	{	
		
		ret=CheckVersion();
		if (ret==FALSE)
		{	
			
			if (DownloadFile()==false)
			{
				_MessageBox(_T("Version.txt"),GetString( _T("BOOT"),_T("MSG_VERSION_ERROR") ),_T("Error"),0);

			}
			else
			{


// GKy 100330 #add P2P #test
#if _P2P_TEST
				base::String  str_;
				str_ = _T(" _SetProgressState(EM_Downloading) "); // + path;

				MessageBox(NULL, str_.c_str(), _T("P2P"), MB_OK); // GKy #test
#endif




				g_pManager->SetTimer(g_child,100,1000);
				_SetProgressState(EM_Downloading);
			}

		}
		else if (ret==-1)
		{
			g_pManager->PostMessageX(WM_UpdateOK);


			_SetProgressState(EM_UpdateFailed);
		}
		else if (ret==-2)
		{
			g_pManager->PostMessageX(WM_UpdateOK);


			_SetProgressState(EM_UpdateFailed);
		}
		else
		{
			g_pManager->PostMessageX(WM_UpdateOK);
			

			_SetProgressState(EM_UpdateOK);
		}

	}

	if (m_selectZone>0&&m_selectZone<=4)
		_SetCheckZone(m_selectZone);
}
BOOL internal_Update::DLPatchEND(void)
{

	
	return ExecuteFile(m_patchFilePath.c_str());
	//SetNewVersion();
	
}


BOOL internal_Update::CheckVersion()
{
	//ReloadUpdateInf();

	IniFile2& Versionini=m_versionIni;

	IniFile2& updateini=m_ini;
	IniFile2& updateState=m_UpdateListIni;


	base::String path;
	base::String path1;
	VersionData versiondata;

	VersionData curdata;




	Versionini.SetSection(_T("BOOT"));
	updateini.SetSection(_T("BOOT"));
	_ProcPostMessage(  GetString( _T("BOOT"),_T("MSG_CHECK_VERSION") ));

	BOOL ret=FALSE;

	_SetProgressState(EM_CheckVersion);


	m_strCurrentVersion	=  Versionini.Str( _T("Current_Version" )) ;

	updateState.SetSection(_T("Current Version"));

	base::String version=updateState.Str(_T("Version"));

	versiondata.SetData(version.c_str(),path);
	curdata.SetData(m_strCurrentVersion.c_str(),path1);

	bool bRegion=path1!=path; 

	if (bRegion)
	{
		_MessageBox(_T("Region"),GetString( _T("BOOT"),_T("MSG_REGION_ERROR") ),_T("Error"),0);
	//	_ProcPostMessage( GetString( _T("BOOT"),_T("MSG_REGION_ERROR") ));
		return -1;
	}


	if (version.length()<=0||!bRegion)
	{

		updateState.SetSection(_T("Begin Version"));
		version=updateState.Str(_T("Version"));
	}


	if (version.length() <=0 ||m_strCurrentVersion.length()<=0 )
	{
	//	_ProcPostMessage( GetString( _T("BOOT"),_T("MSG_VERSION_DL_FAIL") ));
		_SetProgressState(EM_UpdateFailed);
		ret=TRUE;
	}

	
	DWORD V1,V2;
	V1=(versiondata.m_version[0]&0xf)<<28|
		(versiondata.m_version[1]&0xf)<<24|
		(versiondata.m_version[2]&0xff)<<16|
		(versiondata.m_version[3]&0xffff);

	V2=(curdata.m_version[0]&0xf)<<28|
		(curdata.m_version[1]&0xf)<<24|
		(curdata.m_version[2]&0xff)<<16|
		(curdata.m_version[3]&0xffff);

	if (V1==V2)
	//if (version==m_strCurrentVersion)
	{

		_ProcPostMessage( GetString( _T("BOOT"),_T("MSG_START_GAME") ));
		_SetProgressState(EM_UpdateOK);
		ret=TRUE;

	}
	else if (V1<V2)
	{

		_MessageBox(_T("Version"),GetString( _T("BOOT"),_T("MSG_VERSION_BETTER") ),_T("Error"),0);
		//_ProcPostMessage( GetString( _T("BOOT"),_T("MSG_VERSION_BETTER") ));
	//	_SetProgressState(EM_UpdateOK);
		ret=-2;
	}







	//m_bIsUpDateReady=m_state.Int("ready");

	return ret;


}

void internal_Update::SetNewVersion(void)
{

	base::String path;
	VersionData next;
	VersionData client_now;

	IniFile2& Versionini=g_update->m_versionIni;

	IniFile2& updateini=g_update->m_ini;
	IniFile2& updateState=g_update->m_UpdateListIni;

	Versionini.SetSection(_T("BOOT"));

	updateState.SetSection(m_strCurrentVersion.c_str());
	base::String version=updateState.Str(_T("Version"));



	next.SetData(version.c_str(),path);
	client_now.SetData(m_strCurrentVersion.c_str(),path);


	int count=next.m_version[3]-client_now.m_version[3];

	//_SetTotalStep(count);
	m_dlFiles-=count;

	Versionini.SetStr(_T("Current_Version"),version.c_str());

	_SetProgressState(EM_VersionOK);
}


bool internal_Update::DownloadFile()
{

	bool ret=true; // origin


	TCHAR temp[512];
	TCHAR WMD5[128];
	IniFile2& Versionini=m_versionIni;

	IniFile2& updateini=m_ini;
	IniFile2& updateState=m_UpdateListIni;

	updateState.SetSection(m_strCurrentVersion.c_str());

	base::String path=updateState.Str(_T("DownloadPath"));


// GKy 100325 #add P2P #test
#if _P2P_TEST
	base::String  str_;
	str_ = _T("path,DownloadPath=") + path;

	MessageBox(NULL, str_.c_str(), _T("P2P"), MB_OK); // GKy #test
#endif



	if (path.length()==0)
	{
		updateState.SetSection(_T("Pren Version"));
		m_strCurrentVersion=updateState.Str(_T("Version"));

		updateState.SetSection(m_strCurrentVersion.c_str());
		path=updateState.Str(_T("DownloadPath"));

		return false;

	}






	path+=_T("/");

	m_patchFile=updateState.Str(_T("DownloadFile"));
/*
	TCHAR exeFile[1024];

	wsprintf(exeFile,_SC("%s%s"),szWinTmpPath,m_patchFile.c_str());


	
*/
	m_patchFilePath=m_patchFile;
	base::String checkMd5=updateState.Str(_T("MD5"));
	lstrcpy(WMD5,checkMd5.c_str());



// GKy 100325 #add P2P #test
#if _P2P_TEST
	str_ = _T("m_patchFilePath=") + m_patchFilePath;
	MessageBox(NULL, str_.c_str(), _T("P2P"), MB_OK); // GKy #test
#endif
	// GKy 100426 try setup torrent file Name.
	if( g_wsTorrenFile == _T("") ){

		//g_wsTorrenFile = m_patchFilePath.substring(0, m_patchFilePath.length()-4 ); 
		//g_wsTorrenFile = g_wsTorrenFile + _T(".torrent");

		g_wsTorrenFile = m_patchFilePath + _T(".torrent"); // GKy 100503

	}//if


	
	int len=lstrlen(WMD5);
	len = WideCharToMultiByte(CP_ACP, 0, WMD5, len, m_MD5, 256, NULL, NULL);

	

	m_MD5[len]=0;

	strlwr(m_MD5);


	base::String strZipPathURL=m_strZipPathURL+path;
	base::String strZipPathURL2=m_strZipPathURL2+path;
	base::String strProxyURL=m_strProxyPathURL;

	wsprintf(temp,_T("DL:%s"),m_patchFile.c_str());

	_ProcFileText(temp);

	m_strOnDownloadFileName=temp;


// GKy 100325 #add P2P #test
#if _P2P_TEST
	str_ = _T("m_patchFile=") + m_patchFile;
	MessageBox(NULL, str_.c_str(), _T("P2P"), MB_OK); // GKy #test


	str_ = _T("m_strOnDownloadFileName=") + m_strOnDownloadFileName;
	MessageBox(NULL, str_.c_str(), _T("P2P"), MB_OK); // GKy #test
#endif



	_SetProgressState(EM_Downloading);

	////////////////////////////////////////////////////////////


	m_sTime=timeGetTime();

	m_dl->SetFile(m_patchFile.c_str(),m_patchFile.c_str());

	//GetTempPath(sizeof(szWinTmpPath)/sizeof(TCHAR), szWinTmpPath);
//	m_dl->SetDownloadPath(szWinTmpPath);
	//m_dl->SetDownloadPath(szWinTmpPath);
	m_dl->SetDownloadPath(_T(""));



	m_dl->SetURL(strZipPathURL.c_str(),m_iDonwloadPort);
	m_dl->SetURL2(strZipPathURL2.c_str(),m_iDonwloadPort2);
	m_dl->SetProxy(strProxyURL.c_str());


	if( !g_bEnableP2P ){

		//#ORIGIN


		RunDownload(); // GKy 100326 #test hide OLD DL

	}else{

		//#P2P

		////////////////////////////////////////////////////////////////////

// GKy 100325 #add P2P #test
#if _P2P_TEST
		str_ = _T("strZipPathURL=") + strZipPathURL;
		MessageBox(NULL, str_.c_str(), _T("P2P"), MB_OK); // GKy #test


		str_ = _T("strZipPathURL2=") + strZipPathURL2;
		MessageBox(NULL, str_.c_str(), _T("P2P"), MB_OK); // GKy #test
#endif

		//MessageBox(NULL, _T("internal_Update::DownloadFile bf VC_RunP2Pdownloader() "), _T("P2P"), MB_OK); // GKy #test

		g_wsWebSeed = (TCHAR*)strZipPathURL.c_str(); // need keep this value for reStart.


		//------------------------------------------------------------------------------
		// GKy 100503 no matter if vcP2P.exe duplicated, here ensure need start a whole new vcP2P.exe
		//     So must Terminate residual ones.
		EnumWindows((WNDENUMPROC)EnumWindowsProc_vcP2P_close,NULL);  
		Sleep(100);

		//-------------------------------------------------------------------------------
		// GKy 100510 Add more checking, if run vcP2P.exe fail (not existed etc.)

		// GKy 100510 besides executed in thread, here as initial starting
		//if( VC_RunP2Pdownloader() ){ // removed new vcp2p downlaoder using old one instead
		if( false ) {
			Sleep(800); // GKy 100426 wait P2P downloader update IPC file.

			////

	#if _P2P_TEST
			MessageBox(NULL, _T("internal_Update::DownloadFile bf call/start m_tLsc thread "), _T("P2P"), MB_OK); // GKy #test
	#endif

			//Sleep(1000); // GKy 100409 add this avoid crash when init stage: X:not worked
			/////////////////////////////////////////////////////////////////////

	#ifdef _P2P_APPEND
			m_tLsc = new cThreadLSC(); // GKy 100422
			m_tLsc->start();
	#endif

		}else{
			// FLR. run Back as OLD

			g_bEnableP2P = false;

			//g_update->RunDownload(); // GKy 100510 start OLD DL process.
			RunDownload();

		}// if( VC_RunP2Pdownloader() ){


	}// if( g_bEnableP2P ){



	return ret;
}
BOOL internal_Update::ExecFile(void)
{
	BOOL ret=0;
	return ret;
}
BOOL internal_Update::CheckFileMD5(void)
{
	BOOL ret=0;
	/*
	if (!result)	
	{
		TCHAR temp[1000];
		wsprintf(temp,_T("File %s Not Find"),m_patchFile.c_str());

		_MessageBox(NULL,temp,_T("Error"),0);
	}
	if (result)*/
	{

		TCHAR temp[512];
		char fileMd5[1024];		

		int size=m_dl->TotalSize();
		int etime=timeGetTime()-m_sTime;

		m_dlsec=size/((etime+1));

		//scsprintf(temp,_T("(%f kbs) "),m_dl->GetDLSpeed()/1024);

		scsprintf(temp,_T("%s"),GetString(_T("BOOT"),_T("MSG_MD5_CHECKING")));
		_ProcPostMessage(temp);

		
		CMD5Checksum::GetMD5(m_patchFilePath.c_str(),fileMd5,&g_MD5_prog);

		if ( !strcmp(fileMd5,m_MD5)||strlen(m_MD5)==0)
		{

			_SetProgressBarMax(100);
			_SetProgressBarPos(100);
			
			

			if (strlen(m_MD5))
			{
				
				_ProcPostMessage( GetString( _T("BOOT"),_T("MSG_MD5_OK") ));
			}
			

		

			ret=TRUE;
		}
		else if (size>10240) 
		{

 

			static int g_count=0;

			g_count++;
			if (g_count>=3)
			{
				TCHAR temp[1000];
				LPCTSTR pr =GetString( _T("BOOT"),_T("MSG_FILE_MD5_ERROR"));
				wsprintf(temp,pr,m_patchFile.c_str());

				

				_MessageBox(_T("MD5"),temp,_T("Error"),0);
				
				ret=-1;

			}
			SetFileAttributes(m_patchFilePath.c_str(),FILE_ATTRIBUTE_NORMAL) ;
			if (DeleteFile(m_patchFilePath.c_str())==0)
			{

				TCHAR temp[1000];

				LPCTSTR pr =GetString( _T("BOOT"),_T("MSG_FILE_DELETE_ERROR"));
				
				wsprintf(temp,pr,m_patchFile.c_str());
				_MessageBox(_T("Delete File"),temp,_T("Error"),0);

			}

		
		}
		else 
		{

			TCHAR temp[1000];
			
			LPCTSTR pr =GetString( _T("BOOT"),_T("MSG_FILE_DOWNLOAD_ERROR"));
			wsprintf(temp,pr,m_patchFile.c_str());

			_MessageBox(_T("Dowload File"),temp,_T("Error"),0);

			if (DeleteFile(m_patchFilePath.c_str())==0)
			{

				TCHAR temp[1000];
				LPCTSTR pr =GetString( _T("BOOT"),_T("MSG_FILE_DELETE_ERROR"));
			
				wsprintf(temp,pr,m_patchFile.c_str());
				

				_MessageBox(_T("Delete File"),temp,_T("Error"),0);

			}

		}
	}




	return ret;
}


void UpdateIni_Load(TCHAR* IP,TCHAR* port,TCHAR* windowname)
{	
	if (g_update)
		delete g_update;
	g_update=new internal_Update();
	g_update->Initial(IP,port,windowname);	
}
void UpdateIni_Finish(void)
{	





	g_update->SelectZone(g_update->m_selectZone);
	if (g_update)
	delete g_update;
	g_update=NULL;
}


void GetTableToIni(LPCTSTR Section,IniFile2& ini,SquirrelObject &table)
{
 
	SquirrelObject key,value;
	if(table.BeginIteration())
	{
		while(table.Next(key,value)) 
		{
			if (value.GetType()==OT_TABLE)
			{				
				GetTableToIni(key.ToString(),ini,value); 
			}
			else
			{
				if (Section)
				ini.SetSection(Section);
				ini.SetStr(key.ToString(),value.ToString());
			}
		}
		table.EndIteration();
	}

}
bool VC_IsFirstInstall(void)
{
	IniFile2& install=g_update->m_install;

	if (g_update->m_Select==false)
		return false;

	return install.IntDef(_T("first"),1)? true:false;


}

void VC_FinisLauncher(void)
{
g_update->FinisThread();

} 
  
 
void SetCountryAndLanguage(const SQChar* country,const SQChar* language,TCHAR *windowname)
{

	IniFile2 &rusave=g_update->m_runeDevIni;

	IniFile2& install=g_update->m_install;
	
	SquirrelObject root=SquirrelVM::GetRootTable();

	SquirrelObject rutable=root.GetValue(_T("runeDev"));


	rusave.SetSection(_SC("Locale"));
	rusave.SetStr(_SC("Language"),language);

		SquirrelObject sqtable=root.GetValue(_T("Install"));
		SquirrelObject tab=sqtable.GetValue(_T("BOOT"));

		tab.SetValue(_T("select"),country);

	tab.SetValue(_T("first"),0);

	rusave.Save(_T("RuneDev.ini"));

g_update->UpgradeZip(); 



g_update->LoadUpdate(windowname);
}

	 

void RuneConfig_Save(void)
{

	IniFile2 &save=g_update->m_configIni;
	IniFile2 &rusave=g_update->m_runeDevIni;
	SquirrelObject root=SquirrelVM::GetRootTable();

	SquirrelObject sqtable=root.GetValue(_T("config"));


	GetTableToIni(NULL,save,sqtable);

	save.SetSection(_T("character detail")); 

	int i=save.IntDef(_T("paperdoll detail"),6);

	save.SetInt(_T("paperdoll detail"),6-i);

	save.SetSection(_T("visual detail")); 

	 i=save.IntDef(_T("rtlightmap size"),1);

	save.SetInt(_T("rtlightmap size"),i*128);



		save.Save(g_update->m_savePath.c_str());
	SquirrelObject rutable=root.GetValue(_T("runeDev"));




	GetTableToIni(NULL,rusave,rutable);

	

	rusave.Save(_T("RuneDev.ini"));


}


void  _RunCheckMD5(void)
{


	SquirrelObject root=SquirrelVM::GetRootTable();
	SquirrelObject obh=root.GetValue(_T("g_curlProgressbar"));



	CControlUI *child=(CControlUI*)obh.GetInstanceUP(0);

	g_child=child;

	g_pManager=g_child->GetManager();



	//g_update->RunDownload();
	g_pManager->SetTimer(g_child,101,500);
	_SetProgressState(EM_CheckMD5ing);


	//g_update->InitialDL();
	g_update->RunCheckMD5();



}

void  RunDownload(void)
{

// GKy 100325 #add P2P
#if _P2P_TEST
	MessageBox(NULL, _T(" void  RunDownload(void) "), _T("P2P"), MB_OK); // GKy #test
#endif



	SquirrelObject root=SquirrelVM::GetRootTable();
	SquirrelObject obh=root.GetValue(_T("g_curlProgressbar"));



	CControlUI *child=(CControlUI*)obh.GetInstanceUP(0);

	g_child=child;

	g_pManager=g_child->GetManager();

	

	//g_update->RunDownload();

	

	g_update->InitialDL();
	g_update->CheckNeedDLPatch();



}

static int g_LbTcounter = 0;

int DLFile_GetNow(void)
{
  int ret = 0; 

  if( g_bEnableP2P ){

#ifdef _P2P_APPEND


    // 1. Get pro bar
	ret = gLscInfo.getProcessBar();


#endif

	///////////////////////////////////////////
	//////////// APPEND <> EMBED //////////////
	///////////////////////////////////////////


  }else{

	// #ORIGIN


	if (g_update&& g_update->m_dl)
		return g_update->m_dl->DownloadSize();

	return 100;

  }//if( g_bEnableP2P ){

  //return (float)ret; // GKy 100331
  return ret; // GKy 100512

}
int DLFile_GetMax(void)
{
  //return 77; //#test if crash, no

  if( g_bEnableP2P ){

	// #P2P
	return 100; // GKy 100512

  }else{

	// #ORIGIN

	if (g_update&& g_update->m_dl)
	{

		if (g_update->m_dl->m_waring)
		{
			g_update->m_dl->m_waring=false;
			TCHAR msg[512];

			wsprintf(msg,GetString( _T("BOOT"),_T("NOT_ENOUGH_TITLE") ), g_update->m_dl->m_path, g_update->m_dl->m_nSize/(1024*1024));
			_MessageBox(_T("waring"),msg,_T("waring"),0);
			
		}

		return g_update->m_dl->TotalSize();
	}
	return 100;

  }//if( g_bEnableP2P ){


}

double DLFile_GetSpeed(void)
{

  if( g_bEnableP2P ){

	// #P2P

#ifdef _P2P_APPEND
	  //return (double)(gLscInfo.getSpeedDownload() / 100);
	  return (double)(gLscInfo.getSpeedDownload() ); // GKy 100426
#endif



  }else{

	// #ORIGIN

 	if (g_update&& g_update->m_dl)
		return g_update->m_dl->GetDLSpeed();

	return 0; 

  }//if( g_bEnableP2P ){



}

void VC_CheckFileMD5(void )
{

	_RunCheckMD5();
/*
	int ret=g_update->CheckFileMD5();

	if (ret>0)
	{
		_SetProgressState(EM_DownloadingOK);

	}
	else if (ret==0)
	{
		static int Count=3;
		if (Count>0)
		{
			RunDownload();
			Count--;
		}

	}
*/
}
void VC_ExecuteFile(void )
{



	//int ret=g_update->CheckFileMD5();

	//if (ret>0)
	//{
			_SetProgressState(EM_MD5CheckOK);

	//}
	


	
		
}
void VC_SetPlayGame(bool te)
{
lstrcpy(g_clientName,g_update->m_clientName.c_str());
runGame=te;
}
void VC_SetNewVersion(void )
{


	g_update->SetNewVersion();

}
void VC_RunAgain( TCHAR  *IP, TCHAR* port,TCHAR  *account,TCHAR  *password)
{


if (runLauncher)
{

		

	TCHAR pArg1[1024];
	TCHAR accountPassword[1024];

	memset (pArg1,0,sizeof (pArg1));
	if (lstrlen(IP)>0)
	{

		wsprintf(pArg1,_T("PatchIP:%s PatchPort:%s"),IP,port);
		

	}


	if (lstrlen(account)>0&& lstrlen(password )>0)
	{

		wsprintf(accountPassword,_T(" %s %s"),account,password);
		lstrcat(pArg1,accountPassword);
	}
	
	TCHAR tempExe[512];
	wsprintf(tempExe,_T("%slauncher.exe"),szGameExePath);

	if( PathFileExists( tempExe) == false )
		return;

	SHELLEXECUTEINFO ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = _T("open");
	ShExecInfo.lpFile =tempExe;
	ShExecInfo.lpParameters =pArg1;	
	ShExecInfo.lpDirectory = 0;
	ShExecInfo.nShow = SW_SHOWNORMAL;
	ShExecInfo.hInstApp = NULL;	
	ShellExecuteEx(&ShExecInfo);

}

}
void VC_PlayGame(TCHAR  *account,TCHAR  *password)
{
	if (runGame)
	{
		TCHAR pArg1[1024];
		TCHAR accountPassword[1024];

		memset (pArg1,0,sizeof (pArg1));

		lstrcpy(pArg1,_T("NoCheckVersion"));

		if (lstrlen(account)>0&& lstrlen(password )>0)
		{

			wsprintf(accountPassword,_T(" %s %s"),account,password);
			lstrcat(pArg1,accountPassword);
		}



		if( PathFileExists( g_clientName) == false )
			lstrcpy(g_clientName,_T("Client.exe"));


		
		TCHAR tempExe[512];
		wsprintf(tempExe,_T("%s%s"),szGameExePath,g_clientName);


	

		
		SHELLEXECUTEINFO ShExecInfo = {0};
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		ShExecInfo.hwnd = NULL;
		ShExecInfo.lpVerb = _T("open");
		ShExecInfo.lpFile =tempExe;		
		ShExecInfo.lpParameters = pArg1;	
		ShExecInfo.lpDirectory = szGameExePath;
		ShExecInfo.nShow = SW_SHOWNORMAL;
		ShExecInfo.hInstApp = NULL;	
		ShellExecuteEx(&ShExecInfo);

	}

}
// GKy 100420 1051L2976 add P2P ---------------------------------------------------------
int VC_RunP2Pdownloader(void)
{
#if _P2P_APPEND
	//if (runGame)
	if (g_bEnableP2P)
	{

		// GKy 100422 add duplicated checking
		//////////////////////////////////////////////////////////////////
		// GKy 100422 if find previous existed, need 1.terminate/reStart(seems impossible) 2.use it.
		// GKy 100423 may be here no need check..vcP2P already have test.
		// GKy 100503 just want to terminate the residual one, BUT afraid of Loop, shift.


		//////////////////////////////////////////////////////////////////


		// vcP2P.exe arguements sample:
		// D:\_GKy\_code\Libtorrent_Sample\bin\vcP2P.exe    "D:\_temp\Libtorrent_downLoad\"  "D:\_GKy\_code\Libtorrent_Sample\bin\"   "patch_2.1.2.2020.tw_2.1.5.2030.tw.torrent"   "http://tw.patch.rom.gamania.com/rom/tw"
		/***********************************


	++ _ArgvSavePath = ; // DL File Save path, often same to Client

	++ _ArgvLoadPath = ; // Load torrent file path

	++ _ArgvLoadFile = ; // torrent file name

	++ _ArgvDnLdPath	// web seed path. ex: http://tw.patch.rom.gamania.com/rom/tw/patch_2.1.2.2020.tw_2.1.5.2030.tw.exe

	++ _ArgvSpdLmtUp	// for  (gLsc.get_session()).set_upload_rate_limit(5000); //unit kBs

	++ _ArgvSpdLmtDn	// for  (gLsc.get_session()).set_download_rate_limit(200*1000);


		
		***********************************/

		TCHAR pArg1[2048];

		TCHAR pArg2[2048];

		memset (pArg1,0,sizeof (pArg1));
		lstrcpy(pArg1,_T(" "));

		memset (pArg2,0,sizeof (pArg2)); // GKy 100512 add
		lstrcpy(pArg2,_T(" "));


		// GKy 100430 try find some key for downloader to get this application HWND by ::FindWindow
		// GKy 100512 obsolete


		if (true)
		{
			wsprintf(
				 pArg2
				//,_T(" %s %s %s %s ")
				//,_T(" %s %s %s %s %d %d ") // GKy 100427 add speed Limit
				,_T(" \"%s\" \"%s\" \"%s\" \"%s\" %d %d ") // GKy 100512 add quote

				,szGameExePath // Save download file Path
				//,_T("R:\\LbT_temp") // #test

				,szGameExePath // Load torrent file Path

				,g_wsTorrenFile.c_str() // Torrent file name

				,g_wsWebSeed.c_str() // Web seed URL

				,g_iLscUpSpeed
				,g_iLscDlSpeed

			);
			lstrcat(pArg1,pArg2);
		}

		
		//TCHAR tempExe[512];
		TCHAR tempExe[2048];
		wsprintf(tempExe,_T("%s%s"),szGameExePath,_T("vcP2P.exe"));

		
		SHELLEXECUTEINFO ShExecInfo = {0};
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		ShExecInfo.hwnd = NULL;
		ShExecInfo.lpVerb = _T("open");
		ShExecInfo.lpFile =tempExe;		
		ShExecInfo.lpParameters = pArg1;	
		ShExecInfo.lpDirectory = szGameExePath;
		ShExecInfo.nShow = SW_SHOWNORMAL;
		ShExecInfo.hInstApp = NULL;	

		if(	ShellExecuteEx(&ShExecInfo) ){

		}else{
			// FLR.
			return 0;
		}// if(	ShellExecuteEx(&ShExecInfo) ){


	}//if (g_bEnableP2P)


	return 1;
#else

	return 1;
#endif // #if _P2P_APPEND
}
// -- GKy 100420 1051L2976 add P2P ---------------------------------------------------------


void VC_SelectZone(int index)
{

	g_update->SelectZone(index);

}

void VC_SetLink(HWND hWnd,LPCTSTR link)
{

	ShellExecute(hWnd, _T("open"), link, 0, 0, SW_SHOW);

}


