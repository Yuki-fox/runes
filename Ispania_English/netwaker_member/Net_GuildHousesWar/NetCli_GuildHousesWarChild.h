/*
#pragma once

#include "NetCli_GuildHousesWar.h"

class NetCli_GuildHousesWarChild : public NetCli_GuildHousesWar
{
public:	
	static	bool _Init();
	static	bool _Release();

public:
	virtual void R_OpenMenu( GuildHouseWarStateENUM State , int Size , GuildHouseWarInfoStruct List[1000] );
	virtual void R_WarRegisterResult( GuildHouseWarRegisterTypeENUM Type , bool Result );
	virtual void R_WarBegin( int GuildID[2] );
	virtual void R_EnterWarResult( bool Result );
	virtual void R_LeaveWarResult( bool Result );
	virtual void R_EndWar( int GuildID[2] , int Score[2] );

	virtual void R_EndWarPlayerInfo	( int Count , GuildWarPlayerInfoStruct PlayerList[1000] );
	virtual void R_PricesResult		( bool Result );

	virtual void R_HistoryResult	( bool DayBefore , int Count , GuildHouseWarHistoryStruct List[1000] );

	virtual void R_GuildScore		( GuildHouseWarFightStruct& Info );
	virtual void R_PlayerScore		( int Count , GuildWarPlayerInfoStruct PlayerList[1000] );
};

*/