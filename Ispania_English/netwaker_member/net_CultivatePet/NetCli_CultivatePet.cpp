#include "NetCli_CultivatePet.h"

//-------------------------------------------------------------------
NetCli_CultivatePet*    NetCli_CultivatePet::This				= NULL;
bool					NetCli_CultivatePet::s_sendPetLocked	= false;
bool					NetCli_CultivatePet::s_resetNameLocked	= false;
//-------------------------------------------------------------------
bool NetCli_CultivatePet::_Init()
{
	Cli_NetReg( PG_CultivatePet_LtoC_PushPetResult		);
	Cli_NetReg( PG_CultivatePet_LtoC_PopPetResult		);
	Cli_NetReg( PG_CultivatePet_LtoC_PetNameResult		);
	Cli_NetReg( PG_CultivatePet_LtoC_PetEventResult		);
	Cli_NetReg( PG_CultivatePet_LtoC_LearnSkillResult	);
	Cli_NetReg( PG_CultivatePet_LtoC_PetLifeResult		);
	Cli_NetReg( PG_CultivatePet_LtoC_SwapItemResult		);
	Cli_NetReg( PG_CultivatePet_LtoC_FixItem			);
	Cli_NetReg( PG_CultivatePet_LtoC_GetProductResult	);
	Cli_NetReg( PG_CultivatePet_LtoC_PetEventProcess	);

	Cli_NetReg( PG_CultivatePet_LtoC_PetMargeResult		);
	Cli_NetReg( PG_CultivatePet_LtoC_RegPetCardResult 	);
    return true;
}
//-------------------------------------------------------------------
bool NetCli_CultivatePet::_Release()
{
    return true;
}
//-------------------------------------------------------------------
void NetCli_CultivatePet::_PG_CultivatePet_LtoC_RegPetCardResult( int Sockid , int Size , void* Data )
{
	PG_CultivatePet_LtoC_RegPetCardResult* pg = (PG_CultivatePet_LtoC_RegPetCardResult*)Data;
	This->RL_RegPetCardResult( pg->Result );
}
void NetCli_CultivatePet::_PG_CultivatePet_LtoC_PetMargeResult	( int Sockid , int Size , void* Data )
{
	PG_CultivatePet_LtoC_PetMargeResult* PG = (PG_CultivatePet_LtoC_PetMargeResult*)Data;
	This->RL_PetMargeResult( PG->Result , PG->PetCount , PG->PetPos , PG->PetBase );
}
void NetCli_CultivatePet::_PG_CultivatePet_LtoC_PetEventProcess	( int Sockid , int Size , void* Data )
{
	PG_CultivatePet_LtoC_PetEventProcess* PG = (PG_CultivatePet_LtoC_PetEventProcess*)Data;
	This->RL_PetEventProcess( PG->PetPos , PG->Event );
}
void NetCli_CultivatePet::_PG_CultivatePet_LtoC_GetProductResult	( int Sockid , int Size , void* Data )
{
	PG_CultivatePet_LtoC_GetProductResult* PG = (PG_CultivatePet_LtoC_GetProductResult*)Data;
	This->RL_GetProductResult( PG->Result );
}
void NetCli_CultivatePet::_PG_CultivatePet_LtoC_FixPetBase		( int Sockid , int Size , void* Data )
{
	PG_CultivatePet_LtoC_FixPetBase* PG = (PG_CultivatePet_LtoC_FixPetBase*)Data;
	This->RL_FixPetBase( PG->PetPos , PG->PetBase );
	
}
void NetCli_CultivatePet::_PG_CultivatePet_LtoC_PushPetResult	( int Sockid , int Size , void* Data )
{
	s_sendPetLocked = false;
	PG_CultivatePet_LtoC_PushPetResult* PG = (PG_CultivatePet_LtoC_PushPetResult*)Data;
	if( PG->Result == true )
		This->RL_PushPetResultOK( PG->PetPos , PG->PetInfo );
	else 
		This->RL_PushPetResultFailed( );

}
void NetCli_CultivatePet::_PG_CultivatePet_LtoC_PopPetResult	( int Sockid , int Size , void* Data )
{
	PG_CultivatePet_LtoC_PopPetResult* PG = (PG_CultivatePet_LtoC_PopPetResult*)Data;
	This->RL_PopPetResult( PG->Result , PG->ItemPos );

}
void NetCli_CultivatePet::_PG_CultivatePet_LtoC_PetNameResult	( int Sockid , int Size , void* Data )
{
	s_resetNameLocked = false;

	PG_CultivatePet_LtoC_PetNameResult* PG = (PG_CultivatePet_LtoC_PetNameResult*)Data;
	This->RL_PetNameResult( PG->Result , PG->Name.Begin() );
}
void NetCli_CultivatePet::_PG_CultivatePet_LtoC_PetEventResult	( int Sockid , int Size , void* Data )
{
	PG_CultivatePet_LtoC_PetEventResult* PG = (PG_CultivatePet_LtoC_PetEventResult*)Data;
	This->RL_PetEventResult( PG->Result , PG->PetPos , PG->OrgEvent , PG->Event , PG->EventTime );
}
void NetCli_CultivatePet::_PG_CultivatePet_LtoC_LearnSkillResult( int Sockid , int Size , void* Data )
{
	PG_CultivatePet_LtoC_LearnSkillResult* PG = (PG_CultivatePet_LtoC_LearnSkillResult*)Data;
	This->RL_LearnSkillResult( PG->Result , PG->PetPos , PG->MagicID , PG->MagicLv );
}
void NetCli_CultivatePet::_PG_CultivatePet_LtoC_PetLifeResult	( int Sockid , int Size , void* Data )
{
	PG_CultivatePet_LtoC_PetLifeResult* PG = (PG_CultivatePet_LtoC_PetLifeResult*)Data;
	This->RL_PetLifeResult( PG->Result , PG->SkillType , PG->PetPos , PG->EventTime );
}
void NetCli_CultivatePet::_PG_CultivatePet_LtoC_SwapItemResult	( int Sockid , int Size , void* Data )
{
	PG_CultivatePet_LtoC_SwapItemResult* PG = (PG_CultivatePet_LtoC_SwapItemResult*)Data;
	This->RL_SwapItemResult( PG->Result );
}
void NetCli_CultivatePet::_PG_CultivatePet_LtoC_FixItem			( int Sockid , int Size , void* Data )
{
	PG_CultivatePet_LtoC_FixItem* PG = (PG_CultivatePet_LtoC_FixItem*)Data;
	This->RL_FixItem( PG->PetPos , PG->PetItemPos , PG->Item );
}


void NetCli_CultivatePet::SL_PushPet		( int PetPos , int ItemPos )
{
	if ( s_sendPetLocked )
		return;
	s_sendPetLocked = true;

	PG_CultivatePet_CtoL_PushPet Send;
	Send.PetPos = PetPos;
	Send.ItemPos = ItemPos;
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_CultivatePet::SL_PopPet			( int PetPos , int ItemPos )
{
	PG_CultivatePet_CtoL_PopPet Send;
	Send.PetPos = PetPos;
	Send.ItemPos = ItemPos;
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_CultivatePet::SL_PetName		( int PetPos , const char* Name )
{
	if ( s_resetNameLocked )
		return;
	s_resetNameLocked = true;

	PG_CultivatePet_CtoL_PetName Send;
	Send.PetPos = PetPos;
	Send.Name = Name;
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_CultivatePet::SL_PetEvent		( int PetPos , int ItemPos , CultivatePetCommandTypeENUM Event )
{
	PG_CultivatePet_CtoL_PetEvent Send;
	Send.PetPos = PetPos;
	Send.ItemPos = ItemPos;
	Send.Event = Event;
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_CultivatePet::SL_LearnSkill		( int PetPos , int MagicID , int MagicLv )
{
	PG_CultivatePet_CtoL_LearnSkill Send;
	Send.PetPos = PetPos;
	Send.MagicID = MagicID;
	Send.MagicLv = MagicLv;
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_CultivatePet::SL_PetLifeSkill	( int PetPos , CultivatePetLifeSkillTypeENUM SkillType , int Lv , int TablePos )
{
	PG_CultivatePet_CtoL_PetLifeSkill Send;
	Send.PetPos	= PetPos;
	Send.SkillType = SkillType;
	Send.Lv = Lv;
	Send.TablePos = TablePos;
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_CultivatePet::SL_SwapItem		( int PetPos , int BodyPos , CultivatePetEQTypeENUM PetItemPos , bool IsFromBody )
{
	PG_CultivatePet_CtoL_SwapItem Send;
	Send.IsFromBody = IsFromBody;
	Send.PetPos = PetPos;
	Send.BodyPos = BodyPos;
	Send.PetItemPos = PetItemPos;
	SendToLocal( sizeof(Send) , &Send );
}

void NetCli_CultivatePet::SL_GetProductRequest	( int PetPos )
{
	PG_CultivatePet_CtoL_GetProductRequest Send;
	Send.PetPos = PetPos;

	SendToLocal( sizeof(Send) , &Send );
}

void NetCli_CultivatePet::SL_PetMarge			( int PetBasePos , int PetPos1 , int PetPos2 )
{
	PG_CultivatePet_CtoL_PetMarge Send;
	if( PetPos2 == -1 )
		Send.PetCount = 2;
	else
		Send.PetCount = 3;
	Send.PetPos[0] = PetBasePos;
	Send.PetPos[1] = PetPos1;
	Send.PetPos[2] = PetPos2;

	SendToLocal( sizeof(Send) , &Send );
}

void NetCli_CultivatePet::SL_RegPetCard( int petPos )
{
	PG_CultivatePet_CtoL_RegPetCard Send;
	Send.PetPos = petPos;
	SendToLocal( sizeof(Send) , &Send );
}
