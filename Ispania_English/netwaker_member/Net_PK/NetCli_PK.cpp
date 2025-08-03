#include "NetCli_PK.h"

//-------------------------------------------------------------------
NetCli_PK*    NetCli_PK::This         = NULL;

//-------------------------------------------------------------------
bool NetCli_PK::_Init()
{
	/*
	_Net.RegOnSrvPacketFunction	( EM_PG_PK_LtoC_PKInviteNotify	  , _PG_PK_LtoC_PKInviteNotify	);
	_Net.RegOnSrvPacketFunction	( EM_PG_PK_LtoC_PKInviteResult	  , _PG_PK_LtoC_PKInviteResult	);
	_Net.RegOnSrvPacketFunction	( EM_PG_PK_LtoC_PKPrepare		  , _PG_PK_LtoC_PKPrepare		);
	_Net.RegOnSrvPacketFunction	( EM_PG_PK_LtoC_PKBegin			  , _PG_PK_LtoC_PKBegin			);
	_Net.RegOnSrvPacketFunction	( EM_PG_PK_LtoC_PKEnd			  , _PG_PK_LtoC_PKEnd			);
	_Net.RegOnSrvPacketFunction	( EM_PG_PK_LtoC_PKCancel		  , _PG_PK_LtoC_PKCancel		);
	*/

	Cli_NetReg( PG_PK_LtoC_PKInviteNotify	);
	Cli_NetReg( PG_PK_LtoC_PKInviteResult	);
	Cli_NetReg( PG_PK_LtoC_PKPrepare		);
	Cli_NetReg( PG_PK_LtoC_PKBegin			);
	Cli_NetReg( PG_PK_LtoC_PKEnd			);
	Cli_NetReg( PG_PK_LtoC_PKCancel			);
	
    return true;
}
//-------------------------------------------------------------------
bool NetCli_PK::_Release()
{
    return true;
}
//-------------------------------------------------------------------
void NetCli_PK::_PG_PK_LtoC_PKInviteNotify	( int Sockid , int Size , void* Data )
{
	PG_PK_LtoC_PKInviteNotify* PG = (PG_PK_LtoC_PKInviteNotify*)Data;
	This->R_PKInviteNotify( PG->TargetID , PG->TargetScore );
}
void NetCli_PK::_PG_PK_LtoC_PKInviteResult	( int Sockid , int Size , void* Data )
{
	PG_PK_LtoC_PKInviteResult* PG = (PG_PK_LtoC_PKInviteResult*)Data;
	This->R_PKInviteResult( PG->TargetID , PG->Result );
}
void NetCli_PK::_PG_PK_LtoC_PKPrepare		( int Sockid , int Size , void* Data )
{
	PG_PK_LtoC_PKPrepare* PG = (PG_PK_LtoC_PKPrepare*)Data;
	This->R_PKPrepare( PG->Player1ID , PG->Player2ID , PG->Player1Score , PG->Player2Score , PG->PosX , PG->PosY , PG->PosZ , PG->Range , PG->Time );
}
void NetCli_PK::_PG_PK_LtoC_PKBegin			( int Sockid , int Size , void* Data )
{
	PG_PK_LtoC_PKBegin* PG = (PG_PK_LtoC_PKBegin*)Data;
	This->R_PKBegin( PG->Time );
}
void NetCli_PK::_PG_PK_LtoC_PKEnd			( int Sockid , int Size , void* Data )
{
	PG_PK_LtoC_PKEnd* PG = (PG_PK_LtoC_PKEnd*)Data;
	This->R_PKEnd( PG->WinnerID , PG->LoserID , PG->IsEven );
}
void NetCli_PK::_PG_PK_LtoC_PKCancel		( int Sockid , int Size , void* Data )
{
	PG_PK_LtoC_PKCancel* PG = (PG_PK_LtoC_PKCancel*)Data;
	This->R_PKCancel( );
}

//-------------------------------------------------------------------
void NetCli_PK::S_PKInviteRequest	( int TargetID )
{
	PG_PK_CtoL_PKInviteRequest Send;
	Send.TargetID = TargetID;

	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_PK::S_PKInviteResult	( int TargetID , bool Result )
{
	PG_PK_CtoL_PKInviteResult Send;
	Send.TargetID = TargetID;
	Send.Result = Result;

	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_PK::S_PKCancel			( )
{
	PG_PK_CtoL_PKCancel Send;

	SendToLocal( sizeof(Send) , &Send );
}