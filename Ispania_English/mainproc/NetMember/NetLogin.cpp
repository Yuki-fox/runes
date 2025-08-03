#include "../GameMain.h"
#include "../InterfaceMember/WorldFrame.h"
#include "../InterfaceMember/CharacterSelect.h"
#include "../ErrorList/errorlist.h"
#include "../mainproc/interfacemember/ChatFrame.h"

// --------------------------------------------------------------------------------------
void CGameMain::On_Login( const char* pszInfo )
{
	g_pError->AddMessage( 1, 0, "login succeed!" );
}

// --------------------------------------------------------------------------------------
void CGameMain::On_Logout()
{
	g_pError->AddMessage( 1, 0, "logout!" );
}

// --------------------------------------------------------------------------------------
void CGameMain::On_LoginFailed( SYSLogFailedENUM emLogFailed )
{
	switch ( emLogFailed )
	{
	case EM_NetLogFailed_PasswordError:
		g_pError->AddMessage( 0, 0, "密碼錯誤!" );
		break;

	case EM_NetLogFailed_ConnectFail:
		g_pError->AddMessage( 0, 0, "連線不成!" );
		break;

	case EM_NetLogFailed_ConnectError:
		g_pError->AddMessage( 0, 0, "連結位置錯誤!" );
		break;

	case EM_NetLogFailed_NotFindError:
		g_pError->AddMessage( 0, 0, "找不到帳號!" );
		break;
		
	case EM_NetLogFailed_ProxyNotExist:
		g_pError->AddMessage( 0, 0, "Proxy不存在!" );
		break;

	case EM_NetLogFailed_SwitchDisconnect:
		g_pError->AddMessage( 0, 0, "未完成註冊及與 Switch 斷線!" );
		break;

	case EM_NetLogFailed_ProxyLoginFailed:
		g_pError->AddMessage( 0, 0, "未完成註冊及與 Proxy 連線失敗!" );
		break;
			
	case EM_NetLogFailed_UnknowError:
		g_pError->AddMessage( 0, 0, "暫時未規類的錯誤!" );
		break;

	default:
		g_pError->AddMessage( 0, 0, "On_LoginFailed error code!" );
		break;
	}	
}

// --------------------------------------------------------------------------------------
void CGameMain::On_ServerConnect( int iServerID , const char* cpszServerName )
{
	int a = 0;
	SetSrvID_Local( iServerID );

	// 清除資料
	g_pCharacterSelect->ClearCharacterInfo();
	CFrame::SendEventString( _Def_UIEvent_SetGlueScreen_, GLUE_SCREEN_CHAR_SELECT, 0 );
}

// --------------------------------------------------------------------------------------
void CGameMain::On_ServerDisConnect( int iServerID )
{
	int a = 0;
    CChatFrame::SendChatMessage( "On_ServerDisConnect" , 2 );
	g_pError->AddMessage( 1, 0, "Server is DisConnect!" );
}

// --------------------------------------------------------------------------------------
void CGameMain::On_SamepleRoleData( int iFieldIndex, SampleRoleData* pSampleRole )
{
	g_pCharacterSelect->SetCharacterInfo( iFieldIndex, pSampleRole );
}

// --------------------------------------------------------------------------------------
void CGameMain::On_SamepleRoleDataEnd()
{
	CFrame::SendEventString( _Def_UIEvent_CharacterListUpdate_, 0, 0 );

#ifdef _AUTOLOGIN
	if( m_bAutoLogin == true )
		SelectRole( m_iAutoLoginRoleID );
#endif

}

// --------------------------------------------------------------------------------------
void CGameMain::On_DeleteRoleResult( EM_DeleteRoleResult emResult )
{
	switch ( emResult )
	{
	case EM_DeleteRole_OK:
		g_pError->AddMessage( 1, 0, "delete role is succeed!" );
		g_pCharacterSelect->ClearCharacterInfo();
		CNetCli_Login::RquestRoleData();		
		break;

	case EM_RoleNotExist:
		g_pError->AddMessage( 0, 0, "EM_RoleNotExist!" );
		break;

	case EM_DeleteSysErr:
		g_pError->AddMessage( 0, 0, "EM_DeleteSysErr!" );
		break;

	default:
		g_pError->AddMessage( 0, emResult, "Delete role failed! reult_code(%d)", emResult );
		break;
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::On_SelectRoleResult( EM_SelectRoleResult emResult, int iDBID )
{
	switch ( emResult )
	{
	case EM_SelectRole_OK:
		g_pWorldFrame->EnterWorld();
		g_pError->AddMessage( 1, 0, "EM_SelectRole_OK!" );
		break;

	case EM_SelectRole_No_This:
		g_pError->AddMessage( 0, 0, "EM_SelectRole_No_This!" );
		break;

	case EM_SelectRole_ZoneNotExist:
		g_pError->AddMessage( 0, 0, "EM_SelectRole_ZoneNotExist!" );
		break;

	default:
		g_pError->AddMessage( 0, emResult, "Select role failed! reult_code(%d)", emResult );
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::On_CreateRoleResult( EM_CreateRoleResult emResult )
{
	switch ( emResult )
	{
	case EM_CreateRole_OK:
		g_pCharacterSelect->ClearCharacterInfo();
		CNetCli_Login::RquestRoleData();
		CFrame::SendEventString( _Def_UIEvent_SetGlueScreen_, GLUE_SCREEN_CHAR_SELECT, 0 );
		g_pError->AddMessage( 1, 0, "EM_CreateRole_OK!" );
		break;

	case EM_RoleNameExist:
		g_pError->AddMessage( 0, 0, "EM_RoleNameExist!" );
		break;

	case EM_SystemError:
		g_pError->AddMessage( 0, 0, "EM_SystemError!" );
		break;

	default:
		g_pError->AddMessage( 0, emResult, "Create role failed! reult_code(%d)", emResult );
		break;
	}
}