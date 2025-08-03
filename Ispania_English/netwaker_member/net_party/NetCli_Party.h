#pragma once

#include "PG/PG_Party.h"
#include "../../MainProc/NetGlobal.h"

class NetCli_Party : public CNetGlobal
{
protected:
    //-------------------------------------------------------------------
    static NetCli_Party* This;
    static bool _Init();
    static bool _Release();
    //-------------------------------------------------------------------

	static void _PG_Party_ChattoC_Invite			( int NetID , int Size , void* Data );
	static void _PG_Party_CtoC_AskJoinToParty		( void *Data , int Size );
	static void _PG_Party_CtoC_AskJoinToPartyResult	( void *Data , int Size );
	static void _PG_Party_CtoC_DeletePBPost			( void *Data , int Size );
	static void _PG_Party_ChattoC_PartyBaseInfo		( int NetID , int Size , void* Data );
	static void _PG_Party_ChattoC_AddMember			( int NetID , int Size , void* Data );
	static void _PG_Party_ChattoC_DelMember			( int NetID , int Size , void* Data );
	static void _PG_Party_ChattoC_FixMember			( int NetID , int Size , void* Data );

	static void _PG_Party_ChattoC_PartyTreasure		( int NetID , int Size , void* Data );
	static void _PG_Party_ChattoC_PartyTreasureLoot	( int NetID , int Size , void* Data );
	static void _PG_Party_ChattoC_GiveItemInfo		( int NetID , int Size , void* Data );
	static void _PG_Party_LtoC_MemberLogout			( int NetID , int Size , void* Data );
	static void _PG_Party_ChattoC_SetMemberPosResult( int NetID , int Size , void* Data );

	static void _PG_ZoneParty_LtoC_Invite			( int NetID , int Size , void* Data );
	static void _PG_ZoneParty_LtoC_AddMember		( int NetID , int Size , void* Data );
	static void _PG_ZoneParty_LtoC_DelMember		( int NetID , int Size , void* Data );
	static void _PG_ZoneParty_LtoC_FixMember		( int NetID , int Size , void* Data );
	static void _PG_ZoneParty_LtoC_SetMemberPosResult( int NetID , int Size , void* Data );
	static void _PG_ZoneParty_LtoC_PartyBase		( int NetID , int Size , void* Data );
	static void _PG_ZoneParty_LtoC_ChangeLeaderResult( int NetID , int Size , void* Data );
	static void _PG_ZoneParty_LtoC_Talk				( int NetID , int Size , void* Data );

	static void _PG_Party_LtoC_GetMemberInfoResult	( int NetID , int Size , void* Data );

	static void		OnEvent_ServerDisConnect		( int ServerID );
public:    
    NetCli_Party() { This = this; }

	
	static void S_Invite		( char* Name , int ClientInfo );
	static void S_AskJoinToParty( char* Name );
	static void S_AskJoinToPartyResult( char* Name, bool bYesNo );
	static void S_DeletePBPost  ( char* Name );
	static void S_Join			( char* LeaderName );
	static void S_KickMember	( char* MemberName , int PartyID );
	static void S_PartyRule		( PartyBaseInfoStruct& PartyInfo );

	static void SChat_SetMemberPosRequest	( int MemberDBID1 , int MemberDBID2 , int MemberPos1 , int MemberPos2 );
	static void SChat_SetMemberMode			( int MemberDBID ,	PartyMemberModeStruct	Mode );

 	virtual void R_Invite			( char* LeaderName , int ClientInfo ) = 0;
	virtual void R_AskJoinToParty	( char* szAskerName , SJobInfo sJobInfo ) = 0;
	virtual void R_AskJoinToPartyResult	( char* szLeaderName , bool bYesNo ) = 0;
	virtual void R_DeletePBPost		() = 0;
	virtual void R_PartyBaseInfo	( PartyBaseInfoStruct& Info ) = 0;
	virtual void R_AddMember		( PartyMemberInfoStruct& Member ) = 0;
	virtual void R_DelMember		( PartyMemberInfoStruct& Member ) = 0;
	virtual void R_FixMember		( PartyMemberInfoStruct& Member ) = 0;


	static void	S_LootItem( int ItemVersion );
	static void	S_LootItem_NotNeed( int ItemVersion );
	static void	S_PassItem( int ItemVersion );
	static void S_AssistItem( int ItemVersion , int DBID );

	virtual void R_PartyTreasure( int ItemVersion , PartyItemShareENUM LootType , ItemFieldStruct& Item ,	StaticVector< int , _MAX_PARTY_COUNT_  >&  LootDBID ) = 0;
	virtual void R_PartyTreasureLoot( int ItemVersion ,	int DBID , int LootValue ) = 0;
	virtual void R_GiveItemInfo( int DBID , ItemFieldStruct& Item , int ItemVersion ) = 0;

	virtual void OnServerDisConnect		( int ServerID ) = 0;
	virtual void R_MemberLogout			( int MemberDBID ) = 0;
	virtual void R_SetMemberPosResult	( bool Result ) = 0;

	//////////////////////////////////////////////////////////////////////////
	static  void SL_GetMemberInfo		( int LeaderDBID );
	virtual void R_GetMemberInfoResult	( int LeaderDBID , vector< PartyMemberInfoStruct > & List ) = 0;
	//-------------------------------------------------------------------
	//¾Ô³õ°Ï°ì Party
	//-------------------------------------------------------------------
	static void	S_Zone_Invite				( int InviteDBID , int ClientInfo );
	static void	S_Zone_Join					( int LeaderDBID );
	static void	S_Zone_KickMember			( int MemberDBID );
	static void S_Zone_SetMemberPosRequest	( int MemberDBID1 , int Pos1 , int MemberDBID2 , int Pos2 );
	static void S_Zone_ChangeLeader			( int NewLeaderDBID );
	static void S_Zone_Talk					( const char* Content );

	virtual void R_Zone_PartyBase	( int PartyID , int LeaderDBID ) = 0;
	virtual void R_Zone_Invite		( int LeaderDBID , int ClientInfo ) = 0;
	virtual void R_Zone_AddMember	( int PartyID , int MemberDBID , int PartyNo , int MemberID ) = 0;
	virtual void R_Zone_DelMember	( int MemberDBID ) = 0;
	virtual void R_Zone_FixMember	( int PartyID , int MemberDBID , int PartyNo , int MemberID ) = 0;
	virtual void R_Zone_SetMemberPosRequest	( bool Result ) = 0;
	virtual void R_Zone_ChangeLeaderResult( bool Result ) = 0;
	virtual void R_Zone_Talk		( int GItemID , const char* Content ) = 0;

};

