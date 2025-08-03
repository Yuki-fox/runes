#pragma once

#include "NetCli_CultivatePet.h"

class NetCli_CultivatePetChild : public NetCli_CultivatePet
{
public:	
	static	bool _Init();
	static	bool _Release();

public:

	virtual void RL_PushPetResultOK		( int PetPos , CultivatePetStruct& PetInfo );
	virtual void RL_PushPetResultFailed	( );
	virtual void RL_PopPetResult		( bool Result , int ItemPos );
	virtual void RL_PetNameResult		( bool Result , const char* Name );
	virtual void RL_PetEventResult		( bool Result , int PetPos , CultivatePetCommandTypeENUM OrgEvent , CultivatePetCommandTypeENUM Event , int EventTime );
	virtual void RL_LearnSkillResult	( bool Result , int PetPos , int MagicID , int MagicLv );
	virtual void RL_PetLifeResult		( bool Result , CultivatePetLifeSkillTypeENUM SkillType , int PetPos , int EventTime );
	virtual void RL_SwapItemResult		( bool Result );
	virtual void RL_FixItem				( int PetPos , int PetItemPos , ItemFieldStruct& Item );
	virtual void RL_FixPetBase			( int PetPos , CultivatePetStruct& PetBase );
	virtual void RL_GetProductResult	( bool Result );
	virtual void RL_PetEventProcess		( int PetPos , CultivatePetCommandTypeENUM Event );
	virtual void RL_PetMargeResult		( PetMargeResultENUM Result , int PetCount , int PetPos[3] , CultivatePetStruct& PetBase );

	virtual void RL_RegPetCardResult	( bool result );
};

