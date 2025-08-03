#include "../GameMain.h"
#include "../InterfaceMember/WorldFrame.h"
#include "../InterfaceMember/UI_MaintainFrame.h"

// --------------------------------------------------------------------------------------
void CGameMain::PlayerItemID( int GItemID )
{
	g_pWorldFrame->SetPlayerID( GItemID );
}

// --------------------------------------------------------------------------------------
void CGameMain::ZoneInfo( int GSrvNetID , int ZoneID , int GSrvID , int MapID )
{
	g_pWorldFrame->SetZoneID( ZoneID );
	g_pWorldFrame->SetMapID( MapID );
    RoleDataEx::G_ZoneID = ZoneID;
}

// --------------------------------------------------------------------------------------
void CGameMain::PlayerData( BaseRoleData *BaseRole )
{
    int a = 0;
    m_RoleData.BaseData = *BaseRole;
    m_RoleData.InitCal(); 
    m_RoleData.TempData.UpdateInfo._Event = 0xffffffff;


	// 任務資料部份 ( 從 g_ObjectData 中將資先取出, 加快每次檢查的速度 )
	//--------------------------------------------------------------------
	/*
		// 打倒目標
		QuestTempDataStruct	*pQuestTempData = &( m_RoleData.TempData.QuestTempData );
		ZeroMemory( pQuestTempData, sizeof( QuestTempDataStruct ) );
		int	iTempIndex = 0;

		for( int index = 0; index < _MAX_QUEST_COUNT_; index++ )
		{
			int iQuestID =  m_RoleData.BaseData.Quest.iQuest[ index ];
			if( iQuestID != 0 )
			{
				GameObjectQuestDetail *pQuest = &( g_ObjectData->GetObj( iQuestID )->QuestDetail );
				pQuestTempData->QuestKillIndexTable[ iTempIndex ] = index;
				m_RoleData.CheckQuest_RequestKill_SetTempData		( index, pQuest );
				m_RoleData.CheckQuest_ClickToGetItem_SetTempData	( index, pQuest );
			}
			else
				break;
		}

		// 點擊物品
	*/

}
// --------------------------------------------------------------------------------------
void CGameMain::PrepareChangeZone()
{
    // 準備進入(換區)區域
    int a = 0;
}

// --------------------------------------------------------------------------------------

void CGameMain::ChangeZoneOK()
{
    /*
    // 載入天空及地表
    InitialSky();

    SetSkyTexture("texture\\sky");

    // 載入地表
    LoadTerrain( "world_client.ros" );	
    */
}
// --------------------------------------------------------------------------------------
void CGameMain::R_ModyNPC( ModifyNPCInfo*  Info )
{
    g_pUI_MaintainFrame->ModifyOpen( Info );
}
// --------------------------------------------------------------------------------------