#pragma once

#include "PG/PG_Billboard.h"
#include "../../MainProc/NetGlobal.h"

class NetCli_Billboard : public CNetGlobal
{
protected:
    //-------------------------------------------------------------------
    static NetCli_Billboard* This;
    static bool _Init();
    static bool _Release();

	static void _PG_Billboard_LtoC_OpenBillboard	( int Sockid , int Size , void* Data );
	static void _PG_Billboard_LtoC_CloseBillboard	( int Sockid , int Size , void* Data );

	static void _PG_Billboard_DtoC_RankInfoResult	( int Sockid , int Size , void* Data );
	static void _PG_Billboard_DtoC_RankInfoList		( int Sockid , int Size , void* Data );


	static void _PG_NewBillboard_DtoC_SearchRankResult	( int Sockid , int Size , void* Data );
	static void _PG_NewBillboard_DtoC_SearchMyRankResult( int Sockid , int Size , void* Data );


public:    
    NetCli_Billboard() { This = this; }

	static void S_CloseBillboardRequest	( );
	static void S_RankInfoRequest		( BillBoardMainTypeENUM	Type , int RankBegin );
	static void S_SetAnonymous			( bool IsAnonymous );

	virtual void R_OpenBillboard( int TargetNpcID ) = 0;
	virtual void R_CloseBillboard( ) = 0;
	virtual void R_RankInfoResult( BillBoardMainTypeENUM Type , int RankBegin , int RankEnd , int RankMax , bool Result ) = 0;
	virtual void R_RankInfoList( BillBoardRoleInfoStruct& Info ) = 0;


public:
	//////////////////////////////////////////////////////////////////////////
	//新版的排行榜
	//////////////////////////////////////////////////////////////////////////
	static void SL_New_SearchRank	( int SortType , int MinRank , int MaxRank  );
	static void SL_New_SearchMyRank	( int SortType );
	static void SL_New_RegisterNote ( int SortType , const char* Note );

	virtual void RD_New_SearchRankResult	( int SortType , int TotalCount , int ID , NewBillBoardRoleInfoStruct& Info ) = 0;
	virtual void RD_New_SearchMyRankResult	( int SortType , int SortValue , int MyRank , int MyLastRank , const char* Note ) = 0;
};

