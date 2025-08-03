#pragma once

#include <string>
#include <vector>
#include <map>
#include "NetCli_Party.h"

#define MAX_PARTY_MEMBERS					6
#define MAX_GROUP_MEMBERS					36

using namespace std;

enum PartyStateInfo
{
	ePartyState_Solo		= 0,			// 沒有隊伍
	ePartyState_Receive,					// 受到邀請
	ePartyState_Leader,						// 隊伍領隊
	ePartyState_Member,						// 隊伍成員
	ePartyState_Join,						// 收到申請加入隊伍的要求
};

struct PartyBaseValueQueue
{
	PartyBaseValueQueue()
	{
		memset(baseValue, 0, sizeof(baseValue));
		memset(maxBaseValue, 0, sizeof(maxBaseValue));
	}
	int baseValue[4];
	int maxBaseValue[4];
};

struct PartyMemberInfo
{
	int			memberId;					// Server用隊員索引值
	int			raidNo;						// 團隊索引值(0 ~ 35)
	int			DBID;						// 玩家資料庫儲存編號
	void*		sprite;						// 指標索引值(儲存基本資料)

	// 特殊旗標容器
	union
	{
		int container;
		struct
		{
			bool	assistant	: 1;		// 助理
			bool	tank		: 1;		// 坦克
			bool	attack		: 1;		// 攻擊手
		};
	};

	// 玩家區域資訊
	union
	{
		int		GItemID;					// -1 代表下線狀態
		struct
		{
			short   ItemID;
			short   ZoneID;
		};
	};	

	int			channelID;					// 分流編號
};

// 戰場隊伍共有資訊
struct BattleGroundPartyBaseData
{
	int partyID;							// 隊伍代碼
	int leaderDBID;							// 領隊編號
};

// 戰場隊伍個人資訊
struct BattleGroundPartyMember
{
	int partyID;
	int partyNo;
	int memberID;
	int DBID;								// 	
	bool isExists;							// 隊員加入時Sprite物件是否還沒建立
};

struct PartyAssignItem
{	
	int				itemVersion;			// 物品鍵值(KEY WORD),用以判別處理項目
	ItemFieldStruct	item;					// 物品資料
	vector<int>		loot;					// 有權獲得物品玩家
};

enum LootItemStatus
{
	eLootItem_None							= 0,	// 未處理
	eLootItem_WaitRoll,						// 等待擲骰中
	eLootItem_Done,							// 已擲骰
	eLootItem_Release,						// 等待刪除
};

struct PartyRollItem
{
	int				itemVersion;			// 物品鍵值(KEY WORD),用以判別處理項目
	float			destructTime;			// 損壞消失時間
	float			time;					// 分配到數時間
	ItemFieldStruct	item;					// 物品資料
	LootItemStatus	status;					// 處理狀況(0未處理, 1等待擲中, 2已擲骰, 3等待刪除)
};

class NetCli_PartyChild : public NetCli_Party
{
public:
	static	PartyBaseInfoStruct			s_info;				// 組隊基本資訊
	static	vector<PartyMemberInfo>		s_member;			// 隊伍成員資料(包函所有小隊與團隊)
	static	vector<PartyMemberInfo*>	s_party;			// 小隊成員([0] 為玩家自己)	
	static	vector<PartyAssignItem>		s_assignItem;
	static	vector<PartyRollItem>		s_rollItem;
	static	PartyStateInfo				s_state;
	static	string						s_leaderName;
	static	int							s_leaderRaidIndex;	// 團隊隊長索引值
	static	int							s_playerNo;			// 玩家所在小隊編號
	static	map<int, PartyBaseValueQueue>	s_baseValueQueue;

	// battle raid 
	static	string						s_BattleLeaderName;			// 戰場小隊領隊名稱
	static	int							s_BattleLeaderDBID;			// 戰場小隊領隊DBID
	static	int							s_BattlePartyNo;			// 玩家所在小隊編號
	static	PartyStateInfo				s_BattleState;				// 玩家戰場小隊狀態
	static	BattleGroundPartyBaseData	s_BattleInfo;				// 戰場小隊資訊
	static	vector<BattleGroundPartyMember>		s_BattleMember;		// 戰場隊員資訊
	static	vector<BattleGroundPartyMember*>	s_BattleParty;		// 戰場小隊成員

	static	bool _Init();
	static	bool _Release();

	static	void _Clear();

	static	void						Update(float elapsedTime);
	static	PartyBaseInfoStruct&		GetPartyInfo()					{ return s_info; }
	static	PartyMemberInfo*			GetPartyMember(int index);
	static	PartyMemberInfo*			GetRaidMember(int index);
	static	PartyMemberInfo*			GetGroupMember(int index);
	static	PartyMemberInfo*			GetGroupMemberByDBID(int DBID);
	static	const char*					GetPartyLeaderName()			{ return s_leaderName.c_str(); }
	static	int							GetPartyLeaderIndex();
	static	int							GetRaidLeaderIndex();
	static	int							GetPlayerRaidNo()				{ return s_playerNo; }
	static	PartyStateInfo				GetPartyState()					{ return s_state; }
	static	void						UpdateLeaderInfo();
	static	void						ClearPartyInfo();
	static	int							InviteToParty(const char* name);
	static	int							AskJoinToParty(char* name);		// Cli to Cli 申請加入隊伍(送出方)
	static	int							AskJoinToPartyResult(char* name, bool bYesNo);	// Cli to Cli 申請加入隊伍的回應
	static	void						DeletePBPost(char* name);		// Cli to Cli 叫接收者把組隊招募留言下架(送出方)
	static	int							InviteAnswerState(bool isAccept);
	static	int							PromoteToLeader(const char* name);
	static	int							SetLootMethod(PartyItemShareENUM mode, const char* name=NULL);
	static	int							SetLootThreshold(int quality);
	static	int							KickMember(const char* name);
	static	const char*					FindMemberName(int DBID);	
	static	bool						InParty(const char* name);
	static	bool						InGroup(const char* name=NULL);
	static	int							GetLeaderChannelID();
	static	bool						SetInstanceLevel(int level);

	static	bool						SwitchToRaid();
	static	bool						SwitchRaidAssistant(int index, bool state);
	static	bool						SwitchRaidMainTank(int index, bool state);
	static	bool						SwitchRaidMainAttack(int index, bool state);
  	static	bool						RaidDegrade(int index);

	static	bool						MoveRaidMember(int no1, int no2);

	// Item
	static	void						AddRollItem(PartyRollItem& rollItem);
	static	void						DeleteRollItem(int itemVersion);
	static	void						ClearRollItem();
	static	PartyRollItem*				GetRollItem(int itemVersion);
	static	void						AddAssignItem(PartyAssignItem& assignItem);
	static	void						DeleteAssignItem(int itemVersion);
	static	void						ClearAssignItem();
	static	PartyAssignItem*			GetAssignItem(int itemVersion);	
	static	const char*					GetAssignItemMember(int itemVersion, int index);
	static	int							AssignTo(int itemVersion, int DBID);
	static	bool						IsAssigner();

	static void							SetPartyMemberBaseValue(int DBID, int HP, int MP, int SP, int SP_Sub);
	static void							SetPartyMemberMaxBaseValue(int DBID, int HP, int MP, int SP, int SP_Sub);

	// 戰場團隊	
	static	void						ClearBattleGroup();
	static	void						InviteBattleGroup(int DBID=-1);
	static	void						AcceptBattleGroup();
	static	void						DeclineBattleGroup();
	static	void						UninviteBattleMember(int DBID, bool checkLeader=true);
	static	void						UpdateBattleMember();
	static	void						RefreshBattleMember(int DBID);
	static	BattleGroundPartyMember*	GetBattlePartyMember(int index);
	static	BattleGroundPartyMember*	GetBattleRaidMember(int index);
	static	BattleGroundPartyMember*	GetBattleGroupMemberByDBID(int DBID);
	static	void						MoveBattleMember(int no1, int no2);
	static	void						PromoteToBattleLeader(int DBID);
	static	int							GetBattleLeaderPartyNo();
	static	int							GetBattleLeaderRaidNo();
	static	bool						InBattleParty(int DBID);
	static	bool						InBattleGroup(int DBID);

	virtual void R_Invite				( char* LeaderName , int ClientInfo );
	virtual void R_AskJoinToParty		( char* szAskerName , SJobInfo sJobInfo );
	virtual void R_AskJoinToPartyResult	( char* szLeaderName , bool bYesNo );
	virtual void R_DeletePBPost			();
	virtual void R_PartyBaseInfo		( PartyBaseInfoStruct& Info );
	virtual void R_AddMember			( PartyMemberInfoStruct& Member );
	virtual void R_DelMember			( PartyMemberInfoStruct& Member );
	virtual void R_FixMember			( PartyMemberInfoStruct& Member );

	virtual void R_PartyTreasure( int ItemVersion , PartyItemShareENUM LootType , ItemFieldStruct& Item ,	StaticVector< int , _MAX_PARTY_COUNT_  >&  LootDBID );
	virtual void R_PartyTreasureLoot( int ItemVersion ,	int DBID , int LootValue );
	virtual void R_GiveItemInfo( int DBID , ItemFieldStruct& Item , int ItemVersion );

	virtual void OnServerDisConnect		( int ServerID );
	virtual void R_MemberLogout			( int MemberDBID );
	virtual void R_SetMemberPosResult	( bool Result );

	virtual void R_Zone_PartyBase	( int PartyID , int LeaderDBID );
	virtual void R_Zone_Invite		( int LeaderDBID , int ClientInfo );
	virtual void R_Zone_AddMember	( int PartyID , int MemberDBID , int PartyNo , int MemberID );
	virtual void R_Zone_DelMember	( int MemberDBID );
	virtual void R_Zone_FixMember	( int PartyID , int MemberDBID , int PartyNo , int MemberID );
	virtual void R_Zone_SetMemberPosRequest	( bool Result );
	virtual void R_Zone_ChangeLeaderResult( bool Result );
	virtual void R_Zone_Talk		( int GItemID , const char* Content );

	virtual void R_GetMemberInfoResult( int LeaderDBID , vector< PartyMemberInfoStruct > & List );
protected:
	void								UpdatePartyMember();
};

