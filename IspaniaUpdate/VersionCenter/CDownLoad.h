#pragma once


#include "Curlpp\curlpp.hpp"
#include <vector>
using namespace std;
class CFileDownLoad
{
public:
	CFileDownLoad();
	virtual ~CFileDownLoad();

	TCHAR m_URL[512];

	TCHAR m_URL2[512];
	TCHAR m_Proxy[512];
	TCHAR m_PassWord[512];
	TCHAR m_destFolder[512];
	TCHAR m_file[512];
	TCHAR m_remotefile[512];

	bool m_waring;//(false);
	int m_port;
	int m_port2;
	int m_starttime;
	int m_totalSize;
	float m_speed;
	int m_offset;
	int m_dlSize   ;
	double m_nSize;

	TCHAR m_path[512];
	ULARGE_INTEGER m_ulFree;

	int TotalSize();
	int DownloadSize();
	int Offset(){return m_offset; }
	float GetDLSpeed();
	vector<CURLcode> m_ErrCode;
	vector<CURLcode>& GetCurlErrCode();
	HANDLE m_hFile;

	void stop(void);
	CFileDownLoad& SetURL(LPCTSTR URLAddress,int port);
	CFileDownLoad& SetURL2(LPCTSTR URLAddress,int port);
	CFileDownLoad& SetProxy(LPCTSTR URLAddress);
	CFileDownLoad& SetDownloadPath(LPCTSTR Directory);
	CFileDownLoad& SetPassWord(LPCTSTR password);
	CFileDownLoad& SetFile(LPCTSTR localfile,LPCTSTR remotefile);
	BOOL DownLoad(int tryCount=3,int first=0);
	BOOL _DownLoad(HANDLE hFile,int offset,int frist=0);
};