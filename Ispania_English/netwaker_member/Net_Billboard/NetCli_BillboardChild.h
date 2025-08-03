#pragma once


#include "NetCli_Billboard.h"

using namespace std;



class NetCli_BillboardChild : public NetCli_Billboard
{
public:	
	static bool _Init();
	static bool _Release();
	

	virtual void R_OpenBillboard( int TargetNpcID );
	virtual void R_CloseBillboard( );
	virtual void R_RankInfoResult( BillBoardMainTypeENUM Type , int RankBegin , int RankEnd , int RankMax , bool Result );
	virtual void R_RankInfoList( BillBoardRoleInfoStruct& Info );

public:
	static vector< BillBoardRoleInfoStruct >	m_BillboardInfo;
	static BillBoardMainTypeENUM				m_SearchType;
	static int									m_RankBegin;
	static int									m_RankEnd;
	static int									m_RankMax;
	static bool									m_Result;


public:
	//////////////////////////////////////////////////////////////////////////
	//新版的排行榜
	//////////////////////////////////////////////////////////////////////////
	virtual void RD_New_SearchRankResult	( int SortType , int TotalCount , int ID , NewBillBoardRoleInfoStruct& Info );
	virtual void RD_New_SearchMyRankResult	(  int SortType , int SortValue , int MyRank , int MyLastRank , const char* Note  );
};

