#include "NetScript.h"
#include "../interfacemember/Quest/UI_QuestList.h"
#include "../interfacemember/Quest/UI_QuestDialog.h"
#include "../interfacemember/Quest/UI_QuestNPCList.h"
#include "../interfacemember/UI_CastingTimeBar.h"

#include "../interfacemember/WorldFrame.h"
#include "../../MainProc/ObjectDataClass.h"

using namespace ui;

CRoleSprite*	CNetScript::m_pLastTouchObj = NULL;

vector< CRoleSprite* >					CNetScript::m_vecUnProcessRole;
vector< CRoleSprite* >					CNetScript::m_vecQuestRole;
map< CRoleSprite* , ScriptEventInfo* >	CNetScript::m_mapRangePlot;
// --------------------------------------------------------------------------------------
bool CNetScript::Init()
{
	if( m_pThis == NULL )
		m_pThis = new CNetScript;

	CNetCli_Script::Init();
	CNetGlobal::Schedular()->Push( OnTimeUpdate , 500 , NULL , NULL );

	return true;
}
// --------------------------------------------------------------------------------------
bool CNetScript::Release()
{
	CNetCli_Script::Release();

	SAFE_DELETE( m_pThis );

	return true;
}
//--------------------------------------------------------------------------------------
void CNetScript::OnTouchQuestNPC ( CRoleSprite* pRoleObj )
{
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
						int iQuestID = pRole->BaseData.Quest.iQuest[ index ];
						if( iQuestID != 0 )
						{
							GameObjDbStruct			*pObj	= g_ObjectData->GetObj( iQuestID );
							GameObjectQuestDetail	*pQuest = &( pObj->QuestDetail );

							if( pQuest->iRequest_QuestID == iQuestNPCID )
							{
								// 檢查及設置 NPC 身上 Quet 狀態
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

}
//--------------------------------------------------------------------------------------
void CNetScript::OnQuestDetail		( int iQuestID , int iMode )
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
void CNetScript::OnUpdate			()
{


}
//--------------------------------------------------------------------------------------
void CNetScript::OnQuestAcceptResult ( int iQuestField, int iQuestID, EM_QUESTCHECK_RESULT emResult )
{
	if( emResult == EM_QUESTCHECK_RESULT_OK )
	{
		RoleDataEx* pRole	= g_pGameMain->RoleData();	
		int			iField	= pRole->Quest_Add( iQuestID );

		// 取出 Quest 資料, 並檢查是否有指定打倒目標
		//-----------------------------------------------------------------
		GameObjectQuestDetail *pQuest = &( g_ObjectData->GetObj( iQuestID )->QuestDetail );
		pRole->CheckQuest_RequestKill_SetTempData	( iField, pQuest );
		pRole->CheckQuest_ClickToGetItem_SetTempData( iField, pQuest );

		g_pUi_QuestList->ReSet();
	
		if( iQuestField != iField )
		{
			g_pError->AddMessage( 0, 0, "OnQuestAcceptResult() 接任務成. 可是欄位與Srv不同( %d )", iQuestID );		
		}
		else
			g_pError->AddMessage( 2, 0, "OnQuestAcceptResult() 接任務成( %d )", iQuestID );		
	}
	else
	{
		g_pError->AddMessage( 0, 0, "OnQuestAcceptResult() 接任務失敗( %d )", emResult );
	}
}
//--------------------------------------------------------------------------------------
void CNetScript::OnQuestCompleteResult	( int iQuestField, int iQuestID, EM_QUESTCHECK_RESULT emResult )
{
	if( emResult == EM_QUESTCHECK_RESULT_OK )
	{
		RoleDataEx*				pRole	= g_pGameMain->RoleData();	
		
		pRole->Quest_Complete( iQuestField, iQuestID );

		g_pUi_QuestList->ReSet();
		g_pError->AddMessage( 2, 0, "OnQuestCompleteResult() 任務完成( %d )", iQuestID );
		
	}
	else
	{
		g_pError->AddMessage( 0, 0, "OnQuestCompleteResult() 任務完成失敗( %d )", emResult );
	}
}
//--------------------------------------------------------------------------------------
void CNetScript::OnQuestDeleteResult		( int iQuestField, int iQuestID, EM_QUESTCHECK_RESULT emResult )
{
	if( emResult == EM_QUESTCHECK_RESULT_OK )
	{
		RoleDataEx*				pRole	= g_pGameMain->RoleData();	

		//pRole->Quest_Complete( iQuestField, iQuestID );
		pRole->Quest_Delete( iQuestField, iQuestID );

		g_pUi_QuestList->ReSet();
		g_pError->AddMessage( 2, 0, "OnQuestDeleteResult() 任務刪除( %d )", iQuestID );				
	}
	else
	{
		g_pError->AddMessage( 0, 0, "OnQuestDeleteResult() 任務刪除失敗( %d )", emResult );
	}
}
//--------------------------------------------------------------------------------------
void CNetScript::OnAdd_Obj ( CRoleSprite* pObj )
{
	m_vecUnProcessRole.push_back( pObj );
}
//--------------------------------------------------------------------------------------
void CNetScript::OnDel_Obj ( CRoleSprite* pObj )
{
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

	{

		vector< CRoleSprite* >::iterator it;

		it = m_vecUnProcessRole.begin();
		while( it != m_vecUnProcessRole.end() )
		{
			if( *it == pObj )
			{
				m_vecUnProcessRole.erase( it );
				return;
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
				return;
			}
			else
				it++;

		}

		{
			map< CRoleSprite* , ScriptEventInfo* >::iterator it = m_mapRangePlot.find( pObj );
			if( it != m_mapRangePlot.end() )
			{
				ScriptEventInfo* pEvent = it->second;
				delete pEvent;

				m_mapRangePlot.erase( it );
			}
		}
	}	
}
//--------------------------------------------------------------------------------------
int CNetScript::OnTimeUpdate			( SchedularInfo* Obj , void* InputClass )
{
	CNetGlobal::Schedular()->Push( OnTimeUpdate , 500 , NULL , NULL );

	vector< CRoleSprite* >::iterator it = m_vecUnProcessRole.begin();

	CRoleSprite	*pRoleSprite = NULL;

	// 檢查有無新增的 Quest NPC 或 範圍劇情觸發
	//----------------------------------------------------------
		while( it != m_vecUnProcessRole.end() )
		{
			pRoleSprite = *it;

			//int iObjID				= pRoleSprite->GetObjectId();
			//int iQuestNpcBeginID	= Def_ObjectClass_QuestNPC;
			//int iQuestNpcEndID		= Def_ObjectClass_QuestNPC + Def_ObjectClass_QuestNPC_Total;

			if( pRoleSprite->GetCheckedStatus() == eChecked_QuestNPC )
			{
				m_vecQuestRole.push_back( pRoleSprite );
				pRoleSprite->SetQuestInfo( pRoleSprite->GetQuestId(), pRoleSprite->GetTouchRange(), 70 );
			}

			if( pRoleSprite->GetAreaRange() != 0 )
			{
				ScriptEventInfo* pEvent = new ScriptEventInfo;
				m_mapRangePlot[ pRoleSprite ] = pEvent;
			}

			/*
			if( iObjID >= iQuestNpcBeginID && iObjID < iQuestNpcEndID )
			{
			}
			*/

			it = m_vecUnProcessRole.erase( it );
		}

	// 檢查 Quest NPC 狀態
	//----------------------------------------------------------
		RoleDataEx*		pRole		= g_pGameMain->RoleData();

		it = m_vecQuestRole.begin();
		while( it != m_vecQuestRole.end() )
		{
			pRoleSprite		= *it;
			int iQuestNpcID = pRoleSprite->GetObjectId();
			//int iQuestCollectID						= pRoleSprite->GetObjectId();
			//GameObjectQuestCollect	*pQuestCollect	= &( g_ObjectData->GetObj( iQuestCollectID )->QuestCollect );
			int* pQuestList		= g_ObjectData->GetObj( iQuestNpcID )->NPC.iQuestID;

			
			QuestDisplay	QuestMode	= eQuest_Display_Null;

			// 檢查 Client 身上的 Quest 有無另指定 NPC 完成
			//-------------------------------------------------------------	

				for( int index = 0; index < _MAX_QUEST_COUNT_; index++ )
				{
					int iQuestID = pRole->BaseData.Quest.iQuest[ index ];
					if( iQuestID != 0 )
					{
						GameObjectQuestDetail *pQuest = &( g_ObjectData->GetObj( iQuestID )->QuestDetail );

						if( pQuest->iRequest_QuestID == iQuestNpcID )
						{
							// 檢查及設置 NPC 身上 Quet 狀態
								if( pRole->CheckQuest_IsComplete( index, iQuestID, pQuest ) == EM_QUESTCHECK_RESULT_OK )
								{
									QuestMode = eQuest_Display_Finish;
									break;
								}
								else
									QuestMode = eQuest_Display_NotFinish;
						}
					}
					else
						break;
				}

			// 檢查 NPC 身上 Quest 狀態了
			//-------------------------------------------------------------	
				if( QuestMode != eQuest_Display_Finish )
				{
					for( int index = 0; index < _MAX_QUEST_COUNT_; index++ )
					{
						int iQuestID = pQuestList[ index ];

						// 檢查角色是否已完成該任務

						if( pRole->CheckQuestHistory_IsComplete( iQuestID ) == true )
							continue;	// 跳過這個 Quest

						if( iQuestID != 0 )
						{	
							GameObjDbStruct			*pObj	= g_ObjectData->GetObj( iQuestID );
							
							if( pObj )
							{
								GameObjectQuestDetail	*pQuest = &( pObj->QuestDetail );
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
											QuestMode = eQuest_Display_Finish;
											break;
										}
										else
										{								
											if( QuestMode < eQuest_Display_NotFinish )
												QuestMode = eQuest_Display_NotFinish;	// 1 灰色?		任務尚未成
										}

										continue;
									}

								// 2 紅色!		有任務可接
								//-------------------------------------------------------------
									// 檢查 Client 的條件和 Quest 是否相符, 是的話, 秀出可以接任務
									if( pRole->CheckQuest_IsAvailable( pQuest ) == EM_QUESTCHECK_RESULT_OK )
									{
										if( QuestMode < qQuest_Display_Take )
											QuestMode = qQuest_Display_Take;
									}
							}
						}
						else
							break;
					}
				}
			
			// 依照最後檢查結果 QuestMode 決定 NPC 顯示的 QuestFlag
			//----------------------------------------------------------
				pRoleSprite->SetQuestDisplay( QuestMode );

			it++;
		}

	// 檢查範圍觸發 NPC 狀態
	//-----------------------------------------------------------------------------------------------
		{
			map< CRoleSprite* , ScriptEventInfo* >::iterator it;
			CRoleSprite*	pMainRole		= g_pWorldFrame->GetPlayer();

			if( pMainRole )
			{
				it = m_mapRangePlot.begin();
				while( it != m_mapRangePlot.end() )
				{

					ScriptEventInfo* pEvent	= it->second;
					pRoleSprite				= it->first;

					// 檢查相對距離
					float fRange = ( pRoleSprite->GetPos() - pMainRole->GetPos()).Magnitude();

					// 檢查有無進入新的事件觸發點
						if( fRange < pRoleSprite->GetAreaRange() )
						{
							// 檢查是否已經觸發過
								if( pEvent->bRangeEvent == false )
								{
									if( pEvent->dwCoolDown == 0 || GetTickCount() > pEvent->dwCoolDown )
									{
										pEvent->bRangeEvent = true;
										pEvent->dwCoolDown	= 0;

										OnEvent_RangePlot( pRoleSprite );
									}
								}
						}

					// 檢查有無脫離事件範圍
						if( fRange > ( pRoleSprite->GetAreaRange() + _MAX_ROLE_TOUCH_RANGE_ ) )
						{
							// 檢查是否已經觸發過
								if( pEvent->bRangeEvent == true )
								{
									pEvent->bRangeEvent = false;
									pEvent->dwCoolDown	= GetTickCount() + 10000;
								}
						}
						
					it++;
				}
			}

		}
	return 0;
}
//--------------------------------------------------------------------------------------
void CNetScript::AcceptQuest ( int iQuestID )
{
	CNetCli_Script::AcceptQuest( m_pLastTouchObj->GetIdentify(), iQuestID );
}
//--------------------------------------------------------------------------------------
void CNetScript::CompleteQuest ( int iQuestID )
{
	CNetCli_Script::CompleteQuest( m_pLastTouchObj->GetIdentify(), iQuestID ); 
}
//--------------------------------------------------------------------------------------
void CNetScript::OnKillObj ( CRoleSprite* pDeadObj )
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
}
//--------------------------------------------------------------------------------------
void CNetScript::OnTouchQuestObj			( CRoleSprite* pObj )
{
	// 檢查是否條件均正確
	int						iObjID			= pObj->GetObjectId();
	RoleDataEx*				pRole			= g_pGameMain->RoleData();
	QuestTempDataStruct*	pQuestTempData	= &( pRole->TempData.QuestTempData );

	for( int iField = 0; iField < pQuestTempData->QuestCollectCount; iField++ )
	{
		for( int index = 0; index < _MAX_QUEST_CLICKIND_COUNT_; index++ )
		{
			int iClickObj			= pQuestTempData->QuestCollect[ iField ].iClickObj[ index ];
	
			if( iClickObj != 0 )
			{
				if( iClickObj == iObjID )
				{
					int						iQuestID	= pQuestTempData->QuestCollectIndexTable[ index ];
					GameObjDbStruct			*pObjDB		= g_ObjectData->GetObj( iQuestID );
					GameObjectQuestDetail	*pQuest		= &( pObjDB->QuestDetail );

					if( pQuest )
					{                        
						

						// 檢查身上及銀行某特定任物物品數量是否已達到要求的數量

							int	iValue = 0; // 身上及銀行所擁有的數量

							for( int iRequestIndex = 0; iRequestIndex < _MAX_QUEST_CLICKIND_COUNT_; iRequestIndex++ )
							{
								if( pQuest->iRequest_ItemID[ iRequestIndex ] == iClickObj )
								{
									if( pQuest->iRequest_ItemVal[ iRequestIndex ] <= iValue )
									{
										return;	// 已達到要求的數量, 不再給予
									}
									else
										break;
								}
							}

						// 打開 CastingTimeBar, 並設定好 Event. 等待時間完畢 or 被中斷
							m_pTouchObj_ID			= iObjID;
							m_pTouchObj_QuestField	= pQuestTempData->QuestCollectIndexTable[ index ];
							int iQuestID			= pRole->BaseData.Quest.iQuest[ m_pTouchObj_QuestField ];

							const char* pszName		= pObj->GetName();

							g_pUi_CastingTimeBar->SetCB_Interrupted	( OnEvent_CastInterrupted	);
							g_pUi_CastingTimeBar->SetCB_Finished	( OnEvent_CastFinished		);

							g_pUi_CastingTimeBar->Start_Mode_Normal	( pObj->GetName(), _TIME_QUEST_CLICKTOGETITEM );

						// 發送封包給 Server 預計特定時間後送回事件						
							ClickQuestObj( m_pTouchObj_QuestField, iQuestID, iClickObj );
					}
				}
			}
			else
				break;
		}
	}
}
//--------------------------------------------------------------------------------------
void CNetScript::OnEvent_CastInterrupted ()
{
	int a = 3;

}
//--------------------------------------------------------------------------------------
void CNetScript::OnEvent_CastFinished ()
{
	int a = 3;

}
//--------------------------------------------------------------------------------------
void CNetScript::OnQuestFlag ( int iQuestID,	int iValue )
{
	RoleDataEx*				pRole			= g_pGameMain->RoleData();

	if( pRole )
	{
		if( iQuestID == -1 )
		{
			pRole->BaseData.Quest.QuestHistory.ReSet();
		}
		else
		{
			if( iValue )
				pRole->BaseData.Quest.QuestHistory.SetFlagOn ( iValue );
			else
				pRole->BaseData.Quest.QuestHistory.SetFlagOff ( iValue );
		}
	}
}
//--------------------------------------------------------------------------------------