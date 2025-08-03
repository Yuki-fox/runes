#pragma once

#include "PG/PG_Teleport.h"
#include "../../MainProc/NetGlobal.h"

class NetCli_Teleport : public CNetGlobal
{
protected:
    //-------------------------------------------------------------------
    static	NetCli_Teleport* This;
    static	bool _Init();
    static	bool _Release();	
    //-------------------------------------------------------------------
	static void _PG_Teleport_LtoC_AddTeleportResult			( int Sockid , int Size , void* Data );
	static void _PG_Teleport_LtoC_DelTeleportResult			( int Sockid , int Size , void* Data );
	static void _PG_Teleport_LtoC_SwapTeleportResult		( int Sockid , int Size , void* Data );	
	static void _PG_Teleport_LtoC_ModifyTeleportResult		( int Sockid , int Size , void* Data );
	static void _PG_Teleport_LtoC_UseTeleportResult			( int Sockid , int Size , void* Data );
															
public:    
    NetCli_Teleport() { This = this; }
	static	NetCli_Teleport*	GetThis()						{ return This; }

	static void SL_AddTeleportRequest			( int ItemPos , int TeleportPos , LocationSaveStruct& Info );
	static void SL_DelTeleportRequest			( int TeleportPos );
	static void SL_SwapTeleportRequest			( int TeleportPos[2] );
	static void SL_ModifyTeleportRequest		( int TeleportPos , LocationSaveStruct& Info );
	static void SL_UseTeleportRequest			( UseTeleportTypeENUM Type , int ItemPos , int TeleportPos );


	virtual void R_AddTeleportResult	( int TeleportPos , LocationSaveStruct& Info , bool Result ) = 0;
	virtual void R_DelTeleportResult	( int TeleportPos , bool Result ) = 0;
	virtual void R_SwapTeleportResult	( int TeleportPos[2] , bool Result ) = 0;
	virtual void R_ModifyTeleportResult	( int TeleportPos , LocationSaveStruct& Info , bool Result ) = 0;
	virtual void R_UseTeleportResult	( UseTeleportTypeENUM Type , int ItemPos , int TeleportPos , bool Result ) = 0;
};

