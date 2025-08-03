#pragma once

#include "NetCli_RoleValue.h"

class NetCli_RoleValueChild : public NetCli_RoleValue
{
public:
	static bool _Init();
	static bool _Release();

	virtual void R_FixAllRoleValue		( BaseAbilityStruct& Ability );
	virtual void R_FixRoleValue			( RoleValueName_ENUM ValueName , double Value );
	virtual void R_DeltaRoleValue		( RoleValueName_ENUM ValueName , double Value );
	virtual void R_SetPoint_OK			( );
	virtual void R_SetPoint_Failed		( );
	virtual void R_NotifyLvUp			( int ItemID , int Level );
	virtual void R_SetTitleFlag			( int TitleID , bool Flag );
	virtual void R_SetFlag				( int FlagID , bool Flag );
	virtual void R_SetTitle_OK			( int TitleID );
	virtual void R_SetTitle_Failed		( );


	virtual void R_RoleInfoChange		( int ItemID  , int Level , int Level_Sub , Voc_ENUM Voc , Voc_ENUM Voc_Sub );

	virtual void R_OpenChangeJob	 	( int TargetID );
	virtual void R_CloseChangeJob	 	( );
	virtual void R_ChangeJobResult	 	( ChangeJobResult_ENUM Result );
	virtual void R_SetPlayerAllZoneState( int GItemID , PlayerStateStruct& State );
	virtual void R_ClientLogoutResult	( int Second );
	virtual void R_CancelClientLogout	( );
	virtual void R_WaitChangeParallel	( int Second , bool Result );

	virtual void R_ClientComputerInfoRequest();
	virtual void R_ObjMode				( int ItemGUID , ObjectModeStruct& Mode );
	virtual void R_ObjRoleInfo			( PlayerRoleData& Role );
	virtual void R_GetTargetRoleValue	( int ItemID, RoleValueName_ENUM ValueName , double Value );
};