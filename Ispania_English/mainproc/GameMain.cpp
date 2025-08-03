#include <time.h>
#include <stdlib.h>  //Include for C Runtime Invalid Parameter Check (see MSDN: "_set_invalid_parameter_handler")
#include <process.h>
#include "GameMain.h"
#include <shellapi.h>
#include <psapi.h>
#include "../netwaker_member/NetWakerClientInc.h"
#include "../interface/Debug/DebugMessage.h"
#include "../Interface/Login/AccountLogin.h"
#include "../Interface/Login/LoginParent.h"
#include "../Interface/Login/CharacterCreate.h"
#include "../Interface/SkillBook/UI_SkillBook.h"
#include "../Interface/WorldFrames/ActionFrame.h"
#include "../Interface/WorldFrames/LoadingFrame.h"
#include "../Interface/WorldFrames/ItemClipboard.h"
#include "../Interface/WorldFrames/GameControl.h"
#include "../Interface/WorldFrames/PartnerFrame.h"
#include "../Interface/WorldFrames/ScrollCombatTextFrame.h"
#include "../Interface/WorldFrames/CastingBarFrame.h"
#include "../Interface/WorldFrames/BagFrame.h"
#include "../Interface/WorldFrames/StoreFrame.h"
#include "../Interface/WorldFrames/TutorialFrame.h"
#include "../Interface/WorldFrames/LootFrame.h"
#include "../Interface/WorldFrames/ChatFrame.h"
#include "../Interface/WorldFrames/ChatEdit.h"
#include "../Interface/WorldFrames/CombatMeter.h"
#include "../Interface/WorldFrames/PetFrame.h"
#include "../Interface/WorldFrames/CoverStringFrame.h"
#include "../Interface/WorldFrames/BattleGroundOptionButtonFrame.h"
#include "../interface/ObjectBloodBar/ObjectBloodBar.h"
#include "../Interface/TransportBook/TeleportBook.h"
#include "../Interface/Houses/HousesFrame.h"
#include "../Interface/GuildHouses/GuildHousesFrame.h"
#include "../Interface/Map/WorldMap.h"
#include "../Interface/Quest/UI_QuestDetail.h"
//#include "../Interface/MouseLight/MouseLight.h"
#include "../tools/CIni/Ini.h"
//#include "../tools/CrashRpt/CrashRpt.h"
//#include "../tools/ApexClient/RaApexClient.h"
#include "../tools/verinfo/verinfo.h"
#include "MyHttpFile/MyHttpFile.h"
#include "../Tools/Cini/IniFile2.h"
#include "../Tools/Mstring/Mstring.h"
#include "../Interface/WorldFrames/TutorialFrame.h"
#include "../tools/SmallObj/SmallObj.h"
//#include "../tools/Vivox/VivoxLib/RequestManager.h" // GKy 100115
#include "../tools/md5/Mymd5.h"
#include "../tools/bcrypt/ow-crypt.h"
#include "LauncherOpera.h"
#include <Psapi.h>
#include <iphlpapi.h> 
#include "../tools/Tracert/Tracert.h"
#include <tchar.h>
#include <windows.h>
#include <shlwapi.h>
#include <iostream>
#include <strsafe.h>
#include <io.h>
#include <DbgHelp.h>
#include "createdump/CreateDump.h"

#include <Windows.h>
#include <wininet.h> 
#include <iostream>
#pragma comment(lib, "Wininet")

#define PRODUCT_ULTIMATE 0x00000001
#define PRODUCT_HOME_BASIC 0x00000002
#define PRODUCT_HOME_PREMIUM 0x00000003
#define PRODUCT_ENTERPRISE 0x00000004
#define PRODUCT_HOME_BASIC_N 0x00000005
#define PRODUCT_BUSINESS 0x00000006
#define PRODUCT_STANDARD_SERVER 0x00000007
#define PRODUCT_DATACENTER_SERVER 0x00000008
#define PRODUCT_SMALLBUSINESS_SERVER 0x00000009
#define PRODUCT_ENTERPRISE_SERVER 0x0000000A
#define PRODUCT_STARTER 0x0000000B
#define PRODUCT_DATACENTER_SERVER_CORE 0x0000000C
#define PRODUCT_STANDARD_SERVER_CORE 0x0000000D
#define PRODUCT_ENTERPRISE_SERVER_CORE 0x0000000E
#define PRODUCT_ENTERPRISE_SERVER_IA64 0x0000000F
#define PRODUCT_BUSINESS_N 0x00000010
#define PRODUCT_WEB_SERVER 0x00000011
#define PRODUCT_CLUSTER_SERVER 0x00000012
#define PRODUCT_HOME_SERVER 0x00000013
#define PRODUCT_STORAGE_EXPRESS_SERVER 0x00000014
#define PRODUCT_STORAGE_STANDARD_SERVER 0x00000015
#define PRODUCT_STORAGE_WORKGROUP_SERVER 0x00000016
#define PRODUCT_STORAGE_ENTERPRISE_SERVER 0x00000017
#define PRODUCT_SERVER_FOR_SMALLBUSINESS 0x00000018
#define PRODUCT_SMALLBUSINESS_SERVER_PREMIUM 0x00000019
#define PRODUCT_HOME_PREMIUM_N 0x0000001A
#define PRODUCT_ENTERPRISE_N 0x0000001B
#define PRODUCT_ULTIMATE_N 0x0000001C
#define PRODUCT_WEB_SERVER_CORE 0x0000001D
#define PRODUCT_MEDIUMBUSINESS_SERVER_MANAGEMENT 0x0000001E
#define PRODUCT_MEDIUMBUSINESS_SERVER_SECURITY 0x0000001F
#define PRODUCT_MEDIUMBUSINESS_SERVER_MESSAGING 0x00000020
#define PRODUCT_SMALLBUSINESS_SERVER_PRIME 0x00000021
#define PRODUCT_HOME_PREMIUM_SERVER 0x00000022
#define PRODUCT_SERVER_FOR_SMALLBUSINESS_V 0x00000023
#define PRODUCT_STANDARD_SERVER_V 0x00000024
#define PRODUCT_DATACENTER_SERVER_V 0x00000025
#define PRODUCT_ENTERPRISE_SERVER_V 0x00000026
#define PRODUCT_DATACENTER_SERVER_CORE_V 0x00000027
#define PRODUCT_STANDARD_SERVER_CORE_V 0x00000028
#define PRODUCT_ENTERPRISE_SERVER_CORE_V 0x00000029
#define PRODUCT_HYPERV 0x0000002A
#define SM_TABLETPC 86
#define SM_MEDIACENTER 87
#define SM_STARTER 88
#define SM_SERVERR2 89
#define VER_SERVER_NT 0x80000000
#define VER_WORKSTATION_NT 0x40000000
#define VER_SUITE_SMALLBUSINESS 0x00000001
#define VER_SUITE_ENTERPRISE 0x00000002
#define VER_SUITE_BACKOFFICE 0x00000004
#define VER_SUITE_COMMUNICATIONS 0x00000008
#define VER_SUITE_TERMINAL 0x00000010
#define VER_SUITE_SMALLBUSINESS_RESTRICTED 0x00000020
#define VER_SUITE_EMBEDDEDNT 0x00000040
#define VER_SUITE_DATACENTER 0x00000080
#define VER_SUITE_SINGLEUSERTS 0x00000100
#define VER_SUITE_PERSONAL 0x00000200
#define VER_SUITE_BLADE 0x00000400
#define VER_SUITE_EMBEDDED_RESTRICTED 0x00000800
#define VER_SUITE_SECURITY_APPLIANCE 0x00001000
#define VER_SUITE_STORAGE_SERVER 0x00002000
#define VER_SUITE_COMPUTE_SERVER 0x00004000
#define VER_SUITE_WH_SERVER 0x00008000
#define VER_NT_WORKSTATION              0x0000001
#define VER_NT_DOMAIN_CONTROLLER        0x0000002
#define VER_NT_SERVER                   0x0000003
#define PROCESSOR_ARCHITECTURE_INTEL            0
#define PROCESSOR_ARCHITECTURE_MIPS             1
#define PROCESSOR_ARCHITECTURE_ALPHA            2
#define PROCESSOR_ARCHITECTURE_PPC              3
#define PROCESSOR_ARCHITECTURE_SHX              4
#define PROCESSOR_ARCHITECTURE_ARM              5
#define PROCESSOR_ARCHITECTURE_IA64             6
#define PROCESSOR_ARCHITECTURE_ALPHA64          7
#define PROCESSOR_ARCHITECTURE_MSIL             8
#define PROCESSOR_ARCHITECTURE_AMD64            9
#define PROCESSOR_ARCHITECTURE_IA32_ON_WIN64    10
#define PROCESSOR_ARCHITECTURE_UNKNOWN 0xFFFF

typedef VOID(WINAPI* GETNATIVESYSTEMINFO)(LPSYSTEM_INFO);
typedef BOOL(WINAPI* GETPRODUCTINFO)(DWORD, DWORD, DWORD, DWORD, PDWORD);

typedef struct _OSVERSIONINFOEXA2 {
	DWORD dwOSVersionInfoSize;
	DWORD dwMajorVersion;
	DWORD dwMinorVersion;
	DWORD dwBuildNumber;
	DWORD dwPlatformId;
	CHAR  szCSDVersion[128];     // Maintenance string for PSS usage
	WORD  wServicePackMajor;
	WORD  wServicePackMinor;
	WORD  wSuiteMask;
	BYTE  wProductType;
	BYTE  wReserved;
} OSVERSIONINFOEXA2, * POSVERSIONINFOEXA2, * LPOSVERSIONINFOEXA2;

typedef struct _OSVERSIONINFOEXW2 {
	DWORD dwOSVersionInfoSize;
	DWORD dwMajorVersion;
	DWORD dwMinorVersion;
	DWORD dwBuildNumber;
	DWORD dwPlatformId;
	WCHAR  szCSDVersion[128];     // Maintenance string for PSS usage
	WORD   wServicePackMajor;
	WORD   wServicePackMinor;
	WORD   wSuiteMask;
	BYTE  wProductType;
	BYTE  wReserved;
} OSVERSIONINFOEXW2, * POSVERSIONINFOEXW2, * LPOSVERSIONINFOEXW2, RTL_OSVERSIONINFOEXW2, * PRTL_OSVERSIONINFOEXW2;

#ifdef UNICODE
typedef OSVERSIONINFOEXW2 OSVERSIONINFOEX2;
typedef POSVERSIONINFOEXW2 POSVERSIONINFOEX2;
typedef LPOSVERSIONINFOEXW2 LPOSVERSIONINFOEX2;
#else
typedef OSVERSIONINFOEXA2 OSVERSIONINFOEX2;
typedef POSVERSIONINFOEXA2 POSVERSIONINFOEX2;
typedef LPOSVERSIONINFOEXA2 LPOSVERSIONINFOEX2;
#endif // UNICODE

#ifdef KALYDO
// Add Exception.h so we can raise exceptions for the Kalydo Error Reporter
#include "../../Kalydo/SocialIntegration/Exception.h"
#endif // KALYDO

#define SEND_MOVE_DELAY_TIME			0.3f
#define SEND_MOVE_FORAWRD_TIME			0.5f
#define MAX_CAMERA_THETA				0.95f
#define MIN_CAMERA_THETA				-0.95f

#define SAVE_CHECK_CODE					0x1011

#define PLAYER_TARGET_FLOOR_ENTITY		"model\\item\\ui_object\\target\\target.ros"

#define GAME_SETTINGS_FILENAME	"GameSettings.ini"
#define GC_MAINSCTION			"Game" 
#define GC_BAGSCTION			"Bag"
#define GC_ACTIONBAR			"ActionBar" 
#define GC_MOUSEMOVE			"MouseMove"
#define GC_NPCTITLE				"NpcTitle"
#define GC_DISABLETITLEHIDE		"DisableTitleHide"
#define GC_PLAYERTITLE			"PlayerTitle"
#define GC_SELFTITLE			"SelfTitleVisible"
#define GC_SELFCASTSTATE		"SelfCastState"
#define GC_ALLSCTVISIBLE		"AllSCTVisible"
#define GC_LASTSELECTEDREALM	"LastSecletedRealm"
#define GC_LASTACCOUNTNAME		"LastAccountName"
#define GC_SHOWGEMEPROMRT		"ShowGemePromrt"
#define GC_ACCOUNTNAME			"AccountName"
#define GC_LASTPLAYERNAME		"LastPlayerName"
#define GC_TITLEVISIBLE			"TitleVisible"
#define GC_GUILDVISIBLE			"GuildVisible"
#define GC_TITLEICONVISIBLE		"TitleIconVisible"
#define GC_CAMERAREVERSE		"CameraReverse"
#define GC_LBUTTONCANCELTARGET	"LButtonCancelButton"
#define GC_RBUTTONCANCELTARGET	"RButtonCancelButton"
#define GC_LBUTTONCAMERAROTATEENABLE	"LButtonCameraRotateEnalbe"
#define GC_CAMERAFOLLOWENABLE	"CameraFollowEnalbe"
#define GC_DISABLEDISPLAYNPCTALK		"NPCTalkDisplayDiable"
#define GC_CAMERASELECTTARGET	"CameraSelectTarget"
#define GC_PLAYLOOTMOTION		"PlayLootMotion"
#define GC_CHATDISPLAYCLASSCOLOR		"ChatDisplayClassColor"
#define GC_AUTOTAKELOOT			"AutoTakeLoot"
#define GC_AGGROPROMPT			"AggroPrompt"
#define GC_AUTOOPENGOODSPACK	"AutoOpenGoodsPack"
#define GC_HIDEOTHERPLAYEREFFECT	"HideOtherPlayerEffect"

#define GC_BLOODBAR				"BloodBar"
#define GC_NPCBLOODBAR			"NPC_BloodBar"
#define GC_PCBLOODBAR			"PLAYER_BloodBar"
#define GC_MOBBLOODBAR			"MOB_BloodBar"
#define GC_SELFBLOODBAR			"SELF_BloodBar"
#define GC_OBB_DISTANCE			"Distance"
#define GC_OBB_TITLE			"Title"
#ifdef KALYDO
	// We place the file update.inf in the Bin/ directory in stead of in the root due to the package 
	// structure of Kalydo. For debug, we use a BinDebug/ directory so we can experiment in debug 
	// without changing the release version.
#ifdef _DEBUG
static const char* UPDATEINF = "BinDebug/update.inf";
#else
static const char* UPDATEINF = "Bin/update.inf";
#endif
#else
static const char* UPDATEINF = "update.inf";
#endif // KALYDO

class CDIB
{
public:
	CDIB() {
		m_hDC = NULL;
		m_hbmBitmap = NULL;
		m_pBitmapBits = NULL;
		m_dwWidth = 0;
		m_dwHeight = 0;
		m_dwBitCount = 0;
	}

	virtual ~CDIB() {
		// Release Bitmap Handle
		if (m_hbmBitmap) {
			DeleteObject(m_hbmBitmap);
			m_hbmBitmap = NULL;
		}

		// Release DC Handle
		if (m_hDC) {
			DeleteDC(m_hDC);
			m_hDC = NULL;
		}
	}

	virtual void Init(DWORD dwWidth, DWORD dwHeight, DWORD dwBitCount) {
		// Prepare to create a bitmap
		BITMAPINFO bmi;
		ZeroMemory(&bmi.bmiHeader, sizeof(BITMAPINFOHEADER));
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth = (int)dwWidth;
		bmi.bmiHeader.biHeight = -(int)dwHeight;
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biCompression = (DWORD)BI_RGB;
		bmi.bmiHeader.biBitCount = (WORD)dwBitCount;

		m_dwWidth = dwWidth;
		m_dwHeight = dwHeight;
		m_dwBitCount = dwBitCount;

		// Create a DC and a bitmap for the font
		m_hDC = CreateCompatibleDC(NULL);
		m_hbmBitmap = CreateDIBSection(m_hDC, &bmi, DIB_RGB_COLORS,
			(VOID**)&m_pBitmapBits, NULL, 0);
		SetMapMode(m_hDC, MM_TEXT);
		SelectObject(m_hDC, m_hbmBitmap);
	}

	virtual HDC GetDC() {
		return m_hDC;
	}

	virtual HBITMAP GetBitmap() {
		return m_hbmBitmap;
	}

	virtual BYTE* GetBitmapBits() {
		return m_pBitmapBits;
	}

	virtual BOOL BitBlt(HDC hdcDest, int nXDest, int nYDest, int nWidth, int nHeight,
		int nXSrc, int nYSrc, DWORD dwRop) {
		return ::BitBlt(hdcDest, nXDest, nYDest, nWidth, nHeight, m_hDC, nXSrc, nYSrc, dwRop);
	}

	virtual DWORD GetWidth() {
		return m_dwWidth;
	}

	virtual DWORD GetHeight() {
		return m_dwHeight;
	}

protected:
	HDC		m_hDC;					// DC Handle
	HBITMAP m_hbmBitmap;			// Bitmap Handle
	BYTE* m_pBitmapBits;			// Bitmap Pointer
	DWORD	m_dwWidth;
	DWORD	m_dwHeight;
	DWORD	m_dwBitCount;
};

char* g_languageCode[MAX_LANGUAGES] = {
	LANGUAGE_CODE_RW,
	LANGUAGE_CODE_TW,
	LANGUAGE_CODE_CN,
	LANGUAGE_CODE_DE,
	LANGUAGE_CODE_JP,
	LANGUAGE_CODE_KR,
	LANGUAGE_CODE_ENUS,
	LANGUAGE_CODE_ENEU,
	LANGUAGE_CODE_SG,
	LANGUAGE_CODE_BR,
	LANGUAGE_CODE_TR,
	LANGUAGE_CODE_RU,
	LANGUAGE_CODE_FR,
	LANGUAGE_CODE_PT,
	LANGUAGE_CODE_PL,
	LANGUAGE_CODE_VN,
	LANGUAGE_CODE_ES,
	LANGUAGE_CODE_ID,
	LANGUAGE_CODE_NL,
	LANGUAGE_CODE_RO,
	LANGUAGE_CODE_PH,
	LANGUAGE_CODE_PL,
	LANGUAGE_CODE_TH,
	LANGUAGE_CODE_SAES,
	LANGUAGE_CODE_SAPT,
	LANGUAGE_CODE_AE,
	LANGUAGE_CODE_ENAR,
	LANGUAGE_CODE_AUS,
	LANGUAGE_CODE_IT
};

CGameMain* g_pGameMain = NULL;
HANDLE							g_hBitmap = NULL;
HWND							g_hWnd = NULL;

bool	CheckEffectDisplay(int id, int targetId)
{
	if (g_pGameMain)
		return g_pGameMain->CheckDisplayEffect(id, targetId);
	return true;
}

// ------------------------------------------------------------------------------------------------
bool	CheckHttpVersionBase(const char* Path, const char* UrlStr, int UrlPort, const char* DlInfoFile)
{

	MyHttpFile _httpFile;

	_httpFile.SetDownloadUrl(UrlStr);
	_httpFile.SetDownloadPort(UrlPort);

	//	_httpFile->SetProxyUrl( DotNetStr::StringToChar( _proxyURL ).c_str() );
	//	_httpFile->SetProxyPort( _proxyPort );

	//ª©¥»Â²¬d
	bool result = _httpFile.Download_NoThread(
		Path
		, "version.txt"
		, "version.txt"
		, 5);

	if (result == false)
	{
		MessageBox(NULL, "ÀË¬dª©¥»¥¢±Ñ¡AµLªk¤U¸üÀË¬dÀÉ", "¿ù»~", MB_OK);
		return 0;
	}
	char	newVersion[32];
	char	version[32];

	char	CheckFilePath[512];
	sprintf(CheckFilePath, "%s/%s", Path, "version.txt");

	//¤ñ¹ï¦r¦ê
	FILE* fs = fopen(CheckFilePath, "r");
	if (fs == NULL)
		return false;

	fgets(newVersion, sizeof(version), fs);

	fclose(fs);

	fs = fopen(DlInfoFile, "r");
	if (fs == NULL)
		return false;

	fread(version, 1, sizeof(version), fs);

	fclose(fs);


	if (stricmp(newVersion, version) == 0)
		return true;

	return false;
}
bool	CheckHttpVersion(const char* IniFile)
{
	/*
	char	fullName[MAX_PATH];
	char	tmp[512];
	GetFullPathName( IniFile , MAX_PATH, fullName, NULL );
	CIni ini(fullName);

	ini.GetString("BOOT", "ZipPath", tmp, MAX_PATH, "");
	string PathStr = tmp;

	ini.GetString("BOOT", "DownloadInfo", tmp, MAX_PATH, "");
	string DlInfoFileStr = tmp;

	ini.GetString("BOOT", "DownloadURL", tmp, MAX_PATH, "");
	string UrlStr = tmp;

	ini.GetString("BOOT", "DownloadPort", tmp, MAX_PATH, "");
	int		UrlPort = atol( tmp );
	*/
	//ini.GetString("Update", "UpdateExe", tmp, MAX_PATH, "");
	//string	UpdateExe = tmp;
	string	UpdateExe = "Launcher.exe";
	/*
	if(		UrlStr.length() == 0
	||	DlInfoFileStr.length() == 0 )
	{
	OutputDebugString( "Update.ini³]©wÀÉ¦³°ÝÃD" );
	return false;
	}

	MyHttpFile::Init();
	bool Ret = CheckHttpVersionBase( PathStr.c_str() , UrlStr.c_str() , UrlPort , DlInfoFileStr.c_str() );
	MyHttpFile::Release();

	if( Ret == false )
	{
	_spawnl( _P_NOWAIT , UpdateExe.c_str() , UpdateExe.c_str() , NULL );
	}
	*/
	_spawnl(_P_NOWAIT, UpdateExe.c_str(), UpdateExe.c_str(), NULL);
	return false;
	//return Ret;
}

// ----------------------------------------------------------------------------------
LRESULT CALLBACK GreenPostProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (iMsg)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		SelectObject(ps.hdc, g_hBitmap);
		EndPaint(hWnd, &ps);
		return 0;

	case WM_TIMER:
		KillTimer(hWnd, 0x00FFFFFF);
		SendMessage(hWnd, WM_CLOSE, 0, 0);
		break;

	case WM_CLOSE:
		DeleteObject(g_hBitmap);
		DestroyWindow(hWnd);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

// ----------------------------------------------------------------------------------
void CreateGreenPostWnd(HINSTANCE hInstance)
{
	WNDCLASS WndClass;

	// Register frame window
	WndClass.style = 0;
	WndClass.lpfnWndProc = GreenPostProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = NULL;
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = "GreenPost";
	RegisterClass(&WndClass);

	HWND hDesktop = GetDesktopWindow();
	RECT desktopRect;
	GetWindowRect(hDesktop, &desktopRect);

	int w = 512;
	int h = 384;
	int x = ((desktopRect.right - desktopRect.left) - w) / 2;
	int y = ((desktopRect.bottom - desktopRect.top) - h) / 2;

	g_hWnd = CreateWindow("GreenPost", "GreenPost", WS_BORDER | WS_DLGFRAME | WS_VISIBLE, x, y, w, h, NULL, NULL, hInstance, NULL);
	g_hBitmap = LoadImage(hInstance, "skin\\RA_Web_Loding1_cn.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	SetTimer(g_hWnd, 0x00FFFFFF, 2000, NULL);

	MSG Msg;
	while (GetMessage(&Msg, NULL, 0U, 0U))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	/*
	PeekMessage( , PM_NOREMOVE );
	while( Msg.message != WM_QUIT )
	{
		BOOL boMessage = PeekMessage( &Msg, NULL, 0U, 0U, PM_REMOVE | PM_NOYIELD );
		if( boMessage )
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
		else
		{
		}
	}
	*/
}


LPVOID  m_lpvState; //keep Crashrpt instance pointer for generating crashreport

void InvalidParameterHandler(const wchar_t* expression,
	const wchar_t* function,
	const wchar_t* file,
	unsigned int line,
	uintptr_t pReserved)
{
	//may be log error here

	//---------------------------------------------------------
	//generate crash report
	//---------------------------------------------------------
	//__try
	//{
	//	RaiseException(EXCEPTION_ACCESS_VIOLATION, 0, 0, NULL);
	//}
	//__except(GenerateErrorReport(m_lpvState, GetExceptionInformation())){}
}

// ------------------------------------------------------------------------------------------------
//BOOL WINAPI CrashCallback(LPVOID lpvState)
//{
//	AddFile(lpvState, "client.config.ini", "NULL");
//	//AddFile(lpvState, "dummy.ini", "Dummy INI File");
//
//	return TRUE;
//} 

#if IS_DETECT_MEMORY_LEAKS_ENABLE
void DetectMemoryLeaks(void)
{
	int tmpDbgFlag;
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
	tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
	tmpDbgFlag |= _CRTDBG_ALLOC_MEM_DF;
	_CrtSetDbgFlag(tmpDbgFlag);
}
#endif
#define CHECK_LUNCHER_NAME		"C7382E70-2747-4481-94BB-967E1C73BAB9"
struct ObjectDataLoadFileStruct
{
	string File;
	ReaderClass< DB_GameObjDbStruct >* Reader;
	int Size;
};
// ------------------------------------------------------------------------------------------------
INT RunApp(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, INT iCmdShow)
{
#if IS_DETECT_MEMORY_LEAKS_ENABLE
	DetectMemoryLeaks();
	//_CrtSetBreakAlloc(123456);
#endif
	//int gameDataSize = sizeof(GameObjDbStruct) - 20000;

	//{
	//	ObjectDataLoadFileStruct LoadFile[] =
	//	{
	//		{ "addpowerobject.db" , GameObjDbStructEx::ObjReader_AddPower(), sizeof(GameObjectAttributeStruct) + gameDataSize },
	//		{ "armorobject.db" , GameObjDbStructEx::ObjReader_Armor(), sizeof(GameObjectItemStruct) + gameDataSize },
	//		{ "bodyobject.db" , GameObjDbStructEx::ObjReader_Body(), sizeof(GameObjectBodyObjStruct) + gameDataSize },
	//		{ "campobject.db" , GameObjDbStructEx::ObjReader_Camp(), sizeof(GameCampStatuteStruct) + gameDataSize },
	//		{ "cardobject.db" , GameObjDbStructEx::ObjReader_Card(), sizeof(GameObjectItemStruct) + gameDataSize },
	//		{ "createroleobject.db" , GameObjDbStructEx::ObjReader_CreateRole(), sizeof(GameObjectCreateRoleStruct) + gameDataSize },
	//		{ "eqrefineabilityobject.db", GameObjDbStructEx::ObjReader_EqRefineAbility(), sizeof(GameObjectEqRefineAttributeStruct) + gameDataSize },
	//		{ "flagobject.db" , GameObjDbStructEx::ObjReader_Flag(), sizeof(GameObjDbStruct) - (sizeof(char) * 20000) },	
	//		{ "imageobject.db" , GameObjDbStructEx::ObjReader_Image(), sizeof(GameObjectImageStruct) + gameDataSize },
	//		{ "itemobject.db" , GameObjDbStructEx::ObjReader_Item(), sizeof(GameObjectItemStruct) + gameDataSize },
	//		{ "keyitemobject.db" , GameObjDbStructEx::ObjReader_KeyItem(), sizeof(GameObjectKeyItemStruct) + gameDataSize },
	//		{ "learnmagicex.db" , GameObjDbStructEx::ObjReader_LearnMagic(), sizeof(GameObjectLearnMagicStruct) + gameDataSize },
	//		{ "magiccollectobject.db" , GameObjDbStructEx::ObjReader_MagicCollect(), sizeof(GameObjectMagicCollectStruct) + gameDataSize },
	//		{ "magicobject.db" , GameObjDbStructEx::ObjReader_MagicBase(), sizeof(GameObjectMagicStruct) + gameDataSize },
	//		{ "mineobject.db" , GameObjDbStructEx::ObjReader_Mine(), sizeof(GameObjectMineStruct) + gameDataSize },
	//		{ "npcobject.db" , GameObjDbStructEx::ObjReader_NPC(), sizeof(GameObjectNpcStruct) + gameDataSize },
	//		{ "phantomobject.db" , GameObjDbStructEx::ObjReader_Phantom(), sizeof(GameObjectPhantomStruct) + gameDataSize },
	//		{ "playerobject.db" , GameObjDbStructEx::ObjReader_Player(), sizeof(GameObjDbStruct) - (sizeof(char) * 20000) },
	//		{ "questdetailobject.db" , GameObjDbStructEx::ObjReader_QuestDetail(), sizeof(GameObjectQuestDetail) + gameDataSize },
	//		{ "questnpcoject.db" , GameObjDbStructEx::ObjReader_QuestNPC(), sizeof(GameObjectNpcStruct) + gameDataSize },
	//		{ "recipeobject.db" , GameObjDbStructEx::ObjReader_Recipe(), sizeof(GameObjectRecipeStruct) + gameDataSize },
	//		{ "runeobject.db" , GameObjDbStructEx::ObjReader_Rune(), sizeof(GameObjectItemStruct) + gameDataSize },
	//		{ "shopobject.db" , GameObjDbStructEx::ObjReader_Shop(), sizeof(GameObjectShopStruct) + gameDataSize },
	//		{ "suitobject.db" , GameObjDbStructEx::ObjReader_Suit(), sizeof(GameObjectSuitStruct) + gameDataSize },
	//		{ "titleobject.db" , GameObjDbStructEx::ObjReader_Title(), sizeof(GameObjectTitleStruct) + gameDataSize },
	//		{ "treasureobject.db" , GameObjDbStructEx::ObjReader_Treasure(), sizeof(GameTreasureStruct) + gameDataSize },
	//		{ "weaponobject.db" , GameObjDbStructEx::ObjReader_Weapon(), sizeof(GameObjectItemStruct) + gameDataSize },
	//		{ "zoneobject.db" , GameObjDbStructEx::ObjReader_Zone(), sizeof(GameObjectZoneStruct) + gameDataSize },		
	//	};
	//	for(int i = 0 ; i < 28 ; i++ )
	//	{
	//		LoadFile[i].Reader->PrintReaderInfo(LoadFile[i].File, LoadFile[i].Size);
	//	}
	//}	


	//28
	//{
	//	ObjectDataLoadFileStruct LoadFile[] =
	//	{
	//		//{ "AddPowerObject.DB" , GameObjDbStructEx::ObjReader_AddPower() },
	//		{ "ArmorObject.DB" , GameObjDbStructEx::ObjReader_Armor() },
	//		//{ "BodyObject.DB" , GameObjDbStructEx::ObjReader_Body() },
	//		//{ "CampObject.DB" , GameObjDbStructEx::ObjReader_Camp() },
	//		{ "CardObject.DB" , GameObjDbStructEx::ObjReader_Card() },
	//		//{ "CreateRoleObject.DB" , GameObjDbStructEx::ObjReader_CreateRole() },
	//		//{ "EqRefineAbilityObject.DB", GameObjDbStructEx::ObjReader_EqRefineAbility() },
	//		//{ "FlagObject.DB" , GameObjDbStructEx::ObjReader_Flag() },	
	//		//{ "ImageObject.DB" , GameObjDbStructEx::ObjReader_Image() },
	//		{ "ItemObject.DB" , GameObjDbStructEx::ObjReader_Item() },
	//		//{ "KeyItemObject.DB" , GameObjDbStructEx::ObjReader_KeyItem() },
	//		//{ "LearnMagic.DB" , GameObjDbStructEx::ObjReader_LearnMagic() },
	//		//{ "MagicCollectObject.DB" , GameObjDbStructEx::ObjReader_MagicCollect() },
	//		//{ "MagicObject.DB" , GameObjDbStructEx::ObjReader_MagicBase() },
	//		//{ "MineObject.DB" , GameObjDbStructEx::ObjReader_Mine() },
	//		//{ "NPCObject.DB" , GameObjDbStructEx::ObjReader_NPC() },
	//		//{ "PhantomObject.DB" , GameObjDbStructEx::ObjReader_Phantom() },
	//		//{ "PlayerObject.DB" , GameObjDbStructEx::ObjReader_Player() },
	//		//{ "QuestDetailObject.DB" , GameObjDbStructEx::ObjReader_QuestDetail() },
	//		//{ "QuestNPCObject.DB" , GameObjDbStructEx::ObjReader_QuestNPC() },
	//		//{ "RecipeObject.DB" , GameObjDbStructEx::ObjReader_Recipe() },
	//		{ "RuneObject.DB" , GameObjDbStructEx::ObjReader_Rune() },
	//		//{ "ShopObject.DB" , GameObjDbStructEx::ObjReader_Shop() },
	//		//{ "SuitObject.DB" , GameObjDbStructEx::ObjReader_Suit() },
	//		//{ "TitleObject.DB" , GameObjDbStructEx::ObjReader_Title() },
	//		//{ "TreasureObject.DB" , GameObjDbStructEx::ObjReader_Treasure() },
	//		{ "WeaponObject.DB" , GameObjDbStructEx::ObjReader_Weapon() },
	//		//{ "ZoneObject.DB" , GameObjDbStructEx::ObjReader_Zone() },		
	//	};
	//	for(int i = 0 ; i < 5 ; i++ )
	//	{
	//		fstream file;
	//		string path = "C:\\Users\\danrer\\Downloads\\new\\";
	//		string filePath = path + LoadFile[i].File;
	//		file.open(filePath.c_str(), ios::out | ios::binary | ios::app);
	//		if (file.is_open())
	//		{
	//			map<string, ReadInfoStruct>* ReadInfo = LoadFile[i].Reader->ReadInfo();
	//			map<string, ReadInfoStruct>::iterator Iter;
	//			for (Iter = ReadInfo->begin(); Iter != ReadInfo->end(); Iter++)
	//			{
	//				ReadInfoStruct* IDData = &(Iter->second);
	//				int	strLen = IDData->ValueName.length() + 1;
	//				file.write((char*) & (strLen), sizeof(int));
	//				file.write((char*) & (IDData->Point), sizeof(int));
	//				file.write((char*)IDData->ValueName.c_str(), strLen);
	//			}
	//			file.close();
	//		}
	//	}
	//}
	//PlayerRoleData* Pt = NULL;
	//char errorBuf[512];
	//sprintf(errorBuf, "buff %d\n", sizeof(Pt->BaseData.Buff));
	//OutputDebugString(errorBuf);



	//{
	//	ObjectDataLoadFileStruct LoadFile[] =
	//	{
	//		{ "addpowerobject.db" , GameObjDbStructEx::ObjReader_AddPower() },
	//		{ "armorobject.db" , GameObjDbStructEx::ObjReader_Armor() },
	//		{ "bodypoject.db" , GameObjDbStructEx::ObjReader_Body() },
	//		{ "camppoject.db" , GameObjDbStructEx::ObjReader_Camp() },
	//		{ "cardpoject.db" , GameObjDbStructEx::ObjReader_Card() },
	//		{ "createRoleobject.db" , GameObjDbStructEx::ObjReader_CreateRole() },
	//		{ "eqrefineabilityobject.db", GameObjDbStructEx::ObjReader_EqRefineAbility() },
	//		{ "flagobject.db" , GameObjDbStructEx::ObjReader_Flag() },
	//		{ "imageobject.db" , GameObjDbStructEx::ObjReader_Image() },
	//		{ "itemobject.db" , GameObjDbStructEx::ObjReader_Item() },
	//		{ "keyitemobject.db" , GameObjDbStructEx::ObjReader_KeyItem() },
	//		{ "learnmagic.db" , GameObjDbStructEx::ObjReader_LearnMagic() },
	//		{ "magiccollectobject.db" , GameObjDbStructEx::ObjReader_MagicCollect() },
	//		{ "magicobject.db" , GameObjDbStructEx::ObjReader_MagicBase() },
	//		{ "mineobject.db" , GameObjDbStructEx::ObjReader_Mine() },
	//		{ "npcobject.db" , GameObjDbStructEx::ObjReader_NPC() },
	//		{ "phantomobject.db" , GameObjDbStructEx::ObjReader_Phantom() },
	//		{ "playerobject.db" , GameObjDbStructEx::ObjReader_Player() },
	//		{ "questdetailobject.db" , GameObjDbStructEx::ObjReader_QuestDetail() },
	//		{ "questnpcobject.db" , GameObjDbStructEx::ObjReader_QuestNPC() },
	//		{ "recipeobject.db" , GameObjDbStructEx::ObjReader_Recipe() },
	//		{ "runeobject.db" , GameObjDbStructEx::ObjReader_Rune() },
	//		{ "shopobject.db" , GameObjDbStructEx::ObjReader_Shop() },
	//		{ "suitobject.db" , GameObjDbStructEx::ObjReader_Suit() },
	//		{ "titleobject.db" , GameObjDbStructEx::ObjReader_Title() },
	//		{ "treasureobject.db" , GameObjDbStructEx::ObjReader_Treasure() },
	//		{ "weaponobject.db" , GameObjDbStructEx::ObjReader_Weapon() },
	//		{ "zoneobject.db" , GameObjDbStructEx::ObjReader_Zone() },
	//	};
	//	for (int i = 0; i < 28; i++)
	//	{
	//		LoadFile[i].Reader->PrintReaderInfo("F:\\fields.csv", LoadFile[i].File);
	//	}
	//}
	//-------------------------------------------------------
	CLogList::Initialize("LogList.txt");
	//-------------------------------------------------------

	//-------------------------------------------------------
	char	szVersion[64];
	char	SavePath[MAX_PATH];
	char	ClientIP[MAX_PATH];
	char	ClientMacAddress[MAX_PATH];
	//-------------------------------------------------------

	//-------------------------------------------------------
	//get current executable file version
	CFileVersionInfo verInfo;
	verInfo.Open(GetModuleHandle(0));
	sprintf(szVersion, "%d.%d.%d.%d", verInfo.GetFileVersionMajor(), verInfo.GetFileVersionMinor(), verInfo.GetFileVersionBuild(), verInfo.GetFileVersionQFE());
	//-------------------------------------------------------
	//-------------------------------------------------------
	// Crash Report
	GetIPMACAddress(ClientIP, ClientMacAddress);
	GetFullPathName("CrashRpt", MAX_PATH, SavePath, NULL);

	UpdateInfInfo info;
	UpdateIni_GetCrashInfo(info);


	if (false == IsDebuggerPresent()) //¤£¬O¥ÎVC¶] ¤~°õ¦æCrash Report
	{
		IniFile2& ini = UpdateIni_GetCrashIni();
		ini.SetSection("CrashString");
		std::string title = ini.StrDef("title", "report");
		std::string context = ini.StrDef("context", "report");
		std::string OK = ini.StrDef("ok", "ok");
		std::string Cancel = ini.StrDef("cancel", "cancel");
		std::string	stitle = Utf8ToChar(title.c_str());
		std::string	scontext = Utf8ToChar(context.c_str());
		std::string	sOK = Utf8ToChar(OK.c_str());
		std::string	sCancel = Utf8ToChar(Cancel.c_str());

		// m_lpvState = Install(CrashCallback, _T("RA_BUG@moliyo.com"), _T("BUG!"));
		// SetAutoSavePath(m_lpvState, SavePath, TRUE);
		// SetAutoUpload(m_lpvState, info.crashIP, info.crashAccount, FALSE);
		// SetDxDiagOutput( m_lpvState, false );
		// SetAppName( m_lpvState,"RA" ); //RA-xxx¶}ÀY¤~·|³Q¤W¶Ç

		// SetString(m_lpvState, stitle.c_str(), scontext.c_str(), sOK.c_str(), sCancel.c_str());

		// _set_invalid_parameter_handler(InvalidParameterHandler);// Set invalid parameter handler to custom Handler
	}

	//-------------------------------------------------------
	// §ó·sBTInstaller Launcher
	if (szCmdLine != NULL && strlen(szCmdLine) >= 14 && (strnicmp(szCmdLine, "UpdateLauncher", 14) == 0))
	{
		LauncherOpera lco;
		lco.UpdateLauncher();
		lco.StartLauncher();
		return 0;
	}

	//-------------------------------------------------------
	//initialize random number 
	//¤£¯à©ñ¦bUpdateIni_GetCrashInfo¤§«e
	srand((unsigned)time(NULL));
	//-------------------------------------------------------

	//-------------------------------------------------------
	//create CGameMain object and set values	
	g_pGameMain = new CGameMain;

	g_pGameMain->SetUpdateInfInfo(info);

	//set mac and IP address
	g_pGameMain->SetMac(ClientMacAddress);
	g_pGameMain->SetIP(ClientIP);

	// ¤½§iºô§}
	g_pGameMain->SetNoticeURL(info.noticeURL);
	g_pGameMain->SetActivateURL(info.activateURL);
	g_pGameMain->SetActivateURLArg(info.activateURLArg);

	// ¸ü¤J»y¨t³]©w
	g_pGameMain->LoadLanguage();

	//set version information
	g_pGameMain->SetVersion(szVersion);
	g_pGameMain->SetVersionNO(info.versionNo);
	g_pGameMain->SetVersionURL(info.versionURL);
	g_pGameMain->SetDownloadPort(info.downloadPort);
	//------------°Ó«°¶}Ãö-------------------------------------------	
	g_pGameMain->setMerchant(info.disableItemMall == 0);

	//ZShop
	g_pGameMain->SetEnableZShop((bool)info.enableZShop);
	g_pGameMain->SetZShopURL(info.ZShopURL);

	g_pGameMain->SetWBWidth(info.webBrowserWidth);
	g_pGameMain->SetWBHeight(info.webBrowserHeight);

	//-------------------------------------------------------
	// CLIENT ¥u¯à°ß¤@¶}±Ò
	if (g_pGameMain->CheckCountry("KR") == true)
	{
		HANDLE hLauncherInstance = ::CreateMutex(NULL, FALSE, _T(CHECK_LUNCHER_NAME));
		if (::GetLastError() == ERROR_ALREADY_EXISTS)
		{
			CloseHandle(hLauncherInstance);
			delete g_pGameMain;
			return -1;
		}
	}

	//-------------------------------------------------------
	//ª©¥»ÀË¬d
	if ((iCmdShow != 1 && iCmdShow != 10) || szCmdLine == NULL || (strnicmp(szCmdLine, "NoCheckVersion", 14) != 0))
	{
		if (g_pGameMain->CheckCountry("JP") == true)
		{
			if (strlen(szCmdLine) == 0)
			{
				ShellExecute(NULL, "open", info.activateURL, 0, 0, SW_SHOW);
				delete g_pGameMain;
				return 0;
			}
		}
		else if (g_pGameMain->CheckCountry("TW") == true)
		{
			wchar_t szPath[1024];
			wchar_t szDir[1024];

			GetCurrentDirectoryW(1024, szDir);
			swprintf_s(szPath, 1024, L"%s/gws/gwiswapper.exe", szDir);

			if (_waccess(szPath, 0) > -1)
			{
				ShellExecuteW(NULL, L"open", szPath, 0, szDir, SW_SHOW);
				delete g_pGameMain;
				return 0;
			}

		}
		else if (CheckHttpVersion("Update.ini") == false)
		{
			delete g_pGameMain;
			return 0;
		}
	}

	//-------------------------------------------------------

	//-------------------------------------------------------
	// °ê®a¥N½X¬°TW, ¨Ï¥Î°Ñ¼Æ¶Ç¤J±b±K


	if (g_pGameMain->CheckCountry("TW") || g_pGameMain->CheckCountry("JP") || g_pGameMain->CheckCountry("VN") || g_pGameMain->CheckCountry("SG") || g_pGameMain->IsGameForgeCountries())
	{
		CSplitArgString splitString(szCmdLine);
		string arg1, arg2;

		arg1 = splitString.GetString(0, 0);
		if (stricmp(arg1.c_str(), "NoCheckVersion") == 0)
		{
			arg1.clear();
			if (splitString.GetArgSize() > 1)
				arg1 = splitString.GetString(1, 1);
			if (splitString.GetArgSize() > 2)
				arg2 = splitString.GetString(2, 2);
		}
		else
		{
			if (splitString.GetArgSize() > 1)
				arg2 = splitString.GetString(1, 1);
		}

		if (g_pGameMain->IsGameForgeCountries())
		{

		}
		else
		{
			g_pGameMain->SetAccountName(arg1.c_str());
			g_pGameMain->SetAccountPassword(arg2.c_str());
		}
	}
	//-------------------------------------------------------

	//-------------------------------------------------------
	//»y­µ¶}Ãö
	if (info.disableVoice == 1)
		g_pGameMain->SetVoiceDisable(true);
	else
		g_pGameMain->SetVoiceDisable(false);
	//-------------------------------------------------------

	//-------------------------------------------------------
	//¤½·|¾Ô¶}Ãö
	if (info.disableGuildBattleGround == 1)
		g_pGameMain->SetGuildBattleGroundState(false);
	else
		g_pGameMain->SetGuildBattleGroundState(true);
	//-------------------------------------------------------

	//-------------------------------------------------------
	if (info.disableCh3Login == 1)
		g_pGameMain->SetCh3LoginState(false);
	else
		g_pGameMain->SetCh3LoginState(true);

	//-------------------------------------------------------
	g_pGameMain->SetLoginEpisodeVersion(info.loginEpisode);

	//-------------------------------------------------------

	//-------------------------------------------------------
	//Åã¥ÜVivox¨Ï¥ÎªÌ±ø¬ù¶}Ãö
	if (info.enableVivoxUserAgreement)
		g_pGameMain->SetVivoxUserAgreementState(true);
	else
		g_pGameMain->SetVivoxUserAgreementState(false);
	//-------------------------------------------------------

	//-------------------------------------------------------

	//-------------------------------------------------------

	//-------------------------------------------------------
	// Vivox initialize // move into RuneDevInitialize()
	//-------------------------------------------------------

	//-------------------------------------------------------
	// ¸ü¤JINI³]©w
#ifdef KALYDO
	g_pGameMain->LoadINI("RuneDev.ini", "eneu");
#else
	g_pGameMain->LoadINI("RuneDev.ini");
#endif
	//-------------------------------------------------------

	//-------------------------------------------------------
	//main loop
	INT reuslt = 0;

	SetCursor(LoadCursor(NULL, IDC_WAIT));

	if (g_pGameMain->RuneDevInitialize(hInstance, 1024, 768, TRUE))
	{
		reuslt = g_pGameMain->Run();

		g_pGameMain->SaveEnvironmentVariables();
		g_pGameMain->SaveSystemVariables();
}
	//-------------------------------------------------------

	//-------------------------------------------------------
	// Game Forge client library shutdown

	//-------------------------------------------------------

	//-------------------------------------------------------
	// µ²§ô¹CÀ¸
	CNetCli_Login_Child::Logout(EM_ClientNetCloseType_CloseWindow);

	g_pACTActionEventsManager->Release();
	//g_pGameMain->RuneDevRelease(); // markrd by slime. delete g_pGameMain will call RuneDevRelease()
	delete g_pGameMain;

	CLogList::Shutdown();
	//-------------------------------------------------------

	return reuslt;
}
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, INT iCmdShow)
{
	char dumpPath[250];
	char dumpName[50];
	INT reuslt = 0;
	__try
	{
		reuslt = RunApp(hInstance, hPrevInstance, szCmdLine, iCmdShow);
	}
	__except (GenerateCrashDumpFile(GetExceptionInformation(), "Client", MiniDumpWithDataSegs, dumpPath, dumpName))
	{
		UpdateInfInfo info;
		UpdateIni_GetCrashInfo(info);
		char tes[512];

		if (!IsDebuggerPresent() && dumpPath && dumpName && strcmp("", info.crashIP) != 0)
		{

			HINTERNET hInternet;
			HINTERNET hFtpSession;
			hInternet = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
			if (hInternet == NULL)
			{
				sprintf(tes, "Error: %lu", GetLastError());
				OutputDebugString(tes);
			}
			else
			{
				hFtpSession = InternetConnect(hInternet,
					info.crashIP,
					INTERNET_DEFAULT_FTP_PORT,
					info.crashAccount,
					info.crashPassword,
					INTERNET_SERVICE_FTP,
					0, 0
				);
				if (hFtpSession == NULL)
				{
					sprintf(tes, "ErrorFTp: %lu\n", GetLastError());
					OutputDebugString(tes);
				}
				else
				{
					if (!FtpPutFile(hFtpSession,
						dumpPath, dumpName,
						FTP_TRANSFER_TYPE_BINARY, 0
					))
					{
						sprintf(tes, "ErrorFTp: %lu\n", GetLastError());
						OutputDebugString(tes);
					}

					InternetCloseHandle(hFtpSession);
				}
				InternetCloseHandle(hInternet);
			}
			MessageBox(NULL, _T("The Crash has been reported!"), _T("Save Dialog"), MB_OK);
		}
		else
			MessageBox(NULL, _T("The Crash has been saved!"), _T("Save Dialog"), MB_OK);
		
	}
	return reuslt;
}
// --------------------------------------------------------------------------------------
void CGameMain::LoadLanguage()
{
	string tempStr;
	//char UpdateFileName[512];

	IniFile2 installIni;
	LoadIniFromZipFile(&installIni, "install.ini", UPDATEINF);

	installIni.SetSection("BOOT");

#ifdef KALYDO
	// use the language as selected on the site, ignore the ini file
	CTString select = application->getGameLanguage();
#else
	CTString select = installIni.StrDef("select", "tw");
#endif // KALYDO

	IniFile2 Localization;
	LoadIniFromZipFile(&Localization, "localization.ini", UPDATEINF);

	Localization.SetSection(select.c_str());

	// ¸ê®Æ»y¨t
	m_DataLanguage = Localization.StrDef("datalanguage", "rw");
	for (int i = 0; i < (int)m_DataLanguage.length(); i++)
		m_DataLanguage[i] = ::toupper(m_DataLanguage[i]);

	m_install = select;

	for (int i = 0; i < (int)m_install.length(); i++)
		m_install[i] = ::toupper(m_install[i]);

	// °ê®a¥N½X
	m_country = Localization.StrDef("country", select.c_str());
	for (int i = 0; i < (int)m_country.length(); i++)
		m_country[i] = ::toupper(m_country[i]);

	// Àx¦s¸ô®|
	m_saveFolder = Localization.StrDef("savefoldername", "");

	// ³]©wµøµ¡¦WºÙ
	tempStr = Localization.StrDef("windowname", "");
	if (tempStr.size() > 0)
	{
		wchar_t* wName = MultiByteToWChar(tempStr.c_str());
		if (wName)
		{
			char* name = TransToChar(wName);
			strcpy(m_windowName, name);
			delete[] name;
			delete[] wName;
		}
	}

	// ¯S§O¨Æ¥ó
	tempStr = Localization.StrDef("specialEventFilter", "");
	if (tempStr.size() > 0)
	{
		wchar_t* wName = MultiByteToWChar(tempStr.c_str());
		if (wName)
		{
			char* name = TransToChar(wName);
			strcpy(m_specialEventFilter, name);
			delete[] name;
			delete[] wName;
		}
	}

	// No Blood
	bool noblood = false;
	tempStr = Localization.StrDef("noblood", "");
	if (tempStr.size() > 0 && tempStr.compare("1") == 0)
	{
		noblood = true;
	}
	IBaseSprite::SetBloodDisable(noblood);

	//µn¤J®É, ¨Ï¥ÎªÌ¿é¤Jªº±K½X¬O§_¬°MD5 Hash¹Lªº±K½X
	m_IsMD5Password = (Localization.Int("IsMD5Password") == 1);

	//¦Û°ÊHash¨Ï¥ÎªÌ±K½XºX¼Ð
	m_IsAutoHashPassword = (Localization.Int("IsAutoHashPassword") == 1);

	//Hash±K½X¤è¦¡ 0=md5, 1=bcrypt
	m_PasswordHashType = (Localization.Int("PasswordHashType"));
}
// --------------------------------------------------------------------------------------
CGameMain::CGameMain()
{
	m_ip.clear();
	m_gameServerIP.clear();
	m_port = 0;
	m_gameServerPort = 0;

	m_currentGameTime = 0;
	m_gameTime = 0;
	m_startFrameTime = 0.0;

	m_playerID = -1;
	m_DBID = -1;
	m_zoneID = -1;
	m_roomID = -1;
	m_GItemID = -1;
	m_mapName.clear();
	m_receiveWaitLoading = false;
	m_isEnterWorld = false;
	m_needLoadTerrain = false;
	m_loadTerrainDelayTime = 0.0f;
	m_sendLoadOkDelayTime = 0.0f;

	m_isCombatState = false;
	m_pPlayer = NULL;
	m_pMouseInPlayer = NULL;
	m_sendMoveTime = 0.0f;

	m_firstJump = false;
	m_falling = false;
	m_pingState = false;
	m_startPing = 0;
	m_ping = 0;

	m_actorState.value = 0;
	m_playerState.value = 0;

	m_selfRevive = false;
	m_resurrect = false;
	m_resurrectTime = 0.0f;

	m_moveVector.Set(0.0f, 0.0f, 0.0f);
	m_direction.Set(0.0f, 0.0f, 0.0f);
	m_position.Set(0.0f, 0.0f, 0.0f);

	m_castSpellState = CASTSPELL_NONE;
	m_castSpellID = 0;
	m_castSpellIndex = -1;
	m_sendCastSpellCount = 0;
	m_isCancelCastSpell = false;
	m_lastCastSpellTime = 0.0;

	m_moveState.Bytes = 0;

	m_mouseMove.enable = false;
	m_mouseMove.distance = 0.0f;
	m_mouseMove.update = 0.0f;
	m_mouseMove.target.Set(0.0f, 0.0f, 0.0f);

	m_follow.target = NULL;
	m_follow.update = 0.0f;

	m_fear.dir = 0;
	m_fear.time = 0.0f;

	m_itemCasting.type = ITEM_CASTING_NONE;
	m_itemCasting.bagPos = -1;
	m_itemCasting.destType = -1;
	m_itemCasting.destPos = -1;

	m_castSpellToItem = 0;

	m_WorldMapID = -1;

	m_campState = false;
	m_campUpdateTime = 0.0f;
	m_parallelID = -1;

	m_displaySrvDebugMsg = false;
	m_PVPZoneState = EM_PK_Normal;

	m_totalFps = 0.0f;
	m_totalPing = 0.0f;
	m_totalLastTime = 0.0;
	m_totalCount = 0;

	m_secondPassword.clear();
	m_sendPassword = false;

	m_country = LANGUAGE_CODE_TW;
	m_language = LANGUAGE_CODE_TW;
	m_campTableId = 0;

	m_pathPositions.clear();
	m_pathTargetPosition.Set(0.0f, 0.0f, 0.0f);
	m_pathMoveTime = 0.0f;
	m_pathMoveUpdate = 0.0f;

	m_resetFontIndexStep = 0;
	m_cursorHyperlink = false;
	m_downloadPort = 0;

	memset(m_focusSprites, 0, sizeof(m_focusSprites));

	m_tombLeftTime = 0.0f;

	m_petGUID = 0;
	m_petItemID = 0;
	m_petMagicID = 0;

	m_cycleMagicId = 0;
	m_cycleMagicPos = 0;

	m_isBattleGround = false;
	m_isDisableTrdae = false;
	m_isDisableDuel = false;
	m_isDisableParty = false;
	m_isEnableBGParty = false;
	m_isWorldBattleGround = false;

	m_inWater = false;			// ¬O§_¦b¤ô¤¤

	m_numBattleGroundPlayers = 0;

	m_submergeFX = NULL;

	m_playTimeQuota = 0;
	m_QuotaUpdateTime = 0.0f;

	m_localIP = "000000000000";

	m_isReloadClientLua = false;
	m_showUpdateElapsedTime = false;

	m_lastDummySpriteUpdateTime = 0.0;

	m_packetReduceState = PACKET_REDUCE_NONE;
	m_packetReducePing = 0;
	m_nextPacketReduceUpdateTime = 0.0f;

	m_playerRefCount = 0;
	m_magicDistance = 0.0f;

	m_displayMaintainServer = false;

	m_botLocked = false;

	m_instanceWaitTime = 0.0f;			// µ¥«Ýª±®a¨M©w¬O§_¦P·N°Æ¥»¶i«×
	m_requestInstance = false;

	memset(m_version, 0, sizeof(m_version));
	memset(m_versionNO, 0, sizeof(m_versionNO));

	m_specialEventMap.clear();

	m_currentMemory = 0;
	m_memoryLoad = 0;
	m_lastMemoryUpdateTime = 0.0f;

	m_attachMountId = -1;
	m_isLoadClientData = false;

	m_inMount = false;
	m_inImplement = false;
	m_delayGetImplementTime = 0.0f;

	m_bIsVoiceSettingsDisable = false;

	m_lastClientSaveTime = 0.0f;				// ³Ì«á§ïÅÜÀx¦s®É¶¡

	m_illegalPositionCount = 0;
	m_lastCheckHeightTime = 0.0f;				// ¤W¦¸ÀË¬d¬ÛÂI®y¼Ð±×²v
	m_lastCheckHeightPosition.m_x = 0.0f;		// ¤W¦¸ÀË¬d®y¼ÐÂI
	m_lastCheckHeightPosition.m_y = 0.0f;		// ¤W¦¸ÀË¬d®y¼ÐÂI
	m_lastCheckHeightPosition.m_z = 0.0f;		// ¤W¦¸ÀË¬d®y¼ÐÂI

	m_clientSetting.Init();
	m_clientSettingBackup.Init();
	m_lastClientSaveTime = 0.0f;				// ³Ì«áÀx¦s®É¶¡

	m_isFreeCamera = false;
	m_cameraFollowSprite = NULL;

	m_targetLineType = TargetLineEffect_None;
	m_targetReferenceSprite = NULL;
	m_targetDirection.m_x = 0.0f;
	m_targetDirection.m_y = 0.0f;
	m_targetDirection.m_z = -1.0f;
	m_targetParabolaHeight = 50.0f;
	m_targetMaxRange = 250.0f;
	m_targetMaxAngle = 0.0f;
	m_targetDisplayDuration = 0.0f;

	m_reduceTextureState = false;		// ¬O§_­°§C§÷½èºë²Ó«×
	//m_paperdollTextureDetail	= ;				// ¨¤¦â­ì©l¶K¹Ïºë²Ó«×
	//m_textureDetail				= ;				// ¤@¯ë¶K¹Ïºë²Ó«×

	m_ZoneDescriptorList.clear();

	m_movePartitionEnable = false;			// ²¾°Ê­«·s³]©w¤À³Î°Ï
	m_vipDisplay = false;
	m_vipValid = false;
	m_bMerchantFrame = true;
	m_cameraControl = false;

	m_lastFixedPositionTime = 0.1f;

	m_checkMemoryTime = 0.0;
	m_isNoSwimming = false;

	m_boReportSelf = false;
	m_checkHeightCount = 0;

	//ZShop
	m_enableZShop = false;
	m_ZShopURL = "";

	//m_ProcessMemory.dwLength = sizeof(m_ProcessMemory);
#ifdef KALYDO
	// extra members for downloading WDB files or parts of them.
	m_HandleWDBPackage = NULL;
	m_isDownloading = false;
	m_shouldSetCameraPos = false;
#endif // KALYDO
}

// --------------------------------------------------------------------------------------
CGameMain::~CGameMain()
{
	for (vector<CRuAlgorithm_PatternMatch_FJS*>::iterator iter = m_patternMatch.begin(); iter != m_patternMatch.end(); iter++) {
		delete* iter;
	}
	m_patternMatch.clear();

	// ²M°£¾Ô³õÁ{®Éª«¥ó
	ClearAllBattleMember();
	}

// --------------------------------------------------------------------------------------
void CGameMain::LoadData()
{
	// ¸ü¤JData
	//CNetGlobal::Initial( GetFullFilePath(""), m_language.c_str() );
	CNetGlobal::Initial(GetFullFilePath(""), g_pGameMain->GetDataLanguage(), g_pGameMain->GetStringLanguage());

	// ¨ú±oª©¥»¸¹½X
	// sprintf_s(m_version, 256, "%s_%s", g_ObjectData->GetString("SYS_VERSION_NUMBER"), g_ObjectData->GetString("SYS_VERSION_LOCALE"));	

	/*
	// ¨ú±oµøµ¡¦WºÙ
	wchar_t* wName = MultiByteToWChar(g_ObjectData->GetString("SYS_WINDOW_TITLE_NAME"));
	if ( wName ) {
		char* name = TransToChar(wName);
		strcpy( m_windowName, name );
		delete [] name;
		delete [] wName;

		if ( m_hFrameWnd )
		{
			SetWindowText( m_hFrameWnd, m_windowName );
		}
	}
	*/

	// ¨ú±oª©¥»¸¹½X
	/*
	{
		char fullName[MAX_PATH];
		GetFullPathName( "Update.ini", MAX_PATH, fullName, NULL );
		CIni ini(fullName);

		ini.GetString( "BOOT", "Current_Version", m_version, sizeof(m_version), "x.xx.xxxx_xx" );
	}
	*/
}

// --------------------------------------------------------------------------------------
void CGameMain::Init()
{
	// ¸ü¤JFDBÀÉ®× 
	if (g_ruResourceManager)
	{
		g_ruResourceManager->LoadPackageFile("fdb\\other.fdb", true);
		g_ruResourceManager->LoadPackageFile("fdb\\interface.fdb", true);
		g_ruResourceManager->LoadPackageFile("fdb\\model.fdb", true);
		g_ruResourceManager->LoadPackageFile("fdb\\motion.fdb", true);
		g_ruResourceManager->LoadPackageFile("fdb\\music.fdb", true);
		g_ruResourceManager->LoadPackageFile("fdb\\sound.fdb", true);

		char temp[512];
		for (int i = 0; i <= 15; i++)
		{
			sprintf_s(temp, 512, "fdb\\texture%d.fdb", i);
			g_ruResourceManager->LoadPackageFile(temp, true);
		}
	}

	// ªì©l¤Æ¬É­±
	if (m_interface)
	{
		m_interface->Initialize(m_language.c_str(), m_DataLanguage.c_str(), m_country.c_str(), m_saveFolder.c_str());
	}

	// Åª¨úÀô¹Ò³]©wÅÜ¼Æ
	LoadEnvironmentVariables();

	// Initialize audio library
	CRuFileStream soundDBStream;
#ifdef KALYDO
	// cannot use the root dir, so find the sound.ini somewhere else
	if (soundDBStream.Open(GetFullFilePath("Bin/sound.ini"), ruSOM_OpenRead) == ruSEC_OK)
#else
	if (soundDBStream.Open(GetFullFilePath("sound.ini"), ruSOM_OpenRead) == ruSEC_OK)
#endif // KALYDO
	{
		char* soundDB = ruNEW char[soundDBStream.GetStreamSize() + 1];
		soundDBStream.Read(soundDB, soundDBStream.GetStreamSize());
		soundDB[soundDBStream.GetStreamSize()] = 0;
		luaL_dostring(m_pRuVM, soundDB);
		delete[] soundDB;
		soundDBStream.Close();
	}

	IBaseSprite::SetCheckEffectDisplayFunc(CheckEffectDisplay);

	// ³]©w´å¼Ð¹Ï§Î
	SetCursor(eCursor_Point);

	/*
	// ¨ú±o°O¾ÐÅé³Ì¤j¥i¥H¨Ï¥Î¶q
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof (statex);
	GlobalMemoryStatusEx (&statex);
	statex.ullTotalPhys /= 1024 * 1024;
	if ( statex.ullTotalPhys < 1024 )
	{
		RuPropertyType propertyValue;
		INT32 detailLevel = 0;

		// ¯È«½«½¶K¹Ïºë²Ó«×
		g_ruEngineSettings->GetSetting("paperdoll:max mip level", propertyValue);

		detailLevel = detailLevel = boost::get<INT32>(propertyValue);

		if ( detailLevel < 2 )
		{
			g_ruEngineSettings->SetSetting("texture:max detail level", 2);
		}

		// ¤@¯ë¶K¹Ïºë²Ó«×
		g_ruEngineSettings->GetSetting("texture:max detail level", propertyValue);

		detailLevel = detailLevel = boost::get<INT32>(propertyValue);

		if ( detailLevel > 2 )
		{
			g_ruEngineSettings->SetSetting("texture:max detail level", 2);
		}
	}
	else if ( statex.ullTotalPhys < 2048 )
	{
		RuPropertyType propertyValue;
		INT32 detailLevel = 0;

		// ¯È«½«½¶K¹Ïºë²Ó«×
		g_ruEngineSettings->GetSetting("paperdoll:max mip level", propertyValue);

		detailLevel = detailLevel = boost::get<INT32>(propertyValue);

		if ( detailLevel < 1 )
		{
			g_ruEngineSettings->SetSetting("texture:max detail level", 1);
		}

		// ¤@¯ë¶K¹Ïºë²Ó«×
		g_ruEngineSettings->GetSetting("texture:max detail level", propertyValue);

		detailLevel = detailLevel = boost::get<INT32>(propertyValue);

		if ( detailLevel > 3 )
		{
			g_ruEngineSettings->SetSetting("texture:max detail level", 3);
		}
	}
	else if ( statex.ullTotalPhys < 4048 )
	{
	}
	*/

	// ¸ü¤J¬É­±
	if (m_interface)
	{
		m_interface->SetProcessType(CInterface::eInterface_Process_Login);

		m_interface->BuildFramesLogin(m_UIWidth, m_UIHeight);

		/*
		int size = 0;
		if ( m_interface->GetUiLogin() )
			size = m_interface->GetUiLogin()->GetTotalMemSize();

		int a = 0;
		*/
	}

	// Load Login Screen
	LoadLoginScreen();
	g_pLoginParent->SendLoginScreen(GLUE_SCREEN_LOGIN);

	// ¸ü¤JCLINET LUA SCRIPT
	LoadClientLua();

	// ¦L¥§ÀY¤WÅã¥Ü¦rÅé©ñ¤j
	if (m_country.compare(LANGUAGE_CODE_ID) == 0)
	{
		IBaseSprite::SetFontScale(1.5f);
	}

	// ¨ú±o¯S®í¾BÁ×¦r¦ê
	char temp[512];
	sprintf_s(temp, 512, "Other\\%s\\specialwords.txt", GetImageLocation("SpecialWords"));
	IRuStream* stream = g_ruResourceManager->LoadStream(temp);
	if (stream) {
		const char* data = (const char*)stream->OpenStreamMapping();
		int size = (int)stream->GetStreamSize();

		// BOM Check
		if (size > 2 && (BYTE)data[0] == 0xEF && (BYTE)data[1] == 0xBB && (BYTE)data[2] == 0xBF) {
			data += 3;
			size -= 3;
		}

		BYTE line[4096];
		int loop, length, last;
		while (size > 0) {
			// ¨ú±o³æ¦æ¦r¦êªø«×
			length = size;
			loop = 0;
			while (loop < size) {
				if (data[loop] == '\n' || (data[loop] == 0x0d && data[loop - 1] != '\n')) {
					length = loop + 1;
					break;
				}
				loop++;
			}

			last = (length < 4096) ? length - 1 : 4095;
			memcpy(line, data, last + 1);
			if (loop < size)
				line[last] = 0;
			else
				line[++last] = 0;

			while (last > 0 && (line[last] == '\t' || line[last] == ' ' || line[last] == '\n' || line[last] == 13 || line[last] == 0)) last--;
			for (int i = 0; i <= last; i++) {
				if (line[i] >= 'A' && line[i] <= 'Z')
					line[i] += 32;
				else if (line[i] >= 0xC0 && line[i] <= 0xD6)
					line[i] += 32;
				else if (line[i] >= 0xD8 && line[i] <= 0xDE)
					line[i] += 32;
			}
			line[last + 1] = 0;

			size -= length;
			data += length;

			PushPatternString(line);
		}
		delete stream;
	}

	// ¸ü¤J APEX
	//CApexClient::Init();	

	/*
	// Allocate buffer and retrieve version information
	m_lpVersionData = new BYTE[dwDataSize];
	if (!::GetFileVersionInfo((LPTSTR)lpszModuleName, dwHandle, dwDataSize,
		(void**)m_lpVersionData) )
	{
		Close();
		return FALSE;
	}

	// Retrieve the first language and character-set identifier
	UINT nQuerySize;
	DWORD* pTransTable;
	if (!::VerQueryValue(m_lpVersionData, _T("\\VarFileInfo\\Translation"),
		(void **)&pTransTable, &nQuerySize) )
	{
		Close();
		return FALSE;
	}
	*/

	// Re-update window
	//	AdjustWindowSize(m_fullscreenResolution.cx, m_fullscreenResolution.cy, m_isWindowed);


	//¥u°O¿ý³Ì«á10µ§®Ä¯à¡A¨C30¬í­p¤@¦¸
	m_Fpslog.resize(10, 0);
	m_Cpulog.resize(10, 0);
	m_Memlog.resize(10, 0);
	m_SendPerformanceTime = m_frameTime - 2 * 60.0f;
	m_CpuUsage.GetUsage();//ªì©l¤ÆCPUUsage
	}

// --------------------------------------------------------------------------------------
void CGameMain::Release()
{
	// Interface member delete
	// InterfaceMemberRelease();

	//CApexClient::Release();

	CNetGlobal::Release();


}

// --------------------------------------------------------------------------------------
void CGameMain::Update()
{
	DWORD dwTimeBegin, dwNet, dwRune, dwWorld;

	if (m_showUpdateElapsedTime)
	{
		// g_pError->AddMessage(0, 0, "Update Elapsed Time:%.5f", m_elapsedTime);
	}

	// ®É¶¡±±¨î
	{
		time_t currTime = (time_t)(m_gameTime + (m_frameTime - m_startFrameTime));
		tm* newTime = localtime(&currTime);

		int temptimeGroup = m_timeGroup;

		switch (m_timeGroupType)
		{
		case 1:
			m_timeGroup = 0;
			break;
		case 2:
			m_timeGroup = 1;
			break;
		case 3:
			m_timeGroup = 2;
			break;
		case 4:
			m_timeGroup = 3;
			break;

		case 0:
		default:
			// Âà´«¬°¹CÀ¸³æ¦ì(¤À)
			m_currentGameTime = (newTime->tm_hour * 60 + newTime->tm_min) % 240;
			if (m_currentGameTime < 30)
				m_timeGroup = 0;
			else if (m_currentGameTime < 120)
				m_timeGroup = 1;
			else if (m_currentGameTime < 150)
				m_timeGroup = 2;
			else
				m_timeGroup = 3;
			break;
		}

		//¹CÀ¸¤p´£¥Ü
		/*
		if( m_controlArg.isShowGemePromrt &&
			temptimeGroup != m_timeGroup &&
			m_gameTime != 0 &&
			( m_frameTime - m_startFrameTime ) > 600 ) //¶}©l¹CÀ¸10¤ÀÄÁ¤~µo°T®§ Á×§KLUAÅÜ¼ÆÁÙ¨SÅª¶i¨Ó´Nµo¥X°T®§
		{
			m_interface->SendWorldEvent(UiEvent_ShowGemePromrt);
		}
		*/
	}

	// ÄÀ©ñÂÂªº¦a¹Ï¸ê°T(3¤ÀÄÁ)
	if (m_pOldRuTerrain && m_frameTime - m_lastLoadTerrainTime > 3.0f * 60.0f)
	{
		ReleaseOldTerrain();
	}


	// ÄÀ©ñ©Ò¦³¦r«¬¯Á¤Þ­È
	if (m_resetFontIndexStep > 0) {
		m_resetFontIndexStep--;

		if (m_resetFontIndexStep == 0)
		{
			CRuFontTexture2::ReleaseAllFontIndex();
		}
	}

	// ­«·s¸ü¤JClient Lua
	if (m_isReloadClientLua)
	{
		LoadClientLua();
		m_isReloadClientLua = false;
	}

	//
	if (m_resurrectTime > 0.0f)
	{
		m_resurrectTime -= m_elapsedTime;
	}

	map<int, float>::iterator iter = m_sendKeyTime.begin();
	while (iter != m_sendKeyTime.end())
	{
		iter->second -= m_elapsedTime;
		if (iter->second < 0.0f)
			iter = m_sendKeyTime.erase(iter);
		else
			iter++;
	}


	// Loading Terrain
	// ©µ¿ð¸ü¤J¦a¹Ï
	if (m_loadTerrainDelayTime > 0.0f)
	{
		m_loadTerrainDelayTime -= m_elapsedTime;
		if (m_loadTerrainDelayTime < 0.001f)
		{
			// ³]©w¦Û°Ê±±¨îÃèÀY
			SetCameraControllerEnable(true);

			// Ãö³¬¤W¤@±i¦a¹Ï­µ¼Ö
			g_pRuGlobalEventManager->StopMusic();

			// ¸ü¤J¦a¹Ï
			LoadTerrain(m_mapName.c_str());

			// §ó·sª±®a§¤¼Ð
			MovePlayerPos(m_changeTerrainPos);

			m_isFirstLoading = false;
		}
	}
	else if (m_interface->GetDelayReloadUiTime() > 0.0f)
	{
		// ©µ¿ð¸ü¤J¬É­±
	}
	else if (m_interface->GetUiWorld() && g_pLoadingFrame->GetLoadingState() && m_isFirstLoading == false)
	{
		// §PÂ_Åª¨ú¶i«×(¥D­n¦³¦a¹Ï»P¬É­±¸ü¤J)

		// ¦a¹ÏÅª¨ú
#ifdef KALYDO
		// we also want progress for downloads, so add that.
		float loadingStatus = min(GetTerrainLoadStatus(), 1.0f) * 0.25f;
		// start the game at 90% of the files
		loadingStatus += min(g_ruResourceManager->GetDownloadingStatus() * 1.1f, 1.0f) * 0.25f;
		loadingStatus += min(m_WDBDownloadProgress, 1.0f) * 0.25f;
#else
		float loadingStatus = min(GetTerrainLoadStatus(), 1.0f) * 0.5f;
#endif // KALYDO

		float xmlLoadingStatus = (m_interface) ? m_interface->GetXmlLoadingStatus() : 0.0f;

		// g_pError->AddMessage(0, 0, "GetTerrainLoadStatus %.4f", loadingStatus);		

		// ¬É­±Åª¨ú
		if (m_interface && CNetGlobal::RoleDataLoaded())
		{
			if (xmlLoadingStatus == 1.0f)
			{
				// ¦pªGÁÙ¦³·í¥¼¦^¨Óªº¸ê®Æ
				if (m_sendKeyTime.size() > 0)
				{
					xmlLoadingStatus = 0.99f;
				}
				else
				{
					m_interface->CloseWorldXml();
					CNetGlobal::SetUIFinish(true);
				}
	}
#ifdef KALYDO
			loadingStatus += xmlLoadingStatus * 0.25f;
#else
			loadingStatus += xmlLoadingStatus * 0.5f;
#endif // KALYDO
}

		if (loadingStatus < 0.99999f)
		{
			g_pLoadingFrame->SetLoadingStatus(loadingStatus);
		}
		else
		{
			// Force collision data to be reset each cycle for 3 cycles
			m_forceColDataResetCycles = 3;

			// ¸ü¤Jµ²§ô
			float delayTimeEnd = 0.3f;
			bool loadingOK = true;

			// ³qª¾¨¤¦â¤w¸g¥i¥H¶i¤J¥@¬É
			if (m_receiveWaitLoading)
			{
				// ©w¦ì¦a¹Ï©Ò¦b¦ì¸m
				UpdateEnvironment(m_changeTerrainPos, true);

				if (m_sendLoadOkDelayTime > 0.0001f)
				{
					loadingOK = false;
					m_sendLoadOkDelayTime -= m_elapsedTime;
				}
				else
				{
					m_receiveWaitLoading = false;
					Net_Move::S_ClientLoadingOK();
				}
			}

			// ÀË¬d©Ð«Î¬O§_»Ý­nµ¥«Ý«Ê¥]µ²§ô
			if (g_pHousesFrame->IsHousesLoading())
			{
			}
			else if (g_pGuildHousesFrame->IsHousesLoading())
			{
			}
			// ÀË¬d¥þ°ìª«¥ó»Ý­nµ¥«Ý¸ü¤J§¹²¦
			else if (IsGlobalLoadComplete() == false || IsAnchorHeightSpriteLoadComplete() == false)
			{
			}
			else if (loadingOK)
			{
				// ©Ð«Î¨t²Î©µ¿ð¸ü¤J®É¶¡
				delayTimeEnd += g_pHousesFrame->ClientLoadingOK();
				delayTimeEnd += g_pGuildHousesFrame->ClientLoadingOK();

				// µ²§ôLOADING
				g_pLoadingFrame->LoadingEnd(delayTimeEnd);
			}
		}
}

	/*
	{
	// ©µ¿ð¸ü¤J¦a¹Ï
	if ( m_loadTerrainDelayTime > 0.0f ) {
	m_loadTerrainDelayTime -= m_elapsedTime;
	if ( m_loadTerrainDelayTime < 0.001f ) {
	SetCameraControllerEnable(true);
	LoadTerrain(m_mapName.c_str());
	MovePlayerPos(m_changeTerrainPos);
	}
	} else if ( m_loadingTerrain ) {
	// ¸ü¤J¦a¹Ïµ¥«Ý¶i«×
	g_pLoadingFrame->SetLoadingStatus(m_pRuTerrain->GetLoadingStatus());
	} else if ( !g_pHousesFrame->IsHousesLoading() && m_receiveWaitLoading ) {	// ¸ü¤Jµ²§ô,³qª¾SERVER¥[¤J¨¤¦â¸ê®Æ
	float delayTimeEnd = 0.5f;

	// ©Ð«Î¨t²Î©µ¿ð¸ü¤J®É¶¡
	delayTimeEnd += g_pHousesFrame->ClientLoadingOK();
	m_receiveWaitLoading = false;
	Net_Move::S_ClientLoadingOK();

	// µ²§ôLOADING
	g_pLoadingFrame->LoadingEnd(delayTimeEnd);
	}
	}
	*/

	// Update action events manager
	if (g_pACTActionEventsManager)
		g_pACTActionEventsManager->Update(m_elapsedTime);

	// If force collision data reset cycles is nonzero, reset collision data for all actors
	if (m_forceColDataResetCycles > 0)
	{
		IBaseSprite::ResetAllCollisionData();

		// Update force collision data reset cycles temp variable
		m_forceColDataResetCycles = max(0, m_forceColDataResetCycles - 1);
	}

	// Process network events
	dwTimeBegin = GetTickCount();
	CNetGlobal::Process(m_frameTime, m_elapsedTime);
	dwNet = GetTickCount() - dwTimeBegin;


	// Invoke base class update
	dwTimeBegin = GetTickCount();
	CRuneDev::Update();
	dwRune = GetTickCount() - dwTimeBegin;

	// Update world
	dwTimeBegin = GetTickCount();
	if (m_isEnterWorld && CNetGlobal::RoleDataLoaded() && !g_pLoadingFrame->GetLoadingState()) {
		UpdateWorld();
	}
	dwWorld = GetTickCount() - dwTimeBegin;

	/*
	if ( m_pPlayer ) {
	static CRuVector3 lastPos;
	CRuVector3 pos = m_pPlayer->GetPos();
	CRuVector3 moveVector = m_pPlayer->GetMoveVector();
	float moveTime = m_pPlayer->GetMoveTime();
	if ( lastPos != pos ) {
	lastPos = pos;
	g_pError->AddMessage(0 ,0, "elapsedTime[%.3f] Player Type[%d] Pos(%.2f, %.2f, %.2f) Move[%.2f](%.2f, %.2f, %.2f)",
	m_elapsedTime, m_pPlayer->GetMoveType(), pos.m_x, pos.m_y, pos.m_z, moveTime, moveVector.m_x, moveVector.m_y, moveVector.m_z);
	}
	}
	*/

	// Check stuff related to the player sprite
	CRoleSprite* playerSpr = GetPlayer();
	if (playerSpr)
	{
		// Get player actor
		CRuFusion_Actor* playerActor = playerSpr->GetRuActor();

		// Determine if player is under water
		if (playerActor)
		{
			if (playerActor->IsSubmerged())
			{
				CRuWorld_WaterBodyDescriptor* waterBodyDescriptor = m_pRuTerrain->GetWaterBodyDescriptorAtPoint(playerSpr->GetPos());

				if (waterBodyDescriptor)
				{
					m_pRuWorldEnvironment->SetFogOverrideParameters(waterBodyDescriptor->m_submergedColor, waterBodyDescriptor->m_submergedFogStart, waterBodyDescriptor->m_submergedFogEnd, waterBodyDescriptor->m_submergedConstantFog);
				}
				else
				{
				}

				if (m_submergeFX == NULL)
				{
					m_submergeFX = ruNEW CRuRendererPlugin_SubmergeFX();
					m_submergeFX->RegisterPlugin(g_ruDefaultRenderContext);
					if (waterBodyDescriptor)
					{
						m_submergeFX->SetDistortionTextureTrack(&waterBodyDescriptor->m_track_SubmergedBumpName);
						m_submergeFX->SetDistortionStrength(waterBodyDescriptor->GetSubmergedBumpStrength());
					}
				}

				m_pRuWorldEnvironment->EnableFogOverride(TRUE);
				m_pRuWorldEnvironment->IncrementIteration();
			}
			else
			{
				ruSAFE_RELEASE(m_submergeFX);

				m_pRuWorldEnvironment->EnableFogOverride(FALSE);
				m_pRuWorldEnvironment->IncrementIteration();
			}
		}
		else
		{
			ruSAFE_RELEASE(m_submergeFX);
		}

		// Determine if the camera is supposed to be in first-person mode
		if (m_isFirstPersonCamera)
		{
			playerSpr->SetVisible(false);
		}
		else
		{
			playerSpr->SetVisible(true);
		}

		// Clear list of active collision triggers
		m_activeColTriggersThisCycle.Clear();

		// Get player position
		CRuVector3 playerPos = playerSpr->GetPos();

		// Iterate through sprites with collision plot triggers
		CRuArrayList<IBaseSprite*>& plotColSprites = IBaseSprite::GetPlotCollisionSprites();
		for (INT32 i = 0; i < plotColSprites.Count(); ++i)
		{
			if (playerSpr == plotColSprites[i])
				continue;

			CRuEntity* sprEntity = plotColSprites[i]->GetRuEntity();

			if (sprEntity)
			{
				// Perform a simple bounding check to make sure the collision distance is within tolerable range
				CRuSphere colSprWorldBounds = sprEntity->GetWorldBounds();
				if (colSprWorldBounds.Contains(playerPos))
				{
					// Perform an infinite ray check(up & down rays) against collision sprite triangles
					CRuSegment3 colTestSeg(playerPos - CRuVector3(0.0f, colSprWorldBounds.Radius(), 0.0f), playerPos + CRuVector3(0.0f, colSprWorldBounds.Radius(), 0.0f));

					CRuCollisionQuery_Segment_HitTest segQuery;

					segQuery.m_colSegment = colTestSeg;
					segQuery.m_exclusionMask = 0x00000000;
					segQuery.m_ignoreBackface = FALSE;
					segQuery.m_ignoreCollisionFlag = TRUE;

					sprEntity->QueryCollision(&segQuery);

					if (segQuery.m_positiveHit)
					{
						// Store identity ID
						m_activeColTriggersThisCycle.Add(plotColSprites[i]->GetIdentify());

						// Trigger collision event
						CNetCli_Script::OnEvent_CollisionTrigger(plotColSprites[i]->GetIdentify());
					}
				}
			}
		}

		// Iterate through list of sprites whose trigger was active last cycle and find ones which are no longer active
		for (INT32 i = 0; i < m_activeColTriggersLastCycle.Count(); ++i)
		{
			BOOL triggerDeactivated = TRUE;

			for (INT32 j = 0; j < m_activeColTriggersThisCycle.Count(); ++j)
			{
				if (m_activeColTriggersLastCycle[i] == m_activeColTriggersThisCycle[j])
				{
					triggerDeactivated = FALSE;
					break;
				}
			}

			if (triggerDeactivated)
			{
				CNetCli_Script::OnEvent_CollisionOutTrigger(m_activeColTriggersLastCycle[i]);
			}
		}

		// Copy
		m_activeColTriggersLastCycle.CloneFrom(m_activeColTriggersThisCycle);
	}
	else
	{
		ruSAFE_RELEASE(m_submergeFX);
	}

}

// --------------------------------------------------------------------------------------
float CGameMain::GetTerrainLoadStatus()
{
	if (m_loadTerrainDelayTime > 0.001f) {
		// ©µ¿ð¸ü¤J
		return 0.0f;
	}
	else if (m_loadingTerrain && m_pRuTerrain) {
		// ¨ú±o¦aªí¸ü¤J¶i«×
		return min(m_pRuTerrain->GetLoadingStatus(), 1.0f);
	}
	return 1.0f;
}

// --------------------------------------------------------------------------------------
// ­«·s³]©wª«¥ó¬O§_Åã¥Ü¦WºÙ
void CGameMain::ResetSpriteNameVisible(CRoleSprite* sprite)
{
	if (sprite) {
		bool visible = false;

		// ¬°¥Ø«e¥Ø¼Ð
		if (sprite == GetPlayerTarget()) {
			visible = true;
		}
		else if (sprite == GetPlayer()) {
			visible = m_clientSetting.isSelfTitleVisible;
		}
		else {
			switch (sprite->GetObjectType())
			{
			case eSpriteObject_Player:
				visible = m_clientSetting.isPlayerTitleVisible;
				break;

			default:
				visible = m_clientSetting.isNPCTitleVisible;
				break;
			}
		}
		sprite->SetNameVisibleForce(visible);
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::SetSpriteGuildID(CRoleSprite* sprite, int guildID)
{
	CRoleSprite* player = GetPlayer();
	if (sprite && sprite->GetGuildID() != guildID && sprite->GetObjectType() == eSpriteObject_Player)
	{
		sprite->SetGuildID(guildID);

		if (guildID != 0 && m_clientSetting.isGuildVisible) {
			sprite->SetGuildText(NetCli_GuildChild::GetGuildNameByID(guildID));
		}
		else {
			sprite->SetGuildText(NULL);
		}

		// §ó·s¤½·|¼Ä¹ï¸ê°T
		GuildEnemyUpdate();
	}
}

// --------------------------------------------------------------------------------------
// ¤½·|¼Ä¹ï¸ê°T§ó·s
void CGameMain::GuildEnemyUpdate()
{
	CRoleSprite* player = m_pPlayer;
	if (player == NULL)
		return;

	// ·j´M©Ò¦³¼Ä¹ï¤½·|	
	vector<int> enemyGuild;
	{
		vector<GuildDeclareWarInfo>& guildDeclare = NetCli_GuildChild::s_declareWarInfo;
		for (vector<GuildDeclareWarInfo>::iterator iter = guildDeclare.begin(); iter != guildDeclare.end(); iter++)
		{
			if (iter->bothID[0] == player->GetGuildID()) {
				enemyGuild.push_back(iter->bothID[1]);
			}
			else if (iter->bothID[1] == player->GetGuildID()) {
				enemyGuild.push_back(iter->bothID[0]);
			}
		}
	}

	for (map<int, CRoleSprite*>::iterator iter = m_spriteContainer.begin(); iter != m_spriteContainer.end(); iter++)
	{
		CRoleSprite* sprite = iter->second;

		if (sprite->GetObjectType() != eSpriteObject_Player /* || (player->GetIdentify() >> 16) != (sprite->GetIdentify() >> 16)*/)
			continue;

		if (sprite != player)
		{
			bool enemy = false;
			for (vector<int>::iterator enemy_iter = enemyGuild.begin(); enemy_iter != enemyGuild.end(); enemy_iter++)
			{
				if (sprite->GetGuildID() == *enemy_iter)
				{
					enemy = true;
					break;
				}
			}
			sprite->SetPVPGuildState(enemy);

			// ¦å±ø§ïÅÜ
			g_pObjectBloodBar->ChangeState(sprite->GetIdentify());
		}
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::UpdatePVPZoneState(int state)
{
	if (state != -1)
	{
		switch (state)
		{
		case EM_PK_Normal:
			if (m_PVPZoneState != state)
				SendSystemMsgEx("%s%s%s", "|cff00ff00", g_ObjectData->GetString("SYS_PK_TYPE1"), "|r");
			break;

		case EM_PK_All:
			SendSystemMsgEx("%s%s%s", "|cffff0000", g_ObjectData->GetString("SYS_PK_TYPE2"), "|r");
			break;

		case EM_PK_Party:
			SendSystemMsgEx("%s%s%s", "|cffff0000", g_ObjectData->GetString("SYS_PK_TYPE3"), "|r");
			break;

		case EM_PK_Party_Guild:
			SendSystemMsgEx("%s%s%s", "|cffff0000", g_ObjectData->GetString("SYS_PK_TYPE4"), "|r");
			break;
		}
		m_PVPZoneState = state;
	}

	CRoleSprite* player = GetPlayer();
	if (player == NULL)
		return;

	// ½d³ò§ïÅÜ
	for (map<int, CRoleSprite*>::iterator iter = m_spriteContainer.begin(); iter != m_spriteContainer.end(); iter++)
	{
		CRoleSprite* sprite = iter->second;
		if (sprite->GetObjectType() != eSpriteObject_Player || (player->GetIdentify() >> 16) != (sprite->GetIdentify() >> 16))
			continue;

		bool enemy = false;

		// ¦Û¤v
		if (sprite == player)
		{
			enemy = false;
		}
		// ¦P¶¤¤Í
		else if (sprite->GetPartyState())
		{
			if (m_PVPZoneState == EM_PK_All)
				enemy = true;
		}
		// ¦P¤½·|
		else if (player->GetGuildID() > 0 && sprite->GetGuildID() == player->GetGuildID())
		{
			if (m_PVPZoneState == EM_PK_All || m_PVPZoneState == EM_PK_Party)
				enemy = true;
		}
		else
		{
			if (m_PVPZoneState != EM_PK_Normal)
				enemy = true;
		}

		// ª¬ºA§óÅÜ
		if (sprite->GetPVPZoneState() != enemy)
		{
			sprite->SetPVPZoneState(enemy);

			// ¦å±ø§ïÅÜ
			g_pObjectBloodBar->ChangeState(sprite->GetIdentify());
		}
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::ChangeGuildName(int guildID)
{
	// ·j´M©Ò¦³¤Hª¬ºA§ïÅÜ
	for (map<int, CRoleSprite*>::iterator iter = m_spriteContainer.begin(); iter != m_spriteContainer.end(); iter++)
	{
		CRoleSprite* sprite = iter->second;
		if (sprite->GetGuildID() == guildID)
		{
			if (guildID != 0 && m_clientSetting.isGuildVisible) {
				sprite->SetGuildText(NetCli_GuildChild::GetGuildNameByID(guildID));
			}
			else {
				sprite->SetGuildText(NULL);
			}
		}
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::SetCampState(bool state)
{
	if (m_campState != state)
	{
		m_campState = state;
		m_campUpdateTime = 0.0f;
		if (m_campState) {
			NetCli_RoleValue::S_Logout();
		}
		else {
			NetCli_RoleValue::S_CancelLogout();
			m_interface->SendWorldEvent("FORCE_DIALOG_HIDE");
		}
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::ChangeParallelID(int id)
{
	if (m_parallelID != id)
	{
		m_parallelID = id;
		if (m_parallelID >= 0) {
			NetCli_RoleValue::S_ChangeParallel(m_parallelID);
		}
		else {
			NetCli_RoleValue::S_CancelChangeParallel();
			m_interface->SendWorldEvent("FORCE_DIALOG_HIDE");
		}
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::ClearTotalCount()
{
	m_totalFps = 0.0f;
	m_totalPing = 0.0f;
	m_totalCount = 0;
}

//-------------------------------------------------------------------------------------
//¨ú±oOS¸ê°Tc++
//-------------------------------------------------------------------------------------
bool CGameMain::GetOSDisplayString(char* pszOS, size_t cchLen)
{
	OSVERSIONINFOEX2 osvi;
	SYSTEM_INFO si;
	GETNATIVESYSTEMINFO pGNSI;
	GETPRODUCTINFO pGPI;
	BOOL bOsVersionInfoEx;
	DWORD dwType;

	ZeroMemory(&si, sizeof(SYSTEM_INFO));
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));

	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO*)&osvi);
	if (!bOsVersionInfoEx)
		return false;

	// Call GetNativeSystemInfo if supported or GetSystemInfo otherwise.

	pGNSI = (GETNATIVESYSTEMINFO)GetProcAddress(
		GetModuleHandle(TEXT("kernel32.dll")), "GetNativeSystemInfo");
	if (NULL != pGNSI)
		pGNSI(&si);
	else
		GetSystemInfo(&si);

	if (VER_PLATFORM_WIN32_NT == osvi.dwPlatformId && osvi.dwMajorVersion > 4)
	{
		StringCchCat(pszOS, cchLen, TEXT("Microsoft "));

		// Test for the specific product.
		if (osvi.dwMajorVersion == 6)
		{
			if (osvi.dwMinorVersion == 0)
			{
				if (osvi.wProductType == VER_NT_WORKSTATION)
					StringCchCat(pszOS, cchLen, TEXT("Windows Vista "));
				else
					StringCchCat(pszOS, cchLen, TEXT("Windows Server 2008 "));
			}
			else if (osvi.dwMinorVersion == 1)
				StringCchCat(pszOS, cchLen, TEXT("Windows 7 "));

			pGPI = (GETPRODUCTINFO)GetProcAddress(
				GetModuleHandle(TEXT("kernel32.dll")), "GetProductInfo");
			if (pGPI)
			{
				pGPI(6, 0, 0, 0, &dwType);
			}

			switch (dwType)
			{
			case PRODUCT_ULTIMATE:
				StringCchCat(pszOS, cchLen, TEXT("Ultimate Edition"));
				break;
			case PRODUCT_HOME_PREMIUM:
				StringCchCat(pszOS, cchLen, TEXT("Home Premium Edition"));
				break;
			case PRODUCT_HOME_BASIC:
				StringCchCat(pszOS, cchLen, TEXT("Home Basic Edition"));
				break;
			case PRODUCT_ENTERPRISE:
				StringCchCat(pszOS, cchLen, TEXT("Enterprise Edition"));
				break;
			case PRODUCT_BUSINESS:
				StringCchCat(pszOS, cchLen, TEXT("Business Edition"));
				break;
			case PRODUCT_STARTER:
				StringCchCat(pszOS, cchLen, TEXT("Starter Edition"));
				break;
			case PRODUCT_CLUSTER_SERVER:
				StringCchCat(pszOS, cchLen, TEXT("Cluster Server Edition"));
				break;
			case PRODUCT_DATACENTER_SERVER:
				StringCchCat(pszOS, cchLen, TEXT("Datacenter Edition"));
				break;
			case PRODUCT_DATACENTER_SERVER_CORE:
				StringCchCat(pszOS, cchLen, TEXT("Datacenter Edition (core installation)"));
				break;
			case PRODUCT_ENTERPRISE_SERVER:
				StringCchCat(pszOS, cchLen, TEXT("Enterprise Edition"));
				break;
			case PRODUCT_ENTERPRISE_SERVER_CORE:
				StringCchCat(pszOS, cchLen, TEXT("Enterprise Edition (core installation)"));
				break;
			case PRODUCT_ENTERPRISE_SERVER_IA64:
				StringCchCat(pszOS, cchLen, TEXT("Enterprise Edition for Itanium-based Systems"));
				break;
			case PRODUCT_SMALLBUSINESS_SERVER:
				StringCchCat(pszOS, cchLen, TEXT("Small Business Server"));
				break;
			case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM:
				StringCchCat(pszOS, cchLen, TEXT("Small Business Server Premium Edition"));
				break;
			case PRODUCT_STANDARD_SERVER:
				StringCchCat(pszOS, cchLen, TEXT("Standard Edition"));
				break;
			case PRODUCT_STANDARD_SERVER_CORE:
				StringCchCat(pszOS, cchLen, TEXT("Standard Edition (core installation)"));
				break;
			case PRODUCT_WEB_SERVER:
				StringCchCat(pszOS, cchLen, TEXT("Web Server Edition"));
				break;
			default:
				StringCchCat(pszOS, cchLen, TEXT("Unknown"));
				break;
			}
			if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
				StringCchCat(pszOS, cchLen, TEXT(", 64-bit"));
			else if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL)
				StringCchCat(pszOS, cchLen, TEXT(", 32-bit"));
		}

		if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2)
		{
			if (GetSystemMetrics(SM_SERVERR2))
				StringCchCat(pszOS, cchLen, TEXT("Windows Server 2003 R2, "));
			else if (osvi.wSuiteMask == VER_SUITE_STORAGE_SERVER)
				StringCchCat(pszOS, cchLen, TEXT("Windows Storage Server 2003"));
			else if (osvi.wProductType == VER_NT_WORKSTATION
				&& si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
				StringCchCat(pszOS, cchLen, TEXT("Windows XP Professional x64 Edition"));
			else
				StringCchCat(pszOS, cchLen, TEXT("Windows Server 2003, "));

			// Test for the server type.
			if (osvi.wProductType != VER_NT_WORKSTATION)
			{
				if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
				{
					if ((osvi.wSuiteMask & VER_SUITE_DATACENTER) == VER_SUITE_DATACENTER)
						StringCchCat(pszOS, cchLen, TEXT("Datacenter Edition for Itanium-based Systems"));
					else if ((osvi.wSuiteMask & VER_SUITE_ENTERPRISE) == VER_SUITE_ENTERPRISE)
						StringCchCat(pszOS, cchLen, TEXT("Enterprise Edition for Itanium-based Systems"));
				}
				else if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
				{
					if ((osvi.wSuiteMask & VER_SUITE_DATACENTER) == VER_SUITE_DATACENTER)
						StringCchCat(pszOS, cchLen, TEXT("Datacenter x64 Edition"));
					else if ((osvi.wSuiteMask & VER_SUITE_ENTERPRISE) == VER_SUITE_ENTERPRISE)
						StringCchCat(pszOS, cchLen, TEXT("Enterprise x64 Edition"));
					else
						StringCchCat(pszOS, cchLen, TEXT("Standard x64 Edition"));
				}
				else
				{
					if ((osvi.wSuiteMask & VER_SUITE_COMPUTE_SERVER) == VER_SUITE_COMPUTE_SERVER)
						StringCchCat(pszOS, cchLen, TEXT("Compute Cluster Edition"));
					else if ((osvi.wSuiteMask & VER_SUITE_DATACENTER) == VER_SUITE_DATACENTER)
						StringCchCat(pszOS, cchLen, TEXT("Datacenter Edition"));
					else if ((osvi.wSuiteMask & VER_SUITE_ENTERPRISE) == VER_SUITE_ENTERPRISE)
						StringCchCat(pszOS, cchLen, TEXT("Enterprise Edition"));
					else if ((osvi.wSuiteMask & VER_SUITE_BLADE) == VER_SUITE_BLADE)
						StringCchCat(pszOS, cchLen, TEXT("Web Edition"));
					else
						StringCchCat(pszOS, cchLen, TEXT("Standard Edition"));
				}
			}
		}
		if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)
		{
			StringCchCat(pszOS, cchLen, TEXT("Windows XP "));
			if ((osvi.wSuiteMask & VER_SUITE_PERSONAL) == VER_SUITE_PERSONAL)
				StringCchCat(pszOS, cchLen, TEXT("Home Edition"));
			else
				StringCchCat(pszOS, cchLen, TEXT("Professional"));
		}

		if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0)
		{
			StringCchCat(pszOS, cchLen, TEXT("Windows 2000 "));
			if (osvi.wProductType == VER_NT_WORKSTATION)
			{
				StringCchCat(pszOS, cchLen, TEXT("Professional"));
			}
			else
			{
				if ((osvi.wSuiteMask & VER_SUITE_DATACENTER) == VER_SUITE_DATACENTER)
					StringCchCat(pszOS, cchLen, TEXT("Datacenter Server"));
				else if ((osvi.wSuiteMask & VER_SUITE_ENTERPRISE) == VER_SUITE_ENTERPRISE)
					StringCchCat(pszOS, cchLen, TEXT("Advanced Server"));
				else
					StringCchCat(pszOS, cchLen, TEXT("Server"));
			}
		}
		// Include service pack (if any) and build number.
		size_t cchVerLen = 0;
		StringCchLength(osvi.szCSDVersion, MAX_PATH, &cchVerLen);
		if (cchVerLen > 0)
		{
			StringCchCat(pszOS, cchLen, TEXT(" "));
			StringCchCat(pszOS, cchLen, osvi.szCSDVersion);
		}

		TCHAR szBuf[80];
		StringCchPrintf(szBuf, 80, TEXT(" (build %d)"), osvi.dwBuildNumber);
		StringCchCat(pszOS, cchLen + 80, szBuf);

		return true;
	}
	else
	{
		//Does not support this version of Windows.
		return false;
	}
}
// --------------------------------------------------------------------------------------
// §ó·sª±®a PVP BUFF
// ¦]¬°¦³µ¥¯Å»P¤p¶¤§ïÅÜ,¨ä¥Lª±®aBUFF§ïÅÜ,¤Ó¦h¨Æ¥óÄ²µo¦]¦¹³£§ï¬°²Î¤@®É¶¡§ó·s
void CGameMain::UpdateSpritePKBuff()
{
	CRoleSprite* player = GetPlayer();
	if (player == NULL)
		return;

	// §ïÅÜ©Ò¦³ª«¥ó
	for (map<int, CRoleSprite*>::iterator iter = m_spriteContainer.begin(); iter != m_spriteContainer.end(); iter++)
	{
		CRoleSprite* sprite = iter->second;
		if (sprite->GetObjectType() != eSpriteObject_Player)
			continue;

		bool isAttack = false;
		if (GetPVEState())
		{
			// ¦P¤p¶¤
			if (player == sprite || sprite->GetPartyState())
				isAttack = false;
			// ¹ï¤è¦³«OÅ@
			else if (sprite->GetProtectBuffState())
				isAttack = false;
			// Âù¤è³£¶}±Ò
			else if (player->GetPKBuffState() && sprite->GetPKBuffState())
				isAttack = true;
		}
		else
		{
			// ¦P¤p¶¤
			if (player == sprite || sprite->GetPartyState())
				isAttack = false;
			// ¹ï¤è¦³«OÅ@
			else if (sprite->GetProtectBuffState())
				isAttack = false;
			// ¹ï¤è¬°¸o¥Ç¦Û¤v«D·s¤â
			else if (sprite->GetCriminalBuffState() && player->GetCampID() > 0)
				isAttack = true;
			// ¦Û¤v¬°¸o¥Ç¹ï¤è«D·s¤â
			else if (player->GetCriminalBuffState() && sprite->GetCampID() > 0)
				isAttack = true;
			// ¦Û¤v¶}±ÒPK,¹ï¤è¤£¬°·s¤â
			else if (player->GetPKBuffState() && sprite->GetCampID() > 0)
				isAttack = true;
			// ¹ï¤è¶}±ÒPK,¦Û¤v¬O«D·s¤â
			else if (sprite->GetPKBuffState() && player->GetCampID() > 0)
				isAttack = true;
			// ¦Û¤v¸ò¹ï¤è³£¬°·s¤â,»Ý­nÂù¤è³£¶}±ÒPK
			else if (player->GetCampID() == 0 && sprite->GetCampID() == 0 && player->GetPKBuffState() && sprite->GetPKBuffState())
				isAttack = true;
		}

		if (sprite->GetPVPState() != isAttack)
		{
			// ª¬ºA§ïÅÜ
			sprite->SetPVPState(isAttack);

			// ¦å±ø§ïÅÜ
			g_pObjectBloodBar->ChangeState(sprite->GetIdentify());
		}
	}
}

// --------------------------------------------------------------------------------------
bool CGameMain::GetSpritePVPState(CRoleSprite* sprite)
{
	if (sprite == NULL)
		return false;

	// ½d³ò°Ï°ìPK
	if (m_PVPZoneState != EM_PK_Normal)
		return true;

	// ·j´M©Ò¦³¼Ä¹ï¤½·|	
	vector<GuildDeclareWarInfo>& guildDeclare = NetCli_GuildChild::s_declareWarInfo;
	for (vector<GuildDeclareWarInfo>::iterator iter = guildDeclare.begin(); iter != guildDeclare.end(); iter++)
	{
		if (iter->bothID[0] == sprite->GetGuildID() || iter->bothID[1] == sprite->GetGuildID())
			return true;
	}

	// ÀË¬dBUFFPK
	SpriteSpecialEffect effect = sprite->GetSpecailEffect();
	if (effect.pvp > 1)
		return true;

	return false;
}

// --------------------------------------------------------------------------------------
void CGameMain::SetSpriteCampId(CRoleSprite* sprite, int campId, bool force)
{
	CRoleSprite* player = GetPlayer();
	if (sprite == NULL)
		return;

	if (force == false && sprite->GetCampID() == campId)
		return;

	switch (sprite->GetObjectType())
	{
	case eSpriteObject_Player:
	case eSpriteObject_NPC:
	case eSpriteObject_QuestNPC:
		break;

	default:
		return;
	}

	sprite->SetCampID((CampID_ENUM)campId);
	if (player == NULL)
		return;

	if (sprite == player)
	{
		// ·j´M©Ò¦³¤Hª¬ºA§ïÅÜ
		for (map<int, CRoleSprite*>::iterator iter = m_spriteContainer.begin(); iter != m_spriteContainer.end(); iter++)
		{
			CRoleSprite* sprite = iter->second;
			if (sprite == player)
				continue;

			switch (sprite->GetObjectType())
			{
			case eSpriteObject_Player:
			case eSpriteObject_NPC:
			case eSpriteObject_QuestNPC:
				SetSpriteCampId(sprite, sprite->GetCampID(), true);
				break;
			}
		}
	}
	else
	{
		GameObjDbStruct* campTable = CNetGlobal::GetObj(m_campTableId);
		if (campTable == NULL)
			return;

		int camp1 = min(max(player->GetCampID(), 0), _MAX_CAMP_COUNT_ - 1);
		int camp2 = min(max(sprite->GetCampID(), 0), _MAX_CAMP_COUNT_ - 1);

		bool enemy = false;
		bool attack = false;
		switch (campTable->Camp.CampInfo[camp1][camp2])
		{
		case EM_GameCampType_Alliance:		//¦P·ù¤£¥i§ðÀ» ( ·|À°¦£ )
			enemy = false;
			attack = false;
			break;

		case EM_GameCampType_Friend:		//¤Íµ½( ·|À°¦£ ) ¡A ¥i±j¨î§ðÀ»
		case EM_GameCampType_Neutrality:	//¤¤¥ß
			enemy = false;
			attack = true;
			break;

		case EM_GameCampType_Enemy:			//¼Ä¤H
			enemy = true;
			attack = true;
			break;
		}

		// NPCª«¥ó
		switch (sprite->GetObjectType())
		{
		case eSpriteObject_NPC:
		case eSpriteObject_QuestNPC:
		{
			GameObjDbStructEx* NPC = CNetGlobal::GetObj(sprite->GetObjectId());
			if (NPC->IsNPC() && NPC->NPC.SearchRange == 0)
				enemy = false;
		}
		break;
		}

		sprite->SetCampEnemyState(enemy);
		sprite->SetCampAttackState(attack);
		g_pObjectBloodBar->ChangeState(sprite->GetIdentify());
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::ResetObjectGuildFlag(int guildID, const char* insignia, int insigniaMainColor, int insigniaOffColor, const char* banner, int bannerMainColor, int bannerOffColor)
{
	for (map<int, CRoleSprite*>::iterator iter = m_spriteContainer.begin(); iter != m_spriteContainer.end(); iter++)
	{
		CRoleSprite* sprite = iter->second;
		if (sprite->GetGuildID() == guildID && sprite->GetOrnamentID())
		{
			sprite->CreateOrnamentEntity(sprite->GetOrnamentID(), insignia, insigniaMainColor, insigniaOffColor, banner, bannerMainColor, bannerOffColor);
			//sprite->PaperdollBuild();
		}
	}
}

// --------------------------------------------------------------------------------------
bool CGameMain::CheckSecondPassword()
{
	if (m_secondPassword.empty())
	{
		// ¤w¸g°e¥Xµ¥«Ý¦^À³
		if (m_sendPassword)
		{
			SendWarningMsg(g_ObjectData->GetString("SYS_PASSWORD_WAIT_CONFIRM"));
		}
		// ÁÙ¥¼°e¥X«Ê¥]
		else if (g_pInterface)
		{
			// Åã¥Ü¬É­±
			g_pInterface->SendWorldEvent("PASSWORD_CONFIRM");
		}
		return false;
	}
	return true;
}


// --------------------------------------------------------------------------------------
void CGameMain::SetSecondPassword(const char* password)
{
	m_secondPassword = password;
	m_sendPassword = false;

	if (!m_secondPassword.empty())
	{
		lua_State* L = g_pInterface->GetLuaState();
		if (L)
		{
			lua_getglobal(L, LUA_PASSWORD_FUNC);
			if (lua_isfunction(L, -1))
			{
				lua_call(L, 0, 0);
			}
		}
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::SendSecondPassword(const char* password)
{
	m_sendPassword = true;
	NetCli_Other::S_CheckPassword(password);
}

// --------------------------------------------------------------------------------------
void CGameMain::ClearSecondPassword()
{
	m_secondPassword.clear();
	m_sendPassword = false;
}

// --------------------------------------------------------------------------------------
void CGameMain::SetCampTableObj(int id)
{
	if (m_campTableId != id)
	{
		m_campTableId = id;

		CRoleSprite* player = GetPlayer();
		if (player)
		{
			SetSpriteCampId(player, player->GetCampID(), true);
		}
	}
}

// --------------------------------------------------------------------------------------
const char* CGameMain::GetImageLocation(const char* section)
{
	const char* location = m_language.c_str();

	if (section != NULL)
	{
		// «O¯d¥~³¡¨Ï¥Î
		static string imageLocationString;
		imageLocationString.clear();

		if (stricmp("Tutorial", section) == 0)
			imageLocationString = GetImageSwitch("Tutorial", location, location);
		else if (stricmp("TutorialFrame", section) == 0)
			imageLocationString = GetImageSwitch("TutorialFrame", location, location);
		else if (stricmp("WorldMap", section) == 0)
		{
			imageLocationString = GetImageSwitch("WorldMap", location, location);
			if ((stricmp(imageLocationString.c_str(), "ENEU") == 0) && (stricmp(m_country.c_str(), "SG") == 0))
				imageLocationString = "RW";
		}
		else if (stricmp("Logo", section) == 0)
		{
			location = m_country.c_str();
			imageLocationString = GetImageSwitch("Logo", location, location);
		}
		else if (stricmp("LoadingScreen", section) == 0)
			imageLocationString = GetImageSwitch("LoadingScreen", location, location);
		else if (stricmp("SpecialWords", section) == 0)
			imageLocationString = m_country.c_str();
		else if (stricmp("ClassImage", section) == 0)
			imageLocationString = GetImageSwitch("ClassImage", location, location);
		else if (stricmp("FightImage", section) == 0)
		{
			imageLocationString = GetImageSwitch("FightImage", location, "");
			location = imageLocationString.c_str();
		}

		if (imageLocationString.length() > 0)
			location = imageLocationString.c_str();
	}

	return location;
}

// --------------------------------------------------------------------------------------
void CGameMain::SendCreateRoleSprite(CRoleSprite* pRoleSprite)
{
	CNetCli_Script::OnEvent_AddObj(pRoleSprite->GetIdentify());
}

// --------------------------------------------------------------------------------------
void CGameMain::SendDeleteRoleSprite(CRoleSprite* pRoleSprite)
{
	if (pRoleSprite == m_pPlayer)
	{
		SetPlayerTarget(NULL);
		SetMouseInPlayer(NULL);
		FollowSprite(NULL);
		m_sendSpellTarget = NULL;
		m_pPlayer = NULL;
	}
	if (pRoleSprite == m_sendSpellTarget)
		m_sendSpellTarget = NULL;

	if (pRoleSprite == m_pMouseInPlayer)
		SetMouseInPlayer(NULL);

	if (pRoleSprite == m_follow.target)
		FollowSprite(NULL);

	if (pRoleSprite == GetPlayerTarget())
		SetPlayerTarget(NULL);

	if (pRoleSprite == m_mouseMove.sprite)
		ClearMoveToPoint();

	if (pRoleSprite == m_cameraFollowSprite)
		SetCameraFollowSprite(NULL);

	if (pRoleSprite == m_targetReferenceSprite)
		ReleaseTargetEffect();

	// ²M°£µJÂIª«¥ó
	for (int i = 0; i < MAX_FOCUS_SPRITES; i++)
	{
		if (pRoleSprite == m_focusSprites[i]) SetFocusSprite(i, NULL);
	}

	if (pRoleSprite->GetContainerState())
	{
		CNetCli_Script::OnEvent_DelObj(pRoleSprite->GetIdentify());
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::SendPlayerGItemIDChange(CRoleSprite* pRoleSprite, int GItemID)
{
	if (pRoleSprite)
	{
		int oldID = pRoleSprite->GetIdentify();
		if (oldID != GItemID)
			g_pObjectBloodBar->ChangeGameID(oldID, GItemID);

	}
	if (m_pPlayer && pRoleSprite)
	{
		CRoleSprite* player = m_pPlayer;
		if (player->GetAttachParentId() == pRoleSprite->GetIdentify())
		{
			// ÀË¬d¦Û¤v¥¼§ó´«°Ï°ì,µ¥«Ý´«°Ï°ì®É¦bÄ²µo
			if ((m_GItemID & 0xFFFF0000) != (GItemID & 0xFFFF0000))
				m_attachMountId = GItemID;
			else
			{
				Net_Move::S_AttachObjRequest(player->GetIdentify(), GItemID, "p_mount01", "p_mount02", 2);
				m_attachMountId = -1;
			}
		}
	}
}

// --------------------------------------------------------------------------------------
LRESULT	CGameMain::WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_SYSKEYDOWN:
		switch (wParam)
		{
		case VK_F4:
			if (GetKeyState(VK_MENU) < 0)
				SendMessage(m_hFrameWnd, WM_CLOSE, 0, 0);
			break;
		}
		break;

	case WM_KEYDOWN:
	{
		wchar_t temp[512];
		GetKeyNameTextW((LONG)lParam, temp, 512);

		int a = 0;
	}
	break;

	case WM_KEYUP:
	{
	}
	break;

	case WM_SIZE:
		if (g_ruUIRenderer)
		{
			switch (wParam)
			{
			case SIZE_RESTORED:
			{
				RECT rect = m_restoredWindowRect;

				DWORD height = rect.bottom - rect.top;
				DWORD width = rect.right - rect.left;

				m_resetFontIndexStep = 2;
				
				SetScreenSize(width, height, m_isWindowed, FALSE);

				// µo¥X¨Æ¥ó, Åý»Ý­nªº UI §ïÅÜ³]©w
				if (m_interface && m_interface->GetUiWorld())
					m_interface->SendWorldEvent(UiEvent_Sys_WindowsResize);
			}
			break;

			// ³Ì¤j¤Æ
			case SIZE_MAXIMIZED:
			{
				RECT rect;
				GetClientRect(hWnd, &rect);

				DWORD height = rect.bottom - rect.top;
				DWORD width = rect.right - rect.left;

				m_resetFontIndexStep = 2;

				SetScreenSize(width, height, m_isWindowed, FALSE);

				// µo¥X¨Æ¥ó, Åý»Ý­nªº UI §ïÅÜ³]©w
				if (m_interface && m_interface->GetUiWorld())
					m_interface->SendWorldEvent(UiEvent_Sys_WindowsResize);
			}
			break;
			}
		}
		break;

	case WM_SIZING:
		if (g_ruUIRenderer)
		{
			RECT* lpRect = (RECT*)lParam;

			float height = (float)(lpRect->bottom - lpRect->top);
			float width = (float)(lpRect->right - lpRect->left);
			if (height / width > 0.8f) {
				width = height / 0.8f;
			}
			else if (height / width < 0.5625f) {
				width = height / 0.5625f;
			}

			lpRect->right = lpRect->left + static_cast<LONG>(width);
			lpRect->bottom = lpRect->top + static_cast<LONG>(height);

			m_restoredWindowRect = *lpRect;		
		}
		break;

	case WM_CLOSE:
	{
		WriteINI("RuneDev.ini");
		if (m_isExit == false && g_pLoadingFrame && g_pLoadingFrame->GetLoadingState() == false)
		{
			if (m_interface)
			{
				switch (m_interface->GetProcessType())
				{
				case CInterface::eInterface_Process_World:
					m_interface->SendWorldEvent("CLOSE_WINDOW");
					return true;
					break;

				case CInterface::eInterface_Process_Login:
					// §R°£«Ø¨¤¸ê°T
					if (g_pCharacterCreate->GetRoleSprite() != NULL)
					{
						g_pCharacterCreate->DeleteRoleSprite();
					}
					break;
				}
			}
		}
	} break;

	}

	return CRuneDev::WndProc(hWnd, iMsg, wParam, lParam);
}
// --------------------------------------------------------------------------------------
void CGameMain::WriteINI(const char* fileName)
{
	//char tmp[MAX_PATH];
	char fullName[MAX_PATH];

	GetFullPathName(fileName, MAX_PATH, fullName, NULL);

	CIni ini(fullName);

	// Write ErrorList Pos to ini file

	RECT rect;
	if (GetWindowRect(CErrorList::GetHandle(), &rect))
	{
		int iX = rect.left;
		int iY = rect.top;
		int iWidth = rect.right - rect.left;
		int iHeight = rect.bottom - rect.top;

		ini.WriteInt("ErrorList", "PosX", iX);
		ini.WriteInt("ErrorList", "PosY", iY);
		ini.WriteInt("ErrorList", "Width", iWidth);
		ini.WriteInt("ErrorList", "Height", iHeight);
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::LoadLuaFile(const char* dir)
{
	WIN32_FIND_DATA fd;
	HANDLE h;

	if (dir == NULL)
		return;

	string fullName(dir);
	if (fullName[fullName.size() - 1] == '\\')
		fullName += "*.*";
	else
		fullName += "\\*.*";

	h = FindFirstFile(fullName.c_str(), &fd);
	if (h != INVALID_HANDLE_VALUE)
	{
		do
		{
			string fileName(dir);
			if (fileName[fileName.size() - 1] != '\\')
				fileName += "\\";
			fileName += fd.cFileName;
			//¥Ø¿ý
			if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
			}
			//ÀÉ®×
			else
			{
				m_interface->LoadLuaFile(fileName.c_str());
			}
		} while (FindNextFile(h, &fd));
		FindClose(h);
	}
}
// --------------------------------------------------------------------------------------
void CGameMain::SpriteMsgProc(IBaseSprite* sprite, SpriteMessageType msg, INT64 lParam, INT64 rParam)
{
	// ¨ä¥LÃþ«¬¤£³B²z
	if (sprite->GetClassType() != eSpriteClass_Role)
		return;

	CRuneDev::SpriteMsgProc(sprite, msg, lParam, rParam);

	// ¬É­±¤£¦s¦b
	if (m_interface == NULL)
		return;

	switch (msg)
	{
	case eSpriteMsg_NameColor_Changed:
		g_pObjectBloodBar->ChangeState(sprite->GetIdentify());
		break;

	case eSpriteMsg_Unlink:
		if (m_pPlayer == sprite)
		{
			// ª«¥ó³sµ²®ø¥¢
			if (m_pPlayer->GetAttachParentId() != 0)
			{
				// ¸Ñ°£ª«¥ó³sµ²
				Net_Move::S_DetachObjRequest(m_pPlayer->GetIdentify(), m_pPlayer->GetAttachParentId());
			}
		}
		break;

		// ¹ï¶H¯S§Oª¬ºA»Ý­n¨ú®ø¬Iªk
	case eSpriteMsg_CancelSpell:
	{
		// ¬Iªk¹ï¶H
		if (m_sendSpellTarget == sprite)
		{
			GameObjDbStruct* itemDB = CNetGlobal::GetObj(m_sendSpellMagicID);
			if (itemDB)
			{
				// ¥i¥H§ðÀ»¹ï¶H
				if (itemDB->MagicCol.TargetType == EM_Releation_Enemy)
				{
					// °e¥X¨ú®ø¬Iªk
					NetCli_Magic::CancelSpellMagic();
				}
			}
		}
	}
	break;

	case eSpriteMsg_CantCtrl:
	{
		CRoleSprite* player = GetPlayer();
		CRoleSprite* speller = FindSpriteByID(lParam);
		if (player && sprite == player)
		{
			CRoleSprite* target = player->GetTargetSprite();
			if (speller == target)
				SetPlayerTarget(NULL);
		}

	}

	break;

	case eSpriteMsg_Position_Changed:
		break;

	case eSpriteMsg_Dot_Value:
	{
		DOTBuffValueInfo* bvi = (DOTBuffValueInfo*)lParam;
		if (bvi)
		{
			SCTInfo sct;
			sct.source = bvi->source;
			sct.target = sprite->GetIdentify();

			if (bvi->absorb)
			{
				sct.type = eSCT_Normal_Absorb;
			}
			else
			{
				sct.type = eSCT_Magic_Damage;

				switch (bvi->type)
				{
				case 0:
					sct.hp = bvi->value;
					break;
				case 1:
					sct.mp = bvi->value;
					break;
				case 2:
				case 3:
				case 4:
					sct.sp = bvi->value;
					break;
				}
			}
			g_pScrollCombatTextFrame->PushSCTInfo(sct);

			const char* magicName = "";
			GameObjDbStructEx* itemDB = CNetGlobal::GetObj(bvi->id);
			if (itemDB)
				magicName = itemDB->GetName();

			// ¾Ô°«°T®§
			CRoleSprite* player = GetPlayer();
			CRoleSprite* source = FindSpriteByID(sct.source);
			CRoleSprite* target = (CRoleSprite*)sprite;

			// ª±®a¦s¦b¨Ã¥B§ðÀ»©Î³Q§ðÀ»¤è¬°ª±®a
			if (player == sprite || player == source)
			{
				char temp[1024];
				const char* name_s = (source) ? source->GetName() : bvi->sourceName;
				const char* name_t = (sprite) ? ((CRoleSprite*)sprite)->GetName() : "";
				const char* combatLog = "";
				int value = 0;
				MagicAttackTypeENUM AtkType;

				if (bvi->absorb)
				{
					value = 0;
					sprintf_s(temp, 1024, "|cFFF4F021%s|r", g_ObjectData->GetString("SYS_COMBAT_MAGIC_ABSORB"));
					combatLog = temp;
					//combatLog = g_ObjectData->GetString("SYS_COMBAT_MAGIC_ABSORB");
				}
				else
				{
					if (bvi->value == 0)
					{
						if (itemDB->MagicBase.DotMagic.Base > 0)
						{
							if (sprite == source) {
								sprintf_s(temp, 1024, "|cFF05FF0F%s|r", g_ObjectData->GetString("SYS_COMBAT_MAGIC_HEAL_SELF"));
								combatLog = temp;
								//combatLog = g_ObjectData->GetString("SYS_COMBAT_MAGIC_HEAL_SELF");
							}
							else
							{
								sprintf_s(temp, 1024, "|cFF05FF0F%s|r", g_ObjectData->GetString("SYS_COMBAT_MAGIC_HEAL"));
								combatLog = temp;
								//combatLog = g_ObjectData->GetString("SYS_COMBAT_MAGIC_HEAL");
							}
						}
						else
						{
							sprintf_s(temp, 1024, "|cFFF4F021%s|r", g_ObjectData->GetString("SYS_COMBAT_MAGIC_NORMAL"));
							combatLog = temp;
							//combatLog = g_ObjectData->GetString("SYS_COMBAT_MAGIC_NORMAL");
						}
					}
					else if (bvi->value > 0)
					{
						value = bvi->value;
						if (sprite == source)
						{
							sprintf_s(temp, 1024, "|cFF05FF0F%s|r", g_ObjectData->GetString("SYS_COMBAT_MAGIC_HEAL_SELF"));
							combatLog = temp;
							//combatLog = g_ObjectData->GetString("SYS_COMBAT_MAGIC_HEAL_SELF");
						}
						else
						{
							sprintf_s(temp, 1024, "|cFF05FF0F%s|r", g_ObjectData->GetString("SYS_COMBAT_MAGIC_HEAL"));
							combatLog = temp;
							//combatLog = g_ObjectData->GetString("SYS_COMBAT_MAGIC_HEAL");
						}
					}
					else
					{
						value = -bvi->value;
						sprintf_s(temp, 1024, "|cFFF4F021%s|r", g_ObjectData->GetString("SYS_COMBAT_MAGIC_NORMAL"));
						combatLog = temp;
						//combatLog = g_ObjectData->GetString("SYS_COMBAT_MAGIC_NORMAL");
					}

					switch (bvi->type)
					{
					case 0:
						AtkType = EM_MagicAttackType_HP;
						break;
					case 1:
						AtkType = EM_MagicAttackType_MP;
						break;
					case 2:
						AtkType = EM_MagicAttackType_SP_Warrior;
						break;
					case 3:
						AtkType = EM_MagicAttackType_SP_Ranger;
						break;
					case 4:
						AtkType = EM_MagicAttackType_SP_Rogue;
						break;
					}
				}
				NetCli_MagicChild::CombatLog(combatLog, name_s, name_t, magicName, AtkType, value, 0);
			}

			// ¾Ô°«°T®§
			if (source && target && player)
			{
				const char* szSourceName = source->GetName();
				const char* szTargetName = target->GetName();

				if (bvi->type == EM_MagicAttackType_HP &&
					(source == player || NetCli_PartyChild::InParty(szSourceName) || target == player || NetCli_PartyChild::InParty(szTargetName)))
				{
					int value = bvi->value;

					if (bvi->absorb)
					{
						if (value < 0)
						{
							g_pCombatMeter->AddDamage(szSourceName, szTargetName, magicName, "ABSORB", -value);
						}
					}
					else
					{
						if (value > 0)
						{
							g_pCombatMeter->AddHeal(szSourceName, szTargetName, magicName, "NORMAL", value);
						}
						else
						{
							g_pCombatMeter->AddDamage(szSourceName, szTargetName, magicName, "NORMAL", -value);
						}
					}
				}
			}
		}
	}
	break;

	case eSpriteMsg_Combat_Logo:
		break;

	case eSpriteMsg_Delete:
	case eSpriteMsg_MaxHP_Changed:
	case eSpriteMsg_HP_Changed:
	case eSpriteMsg_MP_Changed:
	case eSpriteMsg_SP_Changed:
	case eSpriteMsg_MaxMP_Changed:
	case eSpriteMsg_MaxSP_Changed:
	case eSpriteMsg_Level_Changed:
	case eSpriteMsg_Buff_Changed:
	case eSpriteMsg_Portrait_Changed:
	case eSpriteMsg_Vocation_Changed:
	case eSpriteMsg_Target_Changed:
	case eSpriteMsg_Pet_Changed:
	case eSpriteMsg_Relation_Changed:
	case eSpriteMsg_CastingTime_Changed:
	{
		char tmp[64];
		CRoleSprite* player = m_pPlayer;
		CRoleSprite* target = (player) ? player->GetTargetSprite() : NULL;
		CRoleSprite* targettarget = (target) ? target->GetTargetSprite() : NULL;
		CRoleSprite* pet = (player) ? player->GetPetSprite() : NULL;

		// ¦å±ø§ó·s
		if (g_pObjectBloodBar)
		{
			switch (msg)
			{
				// ­«·s³]©w¦å¶q
			case eSpriteMsg_MaxHP_Changed:
			case eSpriteMsg_HP_Changed:
				g_pObjectBloodBar->ChangeHp(sprite->GetIdentify());
				break;

				// ¥Ø«e§ïÅÜ
			case eSpriteMsg_Target_Changed:
				g_pObjectBloodBar->TargetChange(sprite->GetIdentify(), rParam);
				break;
			}
		}

		// ª±®a
		if (sprite == player) {
			UnitMsgProc("player", msg, (CRoleSprite*)sprite, lParam, rParam);
		}
		// ª±®aªº¥Ø¼Ð
		if (sprite == target) {
			UnitMsgProc("target", msg, (CRoleSprite*)sprite, lParam, rParam);
		}
		// ª±®a¥Ø¼Ðªº¥Ø¼Ð
		if (sprite == targettarget) {
			UnitMsgProc("targettarget", msg, (CRoleSprite*)sprite, lParam, rParam);
		}
		// ª±®aÃdª«
		if (sprite == pet) {
			UnitMsgProc("pet", msg, (CRoleSprite*)sprite, lParam, rParam);
		}

		// Focus
		for (int i = 0; i < MAX_FOCUS_SPRITES; i++)
		{
			if (m_focusSprites[i])
			{
				if (m_focusSprites[i] == sprite)
				{
					sprintf_s(tmp, 64, "focus%d", i + 1);
					UnitMsgProc(tmp, msg, (CRoleSprite*)sprite, lParam, rParam);
				}

				// Focus target
				if (m_focusSprites[i]->GetTargetSprite())
				{
					if (m_focusSprites[i]->GetTargetSprite() == sprite)
					{
						sprintf_s(tmp, 64, "focus%dtarget", i + 1);
						UnitMsgProc(tmp, msg, (CRoleSprite*)sprite, lParam, rParam);
					}

					// Focus target target
					if (m_focusSprites[i]->GetTargetSprite()->GetTargetSprite())
					{
						if (m_focusSprites[i]->GetTargetSprite()->GetTargetSprite() == sprite)
						{
							sprintf_s(tmp, 64, "focus%dtargettarget", i + 1);
							UnitMsgProc(tmp, msg, (CRoleSprite*)sprite, lParam, rParam);
						}
					}
				}
			}
		}

		// ¾Ô³õ¤¤
		if (IsBattleGroundZone())
		{
			// ¾Ô³õ¤p¶¤¦¨­û
			vector<BattleGroundPartyMember*>& battleParty = NetCli_PartyChild::s_BattleParty;
			vector<BattleGroundPartyMember>& battleRaidMember = NetCli_PartyChild::s_BattleMember;

			// ¶¤¥î
			int index = 1;
			for (vector<BattleGroundPartyMember*>::iterator iter = battleParty.begin(); iter != battleParty.end(); index++, iter++)
			{
				// ¶¤¤Í
				if (((CRoleSprite*)sprite)->GetDBID() == (*iter)->DBID) {
					sprintf_s(tmp, 64, "party%d", index);
					UnitMsgProc(tmp, msg, (CRoleSprite*)sprite, lParam, rParam);
				}

				// ¶¤¤Í¥Ø¼Ð
				CRoleSprite* partyTarget = FindSpriteByDBID((*iter)->DBID);
				partyTarget = (partyTarget) ? partyTarget->GetTargetSprite() : NULL;
				if (sprite == partyTarget) {
					sprintf_s(tmp, 64, "party%dtarget", index);
					UnitMsgProc(tmp, msg, (CRoleSprite*)sprite, lParam, rParam);
				}
			}

			// ¹Î¶¤
			index = 1;
			for (vector<BattleGroundPartyMember>::iterator iter = battleRaidMember.begin(); iter != battleRaidMember.end(); index++, iter++)
			{
				// ¶¤¤Í
				if (((CRoleSprite*)sprite)->GetDBID() == iter->DBID) {
					sprintf_s(tmp, 64, "raid%d", (iter->partyNo + 1));
					UnitMsgProc(tmp, msg, (CRoleSprite*)sprite, lParam, rParam);
				}

				// ¶¤¤Í¥Ø¼Ð
				CRoleSprite* partyTarget = FindSpriteByDBID(iter->DBID);
				partyTarget = (partyTarget) ? partyTarget->GetTargetSprite() : NULL;
				if (sprite == partyTarget) {
					sprintf_s(tmp, 64, "raid%dtarget", (iter->partyNo + 1));
					UnitMsgProc(tmp, msg, (CRoleSprite*)sprite, lParam, rParam);
				}
			}
		}
		else
		{
			// ¶¤¥î	
			int index = 1;
			for (vector<PartyMemberInfo*>::iterator iter = NetCli_PartyChild::s_party.begin(); iter != NetCli_PartyChild::s_party.end(); index++, iter++)
			{
				// ¶¤¤Í
				if (((CRoleSprite*)sprite)->GetDBID() == (*iter)->DBID) {
					sprintf_s(tmp, 64, "party%d", index);
					UnitMsgProc(tmp, msg, (CRoleSprite*)sprite, lParam, rParam);
				}

				// ¶¤¤Í¥Ø¼Ð
				CRoleSprite* partyTarget = FindSpriteByDBID((*iter)->DBID);
				partyTarget = (partyTarget) ? partyTarget->GetTargetSprite() : NULL;
				if (sprite == partyTarget) {
					sprintf_s(tmp, 64, "party%dtarget", index);
					UnitMsgProc(tmp, msg, (CRoleSprite*)sprite, lParam, rParam);
				}
			}

			// ¹Î¶¤
			if (NetCli_PartyChild::GetPartyInfo().Type == EM_PartyType_Raid)
			{
				int index = 1;
				for (vector<PartyMemberInfo>::iterator iter = NetCli_PartyChild::s_member.begin(); iter != NetCli_PartyChild::s_member.end(); index++, iter++)
				{
					// ¶¤¤Í
					if (((CRoleSprite*)sprite)->GetDBID() == iter->DBID) {
						sprintf_s(tmp, 64, "raid%d", (iter->raidNo + 1));
						UnitMsgProc(tmp, msg, (CRoleSprite*)sprite, lParam, rParam);
					}

					// ¶¤¤Í¥Ø¼Ð
					CRoleSprite* partyTarget = FindSpriteByDBID(iter->DBID);
					partyTarget = (partyTarget) ? partyTarget->GetTargetSprite() : NULL;
					if (sprite == partyTarget) {
						sprintf_s(tmp, 64, "raid%dtarget", (iter->raidNo + 1));
						UnitMsgProc(tmp, msg, (CRoleSprite*)sprite, lParam, rParam);
					}
				}
			}
		}
	}
	break;
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::UnitMsgProc(const char* unit, SpriteMessageType msg, CRoleSprite* sprite, int lParam, int rParam)
{
	CUi* ui = m_interface->GetUiWorld();
	lua_State* L = m_interface->GetLuaState();
	if (ui == NULL || L == NULL)
		return;

	// ·í¹êÅé¨¤¦â³Q§R°£®É,³qª¾§ó·sBUFF¸ê°T
	if (msg == eSpriteMsg_Delete)
		msg = eSpriteMsg_Buff_Changed;

	lua_pushstring(L, unit);
	lua_setglobal(L, "arg1");
	switch (msg)
	{
	case eSpriteMsg_Target_Changed:
		ui->SendEvent("UNIT_TARGET_CHANGED");
		break;

	case eSpriteMsg_Pet_Changed:
		ui->SendEvent("UNIT_PET_CHANGED");
		break;

	case eSpriteMsg_HP_Changed:
		lua_pushnumber(L, lParam);
		lua_setglobal(L, "arg2");
		ui->SendEvent("UNIT_HEALTH");
		break;

	case eSpriteMsg_MP_Changed:
		lua_pushnumber(L, lParam);
		lua_setglobal(L, "arg2");
		ui->SendEvent("UNIT_MANA");
		break;

	case eSpriteMsg_SP_Changed:
		lua_pushnumber(L, lParam);
		lua_setglobal(L, "arg2");
		ui->SendEvent("UNIT_SKILL");
		break;

	case eSpriteMsg_MaxHP_Changed:
		lua_pushnumber(L, lParam);
		lua_setglobal(L, "arg2");
		ui->SendEvent("UNIT_MAXHEALTH");
		break;

	case eSpriteMsg_MaxMP_Changed:
		lua_pushnumber(L, lParam);
		lua_setglobal(L, "arg2");
		ui->SendEvent("UNIT_MAXMANA");
		break;

	case eSpriteMsg_MaxSP_Changed:
		lua_pushnumber(L, lParam);
		lua_setglobal(L, "arg2");
		ui->SendEvent("UNIT_MAXSKILL");
		break;

	case eSpriteMsg_Level_Changed:
		lua_pushnumber(L, lParam);
		lua_setglobal(L, "arg2");
		lua_pushnumber(L, rParam);
		lua_setglobal(L, "arg3");
		ui->SendEvent("UNIT_LEVEL");
		break;

	case eSpriteMsg_Buff_Changed:
		ui->SendEvent("UNIT_BUFF_CHANGED");

		// ¦pªGª±®aBUFF¸ê®Æ¦³§ó°Ê
		if (strcmp(unit, "player") == 0)
		{
			g_pActionFrame->UpdateExtraSkill();
		}
		break;

	case eSpriteMsg_Portrait_Changed:
		if (strncmp(unit, "raid", 4) != 0)
			ui->SendEvent("UNIT_PORTRAIT_UPDATE");
		break;

	case eSpriteMsg_Vocation_Changed:
		ui->SendEvent("UNIT_CLASS_CHANGED");
		break;

	case eSpriteMsg_Relation_Changed:
		ui->SendEvent("UNIT_RELATION");
		break;

	case eSpriteMsg_CastingTime_Changed:
		ui->SendEvent("UNIT_CASTINGTIME");
		break;
	}
}

// --------------------------------------------------------------------------------------
#ifdef KALYDO
void CGameMain::LoadINI(const char* fileName, const char* language)
#else
void CGameMain::LoadINI(const char* fileName)
#endif // KALYDO
{
	bool IgonerLocalization = false;
	bool insideServer = false;
	bool bSkipVersionCheck = false;
	char tmp[MAX_PATH];
	char fullName[MAX_PATH];

	GetFullPathName(fileName, MAX_PATH, fullName, NULL);

	CIni ini(fullName);
	ini.GetString("Path", "Resource", tmp, MAX_PATH, "");

	// ³]©w¸ê·½¥Ø¿ý
	SetResourcePath(tmp);

	ini.GetString("Locale", "IgonerLocalization", tmp, MAX_PATH, "");
	if (strcmp(tmp, "1") == 0)
		IgonerLocalization = true;

	ini.GetString("Server", "RunewakerInsideTestServer", tmp, MAX_PATH, "");
	if (strcmp(tmp, "1") == 0)
		insideServer = true;

	ini.GetString("Server", "DisplayMaintainMode", tmp, MAX_PATH, "");
	if (strcmp(tmp, "1") == 0)
		m_displayMaintainServer = true;

	ini.GetString("Locale", "LoginEpisode", tmp, MAX_PATH, "");
	if (strlen(tmp) > 0) {
		int version = atoi(tmp);
		if (version >= 0 && version <= m_updateInf.loginEpisode)
			SetLoginEpisodeVersion(version);
	}

	bSkipVersionCheck = (ini.GetInt("Debug", "SkipVersionCheck", 0) > 0);

	if (bSkipVersionCheck)
	{
		SetVersion("");
		SetVersionURL("");
	}

	if (insideServer)
	{
		// ³]©wIP address
		ini.GetString("Server", "IP", tmp, MAX_PATH, "");
		SetIP(tmp);

		// ³]©wIP port
		ini.GetString("Server", "Port", tmp, MAX_PATH, "");
		SetPort(atoi(tmp));
	}
	else
	{
		string ip, port;
		IniFile2& serverList = GetServerListIni();
		serverList.SetSection("TestServer");
		ip = serverList.Str("IP");
		port = serverList.Str("Port");
		if (ip.empty() && port.empty())
		{
			char section[512];
			ini.GetString("Zone", "Select", tmp, MAX_PATH, "1");
			sprintf_s(section, 512, "Server%s", tmp);
			serverList.SetSection(section);
			ip = serverList.Str("IP");
			port = serverList.Str("Port");
		}
		SetIP(ip.c_str());
		SetPort(atoi(port.c_str()));
	}

	// ª½±µ³s±µGame Server
	ini.GetString("Server", "PassGameServerIP", tmp, MAX_PATH, "");
	m_gameServerIP = tmp;
	ini.GetString("Server", "PassGameServerPort", tmp, MAX_PATH, "");
	m_gameServerPort = atoi(tmp);

	// Crash Report IP
	ini.GetString("Server", "CrashReportIP", tmp, MAX_PATH, "");

	// ¦a°Ïª©¥»
	ini.GetString("Locale", "Language", tmp, MAX_PATH, m_language.c_str());

#ifdef KALYDO 
	// in online version always use the language set by either the website, or default = eneu
	// NOTE we completely ignore the setting in the ini files this way
	if (strcmp(language, ""))
		strcpy(tmp, language);

#else
	// don't know why this is needed in the client version
	strupr(tmp);
#endif // KALYDO

	// ­­¨î¥u¯à¨Ï¥Î¦a°Ï»y¨t
	if (IgonerLocalization == false)
	{
		IniFile2 installIni;
		LoadIniFromZipFile(&installIni, "install.ini", UPDATEINF);
		installIni.SetSection("BOOT");
#ifdef KALYDO
		string select = tmp;
#else
		string select = installIni.StrDef("select", m_DataLanguage.c_str());
#endif // KALYDO

		IniFile2 localization;
		LoadIniFromZipFile(&localization, "localization.ini", UPDATEINF);
		localization.SetSection(select.c_str());

		string languageSelect = localization.StrDef("langselect", select.c_str());
		vector<string> totalLanguage;

		const char* begin = languageSelect.c_str();
		const char* next = strchr(begin, ',');
		const char* end = begin + languageSelect.length();
		char temp[512];
		while (next)
		{
			memcpy(temp, begin, next - begin);
			temp[next - begin] = 0;

			for (int i = 0; temp[i]; i++)
			{
				if (temp[i] == ':')
				{
					temp[i] = 0;
					break;
				}
			}
			totalLanguage.push_back(temp);

			begin = next + 1;
			next = strchr(begin, ',');
		}

		next = end;
		memcpy(temp, begin, next - begin);
		temp[next - begin] = 0;

		for (int i = 0; temp[i]; i++)
		{
			if (temp[i] == ':')
			{
				temp[i] = 0;
				break;
			}
		}
		totalLanguage.push_back(temp);

		vector<string>::iterator iter = totalLanguage.begin();
		for (; iter != totalLanguage.end(); iter++)
		{
			if (stricmp(iter->c_str(), tmp) == 0)
				break;
		}

		if (iter == totalLanguage.end())
		{
			if (totalLanguage.empty())
				strcpy(tmp, m_DataLanguage.c_str());
			else
				strcpy(tmp, totalLanguage[0].c_str());
		}
}

	int pos = 0;
	for (; pos < MAX_LANGUAGES; pos++)
	{
		if (stricmp(g_languageCode[pos], tmp) == 0)
		{
			m_language = tmp;
			break;
		}
	}
	for (int i = 0; i < (int)m_language.length(); i++)
		m_language[i] = ::toupper(m_language[i]);

	m_StringLanguage = m_language;

	// ¬O§_±µ¦¬SERVER¨Óªº°£¿ù°T®§
	m_displaySrvDebugMsg = (bool)ini.GetInt("Server", "DebugMsg", m_displaySrvDebugMsg);

	// Set ErrorList Pos
	if (ini.IsSectionExist("ErrorList") && g_pError)
	{
		int x = ini.GetInt("ErrorList", "PosX", 0);
		int y = ini.GetInt("ErrorList", "PosY", 0);
		int w = ini.GetInt("ErrorList", "Width", 500);
		int h = ini.GetInt("ErrorList", "Height", 500);
		SetWindowPos(CErrorList::GetHandle(), HWND_NOTOPMOST, x, y, w, h, 0);
	}

	int ChatLog = ini.GetInt("ChatLog", "ChatLog", 0);

	// ¶}±ÒLOG°O¿ý Áú°ê¤£°O¿ý
	if (ChatLog == 1 && m_country.compare(LANGUAGE_CODE_KR) != 0)
	{
		m_IsChatLog = true;
	}
	else
		m_IsChatLog = false;

	// ³]©w Glow ¶}Ãö
	//	bool bGlow = ini.GetBool( "Option", "Glow", false );
	//	g_ruEngineSettings->SetSetting("glowFX:enable", false);

#ifdef _AUTOLOGIN
	CNetCli_Login::m_bAutoLogin = (ini.GetInt("Login", "AutoLogin", 0)) ? true : false;

	ini.GetString("Login", "ID", CNetCli_Login::m_szAutoLogingID, 32, "");
	ini.GetString("Login", "PW", CNetCli_Login::m_szAutoLogingPW, 32, "");

	CNetCli_Login::m_iAutoLoginRoleID = ini.GetInt("Login", "RoleID", 0);

	std::string Password = CNetCli_Login::m_szAutoLogingPW;
	bool		MD5Password = m_IsMD5Password;


	if ((m_IsMD5Password == false) && (m_IsAutoHashPassword == true))
	{
		//¦Û°ÊHash±K½X
		switch (m_PasswordHashType)
		{
		case 0: //md5
		{
			MyMD5Class myMd5Class;
			myMd5Class.ComputeStringHash(CNetCli_Login::m_szAutoLogingPW, NULL);
			Password = myMd5Class.GetMd5Str();
			MD5Password = true;
		}
		break;
		case 1: //bcrypt
		{
			char* bcPassword = crypt(CNetCli_Login::m_szAutoLogingPW, BCRYPT_DEFAULT_SALT);

			if (bcPassword != NULL)
			{
				Password = bcPassword;
			}

			MD5Password = false;
		}
		break;
		}
	}

	CNetCli_Login::Login(GetIP(), GetPort(), CNetCli_Login::m_szAutoLogingID, (char*)Password.c_str(), GetMac(), MD5Password);

#endif
}

// --------------------------------------------------------------------------------------
void CGameMain::SetGameTime(int time, int timeZone)
{
	m_gameTime = time;
	m_serverTimeZone = timeZone;
	m_startFrameTime = m_frameTime;
	m_currentGameTime = 0;
}

// --------------------------------------------------------------------------------------
void CGameMain::SetPlayerID(int playerID)
{
	m_playerID = playerID;
	//g_pError->AddMessage(0, 0, "Receive Player'ID %x", m_playerID);
}

// --------------------------------------------------------------------------------------
void CGameMain::SetGItemID(int GItemID)
{
	m_GItemID = GItemID;
	IBaseSprite::SetLocalPlayerId(GItemID);

	if (m_attachMountId != -1)
	{
		Net_Move::S_AttachObjRequest(GItemID, m_attachMountId, "p_mount01", "p_mount02", 2);
		m_attachMountId = -1;
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::SetPlayerPos(CRuVector3& pos)
{
	if (m_pPlayer)
	{
		CRoleSprite* spriteControlProxy = m_pPlayer->GetProxyControlSprite();

		spriteControlProxy->SetPos(pos);
		m_moveVector = spriteControlProxy->GetMoveVector();
		m_direction = spriteControlProxy->GetDirection();
		m_position = spriteControlProxy->GetPos();
		m_sendMoveTime = 0.0f;

		m_checkPlayerHeight = (UINT)m_position.m_y;
		m_playerHeight = m_position.m_y;

		// ²M°£·Æ¹«²¾°Ê
		ClearMoveToPoint();

		// ²M°£¸ô®|²¾°Ê
		ClearPathMove();

		// ²M°£ÂÂ¦³¸ê°T,±N²{¦b®y¼ÐÂI³]©w¬°³Ì«á½T»{®y¼Ð
		m_rightPosition = m_position;
		m_obstructPosition.Clear();
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::SetZoneID(int zoneID)
{
	if (m_zoneID != zoneID)
	{
		CBattleGroundOptionButtonFrame::LeaveBattleGroundGroup(m_zoneID);

		m_zoneID = zoneID;

		if (g_pRuGlobalEventManager)
		{
			g_pRuGlobalEventManager->SetEnvironmentMusicEnable(TRUE);
			g_pRuGlobalEventManager->ClearAllSoundIdex();
		}

		// ¹LZONE¸ê®Æ
		if (m_zoneName.size() > 0 && m_interface->GetLuaState())
		{
			lua_State* L = m_interface->GetLuaState();

			char temp[1024];
			sprintf_s(temp, 1024, "ZONE_%s", m_zoneName.c_str());
			strupr(temp);

			const char* name = g_ObjectData->GetString(temp);
			if (strlen(name) <= 0)
				name = temp;

			lua_pushstring(L, name);
			lua_setglobal(L, "arg1");
			lua_pushnumber(L, GetZoneID());				//  ·s¤â±Ð¾Ç­n¥Î
			lua_setglobal(L, "arg2");
			m_interface->SendWorldEvent("ZONE_CHANGED");
		}

		// ²M°£¦º¤`°Ï°ì
		m_interface->SendWorldEvent("WARNING_STOP");

		// ²M°£¸ü¨ã¼öÁä
		g_pActionFrame->ClearImplementAction();

		// ²M°£¯S©w¨Æ¥óºX¼Ð
		ClearRoomValue();

		// ²M°£°Æ¥»¶i«×°T°Ý
		ClearInstanceWaitTime();

		// ²M°£¼È®É®y¼Ð
		ClearPartitionPosition();

		// ²M°£©Ò¦³¥þ°ìª«¥ó
		for (map<int, CRoleSprite*>::iterator iter = m_spriteContainer.begin(); iter != m_spriteContainer.end(); iter++)
		{
			if ((iter->second)->GetStaticObjectState()) {
				DeleteSprite((iter->second)->GetWorldGUID());
			}
		}

		// ²M°£PK¦æ¬°
		{
			DuelInfo info = g_pGameControl->GetDuelInfo();
			switch (info.step)
			{
			case eDuelStep_None:
			case eDuelStep_Cancel:
				break;

			default:
				g_pGameControl->SetDuelStep(eDuelStep_Finish, 0);
			}
		}
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::SetRoomID(int roomID)
{
	if (m_roomID != roomID)
	{
		m_roomID = roomID;

		// ²M°£©Ò¦³¥þ°ìª«¥ó
		for (map<int, CRoleSprite*>::iterator iter = m_spriteContainer.begin(); iter != m_spriteContainer.end(); iter++)
		{
			CRoleSprite* sprite = iter->second;
			if (sprite->GetStaticObjectState() && sprite->RefCount() > 0)
				DeleteSprite(sprite->GetWorldGUID());
		}

		// ²M°£PK¦æ¬°
		{
			DuelInfo info = g_pGameControl->GetDuelInfo();
			switch (info.step)
			{
			case eDuelStep_None:
			case eDuelStep_Cancel:
				break;

			default:
				g_pGameControl->SetDuelStep(eDuelStep_Finish, 0);
			}
		}
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::SetMapName(const char* name)
{
	if (name != NULL && m_mapName.compare(name) != 0)
	{
		m_mapName = name;
#ifdef KALYDO
		// Download a part of the WDB and let the game load the terrain
		// if enough is there.
		std::string packageName(name);
		int backslash = packageName.find_last_of('\\');
		int forwardslash = packageName.find_last_of('/');
		if (backslash == std::string::npos)
			backslash = -1;
		if (forwardslash == std::string::npos)
			forwardslash = -1;

		packageName.erase(0, max(backslash, forwardslash) + 1);
		int dot = packageName.find_last_of('.');
		packageName.erase(dot);
		packageName.insert(0, "wdb_");
		SAFE_DELETE(m_HandleWDBPackage);
		m_HandleWDBPackage = new THandler(this, &CGameMain::handleWDB, new std::string(packageName));

		m_WDBDownloadProgress = 0.0f;
		m_isDownloading = true;
		m_shouldSetCameraPos = false;
		application->requestPackage(packageName.c_str(), m_HandleWDBPackage);
#endif // KALYDO
		m_needLoadTerrain = true;
		m_isWaitGlobalObject = true;
		m_globalObject.clear();
		m_spriteAnchorHeight.clear();
		if (g_pObjectBloodBar)
			g_pObjectBloodBar->Clear();
	}
}

// --------------------------------------------------------------------------------------
bool CGameMain::IsGlobalLoadComplete()
{
	if (m_isWaitGlobalObject)
		return false;

	CRuAABB bounds;
	if (m_pRuTerrain)
	{
		bounds = m_pRuTerrain->GetEnumerationBounds();
	}

	vector<int>::iterator iter = m_globalObject.begin();
	while (iter != m_globalObject.end())
	{
		CRoleSprite* sprite = FindSpriteByID(*iter);
		if (sprite && bounds.Contains(sprite->GetPos()) == TRUE)
		{
			if (sprite->IsProxyLoadComplete() == false)
				return false;
			iter++;
		}
		else
		{
			iter = m_globalObject.erase(iter);
		}
	}
	return true;
	}

// --------------------------------------------------------------------------------------
bool CGameMain::InsertGlobalObject(int GItemID)
{
	if (m_isWaitGlobalObject == false)
		return false;

	m_globalObject.push_back(GItemID);
	return true;
}

// --------------------------------------------------------------------------------------
void CGameMain::SetMouseInPlayer(CRoleSprite* sprite)
{
	lua_State* L = m_interface->GetLuaState();
	const char* name = NULL;
	int id = 0;

	if (m_pMouseInPlayer != sprite)
	{
		/*
		if ( m_pMouseInPlayer )
			m_pMouseInPlayer->DisableCelHighlight();
		*/

		m_pMouseInPlayer = sprite;
		if (m_pMouseInPlayer)
		{
			// m_pMouseInPlayer->EnableCelHighlight( RUCOLOR_ARGB( 0xFF, 0x8F, 0x8F, 0x20 ) );
			name = m_pMouseInPlayer->GetName();
			id = m_pMouseInPlayer->GetIdentify();
		}

		lua_pushstring(L, name);
		lua_setglobal(L, "arg1");
		lua_pushnumber(L, id);
		lua_setglobal(L, "arg2");
		m_interface->SendWorldEvent("UPDATE_MOUSEOVER_UNIT");
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::SetPlayer(CRoleSprite* sprite, CRuVector3& pos, CRuVector3& dir)
{
	m_isWaitGlobalObject = false;								// ª±®a¥[¤J¤§«eªº©Ò¦³¥þ°ìª«¥ó,»Ý­nµ¥«Ý¼Ò«¬¸ü¤J§¹²¦	

	m_position = pos;
	m_direction = dir;
	m_sendMoveTime = 0.0f;
	m_checkPlayerHeight = (UINT)m_position.m_y;
	m_playerHeight = m_position.m_y;

	if (m_pPlayer != sprite)
	{
		if (m_pPlayer) {
			g_pObjectBloodBar->DelObj(m_pPlayer->GetIdentify());
		}

		if (sprite) {
			g_pObjectBloodBar->AddObj(sprite->GetIdentify());
		}

		m_pPlayer = sprite;

		if (m_pPlayer)
		{
			m_pPlayer->SetDirection(dir, true, true);

			// ½T»{¬°³Ì«á¶Ç°e®y¼ÐÂI
			SetPlayerPos(pos);

			//m_pPlayer->SetGravityMode(false);

			// ²¾°Êª±®a®y¼Ð¥H¤Î¨¤¦â­±¦V
			MovePlayerPos(pos);

			// ³qª¾¬É­±¨Æ¥ó
			UnitMsgProc("player", eSpriteMsg_Portrait_Changed, m_pPlayer, 0, 0);

			// ª±®a¨¤¦â«Ø¥ß
			m_interface->SendWorldEvent(UiEvent_CreatePlayerSprite);
		}
	}
	else
	{
		if (m_pPlayer)
		{
			CRuVector3 moveVector = pos - m_pPlayer->GetPos();
			if (moveVector.Magnitude() > 0.01f)
				g_pTeleportBook->BreakCasting();
		}

	}
}

// --------------------------------------------------------------------------------------
void CGameMain::SetPlayerTarget(CRoleSprite* sprite)
{
	CRoleSprite* player = m_pPlayer;

	if (player == NULL)
		return;

	CRoleSprite* target = player->GetTargetSprite();
	if (target != sprite && (sprite == NULL || sprite->GetTargetState())) {
		// ³]©wÀY¤W¦WºÙ
		if (target) {
			bool nameVisible = true;
			switch (target->GetObjectType())
			{
			case eSpriteObject_Player:
				if (target == player)
					nameVisible = m_clientSetting.isSelfTitleVisible;
				else
					nameVisible = m_clientSetting.isPlayerTitleVisible;
				break;

			case eSpriteObject_NPC:
			case eSpriteObject_QuestNPC:
				nameVisible = m_clientSetting.isNPCTitleVisible;
				break;
			}

			// ­Y¬OÅã¥ÜÀY¤W¦WºÙ,¨Ã¥B¦³¦å±ø¤£Åã¥Ü¦WºÙ
			if (nameVisible) {
				nameVisible = !g_pObjectBloodBar->IsTheSpriteHaveBloodBar(target->GetIdentify());
			}

			target->SetNameVisibleForce(nameVisible);

			// ²M°£¦aªO¿ï¨úª«¥ó
			target->ReleaseFloorEntity();
		}

		// ³]©w¥Ø¼Ð
		player->SetTargetSprite(sprite);

		// §ó´«¥Ø¼Ð,²M°£¾Ô°«ª¬ºA
		CancelAttack();

		int targetGItemID = 0;
		if (sprite) {
			bool nameVisible = true;

			// ¥Ø¼Ð¬°ª±®a¦Û¤v
			if (sprite == player) {
				nameVisible = GetSelfTitleVisible();
			}

			// ­Y¬OÅã¥ÜÀY¤W¦WºÙ,¨Ã¥B¦³¦å±ø¤£Åã¥Ü¦WºÙ
			if (nameVisible) {
				nameVisible = !g_pObjectBloodBar->IsTheSpriteHaveBloodBar(sprite->GetIdentify());
			}

			// Åã¥Ü¦a®æ¿ï¨úª«¥ó
			sprite->CreateFloorEntity(PLAYER_TARGET_FLOOR_ENTITY);

			// ¦pªG¬°¥Ø«e¥Ø¼Ð,Åã¥ÜÀY¦V¦WºÙ
			sprite->SetNameVisibleForce(nameVisible);

			targetGItemID = sprite->GetIdentify();
			g_pTutorailFrame->doClickNpc(sprite->GetObjectId());
		}

		// ²M°£¦Û°Ê¬Iªk
		if (m_cycleMagicId != 0)
		{
			ClearCycleSpell();
		}

		// ¶Ç°eµ¹SERVER¥Ø«e¥Ø¼Ð
		NetCli_Other::SL_LookTarget(targetGItemID);

		// °e¥Xª±®a¥Ø¼Ð§ïÅÜ
		NetCli_Other::S_SendPlayerTarget(player->GetIdentify(), targetGItemID);


	}
}

// --------------------------------------------------------------------------------------
CRoleSprite* CGameMain::GetPlayerTarget()
{
	if (m_pPlayer)
		return m_pPlayer->GetTargetSprite();
	return NULL;
}

// --------------------------------------------------------------------------------------
CRoleSprite* CGameMain::GetPlayerPet()
{
	if (m_pPlayer)
	{
		return m_pPlayer->GetPetSprite();
	}
	return NULL;
}

// --------------------------------------------------------------------------------------
void CGameMain::SetFocusSprite(int index, CRoleSprite* sprite)
{
	if (index < 0 || index >= MAX_FOCUS_SPRITES)
		return;
	if (m_focusSprites[index] == sprite)
		return;
	if (sprite && sprite->GetTargetState() == false)
		return;

	m_focusSprites[index] = sprite;
	if (m_interface)
	{
		char temp[512];
		lua_State* L = m_interface->GetLuaState();
		if (L)
		{
			sprintf_s(temp, 512, "focus%d", index + 1);
			lua_pushstring(L, temp);
			lua_setglobal(L, "arg1");
			lua_pushnumber(L, index + 1);
			lua_setglobal(L, "arg2");
		}
		m_interface->SendWorldEvent(UiEvent_Focus_Changed);
	}
}

// --------------------------------------------------------------------------------------
CRoleSprite* CGameMain::GetFocusSprite(int index)
{
	if (index < 0 || index >= MAX_FOCUS_SPRITES)
		return NULL;
	return m_focusSprites[index];
}

// --------------------------------------------------------------------------------------
void CGameMain::ClearFocusSprite(CRoleSprite* sprite)
{
	if (sprite == NULL)
		return;

	for (int i = 0; i < MAX_FOCUS_SPRITES; i++)
	{
		if (m_focusSprites[i] == sprite)
		{
			m_focusSprites[i]->SetTargetSprite(NULL);
			SetFocusSprite(i, NULL);
		}
	}
}

// --------------------------------------------------------------------------------------
CRoleSprite* CGameMain::GetTargetNearest(bool enemy, bool reverse)
{
	if (m_pPlayer == NULL || m_pCamera == NULL)
		return NULL;

	CRuVector3 cameraForward = (m_pCamera->GetTargetPosition() - m_pCamera->GetCameraPosition()).Normalize();
	CRuVector3 cameraPostion = m_pCamera->GetCameraPosition();
	CRuVector3 playerPostion = m_pPlayer->GetPos();
	CRuVector3 playerForward = m_pPlayer->GetDirection();
	multimap<float, CRoleSprite*> depathMap;
	int count = 0;
	for (map<int, CRoleSprite*>::iterator iter = m_spriteContainer.begin(); iter != m_spriteContainer.end(); iter++)
	{
		CRoleSprite* sprite = iter->second;
		CRuVector3 spritePoint = sprite->GetPos();

		if (!sprite->GetCursorState() || !sprite->GetTargetState() || sprite->GetDeadState())
			continue;

		// ¼Ä¹ï¹ï¶H
		if (enemy)
		{
			// ¤£¯à§ðÀ»
			if (!sprite->GetCanAttackState())
				continue;
			// ¼ç§Î¬Ý¤£¨ì
			if (!sprite->CheckRenderState())
				continue;
		}
		// ¤Íµ½¹ï¶H
		else
		{
			if (sprite->GetCanAttackState() || sprite->GetEnemyState())
				continue;
		}

		// ¨Ï¥ÎÃèÀY§¤¼Ð¿ï¨ú¥Ø¼Ð
		if (g_pGameMain->GetCameraSelectTarget())
		{
			CRuVector3 viewportPoint = spritePoint;
			float distance = (playerPostion - spritePoint).Magnitude();

			m_pCamera->GetCameraTransform().TransformPoint(spritePoint);

			// Âà´«¬°2D®y¼Ð
			g_pGameMain->WorldToViewport(viewportPoint);

			if (spritePoint.m_z > 0 && distance < 200.0f &&
				viewportPoint.m_x > 0.0f && viewportPoint.m_x < 1024.0f &&
				viewportPoint.m_y > 0.0f && viewportPoint.m_y < 768.0f)
			{
				depathMap.insert(make_pair(distance, sprite));
			}
		}
		// ©ÎªÌ¨Ï¥Îª±®a§¤¼Ð¥H¤Î­±¦V
		else
		{
			CRuVector3 disVector = spritePoint - playerPostion;
			CRuVector3 tempVector = disVector;
			CRuVector3 forward(cameraForward.m_x, 0.0f, cameraForward.m_z);
			forward.Normalize();
			tempVector.Normalize();

			if (forward.Dot(tempVector) > 0.707f)	// §¨¨¤45«×¤ºª«¥ó¦³®Ä
			{
				float distance = disVector.Magnitude();
				if (distance < 200.0f && (disVector.m_y) < max(distance * 0.5f, 20.0f))
					depathMap.insert(make_pair(distance, sprite));
			}
		}

		/*
		CRuVector3 disVector = spritePoint - playerPostion;
		CRuVector3 tempVector = disVector;
		if ( cameraForward.Dot(tempVector.Normalize()) > 0.64f )	// §¨¨¤50«×¤ºª«¥ó¦³®Ä
		{
			float distance = disVector.Magnitude();
			if ( distance < 200.0f && (disVector.m_y) < max(distance * 0.5f, 20.0f) )
				depathMap.insert(make_pair(distance, sprite));
		}
		*/
	}

	CRoleSprite* target = GetPlayerTarget();
	if (depathMap.empty())
		return target;

	if (depathMap.size() == 1 || target == NULL)
		return depathMap.begin()->second;

	for (multimap<float, CRoleSprite*>::iterator iter = depathMap.begin(); iter != depathMap.end(); iter++)
	{
		// §ä¨ì¥Ø«e¥Ø¼Ð
		if (iter->second == target)
		{
			if (reverse)
			{
				multimap<float, CRoleSprite*>::iterator previous;
				if (iter == depathMap.begin())
					previous = depathMap.end();
				else
					previous = iter;
				previous--;
				return previous->second;
			}
			else
			{
				multimap<float, CRoleSprite*>::iterator next = ++iter;
				if (next != depathMap.end())
					return next->second;
				return depathMap.begin()->second;
			}
		}
	}

	return depathMap.begin()->second;
}

// --------------------------------------------------------------------------------------
void CGameMain::EnterWorld(int DBID, const char* playerName)
{
	m_DBID = DBID;
	m_pingState = false;
	m_isEnterWorld = true;
	m_playerName = playerName;

	// ²M°£ Login ÃèÀY¼È®É»·¶Z
	m_cameraTempFarPlane = 0.0f;

	m_isFirstLoading = true;

	if (g_pLoadingFrame)
		g_pLoadingFrame->LoadingStart();

	// ¬É­±¸ü¤J¤¤
	CNetGlobal::ClearTempEvent();
	CNetGlobal::SetUIFinish(false);

	//CNetGlobal::NetRecvEvent(false);
	m_interface->SetPlayerName(playerName);
	m_interface->OpenWorldXml();
	m_interface->SetProcessType(CInterface::eInterface_Process_World);
}

// --------------------------------------------------------------------------------------
void CGameMain::LeaveWorld()
{
	//CApexClient::Release();
	CNetGlobal::RoleDataLoaded();

	m_DBID = -1;
	m_playerID = -1;
	m_zoneID = -1;
	m_isLoadClientData = false;
	m_isEnterWorld = false;
	m_isCombatState = false;
	m_pPlayer = NULL;
	m_pMouseInPlayer = NULL;
	m_campState = false;
	m_parallelID = -1;
	m_sendPassword = false;
	m_tombLeftTime = 0.0f;
	m_isFirstLoading = false;
	m_inMount = false;
	m_inImplement = false;
	m_vipDisplay = false;
	m_vipValid = false;
	m_delayGetImplementTime = 0.0f;
	m_sendKeyTime.clear();
	m_secondPassword.clear();
	m_mapName.clear();


	// ²M°£Âê©w¦V«e¶]
	g_pGameMain->ToggleAutoRune(2);

	// 
	ClearMoveToPoint();
	ClearPathMove();

	// ³]©w´å¼Ð¹Ï§Î
	SetCursor(eCursor_Point);

	// ­«³]®É¶¡
	m_timeGroup = 0;

	// ¸Ñ°£´å¼ÐÂê©w
	UnlockCursorPos();
	SetCursorVisible(true);

	// ¸ü¤J¦aªí
	LoadLoginScreen();

	// 
	CRuVector3 loginPosition;
	loginPosition.m_x = 1560.00;
	loginPosition.m_y = 50.00;
	loginPosition.m_z = 3022.00;
	SetPlayerAnchor(loginPosition);

	// Â÷¶}¹CÀ¸¥@¬É
	m_interface->LeaveFramesWorld();

	// 
	m_interface->SetProcessType(CInterface::eInterface_Process_Login);
}

// --------------------------------------------------------------------------------------
CRoleSprite* CGameMain::GetUnitSprite(const char* str, CRoleSprite* src)
{
	if (str)
	{
		int len = (int)strlen(str);
		if (strncmp(str, "player", 6) == 0)
		{
			if (len == 6)
				return m_pPlayer;
			else
				return GetUnitSprite(str + 6, m_pPlayer);
		}
		if (strncmp(str, "target", 6) == 0)
		{
			CRoleSprite* target = NULL;
			if (src)
				target = src->GetTargetSprite();
			else
				target = GetPlayerTarget();

			if (target)
			{
				if (len > 6)
					return GetUnitSprite(str + 6, target);
				else
					return target;
			}
		}
		if (strncmp(str, "pet", 3) == 0)
		{
			CRoleSprite* pet = NULL;
			if (src)
				pet = src->GetPetSprite();
			else
				pet = GetPlayerPet();

			if (pet)
			{
				if (len > 3)
					return GetUnitSprite(str + 3, pet);
				else
					return pet;
			}
		}
		if (strncmp(str, "party", 5) == 0 && (str[5] > '0' && str[5] < '6'))
		{
			CRoleSprite* sprite = NULL;

			// ¾Ô³õ©Î±j¨î¾Ô³õ²Õ¶¤
			if (IsBattleGroundZone())
			{
				BattleGroundPartyMember* member = NetCli_PartyChild::GetBattlePartyMember(str[5] - '1');
				if (member && (sprite = FindSpriteByDBID(member->DBID)) == NULL)
				{
					sprite = FindBettleMemberByDBID(member->DBID);
				}
			}
			// ¤@¯ë²Õ¶¤
			else
			{
				PartyMemberInfo* member = NetCli_PartyChild::GetPartyMember(str[5] - '1');
				if (member && (sprite = FindSpriteByDBID(member->DBID)) == NULL)
				{
					sprite = (CRoleSprite*)(member->sprite);
				}
			}

			if (sprite)
			{
				if (len > 6)
					return GetUnitSprite(str + 6, sprite);
				else
					return sprite;
			}
		}
		if (strncmp(str, "raid", 4) == 0)
		{
			int number = atoi(str + 4);
			if (number > 0 && number <= 36)
			{
				CRoleSprite* sprite = NULL;

				// ¾Ô³õ©Î±j¨î¾Ô³õ²Õ¶¤
				if (IsBattleGroundZone())
				{
					BattleGroundPartyMember* member = NetCli_PartyChild::GetBattleRaidMember(number - 1);
					if (member && (sprite = FindSpriteByDBID(member->DBID)) == NULL)
					{
						sprite = FindBettleMemberByDBID(member->DBID);
					}
				}
				else
				{
					PartyMemberInfo* member = NetCli_PartyChild::GetRaidMember(number - 1);
					if (member && (sprite = g_pGameMain->FindSpriteByDBID(member->DBID)) == NULL)
					{
						sprite = (CRoleSprite*)(member->sprite);
					}
				}

				if (sprite)
				{
					int count = number / 10;
					if (len > 5 + count)
						return GetUnitSprite(str + 5 + count, sprite);
					else
						return sprite;
				}
			}
		}
		if (strncmp(str, "focus", 5) == 0)
		{
			int number = atoi(str + 5);
			if (number > 0 && number <= MAX_FOCUS_SPRITES)
			{
				CRoleSprite* sprite = m_focusSprites[number - 1];
				if (sprite)
				{
					int count = number / 10;
					if (len > 6 + count)
						return GetUnitSprite(str + 6 + count, sprite);
					else
						return sprite;
				}
			}
		}
		if (strncmp(str, "mouseover", 9) == 0)
		{
			if (m_pMouseInPlayer)
			{
				if (len == 9)
					return m_pMouseInPlayer;
				else
					return GetUnitSprite(str + 9, m_pMouseInPlayer);
			}

		}
	}
	return NULL;
}

// --------------------------------------------------------------------------------------
void CGameMain::SendWarningMsg(const char* msg)
{
	if (m_interface && m_interface->GetUiWorld())
	{
		lua_State* L = m_interface->GetUiWorld()->GetLuaState();
		if (msg && L)
		{
			lua_pushstring(L, msg);
			lua_setglobal(L, "arg1");
			m_interface->SendWorldEvent("WARNING_MESSAGE");
		}
	}
}
// --------------------------------------------------------------------------------------
void CGameMain::SendWarningMsgEx(const char* format, ...)
{
	char buf[2048];
	va_list args;
	va_start(args, format);
	SwapStringExA(buf, format, args);
	va_end(args);

	SendWarningMsg(buf);
}
// --------------------------------------------------------------------------------------
void CGameMain::SendSystemMsg(const char* msg)
{
	if (m_interface && m_interface->GetUiWorld())
	{
		lua_State* L = m_interface->GetUiWorld()->GetLuaState();
		if (msg && L)
		{
			lua_pushstring(L, msg);
			lua_setglobal(L, "arg1");
			m_interface->SendWorldEvent("SYSTEM_MESSAGE");
		}
	}
}
// --------------------------------------------------------------------------------------
void CGameMain::SendSystemMsgEx(const char* format, ...)
{
	char buf[2048];
	va_list args;
	va_start(args, format);
	SwapStringExA(buf, format, args);
	va_end(args);

	SendSystemMsg(buf);
}
// --------------------------------------------------------------------------------------
void CGameMain::SendSystemChatMsg(const char* msg)
{
	if (msg)
	{
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", msg);
	}
}
// --------------------------------------------------------------------------------------
void CGameMain::SendSystemChatMsgEx(const char* format, ...)
{
	char buf[2048];
	va_list args;
	va_start(args, format);
	SwapStringExA(buf, format, args);
	va_end(args);

	SendSystemChatMsg(buf);
}
// --------------------------------------------------------------------------------------
void CGameMain::SendScrollBannerMessage(const char* msg)
{
	if (!msg)
		return;
	lua_State* L = m_interface->GetLuaState();
	lua_pushstring(L, msg);
	lua_setglobal(L, "arg1");
	g_pInterface->SendWorldEvent(UiEvent_ScrollBannerMessage);

	SendSystemChatMsg(msg);
}
// --------------------------------------------------------------------------------------
void CGameMain::SendScrollBannerMessageEx(const char* format, ...)
{
	char buf[2048];
	va_list args;
	va_start(args, format);
	SwapStringExA(buf, format, args);
	va_end(args);

	SendScrollBannerMessage(buf);
}
// --------------------------------------------------------------------------------------
void CGameMain::SendMessageDialog(const char* msg)
{
	if (!msg)
		return;
	lua_State* L = m_interface->GetLuaState();
	lua_pushstring(L, msg);
	lua_setglobal(L, "arg1");
	g_pInterface->SendWorldEvent(UiEvent_ShowMessageDialog);
}
// --------------------------------------------------------------------------------------
void CGameMain::SendMessageDialogEx(const char* format, ...)
{
	char buf[2048];
	va_list args;
	va_start(args, format);
	SwapStringExA(buf, format, args);
	va_end(args);

	SendMessageDialog(buf);
}

// --------------------------------------------------------------------------------------
void CGameMain::SendPlayTimeQuataNotify(const char* msg)
{
	if (msg == NULL)
		return;

	lua_State* L = m_interface->GetLuaState();
	lua_pushstring(L, msg);
	lua_setglobal(L, "arg1");
	g_pInterface->SendWorldEvent(UiEvent_PlayTimeQuataNotify);
}
// --------------------------------------------------------------------------------------
float CGameMain::GetDistanceWithPlayer(CRoleSprite* sprite)
{
	if (sprite == NULL || m_pPlayer == NULL)
		return 100000.0f;

	CRoleSprite* spriteControlProxy = m_pPlayer->GetProxyControlSprite();
	return (spriteControlProxy->GetPos() - sprite->GetPos()).Magnitude();
}

// --------------------------------------------------------------------------------------
void CGameMain::FollowSprite(CRoleSprite* sprite)
{
	if (sprite && sprite == m_pPlayer) {
		// ¤£¯à¸òÀH¦Û¤v
		SendWarningMsg(g_ObjectData->GetString("SYS_FOLLOW_SELF"));
		return;
	}

	if (m_follow.target != sprite)
	{
		const char* targetName = "";
		if (sprite) {
			targetName = sprite->GetName();
		}
		else if (m_follow.target) {
			targetName = m_follow.target->GetName();
		}
		m_follow.target = sprite;
		m_follow.update = 0.0f;

		if (m_follow.target == NULL)
		{
			// °±¤î¸òÀH
			SendSystemMsgEx(g_ObjectData->GetString("SYS_FOLLOW_END"), targetName);
		}
		else
		{
			// °±¤î·Æ¹«²¾°Ê
			ClearMoveToPoint();

			// ¸òÀH
			SendSystemMsgEx(g_ObjectData->GetString("SYS_FOLLOW_BEGIN"), targetName);
		}
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::MoveForwardState(bool start)
{
	m_moveState.forward = start;
	m_moveState.autoRun = false;
	FollowSprite(NULL);
	ClearPathMove();
	ClearMoveToPoint();
}
// --------------------------------------------------------------------------------------
void CGameMain::MoveBackwardState(bool start)
{
	m_moveState.backward = start;
	m_moveState.autoRun = false;
	FollowSprite(NULL);
	ClearPathMove();
	ClearMoveToPoint();
}
// --------------------------------------------------------------------------------------
void CGameMain::TurnRightState(bool start)
{
	m_moveState.turnRight = start;
	FollowSprite(NULL);
	ClearPathMove();
	ClearMoveToPoint();
}
// --------------------------------------------------------------------------------------
void CGameMain::TurnLeftState(bool start)
{
	m_moveState.turnLeft = start;
	FollowSprite(NULL);
	ClearPathMove();
	ClearMoveToPoint();
}
// --------------------------------------------------------------------------------------
void CGameMain::StrafeRightState(bool start)
{
	m_moveState.strafeRight = start;
	m_moveState.autoRun = false;
	FollowSprite(NULL);
	ClearPathMove();
	ClearMoveToPoint();
}
// --------------------------------------------------------------------------------------
void CGameMain::StrafeLeftState(bool start)
{
	m_moveState.strafeLeft = start;
	m_moveState.autoRun = false;
	FollowSprite(NULL);
	ClearPathMove();
	ClearMoveToPoint();
}
// --------------------------------------------------------------------------------------
// forceState ­È¬°1 ±j¨î¶}±Ò
// forceState ­È¬°2 ±j¨îÃöÃö
void CGameMain::ToggleAutoRune(int forceState)
{
	if (forceState == 0)
		m_moveState.autoRun = !m_moveState.autoRun;
	else if (forceState == 1)
		m_moveState.autoRun = true;
	else if (forceState == 2)
		m_moveState.autoRun = false;

	FollowSprite(NULL);
	ClearPathMove();
	ClearMoveToPoint();
}
// --------------------------------------------------------------------------------------
void CGameMain::ToggleRun()
{
	m_moveState.walk = !m_moveState.walk;
	if (m_pPlayer)
	{
		m_pPlayer->SetWalkState(m_moveState.walk);
	}
}
// --------------------------------------------------------------------------------------
void CGameMain::ToggleSheath()
{
	if (m_pPlayer)
	{
		m_pPlayer->SetUnholsterState(!m_pPlayer->GetUnholsterState());
	}
}
// --------------------------------------------------------------------------------------
void CGameMain::SitOrStand()
{
	if (m_pPlayer)
	{
		// §¤¤U
		if (!m_pPlayer->GetSitState())
		{
			ClearPathMove();
			ClearMoveToPoint();
		}
		m_pPlayer->SetSitState(!m_pPlayer->GetSitState());
	}
}

// --------------------------------------------------------------------------------------
int CGameMain::OnClick(CRoleSprite* target)
{
	//CRoleSprite* player = GetPlayer();
	CRoleSprite* player = GetPlayer();

	if (player && m_botLocked == false)
	{
		CRoleSprite* spriteControlProxy = player->GetProxyControlSprite();
		CRoleSprite* implementSprite = player->GetImplementSprite();

		// ª±®a¬O§_¤w¸g¦º¤`
		if (player->GetDeadState() == true)
		{
			return -1;
		}

		// ¬O§_¦b¸ü¨ã¤W¤£¯à§@°Ê§@
		if (implementSprite)
		{
			return -1;
		}

		if (target == NULL)
			target = player->GetTargetSprite();

		if (target == NULL)
		{
			SendWarningMsg(g_ObjectData->GetString("SYS_TOUCH_NOTARGET"));

			return -1;				// µL°µ¥Î¥Ø¼Ð
		}

		float distance = (target->GetPos() - spriteControlProxy->GetPos()).Magnitude();

		// «ÍÅéª«¥ó
		if (target->GetCorpseState())
		{
			if (distance > _MAX_ROLE_TOUCH_RANGE_)
			{
				SendWarningMsg(g_ObjectData->GetString("SYS_TOUCH_TARGET_TOFAR"));
				return -2;			// ¶ZÂ÷¹L»·
			}

			if (CNetGlobal::RoleData()->PlayerBaseData->PlayTimeQuota < 2 * 60 * 60)
			{
				g_pGameMain->SendSystemMsg(g_ObjectData->GetString("BTI_MSG_NOTHEALTH"));
				g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("BTI_MSG_NOTHEALTH"));
				return -2;
			}

			// ­n¨D¶}±Ò«ÍÅé
			g_pLootFrame->SendOpenBooty(target->GetIdentify());
		}
		// Àu¥ý§PÂ_¬O§_¥i§ðÀ»
		else if (target->GetPriorityAttack() && target->GetCanAttackState())
		{
			if (target->GetDeadState())
			{
				SendWarningMsg(g_ObjectData->GetString("SYS_TOUCH_TARGET_DEAD"));
				return -3;		// ¥Ø¼Ð¤w¦º¤`
			}
			else
				AttackTarget(target);
		}
		// §PÂ_¥ô°ÈÄ²µo
		else if (target->GetTouchRange() != 0)
		{
			if (distance > target->GetTouchRange())
			{
				SendWarningMsg(g_ObjectData->GetString("SYS_TOUCH_TARGET_TOFAR"));
				return -2;			// ¶ZÂ÷¹L»·
			}
			else
				CNetCli_Script::OnEvent_TouchPlot(target->GetIdentify());
		}
		else if (target->GetPriorityAttack() == false && target->GetCanAttackState())
		{
			if (target->GetDeadState())
			{
				SendWarningMsg(g_ObjectData->GetString("SYS_TOUCH_TARGET_DEAD"));
				return -3;		// ¥Ø¼Ð¤w¦º¤`
			}
			else
				AttackTarget(target);
		}
		// ¶ð­¼¸ü¨ã
		else if (target->GetImplementMaxNum() > target->GetImplementCount())
		{
			if (distance > MAX_IMPLEMENT_TOUCH_RANGE)
			{
				SendWarningMsg(g_ObjectData->GetString("SYS_TOUCH_TARGET_TOFAR"));
				return -2;			// ¶ZÂ÷¹L»·
			}
			else if (player->GetMountSprite())
			{
				SendWarningMsg(g_ObjectData->GetString("SYS_IMPELMENT_LIFTING_MOUNT"));
				return -2;			// ¥²»Ý¤U°¨
			}
			else if (player->IsDisableWagon())
			{
				SendWarningMsg(g_ObjectData->GetString("SYS_IMPELMENT_HAS_BUFF"));
				return -3;			// §A¨­¤W¦³ª¬ºA¤£¯à¤W¸ü¨ã
			}
			else
			{
				Net_Move::S_AttachObjRequest(player->GetIdentify(), target->GetIdentify(), "", "", eMoveAttach_Implement);
			}
		}
		/*
		else if ( target->GetCanAttackState() )
		{
			if ( target->GetDeadState() )
			{
				SendWarningMsg(g_ObjectData->GetString("SYS_TOUCH_TARGET_DEAD"));
				return -3;		// ¥Ø¼Ð¤w¦º¤`
			}
			else
				AttackTarget(target);
		}
		*/
		//¦]¬°¤k¹²­nµL½u¶ZÂ÷¨Ï¥Î ¤Î¤£¯à¨Ï¥Îªº³Ã­Ñ ¤£¥X²{"¶ZÂ÷¹L»·" §ï¦¨¨C­Ó	CheckedStatus ³£­n§PÂ_
		//else if ( distance > _MAX_ROLE_TOUCH_RANGE_ )
		//{
		//	SendWarningMsg(g_ObjectData->GetString("SYS_TOUCH_TARGET_TOFAR"));
		//	return -2;		// ¶ZÂ÷¹L»·
		//}
		else
		{
			float maxTouchRange = target->GetClientTouchRange();

			switch (target->GetCheckedStatus())
			{
			case eChecked_QuestNPC:
				if (distance > maxTouchRange)
				{
					SendWarningMsg(g_ObjectData->GetString("SYS_TOUCH_TARGET_TOFAR"));
					return -2;		// ¶ZÂ÷¹L»·
				}
				CNetCli_Script::OnEvent_TouchQuestNPC(target->GetIdentify());
				break;

			case eChecked_QuestObj:
				if (distance > maxTouchRange)
				{
					SendWarningMsg(g_ObjectData->GetString("SYS_TOUCH_TARGET_TOFAR"));
					return -2;		// ¶ZÂ÷¹L»·
				}
				// ¦Û°Ê¤U°¨,¤£¯à¦bÃM­¼®É°µ¥Î
				// Dismount();
				CNetCli_Script::OnEvent_TouchQuestObj(target->GetIdentify());
				break;

			case eChecked_Mine:
			case eChecked_Herb:
			case eChecked_Lumber:
				if (distance > maxTouchRange)
				{
					SendWarningMsg(g_ObjectData->GetString("SYS_TOUCH_TARGET_TOFAR"));
					return -2;		// ¶ZÂ÷¹L»·
				}
				CNetCli_Gather::OnEvent_TouchGatherObj(target->GetIdentify());
				break;
			case eChecked_FurnitureNormal:						// ¤@¯ë³Ã­Ñ
			case eChecked_FurnitureSound:						// ³Ã­Ñ ¯dÁn¾÷
				g_pHousesFrame->HousesControl_UseFurniture(g_pHousesFrame->m_MouseEnterFurnishingID);
				break;

			case eChecked_FurnitureStorage: // ³Ã­Ñ Ä_½c ¦ç¬[
				if (distance > maxTouchRange)
				{
					SendWarningMsg(g_ObjectData->GetString("SYS_TOUCH_TARGET_TOFAR"));
					return -2;		// ¶ZÂ÷¹L»·
				}
				g_pHousesFrame->HousesControl_UseFurniture(g_pHousesFrame->m_MouseEnterFurnishingID);
				break;
			case eChecked_Maid: // ¤k¹²
				g_pHousesFrame->HousesControl_DoMaid();
				break;
			case eChecked_Servant: // ¤k¹²
				g_pHousesFrame->HousesControl_DoServant(target->GetWorldGUID());
				break;
			case eChecked_GuildHousesBuildPoint:							// ¤½·|«Î°Ñ¦ÒÂI
				g_pGuildHousesFrame->BuildPointOnClick(target->GetWorldGUID());
				break;
			}
		}
	}
	return 0;
}

// --------------------------------------------------------------------------------------
void CGameMain::MoveToScreenPoint_AddSprite(int SpriteID)
{
	m_MoveToScreenPointSpriteIDs.insert(SpriteID);
}
// --------------------------------------------------------------------------------------
void CGameMain::MoveToScreenPoint_DelSprite(int SpriteID)
{
	m_MoveToScreenPointSpriteIDs.erase(SpriteID);
}
// --------------------------------------------------------------------------------------
void CGameMain::MoveToScreenPoint_ClearSprite()
{
	m_MoveToScreenPointSpriteIDs.clear();
}

// --------------------------------------------------------------------------------------
void CGameMain::MoveToScreenPoint(int x, int y)
{
	if (m_pCamera == NULL)
		return;

	// Get the target position for the pick ray in world coordinates
	CRuVector3 homPt, viewPt, worldPt;
	m_pCamera->Coord_ScreenToHomogeneous(x, y, this->GetScreenWidth(), this->GetScreenHeight(), homPt);
	m_pCamera->Coord_HomogeneousToView(homPt, viewPt);
	m_pCamera->Coord_ViewToWorld(viewPt, worldPt);

	// Build collision segment
	CRuSegment3 colSegment(m_pCamera->GetCameraPosition(), m_pCamera->GetCameraPosition() + (worldPt - m_pCamera->GetCameraPosition()).Normalize() * 1200.0f);

	// Query against terrain for nearest intersection
	CRuCollisionQuery_Segment_Nearest segQuery;
	CRuMatrix4x4 identityMatrix;
	segQuery.m_worldTransform = &identityMatrix;
	segQuery.m_invWorldTransform = &identityMatrix;
	segQuery.m_ignoreCollisionFlag = FALSE;
	segQuery.m_exclusionMask = 0x00A00000;			//!< Ignore water and foliage
	segQuery.m_colSegment = colSegment;


	int SpriteID;
	CRoleSprite* pSprite;

	set<int>::iterator itPos = m_MoveToScreenPointSpriteIDs.begin();
	set<int>::iterator itEnd = m_MoveToScreenPointSpriteIDs.end();
	for (; itPos != itEnd; ++itPos)
	{
		SpriteID = *itPos;
		pSprite = FindSpriteByID(SpriteID);
		if (NULL == pSprite || NULL == pSprite->GetRuEntity())
		{
			continue;
		}
		pSprite->GetRuEntity()->QueryCollision(&segQuery);

		if (pSprite->GetRuEntity()->GetOption(ruENTITYOPTION_DECALTARGET) != TRUE)
		{
			pSprite->GetRuEntity()->SetOption(ruENTITYOPTION_DECALTARGET, TRUE);
		}

		//g_pGameMain->GetRuTerrain()->AddChild( pSprite->GetRuEntity() );

		//pSprite->SetVisible( false );
	}

	IBaseSprite::GetCollisionContainer()->QueryCollision(&segQuery);

	if (segQuery.m_positiveHit)
	{
		SetMoveToPoint(segQuery.m_collisionPoint);
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::SetMoveToPoint(CRuVector3& point, CRoleSprite* sprite, bool forceMove)
{
	FollowSprite(NULL);
	ClearPathMove();
	ClearMoveTargetEntity();

	// Ãö³¬¦Û°Ê¦V°Ê²¾°Ê
	m_moveState.autoRun = false;

	CRoleSprite* player = GetPlayer();
	if (player && m_botLocked == false)
	{
		CRoleSprite* controlSprite = player->GetProxyControlSprite();
		SpriteSpecialEffect effect = player->GetSpecailEffect();
		bool isMoveControl = !IsPlayerMoveLocked();

		float distance = (point - controlSprite->GetPos()).Magnitude();
		if (sprite && sprite->CheckTouchRange(distance) && forceMove)
		{
			CRuVector3 direction = sprite->GetPos() - player->GetPos();
			// if (!IsSame(direction.m_x, 0.0f) && IsSame(!direction.m_z, 0.0f))
			// {
				// direction.m_y = 0.0f;
			// }
			direction.m_y = 0.0f;
			// ½Õ¾ã­±¦V
			controlSprite->SetDirection(direction);

			OnClick(sprite);
		}
		else
		{
			// ¯S®íª¬ºA¤£¯à²¾°Ê
			if (isMoveControl == false || player->IsProxyLoadComplete() == false)
				return;

			m_mouseMove.enable = true;
			m_mouseMove.update = 0.5f;
			m_mouseMove.sprite = sprite;

			// ¹ï¥Ø¼Ð²¾°Ê
			if (sprite)
			{
				m_mouseMove.target = player->GetPos();
				m_mouseMove.distance = 0.0f;
			}
			// ¹ï®y¼Ð²¾°Ê
			else
			{
				m_mouseMove.target = point;
				m_mouseMove.distance = distance;
				DisplayMoveTargetEntity(point);
			}

			controlSprite->SetTargetPos(point);
		}
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::ClearMoveToPoint()
{
	m_magicDistance = 0.0f;					// ¨ú®ø²¾°Ê¬Iªk

	if (m_mouseMove.enable)
	{
		m_mouseMove.enable = false;
		m_mouseMove.sprite = NULL;

		if (m_pPlayer)
		{
			m_pPlayer->SetPos(m_pPlayer->GetPos());

			CRoleSprite* mountSprite = m_pPlayer->GetMountSprite();
			if (mountSprite) {
				mountSprite->SetPos(mountSprite->GetPos());
			}
		}
	}
	ClearMoveTargetEntity();
}

// --------------------------------------------------------------------------------------
void CGameMain::RotateCamera(float xDelta, float yDelta, bool updatePlayerDir)
{
	if (m_pCamera == NULL || (xDelta == 0.0f && yDelta == 0.0f))
		return;

	if (m_cameraControl)
		return;

	/*
	if ( m_cameraFollowSprite )
		return;
	*/

	CRuVector3 dir = m_pCamera->GetTargetPosition() - m_pCamera->GetCameraPosition();
	if (dir.m_x != 0.0f || dir.m_y != 0.0f || dir.m_z != 0.0f)
	{
		dir.Normalize();
		float theta = dir.Dot(CRuVector3(0.0f, 1.0f, 0.0f));

		if (theta + yDelta > MAX_CAMERA_THETA)
			yDelta = MAX_CAMERA_THETA - theta;
		if (theta + yDelta < MIN_CAMERA_THETA)
			yDelta = MIN_CAMERA_THETA - theta;

		m_pCamera->Cycle(xDelta, CRuVector3(0.0f, 1.0f, 0.0f));
		m_pCamera->Cycle(yDelta, m_pCamera->GetCameraRightVector());

		if (updatePlayerDir)
		{
			RotateTargetEffect(xDelta, yDelta);

			if (m_targetEntity)
				yDelta = 0.0f;

			//SetTargetEffectDirection(m_pCamera->GetCameraPosition() - m_pCamera->GetTargetPosition());
		}

		if (updatePlayerDir && m_pPlayer && m_pathPoints.Count() == 0 && m_isFreeCamera == false)
		{
			CRoleSprite* controlSprite = m_pPlayer->GetProxyControlSprite();
			SpriteSpecialEffect effect = m_pPlayer->GetSpecailEffect();
			bool isMoveControl = m_pPlayer->GetMoveControl();

			FollowSprite(NULL);				// ¨ú®ø¸òÀH

			if (isMoveControl && !m_mouseMove.enable)
			{
				switch (m_pPlayer->GetActorState(1))
				{
				case ruFUSION_ACTORSTATE_SIT_BEGIN:
				case ruFUSION_ACTORSTATE_SIT_LOOP:
				case ruFUSION_ACTORSTATE_SIT_END:
				case ruFUSION_ACTORSTATE_SLEEP_BEGIN:
				case ruFUSION_ACTORSTATE_SLEEP_LOOP:
				case ruFUSION_ACTORSTATE_SLEEP_END:
				case ruFUSION_ACTORSTATE_DYING:
				case ruFUSION_ACTORSTATE_DEAD:
				case ruFUSION_ACTORSTATE_REVIVING:
				case ruFUSION_ACTORSTATE_SPAWN_BEGIN:
				case ruFUSION_ACTORSTATE_SPAWN_END:
				case ruFUSION_ACTORSTATE_CROUCH_BEGIN:
				case ruFUSION_ACTORSTATE_CROUCH_LOOP:
				case ruFUSION_ACTORSTATE_CROUCH_END:
				case ruFUSION_ACTORSTATE_CRAFTING_BEGIN:
				case ruFUSION_ACTORSTATE_CRAFTING_LOOP:
				case ruFUSION_ACTORSTATE_CRAFTING_END:
				case ruFUSION_ACTORSTATE_MINING_BEGIN:
				case ruFUSION_ACTORSTATE_MINING_LOOP:
				case ruFUSION_ACTORSTATE_MINING_END:
				case ruFUSION_ACTORSTATE_LUMBERJACK_BEGIN:
				case ruFUSION_ACTORSTATE_LUMBERJACK_LOOP:
				case ruFUSION_ACTORSTATE_LUMBERJACK_END:
				case ruFUSION_ACTORSTATE_GATHER_BEGIN:
				case ruFUSION_ACTORSTATE_GATHER_LOOP:
				case ruFUSION_ACTORSTATE_GATHER_END:
				case ruFUSION_ACTORSTATE_LOOT_BEGIN:
				case ruFUSION_ACTORSTATE_LOOT_LOOP:
				case ruFUSION_ACTORSTATE_LOOT_END:
				case ruFUSION_ACTORSTATE_CARRY_BEGIN:
				case ruFUSION_ACTORSTATE_CARRY_LOOP:
				case ruFUSION_ACTORSTATE_CARRY_END:
				case ruFUSION_ACTORSTATE_DAZED_BEGIN:
				case ruFUSION_ACTORSTATE_DAZED_LOOP:
				case ruFUSION_ACTORSTATE_DAZED_END:
				case ruFUSION_ACTORSTATE_SLUMBER_BEGIN:
				case ruFUSION_ACTORSTATE_SLUMBER_LOOP:
				case ruFUSION_ACTORSTATE_SLUMBER_END:
				case ruFUSION_ACTORSTATE_ACTIVATE_BEGIN:
				case ruFUSION_ACTORSTATE_ACTIVATE_LOOP:
				case ruFUSION_ACTORSTATE_ACTIVATE_END:
				case ruFUSION_ACTORSTATE_READING_BEGIN:
				case ruFUSION_ACTORSTATE_READING_LOOP:
				case ruFUSION_ACTORSTATE_READING_END:
				case ruFUSION_ACTORSTATE_SIT_CHAIR_BEGIN:
				case ruFUSION_ACTORSTATE_SIT_CHAIR_LOOP:
				case ruFUSION_ACTORSTATE_SIT_CHAIR_END:
					break;

				default:
					controlSprite->SetDirection(m_pCamera->GetTargetPosition() - m_pCamera->GetCameraPosition());
				}
			}
		}
	}
}

float mine_acos(float value)
{
	if (value < -1.0f) value = -1.0f;
	if (value > 1.0f) value = 1.0f;
	return acos(value);
}

// --------------------------------------------------------------------------------------
void CGameMain::SetCameraForward(CRuVector3 forward, bool immediately)
{
	if (GetCameraFollowEnable() == false)
		return;

	if (m_pPlayer == NULL || m_pCamera == NULL)
		return;

	m_cameraFinalVector = forward.Normalize();
	if (immediately)
	{
		CRoleSprite* controlSprite = m_pPlayer->GetProxyControlSprite();
		CRuVector3 playerPosition = controlSprite->GetPos();

		m_cameraCurrentVector = m_cameraFinalVector;

		// ¨ú±oª±®a­±¦V»PÃèÀY¤è¦V(¤£­pºâY¶b)
		forward.m_y = 0.0f;
		forward.Normalize();

		CRuVector3 relativeCamPos = m_pCamera->GetTargetPosition() - m_pCamera->GetCameraPosition();
		relativeCamPos.m_y = 0.0f;
		relativeCamPos.Normalize();

		// ¨ú±o¨â­Ó¦V¶q§¨¨¤
		float theta = mine_acos(forward.Dot(relativeCamPos));

		// g_pError->AddMessage(0, 0, "theta %f forward(%f,%f) camera(%f,%f)", theta, forward.m_x, forward.m_z, relativeCamPos.m_x, relativeCamPos.m_z);

		// §¨¨¤¤Ó¤p®É¤£³B²z
		if (theta < 0.005f)
			return;

		// ¥Ñ¦V¶q¿n¨ú±o¨â¦V¶q ¬°¥¿¦V±ÛÂà©Î
		CRuVector3 vecCross = CrossProduct(relativeCamPos, forward);
		vecCross.Normalize();
		float angle = mine_acos(vecCross.Dot(CRuVector3(0.0f, 1.0f, 0.0f)));

		if (angle > -0.1 && angle < 0.1)
			theta = -theta;

		// ­pºâ±ÛÂà¦Ê¤À¤ñ
		FLOAT fPercentage = 1.0f - powf(0.7f, 4.0f * m_elapsedTime);

		RotateCamera(theta * fPercentage, 0.0f);
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::SetTutorialState(int index, bool state)
{
	if (state)
		CNetGlobal::RoleData()->PlayerBaseData->ClientFlag.SetFlagOn(index);
	else
		CNetGlobal::RoleData()->PlayerBaseData->ClientFlag.SetFlagOff(index);
	NetCli_RoleValue::S_ClientFlag(CNetGlobal::RoleData()->PlayerBaseData->ClientFlag);
}

// --------------------------------------------------------------------------------------
bool CGameMain::GetTutorialState(int index)
{
	return CNetGlobal::RoleData()->PlayerBaseData->ClientFlag.GetFlag(index);
}

// --------------------------------------------------------------------------------------
void CGameMain::ClearTutorialState()
{
	CNetGlobal::RoleData()->PlayerBaseData->ClientFlag.ReSet();
	NetCli_RoleValue::S_ClientFlag(CNetGlobal::RoleData()->PlayerBaseData->ClientFlag);
}

// --------------------------------------------------------------------------------------
void CGameMain::TakeScreenshot()
{
	IRuSwapChain* swapChain = GetRuSwapChain();

	if (swapChain)
	{
		// Build path to the screen shots directory
		char outputDirectory[2048];
		sprintf(outputDirectory, "%s%s", g_ruResourceManager->GetRootDirectory(), "screenshots");

		// Make sure the screen shots directory exists
		CreateDirectory(outputDirectory, NULL);
		//							SHCreateDirectoryEx(NULL, outputDirectory, NULL);

		// Get local system time
		SYSTEMTIME localTime;
		GetLocalTime(&localTime);

		// Build the output file name
		char outputFileName[2048];
		sprintf(outputFileName, "%s\\RAScrnShot_%02d%02d%02d_%02d%02d%02d.bmp", outputDirectory, localTime.wYear, localTime.wMonth, localTime.wDay, localTime.wHour, localTime.wMinute, localTime.wSecond);

		// Save swap chain to file
		swapChain->SaveToFile(outputFileName, ruIMAGEFILEFORMAT_BMP);

		g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, outputFileName, "");
		SendSystemMsg(g_ObjectData->GetString("TAKE_SCREEN_SHOT"));
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::CancelCastSpell()
{
	GameObjDbStructEx* itemDB;
	switch (m_castSpellState)
	{
	case CASTSPELL_CASTING:
		itemDB = CNetGlobal::GetObj(m_castSpellID);
		if (itemDB)
		{
			float SpellTime = itemDB->MagicCol.SpellTime;
			SpellTime = SpellTime * (1 + CNetGlobal::RoleData()->TempData.Attr.MagicSpellSpeed());

			if (itemDB->MagicCol.Flag.Interrupt_OnMove && SpellTime > 0.1f)
				NetCli_Magic::CancelSpellMagic();
		}
		break;
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::LoadClientLua()
{
	CUi* ui = m_interface->GetUi();
	if (ui == NULL)
		return;

	char filename[512];
	for (int i = 0; i < 1000; i++)
	{
		sprintf_s(filename, 512, "CliLuaScript\\%05d.lua", i);

		IRuStream* stream = g_ruResourceManager->LoadStream(filename);
		if (stream)
		{
			ui->LoadScriptFromBuffer((const char*)stream->OpenStreamMapping(), stream->GetStreamSize(), filename);
		}
	}

	// ªì©l¤Æ¤¶­±ÅÜ¦â¦r¦ê
	g_pQuestDetail->InitKeyWorld();

	// LoadLuaFile("Data\\CliLuaScript\\");
}

// --------------------------------------------------------------------------------------
void CGameMain::UpdateMouseMove()
{
	CRoleSprite* player = GetPlayer();
	if (player == NULL)
		return;

	CRoleSprite* controlSprite = player->GetProxyControlSprite();
	if (controlSprite == NULL)
		return;

	float speed = controlSprite->GetMoveSpeed();
	if (m_moveState.walk)
		speed /= WALK_SLOW_SCALE;

	// ¯S®í±¡ªpª±®a³QÂê©wµLªk²¾°Ê
	if (IsPlayerMoveLocked() == true) {
		ClearMoveToPoint();
		return;
	}

	if (m_mouseMove.enable)
	{
		//ÂI¿ïª«¥ó²¾°Ê
		if (m_mouseMove.sprite)
		{
			CRoleSprite* sprite = m_mouseMove.sprite;
			CRoleSprite* target = player->GetTargetSprite();

			/*
			if ( sprite->GetTargetState() && target != sprite )
			{
				ClearMoveToPoint();
				return;
			}
			*/

			float distance = 0.0f;
			if (sprite != controlSprite)
				distance = (sprite->GetPos() - controlSprite->GetPos()).Magnitude();

			if (m_magicDistance != 0.0f && (m_magicDistance * 0.95f) > distance)
			{
				ClearMoveToPoint();						// ²M°£²¾°Ê¸ê°T
				SendPlayerPos();						// °e¥X¥Ø«e§¤¼Ð­È
				SetBeginSpell(sprite);					// ¹ï¥Ø¼Ð¬I©ñªk³N
				return;
			}
			else if (sprite->CheckTouchRange(distance))
			{
				ClearMoveToPoint();						// ²M°£²¾°Ê¸ê°T
				SendPlayerPos();
				OnClick(sprite);
				return;
			}

			if (m_mouseMove.update > 0.0f)
				m_mouseMove.update -= m_elapsedTime;
			else
			{
				float distance = (controlSprite->GetPos() - m_mouseMove.target).Magnitude();
				if (distance < speed / 2.0f)
				{
					ClearMoveToPoint();
				}
				else
				{
					m_mouseMove.update = 0.5f;
					m_mouseMove.target = controlSprite->GetPos();
					controlSprite->SetTargetPos(sprite->GetPos());
				}
			}
		}
		else
		{
			float distance = (m_mouseMove.target - controlSprite->GetPos()).Magnitude();

			if (m_magicDistance != 0.0f && (m_magicDistance * 0.95f) > distance)
			{
				ClearMoveToPoint();					// ²M°£²¾°Ê¸ê°T				
				SetBeginSpell(NULL);				// ¹ï¥Ø¼Ð¬I©ñªk³N
				return;
			}

			if (controlSprite->GetMoveTime() < 0.05f)
				ClearMoveToPoint();
			else if (m_mouseMove.update > 0.0f)
				m_mouseMove.update -= m_elapsedTime;
			else
			{
				if (m_mouseMove.distance - distance < speed / 4.0f)
				{
					ClearMoveToPoint();
				}
				else
				{
					m_mouseMove.update = 0.5f;
					m_mouseMove.distance = distance;
					controlSprite->SetTargetPos(m_mouseMove.target);
				}
			}
		}

		// ÃèÀY¸òÀH¨¤¦â²¾°Ê
		SetCameraForward(controlSprite->GetDirection(), true);
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::UpdatePathMove()
{
	CRoleSprite* player = GetPlayer();
	if (player == NULL)
		return;

	CRoleSprite* controlSprite = player->GetProxyControlSprite();
	if (controlSprite == NULL)
		return;

	// ©¹¥Ø¼Ð²¾°Ê
	if (m_pathPoints.Count() > 0)
	{
		CRuVector3 playerPosition = controlSprite->GetPos();
		CRuVector3 targetPosition = m_pathPoints.Get(0);
		CRuVector3 moveVector = targetPosition - playerPosition;
		moveVector.m_y = 0.0f;

		float speed = controlSprite->GetMoveSpeed();

		// ¯S®í±¡ªpª±®a³QÂê©wµLªk²¾°Ê
		if (IsPlayerMoveLocked() == true)
			return;

		// ÀË´ú¬O§_¨ì‘´¥Ø¼ÐÂI (¶ZÂ÷§PÂ_©ÎªÌ¦V¶q§PÂ_)
		if (moveVector.Magnitude() / speed < 0.5f)
		{
			m_pathPoints.RemoveAt(0);
		}

		// ²¾°Ê¨ì¤U¤@ÂI
		if (m_pathPoints.Count() > 0)
		{
			targetPosition = m_pathPoints.Get(0);

			controlSprite->SetDirection(CRuVector3(targetPosition.m_x - playerPosition.m_x, 0.0f, targetPosition.m_z - playerPosition.m_z));
			controlSprite->SetMoveVector(CRuVector3(0.0f, 0.0f, speed), NEXT_MOVETIME);

			// ÃèÀY¸òÀH¨¤¦â²¾°Ê
			SetCameraForward(controlSprite->GetDirection(), true);

			/*
			// ÃèÀY¸òÀH¨¤¦â²¾°Ê
			if ( m_pCamera )
			{
				// ­pºâ®t­È

				CRuVector3 relativeCamPos = m_pCamera->GetCameraPosition() - m_pCamera->GetTargetPosition();
				CRuVector3 absoluteCamPos = controlSprite->GetDirection() * (relativeCamPos.Magnitude() * -1.0f) + playerPosition;
				m_pCamera->SetTargetPosition( playerPosition + CRuVector3(0.0f, 16.0f, 0.0f) );
				m_pCamera->SetCameraPosition( absoluteCamPos + CRuVector3(0.0f, 16.0f, 0.0f) );
				m_pCamera->SetCameraUpVector( CRuVector3(0.0f, 1.0f, 0.0f) );
			}
			*/

			// ·sªº²¾°Ê¦V¶q
			/*
			CRuVector3 moveVector(targetPosition.m_x - playerPosition.m_x, 0.0f, targetPosition.m_z - playerPosition.m_z);
			float time = moveVector.Magnitude() / speed;
			moveVector = moveVector / time;
			CRuQuaternion quaternion;
			quaternion.RotateIntoVector(CRuVector3(0.0f, 0.0f, 1.0f), player->GetDirection());
			quaternion.TransformVector(moveVector);
			player->SetMoveVector(moveVector, NEXT_MOVETIME);
			*/
		}
	}
	/*
	m_pathPoints
	if ( m_pathMoveTime < 0.01f )
		return;

	if ( player == NULL )
		return;

	CRuVector3 playerPosition = player->GetPos();
	float playerSpeed = player->GetMoveSpeed();

	m_pathMoveUpdate += m_elapsedTime;
	if ( m_pathMoveUpdate >= m_pathMoveTime )
	{
		// ÀË´ú¥Ø«e®y¼ÐÂI»P¥Ø¼Ð®y¼ÐÂI¶ZÂ÷®t¶Z
		if ( (playerPosition - m_pathTargetPosition).Magnitude() > playerSpeed )
		{
			//
		}


		if ( m_pathPositions.empty() == false )
		{

		}
	}



	vector<CRuVector3>		m_pathPositions;			// ¸ô®|¸ê®Æ
	CRuVector3				m_pathTargetPosition;		// ¸ô®|²¾°Ê¥Ø¼ÐÂI
	float					m_pathMoveTime;				// ¸ô®|²¾°Ê¨ì‘´¥Ø¼Ð©Ò»Ý®É¶¡
	float					m_pathMoveUpdate;			// ¸ô®|²¾°Ê§ó·s®É¶¡
	*/
}

// --------------------------------------------------------------------------------------
void CGameMain::SendPacketReduceMode(PacketReduceState state)
{
	if (m_packetReduceState == state)
		return;

	SendPacketModeStruct mode;

	memset(&mode, 0, sizeof(mode));

	m_packetReduceState = state;
	switch (state)
	{
	case PACKET_REDUCE_NONE:
		NetCli_Other::SL_SetSendPacketMode(mode);
		break;

	case PACKET_REDUCE_PLAYER_MOVE:
		mode.ReducePlayerMove = true;
		NetCli_Other::SL_SetSendPacketMode(mode);
		break;

	case PACKET_REDUCE_PLAYER_MOVE_ALL:
		mode.ReducePlayerMove = true;
		mode.IgnorePlayerMove = true;
		NetCli_Other::SL_SetSendPacketMode(mode);
		break;

	case PACKET_REDUCE_MOVE_ALL:
		mode.ReducePlayerMove = true;
		mode.IgnorePlayerMove = true;
		mode.IgnoreAllMove = true;
		NetCli_Other::SL_SetSendPacketMode(mode);
		break;
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::CheckPlayerPositionHeight()
{
	if (m_pPlayer == NULL || m_pRuTerrain == NULL)
		return;

	CRoleSprite* spriteControlProxy = m_pPlayer->GetProxyControlSprite();
	CRoleSprite* mountSprite = m_pPlayer->GetMountSprite();
	IBaseSprite* parentMount = m_pPlayer->GetParentMount();
	CRuVector3 vec = spriteControlProxy->GetMoveVector();
	CRuVector3 dir = spriteControlProxy->GetDirection();
	CRuVector3 pos = spriteControlProxy->GetPos();
	float moveTime = spriteControlProxy->GetMoveTime();
	float moveSpeed = spriteControlProxy->GetMoveSpeed();
	CRuFusion_ActorState actorState = spriteControlProxy->GetActorState();
	bool aboveWater = spriteControlProxy->GetAboveWaterState();
	bool isInWater = false;

	if (spriteControlProxy->GetRuActor())
		isInWater = spriteControlProxy->GetRuActor()->IsInWater();

	// ÀË¬d°ª«×
	float height = pos.m_y;

	if (m_checkPlayerHeight != m_playerHeight)
		m_playerHeight = (float)m_checkPlayerHeight;

	// if ( m_inWater == false && mountSprite == NULL )
	{
		switch (actorState)
		{
		case ruFUSION_ACTORSTATE_FALLING:
		case ruFUSION_ACTORSTATE_JUMP:
			break;

			// ¤£¬O¸õÅD©ÎªÌ±¼¸¨ª¬ºAÀË´úª±®a®y¼Ð°ª«×
		default:
			if (spriteControlProxy->IsAboveWater() || (spriteControlProxy->IsInWater() && (actorState == ruFUSION_ACTORSTATE_SWIM_SURFACE || actorState == ruFUSION_ACTORSTATE_SWIM_DIVE)))
			{
				if (IsNoSwimming())
				{
					m_playerHeightErrorTime += m_elapsedTime;
					if (m_playerHeightErrorTime > 5.0f)
					{
						m_playerHeightErrorTime = 0.0f;

						// ³qª¾SERVER
						NetCli_Other::SL_Suicide(2);
					}
				}
				else
				{
					m_illegalPositionCount = 0;
					m_playerHeightErrorTime = 0.0f;
				}
				break;
			}

			if (parentMount == NULL)
			{
				if (m_frameTime - m_lastCheckHeightTime > 0.5f)
				{
					m_lastCheckHeightTime = (float)m_frameTime;

					float distance = sqrt(pow(pos.m_x - m_lastCheckHeightPosition.m_x, 2) + pow(pos.m_z - m_lastCheckHeightPosition.m_z, 2));

					if ((pos.m_y - m_lastCheckHeightPosition.m_y) > distance * 3.0f)
					{
						m_illegalPositionCount++;
					}
					else
					{
						m_illegalPositionCount--;
					}

					if (m_illegalPositionCount > 10)
					{
						m_illegalPositionCount = 10;

						// ³qª¾SERVER
						NetCli_Other::SL_Suicide(1);
					}
					else if (m_illegalPositionCount < 0)
					{
						m_illegalPositionCount = 0;
					}

					m_lastCheckHeightPosition = pos;
				}
			}

			// ¤ñ¤W¦¸ÀË´ú­È­n°ª
			if (parentMount == NULL && pos.m_y > m_playerHeight)
			{
				CRuVector3 position = pos + CRuVector3(0.0f, 10.0f, 0.0f);
				float height;

				if (IBaseSprite::TerrainTestHight(position, height, 30) == false &&
					IBaseSprite::TerrainTestHight(position + CRuVector3(3.0f, 0.0f, 0.0f), height, 30) == false &&
					IBaseSprite::TerrainTestHight(position + CRuVector3(-3.0f, 0.0f, 0.0f), height, 30) == false &&
					IBaseSprite::TerrainTestHight(position + CRuVector3(0.0f, 0.0f, 3.0f), height, 30) == false &&
					IBaseSprite::TerrainTestHight(position + CRuVector3(0.0f, 0.0f, -3.0f), height, 30) == false)
				{
					m_playerHeightErrorTime += m_elapsedTime;

					if (m_playerHeightErrorTime > 5.0f)
					{
						m_playerHeightErrorTime = 0.0f;

						// §Aªº°ª«×¦³¿ù»~
						// g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_WARNING_POSITION_ERROR"));

						// ³qª¾SERVER
						NetCli_Other::SL_Suicide(2);
					}

					//
					height = m_playerHeight;
				}
				else
				{
					m_playerHeightErrorTime = 0.0f;
				}

				//g_pError->Show();
				//g_pError->AddMessage(0, 0, "%.2f,%.2f,%.2f", pos.m_y, m_playerHeight, height);
			}
			break;
		}
	}

	m_playerHeight = height;
	m_checkPlayerHeight = (UINT)m_playerHeight;
}

// --------------------------------------------------------------------------------------
bool CGameMain::PathMoveTo(CRuVector3 target)
{
	CRoleSprite* player = GetPlayer();
	if (player == NULL)
		return false;

	ClearPathMove();

	int nResult = FindPath(player->GetPos(), target);
	if (nResult != 0)
	{
		m_pathPoints.Clear();

		if (-1 == nResult) //¤Ó»·
		{
			g_pGameMain->SendSystemChatMsg(g_ObjectData->GetString("SYS_WORLDMAP_AUTOMOVE_FAIL1"));
		}
		else if (-2 == nResult) //¨S¦³¸ô®|
		{
			g_pGameMain->SendSystemChatMsg(g_ObjectData->GetString("SYS_WORLDMAP_AUTOMOVE_FAIL2"));
		}

		return false;
	}

	SendWarningMsg(g_ObjectData->GetString("SYS_PATH_MOVE_START"));
	return true;
}

// --------------------------------------------------------------------------------------
void CGameMain::ClearPathMove()
{
	if (m_pathPoints.Count() > 0)
	{
		SendWarningMsg(g_ObjectData->GetString("SYS_CLEAR_PATH_MOVE"));
	}
	m_pathPoints.Clear();
}

// --------------------------------------------------------------------------------------
// ª±®aª«¥ó²¾°Ê³QÂê¦íµLªk²¾°Ê
bool CGameMain::IsPlayerMoveLocked()
{
	CRoleSprite* player = m_pPlayer;

	if (player == NULL)
		return true;

	if (m_botLocked)
		return true;

	if (m_isFreeCamera)
		return true;

	if (m_cameraControl)
		return true;

	if (m_cameraFollowSprite != NULL)
		return true;

	bool isMoveControl = player->GetMoveControl();

	// ¯S®íª¬ºA¤£¯à²¾°Ê
	if (isMoveControl == false || player->IsProxyLoadComplete() == false)
		return true;

	return false;
}

// --------------------------------------------------------------------------------------
void CGameMain::SetPVEState(bool isPVE)
{
	if (m_isPVE != isPVE)
	{
		//if ( strcmp(m_country.c_str(), "CN") == 0 )
		{
			if (isPVE)
			{
				SendSystemMsgEx("%s%s%s", "|cff00ff00", g_ObjectData->GetString("SYS_PK_TYPE1"), "|r");
			}
			else
			{
				SendSystemMsgEx("%s%s%s", "|cffff0000", g_ObjectData->GetString("SYS_PK_TYPE2"), "|r");
			}
		}

		m_isPVE = isPVE;
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::ComparePlayerName()
{
	static bool firstSendName = false;
	if (firstSendName == true)
		return;

	for (map<int, CRoleSprite*>::iterator iter = m_spriteContainer.begin(); iter != m_spriteContainer.end(); iter++)
	{
		if (iter->second->GetObjectType() != eSpriteObject_Player)
			continue;

		for (map<int, CRoleSprite*>::iterator iterA = m_spriteContainer.begin(); iterA != m_spriteContainer.end(); iterA++)
		{
			if (iterA->second->GetObjectType() != eSpriteObject_Player || iterA->second == iter->second)
				continue;

			if (strcmp(iterA->second->GetName(), iter->second->GetName()) == 0)
			{
				NetCli_Other::S_RoleNameDuplicateErr(iter->second->GetDBID(), iterA->second->GetDBID(), iter->second->GetName());
				firstSendName = true;
			}
		}
	}
}

// --------------------------------------------------------------------------------------
int CGameMain::GetBattleGroundType()
{
	int Result = 0;

	if (m_isBattleGround)
	{
		Result = m_zoneID;
	}

	return Result;
}

// --------------------------------------------------------------------------------------
bool CGameMain::IsEnableBattleGroup()
{
	if (m_isEnableBGParty)
		return true;

	if (m_isBattleGround == false || m_isDisableParty)
		return false;

	return true;
}

// --------------------------------------------------------------------------------------
void CGameMain::ClearAllBattleMember()
{
	for (map<int, CRoleSprite*>::iterator iter = m_BattleMember.begin(); iter != m_BattleMember.end(); iter++)
	{
		if (iter->second == NULL)
			continue;

		delete iter->second;
		iter->second = NULL;
	}

	m_BattleMember.clear();
}

// --------------------------------------------------------------------------------------
bool CGameMain::AddBattleMember(int GItemID, CRoleSprite* sprite)
{
	map<int, CRoleSprite*>::iterator iter = m_BattleMember.find(GItemID);
	if (iter != m_BattleMember.end())
		return false;

	m_BattleMember.insert(make_pair(GItemID, sprite));
	return true;
}

// --------------------------------------------------------------------------------------
bool CGameMain::DeleteBettleMember(int GItemID)
{
	map<int, CRoleSprite*>::iterator iter = m_BattleMember.find(GItemID);
	if (iter == m_BattleMember.end())
		return false;

	if (iter->second != NULL) delete iter->second;

	m_BattleMember.erase(iter);
	return true;
}

// --------------------------------------------------------------------------------------
CRoleSprite* CGameMain::FindBettleMemberByGItemID(int GItemID)
{
	map<int, CRoleSprite*>::iterator iter = m_BattleMember.find(GItemID);
	if (iter != m_BattleMember.end())
		return iter->second;

	return NULL;
}

// --------------------------------------------------------------------------------------
CRoleSprite* CGameMain::FindBettleMemberByDBID(int DBID)
{
	for (map<int, CRoleSprite*>::iterator iter = m_BattleMember.begin(); iter != m_BattleMember.end(); iter++)
	{
		if (iter->second == NULL)
			continue;

		if (iter->second->GetDBID() == DBID)
			return iter->second;
	}
	return NULL;
}

// --------------------------------------------------------------------------------------
CRoleSprite* CGameMain::FindBettleMemberByWorldID(int WorldID, const char* playerName)
{
	if (playerName == NULL)
		return NULL;

	for (map<int, CRoleSprite*>::iterator iter = m_BattleMember.begin(); iter != m_BattleMember.end(); iter++)
	{
		if (iter->second == NULL || iter->second->GetWorldID() != WorldID)
			continue;

		if (strcmp(iter->second->GetName(), playerName) == 0)
			return iter->second;
	}
	return NULL;
}

// --------------------------------------------------------------------------------------
CRoleSprite* CGameMain::FindBattleMemberByName(const char* playerName)
{
	// ¨ú±o¦øªA¾¹¥N¸¹
	int worldID = g_pAccountLogin->GetWorldID();

	// ¨ú±o¥»¦a¦WºÙ
	const char* localName = playerName;

	// ª±®a¦WºÙ·|¥H'-'¤À¹j Ex Servername-PlayerName
	const char* separate = strchr(playerName, '-');
	if (separate != NULL)
	{
		char serverName[512];

		localName = separate + 1;

		int size = int(separate - playerName);
		memcpy(serverName, playerName, size);
		serverName[size] = 0;

		// ¥Î¦WºÙ¨ú¦^¦øªA¾¹¥N½X
		int id = g_pAccountLogin->FindWorldIDByName(serverName);
		if (id != -1)
			worldID = id;
	}

	return FindBettleMemberByWorldID(worldID, localName);
}

// --------------------------------------------------------------------------------------
void CGameMain::AddAttachObj(int source, int target, int mode, const char* sourceLink, const char* targetLink)
{
	AttachObjectInfo info;
	info.sourceGItemID = source;
	info.targetGItemID = target;
	info.AttachType = mode;
	info.sourceLinePoint = sourceLink;
	info.targetLinePoint = targetLink;

	m_attachObject.push_back(info);
}

// --------------------------------------------------------------------------------------
void CGameMain::DeleteAttachObj(int GItemID)
{
	vector<AttachObjectInfo>::iterator iter = m_attachObject.begin();
	while (iter != m_attachObject.end())
	{
		if (iter->sourceGItemID == GItemID)
			iter = m_attachObject.erase(iter);
		else
			iter++;
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::ClearAttachObj()
{
	m_attachObject.clear();
}

// --------------------------------------------------------------------------------------
void CGameMain::UpdateAttachObj()
{
	vector<AttachObjectInfo>::iterator iter = m_attachObject.begin();
	while (iter != m_attachObject.end())
	{
		CRoleSprite* source = FindSpriteByID(iter->sourceGItemID);
		CRoleSprite* target = FindSpriteByID(iter->targetGItemID);
		vector<AttachObjectInfo>::iterator iterNext = iter + 1;

		switch (iter->AttachType)
		{
		case eMoveAttach_Implement:
			if (source && target)
			{
				source->AttachImplement(target, eAttach_Down, ruFUSION_MIME_RIDE_IDLE, atoi(iter->targetLinePoint.c_str()));
				iterNext = m_attachObject.erase(iter);
			}
			break;

		case eMoveAttach_Mount:
			if (source && target && target->GetMountSprite())
			{
				source->AttachMountSprite(target, iter->targetLinePoint.c_str());
				iterNext = m_attachObject.erase(iter);
			}
			break;

		case eMoveAttach_Point:
			if (source && target)
			{
				source->SetAttachParent(target, IBaseSprite::GetAttachPointEnum(iter->targetLinePoint.c_str()), false);
			}
			break;

		case eMoveAttach_Point2:
			if (source && target)
			{
				source->SetAttachParent(target, IBaseSprite::GetAttachPointEnum(iter->targetLinePoint.c_str()), true);
			}
			break;

		default:
			if (source && target && source->IsProxyLoadComplete() && target->IsProxyLoadComplete())
			{
				source->FollowSprite(target, iter->targetLinePoint.c_str(), iter->sourceLinePoint.c_str());
				iterNext = m_attachObject.erase(iter);
			}
		}

		iter = iterNext;
	}
}

// --------------------------------------------------------------------------------------
// ¨ú±o«D¤ìÀY¤Hª±®a¨¤¦â¼Æ¶q
int CGameMain::GetPlayerSpriteCount()
{
	int count = 0;
	for (map<int, CRoleSprite*>::iterator iter = m_spriteContainer.begin(); iter != m_spriteContainer.end(); iter++)
	{
		CRoleSprite* sprite = iter->second;
		if (sprite->GetObjectType() == eSpriteObject_Player && sprite->GetObjectId() != DUMMY_ID)
		{
			count++;
		}
	}

	return count;
}

// --------------------------------------------------------------------------------------
void CGameMain::UpdateDummySprite()
{
	CRoleSprite* player = m_pPlayer;
	if (player == NULL)
		return;

	if (m_frameTime - m_lastDummySpriteUpdateTime > 20)
	{
		m_lastDummySpriteUpdateTime = m_frameTime;

		// ¨ú±o¤@¦¸°O¾ÐÅé¨Ï¥Î¶q
		CheckMemoryStatus(true);

		bool inStreeZone = InStressZone();

		int count = GetPlayerSpriteCount();
		if (count < REPLACE_PLAYER_NUM || (!inStreeZone && count < REPLACE_MAX_PLAYER_NUM))
		{
			multimap<REAL, CRoleSprite*> dummySprite;
			int size = 0;

			// ¨Ì¶ZÂ÷±Æ§Ç©Ò¦³¤ìÀY¤Hª±®a
			for (map<int, CRoleSprite*>::iterator iter = m_spriteContainer.begin(); iter != m_spriteContainer.end(); iter++)
			{
				CRoleSprite* sprite = iter->second;
				if (sprite->GetObjectType() == eSpriteObject_Player && sprite->GetObjectId() == DUMMY_ID)
				{
					REAL distance = (player->GetPos() - sprite->GetPos()).Magnitude();
					dummySprite.insert(make_pair(distance, sprite));
				}
			}

			// ¤£¬O¦b¶}¾Á¼@¸¨¨C¦¸¦^´_ REPLACE_PLAYER_NUM ¤H¦¸¬°¥¿±`¨¤¦â
			if (!inStreeZone)
			{
				size = max(REPLACE_MAX_PLAYER_NUM - count, 0);
			}
			else
			{
				size = max(REPLACE_PLAYER_NUM - count, 0);
			}

			multimap<REAL, CRoleSprite*>::iterator iter = dummySprite.begin();
			for (int i = 0; i < size && iter != dummySprite.end(); i++)
			{
				CRoleSprite* sprite = iter->second;
				CRuVector3 position = sprite->GetPos();

#ifndef NOCHECKMEM

				// °O¾ÐÅé¥Î¶q
				if (m_memoryLoad > 96 || m_currentMemory > REPLACE_MEMORY_SIZE)
					break;

#endif //NOCHECKMEM

				// int mountId = sprite->GetOriginalMountId();

				// ­«·s³]©wª«¥ó¸ê®Æ
				sprite->SetObjectId(sprite->GetOriginalObjectId(), true);

				// ­«³]¥~«¬
				sprite->SetLock(sprite->GetLock());

				// ³]©w­«¤O
				sprite->SetGravityMode(true);

				// ³]©w§¤¼Ð
				sprite->SetPos(position);

				// ¦WºÙ§ó·s
				sprite->RefreshWidget();

				// §ó·s§¤ÃM
				sprite->ResetMount();

				// ¬O§_¦³§¤ÃM
				/*
				if ( mountId != 0 )
				{
					sprite->CreateMountSprite(mountId);
					sprite->SetAboveWaterState(sprite->GetAboveWaterState());
				}
				*/

				// Åã¥Ü¸Ë³Æ¸ê®Æ
				PlayerEquipmentStruct* equipInfo = (PlayerEquipmentStruct*)sprite->GetEquipmentInfo();
				if (equipInfo)
				{
					NetCli_MoveChild::SetEquipmentInfo(sprite, equipInfo->EQLook, equipInfo->ShowEQ);

					sprite->ReleaseEquipmentInfo();
				}

				iter = dummySprite.erase(iter);
			}
		}
	}
}

// --------------------------------------------------------------------------------------
bool CGameMain::InStressZone()
{
	CRuWorld_ZoneDescriptor* zoneDescriptor = m_pRuWorldEnvironment->GetCurrentZoneDescriptor();
	if (zoneDescriptor)
	{
		const char* name = zoneDescriptor->GetName();
		if (strcmp(name, "Hintervale") == 0)
			return true;
		else if (strcmp(name, "Campaign Garrison") == 0)
			return true;
	}
	return false;
}

// --------------------------------------------------------------------------------------
void CGameMain::SetTombInfo(int zoneId, CRuVector3 position, float leftTime, int debtExp, int debtTp, int exp)
{
	ClearTomb();

	m_tombPosition = position;
	m_tombLeftTime = leftTime;
	m_tombZoneId = zoneId;
	m_tombDebtExp = debtExp;
	m_tombDebtTp = debtTp;
	m_tombExp = exp;

	g_pLogList->AddMessage("[MapIcon TOMB] ZoneID:%d Position:(%.2f, %.2f, %.2f)", zoneId, position.m_x, position.m_y, position.m_z);

	g_pWorldMapFrame->AddMapIcon(TOMB_MAP_ICON_ID, zoneId, position.m_x, position.m_z, g_ObjectData->GetString("SYS_YOUR_TOMB"), EM_MapIconType_Tomb, 1.0f, 1.0f, 1.0f);
}

// --------------------------------------------------------------------------------------
void CGameMain::ClearTomb()
{
	m_tombPosition.m_x = 0.0f;
	m_tombPosition.m_y = 0.0f;
	m_tombPosition.m_z = 0.0f;
	m_tombLeftTime = 0.0f;
	m_tombZoneId = 0;
	m_tombDebtExp = 0;
	m_tombDebtTp = 0;
	m_tombExp = 0;

	g_pLogList->AddMessage("[MapIcon TOMB] Clear Tomb");

	g_pWorldMapFrame->ClearMapIcon(TOMB_MAP_ICON_ID);
}

// --------------------------------------------------------------------------------------
void CGameMain::UpdateTomb()
{
	/*
	if ( m_tombLeftTime > 0.0001f )
	{
		m_tombLeftTime -= m_elapsedTime;
		if ( m_tombLeftTime < 0.0001f )
			ClearTomb();
	}
	*/
}

// --------------------------------------------------------------------------------------
void CGameMain::CreatePet(int petType, int petGUID, int petItemID, int petMagicID)
{
	if (petType != EM_SummonPetType_Normal)
		return;

	bool isNew = true;			// ¬O§_²Ä¤@¦¸¥[¤J
	map<int, PetInfo>::iterator iter = m_petInfo.find(petType);

	if (iter != m_petInfo.end())
	{
		// Ãþ«¬¬Û¦P,­«·s©Û´«Ãdª«,©ÎªÌÃdª«´«°Ï§ïÅÜ¸ê®Æ

		// ¬Û®e¬É­±
		// g_pInterface->SendWorldEvent("PET_ACTIONBAR_HIDE");

		isNew = false;
	}

	if (m_petGUID != 0)
	{
		g_pInterface->SendWorldEvent("PET_ACTIONBAR_HIDE");
	}

	m_petInfo[petType].GUID = petGUID;
	m_petInfo[petType].ItemID = petItemID;
	m_petInfo[petType].MagicID = petMagicID;

	m_petGUID = petGUID;
	m_petItemID = petItemID;
	m_petMagicID = petMagicID;

	// ³]©wServerª¬ºA»PClient¬Û¦P
	{
		CRoleSprite* target = GetPlayerTarget();
		int targetID = (target) ? target->GetIdentify() : 0;

		// ¨ú±o¨¤¦â³Ì«á³]©w­È
		PetCommandStruct	command;
		command._value = CNetGlobal::RoleData()->PlayerBaseData->ClientData[254];

		// ²¾°Ê¸òÀH
		if (command.movable)
			NetCli_Magic::PetCommand(m_petGUID, EM_PET_STOP, targetID, 0);
		else
			NetCli_Magic::PetCommand(m_petGUID, EM_PET_FOLLOW, targetID, 0);

		// ªðÀ»
		if (command.strikeBack)
			NetCli_Magic::PetCommand(m_petGUID, EM_PET_STRICKBACK_OFF, targetID, 0);
		else
			NetCli_Magic::PetCommand(m_petGUID, EM_PET_STRICKBACK, targetID, 0);

		m_petInfo[petType].Status = command;
	}

	g_pActionFrame->UpdatePetSkill(m_petMagicID);
	g_pInterface->SendWorldEvent("PET_ACTIONBAR_SHOW");
}

// --------------------------------------------------------------------------------------
void CGameMain::DeletePet(int petType, int petGUID)
{
	if (petType != EM_SummonPetType_Normal)
		return;

	if (m_petGUID == petGUID)
	{
		m_petGUID = 0;
		m_petItemID = 0;
		m_petMagicID = 0;

		g_pInterface->SendWorldEvent("PET_ACTIONBAR_HIDE");
	}

	map<int, PetInfo>::iterator iter = m_petInfo.find(petType);
	if (iter != m_petInfo.end())
	{
		if (m_petInfo[petType].GUID == petGUID)
		{
			m_petInfo.erase(iter);

			g_pInterface->SendWorldEvent("PET_ACTIONBAR_HIDE");
		}
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::UpdatePetSkill()
{
	GameObjDbStructEx* obj = CNetGlobal::GetObj(m_petMagicID);

	if (obj == NULL ||
		obj->IsMagicBase() == false ||
		obj->MagicBase.MagicFunc != EM_MAGICFUN_SummonCreature ||
		obj->MagicBase.SummonCreature.Type != EM_MagicSummonCreatureType_Pet)
		return;

	vector<int> newSkill;
	/*
	newSkill.push_back(EM_PET_STOP);
	newSkill.push_back(EM_PET_FOLLOW);
	newSkill.push_back(EM_PET_ATTACK);
	newSkill.push_back(EM_PET_ATTACKOFF);
	newSkill.push_back(EM_PET_PROTECT);
	newSkill.push_back(EM_PET_GUARD);
	*/
	/*
		for( int i = 0 ; i < _MAX_PET_SKILL_COUNT_ ; i++ )
		{

			PetSkillStruct& skill = obj->MagicBase.SummonCreature.Skill[i];

			// §Þ¯à½s¸¹¤£¦s¦b,©ÎªÌ¨Ã«Dª±®a¥i¥H¨Ï¥Î
			if ( skill.SkillID == 0 || skill.SpellRightTime != EM_PetSpellRightTime_OwnerStartUsing )
				continue;

			// ÀË¬d­«­nª««~¬O§_¦s¦b
			if ( skill.KeyItemID == 0 || CNetGlobal::RoleData()->CheckKeyItem( skill.KeyItemID ) == true )
			{
				newSkill.push_back(skill.SkillID);
			}
		}

		if ( m_petSkill != newSkill )
		{
			m_petSkill = newSkill;
			g_pInterface->SendWorldEvent("PET_ACTIONBAR_UPDATE");
		}
		*/
}

// --------------------------------------------------------------------------------------
void CGameMain::SetPetMovable(bool enable)
{
	m_petStatus.movable = !enable;
	CNetGlobal::RoleData()->PlayerBaseData->ClientData[254] = (char)m_petStatus._value;
	if (g_pInterface)
		g_pInterface->SendWorldEvent("PET_ACTIONBAR_UPDATE");
}

// --------------------------------------------------------------------------------------
void CGameMain::SetPetStrikeBackEnable(bool enable)
{
	m_petStatus.strikeBack = !enable;
	CNetGlobal::RoleData()->PlayerBaseData->ClientData[254] = (char)m_petStatus._value;
	if (g_pInterface)
		g_pInterface->SendWorldEvent("PET_ACTIONBAR_UPDATE");
}

// --------------------------------------------------------------------------------------
void CGameMain::UpdatePetSprite(CRoleSprite* player)
{
	if (player == NULL)
		return;

	for (map<int, CRoleSprite*>::iterator iter = m_spriteContainer.begin(); iter != m_spriteContainer.end(); iter++)
	{
		CRoleSprite* sprite = iter->second;
		if (sprite->GetMasterID() == player->GetIdentify() && sprite->GetMasterSprite() == NULL)
		{
			sprite->SetMasterSprite(player, sprite->IsPlayerPet());
		}
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::SetCycleSpell(int id, int index)
{
	if (m_cycleMagicId != id)
	{
		int oldCycleMagicId = m_cycleMagicId;

		// ²M°£³sÄò¬Iªk
		m_cycleMagicId = id;
		m_cycleMagicPos = index;

		// ³]©w´`Àô¬Iªk
		NetCli_Other::SL_SetCycleMagic(m_cycleMagicId, m_cycleMagicPos);

		g_pActionFrame->UpdateSkillAction(oldCycleMagicId);
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::ClearCycleSpell()
{
	SetCycleSpell(0, 0);
}

// --------------------------------------------------------------------------------------
void CGameMain::AddPartitionPosition(CRoleSprite* sprite, CRuVector3 position)
{
	map<CRoleSprite*, CRuVector3>::iterator iter = m_partitionPosition.find(sprite);
	if (iter != m_partitionPosition.end())
		iter->second = position;
	else
		m_partitionPosition.insert(make_pair(sprite, position));
}

// --------------------------------------------------------------------------------------
CRuVector3* CGameMain::FindPartitionPosition(CRoleSprite* sprite)
{
	map<CRoleSprite*, CRuVector3>::iterator iter = m_partitionPosition.find(sprite);
	if (iter != m_partitionPosition.end())
		return &(iter->second);
	return NULL;
}

// --------------------------------------------------------------------------------------
int	 CGameMain::GetPetGUID()
{
	CRoleSprite* petSprite = (m_pPlayer) ? m_pPlayer->GetPetSprite() : NULL;
	if (petSprite)
		return petSprite->GetIdentify();
	return m_petGUID;
}

// --------------------------------------------------------------------------------------
void CGameMain::AddEliteBell(int GItemID)
{
	map<int, float>::iterator iter = m_eliteBell.find(GItemID);
	if (iter != m_eliteBell.end())
		return;

	CRoleSprite* player = GetPlayer();
	CRoleSprite* sprite = FindSpriteByID(GItemID);
	if (sprite)
	{
		// ÀY¤W¥[¤J¯S®Ä
		CEffectSprite* effect = new CEffectSprite;
		if (effect->LoadRuEntity("model\\item\\ui_object\\boss_warning\\act_boss_warning_c_top.ros"))
		{
			effect->AttachSprite(player, eAttach_Top);
			effect->SetEffectMotionStep(eEffectMotion_Step_Explode);
		}
		else
			delete effect;

		// Åã¥ÜÄµ§i°T®§
		SendSystemMsgEx(g_ObjectData->GetString("SYS_ELITE_BELL"), sprite->GetName());

		if (m_interface)
			m_interface->SendWorldEvent("ELITE_BOSS_BELL");
	}

	m_eliteBell.insert(make_pair(GItemID, 10.0f));
}

// --------------------------------------------------------------------------------------
void CGameMain::UpdateEliteBell()
{
	map<int, float>::iterator iter = m_eliteBell.begin();
	while (iter != m_eliteBell.end())
	{
		iter->second -= m_elapsedTime;
		if (iter->second < 0.0f)
		{
			iter = m_eliteBell.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}

// ª««~«áÄò°õ¦æ¤º®e
// --------------------------------------------------------------------------------------
int	CGameMain::PushItemCommand(ItemCommand command)
{
	m_lastItemCommandID++;
	if (m_lastItemCommandID < 1)
		m_lastItemCommandID = 1;

	command.startTime = (float)m_frameTime;
	m_itemCommand.insert(make_pair(m_lastItemCommandID, command));
	return m_lastItemCommandID;
}

// --------------------------------------------------------------------------------------
void CGameMain::ExcuteItemCommand(int id)
{
	map<int, ItemCommand>::iterator iter = m_itemCommand.find(id);
	if (iter != m_itemCommand.end())
	{
		switch (iter->second.type)
		{
		case ITEM_COMMAND_USE_BAG:
		{
			ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(iter->second.index);
			if (item && item->OrgObjID == iter->second.objiectID)
			{
				if (item->CreateTime == iter->second.createTime && item->Serial == iter->second.serial)
				{
					g_pBagFrame->Use(iter->second.index, iter->second.fromBag);
				}
			}
		}
		break;

		case ITEM_COMMAND_BAG_TO_BANK:
			break;
		}

		m_itemCommand.erase(iter);
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::RemoveItemCommand(int id)
{
	map<int, ItemCommand>::iterator iter = m_itemCommand.find(id);
	if (iter != m_itemCommand.end())
		m_itemCommand.erase(iter);
}

// --------------------------------------------------------------------------------------
// ¨ú±o¯S®í¦r¦ê
string CGameMain::GetSpecialEventString(int id)
{
	return "";
}

/*
// --------------------------------------------------------------------------------------
void CGameMain::SetZoneModeInfo(ZoneInfoModeStruct& info)
{
	m_zoneMode.isPVE				= info.IsPvE;
	m_zoneMode.isBattleGround		= info.IsBattleGround;
	m_zoneMode.isDiablePVPRule		= info.IsDisablePVPRule;
	m_zoneMode.isDiableTrade		= info.IsDisableTrade;
	m_zoneMode.isDisableDual		= info.IsDisableDual;
	m_zoneMode.isDisableSendGift	= info.IsDisableSendGift;
	m_zoneMode.isEnableBGParty		= info.IsZonePartyEnabled;
	m_zoneMode.isDisableParty		= info.IsDisableParty;
}
*/

// --------------------------------------------------------------------------------------
string CGameMain::GetDefaultFontPath()
{
	char path[512];

	if (m_language.compare("VN") == 0 ||
		m_language.compare("KR") == 0 ||
		m_language.compare("JP") == 0 ||
		m_language.compare("ID") == 0 ||
		m_language.compare("RU") == 0 ||
		m_language.compare("PL") == 0 ||
		m_language.compare("TH") == 0)
	{
		sprintf_s(path, 512, "fonts\\DFHEIMDU_%s.TTF", m_language.c_str());
	}
	else
	{
		strcpy(path, DEFAULT_FONT_FILENAME);
	}
	return path;
}

// --------------------------------------------------------------------------------------
int	CGameMain::GetItemMallSessionID()
{
	int Result = 0;

	std::map<int, int>::iterator it = NetCli_Other::m_SessionID.find(EM_SessionType_OpenItemMall);

	if (it != NetCli_Other::m_SessionID.end())
	{
		Result = it->second;
	}

	return Result;
}

// --------------------------------------------------------------------------------------
int	CGameMain::GetZShopSessionID()
{
	int Result = 0;

	std::map<int, int>::iterator it = NetCli_Other::m_SessionID.find(EM_SessionType_OpenZShop);

	if (it != NetCli_Other::m_SessionID.end())
	{
		Result = it->second;
	}

	return Result;
}

// --------------------------------------------------------------------------------------
int	CGameMain::GetPlayerCount()
{
	int count = 0;
	for (map<int, CRoleSprite*>::iterator iter = m_spriteContainer.begin(); iter != m_spriteContainer.end(); iter++)
	{
		if (iter->second->GetObjectType() == eSpriteObject_Player)
			count++;
	}
	return count;
}

// --------------------------------------------------------------------------------------
bool CGameMain::IsGameForgeCountries()
{
	bool Result = false;

	return Result;
}

// --------------------------------------------------------------------------------------
void CGameMain::UpdateWorld()
{
	CUi* ui = m_interface->GetUiWorld();
	lua_State* L = m_interface->GetLuaState();

	// Ping Game Server
	{
		static double	lastTime = 0.0f;
		double			fTimeProcess = static_cast<double>(m_frameTime) - lastTime;
		if (fTimeProcess > DF_TIME_PING)
		{
			lastTime = static_cast<double>(m_frameTime);
			Net_Move::S_Ping(GetTickCount());
			m_ping = Net_Move::GetRespondTime();
		}
	}

	//30¬í¬ö¿ýFPS¡BCPU¡BMEM¡A¨Ñ¨C10¤ÀÄÁµo°eµ¹DataCenter(¸g¥ÑLocal)
	if ((m_frameTime - m_FpsLogTime) > 30.0f)
	{
		m_FpsLogTime = m_frameTime;
		//fps
		float fps = GetFramerate();
		m_Fpslog.push_back(fps);
		if (m_Fpslog.size() > 10)
		{
			m_Fpslog.erase(m_Fpslog.begin());
		}
		//cpuusage
		float cpuusage = (float)GetCpuUsage();
		//g_pLogList->AddMessage( "cpuusage : %.0f%%",cpuusage);
		m_Cpulog.push_back(cpuusage);
		if (m_Cpulog.size() > 10)
		{
			m_Cpulog.erase(m_Cpulog.begin());
		}
		//memusage
		float memUsage = (float)GetCurrentMemory();
		m_Memlog.push_back(memUsage / 1024);//§ïmb
		if (m_Memlog.size() > 10)
		{
			m_Memlog.erase(m_Memlog.begin());
		}
		//g_pLogList->AddMessage( "FPS : (%.2f) , Cpu : (%.0f%%) , Mem : (%.0fKB) , MAC Address: %s", fps , cpuusage , memUsage/1024 , GetMac());
	}
	//ÀË¬dping­È¬O§_¶W¹L1000¡A¶W¹L1000¶i¦æTracert¨Ã¶Ç°eµ¹Server°O¿ýLog
	//ping
	/*int fPing = GetPing();
	if (fPing > 1000)
	{
		TracertResultList result = MyTracert(GetIP());
		TracertList pg;
		pg.count = result.count;
		for (int i =0; i<result.count; i++)
		{
			pg.Tracert[i].round_time = result.Tracert[i].round_time;
			pg.Tracert[i].ip_address = result.Tracert[i].ip_address.s_addr;
		}
		NetCli_Other::S_SendTracertList(GetDBID(), fPing, pg);
	}*/


	//¨C10¤ÀÄÁµo°e®Ä¯à²Î­pµ¹DataCenter(¸g¥ÑLocal)
	if ((m_frameTime - m_SendPerformanceTime) > 10 * 60.0f)
		//if ( (m_frameTime - m_SendPerformanceTime) > 30.0f )
	{
		m_SendPerformanceTime = m_frameTime;
		float MaxCPUUsage = 0;
		float MaxMemUsage = 0;
		float MaxFPS = 0;
		float AverageCPUUsage = 0;
		float AverageMemUsage = 0;
		float AverageFPS = 0;
		float MinCPUUsage = 0;
		float MinMemUsage = 0;
		float MinFPS = 0;
		float sum;
		vector<float> TempVector;
		//CPUlog
		TempVector.clear();
		TempVector = m_Cpulog;
		//memcpy(&TempVector[0], &m_Cpulog[0], m_Cpulog.size() * sizeof(float));
		sum = 0;
		for (int i = 0; i < TempVector.size(); i++)
		{
			if (TempVector[i] > MaxCPUUsage)
			{
				MaxCPUUsage = TempVector[i];
			}
			if (i == 0)
			{
				MinCPUUsage = TempVector[i];
			}
			else if (TempVector[i] < MinCPUUsage)
			{
				MinCPUUsage = TempVector[i];
			}
			sum += TempVector[i];
		}
		AverageCPUUsage = sum / TempVector.size();
		//MEMlog
		TempVector.clear();
		TempVector = m_Memlog;
		//memcpy(&TempVector[0], &m_Memlog[0], m_Memlog.size() * sizeof(float));
		sum = 0;
		for (int i = 0; i < TempVector.size(); i++)
		{
			if (TempVector[i] > MaxMemUsage)
			{
				MaxMemUsage = TempVector[i];
			}
			if (i == 0)
			{
				MinMemUsage = TempVector[i];
			}
			else if (TempVector[i] < MinMemUsage)
			{
				MinMemUsage = TempVector[i];
			}
			sum += TempVector[i];
		}
		AverageMemUsage = sum / TempVector.size();
		//FPSlog
		TempVector.clear();
		TempVector = m_Fpslog;
		//memcpy(&TempVector[0], &m_Fpslog[0], m_Fpslog.size() * sizeof(float));
		sum = 0;
		for (int i = 0; i < TempVector.size(); i++)
		{
			if (TempVector[i] > MaxFPS)
			{
				MaxFPS = TempVector[i];
			}
			if (i == 0)
			{
				MinFPS = TempVector[i];
			}
			else if (TempVector[i] < MinFPS)
			{
				MinFPS = TempVector[i];
			}
			sum += TempVector[i];
		}
		AverageFPS = sum / TempVector.size();
		//ping
		float ping = (float)GetPing();
		//Pos
		CRoleSprite* player = GetPlayer();
		//µo°eµ¹Local
		if (player != NULL)
		{
			g_pLogList->AddMessage("Send to Server MAC Address: %s,/r/nMaxCpu : (%.0f%%), MaxMem : (%.0fMB), MaxFPS : (%.2f),/r/nAverageCpu : (%.0f%%), AverageMem : (%.0fMB), AverageFPS : (%.2f),/r/nMinCpu : (%.0f%%), MinMem : (%.0fMB), MinFPS : (%.2f), Ping : (%d), Postion : (%.2f,%.2f,%.2f) ", GetMac(), MaxCPUUsage, MaxMemUsage / 1024 / 1024, MaxFPS, AverageCPUUsage, AverageMemUsage / 1024 / 1024, AverageFPS, MinCPUUsage, MinMemUsage / 1024 / 1024, MinFPS, ping, player->GetPos().m_x, player->GetPos().m_y, player->GetPos().m_z);
			NetCli_Other::S_SendPerformance(GetMac(), MaxCPUUsage, MaxMemUsage, MaxFPS, AverageCPUUsage, AverageMemUsage, AverageFPS, MinCPUUsage, MinMemUsage, MinFPS, ping, player->GetPos().m_x, player->GetPos().m_y, player->GetPos().m_z);
		}
	}

	// VIP
	// Åã¥ÜVIP°T®§
	if (m_vipValid && !m_vipDisplay)
	{
		m_vipDisplay = true;
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_VIP"));
	}

	int vipTime = TimeExchangeFloatToInt(CNetGlobal::RoleData()->PlayerBaseData->VipMember.Term);
	if (vipTime - m_serverTimeZone * 60 * 60 > m_gameTime)
	{
		m_vipValid = true;
	}
	else if (m_vipValid)
	{
		m_vipValid = false;
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_VIP_INVALID"));
	}

#ifndef NOCHECKMEM
	// Åã¥Ü°O¾ÐÅéÄµ§i
	if (GetUpdateInfo().checkMemory > 0 && (DWORD)GetUpdateInfo().checkMemory < m_currentMemory && !g_pRuneDev->IsDisableMemoryWarning())
	{
		m_checkMemoryTime -= m_elapsedTime;
		if (m_checkMemoryTime < 0.0 && m_interface) {
			m_interface->SendWorldEvent("WARNING_MEMORY");
			m_checkMemoryTime = 365.0 * 24.0 * 60.0 * 60.0 * 1000.0;
		}
	}
#endif //NOCHECKMEM

	// ­p®É
	if (m_frameTime - m_totalLastTime > 1.0f)
	{
		m_totalLastTime = m_frameTime;
		m_totalFps += m_fps;
		m_totalPing += (float)m_ping;
		m_totalCount++;
	}

	// ¨¾¨I°g´£¥Ü®É¶¡
	int playTimeQuota = CNetGlobal::RoleData()->PlayerBaseData->PlayTimeQuota;
	int playQoutaMinute = (playTimeQuota + 59) / 60;

	string strMsg = "";

	// ¨C¤ÀÄÁ§ó·s¤@¦¸
	if (m_playTimeQuota != playQoutaMinute)
	{

		if (playQoutaMinute == 4 * 60)				// ¹CÀ¸1¤p®É
		{
			strMsg = g_ObjectData->GetString("SYS_PLAYTIME_QOUTA_MSG1");
			//g_pGameMain->SendSystemMsg(strMsg.c_str());
			g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", strMsg.c_str());
			g_pGameMain->SendPlayTimeQuataNotify(strMsg.c_str());

		}
		else if (playQoutaMinute == 3 * 60)			// ¹CÀ¸2¤p®É
		{
			strMsg = g_ObjectData->GetString("SYS_PLAYTIME_QOUTA_MSG2");
			//g_pGameMain->SendSystemMsg(strMsg.c_str());
			g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", strMsg.c_str());
			g_pGameMain->SendPlayTimeQuataNotify(strMsg.c_str());
		}
		else if (playQoutaMinute == 2 * 60)			// ¹CÀ¸3¤p®É
		{
			strMsg = g_ObjectData->GetString("SYS_PLAYTIME_QOUTA_MSG3");
			//g_pGameMain->SendSystemMsg(strMsg.c_str());
			g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", strMsg.c_str());
			g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_PLAYTIME_QOUTA_HALF"));
			g_pGameMain->SendPlayTimeQuataNotify(strMsg.c_str());
		}
		//else if ( playQoutaMinute == 1*60 || (playQoutaMinute >= 30 && playQoutaMinute <= 90 && (playQoutaMinute % 5) == 0) )
		else if (playQoutaMinute == 1 * 60 || playQoutaMinute == 30)
		{
			strMsg = g_ObjectData->GetString("SYS_PLAYTIME_QOUTA_MSG4");
			//g_pGameMain->SendSystemMsg(strMsg.c_str());
			g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", strMsg.c_str());
			g_pGameMain->SendPlayTimeQuataNotify(strMsg.c_str());
		}
		else if (playQoutaMinute == 0)
		{
			strMsg = g_ObjectData->GetString("SYS_PLAYTIME_QOUTA_MSG5");
			//g_pGameMain->SendSystemMsg(strMsg.c_str());
			g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", strMsg.c_str());
			g_pGameMain->SendPlayTimeQuataNotify(strMsg.c_str());
			m_QuotaUpdateTime = 900.0f; // 15 mins per time
		}
		m_playTimeQuota = playQoutaMinute;
	}
	else if (playTimeQuota <= 0)
	{
		m_QuotaUpdateTime -= m_elapsedTime;
		if (m_QuotaUpdateTime < 0.0f)
		{
			m_QuotaUpdateTime = 900.0f; // 15 mins per time
			strMsg = g_ObjectData->GetString("SYS_PLAYTIME_QOUTA_MSG5");
			//g_pGameMain->SendSystemMsg(strMsg.c_str());
			g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", strMsg.c_str());
			g_pGameMain->SendPlayTimeQuataNotify(strMsg.c_str());
		}
	}

	// ­«·s°T°Ý°Æ¥»¶i«×
	if (m_requestInstance)
	{
		RequestInstanceRecord();
	}

	// ¨¾¨I°g®É¶¡¤º²æÂ÷¶¤¥î
	if (playTimeQuota < 2 * 60 * 60)
	{
		// ­Y¦b¶¤¥î¤º­ç¥X¶¤¥î
		if (NetCli_PartyChild::GetPartyState() == ePartyState_Leader || NetCli_PartyChild::GetPartyState() == ePartyState_Member)
		{
			static double lastSendKickMemberTime = 0.0f;
			if (m_frameTime - lastSendKickMemberTime > 30.0f)
			{
				lastSendKickMemberTime = m_frameTime;
				NetCli_PartyChild::KickMember(g_pGameMain->GetPlayerName());
				g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_PLAYTIME_QOUTA_LEAVE_PARTY"));
			}
		}
	}

	// «Ê¥]°I´î x ¬í­pºâ¤@¦¸
	if (m_frameTime - m_nextPacketReduceUpdateTime > (6.0 * (m_packetReduceState + 1)))
	{
		m_nextPacketReduceUpdateTime = m_frameTime;

		switch (m_packetReduceState)
		{
		case PACKET_REDUCE_NONE:
			if (m_packetReducePing > 10000)
				SendPacketReduceMode(PACKET_REDUCE_MOVE_ALL);
			else if (m_packetReducePing > 5000)
				SendPacketReduceMode(PACKET_REDUCE_PLAYER_MOVE_ALL);
			else if (m_packetReducePing > 1500)
				SendPacketReduceMode(PACKET_REDUCE_PLAYER_MOVE);
			break;

		case PACKET_REDUCE_PLAYER_MOVE:
			if (m_packetReducePing > 5000)
				SendPacketReduceMode(PACKET_REDUCE_MOVE_ALL);
			else if (m_packetReducePing > 1500)
				SendPacketReduceMode(PACKET_REDUCE_PLAYER_MOVE_ALL);
			else if (m_packetReducePing < 500)
				SendPacketReduceMode(PACKET_REDUCE_NONE);
			break;

		case PACKET_REDUCE_PLAYER_MOVE_ALL:
			if (m_packetReducePing > 5000)
				SendPacketReduceMode(PACKET_REDUCE_MOVE_ALL);
			else if (m_packetReducePing < 500)
				SendPacketReduceMode(PACKET_REDUCE_PLAYER_MOVE);
			break;

		case PACKET_REDUCE_MOVE_ALL:
			if (m_packetReducePing < 500)
				SendPacketReduceMode(PACKET_REDUCE_PLAYER_MOVE_ALL);
			break;
		}
	}
	else
	{
		m_packetReducePing = (m_packetReducePing + m_ping) / 2;
	}

	// Get Player's sprite pointer
	if (m_pPlayer == NULL)
	{
		CRoleSprite* sprite = FindSpriteByWorldGUID(m_playerID);
		if (sprite) {
			SetPlayer(sprite, sprite->GetPos(), sprite->GetDirection());
		}
	}

	if (m_pPlayer)
	{
		CRoleSprite* player = m_pPlayer;
		CRoleSprite* controlSprite = player->GetProxyControlSprite();
		SpriteSpecialEffect effect = player->GetSpecailEffect();
		bool isMoveControl = !IsPlayerMoveLocked();

		if (player->GetAttachParentId() != 0)
		{
			bool isFollowCamera = GetCameraFollowEnable();

			//SetCameraFollowEnable(true);

			// ÃèÀY¸òÀH¨¤¦â²¾°Ê
			SetCameraForward(controlSprite->GetDirection(), true);

			SetCameraFollowEnable(isFollowCamera);
		}

		// µe­±ÅÜ·t
		CUiTexture* maskTexture = (g_pGameControl) ? g_pGameControl->GetDarkTexture() : NULL;

		if (maskTexture)
		{
			float alpha = maskTexture->GetAlpha();
			if (effect.blind)
			{
				if (maskTexture->IsVisible())
				{
					alpha += m_elapsedTime * 2;
					maskTexture->SetAlpha(alpha);
				}
				else
				{
					maskTexture->SetAlpha(0.0f);
					maskTexture->SetVisible(true);
				}
			}
			else
			{
				if (maskTexture->IsVisible())
				{
					if (alpha > 0.0f)
					{
						alpha -= m_elapsedTime;
						maskTexture->SetAlpha(alpha);
					}
					else
					{
						maskTexture->SetVisible(false);
					}
				}
			}
		}

		// ¦Û°Ê±±¨îÃèÀY
		if (m_isFreeCamera && GetCamera())
		{
			CRuCamera* pCamera = GetCamera();
			CRuVector3 relativeCamPos = m_pCamera->GetTargetPosition() - m_pCamera->GetCameraPosition();

			// ¤@¯ë²¾°Ê³B²z
			float forwardDelta = 0.0f;
			float rightDelta = 0.0f;

			if (m_moveState.forward || m_moveState.autoRun)
				forwardDelta += m_elapsedTime;
			if (m_moveState.backward)
				forwardDelta -= m_elapsedTime;
			if (m_moveState.strafeRight)
			{
				if (effect.inverse == 0)
					rightDelta += m_elapsedTime;
				else
					rightDelta -= m_elapsedTime;
			}
			if (m_moveState.strafeLeft)
			{
				if (effect.inverse == 0)
					rightDelta -= m_elapsedTime;
				else
					rightDelta += m_elapsedTime;
			}

			// ±ÛÂà¤W¤U
			float yDelta = 0.0f;
			if (m_moveState.turnLeft)
				yDelta += m_elapsedTime * 50.0f;
			if (m_moveState.turnRight)
				yDelta -= m_elapsedTime * 50.0f;

			// ÃèÀY²¾°Ê
			if (forwardDelta != 0.0f || rightDelta != 0.0f || yDelta != 0.0f)
			{
				CRuQuaternion quaternion;
				CRuVector3 moveVector;

				moveVector.m_x = rightDelta * 50.0f;
				moveVector.m_z = forwardDelta * 50.0f;

				relativeCamPos.m_y = 0.0f;

				// Âà´«¬°¨¤¦â´Â¦V¥Ø¼Ð¦V¶q
				quaternion.RotateIntoVector(relativeCamPos, CRuVector3(0.0f, 0.0f, 1.0f));
				quaternion.TransformVector(moveVector);
				moveVector.m_y = yDelta;

				pCamera->SetCameraPosition(m_pCamera->GetCameraPosition() + moveVector);
				pCamera->SetTargetPosition(m_pCamera->GetTargetPosition() + moveVector);

				if (m_movePartitionEnable)
				{
					CRuVector3 position = pCamera->GetCameraPosition();
					Net_Move::S_SetPartition(position.m_x, position.m_y, position.m_z);
				}
			}
		}

		// ¤W¸ü¨ã
		if (player->GetAttachParentId() != 0)
		{
			if (m_inMount == false)
			{
				m_inMount = true;

				if (m_interface)
				{
					m_interface->SendWorldEvent("ENTER_MOUNT");
				}
			}

			// ÅÜ¨­ª¬ºA,¦Û°Ê¤U§¤ÃM
			if (player->GetRuTempActor() != NULL)
			{
				Net_Move::S_DetachObjRequest(player->GetIdentify(), player->GetAttachParentId());
			}

			CancelAttack();
		}
		// Â÷¶}¸ü¨ã
		else
		{
			if (m_inMount)
			{
				m_inMount = false;

				if (m_interface)
				{
					m_interface->SendWorldEvent("LEAVE_MOUNT");
				}
			}
		}

		// ¤W¸ü¨ã
		if (player->GetImplementSprite())
		{
			if (m_inImplement == false)
			{
				m_inImplement = true;

				m_cameraDistance *= 1.5f;

				m_delayGetImplementTime = 10.0f;

				GameObjDbStruct* itemDB = g_ObjectData->GetObj(player->GetImplementSprite()->GetObjectId());
				if (itemDB)
				{
					SetTargetEffectMaxRange((float)itemDB->NPC.CarryShootRange);
					SetTargetEffectMaxAngle((float)itemDB->NPC.CarryShootAngle);
				}

				if (m_interface)
				{
					m_interface->SendWorldEvent("IMPLEMENT_ACTIONBAR_SHOW");
				}
			}

			// µ¥«Ý®É¶¡¬d¸ß§Þ¯àÃþ«¬
			if (m_delayGetImplementTime > 0.0f)
			{
				m_delayGetImplementTime -= m_elapsedTime;

				if (GetTargetEffectType() == TargetLineEffect_None)
				{
					for (int i = 0; i < 10; i++)
					{
						ImplementActionItem* item = g_pActionFrame->GetImplementActionItem(i);
						if (item == NULL)
							continue;

						GameObjDbStruct* magicDB = g_ObjectData->GetObj(item->skillId);
						GameObjDbStruct* imageDB = (magicDB) ? g_ObjectData->GetObj(magicDB->ImageID) : NULL;
						if (magicDB && imageDB)
						{
							if (magicDB->MagicCol.TargetType == EM_Releation_Locatoin)
							{
								switch (imageDB->Image.MagicShowType)
								{
								case EM_GameObjectMagicShowType_Single:
								case EM_GameObjectMagicShowType_Multi:
								case EM_GameObjectMagicShowType_PolyLine:
									CreateTargetEffect(TargetLineEffect_Line, player->GetImplementSprite());
									break;

								case EM_GameObjectMagicShowType_Parabola:
								case EM_GameObjectMagicShowType_ParabolaMulti:
									CreateTargetEffect(TargetLineEffect_Parabola, player->GetImplementSprite());
									break;
								}

								m_delayGetImplementTime = 0.0f;
								break;
							}
						}
					}
				}
				else
				{
					m_delayGetImplementTime = 0.0f;
				}
			}
		}
		// Â÷¶}¸ü¨ã
		else
		{
			if (m_inImplement)
			{
				m_inImplement = false;
				m_delayGetImplementTime = 0.0f;

				m_cameraDistance /= 1.5f;

				// ¸É°eª±®a§¤¼ÐÂI
				SendPlayerPos(true);

				ReleaseTargetEffect();

				if (m_interface)
				{
					m_interface->SendWorldEvent("IMPLEMENT_ACTIONBAR_HIDE");
				}

				ClearSelectAreaEntity();
			}
		}

		/*
		// ¨ú±o¨¤¦â²¾°Ê¸ô®|ÂI
		CRuFusion_Actor *pRuActor = controlSprite->GetRuActor();
		if ( pRuActor && m_pRuTerrain )
		{
			CRuArrayList<CRuVector3>& movementPoints = pRuActor->GetSojourner()->GetMovementEndPoints();
			int count = movementPoints.Count();
			if ( count > 0 )
			{
				if ( controlSprite->GetMoveType() == eRoleSpriteMove_Normal )
				{
					CRuCollisionQuery_Segment_HitTest segQuery;
					CRuVector3 pt1, pt2;

					pt1 = m_rightPosition;
					pt1.m_y += 10.0f;
					pt2 = movementPoints.Get(0);
					pt2.m_y += 10.0f;

					// ²Ä¤@ÂI®y¼Ð½T»{,¥i¥H§PÂ_¬O§_¦³­×§ï®y¼Ð­È
					segQuery.m_colSegment = CRuSegment3(pt1, pt2);
					segQuery.m_exclusionMask = 0x00A00000;
					segQuery.m_ignoreBackface = FALSE;

					IBaseSprite::GetCollisionContainer()->QueryCollision(&segQuery);

					// ´ú¸Õ¦³¹J¨ìªýÀÉ
					if (segQuery.m_positiveHit)	{
						SetPlayerPos(m_rightPosition);
					} else {
						m_rightPosition = pt2;

						// ¸I¨ì»ÙÃª±N³Ì«á½T¥i®y¼Ð­È°ï¤J m_obstructPosition, ¦b·j´M¤U¤@­Ó
						for ( int i = 1; i < count; i++ )
						{
							pt1 = m_rightPosition;
							pt1.m_y += 10.0f;
							pt2 = movementPoints.Get(i);
							pt2.m_y += 10.0f;

							segQuery.m_colSegment = CRuSegment3(pt1, pt2);
							segQuery.m_exclusionMask = 0x00A00000;
							segQuery.m_ignoreBackface = FALSE;

							IBaseSprite::GetCollisionContainer()->QueryCollision(&segQuery);

							// ´ú¸Õ¦³¹J¨ìªýÀÉ
							if (segQuery.m_positiveHit)
							{
								// °O¿ý³Ì«á¤@¦¸¥¿½T¦ì¸m
								m_obstructPosition.Add(m_rightPosition);
							}
							m_rightPosition = pt2;
						}
					}
				}
				else
				{
					CRuVector3 position = controlSprite->GetPos();
					if ( m_pRuTerrain )
						position.m_y = RuEntity_FindRestingHeight(position, m_pRuTerrain, 100.0f);
					//g_pError->AddMessage(0, 0, "Anchor Position (%.2f, %.2f, %.2f)", position.m_x, position.m_y, position.m_z);
					m_rightPosition = position;
				}
			}
		}
		*/

		// ÀË´ú¨¤¦â¬O§_¶i¤J¤ô­±(©³),¸Ñ°£BUFF
		bool inWater = controlSprite->IsInWater();
		if (inWater != m_inWater)
		{
			ClientStateStruct environment;
			environment.InWater = inWater;
			m_inWater = inWater;
			NetCli_RoleValue::S_ClientEnvironmentState(environment);

			Dismount();
		}

		// ¦aªí¨¤¦â©w¦ì
		SetPlayerAnchor(controlSprite->GetPos());

		// Casting §P§O¨¤¦â°Ê§@
		if (m_itemCasting.type != ITEM_CASTING_NONE)
		{
			switch (player->GetActorState(1))
			{
			case ruFUSION_ACTORSTATE_CRAFTING_BEGIN:
			case ruFUSION_ACTORSTATE_CRAFTING_LOOP:
			case ruFUSION_ACTORSTATE_CRAFTING_END:
				break;

			default:
				// ¨Ã¤£¦b°¨¤W
				if (player->GetRideState() == false)
				{
					ItemCastingInterrupt();
				}
			}
		}

		// ¥Ø¼Ð¤£¦s¦b©Î¦º¤`
		{
			CRoleSprite* target = player->GetTargetSprite();
			if (target == NULL || target->GetDeadState())
			{
				// ³sÄò¬Iªk
				if (m_cycleMagicId != 0)
					ClearCycleSpell();

				// ¨ú®ø¨¤¦â§ðÀ»°Ê§@
				if (m_attackWorldGUID != 0)
					CancelAttack();
			}

			// ¥Ø¼Ð¤£¥i¨£®ÉÁôÂÃ
			if (target && target->GetPartyState() == false)
			{
				if (target->GetSneakHiddenState())
				{
					SetPlayerTarget(NULL);
				}
			}

			/*
			if ( target && target->CheckRenderState() == false && target->GetPartyState() == false )
			{
				SetPlayerTarget(NULL);
			}
			*/
		}

		// °lÂÜ¶°ÂI¥Ø¼Ð
		{
			for (int i = 0; i < MAX_FOCUS_SPRITES; i++)
			{
				if (m_focusSprites[i] && m_focusSprites[i]->GetPartyState() == false && m_focusSprites[i]->GetSneakHiddenState())
				{
					SetFocusSprite(i, NULL);
				}
			}
		}

		// ¤£¯à²¾°Ê,¨ú®ø¹ï¥Ø¦a²¾°Ê
		if (isMoveControl == false || effect.lockedMove > 0 || effect.dizzy > 0 || effect.sleep > 0 || effect.sleep > 0 || effect.fear > 0)
		{
			ClearPathMove();
			ClearMoveToPoint();
		}

		// ª«¥ó¥¼«Ø¥ß
		if (player->IsProxyLoadComplete() == false)
		{
		}
		// ®£Äß¤¤
		else if (effect.fear > 0 && effect.lockedMove <= 0 && effect.dizzy <= 0 && effect.sleep <= 0)
		{
			float time = m_fear.time - m_elapsedTime;
			if (time < 0.0f)
			{
				m_fear.dir = rand() % 12;
				time = 0.3f * (rand() % 5);

				if (m_fear.dir <= 9 && m_fear.dir != 4)
				{
					CRuVector3 direction;
					direction.m_x = 1.0f - (m_fear.dir % 3);
					direction.m_z = 1.0f - (m_fear.dir / 3);
					direction.m_y = 0.0f;
					controlSprite->SetDirection(direction);
				}
			}
			m_fear.time = time;

			if (m_fear.dir != 4)
			{
				controlSprite->SetMoveVector(CRuVector3(0.0f, 0.0f, max(controlSprite->GetMoveSpeed(), 40.0f)), NEXT_MOVETIME);
			}
		}
		// ÀË¬d¬O§_¦³¯S®í¦æ¬°³y¦¨µLªk²¾°Ê
		else if (isMoveControl && m_botLocked == false)
		{
			CRuVector3 playerPoint = controlSprite->GetPos();

			float speed = controlSprite->GetMoveSpeed();
			if (m_moveState.walk)
				speed /= WALK_SLOW_SCALE;

			// ¸òÀH¹ï¶H
			if (m_follow.target)
			{
				CRuVector3 targetPoint = m_follow.target->GetPos();
				CRuVector3 moveVector = targetPoint - playerPoint;

				float distance, moveTime;
				if (moveVector.m_x > -0.1f && moveVector.m_x < 0.1f && moveVector.m_y > -0.1f && moveVector.m_y < 0.1f && moveVector.m_z > -0.1f && moveVector.m_z < 0.1f)
				{
					distance = 0.0f;
					moveTime = 0.0f;
				}
				else
				{
					distance = moveVector.Magnitude();
					moveTime = distance / speed;
				}

				if (moveTime > 5.0f) {
					// ®É¶¡¤Óªø,¨ú®ø¸òÀH
					FollowSprite(NULL);
				}
				else if (moveTime > 1.0f) {
					controlSprite->SetDirection(targetPoint - playerPoint);
					controlSprite->SetMoveVector(CRuVector3(0.0f, 0.0f, speed), 0.5f);
				}
			}
			else if (m_mouseMove.enable)
			{
				UpdateMouseMove();
			}
			else
			{
				// ¤@¯ë²¾°Ê³B²z
				float forwardDelta = 0.0f;
				float rightDelta = 0.0f;

				if (m_moveState.forward || m_moveState.autoRun)
					forwardDelta += m_elapsedTime;
				if (m_moveState.backward)
					forwardDelta -= m_elapsedTime;
				if (m_moveState.strafeRight)
				{
					if (effect.inverse == 0)
						rightDelta += m_elapsedTime;
					else
						rightDelta -= m_elapsedTime;
				}
				if (m_moveState.strafeLeft)
				{
					if (effect.inverse == 0)
						rightDelta -= m_elapsedTime;
					else
						rightDelta += m_elapsedTime;
				}

				if (forwardDelta != 0.0f || rightDelta != 0.0f)
				{
					//FollowSprite(NULL);		// °±¤î¸òÀH

					controlSprite->SetMoveVector(controlSprite->ComputeMoveVector(forwardDelta, rightDelta), NEXT_MOVETIME);
				}
			}

			/*
			// ¯S§O²¾°Ê¤¤Â_ªk³N
			if ( controlSprite->GetMoveTime() > 0.1f )
				CancelCastSpell();
			*/

			// ±ÛÂàÃèÀY
			float xDelta = 0.0f;
			float yDelta = 0.0f;
			if (m_moveState.turnLeft)
				xDelta += m_elapsedTime * 1.5f;
			if (m_moveState.turnRight)
				xDelta -= m_elapsedTime * 1.5f;
			RotateCamera(xDelta, yDelta, true);
		}
	}

	if (m_pRuWorldEnvironment)
	{
		CRuWorld_ZoneDescriptor* zoneDescriptor = m_pRuWorldEnvironment->GetCurrentZoneDescriptor();
		if (zoneDescriptor)
		{
			const char* name = zoneDescriptor->GetName();
			int ParentZoneID = zoneDescriptor->GetParentZoneID();
			if (m_zoneName.compare(name) != 0)
			{
				// µù¥U¦a¹Ï°Ï°ìÅÜ§óCALLBACK
				for (vector<ZoneDescriptorCallback>::iterator iter = m_ZoneDescriptorList.begin(); iter != m_ZoneDescriptorList.end(); iter++)
				{
					(*iter)(zoneDescriptor);
				}

				char temp[MAX_PATH];
				m_zoneName = name;

				//ZONE0»Ý­nÅã¥Ü©Ð«Î¦WºÙ ¥Ñ©Ð«Î¸ü§¹«á¦Û¤vµo¥X¨Æ¥ó
				if ((GetZoneID() % 1000) == 400)
				{
					m_WorldMapID = GetZoneID() % 1000;
					m_interface->SendWorldEvent(UiEvent_MapDisable);
				}
				else
				{
					sprintf(temp, "ZONE_%s", m_zoneName.c_str());
					strupr(temp);

					name = g_ObjectData->GetString(temp);
					if (strlen(name) <= 0)
						name = temp;

					lua_pushstring(L, name);
					lua_setglobal(L, "arg1");
					//lua_pushstring(L, temp);
					lua_pushnumber(L, GetZoneID());//  ·s¤â±Ð¾Ç­n¥Î
					lua_setglobal(L, "arg2");

					m_interface->SendWorldEvent(UiEvent_ZoneChanged);

					const WorldMapInfoStruct* pWorldMapInfo = g_ObjectData->GetWorldMapInfoByName(m_zoneName.c_str());

					m_zoneName2 = "";
					//©¹¤W¤@¼h§ä
					int ParentZoneID;
					while (pWorldMapInfo == NULL)
					{
						ParentZoneID = zoneDescriptor->GetParentZoneID();
						zoneDescriptor = m_pRuTerrain->GetZoneDescriptorByID(ParentZoneID);

						if (zoneDescriptor == NULL)
							break;

						m_zoneName2 = zoneDescriptor->GetName();
						pWorldMapInfo = g_ObjectData->GetWorldMapInfoByName(m_zoneName2.c_str());
					}
					//	m_zoneName2=name;

						//§ä¤£¨ì
					if (pWorldMapInfo == NULL)
					{
						m_WorldMapID = -1;
						m_interface->SendWorldEvent(UiEvent_MapDisable);
					}
					else
					{
						m_interface->SendWorldEvent(UiEvent_MapEnable);
						m_WorldMapID = pWorldMapInfo->MapID;
						m_interface->SendWorldEvent(UiEvent_MapChanged);
					}

				}
			}
		}
	}

	// ´å¼Ð¿ï¨ìª«¥ó°{Ã{
	CRoleSprite* highlightSprite = m_pMouseInPlayer;

	/*
	if ( highlightSprite )
	{
		switch (highlightSprite->GetCheckedStatus())
		{
		case eChecked_QuestObj:
			{
				if( m_pMouseInPlayer->GetTouchRange() != 0 )
					break;
				else
					// ¦³ÂI¦MÀIªº§PÂ_¤è¦¡,¨C¦¸UPDATE¥­§¡­n§PÂ_5000¦¸
					if ( CNetCli_Script_Child::CheckQuestObj( m_pMouseInPlayer->GetObjectId() ) == -1 )
						highlightSprite = NULL;
			} break;

		case eChecked_Mine:
		case eChecked_Herb:
		case eChecked_Lumber:
			break;
		}
	}
	*/

	// ªð°j¨¤¦âµn¤J¬É­±
	if (m_campState == true)
	{
		m_campUpdateTime += m_elapsedTime;
		if (m_campUpdateTime > 20.0f)
			SetCampState(false);
	}

	// ª««~¼È¦s©R¥O,¶W¹L¤Ó¤[®É¶¡§R°£
	map<int, ItemCommand>::iterator iter = m_itemCommand.begin();
	while (iter != m_itemCommand.end())
	{
		if (m_frameTime - iter->second.startTime > 30.0f)
			m_itemCommand.erase(iter);
		else
			iter++;
	}

	// °Æ¥»¶i«×°T°Ý
	if (m_instanceWaitTime > 0.0f)
	{
		if (m_instanceWaitTime - m_elapsedTime < 0.01f)
		{
			DeclineInstanceRecord();
		}
		else
		{
			m_instanceWaitTime -= m_elapsedTime;
		}
	}

	// ¨ú®ø¥Ø¼Ð
	{
		CRoleSprite* pTarget = GetPlayerTarget();
		if (pTarget && pTarget != m_pPlayer) {
			if (!pTarget->GetCursorState() || !pTarget->GetCursorEnableState() || !pTarget->GetTargetState() || pTarget->IsScriptHide() || !pTarget->IsVisible()) {
				SetPlayerTarget(NULL);
			}
		}
	}

	SetHighlightSprite(highlightSprite);

	// ¼ç§Î³B²z
	UpdateSneakSprite();

	// ¸IÄ²ÀË´ú
	CheckTouchSprite();

	// §ïÅÜ´å¼Ðª¬ºA
	UpdateCursor();

	// ¸ô®|²¾°Ê
	UpdatePathMove();

	// ©w¦ì®y¼Ð«Ê¥]
	FixedPosition();

	// ÀË´ú¬O§_»Ý­n°e¥X²¾°Ê«Ê¥]
	SendPlayerPos();

	// ÀË´ú¨¤¦â°Ê§@
	SendPlayerActor();

	// ÃèÀY±±¨î
	ControlCamera();

	// PK Buff §ïÅÜ
	UpdateSpritePKBuff();

	// °Ï°ì PVP ª¬ºA
	UpdatePVPZoneState();

	// §ó·s´£¥ÜÄµ§i
	UpdateEliteBell();

	// §ó·s³sµ²ª«¥ó
	UpdateAttachObj();

	// §ó·s¹Ó¸O
	// UpdateTomb();

	UpdateDummySprite();

	CheckPlayerPositionHeight();

	// Àx¦sCLIENT³]©w­È
	SaveClientSetting();

	// §ó·s¥Ø¼Ð§¤¼ÐÂI
	UpdateTargetEffectPosition();
}

// --------------------------------------------------------------------------------------
void CGameMain::CheckTouchSprite()
{
	if (m_pPlayer == NULL)
		return;

	CRoleSprite* spriteControlProxy = m_pPlayer->GetProxyControlSprite();
	for (map<int, CRoleSprite*>::iterator iter = m_spriteContainer.begin(); iter != m_spriteContainer.end(); iter++)
	{
		CRoleSprite* sprite = iter->second;
		if (sprite->GetTouchState()) {
			float distance = (sprite->GetPos() - spriteControlProxy->GetPos()).Magnitude();
			if (sprite->GetTouchInState()) {
				if (distance > 18.0f)
					sprite->SetTouchInState(false);
			}
			else if (distance < 12.0f) {
				sprite->SetTouchInState(true);
				NetCli_Item::_GetGroundItem(sprite->GetIdentify(), -1);
			}
		}
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::FixedPosition()
{
	if (m_pPlayer == NULL)
		return;

	CRoleSprite* spriteControlProxy = m_pPlayer->GetProxyControlSprite();
	CRuVector3 vec = spriteControlProxy->GetMoveVector();
	CRuVector3 dir = spriteControlProxy->GetDirection();
	CRuVector3 pos = spriteControlProxy->GetPos();

	m_lastFixedPositionTime -= m_elapsedTime;

	// ¦pªG®y¼Ð¦³§ïÅÜ, ©T©w®É¶¡¶Ç°e©w¦ì§¤¼ÐÂIµ¹ SERVER
	if ((pos - m_lastFixedPosition).Magnitude() > 2.0f && m_lastFixedPositionTime < 0.0f)
	{
		IBaseSprite* parentMount = spriteControlProxy->GetParentMount();
		int attachParentId = m_pPlayer->GetAttachParentId();
		float moveTime = spriteControlProxy->GetMoveTime();
		float moveSpeed = spriteControlProxy->GetMoveSpeed();
		CRuFusion_ActorState actorState = spriteControlProxy->GetActorState();
		ClientMoveTypeENUM moveType = EM_ClientMoveType_Normal;
		bool isAboveWater = spriteControlProxy->IsAboveWater();

		int parentMountId = 0;
		if (parentMount != NULL)
			parentMountId = parentMount->GetIdentify();

		// ¨ú±o¨¤¦â©Ò´Â¤è¦V
		float theta = acos(dir.Dot(CRuVector3(1.0f, 0.0f, 0.0f)));
		if (dir.m_z > 0.0f)
			theta = PI + PI - theta;

		// Âà´«¬°¨¤«×
		theta = theta * 180.0f / PI;
		Net_Move::PlayerMoveObject(m_pPlayer->GetIdentify(), pos.m_x, pos.m_y, pos.m_z, theta, parentMountId, vec.m_x, vec.m_y, vec.m_z, EM_ClientMoveType_OntimeSetCliPos);

		m_lastFixedPositionTime = 0.1f;
		m_lastFixedPosition = pos;
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::SendPlayerPos(bool forceSend, bool setPos)
{
	if (m_pPlayer == NULL)
		return;

	CRoleSprite* spriteControlProxy = m_pPlayer->GetProxyControlSprite();
	IBaseSprite* parentMount = spriteControlProxy->GetParentMount();
	int attachParentId = m_pPlayer->GetAttachParentId();
	bool isSend = forceSend;
	int type = 0;
	m_sendMoveTime += m_elapsedTime;
	float moveTime = spriteControlProxy->GetMoveTime();
	float moveSpeed = spriteControlProxy->GetMoveSpeed();
	CRuVector3 vec = spriteControlProxy->GetMoveVector();
	CRuVector3 dir = spriteControlProxy->GetDirection();
	CRuVector3 pos = spriteControlProxy->GetPos();
	CRuFusion_ActorState actorState = spriteControlProxy->GetActorState();
	ClientMoveTypeENUM moveType = EM_ClientMoveType_Normal;
	bool isAboveWater = spriteControlProxy->IsAboveWater();

	// ·í¦b¥i¥H²¾°Êª«¥ó¤W,¤W¦¸¶Ç°e®y¼Ð»P¥Ø«e®y¼Ð¶ZÂ÷®t¶Z¹L¤j»Ý­n¸É°e¤¤¶¡¸ô®|¹Lµ{,
	// Á×§KSERVERµLªk³B²z±Nª±®a­«·s©w¦ì
	if (parentMount)
	{
		CRuVector3 nextPosition = pos;
		while ((nextPosition - m_position).Magnitude() > 2.0f * moveSpeed)
		{
			CRuVector3 forwardVector = nextPosition - m_position;
			forwardVector.Normalize();

			nextPosition = m_position + forwardVector * (2.0f * moveSpeed);

			// ¨ú±o¨¤¦â©Ò´Â¤è¦V
			float theta = acos(dir.Dot(CRuVector3(1.0f, 0.0f, 0.0f)));
			if (dir.m_z > 0.0f)
				theta = PI + PI - theta;

			// Âà´«¬°¨¤«×
			theta = theta * 180.0f / PI;
			Net_Move::PlayerMoveObject(m_pPlayer->GetIdentify(),
				nextPosition.m_x, nextPosition.m_y, nextPosition.m_z,
				theta,
				parentMount->GetIdentify(),
				vec.m_x, vec.m_y, vec.m_z,
				EM_ClientMoveType_Mount);

			m_position = nextPosition;
			nextPosition = pos;
		}
	}

	/*
	CRuVector3 parentMountPosition;
	if ( parentMount )
	{
		parentMountPosition = parentMount->GetPos();

		g_pError->AddMessage(0, 0, "SendPlayerPos Time:%.2f [%d] pos(%.2f %.2f %.2f) parent(%.2f %.2f %.2f)",
			m_frameTime,
			type,
			pos.m_x, pos.m_y, pos.m_z,
			parentMountPosition.m_x, parentMountPosition.m_y, parentMountPosition.m_z);
	}
	*/

	float maxSendTime;
	int count = 0;
	for (map<int, CRoleSprite*>::iterator iter = m_spriteContainer.begin(); iter != m_spriteContainer.end(); iter++)
	{
		if (iter->second->GetObjectType() == eSpriteObject_Player)
			count++;
	}

	maxSendTime = SEND_MOVE_DELAY_TIME + max(count - 12, 0) * 0.03f;
	if (maxSendTime > 2.0f)
		maxSendTime = 2.0f;

	// ¤§«á·|¥[¤WY¶b¦V¶q
	dir.m_y = 0.0f;
	dir.Normalize();

	// ¦bªÅ¤¤­¸¦æ¤£°e«Ê¥]©ÎªÌ¦³³sµ²ª«¥ó¤£°e«Ê¥]
	if (spriteControlProxy->IsFloat())
		return;

	if (attachParentId != 0) {
		m_position = pos;
		return;
	}

	if (setPos)
	{
		moveType = EM_ClientMoveType_SetPos;
		vec.m_x = 0.0f;
		vec.m_y = 0.0f;
		vec.m_z = 0.0f;
	}

	switch (spriteControlProxy->GetMoveType())
	{
	case eRoleSpriteMove_Normal:
		break;
	case eRoleSpriteMove_StrikeBack:
	case eRoleSpriteMove_Charge:
		return;
	}

	switch (actorState)
	{
	case ruFUSION_ACTORSTATE_DYING:
	case ruFUSION_ACTORSTATE_DEAD:
	case ruFUSION_ACTORSTATE_REVIVING:
		return;

	case ruFUSION_ACTORSTATE_FALLING:
		m_falling = true;
		if (m_sendActorState == ruFUSION_ACTORSTATE_JUMP)
			forceSend = true;

	case ruFUSION_ACTORSTATE_JUMP:
		moveType = EM_ClientMoveType_Jump;
		vec = spriteControlProxy->GetLatentVelocity();
		vec.m_y = 0.0f;

		if (m_firstJump)
		{
			type = 1;				// ªì¦¸¸õÅD
			isSend = true;
			m_firstJump = false;
		}
		else if (m_sendMoveTime > SEND_MOVE_FORAWRD_TIME)
		{
			type = 2;			// ¸õÅD±¼¸¨¤¤,©T©w®É¶¡©w¦ì
			isSend = true;
		}
		else if (forceSend)
		{
			type = 6;			// ±j¨î¦b¸õÅD¤¤,µo¥X¨¤¦â¥Ø«e¤è¦V®y¼Ð
			isSend = true;
		}
		else
		{
			return;
		}
		break;

	default:
		if (m_falling == true) {
			// ±¼¸¨°±¤î
			m_falling = false;
			isSend = true;
		}
		else if (dir != m_direction && m_sendMoveTime > maxSendTime) {
			// ·í­±¦V§ïÅÜ®É,¨C¹j maxSendTime ®É¶¡°e¥X«Ê¥]
			type = 3;
			isSend = true;
		}
		else if (vec != m_moveVector) {
			// ·í¦V¶q»P¤è¦V§ïÅÜ®É
			type = 4;
			isSend = true;
		}
		else if (m_sendMoveTime > SEND_MOVE_FORAWRD_TIME && (moveTime > 0.0f || (pos - m_position).Magnitude() > 40.0f)) {
			// ·í©T©w´Â¦V¬Y­Ó¤è¦V²¾°Ê®É¡A¶¡¹j SEND_MOVE_FORAWRD_TIME ®É¶¡§ó·s«Ê¥]¤@¦¸
			type = 5;
			isSend = true;
		}
	}

	// ·íª±¤£°±§ïÅÜ²¾°Ê¤è¦V®É¡A´î¤Ö«Ê¥]¶Ç»¼¦¸¼Æ
	if (isSend && forceSend == false)
	{
		static int packageCount = 0;	// °O¿ýµu®É¶¡¶Ç»¼²¾°Ê«Ê¥]­Ó¼Æ

		// ÀË¬d¬O§_«Ê¥]¶Ç»¼¶¡¹j¤Óµu¡A­Y®É¶¡¶¡¹j¤Óµu²Ö­p«Ê¥]­Ó¼Æ
		if (m_sendMoveTime > maxSendTime)
		{
			if (packageCount > 0)
				packageCount--;
		}
		else if (packageCount < 5)
			packageCount++;
		else
			isSend = false;
	}

	if (isSend)
	{
		CRuVector3 distance = pos - m_position;
		distance.m_y = 0.0f;
		int parentMountId = 0;

		if (parentMount != NULL)
			parentMountId = parentMount->GetIdentify();
		else
		{
			CRuCollisionQuery_Segment_Nearest segQuery;

			// ³]©w´ú¸Õ¥Î½u¬q¤Î¨ä¥L³]©w
			segQuery.m_colSegment = CRuSegment3(pos + CRuVector3(0.0f, 12.0f, 0.0f), m_position + CRuVector3(0.0f, 12.0f, 0.0f));
			segQuery.m_exclusionMask = RuWorld::ruTRIFLAGS_WATER | RuWorld::ruTRIFLAGS_FOLIAGE;
			//segQuery.m_ignoreBackface = FALSE;
			//segQuery.m_ignoreCollisionFlag = TRUE;

			CRuContainer_Collision* collisionContainer = IBaseSprite::GetCollisionContainer();
			if (collisionContainer)
			{
				BOOL removeResult = FALSE;

				// ¼È®É²¾°£¦Û¤v,Á×§K´ú¸Õ¨ì¦Û¤w
				/*
				if ( ignoreEntity )
				{
					removeResult = collisionContainer->RemoveCollisionObject(ignoreEntity);
				}
				*/

				collisionContainer->QueryCollision(&segQuery);

				/*
				if ( ignoreEntity && removeResult )
					collisionContainer->InsertCollisionObject(ignoreEntity, ignoreEntity->GetWorldBounds(), TRUE);
				*/
			}

			// Keep the closest candidate sprite
			if (segQuery.m_positiveHit && spriteControlProxy)
			{
				spriteControlProxy->SetPos(m_position);
				return;
			}
		}

		bool clearTest = true;
		if (pos.m_y >= m_position.m_y)
		{
			float height;
			switch (moveType)
			{
			case EM_ClientMoveType_Normal:
			case EM_ClientMoveType_SetPos:				//¤¤³~´«¥Ø¼Ðr
			case EM_ClientMoveType_Mount:				//¦b·|²¾°Êª«¥ó¤W(¸ü¨ã ¦p¹q±è)
			case EM_ClientMoveType_SetPetPos:			//¦b·|²¾°Êª«¥ó¤W(¸ü¨ã ¦p¹q±è),­×¥¿Ãdª«§¤¼Ð
				// ÀË¬d¯BªÅ
				if (IsNoSwimming() && (!IBaseSprite::TerrainTestHight(pos, height, 100.0f) || (pos.m_y - height) > 25.0f))
				{
					clearTest = false;
					m_checkHeightCount++;
					if (!m_boReportSelf && m_checkHeightCount > 60)
					{
						m_boReportSelf = true;

						AutoReportBaseInfoStruct info;
						info.GUID = m_pPlayer->GetIdentify();
						info.X = pos.m_x;
						info.Y = pos.m_y;
						info.Z = pos.m_z;
						NetCli_OtherChild::SL_AutoReportPossibleCheater(info, EM_AutoReportPossibleCheater_Floating, GetDBID());
					}

					// ³qª¾SERVER
					// NetCli_Other::SL_Suicide(2);
				}
				break;
			}
		}

		if (clearTest)
			m_checkHeightCount = 0;

		CRuQuaternion quaternion;
		m_moveVector = vec;
		m_direction = dir;
		m_position = pos;
		m_sendMoveTime = 0.0f;
		m_sendActorState = actorState;

		if (m_parentMountId != parentMountId || parentMountId != 0)
		{
			ClientToClientInfo info;
			CRuVector3 offset;

			if (parentMount)
			{
				offset = parentMount->GetPos() - pos;
			}

			info.parentMount.id = parentMountId;
			info.parentMount.x = offset.m_x;
			info.parentMount.y = offset.m_y;
			info.parentMount.z = offset.m_z;

			/*
			for ( int i = 0; i < 20; i++ )
			{
				info._data[i] = i;
			}
			*/

			NetCli_Other::SL_TransferData_Range(info._data);

			m_parentMountId = parentMountId;
		}

		switch (moveType)
		{
		case EM_ClientMoveType_SetPos:
			break;

		case EM_ClientMoveType_Normal:
			if (parentMount != NULL)
			{
				moveType = EM_ClientMoveType_Mount;
			}

			// Âà´«¬°¨¤¦â´Â¦V¥Ø¼Ð¦V¶q
			quaternion.RotateIntoVector(dir, CRuVector3(0.0f, 0.0f, 1.0f));
			quaternion.TransformVector(vec);
			break;

		case EM_ClientMoveType_Jump:
			// ±N¥Ø¼Ð¦V¶qÂà´«¬°¨¤¦âZ¶b¦V¶q
			quaternion.RotateIntoVector(CRuVector3(0.0f, 0.0f, 1.0f), dir);
			quaternion.TransformVector(m_moveVector);
			break;
		}

		// ¨ú±o¨¤¦â©Ò´Â¤è¦V
		float theta = acos(dir.Dot(CRuVector3(1.0f, 0.0f, 0.0f)));
		if (dir.m_z > 0.0f)
			theta = PI + PI - theta;

		// Âà´«¬°¨¤«×
		theta = theta * 180.0f / PI;
		Net_Move::PlayerMoveObject(m_pPlayer->GetIdentify(), pos.m_x, pos.m_y, pos.m_z, theta, parentMountId, vec.m_x, vec.m_y, vec.m_z, moveType);

		// ¦³²¾°Ê¦V¶q¤¤Â_
		if (abs(vec.m_x - 0.0f) > 0.01f || abs(vec.m_y - 0.0f) > 0.01f || abs(vec.m_z - 0.0f) > 0.01f) {
			if (ItemCastingEnable()) {
				ItemCastingInterrupt();
			}
			else if (g_pTeleportBook->BreakCasting()) {
			}
		}

		//g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", "[%d] pos(%.2f %.2f %.2f) vetor(%.2f %.2f %.2f) moveType(%d) dis(%.2f) grav(%.2f)", type, pos.m_x, pos.m_y, pos.m_z, vec.m_x, vec.m_y, vec.m_z, moveType, distance.Magnitude(), m_pPlayer->GetRuActor()->GetGravitySpeed());
		//g_pError->AddMessage(0, 0, "SendPlayerPos Time:%.2f [%d] pos(%.2f %.2f %.2f) vetor(%.2f %.2f %.2f) moveType(%d) dis(%.2f)", m_frameTime, type, pos.m_x, pos.m_y, pos.m_z, vec.m_x, vec.m_y, vec.m_z, moveType, distance.Magnitude());

		/*
		CRuVector3 parentMountPosition;
		if ( parentMount )
		{
			parentMountPosition = parentMount->GetPos();
		}

		g_pError->AddMessage(0, 0, "SendPlayerPos Time:%.2f [%d] pos(%.2f %.2f %.2f) parent(%.2f %.2f %.2f) parentEntity(%.2f %.2f %.2f)",
			m_frameTime,
			type,
			pos.m_x, pos.m_y, pos.m_z,
			parentMountPosition.m_x, parentMountPosition.m_y, parentMountPosition.m_z,
			parentMountEntityPosition.m_x, parentMountEntityPosition.m_y, parentMountEntityPosition.m_z);
		*/
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::SendPlayerActor()
{
	if (m_pPlayer)
	{
		PLAYER_ACTOR_STATE actorState;
		RoleSpriteStatus playerState;
		bool sendActorState = false;

		actorState.background = m_pPlayer->GetActorState(1);
		actorState.foreground = m_pPlayer->GetActorState(2);
		actorState.transient = m_pPlayer->GetActorState(3);

		playerState.value = 0;
		playerState.isUnholster = m_pPlayer->GetUnholsterState();
		playerState.isCombat = m_pPlayer->GetCombatState();
		playerState.isWalk = m_pPlayer->GetWalkState();
		playerState.isSit = m_pPlayer->GetSitState();
		playerState.isSitChair = m_pPlayer->GetSitChairState();

		switch (actorState.background)
		{
		case ruFUSION_ACTORSTATE_NORMAL:
			m_actorState.background = actorState.background;
			break;

		case ruFUSION_ACTORSTATE_SIT_BEGIN:
		case ruFUSION_ACTORSTATE_SIT_LOOP:
		case ruFUSION_ACTORSTATE_SIT_END:
		case ruFUSION_ACTORSTATE_SLEEP_BEGIN:
		case ruFUSION_ACTORSTATE_SLEEP_LOOP:
		case ruFUSION_ACTORSTATE_SLEEP_END:
		case ruFUSION_ACTORSTATE_DYING:
		case ruFUSION_ACTORSTATE_DEAD:
		case ruFUSION_ACTORSTATE_REVIVING:
		case ruFUSION_ACTORSTATE_SPAWN_BEGIN:
		case ruFUSION_ACTORSTATE_SPAWN_END:
		case ruFUSION_ACTORSTATE_CROUCH_BEGIN:
		case ruFUSION_ACTORSTATE_CROUCH_LOOP:
		case ruFUSION_ACTORSTATE_CROUCH_END:
		case ruFUSION_ACTORSTATE_CRAFTING_BEGIN:
		case ruFUSION_ACTORSTATE_CRAFTING_LOOP:
		case ruFUSION_ACTORSTATE_CRAFTING_END:
		case ruFUSION_ACTORSTATE_MINING_BEGIN:
		case ruFUSION_ACTORSTATE_MINING_LOOP:
		case ruFUSION_ACTORSTATE_MINING_END:
		case ruFUSION_ACTORSTATE_DAZED_BEGIN:
		case ruFUSION_ACTORSTATE_DAZED_LOOP:
		case ruFUSION_ACTORSTATE_DAZED_END:
		case ruFUSION_ACTORSTATE_SLUMBER_BEGIN:
		case ruFUSION_ACTORSTATE_SLUMBER_LOOP:
		case ruFUSION_ACTORSTATE_SLUMBER_END:
		case ruFUSION_ACTORSTATE_ACTIVATE_BEGIN:
		case ruFUSION_ACTORSTATE_ACTIVATE_LOOP:
		case ruFUSION_ACTORSTATE_ACTIVATE_END:
		case ruFUSION_ACTORSTATE_READING_BEGIN:
		case ruFUSION_ACTORSTATE_READING_LOOP:
		case ruFUSION_ACTORSTATE_READING_END:
		case ruFUSION_ACTORSTATE_SIT_CHAIR_BEGIN:
		case ruFUSION_ACTORSTATE_SIT_CHAIR_LOOP:
		case ruFUSION_ACTORSTATE_SIT_CHAIR_END:
		case ruFUSION_ACTORSTATE_EMOTE_HANDSTAND:
		case ruFUSION_ACTORSTATE_EMOTE_ROFL:
		case ruFUSION_ACTORSTATE_EMOTE_SWEEP1:
		case ruFUSION_ACTORSTATE_EMOTE_SWEEP2:
		case ruFUSION_ACTORSTATE_EMOTE_SWEEP3:
			if (m_actorState.background != actorState.background)
			{
				m_actorState.background = actorState.background;
				sendActorState = true;
			}
			break;
		}

		switch (actorState.transient)
		{
		case ruFUSION_ACTORSTATE_NORMAL:
			m_actorState.transient = actorState.transient;
			break;

		case ruFUSION_ACTORSTATE_USE:
		case ruFUSION_ACTORSTATE_EMOTE_ANGRY:
		case ruFUSION_ACTORSTATE_EMOTE_APPLAUSE:
		case ruFUSION_ACTORSTATE_EMOTE_APPROVAL:
		case ruFUSION_ACTORSTATE_EMOTE_BEG:
		case ruFUSION_ACTORSTATE_EMOTE_CRY:
		case ruFUSION_ACTORSTATE_EMOTE_HEAD_NOD:
		case ruFUSION_ACTORSTATE_EMOTE_HEAD_SHAKE:
		case ruFUSION_ACTORSTATE_EMOTE_LAUGH:
		case ruFUSION_ACTORSTATE_EMOTE_POINT:
		case ruFUSION_ACTORSTATE_EMOTE_PROVOKE:
		case ruFUSION_ACTORSTATE_EMOTE_SALUTE:
		case ruFUSION_ACTORSTATE_EMOTE_SPEAK:
		case ruFUSION_ACTORSTATE_EMOTE_SURRENDER:
		case ruFUSION_ACTORSTATE_EMOTE_THREATEN:
		case ruFUSION_ACTORSTATE_EMOTE_VICTORY:
		case ruFUSION_ACTORSTATE_EMOTE_WAVE:
		case ruFUSION_ACTORSTATE_EMOTE_EAT:
		case ruFUSION_ACTORSTATE_EMOTE_DRINK:
		case ruFUSION_ACTORSTATE_EMOTE_IDLE:
		case ruFUSION_ACTORSTATE_EMOTE_IDLE0:
		case ruFUSION_ACTORSTATE_EMOTE_IDLE1:
		case ruFUSION_ACTORSTATE_EMOTE_IDLE2:
		case ruFUSION_ACTORSTATE_EMOTE_DANCE:
		case ruFUSION_ACTORSTATE_EMOTE_PLAY_SAXOPHONE:
		case ruFUSION_ACTORSTATE_EMOTE_PLAY_TAMBOURINE:
		case ruFUSION_ACTORSTATE_EMOTE_PLAY_GUITAR:
		case ruFUSION_ACTORSTATE_EMOTE_JUMP_INPLACE:
		case ruFUSION_ACTORSTATE_EMOTE_BELLY:
		case ruFUSION_ACTORSTATE_EMOTE_EMBRAASSED:
		case ruFUSION_ACTORSTATE_EMOTE_EMBRACE:
		case ruFUSION_ACTORSTATE_EMOTE_KISS:
		case ruFUSION_ACTORSTATE_EMOTE_SALUTE2:
		case ruFUSION_ACTORSTATE_EMOTE_CUTE1:
		case ruFUSION_ACTORSTATE_EMOTE_CUTE2:
		case ruFUSION_ACTORSTATE_EMOTE_CUTE3:
		case ruFUSION_ACTORSTATE_EMOTE_HANGOVER:
		case ruFUSION_ACTORSTATE_EMOTE_STRETCH:
		case ruFUSION_ACTORSTATE_EMOTE_ROCK:
		case ruFUSION_ACTORSTATE_EMOTE_KAMEHAMEHA:
		case ruFUSION_ACTORSTATE_EMOTE_KICK:
		case ruFUSION_ACTORSTATE_EMOTE_BREAKDANCE:
		case ruFUSION_ACTORSTATE_EMOTE_TAPDANCE:
		case ruFUSION_ACTORSTATE_EMOTE_TIRED:
		case ruFUSION_ACTORSTATE_EMOTE_FEAR:
		case ruFUSION_ACTORSTATE_EMOTE_AMAZED:
		case ruFUSION_ACTORSTATE_EMOTE_THINK:
		case ruFUSION_ACTORSTATE_HOLSTER:
		case ruFUSION_ACTORSTATE_UNHOLSTER:
		case ruFUSION_ACTORSTATE_HOLSTER_2H:
		case ruFUSION_ACTORSTATE_UNHOLSTER_2H:
			if (m_actorState.transient != actorState.transient)
			{
				m_actorState.transient = actorState.transient;
				sendActorState = true;
			}
			break;
		}

		if (m_playerState.value != playerState.value)
		{
			m_playerState.value = playerState.value;
			sendActorState = true;
		}

		if (sendActorState)
		{
			Net_Move::S_DoAction(m_pPlayer->GetIdentify(), m_playerState.value, m_actorState.value);
		}
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::UpdateCursor()
{
	// ÀË¬d¥þ°ìª«¥ó¬O§_¦s¦b
	if (g_pItemClipboard == NULL ||
		g_pBagFrame == NULL ||
		g_pStoreFrame == NULL ||
		g_pHousesFrame == NULL ||
		g_pGuildHousesFrame == NULL) {
		return;
	}

	// ¬O§_¦³©ì¦²ª«¥ó
	if (g_pItemClipboard->GetType() != eClipboardItem_Type_None)
	{
		SetCursor(eCursor_DragItem, g_pItemClipboard->GetFile());
		SetCastSpellState(CASTSPELL_NONE, true);
		return;
	}

	int useIndex = g_pBagFrame->GetUseItemIndex();
	if (useIndex != -1)
	{
		if (CNetGlobal::RoleData() != NULL)
		{
			ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(useIndex);
			if (item && !item->IsEmpty())
			{
				GameObjDbStruct* itemDB = CNetGlobal::GetObj(item->OrgObjID);
				switch (itemDB->Item.Class)
				{
				case EM_Item_Rune:
					SetCursor(eCursor_Rune);
					return;

				default:
					if (itemDB->Item.ItemType == EM_ItemType_Pet)
					{
						SetCursor(eCursor_Egg);
					}
					else
					{
						switch (itemDB->Item.UseType)
						{
						case EM_ItemUseType_EqRefine:			//½Äµ¥
							SetCursor(eCursor_Enhance);
							return;
						case EM_ItemUseType_FixEq:				//­×¸Ë
							SetCursor(eCursor_Repair);
							return;
						case EM_ItemUseType_Dissolution:		// ¤À¸Ñ
							SetCursor(eCursor_Dissolution);
							return;
						case EM_ItemUseType_Coloring:			// ¬V¦â
							SetCursor(eCursor_Brush);
							return;
						case EM_ItemUseType_ClsEqSoulbound:		// ¸Ñ¸j©w
							SetCursor(eCursor_Enhance);
							return;
						case EM_ItemUseType_ClsTrade:		// ¸Ñ¸j©w
							SetCursor(eCursor_Enhance);
							return;
						case EM_ItemUseType_EQDigHole:			// ¥´¬}
							SetCursor(eCursor_Enhance);
							return;
						case EM_ItemUseType_EQClearRune:		// ²M°£²Å¤å
							SetCursor(eCursor_Enhance);
							return;
						case EM_ItemUseType_ItemLock:			// Âê©w
							SetCursor(eCursor_Locked);
							return;
						case EM_ItemUseType_ItemUnLock:			// ¸ÑÂê
							SetCursor(eCursor_Unlocked);
							return;
						case EM_ItemUseType_EQProtect:			// ¸Ë³Æ«OÅ@
							SetCursor(eCursor_Restore);
							return;
						case EM_ItemUseType_EQLimitMax:			// ¸Ë³Æ­@¤[¤W­­¨ìMax
							SetCursor(eCursor_Protect);
							return;
						case EM_ItemUseType_IgnoreLimitLv:		// ©¿²¤µ¥¯Å­­¨î
							SetCursor(eCursor_IgonreLevel);
							return;
						case EM_ItemUseType_PK_EQProtect:		//PK¸Ë³Æ«OÅ@
							SetCursor(eCursor_Point2);
							return;
						case EM_ItemUseType_PK_EQProtect_Clear:	//PK¸Ë³Æ«OÅ@²M°£
							SetCursor(eCursor_Point2);
							return;
						case EM_ItemUseType_ClsEqAbility:	//PK¸Ë³Æ«OÅ@²M°£
							SetCursor(eCursor_Point2);
							return;
						case EM_ItemUseType_ItemtoItemLua:	//ª««~¹ïª««~Ä²µolua
							SetCursor(eCursor_Enhance);
							return;
						}
					}
				}
				SetCastSpellState(CASTSPELL_NONE, true);
			}
		}
		return;
	}

	// ª««~¹ï¦a¨Ï¥Î
	if (g_pBagFrame->GetUseItemToLocationID() != -1)
	{
		SetCursor(eCursor_Point);
		return;
	}

	if (g_pStoreFrame->GetRepairState())
	{
		SetCursor(eCursor_Repair);
		return;
	}

	if (m_castSpellToItem != 0)
	{
		SetCursor(eCursor_Dissolution);
		return;
	}

	if (m_castSpellState == CASTSPELL_WAITING_LOCATION_EXTRA)
	{
		SetCursor(eCursor_Cast_Magic);
		return;
	}

	// ÃB¥~¬Iªk»Ý­nÂI¿ï¥Ø¼Ð
	if (m_castSpellState == CASTSPELL_WAITING_TARGET_EXTRA)
	{
		SetCursor(eCursor_Cast_Magic);
		return;
	}

	// ¬IªkÂI¿ï¥Ø¼Ð©Î®y¼Ð
	if (m_castSpellState == CASTSPELL_WAITING_LOCATION || m_castSpellState == CASTSPELL_WAITING_TARGET) {
		CursorType cursor = eCursor_Cast_Phy;

		// ¨ÌÂ¾·~´å¼Ð¤£¦P
		GameObjDbStructEx* objDB = CNetGlobal::GetObj(m_castSpellID);
		if (objDB) {
			switch (objDB->MagicCol.JobInfo.Job)
			{
			case EM_Vocation_GameMaster:
			case EM_Vocation_Warrior:
			case EM_Vocation_Ranger:
			case EM_Vocation_Rogue:
			case EM_Vocation_Knight:
			case EM_Vocation_Psyron:
			case EM_Vocation_Duelist:
				cursor = eCursor_Cast_Phy;
				break;

			case EM_Vocation_Wizard:
			case EM_Vocation_Priest:
			case EM_Vocation_RuneDancer:
			case EM_Vocation_Druid:
			case EM_Vocation_Harpsyn:
				cursor = eCursor_Cast_Magic;
				break;
			}
		}
		SetCursor(cursor);
		return;
	}

	//©Ð«Î
	if (g_pHousesFrame->GetHousesState() == EHousesState_Enter)
	{
		CursorType HousesCursor = g_pHousesFrame->GetHousesCursor();
		if (HousesCursor != eCursor_Null)
		{
			SetCursor(HousesCursor);
			return;
		}
	}

	//¤½·|©Ð«Î
	if (g_pGuildHousesFrame->GetHousesState() == EHousesState_Enter)
	{
		CursorType HousesCursor = g_pGuildHousesFrame->GetGuildHousesCursor();
		if (HousesCursor != eCursor_Null)
		{
			SetCursor(HousesCursor);
			return;
		}
	}

	// ¬O§_¦³¿ï¨ìª«¥ó
	if (m_pMouseInPlayer == NULL)
	{
		if (m_cursorHyperlink)
			SetCursor(eCursor_Hyperlink);
		else
			SetCursor(eCursor_Point);
		return;
	}

	// ¦b¸ü¨ã¤W
	CRoleSprite* implementSprite = NULL;
	if (m_pPlayer && m_pPlayer->GetImplementSprite())
	{
		implementSprite = m_pPlayer->GetImplementSprite();
	}

	if (implementSprite)
	{
		SetCursor(eCursor_Point);
		return;
	}

	RoleCheckedStatus emCheckedStatus = (RoleCheckedStatus)m_pMouseInPlayer->GetCheckedStatus();

	// ¬O§_¥i¥HÀË«ÍÅé
	if (m_pMouseInPlayer->GetCorpseState())
	{
		SetCursor(eCursor_Pickup);
		return;
	}

	// ¬O§_¥i¥H§ðÀ»Àu¥ý§PÂ_
	if (m_pMouseInPlayer->GetPriorityAttack() && m_pMouseInPlayer->GetCanAttackState())
	{
		SetCursor(eCursor_Attack);
		return;
	}

	/*
	// ¬O§_¥i¥H¶ð­¼
	if ( m_pMouseInPlayer->GetImplementMaxNum() > m_pMouseInPlayer->GetImplementCount() )
	{
		SetCursor(eCursor_Interact);
		return;
	}
	*/

	// ±j¨îÃö³¬´å¼Ð
	if (m_pMouseInPlayer->GetDisableQuestState())
	{
		SetCursor(eCursor_unableinspect);
		return;
	}

	// ¦³³]©w¯S©w´å¼ÐÃþ«¬
	CursorType	emCursorType = (CursorType)m_pMouseInPlayer->GetCursorMode();
	if (emCursorType != eCursor_Null)
	{
		switch (emCursorType)
		{
		case eCursor_Interact:	// ¾¦½ü
		case eCursor_UnableInteract:
		{
			if (GetDistanceWithPlayer(m_pMouseInPlayer) > (float)m_pMouseInPlayer->GetTouchRange())
				SetCursor(eCursor_UnableInteract);
			else
				SetCursor(eCursor_Interact);
		} break;
		case eCursor_Speak:
		case eCursor_UnableSpeak:
		{
			if (GetDistanceWithPlayer(m_pMouseInPlayer) > (float)m_pMouseInPlayer->GetTouchRange())
				SetCursor(eCursor_UnableSpeak);
			else
				SetCursor(eCursor_Speak);
		} break;
		case eCursor_Mine:
		case eCursor_UnableMine:
		{
			if (GetDistanceWithPlayer(m_pMouseInPlayer) > (float)m_pMouseInPlayer->GetTouchRange())
				SetCursor(eCursor_UnableMine);
			else
				SetCursor(eCursor_Mine);
		} break;
		case eCursor_Herb:
		case eCursor_UnableHerb:
		{
			if (GetDistanceWithPlayer(m_pMouseInPlayer) > (float)m_pMouseInPlayer->GetTouchRange())
				SetCursor(eCursor_UnableHerb);
			else
				SetCursor(eCursor_Herb);
		} break;
		case eCursor_Shop:
		case eCursor_UnableShop:
		{
			if (GetDistanceWithPlayer(m_pMouseInPlayer) > (float)m_pMouseInPlayer->GetTouchRange())
				SetCursor(eCursor_UnableShop);
			else
				SetCursor(eCursor_Shop);
		} break;
		case eCursor_Mail:
		case eCursor_UnableMail:
		{
			if (GetDistanceWithPlayer(m_pMouseInPlayer) > (float)m_pMouseInPlayer->GetTouchRange())
				SetCursor(eCursor_UnableMail);
			else
				SetCursor(eCursor_Mail);
		} break;
		case eCursor_inspect:
		case eCursor_unableinspect:
		{
			if (GetDistanceWithPlayer(m_pMouseInPlayer) > (float)m_pMouseInPlayer->GetTouchRange())
				SetCursor(eCursor_unableinspect);
			else
				SetCursor(eCursor_inspect);
		} break;
		case eCursor_OpenSound:
		case eCursor_CloseSound:
		{
			if (GetDistanceWithPlayer(m_pMouseInPlayer) > (float)m_pMouseInPlayer->GetTouchRange())
				SetCursor(eCursor_CloseSound);
			else
				SetCursor(eCursor_OpenSound);
		} break;
		case eCursos_Implement:
		case eCursos_UnableImplement:
		{
			if (GetDistanceWithPlayer(m_pMouseInPlayer) > (float)m_pMouseInPlayer->GetTouchRange())
				SetCursor(eCursos_UnableImplement);
			else
				SetCursor(eCursos_Implement);
		} break;

		case eCursor_Locked:
		case eCursor_Unlocked:
		{
			if (GetDistanceWithPlayer(m_pMouseInPlayer) > (float)m_pMouseInPlayer->GetTouchRange())
				SetCursor(eCursor_Unlocked);
			else
				SetCursor(eCursor_Locked);
		} break;
		case eCursor_Sit:
		case eCursor_UnableSit:
		{
			if (GetDistanceWithPlayer(m_pMouseInPlayer) > (float)m_pMouseInPlayer->GetTouchRange())
				SetCursor(eCursor_UnableSit);
			else
				SetCursor(eCursor_Sit);
		} break;
		case eCursor_Attack:
		case eCursor_UnableAttack:
		{
			if (GetDistanceWithPlayer(m_pMouseInPlayer) > (float)m_pMouseInPlayer->GetTouchRange())
				SetCursor(eCursor_UnableAttack);
			else
				SetCursor(eCursor_Attack);
		} break;
		case eCursor_Repair:
		case eCursor_UnableRepair:
		{
			if (GetDistanceWithPlayer(m_pMouseInPlayer) > (float)m_pMouseInPlayer->GetTouchRange())
				SetCursor(eCursor_UnableRepair);
			else
				SetCursor(eCursor_Repair);
		} break;
		case eCursor_Cast_Phy:
		case eCursor_Cast_Magic:
		case eCursor_Hyperlink:
		case eCursor_FurnitureRorare:
		case eCursor_Protect:
		case eCursor_Brush:
		case eCursor_Enhance:
		case eCursor_Pickup:
			SetCursor(emCursorType);
			break;
		default:
		{
			// ¨Ï¥Î¾¦½ü
			if (GetDistanceWithPlayer(m_pMouseInPlayer) > (float)m_pMouseInPlayer->GetTouchRange())
				SetCursor(eCursor_UnableInteract);
			else
				SetCursor(eCursor_Interact);
		}
		}
		return;
	}

	// ¬O§_¦³Ä²¸I¾Ú±¡
	if (m_pMouseInPlayer->GetTouchRange() != 0)
	{
		switch (emCheckedStatus)
		{
		case eChecked_Tomb:
			// ¨Ï¥Î¾¦½ü
			if (GetDistanceWithPlayer(m_pMouseInPlayer) > (float)m_pMouseInPlayer->GetTouchRange())
				SetCursor(eCursor_UnableInteract);
			else
				SetCursor(eCursor_Interact);
			break;

		case eChecked_TouchPlot:	// ¤v¸g¨S¦³³oªF¦è¤F
		case eChecked_QuestObj:
		case eChecked_QuestNPC:
			// ¨Ï¥Î¾¦½ü
			if (GetDistanceWithPlayer(m_pMouseInPlayer) > (float)m_pMouseInPlayer->GetTouchRange())
				SetCursor(eCursor_UnableInteract);
			else
				SetCursor(eCursor_Interact);
			break;

		default:
			// ¨Ï¥Î»¡¸Ü
			if (GetDistanceWithPlayer(m_pMouseInPlayer) > (float)m_pMouseInPlayer->GetTouchRange())
				SetCursor(eCursor_UnableSpeak);
			else
				SetCursor(eCursor_Speak);
			break;
		}
		return;
	}

	// §ðÀ»§PÂ_
	if (m_pMouseInPlayer->GetPriorityAttack() == false && m_pMouseInPlayer->GetCanAttackState())
	{
		SetCursor(eCursor_Attack);
		return;
	}

	// ¥i¥H¶ð­¼
	if (m_pMouseInPlayer->GetImplementMaxNum() > m_pMouseInPlayer->GetImplementCount())
	{
		if (GetDistanceWithPlayer(m_pMouseInPlayer) > MAX_IMPLEMENT_TOUCH_RANGE)
			SetCursor(eCursos_UnableImplement);
		else
			SetCursor(eCursos_Implement);
		return;
	}


	switch (emCheckedStatus)
	{
	case eChecked_Tomb:
		/*
		{
			if (GetDistanceWithPlayer(m_pMouseInPlayer) > _MAX_ROLE_TOUCH_RANGE_)
				SetCursor(eCursor_UnableInteract);
			else
				SetCursor(eCursor_Interact);

		}
		*/
		return;

		// °Ó¤H
	case eChecked_Shop:
		if (GetDistanceWithPlayer(m_pMouseInPlayer) > m_pMouseInPlayer->GetClientTouchRange())
			SetCursor(eCursor_UnableShop);
		else
			SetCursor(eCursor_Shop);
		return;

	case eChecked_Mine:
		if (GetDistanceWithPlayer(m_pMouseInPlayer) > m_pMouseInPlayer->GetClientTouchRange())
			SetCursor(eCursor_UnableMine);
		else
			SetCursor(eCursor_Mine);
		return;

	case eChecked_Herb:
		if (GetDistanceWithPlayer(m_pMouseInPlayer) > m_pMouseInPlayer->GetClientTouchRange())
			SetCursor(eCursor_UnableHerb);
		else
			SetCursor(eCursor_Herb);
		return;

	case eChecked_Lumber:
		if (GetDistanceWithPlayer(m_pMouseInPlayer) > m_pMouseInPlayer->GetClientTouchRange())
			SetCursor(eCursor_Unablefell);
		else
			SetCursor(eCursor_Fell);
		return;
		//case eChecked_FurnitureNormal:
		//	break;

	case eChecked_FurnitureStorage: // ³Ã­Ñ Ä_½c ¦ç¬[
		if (GetDistanceWithPlayer(m_pMouseInPlayer) > m_pMouseInPlayer->GetClientTouchRange())
			SetCursor(eCursor_UnableInteract);
		else
			SetCursor(eCursor_Interact);
		return;

	case eChecked_FurnitureSound: // ³Ã­Ñ ¯dÁn¾÷
		if (m_pMouseInPlayer->GetActorState() == ruFUSION_ACTORSTATE_ACTIVATE_LOOP ||
			m_pMouseInPlayer->GetActorState() == ruFUSION_ACTORSTATE_ACTIVATE_BEGIN)
			SetCursor(eCursor_CloseSound);
		else
			SetCursor(eCursor_OpenSound);
		return;

	case eChecked_Maid: // ¤k¹²
		//if (GetDistanceWithPlayer(m_pMouseInPlayer) > _MAX_ROLE_TOUCH_RANGE_)
		//	SetCursor(eCursor_UnableSpeak);
		//else
		//	SetCursor(eCursor_Speak);
		SetCursor(eCursor_Speak);
		return;

	case eChecked_Servant: // ¤k¹²
		SetCursor(eCursor_Speak);
		return;

	case eChecked_GuildHousesBuildPoint: // ¤½·|«Î°Ñ¦ÒÂI
		SetCursor(eCursor_Interact);
		return;

	case eChecked_TouchPlot:	// ¤v¸g¨S¦³³oªF¦è¤F
	case eChecked_QuestObj:
	case eChecked_QuestNPC:
		if (m_pMouseInPlayer->GetTouchRange() != 0)
		{
			// Ä²¸I¼@±¡
			if (emCheckedStatus == eChecked_QuestObj)
			{
				// ¨Ï¥Î¾¦½ü
				if (GetDistanceWithPlayer(m_pMouseInPlayer) > (float)m_pMouseInPlayer->GetTouchRange())
					SetCursor(eCursor_UnableInteract);
				else
					SetCursor(eCursor_Interact);
			}
			else
			{
				// ¨Ï¥Î»¡¸Ü
				if (GetDistanceWithPlayer(m_pMouseInPlayer) > (float)m_pMouseInPlayer->GetTouchRange())
					SetCursor(eCursor_UnableSpeak);
				else
					SetCursor(eCursor_Speak);
			}
		}
		else
		{
			// TODO §ï¦¨¥Î¸ü¤J®É§PÂ_
			if (m_pMouseInPlayer->GetCheckedStatus() == eChecked_QuestObj)
			{
				if (CNetCli_Script_Child::CheckQuestObj(m_pMouseInPlayer->GetObjectId()) != -1)
				{
					//float fRadius = m_pMouseInPlayer->GetRadius();
					if (GetDistanceWithPlayer(m_pMouseInPlayer) > m_pMouseInPlayer->GetClientTouchRange())
						SetCursor(eCursor_UnableInteract);
					else
						SetCursor(eCursor_Interact);
				}
				else
					SetCursor(eCursor_Point);
			}
			else
			{
				if (GetDistanceWithPlayer(m_pMouseInPlayer) > m_pMouseInPlayer->GetClientTouchRange())
					SetCursor(eCursor_UnableSpeak);
				else
					SetCursor(eCursor_Speak);
			}
		}
		return;

	default:
	{
		//§PÂ_¬O¤£¬O´È¤l
		float distance;
		CRuEntity* pEntity = m_pMouseInPlayer->GetRuEntity();
		if (pEntity && EntityIsChair(pEntity, &distance))
		{
			if (distance > DF_SIT_TO_CHAIR_MAX_DISTANCE)
			{
				SetCursor(eCursor_UnableSit);
			}
			else
			{
				SetCursor(eCursor_Sit);
			}
			return;
		}
	}
	SetCursor(eCursor_Point);
	break;
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::ControlCamera()
{
	if (m_pCamera == NULL)
		return;

	// ÃèÀY¸òÀH¨¤¦â²¾°Ê
	CRoleSprite* sprite = (m_cameraFollowSprite) ? m_cameraFollowSprite : this->GetPlayer();

	if (sprite && !m_isFreeCamera)
	{
		CRoleSprite* implementSprite = sprite->GetImplementSprite();
		CRoleSprite* controlSprite = sprite->GetProxyControlSprite();
		CRoleSprite* mountSprite = sprite->GetMountSprite();
		CRuVector3 playerPosition = controlSprite->GetPos();
		CRuVector3 positionHeight = CRuVector3(0.0f, 16.0f, 0.0f);

		if (implementSprite)
			positionHeight.m_y += 20.0f;
		else if (mountSprite) {
			if (m_mountObjectId != mountSprite->GetObjectId()) {
				CRuVector3 position = mountSprite->GetAttachPos(ATTACH_POINT_RIDE_01) + 6.0f;
				m_mountObjectId = mountSprite->GetObjectId();
				m_mountHeight = position.m_y - mountSprite->GetPos().m_y;
			}
			positionHeight.m_y = m_mountHeight;
		}

		CRuVector3 relativeCamPos = m_pCamera->GetCameraPosition() - m_pCamera->GetTargetPosition();
		CRuVector3 absoluteCamPos = relativeCamPos + controlSprite->GetPos();

		m_pCamera->SetTargetPosition(controlSprite->GetPos() + positionHeight);
		m_pCamera->SetCameraPosition(absoluteCamPos + positionHeight);

		if (m_cameraFollowSprite)
		{
			CRuVector3 forward = controlSprite->GetDirection();

			// ¨ú±oª±®a­±¦V»PÃèÀY¤è¦V(¤£­pºâY¶b)
			forward.m_y = 0.0f;
			forward.Normalize();

			CRuVector3 relativeCamPos = m_pCamera->GetTargetPosition() - m_pCamera->GetCameraPosition();
			relativeCamPos.m_y = 0.0f;
			relativeCamPos.Normalize();

			// ¨ú±o¨â­Ó¦V¶q§¨¨¤
			float theta = mine_acos(forward.Dot(relativeCamPos));

			// §¨¨¤¤Ó¤p®É¤£³B²z
			if (theta > 0.005f)
			{
				// ¥Ñ¦V¶q¿n¨ú±o¨â¦V¶q ¬°¥¿¦V±ÛÂà©Î
				CRuVector3 vecCross = CrossProduct(relativeCamPos, forward);
				vecCross.Normalize();
				float angle = mine_acos(vecCross.Dot(CRuVector3(0.0f, 1.0f, 0.0f)));

				if (angle > -0.1 && angle < 0.1)
					theta = -theta;

				// ­pºâ±ÛÂà¦Ê¤À¤ñ
				FLOAT fPercentage = 1.0f - powf(0.7f, 4.0f * m_elapsedTime);

				RotateCamera(theta * fPercentage, 0.0f);
			}
		}
	}

	m_pCamera->SetCameraUpVector(CRuVector3(0.0f, 1.0f, 0.0f));
}

// --------------------------------------------------------------------------------------
void CGameMain::UpdateSneakSprite()
{
	CRoleSprite* player = m_pPlayer;
	if (player == NULL)
		return;

	for (map<int, CRoleSprite*>::iterator iter = m_spriteContainer.begin(); iter != m_spriteContainer.end(); iter++)
	{
		CRoleSprite* sprite = iter->second;
		SpriteSpecialEffect effect = sprite->GetSpecailEffect();

		// Áô§Î
		if (effect.invisible > 0) {
			sprite->SetSneakAlphaState(false);
			sprite->SetSneakHiddenState(true);
		}
		else if (effect.sneak > 0) {
			int level = player->GetLevel() - sprite->GetLevel();
			float baseDistance = 30.0f;

			float distance = 0.0f;
			if (m_isBattleGround && player->GetCampID() == sprite->GetCampID())
			{
				distance = 10000.0f;
			}
			else
			{
				if (player->GetSpecailEffect().detectSneak > 0)
					baseDistance += (float)(min(max(g_ObjectData->GetSysKeyValue("Listening"), 0), 500));
				distance = baseDistance + ((level > 0) ? level * 5.0f : level * 3.0f);
			}

			sprite->SetSneakAlphaState(true);
			if (player == sprite || distance > (player->GetPos() - sprite->GetPos()).Magnitude())
				sprite->SetSneakHiddenState(false);
			else
				sprite->SetSneakHiddenState(true);
		}
		else {
			sprite->SetSneakAlphaState(false);
			sprite->SetSneakHiddenState(false);
		}
	}

	// ÀË¬d¥Ø¼Ð¬O§_¬Ý¤£¨ì
	CRoleSprite* target = player->GetTargetSprite();
	if (target && target->GetSneakHiddenState()) {
		SetPlayerTarget(NULL);
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::MovePlayerPos(CRuVector3& pos)
{
	CRoleSprite* player = m_pPlayer;
	if (player)
	{
		CRuVector3 moveVector = pos - player->GetPos();
		CRuVector3 dir = player->GetDirection();

		dir.m_y = -abs(dir.m_z);

		m_pCamera->EnableYSmoothing(TRUE);
		m_pCamera->Move(moveVector, false);
		{
			CRuEntity* playerEntity = player->GetRuEntity();
			CRuQuaternion playerRotation = playerEntity ? playerEntity->GetRotation(ruFRAMETYPE_WORLD) : CRuQuaternion(0.0f, 0.0f, 0.0f, 1.0f);
			CRuVector3 basisForwardVector(0.0f, 0.0f, -1.0f);

			playerRotation.TransformVector(basisForwardVector);
			REAL camToTargetDistance = (m_pCamera->GetCameraPosition() - m_pCamera->GetTargetPosition()).Magnitude();
			m_pCamera->SetCameraPosition(m_pCamera->GetTargetPosition() - dir * camToTargetDistance);
		}

		{

			if (moveVector.Magnitude() > 0.01f)
				g_pTeleportBook->BreakCasting();
		}

	}
	else
	{
		m_pCamera->SetTargetPosition(pos + CRuVector3(0.0f, 16.0f, 0.0f));
		m_pCamera->SetCameraPosition(pos + CRuVector3(0.0f, 64.0f, -48.0f));
		m_pCamera->SetCameraUpVector(CRuVector3(0.0f, 1.0f, 0.0f));
	}

	// ­×¥¿¶ZÂ÷
	{
		CRuVector3 cameraVector = m_pCamera->GetCameraPosition() - m_pCamera->GetTargetPosition();
		cameraVector.Normalize();
		m_pCamera->SetCameraPosition(m_pCamera->GetTargetPosition() + cameraVector * m_cameraDistance);
	}

	m_pRuCameraController->UpdatePresetCameraConfiguration();
}

// --------------------------------------------------------------------------------------
bool CGameMain::IsPatternStringVisible(const BYTE* pattern)
{
	bool IsSaved = false;
	bool Sucess = false;
	for (vector<CRuAlgorithm_PatternMatch_FJS*>::iterator iter = m_patternMatch.begin(); iter != m_patternMatch.end(); iter++) {
		CRuAlgorithm_PatternMatch_FJS* algoFJS = *iter;
		const BYTE* pstring = algoFJS->GetPattern();
		Sucess = false;
		if (strlen((char*)pattern) == algoFJS->GetPatternLength())
		{
			Sucess = true;
			for (int i = 0; i < strlen((char*)pattern); i++)
			{
				if (pstring[i] != pattern[i])
				{
					Sucess = false;
					break;
				}
			}
		}
		if (Sucess)
		{
			IsSaved = true;
		}
	}
	if (!IsSaved)
	{
		return true;
	}
	else
	{
		return false;
	}
}
// --------------------------------------------------------------------------------------
void CGameMain::PushPatternString(const BYTE* pattern)
{
	if (pattern == NULL || pattern[0] == 0)
		return;

	CRuAlgorithm_PatternMatch_FJS* algoFJS = ruNEW CRuAlgorithm_PatternMatch_FJS();
	algoFJS->SetPattern(pattern, (INT32)strlen((char*)pattern));

	bool IsSaved = false;
	bool Sucess = false;
	for (vector<CRuAlgorithm_PatternMatch_FJS*>::iterator iter = m_patternMatch.begin(); iter != m_patternMatch.end(); iter++) {
		CRuAlgorithm_PatternMatch_FJS* algoFJS = *iter;
		const BYTE* pstring = algoFJS->GetPattern();
		Sucess = false;
		if (strlen((char*)pattern) == algoFJS->GetPatternLength())
		{
			Sucess = true;
			for (int i = 0; i < strlen((char*)pattern); i++)
			{
				if (pstring[i] != pattern[i])
				{
					Sucess = false;
					break;
				}
			}
		}
		if (Sucess)
		{
			IsSaved = true;
		}
	}
	if (!IsSaved)
	{
		m_patternMatch.push_back(algoFJS);
	}
}
// --------------------------------------------------------------------------------------
void CGameMain::RemovePatternString(const BYTE* pattern, int type)
{
	int i, k = 0, s = 0, j = 0;
	int dlen = 0;
	if (m_patternMatch.size() == 1)
	{
		CRuAlgorithm_PatternMatch_FJS* data = m_patternMatch.back();
		const BYTE* temp = data->GetPattern();
		dlen = data->GetPatternLength();
		k = 0;
		if (dlen == strlen((char*)pattern))
		{
			for (i = 0; i < dlen; i++)
			{
				if (temp[i] != pattern[i])
				{
					k = 1;
					break;
				}
			}
		}
		else
		{
			k = 1;
		}

		if (k == 0)
		{
			m_patternMatch.clear();
		}

	}
	else if (m_patternMatch.size() >= 2)
	{
		for (vector<CRuAlgorithm_PatternMatch_FJS*>::iterator iter = m_patternMatch.begin(); iter != m_patternMatch.end(); iter++) {
			CRuAlgorithm_PatternMatch_FJS* algoFJS = *iter;
			const BYTE* pstring = algoFJS->GetPattern();
			k = 0;	s = 0;

			if (strlen((char*)pattern) == algoFJS->GetPatternLength())
			{
				for (i = 0; i < strlen((char*)pattern); i++)
				{
					if (pstring[i] != pattern[i])
					{
						k = 1;
						break;
					}
				}
			}
			else
			{
				k = 1;
			}

			if (k == 0)
			{
				if (type == 2)
				{
					g_pCoverStringFrame->MatchOverlap();
				}

				if (iter != m_patternMatch.end() - 1)
				{
					m_patternMatch.erase(iter);
				}
				else if (iter == m_patternMatch.end()) {
					CRuAlgorithm_PatternMatch_FJS* data = m_patternMatch.back();
					const BYTE* temp = data->GetPattern();

					//	m_patternMatch.pop_back();
				}
				else if (iter == m_patternMatch.end() - 1) {
					CRuAlgorithm_PatternMatch_FJS* data = m_patternMatch.back();
					const BYTE* temp = data->GetPattern();
					//	m_patternMatch.erase(iter);
				}
			}

			if (j == 0)
			{
				j = 1;
				CRuAlgorithm_PatternMatch_FJS* data = m_patternMatch.back();
				const BYTE* temp = data->GetPattern();
				dlen = data->GetPatternLength();
				k = 0;
				if (dlen == strlen((char*)pattern))
				{
					for (i = 0; i < dlen; i++)
					{
						if (temp[i] != pattern[i])
						{
							k = 1;
							break;
						}
					}
				}
				else
				{
					k = 1;
				}

				if (k == 0)
				{
					m_patternMatch.pop_back();
				}
			}
		}
	}
}
// --------------------------------------------------------------------------------------
bool CGameMain::CheckPatternString(const char* text)
{
	char szBuffer[4096];

	strcpy_s(szBuffer, sizeof(szBuffer), text);

	string Source = szBuffer;
	MatchPatternString((byte*)szBuffer);

	if (strcmp(Source.c_str(), szBuffer) == 0)
	{
		return true;
	}
	else
		return false;
}

// --------------------------------------------------------------------------------------
void CGameMain::MatchPatternString(BYTE* text)
{
	const char replaceArray[10] = { '!', '@', '#', '$', '%', '^', '&', '*', '(', ')' };

	BYTE buf[4096];
	int length = 0;
	for (; text[length]; length++) {
		if (text[length] >= 'A' && text[length] <= 'Z')
			buf[length] = text[length] + 32;
		else if (text[length] >= 0xC0 && text[length] <= 0xD6)
			buf[length] = text[length] + 32;
		else if (text[length] >= 0xD8 && text[length] <= 0xDE)
			buf[length] = text[length] + 32;
		else
			buf[length] = text[length];
	}

	if (m_language.compare(LANGUAGE_CODE_DE) == 0 ||
		m_language.compare(LANGUAGE_CODE_ENUS) == 0 ||
		m_language.compare(LANGUAGE_CODE_ENEU) == 0 ||
		m_language.compare(LANGUAGE_CODE_BR) == 0 ||
		m_language.compare(LANGUAGE_CODE_TR) == 0 ||
		m_language.compare(LANGUAGE_CODE_FR) == 0 ||
		m_language.compare(LANGUAGE_CODE_PT) == 0 ||
		m_language.compare(LANGUAGE_CODE_PL) == 0 ||
		m_language.compare(LANGUAGE_CODE_NL) == 0 ||
		m_language.compare(LANGUAGE_CODE_RO) == 0 ||
		m_language.compare(LANGUAGE_CODE_VN) == 0 ||
		m_language.compare(LANGUAGE_CODE_TH) == 0 ||
		m_language.compare(LANGUAGE_CODE_SAES) == 0 ||
		m_language.compare(LANGUAGE_CODE_SAPT) == 0 ||
		m_language.compare(LANGUAGE_CODE_ES) == 0 ||
		m_language.compare(LANGUAGE_CODE_PH) == 0 ||
		m_language.compare(LANGUAGE_CODE_AUS) == 0)
	{
		int iLen = 0;
		int iPosition = 0;
		while ((iLen = SeparateStringA(buf + iPosition, length - iPosition)) > 0)
		{
			for (vector<CRuAlgorithm_PatternMatch_FJS*>::iterator iter = m_patternMatch.begin(); iter != m_patternMatch.end(); iter++) {
				CRuAlgorithm_PatternMatch_FJS* algoFJS = *iter;
				int matchLength = algoFJS->GetPatternLength();;

				if (matchLength == iLen && memcmp(algoFJS->GetPattern(), buf + iPosition, iLen) == 0)
				{
					int matchLength = algoFJS->GetPatternLength();
					for (int n = 0; n < matchLength; n++)
					{
						int index = iPosition + n;
						if (m_language.compare(LANGUAGE_CODE_VN) == 0)
						{
							text[index] = '*';
						}
						else
						{
							text[index] = replaceArray[(unsigned char)text[index] % 10];
						}
					}
				}
			}
			iPosition += iLen;
		}
	}
	else
	{
		for (vector<CRuAlgorithm_PatternMatch_FJS*>::iterator iter = m_patternMatch.begin(); iter != m_patternMatch.end(); iter++) {
			CRuAlgorithm_PatternMatch_FJS* algoFJS = *iter;
			algoFJS->Match((const BYTE*)buf, length);

			for (int i = 0; i < algoFJS->GetNumMatches(); i++) {
				int matchLength = algoFJS->GetPatternLength();
				for (int n = 0; n < matchLength; n++) {
					int index = algoFJS->GetMatchPosition(i) + n;
					if (g_pCoverStringFrame->MatchCoverStringFromOverlap(text, index))
					{
						if (m_language.compare(LANGUAGE_CODE_JP) == 0)
						{
							text[index] = '*';
						}
						else
						{
							text[index] = replaceArray[(unsigned char)text[index] % 10];
						}
					}
				}
			}
		}
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::ReturnCharacterSelect()
{
	LeaveWorld();
	g_pAccountLogin->UpdateRealmList(3.0f);
}

// --------------------------------------------------------------------------------------
bool CGameMain::CheckRightfulName(const char* name)
{
	if (name == NULL || strlen(name) < 4) {
		// ¦WºÙªø«×¹Lµu
		return false;
	}

	int size = 0;
	int index = 0;
	wchar_t wch;

	while (name[index]) {
		if (name[index] > 0 && name[index] < 0x7F) {
			// UTF8-1
			char ch = ::tolower(name[index]);
			if (ch >= '0' && ch <= '9' || ch >= 'a' && ch <= 'z') {
				size += 1;
				index++;
			}
			else {
				// ¤£¦Xªk¦r¤¸
				return false;
			}
			wch = name[index];
		}
		else if ((unsigned char)name[index] >= 0xC2 && (unsigned char)name[index] <= 0xDF) {
			// UTF8-2
			if (MultiByteToWideChar(CP_UTF8, 0, name + index, 2, &wch, 1) <= 0) {
				// ¤£¦Xªk¦r¤¸
				return false;
			}
			size += 2;
			index += 2;
		}
		else if ((unsigned char)name[index] >= 0xE0 && (unsigned char)name[index] <= 0xEF) {
			// UTF8-3
			if (MultiByteToWideChar(CP_UTF8, 0, name + index, 3, &wch, 1) <= 0) {
				// ¤£¦Xªk¦r¤¸
				return false;
			}
			size += 2;
			index += 3;
		}
		else if ((unsigned char)name[index] >= 0xF0 && (unsigned char)name[index] <= 0xF4) {
			// UTF8-4
			if (MultiByteToWideChar(CP_UTF8, 0, name + index, 4, &wch, 1) <= 0) {
				// ¤£¦Xªk¦r¤¸
				return false;
			}
			size += 2;
			index += 4;
		}
		else {
			// ¤£¦Xªk¦r¤¸
			return false;
		}

		// «D¤¤¤å¦r¤¸ 
		if (wch > 0x80 && (wch < 0x4e00 || wch > 0x9fa5)) {
			// ¤£¦Xªk¦r¤¸
			return false;
		}

		if (size > 16) {
			// ¤£¦Xªk¦r¤¸
			return false;
		}
	}

	return true;
}

// --------------------------------------------------------------------------------------
void CGameMain::SaveEnvironmentVariables()
{
	char filename[512];
	const char* savePath = m_interface->GetSavePath();
	if (savePath && strlen(savePath) > 0)
		sprintf(filename, "%s\\%s", savePath, GAME_SETTINGS_FILENAME);
	else
		strcpy(filename, GAME_SETTINGS_FILENAME);

	CIni gameConfig(filename);
	gameConfig.WriteBool(GC_MAINSCTION, GC_MOUSEMOVE, GetMouseMoveEnable());
	gameConfig.WriteBool(GC_MAINSCTION, GC_NPCTITLE, GetNPCTitleVisible());
	gameConfig.WriteBool(GC_MAINSCTION, GC_DISABLETITLEHIDE, GetDisableTitleHide());
	gameConfig.WriteBool(GC_MAINSCTION, GC_PLAYERTITLE, GetPlayerTitleVisible());
	gameConfig.WriteBool(GC_MAINSCTION, GC_SELFTITLE, GetSelfTitleVisible());
	gameConfig.WriteBool(GC_MAINSCTION, GC_SELFCASTSTATE, GetSelfCastState());
	gameConfig.WriteBool(GC_MAINSCTION, GC_TITLEVISIBLE, GetTitleVisible());
	gameConfig.WriteBool(GC_MAINSCTION, GC_GUILDVISIBLE, GetGuildVisible());
	gameConfig.WriteBool(GC_MAINSCTION, GC_TITLEICONVISIBLE, GetTitleIconVisible());
	gameConfig.WriteBool(GC_MAINSCTION, GC_CAMERAREVERSE, GetCameraYReverse());
	gameConfig.WriteBool(GC_MAINSCTION, GC_LBUTTONCANCELTARGET, GetLButtonCancelTarget());
	gameConfig.WriteBool(GC_MAINSCTION, GC_RBUTTONCANCELTARGET, GetRButtonCancelTarget());
	gameConfig.WriteBool(GC_MAINSCTION, GC_LBUTTONCAMERAROTATEENABLE, GetLButtonCameraRotateEnable());
	gameConfig.WriteBool(GC_MAINSCTION, GC_CAMERAFOLLOWENABLE, GetCameraFollowEnable());
	gameConfig.WriteBool(GC_MAINSCTION, GC_DISABLEDISPLAYNPCTALK, GetDisableDisplayNPCTalk());
	gameConfig.WriteBool(GC_MAINSCTION, GC_CAMERASELECTTARGET, GetCameraSelectTarget());

	gameConfig.WriteBool(GC_MAINSCTION, GC_PLAYLOOTMOTION, GetLootPlayerMotion());
	gameConfig.WriteBool(GC_MAINSCTION, GC_CHATDISPLAYCLASSCOLOR, GetChatDisplayClassColor());
	gameConfig.WriteBool(GC_MAINSCTION, GC_AUTOTAKELOOT, GetAutoTakeLoot());
	gameConfig.WriteBool(GC_MAINSCTION, GC_AGGROPROMPT, GetAggroPrompt());
	gameConfig.WriteBool(GC_MAINSCTION, GC_AUTOOPENGOODSPACK, GetAutoOpenGoodsPack());
	gameConfig.WriteBool(GC_MAINSCTION, GC_HIDEOTHERPLAYEREFFECT, GetHideOtherPlayerEffect());

	//gameConfig.WriteBool(GC_MAINSCTION, GC_SERVERSAVECLIENTDATA,	GetServerSaveClientData());


	//blood bar
	if (g_pObjectBloodBar)
	{
		bool visible = g_pObjectBloodBar->GetBloodBar();
		int visibleFlag = g_pObjectBloodBar->GetVisable();
		int distance = g_pObjectBloodBar->GetDistance();
		gameConfig.WriteBool(GC_BLOODBAR, GC_BLOODBAR, visible);
		gameConfig.WriteBool(GC_BLOODBAR, GC_NPCBLOODBAR, visibleFlag & OBB_NPC_FLAG);
		gameConfig.WriteBool(GC_BLOODBAR, GC_PCBLOODBAR, visibleFlag & OBB_PLAYER_FLAG);
		gameConfig.WriteBool(GC_BLOODBAR, GC_MOBBLOODBAR, visibleFlag & OBB_EMENY_FLAG);
		gameConfig.WriteBool(GC_BLOODBAR, GC_SELFBLOODBAR, visibleFlag & OBB_SELF_FLAG);
		gameConfig.WriteInt(GC_BLOODBAR, GC_OBB_DISTANCE, distance);
		gameConfig.WriteBool(GC_BLOODBAR, GC_OBB_TITLE, visibleFlag & OBB_TITLE_FLAG);

		gameConfig.WriteBool(GC_MAINSCTION, GC_ALLSCTVISIBLE, GetDisplayAllSCT());
		gameConfig.WriteBool(GC_MAINSCTION, GC_LASTSELECTEDREALM, GetLastRealmState());
		gameConfig.WriteBool(GC_MAINSCTION, GC_LASTACCOUNTNAME, GetLastAccountNameState());

	}

	// Àx¦s±b¸¹¦WºÙ
	gameConfig.WriteString(GC_MAINSCTION, GC_ACCOUNTNAME, g_pAccountLogin->GetAccountName());

	const char* lastPlayerName = g_pAccountLogin->GetSelectedPlayerName();
	if (strlen(lastPlayerName) > 0)
	{
		// Àx¦s³Ì«áµn¤J¨¤¦â¦WºÙ
		gameConfig.WriteString(GC_MAINSCTION, GC_LASTPLAYERNAME, lastPlayerName);
	}

	SaveClientSetting(true);

	if (m_interface)
	{
		m_interface->SaveClientData();
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::LoadEnvironmentVariables()
{
	char filename[512];
	const char* savePath = NULL;
	CInterface* pInterface = GetInterface();
	if (pInterface)
		savePath = pInterface->GetSavePath();

	if (savePath && strlen(savePath) > 0)
		sprintf(filename, "%s\\%s", savePath, GAME_SETTINGS_FILENAME);
	else
		strcpy(filename, GAME_SETTINGS_FILENAME);

	if (CheckCountry("JP"))	//if ( m_country.compare("JP") == 0 )
	{
		SetSelfCastState(true);
	}

	CIni gameConfig(filename);
	SetMouseMoveEnable(gameConfig.GetBool(GC_MAINSCTION, GC_MOUSEMOVE, GetMouseMoveEnable()));
	SetNPCTitleVisible(gameConfig.GetBool(GC_MAINSCTION, GC_NPCTITLE, GetNPCTitleVisible()));
	SetDisableTitleHide(gameConfig.GetBool(GC_MAINSCTION, GC_DISABLETITLEHIDE, GetDisableTitleHide()));
	SetPlayerTitleVisible(gameConfig.GetBool(GC_MAINSCTION, GC_PLAYERTITLE, GetPlayerTitleVisible()));
	SetSelfCastState(gameConfig.GetBool(GC_MAINSCTION, GC_SELFCASTSTATE, GetSelfCastState()));
	SetSelfTitleVisible(gameConfig.GetBool(GC_MAINSCTION, GC_SELFTITLE, GetSelfTitleVisible()));
	SetDisplayAllSCT(gameConfig.GetBool(GC_MAINSCTION, GC_ALLSCTVISIBLE, GetDisplayAllSCT()));
	SetLastRealmState(gameConfig.GetBool(GC_MAINSCTION, GC_LASTSELECTEDREALM, GetLastRealmState()));
	SetLastAccountNameState(gameConfig.GetBool(GC_MAINSCTION, GC_LASTACCOUNTNAME, GetLastAccountNameState()));
	SetShowGemePromrt(gameConfig.GetBool(GC_MAINSCTION, GC_SHOWGEMEPROMRT, GetShowGemePromrt()));
	SetTitleVisible(gameConfig.GetBool(GC_MAINSCTION, GC_TITLEVISIBLE, GetTitleVisible()));
	SetGuildVisible(gameConfig.GetBool(GC_MAINSCTION, GC_GUILDVISIBLE, GetGuildVisible()));
	SetTitleIconVisible(gameConfig.GetBool(GC_MAINSCTION, GC_TITLEICONVISIBLE, GetTitleIconVisible()));
	SetCameraYReverse(gameConfig.GetBool(GC_MAINSCTION, GC_CAMERAREVERSE, GetCameraYReverse()));
	SetLButtonCancelTarget(gameConfig.GetBool(GC_MAINSCTION, GC_LBUTTONCANCELTARGET, GetLButtonCancelTarget()));
	SetRButtonCancelTarget(gameConfig.GetBool(GC_MAINSCTION, GC_RBUTTONCANCELTARGET, GetRButtonCancelTarget()));
	SetLButtonCameraRotateEnable(gameConfig.GetBool(GC_MAINSCTION, GC_LBUTTONCAMERAROTATEENABLE, GetLButtonCameraRotateEnable()));
	SetCameraFollowEnable(gameConfig.GetBool(GC_MAINSCTION, GC_CAMERAFOLLOWENABLE, GetCameraFollowEnable()));
	SetDisableDisplayNPCTalk(gameConfig.GetBool(GC_MAINSCTION, GC_DISABLEDISPLAYNPCTALK, GetDisableDisplayNPCTalk()));
	SetCameraSelectTarget(gameConfig.GetBool(GC_MAINSCTION, GC_CAMERASELECTTARGET, GetCameraSelectTarget()));

	SetLootPlayerMotion(gameConfig.GetBool(GC_MAINSCTION, GC_PLAYLOOTMOTION, GetLootPlayerMotion()));
	SetChatDisplayClassColor(gameConfig.GetBool(GC_MAINSCTION, GC_CHATDISPLAYCLASSCOLOR, GetChatDisplayClassColor()));
	SetAutoTakeLoot(gameConfig.GetBool(GC_MAINSCTION, GC_AUTOTAKELOOT, GetAutoTakeLoot()));
	SetAggroPrompt(gameConfig.GetBool(GC_MAINSCTION, GC_AGGROPROMPT, GetAggroPrompt()));
	SetAutoOpenGoodsPack(gameConfig.GetBool(GC_MAINSCTION, GC_AUTOOPENGOODSPACK, GetAutoOpenGoodsPack()));
	SetHideOtherPlayerEffect(gameConfig.GetBool(GC_MAINSCTION, GC_HIDEOTHERPLAYEREFFECT, GetHideOtherPlayerEffect()));

	//SetServerSaveClientData	( gameConfig.GetBool(GC_MAINSCTION, GC_SERVERSAVECLIENTDATA,	GetServerSaveClientData()) );

	if (g_pObjectBloodBar)
	{
		// get blood bar def
		bool visible = g_pObjectBloodBar->GetBloodBar();
		int visibleFlag = g_pObjectBloodBar->GetVisable();
		int distance = g_pObjectBloodBar->GetDistance();
		int newFlag = 0;
		// read ini
		visible = gameConfig.GetBool(GC_BLOODBAR, GC_BLOODBAR, visible);
		if (gameConfig.GetBool(GC_BLOODBAR, GC_NPCBLOODBAR, visibleFlag & OBB_NPC_FLAG))
			newFlag |= OBB_NPC_FLAG;
		if (gameConfig.GetBool(GC_BLOODBAR, GC_PCBLOODBAR, visibleFlag & OBB_PLAYER_FLAG))
			newFlag |= OBB_PLAYER_FLAG;
		if (gameConfig.GetBool(GC_BLOODBAR, GC_MOBBLOODBAR, visibleFlag & OBB_EMENY_FLAG))
			newFlag |= OBB_EMENY_FLAG;
		if (gameConfig.GetBool(GC_BLOODBAR, GC_SELFBLOODBAR, visibleFlag & OBB_SELF_FLAG))
			newFlag |= OBB_SELF_FLAG;
		if (gameConfig.GetBool(GC_BLOODBAR, GC_OBB_TITLE, visibleFlag & OBB_TITLE_FLAG))
			newFlag |= OBB_TITLE_FLAG;

		distance = gameConfig.GetInt(GC_BLOODBAR, GC_OBB_DISTANCE, distance);

		//set Blood Bar
		g_pObjectBloodBar->SetBloodBar(visible);
		g_pObjectBloodBar->SetVisable(newFlag);
		g_pObjectBloodBar->SetDistance(distance);
	}

	// ³Ì«áµn¤J¦øªA¾¹
	char tmp[512];
	if (GetLastAccountNameState()) {
		gameConfig.GetString(GC_MAINSCTION, GC_ACCOUNTNAME, tmp, 512, "");
	}
	else {
		memset(tmp, 0, sizeof(tmp));
	}
	g_pAccountLogin->SetAccountName(tmp);

	// ³Ì«á¿ï¾Üµn¤J¨¤¦â
	{
		gameConfig.GetString(GC_MAINSCTION, GC_LASTPLAYERNAME, tmp, 512, "");
		g_pAccountLogin->SetSelectedPlayerName(tmp);
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::SetCombatState(bool combatState)
{
	if (m_isCombatState != combatState)
	{
		m_isCombatState = combatState;
		if (m_pPlayer)
		{
			SCTInfo info;
			info.source = m_pPlayer->GetIdentify();
			info.target = m_pPlayer->GetIdentify();

			if (m_isCombatState)
				info.type = eSCT_Combat_Enter;
			else
				info.type = eSCT_Combat_Leave;
			g_pScrollCombatTextFrame->PushSCTInfo(info);
		}

		if (m_isCombatState == false)
		{
			CancelAttack();

			// ¾Ô°«µ²§ô­«·s¶}±Ò§ðÀ»
			if (m_petGUID != 0)
			{
				NetCli_Magic::PetCommand(m_petGUID, EM_PET_ATTACK, -1, 0);
			}
		}
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::AttackTarget(CRoleSprite* target)
{
	CRoleSprite* player = m_pPlayer;

	if (target == NULL)
		target = GetPlayerTarget();

	if (target && player)
	{
		// Âê©w¤£¯à§ðÀ»
		switch (player->GetActorState())
		{
		case ruFUSION_ACTORSTATE_SIT_BEGIN:
		case ruFUSION_ACTORSTATE_SIT_LOOP:
		case ruFUSION_ACTORSTATE_SLEEP_BEGIN:
		case ruFUSION_ACTORSTATE_SLEEP_LOOP:
		case ruFUSION_ACTORSTATE_CROUCH_BEGIN:
		case ruFUSION_ACTORSTATE_CROUCH_LOOP:
			break;
		}

		m_attackWorldGUID = target->GetWorldGUID();
		NetCli_Attack::S_AttackSignal(target->GetIdentify());

		if (m_pPlayer)
			m_pPlayer->SetCombatState(true);

		//g_pActionFrame->UpdateSkillAction(DF_SKILLID_ATTACK);
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::CancelAttack()
{
	if (m_attackWorldGUID != 0)
	{
		m_attackWorldGUID = 0;
		NetCli_Attack::S_EndAttackSignal();
		if (m_pPlayer)
			m_pPlayer->SetCombatState(false);

		//g_pActionFrame->UpdateSkillAction(DF_SKILLID_ATTACK);
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::SetPlayerAction(int action)
{
	/*
	if ( m_pPlayer && SUCCEEDED( m_pPlayer->SetAction(action) ) )
	{
	switch ( action )
	{
	case eAction_Idle:
	case eAction_Fight_Idle:
	case eAction_Sit:
	case eAction_StandUp:
	case eAction_Sleep:
	case eAction_Holster:
	case eAction_Unholster:
	SendDoAction( action, m_pPlayer->GetStateFlag() );
	break;
	}
	}
	*/
}

// --------------------------------------------------------------------------------------
void CGameMain::Jump()
{
	if (m_pPlayer)
	{
		CRoleSprite* controlSprite = m_pPlayer->GetProxyControlSprite();
		SpriteSpecialEffect effect = m_pPlayer->GetSpecailEffect();

		if (m_isFreeCamera)
		{
			return;
		}

		if (effect.lockedMove > 0 || effect.fear > 0)
			return;

		float height = 0.0f;
		CRuVector3 nowPos = controlSprite->GetPos();
		if (m_pPlayer->GetAboveWaterState())
		{
		}
		else if (!IBaseSprite::TerrainTestHight(nowPos, height, 100) || (nowPos.m_y - height) > 5.0f)
			return;

		/*
		controlSprite->JumpEx();
		return;
		*/

		if (!controlSprite->Jump())
			return;

		CRuVector3 pos = controlSprite->GetPos();
		CRuVector3 dir = controlSprite->GetDirection();
		CRuVector3 vec = controlSprite->GetLatentVelocity();

		m_firstJump = true;
		m_moveVector = vec;
		m_direction = dir;
		m_position = pos;
		m_sendMoveTime = 0.0f;

		// ±N¥Ø¼Ð¦V¶qÂà´«¬°¨¤¦âZ¶b¦V¶q
		CRuQuaternion quaternion;
		quaternion.RotateIntoVector(CRuVector3(0.0f, 0.0f, 1.0f), dir);
		quaternion.TransformVector(m_moveVector);

		// ¨ú±o¨¤¦â©Ò´Â¤è¦V
		float theta = acos(dir.Dot(CRuVector3(1.0f, 0.0f, 0.0f)));
		if (dir.m_z > 0.0f)
			theta = PI + PI - theta;

		// Âà´«¬°¨¤«×
		theta = theta * 180.0f / PI;
		Net_Move::S_JumpBegin(m_pPlayer->GetIdentify(), pos.m_x, pos.m_y, pos.m_z, theta, vec.m_x, vec.m_y, vec.m_z);

		//g_pError->AddMessage(0, 0, "S_JumpBegin pos(%.2f %.2f %.2f) angle(%.2f) vetor(%.2f %.2f %.2f)", pos.m_x, pos.m_y, pos.m_z, theta, vec.m_x, vec.m_y, vec.m_z);

		if (vec.m_x != 0.0f || vec.m_z != 0.0f)
			SendPlayerPos(true);

		// g_pError->AddMessage(0, 0, "S_JumpBegin pos(%.2f %.2f %.2f) angle(%.2f) vetor(%.2f %.2f %.2f)", pos.m_x, pos.m_y, pos.m_z, theta, vec.m_x, vec.m_y, vec.m_z);

		// ¤¤Â_¨¤¦â¬Iªk
		if (ItemCastingEnable()) {
			ItemCastingInterrupt();
		}
		else if (g_pTeleportBook->BreakCasting()) {
		}

		// ¤¤Â_¨¤¦â¬Iªk
		// CancelCastSpell();
		// g_pDebugMessage->DebugOutput(0, "Jump (%.2f %.2f %.2f) (%.2f, %.2f,%.2f)", pos.m_x, pos.m_y, pos.m_z, vec.m_x, vec.m_y, vec.m_z);

	}
}

// --------------------------------------------------------------------------------------
void CGameMain::DoEmotion(CRuFusion_ActorState emotion)
{
	if (m_pPlayer)
	{
		m_pPlayer->SetActorState(emotion);
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::TranslateBaseValue(CRoleSprite* sprite, int mp, int sp, int sub_sp, int* outValue)
{
	if (sprite == NULL || outValue == NULL)
		return;

	int valueType[2] = { -1, -1 };
	int jobType[2] = { sprite->GetVocation(), sprite->GetVocationSub() };

	for (int i = 0; i < 2; i++)
	{
		outValue[i] = 0;
		switch (jobType[i])
		{
		case EM_Vocation_Warrior:
		case EM_Vocation_Psyron:
			valueType[i] = EM_MagicAttackType_SP_Warrior;
			outValue[i] = (i == 0) ? sp : sub_sp;
			break;

		case EM_Vocation_Ranger:
		case EM_Vocation_Harpsyn:
			valueType[i] = EM_MagicAttackType_SP_Ranger;
			outValue[i] = (i == 0) ? sp : sub_sp;
			break;

		case EM_Vocation_Rogue:
		case EM_Vocation_Duelist:
			valueType[i] = EM_MagicAttackType_SP_Rogue;
			outValue[i] = (i == 0) ? sp : sub_sp;
			break;

		case EM_Vocation_GameMaster:
		case EM_Vocation_Wizard:
		case EM_Vocation_Priest:
		case EM_Vocation_Knight:
		case EM_Vocation_RuneDancer:
		case EM_Vocation_Druid:
		case EM_Vocation13:
			valueType[i] = EM_MagicAttackType_MP;
			outValue[i] = mp;
			break;
		}
	}

	// ¦pªG¥D°ÆÂ¾¬Û¦P±N²Ä¤G­Ó§Þ¯à­È²M°£
	if (valueType[0] == valueType[1]) {
		outValue[1] = 0;
	}
}
// --------------------------------------------------------------------------------------
void CGameMain::ReceivePing(int iTime)
{
	m_pingState = false;
	//m_ping = GetTickCount() - iTime; // (int)(m_frameTime * 1000) - m_startPing;
}

// --------------------------------------------------------------------------------------
void CGameMain::WorldToViewport(CRuVector3& point)
{
	g_ruUIRenderer->WorldToViewport(m_pCamera, point);
}

// --------------------------------------------------------------------------------------
void CGameMain::WorldToScreen(CRuVector3& point)
{
	g_ruUIRenderer->WorldToScreen(m_pCamera, point);
}

// --------------------------------------------------------------------------------------
void CGameMain::SetCursor(CursorType type, const char* dragItem)
{
	const char* cursor = NULL;
	if (type == m_cursorType && type != eCursor_DragItem)
		return;

	if (dragItem == NULL)
		dragItem = "";

	switch (type)
	{
	case eCursor_Null:
		cursor = "";
		break;
	case eCursor_DragItem:
		if (strcmp(dragItem, m_dragItemStr.c_str()) == 0)
			return;
		cursor = "Interface\\Cursor\\Item";
		break;
	case eCursor_Point:
		cursor = "Interface\\Cursor\\Point";
		break;
	case eCursor_Point2:
		cursor = "Interface\\Cursor\\Point2";
		break;
	case eCursor_Rune:
		cursor = "Interface\\Cursor\\rune";
		break;
	case eCursor_Dissolution:
		cursor = "Interface\\Cursor\\Dissolution";
		break;
	case eCursor_Repair:
		cursor = "Interface\\Cursor\\repair";
		break;
	case eCursor_UnableRepair:
		cursor = "Interface\\Cursor\\unablerepair";
		break;
	case eCursor_Cast_Phy:
		cursor = "Interface\\Cursor\\Cast_phy";
		break;
	case eCursor_Cast_Magic:
		cursor = "Interface\\Cursor\\Cast_magic";
		break;
	case eCursor_Enhance:
		cursor = "Interface\\Cursor\\Enhance";
		break;
	case eCursor_Brush:
		cursor = "Interface\\Cursor\\Brush";
		break;
	case eCursor_Mail:
		cursor = "Interface\\Cursor\\mail";
		break;
	case eCursor_Attack:
		cursor = "Interface\\Cursor\\attack";
		break;
	case eCursor_UnableAttack:
		cursor = "Interface\\Cursor\\unableattack";
		break;
	case eCursor_Interact:
		cursor = "Interface\\Cursor\\interact";
		break;
	case eCursor_UnableInteract:
		cursor = "Interface\\Cursor\\unableinteract";
		break;
	case eCursor_Speak:
		cursor = "Interface\\Cursor\\speak";
		break;
	case eCursor_UnableSpeak:
		cursor = "Interface\\Cursor\\unablespeak";
		break;
	case eCursor_Mine:
		cursor = "Interface\\Cursor\\mine";
		break;
	case eCursor_UnableMine:
		cursor = "Interface\\Cursor\\unablemine";
		break;
	case eCursor_Herb:
		cursor = "Interface\\Cursor\\herb";
		break;
	case eCursor_UnableHerb:
		cursor = "Interface\\Cursor\\unableherb";
		break;
	case eCursor_Fell:
		cursor = "Interface\\Cursor\\fell";
		break;
	case eCursor_Unablefell:
		cursor = "Interface\\Cursor\\unablefell";
		break;
	case eCursor_Fishing:
		cursor = "Interface\\Cursor\\fishing";
		break;
	case eCursor_UnableFishing:
		cursor = "Interface\\Cursor\\unablefishing";
		break;
	case eCursor_Shop:
		cursor = "Interface\\Cursor\\shop";
		break;
	case eCursor_UnableShop:
		cursor = "Interface\\Cursor\\unableshop";
		break;
	case eCursor_Locked:
		cursor = "Interface\\Cursor\\picklock";
		break;
	case eCursor_Unlocked:
		cursor = "Interface\\Cursor\\unablepicklock";
		break;
	case eCursor_FurnitureMove:
		cursor = "Interface\\Cursor\\FurnitureMove";
		break;
	case eCursor_FurnitureRorare:
		cursor = "Interface\\Cursor\\FurnitureRorare";
		break;
	case eCursor_Restore:
		cursor = "Interface\\Cursor\\restore";
		break;
	case eCursor_Protect:
		cursor = "Interface\\Cursor\\Protect";
		break;
	case eCursor_IgonreLevel:
		cursor = "Interface\\Cursor\\IgonreLevel";
		break;
	case eCursor_Pickup:
		cursor = "Interface\\Cursor\\Pickup";
		break;
	case eCursor_Hyperlink:
		cursor = "Interface\\Cursor\\Hyperlink";
		break;
	case eCursor_Egg:
		cursor = "Interface\\Cursor\\egg";
		break;
	case eCursor_inspect:
		cursor = "Interface\\Cursor\\inspect";
		break;
	case eCursor_unableinspect:
		cursor = "Interface\\Cursor\\unableinspect";
		break;
	case eCursor_UnableMail:
		cursor = "Interface\\Cursor\\unablemail";
		break;
	case eCursor_Sit:
		cursor = "Interface\\Cursor\\sit";
		break;

	case eCursor_UnableSit:
		cursor = "Interface\\Cursor\\unablesit";
		break;

	case eCursor_OpenSound:
		cursor = "Interface\\Cursor\\opensound";
		break;

	case eCursor_CloseSound:
		cursor = "Interface\\Cursor\\closesound";
		break;

	case eCursos_Implement:
		cursor = "Interface\\Cursor\\Implement";
		break;

	case eCursos_UnableImplement:
		cursor = "Interface\\Cursor\\Implement";
		break;
	}

	m_cursorType = type;
	m_dragItemStr = dragItem;

	if (m_dragItemStr.empty())
	{
		SetCursorIcon(cursor);
		ClearDragCursorIcon();
	}
	else
	{
		SetCursorIcon(m_dragItemStr.c_str());
		SetDragCursorIcon(cursor);
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::PlayerDead(bool selfRevive, int resurrectTimeRemaining)
{
	// ¸Ñ°£§¤ÃMª¬ºA
	g_pGameControl->LeaveMount();

	// ¸Ñ°£©¹«e¶]
	ToggleAutoRune(2);

	// ¸Ñ°£¾Ô°«¤¤
	SetCombatState(false);

	// ²M°£¥Ø¼Ð
	SetPlayerTarget(NULL);

	ClearPathMove();
	ClearMoveToPoint();

	m_selfRevive = selfRevive;
	m_resurrectTime = (float)resurrectTimeRemaining;

	if (m_interface)
	{
		m_interface->SendWorldEvent("WARNING_STOP");				// ²M°£¦º¤`°Ï°ì
		m_interface->SendWorldEvent("PLAYER_DEAD");
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::ResurrectRequest(int source, int magicBaseID)
{
	m_resurrect = true;

	if (m_interface)
	{
		CRoleSprite* sprite = FindSpriteByID(source);
		lua_State* L = m_interface->GetLuaState();
		const char* name = "";
		if (sprite)
			name = sprite->GetName();

		if (L)
		{
			lua_pushstring(L, name);
			lua_setglobal(L, "arg1");
			m_interface->SendWorldEvent("RESURRECT_REQUEST");
		}
	}
}

// --------------------------------------------------------------------------------------
bool CGameMain::TestHeight(CRuVector3 position, float& outHeight)
{
	CRuCollisionQuery_Segment_Nearest segQuery;

	for (int i = 1; i <= 3; i++) {
		float distance = (float)pow(15.0, (double)i);

		// ³]©w´ú¸Õ¥Î½u¬q¤Î¨ä¥L³]©w
		segQuery.m_colSegment = CRuSegment3(position, position + CRuVector3(0.0, -distance, 0.0f));
		segQuery.m_exclusionMask = RuWorld::ruTRIFLAGS_WATER | RuWorld::ruTRIFLAGS_FOLIAGE;
		segQuery.m_ignoreBackface = FALSE;
		segQuery.m_ignoreCollisionFlag = TRUE;

		IBaseSprite::GetCollisionContainer()->QueryCollision(&segQuery);
		if (segQuery.m_positiveHit) {
			outHeight = segQuery.m_collisionPoint.m_y;
			return true;
		}

		// ³]©w´ú¸Õ¥Î½u¬q¤Î¨ä¥L³]©w
		segQuery.m_colSegment = CRuSegment3(position, position + CRuVector3(0.0, distance, 0.0f));
		segQuery.m_exclusionMask = RuWorld::ruTRIFLAGS_WATER | RuWorld::ruTRIFLAGS_FOLIAGE;
		segQuery.m_ignoreBackface = FALSE;
		segQuery.m_ignoreCollisionFlag = TRUE;

		IBaseSprite::GetCollisionContainer()->QueryCollision(&segQuery);
		if (segQuery.m_positiveHit) {
			outHeight = segQuery.m_collisionPoint.m_y;
			return true;
		}
	}

	return false;
}

// --------------------------------------------------------------------------------------
void CGameMain::SetPlayerCameraPos(CRuVector3& pos)
{
#ifdef KALYDO
	if (m_needLoadTerrain && m_isDownloading)
	{
		// postpone this message until the map is downloaded.
		m_cameraPos = pos;
		m_shouldSetCameraPos = true;
		return;
	}
#endif // KALYDO
	// g_pError->AddMessage(0, 0, "[SetCameraPos] Set Player's Pos (%.2f %.2f %.2f)", pos.m_x, pos.m_y, pos.m_z);	

	m_changeTerrainPos = pos;
	m_receiveWaitLoading = true;
	m_sendLoadOkDelayTime = 1.0f;

	// ³]©w´å¼Ð¹Ï§Î
	SetCursor(eCursor_Point);

	// ²M°£·Æ¹«²¾°Ê
	ClearMoveToPoint();
	// ²M°£¸ô®|²¾°Ê
	ClearPathMove();

	if (m_needLoadTerrain)
	{
		m_needLoadTerrain = false;

		// ©µ¿ð¦a¹Ï¸ü¤J
		m_loadTerrainDelayTime = 1.0f;

		//§PÂ_¬O§_¬OÂ÷¶} ©Ð«Î
		g_pHousesFrame->LeaveWorld();

		//§PÂ_¬O§_¬OÂ÷¶} ©Ð«Î
		g_pGuildHousesFrame->LeaveWorld();

		// Åã¥Ü¸ü¤Jµe­±
		if (g_pLoadingFrame)
			g_pLoadingFrame->LoadingStart();
	}
	else if (m_pRuTerrain)
	{
		MovePlayerPos(pos);

		m_pRuTerrain->SetCamera(m_pCamera);
		m_pRuTerrain->Update(0.01f);

		//g_pError->AddMessage(0, 0, "Terrain Loading Status %.4f", m_pRuTerrain->GetLoadingStatus());

		RuPropertyType propertyValue;
		REAL viewDistance = 0.0f;

		REAL distance = 0.0f;
		if (m_pPlayer)
			distance = (m_pPlayer->GetPos() - pos).Magnitude();

		if (g_ruEngineSettings->GetSetting("world:view distance", propertyValue))
		{
			viewDistance = boost::get<REAL>(propertyValue);
		}

		if (m_pRuTerrain->GetLoadingStatus() < 0.75f || distance > viewDistance * 2)
		{
			// Åã¥Ü¸ü¤Jµe­±
			if (g_pLoadingFrame)
				g_pLoadingFrame->LoadingStart();

			// ¦aªí¸ê°T¤w¸ü¤J
			m_loadingTerrain = true;
		}
		else
		{
			// ¥[³t¦aªíÅª¨ú
			g_ruEngineSettings->SetSetting("system:chunk dequeue rate", 100.0f);
			g_ruEngineSettings->SetSetting("system:backdrop dequeue rate", 100.0f);

			// Give global worker thread maximum allocation
			g_ruGlobalWorkerThread->SetSleepPeriod(0);
			g_ruResourceManager->SetObjectDequeueRate(100.0f);
			g_ruResourceManager->SetTextureDequeueRate(100.0f);

			while (m_pRuTerrain->GetLoadingStatus() < 1.0f)
			{
				// Update global function schedule
				g_ruFunctionSchedule->Update();

				m_pRuTerrain->SetCamera(m_pCamera);
				m_pRuTerrain->Update(0.01f);
				Sleep(10);
			}

			m_receiveWaitLoading = false;
			m_sendLoadOkDelayTime = 0.0f;
			Net_Move::S_ClientLoadingOK();

			// ¦^´_¥¿±`Åª¨ú³t«×
			g_ruEngineSettings->SetSetting("system:chunk dequeue rate", 8.0f);
			g_ruEngineSettings->SetSetting("system:backdrop dequeue rate", 16.0f);

			// Restore global worker thread default allocation
			g_ruGlobalWorkerThread->SetSleepPeriod(25);
			g_ruResourceManager->SetObjectDequeueRate(1.0f);
			g_ruResourceManager->SetTextureDequeueRate(1.0f);
		}
	}
	else {
		// ¥¼¸ü¤J¦aªí
		assert(false);

	}

	// m_changeTerrain = false;
}

// --------------------------------------------------------------------------------------
int CGameMain::CanUseSkill(int skillID, int* outNeedType, int* outNeedValue)
{
	bool				bSpecMagic = false;
	GameObjDbStructEx* itemDB = CNetGlobal::GetObj(skillID);
	RoleDataEx* roleData = CNetGlobal::RoleData();

	// ¹ï¶HÀË´ú
	CRoleSprite* player = GetPlayer();
	CRoleSprite* target = (player) ? player->GetTargetSprite() : NULL;

	if (player && player->IsCantCtrl())
		return CASTSPELL_ERROR_CANTCTRL;	// µLªk±±¨î (¤ß±±¤¤


	if (skillID == DF_SKILLID_ATTACK)			// ¤@¯ë§ðÀ»
		return 0;

	if (itemDB == NULL)
		return 0;

	// ¨¤¦â¸ê°T¤£¦s¦b
	if (roleData == NULL)
		return CASTSPELL_ERROR_EXIST;

	if (itemDB->Classification == EM_ObjectClass_KeyItem)
	{
		switch (skillID)
		{
		case DF_SKILLID_ATTACK: // ¤@¯ë§ðÀ»;
		case DF_SKILLID_DISENCHANT: // ¤À¸Ñ
		case DF_SKILLID_PK:			// PK
			return 0;
		case DF_SKILLID_PORTOL:
		{
			int iMagicID = CNetGlobal::GetKeyMagicID((skillID - Def_ObjectClass_KeyItem));
			if (iMagicID != 0)
			{
				itemDB = CNetGlobal::GetObj(iMagicID);
				bSpecMagic = true;
			}
		}break;
		default:
		{
			// ¥Í¬¡§Þ¯à
			if (skillID >= (Def_ObjectClass_KeyItem + EM_SkillValueType_BlackSmith) && skillID <= (Def_ObjectClass_KeyItem + EM_SkillValueType_Herblism))
			{
				return 0;
			}

			// ·s¤â¶Ç°e³N
			if (skillID >= DF_SKILLID_BEGINNERPORTOL_BEGIN && skillID <= DF_SKILLID_BEGINNERPORTOL_END)
			{
				int iMagicID = CNetGlobal::GetKeyMagicID((skillID - Def_ObjectClass_KeyItem));
				if (iMagicID != 0)
				{
					itemDB = CNetGlobal::GetObj(iMagicID);
					bSpecMagic = true;
				}
			}
		}
		}
	}

	if (itemDB == NULL || itemDB->Classification != EM_ObjectClass_MagicCollect)
		return CASTSPELL_ERROR_EXIST;			// ªk³N¤£¦s¦b©ÎªÌ«Dªk³NÃþ«¬

	if (itemDB->MagicCol.EffectType == EM_Magic_Eq)
		return CASTSPELL_ERROR_ACTIVE;			// ³Q°Êªk³N¤£¯à¬Iªk

	if (player && player->IsDisableJobSkill())
		return CASTSPELL_ERROR_DISABLEJOBSKILL;	// µLªk¨Ï¥ÎÂ¾·~§Þ¯à

	// ÀË¬dÂ¾·~Ãþ«¬
	int skillIndex = -1;
	for (int i = 0; i < _MAX_SPSkill_COUNT_ + _MAX_NormalSkill_COUNT_ + _MAX_NormalSkill_COUNT_; i++)
	{
		if (roleData->PlayerTempData->Skill.AllSkill[i] == skillID)
		{
			skillIndex = i;
			break;
		}
	}

	//ºÙ¸¹§Þ¯à
	if (skillIndex == -1 && bSpecMagic == false)
	{
		GameObjDbStructEx* pTitleObj = CNetGlobal::GetObj(CNetGlobal::RoleData()->BaseData.TitleID);
		if (pTitleObj)
		{
			if (pTitleObj->Title.SkillID == skillID)
			{
				bSpecMagic = true;
			}
		}
	}

	// ®M¸Ë§Þ¯à
	if (skillIndex == -1 && bSpecMagic == false && roleData->IsEquipSuitSkill(skillID))
	{
		bSpecMagic = true;
	}

	//¤ÛÆF²Õ¦X§Þ¯à
	if (skillIndex == -1 && bSpecMagic == false)
	{
		//key = ¤jid + ¤pid * 0x100000000
		int EQ1 = roleData->PlayerBaseData->Phantom.EQ[0];
		int EQ2 = roleData->PlayerBaseData->Phantom.EQ[1];

		INT64 TableID = 0;
		if (EQ1 > EQ2)
		{
			TableID = EQ1 + EQ2 * 0x100000000;
		}
		else
		{
			TableID = EQ2 + EQ1 * 0x100000000;
		}

		if (skillID == RoleDataEx::PhantomSkill[TableID])
		{
			bSpecMagic = true;
		}
	}

	if (skillIndex == -1 && bSpecMagic == false)
	{
		return CASTSPELL_ERROR_CLASS;					// «D¥Ø«eÂ¾·~¥i¥H¨Ï¥Î
	}

	// §Þ¯àµ¥¯Å¬O§_¨¬°÷
	if (itemDB->MagicCol.LimitLv > roleData->TempData.Attr.Level)
	{
		return CASTSPELL_ERROR_LEVEL;					// µ¥¯Å¤£¨¬
	}

	/*
	if ( roleData->GetSpellTime(itemDB) > 0.0f && m_pPlayer && m_pPlayer->GetMoveTime() > 0.0f )
	return CASTSPELL_ERROR_MOVE;
	*/

	// ®ø¯Ó
	for (int i = 0; i < 2; i++)
	{
		if (itemDB->MagicCol.Cost[i].Value > 0)
		{
			int value = itemDB->MagicCol.Cost[i].Value;
			int skillPoint = (int)(CNetGlobal::RoleData()->GetSkillPoint(itemDB->GUID));
			ItemFieldStruct* itemField;
			switch (itemDB->MagicCol.Cost[i].Type)
			{
			case EM_MagicSpellCostType_HP:
				if (roleData->BaseData.HP <= value)
					return CASTSPELL_ERROR_HP;				// HP¤£¨¬
				break;

			case EM_MagicSpellCostType_MP:
			{
				float TempValue = (float)value;
				if (RoleDataEx::MPCostPowerRate.size() > (unsigned)skillPoint)
					TempValue = TempValue * RoleDataEx::MPCostPowerRate[skillPoint];
				if (roleData->BaseData.MP < TempValue * (100 - roleData->TempData.Attr.ManaDecrease()) / 100)
					return CASTSPELL_ERROR_MP;				// MP¤£¨¬
			}
			break;

			case EM_MagicSpellCostType_SP_Warrior:
				if ((roleData->BaseData.Voc == EM_Vocation_Warrior || roleData->BaseData.Voc == EM_Vocation_Psyron) &&
					roleData->BaseData.SP < value * (100 - roleData->TempData.Attr.SPWarriorDecrease()) / 100)
				{
					return CASTSPELL_ERROR_SP_WARRIOR;		// SP¤£¨¬
				}
				else if ((roleData->BaseData.Voc_Sub == EM_Vocation_Warrior || roleData->BaseData.Voc_Sub == EM_Vocation_Psyron) &&
					roleData->BaseData.SP_Sub < value * (100 - roleData->TempData.Attr.SPWarriorDecrease()) / 100)
				{
					return CASTSPELL_ERROR_SP_WARRIOR;		// SP¤£¨¬
				}
				break;

			case EM_MagicSpellCostType_SP_Ranger:
				if ((roleData->BaseData.Voc == EM_Vocation_Ranger || roleData->BaseData.Voc == EM_Vocation_Harpsyn) && roleData->BaseData.SP < value * (100 - roleData->TempData.Attr.SPRangerDecrease()) / 100)
					return CASTSPELL_ERROR_SP_RANGER;		// SP¤£¨¬
				else if ((roleData->BaseData.Voc_Sub == EM_Vocation_Ranger || roleData->BaseData.Voc_Sub == EM_Vocation_Harpsyn) && roleData->BaseData.SP_Sub < value * (100 - roleData->TempData.Attr.SPRangerDecrease()) / 100)
					return CASTSPELL_ERROR_SP_RANGER;		// SP¤£¨¬
				break;

			case EM_MagicSpellCostType_SP_Rogue:
				if ((roleData->BaseData.Voc == EM_Vocation_Rogue || roleData->BaseData.Voc == EM_Vocation_Duelist) &&
					roleData->BaseData.SP < value * (100 - roleData->TempData.Attr.SPRogueDecrease()) / 100)
				{
					return CASTSPELL_ERROR_SP_ROGUE;		// SP¤£¨¬
				}
				else if ((roleData->BaseData.Voc_Sub == EM_Vocation_Rogue || roleData->BaseData.Voc_Sub == EM_Vocation_Duelist) &&
					roleData->BaseData.SP_Sub < value * (100 - roleData->TempData.Attr.SPRogueDecrease()) / 100)
				{
					return CASTSPELL_ERROR_SP_ROGUE;		// SP¤£¨¬
				}
				break;

			case EM_MagicSpellCostType_Ammo_All:
				itemField = &roleData->BaseData.EQ.Bow;
				if (itemField->IsEmpty())
					return CASTSPELL_ERROR_BOW;				// »Ý¨D»·µ{ªZ¾¹

				// ÀË¬d»·µ{¼uÃÄ
				itemField = &roleData->BaseData.EQ.Ammo;
				if (itemField->IsEmpty())
					return CASTSPELL_ERROR_AMMO;			// ¼uÃÄ¤£¨¬
				else if (itemField->Count < value)
					return CASTSPELL_ERROR_AMMO;			// ¼uÃÄ¤£¨¬
				break;

			case EM_MagicSpellCostType_Ammo_Throw:
				// ÀË¬d§ëÂYª««~
				itemField = &roleData->BaseData.EQ.Ammo;
				if (itemField->IsEmpty())
					return CASTSPELL_ERROR_ARROW;			// ¼uÃÄ¤£¨¬
				else
				{
					GameObjDbStruct* ammoData = CNetGlobal::GetObj(itemField->OrgObjID);
					if (ammoData->Item.WeaponType != EM_Weapon_Throw)
						return CASTSPELL_ERROR_ARROW;		// »Ý¨D§ëÂYª««~

					if (itemField->Count < value)
						return CASTSPELL_ERROR_ARROW;		// §ëÂYª««~¤£¨¬
				}
				break;

			case EM_MagicSpellCostType_Ammo_Gun:
				// ÀË¬d»·µ{¸Ë³Æ
				itemField = &roleData->BaseData.EQ.Bow;
				if (itemField->IsEmpty())
					return CASTSPELL_ERROR_GUN;			// »Ý¨Dºj
				else
				{
					GameObjDbStruct* rangeData = CNetGlobal::GetObj(itemField->OrgObjID);
					if (rangeData->Item.WeaponType != EM_Weapon_Gun)
						return CASTSPELL_ERROR_GUN;		// »Ý¨Dºj
				}

				// ÀË¬d»·µ{¼uÃÄ
				itemField = &roleData->BaseData.EQ.Ammo;
				if (itemField->IsEmpty())
					return CASTSPELL_ERROR_ARROW;			// ¼uÃÄ¤£¨¬
				else
				{
					GameObjDbStruct* ammoData = CNetGlobal::GetObj(itemField->OrgObjID);
					if (ammoData->Item.WeaponType != EM_Weapon_Ammo_Gun)
						return CASTSPELL_ERROR_ARROW;		// »Ý¨D¼uÃÄ

					if (itemField->Count < value)
						return CASTSPELL_ERROR_ARROW;		// ¼uÃÄ¤£¨¬
				}
				break;

			case EM_MagicSpellCostType_Ammo_Bow:
				// ÀË¬d»·µ{¸Ë³Æ
				itemField = &roleData->BaseData.EQ.Bow;
				if (itemField->IsEmpty())
					return CASTSPELL_ERROR_BOW;				// »Ý¨D¤}
				else
				{
					GameObjDbStruct* rangeData = CNetGlobal::GetObj(itemField->OrgObjID);
					if (rangeData->Item.WeaponType != EM_Weapon_Bow)
						return CASTSPELL_ERROR_BOW;			// »Ý¨D¤}
				}

				// ÀË¬d»·µ{¼uÃÄ
				itemField = &roleData->BaseData.EQ.Ammo;
				if (itemField->IsEmpty())
					return CASTSPELL_ERROR_ARROW;			// ¼uÃÄ¤£¨¬
				else
				{
					GameObjDbStruct* ammoData = CNetGlobal::GetObj(itemField->OrgObjID);
					if (ammoData->Item.WeaponType != EM_Weapon_Ammo_Bow)
						return CASTSPELL_ERROR_ARROW;		// »Ý¨D¼uÃÄ

					if (itemField->Count < value)
						return CASTSPELL_ERROR_ARROW;		// ¼uÃÄ¤£¨¬
				}
				break;

			case EM_MagicSpellCostType_Item:
			{
				int bodyIndex = 0;
				while (bodyIndex < roleData->PlayerBaseData->Body.Count)
				{
					if (roleData->PlayerBaseData->Body.Item[bodyIndex].OrgObjID == value)
						break;
					bodyIndex++;
				}
				if (bodyIndex >= roleData->PlayerBaseData->Body.Count)
					return CASTSPELL_ERROR_ITEM;		// ª««~»Ý¨D¤£¨¬
			}
			break;
			}
		}
	}

	//»Ý¨D
	//////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < 2; i++)
	{
		//ÀË¬d»Ý¨D
		bool IsFind = false;
		MagicSpellNeedTypeENUM NeedType = itemDB->MagicCol.Need[i].Type;
		int NeedValue = itemDB->MagicCol.Need[i].Value;

		if (outNeedType) {
			*outNeedType = NeedType;
		}
		if (outNeedValue) {
			*outNeedValue = NeedValue;
		}

		switch (NeedType)
		{
		case EM_MagicSpellNeedType_None:				//µL»Ý¨D
			break;
		case EM_MagicSpellNeedType_Weapon:				//»Ý¨D ¦Û¤v¸Ë ªZ¾¹
		{
			if (roleData->GetEqItem(EM_EQWearPos_MainHand)->OrgObjID != NeedValue
				&& roleData->GetEqItem(EM_EQWearPos_SecondHand)->OrgObjID != NeedValue)
			{
				return CASTSPELL_ERROR_NEED_WEAPON;
			}
		}
		break;
		case EM_MagicSpellNeedType_EQ:					//»Ý¨D ¦Û¤v¸Ë ¸Ë³Æ
		{
			for (int i = 0; i < EM_EQWearPos_MaxCount; i++)
			{
				if (roleData->BaseData.EQ.Item[i].OrgObjID == NeedValue)
				{
					IsFind = true;
					break;
				}
			}
			if (IsFind == false)
			{
				return CASTSPELL_ERROR_NEED_EQ;
			}
		}
		break;
		case EM_MagicSpellNeedType_Suit:				//»Ý¨D ¦Û¤v¸Ë ®M¸Ë
		{
			int i;
			for (i = 0; i < EM_EQWearPos_MaxCount; i++)
			{
				GameObjDbStructEx* OrgDB;
				OrgDB = CNetGlobal::GetObj(roleData->BaseData.EQ.Item[i].OrgObjID);
				if (OrgDB == NULL)
					continue;
				if (OrgDB->Item.SuitID == NeedValue)
					break;
			}
			if (i == EM_EQWearPos_MaxCount)
				return CASTSPELL_ERROR_NEED_SUIT;
		}
		break;
		case EM_MagicSpellNeedType_Buf:					//»Ý¨D ¦Û¤v¦³ ªk³NBuf
		{
			for (int i = 0; i < roleData->BaseData.Buff.Size(); i++)
			{
				if (roleData->BaseData.Buff[i].BuffID == NeedValue)
				{
					IsFind = true;
					break;
				}
			}
			if (IsFind == false)
			{
				return CASTSPELL_ERROR_NEED_BUFF;
			}
		}
		break;
		case EM_MagicSpellNeedType_Buf_Target:			//»Ý¨D ¥Ø¼Ð¦³ ªk³NBuf
			if (target)
			{
				for (int i = 0; i < target->GetBuffSize(); i++) {
					RoleBuffInfo* buff = target->GetBuffInfo(i);
					if (buff && buff->id == NeedValue) {
						IsFind = true;
						break;
					}
				}
				if (!IsFind) {
					return CASTSPELL_ERROR_TARGET_NEED_BUFF;
				}
			}
			break;
		case EM_MagicSpellNeedType_NoBuf:				//»Ý¨D ¦Û¤v¨S¦³ ªk³NBuf
		{
			for (int i = 0; i < roleData->BaseData.Buff.Size(); i++)
			{
				if (roleData->BaseData.Buff[i].BuffID == NeedValue)
				{
					return CASTSPELL_ERROR_NEED_NOBUFF;
				}
			}
		}
		break;
		case EM_MagicSpellNeedType_NoBuf_Target:		//»Ý¨D ¥Ø¼Ð¨S¦³ ªk³NBuf
			if (target)
			{
				for (int i = 0; i < target->GetBuffSize(); i++) {
					RoleBuffInfo* buff = target->GetBuffInfo(i);
					if (buff && buff->id == NeedValue) {
						return CASTSPELL_ERROR_TARGET_NEED_NOBUFF;
					}
				}
			}
			break;

		case EM_MagicSpellNeedType_WeaponType_Unarmed:	//»Ý¨D ¦Û¤v¸Ë ªZ¾¹Ãþ«¬ ªÅ¤â
		{
			if (roleData->TempData.Attr.MainWeaponType != EM_Weapon_Unarmed)
			{
				return CASTSPELL_ERROR_NEED_UNARMED;
			}
		}
		break;
		case EM_MagicSpellNeedType_WeaponType_Blade:    //»Ý¨D ¦Û¤v¸Ë ªZ¾¹Ãþ«¬ ¼C
		{
			if (roleData->TempData.Attr.MainWeaponType != EM_Weapon_Blade)
			{
				return CASTSPELL_ERROR_NEED_BLADE;
			}
		}
		break;
		case EM_MagicSpellNeedType_WeaponType_Dagger:   //»Ý¨D ¦Û¤v¸Ë ªZ¾¹Ãþ«¬ ¤P­º
		{
			if (roleData->TempData.Attr.MainWeaponType != EM_Weapon_Dagger)
			{
				return CASTSPELL_ERROR_NEED_DAGGER;
			}
		}
		break;
		case EM_MagicSpellNeedType_WeaponType_Wand:     //»Ý¨D ¦Û¤v¸Ë ªZ¾¹Ãþ«¬ Åv§ú
		{
			if (roleData->TempData.Attr.MainWeaponType != EM_Weapon_Wand)
			{
				return CASTSPELL_ERROR_NEED_WAND;
			}
		}
		break;
		case EM_MagicSpellNeedType_WeaponType_Axe:      //»Ý¨D ¦Û¤v¸Ë ªZ¾¹Ãþ«¬ ©ò
		{
			if (roleData->TempData.Attr.MainWeaponType != EM_Weapon_Axe)
			{
				return CASTSPELL_ERROR_NEED_AXE;
			}
		}
		break;
		case EM_MagicSpellNeedType_WeaponType_Bludgeon: //»Ý¨D ¦Û¤v¸Ë ªZ¾¹Ãþ«¬ Âñ´Ò´Î
		{
			if (roleData->TempData.Attr.MainWeaponType != EM_Weapon_Bludgeon)
			{
				return CASTSPELL_ERROR_NEED_BLUDGEON;
			}
		}
		break;
		case EM_MagicSpellNeedType_WeaponType_Claymore: //»Ý¨D ¦Û¤v¸Ë ªZ¾¹Ãþ«¬ Âù¤â¼C
		{
			if (roleData->TempData.Attr.MainWeaponType != EM_Weapon_Claymore)
			{
				return CASTSPELL_ERROR_NEED_CLAYMORE;
			}
		}
		break;
		case EM_MagicSpellNeedType_WeaponType_Staff:    //»Ý¨D ¦Û¤v¸Ë ªZ¾¹Ãþ«¬ §ú
		{
			if (roleData->TempData.Attr.MainWeaponType != EM_Weapon_Staff)
			{
				return CASTSPELL_ERROR_NEED_STAFF;
			}
		}
		break;
		case EM_MagicSpellNeedType_WeaponType_2H_Axe:   //»Ý¨D ¦Û¤v¸Ë ªZ¾¹Ãþ«¬ Âù¤â©ò
		{
			if (roleData->TempData.Attr.MainWeaponType != EM_Weapon_2H_Axe)
			{
				return CASTSPELL_ERROR_NEED_2H_AXE;
			}
		}
		break;
		case EM_MagicSpellNeedType_WeaponType_2H_Hammer://»Ý¨D ¦Û¤v¸Ë ªZ¾¹Ãþ«¬ Âù¤âÂñ
		{
			if (roleData->TempData.Attr.MainWeaponType != EM_Weapon_2H_Hammer)
			{
				return CASTSPELL_ERROR_NEED_2H_HAMMER;
			}
		}
		break;
		case EM_MagicSpellNeedType_WeaponType_Polearm:	//»Ý¨D ¦Û¤v¸Ë ªZ¾¹Ãþ«¬ ºj(ªø¥Ù)
		{
			if (roleData->TempData.Attr.MainWeaponType != EM_Weapon_Polearm)
			{

				return CASTSPELL_ERROR_NEED_POLEARM;
			}
		}
		break;
		case EM_MagicSpellNeedType_WeaponType_SwordType: //»Ý¨D ¦Û¤v¸Ë ªZ¾¹Ãþ«¬ ¼CÃþ(³æÂù¤â)
		{
			if (roleData->TempData.Attr.MainWeaponType != EM_Weapon_Blade
				&& roleData->TempData.Attr.MainWeaponType != EM_Weapon_Claymore)
			{
				return CASTSPELL_ERROR_NEED_SWORD_TYPE;
			}
		}
		break;
		case EM_MagicSpellNeedType_WeaponType_AxeType:	//»Ý¨D ¦Û¤v¸Ë ªZ¾¹Ãþ«¬ ©òÃþ(³æÂù¤â)
		{
			if (roleData->TempData.Attr.MainWeaponType != EM_Weapon_2H_Axe
				&& roleData->TempData.Attr.MainWeaponType != EM_Weapon_Axe)
			{
				return CASTSPELL_ERROR_NEED_AXE_TYPE;
			}
		}
		break;
		case EM_MagicSpellNeedType_WeaponType_Shield:	//»Ý¨D ¦Û¤v¸Ë ªZ¾¹Ãþ«¬ ©òÃþ(³æÂù¤â)
		{
			if (roleData->BaseData.EQ.SecondHand.Mode.EQCannotEquipment)
				return CASTSPELL_ERROR_NEED_SHIELD;

			GameObjDbStructEx* OrgObjDB = CNetGlobal::GetObj(roleData->BaseData.EQ.SecondHand.OrgObjID);
			if (!OrgObjDB->IsArmor() || OrgObjDB->Item.ArmorType != EM_Armor_Shield)
			{
				return CASTSPELL_ERROR_NEED_SHIELD;
			}
		}
		break;
		case EM_MagicSpellNeedType_Distance:
			if (target && player)
			{
				int distance = (int)((target->GetPos() - player->GetPos()).Magnitude());
				if (distance < NeedValue)
					return CASTSPELL_ERROR_NEED_DITANCE;
			}
			break;
		case EM_MagicSpellNeedType_NotAttak:			//»Ý¨D ¦Û¤v«D¾Ô°«
			if (player && GetCombatState())
			{
				return CASTSPELL_ERROR_NEED_NOCOMBAT;
			}
			break;
		case EM_MagicSpellNeedType_Attack:				//»Ý¨D ¦Û¤v¾Ô°«
			if (player && !GetCombatState())
			{
				return CASTSPELL_ERROR_NEED_COMBAT;
			}
			break;
		case EM_MagicSpellNeedType_Critical:			//»Ý¨D ¦Û¤vÃzÀ»
		{
			if (_DEF_ATTACK_EVENT_EFFECT_TIME_ < RoleDataEx::G_SysTime - roleData->TempData.BackInfo.LastCriticalTime)
			{
				return CASTSPELL_ERROR_NEED_CRITICAL;
			}

		}
		break;
		case EM_MagicSpellNeedType_BeCritical:			//»Ý¨D ¦Û¤v³QÃzÀ»
		{
			if (_DEF_ATTACK_EVENT_EFFECT_TIME_ < RoleDataEx::G_SysTime - roleData->TempData.BackInfo.LastBeCriticalTime)
			{
				return CASTSPELL_ERROR_NEED_BECRITICAL;
			}

		}
		break;

		case EM_MagicSpellNeedType_BeDodge:				//»Ý¨D ¦Û¤v³Q°{Á×(¹ï¤è°{Á×)
		{
			if (_DEF_ATTACK_EVENT_EFFECT_TIME_ < RoleDataEx::G_SysTime - roleData->TempData.BackInfo.LastBeDodgeTime)
			{
				return CASTSPELL_ERROR_NEED_BEDODGE;
			}
		}
		break;
		case EM_MagicSpellNeedType_Dodge:				//»Ý¨D ¦Û¤v°{Á×
		{
			if (_DEF_ATTACK_EVENT_EFFECT_TIME_ < RoleDataEx::G_SysTime - roleData->TempData.BackInfo.LastDodgeTime)
			{
				return CASTSPELL_ERROR_NEED_DODGE;
			}
		}
		break;
		case EM_MagicSpellNeedType_Miss:				//»Ý¨D ¦Û¤vMiss
		{
			if (_DEF_ATTACK_EVENT_EFFECT_TIME_ < RoleDataEx::G_SysTime - roleData->TempData.BackInfo.LastMissTime)
			{
				return false;
			}
		}
		break;
		case EM_MagicSpellNeedType_Parry:				//»Ý¨D ¦Û¤v®æ¾×
		{
			if (_DEF_ATTACK_EVENT_EFFECT_TIME_ < RoleDataEx::G_SysTime - roleData->TempData.BackInfo.LastParryTime)
			{
				return CASTSPELL_ERROR_NEED_PARRY;
			}
		}
		break;
		case EM_MagicSpellNeedType_BeParry:				//»Ý¨D ¦Û¤v³Q®æ¾×
		{
			if (_DEF_ATTACK_EVENT_EFFECT_TIME_ < RoleDataEx::G_SysTime - roleData->TempData.BackInfo.LastBeParryTime)
			{
				return CASTSPELL_ERROR_NEED_BEPARRY;
			}
		}
		case EM_MagicSpellNeedType_NotAttack_Target:	//»Ý¨D ¥Ø¼Ð«D¾Ô°«		
			if (target && target->GetCombatState())
			{
				return CASTSPELL_ERROR_TARGET_NEED_NOCOMBAT;
			}
			break;
		case EM_MagicSpellNeedType_Attack_Target:		//»Ý¨D ¥Ø¼Ð¾Ô°«
			if (target && !target->GetCombatState())
			{
				return CASTSPELL_ERROR_TARGET_NEED_COMBAT;
			}
			break;
		case EM_MagicSpellNeedType_SelfHp_Smaller_Percent:		//»Ý¨D HP¦Ê¤À¤ñ¤p©ó
		{

			int Rate = int(roleData->BaseData.HP * 100 / int(roleData->TempData.Attr.Fin.MaxHP));
			if (NeedValue < Rate)
			{
				return CASTSPELL_ERROR_NEED_HP_LOWER;
			}
		}
		break;
		case EM_MagicSpellNeedType_SelfHp_Greater_Percent:		//»Ý¨D HP¦Ê¤À¤ñ¤j©ó
		{
			int Rate = int(roleData->BaseData.HP * 100 / int(roleData->TempData.Attr.Fin.MaxHP));
			if (NeedValue > Rate)
			{
				return CASTSPELL_ERROR_NEED_HP_HIGHER;
			}
		}
		break;
		case EM_MagicSpellNeedType_Self_Job:					//»Ý¨D Â¾·~
		{
			if (roleData->BaseData.Voc != NeedValue
				&& roleData->BaseData.Voc_Sub != NeedValue)
			{
				return CASTSPELL_ERROR_NEED_JOB;
			}
		}
		break;
		case EM_MagicSpellNeedType_WeaponType_LongDistWeapon:	//»Ý¨D ªø¶ZÂ÷ªZ¾¹
		{
			if (roleData->TempData.Attr.BowWeaponType == EM_Weapon_None)
			{
				return CASTSPELL_ERROR_NEED_LONGWEAPON;
			}
		}
		break;
		case EM_MagicSpellNeedType_WeaponType_Hammer:			//»Ý¨D ¦Û¤v¸Ë ªZ¾¹Ãþ«¬ ÂñÃþ(³æÂù¤â)
		{
			if (roleData->TempData.Attr.MainWeaponType != EM_Weapon_Bludgeon
				&& roleData->TempData.Attr.MainWeaponType != EM_Weapon_2H_Hammer)
			{
				return CASTSPELL_ERROR_NEED_HAMMER_TYPE;
			}
		}
		break;
		case EM_MagicSpellNeedType_WeaponType_2H_Weapon:		//»Ý¨D ¦Û¤v¸Ë ªZ¾¹Ãþ«¬ Âù¤âªZ¾¹
		{
			if (roleData->TempData.Attr.MainWeaponType != EM_Weapon_Staff
				&& roleData->TempData.Attr.MainWeaponType != EM_Weapon_2H_Hammer
				&& roleData->TempData.Attr.MainWeaponType != EM_Weapon_2H_Axe
				&& roleData->TempData.Attr.MainWeaponType != EM_Weapon_Claymore
				&& roleData->TempData.Attr.MainWeaponType != EM_Weapon_Polearm)
			{
				return CASTSPELL_ERROR_NEED_2H_WEAPON_TYPE;
			}
		}
		break;
		case EM_MagicSpellNeedType_BuffGroup:
		{
			int i;
			for (i = 0; i < roleData->BaseData.Buff.Size(); i++)
			{
				BuffBaseStruct& Buf = roleData->BaseData.Buff[i];
				if (Buf.Magic != NULL && Buf.Magic->MagicBase.MagicGroupID != NeedValue)
					continue;
				break;
			}
			if (i == roleData->BaseData.Buff.Size())
			{
				return CASTSPELL_ERROR_NEED_BUFF_GROUP;
			}

		}
		break;
		}
	}
	//////////////////////////////////////////////////////////////////////////

	// §Nƒj®É¶¡
	float cooldown[2];
	roleData->GetObjectCooldown(skillID, cooldown);
	if (cooldown[1] > 0.0f)
		return CASTSPELL_ERROR_COLDOWN;				// §Nƒj¤¤


	float	AttackDistance = 0;

	if (itemDB->MagicCol.Flag.ReferenceWeapon_Bow)
	{
		AttackDistance = roleData->TempData.Attr.Fin.Bow_AttackRange;
	}
	else if (itemDB->MagicCol.Flag.ReferenceWeapon_Throw)
	{
		AttackDistance = roleData->TempData.Attr.Fin.Throw_AttackRange;
	}

	AttackDistance = float(itemDB->MagicCol.AttackDistance) + AttackDistance;


	//////////////////////////////////////////////////////////////////////////
	if (itemDB->MagicCol.Flag.Spell_Back)
	{
		//if( Target->CalAngle( this ) < 90 )
		//{
		//return EM_GameMessageEvent_MagicError_Back;
		//}
	}

	if (itemDB->MagicCol.Flag.Spell_Front)
	{
		//if( Target->CalAngle( this ) > 90 )
		//{
		//return EM_GameMessageEvent_MagicError_Front;
		//}
	}

	if (itemDB->MagicCol.Flag.Spell_LookAtTarget)
	{
		//if( CalAngle( Target ) > 45 )
		//{	
		//Net_GameMsgEvent( EM_GameMessageEvent_MagicError_LookAt );
		//Msg( "ªk³N ­n­±¹ï¼Ä¤H¤~¥i¬I®i" );
		//return EM_GameMessageEvent_MagicError_LookAt;
		//}
	}
	//////////////////////////////////////////////////////////////////////////	

	bool checkDistance = false;
	switch (itemDB->MagicCol.TargetType)
	{
	case EM_Releation_Locatoin:
		// ¹ï¦a¨Ï¥Î
		break;

	case EM_Releation_Self:
		// ¹ï¦Û¤v«K¥Î
		break;

	case EM_Releation_Member:
	case EM_Releation_Friend:
	case EM_Releation_Friend_NPC:
	case EM_Releation_Player:
	case EM_Releation_GuildPlayer:
	case EM_Releation_CountryPlayer:
	case EM_Releation_Self_Pet:
	case EM_Releation_Member_Pet:
		// ¤Íµ½ªk³N
		if (target && !target->GetCanAttackState()) {
			checkDistance = true;
		}
		break;

	case EM_Releation_Corpse:
	case EM_Releation_Npc_Corpse:
	case EM_Releation_Player_Corpse:
		if (target && !target->GetDeadState()) {
			return CASTSPELL_ERROR_TARGET;
		}
		if (target && !target->GetCanAttackState()) {
			checkDistance = true;
		}
		break;

	case EM_Releation_Enemy:
	case EM_Releation_NotGuildPlayer:
	case EM_Releation_NoCountryPlayer:
		// ¤³«ëª«¥ó
		if (target && target->GetDeadState()) {
			return CASTSPELL_ERROR_TARGET;
		}
		/*
		if ( target == NULL || !target->GetCanAttackState() ) {
		return CASTSPELL_ERROR_TARGET;
		}
		*/
		checkDistance = true;
		break;

	case EM_Releation_All:
		if (target && target->GetDeadState()) {
			return CASTSPELL_ERROR_TARGET;
		}
		// ©Ò¦³
		break;
	}

	if (target && checkDistance)
	{
		// ¥[¤Wª«¥ó¤j¤p¶ZÂ÷
		GameObjDbStructEx* targetObj = CNetGlobal::GetObj(target->GetObjectId());
		if (targetObj)
		{
			GameObjDbStructEx* imageDB = CNetGlobal::GetObj(targetObj->ImageID);
			if (imageDB)
			{
				AttackDistance += max(0, imageDB->Image.BoundRadiusY);
			}
		}

		if (GetDistanceWithPlayer(target) > AttackDistance)
			return CASTSPELL_ERROR_DITANCE;			// ¬Iªk¶ZÂ÷¹L»·
	}

	return 0;
}

// --------------------------------------------------------------------------------------
bool CGameMain::CastSpellByID(int ID, CRoleSprite* target)
{
	CRoleSprite* player = GetPlayer();
	GameObjDbStructEx* itemDB = CNetGlobal::GetObj(ID);
	RoleDataEx* roleData = CNetGlobal::RoleData();
	bool mouseMoveEnable = GetMouseMoveEnable();

	g_pTeleportBook->BreakCasting();

	// §ó·s®É¤£¯àÄ²µo	
	if (!m_skipCastSpellCheck && (IsUIUpdate() || m_botLocked))
	{
		return false;
	}

	if (player == NULL || itemDB == NULL)
		return false;

	if (target == NULL)
		target = player->GetTargetSprite();

	// ¦º¤`
	if (player->GetDeadState())
	{
		// ¤w¸g¦º¤`
		return false;
	}

	// ¸ü¨ã¤W¤£¯à¬Iªk
	if (player->GetImplementSprite())
	{
		// ¸ü¨ã¤W¤£¯à¬Iªk
		SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_IMPLEMENT_ERROR"));
		return false;
	}

	// §¤ÃM¤W¤£¯à¬I©ñªk³N
	if (m_inMount)
	{
		// ¸ü¨ã¤W¤£¯à¬Iªk
		SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_MOUNT_ERROR"));
		return false;
	}

	// ¯S®í­«­nª««~,°õ¦æªk³N
	if (ID == DF_SKILLID_PK ||
		ID == DF_SKILLID_PORTOL ||
		(ID >= DF_SKILLID_BEGINNERPORTOL_BEGIN && ID <= DF_SKILLID_BEGINNERPORTOL_END))
	{
		// ÀË¬d­«­nª««~¬O§_¦s¦b
		if (CNetGlobal::RoleData()->CheckKeyItem(ID) == false)
		{
			return false;
		}

		NetCli_Other::S_SysKeyFunctionRequest((ID - Def_ObjectClass_KeyItem), 0);
		return true;
	}

	// ¤@¯ëª«²z§ðÀ»
	if (ID == DF_SKILLID_ATTACK)
	{
		if (mouseMoveEnable)
		{
			if (target)
			{
				// ²¾°Ê¨ì¥Ø¼Ð®Ç¨Ã°õ¦æ
				g_pGameMain->SetMoveToPoint(target->GetPos(), target);
			}
		}
		else
		{
			// ª½±µ§ðÀ»
			AttackTarget(target);
		}
		return true;
	}

	if (m_cycleMagicId != 0 && ID == m_cycleMagicId)
	{
		ClearCycleSpell();
		return true;
	}

	// ÀË¬d¬O§_¥Ø«e¥¿¦b¬Iªk¤¤
	switch (m_castSpellState)
	{
		//case CASTSPELL_WAITING_LOCATION:
		//case CASTSPELL_WAITING_TARGET:
	case CASTSPELL_CASTING:
		return false;
	}

	// ¤À¸Ñª««~[¯S¨Ò³W«h]
	if (ID == DF_SKILLID_DISENCHANT)
	{
		if (m_castSpellToItem == 0)
		{
			m_castSpellToItem = ID;
			return true;
		}
		else
		{
			return false;
		}
	}

	// °ò¥»ªk³NÀË¬d
	{
		bool bSpecMagic = false;

		// ¨¤¦â¸ê°T¤£¦s¦b
		if (roleData == NULL)
		{
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("WARRING_SKILL_NOT_EXIST"));
			return false;
		}

		if (itemDB->Classification == EM_ObjectClass_KeyItem)
		{
			switch (ID)
			{
			case DF_SKILLID_PORTOL:
			{
				int iMagicID = CNetGlobal::GetKeyMagicID((ID - Def_ObjectClass_KeyItem));
				if (iMagicID != 0)
				{
					itemDB = CNetGlobal::GetObj(iMagicID);
					bSpecMagic = true;
				}
			}break;

			default:
			{
				if (ID >= (Def_ObjectClass_KeyItem + EM_SkillValueType_BlackSmith) && ID <= (Def_ObjectClass_KeyItem + EM_SkillValueType_Herblism))
				{
					bSpecMagic = true;
				}
			}
			}
		}

		// ªk³N¤£¦s¦b©ÎªÌ«Dªk³NÃþ«¬
		if (itemDB == NULL || itemDB->Classification != EM_ObjectClass_MagicCollect)
		{
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("WARRING_SKILL_NOT_EXIST"));
			return false;
		}

		// ³Q°Êªk³N¤£¯à¬Iªk
		if (itemDB->MagicCol.EffectType == EM_Magic_Eq)
		{
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("WARRING_SKILL_PASSIVE"));
			return false;
		}

		// ÀË¬dÂ¾·~Ãþ«¬
		int skillIndex = -1;
		for (int i = 0; i < _MAX_SPSkill_COUNT_ + _MAX_NormalSkill_COUNT_ + _MAX_NormalSkill_COUNT_; i++)
		{
			if (roleData->PlayerTempData->Skill.AllSkill[i] == ID)
			{
				skillIndex = i;
				break;
			}
		}

		// ®M¸Ë§Þ¯à
		if (skillIndex == -1 && bSpecMagic == false && roleData->IsEquipSuitSkill(ID))
		{
			bSpecMagic = true;
		}

		//¤ÛÆF²Õ¦X§Þ¯à
		if (skillIndex == -1 && bSpecMagic == false)
		{
			//key = ¤jid + ¤pid * 0x100000000
			int EQ1 = roleData->PlayerBaseData->Phantom.EQ[0];
			int EQ2 = roleData->PlayerBaseData->Phantom.EQ[1];

			INT64 TableID = 0;
			if (EQ1 > EQ2)
			{
				TableID = EQ1 + EQ2 * 0x100000000;
			}
			else
			{
				TableID = EQ2 + EQ1 * 0x100000000;
			}

			if (ID == RoleDataEx::PhantomSkill[TableID])
			{
				bSpecMagic = true;
			}
		}

		// «D¥Ø«eÂ¾·~¥i¥H¨Ï¥Î
		if (skillIndex == -1 && bSpecMagic == false)
		{
			GameObjDbStructEx* pTitleObj = CNetGlobal::GetObj(CNetGlobal::RoleData()->BaseData.TitleID);
			if (pTitleObj && pTitleObj->Title.SkillID == ID)
			{

			}
			else
			{
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("WARRING_SKILL_CLASS_WRONG"));
				return false;
			}
		}

		m_castSpellIndex = skillIndex;
	}

	// ÀË¬d¬O§_¥i¥H¬Iªk
	/*
	int needType, needValue;
	switch( CanUseSkill(ID, &needType, &needValue) )
	{
	case CASTSPELL_ERROR_EXIST:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("WARRING_SKILL_NOT_EXIST"));
		return false;

	case CASTSPELL_ERROR_ACTIVE:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("WARRING_SKILL_PASSIVE"));
		return false;

	case CASTSPELL_ERROR_CLASS:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("WARRING_SKILL_CLASS_WRONG"));
		return false;

	case CASTSPELL_ERROR_HP:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("WARRING_SKILL_ABSENCE_HP"));
		return false;

	case CASTSPELL_ERROR_MP:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("WARRING_SKILL_ABSENCE_MP"));
		return false;

	case CASTSPELL_ERROR_SP_WARRIOR:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("WARRING_SKILL_ABSENCE_FURY"));
		return false;

	case CASTSPELL_ERROR_SP_RANGER:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("WARRING_SKILL_ABSENCE_CONCENTRAT"));
		return false;

	case CASTSPELL_ERROR_SP_ROGUE:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("WARRING_SKILL_ABSENCE_VIGOR"));
		return false;

	case CASTSPELL_ERROR_GUN:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("WARRING_SKILL_BOW_NEED"));
		return false;

	case CASTSPELL_ERROR_AMMO:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("WARRING_SKILL_AMMO_NOT_ENOUGH"));
		return false;

	case CASTSPELL_ERROR_BOW:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("WARRING_SKILL_BOW_NEED"));
		return false;

	case CASTSPELL_ERROR_ARROW:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("WARRING_SKILL_AMMO_NOT_ENOUGH"));
		return false;

	case CASTSPELL_ERROR_ITEM:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("WARRING_SKILL_ABSENCE_ITEM"));
		return false;

	case CASTSPELL_ERROR_COLDOWN:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("WARRING_SKILL_COOLDOWN"));
		return false;

	case CASTSPELL_ERROR_TARGET:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("WARRING_TARGET_NEED"));
		return false;

	case CASTSPELL_ERROR_DITANCE:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("WARRING_TARGET_TOO_FAR"));
		return false;

	case CASTSPELL_ERROR_MOVE:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_ERR_MOVE"));
		return false;

	case CASTSPELL_ERROR_NEED_WEAPON:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_NEED_WEAPON"));
		return false;

	case CASTSPELL_ERROR_NEED_EQ:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_NEED_EQ"));
		return false;

	case CASTSPELL_ERROR_NEED_SUIT:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_NEED_SUIT"));
		return false;

	case CASTSPELL_ERROR_NEED_BUFF:
		{
			GameObjDbStructEx* buff = CNetGlobal::GetObj(needValue);
			const char* name = (buff) ? buff->Name : "";
			g_pGameMain->SendWarningMsgEx(g_ObjectData->GetString("SYS_CASTSPELL_NEED_BUFF"), name);
		}
		return false;

	case CASTSPELL_ERROR_NEED_NOBUFF:
		{
			GameObjDbStructEx* buff = CNetGlobal::GetObj(needValue);
			const char* name = (buff) ? buff->Name : "";
			g_pGameMain->SendWarningMsgEx(g_ObjectData->GetString("SYS_CASTSPELL_HAVE_BUFF"), name);
		}
		return false;

	case CASTSPELL_ERROR_NEED_UNARMED:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_NEED_UNARMED"));
		return false;

	case CASTSPELL_ERROR_NEED_BLADE:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_NEED_BLADE"));
		return false;

	case CASTSPELL_ERROR_NEED_DAGGER:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_NEED_DAGGER"));
		return false;

	case CASTSPELL_ERROR_NEED_WAND:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_NEED_WAND"));
		return false;

	case CASTSPELL_ERROR_NEED_AXE:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_NEED_AXE"));
		return false;

	case CASTSPELL_ERROR_NEED_BLUDGEON:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_NEED_BLUDGEON"));
		return false;

	case CASTSPELL_ERROR_NEED_CLAYMORE:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_NEED_CLAYMORE"));
		return false;

	case CASTSPELL_ERROR_NEED_STAFF:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_NEED_STAFF"));
		return false;

	case CASTSPELL_ERROR_NEED_2H_AXE:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_NEED_2H_AXE"));
		return false;

	case CASTSPELL_ERROR_NEED_2H_HAMMER:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_NEED_2H_HAMMER"));
		return false;

	case CASTSPELL_ERROR_NEED_POLEARM:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_NEED_POLEARM"));
		return false;

	case CASTSPELL_ERROR_NEED_SWORD_TYPE:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_NEED_BLADE"));
		return false;

	case CASTSPELL_ERROR_NEED_AXE_TYPE:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_NEED_AXE"));
		return false;

	case CASTSPELL_ERROR_NEED_SHIELD:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_NEED_SHIELD"));
		return false;

	case CASTSPELL_ERROR_NEED_HP_LOWER:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_NEED_HP_LOWER"));
		return false;

	case CASTSPELL_ERROR_NEED_HP_HIGHER:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_NEED_HP_HIGHER"));
		return false;

	case CASTSPELL_ERROR_NEED_JOB:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_NEED_CLASS"));
		return false;

	case CASTSPELL_ERROR_NEED_LONGWEAPON:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_NEED_RANGED"));
		return false;

	case CASTSPELL_ERROR_NEED_HAMMER_TYPE:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_NEED_BLUDGEON"));
		return false;

	case CASTSPELL_ERROR_NEED_2H_WEAPON_TYPE:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_NEED_2H_WEAPON"));
		return false;

	case CASTSPELL_ERROR_NEED_BUFF_GROUP:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_NEED_BUFF_GROUP"));
		return false;

	case CASTSPELL_ERROR_NEED_CRITICAL:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_NEED_CRITICAL"));
		return false;

	case CASTSPELL_ERROR_NEED_BECRITICAL:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_NEED_BECRITICAL"));
		return false;

	case CASTSPELL_ERROR_NEED_DODGE:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_NEED_DODGE"));
		return false;

	case CASTSPELL_ERROR_NEED_MISS:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_NEED_MISS"));
		return false;

	case CASTSPELL_ERROR_NEED_PARRY:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_NEED_PARRY"));
		return false;

	case CASTSPELL_ERROR_NEED_BEPARRY:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_NEED_BEPARRY"));
		return false;

	case CASTSPELL_ERROR_NEED_BEDODGE:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_NEED_BEDODGE"));
		return false;

	case CASTSPELL_ERROR_NEED_COMBAT:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_NEED_COMBAT"));
		return false;
	case CASTSPELL_ERROR_NEED_NOCOMBAT:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_NEED_NOCOMBAT"));
		return false;
	case CASTSPELL_ERROR_NEED_DITANCE:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_NEED_DISTANCE"));
		return false;

	case CASTSPELL_ERROR_TARGET_NEED_BUFF:			//¹ï¶H¥Ø¼Ð»Ý­n¬YBUFF
		{
			GameObjDbStructEx* buff = CNetGlobal::GetObj(needValue);
			const char* name = (buff) ? buff->Name : "";
			g_pGameMain->SendWarningMsgEx(g_ObjectData->GetString("SYS_CASTSPELL_TARGET_NEED_BUFF"), name);
		}
		return false;

	case CASTSPELL_ERROR_TARGET_NEED_NOBUFF:		//¹ï¶H¥Ø¼Ð»Ý­n¬YBUFF
		{
			GameObjDbStructEx* buff = CNetGlobal::GetObj(needValue);
			const char* name = (buff) ? buff->Name : "";
			g_pGameMain->SendWarningMsgEx(g_ObjectData->GetString("SYS_CASTSPELL_TARGET_NEED_NOBUFF"), name);
		}
		return false;

	case CASTSPELL_ERROR_TARGET_NEED_NOCOMBAT:		//¹ï¶H¥Ø¼Ð«D¾Ô°«¤¤
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_TARGET_NOCOMBAT"));
		return false;

	case CASTSPELL_ERROR_TARGET_NEED_COMBAT:		//¹ï¶H¥Ø¼Ð»Ý­n¾Ô°«¤¤
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_TARGET_COMBAT"));
		return false;
	}
	*/


	// »Ý­n¦b¯¸¥ßª¬ºA¤~¥i¥H¬Iªk
	CRuFusion_ActorState actor = player->GetActorState(1);
	switch (actor)
	{
	case ruFUSION_ACTORSTATE_SIT_BEGIN:
	case ruFUSION_ACTORSTATE_SIT_LOOP:
	case ruFUSION_ACTORSTATE_SIT_END:
	case ruFUSION_ACTORSTATE_SLEEP_BEGIN:
	case ruFUSION_ACTORSTATE_SLEEP_LOOP:
	case ruFUSION_ACTORSTATE_SLEEP_END:
	case ruFUSION_ACTORSTATE_CROUCH_BEGIN:
	case ruFUSION_ACTORSTATE_CROUCH_LOOP:
	case ruFUSION_ACTORSTATE_CROUCH_END:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_NEED_STAND"));
		return false;

	case ruFUSION_ACTORSTATE_SPAWN_BEGIN:
	case ruFUSION_ACTORSTATE_SPAWN_END:
		break;
	}

	m_castSpellID = ID;							// °O¿ý¬Iªkªk³N½s¸¹

	switch (itemDB->MagicCol.TargetType)
	{
	case EM_Releation_Locatoin:					// ¹ï¦a¨Ï¥Î
		SetCastSpellState(CASTSPELL_WAITING_LOCATION);
		return true;

	case EM_Releation_Self:						// ¹ï¦Û¤v«K¥Î
		SetBeginSpell(player);
		return true;

	default:									// »Ý­n«ü©w¹ï¶H¥Ø¼Ð
		switch (itemDB->MagicCol.TargetType)
		{
		case EM_Releation_Member:
		case EM_Releation_Friend:
		case EM_Releation_Player:
			//case EM_Releation_All:
		case EM_Releation_GuildPlayer:
		case EM_Releation_CountryPlayer:
			if (GetSelfCastState())
			{
				if (target == NULL || target->GetCanAttackState())
					target = player;
			}
			else if (target && target->GetCanAttackState())
				target = NULL;
			break;
		}

		if (target)
			SetBeginSpell(target);
		else
			SetCastSpellState(CASTSPELL_WAITING_TARGET, true);
		return true;
	}

	return false;
}

// --------------------------------------------------------------------------------------
// name ªk³N¦WºÙ
// level ªk³Nµ¥¯Å
// target ¥Ø¼Ð¹ï¶H
bool CGameMain::CastSpellByName(const char* name, int level, CRoleSprite* target)
{
	if (name == NULL || name[0] == NULL)
		return false;

	int id = -1;
	int maxLv = -1;

	// ¨ú±oªk³N¦Cªí¶µ¥Ø¯Á¤Þ½s¸¹
	for (int i = 0; i < _MAX_SPSkill_COUNT_ + _MAX_NormalSkill_COUNT_ + _MAX_NormalSkill_COUNT_; i++)
	{
		int magicID = CNetGlobal::RoleData()->PlayerTempData->Skill.AllSkill[i];
		GameObjDbStructEx* objData = CNetGlobal::GetObj(magicID);
		if (objData && strcmp(objData->GetName(), name) == 0) {
			if (objData->MagicCol.MagicLV == level) {
				id = magicID;
				break;
			}
			else if (objData->MagicCol.MagicLV > maxLv) {
				id = magicID;
				maxLv = objData->MagicCol.MagicLV;
			}
		}
	}

	// ¯S®í­«­nª««~ªk³N
	// §ðÀ»	
	for (int i = 0; i < 199; i++)
	{
		GameObjDbStructEx* itemDB = g_ObjectData->GetObj(Def_ObjectClass_KeyItem + i);
		if (itemDB && strcmp(itemDB->GetName(), name) == 0)
		{
			id = i + Def_ObjectClass_KeyItem;
			break;
		}
	}

	if (id == -1)
	{
		GameObjDbStructEx* pTitleObj = CNetGlobal::GetObj(CNetGlobal::RoleData()->BaseData.TitleID);
		if (pTitleObj)
		{
			GameObjDbStructEx* pSkillObj = CNetGlobal::GetObj(pTitleObj->Title.SkillID);
			if (pSkillObj)
			{
				if (0 == strcmp(pSkillObj->GetName(), name))
				{
					id = pTitleObj->Title.SkillID;
				}
			}
		}
	}

	if (id == -1)
	{
		RoleDataEx* roleData = CNetGlobal::RoleData();
		for (int i = 0; i < roleData->PlayerBaseData->SuitSkill.MaxEqSkillCount; i++)
		{
			GameObjDbStructEx* pSkillObj = CNetGlobal::GetObj(roleData->PlayerBaseData->SuitSkill.EQ[i]);
			if (pSkillObj)
			{
				if (0 == strcmp(pSkillObj->GetName(), name))
				{
					id = roleData->PlayerBaseData->SuitSkill.EQ[i];
					break;
				}
			}
		}
	}

	return CastSpellByID(id, target);
}

// --------------------------------------------------------------------------------------
bool CGameMain::SetBeginSpell(CRoleSprite* target)
{
	CRoleSprite* player = GetPlayer();
	if (m_castSpellState == CASTSPELL_CASTING || m_castSpellID == 0 || player == NULL)
		return false;

	GameObjDbStructEx* itemDB = CNetGlobal::GetObj(m_castSpellID);
	if (itemDB)
	{
		bool isCast = false;

		// ÂIÀ»²¾°Ê,ÀË¬d¬O§_¬Iªk¶ZÂ÷¹L»·
		if (GetMouseMoveEnable())
		{
			float AttackDistance = 0.0f;

			if (itemDB->MagicCol.Flag.ReferenceWeapon_Bow)
			{
				AttackDistance = CNetGlobal::RoleData()->TempData.Attr.Fin.Bow_AttackRange;
			}
			else if (itemDB->MagicCol.Flag.ReferenceWeapon_Throw)
			{
				AttackDistance = CNetGlobal::RoleData()->TempData.Attr.Fin.Throw_AttackRange;
			}

			AttackDistance = float(itemDB->MagicCol.AttackDistance) + AttackDistance;

			if (target && target != player && itemDB->MagicCol.TargetType != EM_Releation_Locatoin)
			{
				// ¥[¤Wª«¥ó¤j¤p¶ZÂ÷
				GameObjDbStructEx* targetObj = CNetGlobal::GetObj(target->GetObjectId());
				if (targetObj)
				{
					GameObjDbStructEx* imageDB = CNetGlobal::GetObj(targetObj->ImageID);
					if (imageDB)
					{
						AttackDistance += max(0, imageDB->Image.BoundRadiusY);
					}
				}

				// ¶ZÂ÷½d³ò¥~²¾°Ê¨ì¥i¬Iªk¶ZÂ÷
				if (GetDistanceWithPlayer(target) > AttackDistance)
				{
					// ·í¶ZÂ÷¬°ªñ¾Ô¥i§ðÀ»½d³ò²¾°Ê¨ìªñ¾Ôª«²z§ðÀ»½d³ò¤º
					if (AttackDistance < 60.0f)
					{
						AttackDistance = max(target->GetAttackRange(), 20.0f);
					}

					m_magicDistance = AttackDistance;
					SetMoveToPoint(target->GetPos(), target, false);
					return true;
				}
				// ¶ZÂ÷¤º²M°£²¾°Ê¨Ã¶}©l¬Iªk
				else
				{
					ClearMoveToPoint();						// ²M°£²¾°Ê¸ê°T
					SendPlayerPos();						// °e¥X¥Ø«e§¤¼Ð­È
				}
			}
			else
			{
				if (m_pRuSelectAreaEntity)
				{
					CRuVector3 pos = m_pRuSelectAreaEntity->GetTranslation();

					// ¶ZÂ÷½d³ò¥~²¾°Ê¨ì¥i¬Iªk¶ZÂ÷
					if ((player->GetPos() - pos).Magnitude() > AttackDistance)
					{
						m_magicDistance = AttackDistance;
						SetMoveToPoint(pos, NULL, false);
						return true;
					}
					// ¶ZÂ÷¤º²M°£²¾°Ê¨Ã¶}©l¬Iªk
					else
					{
						ClearMoveToPoint();						// ²M°£²¾°Ê¸ê°T
						SendPlayerPos();						// °e¥X¥Ø«e§¤¼Ð­È
					}
				}
			}
		}

		if (m_sendCastSpellCount > 4) {
			if (m_lastCastSpellTime - m_frameTime > 0.01f)
				return false;
			else
			{
			}
		}

		switch (itemDB->MagicCol.TargetType)
		{
		case EM_Releation_Locatoin:					// ¹ï¦a¨Ï¥Î
			if (m_pRuSelectAreaEntity)
			{
				isCast = true;
				CRuVector3 pos = m_pRuSelectAreaEntity->GetTranslation();

				// ¦Û°Ê­±¦V
				if (GetMouseMoveEnable())
					player->SetDirection(pos - player->GetPos());

				// °e¥Xªk³N«e½T»{¤è¦V
				SendPlayerPos(true);

				switch (m_castSpellState)
				{
				case CASTSPELL_WAITING_LOCATION:
					// ®M¸Ë§Þ¯à
					if (CNetGlobal::RoleData()->IsEquipSuitSkill(m_castSpellID))
					{
						NetCli_EQ::S_UseSuitSkillRequest(m_castSpellID, -1, pos.m_x, pos.m_y, pos.m_z);
					}
					else
					{
						NetCli_Magic::BeginSpell_Pos(player->GetIdentify(), pos.m_x, pos.m_y, pos.m_z, m_castSpellID, m_castSpellIndex);
					}
					break;

				case CASTSPELL_WAITING_LOCATION_EXTRA:
				{
					g_pActionFrame->UseExSkill(0, (int)pos.m_x, (int)pos.m_y, (int)pos.m_z);
				}
				break;
				}
			}
			break;

		default:									// »Ý­n«ü©w¹ï¶H¥Ø¼Ð
			if (target)
			{
				if (m_castSpellState == CASTSPELL_WAITING_TARGET_EXTRA)
				{
					g_pActionFrame->UseExSkill(target->GetIdentify(), 0, 0, 0);
				}
				else
				{
					switch (itemDB->MagicCol.TargetType)
					{
					case EM_Releation_Member:
					case EM_Releation_Friend:
					case EM_Releation_Friend_NPC:
					case EM_Releation_Player:
					case EM_Releation_GuildPlayer:
					case EM_Releation_CountryPlayer:
					case EM_Releation_Self_Pet:
					case EM_Releation_Member_Pet:
						if (target->GetCanAttackState())
						{
							g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_TARGET_ERROR"));
							return false;
						}
						break;

					case EM_Releation_Enemy:
					case EM_Releation_All:
						if (!target->GetCanAttackState())
						{
							g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_TARGET_ERROR"));
							return false;
						}
						break;
					}

					// ¯S®í²¾°ÊªýÀÉ´ú¸Õ
					if (itemDB->MagicCol.CliCollisionType != EM_MagicCollisionType_None && itemDB->MagicCol.TargetType != EM_Releation_Player && player != target)
					{
						CRuVector3 p1, p2, p3;
						p1 = player->GetPos();
						p2 = target->GetPos();
						p3 = p1 - p2;

						if (p3.Magnitude() > 0.1f)
						{
							// ÀË¬d°ª«×±×²v
							if (abs(p3.y) / (sqrt(p3.m_x * p3.m_x + p3.m_z * p3.m_z) + 0.00001f) > 1.2f)
							{
								g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_TARGET_COLLISION"));
								return false;
							}

							switch (itemDB->MagicCol.CliCollisionType)
							{
							case EM_MagicCollisionType_Line:
								p3 = (target->GetPos() - player->GetPos()).Normalize();

								// ¦h´ú´X±ø½uÁ×§K­è¦n¤¤¶¡¦³ªÅ»Ø
								for (float i = 0.0f; i < 11.0f; i++) {
									p1 = player->GetPos() + CRuVector3(0.0f, 6.0f + i, 0.0f);
									p2 = target->GetPos() + CRuVector3(0.0f, 6.0f + i, 0.0f) + p3 * (REAL)itemDB->MagicCol.CliCollisionValue;
									if (QueryCollision(p1, p2, p3, true, true, target->GetRuEntity()))
									{
										g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_TARGET_COLLISION"));
										return false;
									}
								}
								break;

							case EM_MagicCollisionType_TargetForward:
								p3 = target->GetDirection();
								p3.m_y = 0.0f;
								p3.Normalize();

								// ¦h´ú´X±ø½uÁ×§K­è¦n¤¤¶¡¦³ªÅ»Ø
								for (float i = 0.0f; i < 11.0f; i++) {
									p1 = player->GetPos() + CRuVector3(0.0f, 6.0f + i, 0.0f);
									p2 = target->GetPos() + CRuVector3(0.0f, 6.0f + i, 0.0f) + p3 * (REAL)itemDB->MagicCol.CliCollisionValue;
									if (QueryCollision(p1, p2, p3, true, true, target->GetRuEntity()))
									{
										g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CASTSPELL_MOVE_COLLISION"));
										return false;
									}
								}
								break;
							}
						}
					}
					isCast = true;

					// ¦Û°Ê­±¦V
					if (GetMouseMoveEnable())
						player->SetDirection(target->GetPos() - player->GetPos());

					// °e¥Xªk³N«e½T»{¤è¦V
					SendPlayerPos(true);

					// ´`Àôªk³N
					if (itemDB->MagicCol.IsPetCycleMagic && GetPlayerTarget())
					{
						// °O¿ý´`Àôªk³N½s¸¹
						SetCycleSpell(m_castSpellID, m_castSpellIndex);
					}

					m_sendSpellTarget = target;
					m_sendSpellMagicID = m_castSpellID;

					if (CNetGlobal::RoleData()->IsEquipSuitSkill(m_castSpellID))
					{
						NetCli_EQ::S_UseSuitSkillRequest(m_castSpellID, target->GetIdentify(), 0, 0, 0);
					}
					else
					{
						NetCli_Magic::BeginSpell(player->GetIdentify(), target->GetIdentify(), m_castSpellID, m_castSpellIndex);
					}
				}
			}
			break;
		}

		if (isCast)
		{
			m_lastCastSpellTime = m_frameTime + 1.0;			// 1¬í«á¦Û°Ê¸Ñ°£
			m_sendCastSpellCount++;

			// ÀË¬d¬O§_»Ý­n¤U°¨§ðÀ»
			if (!itemDB->MagicCol.IsRideEnable) {
				Dismount();
			}

			//player->SetSpellMotion(itemDB->ImageObj->GatherMotion);		// Àu¥ý°µ°Ê§@

			// ¤£³]©w¬Iªk¤¤ª¬ºA
			SetCastSpellState(CASTSPELL_NONE);
			return true;
		}
	}
	return false;
}

// --------------------------------------------------------------------------------------
void CGameMain::SetCastSpellState(eCastSpellState state, bool forceReset)
{
	if (m_castSpellState != state || forceReset)
	{
		GameObjDbStructEx* skillObj;

		// §R°£¿ï¨úª«¥ó
		ClearSelectAreaEntity();

		m_castSpellState = state;
		switch (m_castSpellState)
		{
		case CASTSPELL_NONE:
			m_castSpellID = 0;
			m_castSpellIndex = -1;
			m_castSpellToItem = 0;
			break;

		case CASTSPELL_WAITING_LOCATION_EXTRA:
		{
			ExtraActionItem* extraItem = g_pActionFrame->GetExtraWaitingItem();
			if (extraItem == NULL)
				break;

			m_castSpellID = extraItem->skillId;
		}

		case CASTSPELL_WAITING_LOCATION:
			skillObj = CNetGlobal::GetObj(m_castSpellID);
			if (skillObj)
			{
				DisplaySelectAreaEntity((float)skillObj->MagicCol.AttackDistance, skillObj->MagicCol.EffectRange / 25.0f);
			}
			break;

		case CASTSPELL_WAITING_TARGET:
			break;

		case CASTSPELL_WAITING_TARGET_EXTRA:
		{
			ExtraActionItem* extraItem = g_pActionFrame->GetExtraWaitingItem();
			if (extraItem == NULL)
				break;

			m_castSpellID = extraItem->skillId;
		}
		break;

		case CASTSPELL_CASTING:
			break;
		}
	}
}

// --------------------------------------------------------------------------------------
bool CGameMain::SpellStopCasting()
{
	switch (m_castSpellState)
	{
	case CASTSPELL_CASTING:
		NetCli_Magic::CancelSpellMagic();

	case CASTSPELL_WAITING_LOCATION_EXTRA:
	case CASTSPELL_WAITING_TARGET_EXTRA:
		g_pActionFrame->ClearWaitingSpellID();

	case CASTSPELL_WAITING_LOCATION:
	case CASTSPELL_WAITING_TARGET:
		SetCastSpellState(CASTSPELL_NONE, true);
		return true;

	default:
		if (m_isCancelCastSpell && m_sendCastSpellCount > 0) {
			ClearCastSpellCount();
			NetCli_Magic::CancelSpellMagic();
			return true;
		}
	}
	if (ItemCastingEnable())
	{
		ItemCastingInterrupt();
		return true;
	}
	if (g_pTeleportBook->BreakCasting())
		return true;

	// ¨ú®øª««~¹ïª««~¨Ï¥Î
	if (g_pBagFrame->GetUseItemIndex() != -1)
	{
		g_pBagFrame->UseItemCancel();
		return true;
	}

	// ¨ú®øª««~¹ï¦a¨Ï¥Î
	if (g_pBagFrame->GetUseItemToLocationID() != -1)
	{
		g_pBagFrame->CancelUseItemToLocation();
		return true;
	}

	// ¨ú®øÃdª«¥l³ê
	if (g_pPetFrame->IsSummoningPet())
	{
		g_pPetFrame->StopSummonPet();
		return true;
	}

	return false;
}

// --------------------------------------------------------------------------------------
bool CGameMain::CheckDisplayEffect(int id, int targetId)
{
	return CheckDisplayEffect(FindSpriteByID(id), FindSpriteByID(targetId));
}

// --------------------------------------------------------------------------------------
bool CGameMain::CheckDisplayEffect(CRoleSprite* ownerSprite, CRoleSprite* targetSprite)
{
	bool show = true;

	// ¤£­n°µ¯S§O³B²z
	if (GetHideOtherPlayerEffect()) {
		if (ownerSprite && (ownerSprite->GetObjectType() != eSpriteObject_Player || ownerSprite == m_pPlayer))
			show = true;
		else if (targetSprite && targetSprite == m_pPlayer)
			show = true;
		else
			show = false;
	}

	return show;
}

// --------------------------------------------------------------------------------------
void CGameMain::ResetControlArgument()
{
	m_clientSetting.Init();
}

// --------------------------------------------------------------------------------------
void CGameMain::SetPlayerTitleVisible(bool show)
{
	if (m_clientSetting.isPlayerTitleVisible != show)
	{
		m_clientSetting.isPlayerTitleVisible = show;
		for (map<int, CRoleSprite*>::iterator iter = m_spriteContainer.begin(); iter != m_spriteContainer.end(); iter++)
		{
			CRoleSprite* sprite = iter->second;
			if (sprite->GetObjectType() != eSpriteObject_Player || sprite == m_pPlayer)
				continue;

			if (show)
				sprite->SetNameVisibleForce(true);
			else if (sprite != GetPlayerTarget())
				sprite->SetNameVisibleForce(false);
		}
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::SetNPCTitleVisible(bool show)
{
	if (m_clientSetting.isNPCTitleVisible != show)
	{
		m_clientSetting.isNPCTitleVisible = show;
		for (map<int, CRoleSprite*>::iterator iter = m_spriteContainer.begin(); iter != m_spriteContainer.end(); iter++)
		{
			CRoleSprite* sprite = iter->second;
			switch (sprite->GetObjectType())
			{
			case eSpriteObject_NPC:
			case eSpriteObject_QuestNPC:
				if (show)
					sprite->SetNameVisibleForce(true);
				else if (sprite != GetPlayerTarget())
					sprite->SetNameVisibleForce(false);
				break;
			}
		}
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::SetSelfTitleVisible(bool show)
{
	if (m_clientSetting.isSelfTitleVisible != show)
	{
		m_clientSetting.isSelfTitleVisible = show;
		if (m_pPlayer)
			m_pPlayer->SetNameVisibleForce(show);
	}
}

// --------------------------------------------------------------------------------------
// ¬O§_Åã¥ÜºÙ¸¹
void CGameMain::SetTitleVisible(bool show)
{
	if (m_clientSetting.isTitleVisible != show)
	{
		m_clientSetting.isTitleVisible = show;
		for (map<int, CRoleSprite*>::iterator iter = m_spriteContainer.begin(); iter != m_spriteContainer.end(); iter++)
		{
			(iter->second)->SetTitleVisible(show);
		}
	}
}

// --------------------------------------------------------------------------------------
// ¬O§_Åã¥Ü¤½·|¦WºÙ
void CGameMain::SetGuildVisible(bool show)
{
	if (m_clientSetting.isGuildVisible != show)
	{
		m_clientSetting.isGuildVisible = show;
		for (map<int, CRoleSprite*>::iterator iter = m_spriteContainer.begin(); iter != m_spriteContainer.end(); iter++)
		{
			(iter->second)->SetGuildVisible(show);
		}
	}
}

// --------------------------------------------------------------------------------------
// ¬O§_Åã¥ÜÀY¤Wµ½´c­È¹Ï¥Ü
void CGameMain::SetTitleIconVisible(bool show)
{
	if (m_clientSetting.isTitleIconVisible != show)
	{
		m_clientSetting.isTitleIconVisible = show;
		for (map<int, CRoleSprite*>::iterator iter = m_spriteContainer.begin(); iter != m_spriteContainer.end(); iter++)
		{
			(iter->second)->SetTitleIconVisible(show);
		}
	}
}

// --------------------------------------------------------------------------------------
// type		¨Ï¥ÎÃþ«¬
// srcPos	¨Ó·½­I¥]ª««~¯Á¤Þ½s¸¹
// destType ¥Ø¼Ðª«¥óÃþ«¬
// destPos	¥Ø¼Ðª««~¯Á¤Þ½s¸¹
bool CGameMain::ItemCastingStart(ItemCastingType type, int srcPos, int destType, int destPos)
{
	CRuFusion_ActorState actor = ruFUSION_ACTORSTATE_CRAFTING_BEGIN;
	const char* actEffectStr = NULL;

	// ÀË¬d¬O§_¦³ª««~°µ¥Î¤¤
	if (m_itemCasting.type != ITEM_CASTING_NONE)
		return false;

	int iTarget = -1;
	CRoleSprite* target = GetPlayerTarget();
	if (target)
		iTarget = target->GetIdentify();

	/*
	// ÀË¬dÃM­¼ª¬ºA(¤£¯à¨Ï¥Î)
	CRoleSprite* player = GetPlayer();
	if ( player && player->GetRideState() )
	{
		SendWarningMsg(g_ObjectData->GetString("SYS_GAMEMSGEVENT_167"));
		return false;
	}
	*/

	switch (type)
	{
	case ITEM_CASTING_NONE:
		return false;

	case ITEM_CASTING_USEITEM:
	{
		ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(srcPos);
		if (item == NULL)
			return false;

		// §PÂ_¨Ó·½ª««~Ãþ«¬
		GameObjDbStructEx* itemDB = CNetGlobal::GetObj(item->OrgObjID);
		switch (itemDB->Classification)
		{
		case EM_ObjectClass_Item:
			switch (itemDB->Item.Class)
			{
			case EM_Item_Rune:
				actor = ruFUSION_ACTORSTATE_CRAFTING_BEGIN;
				actEffectStr = g_ObjectData->GetString("FX_NAME_ADDRUNE_HAND");
				break;

			default:
				switch (itemDB->Item.UseType)
				{
				case EM_ItemUseType_EqRefine:			//½Äµ¥
					actor = ruFUSION_ACTORSTATE_CRAFTING_BEGIN;
					actEffectStr = g_ObjectData->GetString("FX_NAME_RUSHLEVEL_HAND");
					break;

				case EM_ItemUseType_Dissolution:		//©î¸Ñª««~
					actor = ruFUSION_ACTORSTATE_CRAFTING_BEGIN;
					actEffectStr = g_ObjectData->GetString("FX_NAME_DESOULBOUND_HAND");
					break;

				case EM_ItemUseType_Coloring:			//¬V¦â
					actor = ruFUSION_ACTORSTATE_CRAFTING_BEGIN;
					break;

				case EM_ItemUseType_ClsEqSoulbound:		//²M°£Soulbound
					actor = ruFUSION_ACTORSTATE_CRAFTING_BEGIN;
					actEffectStr = g_ObjectData->GetString("FX_NAME_DESOULBOUND_HAND");
					break;

				case EM_ItemUseType_ClsTrade:		//²M°£Soulbound
					actor = ruFUSION_ACTORSTATE_CRAFTING_BEGIN;
					actEffectStr = g_ObjectData->GetString("FX_NAME_DESOULBOUND_HAND");
					break;


				case EM_ItemUseType_FixEq:				//­×¸Ë
					actor = ruFUSION_ACTORSTATE_CRAFTING_BEGIN;
					break;

				case EM_ItemUseType_EQDigHole:			//¸Ë³Æ«õ¬}
					actor = ruFUSION_ACTORSTATE_CRAFTING_BEGIN;
					actEffectStr = g_ObjectData->GetString("FX_NAME_ADDRUNEHOLE_HAND");
					break;

				case EM_ItemUseType_EQClearRune:		//²M°£¸Ë³Æ²Å¤å
					actor = ruFUSION_ACTORSTATE_CRAFTING_BEGIN;
					break;

				case EM_ItemUseType_ResetAbility:		//­«³]°ò¥»¼Æ­È
					actor = ruFUSION_ACTORSTATE_CRAFTING_BEGIN;
					break;

				case EM_ItemUseType_ResetSkillPoint:	//­«³]§Þ¯àÂI¼Æ
					actor = ruFUSION_ACTORSTATE_CRAFTING_BEGIN;
					break;
				}
				break;
			}
			break;

		case EM_ObjectClass_Recipe:
			// server script ±±¨î
			//actor = ruFUSION_ACTORSTATE_CRAFTING_BEGIN;
			//actEffectStr = g_ObjectData->GetString("FX_NAME_LEARNFORMULA_HAND");
			break;
		}

		NetCli_Other::S_BeginCastingRequest(EM_CastingType_NormalUseItem, item->OrgObjID, iTarget);
	}
	break;

	// ¤À¸Ñ§Þ¯à
	case ITEM_CASTING_DISSOLUTION:
	{
		ClearCastSpellToItem();			// ²M°£¤À¸Ñ§Þ¯à

		// ÀË´ú¥Ø¼Ðª««~,¬O§_¥i¥H¤À¸Ñ
		ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(destPos);
		if (item == NULL || CNetGlobal::RoleData()->CheckDissolution(NULL, item) == EM_CheckDissolutionResult_Err) {
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_DISSOLUTION_ERROR"));
			return false;
		}

		item->Pos = EM_USE_ITEM;
		actor = ruFUSION_ACTORSTATE_CRAFTING_BEGIN;
		actEffectStr = g_ObjectData->GetString("FX_NAME_DISSOLUTION_HAND");
		NetCli_Other::S_BeginCastingRequest(EM_CastingType_Dissolution, 0, iTarget);
	}
	break;
	case ITEM_CASTING_PARTNER://©I¥s¹Ù¦ñ casting
	{



		actor = ruFUSION_ACTORSTATE_CRAFTING_BEGIN;
		GameObjDbStructEx* partnerDB = g_pPartnerFrame->GetItem(srcPos, destType);
		if (partnerDB)

		{
			actEffectStr = partnerDB->GetName();
			NetCli_Other::S_BeginCastingRequest(EM_CastingType_NormalUseItem, partnerDB->GUID, iTarget);
		}



	}
	break;

	case ITEM_CASTING_REFINE:
		break;
	}

	// play motion
	if (m_pPlayer) {
		if (actor != ruFUSION_ACTORSTATE_NORMAL)
			m_pPlayer->SetActorState(actor, 1);
		if (actEffectStr) {
			m_pPlayer->ClearCastingEffect();
			m_pPlayer->BeginCastingEffect(actEffectStr, "p_left_hand", false);
			m_pPlayer->BeginCastingEffect(actEffectStr, "p_right_hand", false);
		}
	}

	m_itemCasting.type = type;
	m_itemCasting.bagPos = srcPos;
	m_itemCasting.destType = destType;
	m_itemCasting.destPos = destPos;
	m_itemCasting.actor = actor;
	m_itemCasting.hasEffect = (actEffectStr != NULL);

	return true;
}

// --------------------------------------------------------------------------------------
void CGameMain::ItemCastingInterrupt()
{
	switch (m_itemCasting.type)
	{
	case ITEM_CASTING_USEITEM:
	{
		ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(m_itemCasting.bagPos);
		if (item)
		{
			item->Pos = EM_ItemState_NONE;
			g_pBagFrame->UpdateItem(m_itemCasting.bagPos);
		}

		switch (m_itemCasting.destType)
		{
		case 0:
			item = CNetGlobal::RoleData()->GetBodyItem(m_itemCasting.destPos);
			break;
		case 1:
			item = CNetGlobal::RoleData()->GetEqItem(m_itemCasting.destPos);
			break;
		case 2:
			item = CNetGlobal::RoleData()->GetBankItem(m_itemCasting.destPos);
			break;
		}
		if (item)
			item->Pos = EM_ItemState_NONE;
	}
	break;

	case ITEM_CASTING_DISSOLUTION:
	{
		ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(m_itemCasting.destPos);
		if (item)
			item->Pos = EM_ItemState_NONE;
	}
	break;
	case ITEM_CASTING_PARTNER:
	{







	}
	break;

	default:
		return;
	}

	ItemCastingReset();
	NetCli_Other::S_InterruptCastingRequest();
}

// --------------------------------------------------------------------------------------
void CGameMain::ItemCastingStop()
{
	switch (m_itemCasting.type)
	{
	case ITEM_CASTING_USEITEM:
	{
		ItemFieldStruct* srcItem = CNetGlobal::RoleData()->GetBodyItem(m_itemCasting.bagPos);
		if (srcItem)
		{
			ItemFieldStruct* destItem = NULL;
			switch (m_itemCasting.destType)
			{
			case 0:		// ­I¥]
				destItem = CNetGlobal::RoleData()->GetBodyItem(m_itemCasting.destPos);
				if (destItem)
					NetCli_Item::_UseItemToItem_Body(*srcItem, m_itemCasting.bagPos, *destItem, m_itemCasting.destPos, (char*)g_pBagFrame->GetPassword());
				break;

			case 1:		// ¸Ë³Æ
				destItem = CNetGlobal::RoleData()->GetEqItem(m_itemCasting.destPos);
				if (destItem)
					NetCli_Item::_UseItemToItem_EQ(*srcItem, m_itemCasting.bagPos, *destItem, m_itemCasting.destPos, (char*)g_pBagFrame->GetPassword());
				break;

			case 2:		// »È¦æ
				destItem = CNetGlobal::RoleData()->GetBankItem(m_itemCasting.destPos);
				if (destItem)
					NetCli_Item::_UseItemToItem_Bank(*srcItem, m_itemCasting.bagPos, *destItem, m_itemCasting.destPos, (char*)g_pBagFrame->GetPassword());
				break;

			case 3:		// ¨S¦³¥Øªºª««~
			{
				CRoleSprite* target = GetPlayerTarget();
				int GItemID = -1;
				if (target)
					GItemID = target->GetIdentify();
				NetCli_Item::_UseItem_Body(*srcItem, m_itemCasting.bagPos, GItemID);
			}
			break;
			}
		}
	}
	break;


	case ITEM_CASTING_DISSOLUTION:
	{
		ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(m_itemCasting.destPos);
		if (item)
		{
			NetCli_Item::_ItemDissolutionRequest(*item, m_itemCasting.destPos);
		}
	}
	break;
	case ITEM_CASTING_PARTNER:
	{

		g_pPartnerFrame->_CallPartner(m_itemCasting.bagPos, m_itemCasting.destType);


	}
	break;

	default:
		return;
	}

	ItemCastingReset();
}

// --------------------------------------------------------------------------------------
void CGameMain::ItemCastingReset()
{
	if (m_pPlayer) {
		switch (m_itemCasting.actor)
		{
		case ruFUSION_ACTORSTATE_CRAFTING_BEGIN:
		case ruFUSION_ACTORSTATE_CRAFTING_LOOP:
			m_pPlayer->SetActorState(ruFUSION_ACTORSTATE_CRAFTING_END, 1);
			break;
		}

		if (m_itemCasting.hasEffect)
			m_pPlayer->EndCastingEffect();
	}

	m_itemCasting.type = ITEM_CASTING_NONE;
	m_itemCasting.bagPos = -1;
	m_itemCasting.destType = -1;
	m_itemCasting.destPos = -1;
	m_itemCasting.actor = 0;
	m_itemCasting.hasEffect = false;
}

// --------------------------------------------------------------------------------------
bool CGameMain::UseBagItem(int index, int x, int y, int z)
{
	RoleDataEx* Role = CNetGlobal::RoleData();
	ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(index);
	if (item)
	{
		GameObjDbStructEx* itemDB = CNetGlobal::GetObj(item->OrgObjID);
		if (itemDB)
		{
			switch (itemDB->Classification)
			{
			case EM_ObjectClass_Item:
				if (itemDB->Item.CastingTime > 0)
				{
					if (!ItemCastingStart(ITEM_CASTING_USEITEM, index, 3, -1))
						return false;
				}
				else
				{
					if (g_pBagFrame->GetUseItemToLocationID() != -1)
					{
						if (m_pRuSelectAreaEntity)
						{
							CRuVector3 pos = m_pRuSelectAreaEntity->GetTranslation();
							x = (int)pos.m_x;
							y = (int)pos.m_y;
							z = (int)pos.m_z;
						}
						g_pBagFrame->CancelUseItemToLocation();
					}

					CRoleSprite* target = GetPlayerTarget();
					if (target)
						NetCli_Item::_UseItem_Body(*item, index, target->GetIdentify(), (float)x, (float)y, (float)z);
					else
						NetCli_Item::_UseItem_Body(*item, index, -1, (float)x, (float)y, (float)z);
				}
				break;

			case EM_ObjectClass_Recipe:
				if (itemDB->Recipe.Request.Skill != EM_SkillValueType_None)
				{
					/*
					float SkillValue = Role->GetSkillValue( itemDB->Recipe.Request.Skill );
					if( SkillValue < itemDB->Recipe.Request.SkillLV )
					{
						g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, g_ObjectData->GetString( "WARRING_FORMULA_CANT_LEARN" ), "" );
						return false;
					}
					*/
				}

				if (itemDB->Recipe.Request.KeyItemID == 0
					|| Role->CheckKeyItem(itemDB->Recipe.Request.KeyItemID) == true)
				{
					g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, g_ObjectData->GetString("WARRING_FORMULA_ALREADY_LEARN"), "");
					return false;
				}

				if (!ItemCastingStart(ITEM_CASTING_USEITEM, index, 3, -1))
					return false;
				break;

			default:
				return false;
			}
			item->Pos = EM_USE_ITEM;
			return true;
		}
	}
	return false;
}

// --------------------------------------------------------------------------------------
void CGameMain::PlayerResurrect()
{
	m_selfRevive = false;
	m_resurrect = false;
	m_resurrectTime = 0.0f;

	if (m_interface)
	{
		m_interface->SendWorldEvent("PLAYER_ALIVE");
	}
}

/*
// --------------------------------------------------------------------------------------
void CGameMain::MoveToScreenPoint(int x, int y)
{
if (m_pPlayer == NULL || m_pPlayer->IsDead())
return;

CRuVector3 tarPt;
if (g_pGameMain->ChangeScreenPoint(x, y, tarPt))
{
//m_pMoveTarget->SetPos(tarPt);
//m_pMoveTarget->Show();

m_pPlayer->SetDirection(tarPt - m_pPlayer->GetPos());
m_pPlayer->PathMove(tarPt);
}
}

// --------------------------------------------------------------------------------------
void CGameMain::MoveToPlayer(CRoleSprite* pPlayer)
{
static CRoleSprite* pLastPlayer = NULL;
static float lastMoveTime = 0.0f;

if (m_pPlayer == NULL || m_pPlayer->IsDead() || pPlayer == m_pPlayer)
return;

float time = g_pGameMain->GetFrameTime();
if ( pLastPlayer != pPlayer || time - lastMoveTime > 0.5f )
{
CRuVector3 pos;
m_pPlayer->SetDirection(pPlayer->GetPos() - m_pPlayer->GetPos());
m_pPlayer->PathMove(pPlayer->GetPos());

lastMoveTime = time;
}
pLastPlayer = pPlayer;
}
*/



// --------------------------------------------------------------------------------------
void CGameMain::LoadLoginScreen()
{
	g_pAccountLogin->InitLoginEnvironment();

	// ¸ü¤J¦aªí
	LoadTerrain(m_strLoginWdbFileName.c_str());
	SetCameraControllerEnable(false);

	// ³]©wÃèÀY
	CRuVector3 TargetPos;

	// ³]©w¼È®É
	m_cameraTempFarPlane = 1800.0f;

	static bool s_bFirst = true;
	if (s_bFirst)
	{
		s_bFirst = false;
		g_pAccountLogin->SetCameraLayout(g_pAccountLogin->GetAccountLoginCameraLayoutBegin());

		// ³]©wÀô¹Òªì©l¦ì¸m
		TargetPos = g_pAccountLogin->GetAccountLoginCameraLayoutBegin().pos;
	}
	else
	{
		g_pAccountLogin->SetCameraLayout(g_pAccountLogin->GetAccountLoginCameraLayoutEnd());

		// ³]©wÀô¹Òªì©l¦ì¸m
		TargetPos = g_pAccountLogin->GetAccountLoginCameraLayoutEnd().pos;
	}

	//sCameraLayout CameraLayout;
	//CameraLayout.pos.x = 1541.11;
 //	CameraLayout.pos.y = 60.00;
	//CameraLayout.pos.z = 2861.64;
	//CameraLayout.pitch = 7.75;
	//CameraLayout.yaw = 5.71;
 //	CameraLayout.roll = 0.0;
 // 	CameraLayout.zoom = 50;
 //	CameraLayout.time = 0.0;
	//g_pAccountLogin->SetCameraLayout( CameraLayout );
 //
	//g_pAccountLogin->AddControlCamera( CameraLayout );
	//g_pAccountLogin->AddControlCamera( CameraLayout );
 // 	g_pAccountLogin->AddControlCamera( CameraLayout );
	//g_pAccountLogin->AddControlCamera( CameraLayout );
	//g_pAccountLogin->AddControlCamera( CameraLayout );

	//CameraLayout.pos.x = 1585.31;
	//CameraLayout.pos.y = 35.91;
	//CameraLayout.pos.z = 2995.15;
	//CameraLayout.pitch = -7.25;
	//CameraLayout.yaw = -8.15;
	//CameraLayout.roll = 0.0;
	//CameraLayout.zoom = 40;
	//CameraLayout.time = 1.8;
	//g_pAccountLogin->AddControlCamera( CameraLayout );

	//CameraLayout.pos.x = 1585.31;
	//CameraLayout.pos.y = 35.91;
	//CameraLayout.pos.z = 2995.15;
	//CameraLayout.pitch = -7.25;
	//CameraLayout.yaw = -8.15;
	//CameraLayout.roll = 0.0;
	//CameraLayout.zoom = 30;
 // 	CameraLayout.time = 0.7;
	//g_pAccountLogin->AddControlCamera( CameraLayout );


	m_pRuCameraController->UpdatePresetCameraConfiguration();
	if (m_pRuTerrain)
	{
		m_pRuTerrain->SetCamera(m_pCamera);
		m_pRuTerrain->Update(0.01f);

		// ¥[³t¦aªíÅª¨ú
		g_ruEngineSettings->SetSetting("system:chunk dequeue rate", 200.0f);
		g_ruEngineSettings->SetSetting("system:backdrop dequeue rate", 200.0f);

		// Give global worker thread maximum allocation
		g_ruGlobalWorkerThread->SetSleepPeriod(0);
		g_ruResourceManager->SetObjectDequeueRate(200.0f);
		g_ruResourceManager->SetTextureDequeueRate(200.0f);

		while (m_pRuTerrain->GetLoadingStatus() < 1.0f)
		{
			// Update global function schedule
			g_ruFunctionSchedule->Update();

			m_pRuTerrain->SetCamera(m_pCamera);
			m_pRuTerrain->Update(0.01f);
			// Sleep(10);
		}

		// ¦^´_¥¿±`Åª¨ú³t«×
		g_ruEngineSettings->SetSetting("system:chunk dequeue rate", 8.0f);
		g_ruEngineSettings->SetSetting("system:backdrop dequeue rate", 16.0f);

		// Restore global worker thread default allocation
		g_ruGlobalWorkerThread->SetSleepPeriod(25);
		g_ruResourceManager->SetObjectDequeueRate(5.0f);
		g_ruResourceManager->SetTextureDequeueRate(5.0f);
	}

	UpdateEnvironment(TargetPos);
}

// °e¥X²¾°£§¤ÃMBUFF
void CGameMain::Dismount()
{
	CRoleSprite* player = m_pPlayer;
	if (player == NULL || !player->GetRideState())
		return;

	RoleBuffInfo* buff;
	for (int i = 0; (buff = player->GetBuffInfo(i)) != NULL; i++) {
		if (buff->mount && !buff->locked) {
			buff->locked = true;
			NetCli_Magic::CancelAssistMagic(buff->id);
		}
	}
}

// ----------------------------------------------------------------------------
void CGameMain::SetServerInfo(const char* country, int IPNum)
{
	m_serverCountry = country;
	m_IPNum = IPNum;
}

// ----------------------------------------------------------------------------
bool CGameMain::IsUIUpdate()
{
	if (m_skipCastSpellCheck)
		return false;

	if (m_interface == NULL)
		return false;

	if (m_interface->GetUiWorld() == NULL)
		return false;

	if (m_interface->GetUiWorld()->IsUpdate())
		return true;

	if (m_interface->GetUiWorld()->IsEvent())
		return true;

	return false;
}

// ----------------------------------------------------------------------------
bool CGameMain::SaveRoleData(const wchar_t* name)
{
	RoleDataEx* role = CNetGlobal::RoleData();
	if (name == NULL || role == NULL)
		return false;

	wstring filename = m_interface->GetGlobalPathW(name);
	filename += L".rda";

	FILE* fptr = NULL;
	if ((fptr = _wfopen(filename.c_str(), L"wb")) != NULL)
	{
		int baseSize = sizeof(BaseRoleData);
		int playerBaseSize = sizeof(PlayerBaseRoleData);

		fwrite(&baseSize, sizeof(int), 1, fptr);
		fwrite(&playerBaseSize, sizeof(int), 1, fptr);
		fwrite(&role->BaseData, baseSize, 1, fptr);
		fwrite(role->PlayerBaseData, playerBaseSize, 1, fptr);
		fclose(fptr);
	}

	return true;
}

// ----------------------------------------------------------------------------
bool CGameMain::LoadRoleData(const wchar_t* name)
{
	RoleDataEx* role = CNetGlobal::RoleData();
	if (name == NULL || role == NULL)
		return false;

	wstring filename = m_interface->GetGlobalPathW(name);
	filename += L".rda";

	FILE* fptr = NULL;
	if ((fptr = _wfopen(filename.c_str(), L"rb")) != NULL)
	{
		int baseSize, playerBaseSize;

		fread(&baseSize, sizeof(int), 1, fptr);
		fread(&playerBaseSize, sizeof(int), 1, fptr);

		if (baseSize != sizeof(BaseRoleData) || playerBaseSize != sizeof(PlayerBaseRoleData))
		{
			fclose(fptr);
			return false;
		}

		fread(&role->BaseData, baseSize, 1, fptr);
		fread(role->PlayerBaseData, playerBaseSize, 1, fptr);
		fclose(fptr);

		NetCli_Other::SL_GmResetRole(baseSize, playerBaseSize, role->BaseData, *role->PlayerBaseData);

		return true;
	}

	return false;
}

// ----------------------------------------------------------------------------
bool CGameMain::IsInWater()
{
	CRoleSprite* player = m_pPlayer;
	if (player)
	{
		return player->IsInWater();
	}

	return m_inWater;
}

// ----------------------------------------------------------------------------
void CGameMain::SetZoneChannel(int ZoneID, bool boOpen)
{
	if (boOpen)
	{
		m_zoneOpen.insert(ZoneID);
	}
	else
	{
		m_zoneOpen.erase(ZoneID);
	}
}

// ---------------------------------------------------------------------------
void CGameMain::RequestInstanceRecord()
{
	m_requestInstance = true;

	if (g_pInterface && g_pInterface->IsLoadingUI() == false)
	{
		m_instanceWaitTime = 60.0;

		// Åã¥Ü Dialog
		g_pInterface->SendWorldEvent("INSTANCE_CONFIRM_SHOW");

		m_requestInstance = false;
	}
}

// ---------------------------------------------------------------------------
void CGameMain::ClearInstanceWaitTime()
{
	m_instanceWaitTime = 0.0;

	// ²M°£ Dialog
	if (g_pInterface)
	{
		g_pInterface->SendWorldEvent("INSTANCE_CONFIRM_HIDE");
	}
}

// ---------------------------------------------------------------------------
void CGameMain::AcceptInstanceRecord()
{
	if (m_instanceWaitTime > 0.0f)
	{
		m_instanceWaitTime = 0.0f;

		Net_Instance::SL_JoinInstanceResult(true);
	}
}

// ---------------------------------------------------------------------------
void CGameMain::DeclineInstanceRecord()
{
	if (m_instanceWaitTime > 0.0f)
	{
		m_instanceWaitTime = 0.0f;

		Net_Instance::SL_JoinInstanceResult(false);
	}
}

// ---------------------------------------------------------------------------
void CGameMain::CheckMemoryStatus(bool forceUpdate)
{
	// ¨C 10 ¬í¨ú±o¤@¦¸°O¾ÐÅé¨Ï¥Î¶q
	if (m_frameTime - m_lastMemoryUpdateTime > 10.0f || forceUpdate)
	{
		m_lastMemoryUpdateTime = m_frameTime;

		MEMORYSTATUSEX statex;

		statex.dwLength = sizeof(statex);

		GlobalMemoryStatusEx(&statex);

		// ³Ì¤j°O¾ÐÅé
		statex.ullTotalPhys /= 1024 * 1024;

		// 
		statex.ullAvailPhys /= 1024 * 1024;

		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, GetCurrentProcessId());

		if (hProcess != NULL)
		{
			PROCESS_MEMORY_COUNTERS pmc;

			if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
			{
				/*
				printf( "\tPageFaultCount: 0x%08X\n", pmc.PageFaultCount );
				printf( "\tPeakWorkingSetSize: 0x%08X\n",
				pmc.PeakWorkingSetSize );
				printf( "\tWorkingSetSize: 0x%08X\n", pmc.WorkingSetSize );
				printf( "\tQuotaPeakPagedPoolUsage: 0x%08X\n",
				pmc.QuotaPeakPagedPoolUsage );
				printf( "\tQuotaPagedPoolUsage: 0x%08X\n",
				pmc.QuotaPagedPoolUsage );
				printf( "\tQuotaPeakNonPagedPoolUsage: 0x%08X\n",
				pmc.QuotaPeakNonPagedPoolUsage );
				printf( "\tQuotaNonPagedPoolUsage: 0x%08X\n",
				pmc.QuotaNonPagedPoolUsage );
				printf( "\tPagefileUsage: 0x%08X\n", pmc.PagefileUsage );
				printf( "\tPeakPagefileUsage: 0x%08X\n",
				pmc.PeakPagefileUsage );
				*/

				m_currentMemory = DWORD(pmc.WorkingSetSize / (1024 * 1024));
			}

			CloseHandle(hProcess);
		}

		m_memoryLoad = statex.dwMemoryLoad;

		//g_pError->AddMessage(0, 0, "[memory state] load:%d working:%d", m_memoryLoad, m_currentMemory);
	}

#ifndef NOCHECKMEM
	if (m_currentMemory > REDUCE_TEXTURE_DETAIL_SIZE)
	{
		if (m_reduceTextureState)
		{
			g_ruEngineSettings->GetSetting("paperdoll:max mip level", m_paperdollTextureDetail);
			g_ruEngineSettings->GetSetting("texture:max detail level", m_textureDetail);
			if (boost::get<INT32>(m_paperdollTextureDetail) > 2)
				g_ruEngineSettings->SetSetting("paperdoll:max mip level", 2);
			if (boost::get<INT32>(m_textureDetail) < 2)
				g_ruEngineSettings->SetSetting("texture:max detail level", 2);

			m_reduceTextureState = true;
		}
	}
	else if (m_reduceTextureState)
	{
		g_ruEngineSettings->SetSetting("paperdoll:max mip level", m_paperdollTextureDetail);
		g_ruEngineSettings->SetSetting("texture:max detail level", m_textureDetail);

		m_reduceTextureState = false;
	}
#endif //NOCHECKMEM
}

// ---------------------------------------------------------------------------
void CGameMain::AttachPoint(CRuVector3 position, CRuVector3 dir, CRuFusion_ActorState actorState)
{
	if (m_pPlayer == NULL)
		return;

	// ¨ú±o¨¤¦â©Ò´Â¤è¦V
	float theta = acos(dir.Dot(CRuVector3(1.0f, 0.0f, 0.0f)));
	if (dir.m_z > 0.0f)
		theta = PI + PI - theta;

	// Âà´«¬°¨¤«×
	theta = theta * 180.0f / PI;

	m_pPlayer->SetDirection(dir, true, true);
	m_pPlayer->SetPos(position);

	// §ïÅÜ§¤¼Ð
	SendPlayerPos(true, true);
	//Net_Move::PlayerMoveObject(m_pPlayer->GetIdentify(), position.m_x, position.m_y, position.m_z, theta, 0, 0, 0, 0, EM_ClientMoveType_SetPos);

	m_pPlayer->SetActorState(actorState);

	// §ïÅÜ°Ê§@
	SendPlayerActor();
}

// ---------------------------------------------------------------------------
bool CGameMain::SitToEntity(CRuEntity* pEntity)
{
	if (NULL == pEntity || m_pPlayer == NULL)
	{
		return false;
	}

	CRuArrayList<CRuLinkFrame*> linkFrames;
	int len;
	const char* linkName;

	float fNearest = 0;
	CRuLinkFrame* pLinkFrame = NULL;

	CRuVector3 distance;

	RuEntity_EnumerateLinkFrames(pEntity, linkFrames);
	for (int i = 0; i < linkFrames.Count(); ++i)
	{
		linkName = linkFrames[i]->GetLinkName();

		len = (int)strlen(linkName);
		if (len >= 11 && 0 == strncmp(linkName, "p_sit_chair", 11))
		{
			const CRuVector3& rTranslation = linkFrames[i]->GetTranslation(ruFRAMETYPE_WORLD);
			const CRuQuaternion& rRotation = linkFrames[i]->GetRotation(ruFRAMETYPE_WORLD);

			//§PÂ_¨¤«×
			CRuVector3 vecUp(0, 1, 0);
			vecUp = vecUp * rRotation.GetMatrix();

			//10«×
			if (0.9848 > DotProduct(vecUp, CRuVector3(0, 1, 0)))
			{
				continue;
			}

			//·Æ¹«ÂI¨ìªº¶ZÂ÷
			// °Ñ¼Æ
			INT32 x = g_pRuneDev->GetCursorPtX();		// ·Æ¹«x
			INT32 y = g_pRuneDev->GetCursorPtY();		// ·Æ¹«y
			IRuSwapChain* swapChain = g_pRuneDev->GetRuSwapChain();		// ¥ÑCRuneDev¨ú±oIRuSwapChain
			CRuCamera* camera = g_pRuneDev->GetCamera();
			// ±N·Æ¹«®y¼ÐÂà´«¦¨¥@¬É®y¼Ð
			CRuVector3 homPt, viewPt, worldPt;
			camera->Coord_ScreenToHomogeneous(x, y, swapChain->GetWidth(), swapChain->GetHeight(), homPt);
			camera->Coord_HomogeneousToView(homPt, viewPt);
			camera->Coord_ViewToWorld(viewPt, worldPt);

			// «Øºc¸I¼²´ú¸Õµ²ºc
			CRuCollisionQuery_Segment_Nearest query;

			// ³]©w´ú¸Õ¥Î½u¬q¤Î¨ä¥L³]©w
			CRuVector3 pickDir = (worldPt - camera->GetCameraPosition()).Normalize();
			query.m_colSegment = CRuSegment3(worldPt, worldPt + (pickDir * 1000.0f));
			query.m_ignoreBackface = TRUE;
			query.m_ignoreCollisionFlag = TRUE;

			// ´ú¸Õ¸I¼²
			pEntity->QueryCollision(&query);
			if (query.m_positiveHit)
			{
				distance = rTranslation - query.m_collisionPoint;
			}
			else
			{
				//»Pª±®a¶ZÂ÷
				distance = rTranslation - g_pGameMain->GetPlayer()->GetPos();
			}


			if (NULL == pLinkFrame || distance.Magnitude() < fNearest)
			{
				pLinkFrame = linkFrames[i];
				fNearest = distance.Magnitude();
			}
		}
	}

	if (pLinkFrame)
	{
		//»Pª±®a¶ZÂ÷
		distance = pLinkFrame->GetTranslation(ruFRAMETYPE_WORLD) - g_pGameMain->GetPlayer()->GetPos();
		fNearest = distance.Magnitude();

		if (fNearest <= DF_SIT_TO_CHAIR_MAX_DISTANCE)
		{
			const CRuVector3& rTranslation = pLinkFrame->GetTranslation(ruFRAMETYPE_WORLD);
			const CRuQuaternion& rRotation = pLinkFrame->GetRotation(ruFRAMETYPE_WORLD);

			//¤è¦V
			CRuVector3 vecDir(0, 0, -1);
			vecDir = vecDir * rRotation.GetMatrix();

			AttachPoint(rTranslation, vecDir, ruFUSION_ACTORSTATE_SIT_CHAIR_LOOP);

			return true;
		}
	}

	return false;

}


// ---------------------------------------------------------------------------
bool CGameMain::EntityIsChair(CRuEntity* pEntity, float* poutDistance)
{
	if (NULL == pEntity || m_pPlayer == NULL)
	{
		return false;
	}

	CRuArrayList<CRuLinkFrame*> linkFrames;
	int len;
	const char* linkName;

	float fNearest = 0;
	CRuLinkFrame* pLinkFrame = NULL;

	CRuVector3 distance;

	RuEntity_EnumerateLinkFrames(pEntity, linkFrames);
	for (int i = 0; i < linkFrames.Count(); ++i)
	{
		linkName = linkFrames[i]->GetLinkName();

		len = (int)strlen(linkName);
		if (len >= 11 && 0 == strncmp(linkName, "p_sit_chair", 11))
		{
			const CRuVector3& rTranslation = linkFrames[i]->GetTranslation(ruFRAMETYPE_WORLD);
			const CRuQuaternion& rRotation = linkFrames[i]->GetRotation(ruFRAMETYPE_WORLD);

			//§PÂ_¨¤«×
			CRuVector3 vecUp(0, 1, 0);
			vecUp = vecUp * rRotation.GetMatrix();

			//10«×
			if (0.9848 > DotProduct(vecUp, CRuVector3(0, 1, 0)))
			{
				continue;
			}

			//·Æ¹«ÂI¨ìªº¶ZÂ÷
			// °Ñ¼Æ
			INT32 x = g_pRuneDev->GetCursorPtX();		// ·Æ¹«x
			INT32 y = g_pRuneDev->GetCursorPtY();		// ·Æ¹«y
			IRuSwapChain* swapChain = g_pRuneDev->GetRuSwapChain();		// ¥ÑCRuneDev¨ú±oIRuSwapChain
			CRuCamera* camera = g_pRuneDev->GetCamera();
			// ±N·Æ¹«®y¼ÐÂà´«¦¨¥@¬É®y¼Ð
			CRuVector3 homPt, viewPt, worldPt;
			camera->Coord_ScreenToHomogeneous(x, y, swapChain->GetWidth(), swapChain->GetHeight(), homPt);
			camera->Coord_HomogeneousToView(homPt, viewPt);
			camera->Coord_ViewToWorld(viewPt, worldPt);

			// «Øºc¸I¼²´ú¸Õµ²ºc
			CRuCollisionQuery_Segment_Nearest query;

			// ³]©w´ú¸Õ¥Î½u¬q¤Î¨ä¥L³]©w
			CRuVector3 pickDir = (worldPt - camera->GetCameraPosition()).Normalize();
			query.m_colSegment = CRuSegment3(worldPt, worldPt + (pickDir * 1000.0f));
			query.m_ignoreBackface = TRUE;
			query.m_ignoreCollisionFlag = TRUE;

			// ´ú¸Õ¸I¼²
			pEntity->QueryCollision(&query);
			if (query.m_positiveHit)
			{
				distance = rTranslation - query.m_collisionPoint;
			}
			else
			{
				//»Pª±®a¶ZÂ÷
				distance = rTranslation - g_pGameMain->GetPlayer()->GetPos();
			}


			if (NULL == pLinkFrame || distance.Magnitude() < fNearest)
			{
				pLinkFrame = linkFrames[i];
				fNearest = distance.Magnitude();
			}
		}
	}

	if (pLinkFrame)
	{
		//»Pª±®a¶ZÂ÷
		distance = pLinkFrame->GetTranslation(ruFRAMETYPE_WORLD) - g_pGameMain->GetPlayer()->GetPos();
		fNearest = distance.Magnitude();

		if (poutDistance)
		{
			*poutDistance = fNearest;
		}
		return true;
	}

	return false;
}

// ---------------------------------------------------------------------------
void CGameMain::SaveClientSetting(bool Immediately)
{
	if (memcmp(&m_clientSetting, &m_clientSettingBackup, sizeof(ClientSaveSetting)) != 0)
	{
		if (Immediately || m_frameTime - m_lastClientSaveTime > 10.0f)
		{
			SaveClientData(eSaveClientData_Setting, sizeof(m_clientSetting), (char*)&m_clientSetting);

			m_clientSettingBackup = m_clientSetting;

			m_lastClientSaveTime = m_frameTime;
		}
	}
}

// ---------------------------------------------------------------------------
void CGameMain::SaveClientData(int keyID, int size, char* data)
{
	if (data == NULL || size <= 0)
		return;

	MyLzoClass myLzo;
	char* compressData = new char[size + (size >> 1) + 12];

	DWORD encodeSize = myLzo.Encode(data, size, compressData);
	DWORD code = SAVE_CHECK_CODE;
	if (encodeSize > 0)
	{
		// °O¿ý­ì©l¤j¤p
		memcpy(compressData + encodeSize, &size, sizeof(DWORD));

		// °O¿ýÀË¬d½X
		memcpy(compressData + encodeSize + sizeof(DWORD), &code, sizeof(DWORD));

		// Àx¦s­ì©l¤j¤p
		encodeSize += sizeof(DWORD) + sizeof(DWORD);

		// °e¥X«Ê¥]
		NetCli_Other::SL_SaveClientData(keyID, encodeSize, compressData);
	}

	delete[] compressData;
}

// ---------------------------------------------------------------------------
void CGameMain::SendLoadClientData()
{
	if (m_isLoadClientData == false)
	{
		for (int i = eSaveClientData_Header; i < eSaveClientData_Last; i++)
		{
			m_sendKeyTime[i] = 20.0f;			// µ¥«ÝÅª¨úµ²§ô

			NetCli_Other::SL_LoadClientData(i);
		}

		m_isLoadClientData = true;
	}
}

// ---------------------------------------------------------------------------
void CGameMain::ReceiveLoadClientData(int keyID, int size, char* data)
{
	// ¦³Åª¨ú¨ì¶µ¥Ø²M°£µ¥«Ý¸ê®Æ
	m_sendKeyTime.erase(keyID);

	if (data == NULL || size <= 0)
		return;
	if (keyID != eSaveClientData_SkillPlate && (keyID > eSaveClientData_Setting && keyID < eSaveClientData_GameBar && m_clientSetting.isUseServerData == false))
		return;

	MyLzoClass myLzo;
	DWORD originalSize = 0;
	DWORD checkCode = 0;

	DWORD dataSize = size - sizeof(DWORD) - sizeof(DWORD);

	// ¨ú±oÀË¬d½X
	memcpy(&checkCode, data + dataSize + sizeof(DWORD), sizeof(DWORD));

	if (checkCode != SAVE_CHECK_CODE)
		return;

	// ¨ú±o­ì©l¤j¤p
	memcpy(&originalSize, data + dataSize, sizeof(DWORD));

	// ¶W¹L 1M Bytes
	if (originalSize == 0 || originalSize > 1024 * 1024)
		return;

	char* originalData = new char[originalSize];
	myLzo.Decode(data, dataSize, originalSize, originalData);

	switch (keyID)
	{
	case eSaveClientData_Header:
		break;

	case eSaveClientData_Setting:
		// Åª¨ú°ò¥»³]©w¸ê®Æ
	{
		ClientSaveSetting setting;
		memcpy(&setting, originalData, min(originalSize, sizeof(ClientSaveSetting)));

		if (setting.isUseServerData) {
			ClientSaveSetting clientSetting = m_clientSetting;
			memcpy(&m_clientSetting, originalData, min(originalSize, sizeof(ClientSaveSetting)));
			m_clientSetting.isLastSelectedRealm = clientSetting.isLastSelectedRealm;
			m_clientSetting.isLastAccountName = clientSetting.isLastAccountName;
		}
		else {
			m_clientSetting.isUseServerData = false;
		}

		m_clientSettingBackup = m_clientSetting;
	}
	break;
	}

	if (m_interface)
		m_interface->PushClientData(keyID, originalSize, originalData);

	delete[] originalData;
}

bool CGameMain::CheckCountry(const char* country)
{
	if (stricmp(country, "JP") == 0)
	{
		if (stricmp(m_country.c_str(), "HG") == 0)
		{
			return true;
		}
	}

	if (stricmp(m_country.c_str(), country) == 0)
	{
		return true;
	}

	return false;
}

// ---------------------------------------------------------------------------
void CGameMain::PushRoomValue(int keyID, int value)
{
	map<int, int>::iterator iter = m_roomValue.find(keyID);
	if (iter != m_roomValue.end())
	{
		RemoveRoomValue(iter);
	}

	//g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", "SetRoomValue %d, %d", keyID, value);

	switch (keyID)
	{
		// ³õ´º¨Æ¥óÃþ«¬
	case 0:
	{
		char eventStr[256];
		sprintf_s(eventStr, 256, "Type%d", value);
		PushSpecialEvent(eventStr);
	}
	break;

	// ³õ´º­I´º­µ¼Ö
	case 1:
	{

		char musicKey[512];
		sprintf_s(musicKey, 512, "SYS_PATH_MUSIC_%d", value);
		const char* musicPath = g_ObjectData->GetString(musicKey);
		if (musicPath)
		{
			// g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", "Play Music [%s]", musicPath);

			g_pRuGlobalEventManager->PlayMusicByFile(musicPath, 1.0f, TRUE);
			if (g_pRuGlobalEventManager->GetMusicSoundHandle() != 0)
			{
				g_pRuGlobalEventManager->SetEnvironmentMusicEnable(FALSE);
			}
		}
	}
	break;

	// §ïÅÜ³õ´º¥ú·½
	case 2:
	{
		m_forceMainLightColor = (DWORD)value | 0xFF000000;
	}
	break;
	}

	m_roomValue[keyID] = value;
}

// ---------------------------------------------------------------------------
map<int, int>::iterator CGameMain::RemoveRoomValue(int keyID)
{
	map<int, int>::iterator iter = m_roomValue.find(keyID);
	if (iter != m_roomValue.end())
	{
		return RemoveRoomValue(iter);
	}
	return m_roomValue.end();
}

// ---------------------------------------------------------------------------
map<int, int>::iterator CGameMain::RemoveRoomValue(map<int, int>::iterator iter)
{
	switch (iter->first)
	{
	case 0:
	{
		char eventStr[256];
		sprintf_s(eventStr, 256, "Type%d", iter->second);

		RemoveSpecialEvent(eventStr);
	}
	break;

	case 1:
	{
		g_pRuGlobalEventManager->StopMusic();
		g_pRuGlobalEventManager->SetEnvironmentMusicEnable(TRUE);
	}
	break;

	case 2:
	{
		m_forceMainLightColor = 0;
	}
	break;

	default:
		break;
	}

	return m_roomValue.erase(iter);
}

// ---------------------------------------------------------------------------
void CGameMain::ClearRoomValue()
{
	map<int, int>::iterator iter = m_roomValue.begin();
	while (iter != m_roomValue.end())
	{
		iter = RemoveRoomValue(iter);
	}
}

// ---------------------------------------------------------------------------
void CGameMain::SetCameraFollowSprite(CRoleSprite* sprite)
{
	m_cameraFollowSprite = sprite;
}

// ---------------------------------------------------------------------------
void CGameMain::CreateTargetEffect(TargetLineEffectType targetType, CRoleSprite* referenceSprite)
{
	if (referenceSprite == NULL || m_targetMaxRange == 0)
		return;

	// ÄÀ©ñÂÂªº¸ê®Æ
	ReleaseTargetEffect();

	// ¦s¦b®É¶¡
	m_targetDisplayDuration = 10.0f;

	// °O¿ýÃþ«¬¥H¤Î¤è¦V
	m_targetDirection = referenceSprite->GetDirection();

	// ²£¥Íª«¥ó
	CreateTargetEntity();

	// ³sµ²°Ñ¦ÒÂI
	SetTargetEntityTarget(referenceSprite->GetRuEntity(), ATTACH_POINT_SHOOT);

	// ³]©w½u¬qÃþ«¬
	SetTargetEffectType(targetType, 50);

	// §ó·s§¤¼Ð
	UpdateTargetEffectPosition();
}

// ---------------------------------------------------------------------------
void CGameMain::ReleaseTargetEffect()
{
	m_targetReferenceSprite = NULL;

	m_targetLineType = TargetLineEffect_None;

	RelaseTargetEntity();
}

// ---------------------------------------------------------------------------
bool CGameMain::RotateTargetEffect(float xDelta, float yDelta)
{
	CRuQuaternion cycleQuat;
	cycleQuat.FromAngleAxis(CRuVector3(0.0, 1.0f, 0.0f), xDelta);
	cycleQuat.TransformVector(m_targetDirection);
	m_targetDirection.Normalize();

	float theta = m_targetDirection.Dot(CRuVector3(0.0f, 1.0f, 0.0f));
	if (theta + yDelta > 0.9f)
		yDelta = 0.9f - theta;
	if (theta + yDelta < -0.77f)
		yDelta = -0.77f - theta;

	CRuVector3 xBasis;

	// Calculate the x basis vector
	xBasis.SetToCrossProduct(CRuVector3(0.0f, 1.0f, 0.0f), m_targetDirection);

	// Normalize the x basis vector
	xBasis.Normalize();

	cycleQuat.FromAngleAxis(xBasis, yDelta);
	cycleQuat.TransformVector(m_targetDirection);

	m_targetDisplayDuration = 10.0f;
	return true;
}

// ---------------------------------------------------------------------------
bool CGameMain::UpdateTargetEffectPosition()
{
	CRoleSprite* player = GetPlayer();

	if (m_targetPolyLineEntity == NULL || m_targetEntity == NULL)
		return false;

	// ±j¨î­×¥¿¨¤«×
	if (m_targetMaxAngle > 0.0f && m_targetMaxAngle < 180.0f)
	{
		CRoleSprite* player = GetPlayer();
		CRoleSprite* implement = (player) ? player->GetImplementSprite() : NULL;
		implement = player;
		float t;

		if (m_targetMaxAngle > 90)
		{
			t = PI / 2.0f + cos((180.0f - m_targetMaxAngle) * PI / 180.0f);
		}
		else
		{
			t = cos((90.0f - m_targetMaxAngle) * PI / 180.0f);
		}

		if (implement)
		{
			CRuVector3 p1 = implement->GetDirection();
			CRuVector3 p2 = m_targetDirection;
			p1.m_y = 0.0f;
			p2.m_y = 0.0f;
			p1.Normalize();
			p2.Normalize();

			// ¨ú±o¨â­Ó¦V¶q§¨¨¤
			float theta = mine_acos(p1.Dot(p2));
			if (theta > t)
			{
				CRuQuaternion cycleQuat;

				// ¥Ñ¦V¶q¿n¨ú±o¨â¦V¶q ¬°¥¿¦V±ÛÂà©Î
				CRuVector3 vecCross = CrossProduct(p1, p2);
				vecCross.Normalize();

				float angle = mine_acos(vecCross.Dot(CRuVector3(0.0f, 1.0f, 0.0f)));
				if (angle > -0.1 && angle < 0.1)
				{
					cycleQuat.FromAngleAxis(CRuVector3(0.0, 1.0f, 0.0f), theta - t);
				}
				else
				{
					cycleQuat.FromAngleAxis(CRuVector3(0.0, 1.0f, 0.0f), -(theta - t));
				}
				cycleQuat.TransformVector(m_targetDirection);
			}
		}
	}

	// À°¯¥¶ðÂà¦V
	if (player && player->GetImplementSprite())
	{
		CRoleSprite* implementSprite = player->GetImplementSprite();
		if (implementSprite && player->IsImplementAlignment())
		{
			implementSprite->SetDirection(m_targetDirection, true);
		}
	}

	// ¼È®É¤£ÁôÂÃ
	m_targetDisplayDuration = 10.0f;

	// ¬O§_Åã¥Üª«¥ó
	if (m_targetDisplayDuration > 0.0f)
	{
		CRuVector3 direction = m_targetDirection;
		CRuVector3 startPoint = m_targetPolyLineEntity->GetTranslation(ruFRAMETYPE_WORLD);
		CRuVector3 endPoint = startPoint + direction * m_targetMaxRange;

		// ­Ë¼Æ®É¶¡
		m_targetDisplayDuration -= m_elapsedTime;

		// «Øºc¸I¼²´ú¸Õµ²ºc
		CRuCollisionQuery_Segment_Nearest segQuery;

		switch (m_targetLineType)
		{
		case TargetLineEffect_Line:
		{

			// ³]©w´ú¸Õ¥Î½u¬q¤Î¨ä¥L³]©w
			segQuery.m_colSegment = CRuSegment3(startPoint, endPoint);
			segQuery.m_exclusionMask = RuWorld::ruTRIFLAGS_WATER | RuWorld::ruTRIFLAGS_FOLIAGE;
			segQuery.m_ignoreBackface = FALSE;
			segQuery.m_ignoreCollisionFlag = TRUE;

			IBaseSprite::GetCollisionContainer()->QueryCollision(&segQuery);

			if (segQuery.m_positiveHit)
			{
				endPoint = segQuery.m_collisionPoint;
			}
		}
		break;

		case TargetLineEffect_Parabola:
		{
			FLOAT angle = DotProduct(direction, CRuVector3(0.0f, 1.0f, 0.0f));

			// ¨¤«×¤j°ª©¹¦^½Õ¾ã
			if (angle > 0.15f)
			{
				CRuQuaternion cycleQuat;
				CRuVector3 xBasis;

				// Calculate the x basis vector
				xBasis.SetToCrossProduct(CRuVector3(0.0f, 1.0f, 0.0f), direction);

				// Normalize the x basis vector
				xBasis.Normalize();

				cycleQuat.FromAngleAxis(xBasis, 0.15f - angle);
				cycleQuat.TransformVector(direction);
			}

			endPoint = startPoint + direction * m_targetMaxRange;

			if (m_targetParabolaHeight != 0.0f)
			{
				vector<CRuVector3> segments;
				CRuVector3 forward = (endPoint - startPoint).Normalize();
				float segmentLength = 40.0f;
				int sIdx = 0;

				CRuVector3 p0 = startPoint;
				while (sIdx < 25)
				{
					CRuVector3 p1 = p0 + forward * segmentLength;

					segments.push_back(p1);

					p0 = p1;

					sIdx++;
				}

				FLOAT xDis = (endPoint - startPoint).Magnitude();

				if (m_targetParabolaHeight != 0.0f)
				{
					CRuVector3 p2, p3;
					p2 = startPoint;

					for (INT32 i = 0; i < segments.size(); ++i)
					{
						// Scale T such that its range is in [-1, 1]
						REAL t = (segments[i] - startPoint).Magnitude() / xDis * 2.0f - 1.0f;

						p3 = segments[i];

						p3.m_y = p3.m_y + m_targetParabolaHeight - t * t * m_targetParabolaHeight;

						// ³]©w´ú¸Õ¥Î½u¬q¤Î¨ä¥L³]©w
						segQuery.m_colSegment = CRuSegment3(p2, p3);
						segQuery.m_exclusionMask = RuWorld::ruTRIFLAGS_WATER | RuWorld::ruTRIFLAGS_FOLIAGE;
						segQuery.m_ignoreBackface = FALSE;
						segQuery.m_ignoreCollisionFlag = TRUE;

						IBaseSprite::GetCollisionContainer()->QueryCollision(&segQuery);

						if (segQuery.m_positiveHit)
						{
							endPoint = segQuery.m_collisionPoint;
							break;
						}

						p2 = p3;
					}
				}
			}
			else
			{
				// ³]©w´ú¸Õ¥Î½u¬q¤Î¨ä¥L³]©w
				segQuery.m_colSegment = CRuSegment3(startPoint, endPoint);
				segQuery.m_exclusionMask = RuWorld::ruTRIFLAGS_WATER | RuWorld::ruTRIFLAGS_FOLIAGE;
				segQuery.m_ignoreBackface = FALSE;
				segQuery.m_ignoreCollisionFlag = TRUE;

				IBaseSprite::GetCollisionContainer()->QueryCollision(&segQuery);

				if (segQuery.m_positiveHit)
				{
					endPoint = segQuery.m_collisionPoint;
				}
			}
		}
		break;
		}

		// §¤¼Ð¤£¦P¦s¦b®É¶¡­«·s­pºâ
		if (m_targetEntity->GetTranslation(ruFRAMETYPE_WORLD) != endPoint)
		{
			m_targetEntity->SetTranslation(endPoint);
			m_targetDisplayDuration = 10.0f;
		}

		SetTargetEntityVisible(true);
	}
	else
	{
		SetTargetEntityVisible(false);
	}

	return true;
}

// ---------------------------------------------------------------------------
void CGameMain::SetTargetEffectDirection(CRuVector3 dir)
{
	m_targetDirection = dir;
	m_targetDirection.Normalize();
}

// ---------------------------------------------------------------------------
void CGameMain::SetTargetEffectType(TargetLineEffectType targetType, int height)
{
	if (m_targetLineType != targetType || m_targetParabolaHeight != height)
	{
		m_targetLineType = targetType;
		m_targetParabolaHeight = (float)height;
		m_targetDisplayDuration = 10.0f;

		switch (m_targetLineType)
		{
		case TargetLineEffect_Line:			// ½u¬q					
			CEffectSprite::SetPloyLineArgument(m_targetPolyLineEntity, 0.0f, -1.0f, 0.0f, 2.0f);
			break;

		case TargetLineEffect_Parabola:		// ©ßª«½u
			CEffectSprite::SetPloyLineArgument(m_targetPolyLineEntity, 0.0f, -1.0f, m_targetParabolaHeight, 2.0f);
			break;
		}
	}
}

// ---------------------------------------------------------------------------
void CGameMain::RegisterZoneChangeCallBack(ZoneDescriptorCallback event)
{
	m_ZoneDescriptorList.push_back(event);
}

// ---------------------------------------------------------------------------
BOOL CGameMain::QueryCollision(CRuVector3& beginPoint, CRuVector3& endPoint, CRuVector3& outCollisionPoint, bool boTerrain, bool spriteCollision, CRuEntity* ignoreEntity)
{
	CRoleSprite* player = GetPlayer();
	REAL nearestMountPointDist = FLT_MAX;

	CRuCollisionQuery_Segment_Nearest segQuery;

	if (boTerrain)
	{
		// ³]©w´ú¸Õ¥Î½u¬q¤Î¨ä¥L³]©w
		segQuery.m_colSegment = CRuSegment3(beginPoint, endPoint);
		segQuery.m_exclusionMask = RuWorld::ruTRIFLAGS_WATER /*| RuWorld::ruTRIFLAGS_FOLIAGE*/;
		segQuery.m_ignoreBackface = FALSE;
		segQuery.m_ignoreCollisionFlag = FALSE;

		CRuContainer_Collision* collisionContainer = IBaseSprite::GetCollisionContainer();
		if (collisionContainer)
		{
			BOOL removeResult = FALSE;

			// ¼È®É²¾°£¦Û¤v,Á×§K´ú¸Õ¨ì¦Û¤w
			if (ignoreEntity)
			{
				removeResult = collisionContainer->RemoveCollisionObject(ignoreEntity);
			}

			collisionContainer->QueryCollision(&segQuery);

			if (ignoreEntity && removeResult)
				collisionContainer->InsertCollisionObject(ignoreEntity, ignoreEntity->GetWorldBounds(), TRUE);
		}

		// Keep the closest candidate sprite
		if (segQuery.m_positiveHit && segQuery.m_collisionT < nearestMountPointDist)
		{
			nearestMountPointDist = segQuery.m_collisionT;
			outCollisionPoint = segQuery.m_collisionPoint;
		}
	}

	return nearestMountPointDist < FLT_MAX;
}

// ---------------------------------------------------------------------------
float CGameMain::GetMagicFlyTime(CRuVector3& p1, CRuVector3& p2, int magicID, int parabolaHeight)
{
	float distance = (p1 - p2).Magnitude();

	GameObjDbStruct* magicDB = g_ObjectData->GetObj(magicID);
	GameObjDbStruct* imageDB = (magicDB) ? g_ObjectData->GetObj(magicDB->ImageID) : NULL;

	if (imageDB)
	{
		// ¦³­¸¦æ¯S®Ä
		if (strlen(imageDB->Image.FlyACT) > 0)
		{
			if (imageDB->Image.IsPolyLineEffect)
				return 0.2f;

			float speed = 150.0f * (imageDB->Image.ShootMotionScale / 100.0f);
			switch (imageDB->Image.MagicShowType)
			{
			case EM_GameObjectMagicShowType_Single:
			case EM_GameObjectMagicShowType_Multi:
			case EM_GameObjectMagicShowType_PolyLine:
				return (distance / speed) - 0.2f;
				break;

			case EM_GameObjectMagicShowType_Parabola:
			case EM_GameObjectMagicShowType_ParabolaMulti:
				return CParabolaEffectSprite::GetMagicParabolaTime(p1, p2, parabolaHeight, speed) - 0.2f;
				break;
			}
		}
	}

	return 0.0f;
}

// ---------------------------------------------------------------------------
bool CGameMain::InsertAnchorHeightSprite(int id, CRuVector3& position)
{
	if (m_isWaitGlobalObject == false)
		return false;

	m_spriteAnchorHeight[id] = position;
	return true;
}

// ---------------------------------------------------------------------------
bool DifferencePositionXZ(CRuVector3& p1, CRuVector3& p2)
{
	if (abs(p1.m_x - p2.m_x) > 10.0f)
		return true;
	if (abs(p1.m_z - p2.m_z) > 10.0f)
		return true;
	return false;
}

// ---------------------------------------------------------------------------
bool CGameMain::IsAnchorHeightSpriteLoadComplete()
{
	if (m_spriteAnchorHeight.size() == 0)
		return true;

	CRuAABB bounds(-10000.0f, -10000.0f, -10000.0f, 10000.0f, 10000.0f, 10000.0f);
	if (m_pRuTerrain)
	{
		bounds = m_pRuTerrain->GetEnumerationBounds();
	}

	map<CRoleSprite*, CRuVector3> legalList;
	map<int, CRuVector3>::iterator iter = m_spriteAnchorHeight.begin();
	while (iter != m_spriteAnchorHeight.end())
	{
		CRoleSprite* sprite = FindSpriteByID(iter->first);

		// ª«¥ó¤£¦s¦b©ÎªÌ®t¶Z¹L¤j(¤w¸g¦³²¾°Ê¹L)
		if (sprite == NULL || sprite->GetRuEntity() == NULL || DifferencePositionXZ(sprite->GetPos(), iter->second))
			iter = m_spriteAnchorHeight.erase(iter);
		else if (sprite->IsProxyLoadComplete() == false && bounds.Contains(sprite->GetPos()) == TRUE)
			return false;
		else
		{
			legalList[sprite] = iter->second;
			iter++;
		}
	}

	for (map<CRoleSprite*, CRuVector3>::iterator iter = legalList.begin(); iter != legalList.end(); iter++)
	{
		CRoleSprite* proxyControlSprite = (iter->first)->GetProxyControlSprite();
		CRuVector3 p3 = iter->second;
		QueryCollision(iter->second + CRuVector3(0.0f, 20.0f, 0.0f), iter->second + CRuVector3(0.0f, -25.0f, 0.0f), p3, true, true, (iter->first)->GetRuEntity());
		proxyControlSprite->SetPos(p3);
	}

	m_spriteAnchorHeight.clear();
	return true;
}

// ---------------------------------------------------------------------------
void CGameMain::SetPaperdollTextureDetail(int level)
{
	if (m_reduceTextureState)
		m_paperdollTextureDetail = level;
	else
		g_ruEngineSettings->SetSetting("paperdoll:max mip level", level);
}

// ---------------------------------------------------------------------------
int	CGameMain::GetPaperdollTextureDetail()
{
	if (m_reduceTextureState)
		return boost::get<INT32>(m_paperdollTextureDetail);
	else
	{
		RuPropertyType propertyValue;
		g_ruEngineSettings->GetSetting("paperdoll:max mip level", propertyValue);
		return boost::get<INT32>(propertyValue);
	}
}

// ---------------------------------------------------------------------------
void CGameMain::SetTextureDetail(int level)
{
	if (m_reduceTextureState)
		m_textureDetail = level;
	else
		g_ruEngineSettings->SetSetting("texture:max detail level", level);
}

// ---------------------------------------------------------------------------
int	CGameMain::GetTextureDetail()
{
	if (m_reduceTextureState)
		return boost::get<INT32>(m_textureDetail);
	else
	{
		RuPropertyType propertyValue;
		g_ruEngineSettings->GetSetting("texture:max detail level", propertyValue);
		return boost::get<INT32>(propertyValue);
	}
}

// ---------------------------------------------------------------------------
void CGameMain::MovePartition(bool enable)
{
	if (m_movePartitionEnable != enable)
	{
		m_movePartitionEnable = enable;

		if (m_movePartitionEnable)
		{
			g_pChatEdit->ParseText("? setvalue lockpartition 1");
		}
		else
		{
			g_pChatEdit->ParseText("? setvalue lockpartition 0");
		}
	}
}
#ifdef KALYDO
// ---------------------------------------------------------------------------
void CGameMain::detectWDBParts(CRuDataStore_MultiFile* mapFileDataStore)
{
	ruSAFE_RELEASE(mapFileDataStore);

	mapFileDataStore = ruNEW CRuDataStore_MultiFile();

	if (mapFileDataStore->Open(g_pGameMain->GetFullFilePath(g_pGameMain->m_mapName.c_str()), TRUE))
	{
		g_pGameMain->m_WDBDownloadProgress = 0.2f;
		if (mapFileDataStore->GetFilesPending() == 0)
		{
			g_pGameMain->m_WDBDownloadProgress = 0.4f;
			CRuOptimalTerrain_DataSource_DS* terrainDS = ruNEW CRuOptimalTerrain_DataSource_DS();

			if (terrainDS->Open(mapFileDataStore))
			{
				g_pGameMain->m_WDBDownloadProgress = 0.6f;

				if (mapFileDataStore->GetFilesPending() == 0)
				{
					g_pGameMain->m_WDBDownloadProgress = 0.8f;
					// «Ø¥ß¦aªíºÞ²zª«¥ó
					CRuOptimalTerrain* terrain = ruNEW CRuOptimalTerrain();
					terrain->Initialize();
					terrain->AttachDataSource(terrainDS);
					if (mapFileDataStore->GetFilesPending() == 0)
					{
						g_pGameMain->m_WDBDownloadProgress = 1.0f;

						// everything is downloaded!
						ruSAFE_RELEASE(mapFileDataStore);
						g_pGameMain->finishWDB();
					}

					ruSAFE_RELEASE(terrain);
				}
			}
			ruSAFE_RELEASE(terrainDS);
		}
	}

	if (mapFileDataStore)
	{
		// still downloading ...
		mapFileDataStore->SetDownloadCallback(&CGameMain::detectWDBParts);
	}

}

void CGameMain::handleWDB(unsigned int errorType, unsigned int errorCode, void* id,
	void* message, unsigned int mLength)
{
	std::string* packageName = reinterpret_cast<std::string*>(id);

	// we receive a message about the download of a WDB
	if (errorType == CE_PROGRESS)
	{
		if (strstr(packageName->c_str(), "world") == NULL)
		{
			// progress for non-world WDB's
			m_WDBDownloadProgress = (float)errorCode / 100.0f;
		}
		return;
	}

	KASSERT(errorType == CE_NOERROR, "Error receiving WDB package!", "error", ET_PROGGEN, SL_ERROR);


	std::string fileName("wdb\\");
	fileName += *packageName;
	fileName += ".krf";
	TKResult res = krfLoadPackage(fileName.c_str());
	KASSERT(KRF_SUCCESS(res), "Error loading WDB package!", "error", ET_PROGGEN, SL_ERROR);
	if (strstr(packageName->c_str(), "world") != NULL)
	{
		// this is a world WDB, which is too big to download in one part.
		// detect parts that are needed.
		CGameMain::detectWDBParts(NULL);
	}
	else
	{
		// complete WDB loading
		m_WDBDownloadProgress = 1.0f;
		finishWDB();
	}

	delete packageName;
}

void CGameMain::finishWDB()
{
	m_isDownloading = false;
	if (m_shouldSetCameraPos)
	{
		m_shouldSetCameraPos = false;
		SetPlayerCameraPos(m_cameraPos);
	}
}

#endif // KALYDO