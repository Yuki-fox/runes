#include "..\tools\Cini\IniFile2.h"

struct UpdateInfInfo
{
	UpdateInfInfo()
	{
		downloadPort = 80;
		gameGuard = 0;
		disableVoice = 1;
		disableGuildBattleGround = 0;
		disableCh3Login = 0;
		disableBugReport = 0;
		enableVivoxUserAgreement = 0;
		loginEpisode = 0;
		enterWorldPrompt = 0;
		checkMemory = 0;
		itemMallHelpContent = 0;
		hideLoginUI = 0;
		enableZShop = 0;

		memset(crashIP, 0, sizeof(crashIP));
		memset(crashPath, 0, sizeof(crashPath));
		memset(crashAccount, 0, sizeof(crashAccount));
		memset(crashPassword, 0, sizeof(crashPassword));
		memset(versionURL, 0, sizeof(versionURL));
		memset(versionNo, 0, sizeof(versionNo));
		memset(noticeURL, 0, sizeof(noticeURL));
		memset(activateURL, 0, sizeof(activateURL));
		memset(activateURLArg, 0, sizeof(activateURLArg));
		memset(resetPassswordURL, 0, sizeof(resetPassswordURL));
		memset(gmNotification, 0, sizeof(gmNotification));
		memset(ZShopURL, 0, sizeof(ZShopURL));
	}

	char crashIP[MAX_PATH];
	char crashPath[MAX_PATH];
	char crashAccount[MAX_PATH];
	char crashPassword[MAX_PATH];
	char versionURL[MAX_PATH];
	char versionNo[MAX_PATH];
	char noticeURL[MAX_PATH];
	char activateURL[MAX_PATH];	
	char activateURLArg[MAX_PATH];	
	char resetPassswordURL[MAX_PATH];
	char gmNotification[MAX_PATH];
	char ZShopURL[MAX_PATH];
	int downloadPort;
	int gameGuard;
	int disableVoice;
	int	enableVivoxUserAgreement;		// 開啟Vivox同意界面
	int disableGuildBattleGround;
	int disableCh3Login;				// 關閉第3章登入畫面		
	int disableItemMall;				// 關閉商城
	int disableBugReport;				// 關閉問題回報
	int	loginEpisode;					// 第幾章(1 - 4)目前到第四章
	int enterWorldPrompt;				// 進入遊戲前補充說明
	int checkMemory;					// 單位 MB
	int itemMallHelpContent;			// 商城 "?" 按鈕,顯示Q&A內容
	int hideLoginUI;					// 隱藏登入界面
	int enableZShop;					// 使用ZShop介面

	int webBrowserWidth;				// WebBrowser Width
	int webBrowserHeight;				// WebBrowser Height
};

void UpdateIni_GetCrashInfo(UpdateInfInfo& info);
IniFile2& UpdateIni_GetCrashIni(void);
void LoadIniFromZipFile( IniFile2* pIni, const char* pIniFile, const char* pZipFile );

std::string GetImageSwitch( const char* Section, const char* lang, const char* def );

IniFile2& GetLocalizationIni(void);
IniFile2& GetServerListIni(void);
 void GetIPMACAddress( char *IP, char *MACData);