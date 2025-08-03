#pragma once
#pragma warning (disable:4949)

#pragma unmanaged
//////////////////////////////////////////////////////////////////////////
#include <curl/curl.h>
#include <curl/easy.h>
#include <String>
#include <windows.h>
#include <vector>
//////////////////////////////////////////////////////////////////////////
using namespace std;

enum	MyHttpFileProcessTypeENUM
{
	EM_MyHttpFileProcessType_None		,
	EM_MyHttpFileProcessType_Download	,	//¤U¸ü¤¤
	EM_MyHttpFileProcessType_OK			,	//¤U¸ü¦¨¥
	EM_MyHttpFileProcessType_Failed		,	//¤U¸ü¥¢±Ñ
	EM_MyHttpFileProcessType_Interrupt	,	//­n¨D¤¤Â_
};

class MyHttpFile
{
	double	_totalSize;		//ÀÉ®×¤j¤p
	double	_dlSize;		//¤w¤U¸üSize

	std::string	_url;			
	int		_port;

	std::string	_url_proxy;
	int		_port_proxy;

	std::string	_destFolder;
	std::string	_remoteFile;	//¤U¸üªºÀÉ®×¦WºÙ
	std::string  _localFile;		//Àx¦sªºÀÉ®×¦WºÙ
	std::string	_userpwd;		//±b¸¹±K½X
	
	bool	_isRunThread;

	MyHttpFileProcessTypeENUM	_state;		//¥Ø«e³B²zª¬ºA

	HANDLE	_hThread;
	DWORD	_threadId;

	vector<CURLcode>	_curlErrCode;

public:
	MyHttpFile( );
	~MyHttpFile( );
	
	static void Init();
	static void Release();

	//¤U¸ü
	void	SetDownloadUrl( const char* url );
	void	SetDownloadPort( int port );

	void	SetProxyUrl( const char* url );
	void	SetProxyPort( int port );
	
	void	SetUserPwd(std::string	UserPwd );

	bool	Download(std::string destFolder , std::string remoteFile , std::string localFile );
	bool	Download_NoThread(std::string destFolder , std::string remoteFile , std::string localFile , int tryTimes );
	bool	Download_NoThread(std::string destFolder , std::string remoteFile , std::string localFile );
	/*
	bool	Download( const char* url , const char* destFolder , const char* remoteFile , char* localFile );

	bool	Download_NoThread( const char* url , const char* destFolder , const char* remoteFile , char* localFile );

	bool	Download_NoThread( const char* url , const char* destFolder , const char* remoteFile , char* localFile , int TryTimes );
*/
	vector<CURLcode>*	GetCurlErrCode(){ return& _curlErrCode; };

	//­n¨D¤¤Â_¤U¸ü
	bool	Interrupt( );

	//²M°£
	bool	Clear();

	//¬O§_ÁÙ¦b¶] Thread  (¤U¸ü¤¤)
	bool	IsRunThread();

	MyHttpFileProcessTypeENUM State();

	double	TotalSize();
	double	DownloadSize();

	const char* Url( );
	const char* DestFolder( );
	const char* RemoteFileName( );
	const char* LocalFileName( );
	int m_Got_Nothing;
	float m_dlKbs;
	int m_sTime;

protected:
	static DWORD WINAPI _St_DownloadThread( void* This ){ return ((MyHttpFile*)This)->_DownloadThread() ; };
	int    _DownloadThread( );

	int  m_bInterrupt;
	static int CurlProgressCallback(void *clientp,double dltotal,double dlnow,double ultotal,double ulnow );
	static size_t WriteFunctionCallback( void *ptr, size_t size, size_t nmemb, void *stream );

};






//////////////////////////////////////////////////////////////////////////
#pragma managed
