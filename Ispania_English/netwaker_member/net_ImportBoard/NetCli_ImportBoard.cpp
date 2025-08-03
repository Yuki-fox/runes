#include "NetCli_ImportBoard.h"
#include "../../MainProc/NetGlobal.h"
//-------------------------------------------------------------------
NetCli_ImportBoard*    NetCli_ImportBoard::This         = NULL;

//-------------------------------------------------------------------
bool NetCli_ImportBoard::_Init()
{
	/*
	_Net.RegOnSrvPacketFunction	( EM_PG_ImportBoard_DtoC_ImportBoardInfo			, _PG_ImportBoard_DtoC_ImportBoardInfo			);
	_Net.RegOnSrvPacketFunction	( EM_PG_ImportBoard_DtoC_ImportBoardNewVersion		, _PG_ImportBoard_DtoC_ImportBoardNewVersion			);
*/
	Cli_NetReg( PG_ImportBoard_DtoC_ImportBoardInfo			);
	Cli_NetReg( PG_ImportBoard_DtoC_ImportBoardNewVersion	);

    return true;
}
//-------------------------------------------------------------------
bool NetCli_ImportBoard::_Release()
{
    return true;
}
//-------------------------------------------------------------------
void NetCli_ImportBoard::_PG_ImportBoard_DtoC_ImportBoardInfo		( int Sockid , int Size , void* Data )
{
	PG_ImportBoard_DtoC_ImportBoardInfo* pg = (PG_ImportBoard_DtoC_ImportBoardInfo*)Data;

	if( pg->DataSize == 0 || pg->DataCount == 0)
	{
		This->RD_ImportBoardInfo( pg->UpdateTime , pg->DataCount , NULL );
		return;
	}
	
	ImportBoardBaseStruct* BoardInfo = new ImportBoardBaseStruct[ pg->DataCount ];
	
	MyLzoClass myZip;
	myZip.Decode( (char*)pg->Data , pg->DataSize , pg->DataCount * sizeof( ImportBoardBaseStruct ) , (char*)BoardInfo );

	This->RD_ImportBoardInfo( pg->UpdateTime , pg->DataCount , BoardInfo );

	delete [] BoardInfo;

}
void NetCli_ImportBoard::_PG_ImportBoard_DtoC_ImportBoardNewVersion	( int Sockid , int Size , void* Data )
{
	PG_ImportBoard_DtoC_ImportBoardNewVersion* pg = (PG_ImportBoard_DtoC_ImportBoardNewVersion*)Data;
	This->RD_ImportBoardNewVersion( pg->UpdateTime );
}


void NetCli_ImportBoard::SL_ImportBoardRequest( float UpdateTime )
{
	PG_ImportBoard_CtoL_ImportBoardRequest Send;
	Send.UpdateTime = UpdateTime;

	SendToLocal_Base( sizeof(Send) , &Send );
}