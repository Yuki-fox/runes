#include "NetCli_Festival.h"
#include "../../mainproc/GameMain.h"

CNetCli_Festival* CNetCli_Festival::m_pThis	= NULL;

CNetCli_Festival::CNetCli_Festival(void)
{

}

CNetCli_Festival::~CNetCli_Festival(void)
{

}

bool CNetCli_Festival::_Init()
{
	Cli_NetReg( PG_Festival_LtoC_FestivalStatus			);
	return true;
}

bool CNetCli_Festival::_Release()
{
	return true;
}

void CNetCli_Festival::_PG_Festival_LtoC_FestivalStatus( int iSrvID , int iSize , PVOID pData )
{
	M_PACKET( PG_Festival_LtoC_FestivalStatus );
	m_pThis->OnRZ_FestivalStatus( iSrvID, p->iFestivalID, p->iStatus, p->strSceneControl);
}
