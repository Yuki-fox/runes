#pragma once

#include "NetCli_Guild.h"

// ���|�žԵ��c
struct GuildDeclareWarInfo
{
	int bothID[2];			// ���褽�|�s��	
	float time;				// �ѩ��Ԯɶ�
	int startTime;			// �}�l�ɶ�
};

class NetCli_GuildChild : public NetCli_Guild
{
public:	
	static	bool _Init();
	static	bool _Release();
	static	int								m_iBiddingCrystalID;
	static	int								m_iGuildFightInfoStep;
	static	int								m_iBiddingScore;	
	
	//���o���|�W��
	static const char *                           GetGuildNameByID(int guildID);
	
		

		


	//���|�i�ܪ�ui					
 	static void								GuildBoardRequest( int guildID );
	
	static bool								m_bGuildBoardRequesting;
	static vector<GuildDeclareWarInfo>		s_declareWarInfo;
public:
	//////////////////////////////////////////////////////////////////////////
	//�ήɸ�T
	//////////////////////////////////////////////////////////////////////////
	//���|�C���T
	virtual void RL_GuildBaseInfo			( int GuildID , char* GuildName , GuildFlagStruct& Flag , bool IsReady );
	//���|�R��
	virtual void RL_GuildDestroy			( int GuildID  );
	// ���a�[�J�ۤv�����|
	virtual void RD_JoinGuildMember			( GuildMemberStruct& Member );
	// ���a���}�ۤv�����|
	virtual void RD_LeaveGuildMember		( int LeaderDBID , int MemberDBID );
	//���|�����W�U�u
	virtual void RD_GuildPlayerOnline		( int PlayerDBID , bool IsOnline );
	
	//////////////////////////////////////////////////////////////////////////
	// �s�p
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
	// �������|
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
	//�q�����|�H��
	virtual void RL_AllGuildMemberCountResult( int Size , GuildMemberCountStruct MemberList[ DEF_MAX_PG_GUILD_MEMBER_COUNT_ ] );//{};
	//////////////////////////////////////////////////////////////////////////
	// �d���O 
	//////////////////////////////////////////////////////////////////////////
	virtual void RD_BoardPostResult			( bool Result );
	virtual void RD_BoardListCount			( int MaxCount , int Count , int PageID );
	virtual void RD_BoardListData			( int MessageGUID , int Index , char* PlayerName , int Time , char* Title , GuildBoardModeStruct Type );
	virtual void RD_BoardMessageResult		( GuildBoardStruct&	Message );
	virtual void RD_BoardModifyMessageResult( bool Result );
	virtual void RD_BoardModifyModeResult	( bool Result );

	//////////////////////////////////////////////////////////////////////////
	// �^�m���~ 
	//////////////////////////////////////////////////////////////////////////
	virtual void RL_OpenGuildContribution		( int TargetNPCID);	
	virtual void RL_GuildContributionItemResult	( bool Result );
	virtual void RL_GuildContributionClose		( );

	//////////////////////////////////////////////////////////////////////////
	// ���|�ө�
	//////////////////////////////////////////////////////////////////////////
//	virtual void RL_GuildShopOpen( int TargetNPCID , int Score , StaticFlag<256>& Flag );
//	virtual void RD_GuildShopBuyResult( int FlagID , bool Result );
//	virtual void RL_GuildShopClose( );

	//////////////////////////////////////////////////////////////////////////
	//���|�v��
	//////////////////////////////////////////////////////////////////////////
	virtual void RL_GuildFightAuctionOpen		( int TargetNPCID , int CrystalID , int OwnerGuildID , int BitGuildID , int BitScore );
	virtual void RL_GuildFightAuctionClose		();
	virtual void RD_GuildFightAuctionBidResult	( int CrystalID , int OwnerGuildID , int BidGuildID, bool Result );
	virtual void RD_GuildFightAuctionBidNewOne	( int CrystalID , int OwnerGuildID , int BidGuildID );
	virtual void RD_GuildFightInfo				( EM_GuildFightInfo_Result emResult, int iCrystalID, int iDefenderGuildID, int iAttackerGuildID, int iHighestAmount, int iRemainTime, int iFightTime, int iOtherCrystalID, int iGuildPoint );
	virtual void RD_GuildFightAuctionEnd		( int iCrystalID, int iDefenderGuildID, int iAttackerGuildID, int iFightTime, int iEndAmount );
	virtual void RD_GuildFightBeginNotify		( int iCrystalID, int iRemainTime );
	//��L
	virtual void RD_GuildLeaderChange			( int NewLeaderDBID , int OldLeaderDBID );
	virtual void RD_GuildMemberRankChange		( int MemberDBID , int Rank );
	virtual void RL_GuildFightLeaderInvite		( PG_Guild_LtoC_GuildFightLeaderInvite* pPacket );
	virtual void RL_GuildFightInviteNotify		( PG_Guild_LtoC_GuildFightInviteNotify* pPacket );

	//�Y�H���|����
	virtual void RL_GuildIDChange				( int GItemID , int GuildID );

	static	void GuildFightBidDialogCallBack	( int iID, PVOID pContext );

	//////////////////////////////////////////////////////////////////////////
	//���|�ۥ�PK�ž�
	virtual void RL_DeclareWarResult			( DeclareWarResultENUM Result , int Type , int TargetGuildID );
	virtual void RL_WarInfo						( bool IsDeclareWar , int GuildID[2] , int Time , int BeginTime );
	virtual void RL_LoginWarInfo				( int GuildID[2] , int Time , int BeginTime );
	virtual void RD_DeclareWarFinalResult		( int GuildID[2] , int Score[2] , int KillCount[2] );

	virtual void RX_SetGuildFlagInfoResult		( SetGuildFlagInfoResultENUM Result );
	virtual void RD_PrepareWar					( int GuildID , int Time );
	//////////////////////////////////////////////////////////////////////////
	//���|��W
	//////////////////////////////////////////////////////////////////////////
	virtual void RD_ChangeGuildNameResult		( ChangeGuildNameResultENUM Result );
	virtual void RD_ChangeGuildName				( int GuildID , char* GuildName );

	//////////////////////////////////////////////////////////////////////////
	//���|�ɯ�
	//////////////////////////////////////////////////////////////////////////
	virtual void RD_GuildUpgradeResult			( int GuildLv , bool Result );
	virtual void RD_SelectContributionLogResult	( int Count , ContributionInfoLogStruct* Log );
	virtual void RD_Guild_LevelUp				( int GuildID , int Level );

	virtual void RD_GuildBaseInfoUpdate(  );
	virtual void RD_GuildInfoChange				( GuildInfoChangeStruct& ChangeMode );

	//�n�D���|�Ծ��v��Ƶ��G
	virtual void RD_GuildWarHistroyResult( int count , GuildHouseWarHistoryStruct log[] );
};



