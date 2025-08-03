/*
#include "../../mainproc/GameMain.h"
#include "NetCli_GuildHousesWarChild.h"
#include "../../Interface/GuildHouses/GuildHousesFrame.h"

//-----------------------------------------------------------------------------
bool NetCli_GuildHousesWarChild::_Init()
{
	if (This)
		return false;

	This = new NetCli_GuildHousesWarChild;


	return NetCli_GuildHousesWar::_Init();
}
//-----------------------------------------------------------------------------
bool NetCli_GuildHousesWarChild::_Release()
{
	NetCli_GuildHousesWar::_Release();
	if (This)
	{
		delete This;
		This = NULL;
	}
	return true;
}
//-----------------------------------------------------------------------------
void NetCli_GuildHousesWarChild::R_OpenMenu( GuildHouseWarStateENUM State , int Size , GuildHouseWarInfoStruct List[1000] )
{
	g_pGuildHousesFrame->R_OpenMenu( State, Size, List );
}
void NetCli_GuildHousesWarChild::R_WarRegisterResult( GuildHouseWarRegisterTypeENUM Type , bool Result )
{
	g_pGuildHousesFrame->R_WarRegisterResult( Type , Result );
}
void NetCli_GuildHousesWarChild::R_WarBegin( int GuildID[2] )
{
	g_pGuildHousesFrame->R_WarBegin( GuildID );
}
void NetCli_GuildHousesWarChild::R_EnterWarResult( bool Result )
{
	g_pGuildHousesFrame->R_EnterWarResult( Result );
}
void NetCli_GuildHousesWarChild::R_LeaveWarResult( bool Result )
{
	g_pGuildHousesFrame->R_LeaveWarResult( Result );
}
void NetCli_GuildHousesWarChild::R_EndWar( int GuildID[2] , int Score[2] )
{
	g_pGuildHousesFrame->R_EndWar( GuildID , Score );
}
void NetCli_GuildHousesWarChild::R_EndWarPlayerInfo	( int Count , GuildWarPlayerInfoStruct PlayerList[1000] )
{
	g_pGuildHousesFrame->R_EndWarPlayerInfo( Count , PlayerList );
}
void NetCli_GuildHousesWarChild::R_PricesResult		( bool Result )
{
	g_pGuildHousesFrame->R_PricesResult( Result );
}
void NetCli_GuildHousesWarChild::R_HistoryResult	( bool DayBefore , int Count , GuildHouseWarHistoryStruct List[1000] )
{

}

void NetCli_GuildHousesWarChild::R_GuildScore		( GuildHouseWarFightStruct& Info )
{

}
void NetCli_GuildHousesWarChild::R_PlayerScore		( int Count , GuildWarPlayerInfoStruct PlayerList[1000] )
{

}
*/