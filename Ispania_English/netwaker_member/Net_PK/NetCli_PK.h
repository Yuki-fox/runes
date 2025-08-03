#pragma once

#include "PG/PG_PK.h"
#include "../../MainProc/NetGlobal.h"

class NetCli_PK : public CNetGlobal
{
protected:
    //-------------------------------------------------------------------
    static NetCli_PK* This;
    static bool _Init();
    static bool _Release();
	static void _PG_PK_LtoC_PKInviteNotify		( int Sockid , int Size , void* Data );
	static void _PG_PK_LtoC_PKInviteResult		( int Sockid , int Size , void* Data );
	static void _PG_PK_LtoC_PKPrepare			( int Sockid , int Size , void* Data );
	static void _PG_PK_LtoC_PKBegin				( int Sockid , int Size , void* Data );
	static void _PG_PK_LtoC_PKEnd				( int Sockid , int Size , void* Data );
	static void _PG_PK_LtoC_PKCancel			( int Sockid , int Size , void* Data );
public:    
    NetCli_PK() { This = this; }

	static void S_PKInviteRequest	( int TargetID );
	static void S_PKInviteResult	( int TargetID , bool Result );
	static void S_PKCancel			( );

	virtual void R_PKInviteNotify	( int TargetID , int PKScore ) = 0;
	virtual void R_PKInviteResult	( int TargetID , bool Result ) = 0;
	virtual void R_PKPrepare		( int Player1ID , int Player2ID , int Player1Score , int Player2Score 
									, int X , int Y , int Z , int Range , int Time) = 0;
	virtual void R_PKBegin			( int Time ) = 0;
	virtual void R_PKEnd			( int WinnerID , int LoserID , bool IsEven ) = 0;
	virtual void R_PKCancel			( ) = 0;
};

