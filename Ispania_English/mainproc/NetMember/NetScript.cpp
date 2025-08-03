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
		// ���Ȫ���

	
	}
	else
	{
		// ����NPC
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

				// ���m�������A
					g_pUi_QuestNPCList->ReSet();		

				// �]�m���� NPC
					g_pUi_QuestNPCList->Set_InteractNPC( pRoleObj );

				// �]�m NPC Name
					g_pUi_QuestNPCList->Set_NpcName( pRoleObj->GetName() );
					//g_pUi_QuestNPCList->Set_NpcName( pObj->Name );

				// �[�J NPC �|�����o��
					//g_pUi_QuestNPCList->Add_NpcText( pObj->QuestCollect.szQuestNpcText );	// �]�m NPC �|���ƻ�
					g_pUi_QuestNPCList->Add_NpcText( pObj->NPC.szQuestNpcText );	// �]�m NPC �|���ƻ�

				// �ˬd�� NPC �O�_�O���w�������� NPC
				// �ˬd Client ���W�� Quest ���L�t���w NPC ����
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
								// �ˬd�γ]�m NPC ���W Quet ���A
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

				// �[�J NPC �൹�� QUEST
				//------------------------------------------------------------------------------
					for( int Index = 0; Index < 20; Index++ )
					{
						int iQuestID = pObj->NPC.iQuestID[ Index ];

						// �ˬd����O�_�w��������
							if( pRole->CheckQuestHistory_IsComplete( iQuestID ) == true )
								continue;	// ���L�o�� Quest
						
						if( iQuestID != 0 )
						{
							GameObjDbStruct			*pObj	= g_ObjectData->GetObj( iQuestID );
							
							if( pObj )
							{
								GameObjectQuestDetail	*pQuest = &( pObj->QuestDetail );

								// �ˬd����O�_�i�H���ӥ���
								if( pRole->CheckQuest_IsAvailable( pQuest ) != EM_QUESTCHECK_RESULT_OK )
									continue;


								int iType = 0;

								// �ˬd����O�_�֦��� Quest, �άO�_����
									int iQuestField = pRole->CheckQuestList_IsGotQuest( iQuestID );

									if( iQuestField != -1 )
									{
										// �ˬd�ӥ��ȬO�_�����w�t�@NPC����.
										// �Y�O�h�� NPC �Y����ܸӥ���
										if( pQuest->iRequest_QuestID != 0 && pQuest->iRequest_QuestID != iQuestNPCID )
											continue;
										
										// �ˬd�O�_�w������ Quest
										if( pRole->CheckQuest_IsComplete( iQuestField, iQuestID, pQuest ) == EM_QUESTCHECK_RESULT_OK )
										{
											iType = 2;	// �w����
										}
										else
										{
											iType = 1; // �|������
										}
									}
									else
									{
										iType = 0;	// �|�����L
									}

								switch( iType ) 
								{
								case 0: // �|�����L
									g_pUi_QuestNPCList->Add_Available_Quest( iQuestID , pObj->Name );
									break;
								case 1: // �|������
									g_pUi_QuestNPCList->Add_Current_Quest( iQuestID, pObj->Name );
									break;
								case 2: // �w����
									g_pUi_QuestNPCList->Add_Complete_Quest( iQuestID, pObj->Name );
									break;
								}
							}

						}
						else
							break;
					}

				// �q�X����
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
		// �q�X Quest ���㪺�T��

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

		// ���X Quest ���, ���ˬd�O�_�����w���˥ؼ�
		//-----------------------------------------------------------------
		GameObjectQuestDetail *pQuest = &( g_ObjectData->GetObj( iQuestID )->QuestDetail );
		pRole->CheckQuest_RequestKill_SetTempData	( iField, pQuest );
		pRole->CheckQuest_ClickToGetItem_SetTempData( iField, pQuest );

		g_pUi_QuestList->ReSet();
	
		if( iQuestField != iField )
		{
			g_pError->AddMessage( 0, 0, "OnQuestAcceptResult() �����Ȧ�. �i�O���PSrv���P( %d )", iQuestID );		
		}
		else
			g_pError->AddMessage( 2, 0, "OnQuestAcceptResult() �����Ȧ�( %d )", iQuestID );		
	}
	else
	{
		g_pError->AddMessage( 0, 0, "OnQuestAcceptResult() �����ȥ���( %d )", emResult );
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
		g_pError->AddMessage( 2, 0, "OnQuestCompleteResult() ���ȧ���( %d )", iQuestID );
		
	}
	else
	{
		g_pError->AddMessage( 0, 0, "OnQuestCompleteResult() ���ȧ�������( %d )", emResult );
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
		g_pError->AddMessage( 2, 0, "OnQuestDeleteResult() ���ȧR��( %d )", iQuestID );				
	}
	else
	{
		g_pError->AddMessage( 0, 0, "OnQuestDeleteResult() ���ȧR������( %d )", emResult );
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
	// �줬�ʤ����h�d��, �O�_���P�� Role �i�椬��
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

	// �ˬd���L�s�W�� Quest NPC �� �d��@��Ĳ�o
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

	// �ˬd Quest NPC ���A
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

			// �ˬd Client ���W�� Quest ���L�t���w NPC ����
			//-------------------------------------------------------------	

				for( int index = 0; index < _MAX_QUEST_COUNT_; index++ )
				{
					int iQuestID = pRole->BaseData.Quest.iQuest[ index ];
					if( iQuestID != 0 )
					{
						GameObjectQuestDetail *pQuest = &( g_ObjectData->GetObj( iQuestID )->QuestDetail );

						if( pQuest->iRequest_QuestID == iQuestNpcID )
						{
							// �ˬd�γ]�m NPC ���W Quet ���A
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

			// �ˬd NPC ���W Quest ���A�F
			//-------------------------------------------------------------	
				if( QuestMode != eQuest_Display_Finish )
				{
					for( int index = 0; index < _MAX_QUEST_COUNT_; index++ )
					{
						int iQuestID = pQuestList[ index ];

						// �ˬd����O�_�w�����ӥ���

						if( pRole->CheckQuestHistory_IsComplete( iQuestID ) == true )
							continue;	// ���L�o�� Quest

						if( iQuestID != 0 )
						{	
							GameObjDbStruct			*pObj	= g_ObjectData->GetObj( iQuestID );
							
							if( pObj )
							{
								GameObjectQuestDetail	*pQuest = &( pObj->QuestDetail );
								// 3 ����?		���w����������
								//-------------------------------------------------------------
									// �ˬd Client ���W���L�Ӷ�����
									int iQuestIndex = pRole->CheckQuestList_IsGotQuest( iQuestID );

									if( iQuestIndex != -1 )
									{
										// �ˬd�Ӷ����ȬO�_�w�g�����F

										// �ˬd�O�_���w�t�@�� NPC ��������
											if( pQuest->iRequest_QuestID != 0 )
											{
												continue;	// ��� Clinet �w���� Quest, �� Quest �������w�t���w NPC ����
											}

										if( pRole->CheckQuest_IsComplete( iQuestIndex, iQuestID, pQuest ) == EM_QUESTCHECK_RESULT_OK )
										{
											QuestMode = eQuest_Display_Finish;
											break;
										}
										else
										{								
											if( QuestMode < eQuest_Display_NotFinish )
												QuestMode = eQuest_Display_NotFinish;	// 1 �Ǧ�?		���ȩ|����
										}

										continue;
									}

								// 2 ����!		�����ȥi��
								//-------------------------------------------------------------
									// �ˬd Client ������M Quest �O�_�۲�, �O����, �q�X�i�H������
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
			
			// �̷ӳ̫��ˬd���G QuestMode �M�w NPC ��ܪ� QuestFlag
			//----------------------------------------------------------
				pRoleSprite->SetQuestDisplay( QuestMode );

			it++;
		}

	// �ˬd�d��Ĳ�o NPC ���A
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

					// �ˬd�۹�Z��
					float fRange = ( pRoleSprite->GetPos() - pMainRole->GetPos()).Magnitude();

					// �ˬd���L�i�J�s���ƥ�Ĳ�o�I
						if( fRange < pRoleSprite->GetAreaRange() )
						{
							// �ˬd�O�_�w�gĲ�o�L
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

					// �ˬd���L�����ƥ�d��
						if( fRange > ( pRoleSprite->GetAreaRange() + _MAX_ROLE_TOUCH_RANGE_ ) )
						{
							// �ˬd�O�_�w�gĲ�o�L
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
			// �� Kill Quest ����, �ˬd���ȱ���O���O�u�����w���˥ؼ�, �O���ܥi�H�q�X�T���q�����ȧ���


		}
	}
	*/
}
//--------------------------------------------------------------------------------------
void CNetScript::OnTouchQuestObj			( CRoleSprite* pObj )
{
	// �ˬd�O�_���󧡥��T
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
						

						// �ˬd���W�λȦ�Y�S�w�������~�ƶq�O�_�w�F��n�D���ƶq

							int	iValue = 0; // ���W�λȦ�Ҿ֦����ƶq

							for( int iRequestIndex = 0; iRequestIndex < _MAX_QUEST_CLICKIND_COUNT_; iRequestIndex++ )
							{
								if( pQuest->iRequest_ItemID[ iRequestIndex ] == iClickObj )
								{
									if( pQuest->iRequest_ItemVal[ iRequestIndex ] <= iValue )
									{
										return;	// �w�F��n�D���ƶq, ���A����
									}
									else
										break;
								}
							}

						// ���} CastingTimeBar, �ó]�w�n Event. ���ݮɶ����� or �Q���_
							m_pTouchObj_ID			= iObjID;
							m_pTouchObj_QuestField	= pQuestTempData->QuestCollectIndexTable[ index ];
							int iQuestID			= pRole->BaseData.Quest.iQuest[ m_pTouchObj_QuestField ];

							const char* pszName		= pObj->GetName();

							g_pUi_CastingTimeBar->SetCB_Interrupted	( OnEvent_CastInterrupted	);
							g_pUi_CastingTimeBar->SetCB_Finished	( OnEvent_CastFinished		);

							g_pUi_CastingTimeBar->Start_Mode_Normal	( pObj->GetName(), _TIME_QUEST_CLICKTOGETITEM );

						// �o�e�ʥ]�� Server �w�p�S�w�ɶ���e�^�ƥ�						
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