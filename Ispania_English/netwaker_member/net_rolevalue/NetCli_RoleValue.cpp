#include "NetCli_RoleValue.h"

//-------------------------------------------------------------------
NetCli_RoleValue*		NetCli_RoleValue::This			= NULL;
bool					NetCli_RoleValue::IsSetPoint	= false;

//-------------------------------------------------------------------
bool NetCli_RoleValue::_Init()
{
    Cli_NetReg( PG_RoleValue_LtoC_FixAllRoleValue 		);
    Cli_NetReg( PG_RoleValue_LtoC_FixRoleValue    		);
    Cli_NetReg( PG_RoleValue_LtoC_ResultSetPoint  		);
    Cli_NetReg( PG_RoleValue_LtoC_NotifyLvUp      		);
    Cli_NetReg( PG_RoleValue_LtoC_SetTitleFlag    		);
    Cli_NetReg( PG_RoleValue_LtoC_SetFlag         		);
    Cli_NetReg( PG_RoleValue_LtoC_SetTitleResult  		);
    //Cli_NetReg( PG_RoleValue_LtoC_Title           		);
	Cli_NetReg( PG_RoleValue_LtoC_RoleInfoChange  		);
	Cli_NetReg( PG_RoleValue_LtoC_OpenChangeJob	  		);
	Cli_NetReg( PG_RoleValue_LtoC_CloseChangeJob  		);
	Cli_NetReg( PG_RoleValue_LtoC_ChangeJobResult 		);
	Cli_NetReg( PG_RoleValue_LtoC_SetPlayerAllZoneState );
	Cli_NetReg( PG_RoleValue_LtoC_DeltaRoleValue		);

	Cli_NetReg( PG_RoleValue_LtoC_ClientLogoutResult	);
	Cli_NetReg( PG_RoleValue_LtoC_CancelClientLogout	);
	Cli_NetReg( PG_RoleValue_LtoC_WaitChangeParallel	);
	Cli_NetReg( PG_RoleValue_LtoC_ClientComputerInfoRequest	);
	Cli_NetReg( PG_RoleValue_LtoC_ObjMode				);
	Cli_NetReg( PG_RoleValue_LtoC_ObjRoleInfo			);
	Cli_NetReg( PG_RoleValue_LtoC_GetTargetRoleValue			);

	IsSetPoint = false;
    return true;
}
//-------------------------------------------------------------------
bool NetCli_RoleValue::_Release()
{
    return true;
}
//-------------------------------------------------------------------
void NetCli_RoleValue::_PG_RoleValue_LtoC_GetTargetRoleValue	( int Sockid , int Size , void* Data )
{
	PG_RoleValue_LtoC_GetTargetRoleValue *  PG = (PG_RoleValue_LtoC_GetTargetRoleValue *)Data;
	This->R_GetTargetRoleValue( PG->GUID, PG->ValueName, PG->Value );
}
void NetCli_RoleValue::_PG_RoleValue_LtoC_ObjRoleInfo		( int Sockid , int Size , void* Data )
{
	PG_RoleValue_LtoC_ObjRoleInfo *  PG = (PG_RoleValue_LtoC_ObjRoleInfo *)Data;
	
	This->R_ObjRoleInfo( PG->Role );
}
void NetCli_RoleValue::_PG_RoleValue_LtoC_ObjMode				( int Sockid , int Size , void* Data )
{
	PG_RoleValue_LtoC_ObjMode *  PG = (PG_RoleValue_LtoC_ObjMode *)Data;
	This->R_ObjMode( PG->ItemGUID , PG->Mode );
}

void NetCli_RoleValue::_PG_RoleValue_LtoC_ClientComputerInfoRequest( int Sockid , int Size , void* Data )
{
	PG_RoleValue_LtoC_ClientComputerInfoRequest *  PG = (PG_RoleValue_LtoC_ClientComputerInfoRequest *)Data;
	This->R_ClientComputerInfoRequest( );
}

void NetCli_RoleValue::_PG_RoleValue_LtoC_WaitChangeParallel	( int Sockid , int Size , void* Data )
{
	PG_RoleValue_LtoC_WaitChangeParallel *  PG = (PG_RoleValue_LtoC_WaitChangeParallel *)Data;
	This->R_WaitChangeParallel( PG->Second , PG->Result );
}
void NetCli_RoleValue::_PG_RoleValue_LtoC_ClientLogoutResult	( int Sockid , int Size , void* Data )
{
	PG_RoleValue_LtoC_ClientLogoutResult *  PG = (PG_RoleValue_LtoC_ClientLogoutResult *)Data;
	This->R_ClientLogoutResult( PG->Second );
}
void NetCli_RoleValue::_PG_RoleValue_LtoC_CancelClientLogout	( int Sockid , int Size , void* Data )
{
	PG_RoleValue_LtoC_CancelClientLogout *  PG = (PG_RoleValue_LtoC_CancelClientLogout *)Data;
	This->R_CancelClientLogout( );
}

void NetCli_RoleValue::_PG_RoleValue_LtoC_SetPlayerAllZoneState( int Sockid , int Size , void* Data )
{
	PG_RoleValue_LtoC_SetPlayerAllZoneState *  PG = (PG_RoleValue_LtoC_SetPlayerAllZoneState *)Data;
	This->R_SetPlayerAllZoneState( PG->GItemID , PG->State );
}
void NetCli_RoleValue::_PG_RoleValue_LtoC_CloseChangeJob	 ( int Sockid , int Size , void* Data )
{
	This->R_CloseChangeJob(  );
}
void NetCli_RoleValue::_PG_RoleValue_LtoC_OpenChangeJob	 ( int Sockid , int Size , void* Data )
{
	PG_RoleValue_LtoC_OpenChangeJob *  PG = (PG_RoleValue_LtoC_OpenChangeJob *)Data;
	This->R_OpenChangeJob( PG->TargetID );
}
void NetCli_RoleValue::_PG_RoleValue_LtoC_ChangeJobResult	 ( int Sockid , int Size , void* Data )
{
	PG_RoleValue_LtoC_ChangeJobResult *  PG = (PG_RoleValue_LtoC_ChangeJobResult *)Data;
	This->R_ChangeJobResult( PG->Result );
}

void NetCli_RoleValue::_PG_RoleValue_LtoC_RoleInfoChange    ( int Sockid , int Size , void* Data )
{
	PG_RoleValue_LtoC_RoleInfoChange *  PG = (PG_RoleValue_LtoC_RoleInfoChange *)Data;
	This->R_RoleInfoChange( PG->ItemID  , PG->Level , PG->Level_Sub , PG->Voc , PG->Voc_Sub );
}
void NetCli_RoleValue::_PG_RoleValue_LtoC_FixAllRoleValue   ( int Sockid , int Size , void* Data )
{
    PG_RoleValue_LtoC_FixAllRoleValue *  PG = (PG_RoleValue_LtoC_FixAllRoleValue *)Data;
    This->R_FixAllRoleValue( PG->Ability );
}
void NetCli_RoleValue::_PG_RoleValue_LtoC_FixRoleValue      ( int Sockid , int Size , void* Data )        
{
    PG_RoleValue_LtoC_FixRoleValue *  PG = (PG_RoleValue_LtoC_FixRoleValue *)Data;
    This->R_FixRoleValue( PG->ValueName , PG->Value );
}
void NetCli_RoleValue::_PG_RoleValue_LtoC_DeltaRoleValue      ( int Sockid , int Size , void* Data )        
{
	PG_RoleValue_LtoC_DeltaRoleValue *  PG = (PG_RoleValue_LtoC_DeltaRoleValue *)Data;
	This->R_DeltaRoleValue( PG->ValueName , PG->Value );
}
void NetCli_RoleValue::_PG_RoleValue_LtoC_ResultSetPoint    ( int Sockid , int Size , void* Data )     
{
    PG_RoleValue_LtoC_ResultSetPoint *  PG = (PG_RoleValue_LtoC_ResultSetPoint *)Data;
	IsSetPoint = false;
    if( PG->Result == true )
    {
        This->R_SetPoint_OK( );
    }
    else
    {
        This->R_SetPoint_Failed( );
    }

}

void NetCli_RoleValue::_PG_RoleValue_LtoC_NotifyLvUp        ( int Sockid , int Size , void* Data )     
{
    PG_RoleValue_LtoC_NotifyLvUp *  PG = (PG_RoleValue_LtoC_NotifyLvUp *)Data;
    This->R_NotifyLvUp( PG->ItemID , PG->LV );
}
void NetCli_RoleValue::_PG_RoleValue_LtoC_SetTitleFlag      ( int Sockid , int Size , void* Data )     
{
    PG_RoleValue_LtoC_SetTitleFlag *  PG = (PG_RoleValue_LtoC_SetTitleFlag *)Data;
    This->R_SetTitleFlag( PG->TitleID , PG->Flag );
}
void NetCli_RoleValue::_PG_RoleValue_LtoC_SetFlag           ( int Sockid , int Size , void* Data )        
{
    PG_RoleValue_LtoC_SetFlag *  PG = (PG_RoleValue_LtoC_SetFlag *)Data;
    This->R_SetFlag( PG->ID , PG->Flag );
}
void NetCli_RoleValue::_PG_RoleValue_LtoC_SetTitleResult    ( int Sockid , int Size , void* Data )     
{
    PG_RoleValue_LtoC_SetTitleResult *  PG = (PG_RoleValue_LtoC_SetTitleResult *)Data;
    if( PG->Result == true )
        This->R_SetTitle_OK( PG->TitleID );
    else
        This->R_SetTitle_Failed( );
}

/*
void NetCli_RoleValue::_PG_RoleValue_LtoC_Title             ( int Sockid , int Size , void* Data )    
{
    PG_RoleValue_LtoC_Title *  PG = (PG_RoleValue_LtoC_Title *)Data;
    This->R_Title( PG->ItemID , PG->TitleID );
}
*/

void NetCli_RoleValue::S_RequestFixRoleValue   ( )
{
    PG_RoleValue_CtoL_RequestFixRoleValue Send;
    SendToLocal( sizeof(Send) , &Send );
}
void NetCli_RoleValue::S_RequestSetPoint       ( int STR , int STA , int INT , int MND , int AGI )
{
	if ( !IsSetPoint )
	{
		PG_RoleValue_CtoL_RequestSetPoint Send;
		Send.STR = STR;
		Send.STA = STA;
		Send.INT = INT;
		Send.MND = MND;
		Send.AGI = AGI;
		SendToLocal( sizeof(Send) , &Send );

		IsSetPoint = true;
	}
}
void NetCli_RoleValue::S_SetTitleRequest        ( int TitleID , bool IsShowTitle )
{
    PG_RoleValue_CtoL_SetTitlRequest Send;
    Send.TitleID = TitleID;
	Send.IsShowTitle = IsShowTitle;

    SendToLocal( sizeof(Send) , &Send );

}


void NetCli_RoleValue::S_CloseChangeJobRequest	 ( )
{
	PG_RoleValue_CtoL_CloseChangeJobRequest Send;
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_RoleValue::S_ChangeJobRequest	 ( int Job , int Job_Sub )
{
	PG_RoleValue_CtoL_ChangeJobRequest Send;
	Send.Job = Job;
	Send.Job_Sub = Job_Sub;

	SendToLocal( sizeof(Send) , &Send );
}

void NetCli_RoleValue::S_Logout				()
{
	PG_RoleValue_CtoL_ClientLogout Send;
	SendToLocal( sizeof(Send) , &Send );
}

void NetCli_RoleValue::S_CancelLogout		()
{
	PG_RoleValue_CtoL_CancelClientLogout Send;
	SendToLocal( sizeof(Send) , &Send );
}

void NetCli_RoleValue::S_SetPlayerAllZoneState	( PlayerStateStruct& State )
{
	PG_RoleValue_CtoL_SetPlayerAllZoneState Send;
	Send.State._State = State._State;
	SendToLocal( sizeof(Send) , &Send );
}

void NetCli_RoleValue::S_ClientFlag ( StaticFlag<_MAX_CLIENT_FLAG_COUNT_>& ClientFlag  )
{
	PG_RoleValue_CtoL_SetClientFlag Send;
	Send.ClientFlag = ClientFlag;
	SendToLocal( sizeof(Send) , &Send );
}

void NetCli_RoleValue::S_ClientData ( unsigned char ClientData[_MAX_CLIENT_DATA_SIZE_] )
{
	PG_RoleValue_CtoL_SetClientData Send;
	memcpy( Send.ClientData , ClientData , sizeof(Send.ClientData) );
	SendToLocal( sizeof(Send) , &Send );
}

void NetCli_RoleValue::S_ChangeParallel			( int ParallelID )
{
	PG_RoleValue_CtoL_ChangeParallel Send;
	Send.ParallelID = ParallelID;
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_RoleValue::S_CancelChangeParallel	( )
{
	PG_RoleValue_CtoL_CancelChangeParallel Send;
	SendToLocal( sizeof(Send) , &Send );
}

void NetCli_RoleValue::S_ClientComputerInfo	( ClientComputerInfoStruct& Info )
{
	PG_RoleValue_CtoL_ClientComputerInfo Send;
	Send.Info = Info;
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_RoleValue::S_ClientEnvironmentState( ClientStateStruct& State )
{
	PG_RoleValue_CtoL_ClientEnvironmentState Send;
	Send.State = State;
	SendToLocal( sizeof(Send) , &Send );
}