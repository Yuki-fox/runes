
#pragma once
#include <atlmisc.h>


#include "FTPClient.h"

class CFileUpload 
{
public:
	CFileUpload();
	virtual ~CFileUpload();

	 nsFTP::CFTPClient                 m_FTPClient;
	 nsFTP::CLogonInfo                 m_LogonInfo;

	CString m_URL;
	CString m_PassWord;
	CString m_file;
	CString m_remotefile;

	CFileUpload& SetTo(CString sAddress);
	CFileUpload& SetPassWord(CString password);
	CFileUpload& SetFile(CString localfile,CString remotefile);
	BOOL Upload(void);
};