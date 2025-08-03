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
	ePartyState_Solo		= 0,			// �S������
	ePartyState_Receive,					// �����ܽ�
	ePartyState_Leader,						// ����ⶤ
	ePartyState_Member,						// �����
	ePartyState_Join,						// ����ӽХ[�J����n�D
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
	int			memberId;					// Server�ζ������ޭ�
	int			raidNo;						// �ζ����ޭ�(0 ~ 35)
	int			DBID;						// ���a��Ʈw�x�s�s��
	void*		sprite;						// ���Я��ޭ�(�x�s�򥻸��)

	// �S��X�Юe��
	union
	{
		int container;
		struct
		{
			bool	assistant	: 1;		// �U�z
			bool	tank		: 1;		// �Z�J
			bool	attack		: 1;		// ������
		};
	};

	// ���a�ϰ��T
	union
	{
		int		GItemID;					// -1 �N��U�u���A
		struct
		{
			short   ItemID;
			short   ZoneID;
		};
	};	

	int			channelID;					// ���y�s��
};

// �Գ�����@����T
struct BattleGroundPartyBaseData
{
	int partyID;							// ����N�X
	int leaderDBID;							// �ⶤ�s��
};

// �Գ�����ӤH��T
struct BattleGroundPartyMember
{
	int partyID;
	int partyNo;
	int memberID;
	int DBID;								// 	
	bool isExists;							// �����[�J��Sprite����O�_�٨S�إ�
};

struct PartyAssignItem
{	
	int				itemVersion;			// ���~���(KEY WORD),�ΥH�P�O�B�z����
	ItemFieldStruct	item;					// ���~���
	vector<int>		loot;					// ���v��o���~���a
};

enum LootItemStatus
{
	eLootItem_None							= 0,	// ���B�z
	eLootItem_WaitRoll,						// �����Y�뤤
	eLootItem_Done,							// �w�Y��
	eLootItem_Release,						// ���ݧR��
};

struct PartyRollItem
{
	int				itemVersion;			// ���~���(KEY WORD),�ΥH�P�O�B�z����
	float			destructTime;			// �l�a�����ɶ�
	float			time;					// ���t��Ʈɶ�
	ItemFieldStruct	item;					// ���~���
	LootItemStatus	status;					// �B�z���p(0���B�z, 1�����Y��, 2�w�Y��, 3���ݧR��)
};

class NetCli_PartyChild : public NetCli_Party
{
public:
	static	PartyBaseInfoStruct			s_info;				// �ն��򥻸�T
	static	vector<PartyMemberInfo>		s_member;			// ��������(�]��Ҧ��p���P�ζ�)
	static	vector<PartyMemberInfo*>	s_party;			// �p������([0] �����a�ۤv)	
	static	vector<PartyAssignItem>		s_assignItem;
	static	vector<PartyRollItem>		s_rollItem;
	static	PartyStateInfo				s_state;
	static	string						s_leaderName;
	static	int							s_leaderRaidIndex;	// �ζ��������ޭ�
	static	int							s_playerNo;			// ���a�Ҧb�p���s��
	static	map<int, PartyBaseValueQueue>	s_baseValueQueue;

	// battle raid 
	static	string						s_BattleLeaderName;			// �Գ��p���ⶤ�W��
	static	int							s_BattleLeaderDBID;			// �Գ��p���ⶤDBID
	static	int							s_BattlePartyNo;			// ���a�Ҧb�p���s��
	static	PartyStateInfo				s_BattleState;				// ���a�Գ��p�����A
	static	BattleGroundPartyBaseData	s_BattleInfo;				// �Գ��p����T
	static	vector<BattleGroundPartyMember>		s_BattleMember;		// �Գ�������T
	static	vector<BattleGroundPartyMember*>	s_BattleParty;		// �Գ��p������

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
	static	int							AskJoinToParty(char* name);		// Cli to Cli �ӽХ[�J����(�e�X��)
	static	int							AskJoinToPartyResult(char* name, bool bYesNo);	// Cli to Cli �ӽХ[�J����^��
	static	void						DeletePBPost(char* name);		// Cli to Cli �s�����̧�ն��۶үd���U�[(�e�X��)
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

	// �Գ��ζ�	
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

