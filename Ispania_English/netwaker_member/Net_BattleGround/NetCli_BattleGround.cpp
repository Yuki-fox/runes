#include "NetCli_BattleGround.h"
#include "../../mainproc/GameMain.h"


// ¥þ°ì«Å§i
//-------------------------------------------------------------------
CNetCli_BattleGround*		CNetCli_BattleGround::m_pThis				= NULL;
//-------------------------------------------------------------------
CNetCli_BattleGround::CNetCli_BattleGround(void)
{

}
// --------------------------------------------------------------------------------------
CNetCli_BattleGround::~CNetCli_BattleGround(void)
{

}
// --------------------------------------------------------------------------------------
bool CNetCli_BattleGround::Init()
{
	Cli_NetReg( PG_BattleGround_LtoC_GetBattleGroundResult			);
	Cli_NetReg( PG_BattleGround_LtoC_EnterBattleGroundRight			);
	Cli_NetReg( PG_BattleGround_LtoC_NotifyEnterBattleGroundRight	);
	Cli_NetReg( PG_BattleGround_LtoC_LostEnterBattleGroundRight		);
	Cli_NetReg( PG_BattleGround_LtoC_EmptyBattleGround				);
	Cli_NetReg( PG_BattleGround_LtoC_JoinQueueResult				);
	Cli_NetReg( PG_BattleGround_LtoC_BattleGroundClose				);
	Cli_NetReg( PG_BattleGround_LtoC_BattleGroundStatus				);
	Cli_NetReg( PG_BattleGround_LtoC_BattleGroundLandMark			);
	Cli_NetReg( PG_BattleGround_LtoC_SetBattleGroundData			);
	Cli_NetReg( PG_BattleGround_LtoC_BattleGroundScoreInfo			);
	Cli_NetReg( PG_BattleGround_LtoC_BattleGroundWaitQueueStatus	);
	Cli_NetReg( PG_BattleGround_LtoC_AllLandMarkInfo				);
	Cli_NetReg( PG_BattleGround_LtoC_AllBattleGroundPlayerPos		);
	Cli_NetReg( PG_BattleGround_LtoC_UpdateArenaData				);

	Cli_NetReg( PG_BattleGround_LtoC_GetRankPersonalInfoResult		);

	

	return true;
}
//--------------------------------------------------------------------------------------
void CNetCli_BattleGround::_PG_BattleGround_LtoC_GetRankPersonalInfoResult ( int iSrvID , int iSize , PVOID pData )
{
	M_PACKET( PG_BattleGround_LtoC_GetRankPersonalInfoResult );
	m_pThis->OnRZ_GetRankPersonalInfoResult( iSrvID, p->iDBID, p->iRankPoint, p->iLastRoundRank, p->iRank, p->sNote.Begin() );
}
//--------------------------------------------------------------------------------------
void CNetCli_BattleGround::_PG_BattleGround_LtoC_UpdateArenaData ( int iSrvID , int iSize , PVOID pData )
{
	M_PACKET( PG_BattleGround_LtoC_UpdateArenaData );
	m_pThis->OnRZ_UpdateArenaData( iSrvID, p->iArenaType, p->iTeamGUID, &( p->ArenaInfo ) );
}
//--------------------------------------------------------------------------------------
void CNetCli_BattleGround::_PG_BattleGround_LtoC_AllBattleGroundPlayerPos		( int iSrvID , int iSize , PVOID pData )
{
	M_PACKET( PG_BattleGround_LtoC_AllBattleGroundPlayerPos );
	m_pThis->OnRZ_AllBattleGroundPlayerPos(iSrvID, p->iTotalPlayer, p->PlayerPos );
}

void CNetCli_BattleGround::_PG_BattleGround_LtoC_AllLandMarkInfo				( int iSrvID , int iSize , PVOID pData )
{
	M_PACKET( PG_BattleGround_LtoC_AllLandMarkInfo );
	m_pThis->OnRZ_AllLandMarkInfo( iSrvID, &( p->Info ) );
}
// --------------------------------------------------------------------------------------
void CNetCli_BattleGround::_PG_BattleGround_LtoC_BattleGroundScoreInfo( int iSrvID , int iSize , PVOID pData )
{
	M_PACKET( PG_BattleGround_LtoC_BattleGroundScoreInfo );
	m_pThis->OnRZ_BattleGroundScoreInfo( iSrvID, p->iTotalRole, p->pScoreArray );
}
// --------------------------------------------------------------------------------------
void CNetCli_BattleGround::_PG_BattleGround_LtoC_SetBattleGroundData( int iSrvID , int iSize , PVOID pData )
{
	M_PACKET( PG_BattleGround_LtoC_SetBattleGroundData );
	m_pThis->OnRZ_SetBattleGroundData( iSrvID, p->iForceFlagID, p->iCampID, p->iRoomID );
}
// --------------------------------------------------------------------------------------
void CNetCli_BattleGround::_PG_BattleGround_LtoC_BattleGroundLandMark( int iSrvID , int iSize , PVOID pData )
{
	M_PACKET( PG_BattleGround_LtoC_BattleGroundLandMark );
	m_pThis->OnRZ_BattleGroundLandMark( iSrvID, p->iID, &(p->LandMark) );
}
// --------------------------------------------------------------------------------------
void CNetCli_BattleGround::_PG_BattleGround_LtoC_BattleGroundClose( int iSrvID , int iSize , PVOID pData )
{
	M_PACKET( PG_BattleGround_LtoC_BattleGroundClose );
	m_pThis->OnRZ_BattleGroundClose( iSrvID, p->iWinTeamID );
}
// --------------------------------------------------------------------------------------
void CNetCli_BattleGround::_PG_BattleGround_LtoC_BattleGroundStatus( int iSrvID , int iSize , PVOID pData )
{
	M_PACKET( PG_BattleGround_LtoC_BattleGroundStatus );
	m_pThis->OnRZ_BattleGroundStatus( iSrvID, &(p->Data) );
}
// --------------------------------------------------------------------------------------
void CNetCli_BattleGround::_PG_BattleGround_LtoC_EnterBattleGroundRight	( int iSrvID , int iSize , PVOID pData )
{
	M_PACKET( PG_BattleGround_LtoC_EnterBattleGroundRight );
	m_pThis->OnRZ_EnterBattleGroundRight( iSrvID, p->iType, p->iRoomID, p->iTeamID, p->iWaitTime, p->sName.Begin() );
}

void CNetCli_BattleGround::_PG_BattleGround_LtoC_NotifyEnterBattleGroundRight( int iSrvID , int iSize , PVOID pData )
{
	M_PACKET( PG_BattleGround_LtoC_NotifyEnterBattleGroundRight );
	m_pThis->OnRZ_NotifyEnterBattleGroundRight( iSrvID, p->iType, p->iRoomID, p->iWaitTime, p->sName.Begin() );
}

void CNetCli_BattleGround::_PG_BattleGround_LtoC_LostEnterBattleGroundRight( int iSrvID , int iSize , PVOID pData )
{
	M_PACKET( PG_BattleGround_LtoC_LostEnterBattleGroundRight );
	m_pThis->OnRZ_LostEnterBattleGroundRight( iSrvID, p->iType, p->iRoomID, p->iWaitTime, p->sName.Begin() );
}

void CNetCli_BattleGround::_PG_BattleGround_LtoC_EmptyBattleGround( int iSrvID , int iSize , PVOID pData )
{
	M_PACKET( PG_BattleGround_LtoC_EmptyBattleGround );
	m_pThis->OnRZ_EmptyBattleGround( iSrvID, p->iType, p->iRoomID);
}

void CNetCli_BattleGround::_PG_BattleGround_LtoC_JoinQueueResult( int iSrvID , int iSize , PVOID pData )
{
	M_PACKET( PG_BattleGround_LtoC_JoinQueueResult );
	m_pThis->OnRZ_JoinQueueResult( iSrvID, p->iType, p->iRoomID, p->iTeamID, p->iWaitTime, p->sName.Begin(), p->iResult);
}

// --------------------------------------------------------------------------------------
void CNetCli_BattleGround::_PG_BattleGround_LtoC_BattleGroundWaitQueueStatus(int iSrvID , int iSize , PVOID pData )
{
	M_PACKET( PG_BattleGround_LtoC_BattleGroundWaitQueueStatus );
	m_pThis->OnRZ_BattleGroundWaitQueueStatus( iSrvID, p->iBattleGroundType, p->iStatus, p->iWaitID, p->iSrvCheckTime);
}
// --------------------------------------------------------------------------------------
bool CNetCli_BattleGround::Release()
{

	return true;
}
//--------------------------------------------------------------------------------------
void CNetCli_BattleGround::Update				()
{
	/*
	if( m_pThis )
		m_pThis->OnUpdate();
	*/
}
//--------------------------------------------------------------------------------------
void CNetCli_BattleGround::GetBattleGround( int iObjID )
{
	PG_BattleGround_CtoL_GetBattleGround	Packet;

	Packet.iObjID	= iObjID;

	SendToLocal( sizeof( Packet ) , &Packet );
}
//--------------------------------------------------------------------------------------
void CNetCli_BattleGround::JoinBattleGround( int iObjID, int iRoomID, int iParty, int iBattleGroundType, int *pTeamDBID )
{
	PG_BattleGround_CtoL_JoinBattleGround	Packet;

	Packet.iObjID			= iObjID;
	Packet.iBattleGroundID	= iRoomID;
	Packet.iParty			= iParty;
	Packet.iSrvID			= iBattleGroundType;

	if( pTeamDBID != NULL )
	{
		memcpy( Packet.iTeamDBID, pTeamDBID, sizeof( Packet.iTeamDBID ) );
	}
	else
	{
		ZeroMemory( Packet.iTeamDBID, sizeof( Packet.iTeamDBID ) );
	}

	SendToLocal( sizeof( Packet ) , &Packet );
}
//--------------------------------------------------------------------------------------
void CNetCli_BattleGround::EnterBattleGround( int iType, int iRoomID, int iTeamID )
{
	PG_BattleGround_CtoL_EnterBattleGround	Packet;

	Packet.iBattleGroundType = iType;
	Packet.iRoomID			 = iRoomID;
	Packet.iTeamID			 = iTeamID;

	SendToLocal( sizeof( Packet ) , &Packet );

	m_pThis->m_mapLandMark.clear();
}
//--------------------------------------------------------------------------------------
/*
void CNetCli_BattleGround::_PG_Script_LtoC_QuestAccept_Result ( int iCliID , int iSize , PVOID pData )
{
	M_PACKET( PG_Script_LtoC_QuestAccept_Result );	
	int a = 3;
	m_pThis->OnQuestAcceptResult( pPacket->iQuestField, pPacket->iQuestID, pPacket->emResult );
}
*/

void CNetCli_BattleGround::_PG_BattleGround_LtoC_GetBattleGroundResult		( int iSrvID , int iSize , PVOID pData )
{

	M_PACKET( PG_BattleGround_LtoC_GetBattleGroundResult );
	 
	/*
	vector< structBattleGroundResult >	vecBG;

	BYTE* ptr	= (BYTE*)pData;
	ptr			+= sizeof( DWORD );

	// BG COUNT
	int iCount	= *((DWORD*)ptr);
	ptr			+= sizeof( DWORD );

	// TYPE
	int iType	= *((DWORD*)ptr);
	ptr			+= sizeof( DWORD );


	// BG
	structBattleGroundResult BG;

	for( int i=0; i<iCount; i++ )
	{
		BG.iRoomID	=	*((int*)ptr);
		ptr			+=	sizeof(int);

		BG.szName	=	(char*)ptr;
		ptr			+= _MAX_OBJ_NAME_STR_SIZE_;

		BG.iSrvID   = iSrvID;

		vecBG.push_back( BG );
	}

	//M_PACKET( PG_BattleGround_LtoC_GetBattleGroundResult );
	*/

	vector< structBattleGroundResult >	vecBG;
	m_pThis->OnRZ_GetBattleGroundResult( iSrvID, p->iBattleGroundID, 0, vecBG );
}
//--------------------------------------------------------------------------------------
void CNetCli_BattleGround::LeaveBattleGround()
{
	PG_BattleGround_CtoL_LeaveBattleGround Packet;
	SendToLocal( sizeof( Packet ) , &Packet );
}
//--------------------------------------------------------------------------------------
void CNetCli_BattleGround::GetBattleGroundScoreInfo()
{
	PG_BattleGround_CtoL_GetBattleGroundScoreInfo Packet;

	Packet.iClientDBID	= g_pGameMain->GetDBID();

	SendToLocal( sizeof( Packet ) , &Packet );
}
//--------------------------------------------------------------------------------------
void CNetCli_BattleGround::LeaveBattleGroundWaitQueue	( int iDBID, int iType )
{
	PG_BattleGround_CtoL_LeaveBattleGroundWaitQueue	Packet;

	Packet.iDBID				= iDBID;
	Packet.iBattleGroundType	= iType;

	SendToLocal( sizeof( Packet ), &Packet );
}
//--------------------------------------------------------------------------------------
void CNetCli_BattleGround::BattleGroundWaitQueueStatusRespond ( int iSrvID, int iDBID )
{
	PG_BattleGround_CtoL_BattleGroundWaitQueueStatusRespond Packet;

	Packet.iSrvID	= iSrvID;
	Packet.iDBID	= iDBID;

	SendToLocal( sizeof( Packet ), &Packet );
}
//--------------------------------------------------------------------------------------
void CNetCli_BattleGround::GetBattleGroundList( int iDBID )
{
	PG_BattleGround_CtoL_GetBattleGroundList Packet;

	Packet.iDBID = iDBID;
	
	SendToLocal( sizeof( Packet ), &Packet );
}
//--------------------------------------------------------------------------------------
void CNetCli_BattleGround::GetRankPersonalInfo( int iMode )
{
	PG_BattleGround_CtoL_GetRankPersonalInfo	Packet;

	Packet.iMode	= iMode;

	SendToLocal( sizeof( Packet ), &Packet );
}
//--------------------------------------------------------------------------------------
void CNetCli_BattleGround::LeaveBattleGroundRemote( int iDBID, int iType )
{
	PG_BattleGround_CtoL_LeaveBattleGroundRemote	Packet;

	Packet.iBattleGroundType	= iType;
	Packet.iDBID				= iDBID;

	SendToLocal( sizeof( Packet ), &Packet );
}
