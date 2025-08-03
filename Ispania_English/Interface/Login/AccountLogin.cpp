#include <Windows.h>
#include <Shlwapi.h>
#include <shellapi.h>
#include "../../mainproc/GameMain.h"
#include "../../netwaker_member/NetWakerClientInc.h"
#include "../../Tools/Cini/IniFile2.h"
#include "../../tools/Common/Com_Function.h"
#include "AccountLogin.h"
#include "CharacterCreate.h"
#include "CharacterSelect.h"
#include "../../Interface/PartyBoard/PartyBoardFrame.h"
#include "../../tools/md5/Mymd5.h"
#include "../../tools/bcrypt/ow-crypt.h"

CAccountLogin*						g_pAccountLogin = NULL;

int	LuaFunc_DefaultServerLogin			(lua_State *L);
int	LuaFunc_DisconnectFromServer		(lua_State *L);
int LuaFunc_StatusDialogClick			(lua_State *L);
int LuaFunc_AcceptUserAgreement			(lua_State *L);
int LuaFunc_CancelWaitingQueue			(lua_State *L);

int	LuaFunc_GetCurrentRealm				(lua_State *L);
int LuaFunc_GetNumRealms				(lua_State *L);
int LuaFunc_GetRealmInfo				(lua_State *L);
int LuaFunc_ChangeRealm					(lua_State *L);
int	LuaFunc_GetUserAgreementText		(lua_State *L);
int	LuaFunc_IsDisplayVivoxUserAgreement	(lua_State *L);
int LuaFunc_GetVivoxUserAgreementText	(lua_State* L);
int	LuaFunc_AcceptVivoxUserAgreement	(lua_State* L);
int	LuaFunc_DeclineVivoxUserAgreement	(lua_State* L);
int LuaFunc_GetAccountName				(lua_State *L);
int LuaFunc_SetSecondPassword			(lua_State *L);

int LuaFunc_AccountLoginShow			(lua_State *L);
int LuaFunc_GetCurrentParallelID		(lua_State* L);
//
int LuaFunc_SetCameraLayout				(lua_State* L);			// 控制攝影機
int LuaFunc_AddControlCamera			(lua_State* L);			// 控制攝影機

int LuaFunc_GetCameraLayout				(lua_State* L);			// 

// Server List
int	LuaFunc_GetNumServerList			(lua_State* L);
int	LuaFunc_GetServerListInfo			(lua_State* L);
int LuaFunc_SortServerList				(lua_State* L);
int	LuaFunc_ChangeServerList			(lua_State* L);
int LuaFunc_GetCurrentServerList		(lua_State* L);

// Tip board
int LuaFunc_GetServerBoardText			(lua_State* L);
int LuaFunc_RefreshServerList			(lua_State* L);
int LuaFunc_CloseServerList				(lua_State* L);

int	LuaFunc_LinkActivateWeb				(lua_State* L);
int	LuaFunc_OpenActivateExecute			(lua_State* L);
int	LuaFunc_OpenResetPasswordURL		(lua_State* L);

int LuaFunc_IsBeanFanSystem				(lua_State* L);
int LuaFunc_ConfirmPassword				(lua_State* L);
int LuaFunc_GetAccountAge				(lua_State* L);
int LuaFunc_GetServerLimitAge			(lua_State* L);

int LuaFunc_GetServerName				(lua_State* L);

int LuaFunc_GetPasswordErrorCount		(lua_State* L);

int LuaFunc_ConfirmPasswordV2			(lua_State* L);
int LuaFunc_CaptchaRefresh				(lua_State* L);
int LuaFunc_CaptchaReply				(lua_State* L);

string g_serverListLastSortItem;
bool g_serverListSortNegative			= false;

CompressStringData::CompressStringData()
{
	data = NULL;
	decodeSize = 0;
	size = 0;
}

CompressStringData::~CompressStringData()
{
	Clear();
}

void CompressStringData::Clear()
{
	if ( data ) {
		delete [] data;
		data = NULL;
	}

	decodeSize = 0;
	size = 0;
}

void CompressStringData::SetString(const char* str)
{
	Clear();

	if ( str == NULL )
		return;

	size = (int)strlen(str) + 1;

	MyLzoClass myLzo;
	data = new char[size + (size>>1) + 12];
	decodeSize = myLzo.Encode(str, size, data);
}

std::string CompressStringData::GetString()
{
	char temp[1024];

	MyLzoClass myLzo;	
	myLzo.Decode(data, decodeSize, size, temp);

	return temp;
}

// ----------------------------------------------------------------------------
CAccountLogin::CAccountLogin(CInterface* object) : CInterfaceBase(object)
{
	m_connect.gameServer	= LOGIN_DISCONNECT;
	m_connect.realmList		= LOGIN_DISCONNECT;	
	m_serverListData		= false;
	m_gameServerPort		= 0;	

	m_secondPassword.clear();
	m_realmName.clear();
	m_account.clear();
	m_gameServerIP.clear();
	m_selctedPlayerName.clear();

	m_ignoreLogout			= false;

	m_changeParallelID		= -1;
	m_worldID				= 0;
	m_lastReciveServerListTime = 0.0f;
	m_passwordFailedCount	= 0;

	g_pAccountLogin	= this;
	m_showConfirmPassword	= false;

	m_selectServerList = -1;
	m_serverAge			= 0;

	//Board String
	m_cbBoardStringDownloaded   = NULL;
	m_boardStringDownloadStatus = eBoardStringNotDownloaded;
	m_vivoxLincenseTime			= 0;

	m_inputErrorCount			= 0;
	m_returnSelectCharacter		= false;

	m_PasswordErrorCount = 0;
	m_MaxPasswordErrorCount = 0;
	m_FreezeTime = 0;
	m_AccountFreezeRemainingTime = 0;

#ifdef KALYDO
	m_ShouldLogin = false;
#endif // KALYDO

	/*
	FILE* fptr = NULL;
	if ( (fptr = fopen("D:\\ServerList.ini", "r")) != NULL )
	{
		int characters[30] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29 };
		fseek(fptr, 0, SEEK_END);
		int size = ftell(fptr);
		char* temp = new char[size];
		fseek(fptr, 0, SEEK_SET);
		fread(temp, size, 1, fptr);
		LoadServerList(temp, size, characters);
		delete [] temp;
	}
	*/

	//m_passGameServerIP = "gs1.Runewaker.com";
	//m_passGameServerPort = 20000;
}

// ----------------------------------------------------------------------------
CAccountLogin::~CAccountLogin()
{
	g_pAccountLogin	= NULL;
}

// ----------------------------------------------------------------------------
void CAccountLogin::RegisterFunc()
{	
	lua_State* L = GetLuaStateLogin();
	if (L)
	{
		lua_register(L, "DefaultServerLogin",		LuaFunc_DefaultServerLogin);
		lua_register(L, "DisconnectFromServer",		LuaFunc_DisconnectFromServer);
		lua_register(L, "StatusDialogClick",		LuaFunc_StatusDialogClick);
		lua_register(L, "AcceptUserAgreement",		LuaFunc_AcceptUserAgreement);
		lua_register(L, "CancelWaitingQueue",		LuaFunc_CancelWaitingQueue);

		lua_register(L, "GetCurrentRealm",			LuaFunc_GetCurrentRealm);
		lua_register(L, "GetNumRealms",				LuaFunc_GetNumRealms);
		lua_register(L, "GetRealmInfo",				LuaFunc_GetRealmInfo);
		lua_register(L, "ChangeRealm",				LuaFunc_ChangeRealm);
		lua_register(L, "GetUserAgreementText",		LuaFunc_GetUserAgreementText);
		lua_register(L, "IsDisplayVivoxUserAgreement",		LuaFunc_IsDisplayVivoxUserAgreement);
		lua_register(L, "GetVivoxUserAgreementText",		LuaFunc_GetVivoxUserAgreementText);
		lua_register(L, "AcceptVivoxUserAgreement",			LuaFunc_AcceptVivoxUserAgreement);
		lua_register(L, "DeclineVivoxUserAgreement",		LuaFunc_DeclineVivoxUserAgreement);

		lua_register(L, "GetAccountName",			LuaFunc_GetAccountName);

		lua_register(L, "SetSecondPassword",		LuaFunc_SetSecondPassword);

		lua_register(L, "SetCameraLayout",			LuaFunc_SetCameraLayout);
	    lua_register(L, "AddControlCamera",			LuaFunc_AddControlCamera);	
  	    lua_register(L, "GetCameraLayout",			LuaFunc_GetCameraLayout);	


  	   	lua_register(L, "AccountLoginShow",			LuaFunc_AccountLoginShow);

		lua_register(L, "GetCurrentParallelID",		LuaFunc_GetCurrentParallelID);


		lua_register(L, "GetNumServerList",			LuaFunc_GetNumServerList);
		lua_register(L, "GetServerListInfo",		LuaFunc_GetServerListInfo);
		lua_register(L, "SortServerList",			LuaFunc_SortServerList);
		lua_register(L, "ChangeServerList",			LuaFunc_ChangeServerList);
		lua_register(L, "RefreshServerList",		LuaFunc_RefreshServerList);
		lua_register(L, "CloseServerList",			LuaFunc_CloseServerList);
		lua_register(L, "GetCurrentServerList",		LuaFunc_GetCurrentServerList);

		lua_register(L, "GetServerBoardText",		LuaFunc_GetServerBoardText);

		lua_register(L, "LinkActivateWeb",			LuaFunc_LinkActivateWeb);
		lua_register(L, "OpenActivateExecute",		LuaFunc_OpenActivateExecute);
		lua_register(L, "OpenResetPasswordURL",		LuaFunc_OpenResetPasswordURL);

		lua_register(L, "IsBeanFanSystem",			LuaFunc_IsBeanFanSystem);
		lua_register(L, "ConfirmPassword",			LuaFunc_ConfirmPassword);
		lua_register(L, "GetAccountAge",			LuaFunc_GetAccountAge);
		lua_register(L, "GetServerLimitAge",		LuaFunc_GetServerLimitAge);

		lua_register(L, "GetServerName",			LuaFunc_GetServerName);
		lua_register(L, "GetPasswordErrorCount",	LuaFunc_GetPasswordErrorCount);

		lua_register(L, "ConfirmPassword2",			LuaFunc_ConfirmPasswordV2);
		lua_register(L, "ConfirmCaptcha",			LuaFunc_CaptchaReply);
		lua_register(L, "RefreshCaptcha",			LuaFunc_CaptchaRefresh);
	}
}

void CAccountLogin::SetCameraLayout( sCameraLayout& rCameraLayout )
{
	m_CurrentCameraLayout = rCameraLayout;
	m_ControlCameraLayoutList.clear();
	m_fControlCameraTime = 0.0f;
	UpdateCamera( m_CurrentCameraLayout );

	m_SmoothCameraLookAt = m_CameraLookAt;
	m_SmoothCameraEye    = m_CameraEye   ;
	m_SmoothCameraUp     = m_CameraUp    ;

	CRuCamera* pCamera = g_pGameMain->GetCamera();
 
	pCamera->SetTargetPosition( m_SmoothCameraLookAt );
	pCamera->SetCameraPosition( m_SmoothCameraEye    );
	pCamera->SetCameraUpVector( m_SmoothCameraUp     );
}

void CAccountLogin::AddControlCamera( sCameraLayout& rCameraLayout )
{
	m_ControlCameraLayoutList.push_back( rCameraLayout );
}
#define DF_FloatBlend( f1 , f2 , per ) ( ( f2 - f1 ) * per + f1 )
void CAccountLogin::CleanControlCamera()
{

	if( m_ControlCameraLayoutList.size() > 0 )
	{
		std::list<sCameraLayout>::iterator it = m_ControlCameraLayoutList.begin();

		sCameraLayout& rCameraLayout = *it;

		if( m_fControlCameraTime < rCameraLayout.time )
		{
			float per = m_fControlCameraTime / rCameraLayout.time;
			sCameraLayout s;
			s.pos.m_x = DF_FloatBlend( m_CurrentCameraLayout.pos.m_x , rCameraLayout.pos.m_x , per );
			s.pos.m_y = DF_FloatBlend( m_CurrentCameraLayout.pos.m_y , rCameraLayout.pos.m_y , per );
			s.pos.m_z = DF_FloatBlend( m_CurrentCameraLayout.pos.m_z , rCameraLayout.pos.m_z , per );
			s.pitch   = DF_FloatBlend( m_CurrentCameraLayout.pitch   , rCameraLayout.pitch   , per );
			s.yaw     = DF_FloatBlend( m_CurrentCameraLayout.yaw     , rCameraLayout.yaw     , per );
			s.roll    = DF_FloatBlend( m_CurrentCameraLayout.roll    , rCameraLayout.roll    , per );
			s.zoom    = DF_FloatBlend( m_CurrentCameraLayout.zoom    , rCameraLayout.zoom    , per );
			s.pos = m_SmoothCameraLookAt;
			m_CurrentCameraLayout =	s;
		}
	}
 
	m_ControlCameraLayoutList.clear();
	m_fControlCameraTime = 0.0f;

	UpdateCamera( m_CurrentCameraLayout );
 	//m_SmoothCameraLookAt = m_CameraLookAt;
	//m_SmoothCameraEye    = m_CameraEye   ;
	//m_SmoothCameraUp     = m_CameraUp    ;

	//CRuCamera* pCamera = g_pGameMain->GetCamera();

	//pCamera->SetTargetPosition( m_SmoothCameraLookAt );
	//pCamera->SetCameraPosition( m_SmoothCameraEye    );
	//pCamera->SetCameraUpVector( m_SmoothCameraUp     );
}

#define DF_ToPI( p )    ( (p) * 0.017453292519943295769236907684886f )   //( (p) * DF_PI  / 180.0f )
void CAccountLogin::UpdateCamera( sCameraLayout& rCameraLayout )
{

	m_CameraLookAt = rCameraLayout.pos;

	CRuVector3 position( 0.0f, 0.0f, -rCameraLayout.zoom );

	CRuMatrix4x4 matrix;
 	CRuMatrix4x4 matrix2;
  
	float pitch = DF_ToPI( rCameraLayout.pitch );
	float yaw   = DF_ToPI( rCameraLayout.yaw   );
  	float roll  = DF_ToPI( rCameraLayout.roll  );

	matrix.SetToRotationEulerXYZ( pitch, yaw, 0 );
  	matrix2.SetToRotationEulerXYZ( 0, 0, roll );

	matrix = matrix2 * matrix;

	position = position * matrix;
	position += rCameraLayout.pos;
 
	CRuVector3 up( 0.0f , 1.0f , 0.0f );
	up = up * matrix;

    m_CameraEye = position ;
    m_CameraUp = up ;
 
}

void CAccountLogin::GetCameraLayout( sCameraLayout& rCameraLayout )
{
	if( m_ControlCameraLayoutList.size() > 0 )
	{
		rCameraLayout = *m_ControlCameraLayoutList.rbegin();
	}
	else
	{
		rCameraLayout = m_CurrentCameraLayout;
	}
}

// ----------------------------------------------------------------------------
void CAccountLogin::ChangeParallelID(int id)
{
	// 狀態未解除
	if ( m_changeParallelID != -1 )
		return;

	m_changeParallelID = id;
	NetCli_RoleValue::S_ChangeParallel(m_changeParallelID);

}

// ----------------------------------------------------------------------------
void CAccountLogin::Update(float elapsedTime)
{
	if ( m_gameServerPort != 0 && !m_gameServerIP.empty() && m_connect.gameServer == LOGIN_DISCONNECT )
	{
		if ( CNetCli_Login_Child::GetServerLoginRecount() > 0 )
		{			
		}
		else if ( m_delayLoginTime > 0.0f )
		{
			m_delayLoginTime -= elapsedTime;
		}
		else
		{
			// 先設狀態, 因為會在函式內呼叫 ConnectFailed 的 Callback			
			m_connect.gameServer = LOGIN_CONNECTING;			

			const char* accountName = m_serverAccountName.c_str();
			if ( accountName[0] == 0 )
				accountName = m_account.c_str();

			// 刪除所有保留角色資訊
			g_pCharacterCreate->ClearReserveRole();

			string password = GetServerPassword();

			//輸入的密碼本身就是MD5
			if (g_pGameMain->GetIsMD5Password())
			{
				CNetCli_Login::Login(m_gameServerIP.c_str(), m_gameServerPort, (char*)accountName, (char*)password.c_str(), g_pGameMain->GetMac(), true);
			}
			else
			{
				//自動Hash密碼
				if (g_pGameMain->GetIsAutoHashPassword())
				{
					switch (g_pGameMain->GetPasswordHashType())
					{
					case 0: //MD5
						{
							MyMD5Class myMd5Class;
							myMd5Class.ComputeStringHash( password.c_str() , NULL );
							CNetCli_Login::Login(m_gameServerIP.c_str(), m_gameServerPort, (char*)accountName, (char*)myMd5Class.GetMd5Str(), g_pGameMain->GetMac(), true);
						}
						break;
					case 1: //bcrypt
						{
							char* bcPassword = crypt(password.c_str(), BCRYPT_DEFAULT_SALT);

							if (bcPassword != NULL )
							{
								CNetCli_Login::Login(m_gameServerIP.c_str(), m_gameServerPort, (char*)accountName, bcPassword, g_pGameMain->GetMac(), false);
							}
							else
							{
								//log error
							}
						}
						break;
					}
				}
				else //明碼密碼
				{
					CNetCli_Login::Login(m_gameServerIP.c_str(), m_gameServerPort, (char*)accountName, (char*)password.c_str(), g_pGameMain->GetMac(), false);
				}
			}

			m_gameServerIP.clear();
			m_gameServerPort = 0;

			m_delayLoginTime = 2.0f;
		}
   	}

	if( g_pGameMain->IsEnterWorld() )
	{
		return;
	}

	if( elapsedTime * 24.0f > 1.0f )
	{
		elapsedTime	= 1.0f / 24.0f;
	}

	if( m_ControlCameraLayoutList.size() > 0 )
	{
		std::list<sCameraLayout>::iterator it = m_ControlCameraLayoutList.begin();

		sCameraLayout& rCameraLayout = *it;
   
		m_fControlCameraTime += elapsedTime;

		if( m_fControlCameraTime < rCameraLayout.time )
		{
			float per = m_fControlCameraTime / rCameraLayout.time;
			sCameraLayout s;
			s.pos.m_x = DF_FloatBlend( m_CurrentCameraLayout.pos.m_x , rCameraLayout.pos.m_x , per );
			s.pos.m_y = DF_FloatBlend( m_CurrentCameraLayout.pos.m_y , rCameraLayout.pos.m_y , per );
			s.pos.m_z = DF_FloatBlend( m_CurrentCameraLayout.pos.m_z , rCameraLayout.pos.m_z , per );
			s.pitch   = DF_FloatBlend( m_CurrentCameraLayout.pitch   , rCameraLayout.pitch   , per );
			s.yaw     = DF_FloatBlend( m_CurrentCameraLayout.yaw     , rCameraLayout.yaw     , per );
   			s.roll    = DF_FloatBlend( m_CurrentCameraLayout.roll    , rCameraLayout.roll    , per );
			s.zoom    = DF_FloatBlend( m_CurrentCameraLayout.zoom    , rCameraLayout.zoom    , per );
			//s.time    = DF_FloatBlend( g_CurrentCameraLayout.time    , rCameraLayout.time    , per );
			UpdateCamera( s );
		}
		else
		{
			m_CurrentCameraLayout = rCameraLayout;
			m_ControlCameraLayoutList.pop_front();
			m_fControlCameraTime = 0;
			UpdateCamera( m_CurrentCameraLayout );
		}
	}

	//if( true )
	{
//#define XD_SmoothMovePar1   ( 0.7f ) //攝影機平滑移動參數 頁面更新率 FPS       數值越大 速度越快
//#define XD_SmoothMovePar2   ( 5.0f ) //攝影機平滑移動參數 每頁面要改變的百分比 數值越大 速度越快 0.0 ~ 1.0f

		FLOAT fPercentage = 1.0f - powf( 0.7f , 5.0f * elapsedTime );

	 	m_SmoothCameraLookAt += (m_CameraLookAt - m_SmoothCameraLookAt) * fPercentage;
		m_SmoothCameraEye    += (m_CameraEye    - m_SmoothCameraEye   ) * fPercentage;
		m_SmoothCameraUp     += (m_CameraUp     - m_SmoothCameraUp    ) * fPercentage;

		CRuCamera* pCamera = g_pGameMain->GetCamera();

		pCamera->SetTargetPosition( m_SmoothCameraLookAt );
		pCamera->SetCameraPosition( m_SmoothCameraEye    );
		pCamera->SetCameraUpVector( m_SmoothCameraUp     );

		g_pGameMain->SetPlayerAnchor( m_SmoothCameraEye );
	}


	//for board string
	CheckBoardStringDownloadStatus();
}



// ----------------------------------------------------------------------------
void CAccountLogin::Login(const char* name, const char* password)
{
	char accountName[_DEF_MAX_ACCOUNTNAME_SIZE_];
	char accountPassword[_DEF_MAX_PASSWORD_SIZE_];
	lua_State* L = GetLuaState();

	memset(accountName, 0, sizeof(accountName));
	memset(accountPassword, 0, sizeof(accountPassword));
	int len = (int)strlen(name);
	for ( int i = 0; i < len; i++ ) {
		accountName[i] = name[i];
		//if ( i == 63 || accountName[i] == ' ' ) {
		if ( i == (_DEF_MAX_ACCOUNTNAME_SIZE_ - 1) || accountName[i] == ' ' ) {
			accountName[i] = 0;
			break;
		}
	}
	len = (int)strlen(password);
	for ( int i = 0; i < len; i++ ) {
		accountPassword[i] = password[i];
		//if ( i == 63 || accountPassword[i] == ' ' ) {
		if ( i == (_DEF_MAX_PASSWORD_SIZE_ - 1) || accountPassword[i] == ' ' ) {
			accountPassword[i] = 0;
			break;
		}
	}	

	const char* gameServerIP = g_pGameMain->GetGameServerIP();
	int port = g_pGameMain->GetGameServerPort();
	if ( gameServerIP && gameServerIP[0] != 0 && port != 0 )
	{
		m_account = accountName;
		SetServerPassword(accountPassword);

		// 刪除所有保留角色資訊
		g_pCharacterCreate->ClearReserveRole();

		//輸入的密碼本身就是MD5
		if (g_pGameMain->GetIsMD5Password())
		{
			CNetCli_Login::Login(gameServerIP, port, accountName, accountPassword, g_pGameMain->GetMac(), true);
		}
		else
		{
			//自動Hash密碼
			if (g_pGameMain->GetIsAutoHashPassword())
			{
				switch (g_pGameMain->GetPasswordHashType())
				{
				case 0: //MD5
					{
						MyMD5Class myMd5Class;
						myMd5Class.ComputeStringHash( accountPassword , NULL );
						CNetCli_Login::Login(gameServerIP, port, accountName, (char*)myMd5Class.GetMd5Str(), g_pGameMain->GetMac(), true);
					}
					break;
				case 1: //bcrypt
					{
						char* bcPassword = crypt(accountPassword, BCRYPT_DEFAULT_SALT);

						if (bcPassword != NULL )
						{
							CNetCli_Login::Login(gameServerIP, port, accountName, bcPassword, g_pGameMain->GetMac(), false);
						}
						else
						{
							//log error
						}
					}
					break;
				}
			}
			else //明碼密碼
			{
				CNetCli_Login::Login(gameServerIP, port, accountName, accountPassword, g_pGameMain->GetMac(), false);
			}
		}

		return;
	}

	if ( m_connect.realmList == LOGIN_DISCONNECT && m_connect.gameServer == LOGIN_DISCONNECT )
	{
		if ( L )
		{
			lua_pushstring(L, "CANCEL");
			lua_setglobal(L, "arg1");
			lua_pushstring(L, g_ObjectData->GetString("SYS_LOGIN_GET_SERVER_LIST"));
			lua_setglobal(L, "arg2");
			SendLoginEvent("OPEN_LOGIN_DIALOG");
		}

#ifdef KALYDO
		// TEMP: no check server version.
		if (true)
#else
		if (CheckServerVersion())
#endif // KALYDO
		{
			// 如果帳號不相同輸入一次兩次密碼
			if ( stricmp(m_serverListAccount.c_str(), accountName) != 0 )
			{
				m_secondPassword.clear();
			}
			
			SetServerPassword(accountPassword);
			m_serverListAccount = accountName;
			m_vivoxLincenseTime = 0;

			//輸入的密碼本身就是MD5
			if (g_pGameMain->GetIsMD5Password())
			{
				CNetCli_Login_Child::ServerListConnect(g_pGameMain->GetIP(), g_pGameMain->GetPort(), m_serverListAccount.c_str(), accountPassword, true);
			}
			else
			{
				//自動Hash密碼
				if (g_pGameMain->GetIsAutoHashPassword())
				{
					switch (g_pGameMain->GetPasswordHashType())
					{
					case 0: //MD5
						{
							MyMD5Class myMd5Class;
							myMd5Class.ComputeStringHash( accountPassword , NULL );
							CNetCli_Login_Child::ServerListConnect(g_pGameMain->GetIP(), g_pGameMain->GetPort(), m_serverListAccount.c_str(), myMd5Class.GetMd5Str(), true);
						}
						break;
					case 1: //bcrypt
						{
							char* bcPassword = crypt(accountPassword, BCRYPT_DEFAULT_SALT);

							if (bcPassword != NULL )
							{
								CNetCli_Login_Child::ServerListConnect(g_pGameMain->GetIP(), g_pGameMain->GetPort(), m_serverListAccount.c_str(), bcPassword, false);
							}
							else
							{
								//log error
							}
						}
						break;
					}
				}
				else //明碼密碼
				{
					CNetCli_Login_Child::ServerListConnect(g_pGameMain->GetIP(), g_pGameMain->GetPort(), m_serverListAccount.c_str(), accountPassword, false);
				}
			}

			m_connect.realmList = LOGIN_CONNECTING;
			m_account = accountName;
		}
		else
		{
			lua_pushstring(L, "OKAY");
			lua_setglobal(L, "arg1");
			lua_pushstring(L, g_ObjectData->GetString("SYS_LOGIN_VERSION_INCORRECT"));
			lua_setglobal(L, "arg2");
			SendLoginEvent("OPEN_LOGIN_DIALOG");
		}

	} else {
		if ( L )
		{
			lua_pushstring(L, "CANCEL");
			lua_setglobal(L, "arg1");
			lua_pushstring(L, g_ObjectData->GetString("SYS_LOGIN_WAIT_DISCONNECT"));
			lua_setglobal(L, "arg2");
			SendLoginEvent("OPEN_LOGIN_DIALOG");
		}
		Logout(EM_ClientNetCloseType_ReturnAccount, true);
	}
}

// ----------------------------------------------------------------------------
void CAccountLogin::ReLogin()
{
	Login(m_serverListAccount.c_str(), GetServerPassword().c_str());
}

// ----------------------------------------------------------------------------
// 要求斷線
// forceLogout [true] Client 強制要求,不等Server斷線回應
void CAccountLogin::Logout(int closeType, bool forceLogout)
{
	CNetCli_Login_Child::Logout((ClientNetCloseTypeENUM)closeType);	
	
	// 本機強制斷線,忽略下一次的斷線回應封包
	m_ignoreLogout = forceLogout;
	if ( forceLogout )
	{
		// 但是如果未連線不用忽略
		if ( m_connect.gameServer != LOGIN_CONNECTED )
			m_ignoreLogout = false;

		CNetCli_Login::m_lockEnterWorld = false;
		m_connect.realmList = LOGIN_DISCONNECT;
		m_connect.gameServer = LOGIN_DISCONNECT;
		m_gameServerIP.clear();
		m_gameServerPort = 0;
		m_serverListData = false;
		m_lastReciveServerListTime = 0.0f;		

		// CNetCli_Login_Child::CloseServerList();

		// 清除預設選定伺服器名稱(會需要重新選擇伺服器)
		m_realmName.clear();
	}

	//清除Server設定的帳號名稱
	m_serverAccountName = "";
}

// ----------------------------------------------------------------------------
// 收到Server要求斷線回應
void CAccountLogin::ReceiveLogout()
{
	if ( m_ignoreLogout ) {
		m_ignoreLogout = false;
		return;
	} else {		
	}

	m_connect.realmList = LOGIN_DISCONNECT;
	m_connect.gameServer = LOGIN_DISCONNECT;		

	CNetCli_Login::m_lockEnterWorld = false;

	// 若沒有任何連線動作,跳回輸入帳號頁面
	if ( m_gameServerIP.empty() || m_gameServerPort == 0 )
	{
		lua_State* L = GetLuaStateLogin();
		if ( L )
		{
			lua_pushstring(L, GLUE_SCREEN_LOGIN);
			lua_setglobal(L, "arg1");
			SendLoginEvent("SET_LOGIN_SCREEN");
		}

		// 清除最後登入伺服器名稱
		m_realmName.clear();
	}
}

// ----------------------------------------------------------------------------
void CAccountLogin::LoginResult(int resultType)
{
	lua_State*	L		= GetLuaStateLogin();
	const char* text	= NULL;
	const char* text2	= NULL;
	int	loginResultFlag	= 0;

	char Buff[2048];
	
	switch (resultType)
	{
	case EM_NetLogFailed_OK:
		m_connect.gameServer = LOGIN_CONNECTED;

		SendLoginEvent("CLOSE_LOGIN_DIALOG");

		if ( L )
		{
			lua_pushstring(L, GLUE_SCREEN_CHAR_SELECT);
			lua_setglobal(L, "arg1");
			SendLoginEvent("SET_LOGIN_SCREEN");
		}
		return;

	case EM_NetLogFailed_PasswordError:		//密碼錯誤

		text = g_ObjectData->GetString("SYS_LOGIN_PASSWORLD_FAIL");

		if (m_PasswordErrorCount > 0 && m_MaxPasswordErrorCount > 0)
		{
			text2 = g_ObjectData->GetString("SYS_LOGIN_PASSWORLD_FAIL_WITH_ERROR_COUNT");

			if (strlen(text2) > 0)
			{
				sprintf_s(Buff, 2048, text2, m_PasswordErrorCount, m_MaxPasswordErrorCount, m_FreezeTime / 60);
				text = Buff;
			}		
		}
		
		loginResultFlag = 5;
		break;

	case EM_NetLogFailed_ConnectFail:		//連線不成
	case EM_NetLogFailed_ConnectError:		//連結位置錯誤
		text = g_ObjectData->GetString("SYS_LOGIN_CONNECT_FAIL");
		loginResultFlag	= 5;
		break;

	case EM_NetLogFailed_NotFindError:		//找不到帳號
		text = g_ObjectData->GetString("SYS_LOGIN_ACCOUNT_FAIL");
		loginResultFlag	= 5;
		break;

	case EM_NetLogFailed_ProxyNotExist:		//Proxy不存在
		text = g_ObjectData->GetString("SYS_LOGIN_PROXY_FAIL");
		loginResultFlag	= 5;
		break;

	case EM_NetLogFailed_SwitchDisconnect:	//未完成註冊及與 Switch 斷線
		text = g_ObjectData->GetString("SYS_LOGIN_SWITCH_DISCONNECT");
		loginResultFlag	= 4;
		break;

	case EM_NetLogFailed_ProxyLoginFailed:	//未完成註冊及與 Proxy 連線失敗
		text = g_ObjectData->GetString("SYS_LOGIN_PROXY_LOGIN_FAILED");
		loginResultFlag	= 4;
		break;

	case EM_NetLogFailed_AccountExist:		//帳號重覆登入
		text = g_ObjectData->GetString("SYS_LOGIN_ACCOUNT_EXIST");
		loginResultFlag	= 5;
		break;

	case EM_NetLogFailed_LoginTwice:		//登入兩次
		text = g_ObjectData->GetString("SYS_LOGFAILED_LOGINTWICE");		
		break;

	case EM_NetLogFailed_LoginByOtherSrv:	//角色登入其他世界
		text = g_ObjectData->GetString("SYS_LOGFAILED_LOGINBYOTHERSRV");		
		break;

	case EM_NetLogFailed_SrvDisconnect:		//暫時沒用
		text = g_ObjectData->GetString("NetLogFailed_SrvDisconnect");
		break;

	case EM_NetLogFailed_NotEnoughPoint:	//暫時沒用
		text = g_ObjectData->GetString("NetLogFailed_NotEnoughPoint");
		break;

	case EM_NetLogFailed_NotCorrectChargType:		//暫時沒用
		text = g_ObjectData->GetString("NetLogFailed_NotCorrectChargType");
		break;
	
	case EM_NetLogFailed_Suspended:		//帳號警告
		text = g_ObjectData->GetString("SYS_LOGFAILED_SUSPENDED");
		break;
	case EM_NetLogFailed_Banned:		//帳號被踢
	case EM_NetLogFailed_llegalMacAddress:
		text = g_ObjectData->GetString("SYS_LOGFAILED_BANNED");
		break;
	case EM_NetLogFailed_CommLock:
		text = g_ObjectData->GetString("SYS_LOGFAILED_COMMLOCK");
		break;
	case EM_NetLogFailed_AccountStrError:		//帳號自串輸入有問題
		text = g_ObjectData->GetString("SYS_LOGFAILED_ACCOUNTSTRERROR");
		break;
	case EM_NetLogFailed_SwitchError:		//Switch有問題
		text = g_ObjectData->GetString("SYS_LOGFAILED_SWITCHERROR");
		break;
	case EM_NetLogFailed_FreezeType_Self:		//自我冷
		{
			text = g_ObjectData->GetString("SYS_LOGFAILED_FREEZE_SELF");
			text2 = g_ObjectData->GetString("SYS_LOGFAILED_FREEZE_SELF_WITH_TIME");
		}		
		break;
	case EM_NetLogFailed_FreezeType_Official:		//官方冷凍
		{
			text = g_ObjectData->GetString("SYS_LOGFAILED_FREEZE_OFFICIAL");
			text2 = g_ObjectData->GetString("SYS_LOGFAILED_FREEZE_OFFICIAL_WITH_TIME");
		}		
		break;
	case EM_NetLogFailed_FreezeType1_Other:		//其它冷凍
		{
			text = g_ObjectData->GetString("SYS_LOGFAILED_FREEZE_OTHER");
			text2 = g_ObjectData->GetString("SYS_LOGFAILED_FREEZE_OTHER_WITH_TIME");
		}		
		break;
	case EM_NetLogFailed_UnknowError:		//暫時未規類的錯誤
	default:
		text = g_ObjectData->GetString("SYS_LOGIN_UNKNOW_ERROR");
		loginResultFlag	= 3;
		break;
	}

	if ((resultType == EM_NetLogFailed_FreezeType_Self) ||
		(resultType == EM_NetLogFailed_FreezeType_Official) ||
		(resultType == EM_NetLogFailed_FreezeType1_Other))
	{
		if (m_AccountFreezeRemainingTime > 0)
		{
			if (strlen(text2) > 0)
			{
				sprintf_s(Buff, 2048, text2, (int)ceil((float)m_AccountFreezeRemainingTime / 60.0f));

				text = Buff;
			}			
		}
	}

	m_realmName.clear();
	m_connect.gameServer = LOGIN_DISCONNECT;

	if ( L )
	{
		lua_pushstring(L, "OKAY");
		lua_setglobal(L, "arg1");
		lua_pushstring(L, text);
		lua_setglobal(L, "arg2");
		SendLoginEvent("OPEN_LOGIN_DIALOG");
	}
}

// ----------------------------------------------------------------------------
void CAccountLogin::LoginFailedData(int resultType, void* data, unsigned long size)
{
	LoginFailedDataUnion* dataUnion = (LoginFailedDataUnion*)data;

	switch (resultType)
	{
	case EM_NetLogFailed_OK:
		return;
	case EM_NetLogFailed_PasswordError:		//密碼錯誤
		{
			m_PasswordErrorCount = dataUnion->PasswordError.Count;
			m_MaxPasswordErrorCount = dataUnion->PasswordError.Max;
			m_FreezeTime =  dataUnion->PasswordError.FreezeTime;
		}
		break;
	case EM_NetLogFailed_ConnectFail:		//連線不成
	case EM_NetLogFailed_ConnectError:		//連結位置錯誤
		break;
	case EM_NetLogFailed_NotFindError:		//找不到帳號
		break;
	case EM_NetLogFailed_ProxyNotExist:		//Proxy不存在
		break;
	case EM_NetLogFailed_SwitchDisconnect:	//未完成註冊及與 Switch 斷線
		break;
	case EM_NetLogFailed_ProxyLoginFailed:	//未完成註冊及與 Proxy 連線失敗
		break;
	case EM_NetLogFailed_AccountExist:		//帳號重覆登入
		break;
	case EM_NetLogFailed_LoginTwice:		//登入兩次
		break;
	case EM_NetLogFailed_LoginByOtherSrv:	//角色登入其他世界
		break;
	case EM_NetLogFailed_SrvDisconnect:		//暫時沒用
		break;
	case EM_NetLogFailed_NotEnoughPoint:	//暫時沒用
		break;
	case EM_NetLogFailed_NotCorrectChargType:		//暫時沒用
		break;
	case EM_NetLogFailed_Suspended:		//帳號警告
		break;
	case EM_NetLogFailed_Banned:		//帳號被踢
	case EM_NetLogFailed_llegalMacAddress:
		break;
	case EM_NetLogFailed_AccountStrError:		//帳號自串輸入有問題
		break;
	case EM_NetLogFailed_SwitchError:		//Switch有問題
		break;
	case EM_NetLogFailed_FreezeType_Self:		//自我冷凍
	case EM_NetLogFailed_FreezeType_Official:		//官方冷凍
	case EM_NetLogFailed_FreezeType1_Other:		//其它冷凍
		{
			m_AccountFreezeRemainingTime = dataUnion->Freeze.RemainingTime;
		}
		break;
	case EM_NetLogFailed_UnknowError:		//暫時未規類的錯誤
	default:
		break;
	}

}

// ----------------------------------------------------------------------------
void CAccountLogin::OnConnectRealm()
{
	m_connect.realmList = LOGIN_CONNECTED;
}

// ----------------------------------------------------------------------------
void CAccountLogin::OnDisconnectRealm()
{
	m_connect.realmList = LOGIN_DISCONNECT;
	//LoginResult(EM_NetLogFailed_ConnectFail);
}

// ----------------------------------------------------------------------------
void CAccountLogin::OnConnectRealmFailed(int resultType)
{
	m_connect.realmList = LOGIN_DISCONNECT;

	char Buff[2048];

	lua_State* L = GetLuaStateLogin();
	if ( L )
	{
		const char* text = NULL;
		const char* text2 = NULL;

		switch (resultType)
		{
		case EM_ServerListFailed_RoleInWorld:			//角色還在遊戲中
			text = g_ObjectData->GetString("SYS_SERVERLIST_FAIL_PLAYER_EXIST");
			break;
		case EM_ServerListFailed_Password_Error:		//帳號或密碼是錯的
			if( g_pGameMain->CheckCountry("JP") )	//if ( strcmp(g_pGameMain->CheckCountry("JP" ) == 0 || strcmp(g_pGameMain->GetCountry(), "JP") == 0 )
			{
				SendLoginEvent("LOGIN_PASSWORD_FAILED");
				return;
			}

			text = g_ObjectData->GetString("SYS_SERVERLIST_FAIL_ACCOUNT");

			if (m_PasswordErrorCount > 0 && m_MaxPasswordErrorCount > 0)
			{
				text2 = g_ObjectData->GetString("SYS_LOGIN_PASSWORLD_FAIL_WITH_ERROR_COUNT");

				if (strlen(text2) > 0)
				{
					sprintf_s(Buff, 2048, text2, m_PasswordErrorCount, m_MaxPasswordErrorCount, m_FreezeTime / 60);
					text = Buff;
				}
			}		
			break;
		case EM_ServerListFailed_ValidTimeError:
			if( g_pGameMain->CheckCountry("JP") )
			{
				SendLoginEvent("LOGIN_PASSWORD_FAILED");
				return;
			} else if (strlen(g_pGameMain->GetActivateURL()) > 0) {
				g_pGameMain->SetActivateURLArg("3");
				SendLoginEvent("VALID_TIME_ERROR");
				return;
			}
			text = g_ObjectData->GetString("SYS_SERVERLIST_INVALIDTIME");			
			break;
		case EM_ServerListFailed_IPBlock:
			text = g_ObjectData->GetString("SYS_SERVERLIST_FAIL_IPBLOCK");
			break;
		case EM_ServerListFailed_CommLock:
			{
				if (m_connect.gameServer == LOGIN_DISCONNECT)
				{
					text = g_ObjectData->GetString("SYS_SERVERLIST_FAIL_COMMLOCK");
				}
				else
				{
					return;
				}
			}
			break;
		case EM_ServerListFailed_NoWorld:				//沒有一個世界是開的
			text = g_ObjectData->GetString("SYS_SERVERLIST_FAIL_NOWORLD");
			break;
		case EM_ServerListFailed_Freeze_Self:			//自己帳號冷凍
			{
				text = g_ObjectData->GetString("SYS_SERVERLIST_FAIL_FREEZE_SELF");
				text2 = g_ObjectData->GetString("SYS_SERVERLIST_FAIL_FREEZE_SELF_WITH_TIME");
			}
			break;
		case EM_ServerListFailed_Freeze_Official:		//官方帳號冷凍
			{
				text = g_ObjectData->GetString("SYS_SERVERLIST_FAIL_FREEZE_OFFIC");
				text2 = g_ObjectData->GetString("SYS_SERVERLIST_FAIL_FREEZE_OFFIC_WITH_TIME");
			}
			break;
		case EM_ServerListFailed_Freeze_Other3:			//帳號冷凍
			{
				text = g_ObjectData->GetString("SYS_SERVERLIST_FAIL_FREEZE_OTHER3");
				text2 = g_ObjectData->GetString("SYS_SERVERLIST_FAIL_FREEZE_OTHER3_WITH_TIME");
			}
			break;
		case EM_ServerListFailed_Freeze_Other4:			//帳號冷凍
			{
				text = g_ObjectData->GetString("SYS_SERVERLIST_FAIL_FREEZE_OTHER4");
				text2 = g_ObjectData->GetString("SYS_SERVERLIST_FAIL_FREEZE_OTHER4_WITH_TIME");
			}
			break;
		case EM_ServerListFailed_Freeze_Other5:			//帳號冷凍
			{
				text = g_ObjectData->GetString("SYS_SERVERLIST_FAIL_FREEZE_OTHER5");
				text2 = g_ObjectData->GetString("SYS_SERVERLIST_FAIL_FREEZE_OTHER5_WITH_TIME");
			}
			break;
		case EM_ServerListFailed_Freeze_Other6:			//帳號冷凍
			{
				text = g_ObjectData->GetString("SYS_SERVERLIST_FAIL_FREEZE_OTHER6");
				text2 = g_ObjectData->GetString("SYS_SERVERLIST_FAIL_FREEZE_OTHER6_WITH_TIME");
			}
			break;
		case EM_ServerListFailed_Freeze_Other7:			//帳號冷凍
			{
				text = g_ObjectData->GetString("SYS_SERVERLIST_FAIL_FREEZE_OTHER7");
				text2 = g_ObjectData->GetString("SYS_SERVERLIST_FAIL_FREEZE_OTHER7_WITH_TIME");
			}
			break;
		case EM_ServerListFailed_Freeze_Other8:			//帳號冷凍
			{
				text = g_ObjectData->GetString("SYS_SERVERLIST_FAIL_FREEZE_OTHER8");
				text2 = g_ObjectData->GetString("SYS_SERVERLIST_FAIL_FREEZE_OTHER8_WITH_TIME");
			}
			break;
		case EM_ServerListFailed_Freeze_Other9:			//帳號冷凍
			{
				text = g_ObjectData->GetString("SYS_SERVERLIST_FAIL_FREEZE_OTHER9");
				text2 = g_ObjectData->GetString("SYS_SERVERLIST_FAIL_FREEZE_OTHER9_WITH_TIME");
			}
			break;
		case EM_ServerListFailed_BetaKeyRequest:
			{
				SendLoginEvent("LINK_ACTIVATE_WEB");
				//text = g_ObjectData->GetString("SYS_SERVERLIST_FAIL_BETAKEY");
				return;
			}
			break;
		case EM_ServerListFailed_Gama_Wrong_Timeout:
			text = g_ObjectData->GetString("SYS_SERVERLIST_FAIL_GAMA_TIMEOUT");
			break;
		case EM_ServerListFailed_Gama_Wrong_OTP:		//橘子錯誤 Wrong_OTP : One time password is not correct
			text = g_ObjectData->GetString("SYS_SERVERLIST_FAIL_WRONG_OTP");
			break;
		case EM_ServerListFailed_Gama_NoAccount:		//帳號不存在
			text = g_ObjectData->GetString("SYS_SERVERLIST_FAIL_NO_ACCOUNT");
			break;
		case EM_ServerListFailed_Gama_Wrong_EventOTP:
			text = g_ObjectData->GetString("SYS_SERVERLIST_FAIL_WRONG_EVENTOTP");
			break;		
		case EM_ServerListFailed_VN_Wrong_UserState:
			text = g_ObjectData->GetString("SYS_SERVERLIST_FAIL_VN_WRONG_USERSTATE");
			break;
		case EM_ServerListFailed_KR_AgeRestriction :
			text = g_ObjectData->GetString("SYS_SERVERLIST_FAIL_KR_AGE_RESTRICTION");
			break;
		case EM_ServerListFailed_ConnectFailed:
			text = g_ObjectData->GetString("SYS_SERVERLIST_FAIL_CONNECTFAILED");
			break;
		default:
			text = g_ObjectData->GetString("SYS_SERVERLIST_FAIL_UNKNOW");
			break;
		}

		if ((resultType >= EM_ServerListFailed_Freeze_Self) && (resultType <= EM_ServerListFailed_Freeze_Other9))
		{
			if (m_AccountFreezeRemainingTime > 0)
			{
				if (strlen(text2) > 0)
				{
					sprintf_s(Buff, 2048, text2, (int)ceil((float)m_AccountFreezeRemainingTime / 60.0f));
					text = Buff;
				}
			}
		}

		lua_pushstring(L, "OKAY");
		lua_setglobal(L, "arg1");
		lua_pushstring(L, text);
		lua_setglobal(L, "arg2");
		SendLoginEvent("OPEN_LOGIN_DIALOG");
	}	
}

// ----------------------------------------------------------------------------
void CAccountLogin::OnConnectRealmFailedData(int resultType, void* data, unsigned long size)
{
	ServerListFailedDataUnion* pDataUnion = (ServerListFailedDataUnion*)data;

	switch (resultType)
	{
	case EM_ServerListFailed_RoleInWorld:			//角色還在遊戲中
		break;
	case EM_ServerListFailed_Password_Error:		//帳號或密碼是錯的
		{
			m_PasswordErrorCount	= pDataUnion->PasswordError.Count;
			m_MaxPasswordErrorCount = pDataUnion->PasswordError.Max;
			m_FreezeTime			= pDataUnion->PasswordError.FreezeTime;
		}
		break;
	case EM_ServerListFailed_ValidTimeError:
		break;
	case EM_ServerListFailed_IPBlock:
		break;
	case EM_ServerListFailed_NoWorld:				//沒有一個世界是開的
		break;
	case EM_ServerListFailed_Freeze_Self:			//自己帳號冷凍
	case EM_ServerListFailed_Freeze_Official:		//官方帳號冷凍
	case EM_ServerListFailed_Freeze_Other3:			//帳號冷凍
	case EM_ServerListFailed_Freeze_Other4:			//帳號冷凍
	case EM_ServerListFailed_Freeze_Other5:			//帳號冷凍
	case EM_ServerListFailed_Freeze_Other6:			//帳號冷凍
	case EM_ServerListFailed_Freeze_Other7:			//帳號冷凍
	case EM_ServerListFailed_Freeze_Other8:			//帳號冷凍
	case EM_ServerListFailed_Freeze_Other9:			//帳號冷凍
		{
			m_AccountFreezeRemainingTime = pDataUnion->Freeze.RemainingTime;
		}
		break;
	case EM_ServerListFailed_BetaKeyRequest:	
		break;
	case EM_ServerListFailed_Gama_Wrong_Timeout:
		break;
	case EM_ServerListFailed_Gama_Wrong_OTP:		//橘子錯誤 Wrong_OTP : One time password is not correct
		break;
	case EM_ServerListFailed_Gama_NoAccount:		//帳號不存在
		break;
	case EM_ServerListFailed_Gama_Wrong_EventOTP:
		break;		
	case EM_ServerListFailed_VN_Wrong_UserState:
		break;
	case EM_ServerListFailed_KR_AgeRestriction :
		break;
	case EM_ServerListFailed_ConnectFailed:
		break;
	default:
		break;
	}

}

// ----------------------------------------------------------------------------
void CAccountLogin::UpdateRealmList(float delayTime)
{
	m_delayLoginTime = delayTime;	

	vector<ServerListInfoStruct>& realmList = CNetCli_Login_Child::s_serverListInfo;

	for ( vector<ServerListInfoStruct>::iterator iter = realmList.begin(); iter != realmList.end(); iter++ )
	{
		ServerListInfoStruct& realmInfo = *iter;
		if ( !realmInfo.Name.empty() && realmInfo.Name.compare(m_realmName) == 0 )
		{
			// 該分流為可以進入狀態

			// 分流各數為零
			if (realmInfo.Loading.empty())
				m_selectedRegion = 0;
			else
			{
				// 比對找出最小的分流壓力編號
				vector<float>::iterator iter = realmInfo.Loading.begin();
				vector <int> sameLoad;
				float minLoad = *iter;
				int index = 0;				
				
				m_selectedRegion = 0;
				sameLoad.push_back(index);
				while ( iter != realmInfo.Loading.end() )
				{
					if (*iter < minLoad)
					{
						minLoad = *iter;
						sameLoad.clear();
						sameLoad.push_back(index);
					}

					index++;
					iter++;
				}

				// 相同壓力是否有複數個
				if ( sameLoad.size() > 1 )
				{
					m_selectedRegion = sameLoad[rand() % sameLoad.size()];
				}
				else
				{
					m_selectedRegion = sameLoad[0];
				}
			}

			m_gameServerIP = realmInfo.IP;
			m_gameServerPort = realmInfo.Port;
			m_worldID = realmInfo.WorldID;

#ifdef KALYDO
			// game server is selected, send statistic but only once
			static bool first = true;
			if(first)
			{
				TAppStatisticsPair statPair[1];
				memset(statPair, 0, sizeof(statPair));
				strcpy(statPair[0].name, "ServerSelected");
				strcpy(statPair[0].value, "1");
				application->sendAppStatistics(statPair, 1, NULL);
				first = false;
			}
#endif // KALYDO

			lua_State* L = GetLuaState();
			if ( L )
			{
				lua_pushstring(L, "WAITING_QUEUE");
				lua_setglobal(L, "arg1");
				lua_pushstring(L, g_ObjectData->GetString("SYS_CONNECT_GAMESERVER"));
				lua_setglobal(L, "arg2");
				SendLoginEvent("OPEN_LOGIN_DIALOG");
			}

			if ( m_connect.gameServer != LOGIN_DISCONNECT )
			{
				// 更換連線伺服器,需要與原伺服器斷線在重新建立連線
				//Logout(EM_ClientNetCloseType_ChangeWorld, true);
				CNetCli_Login_Child::Logout(EM_ClientNetCloseType_ChangeWorld);

				// 上面函式會中斷與Server List連線需要重新建立連線				
				if (g_pGameMain->GetIsMD5Password())
				{
					//輸入的密碼本身就是MD5
					CNetCli_Login_Child::ServerListConnect(g_pGameMain->GetIP(), g_pGameMain->GetPort(), m_serverListAccount.c_str(), GetServerPassword().c_str(), true);
				}
				else
				{					
					//自動Hash密碼
					if (g_pGameMain->GetIsAutoHashPassword())
					{
						switch (g_pGameMain->GetPasswordHashType())
						{
						case 0: //MD5
							{
								MyMD5Class myMd5Class;
								myMd5Class.ComputeStringHash( GetServerPassword().c_str() , NULL );
								CNetCli_Login_Child::ServerListConnect(g_pGameMain->GetIP(), g_pGameMain->GetPort(), m_serverListAccount.c_str(), myMd5Class.GetMd5Str(), true);
							}
							break;
						case 1: //bcrypt
							{
								char* bcPassword = crypt(GetServerPassword().c_str(), BCRYPT_DEFAULT_SALT);

								if (bcPassword != NULL )
								{
									CNetCli_Login_Child::ServerListConnect(g_pGameMain->GetIP(), g_pGameMain->GetPort(), m_serverListAccount.c_str(), bcPassword, false);
								}
								else
								{
									//log error
								}
							}
							break;
						}
					}
					else
					{
						//明碼密碼
						CNetCli_Login_Child::ServerListConnect(g_pGameMain->GetIP(), g_pGameMain->GetPort(), m_serverListAccount.c_str(), GetServerPassword().c_str(), false);
					}
				}
				
				g_pPartyBoardFrame->ClearList();
			}						
			
			return;
		}
	}	

	// m_delayLoginTime = delayTime;

	// 關閉詢息視窗
	// SendLoginEvent("CLOSE_LOGIN_DIALOG");

	/*
	m_realmName.clear();	

	SendLoginEvent("OPEN_REALM_LIST");
	*/
}

// ----------------------------------------------------------------------------
void CAccountLogin::SetRealmName(const char* realmName, bool update)
{
	if ( realmName == NULL )
		return;

	// 檢查是否可以登入
	for ( vector<ServerListInfo>::iterator iter = m_serverList.begin(); iter != m_serverList.end(); iter++ )
	{
		if ( iter->serverName.compare(realmName) == 0 )
		{
			// 未達合法年齡
			if ( m_age > 0 && m_age < iter->age )
			{
				/*
				lua_State* L = GetLuaStateLogin();
				if ( L )
				{
					lua_pushstring(L, "OKAY");
					lua_setglobal(L, "arg1");
					lua_pushstring(L, g_ObjectData->GetString("SYS_SERVER_LIST_AGE_ERROR"));
					lua_setglobal(L, "arg2");
					SendLoginEvent("OPEN_LOGIN_DIALOG");
				}
				*/
				return;
			}

			// 該伺服器無法登入
			if ( iter->isLogin == false )
			{
				return;
			}

			// 人數已滿無法登入
			if ( iter->isLimitNewAccount && iter->characters == 0 )
			{
				return;
			}

			m_serverAge = iter->age;
		}		
	}	

	if ( m_realmName.compare(realmName) != 0 )
	{
		m_realmName = realmName;
		if ( update )
		{
			UpdateRealmList();
		}
	}
}

// ----------------------------------------------------------------------------
void CAccountLogin::SetAccountName(const char* name)
{
	m_account = name;
}

// ----------------------------------------------------------------------------
const char* CAccountLogin::GetAccountName()
{
	return m_account.c_str();
}

// ----------------------------------------------------------------------------
void CAccountLogin::SetServerPassword(const char* password)
{
	m_passwordData.SetString(password);
}

// ----------------------------------------------------------------------------
string CAccountLogin::GetServerPassword()
{
	return m_passwordData.GetString();
}

// ----------------------------------------------------------------------------
// 觸發設定二次密碼界面
void CAccountLogin::SendSecondPassword()
{
	SendLoginEvent("PASSWORD_SHOW");
}

// ----------------------------------------------------------------------------
int CAccountLogin::SetSecondPassword(const char* password1, const char* password2)
{
	int len = 0;
	while ( password1[len] ) 
	{
		const char ch = tolower(password1[len]);

		if ( ch >= 'a' && ch <= 'z' || ch >= '0' && ch <= '9' ) 
		{
			len++;
		} 
		else 
		{
			bool bAllow = false;
			switch( ch )
			{
			case '!':
			case '@':
			case '#':
			case '$':
			case '%':
			case '^':
			case '&':
			case '*':
			case '(':
			case ')':
			case '_':
			case '+':
			case '{':
			case '}':
			case '|':
			case '<':
			case '>':
			case '?':
				{
					bAllow = true;
					len++;
				}
			}

			if( bAllow == false )
				return -1;				// 不合法字元
		}
	}

	if ( len < 8 ) {
		// 字元長度太短
		return -2;
	}

	if ( strcmp(password1, password2) != 0 ) {
		// 確認密碼不正確
		return -3;
	}

	// 檢查密碼必須含有一個英文字和數字
	bool bAlpha		= false;
	bool bNumber	= false;
	
	for( int i=0; i<strlen( password1 ); i++ )
	{
		const char ch = tolower(password1[i]);
		if ( ch >= 'a' && ch <= 'z' )
		{
			bAlpha = true;
		}
			
		if( ch >= '0' && ch <= '9' ) 
		{
			bNumber = true;
		}
	}

	if( bAlpha == false || bNumber == false )
		return -4;


	CNetCli_Login::SetSecondPassword((char*)password1);
	SendLoginEvent("PASSWORD_HIDE");

	// 調整顯示設定
	{
		char filename[512];
		const char* savePath = g_ruResourceManager->GetRootDirectory();
		if ( savePath && strlen(savePath) > 0 )
			sprintf(filename, "%s\\%s", savePath, "client.config.ini");
		else
			strcpy(filename, "client.config.ini");

		if ( PathFileExists(filename) == false )
		{
			SendLoginEvent("ADJUST_DISPLAY");
		}
	}

	return 0;
}

//return false if client version is different from server;
bool CAccountLogin::CheckServerVersion()
{
	bool Result = true;
	
	string clientVersion = g_pGameMain->GetVersion();
	string versionURL = g_pGameMain->GetVersionURL();

	//no client version information, no further check
	if ((clientVersion == "") || (versionURL == ""))
		return true;

	// 台版不在檢查 Version
	if( g_pGameMain->CheckCountry("TW") )
		return true;

	MyHttpFile httpFile;

	MyHttpFile::Init();
	httpFile.SetDownloadUrl(versionURL.c_str());
	httpFile.SetDownloadPort(g_pGameMain->GetDownloadPort());
	httpFile.Download_NoThread(
		"./"			,
		"version.txt"	,
		"version.tmp"	,
		5);

	MyHttpFile::Release();

	char Buffer[32];

	SecureZeroMemory(Buffer, sizeof(Buffer));
	GetPrivateProfileStringA("Current Version", "Version", NULL, Buffer, sizeof(Buffer), "./version.tmp");
	string serverVersion = Buffer;

	if ((serverVersion != ""))
	{
		string srvVersion[4] = {"", "", "", ""};
		string cliVersion[4] = {"", "", "", ""};

		for (int i = 0; i < 4; i++)
		{
			int srvInd = (int)serverVersion.find_first_of('.');

			if ((srvInd == -1))
			{
				srvVersion[i] = serverVersion;
				break;
			}
			else
			{
				srvVersion[i] = serverVersion.substr(0, srvInd);
				serverVersion = serverVersion.substr(srvInd + 1);
			}
		}

		for (int i = 0; i < 4; i++)
		{
			int cliInd = (int)clientVersion.find_first_of('.');

			if ((cliInd == -1))
			{
				cliVersion[i] = clientVersion;
				break;
			}
			else
			{
				cliVersion[i] = clientVersion.substr(0, cliInd);
				clientVersion = clientVersion.substr(cliInd + 1);
			}
		}

		if ((srvVersion[0] != cliVersion[0]) ||
			(srvVersion[1] != cliVersion[1]) ||
			(srvVersion[2] != cliVersion[2]) ||
			(srvVersion[3] != cliVersion[3]))
		{
			Result = false;
		}
	}
	else
	{
		Result = false; 
	}

	DeleteFile("./version.tmp");

	return Result;
}

// ----------------------------------------------------------------------------
void CAccountLogin::LoadServerList(char* data, int size, int characters[30])
{	
	lua_State* L = GetLuaState();

	m_serverList.clear();
	if ( data == NULL || size < 0 )
		return;

	IniFile2 serverList;
	serverList.SetIniData(data, size);

	char temp[256];
	for (int i = 0; i < 32; i++)
	{		
		string str;
		sprintf_s(temp, 256, "Server%d", i+1);
		serverList.SetSection(temp);
		str = serverList.Str("WorldID");
		if ( str.size() > 0 )
		{
			ServerListInfo info;

			info.characters			= 0;
			info.load				= 100.0f;
			info.isPVP				= false;
			info.isMaintain			= false;
			info.isLogin			= false;
			info.isNew				= false;
			info.isLimitNewAccount	= false;
			info.serverStatus		= 0;
			info.worldID			= atoi(str.c_str());
			info.age				= 0;

			// 隱藏不可見
			char delim[] = " ,";
			str = serverList.Str("HideCountry");

			const char* country = strtok((char*)str.c_str(), delim);
			while ( country != NULL )
			{
				if ( stricmp(country, m_ipCountry.c_str()) == 0 )
					break;
				country = strtok(NULL, delim);
			}

			if ( country != NULL )
				continue;

			// 伺服器狀態
			str = serverList.Str("ServerStatus");
			info.serverStatus = atoi(str.c_str());

			// 維修
			if ( info.serverStatus == 1 )
			{
				info.isMaintain = true;
			}
			// 隱藏
			else if ( info.serverStatus == 2 )
			{
				// 如果為隱藏並且沒有可登入權限
				if ( g_pGameMain->IsDiaplayMaintainServer() == false )
					continue;
			}			

			str = serverList.Str("Location");
			info.location = str;

			str = serverList.Str("Name");
			info.serverName = str;

			str = serverList.Str("PVP");
			info.isPVP = (bool)stricmp(str.c_str(), "false");

			str = serverList.Str("AlwaysTop");
			info.isTop = (stricmp(str.c_str(), "true") == 0);

			str = serverList.Str("New");
			info.isNew = (stricmp(str.c_str(), "true") == 0);

			str = serverList.Str("LimitNewAccount");
			info.isLimitNewAccount = (stricmp(str.c_str(), "true") == 0);

			str = serverList.Str("FlagText");
			info.flagText = str;

			str = serverList.Str("Age");
			info.age = atoi(str.c_str());

			if ( info.age <= 15 )
				info.age = 0;

			m_serverList.push_back(info);
		}
	}

	vector<ServerListInfoStruct>& serverListInfo	= CNetCli_Login_Child::s_serverListInfo;
	int	lastLoginWorldID							= CNetCli_Login_Child::s_lastLoginWorldID;
	string lastServerName;
	

	// 移除不可見
	if ( g_pGameMain->IsDiaplayMaintainServer() == false )
	{
		vector<ServerListInfo>::iterator iter = m_serverList.begin();
		while ( iter != m_serverList.end() )
		{
			bool next = true;

			// 判斷伺服器送來狀態為隱藏
			for ( vector<ServerListInfoStruct>::iterator infoIter = serverListInfo.begin(); infoIter != serverListInfo.end(); infoIter++ )
			{
				if ( infoIter->WorldID == iter->worldID )
				{
					if ( infoIter->ServerStatus == 2 )
					{
						iter = m_serverList.erase(iter);
						next = false;
					}
					break;
				}
			}

			if ( next )
				iter++;
		}
	}	

	for ( vector<ServerListInfo>::iterator iter = m_serverList.begin(); iter != m_serverList.end(); iter++ )
	{
		for ( vector<ServerListInfoStruct>::iterator infoIter = serverListInfo.begin(); infoIter != serverListInfo.end(); infoIter++ )
		{
			if ( infoIter->WorldID == iter->worldID )
			{
				iter->load			= infoIter->TotalPlayerRate;
				iter->serverIP		= infoIter->Port;
				iter->isLogin		= true;

				if ( iter->load <= 0.0f )
					iter->load = 0.1f;

				// 每部伺服器狀態 是否維修中或者隱藏
				switch (infoIter->ServerStatus)
				{
				case 0:
					break;

					// 維修
				case 1:
					iter->isMaintain = true;		// 維修中

					/*
					if ( g_pGameMain->IsDiaplayMaintainServer()  == false )
					{
						iter->load = 100.0f;		// 離線
						iter->isLogin = false;		// 無法登入
					}
					*/
					break;

					// 隱藏
				case 2:
					iter->isMaintain = true;		// 維修中
					break;
				}

				// 伺服器列表狀態 是否維修中
				if ( infoIter->ServerListMode == 1 )
				{	
					/*
					if ( g_pGameMain->IsDiaplayMaintainServer() )
					{
						iter->isMaintain = true;	// 維修中可登入
					}
					else
					{
						iter->load = 100.0f;		// 離線
						iter->isLogin = false;		// 無法登入
					}
					*/
					iter->isMaintain = true;	// 維修中可登入
				}

				// 維修中
				if ( iter->isMaintain )
				{
					if ( g_pGameMain->IsDiaplayMaintainServer() == false )
					{
						iter->load = 100.0f;		// 離線
						iter->isLogin = false;		// 無法登入
					}
				}

				/*
				// 修正目前最後選擇伺服器名稱
				if ( m_realmName.compare(infoIter->Name) == 0 )
					m_realmName		= iter->serverName;
				*/

				// 伺服器名稱以INI為準
				infoIter->Name		= iter->serverName;

				// 記錄最後登入名稱
				if ( lastLoginWorldID == iter->worldID )
					lastServerName = iter->serverName;

				break;
			}
		}

		if ( iter->worldID > 0 && iter->worldID <= 30 )
		{
			iter->characters = characters[iter->worldID - 1];
			if ( iter->characters > 0 && iter->isLimitNewAccount )
				iter->isLimitNewAccount = false;
		}
	}

	// 使用上次最後登入伺服器登入
	if ( g_pGameMain->GetLastRealmState() && m_realmName.empty() )
	{
		SetRealmName(lastServerName.c_str());
	}

	m_lastReciveServerListTime = (float)g_pGameMain->GetFrameTime();
	if ( (m_gameServerIP.empty() || m_gameServerPort == 0) && m_serverListData == false )
	{
		g_serverListLastSortItem.clear();		

		// 先以壓力排序
		SortServerList("load");

		// 是否有角色在依角色排序
		bool hasCharacter = false;
		for ( vector<ServerListInfo>::iterator iter = m_serverList.begin(); iter != m_serverList.end(); iter++ )
		{
			if ( iter->characters > 0 )
			{
				hasCharacter = true;
				break;
			}
		}		

		if ( hasCharacter )
		{
			SortServerList("character", 1);
		}

		// 關閉詢息視窗
		SendLoginEvent("CLOSE_LOGIN_DIALOG");

		// 顯示伺服器列表
		SendLoginEvent("OPEN_SERVER_LIST");		

		// 顯示有效時間
		if ( m_validTime > 0 && strcmp(g_pGameMain->GetCountry(), "KR") == 0 )
		{
			lua_pushstring(L, "OKAY");
			lua_setglobal(L, "arg1");

			string str = g_ObjectData->GetString("SYS_LOGIN_ACCOUNT_VALID_TIME");
			if ( m_validTime > 86400 ) {	
				// 天數
				sprintf_s(temp, 256, "%d%s", m_validTime / 86400 , g_ObjectData->GetString("SYS_DAY"));
			} else if ( m_validTime > 3600 ) {
				// 小時
				sprintf_s(temp, 256, "%d%s", m_validTime / 3600, g_ObjectData->GetString("SYS_HOUR"));
			} else if ( m_validTime > 60 ) {
				// 分鐘
				sprintf_s(temp, 256, "%d%s", m_validTime / 60, g_ObjectData->GetString("SYS_MINUTE"));
			} else {
				// 秒
				sprintf_s(temp, 256, "%d%s", max(m_validTime, 0), g_ObjectData->GetString("SYS_SECOND"));
			}
			string timeStr = temp;
			str = ReplaceKeyString(str, "<TIME>", timeStr);

			lua_pushstring(L, str.c_str());
			lua_setglobal(L, "arg2");
			SendLoginEvent("OPEN_LOGIN_DIALOG");
		}
	}
	else
	{
		string sortItem = g_serverListLastSortItem;
		g_serverListLastSortItem.clear();

		SortServerList(sortItem.c_str());

		// 更新伺服器列表
		SendLoginEvent("SERVER_LIST_UPDATE");
	}
	
	m_serverListData = true;
}

// ----------------------------------------------------------------------------
bool SortServerListByLocation(const ServerListInfo& _Left, const ServerListInfo& _Right)
{
	if ( _Left.isTop != _Right.isTop )
		return _Left.isTop ? true : false;

	if ( g_serverListSortNegative )
		return _Left.location > _Right.location;
	else
		return _Left.location < _Right.location;
}

// ----------------------------------------------------------------------------
bool SortServerListByName(const ServerListInfo& _Left, const ServerListInfo& _Right)
{
	if ( _Left.isTop != _Right.isTop )
		return _Left.isTop ? true : false;

	if ( g_serverListSortNegative )
		return _Left.serverName > _Right.serverName;
	else
		return _Left.serverName < _Right.serverName;
}

// ----------------------------------------------------------------------------
bool SortServerListByType(const ServerListInfo& _Left, const ServerListInfo& _Right)
{
	if ( _Left.isTop != _Right.isTop )
		return _Left.isTop ? true : false;

	if ( g_serverListSortNegative )
		return _Left.isPVP > _Right.isPVP;
	else
		return _Left.isPVP < _Right.isPVP;
}

// ----------------------------------------------------------------------------
bool SortServerListByCharacter(const ServerListInfo& _Left, const ServerListInfo& _Right)
{
	if ( _Left.isTop != _Right.isTop )
		return _Left.isTop ? true : false;

	if ( g_serverListSortNegative )
		return _Left.characters > _Right.characters;
	else
		return _Left.characters < _Right.characters;
}

// ----------------------------------------------------------------------------
bool SortServerListByLoad(const ServerListInfo& _Left, const ServerListInfo& _Right)
{
	if ( _Left.isTop != _Right.isTop )
		return _Left.isTop ? true : false;

	if ( g_serverListSortNegative )
		return _Left.load > _Right.load;
	else
		return _Left.load < _Right.load;
}

// ----------------------------------------------------------------------------
bool SortServerListByWorldID(const ServerListInfo& _Left, const ServerListInfo& _Right)
{
	if ( _Left.isTop != _Right.isTop )
		return _Left.isTop ? true : false;

	if ( g_serverListSortNegative )
		return _Left.worldID > _Right.worldID;
	else
		return _Left.worldID < _Right.worldID;
}

// ----------------------------------------------------------------------------
void CAccountLogin::SortServerList(const char* item, int forceOrder)
{
	switch (forceOrder)
	{
	case 1:
		g_serverListSortNegative = true;
		break;
	case 2:
		g_serverListSortNegative = false;
		break;

	default:
		if ( strcmp(g_serverListLastSortItem.c_str(), item) == 0 )
			g_serverListSortNegative = !g_serverListSortNegative;
		else
			g_serverListSortNegative = false;
		break;
	}	

	if ( strcmp(item, "location") == 0 )
		stable_sort(m_serverList.begin(), m_serverList.end(), SortServerListByLocation);
	else if ( strcmp(item, "name") == 0 )
		stable_sort(m_serverList.begin(), m_serverList.end(), SortServerListByName);
	else if ( strcmp(item, "type") == 0 )
		stable_sort(m_serverList.begin(), m_serverList.end(), SortServerListByType);
	else if ( strcmp(item, "character") == 0 )
		stable_sort(m_serverList.begin(), m_serverList.end(), SortServerListByCharacter);
	else if ( strcmp(item, "load") == 0 )
		stable_sort(m_serverList.begin(), m_serverList.end(), SortServerListByLoad);
	else if ( strcmp(item, "id") == 0 )
		stable_sort(m_serverList.begin(), m_serverList.end(), SortServerListByWorldID);
	else
		return;

	g_serverListLastSortItem = item;
	SendLoginEvent("SERVER_LIST_UPDATE");
}

// ----------------------------------------------------------------------------
void CAccountLogin::DownloadBoardString(OnBoardStringDownloaded cbDownloaded)
{
	if (m_boardStringDownloadStatus != eBoardStringReadytoDownloading)
	{
		string noticeFullURL = g_pGameMain->GetNoticeURL();

		if ( noticeFullURL.empty() )
			return;

		string _host			= "";
		string _remoteFileName	= "";

		//tokenize string
		size_t _pos = noticeFullURL.rfind("/");

		if (_pos != string::npos)
		{
			_remoteFileName = noticeFullURL.substr(_pos + 1);
			_host			= noticeFullURL.erase(_pos);
		}
		
		m_boardStringFile.SetDownloadUrl(_host.c_str());	
		m_boardStringFile.SetDownloadPort(g_pGameMain->GetDownloadPort());
		m_boardStringFile.Download(
			"./"					,
			_remoteFileName.c_str()	,
			"boardstring.tmp"
			);

		//Set CallBackFunction
		m_cbBoardStringDownloaded = cbDownloaded;

		//restart downloading.
		m_boardStringDownloadStatus = eBoardStringReadytoDownloading;
	}
}

// ----------------------------------------------------------------------------
void CAccountLogin::CheckBoardStringDownloadStatus()
{
	switch (m_boardStringDownloadStatus)
	{
	case eBoardStringNotDownloaded:
		break;
	case eBoardStringReadytoDownloading:
		{
			switch (m_boardStringFile.State())
			{
				case EM_MyHttpFileProcessType_Download:
					break;
				case EM_MyHttpFileProcessType_OK:
					{

						m_boardString.clear();
						FILE* fptr = fopen("./boardstring.tmp", "r");
						if ( fptr != NULL )
						{
							fseek(fptr, 0, SEEK_END);
							int size = ftell(fptr);
							char* temp = new char[size + 1];
							memset(temp, 0, size + 1);
							fseek(fptr, 0, SEEK_SET);
							fread(temp, size, 1, fptr);
							m_boardString = temp;
							delete [] temp;

							DeleteFile("./boardstring.tmp");

							if (m_cbBoardStringDownloaded)
							{
								m_cbBoardStringDownloaded(m_boardString.c_str());
							}

							m_boardStringDownloadStatus = eBoardStringDownloaded;
						}
						else
						{
							m_boardStringDownloadStatus = eBoardStringNotDownloaded;
						}

						break;
					}
				case EM_MyHttpFileProcessType_Failed:
				case EM_MyHttpFileProcessType_Interrupt:
					{
						m_boardString = "";
						m_boardStringDownloadStatus = eBoardStringNotDownloaded;
						break;
					}				
			}
			break;
		}		
	case eBoardStringDownloaded:
		break;
	}
}

// ----------------------------------------------------------------------------
void CAccountLogin::ReceiveDownloadBoardString(const char* text)
{
	if ( g_pAccountLogin && text && strlen(text) > 0 )
		g_pAccountLogin->SendLoginEvent("SERVER_BOARD_UPDATE");
}

// ----------------------------------------------------------------------------
void CAccountLogin::SetServerAccount(const char* text)
{
	/*
	CRuCrypto_MD5 md5;
	m_serverListAccount = md5.CalculateMD5Digest( text );
	*/
	m_serverAccountName = text;
	m_serverListAccount = text;
}

// ----------------------------------------------------------------------------
const char*	CAccountLogin::GetServerAccount()
{
	return m_serverListAccount.c_str();
}

// ----------------------------------------------------------------------------
const char* CAccountLogin::FindWorldNameByID(int worldID)
{
	for ( vector<ServerListInfo>::iterator iter = m_serverList.begin(); iter != m_serverList.end(); iter++ )
	{
		if ( iter->worldID == worldID )
			return iter->serverName.c_str();
	}
	return NULL;
}

// ----------------------------------------------------------------------------
int	CAccountLogin::FindWorldIDByName(const char* serverName)
{
	for ( vector<ServerListInfo>::iterator iter = m_serverList.begin(); iter != m_serverList.end(); iter++ )
	{
		if ( iter->serverName.compare(serverName) == 0 )
			return iter->worldID;
	}
	return -1;
}

// ----------------------------------------------------------------------------
void CAccountLogin::CloseServerList()
{
	CNetCli_Login_Child::CloseServerList();

	m_serverListData = false;

	m_connect.realmList = LOGIN_DISCONNECT;

	/*
	if ( m_connect.realmList != LOGIN_DISCONNECT )
	{
		Logout(EM_ClientNetCloseType_ReturnAccount, true);
	}
	*/
}

// ----------------------------------------------------------------------------
void CAccountLogin::RequestServerList()
{
	float fraemTime = (float)g_pGameMain->GetFrameTime();

	// 每10秒更新己次
	if ( fraemTime > m_lastReciveServerListTime + 10.0f )
	{
		m_lastReciveServerListTime = fraemTime;
		CNetCli_Login_Child::RequestServerList();
	}
}

// ----------------------------------------------------------------------------
void CAccountLogin::SendConfirmSecondPassword(const char* password)
{
	/*
	if ( m_passwordFailedCount > 3 )
	{
		// 密碼錯誤3次
		SendLoginEvent("PASSWORD_THIRD_FAILED");
	}
	else
	*/
	{
		CRuCrypto_MD5	md5;
		m_tempSecondPassword = md5.CalculateMD5Digest( password );
		std::transform(m_tempSecondPassword.begin(), m_tempSecondPassword.end(), m_tempSecondPassword.begin(), toupper);

		CNetCli_Login::SM_SecondPassword( m_tempSecondPassword.c_str() );
		m_tempSecondPassword.clear();
	}
}

// ----------------------------------------------------------------------------
void CAccountLogin::RM_SecondPasswordRequest()
{
	m_showConfirmPassword = true;

	if ( m_returnSelectCharacter && !m_tempSecondPassword.empty() )
	{
		SendConfirmSecondPassword(m_tempSecondPassword.c_str());
	}
	else
	{
		SendLoginEvent("CONFIRM_PASSWORD");
	}

	/*
	if ( m_secondPassword.empty() )
	{
		SendLoginEvent("CONFIRM_PASSWORD");
	}
	else
	{
		SendConfirmSecondPassword(m_secondPassword.c_str());
	}
	*/
}

// ----------------------------------------------------------------------------
void CAccountLogin::RM_SecondPasswordResult(bool Result)
{
	m_returnSelectCharacter = false;

	if ( Result )
	{
		m_passwordFailedCount = 0;		

		// m_secondPassword = m_tempSecondPassword;
	}
	else
	{
		m_passwordFailedCount++;

		if ( m_inputErrorCount < 5 )
		{
			// 密碼輸入錯誤
			SendLoginEvent("CONFIRM_PASSWORD_FAILED");
		}
		else
		{
			// 密碼錯誤 5 次
			// SendLoginEvent("LOGIN_LOCK_CHARACTER");
		}		
	}
}

// ----------------------------------------------------------------------------
void CAccountLogin::SM_SecondPassword2(const char* password)
{
	CRuCrypto_MD5	md5;
	m_tempSecondPassword = md5.CalculateMD5Digest( password );
	std::transform(m_tempSecondPassword.begin(), m_tempSecondPassword.end(), m_tempSecondPassword.begin(), toupper);

	CNetCli_Login::SM_SecondPassword2( m_tempSecondPassword.c_str() );
	m_tempSecondPassword.clear();
}
// ----------------------------------------------------------------------------
void CAccountLogin::SM_CaptchaRefresh()
{
	CNetCli_Login::SM_CaptchaRefresh();
}

// ----------------------------------------------------------------------------
void CAccountLogin::SM_CaptchaReply( const char* Captcha )
{
	CNetCli_Login::SM_CaptchaReply(Captcha);
}

// ----------------------------------------------------------------------------
void CAccountLogin::RM_SecondPasswordRequest2()
{
	SendLoginEvent("CONFIRM_PASSWORD2");
}

// ----------------------------------------------------------------------------
void CAccountLogin::RM_SecondPasswordResult2( bool Result )
{
	m_returnSelectCharacter = false;

	if ( !Result )
	{
		m_passwordFailedCount++;

		if ( m_inputErrorCount < 5 )
		{
			// 密碼輸入錯誤
			SendLoginEvent("CONFIRM_PASSWORD_FAILED2");
		}
	}
}

// ----------------------------------------------------------------------------
void CAccountLogin::RM_CaptchaCheck(void* Captcha, int CaptchaSize)
{
	//convert RGB 565 to RGB888
	PBYTE pOrgTraveler = (PBYTE)Captcha;

	BITMAPFILEHEADER* pbmfOrg = (BITMAPFILEHEADER*)pOrgTraveler;
	pOrgTraveler += sizeof(BITMAPFILEHEADER);
	BITMAPINFOHEADER* pbiOrg  = (BITMAPINFOHEADER*)pOrgTraveler;
	pOrgTraveler += sizeof(BITMAPINFOHEADER);
	DWORD dwRMask = *(PDWORD)pOrgTraveler;
	pOrgTraveler += sizeof(DWORD);
	DWORD dwGMask = *(PDWORD)pOrgTraveler;
	pOrgTraveler += sizeof(DWORD);
	DWORD dwBMask = *(PDWORD)pOrgTraveler;
	pOrgTraveler += sizeof(DWORD);

	//------------------------------------------------------------------------------
	unsigned long dwImageSize = (pbiOrg->biWidth * pbiOrg->biHeight * 3) + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	void* pBmp = new BYTE[dwImageSize];
	SecureZeroMemory(pBmp, dwImageSize);
	PBYTE pTraveler = (PBYTE)pBmp;

	//------------------------------------------------------------------------------
	//fill bitmap file header
	BITMAPFILEHEADER* pbmfHeader = (BITMAPFILEHEADER*) pTraveler;
	pbmfHeader->bfType		= 0x4D42;
	pbmfHeader->bfOffBits	= sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	pbmfHeader->bfSize		= dwImageSize;
	pTraveler += sizeof(BITMAPFILEHEADER);
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	//fill bitmap info header
	BITMAPINFOHEADER* pbi = (BITMAPINFOHEADER*)pTraveler;
	pbi->biSize			 = sizeof(BITMAPINFOHEADER);
	pbi->biWidth		 = pbiOrg->biWidth;
	pbi->biHeight		 = pbiOrg->biHeight;
	pbi->biPlanes		 = 1;
	pbi->biBitCount		 = 24;
	pbi->biCompression   = BI_RGB;
	pbi->biXPelsPerMeter = 2834;
	pbi->biYPelsPerMeter = 2834;
	pTraveler += sizeof(BITMAPINFOHEADER);
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	//fill Pixel
	for (int y = 0; y < pbi->biHeight; ++y)
	{
		for (int x = 0; x < pbi->biWidth; ++x)
		{
			PWORD pPixel = (PWORD)pOrgTraveler;

			*pTraveler		 = (BYTE)((*pPixel) & dwBMask) << 3;	//B
			*(pTraveler + 1) = (BYTE)(((*pPixel) & dwGMask) >> 3);	//G
			*(pTraveler + 2) = (BYTE)(((*pPixel) & dwRMask) >> 8);	//R

			pOrgTraveler += sizeof(WORD);
			pTraveler += 3;
		}
	}
	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------
	//write file
	std::wstring wszPath = CharToWchar(g_pGameMain->GetResourceRootDir());

	wszPath = wszPath + L"\\Interface\\";
	_wmkdir(wszPath.c_str());

	wszPath = wszPath + L"\\Login\\";
	_wmkdir(wszPath.c_str());

	wszPath = wszPath + L"\\Captcha\\";
	_wmkdir(wszPath.c_str());

	wszPath = wszPath + L"Captcha.bmp";

	//for test
	HANDLE hFile = CreateFileW( wszPath.c_str(),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	unsigned long bytesWritten = 0;
	WriteFile(hFile, pBmp, dwImageSize, &bytesWritten, NULL);

	CloseHandle(hFile);
	//------------------------------------------------------------------------------

	//send ui event
	SendLoginEvent("CONFIRM_CAPTCHA");

	delete pBmp;
}

// ----------------------------------------------------------------------------
void CAccountLogin::RM_CaptchaCheckResult( int Result )
{
	m_returnSelectCharacter = false;

	switch (Result)
	{
	case EM_CaptchaCheckResult_OK:
		{
			m_passwordFailedCount = 0;

			std::wstring wszPath = CharToWchar(g_pGameMain->GetResourceRootDir());

			wszPath += L"\\Interface\\Login\\Captcha\\Captcha.bmp";

			DeleteFileW(wszPath.c_str());

			SendLoginEvent("CONFIRM_CAPTCHA_OK");

			break;
		}		
	case EM_CaptchaCheckResult_Failed:
		{
			m_passwordFailedCount++;

			if ( m_inputErrorCount < 5 )
			{
				// 密碼輸入錯誤
				SendLoginEvent("CONFIRM_CAPTCHA_FAILED");
			}

			break;
		}		
	case EM_CaptchaCheckResult_TimeExpired:
		{
			SendLoginEvent("CONFIRM_CAPTCHA_TIME_EXPIRED");
			break;
		}		
	}	
}

// ----------------------------------------------------------------------------
void CAccountLogin::SetVivoxLicenseTime(int vivoxTime)
{
	if ( g_pGameMain->IsVivoxUserAgreementEnable() )
	{
		if ( m_vivoxLincenseTime == 0 && vivoxTime == 0 )
		{
			SendLoginEvent("VIVOX_PACT");
		}
		else if ( m_vivoxLincenseTime >= 0 )
		{
			m_vivoxLincenseTime = vivoxTime;
		}
	}	
}

// ----------------------------------------------------------------------------
void CAccountLogin::InitLoginEnvironment()
{
	int chapter = g_pGameMain->GetLoginEpisodeVersion();
	//chapter = 1 + rand() % 6;
	if (chapter == 7)
	{
		//7.0
		m_AccountLoginCameraLayout.push_back(sCameraLayout(7118.0f, 562.0f, 2806.0f, 32.0f, -56.0f, 0.0f, 272.0f, 0.0f));
		m_AccountLoginCameraLayout.push_back(sCameraLayout(6330.0f, 49.0f, 3194.0f, 15.0f, -73.0f, 0.0f, 158.0f, 1.6f));
		m_AccountLoginCameraLayout.push_back(sCameraLayout(5530.69f, 12.0f, 3575.12f, 9.0f, -64.0f, 0.0f, 30.0f, 1.4f));

		m_fPlayerHeight = 2.0f;
		float angle = 296.00f * ruPI / 180.0f;
		m_PlayerDirection = CRuVector3(-sin(angle), 0.0f, -cos(angle));

		g_pGameMain->SetLoginWdbFileName("wdb\\login\\login_ch7.wdb");
	}
	else if( chapter == 6 )
	{
		//6.0
		m_AccountLoginCameraLayout.push_back( sCameraLayout( -1304.83f, 1827.82f, -818.40f , 4.87f , 53.82f, 0.0f, 272.18f , 0.0f ) );
		m_AccountLoginCameraLayout.push_back( sCameraLayout( -1193.22f, 1649.01f, -780.18f , 38.57f , 56.79f , 0.0f, 120.0f , 1.0f ) );
		m_AccountLoginCameraLayout.push_back( sCameraLayout( -1082.01f, 1624.29f, -711.99f , -2.0f , 52.11f , 0.0f, 30.0f , 1.7f ) );

		m_fPlayerHeight = 1614.1000f;
		float angle = 52.11f * ruPI / 180.0f;
		m_PlayerDirection = CRuVector3( -sin(angle), 0.0f, -cos(angle) );

		g_pGameMain->SetLoginWdbFileName( "wdb\\login\\login_ch6.wdb" );
	}
	else if( chapter == 5 )
	{
		//5.0
		m_AccountLoginCameraLayout.push_back( sCameraLayout( 2729.88f, 534.64f, -3746.72f , 1.60f , 127.0f , 0.0f, 200.0f , 0.0f ) );
 		m_AccountLoginCameraLayout.push_back( sCameraLayout( 3470.70f, 237.20f, -4064.82f , 4.2f , 129.60f , -1.5f, 420.63f , 1.0f ) );
		m_AccountLoginCameraLayout.push_back( sCameraLayout( 3470.70f, 237.20f, -4064.82f , -2.0f , 127.71f , 0.0f, 30.0f , 1.7f ) );

		m_fPlayerHeight = 227.66000f;
		float angle = 127.71f * ruPI / 180.0f;
		m_PlayerDirection = CRuVector3( -sin(angle), 0.0f, -cos(angle) );

		g_pGameMain->SetLoginWdbFileName( "wdb\\login\\login_ch5.wdb" );
	}
	else if( chapter == 4 )
	{
		//4.0
		m_AccountLoginCameraLayout.push_back( sCameraLayout( 336.13f, 430.46f, -824.29f , -2.29f , -37.24f ,  0.0f, 178.52f , 0.0f   ) );
		m_AccountLoginCameraLayout.push_back( sCameraLayout( 420.88f, 386.44f, -760.94f , 12.81f , -17.06f , -2.0f,  70.28f , 1.5f ) );
		m_AccountLoginCameraLayout.push_back( sCameraLayout( 420.88f, 386.44f, -760.94f , -2.0f  , -37.06f ,  0.0f,  32.28f , 0.8f ) );

		m_fPlayerHeight = 376.90f;
		float angle = -37.06f * ruPI / 180.0f;
		m_PlayerDirection = CRuVector3( (float)-sin(angle), 0.0f, (float)-cos(angle) );

		g_pGameMain->SetLoginWdbFileName( "wdb\\login\\login_ch4.wdb" );
	}
	else if( chapter == 3 || ( chapter == 0 && g_pGameMain->IsCh3LoginEnable() ) )
	{
		//3.0
		m_AccountLoginCameraLayout.push_back( sCameraLayout( 2913.30f, 1033.06f, 2844.51f, -14.71f, 17.84f, 0.0f, 567.60f, 0.0f) );
		m_AccountLoginCameraLayout.push_back( sCameraLayout( 2924.28f, 891.88f,  2410.28f, -6.67f,  -7.44f, 1.0f, 55.0f,   2.0f) );
		m_AccountLoginCameraLayout.push_back( sCameraLayout( 2924.28f, 891.88f,  2410.28f, -5.67f,  0.57f,  0.0f, 30.0f,   1.0f) );


		m_fPlayerHeight = 881.00f;
		m_PlayerDirection.Set( 0.0f, 0.0f, -1.0f );

		g_pGameMain->SetLoginWdbFileName( "wdb\\login\\login_ch3.wdb" );
	}
	else if ( chapter == 2 || ( chapter == 0 && g_ObjectData->GetObj(1002) && g_ObjectData->GetObj(1003) ) )
	{		
		//2.0

		m_AccountLoginCameraLayout.push_back( sCameraLayout( 1535.01f, 349.52f, 5118.13f, -5.53f, -0.99f,  0.0f, 176.0f, 0.0f) );
		m_AccountLoginCameraLayout.push_back( sCameraLayout( 1412.53f, 277.1f,  5283.38f, 20.19f, -10.76f, 3.0f, 120.0f, 1.5f) );
		m_AccountLoginCameraLayout.push_back( sCameraLayout( 1412.53f, 277.1f,  5283.38f, -6.6f,  4.30f,   0.0f, 30.0f,  0.8f) );


		m_fPlayerHeight = 266.25455f;
		m_PlayerDirection.Set( 0.0f, 0.0f, -1.0f );

		g_pGameMain->SetLoginWdbFileName( "wdb\\login\\login_ch2.wdb" );

	}
	else
	{
		//1.0

		m_AccountLoginCameraLayout.push_back( sCameraLayout( 1560.00f, 50, 3022.00f, 2.0f,  25,     0.0f, 210, 0.0f) );
		m_AccountLoginCameraLayout.push_back( sCameraLayout( 1560.00f, 23, 3022.00f, -4.0f, -11.2f, 0.0f, 30,  2.5f) );


		m_fPlayerHeight = 13.092094f;
		m_PlayerDirection.Set( 0.0f, 0.0f, -1.0f );

		g_pGameMain->SetLoginWdbFileName( "wdb\\login\\login.wdb" );
	}
	
}

// ----------------------------------------------------------------------------
sCameraLayout& CAccountLogin::GetAccountLoginCameraLayoutBegin()
{
	if( m_AccountLoginCameraLayout.empty() )
	{
		m_AccountLoginCameraLayout.push_back( sCameraLayout() );
	}

	std::list<sCameraLayout>::iterator itPos = m_AccountLoginCameraLayout.begin();
	return *itPos;
}

// ----------------------------------------------------------------------------
sCameraLayout& CAccountLogin::GetAccountLoginCameraLayoutEnd()
{
	if( m_AccountLoginCameraLayout.empty() )
	{
		m_AccountLoginCameraLayout.push_back( sCameraLayout() );
	}

	std::list<sCameraLayout>::reverse_iterator itPos = m_AccountLoginCameraLayout.rbegin();
	return *itPos;
}

// ----------------------------------------------------------------------------
void CAccountLogin::DoCameraMotion()
{
	m_ControlCameraLayoutList = m_AccountLoginCameraLayout;
}

// ----------------------------------------------------------------------------
int	LuaFunc_DefaultServerLogin(lua_State *L)
{
	/*
	const char* name = luaL_checkstring(L, 1);
	wstring wname;
	int a = ObjectDataClass::CheckCharacterNameRules(name, EM_CountryType_TH, wname);
	*/
	g_pAccountLogin->Login(luaL_checkstring(L, 1), luaL_checkstring(L, 2));
	return 0;
}

// ----------------------------------------------------------------------------
int	LuaFunc_DisconnectFromServer(lua_State *L)
{
	g_pAccountLogin->Logout(EM_ClientNetCloseType_ReturnAccount, true);
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_StatusDialogClick(lua_State *L)
{
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_AcceptUserAgreement(lua_State *L)
{
	const char* account = g_pGameMain->GetAccountName();
	const char* password = g_pGameMain->GetAccountPassword();

#ifdef KALYDO
	// Use single sign on, if the credentials are already available. If not, set the 'should login' boolean to true
	// so that when the credentials arrive, the game automatically logs in.
	if ( strlen(account) > 0 && strlen(password) > 0 )
	{
		g_pAccountLogin->Login(account, password);
	}
	else
	{
		g_pAccountLogin->SetShouldLogin(true);
	}
	return 0;
#else
	// 自動傳入帳密
	if ( strcmp(g_pGameMain->GetCountry(), "TW") == 0 || strcmp(g_pGameMain->GetCountry(), "VN") == 0 || strcmp(g_pGameMain->GetCountry(), "SG") == 0 )
	{
		if ( strlen(account) > 0 && strlen(password) > 0 )
		{
			g_pAccountLogin->Login(account, password);
		}
	}
	else if ( (g_pGameMain->CheckCountry("JP") == true) ) //else if ( strcmp(g_pGameMain->GetCountry(), "JP") == 0 )
	{
		if ( account && password )
		{
			g_pAccountLogin->Login(account, password);
		}
	}
	else if  (g_pGameMain->IsGameForgeCountries() == true)
	{
		if ( strlen(account) > 0 && strlen(password) > 0 )
		{
			if ( stricmp(account, "AuthorizationCode") == 0 )
			{
				g_pAccountLogin->Login(account, password);
			}
		}
	}
	return 0;
#endif // KALYDO
}

// ----------------------------------------------------------------------------
int LuaFunc_CancelWaitingQueue(lua_State *L)
{
	g_pAccountLogin->Logout(EM_ClientNetCloseType_ReturnAccount, true);
	g_pAccountLogin->ReLogin();
	return 0;
}

// ----------------------------------------------------------------------------
int	LuaFunc_GetCurrentRealm	(lua_State *L)
{
	string& realmName = g_pAccountLogin->GetRealmName();
	if ( realmName.empty() )
		lua_pushnil(L);
	else
		lua_pushstring(L, realmName.c_str());
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetNumRealms(lua_State *L)
{
	lua_pushnumber(L, (lua_Number)CNetCli_Login_Child::s_serverListInfo.size());
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetRealmInfo(lua_State *L)
{
	vector<ServerListInfoStruct>& realmList = CNetCli_Login_Child::s_serverListInfo;
	if ( lua_isnumber(L, 1) )
	{
		int index = (int)lua_tonumber(L, 1) - 1;
		if ( index >= 0 && index < (int)realmList.size() )
		{			
			float load = realmList[index].GetAverageLoading();
			int size = (int)realmList[index].Loading.size();			

			/*
			int size = realmList[index].Loading.size();
			for ( vector<float>::iterator iter = realmList[index].Loading.begin(); iter != realmList[index].Loading.end(); iter++ )
			{
				load += min(1.0f, *iter);
			}
			load /= max(1, size);
			*/

			lua_pushstring(L, realmList[index].Name.c_str());		// 名稱
			lua_pushnumber(L, size);								// 個數
			lua_pushnumber(L, load);								// 壓力
			return 3;
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_ChangeRealm(lua_State *L)
{
	if ( lua_tostring(L, 1) )
	{		
		g_pAccountLogin->SetRealmName(lua_tostring(L, 1));
	}
	return 0;
}

// ----------------------------------------------------------------------------
int	LuaFunc_GetUserAgreementText(lua_State *L)
{	
	char temp[512];
	sprintf_s(temp, 512, "Other\\%s\\UserAgreement.txt", g_pGameMain->GetCountry());
	IRuStream* stream = g_ruResourceManager->LoadStream(temp);
	if ( stream ) {
		int offset = 0;
		if ( stream->GetStreamSize() > 3 )
		{
			BYTE* UTF8_BOM = stream->OpenStreamMapping();
			if ( UTF8_BOM[0] == 0xEF && UTF8_BOM[1] == 0xBB && UTF8_BOM[2] == 0xBF )
			{
				offset = 3;
			}
		}

		char *tempData = ruNEW char [stream->GetStreamSize() + 1 - offset];
		memcpy( tempData, stream->OpenStreamMapping() + offset, stream->GetStreamSize() - offset );
		tempData[stream->GetStreamSize() - offset] = 0;
		lua_pushstring(L, tempData);
		delete [] tempData;
		delete stream;
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_IsDisplayVivoxUserAgreement (lua_State *L)
{
	lua_pushboolean(L, g_pGameMain->IsVivoxUserAgreementEnable());
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetVivoxUserAgreementText	(lua_State* L)
{
	char temp[512];
	sprintf_s(temp, 512, "Other\\%s\\VivoxUserAgreement.txt", g_pGameMain->GetCountry());
	IRuStream* stream = g_ruResourceManager->LoadStream(temp);
	if ( stream ) {
		int offset = 0;
		if ( stream->GetStreamSize() > 3 )
		{
			BYTE* UTF8_BOM = stream->OpenStreamMapping();
			if ( UTF8_BOM[0] == 0xEF && UTF8_BOM[1] == 0xBB && UTF8_BOM[2] == 0xBF )
			{
				offset = 3;
			}
		}

		char *tempData = ruNEW char [stream->GetStreamSize() + 1 - offset];
		memcpy( tempData, stream->OpenStreamMapping() + offset, stream->GetStreamSize() - offset );
		tempData[stream->GetStreamSize() - offset] = 0;
		lua_pushstring(L, tempData);
		delete [] tempData;
		delete stream;
	} else {
		lua_pushnil(L);
	}
	return 1;
}

// ----------------------------------------------------------------------------
int	LuaFunc_AcceptVivoxUserAgreement(lua_State* L)
{
	CNetCli_Login_Child::SetVivoxLicenseTime();
	g_pAccountLogin->SetVivoxLicenseTime(-1);
	g_pGameMain->SetVoiceDisable(false);
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_DeclineVivoxUserAgreement(lua_State* L)
{
	g_pGameMain->SetVoiceDisable(true);
	g_pAccountLogin->SetVivoxLicenseTime(-1);
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetAccountName(lua_State *L)
{
	lua_pushstring(L, g_pAccountLogin->GetAccountName());
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_SetSecondPassword(lua_State *L)
{
	lua_pushnumber(L, g_pAccountLogin->SetSecondPassword(luaL_checkstring(L, 1), luaL_checkstring(L, 2)));
	return 1;
}


// ----------------------------------------------------------------------------
int LuaFunc_SetCameraLayout(lua_State* L)			// 控制攝影機
{
	sCameraLayout s;
	s.pos.m_x = (lua_isnumber(L, 1)) ? (float)lua_tonumber(L, 1) : 0.0f;
	s.pos.m_y = (lua_isnumber(L, 2)) ? (float)lua_tonumber(L, 2) : 0.0f;
	s.pos.m_z = (lua_isnumber(L, 3)) ? (float)lua_tonumber(L, 3) : 0.0f;
	s.pitch   = (lua_isnumber(L, 4)) ? (float)lua_tonumber(L, 4) : 0.0f;
	s.yaw     = (lua_isnumber(L, 5)) ? (float)lua_tonumber(L, 5) : 0.0f;
	s.roll    = (lua_isnumber(L, 6)) ? (float)lua_tonumber(L, 6) : 0.0f;
	s.zoom    = (lua_isnumber(L, 7)) ? (float)lua_tonumber(L, 7) : 0.0f;
	s.time    = (lua_isnumber(L, 8)) ? (float)lua_tonumber(L, 8) : 0.0f;

	g_pAccountLogin->GetAccountLoginCameraLayoutEnd() = s;

	g_pAccountLogin->SetCameraLayout( s );
	return 0;

}

// ----------------------------------------------------------------------------
int LuaFunc_AddControlCamera(lua_State* L)			// 控制攝影機
{
	sCameraLayout s;
	s.pos.m_x = (lua_isnumber(L, 1)) ? (float)lua_tonumber(L, 1) : 0.0f;
	s.pos.m_y = (lua_isnumber(L, 2)) ? (float)lua_tonumber(L, 2) : 0.0f;
	s.pos.m_z = (lua_isnumber(L, 3)) ? (float)lua_tonumber(L, 3) : 0.0f;
 	s.pitch   = (lua_isnumber(L, 4)) ? (float)lua_tonumber(L, 4) : 0.0f;
	s.yaw     = (lua_isnumber(L, 5)) ? (float)lua_tonumber(L, 5) : 0.0f;
 	s.roll    = (lua_isnumber(L, 6)) ? (float)lua_tonumber(L, 6) : 0.0f;
	s.zoom    = (lua_isnumber(L, 7)) ? (float)lua_tonumber(L, 7) : 0.0f;
	s.time    = (lua_isnumber(L, 8)) ? (float)lua_tonumber(L, 8) : 0.0f;


	g_pAccountLogin->GetAccountLoginCameraLayoutEnd() = s;

	g_pAccountLogin->AddControlCamera( s );

	return 0;
}

//#define DF_PIToAngle( p ) ( (p) / * 57.295779513082320876798154814105f )   

// ----------------------------------------------------------------------------
int LuaFunc_GetCameraLayout(lua_State* L)
{
	lua_pushnumber(L, g_pAccountLogin->m_CurrentCameraLayout.pos.m_x );
	lua_pushnumber(L, g_pAccountLogin->m_CurrentCameraLayout.pos.m_y );
 	lua_pushnumber(L, g_pAccountLogin->m_CurrentCameraLayout.pos.m_z );
	lua_pushnumber(L, g_pAccountLogin->m_CurrentCameraLayout.pitch   );
	lua_pushnumber(L, g_pAccountLogin->m_CurrentCameraLayout.yaw     );
	lua_pushnumber(L, g_pAccountLogin->m_CurrentCameraLayout.roll    );
	lua_pushnumber(L, g_pAccountLogin->m_CurrentCameraLayout.zoom    );

	return 7;
}

// ----------------------------------------------------------------------------
int LuaFunc_AccountLoginShow(lua_State *L)
{

	//sCameraLayout cameraLayout;

	//cameraLayout.pos.x = 1419.7;
	//cameraLayout.pos.y = 275.1;
	//cameraLayout.pos.z = 5434.59;
	//cameraLayout.pitch = 23.0;
	//cameraLayout.yaw = -70;
	//cameraLayout.roll = 0.0;
	//cameraLayout.zoom = 28.58;
	//cameraLayout.time = 2.0;

	//g_pAccountLogin->AddControlCamera( cameraLayout );

	//g_pAccountLogin->AddControlCamera( g_pAccountLogin->m_AccountLoginCameraLayout );

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetCurrentParallelID(lua_State* L)
{
	lua_pushnumber(L, g_pAccountLogin->GetParalleZoneID() + 1);
	return 1;
}

// ----------------------------------------------------------------------------
int	LuaFunc_GetNumServerList(lua_State* L)
{
	lua_pushnumber(L, (lua_Number)g_pAccountLogin->m_serverList.size());
	return 1;	
}

// ----------------------------------------------------------------------------
int	LuaFunc_GetServerListInfo(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	vector<ServerListInfo>& serverList = g_pAccountLogin->m_serverList;
	if ( index >= 0 && index < (int)serverList.size() )
	{
		ServerListInfo& info = serverList[index];

		float load = info.load;
		if ( load == 100.0f )
			load = 0.0f;

		bool boAge = true;
		if ( info.age > 0 && g_pAccountLogin->GetAge() < info.age )
			boAge = false;

		lua_pushstring(L, info.location.c_str());
		lua_pushstring(L, info.serverName.c_str());
		lua_pushboolean(L, info.isPVP);
		lua_pushnumber(L, info.characters);
		lua_pushnumber(L, load);
		lua_pushboolean(L, info.isMaintain);
		lua_pushboolean(L, info.isLogin);
		lua_pushstring(L, info.flagText.c_str());
		lua_pushboolean(L, info.isTop);
		lua_pushboolean(L, info.isNew);
		lua_pushboolean(L, info.isLimitNewAccount);
		lua_pushboolean(L, !boAge);
		return 12;
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_SortServerList(lua_State* L)
{
	g_pAccountLogin->SortServerList(luaL_checkstring(L, 1));
	return 0;
}

// ----------------------------------------------------------------------------
int	LuaFunc_ChangeServerList(lua_State* L)
{
	if ( lua_isstring(L, 1) )
		g_pAccountLogin->SetRealmName(lua_tostring(L, 1));
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_RefreshServerList(lua_State* L)
{
	g_pAccountLogin->RequestServerList();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_CloseServerList(lua_State* L)
{
	// 中斷連線
	g_pAccountLogin->CloseServerList();

#ifdef KALYDO
	// returning to login screen by pressing cancel on server select, use single sign-on to login again
	// we've logged in before so the credentials should already be there
	g_pAccountLogin->SetShouldLogin(true);
	application->loginWithSSO();
#endif // KALYDO

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetCurrentServerList(lua_State* L)
{
	lua_pushstring(L, g_pAccountLogin->GetRealmName().c_str());
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetServerBoardText(lua_State* L)
{
	lua_pushstring(L, g_pAccountLogin->GetBoardString());
	return 1;
}

// ----------------------------------------------------------------------------
int	LuaFunc_LinkActivateWeb(lua_State* L)
{
	const char* activateURL = g_pGameMain->GetActivateURL();
	if ( activateURL && activateURL[0] )
	{
		ShellExecute(NULL, "open", activateURL, 0, 0, SW_SHOW);
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_OpenActivateExecute(lua_State* L)
{
	std::string activateURL = g_pGameMain->GetActivateURL();
	std::string activateURLArg = g_pGameMain->GetActivateURLArg();

	if ( activateURL.empty() )
	{
		/*
		const char* key = "[$Account]";
		size_t begin;

		if ( (begin = activateURLArg.find(key)) != string::npos )
			activateURLArg.replace( begin, strlen(key), g_pAccountLogin->GetServerAccount() );
		*/

		ShellExecute(NULL, "open", activateURL.c_str(), activateURLArg.c_str(), 0, SW_SHOW);
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_OpenResetPasswordURL(lua_State* L)
{
	UpdateInfInfo info = g_pGameMain->GetUpdateInfo();

	std::string url = info.resetPassswordURL;

	if ( !url.empty() )
	{		
		const char* key = "[$Account]";
		size_t begin;

		if ( url.size() > 0 && (begin = url.find(key)) != string::npos )
			url.replace( begin, strlen(key), g_pAccountLogin->GetServerAccount() );

		ShellExecute(NULL, "open", url.c_str(), 0, 0, SW_SHOW);
	}
	return 0;
}

//------------------------------------------------------
int LuaFunc_IsBeanFanSystem(lua_State* L)
{
	const char* account = g_pGameMain->GetAccountName();
	const char* password = g_pGameMain->GetAccountPassword();

	// 自動傳入帳密
	if ( strcmp(g_pGameMain->GetCountry(), "TW") == 0 )
	{
		if ( strlen(account) > 0 && strlen(password) > 0 )
		{
			lua_pushboolean(L, true);
			return 1;
		}
	}

	return 0;
}

//------------------------------------------------------
int LuaFunc_ConfirmPassword(lua_State* L)
{
	g_pAccountLogin->SendConfirmSecondPassword(luaL_checkstring(L, 1));
	return 0;
}

//------------------------------------------------------
int LuaFunc_GetAccountAge(lua_State* L)
{
	if ( strcmp(g_pGameMain->GetCountry(), "KR") == 0 )
	{
		lua_pushnumber(L, g_pAccountLogin->GetAge());
	}
	else
	{
		lua_pushnumber(L, -1);
	}
	return 1;
}

//------------------------------------------------------
int LuaFunc_GetServerLimitAge(lua_State* L)
{
	if ( strcmp(g_pGameMain->GetCountry(), "KR") == 0 )
	{
		lua_pushnumber(L, g_pAccountLogin->GetServerAge());
	}
	else
	{
		lua_pushnumber(L, -1);
	}
	return 1;
}

//------------------------------------------------------
int LuaFunc_GetServerName(lua_State* L)
{
	const char* szServerName = g_pAccountLogin->FindWorldNameByID( g_pAccountLogin->GetWorldID() );
	if( NULL == szServerName )
	{
		return 0;
	}

	lua_pushstring( L, szServerName );
	return 1;
}

int LuaFunc_GetPasswordErrorCount(lua_State* L)
{
	lua_pushnumber(L, g_pAccountLogin->GetPasswordErrorCount());
	return 1;

}

//------------------------------------------------------
int LuaFunc_ConfirmPasswordV2(lua_State* L)
{
	g_pAccountLogin->SM_SecondPassword2(luaL_checkstring(L, 1));
	return 0;
}

//------------------------------------------------------
int LuaFunc_CaptchaRefresh(lua_State* L)
{
	g_pAccountLogin->SM_CaptchaRefresh();
	return 0;
}

//------------------------------------------------------
int LuaFunc_CaptchaReply(lua_State* L)
{
	g_pAccountLogin->SM_CaptchaReply(luaL_checkstring(L, 1));
	return 0;
}