#include "NetCli_Guild.h"

//-------------------------------------------------------------------
NetCli_Guild*    NetCli_Guild::This         = NULL;
int									NetCli_Guild::_GuildID = 0;
int									NetCli_Guild::_MemberCount = 0;
vector< GuildMemberBaseInfoStruct > NetCli_Guild::_MemberList;
//-------------------------------------------------------------------
bool NetCli_Guild::_Init()
{
	Cli_NetReg( PG_Guild_LtoC_CreateGuildResult		);
	Cli_NetReg( PG_Guild_LtoC_NewGuildInvite		);	
	Cli_NetReg( PG_Guild_LtoC_NewGuildInviteResult	);
	Cli_NetReg( PG_Guild_LtoC_JoindNewGuild			);
	Cli_NetReg( PG_Guild_LtoC_LeaveNewGuildResult	);
	Cli_NetReg( PG_Guild_LtoC_DelNewGuildResult		);

	Cli_NetReg( PG_Guild_LtoC_GuildInvite			);	
	Cli_NetReg( PG_Guild_LtoC_GuildInviteResult		);
	Cli_NetReg( PG_Guild_LtoC_JoindGuild			);
	Cli_NetReg( PG_Guild_LtoC_LeaveGuildResult		);
	Cli_NetReg( PG_Guild_LtoC_DelGuildResult		);

	Cli_NetReg( PG_Guild_LtoC_GuildInfo				);	
	Cli_NetReg( PG_Guild_LtoC_GuildMemberListCount	);
	Cli_NetReg( PG_Guild_LtoC_GuildMemberBaseInfo	);
	Cli_NetReg( PG_Guild_LtoC_GuildMemberInfo		);
	Cli_NetReg( PG_Guild_DtoC_GuildCreate			);

	Cli_NetReg( PG_Guild_DtoC_BoardPostResult		);	
	Cli_NetReg( PG_Guild_DtoC_BoardListCount		);
	Cli_NetReg( PG_Guild_DtoC_BoardListData			);
	Cli_NetReg( PG_Guild_DtoC_BoardMessageResult	);
	Cli_NetReg( PG_Guild_DtoC_BoardModifyMessageResult	);
	Cli_NetReg( PG_Guild_DtoC_BoardModifyModeResult	);

	Cli_NetReg( PG_Guild_LtoC_GuildBaseInfo	);
	Cli_NetReg( PG_Guild_LtoC_GuildDestroy	);

	Cli_NetReg( PG_Guild_DtoC_JoinGuildMember	);
	Cli_NetReg( PG_Guild_DtoC_LeaveGuildMember	);


	Cli_NetReg( PG_Guild_LtoC_OpenCreateGuild				);
	Cli_NetReg( PG_Guild_LtoC_CloseCreateGuild				);
	Cli_NetReg( PG_Guild_LtoC_OpenGuildContribution			);
	Cli_NetReg( PG_Guild_LtoC_GuildContributionItemResult	);
	Cli_NetReg( PG_Guild_LtoC_GuildContributionClose		);

//	Cli_NetReg( PG_Guild_LtoC_GuildShopOpen			);
//	Cli_NetReg( PG_Guild_DtoC_GuildShopBuyResult	);
//	Cli_NetReg( PG_Guild_LtoC_GuildShopClose		);

	Cli_NetReg( PG_Guild_LtoC_AllGuildMemberCountResult		);
	Cli_NetReg( PG_Guild_DtoL_GuildPlayerOnline		);

	Cli_NetReg( PG_Guild_DtoC_GuildLeaderChange		);
	Cli_NetReg( PG_Guild_DtoC_GuildMemberRankChange		);

	Cli_NetReg( PG_Guild_LtoC_SetGuildReady		);

	Cli_NetReg( PG_Guild_DtoC_GuildFightInfo	);
	Cli_NetReg( PG_Guild_DtoC_GuildFightAuctionBidResult	);
	Cli_NetReg( PG_Guild_DtoC_GuildFightAuctionBidNewOne	);
	Cli_NetReg( PG_Guild_DtoC_GuildFightAuctionEnd	);

	Cli_NetReg( PG_Guild_DtoC_GuildFightBeginNotify	);
	Cli_NetReg( PG_Guild_LtoC_GuildFightLeaderInvite	);
	Cli_NetReg( PG_Guild_LtoC_GuildFightInviteNotify	);
	Cli_NetReg( PG_Guild_LtoC_GuildIDChange	);

	Cli_NetReg( PG_Guild_LtoC_DeclareWarResult			);
	Cli_NetReg( PG_Guild_LtoC_WarInfo					);
	Cli_NetReg( PG_Guild_LtoC_LoginWarInfo				);
	Cli_NetReg( PG_Guild_DtoC_DeclareWarFinalResult		);

	Cli_NetReg( PG_Guild_XtoC_SetGuildFlagInfoResult	);

	Cli_NetReg( PG_Guild_DtoC_ChangeGuildNameResult		);
	Cli_NetReg( PG_Guild_DtoC_ChangeGuildName			);

	Cli_NetReg( PG_Guild_DtoC_PrepareWar				);

	Cli_NetReg( PG_Guild_DtoC_GuildUpgradeResult			);
	Cli_NetReg( PG_Guild_DtoC_SelectContributionLogResult	);

	Cli_NetReg( PG_Guild_DtoX_ModifySimpleGuild	);

	Cli_NetReg( PG_Guild_DtoC_GuildBaseInfoUpdate		);
	Cli_NetReg( PG_Guild_DtoC_Guild_LevelUp				);
	Cli_NetReg( PG_Guild_DtoC_GuildInfoChange			);
	Cli_NetReg( PG_Guild_DtoC_GuildWarHistroyResult 	);
    return true;
}
//-------------------------------------------------------------------
bool NetCli_Guild::_Release()
{
    return true;
}
//-------------------------------------------------------------------
void NetCli_Guild::_PG_Guild_DtoC_GuildWarHistroyResult( int Sockid , int Size , void* Data )
{
	PG_Guild_DtoC_GuildWarHistroyResult* pg = (PG_Guild_DtoC_GuildWarHistroyResult*)Data;
	This->RD_GuildWarHistroyResult( pg->Count , pg->Log );
}
void NetCli_Guild::_PG_Guild_DtoC_GuildInfoChange		( int Sockid , int Size , void* Data )
{
	PG_Guild_DtoC_GuildInfoChange* PG = (PG_Guild_DtoC_GuildInfoChange*)Data;
	This->RD_GuildInfoChange( PG->ChangeMode );
}

void NetCli_Guild::_PG_Guild_DtoC_Guild_LevelUp( int Sockid , int Size , void* Data )
{
	PG_Guild_DtoC_Guild_LevelUp* PG = (PG_Guild_DtoC_Guild_LevelUp*)Data;
	This->RD_Guild_LevelUp( PG->GuildID , PG->Level );
}

void NetCli_Guild::_PG_Guild_DtoC_GuildBaseInfoUpdate( int Sockid , int Size , void* Data )
{
	This->RD_GuildBaseInfoUpdate( );
}
void NetCli_Guild::_PG_Guild_DtoX_ModifySimpleGuild			( int Sockid , int Size , void* Data )
{
	PG_Guild_DtoX_ModifySimpleGuild* PG = (PG_Guild_DtoX_ModifySimpleGuild*)Data;
	This->RD_ModifySimpleGuild( PG->SimpleGuildInfo );
}
void NetCli_Guild::_PG_Guild_DtoC_GuildUpgradeResult			( int Sockid , int Size , void* Data )
{
	PG_Guild_DtoC_GuildUpgradeResult* PG = (PG_Guild_DtoC_GuildUpgradeResult*)Data;
	This->RD_GuildUpgradeResult( PG->GuildLv , PG->Result );
}
void NetCli_Guild::_PG_Guild_DtoC_SelectContributionLogResult	( int Sockid , int Size , void* Data )
{
	PG_Guild_DtoC_SelectContributionLogResult* PG = (PG_Guild_DtoC_SelectContributionLogResult*)Data;
	This->RD_SelectContributionLogResult( PG->Count , PG->Log );
}
void NetCli_Guild::_PG_Guild_DtoC_PrepareWar				( int Sockid , int Size , void* Data )
{
	PG_Guild_DtoC_PrepareWar* PG = (PG_Guild_DtoC_PrepareWar*)Data;
	This->RD_PrepareWar( PG->GuildID , PG->Time );
}
void NetCli_Guild::_PG_Guild_DtoC_ChangeGuildNameResult		( int Sockid , int Size , void* Data )
{
	PG_Guild_DtoC_ChangeGuildNameResult* PG = (PG_Guild_DtoC_ChangeGuildNameResult*)Data;
	This->RD_ChangeGuildNameResult( PG->Result );
}
void NetCli_Guild::_PG_Guild_DtoC_ChangeGuildName			( int Sockid , int Size , void* Data )
{
	PG_Guild_DtoC_ChangeGuildName* PG = (PG_Guild_DtoC_ChangeGuildName*)Data;
	This->RD_ChangeGuildName( PG->GuildID , (char*)PG->GuildName.Begin() );
}
//-------------------------------------------------------------------
void NetCli_Guild::_PG_Guild_XtoC_SetGuildFlagInfoResult	( int Sockid , int Size , void* Data )
{
	PG_Guild_XtoC_SetGuildFlagInfoResult* PG = (PG_Guild_XtoC_SetGuildFlagInfoResult*)Data;
	This->RX_SetGuildFlagInfoResult( PG->Result );
}
void NetCli_Guild::_PG_Guild_DtoC_DeclareWarFinalResult		( int Sockid , int Size , void* Data )
{
	PG_Guild_DtoC_DeclareWarFinalResult* PG = (PG_Guild_DtoC_DeclareWarFinalResult*)Data;
	This->RD_DeclareWarFinalResult( PG->GuildID , PG->Score , PG->KillCount );
}
void NetCli_Guild::_PG_Guild_LtoC_LoginWarInfo			( int Sockid , int Size , void* Data )
{
	PG_Guild_LtoC_LoginWarInfo* PG = (PG_Guild_LtoC_LoginWarInfo*)Data;
	This->RL_LoginWarInfo( PG->GuildID , PG->Time , PG->BeginTime );
}
void NetCli_Guild::_PG_Guild_LtoC_DeclareWarResult		( int Sockid , int Size , void* Data )
{
	PG_Guild_LtoC_DeclareWarResult* PG = (PG_Guild_LtoC_DeclareWarResult*)Data;
	This->RL_DeclareWarResult( PG->Result , PG->Type , PG->TargetGuildID );
}
void NetCli_Guild::_PG_Guild_LtoC_WarInfo				( int Sockid , int Size , void* Data )
{
	PG_Guild_LtoC_WarInfo* PG = (PG_Guild_LtoC_WarInfo*)Data;
	This->RL_WarInfo( PG->IsDeclareWar , PG->GuildID , PG->Time , PG->BeginTime );
}

void NetCli_Guild::_PG_Guild_LtoC_GuildIDChange				( int Sockid , int Size , void* Data )
{
	PG_Guild_LtoC_GuildIDChange* PG = (PG_Guild_LtoC_GuildIDChange*)Data;
	This->RL_GuildIDChange( PG->GItemID , PG->GuildID );
}
void NetCli_Guild::_PG_Guild_LtoC_GuildFightInviteNotify				( int Sockid , int Size , void* Data )
{
	PG_Guild_LtoC_GuildFightInviteNotify* PG = (PG_Guild_LtoC_GuildFightInviteNotify*)Data;
	This->RL_GuildFightInviteNotify( PG );
}

void NetCli_Guild::_PG_Guild_LtoC_SetGuildReady				( int Sockid , int Size , void* Data )
{
	PG_Guild_LtoC_SetGuildReady* PG = (PG_Guild_LtoC_SetGuildReady*)Data;
	This->RL_SetGuildReady( PG->Result );
}
//-------------------------------------------------------------------
void NetCli_Guild::_PG_Guild_DtoC_GuildLeaderChange			( int Sockid , int Size , void* Data )
{
	PG_Guild_DtoC_GuildLeaderChange* PG = (PG_Guild_DtoC_GuildLeaderChange*)Data;
	This->RD_GuildLeaderChange( PG->NewLeaderDBID , PG->OldLeaderDBID );
}

void NetCli_Guild::_PG_Guild_DtoC_GuildMemberRankChange			( int Sockid , int Size , void* Data )
{
	PG_Guild_DtoC_GuildMemberRankChange* PG = (PG_Guild_DtoC_GuildMemberRankChange*)Data;
	This->RD_GuildMemberRankChange( PG->MemberDBID , PG->Rank );
}

void NetCli_Guild::_PG_Guild_DtoL_GuildPlayerOnline			( int Sockid , int Size , void* Data )
{
	PG_Guild_DtoL_GuildPlayerOnline* PG = (PG_Guild_DtoL_GuildPlayerOnline*)Data;
	This->RD_GuildPlayerOnline( PG->PlayerDBID , PG->IsOnline );
}
//公會戰競標
void NetCli_Guild::_PG_Guild_LtoC_GuildFightAuctionOpen			( int Sockid , int Size , void* Data )
{
	PG_Guild_LtoC_GuildFightAuctionOpen* PG = (PG_Guild_LtoC_GuildFightAuctionOpen*)Data;
	This->RL_GuildFightAuctionOpen( PG->TargetNPCID , PG->CrystalID , PG->OwnerGuildID , PG->BidGuildID , PG->BidScore );
}
void NetCli_Guild::_PG_Guild_LtoC_GuildFightAuctionClose		( int Sockid , int Size , void* Data )
{
	PG_Guild_LtoC_GuildFightAuctionClose* PG = (PG_Guild_LtoC_GuildFightAuctionClose*)Data;
	This->RL_GuildFightAuctionClose( );
}
void NetCli_Guild::_PG_Guild_DtoC_GuildFightAuctionBidResult	( int Sockid , int Size , void* Data )
{
	PG_Guild_DtoC_GuildFightAuctionBidResult* PG = (PG_Guild_DtoC_GuildFightAuctionBidResult*)Data;
	This->RD_GuildFightAuctionBidResult( PG->CrystalID , PG->OwnerGuildID , PG->BidGuildID, (PG->iResult != 0) );
}
void NetCli_Guild::_PG_Guild_DtoC_GuildFightAuctionBidNewOne	( int Sockid , int Size , void* Data )
{
	PG_Guild_DtoC_GuildFightAuctionBidNewOne* PG = (PG_Guild_DtoC_GuildFightAuctionBidNewOne*)Data;
	This->RD_GuildFightAuctionBidNewOne( PG->CrystalID , PG->OwnerGuildID , PG->BidGuildID );
}
void NetCli_Guild::_PG_Guild_LtoC_AllGuildMemberCountResult		( int Sockid , int Size , void* Data )
{
	PG_Guild_LtoC_AllGuildMemberCountResult* PG = (PG_Guild_LtoC_AllGuildMemberCountResult*)Data;
	This->RL_AllGuildMemberCountResult( PG->Count , PG->MemberList);
}

/*
void NetCli_Guild::_PG_Guild_LtoC_GuildShopOpen				( int Sockid , int Size , void* Data )
{
	PG_Guild_LtoC_GuildShopOpen* PG = (PG_Guild_LtoC_GuildShopOpen*)Data;
	This->RL_GuildShopOpen( PG->TargetNPCID , PG->Score , PG->Flag );
}
void NetCli_Guild::_PG_Guild_DtoC_GuildShopBuyResult		( int Sockid , int Size , void* Data )
{
	PG_Guild_DtoC_GuildShopBuyResult* PG = (PG_Guild_DtoC_GuildShopBuyResult*)Data;
	This->RD_GuildShopBuyResult( PG->FlagID , PG->Result );
}
void NetCli_Guild::_PG_Guild_LtoC_GuildShopClose			( int Sockid , int Size , void* Data )
{
	This->RL_GuildShopClose( );
}
*/
void NetCli_Guild::_PG_Guild_LtoC_OpenCreateGuild				( int Sockid , int Size , void* Data )	
{
	PG_Guild_LtoC_OpenCreateGuild* PG = (PG_Guild_LtoC_OpenCreateGuild*)Data;
	This->RL_OpenCreateGuild( PG->TargetNPCID );
}
void NetCli_Guild::_PG_Guild_LtoC_CloseCreateGuild				( int Sockid , int Size , void* Data )	
{
	PG_Guild_LtoC_CloseCreateGuild* PG = (PG_Guild_LtoC_CloseCreateGuild*)Data;
	This->RL_CloseCreateGuild( );
}
void NetCli_Guild::_PG_Guild_LtoC_OpenGuildContribution			( int Sockid , int Size , void* Data )	
{
	PG_Guild_LtoC_OpenGuildContribution* PG = (PG_Guild_LtoC_OpenGuildContribution*)Data;
	This->RL_OpenGuildContribution( PG->TargetNPCID );

}
void NetCli_Guild::_PG_Guild_LtoC_GuildContributionItemResult	( int Sockid , int Size , void* Data )	
{
	PG_Guild_LtoC_GuildContributionItemResult* PG = (PG_Guild_LtoC_GuildContributionItemResult*)Data;
	This->RL_GuildContributionItemResult( PG->Result );

}
void NetCli_Guild::_PG_Guild_LtoC_GuildContributionClose		( int Sockid , int Size , void* Data )	
{
	PG_Guild_LtoC_GuildContributionClose* PG = (PG_Guild_LtoC_GuildContributionClose*)Data;
	This->RL_GuildContributionClose( );
}

void NetCli_Guild::_PG_Guild_DtoC_JoinGuildMember			( int Sockid , int Size , void* Data )
{
	PG_Guild_DtoC_JoinGuildMember* PG = (PG_Guild_DtoC_JoinGuildMember*)Data;
	This->RD_JoinGuildMember( PG->Member );
}

void NetCli_Guild::_PG_Guild_DtoC_LeaveGuildMember			( int Sockid , int Size , void* Data )
{
	PG_Guild_DtoC_LeaveGuildMember* PG = (PG_Guild_DtoC_LeaveGuildMember*)Data;
	This->RD_LeaveGuildMember( PG->LeaderDBID , PG->MemberDBID );
}

void NetCli_Guild::_PG_Guild_LtoC_GuildBaseInfo			( int Sockid , int Size , void* Data )	
{
	PG_Guild_LtoC_GuildBaseInfo* PG = (PG_Guild_LtoC_GuildBaseInfo*)Data;
	This->RL_GuildBaseInfo( PG->GuildID , (char*)PG->GuildName.Begin() , PG->Flag , PG->IsReady );
}
void NetCli_Guild::_PG_Guild_LtoC_GuildDestroy			( int Sockid , int Size , void* Data )	
{
	PG_Guild_LtoC_GuildDestroy* PG = (PG_Guild_LtoC_GuildDestroy*)Data;
	This->RL_GuildDestroy( PG->GuildID );
}
void NetCli_Guild::_PG_Guild_LtoC_CreateGuildResult		( int Sockid , int Size , void* Data )
{
	PG_Guild_LtoC_CreateGuildResult *  PG = (PG_Guild_LtoC_CreateGuildResult *)Data;
	This->RL_CreateGuildResult( PG->GuildID , (char*)PG->GuildName.Begin() , PG->Result );
}
void NetCli_Guild::_PG_Guild_LtoC_NewGuildInvite		( int Sockid , int Size , void* Data )
{
	PG_Guild_LtoC_NewGuildInvite *  PG = (PG_Guild_LtoC_NewGuildInvite *)Data;
	This->RL_NewGuildInvite( PG->GuildID , (char*)PG->LeaderName.Begin() , PG->LeaderDBID );
}
void NetCli_Guild::_PG_Guild_LtoC_NewGuildInviteResult	( int Sockid , int Size , void* Data )
{
	PG_Guild_LtoC_NewGuildInviteResult *  PG = (PG_Guild_LtoC_NewGuildInviteResult *)Data;
	This->RL_NewGuildInviteResult( (char*)PG->PlayerName.Begin() , PG->Result );
}
void NetCli_Guild::_PG_Guild_LtoC_JoindNewGuild		( int Sockid , int Size , void* Data )
{
	PG_Guild_LtoC_JoindNewGuild *  PG = (PG_Guild_LtoC_JoindNewGuild *)Data;
	This->RL_JoindNewGuild( PG->GuildID , (char*)PG->LeaderName.Begin() );
}
void NetCli_Guild::_PG_Guild_LtoC_LeaveNewGuildResult	( int Sockid , int Size , void* Data )
{
	PG_Guild_LtoC_LeaveNewGuildResult *  PG = (PG_Guild_LtoC_LeaveNewGuildResult *)Data;
	This->RL_LeaveNewGuildResult( (char*)PG->KickPlayerName.Begin() , PG->Result );
}
void NetCli_Guild::_PG_Guild_LtoC_DelNewGuildResult		( int Sockid , int Size , void* Data )
{
	PG_Guild_LtoC_DelNewGuildResult *  PG = (PG_Guild_LtoC_DelNewGuildResult *)Data;
	This->RL_DelNewGuildResult( PG->Result );
}
void NetCli_Guild::_PG_Guild_LtoC_GuildInfo				( int Sockid , int Size , void* Data )
{
	PG_Guild_LtoC_GuildInfo *  PG = (PG_Guild_LtoC_GuildInfo *)Data;
	This->RL_GuildInfo( PG->Guild , PG->MemberCount );
}
void NetCli_Guild::_PG_Guild_LtoC_GuildMemberListCount	( int Sockid , int Size , void* Data )
{
	PG_Guild_LtoC_GuildMemberListCount *  PG = (PG_Guild_LtoC_GuildMemberListCount *)Data;
	_GuildID = PG->GuildID;
	_MemberCount = PG->MemberCount;
	_MemberList.clear();
}
void NetCli_Guild::_PG_Guild_LtoC_GuildMemberBaseInfo	( int Sockid , int Size , void* Data )
{
	PG_Guild_LtoC_GuildMemberBaseInfo *  PG = (PG_Guild_LtoC_GuildMemberBaseInfo *)Data;
	_MemberList.push_back( PG->Info );
	if ( (int)_MemberList.size() >= _MemberCount )
	{
		This->RL_GuildMemberBaseInfo(_MemberList);
	}
}
void NetCli_Guild::_PG_Guild_LtoC_GuildMemberInfo		( int Sockid , int Size , void* Data )
{
	PG_Guild_LtoC_GuildMemberInfo *  PG = (PG_Guild_LtoC_GuildMemberInfo *)Data;
	This->RL_GuildMemberInfo( PG->Member );
}
void NetCli_Guild::_PG_Guild_DtoC_GuildCreate			( int Sockid , int Size , void* Data )
{
	PG_Guild_DtoC_GuildCreate *  PG = (PG_Guild_DtoC_GuildCreate *)Data;
	This->RD_GuildCreate( PG->GuildID );
}
//////////////////////////////////////////////////////////////////////////
void NetCli_Guild::_PG_Guild_LtoC_GuildInvite				( int Sockid , int Size , void* Data )
{
	PG_Guild_LtoC_GuildInvite *  PG = (PG_Guild_LtoC_GuildInvite *)Data;
	This->RL_GuildInvite( PG->GuildID , (char*)PG->LeaderName.Begin() , PG->LeaderDBID );
}
void NetCli_Guild::_PG_Guild_LtoC_GuildInviteResult			( int Sockid , int Size , void* Data )
{
	PG_Guild_LtoC_GuildInviteResult *  PG = (PG_Guild_LtoC_GuildInviteResult *)Data;
	This->RL_GuildInviteResult( (char*)PG->PlayerName.Begin() , PG->Result );
}
void NetCli_Guild::_PG_Guild_LtoC_JoindGuild				( int Sockid , int Size , void* Data )
{
	PG_Guild_LtoC_JoindGuild *  PG = (PG_Guild_LtoC_JoindGuild *)Data;
	This->RL_JoindGuild( PG->GuildID , (char*)PG->LeaderName.Begin() );
}
void NetCli_Guild::_PG_Guild_LtoC_LeaveGuildResult			( int Sockid , int Size , void* Data )
{
	PG_Guild_LtoC_LeaveGuildResult *  PG = (PG_Guild_LtoC_LeaveGuildResult *)Data;
	This->RL_LeaveGuildResult( (char*)PG->KickPlayerName.Begin() , PG->Result );
}
void NetCli_Guild::_PG_Guild_LtoC_DelGuildResult			( int Sockid , int Size , void* Data )
{
	PG_Guild_LtoC_DelGuildResult *  PG = (PG_Guild_LtoC_DelGuildResult *)Data;
	This->RL_DelGuildResult( PG->Result );
}
//////////////////////////////////////////////////////////////////////////
void NetCli_Guild::_PG_Guild_DtoC_BoardPostResult			( int Sockid , int Size , void* Data )
{
	PG_Guild_DtoC_BoardPostResult *  PG = (PG_Guild_DtoC_BoardPostResult *)Data;
	This->RD_BoardPostResult( PG->Result );
}
void NetCli_Guild::_PG_Guild_DtoC_BoardListCount			( int Sockid , int Size , void* Data )
{
	PG_Guild_DtoC_BoardListCount *  PG = (PG_Guild_DtoC_BoardListCount *)Data;
	This->RD_BoardListCount( PG->MaxCount , PG->Count , PG->PageID );
}
void NetCli_Guild::_PG_Guild_DtoC_BoardListData				( int Sockid , int Size , void* Data )
{
	PG_Guild_DtoC_BoardListData *  PG = (PG_Guild_DtoC_BoardListData *)Data;
	This->RD_BoardListData( PG->GUID , PG->Index , (char*)PG->PlayerName.Begin() , PG->Time , (char*)PG->Title.Begin() , PG->Type );
}
void NetCli_Guild::_PG_Guild_DtoC_BoardMessageResult		( int Sockid , int Size , void* Data )
{
	PG_Guild_DtoC_BoardMessageResult *  PG = (PG_Guild_DtoC_BoardMessageResult *)Data;
	This->RD_BoardMessageResult( PG->Message );
}
void NetCli_Guild::_PG_Guild_DtoC_BoardModifyMessageResult	( int Sockid , int Size , void* Data )
{
	PG_Guild_DtoC_BoardModifyMessageResult *  PG = (PG_Guild_DtoC_BoardModifyMessageResult *)Data;
	This->RD_BoardModifyMessageResult( PG->Result );
}
void NetCli_Guild::_PG_Guild_DtoC_BoardModifyModeResult		( int Sockid , int Size , void* Data )
{
	PG_Guild_DtoC_BoardModifyModeResult *  PG = (PG_Guild_DtoC_BoardModifyModeResult *)Data;
	This->RD_BoardModifyModeResult( PG->Result );
}
//////////////////////////////////////////////////////////////////////////
void NetCli_Guild::_PG_Guild_DtoC_GuildFightInfo( int Sockid , int Size , void* Data )
{
	PG_Guild_DtoC_GuildFightInfo* PG = (PG_Guild_DtoC_GuildFightInfo*)Data;
	This->RD_GuildFightInfo( PG->emResult, PG->iCrystalID, PG->iDefenderGuildID, PG->iAttackerGuildID, PG->iHighestAmount, PG->iRemainTime, PG->iFightTime, PG->iOtherCrystalID, PG->iGuildPoint );
}

void NetCli_Guild::_PG_Guild_DtoC_GuildFightAuctionEnd( int Sockid , int Size , void* Data )
{
	PG_Guild_DtoC_GuildFightAuctionEnd* PG = (PG_Guild_DtoC_GuildFightAuctionEnd*)Data;
	This->RD_GuildFightAuctionEnd( PG->iCrystalID, PG->iDefenderGuildID, PG->iAttackerGuildID, PG->iFightTime, PG->iHighestAmount );
}

void NetCli_Guild::_PG_Guild_DtoC_GuildFightBeginNotify( int Sockid , int Size , void* Data )
{
	PG_Guild_DtoC_GuildFightBeginNotify* PG = (PG_Guild_DtoC_GuildFightBeginNotify*)Data;
	This->RD_GuildFightBeginNotify( PG->iCrystalID, PG->iRemainTime );
}
//--------------------------------------------------------------------------------------------
void NetCli_Guild::_PG_Guild_LtoC_GuildFightLeaderInvite		( int Sockid , int Size , void* Data )
{
	PG_Guild_LtoC_GuildFightLeaderInvite* PG = (PG_Guild_LtoC_GuildFightLeaderInvite*)Data;
	This->RL_GuildFightLeaderInvite( PG );
}
//--------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////
void NetCli_Guild::SL_CreateGuild				( char* GuildName )
{
	PG_Guild_CtoL_CreateGuild Send;
	Send.GuildName = GuildName;
	SendToLocal_Base( sizeof(Send) , &Send );
}
void NetCli_Guild::SL_NewGuildInvite			( char* PlayerName )
{
	PG_Guild_CtoL_NewGuildInvite Send;
	Send.PlayerName = PlayerName;
	SendToLocal_Base( sizeof(Send) , &Send );
}
void NetCli_Guild::SL_NewGuildInviteResult		( int GuildID , int LeaderDBID , bool Result )
{
	PG_Guild_CtoL_NewGuildInviteResult Send;
	Send.GuildID = GuildID;
	Send.LeaderDBID = LeaderDBID;
	Send.Result = Result;
	SendToLocal_Base( sizeof(Send) , &Send );
}
void NetCli_Guild::SL_LeaveNewGuild			( char* KickPlayerName )
{
	PG_Guild_CtoL_LeaveNewGuild Send;
	Send.PlayerName = KickPlayerName;
	SendToLocal_Base( sizeof(Send) , &Send );

}
void NetCli_Guild::SL_DelNewGuild			( )
{
	PG_Guild_CtoL_DelNewGuild Send;
	SendToLocal_Base( sizeof(Send) , &Send );
}

void NetCli_Guild::SL_GuildInvite			( char* PlayerName )
{
	PG_Guild_CtoL_GuildInvite Send;
	Send.PlayerName = PlayerName;
	SendToLocal_Base( sizeof(Send) , &Send );
}
void NetCli_Guild::SL_GuildInviteResult		( int GuildID , int LeaderDBID , bool Result )
{
	PG_Guild_CtoL_GuildInviteResult Send;
	Send.GuildID = GuildID;
	Send.LeaderDBID = LeaderDBID;
	Send.Result = Result;
	SendToLocal_Base( sizeof(Send) , &Send );
}
void NetCli_Guild::SL_LeaveGuild			( char* KickPlayerName )
{
	PG_Guild_CtoL_LeaveGuild Send;
	Send.PlayerName = KickPlayerName;
	SendToLocal_Base( sizeof(Send) , &Send );

}
void NetCli_Guild::SL_DelGuild			( )
{
	PG_Guild_CtoL_DelGuild Send;
	SendToLocal_Base( sizeof(Send) , &Send );
}

void NetCli_Guild::SL_ModifyGuildInfoRequest		( GuildBaseStruct& Guild )
{
	PG_Guild_CtoL_ModifyGuildInfoRequest Send;
	Send.Guild = Guild;
	SendToLocal_Base( sizeof(Send) , &Send );
}
void NetCli_Guild::SL_ModifyGuildMemberInfoRequest	( GuildMemberStruct& Member )
{
	PG_Guild_CtoL_ModifyGuildMemberInfoRequest Send;
	Send.Member = Member;
	SendToLocal_Base( sizeof(Send) , &Send );
}
void NetCli_Guild::SL_GuildInfoRequet				( int GuildID )
{
	PG_Guild_CtoL_GuildInfoRequet Send;
	Send.GuildID = GuildID;
	SendToLocal_Base( sizeof(Send) , &Send );
}
void NetCli_Guild::SL_GuildMemberListRequet		( int GuildID )
{
	PG_Guild_CtoL_GuildMemberListRequet Send;
	Send.GuildID = GuildID;
	SendToLocal_Base( sizeof(Send) , &Send );	
}
void NetCli_Guild::SL_GuildMemberInfoRequet		( int MemberDBID )
{ 
	PG_Guild_CtoL_GuildMemberInfoRequet Send;
	Send.MemberDBID = MemberDBID;
	SendToLocal_Base( sizeof(Send) , &Send );	
}
//////////////////////////////////////////////////////////////////////////
// 留言板 
//////////////////////////////////////////////////////////////////////////
void NetCli_Guild::SL_BoardPostRequest			( const char* Title ,const char* Msg )
{
	PG_Guild_CtoL_BoardPostRequest Send;
	Send.Message.Title	= Title;
	Send.Message.Message = Msg;
	SendToLocal_Base( sizeof(Send) , &Send );	
}
void NetCli_Guild::SL_BoardListRequest			( int PageID )
{
	PG_Guild_CtoL_BoardListRequest Send;
	Send.PageID = PageID;
	SendToLocal_Base( sizeof(Send) , &Send );	
}
void NetCli_Guild::SL_BoardMessageRequest		( int MessageGUID )
{
	PG_Guild_CtoL_BoardMessageRequest Send;
	Send.GUID = MessageGUID;
	SendToLocal_Base( sizeof(Send) , &Send );	
}
void NetCli_Guild::SL_BoardModifyMessageRequest	( int MessageGUID , const char* Message )
{
	PG_Guild_CtoL_BoardModifyMessageRequest Send;
	Send.Message = Message;
	Send.MessageGUID = MessageGUID;
	SendToLocal_Base( sizeof(Send) , &Send );	
}
void NetCli_Guild::SL_BoardModifyModeRequest	( int MessageGUID , GuildBoardModeStruct	Mode )
{
	PG_Guild_CtoL_BoardModifyModeRequest Send;
	Send.Mode = Mode;
	Send.MessageGUID = MessageGUID;
	SendToLocal_Base( sizeof(Send) , &Send );	
}
//通知Server Client 已關閉建立公會的介面
void NetCli_Guild::SL_CloseCreateGuild			( )
{
	PG_Guild_CtoL_CloseCreateGuild Send;
	SendToLocal_Base( sizeof(Send) , &Send );	
}
//////////////////////////////////////////////////////////////////////////
// 貢獻
//////////////////////////////////////////////////////////////////////////
void NetCli_Guild::SL_GuildContributionItem	( int Pos , ItemFieldStruct& Item , bool IsPut )	
{
	PG_Guild_CtoL_GuildContributionItem Send;
	Send.Pos = Pos;
	Send.Item = Item;
	Send.IsPut = IsPut;
	SendToLocal_Base( sizeof(Send) , &Send );	
}
void NetCli_Guild::SL_GuildContributionOK		( int Money , int Bonus_Money )
{
	PG_Guild_CtoL_GuildContributionOK Send;
	Send.Money = Money;
	Send.Bonus_Money = Bonus_Money;
	SendToLocal_Base( sizeof(Send) , &Send );	
}
void NetCli_Guild::SL_GuildContributionClose	( )
{
	PG_Guild_CtoL_GuildContributionClose Send;
	SendToLocal_Base( sizeof(Send) , &Send );	
}
/*

void NetCli_Guild::SL_GuildShopBuy	(int flagid )
{
	PG_Guild_CtoL_GuildShopBuy Send;
	Send.FlagID=flagid;
	SendToLocal( sizeof(Send) , &Send );	

}
//關閉商店介面
void NetCli_Guild::SL_GuildShopClose	( )
{
	PG_Guild_CtoL_GuildShopClose Send;
	SendToLocal( sizeof(Send) , &Send );	
}
*/
//要求所有公會的成員數
void NetCli_Guild::SL_AllGuildMemberCountRequest	( )
{
	PG_Guild_CtoL_AllGuildMemberCountRequest Send;
	SendToLocal_Base( sizeof(Send) , &Send );	
}

void NetCli_Guild::SL_GuildFightAuctionClose	( )
{
	PG_Guild_CtoL_GuildFightAuctionClose Send;
	SendToLocal_Base( sizeof(Send) , &Send );	
}
void NetCli_Guild::SL_GuildFightAuctionBid ( int BidScore, int iCrystalID )
{
	PG_Guild_CtoL_GuildFightAuctionBid Send;
	Send.CrystalID		= iCrystalID;
	Send.BidScore		= BidScore;
	SendToLocal_Base( sizeof(Send) , &Send );	
}

void NetCli_Guild::SL_GuildFightRequestInvite( int iSrvID, int iRoomID, int iCrystalID, int iGuildID, int iMemberDBID )
{
	PG_Guild_CtoL_GuildFightRequestInvite Send;
	Send.iCrystalID		= iCrystalID;
	Send.iMemberDBID	= iMemberDBID;
	Send.iRoomID		= iRoomID;
	CNetGlobal::SendToServer( iSrvID, sizeof(Send) , &Send );
}


void NetCli_Guild::SL_GuildFightInviteRespond( int iSrvID, int iRoomID,  int iCrystalID, int iGuildID,int iMemberDBID, bool bJoin )
{
	PG_Guild_CtoL_GuildFightInviteRespond Send;
	Send.iCrystalID		= iCrystalID;
	Send.iMemberDBID	= iMemberDBID;
	Send.iRoomID		= iRoomID;
	Send.bJoin			= bJoin;
	Send.iGuildID		= iGuildID;
	CNetGlobal::SendToServer( iSrvID, sizeof(Send) , &Send );
}

void NetCli_Guild::SL_DeclareWar	 ( int Type , int TargetGuildID )
{
	PG_Guild_CtoL_DeclareWar Send;
	Send.Type = Type;
	Send.TargetGuildID = TargetGuildID;
	SendToLocal_Base( sizeof(Send) , &Send );	
}

void  NetCli_Guild::SL_SetGuildFlagInfoRequest		( GuildFlagStruct&	Flag )
{
	PG_Guild_CtoL_SetGuildFlagInfoRequest Send;
	Send.Flag = Flag;
	SendToLocal_Base( sizeof(Send) , &Send );	
}

void  NetCli_Guild::SL_ChangeGuildNameRequest( const char* GuildName )
{
	PG_Guild_CtoL_ChangeGuildNameRequest Send;
	Send.GuildName = GuildName;
	SendToLocal_Base( sizeof(Send) , &Send );	
}

void NetCli_Guild::SL_GuildUpgradeRequest		( int GuildLv )
{
	PG_Guild_CtoL_GuildUpgradeRequest Send;
	Send.GuildLv = GuildLv;
	SendToLocal_Base( sizeof(Send) , &Send );	
}
void NetCli_Guild::SL_SelectContributionLog		( int DayBefore )
{
	PG_Guild_CtoL_SelectContributionLog	Send;
	Send.DayBefore = DayBefore;
	SendToLocal_Base( sizeof(Send) , &Send );	
}

//要求公會戰歷史資料
void NetCli_Guild::SL_GuildWarHistroy(  )
{
	PG_Guild_CtoL_GuildWarHistroy Send;
	SendToLocal_Base( sizeof(Send) , &Send );	
}
