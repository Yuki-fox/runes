#include "NetCli_Billboard.h"

//-------------------------------------------------------------------
NetCli_Billboard*    NetCli_Billboard::This         = NULL;

//-------------------------------------------------------------------
bool NetCli_Billboard::_Init()
{
	/*
	_Net.RegOnSrvPacketFunction	( EM_PG_Billboard_LtoC_OpenBillboard    , _PG_Billboard_LtoC_OpenBillboard      );
	_Net.RegOnSrvPacketFunction	( EM_PG_Billboard_LtoC_CloseBillboard   , _PG_Billboard_LtoC_CloseBillboard     );
	_Net.RegOnSrvPacketFunction	( EM_PG_Billboard_DtoC_RankInfoResult 	, _PG_Billboard_DtoC_RankInfoResult   	);
	_Net.RegOnSrvPacketFunction	( EM_PG_Billboard_DtoC_RankInfoList     , _PG_Billboard_DtoC_RankInfoList		);
*/
	Cli_NetReg( PG_Billboard_LtoC_OpenBillboard 	);
	Cli_NetReg( PG_Billboard_LtoC_CloseBillboard	);
	Cli_NetReg( PG_Billboard_DtoC_RankInfoResult	);
	Cli_NetReg( PG_Billboard_DtoC_RankInfoList  	);


	Cli_NetReg( PG_NewBillboard_DtoC_SearchRankResult	);
	Cli_NetReg( PG_NewBillboard_DtoC_SearchMyRankResult  	);
    return true;
}
//-------------------------------------------------------------------
bool NetCli_Billboard::_Release()
{
    return true;
}
//-------------------------------------------------------------------
void NetCli_Billboard::_PG_NewBillboard_DtoC_SearchRankResult	( int Sockid , int Size , void* Data )
{
	PG_NewBillboard_DtoC_SearchRankResult* PG = (PG_NewBillboard_DtoC_SearchRankResult*)Data;
	This->RD_New_SearchRankResult( PG->SortType , PG->TotalCount , PG->ID , PG->Info );
}
void NetCli_Billboard::_PG_NewBillboard_DtoC_SearchMyRankResult	( int Sockid , int Size , void* Data )
{
	PG_NewBillboard_DtoC_SearchMyRankResult* PG = (PG_NewBillboard_DtoC_SearchMyRankResult*)Data;
	This->RD_New_SearchMyRankResult( PG->SortType , PG->SortValue , PG->MyRank , PG->MyLastRank , PG->MyNote.Begin() );
}

void NetCli_Billboard::_PG_Billboard_LtoC_OpenBillboard		( int Sockid , int Size , void* Data )
{
	PG_Billboard_LtoC_OpenBillboard* PG = (PG_Billboard_LtoC_OpenBillboard*)Data;
	This->R_OpenBillboard( PG->TargetNpcID );
}
void NetCli_Billboard::_PG_Billboard_LtoC_CloseBillboard	( int Sockid , int Size , void* Data )
{
	PG_Billboard_LtoC_CloseBillboard* PG = (PG_Billboard_LtoC_CloseBillboard*)Data;
	This->R_CloseBillboard( );
}

void NetCli_Billboard::_PG_Billboard_DtoC_RankInfoResult	( int Sockid , int Size , void* Data )
{
	PG_Billboard_DtoC_RankInfoResult* PG = (PG_Billboard_DtoC_RankInfoResult*)Data;
	This->R_RankInfoResult( PG->Type , PG->RankBegin , PG->RankEnd , PG->RankMax , PG->Result  );
}
void NetCli_Billboard::_PG_Billboard_DtoC_RankInfoList		( int Sockid , int Size , void* Data )
{	
	PG_Billboard_DtoC_RankInfoList* PG = (PG_Billboard_DtoC_RankInfoList*)Data;
	This->R_RankInfoList( PG->Info );
}

//-------------------------------------------------------------------
void NetCli_Billboard::S_CloseBillboardRequest	( )
{
	PG_Billboard_CtoL_CloseBillboardRequest Send;
	SendToLocal( sizeof(Send) , &Send );
}

void NetCli_Billboard::S_RankInfoRequest		( BillBoardMainTypeENUM	Type , int RankBegin )
{
	PG_Billboard_CtoL_RankInfoRequest Send;
	Send.Type = Type;
	Send.RankBegin = RankBegin;
	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Billboard::S_SetAnonymous			( bool IsAnonymous )
{
	PG_Billboard_CtoL_SetAnonymous Send;
	Send.IsAnonymous = IsAnonymous;	
	SendToLocal( sizeof(Send) , &Send );
}

void NetCli_Billboard::SL_New_SearchRank	( int SortType , int MinRank , int MaxRank )
{
	PG_NewBillboard_CtoL_SearchRank Send;
	Send.SortType = SortType;		//排行類型
	Send.MinRank = MinRank;
	Send.MaxRank = MaxRank;

	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_Billboard::SL_New_SearchMyRank	( int SortType )
{
	PG_NewBillboard_CtoL_SearchMyRank Send;
	Send.SortType = SortType;		//排行類型

	SendToLocal( sizeof(Send) , &Send );
}

void NetCli_Billboard::SL_New_RegisterNote ( int SortType , const char* Note )
{
	PG_NewBillboard_CtoL_RegisterNote Send;
	Send.SortType = SortType;		//排行類型
	Send.Note = Note;

	SendToLocal( sizeof(Send) , &Send );

}
//-------------------------------------------------------------------
