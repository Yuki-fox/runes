#pragma once

#include "PG/PG_RoleValue.h"
#include "../../MainProc/NetGlobal.h"

class NetCli_RoleValue : public CNetGlobal
{
protected:
    //-------------------------------------------------------------------
    static NetCli_RoleValue* This;
	static bool	IsSetPoint;
    static bool _Init();
    static bool _Release();
    //-------------------------------------------------------------------

    
    static void _PG_RoleValue_LtoC_FixAllRoleValue   	( int Sockid , int Size , void* Data );     
    static void _PG_RoleValue_LtoC_FixRoleValue      	( int Sockid , int Size , void* Data );        
    static void _PG_RoleValue_LtoC_ResultSetPoint    	( int Sockid , int Size , void* Data );     
    static void _PG_RoleValue_LtoC_NotifyLvUp        	( int Sockid , int Size , void* Data );     
    static void _PG_RoleValue_LtoC_SetTitleFlag      	( int Sockid , int Size , void* Data );     
    static void _PG_RoleValue_LtoC_SetFlag           	( int Sockid , int Size , void* Data );        
    static void _PG_RoleValue_LtoC_SetTitleResult    	( int Sockid , int Size , void* Data );     
//    static void _PG_RoleValue_LtoC_Title             	( int Sockid , int Size , void* Data );     
    static void _PG_RoleValue_LtoC_RoleInfoChange    	( int Sockid , int Size , void* Data );

	static void _PG_RoleValue_LtoC_OpenChangeJob	 	( int Sockid , int Size , void* Data );
	static void _PG_RoleValue_LtoC_CloseChangeJob	 	( int Sockid , int Size , void* Data );
	static void _PG_RoleValue_LtoC_ChangeJobResult	 	( int Sockid , int Size , void* Data );
	static void _PG_RoleValue_LtoC_SetPlayerAllZoneState( int Sockid , int Size , void* Data );
	static void _PG_RoleValue_LtoC_DeltaRoleValue      	( int Sockid , int Size , void* Data );        

	static void _PG_RoleValue_LtoC_ClientLogoutResult	( int Sockid , int Size , void* Data );
	static void _PG_RoleValue_LtoC_CancelClientLogout	( int Sockid , int Size , void* Data );

	static void _PG_RoleValue_LtoC_WaitChangeParallel	( int Sockid , int Size , void* Data );
	static void _PG_RoleValue_LtoC_ClientComputerInfoRequest( int Sockid , int Size , void* Data );
	static void _PG_RoleValue_LtoC_ObjMode				( int Sockid , int Size , void* Data );	
	static void _PG_RoleValue_LtoC_ObjRoleInfo			( int Sockid , int Size , void* Data );	
	static void _PG_RoleValue_LtoC_GetTargetRoleValue	( int Sockid , int Size , void* Data );	
public:                                              
    NetCli_RoleValue() { This = this; }


    virtual void R_FixAllRoleValue   ( BaseAbilityStruct& Ability ) = 0;
    virtual void R_FixRoleValue      ( RoleValueName_ENUM ValueName , double Value ) = 0;
	virtual void R_DeltaRoleValue     ( RoleValueName_ENUM ValueName , double Value ) = 0;
    virtual void R_SetPoint_OK       ( ) = 0;
    virtual void R_SetPoint_Failed   ( ) = 0;
    virtual void R_NotifyLvUp        ( int ItemID , int Level ) = 0;
    virtual void R_SetTitleFlag      ( int TitleID , bool Flag ) = 0;
    virtual void R_SetFlag           ( int FlagID , bool Flag ) = 0;
    virtual void R_SetTitle_OK       ( int TitleID ) = 0;
    virtual void R_SetTitle_Failed		 ( ) = 0;
//    virtual void R_Title             ( int ItemID , int TitleID ) = 0;
	virtual void R_RoleInfoChange		( int ItemID  , int Level , int Level_Sub , Voc_ENUM Voc , Voc_ENUM Voc_Sub ) = 0;
	virtual void R_GetTargetRoleValue	( int ItemID, RoleValueName_ENUM ValueName , double Value ) = 0;

	virtual void R_OpenChangeJob	 ( int TargetID ) = 0;
	virtual void R_ChangeJobResult	 ( ChangeJobResult_ENUM Result ) = 0;
	virtual void R_CloseChangeJob	 ( ) = 0;
	virtual void R_ClientLogoutResult( int Second ) = 0;
	virtual void R_CancelClientLogout( ) = 0;
	virtual void R_WaitChangeParallel( int Second , bool Result ) = 0;
	virtual void R_ClientComputerInfoRequest() = 0;
	virtual void R_ObjMode			( int ItemGUID , ObjectModeStruct& Mode ) = 0;

	virtual void R_SetPlayerAllZoneState( int GItemID , PlayerStateStruct& State ) = 0;
    static void S_RequestFixRoleValue   ( );
    static void S_RequestSetPoint       ( int STR , int STA , int INT , int MND , int AGI );
    static void S_SetTitleRequest        ( int TitleID , bool IsShowTitle );

	static void S_CloseChangeJobRequest	( );
	static void S_ChangeJobRequest		( int Job , int Job_Sub );
	
	static void S_Logout				();
	static void S_CancelLogout			();
	static void S_SetPlayerAllZoneState	( PlayerStateStruct& State );
	static void S_ClientFlag			( StaticFlag<_MAX_CLIENT_FLAG_COUNT_>& ClientFlag  );
	static void S_ClientData			( unsigned char ClientData[_MAX_CLIENT_DATA_SIZE_] );

	static void S_ChangeParallel		( int ParallelID );
	static void S_CancelChangeParallel	( );

	static void S_ClientComputerInfo	( ClientComputerInfoStruct& Info );
	static void S_ClientEnvironmentState( ClientStateStruct& State );

	virtual void R_ObjRoleInfo			( PlayerRoleData& Role ) = 0;
};

