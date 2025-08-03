#pragma once

#include "PG/PG_Houses.h"
#include "../../MainProc/NetGlobal.h"

class NetCli_Houses : public CNetGlobal
{
protected:
    //-------------------------------------------------------------------
    static	NetCli_Houses* This;
    static	bool _Init();
    static	bool _Release();	
    //-------------------------------------------------------------------

	static void _PG_Houses_LtoC_BuildHouseResult			( int Sockid , int Size , void* Data );
	static void _PG_Houses_LtoC_OpenVisitHouse				( int Sockid , int Size , void* Data );
	static void _PG_Houses_LtoC_CloseVisitHouse				( int Sockid , int Size , void* Data );
	static void _PG_Houses_LtoC_FindHouseResult				( int Sockid , int Size , void* Data );
	static void _PG_Houses_LtoC_HouseInfoLoading			( int Sockid , int Size , void* Data );
	static void _PG_Houses_LtoC_HouseBaseInfo				( int Sockid , int Size , void* Data );
	static void _PG_Houses_LtoC_ItemInfo					( int Sockid , int Size , void* Data );
	static void _PG_Houses_LtoC_HouseInfoLoadOK				( int Sockid , int Size , void* Data );
	static void _PG_Houses_LtoC_SwapBodyHouseItemResult		( int Sockid , int Size , void* Data );
	static void _PG_Houses_LtoC_FixHouseItem				( int Sockid , int Size , void* Data );
	static void _PG_Houses_LtoC_SwapHouseItemResult			( int Sockid , int Size , void* Data );
	static void _PG_Houses_LtoC_HouseItemLayoutResult		( int Sockid , int Size , void* Data );
	static void _PG_Houses_LtoC_ChangeHouseResult			( int Sockid , int Size , void* Data );
	static void _PG_Houses_LtoC_BuyFunctionResult			( int Sockid , int Size , void* Data );
	static void _PG_Houses_LtoC_FunctionSettingResult		( int Sockid , int Size , void* Data );
	static void _PG_Houses_LtoC_BuyEnergyResult				( int Sockid , int Size , void* Data );
	static void _PG_Houses_LtoC_BoxInfoResult				( int Sockid , int Size , void* Data );
	static void _PG_Houses_LtoC_ExchangeEqResult			( int Sockid , int Size , void* Data );
	static void _PG_Houses_LtoC_SetPasswordResult			( int Sockid , int Size , void* Data );
	static void _PG_Houses_LtoC_DestroyItemResult			( int Sockid , int Size , void* Data );
	static void _PG_Houses_LtoC_SetHouseNameResult			( int Sockid , int Size , void* Data );

	static void _PG_Houses_LtoC_PlantClearResult			( int Sockid , int Size , void* Data );
	static void _PG_Houses_LtoC_PlantItemResult				( int Sockid , int Size , void* Data );
	static void _PG_Houses_LtoC_PlantLockResult				( int Sockid , int Size , void* Data );
	static void _PG_Houses_LtoC_PlantGetProductResult		( int Sockid , int Size , void* Data );
	static void _PG_Houses_LtoC_FriendDBID					( int Sockid , int Size , void* Data );
	static void _PG_Houses_LtoC_ModifyFriendDBID			( int Sockid , int Size , void* Data );
	static void _PG_Houses_LtoC_FriendItemLog				( int Sockid , int Size , void* Data );
	static void _PG_Houses_LtoC_ClearItemLogResult			( int Sockid , int Size , void* Data );

	static void _PG_Houses_LtoC_ServantHireList				( int Sockid , int Size , void* Data );
	static void _PG_Houses_LtoC_ServantHireResult			( int Sockid , int Size , void* Data );
	static void _PG_Houses_LtoC_ServantDelResult			( int Sockid , int Size , void* Data );
	static void _PG_Houses_LtoC_ServantEvent				( int Sockid , int Size , void* Data );
	static void _PG_Houses_LtoC_ServantRenameResult			( int Sockid , int Size , void* Data );

	static void _PG_Houses_LtoC_FixServantValue				( int Sockid , int Size , void* Data );
public:	
    NetCli_Houses() { This = this; }
	static	NetCli_Houses*	GetThis()						{ return This; }

	static void S_VisitHouseRequest			( int HouseDBID , char* Password );
	static void S_VisitHouseRequest_ByName	( char* RoleName , char* Password );
	static void S_CloseVisitHouse			( );
	static void S_SwapBodyHouseItem			( int BodyPos , int HousePos , int HouseParentItemDBID , bool IsFromBody );	
	static void S_SwapHouseItemRequest		( int HouseParentItemDBID1 , int HousePos1 , int HouseParentItemDBID2 , int HousePos2 );
	static void S_HouseItemLayoutRequest	( int HouseItemDBID , House3DLayoutStruct& Layout );
	static void S_ChangeHouseRequest		( int HouseType );
	//���άY���
	static void S_BuyFunctionRequest		( int FunctionID );
	//Mode = 0 ��ܸѬ�
	static void S_FunctionSettingRequest	( int FunctionID , int Mode );
	static void S_BuyEnergyRequest			( int Money_Account , const char* Password );
	static void S_LeaveHouse				( int PlayerDBID );
	static void S_BoxInfoRequset			( int BoxItemDBID );

	//���˳�
	static void S_ExchangeEq				( int BoxItemDBID , int BoxPos , EQWearPosENUM EqPos );
	//�]�w�ЫαK�X
	static void S_SetPassword				( const char* Password );

	//�]�w�ЫΦW��
	static void S_SetHouseName				( const char* HouseName );
	//�]�wClient �ݪ���θ�T
	static void S_SetClientData				( const char* ClientData );

	//�R�����~
	static void S_DestroyItemRequest		( int ItemDBID );

	virtual void R_BuildHouseResult			( int HouseDBID , bool Result ) = 0;
	virtual void R_OpenVisitHouse			( int TargetNpcID , bool IsVisitMyHouse ) = 0;
	virtual void R_CloseVisitHouse			() = 0;
	virtual void R_FindHouseResult			( FindHouseResultENUM Result ) = 0;
	virtual void R_HouseInfoLoading			( ) = 0;
	virtual void R_HouseBaseInfo			( HouseBaseDBStruct& HouseBaseInfo ) = 0;
	virtual void R_ItemInfo					( int ItemDBID, int ItemCount , HouseItemDBStruct Item[_DEF_MAX_HOUSE_FURNITURE] ) = 0;
	virtual void R_HouseInfoLoadOK			() = 0;
	virtual void R_SwapBodyHouseItemResult	( int HouseParentItemDBID , int HousePos , int BodyPos , bool Result ) = 0;
	virtual void R_SwapHouseItemResult		( int ParentItemDBID[2] , int HousePos[2] , bool Result ) = 0;
	virtual void R_FixHouseItem				( HouseItemDBStruct& HouseItem ) = 0;
	virtual void R_HouseItemLayoutResult	( int HouseItemDBID , bool Result ) = 0;
	virtual void R_ChangeHouseResult		( int HouseType , bool Result ) = 0;
	virtual void R_BuyFunctionResult		( int FunctionID , bool Result ) = 0;
	virtual void R_FunctionSettingResult	( int FunctionID , int Mode , bool Result ) = 0;
	virtual void R_BuyEnergyResult			( int Money_Account , int Energy ) = 0;
	virtual void R_BoxInfoResult			( int Result ) = 0;
	virtual void R_ExchangeEqResult			( ) = 0;
	virtual void R_SetPasswordResult		( bool Result ) = 0;
	virtual void R_DestroyItemResult		( bool Result ) = 0;
	virtual void R_SetHouseNameResult		( SetHouseNameResultENUM Result ) = 0;

	//////////////////////////////////////////////////////////////////////////
	//�ش�
	//////////////////////////////////////////////////////////////////////////
	virtual void R_PlantClearResult			( int HousePos , PlantClearResultTypeENUM Result ) = 0;
	virtual void R_PlantItemResult			( PlantItemTypeENUM Type , int PotHousePos , int ItemBodyPos , PlantItemResultTypeENUM Result  ) = 0;
	virtual void R_PlantLockResult			( int PotHousePos , PlantLockResultTypeENUM Result ) = 0;
	virtual void R_PlantGetProductResult	( int PotHousePos , PlantGetProductResultTypeENUM Result ) = 0;

	static void S_PlantClearRequest			( int PotHousePos );	
	static void S_PlantItemRequest			( PlantItemTypeENUM	Type , int PotHousePos , int ItemBodyPos );
	static void S_PlantLockRequest			( int PotHousePos );
	static void S_PlantGetProductRequest	( int PotHousePos );

	//////////////////////////////////////////////////////////////////////////
	//�n�͸��
	//////////////////////////////////////////////////////////////////////////
	static void S_AddFriendDBID				( int FriendDBID );
	static void S_DelFriendDBID				( int FriendDBID );
	static void S_ClearItemLogRequest		( );

	virtual void R_FriendDBID				( int HouseDBID , int FriendDBID ) = 0;
	virtual void R_DeleteFriendDBID			( int FriendDBID ) = 0;
	virtual void R_AddFriendDBID			( int FriendDBID ) = 0;
	virtual void R_FriendItemLog			( HouseFriendItemLogStruct& Log ) = 0;
	virtual void R_ClearItemLogResult		( bool Result ) = 0;
	//////////////////////////////////////////////////////////////////////////
	//�n�D�۶ҦC��
	static void	SL_ServantHireListRequest(  );
		//�۶ҦC��^��
		virtual	void RL_ServantHireList( int ID , bool End , HouseServantStruct& Servant ) = 0;
	//�۶Ҥk��
	static void	SL_ServantHireRequest( int ID , int Pos );
		//�۶Ҥk�����G
		virtual	void RL_ServantHireResult( bool Result , int ID , int Pos , HouseServantStruct& Servant ) = 0;
	//�n�D�R���k��
	static void	SL_ServantDelRequest( int ServantID );
		//�k���R�����G
		virtual	void RL_ServantDelResult( int ServantID , bool Result ) = 0;
	//�k�����ʨƥ�
	static void	SL_ServantEventRequest( int ServantID , int EventType1 , int EventType2 );
		//�k�����ʨƥ�q��
		virtual	void RL_ServantEvent( int ServantID , int EventID ) = 0;
	//�k���\�]
	static void	SL_ServantLayout( int ServantID , House3DLayoutStruct Layout );
	//�k���R�W
	static void	SL_ServantRenameRequest( const char* Name );
		//�k���R�W���G
		virtual	void RL_ServantRenameResult( const char* Name , bool Result ) = 0;

	//�ץ��k���ƭ�
	virtual void RL_FixServantValue( int ServantID , HouseServantValueTypeENUM Type , int Value ) = 0;
};

