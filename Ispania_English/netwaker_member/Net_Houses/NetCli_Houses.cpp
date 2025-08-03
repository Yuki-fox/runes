#include "NetCli_Houses.h"

//-------------------------------------------------------------------
NetCli_Houses*    NetCli_Houses::This         = NULL;

//-------------------------------------------------------------------
bool NetCli_Houses::_Init()
{
 
	Cli_NetReg( PG_Houses_LtoC_BuildHouseResult			 );
	Cli_NetReg( PG_Houses_LtoC_OpenVisitHouse			 );
	Cli_NetReg( PG_Houses_LtoC_CloseVisitHouse			 );
	Cli_NetReg( PG_Houses_LtoC_FindHouseResult			 );
	Cli_NetReg( PG_Houses_LtoC_HouseInfoLoading			 );
	Cli_NetReg( PG_Houses_LtoC_HouseBaseInfo			 );
	Cli_NetReg( PG_Houses_LtoC_ItemInfo					 );
	Cli_NetReg( PG_Houses_LtoC_HouseInfoLoadOK			 );
	Cli_NetReg( PG_Houses_LtoC_SwapBodyHouseItemResult	 );
	Cli_NetReg( PG_Houses_LtoC_FixHouseItem				 );
	Cli_NetReg( PG_Houses_LtoC_SwapHouseItemResult		 );
	Cli_NetReg( PG_Houses_LtoC_HouseItemLayoutResult	 );
	Cli_NetReg( PG_Houses_LtoC_ChangeHouseResult		 );
	Cli_NetReg( PG_Houses_LtoC_BuyFunctionResult		 );
	Cli_NetReg( PG_Houses_LtoC_FunctionSettingResult	 );
	Cli_NetReg( PG_Houses_LtoC_BuyEnergyResult			 );
	Cli_NetReg( PG_Houses_LtoC_BoxInfoResult			 );
	Cli_NetReg( PG_Houses_LtoC_ExchangeEqResult			 );
	Cli_NetReg( PG_Houses_LtoC_SetPasswordResult		 );
	Cli_NetReg( PG_Houses_LtoC_DestroyItemResult		 );
	Cli_NetReg( PG_Houses_LtoC_SetHouseNameResult		 );

	Cli_NetReg( PG_Houses_LtoC_PlantClearResult			 );
	Cli_NetReg( PG_Houses_LtoC_PlantItemResult			 );
	Cli_NetReg( PG_Houses_LtoC_PlantLockResult			 );
	Cli_NetReg( PG_Houses_LtoC_PlantGetProductResult	 );

	Cli_NetReg( PG_Houses_LtoC_ModifyFriendDBID			 );
	Cli_NetReg( PG_Houses_LtoC_FriendDBID				 );
	Cli_NetReg( PG_Houses_LtoC_FriendItemLog			 );
	Cli_NetReg( PG_Houses_LtoC_ClearItemLogResult		);
	
	Cli_NetReg( PG_Houses_LtoC_ServantHireList			);
	Cli_NetReg( PG_Houses_LtoC_ServantHireResult		);
	Cli_NetReg( PG_Houses_LtoC_ServantDelResult			);
	Cli_NetReg( PG_Houses_LtoC_ServantEvent				);
	Cli_NetReg( PG_Houses_LtoC_ServantRenameResult		);

	Cli_NetReg( PG_Houses_LtoC_FixServantValue			);
    return true;
}
//-------------------------------------------------------------------
bool NetCli_Houses::_Release()
{
    return true;
}
//-------------------------------------------------------------------
void NetCli_Houses::_PG_Houses_LtoC_FixServantValue( int Sockid , int Size , void* Data )
{
	PG_Houses_LtoC_FixServantValue* PG = (PG_Houses_LtoC_FixServantValue*)Data;
	This->RL_FixServantValue( PG->ServantID , PG->Type , PG->Value );
}
//////////////////////////////////////////////////////////////////////////
//招募列表回應
void NetCli_Houses::_PG_Houses_LtoC_ServantHireList( int Sockid , int Size , void* Data )
{
	PG_Houses_LtoC_ServantHireList* PG = (PG_Houses_LtoC_ServantHireList*)Data;
	This->RL_ServantHireList( PG->ID , PG->End , PG->Servant );
}
//////////////////////////////////////////////////////////////////////////
//招募女僕結果
void NetCli_Houses::_PG_Houses_LtoC_ServantHireResult( int Sockid , int Size , void* Data )
{
	PG_Houses_LtoC_ServantHireResult* PG = (PG_Houses_LtoC_ServantHireResult*)Data;
	This->RL_ServantHireResult( PG->Result , PG->ID , PG->Pos , PG->Servant );
}
//////////////////////////////////////////////////////////////////////////
//女僕刪除結果
void NetCli_Houses::_PG_Houses_LtoC_ServantDelResult( int Sockid , int Size , void* Data )
{
	PG_Houses_LtoC_ServantDelResult* PG = (PG_Houses_LtoC_ServantDelResult*)Data;
	This->RL_ServantDelResult( PG->ServantID , PG->Result );
}
//////////////////////////////////////////////////////////////////////////
//女僕互動事件通知
void NetCli_Houses::_PG_Houses_LtoC_ServantEvent( int Sockid , int Size , void* Data )
{
	PG_Houses_LtoC_ServantEvent* PG = (PG_Houses_LtoC_ServantEvent*)Data;
	This->RL_ServantEvent( PG->ServantID , PG->EventID );
}
//////////////////////////////////////////////////////////////////////////
//女僕命名結果
void NetCli_Houses::_PG_Houses_LtoC_ServantRenameResult( int Sockid , int Size , void* Data )
{
	PG_Houses_LtoC_ServantRenameResult* PG = (PG_Houses_LtoC_ServantRenameResult*)Data;
	This->RL_ServantRenameResult(  PG->Name.Begin() , PG->Result );
}
//////////////////////////////////////////////////////////////////////////
void NetCli_Houses::_PG_Houses_LtoC_ClearItemLogResult			( int Sockid , int Size , void* Data )
{
	PG_Houses_LtoC_ClearItemLogResult* PG = (PG_Houses_LtoC_ClearItemLogResult*)Data;
	This->R_ClearItemLogResult( PG->Result );
}
void NetCli_Houses::_PG_Houses_LtoC_FriendItemLog				( int Sockid , int Size , void* Data )
{
	PG_Houses_LtoC_FriendItemLog* PG = (PG_Houses_LtoC_FriendItemLog*)Data;
	This->R_FriendItemLog( PG->Log );
}
void NetCli_Houses::_PG_Houses_LtoC_ModifyFriendDBID			( int Sockid , int Size , void* Data )
{
	PG_Houses_LtoC_ModifyFriendDBID* PG = (PG_Houses_LtoC_ModifyFriendDBID*)Data;
	if( PG->IsDelete == true )
		This->R_DeleteFriendDBID( PG->FriendDBID );
	else
		This->R_AddFriendDBID( PG->FriendDBID );
}
void NetCli_Houses::_PG_Houses_LtoC_FriendDBID					( int Sockid , int Size , void* Data )
{
	PG_Houses_LtoC_FriendDBID* PG = (PG_Houses_LtoC_FriendDBID*)Data;
	This->R_FriendDBID( PG->HouseDBID , PG->FriendDBID );
}
void NetCli_Houses::_PG_Houses_LtoC_PlantClearResult			( int Sockid , int Size , void* Data )
{
	PG_Houses_LtoC_PlantClearResult* PG = (PG_Houses_LtoC_PlantClearResult*)Data;
	This->R_PlantClearResult( PG->HousePos , PG->Result );
}
void NetCli_Houses::_PG_Houses_LtoC_PlantItemResult				( int Sockid , int Size , void* Data )
{
	PG_Houses_LtoC_PlantItemResult* PG = (PG_Houses_LtoC_PlantItemResult*)Data;
	This->R_PlantItemResult( PG->Type , PG->PotHousePos , PG->ItemBodyPos , PG->Result );
}
void NetCli_Houses::_PG_Houses_LtoC_PlantLockResult				( int Sockid , int Size , void* Data )
{
	PG_Houses_LtoC_PlantLockResult* PG = (PG_Houses_LtoC_PlantLockResult*)Data;
	This->R_PlantLockResult( PG->PotHousePos , PG->Result );
}
void NetCli_Houses::_PG_Houses_LtoC_PlantGetProductResult		( int Sockid , int Size , void* Data )
{
	PG_Houses_LtoC_PlantGetProductResult* PG = (PG_Houses_LtoC_PlantGetProductResult*)Data;
	This->R_PlantGetProductResult( PG->PotHousePos , PG->Result );
}

void NetCli_Houses::_PG_Houses_LtoC_DestroyItemResult			( int Sockid , int Size , void* Data )
{
	PG_Houses_LtoC_DestroyItemResult* PG = (PG_Houses_LtoC_DestroyItemResult*)Data;
	This->R_DestroyItemResult( PG->Result );
}
void NetCli_Houses::_PG_Houses_LtoC_ExchangeEqResult			( int Sockid , int Size , void* Data )
{
	PG_Houses_LtoC_ExchangeEqResult* PG = (PG_Houses_LtoC_ExchangeEqResult*)Data;
	This->R_ExchangeEqResult( );
}
void NetCli_Houses::_PG_Houses_LtoC_SetPasswordResult			( int Sockid , int Size , void* Data )
{
	PG_Houses_LtoC_SetPasswordResult* PG = (PG_Houses_LtoC_SetPasswordResult*)Data;
	This->R_SetPasswordResult( PG->Result );
}
void NetCli_Houses::_PG_Houses_LtoC_SetHouseNameResult			( int Sockid , int Size , void* Data )
{
	PG_Houses_LtoC_SetHouseNameResult* PG = (PG_Houses_LtoC_SetHouseNameResult*)Data;
	This->R_SetHouseNameResult( PG->Result );
}
void NetCli_Houses::_PG_Houses_LtoC_BoxInfoResult				( int Sockid , int Size , void* Data )
{
	PG_Houses_LtoC_BoxInfoResult* PG = (PG_Houses_LtoC_BoxInfoResult*)Data;
	This->R_BoxInfoResult( PG->Result );
}
void NetCli_Houses::_PG_Houses_LtoC_BuildHouseResult			( int Sockid , int Size , void* Data )
{
	PG_Houses_LtoC_BuildHouseResult* PG = (PG_Houses_LtoC_BuildHouseResult*)Data;
	This->R_BuildHouseResult( PG->HouseDBID , PG->Result );
}
void NetCli_Houses::_PG_Houses_LtoC_OpenVisitHouse				( int Sockid , int Size , void* Data )
{
	PG_Houses_LtoC_OpenVisitHouse* PG = (PG_Houses_LtoC_OpenVisitHouse*)Data;
	This->R_OpenVisitHouse( PG->TargetNpcID , PG->IsVisitMyHouse );
}
void NetCli_Houses::_PG_Houses_LtoC_CloseVisitHouse				( int Sockid , int Size , void* Data )
{
	PG_Houses_LtoC_CloseVisitHouse* PG = (PG_Houses_LtoC_CloseVisitHouse*)Data;
	This->R_CloseVisitHouse( );
}
void NetCli_Houses::_PG_Houses_LtoC_FindHouseResult				( int Sockid , int Size , void* Data )
{
	PG_Houses_LtoC_FindHouseResult* PG = (PG_Houses_LtoC_FindHouseResult*)Data;
	This->R_FindHouseResult( PG->Result );
}

void NetCli_Houses::_PG_Houses_LtoC_HouseInfoLoading			( int Sockid , int Size , void* Data )
{
	PG_Houses_LtoC_HouseInfoLoading* PG = (PG_Houses_LtoC_HouseInfoLoading*)Data;
	This->R_HouseInfoLoading( );
}
void NetCli_Houses::_PG_Houses_LtoC_HouseBaseInfo				( int Sockid , int Size , void* Data )
{
	PG_Houses_LtoC_HouseBaseInfo* PG = (PG_Houses_LtoC_HouseBaseInfo*)Data;
	This->R_HouseBaseInfo( PG->HouseBaseInfo );
}
void NetCli_Houses::_PG_Houses_LtoC_ItemInfo					( int Sockid , int Size , void* Data )
{
	PG_Houses_LtoC_ItemInfo* PG = (PG_Houses_LtoC_ItemInfo*)Data;
	This->R_ItemInfo( PG->ItemDBID , PG->ItemCount , PG->Item );
}
void NetCli_Houses::_PG_Houses_LtoC_HouseInfoLoadOK				( int Sockid , int Size , void* Data )
{
	PG_Houses_LtoC_HouseInfoLoadOK* PG = (PG_Houses_LtoC_HouseInfoLoadOK*)Data;
	This->R_HouseInfoLoadOK( );
}
void NetCli_Houses::_PG_Houses_LtoC_SwapBodyHouseItemResult		( int Sockid , int Size , void* Data )
{
	PG_Houses_LtoC_SwapBodyHouseItemResult* PG = (PG_Houses_LtoC_SwapBodyHouseItemResult*)Data;
	This->R_SwapBodyHouseItemResult( PG->HouseParentItemDBID , PG->HousePos , PG->BodyPos , PG->Result );
}
void NetCli_Houses::_PG_Houses_LtoC_FixHouseItem				( int Sockid , int Size , void* Data )
{
	PG_Houses_LtoC_FixHouseItem* PG = (PG_Houses_LtoC_FixHouseItem*)Data;
	This->R_FixHouseItem( PG->HouseItem );
}

void NetCli_Houses::_PG_Houses_LtoC_SwapHouseItemResult			( int Sockid , int Size , void* Data )
{
	PG_Houses_LtoC_SwapHouseItemResult* PG = (PG_Houses_LtoC_SwapHouseItemResult*)Data;
	This->R_SwapHouseItemResult( PG->ParentItemDBID , PG->HousePos , PG->Result );
}
void NetCli_Houses::_PG_Houses_LtoC_HouseItemLayoutResult	( int Sockid , int Size , void* Data )
{
	PG_Houses_LtoC_HouseItemLayoutResult* PG = (PG_Houses_LtoC_HouseItemLayoutResult*)Data;
	This->R_HouseItemLayoutResult( PG->HouseItemDBID , PG->Result );
}
void NetCli_Houses::_PG_Houses_LtoC_ChangeHouseResult			( int Sockid , int Size , void* Data )
{
	PG_Houses_LtoC_ChangeHouseResult* PG = (PG_Houses_LtoC_ChangeHouseResult*)Data;
	This->R_ChangeHouseResult( PG->HouseType , PG->Result );
}
void NetCli_Houses::_PG_Houses_LtoC_BuyFunctionResult			( int Sockid , int Size , void* Data )
{
	PG_Houses_LtoC_BuyFunctionResult* PG = (PG_Houses_LtoC_BuyFunctionResult*)Data;
	This->R_BuyFunctionResult( PG->FunctionID , PG->Result );
}
void NetCli_Houses::_PG_Houses_LtoC_FunctionSettingResult		( int Sockid , int Size , void* Data )
{
	PG_Houses_LtoC_FunctionSettingResult* PG = (PG_Houses_LtoC_FunctionSettingResult*)Data;
	This->R_FunctionSettingResult( PG->FunctionID , PG->Mode , PG->Result );
}
void NetCli_Houses::_PG_Houses_LtoC_BuyEnergyResult				( int Sockid , int Size , void* Data )
{
	PG_Houses_LtoC_BuyEnergyResult* PG = (PG_Houses_LtoC_BuyEnergyResult*)Data;
	This->R_BuyEnergyResult( PG->Money_Account , PG->Energy );
}
//////////////////////////////////////////////////////////////////////////
void NetCli_Houses::S_VisitHouseRequest			( int HouseDBID , char* Password )
{
	PG_Houses_CtoL_VisitHouseRequest Send;
	Send.HouseDBID	= HouseDBID;
	Send.Password	= Password;

	SendToLocal_Base( sizeof( Send ) , &Send );
}
void NetCli_Houses::S_VisitHouseRequest_ByName			( char* RoleName , char* Password )
{
	PG_Houses_CtoL_VisitHouseRequest_ByName Send;
	Send.RoleName	= RoleName;
	Send.Password	= Password;

	SendToLocal_Base( sizeof( Send ) , &Send );
}
void NetCli_Houses::S_CloseVisitHouse			( )
{
	PG_Houses_CtoL_CloseVisitHouse Send;
	SendToLocal_Base( sizeof( Send ) , &Send );
}
void NetCli_Houses::S_SwapBodyHouseItem			( int BodyPos , int HousePos , int HouseParentItemDBID , bool IsFromBody )
{
	PG_Houses_CtoL_SwapBodyHouseItem Send;
	Send.BodyPos = BodyPos;
	Send.HousePos = HousePos;
	Send.HouseParentItemDBID = HouseParentItemDBID;
	Send.IsFromBody = IsFromBody;

	SendToLocal_Base( sizeof( Send ) , &Send );
}

void NetCli_Houses::S_SwapHouseItemRequest		( int HouseParentItemDBID1 , int HousePos1 , int HouseParentItemDBID2 , int HousePos2 )
{
	PG_Houses_CtoL_SwapHouseItemRequest Send;
	Send.HousePos[0] = HousePos1;
	Send.HousePos[1] = HousePos2;
	Send.HouseParentItemDBID[0] = HouseParentItemDBID1;
	Send.HouseParentItemDBID[1] = HouseParentItemDBID2;

	SendToLocal_Base( sizeof( Send ) , &Send );
}
void NetCli_Houses::S_HouseItemLayoutRequest	( int HouseItemDBID , House3DLayoutStruct& Layout )
{
	PG_Houses_CtoL_HouseItemLayoutRequest Send;
	Send.ItemDBID = HouseItemDBID;
	Send.Layout = Layout;
	SendToLocal_Base( sizeof( Send ) , &Send );
}
void NetCli_Houses::S_ChangeHouseRequest		( int HouseType )
{
	PG_Houses_CtoL_ChangeHouseRequest Send;
	Send.HouseType = HouseType;
	SendToLocal_Base( sizeof( Send ) , &Send );
}
void NetCli_Houses::S_BuyFunctionRequest		( int FunctionID )
{
	PG_Houses_CtoL_BuyFunctionRequest Send;
	Send.FunctionID = FunctionID;
	SendToLocal_Base( sizeof( Send ) , &Send );
}
void NetCli_Houses::S_FunctionSettingRequest	( int FunctionID , int Mode )
{
	PG_Houses_CtoL_FunctionSettingRequest Send;
	Send.FunctionID = FunctionID;
	Send.Mode		= Mode;
	SendToLocal_Base( sizeof( Send ) , &Send );
}
void NetCli_Houses::S_BuyEnergyRequest			( int Money_Account , const char* Password )
{
	PG_Houses_CtoL_BuyEnergyRequest Send;
	Send.Money_Account = Money_Account;
	Send.Password = Password;

	SendToLocal_Base( sizeof( Send ) , &Send );
}
void NetCli_Houses::S_LeaveHouse					( int PlayerDBID )
{
	PG_Houses_CtoL_LeaveHouse Send;
	Send.PlayerDBID = PlayerDBID;

	SendToLocal_Base( sizeof( Send ) , &Send );
}
void NetCli_Houses::S_BoxInfoRequset				( int BoxItemDBID )
{
	PG_Houses_CtoL_BoxInfoRequset Send;
	Send.BoxItemDBID = BoxItemDBID;

	SendToLocal_Base( sizeof( Send ) , &Send );
}
//與某一個寶箱換裝備
void NetCli_Houses::S_ExchangeEq					( int BoxItemDBID , int BoxPos , EQWearPosENUM EqPos )
{
	PG_Houses_CtoL_ExchangeEq Send;
	Send.BoxItemDBID = BoxItemDBID;
	Send.BoxPos = BoxPos;
	Send.EqPos = EqPos;
	SendToLocal( sizeof( Send ) , &Send );
}
//設定房屋密碼
void NetCli_Houses::S_SetPassword					( const char* Password )
{
	PG_Houses_CtoL_SetPassword Send;
	Send.Password = Password;
	SendToLocal( sizeof( Send ) , &Send );
}
//設定房屋名稱
void NetCli_Houses::S_SetHouseName					( const char* HouseName )
{
	PG_Houses_CtoL_SetHouseName Send;
	Send.HouseName = HouseName;
	SendToLocal( sizeof( Send ) , &Send );
}

//設定房屋名稱
void NetCli_Houses::S_SetClientData					( const char* ClientData )
{
	PG_Houses_CtoL_SetClientData Send;
	memcpy( Send.ClientData , ClientData , sizeof(Send.ClientData) );
	SendToLocal( sizeof( Send ) , &Send );
}

//刪除物品
void NetCli_Houses::S_DestroyItemRequest			( int ItemDBID )
{
	PG_Houses_CtoL_DestroyItemRequest Send;
	Send.ItemDBID = ItemDBID;
	SendToLocal( sizeof( Send ) , &Send );
}

void NetCli_Houses::S_PlantClearRequest			( int PotHousePos )	
{
	PG_Houses_CtoL_PlantClearRequest Send;
	Send.HousePos = PotHousePos;
	SendToLocal( sizeof( Send ) , &Send );
}
void NetCli_Houses::S_PlantItemRequest			( PlantItemTypeENUM	Type , int PotHousePos , int ItemBodyPos )
{
	PG_Houses_CtoL_PlantItemRequest Send;
	Send.Type			= Type;
	Send.PotHousePos	= PotHousePos;
	Send.ItemBodyPos	= ItemBodyPos;
	SendToLocal( sizeof( Send ) , &Send );
}
void NetCli_Houses::S_PlantLockRequest			( int PotHousePos )
{
	PG_Houses_CtoL_PlantLockRequest Send;
	Send.PotHousePos = PotHousePos;
	SendToLocal( sizeof( Send ) , &Send );
}
void NetCli_Houses::S_PlantGetProductRequest	( int PotHousePos )
{
	PG_Houses_CtoL_PlantGetProductRequest Send;
	Send.PotHousePos = PotHousePos;
	SendToLocal( sizeof( Send ) , &Send );
}
void NetCli_Houses::S_AddFriendDBID				( int FriendDBID )
{
	PG_Houses_CtoL_ModifyFriendDBID Send;
	Send.IsDelete = false;
	Send.FriendDBID = FriendDBID;
	SendToLocal( sizeof( Send ) , &Send );
}
void NetCli_Houses::S_DelFriendDBID				( int FriendDBID )
{
	PG_Houses_CtoL_ModifyFriendDBID Send;
	Send.IsDelete = true;
	Send.FriendDBID = FriendDBID;
	SendToLocal( sizeof( Send ) , &Send );
}
void NetCli_Houses::S_ClearItemLogRequest		( )
{
	PG_Houses_CtoL_ClearItemLogRequest Send;
	SendToLocal( sizeof( Send ) , &Send );
}
//要求招募列表
void NetCli_Houses::SL_ServantHireListRequest(  )
{
	PG_Houses_CtoL_ServantHireListRequest Send;
	SendToLocal( sizeof(Send) , &Send );
}
//////////////////////////////////////////////////////////////////////////
//招募女僕
void NetCli_Houses::SL_ServantHireRequest( int ID , int Pos )
{
	PG_Houses_CtoL_ServantHireRequest Send;
	Send.ID = ID;
	Send.Pos = Pos;
	SendToLocal( sizeof(Send) , &Send );
}
//////////////////////////////////////////////////////////////////////////
//要求刪除女僕
void NetCli_Houses::SL_ServantDelRequest( int ServantID )
{
	PG_Houses_CtoL_ServantDelRequest Send;
	Send.ServantID = ServantID;
	SendToLocal( sizeof(Send) , &Send );
}
//////////////////////////////////////////////////////////////////////////
//女僕互動事件
void NetCli_Houses::SL_ServantEventRequest( int ServantID , int EventType1 , int EventType2 )
{
	PG_Houses_CtoL_ServantEventRequest Send;
	Send.ServantID = ServantID;
	Send.EventType1 = EventType1;
	Send.EventType2 = EventType2;
	SendToLocal( sizeof(Send) , &Send );
}
//////////////////////////////////////////////////////////////////////////
//女僕擺設
void NetCli_Houses::SL_ServantLayout( int ServantID , House3DLayoutStruct Layout )
{
	PG_Houses_CtoL_ServantLayout Send;
	Send.ServantID = ServantID;
	Send.Layout = Layout;
	SendToLocal( sizeof(Send) , &Send );
}
//////////////////////////////////////////////////////////////////////////
//女僕命名
void NetCli_Houses::SL_ServantRenameRequest( const char* Name )
{
	PG_Houses_CtoL_ServantRenameRequest Send;
	Send.Name = Name;
	SendToLocal( sizeof(Send) , &Send );
}
//////////////////////////////////////////////////////////////////////////
