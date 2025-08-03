#pragma once

#include "NetCli_Login.h"	



class CNetCli_Login_Child : public CNetCli_Login, public ServerListSCliListener
{
protected:
	static	ServerListCliClass*						s_serverList;

public:	
	static	vector<ServerListInfoStruct>			s_serverListInfo;	
	static	bool									s_login;	
	static	int										s_serverLoginRecount;
	static	int										s_lastLoginWorldID;

public:
	static bool				_Init					();
	static bool				_Release				();
	static int				GetServerLoginRecount	();
	static void				CloseServerList			();
	static void				RequestServerList		();
	static bool				Logout					( ClientNetCloseTypeENUM Type );

	virtual	void			On_Login				( const char* pszInfo );
	virtual	void			On_Logout				();
	virtual	void			On_LoginFailed			( SYSLogFailedENUM emLogFailed );
	virtual void			On_LoginFailedData		( SYSLogFailedENUM emLogFailed, void* pData, unsigned long dwSize );
	virtual	void			On_ServerConnect		( int iServerID , const char* cpszServerName );
	virtual	void			On_ServerDisConnect		( int iServerID );
	virtual void			On_ServerDisConnect_Battle			( int iServerID );
	virtual void			On_LoadRoleDataBegin	();
	virtual void			On_SamepleRoleData		( int iFieldIndex, SampleRoleData* pSampleRole );
	virtual	void			On_SamepleRoleDataEnd	();
	virtual	void			On_SelectRoleResult		( EM_SelectRoleResult emResult, int iDBID );	
	virtual	void			On_DeleteRoleResult		( EM_DeleteRoleResult emResult );
	virtual	void			On_CreateRoleResult		( EM_CreateRoleResult emResult , int GiftItemID[5] , int GiftItemCount[5] );

	virtual void			On_LoadDataEvent		( EM_LoadDataEvent emEvent, int iValue );

	virtual void			On_EnterWorld			( int iDBID, SampleRoleData* pSampleRole );
	virtual	void			On_LeaveWorld			( int iDBID );

	virtual	void			On_UpdateParalleZone	();
	virtual void			On_ApexServerReady		( int iServerID );
	virtual void			OnGameRoleCount			( char RoleCount[30] , int FileSize , char* FileData );	

	// ServerListSCliListener
	static	void	ServerListConnect(const char* IP, int Port, const char* Account, const char* Password, bool HashedPassword);
	static	void	ServerListProcess();
	static	void	UpdateParalleZonesLoad(const char* name, int index, float load);
	static	int		GetParalleZonesLoad(const char* name, float* outLoad, int outSize);
	static	void	SetVivoxLicenseTime();

	virtual void	OnRevServerList(int LastLoginWorldID , const char* Account , const char* Country , int IPNum , int Age , int ValidTime , int VivoxLicenseTime , vector<ServerListInfoStruct>& SrvList);
	virtual void	OnConnectFailed();
	virtual void	OnConnect();
	virtual void	OnDisConnect();
	virtual void	OnError(ServerListFailed_ENUM ErrCode);
	virtual void	OnErrorData( ServerListFailed_ENUM ErrCode, void* Data, unsigned long Size);
	virtual void	DelRoleRecoverResult( int DBID , bool Result );

	virtual void	UpdateSamepleRoleData();
	virtual void	SetSecondPasswordRequest();

	virtual void	ZoneLoading( int ZoneID , int MaxZonePlayerCount , int ParallelCount , int Count[100] );


	virtual void	On_ApexRecvSrvData		( int iSize, const char* pData );

	virtual void	RM_ChangeNameResult		( int DBID , char* RoleName , ChangeNameResultENUM Result );
	virtual void	On_MaxRoleCount			( int MaxRoleCount );
	virtual void	RD_ReserveRole			( ReserverRoleStruct& Info );
	//////////////////////////////////////////////////////////////////////////
	//要求二次密碼
	virtual void	RM_SecondPasswordRequest( );
	virtual void	RM_SecondPasswordResult( bool Result );

	//要求二次密碼 - 包含驗證碼
	virtual void	RM_SecondPasswordRequest2();
	virtual void	RM_SecondPasswordResult2( bool Result );
	virtual void	RM_CaptchaCheck(void* Captcha, int CaptchaSize);
	virtual void	RM_CaptchaCheckResult( int Result );
	//////////////////////////////////////////////////////////////////////////
	virtual void	RM_SetGameGuard( int iEnable );

	virtual	void	RM_VivoxAccount				( const char* pszAccout, const char* pszPassword, bool bCreateResult );
	virtual	void	RM_VivoxInfo				( const char* pszUrl, const char* pszCountryCode );

	virtual void	OnResetPassword();
	//////////////////////////////////////////////////////////////////////////
	virtual void RL_BonusList(std::vector<LBBox>& boxes, int year, int month, int mday, int page, const wchar_t* description);
	virtual void RL_RequestBonusResult(int id, LBRequestResultENUM result);
	virtual void RL_BonusPeriod(int maxpage, int minpage);
};