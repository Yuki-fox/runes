#include "../../mainproc/GameMain.h"
#include "NetCli_Script_Child.h"

#include "../../interface/WorldFrames/GuildFrame.h"
#include "../../interface/WorldFrames/TutorialFrame.h"
#include "../../interface/map/Minimap.h"
#include "../../interface/WorldFrames/ChatFrame.h"
#include "../../interface/quest/UI_QuestDetail.h"
#include "../../interface/quest/UI_QuestList.h"
#include "../../interface/quest/UI_Questbook.h"
#include "../../interface/ScriptBorder/UI_ScriptBorder.h"
#include "../../Interface//WorldFrames/CastingBarFrame.h"
#include "../../interface/ObjectBloodBar/ObjectBloodBar.h"
#include "../../interface/WorldFrames/ClientCountDownBarFrame.h"
#include "PG/PG_Gather.h"

#include "../../tools/Common/Com_Function.h"


//#include "../interfacemember/Quest/UI_QuestDialog.h"
//#include "../interfacemember/Quest/UI_QuestNPCList.h"
//#include "../interfacemember/UI_CastingTimeBar.h"

//#include "../interfacemember/WorldFrame.h"
//#include "../../MainProc/ObjectDataClass.h"

//using namespace ui;

//CRoleSprite*	CNetCli_Script_Child::m_pLastTouchObj = NULL;
int							CNetCli_Script_Child::m_iScriptCastMotionEnd;
int							CNetCli_Script_Child::m_iEnterCollisionObjID	= 0;
int							CNetCli_Script_Child::m_iLastTouchObjID			= 0;
int							CNetCli_Script_Child::m_iScriptCastAttachToolID	= 0;


vector< CRoleSprite* >		CNetCli_Script_Child::m_vecUnProcessRole;
vector< CRoleSprite* >		CNetCli_Script_Child::m_vecQuestRole;
set< int >					CNetCli_Script_Child::m_mapRangePlotObj;
CRuVector3					CNetCli_Script_Child::m_LastCollisionPos;

string						CNetCli_Script_Child::m_szColor_NPC;
string						CNetCli_Script_Child::m_szColor_Item;
string						CNetCli_Script_Child::m_szColor_Zone;
string						CNetCli_Script_Child::m_szQuest_Complete;
string						CNetCli_Script_Child::m_szQuest_UnComplete;
string						CNetCli_Script_Child::m_szQuest_Title;
string						CNetCli_Script_Child::m_szQuest_NoTitle;


string					CNetCli_Script_Child::m_pSource				= "";
bool					CNetCli_Script_Child::m_bQuestColor			= false;
bool					CNetCli_Script_Child::m_bHyperLink			= false;
bool					CNetCli_Script_Child::m_bNoloopCheckVar		= false;
string					CNetCli_Script_Child::m_lastResult			= "";


//map< CRoleSprite* , ScriptEventInfo* >	CNetCli_Script_Child::m_mapRangePlot;
// --------------------------------------------------------------------------------------
bool CNetCli_Script_Child::Init()
{	
	if( m_pThis == NULL )
		m_pThis = new CNetCli_Script_Child;


	((CNetCli_Script_Child*)m_pThis)->m_iClickQuestObjID = 0;

	CNetCli_Script::Init();
	CNetGlobal::Schedular()->Push( OnTimeUpdate , 1000 , NULL , NULL );
	CNetGlobal::Schedular()->Push( OnMinimapQuestObjTimeUpdate , 1500 , NULL , NULL );

	((CNetCli_Script_Child*)m_pThis)->InitColorString();

	return true;
}
// --------------------------------------------------------------------------------------
bool CNetCli_Script_Child::Release()
{
	CNetCli_Script::Release();

	SAFE_DELETE( m_pThis );

	return true;
}
// --------------------------------------------------------------------------------------
void CNetCli_Script_Child::InitColorString()
{
	const char*		pstr_NPC				= g_ObjectData->GetString( "SYS_COLOR_NPC" );
	const char*		pstr_Item				= g_ObjectData->GetString( "SYS_COLOR_ITEM" );
	const char*		pstr_Zone				= g_ObjectData->GetString( "SYS_COLOR_ZONE" );
	const char*		pstr_Quest_Complete		= g_ObjectData->GetString( "SYS_COLOR_QUEST_COMPLETE" );
	const char*		pstr_Quest_UnComplete	= g_ObjectData->GetString( "SYS_COLOR_QUEST_UNCOMPLETE" );
	const char*		pstr_Quest_Title		= g_ObjectData->GetString( "SYS_COLOR_QUEST_TITLE" );
	const char*		pstr_Quest_NoTitle		= g_ObjectData->GetString( "SYS_COLOR_QUEST_NO_TITLE" );

	if( strlen( pstr_NPC ) != 0 )
		m_szColor_NPC		= pstr_NPC;
	else
		m_szColor_NPC		= "|off00ffff";

	if( strlen( pstr_Item ) != 0 )
		m_szColor_Item		= pstr_Item;
	else
		m_szColor_Item		= "|offffff00";

	if( strlen( pstr_Zone ) != 0 )
		m_szColor_Zone		= pstr_Zone;
	else
		m_szColor_Zone		= "|off00ff00";

	if( strlen( pstr_Quest_Complete ) != 0 )
		m_szQuest_Complete		= pstr_Quest_Complete;
	else
		m_szQuest_Complete		= "|off20ff20";

	if( strlen( pstr_Quest_UnComplete ) != 0 )
		m_szQuest_UnComplete		= pstr_Quest_UnComplete;
	else
		m_szQuest_UnComplete		= "|offff2020";

	if( strlen( pstr_Quest_Title ) != 0 )
		m_szQuest_Title		= pstr_Quest_Title;
	else
		m_szQuest_Title		= "|off20ff20";

	if( strlen( pstr_Quest_NoTitle ) != 0 )
		m_szQuest_NoTitle		= pstr_Quest_NoTitle;
	else
		m_szQuest_NoTitle		= "|offff2020";

}
//--------------------------------------------------------------------------------------
void CNetCli_Script_Child::OnAddObj ( int iObjID )
{	
	CRoleSprite* pObj = g_pGameMain->FindSpriteByID( iObjID );

//	int pid=0;
	if( pObj )
	{
		m_vecUnProcessRole.push_back( pObj );

	//	CRoleSprite* player=g_pGameMain->GetPlayer();
		//if (player==pObj)
			//pid=iObjID;

		g_pObjectBloodBar->AddObj(iObjID);
	}

	//m_vecUnProcessRole.push_back( pObj );
}
//--------------------------------------------------------------------------------------
void CNetCli_Script_Child::OnDelObj ( int iObjID )
{
	int a = 3;

	/*
	// 到互動介面去查詢, 是否有與該 Role 進行互動
	{
		map< CRoleSprite* , CControlInteractUI* >::iterator it = CControlInteractUI::m_mapInteractRange.begin();
		while( it != CControlInteractUI::m_mapInteractRange.end() )
		{
			if( it->first == pObj )
			{
				CControlInteractUI* pUI = it->second;
				pUI->OnLeave_InteractRange( pUI, pObj );

				it = CControlInteractUI::m_mapInteractRange.erase( it );
			}
			else
				it++;
		}
	}
	*/

	CRoleSprite* pObj = g_pGameMain->FindSpriteByID( iObjID );

	if (pObj)
	{
		vector< CRoleSprite* >::iterator it;

		it = m_vecUnProcessRole.begin();
		while( it != m_vecUnProcessRole.end() )
		{
			if( *it == pObj )
			{
				m_vecUnProcessRole.erase( it );
				break;
			}
			else
				it++;
		}

		it = m_vecQuestRole.begin();
		while( it != m_vecQuestRole.end() )
		{
			if( *it == pObj )
			{
				m_vecQuestRole.erase( it );
				break;
			}
			else
				it++;

		}
		
		// TODO 應該改用 Map 來索引
		{
			set<int>::iterator it = m_mapRangePlotObj.find( iObjID );

			if( it != m_mapRangePlotObj.end() )
			{
				m_mapRangePlotObj.erase( it );
			}
		}

		/*
		{
			map< CRoleSprite* , ScriptEventInfo* >::iterator it = m_mapRangePlot.find( pObj );
			if( it != m_mapRangePlot.end() )
			{
				ScriptEventInfo* pEvent = it->second;
				delete pEvent;

				m_mapRangePlot.erase( it );
			}
		}
		*/

		g_pObjectBloodBar->DelObj(iObjID);
	}
}
//--------------------------------------------------------------------------------------
//void CNetCli_Script_Child::OnTouchQuestNPC ( CRoleSprite* pRoleObj )
void CNetCli_Script_Child::OnTouchQuestNPC	( int iObjID )
{
	
	CRoleSprite* pObj = g_pGameMain->FindSpriteByID( iObjID );

	if( pObj == NULL )
		return;
	/*
	if( m_iLastTouchObjID == iObjID )
		return;	
	else
		m_iLastTouchObjID = iObjID;
	*/
	int					iDBID	= pObj->GetObjectId();
	GameObjDbStruct*	pObjDB	= g_ObjectData->GetObj( iDBID );


	if( pObj->GetDisableQuestState() == true )
	{
		return;	// 該 NPC 處於關畢任務及對話狀態
	}


	if( strlen( pObjDB->NPC.szLuaScript ) != 0 )
	{
		// 若有 Lua 字串表示, 該任務 NPC 會透過 Server 觸發對話選項後, 回傳選項至 Server
		CNetCli_Script::LoadSrvOption( iObjID );
	}
	else
	{
		// 清除對話介面內容
		RoleDataEx*			pRole		= CNetGlobal::RoleData();
		pRole->PlayerTempData->SpeakInfo.Reset();

		g_pQuestList->SetQuestNPC( iObjID );	

		// 任務快顯, 若只有一個任務可接或可完成的話, 就直街顯示該任務內容
		int iTotal = 0;

		for( int i=0; i<=2; i++ ) 
			iTotal += (int)g_pQuestList->m_vecQuestID[i].size();

		if( iTotal == 1 && ( g_pQuestList->m_vecQuestID[0].size() != 0 || g_pQuestList->m_vecQuestID[2].size() != 0 ) )
		{
			// 執行快顯

			int iQuestID = 0;

			if( g_pQuestList->m_vecQuestID[0].size() != 0 )
				iQuestID = g_pQuestList->m_vecQuestID[0].front();
			else
				iQuestID = g_pQuestList->m_vecQuestID[2].front();

			g_pQuestDetail->SetQuest( 0, -1, iQuestID );	// 從 NPC 身上顯示任務

			lua_State* L = g_pGameMain->GetInterface()->GetLuaState();

			lua_pushnumber( L, 1 );	// 設定離開直接離開, 不返回上一頁
			lua_setglobal( L, "arg1" );

			g_pInterface->SendWorldEvent( UiEvent_ShowQuestDetailFromNPC );
		}
		else
			g_pInterface->SendWorldEvent( UiEvent_ShowQuestList );
	}
	
	// 發出停止移動封包, 並要求其面向玩家
	CNetCli_Script_Child::DelayPatrol( iObjID, _TIME_DELAYPATROL );
	

	/*
	CRoleSprite* pRoleObj = NULL; // Need to get role from mine's data collector

	if( pRoleObj->GetCheckedStatus() == eChecked_QuestObj )
	{
		// 任務物件

	
	}
	else
	{
		// 任務NPC
		if( g_pUi_QuestNPCList->m_pQuestNPCList && g_pUi_QuestNPCList->m_pQuestNPCList->IsVisible() == true )
		{
			if( m_pLastTouchObj == pRoleObj )
			{
				return;
			}
			else
			{
				m_pLastTouchObj = pRoleObj;
			}
		}
		else
		{
			m_pLastTouchObj = pRoleObj;
		}

		int iQuestNPCID = pRoleObj->GetObjectId();

		// Check Quest
		if( iQuestNPCID != 0 )
		{
			int iGUID = iQuestNPCID;

			GameObjDbStruct*	pObj	= g_ObjectData->GetObj( iGUID );
			if( pObj )
			{
				RoleDataEx*			pRole	= g_pGameMain->RoleData();	

				// 重置介面狀態
					g_pUi_QuestNPCList->ReSet();		

				// 設置互動 NPC
					g_pUi_QuestNPCList->Set_InteractNPC( pRoleObj );

				// 設置 NPC Name
					g_pUi_QuestNPCList->Set_NpcName( pRoleObj->GetName() );
					//g_pUi_QuestNPCList->Set_NpcName( pObj->Name );

				// 加入 NPC 會說的廢話
					//g_pUi_QuestNPCList->Add_NpcText( pObj->QuestCollect.szQuestNpcText );	// 設置 NPC 會說甚麼
					g_pUi_QuestNPCList->Add_NpcText( pObj->NPC.szQuestNpcText );	// 設置 NPC 會說甚麼

				// 檢查該 NPC 是否是指定完成任務 NPC
				// 檢查 Client 身上的 Quest 有無另指定 NPC 完成
				//------------------------------------------------------------------------------
					for( int index = 0; index < _MAX_QUEST_COUNT_; index++ )
					{
						int iQuestID = pRole->PlayerBaseData->Quest.iQuest[ index ];
						if( iQuestID != 0 )
						{
							GameObjDbStruct			*pObj	= g_ObjectData->GetObj( iQuestID );
							GameObjectQuestDetail	*pQuest = &( pObj->QuestDetail );

							if( pQuest->iRequest_QuestID == iQuestNPCID )
							{
								// 檢查及設置 NPC 身上 Quest 狀態
								if( pRole->CheckQuest_IsComplete( index, iQuestID, pQuest ) == EM_QUESTCHECK_RESULT_OK )
								{
									g_pUi_QuestNPCList->Add_Complete_Quest( iQuestID, pObj->Name );
								}
								else
								{
									g_pUi_QuestNPCList->Add_Current_Quest( iQuestID, pObj->Name );
								}
							}
						}
						else
							break;
					}

				// 加入 NPC 能給的 QUEST
				//------------------------------------------------------------------------------
					for( int Index = 0; Index < 20; Index++ )
					{
						int iQuestID = pObj->NPC.iQuestID[ Index ];

						// 檢查角色是否已完成任務
							if( pRole->CheckQuestHistory_IsComplete( iQuestID ) == true )
								continue;	// 跳過這個 Quest
						
						if( iQuestID != 0 )
						{
							GameObjDbStruct			*pObj	= g_ObjectData->GetObj( iQuestID );
							
							if( pObj )
							{
								GameObjectQuestDetail	*pQuest = &( pObj->QuestDetail );

								// 檢查角色是否可以接該任務
								if( pRole->CheckQuest_IsAvailable( pQuest ) != EM_QUESTCHECK_RESULT_OK )
									continue;


								int iType = 0;

								// 檢查角色是否擁有該 Quest, 及是否完成
									int iQuestField = pRole->CheckQuestList_IsGotQuest( iQuestID );

									if( iQuestField != -1 )
									{
										// 檢查該任務是否為指定另一NPC完成.
										// 若是則該 NPC 即不顯示該任務
										if( pQuest->iRequest_QuestID != 0 && pQuest->iRequest_QuestID != iQuestNPCID )
											continue;
										
										// 檢查是否已完成該 Quest
										if( pRole->CheckQuest_IsComplete( iQuestField, iQuestID, pQuest ) == EM_QUESTCHECK_RESULT_OK )
										{
											iType = 2;	// 已完成
										}
										else
										{
											iType = 1; // 尚未完成
										}
									}
									else
									{
										iType = 0;	// 尚未接過
									}

								switch( iType ) 
								{
								case 0: // 尚未接過
									g_pUi_QuestNPCList->Add_Available_Quest( iQuestID , pObj->Name );
									break;
								case 1: // 尚未完成
									g_pUi_QuestNPCList->Add_Current_Quest( iQuestID, pObj->Name );
									break;
								case 2: // 已完成
									g_pUi_QuestNPCList->Add_Complete_Quest( iQuestID, pObj->Name );
									break;
								}
							}

						}
						else
							break;
					}

				// 秀出介面
					g_pUi_QuestNPCList->Show();
				}
		}
	}
	*/
}

//--------------------------------------------------------------------------------------
int CNetCli_Script_Child::OnMinimapQuestObjTimeUpdate ( SchedularInfo* Obj , void* InputClass )
{
	CNetGlobal::Schedular()->Push( OnMinimapQuestObjTimeUpdate , 1500 , NULL , NULL );

	map<int, CRoleSprite*>&		spriteMap			= g_pRuneDev->GetSpriteMap();

	for( map<int, CRoleSprite*>::iterator it = spriteMap.begin(); it != spriteMap.end(); it++ )
	{
		//CRoleSprite*

	}

	return 0;
}

int CNetCli_Script_Child::OnTimeUpdate			( SchedularInfo* Obj , void* InputClass )
{
	CRoleSprite*						pPlayer			= g_pGameMain->GetPlayer();
	vector< CRoleSprite* >::iterator	itUnProcess		= m_vecUnProcessRole.begin();
	CRoleSprite	*pRoleSprite = NULL;

	// 持續觸發事件
		CNetGlobal::Schedular()->Push( OnTimeUpdate , 1000 , NULL , NULL );

	if( pPlayer == NULL )
		return 0;

	// 檢查有無新增的 Quest NPC 或 範圍劇情觸發
	//----------------------------------------------------------

		while( itUnProcess != m_vecUnProcessRole.end() )
		{
			pRoleSprite = *itUnProcess;

			//int iObjID				= pRoleSprite->GetObjectId();
			//int iQuestNpcBeginID	= Def_ObjectClass_QuestNPC;
			//int iQuestNpcEndID		= Def_ObjectClass_QuestNPC + Def_ObjectClass_QuestNPC_Total;

			if( pRoleSprite->GetCheckedStatus() == eChecked_QuestNPC || pRoleSprite->GetCheckedStatus() == eChecked_QuestObj )
			{
				m_vecQuestRole.push_back( pRoleSprite );
			}

			if( pRoleSprite->GetAreaRange() != 0 )
			{
				m_mapRangePlotObj.insert( pRoleSprite->GetIdentify() );
			}

			// 範圍劇情事件
				/*
				if( pRoleSprite->GetAreaRange() != 0 )
				{
					ScriptEventInfo* pEvent = new ScriptEventInfo;
					m_mapRangePlot[ pRoleSprite ] = pEvent;
				}
				*/

			itUnProcess = m_vecUnProcessRole.erase( itUnProcess );
		}

	// 檢查 Quest NPC 狀態
	//----------------------------------------------------------
		RoleDataEx*		pRole		= CNetGlobal::RoleData();

		//it = m_vecQuestRole.begin();
		//while( it != m_vecQuestRole.end() )
		map<int, CRoleSprite*>&		spriteMap			= g_pRuneDev->GetSpriteMap();

		map<int, CRoleSprite*>::iterator it = spriteMap.begin();

		while( it != spriteMap.end() )
		{
			int*			pQuestList	= NULL;
			QuestDisplay	QuestMode	= eQuest_Display_Null;

			pRoleSprite		= it->second;
			int iQuestNpcID = pRoleSprite->GetObjectId();
			//int iQuestCollectID						= pRoleSprite->GetObjectId();
			//GameObjectQuestCollect	*pQuestCollect	= &( g_ObjectData->GetObj( iQuestCollectID )->QuestCollect );
			GameObjDbStructEx* pQuestNpcObj = g_ObjectData->GetObj( iQuestNpcID );

			if( pQuestNpcObj )
				pQuestList		= pQuestNpcObj->NPC.iQuestID;
			else
			{
				it++;
				continue;
			}
			

			if( pRoleSprite->GetCheckedStatus() == eChecked_QuestNPC )
			{
				// 檢查 Client 身上的 Quest 有無另指定 NPC 完成
				//-------------------------------------------------------------					
				{
					for( int index = 0; index < _MAX_QUEST_COUNT_; index++ )
					{
						int					iQuestID	= pRole->PlayerBaseData->Quest.QuestLog[ index ].iQuestID;
						GameObjDbStructEx*	pObj		= g_ObjectData->GetObj( iQuestID );
						
						if( pObj && pObj->IsQuest() )
						{
							GameObjectQuestDetail *pQuest = &( pObj->QuestDetail );

							if( pQuest->iRequest_QuestID == iQuestNpcID )
							{
								// 檢查及設置 NPC 身上 Quet 狀態
									if( pRole->CheckQuest_IsComplete( index, iQuestID, pQuest ) == EM_QUESTCHECK_RESULT_OK )
									{							
										if( (bool)pQuest->iCheck_Loop == true )
										{
											QuestMode = eQuest_Display_FinishedRepeatQuest;
											
											if( pQuest->iQuestGroup == 3 )	// 檢測是不是 PUBLIC 任務
											{
												QuestMode	= eQuest_Display_Public_FinishedQuest;
											}
										}
										else
										{
											QuestMode = eQuest_Display_FinishedQuest;
										}

										break;
									}
									else
										QuestMode = eQuest_Display_UnfinishedQuest;
							}
						}
					}
				}
				// 檢查 NPC 身上 Quest 狀態了
				//-------------------------------------------------------------				
				{
					if( QuestMode != eQuest_Display_FinishedQuest && pRoleSprite->GetDisableQuestState() == false )
					{
						for( int index = 0; index < _MAX_QUEST_COUNT_; index++ )
						{
							int iQuestID = pQuestList[ index ];



							if( iQuestID != 0 )
							{	
								GameObjDbStruct			*pObj	= g_ObjectData->GetObj( iQuestID );
								
								if( pObj )
								{
									GameObjectQuestDetail	*pQuest = &( pObj->QuestDetail );

									// 檢查角色是否已完成該任務
										if( pRole->CheckQuestHistory_IsComplete( iQuestID ) == true )
										{
											if( pQuest->iCheck_Loop == false )
												continue;	// 跳過這個 Quest
										}

									// 3 黃色?		有已完成的任務
									//-------------------------------------------------------------
										// 檢查 Client 身上有無該項任務
										int iQuestIndex = pRole->CheckQuestList_IsGotQuest( iQuestID );

										if( iQuestIndex != -1 )
										{
											// 檢查該項任務是否已經完成了

											// 檢查是否指定另一個 NPC 完成任務
												if( pQuest->iRequest_QuestID != 0 )
												{
													continue;	// 表示 Clinet 已接該 Quest, 該 Quest 的完成已另指定 NPC 完成
												}

											if( pRole->CheckQuest_IsComplete( iQuestIndex, iQuestID, pQuest ) == EM_QUESTCHECK_RESULT_OK )
											{
												if( (bool)pQuest->iCheck_Loop == true )
												{
													QuestMode = eQuest_Display_FinishedRepeatQuest;
													
													if( pQuest->iQuestGroup == 3 )
													{
														QuestMode	= eQuest_Display_Public_FinishedQuest;
													}
												}
												else
												{
													QuestMode = eQuest_Display_FinishedQuest;
												}


												
												break;
											}
											else
											{								
												if( QuestMode < eQuest_Display_UnfinishedQuest )
													QuestMode = eQuest_Display_UnfinishedQuest;	// 1 灰色?		任務尚未成
											}

											continue;
										}

									// 2 紅色!		有任務可接
									//-------------------------------------------------------------
										// 檢查 Client 的條件和 Quest 是否相符, 是的話, 秀出可以接任務
										if( pRole->CheckQuest_IsAvailable( iQuestID, pQuest, false ) == EM_QUESTCHECK_RESULT_OK )
										{
											bool bLuaCheck = true;
											// 檢查有無 LUA QUESTBEGIN
											if( strlen( pQuest->szScript_OnBegin ) != 0 )
											{
												bLuaCheck = g_pQuestBook->CheckQuestBeginLua( pQuest->szScript_OnBegin );

											}

											if( bLuaCheck == true && QuestMode < eQuest_Display_AvailableQuest )
											{
												if( (bool)pQuest->iCheck_Loop == true )
												{
													if( QuestMode < eQuest_Display_AvailableRepeatQuest )
														QuestMode = eQuest_Display_AvailableRepeatQuest;
												}
												else
												{
													if( QuestMode < eQuest_Display_AvailableQuest )
														QuestMode = eQuest_Display_AvailableQuest;
												}

												if( pQuest->iQuestGroup == 3 )
												{
													if( QuestMode < eQuest_Display_Public_AvailableQuest )
														QuestMode	= eQuest_Display_Public_AvailableQuest;
												}

												
											}

											// 檢測是不是 PUBLIC 任務
											if( pQuest->iQuestGroup == 3 )
											{
												switch( QuestMode )
												{
												case eQuest_Display_AvailableRepeatQuest:
												case eQuest_Display_AvailableQuest:
													{
														if( QuestMode < eQuest_Display_Public_AvailableQuest )
															QuestMode	= eQuest_Display_Public_AvailableQuest; 
													}
													
													break;

												case eQuest_Display_FinishedRepeatQuest:
												case eQuest_Display_FinishedQuest:
													{
														if( QuestMode < eQuest_Display_Public_FinishedQuest )
															QuestMode	= eQuest_Display_Public_FinishedQuest;
													}													
													break;
												}

											}

										}

									// 檢查灰色 !
									//-------------------------------------------------------------
										if( QuestMode < eQuest_Display_Get_Gary )
										{
											if( pRole->CheckGrayQuest( iQuestID, pQuest ) == EM_QUESTCHECK_RESULT_OK )
											{
												if( QuestMode < eQuest_Display_Get_Gary )
												{
													QuestMode = eQuest_Display_Get_Gary;
												}
											}
										}


								}
							}
							else
								break;
						}



					}
				}


				
				// 依照最後檢查結果 QuestMode 決定 NPC 顯示的 QuestFlag
				//----------------------------------------------------------
				{
					QuestDisplay emMode = (QuestDisplay) pRoleSprite->GetQuestDisplay();
					
					if( emMode != QuestMode )
					{
						// 判斷可接任務的優先權

						//if( emMode == eQuest_Display_AvailableQuest || emMode == eQuest_Display_AvailableRepeatQuest || emMode == eQuest_Display_Public_AvailableQuest )
						//{
						//	int iCurrentMode	= ((CNetCli_Script_Child*)m_pThis)->GetQuestModePriority( (int)emMode );
						//	int iMode			= ((CNetCli_Script_Child*)m_pThis)->GetQuestModePriority( (int)QuestMode );

						//	if( iMode > iCurrentMode )
						//	{
						//		QuestMode = emMode;
						//	}
						//}


						pRoleSprite->SetQuestDisplay( QuestMode );

						switch( QuestMode )
						{
						case eQuest_Display_Null:					pRoleSprite->SetQuestMinimapIcon( (int)EM_MapIconType_None				); break;
						case eQuest_Display_AvailableQuest:			pRoleSprite->SetQuestMinimapIcon( (int)EM_MapIconType_NewQuest			); break;
						case eQuest_Display_UnfinishedQuest:		pRoleSprite->SetQuestMinimapIcon( (int)EM_MapIconType_UnfinishQuest		); break;
						case eQuest_Display_FinishedQuest:			pRoleSprite->SetQuestMinimapIcon( (int)EM_MapIconType_FinishedQuest		); break;	
						case eQuest_Display_Get_Gary:				pRoleSprite->SetQuestMinimapIcon( (int)EM_MapIconType_GrayQuest			); break;
						
						case eQuest_Display_AvailableRepeatQuest:	pRoleSprite->SetQuestMinimapIcon( (int)EM_MapIconType_RepeatQuest		); break;	
						case eQuest_Display_FinishedRepeatQuest:	pRoleSprite->SetQuestMinimapIcon( (int)EM_MapIconType_RepeatQuestDone	); break;

						case eQuest_Display_Public_AvailableQuest:	pRoleSprite->SetQuestMinimapIcon( (int)EM_MapIconType_PublicQuest		); break;	
						case eQuest_Display_Public_FinishedQuest:	pRoleSprite->SetQuestMinimapIcon( (int)EM_MapIconType_PublicQuestDone	); break;

						}
					}
				}
			}

			// Client 顯示 Script 檢查					// 執行本地顯示劇情
			//-----------------------------------------------------------------------------------------------
			{



				if( pQuestNpcObj->Classification == EM_ObjectClass_QuestNPC )
				{
					if( strlen( pQuestNpcObj->NPC.szLuaDisplayScript ) != 0 )
					{
						if( g_pQuestBook->CheckQuestBeginLua( pQuestNpcObj->NPC.szLuaDisplayScript ) == true )
						{
							pRoleSprite->Hide();
							pRoleSprite->ScriptHide();	// 隱藏 NPC
							pRoleSprite->EnableCollision( false );
						}
						else
						{
							pRoleSprite->Show();
							pRoleSprite->ScriptShow();
							pRoleSprite->EnableCollision( pRoleSprite->CollisionBaseEnabled() );
						}
					}
				}
			}

			it++;
		}

	// 檢查範圍觸發 NPC 狀態
	//-----------------------------------------------------------------------------------------------

		{
			//map< CRoleSprite* , ScriptEventInfo* >::iterator it;

			set<int>::iterator it = m_mapRangePlotObj.begin();				// 範圍劇情 NPC

			// 範圍事件檢查
			while( it != m_mapRangePlotObj.end() )
			{
				CRoleSprite* pRangePlotObj = g_pGameMain->FindSpriteByID(*it);
				if ( pRangePlotObj == NULL )
					it = m_mapRangePlotObj.erase(it);
				else
				{
					int	iRange			= pRangePlotObj->GetAreaRange();
					int	iDistance		= int(( pPlayer->GetPos() - pRangePlotObj->GetPos() ).Magnitude());

					// 設定觸發
					if( iDistance <= iRange && pRangePlotObj->GetRangePlotTrigger() == false) // && pRangePlotObj )
					{
						RangeEvent( pRangePlotObj->GetIdentify() );
						pRangePlotObj->SetRangePlotTrigger( true );
					}

					// 設定脫離
					if( iDistance >= ( iRange + 30 ) && pRangePlotObj->GetRangePlotTrigger() == true )  // && 
					{
						pRangePlotObj->SetRangePlotTrigger( false );
					}
					it++;
				}
			}
		}

	// 碰撞劇情檢查 NPC
	//-----------------------------------------------------------------------------------------------
		{
			// 檢查脫離上次碰撞點距離 15 之後, 清除所有碰撞資料
			if( m_iEnterCollisionObjID == -1 )
			{
				
				if( pPlayer && (( pPlayer->GetPos() -  m_LastCollisionPos ).Magnitude() > 15 ) )
				{
					m_iEnterCollisionObjID	= 0;
					//m_LastCollisionPos.
					//CollisionEvent( iObjID );
				}								
			}

		}



	return 0;
}

//--------------------------------------------------------------------------------------
void CNetCli_Script_Child::OnQuestAcceptResult ( int iQuestField, int iQuestID, EM_QUESTCHECK_RESULT emResult )
{
	switch( emResult  )
	{
		case EM_QUESTCHECK_RESULT_OK:
			{
				RoleDataEx*		pRole		= CNetGlobal::RoleData();
				

				// 取出 Quest 資料, 並檢查是否有指定打倒目標
				//-----------------------------------------------------------------
				GameObjDbStructEx			*pGameObj	= g_ObjectData->GetObj( iQuestID );

				if( pGameObj == NULL )
					return;

				int				iField		= pRole->Quest_Add( iQuestID );
				//GameObjectQuestDetail	*pQuest		= &( g_ObjectData->GetObj( iQuestID )->QuestDetail );
				//pRole->CheckQuest_RequestKill_SetTempData	( iField, pQuest );
				//pRole->CheckQuest_ClickToGetItem_SetTempData( iField, pQuest );

				//g_pUi_QuestList->ReSet();

				//bool bTrack = false;

				// 檢查如果現在快顯不到 5 個. 則自動加入
					//int iNumRequest		= g_pQuestDetail->GetQuestRequest( iQuestID );
					//if( iNumRequest != 0 && g_pQuestBook->m_setTrackID.size() < 5 )
					//{
					//	g_pQuestBook->m_setTrackID.insert( iQuestID );
					//	bTrack = true;						
					//}

				//if( bTrack )
				//{
				//	g_pInterface->SendWorldEvent( UiEvent_ResetQuestTrack );
				//}


				g_pInterface->SendWorldEvent( UiEvent_ResetQuestBook );
				g_pTutorailFrame->doNewQuest(iQuestID);

				

				if( iQuestField != iField )
				{
					g_pError->AddMessage( 0, 0, "OnQuestAcceptResult() 接任務成. 可是欄位與Srv不同( %d )", iQuestID );		
				}
				else
				{
					char buf[512];
					sprintf(buf, g_ObjectData->GetString( "QUEST_MSG_GET" ), pGameObj->GetName() );

					g_pGameMain->SendSystemMsg(buf);
					g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", buf);
					//g_pChatFrame->SendMsgEvent( CHAT_MSG_SYSTEM, szTempString, "" );
					//g_pError->AddMessage( 2, 0, "OnQuestAcceptResult() 接任務成( %d )", iQuestID );

					//加入追蹤
					lua_State* L = g_pGameMain->GetInterface()->GetLuaState();
					lua_pushnumber( L, iQuestID);	
					lua_setglobal( L, "arg1" );
					g_pInterface->SendWorldEvent( UiEvent_ADDNewQuestBook );

				}

				// 重設物件任務關析
				ResetQuestLink( iQuestID );
			} break;

		case EM_QUESTCHECK_RESULT_ALREADYDONE:
			{
				const char* msg = g_ObjectData->GetString("QUEST_MSG_ALREADYDONE");

				g_pGameMain->SendSystemMsg(msg);
				g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", msg);
			} break;

		case EM_QUESTCHECK_RESULT_FULL:
			{
				const char* msg = g_ObjectData->GetString("QUEST_MSG_FULL");
				g_pGameMain->SendSystemMsg(msg);
				g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", msg);
			} break;


		default:
			{
				g_pError->AddMessage( 0, 0, "OnQuestAcceptResult() 接任務失敗( %d )", emResult );
			}
	}
}
//--------------------------------------------------------------------------------------
void CNetCli_Script_Child::OnQuestCompleteResult	( int iQuestNPCID, int iQuestField, int iQuestID, EM_QUESTCHECK_RESULT emResult )
{
	char szMsg[256];
	char szDailyGroupKeyWord[256];
	char szDailyGroupName[256];

	if( emResult == EM_QUESTCHECK_RESULT_OK )
	{
		{
			lua_State* L = g_pGameMain->GetInterface()->GetLuaState();
		
			lua_pushnumber( L, iQuestID );	// 完成什麼任務 
			lua_setglobal( L, "arg2" );		//教學UI
			g_pInterface->SendWorldEvent("QUEST_COMPLETE");
		}


		RoleDataEx*				pRole		= CNetGlobal::RoleData();
		GameObjDbStructEx		*pGameObj	= g_ObjectData->GetObj( iQuestID );

		if( pGameObj == NULL )
			return;

		pRole->Quest_Complete( iQuestField, iQuestID );

		//g_pUi_QuestList->ReSet();
		g_pInterface->SendWorldEvent( UiEvent_ResetQuestBook );
		g_pInterface->SendWorldEvent( UiEvent_ResetQuestTrack );

		// 訊息提示
		char szMsg[512];
		sprintf(szMsg, g_ObjectData->GetString("QUEST_MSG_FINISHED"), pGameObj->GetName());
		g_pGameMain->SendSystemMsg(szMsg);
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", szMsg);		
		g_pRuGlobalEventManager->PlaySoundByFile("sound\\interface\\QuestFinishReportBack.mp3", false, 0.0f, ruSOUNDTYPE_INTERFACE);

		// 完成任務音效
		g_pRuGlobalEventManager->PlaySoundByFile("sound\\interface\\mission_done.mp3", false, 0.0f, ruSOUNDTYPE_INTERFACE);

		//g_pError->AddMessage( 2, 0, "OnQuestCompleteResult() 任務完成( %d )", iQuestID );


		// 重設物件任務關析
		ResetQuestLink( iQuestID );

		// 檢查有無重覆完成任務及群組設定
			if( (bool)pGameObj->QuestDetail.iCheck_Loop == true )
			{


				// 設定該任務今日已完成
				if( pGameObj->QuestDetail.iControlType == 1 )
				{
					int iID = iQuestID - Def_ObjectClass_QuestDetail;
					if( iID >= 0 && _MAX_QUEST_FLAG_COUNT_ > iID )
					{
						pRole->PlayerBaseData->Quest.QuestDaily.SetFlagOn( iID );

						sprintf( szMsg, g_ObjectData->GetString("QUEST_MSG_DAILY_COMPLETE"), pGameObj->GetName() );
						g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", szMsg );		
					}
				}

				// 設定該任務群組完成次數+!
				if( pGameObj->QuestDetail.iQuestGroup != 0 )
				{
					int iID = pGameObj->QuestDetail.iQuestGroup -1;
					if( iID >= 0 && iID < DF_QUESTDAILY_MAXGROUP )
					{
						if( pGameObj->QuestDetail.iQuestGroup != 3 )
						{
							pRole->PlayerBaseData->Quest.iQuestGroup[ iID ]++;
						}
						

						sprintf( szDailyGroupKeyWord, "Q_GROUPNAME_%02d", ( iID + 1 ) );
						sprintf( szDailyGroupName, g_ObjectData->GetString( szDailyGroupKeyWord ) );


						int iDailyLimit = 10;

						if( pGameObj->QuestDetail.iQuestGroup == 1 )
						{
							GuildBaseStruct * guidlinfo = g_pGuildFrame->GetGuildBaseInfo();
							
							int iTotalLV = (int)g_ObjectData->_GuildLvTable.size();

							//if( guidlinfo != NULL && ( guidlinfo->Level >= 1 && guidlinfo->Level <= 10  ) )
							//{
							//	iDailyLimit = g_ObjectData->SysValue().GuildQuestDailyLimit[ ( guidlinfo->Level - 1 ) ];
							//}
							if( guidlinfo != NULL && ( guidlinfo->Level >= 1 && guidlinfo->Level < iTotalLV  ) )
							{
								iDailyLimit = g_ObjectData->_GuildLvTable[ guidlinfo->Level ].Quest;
							}
							else
								iDailyLimit = 0;
						}


						if( pGameObj->QuestDetail.iQuestGroup != 3 )
						{
							if( pRole->PlayerBaseData->Quest.iQuestGroup[ iID ] < iDailyLimit )
							{
								sprintf( szMsg, g_ObjectData->GetString( "QUEST_MSG_DAILYGROUP_COMPLETE" ), pGameObj->GetName(), ( 10 - pRole->PlayerBaseData->Quest.iQuestGroup[ iID ] ),  szDailyGroupName ); 			
								g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", szMsg );
							}
							else
							{
								sprintf( szMsg, g_ObjectData->GetString( "QUEST_MSG_DAILYGROUP_DONE" ), pGameObj->GetName(), szDailyGroupName );
								g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", szMsg );
							}
						}
					}
				}
			}


		// 執行快顯
		if( pGameObj->QuestDetail.iComplete_ShowQuest != 0 )
		{
			int					iQuestID	= pGameObj->QuestDetail.iComplete_ShowQuest;
			GameObjDbStruct		*pQuestObj	= g_ObjectData->GetObj( iQuestID );

			if( pQuestObj == NULL )
				return;

			if( pRole->CheckQuest_IsAvailable( iQuestID, &( pQuestObj->QuestDetail ), true ) == EM_QUESTCHECK_RESULT_OK )
			{
				// 檢查 NPC 身上有無該任務
				CRoleSprite*		pRoleObj	= g_pGameMain->FindSpriteByID( iQuestNPCID );
				if( pRoleObj )
				{
					int					iQuestNPCID = pRoleObj->GetObjectId();
					GameObjDbStruct*	pObj		= g_ObjectData->GetObj( iQuestNPCID );

					if( pObj )
					{
						for( int index=0; index < _MAX_NPC_QUEST_COUNT_; index++ )
						{
							if( pObj->NPC.iQuestID[ index ] == iQuestID )
							{
								g_pQuestDetail->SetQuest( 0, -1, iQuestID );	// 從 NPC 身上顯示任務

								lua_State* L = g_pGameMain->GetInterface()->GetLuaState();

								lua_pushnumber( L, 1 );	// 設定離開直接離開, 不返回上一頁
								lua_setglobal( L, "arg1" );
								g_pInterface->SendWorldEvent( UiEvent_ShowQuestDetailFromNPC );
								return;
							}
						}
					}
				}

			}
		}


	}
	else
	{
		GameObjDbStructEx			*pGameObj	= g_ObjectData->GetObj( iQuestID );

		// 檢查完成失敗的原因
		switch( emResult )
		{
		case EM_QUESTCHECK_RESULT_DAILYDONE: // 每日任務
			{		
				if( pGameObj->QuestDetail.iControlType == 1 )
				{
					sprintf( szMsg, g_ObjectData->GetString( "QUEST_MSG_DAILY_OVERDONE" ), pGameObj->GetName() );
					g_pGameMain->SendSystemMsg(szMsg);
					g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", szMsg );
				}
			} break;
		case EM_QUESTCHECK_RESULT_GROUPFULL: // 群組已滿
			{
				if( pGameObj->QuestDetail.iQuestGroup != 0 )
				{
					int iID = pGameObj->QuestDetail.iQuestGroup -1;
					if( iID >= 0 && iID < DF_QUESTDAILY_MAXGROUP )
					{
						sprintf( szDailyGroupKeyWord, "Q_GROUPNAME_%02d", ( iID + 1 ) );
						sprintf( szDailyGroupName, g_ObjectData->GetString( szDailyGroupKeyWord ) );
						sprintf( szMsg, g_ObjectData->GetString( "QUEST_MSG_DAILYGROUP_OVERDONE" ), szDailyGroupName );
						g_pGameMain->SendSystemMsg(szMsg);
						g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", szMsg );
					}
				}
			} break;
		}
		g_pError->AddMessage( 0, 0, "OnQuestCompleteResult() 任務完成失敗( %d )", emResult );
	}
}
//--------------------------------------------------------------------------------------
void CNetCli_Script_Child::OnQuestDeleteResult		( int iQuestField, int iQuestID, EM_QUESTCHECK_RESULT emResult )
{
	if( emResult == EM_QUESTCHECK_RESULT_OK )
	{
		RoleDataEx*		pRole		= CNetGlobal::RoleData();	

		//pRole->Quest_Complete( iQuestField, iQuestID );

		GameObjDbStructEx			*pGameObj	= g_ObjectData->GetObj( iQuestID );

		if( pGameObj == NULL )
			return;

		pRole->Quest_Delete( iQuestField, iQuestID );

		// 重設物件任務關析
			ResetQuestLink( iQuestID );

		g_pQuestBook->UpdateQuestItem(iQuestID);
		
		g_pInterface->SendWorldEvent( UiEvent_ResetQuestBook );
		g_pInterface->SendWorldEvent( UiEvent_ResetQuestTrack );

		g_pGameMain->SendSystemMsgEx( g_ObjectData->GetString( "QUEST_MSG_ALREADY_GIVEUP" ), pGameObj->GetName() );
		//g_pError->AddMessage( 2, 0, "OnQuestDeleteResult() 任務刪除( %d )", iQuestID );				

		// 放棄音效
		g_pRuGlobalEventManager->PlaySoundByFile("sound\\interface\\mission_giveup.mp3", false, 0.0f, ruSOUNDTYPE_INTERFACE);
	}
	else
	{
		g_pError->AddMessage( 0, 0, "OnQuestDeleteResult() 任務刪除失敗( %d )", emResult );
	}
}

//--------------------------------------------------------------------------------------
void CNetCli_Script_Child::OnQuestFlag ( int iQuestID,	int iValue )
{
	RoleDataEx*		pRole		= CNetGlobal::RoleData();

	if( pRole )
	{
		if( iQuestID == -1 )
		{
			pRole->PlayerBaseData->Quest.QuestHistory.ReSet();
		}
		else
		{
			if( iValue == 1 )
				pRole->PlayerBaseData->Quest.QuestHistory.SetFlagOn ( iQuestID );
			else
				pRole->PlayerBaseData->Quest.QuestHistory.SetFlagOff ( iQuestID );
		}
	}
}
//--------------------------------------------------------------------------------------
void CNetCli_Script_Child::OnKillObj ( int iRoleID )
{

	/*
	CRoleSprite* pDeadObj = g_pGameMain->FindSpriteByID( iRoleID );

	if( pDeadObj )
	{

		RoleDataEx*		pRole		= CNetGlobal::RoleData();
		QuestTempDataStruct*	pQuestTempData	= &( pRole->TempData.QuestTempData );

		int iTempIndex	= pRole->CheckQuest_RequestKill_AddCount( pDeadObj->GetObjectId() );

		
		if( iTempIndex != -1 )
		{

			int iQuestField	= pRole->CheckQuest_RequestKill_Complete( iTempIndex );

			if( iQuestField != -1 )
			{
				// 有 Kill Quest 完成, 檢查任務條件是不是只有指定打倒目標, 是的話可以秀出訊息通知任務完成


			}
		}
	}
	*/
	
}

/*
//--------------------------------------------------------------------------------------
void CNetCli_Script_Child::OnQuestDetail		( int iQuestID , int iMode )
{
	//int iQuestGUID						= Def_ObjectClass_QuestDetail + iQuestID;
	GameObjDbStruct* pQuestObj			= g_ObjectData->GetObj( iQuestID );
	GameObjectQuestDetail* pQuestDetail	= NULL;

	if( pQuestObj )
	{
		pQuestDetail = &( pQuestObj->QuestDetail );
	}

	if( pQuestDetail )
	{
		// 秀出 Quest 完整的訊息

		g_pUi_QuestDialog->ReSet();
		g_pUi_QuestDialog->Set_QuestID		( iQuestID, iMode );
		g_pUi_QuestDialog->Set_NpcName		( m_pLastTouchObj->GetName() );
		g_pUi_QuestDialog->Set_QuestName	( pQuestObj->Name );
		
		switch( iMode )
		{
		case 0:
		case 1:
			{
				g_pUi_QuestDialog->Set_QuestDesc	( pQuestDetail->szQuest_Desc );
				g_pUi_QuestDialog->Set_QuestDetail	( pQuestDetail->szQuest_Accept_Detail );
			} break;
		case 2:
			{
				g_pUi_QuestDialog->Set_QuestDesc	( pQuestDetail->szQuest_Desc );
				g_pUi_QuestDialog->Set_QuestDetail	( pQuestDetail->szQuest_Complete_Detail );
			} break;
		case 10:
			{
				g_pUi_QuestDialog->Set_QuestDesc	( pQuestDetail->szQuest_Desc );
				g_pUi_QuestDialog->Set_QuestDetail	( pQuestDetail->szQuest_Complete_Detail );
			} break;

		}

		g_pUi_QuestDialog->Show();
	}

}
//--------------------------------------------------------------------------------------
void CNetCli_Script_Child::OnUpdate			()
{


}




//--------------------------------------------------------------------------------------
void CNetCli_Script_Child::AcceptQuest ( int iQuestID )
{
	CNetCli_Script::AcceptQuest( m_pLastTouchObj->GetIdentify(), iQuestID );
}
//--------------------------------------------------------------------------------------
void CNetCli_Script_Child::CompleteQuest ( int iQuestID )
{
	CNetCli_Script::CompleteQuest( m_pLastTouchObj->GetIdentify(), iQuestID ); 
}
*/
//--------------------------------------------------------------------------------------
/*
void CNetCli_Script_Child::OnKillObj ( CRoleSprite* pDeadObj )
{
	RoleDataEx*				pRole			= g_pGameMain->RoleData();
	QuestTempDataStruct*	pQuestTempData	= &( pRole->TempData.QuestTempData );

	int iTempIndex	= pRole->CheckQuest_RequestKill_AddCount( pDeadObj->GetObjectId() );
	
	/*
	if( iTempIndex != -1 )
	{
		
		int iQuestField	= pRole->Check_QuestKill( iTempIndex );
		
		if( iQuestField != -1 )
		{
			// 有 Kill Quest 完成, 檢查任務條件是不是只有指定打倒目標, 是的話可以秀出訊息通知任務完成


		}
	}
	*/
/*	
}
*/
int CNetCli_Script_Child::CheckQuestObj ( int iQuestObjID )
{
	GameObjDbStruct*	pObjDB			= g_ObjectData->GetObj( iQuestObjID );
	RoleDataEx*			pRole			= CNetGlobal::RoleData();
	int					iResult			= -1;
	bool				bRequestItem	= false;

	if( pObjDB == NULL )
		return -1;

	for( int i = 0; i < _MAX_QUESTLINK_COUNT; i++ )
	{
		for( int j = 0; j < _MAX_QUEST_COUNT_; j++ )
		{	
			//if( pObjDB->NPC.pQuestLink[ i ] && pObjDB->NPC.pQuestLink[ i ]->GUID == pRole->PlayerBaseData->Quest.QuestLog[j].iQuestID  )
			if( pObjDB->NPC.iQuestLinkObj[ i ] != 0 && pObjDB->NPC.iQuestLinkObj[ i ] == pRole->PlayerBaseData->Quest.QuestLog[j].iQuestID  )
			{
				//GameObjectQuestDetail	*pQuest	= &( pObjDB->NPC.pQuestLink[ i ]->QuestDetail );
				GameObjectQuestDetail*	pQuest		= NULL;
				GameObjDbStruct*		pQuestObj	= g_ObjectData->GetObj( pObjDB->NPC.iQuestLinkObj[ i ] );

				if( pQuestObj != NULL )
				{
					pQuest = &( pQuestObj->QuestDetail );
				}
				else
				{
					return -1;
				}


				

				for( int k = 0; k < _MAX_SIZE_QUEST_ITEM; k++ )
				{
					if( pQuest->iProcess_ClickToGetItem_ObjID[ k ] == iQuestObjID )
					{
						bRequestItem = true;
						for( int l = 0; l < _MAX_SIZE_QUEST_ITEM; l++ )
						{
							if( pQuest->iProcess_ClickToGetItem_GetItem[ k ] == pQuest->iRequest_ItemID[ l ] )
							{
								// 檢查數量
								bool	bCheck 		= pRole->Sc_CheckItemFromRole( pQuest->iRequest_ItemID[  l ] , pQuest->iRequest_ItemVal[ l ] );

								if( bCheck )
								{
									//g_pChatFrame->AddMessage(9, "已取得足夠的物品" );
									return -1;	// 已達到要求的數量, 不再給予
								}
								else
									break;								


							}
						}
					}
				}

				// 發送封包給 Server 預計特定時間後送回事件						
				//ClickQuestObj( j, pRole->PlayerBaseData->Quest.QuestLog[j].iQuestID, iObjSpireID, iObjGUID  );
				
				if( bRequestItem == true )	// 檢查是否有在完成任務需求物品中
					return j;
				else
					return -1;
			}
		}
	}

	return -1;
}
//--------------------------------------------------------------------------------------
void CNetCli_Script_Child::OnTouchQuestObj( int iObjSpireID )
{
	CRoleSprite*	pPlayer				= g_pGameMain->GetPlayer();
	CRoleSprite*	pObj				= g_pGameMain->FindSpriteByID( iObjSpireID );
	CRoleSprite*	pLastClickQuestObj	= g_pGameMain->FindSpriteByID( m_iClickQuestObjID );
	
	if( pObj == NULL )
		return;

	if( m_iClickQuestObjID != 0 && m_iClickQuestObjID == iObjSpireID )
		return;

	if( m_iClickQuestObjID == 0 || pLastClickQuestObj == NULL )
	{
		m_iClickQuestObjID = iObjSpireID;
	}
	else
	{
		// 如果上個點選的物件超過 _MAX_ROLE_TOUCH_RANGE_ ( 40 ) 則重設物件. 有 BUG 存在
		if( pPlayer != NULL && pLastClickQuestObj != NULL )
		{
			CRuVector3			dir;
			float				magnitude;

			dir			= pPlayer->GetPos() - pLastClickQuestObj->GetPos();
			dir.m_y		= 0;
			magnitude	= dir.Magnitude();

			if( magnitude > _MAX_ROLE_TOUCH_RANGE_ )
			{
				m_iClickQuestObjID = iObjSpireID;
			}
		}
		else
			return;
	}
	
	RoleDataEx*			pRole			= CNetGlobal::RoleData();
	int					iObjGUID		= pObj->GetObjectId();
	int					iQuestField		= CNetCli_Script_Child::CheckQuestObj( iObjGUID );

	// 發送封包給 Server 預計特定時間後送回事件
	if( iQuestField != -1 )
	{
		ClickQuestObj( iQuestField, pRole->PlayerBaseData->Quest.QuestLog[ iQuestField ].iQuestID, iObjSpireID, iObjGUID  );

		// 自動下馬,不能在騎乘時做用
		g_pGameMain->Dismount();

		// 開始時間條
		g_pCastingBarFrame->CastingStart(eCastingBar_Quset, pObj->GetName(), (_TIME_CLICKTOGETITEM/1000.0f));

		g_pGameMain->GetPlayer()->SetActorState( ruFUSION_ACTORSTATE_CRAFTING_BEGIN );
	}
	else
	{
		m_iClickQuestObjID = 0;
	}
}
//--------------------------------------------------------------------------------------
void CNetCli_Script_Child::OnClickQuestObjResult	( int iResult, int iItemID, int iItemVal )
{
	m_iClickQuestObjID = 0;

	switch( iResult )
	{
	case EM_GATHER_SUCCESSED:
	case EM_GATHER_ENDCAST: // Okay
		{
			g_pCastingBarFrame->CastingStop(eCastingBar_Quset);
			g_pGameMain->GetPlayer()->SetActorState( ruFUSION_ACTORSTATE_CRAFTING_END );

		} break;
	case EM_GATHER_OBJBUSY:	// 物件忙錄中
		{
			g_pChatFrame->SendMsgEvent( CHAT_MSG_SYSTEM, g_ObjectData->GetString( "GATHER_MSG_OBJBUSY"), "" );
			g_pCastingBarFrame->CastingFailed(eCastingBar_Quset);			
			g_pGameMain->GetPlayer()->SetActorState( ruFUSION_ACTORSTATE_CRAFTING_END );
		} break;
	default:	
		{
			g_pChatFrame->SendMsgEvent( CHAT_MSG_SYSTEM, g_ObjectData->GetString( "QUEST_MSG_GATHERFAILED"), "" );
			g_pCastingBarFrame->CastingFailed(eCastingBar_Quset);
			g_pGameMain->GetPlayer()->SetActorState( ruFUSION_ACTORSTATE_CRAFTING_END );
		} break;
	}

}
//--------------------------------------------------------------------------------------
void CNetCli_Script_Child::OnKillQuestObjNotify ( int iQuestField, int iQuestID, int iDeadObjID )
{
	RoleDataEx*		pRole		= CNetGlobal::RoleData();

	if( pRole->PlayerBaseData->Quest.QuestLog[ iQuestField ].iQuestID == iQuestID )
	{
		GameObjDbStructEx			*pObjDB		= g_ObjectData->GetObj( iQuestID );
		
		if( pObjDB )
		{
			GameObjectQuestDetail	*pQuest			= &( pObjDB->QuestDetail );

			for( int i = 0; i < _MAX_SIZE_QUEST_ITEM; i++ )
			{
				if( pQuest->iRequest_KillID[ i ] == iDeadObjID )
				{
					//pRole->PlayerBaseData->Quest.iQuestKillCount[ iQuestField ][ i ]++;

					pRole->PlayerBaseData->Quest.QuestLog[ iQuestField ].iQuestKillCount[ i ]++;
				
					lua_State* L = g_pGameMain->GetInterface()->GetLuaState();
					lua_pushnumber( L, iQuestID);	
					lua_setglobal( L, "arg1" );
					g_pInterface->SendWorldEvent( UiEvent_CheckGetItemBeQueueItem );
					

					GameObjDbStructEx	*pDeadObj	= g_ObjectData->GetObj( iDeadObjID );
					assert(pDeadObj);


					//QUEST_MSG_KILL_MONSTER_FORMAT <quest>:<type><monster> <count>/<max> <complete>
					string strTemp = g_ObjectData->GetString( "QUEST_MSG_KILL_MONSTER_FORMAT" );

 					strTemp = ReplaceKeyString( strTemp, "<quest>", pObjDB->GetName() );

					// 檢查有無前置字串
					if( pQuest->iRequest_KillStr[ i ] != 0 )
					{
						string strType = g_ObjectData->GetString( Com_FormatString( "Q_REQUEST_KILLSTR_%02d", pQuest->iRequest_KillStr[ i ] ) );
						strTemp = ReplaceKeyString( strTemp, "<type>", strType );
					}
					else
					{
						strTemp = ReplaceKeyString( strTemp, "<type>", "" );
					}

					strTemp = ReplaceKeyString( strTemp, "<monster>", pDeadObj->GetName() );

					strTemp = ReplaceKeyString( strTemp, "<count>", pRole->PlayerBaseData->Quest.QuestLog[ iQuestField ].iQuestKillCount[ i ] );
					strTemp = ReplaceKeyString( strTemp, "<max>", pQuest->iRequest_KillVal[ i ] );

					// 附加完成
					if( pRole->PlayerBaseData->Quest.QuestLog[ iQuestField ].iQuestKillCount[ i ] == pQuest->iRequest_KillVal[ i ] )
					{
						char temp[256];
						sprintf(temp, "(%s)", g_ObjectData->GetString("QUEST_S_REQUEST_COMPLETE") );
						strTemp = ReplaceKeyString( strTemp, "<complete>", temp );
						g_pChatFrame->SendWorldEvent("QUEST_REQUEST_COMPLETE");// 送出事件給LUA
					}
					else
					{
						strTemp = ReplaceKeyString( strTemp, "<complete>", "" );
					}

					g_pGameMain->SendSystemMsg( strTemp.c_str() );
					g_pGameMain->SendSystemChatMsg( strTemp.c_str() );

					// 發出事件
					g_pInterface->SendWorldEvent( UiEvent_ResetQuestBook );
					g_pInterface->SendWorldEvent( UiEvent_ResetQuestTrack );
					return;
				}
			}
		}
		else
		{
			g_pChatFrame->AddMessage(9, "SYS: OnQuestKillObjNotify() 取不到資料結構" );
		}		

	}
	else
	{
		// 資料有錯 Error
		g_pChatFrame->AddMessage(9, "SYS: OnQuestKillObjNotify() 欄位不符合" );
	}
}

void CNetCli_Script_Child::OnPlayMotion ( int iObjID, const char* pszMotion )
{
	CRoleSprite* pObj = g_pGameMain->FindSpriteByID( iObjID );

	if( pObj )
	{
		//int iMotionID = IBaseSprite::GetMotionEnum( pszMotion );
		//if( iMotionID != 0 )	
			//pObj->Motion( iMotionID );
	}

}

void CNetCli_Script_Child::OnSpeakOption ( int iCliID , int iSize , PVOID pData )
{	
	RoleDataEx*			pRole		= CNetGlobal::RoleData();
	SpeakOptionStruct	option;

	// 重置對話資料
	pRole->PlayerTempData->SpeakInfo.Reset();
	// 清除選項
	//pRole->PlayerTempData->SpeakInfo.SpeakOption.Clear();

	char*				ptr			= (char*)pData;

	GamePGCommandEnum	Command;
	int					iLen;
	int					iIconID;
	int					iOptionCount;
	int					iQuestNPCID;
	int					iLoadQuest;
	char				szBuff[ 4096 ];

	Command			= *((GamePGCommandEnum*)ptr);	
	ptr				+= sizeof( GamePGCommandEnum );
	
	// 任務 NPC 的 GUID
	iQuestNPCID		= *((int*)ptr);
	ptr				+= sizeof( int );

	// 是否讀取任務
	iLoadQuest		= *((int*)ptr);
	ptr				+= sizeof( int );

	// Option 數量
	iOptionCount	= *((int*)ptr);
	ptr				+= sizeof( int );

	// Detail Len
	iLen			= *((int*)ptr);
	ptr				+= sizeof( int );


	// Detail
		if( iLen != 0 ) // 當點擊目標為任務 NPC 時, 對話不會載入
		{
			strcpy( szBuff, ptr );	
			ptr				+= iLen;



			// EDIT0111
			string sString = CNetCli_Script_Child::ReplaceSystemKeyword( szBuff );
			//pRole->PlayerTempData->SpeakInfo.SpeakDetail = sString.c_str();
			((CNetCli_Script_Child*)CNetCli_Script_Child::m_pThis)->m_SpeakDetail = sString.c_str();

		}

	// Option
		for( int i =0; i < iOptionCount; i++ )
		{
			// Option Len
			iIconID			= *((int*)ptr);
			ptr				+= sizeof( int );

			// Option Len
			iLen			= *((int*)ptr);
			ptr				+= sizeof( int );

			// Option Text
			strcpy( szBuff, ptr );
			ptr				+= iLen;

			// EDIT0111
			//strcpy( option.szOption, g_ObjectData->GetString( szBuff ) );
			//strcpy( option.szOption, szBuff );

			option.iIconID	= iIconID;

			string sString = CNetCli_Script_Child::ReplaceSystemKeyword( szBuff );
			option.szOption = sString.c_str();

			//
			if( option.iIconID == 0 )
			{
				option.iIconID = EM_MapIconType_Speak;
			}

			pRole->PlayerTempData->SpeakInfo.SpeakOption.Push_Back( option );

		}

	// NPCID
		pRole->PlayerTempData->SpeakInfo.iSpeakNPCID	= iQuestNPCID;


	// LoadQuest
	if( iLoadQuest != 0 )
		g_pQuestList->SetQuestNPC( iQuestNPCID );
	else
		g_pQuestList->Clear();

	g_pInterface->SendWorldEvent(UiEvent_ShowQuestList);


	int a = 3;
	/*
	char*	ptr				= (char*)pData;
	
	int		iszLen			= 0;
	int		iOptionCount	= 0;
	string	sDetail, sOption, sLuaFunc;

	char	szBuff[ _MAX_SPEAKDETAIL_SIZE_ ];

	iOptionCount	= *((int*)ptr);	ptr+= sizeof(int);
	
	iszLen			= *((int*)ptr); ptr+= sizeof(int);
	strcpy( szBuff, ptr, iszLen );
	sDetail			= szBuff;

	iszLen			= *((int*)ptr); ptr+= sizeof(int);
	strcpy( szBuff, ptr, iszLen );
	*/




	


}
//--------------------------------------------------------------------------------------
void CNetCli_Script_Child::OnChoiceOptionResult()
{
	CUiFrame* pFrame = (CUiFrame*)g_pGameMain->GetInterface()->GetUiWorld()->FindObject( "SpeakFrame" );
	if( pFrame != NULL )
		pFrame->SetEnable( true );

	m_bSendingOption = false;
}
//--------------------------------------------------------------------------------------
void CNetCli_Script_Child::OnCloseSpeak()
{
	g_pInterface->SendWorldEvent(UiEvent_CloseSpeakFrame);
}
//--------------------------------------------------------------------------------------
void CNetCli_Script_Child::OnPlaySound ( int iObjID, const char* pszSoundFile, bool bLoop, int iSoundID )
{
	CRoleSprite* pObj = g_pGameMain->FindSpriteByID( iObjID );

	if( pObj )
	{
		g_pRuGlobalEventManager->PlaySoundByFile(pszSoundFile, bLoop, 0.0f, ruSOUNDTYPE_INTERFACE, iSoundID);
	}
}
//--------------------------------------------------------------------------------------
void CNetCli_Script_Child::OnPlay3DSound ( int iObjID, const char* pszSoundFile, bool bLoop, int iSoundID )
{
	CRoleSprite* pObj = g_pGameMain->FindSpriteByID( iObjID );

	if( pObj )
	{
		g_pRuGlobalEventManager->Play3DSoundByFile(pszSoundFile, pObj->GetPos(), bLoop, 0.0f, ruSOUNDTYPE_SFX, iSoundID);
	}


}
//--------------------------------------------------------------------------------------
void CNetCli_Script_Child::OnPlayMusic ( int iObjID, const char* pszSoundFile, bool bLoop, int iSoundID )
{
	CRoleSprite* pObj = g_pGameMain->FindSpriteByID( iObjID );

	if( pObj )
	{
		g_pRuGlobalEventManager->PlaySoundByFile(pszSoundFile, bLoop, 0.0f, ruSOUNDTYPE_MUSIC, iSoundID);
	}


}
//--------------------------------------------------------------------------------------
void CNetCli_Script_Child::OnModifyScriptState ( CRoleSprite* pObj )
{
	set< int >::iterator it; 

	if( pObj == NULL )
		return;
	
	it = m_mapRangePlotObj.find( pObj->GetIdentify() );

	// 範圍事件檢查
	if( pObj->GetAreaRange() == 0 )
	{
		if( it != m_mapRangePlotObj.end() )
		{
			m_mapRangePlotObj.erase( it );
		}
	}
	else
	{
		// 在資料中找不到人物物件, 便插入
		if( it == m_mapRangePlotObj.end() )
		{
			m_mapRangePlotObj.insert( pObj->GetIdentify() );
		}
	}
}
//--------------------------------------------------------------------------------------
void CNetCli_Script_Child::OnQuestState( int iQuestID, int iState )
{
	RoleDataEx*			pRole		= CNetGlobal::RoleData();

	int iQuestField = pRole->CheckQuestList_IsGotQuest( iQuestID );

	if( iQuestField != -1 )
	{
		pRole->PlayerBaseData->Quest.QuestLog[ iQuestField ].iMode = iState;

		if( iState == -1 )
		{
			// Quest failed
			// Show a messagebox to notice client, "Your quest is failed"
			GameObjDbStructEx*	pObj		= g_ObjectData->GetObj( iQuestID );
			
			if( pObj != NULL )
			{
				lua_State* L = g_pGameMain->GetInterface()->GetLuaState();

				lua_pushnumber( L, iQuestID );
				lua_setglobal( L, "arg1");

				lua_pushstring( L, pObj->Name );
				lua_setglobal( L, "arg2");

				g_pInterface->SendWorldEvent( "EVENT_QUEST_FAILED" );
			}
		}

		
	}

	// 若是設定任務失敗, 則印出任務失敗的訊息 [xxxx]任務失敗
}
/*
//--------------------------------------------------------------------------------------
void CNetCli_Script_Child::OnEvent_CastInterrupted ()
{
	int a = 3;

}
//--------------------------------------------------------------------------------------
void CNetCli_Script_Child::OnEvent_CastFinished ()
{
	int a = 3;

}

//--------------------------------------------------------------------------------------
*/
void CNetCli_Script_Child::OnClearBorder ()
{
	g_pScriptBorder->Clear();
}
//--------------------------------------------------------------------------------------
void CNetCli_Script_Child::OnAddBorderPage( const char* pszAddPage )
{
	string sString = CNetCli_Script_Child::ReplaceSystemKeyword( pszAddPage );
	g_pScriptBorder->m_vecBorderPage.push_back( sString );
}
//--------------------------------------------------------------------------------------
void CNetCli_Script_Child::OnShowBorder( int iQuestID, const char* pszTitle, const char* pszTexture )
{
	RoleDataEx*			pRole		= CNetGlobal::RoleData();

	if( iQuestID != 0 )
	{	
		int					iQuestField	= pRole->CheckQuestList_IsGotQuest( iQuestID );
		GameObjDbStructEx	*pGameObj	= g_ObjectData->GetObj( iQuestID );

		if( pGameObj == NULL )
			return;

		if( iQuestField != -1 )
		{
			g_pGameMain->SendSystemMsgEx( g_ObjectData->GetString( "QUEST_MSG_ALREADY_GET" ), pGameObj->GetName() );
			return;
		}
	}


	g_pScriptBorder->m_sTitle			= CNetCli_Script_Child::ReplaceSystemKeyword( pszTitle );
	g_pScriptBorder->m_sTexture			= pszTexture;
	g_pScriptBorder->m_iBorderQuestID	= iQuestID;

	g_pScriptBorder->ShowBorder();
}
//--------------------------------------------------------------------------------------
void CNetCli_Script_Child::OnEnterTrigger					( int iObjID	)
{
	CRoleSprite*	pPlayer		= g_pGameMain->GetPlayer();

	if( m_iEnterCollisionObjID != iObjID && m_iEnterCollisionObjID == 0 )
	{
		m_iEnterCollisionObjID	= iObjID;
		m_LastCollisionPos		= pPlayer->GetPos();
			CollisionEvent( iObjID );
	}
}
//--------------------------------------------------------------------------------------
void CNetCli_Script_Child::OnLeaveTrigger					( int iObjID	)
{
	m_iEnterCollisionObjID = -1;
}
//--------------------------------------------------------------------------------------
void CNetCli_Script_Child::OnScriptCast ( const char* pszString, int iTime, int iMotionID, int iMotionEndID )
{
	string sRangeString = CNetCli_Script_Child::ReplaceSystemKeyword( pszString );

	// 開始時間條
	g_pCastingBarFrame->CastingStart( eCastingBar_Quset, sRangeString.c_str(), ( iTime / 1000.0f ) );
	
	// 開始放動作
	m_iScriptCastMotionEnd = iMotionEndID;

	if ( g_pGameMain->GetPlayer() )
		g_pGameMain->GetPlayer()->SetActorState( (CRuFusion_ActorState)iMotionID );
}
//--------------------------------------------------------------------------------------
void CNetCli_Script_Child::OnScriptCastResult ( int iResult )
{
	switch( iResult )
	{
	case EM_GATHER_SUCCESSED:
		{
			//g_pGameMain->GetPlayer()->SetActorState( (CRuFusion_ActorState)m_iScriptCastMotionEnd );
			g_pCastingBarFrame->CastingStop(eCastingBar_Quset);
		} break;
	default:
		{
			g_pCastingBarFrame->CastingFailed(eCastingBar_Quset);
		} break;
	}

	if( m_iScriptCastAttachToolID != 0 )
	{
		m_iScriptCastAttachToolID = 0;
		g_pGameMain->GetPlayer()->DetachCraftingTool();
	}

	if ( g_pGameMain->GetPlayer() )
	{
		g_pGameMain->GetPlayer()->SetActorState( (CRuFusion_ActorState)m_iScriptCastMotionEnd );
	}
}
//--------------------------------------------------------------------------------------
bool CNetCli_Script_Child::CheckQuestlink( GameObjDbStructEx* pObj )
{
	RoleDataEx*			pRole		= CNetGlobal::RoleData();

	if( pObj == NULL )
		return false;

	if( !( pObj->IsNPC() || pObj->IsQuestNPC() ) )
		return false;

	int iQuestID	= 0;
	for( int i=0; i< _MAX_QUESTLINK_COUNT;i++)
	{
		for( int j=0; j<_MAX_QUEST_COUNT_; j++ )
		{
			//if( pObj->NPC.pQuestLink[i] != NULL )

			iQuestID = pObj->NPC.iQuestLinkObj[i];
			if( iQuestID != 0 && iQuestID == pRole->PlayerBaseData->Quest.QuestLog[j].iQuestID )
			{				
				return true;
			}
		}
	}

	return false;
}
//--------------------------------------------------------------------------------------
void CNetCli_Script_Child::ResetQuestLink( int iQuestID )
{
	RoleDataEx*			pRole		= CNetGlobal::RoleData();
	map<int, CRoleSprite*>& SpriteMap = g_pRuneDev->GetSpriteMap();

	bool bFoundQuest = false;
	
	for( int j=0; j<_MAX_QUEST_COUNT_; j++ )
	{
		if( iQuestID == pRole->PlayerBaseData->Quest.QuestLog[j].iQuestID )
		{
			bFoundQuest = true;
			break;
		}	
	}

	if( bFoundQuest == true )
	{
		// 將有關聯的 NPC 及任務物件設關希
		for( map<int, CRoleSprite*>::iterator it = SpriteMap.begin(); it != SpriteMap.end(); it++ )
		{
			CRoleSprite*		pSprite			= it->second;
			int					iObjGUID		= pSprite->GetObjectId();
			GameObjDbStructEx*	pObjDB			= NULL;
			bool				bQuestLink		= false;

			if( ( pObjDB = g_ObjectData->GetObj( iObjGUID ) ) == NULL )
				continue;

			if( !( pObjDB->IsNPC() || pObjDB->IsQuestNPC() ) )
				continue;

			for( int i = 0; i < _MAX_QUESTLINK_COUNT; i++ )
			{
				//if( pObjDB->NPC.pQuestLink[ i ] != NULL && pObjDB->NPC.pQuestLink[ i ]->GUID == iQuestID )
				if( pObjDB->NPC.iQuestLinkObj[ i ] != 0 && pObjDB->NPC.iQuestLinkObj[ i ] == iQuestID )
				{
					if( pSprite->GetCheckedStatus() == eChecked_QuestObj )
					{
						pSprite->SetQuestMinimapIcon( (int)EM_MapIconType_QuestOBJ );
						
						if( pObjDB->NPC.IgnoreQuestObjEffect == false )
							pSprite->CreateCorpseEffect();
					}
					else
					{
						pSprite->SetQuestMinimapIcon( (int)EM_MapIconType_QuestNPC );
					}

					break;
				}
			}
		}

	}
	else
	{
		// 將原本關聯的 NPC 及任務物件關希關掉
		for( map<int, CRoleSprite*>::iterator it = SpriteMap.begin(); it != SpriteMap.end(); it++ )
		{
			CRoleSprite*		pSprite			= it->second;
			int					iObjGUID		= pSprite->GetObjectId();
			GameObjDbStructEx*	pObjDB			= NULL;
			bool				bQuestLink		= false;

			if( ( pObjDB = g_ObjectData->GetObj( iObjGUID ) ) == NULL )
				continue;

			if( !( pObjDB->IsNPC() || pSprite->GetCheckedStatus() == eChecked_QuestObj ) )
				continue;

			for( int i = 0; i < _MAX_QUESTLINK_COUNT; i++ )
			{
				//if( pObjDB->NPC.pQuestLink[ i ] != NULL && pObjDB->NPC.pQuestLink[ i ]->GUID == iQuestID )
				if( pObjDB->NPC.iQuestLinkObj[i] != 0 && pObjDB->NPC.iQuestLinkObj[ i ] == iQuestID )
				{
					pSprite->SetQuestMinimapIcon( (int)EM_MapIconType_None );
					pSprite->ReleaseCorpseEffect();
					break;
				}
			}
		}
	}
}
//--------------------------------------------------------------------------------------
void CNetCli_Script_Child::OnResetDailyQuest( int iType )
{
	RoleDataEx*			pRole		= CNetGlobal::RoleData();
	
	if( iType == 1 )
	{	
		// 重置每日任務的資料並告知 Client

		// 重置每日任務
		pRole->PlayerBaseData->Quest.QuestDaily.ReSet();

		// 重置群組任務
		for( int i=0; i<DF_QUESTDAILY_MAXGROUP; i++ )
		{
			pRole->PlayerBaseData->Quest.iQuestGroup[i] = 0;
		}

		// 更新紀錄
		pRole->PlayerBaseData->Quest.LastCompleteTime = 0;

		// 發訊息給 Client
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString( "QUEST_MSG_DAILYRESET" ) );

		//為了重置大地圖任務ICON
		g_pInterface->SendWorldEvent( UiEvent_ResetQuestBook );

	}

}
//--------------------------------------------------------------------------------------
string CNetCli_Script_Child::ReplaceSystemKeyword ( string pSource, bool bQuestColor, bool bHyperLink, bool bNoloopCheckVar )
{
	if ( pSource.empty() )
		return "";

	if( strcmp( pSource.c_str(), m_pSource.c_str() ) == 0 && m_bQuestColor == bQuestColor && m_bHyperLink == bHyperLink && m_bNoloopCheckVar == bNoloopCheckVar )
	{
		return m_lastResult;
	}

	size_t	iBegin		= 0;
	size_t	iEnd		= 0;
	size_t	iEqualPos	= 0;

	char	szKeyWord[8192];
	//char	szString[8192];
	char	szVarKeyWord[8192];

	int		off = 0;

	map< int, string >	mapVarString;

	bool	bReplace	= false;

	// EXAMPLE:	"Go to kill <$VAR1>, and <$VAR2>. then go to <ZONE_ASHLA><$SETVAR1=asdf><$SETVAR2=<$PLAYERNAME>>"

	// 先檢查有無<$VAR1> .. <$VARN>, 有的話, 從後面取出,  取代並代換字串

	// 先找出替代變數 <$SETVARN=XXXX>

	// 由於有可能 <$SETVAR2=<$PLAYERNAME>>" , 也就是替代變數也是系統變數的情況, 所以 = 後面, 要多搜尋一次 >


	while(1)
	{
		bReplace		= false;
		off				= 0;
		iBegin			= 0;
		iEnd			= 0;
		iEqualPos		= 0;
		

	// 取出替代變數
		
		while( 1 )
		{

			iBegin = pSource.find( "[" , off );
			if( iBegin == string::npos )
			{
				break;
			}
			else
			{
				off = (int)iBegin + 1;
			}

			iEnd = pSource.find( "]" , off );
			if( iEnd == string::npos || iBegin >= iEnd )
			{
				break;
			}
			else
			{
				iEqualPos = pSource.find( "=", off );

				if( iEqualPos < iEnd )
				{
					int iAnotherBegin = (int)(pSource.find( "[" , iEqualPos ));

					if( iAnotherBegin < iEnd )
					{
						int iAnotherEnd = (int)(pSource.find( "]", ( iEnd + 1) ));
						if( iAnotherEnd == string::npos || iEnd >= iAnotherEnd )
						{
							break;
						}
						else
						{
							iEnd	= iAnotherEnd;
							off		= iAnotherEnd + 1;
						}
					}
				}			
				else
				{
					off = (int)iEnd + 1;
				}
			}

			int iKeyWordLen = (int)(iEnd - iBegin - 1);

			if( iKeyWordLen > 7 )
			{
				ZeroMemory( szKeyWord, sizeof( szKeyWord ) );
				ZeroMemory( szVarKeyWord, sizeof( szVarKeyWord ) );

				memcpy( szVarKeyWord, ( pSource.c_str() + ( iBegin + 1 ) ), 7 );
				memcpy( szKeyWord, ( pSource.c_str() + ( iBegin + 1 ) ), iKeyWordLen );

				
				if( stricmp( szVarKeyWord, "$SETVAR" ) == 0 )
				{
					int		iVarID		= 0;
					int		iVarID_End	= 0;
					char	szVarString[256];


					ZeroMemory( szVarString, sizeof( szVarString ) );

					// 找出 VAR 'N'
					for( int i = 7; i < iKeyWordLen; i++ )
					{
						if( szKeyWord[i] == '=' )
						{
							iVarID_End = i;
							break;
						}
					}
					
					if( iVarID_End != 0 )
					{
						char szVarID[256];
						ZeroMemory( szVarID, sizeof( szVarID ) );

						memcpy( szVarID, &(szKeyWord[7]), ( iVarID_End - 7 ) );
						iVarID = atoi( szVarID );
						
						memcpy( szVarString, &(szKeyWord[ (iVarID_End + 1 ) ]), ( iKeyWordLen - iVarID_End - 1 )  );

						mapVarString[ iVarID ] = szVarString;
						pSource.replace( iBegin, ( strlen( szKeyWord ) + 2 ), "" );
						bReplace = true;

						off = (int)iBegin;

					}	
				}
			}
		}


	// 替代變數, 找出 <$VAR1> 代換
		off = 0;
		while( 1 )
		{
			iBegin = pSource.find( "[" , off );
			if( iBegin == string::npos )
			{
				break;
			}
			else
			{
				off	= (int)iBegin + 1;
			}

			iEnd = pSource.find( "]" , off );
			if( iEnd == string::npos || iBegin >= iEnd )
			{
				break;
			}
			else
			{
				off	= (int)iEnd + 1;
			}

			// Check keyword and replace
			int iKeyWordLen = (int)(iEnd - iBegin - 1);

			ZeroMemory( szKeyWord, sizeof( szKeyWord ));
			memcpy( szKeyWord, ( pSource.c_str() + ( iBegin + 1 ) ), iKeyWordLen );

			int	iGUID = atoi( szKeyWord );

			// 檢查有沒有 $ 替代變數, <$VAR1>,<$PLAYERNAME>,<$JOBNAME>, <$SUBJOBNAME>

			if( szKeyWord[0] == '$' )
			{
				if( iKeyWordLen > 4  )
				{
					char szVarKey[5];				
					memset( szVarKey, 0, sizeof( szVarKey ) );
					memcpy( szVarKey, szKeyWord, 4 );								
				
					if( iKeyWordLen > 4 && stricmp( szVarKey, "$VAR" ) == 0 )
					{
						int		iVarID		= 0;
						int		iVarID_End	= 0;
						//char	szVarString[256];

						char szVarID[256];
						ZeroMemory( szVarID, sizeof( szVarID ) );

						memcpy( szVarID, &(szKeyWord[4]), ( iEnd - 4 ) );
						iVarID = atoi( szVarID );

						map< int, string >::iterator it = mapVarString.find( iVarID );

						if( it != mapVarString.end() )
						{
							pSource.replace( iBegin, ( strlen( szKeyWord ) + 2 ), it->second.c_str() );
							bReplace = true;
							off = (int)iBegin + (int)strlen( it->second.c_str() ) ;
						}
					}
				}
			}
		}


	// 替代系統字串
		off = 0;
		while( 1 )
		{
			iBegin = pSource.find( "[" , off );
			if( iBegin == string::npos )
			{
				break;
			}
			else
			{
				off = (int)iBegin + 1;
			}

			iEnd = pSource.find( "]" , off );
			if( iEnd == string::npos || iBegin >= iEnd )
			{
				break;
			}
			else
			{
				off = (int)iEnd + 1;
			}

			// Check keyword and replace
			int iKeyWordLen = (int)(iEnd - iBegin - 1);

			ZeroMemory( szKeyWord, sizeof( szKeyWord ));
			memcpy( szKeyWord, ( pSource.c_str() + ( iBegin + 1 ) ), iKeyWordLen );

			int	iGUID = atoi( szKeyWord );

			// 檢查有沒有 $ 替代變數, <$PLAYERNAME>,<$JOBNAME>, <$SUBJOBNAME>

			if( szKeyWord[0] == '$' )
			{
				// <$PLAYERNAME>
					if( iKeyWordLen == strlen( "$PLAYERNAME" ) && stricmp( szKeyWord, "$PLAYERNAME" ) == 0 )
					{				
						RoleDataEx*	pRole				= CNetGlobal::RoleData();

						pSource.replace( iBegin, ( strlen( szKeyWord ) + 2 ), pRole->RoleName() );
						bReplace	= true;
						off			= iBegin + strlen( pRole->RoleName() ) ;

					}
			}
 			else
			{
				// 物件代碼及字串變數

				//int		iColor_NPC	= ObjectDataClass::St_SysValue()->SysQuestColor_NPC;
				//int		iColor_Item	= ObjectDataClass::St_SysValue()->SysQuestColor_ITEM;
				//int		iColor_Zone = ObjectDataClass::St_SysValue()->SysQuestColor_ZONE;



				bool	bPlural		= false;
				char	szText[4096];
				char	szName[4096];


				// 檢查是不是替代名稱字串
				//---------------------------------------------------
				string	sReplaceText	= "";

				bool	bReplaceString	= false;
				int		iReplaceKeyLen	= 0;

				for( int i = 0; i < sizeof( szKeyWord ); i++ )
				{
					if( szKeyWord[ i ] == '|' )
					{
						if( ( i + 1 ) < sizeof( szKeyWord ) )
						{
							bReplaceString	= true;											
							sReplaceText	= &( szKeyWord[ ( i + 1 ) ] );	// 去掉 '|' [110000|XXXXX]
							szKeyWord[ i ]	= 0;
							iKeyWordLen		= iKeyWordLen - ( (int)strlen( sReplaceText.c_str() ) + 1 );
							break;
						}
					}
				}
				//---------------------------------------------------

				// 檢查是不是複數物件名稱
				//---------------------------------------------------
					if( _strnicmp( szKeyWord, "<S>", strlen( "<S>" ) )== 0 )
					{
						char* ptr = szKeyWord;

						bPlural		= true;					
						iKeyWordLen	-= 3;
						memmove( szKeyWord, ( ptr + 3 ), ( strlen( ptr ) + 1 ) );
						iGUID = atoi( szKeyWord );
					}
				//---------------------------------------------------


				if( iKeyWordLen == 6 && iGUID >= 100000 )
				{
					GameObjDbStructEx*	pObj		= g_ObjectData->GetObj( iGUID );

					if( pObj == NULL )
					{
						sprintf( szName, "NullObj-%d", iGUID ); 
					}	
					else
					{
						if( bPlural == true )
						{
							if( strlen( pObj->GetPluralName() ) == 0 )
							{
								sprintf( szName, "Sys%d_Name_Plural", iGUID );
							}
							else
							{
								sprintf( szName, "%s", pObj->GetPluralName() );
							}
						}
						else
						{
							sprintf( szName, "%s", pObj->GetName() );
						}
					}

					if( bReplaceString == true )
					{
						strcpy( szName, sReplaceText.c_str() );
					}

					if( iGUID >= Def_ObjectClass_NPC && iGUID < ( Def_ObjectClass_QuestNPC + Def_ObjectClass_QuestNPC_Total ) ) // 怪物 && 任務 NPC
					{
						if( bHyperLink == true )
						{
							sprintf( szText, "|Hnpc:%d|h%s[%s]|r|h", iGUID, m_szColor_NPC.c_str(), szName );
						}
						else
						if( bQuestColor == true )
						{
							sprintf( szText, "%s%s|r", m_szColor_NPC.c_str(), szName );
						}
						else
						{
							// 甚麼都沒有
							sprintf( szText, "%s", szName );
						}
					}
					else if( iGUID >= Def_ObjectClass_QuestDetail && 
						     iGUID < ( Def_ObjectClass_QuestDetail + 20000 ) ) //任務
					{
						int iColor = 0;
						if( CNetGlobal::RoleData()->CheckQuestHistory_IsComplete( iGUID ) )
						{
							//iColor = 0xff20ff20;
							sprintf( szText, "%s%s|r", m_szQuest_Complete.c_str(), szName );
						}
						else
						{
							//iColor = 0xffff2020;
							sprintf( szText, "%s%s|r", m_szQuest_UnComplete.c_str(), szName );
						}

						
					}
					else if( iGUID >= Def_ObjectClass_Title && 
						iGUID < ( Def_ObjectClass_Title + _MAX_TITLE_FLAG_COUNT_ ) ) //頭銜
					{
						int iColor = 0;
						if( CNetGlobal::RoleData()->PlayerBaseData->Title.GetFlag( iGUID - Def_ObjectClass_Title ) )
						{
							//iColor = 0xff20ff20;
							sprintf( szText, "%s%s|r", m_szQuest_Title.c_str(), szName );
						}
						else
						{
							//iColor = 0xffff2020;
							sprintf( szText, "%s%s|r", m_szQuest_NoTitle.c_str(), szName );
						}					
					}
					else	// 其它的都當物品
					{
						if( bHyperLink == true )
						{
							sprintf( szText, "|Hitem:%x|h%s[%s]|r|h", iGUID, m_szColor_Item.c_str(), szName );
						}
						else
						if( bQuestColor == true )
						{
							sprintf( szText, "%s%s|r", m_szColor_Item.c_str(), szName );
						}
						else
						{
							// 甚麼都沒有
							sprintf( szText, "%s", szName );
						}
					}

					if( bPlural == true )
					{
						int iKeyLen = (int)( strlen( szKeyWord ) + 5 );

						if( bReplaceString == true )
						{
							iKeyLen += (int)( strlen( sReplaceText.c_str() ) + 1 );
						}
						pSource.replace( iBegin, iKeyLen, szText );		// <S>110000
					}
					else
					{
						int iKeyLen = (int)( strlen( szKeyWord ) + 2 );

						if( bReplaceString == true )
						{
							iKeyLen += (int)( strlen( sReplaceText.c_str() ) + 1 );
						}
						pSource.replace( iBegin, iKeyLen, szText );		// 110000
					}
					
					bReplace	= true;
					off			= (int)iBegin + (int)strlen( szText ) ;
				}
				else
				{
					
					const char* pString = g_ObjectData->GetString( szKeyWord );
					
					if( pString != NULL )
					{
						char szString[4096];

						if( bReplaceString == true )
						{
							strcpy( szString, sReplaceText.c_str() );
							pString = szString;
						}

						if( _strnicmp( szKeyWord, "ZONE_", strlen( "ZONE_" ) )== 0 )
						{
							if( bHyperLink == true || bQuestColor == true )
							{
								sprintf( szText, "%s%s|r", m_szColor_Zone.c_str(), pString );
							}
							else
							{
								sprintf( szText, "%s", pString );
							}
						}
						else
						{
							sprintf( szText, "%s", pString );
						}

						int iKeyLen = (int)( strlen( szKeyWord ) + 2 );

						if( bReplaceString == true )
						{
							iKeyLen += (int)( strlen( sReplaceText.c_str() ) + 1 );
						}

						pSource.replace( iBegin, iKeyLen, szText );
						bReplace	= true;
						off			= (int)iBegin + (int)strlen( szText ) ;
					}
				}
			}
		}

		if( bReplace == false || bNoloopCheckVar == true )
		{
			break;
		}
		else
		{

		}
	}



/*
	// 系統字串
		off = 0;
		while( 1 )
		{
			iBegin = pSource.find( "[" , off );
			if( iBegin == string::npos )
			{
				break;
			}
			iEnd = pSource.find( "]" , off );
			if( iEnd == string::npos || iBegin >= iEnd )
			{
				break;
			}

			// Check keyword and replace
			int iKeyWordLen = iEnd - iBegin - 1;

			ZeroMemory( szKeyWord, sizeof( szKeyWord ));
			memcpy( szKeyWord, ( pSource.c_str() + ( iBegin + 1 ) ), iKeyWordLen );

			int	iGUID = atoi( szKeyWord );

			// 檢查有沒有 $ 替代變數, <$VAR1>,<$PLAYERNAME>,<$JOBNAME>, <$SUBJOBNAME>
			
			if( szKeyWord[0] == '$' )
			{
				// <$PLAYERNAME>

				// <$JOBNAME>

				// <$SUBJOBNAME>

				// <$VARN>

				off = iEnd + 1;

			}
			else
			{
				// 物件代碼及字串變數
				if( iKeyWordLen == 6 && iGUID >= 100000 )
				{
					GameObjDbStruct*	pObj		= g_ObjectData->GetObj( iGUID );

					if( pObj == NULL )
						return pSource;

					pSource.replace( iBegin, ( strlen( szKeyWord ) + 2 ), pObj->Name );

					off = iBegin + strlen( pObj->Name ) ;
				}
				else
				{
						sprintf( szString, "%s", g_ObjectData->GetString( szKeyWord ) );

						if( strlen( szString ) != 0 )
						{
							pSource.replace( iBegin, ( strlen( szKeyWord ) + 2 ), szString );
							off = iBegin + strlen( szString ) ;
						}
						else
							break;
				}
			}
		}
*/

	for( map< string, string >::iterator it = g_pQuestDetail->m_mapColorKeyWord.begin(); it != g_pQuestDetail->m_mapColorKeyWord.end(); it++ )
	{
		string	sKey		= it->first;
		string	sReplace	= it->second;


		size_t	iBegin;
		while ( (iBegin = pSource.find( sKey )) != string::npos ) 
		{
			pSource.replace( iBegin, sKey.size(), sReplace );
		}
	}
	
	m_lastResult = pSource;

	return pSource;
}
//---------------------------------------------------------------
void CNetCli_Script_Child::OnAttachCastMotionToolID	( int iObjID, int iToolID )
{
	CRoleSprite* pPlayer		= g_pGameMain->FindSpriteByID( iObjID );

	if( g_ObjectData->GetObj( iToolID ) != NULL )
	{
		m_iScriptCastAttachToolID	= iToolID;

		if( pPlayer != NULL )
			pPlayer->SetHandsObjectId( eRoleHands_Crafting, m_iScriptCastAttachToolID	);	

	}
}
//---------------------------------------------------------------
void CNetCli_Script_Child::OnAttachTool( int iObjID, int iToolID, const char* pszLink )
{
	CRoleSprite* pPlayer		= g_pGameMain->FindSpriteByID( iObjID );

	if( g_ObjectData->GetObj( iToolID ) != NULL )
	{
		m_iScriptCastAttachToolID	= iToolID;

		if( pPlayer != NULL )
		{
			pPlayer->SetHandsObjectId( eRoleHands_Crafting, m_iScriptCastAttachToolID	);	
			pPlayer->AttachCraftingTool( pszLink );	
		}
	}
	else
	{
		if( pPlayer != NULL )
		{
			pPlayer->SetHandsObjectId( eRoleHands_Crafting, 0	);	
			pPlayer->AttachCraftingTool( pszLink );
		}
	}
}
//---------------------------------------------------------------
void CNetCli_Script_Child::OnStartClientCountDown ( int iStartNumber, int iEndNumber, int iOffset, int iMode, int iShowStatusBar, const char* str )
{
	// StartNumber 表示開使數值, iOffset 表示每秒加的數值, 倒數的話, Ex ( iStartNumber = 10 , iOffset = -1 ), 正數的話, ( iStartNumber = 0 , iOffset = 1 )

	lua_State* L = g_pInterface->GetLuaState();
	if ( L )
	{
		switch (iShowStatusBar)
		{
		case 0:
			{
				lua_pushnumber(L, iStartNumber);
				lua_setglobal(L, "arg1");
				lua_pushnumber(L, iMode);					// 0 沒有顯示最後文字 ,1 Fight, 2 Go
				lua_setglobal(L, "arg2");
				g_pInterface->SendWorldEvent("COUNTDOWN_START");
				break;
			}
		case -1:
			{
				g_pClientCountDownBarFrame->CountDownStop();
				break;
			}
		default:
			{
				g_pClientCountDownBarFrame->CountDownStart(str, (float)iStartNumber, (float)iEndNumber , iShowStatusBar);
			}
		}
	}
}
//---------------------------------------------------------------
void CNetCli_Script_Child::OnSetCameraFollowUnit( int iObjID, int iOffX, int iOffY, int iOffZ )
{

}
//---------------------------------------------------------------
void CNetCli_Script_Child::OnStopSound( int iObjID, int iSoundID )
{
	g_pRuGlobalEventManager->StopSoundByIndex(iSoundID);
}

int	CNetCli_Script_Child::GetQuestModePriority( int iMode )
{
	QuestDisplay emMode = (QuestDisplay)iMode;

	if( emMode == eQuest_Display_AvailableQuest )
		return 0;

	if( emMode == eQuest_Display_AvailableRepeatQuest )
		return 1;
	
	if( emMode == eQuest_Display_Public_AvailableQuest )
		return 2;

	return 0;
}