#include "NetCli_GuildHouses.h"

//-------------------------------------------------------------------
NetCli_GuildHouses*    NetCli_GuildHouses::This         = NULL;

//-------------------------------------------------------------------
bool NetCli_GuildHouses::_Init()
{
	Cli_NetReg( PG_GuildHouses_LtoC_BuildHouseResult );
	Cli_NetReg( PG_GuildHouses_LtoC_OpenVisitHouse	 );
	Cli_NetReg( PG_GuildHouses_LtoC_FindHouseResult	 );
	Cli_NetReg( PG_GuildHouses_LtoC_CloseVisitHouse	 );
	Cli_NetReg( PG_GuildHouses_LtoC_HouseInfoLoading );
	Cli_NetReg( PG_GuildHouses_LtoC_HouseInfoLoadOK	 );

	Cli_NetReg( PG_GuildHouses_LtoC_CreateBuildingResult	 );
	Cli_NetReg( PG_GuildHouses_LtoC_BuildingUpgradeResult	 );
	Cli_NetReg( PG_GuildHouses_LtoC_DeleteBuildingResult	 );

	Cli_NetReg( PG_GuildHouses_LtoC_AllBuildingInfo	 		);

	Cli_NetReg( PG_GuildHouses_LtoC_NewBuildingInfo	 		);
	Cli_NetReg( PG_GuildHouses_LtoC_DelBuildingInfo	 		);

	Cli_NetReg( PG_GuildHouses_LtoC_HouseItemInfo			);
	Cli_NetReg( PG_GuildHouses_LtoC_SwapBodyHouseItemResult );
	Cli_NetReg( PG_GuildHouses_LtoC_FixHouseItem			);
	Cli_NetReg( PG_GuildHouses_LtoC_SwapHouseItemResult		);


	Cli_NetReg( PG_GuildHouses_LtoC_FurnitureItemInfo				);
	Cli_NetReg( PG_GuildHouses_LtoC_SwapBodyHouseFurnitureResult	);
	Cli_NetReg( PG_GuildHouses_LtoC_FixHouseFurniture				);
	Cli_NetReg( PG_GuildHouses_LtoC_SwapHouseFurnitureResult		);
	Cli_NetReg( PG_GuildHouses_LtoC_HouseItemLayoutResult			);

	Cli_NetReg( PG_GuildHouses_LtoC_StoreConfig						);
	Cli_NetReg( PG_GuildHouses_LtoC_SetStoreConfigResult			);

	Cli_NetReg( PG_GuildHouses_LtoC_ActiveBuildingResult			);
	Cli_NetReg( PG_GuildHouses_LtoC_BuyItemPageResult				);
	Cli_NetReg( PG_GuildHouses_LtoC_ItemLogList						);
	Cli_NetReg( PG_GuildHouses_LtoC_ItemLog							);

	Cli_NetReg( PG_GuildHouses_LtoC_BuildingActiveType				);
	Cli_NetReg( PG_GuildHouses_LtoC_FurnitureSendBackResult			);
	Cli_NetReg( PG_GuildHouses_LtoC_BuyHouseStyleResult				);
//	Cli_NetReg( PG_GuildHouses_LtoC_SetHouseStyleResult				);
	Cli_NetReg( PG_GuildHouses_LtoC_HouseStyle						);

    return true;
}
//-------------------------------------------------------------------
bool NetCli_GuildHouses::_Release()
{
    return true;
}
//-------------------------------------------------------------------
void NetCli_GuildHouses::_PG_GuildHouses_LtoC_HouseStyle					( int Sockid , int Size , void* Data )
{
	PG_GuildHouses_LtoC_HouseStyle* PG = (PG_GuildHouses_LtoC_HouseStyle*)Data;
	This->RL_HouseStyle( PG->Type );
}
void NetCli_GuildHouses::_PG_GuildHouses_LtoC_BuyHouseStyleResult			( int Sockid , int Size , void* Data )
{
	PG_GuildHouses_LtoC_BuyHouseStyleResult* PG = (PG_GuildHouses_LtoC_BuyHouseStyleResult*)Data;
	This->RL_BuyHouseStyleResult( PG->Type , PG->Result );
}
/*
void NetCli_GuildHouses::_PG_GuildHouses_LtoC_SetHouseStyleResult			( int Sockid , int Size , void* Data )
{
	PG_GuildHouses_LtoC_SetHouseStyleResult* PG = (PG_GuildHouses_LtoC_SetHouseStyleResult*)Data;
	This->RL_SetHouseStyleResult( PG->Result );
}
*/
void NetCli_GuildHouses::_PG_GuildHouses_LtoC_FurnitureSendBackResult		( int Sockid , int Size , void* Data )
{
	PG_GuildHouses_LtoC_FurnitureSendBackResult* PG = (PG_GuildHouses_LtoC_FurnitureSendBackResult*)Data;
	This->RL_FurnitureSendBackResult( PG->Pos , PG->Result );
}
void NetCli_GuildHouses::_PG_GuildHouses_LtoC_BuildingActiveType			( int Sockid , int Size , void* Data )
{
	PG_GuildHouses_LtoC_BuildingActiveType* PG = (PG_GuildHouses_LtoC_BuildingActiveType*)Data;
	This->RL_BuildingActiveType( PG->BuildDBID , PG->IsActive );
}
void NetCli_GuildHouses::_PG_GuildHouses_LtoC_BuyItemPageResult				( int Sockid , int Size , void* Data )
{
	PG_GuildHouses_LtoC_BuyItemPageResult* PG = (PG_GuildHouses_LtoC_BuyItemPageResult*)Data;
	This->RL_BuyItemPageResult( PG->PageID , PG->Result );
}

void NetCli_GuildHouses::_PG_GuildHouses_LtoC_ActiveBuildingResult			( int Sockid , int Size , void* Data )
{
	PG_GuildHouses_LtoC_ActiveBuildingResult* PG = (PG_GuildHouses_LtoC_ActiveBuildingResult*)Data;
	This->RL_ActiveBuildingResult( PG->BuildDBID , PG->IsActive ,PG->Result );
}

void NetCli_GuildHouses::_PG_GuildHouses_LtoC_ItemLogList					( int Sockid , int Size , void* Data )
{
	PG_GuildHouses_LtoC_ItemLogList* PG = (PG_GuildHouses_LtoC_ItemLogList*)Data;
	This->RL_ItemLogList( PG->Count , PG->Log );
}
void NetCli_GuildHouses::_PG_GuildHouses_LtoC_ItemLog						( int Sockid , int Size , void* Data )
{
	PG_GuildHouses_LtoC_ItemLog* PG = (PG_GuildHouses_LtoC_ItemLog*)Data;
	This->RL_ItemLog( PG->ItemLog );
}
void NetCli_GuildHouses::_PG_GuildHouses_LtoC_StoreConfig					( int Sockid , int Size , void* Data )
{
	PG_GuildHouses_LtoC_StoreConfig* PG = (PG_GuildHouses_LtoC_StoreConfig*)Data;
	This->RL_StoreConfig( PG->Config );
}
void NetCli_GuildHouses::_PG_GuildHouses_LtoC_SetStoreConfigResult			( int Sockid , int Size , void* Data )
{
	PG_GuildHouses_LtoC_SetStoreConfigResult* PG = (PG_GuildHouses_LtoC_SetStoreConfigResult*)Data;
	This->RL_SetStoreConfigResult( PG->Result );
}

void NetCli_GuildHouses::_PG_GuildHouses_LtoC_FurnitureItemInfo				( int Sockid , int Size , void* Data )
{
	PG_GuildHouses_LtoC_FurnitureItemInfo* PG = (PG_GuildHouses_LtoC_FurnitureItemInfo*)Data;
	This->RL_FurnitureItemInfo( PG->Item );
}
void NetCli_GuildHouses::_PG_GuildHouses_LtoC_SwapBodyHouseFurnitureResult	( int Sockid , int Size , void* Data )
{
	PG_GuildHouses_LtoC_SwapBodyHouseFurnitureResult* PG = (PG_GuildHouses_LtoC_SwapBodyHouseFurnitureResult*)Data;
	This->RL_SwapBodyHouseFurnitureResult( PG->BodyPos , PG->HousePos, PG->Result );

}
void NetCli_GuildHouses::_PG_GuildHouses_LtoC_FixHouseFurniture				( int Sockid , int Size , void* Data )
{
	PG_GuildHouses_LtoC_FixHouseFurniture* PG = (PG_GuildHouses_LtoC_FixHouseFurniture*)Data;
	This->RL_FixHouseFurniture( PG->Item );

}
void NetCli_GuildHouses::_PG_GuildHouses_LtoC_SwapHouseFurnitureResult		( int Sockid , int Size , void* Data )
{
	PG_GuildHouses_LtoC_SwapHouseFurnitureResult* PG = (PG_GuildHouses_LtoC_SwapHouseFurnitureResult*)Data;
	This->RL_SwapHouseFurnitureResult( PG->Pos , PG->Result );

}
void NetCli_GuildHouses::_PG_GuildHouses_LtoC_HouseItemLayoutResult			( int Sockid , int Size , void* Data )
{
	PG_GuildHouses_LtoC_HouseItemLayoutResult* PG = (PG_GuildHouses_LtoC_HouseItemLayoutResult*)Data;
	This->RL_HouseItemLayoutResult( PG->Pos , PG->Result );

}

void NetCli_GuildHouses::_PG_GuildHouses_LtoC_HouseItemInfo				( int Sockid , int Size , void* Data )
{
	PG_GuildHouses_LtoC_HouseItemInfo* PG = (PG_GuildHouses_LtoC_HouseItemInfo*)Data;
	This->RL_HouseItemInfo( PG->PageID , PG->MaxPage , PG->GetCount , PG->ItemCount , PG->Item );
}
void NetCli_GuildHouses::_PG_GuildHouses_LtoC_SwapBodyHouseItemResult	( int Sockid , int Size , void* Data )
{
	PG_GuildHouses_LtoC_SwapBodyHouseItemResult* PG = (PG_GuildHouses_LtoC_SwapBodyHouseItemResult*)Data;
	This->RL_SwapBodyHouseItemResult( PG->BodyPos , PG->HouseItemPageID , PG->HouseItemPos , PG->GetCount , PG->Result );
}
void NetCli_GuildHouses::_PG_GuildHouses_LtoC_FixHouseItem				( int Sockid , int Size , void* Data )
{
	PG_GuildHouses_LtoC_FixHouseItem* PG = (PG_GuildHouses_LtoC_FixHouseItem*)Data;
	This->RL_FixHouseItem( PG->Item );
}
void NetCli_GuildHouses::_PG_GuildHouses_LtoC_SwapHouseItemResult		( int Sockid , int Size , void* Data )
{
	PG_GuildHouses_LtoC_SwapHouseItemResult* PG = (PG_GuildHouses_LtoC_SwapHouseItemResult*)Data;
	This->RL_SwapHouseItemResult( PG->HouseItemPos , PG->HouseItemPageID , PG->Result );
}


void NetCli_GuildHouses::_PG_GuildHouses_LtoC_NewBuildingInfo	( int Sockid , int Size , void* Data )
{
	PG_GuildHouses_LtoC_NewBuildingInfo* PG = (PG_GuildHouses_LtoC_NewBuildingInfo*)Data;
	This->RL_NewBuildingInfo( PG->Building );
}
void NetCli_GuildHouses::_PG_GuildHouses_LtoC_DelBuildingInfo	( int Sockid , int Size , void* Data )
{
	PG_GuildHouses_LtoC_DelBuildingInfo* PG = (PG_GuildHouses_LtoC_DelBuildingInfo*)Data;
	This->RL_DelBuildingInfo( PG->BuildingDBID );
}

void NetCli_GuildHouses::_PG_GuildHouses_LtoC_AllBuildingInfo	( int Sockid , int Size , void* Data )
{
	PG_GuildHouses_LtoC_AllBuildingInfo* PG = (PG_GuildHouses_LtoC_AllBuildingInfo*)Data;
	This->RL_AllBuildingInfo( PG->GuildID , PG->Count , PG->Building );
}

void NetCli_GuildHouses::_PG_GuildHouses_LtoC_CreateBuildingResult	( int Sockid , int Size , void* Data )
{
	PG_GuildHouses_LtoC_CreateBuildingResult* PG = (PG_GuildHouses_LtoC_CreateBuildingResult*)Data;
	This->RL_CreateBuildingResult( PG->Point , PG->BuildingID , PG->BuildingDBID, PG->ParentBuildingDBID , PG->Result );
}
void NetCli_GuildHouses::_PG_GuildHouses_LtoC_BuildingUpgradeResult	( int Sockid , int Size , void* Data )
{
	PG_GuildHouses_LtoC_BuildingUpgradeResult* PG = (PG_GuildHouses_LtoC_BuildingUpgradeResult*)Data;
	This->RL_BuildingUpgradeResult( PG->GItemID , PG->BuildingDBID , PG->UpgradeBuildingID , PG->Result );
}
void NetCli_GuildHouses::_PG_GuildHouses_LtoC_DeleteBuildingResult	( int Sockid , int Size , void* Data )
{
	PG_GuildHouses_LtoC_DeleteBuildingResult* PG = (PG_GuildHouses_LtoC_DeleteBuildingResult*)Data;
	This->RL_DeleteBuildingResult( PG->BuildingDBID , PG->Result );
}

void NetCli_GuildHouses::_PG_GuildHouses_LtoC_BuildHouseResult	( int Sockid , int Size , void* Data )
{
	PG_GuildHouses_LtoC_BuildHouseResult* PG = (PG_GuildHouses_LtoC_BuildHouseResult*)Data;
	This->RL_BuildHouseResult( PG->GuildID , PG->Result );
}
void NetCli_GuildHouses::_PG_GuildHouses_LtoC_OpenVisitHouse	( int Sockid , int Size , void* Data )
{
	PG_GuildHouses_LtoC_OpenVisitHouse* PG = (PG_GuildHouses_LtoC_OpenVisitHouse*)Data;
	This->RL_OpenVisitHouse( PG->TargetNpcID , PG->IsVisitMyGuildHouse );
}
void NetCli_GuildHouses::_PG_GuildHouses_LtoC_FindHouseResult	( int Sockid , int Size , void* Data )
{
	PG_GuildHouses_LtoC_FindHouseResult* PG = (PG_GuildHouses_LtoC_FindHouseResult*)Data;
	This->RL_FindHouseResult( PG->Result );
}
void NetCli_GuildHouses::_PG_GuildHouses_LtoC_CloseVisitHouse	( int Sockid , int Size , void* Data )
{
	PG_GuildHouses_LtoC_CloseVisitHouse* PG = (PG_GuildHouses_LtoC_CloseVisitHouse*)Data;
	This->RL_CloseVisitHouse( );
}
void NetCli_GuildHouses::_PG_GuildHouses_LtoC_HouseInfoLoading	( int Sockid , int Size , void* Data )
{
	PG_GuildHouses_LtoC_HouseInfoLoading* PG = (PG_GuildHouses_LtoC_HouseInfoLoading*)Data;
	This->RL_HouseInfoLoading( PG->GuildID );
}
void NetCli_GuildHouses::_PG_GuildHouses_LtoC_HouseInfoLoadOK	( int Sockid , int Size , void* Data )
{
	PG_GuildHouses_LtoC_HouseInfoLoadOK* PG = (PG_GuildHouses_LtoC_HouseInfoLoadOK*)Data;
	This->RL_HouseInfoLoadOK( );
}

void NetCli_GuildHouses::SL_VisitHouseRequest	( int GuildID )
{
	PG_GuildHouses_CtoL_VisitHouseRequest Send;
	Send.GuildID = GuildID;

	SendToLocal_Base( sizeof( Send ) , &Send );
}
void NetCli_GuildHouses::SL_CloseVisitHouse		()
{
	PG_GuildHouses_CtoL_CloseVisitHouse Send;
	SendToLocal_Base( sizeof( Send ) , &Send );
}
void NetCli_GuildHouses::SL_LeaveHouse			( int PlayerDBID )
{
	PG_GuildHouses_CtoL_LeaveHouse Send;
	Send.PlayerDBID = PlayerDBID;
	SendToLocal_Base( sizeof( Send ) , &Send );
}
void NetCli_GuildHouses::SL_CreateBuildingRequest	( char* Point , int BuildingID , int ParentBuildingDBID , float Dir )
{
	PG_GuildHouses_CtoL_CreateBuildingRequest Send;
	strncpy( Send.Point , Point , sizeof( Send.Point ) );
	Send.BuildingID = BuildingID;
	Send.ParentBuildingDBID = ParentBuildingDBID;
	Send.Dir = Dir;
	SendToLocal_Base( sizeof( Send ) , &Send );
}
void NetCli_GuildHouses::SL_BuildingUpgradeRequest	( int BuildingDBID , int UpgradeBuildingID )
{
	PG_GuildHouses_CtoL_BuildingUpgradeRequest Send;
	Send.BuildingDBID = BuildingDBID;
	Send.UpgradeBuildingID = UpgradeBuildingID;
	SendToLocal_Base( sizeof( Send ) , &Send );
}
void NetCli_GuildHouses::SL_DeleteBuildingRequest	( int BuildingDBID )
{
	PG_GuildHouses_CtoL_DeleteBuildingRequest Send;
	Send.BuildingDBID = BuildingDBID;
	SendToLocal_Base( sizeof( Send ) , &Send );
}

void NetCli_GuildHouses::SL_AllBuildingInfoRequest( int GuildID )
{
	PG_GuildHouses_CtoL_AllBuildingInfoRequest Send;
	Send.GuildID = GuildID;
	SendToLocal_Base( sizeof( Send ) , &Send );
}

void NetCli_GuildHouses::SL_HouseItemRequest		( int PageID )
{
	PG_GuildHouses_CtoL_HouseItemRequest Send;
	Send.PageID = PageID;
	SendToLocal_Base( sizeof( Send ) , &Send );
}
void NetCli_GuildHouses::SL_SwapBodyHouseItem		( int BodyPos , int HouseItemPageID , int HouseItemPos , bool IsFromBody )
{
	PG_GuildHouses_CtoL_SwapBodyHouseItem Send;
	Send.BodyPos = BodyPos;
	Send.HouseItemPageID=HouseItemPageID;
	Send.HouseItemPos=HouseItemPos;
	Send.IsFromBody=IsFromBody;

	SendToLocal_Base( sizeof( Send ) , &Send );
}
void NetCli_GuildHouses::SL_SwapHouseItemRequest	( int HouseItemPos1 , int HouseItemPos2 , int HouseItemPageID1 , int HouseItemPageID2 )
{
	PG_GuildHouses_CtoL_SwapHouseItemRequest Send;
	Send.HouseItemPos[0] = HouseItemPos1;
	Send.HouseItemPos[1] = HouseItemPos2;
	Send.HouseItemPageID[0] = HouseItemPageID1;
	Send.HouseItemPageID[1] = HouseItemPageID2;
	SendToLocal_Base( sizeof( Send ) , &Send );
}

//////////////////////////////////////////////////////////////////////////
//¤½·|³Ã­Ñ
//////////////////////////////////////////////////////////////////////////
void NetCli_GuildHouses::SL_SwapBodyHouseFurniture		( int BodyPos , int HousePos )
{
	PG_GuildHouses_CtoL_SwapBodyHouseFurniture Send;
	Send.BodyPos = BodyPos;
	Send.HousePos = HousePos;
	SendToLocal_Base( sizeof( Send ) , &Send );
}
void NetCli_GuildHouses::SL_SwapHouseFurnitureRequest	( int Pos1 , int Pos2 )
{
	PG_GuildHouses_CtoL_SwapHouseFurnitureRequest Send;
	Send.Pos[0] = Pos1;
	Send.Pos[1] = Pos2;
	SendToLocal_Base( sizeof( Send ) , &Send );
}
void NetCli_GuildHouses::SL_HouseItemLayoutRequest		( int BuildingObjID , int Pos , House3DLayoutStruct& Layout )
{
	PG_GuildHouses_CtoL_HouseItemLayoutRequest Send;
	Send.BuildingObjID = BuildingObjID;
	Send.Pos = Pos;
	Send.Layout = Layout;
	SendToLocal_Base( sizeof( Send ) , &Send );
}

void NetCli_GuildHouses::SL_SetStoreConfig	( GuildStorehouseRightStruct& Config )
{
	PG_GuildHouses_CtoL_SetStoreConfig Send;
	Send.Config = Config;
	SendToLocal_Base( sizeof( Send ) , &Send );
}

void NetCli_GuildHouses::SL_ItemLogRequest( int PageID , int DayBefore  )
{
	PG_GuildHouses_CtoL_ItemLogRequest Send;
	Send.Day = DayBefore;
	Send.PageID = PageID;
	SendToLocal_Base( sizeof( Send ) , &Send );
}

void NetCli_GuildHouses::SL_ActiveBuildingRequest	( int BuildingDBID , bool IsActive )
{
	PG_GuildHouses_CtoL_ActiveBuildingRequest Send;
	Send.BuildDBID = BuildingDBID;
	Send.IsActive = IsActive;
	SendToLocal_Base( sizeof( Send ) , &Send );
}

void NetCli_GuildHouses::SL_BuyItemPageRequest( int PageID )
{
	PG_GuildHouses_CtoL_BuyItemPageRequest Send;
	Send.PageID = PageID;
	SendToLocal_Base( sizeof( Send ) , &Send );
}

void NetCli_GuildHouses::SL_FurnitureSendBack( int Pos )
{
	PG_GuildHouses_CtoL_FurnitureSendBack Send;
	Send.Pos = Pos;
	SendToLocal_Base( sizeof( Send ) , &Send );
}
void NetCli_GuildHouses::SL_BuyHouseStyle		( int Type )
{
	PG_GuildHouses_CtoL_BuyHouseStyle Send;
	Send.Type = Type;
	SendToLocal_Base( sizeof( Send ) , &Send );
}
/*
void NetCli_GuildHouses::SL_SetHouseStyle		( int Type )
{
	PG_GuildHouses_CtoL_SetHouseStyle Send;
	Send.Type = Type;
	SendToLocal_Base( sizeof( Send ) , &Send );
}
*/
void NetCli_GuildHouses::SL_HouseStyleRequest	( )
{
	PG_GuildHouses_CtoL_HouseStyleRequest Send;
	SendToLocal_Base( sizeof( Send ) , &Send );
}