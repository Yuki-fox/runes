#pragma once

#include "NetCli_Teleport.h"

class NetCli_TeleportChild : public NetCli_Teleport
{
public:	
	static	bool _Init();
	static	bool _Release();

public:
	virtual void R_AddTeleportResult	( int TeleportPos , LocationSaveStruct& Info , bool Result );
	virtual void R_DelTeleportResult	( int TeleportPos , bool Result );
	virtual void R_SwapTeleportResult	( int TeleportPos[2] , bool Result );
	virtual void R_ModifyTeleportResult	( int TeleportPos , LocationSaveStruct& Info , bool Result );
	virtual void R_UseTeleportResult	( UseTeleportTypeENUM Type , int ItemPos , int TeleportPos , bool Result );
};

