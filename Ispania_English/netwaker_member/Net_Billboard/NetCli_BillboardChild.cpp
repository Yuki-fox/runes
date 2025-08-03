#include "NetCli_BillboardChild.h"
#include "../../mainproc/GameMain.h"
#include "../Interface/WorldFrames/ChatFrame.h"
#include "../Interface/Billboard/BillboardFrame.h"
#include "../Interface/WorldFrames/UI_SortScoreBoard.h"
//-----------------------------------------------------------------------------
vector< BillBoardRoleInfoStruct >	NetCli_BillboardChild::m_BillboardInfo;
BillBoardMainTypeENUM				NetCli_BillboardChild::m_SearchType = EM_BillBoardMainType_Money;
int									NetCli_BillboardChild::m_RankBegin = 0;
int									NetCli_BillboardChild::m_RankEnd = 0;
int									NetCli_BillboardChild::m_RankMax = 0;
bool								NetCli_BillboardChild::m_Result = false;
//-----------------------------------------------------------------------------
bool NetCli_BillboardChild::_Init()
{
	if (This)
		return false;

	This = new NetCli_BillboardChild;
	return NetCli_Billboard::_Init();
}

//-----------------------------------------------------------------------------
bool NetCli_BillboardChild::_Release()
{
	NetCli_Billboard::_Release();
	if (This)
	{
		delete This;
		This = NULL;
	}
	return true;
}
//-----------------------------------------------------------------------------
void NetCli_BillboardChild::R_OpenBillboard( int TargetNpcID )
{
	g_pBillboardFrame->Open( TargetNpcID );
	//g_pChatFrame->AddMessage(5, "開啟排行榜");	
}
void NetCli_BillboardChild::R_CloseBillboard( )
{
	g_pBillboardFrame->Close( );
	//g_pChatFrame->AddMessage(5, "關閉排行榜");	
}
void NetCli_BillboardChild::R_RankInfoResult( BillBoardMainTypeENUM Type , int RankBegin , int RankEnd , int RankMax , bool Result )
{
	//char Buf[256];
	//sprintf( Buf , "[RankInfo] : Type=%d RankBegin=%d RankEnd=%d Result=%d" , Type , RankBegin , RankEnd , Result );	
	//g_pChatFrame->AddMessage(5 , Buf );

	m_Result = Result;
	if( Result == true )
	{
		m_BillboardInfo.clear();
		m_SearchType = Type;
		m_RankBegin = RankBegin;
		m_RankEnd = RankEnd;
		m_RankMax = RankMax;
	}
	else
	{
		g_pBillboardFrame->UpdateData();
	}
}
void NetCli_BillboardChild::R_RankInfoList( BillBoardRoleInfoStruct& Info )
{
//	char Buf[256];
//	sprintf( Buf , "[RankInfo] : Rank=%d Name=%s IsAnonymous=%d" , Info.Rank , Info.RoleName.Begin() , Info.IsAnonymous );	
//	g_pChatFrame->AddMessage(5 , Buf );

	m_BillboardInfo.push_back( Info );

	if( m_BillboardInfo.size() ==   m_RankEnd - m_RankBegin +1 )
		g_pBillboardFrame->UpdateData();
}
//-----------------------------------------------------------------------------
void NetCli_BillboardChild::RD_New_SearchRankResult	( int SortType , int TotalCount , int ID , NewBillBoardRoleInfoStruct& Info )
{
	if (g_pSortScoreBoard)
		g_pSortScoreBoard->SearchRankResult(SortType,TotalCount,ID,Info);

}
void NetCli_BillboardChild::RD_New_SearchMyRankResult	(  int SortType , int SortValue , int MyRank , int MyLastRank , const char* Note   )
{
	if (g_pSortScoreBoard)
		g_pSortScoreBoard->SearchMyRankResult(SortType,SortValue,MyRank,MyLastRank,Note);
}