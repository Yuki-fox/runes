#pragma once

#include <map>
#include <string>
#include <Windows.h>


#include <string>
#include "../../mainproc/RuneDev.h"			// for CRoleSprite*

#include "PG/PG_BattleGround.h"
#include "../../MainProc/NetGlobal.h"

struct structBattleGroundResult
{
	int											iSrvID;
	int											iRoomID;
	StaticString< _MAX_OBJ_NAME_STR_SIZE_ >		szName;
};

class CNetCli_BattleGround:public CNetGlobal
{
public:
	CNetCli_BattleGround(void);
	~CNetCli_BattleGround(void);

	// 封包註冊
	//-----------------------------------------------------------
	static void _PG_BattleGround_LtoC_GetBattleGroundResult			( int iSrvID , int iSize , PVOID pData );
	static void _PG_BattleGround_LtoC_EnterBattleGroundRight		( int iSrvID , int iSize , PVOID pData );
	static void _PG_BattleGround_LtoC_NotifyEnterBattleGroundRight	( int iSrvID , int iSize , PVOID pData );
	static void _PG_BattleGround_LtoC_LostEnterBattleGroundRight	( int iSrvID , int iSize , PVOID pData );
	static void _PG_BattleGround_LtoC_EmptyBattleGround				( int iSrvID , int iSize , PVOID pData );
	static void _PG_BattleGround_LtoC_JoinQueueResult				( int iSrvID , int iSize , PVOID pData );
	static void _PG_BattleGround_LtoC_BattleGroundClose				( int iSrvID , int iSize , PVOID pData );
	static void _PG_BattleGround_LtoC_BattleGroundStatus			( int iSrvID , int iSize , PVOID pData );
	static void _PG_BattleGround_LtoC_BattleGroundLandMark			( int iSrvID , int iSize , PVOID pData );
	static void _PG_BattleGround_LtoC_SetBattleGroundData			( int iSrvID , int iSize , PVOID pData );
	static void _PG_BattleGround_LtoC_BattleGroundScoreInfo			( int iSrvID , int iSize , PVOID pData );
	static void _PG_BattleGround_LtoC_BattleGroundWaitQueueStatus	( int iSrvID , int iSize , PVOID pData );
	static void _PG_BattleGround_LtoC_AllLandMarkInfo				( int iSrvID , int iSize , PVOID pData );
	static void _PG_BattleGround_LtoC_AllBattleGroundPlayerPos		( int iSrvID , int iSize , PVOID pData );
	static void	_PG_BattleGround_LtoC_UpdateArenaData				( int iCliID , int iSize , PVOID pData );
	static void	_PG_BattleGround_LtoC_GetRankPersonalInfoResult		( int iCliID , int iSize , PVOID pData );


	//-----------------------------------------------------------
	static bool			Init								();
	static bool			Release								();
	static void			Update								();

	static void			GetBattleGroundList					( int iDBID );

	static void			GetBattleGround						( int iObjID );
	static void			JoinBattleGround					( int iObjID, int iRoomID, int iParty, int iBattleGroundType, int* pTeamDBID );
	static void			EnterBattleGround					( int iBattleGroundType, int iRoomID, int iTeamID );
	static void			LeaveBattleGround					();															// 離開戰場
	static void			LeaveBattleGroundWaitQueue			( int iDBID, int iType );						// 離開等待進入戰場的佇列
	static void			GetBattleGroundScoreInfo			();
	static void			BattleGroundWaitQueueStatusRespond	( int iSrvID, int iDBID );

	static void			LeaveBattleGroundRemote				( int iDBID, int iType );


	virtual void		OnRZ_GetBattleGroundResult			( int iSrvID, int iBGType, int iCount, vector< structBattleGroundResult > vecBG ) = 0;
	virtual void		OnRZ_EnterBattleGroundRight			( int iSrvID, int iType, int iRoomID, int iTeamID, int iWaitTime, const char* pszName ) = 0;
	virtual void		OnRZ_NotifyEnterBattleGroundRight	( int iSrvID, int iType, int iRoomID, int iWaitTime, const char* pszName ) = 0;
	virtual void		OnRZ_LostEnterBattleGroundRight		( int iSrvID, int iType, int iRoomID, int iWaitTime, const char* pszName ) = 0;
	
	virtual void		OnRZ_EmptyBattleGround				( int iSrvID, int iType, int iRoomID ) = 0;
	virtual void		OnRZ_JoinQueueResult				( int iSrvID, int iType, int iRoomID, int iTeamID, int iWaitTime, const char* pszName, int iResult) = 0;
	virtual void		OnRZ_BattleGroundClose				( int iSrvID, int iWinTeam ) = 0;
	virtual void		OnRZ_BattleGroundStatus				( int iSrvID, StructBattleGroundValue* pData ) = 0;
	virtual void		OnRZ_BattleGroundLandMark			( int iSrvID, int iID, StructLandMark* pLandMark ) = 0;
	virtual void		OnRZ_SetBattleGroundData			( int iSrvID, int iForceFlagID, int iCampID, int iRoomID ) = 0;
	virtual	void		OnRZ_BattleGroundScoreInfo			( int iSrvID, int iTotalRole, StructBattleGroundScore* pScoreArray ) = 0;
	virtual	void		OnRZ_BattleGroundWaitQueueStatus	( int iSrvID, int iType, int iStatus, int iWaitID, int iSrvCheckTime) = 0;
	virtual	void		OnRZ_AllLandMarkInfo				( int iSrvID, StructRoomLandMark* pInfo ) = 0;
	virtual	void		OnRZ_AllBattleGroundPlayerPos		( int iSrvID, int iTotalRole, StructBattleGroundPlayerPos* pPosArray ) = 0;
	virtual void		OnRZ_UpdateArenaData				( int iSrvID, int iArenaType, int iTeamGUID, StructArenaInfo* pArenaInfo ) = 0;
	virtual void		OnRZ_GetRankPersonalInfoResult		( int iSrvID, int iDBID, int iRankPoint, int iLastRoundRank, int iRank, const char* pszNote ) = 0;



	//Rank system
	static	void		GetRankPersonalInfo					( int iDBID );


	StructBattleGroundValue							m_BattleGroundData;
	map< int, StructLandMark >						m_mapLandMark;

	static CNetCli_BattleGround*	m_pThis;
};

