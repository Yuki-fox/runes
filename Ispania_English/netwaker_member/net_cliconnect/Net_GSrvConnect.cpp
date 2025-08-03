#include "Net_GSrvConnect.h"
#include "PG/PG_CliConnect.h"
//#include "../MainProc/Global.h"
#pragma unmanaged

Net_GSrvConnect*  Net_GSrvConnect::This         = NULL;


bool Net_GSrvConnect::_Init()
{    
	/*
    _Net.RegOnSrvPacketFunction	( EM_PG_CliConnect_LtoC_PlayerItemID       		, _PG_CliConnect_LtoC_PlayerItemID       	);
    _Net.RegOnSrvPacketFunction	( EM_PG_CliConnect_LtoC_ZoneInfo           		, _PG_CliConnect_LtoC_ZoneInfo           	);
    _Net.RegOnSrvPacketFunction	( EM_PG_CliConnect_LtoC_PlayerData         		, _PG_CliConnect_LtoC_PlayerData         	);
    _Net.RegOnSrvPacketFunction	( EM_PG_CliConnect_LtoC_PrepareChangeZone  		, _PG_CliConnect_LtoC_PrepareChangeZone  	);
    _Net.RegOnSrvPacketFunction	( EM_PG_CliConnect_LtoC_ChangeZoneOK       		, _PG_CliConnect_LtoC_ChangeZoneOK       	);
    _Net.RegOnSrvPacketFunction	( EM_PG_CliConnect_LtoC_ModifyNPCInfo			, _PG_CliConnect_LtoC_ModifyNPCInfo      	);
    _Net.RegOnSrvPacketFunction	( EM_PG_CliConnect_LtoC_GetNPCMoveInfoResult    , _PG_CliConnect_LtoC_GetNPCMoveInfoResult	);
    _Net.RegOnSrvPacketFunction	( EM_PG_CliConnect_LtoC_GetFlagListResult		, _PG_CliConnect_LtoC_GetFlagListResult		);
	_Net.RegOnSrvPacketFunction	( EM_PG_CliConnect_LtoC_EditFlagResult			, _PG_CliConnect_LtoC_EditFlagResult		);
	_Net.RegOnSrvPacketFunction	( EM_PG_CliConnect_LtoC_DelFlagResult			, _PG_CliConnect_LtoC_DelFlagResult			);
	_Net.RegOnSrvPacketFunction	( EM_PG_CliConnect_LtoC_DelFlagGroupResult		, _PG_CliConnect_LtoC_DelFlagGroupResult	);
	_Net.RegOnSrvPacketFunction	( EM_PG_CliConnect_LtoC_LogoutResult			, _PG_CliConnect_LtoC_LogoutResult			);
	_Net.RegOnSrvPacketFunction	( EM_PG_CliConnect_LtoC_ParalledID				, _PG_CliConnect_LtoC_ParalledID			);
	_Net.RegOnSrvPacketFunction	( EM_PG_CliConnect_LtoC_PlayerData_Zip			, _PG_CliConnect_LtoC_PlayerData_Zip	);
	_Net.RegOnSrvPacketFunction	( EM_PG_CliConnect_LtoC_SysVersionInfo			, _PG_CliConnect_LtoC_SysVersionInfo	);
*/



	_NetBattle.RegOnLoginFunction( _OnLoginBattleGround );
	_NetBattle.RegOnLogoutFunction( _OnLogoutBattleGround );
	_Net.RegOnSrvPacketFunction			( EM_PG_CliConnect_LtoC_PlayerItemID       		, _PG_CliConnect_LtoC_PlayerItemID_Base       	);
	_NetBattle.RegOnSrvPacketFunction	( EM_PG_CliConnect_LtoC_PlayerItemID       		, _PG_CliConnect_LtoC_PlayerItemID_Battle      	);
//	Cli_NetReg( PG_CliConnect_LtoC_PlayerItemID       	);
	Cli_NetReg( PG_CliConnect_LtoC_ZoneInfo           	);
	Cli_NetReg( PG_CliConnect_LtoC_PlayerData         	);
	Cli_NetReg( PG_CliConnect_LtoC_PrepareChangeZone  	);
	Cli_NetReg( PG_CliConnect_LtoC_ChangeZoneOK       	);
	Cli_NetReg( PG_CliConnect_LtoC_ModifyNPCInfo		);
	Cli_NetReg( PG_CliConnect_LtoC_GetNPCMoveInfoResult	);
	Cli_NetReg( PG_CliConnect_LtoC_GetFlagListResult	);
	Cli_NetReg( PG_CliConnect_LtoC_EditFlagResult		);
	Cli_NetReg( PG_CliConnect_LtoC_DelFlagResult		);
	Cli_NetReg( PG_CliConnect_LtoC_DelFlagGroupResult	);
	Cli_NetReg( PG_CliConnect_LtoC_LogoutResult			);
	Cli_NetReg( PG_CliConnect_LtoC_ParalledID			);
	Cli_NetReg( PG_CliConnect_LtoC_PlayerData_Zip		);
	Cli_NetReg( PG_CliConnect_LtoC_SysVersionInfo		);
	Cli_NetReg( PG_CliConnect_LtoC_ConnectToBattleWorld	);
	Cli_NetReg( PG_CliConnect_LtoC_SkyProcType			);
	Cli_NetReg( PG_CliConnect_LtoC_TouchTomb			);
	Cli_NetReg( PG_CliConnect_LtoC_TombInfo				);
	Cli_NetReg( PG_CliConnect_LtoC_WorldReturnNotify	);
	Cli_NetReg( PG_CliConnect_LtoC_ZoneOpenState		);
    return true;
}
bool Net_GSrvConnect::_Release()
{
    return true;
}
//------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
void Net_GSrvConnect::_PG_CliConnect_LtoC_ZoneOpenState	( int Sockid , int Size , void* Data )
{
	PG_CliConnect_LtoC_ZoneOpenState* PG = (PG_CliConnect_LtoC_ZoneOpenState*) Data;
	This->RL_ZoneOpenState( PG->ZoneID , PG->IsOpen );
}

void Net_GSrvConnect::_PG_CliConnect_LtoC_WorldReturnNotify	( int Sockid , int Size , void* Data )
{
	PG_CliConnect_LtoC_WorldReturnNotify* PG = (PG_CliConnect_LtoC_WorldReturnNotify*) Data;
	This->R_WorldReturnNotify( PG->ZoneID );
}
void Net_GSrvConnect::_OnLoginBattleGround					( string sInfo )
{
	This->OnLoginBattleGround();
}
void Net_GSrvConnect::_OnLogoutBattleGround					(  )
{
	This->OnLogoutBattleGround();
}

void Net_GSrvConnect::_PG_CliConnect_LtoC_TombInfo			( int Sockid , int Size , void* Data )
{
	PG_CliConnect_LtoC_TombInfo* PG = (PG_CliConnect_LtoC_TombInfo*)Data;
	This->R_TombInfo( PG->CreateTime , PG->ZoneID , PG->X , PG->Y , PG->Z , PG->DebtExp , PG->DebtTp , PG->Exp );
}

void Net_GSrvConnect::_PG_CliConnect_LtoC_TouchTomb			( int Sockid , int Size , void* Data )
{
	PG_CliConnect_LtoC_TouchTomb* PG = (PG_CliConnect_LtoC_TouchTomb*)Data;
	This->R_TouchTomb( );
}
void Net_GSrvConnect::_PG_CliConnect_LtoC_SkyProcType			( int Sockid , int Size , void* Data )
{
	PG_CliConnect_LtoC_SkyProcType* PG = (PG_CliConnect_LtoC_SkyProcType*) Data;
	This->R_SkyProcType( PG->Type );
}
void Net_GSrvConnect::_PG_CliConnect_LtoC_ConnectToBattleWorld( int Sockid , int Size , void* Data )
{
	PG_CliConnect_LtoC_ConnectToBattleWorld* PG = (PG_CliConnect_LtoC_ConnectToBattleWorld*) Data;
	This->R_ConnectToBattleWorld( PG->IPStr , PG->Port );
}
void Net_GSrvConnect::_PG_CliConnect_LtoC_SysVersionInfo	( int Sockid , int Size , void* Data )
{
	PG_CliConnect_LtoC_SysVersionInfo* PG = (PG_CliConnect_LtoC_SysVersionInfo*) Data;
	This->R_SysVersionInfo( PG->LearnMagicID , PG->Version , PG->PK_DotRate );
}

void Net_GSrvConnect::_PG_CliConnect_LtoC_PlayerData_Zip	( int Sockid , int Size , void* Data )
{
	PG_CliConnect_LtoC_PlayerData_Zip* PG = (PG_CliConnect_LtoC_PlayerData_Zip*) Data;


	struct
	{
		BaseRoleData			BaseData;
		PlayerBaseRoleData	 	PlayerBaseData;
		SysBaseRoleData			Base;
	}TempRoleBase;

//	BaseRoleData	BaseData;


	MyLzoClass myZip;
	int RetSize = myZip.Decode( PG->Data , PG->Size , sizeof(TempRoleBase) , (char*)&TempRoleBase );

	assert( RetSize != 0 );

	This->PlayerData( &TempRoleBase.BaseData , &TempRoleBase.PlayerBaseData , &TempRoleBase.Base );
}
void Net_GSrvConnect::_PG_CliConnect_LtoC_ParalledID		( int Sockid , int Size , void* Data )
{
	PG_CliConnect_LtoC_ParalledID* PG = (PG_CliConnect_LtoC_ParalledID*) Data;
	This->R_ParalledID( PG->ParalledID );
}
void Net_GSrvConnect::_PG_CliConnect_LtoC_LogoutResult		( int Sockid , int Size , void* Data )
{
	PG_CliConnect_LtoC_LogoutResult* PG = (PG_CliConnect_LtoC_LogoutResult*) Data;
	This->R_LogoutResult( PG->Result );
}

void Net_GSrvConnect::_PG_CliConnect_LtoC_ModifyNPCInfo( int Sockid , int Size , void* Data )
{
    PG_CliConnect_LtoC_ModifyNPCInfo* PG = (PG_CliConnect_LtoC_ModifyNPCInfo*) Data;
    This->R_ModyNPC( &PG->Info );
}
void Net_GSrvConnect::_PG_CliConnect_LtoC_PlayerItemID_Base ( int Sockid , int Size , void* Data )
{
	SetSrvID_Local_Base( Sockid );
    PG_CliConnect_LtoC_PlayerItemID* PG = (PG_CliConnect_LtoC_PlayerItemID*) Data;
    This->PlayerItemID( PG->GItemID, PG->WorldGUID , PG->DBID );
}
void Net_GSrvConnect::_PG_CliConnect_LtoC_PlayerItemID_Battle ( int Sockid , int Size , void* Data )
{
	SetSrvID_Local_Battle( Sockid );
	PG_CliConnect_LtoC_PlayerItemID* PG = (PG_CliConnect_LtoC_PlayerItemID*) Data;
	This->PlayerItemID( PG->GItemID, PG->WorldGUID , PG->DBID );
}
void Net_GSrvConnect::_PG_CliConnect_LtoC_ZoneInfo     ( int Sockid , int Size , void* Data )
{
    PG_CliConnect_LtoC_ZoneInfo* PG = (PG_CliConnect_LtoC_ZoneInfo*) Data;
	This->ZoneInfo( Sockid , PG->ZoneID , Sockid , PG->MapID , PG->CampObjID , PG->LanguageType , PG->ZoneMode, PG->BG_iNumTeamPeople );
}
void Net_GSrvConnect::_PG_CliConnect_LtoC_PlayerData   ( int Sockid , int Size , void* Data )
{
    PG_CliConnect_LtoC_PlayerData* PG = (PG_CliConnect_LtoC_PlayerData*) Data;
    This->PlayerData( &PG->BaseData , &PG->PlayerBaseData , &PG->Base );
}
void Net_GSrvConnect::_PG_CliConnect_LtoC_PrepareChangeZone( int Sockid , int Size , void* Data )
{
    PG_CliConnect_LtoC_PrepareChangeZone* PG = (PG_CliConnect_LtoC_PrepareChangeZone*) Data;
    This->PrepareChangeZone( );
}
void Net_GSrvConnect::_PG_CliConnect_LtoC_ChangeZoneOK ( int Sockid , int Size , void* Data )
{
    PG_CliConnect_LtoC_ChangeZoneOK* PG = (PG_CliConnect_LtoC_ChangeZoneOK*) Data;
    This->ChangeZoneOK( );
}
void Net_GSrvConnect::_PG_CliConnect_LtoC_GetNPCMoveInfoResult( int Sockid , int Size , void* Data )
{
	static vector<NPC_MoveBaseStruct> MoveList;
	PG_CliConnect_LtoC_GetNPCMoveInfoResult* PG = (PG_CliConnect_LtoC_GetNPCMoveInfoResult*) Data;

	if( PG->TotalCount == 0 )
	{	
		This->R_GetNPCMoveInfoResult( PG->GUID , MoveList );
		return;
	}
	else
	{
		MoveList.push_back( PG->Base );
	}

	if( MoveList.size() == PG->TotalCount )
	{
		This->R_GetNPCMoveInfoResult( PG->GUID , MoveList );
		MoveList.clear();
		return;
	}
	
}
void Net_GSrvConnect::_PG_CliConnect_LtoC_GetFlagListResult( int Sockid , int Size , void* Data )
{
	PG_CliConnect_LtoC_GetFlagListResult* pPacket = (PG_CliConnect_LtoC_GetFlagListResult*) Data;
	vector< vector < CliFlagPosInfo > > vecFlagList;
	
	CliFlagPosInfo	Info;
	int				iGroup		= pPacket->iTotalGroup;
	int				iFlagCount	= 0;
	int				iIndex		= 0;
	BYTE*			ptr			= (BYTE*)pPacket + ( sizeof( DWORD ) * 2 );
	
	vecFlagList.resize( iGroup );

	for( int iGroupID = 0; iGroupID < iGroup; iGroupID++ )
	{
		iFlagCount	= *( (int*) ptr );
		ptr		= ptr + sizeof( int );

		vector < CliFlagPosInfo > &vecInfo = vecFlagList[ iGroupID ];
		vecInfo.resize( iFlagCount );
		
		for( int iFlagID = 0; iFlagID < iFlagCount; iFlagID++ )
		{
			Info	= *( (CliFlagPosInfo*) ptr );
			ptr		= ptr + sizeof( CliFlagPosInfo );

			vecInfo[ iFlagID ] = Info;
		}
	}

	This->R_GetFlagListResult( Sockid, vecFlagList );
}
void Net_GSrvConnect::_PG_CliConnect_LtoC_EditFlagResult( int Sockid , int Size , void* Data )
{
	PG_CliConnect_LtoC_EditFlagResult* pPacket = (PG_CliConnect_LtoC_EditFlagResult*) Data;
	This->R_EditFlagResult( Sockid, pPacket->iResult );
}
void Net_GSrvConnect::_PG_CliConnect_LtoC_DelFlagResult( int Sockid , int Size , void* Data )
{
	PG_CliConnect_LtoC_DelFlagResult* pPacket = (PG_CliConnect_LtoC_DelFlagResult*) Data;
	This->R_DelFlagResult( Sockid, pPacket->iResult );
}
void Net_GSrvConnect::_PG_CliConnect_LtoC_DelFlagGroupResult( int Sockid , int Size , void* Data )
{
	PG_CliConnect_LtoC_DelFlagGroupResult* pPacket = (PG_CliConnect_LtoC_DelFlagGroupResult*) Data;
	This->R_DelFlagGroupResult( Sockid, pPacket->iResult );
}
//------------------------------------------------------------------------------------------------------
void Net_GSrvConnect::Logout( )
{
    PG_CliConnect_CtoL_RequestLogout Send;

    SendToLocal(  sizeof(Send) , &Send );
}
//------------------------------------------------------------------------------------------------------
void Net_GSrvConnect::ModyNPC( ModifyNPCInfo*  Info )
{
    PG_CliConnect_CtoL_ModifyNPCInfo Send;
    Send.Info = *Info;
    SendToLocal(  sizeof(Send) , &Send );
}
//------------------------------------------------------------------------------------------------------
//儲存NPC移動資料
void Net_GSrvConnect::SaveNPCMoveInfo( int GItemID , vector<NPC_MoveBaseStruct>& List )
{
	PG_CliConnect_CtoL_SaveNPCMoveInfo Send;

	int Count = (int)List.size();
	Send.GUID = GItemID;
	Send.TotalCount = Count;

	if( Count == 0 )
	{
		Send.IndexID = -1;
		SendToLocal(  sizeof(Send) , &Send );
		return;
	}

	for( int i = 0 ; i < Count ; i++ )
	{
		Send.IndexID = i;
		Send.Base = List[i];
		SendToLocal(  sizeof(Send) , &Send );
	}
}
//------------------------------------------------------------------------------------------------------
void Net_GSrvConnect::GetNPCMoveInfo ( int iNPCObjID )
{
	PG_CliConnect_CtoL_GetNPCMoveInfoRequest Packet;
	Packet.GUID = iNPCObjID;

	SendToLocal( sizeof(Packet), &Packet );
}
//------------------------------------------------------------------------------------------------------
void Net_GSrvConnect::GetFlagList()
{
	PG_CliConnect_CtoL_GetFlagList Packet;
	SendToLocal( sizeof(Packet), &Packet );
}
//------------------------------------------------------------------------------------------------------
void	Net_GSrvConnect::EditFlag				( CliFlagPosInfo Flag )
{
	PG_CliConnect_CtoL_EditFlag Packet;

	Packet.ObjID	= Flag.GroupID;
	Packet.ID		= Flag.FlagID;
	Packet.X		= Flag.X;
	Packet.Y		= Flag.Y;
	Packet.Z		= Flag.Z;
	Packet.Dir		= Flag.Dir;
	Packet.FlagDBID	= Flag.FlagDBID;

	SendToLocal( sizeof(Packet), &Packet );
}
//------------------------------------------------------------------------------------------------------
void Net_GSrvConnect::DelFlag					( int GroupID, int FlagID )
{
	PG_CliConnect_CtoL_DelFlag Packet;

	Packet.GroupID	= GroupID;
	Packet.FlagID	= FlagID;

	SendToLocal( sizeof(Packet), &Packet );
}
//------------------------------------------------------------------------------------------------------
void Net_GSrvConnect::DelFlagGroup			( int iFlagGroupID )
{
	PG_CliConnect_CtoL_DelFlagGroup Packet;

	Packet.iFlagGroupID	= iFlagGroupID;

	SendToLocal( sizeof(Packet), &Packet );
}

void Net_GSrvConnect::S_ConnectToBattleWorldOK( int CliNetID )
{
	PG_CliConnect_CtoL_ConnectToBattleWorldOK Send;
	Send.CliNetID = CliNetID;

	SendToLocal_Base( sizeof(Send), &Send );
}
void Net_GSrvConnect::S_WorldReturnOK( )
{
	PG_CliConnect_CtoL_WorldReturnOK Send;
	SendToLocal_Battle( sizeof(Send), &Send );
}
#pragma managed