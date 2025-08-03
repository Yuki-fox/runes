#pragma once

#include "NetCli_Houses.h"

class NetCli_HousesChild : public NetCli_Houses
{
public:	
	static	bool _Init();
	static	bool _Release();

public:

	virtual void R_BuildHouseResult				( int HouseDBID , bool Result );
	virtual void R_OpenVisitHouse				( int TargetNpcID , bool IsVisitMyHouse );
	virtual void R_CloseVisitHouse				();
	virtual void R_FindHouseResult				( FindHouseResultENUM Result );
	virtual void R_HouseInfoLoading				();
	virtual void R_HouseBaseInfo				( HouseBaseDBStruct& HouseBaseInfo );
	virtual void R_ItemInfo						(  int ItemDBID, int ItemCount , HouseItemDBStruct Item[_DEF_MAX_HOUSE_FURNITURE] );
	virtual void R_HouseInfoLoadOK				();
	virtual void R_SwapBodyHouseItemResult		( int HouseParentItemDBID , int HousePos , int BodyPos , bool Result );
	virtual void R_FixHouseItem					( HouseItemDBStruct& HouseItem );
	virtual void R_SwapHouseItemResult			( int ParentItemDBID[2] , int HousePos[2] , bool Result );
	virtual void R_HouseItemLayoutResult		( int HouseItemDBID , bool Result );
	virtual void R_ChangeHouseResult			( int HouseType , bool Result );
	virtual void R_BuyFunctionResult			( int FunctionID , bool Result );
	virtual void R_FunctionSettingResult		( int FunctionID , int Mode , bool Result );
	virtual void R_BuyEnergyResult				( int Money_Account , int Energy );
	virtual void R_BoxInfoResult				( int Result );
	virtual void R_ExchangeEqResult				( );
	virtual void R_SetPasswordResult			( bool Result );
	virtual void R_DestroyItemResult			( bool Result );
	virtual void R_SetHouseNameResult			( SetHouseNameResultENUM Result );
	virtual void R_FriendDBID					( int HouseDBID , int FriendDBID );
	virtual void R_DeleteFriendDBID				( int FriendDBID );
	virtual void R_AddFriendDBID				( int FriendDBID );
	virtual void R_ClearItemLogResult			( bool Result );

	//////////////////////////////////////////////////////////////////////////
	//種植
	//////////////////////////////////////////////////////////////////////////
	virtual void R_PlantClearResult				( int HousePos , PlantClearResultTypeENUM Result );
	virtual void R_PlantItemResult				( PlantItemTypeENUM Type , int PotHousePos , int ItemBodyPos , PlantItemResultTypeENUM Result  );
	virtual void R_PlantLockResult				( int PotHousePos , PlantLockResultTypeENUM Result );
	virtual void R_PlantGetProductResult		( int PotHousePos , PlantGetProductResultTypeENUM Result );
	virtual void R_FriendItemLog				( HouseFriendItemLogStruct& Log );

	//////////////////////////////////////////////////////////////////////////
	//招募列表回應
	virtual	void RL_ServantHireList( int ID , bool End , HouseServantStruct& Servant );
	//招募女僕結果
	virtual	void RL_ServantHireResult( bool Result , int ID , int Pos , HouseServantStruct& Servant );
	//女僕刪除結果
	virtual	void RL_ServantDelResult( int ServantID , bool Result );
	//女僕互動事件通知
	virtual	void RL_ServantEvent( int ServantID , int EventID );
	//女僕命名結果
	virtual	void RL_ServantRenameResult( const char* Name , bool Result );
	//修正女僕數值
	virtual void RL_FixServantValue( int ServantID , HouseServantValueTypeENUM Type , int Value );
};

