#pragma once


#include "NetCli_Other.h"

using namespace std;

struct NetInspectStruct
{
	int			GItemID;				// �˵��˳ƪ���s��
	EQStruct	EQInfo;					// �˵��˳Ƹ�T
	string		note;					// �˵��˳Ƶ���
};

struct ObjectPosStruct
{
	ObjectPosInfoStruct	Info;
	bool				IsShow;
	int					ColorID;
};

struct FindDBIDRoleNameStruct
{
	int    DBID;
	string RoleName;
	bool   IsFind;
};

class IFindDBIDRoleNameWorkBase
{
public:
	IFindDBIDRoleNameWorkBase()
	{
		DBID = -1;
	   IsFind = false;
	}

	int    DBID;
	string RoleName;
	bool   IsFind;

	virtual void Update() = 0;
};

class NetCli_OtherChild : public NetCli_Other
{
public:	
	static bool _Init();
	static bool _Release();

	static NetInspectStruct				s_inspectInfo;
	static vector<ObjectPosStruct>		_AllObjectPos;

	static float						_ColorList[3][125];

	static int s_NPCHateListGUID ;
	static NPCHateList s_NPCHateList ;


	static std::set< string > _FindingStructName;
  	static std::set< int >    _FindingStructDBID;

	static std::map< string, FindDBIDRoleNameStruct > _NameFindStructMap;
  	static std::map< int   , FindDBIDRoleNameStruct > _DBIDFindStructMap;

	static std::list< IFindDBIDRoleNameWorkBase* > _FindDBIDRoleNameWorks;

	static void _InsertDBIDRoleName( int DBID, const char* RoleName, bool IsFind );
  
	static bool s_bRunGlobalPlotResulted;	

	static set<INT64>					m_ReportList;

public:
	void R_OpenDialog					( LuaDialogType_ENUM DialogType , const char* Title, const char* Content , vector<string>& SelectStr, pfCliSpeakDialogfunc pFunc, PVOID pContext );
	void R_CloseDialog					();
	void R_DialogSetContent				( const char* Content );
	void R_DialogSetTitle				( const char* cpszTitle );
	void R_CreateDialog					( LuaDialogType_ENUM DialogType , const char* Content );

	static void S_Inspect				(int GItemID);
	static NetInspectStruct&			GetInspectInfo()								{ return s_inspectInfo; }
	static ItemFieldStruct*				GetInspectItemFieldStruct(int index);

//	virtual void R_SystemMessage		( SystemMessageENUM Msg );
//	virtual void R_SystemMessageData	( SystemMessageDataENUM Msg , int Size , char* Data );

	//Casting Bar
	virtual void R_BeginCastingResult	( CastingTypeENUM Type , int ItemID , int CastingTime , bool Result );
	virtual void R_InterruptCasting		( CastingTypeENUM Type );
	virtual void R_CastingOK			( CastingTypeENUM Type );
//	virtual void R_ItemExchangeResult	( ItemExchangeResultTypeENUM ResultType , ItemFieldStruct& Item , int Money , int Money_Account );
	virtual void R_RangeDataTransfer	( int DataSize , const char* Data );
	virtual void R_CtoC_Emote           ( PG_CtoC_Emote* pEmoteInfo );
	virtual void R_ColoringShopResult	( ColoringShopResultENUM Result );
	virtual void R_PlayerInfoOK			( int PlayerGItemID , EQStruct& EQ , const char* Note );
	virtual void R_PlayerInfoFailed		( int PlayerGItemID );
	virtual void R_SysKeyFunctionResult	( bool Result , int KeyID , int TargetGUID );
	virtual void R_ObjectPosInfo		( int Count , ObjectPosInfoStruct Info[6000] );
	virtual void R_OpenClientMenu		( ClientMenuType_ENUM MenuType );
	virtual	void R_PlayerTarget			( int PlayerGItemID, int TargetGItemID );
	virtual void R_RunGlobalPlotResult	( bool Result );
	virtual void R_HateListResult		( int NpcGUID , NPCHateList& NPCHate );
	virtual void R_OpenGiveItemPlo		( bool IsSort , int Count , int TargetGUID );
	virtual void R_CheckPasswordResult	( char* Password , bool Result );	
	virtual void R_Find_DBID_RoleName	( int DBID , char* RoleName , bool IsFind );
	virtual void R_CampID				( int GItemID , CampID_ENUM CampID );
	virtual void R_GoodEvilValue		( int PlayerGUID , float GoodEvilValue , GoodEvilTypeENUM GoodEvilType );
	virtual void R_ExchangeMoneyTable	( int Count , int DataSize , char* Data );
	virtual void R_DBStringTable		( int Count , int DataSize , char* Data );
	virtual void R_ColoringHorseResult( ColoringShopResultENUM Result );
	virtual void R_PKDeadDropItem( ItemFieldStruct& Item );
	virtual void R_PlayerEvent( PlayerEventTypeENUM Type );
	virtual void R_ClientClock( int BeginTime , int NowTime , int EndTime , int Type );
	virtual void R_AccountMoneyLockInfo ( int LockAccountMoney[ Def_AccountMoneyLockTime_Max_Count_ ] , int LockAccount_Forever );
	virtual void R_AccountMoneyTranError( );	
	virtual void RL_SetCycleMagic( int MagicID , int MagicPos );
	//////////////////////////////////////////////////////////////////////////
	//��ܬY����
	static void Map_ShowIcon( ObjectPosStruct Info );

	//�M���p�a�ϤW�Ҧ�������
	static void Map_ClearAllObjectPoint	( );

	//�]�w�Y�@��ID���������
	static void Map_SetObject_ID			( int OrgObjID );


	//�]�w�Y�@�������������
	static void Map_SetObject_Type			( int Type );

	//�]�w�C��ε���
	static void Map_ResetPointColor_Lv		( );





	//���oFindDBIDRoleNameStruct ��䤣�s�b�� �|�o�ʥ]�Vserver�n
	static FindDBIDRoleNameStruct* GetFindDBIDRoleNameStruct( const char* RoleName );
 	static FindDBIDRoleNameStruct* GetFindDBIDRoleNameStruct( int DBID );

	//�Ψ���W�٨��oDBID �䤣��Ǧ^-1
	static int GetDBID( const char* RoleName );
 	//��DBID���o����W�� �䤣��Ǧ^ ""
	static const char* GetRoleName( int DBID );
 
	static void InsertDBIDRoleName( int DBID, const char* RoleName );

	static void AddFindDBIDRoleNameWork( IFindDBIDRoleNameWorkBase* pWork );

	//�n�D�}�Ȧ�
	static void S_OpenBank();

	//�n�D�}AC
 	static void S_OpenAuction();

	//�n�D�}�H�c
  	static void S_OpenMail();

	static void SL_AutoReportPossibleCheater( AutoReportBaseInfoStruct& TargetInfo , AutoReportPossibleCheaterENUM Type, int DBID );

	//�R��¾�~
	virtual void RL_DelJob( Voc_ENUM Job );

	//�b������w���

	//////////////////////////////////////////////////////////////////////////
	//�p�C������
	virtual void RL_SetSmallGameMenuType	( int MenuID , SmallGameMenuTypeENUM Type  );
	virtual void RL_SetSmallGameMenuValue	( int MenuID , int ID , int Value );
	virtual void RL_SetSmallGameMenuStr		( int MenuID , int ID , const char* Content );
	//////////////////////////////////////////////////////////////////////////
	virtual void RL_ResetInstanceResult		( bool Result );
	virtual void RL_ResetInstanceInfo		( int ZoneID , bool IsReset );
	virtual void RL_PartyDice				( int PlayerDBID , int Type , int Rand );	
	//////////////////////////////////////////////////////////////////////////
	virtual void RL_BodyShopResult( bool Result );
	//////////////////////////////////////////////////////////////////////////
	virtual void RL_MoneyVendorReport		( int Count );
	virtual void RL_BotReport				( int Count );
	virtual void RL_UnlockBotReport			( const char* Name , UnlockBotTypeENUM Type );

	//////////////////////////////////////////////////////////////////////////
	virtual void RL_SortBankEnd( );

	//////////////////////////////////////////////////////////////////////////
	virtual void RD_LoadClientDataResult( int KeyID , int Size , char* Data  );
	virtual void RL_SaveClientDataResult( int KeyID , bool Result  );

	virtual void RD_LoadClientDataResult_Account( int KeyID , int Size , char* Data  );
	virtual void RM_SaveClientDataResult_Account( int KeyID , bool Result  );

	//////////////////////////////////////////////////////////////////////////
	//�]�wMap �� Icon & Tips
	virtual void RL_SetMapMark		( MapMarkInfoStruct& Info );
	virtual void RL_ClsMapMark		( int MarkID );

	virtual void RL_SetRoomValue	( int KeyID , int Value );
	virtual void RL_ClsRoomValue	( int KeyID );

	virtual void RL_AddExSkill( ExSkillBaseStruct&	Info );
	virtual void RL_DelExSkill( ExSkillBaseStruct&	Info );
	virtual void RL_SpellExSkillResult( bool Result , RoleErrorMsgENUM Msg );

	virtual void RL_BaseObjValueChange( int GItemID , ObjValueChangeENUM Type , const char* ValueStr , int Value );
	virtual void RL_TransferData_Range( int GItemID , char Data[20] );
	//////////////////////////////////////////////////////////////////////////
	//lua Ĳ�o��client �ݨƥ�
	virtual void RL_LuaClientEvent( int GItemID , int EventType , const char* Content );
	//////////////////////////////////////////////////////////////////////////
	virtual void RL_SetRoleValueResult	( bool Result );
	virtual void RL_ZoneOpen			( int ZoneID );
	//////////////////////////////////////////////////////////////////////////
	virtual void RL_SetNewTitleSysResult( bool Result );
	virtual void RL_NewTitleSys_UseItemResult( NewTitleSysUseItemResultENUM Result );	

	//////////////////////////////////////////////////////////////////////////
	virtual void RL_OpenSessionResult	( SessionTypeENUM Type, int SessionID, int CBID );
	virtual void RL_FixAccountFlag( StaticFlag< _ACCOUNT_FLAG_COUNT_ >& AccountFlag );
	//////////////////////////////////////////////////////////////////////////
	//���Ѫ��ƥ�
	virtual void RL_WeekInstances( int day , int count , WeekZoneStruct info[] );
	//�n�D���G
	virtual void RL_WeekInstancesTeleportResult( bool result );
	//���m�ƥ��ǰe���Ƶ��G
	virtual void RL_WeekInstancesReset( bool result );
	//�ƥ��ѤU���ͦs�ɶ�
	virtual void RL_WeekInstancesLiveTime( int sec );
	//�}����@�ɰƥ�����
	virtual void RL_OpenWeekInstancesFrame(  );
	//////////////////////////////////////////////////////////////////////////
	//�]�F�ɶ����G
	virtual void RL_PhantomRankUpResult( bool summon, bool result );
	//�]�F�ɯŵ��G
	virtual void RL_PhantomLevelUpResult( bool result );
	//�]�F�˳Ƶ��G
	virtual void RL_PhantomEQResult( bool result );
	//�ץ��]�F�˳�
	virtual void RL_FixPhantomEQ( int eqeQ[2] );
	//�ץ��]�F���
	virtual void RL_FixPhantom( int phantomID , PhantomBase& info );
	//�}�Ҥ��F����
	virtual void RL_OpenPhantomFrame(  );
	//////////////////////////////////////////////////////////////////////////
	//��x�]���_�����
	virtual void RL_ItemCombinInfo( int groupID , ItemCombineStruct& itemCombine );
	//��x�]���_������ID
	virtual void RL_ItemCombinGroupID( int groupID );
};

