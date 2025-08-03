#include "Net_Instance.h"
#include "../MainProc/NetGlobal.h"
#pragma unmanaged

Net_Instance*  Net_Instance::This         = NULL;


bool Net_Instance::_Init()
{
	Cli_NetReg( PG_Instance_LtoC_ResetTime				);
	Cli_NetReg( PG_Instance_LtoC_JoinInstanceRequest	);
	Cli_NetReg( PG_Instance_LtoC_InstanceReset			);
	Cli_NetReg( PG_Instance_LtoC_InstanceCreate			);
	Cli_NetReg( PG_Instance_LtoC_FixInstanceExtend		);
	Cli_NetReg( PG_Instance_LtoC_InstanceRestTime		);
	Cli_NetReg( PG_Instance_LtoC_GetWorldVarResult		);
    return true;
}
//-------------------------------------------------------------------
void Net_Instance::_PG_Instance_LtoC_GetWorldVarResult		( int Sockid , int Size , void* Data )
{
	PG_Instance_LtoC_GetWorldVarResult* PG = (PG_Instance_LtoC_GetWorldVarResult*)Data;
	This->R_GetWorldVarResult( PG->VarName.Begin(), PG->VarValue );
}

void Net_Instance::_PG_Instance_LtoC_FixInstanceExtend		( int Sockid , int Size , void* Data )
{
	PG_Instance_LtoC_FixInstanceExtend* PG = (PG_Instance_LtoC_FixInstanceExtend*)Data;
	This->R_FixInstanceExtend( PG->ZoneID , PG->ExtendTime , PG->LiveTime , PG->KeyID );
}
void Net_Instance::_PG_Instance_LtoC_InstanceRestTime		( int Sockid , int Size , void* Data )
{
	PG_Instance_LtoC_InstanceRestTime* PG = (PG_Instance_LtoC_InstanceRestTime*)Data;
	This->R_InstanceRestTime( PG->ZoneID , PG->ExtendTime , PG->LiveTime , PG->KeyID );
}
bool Net_Instance::_Release()
{
    return true;
}
void Net_Instance::_PG_Instance_LtoC_InstanceCreate		( int Sockid , int Size , void* Data )
{
	PG_Instance_LtoC_InstanceCreate* PG = (PG_Instance_LtoC_InstanceCreate*)Data;
	This->R_InstanceCreate( PG->PlayerDBID , PG->ZoneID , PG->Key );
}
void Net_Instance::_PG_Instance_LtoC_ResetTime				( int Sockid , int Size , void* Data )
{
	PG_Instance_LtoC_ResetTime* PG = (PG_Instance_LtoC_ResetTime*)Data;
	This->R_ResetTime( 	PG->Time , PG->InstanceSaveID , PG->ValueKeyID , PG->IsLogin );
}
void Net_Instance::_PG_Instance_LtoC_JoinInstanceRequest	( int Sockid , int Size , void* Data )
{
	PG_Instance_LtoC_JoinInstanceRequest* PG = (PG_Instance_LtoC_JoinInstanceRequest*)Data;
	This->R_JoinInstanceRequest(  );
}
void Net_Instance::_PG_Instance_LtoC_InstanceReset			( int Sockid , int Size , void* Data )
{
	PG_Instance_LtoC_InstanceReset* PG = (PG_Instance_LtoC_InstanceReset*)Data;
	This->R_InstanceReset( PG->InstanceKey );
}

void Net_Instance::SL_JoinInstanceResult( bool Result )
{
	PG_Instance_CtoL_JoinInstanceResult Send;
	Send.Result = Result;
	SendToLocal( sizeof(Send) , &Send );
}

void Net_Instance::SL_InstanceExtend			( int ZoneID , int ExtendTime , int SavePos )
{
	PG_Instance_CtoL_InstanceExtend Send;
	Send.ZoneID = ZoneID;
	Send.ExtendTime = ExtendTime;
	Send.SavePos = SavePos;
	SendToLocal( sizeof(Send) , &Send );
}
void Net_Instance::SL_InstanceRestTimeRequest	( int ZoneID , int SavePos )
{
	PG_Instance_CtoL_InstanceRestTimeRequest Send;
	Send.ZoneID = ZoneID;
	Send.SavePos = SavePos;
	SendToLocal( sizeof(Send) , &Send );
}
void Net_Instance::SL_GetWorldVar( const char* pszVarName )
{
	PG_Instance_CtoL_GetWorldVar Send;
	Send.VarName = pszVarName;
	SendToLocal( sizeof(Send) , &Send );
}