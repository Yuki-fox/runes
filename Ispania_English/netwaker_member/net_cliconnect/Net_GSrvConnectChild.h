#pragma once

#include "Net_GSrvConnect.h"

class Net_GSrvConnectChild : public Net_GSrvConnect
{
public:
	static bool _Init();
	static bool _Release();

	static ZoneInfoModeStruct		s_zoneMode;			// 區域資料
	static int	CheckLoginBattleGroundCount;

	virtual	void	PlayerItemID( int GItemID, int WorldGUID , int DBID );
	virtual	void	ZoneInfo( int GSrvNetID , int ZoneID , int GSrvID , int MapID , int CampObjID , LanguageTypeENUM LanguageType , ZoneInfoModeStruct ZoneMode, int teamNumPeople );
	virtual	void	PlayerData( BaseRoleData *BaseRole , PlayerBaseRoleData *PlayerBaseRole , SysBaseRoleData* Base );
	virtual	void	PrepareChangeZone();
	virtual	void	ChangeZoneOK();
	virtual	void	R_ModyNPC( ModifyNPCInfo*  Info );

	virtual void	R_SysVersionInfo		( int LearnMagicID , SystemVersionENUM Version , float PK_DotRate );
	//回應 npc 路徑資訊
	virtual void	R_GetNPCMoveInfoResult	( int GItemID , vector<NPC_MoveBaseStruct>& List );

	virtual void	R_GetFlagListResult		( int GitemID, vector< vector < CliFlagPosInfo > >& vecFlagList );
	virtual void	R_EditFlagResult		( int GitemID, int iResult );
	virtual void	R_DelFlagResult			( int GitemID, int iResult );
	virtual void	R_DelFlagGroupResult	( int GitemID, int iResult );
	virtual void	R_LogoutResult			( bool Result );
	virtual void	R_ParalledID			( int ParalledID );
	virtual void	R_SkyProcType			( ClientSkyProcTypeENUM Type );
	virtual void	R_TouchTomb				( ) ;
	virtual void	R_TombInfo				( int CreateTime , int ZoneID , float X , float Y , float Z , int DebtExp , int DebtTp , int Exp );
	//////////////////////////////////////////////////////////////////////////
	virtual void	R_ConnectToBattleWorld( char* IPStr , int Port  );
	virtual void	OnLoginBattleGround		( );
	virtual void	OnLogoutBattleGround	( );
	virtual void	R_WorldReturnNotify		( int ZoneID );
	//////////////////////////////////////////////////////////////////////////
	virtual void	RL_ZoneOpenState		( int ZoneID , bool IsOpen );
};



