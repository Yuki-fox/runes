#include <algorithm>
#include "../../mainproc/GameMain.h"
#include "../../Interface/Login/AccountLogin.h"
#include "../../Interface/Login/LoginParent.h"
#include "../../Interface/Login/CharacterSelect.h"
#include "../../Interface/Login/CharacterCreate.h"
#include "../../Interface/Debug/DebugMessage.h"
#include "../../Interface/WorldFrames/LoginBonusFrame.h"

#include "NetCli_Login_Child.h"
#include "../Net_BattleGround/NetCli_BattleGround_Child.h"

//#include "../../tools/ApexClient/RaApexClient.h"


using namespace std;

ServerListCliClass*					CNetCli_Login_Child::s_serverList = NULL;
vector<ServerListInfoStruct>		CNetCli_Login_Child::s_serverListInfo;
bool								CNetCli_Login_Child::s_login = false;
int									CNetCli_Login_Child::s_serverLoginRecount = 0;
int									CNetCli_Login_Child::s_lastLoginWorldID = 0;

// --------------------------------------------------------------------------------------
bool CNetCli_Login_Child::_Init()
{
	if (m_pThis)
		return false;

	m_pThis = new CNetCli_Login_Child;

	s_serverList = new ServerListCliClass;
	s_serverList->Init();
	return CNetCli_Login::_Init();
}

// --------------------------------------------------------------------------------------
bool CNetCli_Login_Child::_Release()
{
	CNetCli_Login::_Release();

	if (m_pThis)
	{
		delete m_pThis;
		m_pThis = NULL;
	}
	if( s_serverList != NULL )
	{
		delete s_serverList;
		s_serverList = NULL;
	}

	return true;
}

// --------------------------------------------------------------------------------------
void CNetCli_Login_Child::On_Login(const char* pszInfo)
{
	//g_pDebugMessage->DebugOutput(1, "login succeed!");	
	//s_waitConnectTime = 0.0f;

#ifdef KALYDO
	// successful login, send statistic but only once
	static bool first = true;
	if(first)
	{
		TAppStatisticsPair statPair[1];
		memset(statPair, 0, sizeof(statPair));
		strcpy(statPair[0].name, "LoggedIn");
		strcpy(statPair[0].value, "1");
		application->sendAppStatistics(statPair, 1, NULL);
		first = false;
	}
#endif // KALYDO

	s_login = true;
	s_enterZone1 = false;

	g_pAccountLogin->LoginResult(EM_NetLogFailed_OK);	
	LoadRoleData(m_szAccount);

	s_serverLoginRecount++;
}

// --------------------------------------------------------------------------------------
void CNetCli_Login_Child::On_Logout()
{
	s_serverLoginRecount--; 

	if ( s_serverLoginRecount < 0 )
	{
		s_serverLoginRecount = 0;
		return;
	}

	s_login = false;

	_ClearRoleData();
	s_localServerList.clear();
	s_waitConnectTime = 0.0f;

	// 將頁面切回"Login"頁次
	g_pDebugMessage->DebugOutput(1, "logout!");

	if ( g_pGameMain->IsEnterWorld() ) {
		g_pGameMain->LeaveWorld();
	}
	
	g_pAccountLogin->ReceiveLogout();
	// g_pGameMain->SendSystemMsg("與伺服器斷線!!");

	//清除戰場排隊資料
	CNetCli_BattleGround_Child::OnLogOut();
}

// --------------------------------------------------------------------------------------
void CNetCli_Login_Child::On_LoginFailed(SYSLogFailedENUM emLogFailed)
{
	g_pAccountLogin->LoginResult(emLogFailed);
	/*
	string sError;
	switch ( emLogFailed )
	{
	case EM_NetLogFailed_PasswordError:
		g_pDebugMessage->DebugOutput( 0, "密碼錯誤!" );
		sError = "密碼錯誤!";
		break;

	case EM_NetLogFailed_ConnectFail:
		g_pDebugMessage->DebugOutput( 0, "連線不成!" );
		sError = "";
		break;

	case EM_NetLogFailed_ConnectError:
		g_pDebugMessage->DebugOutput( 0, "連結位置錯誤!" );
		sError = "連結位置錯誤!";
		break;

	case EM_NetLogFailed_NotFindError:
		g_pDebugMessage->DebugOutput( 0, "找不到帳號!" );
		sError = "找不到帳號!";
		break;

	case EM_NetLogFailed_ProxyNotExist:
		g_pDebugMessage->DebugOutput( 0, "Proxy不存在!" );
		sError = "Proxy不存在!";
		break;

	case EM_NetLogFailed_SwitchDisconnect:
		g_pDebugMessage->DebugOutput( 0, "未完成註冊及與 Switch 斷線!" );
		sError = "未完成註冊及與 Switch 斷線!!";
		break;

	case EM_NetLogFailed_ProxyLoginFailed:
		g_pDebugMessage->DebugOutput( 0, "未完成註冊及與 Proxy 連線失敗!" );
		sError = "未完成註冊及與 Proxy 連線失敗!!";
		break;

	case EM_NetLogFailed_AccountExist:
		g_pDebugMessage->DebugOutput( 0, "帳號重覆登入" );
		sError = "帳號重覆登入!";
		break;


	case EM_NetLogFailed_UnknowError:
		g_pDebugMessage->DebugOutput( 0, "暫時未規類的錯誤!" );
		sError = "暫時未規類的錯誤!";
		break;

	default:
		{
			g_pDebugMessage->DebugOutput( 0, "On_LoginFailed error code!" );
			char szErr[256];
			sprintf( szErr, "Login Failed: Error code = %d", (int) emLogFailed );
			sError = szErr;
		} break;
	}

	MessageBox( NULL, sError.c_str(), "Error", MB_OK );
	*/
}

// --------------------------------------------------------------------------------------
void CNetCli_Login_Child::On_LoginFailedData( SYSLogFailedENUM emLogFailed, void* pData, unsigned long dwSize )
{
	g_pAccountLogin->LoginFailedData(emLogFailed, pData, dwSize);
}

// --------------------------------------------------------------------------------------
void CNetCli_Login_Child::On_ServerConnect(int iServerID , const char* cpszServerName)
{	
	//SetSrvID_Local( iServerID );

	// 清除資料
	//g_pCharacterSelect->Clear();
}

// --------------------------------------------------------------------------------------
void CNetCli_Login_Child::On_ServerDisConnect_Battle( int iServerID )
{
	if( iServerID == _ServerNetID.m_iLocal_Battle )
	{
		g_pDebugMessage->DebugOutput( 1, "Local BattleServer is DisConnect!" );
		_ServerNetID.m_iLocal_Battle = -1;

		// 如果在戰場中沒有收到戰場連線中斷,可能會ZONE已經CRASH 中斷連線
		if ( _InBattleGround )
		{
			s_waitConnectTime = 10.0f;
		}
	}	
}
void CNetCli_Login_Child::On_ServerDisConnect( int iServerID )
{
	//CNetCli_Login_Child::s_X.erase( Sockid );
	bool disconnectLocal = false;

	set<int>::iterator iter = s_localServerList.find(iServerID);
	if ( iter != s_localServerList.end() ) {
		disconnectLocal = true;
		s_localServerList.erase(iter);
	}

	if( iServerID == _ServerNetID.m_iChat )
	{
		g_pDebugMessage->DebugOutput( 1, "Chat Server is DisConnect!" );
	}
	else if( iServerID == _ServerNetID.m_iLocal_Base )
	{
		g_pDebugMessage->DebugOutput( 1, "Local Server is DisConnect!" );
		_ServerNetID.m_iLocal_Base = -1;
	}	
	else if( iServerID == _ServerNetID.m_iMaster )
	{
		// 角色未登入遊戲伺服器並且連線中
		if( g_pGameMain->IsEnterWorld() == false && s_login == true )
		{
			CNetCli_Login_Child::On_Logout();
		}

		g_pDebugMessage->DebugOutput( 1, "Master Server is DisConnect!" );
		g_pError->AddMessage(0, 0, "Master Server is DisConnect!");
	}	

	// 沒有任何連線,倒數計數10秒後斷線
	// 或者戰場中有其一條連線中斷
	if ( disconnectLocal && (s_localServerList.empty() || _InBattleGround) ) {
		s_waitConnectTime = 10.0f;
	}
}

// --------------------------------------------------------------------------------------
void CNetCli_Login_Child::On_LoadRoleDataBegin	()
{	
	g_pCharacterSelect->LoadCharacterStart();
}

// --------------------------------------------------------------------------------------
void CNetCli_Login_Child::On_SamepleRoleData( int iFieldIndex, SampleRoleData* pSampleRole )
{
}

// --------------------------------------------------------------------------------------
void CNetCli_Login_Child::On_SamepleRoleDataEnd()
{
	// 預選最後一個登入過的角色
	int selected = 0;
	int index = 0;
	const char* lastSelectedPlayerName = g_pAccountLogin->GetSelectedPlayerName();
	for ( map<int, SampleRoleData*>::iterator iter = m_mapSampleRole.begin(); iter != m_mapSampleRole.end(); iter++ )
	{
		index++;
		if ( strcmp(iter->second->RoleName.Begin(), lastSelectedPlayerName) == 0 )
		{
			selected = index;
			break;
		}
	}

	if ( selected > 0 )
	{
		g_pCharacterSelect->SetSelected(selected);
	}

	g_pCharacterSelect->LoadCharacterEnd();

#ifdef _AUTOLOGIN
	if( m_bAutoLogin == true )
		SelectRole( m_iAutoLoginRoleID , 0 );
#endif

}

// --------------------------------------------------------------------------------------
void CNetCli_Login_Child::On_SelectRoleResult( EM_SelectRoleResult emResult, int iDBID )
{
	switch ( emResult )
	{
	case EM_SelectRole_OK:		// 要作訊息 , 解索
		g_pDebugMessage->DebugOutput( 1, "EM_SelectRole_OK!" );
		break;

	case EM_SelectRole_No_This:
		g_pDebugMessage->DebugOutput( 0, "EM_SelectRole_No_This!" );
		break;

	case EM_SelectRole_ZoneNotExist:	// 要作訊息		// 發生可能只有 , GM 把 ZONE 重開, 或 ZONE 沒開
		s_enterZone1 = true;
		g_pDebugMessage->DebugOutput( 0, "EM_SelectRole_ZoneNotExist!" );
		// 這邊要秀一個訊息
		{
			lua_State* L = g_pAccountLogin->GetLuaState();
			lua_pushstring(L, "SELECT_CHARACTER_ZONENOTEXIST");
			lua_setglobal(L, "arg1");
			lua_pushnil(L);
			lua_setglobal(L, "arg2");
			g_pAccountLogin->SendLoginEvent("OPEN_LOGIN_DIALOG");
		}
		break;

	case EM_SelectRole_NoSecondPassword_:		// 需要輸入二次密碼才能登入
		g_pAccountLogin->SendSecondPassword();
		break;

	case EM_SelectRole_NeedChangeRoleName_:		// 需要重新更名才能進入遊戲
		{
			SampleRoleData* roleData = GetCharacterInfoByDBID(iDBID);
			if ( roleData != NULL && roleData->SysFlag.ChangeNameEnabled )
			{
				g_pAccountLogin->SendLoginEvent("LOGINRENAME_SHOW");
			}			
		}		
		break;

	case EM_SelectRole_Parallel_Zone_Full_:		// 分流已滿. 請選則其它分流		
		g_pDebugMessage->DebugOutput( 0, "EM_SelectRole_ZoneNotExist!" );
		{
			string realName = g_pAccountLogin->GetRealmName();
			CNetCli_Login_Child::UpdateParalleZonesLoad(realName.c_str(), g_pAccountLogin->GetParalleZoneID(), 1.0f);

			lua_State* L = g_pAccountLogin->GetLuaState();
			lua_pushstring(L, "OKAY");
			lua_setglobal(L, "arg1");
			lua_pushstring(L, g_ObjectData->GetString("SYS_PARALLE_ZONE_LOAD_FULL"));
			lua_setglobal(L, "arg2");
			g_pAccountLogin->SendLoginEvent("OPEN_LOGIN_DIALOG");
		}
		break;
	case EM_SelectRole_ServerShutdown_:	// Server Cloing
		g_pDebugMessage->DebugOutput( 0, "EM_SelectRole_ServerShutdown_!" );
		// 這邊要秀一個訊息
		{
			lua_State* L = g_pAccountLogin->GetLuaState();
			lua_pushstring(L, "OKAY");
			lua_setglobal(L, "arg1");
			lua_pushstring(L, g_ObjectData->GetString("SYS_SELECTROLE_SERVERSHUTDOWN"));
			lua_setglobal(L, "arg2");
			g_pAccountLogin->SendLoginEvent("OPEN_LOGIN_DIALOG");
		}
		break;

	case EM_SelectRole_SecondPasswordErr_:
		{
			lua_State* L = g_pAccountLogin->GetLuaState();
			lua_pushstring(L, "OKAY");
			lua_setglobal(L, "arg1");
			lua_pushstring(L, g_ObjectData->GetString("SYS_SELECTROLE_PASSWORD_FAILED"));
			lua_setglobal(L, "arg2");
			g_pAccountLogin->SendLoginEvent("OPEN_LOGIN_DIALOG");
		}		
		break;

	default:
		g_pDebugMessage->DebugOutput( 0, "Select role failed! reult_code(%d)", emResult );
	}
}

// --------------------------------------------------------------------------------------
void CNetCli_Login_Child::On_DeleteRoleResult( EM_DeleteRoleResult emResult )
{
	lua_State* L = g_pAccountLogin->GetLuaState();

	switch ( emResult )
	{
	case EM_DeleteRole_OK:
		g_pDebugMessage->DebugOutput( 1, "delete role is succeed!" );
		g_pCharacterSelect->UpdateCharacterList();
		g_pCharacterSelect->UpdateSprite();
		break;

	case EM_RoleNotExist:
		if ( L )
		{
			lua_pushstring(L, "OKAY");
			lua_setglobal(L, "arg1");
			lua_pushstring(L, g_ObjectData->GetString("SYS_DELETE_ROLE_NOT_EXIST"));
			lua_setglobal(L, "arg2");
			g_pAccountLogin->SendLoginEvent("OPEN_LOGIN_DIALOG");
		}
		break;

	case EM_DeleteSysErr:
		if ( L )
		{
			lua_pushstring(L, "OKAY");
			lua_setglobal(L, "arg1");
			lua_pushstring(L, g_ObjectData->GetString("SYS_DELETE_ROLE_ERROR"));
			lua_setglobal(L, "arg2");
			g_pAccountLogin->SendLoginEvent("OPEN_LOGIN_DIALOG");
		}
		break;

	case EM_DeletePasswordErr:
		if ( L )
		{
			lua_pushstring(L, "OKAY");
			lua_setglobal(L, "arg1");
			lua_pushstring(L, g_ObjectData->GetString("SYS_DELETE_ROLE_PASSWORD_ERROR"));
			lua_setglobal(L, "arg2");
			g_pAccountLogin->SendLoginEvent("OPEN_LOGIN_DIALOG");
		}
		break;

	default:
		g_pDebugMessage->DebugOutput( 0, "Delete role failed! reult_code(%d)", emResult );
		break;
	}
}

// --------------------------------------------------------------------------------------
void  CNetCli_Login_Child::On_CreateRoleResult( EM_CreateRoleResult emResult , int GiftItemID[5] , int GiftItemCount[5] )
{
	g_pCharacterCreate->CreateResult(emResult, GiftItemID, GiftItemCount);
	/*
	switch ( emResult )
	{
	case EM_CreateRoleResult_OK:
		g_pLoginParent->SendLoginEvent("CHARACTER_CREATE_SUCCESS");
		g_pCharacterSelect->UpdateCharacterList();
		g_pDebugMessage->DebugOutput( 1, "EM_CreateRole_OK!" );
		return;

	case EM_CreateRoleResult_RoleNameExist:
		g_pDebugMessage->DebugOutput( 0, "EM_RoleNameExist!" );
		break;

	case EM_CreateRoleResult_SystemError:
		g_pDebugMessage->DebugOutput( 0, "EM_SystemError!" );
		break;

	default:
		g_pDebugMessage->DebugOutput( 0, "Create role failed! reult_code(%d)", emResult );
		break;
	}
	g_pLoginParent->SendLoginScreen("CHARACTER_CREATE_FAILED");
	*/
}
// --------------------------------------------------------------------------------------
void CNetCli_Login_Child::On_EnterWorld ( int iDBID, SampleRoleData* pSampleRole )
{
	g_pCharacterSelect->Close();
	g_pAccountLogin->SetSelectedPlayerName(pSampleRole->RoleName.Begin());
	g_pAccountLogin->CloseServerList();
	g_pGameMain->EnterWorld(iDBID, pSampleRole->RoleName.Begin());
}
// --------------------------------------------------------------------------------------
void CNetCli_Login_Child::On_LeaveWorld ( int iDBID )
{
	g_pGameMain->LeaveWorld();
}
// --------------------------------------------------------------------------------------
void CNetCli_Login_Child::On_LoadDataEvent ( EM_LoadDataEvent emEvent, int iValue )
{
	g_pCharacterSelect->LoadCharacterEvent( emEvent, iValue );
}
// --------------------------------------------------------------------------------------
void CNetCli_Login_Child::On_UpdateParalleZone()
{
	/*
	// 補足伺服器負荷個數
	for ( vector<ServerListInfoStruct>::iterator iter = s_serverListInfo.begin(); iter != s_serverListInfo.end(); iter++ )
	{
		if ( iter->Name.compare(g_pAccountLogin->GetRealmName()) == 0 )
		{
			vector<float>& loading = iter->Loading;
			if ( loading.size() > m_ParallelZoneCount )
			{
				vector<float>::iterator loading_iter = loading.begin();
				for ( int i = 0; i < m_ParallelZoneCount; i++ )
					loading_iter++;
				
				while (loading_iter != loading.end())
					loading_iter = loading.erase(loading_iter);
			}
			else if ( loading.size() < m_ParallelZoneCount )
			{
				for ( int i = loading.size(); i < m_ParallelZoneCount; i++ )
					loading.push_back(0.0f);
			}
			break;
		}
	}
	*/

	g_pCharacterSelect->UpdateParalleZone();
}
// --------------------------------------------------------------------------------------
void CNetCli_Login_Child::ServerListConnect(const char* IP, int Port, const char* Account, const char* Password, bool HashedPassword)
{
	s_serverList->Connect((char*)IP, Port, Account, Password, HashedPassword, (CNetCli_Login_Child*)m_pThis);
}
// --------------------------------------------------------------------------------------
void CNetCli_Login_Child::ServerListProcess()
{	
	if ( s_waitConnectTime > 0.001f ) {
		s_waitConnectTime -= 0.1f;
		if ( s_waitConnectTime < 0.001f ) {

			// 戰場斷線
			if ( _InBattleGround )
			{
				_InBattleGround = false;
				_NetBattle.Logout( EM_ClientNetCloseType_LeaveBattleWorld );
			}

			// ZONE斷線
			_Net.Logout( EM_ClientNetCloseType_ServerError );

			g_pError->AddMessage(0, 0, "Client logout!!");
			s_waitConnectTime = 0.0f;

			// 強制與伺服器斷線,清除目前所在分區名稱
			// g_pAccountLogin->ClearRealmName();
			
			m_pThis->On_Logout();
		}
	}
	s_serverList->Process();
}
// --------------------------------------------------------------------------------------
void CNetCli_Login_Child::UpdateParalleZonesLoad(const char* name, int index, float load)
{
	for ( vector<ServerListInfoStruct>::iterator iter = s_serverListInfo.begin(); iter != s_serverListInfo.end(); iter++ ) {
		ServerListInfoStruct &realmInfo = *iter;
		if ( realmInfo.Name.compare(name) == 0 ) {
			if ( index < (int)realmInfo.Loading.size() ) {
				realmInfo.Loading[index] = load;

				// 更新分流資訊
				g_pCharacterSelect->UpdateParalleZone();
			}
			break;
		}
	}
}
// --------------------------------------------------------------------------------------
int CNetCli_Login_Child::GetParalleZonesLoad(const char* name, float* outLoad, int outSize)
{
	int size = 0;
	for ( vector<ServerListInfoStruct>::iterator iter = s_serverListInfo.begin(); iter != s_serverListInfo.end(); iter++ ) {
		ServerListInfoStruct &realmInfo = *iter;
		if ( realmInfo.Name.compare(name) == 0 ) {
			for ( vector<float>::iterator iterLoad = realmInfo.Loading.begin(); iterLoad != realmInfo.Loading.end(); iterLoad++ ) {
				if ( size < outSize ) {
					outLoad[size++] = *iterLoad;
				} else {
					break;
				}
			}
		}
	}
	return size;
}

// --------------------------------------------------------------------------------------
void CNetCli_Login_Child::SetVivoxLicenseTime()
{
	s_serverList->SetVivoxLicenseTime();
}

// --------------------------------------------------------------------------------------
bool CompareGroupID(const ServerListInfoStruct& _Left, const ServerListInfoStruct& _Right)
{
	return (_Left.WorldID >> 16) < (_Right.WorldID >> 16);
}

// --------------------------------------------------------------------------------------
void CNetCli_Login_Child::OnRevServerList(int LastLoginWorldID , const char* Account , const char* Country , int IPNum , int Age , int ValidTime , int VivoxLicenseTime , vector<ServerListInfoStruct>& SrvList)
{

	s_serverListInfo.clear();
	s_lastLoginWorldID = LastLoginWorldID;

	g_pAccountLogin->SetIPCountry(Country);
	g_pAccountLogin->SetServerAccount(Account);
	g_pAccountLogin->SetLastLoginWorldID(LastLoginWorldID);
	g_pAccountLogin->SetAge(Age);
	g_pAccountLogin->SetValidTime(ValidTime);
	g_pGameMain->SetServerInfo(Country, IPNum);

	g_pAccountLogin->SetVivoxLicenseTime(VivoxLicenseTime);

	//sort(SrvList.begin(), SrvList.end());
	//stable_sort(SrvList.begin(), SrvList.end(), CompareGroupID);
	for ( vector<ServerListInfoStruct>::iterator iter = SrvList.begin(); iter != SrvList.end(); iter++ )
	{
		/*
		iter->Name = g_ObjectData->GetSpecialString(iter->Name.c_str());
		if ( LastLoginWorldID == (*iter).WorldID && g_pGameMain->GetLastRealmState() )
		{
			if ( realmName.empty() )
			{
				g_pAccountLogin->SetRealmName((*iter).Name.c_str(), false);
			}
		}
		*/
		s_serverListInfo.push_back(*iter);
	}
	// g_pAccountLogin->UpdateRealmList();
}
// --------------------------------------------------------------------------------------
void CNetCli_Login_Child::OnConnectFailed()
{
	// 連線失敗
	//g_pAccountLogin->OnDisconnectRealm();
	g_pAccountLogin->OnConnectRealmFailed( EM_ServerListFailed_ConnectFailed );
}
// --------------------------------------------------------------------------------------
void CNetCli_Login_Child::OnConnect()
{
	// 連線成
	g_pAccountLogin->OnConnectRealm();
}
// --------------------------------------------------------------------------------------
void CNetCli_Login_Child::OnDisConnect()
{
	// 中斷連線
	g_pAccountLogin->OnDisconnectRealm();
}
// --------------------------------------------------------------------------------------
void CNetCli_Login_Child::OnError( ServerListFailed_ENUM ErrCode )
{
	// 連線失敗
	g_pAccountLogin->OnConnectRealmFailed(ErrCode);
}
// --------------------------------------------------------------------------------------
void CNetCli_Login_Child::OnErrorData( ServerListFailed_ENUM ErrCode, void* Data, unsigned long Size)
{
	g_pAccountLogin->OnConnectRealmFailedData(ErrCode, Data, Size);
}

void CNetCli_Login_Child::DelRoleRecoverResult( int DBID , bool Result )
{
	//回復角色
	g_pCharacterSelect->UpdateCharacterList();
	//g_pCharacterSelect->UpdateSprite();
}

void CNetCli_Login_Child::UpdateSamepleRoleData()
{
	// 更新角色資料
	g_pCharacterSelect->UpdateCharacterList();
}

void	CNetCli_Login_Child::SetSecondPasswordRequest()
{
	// 收到設定帳號二次密碼
	g_pAccountLogin->SendSecondPassword();

}

void	CNetCli_Login_Child::ZoneLoading( int ZoneID , int MaxZonePlayerCount , int ParallelCount , int Count[100] )
{

}
// --------------------------------------------------------------------------------------
void CNetCli_Login_Child::On_ApexRecvSrvData ( int iSize, const char* pData )
{
	//CApexClient::OnApexRecvFromSrv( (const char*)pData, iSize );
}
// --------------------------------------------------------------------------------------
void CNetCli_Login_Child::On_ApexServerReady( int iServerID )
{
	m_pThis->m_iApexSrvID = iServerID;

	//  連線 APEX
	//CApexClient::SetSendFunc( ApexSendToSrv );
	//
	//int iVal = CApexClient::StartApex();

	//CNetCli_Login::ApexStartValue( iVal );
}
// --------------------------------------------------------------------------------------
void	CNetCli_Login_Child::RM_ChangeNameResult( int DBID , char* RoleName , ChangeNameResultENUM Result )
{
	switch (Result)
	{
	case EM_ChangeNameResult_OK:
		{
			// 修正角色名稱
			SampleRoleData*	roleData = GetCharacterInfoByDBID(DBID);
			if ( roleData ) {
				roleData->SysFlag.ChangeNameEnabled = false;
				roleData->RoleName = RoleName;
			}
		}
		g_pCharacterCreate->RenameResult(EM_CreateRoleResult_OK);
		break;
	case EM_ChangeNameResult_Failed:
		g_pCharacterCreate->RenameResult(EM_CreateRoleResult_SystemError);
		break;
	case EM_ChangeNameResult_NameErr:
		g_pCharacterCreate->RenameResult(EM_CreateRoleResult_RoleNameExist);
		break;
	case EM_ChangeNameResult_NameUsed:		//名稱重復
		g_pCharacterCreate->RenameResult(EM_CreateRoleResult_RoleNameExist);
		break;
	}

}
// --------------------------------------------------------------------------------------
void	CNetCli_Login_Child::OnGameRoleCount			( char RoleCount[30] , int FileSize , char* FileData )
{
	//各Server人數
	TRACE0( "OnGameRoleCount" );

	int characters[30];
	for ( int i = 0; i < 30; i++ )
		characters[i] = (unsigned char)RoleCount[i];

	g_pAccountLogin->LoadServerList(FileData, FileSize, characters);
}


// --------------------------------------------------------------------------------------
int	CNetCli_Login_Child::GetServerLoginRecount()
{
	return s_serverLoginRecount;
}

// --------------------------------------------------------------------------------------
void CNetCli_Login_Child::CloseServerList()
{
	if ( s_serverList ) s_serverList->NetClose();
}

// --------------------------------------------------------------------------------------
void CNetCli_Login_Child::RequestServerList()
{
	if ( s_serverList ) s_serverList->ServerListRequest();
}

void	CNetCli_Login_Child::On_MaxRoleCount			( int MaxRoleCount )
{
	g_pCharacterCreate->SetMaxCreateRoleCount(MaxRoleCount);
}
void	CNetCli_Login_Child::RD_ReserveRole			( ReserverRoleStruct& Info )
{
	g_pCharacterCreate->R_ReserveRole(Info);
}
//-------------------------------------------------------------------
bool CNetCli_Login_Child::Logout	( ClientNetCloseTypeENUM Type )
{
	if ( s_serverList ) s_serverList->NetClose();
	_ClearRoleData();
	return CNetGlobal::Logout( Type );
}
void CNetCli_Login_Child::RM_SecondPasswordRequest( )
{
	g_pAccountLogin->RM_SecondPasswordRequest();
}
void CNetCli_Login_Child::RM_SecondPasswordResult( bool Result )
{
	g_pAccountLogin->RM_SecondPasswordResult(Result);
}

//-------------------------------------------------------------------
void CNetCli_Login_Child::RM_SecondPasswordRequest2()
{
	g_pAccountLogin->RM_SecondPasswordRequest2();
}

//-------------------------------------------------------------------
void CNetCli_Login_Child::RM_SecondPasswordResult2( bool Result )
{
	g_pAccountLogin->RM_SecondPasswordResult2(Result);
}

//-------------------------------------------------------------------
void CNetCli_Login_Child::RM_CaptchaCheck(void* Captcha, int CaptchaSize)
{
	g_pAccountLogin->RM_CaptchaCheck(Captcha, CaptchaSize);
}

//-------------------------------------------------------------------
void CNetCli_Login_Child::RM_CaptchaCheckResult( int Result )
{
	g_pAccountLogin->RM_CaptchaCheckResult(Result);
}

//-------------------------------------------------------------------
void CNetCli_Login_Child::RM_SetGameGuard( int iEnable )
{

}
//-------------------------------------------------------------------
void CNetCli_Login_Child::RM_VivoxAccount ( const char* pszAccout, const char* pszPassword, bool bCreateResult )
{

}

void CNetCli_Login_Child::RM_VivoxInfo( const char* pszUrl, const char* pszCountryCode )
{

}

void CNetCli_Login_Child::OnResetPassword()
{
	g_pAccountLogin->SendLoginEvent("RESET_PASSWORD");
}

//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
void CNetCli_Login_Child::RL_BonusList(std::vector<LBBox>& boxes, int year, int month, int mday, int page, const wchar_t* description)
{
	if (g_pLoginBonusFrame != NULL)
	{
		g_pLoginBonusFrame->RL_BonusList(boxes, year, month, mday, page, description);
	}
}

//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
void CNetCli_Login_Child::RL_RequestBonusResult(int id, LBRequestResultENUM result)
{
	if (g_pLoginBonusFrame != NULL)
	{
		g_pLoginBonusFrame->RL_RequestBonusResult(id, result);
	}
}

//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
void CNetCli_Login_Child::RL_BonusPeriod(int maxpage, int minpage)
{
	if (g_pLoginBonusFrame != NULL)
	{
		g_pLoginBonusFrame->RL_BonusPeriod(maxpage, minpage);
	}
}