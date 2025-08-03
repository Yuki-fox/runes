#pragma once

#include <string>
#include "NetCli_Attack.h"

using namespace std;

class NetCli_AttackChild : public NetCli_Attack
{
public:
	static bool _Init();
	static bool _Release();

	virtual void R_AttackSignalResult	( int TargetID , AttackSignalResultENUM Result );
	virtual void R_AttackResult			( int AttackID , int UnderAttackID , int DamageHP , int OrgDamage , ATTACK_DMGTYPE_ENUM	Type , AttackHandTypeENUM HandType );
	virtual void R_Dead					( int DeaderID , int KillerID , int KillerOwnerID , int Time , RolePosStruct& Pos , int ReviveTime , DeadOptionMode Mode );
	virtual void R_Resurrection			( int ItemID );
	virtual void R_HPMPSP				( int ItemID , int HP , int MP , int SP , int SP_Sub );
	virtual void R_MaxHPMaxMPMaxSP		( int ItemID , int MaxHP , int MaxMP , int MaxSP , int MaxSP_Sub );
	virtual void R_PartyMemberHPMPSP	( int DBID , int HP , int MP , int SP , int SP_Sub);
	virtual void R_PartyMemberMaxHPMPSP ( int DBID , int MaxHP , int MaxMP , int MaxSP , int MaxSP_Sub );
	virtual void R_NPCAttackTarget		( int NpcGUID , int TargetGUID );
	virtual void R_SetAttackMode		( int GItemID , bool Flag );
	virtual void R_ZonePKFlag			( PKTypeENUM PKType );
};



