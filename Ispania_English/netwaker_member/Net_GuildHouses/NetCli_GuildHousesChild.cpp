#include "../../mainproc/GameMain.h"
#include "NetCli_GuildHousesChild.h"

#include "../../Interface/GuildHouses/GuildHousesFrame.h"

//-----------------------------------------------------------------------------
bool NetCli_GuildHousesChild::_Init()
{
	if (This)
		return false;

	This = new NetCli_GuildHousesChild;


	return NetCli_GuildHouses::_Init();
}
//-----------------------------------------------------------------------------
bool NetCli_GuildHousesChild::_Release()
{
	NetCli_GuildHouses::_Release();
	if (This)
	{
		delete This;
		This = NULL;
	}
	return true;
}
//-----------------------------------------------------------------------------
void NetCli_GuildHousesChild::RL_BuildHouseResult	( int GuildID , bool Result )
{
	g_pGuildHousesFrame->RL_BuildHouseResult( GuildID , Result );
}
void NetCli_GuildHousesChild::RL_OpenVisitHouse		( int TargetNpcID , bool IsVisitMyGuildHouse )
{
	g_pGuildHousesFrame->RL_OpenVisitHouse( TargetNpcID , IsVisitMyGuildHouse );
}
void NetCli_GuildHousesChild::RL_FindHouseResult	( bool Result )
{
	g_pGuildHousesFrame->RL_FindHouseResult( Result );
}
void NetCli_GuildHousesChild::RL_CloseVisitHouse	( )
{
	g_pGuildHousesFrame->RL_CloseVisitHouse();
}
void NetCli_GuildHousesChild::RL_HouseInfoLoading	( int GuildID )
{
	g_pGuildHousesFrame->RL_HouseInfoLoading( GuildID );
}
void NetCli_GuildHousesChild::RL_HouseInfoLoadOK	( )
{
	g_pGuildHousesFrame->RL_HouseInfoLoadOK();
}
void NetCli_GuildHousesChild::RL_CreateBuildingResult	( char* Point , int BuildingID , int BuildingDBID, int ParentBuildingDBID , bool Result )
{
	g_pGuildHousesFrame->RL_CreateBuildingResult( Point , BuildingID , BuildingDBID, ParentBuildingDBID , Result );
}
void NetCli_GuildHousesChild::RL_BuildingUpgradeResult	( int GItemID , int BuildingDBID , int UpgradeBuildingID , int Result )
{
	g_pGuildHousesFrame->RL_BuildingUpgradeResult( GItemID , BuildingDBID , UpgradeBuildingID , Result );
}
void NetCli_GuildHousesChild::RL_DeleteBuildingResult	( int BuildingDBID , int Result )
{
	g_pGuildHousesFrame->RL_DeleteBuildingResult( BuildingDBID , Result );
}
void NetCli_GuildHousesChild::RL_AllBuildingInfo		( int GuildID , int Count , GuildBuildingObjectInfoStruct* Building )
{
  	g_pGuildHousesFrame->RL_AllBuildingInfo( GuildID , Count , Building );
}
void NetCli_GuildHousesChild::RL_NewBuildingInfo		( GuildBuildingObjectInfoStruct& Building )
{
	g_pGuildHousesFrame->RL_NewBuildingInfo( Building );
}
void NetCli_GuildHousesChild::RL_DelBuildingInfo		( int BuildingDBID )
{
	g_pGuildHousesFrame->RL_DelBuildingInfo( BuildingDBID );
}
void NetCli_GuildHousesChild::RL_HouseItemInfo			( int PageID , int MaxPage , int GetCount , int ItemCount , GuildHouseItemStruct	Item[ 100 ] )
{
	g_pGuildHousesFrame->RL_HouseItemInfo( PageID, MaxPage, GetCount, ItemCount, Item );
}
void NetCli_GuildHousesChild::RL_SwapBodyHouseItemResult	( int BodyPos , int	HouseItemPageID , int HouseItemPos , int GetCount , bool	Result )
{
	g_pGuildHousesFrame->RL_SwapBodyHouseItemResult( BodyPos, HouseItemPageID, HouseItemPos, GetCount, Result );
}
void NetCli_GuildHousesChild::RL_FixHouseItem			( GuildHouseItemStruct&	Item )
{
	g_pGuildHousesFrame->RL_FixHouseItem( Item );
}
void NetCli_GuildHousesChild::RL_SwapHouseItemResult		( int HouseItemPos[2] , int HouseItemPageID[2] , bool Result )
{
	g_pGuildHousesFrame->RL_SwapHouseItemResult( HouseItemPos, HouseItemPageID, Result );
}
void NetCli_GuildHousesChild::RL_BuyItemPageResult		( int PageID , bool Result )
{
	g_pGuildHousesFrame->RL_BuyItemPageResult( PageID, Result );
}
void NetCli_GuildHousesChild::RL_FurnitureItemInfo( GuildHouseFurnitureItemStruct& Item )
{
	g_pGuildHousesFrame->RL_FurnitureItemInfo( Item );
}
void NetCli_GuildHousesChild::RL_SwapBodyHouseFurnitureResult( int BodyPos , int HousePos, bool Result )
{
	g_pGuildHousesFrame->RL_SwapBodyHouseFurnitureResult( BodyPos, HousePos, Result );
}
void NetCli_GuildHousesChild::RL_FixHouseFurniture( GuildHouseFurnitureItemStruct& Item )
{
	g_pGuildHousesFrame->RL_FixHouseFurniture( Item );
}
void NetCli_GuildHousesChild::RL_SwapHouseFurnitureResult( int Pos[2] , bool Result )
{
	g_pGuildHousesFrame->RL_SwapHouseFurnitureResult( Pos, Result );
}
void NetCli_GuildHousesChild::RL_HouseItemLayoutResult( int Pos , bool Result )
{
	g_pGuildHousesFrame->RL_HouseItemLayoutResult( Pos, Result );
}
void NetCli_GuildHousesChild::RL_StoreConfig( GuildStorehouseRightStruct& Config )
{
	g_pGuildHousesFrame->RL_StoreConfig( Config );
}
void NetCli_GuildHousesChild::RL_SetStoreConfigResult( bool Result )
{
	g_pGuildHousesFrame->RL_SetStoreConfigResult( Result );
}
void NetCli_GuildHousesChild::RL_ItemLogList( int Count , GuildItemStoreLogStruct Log[1000] )
{
	g_pGuildHousesFrame->RL_ItemLogList( Count, Log );
}
void NetCli_GuildHousesChild::RL_ItemLog					( GuildItemStoreLogStruct& ItemLog )
{
	g_pGuildHousesFrame->RL_ItemLog( ItemLog );
}
void NetCli_GuildHousesChild::RL_FurnitureSendBackResult	( int Pos , bool Result )
{

}
void NetCli_GuildHousesChild::RL_ActiveBuildingResult	( int BuildDBID , bool IsActive , bool Result )
{
	g_pGuildHousesFrame->RL_ActiveBuildingResult( BuildDBID, IsActive, Result );
}
void NetCli_GuildHousesChild::RL_BuildingActiveType		( int BuildDBID , bool IsActive )
{
	g_pGuildHousesFrame->RL_BuildingActiveType(  BuildDBID,  IsActive );
}
void NetCli_GuildHousesChild::RL_BuyHouseStyleResult		( int Type , bool Result )
{

	g_pGuildHousesFrame->BuyHouseStyleResult( Type , Result );
}
/*void NetCli_GuildHousesChild::RL_SetHouseStyleResult		( bool Result )
{

}
*/
void NetCli_GuildHousesChild::RL_HouseStyle				( int Type )
{

	g_pGuildHousesFrame->HouseStyle(Type);
}