#pragma once

#include "NetCli_GuildHouses.h"

class NetCli_GuildHousesChild : public NetCli_GuildHouses
{
public:	
	static	bool _Init();
	static	bool _Release();

public:

	virtual void RL_BuildHouseResult		( int GuildID , bool Result );
	virtual void RL_OpenVisitHouse			( int TargetNpcID , bool IsVisitMyGuildHouse );
	virtual void RL_FindHouseResult			( bool Result );
	virtual void RL_CloseVisitHouse			( );
	virtual void RL_HouseInfoLoading		( int GuildID );
	virtual void RL_HouseInfoLoadOK			( );

	virtual void RL_CreateBuildingResult	( char* Point , int BuildingID , int BuildingDBID, int ParentBuildingDBID , bool Result );
	virtual void RL_BuildingUpgradeResult	( int GItemID , int BuildingDBID , int UpgradeBuildingID , int Result );
	virtual void RL_DeleteBuildingResult	( int BuildingDBID , int Result );
	virtual void RL_AllBuildingInfo			( int GuildID , int Count , GuildBuildingObjectInfoStruct* Building );	
	virtual void RL_NewBuildingInfo			( GuildBuildingObjectInfoStruct& Building );
	virtual void RL_DelBuildingInfo			( int BuildingDBID );

	virtual void RL_HouseItemInfo			( int PageID , int MaxPage , int GetCount , int ItemCount , GuildHouseItemStruct	Item[ 100 ] );
	virtual void RL_SwapBodyHouseItemResult	( int BodyPos , int	HouseItemPageID , int HouseItemPos , int GetCount , bool	Result );
	virtual void RL_FixHouseItem			( GuildHouseItemStruct&	Item );
	virtual void RL_SwapHouseItemResult		( int HouseItemPos[2] , int HouseItemPageID[2] , bool Result );
	virtual void RL_BuyItemPageResult		( int PageID , bool Result );

	virtual void RL_FurnitureItemInfo		( GuildHouseFurnitureItemStruct& Item );
	virtual void RL_SwapBodyHouseFurnitureResult( int BodyPos , int HousePos, bool Result );
	virtual void RL_FixHouseFurniture		( GuildHouseFurnitureItemStruct& Item );
	virtual void RL_SwapHouseFurnitureResult( int Pos[2] , bool Result );
	virtual void RL_HouseItemLayoutResult	( int Pos , bool Result );
	virtual void RL_FurnitureSendBackResult	( int Pos , bool Result );

	virtual void RL_StoreConfig				( GuildStorehouseRightStruct& Config );
	virtual void RL_SetStoreConfigResult	( bool Result );

	virtual void RL_ItemLogList				( int Count , GuildItemStoreLogStruct Log[1000] );
	virtual void RL_ItemLog					( GuildItemStoreLogStruct& ItemLog );
	virtual void RL_ActiveBuildingResult	( int BuildDBID , bool IsActive , bool Result );
	virtual void RL_BuildingActiveType		( int BuildDBID , bool IsActive );

	virtual void RL_BuyHouseStyleResult		( int Type , bool Result );
//	virtual void RL_SetHouseStyleResult		( bool Result );
	virtual void RL_HouseStyle				( int Type );
};

