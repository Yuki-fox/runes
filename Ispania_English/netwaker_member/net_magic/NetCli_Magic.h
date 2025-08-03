#pragma once

#include "PG/PG_Magic.h"
#include "../../MainProc/NetGlobal.h"

class NetCli_Magic : public CNetGlobal
{
protected:
    //-------------------------------------------------------------------
    static NetCli_Magic* This;
    static bool _Init();
    static bool _Release();
    //-------------------------------------------------------------------
    static void _PG_Magic_LtoC_BeginSpell         	( int Sockid , int Size , void* Data );
    static void _PG_Magic_LtoC_EndSpell           	( int Sockid , int Size , void* Data );
    static void _PG_Magic_LtoC_BeginSpellResult   	( int Sockid , int Size , void* Data );
    static void _PG_Magic_LtoC_MagicAttack        	( int Sockid , int Size , void* Data );
    static void _PG_Magic_LtoC_MagicAssistResult  	( int Sockid , int Size , void* Data );
    static void _PG_Magic_LtoC_FixAssistMagicTime 	( int Sockid , int Size , void* Data );
    static void _PG_Magic_LtoC_CancelSpellMagic   	( int Sockid , int Size , void* Data );
    static void _PG_Magic_LtoC_SetMagicPointResult	( int Sockid , int Size , void* Data );
	static void _PG_Magic_LtoC_AddBuffInfo			( int Sockid , int Size , void* Data );
	static void _PG_Magic_LtoC_ModifyBuffInfo		( int Sockid , int Size , void* Data );
	static void _PG_Magic_LtoC_DelBuffInfo			( int Sockid , int Size , void* Data );
	static void _PG_Magic_LtoC_ResetColdown			( int Sockid , int Size , void* Data );

	static void _PG_Magic_LtoC_BeginShoot			( int Sockid , int Size , void* Data );
	static void _PG_Magic_LtoC_EndShoot				( int Sockid , int Size , void* Data );
	static void _PG_Magic_LtoC_SysBeginShoot		( int Sockid , int Size , void* Data );
	static void _PG_Magic_LtoC_SysEndShoot			( int Sockid , int Size , void* Data );
	static void _PG_Magic_LtoC_MagicShieldInfo		( int Sockid , int Size , void* Data );

	static void _PG_Magic_LtoC_MagicEnd				( int Sockid , int Size , void* Data );

	static void _PG_Magic_LtoC_CreatePet			( int Sockid , int Size , void* Data );
	static void _PG_Magic_LtoC_DeletePet			( int Sockid , int Size , void* Data );
	static void _PG_Magic_LtoC_PetMagicSwitchResult	( int Sockid , int Size , void* Data );

	static void _PG_Magic_LtoC_RaiseMagic			( int Sockid , int Size , void* Data );
	static void _PG_Magic_LtoC_BuffSkillResult		( int Sockid , int Size , void* Data );
	static void _PG_Magic_LtoC_PartyMemberBuff		( int Sockid , int Size , void* Data );
	static void _PG_Magic_LtoC_CreatePet_Range		( int Sockid , int Size , void* Data );
public:    
    NetCli_Magic() { This = this; }

    static void BeginSpell			( int OwnerID , int TargetID , int MagicID , int MagicPos );
	static void BeginSpell_Pos		( int OwnerID , float X , float Y , float Z , int MagicID , int MagicPos );
    static void SetMagicPoint		( int MagicID , int MagicPos );
	static void CancelAssistMagic	( int MagicID );
	static void CancelSpellMagic	( );
	static void PetCommand			( int PetGUID , PET_ACTION_ENUM Action , int TargetID , int MagicID );
	static void PetMagicSwitch		( int PetGUID , int ActiveMagicID , bool Active );

	static void SL_BuffSkillRequest	( int BuffID , int BuffSkillID ,  int TargetID , float TargetX , float TargetY , float TargetZ  );

    virtual void R_BeginSpell         ( int OwnerID , int TargetID , float X , float Y , float Z , int MagicID , int SerialID , int SpellTime ) = 0;
    virtual void R_EndSpell           ( int SerailID ) = 0;
	virtual void R_MagicEnd			  ( int MagicID , int Coldown_Normal , int Coldown_All ) = 0;
    virtual void R_BeginSpellOK       ( int MagicID , int SerialID , int SpellTime , int ShootAngle ) = 0;
    virtual void R_BeginSpellFailed   ( int MagicID , int SerialID ) = 0;
    virtual void R_MagicAttack        ( int MagicBaseID , int Serial , MagicAttackTypeENUM	AtkType , vector< MagicAtkEffectInfoStruct >& List ) = 0;
    virtual void R_MagicAssist        ( int MagicBaseID , int Serial , vector< MagicAssistEffectInfoStruct >& List ) = 0;

	virtual void R_ResetColdown		  ( ResetColdownTypeENUM Type ) = 0;
    virtual void R_FixAssistMagicTime ( int MagicID , int EffectTime , int BuffOwnerID ) = 0;
    virtual void R_CancelSpellMagic   ( int OwnerID , int SerialID ) = 0;
    virtual void R_SetMagicPointResult( int MagicID , int MagicPos , SetMagicPointResultCodeENUM Result ) = 0;

	virtual void R_AddBuffInfo( int OwnerID, int SpellerID , int MagicID , int MagicLv , int EffectTime ) = 0;
	virtual void R_ModifyBuffInfo( int OwnerID, int SpellerID , int MagicID , int MagicLv , int EffectTime ) = 0;
	virtual void R_DelBuffInfo( int OwnerID, int MagicID , int BuffOwnerID ) = 0;

	virtual void R_BeginShoot	( int SerialID , int TargetCount , int TargetGUIDList[_DEF_MAX_MAGIC_TARGET_COUNT_] ) = 0;
	virtual void R_EndShoot		( int SerialID ) = 0;
	virtual void R_SysBeginShoot( int OwnerID , int TargetID , int MagicColID , int SerialID , float TX , float TY , float TZ ) = 0;
	virtual void R_SysEndShoot	( int SerialID ) = 0;

	virtual void R_MagicShieldInfo( int AttackGUID , int UnAttackGUID , int AttackMagicID , int ShieldMagicID , int AbsobDamage ) = 0;

	virtual void R_CreatePet			( int PetGUID , int PetWorldGUID , int PetItemID , int MagicBaseID , SummonPetTypeENUM	PetType ) = 0;
	virtual void R_DeletePet			( int PetGUID , SummonPetTypeENUM PetType ) = 0;
	virtual void R_PetMagicSwitchResult	( int PetGUID , SummonPetTypeENUM PetType , int ActiveMagicID , bool Active ) = 0;
	virtual void R_CreatePet_Range		( int PetGUID ) = 0;
	
	virtual void R_RaiseMagic			( int SpellerID , int MagicBaseID ) = 0;

	virtual void R_BuffSkillResult		( bool Result  ) = 0;
	virtual void R_PartyMemberBuff		( int MemberDBID , int Count , MemberBuffInfoStruct Buff[_MAX_BUFF_COUNT_] ) = 0;
};

