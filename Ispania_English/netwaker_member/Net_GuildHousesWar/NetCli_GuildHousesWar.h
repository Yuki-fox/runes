/*
#pragma once

#include "PG/PG_GuildHousesWar.h"
#include "../../MainProc/NetGlobal.h"

class NetCli_GuildHousesWar : public CNetGlobal
{
protected:
    //-------------------------------------------------------------------
    static	NetCli_GuildHousesWar* This;
    static	bool _Init();
    static	bool _Release();	

	static void _PG_GuildHousesWar_DtoC_OpenMenu				( int Sockid , int Size , void* Data );
	static void _PG_GuildHousesWar_DtoC_WarRegisterResult		( int Sockid , int Size , void* Data );
	static void _PG_GuildHousesWar_LtoC_WarBegin				( int Sockid , int Size , void* Data );
	static void _PG_GuildHousesWar_LtoC_EnterWarResult			( int Sockid , int Size , void* Data );
	static void _PG_GuildHousesWar_LtoC_LeaveWarResult			( int Sockid , int Size , void* Data );
	static void _PG_GuildHousesWar_LtoC_EndWar					( int Sockid , int Size , void* Data );

	static void _PG_GuildHousesWar_LtoC_EndWarPlayerInfo		( int Sockid , int Size , void* Data );
	static void _PG_GuildHousesWar_LtoC_PricesResult			( int Sockid , int Size , void* Data );
	static void _PG_GuildHousesWar_DtoC_HistoryResult			( int Sockid , int Size , void* Data );

	static void _PG_GuildHousesWar_LtoC_GuildScore				( int Sockid , int Size , void* Data );
	static void _PG_GuildHousesWar_LtoC_PlayerScore				( int Sockid , int Size , void* Data );
public:    
    NetCli_GuildHousesWar() { This = this; }
	static	NetCli_GuildHousesWar*	GetThis()						{ return This; }

	static void SL_OpenMenu		( );
	static void SL_WarRegister	( GuildHouseWarRegisterTypeENUM Type );
	static void SL_EnterWar		( );
	static void SL_LeaveWar		( );
	static void SL_PricesRequest( );

	virtual void R_OpenMenu			( GuildHouseWarStateENUM State , int Size , GuildHouseWarInfoStruct List[1000] ) = 0;
	virtual void R_WarRegisterResult( GuildHouseWarRegisterTypeENUM Type , bool Result ) = 0;
	virtual void R_WarBegin			( int GuildID[2] ) = 0;
	virtual void R_EnterWarResult	( bool Result ) = 0;
	virtual void R_LeaveWarResult	( bool Result ) = 0;
	virtual void R_EndWar			( int GuildID[2] , int Score[2] ) = 0;
	virtual void R_PlayerScore		( int Count , GuildWarPlayerInfoStruct PlayerList[1000] ) = 0;

	virtual void R_EndWarPlayerInfo	( int Count , GuildWarPlayerInfoStruct PlayerList[1000] ) = 0;
	virtual void R_PricesResult		( bool Result ) = 0;

	//////////////////////////////////////////////////////////////////////////
	//要求歷史紀錄
	static void SL_HistoryRequest	( int DayBefore );
	virtual void R_HistoryResult	( bool DayBefore , int Count , GuildHouseWarHistoryStruct List[1000] ) = 0;

	//////////////////////////////////////////////////////////////////////////
	//定時修正公會戰績分
	virtual void R_GuildScore		( GuildHouseWarFightStruct& Info ) = 0;
	//要求個人積分
	static void SL_PlayerScoreRequest( );


};

*/