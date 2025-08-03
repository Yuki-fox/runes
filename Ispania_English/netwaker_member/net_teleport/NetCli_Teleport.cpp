#include "NetCli_Teleport.h"

//-------------------------------------------------------------------
NetCli_Teleport*    NetCli_Teleport::This         = NULL;

//-------------------------------------------------------------------
bool NetCli_Teleport::_Init()
{
	Cli_NetReg( PG_Teleport_LtoC_AddTeleportResult		);
	Cli_NetReg( PG_Teleport_LtoC_DelTeleportResult		);
	Cli_NetReg( PG_Teleport_LtoC_SwapTeleportResult		);
	Cli_NetReg( PG_Teleport_LtoC_ModifyTeleportResult	);
	Cli_NetReg( PG_Teleport_LtoC_UseTeleportResult		);

    return true;
}
//-------------------------------------------------------------------
bool NetCli_Teleport::_Release()
{
    return true;
}
//-------------------------------------------------------------------
void NetCli_Teleport::_PG_Teleport_LtoC_AddTeleportResult		( int Sockid , int Size , void* Data )
{
	PG_Teleport_LtoC_AddTeleportResult* PG = (PG_Teleport_LtoC_AddTeleportResult*)Data;
	This->R_AddTeleportResult( 	PG->TeleportPos , PG->Info , PG->Result );
}
void NetCli_Teleport::_PG_Teleport_LtoC_DelTeleportResult		( int Sockid , int Size , void* Data )
{
	PG_Teleport_LtoC_DelTeleportResult* PG = (PG_Teleport_LtoC_DelTeleportResult*)Data;
	This->R_DelTeleportResult( 	PG->TeleportPos , PG->Result );
}
void NetCli_Teleport::_PG_Teleport_LtoC_SwapTeleportResult		( int Sockid , int Size , void* Data )	
{
	PG_Teleport_LtoC_SwapTeleportResult* PG = (PG_Teleport_LtoC_SwapTeleportResult*)Data;
	This->R_SwapTeleportResult( PG->TeleportPos , PG->Result );
}
void NetCli_Teleport::_PG_Teleport_LtoC_ModifyTeleportResult	( int Sockid , int Size , void* Data )
{
	PG_Teleport_LtoC_ModifyTeleportResult* PG = (PG_Teleport_LtoC_ModifyTeleportResult*)Data;
	This->R_ModifyTeleportResult( PG->TeleportPos , PG->Info , PG->Result );
}
void NetCli_Teleport::_PG_Teleport_LtoC_UseTeleportResult		( int Sockid , int Size , void* Data )
{
	PG_Teleport_LtoC_UseTeleportResult* PG = (PG_Teleport_LtoC_UseTeleportResult*)Data;
	This->R_UseTeleportResult( PG->Type	,PG->ItemPos , PG->TeleportPos , PG->Result );
}
//////////////////////////////////////////////////////////////////////////
void NetCli_Teleport::SL_AddTeleportRequest		( int ItemPos , int TeleportPos , LocationSaveStruct& Info )
{
	PG_Teleport_CtoL_AddTeleportRequest Send;
	Send.Info = Info;
	Send.ItemPos = ItemPos;
	Send.TeleportPos = TeleportPos;
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Teleport::SL_DelTeleportRequest		( int TeleportPos )
{
	PG_Teleport_CtoL_DelTeleportRequest Send;
	Send.TeleportPos = TeleportPos;
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Teleport::SL_SwapTeleportRequest	( int TeleportPos[2] )
{
	PG_Teleport_CtoL_SwapTeleportRequest Send;
	Send.TeleportPos[0] = TeleportPos[0];
	Send.TeleportPos[1] = TeleportPos[1];
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Teleport::SL_ModifyTeleportRequest	( int TeleportPos , LocationSaveStruct& Info )
{
	PG_Teleport_CtoL_ModifyTeleportRequest Send;
	Send.Info = Info;
	Send.TeleportPos = TeleportPos;
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Teleport::SL_UseTeleportRequest		( UseTeleportTypeENUM Type , int ItemPos , int TeleportPos )
{
	PG_Teleport_CtoL_UseTeleportRequest Send;
	Send.ItemPos = ItemPos;
	Send.TeleportPos = TeleportPos;
	Send.Type = Type;
	SendToLocal( sizeof(Send) , &Send );
}
//////////////////////////////////////////////////////////////////////////