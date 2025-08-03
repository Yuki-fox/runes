#include "../../mainproc/GameMain.h"
#include "NetCli_BG_GuildWarChild.h"
#include "../../Interface/GuildHouses/GuildHousesFrame.h"
#include "../../Interface/GuildHouses/GuildHouseStatus.h"
#include "../../Interface/WorldFrames/GuildWarScore.h"
//-----------------------------------------------------------------------------
bool NetCli_BG_GuildWarChild::_Init()
{
	if (This)
		return false;

	This = new NetCli_BG_GuildWarChild;


	return NetCli_BG_GuildWar::_Init();
}
//-----------------------------------------------------------------------------
bool NetCli_BG_GuildWarChild::_Release()
{
	NetCli_BG_GuildWar::_Release();
	if (This)
	{
		delete This;
		This = NULL;
	}
	return true;
}
//-----------------------------------------------------------------------------
//要求跨伺服器公會名稱結果
void NetCli_BG_GuildWarChild::RBL_GuildNameResult( int WorldGuildID , const char* GuildName )
{
	g_pGuildHousesFrame->RBL_GuildNameResult( WorldGuildID, GuildName );
}
//開啟註冊公會戰介面
void NetCli_BG_GuildWarChild::RL_OpenMenuResult(int PlayerDBID , int ZoneGroupID , GuildHouseWarStateENUM State , int NextTime , int Count , GuildHouseWarInfoStruct List[1000] )
{
	g_pGuildHousesFrame->RL_OpenMenuResult( PlayerDBID , ZoneGroupID , State ,NextTime, Count , List );
}
//註冊公會戰( or 取消 )結果
void NetCli_BG_GuildWarChild::RL_RegisterResult(GuildWarRegisterResultENUM Result , GuildHouseWarInfoStruct& Info )
{
	g_pGuildHousesFrame->RL_RegisterResult( Result, Info );
}
//公會戰狀況( 開始 , 準備 , 結束 )
void NetCli_BG_GuildWarChild::RL_Status(GuildWarStatusTypeENUM Status , int NextTime )
{
	g_pGuildHousesFrame->RL_Status( Status ,NextTime);
}
//要求進入結果
void NetCli_BG_GuildWarChild::RL_EnterResult(bool Result )
{
	g_pGuildHousesFrame->RL_EnterResult( Result );
}
//要求離開公會戰場結果
void NetCli_BG_GuildWarChild::RBL_LeaveResult(bool Result )
{
	g_pGuildHousesFrame->RBL_LeaveResult( Result );
}
//公會戰績分資訊
void NetCli_BG_GuildWarChild::RBL_GuildScore( GuildHouseWarFightStruct& Info , int Time )
{	
	g_pGuildWarScore->RBL_GuildScore(Info,Time);
	g_pGuildHousesFrame->RBL_GuildScore( Info );
}
//公會戰開始
void NetCli_BG_GuildWarChild::RBL_BeginWar(  )
{
	g_pGuildHousesFrame->RBL_BeginWar(  );
}
//公會戰結束
void NetCli_BG_GuildWarChild::RBL_EndWar(  )
{
	g_pGuildHousesFrame->RBL_EndWar(  );
}
//////////////////////////////////////////////////////////////////////////
//要求公會戰場玩家資料
//////////////////////////////////////////////////////////////////////////
void NetCli_BG_GuildWarChild::RBL_PlayerInfoResult( int TotalCount , int ID , GuildWarPlayerInfoStruct& Info )
{
	g_pGuildHouseWarStatus->UpdateScore( TotalCount, ID, &Info );
}

void NetCli_BG_GuildWarChild::RBL_PlayerFinalInfo( int PlayerDBID , int PrizeType , int Score , int OrderID , Voc_ENUM Voc , Voc_ENUM Voc_Sub , int Level , int Level_Sub)
{
	g_pGuildHouseWarStatus->UpdateFinalInfo( PlayerDBID, PrizeType, Score, OrderID, Voc, Voc_Sub, Level, Level_Sub );
}

//////////////////////////////////////////////////////////////////////////
//公會戰排行榜
void NetCli_BG_GuildWarChild::RL_GuildWarHisotry( GuildHouseWarHistoryStruct Hisotry[50] )
{
	g_pGuildHousesFrame->GuildWarLadderHisotryResult(Hisotry  );
}
void NetCli_BG_GuildWarChild::RL_GuildWarRankInfo( GuildWarRankInfoStruct& Info )
{
	g_pGuildHousesFrame->GuildWarLadderResult(Info  );

}