#pragma once

#include "Net_Instance.h"

class NetCli_InstanceChild : public Net_Instance
{
public:
	static bool _Init();
	static bool _Release();

	virtual void R_ResetTime( int Time , int InstanceSaveID , int ValueKeyID , bool IsLogin );
	virtual void R_JoinInstanceRequest( );
	virtual void R_InstanceReset( int InstanceKey );
	virtual void R_InstanceCreate( int PlayerDBID , int ZoneID , int Key );

	virtual void R_FixInstanceExtend( int ZoneID , int ExtendTime , int LiveTime , int KeyID );
	virtual void R_InstanceRestTime( int ZoneID , int ExtendTime , int LiveTime , int KeyID );

	virtual void R_GetWorldVarResult		( const char* pszVarName, int iVarValue );
};