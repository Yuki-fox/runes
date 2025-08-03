#pragma once

#include "PG/PG_GuildHouses.h"
#include "../../MainProc/NetGlobal.h"

class NetCli_GuildHouses : public CNetGlobal
{
protected:
    //-------------------------------------------------------------------
    static	NetCli_GuildHouses* This;
    static	bool _Init();
    static	bool _Release();	
    //-------------------------------------------------------------------
	static void _PG_GuildHouses_LtoC_BuildHouseResult				( int Sockid , int Size , void* Data );
	static void _PG_GuildHouses_LtoC_OpenVisitHouse					( int Sockid , int Size , void* Data );
	static void _PG_GuildHouses_LtoC_FindHouseResult				( int Sockid , int Size , void* Data );
	static void _PG_GuildHouses_LtoC_CloseVisitHouse				( int Sockid , int Size , void* Data );
	static void _PG_GuildHouses_LtoC_HouseInfoLoading				( int Sockid , int Size , void* Data );
	static void _PG_GuildHouses_LtoC_HouseInfoLoadOK				( int Sockid , int Size , void* Data );

	static void _PG_GuildHouses_LtoC_CreateBuildingResult			( int Sockid , int Size , void* Data );
	static void _PG_GuildHouses_LtoC_BuildingUpgradeResult			( int Sockid , int Size , void* Data );
	static void _PG_GuildHouses_LtoC_DeleteBuildingResult			( int Sockid , int Size , void* Data );
	static void _PG_GuildHouses_LtoC_AllBuildingInfo				( int Sockid , int Size , void* Data );

	static void _PG_GuildHouses_LtoC_NewBuildingInfo				( int Sockid , int Size , void* Data );
	static void _PG_GuildHouses_LtoC_DelBuildingInfo				( int Sockid , int Size , void* Data );

	static void _PG_GuildHouses_LtoC_HouseItemInfo					( int Sockid , int Size , void* Data );
	static void _PG_GuildHouses_LtoC_SwapBodyHouseItemResult		( int Sockid , int Size , void* Data );
	static void _PG_GuildHouses_LtoC_FixHouseItem					( int Sockid , int Size , void* Data );
	static void _PG_GuildHouses_LtoC_SwapHouseItemResult			( int Sockid , int Size , void* Data );

	static void _PG_GuildHouses_LtoC_FurnitureItemInfo				( int Sockid , int Size , void* Data );
	static void _PG_GuildHouses_LtoC_SwapBodyHouseFurnitureResult	( int Sockid , int Size , void* Data );
	static void _PG_GuildHouses_LtoC_FixHouseFurniture				( int Sockid , int Size , void* Data );
	static void _PG_GuildHouses_LtoC_SwapHouseFurnitureResult		( int Sockid , int Size , void* Data );
	static void _PG_GuildHouses_LtoC_HouseItemLayoutResult			( int Sockid , int Size , void* Data );

	static void _PG_GuildHouses_LtoC_StoreConfig					( int Sockid , int Size , void* Data );
	static void _PG_GuildHouses_LtoC_SetStoreConfigResult			( int Sockid , int Size , void* Data );

	static void _PG_GuildHouses_LtoC_ItemLog						( int Sockid , int Size , void* Data );
	static void _PG_GuildHouses_LtoC_ItemLogList					( int Sockid , int Size , void* Data );

	static void _PG_GuildHouses_LtoC_ActiveBuildingResult			( int Sockid , int Size , void* Data );
	static void _PG_GuildHouses_LtoC_BuyItemPageResult				( int Sockid , int Size , void* Data );
	static void _PG_GuildHouses_LtoC_BuildingActiveType				( int Sockid , int Size , void* Data );
	static void _PG_GuildHouses_LtoC_FurnitureSendBackResult		( int Sockid , int Size , void* Data );
	static void _PG_GuildHouses_LtoC_BuyHouseStyleResult			( int Sockid , int Size , void* Data );
//	static void _PG_GuildHouses_LtoC_SetHouseStyleResult			( int Sockid , int Size , void* Data );
	static void _PG_GuildHouses_LtoC_HouseStyle						( int Sockid , int Size , void* Data );
public:    
    NetCli_GuildHouses() { This = this; }
	static	NetCli_GuildHouses*	GetThis()						{ return This; }

	virtual void RL_BuildHouseResult	( int GuildID , bool Result ) = 0;
	virtual void RL_OpenVisitHouse		( int TargetNpcID , bool IsVisitMyGuildHouse ) = 0;
	virtual void RL_FindHouseResult		( bool Result ) = 0;
	virtual void RL_CloseVisitHouse		( ) = 0;
	virtual void RL_HouseInfoLoading	( int GuildID ) = 0;
	virtual void RL_HouseInfoLoadOK		( ) = 0;

	static void SL_VisitHouseRequest	( int GuildID );
	static void SL_CloseVisitHouse		( );
	static void SL_LeaveHouse			( int PlayerDBID );

	//////////////////////////////////////////////////////////////////////////
	//公會建築
	//////////////////////////////////////////////////////////////////////////
	static void SL_CreateBuildingRequest	( char* Point , int BuildingID , int ParentBuildingDBID , float Dir );
	static void SL_BuildingUpgradeRequest	( int BuildingDBID , int UpgradeBuildingID );
	static void SL_DeleteBuildingRequest	( int BuildingDBID );
	static void SL_ActiveBuildingRequest	( int BuildingDBID , bool IsActive );

	virtual void RL_CreateBuildingResult	( char* Point , int BuildingID , int BuildingDBID, int ParentBuildingDBID , bool Result ) = 0;
	virtual void RL_BuildingUpgradeResult	( int GItemID , int BuildingDBID , int UpgradeBuildingID , int Result ) = 0;
	virtual void RL_DeleteBuildingResult	( int BuildingDBID , int Result ) = 0;

	virtual void RL_ActiveBuildingResult	( int BuildDBID , bool IsActive , bool Result ) = 0;
	virtual void RL_BuildingActiveType		( int BuildDBID , bool IsActive ) = 0;
	/////////////////////////////////////////////////////////////////////////
	//要求公會全部的建築資訊
	//////////////////////////////////////////////////////////////////////////
	virtual void RL_AllBuildingInfo( int GuildID , int Count , GuildBuildingObjectInfoStruct* Building ) = 0;
	virtual void RL_NewBuildingInfo( GuildBuildingObjectInfoStruct& Building ) = 0;
	virtual void RL_DelBuildingInfo( int BuildingDBID ) = 0;

	static void SL_AllBuildingInfoRequest( int GuildID );
	//////////////////////////////////////////////////////////////////////////
	//公會倉庫
	//////////////////////////////////////////////////////////////////////////
	static void SL_HouseItemRequest			( int PageID );
	static void SL_SwapBodyHouseItem		( int BodyPos , int HouseItemPageID , int HouseItemPos , bool IsFromBody );
	static void SL_SwapHouseItemRequest		( int HouseItemPos1 , int HouseItemPos2 , int HouseItemPageID1 , int HouseItemPageID2 );
	static void SL_BuyItemPageRequest		( int PageID );

	virtual void RL_HouseItemInfo			( int PageID , int MaxPage , int GetCount , int ItemCount , GuildHouseItemStruct	Item[ 100 ] ) = 0;
	virtual void RL_SwapBodyHouseItemResult	( int BodyPos , int	HouseItemPageID , int HouseItemPos , int GetCount , bool	Result ) = 0;
	virtual void RL_FixHouseItem			( GuildHouseItemStruct&	Item ) = 0;
	virtual void RL_SwapHouseItemResult		( int HouseItemPos[2] , int HouseItemPageID[2] , bool Result ) = 0;
	virtual void RL_BuyItemPageResult		( int PageID , bool Result ) = 0;	
	//////////////////////////////////////////////////////////////////////////
	//公會傢俱
	//////////////////////////////////////////////////////////////////////////
	static void SL_SwapBodyHouseFurniture	( int BodyPos , int HousePos );
	static void SL_SwapHouseFurnitureRequest( int Pos1 , int Pos2 );
	static void SL_HouseItemLayoutRequest	( int BuildingObjID , int Pos , House3DLayoutStruct& Layout );
	static void SL_FurnitureSendBack		( int Pos );

	virtual void RL_FurnitureItemInfo( GuildHouseFurnitureItemStruct& Item ) = 0;
	virtual void RL_SwapBodyHouseFurnitureResult( int BodyPos , int HousePos, bool Result ) = 0;
	virtual void RL_FixHouseFurniture( GuildHouseFurnitureItemStruct& Item ) = 0;
	virtual void RL_SwapHouseFurnitureResult( int Pos[2] , bool Result ) = 0;
	virtual void RL_HouseItemLayoutResult( int Pos , bool Result ) = 0;
	virtual void RL_FurnitureSendBackResult( int Pos , bool Result ) = 0;
	//////////////////////////////////////////////////////////////////////////
	//公會倉庫權限設定
	//////////////////////////////////////////////////////////////////////////
	virtual void RL_StoreConfig( GuildStorehouseRightStruct& Config ) = 0;
	virtual void RL_SetStoreConfigResult( bool Result ) = 0;

	static void SL_SetStoreConfig	( GuildStorehouseRightStruct& Config );
	//////////////////////////////////////////////////////////////////////////
	//公會倉庫Log
	//////////////////////////////////////////////////////////////////////////
	static void SL_ItemLogRequest( int PageID , int DayBefore  );
	virtual void RL_ItemLogList( int Count , GuildItemStoreLogStruct Log[1000] ) = 0;
	virtual void RL_ItemLog( GuildItemStoreLogStruct& ItemLog ) = 0;
	//////////////////////////////////////////////////////////////////////////
	//工會樣式
	static void SL_BuyHouseStyle		( int Type );
	virtual void RL_BuyHouseStyleResult	( int Type , bool Result ) = 0;
//	static void SL_SetHouseStyle		( int Type );
//	virtual void RL_SetHouseStyleResult	( bool Result ) = 0;
	static void SL_HouseStyleRequest	( );
	virtual void RL_HouseStyle			( int Type ) = 0;

};

