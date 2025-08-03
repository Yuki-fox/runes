#pragma once

#include "NetCli_BG_GuildWar.h"

class NetCli_BG_GuildWarChild : public NetCli_BG_GuildWar
{
public:	
	static	bool _Init();
	static	bool _Release();

public:

	//要求跨伺服器公會名稱結果
	virtual	void RBL_GuildNameResult( int WorldGuildID , const char* GuildName );
	//開啟註冊公會戰介面
	virtual	void RL_OpenMenuResult(int PlayerDBID , int ZoneGroupID , GuildHouseWarStateENUM State , int NextTime , int Count , GuildHouseWarInfoStruct List[1000] );
	//註冊公會戰( or 取消 )結果
	virtual	void RL_RegisterResult(GuildWarRegisterResultENUM Result , GuildHouseWarInfoStruct& Info );
	//公會戰狀況( 開始 , 準備 , 結束 )
	virtual	void RL_Status(GuildWarStatusTypeENUM Status , int NextTime );
	//要求進入結果
	virtual	void RL_EnterResult(bool Result );
	//要求離開公會戰場結果
	virtual	void RBL_LeaveResult(bool Result );

	//////////////////////////////////////////////////////////////////////////
	//進入戰場後 戰場資訊處理
	//////////////////////////////////////////////////////////////////////////
	//公會戰績分資訊
	virtual	void RBL_GuildScore( GuildHouseWarFightStruct& Info , int Time );
	//公會戰開始
	virtual	void RBL_BeginWar(  );
	//公會戰結束
	virtual	void RBL_EndWar(  );
	
	//////////////////////////////////////////////////////////////////////////
	//要求公會戰場玩家資料
	//////////////////////////////////////////////////////////////////////////
	virtual	void RBL_PlayerInfoResult( int TotalCount , int ID , GuildWarPlayerInfoStruct& Info );
	virtual	void RBL_PlayerFinalInfo( int PlayerDBID , int PrizeType , int Score , int OrderID , Voc_ENUM Voc , Voc_ENUM Voc_Sub , int Level , int Level_Sub );

	//////////////////////////////////////////////////////////////////////////
	//公會戰排行榜
	virtual	void RL_GuildWarHisotry( GuildHouseWarHistoryStruct Hisotry[50] );
	virtual	void RL_GuildWarRankInfo( GuildWarRankInfoStruct& Info );
};

