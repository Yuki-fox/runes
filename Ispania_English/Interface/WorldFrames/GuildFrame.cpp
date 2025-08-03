#include <time.h>
#include "../../netwaker_member/NetWakerClientInc.h"
#include "../../mainproc/GameMain.h"
#include "../WorldFrames/LoadingFrame.h"
#include "..\tools\Cini\Ini.h"
#include "GuildFrame.h"
#include "../interface/debug/DebugMessage.h"
#include "ChatFrame.h"
#include "..\map\MapCommon.h"
#include "ItemClipBoard.h"
#include "../../Tools/Cini/IniFile2.h"
#include "../interface/login/CharacterCreate.h"//檢查名稱合法
#include "BagFrame.h"
#include "../WorldFrames/ItemUTIL.h"
#include "../Login/AccountLogin.h"
#include "../GuildHouses/GuildHousesFrame.h"


#define GUILD_LOGDAY_MAX 7

//#define CREATEGUILD_COST (10*10000) //10 Gold
//#define MSG_LEDADER_CHANGE "%s 讓位 %s 為新會長" 

#define DF_GETGUILDINFO_UPDATE (5*1000)

#define DF_GETGUILDINFO_COUNTER (60*2*1000)//公會資訊更新間隔 2分鐘
#define DF_POST_COOLDOWN (60*2*1000)//POST文間隔 2分鐘

#define DF_GUILDMEMBER_UPDATETIMER (2*60*1000)
#define DF_GUILDINFO_UPDATETIMER 300000	//(60*5*1000)




//#define INVITE_WHOTO_NEWGUILD "你邀請 %s 加入連署"
//#define INVITE_WHOTO_GUILD "你邀請 %s 加入公會"
//#define WAIT_WHOTO_GUILD "等待 %s 回應中..."
//#define YOUARE_FIRE_GUILD "你退出公會"
//#define REMOVEMEMBER_GUILD "%s 退出公會"
//#define ADDMEMBER_GUILD "%s 加入公會"
//#define MEMBERLOGIN_GUILD "%s 上線了(%s)"
//#define MEMBERLOGOUT_GUILD "%s 下線了(%s)"
#define BBS_POST_LOG "BBS_LOG.txt"
//#define GUILD_NOTE "公告"
//#define GUILD_POST "留言%02d"
//#define GUILD_CONTEXT "內容"
//#define POST_AUTHOR "作者"
//#define POST_TITLE "主題"
//#define POST_DATE "日期"

//#define OFFLINETIME_STR "%d天%d小時..."


CGuildFrame*					g_pGuildFrame = NULL;


int LuaFunc_IsInGuild				(lua_State* L);			// 是否有公會
int LuaFunc_GuildCreate				(lua_State* L);			// 註冊成立公會
int	LuaFunc_GetGuildInfo			(lua_State* L);			// 取得玩家公會資訊
//int LuaFunc_GuildPetitionSignature	(lua_State* L);			// 要求公會連署
int LuaFunc_GuildPetitionAccept		(lua_State* L);			// 同意公會連署
int LuaFunc_GuildPetitionDecline	(lua_State* L);			// 拒絕公會連署
int LuaFunc_GuildInviteResult		(lua_State* L);			// 回應加入公會請求
int LuaFunc_GuildPetitionQuit		(lua_State* L);			// 退出公會連署
int LuaFunc_GuildPetitionKick		(lua_State* L);			// 移除公會連署玩家
int LuaFunc_GetNumGuildMembers		(lua_State* L);			// 取得公會成員數量
int LuaFunc_GetGuildRosterInfo		(lua_State* L);			// 取得公會成員資料

int LuaFunc_GuildInvite				(lua_State* L);			// 邀請加入公會
int LuaFunc_GuildUninvite			(lua_State* L);			// 踢出公會
int LuaFunc_GuildLeave				(lua_State* L);			// 離開公會

int LuaFunc_ExpandGuildGroup		(lua_State* L);			// 展開群組
int LuaFunc_CollapseGuildGroup		(lua_State* L);			// 關閉群組

int LuaFunc_SetGuildRosterSelection	(lua_State* L);			// 設定擇取對象
int LuaFunc_GetGuildRosterSelection	(lua_State* L);			// 取得擇取對象


int LuaFunc_GF_GetRankCount			(lua_State* L);			//取得階級數量
int LuaFunc_GF_GetRankStr			(lua_State* L);			//取得階級名稱
int LuaFunc_GF_CanGuildWaring		(lua_State* L);			//公會宣戰
int LuaFunc_GF_SetRankStr			(lua_State* L);			//設定階級名稱
int LuaFunc_GF_GetRankInfo			(lua_State* L);		    //取得權限
int LuaFunc_GF_SetRankInfo			(lua_State* L);		    //設定權限
int LuaFunc_GF_SetGuildDesc(lua_State* L);		    //設定公會招生與敘述

int LuaFunc_GF_AskLeaderChangeResult(lua_State* L);
int LuaFunc_GF_GuildInfoApply		(lua_State* L);			//套用公會資訊
int LuaFunc_GF_SetGroupEnable		(lua_State* L);			//使用群組
int LuaFunc_GF_GetMenuFlag 		(lua_State* L);			    //取得選單
int LuaFunc_GF_GetGuildFunc 		(lua_State* L);			    
int LuaFunc_GF_LeaderChange			(lua_State* L);			// 會長退位
int LuaFunc_GF_SetMemberRank			(lua_State* L);			// 設定會員階級

int LuaFunc_GF_SetMemberCadreNote(lua_State* L);			// 設定會員幹部註解
int LuaFunc_GF_GetMemberCadreNote(lua_State* L);			// 取得會員幹部註解

int LuaFunc_GF_SetMemberSelfNote(lua_State* L);			// 設定個人註解
int LuaFunc_GF_GetMemberSelfNote(lua_State* L);			// 取得個人註解
int LuaFunc_GC_CloseGuildCommand(lua_State* L);			
int LuaFunc_GF_CanCreateGuildCost(lua_State* L);    //檢查10G
int LuaFunc_GF_SetGuildNote(lua_State* L);     //公會告示版ui 設定公會告示
int LuaFunc_GF_Rename(lua_State* L);			// 公會改名
int LuaFunc_GF_GetGuildPostCount(lua_State* L);     //公會BBS 取得目前留言數量
int LuaFunc_GF_GetGuildPostInfo(lua_State* L);     //公會BBS 取得目前留言資訊
int LuaFunc_GF_SetGuildPostInfo(lua_State* L);      //公會BBS 設定目前留言資訊
int LuaFunc_GF_SetGuildPost(lua_State* L);			// 設定留言
int LuaFunc_GF_GetGuildPostRequest(lua_State* L);			// 請求留言
int LuaFunc_GF_GetBBSMenuFlag(lua_State* L);	


int LuaFunc_GF_GetPostDate(lua_State* L);			// 取得留言資料

int LuaFunc_GF_NewPost(lua_State* L);			// 新增留言
//int LuaFunc_GF_DeletePost(lua_State* L);			// 刪除留言
int LuaFunc_GF_GetBBSInfoRequest(lua_State* L);			// 請求目前幾筆資料


int LuaFunc_GF_GetPostCountRequest(lua_State* L);			// 請求目前幾筆資料




int Luafunc_GuildBoard_GetNextGuildIDName  (lua_State* L); //公會告示版ui 取得公會名稱及ID  
int LuaFunc_GuildBoard_RequestGuildInfo(lua_State* L);     //公會告示版ui 取得公會資訊  
//向Server 讀取所有資料
int LuaFunc_GF_ReadAllDate(lua_State* L);



//讀取BAG的物品
int LuaFunc_GCB_GetContributionItem(lua_State* L);  
int LuaFunc_GCB_RemoveContributionItem(lua_State* L);  
int LuaFunc_GCB_ContributionItemOK(lua_State* L);  
int LuaFunc_GCB_CloseContribution(lua_State* L); 
int LuaFunc_GCB_GetGuildPoint(lua_State* L); 
int LuaFunc_GCB_GetGuildResource(lua_State* L); 
int LuaFunc_GCB_GetGuildLevel(lua_State* L); 
int LuaFunc_GCB_GetGuildLevelCount(lua_State* L); 
int LuaFunc_GCB_GetGuildLevelInfo(lua_State* L); 
int LuaFunc_GCB_GetGuildResLog(lua_State* L); 
int LuaFunc_GCB_GetGuildResLogAll(lua_State* L); 

int LuaFunc_GCB_GetGuildResLogCount(lua_State* L); 
int LuaFunc_GCB_SetGuildResLogDay(lua_State* L); 

int LuaFunc_GCB_GuildUpgrade(lua_State* L); 


int LuaFunc_GSF_CloseGuildShop(lua_State* L); 

int LuaFunc_GSF_GetGuildShopCount(lua_State* L);
int LuaFunc_GSF_GetGuildShopFuncInfo(lua_State* L);
int LuaFunc_GSF_GetGuildShopBuy(lua_State* L);
int LuaFunc_CheckRightfulName(lua_State* L);

// 公會宣戰
int LuaFunc_GetGuildWarItemInfo(lua_State* L);
int LuaFunc_DeclareGuildWar(lua_State* L);
int LuaFunc_GetGuildWarInfo(lua_State* L);
int LuaFunc_GetGuildWarOutdoorsPoint(lua_State* L);				// 取得野外宣戰戰績
int LuaFunc_GetGuildWarOutdoorsDeclareInfo(lua_State* L);		// 取得自己公會宣戰資訊	
int LuaFunc_GetGuildWarOutdoorsNumEnemies(lua_State* L);		// 取得敵對公會宣戰個數
int LuaFunc_GetGuildWarOutdoorsEnemyInfo(lua_State* L);			// 取得敵對公會宣戰資訊	

int LuaFunc_GetGuildWarPoint(lua_State* L);				// 取得公會屋宣戰戰績


//公會旗
int LuaFunc_GetGuildFlagInsigniaCount(lua_State* L);
int LuaFunc_GuildFlagBannerCount     (lua_State* L);
int LuaFunc_GetGuildFlagInsigniaType (lua_State* L);
int LuaFunc_GuildFlagBannerType      (lua_State* L);


int LuaFunc_GuildFlagInfo            (lua_State* L);

int LuaFunc_SetGuildFlagInfoRequest  (lua_State* L);

int  LuaFunc_GetSelfGuildRank		(lua_State* L);


struct CachGuildMember
{
	GuildMemberStruct m_info;		//詳細

	CachGuildMember()
	{
		m_info.LogoutTime=-1;
	}
	int m_updateTime;				//記錄資料更新
};


bool  CheckStr(const char *guildName,int len=24);

typedef vector<GuildMemberBaseInfoStruct> simpleGuildMemberList;
typedef vector<CachGuildMember> SinfoGuildMemberList;
//名稱錯誤訊息

const char *GetNameCheckErr(int type)
{
	switch (type)
	{
	case 0:
		return g_ObjectData->GetString("SYS_CREATEROLE_ERROR_SHORT");
	case 1:
		return g_ObjectData->GetString("SYS_CREATEROLE_ERROR_LONG");
	case 2:

		return g_ObjectData->GetString("SYS_CREATEROLE_ERROR_WRONGFUL");
	case 3:
		return g_ObjectData->GetString("SYS_CREATEROLE_ERROR_STRING");
	case 4:
		return g_ObjectData->GetString("SYS_CREATEROLE_ONLYBASICLETTERS");
	case 5:
		return g_ObjectData->GetString("SYS_CREATEROLE_ENDLETTERERROR");

	}
	return  g_ObjectData->GetString("SYS_CREATEROLE_UNKNOW"); 
}

//內部簡易公會名稱
struct _Guild_data
{
	char m_name[_MAX_NAMERECORD_SIZE_];
	int m_lev;//0 連署中,1:建立
	GuildBaseStruct m_info;//公會的基本資料
	int m_MemberCount;//成員數量
	int m_updateTime; //更新時間
};
typedef map<int ,_Guild_data> simpleGuildMap;

typedef vector<ContributionInfoLogStruct> ContributionDayLog;


struct ContributionLogDay
{
	bool m_bRead;
	int m_time;
	ContributionDayLog m_list;
	ContributionLogDay():m_bRead(false),m_time(0)
	{
		
	}
};

//s_AllGuildMap.clear();
//內部使用的公會資料
class Guild_internal
{
public:

	Guild_internal(CInterface*	ui):m_interface(ui),m_UserGroup(false),m_RosterSelection(-1),m_RosterSelectionDBID(-1),m_day(0)
	{
		memset (m_GroupIsExpand,0,sizeof(m_GroupIsExpand));
		m_bLeaderChange=false;
	}

	bool isChangeDay(void);
	// 取得公會ID
	int GetGuildIDByName(const char* name);

	//公會成員上下線
	void GuildPlayerOnline		( int PlayerDBID , bool IsOnline );

	//更新資料
	void UpDateNewMember(GuildMemberStruct& member,bool newMember);


	//成員的詳細資料
	void GuildMemberInfo( GuildMemberStruct& Member );
	//踢除成員
	void  RemoveMemberByKick(int LeaderDBID,int memid);

	//加入公會
	void JoinGuild			( int GuildID , char* LeaderName );
	//加入連署公會
	void JoinNewGuild			( int GuildID , char* LeaderName );

	//正式公會邀請
	void GuildInviteResult(char* PlayerName , int	Result );
	
	//設定使用組群
	void SetUserGroup(bool bGroup);
// 要求重新取得自己公會資訊
	void ReloadGuildDate(void);
	//取得公會基本資料
	void GuildBoardRequest( int guildID ,bool& v );

	//void GuildInfoChange(GuildInfoChangeStruct& ChangeMode);

	void Clear(void);

	//收到公會的會員列表
	void GuildMemberBaseInfo( vector< GuildMemberBaseInfoStruct >& MemberList);

	//離開公會.　誰 　
	void LeaveGuildResult	( char* KickPlayerName , bool Result );
	//離開連署公會.　誰 　
	void LeaveNewGuildResult	( char* KickPlayerName , bool Result );
	//取得公會名稱
	const char *GetGuildNameByID(int GuildID);

	//改變公會佈告欄
	void SendEvent_GuildBoardResult( _Guild_data* data );
	//得到某公會的基本資料
	void  GuildInfo(GuildBaseStruct& guild,int memberCount,bool bNeedSend);

	//收到公會資源變動
	void ModifySimpleGuild( SimpleGuildInfoStruct& SimpleGuildInfo );

	//取得自己公會的基本資料
	_Guild_data* GetSelfGuildData(void);
	//取得某公會的基本資料
	_Guild_data* GetGuildData(int GuildID);

	// 收到某公會更名結果
	void ChangeGuildNameResult		( int Result );

// 其他人收到某公會更名
	void ChangeGuildName( int GuildID , char* GuildName );
	//某公會基本資料　維護公會列表
	void GuildBaseInfo( int GuildID , char* GuildName , GuildFlagStruct& Flag, bool IsReady );
	//某公會倒了
	void GuildDestroy			( int GuildID  );

	//移走會員
	void RemoveMember(char *MemberNAME);
	//移走會員
	void RemoveMemberByID(int dbid);
	//移走會員詳細資料
	void RemoveSInfoMember(int dbid);

	ContributionLogDay& GetGuildResLogTable(int day);
	//打開更名對話框
	void OpenRenameDialog();

	//重造會員列表
	void UpdateMemberInfo();

	//選到
	void SetGuildRosterSelection(int index);
	//向server 討會員詳細資料
	void ReloadGuildMemberDate(int dbid);

	//找資料
	CachGuildMember * GetMemberInfo(int dbid);

	//找基本資料
	GuildMemberBaseInfoStruct * GetMemberBaseInfo(int dbid);
	
	//找基本資料
	GuildMemberBaseInfoStruct *GetSelfBaseInfo(void);
	//取得被選擇的資料
	GuildMemberBaseInfoStruct* GetSelectBaseInfoStruct(void);

	//取得自己的資料
	CachGuildMember*		GetSelfInfo(void);

	// 玩家離開自己的公會
	void LeaveGuildMember		( int LeaderDBID , int MemberDBID );


	

	//公會改名
	 void GuildRename(const char * GuildName);

	//加入新成員
	void JoinGuildMember( GuildMemberStruct& Member );

	//取得公會成員列表數量
	int GetNumGuildMembers(void);

	//取得索引表
	GuildMemberListInfo *GetGuildMemberInfo(int index);
	//設定群組開關
	void SetGuildHeaderState(int index, bool isExpand);

	//選到哪個?
	int GetGuildRosterSelection(void );


	bool SetLogDay(int index);

	void SetDayLogTable(int count,ContributionInfoLogStruct* lists);

	//創立公會的回覆
	void CreateGuildResult( int GuildID , char* GuildName , int Result );
	//連署公會的邀請
	void NewGuildInvite( int GuildID, char* LeaderName, int LeaderDBID );

	//秀出訊息誰離開了
	void				ShowMemberLeave(const char *name);  
	//UI 
	CInterface*					m_interface;

	simpleGuildMap& GetGuildMap(void){return m_AllGuildMap;}

//公會資料
	simpleGuildMemberList  m_baseMemberList; //簡易

	SinfoGuildMemberList			m_MemberList;	//詳細
	vector<GuildMemberListInfo>		m_GuildMemberInfo;//會員列表 群組資料 目前無用.
	bool							m_GroupIsExpand[_MAX_GUILD_GROUP_COUNT_];
	int								m_RosterSelection;//是某有選人.
	int								m_RosterSelectionDBID;//選到誰.
	bool							m_UserGroup;//使用群組.


	int m_LeaderChangeWaitTime;
	int m_LeaderChangeDBID;
	bool m_bLeaderChange;

	void Update(float elapsedTime)
	{
		if (m_bLeaderChange)
		{
			m_LeaderChangeWaitTime-=(int)elapsedTime;
			if (m_LeaderChangeWaitTime<0)
			{
				m_bLeaderChange=false;

				GuildMemberBaseInfoStruct* selectInfo = GetMemberBaseInfo(m_LeaderChangeDBID);
				if (selectInfo)
				{				
					lua_State* L = g_pGuildFrame->GetLuaState();
					if (L)
					{
						lua_pushstring( L, selectInfo->MemberName.Begin() );
						lua_setglobal( L, "arg1" );
						lua_pushboolean( L, false );
						lua_setglobal( L, "arg2" );
						m_interface->SendWorldEvent("GUILD_ASK_LEADERCHANGE_RESULT");	

					}
					
				}


				

			}

		}

	}

	void SetLeaderChangeBegin(int dbid)
	{
		m_bLeaderChange=true;
		m_LeaderChangeWaitTime=30000;
		m_LeaderChangeDBID=dbid;

	}

	void SetLeaderChangeEnd()
	{
		m_bLeaderChange=false;
		m_LeaderChangeWaitTime=0;
		m_LeaderChangeDBID=-1;

	}

	//總公會表

	       	time_t m_timeT;                     
	
	int                            m_day;
	ContributionLogDay             m_logs[7];

	
	simpleGuildMap					m_AllGuildMap;
};

Guild_internal* g_internal;
bool Guild_internal::isChangeDay(void)
{
	bool ret=false;




	time_t nowTime;
	time( &nowTime );


	struct tm *gmt,nowMT,*gmt1;
	gmt = localtime( &nowTime );

	nowMT=*gmt;

	gmt1= localtime( &m_timeT );

//	nowMT.

	if (gmt1->tm_mday!=nowMT.tm_mday)
	{
		time( &m_timeT );


		for(int i=0;i<7;i++)
		{
			m_logs[i].m_time=0;
			m_logs[i].m_bRead=false;
		}

		ret=true;
	}






	return ret;
}
void Guild_internal::Clear(void)
{

	m_bLeaderChange=0;

	time( &m_timeT );


	for(int i=0;i<7;i++)
	{
		m_logs[i].m_time=0;
		m_logs[i].m_bRead=false;
	}

}
//正式公會邀請
void Guild_internal::GuildInviteResult(char* PlayerName , int	Result )
{  
	char temp[MAX_PATH]="";
	const char *pStrMsg=g_ObjectData->GetString( "GUILD_MSG_ADDMEMBER_FAILL");
	switch( Result )
	{
	case EM_GuildInviteResult_OK			:	//要請成
		pStrMsg=g_ObjectData->GetString( "GUILD_MSG_ADDMEMBER_GUILD");
		//sprintf(temp,"%s 加入公會" , PlayerName);
		//g_pDebugMessage->DebugOutput( 2 , "%s 加入公會" , PlayerName );
		break;

	case EM_GuildInviteResult_NotFind	:	//找不到 想邀請者
		pStrMsg=g_ObjectData->GetString( "GUILD_MSG_ADDMEMBER_FAIL_MISS");
		//sprintf(temp,"%s 加入公會失敗 (找不到 想邀請者)" , PlayerName );
		//g_pDebugMessage->DebugOutput( 2 , "%s 加入公會失敗 (找不到 想邀請者)" , PlayerName );
		break;
	case EM_GuildInviteResult_HasGuild	:	//想邀請者 已經有公會
		pStrMsg=g_ObjectData->GetString( "GUILD_MSG_ADDMEMBER_FAIL_HAVE");
		//sprintf(temp,"%s 加入公會失敗 (想邀請者 已經有公會)" , PlayerName);
		//g_pDebugMessage->DebugOutput( 2 , "%s 加入公會失敗 (想邀請者 已經有公會)" , PlayerName );
		break;
	case EM_GuildInviteResult_Disagree	:	//想邀請者 不同意
		pStrMsg=g_ObjectData->GetString( "GUILD_MSG_ADDMEMBER_NOTAGREE");
		//sprintf(temp,"%s 加入公會失敗 (想邀請者 不同意)" , PlayerName);
		//g_pDebugMessage->DebugOutput( 2 , "%s 加入公會失敗 (想邀請者 不同意)" , PlayerName );
		break;
	}
	sprintf_s(temp,512,pStrMsg , PlayerName);
	g_pChatFrame->SendMsgEvent("CHAT_MSG_SYSTEM", temp, "");

}
ContributionLogDay& Guild_internal::GetGuildResLogTable(int day)
{
	
	return m_logs[day];
}
void Guild_internal::SetDayLogTable(int count,ContributionInfoLogStruct* lists)
{

	if (m_day>=0&&m_day<GUILD_LOGDAY_MAX)
	{

		ContributionLogDay &tab=m_logs[m_day];

		tab.m_bRead=true;
		tab.m_time=timeGetTime();
		tab.m_list.clear();
		for (int i=0;i<count ;i++)
		{

			ContributionInfoLogStruct temp=lists[i];
			
			
			tab.m_list.push_back(temp);

		}
/*
		count=20+rand()%20;
		for (int i=0;i<count;i++)
		{
			ContributionInfoLogStruct temp;
			temp.PlayerDBID=rand()%1000;

			
			//temp.time=timeGetTime();
			

			temp.Resource.Gold=rand()%100;
			tab.m_list.push_back(temp);

		}
*/
		


	}

}

bool Guild_internal::SetLogDay(int index)
{
	bool ret=false;

	
	if (index>=0&&index<GUILD_LOGDAY_MAX)
	{
		isChangeDay();

		m_day=index;
		ContributionLogDay &tab=m_logs[m_day];

		int time=timeGetTime();
		if ((!tab.m_bRead )||((time-tab.m_time)>DF_GETGUILDINFO_UPDATE) )
		{


			if ((!tab.m_bRead )|| index==0 )
			{

				NetCli_Guild::SL_SelectContributionLog(index);
			}
			else
				ret=true;

		}
		else if (tab.m_bRead)
		{
			ret=true;
			//m_iface->SendWorldEvent("GRF_UPDATELOG");
		}

	
	
	}

	return ret;
}
//打開更名對話框
void Guild_internal::OpenRenameDialog()
{

	lua_State* L = g_pGuildFrame->GetLuaState();
	if (L)
	{
		_Guild_data* date=GetSelfGuildData();

			const char *name="[**********]";
		if (date)
			name=date->m_info.GuildName.Begin();
		

		//公會名稱
		lua_pushstring( L, name );
		lua_setglobal( L, "arg1" );
		m_interface->SendWorldEvent("GUILD_RENAME_OPEN");	
		

	}

}

 void Guild_internal::GuildRename(const char * GuildName)
 {


	// NetCli_ChannelChild::S_DataTransmitToCli_Name( (char*)name , &Send , sizeof(Send) );

	 NetCli_Guild::SL_ChangeGuildNameRequest( GuildName );

 }


// 回應公會更名
void Guild_internal::ChangeGuildNameResult( int Result )
{

	char temp[MAX_PATH]="";
	const char *pStrMsg=g_ObjectData->GetString( "SYS_GUILD_RENAME_FLAGERR");
	switch (Result)
	{
	case EM_ChangeGuildNameResult_OK:		 // 更名OK
		{
			pStrMsg=g_ObjectData->GetString( "SYS_GUILD_RENAME_OK");
			_Guild_data* _date=GetSelfGuildData();
			if (_date)
			_date->m_info.IsNeedChangeName=false;
		}

		break;
	case EM_ChangeGuildNameResult_NameUsed:	 //公會名字重複
		pStrMsg=g_ObjectData->GetString( "SYS_GUILD_RENAME_USED");
		OpenRenameDialog();
		break;
	case EM_ChangeGuildNameResult_FlagError: //沒有開啟改名
		pStrMsg=g_ObjectData->GetString( "SYS_GUILD_RENAME_FLAGERR");
		break;
	case EM_ChangeGuildNameResult_NotLeader: //不是會長
		pStrMsg=g_ObjectData->GetString( "SYS_GUILD_RENAME_NOTLEADER");
		break;



	case EM_ChangeGuildNameResult_NameToShort	:		// 名稱太短,不合法
	case EM_ChangeGuildNameResult_NameToLong		:		// 名稱太長,不合法
	case EM_ChangeGuildNameResult_NameIsWrongful	:		// 有錯誤的符號或字元
	case EM_ChangeGuildNameResult_HasSpecialString:		// 特殊名稱不能用
	case EM_ChangeGuildNameResult_OnlyBasicLetters:		// 只能使用基本英文字母
	case EM_ChangeGuildNameResult_EndLetterError	:		// 結尾字元錯誤(',")
		pStrMsg=GetNameCheckErr(Result-EM_ChangeGuildNameResult_NameToShort);
		break;


	case EM_ChangeGuildNameResult_UnknowErr:

		pStrMsg=g_ObjectData->GetString( "SYS_CREATEROLE_UNKNOW");
		break;

	}

	strcpy(temp,pStrMsg );
	g_pChatFrame->SendMsgEvent("CHAT_MSG_SYSTEM", temp, "");
}

//公會成員上下線
void Guild_internal::GuildPlayerOnline( int PlayerDBID , bool IsOnline )
{
	if (PlayerDBID== g_pGameMain->GetDBID())
		return ;

	GuildMemberBaseInfoStruct* ret=	GetMemberBaseInfo(PlayerDBID);
	if (ret)
	{

		BOOL oldState =ret->IsOnline;
		time_t NowTime;
		time( &NowTime );

		ret->IsOnline=IsOnline;

		if (!IsOnline)
		{
			CachGuildMember * sinfo=GetMemberInfo(PlayerDBID);
			if (sinfo)
			{
				sinfo->m_info.LogoutTime=(int)NowTime;
			}
		}

		if (IsOnline)
			NetCli_GuildChild::SL_GuildMemberListRequet(CNetGlobal::RoleData()->BaseData.GuildID);
		if ((bool)oldState!=IsOnline)
		    g_pGuildFrame->MemberLogin(ret->MemberName.Begin(),NowTime,IsOnline);
	}

}

int Guild_internal::GetGuildIDByName(const char* name)
{
	for ( simpleGuildMap::iterator it = m_AllGuildMap.begin(); it != m_AllGuildMap.end(); it++ )
	{
		if ( stricmp( (it->second).m_name, name ) == 0 )
			return it->first;
	}
	return 0;
}

// 玩家離開自己的公會
void Guild_internal::LeaveGuildMember		( int LeaderDBID , int MemberDBID )
{
	RemoveMemberByKick(LeaderDBID,MemberDBID);

}


//加入新成員
void Guild_internal::JoinGuildMember( GuildMemberStruct& Member )
{
	UpDateNewMember(Member,true);
}

//更新資料
void Guild_internal::UpDateNewMember(GuildMemberStruct& member,bool newMember)
{

	if (!g_pGuildFrame->isHaveGuild())
		return ;
	int i;
	bool bFind=false;
	int count=(int)m_MemberList.size();

	for(i=0;i<count;i++)
	{
		CachGuildMember&mber=m_MemberList[i];
		if (mber.m_info.MemberDBID==member.MemberDBID)
		{
			mber.m_info=member;
			mber.m_updateTime=timeGetTime();
			bFind=true;
			break ;
		}

	}
	if (i==count)
	{
		CachGuildMember temp;
		temp.m_info=member;
		temp.m_updateTime=timeGetTime();

		m_MemberList.push_back(temp);

	}

	GuildMemberBaseInfoStruct temp;
	temp.	MemberDBID	= member.	MemberDBID;
	temp.	MemberName	= member.	MemberName;
	temp.	Rank		= member.	Rank;				//角色公會等級
	temp.	Voc			= member.	Voc;
	temp.	Voc_Sub		= member.	Voc_Sub;
	temp.	Sex			= member.	Sex;
	temp.	LV			= member.	LV;
	temp.	LV_Sub		= member.	LV_Sub;
	temp.	GroupID		= member.	GroupID;
	temp.	IsOnline	= member.	IsOnline;
	temp.	GUID		= -1;
	GuildMemberBaseInfoStruct * info=GetMemberBaseInfo( member.MemberDBID);
	if (info)
	{
		temp.GUID=info->GUID;
		*info=temp;
	}
	else
		m_baseMemberList.push_back(temp);	







	UpdateMemberInfo();

	if (newMember)
		g_pGuildFrame->AddMember(member.MemberName.Begin());
	g_pGuildFrame->UpdateMemberInfo(member.MemberDBID);
}
void CGuildFrame::GuildInfoChange( GuildInfoChangeStruct& ChangeMode )
{

	if (ChangeMode.GuildMessage)
	{

		int i;
		for(i=0;i<6;i++)
		{
			m_BBSTime[i]=-1;
		}


		SendWorldEvent("GUILD_BBS_MESSAGEUPDATE");
		//NetCli_GuildChild::SL_GuildInfoRequet(CNetGlobal::RoleData()->BaseData.GuildID);
	}

	//if (g_internal)
	//{
	//	g_internal->GuildInfoChange(ChangeMode);
	//}


}

//成員的詳細資料
void Guild_internal::GuildMemberInfo( GuildMemberStruct& Member )
{
	_Guild_data* date=GetSelfGuildData();
// 判別是否公會成員
	if (date&&Member.GuildID==date->m_info.GuildID)
	{
		
		UpDateNewMember(Member,false);

	}
	
	g_pGuildFrame->AddMemberInfo(Member.MemberDBID);

}
//加入公會
void Guild_internal::JoinGuild			( int GuildID , char* LeaderName )
{
	char tmp[MAX_PATH];
	const char *pStr=g_ObjectData->GetString("GUILD_MSG_JOINGUILD");

	sprintf_s(tmp, MAX_PATH,pStr,   LeaderName,GetGuildNameByID(GuildID));
	g_pChatFrame->SendMsgEvent("CHAT_MSG_SYSTEM", tmp, "");

	//g_pDebugMessage->DebugOutput( 2 , "加入公會 %s LeaderName=%s" , GetGuildNameByID(GuildID) , LeaderName );
	CNetGlobal::RoleData()->BaseData.GuildID = GuildID;

	// 要求重新取得自己公會資訊
	ReloadGuildDate();

}
//加入連署公會
void Guild_internal::JoinNewGuild( int GuildID , char* LeaderName )
{
	char tmp[MAX_PATH];
	const char *pStr=g_ObjectData->GetString("GUILD_MSG_INVATE_NEW");
	sprintf_s(tmp,MAX_PATH, pStr , LeaderName, GetGuildNameByID(GuildID ));
	g_pChatFrame->SendMsgEvent("CHAT_MSG_SYSTEM", tmp, "");




	CNetGlobal::RoleData()->BaseData.GuildID = GuildID;

	// 要求重新取得自己公會資訊
	ReloadGuildDate();

}

//設定使用組群

void Guild_internal::SetUserGroup(bool bGroup)
{
	m_UserGroup=bGroup;
}

//取得自己公會的基本資料
_Guild_data* Guild_internal::GetSelfGuildData(void)
{
	int GuildID=CNetGlobal::RoleData()->BaseData.GuildID;
	_Guild_data* guildInfo=GetGuildData(GuildID);

	return guildInfo;

}



	
GuildMemberBaseInfoStruct *  CGuildFrame::GetMemberBaseInfo(int dbid)
{

	return g_internal->GetMemberBaseInfo(dbid);

}


//找基本資料
GuildMemberBaseInfoStruct * Guild_internal::GetMemberBaseInfo(int dbid)
{
	GuildMemberBaseInfoStruct * ret=NULL;
	int count=(int)m_baseMemberList.size();

	
	for (int i=0;i<count ;i++)
	{
		GuildMemberBaseInfoStruct * obj=&m_baseMemberList[i];
		if (obj->MemberDBID==dbid)
		{
			ret=obj;
			break;
		}
	}
	return ret;

}

//秀出訊息誰離開了
void Guild_internal::ShowMemberLeave(const char  *name)
{

	 int GuildID=CNetGlobal::RoleData()->BaseData.GuildID;
	_Guild_data* guildInfo=GetGuildData(GuildID);

	if (guildInfo)
	{
		char temp[512];
		const char *GuildLeaveMsg;
		if (guildInfo->m_info.IsReady)


			GuildLeaveMsg=g_ObjectData->GetString("GUILD_MSG_REMOVEMEMBER_RGUILD");
		else
			GuildLeaveMsg=g_ObjectData->GetString("GUILD_MSG_REMOVEMEMBER_GUILD");

		sprintf_s(temp,512,GuildLeaveMsg,name);
		g_pChatFrame->SendMsgEvent(CHAT_MSG_GUILD, temp,"" );

	}
}

//找自已基本資料
GuildMemberBaseInfoStruct *Guild_internal::GetSelfBaseInfo(void)
{
	int selfID=g_pGameMain->GetDBID();	
	return GetMemberBaseInfo(selfID);	
}

//取得被選擇的資料
GuildMemberBaseInfoStruct* Guild_internal::GetSelectBaseInfoStruct(void)
{	
	return GetMemberBaseInfo(m_RosterSelectionDBID);
}
void Guild_internal::RemoveMemberByKick(int LeaderDBID,int MemberDBID)
{
	if (MemberDBID==g_pGameMain->GetDBID())
	{//我被踢了
		g_pGuildFrame->YouGetFire();
		CNetGlobal::RoleData()->BaseData.GuildID =0;

		g_pGuildFrame->ClearGuildInfo();
		g_pGuildFrame->UpdateGuild();

		g_pGameMain->SetSpriteGuildID( g_pGameMain->GetPlayer(), 0 );
	}
	else
	{
		GuildMemberBaseInfoStruct * ret=	GetMemberBaseInfo(MemberDBID);	
		if (ret)


		ShowMemberLeave(ret->MemberName.Begin());

		RemoveSInfoMember(MemberDBID);

		RemoveMemberByID(MemberDBID);
		
		
		UpdateMemberInfo();


	}

}

// 要求重新取得自己公會資訊
void Guild_internal::ReloadGuildDate(void)
{	
	NetCli_GuildChild::SL_GuildInfoRequet(CNetGlobal::RoleData()->BaseData.GuildID);
	NetCli_GuildChild::SL_GuildMemberListRequet(CNetGlobal::RoleData()->BaseData.GuildID);
}

//取得公會名稱
const char *Guild_internal::GetGuildNameByID(int GuildID)
{
	const char * name=NULL;
	_Guild_data*date=GetGuildData(GuildID);
	if (date)
		name=date->m_name;
	
	return name;


}
//收到公會的會員列表
void Guild_internal::GuildMemberBaseInfo( vector< GuildMemberBaseInfoStruct >& MemberList)
{
	// 判別是否自己公會
	if ( NetCli_Guild::_GuildID == CNetGlobal::RoleData()->BaseData.GuildID )
	{	
		m_baseMemberList = MemberList;

		// 成員資料更新,需要更新顯示界面
		UpdateMemberInfo();
	}
}

//取得公會基本資料
void Guild_internal::GuildBoardRequest( int guildID,bool& v )
{
	_Guild_data* date= GetGuildData (guildID);
	if (date)
	{
		int nowTime = timeGetTime();
		if ( ( nowTime - date->m_updateTime ) > DF_GUILDINFO_UPDATETIMER )
		{
									
			v=true;//資料變舊了　向server討
			 NetCli_Guild::SL_GuildInfoRequet(guildID);
			date->m_updateTime = timeGetTime();
			

		}
		else
		{
			SendEvent_GuildBoardResult( date );

		}
	}	
}



//離開連署公會.　誰 　
void Guild_internal::LeaveNewGuildResult	( char* KickPlayerName , bool Result )
{
	char tmp[MAX_PATH];
	if( Result )
	{	
		const char* selfName = CNetGlobal::RoleData()->BaseData.RoleName.Begin();		
		if (strcmp(selfName,KickPlayerName)==0) 	/*自己離開公會.*/
		{
			int GuildID=CNetGlobal::RoleData()->BaseData.GuildID;
			
			const char *GuildLeaveMsg=g_ObjectData->GetString("GUILD_MSG_SELF_LEAVED");
			
			_Guild_data* ret=GetGuildData(GuildID);
			const char *GuildName="";
			if (ret)			
				GuildName=ret->m_name;
			sprintf_s(tmp, MAX_PATH,GuildLeaveMsg, GuildName);
			g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, tmp, "");

			CNetGlobal::RoleData()->BaseData.GuildID = 0;
		
			g_pGuildFrame->ClearGuildInfo();
			g_pGuildFrame->UpdateGuild();

			g_pGameMain->SetSpriteGuildID( g_pGameMain->GetPlayer(), 0 );

			g_pGuildFrame->SendWorldEvent("GUILD_KICK");
		}
		else /*kick 其他成員.*/
		{
			//要求新的名單
			//SL_GuildMemberListRequet(_RoleData.BaseData.GuildID);
		//	ShowMemberLeave(KickPlayerName);
			g_internal->RemoveMember(KickPlayerName);
		//	const char *GuildLeaveMsg=g_ObjectData->GetString("GUILD_MSG_REMOVEMEMBER_GUILD");
		//	sprintf(tmp, GuildLeaveMsg , KickPlayerName);
		//	g_pChatFrame->SendMsgEvent(CHAT_MSG_GUILD, tmp, "");
		}		
	}
	else
	{
		const char *pStr=g_ObjectData->GetString("CRAFT_FRAME_EXIT");
		const char *pStrFail=g_ObjectData->GetString("FAIL");
		sprintf_s(tmp,MAX_PATH, "%s %s%s" , KickPlayerName,pStr,pStrFail);
		g_pChatFrame->SendMsgEvent(CHAT_MSG_GUILD, tmp, "");
	}

}

//連署公會的邀請
void Guild_internal::NewGuildInvite( int GuildID, char* LeaderName, int LeaderDBID )
{
	char tmp[MAX_PATH];
	//sprintf(tmp, "邀請加入連署公會(%d) 要請人(%s) DBID(%d)", GuildID, LeaderName, LeaderDBID);

	const char * pStr=g_ObjectData->GetString("ASK_INVITE_TO_GUILD");

	sprintf_s(tmp,MAX_PATH,pStr, LeaderName, GetGuildNameByID(GuildID));
	g_pChatFrame->SendMsgEvent("CHAT_MSG_SYSTEM", tmp, "");

	g_pGuildFrame->PetitionSignature(GuildID, LeaderName, LeaderDBID);
}


//創立公會的回覆
void Guild_internal::CreateGuildResult( int GuildID , char* GuildName , int Result )
{
	char tmp[MAX_PATH];
	const char * pStr=g_ObjectData->GetString("GUILD_NEWGUILD");
	const char * strResult=g_ObjectData->GetString("FAIL");
	switch( Result )
	{
	case EM_CreateGuildResult_OK:			//成?

		strResult=g_ObjectData->GetString("SUCCEEDED");
		CNetGlobal::RoleData()->BaseData.GuildID = GuildID;


		// 要求重新取得自己公會資訊
		ReloadGuildDate();

		break;
	case EM_CreateGuildResult_Failed_Exist:				//已有公會
		strResult=g_ObjectData->GetString("GUILD_HAVEDGUILD");
		break;
	case EM_CreateGuildResult_Failed_Name:				//公會名稱重復or 字串有問題
		strResult=g_ObjectData->GetString("GUILD_ERROR_MSG");
		break;

	case EM_CreateGuildResult_Failed_Money:				//金錢不足

		strResult=g_ObjectData->GetString("GUILD_ERROR_MSG1");
		break;	
	case EM_CreateGuildResult_NameToShort:				// 名稱太短,不合法
			
	case EM_CreateGuildResult_NameToLong:				// 名稱太長,不合法
	
	case EM_CreateGuildResult_NameIsWrongful:			// 有錯誤的符號或字元
		
	case EM_CreateGuildResult_HasSpecialString:			// 特殊名稱不能用
	
	case EM_CreateGuildResult_OnlyBasicLetters:			// 只能使用基本英文字母
		
	case EM_CreateGuildResult_EndLetterError:			// 結尾字元錯誤(',")


		strResult=GetNameCheckErr(Result-EM_CreateGuildResult_NameToShort);
		break;


	}

	sprintf_s(tmp,MAX_PATH, "%s %s %s", pStr, GuildName,strResult);
	g_pChatFrame->SendMsgEvent("CHAT_MSG_SYSTEM", tmp, "");



	g_pGuildFrame->CloseGuildCommand();


}

//-----------------------------------------------------------------------------
////選到哪個?
int Guild_internal::GetGuildRosterSelection(void)
{
	return m_RosterSelection;
}

//-----------------------------------------------------------------------------
//設定群組開關
void Guild_internal::SetGuildHeaderState(int index, bool isExpand)
{
	GuildMemberListInfo* info = GetGuildMemberInfo(index);
	if ( info && info->isHeader )
	{
		if ( *info->pIsExpand != isExpand )
		{
			*info->pIsExpand = isExpand;
			UpdateMemberInfo();
		}
	}
}
//取得索引表
GuildMemberListInfo * Guild_internal::GetGuildMemberInfo(int index)
{
	GuildMemberListInfo *ret=NULL;
	if (index>=0&&index <(int )m_GuildMemberInfo.size())
		ret=&m_GuildMemberInfo[index];

	return ret;


}


//-----------------------------------------------------------------------------
//取得公會顯示表格數量
int Guild_internal::GetNumGuildMembers(void)
{
	int count=0;
	int GuildID=CNetGlobal::RoleData()->BaseData.GuildID;
	
	if (GuildID>0)
		count=(int)m_GuildMemberInfo.size();

	return count;//
}

//-----------------------------------------------------------------------------
//取得自己的資料
CachGuildMember * Guild_internal::GetSelfInfo()
{
	int selfID=g_pGameMain->GetDBID();	
	return GetMemberInfo(selfID);
}

//找會員詳細資料
CachGuildMember * Guild_internal::GetMemberInfo(int dbid)
{
	CachGuildMember * ret=NULL;
	SinfoGuildMemberList::iterator it=m_MemberList.begin();
	for(;it!=m_MemberList.end();++it)
	{		
		if (it->m_info.MemberDBID==dbid)
		{			
			ret=&(*it);
			break;
		}
	}
	return ret;
}

//向server 討會員詳細資料
void  Guild_internal::ReloadGuildMemberDate(int dbid)
{

	CachGuildMember *ret=GetMemberInfo(dbid);

	bool reload=false;
	if (ret)
	{
		//檢查時間過期了
		int time=timeGetTime();
		if ((time-ret->m_updateTime)>DF_GUILDMEMBER_UPDATETIMER)
			reload=	false;		
	}
	if (!ret||reload)
		NetCli_GuildChild::SL_GuildMemberInfoRequet(dbid);
}

//ui　觸發選到
void Guild_internal::SetGuildRosterSelection(int index)
{
	GuildMemberListInfo* info = GetGuildMemberInfo(index);
	if ( info && !info->isHeader )
	{
		m_RosterSelectionDBID = info->member->MemberDBID;
		m_RosterSelection = index;
		ReloadGuildMemberDate(m_RosterSelectionDBID);
	}
	else
	{
		m_RosterSelectionDBID = 0;
		m_RosterSelection = -1;
	}
}
//重造會員列表
void Guild_internal::UpdateMemberInfo()
{
	int GuildID=CNetGlobal::RoleData()->BaseData.GuildID;
	m_GuildMemberInfo.clear();
	m_RosterSelection = -1;

	if ( GuildID != 0 && !m_baseMemberList.empty() )
	{
		_Guild_data* date=GetGuildData(GuildID);
		GuildMemberListInfo info;
		if ( date&& date->m_info.IsReady&& m_UserGroup)			// 正式成立公會組識
		{
			for ( int i = 0; i < _MAX_GUILD_GROUP_COUNT_; i++ )
			{
				info.isHeader		= true;
				info.pIsExpand		= &(m_GroupIsExpand[i]);
				info.groupName		= date->m_info.Group[i].Begin();
				m_GuildMemberInfo.push_back(info);

				if ( *(info.pIsExpand) )
				{
					vector<GuildMemberBaseInfoStruct>::iterator iter = m_baseMemberList.begin();
					for ( ; iter != m_baseMemberList.end(); ++iter )
					{
						if ( (*iter).GroupID == i )
						{
							// 選取項目
							if ( m_RosterSelectionDBID == (*iter).MemberDBID )
								m_RosterSelection = (int)m_GuildMemberInfo.size();

							info.isHeader	= false;
							info.pIsExpand	= NULL;
							info.member		= &(*iter);
							m_GuildMemberInfo.push_back(info);
						}
					}
				}
			}
		}
		else
		{
			vector<GuildMemberBaseInfoStruct>::iterator iter = m_baseMemberList.begin();
			for ( ; iter != m_baseMemberList.end(); ++iter)
			{
				// 選取項目
				if ( m_RosterSelectionDBID == (*iter).MemberDBID )
					m_RosterSelection = (int)m_GuildMemberInfo.size();

				NetCli_OtherChild::InsertDBIDRoleName(iter->MemberDBID,iter->MemberName.Begin());

				info.isHeader	= false;
				info.pIsExpand	= NULL;
				info.member		= &(*iter);
				m_GuildMemberInfo.push_back(info);
			}
		}
	}
	if ( m_RosterSelection == -1 )
		m_RosterSelectionDBID = 0;
	g_pGuildFrame->UpdateGuild();


}
void  Guild_internal::RemoveSInfoMember(int dbid)
{			
	SinfoGuildMemberList::iterator it=m_MemberList.begin();
	for(;it!=m_MemberList.end();++it)
	{		
		if (it->m_info.MemberDBID==dbid)
		{
			m_MemberList.erase(it);
			break;
		}
	}
}
//移走會員
void Guild_internal::RemoveMemberByID(int dbid)
{
	bool find=false;

	simpleGuildMemberList::iterator iter = m_baseMemberList.begin();

	for ( ; iter != m_baseMemberList.end(); ++iter )
	{
		if ( iter->MemberDBID==dbid )
		{
			m_baseMemberList.erase(iter);
			find=true;
			break;
		}
	}
	if (find)
		UpdateMemberInfo();

}

//移走會員
void Guild_internal::RemoveMember(char *MemberNAME)
{
	bool find=false;
	
	simpleGuildMemberList::iterator iter = m_baseMemberList.begin();

	for ( ; iter != m_baseMemberList.end(); ++iter )
	{
		if ( strcmp((*iter).MemberName.Begin(), MemberNAME) == 0 )
		{
			m_baseMemberList.erase(iter);
			find=true;
			break;
		}
	}
	if (find)
		UpdateMemberInfo();
}

//踢誰離開公會 　使用踢人指令者會收到
void Guild_internal::LeaveGuildResult	( char* KickPlayerName , bool Result )
{
	char tmp[MAX_PATH];
	// 不能分辨自己離開跟被踢除公會
	if( Result )//成?
	{
		const char* selfName = CNetGlobal::RoleData()->BaseData.RoleName.Begin();		 
		if (strcmp(selfName,KickPlayerName)==0) //自踢
		{
			
			g_pGuildFrame->SelfLeaveGuid();//呼叫自己離開公會
			CNetGlobal::RoleData()->BaseData.GuildID = 0;//清除公會ID
			
		}
		else /*kick 其他成員.*/
		{
			//要求新的名單
			//SL_GuildMemberListRequet(_RoleData.BaseData.GuildID);

			//sprintf(tmp, "%s 離開公會" , KickPlayerName);
			//	g_pChatFrame->SendMsgEvent(CHAT_MSG_GUILD, tmp, "");

			//ShowMemberLeave(KickPlayerName);
			RemoveMember(KickPlayerName);			
		}

	}
	else//失敗　踢除某人失敗
	{

		const char *pStr=g_ObjectData->GetString("CRAFT_FRAME_EXIT");
		const char *pStrFail=g_ObjectData->GetString("FAIL");
		sprintf_s(tmp,512, "%s %s%s" , KickPlayerName,pStr,pStrFail);
		g_pChatFrame->SendMsgEvent(CHAT_MSG_GUILD, tmp, "");
	}
}

void Guild_internal::SendEvent_GuildBoardResult( _Guild_data* data )
{
	lua_State* L = g_pGuildFrame->GetLuaState();

	//lua_pushnumber( L, Guild.GuildID );
	//lua_setglobal( L, "GuildID" );


	//公會等級
	lua_pushnumber( L, data->m_info.Level );
	lua_setglobal( L, "GuildLevel" );

	//公會名稱
	lua_pushstring( L, data->m_info.GuildName.Begin() );
	lua_setglobal( L, "GuildName" );

	//會長名稱
	lua_pushstring( L, data->m_info.LeaderName.Begin() );
	lua_setglobal( L, "LeaderName" );

	//現有人數
	lua_pushnumber( L, data->m_MemberCount );
	lua_setglobal( L, "MemberCount" );

	//最多可以有多少會員
	lua_pushnumber( L, data->m_info.MaxMemberCount );
	lua_setglobal( L, "MaxMemberCount" );

	//是否招生
	lua_pushboolean( L, data->m_info.IsRecruit );
	lua_setglobal( L, "IsRecruit" );

	//公會據點
	//lua_pushboolean( L, data->info.IsRecruit );
	//lua_setglobal( L, "GuildFootHold" );

	//公會屋
	lua_pushboolean( L, data->m_info.IsOwnHouse );
	lua_setglobal( L, "IsOwnHouse" );

 	//開放參觀
	lua_pushboolean( L, !data->m_info.IsLockGuildHouse );
	lua_setglobal( L, "IsOpenVisit" );

	//留言版
	lua_pushboolean( L,true/* data->info.IsEnabledGuildBoard */);
	lua_setglobal( L, "isBBSEnable" );

	//公會敘述
	lua_pushstring( L, data->m_info.Introduce.SafeStr() );
	lua_setglobal( L, "Introduce" );

	//int		GuildID;									//公會ID
	//StaticString< _MAX_NAMERECORD_SIZE_ >   GuildName;	//公會名稱
	//StaticString< _MAX_NAMERECORD_SIZE_ >   LeaderName;	//會長名稱
	//int		LeaderDBID;
	//bool	IsRecruit;									//是否招生
	//bool	IsReady;									//是否正式成立
	//bool	IsNeedSave;									//如果資料有更動則會為 true
	//int		Medal;										//徽章
	//int		MaxMemberCount;								//最多可以有多少會員
	//int		Score;										//公會積分
	//StaticString< 512 >   Note;							//公告
	//StaticString< 512 >   Introduce;					//公會介紹

	//int		RankCount;									//公會階層數
	//GuildRankBaseInfoStruct	Rank[ EM_GuildRank_Count ];						//每個等級的權限 與名稱
	//StaticString< _MAX_NAMERECORD_SIZE_ >   Group[_MAX_GUILD_GROUP_COUNT_];	//群組名稱


	m_interface->GetUiWorld()->SendEvent( UiEvent_GuildBoardResult );

}


// 收到某公會更名
void Guild_internal::ChangeGuildName( int GuildID , char* GuildName )
{

	_Guild_data* info = GetGuildData(GuildID);
	if (!info)
	{
		_Guild_data temp;
		strncpy(temp.m_name,GuildName,_MAX_NAMERECORD_SIZE_);
		temp.m_lev=0;
		temp.m_updateTime = -DF_GUILDINFO_UPDATETIMER;
		temp.m_info.Init();
		temp.m_info.GuildID = GuildID;
		
		temp.m_info.GuildName=GuildName;
		m_AllGuildMap.insert(pair<int,_Guild_data>(GuildID,temp));

		

	}
	else
	{
		strncpy(info->m_name,GuildName,_MAX_NAMERECORD_SIZE_);
		info->m_info.GuildName=GuildName;

	}

	//g_pGameMain->ChangeGuildName(GuildID,GuildName);

}

//某公會基本資料　維護公會列表
void Guild_internal::GuildBaseInfo( int GuildID , char* GuildName , GuildFlagStruct& Flag , bool IsReady )
{
	bool bIsChange = false;

	_Guild_data* info = GetGuildData(GuildID);
	if (!info)
	{
		_Guild_data temp;
		strncpy(temp.m_name,GuildName,_MAX_NAMERECORD_SIZE_);
		temp.m_lev=IsReady?1:0;
		temp.m_updateTime = -DF_GUILDINFO_UPDATETIMER;
		temp.m_info.Init();
		temp.m_info.GuildID = GuildID;
		temp.m_info.GuildFlag = Flag;
		m_AllGuildMap.insert(pair<int,_Guild_data>(GuildID,temp));

		bIsChange = true;		
	}
	else 
	{
		strncpy(info->m_name,GuildName,_MAX_NAMERECORD_SIZE_);
		info->m_lev = IsReady ? 1 : 0;

		if ( info->m_info.GuildFlag.InsigniaType != Flag.InsigniaType ||
			 info->m_info.GuildFlag.BannerType != Flag.BannerType ||
			 info->m_info.GuildFlag.InsigniaMainColor != Flag.InsigniaMainColor ||
			 info->m_info.GuildFlag.InsigniaOffColor != Flag.InsigniaOffColor ||
			 info->m_info.GuildFlag.BannerMainColor != Flag.BannerMainColor ||
			 info->m_info.GuildFlag.BannerOffColor != Flag.BannerOffColor )
		{
			bIsChange = true;
		}
		info->m_info.GuildFlag = Flag;
	}

	// 重設所有玩家旗子物件
	if ( IsReady && bIsChange )
	{
		GuildFlagStruct* guildFlag = g_pGuildFrame->GetGuildFlagStruct(GuildID);
		if ( guildFlag == NULL )
			return;

		g_pGameMain->ResetObjectGuildFlag(GuildID,
			g_ObjectData->GetGuildFlagInsigniaType(guildFlag->InsigniaType), guildFlag->InsigniaMainColor, guildFlag->InsigniaOffColor,
			g_ObjectData->GuildFlagBannerType(guildFlag->BannerType), guildFlag->BannerMainColor, guildFlag->BannerOffColor);
	}
}
/*
void Guild_internal::GuildInfoChange(GuildInfoChangeStruct& ChangeMode)
{



}
*/
//某公會倒了
void Guild_internal::GuildDestroy( int GuildID  )
{
	simpleGuildMap::iterator it=m_AllGuildMap.find(GuildID);
	if (it!=m_AllGuildMap.end())	
		m_AllGuildMap.erase(it);
	
}

//取得某公會的基本資料
_Guild_data* Guild_internal::GetGuildData(int GuildID)
{
	
	_Guild_data* ret=NULL;
	simpleGuildMap::iterator it=m_AllGuildMap.find(GuildID);
	if (it!=m_AllGuildMap.end())
		ret=&it->second;	
	return ret;
}

//struct SimpleGuildInfoStruct
//{
//int		GuildID;
//GuildResourceStruct Resource;			//資源
//};
//收到公會資源變動
void Guild_internal::ModifySimpleGuild( SimpleGuildInfoStruct& SimpleGuildInfo )
{

	int SelfGuildID = CNetGlobal::RoleData()->BaseData.GuildID;

	_Guild_data* ret=GetGuildData( SimpleGuildInfo.GuildID);

	if (ret)
	{
		ret->m_info.Resource=SimpleGuildInfo.Resource;
	
	

		if (SelfGuildID==SimpleGuildInfo.GuildID)
		{
			
			g_pGuildFrame->SendWorldEvent("UPDATE_GUILD_INFO");
		}

	}


}
//得到某公會的基本資料
void  Guild_internal::GuildInfo(GuildBaseStruct& guild,int memberCount,bool bNeedSend)
{
	int SelfGuildID = CNetGlobal::RoleData()->BaseData.GuildID;

	_Guild_data* ret=GetGuildData( guild.GuildID);

	if (!ret)
	{
		_Guild_data temp;
		strncpy(temp.m_name,guild.GuildName,_MAX_NAMERECORD_SIZE_);
		temp.m_lev=guild.IsReady?1:0;
		temp.m_updateTime = -DF_GUILDINFO_UPDATETIMER;		
	//	temp.m_info.GuildID = GuildID;

		m_AllGuildMap.insert(pair<int,_Guild_data>(guild.GuildID,temp));
		ret=GetGuildData( guild.GuildID);
	}
	
	if (ret)
	{
		ret->m_info=guild;
		ret->m_MemberCount=memberCount;
		if (bNeedSend)
			SendEvent_GuildBoardResult( ret );
	}
	

	//如果是我的公會 更新自己的
	if (SelfGuildID>0&&guild.GuildID == SelfGuildID)
	{		

		GuildBaseStruct& myGuild=ret->m_info;
		//我的公會資訊
		const char *Msg=g_ObjectData->GetString("SYS_GUILD_RANK_LIST");
		char temp[1024];
		strcpy(temp,Msg);

		if (strlen(temp)==0)
		{
			temp[0]=0;
			const char *Msg=g_ObjectData->GetString("GUILD_N_COLON");
			
			char Rank[512];
			int i;
			for (i=10;i>0;--i)
			{
				sprintf_s(Rank,512,Msg,i);
				strcat_s(temp,1024,Rank);
			}
		}


		char *p=strtok(temp,",");

		for (int i=9;i>=0;--i)
		{
			const char *pRankName=myGuild.Rank[i].RankName.Begin();
			int len=(int)strlen(pRankName);

			if (len<=0)
			{
				if (p)
					myGuild.Rank[i].RankName=p;
			}
			p=strtok(NULL,",");

		}









		// 資料更新,需要更新顯示界面
		g_pGuildFrame->UpDateGuildInfo(ret->m_info);
		UpdateMemberInfo();

	}

	

/*
	
	if ( Guild.GuildID == SelfGuildID)
	{
		bool isInitial = false;

		// 初始化
		if ( s_GuildInfo.GuildID == 0 || (s_GuildInfo.IsReady != Guild.IsReady) )
			isInitial = true;

		s_GuildInfo = Guild;
		if ( isInitial )
		{
			memset(s_GroupIsExpand, 0, sizeof(s_GroupIsExpand));
		}


		for ( int i = 0; i < _MAX_GUILD_GROUP_COUNT_; i++ )
		{
			if ( strlen(s_GuildInfo.Group[i].Begin()) <= 0 )
			{
				char tmp[MAX_PATH];

				const char *pStr=g_ObjectData->GetString("UI_FRIENDINFO_GROUP");
				sprintf(tmp, "%s%d", pStr,i);
				s_GuildInfo.Group[i] = tmp;
			}
		}

	
	}
	*/

	
}


bool sortPostList(int postB,int postA)
{
	CGuildFrame::BBSPost *A,*B;
	A=g_pGuildFrame->GetPostInfo(postA);
	B=g_pGuildFrame->GetPostInfo(postB);

	if (A->m_post.Type.Top)
	{
		if (B->m_post.Type.Top)
			return A->m_post.Time<B->m_post.Time;
		else 
			return false;
	}
	else
	{
		if (B->m_post.Type.Top)
			return true;
		else
			return A->m_post.Time<B->m_post.Time;
	}
	return 0;
}
// ----------------------------------------------------------------------------------
CGuildFrame::CGuildFrame(CInterface* object)
: CInterfaceBase(object),m_bCanSend(true),m_bshowMessage(false)
{

	g_internal=new Guild_internal(object);
	m_BBSPage=0;
	m_Open=0;
	m_ModifyTime=0;
	m_BBSPostTime=timeGetTime()-DF_POST_COOLDOWN;
		m_GuildDateLive=timeGetTime();


	m_reloadIndex=0;
	m_maxBBSPage=0;


	m_guildID		= 0;
	m_leaderDBID	= 0;
	m_leaderName.clear();

	g_pGuildFrame = this;
	m_haveGuild=false;

	for (int i=0;i<6;i++)
	m_BBSTime[i]=-1;

	memset(&m_tempGuildInfo,0,sizeof(m_tempGuildInfo));
	
}

// ----------------------------------------------------------------------------------
CGuildFrame::~CGuildFrame()
{
	delete g_internal;
	g_internal=NULL;

}
void CGuildFrame::SetGuildResLogDay(int day)
{

	if (g_internal->SetLogDay(day))
	{
		lua_State* L = GetLuaState();
		lua_pushnumber(L,day+1);
		lua_setglobal(L, "arg1");
		SendWorldEvent("GRF_UPDATELOG");
	}


}
// ----------------------------------------------------------------------------------
void CGuildFrame::RegisterFunc()
{
	lua_State* L = GetLuaState();
	if (L)
	{
		lua_register(L, "IsInGuild",				LuaFunc_IsInGuild);
		lua_register(L, "GuildCreate",				LuaFunc_GuildCreate);
		lua_register(L, "GetGuildInfo",				LuaFunc_GetGuildInfo);
		//lua_register(L, "GuildPetitionSignature",	LuaFunc_GuildPetitionSignature);
		lua_register(L, "GuildPetitionAccept",		LuaFunc_GuildPetitionAccept);
		lua_register(L, "GuildPetitionDecline",		LuaFunc_GuildPetitionDecline);
		lua_register(L, "GuildInviteResult",		LuaFunc_GuildInviteResult);


		
		lua_register(L, "GuildPetitionQuit",		LuaFunc_GuildPetitionQuit);
		lua_register(L, "GuildPetitionKick",		LuaFunc_GuildPetitionKick);
		lua_register(L, "GetNumGuildMembers",		LuaFunc_GetNumGuildMembers);
		lua_register(L, "GetGuildRosterInfo",		LuaFunc_GetGuildRosterInfo);
	


		

		lua_register(L, "GuildLeave",				LuaFunc_GuildLeave);
		lua_register(L, "GuildInvite",				LuaFunc_GuildInvite);
		lua_register(L, "GuildUninvite",			LuaFunc_GuildUninvite);
		
		lua_register(L, "ExpandGuildGroup",			LuaFunc_ExpandGuildGroup);
		lua_register(L, "CollapseGuildGroup",		LuaFunc_CollapseGuildGroup);

		lua_register(L, "SetGuildRosterSelection",	LuaFunc_SetGuildRosterSelection);
		lua_register(L, "GetGuildRosterSelection",	LuaFunc_GetGuildRosterSelection);

		lua_register(L, "GF_GetRankCount",	LuaFunc_GF_GetRankCount);
		lua_register(L, "GF_GetRankStr",	LuaFunc_GF_GetRankStr);
		lua_register(L, "GF_SetGuildDesc",	LuaFunc_GF_SetGuildDesc);

		lua_register(L, "GC_CloseGuildCommand",	LuaFunc_GC_CloseGuildCommand);
		
		
		lua_register(L, "GF_GetRankInfo",	LuaFunc_GF_GetRankInfo);
		lua_register(L, "GF_CanGuildWaring",	LuaFunc_GF_CanGuildWaring);

	
		lua_register(L, "GF_SetRankInfo",	LuaFunc_GF_SetRankInfo);
		lua_register(L, "GF_SetMemberRank",	LuaFunc_GF_SetMemberRank);
		lua_register(L, "GF_SetMemberCadreNote",	LuaFunc_GF_SetMemberCadreNote);
		lua_register(L, "GF_GetMemberCadreNote",	LuaFunc_GF_GetMemberCadreNote);
		lua_register(L, "GF_SetMemberSelfNote",	LuaFunc_GF_SetMemberSelfNote);
		lua_register(L, "GF_GetMemberSelfNote",	LuaFunc_GF_GetMemberSelfNote);
		lua_register(L, "GF_CanCreateGuildCost",		LuaFunc_GF_CanCreateGuildCost);

		lua_register(L, "GF_Rename",		LuaFunc_GF_Rename);// 公會改名

		//公會留言版
		lua_register(L, "GF_SetGuildNote",		LuaFunc_GF_SetGuildNote);//ui 設定公會告示
		lua_register(L, "GF_GetGuildPostCount",		LuaFunc_GF_GetGuildPostCount); //公會BBS 取得目前留言數量
		lua_register(L, "GF_GetGuildPostInfo",		LuaFunc_GF_GetGuildPostInfo);//公會BBS 取得目前留言資訊
		lua_register(L, "GF_SetGuildPostInfo",		LuaFunc_GF_SetGuildPostInfo);//公會BBS 設定目前留言資訊
		lua_register(L, "GF_SetGuildPost",		LuaFunc_GF_SetGuildPost);// 設定留言
		lua_register(L, "GF_GetGuildPostRequest",		LuaFunc_GF_GetGuildPostRequest);// 取得留言請求
		lua_register(L, "GF_GetPostDate",		LuaFunc_GF_GetPostDate);// 取得留言請求

		
		lua_register(L, "GF_NewPost",		LuaFunc_GF_NewPost);// 新增留言
		//lua_register(L, "GF_DeletePost",		LuaFunc_GF_DeletePost);// 刪除留言
		lua_register(L, "GF_GetBBSInfoRequest",		LuaFunc_GF_GetBBSInfoRequest);// 目前BBSINFO需求


		lua_register(L, "GF_GetPostCountRequest",		LuaFunc_GF_GetPostCountRequest);
		
		lua_register(L, "GF_GetBBSMenuFlag",		LuaFunc_GF_GetBBSMenuFlag);// 取得留言選單
		
		lua_register(L, "GF_SetRankStr",	LuaFunc_GF_SetRankStr);
		lua_register(L, "GF_GuildInfoApply",	LuaFunc_GF_GuildInfoApply);
		lua_register(L, "GF_SetGroupEnable",	LuaFunc_GF_SetGroupEnable);

		lua_register(L, "GF_GetMenuFlag",	LuaFunc_GF_GetMenuFlag);
		lua_register(L, "GF_GetGuildFunc",	LuaFunc_GF_GetGuildFunc);

		lua_register(L, "GF_LeaderChange",	LuaFunc_GF_LeaderChange);

		lua_register(L, "GuildBoard_GetNextGuildIDName"  , Luafunc_GuildBoard_GetNextGuildIDName );
		lua_register(L, "GuildBoard_RequestGuildInfo"    , LuaFunc_GuildBoard_RequestGuildInfo   );
		lua_register(L, "GF_ReadAllDate"    , LuaFunc_GF_ReadAllDate   );
		lua_register(L, "GCB_GetContributionItem"    , LuaFunc_GCB_GetContributionItem   );
		lua_register(L, "GCB_RemoveContributionItem"    , LuaFunc_GCB_RemoveContributionItem   );
		lua_register(L, "GCB_ContributionItemOK"    , LuaFunc_GCB_ContributionItemOK   );
		lua_register(L, "GCB_CloseContribution"    , LuaFunc_GCB_CloseContribution   );
		lua_register(L, "GCB_GetGuildPoint"    , LuaFunc_GCB_GetGuildPoint   );
		lua_register(L, "GCB_GetGuildResource"    , LuaFunc_GCB_GetGuildResource   );
		lua_register(L, "GCB_GetGuildLevel"    , LuaFunc_GCB_GetGuildLevel   );
		lua_register(L, "GCB_GetGuildLevelCount"    , LuaFunc_GCB_GetGuildLevelCount   );
		lua_register(L, "GCB_GetGuildLevelInfo"    , LuaFunc_GCB_GetGuildLevelInfo   );
		lua_register(L, "GCB_GetGuildResLog"    , LuaFunc_GCB_GetGuildResLog   );

		lua_register(L, "GCB_GetGuildResLogAll"    , LuaFunc_GCB_GetGuildResLogAll   );
	
		lua_register(L, "GCB_SetGuildResLogDay"    , LuaFunc_GCB_SetGuildResLogDay   );

		lua_register(L, "GCB_GetGuildResLogCount"    , LuaFunc_GCB_GetGuildResLogCount   );
		lua_register(L, "GCB_GuildUpgrade"    , LuaFunc_GCB_GuildUpgrade   );

		lua_register(L, "GF_AskLeaderChangeResult"		, LuaFunc_GF_AskLeaderChangeResult);
	


		lua_register(L, "GSF_CloseGuildShop"    , LuaFunc_GSF_CloseGuildShop   );
		lua_register(L, "GSF_GetGuildShopCount"    , LuaFunc_GSF_GetGuildShopCount   );
		lua_register(L, "GSF_GetGuildShopFuncInfo"    , LuaFunc_GSF_GetGuildShopFuncInfo   );
		lua_register(L, "GSF_GetGuildShopBuy"    , LuaFunc_GSF_GetGuildShopBuy   );

		lua_register(L, "CheckRightfulName"    , LuaFunc_CheckRightfulName   );

		lua_register(L, "GetGuildWarItemInfo"				, LuaFunc_GetGuildWarItemInfo);
		lua_register(L, "DeclareGuildWar"					, LuaFunc_DeclareGuildWar);
		lua_register(L, "GetGuildWarInfo"					, LuaFunc_GetGuildWarInfo);
		lua_register(L, "GetGuildWarOutdoorsPoint"			, LuaFunc_GetGuildWarOutdoorsPoint);
		lua_register(L, "GetGuildWarPoint"					, LuaFunc_GetGuildWarPoint);
		lua_register(L, "GetGuildWarOutdoorsDeclareInfo"	, LuaFunc_GetGuildWarOutdoorsDeclareInfo);
		lua_register(L, "GetGuildWarOutdoorsNumEnemies"		, LuaFunc_GetGuildWarOutdoorsNumEnemies);
		lua_register(L, "GetGuildWarOutdoorsEnemyInfo"		, LuaFunc_GetGuildWarOutdoorsEnemyInfo);





		lua_register(L, "GetGuildFlagInsigniaCount"			, LuaFunc_GetGuildFlagInsigniaCount);
		lua_register(L, "GuildFlagBannerCount"				, LuaFunc_GuildFlagBannerCount     );
		lua_register(L, "GetGuildFlagInsigniaType"			, LuaFunc_GetGuildFlagInsigniaType );
		lua_register(L, "GuildFlagBannerType"				, LuaFunc_GuildFlagBannerType      );
   		lua_register(L, "GuildFlagInfo"						, LuaFunc_GuildFlagInfo            );
     	lua_register(L, "SetGuildFlagInfoRequest"			, LuaFunc_SetGuildFlagInfoRequest  );

		lua_register(L, "GetSelfGuildRank"					, LuaFunc_GetSelfGuildRank  );



	}
}
//-------------------------------------------
//打開公會命令介面
void          CGuildFrame:: OpenGuildCommand(int iObjID)
{
#ifdef _DEBUG
iObjID=-1;
#endif
	m_npcId=iObjID;

	
	CRoleSprite * npcSprite= g_pGameMain->FindSpriteByID( iObjID );
//	GameObjDbStruct* npcDB = CNetGlobal::GetObj( iObjID );

	if (npcSprite==NULL) 
			g_pDebugMessage->DebugOutput( 2 , "物件資料庫無此NPC %d", iObjID );
	if ( npcSprite  || iObjID==-1)
	{
		// 開啟公會界面
		SendWorldEvent("GUILD_COMMAND_OPEN");
		g_pDebugMessage->DebugOutput( 2 , "開啟公會界面 NPC編號 %d", iObjID );
		m_Open=true;
	}
}

bool			CGuildFrame::IsOpen()
{

	return m_Open;

}

void CGuildFrame::Update(float elapsedTime)
{
	// 更新公會對戰資訊
	vector<GuildDeclareWarInfo>& guildWarInfo = NetCli_GuildChild::s_declareWarInfo;
	for ( vector<GuildDeclareWarInfo>::iterator iter = guildWarInfo.begin(); iter != guildWarInfo.end(); iter++ )
		iter->time -= elapsedTime;


	if (g_internal)
		g_internal->Update( elapsedTime);

	if (m_bshowMessage)
	{
		if ( g_pLoadingFrame &&! g_pLoadingFrame->GetLoadingState() ) 
		{


			struct tm *gmt;
			time_t NowTime;
			time( &NowTime );
			gmt = localtime( &NowTime );
			char temp1[1024];

			sprintf_s( temp1,1024 , "%02d/%02d/%02d %02d:%02d" ,  1 + gmt->tm_mon , gmt->tm_mday,  gmt->tm_year -100, gmt->tm_hour , gmt->tm_min);

			size_t len=1024;

			char temp[1024];
			const char *pSafeString=NULL;
			sprintf_s(temp,len,"[%s] %s", m_tempGuildInfo.Note.SafeStr(),temp1);

			pSafeString=ItemUTIL::checkUtf8SafeString(temp,len,NULL);
			g_pChatFrame->SendMsgEvent(CHAT_MSG_GUILD,pSafeString ,g_ObjectData->GetString("GUILD_MSG_SHORTNOTE") );

			m_bshowMessage=false;
		}

	}

	
	if (!IsOpen() || m_npcId==-1)
		return;

	


	

	
	if ( g_pGameMain->GetDistanceWithPlayer(m_npcId) > _MAX_ROLE_TOUCH_MAXRANGE_ )
	{
		CloseGuildCommand();
	}
}

//-------------------------------------------
//關閉公會命令介面
void          CGuildFrame::CloseGuildCommand()
{
	NetCli_Guild::SL_CloseCreateGuild( );

	// 關閉公會界面
	SendWorldEvent("GUILD_COMMAND_CLOSE");
	m_npcId=-1;
	m_Open=false;
}
// ----------------------------------------------------------------------------------
//通知UI 公會註冊
void CGuildFrame::RegisterGuild()
{
	// 檢查公會是否已成立或者已經加入公會
	if ( CNetGlobal::RoleData()->BaseData.GuildID != 0 )
		return;

	// 跳出訊息視窗
	lua_State* L = GetLuaState();
	if ( L )
	{
		OpenGuildCommand(-1);
	//	SendWorldEvent("GUILD_REGISTER");			// 申請公會
	}
}


void CGuildFrame::AddMember(const char 	*name)
{
	char temp[512];
	sprintf_s(temp,512,g_ObjectData->GetString("GUILD_MSG_ADDMEMBER_GUILD"),name);
	g_pChatFrame->SendMsgEvent(CHAT_MSG_GUILD, temp, "" );
}
void CGuildFrame::YouGetFire(void)
{
	char temp[512];

	_Guild_data* guildInfo=g_internal->GetGuildData(m_tempGuildInfo.GuildID);
	const char *GuildLeaveMsg=NULL;
	const char *GuildName=NULL;// guildInfo->m_name;
	if (guildInfo)
	{

		GuildName= guildInfo->m_name;

		if (guildInfo->m_info.IsReady)
			GuildLeaveMsg=g_ObjectData->GetString("GUILD_MSG_YOUARE_FIRE_RGUILD");
		else
			GuildLeaveMsg=g_ObjectData->GetString("GUILD_MSG_YOUARE_FIRE_GUILD");

		sprintf_s(temp,512,GuildLeaveMsg,GuildName);

		g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, temp,"" );

	}

	



	
	





	m_guildID=0;
	m_haveGuild=false;
	

	SendWorldEvent("GUILD_KICK");	


}


// ----------------------------------------------------------------------------------
//通知UI 會員資料有更新
void CGuildFrame::UpdateMemberInfo(int db)
{
	lua_State* L = GetLuaState();

	if (L)
	{

		GuildMemberStruct* getMemData=GetGuildMemberStruct(db);
		lua_pushnumber(L, db);
		lua_setglobal(L, "arg1");
		//lua_pushnumber(L, getMemData->LeaderNote);
		//lua_setglobal(L, "cadreNode");
		//lua_pushnumber(L, getMemData->SelfNote);
		//lua_setglobal(L, "SelfNote");

		SendWorldEvent("UPDATE_GUILD_MEMBER_INFO");	
	}

}
// ----------------------------------------------------------------------------------
//取得自己的公會資料
bool CGuildFrame::GetGuildInfo(void)
{
	bool ret=true;
	//g_internal->OpenRenameDialog();

	int nowTime=timeGetTime();
	int guildID = CNetGlobal::RoleData()->BaseData.GuildID;	
	if ( guildID != 0 )
	{		
		if ((nowTime-m_GuildDateLive)>DF_GETGUILDINFO_COUNTER)
		{

			m_GuildDateLive=timeGetTime();

			// 要求重新取得自己公會資訊
			g_internal->ReloadGuildDate();
		}
	}
	return ret;
	
	
}
void	 CGuildFrame::OpenGuildShop(int iObjID, int score,StaticFlag<256>& Flag)
{

#ifdef _DEBUG
	iObjID=-1;
#endif
	m_npcId=iObjID;


	CRoleSprite * npcSprite= g_pGameMain->FindSpriteByID( iObjID );
	//	GameObjDbStruct* npcDB = CNetGlobal::GetObj( iObjID );

	if (npcSprite==NULL) 
		g_pDebugMessage->DebugOutput( 2 , "物件資料庫無此NPC %d", iObjID );
	if ( npcSprite  || iObjID==-1)
	{
		// 要求重新取得自己公會資訊
		g_internal->ReloadGuildDate();
		// 開啟公會商店
		SendWorldEvent("GUILD_SHOP_OPEN");
		g_pDebugMessage->DebugOutput( 2 , "開啟公會商店 NPC編號 %d", iObjID );
		m_Open=true;
	}

}
void	 CGuildFrame::CloseGuildShop()
{
//	NetCli_Guild::SL_GuildShopClose( );

	// 關閉公會界面
	SendWorldEvent("GUILD_SHOP_CLOSE");
	m_npcId=-1;
	m_Open=false;

}


// ----------------------------------------------------------------------------------
//取得自己的簡易資料

GuildRankBaseInfoStruct*    CGuildFrame::GetUserRankInfo(void)
{
	GuildRankBaseInfoStruct* ret=NULL;
	GuildMemberBaseInfoStruct* baseInfo = g_internal->GetSelfBaseInfo();
	if ( baseInfo )
	{
		int guildID = CNetGlobal::RoleData()->BaseData.GuildID;
		_Guild_data *date=g_internal->GetGuildData(guildID);
		if (date)
			ret=&date->m_info.Rank[baseInfo->Rank]; 
		
	}
	return ret;

}


int CGuildFrame::GetUserRank(void)
{
	GuildMemberBaseInfoStruct* baseInfo = g_internal->GetSelfBaseInfo();
	if ( baseInfo )
	{
		return baseInfo->Rank; 
	}
	return 0;
}

// ----------------------------------------------------------------------------------
//建立公會

bool  CheckStr(const char *guildName,int len)
{
	/*
	WCHAR temp[2048];

	size_t destLen=64;

	const char *	stringa=ItemUTIL::checkUtf8SafeString(guildName,destLen,temp);
	*/

	if (len<=24)
	{
	CountryTypeENUM language = (CountryTypeENUM)g_ObjectData->GetDataLanguageID();

	std::wstring widestring;
	int resultCode=0;

	

	
	resultCode = g_ObjectData->CheckGuildNameRules(guildName, (CountryTypeENUM)language, widestring);

	

	if ( resultCode != EM_CharacterNameRules_Rightful )
		return false;
	}
	return g_pGameMain->CheckPatternString(guildName);
}

bool CGuildFrame::CreateGuild(const char* guildName)
{

	bool ret=false;
	// 檢查公會是否已成立或者已經加入公會
	if ( CNetGlobal::RoleData()->BaseData.GuildID != 0 )
	{
		g_pDebugMessage->DebugOutput( 2 , "你自己已有工會%s",NetCli_GuildChild::GetGuildNameByID(CNetGlobal::RoleData()->BaseData.GuildID));
		return ret;
	}
	/*
	
	
	int len=strlen(guildName);

	int j=0;
	for (int i=0;i<len;i++)
	{

		if (guildName[i]>=0x80)
		{
			temp[j++]=guildName[i++];
			temp[j++]=guildName[i];

		}
		else  if (guildName[i]!='\'')
		{
			temp[j++]=guildName[i];
		}
			//吃掉單引號
		

	}
	temp[j]=0;
	*/

	char temp[256];
	strcpy(temp,guildName);
		//CheckSqlStr(temp);
	if (CheckStr(temp)==true)//改成 UTF8 不適用CheckSqlStr檢查 
	{
		NetCli_Guild::SL_CreateGuild((char*)guildName);
		ret=true;
	}
	//else
	/*{
		sprintf(temp,g_ObjectData->GetString("GUILD_NAME_INVATE"),guildName);
		g_pChatFrame->SendMsgEvent(CHAT_MSG_GUILD, temp,"" );
	}*/

	return ret;
}
// ----------------------------------------------------------------------------------
//--上傳公會資訊
void CGuildFrame::GuildInfoApply(void)
{
	{//權限檢查
		GuildRankBaseInfoStruct* ri=GetUserRankInfo();
		// 檢查是否有權限
		//if (!ri|| !ri->Setting.AdjustGroup)
		//	return ;
		//m_tempGuildInfo.RankCount=10;
		m_tempGuildInfo.IsNeedSave=true;
		
		int guildID = CNetGlobal::RoleData()->BaseData.GuildID;
		_Guild_data *date=g_internal->GetGuildData(guildID);
		if (date)
			date->m_info=m_tempGuildInfo;
		//NetCli_GuildChild::s_GuildInfo=m_tempGuildInfo;

		int time=timeGetTime();
		if ((time-m_ModifyTime)>=DF_GETGUILDINFO_UPDATE)
		{
			m_ModifyTime=time;
	
			NetCli_Guild::SL_ModifyGuildInfoRequest(m_tempGuildInfo);
		}
	}
}
//---------------------------------------------------------------------------------
//通知會長換人
void CGuildFrame::LeaderChangeMsg(int newid,int oldid)
{

	GuildMemberBaseInfoStruct* newLedaer= g_internal->GetMemberBaseInfo(newid);
	GuildMemberBaseInfoStruct* oldLedaer= g_internal->GetMemberBaseInfo(oldid);

	char temp[512];
	const char *oldname=NULL;
	if (oldLedaer)
		oldname= oldLedaer->MemberName.Begin();
	const char *newname=NULL;
	if (newLedaer)
		newname=newLedaer->MemberName.Begin();

	const char *Msg=g_ObjectData->GetString("GUILD_MSG_LEDADER_CHANGE");
	if (oldname&&newname)
		sprintf_s( temp ,512, Msg ,oldname, newname);
	else
	{
		Msg=g_ObjectData->GetString("GUILD_STR_LEADERCHANGE");
		
		sprintf_s( temp ,512,Msg,newname);
	}

	
	
	g_pChatFrame->SendMsgEvent(CHAT_MSG_GUILD, temp,"" );

	NetCli_Guild::SL_GuildMemberInfoRequet(newid);
	NetCli_Guild::SL_GuildMemberInfoRequet(oldid);
}
//---------------------------------------------------------------------------------
bool CGuildFrame::isHaveGuild(void)
{

	return m_haveGuild? true:false;
}
void  CGuildFrame::ClearGuildInfo()
{


memset(&m_tempGuildInfo,0,sizeof(m_tempGuildInfo));
}
//---------------------------------------------------------------------------------
void	CGuildFrame::SelfLeaveGuid(void)
{
	//char tmp[512];
	YouGetFire();
	ClearGuildInfo();
	UpdateGuild();

}
// ----------------------------------------------------------------------------------
//連署邀請
void CGuildFrame::PetitionSignature(int guildID, const char* leaderName, int leaderDBID)
{
	if ( m_guildID != 0 )
	{		
		if ( m_guildID != guildID )
			NetCli_Guild::SL_NewGuildInviteResult(guildID, leaderDBID, false);		// 已被邀請中,直接拒絕邀請

		// 重複邀請不處理
		return;
	}

	m_guildID		= guildID;
	m_leaderDBID	= leaderDBID;
	m_leaderName	= leaderName;

	// 跳出訊息視窗
	lua_State* L = GetLuaState();
	if ( L )
	{
		char tmp[256];

		//sprintf(tmp, "%d", m_guildID);  
		//fuming 2007/9/13
		
		sprintf_s(tmp,256, "%s",NetCli_GuildChild::GetGuildNameByID(guildID) );//NetCli_GuildChild::s_GuildInfo.GuildName.Begin());

		lua_pushstring(L, m_leaderName.c_str());
		lua_setglobal(L, "arg1");
		lua_pushstring(L, tmp);
		lua_setglobal(L, "arg2");
		SendWorldEvent("GUILD_PETITION_SIGNATURE");
	}
}
//公會成員上下線
void  CGuildFrame::GuildPlayerOnline		( int PlayerDBID , bool IsOnline )
{
	g_internal->GuildPlayerOnline(PlayerDBID,IsOnline);
}

void CGuildFrame::MemberLogin(const char * name,time_t time,bool IsOnline)
{

	char temp1[512];
	struct tm *gmt;
	gmt = localtime( &time );

	sprintf_s( temp1 ,512, "%02d/%02d/%02d %02d:%02d" ,  1 + gmt->tm_mon , gmt->tm_mday,  gmt->tm_year -100, gmt->tm_hour , gmt->tm_min);

	

	//公會頻道提示MSG
	char temp[512];
	const char *pMsg;
	if(IsOnline)
		pMsg=g_ObjectData->GetString("GUILD_MSG_MEMBERLOGIN_GUILD");
	else
		pMsg=g_ObjectData->GetString("GUILD_MSG_MEMBERLOGOUT_GUILD");

	

	sprintf_s(temp,512,pMsg,name,temp1);

	g_pChatFrame->SendMsgEvent(CHAT_MSG_GUILD, temp,"" );

	//更新列表狀態
	UpdateGuild();
}



// ----------------------------------------------------------------------------------
//加入公會邀請
void CGuildFrame::GuildInviteRequest(int guildID, const char* leaderName, int leaderDBID)
{
	if ( m_guildID != 0 )
	{		
		if ( m_guildID != guildID )
			NetCli_Guild::SL_GuildInviteResult(guildID, leaderDBID, false);		// 已被邀請中,直接拒絕邀請

		// 重複邀請不處理
		return;
	}

	m_guildID		= guildID;
	m_leaderDBID	= leaderDBID;
	m_leaderName	= leaderName;

	// 跳出訊息視窗
	lua_State* L = GetLuaState();
	if ( L )
	{
		char tmp[MAX_PATH];

		//sprintf(tmp, "%d", m_guildID);  
		//fuming 2007/9/13

		sprintf_s(tmp,MAX_PATH, "%s",NetCli_GuildChild::GetGuildNameByID(guildID) );//NetCli_GuildChild::s_GuildInfo.GuildName.Begin());

		lua_pushstring(L, m_leaderName.c_str());
		lua_setglobal(L, "arg1");
		lua_pushstring(L, tmp);
		lua_setglobal(L, "arg2");
		SendWorldEvent("GUILD_INVITE_REQUEST");
	}
}
// ----------------------------------------------------------------------------------
//回應連署
void CGuildFrame::PetitionReply(bool agree)
{
	if ( m_guildID == 0 )								// 沒有被邀請過,回應無效
		return;

	// 回應邀請連署
	NetCli_Guild::SL_NewGuildInviteResult(m_guildID, m_leaderDBID, agree);

	m_guildID		= 0;
	m_leaderDBID	= 0;
	m_leaderName.clear();
}

//回應正式
void CGuildFrame::GuildInviteReply(bool agree)
{
	if ( m_guildID == 0 )								// 沒有被邀請過,回應無效
		return;

	// 回應正式邀請
	NetCli_Guild::SL_GuildInviteResult(m_guildID, m_leaderDBID, agree);

	if (!agree)
	{
		m_guildID		=0;
		m_leaderDBID	=0;//m_leaderDBID;
		m_leaderName.clear();

	}

	
}
//---------------------------------------------------------------------
void CGuildFrame::GuildMemberRankChange		( int MemberDBID , int Rank )
{

	GuildMemberBaseInfoStruct* newLedaer= g_internal->GetMemberBaseInfo(MemberDBID);
	if(newLedaer)
	{

		newLedaer->Rank=(GuildRankENUM)Rank;

		char temp[MAX_PATH];
		const char * pStr=g_ObjectData->GetString("GUILD_MSG_RANK_CHANGE");
		const char *pRankStr=m_tempGuildInfo.Rank[Rank].RankName.Begin();

		sprintf_s(temp,MAX_PATH,pStr,newLedaer->MemberName.Begin(),CNetCli_Script_Child::ReplaceSystemKeyword(pRankStr).c_str());
		g_pChatFrame->SendMsgEvent(CHAT_MSG_GUILD, temp,"" );
		UpdateGuild();
	}
}

//-------------------------------------------------
int LuaFunc_GuildInviteResult(lua_State* L)
{
	bool enable = lua_toboolean(L, 1) != 0;

    g_pGuildFrame->GuildInviteReply(enable);

    return 0;
}
void CGuildFrame::ChangeGuildNameResult(int Result)
{
	g_internal->ChangeGuildNameResult(Result);
}
// ----------------------------------------------------------------------------------
//毀滅連署
void CGuildFrame::PetitionQuit()
{
	// 檢查是否為成立中公會
	if ( CNetGlobal::RoleData()->BaseData.GuildID == 0 )
		return;

	m_guildID=0;

	NetCli_Guild::SL_DelNewGuild();
}
//----------------------------------
//通知UI 刷新公會資訊
void CGuildFrame::UpDateGuildInfo(GuildBaseStruct &source)
{

	m_GuildDateLive=timeGetTime();
	m_haveGuild=true;

	if (strcmp(m_tempGuildInfo.Note.SafeStr(),source.Note.SafeStr()))
	{
		m_bshowMessage=true;
		
		//

	}


	m_tempGuildInfo=source;	
//	m_guildID=m_tempGuildInfo.GuildID;


	
	
	SendWorldEvent("UPDATE_GUILD_INFO");
	

}

//----------------------------------
//會長交替
void CGuildFrame::LeaderChange(void)
{
	GuildMemberBaseInfoStruct* selfInfo = g_internal->GetSelfBaseInfo();
	GuildMemberBaseInfoStruct* selectInfo = g_internal->GetSelectBaseInfoStruct();
	if ( selfInfo&&selectInfo )
	{
		if (selfInfo->Rank==EM_GuildRank_Leader)
		{
			m_tempGuildInfo.LeaderName=selectInfo->MemberName;
			m_tempGuildInfo.LeaderDBID=selectInfo->MemberDBID;
			m_ModifyTime=-1;
			GuildInfoApply();

			/*
			GuildMemberStruct* temp=GetGuildMemberStruct(selfInfo->MemberDBID);		
			_Guild_data* date=g_internal->GetSelfGuildData();
			int maxRank=3;
			if (date)
				maxRank=date->m_info.RankCount;// NetCli_GuildChild::s_GuildInfo.RankCount;
			if (maxRank<1&&maxRank>10)
				maxRank=1;
			temp->Rank=(GuildRankENUM)(maxRank-2);			
			NetCli_Guild::SL_ModifyGuildMemberInfoRequest(*temp);
			*/
/*
			selfInfo->Rank=temp->Rank;
			temp=g_pGuildFrame->GetGuildMemberStruct(selectInfo->MemberDBID);

			temp->Rank=EM_GuildRank_Leader;		
			selectInfo->Rank=temp->Rank;
			NetCli_Guild::SL_ModifyGuildMemberInfoRequest(*temp);

*/			

			UpdateGuild();
		}
	}

}
// 其他人收到某公會更名
void CGuildFrame::ChangeGuildName( int GuildID , char* GuildName  )
{
	g_internal->ChangeGuildName(GuildID,GuildName);

}


void CGuildFrame::GuildBaseInfo( int GuildID , char* GuildName , GuildFlagStruct& Flag , bool IsReady )
{
	g_internal->GuildBaseInfo(GuildID,GuildName,Flag,IsReady);

}

// 某公會倒了　維護列表
void CGuildFrame::GuildDestroy			( int GuildID  )
{
	g_internal->GuildDestroy(GuildID);

}

//-------------------------------------
//開始 成員資料需求
void CGuildFrame::BeginMemberInfoRequest(void)
{
	m_waitEvent.m_member.clear();
	m_waitEvent.m_eventFunc=0;
	

}
//-------------------------------------
//新增 成員資料需求
void CGuildFrame::AddMemberInfoRequest(int dbid)
{
	GuildMemberStruct* temp= GetGuildMemberStruct(dbid);		
	if (temp==0)
		m_waitEvent.m_member.push_back(dbid);

}
//-------------------------------------
//接收到新成員資料
//檢查列表滿足就 執行
void CGuildFrame::AddMemberInfo(int dbid)
{
	
	vector<int>::iterator it=m_waitEvent.m_member.begin();
	for(;it!=m_waitEvent.m_member.end();++it)
	{
		if(*it==dbid)
		{
			m_waitEvent.m_member.erase(it);
			if (m_waitEvent.m_member.size()==0)
				(this->*m_waitEvent.m_eventFunc)();
			break;
		}


	}
}
//-------------------------------------
//必備公會詳細資料請求結束
void CGuildFrame::EndMemberInfo(Event func)
{
	
	m_waitEvent.m_eventFunc=func;

	int count=(int)m_waitEvent.m_member.size();
	for(int i=0;i<count;i++)
	{
		NetCli_Guild::SL_GuildMemberInfoRequet(m_waitEvent.m_member[i]);

	}
	if (m_waitEvent.m_member.size()==0)
		(this->*func)();


}
//-------------------------------------
//會長交替請求
void CGuildFrame::LeaderChangeRequest(int dbID)
{	
	GuildMemberBaseInfoStruct* selfInfo = g_internal->GetSelfBaseInfo();


	GuildMemberBaseInfoStruct* selectInfo = g_internal-> GetSelectBaseInfoStruct();	
	if ( selectInfo )
	{
		if (dbID!=-1)
			selectInfo = g_internal-> GetMemberBaseInfo(dbID);	
		if ( selfInfo&&selectInfo )
		{
			if (selfInfo->Rank==EM_GuildRank_Leader)
			{
				//PG_CtoC_LeaderChange Send;
				//Send.SenderName=selfInfo->MemberName;

				NetCli_ChannelChild::SendToCliAskLeaderChange(selectInfo ->MemberName.Begin(),selfInfo->MemberName.Begin());
				/*
				BeginMemberInfoRequest();
				AddMemberInfoRequest(selfInfo->MemberDBID);
				AddMemberInfoRequest(selectInfo->MemberDBID);
				EndMemberInfo(&CGuildFrame::LeaderChange);			
				*/
			}
		}
	}
}
// ----------------------------------------------------------------------------------
//設定階級名稱
void CGuildFrame::SetRankStr(int index,const char *name)
{

	GuildRankBaseInfoStruct* ri=GetUserRankInfo();
	// 檢查是否有權限
	if (!ri->Setting.AdjustRank)
		return ;

	char tempBuff[512];
	strncpy(tempBuff,name,31);

	tempBuff[31]=0;		
	
	size_t destLen=31;

	m_tempGuildInfo.Rank[index].RankName=ItemUTIL::checkUtf8SafeString(tempBuff,destLen,NULL);



}
// ----------------------------------------------------------------------------------
//連署消去
void CGuildFrame::PetitionKick(const char* name)
{
	// 檢查是否為成立中公會
	if ( CNetGlobal::RoleData()->BaseData.GuildID == 0 )
		return;

	NetCli_Guild::SL_LeaveNewGuild((char*)name);		// 踢掉公會
}
//--------------------------公會正式成立 
//跑馬燈
void CGuildFrame::GuildCreate(int GuildID)
{
	const char *pGuildName=NetCli_GuildChild::GetGuildNameByID(GuildID);
	if(GuildID==CNetGlobal::RoleData()->BaseData.GuildID)
	{
		g_internal->ReloadGuildDate();

	}
	char temp[1024];
	const char *pStr=g_ObjectData->GetString("GUILD_MSG_GUILDREADY");

	if (pStr&&pGuildName)
	{
		sprintf_s(temp,1024,pStr,pGuildName);

		g_pGameMain->SendScrollBannerMessage( temp );

	}


}

// ----------------------------------------------------------------------------------
//邀請
void CGuildFrame::Invite(const char* name)
{

	//戰場中
	if( g_pGameMain->IsBattleGroundZone()|| (g_pGuildHousesFrame->isGuildWaring()))
	{

		const char *pMsg;

		pMsg=g_ObjectData->GetString("GUILD_MSG_CANT_INVITE_GUILDWAR");
		g_pGameMain->SendWarningMsg(pMsg);


		return ;

	}


	_Guild_data*date=g_internal->GetSelfGuildData();
	//GuildBaseStruct& guildInfo = date->m_info;
	if (!date || date->m_info.GuildID == 0 )
	{
		// 沒有公會
		return;
	}
	GuildBaseStruct& guildInfo = date->m_info;
	if ( name == NULL || strlen(name) < 1 )
	{
		// 需要有邀請對象
	}
	CRoleSprite* player = g_pGameMain->GetPlayer();
	const char *pPlayerName=player->GetName();
	char temp1[512];
	char temp2[512];
	strcpy(temp1,pPlayerName);
	strlwr(temp1);
	strcpy(temp2,name);
	strlwr(temp2);
	if (! strcmp(temp1,temp2) )
	{
		// 耍 蠢 邀請自己
		SendWorldEvent("GUILDINVITE_SELF");

		return;
	}
	
	// 正式公會
	if ( guildInfo.IsReady )
	{
		// 檢查是否可以邀請, 某些特殊狀態下不能發出邀請
		{
			int count = (int)g_pGuildHousesFrame->m_vecGuildHouseWarInfo.size();
			int GuildID = CNetGlobal::RoleData()->BaseData.GuildID | (g_pAccountLogin->GetWorldID()*0x1000000);

			for ( int i = 0; i < count; i++ )
			{
				GuildHouseWarInfoStruct* pGuildInfo = &g_pGuildHousesFrame->m_vecGuildHouseWarInfo[i];

				if ( GuildID == pGuildInfo->GuildID || GuildID == pGuildInfo->EnemyGuildID )
				{
					switch (pGuildInfo->State)
					{
					case EM_GuildHouseWarState_Appointment:
					case EM_GuildHouseWarState_Prepare:
					case EM_GuildHouseWarState_Fight:
						return;
					}
				}
			}
		}

		// 檢查是否有權限
		GuildMemberBaseInfoStruct* baseInfo = g_internal->GetSelfBaseInfo();
		if ( baseInfo )
		{
			GuildRankBaseInfoStruct& rankInfo = guildInfo.Rank[baseInfo->Rank];
			if ( rankInfo.Setting.Invite )
			{
				char temp[512];

				sprintf_s(temp,512,g_ObjectData->GetString("GUILD_MSG_INVITE_WHOTO_GUILD"),name);
				g_pChatFrame->SendMsgEvent("CHAT_MSG_SYSTEM", temp,"" );
				NetCli_Guild::SL_GuildInvite((char*)name);

			//	sprintf(temp,g_ObjectData->GetString("GUILD_MSG_WAIT_WHOTO_GUILD"),name);
			//	g_pChatFrame->SendMsgEvent("CHAT_MSG_SYSTEM", temp,"" );
			}
			else
			{
				// 沒有權限
			}
		}
		else
		{
			// 不在公會中,可能資料有問題
		}
	}
	// 連署中公會
	else
	{
		// 檢查是否有權限
		if ( guildInfo.LeaderDBID == g_pGameMain->GetDBID() )
		{
			char temp[512];

			sprintf_s(temp,512,g_ObjectData->GetString("GUILD_MSG_INVITE_WHOTO_NEWGUILD"),name);

			g_pChatFrame->SendMsgEvent("CHAT_MSG_SYSTEM", temp,"" );
			NetCli_Guild::SL_NewGuildInvite((char*)name);
		//	sprintf(temp,g_ObjectData->GetString("GUILD_MSG_WAIT_WHOTO_GUILD"),name);
		//	g_pChatFrame->SendMsgEvent("CHAT_MSG_SYSTEM", temp,"" );
		}
		else
		{
			// 沒有權限
		}
	}
}
//-----------------------------
//取得公會成員的詳細資料
GuildMemberStruct*        CGuildFrame::GetGuildMemberStruct(int dbid)
{
	CachGuildMember *p=g_internal->GetMemberInfo(dbid);

	GuildMemberStruct*ret=NULL;
	if (p)
		ret= &p->m_info;

	return ret;
}

// ----------------------------------------------------------------------------------
//踢人
void CGuildFrame::Uninvite(const char* name)
{
	_Guild_data*date=g_internal->GetSelfGuildData();
	if (!date)
		return ;

	GuildBaseStruct& guildInfo =date->m_info;
	
	if ( guildInfo.GuildID == 0 )
	{
		// 沒有公會
		return;
	}
	if ( name == NULL || strlen(name) < 1 )
	{
		// 需要有移除玩家對象
		return;
	}

	// 正式公會
	if ( guildInfo.IsReady )
	{
		// 檢查是否有權限
		GuildMemberBaseInfoStruct* baseInfo = g_internal->GetSelfBaseInfo();
		if ( baseInfo )
		{
			GuildRankBaseInfoStruct& rankInfo = guildInfo.Rank[baseInfo->Rank];
			if ( rankInfo.Setting.Kick )
			{
				NetCli_Guild::SL_LeaveGuild((char*)name);
			}
			else
			{
				// 沒有權限
			}
		}
		else
		{
			// 不在公會中,可能資料有問題
		}
	}
	// 連署中公會
	else
	{
		// 檢查是否有權限
		if ( guildInfo.LeaderDBID == g_pGameMain->GetDBID() )
		{
			NetCli_Guild::SL_LeaveNewGuild((char*)name);
		}
		else
		{
			// 沒有權限
		}
	}
}

// ----------------------------------------------------------------------------------
//退出公會
void CGuildFrame::Leave()
{
	_Guild_data*date=g_internal->GetSelfGuildData();
	if (!date)
		return ;

	GuildBaseStruct& guildInfo =date->m_info;
	if ( guildInfo.GuildID == 0 )
	{
		// 沒有公會
		return;
	}

	// 正式公會
	if ( guildInfo.IsReady )
	{
		NetCli_Guild::SL_LeaveGuild((char*)CNetGlobal::RoleData()->BaseData.RoleName.Begin());
	}
	// 連署中公會
	else
	{
		NetCli_Guild::SL_LeaveNewGuild((char*)CNetGlobal::RoleData()->BaseData.RoleName.Begin());
	}
}

// ----------------------------------------------------------------------------------
//通知會員列表資料更新
void CGuildFrame::UpdateGuild()
{
	

	SendWorldEvent("UPDATE_GUILD_MEMBER");
}

// ----------------------------------------------------------------------------------
int LuaFunc_IsInGuild(lua_State* L)
{
	if ( NetCli_GuildChild::GetGuildNameByID(CNetGlobal::RoleData()->BaseData.GuildID) )
		lua_pushboolean(L, true);
	else
		lua_pushboolean(L, false);
	return 1;
}

// ----------------------------------------------------------------------------------
//公會建立
int LuaFunc_GuildCreate(lua_State* L)
{
	

	const char* guildName = luaL_checkstring(L, 1);
	g_pDebugMessage->DebugOutput( 2 , "要求建立公會  %s", guildName );
	bool ret=g_pGuildFrame->CreateGuild(guildName);


		lua_pushboolean(L, ret);
	return 1;
}

int CGuildFrame::GetContributionItem()
{
	ClipboardItemInfo itemInfo = g_pItemClipboard->GetItemInfo();
	ClipboardItemType itemType = g_pItemClipboard->GetType();

	ItemFieldStruct* item = NULL;
	lua_State* L = GetLuaState();


	if( itemType == eClipboardItem_Type_Bag )
	{
		int index=itemInfo.bag.pos;
		
		item = CNetGlobal::RoleData()->GetBodyItem(index);
		if (item)
		{
			GameObjDbStructEx* itemDB = CNetGlobal::GetObj(item->OrgObjID);			
			//itemDB->Item.ItemType == EM_ItemType_GuildContribution|| 
			if (itemDB&&itemDB->Item.GuildResourceType!=EM_GuildResourceType_None&&itemDB->Item.Class ==EM_Item_Normal)
			{
				g_pItemClipboard->Clear();
				item->Pos=EM_USE_GUILDCONTRIBUTION;
				g_pBagFrame->ClearPickupItemIndex();
				m_itemPos.push_back(index);
				lua_pushnumber(L, index+1);

				// push item texture file
				GameObjDbStruct* imageDB = CNetGlobal::GetObj(itemDB->ImageID);
				if ( imageDB )
					lua_pushstring(L, imageDB->Image.ACTField);
				else
					lua_pushstring(L, "");
				
				lua_pushstring(L, itemDB->GetName());						// push Item name
				lua_pushnumber(L, item->GetCount());							// push item count
				lua_pushnumber(L, itemDB->Item.GuildContributionValue);	// push item Points

				lua_pushnumber(L, itemDB->Item.GuildResourceType+1);	// push item Points
				lua_pushnumber(L, itemDB->Item.GuildResourceValue);	// push item Points


			
			

			//	lua_pushboolean(L, item->Pos != EM_ItemState_NONE);		// push item is locked
				return 7;
			}
			else
			{
				
				const char* tempStr;

				tempStr=g_ObjectData->GetString("MSG_GUILDCONTRIBUTION_WRONGTHING");
				
				g_pGameMain->SendWarningMsg(tempStr);

				//g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, tempStr, "" );
				
			}
		}
	}


	return 0;
}
//重置時間
//重置公會更新的COOLDOWN
//void GuildBaseInfoUpdate(void);
void CGuildFrame::GuildBaseInfoUpdate(void)
{

	int time=timeGetTime()-(DF_GUILDINFO_UPDATETIMER+5000);

	{
		{
			const char* temp=g_ObjectData->GetString("GUILD_BASEINFO_UPDATE");
			g_pChatFrame->SendMsgEvent("CHAT_MSG_SYSTEM", temp,"" );
		}

	}
NetCli_GuildChild::SL_GuildInfoRequet(CNetGlobal::RoleData()->BaseData.GuildID);

	m_GuildDateLive=time;

}
void CGuildFrame::BuildFrames(void)
{
m_guildID=0;
g_internal->Clear();




}
// 玩家離開自己的公會
void CGuildFrame::LeaveGuildMember		( int LeaderDBID , int MemberDBID )
{
	g_internal->LeaveGuildMember(LeaderDBID,MemberDBID);

}

//成員的詳細資料
void CGuildFrame::GuildMemberInfo( GuildMemberStruct& Member )
{
	g_internal->GuildMemberInfo(Member);
}
//加入新成員
void CGuildFrame::JoinGuildMember( GuildMemberStruct& Member )
{
	g_internal->JoinGuildMember(Member);
}
// ----------------------------------------------------------------------------------
// 正式公會建立結果
void CGuildFrame::GuildReadyResult(int Result)
{

	char temp[MAX_PATH];
	const char *strResult=0;

	_Guild_data*date=g_internal->GetSelfGuildData();
	if (!date)
		return ;

	GuildBaseStruct& guildInfo =date->m_info;


	const char *pGuildName=guildInfo.GuildName.Begin();
	const char *tempStr=g_ObjectData->GetString("GUILD_MSG_GUILDREADY");
	
	if (Result)	
	{
		strResult=g_ObjectData->GetString("SUCCEEDED");	
	}
	else
	{
		strResult=g_ObjectData->GetString("FAIL");	
		{
			char temp2[MAX_PATH];
			const char *DeBugtempStr=g_ObjectData->GetString("GUILD_MSG_READY_FAIL");
			const char * name=CNetGlobal::RoleData()->RoleName();
			//const char *Guildname=NetCli_GuildChild::GetGuildNameByID(m_guildID);
			sprintf_s(temp2,MAX_PATH,DeBugtempStr,name,pGuildName);
			g_pDebugMessage->DebugOutput( 2 , temp2  );
		}

	}

	sprintf_s(temp,MAX_PATH,tempStr,pGuildName,strResult);
	g_pGameMain->SendWarningMsg(temp);


}
bool CGuildFrame::IsLeader(void)
{

	bool bIsLeader=false;
	_Guild_data*date=g_internal->GetSelfGuildData();
	if (date)
	{
		GuildBaseStruct& guildInfo =date->m_info;

		bIsLeader=g_pGameMain->GetDBID() == guildInfo.LeaderDBID;

	}

	GuildMemberBaseInfoStruct* selfB = g_internal->GetSelfBaseInfo();
	bool rank9=false;
	if (selfB&&selfB->Rank==EM_GuildRank_Leader) 
		rank9=true;
	
	return (bIsLeader|rank9);

}

// ----------------------------------------------------------------------------------
//取得公會資訊
int LuaFunc_GetGuildInfo(lua_State* L)
{
	int nowTime= timeGetTime(); 
	
	if (g_pGuildFrame)
	g_pGuildFrame->GetGuildInfo();

	_Guild_data*date=g_internal->GetSelfGuildData();
	if (!date)
		return 0;
	{/*
		CLuaTable table(L);
		CLuaTable table1(L,"sub");
		table.SetTable("sub",table1);
		CLuaTable SubT=table.SubTable(333);
			CLuaTable Sub2=table.SubTable("sub");
		table.SetString(1,"test22");
		table.SetInt("test",2213);
		std::map<int, std::string> data;
		table.GetMap(data);
		std::map<std::string, float> data2;
		table.GetMap(data2);
		int a=table.GetLength();
		table.SetUserData("this",g_pGuildFrame);
		int test=table.GetInt("test",-1);
		CGuildFrame *t=(CGuildFrame *)table.GetUserData("this",NULL);
		std::string str=table.GetString(1,"nome");
		SubT.SetInt("hp",1);
		CLuaTable SubT2=table.SubTable(333);
		int hp=SubT2.GetInt("hp",100);
	*/}
	GuildBaseStruct& guildInfo =date->m_info;

	int guildID = CNetGlobal::RoleData()->BaseData.GuildID;
	if ( guildID != 0 )
	{
		char temp[1024]={0};
		const char *pSafeString;

		// 公會名稱
		lua_pushstring(L, guildInfo.GuildName.Begin());
		//會長大名
		lua_pushstring(L, guildInfo.LeaderName.Begin());

		//是否招生
		lua_pushboolean(L, guildInfo.IsRecruit);

		// 公會是否已成立
		lua_pushboolean(L, guildInfo.IsReady);
		
		//最多可以有多少會員
		lua_pushnumber(L, guildInfo.MaxMemberCount);
		//公會積分
		lua_pushnumber(L, 0/*guildInfo.Score*/);

		size_t len=1024;

		memset(temp,0,1024);
		//公告
		strcpy_s(temp,len,guildInfo.Note.SafeStr());
		pSafeString=ItemUTIL::checkUtf8SafeString(temp,len,NULL);		
		//pSafeString[len]=0;
		lua_pushstring(L,pSafeString );

		memset(temp,0,1024);
		len=1024;
		//公會介紹
		strcpy_s(temp,len,guildInfo.Introduce.SafeStr());
		pSafeString=ItemUTIL::checkUtf8SafeString(temp,len,NULL);		
		//pSafeString[len]=0;
		lua_pushstring(L, pSafeString);


		GuildMemberBaseInfoStruct* selfB = g_internal->GetSelfBaseInfo();
		bool rank9=false;
		if (selfB&&selfB->Rank==EM_GuildRank_Leader) 
			rank9=true;
		bool IsLeader=g_pGameMain->GetDBID() == guildInfo.LeaderDBID;

	
		// 是否為公會會長
		lua_pushboolean(L,IsLeader| rank9);

		
		
		
		// 公會是否有開放留言板
		lua_pushboolean(L, true/*guildInfo.IsEnabledGuildBoard*/);//10
		

	

		bool OpenGuildFrame=true;
		if (IsLeader)
		{
			
			if ( guildInfo.IsNeedChangeName)
			{
				g_internal->OpenRenameDialog();
				OpenGuildFrame=false;
					
			}

		}

		lua_pushboolean(L,OpenGuildFrame);//11


		// 公會RANK
		lua_pushnumber(L, guildInfo.Level);


		//公會屋
		lua_pushboolean( L, guildInfo.IsOwnHouse );

		//開放參觀
  		lua_pushboolean( L, !guildInfo.IsLockGuildHouse );

		return 14;
	}
	return 0;
}



//-------------------------------------------------------------------------------
//開啟公會貢獻介面
void CGuildFrame::OpenGuildContribution(int iObjID)
{

#ifdef _DEBUG
		iObjID=-1;
#endif
		m_npcId=iObjID;


		CRoleSprite * npcSprite= g_pGameMain->FindSpriteByID( iObjID );
		//	GameObjDbStruct* npcDB = CNetGlobal::GetObj( iObjID );

		if (npcSprite==NULL) 
			g_pDebugMessage->DebugOutput( 2 , "物件資料庫無此NPC %d", iObjID );
		if ( npcSprite  || iObjID==-1)
		{
			// 要求重新取得自己公會資訊
			g_internal->ReloadGuildDate();

			// 開啟公會貢獻介面
			SendWorldEvent("GUILD_CONTRIBUTION_OPEN");
			g_pDebugMessage->DebugOutput( 2 , "開啟公會貢獻介面 NPC編號 %d", iObjID );
			m_Open=true;
		}


	}
/*
// ----------------------------------------------------------------------------------
int LuaFunc_GuildPetitionSignature(lua_State* L)
{
	const char* name = luaL_checkstring(L, 1);
	NetCli_Guild::SL_NewGuildInvite((char*)name);				// 邀請加入公會連署
	return 0;
}
*/

// ----------------------------------------------------------------------------------
//加入連署
int LuaFunc_GuildPetitionAccept(lua_State* L)
{
	g_pGuildFrame->PetitionReply(true);
	return 0;
}

// ----------------------------------------------------------------------------------
//拒絕連署
int LuaFunc_GuildPetitionDecline(lua_State* L)
{
	g_pGuildFrame->PetitionReply(false);
	return 0;
}

// ----------------------------------------------------------------------------------
//退出連署
int LuaFunc_GuildPetitionQuit(lua_State* L)
{
	g_pGuildFrame->PetitionQuit();
	return 0;
}

// ----------------------------------------------------------------------------------
//連署踢人
int LuaFunc_GuildPetitionKick(lua_State* L)
{
	const char* name = luaL_checkstring(L, 1);
	g_pGuildFrame->PetitionKick(name);
	return 0;
}



// ----------------------------------------------------------------------------------
// 取得公會列表人數
int LuaFunc_GetNumGuildMembers(lua_State* L)
{
	lua_pushnumber(L, g_internal->GetNumGuildMembers());
	return 1;
}

// ----------------------------------------------------------------------------------
// 取得公會成員資料
int LuaFunc_GetGuildRosterInfo(lua_State* L)
{
	const char* name = NULL;
	const char* className = NULL;
	const char* subClassName = NULL;
	const char* guildTitle = g_ObjectData->GetString("GUILD_TEXT_TITLE_INVATE");
	//"稱號不存在";//NULL;GUILD_TEXT_TITLE_INVATE
	const char* ZoneName = g_ObjectData->GetString("GUILD_TEXT_ZONE_UNKOWN");//"unkown";//NULL;

	char Note [1024]={0};
	bool isHeader = false;
	bool isCollapsed = false;
	bool IsOnLine= false;
	int rank = 0;
	int level = 0;
	int subLevel = 0;
	int dbid = -1;
	char logoutTime[MAX_PATH]="??:??:??";
	char  tZone[512];

	strcpy(tZone,ZoneName);

	const char *pSafeString=NULL;

	

	if ( lua_isnumber(L, 1) )
	{
		int index=(int)lua_tonumber(L, 1) - 1;
		GuildMemberListInfo* info = g_internal->GetGuildMemberInfo(index);
		if ( info )		
		{
			_Guild_data*date=g_internal->GetSelfGuildData();
			if (!date)
				return 0;

			GuildBaseStruct& guildInfo =date->m_info;

			int maxRank=guildInfo.RankCount;
			if (maxRank<1&&maxRank>10)
				maxRank=1;
			isHeader = info->isHeader;
			if ( isHeader )
			{
				name = info->groupName;
				isCollapsed = !(*info->pIsExpand);
			}
			else
			{
				GuildMemberStruct* temp=g_pGuildFrame->GetGuildMemberStruct(info->member->MemberDBID);		

				name = info->member->MemberName.Begin();
				rank = info->member->Rank;
				if (rank!=9&& rank>=maxRank)
				{
					
					if (temp)
					{
						temp->Rank=(GuildRankENUM)(maxRank-2);
						NetCli_Guild::SL_ModifyGuildMemberInfoRequest(*temp);
					}
					
				

					rank=(GuildRankENUM)(maxRank-2);	
					info->member->Rank=(GuildRankENUM)rank;
				}	
			

				className = g_ObjectData->GetClassNameByID(info->member->Voc);			
				subClassName = g_ObjectData->GetClassNameByID(info->member->Voc_Sub);
				level = info->member->LV;
				subLevel = info->member->LV_Sub;
				dbid =info->member->MemberDBID;

				IsOnLine=info->member->IsOnline;
				if (temp&&temp->LogoutTime!=-1)
				{

					time_t logoutTimeX=temp->LogoutTime;
					time_t NowTime;
					time( &NowTime );
					float df=(float)difftime(NowTime,logoutTimeX);
					int  sec=(int)df;
					int mine=sec/60;
					int hour=mine/60;
					int day=hour/24;
					if (day==0)
						sprintf_s(logoutTime,512,"%02d:%02d:%02d",hour%24,mine%60,sec%60);
					else
					{
						const char *pStr=g_ObjectData->GetString("OFFLINETIME_STR");
						sprintf_s(logoutTime,512,pStr,day,hour%24);
					}
					size_t len=1024;
					strcpy_s(Note,len,temp->LeaderNote.SafeStr());
					pSafeString=ItemUTIL::checkUtf8SafeString(Note,len,NULL);		

					
				}



				if (info->member->GUID!=-1)
				{
			
				int DBZoneID = (info->member->GUID / 0x10000)%_DEF_ZONE_BASE_COUNT_;

			//GameObjDbStructEx*	ZoneDB		= CNetGlobal::GetObj(  +  Def_ObjectClass_Zone );


				//int DBZoneID = (info->member->GUID>>16) % _DEF_ZONE_BASE_COUNT_ ;
				

					
				const char *cZoneName=	Func_GetLocalMapName(DBZoneID);
				if (strlen(cZoneName)>0)
					strcpy(tZone,cZoneName);
					
				else
				{

					const WorldMapInfoStruct* pWorldMapInfo = g_ObjectData->GetWorldMapInfoByID( DBZoneID );

					if ( pWorldMapInfo )
					{
						sprintf_s(tZone,512,"ZONE_%s(%X)",pWorldMapInfo->MapName,DBZoneID);
					}					
					//strcpy(tZone,ZoneName);

				}
				}
				
			
			}
		}
	}

	lua_pushstring(L, name);
	lua_pushnumber(L, rank+1);
	lua_pushstring(L, className);
	lua_pushnumber(L, level);
	lua_pushstring(L, subClassName);
	lua_pushnumber(L, subLevel);
	lua_pushboolean(L, isHeader);
	lua_pushboolean(L, isCollapsed);
	lua_pushnumber(L, dbid);

	
	lua_pushstring(L, guildTitle);

	lua_pushboolean(L, IsOnLine);
	lua_pushstring(L, logoutTime);
	lua_pushstring(L, tZone);
	lua_pushstring(L, pSafeString);

	
	return 14;
}

// ----------------------------------------------------------------------------------
//邀請
int LuaFunc_GuildInvite(lua_State* L)
{	

	g_pGuildFrame->Invite(lua_tostring(L, 1));
	return 0;
}

// ----------------------------------------------------------------------------------
// 踢人
int LuaFunc_GuildUninvite(lua_State* L)
{
	g_pGuildFrame->Uninvite(lua_tostring(L, 1));
	return 0;
}

// ----------------------------------------------------------------------------------
// 退出公會
int LuaFunc_GuildLeave(lua_State* L)
{
	g_pGuildFrame->Leave();
	return 0;
}

// ----------------------------------------------------------------------------------
// 群組的節點展開
int LuaFunc_ExpandGuildGroup(lua_State* L)
{
	g_internal->SetGuildHeaderState(luaL_checkint(L, 1) - 1, true);
	return 0;
}

// ----------------------------------------------------------------------------------
// 群組的節點關閉
int LuaFunc_CollapseGuildGroup(lua_State* L)
{
	g_internal->SetGuildHeaderState(luaL_checkint(L, 1) - 1, false);
	return 0;
}

// ----------------------------------------------------------------------------------
//設定目前選擇會員 讀取 詳細資料
int LuaFunc_SetGuildRosterSelection(lua_State* L)
{
	g_internal->SetGuildRosterSelection(luaL_checkint(L, 1) - 1);
	return 0;
}

// ----------------------------------------------------------------------------------
//取得目前選擇會員 
int LuaFunc_GetGuildRosterSelection(lua_State* L)
{
	lua_pushnumber(L, g_internal->GetGuildRosterSelection() + 1);
	return 1;
}
//----------------------------------
//取得階級數量
int LuaFunc_GF_GetRankCount			(lua_State* L)
{
	_Guild_data*date=g_internal->GetSelfGuildData();
	if (!date)
		return 0;

	GuildBaseStruct& guildInfo =date->m_info;

	int count=guildInfo.RankCount-1;
	GuildMemberBaseInfoStruct *info= g_internal->GetSelfBaseInfo();

	if (info&& info->Rank<(count))
		count=info->Rank;
	lua_pushnumber(L,count);
	return 1;

}


//----------------------------------
//取得階級名稱
int LuaFunc_GF_GetRankStr			(lua_State* L)
{
	int index=luaL_checkint(L,1)-1;
	if (index>=0&&index<EM_GuildRank_Count)
	{
		_Guild_data*date=g_internal->GetSelfGuildData();
		if (!date)		
			return 0;
		
		GuildBaseStruct& guildInfo =date->m_info;
		char tempRankStr[1024];
		char RankStr[512];
		memset(RankStr,0,512);
		wchar_t u16buff[512];
		strcpy_s(RankStr,512,guildInfo.Rank[index].RankName.SafeStr());


		size_t destLen=512;

		const char*pSafeStr=ItemUTIL::checkUtf8SafeString(RankStr,destLen,u16buff);

		int len=(int)strlen(pSafeStr);
		if (len<=0)
		{
			const char *Msg=g_ObjectData->GetString("SYS_RANK");
			sprintf_s(tempRankStr,1024,"%s %d",Msg,10-index);
		}
		else
		{
			
			strcpy_s(tempRankStr,1024,CNetCli_Script_Child::ReplaceSystemKeyword(pSafeStr).c_str());		
		}


		lua_pushstring(L,tempRankStr);
		return 1;
	}

	return 0;
}

//----------------------------------
//設定階級名稱
int LuaFunc_GF_SetRankStr			(lua_State* L)
{
	int index=luaL_checkint(L,1)-1;
	const char* name=luaL_checkstring(L,2);
	if (index>=0&&index<EM_GuildRank_Count)
	{
		g_pGuildFrame->SetRankStr(index,name);

	}
	return 0;
}
//----------------------------------
//套用公會資訊
int LuaFunc_GF_GuildInfoApply		(lua_State* L)
{

	g_pGuildFrame->GuildInfoApply();

	//if (g_pGuildFrame)
	//	g_pGuildFrame->GetGuildInfo();
	return 0;
}
//----------------------------------
//設定群組ENABLE
int LuaFunc_GF_SetGroupEnable	(lua_State* L)
{
	bool enable=lua_toboolean(L,1) != 0;

	g_internal->SetUserGroup(enable);
	//NetCli_GuildChild::s_UserGroup=enable;

	g_internal->UpdateMemberInfo();
	//if (g_pGuildFrame)
	//	g_pGuildFrame->UpdateGuild();

	return 0;
}
//----------------------------------
//取得公會選單選項
int LuaFunc_GF_GetMenuFlag(lua_State* L)
{
	
	GuildRankBaseInfoStruct* self=0;
	GuildRankBaseInfoStruct* select=0;
	_Guild_data*date=g_internal->GetSelfGuildData();
	if (!date)
		return 0;

	GuildBaseStruct& guildInfo =date->m_info;

	GuildMemberBaseInfoStruct* selfInfo = g_internal->GetSelfBaseInfo();
	if ( selfInfo )
	{
		self= &guildInfo.Rank[selfInfo->Rank];

	}
	GuildMemberBaseInfoStruct* selectInfo = g_internal->GetSelectBaseInfoStruct();
	if ( selectInfo )
	{
		select= &guildInfo.Rank[selectInfo->Rank];

	}

	int menu=0;
	if (self&&select)
	{
		//調整階級
		if (self->Setting.AdjustRank)
		{ //只能調整比自己低一階 不能調整自己
			if (selfInfo->Rank>selectInfo->Rank&&selfInfo!=selectInfo)
				menu|=1;
		}
		//踢除會員		
		if (self->Setting.Kick)
		{
			//只能踢人比自己低一階 不能踢自己
			if (selfInfo->Rank>selectInfo->Rank&&selfInfo!=selectInfo)
				menu|=2;
		}
		//調整群組
		if (self->Setting.AdjustGroup)
		{
			//只能調整比自己低一階
			if (selfInfo->Rank>selectInfo->Rank)
				menu|=4;
		}

		//設定成員幹部註解
		if (self->Setting.EditMemberNote)
		{
			//只能註解比自己&自己低一階 
			if (selfInfo->Rank>=selectInfo->Rank)
				menu|=8;
		}		
	}

	lua_pushboolean(L,selfInfo!=selectInfo);	
	lua_pushboolean(L,selectInfo->Rank!=9 &&selectInfo->IsOnline);	
	lua_pushboolean(L,(menu&1)?true:false);	
	lua_pushboolean(L,(menu&2)?true:false);	
	lua_pushboolean(L,(menu&4)?true:false);	
	lua_pushboolean(L,(menu&8)?true:false);	

	


	return 6;
}
//----------------------------------

int LuaFunc_GF_GetGuildFunc 		(lua_State* L)			    
{

	GuildRankBaseInfoStruct* self=0;
	
	_Guild_data*date=g_internal->GetSelfGuildData();
	if (!date)
		return 0;

	GuildBaseStruct& guildInfo =date->m_info;
	GuildMemberBaseInfoStruct* selfInfo = g_internal->GetSelfBaseInfo();
	if ( selfInfo )
	{
		self= &guildInfo.Rank[selfInfo->Rank];
		if (self)
		{

			bool vleader=(selfInfo->Rank==EM_GuildRank_Leader)?true:false;
			//可以踢人
			lua_pushboolean(L,self->Setting.Kick);	

			//可以收人		
			bool v=false;
		//	if (NetCli_GuildChild::s_GuildInfo.IsRecruit&&self->Setting.Invite||vleader)//公會招生
			if (self->Setting.Invite||vleader)
			{
			//	if( g_pGameMain->IsBattleGroundZone()==false && g_pGuildHousesFrame->isGuildWaring()==false)
					v=true;			
					
			}
			lua_pushboolean(L,v);	

			//設定公告
			lua_pushboolean(L,self->Setting.SetGuildNote);	
		
			//設定留言
			lua_pushboolean(L,self->Setting.PostMessage);	
			
			//留言板管理
			lua_pushboolean(L,self->Setting.ManageMessage);			

			//退位
			lua_pushboolean(L,vleader);	
			

			return 6;
		}

	}

	return 0;

}
//----------------------------------
//會長輪替
int LuaFunc_GF_LeaderChange 		(lua_State* L)			    //會長交換
{

	int DBID=-1;
	
	if( !lua_isnil( L, 1 ) && !lua_isnone( L, 1 ) )
	{
		DBID=luaL_checkint(L,1);
	}



	if (g_pGuildFrame)
		g_pGuildFrame->LeaderChangeRequest(DBID);
	

	return 0;

}
//----------------------------------
//取得權限
int LuaFunc_GF_GetRankInfo(lua_State* L)
{
  
	int index=luaL_checkint(L,1)-1;
	if (index<0&&index>9)
		return 0;

	_Guild_data*date=g_internal->GetSelfGuildData();
	if (!date)
		return 0;

	GuildBaseStruct& guildInfo =date->m_info;
	GuildRankBaseInfoStruct* self= &guildInfo.Rank[index];

	lua_pushboolean(L,self->Setting.Invite);
	lua_pushboolean(L,self->Setting.Kick);
	lua_pushboolean(L,self->Setting.AdjustRank);
	lua_pushboolean(L,self->Setting.EditMemberNote);
	lua_pushboolean(L,self->Setting.SetGuildNote);
	lua_pushboolean(L,self->Setting.PostMessage);
	lua_pushboolean(L,self->Setting.ManageMessage);
	lua_pushboolean(L,self->Setting.Building);
	lua_pushboolean(L,self->Setting.Furniture);
	lua_pushboolean(L,self->Setting.GuildWarRegister);
	lua_pushboolean(L,self->Setting.SetGuildHouseStyle);


	

	return 11;
}
//----------------------------------
//設定階級的資料
void CGuildFrame::SetRankInfo(int index,bool Invite,bool Kick,bool AdjustRank,bool EditMemberNote,bool SetGuildNote,bool PostMessage,bool ManageMessage,bool Building,bool Furniture,bool GuildWarRegister,bool SetGuildHouseStyle )
{
	GuildRankBaseInfoStruct* self= &m_tempGuildInfo.Rank[index];
	self->Setting.Invite=Invite;
	self->Setting.Kick=Kick;
	self->Setting.AdjustRank=AdjustRank;
	self->Setting.EditMemberNote=EditMemberNote;
	self->Setting.SetGuildNote=SetGuildNote;
	self->Setting.PostMessage=PostMessage;
	self->Setting.ManageMessage=ManageMessage;
	self->Setting.Building=Building;
	self->Setting.Furniture=Furniture;
	self->Setting.GuildWarRegister=GuildWarRegister;
	self->Setting.SetGuildHouseStyle=SetGuildHouseStyle;

	
}
//公會宣戰的權限
bool CGuildFrame::CanGuildWaring(void)
{
	bool ret=false;
	GuildRankBaseInfoStruct* rank= GetUserRankInfo();
	if (rank)
	{

		ret=rank->Setting.GuildWarRegister;

	}

	return ret;
}
//公會建設的權限

bool CGuildFrame::CanGuildBuilding(void)
{

	bool ret=false;
	GuildRankBaseInfoStruct* rank= GetUserRankInfo();
	if (rank)
	{

		ret=rank->Setting.Building;

	}

	return ret;
}



//----------------------------------
GuildFlagStruct* CGuildFrame::GetGuildFlagStruct( int GuildID )
{
	_Guild_data* info = g_internal->GetGuildData(GuildID);
	if( NULL == info || info->m_lev == 0 )
	{
		return NULL;
	}

	return &info->m_info.GuildFlag;
}

//----------------------------------
//設定階級的資料
int LuaFunc_GF_SetRankInfo			(lua_State* L)
{
	int index=luaL_checkint(L,1)-1;
	if (index<0&&index>9)
		return 0;

	g_pGuildFrame->SetRankInfo(index,
		lua_toboolean(L,2) != 0,
		lua_toboolean(L,3) != 0,
		lua_toboolean(L,4) != 0,
		lua_toboolean(L,5) != 0,
		lua_toboolean(L,6) != 0,
		lua_toboolean(L,7) != 0,
		lua_toboolean(L,8) != 0,
		lua_toboolean(L,9) != 0,
		lua_toboolean(L,10) != 0,
		lua_toboolean(L,11) != 0,
		lua_toboolean(L,12) != 0
		);
	return 0;
}

//-------------------------------------------------------
//設定會員幹部註解
bool  CGuildFrame::SetMemberCadreNote(int db,const char* name)
{
	bool ret=false;
	GuildMemberStruct* getMemData=GetGuildMemberStruct(db);

	if (getMemData)
	{
		if (CheckStr(name,64)==true)
		{
			 ret=true;

			 char tempBuff[512];
			 strncpy(tempBuff,name,255);

			 tempBuff[255]=0;		

			
			  size_t destLen=255;
		
			 getMemData->LeaderNote=ItemUTIL::checkUtf8SafeString(tempBuff,destLen,NULL);
			NetCli_Guild::SL_ModifyGuildMemberInfoRequest(*getMemData);
		}

	}
	return ret;

}


//-------------------------------------------------------
// 設定會員階級
void		CGuildFrame::SetMemberRankRequest(int index,int rank)
{

	GuildMemberStruct* getMemData=GetGuildMemberStruct(index);

	if (getMemData)
	{

		getMemData->Rank=(GuildRankENUM)rank;
		getMemData->RankTime=0;
		
		GuildMemberBaseInfoStruct* selectInfo = g_internal->GetSelectBaseInfoStruct();

		if (selectInfo)
		selectInfo->Rank=getMemData->Rank;
		NetCli_Guild::SL_ModifyGuildMemberInfoRequest(*getMemData);


		UpdateGuild();
	}
	

}
//-------------------------------------------------------
// 設定會員階級
int LuaFunc_GF_SetMemberRank(lua_State* L)
{

	int index=luaL_checkint(L,1);
    int rank=luaL_checkint(L,2)-1;
	if (rank<0&&rank>8)
		return 0;
	
	if (g_pGuildFrame)
		g_pGuildFrame->SetMemberRankRequest(index,rank);


	return 0;
}

void CGuildFrame::sortList()
{
	int i;
	//int j;


	BBSMessages::iterator it=m_BBSMessages.begin();
	BBSMessages::iterator next=it;
	while(next!=m_BBSMessages.end())
	{
		it=next;
		next++;
		if (it->m_post.Type.Del)
		{
			next=m_BBSMessages.erase(it);
			//break;
		}
		
	}			

	m_PostList.resize(m_BBSMessages.size());
	for (i=0;i<(int)m_PostList.size();i++)
	{		
		m_PostList[i]=i;		
	}
	//m_PostList.resize(i);
		
	std::sort( m_PostList.begin() , m_PostList.end() , sortPostList );
	lua_State* L = GetLuaState();
	if (L)
	{
		lua_pushnumber(L, (lua_Number)m_BBSMessages.size());
		lua_setglobal(L, "arg1");
		SendWorldEvent("GUILD_BBS_PAGEUPDATE");
	}

}



//-------------------------------------------------------
// 設定會員幹部註解

int LuaFunc_GF_SetMemberCadreNote(lua_State* L)
{

	int index=luaL_checkint(L,1);
	const char* Note=luaL_checkstring(L,2);
	bool ret=false;

	

	if (g_pGuildFrame)
		 ret=g_pGuildFrame->SetMemberCadreNote(index,Note);

		lua_pushboolean(L,ret);
	return 1;
}

//-------------------------------------------------------
// 取得會員幹部註解

const char*		CGuildFrame::	GetMemberCadreNote(void)
{

	GuildMemberBaseInfoStruct* selectInfo =g_internal->GetSelectBaseInfoStruct();

	if ( selectInfo )
	{
		GuildMemberStruct* getMemData=GetGuildMemberStruct(selectInfo->MemberDBID);

		if (getMemData)
		{
			return getMemData->LeaderNote.Begin();
		}
	}
	return NULL;
}

//-------------------------------------------------------
// 取得會員幹部註解

int LuaFunc_GF_GetMemberCadreNote(lua_State* L)			
{
	const char*	note=NULL;
	if (g_pGuildFrame)
	note=g_pGuildFrame->GetMemberCadreNote();
		
		
	lua_pushstring(L,note);
	return 1;
}
//---------------------------------------------------------------
// 設定個人註解
bool		 CGuildFrame::SetMemberSelfNote(int db,const char* name)
{
	bool ret=false;
	GuildMemberStruct* getMemData=GetGuildMemberStruct(db);

	if (getMemData)
	{

		if (CheckStr(name,64)==true)
		{

			char tempBuff[512];
			strncpy(tempBuff,name,255);

			tempBuff[255]=0;		
			 size_t destLen=255;

			getMemData->SelfNote=ItemUTIL::checkUtf8SafeString(tempBuff,destLen,NULL);
			NetCli_Guild::SL_ModifyGuildMemberInfoRequest(*getMemData);
			ret=true;
		}

	}
	return ret;

}
//---------------------------------------------------------------------
// 設定個人註解
int LuaFunc_GF_SetMemberSelfNote(lua_State* L)			
{
	int index=luaL_checkint(L,1);
	const char* Note=luaL_checkstring(L,2);

	bool ret=false;

	if (g_pGuildFrame)
		ret=g_pGuildFrame->SetMemberSelfNote(index,Note);

	lua_pushboolean(L,ret);
	return 1;

}
//---------------------------------------------------
// 取得個人註解
const char*		CGuildFrame::	GetMemberSelfNote(void)
{

	GuildMemberBaseInfoStruct* selectInfo = g_internal->GetSelectBaseInfoStruct();

	if ( selectInfo )
	{
		GuildMemberStruct* getMemData=GetGuildMemberStruct(selectInfo->MemberDBID);

		if (getMemData)
		{
			return getMemData->SelfNote.SafeStr();
		}
	}
	return NULL;
}
//---------------------------------------------------
// 取得個人註解
int LuaFunc_GF_GetMemberSelfNote(lua_State* L)			

{
	const char*	note=NULL;
	if (g_pGuildFrame)
		note=g_pGuildFrame->GetMemberSelfNote();


	lua_pushstring(L,note);
	return 1;

}

void  CGuildFrame::AskLeaderChange(const char* SendName)
{


	_Guild_data*date=g_internal->GetSelfGuildData();

	if (!strcmp(date->m_info.LeaderName,SendName))
	{
		// 跳出訊息視窗
		lua_State* L = GetLuaState();
		lua_pushstring(L, SendName);
		lua_setglobal(L, "arg1");

		SendWorldEvent("GUILD_ASK_LEADERCHANGE");

	}

	
	



}

void CGuildFrame::AskLeaderChangeResult(bool YesNo)
{

	_Guild_data*date=g_internal->GetSelfGuildData();

	GuildMemberBaseInfoStruct* selfInfo = g_internal->GetSelfBaseInfo();

	if (selfInfo&&date)
	{

		NetCli_ChannelChild::SendToCliAskLeaderChangeResult(date->m_info.LeaderName.Begin(), selfInfo->MemberDBID,YesNo);

	}



}

void  CGuildFrame::AskLeaderChangeResult(int dbid,bool YesNo)
{

	_Guild_data*date=g_internal->GetSelfGuildData();
	if (date)
	{
		GuildMemberBaseInfoStruct* selfInfo = g_internal->GetSelfBaseInfo();
		GuildMemberBaseInfoStruct* selectInfo = g_internal->GetSelectBaseInfoStruct();
		if (selfInfo&&selectInfo)
		{
			if (selfInfo->Rank==EM_GuildRank_Leader&&selectInfo->MemberDBID== dbid)
			{

				if (!YesNo)
				{
					g_internal->SetLeaderChangeEnd();
					// 跳出訊息視窗
					lua_State* L = GetLuaState();
					lua_pushstring(L, selectInfo->MemberName.Begin());
					lua_setglobal(L, "arg1");

					lua_pushboolean(L, YesNo);
					lua_setglobal(L, "arg2");

					SendWorldEvent("GUILD_ASK_LEADERCHANGE_RESULT");
				}
				else
				{

					BeginMemberInfoRequest();
					AddMemberInfoRequest(selfInfo->MemberDBID);
					AddMemberInfoRequest(selectInfo->MemberDBID);
					EndMemberInfo(&CGuildFrame::LeaderChange);			

				}

			}
		}

	}


}
bool CGuildFrame::SetGuildDesc( const char* desc, bool V, bool visitHouse )
{
	bool ret=false;

	m_tempGuildInfo.IsRecruit=V;

	if (CheckStr(desc,500)==true)
	{
		ret=true;
		char tempBuff[512]={0};
		strncpy(tempBuff,desc,500);

		tempBuff[500]=0;		
		size_t destLen=500;

		m_tempGuildInfo.Introduce=ItemUTIL::checkUtf8SafeString(tempBuff,destLen,NULL);
		
	}
											   
	m_tempGuildInfo.IsLockGuildHouse = !visitHouse;

	return ret;
}
int LuaFunc_GF_CanGuildWaring(lua_State* L)	
{
	bool ret=false;
	if (g_pGuildFrame)
		ret=g_pGuildFrame->CanGuildWaring();


	lua_pushboolean( L,ret);
	return 1;
}
//---------------------------------------------------
//設定公會招生與敘述.
int LuaFunc_GF_SetGuildDesc(lua_State* L)		
{

	bool ret=false;

	
	const char * desc=luaL_checkstring(L,1);
	bool enable=lua_toboolean(L,2) != 0;
 	bool visitHouse=lua_toboolean(L,3) != 0;

	if (g_pGuildFrame)
		ret=g_pGuildFrame->SetGuildDesc(desc,enable,visitHouse);

	lua_pushboolean( L,ret);

	return 1;
}
//---------------------------------------------------

int LuaFunc_GC_CloseGuildCommand(lua_State* L)
{
	NetCli_Guild::SL_CloseCreateGuild( );
	if (g_pGuildFrame)
              g_pGuildFrame->  CloseGuildCommand(); //關閉公會命令介面.
	return 0;
}








//---------------------------------------------------
//公會告示版ui 取得公會ID及名稱 .
int Luafunc_GuildBoard_GetNextGuildIDName(lua_State* L)
{
	int index = (int)luaL_checknumber(L,1);	

	simpleGuildMap &map=g_internal->GetGuildMap();
		
	simpleGuildMap::iterator it = map.upper_bound(index);

	if( it == map.end() )
	{
		return 0;
	}	
	_Guild_data& data = it->second;
	lua_pushnumber( L, it->first );
 	lua_pushstring( L, data.m_name );
 
	return 2;
}

//---------------------------------------------------
//公會告示版ui 取得公會資訊.
int LuaFunc_GuildBoard_RequestGuildInfo(lua_State* L)
{
	int guildID = (int)luaL_checknumber(L,1);
	//g_pGuildFrame->GuildBoardRequest(guildID);
	NetCli_GuildChild::GuildBoardRequest( guildID );
	return 0;
}
//---------------------------------------------------
//檢查10G.
int LuaFunc_GF_CanCreateGuildCost(lua_State* L)
{
	bool v=false;

	int price=g_ObjectData->GetSysKeyValue("Guild_CreateCost");
//	int price=CREATEGUILD_COST;

#ifdef _DEBUG 
price=0;
#endif

	if (CNetGlobal::RoleData()->BodyMoney() >= price )
		v=true;


	lua_pushboolean( L, v );

	return 1;

}
//---------------------------------------------------
//公會告示版ui 設定公會告示.
void		CGuildFrame::BBS_PageDown(void)
{
	NetCli_Guild::SL_BoardListRequest(m_BBSPage+1);

}
void		CGuildFrame::BBS_PageUP(void)
{
	if (m_BBSPage>0)
		NetCli_Guild::SL_BoardListRequest(m_BBSPage-1);

}
void       	CGuildFrame::BBSPageUpdate(void)
{
	// 通知頁面更新.

	m_reloadIndex++;
	if (m_reloadIndex>=m_maxBBSPage)
	{

		
		
		sortList();
		
	}
	else
	{
		m_bCanSend=true;

		reLoadBBSPage(m_reloadIndex);


	}

}


void CGuildFrame::SetBBSPageCount(int MaxCount,int Count,int PageID)
{

	m_maxBBSPage=(MaxCount/10);
	if (MaxCount%10)
		m_maxBBSPage++;


	if (MaxCount!=m_BBSMessages.size())
		m_BBSMessages.resize(MaxCount);
	m_BBSPage=PageID;

	int now_max=(PageID*10+Count);


	if (PageID>=0&&PageID<6)
	m_BBSTime[PageID]=timeGetTime();
	//if (now_max<MaxCount)
	//	NetCli_Guild::SL_BoardListRequest(m_BBSPage+1);
	//else//m_BBSMessages[]
//	BBSPageUpdate();

}
void CGuildFrame::SetBBSPostDate( int MessageGUID , int Index , char* PlayerName , int Time , char* Title , GuildBoardModeStruct Type )
{
	int max_size=(int)m_BBSMessages.size();
	int pindex=Index+m_BBSPage*10;
	if (pindex>=0&&pindex<max_size)
	{
	
		m_BBSMessages[pindex].m_post.PlayerName=PlayerName;
		m_BBSMessages[pindex].m_post.Time=Time;
		m_BBSMessages[pindex].m_post.Title=Title;
		m_BBSMessages[pindex].m_post.Type=Type;
		m_BBSMessages[pindex].m_post.GUID=MessageGUID;

		m_BBSMessages[pindex].m_time=-1;

		m_BBSMessages[pindex].m_visible=1;
		if (Index==9||(pindex==m_BBSMessages.size()-1))
			BBSPageUpdate();
	}

	


}
bool CGuildFrame::SetGuildNote(const char * note)
{

	bool ret=false;
	 wchar_t u16[512];
	 if (CheckStr(note,500)==true)
	 {
		 ret=true;
		 char tempBuff[512]={0};
		 strncpy(tempBuff,note,500);
		 tempBuff[500]=0;		
		 m_bshowMessage=true;	
		  int len=(int)strlen(tempBuff);
		 size_t destLen=len;
		 m_tempGuildInfo.Note=ItemUTIL::checkUtf8SafeString(tempBuff,destLen,u16);
		len=(int)wcslen(u16);
	 }
	return ret;

}
//---------------------------------------------------
//公會告示版ui 設定公會告示.
int LuaFunc_GF_SetGuildNote(lua_State* L)
{

	bool ret=false;
	const char* Note=luaL_checkstring(L, 1);

	if (g_pGuildFrame)
		ret=g_pGuildFrame->SetGuildNote(Note);


	lua_pushboolean(L,ret);


	return 1;
}

void CGuildFrame::reLoadBBSPage(int page)
{

	if (page>=0&&page<6&&m_bCanSend)
	{

		int time=timeGetTime();
		if (((time-m_BBSTime[page])>DF_GETGUILDINFO_COUNTER)||(m_BBSTime[page]==-1))
		{
			m_bCanSend=false;
			m_BBSPage=page;
			m_BBSTime[page]=time;
			NetCli_Guild::SL_BoardListRequest(m_BBSPage);
		}

	}
}
//---------------------------------------------------
//公會BBS 取得目前留言數量.
int		CGuildFrame::GetPostCount(void)
{
	int count=(int)m_PostList.size();
/*

	for (int i=0;i<7;i++)
	{
		int it=index+i;
		if (it<count)
		{
			int pnow=m_PostList[index+i];

			int now_page=pnow/10;
				reLoadBBSPage(now_page);
		}
	}	*/
	return count;

}

int LuaFunc_GF_GetPostCountRequest(lua_State* L)
{

	if (g_pGuildFrame)
		g_pGuildFrame->GetPostCountRequest();


	return 0;

}

//---------------------------------------------------
//公會BBS 請求目前頁面數量
void		CGuildFrame::	GetPostCountRequest(void)
{

	//測試碼
	/*
	int i;
	for(i=0;i<4;i++)
	{
		char temp[64];
		sprintf(temp,"TestTile%d",i);
		NetCli_Guild::SL_BoardPostRequest(temp,"內容無用");

	}
	*/

	
	m_BBSPage=0;
	m_reloadIndex=0;

	m_bCanSend=true;
	reLoadBBSPage(m_reloadIndex);

	//NetCli_Guild::SL_BoardListRequest(m_BBSPage);

}
//---------------------------------------------------
//公會BBS 取得目前頁面數量
int LuaFunc_GF_GetGuildPostCount(lua_State* L)
{
	//int index=luaL_checkint(L,1);
	//if (index<0)
	//	return 0;
	int count=0;
	if (g_pGuildFrame)
		count=g_pGuildFrame->GetPostCount();

	

	lua_pushnumber(L, count);
	return 1;
}
CGuildFrame::BBSPost*		CGuildFrame::GetPostByID(int guid)
{
	int count=(int)m_BBSMessages.size();
	int i;
	for(i=0;i<count;i++)
	{
		if (m_BBSMessages[i].m_post.GUID==guid)
			return  &m_BBSMessages[i];


	}
	return NULL;
}

CGuildFrame::BBSPost* CGuildFrame::GetPostInfoBySort(int index)
{
	if (index>=0&&index<m_PostList.size())
	{
		int t=	m_PostList[index];
		return GetPostInfo(t);
	}
	return NULL;
}

//公會BBS 取得目前留言資訊
CGuildFrame::BBSPost* CGuildFrame::GetPostInfo(int index)
{

	if (index>=0&&index<m_BBSMessages.size())
	{
		return &m_BBSMessages[index];

	}

	return NULL;
}
//---------------------------------------------------
//公會BBS 取得目前留言資訊
int LuaFunc_GF_GetGuildPostInfo(lua_State* L)
{
	int index=luaL_checkint(L,1)-1;
	if (g_pGuildFrame)
	{


		CGuildFrame::BBSPost* post=g_pGuildFrame->GetPostInfoBySort(index);

		if (post&&post->m_visible)
		{

			lua_pushboolean(L, post->m_post.Type.Top);
		
			lua_pushstring(L, post->m_post.Title.Begin());
			lua_pushstring(L, post->m_post.PlayerName.Begin());

			char temp[MAX_PATH];
			time_t NowTime=post->m_post.Time;
		
			struct tm *gmt;
			gmt = localtime( &NowTime );

			sprintf_s( temp,MAX_PATH , "%02d/%02d/%02d %02d:%02d" ,  1 + gmt->tm_mon , gmt->tm_mday,  gmt->tm_year -100, gmt->tm_hour , gmt->tm_min);
			lua_pushstring(L, temp);
			lua_pushnumber(L, post->m_post.GUID);
			
			return 5;

		}



	}


	return 0;
}

//---------------------------------------------------
//公會BBS 設定目前留言資訊
int LuaFunc_GF_SetGuildPostInfo(lua_State* L)
{
	int guid			=	luaL_checkint(L,1);
	GuildBoardModeStruct PostMode;
	PostMode.Top			=	lua_toboolean(L,2) != 0;
	PostMode.Del			=	lua_toboolean(L,3) != 0;
	if (g_pGuildFrame)
	{	
		CGuildFrame::BBSPost *post=	g_pGuildFrame->GetPostByID(guid);
		if (post)
		{		
			post->m_post.Type=PostMode;
			NetCli_Guild::SL_BoardModifyModeRequest(guid,PostMode);			
			g_pGuildFrame->sortList();
		}
	}

	return 0;
}

//收到公會的會員列表
void CGuildFrame::GuildMemberBaseInfo( vector< GuildMemberBaseInfoStruct >& MemberList)
{
	g_internal->GuildMemberBaseInfo(MemberList);
}
void CGuildFrame::GuildUpgradeRequest				( void )
{
	NetCli_Guild::SL_GuildUpgradeRequest(m_tempGuildInfo.Level+1);
}

void  CGuildFrame::Guild_upGrade(bool Result,int lev)
{
	char tempStr[MAX_PATH];
	const char *format=NULL;
	if (Result)
	{

		g_pRuGlobalEventManager->PlaySoundByFile("sound\\interface\\guildupgrade.mp3", false, 0.0f, ruSOUNDTYPE_INTERFACE);

		sprintf_s(tempStr,MAX_PATH,"MSG_GUILDUPGRADE_LV%d",lev);
		format=g_ObjectData->GetString(tempStr);
		g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, format, "" );

		format=g_ObjectData->GetString("MSG_GUILDUPGRADE_SUCCESS");

		g_internal->ReloadGuildDate();
		//tempStr=;
	}
	else
	{
		format=g_ObjectData->GetString("MSG_GUILDUPGRADE_FAIL");

	}
	sprintf_s(tempStr,MAX_PATH,format,lev);
	g_pGameMain->SendWarningMsg(tempStr);

	
		SendWorldEvent("GRF_UPDATERESOURCE");
}

void CGuildFrame::GuildUpgradeResult			( int GuildLv , bool Result )
{




	int  lev=m_tempGuildInfo.Level+1;


//	Guild_upGrade(Result,lev);


	SendWorldEvent("GRF_UPDATERESOURCE");
}
void CGuildFrame::SetDayLogTable(int count,ContributionInfoLogStruct* lists)
{
	g_internal->SetDayLogTable(count,lists);


	lua_State* L = GetLuaState();

	lua_pushnumber(L,g_internal-> m_day+1);
	lua_setglobal(L, "arg1");


	SendWorldEvent("GRF_UPDATELOG");

}
//---------------------------------------------------
//公會BBS 設定留言
int  CGuildFrame::SetGuildPost(int guid,const char *tTitle,const char *tMessage)
{
	BBSPost *post=	GetPostByID(guid);
	int  ret=0;
	if (post)
	{
		if (CheckStr(tTitle,127)!=true) 
			ret|=1;
		if (CheckStr(tMessage,511)!=true) 
			ret|=2;
		if (ret==0)
		{



			
			size_t destLen=127;
			char tempTitleBuff[128];
			strncpy(tempTitleBuff,tTitle,127);
			tempTitleBuff[127]=0;		
			post->m_post.Title=ItemUTIL::checkUtf8SafeString(tempTitleBuff,destLen,NULL);

			char tempMessageBuff[512];
			strncpy(tempMessageBuff,tMessage,511);
			destLen=511;
			const char *safeMessage=ItemUTIL::checkUtf8SafeString(tempMessageBuff,destLen,NULL);
			

			
			post->m_post.Message=safeMessage;
			NetCli_Guild::SL_BoardModifyMessageRequest(guid,safeMessage);	
		}
	}
	return ret;
}
//收到公會資源變動
void CGuildFrame::ModifySimpleGuild( SimpleGuildInfoStruct& SimpleGuildInfo )
{
	m_tempGuildInfo.Resource=SimpleGuildInfo.Resource;	

	g_internal->ModifySimpleGuild(SimpleGuildInfo);	
}
//--接收到公會資料 可能是查詢其他工會的 或是自己的公會
void CGuildFrame::GuildInfo(GuildBaseStruct& Guild , int MemberCount,bool bNeedSend )
{
	g_internal->GuildInfo(Guild,MemberCount,bNeedSend);	
}

//取得公會基本資料
void CGuildFrame::GuildBoardRequest( int guildID,bool&v)
{
	g_internal->GuildBoardRequest(guildID,v);
}
//---------------------------------------------------
//公會BBS 設定留言
int LuaFunc_GF_SetGuildPost(lua_State* L)		
{
	int guid			=	luaL_checkint(L,1);
	const char * Title	=	luaL_checkstring(L,2);
	const char * Message=	luaL_checkstring(L,3);

	int ret=0;
	if (g_pGuildFrame)
		ret=g_pGuildFrame->SetGuildPost(guid,Title,Message);

	lua_pushboolean(L,ret&1);
	lua_pushboolean(L,ret&2);
	


	return 2;
}
void CGuildFrame:: Load(void)
{

m_BBS_SystemStr[0]=g_ObjectData->GetString("GUILD_MSG_SHORTNOTE");
m_BBS_SystemStr[1]=g_ObjectData->GetString("GUILD_MSG_CONTEXT");
m_BBS_SystemStr[2]=g_ObjectData->GetString("GUILD_MSG_POST_AUTHOR");
m_BBS_SystemStr[3]=g_ObjectData->GetString("GUILD_MSG_POST_TITLE");
m_BBS_SystemStr[4]=g_ObjectData->GetString("GUILD_MSG_POST");
m_BBS_SystemStr[5]=g_ObjectData->GetString("GUILD_MSG_POST_DATE");
m_BBS_SystemStr[6]=g_ObjectData->GetString("GUILD_MSG_CONTEXT");
//srand(time(0));



}
void CGuildFrame::GuildContributionItemResult(int eResct)
{



	 char tempStr[MAX_PATH];
	if (eResct)
	{
		const char *format=g_ObjectData->GetString("MSG_GUILDCONTRIBUTION_SUCCESS");
		strcpy(tempStr,format);
		//tempStr=;
	}
	else
		strcpy(tempStr,g_ObjectData->GetString("MSG_GUILDCONTRIBUTION_NOGUILD"));
	//g_pGameMain->SendWarningMsg(tempStr);
		
g_internal->ReloadGuildDate();
	g_pGameMain->SendWarningMsg(tempStr);
	//g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, tempStr, "" );

}

void CGuildFrame::Save()
{
SaveBBSToFile();
}
void CGuildFrame::SaveBBSToFile(void)
{

	char globalName[MAX_PATH];
	GetLocalPath(BBS_POST_LOG, globalName, MAX_PATH);
	IniFile2 BBSlog;


	BBSlog.SetSection(m_BBS_SystemStr[0].c_str());	

	BBSlog.SetStr(m_BBS_SystemStr[1].c_str(),m_tempGuildInfo.Note.SafeStr());

	char temp[4096];
	for (int i=0;i<m_PostList.size();i++)
	{
		sprintf_s(temp,4096,m_BBS_SystemStr[4].c_str(),i);
		int pindex=	m_PostList[i];
		BBSPost* post=GetPostInfo(pindex);

		BBSlog.SetSection(temp);
		if (post&&post->m_visible)
		{
			BBSlog.SetStr(m_BBS_SystemStr[2].c_str(),post->m_post.PlayerName.Begin());
			BBSlog.SetStr(m_BBS_SystemStr[3].c_str(),post->m_post.Title.Begin());
			char dateX[MAX_PATH];
			time_t NowTime=post->m_post.Time;
			struct tm *gmt;
			gmt = localtime( &NowTime );
			sprintf_s( dateX ,MAX_PATH, "%02d/%02d/%02d %02d:%02d" ,  1 + gmt->tm_mon , gmt->tm_mday,  gmt->tm_year -100, gmt->tm_hour , gmt->tm_min);
			BBSlog.SetStr(m_BBS_SystemStr[5].c_str(),dateX);
			BBSlog.SetStr(m_BBS_SystemStr[6].c_str(),post->m_post.Message.Begin());	
		}
	}
	WCHAR  wtemp[1024];

	MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, globalName, -1, wtemp, 1024) ;

	HANDLE hFile = CreateFileW( wtemp,  GENERIC_WRITE , FILE_SHARE_READ , NULL , CREATE_ALWAYS , FILE_ATTRIBUTE_NORMAL , NULL );
	if (hFile!=INVALID_HANDLE_VALUE)
	{

		DWORD nWritten=0;
		std::string buff;

		BBSlog.SaveString(buff);

		WriteFile( hFile, buff.c_str(), (DWORD)buff.length(), &nWritten, NULL ) ;
		CloseHandle(hFile);
	}
}

//踢除成員
void CGuildFrame::RemoveMemberByKick(int LeaderDBID,int memid)
{
	 g_internal->RemoveMemberByKick(LeaderDBID,memid);
}


void CGuildFrame::SetBoardMessage(GuildBoardStruct &Message)
{
	int count=(int)m_BBSMessages.size();
	for(int i=0;i<count;i++)
	{
		if (m_BBSMessages[i].m_post.GUID==Message.GUID)
		{

			lua_State* L = GetLuaState();

			if (L)
			{


				m_BBSMessages[i].m_post=Message;		

				lua_pushnumber(L, Message.GUID);
				lua_setglobal(L, "arg1");


				SendWorldEvent("OPEN_POSTVIEW");
				return ;
			}
		}


	}



}
void CGuildFrame::GetPostContextRequest(int guid)
{
	int count=(int)m_BBSMessages.size();
	int time=timeGetTime();
	for(int i=0;i<count;i++)
	{
		if (m_BBSMessages[i].m_post.GUID==guid)
		{
			if ((time-m_BBSMessages[i].m_time)>DF_POST_COOLDOWN||(m_BBSMessages[i].m_time==-1))
			{
				m_BBSMessages[i].m_time=time;
				NetCli_Guild::SL_BoardMessageRequest(guid);
			}
			else
			{
					lua_State* L = GetLuaState();

				lua_pushnumber(L, guid);
				lua_setglobal(L, "arg1");


				SendWorldEvent("OPEN_POSTVIEW");


			}


		}

	}

	
}
//---------------------------------------------------
//公會BBS // 取得留言
int LuaFunc_GF_GetGuildPostRequest(lua_State* L)
{
	int GUID=luaL_checkint(L,1);

	if (g_pGuildFrame)
	{
		g_pGuildFrame->GetPostContextRequest(GUID);

	}



	return 0;

}

void		CGuildFrame::RecvBoardPost(bool Result)
{
	if (Result)
	{
		int i;
		for(i=0;i<6;i++)
		{
			m_BBSTime[i]=-1;
		}


		GetPostCountRequest();
		
		//NetCli_Guild::SL_BoardListRequest(m_BBSPage);
		//BBSPageUpdate();
	}
}

//---------------------------------------------------
//公會BBS // 新增留言
int	CGuildFrame::NewPost(const char* tTitle,const char* tMessage)
{

	int ret=0;
	int time=timeGetTime();

	if (CheckStr(tTitle,127)!=true)
		ret|=1;

	if (CheckStr(tMessage,512)!=true)
		ret|=2;

	//留言間隔時間檢查  DEBUG模式30 秒 release 2分鐘
#ifdef _DEBUG
	if ((time-m_BBSPostTime)<30*1000)
		ret|=4;
#else
	if ((time-m_BBSPostTime)<DF_POST_COOLDOWN)
			ret|=4;

#endif

	if (ret==0)
	{
		
		size_t destLen=127;

		char tempTitleBuff[128];
		strncpy(tempTitleBuff,tTitle,127);
		tempTitleBuff[127]=0;		


		const char* safeTitle=ItemUTIL::checkUtf8SafeString(tempTitleBuff,destLen,NULL);

		strcpy(tempTitleBuff,safeTitle);
		char tempMessageBuff[512];
		strncpy(tempMessageBuff,tMessage,511);
		tempMessageBuff[511]=0;		
		 destLen=511;
		const char*  safeMessage=ItemUTIL::checkUtf8SafeString(tempMessageBuff,destLen,NULL);

		strcpy(tempMessageBuff,safeMessage);

	
		
		m_BBSPostTime=time;

		NetCli_Guild::SL_BoardPostRequest(tempTitleBuff,tempMessageBuff);
	}

		
		
	

	return ret;


	

}

//---------------------------------------------------
//公會BBS // 新增留言
int LuaFunc_GF_NewPost(lua_State* L)
{

	const char * Title=luaL_checkstring(L,1);
	const char * Message=luaL_checkstring(L,2);
	int ret=0;
	if (g_pGuildFrame)
		ret=g_pGuildFrame->NewPost(Title,Message);

	lua_pushboolean(L, ret&1);
	lua_pushboolean(L, ret&2);
	lua_pushboolean(L, ret&4);


	return 3;
}
/*
//---------------------------------------------------
//公會BBS 	// 刪除留言
int LuaFunc_GF_DeletePost(lua_State* L)
{
		return 0;
}*/
//---------------------------------------------------
// 請求目前幾筆資料
int LuaFunc_GF_GetBBSInfoRequest(lua_State* L)
{
	if (g_pGuildFrame)
		g_pGuildFrame->GetPostCountRequest();
	
	GuildMemberBaseInfoStruct* selfInfo = g_internal->GetSelfBaseInfo();
	_Guild_data*date=g_internal->GetSelfGuildData();
	if (!date)
		return 0;

	GuildBaseStruct& guildInfo =date->m_info;

	GuildRankBaseInfoStruct* self=0;
	if ( selfInfo )
	{
		self= &guildInfo.Rank[selfInfo->Rank];	
		lua_pushboolean(L,self->Setting.SetGuildNote);
		lua_pushboolean(L,self->Setting.PostMessage);
	
		return 2;
	}

	
	return 0;
}
//---------------------------------------------------
// 請求目前幾筆資料
int LuaFunc_GF_GetPostDate(lua_State* L)
{
	int GUID=luaL_checkint(L,1);

	if (g_pGuildFrame)
	{
		CGuildFrame::BBSPost* pPost=g_pGuildFrame->GetPostByID(GUID);
		if (pPost)
		{

			lua_pushnumber(L,pPost->m_post.Type._Type);
			lua_pushstring(L,pPost->m_post.Title.Begin());
			lua_pushstring(L,pPost->m_post.Message.Begin());
			lua_pushstring(L,pPost->m_post.PlayerName.Begin());

			char temp[512];
			time_t NowTime=pPost->m_post.Time;

			struct tm *gmt;
			gmt = localtime( &NowTime );

			sprintf_s( temp ,512, "%02d/%02d/%02d %02d:%02d" ,  1 + gmt->tm_mon , gmt->tm_mday,  gmt->tm_year -100, gmt->tm_hour , gmt->tm_min);
			lua_pushstring(L, temp);

			return 5;
		}


		

	}



	return 0;

}

int LuaFunc_GF_GetBBSMenuFlag(lua_State* L)
{

	GuildMemberBaseInfoStruct* selfInfo = g_internal->GetSelfBaseInfo();
	_Guild_data*date=g_internal->GetSelfGuildData();
	if (!date)
		return 0;

	GuildBaseStruct& guildInfo =date->m_info;

	GuildRankBaseInfoStruct* self=0;
	if ( selfInfo )
	{
		self= &guildInfo.Rank[selfInfo->Rank];	
		lua_pushboolean(L,self->Setting.ManageMessage);
		//lua_pushboolean(L,self->Setting.ManageMessage);
		//lua_pushboolean(L,self->Setting.ManageMessage);

		return 1;
	}
	return 0;
}
GuildMemberListInfo* CGuildFrame::GetGuildRosterInfo(int index)
{

	return g_internal->GetGuildMemberInfo(index);

}

int CGuildFrame::GetNumGuildMembers(void)
{
	int count=	g_internal->GetNumGuildMembers();

	return count;
}
void CGuildFrame::ReadAllMemberDate(void)
{
	int count=	g_internal->GetNumGuildMembers();
	for (int i=0;i<count;i++)
	{
		GuildMemberListInfo* info = g_internal->GetGuildMemberInfo(i);
			g_internal->ReloadGuildMemberDate(info->member->MemberDBID);
	}
}
int LuaFunc_GF_ReadAllDate(lua_State* L)
{
	if (g_pGuildFrame)
	{
		g_pGuildFrame->ReadAllMemberDate();

	}
	return 0;
}


//讀取BAG的物品
int LuaFunc_GCB_GetContributionItem(lua_State* L)
{
	if (g_pGuildFrame)
	{
		return g_pGuildFrame->GetContributionItem();

	}
	return 0;
}
  
int LuaFunc_GCB_RemoveContributionItem(lua_State* L)
{
	int index=luaL_checkint(L,1)-1;
	if (g_pGuildFrame)
	{
		 g_pGuildFrame->RemoveContributionItem(index);

	}
	return  0;
}
//------------------------------------------------------------------
//移除貢品
void CGuildFrame::RemoveContributionItem(int index)
{ 
	std::vector<int>::iterator it=m_itemPos.begin();
	for(;it!=m_itemPos.end();++it)
	{
		if (*it==index)
		{
			ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(index);
			if (item)
			{
				GameObjDbStruct* itemDB = CNetGlobal::GetObj(item->OrgObjID);		
				item->Pos=EM_ItemState_NONE;



				m_itemPos.erase(it);
				break;
			}
		}
	}
}
//關閉貢獻界面
void CGuildFrame::CloseGuildContribution(void)
{

	CloseContribution();

	// 關閉貢獻界面
	SendWorldEvent("GUILD_CONTRIBUTION_CLOSE");
	m_npcId=-1;
	m_Open=false;
}
//.把供品傳給server
void CGuildFrame::ContributionItemOK(int Money,int BonusMoney)
{
	//戰場中
	if( g_pGameMain->IsBattleGroundZone() )
	{
		return ;

	}

m_GuildPointTotal=Money;
	// 檢查公會是否已成立或者已經加入公會
	if ( CNetGlobal::RoleData()->BaseData.GuildID == 0 )
	{

		
		const char* tempStr;

		tempStr=g_ObjectData->GetString("MSG_GUILDCONTRIBUTION_NOGUILD");

		g_pGameMain->SendWarningMsg(tempStr);
		//g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, tempStr, "" );
		return ;
	}

	int count=(int)m_itemPos.size();
	ItemFieldStruct* item = NULL;
	int index;
	
	for(int i=0;i<count;i++)
	{
		

		index=m_itemPos[i];
		item = CNetGlobal::RoleData()->GetBodyItem(index);
		if (item)
		{
			GameObjDbStruct* itemDB = CNetGlobal::GetObj(item->OrgObjID);			
			m_GuildPointTotal+=itemDB->Item.GuildContributionValue*item->GetCount();
			NetCli_Guild::SL_GuildContributionItem(index,*item,true);
		}


	}
	m_itemPos.clear();
	if (count||Money>0||BonusMoney>0)
	{
		NetCli_Guild::SL_GuildContributionOK( Money,BonusMoney );

		
	}
}
//確定貢獻...把供品傳給server
int LuaFunc_GCB_ContributionItemOK(lua_State* L)
{
	int Money=luaL_checkint(L,1);
	int BonusMoney=luaL_checkint(L,2);
	if (g_pGuildFrame)
	{
		g_pGuildFrame->ContributionItemOK(Money,BonusMoney);

	}

return  0;
}

int CGuildFrame::GetGuildResource(int index)

{
	int point=0;

	if (index>=0&&index<7)
	point=m_tempGuildInfo.Resource._Value[index];
	return point;

}
int CGuildFrame::GetGuildLevelInfo(int level,int index)
{
	int aVaule=-1;

	vector< GuildLvInfoStruct >&Table=g_ObjectData->GuildLvTable();

	if (level>=0&&level<Table.size())
	{
		switch (index)
		{
		case 0:		case 1:	case 2:		case 3:		case 4:		case 5:		case 6:
			aVaule=	Table[level]._UpgradeValue[index];
			break;		
		case 7://	int RankCount;
			aVaule=	Table[level].RankCount;
			break;	
		
		case 8://int	MemberCount;
			aVaule=	Table[level].MemberCount;
			break;
		case 9://	int	DeclareWar;
			aVaule=	Table[level].DeclareWar;
			break;
		case 10://	int	GuildFlag;
			aVaule=	Table[level].GuildFlag;
			break;
		case 11://	int	GuildHouse;
			aVaule=	Table[level].GuildHouse;
			break;
		case 12://	int	GuildWar;
			aVaule=	Table[level].GuildWar;
			break;
		case 13://	int	CityWar;
			aVaule=	Table[level].CityWar;
			break;
		case 14://	int	Quest;
			aVaule=	Table[level].Quest;
			break;
	

		case 15://	int	ChangeGuildStyle;
			aVaule=	Table[level].ChangeGuildStyle;
			break;
		
		}
		

	}

	return aVaule;


}


int CGuildFrame::GetGuildLevel(void)

{
	
		return m_tempGuildInfo.Level;
	

}
void CGuildFrame::LeaveWorld()
{
	for (int i=0;i<6;i++)
	m_BBSTime[i]=-1;
	m_BBSPostTime=-1;
	m_PostList.clear();
	m_BBSPage=0;
	m_BBSMessages.clear();
if (g_internal)
	g_internal->GetGuildMap().clear();
}
//---公會積分
int CGuildFrame::GetGuildPoint()
{
	

	 return /*m_tempGuildInfo.Score*/0;


}

//取得自己公會名稱
const char *CGuildFrame::GetGuildName(void)
{
	return m_tempGuildInfo.GuildName.Begin();
}

//取得公會名稱
const char *CGuildFrame::GetGuildNameByID(int GuildID)
{
	return g_internal->GetGuildNameByID(GuildID);
}
int CGuildFrame::GetGuildIDByName(const char* name)
{
	return g_internal->GetGuildIDByName(name);
}
//受到連署邀請
void CGuildFrame::NewGuildInvite( int GuildID, char* LeaderName, int LeaderDBID )
{
	g_internal->NewGuildInvite(GuildID,LeaderName,LeaderDBID);
}

//關閉貢獻介面
void CGuildFrame::CloseContribution(void)
{

	NetCli_Guild::SL_GuildContributionClose();
}
int LuaFunc_GCB_CloseContribution(lua_State* L)
{
	if (g_pGuildFrame)
	{
		g_pGuildFrame->CloseGuildContribution();

	}

	return  0;
}


int LuaFunc_GCB_GetGuildResource(lua_State* L)
{
	int point=0;

	int index=luaL_checkint(L,1)-1;
	if (g_pGuildFrame)
	{
		point=	g_pGuildFrame->GetGuildResource(index);



	}
	lua_pushnumber(L,point);

	return  1;
}
int LuaFunc_GCB_GetGuildPoint(lua_State* L)
{
	int point=0;

	if (g_pGuildFrame)
	{
		point=	g_pGuildFrame->GetGuildPoint();


			
	}
	lua_pushnumber(L,point);

	return  1;
}
int LuaFunc_GSF_CloseGuildShop(lua_State* L)
{
	if (g_pGuildFrame)
	{
		g_pGuildFrame->CloseGuildShop();

	}


	return  0;
}
int		CGuildFrame::GetGuildShopCount(void)
{
	int count=0;
	
/*
	vector<GuildShopInfoStruct>& shop=g_ObjectData->GuildShop();

	m_ShoptabList.clear();
	int ShopCount=shop.size();
	for(int i=0;i<ShopCount;i++)
	{
		bool needflag=false;
		int NeedFlagID=shop[i].NeedFlagID;
		if (NeedFlagID!=-1)
			needflag=m_tempGuildInfo.Flag.GetFlag(NeedFlagID);
		if (NeedFlagID==-1 || needflag ||m_tempGuildInfo.Flag.GetFlag(i))
		{
			ShopTrans tmp;
			tmp.eable=m_tempGuildInfo.Flag.GetFlag(i);
			tmp.index=i;
			m_ShoptabList.push_back(tmp);
			count++;
		}
	}
*/	
	return count;
}
int LuaFunc_GSF_GetGuildShopCount(lua_State* L)
{
	int count=0;


	if (g_pGuildFrame)
	{
		count=g_pGuildFrame->GetGuildShopCount();

	}
	lua_pushnumber(L,count);
return  1;
}

//離開連署公會 　使用踢人指令者會收到
void CGuildFrame::LeaveNewGuildResult	( char* KickPlayerName , bool Result )
{

	g_internal->LeaveNewGuildResult(KickPlayerName,Result);

}

int  CGuildFrame::GetGuildShopFuncInfo(int index)
{
	lua_State* L = GetLuaState();
	int cost=0;
	int ID=0;
	const char *msg=NULL;
	bool ebale=false;
	/*
	if (index>=0&&index<m_ShoptabList.size())
	{
		char temp[512];
		
		vector<GuildShopInfoStruct>& shop=g_ObjectData->GuildShop();
		ShopTrans & t=m_ShoptabList[index];

		ID=t.index;
		cost=shop[ID].Cost;
		ebale=t.eable;

		sprintf(temp,"GUILD_SHOPFUNC_FLAG%d",shop[ID].FlagID+1);

		msg=g_ObjectData->GetString(temp);
	}
	*/
	
	lua_pushnumber(L,cost);
	lua_pushboolean(L,ebale);
	lua_pushstring(L,msg);
	lua_pushnumber(L,ID);
	return 4;

}

int LuaFunc_GSF_GetGuildShopFuncInfo(lua_State* L)
{

	int index=luaL_checkint(L,1)-1;
	if (g_pGuildFrame)
	{

		return g_pGuildFrame->GetGuildShopFuncInfo(index);
	}


	return  0;
}
void CGuildFrame::GuildShopBuyResult(int id, bool result)
{
/*	char tempStr[MAX_PATH];

	vector<GuildShopInfoStruct>& shop=g_ObjectData->GuildShop();
	//ShopTrans & t=m_ShoptabList[id];

	sprintf(tempStr,"GUILD_SHOPFUNC_FLAG%d",shop[id].FlagID+1);
	const char *pmsg=g_ObjectData->GetString(tempStr);

	const char *pmsg1;
	if (result)
	{
		
		pmsg1=g_ObjectData->GetString("SUCCEEDED");
	}
	else
		pmsg1=g_ObjectData->GetString("FAIL");

	ShopFubTab::iterator end=m_ShoptabList.end();


	for (ShopFubTab::iterator  it=m_ShoptabList.begin();it!=end;++it)
	{
		
		if (it->index==id)
		{

			m_ShoptabList.erase(it);
			break;
		}
	}
	if (m_ShoptabList.size()==0)
	{
		// 要求重新取得自己公會資訊
		g_internal->ReloadGuildDate();
		// 開啟公會商店
		//SendWorldEvent("GUILD_SHOP_OPEN");

	}

	

	
	sprintf(tempStr,"%s %s",pmsg,pmsg1);
	

	g_pGameMain->SendWarningMsg(tempStr);
	*/
}
//正式公會邀請
void CGuildFrame::GuildInviteResult(char* PlayerName , int	Result )
{
	g_internal->GuildInviteResult(PlayerName,Result);
}

//離開公會　踢者　
void CGuildFrame::LeaveGuildResult	( char* KickPlayerName , bool Result )
{
	g_internal->LeaveGuildResult(KickPlayerName,Result);
}



//創立公會的回覆
void CGuildFrame::CreateGuildResult( int GuildID , char* GuildName , int Result )
{
	g_internal->CreateGuildResult(GuildID ,GuildName,Result);
}
//加入公會
void CGuildFrame::JoinGuild( int GuildID , char* LeaderName )
{
	g_internal->JoinGuild(GuildID,LeaderName);

}
//加入連署公會
void CGuildFrame::JoinNewGuild( int GuildID , char* LeaderName )
{
	g_internal->JoinNewGuild(GuildID,LeaderName);

}

//---------------------------------
//公會商店買
void CGuildFrame::GuildShopBuy(int dbid)
{
	/*
	vector<GuildShopInfoStruct>& shop=g_ObjectData->GuildShop();
	if (dbid>=0&& dbid<shop.size())
	{
		NetCli_Guild::SL_GuildShopBuy	( dbid );
	}
	*/
}
void CGuildFrame::GuildCanNotJoin()
{
	m_guildID		=0;
	m_leaderDBID	=0;//m_leaderDBID;
	m_leaderName.clear();
}
int  LuaFunc_CheckRightfulName(lua_State* L)
{

	const char * stringa=luaL_checkstring(L,1);
	WCHAR wtemp[2048];
	char  temp[2048];
	memset(wtemp,0,2048);

	strncpy(temp,stringa,63);

	size_t destLen=63;

	stringa=ItemUTIL::checkUtf8SafeString(temp,destLen,wtemp);

	
	CountryTypeENUM language=(CountryTypeENUM)g_ObjectData->GetDataLanguageID();

	std::wstring widestring;
	int resultCode = g_ObjectData->CheckGuildNameRules(stringa, (CountryTypeENUM)language, widestring);


	bool ret=false;


	if (resultCode==EM_CharacterNameRules_Rightful)
		ret=g_pGameMain->CheckPatternString(stringa);



	//bool v=g_pGameMain->CheckRightfulName(stringa);


	lua_pushboolean(L,ret);

	lua_pushnumber(L,(lua_Number)destLen);
	return  2;
}
 
int LuaFunc_GetGuildWarItemInfo(lua_State* L)
{
	int index = luaL_checkint(L, 1);
	int time = 0, money = 0, score = 0;
	g_ObjectData->GetGuildWarDeclareTypeInfo(index, time, money);

	if ( time > 0 )
	{
		lua_pushnumber(L, time);				// 單位分
		lua_pushnumber(L, money);				// 公會幣值
		//lua_pushnumber(L, score);
		return 2;
	}
	return 0;
}

int LuaFunc_DeclareGuildWar(lua_State* L)
{
	CRoleSprite* player = g_pGameMain->GetPlayer();

	int index = luaL_checkint(L, 1);
	const char* guildNmae = luaL_checkstring(L, 2);

	int time = 0, money = 0, score = 0;
	g_ObjectData->GetGuildWarDeclareTypeInfo(index, time, money);

	// 你沒有任何一個公會
	if ( player->GetGuildID() <= 0 )
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_GUILDWAR_NO_GUILD"));
		lua_pushboolean(L, false);
		return 1;
	}


	//_Guild_data* data = g_internal->GetSelfGuildData();


	bool bWar=g_pGuildFrame->CanGuildWaring();
	if ( !bWar )	
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_GUILDWAR_NOT_LEADER"));
		lua_pushboolean(L, false);
		return 1;
	}	

	// 取得公會名稱
	int guildID = g_pGuildFrame->GetGuildIDByName(guildNmae);
	if ( guildID == 0 )
	{
		// 公會名稱不存在
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_GUILDWAR_NOT_EXIST"));
		lua_pushboolean(L, false);
		return 1;
	}

	// 檢查公會金幣
	if ( g_pGuildFrame->GetGuildResource(0) < money )
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_GUILDWAR_NOT_ENOUGH_MONEY"));
		lua_pushboolean(L, false);
		return 1;
	}

	/*
	// 檢查金錢
	if ( CNetGlobal::RoleData()->BodyMoney() <= money )
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_GUILDWAR_NOT_ENOUGH_MONEY"));
		lua_pushboolean(L, false);
		return 1;
	}

	// 檢查積分
	if ( g_pGuildFrame->GetGuildPoint() <= score )
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_GUILDWAR_NOT_ENOUGH_POINT"));
		lua_pushboolean(L, false);
		return 1;
	}
	*/

	NetCli_Guild::SL_DeclareWar(index, guildID);
	lua_pushboolean(L, true);
	return 1;
}

// 取得公會之間宣戰資訊
int LuaFunc_GetGuildWarInfo(lua_State* L)
{
	vector<GuildDeclareWarInfo>& guildWarInfo = NetCli_GuildChild::s_declareWarInfo;
	for ( vector<GuildDeclareWarInfo>::iterator iter = guildWarInfo.begin(); iter != guildWarInfo.end(); iter++ )
	{
		//lua_pushstring(L, g_pGuildFrame->GetGuildNameByID(iter->bothID[0]) );
		//lua_pushstring(L, g_pGuildFrame->GetGuildNameByID(iter->bothID[1]) );
	}
	return 0;
}
//------------------------------------------------------------------
int LuaFunc_GetGuildWarPoint(lua_State* L)
{
	GuildBaseStruct* info = g_pGuildFrame->GetGuildBaseInfo();
	lua_pushnumber(L, info->HousesWar.WinCount);	//勝場
	lua_pushnumber(L, info->HousesWar.EvenCount);	//平手
	lua_pushnumber(L, info->HousesWar.LostCount);	//敗場
	return 3;
}
//------------------------------------------------------------------
int LuaFunc_GetGuildWarOutdoorsPoint(lua_State* L)
{
	GuildBaseStruct* info = g_pGuildFrame->GetGuildBaseInfo();
	lua_pushnumber(L, info->WarWinCount);	//勝場
	lua_pushnumber(L, info->WarEvenCount);	//平手
	lua_pushnumber(L, info->WarLostCount);	//敗場
	return 3;
}
//------------------------------------------------------------------
int LuaFunc_GetGuildWarOutdoorsDeclareInfo(lua_State* L)
{
	CRoleSprite* player = g_pGameMain->GetPlayer();


	if ( player == NULL || player->GetGuildID() == 0 )
		return 0;
	 int GuildID = player->GetGuildID() & 0xffffff;
	vector<GuildDeclareWarInfo>& guildWarInfo = NetCli_GuildChild::s_declareWarInfo;
	for ( vector<GuildDeclareWarInfo>::iterator iter = guildWarInfo.begin(); iter != guildWarInfo.end(); iter++ )
	{
		if ( iter->bothID[0] == GuildID)
		{			
			time_t time = iter->startTime;
			struct tm* tmnow = localtime(&time);
			lua_pushstring(L, g_pGuildFrame->GetGuildNameByID(iter->bothID[1]));
			lua_pushnumber(L, tmnow->tm_mon+1);
			lua_pushnumber(L, tmnow->tm_mday);
			lua_pushnumber(L, tmnow->tm_hour);
			lua_pushnumber(L, tmnow->tm_min);
			lua_pushnumber(L, iter->time);	// 剩於時間(秒)
			return 6;
		}
	}
	return 0;
}

//------------------------------------------------------------------
int LuaFunc_GetGuildWarOutdoorsNumEnemies(lua_State* L)
{
	int count = 0;
	CRoleSprite* player = g_pGameMain->GetPlayer();
	if ( player )
	{

		 int GuildID = player->GetGuildID() & 0xffffff;
		vector<GuildDeclareWarInfo>& guildWarInfo = NetCli_GuildChild::s_declareWarInfo;
		for ( vector<GuildDeclareWarInfo>::iterator iter = guildWarInfo.begin(); iter != guildWarInfo.end(); iter++ )
		{
			if ( iter->bothID[1] ==GuildID )
				count++;
		}
	}
	lua_pushnumber(L, count);
	return 1;
}
//------------------------------------------------------------------
int LuaFunc_GetGuildWarOutdoorsEnemyInfo(lua_State* L)
{
	CRoleSprite* player = g_pGameMain->GetPlayer();
	if ( player == NULL || player->GetGuildID() == 0 )
		return 0;

	 int GuildID = player->GetGuildID() & 0xffffff;

	int count = luaL_checkint(L, 1);
	vector<GuildDeclareWarInfo>& guildWarInfo = NetCli_GuildChild::s_declareWarInfo;
	for ( vector<GuildDeclareWarInfo>::iterator iter = guildWarInfo.begin(); iter != guildWarInfo.end(); iter++ )
	{
		if ( iter->bothID[1] == GuildID )
		{
			count--;
			if ( count == 0 )
			{
				time_t time = iter->startTime;
				struct tm* tmnow = localtime(&time);
				lua_pushstring(L, g_pGuildFrame->GetGuildNameByID(iter->bothID[0]));
				lua_pushnumber(L, tmnow->tm_mon+1);
				lua_pushnumber(L, tmnow->tm_mday);
				lua_pushnumber(L, tmnow->tm_hour);
				lua_pushnumber(L, tmnow->tm_min);
				lua_pushnumber(L, iter->time);	// 剩於時間(秒)
				return 6;
			}
		}
	}
	return 0;
}

int LuaFunc_GSF_GetGuildShopBuy(lua_State* L)
{
	int dbID=luaL_checkint(L,1);
	if (g_pGuildFrame)
	{

		 g_pGuildFrame->GuildShopBuy(dbID);
	}

	return  0;
}

int LuaFunc_GetGuildFlagInsigniaCount(lua_State* L)
{
	lua_pushnumber( L, g_ObjectData->GetGuildFlagInsigniaCount() );
	return 1;
}

int LuaFunc_GuildFlagBannerCount     (lua_State* L)
{
	lua_pushnumber( L, g_ObjectData->GuildFlagBannerCount() );
	return 1;
}

int LuaFunc_GetGuildFlagInsigniaType (lua_State* L)
{
	int index = luaL_checkint(L,1) - 1;
	const char* str = g_ObjectData->GetGuildFlagInsigniaType( index );
	if( NULL == str ) 
		return 0;
	lua_pushstring( L, str );
	return 1;
}

int LuaFunc_GuildFlagBannerType      (lua_State* L)
{
	int index = luaL_checkint(L,1) - 1;
	const char* str = g_ObjectData->GuildFlagBannerType( index );
	if( NULL == str ) 
		return 0;
	lua_pushstring( L, str );
	return 1;

}

int LuaFunc_GuildFlagInfo( lua_State* L )
{
	GuildFlagStruct* pGuildFlag = NULL;

	const char* name =  luaL_checkstring(L,1);	

	simpleGuildMap &map=g_internal->GetGuildMap();
  	simpleGuildMap::iterator itPos = map.begin();
  	simpleGuildMap::iterator itEnd = map.end();
	for( ; itPos != itEnd ; ++itPos )
	{
		if( strcmp( itPos->second.m_name , name ) == 0 )
		{
			pGuildFlag = &(itPos->second.m_info.GuildFlag);
			break;
		}
	}
	if( NULL == pGuildFlag )
	{
		return 0;
	}

	CRuColor4 InsigniaMainColor( (UINT32)pGuildFlag->InsigniaMainColor );
	CRuColor4 InsigniaOffColor ( (UINT32)pGuildFlag->InsigniaOffColor  );
	CRuColor4 BannerMainColor  ( (UINT32)pGuildFlag->BannerMainColor   );
	CRuColor4 BannerOffColor   ( (UINT32)pGuildFlag->BannerOffColor    );
 
	lua_pushnumber( L, pGuildFlag->InsigniaType + 1 );
	lua_pushnumber( L, InsigniaMainColor.m_r    );
	lua_pushnumber( L, InsigniaMainColor.m_g    );
	lua_pushnumber( L, InsigniaMainColor.m_b    );
	lua_pushnumber( L, InsigniaOffColor.m_r     );
	lua_pushnumber( L, InsigniaOffColor.m_g     );
	lua_pushnumber( L, InsigniaOffColor.m_b     );
	lua_pushnumber( L, pGuildFlag->BannerType + 1  );
	lua_pushnumber( L, BannerMainColor.m_r      );
	lua_pushnumber( L, BannerMainColor.m_g      );
	lua_pushnumber( L, BannerMainColor.m_b      );
	lua_pushnumber( L, BannerOffColor.m_r       );
	lua_pushnumber( L, BannerOffColor.m_g       );
	lua_pushnumber( L, BannerOffColor.m_b       );

	return 14;
}

int LuaFunc_SetGuildFlagInfoRequest  (lua_State* L)
{
	GuildFlagStruct GuildFlag;

	GuildFlag.InsigniaType = luaL_checkint(L,1) - 1;

	CRuColor4 InsigniaMainColor( 1.0f, (float)luaL_checknumber(L,2), (float)luaL_checknumber(L,3), (float)luaL_checknumber(L,4) );
	CRuColor4 InsigniaOffColor ( 1.0f, (float)luaL_checknumber(L,5), (float)luaL_checknumber(L,6), (float)luaL_checknumber(L,7) );
	
	GuildFlag.InsigniaMainColor = (INT)InsigniaMainColor.GetARGB();
 	GuildFlag.InsigniaOffColor  = (INT)InsigniaOffColor.GetARGB();

	GuildFlag.BannerType = luaL_checkint(L,8) - 1;	
	CRuColor4 BannerMainColor  ( 1.0f, (float)luaL_checknumber(L,9), (float)luaL_checknumber(L,10), (float)luaL_checknumber(L,11) );
	CRuColor4 BannerOffColor   ( 1.0f, (float)luaL_checknumber(L,12), (float)luaL_checknumber(L,13), (float)luaL_checknumber(L,14) );

	GuildFlag.BannerMainColor = (INT)BannerMainColor.GetARGB();
	GuildFlag.BannerOffColor  = (INT)BannerOffColor.GetARGB();

	NetCli_Guild::SL_SetGuildFlagInfoRequest( GuildFlag );
	return 0;
}


int LuaFunc_GF_Rename(lua_State* L)			// 公會改名
{

	const char* name =  luaL_checkstring(L,1);	

	char temp[1024];
	strcpy(temp,name);
	bool ret=  CheckStr(temp);



	if (ret&&g_internal)
		g_internal->GuildRename(temp);


	return 0;
}
int LuaFunc_GCB_GetGuildLevel(lua_State* L)
{

	int level=0;

	if (g_pGuildFrame)
	{
		level=	g_pGuildFrame->GetGuildLevel();
		if (level<0)
			level=0;



	}

	lua_pushnumber( L, level+1 );

	return 1;
}
int LuaFunc_GCB_GetGuildLevelInfo(lua_State* L)
{
	int res=-1;

	int level = luaL_checkint(L,1)-1;
	int index = luaL_checkint(L,2) - 1;
	if (g_pGuildFrame)
	res=g_pGuildFrame->GetGuildLevelInfo(level,index);


	lua_pushnumber( L,res  );

	return 1;
}


int LuaFunc_GCB_GetGuildResLog(lua_State* L)
{
	int day= luaL_checkint(L,1);//day
	int type = luaL_checkint(L,2) - 1; //type
	int index = luaL_checkint(L,3) -1; //index

	char Name[512];
	char date[512]={0};
	int res=-1;
	
	if (g_internal&&(day>=0&&day<GUILD_LOGDAY_MAX)&&(type>=0&&type<7))
	{
		ContributionLogDay& daylog=	g_internal->GetGuildResLogTable(day);

		if (index>=0&&index<daylog.m_list.size())
		{

			int dbid=daylog.m_list[index].PlayerDBID;
			GuildMemberBaseInfoStruct * sinfo=g_internal->GetMemberBaseInfo(dbid);
			if (sinfo)
			{
				strcpy(Name,sinfo->MemberName.Begin());
				

			}else 
				Name[0]=0;//sprintf(Name,"%08u",dbid);

			res=daylog.m_list[index].Resource._Value[type];
			time_t time= (daylog.m_list[index].Time)*(24*60*60);
				
				
			struct tm *gmt;
			
			gmt = localtime( &time );
			if (gmt)
			sprintf_s( date ,512, "%02d/%02d/%02d" ,  1 + gmt->tm_mon , gmt->tm_mday,  gmt->tm_year -100);
			//, gmt->tm_hour , gmt->tm_min);



			//	float	Time;
			//GuildResourceStruct Resource;
		}
		


	}

	lua_pushstring( L,Name  );
	lua_pushnumber( L,res  );
	lua_pushstring( L,date  );

	return 3;
}
int LuaFunc_GCB_GetGuildResLogAll(lua_State* L)
{
	int day= luaL_checkint(L,1);//day
	
	int index = luaL_checkint(L,2) -1; //index

	char Name[512];
	char date[512]={0};
	int res[7];

	if (g_internal&&(day>=0&&day<GUILD_LOGDAY_MAX))
	{
		ContributionLogDay& daylog=	g_internal->GetGuildResLogTable(day);

		if (index>=0&&index<daylog.m_list.size())
		{

			int dbid=daylog.m_list[index].PlayerDBID;
			GuildMemberBaseInfoStruct * sinfo=g_internal->GetMemberBaseInfo(dbid);
			if (sinfo)
			{
				strcpy(Name,sinfo->MemberName.Begin());


			}
			else 
				Name[0]=0;//sprintf(Name,"%08u",dbid);

			for (int i=0;i<7;i++)
			{
				res[i]=daylog.m_list[index].Resource._Value[i];

			}
			
			time_t time= (daylog.m_list[index].Time)*(24*60*60);


			struct tm *gmt;

			gmt = localtime( &time );
			if (gmt)
				sprintf_s( date ,512, "%02d/%02d/%02d" ,  1 + gmt->tm_mon , gmt->tm_mday,  gmt->tm_year -100);			
		}



	}

	lua_pushstring( L,Name  );
	for (int i=0;i<7;i++)
	{
		lua_pushnumber( L,res[i]  );
	}
	lua_pushstring( L,date  );

	return 9;
}

int LuaFunc_GCB_SetGuildResLogDay(lua_State* L)
{

	int day= luaL_checkint(L,1);//day
	if (g_pGuildFrame)
		g_pGuildFrame->SetGuildResLogDay(day);
	return 0;
}
int LuaFunc_GCB_GetGuildResLogCount(lua_State* L)
{
	int count=0;
	int day= luaL_checkint(L,1);//day


	if (day>=0&&day<GUILD_LOGDAY_MAX)
	{
		ContributionLogDay& daylog=	g_internal->GetGuildResLogTable(day);

		count=(int)daylog.m_list.size();

	}
	lua_pushnumber( L,count  );
	return 1;
}
//------------------------------------------------------
int LuaFunc_GCB_GuildUpgrade(lua_State* L)
{

	if (g_pGuildFrame)
		g_pGuildFrame->GuildUpgradeRequest();

	return 0;
}
//------------------------------------------------------
int LuaFunc_GCB_GetGuildLevelCount(lua_State* L)
{
	int count=0;
	vector< GuildLvInfoStruct >&Table=g_ObjectData->GuildLvTable();
	count=(int)Table.size();
	lua_pushnumber( L,count  );
	return 1;
}

void CGuildFrame::Guild_LevelUp				( int GuildID , int Level )

{
	Guild_upGrade(true,Level);
}

//------------------------------------------------------
int LuaFunc_GetSelfGuildRank(lua_State* L)
{
	int rank = g_pGuildFrame->GetUserRank() + 1;
	lua_pushnumber( L, rank );
	return 1;
}

//------------------------------------------------------

int LuaFunc_GF_AskLeaderChangeResult(lua_State* L)
{
	bool YesNo =  lua_toboolean(L,1) != 0;//day

	if (g_pGuildFrame)
		g_pGuildFrame->AskLeaderChangeResult(YesNo);

	return 0;
}