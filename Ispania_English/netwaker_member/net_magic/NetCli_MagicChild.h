#pragma once

#include "NetCli_Magic.h"

class NetCli_MagicChild : public NetCli_Magic
{
public:    
	static bool _Init();
	static bool _Release();

	virtual void R_BeginSpell         	( int OwnerID , int TargetID , float TargetX , float TargetY , float TargetZ , int MagicID , int SerialID , int SpellTime );
	virtual void R_EndSpell           	( int Serial );
	virtual void R_MagicEnd				( int MagicID , int Coldown_Normal , int Coldown_All );
	virtual void R_BeginSpellOK       	( int MagicID , int SerialID , int SpellTime , int ShootAngle );
	virtual void R_BeginSpellFailed   	( int MagicID , int SerialID );

	virtual void R_MagicAttack        	( int MagicBaseID , int SerialID , MagicAttackTypeENUM	AtkType , vector< MagicAtkEffectInfoStruct >& List );
	virtual void R_MagicAssist        	( int MagicBaseID , int SerialID , vector< MagicAssistEffectInfoStruct >& List );

	virtual void R_FixAssistMagicTime 	( int MagicID , int EffectTime , int BuffOwnerID );
	virtual void R_CancelSpellMagic   	( int OwnerID , int SerialID );
	virtual void R_SetMagicPointResult	( int MagicID , int MagicPos , SetMagicPointResultCodeENUM Result );

	virtual void R_AddBuffInfo			( int OwnerID, int SpellerID, int MagicID , int MagicLv, int EffectTime );
	virtual void R_ModifyBuffInfo			( int OwnerID, int SpellerID, int MagicID , int MagicLv, int EffectTime );
	virtual void R_DelBuffInfo			( int OwnerID , int MagicID , int BuffOwnerID );
	virtual void R_ResetColdown			( ResetColdownTypeENUM Type );

	virtual void R_BeginShoot			( int SerialID , int TargetCount , int TargetGUIDList[_DEF_MAX_MAGIC_TARGET_COUNT_] );
	virtual void R_EndShoot				( int SerialID );
	virtual void R_SysBeginShoot		( int OwnerID , int TargetID , int MagicColID , int SerialID , float TX , float TY , float TZ );
	virtual void R_SysEndShoot			( int SerialID );

	virtual void R_MagicShieldInfo( int AttackGUID , int UnAttackGUID , int AttackMagicID , int ShieldMagicID , int AbsobDamage );


	virtual void R_CreatePet			( int PetGUID , int PetWorldGUID , int PetItemID , int MagicBaseID , SummonPetTypeENUM	PetType );
	virtual void R_DeletePet			(  int PetGUID , SummonPetTypeENUM PetType );
	virtual void R_PetMagicSwitchResult	( int PetGUID , SummonPetTypeENUM PetType , int ActiveMagicID , bool Active );
	virtual void R_CreatePet_Range		( int PetGUID );

	virtual void R_RaiseMagic			( int SpellerID , int MagicBaseID );

	virtual void R_BuffSkillResult		( bool Result  );
	virtual void R_PartyMemberBuff		( int MemberDBID , int Count , MemberBuffInfoStruct Buff[_MAX_BUFF_COUNT_] );

	static	void CombatLog				(string combatStr, string name_s, string name_t, string spellName, MagicAttackTypeENUM atkType, int dmg, int absorb);	
};

