#include "Net_Move.h"
#include "../MainProc/NetGlobal.h"
#include "../ErrorList/errorlist.h"
#pragma unmanaged

Net_Move*  Net_Move::This         = NULL;


bool Net_Move::_Init()
{
	/*
    _Net.RegOnSrvPacketFunction	( EM_PG_Move_LtoC_AddToPartition        	, _PG_Move_LtoC_AddToPartition  );
    _Net.RegOnSrvPacketFunction	( EM_PG_Move_LtoC_DelFromPartition      	, _PG_Move_LtoC_DelFromPartition);
    _Net.RegOnSrvPacketFunction	( EM_PG_Move_LtoC_SetPostion            	, _PG_Move_LtoC_SetPostion      );
    _Net.RegOnSrvPacketFunction	( EM_PG_Move_LtoC_ObjectMove            	, _PG_Move_LtoC_ObjectMove      );
    _Net.RegOnSrvPacketFunction	( EM_PG_Move_LtoC_DoAction              	, _PG_Move_LtoC_DoAction        );
    _Net.RegOnSrvPacketFunction	( EM_PG_Move_LtoC_MoveSpeed             	, _PG_Move_LtoC_MoveSpeed       );
    _Net.RegOnSrvPacketFunction	( EM_PG_Move_LtoC_MoveInfo	             	, _PG_Move_LtoC_MoveInfo       );
    _Net.RegOnSrvPacketFunction	( EM_PG_Move_LtoC_EQInfo                	, _PG_Move_LtoC_EQInfo          );
    _Net.RegOnSrvPacketFunction	( EM_PG_Move_LtoC_FixEQInfo             	, _PG_Move_LtoC_FixEQInfo       );
    _Net.RegOnSrvPacketFunction	( EM_PG_Move_LtoC_LookFace              	, _PG_Move_LtoC_LookFace        );
    _Net.RegOnSrvPacketFunction	( EM_PG_Move_LtoC_ClientAutoPlot        	, _PG_Move_LtoC_ClientAutoPlot  );
    _Net.RegOnSrvPacketFunction	( EM_PG_Move_LtoC_ActionType            	, _PG_Move_LtoC_ActionType      );
    _Net.RegOnSrvPacketFunction	( EM_PG_Move_LtoC_JumpBegin             	, _PG_Move_LtoC_JumpBegin		);
	_Net.RegOnSrvPacketFunction	( EM_PG_Move_LtoC_Relation              	, _PG_Move_LtoC_Relation		);
	_Net.RegOnSrvPacketFunction	( EM_PG_Move_LtoC_PartyMemberMove       	, _PG_Move_LtoC_PartyMemberMove );
	_Net.RegOnSrvPacketFunction	( EM_PG_Move_LtoC_PlayMotion				, _PG_Move_LtoC_PlayMotion		);
	_Net.RegOnSrvPacketFunction	( EM_PG_Move_LtoC_SpecialMove				, _PG_Move_LtoC_SpecialMove		);
	_Net.RegOnSrvPacketFunction	( EM_PG_Move_LtoC_Ping						, _PG_Move_LtoC_Ping			);
	_Net.RegOnSrvPacketFunction	( EM_PG_Move_LtoC_ClientLoading				, _PG_Move_LtoC_ClientLoading	);
	_Net.RegOnSrvPacketFunction	( EM_PG_Move_LtoC_SetMinimapIcon			, _PG_Move_LtoC_SetMinimapIcon	);
	_Net.RegOnSrvPacketFunction	( EM_PG_Move_LtoC_SpecialEfficacy			, _PG_Move_LtoC_SpecialEfficacy	);
	_Net.RegOnSrvPacketFunction	( EM_PG_Move_LtoC_SetDir					, _PG_Move_LtoC_SetDir			);
	_Net.RegOnSrvPacketFunction	( EM_PG_Move_LtoC_SetIdleMotion				, _PG_Move_LtoC_SetIdleMotion	);
	_Net.RegOnSrvPacketFunction	( EM_PG_Move_LtoC_SetGuildID				, _PG_Move_LtoC_SetGuildID		);
	_Net.RegOnSrvPacketFunction	( EM_PG_Move_LtoC_SetTitleID				, _PG_Move_LtoC_SetTitleID		);
	_Net.RegOnSrvPacketFunction	( EM_PG_Move_LtoC_PlayerDropDownDamage		, _PG_Move_LtoC_PlayerDropDownDamage		);
	_Net.RegOnSrvPacketFunction	( EM_PG_Move_LtoC_HorseColor				, _PG_Move_LtoC_HorseColor		);
	_Net.RegOnSrvPacketFunction	( EM_PG_Move_LtoC_ObjGuildFurnitureInfo		, _PG_Move_LtoC_ObjGuildFurnitureInfo		);
	_Net.RegOnSrvPacketFunction	( EM_PG_Move_LtoC_ObjGuildBuildingInfo		, _PG_Move_LtoC_ObjGuildBuildingInfo		);
	_Net.RegOnSrvPacketFunction	( EM_PG_Move_LtoC_RoomID					, _PG_Move_LtoC_RoomID			);
*/
	Cli_NetReg( PG_Move_LtoC_AddToPartition        		);
	Cli_NetReg( PG_Move_LtoC_DelFromPartition      		);
	Cli_NetReg( PG_Move_LtoC_SetPostion            		);
	Cli_NetReg( PG_Move_LtoC_ObjectMove            		);
	Cli_NetReg( PG_Move_LtoC_DoAction              		);
	Cli_NetReg( PG_Move_LtoC_MoveSpeed             		);
	Cli_NetReg( PG_Move_LtoC_MoveInfo	           		);
	Cli_NetReg( PG_Move_LtoC_EQInfo                		);
	Cli_NetReg( PG_Move_LtoC_FixEQInfo             		);
	Cli_NetReg( PG_Move_LtoC_LookFace              		);
	Cli_NetReg( PG_Move_LtoC_ClientAutoPlot        		);
	Cli_NetReg( PG_Move_LtoC_ActionType            		);
	Cli_NetReg( PG_Move_LtoC_JumpBegin             		);
	Cli_NetReg( PG_Move_LtoC_Relation              		);
	Cli_NetReg( PG_Move_LtoC_PartyMemberMove       		);
	Cli_NetReg( PG_Move_LtoC_PlayMotion					);
	Cli_NetReg( PG_Move_LtoC_SpecialMove				);
	Cli_NetReg( PG_Move_LtoC_Ping						);
	Cli_NetReg( PG_Move_LtoC_ClientLoading				);
	Cli_NetReg( PG_Move_LtoC_SetMinimapIcon				);
	Cli_NetReg( PG_Move_LtoC_SpecialEfficacy			);
	Cli_NetReg( PG_Move_LtoC_SetDir						);
	Cli_NetReg( PG_Move_LtoC_SetIdleMotion				);
	Cli_NetReg( PG_Move_LtoC_SetGuildID					);
	Cli_NetReg( PG_Move_LtoC_SetTitleID					);
	Cli_NetReg( PG_Move_LtoC_PlayerDropDownDamage		);
	Cli_NetReg( PG_Move_LtoC_HorseColor					);
	Cli_NetReg( PG_Move_LtoC_ObjGuildFurnitureInfo		);
	Cli_NetReg( PG_Move_LtoC_ObjGuildBuildingInfo		);
	Cli_NetReg( PG_Move_LtoC_RoomID						);
	Cli_NetReg( PG_Move_LtoC_PingLog					);
	Cli_NetReg( PG_Move_LtoC_DeadZone					);

	Cli_NetReg( PG_Move_LtoC_AttachObj					);
	Cli_NetReg( PG_Move_LtoC_DetachObj					);


	Cli_NetReg( PG_Move_LtoC_BattleGround_AddPlayer		);
	Cli_NetReg( PG_Move_LtoC_BattleGround_DelPlayer		);
	Cli_NetReg( PG_Move_LtoC_BattleGround_PlayerPos		);
	Cli_NetReg( PG_Move_LtoC_BattleGround_PlayerHPMP	);

	Cli_NetReg( PG_Move_LtoC_UnlockRolePosOK			);
	Cli_NetReg( PG_Move_LtoC_UnlockRolePosTime			);
	Cli_NetReg( PG_Move_LtoC_FlowerPotInfo				);

	Cli_NetReg( PG_Move_LtoC_AttachObjResult			);
	Cli_NetReg( PG_Move_LtoC_ColdownInfo				);

	Cli_NetReg( PG_Move_LtoC_PlayerPos					);

	Cli_NetReg( PG_Move_LtoC_ModelRate					);
	Cli_NetReg( PG_Move_LtoC_PlayerMove					);
	Cli_NetReg( PG_Move_LtoC_AllPlayerPos				);
	
	
    return true;
}
//-------------------------------------------------------------------
bool Net_Move::_Release()
{
    return true;
}
//-------------------------------------------------------------------
void Net_Move::_PG_Move_LtoC_AllPlayerPos( int Sockid , int Size , void* Data )
{
	PG_Move_LtoC_AllPlayerPos* pg = (PG_Move_LtoC_AllPlayerPos*)Data;
	This->RL_AllPlayerPos( pg->PlayerCount , pg->List );
}
void Net_Move::_PG_Move_LtoC_BattleGround_PlayerInfo_Zip	( int Sockid , int Size , void* Data )
{
	BattleGroundPlayerHPMPBaseStruct List[500];
	PG_Move_LtoC_BattleGround_PlayerInfo_Zip* PG = (PG_Move_LtoC_BattleGround_PlayerInfo_Zip*)Data;

	MyLzoClass myZip;
	int RetSize = myZip.Decode( PG->Data , PG->Size , sizeof(List) , (char*)&List );
	assert( RetSize != 0 );

	for( int i = 0 ; i < PG->Count ; i++ )
	{
		This->R_BattleGround_PlayerPos( List[i].GItemID , (float)List[i].X , (float)List[i].Y , (float)List[i].Z );
		This->R_BattleGround_PlayerHPMP( List[i].GItemID , (float)List[i].HP , (float)List[i].MP , (float)List[i].SP , (float)List[i].SP_Sub , (float)List[i].MaxHP , (float)List[i].MaxMP );
	}
	/*
	PG_CliConnect_LtoC_PlayerData_Zip* PG = (PG_CliConnect_LtoC_PlayerData_Zip*) Data;
	struct
	{
		BaseRoleData			BaseData;
		PlayerBaseRoleData	 	PlayerBaseData;
	}TempRoleBase;

	//	BaseRoleData	BaseData;


	MyLzoClass myZip;
	int RetSize = myZip.Decode( PG->Data , PG->Size , sizeof(TempRoleBase) , (char*)&TempRoleBase );

	assert( RetSize != 0 );

	This->PlayerData( &TempRoleBase.BaseData , &TempRoleBase.PlayerBaseData );
*/
}

void Net_Move::_PG_Move_LtoC_PlayerMove				( int Sockid , int Size , void* Data )
{
	PG_Move_LtoC_PlayerMove* PG = (PG_Move_LtoC_PlayerMove*)Data;
	RolePosStruct       Pos;
	Pos.X = PG->X;
	Pos.Y = PG->Y;
	Pos.Z = PG->Z;
	Pos.Dir = (float)PG->Dir;

	This->ObjectMove( PG->GItemID , &Pos , (float)PG->vX , 0 , (float)PG->vZ , (ClientMoveTypeENUM)PG->Type );
/*
	PG_Move_LtoC_ObjectMove* PG = (PG_Move_LtoC_ObjectMove*)Data;
	This->ObjectMove( PG->GItemID , &PG->Pos , PG->vX , PG->vY , PG->vZ , PG->Type );
	*/
}
void Net_Move::_PG_Move_LtoC_PlayerPos					( int Sockid , int Size , void* Data )
{
	PG_Move_LtoC_PlayerPos * PG = (PG_Move_LtoC_PlayerPos *)Data;
	This->R_PlayerPos( PG->Size , PG->Info );
}
void Net_Move::_PG_Move_LtoC_ColdownInfo				( int Sockid , int Size , void* Data )
{
	PG_Move_LtoC_ColdownInfo * PG = (PG_Move_LtoC_ColdownInfo *)Data;
	This->R_ColdownInfo( PG->GItemID , PG->Coldown );
}
void Net_Move::_PG_Move_LtoC_UnlockRolePosTime			( int Sockid , int Size , void* Data )
{
	PG_Move_LtoC_UnlockRolePosTime * PG = (PG_Move_LtoC_UnlockRolePosTime *)Data;
	This->R_UnlockRolePosTime( PG->Time );
}
void Net_Move::_PG_Move_LtoC_FlowerPotInfo				( int Sockid , int Size , void* Data )
{
	PG_Move_LtoC_FlowerPotInfo * PG = (PG_Move_LtoC_FlowerPotInfo *)Data;
	This->R_FlowerPotInfo( PG->GItemID , PG->Item );
}
void Net_Move::_PG_Move_LtoC_AttachObjResult			( int Sockid , int Size , void* Data )
{
	PG_Move_LtoC_AttachObjResult * PG = (PG_Move_LtoC_AttachObjResult *)Data;
	//This->R_AttachObjResult( PG->Result );
	switch( PG->Type )
	{
	case EM_AttachObjRequest_Attach:
		This->R_AttachObjResult( (bool)PG->Result , PG->Pos );
		break;
	case EM_AttachObjRequest_Detach:
		This->R_DetachObjResult( (bool)PG->Result );
		break;
	}
}
void Net_Move::_PG_Move_LtoC_UnlockRolePosOK			( int Sockid , int Size , void* Data )
{
	PG_Move_LtoC_UnlockRolePosOK * PG = (PG_Move_LtoC_UnlockRolePosOK *)Data;
	This->R_UnlockRolePosOK( PG->Result );
}
void Net_Move::_PG_Move_LtoC_BattleGround_AddPlayer	( int Sockid , int Size , void* Data )
{
	PG_Move_LtoC_BattleGround_AddPlayer * PG = (PG_Move_LtoC_BattleGround_AddPlayer *)Data;
	This->R_BattleGround_AddPlayer( PG->Info );
}
void Net_Move::_PG_Move_LtoC_BattleGround_DelPlayer	( int Sockid , int Size , void* Data )
{
	PG_Move_LtoC_BattleGround_DelPlayer * PG = (PG_Move_LtoC_BattleGround_DelPlayer *)Data;
	This->R_BattleGround_DelPlayer( PG->GItemID );
}
void Net_Move::_PG_Move_LtoC_BattleGround_PlayerPos	( int Sockid , int Size , void* Data )
{
	PG_Move_LtoC_BattleGround_PlayerPos * PG = (PG_Move_LtoC_BattleGround_PlayerPos *)Data;
	This->R_BattleGround_PlayerPos( PG->GItemID , PG->X , PG->Y , PG->Z );
}
void Net_Move::_PG_Move_LtoC_BattleGround_PlayerHPMP( int Sockid , int Size , void* Data )
{
	PG_Move_LtoC_BattleGround_PlayerHPMP * PG = (PG_Move_LtoC_BattleGround_PlayerHPMP *)Data;
	This->R_BattleGround_PlayerHPMP( PG->GItemID , PG->HP , PG->MP , PG->SP , PG->SP_Sub , PG->MaxHP , PG->MaxMP );
}

void Net_Move::_PG_Move_LtoC_AttachObj			( int Sockid , int Size , void* Data )
{
	PG_Move_LtoC_AttachObj * PG = (PG_Move_LtoC_AttachObj *)Data;
	This->R_AttachObj( PG->GItemID , PG->AttachItemID , PG->AttachType , PG->ItemPos.Begin() , PG->AttachPos.Begin() , PG->Pos );
}
void Net_Move::_PG_Move_LtoC_DetachObj			( int Sockid , int Size , void* Data )
{
	PG_Move_LtoC_DetachObj * PG = (PG_Move_LtoC_DetachObj *)Data;
	This->R_DetachObj( PG->GItemID );
}

void Net_Move::_PG_Move_LtoC_DeadZone		( int Sockid , int Size , void* Data )
{
	PG_Move_LtoC_DeadZone * PG = (PG_Move_LtoC_DeadZone *)Data;
	This->R_DeadZone( PG->Type , PG->DeadCountDown  );
}
void Net_Move::_PG_Move_LtoC_PingLog		( int Sockid , int Size , void* Data )
{
	PG_Move_LtoC_PingLog * PG = (PG_Move_LtoC_PingLog *)Data;
	This->R_PingLog( PG->ID , PG->Time  );
}

void Net_Move::_PG_Move_LtoC_RoomID			( int Sockid , int Size , void* Data )
{
	PG_Move_LtoC_RoomID * PG = (PG_Move_LtoC_RoomID *)Data;
	This->R_RoomID( PG->RoomID );
}
void Net_Move::_PG_Move_LtoC_HorseColor		( int Sockid , int Size , void* Data )
{
	PG_Move_LtoC_HorseColor * PG = (PG_Move_LtoC_HorseColor *)Data;
	This->R_HorseColor( PG->GItemID , PG->Color );
}
void Net_Move::_PG_Move_LtoC_ObjGuildFurnitureInfo( int Sockid , int Size , void* Data )
{
	PG_Move_LtoC_ObjGuildFurnitureInfo * PG = (PG_Move_LtoC_ObjGuildFurnitureInfo *)Data;
	This->R_ObjGuildFurnitureInfo( PG->GItemID , PG->FurnitureDBID );
}
void Net_Move::_PG_Move_LtoC_ObjGuildBuildingInfo( int Sockid , int Size , void* Data )
{
	PG_Move_LtoC_ObjGuildBuildingInfo * PG = (PG_Move_LtoC_ObjGuildBuildingInfo *)Data;
	This->R_ObjGuildBuildingInfo( PG->GItemID , PG->BuildingParentDBID , PG->BuildingDBID , PG->BuildingID , PG->Point );
}
void Net_Move::_PG_Move_LtoC_PlayerDropDownDamage( int Sockid , int Size , void* Data )
{
	PG_Move_LtoC_PlayerDropDownDamage * PG = (PG_Move_LtoC_PlayerDropDownDamage *)Data;
	This->R_PlayerDropDownDamage( PG->Damage , PG->Type );
}
void Net_Move::_PG_Move_LtoC_SetDir( int Sockid , int Size , void* Data )
{
	PG_Move_LtoC_SetDir * PG = (PG_Move_LtoC_SetDir *)Data;
	This->R_SetDir( PG->GItemID , PG->Dir );
}
void Net_Move::_PG_Move_LtoC_SetGuildID( int Sockid , int Size , void* Data )
{
	PG_Move_LtoC_SetGuildID * PG = (PG_Move_LtoC_SetGuildID *)Data;
	This->R_SetGuildID( PG->GItemID , PG->GuildID );
}

void Net_Move::_PG_Move_LtoC_SetTitleID( int Sockid , int Size , void* Data )
{
	PG_Move_LtoC_SetTitleID * PG = (PG_Move_LtoC_SetTitleID *)Data;
	This->R_SetTitleID( PG->GItemID , PG->IsShowTitle , PG->TitleID , PG->TitleStr.Begin() );
}

void Net_Move::_PG_Move_LtoC_SpecialEfficacy	( int Sockid , int Size , void* Data )
{
	PG_Move_LtoC_SpecialEfficacy * PG = (PG_Move_LtoC_SpecialEfficacy *)Data;
	This->R_SpecialEfficacy( PG->ItemGUID , PG->EffectType );
}

void Net_Move::_PG_Move_LtoC_ClientLoading      ( int Sockid , int Size , void* Data )
{
	PG_Move_LtoC_ClientLoading * PG = (PG_Move_LtoC_ClientLoading *)Data;
	This->R_ClientLoading( PG->CameraX , PG->CameraY , PG->CameraZ );
}

void Net_Move::_PG_Move_LtoC_Ping      ( int Sockid , int Size , void* Data )
{
	PG_Move_LtoC_Ping * PG = (PG_Move_LtoC_Ping *)Data;
	This->R_Ping( PG->Time );
}

void Net_Move::_PG_Move_LtoC_JumpBegin      ( int Sockid , int Size , void* Data )
{
    PG_Move_LtoC_JumpBegin * PG = (PG_Move_LtoC_JumpBegin *)Data;
    This->R_JumpBegin( PG->GItemID , PG->X , PG->Y , PG->Z , PG->Dir , PG->vX , PG->vY , PG->vZ );

}
void Net_Move::_PG_Move_LtoC_Relation		 ( int Sockid , int Size , void* Data )
{
	PG_Move_LtoC_Relation * PG = (PG_Move_LtoC_Relation *)Data;
	This->R_Relation( PG->GItemID , PG->Relation );

}
void Net_Move::_PG_Move_LtoC_PartyMemberMove ( int Sockid , int Size , void* Data )
{
	PG_Move_LtoC_PartyMemberMove * PG = (PG_Move_LtoC_PartyMemberMove *)Data;
	This->R_PartyMemberMove( PG->DBID , PG->GItemID , PG->X , PG->Y , PG->Z );
}

void Net_Move::_PG_Move_LtoC_SetIdleMotion ( int Sockid , int Size , void* Data )
{
	PG_Move_LtoC_SetIdleMotion * PG = (PG_Move_LtoC_SetIdleMotion *)Data;
	This->R_SetIdleMotion( PG->iObjID , PG->iMotionID );
}
/*
void Net_Move::_PG_Move_LtoC_JumpEnd        ( int Sockid , int Size , void* Data )
{
    PG_Move_LtoC_JumpEnd* PG = (PG_Move_LtoC_JumpEnd*)Data;
    This->R_JumpEnd( PG->GItemID , PG->X , PG->Y , PG->Z , PG->Dir );
}
*/
//-------------------------------------------------------------------
void Net_Move::_PG_Move_LtoC_ActionType ( int Sockid , int Size , void* Data )
{
    PG_Move_LtoC_ActionType * PG = (PG_Move_LtoC_ActionType *)Data;

    This->ActionType( PG->GItemID , PG->Action );
}
//-------------------------------------------------------------------
void Net_Move::_PG_Move_LtoC_ClientAutoPlot ( int Sockid , int Size , void* Data )
{
    PG_Move_LtoC_ClientAutoPlot* PG = (PG_Move_LtoC_ClientAutoPlot*)Data;

    This->ClientAutoPlot( PG->GItemID , (char*)PG->AutoPlot.Begin() );

}
//-------------------------------------------------------------------
void Net_Move::_PG_Move_LtoC_AddToPartition ( int Sockid , int Size , void* Data )
{
    PG_Move_LtoC_AddToPartition* PG = (PG_Move_LtoC_AddToPartition*)Data;
    
    This->AddToPartition( PG->iWorldID, PG->GItemID , PG->WorldGUID , PG->DBID , PG->MasterGUID , PG->CampID , &PG->Pos , &PG->GraphObj );

}
//-------------------------------------------------------------------
void Net_Move::_PG_Move_LtoC_DelFromPartition( int Sockid , int Size , void* Data )
{
    PG_Move_LtoC_DelFromPartition* PG = (PG_Move_LtoC_DelFromPartition*)Data;
    This->DelFromPartition( PG->GItemID , PG->WorldGUID );

}
//-------------------------------------------------------------------
void Net_Move::_PG_Move_LtoC_SetPostion     ( int Sockid , int Size , void* Data )
{
    PG_Move_LtoC_SetPostion* PG = (PG_Move_LtoC_SetPostion*)Data;
    This->SetPos( PG->GItemID , &PG->Pos );
}
//-------------------------------------------------------------------
void Net_Move::_PG_Move_LtoC_ObjectMove     ( int Sockid , int Size , void* Data )
{
    PG_Move_LtoC_ObjectMove* PG = (PG_Move_LtoC_ObjectMove*)Data;
    This->ObjectMove( PG->GItemID , &PG->Pos , PG->vX , PG->vY , PG->vZ , PG->Type );
}
//-------------------------------------------------------------------
void Net_Move::_PG_Move_LtoC_DoAction       ( int Sockid , int Size , void* Data )
{
    PG_Move_LtoC_DoAction* PG = (PG_Move_LtoC_DoAction*)Data;
    This->DoAction( PG->GItemID , PG->ActionID , PG->TempActionID );
}
//-------------------------------------------------------------------
void Net_Move::_PG_Move_LtoC_MoveSpeed      ( int Sockid , int Size , void* Data )
{
    PG_Move_LtoC_MoveSpeed* PG = (PG_Move_LtoC_MoveSpeed*)Data;
    This->MoveSpeed( PG->GItemID , PG->Speed );
}
//-------------------------------------------------------------------
void Net_Move::_PG_Move_LtoC_MoveInfo      ( int Sockid , int Size , void* Data )
{
	PG_Move_LtoC_MoveInfo* PG = (PG_Move_LtoC_MoveInfo*)Data;
	This->MoveInfo( PG->GItemID , PG->Gravity , PG->JumpRate );
}
//-------------------------------------------------------------------
void Net_Move::_PG_Move_LtoC_SetMinimapIcon      ( int Sockid , int Size , void* Data )
{
	PG_Move_LtoC_SetMinimapIcon* PG = (PG_Move_LtoC_SetMinimapIcon*)Data;
	This->SetMinimapIcon( PG->iObjID, PG->iIconID );
}
//-------------------------------------------------------------------
void Net_Move::_PG_Move_LtoC_ModelRate      ( int Sockid , int Size , void* Data )
{
    PG_Move_LtoC_ModelRate* PG = (PG_Move_LtoC_ModelRate*)Data;
    This->ModelRate( PG->GItemID , PG->ModelRate );
}
//-------------------------------------------------------------------
void Net_Move::_PG_Move_LtoC_EQInfo         ( int Sockid , int Size , void* Data )
{
    PG_Move_LtoC_EQInfo* PG = (PG_Move_LtoC_EQInfo*)Data;
    This->EQInfo( PG->ItemID , PG->EQLook , PG->ShowEQ );
}
//-------------------------------------------------------------------
void Net_Move::_PG_Move_LtoC_FixEQInfo      ( int Sockid , int Size , void* Data )
{
    PG_Move_LtoC_FixEQInfo* PG = (PG_Move_LtoC_FixEQInfo*)Data;
    This->FixEQInfo( PG->ItemID , PG->Pos , PG->EQ );
}
//-------------------------------------------------------------------
void Net_Move::_PG_Move_LtoC_LookFace       ( int Sockid , int Size , void* Data )
{
    PG_Move_LtoC_LookFace* PG = (PG_Move_LtoC_LookFace*)Data;
    This->LookFace( PG->GItemID , PG->Look );
}
//-------------------------------------------------------------------
void Net_Move::_PG_Move_LtoC_PlayMotion		( int Sockid , int Size , void* Data )
{
	PG_Move_LtoC_PlayMotion* PG = (PG_Move_LtoC_PlayMotion*)Data;
	This->R_PlayMotion( PG->iObjID, PG->iMotionID );
}
//-------------------------------------------------------------------
void Net_Move::_PG_Move_LtoC_SpecialMove		( int Sockid , int Size , void* Data )
{
	PG_Move_LtoC_SpecialMove* PG = (PG_Move_LtoC_SpecialMove*)Data;
	This->R_SpecialMove( PG->ItemGUID , PG->MoveType , PG->TargetGUID , PG->X , PG->Y , PG->Z , PG->Dir , PG->MagicCollectID );
}
//-------------------------------------------------------------------
// 資料送出
//-------------------------------------------------------------------
void Net_Move::PlayerMoveObject( int GItemID , float X , float Y , float Z , float Dir , int AttachOjbID , float vX , float vY, float vZ , ClientMoveTypeENUM Type  )
{
    PG_Move_CtoL_PlayerMoveObject Send;

    Send.GItemID = GItemID;
    Send.Pos.X = X;
    Send.Pos.Y = Y;
    Send.Pos.Z = Z;
    Send.Pos.Dir = Dir;
	Send.vX = vX;
	Send.vZ = vZ;
	Send.vY = vY;
    Send.Type = Type;
	//Send.GroundHeight = GroundHeight;
	Send.AttachObjID = AttachOjbID;

    SendToLocal( sizeof(Send) , &Send );
}
//-------------------------------------------------------------------
void Net_Move::S_DoAction( int GItemID , int ActionID , int TempActionID )
{
    PG_Move_CtoL_DoAction Send;
    Send.GItemID = GItemID;
    Send.ActionID = ActionID;
    Send.TempActionID = TempActionID;
    SendToLocal( sizeof(Send) , &Send );   
}

void Net_Move::S_JumpBegin( int GItemID  , float X , float Y , float Z , float Dir , float vX , float vY , float vZ )
{
    PG_Move_CtoL_JumpBegin Send;
    Send.GItemID = GItemID;
    Send.X = X;
    Send.Y = Y;
    Send.Z = Z;
    Send.Dir = Dir;
    Send.vX = vX;
    Send.vY = vY;
    Send.vZ = vZ;
    SendToLocal( sizeof(Send) , &Send );   
}
/*
void Net_Move::S_JumpEnd( int GItemID  , float X , float Y , float Z , float Dir  )
{
    PG_Move_CtoL_JumpEnd Send;
    Send.GItemID = GItemID;
    Send.X = X;
    Send.Y = Y;
    Send.Z = Z;
    Send.Dir = Dir;

    SendToLocal( sizeof(Send) , &Send );   
}*/

void Net_Move::S_Ping( int Time )
{
	PG_Move_CtoL_Ping Send;
	Send.Time = Time;
	SendToLocal( sizeof(Send) , &Send );   
}

int	Net_Move::GetRespondTime()
{
	return CNetGlobal::GetRespondTime();
}

//回應地圖載入OK
void Net_Move::S_ClientLoadingOK( )
{
	PG_Move_CtoL_ClientLoadingOK Send;
	SendToLocal( sizeof(Send) , &Send );   
}

void Net_Move::S_PingLog( int ID , int Time , int ClietNetProcTime )
{
	PG_Move_CtoL_PingLog Send;
	Send.ID = ID;
	Send.Time = Time;
	Send.ClietNetProcTime = ClietNetProcTime;
	SendToLocal( sizeof(Send) , &Send );   
}

void Net_Move::S_UnlockRolePosRequest( float X , float Y , float Z , bool Cancel )
{
	PG_Move_CtoL_UnlockRolePosRequest Send;
	Send.X = X;
	Send.Y = Y;
	Send.Z = Z;
	Send.Cancel = Cancel;
	SendToLocal( sizeof(Send) , &Send );   
}

void Net_Move::S_AttachObjRequest(  int GItemID , int WagonItemID , const char* ItemPos , const char* WagonPos , int AttachType )
{
	//g_pError->AddMessage(0, 0, "AttachObjRequest GItemID(%d) WagonItemID(%d)", GItemID, WagonItemID);

	PG_Move_CtoL_AttachObjRequest Send;
	Send.GItemID = GItemID;
	Send.WagonItemID = WagonItemID;
	Send.Type = EM_AttachObjRequest_Attach;		//0 Attach 1 Detach
	Send.ItemPos = ItemPos;
	Send.WagonPos = WagonPos;
	Send.AttachType = AttachType;						//Client attach 方式 server不處理
	SendToLocal( sizeof(Send) , &Send );   
}

void Net_Move::S_DetachObjRequest(  int GItemID , int WagonItemID  )
{
	PG_Move_CtoL_AttachObjRequest Send;
	Send.GItemID = GItemID;
	Send.WagonItemID = WagonItemID;
	Send.Type = EM_AttachObjRequest_Detach;		//0 Attach 1 Detach
	SendToLocal( sizeof(Send) , &Send );   
}

//設定顯示分割區
void Net_Move::S_SetPartition		( float X , float Y , float Z )
{
	PG_Move_CtoL_SetPartition Send;
	Send.X = X;
	Send.Y = Y;
	Send.Z = Z;
	SendToLocal( sizeof(Send) , &Send );   
}