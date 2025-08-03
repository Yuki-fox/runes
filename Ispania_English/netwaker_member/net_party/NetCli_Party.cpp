#include "../mainproc/GameMain.h"
#include "NetCli_Party.h"
#include "../net_channel/NetCli_Channel.h"

//-------------------------------------------------------------------
NetCli_Party*    NetCli_Party::This         = NULL;
//-------------------------------------------------------------------
bool NetCli_Party::_Init()
{
	/*
	_Net.RegOnSrvPacketFunction	( EM_PG_Party_ChattoC_Invite			, _PG_Party_ChattoC_Invite			);
	_Net.RegOnSrvPacketFunction	( EM_PG_Party_ChattoC_PartyBaseInfo		, _PG_Party_ChattoC_PartyBaseInfo	);
	_Net.RegOnSrvPacketFunction	( EM_PG_Party_ChattoC_AddMember			, _PG_Party_ChattoC_AddMember		);
	_Net.RegOnSrvPacketFunction	( EM_PG_Party_ChattoC_DelMember			, _PG_Party_ChattoC_DelMember		);
	_Net.RegOnSrvPacketFunction	( EM_PG_Party_ChattoC_FixMember			, _PG_Party_ChattoC_FixMember		);
	_Net.RegOnSrvPacketFunction	( EM_PG_Party_ChattoC_PartyTreasure		, _PG_Party_ChattoC_PartyTreasure		);
	_Net.RegOnSrvPacketFunction	( EM_PG_Party_ChattoC_PartyTreasureLoot	, _PG_Party_ChattoC_PartyTreasureLoot	);
	_Net.RegOnSrvPacketFunction	( EM_PG_Party_ChattoC_GiveItemInfo		, _PG_Party_ChattoC_GiveItemInfo		);
	_Net.RegOnSrvPacketFunction	( EM_PG_Party_LtoC_MemberLogout			, _PG_Party_LtoC_MemberLogout			);
	_Net.RegOnSrvPacketFunction	( EM_PG_Party_ChattoC_SetMemberPosResult, _PG_Party_ChattoC_SetMemberPosResult	);
*/
	Cli_NetReg( PG_Party_ChattoC_Invite				);
	Cli_NetReg( PG_Party_ChattoC_PartyBaseInfo		);
	Cli_NetReg( PG_Party_ChattoC_AddMember			);
	Cli_NetReg( PG_Party_ChattoC_DelMember			);
	Cli_NetReg( PG_Party_ChattoC_FixMember			);
	Cli_NetReg( PG_Party_ChattoC_PartyTreasure		);
	Cli_NetReg( PG_Party_ChattoC_PartyTreasureLoot	);
	Cli_NetReg( PG_Party_ChattoC_GiveItemInfo		);
	Cli_NetReg( PG_Party_LtoC_MemberLogout			);
	Cli_NetReg( PG_Party_ChattoC_SetMemberPosResult	);


	Cli_NetReg( PG_ZoneParty_LtoC_PartyBase			);
	Cli_NetReg( PG_ZoneParty_LtoC_Invite			);
	Cli_NetReg( PG_ZoneParty_LtoC_AddMember			);
	Cli_NetReg( PG_ZoneParty_LtoC_DelMember			);
	Cli_NetReg( PG_ZoneParty_LtoC_FixMember			);
	Cli_NetReg( PG_ZoneParty_LtoC_SetMemberPosResult );
	Cli_NetReg( PG_ZoneParty_LtoC_ChangeLeaderResult );
	Cli_NetReg( PG_ZoneParty_LtoC_Talk				 );
	Cli_NetReg( PG_Party_LtoC_GetMemberInfoResult	 );

	_Net.RegOnSrvDisConnectFunction		( OnEvent_ServerDisConnect	);

	ClitoCli_NetReg( PG_Party_CtoC_AskJoinToParty );		// 申請加入隊伍
	ClitoCli_NetReg( PG_Party_CtoC_AskJoinToPartyResult );	// 申請加入隊伍的回應
	ClitoCli_NetReg( PG_Party_CtoC_DeletePBPost );			// 要求刪除在組隊招募留言板上的留言

    return true;
}
//-------------------------------------------------------------------
bool NetCli_Party::_Release()
{
    return true;
}
//-------------------------------------------------------------------
void NetCli_Party::_PG_Party_LtoC_GetMemberInfoResult	( int NetID , int Size , void* Data )
{
	static vector< PartyMemberInfoStruct > List;
	PG_Party_LtoC_GetMemberInfoResult* PG = (PG_Party_LtoC_GetMemberInfoResult*) Data;

	if( PG->ID == 0 )
		List.clear();

	if( PG->TotalCount != 0 )
		List.push_back( PG->Member );


	if( List.size() >= PG->TotalCount )
	{
		This->R_GetMemberInfoResult( PG->LeaderDBID , List );
		List.clear();
	}
}
void NetCli_Party::_PG_ZoneParty_LtoC_Talk				( int NetID , int Size , void* Data )
{
	PG_ZoneParty_LtoC_Talk* PG = (PG_ZoneParty_LtoC_Talk*) Data;
	This->R_Zone_Talk( PG->GItemID , PG->Content.Begin() );
}
void NetCli_Party::_PG_ZoneParty_LtoC_ChangeLeaderResult( int NetID , int Size , void* Data )
{
	PG_ZoneParty_LtoC_ChangeLeaderResult* PG = (PG_ZoneParty_LtoC_ChangeLeaderResult*) Data;
	This->R_Zone_ChangeLeaderResult( PG->Result );
}
void NetCli_Party::_PG_ZoneParty_LtoC_PartyBase( int NetID , int Size , void* Data )
{
	PG_ZoneParty_LtoC_PartyBase* PG = (PG_ZoneParty_LtoC_PartyBase*) Data;
	This->R_Zone_PartyBase( PG->PartyID , PG->LeaderDBID );
}
void NetCli_Party::_PG_ZoneParty_LtoC_SetMemberPosResult( int NetID , int Size , void* Data )
{
	PG_ZoneParty_LtoC_SetMemberPosResult* PG = (PG_ZoneParty_LtoC_SetMemberPosResult*) Data;
	This->R_Zone_SetMemberPosRequest( PG->Result );
}
void NetCli_Party::_PG_ZoneParty_LtoC_Invite		( int NetID , int Size , void* Data )
{
	PG_ZoneParty_LtoC_Invite* PG = (PG_ZoneParty_LtoC_Invite*) Data;
	This->R_Zone_Invite( PG->LeaderDBID , PG->ClientInfo );
}
void NetCli_Party::_PG_ZoneParty_LtoC_AddMember		( int NetID , int Size , void* Data )
{
	PG_ZoneParty_LtoC_AddMember* PG = (PG_ZoneParty_LtoC_AddMember*) Data;
	This->R_Zone_AddMember( PG->PartyID , PG->MemberDBID , PG->PartyNo , PG->MemberID );
}
void NetCli_Party::_PG_ZoneParty_LtoC_DelMember		( int NetID , int Size , void* Data )
{
	PG_ZoneParty_LtoC_DelMember* PG = (PG_ZoneParty_LtoC_DelMember*) Data;
	This->R_Zone_DelMember( PG->MemberDBID );
}
void NetCli_Party::_PG_ZoneParty_LtoC_FixMember		( int NetID , int Size , void* Data )
{
	PG_ZoneParty_LtoC_FixMember* PG = (PG_ZoneParty_LtoC_FixMember*) Data;
	This->R_Zone_FixMember( PG->PartyID , PG->MemberDBID , PG->PartyNo , PG->MemberID );
}


void NetCli_Party::_PG_Party_ChattoC_SetMemberPosResult( int NetID , int Size , void* Data )
{
	PG_Party_ChattoC_SetMemberPosResult* PG = (PG_Party_ChattoC_SetMemberPosResult*) Data;
	This->R_SetMemberPosResult( PG->Result );
}

void NetCli_Party::_PG_Party_LtoC_MemberLogout			( int NetID , int Size , void* Data )
{
	PG_Party_LtoC_MemberLogout* PG = (PG_Party_LtoC_MemberLogout*) Data;
	This->R_MemberLogout( PG->MemberDBID );
}
void NetCli_Party::OnEvent_ServerDisConnect		( int ServerID )
{
    //////////////////////////////////////////////////////////////////////////
    // TODO: 改變解構順序, 使斷線的步驟不會發生錯誤.
    //////////////////////////////////////////////////////////////////////////

    if (This) // 安全使用指標, 暫時避開解構時的順序問題.
    {
        This->OnServerDisConnect( ServerID );
    }
    else
    {
        char szDebug[256];
        szDebug[255] = '\0';
        _snprintf(szDebug, 255, "NetCli_Party::OnEvent_ServerDisConnect(%d) Error : This == NULL !\n", ServerID);
        OutputDebugString(szDebug);
    }
}
void NetCli_Party::_PG_Party_ChattoC_GiveItemInfo		( int NetID , int Size , void* Data )
{
	PG_Party_ChattoC_GiveItemInfo* PG = (PG_Party_ChattoC_GiveItemInfo*) Data;
	This->R_GiveItemInfo( PG->DBID , PG->Item , PG->Version );
}

void NetCli_Party::_PG_Party_ChattoC_Invite			( int NetID , int Size , void* Data )
{
	PG_Party_ChattoC_Invite* PG = (PG_Party_ChattoC_Invite*) Data;
	This->R_Invite( (char*)PG->LeaderName.Begin() , PG->ClientInfo );
}

void NetCli_Party::_PG_Party_CtoC_AskJoinToParty	( void *Data , int Size )
{
	PG_Party_CtoC_AskJoinToParty* PG = (PG_Party_CtoC_AskJoinToParty*) Data;
	This->R_AskJoinToParty( (char*)PG->AskerName.Begin() , PG->sJobInfo );
}

void NetCli_Party::_PG_Party_CtoC_AskJoinToPartyResult( void *Data , int Size )
{
	PG_Party_CtoC_AskJoinToPartyResult* PG = (PG_Party_CtoC_AskJoinToPartyResult*) Data;
	This->R_AskJoinToPartyResult( (char*)PG->LeaderName.Begin() , PG->bYesNo );
}

void NetCli_Party::_PG_Party_CtoC_DeletePBPost( void *Data , int Size )
{
	This->R_DeletePBPost();
}

void NetCli_Party::_PG_Party_ChattoC_PartyBaseInfo	( int NetID , int Size , void* Data )
{
	PG_Party_ChattoC_PartyBaseInfo* PG = (PG_Party_ChattoC_PartyBaseInfo*) Data;
	This->R_PartyBaseInfo( PG->Info );
}
void NetCli_Party::_PG_Party_ChattoC_AddMember		( int NetID , int Size , void* Data )
{
	PG_Party_ChattoC_AddMember* PG = (PG_Party_ChattoC_AddMember*) Data;
	This->R_AddMember( PG->NewMember );
}
void NetCli_Party::_PG_Party_ChattoC_DelMember		( int NetID , int Size , void* Data )
{
	PG_Party_ChattoC_DelMember* PG = (PG_Party_ChattoC_DelMember*) Data;
	This->R_DelMember( PG->Member );
}
void NetCli_Party::_PG_Party_ChattoC_FixMember		( int NetID , int Size , void* Data )
{
	PG_Party_ChattoC_FixMember* PG = (PG_Party_ChattoC_FixMember*) Data;
	This->R_FixMember( PG->Member );
}
void NetCli_Party::_PG_Party_ChattoC_PartyTreasure		( int NetID , int Size , void* Data )
{
	PG_Party_ChattoC_PartyTreasure* PG = (PG_Party_ChattoC_PartyTreasure*) Data;
	This->R_PartyTreasure	( PG->ItemVersion , PG->LootType , PG->Item , PG->LootDBID );
}
void NetCli_Party::_PG_Party_ChattoC_PartyTreasureLoot	( int NetID , int Size , void* Data )
{
	PG_Party_ChattoC_PartyTreasureLoot* PG = (PG_Party_ChattoC_PartyTreasureLoot*) Data;
	This->R_PartyTreasureLoot( PG->ItemVersion , PG->DBID , PG->LootValue );

}
//-------------------------------------------------------------------
void NetCli_Party::S_Invite		( char* Name , int ClientInfo )
{
	PG_Party_CtoChat_Invite Send;
	Send.InviteName = Name;
	Send.ClientInfo = ClientInfo;
	SendToChat( sizeof( Send ) , &Send );
}
void NetCli_Party::S_AskJoinToParty( char* Name )
{
	RoleDataEx*	pRole = CNetGlobal::RoleData();

	PG_Party_CtoC_AskJoinToParty Send;
	Send.AskerName				= g_pGameMain->GetPlayerName();
	Send.sJobInfo.Voc			= pRole->BaseData.Voc;
	Send.sJobInfo.VocLV			= pRole->Level();
	Send.sJobInfo.Voc_Sub		= pRole->BaseData.Voc_Sub;
	Send.sJobInfo.Voc_SubLV		= pRole->Level_Sub();

	NetCli_Channel::S_DataTransmitToCli_Name( Name, &Send, sizeof(Send) );
}
void NetCli_Party::S_AskJoinToPartyResult( char* Name, bool bYesNo )
{
	PG_Party_CtoC_AskJoinToPartyResult Send;
	Send.LeaderName = g_pGameMain->GetPlayerName();
	Send.bYesNo		= bYesNo;

	NetCli_Channel::S_DataTransmitToCli_Name( Name, &Send, sizeof(Send) );
}
void NetCli_Party::S_DeletePBPost( char* Name )
{
	PG_Party_CtoC_DeletePBPost Send;

	NetCli_Channel::S_DataTransmitToCli_Name( Name, &Send, sizeof(Send) );
}
void NetCli_Party::S_Join		( char* LeaderName )
{
	PG_Party_CtoChat_Join Send;
	Send.LeaderName = LeaderName;
	SendToChat( sizeof( Send ) , &Send );
}
void NetCli_Party::S_KickMember	( char* MemberName , int PartyID )
{
	PG_Party_CtoChat_KickMember Send;
	Send.MemberName = MemberName;
	Send.PartyID = PartyID;
	SendToChat( sizeof( Send ) , &Send );
}
void NetCli_Party::S_PartyRule	( PartyBaseInfoStruct& PartyInfo )
{
	PG_Party_CtoChat_PartyRule Send;
	Send.PartyInfo = PartyInfo;
	SendToChat( sizeof( Send ) , &Send );
}

void NetCli_Party::S_LootItem( int ItemVersion )
{
	PG_Party_CtoChat_PartyTreasureLoot Send;
	Send.DBID = -1;
	Send.Type = EM_PartyTreasureLootType_Loot;
	Send.ItemVersion = ItemVersion;
	SendToChat( sizeof( Send ) , &Send );
}
void NetCli_Party::S_PassItem( int ItemVersion )
{
	PG_Party_CtoChat_PartyTreasureLoot Send;
	Send.DBID = -1;
	Send.Type = EM_PartyTreasureLootType_Pass;
	Send.ItemVersion = ItemVersion;
	SendToChat( sizeof( Send ) , &Send );
}
void NetCli_Party::S_AssistItem( int ItemVersion , int DBID )
{
	PG_Party_CtoChat_PartyTreasureLoot Send;
	Send.DBID = DBID;
	Send.Type = EM_PartyTreasureLootType_Assist;
	Send.ItemVersion = ItemVersion;
	SendToChat( sizeof( Send ) , &Send );
}
void NetCli_Party::S_LootItem_NotNeed( int ItemVersion )
{
	PG_Party_CtoChat_PartyTreasureLoot Send;
	Send.Type = EM_PartyTreasureLootType_Loot_NotNeed;
	Send.ItemVersion = ItemVersion;
	SendToChat( sizeof( Send ) , &Send );
}
void NetCli_Party::SChat_SetMemberPosRequest	( int MemberDBID1 , int MemberDBID2 , int MemberPos1 , int MemberPos2 )
{
	PG_Party_CtoChat_SetMemberPosRequest Send;
	Send.MemberDBID[0] = MemberDBID1;
	Send.MemberDBID[1] = MemberDBID2;
	Send.Pos[0] = MemberPos1;
	Send.Pos[1] = MemberPos2;
	SendToChat( sizeof( Send ) , &Send );
}
void NetCli_Party::SChat_SetMemberMode			( int MemberDBID ,	PartyMemberModeStruct	Mode )
{
	PG_Party_CtoChat_SetMemberMode Send;
	Send.MemberDBID = MemberDBID;
	Send.Mode._Mode = Mode._Mode;
	SendToChat( sizeof( Send ) , &Send );
}

void NetCli_Party::S_Zone_Invite				( int InviteDBID , int ClientInfo )
{
	PG_ZoneParty_CtoL_Invite Send;
	Send.InviteDBID = InviteDBID;
	Send.ClientInfo = ClientInfo;
	SendToLocal( sizeof( Send ) , &Send );
}
void NetCli_Party::S_Zone_Join					( int LeaderDBID )
{
	PG_ZoneParty_CtoL_Join Send;
	Send.LeaderDBID = LeaderDBID;
	SendToLocal( sizeof( Send ) , &Send );
}
void NetCli_Party::S_Zone_KickMember			( int MemberDBID )
{
	PG_ZoneParty_CtoL_KickMember Send;
	Send.MemberDBID = MemberDBID;
	SendToLocal( sizeof( Send ) , &Send );
}
void NetCli_Party::S_Zone_SetMemberPosRequest	( int MemberDBID1 , int Pos1 , int MemberDBID2 , int Pos2 )
{
	PG_ZoneParty_CtoL_SetMemberPosRequest Send;
	Send.MemberDBID[0] = MemberDBID1;
	Send.MemberDBID[1] = MemberDBID2;
	Send.Pos[0] = Pos2;
	Send.Pos[1] = Pos1;

	SendToLocal( sizeof( Send ) , &Send );
}

void NetCli_Party::S_Zone_ChangeLeader			( int NewLeaderDBID )
{
	PG_ZoneParty_CtoL_ChangeLeader Send;
	Send.NewLeaderDBID = NewLeaderDBID;
	SendToLocal( sizeof( Send ) , &Send );
}
void NetCli_Party::S_Zone_Talk					( const char* Content )
{
	PG_ZoneParty_CtoL_Talk Send;
	Send.Content = Content;
	SendToLocal( sizeof( Send ) , &Send );
}
void NetCli_Party::SL_GetMemberInfo		( int LeaderDBID )
{
	PG_Party_CtoL_GetMemberInfo Send;
	Send.LeaderDBID = LeaderDBID;
	SendToLocal( sizeof( Send ) , &Send );
}
