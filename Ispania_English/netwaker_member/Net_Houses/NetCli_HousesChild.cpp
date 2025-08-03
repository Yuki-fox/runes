#include "../../mainproc/GameMain.h"
#include "../../Interface/WorldFrames/ChatFrame.h"
#include "NetCli_HousesChild.h"
#include "../interface/debug/DebugMessage.h"
#include "../../Interface/Houses/HousesFrame.h"

//-----------------------------------------------------------------------------
bool NetCli_HousesChild::_Init()
{
	if (This)
		return false;

	This = new NetCli_HousesChild;


	return NetCli_Houses::_Init();
}
//-----------------------------------------------------------------------------
bool NetCli_HousesChild::_Release()
{
	NetCli_Houses::_Release();
	if (This)
	{
		delete This;
		This = NULL;
	}
	return true;
}
//-----------------------------------------------------------------------------
void NetCli_HousesChild::R_BuildHouseResult				( int HouseDBID , bool Result )
{
	g_pDebugMessage->DebugOutput( 2 , "R_BuildHouseResult HouseDBID=%d Result=%d" , HouseDBID , Result );
	g_pHousesFrame->R_BuildHouseResult( HouseDBID, Result );
}
void NetCli_HousesChild::R_OpenVisitHouse				( int TargetNpcID , bool IsVisitMyHouse )
{
	g_pDebugMessage->DebugOutput( 2 , "R_OpenVisitHouse TargetNpcID=%d" , TargetNpcID);
	g_pHousesFrame->R_OpenVisitHouse( TargetNpcID, IsVisitMyHouse );
}
void NetCli_HousesChild::R_CloseVisitHouse				()
{
	g_pDebugMessage->DebugOutput( 2 , "R_CloseVisitHouse" );
	g_pHousesFrame->R_CloseVisitHouse( );
}
void NetCli_HousesChild::R_FindHouseResult				( FindHouseResultENUM Result )
{
	g_pDebugMessage->DebugOutput( 2 , "R_FindHouseResult Result=%d" , Result);
	g_pHousesFrame->R_FindHouseResult( Result );
}
void NetCli_HousesChild::R_HouseInfoLoading				()
{
	g_pDebugMessage->DebugOutput( 2 , "R_HouseInfoLoading" );
	g_pHousesFrame->R_HouseInfoLoading( );
}
void NetCli_HousesChild::R_HouseBaseInfo				( HouseBaseDBStruct& HouseBaseInfo )
{
	g_pDebugMessage->DebugOutput( 2 , "R_HouseBaseInfo" );	
	g_pHousesFrame->R_HouseBaseInfo( HouseBaseInfo );
}
void NetCli_HousesChild::R_ItemInfo						( int ItemDBID , int ItemCount , HouseItemDBStruct Item[_DEF_MAX_HOUSE_FURNITURE] )
{
	g_pDebugMessage->DebugOutput( 2 , "R_ItemInfo ItemCount=%d" , ItemCount );
	g_pHousesFrame->R_ItemInfo( ItemDBID, ItemCount, Item );
}
void NetCli_HousesChild::R_HouseInfoLoadOK				()
{
	g_pDebugMessage->DebugOutput( 2 , "R_HouseInfoLoadOK" );
	g_pHousesFrame->R_HouseInfoLoadOK( );
}
void NetCli_HousesChild::R_SwapBodyHouseItemResult		( int HouseParentItemDBID , int HousePos , int BodyPos , bool Result )
{
	g_pDebugMessage->DebugOutput( 2 , "R_SwapBodyHouseItemResult HouseParentItemDBID=%d HousePos=%d BodyPos=%d Result=%d" , HouseParentItemDBID , HousePos , BodyPos , Result );
	g_pHousesFrame->R_SwapBodyHouseItemResult( HouseParentItemDBID, HousePos, BodyPos, Result );
}
void NetCli_HousesChild::R_FixHouseItem					( HouseItemDBStruct& HouseItem )
{
	g_pDebugMessage->DebugOutput( 2 , "R_FixHouseItem" );
	g_pHousesFrame->R_FixHouseItem( HouseItem );
}
void NetCli_HousesChild::R_SwapHouseItemResult			( int ParentItemDBID[2] , int HousePos[2] , bool Result )
{
	g_pDebugMessage->DebugOutput( 2 , "R_SwapHouseItemResult HousePos1=%d HousePos2=%d Result=%d" , HousePos[0] , HousePos[1] , Result );
	g_pHousesFrame->R_SwapHouseItemResult( ParentItemDBID , HousePos , Result );
}
void NetCli_HousesChild::R_HouseItemLayoutResult	( int HouseItemDBID , bool Result )
{
	g_pDebugMessage->DebugOutput( 2 , "R_HouseItemLayoutResult HouseItemDBID=%d Result=%d" , HouseItemDBID , Result );
	g_pHousesFrame->R_HouseItemLayoutResult( HouseItemDBID, Result );
}
void NetCli_HousesChild::R_ChangeHouseResult			( int HouseType , bool Result )
{
	g_pDebugMessage->DebugOutput( 2 , "R_ChangeHouseResult HouseType=%d Result=%d" , HouseType , Result );
	g_pHousesFrame->R_ChangeHouseResult( HouseType, Result );
}
void NetCli_HousesChild::R_BuyFunctionResult			( int FunctionID , bool Result )
{
	g_pDebugMessage->DebugOutput( 2 , "R_BuyFunctionResult FunctionID=%d Result=%d" , FunctionID , Result );
	g_pHousesFrame->R_BuyFunctionResult( FunctionID, Result );
}
void NetCli_HousesChild::R_FunctionSettingResult		( int FunctionID , int Mode , bool Result )
{
	g_pDebugMessage->DebugOutput( 2 , "R_FunctionSettingResult FunctionID=%d Mode=%d" , FunctionID , Mode );
	g_pHousesFrame->R_FunctionSettingResult( FunctionID, Mode,Result );
}
void NetCli_HousesChild::R_BuyEnergyResult				( int Money_Account , int Energy )
{
	g_pDebugMessage->DebugOutput( 2 , "R_BuyEnergyResult Money_Account=%d Energy=%d" , Money_Account , Energy );
	g_pHousesFrame->R_BuyEnergyResult( Money_Account, Energy );
}
void NetCli_HousesChild::R_BoxInfoResult				( int Result )
{

}
void NetCli_HousesChild::R_ExchangeEqResult				( )
{

}
void NetCli_HousesChild::R_SetPasswordResult			( bool Result )
{
	g_pHousesFrame->R_SetPasswordResult( Result );
}
void NetCli_HousesChild::R_DestroyItemResult			( bool Result )
{

}

void NetCli_HousesChild::R_SetHouseNameResult			( SetHouseNameResultENUM Result )
{

	switch( Result )
	{
	case EM_SetHouseNameResult_OK:
		{
			g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString( "SYS_HOUSE_SET_NAME_SUCCEED" ) );

			int energy = g_ObjectData->GetSysKeyValue( "House_ChangeName_Energy" );
			g_pGameMain->SendSystemChatMsgEx( g_ObjectData->GetString( "UI_HOUSEFRAME_COST_ENERGY" ) , energy );

		}
  		break;
	case EM_SetHouseNameResult_NameErr:
		g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString( "SYS_HOUSE_SET_NAME_FAIL4" ) );
		break;
	case EM_SetHouseNameResult_EnergyErr:
   		g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString( "SYS_HOUSE_SET_NAME_FAIL1" ) );
	    break;
	//case EM_SetHouseNameResult_DataErr:
	//	g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString( "SYS_HOUSE_SET_NAME_FAIL5" ) );
	//	break;
	default:
  		g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString( "SYS_HOUSE_SET_NAME_FAIL5" ) );
	    break;
	}

}
//////////////////////////////////////////////////////////////////////////
//種植
//////////////////////////////////////////////////////////////////////////
void NetCli_HousesChild::R_PlantClearResult				( int HousePos , PlantClearResultTypeENUM Result )
{
	g_pHousesFrame->R_PlantClearResult( HousePos, Result );
}
void NetCli_HousesChild::R_PlantItemResult				( PlantItemTypeENUM Type , int PotHousePos , int ItemBodyPos , PlantItemResultTypeENUM Result  )
{
	g_pHousesFrame->R_PlantItemResult( Type, PotHousePos, ItemBodyPos, Result );
}
void NetCli_HousesChild::R_PlantLockResult				( int PotHousePos , PlantLockResultTypeENUM Result )
{
	g_pHousesFrame->R_PlantLockResult( PotHousePos, Result );
}
void NetCli_HousesChild::R_PlantGetProductResult		( int PotHousePos , PlantGetProductResultTypeENUM Result )
{
	g_pHousesFrame->R_PlantGetProductResult( PotHousePos, Result );
}

void NetCli_HousesChild::R_FriendDBID					( int HouseDBID , int FriendDBID )
{
	g_pHousesFrame->R_FriendDBID( HouseDBID, FriendDBID );
}

void NetCli_HousesChild::R_DeleteFriendDBID				( int FriendDBID )
{
	g_pHousesFrame->R_DeleteFriendDBID( FriendDBID );
}

void NetCli_HousesChild::R_AddFriendDBID				( int FriendDBID )
{
	g_pHousesFrame->R_AddFriendDBID( FriendDBID );
}

void NetCli_HousesChild::R_FriendItemLog				( HouseFriendItemLogStruct& Log )
{
	g_pHousesFrame->R_FriendItemLog( Log );
}

void NetCli_HousesChild::R_ClearItemLogResult			( bool Result )
{

}

//招募列表回應
void NetCli_HousesChild::RL_ServantHireList( int ID , bool End , HouseServantStruct& Servant )
{
	g_pHousesFrame->RL_ServantHireList( ID , End , Servant );
}
//招募女僕結果
void NetCli_HousesChild::RL_ServantHireResult( bool Result , int ID , int Pos , HouseServantStruct& Servant )
{
	g_pHousesFrame->RL_ServantHireResult( Result , ID , Pos , Servant );
}
//女僕刪除結果
void NetCli_HousesChild::RL_ServantDelResult( int ServantID , bool Result )
{
	g_pHousesFrame->RL_ServantDelResult( ServantID , Result );
}
//女僕互動事件通知
void NetCli_HousesChild::RL_ServantEvent( int ServantID , int EventID )
{
	g_pHousesFrame->RL_ServantEvent( ServantID , EventID );
}
//女僕命名結果
void NetCli_HousesChild::RL_ServantRenameResult( const char* Name , bool Result )
{
	g_pHousesFrame->RL_ServantRenameResult( Name , Result );
}

//修正女僕數值
void NetCli_HousesChild::RL_FixServantValue( int ServantID , HouseServantValueTypeENUM Type , int Value )
{
	g_pHousesFrame->RL_FixServantValue( ServantID, Type, Value );
}