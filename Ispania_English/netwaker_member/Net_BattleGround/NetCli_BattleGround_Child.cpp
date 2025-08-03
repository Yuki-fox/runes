#include "../../mainproc/GameMain.h"
#include "NetCli_BattleGround_Child.h"
#include "../../Interface/WorldFrames/ChatFrame.h"
#include "../../Interface/WorldFrames/BattleGroundStatusFrame.h"
#include "../../Interface/WorldFrames/BattleGroundScoreFrame.h"
#include "../../Interface/WorldFrames/BattleGroundOptionButtonFrame.h"
#include "../../Interface/WorldFrames/BattleGroundRoomListFrame.h"
#include "../../Interface/WorldFrames/BattleGroundAreneScoreFrame.h"
#include "../../Interface/WorldFrames/HorseRacingRankingFrame.h"
#include "../../Interface/WorldFrames/BattleGroundQueueFrame.h"
#include "../../Interface/WorldFrames/BattleGroundBloodStatusFrame.h"
#include "../../Interface/Map/WorldMap.h"
#include "../../netwaker_member/net_party/NetCli_PartyChild.h"
#include "../Net_Other/NetCli_OtherChild.h"
#include "../Net_Script/NetCli_Script_Child.h"

struct StructEnterBattleGroundContext
{
	int		iSrvID;
	int		iType;
	int		iRoomID;
	int		iTeamID;
};

// --------------------------------------------------------------------------------------
bool CNetCli_BattleGround_Child::Init()
{	
	if( m_pThis == NULL )
		m_pThis = new CNetCli_BattleGround_Child;

	CNetCli_BattleGround::Init();

	CNetCli_BattleGround_Child* _TmpThis = (CNetCli_BattleGround_Child*) m_pThis;

	_TmpThis->m_mapBattleGroundNames[EM_BG_TYPE_NONE]					= "";
	_TmpThis->m_mapBattleGroundNames[EM_BG_TYPE_HORSE_RACING]			= g_ObjectData->GetString("ZONE_EVENTDGN_RACETRACK_01");
	_TmpThis->m_mapBattleGroundNames[EM_BG_TYPE_1_VS_1_ARENA]			= g_ObjectData->GetString("ZONE_BG_COLISEUM_1V1_001");
	_TmpThis->m_mapBattleGroundNames[EM_BG_TYPE_3_VS_3_ARENA]			= g_ObjectData->GetString("ZONE_BG_COLISEUM_3V3_001");
	_TmpThis->m_mapBattleGroundNames[EM_BG_TYPE_6_VS_6_ARENA]			= g_ObjectData->GetString("ZONE_BG_COLISEUM_6V6_001");
	_TmpThis->m_mapBattleGroundNames[EM_BG_TYPE_CTF_001_ARENA]			= g_ObjectData->GetString("ZONE_BG_CTF_001");
	_TmpThis->m_mapBattleGroundNames[EM_BG_TYPE_TOWER_DEFENSE_001]		= g_ObjectData->GetString("ZONE_BG_TOWER_DEFENSE_001");

	_TmpThis->m_mapBattleGroundNames[EM_BG_TYPE_W_HORSE_RACING]			= g_ObjectData->GetString("ZONE_EVENTDGN_RACETRACK_01");
	_TmpThis->m_mapBattleGroundNames[EM_BG_TYPE_W_1_VS_1_ARENA]			= g_ObjectData->GetString("ZONE_BG_COLISEUM_1V1_001");
	_TmpThis->m_mapBattleGroundNames[EM_BG_TYPE_W_3_VS_3_ARENA]			= g_ObjectData->GetString("ZONE_BG_COLISEUM_3V3_001");
	_TmpThis->m_mapBattleGroundNames[EM_BG_TYPE_W_6_VS_6_ARENA]			= g_ObjectData->GetString("ZONE_BG_COLISEUM_6V6_001");
	_TmpThis->m_mapBattleGroundNames[EM_BG_TYPE_W_CTF_001_ARENA]		= g_ObjectData->GetString("ZONE_BG_CTF_001");
	_TmpThis->m_mapBattleGroundNames[EM_BG_TYPE_W_TOWER_DEFENSE_001]	= g_ObjectData->GetString("ZONE_BG_TOWER_DEFENSE_001");



	_TmpThis->m_mapGetBattleGroundResultFunctions.clear();
	_TmpThis->m_mapGetBattleGroundResultFunctions[EM_BG_TYPE_NONE]				= GetBattleGroundResultWithoutRoomList;
	_TmpThis->m_mapGetBattleGroundResultFunctions[EM_BG_TYPE_HORSE_RACING]		= GetBattleGroundResultWithoutRoomList;
	_TmpThis->m_mapGetBattleGroundResultFunctions[EM_BG_TYPE_W_HORSE_RACING]	= GetBattleGroundResultWithoutRoomList;
	//_TmpThis->m_mapGetBattleGroundResultFunctions[EM_BG_TYPE_1_VS_1_ARENA] = GetBattleGroundResultWithoutRoomList;
	//_TmpThis->m_mapGetBattleGroundResultFunctions[EM_BG_TYPE_3_VS_3_ARENA] = GetBattleGroundResultWithoutRoomList;

	_TmpThis->m_mapBattleGroundCloseFunctions.clear();
	_TmpThis->m_mapBattleGroundCloseFunctions[EM_BG_TYPE_NONE]					= BattleGroundCloseNormal;
	_TmpThis->m_mapBattleGroundCloseFunctions[EM_BG_TYPE_HORSE_RACING]			= BattleGroundCloseHorseRacing;
	_TmpThis->m_mapBattleGroundCloseFunctions[EM_BG_TYPE_W_HORSE_RACING]		= BattleGroundCloseHorseRacing;

	_TmpThis->m_mapBattleGroundStatusFunctions.clear();
	_TmpThis->m_mapBattleGroundStatusFunctions[EM_BG_TYPE_NONE]					= BattleGroundStatusNormal;
	_TmpThis->m_mapBattleGroundStatusFunctions[EM_BG_TYPE_HORSE_RACING]			= BattleGroundStatusHorseRacing;
	_TmpThis->m_mapBattleGroundStatusFunctions[EM_BG_TYPE_TOWER_DEFENSE_001]	= BattleGroundTowerInfo;

	_TmpThis->m_mapBattleGroundStatusFunctions[EM_BG_TYPE_W_HORSE_RACING]		= BattleGroundStatusHorseRacing;
	_TmpThis->m_mapBattleGroundStatusFunctions[EM_BG_TYPE_W_TOWER_DEFENSE_001]	= BattleGroundTowerInfo;

	return true;
}
// --------------------------------------------------------------------------------------
bool CNetCli_BattleGround_Child::Release()
{
	CNetCli_BattleGround::Release();
	SAFE_DELETE( m_pThis );
	return true;
}

void CNetCli_BattleGround_Child::OnLogOut()
{
	CNetCli_BattleGround_Child* _This = (CNetCli_BattleGround_Child*) m_pThis;
	_This->m_mapBattleGroundQueueStatus.clear();
}


void CNetCli_BattleGround_Child::UpdateBattleGroundMinimap()
{
	CNetCli_BattleGround_Child* _This = (CNetCli_BattleGround_Child*) m_pThis;

	int iTotalLandMark = (int)_This->m_mapLandMark.size();

	int iType = g_pGameMain->GetBattleGroundType();

	if (iType > 0)
	{
		g_pWorldMapFrame->ClearMapIcon( iType );

		for( map< int, StructLandMark >::iterator it = _This->m_mapLandMark.begin(); it != _This->m_mapLandMark.end(); it++ )
		{
			StructLandMark*		pLandMark	= &( it->second );
			int					iID			= it->first;
			std::string			strText		= CNetCli_Script_Child::ReplaceSystemKeyword( pLandMark->Name.Begin() );

			g_pWorldMapFrame->AddMapIcon( 
				iType,
				iType,
				pLandMark->fX,
				pLandMark->fZ,
				strText.c_str(),
				_This->m_BattleGroundData.iLandMark[ iID ]	);
		}

		for (map< string, StructBattleGroundPlayerPos>::iterator itPos = _This->m_mapBattleGroundPlayerPos.begin(); itPos != _This->m_mapBattleGroundPlayerPos.end(); itPos++)
		{
			StructBattleGroundPlayerPos* pPos = &( itPos->second );
			string sName = itPos->first;

			int iTeamID = _This->m_mapBattleGroundScores[sName.c_str()].iTeamID;
			int iDBID = _This->m_mapBattleGroundScores[sName.c_str()].iDBID;

			if (iDBID == ( g_pGameMain->GetDBID() % _DEF_MAX_DBID_COUNT_ ) ) //玩家本身
				continue;

			int iIconType = 0;

			switch (iTeamID)
			{
			case EM_BG_TEAM1:
				iIconType = EM_MapIconType_BGTeam1;
				break;
			case EM_BG_TEAM2:
				iIconType = EM_MapIconType_BGTeam2;
				break;
			case EM_BG_TEAM3:
				iIconType = EM_MapIconType_BGTeam3;
				break;
			case EM_BG_TEAM4:
				iIconType = EM_MapIconType_BGTeam4;
				break;
			case EM_BG_TEAM5:
				iIconType = EM_MapIconType_BGTeam5;
				break;
			case EM_BG_TEAM6:
				iIconType = EM_MapIconType_BGTeam6;
				break;
			case EM_BG_TEAM7:
				iIconType = EM_MapIconType_BGTeam7;
				break;
			case EM_BG_TEAM8:
				iIconType = EM_MapIconType_BGTeam8;
				break;
			}

			g_pWorldMapFrame->AddMapIcon( 
				iType,
				iType,
				pPos->Pos.X,
				pPos->Pos.Z,
				sName.c_str(),
				iIconType);
		}
	}
}

// --------------------------------------------------------------------------------------
void CNetCli_BattleGround_Child::OnRZ_GetBattleGroundResult	( int iSrvID, int iType, int iCount, vector< structBattleGroundResult > vecBG )
{
	/*if (iType == EM_BG_TYPE_HORSE_RACING)
	{
		if(g_pBattleGroundQueueFrame->FlagOfOpenInterface != true)
		{
			CRoleSprite* target = g_pGameMain->GetPlayerTarget();
			RoleDataEx* role = CNetGlobal::RoleData();
			if (role != NULL)
			{
				if (target != NULL)
				{
					JoinBattleGround( target->GetIdentify(), 0, -1, iType, NULL );
				}
			}
		}
		g_pBattleGroundQueueFrame->FlagOfOpenInterface = false;
	}*/
	//if(iType != EM_BG_TYPE_HORSE_RACING)
	//{
		g_pBattleGroundQueueFrame->GetBattleGroundResult(iType);
	//}
}

void CNetCli_BattleGround_Child::GetBattleGroundResultWithRoomList(int iSrvID, int iType, int iCount, vector< structBattleGroundResult >& vecBG)
{
	g_pBattleGroundRoomListFrame->UpdateRoomList(iType, vecBG);
	g_pBattleGroundRoomListFrame->Open();
}

void CNetCli_BattleGround_Child::GetBattleGroundResultWithoutRoomList(int iSrvID, int iType, int iCount, vector< structBattleGroundResult >& vecBG)
{
	CRoleSprite* target = g_pGameMain->GetPlayerTarget();
	RoleDataEx* role = CNetGlobal::RoleData();
	if (role != NULL)
	{
		if (target != NULL)
		{
			//JoinBattleGround( target->GetIdentify(), 0, role->PartyID() );
			JoinBattleGround( target->GetIdentify(), 0, -1, iType, NULL );
		}
		else
		{
			//JoinBattleGround( 0, 0, role->PartyID() );
			JoinBattleGround( 0, 0, -1, iType, NULL );
		}			
	}	
}

void CNetCli_BattleGround_Child::OnRZ_EnterBattleGroundRight	( int iSrvID, int iType, int iRoomID, int iTeamID, int iWaitTime, const char* pszName )
{
	map< int, StructBattleGroundQueueStatus >::iterator it = m_mapBattleGroundQueueStatus.find(iType);


	// 檢查 Type, 如果有自動加入, 且不在隊伍中的話, 則直接送給 Server 加入
	// 如果有隊伍, 則加入介面中, 隊長則自動加入
// 	if( iType == 193 )
// 	{
// 		EnterBattleGround( iType, iRoomID, iTeamID );
// 		return;
// 	}


	if (it != m_mapBattleGroundQueueStatus.end())
	{
		//改變指定戰場Queue狀態為獲得Ticket
		StructBattleGroundQueueStatus& status = m_mapBattleGroundQueueStatus[iType];
		status.emQueueStatus     = EM_BATTLEGROUNDSTATUS_GOT_TICKET_WAIT_ENTER;
		status.dwTimeToEnterQueue = timeGetTime();
		status.dwTimeToLeaveQueue = status.dwTimeToEnterQueue + iWaitTime;
		status.iRoomID           = iRoomID;
		status.iTeamID           = iTeamID;
	}
	else
	{
		//新增一筆狀態
		StructBattleGroundQueueStatus newStatus;
		SecureZeroMemory(&newStatus, sizeof(StructBattleGroundQueueStatus));
		newStatus.emQueueStatus	    = EM_BATTLEGROUNDSTATUS_GOT_TICKET_WAIT_ENTER;
		newStatus.iType				= iType;
		newStatus.dwTimeToEnterQueue = timeGetTime();
		newStatus.dwTimeToLeaveQueue = newStatus.dwTimeToEnterQueue + iWaitTime;
		newStatus.iRoomID           = iRoomID;
		newStatus.iTeamID           = iTeamID;

		m_mapBattleGroundQueueStatus[iType] = newStatus;
	}

	//彈出通知進入戰場介面 
	g_pBattleGroundOptionButtonFrame->ShowEnterBGQueryDialog(iType, iRoomID);	
}

// --------------------------------------------------------------------------------------
void CNetCli_BattleGround_Child::OnRZ_NotifyEnterBattleGroundRight	( int iSrvID, int iType, int iRoomID, int iWaitTime, const char* pszName )
{
	map< int, StructBattleGroundQueueStatus >::iterator it = m_mapBattleGroundQueueStatus.find(iType);
	
	if (it != m_mapBattleGroundQueueStatus.end())
	{
		switch (it->second.emQueueStatus)
		{
		case EM_BATTLEGROUNDSTATUS_GOT_TICKET_WAIT_ENTER:
			{
				//彈出通知進入戰場介面 
				g_pBattleGroundOptionButtonFrame->ShowEnterBGQueryDialog(iType, iRoomID);
				break;
			}

		case EM_BATTLEGROUNDSTATUS_GOT_TICKET_WAIT_ENTER_HIDE:
			{
				g_pBattleGroundOptionButtonFrame->StartFlashBGOptionButton();
				break;
			}
		}
	}
}
// --------------------------------------------------------------------------------------
void CNetCli_BattleGround_Child::OnRZ_LostEnterBattleGroundRight( int iSrvID, int iType, int iRoomID, int iWaitTime, const char* pszName )
{
	map< int, StructBattleGroundQueueStatus >::iterator it = m_mapBattleGroundQueueStatus.find(iType);

	if (it != m_mapBattleGroundQueueStatus.end())
	{
		//從狀態Queue中移除
		m_mapBattleGroundQueueStatus.erase(it);

		char Msg[256];
		if((iType % 1000) == 190)
		{
			CNetCli_BattleGround::LeaveBattleGroundRemote(g_pGameMain->GetDBID(), iType);
			NetCli_PartyChild::KickMember(g_pGameMain->GetPlayerName());
			sprintf(Msg, g_ObjectData->GetString("BG_MSG_LEAVE_EVENT_ZONE_GROUP"), GetBattleGroundName(iType));

			map< int, StructBattleGroundQueueStatus >::iterator it = m_mapBattleGroundQueueStatus.find(iType);
			if (it != m_mapBattleGroundQueueStatus.end())
			{
				//從狀態Queue中移除
				m_mapBattleGroundQueueStatus.erase(it);
			}
		}
		else
		{
			sprintf(Msg, g_ObjectData->GetString("BG_MSG_LOST_ENTER_BG_RIGHT"), GetBattleGroundName(iType));
		}

		g_pGameMain->SendSystemMsg(Msg);
		g_pChatFrame->SendMsgEvent( CHAT_MSG_SYSTEM, Msg, "" );
	}
}
// --------------------------------------------------------------------------------------
void CNetCli_BattleGround_Child::OnRZ_EmptyBattleGround( int iSrvID, int iType, int iRoomID )
{
	//通知該戰場已消失, 詢問是否要重新選擇
	//...

	map< int, StructBattleGroundQueueStatus >::iterator it = m_mapBattleGroundQueueStatus.find(iType);

	if (it != m_mapBattleGroundQueueStatus.end())
	{
		//從狀態Queue中移除
		m_mapBattleGroundQueueStatus.erase(it);
	}
}

// --------------------------------------------------------------------------------------
void CNetCli_BattleGround_Child::OnRZ_JoinQueueResult( int iSrvID, int iType, int iRoomID, int iTeamID, int iWaitTime, const char* pszName, int iResult)
{
	char Msg[256];

	switch (iResult)
	{
	case EM_BG_JOINQUEUE_RESULT_OK: 
		break;

	case EM_BG_JOINQUEUE_RESULT_INBG:// 隊員或自己已在佇列中
		{
			if((iType % 1000) > 400)
			{
				sprintf(Msg, g_ObjectData->GetString("BG_MSG_JOINQUEUE_RESULT_INBG"));

				g_pGameMain->SendSystemMsg(Msg);
				g_pChatFrame->SendMsgEvent( CHAT_MSG_SYSTEM, Msg, "" );
			}
			break;
		}

	case EM_BG_JOINQUEUE_RESULT_NOPARTY:// 隊伍不存在
		break;

	case EM_BG_JOINQUEUE_RESULT_INQUEUE:
		{
			sprintf(Msg, g_ObjectData->GetString("BG_MSG_JOINQUEUE_RESULT_INQUEUE"), GetBattleGroundName(iType));

			g_pGameMain->SendSystemMsg(Msg);
			g_pChatFrame->SendMsgEvent( CHAT_MSG_SYSTEM, Msg, "" );
			break;
		}
	case EM_BG_JOINQUEUE_RESULT_WAITENTERBG:
		{
			if((iType % 1000) > 400)
			{
				sprintf(Msg, g_ObjectData->GetString("BG_MSG_JOINQUEUE_RESULT_WAITENTERBG"), GetBattleGroundName(iType));

				g_pGameMain->SendSystemMsg(Msg);
				g_pChatFrame->SendMsgEvent( CHAT_MSG_SYSTEM, Msg, "" );
			}
			else
			{
				StructBattleGroundQueueStatus newStatus;
				SecureZeroMemory(&newStatus, sizeof(StructBattleGroundQueueStatus));
				newStatus.emQueueStatus	    = EM_BATTLEGROUNDSTATUS_GOT_TICKET_WAIT_ENTER;
				newStatus.iType				= iType;
				newStatus.dwTimeToEnterQueue = timeGetTime();
				newStatus.dwTimeToLeaveQueue = newStatus.dwTimeToEnterQueue + iWaitTime;
				newStatus.iRoomID           = iRoomID;
				newStatus.iTeamID           = iTeamID;

				m_mapBattleGroundQueueStatus[iType] = newStatus;

				g_pBattleGroundOptionButtonFrame->ShowEnterBGQueryDialog(iType, iRoomID);
			}
			
			break;
		}
	case EM_BG_JOINQUEUE_RESULT_TEAM_INBG:
	case EM_BG_JOINQUEUE_RESULT_TEAM_WAITENTERBG:	// 某個隊員已經在佇列當中
		{

			//pszName 隊員的 DBID

			sprintf(Msg, g_ObjectData->GetString("BG_MSG_TEAM_CHECK"), GetBattleGroundName(iType));
			//sprintf( Msg, "BG_MSG_TEAM_CHECK");

			g_pGameMain->SendSystemMsg(Msg);
			g_pChatFrame->SendMsgEvent( CHAT_MSG_SYSTEM, Msg, "" );
			break;
		}
	/*case EM_BG_JOINQUEUE_RESULT_NOSLOT:
		{
			CNetCli_BattleGround::LeaveBattleGroundWaitQueue(g_pGameMain->GetDBID(), iType);

			sprintf(Msg, g_ObjectData->GetString("BG_MSG_EVENT_ZONE_NO_SLOT"), GetBattleGroundName(iType));
			g_pGameMain->SendSystemMsg(Msg);
			g_pChatFrame->SendMsgEvent( CHAT_MSG_SYSTEM, Msg, "" );
			break;
		}*/

	}
}

// --------------------------------------------------------------------------------------
void CNetCli_BattleGround_Child::OnRZ_BattleGroundClose ( int iSrvID, int iWinTeam )
{
	int iType = g_pGameMain->GetBattleGroundType();

	if (iType > 0)
	{
		map< int, OnBattleGroundCloseFunctionBase >::iterator it = m_mapBattleGroundCloseFunctions.find(iType);

		if (it != m_mapBattleGroundCloseFunctions.end())
		{
			it->second(iSrvID, iWinTeam);
		}
		else
		{
			m_mapBattleGroundCloseFunctions[0](iSrvID, iWinTeam);
		}
	}
}

void CNetCli_BattleGround_Child::BattleGroundCloseHorseRacing( int iSrvID, int iWinTeam )
{
	g_pHorseRacingRankingFrame->FinalRanking(iWinTeam);
}

void CNetCli_BattleGround_Child::BattleGroundCloseNormal( int iSrvID, int iWinTeam )
{
	g_pBattleGroundStatusFrame->BattleGroundClose(iWinTeam);
}

// --------------------------------------------------------------------------------------
void CNetCli_BattleGround_Child::OnRZ_BattleGroundStatus ( int iSrvID, StructBattleGroundValue* pData )
{
	m_BattleGroundData = *pData;

	int iType = g_pGameMain->GetBattleGroundType();

	if (iType > 0)
	{
		map< int, OnBattleGroundStatusFunctionBase >::iterator it = m_mapBattleGroundStatusFunctions.find(iType);

		if (it != m_mapBattleGroundStatusFunctions.end())
		{
			it->second(iSrvID, &m_BattleGroundData);
		}
		else
		{
			m_mapBattleGroundStatusFunctions[0](iSrvID, &m_BattleGroundData);
		}
	}
}

void CNetCli_BattleGround_Child::BattleGroundStatusHorseRacing	( int iSrvID, StructBattleGroundValue* pData )
{
	g_pHorseRacingRankingFrame->UpdateRanking(pData);
}

void CNetCli_BattleGround_Child::BattleGroundStatusNormal( int iSrvID, StructBattleGroundValue* pData )
{
	g_pBattleGroundScoreFrame->UpdateScore(pData);


}

void CNetCli_BattleGround_Child::BattleGroundTowerInfo( int iSrvID, StructBattleGroundValue* pData )
{
	g_pBattleGroundBloodStatusFrame->UpdateTowerInfo(pData);
}

// --------------------------------------------------------------------------------------
void CNetCli_BattleGround_Child::OnRZ_SetBattleGroundData( int iSrvID, int iForceFlagID, int iCampID, int iRoomID )
{
	if( iForceFlagID != 0 )
	{
		GameObjDbStructEx* pObj = g_ObjectData->GetObj( iForceFlagID );
		if( pObj != NULL )
		{
			CRoleSprite* pSprite = g_pGameMain->GetPlayer();

			if( pSprite != NULL )
			{
				pSprite->CreateBackEntity( iForceFlagID );
			}	
		}
	}

}
// --------------------------------------------------------------------------------------
void CNetCli_BattleGround_Child::OnRZ_BattleGroundScoreInfo		( int iSrvID, int iTotalRole, StructBattleGroundScore* pScoreArray )
{
	m_mapBattleGroundScores.clear();

	StructBattleGroundScore* pTmp = pScoreArray;
	for (int i = 0; i < iTotalRole; i++)
	{
		m_mapBattleGroundScores[pTmp->Name.Begin()] = *pTmp;
		pTmp++;
	}

	g_pBattleGroundStatusFrame->UpdateScore(iSrvID, iTotalRole, pScoreArray);
}

void CNetCli_BattleGround_Child::OnRZ_BattleGroundWaitQueueStatus( int iSrvID, int iType, int iStatus, int iWaitID, int iSrvCheckTime)
{
	map< int, StructBattleGroundQueueStatus >::iterator it = m_mapBattleGroundQueueStatus.find(iType);

	//already in the wait queue
	if (it != m_mapBattleGroundQueueStatus.end())
	{
		StructBattleGroundQueueStatus& status = m_mapBattleGroundQueueStatus[iType];

		int deltaID   = status.iWaitID - iWaitID;
		int deltaTime = iSrvCheckTime - status.iSrvCheckTime;
		int timePerID = 0;  //mini seconds per ID

		if (deltaID > 0) //Queue 真的有在消化
		{
			timePerID = deltaTime / deltaID;
		}

		if (timePerID > 0)
		{
			status.dwTimeExcepted = iWaitID * timePerID;
		}
		else
		{
			status.dwTimeExcepted = 0; //unknown
		}

		status.iWaitID       = iWaitID;
		status.iSrvCheckTime = iSrvCheckTime;
	}
	else
	{
		StructBattleGroundQueueStatus newWait;
		SecureZeroMemory(&newWait, sizeof(StructBattleGroundQueueStatus));
		newWait.emQueueStatus	   = EM_BATTLEGROUNDSTATUS_IN_QUEUE_WAIT_ENTER;
		newWait.iType			   = iType;
		newWait.dwTimeToEnterQueue = timeGetTime();
		newWait.iWaitID			   = iWaitID;
		newWait.iSrvCheckTime      = iSrvCheckTime;

		m_mapBattleGroundQueueStatus[iType] = newWait;

		if((iType % 1000) > 400)
		{
			char Msg[256];

			sprintf(Msg, g_ObjectData->GetString("BG_PALYER_ENTER_QUEUE"), GetBattleGroundName(iType));

			g_pGameMain->SendSystemMsg(Msg);
			g_pChatFrame->SendMsgEvent( CHAT_MSG_SYSTEM, Msg, "" );
		}
	}

	// 回送 Client 的 DBID, 作為回應
	BattleGroundWaitQueueStatusRespond( iSrvID, g_pGameMain->GetDBID() );
}

map< int, StructBattleGroundQueueStatus >& CNetCli_BattleGround_Child::GetQueseStatusMap()
{
	CNetCli_BattleGround_Child* _This = (CNetCli_BattleGround_Child*)m_pThis;

	return _This->m_mapBattleGroundQueueStatus;
}


void CNetCli_BattleGround_Child::OnRZ_AllLandMarkInfo ( int iSrvID, StructRoomLandMark* pInfo )
{
	for( int i = 0; i < DF_MAX_BG_LANDMARK; i++ )
	{
		if( strlen( pInfo->LandMark[ i ].Name ) != 0 )
		{
			m_mapLandMark[ i ] = pInfo->LandMark[ i ];
		}
	}
}

void CNetCli_BattleGround_Child::OnRZ_AllBattleGroundPlayerPos( int iSrvID, int iTotalRole, StructBattleGroundPlayerPos* pPosArray )
{
	StructBattleGroundPlayerPos* pTmp = pPosArray;

	m_mapBattleGroundPlayerPos.clear();

	for (int i = 0; i < iTotalRole; i++)
	{
		m_mapBattleGroundPlayerPos[pTmp->RoleName.Begin()] = *pTmp;
		pTmp++;
	}

	((CNetCli_BattleGround_Child*)m_pThis)->UpdateBattleGroundMinimap();
}

// --------------------------------------------------------------------------------------
void CNetCli_BattleGround_Child::OnRZ_BattleGroundLandMark	( int iSrvID, int iID, StructLandMark* pLandMark )
{
	m_mapLandMark[ iID ] = *pLandMark;

#ifdef _DEBUG
	char szBG[256];
	sprintf( szBG, "BG_LANDMARK: SrvID = %d, Land = %d", iSrvID, iID );
	g_pChatFrame->SendMsgEvent( CHAT_MSG_SYSTEM, szBG, "" );
#endif
}

void CNetCli_BattleGround_Child::OnRZ_UpdateArenaData( int iSrvID, int iArenaType, int iTeamGUID, StructArenaInfo* pArenaInfo )
{
	// iArenaType 1 = 1v1, 2 = 2vs2, 3 = 6vs6, 4 = 2vs2vs2vs2
	if (g_pBattleGroundArenaScoreFrame)
	{
		g_pBattleGroundArenaScoreFrame->UpdateArenaData(iArenaType, iTeamGUID, pArenaInfo);
	}
}

int	CNetCli_BattleGround_Child::GetBattleGroundPeopleLimit()
{
	return 0;
}

const char* CNetCli_BattleGround_Child::GetBattleGroundName(int iType)
{

	static char			szZoneName[256];
	int					iZoneID			= Def_ObjectClass_Zone + ( iType % 1000 );
	GameObjDbStructEx*	pZoneObj		= NULL;


	pZoneObj = RoleDataEx::St_GetObjDB( iZoneID );

	if( pZoneObj != NULL && strlen( pZoneObj->Zone.MapInfoList[0].MapName ) != 0 )
	{
		char szKey[256];
		sprintf( szKey, "ZONE_%s", pZoneObj->Zone.MapInfoList[0].MapName );

		for( int i = 0; i < strlen( szKey ); i++ )
		{
			szKey[i] = ::toupper(szKey[i]);
		}

		sprintf( szZoneName, "%s", g_ObjectData->GetString( szKey ) );
		if( strlen(szZoneName) == 0 )
		{
			sprintf( szZoneName, "Zone_BG_%d", iType );
		}
	}
	else
	{
		sprintf( szZoneName, "Zone_BG_%d", iType );
	}

	return szZoneName;
	/*
	CNetCli_BattleGround_Child* _TmpThis = (CNetCli_BattleGround_Child*) m_pThis;

	map< int, string >::iterator it = _TmpThis->m_mapBattleGroundNames.find(iType);

	if (it != _TmpThis->m_mapBattleGroundNames.end())
	{
		return _TmpThis->m_mapBattleGroundNames[iType].c_str();
	}
	else
	{
		return _TmpThis->m_mapBattleGroundNames[0].c_str();
	}
	*/

}

void CNetCli_BattleGround_Child::OnRZ_GetRankPersonalInfoResult( int iSrvID, int iDBID, int iRankPoint, int iLastRoundRank, int iRank, const char* pszNote )
{

}