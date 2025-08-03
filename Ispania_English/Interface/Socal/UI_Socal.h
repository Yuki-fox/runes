#pragma once

#include "../interface.h"
#include "PG/PG_FriendList.h"

#include "PG/PG_ChannelBase.h"


class CUI_Socal;
extern CUI_Socal*		g_pUi_Socal;

enum ESocalUnitType
{
	SUT_Friend     = 0,
	SUT_BadFriend  = 1,
	SUT_HateFriend = 2,
};


class CUI_Socal : public CInterfaceBase
{
public:

	CUI_Socal	( CInterface* pObject );
	~CUI_Socal	(void);


	virtual void RegisterFunc	();	
	virtual void Update(float elapsedTime);
	virtual void DestroyFrames();
	virtual	void LeaveWorld();

 	void UpdateAddFriendList( );

	//設朋友頭像
	void SetFriendModel( int nFriendID , const char* szUIModel );
	//bool GetSelfFriendInfo( StructFriendInfo& rInfo );

	bool GetSelfModelInfo( SModelInfo& rModelInfo );

	void FriendListLoadOK(){ m_FriendListLoadOK = true; }
	bool IsFriendListLoadOK(){ return m_FriendListLoadOK; } 
	
	void RChat_PlayerOnlineInfo( int PlayerDBID , char* RoleName , Voc_ENUM Voc , Voc_ENUM Voc_Sub , Sex_ENUM Sex
								, int LV , int LV_Sub , int GuildID , int TitleID );
	void RChat_PlayerOfflineInfo( int PlayerDBID );

	void GetModleInfoRequest( const char* pTargetName );
	void R_CtoC_GetModleInfoResult( int DBID , SModelInfo& ModelInfo );

 	void ViewFriendInfoRequest( ESocalUnitType type, const char* name );

 	void R_CtoC_GiveYouMyFriendInfo( const char* pName, SFriendInfo& FriendInfo );

	void R_CtoC_AmIYourFriendResult( FriendListTypeENUM FriendListType, const char* AskerName, const char* AnswererName, bool Result );

	bool IsMyFriend( const char* name, FriendListTypeENUM Type = EM_FriendListType_Friend );

	void SL_SearchFindPartyPlayerRequest( Voc_ENUM MainJob , Voc_ENUM SubJob , int MinMainJobLv , int MaxMainJobLv , int MinSubJobLv , int MaxSubJobLv );
	void RL_SearchFindPartyPlayerResult( vector<SearchFindPartyPlayerStruct>& PlayerInfo );


	void RL_FixFriendInfo( FriendListTypeENUM Type , int Pos , BaseFriendStruct& Info );	
	//map< int, SSocalFriendInfo > m_mapFriendInfo;
	
	//SSocalFriendType m_FriendTypes[DF_FriendTypeCount];

    static vector< SearchFindPartyPlayerStruct > m_vecSearchResult;
public:
	void AddFriend( ESocalUnitType type, const char* name, int groupID );
	void DelFriend( ESocalUnitType type, const char* name, RelationTypeENUM relationType );

	void AddSocalGroup( ESocalUnitType type, const char* Name );
	void DelSocalGroup( ESocalUnitType type, int groupID );
	void ModifyGroupName( ESocalUnitType type, int groupID, const char* newName );
	void ModifyGroupSort( ESocalUnitType type, int groupID, int sort );

	void PlayerKillMe( int DBID, const char* Name );  //被殺
	void KillPlayer( int DBID, const char* Name );	  //殺人

	void SetIsDisablePVPRule( bool IsDisable ){ m_IsDisablePVPRule =IsDisable; };
	bool IsDisablePVPRule(){ return m_IsDisablePVPRule; };
protected:
	bool m_IsDisablePVPRule;
protected:
	void _AddFriend ( int DBID, const char* Name, int groupID );
	void _AddBadFriend ( int DBID, const char* Name );
	void _AddHateFriend( int DBID, const char* Name );

  	void _ShowFriendDetail( const char* Name );
public:
	string GetRelationString( RelationTypeENUM relationType );
 	RelationTypeENUM GetRelationENUM( string& strRelationType );
public:
	bool m_FriendListLoadOK;

	struct SAddFriend
	{
		ESocalUnitType Type;
		string Name;
		int GroupID;
	};

	std::list<SAddFriend> m_AddFriendList;

	std::set<int> m_PlayerOnlineSet;	
 	std::map< string, SFriendInfo > m_mapFriendDetail;
   	std::map< string, SModelInfo > m_mapModelInfo;

	//互為朋友
	std::set<int> m_EachOtherFriendSet;

	//登入時 會沒有脫離關係訊息 暫存起來 Update 再秀
	std::list<string> m_SystemMsgList;

};
