#pragma once

#include "PG/PG_CultivatePet.h"
#include "../../MainProc/NetGlobal.h"

class NetCli_CultivatePet : public CNetGlobal
{
protected:
    //-------------------------------------------------------------------
    static	NetCli_CultivatePet* This;
    static	bool _Init();
    static	bool _Release();	

	static void _PG_CultivatePet_LtoC_PushPetResult		( int Sockid , int Size , void* Data );
	static void _PG_CultivatePet_LtoC_PopPetResult		( int Sockid , int Size , void* Data );
	static void _PG_CultivatePet_LtoC_PetNameResult		( int Sockid , int Size , void* Data );
	static void _PG_CultivatePet_LtoC_PetEventResult	( int Sockid , int Size , void* Data );	
	static void _PG_CultivatePet_LtoC_LearnSkillResult	( int Sockid , int Size , void* Data );
	static void _PG_CultivatePet_LtoC_PetLifeResult		( int Sockid , int Size , void* Data );
	static void _PG_CultivatePet_LtoC_SwapItemResult	( int Sockid , int Size , void* Data );	
	static void _PG_CultivatePet_LtoC_FixItem			( int Sockid , int Size , void* Data );	
	static void _PG_CultivatePet_LtoC_FixPetBase		( int Sockid , int Size , void* Data );	
	static void _PG_CultivatePet_LtoC_GetProductResult	( int Sockid , int Size , void* Data );	
	static void _PG_CultivatePet_LtoC_PetEventProcess	( int Sockid , int Size , void* Data );	
	static void _PG_CultivatePet_LtoC_PetMargeResult	( int Sockid , int Size , void* Data );	
	static void _PG_CultivatePet_LtoC_RegPetCardResult	( int Sockid , int Size , void* Data );	
	static bool	s_sendPetLocked;
	static bool	s_resetNameLocked;
public:    
    NetCli_CultivatePet() { This = this; }
	static	NetCli_CultivatePet*	GetThis()						{ return This; }

	virtual void RL_PushPetResultOK		( int PetPos , CultivatePetStruct& PetInfo ) = 0;
	virtual void RL_PushPetResultFailed	( ) = 0;
	virtual void RL_PopPetResult		( bool Result , int ItemPos ) = 0;
	virtual void RL_PetNameResult		( bool Result , const char* Name ) = 0;
	virtual void RL_PetEventResult		( bool Result , int PetPos , CultivatePetCommandTypeENUM OrgEvent , CultivatePetCommandTypeENUM Event , int EventTime ) = 0;
	virtual void RL_LearnSkillResult	( bool Result , int PetPos , int MagicID , int MagicLv ) = 0;
	virtual void RL_PetLifeResult		( bool Result , CultivatePetLifeSkillTypeENUM SkillType , int PetPos , int EventTime ) = 0;
	virtual void RL_SwapItemResult		( bool Result ) = 0;
	virtual void RL_FixItem				( int PetPos , int PetItemPos , ItemFieldStruct& Item ) = 0;
	virtual void RL_FixPetBase			( int PetPos , CultivatePetStruct& PetBase ) = 0;
	virtual void RL_GetProductResult	( bool Result ) = 0;
	virtual void RL_PetEventProcess		( int PetPos , CultivatePetCommandTypeENUM Event ) = 0; 

	static void SL_PushPet				( int PetPos , int ItemPos );
	static void SL_PopPet				( int PetPos , int ItemPos );
	static void SL_PetName				( int PetPos , const char* Name );
	static void SL_PetEvent				( int PetPos , int ItemPos , CultivatePetCommandTypeENUM Event );
	static void SL_LearnSkill			( int PetPos , int MagicID , int MagicLv );
	static void SL_PetLifeSkill			( int PetPos , CultivatePetLifeSkillTypeENUM SkillType , int Lv , int TablePos );
	static void SL_SwapItem				( int PetPos , int BodyPos , CultivatePetEQTypeENUM PetItemPos , bool IsFromBody );
	static void SL_GetProductRequest	( int PetPos );

	//////////////////////////////////////////////////////////////////////////
	//寵物融合 
	static void SL_PetMarge				( int PetBasePos , int PetPos1 , int PetPos2 = -1 );
	virtual void RL_PetMargeResult		( PetMargeResultENUM Result , int PetCount , int PetPos[3] , CultivatePetStruct& PetBase ) = 0;
	//////////////////////////////////////////////////////////////////////////
	//登入寵物卡片
	static void SL_RegPetCard( int petPos );
	//登入寵物卡片
	virtual void RL_RegPetCardResult( bool result ) = 0;
};

