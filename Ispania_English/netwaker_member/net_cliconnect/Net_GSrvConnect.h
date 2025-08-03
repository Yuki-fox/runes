#ifndef __Net_GSrvConnect_H__
#define __Net_GSrvConnect_H__

#include "../../MainProc/NetGlobal.h"
#include "PG/PG_CliConnect.h"

#pragma unmanaged

struct CliFlagPosInfo
{
	int             GroupID;		//類別
	int             FlagID;         //編號
	float           X;
	float           Y;
	float           Z;    
	float			Dir;
	int				FlagDBID;	//旗子資料庫ID
};

class Net_GSrvConnect : public CNetGlobal
{
    
protected:
    //-------------------------------------------------------------------
    static Net_GSrvConnect* This;
    static bool _Init();
    static bool _Release();

    static void _PG_CliConnect_LtoC_PlayerItemID_Base	( int Sockid , int Size , void* Data );
    static void _PG_CliConnect_LtoC_PlayerItemID_Battle	( int Sockid , int Size , void* Data );
    static void _PG_CliConnect_LtoC_ZoneInfo			( int Sockid , int Size , void* Data );
    static void _PG_CliConnect_LtoC_PlayerData			( int Sockid , int Size , void* Data );
    static void _PG_CliConnect_LtoC_PrepareChangeZone	( int Sockid , int Size , void* Data );
    static void _PG_CliConnect_LtoC_ChangeZoneOK		( int Sockid , int Size , void* Data );

    static void _PG_CliConnect_LtoC_ModifyNPCInfo		( int Sockid , int Size , void* Data );
	static void _PG_CliConnect_LtoC_GetNPCMoveInfoResult( int Sockid , int Size , void* Data );
	
	static void _PG_CliConnect_LtoC_GetFlagListResult	( int Sockid , int Size , void* Data );
	static void _PG_CliConnect_LtoC_EditFlagResult		( int Sockid , int Size , void* Data );
	static void _PG_CliConnect_LtoC_DelFlagResult		( int Sockid , int Size , void* Data );
	static void _PG_CliConnect_LtoC_DelFlagGroupResult	( int Sockid , int Size , void* Data );
	static void _PG_CliConnect_LtoC_LogoutResult		( int Sockid , int Size , void* Data );
	static void _PG_CliConnect_LtoC_ParalledID			( int Sockid , int Size , void* Data );
	static void _PG_CliConnect_LtoC_PlayerData_Zip		( int Sockid , int Size , void* Data );
	static void _PG_CliConnect_LtoC_SysVersionInfo		( int Sockid , int Size , void* Data );

	static void _PG_CliConnect_LtoC_ConnectToBattleWorld( int Sockid , int Size , void* Data );
	static void _PG_CliConnect_LtoC_SkyProcType			( int Sockid , int Size , void* Data );
	static void _PG_CliConnect_LtoC_TouchTomb			( int Sockid , int Size , void* Data );
	static void _PG_CliConnect_LtoC_TombInfo			( int Sockid , int Size , void* Data );
	static void _PG_CliConnect_LtoC_WorldReturnNotify	( int Sockid , int Size , void* Data );
	static void _PG_CliConnect_LtoC_ZoneOpenState		( int Sockid , int Size , void* Data );
	static void _OnLoginBattleGround					( string sInfo );
	static void _OnLogoutBattleGround					( );
public:
    Net_GSrvConnect( ){ This = this; };
    //-------------------------------------------------------------------
    // 資料送出
    //-------------------------------------------------------------------
    static void Logout();

	//修改NPC資料
    static void ModyNPC( ModifyNPCInfo*  Info );

	//儲存NPC移動資料
	static void SaveNPCMoveInfo( int GItemID , vector<NPC_MoveBaseStruct>& List );

	static void	GetNPCMoveInfo			( int iNPCObjID );



	static void	GetFlagList				();
	static void	EditFlag				( CliFlagPosInfo Flag );
	static void DelFlag					( int iFlagGroupID, int iID );
	static void DelFlagGroup			( int iFlagGroupID );

    //-------------------------------------------------------------------
    //  事件繼承實做
    //-------------------------------------------------------------------
    virtual void PlayerItemID           ( int GItemID, int WorldGUID , int DBID ) = 0;
    virtual void ZoneInfo               ( int GSrvNetID , int ZoneID , int GSrvID , int MapID , int CampObjID , LanguageTypeENUM LanguageType , ZoneInfoModeStruct ZoneMode, int teamNumPeople ) = 0;
    virtual void PlayerData             ( BaseRoleData *BaseRole , PlayerBaseRoleData *PlayerBaseRole , SysBaseRoleData* Base) = 0;
	virtual void PrepareChangeZone      () = 0;
    virtual void ChangeZoneOK           () = 0;
    virtual void R_ModyNPC              ( ModifyNPCInfo*  Info ) = 0;

	virtual void R_SysVersionInfo		( int LearnMagicID , SystemVersionENUM Version , float PK_DotRate ) = 0;

	//回應 npc 路徑資訊
	virtual void R_GetNPCMoveInfoResult	( int GItemID , vector<NPC_MoveBaseStruct>& List ) = 0;

	virtual void R_GetFlagListResult	( int GitemID, vector< vector < CliFlagPosInfo > >& vecFlagList ) = 0;
	virtual void R_EditFlagResult		( int GitemID, int iResult ) = 0;
	virtual void R_DelFlagResult		( int GitemID, int iResult ) = 0;
	virtual void R_DelFlagGroupResult	( int GitemID, int iResult ) = 0;
	virtual void R_LogoutResult			( bool Result ) = 0;
	virtual void R_ParalledID			( int ParalledID ) = 0;

	virtual void R_SkyProcType			( ClientSkyProcTypeENUM Type ) = 0;
	virtual void R_TouchTomb			( ) = 0;
	virtual void R_TombInfo				( int CreateTime , int ZoneID , float X , float Y , float Z , int DebtExp , int DebtTp , int Exp ) = 0;
	//////////////////////////////////////////////////////////////////////////
	//聯結其他世界
	//////////////////////////////////////////////////////////////////////////
	virtual void R_ConnectToBattleWorld( char* IPStr , int Port  ) = 0;
	static void S_ConnectToBattleWorldOK( int CliNetID );
	virtual void OnLoginBattleGround( ) = 0;
	virtual void OnLogoutBattleGround( ) = 0;
	virtual void R_WorldReturnNotify( int ZoneID ) = 0;
	static void S_WorldReturnOK( );
	//////////////////////////////////////////////////////////////////////////
	virtual void RL_ZoneOpenState		( int ZoneID , bool IsOpen ) = 0;
};

#pragma managed
#endif //__Net_GSrvConnect_H__