#include "../../mainproc/GameMain.h"
#include "NetCli_GuildChild.h"
#include "../net_other/NetCli_OtherChild.h"
#include "../interface/debug/DebugMessage.h"
#include "../interface/WorldFrames/ChatFrame.h"
#include "../Interface/WorldFrames/GuildFrame.h"
#include "../Interface/GuildHouses/GuildHousesFrame.h"







bool								NetCli_GuildChild::m_bGuildBoardRequesting = false;

int									NetCli_GuildChild::m_iBiddingCrystalID		=0;
int									NetCli_GuildChild::m_iBiddingScore			=0;

int									NetCli_GuildChild::m_iGuildFightInfoStep	=0;

vector<GuildDeclareWarInfo>			NetCli_GuildChild::s_declareWarInfo;


//-----------------------------------------------------------------------------
bool NetCli_GuildChild::_Init()
{
	
	if (This)
		return false;

	
	This = new NetCli_GuildChild;

	return NetCli_Guild::_Init();
}

//-----------------------------------------------------------------------------
bool NetCli_GuildChild::_Release()
{
	NetCli_Guild::_Release();
	if (This)
	{
		delete This;
		This = NULL;
	}
	return true;

}
//-----------------------------------------------------------------------------
void NetCli_GuildChild::RL_CreateGuildResult( int GuildID , char* GuildName , CreateGuildResultENUM Result )
{
	char tmp[MAX_PATH];
	sprintf(tmp, "建立連署公會(%s=%d) %d",GuildName,GuildID, Result );
	g_pGuildFrame->CreateGuildResult(GuildID,GuildName,Result);
	g_pDebugMessage->DebugOutput( 2 , tmp);

}
//-----------------------------------------------------------------------------
void NetCli_GuildChild::RL_NewGuildInvite( int GuildID, char* LeaderName, int LeaderDBID )
{
	char tmp[MAX_PATH];
	sprintf(tmp, "邀請加入連署公會(%d) 要請人(%s) DBID(%d)", GuildID, LeaderName, LeaderDBID);
	g_pGuildFrame->NewGuildInvite(GuildID,LeaderName,LeaderDBID);
	g_pDebugMessage->DebugOutput( 2 , tmp);
}
//-----------------------------------------------------------------------------
void NetCli_GuildChild::RL_NewGuildInviteResult	( char* PlayerName , NewGuildInviteResultENUM	Result )
{
	char tmp[MAX_PATH]="";
	const char * pStr;

	pStr=g_ObjectData->GetString("GUILD_MSG_MEMBERFULLORFAIL");
	sprintf(tmp, pStr, PlayerName);
	switch( Result )
	{
	case EM_NewGuildInviteResult_OK			:	//要請成
		pStr=g_ObjectData->GetString("GUILD_MSG_ADDMEMBER");
			
		sprintf(tmp, pStr, PlayerName);
	//	g_pChatFrame->SendMsgEvent("CHAT_MSG_SYSTEM", tmp, "");

		// 重複要求資料,或者只加入單筆玩家資料
		break;

	case EM_NewGuildInviteResult_Failed:
		pStr=g_ObjectData->GetString("GUILD_MSG_MEMBERFULLORFAIL");
		sprintf(tmp, pStr, PlayerName);
		break;

	case EM_NewGuildInviteResult_NotFind	:	//找不到 想邀請者
		pStr=g_ObjectData->GetString("GUILD_MSG_NOTFIND");
		sprintf(tmp, pStr, PlayerName);
		
		//sprintf(tmp, "%s 加入連署失敗 (找不到 想邀請者)", PlayerName );
		//g_pDebugMessage->DebugOutput( 2 , ");
	
		break;

	case EM_NewGuildInviteResult_HasGuild	:	//想邀請者 已經有公會

		pStr=g_ObjectData->GetString("GUILD_MSG_PLAYERHAVEGUILD");
		sprintf(tmp, pStr, PlayerName);
		//sprintf(tmp,  "%s 加入連署失敗 (想邀請者 已經有公會)", PlayerName);
	//	g_pDebugMessage->DebugOutput( 2 , "%s 加入連署失敗 (想邀請者 已經有公會)", PlayerName );
		break;

	case EM_NewGuildInviteResult_Disagree	:	//想邀請者 不同意
		pStr=g_ObjectData->GetString("GUILD_MSG_PLAYER_SAYNO");
		sprintf(tmp, pStr, PlayerName);
		
	//	sprintf(tmp,  "%s 加入連署失敗 (想邀請者 不同意)", PlayerName );
	//	g_pDebugMessage->DebugOutput( 2 , "%s 加入連署失敗 (想邀請者 不同意)", PlayerName );
		break;
	}
	g_pChatFrame->SendMsgEvent("CHAT_MSG_SYSTEM", tmp, "");
}

void NetCli_GuildChild::RL_JoindNewGuild			( int GuildID , char* LeaderName )
{
	g_pGuildFrame->JoinNewGuild(GuildID,LeaderName);
	const char * pStr;
	pStr=GetGuildNameByID(GuildID);

	g_pDebugMessage->DebugOutput( 2 , "加入%s 的%s連署公會" ,LeaderName, pStr );
}

void NetCli_GuildChild::RL_LeaveNewGuildResult( char* KickPlayerName , bool Result )
{
	g_pGuildFrame->LeaveNewGuildResult(KickPlayerName,Result);

	if( Result )
	{	
		g_pDebugMessage->DebugOutput( 2 , "要求 %s 離開連署公會 " , KickPlayerName );
	}
	else
	{
		g_pDebugMessage->DebugOutput( 2 ,"要求 %s 離開連署公會 失敗" , KickPlayerName );
	}

}
//會長專用
void NetCli_GuildChild::RL_DelNewGuildResult		( bool Result )
{
	if( Result )
	{
		_RoleData.BaseData.GuildID = 0;
		g_pDebugMessage->DebugOutput( 2 , "刪除連署公會 " );
	}
	else
	{
		g_pDebugMessage->DebugOutput( 2 , "刪除連署公會 失敗" );
	}
}

//////////////////////////////////////////////////////////////////////////
void NetCli_GuildChild::RL_GuildInvite		( int GuildID , char* LeaderName , int LeaderDBID )
{
	g_pDebugMessage->DebugOutput( 2 , "邀請加入公會(%s) 要請人(%s) DBID(%d)" ,GetGuildNameByID( GuildID) , LeaderName , LeaderDBID);

	//g_pGuildFrame->PetitionSignature(GuildID, LeaderName, LeaderDBID);
	g_pGuildFrame->GuildInviteRequest(GuildID, LeaderName, LeaderDBID);
}
void NetCli_GuildChild::RL_GuildInviteResult( char* PlayerName , GuildInviteResultENUM	Result )
{
	g_pGuildFrame->GuildInviteResult(PlayerName,Result);
	g_pDebugMessage->DebugOutput( 2 , "邀請 %s Result=%d" ,PlayerName, Result );
}
void NetCli_GuildChild::RL_JoindGuild		( int GuildID , char* LeaderName )
{
	g_pGuildFrame->JoinGuild(GuildID,LeaderName);
	const char * pStr;
	pStr=GetGuildNameByID(GuildID);

	g_pDebugMessage->DebugOutput( 2 , "加入%s 的%s公會" ,LeaderName, pStr );
	
}
void NetCli_GuildChild::RL_LeaveGuildResult	( char* KickPlayerName , bool Result )
{
	g_pGuildFrame->LeaveGuildResult(KickPlayerName,Result);

	if( Result )
	{	
		g_pDebugMessage->DebugOutput( 2 , "要求 %s 離開公會 " , KickPlayerName );
	}
	else
	{
		g_pDebugMessage->DebugOutput( 2 ,"要求 %s 離開公會 失敗" , KickPlayerName );
	}
	
}


void NetCli_GuildChild::RL_DelGuildResult	( bool Result )
{
	if( Result )
	{
		g_pDebugMessage->DebugOutput( 2 , "刪除公會 " );
	}
	else
	{
		g_pDebugMessage->DebugOutput( 2 , "刪除公會 失敗" );
	}
}
/////////////////////////////////////////////////////////////////////////
//--接收到公會資料 可能是查詢其他工會的 或是自己的公會
void NetCli_GuildChild::RL_GuildInfo(GuildBaseStruct& Guild , int MemberCount )
{
	g_pGuildFrame->GuildInfo(Guild,MemberCount,m_bGuildBoardRequesting);
	m_bGuildBoardRequesting=false; 	
	g_pDebugMessage->DebugOutput( 2 , "收到公會(%s %d)資料 " , Guild.GuildName.Begin() , Guild.GuildID );
}
void NetCli_GuildChild::RD_ModifySimpleGuild( SimpleGuildInfoStruct& SimpleGuildInfo )
{

	g_pGuildFrame->ModifySimpleGuild(SimpleGuildInfo);
}
void NetCli_GuildChild::RL_GuildMemberBaseInfo	( vector< GuildMemberBaseInfoStruct >& MemberList )
{
	g_pGuildFrame->GuildMemberBaseInfo(MemberList);
	
	g_pDebugMessage->DebugOutput( 2 , "收到公會成員列表" );
	
}
void NetCli_GuildChild::RL_GuildMemberInfo		( GuildMemberStruct& Member )
{
	g_pGuildFrame->GuildMemberInfo(Member);
	
	g_pDebugMessage->DebugOutput( 2 , "收到公會成員 (%s) 資料 " , Member.MemberName.Begin() );
}
void NetCli_GuildChild::RD_GuildCreate			( int GuildID )
{
	
	if (g_pGuildFrame)
		g_pGuildFrame->GuildCreate(GuildID);
	//ReloadGuildDate();

	char temp[MAX_PATH];
	const char *pGuildName=GetGuildNameByID(GuildID);

	sprintf(temp," %s成為正式公會", pGuildName);
	g_pDebugMessage->DebugOutput( 2 , temp  );
}
//////////////////////////////////////////////////////////////////////////
void NetCli_GuildChild::RD_BoardPostResult			( bool Result )
{
		g_pGuildFrame->RecvBoardPost(Result);
	if( Result )
		g_pDebugMessage->DebugOutput( 2 , "Post文章"  );
	else
		g_pDebugMessage->DebugOutput( 2 , "Post文章失敗"  );
}
void NetCli_GuildChild::RD_BoardListCount			( int MaxCount , int Count , int PageID )
{
	if (g_pGuildFrame)
		g_pGuildFrame->SetBBSPageCount(MaxCount,Count,PageID);

	g_pDebugMessage->DebugOutput( 2 , "BoardList MaxCount=%d Count=%d PageID=%d" , MaxCount , Count , PageID  );
}
void NetCli_GuildChild::RD_BoardListData			( int MessageGUID , int Index , char* PlayerName , int Time , char* Title , GuildBoardModeStruct Type )
{
	if (g_pGuildFrame)
		g_pGuildFrame->SetBBSPostDate(MessageGUID,Index,PlayerName,Time,Title,Type);

	g_pDebugMessage->DebugOutput( 2 , "BoardData Index=%d Title=%s" , Index , Title );
}
void NetCli_GuildChild::RD_BoardMessageResult		( GuildBoardStruct&	Message )
{
	if (g_pGuildFrame)
		g_pGuildFrame->SetBoardMessage(Message);

	g_pDebugMessage->DebugOutput( 2 , "BoardMessage Title=%s" , Message.Title.Begin() );
}
void NetCli_GuildChild::RD_BoardModifyMessageResult( bool Result )
{
	if( Result )
		g_pDebugMessage->DebugOutput( 2 , "修改文章"  );
	else
		g_pDebugMessage->DebugOutput( 2 , "修改文章失敗"  );

}
void NetCli_GuildChild::RD_BoardModifyModeResult	( bool Result )
{
	if( Result )
		g_pDebugMessage->DebugOutput( 2 , "修改文章屬性"  );
	else
		g_pDebugMessage->DebugOutput( 2 , "修改文章屬性失敗"  );
}

void NetCli_GuildChild::RL_OpenGuildContribution		( int TargetNPCID)
{
	g_pGuildFrame->OpenGuildContribution(TargetNPCID);
	g_pDebugMessage->DebugOutput( 2 , "開起公會貢獻介面"  );
}
void NetCli_GuildChild::RL_GuildContributionItemResult	( bool Result )
{


	g_pGuildFrame->GuildContributionItemResult(Result);
	
	g_pDebugMessage->DebugOutput( 2 , "設定貢獻物品的結果"  );
}
void NetCli_GuildChild::RL_GuildContributionClose		( )
{
	g_pGuildFrame->CloseGuildContribution();
	g_pDebugMessage->DebugOutput( 2 , "要求關閉貢獻介面"  );
}
void NetCli_GuildChild::RL_OpenCreateGuild			( int TargetNPCID)
{
	g_pGuildFrame->OpenGuildCommand(TargetNPCID);
	g_pDebugMessage->DebugOutput( 2 , "要求開啟建立公會介面"  );
}
void NetCli_GuildChild::RL_CloseCreateGuild			( )
{
	g_pGuildFrame->CloseGuildCommand();
	g_pDebugMessage->DebugOutput( 2 , "要求關閉建立公會介面"  );
}

void NetCli_GuildChild::RL_AllGuildMemberCountResult( int Size , GuildMemberCountStruct MemberList[ DEF_MAX_PG_GUILD_MEMBER_COUNT_ ] )
{


}


// 玩家加入自己的公會
void NetCli_GuildChild::RD_JoinGuildMember			( GuildMemberStruct& Member )
{
	g_pGuildFrame->JoinGuildMember(Member);

	char Buf[256];
	sprintf( Buf , "加入新公會成員 %s" , Member.MemberName.Begin());	
	g_pDebugMessage->DebugOutput( 2 , Buf  );
}
// 玩家離開自己的公會
void NetCli_GuildChild::RD_LeaveGuildMember		( int LeaderDBID , int MemberDBID )
{

	g_pGuildFrame->LeaveGuildMember(LeaderDBID,MemberDBID);

	char Buf[256];

	sprintf( Buf , "公會成員離開公會 %d" , MemberDBID	);

	g_pDebugMessage->DebugOutput( 2 , Buf  );
}

/*
void NetCli_GuildChild::RL_GuildShopOpen( int TargetNPCID , int Score , StaticFlag<256>& Flag )
{
	g_pGuildFrame->OpenGuildShop(TargetNPCID,Score,Flag);

	g_pDebugMessage->DebugOutput( 2 , "開啟公會商店" );
}
void NetCli_GuildChild::RD_GuildShopBuyResult( int FlagID , bool Result )
{
		g_pGuildFrame->GuildShopBuyResult(FlagID,Result);

		char Buf[256];

		///if ()
	g_pDebugMessage->DebugOutput( 2 , Buf );
}
void NetCli_GuildChild::RL_GuildShopClose( )
{

	g_pGuildFrame->CloseGuildShop();
	g_pDebugMessage->DebugOutput( 2 , "要求關閉公會商店" );
}
*/

//////////////////////////////////////////////////////////////////////////
//及時資訊
//////////////////////////////////////////////////////////////////////////
//公會列表資訊
void NetCli_GuildChild::RL_GuildBaseInfo			( int GuildID , char* GuildName , GuildFlagStruct& Flag , bool IsReady )
{
	g_pGuildFrame->GuildBaseInfo( GuildID, GuildName, Flag, IsReady );
	g_pDebugMessage->DebugOutput( 2 , "公會列表資訊%d=%s %d",GuildID,GuildName,IsReady );

}
//公會刪除
void NetCli_GuildChild::RL_GuildDestroy	( int GuildID  )
{
	//公會倒會了
	const char *GuildName=NULL;
	char temp[1024];
	if (g_pGuildFrame)
	{
		GuildName=g_pGuildFrame->GetGuildNameByID(GuildID);
		sprintf(temp, "公會刪除%s", GuildName);
		if (GuildName)
			g_pGuildFrame->GuildDestroy(GuildID);
	}

	g_pDebugMessage->DebugOutput( 2 ,temp);
}

//公會成員上下線
void NetCli_GuildChild::RD_GuildPlayerOnline		( int PlayerDBID , bool IsOnline )
{

	g_pGuildFrame->GuildPlayerOnline(PlayerDBID,IsOnline);


	
	g_pDebugMessage->DebugOutput( 2 , "RD_GuildPlayerOnline PlayerDBID=%d IsOnline=%d" , PlayerDBID , IsOnline );
}
//------------------------------------------------------------------------------
const char * NetCli_GuildChild::GetGuildNameByID(int guildID)
{

	return  g_pGuildFrame->GetGuildNameByID(guildID);
}

//------------------------------------------------------------------------------

void NetCli_GuildChild::GuildBoardRequest( int guildID )
{
	m_bGuildBoardRequesting=true;
	g_pGuildFrame->GuildBoardRequest(guildID,m_bGuildBoardRequesting);
	g_pDebugMessage->DebugOutput( 2 , "GuildBoardRequest Guild=%d" , guildID );
}




void NetCli_GuildChild::RL_GuildFightAuctionOpen( int TargetNPCID , int CrystalID , int OwnerGuildID , int BitGuildID , int BitScore )
{

}
//------------------------------------------------------------------------------
void NetCli_GuildChild::RL_GuildFightAuctionClose( )
{

}
//------------------------------------------------------------------------------
void NetCli_GuildChild::RD_GuildFightAuctionBidResult( int CrystalID , int OwnerGuildID , int BitGuildID, bool Result )
{

}
//------------------------------------------------------------------------------
void NetCli_GuildChild::RD_GuildFightAuctionBidNewOne( int CrystalID , int OwnerGuildID , int BitGuildID )
{

}
//------------------------------------------------------------------------------
void NetCli_GuildChild::RD_GuildLeaderChange		  ( int NewLeaderDBID , int OldLeaderDBID )
{

	g_pGuildFrame->LeaderChangeMsg(NewLeaderDBID,OldLeaderDBID);
	g_pDebugMessage->DebugOutput( 2 , "更換會長" );	
}
//------------------------------------------------------------------------------
void NetCli_GuildChild::RD_GuildMemberRankChange		( int MemberDBID , int Rank )
{
	g_pGuildFrame->GuildMemberRankChange	(MemberDBID,Rank);
	g_pDebugMessage->DebugOutput( 2 , "更改成員Rank" );	
}
//------------------------------------------------------------------------------
void NetCli_GuildChild::RL_SetGuildReady		( bool Result )
{
	g_pGuildFrame->GuildReadyResult	(Result);
	char Buf[256];
	sprintf( Buf , "連署公會結果 %s " , Result?"ture":"false"	);
	g_pDebugMessage->DebugOutput( 2 , Buf  );	

}
//------------------------------------------------------------------------------
void NetCli_GuildChild::RD_GuildFightInfo( EM_GuildFightInfo_Result emResult, int iCrystalID, int iDefenderGuildID, int iAttackerGuildID, int iHighestAmount, int iRemainTime, int iFightTime, int iOtherCrystalID, int iGuildPoint )
{
	// iDefenderGuildID		目前衛冕者公會, 為 0 表第一次競標
	// iAttackerGuildID		目前出價最高的公會, 或最後一次出價的公會
	// iRemainTime			== 0 時表已超過時間.
	// iFightTime			本週戰鬥時間 60s + 60m * 60 + 24 * 60m + 7 
	//SYSTEMTIME st;
	//GetSystemTime( &st );              // gets current time
	//int iTimeNowVal			= st.wSecond + ( st.wMinute * 60 ) + ( st.wHour * 3600 ) + ( st.wDayOfWeek * 86400 );
	
	//	int					iResult;			//  查詢結果 0 正常, 1 查詢者本身已經下標其它水晶了, 2 查詢者本身已經擁有其它水晶了
	// 要考慮的狀況
	// 超過下標時間, 顯示目前衛冕者 ( 有兩種狀況, 有衛冕者 及無衛冕者 ), 挑戰者, 何時戰鬥
	// 在時間內, 出現下標選項
	// 在時間內, 但公會積分不夠
	// 下標的公會 1 查詢者本身已經下標其它水晶了, 2 查詢者本身已經擁有其它水晶了

	m_iBiddingCrystalID = iCrystalID;
	m_iBiddingScore		= iHighestAmount;

	if( iRemainTime == 0  )
	{
		// 已經過了競標時間
		m_iGuildFightInfoStep = 1;

		vector<string> vecOption;		
		vecOption.push_back( "離開" );
		
		NetCli_Other::This->R_OpenDialog( EM_LuaDialogType_Select, "DAMNTITLE", "TIMEOUT", vecOption, GuildFightBidDialogCallBack, NULL );
	}
	else
	if( iAttackerGuildID == _RoleData.BaseData.GuildID && iAttackerGuildID != 0 )
	{
		// 你的公會目前仍然為最高價
		m_iGuildFightInfoStep = 2;

		vector<string> vecOption;		
		vecOption.push_back( "離開" );
		NetCli_Other::This->R_OpenDialog( EM_LuaDialogType_Select, "DAMNTITLE", "HIGH", vecOption, GuildFightBidDialogCallBack, NULL );
	}
	else
	//if( iAttackerGuildID != _RoleData.BaseData.GuildID && )
	{
		// 你的公會可以出價參加競標
		m_iGuildFightInfoStep = 3;

		vector<string> vecOption;		
		vecOption.push_back( "下標" );
		vecOption.push_back( "離開" );
		NetCli_Other::This->R_OpenDialog( EM_LuaDialogType_Select, "DAMNTITLE", "HIGH", vecOption, GuildFightBidDialogCallBack, NULL  );
		//NetCli_Other::R_DialogSetTitle( )
	}

	//if( )
	//NetCli_Other::This->->R_OpenDialog( EM_LuaDialogType_Select, DialogContent, DialogSelectStr );





}
//------------------------------------------------------------------------------
void NetCli_GuildChild::GuildFightBidDialogCallBack( int iID, PVOID pContext )
{

	switch( m_iGuildFightInfoStep )
	{
	case 1:
	case 2:
		{
			NetCli_Other::This->R_CloseDialog();
		} break;
	case 3:
		{
			if( iID == 0 ) // 下標
			{
				SL_GuildFightAuctionBid( (int)( m_iBiddingScore * 1.5 ), m_iBiddingCrystalID  );
			}

			NetCli_Other::This->R_CloseDialog();
		} break;

	}
}
//------------------------------------------------------------------------------
void NetCli_GuildChild::RD_GuildFightAuctionEnd		( int iCrystalID, int iDefenderGuildID, int iAttackerGuildID, int iFightTime, int iEndAmount )
{
	// 公會戰競標已經結束, 這封包會同時通知 iDefenderGuildID and iAttackerGuildID
	char szMsg[256];
	char szCrystalID[256];
	char szCrystalName[256];
	const char* DefenderGuildName = GetGuildNameByID( iDefenderGuildID );
	const char* AttackerGuildName = GetGuildNameByID( iAttackerGuildID );

	sprintf( szCrystalID, "CRYSTAL_NAME_%02d", iCrystalID );
	sprintf( szCrystalName, g_ObjectData->GetString( szCrystalID ) );
	sprintf( szMsg, g_ObjectData->GetString( "GUILD_MSG_GUILDFIGHT_AUCTIONEND" ), szCrystalName, AttackerGuildName, DefenderGuildName );

	g_pChatFrame->SendMsgEvent("CHAT_MSG_SYSTEM", szMsg, "" );
}
//------------------------------------------------------------------------------
void NetCli_GuildChild::RD_GuildFightBeginNotify		( int iCrystalID, int iRemainTime )
{
	// 公會戰即將於五分鐘內開始, 這封包會同時通知 iDefenderGuildID and iAttackerGuildID, iRemainTime 是正確的秒數
	if ( iRemainTime > 60 )
		g_pChatFrame->SendMsgEvent("CHAT_MSG_SYSTEM", g_ObjectData->GetString( "GUILD_MSG_GUILDFIGHT_BEGINNOTIFY" ), "" );
	else
		g_pChatFrame->SendMsgEvent("CHAT_MSG_SYSTEM", g_ObjectData->GetString( "GUILD_MSG_GUILDFIGHT_BEGINNOTIFY_1" ), "" );
}
//------------------------------------------------------------------------------
void NetCli_GuildChild::RL_GuildFightLeaderInvite( PG_Guild_LtoC_GuildFightLeaderInvite* pPacket )
{
	// 收到系統
	g_pChatFrame->SendMsgEvent("CHAT_MSG_SYSTEM", "[TEMP]收到公會戰會長開始邀請通知" , "" );
}

void NetCli_GuildChild::RL_GuildFightInviteNotify( PG_Guild_LtoC_GuildFightInviteNotify* pPacket )
{
	g_pChatFrame->SendMsgEvent("CHAT_MSG_SYSTEM", "[TEMP]收到邀請通知" , "" );
	SL_GuildFightInviteRespond( pPacket->iSrvID, pPacket->iRoomID, pPacket->iCrystalID, _RoleData.BaseData.GuildID, _RoleData.Base.DBID, true );
}
//某人公會改變
void NetCli_GuildChild::RL_GuildIDChange				( int GItemID , int GuildID )
{
	CRoleSprite* sprite = g_pGameMain->FindSpriteByID( GItemID & 0xffffff);
	if ( sprite ) {
		g_pGameMain->SetSpriteGuildID(sprite, GuildID);
	}	

	/*
	CRoleSprite* player = g_pGameMain->GetPlayer();	
	if ( sprite ) {
		sprite->SetGuildID(GuildID);		
		sprite->SetTitleText(NetCli_GuildChild::GetGuildNameByID(GuildID));

		// PK State 同隊友,同公會不能互打
		int PKState = g_pGameMain->GetPlayerKillState();
		if ( PKState == EM_PK_Party_Guild && player ) {
			if ( player == sprite ) {
				// 自己加入重新設定範圍內玩家
				g_pGameMain->SetPlayerKillState(PKState, false);
			} else if ( GuildID > 0 && sprite->GetGuildID() == player->GetGuildID() ) {
				// 同公會
				sprite->SetEnemyState(false);
				sprite->SetCanAttackState(false);
			} else {
				// 不同公會
				sprite->SetEnemyState(true);
				sprite->SetCanAttackState(true);
			}
		}
	}
	*/
}
static int  GetNowTime()
{
	time_t NowTime;
	time( &NowTime );
	NowTime+=5*60;
	return (int)NowTime;

}
//////////////////////////////////////////////////////////////////////////
//公會自由PK宣戰
void NetCli_GuildChild::RL_DeclareWarResult		( DeclareWarResultENUM Result , int Type , int TargetGuildID )
{
	const char* guildName = GetGuildNameByID(TargetGuildID);
	int time, money, score=0;

	g_ObjectData->GetGuildWarDeclareTypeInfo(Type, time, money );

	switch (Result)
	{
	case EM_DeclareWarResult_OK:
		{
			CRoleSprite* player = g_pGameMain->GetPlayer();
			int BeginTime=GetNowTime();


			char timeStr[256];

			int guildID = player->GetGuildID() & 0xffffff;
			GuildDeclareWarInfo info;
			info.bothID[0] = guildID;
			info.bothID[1] =TargetGuildID & 0xffffff;
			info.startTime = BeginTime;
			info.time =-1;// 35*60;//(float)Time;
			s_declareWarInfo.push_back(info);

			g_pInterface->SendWorldEvent("GUILDWAR_DECLARELIST_UPDATE");

			g_ObjectData->SecondsToTimeAbbrev(time * 60.0f, timeStr);

			g_pChatFrame->SendChatMessageEx("CHAT_MSG_SYSTEM", "", g_ObjectData->GetString("SYS_GUILDDECLARE_SUCCESS"), guildName, timeStr);
		}
		break;

	case EM_DeclareWarResult_DataErr:			//資料錯誤	
	case EM_DeclareWarResult_OnWarErr:			//宣戰中
		g_pChatFrame->SendChatMessage("CHAT_MSG_SYSTEM", "", g_ObjectData->GetString("SYS_GUILDDECLARE_DATA_ERROR"));
		break;
	
	case EM_DeclareWarResult_OnWarPrepareErr:	//宣戰準備中
		g_pChatFrame->SendChatMessage("CHAT_MSG_SYSTEM", "", g_ObjectData->GetString("SYS_GUILDDECLARE_PREPAREERR"));
		break;

	case EM_DeclareWarResult_MonyeErr:			//金錢不足
		g_pChatFrame->SendChatMessage("CHAT_MSG_SYSTEM", "", g_ObjectData->GetString("SYS_GUILDWAR_NOT_ENOUGH_MONEY"));
		break;

	case EM_DeclareWarResult_LeaderErr:			//不是會長
		g_pChatFrame->SendChatMessage("CHAT_MSG_SYSTEM", "", g_ObjectData->GetString("SYS_GUILDWAR_NOT_LEADER"));
		break;
		
	case EM_DeclareWarResult_ScoreErr:			//公會機分不足
		g_pChatFrame->SendChatMessage("CHAT_MSG_SYSTEM", "", g_ObjectData->GetString("SYS_GUILDWAR_NOT_ENOUGH_POINT"));
		break;

	case EM_DeclareWarResult_TimeErr:			//宣戰過快
		g_pChatFrame->SendChatMessageEx("CHAT_MSG_SYSTEM", "", g_ObjectData->GetString("SYS_GUILDDECLARE_TIME_ERROR"));
		break;

	case EM_DeclareWarResult_GuildLvErr:		// 公會等級不足
		g_pChatFrame->SendChatMessageEx("CHAT_MSG_SYSTEM", "", g_ObjectData->GetString("SYS_GUILDDECLARE_LEVEL_ERROR"));
		break;

	case EM_DeclareWarResult_TargetGuildErr:	//目標公會不存在
		g_pChatFrame->SendChatMessageEx("CHAT_MSG_SYSTEM", "", g_ObjectData->GetString("SYS_GUILDDECLARE_TARGET_NOT_EXIST"));
		break;

	case EM_DeclareWarResult_TargetGuildLvErr:	//目標公會等級不足
		g_pChatFrame->SendChatMessageEx("CHAT_MSG_SYSTEM", "", g_ObjectData->GetString("SYS_GUILDDECLARE_TARGET_LEVEL_ERROR"));
		break;
	}
}

void NetCli_GuildChild::RD_PrepareWar					( int GuildID , int Time )
{
	char timeStr[256];
	g_ObjectData->SecondsToTimeAbbrev((float)Time, timeStr);

	const char* guildName = GetGuildNameByID(GuildID & 0xffffff);
	g_pChatFrame->SendChatMessageEx("CHAT_MSG_SYSTEM", "", g_ObjectData->GetString("SYS_GUILDWAR_DECLARE_READY"), guildName, timeStr);
}

void NetCli_GuildChild::RL_WarInfo				( bool IsDeclareWar , int GuildID[2] , int Time , int BeginTime )
{
	GuildDeclareWarInfo info;
	info.bothID[0] = GuildID[0] & 0xffffff;
	info.bothID[1] = GuildID[1] & 0xffffff;
	info.startTime = BeginTime;
	info.time = (float)Time;

	CRoleSprite* player = g_pGameMain->GetPlayer();

	bool flash = false;
	// 成立對戰
	if ( IsDeclareWar )
	{
		for ( vector<GuildDeclareWarInfo>::iterator iter = s_declareWarInfo.begin(); iter != s_declareWarInfo.end(); iter++ )
		{
			if ( memcmp(iter->bothID, info.bothID, sizeof(int)*2) == 0 )
			{
				iter->time = (float)Time;
				g_pInterface->SendWorldEvent("GUILDWAR_DECLARELIST_UPDATE");
				return ;
			}
		}

		// 增加
		s_declareWarInfo.push_back(info);

		// 資訊
		if ( player )
		{
			int guildID = player->GetGuildID();
			if ( GuildID[0] == guildID || GuildID[1] == guildID )
			{
				const char* guildName;
				if ( GuildID[0] == guildID )
					guildName = GetGuildNameByID(GuildID[1]);
				else
					guildName = GetGuildNameByID(GuildID[0]);

				char timeText[256];
				g_ObjectData->SecondsToTimeAbbrev((float)Time, timeText);
				g_pChatFrame->SendChatMessageEx("CHAT_MSG_SYSTEM", "", g_ObjectData->GetString("SYS_GUILDWAR_DECLARE"), guildName, timeText);

				flash = true;
			}			
		}
	}
	// 解除對戰
	else
	{
		for ( vector<GuildDeclareWarInfo>::iterator iter = s_declareWarInfo.begin(); iter != s_declareWarInfo.end(); iter++ )
		{
			if ( memcmp(iter->bothID, info.bothID, sizeof(int)*2) == 0 )
			{
				s_declareWarInfo.erase(iter);
				break;
			}
		}

		// 資訊
		if ( player )
		{
			int guildID = player->GetGuildID();
			if ( GuildID[0] == guildID || GuildID[1] == guildID )
			{
				const char* guildName;
				if ( GuildID[0] == guildID )
					guildName = GetGuildNameByID(GuildID[1]);
				else
					guildName = GetGuildNameByID(GuildID[0]);
				g_pChatFrame->SendChatMessageEx("CHAT_MSG_SYSTEM", "", g_ObjectData->GetString("SYS_GUILDWAR_REMOVE_DECLARE"), guildName);

				flash = true;
			}			
		}
	}

	g_pInterface->SendWorldEvent("GUILDWAR_DECLARELIST_UPDATE");

	// 修正玩家關係
	g_pGameMain->GuildEnemyUpdate();
}

// 公會宣戰關係,初次接收到公會玩家
void NetCli_GuildChild::RL_LoginWarInfo			( int GuildID[2] , int Time , int BeginTime )
{
	GuildDeclareWarInfo info;
	info.bothID[0] = GuildID[0];
	info.bothID[1] = GuildID[1];
	info.startTime = BeginTime;
	info.time = (float)Time;	

	for ( vector<GuildDeclareWarInfo>::iterator iter = s_declareWarInfo.begin(); iter != s_declareWarInfo.end(); iter++ )
	{
		if ( memcmp(iter->bothID, info.bothID, sizeof(int)*2) == 0 )
		{
			// 重複要求
			// assert(false);
			return;
		}
	}

	s_declareWarInfo.push_back(info);
	g_pGameMain->GuildEnemyUpdate();
}

void NetCli_GuildChild::RD_DeclareWarFinalResult		( int GuildID[2] , int Score[2] , int KillCount[2] )
{
	char temp[256];
	int winner[3];
	int loser[3];

	if ( Score[0] > Score[1] )
	{
		winner[0] = GuildID[0];
		winner[1] = Score[0];
		winner[2] = KillCount[0];

		loser[0] = GuildID[1];
		loser[1] = Score[1];
		loser[2] = KillCount[1];
	}
	else
	{
		winner[0] = GuildID[1];
		winner[1] = Score[1];
		winner[2] = KillCount[1];

		loser[0] = GuildID[0];
		loser[1] = Score[0];
		loser[2] = KillCount[0];
	}

	// 處理自己公會
	GuildBaseStruct* info = g_pGuildFrame->GetGuildBaseInfo();
	if ( info ) {
		if ( info->GuildID == winner[0] ) {
			if ( winner[1] == loser[1] ) {
				info->WarEvenCount++;
			} else {
				info->WarWinCount++;
			}
		} else if ( info->GuildID == loser[0] ) {
			if ( winner[1] == loser[1] ) {
				info->WarEvenCount++;
			} else {
				info->WarLostCount++;
			}
		}
	}

	const char* winnerName = g_pGuildFrame->GetGuildNameByID(winner[0]);
	const char* loserName = g_pGuildFrame->GetGuildNameByID(loser[0]);

	if ( winnerName == NULL )
		winnerName = "unknown";
	if ( loserName == NULL )
		loserName = "unknown";

	if ( winner[1] == loser[1] )
		sprintf_s(temp, 256, g_ObjectData->GetString("SYS_GUILDWAR_RESULT_DRAW"), winnerName, loserName, winner[1], loser[1], winner[2], loser[2]);
	else
		sprintf_s(temp, 256, g_ObjectData->GetString("SYS_GUILDWAR_RESULT"), winnerName, loserName, winner[1], loser[1], winner[2], loser[2]);		

	g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", temp);
}

void NetCli_GuildChild::RX_SetGuildFlagInfoResult		( SetGuildFlagInfoResultENUM Result )
{

	switch(Result)
	{
	case EM_SetGuildFlagInfoResult_OK :
		g_pGameMain->SendSystemMsg( g_ObjectData->GetString("SYS_SETGUILDFLAG_OK") );
		break;
	case EM_SetGuildFlagInfoResult_GuildError :
		g_pGameMain->SendSystemMsg( g_ObjectData->GetString("SYS_SETGUILDFLAG_GUILDERROR") );
		break;
	case EM_SetGuildFlagInfoResult_LeaderError :
		g_pGameMain->SendSystemMsg( g_ObjectData->GetString("SYS_SETGUILDFLAG_LEADERERROR") );
		break;
	case EM_SetGuildFlagInfoResult_ScoreError :
		g_pGameMain->SendSystemMsg( g_ObjectData->GetString("SYS_SETGUILDFLAG_SCOREERROR") );
		break;
	}

}

void NetCli_GuildChild::RD_ChangeGuildNameResult		( ChangeGuildNameResultENUM Result )
{
	if (g_pGuildFrame)
		g_pGuildFrame->ChangeGuildNameResult(Result);
}
void NetCli_GuildChild::RD_ChangeGuildName				( int GuildID , char* GuildName )
{
	if (g_pGuildFrame)
		g_pGuildFrame->ChangeGuildName(GuildID,GuildName);

	g_pGameMain->ChangeGuildName(GuildID);
}


void NetCli_GuildChild::RD_GuildUpgradeResult			( int GuildLv , bool Result )
{
	if (g_pGuildFrame)
		g_pGuildFrame->GuildUpgradeResult(GuildLv,Result);
}

void NetCli_GuildChild::RD_SelectContributionLogResult	( int Count , ContributionInfoLogStruct* Log )
{
	if (g_pGuildFrame)
		g_pGuildFrame->SetDayLogTable(Count,Log);

}

void NetCli_GuildChild::RD_GuildBaseInfoUpdate(  )
{

	if (g_pGuildFrame)
		g_pGuildFrame->GuildBaseInfoUpdate();
}

void NetCli_GuildChild::RD_Guild_LevelUp				( int GuildID , int Level )
{
	if (g_pGuildFrame)
		g_pGuildFrame->Guild_LevelUp(GuildID,Level);
}

void NetCli_GuildChild::RD_GuildInfoChange				( GuildInfoChangeStruct& ChangeMode )
{

	if (g_pGuildFrame)
		g_pGuildFrame->GuildInfoChange(ChangeMode);

}

//要求公會戰歷史資料結果
void NetCli_GuildChild::RD_GuildWarHistroyResult( int count , GuildHouseWarHistoryStruct log[] )
{

	if (g_pGuildHousesFrame)
		g_pGuildHousesFrame->GuildWarHistroyResult(count,log);
}
