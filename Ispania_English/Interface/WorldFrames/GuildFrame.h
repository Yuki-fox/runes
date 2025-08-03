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
	bool	isHeader;						// 是否為群組資料
	bool*	pIsExpand;						// 是否展開

	union 
	{
		const char*	groupName;				// 群組名稱
		GuildMemberBaseInfoStruct* member;	// 成員資料
	};
};


class CGuildFrame : public CInterfaceBase
{
public:


	//公會留言版 留言資料.
	struct BBSPost
	{
		bool m_visible;//已經得取.
		int  m_time;//live.
		GuildBoardStruct	 m_post; //詳細資料.
		BBSPost():m_visible(false),m_time(-1){}
	};
	//公會留言版 留言資料LIST.
	typedef vector<BBSPost> BBSMessages;
	//用來排序.
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

	//公會建設的權限
	 bool CanGuildBuilding(void);
	 //公會宣戰的權限
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
	        void                OpenGuildCommand(int iObjID); //打開公會命令介面.
			void                CloseGuildCommand(); //關閉公會命令介面.
			void                AddMember(const char *name);  
			void                MemberLogin(const char *name,time_t time,bool online);  
			void				SelfLeaveGuid(void);
			void				OpenGuildContribution(int iObjID);//開啟公會貢獻介面.
			void				CloseGuildContribution(void);
			int					GetContributionItem(void);//貢獻物品.
			void				RemoveContributionItem(int index);//移除物品.
			void				ContributionItemOK(int Money,int bonus);//貢獻確定
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
			void				GuildInfoApply(void);//上傳公會資訊.
			void				UpDateGuildInfo(GuildBaseStruct& source);//更新公會資訊.
			void				SetRankStr(int index,const char *name);
			void				RecvBoardPost(bool Result);
			void				GetPostContextRequest(int guid);
			void				SetBoardMessage(GuildBoardStruct &Message);
			void                SaveBBSToFile(void);
			void				OpenGuildShop(int npcid, int score,StaticFlag<256>& Flag);
			void				CloseGuildShop(void);
			int					GetGuildShopCount(void);
			void				GuildMemberRankChange( int MemberDBID , int Rank );


			//正式公會邀請
			void GuildInviteResult(char* PlayerName , int	Result );
		
			//加入新成員
			void JoinGuildMember( GuildMemberStruct& Member );

			// 玩家離開自己的公會
			void LeaveGuildMember		( int LeaderDBID , int MemberDBID );
			

			//收到成員的詳細資料
			void GuildMemberInfo( GuildMemberStruct& Member );

			//加入連屬公會
			void JoinNewGuild			( int GuildID , char* LeaderName );
			//加入公會
			void JoinGuild			( int GuildID , char* LeaderName );

			//公會成員上下線
			void GuildPlayerOnline		( int PlayerDBID , bool IsOnline );
			


			//踢除成員
			void  RemoveMemberByKick(int LeaderDBID,int memid);

			//取得公會基本資料
			void GuildBoardRequest( int guildID,bool&v);
			//取得公會名稱
			const char *GetGuildNameByID(int GuildID);
			// 取得公會編號
			int GetGuildIDByName(const char* name);
			//收到公會的會員列表
			void GuildMemberBaseInfo( vector< GuildMemberBaseInfoStruct >& MemberList);
			//受到邀請
			void NewGuildInvite( int GuildID, char* LeaderName, int LeaderDBID );
			//創立公會的回覆
			void CreateGuildResult( int GuildID , char* GuildName , int  Result );

			//離開公會誰 　使用踢人指令者會收到
			void LeaveGuildResult	( char* KickPlayerName , bool Result );
			//離開連署公會 　使用踢人指令者會收到
			void LeaveNewGuildResult	( char* KickPlayerName , bool Result );

			//--接收到公會資料 可能是查詢其他工會的 或是自己的公會
			void GuildInfo(GuildBaseStruct& Guild , int MemberCount,bool bNeedSend );


			void GuildInfoChange( GuildInfoChangeStruct& ChangeMode );


			//收到公會資源變動
			void ModifySimpleGuild( SimpleGuildInfoStruct& SimpleGuildInfo );
			// 收到某公會更名結果
			void ChangeGuildNameResult		( int Result );

			// 收到某公會更名
			 void ChangeGuildName			( int GuildID , char* GuildName );

			//收到公會列表資料　
			void GuildBaseInfo( int GuildID , char* GuildName , GuildFlagStruct& Flag , bool IsReady );
			// 公會倒了
			void GuildDestroy			( int GuildID  );

			void Guild_LevelUp				( int GuildID , int Level );

	GuildMemberStruct*			GetGuildMemberStruct(int dbid);
	GuildMemberBaseInfoStruct *  GetMemberBaseInfo(int dbid);
	void						LeaderChangeRequest(int dbid=-1);
	void						LeaderChange(void);
	bool IsLeader(void);
	bool						SetGuildNote(const char *note);//設定公會告示.
	void						BBS_PageDown(void);
	void						BBS_PageUP(void);
	void                        BBSPageUpdate(void);
	void						GetPostCountRequest(void);
	BBSPost*					GetPostInfo(int index);
	BBSPost*					GetPostInfoBySort(int index);
	//公會BBS 取得留言資訊.
	BBSPost*			GetPostByID(int guid);

	//通知會長換人
	void LeaderChangeMsg(int newid,int oldid);
	 
	//公會BBS 取得目前留言數量.
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

	//重置時間

	void GuildBaseInfoUpdate(void);

	void GuildCanNotJoin();

protected:

	int m_GuildDateLive;//公會資料的老舊.
	bool m_haveGuild;


	int m_reloadIndex;
	int m_maxBBSPage;

	std::vector<int> m_itemPos;
	ShopFubTab       m_ShoptabList;  
	bool m_bshowMessage;

	

	bool                        m_Open;

	int                         m_GuildPointTotal;
	int							m_npcId; //NPC的ID.
	int							m_guildID;					// 公會唯一ID.
	int							m_leaderDBID;				// 會長唯一DBID.
	string						m_leaderName;				// 會長名稱.
	string                      m_BBS_SystemStr[7];
	int                         m_BBSTime[6];//留言刷新時間.
	int                         m_BBSPostTime;//留言間隔時間.
	bool						m_bCanSend;
	GuildBaseStruct			m_tempGuildInfo;               //修改公會權限用.
	BBSMessages             m_BBSMessages; //公會的留言板.
	PostList				m_PostList;//留言的排序.
	int                     m_BBSPage;
	int						m_ModifyTime; //控制修改間隔.
	waitEvent                m_waitEvent;


	
};