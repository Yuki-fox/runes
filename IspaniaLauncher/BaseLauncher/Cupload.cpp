#include "stdafx.h"
#include "CUpload.h"
#include <fcntl.h>
#include <io.h>
#include <sys/stat.h>
//#define LOCAL_FILE      "/tmp/uploadthis.txt"
//#define UPLOAD_FILE_AS  "while-uploading.txt"
//#define REMOTE_URL      "ftp://localhost/"  UPLOAD_FILE_AS
//#define RENAME_FILE_TO  "renamed-and-fine.txt"

CFileUpload::CFileUpload():m_FTPClient(nsSocket::CreateDefaultBlockingSocketInstance(), 30)
{

	/* In windows, this will init the winsock stuff */
	


	
	

}
CFileUpload::~CFileUpload()
{

	
}

CFileUpload& CFileUpload::SetTo(CString sAddress)
{
	m_URL=sAddress;

	return *this;
}
CFileUpload& CFileUpload::SetPassWord(CString password)
{
	m_PassWord=password;
return *this;
}
CFileUpload& CFileUpload::SetFile(CString file,CString remotefile)
{
	m_file=file;
	m_remotefile=remotefile;
return *this;
}
BOOL CFileUpload::Upload(void)
{

	
	TCHAR temp[512];

	lstrcpy(temp,(LPCTSTR)m_PassWord);

	tstring name=temp;
	tstring pass=temp;

	int len=lstrlen(temp);
	for (int i=0;i<len;i++)
	{

		if (temp[i]==':')
		{
			temp[i]=0;
			name=temp;
			pass=&temp[i+1];
			break;
		}


	}


	





	m_LogonInfo.SetHost((LPCTSTR)m_URL,nsFTP::DEFAULT_FTP_PORT,name,pass);
	m_FTPClient.Login(m_LogonInfo);



	




	m_FTPClient.UploadFile((LPCTSTR)m_file,(LPCTSTR)m_remotefile);


	
 
		

		
	

	 m_FTPClient.Logout();

		
	
	

	return TRUE;
}
