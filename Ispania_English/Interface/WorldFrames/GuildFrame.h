#pragma once

#include <string>
#include "../interface.h"

class CGuildFrame;
extern CGuildFrame*			g_pGuildFrame;

using namespace std;
struct GuildMemberBaseInfoStruct;
struct ContributionInfoLogStruct;
struct GuildMemberListInfo
{
	bool	isHeader;						// �O�_���s�ո��
	bool*	pIsExpand;						// �O�_�i�}

	union 
	{
		const char*	groupName;				// �s�զW��
		GuildMemberBaseInfoStruct* member;	// �������
	};
};


class CGuildFrame : public CInterfaceBase
{
public:


	//���|�d���� �d�����.
	struct BBSPost
	{
		bool m_visible;//�w�g�o��.
		int  m_time;//live.
		GuildBoardStruct	 m_post; //�ԲӸ��.
		BBSPost():m_visible(false),m_time(-1){}
	};
	//���|�d���� �d�����LIST.
	typedef vector<BBSPost> BBSMessages;
	//�ΨӱƧ�.
	typedef vector<int> PostList;

	struct ShopTrans
	{
		bool eable;
		int index;
	};
	typedef vector<ShopTrans> ShopFubTab;

	

	typedef void (CGuildFrame::*Event)(void);    

	struct waitEvent
	{
		bool work;
		vector<int> m_member;

		Event m_eventFunc;	

	};
	CGuildFrame(CInterface* object);
	virtual ~CGuildFrame();

	//���|�س]���v��
	 bool CanGuildBuilding(void);
	 //���|�žԪ��v��
	 bool CanGuildWaring(void);

	int GetNumGuildMembers(void);
	GuildMemberListInfo* GetGuildRosterInfo(int index);

	virtual	void				Update(float elapsedTime);
	virtual	void				RegisterFunc();
	virtual	void				RegisterGuild();
	virtual	bool				CreateGuild(const char* guildName);
	virtual void				PetitionSignature(int guildID, const char* leaderName, int leaderDBID);
	virtual void				PetitionReply(bool agree);
	virtual	void				PetitionQuit();
	virtual	void				PetitionKick(const char* name);
	void						GuildInviteRequest(int guildID, const char* leaderName, int leaderDBID);
			void				GuildInviteReply(bool agree);
			void BuildFrames(void);
	virtual	void	LeaveWorld();

	const		char *GetGuildName(void);
			void				GuildReadyResult(int Result); 

	virtual	void				Invite(const char* name);
	virtual	void				Uninvite(const char* name);
	virtual	void				Leave();
	
	virtual	void				UpdateGuild();
			void                Save();
	        void                OpenGuildCommand(int iObjID); //���}���|�R�O����.
			void                CloseGuildCommand(); //�������|�R�O����.
			void                AddMember(const char *name);  
			void                MemberLogin(const char *name,time_t time,bool online);  
			void				SelfLeaveGuid(void);
			void				OpenGuildContribution(int iObjID);//�}�Ҥ��|�^�m����.
			void				CloseGuildContribution(void);
			int					GetContributionItem(void);//�^�m���~.
			void				RemoveContributionItem(int index);//�������~.
			void				ContributionItemOK(int Money,int bonus);//�^�m�T�w
			void				CloseContribution(void);
			int					GetGuildPoint(void);

			void AskLeaderChange(const char* SendName);
			void  AskLeaderChangeResult(int dbid,bool YesNo);
			void AskLeaderChangeResult(bool YesNo);
			int					GetGuildResource(int index);
			int GetGuildShopFuncInfo(int index);

			void GuildShopBuy(int dbid);
			void GuildShopBuyResult(int id, bool result);
			void				ReadAllMemberDate(void);
			void                YouGetFire(void);  
			
			void				UpdateMemberInfo(int db);
			bool				SetMemberCadreNote(int db,const char* name);
			const char*			GetMemberCadreNote(void);
			bool				SetMemberSelfNote(int db,const char* name);
			const char*			GetMemberSelfNote(void);
			bool				SetGuildDesc(const char*desc,bool V,bool visitHouse);
			bool				IsOpen();
	GuildRankBaseInfoStruct*    GetUserRankInfo(void);
			int					GetUserRank(void);
			void				GuildInfoApply(void);//�W�Ǥ��|��T.
			void				UpDateGuildInfo(GuildBaseStruct& source);//��s���|��T.
			void				SetRankStr(int index,const char *name);
			void				RecvBoardPost(bool Result);
			void				GetPostContextRequest(int guid);
			void				SetBoardMessage(GuildBoardStruct &Message);
			void                SaveBBSToFile(void);
			void				OpenGuildShop(int npcid, int score,StaticFlag<256>& Flag);
			void				CloseGuildShop(void);
			int					GetGuildShopCount(void);
			void				GuildMemberRankChange( int MemberDBID , int Rank );


			//�������|�ܽ�
			void GuildInviteResult(char* PlayerName , int	Result );
		
			//�[�J�s����
			void JoinGuildMember( GuildMemberStruct& Member );

			// ���a���}�ۤv�����|
			void LeaveGuildMember		( int LeaderDBID , int MemberDBID );
			

			//���즨�����ԲӸ��
			void GuildMemberInfo( GuildMemberStruct& Member );

			//�[�J�s�ݤ��|
			void JoinNewGuild			( int GuildID , char* LeaderName );
			//�[�J���|
			void JoinGuild			( int GuildID , char* LeaderName );

			//���|�����W�U�u
			void GuildPlayerOnline		( int PlayerDBID , bool IsOnline );
			


			//�𰣦���
			void  RemoveMemberByKick(int LeaderDBID,int memid);

			//���o���|�򥻸��
			void GuildBoardRequest( int guildID,bool&v);
			//���o���|�W��
			const char *GetGuildNameByID(int GuildID);
			// ���o���|�s��
			int GetGuildIDByName(const char* name);
			//���줽�|���|���C��
			void GuildMemberBaseInfo( vector< GuildMemberBaseInfoStruct >& MemberList);
			//�����ܽ�
			void NewGuildInvite( int GuildID, char* LeaderName, int LeaderDBID );
			//�Хߤ��|���^��
			void CreateGuildResult( int GuildID , char* GuildName , int  Result );

			//���}���|�� �@�ϥν�H���O�̷|����
			void LeaveGuildResult	( char* KickPlayerName , bool Result );
			//���}�s�p���| �@�ϥν�H���O�̷|����
			void LeaveNewGuildResult	( char* KickPlayerName , bool Result );

			//--�����줽�|��� �i��O�d�ߨ�L�u�|�� �άO�ۤv�����|
			void GuildInfo(GuildBaseStruct& Guild , int MemberCount,bool bNeedSend );


			void GuildInfoChange( GuildInfoChangeStruct& ChangeMode );


			//���줽�|�귽�ܰ�
			void ModifySimpleGuild( SimpleGuildInfoStruct& SimpleGuildInfo );
			// ����Y���|��W���G
			void ChangeGuildNameResult		( int Result );

			// ����Y���|��W
			 void ChangeGuildName			( int GuildID , char* GuildName );

			//���줽�|�C���ơ@
			void GuildBaseInfo( int GuildID , char* GuildName , GuildFlagStruct& Flag , bool IsReady );
			// ���|�ˤF
			void GuildDestroy			( int GuildID  );

			void Guild_LevelUp				( int GuildID , int Level );

	GuildMemberStruct*			GetGuildMemberStruct(int dbid);
	GuildMemberBaseInfoStruct *  GetMemberBaseInfo(int dbid);
	void						LeaderChangeRequest(int dbid=-1);
	void						LeaderChange(void);
	bool IsLeader(void);
	bool						SetGuildNote(const char *note);//�]�w���|�i��.
	void						BBS_PageDown(void);
	void						BBS_PageUP(void);
	void                        BBSPageUpdate(void);
	void						GetPostCountRequest(void);
	BBSPost*					GetPostInfo(int index);
	BBSPost*					GetPostInfoBySort(int index);
	//���|BBS ���o�d����T.
	BBSPost*			GetPostByID(int guid);

	//�q���|�����H
	void LeaderChangeMsg(int newid,int oldid);
	 
	//���|BBS ���o�ثe�d���ƶq.
	int							GetPostCount(void);
	void						SetMemberRankRequest(int index,int rank);


	int						SetGuildPost(int guid,const char *Title,const char *Message);
	void						SetBBSPageCount(int MaxCount,int Count,int PageID);
	void 						SetBBSPostDate( int MessageGUID , int Index , char* PlayerName , int Time , char* Title , GuildBoardModeStruct Type );		

	int							NewPost(const char* Title,const char* Message);
	void sortList();
	void BeginMemberInfoRequest(void);
	void AddMemberInfoRequest(int dbid);
	void EndMemberInfo(Event func);
	void AddMemberInfo(int dbid);
	void reLoadBBSPage(int page);
	
    void Load(void);

	void GuildCreate(int dbid);

	void GuildContributionItemResult(int eResct);
	bool isHaveGuild(void);
	bool GetGuildInfo(void);
	int GetGuildLevel(void);
	int GetGuildLevelInfo(int level,int index);
	void SetDayLogTable(int count,ContributionInfoLogStruct* lists);

	void SetGuildResLogDay(int day);

	void GuildUpgradeRequest( void );
	void Guild_upGrade(bool Result,int lev);
	
	void GuildUpgradeResult			( int GuildLv , bool Result );

	void SetRankInfo(int index,bool v1,bool v2,bool v3,bool v4,bool v5,bool v6,bool v7,bool vbuild,bool Furniture ,bool GuildWarRegister,bool SetGuildHouseStyle );

	GuildBaseStruct*		GetGuildBaseInfo()					{ return &m_tempGuildInfo; }

	GuildFlagStruct* GetGuildFlagStruct( int GuildID );
	void ClearGuildInfo(void);

	//���m�ɶ�

	void GuildBaseInfoUpdate(void);

	void GuildCanNotJoin();

protected:

	int m_GuildDateLive;//���|��ƪ�����.
	bool m_haveGuild;


	int m_reloadIndex;
	int m_maxBBSPage;

	std::vector<int> m_itemPos;
	ShopFubTab       m_ShoptabList;  
	bool m_bshowMessage;

	

	bool                        m_Open;

	int                         m_GuildPointTotal;
	int							m_npcId; //NPC��ID.
	int							m_guildID;					// ���|�ߤ@ID.
	int							m_leaderDBID;				// �|���ߤ@DBID.
	string						m_leaderName;				// �|���W��.
	string                      m_BBS_SystemStr[7];
	int                         m_BBSTime[6];//�d����s�ɶ�.
	int                         m_BBSPostTime;//�d�����j�ɶ�.
	bool						m_bCanSend;
	GuildBaseStruct			m_tempGuildInfo;               //�ק綠�|�v����.
	BBSMessages             m_BBSMessages; //���|���d���O.
	PostList				m_PostList;//�d�����Ƨ�.
	int                     m_BBSPage;
	int						m_ModifyTime; //����קﶡ�j.
	waitEvent                m_waitEvent;


	
};