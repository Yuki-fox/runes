#include "NetCli_Magic.h"

//-------------------------------------------------------------------
NetCli_Magic*    NetCli_Magic::This         = NULL;

//-------------------------------------------------------------------
bool NetCli_Magic::_Init()
{
    Cli_NetReg( PG_Magic_LtoC_BeginSpell         	);
    Cli_NetReg( PG_Magic_LtoC_EndSpell           	);
    Cli_NetReg( PG_Magic_LtoC_BeginSpellResult   	);
    Cli_NetReg( PG_Magic_LtoC_MagicAttack        	);
    Cli_NetReg( PG_Magic_LtoC_MagicAssistResult  	);
    Cli_NetReg( PG_Magic_LtoC_FixAssistMagicTime 	);
    Cli_NetReg( PG_Magic_LtoC_CancelSpellMagic   	);
    Cli_NetReg( PG_Magic_LtoC_SetMagicPointResult	);
	Cli_NetReg( PG_Magic_LtoC_AddBuffInfo			);
	Cli_NetReg( PG_Magic_LtoC_ModifyBuffInfo		);
	Cli_NetReg( PG_Magic_LtoC_DelBuffInfo			);
	Cli_NetReg( PG_Magic_LtoC_BeginShoot			);
	Cli_NetReg( PG_Magic_LtoC_EndShoot				);
	Cli_NetReg( PG_Magic_LtoC_SysBeginShoot			);
	Cli_NetReg( PG_Magic_LtoC_SysEndShoot			);
	Cli_NetReg( PG_Magic_LtoC_MagicShieldInfo		);
	Cli_NetReg( PG_Magic_LtoC_MagicEnd				);
	Cli_NetReg( PG_Magic_LtoC_CreatePet				);
	Cli_NetReg( PG_Magic_LtoC_DeletePet				);
	Cli_NetReg( PG_Magic_LtoC_PetMagicSwitchResult	);
	Cli_NetReg( PG_Magic_LtoC_RaiseMagic			);
	Cli_NetReg( PG_Magic_LtoC_ResetColdown			);
	Cli_NetReg( PG_Magic_LtoC_BuffSkillResult		);
	Cli_NetReg( PG_Magic_LtoC_PartyMemberBuff		);
	Cli_NetReg( PG_Magic_LtoC_CreatePet_Range		);
    return true;
}
//-------------------------------------------------------------------
bool NetCli_Magic::_Release()
{
    return true;
}
//-------------------------------------------------------------------
void NetCli_Magic::_PG_Magic_LtoC_CreatePet_Range		( int Sockid , int Size , void* Data )
{
	PG_Magic_LtoC_CreatePet_Range* PG = (PG_Magic_LtoC_CreatePet_Range*)Data;
	This->R_CreatePet_Range( PG->PetGUID );
}
void NetCli_Magic::_PG_Magic_LtoC_PartyMemberBuff		( int Sockid , int Size , void* Data )
{
	PG_Magic_LtoC_PartyMemberBuff* PG = (PG_Magic_LtoC_PartyMemberBuff*)Data;
	This->R_PartyMemberBuff( PG->MemberDBID , PG->Count , PG->Buff  );
}

void NetCli_Magic::_PG_Magic_LtoC_BuffSkillResult		( int Sockid , int Size , void* Data )
{
	PG_Magic_LtoC_BuffSkillResult* PG = (PG_Magic_LtoC_BuffSkillResult*)Data;
	This->R_BuffSkillResult( PG->Result  );
}
void NetCli_Magic::_PG_Magic_LtoC_RaiseMagic		( int Sockid , int Size , void* Data )
{
	PG_Magic_LtoC_RaiseMagic* PG = (PG_Magic_LtoC_RaiseMagic*)Data;
	This->R_RaiseMagic( PG->SpellerID , PG->MagicBaseID );
}

void NetCli_Magic::_PG_Magic_LtoC_CreatePet			( int Sockid , int Size , void* Data )
{
	PG_Magic_LtoC_CreatePet* PG = (PG_Magic_LtoC_CreatePet*)Data;
	This->R_CreatePet( PG->PetGUID , PG->PetWorldGUID , PG->PetItemID , PG->MagicBaseID , PG->PetType );
}
void NetCli_Magic::_PG_Magic_LtoC_DeletePet			( int Sockid , int Size , void* Data )
{
	PG_Magic_LtoC_DeletePet* PG = (PG_Magic_LtoC_DeletePet*)Data;
	This->R_DeletePet( PG->PetGUID , PG->PetType  );
}
void NetCli_Magic::_PG_Magic_LtoC_PetMagicSwitchResult	( int Sockid , int Size , void* Data )
{
	PG_Magic_LtoC_PetMagicSwitchResult* PG = (PG_Magic_LtoC_PetMagicSwitchResult*)Data;
	This->R_PetMagicSwitchResult( PG->PetGUID , PG->PetType , PG->ActiveMagicID , PG->Active );
}

void NetCli_Magic::_PG_Magic_LtoC_MagicEnd		( int Sockid , int Size , void* Data )
{
	PG_Magic_LtoC_MagicEnd* PG = (PG_Magic_LtoC_MagicEnd*)Data;
	This->R_MagicEnd( PG->MagicID , PG->Coldown_Normal , PG->Coldown_All );
}


void NetCli_Magic::_PG_Magic_LtoC_MagicShieldInfo		( int Sockid , int Size , void* Data )
{
	PG_Magic_LtoC_MagicShieldInfo* PG = (PG_Magic_LtoC_MagicShieldInfo*)Data;
	This->R_MagicShieldInfo( PG->AttackGUID , PG->UnAttackGUID , PG->AttackMagicID , PG->ShieldMagicID , PG->AbsobDamage );
}
void NetCli_Magic::_PG_Magic_LtoC_ResetColdown			( int Sockid , int Size , void* Data )
{
	PG_Magic_LtoC_ResetColdown* PG = (PG_Magic_LtoC_ResetColdown*)Data;

	This->R_ResetColdown( PG->Type );
}
//-------------------------------------------------------------------
void NetCli_Magic::BeginSpell    ( int OwnerID , int TargetID , int MagicID , int MagicPos )
{
    PG_Magic_CtoL_BeginSpell Send;
    Send.OwnerID = OwnerID;
    Send.TargetID = TargetID;
    Send.MagicID = MagicID;
    Send.MagicPos = MagicPos;

    SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Magic::CancelAssistMagic	  ( int MagicID )
{
	PG_Magic_LtoC_CancelAssistMagic Send;
	Send.MagicID = MagicID;

	SendToLocal( sizeof(Send) , &Send );
}
//-------------------------------------------------------------------
void NetCli_Magic::_PG_Magic_LtoC_BeginShoot			( int Sockid , int Size , void* Data )
{
	PG_Magic_LtoC_BeginShoot* PG = (PG_Magic_LtoC_BeginShoot*)Data;
	This->R_BeginShoot( PG->SerialID , PG->TargetCount , PG->TargetGUIDList );
}
void NetCli_Magic::_PG_Magic_LtoC_EndShoot			( int Sockid , int Size , void* Data )
{
	PG_Magic_LtoC_EndShoot* PG = (PG_Magic_LtoC_EndShoot*)Data;
	This->R_EndShoot( PG->SerialID );
}

void NetCli_Magic::_PG_Magic_LtoC_SysBeginShoot			( int Sockid , int Size , void* Data )
{
	PG_Magic_LtoC_SysBeginShoot* PG = (PG_Magic_LtoC_SysBeginShoot*)Data;
	This->R_SysBeginShoot( PG->OwnerID , PG->TargetID , PG->MagicColID , PG->SerialID , PG->TX , PG->TY , PG->TZ );

}
void NetCli_Magic::_PG_Magic_LtoC_SysEndShoot			( int Sockid , int Size , void* Data )
{
	PG_Magic_LtoC_SysEndShoot* PG = (PG_Magic_LtoC_SysEndShoot*)Data;
	This->R_SysEndShoot( PG->SerialID );
}

void NetCli_Magic::_PG_Magic_LtoC_AddBuffInfo			( int Sockid , int Size , void* Data )
{
	PG_Magic_LtoC_AddBuffInfo* PG = (PG_Magic_LtoC_AddBuffInfo*)Data;
	This->R_AddBuffInfo( PG->GItemID , PG->TargetGItemID , PG->MagicID , PG->MagicLv , PG->EffectTime );
}

void NetCli_Magic::_PG_Magic_LtoC_ModifyBuffInfo			( int Sockid , int Size , void* Data )
{
	PG_Magic_LtoC_ModifyBuffInfo* PG = (PG_Magic_LtoC_ModifyBuffInfo*)Data;
	This->R_ModifyBuffInfo( PG->GItemID , PG->TargetGItemID , PG->MagicID , PG->MagicLv , PG->EffectTime );
}
void NetCli_Magic::_PG_Magic_LtoC_DelBuffInfo			( int Sockid , int Size , void* Data )
{
	PG_Magic_LtoC_DelBuffInfo* PG = (PG_Magic_LtoC_DelBuffInfo*)Data;
	This->R_DelBuffInfo( PG->GItemID , PG->MagicID , PG->BuffOwnerID );
}
//-------------------------------------------------------------------

void NetCli_Magic::_PG_Magic_LtoC_BeginSpell         ( int Sockid , int Size , void* Data )
{
    PG_Magic_LtoC_BeginSpell*  PG = (PG_Magic_LtoC_BeginSpell*)Data;
    This->R_BeginSpell( PG->OwnerID , PG->TargetID , PG->TargetX , PG->TargetY , PG->TargetZ , PG->Magic , PG->SerialID , PG->SpellTime );    
}

void NetCli_Magic::_PG_Magic_LtoC_EndSpell           ( int Sockid , int Size , void* Data )
{
    PG_Magic_LtoC_EndSpell*  PG = (PG_Magic_LtoC_EndSpell*)Data;
    This->R_EndSpell( PG->SerialID );
}
void NetCli_Magic::_PG_Magic_LtoC_BeginSpellResult   ( int Sockid , int Size , void* Data )
{
    PG_Magic_LtoC_BeginSpellResult*  PG = (PG_Magic_LtoC_BeginSpellResult*)Data;
    if( PG->Result == true)
        This->R_BeginSpellOK( PG->MagicID , PG->SerialID , PG->SpellTime , PG->ShootAngle );
    else
        This->R_BeginSpellFailed( PG->MagicID , PG->SerialID );
}
void NetCli_Magic::_PG_Magic_LtoC_MagicAttack        ( int Sockid , int Size , void* Data )
{
    static vector< MagicAtkEffectInfoStruct > List;
    List.clear();

    PG_Magic_LtoC_MagicAttack*  PG = (PG_Magic_LtoC_MagicAttack*)Data;

    for( int i = 0 ; i < PG->Count ; i++ )
    {
        List.push_back( PG->Atk[i] );
    }

    This->R_MagicAttack( PG->MagicBaseID , PG->SerialID , PG->AtkType , List );
}
void NetCli_Magic::_PG_Magic_LtoC_MagicAssistResult  ( int Sockid , int Size , void* Data )
{   
    static vector< MagicAssistEffectInfoStruct > List;
    List.clear();

    PG_Magic_LtoC_MagicAssistResult*  PG = (PG_Magic_LtoC_MagicAssistResult*)Data;

    for( int i = 0 ; i < PG->Count ; i++ )
    {
        List.push_back( PG->Atk[i] );
    }

    This->R_MagicAssist( PG->MagicBaseID , PG->SerialID , List );

}
void NetCli_Magic::_PG_Magic_LtoC_FixAssistMagicTime ( int Sockid , int Size , void* Data )
{
    PG_Magic_LtoC_FixAssistMagicTime*  PG = (PG_Magic_LtoC_FixAssistMagicTime*)Data;
    This->R_FixAssistMagicTime( PG->MagicID , PG->EffectTime , PG->BuffOwnerID );
}
void NetCli_Magic::_PG_Magic_LtoC_CancelSpellMagic   ( int Sockid , int Size , void* Data )
{    
    PG_Magic_LtoC_CancelSpellMagic*  PG = (PG_Magic_LtoC_CancelSpellMagic*)Data;
    This->R_CancelSpellMagic( PG->OwnerID , PG->SerialID );

}
void NetCli_Magic::_PG_Magic_LtoC_SetMagicPointResult( int Sockid , int Size , void* Data )
{
    PG_Magic_LtoC_SetMagicPointResult* PG = (PG_Magic_LtoC_SetMagicPointResult*)Data;
    This->R_SetMagicPointResult( PG->MagicID , PG->MagicPos , PG->Result );
}
void NetCli_Magic::SetMagicPoint ( int MagicID , int MagicPos )
{
    PG_Magic_CtoL_SetMagicPoint Send;
    Send.MagicID  = MagicID;
    Send.MagicPos = MagicPos;

    SendToLocal( sizeof(Send) , &Send );
}

void NetCli_Magic::BeginSpell_Pos		( int OwnerID , float X , float Y , float Z , int MagicID , int MagicPos )
{
	PG_Magic_CtoL_BeginSpell_Pos Send;
	Send.OwnerID = OwnerID;
	Send.X = X;
	Send.Y = Y;
	Send.Z = Z;
	Send.MagicID = MagicID;
	Send.MagicPos = MagicPos;

	SendToLocal( sizeof(Send) , &Send );
}

void NetCli_Magic::CancelSpellMagic	( )
{
	PG_Magic_CtoL_CancelMagicRequest Send;
	SendToLocal( sizeof(Send) , &Send );
}

void NetCli_Magic::PetCommand( int PetGUID , PET_ACTION_ENUM Action , int TargetID , int MagicID )
{
	PG_Magic_CtoL_PetCommand Send;
	Send.Action = Action;
	Send.MagicID = MagicID;
	Send.TargetID = TargetID;
	Send.PetGUID = PetGUID;
	SendToLocal( sizeof(Send) , &Send );
}

void NetCli_Magic::PetMagicSwitch( int PetGUID , int ActiveMagicID , bool Active )
{
	PG_Magic_CtoL_PetMagicSwitchRequest Send;
	Send.ActiveMagicID = ActiveMagicID;
	Send.Active = Active;
	Send.PetGUID = PetGUID;
	SendToLocal( sizeof(Send) , &Send );
}

void NetCli_Magic::SL_BuffSkillRequest	( int BuffID , int BuffSkillID ,  int TargetID 
					, float TargetX , float TargetY , float TargetZ  )
{
	PG_Magic_CtoL_BuffSkillRequest Send;
	Send.BuffID = BuffID;
	Send.BuffSkillID = BuffSkillID;
	Send.TargetID = TargetID;
	Send.PosX = TargetX;
	Send.PosY = TargetY;
	Send.PosZ = TargetZ;
	SendToLocal( sizeof(Send) , &Send );
}