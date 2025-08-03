#ifndef __Net_Instance_H__
#define __Net_Instance_H__
#include "PG/PG_Instance.h"

#include "../../MainProc/NetGlobal.h"

#pragma unmanaged

class Net_Instance : public CNetGlobal
{
protected:
    //-------------------------------------------------------------------
    static Net_Instance* This;
    static bool _Init();
    static bool _Release();

	static void _PG_Instance_LtoC_ResetTime				( int Sockid , int Size , void* Data );
	static void _PG_Instance_LtoC_JoinInstanceRequest	( int Sockid , int Size , void* Data );
	static void _PG_Instance_LtoC_InstanceReset			( int Sockid , int Size , void* Data );
	static void _PG_Instance_LtoC_InstanceCreate		( int Sockid , int Size , void* Data );

	static void _PG_Instance_LtoC_FixInstanceExtend		( int Sockid , int Size , void* Data );
	static void _PG_Instance_LtoC_InstanceRestTime		( int Sockid , int Size , void* Data );

	static void _PG_Instance_LtoC_GetWorldVarResult		( int Sockid , int Size , void* Data );

	
public:
    Net_Instance() { This = this; }

	virtual void R_ResetTime( int Time , int InstanceSaveID , int ValueKeyID , bool IsLogin ) = 0;
	virtual void R_JoinInstanceRequest( ) = 0;
	virtual void R_InstanceReset( int InstanceKey ) = 0;
	virtual void R_InstanceCreate( int PlayerDBID , int ZoneID , int Key ) {};
	static void SL_JoinInstanceResult( bool Result );


	//副本儲存時間展期
	static void SL_InstanceExtend( int ZoneID , int ExtendTime , int SavePos );
		virtual void R_FixInstanceExtend( int ZoneID , int ExtendTime , int LiveTime , int KeyID ) = 0;

	//查詢副本重置時間
	static void SL_InstanceRestTimeRequest( int ZoneID , int SavePos );
		virtual void R_InstanceRestTime( int ZoneID , int ExtendTime , int LiveTime , int KeyID ) = 0;

	// WorldVar
	virtual void	R_GetWorldVarResult		( const char* pszVarName, int iVarValue ) = 0;
	static	void	SL_GetWorldVar			( const char* pszVarName );
};

#pragma managed
#endif //__Net_Instance_H__