#include "stdafx.h"
#include "CDownload.h"
#include <fcntl.h>
#include <io.h>
#include <sys/stat.h>
#include <mmsystem.h>
#include <fstream>

 

class HFILE_trait : public curlpp::output_trait
{
  
public:
	virtual ~HFILE_trait()
	{}

	HFILE_trait( HANDLE file ) : m_file( file )
	{}

	size_t write( void *buffer, size_t length );

protected:
	HANDLE m_file;
};
size_t HFILE_trait::write( void *buffer, size_t length )
{
	DWORD writeBytesOut=0;

	WriteFile( m_file, buffer, length, &writeBytesOut, NULL );
	
	
	return writeBytesOut;

}


class my_progress_trait : public curlpp::progress_trait
{
public:
	my_progress_trait():m_This(NULL),m_first(true)
	{}
	void set(CFileDownLoad* This)
	{
		GetModuleFileName(NULL,This->m_path,512);
		int len=lstrlen(This->m_path);
		for(int i=len-1;i>0;--i)
		{
			if (This->m_path[i]=='\\')
			{

				This->m_path[i+1]=0;

				break;
			}

		}

	
		m_first=true;
		m_This=This;
	}
	bool progress_function(double dltotal,
		double dlnow,
		double ultotal,
		double ulnow);

	
	bool m_first;
	

	CFileDownLoad* m_This;
};

void _GetDiskFree(TCHAR* path ,ULARGE_INTEGER& ulFree )
{
	ULARGE_INTEGER ulAvailable; 
	ULARGE_INTEGER ulTotal; 




	



	GetDiskFreeSpaceEx(path, &ulAvailable,&ulTotal, &ulFree); 

}



bool my_progress_trait::progress_function(double dltotal,
										  double dlnow,
										  double ultotal,
										  double ulnow)
{
	 
	m_This->m_totalSize = dltotal;		//檔案大小
	m_This->m_dlSize	 =(int) dlnow;		//已下載Size

	int nowtime=timeGetTime();
	float time=(nowtime-m_This->m_starttime);
	if (time<=0) 
		time=1;
	m_This->m_speed= ((float)m_This-> m_dlSize/(time/1000));


	if (m_first)
	{
		

		m_first=false;

		_GetDiskFree(m_This->m_path,m_This->m_ulFree);
		m_This->m_nSize=dltotal-dlnow;
		
		if( m_This->m_nSize > m_This->m_ulFree.QuadPart )
			m_This->m_waring=true;
		else
			m_This->m_waring=false;


	}


	return true;
}








CFileDownLoad::CFileDownLoad():m_port(-1),m_hFile(INVALID_HANDLE_VALUE),m_waring(false)
{
	curlpp::initialize();
	m_Proxy[0]=0;
}
CFileDownLoad::~CFileDownLoad()
{

 curlpp::terminate();
}

CFileDownLoad& CFileDownLoad::SetDownloadPath(LPCTSTR Directory)
{

	
	lstrcpy(m_destFolder,Directory);
	return *this;
}
	CFileDownLoad& CFileDownLoad::SetProxy(LPCTSTR URLAddress)
	{
		lstrcpy(m_Proxy,URLAddress);
		return *this;
	}
CFileDownLoad& CFileDownLoad::SetURL(LPCTSTR sAddress,int port)
{
	lstrcpy(m_URL,sAddress);
	m_port=port;

	return *this;
}
CFileDownLoad& CFileDownLoad::SetPassWord(LPCTSTR password)
{
	lstrcpy(m_PassWord,password);
	
	return *this;
}
CFileDownLoad& CFileDownLoad::SetFile(LPCTSTR file,LPCTSTR remotefile)
{
	lstrcpy(m_file,file);
	
	lstrcpy(m_remotefile,remotefile);

	return *this;
}
vector<CURLcode>& CFileDownLoad::GetCurlErrCode()
{

	

	return m_ErrCode;

}
float CFileDownLoad::GetDLSpeed()
{

	return m_speed;
}
int CFileDownLoad::TotalSize()
{
	return m_totalSize;
}
int CFileDownLoad::DownloadSize()
{
return m_dlSize+m_offset;
}

CFileDownLoad&  CFileDownLoad::SetURL2(LPCTSTR URLAddress,int port)
{
	lstrcpy(m_URL2,URLAddress);
	m_port2=port;

	return *this;

}
 my_progress_trait progress_trait;
BOOL CFileDownLoad::_DownLoad(HANDLE hFile,int offset,int frist)
{



	
	BOOL ret=TRUE;

	curlpp::easy request;



	TCHAR UrlFile[ 512 ];

	if (!frist)
		wsprintf( UrlFile , _T("%s%s"), (LPCTSTR)m_URL ,(LPCTSTR) m_remotefile );
	else
		wsprintf( UrlFile , _T("%s%s"), (LPCTSTR)m_URL2 ,(LPCTSTR) m_remotefile );

	char localFile[512];
	
	int len=lstrlen(UrlFile);
	 len = WideCharToMultiByte(CP_ACP, 0, UrlFile, len, localFile, 511, NULL, NULL);

	localFile[len]=0;



	HFILE_trait body_trait( hFile );
	request.m_body_storage.trait( &body_trait ); 


	request.resume_from(offset);
//std:wstring t(UrlFile);

	 request.url( localFile);


	 char Proxyname[512];
	 len=lstrlen(m_Proxy);
	 len = WideCharToMultiByte(CP_ACP, 0, m_Proxy, len, Proxyname, 511, NULL, NULL);

	 Proxyname[len]=0;


	 std::string proxys=Proxyname;

	 if (!frist)
	 {

		 if (m_port>0)	
			 request.port(m_port);
	 }
	 else
	 {
		 if (m_port2>0)	
			 request.port(m_port2);

	 }
	

	 if (strlen(Proxyname)>0)
		request.proxy_name(proxys);

	 try
	 {
		 progress_trait.set(this);
		 request.progress();
		
		 request.m_progress_storage.trait( &progress_trait );


		 request.perform();
	 }
	 catch (curlpp:: exception* err)
	 {

		const char * strerr=err->what();

		char temp[512];
		


		sprintf( temp ,"%s %d %s",localFile,m_port,strerr);


		MessageBoxA(NULL,temp,"Error",0);

		ret=FALSE;

	 }

	

	
#ifdef _DEBUG
	getchar();
#endif
	
	return ret;


}
void CFileDownLoad::stop(void)
{
 
if (m_hFile!=INVALID_HANDLE_VALUE)
CloseHandle(m_hFile);
m_hFile=0;
}

BOOL CFileDownLoad::DownLoad(int tryCount,int first)
{

	BOOL ret=TRUE;
	int count=tryCount;
	m_starttime=timeGetTime();
 

	TCHAR wlocalFile[ 512 ];
	wsprintf( wlocalFile , _T("%s%s"),(LPCTSTR) m_destFolder , (LPCTSTR)m_file );
//	char localFile[512];

	//int len=lstrlen(wlocalFile);
	//len = WideCharToMultiByte(CP_ACP, 0, wlocalFile, len, localFile, 511, NULL, NULL);

//	localFile[len]=0;

	if ( (m_hFile = CreateFile( wlocalFile, GENERIC_EXECUTE|GENERIC_WRITE, FILE_APPEND_DATA, NULL, OPEN_EXISTING, 0, NULL ) ) == INVALID_HANDLE_VALUE ) 
	{
		m_hFile =CreateFile(wlocalFile,GENERIC_EXECUTE|GENERIC_WRITE,0,NULL,CREATE_ALWAYS,0,NULL);
		
		
	}
	
//	std::ofstream file(wlocalFile,std::ios::app|std::ios::binary); 


	

	m_totalSize=100; //origin

	m_speed=0;
	m_dlSize=0;
	do 
	{




		m_offset=GetFileSize(m_hFile,NULL);
		SetFilePointer(m_hFile,m_offset,0,FILE_BEGIN);		
		
		try 
		{
			count--;
			ret=_DownLoad(m_hFile,m_offset,first);
			if (ret)
				break;
		}
		catch (...)
		{
			
			if (count<=0)
			{
				bool dialogg=false;

				if(!first)
				{
					int len=lstrlen(m_URL2);
					if (len==0)
					{
						dialogg=true;
						ret=false;

					}

				}
				else
					dialogg=true;
				if (dialogg)
				{
					TCHAR temp[512];
					TCHAR UrlFile[512];
					wsprintf( UrlFile ,_T("%s:%d %s"), (LPCTSTR)m_URL ,m_port,(LPCTSTR) m_remotefile );


					wsprintf( temp , _T("Download %s fail!!") ,UrlFile);


					MessageBoxW(NULL,temp,_T("Error"),0);
				}		
			}
		}


	}while(count>0);


	CloseHandle(m_hFile);

	return ret;

	
}
