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
#include "../interface/login/CharacterCreate.h"//�ˬd�W�٦X�k
#include "BagFrame.h"
#include "../WorldFrames/ItemUTIL.h"
#include "../Login/AccountLogin.h"
#include "../GuildHouses/GuildHousesFrame.h"


#define GUILD_LOGDAY_MAX 7

//#define CREATEGUILD_COST (10*10000) //10 Gold
//#define MSG_LEDADER_CHANGE "%s ���� %s ���s�|��" 

#define DF_GETGUILDINFO_UPDATE (5*1000)

#define DF_GETGUILDINFO_COUNTER (60*2*1000)//���|��T��s���j 2����
#define DF_POST_COOLDOWN (60*2*1000)//POST�嶡�j 2����

#define DF_GUILDMEMBER_UPDATETIMER (2*60*1000)
#define DF_GUILDINFO_UPDATETIMER 300000	//(60*5*1000)




//#define INVITE_WHOTO_NEWGUILD "�A�ܽ� %s �[�J�s�p"
//#define INVITE_WHOTO_GUILD "�A�ܽ� %s �[�J���|"
//#define WAIT_WHOTO_GUILD "���� %s �^����..."
//#define YOUARE_FIRE_GUILD "�A�h�X���|"
//#define REMOVEMEMBER_GUILD "%s �h�X���|"
//#define ADDMEMBER_GUILD "%s �[�J���|"
//#define MEMBERLOGIN_GUILD "%s �W�u�F(%s)"
//#define MEMBERLOGOUT_GUILD "%s �U�u�F(%s)"
#define BBS_POST_LOG "BBS_LOG.txt"
//#define GUILD_NOTE "���i"
//#define GUILD_POST "�d��%02d"
//#define GUILD_CONTEXT "���e"
//#define POST_AUTHOR "�@��"
//#define POST_TITLE "�D�D"
//#define POST_DATE "���"

//#define OFFLINETIME_STR "%d��%d�p��..."


CGuildFrame*					g_pGuildFrame = NULL;


int LuaFunc_IsInGuild				(lua_State* L);			// �O�_�����|
int LuaFunc_GuildCreate				(lua_State* L);			// ���U���ߤ��|
int	LuaFunc_GetGuildInfo			(lua_State* L);			// ���o���a���|��T
//int LuaFunc_GuildPetitionSignature	(lua_State* L);			// �n�D���|�s�p
int LuaFunc_GuildPetitionAccept		(lua_State* L);			// �P�N���|�s�p
int LuaFunc_GuildPetitionDecline	(lua_State* L);			// �ڵ����|�s�p
int LuaFunc_GuildInviteResult		(lua_State* L);			// �^���[�J���|�ШD
int LuaFunc_GuildPetitionQuit		(lua_State* L);			// �h�X���|�s�p
int LuaFunc_GuildPetitionKick		(lua_State* L);			// �������|�s�p���a
int LuaFunc_GetNumGuildMembers		(lua_State* L);			// ���o���|�����ƶq
int LuaFunc_GetGuildRosterInfo		(lua_State* L);			// ���o���|�������

int LuaFunc_GuildInvite				(lua_State* L);			// �ܽХ[�J���|
int LuaFunc_GuildUninvite			(lua_State* L);			// ��X���|
int LuaFunc_GuildLeave				(lua_State* L);			// ���}���|

int LuaFunc_ExpandGuildGroup		(lua_State* L);			// �i�}�s��
int LuaFunc_CollapseGuildGroup		(lua_State* L);			// �����s��

int LuaFunc_SetGuildRosterSelection	(lua_State* L);			// �]�w�ܨ���H
int LuaFunc_GetGuildRosterSelection	(lua_State* L);			// ���o�ܨ���H


int LuaFunc_GF_GetRankCount			(lua_State* L);			//���o���żƶq
int LuaFunc_GF_GetRankStr			(lua_State* L);			//���o���ŦW��
int LuaFunc_GF_CanGuildWaring		(lua_State* L);			//���|�ž�
int LuaFunc_GF_SetRankStr			(lua_State* L);			//�]�w���ŦW��
int LuaFunc_GF_GetRankInfo			(lua_State* L);		    //���o�v��
int LuaFunc_GF_SetRankInfo			(lua_State* L);		    //�]�w�v��
int LuaFunc_GF_SetGuildDesc(lua_State* L);		    //�]�w���|�ۥͻP�ԭz

int LuaFunc_GF_AskLeaderChangeResult(lua_State* L);
int LuaFunc_GF_GuildInfoApply		(lua_State* L);			//�M�Τ��|��T
int LuaFunc_GF_SetGroupEnable		(lua_State* L);			//�ϥθs��
int LuaFunc_GF_GetMenuFlag 		(lua_State* L);			    //���o���
int LuaFunc_GF_GetGuildFunc 		(lua_State* L);			    
int LuaFunc_GF_LeaderChange			(lua_State* L);			// �|���h��
int LuaFunc_GF_SetMemberRank			(lua_State* L);			// �]�w�|������

int LuaFunc_GF_SetMemberCadreNote(lua_State* L);			// �]�w�|���F������
int LuaFunc_GF_GetMemberCadreNote(lua_State* L);			// ���o�|���F������

int LuaFunc_GF_SetMemberSelfNote(lua_State* L);			// �]�w�ӤH����
int LuaFunc_GF_GetMemberSelfNote(lua_State* L);			// ���o�ӤH����
int LuaFunc_GC_CloseGuildCommand(lua_State* L);			
int LuaFunc_GF_CanCreateGuildCost(lua_State* L);    //�ˬd10G
int LuaFunc_GF_SetGuildNote(lua_State* L);     //���|�i�ܪ�ui �]�w���|�i��
int LuaFunc_GF_Rename(lua_State* L);			// ���|��W
int LuaFunc_GF_GetGuildPostCount(lua_State* L);     //���|BBS ���o�ثe�d���ƶq
int LuaFunc_GF_GetGuildPostInfo(lua_State* L);     //���|BBS ���o�ثe�d����T
int LuaFunc_GF_SetGuildPostInfo(lua_State* L);      //���|BBS �]�w�ثe�d����T
int LuaFunc_GF_SetGuildPost(lua_State* L);			// �]�w�d��
int LuaFunc_GF_GetGuildPostRequest(lua_State* L);			// �ШD�d��
int LuaFunc_GF_GetBBSMenuFlag(lua_State* L);	


int LuaFunc_GF_GetPostDate(lua_State* L);			// ���o�d�����

int LuaFunc_GF_NewPost(lua_State* L);			// �s�W�d��
//int LuaFunc_GF_DeletePost(lua_State* L);			// �R���d��
int LuaFunc_GF_GetBBSInfoRequest(lua_State* L);			// �ШD�ثe�X�����


int LuaFunc_GF_GetPostCountRequest(lua_State* L);			// �ШD�ثe�X�����




int Luafunc_GuildBoard_GetNextGuildIDName  (lua_State* L); //���|�i�ܪ�ui ���o���|�W�٤�ID  
int LuaFunc_GuildBoard_RequestGuildInfo(lua_State* L);     //���|�i�ܪ�ui ���o���|��T  
//�VServer Ū���Ҧ����
int LuaFunc_GF_ReadAllDate(lua_State* L);



//Ū��BAG�����~
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

// ���|�ž�
int LuaFunc_GetGuildWarItemInfo(lua_State* L);
int LuaFunc_DeclareGuildWar(lua_State* L);
int LuaFunc_GetGuildWarInfo(lua_State* L);
int LuaFunc_GetGuildWarOutdoorsPoint(lua_State* L);				// ���o���~�žԾ��Z
int LuaFunc_GetGuildWarOutdoorsDeclareInfo(lua_State* L);		// ���o�ۤv���|�žԸ�T	
int LuaFunc_GetGuildWarOutdoorsNumEnemies(lua_State* L);		// ���o�Ĺ綠�|�žԭӼ�
int LuaFunc_GetGuildWarOutdoorsEnemyInfo(lua_State* L);			// ���o�Ĺ綠�|�žԸ�T	

int LuaFunc_GetGuildWarPoint(lua_State* L);				// ���o���|�ΫžԾ��Z


//���|�X
int LuaFunc_GetGuildFlagInsigniaCount(lua_State* L);
int LuaFunc_GuildFlagBannerCount     (lua_State* L);
int LuaFunc_GetGuildFlagInsigniaType (lua_State* L);
int LuaFunc_GuildFlagBannerType      (lua_State* L);


int LuaFunc_GuildFlagInfo            (lua_State* L);

int LuaFunc_SetGuildFlagInfoRequest  (lua_State* L);

int  LuaFunc_GetSelfGuildRank		(lua_State* L);


struct CachGuildMember
{
	GuildMemberStruct m_info;		//�Բ�

	CachGuildMember()
	{
		m_info.LogoutTime=-1;
	}
	int m_updateTime;				//�O����Ƨ�s
};


bool  CheckStr(const char *guildName,int len=24);

typedef vector<GuildMemberBaseInfoStruct> simpleGuildMemberList;
typedef vector<CachGuildMember> SinfoGuildMemberList;
//�W�ٿ��~�T��

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

//����²�����|�W��
struct _Guild_data
{
	char m_name[_MAX_NAMERECORD_SIZE_];
	int m_lev;//0 �s�p��,1:�إ�
	GuildBaseStruct m_info;//���|���򥻸��
	int m_MemberCount;//�����ƶq
	int m_updateTime; //��s�ɶ�
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
//�����ϥΪ����|���
class Guild_internal
{
public:

	Guild_internal(CInterface*	ui):m_interface(ui),m_UserGroup(false),m_RosterSelection(-1),m_RosterSelectionDBID(-1),m_day(0)
	{
		memset (m_GroupIsExpand,0,sizeof(m_GroupIsExpand));
		m_bLeaderChange=false;
	}

	bool isChangeDay(void);
	// ���o���|ID
	int GetGuildIDByName(const char* name);

	//���|�����W�U�u
	void GuildPlayerOnline		( int PlayerDBID , bool IsOnline );

	//��s���
	void UpDateNewMember(GuildMemberStruct& member,bool newMember);


	//�������ԲӸ��
	void GuildMemberInfo( GuildMemberStruct& Member );
	//�𰣦���
	void  RemoveMemberByKick(int LeaderDBID,int memid);

	//�[�J���|
	void JoinGuild			( int GuildID , char* LeaderName );
	//�[�J�s�p���|
	void JoinNewGuild			( int GuildID , char* LeaderName );

	//�������|�ܽ�
	void GuildInviteResult(char* PlayerName , int	Result );
	
	//�]�w�ϥβոs
	void SetUserGroup(bool bGroup);
// �n�D���s���o�ۤv���|��T
	void ReloadGuildDate(void);
	//���o���|�򥻸��
	void GuildBoardRequest( int guildID ,bool& v );

	//void GuildInfoChange(GuildInfoChangeStruct& ChangeMode);

	void Clear(void);

	//���줽�|���|���C��
	void GuildMemberBaseInfo( vector< GuildMemberBaseInfoStruct >& MemberList);

	//���}���|.�@�� �@
	void LeaveGuildResult	( char* KickPlayerName , bool Result );
	//���}�s�p���|.�@�� �@
	void LeaveNewGuildResult	( char* KickPlayerName , bool Result );
	//���o���|�W��
	const char *GetGuildNameByID(int GuildID);

	//���ܤ��|�G�i��
	void SendEvent_GuildBoardResult( _Guild_data* data );
	//�o��Y���|���򥻸��
	void  GuildInfo(GuildBaseStruct& guild,int memberCount,bool bNeedSend);

	//���줽�|�귽�ܰ�
	void ModifySimpleGuild( SimpleGuildInfoStruct& SimpleGuildInfo );

	//���o�ۤv���|���򥻸��
	_Guild_data* GetSelfGuildData(void);
	//���o�Y���|���򥻸��
	_Guild_data* GetGuildData(int GuildID);

	// ����Y���|��W���G
	void ChangeGuildNameResult		( int Result );

// ��L�H����Y���|��W
	void ChangeGuildName( int GuildID , char* GuildName );
	//�Y���|�򥻸�ơ@���@���|�C��
	void GuildBaseInfo( int GuildID , char* GuildName , GuildFlagStruct& Flag, bool IsReady );
	//�Y���|�ˤF
	void GuildDestroy			( int GuildID  );

	//�����|��
	void RemoveMember(char *MemberNAME);
	//�����|��
	void RemoveMemberByID(int dbid);
	//�����|���ԲӸ��
	void RemoveSInfoMember(int dbid);

	ContributionLogDay& GetGuildResLogTable(int day);
	//���}��W��ܮ�
	void OpenRenameDialog();

	//���y�|���C��
	void UpdateMemberInfo();

	//���
	void SetGuildRosterSelection(int index);
	//�Vserver �Q�|���ԲӸ��
	void ReloadGuildMemberDate(int dbid);

	//����
	CachGuildMember * GetMemberInfo(int dbid);

	//��򥻸��
	GuildMemberBaseInfoStruct * GetMemberBaseInfo(int dbid);
	
	//��򥻸��
	GuildMemberBaseInfoStruct *GetSelfBaseInfo(void);
	//���o�Q��ܪ����
	GuildMemberBaseInfoStruct* GetSelectBaseInfoStruct(void);

	//���o�ۤv�����
	CachGuildMember*		GetSelfInfo(void);

	// ���a���}�ۤv�����|
	void LeaveGuildMember		( int LeaderDBID , int MemberDBID );


	

	//���|��W
	 void GuildRename(const char * GuildName);

	//�[�J�s����
	void JoinGuildMember( GuildMemberStruct& Member );

	//���o���|�����C��ƶq
	int GetNumGuildMembers(void);

	//���o���ު�
	GuildMemberListInfo *GetGuildMemberInfo(int index);
	//�]�w�s�ն}��
	void SetGuildHeaderState(int index, bool isExpand);

	//������?
	int GetGuildRosterSelection(void );


	bool SetLogDay(int index);

	void SetDayLogTable(int count,ContributionInfoLogStruct* lists);

	//�Хߤ��|���^��
	void CreateGuildResult( int GuildID , char* GuildName , int Result );
	//�s�p���|���ܽ�
	void NewGuildInvite( int GuildID, char* LeaderName, int LeaderDBID );

	//�q�X�T�������}�F
	void				ShowMemberLeave(const char *name);  
	//UI 
	CInterface*					m_interface;

	simpleGuildMap& GetGuildMap(void){return m_AllGuildMap;}

//���|���
	simpleGuildMemberList  m_baseMemberList; //²��

	SinfoGuildMemberList			m_MemberList;	//�Բ�
	vector<GuildMemberListInfo>		m_GuildMemberInfo;//�|���C�� �s�ո�� �ثe�L��.
	bool							m_GroupIsExpand[_MAX_GUILD_GROUP_COUNT_];
	int								m_RosterSelection;//�O�Y����H.
	int								m_RosterSelectionDBID;//����.
	bool							m_UserGroup;//�ϥθs��.


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

	//�`���|��

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
//�������|�ܽ�
void Guild_internal::GuildInviteResult(char* PlayerName , int	Result )
{  
	char temp[MAX_PATH]="";
	const char *pStrMsg=g_ObjectData->GetString( "GUILD_MSG_ADDMEMBER_FAILL");
	switch( Result )
	{
	case EM_GuildInviteResult_OK			:	//�n�Ц�
		pStrMsg=g_ObjectData->GetString( "GUILD_MSG_ADDMEMBER_GUILD");
		//sprintf(temp,"%s �[�J���|" , PlayerName);
		//g_pDebugMessage->DebugOutput( 2 , "%s �[�J���|" , PlayerName );
		break;

	case EM_GuildInviteResult_NotFind	:	//�䤣�� �Q�ܽЪ�
		pStrMsg=g_ObjectData->GetString( "GUILD_MSG_ADDMEMBER_FAIL_MISS");
		//sprintf(temp,"%s �[�J���|���� (�䤣�� �Q�ܽЪ�)" , PlayerName );
		//g_pDebugMessage->DebugOutput( 2 , "%s �[�J���|���� (�䤣�� �Q�ܽЪ�)" , PlayerName );
		break;
	case EM_GuildInviteResult_HasGuild	:	//�Q�ܽЪ� �w�g�����|
		pStrMsg=g_ObjectData->GetString( "GUILD_MSG_ADDMEMBER_FAIL_HAVE");
		//sprintf(temp,"%s �[�J���|���� (�Q�ܽЪ� �w�g�����|)" , PlayerName);
		//g_pDebugMessage->DebugOutput( 2 , "%s �[�J���|���� (�Q�ܽЪ� �w�g�����|)" , PlayerName );
		break;
	case EM_GuildInviteResult_Disagree	:	//�Q�ܽЪ� ���P�N
		pStrMsg=g_ObjectData->GetString( "GUILD_MSG_ADDMEMBER_NOTAGREE");
		//sprintf(temp,"%s �[�J���|���� (�Q�ܽЪ� ���P�N)" , PlayerName);
		//g_pDebugMessage->DebugOutput( 2 , "%s �[�J���|���� (�Q�ܽЪ� ���P�N)" , PlayerName );
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
//���}��W��ܮ�
void Guild_internal::OpenRenameDialog()
{

	lua_State* L = g_pGuildFrame->GetLuaState();
	if (L)
	{
		_Guild_data* date=GetSelfGuildData();

			const char *name="[**********]";
		if (date)
			name=date->m_info.GuildName.Begin();
		

		//���|�W��
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


// �^�����|��W
void Guild_internal::ChangeGuildNameResult( int Result )
{

	char temp[MAX_PATH]="";
	const char *pStrMsg=g_ObjectData->GetString( "SYS_GUILD_RENAME_FLAGERR");
	switch (Result)
	{
	case EM_ChangeGuildNameResult_OK:		 // ��WOK
		{
			pStrMsg=g_ObjectData->GetString( "SYS_GUILD_RENAME_OK");
			_Guild_data* _date=GetSelfGuildData();
			if (_date)
			_date->m_info.IsNeedChangeName=false;
		}

		break;
	case EM_ChangeGuildNameResult_NameUsed:	 //���|�W�r����
		pStrMsg=g_ObjectData->GetString( "SYS_GUILD_RENAME_USED");
		OpenRenameDialog();
		break;
	case EM_ChangeGuildNameResult_FlagError: //�S���}�ҧ�W
		pStrMsg=g_ObjectData->GetString( "SYS_GUILD_RENAME_FLAGERR");
		break;
	case EM_ChangeGuildNameResult_NotLeader: //���O�|��
		pStrMsg=g_ObjectData->GetString( "SYS_GUILD_RENAME_NOTLEADER");
		break;



	case EM_ChangeGuildNameResult_NameToShort	:		// �W�٤ӵu,���X�k
	case EM_ChangeGuildNameResult_NameToLong		:		// �W�٤Ӫ�,���X�k
	case EM_ChangeGuildNameResult_NameIsWrongful	:		// �����~���Ÿ��Φr��
	case EM_ChangeGuildNameResult_HasSpecialString:		// �S��W�٤����
	case EM_ChangeGuildNameResult_OnlyBasicLetters:		// �u��ϥΰ򥻭^��r��
	case EM_ChangeGuildNameResult_EndLetterError	:		// �����r�����~(',")
		pStrMsg=GetNameCheckErr(Result-EM_ChangeGuildNameResult_NameToShort);
		break;


	case EM_ChangeGuildNameResult_UnknowErr:

		pStrMsg=g_ObjectData->GetString( "SYS_CREATEROLE_UNKNOW");
		break;

	}

	strcpy(temp,pStrMsg );
	g_pChatFrame->SendMsgEvent("CHAT_MSG_SYSTEM", temp, "");
}

//���|�����W�U�u
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

// ���a���}�ۤv�����|
void Guild_internal::LeaveGuildMember		( int LeaderDBID , int MemberDBID )
{
	RemoveMemberByKick(LeaderDBID,MemberDBID);

}


//�[�J�s����
void Guild_internal::JoinGuildMember( GuildMemberStruct& Member )
{
	UpDateNewMember(Member,true);
}

//��s���
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
	temp.	Rank		= member.	Rank;				//���⤽�|����
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

//�������ԲӸ��
void Guild_internal::GuildMemberInfo( GuildMemberStruct& Member )
{
	_Guild_data* date=GetSelfGuildData();
// �P�O�O�_���|����
	if (date&&Member.GuildID==date->m_info.GuildID)
	{
		
		UpDateNewMember(Member,false);

	}
	
	g_pGuildFrame->AddMemberInfo(Member.MemberDBID);

}
//�[�J���|
void Guild_internal::JoinGuild			( int GuildID , char* LeaderName )
{
	char tmp[MAX_PATH];
	const char *pStr=g_ObjectData->GetString("GUILD_MSG_JOINGUILD");

	sprintf_s(tmp, MAX_PATH,pStr,   LeaderName,GetGuildNameByID(GuildID));
	g_pChatFrame->SendMsgEvent("CHAT_MSG_SYSTEM", tmp, "");

	//g_pDebugMessage->DebugOutput( 2 , "�[�J���| %s LeaderName=%s" , GetGuildNameByID(GuildID) , LeaderName );
	CNetGlobal::RoleData()->BaseData.GuildID = GuildID;

	// �n�D���s���o�ۤv���|��T
	ReloadGuildDate();

}
//�[�J�s�p���|
void Guild_internal::JoinNewGuild( int GuildID , char* LeaderName )
{
	char tmp[MAX_PATH];
	const char *pStr=g_ObjectData->GetString("GUILD_MSG_INVATE_NEW");
	sprintf_s(tmp,MAX_PATH, pStr , LeaderName, GetGuildNameByID(GuildID ));
	g_pChatFrame->SendMsgEvent("CHAT_MSG_SYSTEM", tmp, "");




	CNetGlobal::RoleData()->BaseData.GuildID = GuildID;

	// �n�D���s���o�ۤv���|��T
	ReloadGuildDate();

}

//�]�w�ϥβոs

void Guild_internal::SetUserGroup(bool bGroup)
{
	m_UserGroup=bGroup;
}

//���o�ۤv���|���򥻸��
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


//��򥻸��
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

//�q�X�T�������}�F
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

//��ۤw�򥻸��
GuildMemberBaseInfoStruct *Guild_internal::GetSelfBaseInfo(void)
{
	int selfID=g_pGameMain->GetDBID();	
	return GetMemberBaseInfo(selfID);	
}

//���o�Q��ܪ����
GuildMemberBaseInfoStruct* Guild_internal::GetSelectBaseInfoStruct(void)
{	
	return GetMemberBaseInfo(m_RosterSelectionDBID);
}
void Guild_internal::RemoveMemberByKick(int LeaderDBID,int MemberDBID)
{
	if (MemberDBID==g_pGameMain->GetDBID())
	{//�ڳQ��F
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

// �n�D���s���o�ۤv���|��T
void Guild_internal::ReloadGuildDate(void)
{	
	NetCli_GuildChild::SL_GuildInfoRequet(CNetGlobal::RoleData()->BaseData.GuildID);
	NetCli_GuildChild::SL_GuildMemberListRequet(CNetGlobal::RoleData()->BaseData.GuildID);
}

//���o���|�W��
const char *Guild_internal::GetGuildNameByID(int GuildID)
{
	const char * name=NULL;
	_Guild_data*date=GetGuildData(GuildID);
	if (date)
		name=date->m_name;
	
	return name;


}
//���줽�|���|���C��
void Guild_internal::GuildMemberBaseInfo( vector< GuildMemberBaseInfoStruct >& MemberList)
{
	// �P�O�O�_�ۤv���|
	if ( NetCli_Guild::_GuildID == CNetGlobal::RoleData()->BaseData.GuildID )
	{	
		m_baseMemberList = MemberList;

		// ������Ƨ�s,�ݭn��s��ܬɭ�
		UpdateMemberInfo();
	}
}

//���o���|�򥻸��
void Guild_internal::GuildBoardRequest( int guildID,bool& v )
{
	_Guild_data* date= GetGuildData (guildID);
	if (date)
	{
		int nowTime = timeGetTime();
		if ( ( nowTime - date->m_updateTime ) > DF_GUILDINFO_UPDATETIMER )
		{
									
			v=true;//������¤F�@�Vserver�Q
			 NetCli_Guild::SL_GuildInfoRequet(guildID);
			date->m_updateTime = timeGetTime();
			

		}
		else
		{
			SendEvent_GuildBoardResult( date );

		}
	}	
}



//���}�s�p���|.�@�� �@
void Guild_internal::LeaveNewGuildResult	( char* KickPlayerName , bool Result )
{
	char tmp[MAX_PATH];
	if( Result )
	{	
		const char* selfName = CNetGlobal::RoleData()->BaseData.RoleName.Begin();		
		if (strcmp(selfName,KickPlayerName)==0) 	/*�ۤv���}���|.*/
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
		else /*kick ��L����.*/
		{
			//�n�D�s���W��
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

//�s�p���|���ܽ�
void Guild_internal::NewGuildInvite( int GuildID, char* LeaderName, int LeaderDBID )
{
	char tmp[MAX_PATH];
	//sprintf(tmp, "�ܽХ[�J�s�p���|(%d) �n�ФH(%s) DBID(%d)", GuildID, LeaderName, LeaderDBID);

	const char * pStr=g_ObjectData->GetString("ASK_INVITE_TO_GUILD");

	sprintf_s(tmp,MAX_PATH,pStr, LeaderName, GetGuildNameByID(GuildID));
	g_pChatFrame->SendMsgEvent("CHAT_MSG_SYSTEM", tmp, "");

	g_pGuildFrame->PetitionSignature(GuildID, LeaderName, LeaderDBID);
}


//�Хߤ��|���^��
void Guild_internal::CreateGuildResult( int GuildID , char* GuildName , int Result )
{
	char tmp[MAX_PATH];
	const char * pStr=g_ObjectData->GetString("GUILD_NEWGUILD");
	const char * strResult=g_ObjectData->GetString("FAIL");
	switch( Result )
	{
	case EM_CreateGuildResult_OK:			//��?

		strResult=g_ObjectData->GetString("SUCCEEDED");
		CNetGlobal::RoleData()->BaseData.GuildID = GuildID;


		// �n�D���s���o�ۤv���|��T
		ReloadGuildDate();

		break;
	case EM_CreateGuildResult_Failed_Exist:				//�w�����|
		strResult=g_ObjectData->GetString("GUILD_HAVEDGUILD");
		break;
	case EM_CreateGuildResult_Failed_Name:				//���|�W�٭��_or �r�꦳���D
		strResult=g_ObjectData->GetString("GUILD_ERROR_MSG");
		break;

	case EM_CreateGuildResult_Failed_Money:				//��������

		strResult=g_ObjectData->GetString("GUILD_ERROR_MSG1");
		break;	
	case EM_CreateGuildResult_NameToShort:				// �W�٤ӵu,���X�k
			
	case EM_CreateGuildResult_NameToLong:				// �W�٤Ӫ�,���X�k
	
	case EM_CreateGuildResult_NameIsWrongful:			// �����~���Ÿ��Φr��
		
	case EM_CreateGuildResult_HasSpecialString:			// �S��W�٤����
	
	case EM_CreateGuildResult_OnlyBasicLetters:			// �u��ϥΰ򥻭^��r��
		
	case EM_CreateGuildResult_EndLetterError:			// �����r�����~(',")


		strResult=GetNameCheckErr(Result-EM_CreateGuildResult_NameToShort);
		break;


	}

	sprintf_s(tmp,MAX_PATH, "%s %s %s", pStr, GuildName,strResult);
	g_pChatFrame->SendMsgEvent("CHAT_MSG_SYSTEM", tmp, "");



	g_pGuildFrame->CloseGuildCommand();


}

//-----------------------------------------------------------------------------
////������?
int Guild_internal::GetGuildRosterSelection(void)
{
	return m_RosterSelection;
}

//-----------------------------------------------------------------------------
//�]�w�s�ն}��
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
//���o���ު�
GuildMemberListInfo * Guild_internal::GetGuildMemberInfo(int index)
{
	GuildMemberListInfo *ret=NULL;
	if (index>=0&&index <(int )m_GuildMemberInfo.size())
		ret=&m_GuildMemberInfo[index];

	return ret;


}


//-----------------------------------------------------------------------------
//���o���|��ܪ��ƶq
int Guild_internal::GetNumGuildMembers(void)
{
	int count=0;
	int GuildID=CNetGlobal::RoleData()->BaseData.GuildID;
	
	if (GuildID>0)
		count=(int)m_GuildMemberInfo.size();

	return count;//
}

//-----------------------------------------------------------------------------
//���o�ۤv�����
CachGuildMember * Guild_internal::GetSelfInfo()
{
	int selfID=g_pGameMain->GetDBID();	
	return GetMemberInfo(selfID);
}

//��|���ԲӸ��
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

//�Vserver �Q�|���ԲӸ��
void  Guild_internal::ReloadGuildMemberDate(int dbid)
{

	CachGuildMember *ret=GetMemberInfo(dbid);

	bool reload=false;
	if (ret)
	{
		//�ˬd�ɶ��L���F
		int time=timeGetTime();
		if ((time-ret->m_updateTime)>DF_GUILDMEMBER_UPDATETIMER)
			reload=	false;		
	}
	if (!ret||reload)
		NetCli_GuildChild::SL_GuildMemberInfoRequet(dbid);
}

//ui�@Ĳ�o���
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
//���y�|���C��
void Guild_internal::UpdateMemberInfo()
{
	int GuildID=CNetGlobal::RoleData()->BaseData.GuildID;
	m_GuildMemberInfo.clear();
	m_RosterSelection = -1;

	if ( GuildID != 0 && !m_baseMemberList.empty() )
	{
		_Guild_data* date=GetGuildData(GuildID);
		GuildMemberListInfo info;
		if ( date&& date->m_info.IsReady&& m_UserGroup)			// �������ߤ��|����
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
							// �������
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
				// �������
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
//�����|��
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

//�����|��
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

//������}���| �@�ϥν�H���O�̷|����
void Guild_internal::LeaveGuildResult	( char* KickPlayerName , bool Result )
{
	char tmp[MAX_PATH];
	// �������ۤv���}��Q�𰣤��|
	if( Result )//��?
	{
		const char* selfName = CNetGlobal::RoleData()->BaseData.RoleName.Begin();		 
		if (strcmp(selfName,KickPlayerName)==0) //�۽�
		{
			
			g_pGuildFrame->SelfLeaveGuid();//�I�s�ۤv���}���|
			CNetGlobal::RoleData()->BaseData.GuildID = 0;//�M�����|ID
			
		}
		else /*kick ��L����.*/
		{
			//�n�D�s���W��
			//SL_GuildMemberListRequet(_RoleData.BaseData.GuildID);

			//sprintf(tmp, "%s ���}���|" , KickPlayerName);
			//	g_pChatFrame->SendMsgEvent(CHAT_MSG_GUILD, tmp, "");

			//ShowMemberLeave(KickPlayerName);
			RemoveMember(KickPlayerName);			
		}

	}
	else//���ѡ@�𰣬Y�H����
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


	//���|����
	lua_pushnumber( L, data->m_info.Level );
	lua_setglobal( L, "GuildLevel" );

	//���|�W��
	lua_pushstring( L, data->m_info.GuildName.Begin() );
	lua_setglobal( L, "GuildName" );

	//�|���W��
	lua_pushstring( L, data->m_info.LeaderName.Begin() );
	lua_setglobal( L, "LeaderName" );

	//�{���H��
	lua_pushnumber( L, data->m_MemberCount );
	lua_setglobal( L, "MemberCount" );

	//�̦h�i�H���h�ַ|��
	lua_pushnumber( L, data->m_info.MaxMemberCount );
	lua_setglobal( L, "MaxMemberCount" );

	//�O�_�ۥ�
	lua_pushboolean( L, data->m_info.IsRecruit );
	lua_setglobal( L, "IsRecruit" );

	//���|���I
	//lua_pushboolean( L, data->info.IsRecruit );
	//lua_setglobal( L, "GuildFootHold" );

	//���|��
	lua_pushboolean( L, data->m_info.IsOwnHouse );
	lua_setglobal( L, "IsOwnHouse" );

 	//�}����[
	lua_pushboolean( L, !data->m_info.IsLockGuildHouse );
	lua_setglobal( L, "IsOpenVisit" );

	//�d����
	lua_pushboolean( L,true/* data->info.IsEnabledGuildBoard */);
	lua_setglobal( L, "isBBSEnable" );

	//���|�ԭz
	lua_pushstring( L, data->m_info.Introduce.SafeStr() );
	lua_setglobal( L, "Introduce" );

	//int		GuildID;									//���|ID
	//StaticString< _MAX_NAMERECORD_SIZE_ >   GuildName;	//���|�W��
	//StaticString< _MAX_NAMERECORD_SIZE_ >   LeaderName;	//�|���W��
	//int		LeaderDBID;
	//bool	IsRecruit;									//�O�_�ۥ�
	//bool	IsReady;									//�O�_��������
	//bool	IsNeedSave;									//�p�G��Ʀ���ʫh�|�� true
	//int		Medal;										//����
	//int		MaxMemberCount;								//�̦h�i�H���h�ַ|��
	//int		Score;										//���|�n��
	//StaticString< 512 >   Note;							//���i
	//StaticString< 512 >   Introduce;					//���|����

	//int		RankCount;									//���|���h��
	//GuildRankBaseInfoStruct	Rank[ EM_GuildRank_Count ];						//�C�ӵ��Ū��v�� �P�W��
	//StaticString< _MAX_NAMERECORD_SIZE_ >   Group[_MAX_GUILD_GROUP_COUNT_];	//�s�զW��


	m_interface->GetUiWorld()->SendEvent( UiEvent_GuildBoardResult );

}


// ����Y���|��W
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

//�Y���|�򥻸�ơ@���@���|�C��
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

	// ���]�Ҧ����a�X�l����
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
//�Y���|�ˤF
void Guild_internal::GuildDestroy( int GuildID  )
{
	simpleGuildMap::iterator it=m_AllGuildMap.find(GuildID);
	if (it!=m_AllGuildMap.end())	
		m_AllGuildMap.erase(it);
	
}

//���o�Y���|���򥻸��
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
//GuildResourceStruct Resource;			//�귽
//};
//���줽�|�귽�ܰ�
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
//�o��Y���|���򥻸��
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
	

	//�p�G�O�ڪ����| ��s�ۤv��
	if (SelfGuildID>0&&guild.GuildID == SelfGuildID)
	{		

		GuildBaseStruct& myGuild=ret->m_info;
		//�ڪ����|��T
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









		// ��Ƨ�s,�ݭn��s��ܬɭ�
		g_pGuildFrame->UpDateGuildInfo(ret->m_info);
		UpdateMemberInfo();

	}

	

/*
	
	if ( Guild.GuildID == SelfGuildID)
	{
		bool isInitial = false;

		// ��l��
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

		lua_register(L, "GF_Rename",		LuaFunc_GF_Rename);// ���|��W

		//���|�d����
		lua_register(L, "GF_SetGuildNote",		LuaFunc_GF_SetGuildNote);//ui �]�w���|�i��
		lua_register(L, "GF_GetGuildPostCount",		LuaFunc_GF_GetGuildPostCount); //���|BBS ���o�ثe�d���ƶq
		lua_register(L, "GF_GetGuildPostInfo",		LuaFunc_GF_GetGuildPostInfo);//���|BBS ���o�ثe�d����T
		lua_register(L, "GF_SetGuildPostInfo",		LuaFunc_GF_SetGuildPostInfo);//���|BBS �]�w�ثe�d����T
		lua_register(L, "GF_SetGuildPost",		LuaFunc_GF_SetGuildPost);// �]�w�d��
		lua_register(L, "GF_GetGuildPostRequest",		LuaFunc_GF_GetGuildPostRequest);// ���o�d���ШD
		lua_register(L, "GF_GetPostDate",		LuaFunc_GF_GetPostDate);// ���o�d���ШD

		
		lua_register(L, "GF_NewPost",		LuaFunc_GF_NewPost);// �s�W�d��
		//lua_register(L, "GF_DeletePost",		LuaFunc_GF_DeletePost);// �R���d��
		lua_register(L, "GF_GetBBSInfoRequest",		LuaFunc_GF_GetBBSInfoRequest);// �ثeBBSINFO�ݨD


		lua_register(L, "GF_GetPostCountRequest",		LuaFunc_GF_GetPostCountRequest);
		
		lua_register(L, "GF_GetBBSMenuFlag",		LuaFunc_GF_GetBBSMenuFlag);// ���o�d�����
		
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
//���}���|�R�O����
void          CGuildFrame:: OpenGuildCommand(int iObjID)
{
#ifdef _DEBUG
iObjID=-1;
#endif
	m_npcId=iObjID;

	
	CRoleSprite * npcSprite= g_pGameMain->FindSpriteByID( iObjID );
//	GameObjDbStruct* npcDB = CNetGlobal::GetObj( iObjID );

	if (npcSprite==NULL) 
			g_pDebugMessage->DebugOutput( 2 , "�����Ʈw�L��NPC %d", iObjID );
	if ( npcSprite  || iObjID==-1)
	{
		// �}�Ҥ��|�ɭ�
		SendWorldEvent("GUILD_COMMAND_OPEN");
		g_pDebugMessage->DebugOutput( 2 , "�}�Ҥ��|�ɭ� NPC�s�� %d", iObjID );
		m_Open=true;
	}
}

bool			CGuildFrame::IsOpen()
{

	return m_Open;

}

void CGuildFrame::Update(float elapsedTime)
{
	// ��s���|��Ը�T
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
//�������|�R�O����
void          CGuildFrame::CloseGuildCommand()
{
	NetCli_Guild::SL_CloseCreateGuild( );

	// �������|�ɭ�
	SendWorldEvent("GUILD_COMMAND_CLOSE");
	m_npcId=-1;
	m_Open=false;
}
// ----------------------------------------------------------------------------------
//�q��UI ���|���U
void CGuildFrame::RegisterGuild()
{
	// �ˬd���|�O�_�w���ߩΪ̤w�g�[�J���|
	if ( CNetGlobal::RoleData()->BaseData.GuildID != 0 )
		return;

	// ���X�T������
	lua_State* L = GetLuaState();
	if ( L )
	{
		OpenGuildCommand(-1);
	//	SendWorldEvent("GUILD_REGISTER");			// �ӽФ��|
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
//�q��UI �|����Ʀ���s
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
//���o�ۤv�����|���
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

			// �n�D���s���o�ۤv���|��T
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
		g_pDebugMessage->DebugOutput( 2 , "�����Ʈw�L��NPC %d", iObjID );
	if ( npcSprite  || iObjID==-1)
	{
		// �n�D���s���o�ۤv���|��T
		g_internal->ReloadGuildDate();
		// �}�Ҥ��|�ө�
		SendWorldEvent("GUILD_SHOP_OPEN");
		g_pDebugMessage->DebugOutput( 2 , "�}�Ҥ��|�ө� NPC�s�� %d", iObjID );
		m_Open=true;
	}

}
void	 CGuildFrame::CloseGuildShop()
{
//	NetCli_Guild::SL_GuildShopClose( );

	// �������|�ɭ�
	SendWorldEvent("GUILD_SHOP_CLOSE");
	m_npcId=-1;
	m_Open=false;

}


// ----------------------------------------------------------------------------------
//���o�ۤv��²�����

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
//�إߤ��|

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
	// �ˬd���|�O�_�w���ߩΪ̤w�g�[�J���|
	if ( CNetGlobal::RoleData()->BaseData.GuildID != 0 )
	{
		g_pDebugMessage->DebugOutput( 2 , "�A�ۤv�w���u�|%s",NetCli_GuildChild::GetGuildNameByID(CNetGlobal::RoleData()->BaseData.GuildID));
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
			//�Y����޸�
		

	}
	temp[j]=0;
	*/

	char temp[256];
	strcpy(temp,guildName);
		//CheckSqlStr(temp);
	if (CheckStr(temp)==true)//�令 UTF8 ���A��CheckSqlStr�ˬd 
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
//--�W�Ǥ��|��T
void CGuildFrame::GuildInfoApply(void)
{
	{//�v���ˬd
		GuildRankBaseInfoStruct* ri=GetUserRankInfo();
		// �ˬd�O�_���v��
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
//�q���|�����H
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
//�s�p�ܽ�
void CGuildFrame::PetitionSignature(int guildID, const char* leaderName, int leaderDBID)
{
	if ( m_guildID != 0 )
	{		
		if ( m_guildID != guildID )
			NetCli_Guild::SL_NewGuildInviteResult(guildID, leaderDBID, false);		// �w�Q�ܽФ�,�����ڵ��ܽ�

		// �����ܽФ��B�z
		return;
	}

	m_guildID		= guildID;
	m_leaderDBID	= leaderDBID;
	m_leaderName	= leaderName;

	// ���X�T������
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
//���|�����W�U�u
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

	

	//���|�W�D����MSG
	char temp[512];
	const char *pMsg;
	if(IsOnline)
		pMsg=g_ObjectData->GetString("GUILD_MSG_MEMBERLOGIN_GUILD");
	else
		pMsg=g_ObjectData->GetString("GUILD_MSG_MEMBERLOGOUT_GUILD");

	

	sprintf_s(temp,512,pMsg,name,temp1);

	g_pChatFrame->SendMsgEvent(CHAT_MSG_GUILD, temp,"" );

	//��s�C���A
	UpdateGuild();
}



// ----------------------------------------------------------------------------------
//�[�J���|�ܽ�
void CGuildFrame::GuildInviteRequest(int guildID, const char* leaderName, int leaderDBID)
{
	if ( m_guildID != 0 )
	{		
		if ( m_guildID != guildID )
			NetCli_Guild::SL_GuildInviteResult(guildID, leaderDBID, false);		// �w�Q�ܽФ�,�����ڵ��ܽ�

		// �����ܽФ��B�z
		return;
	}

	m_guildID		= guildID;
	m_leaderDBID	= leaderDBID;
	m_leaderName	= leaderName;

	// ���X�T������
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
//�^���s�p
void CGuildFrame::PetitionReply(bool agree)
{
	if ( m_guildID == 0 )								// �S���Q�ܽйL,�^���L��
		return;

	// �^���ܽгs�p
	NetCli_Guild::SL_NewGuildInviteResult(m_guildID, m_leaderDBID, agree);

	m_guildID		= 0;
	m_leaderDBID	= 0;
	m_leaderName.clear();
}

//�^������
void CGuildFrame::GuildInviteReply(bool agree)
{
	if ( m_guildID == 0 )								// �S���Q�ܽйL,�^���L��
		return;

	// �^�������ܽ�
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
//�����s�p
void CGuildFrame::PetitionQuit()
{
	// �ˬd�O�_�����ߤ����|
	if ( CNetGlobal::RoleData()->BaseData.GuildID == 0 )
		return;

	m_guildID=0;

	NetCli_Guild::SL_DelNewGuild();
}
//----------------------------------
//�q��UI ��s���|��T
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
//�|�����
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
// ��L�H����Y���|��W
void CGuildFrame::ChangeGuildName( int GuildID , char* GuildName  )
{
	g_internal->ChangeGuildName(GuildID,GuildName);

}


void CGuildFrame::GuildBaseInfo( int GuildID , char* GuildName , GuildFlagStruct& Flag , bool IsReady )
{
	g_internal->GuildBaseInfo(GuildID,GuildName,Flag,IsReady);

}

// �Y���|�ˤF�@���@�C��
void CGuildFrame::GuildDestroy			( int GuildID  )
{
	g_internal->GuildDestroy(GuildID);

}

//-------------------------------------
//�}�l ������ƻݨD
void CGuildFrame::BeginMemberInfoRequest(void)
{
	m_waitEvent.m_member.clear();
	m_waitEvent.m_eventFunc=0;
	

}
//-------------------------------------
//�s�W ������ƻݨD
void CGuildFrame::AddMemberInfoRequest(int dbid)
{
	GuildMemberStruct* temp= GetGuildMemberStruct(dbid);		
	if (temp==0)
		m_waitEvent.m_member.push_back(dbid);

}
//-------------------------------------
//������s�������
//�ˬd�C�����N ����
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
//���Ƥ��|�ԲӸ�ƽШD����
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
//�|������ШD
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
//�]�w���ŦW��
void CGuildFrame::SetRankStr(int index,const char *name)
{

	GuildRankBaseInfoStruct* ri=GetUserRankInfo();
	// �ˬd�O�_���v��
	if (!ri->Setting.AdjustRank)
		return ;

	char tempBuff[512];
	strncpy(tempBuff,name,31);

	tempBuff[31]=0;		
	
	size_t destLen=31;

	m_tempGuildInfo.Rank[index].RankName=ItemUTIL::checkUtf8SafeString(tempBuff,destLen,NULL);



}
// ----------------------------------------------------------------------------------
//�s�p���h
void CGuildFrame::PetitionKick(const char* name)
{
	// �ˬd�O�_�����ߤ����|
	if ( CNetGlobal::RoleData()->BaseData.GuildID == 0 )
		return;

	NetCli_Guild::SL_LeaveNewGuild((char*)name);		// �𱼤��|
}
//--------------------------���|�������� 
//�]���O
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
//�ܽ�
void CGuildFrame::Invite(const char* name)
{

	//�Գ���
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
		// �S�����|
		return;
	}
	GuildBaseStruct& guildInfo = date->m_info;
	if ( name == NULL || strlen(name) < 1 )
	{
		// �ݭn���ܽй�H
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
		// �A �� �ܽЦۤv
		SendWorldEvent("GUILDINVITE_SELF");

		return;
	}
	
	// �������|
	if ( guildInfo.IsReady )
	{
		// �ˬd�O�_�i�H�ܽ�, �Y�ǯS���A�U����o�X�ܽ�
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

		// �ˬd�O�_���v��
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
				// �S���v��
			}
		}
		else
		{
			// ���b���|��,�i���Ʀ����D
		}
	}
	// �s�p�����|
	else
	{
		// �ˬd�O�_���v��
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
			// �S���v��
		}
	}
}
//-----------------------------
//���o���|�������ԲӸ��
GuildMemberStruct*        CGuildFrame::GetGuildMemberStruct(int dbid)
{
	CachGuildMember *p=g_internal->GetMemberInfo(dbid);

	GuildMemberStruct*ret=NULL;
	if (p)
		ret= &p->m_info;

	return ret;
}

// ----------------------------------------------------------------------------------
//��H
void CGuildFrame::Uninvite(const char* name)
{
	_Guild_data*date=g_internal->GetSelfGuildData();
	if (!date)
		return ;

	GuildBaseStruct& guildInfo =date->m_info;
	
	if ( guildInfo.GuildID == 0 )
	{
		// �S�����|
		return;
	}
	if ( name == NULL || strlen(name) < 1 )
	{
		// �ݭn���������a��H
		return;
	}

	// �������|
	if ( guildInfo.IsReady )
	{
		// �ˬd�O�_���v��
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
				// �S���v��
			}
		}
		else
		{
			// ���b���|��,�i���Ʀ����D
		}
	}
	// �s�p�����|
	else
	{
		// �ˬd�O�_���v��
		if ( guildInfo.LeaderDBID == g_pGameMain->GetDBID() )
		{
			NetCli_Guild::SL_LeaveNewGuild((char*)name);
		}
		else
		{
			// �S���v��
		}
	}
}

// ----------------------------------------------------------------------------------
//�h�X���|
void CGuildFrame::Leave()
{
	_Guild_data*date=g_internal->GetSelfGuildData();
	if (!date)
		return ;

	GuildBaseStruct& guildInfo =date->m_info;
	if ( guildInfo.GuildID == 0 )
	{
		// �S�����|
		return;
	}

	// �������|
	if ( guildInfo.IsReady )
	{
		NetCli_Guild::SL_LeaveGuild((char*)CNetGlobal::RoleData()->BaseData.RoleName.Begin());
	}
	// �s�p�����|
	else
	{
		NetCli_Guild::SL_LeaveNewGuild((char*)CNetGlobal::RoleData()->BaseData.RoleName.Begin());
	}
}

// ----------------------------------------------------------------------------------
//�q���|���C���Ƨ�s
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
//���|�إ�
int LuaFunc_GuildCreate(lua_State* L)
{
	

	const char* guildName = luaL_checkstring(L, 1);
	g_pDebugMessage->DebugOutput( 2 , "�n�D�إߤ��|  %s", guildName );
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
//���m�ɶ�
//���m���|��s��COOLDOWN
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
// ���a���}�ۤv�����|
void CGuildFrame::LeaveGuildMember		( int LeaderDBID , int MemberDBID )
{
	g_internal->LeaveGuildMember(LeaderDBID,MemberDBID);

}

//�������ԲӸ��
void CGuildFrame::GuildMemberInfo( GuildMemberStruct& Member )
{
	g_internal->GuildMemberInfo(Member);
}
//�[�J�s����
void CGuildFrame::JoinGuildMember( GuildMemberStruct& Member )
{
	g_internal->JoinGuildMember(Member);
}
// ----------------------------------------------------------------------------------
// �������|�إߵ��G
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
//���o���|��T
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

		// ���|�W��
		lua_pushstring(L, guildInfo.GuildName.Begin());
		//�|���j�W
		lua_pushstring(L, guildInfo.LeaderName.Begin());

		//�O�_�ۥ�
		lua_pushboolean(L, guildInfo.IsRecruit);

		// ���|�O�_�w����
		lua_pushboolean(L, guildInfo.IsReady);
		
		//�̦h�i�H���h�ַ|��
		lua_pushnumber(L, guildInfo.MaxMemberCount);
		//���|�n��
		lua_pushnumber(L, 0/*guildInfo.Score*/);

		size_t len=1024;

		memset(temp,0,1024);
		//���i
		strcpy_s(temp,len,guildInfo.Note.SafeStr());
		pSafeString=ItemUTIL::checkUtf8SafeString(temp,len,NULL);		
		//pSafeString[len]=0;
		lua_pushstring(L,pSafeString );

		memset(temp,0,1024);
		len=1024;
		//���|����
		strcpy_s(temp,len,guildInfo.Introduce.SafeStr());
		pSafeString=ItemUTIL::checkUtf8SafeString(temp,len,NULL);		
		//pSafeString[len]=0;
		lua_pushstring(L, pSafeString);


		GuildMemberBaseInfoStruct* selfB = g_internal->GetSelfBaseInfo();
		bool rank9=false;
		if (selfB&&selfB->Rank==EM_GuildRank_Leader) 
			rank9=true;
		bool IsLeader=g_pGameMain->GetDBID() == guildInfo.LeaderDBID;

	
		// �O�_�����|�|��
		lua_pushboolean(L,IsLeader| rank9);

		
		
		
		// ���|�O�_���}��d���O
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


		// ���|RANK
		lua_pushnumber(L, guildInfo.Level);


		//���|��
		lua_pushboolean( L, guildInfo.IsOwnHouse );

		//�}����[
  		lua_pushboolean( L, !guildInfo.IsLockGuildHouse );

		return 14;
	}
	return 0;
}



//-------------------------------------------------------------------------------
//�}�Ҥ��|�^�m����
void CGuildFrame::OpenGuildContribution(int iObjID)
{

#ifdef _DEBUG
		iObjID=-1;
#endif
		m_npcId=iObjID;


		CRoleSprite * npcSprite= g_pGameMain->FindSpriteByID( iObjID );
		//	GameObjDbStruct* npcDB = CNetGlobal::GetObj( iObjID );

		if (npcSprite==NULL) 
			g_pDebugMessage->DebugOutput( 2 , "�����Ʈw�L��NPC %d", iObjID );
		if ( npcSprite  || iObjID==-1)
		{
			// �n�D���s���o�ۤv���|��T
			g_internal->ReloadGuildDate();

			// �}�Ҥ��|�^�m����
			SendWorldEvent("GUILD_CONTRIBUTION_OPEN");
			g_pDebugMessage->DebugOutput( 2 , "�}�Ҥ��|�^�m���� NPC�s�� %d", iObjID );
			m_Open=true;
		}


	}
/*
// ----------------------------------------------------------------------------------
int LuaFunc_GuildPetitionSignature(lua_State* L)
{
	const char* name = luaL_checkstring(L, 1);
	NetCli_Guild::SL_NewGuildInvite((char*)name);				// �ܽХ[�J���|�s�p
	return 0;
}
*/

// ----------------------------------------------------------------------------------
//�[�J�s�p
int LuaFunc_GuildPetitionAccept(lua_State* L)
{
	g_pGuildFrame->PetitionReply(true);
	return 0;
}

// ----------------------------------------------------------------------------------
//�ڵ��s�p
int LuaFunc_GuildPetitionDecline(lua_State* L)
{
	g_pGuildFrame->PetitionReply(false);
	return 0;
}

// ----------------------------------------------------------------------------------
//�h�X�s�p
int LuaFunc_GuildPetitionQuit(lua_State* L)
{
	g_pGuildFrame->PetitionQuit();
	return 0;
}

// ----------------------------------------------------------------------------------
//�s�p��H
int LuaFunc_GuildPetitionKick(lua_State* L)
{
	const char* name = luaL_checkstring(L, 1);
	g_pGuildFrame->PetitionKick(name);
	return 0;
}



// ----------------------------------------------------------------------------------
// ���o���|�C��H��
int LuaFunc_GetNumGuildMembers(lua_State* L)
{
	lua_pushnumber(L, g_internal->GetNumGuildMembers());
	return 1;
}

// ----------------------------------------------------------------------------------
// ���o���|�������
int LuaFunc_GetGuildRosterInfo(lua_State* L)
{
	const char* name = NULL;
	const char* className = NULL;
	const char* subClassName = NULL;
	const char* guildTitle = g_ObjectData->GetString("GUILD_TEXT_TITLE_INVATE");
	//"�ٸ����s�b";//NULL;GUILD_TEXT_TITLE_INVATE
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
//�ܽ�
int LuaFunc_GuildInvite(lua_State* L)
{	

	g_pGuildFrame->Invite(lua_tostring(L, 1));
	return 0;
}

// ----------------------------------------------------------------------------------
// ��H
int LuaFunc_GuildUninvite(lua_State* L)
{
	g_pGuildFrame->Uninvite(lua_tostring(L, 1));
	return 0;
}

// ----------------------------------------------------------------------------------
// �h�X���|
int LuaFunc_GuildLeave(lua_State* L)
{
	g_pGuildFrame->Leave();
	return 0;
}

// ----------------------------------------------------------------------------------
// �s�ժ��`�I�i�}
int LuaFunc_ExpandGuildGroup(lua_State* L)
{
	g_internal->SetGuildHeaderState(luaL_checkint(L, 1) - 1, true);
	return 0;
}

// ----------------------------------------------------------------------------------
// �s�ժ��`�I����
int LuaFunc_CollapseGuildGroup(lua_State* L)
{
	g_internal->SetGuildHeaderState(luaL_checkint(L, 1) - 1, false);
	return 0;
}

// ----------------------------------------------------------------------------------
//�]�w�ثe��ܷ|�� Ū�� �ԲӸ��
int LuaFunc_SetGuildRosterSelection(lua_State* L)
{
	g_internal->SetGuildRosterSelection(luaL_checkint(L, 1) - 1);
	return 0;
}

// ----------------------------------------------------------------------------------
//���o�ثe��ܷ|�� 
int LuaFunc_GetGuildRosterSelection(lua_State* L)
{
	lua_pushnumber(L, g_internal->GetGuildRosterSelection() + 1);
	return 1;
}
//----------------------------------
//���o���żƶq
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
//���o���ŦW��
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
//�]�w���ŦW��
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
//�M�Τ��|��T
int LuaFunc_GF_GuildInfoApply		(lua_State* L)
{

	g_pGuildFrame->GuildInfoApply();

	//if (g_pGuildFrame)
	//	g_pGuildFrame->GetGuildInfo();
	return 0;
}
//----------------------------------
//�]�w�s��ENABLE
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
//���o���|���ﶵ
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
		//�վ㶥��
		if (self->Setting.AdjustRank)
		{ //�u��վ��ۤv�C�@�� ����վ�ۤv
			if (selfInfo->Rank>selectInfo->Rank&&selfInfo!=selectInfo)
				menu|=1;
		}
		//�𰣷|��		
		if (self->Setting.Kick)
		{
			//�u���H��ۤv�C�@�� �����ۤv
			if (selfInfo->Rank>selectInfo->Rank&&selfInfo!=selectInfo)
				menu|=2;
		}
		//�վ�s��
		if (self->Setting.AdjustGroup)
		{
			//�u��վ��ۤv�C�@��
			if (selfInfo->Rank>selectInfo->Rank)
				menu|=4;
		}

		//�]�w�����F������
		if (self->Setting.EditMemberNote)
		{
			//�u����Ѥ�ۤv&�ۤv�C�@�� 
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
			//�i�H��H
			lua_pushboolean(L,self->Setting.Kick);	

			//�i�H���H		
			bool v=false;
		//	if (NetCli_GuildChild::s_GuildInfo.IsRecruit&&self->Setting.Invite||vleader)//���|�ۥ�
			if (self->Setting.Invite||vleader)
			{
			//	if( g_pGameMain->IsBattleGroundZone()==false && g_pGuildHousesFrame->isGuildWaring()==false)
					v=true;			
					
			}
			lua_pushboolean(L,v);	

			//�]�w���i
			lua_pushboolean(L,self->Setting.SetGuildNote);	
		
			//�]�w�d��
			lua_pushboolean(L,self->Setting.PostMessage);	
			
			//�d���O�޲z
			lua_pushboolean(L,self->Setting.ManageMessage);			

			//�h��
			lua_pushboolean(L,vleader);	
			

			return 6;
		}

	}

	return 0;

}
//----------------------------------
//�|������
int LuaFunc_GF_LeaderChange 		(lua_State* L)			    //�|���洫
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
//���o�v��
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
//�]�w���Ū����
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
//���|�žԪ��v��
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
//���|�س]���v��

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
//�]�w���Ū����
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
//�]�w�|���F������
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
// �]�w�|������
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
// �]�w�|������
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
// �]�w�|���F������

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
// ���o�|���F������

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
// ���o�|���F������

int LuaFunc_GF_GetMemberCadreNote(lua_State* L)			
{
	const char*	note=NULL;
	if (g_pGuildFrame)
	note=g_pGuildFrame->GetMemberCadreNote();
		
		
	lua_pushstring(L,note);
	return 1;
}
//---------------------------------------------------------------
// �]�w�ӤH����
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
// �]�w�ӤH����
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
// ���o�ӤH����
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
// ���o�ӤH����
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
		// ���X�T������
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
					// ���X�T������
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
//�]�w���|�ۥͻP�ԭz.
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
              g_pGuildFrame->  CloseGuildCommand(); //�������|�R�O����.
	return 0;
}








//---------------------------------------------------
//���|�i�ܪ�ui ���o���|ID�ΦW�� .
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
//���|�i�ܪ�ui ���o���|��T.
int LuaFunc_GuildBoard_RequestGuildInfo(lua_State* L)
{
	int guildID = (int)luaL_checknumber(L,1);
	//g_pGuildFrame->GuildBoardRequest(guildID);
	NetCli_GuildChild::GuildBoardRequest( guildID );
	return 0;
}
//---------------------------------------------------
//�ˬd10G.
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
//���|�i�ܪ�ui �]�w���|�i��.
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
	// �q��������s.

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
//���|�i�ܪ�ui �]�w���|�i��.
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
//���|BBS ���o�ثe�d���ƶq.
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
//���|BBS �ШD�ثe�����ƶq
void		CGuildFrame::	GetPostCountRequest(void)
{

	//���սX
	/*
	int i;
	for(i=0;i<4;i++)
	{
		char temp[64];
		sprintf(temp,"TestTile%d",i);
		NetCli_Guild::SL_BoardPostRequest(temp,"���e�L��");

	}
	*/

	
	m_BBSPage=0;
	m_reloadIndex=0;

	m_bCanSend=true;
	reLoadBBSPage(m_reloadIndex);

	//NetCli_Guild::SL_BoardListRequest(m_BBSPage);

}
//---------------------------------------------------
//���|BBS ���o�ثe�����ƶq
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

//���|BBS ���o�ثe�d����T
CGuildFrame::BBSPost* CGuildFrame::GetPostInfo(int index)
{

	if (index>=0&&index<m_BBSMessages.size())
	{
		return &m_BBSMessages[index];

	}

	return NULL;
}
//---------------------------------------------------
//���|BBS ���o�ثe�d����T
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
//���|BBS �]�w�ثe�d����T
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

//���줽�|���|���C��
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
//���|BBS �]�w�d��
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
//���줽�|�귽�ܰ�
void CGuildFrame::ModifySimpleGuild( SimpleGuildInfoStruct& SimpleGuildInfo )
{
	m_tempGuildInfo.Resource=SimpleGuildInfo.Resource;	

	g_internal->ModifySimpleGuild(SimpleGuildInfo);	
}
//--�����줽�|��� �i��O�d�ߨ�L�u�|�� �άO�ۤv�����|
void CGuildFrame::GuildInfo(GuildBaseStruct& Guild , int MemberCount,bool bNeedSend )
{
	g_internal->GuildInfo(Guild,MemberCount,bNeedSend);	
}

//���o���|�򥻸��
void CGuildFrame::GuildBoardRequest( int guildID,bool&v)
{
	g_internal->GuildBoardRequest(guildID,v);
}
//---------------------------------------------------
//���|BBS �]�w�d��
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

//�𰣦���
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
//���|BBS // ���o�d��
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
//���|BBS // �s�W�d��
int	CGuildFrame::NewPost(const char* tTitle,const char* tMessage)
{

	int ret=0;
	int time=timeGetTime();

	if (CheckStr(tTitle,127)!=true)
		ret|=1;

	if (CheckStr(tMessage,512)!=true)
		ret|=2;

	//�d�����j�ɶ��ˬd  DEBUG�Ҧ�30 �� release 2����
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
//���|BBS // �s�W�d��
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
//���|BBS 	// �R���d��
int LuaFunc_GF_DeletePost(lua_State* L)
{
		return 0;
}*/
//---------------------------------------------------
// �ШD�ثe�X�����
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
// �ШD�ثe�X�����
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


//Ū��BAG�����~
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
//�����^�~
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
//�����^�m�ɭ�
void CGuildFrame::CloseGuildContribution(void)
{

	CloseContribution();

	// �����^�m�ɭ�
	SendWorldEvent("GUILD_CONTRIBUTION_CLOSE");
	m_npcId=-1;
	m_Open=false;
}
//.��ѫ~�ǵ�server
void CGuildFrame::ContributionItemOK(int Money,int BonusMoney)
{
	//�Գ���
	if( g_pGameMain->IsBattleGroundZone() )
	{
		return ;

	}

m_GuildPointTotal=Money;
	// �ˬd���|�O�_�w���ߩΪ̤w�g�[�J���|
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
//�T�w�^�m...��ѫ~�ǵ�server
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
//---���|�n��
int CGuildFrame::GetGuildPoint()
{
	

	 return /*m_tempGuildInfo.Score*/0;


}

//���o�ۤv���|�W��
const char *CGuildFrame::GetGuildName(void)
{
	return m_tempGuildInfo.GuildName.Begin();
}

//���o���|�W��
const char *CGuildFrame::GetGuildNameByID(int GuildID)
{
	return g_internal->GetGuildNameByID(GuildID);
}
int CGuildFrame::GetGuildIDByName(const char* name)
{
	return g_internal->GetGuildIDByName(name);
}
//����s�p�ܽ�
void CGuildFrame::NewGuildInvite( int GuildID, char* LeaderName, int LeaderDBID )
{
	g_internal->NewGuildInvite(GuildID,LeaderName,LeaderDBID);
}

//�����^�m����
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

//���}�s�p���| �@�ϥν�H���O�̷|����
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
		// �n�D���s���o�ۤv���|��T
		g_internal->ReloadGuildDate();
		// �}�Ҥ��|�ө�
		//SendWorldEvent("GUILD_SHOP_OPEN");

	}

	

	
	sprintf(tempStr,"%s %s",pmsg,pmsg1);
	

	g_pGameMain->SendWarningMsg(tempStr);
	*/
}
//�������|�ܽ�
void CGuildFrame::GuildInviteResult(char* PlayerName , int	Result )
{
	g_internal->GuildInviteResult(PlayerName,Result);
}

//���}���|�@��̡@
void CGuildFrame::LeaveGuildResult	( char* KickPlayerName , bool Result )
{
	g_internal->LeaveGuildResult(KickPlayerName,Result);
}



//�Хߤ��|���^��
void CGuildFrame::CreateGuildResult( int GuildID , char* GuildName , int Result )
{
	g_internal->CreateGuildResult(GuildID ,GuildName,Result);
}
//�[�J���|
void CGuildFrame::JoinGuild( int GuildID , char* LeaderName )
{
	g_internal->JoinGuild(GuildID,LeaderName);

}
//�[�J�s�p���|
void CGuildFrame::JoinNewGuild( int GuildID , char* LeaderName )
{
	g_internal->JoinNewGuild(GuildID,LeaderName);

}

//---------------------------------
//���|�ө��R
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
		lua_pushnumber(L, time);				// ����
		lua_pushnumber(L, money);				// ���|����
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

	// �A�S������@�Ӥ��|
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

	// ���o���|�W��
	int guildID = g_pGuildFrame->GetGuildIDByName(guildNmae);
	if ( guildID == 0 )
	{
		// ���|�W�٤��s�b
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_GUILDWAR_NOT_EXIST"));
		lua_pushboolean(L, false);
		return 1;
	}

	// �ˬd���|����
	if ( g_pGuildFrame->GetGuildResource(0) < money )
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_GUILDWAR_NOT_ENOUGH_MONEY"));
		lua_pushboolean(L, false);
		return 1;
	}

	/*
	// �ˬd����
	if ( CNetGlobal::RoleData()->BodyMoney() <= money )
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_GUILDWAR_NOT_ENOUGH_MONEY"));
		lua_pushboolean(L, false);
		return 1;
	}

	// �ˬd�n��
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

// ���o���|�����žԸ�T
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
	lua_pushnumber(L, info->HousesWar.WinCount);	//�ӳ�
	lua_pushnumber(L, info->HousesWar.EvenCount);	//����
	lua_pushnumber(L, info->HousesWar.LostCount);	//�ѳ�
	return 3;
}
//------------------------------------------------------------------
int LuaFunc_GetGuildWarOutdoorsPoint(lua_State* L)
{
	GuildBaseStruct* info = g_pGuildFrame->GetGuildBaseInfo();
	lua_pushnumber(L, info->WarWinCount);	//�ӳ�
	lua_pushnumber(L, info->WarEvenCount);	//����
	lua_pushnumber(L, info->WarLostCount);	//�ѳ�
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
			lua_pushnumber(L, iter->time);	// �ѩ�ɶ�(��)
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
				lua_pushnumber(L, iter->time);	// �ѩ�ɶ�(��)
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


int LuaFunc_GF_Rename(lua_State* L)			// ���|��W
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