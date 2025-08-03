#include "NetCli_Attack.h"

//-------------------------------------------------------------------
NetCli_Attack*    NetCli_Attack::This         = NULL;

//-------------------------------------------------------------------
bool NetCli_Attack::_Init()
{
    Cli_NetReg( PG_Attack_LtoC_AttackResult 		);
    Cli_NetReg( PG_Attack_LtoC_Dead         		);
    Cli_NetReg( PG_Attack_LtoC_Resurrection 		);
    Cli_NetReg( PG_Attack_LtoC_HPMPSP       		);
    Cli_NetReg( PG_Attack_LtoC_MaxHPMaxMPMaxSP		);
    Cli_NetReg( PG_Attack_LtoC_AttackSignalResult	);
	Cli_NetReg( PG_Attack_LtoC_PartyMemberHPMPSP    );
	Cli_NetReg( PG_Attack_LtoC_PartyMemberMaxHPMPSP );
	Cli_NetReg( PG_Attack_LtoC_NPCAttackTarget		);
	Cli_NetReg( PG_Attack_LtoC_SetAttackMode		);
	Cli_NetReg( PG_Attack_LtoC_ZonePKFlag			);
    return true;
}
//-------------------------------------------------------------------
bool NetCli_Attack::_Release()
{
    return true;
}
//-------------------------------------------------------------------
void NetCli_Attack::_PG_Attack_LtoC_ZonePKFlag			( int Sockid , int Size , void* Data )
{
	PG_Attack_LtoC_ZonePKFlag*  PG = (PG_Attack_LtoC_ZonePKFlag*)Data;
	This->R_ZonePKFlag(  PG->PKType );
}
void NetCli_Attack::_PG_Attack_LtoC_SetAttackMode		( int Sockid , int Size , void* Data )
{
	PG_Attack_LtoC_SetAttackMode*  PG = (PG_Attack_LtoC_SetAttackMode*)Data;
	This->R_SetAttackMode( PG->GItemID , PG->Flag );
}
void NetCli_Attack::_PG_Attack_LtoC_NPCAttackTarget		( int Sockid , int Size , void* Data )
{
	PG_Attack_LtoC_NPCAttackTarget*  PG = (PG_Attack_LtoC_NPCAttackTarget*)Data;
	This->R_NPCAttackTarget( PG->NpcGUID , PG->TargetGUID );
}
void NetCli_Attack::_PG_Attack_LtoC_PartyMemberHPMPSP   ( int Sockid , int Size , void* Data )
{
	PG_Attack_LtoC_PartyMemberHPMPSP*  PG = (PG_Attack_LtoC_PartyMemberHPMPSP*)Data;
	This->R_PartyMemberHPMPSP( PG->DBID , PG->HP , PG->MP , PG->SP , PG->SP_Sub );
}
void NetCli_Attack::_PG_Attack_LtoC_PartyMemberMaxHPMPSP( int Sockid , int Size , void* Data )
{
	PG_Attack_LtoC_PartyMemberMaxHPMPSP*  PG = (PG_Attack_LtoC_PartyMemberMaxHPMPSP*)Data;
	This->R_PartyMemberMaxHPMPSP( PG->DBID , PG->MaxHP , PG->MaxMP , PG->MaxSP , PG->MaxSP_Sub );
}

void NetCli_Attack::_PG_Attack_LtoC_AttackResult  ( int Sockid , int Size , void* Data )           
{
    PG_Attack_LtoC_AttackResult*  PG = (PG_Attack_LtoC_AttackResult*)Data;
    This->R_AttackResult( PG->AttackID , PG->UnderAttackID , PG->DamageHP , PG->OrgDamageHP , PG->Type , PG->HandType );
}
void NetCli_Attack::_PG_Attack_LtoC_Dead          ( int Sockid , int Size , void* Data )
{
    PG_Attack_LtoC_Dead*  PG = (PG_Attack_LtoC_Dead*)Data;
    This->R_Dead( PG->DeadID , PG->KillerID , PG->KillerOwnerID , PG->Time , PG->Pos , PG->ReviveTime , PG->Mode );
}
void NetCli_Attack::_PG_Attack_LtoC_Resurrection  ( int Sockid , int Size ,void* Data )
{
    PG_Attack_LtoC_Resurrection *  PG = (PG_Attack_LtoC_Resurrection *)Data;
    This->R_Resurrection( PG->ItemID );
}
void NetCli_Attack::_PG_Attack_LtoC_HPMPSP          ( int Sockid , int Size , void* Data )
{
    PG_Attack_LtoC_HPMP*  PG = (PG_Attack_LtoC_HPMP*)Data;
    This->R_HPMPSP( PG->ItemID , PG->HP , PG->MP , PG->SP , PG->SP_Sub );
}
void NetCli_Attack::_PG_Attack_LtoC_MaxHPMaxMPMaxSP    ( int Sockid , int Size , void* Data )
{
    PG_Attack_LtoC_MaxHPMaxMP*  PG = (PG_Attack_LtoC_MaxHPMaxMP*)Data;
    This->R_MaxHPMaxMPMaxSP( PG->ItemID , PG->MaxHP , PG->MaxMP , PG->MaxSP , PG->MaxSP_Sub );
}
void NetCli_Attack::_PG_Attack_LtoC_AttackSignalResult    ( int Sockid , int Size , void* Data )
{
    PG_Attack_LtoC_AttackSignalResult*  PG = (PG_Attack_LtoC_AttackSignalResult*)Data;
    This->R_AttackSignalResult( PG->TargetID , PG->Result );
}
//-------------------------------------------------------------------
void NetCli_Attack::S_AttackSignal              ( int TargetID )
{
    PG_Attack_CtoL_AttackSignal Send;
    Send.TargetID = TargetID;
    SendToLocal( sizeof(Send) , &Send );
}
//-------------------------------------------------------------------
void NetCli_Attack::S_EndAttackSignal           ()
{
    PG_Attack_CtoL_EndAttackSignal Send;
    SendToLocal( sizeof(Send) , &Send );
}
//-------------------------------------------------------------------
void NetCli_Attack::S_RequestResurrection       ( RequestResurrection_ENUM Type )
{
    PG_Attack_CtoL_RequestResurrection  Send;    
	Send.Type = Type;
    SendToLocal( sizeof(Send) , &Send );
}
//-------------------------------------------------------------------