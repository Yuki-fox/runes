#include "ReadUpdateIni.h"
#include "..\Tools\Cini\IniFile2.h"
#include "..\Tools\Zip\unzip.h"
#ifdef KALYDO
	#ifdef _DEBUG
		#define UPDATEINF "BinDebug/update.inf"
	#else
		#define UPDATEINF "Bin/update.inf"
	#endif
#else
	#define UPDATEINF "update.inf"
#endif // KALYDO
#define UPDATEINI "update_%s.ini"
#define SERVERINI "serverlist_%s.ini"
#define OLDUPDATEINI "update.ini"

#include <Windows.h>
#include <Iphlpapi.h>
#include <Assert.h>
#pragma comment(lib, "iphlpapi.lib")

 void GetIPMACAddress( char *IP, char *MACData)
{
	//memset(MACData,0,10);
	

	IP_ADAPTER_INFO AdapterInfo[16];       // Allocate information
	// for up to 16 NICs
	DWORD dwBufLen = sizeof(AdapterInfo);  // Save memory size of buffer

	DWORD dwStatus = GetAdaptersInfo(      // Call GetAdapterInfo
		AdapterInfo,                 // [out] buffer to receive data
		&dwBufLen);                  // [in] size of receive data buffer
	assert(dwStatus == ERROR_SUCCESS);  // Verify return value is
	// valid, no buffer overflow

	PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo; // Contains pointer to
	// current adapter info

	if (pAdapterInfo && MACData != NULL )
	{
		string	sMacAddress;
		char	buffer[256];

		for( UINT i=0; i < pAdapterInfo->AddressLength; i++ )
		{
			sprintf( buffer, "%02x", pAdapterInfo->Address[i] );
			sMacAddress += buffer;			
		}
		sprintf( MACData, "%s", sMacAddress.c_str() );
	}

	if (pAdapterInfo&&IP)
	{
		memset(IP,0,20);
		memcpy(IP,pAdapterInfo->IpAddressList.IpAddress.String,4*4); //  IP address
	}

}

class MinGZip
{

	HZIP	m_hz; 

public:
	MinGZip():m_hz(0){}


	BOOL Open(const char* zipfile,const char* password);
	int GetData(const char* filename,void* data);
	~MinGZip();
	
};
BOOL MinGZip::Open(const char* zipfile,const char* password)
{
	m_hz = OpenZip( zipfile ,password );
	return m_hz? TRUE:FALSE;
}
int MinGZip::GetData(const char* filename,void* data)
{
	int size=0;
	if (m_hz)
	{
		int index;
		ZIPENTRY ze; 
		ZRESULT	zRet;	
		zRet=FindZipItem(m_hz,filename,true,&index,&ze);
		if (zRet==ZR_OK)
		{
			
			size=ze.unc_size;
			if (data)
			{
				zRet=UnzipItem(m_hz,index,data,size);
			}

		}


	}

	return size;
}

MinGZip::~MinGZip()
{
	CloseZip(m_hz);
}

	IniFile2 g_imageSwitch;
	IniFile2 g_localization;
	IniFile2 g_serverList;	
	IniFile2 g_crashIni;	

	IniFile2& GetLocalizationIni(void)
	{
		return g_localization;
	}
	IniFile2& GetServerListIni(void)
	{

		return g_serverList;

	}
class internal_Update
{
private:
	char*  m_password;

	MinGZip m_zip;
	
	void	MakePassWord(void);
public:
	
	internal_Update();
	~internal_Update();
	void	Initial(void);
	void	LoadINI(void);
	
	void	LoadIniFileFrom(IniFile2& pIni, const char* file );
	void	LoadIniFileFromZip( IniFile2* pIni, const char* file, const char* zipfile );


public:
	IniFile2 m_ini;
	
	IniFile2 m_version;	

};



internal_Update::internal_Update():m_password(0)
{

}
internal_Update::~internal_Update()
{
	if (m_password)
		delete []m_password;
}
void internal_Update::MakePassWord(void)
{
	/*
	int a[4];

	a[0] = rand();
	a[1] = rand();
	a[2] = rand();
	a[3] = rand();


	for (int i=0;i<128;i++)
		m_password[i]=0;

	for (int i=0;i<128;i++)
		m_password[i]= 41 %26+'A';


	m_password[0]='t';
	m_password[3]='v';
	m_password[1]='b';
	m_password[2]='s';
	for (int i=4;i<128;i+=2)
		m_password[i]= 18467 %26+'a';
	m_password[1]--;



	for (int i=5;i<128;i+=2)
		m_password[i]=6334%26+'A';
	m_password[2]--;
	for (int i=6;i<128;i+=3)
		m_password[i]=26500%10+'0';
	m_password[3]--;
	*/

	//strcpy( m_password, "tarubK2Vy5gT8Xo6" );
	strcpy( m_password, "159258gG" );

}

void internal_Update::LoadIniFileFrom(IniFile2& Ini, const char* file )
{

	int size= m_zip.GetData(file,0);
	if (size>0)
	{
		char* data=new char[size] ;

		if (data)
		{
			m_zip.GetData(file,data);
			Ini.SetIniData(data,size);
			delete []data;
		}

	}

}
void internal_Update::LoadIniFileFromZip( IniFile2* pIni, const char* file, const char* zipfile )
{
	if( pIni == NULL )
		return;	

	MinGZip zip;

	if (zip.Open(zipfile,m_password))
	{
		int size=zip.GetData(file,0);
		if (size>0)
		{
			char* data=new char[size] ;
			if (data)
			{
				zip.GetData(file,data);
				pIni->SetIniData(data,size);
				delete []data;
			}
		}	

	}
}

void internal_Update::Initial(void)
{

	if (m_password)
		delete []m_password;
	m_password=new char[128];
	MakePassWord();
	m_password[16]=0;

}
void internal_Update::	LoadINI(void)
{
	if (m_zip.Open(UPDATEINF,m_password))
	{

		IniFile2 installIni;
		LoadIniFileFrom( installIni, "install.ini");


		IniFile2 Localization;	
		LoadIniFileFrom( Localization, "localization.ini"  );




		char FileName[512];

		installIni.SetSection( "BOOT" );

		CTString select	=	installIni.StrDef("select","eneu");
		Localization.SetSection(select.c_str());

		CTString country =Localization.StrDef("country","eneu");
		CTString language =Localization.StrDef("language","eneu");

		sprintf( FileName,UPDATEINI,(LPCTSTR) country.c_str());

		// 讀取對應國家的 Update_xxx.ini

		LoadIniFileFrom( m_ini, FileName);

		sprintf( FileName,SERVERINI,(LPCTSTR) country.c_str());

		// 讀取對應國家的 Update_xxx.ini

		LoadIniFileFrom( g_serverList, FileName);
		// 讀取對應國家的 crash_xxx.ini
		sprintf( FileName,"crash_%s.ini",(LPCTSTR) language.c_str());		

		LoadIniFileFrom( g_crashIni, FileName);

		LoadIniFileFrom( m_version,  "version.ini");

		LoadIniFileFrom( g_localization,  "localization.ini");
		LoadIniFileFrom( g_imageSwitch,  "imageswitch.ini");

		
	}


/*
		m_ini.SetIniData(data,len);

	// 讀取對應國家的 Update_xxx.ini
	{
		//解壓縮		
		HZIP	hz; 
		ZRESULT	zRet;	
		hz = OpenZip( UPDATEINF ,m_password );

		ZIPENTRY ze; 

		if (hz)
		{
			
			int index;
			sprintf(tempz,UPDATEINI,(LPCTSTR)a.c_str());
			
			zRet=FindZipItem(hz,tempz,true,&index,&ze);
			if (zRet==ZR_OK)
			{
				char* data=new char[ze.unc_size] ;
				DWORD len=ze.unc_size;

				zRet=UnzipItem(hz,index,data,len);
				if (zRet==ZR_OK)
					m_ini.SetIniData(data,len);
				else
					m_ini.SetIniFile(_T(OLDUPDATEINI));
				delete []data;

			}

			CloseZip(hz);
		}
		//else
		//	m_ini.SetIniFile(_T(OLDUPDATEINI));



	}
	*/
}

void LoadIniFromZipFile( IniFile2* pIni , const char* pIniFile, const char* pZipFile )
{
	internal_Update update;

	update.Initial();
	update.LoadIniFileFromZip( pIni, pIniFile, pZipFile );
}

std::string GetImageSwitch( const char* Section, const char* lang, const char* def )
{

	g_imageSwitch.SetSection(Section);

	return g_imageSwitch.StrDef(lang,def);
	

}
IniFile2& UpdateIni_GetCrashIni(void)
{

	return g_crashIni;

}

void UpdateIni_GetCrashInfo(UpdateInfInfo& info)
{
	internal_Update update;
	update.Initial();
	update.LoadINI();

	CTString a;
	update.m_ini.SetSection("CrashRpt");

	// 這個目前應該沒做用
	a = update.m_ini.StrDef("Path","");
	if ( a.length() > 2 )
		strcpy(info.crashPath, a.c_str());	
	
	a = update.m_ini.StrDef("IP","");
	strcpy(info.crashIP, a.c_str());

	a = update.m_ini.StrDef("Account","test");	
	strcpy(info.crashAccount, a.c_str());

	a = update.m_ini.StrDef("Password","1234");	
	strcpy(info.crashPassword, a.c_str());

	update.m_ini.SetSection("BOOT");
	a = update.m_ini.StrDef("VersionURL", "");
	strcpy(info.versionURL, a.c_str());

	a = update.m_ini.StrDef("NoticeURL", "");
	strcpy(info.noticeURL, a.c_str());

	a = update.m_ini.StrDef("ActivateURL", "");
	strcpy(info.activateURL, a.c_str());	

	a = update.m_ini.StrDef("ActivateURLArg", "");
	strcpy(info.activateURLArg, a.c_str());

	a = update.m_ini.StrDef("ResetPassswordURL", "");
	strcpy(info.resetPassswordURL, a.c_str());

	a = update.m_ini.StrDef("GMNotificationURL", "");
	strcpy(info.gmNotification, a.c_str());

	a = update.m_version.StrDef("Current_Version", "can't get veriosn");
	strcpy(info.versionNo, a.c_str());

	a = update.m_ini.StrDef("ZShopURL", "");
	strcpy(info.ZShopURL, a.c_str());
	
	info.downloadPort = atoi(update.m_ini.StrDef("DownloadPort", "").c_str());
	info.gameGuard = atoi(update.m_ini.StrDef("EnableGameGuard", "").c_str());
	info.disableVoice = atoi(update.m_ini.StrDef("DisableVoice", "").c_str());
	info.enableVivoxUserAgreement = atoi(update.m_ini.StrDef("EnableVivoxUserAgreement", "").c_str());
	info.disableGuildBattleGround = atoi(update.m_ini.StrDef("DisableGuildBattleGround", "").c_str());
	info.disableCh3Login = atoi(update.m_ini.StrDef("DisableCh3Login", "").c_str());
	info.disableItemMall = update.m_ini.IntDef("DisableItemMall",0);
	info.disableBugReport = atoi(update.m_ini.StrDef("DisableBugReport", "").c_str());	
	info.loginEpisode = atoi(update.m_ini.StrDef("LoginEpisode", "").c_str());	
	info.enterWorldPrompt = atoi(update.m_ini.StrDef("EnterWorldSupplement", "").c_str());
	info.checkMemory = atoi(update.m_ini.StrDef("CheckMemory", "").c_str());

	info.itemMallHelpContent = update.m_ini.IntDef("ItemMallHelpContent",0);
	info.hideLoginUI = update.m_ini.IntDef("HideLoginUI",0);
	info.enableZShop = update.m_ini.IntDef("EnableZShop",0);

	info.webBrowserWidth	=  update.m_ini.IntDef("WebBrowserWidth", 808);
	info.webBrowserHeight	=  update.m_ini.IntDef("WebBrowserHeight", 660 );
}