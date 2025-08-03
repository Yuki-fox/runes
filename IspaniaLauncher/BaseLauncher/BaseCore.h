#pragma once
#include "IniFile2.h"
//class cFilePackage;

class DownloadThreadFile;
class DownloadThreadFileVCp2p;
class BaseCore
{
public:

	BaseCore();
	~BaseCore();
	void Init(CString &ip,CString &port);
	

	//cFilePackage* m_pack;

	BOOL DownloadVersion();
	BOOL DownloadP2pTorrent();
	void DownloadUpadteZip(CString &exename);
	BOOL _DownloadUpadteZip(CString &exename);


	void DownloadVcp2p(CString &exename);
	BOOL checkVCP2P(CString &exename);

	void DownloadThread_File2(void);
	void DownloadThread_File(void);
	void MakePassWord(void);

	IniFile2 m_VresionIni;
	IniFile2 m_ini;
	IniFile2 m_crashString;
	CString m_cli_version;

	IniFile2 m_string;
	CString m_strVersionURL ;
	CString m_strZipPathURL;
	CString m_strZipPathURL2;
	int m_port;
	BOOL m_GoUpdate;
	BOOL m_GoVCP2P;

	CString m_filename;
	char *m_password;


	DownloadThreadFile *m_file;
	DownloadThreadFileVCp2p *m_fileVcp2p;

	static BaseCore g_core;
};