#include "NetCli_PublicEncounter.h"
#include "../../mainproc/GameMain.h"

CNetCli_PublicEncounter* CNetCli_PublicEncounter::m_pThis	= NULL;

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
bool CNetCli_PublicEncounter::_Init()
{
	Cli_NetReg( PG_PE_LtoC_PEConfigData			);
	Cli_NetReg( PG_PE_LtoC_PEStatus				);
	Cli_NetReg( PG_PE_LtoC_PhaseStatus			);
	Cli_NetReg( PG_PE_LtoC_PlayerScore			);
	Cli_NetReg( PG_PE_LtoC_PEActivated			);
	Cli_NetReg( PG_PE_LtoC_PEDeactivated		);
	Cli_NetReg( PG_PE_LtoC_PEActivePhaseChange	);
	Cli_NetReg( PG_PE_LtoC_ObjectiveValueChage	);
	Cli_NetReg( PG_PE_LtoC_PESuccess			);
	Cli_NetReg( PG_PE_LtoC_PEFailed				);
	Cli_NetReg( PG_PE_LtoC_PhaseSuccess			);
	Cli_NetReg( PG_PE_LtoC_PhaseFailed			);
	Cli_NetReg( PG_PE_LtoC_ObjectiveAchieved	);
	Cli_NetReg( PG_PE_LtoC_ObjectiveFlag		);
	Cli_NetReg( PG_PE_LtoC_PEFlag				);

	return true;
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
bool CNetCli_PublicEncounter::_Release()
{
	return true;
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void CNetCli_PublicEncounter::_PG_PE_LtoC_PEConfigData(int iSrvID, int iSize, PVOID pData)
{
	m_pThis->OnRZ_PEConfigData(iSrvID, iSize, pData);	

}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void CNetCli_PublicEncounter::_PG_PE_LtoC_PEStatus(int iSrvID, int iSize, PVOID pData)
{
	M_PACKET( PG_PE_LtoC_PEStatus );
	m_pThis->OnRZ_PEStatus(iSrvID, p->iPEIndex, p->iStatus);
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void CNetCli_PublicEncounter::_PG_PE_LtoC_PhaseStatus(int iSrvID, int iSize, PVOID pData)
{
	M_PACKET( PG_PE_LtoC_PhaseStatus );
	m_pThis->OnRZ_PhaseStatus(iSrvID, p->iPEIndex, p->iPHIndex, p->iStatus);
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void CNetCli_PublicEncounter::_PG_PE_LtoC_ObjectiveStatus(int iSrvID, int iSize, PVOID pData)
{
	M_PACKET( PG_PE_LtoC_ObjectiveStatus );
	m_pThis->OnRZ_ObjectiveStatus(iSrvID, p->iPEIndex, p->iPHIndex, p->iOBIndex, p->iStatus);
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void CNetCli_PublicEncounter::_PG_PE_LtoC_PlayerScore(int iSrvID, int iSize, PVOID pData)
{
	M_PACKET( PG_PE_LtoC_PlayerScore );
	m_pThis->OnRZ_PlayerScore(iSrvID, p->iPEIndex, p->iScore);
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void CNetCli_PublicEncounter::_PG_PE_LtoC_PEActivated(int iSrvID, int iSize, PVOID pData)
{
	M_PACKET( PG_PE_LtoC_PEActivated );
	m_pThis->OnRZ_PEActivated(iSrvID, p->iPEIndex, p->iActivePHIndex);
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void CNetCli_PublicEncounter::_PG_PE_LtoC_PEDeactivated(int iSrvID, int iSize, PVOID pData)
{
	M_PACKET( PG_PE_LtoC_PEDeactivated );
	m_pThis->OnRZ_PEDeactivated(iSrvID, p->iPEIndex);
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void CNetCli_PublicEncounter::_PG_PE_LtoC_PEActivePhaseChange(int iSrvID, int iSize, PVOID pData)
{
	M_PACKET( PG_PE_LtoC_PEActivePhaseChange );
	m_pThis->OnRZ_PEActivePhaseChange(iSrvID, p->iPEIndex, p->iActivePHIndex);
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void CNetCli_PublicEncounter::_PG_PE_LtoC_ObjectiveValueChage(int iSrvID, int iSize, PVOID pData)
{
	M_PACKET( PG_PE_LtoC_ObjectiveValueChage );
	m_pThis->OnRZ_ObjectiveValueChage(iSrvID, p->iPEIndex, p->iPHIndex, p->iOBIndex, p->iNowValue);
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void CNetCli_PublicEncounter::_PG_PE_LtoC_PESuccess(int iSrvID, int iSize, PVOID pData)
{
	M_PACKET( PG_PE_LtoC_PESuccess );
	m_pThis->OnRZ_PESuccess(iSrvID, p->iPEIndex, p->iResetTimeGap);
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void CNetCli_PublicEncounter::_PG_PE_LtoC_PEFailed(int iSrvID, int iSize, PVOID pData)
{
	M_PACKET( PG_PE_LtoC_PEFailed );
	m_pThis->OnRZ_PEFailed(iSrvID, p->iPEIndex, p->iResetTimeGap);
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void CNetCli_PublicEncounter::_PG_PE_LtoC_PhaseSuccess(int iSrvID, int iSize, PVOID pData)
{
	M_PACKET( PG_PE_LtoC_PhaseSuccess );
	m_pThis->OnRZ_PhaseSuccess(iSrvID, p->iPEIndex, p->iPHIndex, p->iNextPhaseTimeGap);
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void CNetCli_PublicEncounter::_PG_PE_LtoC_PhaseFailed(int iSrvID, int iSize, PVOID pData)
{
	M_PACKET( PG_PE_LtoC_PhaseFailed );
	m_pThis->OnRZ_PhaseFailed(iSrvID, p->iPEIndex, p->iPHIndex, p->iNextPhaseTimeGap);
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void CNetCli_PublicEncounter::_PG_PE_LtoC_ObjectiveAchieved(int iSrvID, int iSize, PVOID pData)
{
	M_PACKET( PG_PE_LtoC_ObjectiveAchieved );
	m_pThis->OnRZ_ObjectiveAchieved(iSrvID, p->iPEIndex, p->iPHIndex, p->iOBIndex);
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void CNetCli_PublicEncounter::_PG_PE_LtoC_ObjectiveFlag(int iSrvID, int iSize, PVOID pData)
{
	M_PACKET( PG_PE_LtoC_ObjectiveFlag );
	m_pThis->OnRZ_ObjectiveFlag(iSrvID, p->iPEIndex, p->iPHIndex, p->iOBIndex, p->iFlag);
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void CNetCli_PublicEncounter::_PG_PE_LtoC_PEFlag(int iSrvID, int iSize, PVOID pData)
{
	M_PACKET( PG_PE_LtoC_PEFlag );
	m_pThis->OnRZ_PEFlag(iSrvID, p->iPEIndex, p->iFlag);
}

//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
void CNetCli_PublicEncounter::SL_RegionChanged(int iRegionID)
{
	PG_PE_CtoL_RegionChanged Send;
	Send.iRegionID = iRegionID;

	SendToLocal( sizeof(Send) , &Send );
}
