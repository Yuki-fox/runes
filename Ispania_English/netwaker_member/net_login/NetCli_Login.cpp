#include "NetCli_login.h"
//#include "../../Tools/ApexClient/RaApexClient.h"
//-------------------------------------------------------------------
CNetCli_Login*		CNetCli_Login::m_pThis		= NULL;
DWORD				CNetCli_Login::m_dwStep		= 0;
char				CNetCli_Login::m_szAccount[_DEF_MAX_ACCOUNTNAME_SIZE_];
bool				CNetCli_Login::m_lockEnterWorld = false;
bool				CNetCli_Login::m_lockRecoverDelete = false;
int					CNetCli_Login::m_iApexSrvID			= -1;
bool				CNetCli_Login::s_enterZone1 = false;


#ifdef _AUTOLOGIN
	bool		CNetCli_Login::m_bAutoLogin;
	char		CNetCli_Login::m_szAutoLogingID[_DEF_MAX_ACCOUNTNAME_SIZE_];
	char		CNetCli_Login::m_szAutoLogingPW[_DEF_MAX_PASSWORD_SIZE_];
	int			CNetCli_Login::m_iAutoLoginRoleID;
#endif

//-------------------------------------------------------------------
bool CNetCli_Login::_Init()
{
	//_Net.RegOnLoginFunction				( OnEvent_Login				); ¥Ñ Master ¨ÓºÊ±±
	_Net.RegOnLogoutFunction			( OnEvent_Logout			);
	_Net.RegOnLoginFailFunction			( OnEvent_LogFailed			);
	_Net.RegOnLoginFailDataFunction		( OnEvent_LogFailedData		);

	_Net.RegOnSrvConnectFunction		( OnEvent_ServerConnect		);
	_Net.RegOnSrvDisConnectFunction		( OnEvent_ServerDisConnect	);


	_NetBattle.RegOnSrvDisConnectFunction( OnEvent_ServerDisConnect_Battle	);

	

/*
	_Net.RegOnSrvPacketFunction			( EM_PG_Login_M2C_IdentifyMaster			, _PG_Login_M2C_IdentifyMaster			);
	_Net.RegOnSrvPacketFunction			( EM_PG_Login_M2C_SampleRoleData			, _PG_Login_M2C_SampleRoleData			);
	_Net.RegOnSrvPacketFunction			( EM_PG_Login_M2C_SampleRoleDataEnd			, _PG_Login_M2C_SampleRoleDataEnd		);
	_Net.RegOnSrvPacketFunction			( EM_PG_Login_M2C_CreateRoleResult			, _PG_Login_M2C_CreateRoleResult		);
	_Net.RegOnSrvPacketFunction			( EM_PG_Login_M2C_DeleteRoleResult			, _PG_Login_M2C_DeleteRoleResult		);
	_Net.RegOnSrvPacketFunction			( EM_PG_Login_M2C_SelectRoleResult			, _PG_Login_M2C_SelectRoleResult		);
	_Net.RegOnSrvPacketFunction			( EM_PG_Login_M2C_EnterWorld				, _PG_Login_M2C_EnterWorld				);
	_Net.RegOnSrvPacketFunction			( EM_PG_Login_M2C_LeaveWorld				, _PG_Login_M2C_LeaveWorld				);
	_Net.RegOnSrvPacketFunction			( EM_PG_Login_M2C_LoginResult				, _PG_Login_M2C_LoginResult				);
	_Net.RegOnSrvPacketFunction			( EM_PG_Login_M2C_LoadDataEvent				, _PG_Login_M2C_LoadDataEvent			);
	_Net.RegOnSrvPacketFunction			( EM_PG_Login_D2C_DelRoleRecoverResult		, _PG_Login_D2C_DelRoleRecoverResult	);
	_Net.RegOnSrvPacketFunction			( EM_PG_Login_M2C_SetSecondPasswordRequest	, _PG_Login_M2C_SetSecondPasswordRequest);	
	_Net.RegOnSrvPacketFunction			( EM_PG_Login_M2C_ZoneLoading				, _PG_Login_M2C_ZoneLoading				);
	_Net.RegOnSrvPacketFunction			( EM_PG_Login_M2C_APEXTOCLIENT				, _PG_Login_M2C_APEXTOCLIENT			);
	_Net.RegOnSrvPacketFunction			( EM_PG_Login_APEX2C_APEXSERVERID			, _PG_Login_APEX2C_APEXSERVERID			);
	_Net.RegOnSrvPacketFunction			( EM_PG_Login_M2C_ChangeNameResult			, _PG_Login_M2C_ChangeNameResult		);
	*/

	Cli_NetReg( PG_Login_M2C_IdentifyMaster				);
	Cli_NetReg( PG_Login_M2C_SampleRoleData				);
	Cli_NetReg( PG_Login_M2C_SampleRoleDataEnd			);
	Cli_NetReg( PG_Login_M2C_CreateRoleResult			);
	Cli_NetReg( PG_Login_M2C_DeleteRoleResult			);
	Cli_NetReg( PG_Login_M2C_SelectRoleResult			);
	Cli_NetReg( PG_Login_M2C_EnterWorld					);
	Cli_NetReg( PG_Login_M2C_LeaveWorld					);
	Cli_NetReg( PG_Login_M2C_LoginResult				);
	Cli_NetReg( PG_Login_M2C_LoadDataEvent				);
	Cli_NetReg( PG_Login_D2C_DelRoleRecoverResult		);
	Cli_NetReg( PG_Login_M2C_SetSecondPasswordRequest	);
	Cli_NetReg( PG_Login_M2C_ZoneLoading				);
	Cli_NetReg( PG_Login_M2C_APEXTOCLIENT				);
	Cli_NetReg( PG_Login_APEX2C_APEXSERVERID			);
	Cli_NetReg( PG_Login_M2C_ChangeNameResult			);
	Cli_NetReg( PG_Login_MtoC_MaxRoleCount				);
	Cli_NetReg( PG_Login_DtoC_ReserveRole				);
	Cli_NetReg( PG_Login_MtoC_SecondPasswordRequest		);
	Cli_NetReg( PG_Login_MtoC_SecondPasswordResult		);

	Cli_NetReg( PG_Login_MtoC_SetGameGuard				);
	Cli_NetReg( PG_Login_MtoC_VivoxAccount				);
	Cli_NetReg( PG_Login_MtoC_VivoxInfo					);

	Cli_NetReg( PG_Login_MtoC_SecondPasswordRequest2	);
	Cli_NetReg( PG_Login_MtoC_SecondPasswordResult2		);
	Cli_NetReg( PG_Login_MtoC_CaptchaCheck				);
	Cli_NetReg( PG_Login_MtoC_CaptchaCheckResult		);

	Cli_NetReg( PG_Login_LtoC_BonusList					);
	Cli_NetReg( PG_Login_LtoC_RequestBonusResult		);
	Cli_NetReg( PG_Login_LtoC_BonusPeriod				);

	return true;
}

//-------------------------------------------------------------------
void CNetCli_Login::_PG_Login_MtoC_SecondPasswordRequest2( int iServerID , int iSize , PVOID pData )
{
	PG_Login_MtoC_SecondPasswordRequest2 *PG = (PG_Login_MtoC_SecondPasswordRequest2*)pData;
	m_pThis->RM_SecondPasswordRequest2();
}

//-------------------------------------------------------------------
void CNetCli_Login::_PG_Login_MtoC_SecondPasswordResult2( int iServerID , int iSize , PVOID pData )
{
	PG_Login_MtoC_SecondPasswordResult2 *PG = (PG_Login_MtoC_SecondPasswordResult2*)pData;
	m_pThis->RM_SecondPasswordResult2( PG->Result );
}

//-------------------------------------------------------------------
void CNetCli_Login::_PG_Login_MtoC_CaptchaCheck( int iServerID , int iSize , PVOID pData )
{
	PG_Login_MtoC_CaptchaCheck *PG = (PG_Login_MtoC_CaptchaCheck*)pData;

	PBYTE pTravaler = PBYTE(pData);
	pTravaler += sizeof(PG_Login_MtoC_CaptchaCheck);

	m_pThis->RM_CaptchaCheck(pTravaler, PG->CaptchaSize);
}

//-------------------------------------------------------------------
void CNetCli_Login::_PG_Login_MtoC_CaptchaCheckResult( int iServerID , int iSize , PVOID pData )
{
	PG_Login_MtoC_CaptchaCheckResult *PG = (PG_Login_MtoC_CaptchaCheckResult*)pData;
	m_pThis->RM_CaptchaCheckResult( PG->Result );
}

//-------------------------------------------------------------------
void CNetCli_Login::_PG_Login_LtoC_BonusList( int iServerID , int iSize , PVOID pData )
{
	PG_Login_LtoC_BonusList* pg = (PG_Login_LtoC_BonusList*)pData;

	PBYTE pTraveler = (PBYTE)pg;
	pTraveler += sizeof(PG_Login_LtoC_BonusList);

	LBBox Box;
	std::vector<LBBox> Boxes;
	std::wstring Description = (wchar_t*)pTraveler;
	pTraveler += pg->DescriptionSize;

	for (int i = 0; i < pg->BoxCount; ++i)
	{
		Box.Init();

		PG_LBBox* pBox = (PG_LBBox*)pTraveler;
		Box.ID		  = pBox->ID;
		Box.ToWho	  = pBox->ToWho;
		Box.StartDate = pBox->StartDate;
		Box.EndDate	  = (float)pBox->EndDate;
		Box.Money	  = pBox->Money;
		Box.Status	  = pBox->Status;
		pTraveler += sizeof(PG_LBBox);

		for (int j = 0; j < pBox->GoodsCount; ++j)
		{
			LBGoods* pGoods = (LBGoods*)pTraveler;
			Box.Goods.push_back(*pGoods);
			pTraveler += sizeof(LBGoods);
		}

		for (int j = 0; j < pBox->ConstraintCount; ++j)
		{
			LBConstraint* pConstraint = (LBConstraint*)pTraveler;
			Box.Constraints.push_back(*pConstraint);
			pTraveler += sizeof(LBConstraint);
		}

		Boxes.push_back(Box);
	}

	m_pThis->RL_BonusList(Boxes, pg->Year, pg->Month, pg->MDay, pg->Page, Description.c_str());
}

//-------------------------------------------------------------------
void CNetCli_Login::_PG_Login_LtoC_RequestBonusResult( int iServerID , int iSize , PVOID pData )
{
	PG_Login_LtoC_RequestBonusResult* pg = (PG_Login_LtoC_RequestBonusResult*)pData;
	m_pThis->RL_RequestBonusResult(pg->ID, pg->Result);
}
//-------------------------------------------------------------------
void CNetCli_Login::_PG_Login_LtoC_BonusPeriod( int iServerID , int iSize , PVOID pData )
{
	PG_Login_LtoC_BonusPeriod* pg = (PG_Login_LtoC_BonusPeriod*)pData;
	m_pThis->RL_BonusPeriod(pg->MaxPage, pg->MinPage);
}
//-------------------------------------------------------------------
void CNetCli_Login::_PG_Login_MtoC_VivoxInfo( int iServerID , int iSize , PVOID pData )
{	
	PG_Login_MtoC_VivoxInfo *PG = (PG_Login_MtoC_VivoxInfo*)pData;
	m_pThis->RM_VivoxInfo( PG->Url.Begin(), PG->CountryCode.Begin() );
}
//-------------------------------------------------------------------
void CNetCli_Login::_PG_Login_MtoC_VivoxAccount( int iServerID , int iSize , PVOID pData )
{	
	PG_Login_MtoC_VivoxAccount *PG = (PG_Login_MtoC_VivoxAccount*)pData;
	m_pThis->RM_VivoxAccount( PG->Account.Begin(), PG->Password.Begin(), PG->bCreateResult );
}
void CNetCli_Login::_PG_Login_MtoC_SetGameGuard( int iServerID , int iSize , PVOID pData )
{
	PG_Login_MtoC_SetGameGuard *PG = (PG_Login_MtoC_SetGameGuard*)pData;
	m_pThis->RM_SetGameGuard( PG->iEnable );
}

void CNetCli_Login::_PG_Login_MtoC_SecondPasswordRequest( int iServerID , int iSize , PVOID pData )
{
	PG_Login_MtoC_SecondPasswordRequest *PG = (PG_Login_MtoC_SecondPasswordRequest*)pData;
	m_pThis->RM_SecondPasswordRequest( );
}
void CNetCli_Login::_PG_Login_MtoC_SecondPasswordResult	( int iServerID , int iSize , PVOID pData )
{
	PG_Login_MtoC_SecondPasswordResult *PG = (PG_Login_MtoC_SecondPasswordResult*)pData;
	m_pThis->RM_SecondPasswordResult( PG->Result );
}

void CNetCli_Login::_PG_Login_DtoC_ReserveRole		( int iServerID , int iSize , PVOID pData )
{
	PG_Login_DtoC_ReserveRole *PG = (PG_Login_DtoC_ReserveRole*)pData;

	m_pThis->RD_ReserveRole( PG->Info );
}
//-------------------------------------------------------------------
void CNetCli_Login::_PG_Login_MtoC_MaxRoleCount		( int iServerID , int iSize , PVOID pData )
{
	PG_Login_MtoC_MaxRoleCount *PG = (PG_Login_MtoC_MaxRoleCount*)pData;

	m_pThis->On_MaxRoleCount( PG->Count );
}
//-------------------------------------------------------------------
void CNetCli_Login::_PG_Login_M2C_ChangeNameResult	( int iServerID , int iSize , PVOID pData )
{
	PG_Login_M2C_ChangeNameResult *PG = (PG_Login_M2C_ChangeNameResult*)pData;

	m_pThis->RM_ChangeNameResult( PG->DBID , PG->RoleName , PG->Result );
}
//-------------------------------------------------------------------
void CNetCli_Login::_PG_Login_APEX2C_APEXSERVERID( int iServerID , int iSize , PVOID pData )
{
	PG_Login_APEX2C_APEXSERVERID *PG = (PG_Login_APEX2C_APEXSERVERID*)pData;

	m_pThis->On_ApexServerReady( iServerID );
}
//-------------------------------------------------------------------
void CNetCli_Login::_PG_Login_M2C_ZoneLoading( int iServerID , int iSize , PVOID pData )
{
	PG_Login_M2C_ZoneLoading *PG = (PG_Login_M2C_ZoneLoading*)pData;
	m_pThis->ZoneLoading( PG->ZoneID , PG->MaxZoneCount , PG->ParallelCount , PG->Count );
}

void CNetCli_Login::_PG_Login_M2C_SetSecondPasswordRequest( int iServerID , int iSize , PVOID pData )
{
	PG_Login_M2C_SetSecondPasswordRequest *PG = (PG_Login_M2C_SetSecondPasswordRequest*)pData;
	m_pThis->SetSecondPasswordRequest();
}
void CNetCli_Login::_PG_Login_D2C_DelRoleRecoverResult( int iServerID , int iSize , PVOID pData )
{
	PG_Login_D2C_DelRoleRecoverResult *PG = (PG_Login_D2C_DelRoleRecoverResult*)pData;

	m_lockRecoverDelete = false;
	if ( PG->Result ) {
		map< int, SampleRoleData*>::iterator itRole = m_pThis->m_mapSampleRole.find( PG->DBID );
		if( itRole != m_pThis->m_mapSampleRole.end() )
		{
			(itRole->second)->DestroyTime = 0;
		}
	}
	m_pThis->DelRoleRecoverResult( PG->DBID , PG->Result );
}

void CNetCli_Login::_ClearRoleData()
{
	if ( m_pThis == NULL )
		return;

	// §R°£©Ò¦³§@·~¥Îªº¼È®É¸ê®Æ
	map< int , SampleRoleData* >::iterator it;
	SampleRoleData* pSampleRole = NULL;	

	m_pThis->m_playerNameList.clear();
	for( it = m_pThis->m_mapSampleRole.begin(); it != m_pThis->m_mapSampleRole.end(); it++ )
	{
		pSampleRole = it->second;
		m_pThis->m_playerNameList.push_back(pSampleRole->RoleName.Begin());				// °O¿ý¨ä¥L¦s¦b¨¤¦â
		delete pSampleRole;
	}

	m_pThis->m_mapSampleRole.clear();
}
//-------------------------------------------------------------------
void CNetCli_Login::_Update(float elapsedTime)
{
	static float lastTime = 0.0f;

	lastTime += elapsedTime;
	if ( lastTime < 1.0f )
		return;
	else if ( lastTime > 5.0f )
		lastTime = 5.0f;

	lastTime -= 1.0f;
	map<int, SampleRoleData* >::iterator it = m_pThis->m_mapSampleRole.begin();

	bool update = false;
	while ( it != m_pThis->m_mapSampleRole.end() ) {
		SampleRoleData* pSampleRole = it->second;

		if ( pSampleRole->DestroyTime <= 0 ) {
			it++;
		} else {
			pSampleRole->DestroyTime--;
			if ( pSampleRole->DestroyTime == 0 ) {
				it = m_pThis->m_mapSampleRole.erase( it );
				delete pSampleRole;
			} else {
				it++;
			}
		}
	}
}
//-------------------------------------------------------------------
void CNetCli_Login::OnEvent_Login		( string sInfo )
{
	m_pThis->On_Login( sInfo.c_str() );
}
//-------------------------------------------------------------------
void CNetCli_Login::OnEvent_Logout		()
{
    //////////////////////////////////////////////////////////////////////////
    // TODO: §ïÅÜ¸Ñºc¶¶§Ç, ¨ÏÂ_½uªº¨BÆJ¤£·|µo¥Í¿ù»~.
    //////////////////////////////////////////////////////////////////////////

    if (m_pThis) // ¦w¥þ¨Ï¥Î«ü¼Ð, ¼È®ÉÁ×¶}¸Ñºc®Éªº¶¶§Ç°ÝÃD.
    {
	    m_pThis->On_Logout();
    }
    else
    {
        char szDebug[256];
        szDebug[255] = '\0';
        _snprintf(szDebug, 255, "CNetCli_Login::OnEvent_Logout() Error : m_pThis == NULL !\n");
        OutputDebugString(szDebug);
    }
}
//-------------------------------------------------------------------
void CNetCli_Login::OnEvent_LogFailed	( SYSLogFailedENUM emLogFailed )
{
	m_pThis->On_LoginFailed( emLogFailed );
}

//-------------------------------------------------------------------
void CNetCli_Login::OnEvent_LogFailedData( SYSLogFailedENUM emLogFailed, void* pData, unsigned long dwSize )
{
	m_pThis->On_LoginFailedData(emLogFailed, pData, dwSize);
}

//-------------------------------------------------------------------
void CNetCli_Login::ApexSendToSrv( int iSize, const char* pData )
{
	int		iPacketSize =	sizeof(int) * 2 + iSize;
	BYTE*	pPacket		=	new BYTE[ iPacketSize ];
	BYTE*	ptr			=	pPacket;

	*((DWORD*)ptr)		=	EM_PG_Login_C2M_APEXTOSERVER;
	ptr					=	ptr + sizeof( DWORD );

	*((DWORD*)ptr)		=	iSize;
	ptr					=	ptr + sizeof( DWORD );

	memcpy( ptr, pData, iSize );

	//_Net->SendToCli( CliNetID, iPacketSize, pData );

	if( m_pThis->m_iApexSrvID != -1 )
	{
		_Net.SendToSrv( m_pThis->m_iApexSrvID, iPacketSize, (PVOID)pPacket );
	}

	delete [] pPacket;
}
//-------------------------------------------------------------------
void CNetCli_Login::_PG_Login_M2C_APEXTOCLIENT( int iServerID , int iSize , PVOID pData )
{
	BYTE*	ptr			= (BYTE*)pData;	
	ptr	+= sizeof(int);

	int		iDataSize	= *((int*)ptr);
	ptr	+= sizeof(int);

	BYTE*	pApexData	= ptr;

	m_pThis->On_ApexRecvSrvData( iDataSize, (const char*)pApexData );
}

//-------------------------------------------------------------------
void CNetCli_Login::OnEvent_ServerConnect	( int iServerID , string sServerName )
{
	if( m_dwStep == 0 )
	{
		m_dwStep++; // ³s½u¨ì Switch ¦¨¥
	}

	m_pThis->On_ServerConnect( iServerID, sServerName.c_str() );
}
//-------------------------------------------------------------------
void CNetCli_Login::OnEvent_ServerDisConnect_Battle	( int iServerID )
{
	m_pThis->On_ServerDisConnect_Battle( iServerID );
}
void CNetCli_Login::OnEvent_ServerDisConnect	( int iServerID )
{
	if( m_dwStep == 1 )
	{
		m_dwStep++; // ²æÂ÷ Switch
	}

    //////////////////////////////////////////////////////////////////////////
    // TODO: §ïÅÜ¸Ñºc¶¶§Ç, ¨ÏÂ_½uªº¨BÆJ¤£·|µo¥Í¿ù»~.
    //////////////////////////////////////////////////////////////////////////

    if (m_pThis) // ¦w¥þ¨Ï¥Î«ü¼Ð, ¼È®ÉÁ×¶}¸Ñºc®Éªº¶¶§Ç°ÝÃD.
    {
        m_pThis->On_ServerDisConnect( iServerID );
    }
    else
    {
        char szDebug[256];
        szDebug[255] = '\0';
        _snprintf(szDebug, 255, "CNetCli_Login::OnEvent_ServerDisConnect(%d) Error : m_pThis == NULL !\n", iServerID);
        OutputDebugString(szDebug);
    }
}
//-------------------------------------------------------------------
bool CNetCli_Login::_Release()				{ return true;				}
//-------------------------------------------------------------------
void CNetCli_Login::_PG_Login_M2C_IdentifyMaster			( int iServerID , int iSize , PVOID pData )
{
	PG_Login_M2C_IdentifyMaster* ptIdentifyMaster = (PG_Login_M2C_IdentifyMaster*)pData;
	CNetGlobal::SetSrvID_Master( iServerID );
}
//-------------------------------------------------------------------
void CNetCli_Login::_PG_Login_M2C_SampleRoleData			( int iServerID , int iSize , PVOID pData )
{
	M_PACKET( PG_Login_M2C_SampleRoleData );
	
	SampleRoleData* pSampleRole = new SampleRoleData;
	*pSampleRole				= pPacket->SampleRole;
	
	m_pThis->m_mapSampleRole[ pPacket->iDBID ] = pSampleRole;
	m_pThis->On_SamepleRoleData( (int)m_pThis->m_mapSampleRole.size(), pSampleRole );
}
//-------------------------------------------------------------------
void CNetCli_Login::_PG_Login_M2C_SampleRoleDataEnd		( int iServerID , int iSize , PVOID pData )
{
	PG_Login_M2C_SampleRoleDataEnd* ptSampleRoleDatEnd = (PG_Login_M2C_SampleRoleDataEnd*)pData;
	SetNumParalleZones(ptSampleRoleDatEnd->ParallelZoneCount);
	m_pThis->On_SamepleRoleDataEnd();
}
//-------------------------------------------------------------------
void CNetCli_Login::_PG_Login_M2C_SelectRoleResult		( int iServerID , int iSize , PVOID pData )
{
	M_PACKET( PG_Login_M2C_SelectRoleResult );

	m_lockEnterWorld = false;

	m_pThis->On_SelectRoleResult( pPacket->emResult, pPacket->iDBID );

	if(  pPacket->emResult == EM_SelectRole_OK )
	{
		map< int, SampleRoleData* >::iterator it = m_pThis->m_mapSampleRole.find( pPacket->iDBID );

		if( it != m_pThis->m_mapSampleRole.end() )
		{
			SampleRoleData* pSampleRole = it->second;
			m_pThis->On_EnterWorld( pPacket->iDBID, pSampleRole );
		}	

		_ClearRoleData();
	}

	

}
//-------------------------------------------------------------------
void CNetCli_Login::_PG_Login_M2C_CreateRoleResult		( int iServerID , int iSize , PVOID pData )
{
	M_PACKET( PG_Login_M2C_CreateRoleResult );

	if( pPacket->emResult == EM_CreateRoleResult_OK )
	{
		SampleRoleData* pSampleRole = new SampleRoleData;
		*pSampleRole				= pPacket->SampleRole;

		m_pThis->m_mapSampleRole[ pPacket->iDBID ] = pSampleRole;
	}

	m_pThis->On_CreateRoleResult( pPacket->emResult , pPacket->GiftItemID , pPacket->GiftItemCount );
}
//-------------------------------------------------------------------
void CNetCli_Login::_PG_Login_M2C_DeleteRoleResult		( int iServerID , int iSize , PVOID pData )
{
	M_PACKET( PG_Login_M2C_DeleteRoleResult );

	if( pPacket->emResult == EM_DeleteRole_OK )
	{	
		map< int, SampleRoleData*>::iterator itRole = m_pThis->m_mapSampleRole.find( pPacket->iDBID );
		if( itRole != m_pThis->m_mapSampleRole.end() )
		{
			SampleRoleData* pSampleRole = itRole->second;

			// §R°£¨¤¦â¥u°µ®É¶¡³]©w
			pSampleRole->DestroyTime = 24 * 60 * 60;

			/*			
			m_pThis->m_mapSampleRole.erase( itRole );
			delete pSampleRole;
			*/
		}
	}

	m_pThis->On_DeleteRoleResult( pPacket->emResult );
}
//-------------------------------------------------------------------
void CNetCli_Login::_PG_Login_M2C_EnterWorld				( int iServerID , int iSize , PVOID pData )
{
	M_PACKET( PG_Login_M2C_EnterWorld );
	//m_pThis->On_EnterWorld();
}
//-------------------------------------------------------------------
void CNetCli_Login::_PG_Login_M2C_LeaveWorld				( int iServerID , int iSize , PVOID pData )
{
	M_PACKET( PG_Login_M2C_LeaveWorld );
	m_pThis->On_LeaveWorld( pPacket->iDBID );
}
//-------------------------------------------------------------------
void CNetCli_Login::_PG_Login_M2C_LoginResult				( int iServerID , int iSize , PVOID pData )
{
	M_PACKET( PG_Login_M2C_LoginResult );
	//m_pThis->On_LeaveWorld( pPacket->iDBID );
	if( pPacket->emResult == EM_LoginResult_OK )
	{
		m_pThis->On_Login( NULL );
	}
	else
	if( pPacket->emResult == EM_LoginResult_AccountExist )
	{
		m_pThis->On_LoginFailed( EM_NetLogFailed_AccountExist );
	}
}
//-------------------------------------------------------------------
void CNetCli_Login::_PG_Login_M2C_LoadDataEvent ( int iServerID , int iSize , PVOID pData )
{
	M_PACKET( PG_Login_M2C_LoadDataEvent );
	m_pThis->On_LoadDataEvent( pPacket->emEvent, pPacket->iValue );
}
//===================================================================
//===================================================================
void CNetCli_Login::CreateRole		( CreateRoleData* pCreateRoleData )
{
	PG_Login_C2M_CreateRole			Packet;

	Packet.iFieldIndex		= (int)m_pThis->m_mapSampleRole.size();
	Packet.CreateRole		= *pCreateRoleData;
	strcpy( Packet.szAccountName, m_szAccount );

	SendToMaster( sizeof( PG_Login_C2M_CreateRole ), &Packet );
}
//-------------------------------------------------------------------
void CNetCli_Login::SelectRole		( int iFieldIndex , int ParallelZoneID )
{
	PG_Login_C2M_SelectRole Packet;

	Packet.iDBID = iFieldIndex;
	Packet.ParallelZoneID = ParallelZoneID;
	strcpy( Packet.szAccountName, m_szAccount );

	SendToMaster( sizeof( PG_Login_C2M_SelectRole ), &Packet );
}
//-------------------------------------------------------------------
void CNetCli_Login::SelectRole_EnterZone1		( int iFieldIndex , int ParallelZoneID )
{
	PG_Login_C2M_SelectRole_EnterZone1 Packet;

	Packet.iDBID = iFieldIndex;
	Packet.ParallelZoneID = ParallelZoneID;
	strcpy( Packet.szAccountName, m_szAccount );

	SendToMaster( sizeof( PG_Login_C2M_SelectRole_EnterZone1 ), &Packet );
}
//-------------------------------------------------------------------
void CNetCli_Login::DeleteRole		( int iFieldIndex , char* Password )
{
	PG_Login_C2M_DeleteRole Packet;
	Packet.iDBID	= iFieldIndex;
	strcpy( Packet.szAccountName, m_szAccount );
	Packet.IsImmediately = false;
	Packet.Password = Password;
	SendToMaster( sizeof( PG_Login_C2M_DeleteRole ), &Packet );
}
//-------------------------------------------------------------------
void CNetCli_Login::SetNumParalleZones(int parallelZones)
{
	m_pThis->m_ParallelZoneCount = parallelZones;
	m_pThis->On_UpdateParalleZone();
}
//-------------------------------------------------------------------
SampleRoleData* CNetCli_Login::GetCharacterInfo(int serial)
{	
	for ( map<int, SampleRoleData*>::iterator iter = m_pThis->m_mapSampleRole.begin(); iter != m_pThis->m_mapSampleRole.end(); iter++ )
	{
		if ( serial == 1 )
			return iter->second;
		serial--;
	}
	return NULL;
}
//-------------------------------------------------------------------
SampleRoleData*	CNetCli_Login::GetCharacterInfoByDBID(int DBID)
{
	for ( map<int, SampleRoleData*>::iterator iter = m_pThis->m_mapSampleRole.begin(); iter != m_pThis->m_mapSampleRole.end(); iter++ )
	{
		if ( (iter->second)->DBID == DBID )
			return iter->second;
	}
	return NULL;
}
//-------------------------------------------------------------------
void CNetCli_Login::DeleteCharacter(int serial, const char* password)
{
	for ( map<int, SampleRoleData*>::iterator iter = m_pThis->m_mapSampleRole.begin(); iter != m_pThis->m_mapSampleRole.end(); iter++ )
	{
		if ( serial == 1 )
		{
			DeleteRole(iter->first, (char*)password);
			return;
		}
		serial--;
	}
}
//-------------------------------------------------------------------
void CNetCli_Login::RecoverCharacter(int serial)
{
	if ( m_lockRecoverDelete )
		return;

	m_lockRecoverDelete = true;
	for ( map<int, SampleRoleData*>::iterator iter = m_pThis->m_mapSampleRole.begin(); iter != m_pThis->m_mapSampleRole.end(); iter++ )
	{
		if ( serial == 1 )
		{			
			DelRoleRecoverRequest(iter->first);
			return;
		}
		serial--;
	}
}
//-------------------------------------------------------------------
void CNetCli_Login::EnterWorld(int serial , int ParallelZoneID )
{
	if ( m_lockEnterWorld )
		return;
	
	for ( map<int, SampleRoleData*>::iterator iter = m_pThis->m_mapSampleRole.begin(); iter != m_pThis->m_mapSampleRole.end(); iter++ )
	{
		if ( serial == 1 )
		{
			if ( s_enterZone1 )
			{
				s_enterZone1 = false;
				SelectRole_EnterZone1(iter->first , ParallelZoneID );
			}
			else
			{
				SelectRole(iter->first , ParallelZoneID );
			}
			
			m_lockEnterWorld = true;
			return;
		}
		serial--;
	}
}
//-------------------------------------------------------------------
bool CNetCli_Login::Login	( const char* szIP, int iPort, char* szAccount, char* szPassword, const char* pszMacAddress, bool HashedPassword )
{
	char szTempIP[255];
	strcpy( szTempIP, szIP );
	strcpy( m_szAccount, szAccount );

	return CNetGlobal::NetConnect( szTempIP, iPort, szAccount, szPassword, pszMacAddress, HashedPassword );
}
//-------------------------------------------------------------------
void CNetCli_Login::RquestRoleData()
{
	// ¨ÌÄò§â¤Hª«¸ê®Æ­«·sµo°e
	map< int , SampleRoleData* >::iterator it;
	SampleRoleData* pRole = NULL;

	int index = 0;

	for( it = m_pThis->m_mapSampleRole.begin(); it != m_pThis->m_mapSampleRole.end(); it++ )
	{
		pRole = it->second;
		m_pThis->On_SamepleRoleData( index, pRole );
		index++;
	}
	m_pThis->On_SamepleRoleDataEnd();
}


void CNetCli_Login::LoadRoleData( const char* pszAccount )
{
	PG_Login_C2M_LoadRoleData Packet;

	strcpy( Packet.szAccountName, pszAccount );
	
	Packet.PlayTimeQuota = _Net.PlayTimeQuota();

	SendToMaster( sizeof( PG_Login_C2M_LoadRoleData ), &Packet );

	m_pThis->On_LoadRoleDataBegin();
}

//¦^´_¨¤¦â
void CNetCli_Login::DelRoleRecoverRequest				( int DBID )
{
	PG_Login_C2M_DelRoleRecoverRequest Send;
	Send.DBID = DBID;

	SendToMaster( sizeof( Send ), &Send );	
}

void CNetCli_Login::SetSecondPassword( char* Password )
{
	PG_Login_C2M_SetSecondPassword Send;
	strncpy( Send.Password , Password , sizeof(Send.Password) );
	SendToMaster( sizeof( Send ), &Send );	
}

void CNetCli_Login::ZoneLoadingRequest( int ZoneID )
{
	PG_Login_C2M_ZoneLoadingRequest Send;
	Send.ZoneID = ZoneID;
	SendToMaster( sizeof( Send ), &Send );	
}

//-------------------------------------------------------------------
void CNetCli_Login::ApexStartValue( int iValue )
{
	if( m_pThis->m_iApexSrvID != -1 )
	{
		PG_Login_C2APEX_APEXSTARTVALUE	Packet;

		Packet.iReturn	= iValue;

		_Net.SendToSrv( m_pThis->m_iApexSrvID, sizeof( Packet ), (PVOID)&Packet );
	}
}
//-------------------------------------------------------------------
void CNetCli_Login::SM_ChangeNameRequest( int DBID , const char* RoleName )
{
	PG_Login_C2M_ChangeNameRequest Send;
	Send.DBID = DBID;
	strncpy( Send.RoleName , RoleName , sizeof( Send.RoleName ) );

	SendToMaster( sizeof( Send ), &Send );	
}
//-------------------------------------------------------------------
void CNetCli_Login::SM_SecondPassword( const char* Password )
{
	PG_Login_CtoM_SecondPassword Send;
	strncpy( Send.Password , Password , sizeof(Send.Password) );
	SendToMaster( sizeof( Send ), &Send );	
}

//-------------------------------------------------------------------
void CNetCli_Login::SM_SecondPassword2( const char* Password)
{
	PG_Login_CtoM_SecondPassword2 Send;
	strncpy( Send.Password , Password , sizeof(Send.Password) );
	SendToMaster( sizeof( Send ), &Send );	
}

//-------------------------------------------------------------------
void CNetCli_Login::SM_CaptchaRefresh()
{
	PG_Login_CtoM_CaptchaRefresh Send;
	SendToMaster( sizeof( Send ), &Send );	
}

//-------------------------------------------------------------------
void CNetCli_Login::SM_CaptchaReply( const char* Captcha )
{
	PG_Login_CtoM_CaptchaReply Send;
	strncpy( Send.Captcha , Captcha , sizeof(Send.Captcha) );
	SendToMaster( sizeof( Send ), &Send );	
}

//-------------------------------------------------------------------
void CNetCli_Login::SM_GameGuardReport( int iType, int iSize, const char* pszReport )
{
	PG_Login_CtoM_GameGuardReport	Packet;

	Packet.iType	= iType;
	Packet.iSize	= iSize;
	Packet.Report	= pszReport;

	SendToMaster( sizeof( Packet ), &Packet );	
}
//-------------------------------------------------------------------
void CNetCli_Login::SM_CreateVivoxAccount	( const char* pszAccout )
{
	PG_Login_CtoM_CreateVivoxAccount	Packet;
	Packet.Account = pszAccout;
	SendToMaster( sizeof( Packet ), &Packet );
}

//-------------------------------------------------------------------
void CNetCli_Login::SL_RequestBonusList(int page)
{
	PG_Login_CtoL_RequestBonusList Send;
	Send.Page = page;

	SendToLocal(sizeof(Send), &Send);
}

//-------------------------------------------------------------------
void CNetCli_Login::SL_RequestBonus(int id)
{
	PG_Login_CtoL_RequestBonus Send;
	Send.ID = id;

	SendToLocal(sizeof(Send), &Send);
}