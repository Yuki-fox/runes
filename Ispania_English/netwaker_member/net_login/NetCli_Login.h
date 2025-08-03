#pragma once

//#include "../NetWaker/GSrvNetWaker.h"
#include "../mainproc/NetGlobal.h"
#include "PG/PG_Login.h"


class CNetCli_Login : public CNetGlobal
{

protected:
	//-------------------------------------------------------------------
	static CNetCli_Login*	m_pThis;
	static DWORD			m_dwStep;
	static bool				_Init();
	static bool				_Release();
	static void				_ClearRoleData();
	static void				_Update(float elapsedTime);

	//-------------------------------------------------------------------
	// from Master
	static void		_PG_Login_M2C_IdentifyMaster				( int iServerID , int iSize , PVOID pData );
	static void		_PG_Login_M2C_SampleRoleData				( int iServerID , int iSize , PVOID pData );
	static void		_PG_Login_M2C_SampleRoleDataEnd				( int iServerID , int iSize , PVOID pData );
	static void		_PG_Login_M2C_CreateRoleResult				( int iServerID , int iSize , PVOID pData );
	static void		_PG_Login_M2C_DeleteRoleResult				( int iServerID , int iSize , PVOID pData );
	static void		_PG_Login_M2C_SelectRoleResult				( int iServerID , int iSize , PVOID pData );
	static void		_PG_Login_M2C_EnterWorld					( int iServerID , int iSize , PVOID pData );
	static void		_PG_Login_M2C_LeaveWorld					( int iServerID , int iSize , PVOID pData );
	static void		_PG_Login_M2C_LoginResult					( int iServerID , int iSize , PVOID pData );
	static void		_PG_Login_M2C_LoadDataEvent					( int iServerID , int iSize , PVOID pData );

	static void		_PG_Login_D2C_DelRoleRecoverResult			( int iServerID , int iSize , PVOID pData );
	static void		_PG_Login_M2C_SetSecondPasswordRequest		( int iServerID , int iSize , PVOID pData );
	static void		_PG_Login_M2C_ZoneLoading					( int iServerID , int iSize , PVOID pData );
	static void		_PG_Login_M2C_APEXTOCLIENT					( int iServerID , int iSize , PVOID pData );
	static void		_PG_Login_APEX2C_APEXSERVERID				( int iServerID , int iSize , PVOID pData );
	static void		_PG_Login_M2C_ChangeNameResult				( int iServerID , int iSize , PVOID pData );

	static void		_PG_Login_MtoC_MaxRoleCount					( int iServerID , int iSize , PVOID pData );
	static void		_PG_Login_DtoC_ReserveRole					( int iServerID , int iSize , PVOID pData );

	static void		_PG_Login_MtoC_SecondPasswordRequest		( int iServerID , int iSize , PVOID pData );
	static void		_PG_Login_MtoC_SecondPasswordResult			( int iServerID , int iSize , PVOID pData );

	static void		_PG_Login_MtoC_SetGameGuard					( int iServerID , int iSize , PVOID pData );

	static void		_PG_Login_MtoC_VivoxAccount					( int iServerID , int iSize , PVOID pData );
	static void		_PG_Login_MtoC_VivoxInfo					( int iServerID , int iSize , PVOID pData );

	static void		_PG_Login_MtoC_SecondPasswordRequest2		( int iServerID , int iSize , PVOID pData );
	static void		_PG_Login_MtoC_SecondPasswordResult2		( int iServerID , int iSize , PVOID pData );
	static void		_PG_Login_MtoC_CaptchaCheck					( int iServerID , int iSize , PVOID pData );
	static void		_PG_Login_MtoC_CaptchaCheckResult			( int iServerID , int iSize , PVOID pData );
	//-------------------------------------------------------------------
	static void		_PG_Login_LtoC_BonusList					( int iServerID , int iSize , PVOID pData );
	static void		_PG_Login_LtoC_RequestBonusResult			( int iServerID , int iSize , PVOID pData );
	static void		_PG_Login_LtoC_BonusPeriod					( int iServerID , int iSize , PVOID pData );
	//-------------------------------------------------------------------	
	static void		OnEvent_Login					( string sInfo );						// loging to switch server
	static void		OnEvent_Logout					();										// logout
	static void		OnEvent_LogFailed				( SYSLogFailedENUM emLogFailed );
	static void		OnEvent_LogFailedData			( SYSLogFailedENUM emLogFailed, void* pData, unsigned long dwSize );

	static void		OnEvent_ServerConnect			( int iServerID , string sServerName );
	static void		OnEvent_ServerDisConnect		( int iServerID );	
	static void		OnEvent_ServerDisConnect_Battle	( int iServerID );	
	
	int												m_ParallelZoneCount;
	int												m_currentParalleZone;
	map< int, SampleRoleData* >						m_mapSampleRole;	
	vector<string>									m_playerNameList;

public:
    CNetCli_Login( ){ m_dwStep = 0; m_pThis = this; m_ParallelZoneCount = 0; m_currentParalleZone = 0; };
	//-------------------------------------------------------------------
	//  Login
	//-------------------------------------------------------------------

	// 連線後, 從 Master 收到所有目前角色資料( Sample )

	static int	GetNumParalleZones					()				{ return m_pThis->m_ParallelZoneCount; }
	static void SetNumParalleZones					(int parallelZones);
	static int	GetNumCharacters					()				{ return (int)m_pThis->m_mapSampleRole.size(); }
	static SampleRoleData*	GetCharacterInfo		(int serial);
	static SampleRoleData*	GetCharacterInfoByDBID	(int DBID);
	static void	DeleteCharacter						(int serial, const char* password);
	static void RecoverCharacter					(int serial);
	static void	EnterWorld							(int serial , int ParallelZoneID );
	static vector<string>& GetPlayerNameList		()				{ return m_pThis->m_playerNameList; }

	static bool Login								( const char* szIP, int iPort, char* szAccount, char* szPassword, const char* pszMacAddress, bool HashedPassword );	
		
		virtual void On_Login						( const char* pszInfo ) =0;
		virtual void On_Logout						() =0;
		virtual void On_LoginFailed					( SYSLogFailedENUM emLogFailed ) =0;	
		virtual void On_LoginFailedData				( SYSLogFailedENUM emLogFailed, void* pData, unsigned long dwSize ) =0;
	
//	static bool	Logout								( ClientNetCloseTypeENUM Type );

		virtual void On_ServerConnect					( int iServerID , const char* cpszServerName ){};
		virtual void On_ServerDisConnect				( int iServerID ){};
		virtual void On_ServerDisConnect_Battle			( int iServerID ) = 0;


	static void LoadRoleData						( const char* pszAccount );

		virtual void On_LoadRoleDataBegin				() = 0;
		virtual void On_LoadDataEvent					( EM_LoadDataEvent emEvent, int iValue ) = 0;
		virtual void On_SamepleRoleData					( int iFieldIndex, SampleRoleData* pSampleRole ) =0; // iFieldIndex = 流水號
		virtual void On_SamepleRoleDataEnd				() = 0;
		
		//static void CreateRole						( int iFieldIndex, CreateRoleData* pCreateRoleData );
		static void CreateRole						( CreateRoleData* pCreateRoleData );
		static void SelectRole						( int iFieldIndex , int ParallelZoneID );
		static void SelectRole_EnterZone1			( int iFieldIndex , int ParallelZoneID );
		static void DeleteRole						( int iFieldIndex , char* Password );

	// 向 Master 要求某動作後的回傳結果

	virtual void On_DeleteRoleResult				( EM_DeleteRoleResult emResult ) =0;
	virtual void On_CreateRoleResult				( EM_CreateRoleResult emResult , int GiftItemID[5] , int GiftItemCount[5] ) =0;
	virtual void On_SelectRoleResult				( EM_SelectRoleResult emResult, int iDBID ) =0;

	// 向 Master 要求重發所有角色資料
	static void RquestRoleData						();

	// Login Event
	virtual void On_EnterWorld						( int iDBID, SampleRoleData* pSampleRole ) = 0;
	virtual	void On_LeaveWorld						( int iDBID ) = 0;

	virtual	void On_UpdateParalleZone				() = 0;	

	//回復角色
	static void DelRoleRecoverRequest				( int DBID );
		virtual void DelRoleRecoverResult			( int DBID , bool Result ) = 0;

	// 要求更新角色
	virtual void	UpdateSamepleRoleData() = 0;

	virtual void	SetSecondPasswordRequest() = 0;
		static void SetSecondPassword( char* Password );


	//保留
	static void ZoneLoadingRequest( int ZoneID );
		virtual void ZoneLoading( int ZoneID , int MaxZonePlayerCount , int ParallelCount , int Count[100] ) = 0;
	//
	//---------------------------------------------------------------------------------------
	//更名要求
	static void SM_ChangeNameRequest( int DBID , const char* RoleName );
		virtual void RM_ChangeNameResult( int DBID , char* RoleName , ChangeNameResultENUM Result ) = 0;


	//////////////////////////////////////////////////////////////////////////
	//要求二次密碼
	virtual void RM_SecondPasswordRequest( ) = 0;
		static void SM_SecondPassword( const char* Password );
			virtual void RM_SecondPasswordResult( bool Result ) = 0;

	//要求二次密碼 - 包含驗證碼
	virtual void RM_SecondPasswordRequest2() = 0;
	static	void SM_SecondPassword2( const char* Password);
	virtual void RM_SecondPasswordResult2( bool Result ) = 0;
	virtual void RM_CaptchaCheck(void* Captcha, int CaptchaSize) = 0;
	static	void SM_CaptchaRefresh();
	static	void SM_CaptchaReply( const char* Captcha );
	virtual void RM_CaptchaCheckResult( int Result ) = 0;
	//////////////////////////////////////////////////////////////////////////
	virtual void RM_SetGameGuard( int iEnable ) = 0;

	// APEX
	virtual void	On_ApexServerReady			( int iServerID ) = 0;
	virtual void	On_ApexRecvSrvData			( int iSize, const char* pData ) = 0;
	static void		ApexSendToSrv				( int iSize, const char* pData );

	static	void	ApexStartValue				( int iValue );

	virtual void	On_MaxRoleCount				( int MaxRoleCount ) = 0;

	virtual void	RD_ReserveRole				( ReserverRoleStruct& Info ) = 0;


	static	void	SM_GameGuardReport			( int iType, int iSize, const char* pszReport );

	//-------------------------------------------------------------------------------------------
	virtual	void	RM_VivoxAccount				( const char* pszAccout, const char* pszPassword, bool bCreateResult ) = 0;
	virtual	void	RM_VivoxInfo				( const char* pszUrl, const char* pszCountryCode ) = 0;
	static	void	SM_CreateVivoxAccount		( const char* pszAccout );
	//-------------------------------------------------------------------------------------------
	static void SL_RequestBonusList(int page);
	static void SL_RequestBonus(int id);

	virtual void RL_BonusList(std::vector<LBBox>& boxes, int year, int month, int mday, int page, const wchar_t* description) = 0;
	virtual void RL_RequestBonusResult(int id, LBRequestResultENUM result) = 0;
	virtual void RL_BonusPeriod(int maxpage, int minpage) = 0;
	//-------------------------------------------------------------------------------------------
	static char		m_szAccount[_DEF_MAX_ACCOUNTNAME_SIZE_];
	static int		m_iApexSrvID;
	static	bool	s_enterZone1;

#ifdef _AUTOLOGIN
	static bool		m_bAutoLogin;
	static char		m_szAutoLogingID[_DEF_MAX_ACCOUNTNAME_SIZE_];
	static char		m_szAutoLogingPW[_DEF_MAX_PASSWORD_SIZE_];
	static int		m_iAutoLoginRoleID;
	static bool		m_lockRecoverDelete;
	static bool		m_lockEnterWorld;
#endif
};
