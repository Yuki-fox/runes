#pragma once

#include "PG/PG_Guild.h"
#include "../../MainProc/NetGlobal.h"

class NetCli_Guild : public CNetGlobal
{
protected:
    //-------------------------------------------------------------------
    static	NetCli_Guild* This;
    static	bool _Init();
    static	bool _Release();	

	
	static void _PG_Guild_LtoC_CreateGuildResult			( int Sockid , int Size , void* Data );
	static void _PG_Guild_LtoC_NewGuildInvite				( int Sockid , int Size , void* Data );
	static void _PG_Guild_LtoC_NewGuildInviteResult			( int Sockid , int Size , void* Data );
	static void _PG_Guild_LtoC_JoindNewGuild				( int Sockid , int Size , void* Data );
															
	static void _PG_Guild_LtoC_LeaveNewGuildResult			( int Sockid , int Size , void* Data );
	static void _PG_Guild_LtoC_DelNewGuildResult			( int Sockid , int Size , void* Data );

	static void _PG_Guild_LtoC_GuildInvite					( int Sockid , int Size , void* Data );
	static void _PG_Guild_LtoC_GuildInviteResult			( int Sockid , int Size , void* Data );
	static void _PG_Guild_LtoC_JoindGuild					( int Sockid , int Size , void* Data );
	static void _PG_Guild_LtoC_LeaveGuildResult				( int Sockid , int Size , void* Data );
	static void _PG_Guild_LtoC_DelGuildResult				( int Sockid , int Size , void* Data );

	static void _PG_Guild_LtoC_GuildInfo					( int Sockid , int Size , void* Data );
	static void _PG_Guild_LtoC_GuildMemberListCount			( int Sockid , int Size , void* Data );
	static void _PG_Guild_LtoC_GuildMemberBaseInfo			( int Sockid , int Size , void* Data );
	static void _PG_Guild_LtoC_GuildMemberInfo				( int Sockid , int Size , void* Data );
	static void _PG_Guild_DtoC_GuildCreate					( int Sockid , int Size , void* Data );
	static void _PG_Guild_DtoX_ModifySimpleGuild			( int Sockid , int Size , void* Data );


	static void _PG_Guild_DtoC_BoardPostResult				( int Sockid , int Size , void* Data );	
	static void _PG_Guild_DtoC_BoardListCount				( int Sockid , int Size , void* Data );	
	static void _PG_Guild_DtoC_BoardListData				( int Sockid , int Size , void* Data );	
	static void _PG_Guild_DtoC_BoardMessageResult			( int Sockid , int Size , void* Data );	
	static void _PG_Guild_DtoC_BoardModifyMessageResult		( int Sockid , int Size , void* Data );	
	static void _PG_Guild_DtoC_BoardModifyModeResult		( int Sockid , int Size , void* Data );	

	static void _PG_Guild_LtoC_GuildBaseInfo				( int Sockid , int Size , void* Data );	
	static void _PG_Guild_LtoC_GuildDestroy					( int Sockid , int Size , void* Data );	

	static void _PG_Guild_DtoC_JoinGuildMember				( int Sockid , int Size , void* Data );	
	static void _PG_Guild_DtoC_LeaveGuildMember				( int Sockid , int Size , void* Data );	

	static void _PG_Guild_LtoC_OpenCreateGuild				( int Sockid , int Size , void* Data );	
	static void _PG_Guild_LtoC_CloseCreateGuild				( int Sockid , int Size , void* Data );	
	static void _PG_Guild_LtoC_OpenGuildContribution		( int Sockid , int Size , void* Data );	
	static void _PG_Guild_LtoC_GuildContributionItemResult	( int Sockid , int Size , void* Data );	
	static void _PG_Guild_LtoC_GuildContributionClose		( int Sockid , int Size , void* Data );	

//	static void _PG_Guild_LtoC_GuildShopOpen				( int Sockid , int Size , void* Data );
//	static void _PG_Guild_DtoC_GuildShopBuyResult			( int Sockid , int Size , void* Data );
//	static void _PG_Guild_LtoC_GuildShopClose				( int Sockid , int Size , void* Data );

	static void _PG_Guild_LtoC_AllGuildMemberCountResult	( int Sockid , int Size , void* Data );

	//公會戰競標
	static void _PG_Guild_LtoC_GuildFightAuctionOpen		( int Sockid , int Size , void* Data );
	static void _PG_Guild_LtoC_GuildFightAuctionClose		( int Sockid , int Size , void* Data );
	static void _PG_Guild_DtoC_GuildFightAuctionBidResult	( int Sockid , int Size , void* Data );
	static void _PG_Guild_DtoC_GuildFightAuctionBidNewOne	( int Sockid , int Size , void* Data );
	static void _PG_Guild_DtoL_GuildPlayerOnline			( int Sockid , int Size , void* Data );

	//會長更換
	static void _PG_Guild_DtoC_GuildLeaderChange			( int Sockid , int Size , void* Data );
	static void _PG_Guild_DtoC_GuildMemberRankChange		( int Sockid , int Size , void* Data );

	static void _PG_Guild_LtoC_SetGuildReady				( int Sockid , int Size , void* Data );

	static void _PG_Guild_DtoC_GuildFightInfo				( int Sockid , int Size , void* Data );
	static void _PG_Guild_DtoC_GuildFightAuctionEnd			( int Sockid , int Size , void* Data );

	static void _PG_Guild_DtoC_GuildFightBeginNotify		( int Sockid , int Size , void* Data );
	static void _PG_Guild_LtoC_GuildFightLeaderInvite		( int Sockid , int Size , void* Data );
	static void _PG_Guild_LtoC_GuildFightInviteNotify		( int Sockid , int Size , void* Data );
	
	static void _PG_Guild_LtoC_GuildIDChange				( int Sockid , int Size , void* Data );
		
	static void _PG_Guild_LtoC_DeclareWarResult				( int Sockid , int Size , void* Data );
	static void _PG_Guild_LtoC_WarInfo						( int Sockid , int Size , void* Data );
	static void _PG_Guild_LtoC_LoginWarInfo					( int Sockid , int Size , void* Data );
	static void _PG_Guild_DtoC_DeclareWarFinalResult		( int Sockid , int Size , void* Data );
	static void _PG_Guild_XtoC_SetGuildFlagInfoResult		( int Sockid , int Size , void* Data );

	static void _PG_Guild_DtoC_ChangeGuildNameResult		( int Sockid , int Size , void* Data );
	static void _PG_Guild_DtoC_ChangeGuildName				( int Sockid , int Size , void* Data );

	static void _PG_Guild_DtoC_PrepareWar					( int Sockid , int Size , void* Data );

	static void _PG_Guild_DtoC_GuildUpgradeResult			( int Sockid , int Size , void* Data );
	static void _PG_Guild_DtoC_SelectContributionLogResult	( int Sockid , int Size , void* Data );
	static void _PG_Guild_DtoC_GuildBaseInfoUpdate			( int Sockid , int Size , void* Data );

	static void _PG_Guild_DtoC_Guild_LevelUp				( int Sockid , int Size , void* Data );
	static void _PG_Guild_DtoC_GuildInfoChange				( int Sockid , int Size , void* Data );
	static void _PG_Guild_DtoC_GuildWarHistroyResult		( int Sockid , int Size , void* Data );
public:
	static int	_GuildID;
	static int	_MemberCount;
	static vector< GuildMemberBaseInfoStruct > _MemberList;	
public:    
    NetCli_Guild() { This = this; }

	static	NetCli_Guild*	GetThis()		{ return This; }
	//////////////////////////////////////////////////////////////////////////
	

	//////////////////////////////////////////////////////////////////////////
	// 連署
	//////////////////////////////////////////////////////////////////////////
	static void SL_CreateGuild				( char* GuildName );
	static void SL_NewGuildInvite			( char* PlayerName );
	static void SL_NewGuildInviteResult		( int GuildID , int LeaderDBID , bool Result );
	static void SL_LeaveNewGuild			( char* KickPlayerName );
	static void SL_DelNewGuild				( );

	//通知Server Client 已關閉建立公會的介面
	static void SL_CloseCreateGuild			( );
	//////////////////////////////////////////////////////////////////////////
	// 正式公會
	//////////////////////////////////////////////////////////////////////////
	static void SL_GuildInvite				( char* PlayerName );
	static void SL_GuildInviteResult		( int GuildID , int LeaderDBID , bool Result );
	static void SL_LeaveGuild				( char* KickPlayerName );
	static void SL_DelGuild					( );

	static void SL_ModifyGuildInfoRequest		( GuildBaseStruct& Guild );
	static void SL_ModifyGuildMemberInfoRequest	( GuildMemberStruct& Member );
	static void SL_GuildInfoRequet				( int GuildID );
	static void SL_GuildMemberListRequet		( int GuildID );
	static void SL_GuildMemberInfoRequet		( int MemberDBID );

	//要求所有公會的成員數
	static void SL_AllGuildMemberCountRequest	( );
	
	//////////////////////////////////////////////////////////////////////////
	// 留言板 
	//////////////////////////////////////////////////////////////////////////
	static void SL_BoardPostRequest				( const char* Title , const char* Message );
	static void SL_BoardListRequest				( int PageID );
	static void SL_BoardMessageRequest			( int MessageGUID );
	static void SL_BoardModifyMessageRequest	( int MessageGUID , const char* Message );
	static void SL_BoardModifyModeRequest		( int MessageGUID  , GuildBoardModeStruct	Mode );	

	//////////////////////////////////////////////////////////////////////////
	// 貢獻
	//////////////////////////////////////////////////////////////////////////
	//放置要貢獻的物品
	static void SL_GuildContributionItem	( int Pos , ItemFieldStruct& Item , bool IsPut );
	//確定要貢獻
	static void SL_GuildContributionOK		( int Money , int Bonus_Money );
	//關閉貢獻介面
	static void SL_GuildContributionClose	( );
	//////////////////////////////////////////////////////////////////////////
	// 商店  12/5 fuming 
	//////////////////////////////////////////////////////////////////////////
 
//	static void SL_GuildShopBuy	(int flagid );
	//關閉商店介面
//	static void SL_GuildShopClose	( );
	
	//////////////////////////////////////////////////////////////////////////
	//及時資訊
	//////////////////////////////////////////////////////////////////////////
	//公會列表資訊
	virtual void RL_GuildBaseInfo			( int GuildID , char* GuildName , GuildFlagStruct& Flag ,bool IsReady ) = 0;
	//公會刪除
	virtual void RL_GuildDestroy			( int GuildID  ) = 0;

	// 玩家加入自己的公會
	virtual void RD_JoinGuildMember			( GuildMemberStruct& Member ) = 0;
	// 玩家離開自己的公會
	virtual void RD_LeaveGuildMember		( int LeaderDBID , int MemberDBID ) = 0;

	virtual void RD_GuildPlayerOnline		( int PlayerDBID , bool IsOnline ) = 0;
	//////////////////////////////////////////////////////////////////////////
	virtual void RL_CreateGuildResult		( int GuildID , char* GuildName , CreateGuildResultENUM Result ) = 0;
	virtual void RL_NewGuildInvite			( int GuildID , char* LeaderName , int LeaderDBID ) = 0;
	virtual void RL_NewGuildInviteResult	( char* PlayerName , NewGuildInviteResultENUM	Result ) = 0;
	virtual void RL_JoindNewGuild			( int GuildID , char* LeaderName ) = 0;
	virtual void RL_LeaveNewGuildResult		( char* KickPlayerName , bool Result ) = 0;
	virtual void RL_DelNewGuildResult		( bool Result ) = 0;

	virtual void RL_GuildInvite				( int GuildID , char* LeaderName , int LeaderDBID ) = 0;
	virtual void RL_GuildInviteResult		( char* PlayerName , GuildInviteResultENUM	Result ) = 0;
	virtual void RL_JoindGuild				( int GuildID , char* LeaderName ) = 0;
	virtual void RL_LeaveGuildResult		( char* KickPlayerName , bool Result ) = 0;
	virtual void RL_DelGuildResult			( bool Result ) = 0;


	virtual void RL_GuildInfo			( GuildBaseStruct& Guild , int MemberCount ) = 0;
	virtual void RL_GuildMemberBaseInfo	( vector< GuildMemberBaseInfoStruct >& MemberList ) = 0;
	virtual void RL_GuildMemberInfo		( GuildMemberStruct& Member ) = 0;
	virtual void RD_GuildCreate			( int GuildID ) = 0;
	virtual void RD_ModifySimpleGuild	( SimpleGuildInfoStruct& SimpleGuildInfo ) = 0;

	virtual void RL_SetGuildReady		( bool Result ) = 0;
	
	//通知公會人數
	virtual void RL_AllGuildMemberCountResult( int Size , GuildMemberCountStruct MemberList[ DEF_MAX_PG_GUILD_MEMBER_COUNT_ ] ) = 0;
	//////////////////////////////////////////////////////////////////////////
	// 留言板 
	//////////////////////////////////////////////////////////////////////////
	virtual void RD_BoardPostResult			( bool Result ) =0;
	virtual void RD_BoardListCount			( int MaxCount , int Count , int PageID ) =0;
	virtual void RD_BoardListData			( int MessageGUID , int Index , char* PlayerName , int Time , char* Title , GuildBoardModeStruct Type ) =0;
	virtual void RD_BoardMessageResult		( GuildBoardStruct&	Message ) =0;
	virtual void RD_BoardModifyMessageResult( bool Result ) =0;
	virtual void RD_BoardModifyModeResult	( bool Result ) =0;

	//////////////////////////////////////////////////////////////////////////
	// 貢獻物品 
	//////////////////////////////////////////////////////////////////////////
	virtual void RL_OpenCreateGuild				( int TargetNPCID) = 0;
	virtual void RL_CloseCreateGuild			( )	= 0;
	virtual void RL_OpenGuildContribution		( int TargetNPCID) = 0;	
	virtual void RL_GuildContributionItemResult	( bool Result ) = 0;
	virtual void RL_GuildContributionClose		( ) = 0;

//	virtual void RL_GuildShopOpen( int TargetNPCID , int Score , StaticFlag<256>& Flag ) = 0;
//	virtual void RD_GuildShopBuyResult( int FlagID , bool Result ) = 0;
//	virtual void RL_GuildShopClose( ) = 0;

	//////////////////////////////////////////////////////////////////////////
	//公會競標
	//////////////////////////////////////////////////////////////////////////
	virtual void RL_GuildFightAuctionOpen( int TargetNPCID , int CrystalID , int OwnerGuildID , int BitGuildID , int BitScore ) = 0;
	virtual void RL_GuildFightAuctionClose( ) = 0;
	virtual void RD_GuildFightAuctionBidResult( int CrystalID , int OwnerGuildID , int BidGuildID, bool Result ) = 0;
	virtual void RD_GuildFightAuctionBidNewOne( int CrystalID , int OwnerGuildID , int BidGuildID ) = 0;

	virtual void RD_GuildFightInfo				( EM_GuildFightInfo_Result emResult, int iCrystalID, int iDefenderGuildID, int iAttackerGuildID, int iHighestAmount, int iRemainTime, int iFightTime, int iOtherCrystalID, int iGuildPoint ) = 0;
		// 收到 DataCenter 送來的資訊, 開啟介面讓 Client 下柱

	virtual void RD_GuildFightAuctionEnd		( int iCrystalID, int iDefenderGuildID, int iAttackerGuildID, int iFightTime, int iEndAmount ) = 0;

	// 公會戰
	virtual void RD_GuildFightBeginNotify		( int iCrystalID, int iRemainTime ) = 0;

	//////////////////////////////////////////////////////////////////////////
	//公會戰競標, 及
	//////////////////////////////////////////////////////////////////////////
	static void SL_GuildFightAuctionClose		( );
	static void SL_GuildFightAuctionBid			( int BitScore, int iCrystalID );

	virtual void RL_GuildFightLeaderInvite		( PG_Guild_LtoC_GuildFightLeaderInvite* pPacket ) = 0;
	//其他
	virtual void RD_GuildLeaderChange			( int NewLeaderDBID , int OldLeaderDBID ) = 0;
	virtual void RD_GuildMemberRankChange		( int MemberDBID , int Rank ) = 0;

	virtual	void RL_GuildFightInviteNotify		( PG_Guild_LtoC_GuildFightInviteNotify* pPacket ) = 0;

	static	void SL_GuildFightRequestInvite		( int iSrvID, int iRoomID, int iCrystalID, int iGuildID, int iMemberDBID );
	static	void SL_GuildFightInviteRespond		( int iSrvID, int iRoomID, int iCrystalID, int iGuildID, int iMemberDBID, bool bJoin );

	//某人公會改變
	virtual void RL_GuildIDChange				( int GItemID , int GuildID ) = 0;

	//////////////////////////////////////////////////////////////////////////
	//公會自由PK宣戰
	static	void SL_DeclareWar					( int Type , int TargetGuildID );

	virtual void RL_DeclareWarResult			( DeclareWarResultENUM Result , int Type , int TargetGuildID ) = 0;
	virtual void RL_WarInfo						( bool IsDeclareWar , int GuildID[2] , int Time , int BeginTime ) = 0;
	virtual void RL_LoginWarInfo				( int GuildID[2] , int Time , int BeginTime ) = 0;
	virtual void RD_DeclareWarFinalResult		( int GuildID[2] , int Score[2] , int KillCount[2] ) = 0;


	static void  SL_SetGuildFlagInfoRequest		( GuildFlagStruct&	Flag );
	virtual void RX_SetGuildFlagInfoResult		( SetGuildFlagInfoResultENUM Result ) = 0;
	virtual void RD_PrepareWar					( int GuildID , int Time ) = 0;

	//////////////////////////////////////////////////////////////////////////
	//公會改名
	//////////////////////////////////////////////////////////////////////////
	static  void SL_ChangeGuildNameRequest		( const char* GuildName );
	virtual void RD_ChangeGuildNameResult		( ChangeGuildNameResultENUM Result ) = 0;
	virtual void RD_ChangeGuildName				( int GuildID , char* GuildName ) = 0;

	//////////////////////////////////////////////////////////////////////////
	//公會升級
	//////////////////////////////////////////////////////////////////////////
	static void  SL_GuildUpgradeRequest			( int GuildLv );
	virtual void RD_GuildUpgradeResult			( int GuildLv , bool Result ) = 0;

	static void  SL_SelectContributionLog		( int DayBefore );
	virtual void RD_SelectContributionLogResult	( int Count , ContributionInfoLogStruct* Log ) = 0;

	virtual void RD_Guild_LevelUp				( int GuildID , int Level ) = 0;
	//////////////////////////////////////////////////////////////////////////
	//通知公會基本資料有修改
	//////////////////////////////////////////////////////////////////////////
	virtual void RD_GuildBaseInfoUpdate			(  ) = 0;

	virtual void RD_GuildInfoChange				( GuildInfoChangeStruct& ChangeMode ) = 0;
	//////////////////////////////////////////////////////////////////////////
	//要求公會戰歷史資料
	static void SL_GuildWarHistroy(  );
	//要求公會戰歷史資料結果
	virtual void RD_GuildWarHistroyResult( int count , GuildHouseWarHistoryStruct log[] ) = 0;
};

