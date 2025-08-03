#pragma once

#include "PG/PG_Other.h"
#include "../../MainProc/NetGlobal.h"

//-----------------------------------------------------------------------------------------------------
//Cli to Cli 傳送情緒指令
struct PG_CtoC_Emote
{
	CliToCliPGCommandEnum Command;

	int sourceID;
	char szTargetName[31];
	int emoteID;

	PG_CtoC_Emote()
	{
		Command = EM_PG_CtoC_Emote;
	}
};

//-----------------------------------------------------------------------------------------------------
// 範圍廣播玩家目標
struct PG_CtoC_PlayerTarget
{
	CliToCliPGCommandEnum Command;

	int player;
	int target;

	PG_CtoC_PlayerTarget()
	{
		Command = EM_PG_CtoC_PlayerTarget;
	}
};

enum EM_CLI_SPEAKDIALOG_EVENT
{
	EM_CLI_SPEAKDIALOG_EVENT_CLOSE	= -1,
	EM_CLI_SPEAKDIALOG_EVENT_SELECT	= 0
};

typedef void (*pfCliSpeakDialogfunc)( int iID, PVOID pContext );

//-----------------------------------------------------------------------------------------------------
#define OpenSessionResultFunc boost::function<void (int sessiontype, int sessionid)>

//-----------------------------------------------------------------------------------------------------
//typedef - marked by slime 2010/08/02
union ClientToClientInfo
{
	char _data[20];
	struct {
		int id;
		float x, y, z;
	}parentMount;
};

enum ClientLuaEventType
{
	EM_CLIENT_LUA_EVENT_TYPE01        = 0,
	EM_CLIENT_LUA_EVENT_TYPE02		  = 1,
	EM_CLIENT_LUA_EVENT_HOUSE_SERVANT = 99
};

class NetCli_Other : public CNetGlobal
{
protected:
    //-------------------------------------------------------------------

    static bool _Init();
    static bool _Release();

	static void _PG_Other_LtoC_OpenDialog			( int Sockid , int Size , void* Data );
	static void _PG_Other_LtoC_DialogSelectStr		( int Sockid , int Size , void* Data );
	static void _PG_Other_LtoC_CloseDialog			( int Sockid , int Size , void* Data );
	static void _PG_Other_LtoC_DialogSelectStrEnd	( int Sockid , int Size , void* Data );
	static void _PG_Other_LtoC_DialogSetContent		( int Sockid , int Size , void* Data );
	static void _PG_Other_LtoC_DialogSetTitle		( int Sockid , int Size , void* Data );
//	static void _PG_Other_XtoC_SystemMessage		( int Sockid , int Size , void* Data );
//	static void _PG_Other_XtoC_SystemMessageData	( int Sockid , int Size , void* Data );

	static void _PG_Other_LtoC_BeginCastingResult	( int Sockid , int Size , void* Data );
	static void _PG_Other_LtoC_InterruptCasting		( int Sockid , int Size , void* Data );
	static void _PG_Other_LtoC_CastingOK			( int Sockid , int Size , void* Data );
	static void _PG_Other_LtoC_ItemExchangeResult	( int Sockid , int Size , void* Data );
	static void _PG_Other_LtoC_RangeDataTransfer	( int Sockid , int Size , void* Data );

	static void _PG_Other_LtoC_ColoringShopResult	( int Sockid , int Size , void* Data );
	static void _PG_Other_LtoC_PlayerInfoResult		( int Sockid , int Size , void* Data );

	static void _PG_Other_LtoC_SysKeyFunctionResult	( int Sockid , int Size , void* Data );

	static void _PG_Other_LtoC_ObjectPosInfo		( int Sockid , int Size , void* Data );
	static void _PG_Other_LtoC_OpenClientMenu		( int Sockid , int Size , void* Data );	
	static void _PG_Other_LtoC_RunGlobalPlotResult	( int Sockid , int Size , void* Data );	
	static void _PG_Other_LtoC_HateListResult		( int Sockid , int Size , void* Data );	

	static void _PG_Other_LtoC_OpenGiveItemPlot		( int Sockid , int Size , void* Data );	
	static void _PG_Other_LtoC_CheckPasswordResult	( int Sockid , int Size , void* Data );	

	static void _PG_Other_DtoC_Find_DBID_RoleName	( int Sockid , int Size , void* Data );	
	static void _PG_Other_LtoC_CampID				( int Sockid , int Size , void* Data );	
	static void _PG_Other_LtoC_GoodEvilValue		( int Sockid , int Size , void* Data );	
	static void _PG_Other_LtoC_ExchangeMoneyTable	( int Sockid , int Size , void* Data );	
	static void _PG_Other_LtoC_DBStringTable		( int Sockid , int Size , void* Data );	
	static void _PG_Other_LtoC_ColoringHorseResult	( int Sockid , int Size , void* Data );	
	static void _PG_Other_LtoC_PKDeadDropItem		( int Sockid , int Size , void* Data );	
	static void _PG_Other_LtoC_PlayerEvent			( int Sockid , int Size , void* Data );	
	static void _PG_Other_LtoC_ClientClock			( int Sockid , int Size , void* Data );	
	static void _PG_Other_LtoC_AccountMoneyLockInfo	( int Sockid , int Size , void* Data );	
	static void _PG_Other_LtoC_AccountMoneyTranError( int Sockid , int Size , void* Data );	
	static void _PG_Other_LtoC_SetCycleMagic		( int Sockid , int Size , void* Data );	
	static void _PG_Other_LtoC_DelJob				( int Sockid , int Size , void* Data );	
	static void _PG_Other_LtoC_SetSmallGameMenuType	( int Sockid , int Size , void* Data );
	static void _PG_Other_LtoC_SetSmallGameMenuValue( int Sockid , int Size , void* Data );
	static void _PG_Other_LtoC_SetSmallGameMenuStr	( int Sockid , int Size , void* Data );

	static void _PG_Other_LtoC_ResetInstanceResult	( int Sockid , int Size , void* Data );
	static void _PG_Other_LtoC_ResetInstanceInfo	( int Sockid , int Size , void* Data );
	static void _PG_Other_LtoC_PartyDice			( int Sockid , int Size , void* Data );

	static void _PG_Other_LtoC_BodyShopResult		( int Sockid , int Size , void* Data );

	static void _PG_Other_LtoC_MoneyVendorReport	( int Sockid , int Size , void* Data );
	static void _PG_Other_LtoC_BotReport			( int Sockid , int Size , void* Data );
	static void _PG_CliConnect_LtoC_ZoneOpenState	( int Sockid , int Size , void* Data );

	static void _PG_Other_LtoC_UnlockBotReport		( int Sockid , int Size , void* Data );

	static void _PG_Other_LtoC_SortBankEnd			( int Sockid , int Size , void* Data );

	static void _PG_Other_DtoC_LoadClientDataResult ( int Sockid , int Size , void* Data );
	static void _PG_Other_LtoC_SaveClientDataResult ( int Sockid , int Size , void* Data );

	static void _PG_Other_DtoC_LoadClientDataResult_Account ( int Sockid , int Size , void* Data );
	static void _PG_Other_MtoC_SaveClientDataResult_Account ( int Sockid , int Size , void* Data );

	static void _PG_Other_LtoC_SetMapMark			( int Sockid , int Size , void* Data );
	static void _PG_Other_LtoC_ClsMapMark			( int Sockid , int Size , void* Data );

	static void _PG_Other_LtoC_SetRoomValue			( int Sockid , int Size , void* Data );
	static void _PG_Other_LtoC_ClsRoomValue			( int Sockid , int Size , void* Data );

	static void _PG_Other_LtoC_AddExSkill			( int Sockid , int Size , void* Data );
	static void _PG_Other_LtoC_DelExSkill			( int Sockid , int Size , void* Data );
	static void _PG_Other_LtoC_SpellExSkillResult	( int Sockid , int Size , void* Data );
	static void _PG_Other_LtoC_BaseObjValueChange	( int Sockid , int Size , void* Data );
	static void _PG_Other_LtoC_TransferData_Range	( int Sockid , int Size , void* Data );
	static void _PG_Other_LtoC_LuaClientEvent		( int Sockid , int Size , void* Data );
	static void _PG_Other_LtoC_SetRoleValueResult	( int Sockid , int Size , void* Data );
	static void _PG_Other_LtoC_ZoneOpen				( int Sockid , int Size , void* Data );
	static void _PG_Other_LtoC_SetNewTitleSysResult	( int Sockid , int Size , void* Data );
	static void _PG_Other_LtoC_NewTitleSys_UseItemResult( int Sockid , int Size , void* Data );
	static void _PG_Other_LtoC_OpenSessionResult	( int Sockid , int Size , void* Data );
	static void _PG_Other_LtoC_FixAccountFlag		( int Sockid , int Size , void* Data );

	static void _PG_Other_LtoC_WeekInstances				( int Sockid , int size , void* data );
	static void _PG_Other_LtoC_WeekInstancesTeleportResult	( int Sockid , int size , void* data );
	static void _PG_Other_LtoC_WeekInstancesReset			( int Sockid , int size , void* data );
	static void _PG_Other_LtoC_WeekInstancesLiveTime		( int Sockid , int size , void* data );
	static void _PG_Other_LtoC_OpenWeekInstancesFrame		( int Sockid , int size , void* data );

	static void _PG_Other_LtoC_PhantomRankUpResult			( int Sockid , int size , void* data );
	static void _PG_Other_LtoC_PhantomLevelUpResult			( int Sockid , int size , void* data );
	static void _PG_Other_LtoC_PhantomEQResult				( int Sockid , int size , void* data );
	static void _PG_Other_LtoC_FixPhantomEQ					( int Sockid , int size , void* data );
	static void _PG_Other_LtoC_FixPhantom					( int Sockid , int size , void* data );
	static void _PG_Other_LtoC_OpenPhantomFrame				( int Sockid , int size , void* data );
	static void _PG_Other_LtoC_ItemCombinInfo				( int Sockid , int size , void* data );
	static void _PG_Other_LtoC_ItemCombinGroupID			( int Sockid , int size , void* data );
	//-----------------------------------------------------------------------------------------------------
	//Cli to Cli 傳送情緒指令
	static void _PG_CtoC_Emote( void *Data , int Size );

	//-----------------------------------------------------------------------------------------------------
	static void _PG_CtoC_PlayerTarget( void *Data , int Size );

	//-----------------------------------------------------------------------------------------------------

public:
	static NetCli_Other* This;
	static pfCliSpeakDialogfunc	m_pfuncCliSpeakDialog;
	static PVOID				m_pCliSpeakDialogContext;

	static LuaDialogType_ENUM	m_DialogType;
	static string				m_DialogContent;
	static vector<string>		m_DialogSelectStr;
	static map<int, int>		m_SessionID;
	static int					m_SessionCBID;
	static map<int, OpenSessionResultFunc> m_OpenSessionResultFunc;

	static void test()
	{
		/*
		DialogContent = "TTTTTTTTTTTTTTTTTTT";
		DialogSelectStr.push_back( "111111111" );
		DialogSelectStr.push_back( "2222222222" );
		DialogSelectStr.push_back( "333333" );
		DialogSelectStr.push_back( "4444" );

		This->R_OpenDialog( EM_LuaDialogType_Select, NULL, DialogContent.c_str(), DialogSelectStr, NULL );
		*/

	}

    NetCli_Other() { This = this; }

public:
	static void S_DialogInput		( int SelectID, const char* pszText, int iCheckResult );

	static void S_DialogSelectID	( int SelectID );

	//要求開始跑Casting Bar
	static void S_BeginCastingRequest	( CastingTypeENUM Type , int ItemID , int TargetGUID );
	//中斷Casting Bar
	static void S_InterruptCastingRequest();

	//傳送效能資訊
	static void S_SendPerformance(const char* cMac, float MaxCPUUsage, float MaxMemUsage, float MaxFPS, float AverageCPUUsage, float AverageMemUsage, float AverageFPS, float MinCPUUsage, float MinMemUsage, float MinFPS, float Ping, float PosX, float PosY, float PosZ );

	//傳送Tracert資訊
	static void S_SendTracertList(int DBID, int ping, TracertList list);

	//要求兌換物品
//	static void S_ItemExchangeRequest	( char* ItemKey , int ItemSerial );

	//要求轉送範圍資訊
	static void S_RangeDataTransferRequest( int Range , int Size , const char* Data );
	static void	S_SendEmote( int sourceID, const char* pTargetName, int emoteID );	
	static void S_SendPlayerTarget(int sourceID, int targetID);	

	static void	S_ColoringShopRequest( ColoringStruct& ColorInfo , const char* Password );

	//要求某玩家的裝備資料
	static void	S_PlayerInfoRequest( int PlayerGItemID );


	//static void	S_PlayerInfoRequest( int PlayerGItemID );
	static void S_SysKeyFunctionRequest( int KeyID , int TargetGUID );

	//要求區域內所有NPC的位置資料
	static void S_AllObjectPosRequest( );

	//系統全域的劇情
	static void S_RunGlobalPlotRequest( int PlotID , int Value = 0 );

	//要求仇恨列表	
	static void S_HateListRequest( int NpcGUID );

	static void S_ManagePasswordKey( );
	static void S_ManagePassword( const char* MD5 );

	static void S_GiveItemPlot( bool IsSort , int ItemPos[4] );

	static void S_CheckPassword( const char* Password );

	//GM回報
	static void SL_GmNotification( const char* Subject, const char* Content, const char* Classification );

	//要求仇恨列表

	// Client 端 Script 對話框專用
	virtual void R_CloseDialog			() = 0;
	//virtual void R_CreateDialog			( LuaDialogType_ENUM DialogType , const char* Title, const char* Content , vector<string>& SelectStr, pfCliSpeakDialogfunc pFunc, PVOID pContext = NULL ) = 0;
	virtual void R_CreateDialog			( LuaDialogType_ENUM DialogType , const char* Content ) = 0;
	virtual void R_OpenDialog			( LuaDialogType_ENUM DialogType , const char* Title, const char* Content , vector<string>& SelectStr, pfCliSpeakDialogfunc pFunc, PVOID pContext = NULL ) = 0;
	virtual void R_DialogSetContent		( const char* Content ) = 0;
	virtual void R_DialogSetTitle		( const char* cpszTitle ) = 0;


	// 對話選則盒的流程
	// R_OpenDialog

	//virtual void R_SystemMessage		( SystemMessageENUM Msg ) = 0;
//	virtual void R_SystemMessageData	( SystemMessageDataENUM Msg , int Size , char* Data ) = 0;

	//Casting Bar
	virtual void R_BeginCastingResult	( CastingTypeENUM Type , int ItemID , int CastingTime , bool Result ) = 0;
	virtual void R_InterruptCasting		( CastingTypeENUM Type ) = 0;
	virtual void R_CastingOK			( CastingTypeENUM Type ) = 0;
//	virtual void R_ItemExchangeResult	( ItemExchangeResultTypeENUM ResultType , ItemFieldStruct& Item , int Money , int Money_Account ) = 0;
	virtual void R_RangeDataTransfer	( int DataSize , const char* Data ) = 0;

	virtual void R_CtoC_Emote           ( PG_CtoC_Emote* pEmoteInfo ) = 0;

	virtual void R_ColoringShopResult	( ColoringShopResultENUM Result ) = 0;

	virtual void R_PlayerInfoOK			( int PlayerGItemID , EQStruct& EQ , const char* Note ) = 0;
	virtual void R_PlayerInfoFailed		( int PlayerGItemID ) = 0;
	virtual void R_SysKeyFunctionResult	( bool Result , int KeyID , int TargetGUID ) = 0;
	virtual void R_ObjectPosInfo		( int Count , ObjectPosInfoStruct Info[6000] ) = 0;
	virtual void R_OpenClientMenu		( ClientMenuType_ENUM MenuType ) = 0;
	virtual	void R_PlayerTarget			(int PlayerGItemID, int TargetGItemID) = 0;
	virtual void R_RunGlobalPlotResult	( bool Result ) = 0;
	virtual void R_HateListResult		( int NpcGUID , NPCHateList& NPCHate ) = 0;
	virtual void R_OpenGiveItemPlo		( bool IsSort , int Count , int TargetGUID ) = 0;
	virtual void R_CheckPasswordResult	( char* Password , bool Result ) = 0;	
	virtual void R_CampID				( int GItemID , CampID_ENUM CampID ) = 0;
	virtual void R_GoodEvilValue		( int PlayerGUID , float GoodEvilValue , GoodEvilTypeENUM GoodEvilType ) = 0;
	virtual void R_ExchangeMoneyTable	( int Count , int DataSize , char* Data ) = 0;
	virtual void R_DBStringTable		( int Count , int DataSize , char* Data ) = 0;
	virtual void R_AccountMoneyTranError( ) = 0;	
	//////////////////////////////////////////////////////////////////////////
	//dbid 與 名字查尋  1.dbid = -1  查尋DBID   , 2 dbid != -1 查尋名字

	static  void S_Find_DBID_RoleName	( int DBID , const char* RoleName );
	virtual void R_Find_DBID_RoleName	( int DBID , char* RoleName , bool IsFind ) = 0;
//--------------------------------------------------------------------------------------------
	static	string						m_DialogTitle;

	//////////////////////////////////////////////////////////////////////////
	//座騎染色 
	//////////////////////////////////////////////////////////////////////////
	static void S_ColoringHorse( int Color[4], char* Password , int ItemPos );
	virtual void R_ColoringHorseResult( ColoringShopResultENUM Result ) = 0;
	//////////////////////////////////////////////////////////////////////////
	//角色名稱重
	//////////////////////////////////////////////////////////////////////////
	static void S_RoleNameDuplicateErr( int DBID1 , int DBID2 , const char* RoleName );
	//////////////////////////////////////////////////////////////////////////
	//死亡掉落物品
	//////////////////////////////////////////////////////////////////////////
	virtual void R_PKDeadDropItem( ItemFieldStruct& Item ) = 0;
	//////////////////////////////////////////////////////////////////////////
	virtual void R_PlayerEvent( PlayerEventTypeENUM Type ) = 0;

	virtual void R_ClientClock( int BeginTime , int NowTime , int EndTime , int Type ) = 0;

	//////////////////////////////////////////////////////////////////////////
	//帳號幣鎖定資料
	virtual void R_AccountMoneyLockInfo ( int LockAccountMoney[ Def_AccountMoneyLockTime_Max_Count_ ] , int LockAccount_Forever ) = 0;	

	//////////////////////////////////////////////////////////////////////////
	//通知Server看的目標
	static void SL_LookTarget( int TargetGUID );
	//設定玩家循環法術
	static void SL_SetCycleMagic( int MagicID , int MagicPos );
	//收到設定玩家循環法術
	virtual void RL_SetCycleMagic( int MagicID , int MagicPos ) = 0;

	//要求關閉Master 的登入
	static void SL_CloseMaster( const char* Password );

	//刪除職業
	virtual void RL_DelJob( Voc_ENUM Job ) = 0;

	//////////////////////////////////////////////////////////////////////////
	//小遊戲介面
	virtual void RL_SetSmallGameMenuType	( int MenuID , SmallGameMenuTypeENUM Type  ) = 0;
	virtual void RL_SetSmallGameMenuValue	( int MenuID , int ID , int Value ) = 0;
	virtual void RL_SetSmallGameMenuStr		( int MenuID , int ID , const char* Content ) = 0;
	//////////////////////////////////////////////////////////////////////////
	//封包衰減設定
	static void SL_SetSendPacketMode		( SendPacketModeStruct& Mode ); 
	//////////////////////////////////////////////////////////////////////////
	//副本重置
	static void SL_ResetInstanceRequest		( int ZoneID );
	virtual void RL_ResetInstanceResult		( bool Result ) = 0;
	virtual void RL_ResetInstanceInfo		( int ZoneID , bool IsReset ) = 0;

	//////////////////////////////////////////////////////////////////////////
	static bool SL_GmResetRole				( int RoleBaseSize , int PlayerRoleBaseSize , BaseRoleData&	BaseData , PlayerBaseRoleData& PlayerBaseData );
	//////////////////////////////////////////////////////////////////////////	
	static void SL_PartyDice				( int Type );
	virtual void RL_PartyDice				( int PlayerDBID , int Type , int Rand ) = 0;
	//////////////////////////////////////////////////////////////////////////
	virtual void RL_BodyShopResult			( bool Result ) = 0;
	static void SL_BodyShop					( char Bone[15]  );
	//////////////////////////////////////////////////////////////////////////
	virtual void RL_MoneyVendorReport		( int Count ) = 0;
	static void SL_MoneyVendorReport		( const char* Name , const char* Content );


	//////////////////////////////////////////////////////////////////////////
	//通知Server有人用機器人
	static void SL_BotReport				( const char* Name , const char* Content );
		//回應被人通知使用機器人
		virtual void RL_BotReport				( int Count ) = 0;
			//回應自己不是機器人
			static void SL_UnlockBotReport			( );
			//通知檢舉者處理狀況
			virtual void RL_UnlockBotReport			( const char* Name , UnlockBotTypeENUM Type ) = 0;

	//////////////////////////////////////////////////////////////////////////
	static void SL_SortBankRequest();
		virtual void RL_SortBankEnd( ) = 0;
	
	//////////////////////////////////////////////////////////////////////////
	virtual void RD_LoadClientDataResult( int KeyID , int Size , char* Data  ) = 0;
	virtual void RL_SaveClientDataResult( int KeyID , bool Result  ) = 0;

	static void SL_LoadClientData	( int KeyID );
	static void SL_SaveClientData	( int KeyID , int Size , char* Data );

	virtual void RD_LoadClientDataResult_Account( int KeyID , int Size , char* Data  ) = 0;
	virtual void RM_SaveClientDataResult_Account( int KeyID , bool Result  ) = 0;

	static void SM_LoadClientData_Account	( int KeyID );
	static void SM_SaveClientData_Account	( int KeyID , int Size , char* Data );

	//////////////////////////////////////////////////////////////////////////
	//設定Map 的 Icon & Tips
	virtual void RL_SetMapMark		( MapMarkInfoStruct& Info ) = 0;
	virtual void RL_ClsMapMark		( int MarkID ) = 0;

	//房間變數
	virtual void RL_SetRoomValue( int KeyID , int Value ) = 0;
	virtual void RL_ClsRoomValue( int KeyID ) = 0;
	//////////////////////////////////////////////////////////////////////////
	//自殺封包
	static void SL_Suicide			( int Type );
	//////////////////////////////////////////////////////////////////////////
	//企劃設定的額外技能
	virtual void RL_AddExSkill( ExSkillBaseStruct&	Info ) = 0;
	virtual void RL_DelExSkill( ExSkillBaseStruct&	Info ) = 0;
	virtual void RL_SpellExSkillResult( bool Result , RoleErrorMsgENUM Msg ) = 0;

	static void SL_SpellExSkill( int TargetID , float TargetX , float TargetY , float TargetZ ,ExSkillBaseStruct& Info , int ShootAngle , int ExplodeTime );
	//////////////////////////////////////////////////////////////////////////
	//client 端事件需要記錄給營運商查詢的事件
	static void SL_ClientEventLog( int EventType );
	//////////////////////////////////////////////////////////////////////////
	//範圍內有物件改變設地數值
	virtual void RL_BaseObjValueChange( int GItemID , ObjValueChangeENUM Type , const char* ValueStr , int Value ) = 0;
	//////////////////////////////////////////////////////////////////////////
	static void SL_TransferData_Range( char Data[20] );
	virtual void RL_TransferData_Range( int GItemID , char Data[20] ) = 0;
	//////////////////////////////////////////////////////////////////////////
	//lua 觸發的client 端事件
	virtual void RL_LuaClientEvent( int GItemID , int EventType , const char* Content ) = 0;
	//////////////////////////////////////////////////////////////////////////
	virtual void RL_SetRoleValueResult	( bool Result ) = 0;	
	static void SL_SetRoleValue			( RoleValueName_ENUM	Type , double Value );
	//////////////////////////////////////////////////////////////////////////
	virtual void RL_ZoneOpen			( int ZoneID ) = 0;
	//////////////////////////////////////////////////////////////////////////
	static void SL_ClientLanguage			( int iLanguage );
	//////////////////////////////////////////////////////////////////////////
	//[新]頭銜系統
	static void SL_SetNewTitleSys( NewTitleSysPosENUM Pos , int TitleID );
	virtual void RL_SetNewTitleSysResult( bool Result ) = 0;

	//PosType 0 身體 1 銀行 2 EQ
	static void SL_NewTitleSys_UseItem( int PosType , int Pos );
	virtual void RL_NewTitleSys_UseItemResult( NewTitleSysUseItemResultENUM Result ) = 0;	

	//////////////////////////////////////////////////////////////////////////
	static void SL_OpenSession			(SessionTypeENUM Type, OpenSessionResultFunc func = NULL);
	static void SL_CloseSession			(SessionTypeENUM Type, int SessionID);
	virtual void RL_OpenSessionResult	(SessionTypeENUM Type, int SessionID, int CBID) = 0;
	//////////////////////////////////////////////////////////////////////////
	virtual void RL_FixAccountFlag( StaticFlag< _ACCOUNT_FLAG_COUNT_ >& AccountFlag ) = 0;
	static void SL_AutoReportPossibleCheater( AutoReportBaseInfoStruct& TargetInfo , AutoReportPossibleCheaterENUM Type );

	//要求今天的複本
	static void SL_WeekInstancesRequest(  );
		//今天的複本
		virtual void RL_WeekInstances( int day , int count , WeekZoneStruct info[] ) = 0;

	//要求傳送到某副本
	static void SL_WeekInstancesTeleport( int zoneID );
		//要求結果
		virtual void RL_WeekInstancesTeleportResult( bool result ) = 0;

	//要求重置某副本
	static void SL_WeekInstancesZoneReset( int zoneID );

	//重置副本傳送次數
	static void SL_WeekInstancesReset(  );
		//重置副本傳送次數結果
		virtual void RL_WeekInstancesReset( bool result ) = 0;

	//複本剩下的生存時間
	virtual void RL_WeekInstancesLiveTime( int sec ) = 0;
	//開啟鏡世界副本介面
	virtual void RL_OpenWeekInstancesFrame(  ) = 0;
	//////////////////////////////////////////////////////////////////////////
	//魔靈升階(或產生)
	static void SL_PhantomRankUp( int phantomID , bool summon );
		//魔靈升階結果
		virtual void RL_PhantomRankUpResult( bool summon , bool result ) = 0;

	//魔靈升級
	static void SL_PhantomLevelUp( int phantomID );
		//魔靈升級結果
		virtual void RL_PhantomLevelUpResult( bool result ) = 0;

	//魔靈裝備
	static void SL_PhantomEQ( int eq[2] );
		//魔靈裝備結果
		virtual void RL_PhantomEQResult( bool result ) = 0;

	//修正魔靈裝備
	virtual void RL_FixPhantomEQ( int eq[2] ) = 0;
	//修正魔靈資料
	virtual void RL_FixPhantom( int phantomID , PhantomBase& info ) = 0;
	//開啟幻靈介面
	virtual void RL_OpenPhantomFrame(  ) = 0;

	//後台魔幻寶盒資料
	virtual void RL_ItemCombinInfo( int groupID , ItemCombineStruct& itemCombine ) = 0;
	//後台魔幻寶盒索引ID
	virtual void RL_ItemCombinGroupID( int groupID ) = 0;

};


