#include "stdafx.h"
#include "resource.h"
#include <direct.h>
#include <process.h>
#include <fstream>
#include "Zip/unzip.h"

#include "BaseCore.h"


#include "CDownload.h"
#include "FileVersion.h"

#include "DownloadThread_File.h"


#define UPDATEINI "update_%s.ini"
#define OLDUPDATEINI "update.ini"
#define UPDATEINF "update.inf"
//CIni update;

#define STRINGINI "string_%s.ini"
CFileDownLoad file;
BaseCore::BaseCore():m_GoUpdate(TRUE)
{

	m_password=NULL;


}
BaseCore::~BaseCore()
{

	


}
void BaseCore::MakePassWord(void)
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
BOOL BaseCore::DownloadP2pTorrent()
{
	int ret=FALSE;
	
	m_VresionIni.SetSection(_T("Current Version"));

	CString Version=m_VresionIni.Str(_T("Version"));

	if (Version!=m_cli_version)
	{

		m_VresionIni.SetSection(m_cli_version);
		CString filename=m_VresionIni.Str(_T("DownloadFile"));

		TCHAR tempTorrent[512];
		TCHAR tempTorrent1[512];
		wsprintf(tempTorrent,_T("%s.torrent"),(LPCTSTR)filename);

		wsprintf(tempTorrent1,_T("%s.torrent.txt"),(LPCTSTR)filename);
		DeleteFile(tempTorrent);
		//	_mkdir( DownloadPath );		

		file.SetFile(tempTorrent,tempTorrent1);
		file.SetDownloadPath("");	

		file.SetProgreessBar(NULL);
	
		//dlg3.SetTitleText(m_strVersionURL);
		//dlg3.SetFileName(tempTorrent);


		TCHAR url[2048];
		wsprintf(url,_T("%s%s.torrent port=%x\n"),m_strVersionURL,(LPCTSTR)filename,m_port);
		FILE* ifs=fopen("log.txt","a+");
		if (ifs)
		{
			fwrite(url,lstrlen(url),1,ifs);
			fclose(ifs);

		}


		file.SetURL(m_strVersionURL,m_port);
		file.SetURL2(m_strZipPathURL2,m_port);
		if (file.DownLoad())
		{





			ret=TRUE;
		}
	}
	

	
	m_GoUpdate=ret;

	return ret;

}

void BaseCore::Init(CString &ip,CString &port)
{

	m_password=new char[128];

	MakePassWord();

	m_password[16]=0;
	IniFile2 Localization;
	//[BOOT]
	//Current_Version=2.1.7.2135.ae

	IniFile2 cli_version;


	IniFile2 Install;
	Localization.SetIniFile(_T("localization.ini"));
	Install.SetIniFile(_T("localization.ini"));
	{
		//解壓縮		
		HZIP	hz; 
		ZRESULT	zRet;	
		hz = OpenZip( UPDATEINF ,m_password ); 
		
		ZIPENTRY ze; 

		if (hz)
		{
			
			int index;		
			CString select="en";
			CString a="en";

			zRet=FindZipItem(hz,_T("install.ini"),true,&index,&ze);
			if (zRet==ZR_OK ||zRet==ZR_MORE)
			{
				char* data=new char[ze.unc_size] ;
				DWORD len=ze.unc_size;

				zRet=UnzipItem(hz,index,data,len);
				Install.Reset();
				Install.SetIniData(data,len);

				delete []data;
			}
			else
			{
				zRet=FindZipItem(hz,_T("localization.ini"),true,&index,&ze);
				if (zRet==ZR_OK)
				{
					char* data=new char[ze.unc_size] ;
					DWORD len=ze.unc_size;

					zRet=UnzipItem(hz,index,data,len);

					Install.Reset();
					Install.SetIniData(data,len);

					delete []data;
				}

			}
			zRet=FindZipItem(hz,_T("version.ini"),true,&index,&ze);
			if (zRet==ZR_OK||zRet==ZR_MORE)
			{
				char* data=new char[ze.unc_size] ;
				DWORD len=ze.unc_size;

				zRet=UnzipItem(hz,index,data,len);
				cli_version.Reset();
				cli_version.SetIniData(data,len);

				cli_version.SetSection(_T("BOOT"));
				m_cli_version=cli_version.Str(_T("Current_Version"));

				delete []data;
			}


			

			zRet=FindZipItem(hz,_T("localization.ini"),true,&index,&ze);
			if (zRet==ZR_OK ||zRet==ZR_MORE)
			{
				char* data=new char[ze.unc_size] ;
				DWORD len=ze.unc_size;

				zRet=UnzipItem(hz,index,data,len);
				Localization.Reset();
				Localization.SetIniData(data,len);
				
				delete []data;
			}
		
			select=Install.StrDef("select","en");
			Localization.SetSection(select);
			a=Localization.StrDef("country","en");

			CString name;
			name.Format(UPDATEINI,a);			
			zRet=FindZipItem(hz,name,true,&index,&ze);
			if (zRet==ZR_OK ||zRet==ZR_MORE)
			{
				char* data=new char[ze.unc_size] ;
				DWORD len=ze.unc_size;

				zRet=UnzipItem(hz,index,data,len);
				if (zRet==ZR_OK||zRet==ZR_MORE)
					m_ini.SetIniData(data,len);
				else
					m_ini.SetIniFile(_T(OLDUPDATEINI));
				delete []data;

			}

			CString sname;
			sname.Format(STRINGINI,a);			
			zRet=FindZipItem(hz,sname,true,&index,&ze);
			if (zRet==ZR_OK ||zRet==ZR_MORE)
			{
				char* data=new char[ze.unc_size] ;
				DWORD len=ze.unc_size;

				zRet=UnzipItem(hz,index,data,len);
				if (zRet==ZR_OK||zRet==ZR_MORE)
					m_string.SetIniData(data,len);

				delete []data;

			}
			CString csname;
			csname.Format("crash_%s.ini",a);			
			zRet=FindZipItem(hz,csname,true,&index,&ze);
			if (zRet==ZR_OK ||zRet==ZR_MORE)
			{
				char* data=new char[ze.unc_size] ;
				DWORD len=ze.unc_size;

				zRet=UnzipItem(hz,index,data,len);
				if (zRet==ZR_OK||zRet==ZR_MORE)
					m_crashString.SetIniData(data,len);

				delete []data;

			}

			CloseZip(hz);
		}
		else
			m_ini.SetIniFile(_T(OLDUPDATEINI));
		
		

	}

	delete []m_password;
	m_password=NULL;



	CString tempStr=m_ini.Str(_T( "VersionURL") ) ;

	TCHAR temp[1024];
	lstrcpy(temp,(LPCTSTR)tempStr);
	int len=lstrlen(temp);
	if (temp[len-1]!='/')
		lstrcat(temp,_T("/"));

	m_strVersionURL = temp;
	m_strZipPathURL2 = temp;

	if (ip.GetLength()>0)
	{
		m_strVersionURL=ip;
		m_strZipPathURL2=ip;
	}





	tempStr=m_ini.Str( _T("DownloadURL") );
	lstrcpy(temp,(LPCTSTR)tempStr);
	 len=lstrlen(temp);
	if (temp[len-1]!='/')
		lstrcat(temp,_T("/"));

	m_strZipPathURL	= temp;


	if (ip.GetLength()>0)
	{
		m_strZipPathURL=ip;
	}



	m_port= m_ini.IntDef( _T("DownloadPort"),0 );

	if (port.GetLength()>0)
		m_port=atoi((LPCTSTR)port);


}

#include "ProgressBarDLG.h"
CProgressBarDLG dlg,dlg2;
void BaseCore::DownloadVcp2p(CString &exename)
{
	m_filename=exename;
	m_fileVcp2p=new DownloadThreadFileVCp2p(this);
//	m_fileVcp2p->start();
	file.SetProgreessBar(&dlg2);

	dlg2.m_thread=m_fileVcp2p;
	dlg2.DoModal();
	m_VresionIni.SetSection(_T("Current Version"));
	CString vcp2p=m_VresionIni.StrDef(_T("vcp2p"),_T(""));
	if(vcp2p.GetLength()>0)
		DownloadP2pTorrent();
}

void BaseCore::DownloadUpadteZip(CString &exename)
{
	
	m_filename=exename;
	m_file=new DownloadThreadFile(this);
	//m_file->start();
	dlg.m_thread=m_file;
	file.SetProgreessBar(&dlg);
	
	dlg.DoModal();


}
BOOL BaseCore::checkVCP2P(CString &exename)
{
	BOOL ret=FALSE;
	BOOL DOWNLOAD=TRUE;


	m_GoVCP2P=FALSE;

	CFileVersion ispaniaVersion;
	CString ver;
	CString exe=_T("vcp2p_");

	m_VresionIni.SetSection(_T("Current Version"));
	CString vcp2p=m_VresionIni.StrDef(_T("vcp2p"),_T(""));
	if(vcp2p.GetLength()==0)
		return TRUE;


	OFSTRUCT of;
	
	

	{
	


		if (ispaniaVersion.Open(exename))
		{
			ver=ispaniaVersion.GetFixedFileVersion();

			ver=exe+ver+_T(".zip");

			if (ver==vcp2p)
				DOWNLOAD=FALSE; 
		}
	}
	if (DOWNLOAD)
	{

		DeleteFile( VCP2PZIP );
		file.SetFile(CString(VCP2PZIP),(LPCTSTR)vcp2p);
		file.SetDownloadPath(CString(""));
		file.SetURL(m_strVersionURL,m_port);
		file.SetURL2(m_strZipPathURL2,m_port);

		int dlret=file.DownLoad(3,0);
		if (dlret)
		{

			HANDLE hFileDB  = CreateFile(VCP2PZIP,
				GENERIC_READ,
				FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL);
			if (hFileDB!=INVALID_HANDLE_VALUE &&hFileDB!=NULL)
			{
				int dbSize=GetFileSize(hFileDB,0);


				CloseHandle(hFileDB);
				if (dbSize<4096)
				{
					dlret=0;
					DeleteFile( VCP2PZIP );

				}
			}
		}


		if (!dlret) 
		{
			int len=lstrlen(file.m_URL2);
			if (len>0)
			{

				dlret=file.DownLoad(3,1);
			}

		}
		if (dlret)
		{

			HANDLE hFileDB  = CreateFile(VCP2PZIP,
				GENERIC_READ,
				FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL);
			if (hFileDB!=INVALID_HANDLE_VALUE &&hFileDB!=NULL)
			{
				int dbSize=GetFileSize(hFileDB,0);


				CloseHandle(hFileDB);  if (dbSize<4096)
				{
					dlret=0;
					DeleteFile( VCP2PZIP );

				}
			}
		}


		if (dlret)
			ret=TRUE;



	}
	else
		ret=TRUE;

	

	m_GoVCP2P=ret;
	return ret;
}

void BaseCore::DownloadThread_File2(void)
{





	if (checkVCP2P(m_filename)==FALSE)
	{
		MessageBox(NULL,_T("Can't download vcp2p.zip"),_T("vcp2p"),0);

		dlg2.SetProgress(0);
		dlg2.EndDialog(S_FALSE);
	}
	else
	{
		Sleep(1000);

		dlg2.SetProgress(100);
		dlg2.EndDialog(S_OK);
	}


}

void BaseCore::DownloadThread_File(void)
{





if (_DownloadUpadteZip(m_filename)==FALSE)
{
	MessageBox(NULL,_T("Can't download ClientUpdate.zip"),_T("update"),0);
	
	dlg.SetProgress(0);
	dlg.EndDialog(S_FALSE);
}
else
{
	Sleep(1000);

	dlg.SetProgress(100);
	dlg.EndDialog(S_OK);
}


}

BOOL BaseCore::_DownloadUpadteZip(CString &exename)
{
	BOOL ret=FALSE;
	BOOL DOWNLOAD=TRUE;




	CFileVersion ispaniaVersion;
	CString ver;

	m_VresionIni.SetSection(_T("Current Version"));
	CString update=m_VresionIni.StrDef(_T("UpdateFile"),_T("1.1.1.1.zip"));

OFSTRUCT of;
		HANDLE hFile = NULL;
	if ( (hFile = (HANDLE)OpenFile( _T("vcfile.res"), &of, OF_READ | OF_SHARE_COMPAT)) != (HANDLE)HFILE_ERROR )

	{
		CloseHandle(hFile);

	
		if (ispaniaVersion.Open(exename))
		{
			ver=ispaniaVersion.GetFixedFileVersion();

			ver=ver+_T(".zip");

			if (ver==update)
				DOWNLOAD=FALSE; 
		}
	}
	if (DOWNLOAD)
	{

		DeleteFile( UpdateRar );
		file.SetFile(CString(UpdateRar),(LPCTSTR)update);
		file.SetDownloadPath(CString(""));
		file.SetURL(m_strZipPathURL,m_port);
		file.SetURL2(m_strZipPathURL2,m_port);

		int dlret=file.DownLoad(3,0);
		if (dlret)
		{

			HANDLE hFileDB  = CreateFile(UpdateRar,
				GENERIC_READ,
				FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL);
			if (hFileDB!=INVALID_HANDLE_VALUE &&hFileDB!=NULL)
			{
				int dbSize=GetFileSize(hFileDB,0);
				

				CloseHandle(hFileDB);if (dbSize<4096)
				{
					dlret=0;
					DeleteFile( UpdateRar );

				}
			}
		}

		
		if (!dlret) 
		{
			int len=lstrlen(file.m_URL2);
			if (len>0)
			{

				dlret=file.DownLoad(3,1);
			}

		}
		if (dlret)
		{

			HANDLE hFileDB  = CreateFile(UpdateRar,
				GENERIC_READ,
				FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL);
			if (hFileDB!=INVALID_HANDLE_VALUE &&hFileDB!=NULL)
			{
				int dbSize=GetFileSize(hFileDB,0);
				

				CloseHandle(hFileDB);  if (dbSize<4096)
				{
					dlret=0;
					DeleteFile( UpdateRar );

				}
			}
		}

		
		if (dlret)
			ret=TRUE;



	}
	else
		ret=TRUE;

	
m_GoUpdate=ret;
	return ret;



}


BOOL BaseCore::DownloadVersion()
{
	int ret=FALSE;
	
	CString filename="version.txt";

	CString DownloadPath;

	CString _DownloadPath=m_ini.Str( _T("ZipPath") ) ;

	DownloadPath=_DownloadPath;
	//產生目錄
	CreateDirectory(DownloadPath,NULL);
	TCHAR tempVersion[512];
	wsprintf(tempVersion,_T("%s\\%s"),(LPCTSTR)DownloadPath,(LPCTSTR)filename);

	DeleteFile(tempVersion);
//	_mkdir( DownloadPath );		

	file.SetFile(filename,filename);
	file.SetDownloadPath(DownloadPath+"/");	

	file.SetURL(m_strVersionURL,m_port);
	if (file.DownLoad())
	{
		m_VresionIni.SetIniFile(DownloadPath+"/"+filename);


	


		ret=TRUE;
	}
	m_GoUpdate=ret;

	return ret;
}
