#pragma once

#include "NetCli_BattleGround.h"

#define OnGetBattleGroundResultFunctionBase     boost::function<void ( int iSrvID, int iType, int iCount, vector< structBattleGroundResult >& vecBG) >
#define OnBattleGroundCloseFunctionBase			boost::function<void ( int iSrvID, int iWinTeam) >
#define OnBattleGroundStatusFunctionBase		boost::function<void ( int iSrvID, StructBattleGroundValue* pData) >

enum ENUM_BATTLEGROUNDQUEUESTATUS
{
	EM_BATTLEGROUNDSTATUS_NONE						= 0,		// It should never be none.	
	EM_BATTLEGROUNDSTATUS_IN_QUEUE_WAIT_ENTER		,
	EM_BATTLEGROUNDSTATUS_GOT_TICKET_WAIT_ENTER		,
	EM_BATTLEGROUNDSTATUS_GOT_TICKET_WAIT_ENTER_HIDE,
	EM_BATTLEGROUNDSTATUS_TICKET_SENT
};

enum ENUM_BATTLEGROUND_PLAYER_TEAM
{
	EM_BG_TEAM1,
	EM_BG_TEAM2,
	EM_BG_TEAM3,
	EM_BG_TEAM4,
	EM_BG_TEAM5,
	EM_BG_TEAM6,
	EM_BG_TEAM7,
	EM_BG_TEAM8
};

enum ENUM_BATTLEGROUND_TYPE
{
	EM_BG_TYPE_NONE					= 0,
	EM_BG_TYPE_HORSE_RACING			= 350,
	EM_BG_TYPE_1_VS_1_ARENA			= 410,
	EM_BG_TYPE_6_VS_6_ARENA			= 411,
	EM_BG_TYPE_3_VS_3_ARENA			= 430,
	EM_BG_TYPE_CTF_001_ARENA		= 431,
	EM_BG_TYPE_TOWER_DEFENSE_001	= 432,
	
	EM_BG_TYPE_W_HORSE_RACING		= 441,
	EM_BG_TYPE_W_1_VS_1_ARENA		= 440,
	EM_BG_TYPE_W_6_VS_6_ARENA		= 443,
	EM_BG_TYPE_W_3_VS_3_ARENA		= 442,
	EM_BG_TYPE_W_CTF_001_ARENA		= 444,
	EM_BG_TYPE_W_TOWER_DEFENSE_001	= 445,

};

struct StructBattleGroundQueueStatus
{
	ENUM_BATTLEGROUNDQUEUESTATUS	emQueueStatus;				// ���A
	int								iType;						// �Գ�����(ZoneID)
	DWORD							dwTimeToEnterQueue;			// �i�J��C���ɶ�
	DWORD							dwTimeExcepted;				// �w���i�J�Գ��ɶ�
	DWORD							dwTimeToLeaveQueue;			// �N�Q������C���ɶ�
	int								iSrvCheckTime;				// �W�@�����osever WaitQueueStatus���ɶ�
	int								iWaitID;					// �W�@�����osever WaitQueueStatus��QueueID
	int                             iRoomID;                    // (�w��oTicket���A) roomID
	int                             iTeamID;                    // (�w��oTicket���A) teamID
};

class CNetCli_BattleGround_Child : public CNetCli_BattleGround
{
public:
	static  void		UpdateBattleGroundMinimap			();

	static	bool		Init								();
	static	bool		Release								();
	static  void        OnLogOut							();

	static	int			GetBattleGroundPeopleLimit			();	// 0 �H��, 1 ����, 2 �h�H
	static  const char* GetBattleGroundName(int iType);

	virtual void		OnRZ_GetBattleGroundResult			( int iSrvID, int iType, int iCount, vector< structBattleGroundResult > vecBG );

	static void			GetBattleGroundResultWithRoomList	( int iSrvID, int iType, int iCount, vector< structBattleGroundResult >& vecBG);
	static void			GetBattleGroundResultWithoutRoomList( int iSrvID, int iType, int iCount, vector< structBattleGroundResult >& vecBG);

	virtual void		OnRZ_EnterBattleGroundRight			( int iSrvID, int iType, int iRoomID, int iTeamID, int iWaitTime, const char* pszName );
	virtual void		OnRZ_NotifyEnterBattleGroundRight	( int iSrvID, int iType, int iRoomID, int iWaitTime, const char* pszName );
	virtual void		OnRZ_LostEnterBattleGroundRight		( int iSrvID, int iType, int iRoomID, int iWaitTime, const char* pszName );
	
	virtual void		OnRZ_EmptyBattleGround				( int iSrvID, int iType, int iRoomID );
	virtual void		OnRZ_JoinQueueResult				( int iSrvID, int iType, int iRoomID, int iTeamID, int iWaitTime, const char* pszName, int iResult);

	virtual void		OnRZ_BattleGroundClose				( int iSrvID, int iWinTeam );

	virtual void		OnRZ_GetRankPersonalInfoResult		( int iSrvID, int iDBID, int iRankPoint, int iLastRoundRank, int iRank, const char* pszNote );

	static void			BattleGroundCloseHorseRacing		( int iSrvID, int iWinTeam );
	static void			BattleGroundCloseNormal				( int iSrvID, int iWinTeam );

	virtual void		OnRZ_BattleGroundStatus				( int iSrvID, StructBattleGroundValue* pData );

	static void			BattleGroundStatusHorseRacing		( int iSrvID, StructBattleGroundValue* pData );
	static void			BattleGroundStatusNormal			( int iSrvID, StructBattleGroundValue* pData );
	static void			BattleGroundTowerInfo				( int iSrvID, StructBattleGroundValue* pData );

	virtual void		OnRZ_BattleGroundLandMark			( int iSrvID, int iID, StructLandMark* pLandMark );
	virtual void		OnRZ_SetBattleGroundData			( int iSrvID, int iForceFlagID, int iCampID, int iRoomID );
	virtual	void		OnRZ_BattleGroundScoreInfo			( int iSrvID, int iTotalRole, StructBattleGroundScore* pScoreArray );
	virtual	void		OnRZ_BattleGroundWaitQueueStatus	( int iSrvID, int iType, int iStatus, int iWaitID, int iSrvCheckTime);
	virtual	void		OnRZ_AllLandMarkInfo				( int iSrvID, StructRoomLandMark* pInfo );
	virtual	void		OnRZ_AllBattleGroundPlayerPos		( int iSrvID, int iTotalRole, StructBattleGroundPlayerPos* pPosArray );
	virtual void		OnRZ_UpdateArenaData				( int iSrvID, int iArenaType, int iTeamGUID, StructArenaInfo* pArenaInfo );


	static map< int, StructBattleGroundQueueStatus >& GetQueseStatusMap();
	
	map< int, StructBattleGroundQueueStatus >			m_mapBattleGroundQueueStatus;
	map< string, StructBattleGroundPlayerPos >          m_mapBattleGroundPlayerPos;
	map< string, StructBattleGroundScore >				m_mapBattleGroundScores;
	map< int, string >									m_mapBattleGroundNames;
	map< int, OnGetBattleGroundResultFunctionBase >     m_mapGetBattleGroundResultFunctions;
	map< int, OnBattleGroundCloseFunctionBase >			m_mapBattleGroundCloseFunctions;
	map< int, OnBattleGroundStatusFunctionBase >		m_mapBattleGroundStatusFunctions;
};

