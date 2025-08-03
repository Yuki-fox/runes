#include "NetCli_FriendList.h"

//--------------------------------------------------------------------------------------------------------------------------------------
NetCli_FriendList*    NetCli_FriendList::This         = NULL;

//--------------------------------------------------------------------------------------------------------------------------------------
bool NetCli_FriendList::_Init()
{
	/*
	_Net.RegOnSrvPacketFunction	( EM_PG_FriendList_LtoC_InsertFriendResult		, _PG_FriendList_LtoC_InsertFriendResult		 );
	_Net.RegOnSrvPacketFunction	( EM_PG_FriendList_ChattoC_PlayerOnlineInfo		, _PG_FriendList_ChattoC_PlayerOnlineInfo		 );
	_Net.RegOnSrvPacketFunction	( EM_PG_FriendList_ChattoC_PlayerOfflineInfo	, _PG_FriendList_ChattoC_PlayerOfflineInfo	     );
	_Net.RegOnSrvPacketFunction	( EM_PG_FriendList_LtoC_EraseFriendResult		, _PG_FriendList_LtoC_EraseFriendResult		     );
	_Net.RegOnSrvPacketFunction	( EM_PG_FriendList_LtoC_ModifyFriendResult		, _PG_FriendList_LtoC_ModifyFriendResult		 );
	_Net.RegOnSrvPacketFunction	( EM_PG_FriendList_LtoC_FriendListInfo_Zip		, _PG_FriendList_LtoC_FriendListInfo_Zip		 );
	_Net.RegOnSrvPacketFunction	( EM_PG_FriendList_LtoC_FixFriendInfo			, _PG_FriendList_LtoC_FixFriendInfo				 );
	_Net.RegOnSrvPacketFunction	( EM_PG_Friend_LtoC_SearchFindPartyPlayerResult	 , _PG_Friend_LtoC_SearchFindPartyPlayerResult		);
*/
	Cli_NetReg( PG_FriendList_LtoC_InsertFriendResult		);
//	Cli_NetReg( PG_FriendList_ChattoC_PlayerOnlineInfo		);
	_Net.RegOnSrvPacketFunction( EM_PG_FriendList_ChattoC_PlayerOnlineInfo , _PG_FriendList_ChattoC_PlayerOnlineInfo , 10000 );
//	Cli_NetReg( PG_FriendList_ChattoC_PlayerOfflineInfo		);
	_Net.RegOnSrvPacketFunction( EM_PG_FriendList_ChattoC_PlayerOfflineInfo , _PG_FriendList_ChattoC_PlayerOfflineInfo , 10000 );
	Cli_NetReg( PG_FriendList_LtoC_EraseFriendResult		);
	Cli_NetReg( PG_FriendList_LtoC_ModifyFriendResult		);
	Cli_NetReg( PG_FriendList_LtoC_FriendListInfo_Zip		);
	Cli_NetReg( PG_FriendList_LtoC_FixFriendInfo			);
	Cli_NetReg( PG_Friend_LtoC_SearchFindPartyPlayerResult	);

    return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------
bool NetCli_FriendList::_Release()
{
    return true;
}
//--------------------------------------------------------------------------------------------------------------------------------------
void NetCli_FriendList::_PG_FriendList_LtoC_FixFriendInfo		( int Sockid , int Size , void* Data )
{
	PG_FriendList_LtoC_FixFriendInfo*  PG = (PG_FriendList_LtoC_FixFriendInfo *)Data;
	This->RL_FixFriendInfo( PG->Type , PG->Pos , PG->Info );
}
void NetCli_FriendList::_PG_FriendList_LtoC_InsertFriendResult	( int Sockid , int Size , void* Data )
{
	PG_FriendList_LtoC_InsertFriendResult*  PG = (PG_FriendList_LtoC_InsertFriendResult *)Data;
	This->RL_InsertFriendResult( PG->Type , PG->Pos , PG->Result );
}
void NetCli_FriendList::_PG_FriendList_ChattoC_PlayerOnlineInfo	( int Sockid , int Size , void* Data )
{
	PG_FriendList_ChattoC_PlayerOnlineInfo *  PG = (PG_FriendList_ChattoC_PlayerOnlineInfo *)Data;
	This->RChat_PlayerOnlineInfo( PG->PlayerDBID , (char*)PG->RoleName.Begin() , PG->Race , PG->Voc , PG->Voc_Sub , PG->Sex , PG->LV , PG->LV_Sub , PG->GuildID , PG->TitleID );
}
void NetCli_FriendList::_PG_FriendList_ChattoC_PlayerOfflineInfo( int Sockid , int Size , void* Data )
{
	PG_FriendList_ChattoC_PlayerOfflineInfo*  PG = (PG_FriendList_ChattoC_PlayerOfflineInfo *)Data;
	This->RChat_PlayerOfflineInfo( PG->PlayerDBID );
}
void NetCli_FriendList::_PG_FriendList_LtoC_EraseFriendResult	( int Sockid , int Size , void* Data )
{
	PG_FriendList_LtoC_EraseFriendResult*  PG = (PG_FriendList_LtoC_EraseFriendResult *)Data;
	This->RL_EraseFriendResult( PG->Type , PG->DBID , PG->Result );
}
void NetCli_FriendList::_PG_FriendList_LtoC_ModifyFriendResult	( int Sockid , int Size , void* Data )
{
	PG_FriendList_LtoC_ModifyFriendResult*  PG = (PG_FriendList_LtoC_ModifyFriendResult *)Data;
	This->RL_ModifyFriendResult( PG->Type , PG->DBID , PG->Result );
}
void NetCli_FriendList::_PG_FriendList_LtoC_FriendListInfo_Zip	( int Sockid , int Size , void* Data )
{
	PG_FriendList_LtoC_FriendListInfo_Zip*  PG = (PG_FriendList_LtoC_FriendListInfo_Zip *)Data;

	FriendListStruct FriendList;
	MyLzoClass myZip;
	int RetSize = myZip.Decode( PG->Data , PG->Size , sizeof(FriendList) , (char*)&FriendList );

	assert( RetSize != 0 );

	This->RL_FriendListInfo_Zip( FriendList );
}

void NetCli_FriendList::OnEvent_ServerDisConnect		( int ServerID )
{
	This->OnServerDisConnect( ServerID );
}

void NetCli_FriendList::SL_InsertFriend			( FriendListTypeENUM Type , int Pos , BaseFriendStruct& Info )
{
	PG_FriendList_CtoL_InsertFriend Send;
	Send.Type = Type;
	Send.Pos = Pos;
	Send.Info = Info;
	SendToLocal_Base(  sizeof(Send) , &Send );
}
void NetCli_FriendList::SL_EraseFriend			( FriendListTypeENUM Type , int DBID )
{
	PG_FriendList_CtoL_EraseFriend Send;
	Send.Type = Type;
	Send.DBID = DBID;
	SendToLocal_Base(  sizeof(Send) , &Send );
}
void NetCli_FriendList::SL_ModifyFriend			( FriendListTypeENUM Type , BaseFriendStruct& Info )
{
	PG_FriendList_CtoL_ModifyFriend Send;
	Send.Type = Type;
	Send.Info = Info;
	SendToLocal_Base(  sizeof(Send) , &Send );
}
void NetCli_FriendList::SL_ModifyNote			( const char* Note )
{
	PG_FriendList_CtoL_ModifyNote Send;
	Send.Note = Note;
	SendToLocal_Base(  sizeof(Send) , &Send );
}

void NetCli_FriendList::SL_FixClientData		( int Pos , char* CliData )
{
	PG_FriendList_CtoL_FixClientData Send;
	Send.Pos = Pos;
	memcpy( Send.CliData , CliData , sizeof(Send.CliData) );
	SendToLocal_Base(  sizeof(Send) , &Send );
}

void NetCli_FriendList::SL_FixGroupSortID		( char	GroupSortID[ _MAX_FRIENDLIST_GROUP_COUNT_ ] )
{
	PG_FriendList_CtoL_FixGroupSortID Send;
	memcpy( Send.GroupSortID , GroupSortID , sizeof(Send.GroupSortID) );
	SendToLocal_Base(  sizeof(Send) , &Send );
}

void NetCli_FriendList::_PG_Friend_LtoC_SearchFindPartyPlayerResult( int Sockid , int Size , void* Data )
{
	static vector<SearchFindPartyPlayerStruct> PlayerList;

	PG_Friend_LtoC_SearchFindPartyPlayerResult *  PG = (PG_Friend_LtoC_SearchFindPartyPlayerResult *)Data;

	if( PG->Index == 0 )
		PlayerList.clear();

	if( PG->Count == 0 )
	{
		This->RL_SearchFindPartyPlayerResult( PlayerList );
		return;
	}
	
	PlayerList.push_back( PG->Info );
	if( (int)PlayerList.size() >= PG->Count )
	{
		This->RL_SearchFindPartyPlayerResult( PlayerList );
		return;
	}
}
void NetCli_FriendList::SL_SearchFindPartyPlayerRequest( Voc_ENUM MainJob , Voc_ENUM SubJob , int MinMainJobLv , int MaxMainJobLv , int MinSubJobLv , int MaxSubJobLv )
{
	PG_Friend_CtoL_SearchFindPartyPlayerRequest Send;
	Send.MainJob = MainJob;
	Send.SubJob = SubJob;		
	Send.MinMainJobLv = MinMainJobLv;
	Send.MaxMainJobLv = MaxMainJobLv;
	Send.MinSubJobLv = MinSubJobLv;
	Send.MaxSubJobLv = MaxSubJobLv;
	SendToLocal_Base( sizeof(Send) , &Send );
}

/*

void NetCli_FriendList::_PG_Friend_ChattoC_PlayerOnline		( int Sockid , int Size , void* Data )
{
	PG_Friend_ChattoC_PlayerOnline *  PG = (PG_Friend_ChattoC_PlayerOnline *)Data;

	This->RChat_PlayerOnline( PG->Info );
}
void NetCli_FriendList::_PG_Friend_ChattoC_PlayerOffline	( int Sockid , int Size , void* Data )
{
	PG_Friend_ChattoC_PlayerOffline *  PG = (PG_Friend_ChattoC_PlayerOffline *)Data;
	This->RChat_PlayerOffline( (char*)PG->RoleName.Begin() );

}
void NetCli_FriendList::_PG_Friend_ChattoC_ModifyInfo		( int Sockid , int Size , void* Data )
{
	PG_Friend_ChattoC_ModifyInfo *  PG = (PG_Friend_ChattoC_ModifyInfo *)Data;
//	This->RChat_ModifyInfo(  (char*)PG->RoleName.Begin() , (char*)PG->Note.Begin() , PG->ZoneID , PG->Voc , PG->Voc_Sub , PG->LV , PG->LV_Sub , PG->Sex );
	This->RChat_ModifyInfo( PG->Info );

}

void NetCli_FriendList::_PG_Friend_ChattoC_BefriendRequest	( int Sockid , int Size , void* Data )
{
	PG_Friend_ChattoC_BefriendRequest *  PG = (PG_Friend_ChattoC_BefriendRequest *)Data;
	This->RChat_BefriendRequest( (char*)PG->Name.Begin() );
}
void NetCli_FriendList::_PG_Friend_LtoC_AddFriend			( int Sockid , int Size , void* Data )		
{
	PG_Friend_LtoC_AddFriend *  PG = (PG_Friend_LtoC_AddFriend *)Data;
	This->RL_AddFriend( (char*)PG->FriendName.Begin() );
}
void NetCli_FriendList::_PG_Friend_LtoC_DelFriend			( int Sockid , int Size , void* Data )		
{
	PG_Friend_LtoC_DelFriend *  PG = (PG_Friend_LtoC_DelFriend *)Data;
	This->RL_DelFriend( (char*)PG->FriendName.Begin(), PG->bBackGound );
}
void NetCli_FriendList::_PG_Friend_LtoC_FriendList			( int Sockid , int Size , void* Data )
{
	PG_Friend_LtoC_FriendList *  PG = (PG_Friend_LtoC_FriendList *)Data;
	This->RL_FriendList( PG->FriendList );
}
//--------------------------------------------------------------------------------------------------------------------------------------
void NetCli_FriendList::_PG_Friend_LtoC_AddBadFriendResult			( int Sockid , int Size , void* Data )
{
	PG_Friend_LtoC_AddBadFriendResult *  PG = (PG_Friend_LtoC_AddBadFriendResult *)Data;
	This->RL_AddBadFriendResult( PG->FriendName.Begin(), PG->Result );
}
//--------------------------------------------------------------------------------------------------------------------------------------
void NetCli_FriendList::_PG_Friend_LtoC_DelBadFriendResult			( int Sockid , int Size , void* Data )
{
	PG_Friend_LtoC_AddBadFriendResult *  PG = (PG_Friend_LtoC_AddBadFriendResult *)Data;
	This->RL_DelBadFriendResult( PG->FriendName.Begin(), PG->Result );
}
//--------------------------------------------------------------------------------------------------------------------------------------
void NetCli_FriendList::_PG_Friend_ChattoC_BefriendAnswer			( int Sockid , int Size , void* Data )
{
	PG_Friend_ChattoC_BefriendAnswer *  PG = (PG_Friend_ChattoC_BefriendAnswer *)Data;
	This->RL_BefriendAnswer( PG->iResult, PG->Name.Begin() );
}
//--------------------------------------------------------------------------------------------------------------------------------------
void NetCli_FriendList::SChat_BefriendRequest	( char* Name )
{
	PG_Friend_CtoChat_BefriendRequest Send;
	Send.Name = Name;

	SendToChat( sizeof(Send) , &Send );
}
void NetCli_FriendList::SChat_BefriendResult	( char* Name , bool Result )
{
	PG_Friend_CtoChat_BefriendResult Send;
	Send.Name = Name;
	Send.Result = Result;

	SendToChat( sizeof(Send) , &Send );
}
void NetCli_FriendList::SL_DelFriend			( char* Name, bool bBackGound )
{
	PG_Friend_CtoL_DelFriend Send;
	Send.FriendName = Name;
	Send.bBackGound	= bBackGound;

	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_FriendList::SL_ModifyMyInfo			( char* Note )
{
	PG_Friend_CtoL_ModifyMyInfo Send;
	Send.Note = Note;

	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_FriendList::SL_AddBadFriend			( char* Name, bool bCheckRole )
{
	PG_Friend_CtoL_AddBadFriend Send;

	Send.RoleName			= Name;
	Send.IsNeedCheckRole	= bCheckRole;

	SendToLocal( sizeof(Send) , &Send );
}
void NetCli_FriendList::SL_DelBadFriend			( char* Name )
{
	PG_Friend_CtoL_DelBadFriend Send;
	Send.RoleName = Name;

	SendToLocal( sizeof(Send) , &Send );
}

void NetCli_FriendList::SL_SetFriendInfo		( const char* Name , const char* Group , const char* Note )
{
	PG_Friend_CtoL_SetFriendInfo Send;
	Send.FriendName = Name;
	Send.Type = Group;
	Send.Note = Note;
	SendToLocal( sizeof(Send) , &Send );
}

void NetCli_FriendList::SL_SetBadFriendInfo		( const char* Name, const char* Note )
{
	PG_Friend_CtoL_SetBadFriendInfo Packet;

	Packet.FriendName	= Name;
	Packet.Note			= Note;

	SendToLocal( sizeof(Packet) , &Packet );
}
*/