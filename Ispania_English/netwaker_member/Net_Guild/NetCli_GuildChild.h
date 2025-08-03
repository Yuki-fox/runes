#pragma once

#include "NetCli_Guild.h"

// 公會宣戰結構
struct GuildDeclareWarInfo
{
	int bothID[2];			// 雙方公會編號	
	float time;				// 剩於對戰時間
	int startTime;			// 開始時間
};

class NetCli_GuildChild : public NetCli_Guild
{
public:	
	static	bool _Init();
	static	bool _Release();
	static	int								m_iBiddingCrystalID;
	static	int								m_iGuildFightInfoStep;
	static	int								m_iBiddingScore;	
	
	//取得公會名稱
	static const char *                           GetGuildNameByID(int guildID);
	
		

		


	//公會告示版ui					
 	static void								GuildBoardRequest( int guildID );
	
	static bool								m_bGuildBoardRequesting;
	static vector<GuildDeclareWarInfo>		s_declareWarInfo;
public:
	//////////////////////////////////////////////////////////////////////////
	//及時資訊
	//////////////////////////////////////////////////////////////////////////
	//公會列表資訊
	virtual void RL_GuildBaseInfo			( int GuildID , char* GuildName , GuildFlagStruct& Flag , bool IsReady );
	//公會刪除
	virtual void RL_GuildDestroy			( int GuildID  );
	// 玩家加入自己的公會
	virtual void RD_JoinGuildMember			( GuildMemberStruct& Member );
	// 玩家離開自己的公會
	virtual void RD_LeaveGuildMember		( int LeaderDBID , int MemberDBID );
	//公會成員上下線
	virtual void RD_GuildPlayerOnline		( int PlayerDBID , bool IsOnline );
	
	//////////////////////////////////////////////////////////////////////////
	// 連署
	//////////////////////////////////////////////////////////////////////////
	virtual void RL_OpenCreateGuild				( int TargetNPCID);
	virtual void RL_CloseCreateGuild			( );

	virtual void RL_CreateGuildResult		( int GuildID , char* GuildName , CreateGuildResultENUM Result );
	virtual void RL_NewGuildInvite			( int GuildID , char* LeaderName ,int LeaderDBID );
	virtual void RL_NewGuildInviteResult	( char* PlayerName , NewGuildInviteResultENUM	Result );
	virtual void RL_JoindNewGuild			( int GuildID , char* LeaderName );
	virtual void RL_LeaveNewGuildResult		( char* KickPlayerName , bool Result );
	virtual void RL_DelNewGuildResult		( bool Result );
	//////////////////////////////////////////////////////////////////////////
	// 正式公會
	//////////////////////////////////////////////////////////////////////////
	virtual void RL_GuildInvite				( int GuildID , char* LeaderName , int LeaderDBID );
	virtual void RL_GuildInviteResult		( char* PlayerName , GuildInviteResultENUM	Result );
	virtual void RL_JoindGuild				( int GuildID , char* IntiveName );
	virtual void RL_LeaveGuildResult		( char* KickPlayerName , bool Result );
	virtual void RL_DelGuildResult			( bool Result );
	//////////////////////////////////////////////////////////////////////////
	virtual void RL_GuildInfo			( GuildBaseStruct& Guild , int MemberCount );
	virtual void RL_GuildMemberBaseInfo	( vector< GuildMemberBaseInfoStruct >& MemberList );
	virtual void RL_GuildMemberInfo		( GuildMemberStruct& Member );
	virtual void RD_GuildCreate			( int GuildID );
	virtual void RL_SetGuildReady		( bool Result );
	virtual void RD_ModifySimpleGuild	( SimpleGuildInfoStruct& SimpleGuildInfo );
	//通知公會人數
	virtual void RL_AllGuildMemberCountResult( int Size , GuildMemberCountStruct MemberList[ DEF_MAX_PG_GUILD_MEMBER_COUNT_ ] );//{};
	//////////////////////////////////////////////////////////////////////////
	// 留言板 
	//////////////////////////////////////////////////////////////////////////
	virtual void RD_BoardPostResult			( bool Result );
	virtual void RD_BoardListCount			( int MaxCount , int Count , int PageID );
	virtual void RD_BoardListData			( int MessageGUID , int Index , char* PlayerName , int Time , char* Title , GuildBoardModeStruct Type );
	virtual void RD_BoardMessageResult		( GuildBoardStruct&	Message );
	virtual void RD_BoardModifyMessageResult( bool Result );
	virtual void RD_BoardModifyModeResult	( bool Result );

	//////////////////////////////////////////////////////////////////////////
	// 貢獻物品 
	//////////////////////////////////////////////////////////////////////////
	virtual void RL_OpenGuildContribution		( int TargetNPCID);	
	virtual void RL_GuildContributionItemResult	( bool Result );
	virtual void RL_GuildContributionClose		( );

	//////////////////////////////////////////////////////////////////////////
	// 公會商店
	//////////////////////////////////////////////////////////////////////////
//	virtual void RL_GuildShopOpen( int TargetNPCID , int Score , StaticFlag<256>& Flag );
//	virtual void RD_GuildShopBuyResult( int FlagID , bool Result );
//	virtual void RL_GuildShopClose( );

	//////////////////////////////////////////////////////////////////////////
	//公會競標
	//////////////////////////////////////////////////////////////////////////
	virtual void RL_GuildFightAuctionOpen		( int TargetNPCID , int CrystalID , int OwnerGuildID , int BitGuildID , int BitScore );
	virtual void RL_GuildFightAuctionClose		();
	virtual void RD_GuildFightAuctionBidResult	( int CrystalID , int OwnerGuildID , int BidGuildID, bool Result );
	virtual void RD_GuildFightAuctionBidNewOne	( int CrystalID , int OwnerGuildID , int BidGuildID );
	virtual void RD_GuildFightInfo				( EM_GuildFightInfo_Result emResult, int iCrystalID, int iDefenderGuildID, int iAttackerGuildID, int iHighestAmount, int iRemainTime, int iFightTime, int iOtherCrystalID, int iGuildPoint );
	virtual void RD_GuildFightAuctionEnd		( int iCrystalID, int iDefenderGuildID, int iAttackerGuildID, int iFightTime, int iEndAmount );
	virtual void RD_GuildFightBeginNotify		( int iCrystalID, int iRemainTime );
	//其他
	virtual void RD_GuildLeaderChange			( int NewLeaderDBID , int OldLeaderDBID );
	virtual void RD_GuildMemberRankChange		( int MemberDBID , int Rank );
	virtual void RL_GuildFightLeaderInvite		( PG_Guild_LtoC_GuildFightLeaderInvite* pPacket );
	virtual void RL_GuildFightInviteNotify		( PG_Guild_LtoC_GuildFightInviteNotify* pPacket );

	//某人公會改變
	virtual void RL_GuildIDChange				( int GItemID , int GuildID );

	static	void GuildFightBidDialogCallBack	( int iID, PVOID pContext );

	//////////////////////////////////////////////////////////////////////////
	//公會自由PK宣戰
	virtual void RL_DeclareWarResult			( DeclareWarResultENUM Result , int Type , int TargetGuildID );
	virtual void RL_WarInfo						( bool IsDeclareWar , int GuildID[2] , int Time , int BeginTime );
	virtual void RL_LoginWarInfo				( int GuildID[2] , int Time , int BeginTime );
	virtual void RD_DeclareWarFinalResult		( int GuildID[2] , int Score[2] , int KillCount[2] );

	virtual void RX_SetGuildFlagInfoResult		( SetGuildFlagInfoResultENUM Result );
	virtual void RD_PrepareWar					( int GuildID , int Time );
	//////////////////////////////////////////////////////////////////////////
	//公會改名
	//////////////////////////////////////////////////////////////////////////
	virtual void RD_ChangeGuildNameResult		( ChangeGuildNameResultENUM Result );
	virtual void RD_ChangeGuildName				( int GuildID , char* GuildName );

	//////////////////////////////////////////////////////////////////////////
	//公會升級
	//////////////////////////////////////////////////////////////////////////
	virtual void RD_GuildUpgradeResult			( int GuildLv , bool Result );
	virtual void RD_SelectContributionLogResult	( int Count , ContributionInfoLogStruct* Log );
	virtual void RD_Guild_LevelUp				( int GuildID , int Level );

	virtual void RD_GuildBaseInfoUpdate(  );
	virtual void RD_GuildInfoChange				( GuildInfoChangeStruct& ChangeMode );

	//要求公會戰歷史資料結果
	virtual void RD_GuildWarHistroyResult( int count , GuildHouseWarHistoryStruct log[] );
};



