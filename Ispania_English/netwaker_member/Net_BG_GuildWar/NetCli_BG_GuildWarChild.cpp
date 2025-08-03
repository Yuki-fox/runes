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
//�n�D����A�����|�W�ٵ��G
void NetCli_BG_GuildWarChild::RBL_GuildNameResult( int WorldGuildID , const char* GuildName )
{
	g_pGuildHousesFrame->RBL_GuildNameResult( WorldGuildID, GuildName );
}
//�}�ҵ��U���|�Ԥ���
void NetCli_BG_GuildWarChild::RL_OpenMenuResult(int PlayerDBID , int ZoneGroupID , GuildHouseWarStateENUM State , int NextTime , int Count , GuildHouseWarInfoStruct List[1000] )
{
	g_pGuildHousesFrame->RL_OpenMenuResult( PlayerDBID , ZoneGroupID , State ,NextTime, Count , List );
}
//���U���|��( or ���� )���G
void NetCli_BG_GuildWarChild::RL_RegisterResult(GuildWarRegisterResultENUM Result , GuildHouseWarInfoStruct& Info )
{
	g_pGuildHousesFrame->RL_RegisterResult( Result, Info );
}
//���|�Ԫ��p( �}�l , �ǳ� , ���� )
void NetCli_BG_GuildWarChild::RL_Status(GuildWarStatusTypeENUM Status , int NextTime )
{
	g_pGuildHousesFrame->RL_Status( Status ,NextTime);
}
//�n�D�i�J���G
void NetCli_BG_GuildWarChild::RL_EnterResult(bool Result )
{
	g_pGuildHousesFrame->RL_EnterResult( Result );
}
//�n�D���}���|�Գ����G
void NetCli_BG_GuildWarChild::RBL_LeaveResult(bool Result )
{
	g_pGuildHousesFrame->RBL_LeaveResult( Result );
}
//���|���Z����T
void NetCli_BG_GuildWarChild::RBL_GuildScore( GuildHouseWarFightStruct& Info , int Time )
{	
	g_pGuildWarScore->RBL_GuildScore(Info,Time);
	g_pGuildHousesFrame->RBL_GuildScore( Info );
}
//���|�Զ}�l
void NetCli_BG_GuildWarChild::RBL_BeginWar(  )
{
	g_pGuildHousesFrame->RBL_BeginWar(  );
}
//���|�Ե���
void NetCli_BG_GuildWarChild::RBL_EndWar(  )
{
	g_pGuildHousesFrame->RBL_EndWar(  );
}
//////////////////////////////////////////////////////////////////////////
//�n�D���|�Գ����a���
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
//���|�ԱƦ�]
void NetCli_BG_GuildWarChild::RL_GuildWarHisotry( GuildHouseWarHistoryStruct Hisotry[50] )
{
	g_pGuildHousesFrame->GuildWarLadderHisotryResult(Hisotry  );
}
void NetCli_BG_GuildWarChild::RL_GuildWarRankInfo( GuildWarRankInfoStruct& Info )
{
	g_pGuildHousesFrame->GuildWarLadderResult(Info  );

}