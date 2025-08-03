#pragma once


#include "NetCli_PK.h"

using namespace std;



class NetCli_PKChild : public NetCli_PK
{
public:	
	static bool _Init();
	static bool _Release();

public:
	virtual void R_PKInviteNotify	( int TargetID , int PKScore );
	virtual void R_PKInviteResult	( int TargetID , bool Result );
	virtual void R_PKPrepare		( int Player1ID , int Player2ID , int Player1Score , int Player2Score 
									, int X , int Y , int Z , int Range , int Time);
	virtual void R_PKBegin			( int Time );
	virtual void R_PKEnd			( int WinnerID , int LoserID , bool IsEven );
	virtual void R_PKCancel			( );
};

