#include "NetCli_Gather.h"

// ¥þ°ì«Å§i
//-------------------------------------------------------------------
CNetCli_Gather*		CNetCli_Gather::m_pThis				= NULL;
//-------------------------------------------------------------------
CNetCli_Gather::CNetCli_Gather(void)
{

}
// --------------------------------------------------------------------------------------
CNetCli_Gather::~CNetCli_Gather(void)
{

}
// --------------------------------------------------------------------------------------
bool CNetCli_Gather::Init()
{
	Cli_NetReg( PG_Gather_LtoC_ClickGatherObj_Result		);
	Cli_NetReg( PG_Gather_LtoC_GatherMotion					);
		
	return true;
}
// --------------------------------------------------------------------------------------
bool CNetCli_Gather::Release()
{
	return true;
}
//--------------------------------------------------------------------------------------
void CNetCli_Gather::_PG_Gather_LtoC_ClickGatherObj_Result ( int iCliID , int iSize , PVOID pData )
{
	M_PACKET( PG_Gather_LtoC_ClickGatherObj_Result );	
	m_pThis->OnClickGatherObjResult( pPacket->iResult, pPacket->iItemID, pPacket->iItemVal );
}


void CNetCli_Gather::ClickGatherObj ( int iObjID )
{
	PG_Gather_CtoL_ClickGatherObj Packet;

	Packet.iObjID = iObjID;

	SendToLocal( sizeof( Packet ), &Packet );

}
// --------------------------------------------------------------------------------------
void CNetCli_Gather::_PG_Gather_LtoC_GatherMotion ( int iCliID , int iSize , PVOID pData )
{
	M_PACKET( PG_Gather_LtoC_GatherMotion );	
	m_pThis->OnGatherMotion( pPacket->iGatherID, pPacket->iMotionID, pPacket->iMineType );

}
// --------------------------------------------------------------------------------------
void CNetCli_Gather::CancelGather ( int iObjID )
{
	PG_Gather_CtoL_CancelGather	Packet;
	Packet.iObjID	= iObjID;
	SendToLocal( sizeof( Packet ), &Packet );
}