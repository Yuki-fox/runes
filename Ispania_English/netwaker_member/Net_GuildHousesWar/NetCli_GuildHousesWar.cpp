/*
#include "NetCli_GuildHousesWar.h"

//-------------------------------------------------------------------
NetCli_GuildHousesWar*    NetCli_GuildHousesWar::This         = NULL;

//-------------------------------------------------------------------
bool NetCli_GuildHousesWar::_Init()
{
	Cli_NetReg( PG_GuildHousesWar_DtoC_OpenMenu				);
	Cli_NetReg( PG_GuildHousesWar_DtoC_WarRegisterResult	);
	Cli_NetReg( PG_GuildHousesWar_LtoC_WarBegin				);
	Cli_NetReg( PG_GuildHousesWar_LtoC_EnterWarResult		);
	Cli_NetReg( PG_GuildHousesWar_LtoC_LeaveWarResult		);
	Cli_NetReg( PG_GuildHousesWar_LtoC_EndWar				);
	Cli_NetReg( PG_GuildHousesWar_DtoC_HistoryResult		);
	Cli_NetReg( PG_GuildHousesWar_LtoC_GuildScore			);
	Cli_NetReg( PG_GuildHousesWar_LtoC_PlayerScore			);

    return true;
}
//-------------------------------------------------------------------
bool NetCli_GuildHousesWar::_Release()
{
    return true;
}
//-------------------------------------------------------------------
void NetCli_GuildHousesWar::_PG_GuildHousesWar_LtoC_GuildScore				( int Sockid , int Size , void* Data )
{
	PG_GuildHousesWar_LtoC_GuildScore* PG = (PG_GuildHousesWar_LtoC_GuildScore*)Data;
	This->R_GuildScore( PG->Info );
}
void NetCli_GuildHousesWar::_PG_GuildHousesWar_LtoC_PlayerScore				( int Sockid , int Size , void* Data )
{
	PG_GuildHousesWar_LtoC_PlayerScore* PG = (PG_GuildHousesWar_LtoC_PlayerScore*)Data;
	//This->R_PlayerScore( PG->PlayerDBID , PG->HitPoint , PG->KillPoint , PG->BuildPoint , PG->DestroyPoint , PG->HealPoint );
	This->R_PlayerScore( PG->Count , PG->PlayerList );
}

void NetCli_GuildHousesWar::_PG_GuildHousesWar_DtoC_HistoryResult			( int Sockid , int Size , void* Data )
{
	PG_GuildHousesWar_DtoC_HistoryResult* PG = (PG_GuildHousesWar_DtoC_HistoryResult*)Data;
	This->R_HistoryResult( PG->DayBefore , PG->Count , PG->List );
}
void NetCli_GuildHousesWar::_PG_GuildHousesWar_LtoC_EndWarPlayerInfo		( int Sockid , int Size , void* Data )
{
	PG_GuildHousesWar_LtoC_EndWarPlayerInfo* PG = (PG_GuildHousesWar_LtoC_EndWarPlayerInfo*)Data;
	This->R_EndWarPlayerInfo( PG->Count , PG->PlayerList );
}
void NetCli_GuildHousesWar::_PG_GuildHousesWar_LtoC_PricesResult			( int Sockid , int Size , void* Data )
{
	PG_GuildHousesWar_LtoC_PricesResult* PG = (PG_GuildHousesWar_LtoC_PricesResult*)Data;
	This->R_PricesResult( PG->Result );
}

void NetCli_GuildHousesWar::_PG_GuildHousesWar_DtoC_OpenMenu				( int Sockid , int Size , void* Data )
{
	PG_GuildHousesWar_DtoC_OpenMenu* PG = (PG_GuildHousesWar_DtoC_OpenMenu*)Data;
	This->R_OpenMenu( PG->State , PG->Size , PG->List );
}
void NetCli_GuildHousesWar::_PG_GuildHousesWar_DtoC_WarRegisterResult		( int Sockid , int Size , void* Data )
{
	PG_GuildHousesWar_DtoC_WarRegisterResult* PG = (PG_GuildHousesWar_DtoC_WarRegisterResult*)Data;
	This->R_WarRegisterResult( PG->Type , PG->Result );
}
void NetCli_GuildHousesWar::_PG_GuildHousesWar_LtoC_WarBegin				( int Sockid , int Size , void* Data )
{
	PG_GuildHousesWar_LtoC_WarBegin* PG = (PG_GuildHousesWar_LtoC_WarBegin*)Data;
	This->R_WarBegin( PG->GuildID );
}
void NetCli_GuildHousesWar::_PG_GuildHousesWar_LtoC_EnterWarResult			( int Sockid , int Size , void* Data )
{
	PG_GuildHousesWar_LtoC_EnterWarResult* PG = (PG_GuildHousesWar_LtoC_EnterWarResult*)Data;
	This->R_EnterWarResult( PG->Result );
}
void NetCli_GuildHousesWar::_PG_GuildHousesWar_LtoC_LeaveWarResult			( int Sockid , int Size , void* Data )
{
	PG_GuildHousesWar_LtoC_LeaveWarResult* PG = (PG_GuildHousesWar_LtoC_LeaveWarResult*)Data;
	This->R_LeaveWarResult( PG->Result );
}
void NetCli_GuildHousesWar::_PG_GuildHousesWar_LtoC_EndWar					( int Sockid , int Size , void* Data )
{
	PG_GuildHousesWar_LtoC_EndWar* PG = (PG_GuildHousesWar_LtoC_EndWar*)Data;
	This->R_EndWar( PG->GuildID , PG->Score );
}

void NetCli_GuildHousesWar::SL_WarRegister	( GuildHouseWarRegisterTypeENUM Type )
{
	PG_GuildHousesWar_CtoL_WarRegister Send;
	Send.Type = Type;
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_GuildHousesWar::SL_EnterWar		( )
{
	PG_GuildHousesWar_CtoL_EnterWar Send;
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_GuildHousesWar::SL_LeaveWar		( )
{
	PG_GuildHousesWar_CtoL_LeaveWar Send;
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_GuildHousesWar::SL_OpenMenu		( )
{
	PG_GuildHousesWar_CtoL_OpenMenu Send;
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_GuildHousesWar::SL_PricesRequest( )
{
	PG_GuildHousesWar_CtoL_PricesRequst Send;
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_GuildHousesWar::SL_HistoryRequest	( int DayBefore )
{
	PG_GuildHousesWar_CtoL_HistoryRequest Send;
	Send.DayBefore = DayBefore;
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_GuildHousesWar::SL_PlayerScoreRequest(  )
{
	PG_GuildHousesWar_CtoL_PlayerScoreRequest Send;
	SendToLocal( sizeof(Send) , &Send );
}*/