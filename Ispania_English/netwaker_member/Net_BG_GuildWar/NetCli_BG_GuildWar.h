#pragma once

#include "PG/PG_BG_GuildWar.h"
#include "../../MainProc/NetGlobal.h"

class NetCli_BG_GuildWar:public CNetGlobal
{
protected:
	static NetCli_BG_GuildWar *This;
	static bool _Init();
	static bool _Release();
	static void _PG_BG_GuildWar_BLtoC_GuildNameResult	( int Sockid , int Size , void* Data );
	static void _PG_BG_GuildWar_LtoC_OpenMenuResult		( int Sockid , int Size , void* Data );
	static void _PG_BG_GuildWar_LtoC_RegisterResult		( int Sockid , int Size , void* Data );
	static void _PG_BG_GuildWar_LtoC_Status				( int Sockid , int Size , void* Data );
	static void _PG_BG_GuildWar_LtoC_EnterResult		( int Sockid , int Size , void* Data );
	static void _PG_BG_GuildWar_BLtoC_LeaveResult		( int Sockid , int Size , void* Data );
	static void _PG_BG_GuildWar_BLtoC_GuildScore		( int Sockid , int Size , void* Data );
	static void _PG_BG_GuildWar_BLtoC_BeginWar			( int Sockid , int Size , void* Data );
	static void _PG_BG_GuildWar_BLtoC_EndWar			( int Sockid , int Size , void* Data );
	static void _PG_BG_GuildWar_BLtoC_PlayerInfoResult	( int Sockid , int Size , void* Data );
	static void _PG_BG_GuildWar_BLtoC_PlayerFinalInfo	( int Sockid , int Size , void* Data );
	static void _PG_BG_GuildWar_BLtoC_PlayerListInfo_Zip( int Sockid , int Size , void* Data );

	static void _PG_BG_GuildWar_LtoC_GuildWarHisotry	( int Sockid , int Size , void* Data );
	static void _PG_BG_GuildWar_LtoC_GuildWarRankInfo	( int Sockid , int Size , void* Data );
public:

	//要求跨伺服器公會名稱要求
	static void	SBL_GuildNameRequest( int WorldGuildID );
		//要求跨伺服器公會名稱結果
		virtual	void RBL_GuildNameResult( int WorldGuildID , const char* GuildName ) = 0;

	//開啟註冊公會戰介面
	static void	SL_OpenMenuRequest( );
		//開啟註冊公會戰介面
		virtual	void RL_OpenMenuResult(int PlayerDBID , int ZoneGroupID , GuildHouseWarStateENUM State , int NextTime , int Count , GuildHouseWarInfoStruct List[1000] ) = 0;

	//註冊公會戰( or 取消 )
	static void	SL_RegisterRequest( int ZoneGroupID , GuildWarRegisterTypeENUM Type , int GuildCount , bool IsAcceptAssignment = false , int TargetGuild = 0 );
		//註冊公會戰( or 取消 )結果
		virtual	void RL_RegisterResult(GuildWarRegisterResultENUM Result,GuildHouseWarInfoStruct& Info ) = 0;

	//公會戰狀況( 開始 , 準備 , 結束 )
	virtual	void RL_Status(GuildWarStatusTypeENUM Status , int NextTime ) = 0;
	//要求進入公會戰場
	static void	SL_EnterRequest( int ZoneGroupID );
	//要求進入結果
	virtual	void RL_EnterResult(bool Result ) = 0;

	//要求離開公會戰場
	static void	SBL_LeaveRequest( );
		//要求離開公會戰場結果
		virtual	void RBL_LeaveResult(bool Result ) = 0;

	//公會戰績分資訊
	virtual	void RBL_GuildScore( GuildHouseWarFightStruct& Info , int Time  ) = 0;
	//公會戰開始
	virtual	void RBL_BeginWar(  ) = 0;
	//公會戰結束
	virtual	void RBL_EndWar(  ) = 0;

	//要求公會戰場玩家資料
	static void	SBL_PlayerInfoRequest( );
		virtual	void RBL_PlayerInfoResult( int TotalCount , int ID , GuildWarPlayerInfoStruct& Info ) = 0;
	virtual	void RBL_PlayerFinalInfo( int PlayerDBID , int PrizeType , int Score , int OrderID , Voc_ENUM Voc , Voc_ENUM Voc_Sub , int Level , int Level_Sub ) = 0;

	//////////////////////////////////////////////////////////////////////////
	//公會戰排行榜
	static void SL_GuildWarRankInfoRequest( GuildWarRankInfoTypeENUM Type , int WorldGuildID );
	virtual	void RL_GuildWarHisotry( GuildHouseWarHistoryStruct Hisotry[50] ) = 0;
	virtual	void RL_GuildWarRankInfo( GuildWarRankInfoStruct& Info ) = 0;
};
