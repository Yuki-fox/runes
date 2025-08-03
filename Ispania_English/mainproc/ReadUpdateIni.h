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
	int	enableVivoxUserAgreement;		// �}��Vivox�P�N�ɭ�
	int disableGuildBattleGround;
	int disableCh3Login;				// ������3���n�J�e��		
	int disableItemMall;				// �����ӫ�
	int disableBugReport;				// �������D�^��
	int	loginEpisode;					// �ĴX��(1 - 4)�ثe��ĥ|��
	int enterWorldPrompt;				// �i�J�C���e�ɥR����
	int checkMemory;					// ��� MB
	int itemMallHelpContent;			// �ӫ� "?" ���s,���Q&A���e
	int hideLoginUI;					// ���õn�J�ɭ�
	int enableZShop;					// �ϥ�ZShop����

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