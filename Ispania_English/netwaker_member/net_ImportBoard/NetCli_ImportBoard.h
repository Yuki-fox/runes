#pragma  once
#include "PG/PG_ImportBoard.h"
#include "../../MainProc/NetGlobal.h"

class NetCli_ImportBoard : public CNetGlobal
{
protected:
    //-------------------------------------------------------------------
    static NetCli_ImportBoard* This;
    static bool _Init();
    static bool _Release();
    //-------------------------------------------------------------------
	static void _PG_ImportBoard_DtoC_ImportBoardInfo		( int Sockid , int Size , void* Data );
	static void _PG_ImportBoard_DtoC_ImportBoardNewVersion	( int Sockid , int Size , void* Data );
public:    
    NetCli_ImportBoard() { This = this; }

	static void SL_ImportBoardRequest( float UpdateTime );

	virtual void RD_ImportBoardInfo( float UpdateTime , int DataCount , ImportBoardBaseStruct Data[] ) = 0;
	virtual void RD_ImportBoardNewVersion( float UpdateTime ) = 0;
};

