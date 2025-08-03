#include "NetCli_BG_GuildWar.h"

NetCli_BG_GuildWar* NetCli_BG_GuildWar::This = NULL;
bool NetCli_BG_GuildWar::_Init()
{
	Cli_NetReg( PG_BG_GuildWar_BLtoC_GuildNameResult	);
	Cli_NetReg( PG_BG_GuildWar_LtoC_OpenMenuResult		);
	Cli_NetReg( PG_BG_GuildWar_LtoC_RegisterResult		);
	Cli_NetReg( PG_BG_GuildWar_LtoC_Status				);
	Cli_NetReg( PG_BG_GuildWar_LtoC_EnterResult			);
	Cli_NetReg( PG_BG_GuildWar_BLtoC_LeaveResult		);
	Cli_NetReg( PG_BG_GuildWar_BLtoC_GuildScore			);
	Cli_NetReg( PG_BG_GuildWar_BLtoC_BeginWar			);
	Cli_NetReg( PG_BG_GuildWar_BLtoC_EndWar				);

	Cli_NetReg( PG_BG_GuildWar_BLtoC_PlayerInfoResult	);
	Cli_NetReg( PG_BG_GuildWar_BLtoC_PlayerFinalInfo	);
	Cli_NetReg( PG_BG_GuildWar_BLtoC_PlayerListInfo_Zip	);

	Cli_NetReg( PG_BG_GuildWar_LtoC_GuildWarHisotry		);
	Cli_NetReg( PG_BG_GuildWar_LtoC_GuildWarRankInfo	);
	return true;
}
bool NetCli_BG_GuildWar::_Release()
{
	return true;
}
//////////////////////////////////////////////////////////////////////////
void NetCli_BG_GuildWar::_PG_BG_GuildWar_LtoC_GuildWarHisotry	( int Sockid , int Size , void* Data )
{
	PG_BG_GuildWar_LtoC_GuildWarHisotry* PG = (PG_BG_GuildWar_LtoC_GuildWarHisotry*)Data;
	This->RL_GuildWarHisotry( PG->Hisotry );
}
void NetCli_BG_GuildWar::_PG_BG_GuildWar_LtoC_GuildWarRankInfo	( int Sockid , int Size , void* Data )
{
	PG_BG_GuildWar_LtoC_GuildWarRankInfo* PG = (PG_BG_GuildWar_LtoC_GuildWarRankInfo*)Data;
	This->RL_GuildWarRankInfo( PG->Info );
}

void NetCli_BG_GuildWar::_PG_BG_GuildWar_BLtoC_PlayerListInfo_Zip( int Sockid , int Size , void* Data )
{

	PG_BG_GuildWar_BLtoC_PlayerListInfo_Zip* PG = (PG_BG_GuildWar_BLtoC_PlayerListInfo_Zip*) Data;


	PlayerListInfoBase Temp;

	//	BaseRoleData	BaseData;


	MyLzoClass myZip;
	int RetSize = myZip.Decode( (const char*)PG->Data , PG->DataSize , sizeof(Temp) , (char*)&Temp );
	
	for( int i = 0 ; i < Temp.Count ; i++ )
	{
		This->RBL_PlayerInfoResult( Temp.Count , i , Temp.Info[i] );
	}

}
void NetCli_BG_GuildWar::_PG_BG_GuildWar_BLtoC_PlayerInfoResult	( int Sockid , int Size , void* Data )
{
	PG_BG_GuildWar_BLtoC_PlayerInfoResult* PG = (PG_BG_GuildWar_BLtoC_PlayerInfoResult*)Data;
	This->RBL_PlayerInfoResult( PG->TotalCount , PG->ID , PG->Info );
}
void NetCli_BG_GuildWar::_PG_BG_GuildWar_BLtoC_PlayerFinalInfo	( int Sockid , int Size , void* Data )
{
	PG_BG_GuildWar_BLtoC_PlayerFinalInfo* PG = (PG_BG_GuildWar_BLtoC_PlayerFinalInfo*)Data;
	This->RBL_PlayerFinalInfo( PG->PlayerDBID , PG->PrizeType , PG->Score , PG->OrderID , PG->Voc , PG->Voc_Sub , PG->Level , PG->Level_Sub );
}
//////////////////////////////////////////////////////////////////////////
//要求跨伺服器公會名稱結果
void NetCli_BG_GuildWar::_PG_BG_GuildWar_BLtoC_GuildNameResult( int Sockid , int Size , void* Data )
{
	PG_BG_GuildWar_BLtoC_GuildNameResult* PG = (PG_BG_GuildWar_BLtoC_GuildNameResult*)Data;
	This->RBL_GuildNameResult( PG->WorldGuildID , PG->GuildName );
}
//////////////////////////////////////////////////////////////////////////
//開啟註冊公會戰介面
void NetCli_BG_GuildWar::_PG_BG_GuildWar_LtoC_OpenMenuResult( int Sockid , int Size , void* Data )
{
	PG_BG_GuildWar_LtoC_OpenMenuResult* PG = (PG_BG_GuildWar_LtoC_OpenMenuResult*)Data;
	This->RL_OpenMenuResult( PG->PlayerDBID , PG->ZoneGroupID , PG->State , PG->NextTime , PG->Count , PG->List );
}
//////////////////////////////////////////////////////////////////////////
//註冊公會戰( or 取消 )結果
void NetCli_BG_GuildWar::_PG_BG_GuildWar_LtoC_RegisterResult( int Sockid , int Size , void* Data )
{
	PG_BG_GuildWar_LtoC_RegisterResult* PG = (PG_BG_GuildWar_LtoC_RegisterResult*)Data;
	This->RL_RegisterResult( PG->Result , PG->Info );
}
//////////////////////////////////////////////////////////////////////////
//公會戰狀況( 開始 , 準備 , 結束 )
void NetCli_BG_GuildWar::_PG_BG_GuildWar_LtoC_Status( int Sockid , int Size , void* Data )
{
	PG_BG_GuildWar_LtoC_Status* PG = (PG_BG_GuildWar_LtoC_Status*)Data;
	This->RL_Status( PG->Status , PG->NextTime );
}
//////////////////////////////////////////////////////////////////////////
//要求進入結果
void NetCli_BG_GuildWar::_PG_BG_GuildWar_LtoC_EnterResult( int Sockid , int Size , void* Data )
{
	PG_BG_GuildWar_LtoC_EnterResult* PG = (PG_BG_GuildWar_LtoC_EnterResult*)Data;
	This->RL_EnterResult( PG->Result );
}
//////////////////////////////////////////////////////////////////////////
//要求離開公會戰場結果
void NetCli_BG_GuildWar::_PG_BG_GuildWar_BLtoC_LeaveResult( int Sockid , int Size , void* Data )
{
	PG_BG_GuildWar_BLtoC_LeaveResult* PG = (PG_BG_GuildWar_BLtoC_LeaveResult*)Data;
	This->RBL_LeaveResult( PG->Result );
}
//////////////////////////////////////////////////////////////////////////
//公會戰績分資訊
void NetCli_BG_GuildWar::_PG_BG_GuildWar_BLtoC_GuildScore( int Sockid , int Size , void* Data )
{
	PG_BG_GuildWar_BLtoC_GuildScore* PG = (PG_BG_GuildWar_BLtoC_GuildScore*)Data;
	This->RBL_GuildScore( PG->Info , PG->Time );
}
//////////////////////////////////////////////////////////////////////////
//公會戰開始
void NetCli_BG_GuildWar::_PG_BG_GuildWar_BLtoC_BeginWar( int Sockid , int Size , void* Data )
{
	PG_BG_GuildWar_BLtoC_BeginWar* PG = (PG_BG_GuildWar_BLtoC_BeginWar*)Data;
	This->RBL_BeginWar(  );
}
//////////////////////////////////////////////////////////////////////////
//公會戰結束
void NetCli_BG_GuildWar::_PG_BG_GuildWar_BLtoC_EndWar( int Sockid , int Size , void* Data )
{
	PG_BG_GuildWar_BLtoC_EndWar* PG = (PG_BG_GuildWar_BLtoC_EndWar*)Data;
	This->RBL_EndWar(  );
}
//////////////////////////////////////////////////////////////////////////
//要求跨伺服器公會名稱要求
void NetCli_BG_GuildWar::SBL_GuildNameRequest( int WorldGuildID )
{
	PG_BG_GuildWar_CtoBL_GuildNameRequest Send;
	Send.WorldGuildID = WorldGuildID;
	SendToLocal( sizeof(Send) , &Send );
}
//////////////////////////////////////////////////////////////////////////
//開啟註冊公會戰介面
void NetCli_BG_GuildWar::SL_OpenMenuRequest(  )
{
	PG_BG_GuildWar_CtoL_OpenMenuRequest Send;
	SendToLocal( sizeof(Send) , &Send );
}
//////////////////////////////////////////////////////////////////////////
//註冊公會戰( or 取消 )
void NetCli_BG_GuildWar::SL_RegisterRequest( int ZoneGroupID , GuildWarRegisterTypeENUM Type , int GuildCount , bool IsAcceptAssignment , int TargetGuild )
{
	PG_BG_GuildWar_CtoL_RegisterRequest Send;
	Send.Type = Type;
	Send.ZoneGroupID = ZoneGroupID;
	Send.GuildCount = GuildCount;
	Send.IsAcceptAssignment = IsAcceptAssignment;
	Send.TargetGuild = TargetGuild;
	SendToLocal( sizeof(Send) , &Send );
}
//////////////////////////////////////////////////////////////////////////
//要求進入公會戰場
void NetCli_BG_GuildWar::SL_EnterRequest( int ZoneGroupID  )
{
	PG_BG_GuildWar_CtoL_EnterRequest Send;
	Send.ZoneGroupID = ZoneGroupID;
	SendToLocal( sizeof(Send) , &Send );
}
//////////////////////////////////////////////////////////////////////////
//要求離開公會戰場
void NetCli_BG_GuildWar::SBL_LeaveRequest(  )
{
	PG_BG_GuildWar_CtoBL_LeaveRequest Send;
	SendToLocal( sizeof(Send) , &Send );
}
//////////////////////////////////////////////////////////////////////////
void NetCli_BG_GuildWar::SBL_PlayerInfoRequest( )
{
	//PG_BG_GuildWar_CtoBL_PlayerInfoRequest Send;
	//SendToLocal( sizeof(Send) , &Send );
}
void NetCli_BG_GuildWar::SL_GuildWarRankInfoRequest( GuildWarRankInfoTypeENUM Type , int WorldGuildID )
{
	PG_BG_GuildWar_CtoL_GuildWarRankInfoRequest Send;
	Send.Type = Type;
	Send.WorldGuildID = WorldGuildID;
	SendToLocal_Base( sizeof(Send) , &Send );
}