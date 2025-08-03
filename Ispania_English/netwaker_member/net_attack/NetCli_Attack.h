#pragma once

#include "PG/PG_Attack.h"
#include "../../MainProc/NetGlobal.h"

class NetCli_Attack : public CNetGlobal
{
protected:
    //-------------------------------------------------------------------
    static NetCli_Attack* This;
    static bool _Init();
    static bool _Release();
    //-------------------------------------------------------------------

    static void _PG_Attack_LtoC_AttackResult        ( int Sockid , int Size , void* Data );            
    static void _PG_Attack_LtoC_Dead                ( int Sockid , int Size , void* Data );
    static void _PG_Attack_LtoC_Resurrection        ( int Sockid , int Size , void* Data );
    static void _PG_Attack_LtoC_HPMPSP              ( int Sockid , int Size , void* Data );
    static void _PG_Attack_LtoC_MaxHPMaxMPMaxSP     ( int Sockid , int Size , void* Data );
    static void _PG_Attack_LtoC_AttackSignalResult  ( int Sockid , int Size , void* Data );

	static void _PG_Attack_LtoC_PartyMemberHPMPSP   ( int Sockid , int Size , void* Data );
    static void _PG_Attack_LtoC_PartyMemberMaxHPMPSP( int Sockid , int Size , void* Data );
	static void _PG_Attack_LtoC_NPCAttackTarget		( int Sockid , int Size , void* Data );
	static void _PG_Attack_LtoC_SetAttackMode		( int Sockid , int Size , void* Data );
	static void _PG_Attack_LtoC_ZonePKFlag			( int Sockid , int Size , void* Data );

public:    
    NetCli_Attack() { This = this; }

    virtual void R_AttackSignalResult	( int TargetID , AttackSignalResultENUM Result ) = 0;
    virtual void R_AttackResult 		( int AttackID , int UnderAttackID , int DamageHP , int OrgDamage , ATTACK_DMGTYPE_ENUM	Type , AttackHandTypeENUM HandType ) = 0;
    virtual void R_Dead         		( int DeaderID , int KillerID , int KillerOwnerID , int Time , RolePosStruct& Pos , int ReviveTime , DeadOptionMode Mode ) = 0;
    virtual void R_Resurrection 		( int ItemID ) = 0;
    virtual void R_HPMPSP				( int ItemID , int HP , int MP , int SP , int SP_Sub ) = 0;
    virtual void R_MaxHPMaxMPMaxSP		( int ItemID , int MaxHP , int MaxMP , int MaxSP , int MaxSP_Sub ) = 0;
	virtual void R_PartyMemberHPMPSP	( int DBID , int HP , int MP , int SP , int SP_Sub ) = 0;
	virtual void R_PartyMemberMaxHPMPSP ( int DBID , int MaxHP , int MaxMP , int MaxSP , int SP_MaxSP ) = 0;
	virtual void R_NPCAttackTarget		( int NpcGUID , int TargetGUID ) = 0;
	virtual void R_SetAttackMode		( int GItemID , bool Flag ) = 0;
	virtual void R_ZonePKFlag			( PKTypeENUM PKType ) = 0;

    static void S_AttackSignal              ( int TargetID );
    static void S_EndAttackSignal           ( );
    static void S_RequestResurrection       ( RequestResurrection_ENUM Type );

	

};

