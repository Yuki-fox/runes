#include "../../mainproc/GameMain.h"
#include "../../netwaker_member/NetWakerClientInc.h"

#include "UI_QuestDetail.h"
#include "UI_QuestBook.h"

#include "../ItemLink/ItemLink.h"
#include "../WorldFrames/NpcTrackFrame.h"

CUI_QuestBook*		g_pQuestBook	= NULL;
//-------------------------------------------------------------------
int			LuaFunc_GetNumQuestBookButton_QuestBook		( lua_State *L );
int			LuaFunc_ViewQuest_QuestBook					( lua_State *L );
int			LuaFunc_GetQuestInfo						( lua_State *L );
int			LuaFunc_GetQuestCatalogInfo					( lua_State *L );

int			LuaFunc_GetVocLV							( lua_State *L );
int			LuaFunc_CheckQuest							( lua_State *L );
int			LuaFunc_CheckFlag							( lua_State *L );
int			LuaFunc_GetQuestRequest						( lua_State *L );
int			LuaFunc_GetQuestTrack						( lua_State *L );
int			LuaFunc_SetQuestTrack						( lua_State *L );
int			LuaFunc_GetCurrentVocLV						( lua_State *L );

int			LuaFunc_DeleteQuestByID						( lua_State *L );

int			LuaFunc_QuestBook_GetQuestHyperLink         ( lua_State *L );

int			LuaFunc_QuestDebug							( lua_State *L );
int			LuaFunc_QuestNpcDebug						( lua_State *L );
/*
int			LuaFunc_OnLoad_QuestList		( lua_State *L );
int			LuaFunc_OnClick_QuestListButton	( lua_State *L );
int			LuaFunc_GetQuestListText		( lua_State *L );
int			LuaFunc_GetNumQuest				( lua_State *L );	
int			LuaFunc_GetQuestNameByIndex		( lua_State *L );	// 傳入 _Type, _Index 
*/
//-------------------------------------------------------------------
CUI_QuestBook::CUI_QuestBook( CInterface* object ) : CInterfaceBase( object )
{
	g_pQuestBook = this;
}
// --------------------------------------------------------------------------------------
CUI_QuestBook::~CUI_QuestBook(void)
{
	g_pQuestBook = NULL;
}
//--------------------------------------------------------------------------------------
void CUI_QuestBook::RegisterFunc(void)
{
	lua_State* l = GetLuaStateWorld();
	if (l)
	{
		lua_register( l, "GetNumQuestBookButton_QuestBook",			LuaFunc_GetNumQuestBookButton_QuestBook		);
		lua_register( l, "GetQuestInfo",							LuaFunc_GetQuestInfo						);
		lua_register( l, "ViewQuest_QuestBook",						LuaFunc_ViewQuest_QuestBook					);
		lua_register( l, "GetQuestCatalogInfo",						LuaFunc_GetQuestCatalogInfo					);


		/*
		lua_register( l, "OnLoad_QuestList",			LuaFunc_OnLoad_QuestList		);
		lua_register( l, "OnClick_QuestListButton",		LuaFunc_OnClick_QuestListButton	);
		lua_register( l, "GetQuestListText",			LuaFunc_GetQuestListText		);
		lua_register( l, "GetNumQuest",					LuaFunc_GetNumQuest				);
		lua_register( l, "GetQuestNameByIndex",			LuaFunc_GetQuestNameByIndex		);
		*/

		lua_register( l, "GetVocLV",								LuaFunc_GetVocLV							);
		lua_register( l, "GetCurrentVocLV",							LuaFunc_GetCurrentVocLV						);
		lua_register( l, "CheckQuest",								LuaFunc_CheckQuest							);
		lua_register( l, "CheckFlag",								LuaFunc_CheckFlag							);

		lua_register( l, "GetQuestTrack",							LuaFunc_GetQuestTrack						);
		lua_register( l, "SetQuestTrack",							LuaFunc_SetQuestTrack						);

		lua_register( l, "DeleteQuestByID",							LuaFunc_DeleteQuestByID						);

		lua_register( l, "QuestBook_GetQuestHyperLink",				LuaFunc_QuestBook_GetQuestHyperLink			);

		lua_register( l, "QuestDebug",								LuaFunc_QuestDebug							);
		lua_register( l, "QuestNpcDebug",							LuaFunc_QuestNpcDebug						);

	}
}
//--------------------------------------------------------------------------------------
int	CUI_QuestBook::GetQuestGUID			( int iIndex )
{
	if( iIndex >= 0 && iIndex < g_pQuestBook->m_vecQuestID.size() )
	{
		return g_pQuestBook->m_vecQuestID[ iIndex ];
	}

	return -1;
}

 //--------------------------------------------------------------------------------------
void CUI_QuestBook::DestroyFrames()
{
	m_vecQuestID.clear();
	m_setTrackID.clear();	            // 紀錄正在追蹤的任務, 記陸內容為 m_vecQuestID 的順序 ( Index )
}

//--------------------------------------------------------------------------------------
int	LuaFunc_GetNumQuestBookButton_QuestBook	( lua_State *L )
{
	if( g_pQuestBook )
	{
		g_pQuestBook->m_vecQuestID.clear();

		RoleDataEx*			pRole		= CNetGlobal::RoleData();

		// 計算任務
		int iTotalQuest = 0;
		for( int index = 0; index < _MAX_QUEST_COUNT_; index++ )
		{
			int					iQuestID	= pRole->PlayerBaseData->Quest.QuestLog[ index ].iQuestID;
			GameObjDbStruct*	pObj		= g_ObjectData->GetObj( iQuestID );

			if( pObj )
			{
				// 這邊推入要照 分類, 然後任務等級排序
				bool	bInsert		= false;
				int		iCatalog	= pObj->QuestDetail.iQuest_Catalog;
				int		iLV			= pObj->QuestDetail.iCheck_LV;

				vector< int >::iterator it = g_pQuestBook->m_vecQuestID.begin();

				for( it = g_pQuestBook->m_vecQuestID.begin(); it != g_pQuestBook->m_vecQuestID.end(); it++ )
				{
					GameObjDbStruct*	pQuestObj = g_ObjectData->GetObj( *it );

					if( iCatalog <= pQuestObj->QuestDetail.iQuest_Catalog )
					{
						if( iLV < pQuestObj->QuestDetail.iCheck_LV )
						{
							g_pQuestBook->m_vecQuestID.insert( it, iQuestID );
							bInsert	= true;
							break;;
						}
					}
				}

				if( bInsert == false )
				{
					g_pQuestBook->m_vecQuestID.push_back( iQuestID );
				}
				


				iTotalQuest++;
			}
		}


		// 檢查角色目前擁有的任務, 若沒有任務則顯示你沒有任何的任務訊息
		//if( g_pQuestBook->m_vecQuestID.size() == 0 )
		//{
		//	g_pGameMain->SendSystemMsgEx( g_ObjectData->GetString( "QUEST_MSG_NOQUEST" ) );
		//}




		// TODO 這邊應該要由記錄檔讀取追蹤內容





		lua_pushnumber( L, iTotalQuest );
		return 1;
	}

	return 0;
}

//--------------------------------------------------------------------------------------
int LuaFunc_GetQuestInfo ( lua_State *L )
{
	bool bCompleteQuest = false;
	if( g_pQuestBook )
	{
		int iIndex		= luaL_checkint( L, 1 ) - 1;
		

		if( iIndex < g_pQuestBook->m_vecQuestID.size() )
		{
			RoleDataEx*			pRole		= CNetGlobal::RoleData();
			int					iQuestID	= g_pQuestBook->m_vecQuestID[ iIndex ];
			GameObjDbStructEx*	pObj		= g_ObjectData->GetObj( iQuestID );

			int					iQuestField = pRole->CheckQuestList_IsGotQuest( iQuestID );

			if( pObj )
			{
				int iQuestRequest	= 0;	// 0 表沒有需求, 1 表有需求, 但沒打開點選, 2 表有需求, 且快顯
				int iNumRequest		= g_pQuestDetail->GetQuestRequest( iQuestID );

				if( iNumRequest != 0 )
				{
					if( g_pQuestBook->m_setTrackID.find( iQuestID ) == g_pQuestBook->m_setTrackID.end() )
						iQuestRequest = 1;
					else
						iQuestRequest = 2;
				}

				char	szQuestName[256];
				int		iQuestType		= pObj->QuestDetail.iQuest_Type;

				// 檢查任務是否已完成
				if( iQuestField != -1 )
				{
					if( pRole->CheckQuest_IsComplete( iQuestField, iQuestID, &( pObj->QuestDetail ) ) == EM_QUESTCHECK_RESULT_OK )
					{
						bCompleteQuest = true;
					}
				}

				if( iQuestType != 0 )
				{
					char szKeyStr[256];
	
					sprintf( szKeyStr, "QUEST_TYPE_%d", ( iQuestType + 2 ) );
					const char* pszString = g_ObjectData->GetString( szKeyStr );
					sprintf( szQuestName, "%s (%s)", pObj->GetName(), pszString );

				}
				else
				{
					strcpy( szQuestName, pObj->GetName() );
				}

//				if( bCompleteQuest == true )
//				{
//					sprintf( szQuestName, "%s (%s)", pObj->GetName(), g_ObjectData->GetString( "QUEST_S_REQUEST_COMPLETE") );
//				}

				//pQuest->QuestDetail.iControlType 	!= 0;		// 每日任務
				//pQuest->QuestDetail.iQuestGroup 	!= 0;		// 每日群組

				bool bDaily = false;
				int lua_bDaily_num = 0; //傳回lua  1表示為每日任務
				if( pObj->QuestDetail.iControlType != 0 || pObj->QuestDetail.iQuestGroup != 0 )
				{
					bDaily = true;
					lua_bDaily_num = 1;
				}

				lua_pushnumber( L, ( iIndex + 1 )						);	// 分類夾
				lua_pushnumber( L, ( pObj->QuestDetail.iQuest_Catalog	)	);	// 打開狀態
				lua_pushstring( L, szQuestName							);
				lua_pushnumber( L, iQuestRequest						);	// 快顯狀狀態
				lua_pushnumber( L, pObj->QuestDetail.iCheck_LV			);	// 快顯狀狀態
  				lua_pushboolean( L, bDaily								);	// 每日
				lua_pushnumber( L, lua_bDaily_num						);	// 每日
				lua_pushnumber( L, iQuestType							);  // 任務類別
				lua_pushnumber( L, iQuestID                             );  // 任務ID
				lua_pushboolean( L, bCompleteQuest ); //任務是否已完成
				lua_pushnumber( L, pObj->QuestDetail.iQuestGroup ); //任務是否已完成
				return 11;

			}
		}
	}
			
	return 0;
}
//--------------------------------------------------------------------------------------
int			LuaFunc_ViewQuest_QuestBook					( lua_State *L )
{
	if( g_pQuestBook )
	{
		int iIndex	= luaL_checkint( L, 1 ) - 1;

		if( iIndex < g_pQuestBook->m_vecQuestID.size() )
		{
			int iQuestID = g_pQuestBook->m_vecQuestID[ iIndex ];
			g_pQuestDetail->SetQuest( 1, iIndex, iQuestID );
		}
	}

	return 0;
}
//--------------------------------------------------------------------------------------
int LuaFunc_GetQuestCatalogInfo ( lua_State *L )
{
	if( g_pQuestBook )
	{
		char szKeyStr[256];
		int iIndex	= luaL_checkint( L, 1 );

		sprintf( szKeyStr, "QUEST_DIR_%03d", iIndex );
		const char* pszString = g_ObjectData->GetString( szKeyStr );

		if( pszString == NULL || strcmp( pszString, "" ) == 0 || strcmp( pszString, szKeyStr ) == 0 )
		{
			sprintf( szKeyStr, "QUEST_DIR_%02d", iIndex );
			pszString = g_ObjectData->GetString( szKeyStr );

			if( pszString == NULL || strcmp( pszString, "" ) == 0 || strcmp( pszString, szKeyStr ) == 0 )
			{
				sprintf( szKeyStr, "QUEST_DIR_%d", iIndex );
				pszString = g_ObjectData->GetString( szKeyStr );
			}
		}

		if( pszString == NULL || strcmp( pszString, "" ) == 0 )
		{
			lua_pushstring( L, "" );	
		}
		else
		{
			lua_pushstring( L, pszString );
		}
		
		return 1;
	}

	return 0;
}
//--------------------------------------------------------------------------------------
string CUI_QuestBook::GetLuaStringResult( const char* pszScript )
{
	lua_State*	L			= GetLuaStateWorld();
	int			iStack		= lua_gettop (L);
	
	string		str;

	if ( L )
	{		
		lua_getglobal( L, pszScript );

		if (!lua_isnil( L, -1))
		{
			if (lua_pcall( L, 0, 1, 0) != 0)
			{				
				const char* err = lua_tostring( L, -1);
				//lua_pop( L, 1);
				UiOuputError(err);
			}
			else
			{

				int debug_StackNum = lua_gettop (L);

				switch (lua_type(L, -1))
				{
				case LUA_TSTRING:
					str = (bool)( atoi( lua_tostring(L,-1) ) );
					break;
				}
			}
		}
	}

	int iStackNow = lua_gettop (L);
	lua_pop( L, ( lua_gettop (L) - iStack ) );

	return str;
}
//--------------------------------------------------------------------------------------
bool CUI_QuestBook::CheckQuestBeginLua( const char* pszScript )
{
	lua_State*	L		= GetLuaStateWorld();
	int			iStack	= lua_gettop (L);
	bool		bReturn = false;

	if ( L )
	{		
		lua_getglobal( L, pszScript );

		if (!lua_isnil( L, -1))
		{
			if (lua_pcall( L, 0, 1, 0) != 0)
			{				
				const char* err = lua_tostring( L, -1);
				//lua_pop( L, 1);
				UiOuputError(err);
			}
			else
			{
				
				int debug_StackNum = lua_gettop (L);
				
				switch (lua_type(L, -1))
				{
				case LUA_TNUMBER:
					bReturn	= (bool)lua_tonumber(L, -1);
					break;
				case LUA_TSTRING:
					bReturn = (bool)( atoi( lua_tostring(L,-1) ) );
					break;
				case LUA_TBOOLEAN:
					bReturn = (bool) lua_toboolean(L,-1);
					break;
				case LUA_TNIL:
					bReturn = false;
					break;
				}

				//int iReturn	= luaL_checkint( L, 1 );
				//lua_pop( L, 1 );
			}

		}
		else
		{
			//const char* err = lua_tostring( L, -1);
			//lua_pop( L, 1);
			//UiOuputError(err);
		}
	}

	int iStackNow = lua_gettop (L);
	lua_pop( L, ( lua_gettop (L) - iStack ) );

	return bReturn;
}
//-------------------------------------------------------------------------------------------------
int LuaFunc_GetVocLV ( lua_State *L )
{	
	int iIndex	= luaL_checkint( L, 1 );

	if( iIndex >=0 && iIndex < 16 )
	{
		RoleDataEx*	pRole		= CNetGlobal::RoleData();
		int			iLV			= pRole->PlayerBaseData->AbilityList[ iIndex ].Level;

		lua_pushnumber( L, iLV );
		return 1;
	}

	return 0;
}
//-------------------------------------------------------------------------------------------------
int	LuaFunc_CheckQuest ( lua_State *L )
{	
	RoleDataEx*	pRole		= CNetGlobal::RoleData();
	int			iQuestID	= luaL_checkint( L, 1 );

	
		//int			iLV			= pRole->PlayerBaseData->AbilityList[ iIndex ].Level;

		//lua_pushnumber( L, iLV );
	int iResult = 0; // 0 未接未完成 1 已接 2 已完成

	if( pRole->CheckQuestHistory_IsComplete( iQuestID ) )
	{
		iResult = 2;
	}
	else
	if( pRole->CheckQuestList_IsGotQuest( iQuestID ) != -1 )
	{
		iResult = 1;
	}

	lua_pushnumber( L, iResult );
	return 1;
}
//-------------------------------------------------------------------------------------------------
int	LuaFunc_CheckFlag ( lua_State *L )
{
	RoleDataEx*	pRole		= CNetGlobal::RoleData();
	int iFlagID = luaL_checkint( L, 1 );
	int iResult = (int) pRole->BaseData.KeyItem.GetFlag( iFlagID - Def_ObjectClass_KeyItem );

	lua_pushnumber( L, iResult );
	return 1;
}
//-------------------------------------------------------------------------------------------------
int	LuaFunc_GetQuestTrack ( lua_State *L )
{
	int iIndex		= luaL_checkint( L, 1 );
	int iTrackSize	= (int)g_pQuestBook->m_setTrackID.size();

	if( iIndex == -1 )
	{		
		lua_pushnumber( L, iTrackSize );
		return 1;
	}
	else
	{
		iIndex = iIndex - 1;

		int iQuestGUID	= g_pQuestBook->GetQuestGUID( iIndex );

		if( iIndex >= 0 && iIndex < iTrackSize )
		{
			//int iIndex = g_pQuestBook->m_setTrackID.[ iIndex ];
			int iQuestGUID = 0;

			set< int >::iterator it = g_pQuestBook->m_setTrackID.begin();
			for( int i=0; i < g_pQuestBook->m_setTrackID.size(); i++ )
			{
				if( i == iIndex )
				{
					iQuestGUID = *it;
					break;
				}

				it++;
			}

			int iCount = 0;
			for( vector<int>::iterator it = g_pQuestBook->m_vecQuestID.begin(); it != g_pQuestBook->m_vecQuestID.end(); it++ )
			{
				iCount++;
				if( *it == iQuestGUID )
				{					
					lua_pushnumber( L, iCount );
					return 1;
				}

			}
		}
	}

	return 0;
}
//-------------------------------------------------------------------------------------------------
int LuaFunc_SetQuestTrack ( lua_State *L )
{
 	int iIndex = luaL_checkint( L, 1 );

	int iQuestGUID = -1;
	if( iIndex >= Def_ObjectClass_QuestDetail && 
		iIndex < Def_ObjectClass_QuestDetail + Def_ObjectClass_QuestDetail_Total )
	{
		iQuestGUID = iIndex;
	}
	else
	{
		iQuestGUID = g_pQuestBook->GetQuestGUID( iIndex - 1 );
	}

	int iNumShowTrack	= 5;

	if( iQuestGUID != -1 )
	{
		int iTrackSize = (int)g_pQuestBook->m_setTrackID.size();
	
		//if( iTrackSize >= 5 && g_pQuestBook->m_setTrackID.find( iQuestGUID ) == g_pQuestBook->m_setTrackID.end() )
		/*
		if( g_pQuestBook->m_setTrackID.find( iQuestGUID ) == g_pQuestBook->m_setTrackID.end() )
		{
			g_pGameMain->SendSystemMsg( g_ObjectData->GetString( "QUEST_MSG_FULLTRACK" ) );
			g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString( "QUEST_MSG_FULLTRACK" ) );

			// 應該可以改為直接判斷回傳值做跟事件觸發相同的事
			g_pInterface->SendWorldEvent( UiEvent_ResetQuestTrack );
			lua_pushnumber( L, 0 );
			return 1;
		}
		*/


		if( g_pQuestBook->m_setTrackID.find( iQuestGUID ) == g_pQuestBook->m_setTrackID.end() )
		{
			g_pQuestBook->m_setTrackID.insert( iQuestGUID );
			g_pInterface->SendWorldEvent( UiEvent_ResetQuestTrack );
			lua_pushnumber( L, 1 );
			return 1;
		}
		else
		{
			g_pQuestBook->m_setTrackID.erase( iQuestGUID );
			g_pInterface->SendWorldEvent( UiEvent_ResetQuestTrack );
			lua_pushnumber( L, 0 );
			return 1;
		}
	}

	//g_pGameMain->GetInterface()->GetUiWorld()->SendEvent( UiEvent_ResetQuestTrack );

	
	return 0;
}
//-------------------------------------------------------------------------------------------------
int	LuaFunc_GetCurrentVocLV	( lua_State *L )
{
		RoleDataEx*	pRole		= CNetGlobal::RoleData();
		int			iLV			= pRole->PlayerBaseData->AbilityList[ pRole->BaseData.Voc ].Level;

		lua_pushnumber( L, iLV );
		return 1;
}
//-------------------------------------------------------------------------------------------------
int	LuaFunc_DeleteQuestByID	( lua_State *L )
{
	int iQuestID		= luaL_checkint( L, 1 );

	RoleDataEx*	pRole	= CNetGlobal::RoleData();

	int iQuestField = -1;

	if( pRole != NULL )
	{
		for( int index = 0; index < _MAX_QUEST_COUNT_; index++ )
		{
			if( pRole->PlayerBaseData->Quest.QuestLog[ index ].iQuestID == iQuestID )
			{
				iQuestField = index;
				break;
			}
		}

		if( iQuestField != -1 )
		{
			CNetCli_Script::DeleteQuest( iQuestField, iQuestID );
		}	
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_QuestBook_GetQuestHyperLink ( lua_State *L )
{
	int iIndex     = luaL_checkint( L, 1 ) - 1;
	int iQuestGUID = g_pQuestBook->GetQuestGUID( iIndex );

	string link;
	if( !QuestGUID_To_HyperLink( iQuestGUID, link ) )
	{
		return 0;
	}
	lua_pushstring(L, link.c_str() );
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_QuestDebug( lua_State *L )
{
	if( !g_pGameMain->IsShowServerDebugMsg() )
	{
		return 0;
	}

	int iQuestGUID = luaL_checkint( L, 1 );

	g_pGameMain->SendSystemChatMsgEx( "Quest(%d) Debug:", iQuestGUID );
	
	RoleDataEx* pRole = CNetGlobal::RoleData();

 	GameObjDbStructEx* pQuestDetail = g_ObjectData->GetObj( iQuestGUID ); 

	// 檢查角色是否已完成該任務
	if( pRole->CheckQuestHistory_IsComplete( iQuestGUID ) == true )
	{
		g_pGameMain->SendSystemChatMsg( "IsComplete(Yes)" );
	}
	else
	{
		g_pGameMain->SendSystemChatMsg( "IsComplete(NO)" );
	}


	if( pRole->CheckQuest_IsAvailable( iQuestGUID, &pQuestDetail->QuestDetail, true ) == EM_QUESTCHECK_RESULT_OK )
	{
		g_pGameMain->SendSystemChatMsg( "CheckQuest_IsAvailable(Yes)" );
	}
	else
	{
 		g_pGameMain->SendSystemChatMsg( "CheckQuest_IsAvailable(NO)" );
	}
	

	bool bLuaCheck = true;
	// 檢查有無 LUA QUESTBEGIN
	if( strlen( pQuestDetail->QuestDetail.szScript_OnBegin ) != 0 )
	{
		bLuaCheck = g_pQuestBook->CheckQuestBeginLua( pQuestDetail->QuestDetail.szScript_OnBegin );
	}

	if( bLuaCheck )
	{
		g_pGameMain->SendSystemChatMsg( "CheckQuestBeginLua(Yes)" );		
	}
	else
	{
 		g_pGameMain->SendSystemChatMsg( "CheckQuestBeginLua(NO)" );		
	}
	
	if( pQuestDetail )
	{
		for( int i = 0 ; i < 5 ; ++i )
		{
			int NpcID = pQuestDetail->QuestDetail.iQuestProvider[i];
			GameObjDbStructEx* pObjNPC = g_ObjectData->GetObj( NpcID );
			if( NpcID != 0 && NULL== pObjNPC )
			{
				g_pGameMain->SendSystemChatMsgEx( "iQuestProvider %d = %d, Not Exist!!", i + 1 , NpcID );
			}
			else if( pObjNPC )
			{
				if( CNpcTrackFrame::IsNpcVisible( pObjNPC, true ) )
				{
					g_pGameMain->SendSystemChatMsgEx( "Quest Provider %d = %d", i + 1 , NpcID );
				}
				else
				{
					g_pGameMain->SendSystemChatMsgEx( "Quest Provider %d = %d, invisible!!", i + 1 , NpcID );
				}
			}
		}
	}

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_QuestNpcDebug( lua_State *L )
{
	RoleDataEx* pRole = CNetGlobal::RoleData();
	if( !g_pGameMain->IsShowServerDebugMsg() )
	{
		return 0;
	}

	int iNpcGUID = luaL_checkint( L, 1 );

	g_pGameMain->SendSystemChatMsgEx( "QuestNpc(%d) Debug:", iNpcGUID );

	GameObjDbStructEx* pObjNPC = g_ObjectData->GetObj( iNpcGUID );
	if( NULL== pObjNPC )
	{
		g_pGameMain->SendSystemChatMsgEx( "Not Exist!!", iNpcGUID );
		return 0;
	}

	if( !CNpcTrackFrame::IsNpcVisible( pObjNPC, true ) )
	{
		g_pGameMain->SendSystemChatMsgEx( "invisible!!", iNpcGUID );
	}


  	g_pGameMain->SendSystemChatMsgEx( "IsShowNpcPos(%d)", pObjNPC->NPC.IsShowNpcPos );
	g_pGameMain->SendSystemChatMsgEx( "ZoneID(%d)", pObjNPC->NPC.ZoneID );

  	int n;
	int iQuestID;
	int* pQuestList = pObjNPC->NPC.iQuestID;

	for( n = 0; n < _MAX_NPC_QUEST_COUNT_; ++n )
	{
		iQuestID = pQuestList[ n ];
		if( iQuestID == 0 )
		{
			break;
		}

		// 檢查角色是否已完成該任務
		if( pRole->CheckQuestHistory_IsComplete( iQuestID ) == true )
		{
			g_pGameMain->SendSystemChatMsgEx( "   %d Quest(%d) , completed", n + 1 , iQuestID );
		}
		else
		{
			g_pGameMain->SendSystemChatMsgEx( "   %d Quest(%d)", n + 1 , iQuestID );
		}
	}

	return 0;
}




// ----------------------------------------------------------------------------
void CUI_QuestBook::Save()
{	
	FILE* fptr = NULL;
	wstring globalName = GetGlobalPathW(L"QuestTrack.bsd");
	wstring localName = GetLocalPathW(L"QuestTrack.bsd");

	// 儲存共用資料			

	// 記錄資料	

	// 關閉檔案	

	// 儲存本地資料
	if ( (fptr = _wfopen(localName.c_str(), L"wb")) != NULL )
	{
		//int size;
		//int version;
		//int classNum;
		//int itemNum;

		int iTrackSize	= (int)g_pQuestBook->m_setTrackID.size();
		fwrite(&iTrackSize, sizeof(int), 1, fptr);

		for( set< int >::iterator it = g_pQuestBook->m_setTrackID.begin(); it != g_pQuestBook->m_setTrackID.end(); it++ )
		{
			int iQuestID = *it;
			fwrite(&iQuestID, sizeof(int), 1, fptr);
		}

		// 關閉檔案
		fclose(fptr);
	}
}

// ----------------------------------------------------------------------------
void CUI_QuestBook::Load()
{
	FILE* fptr = NULL;
	wstring globalName = GetGlobalPathW(L"QuestTrack.bsd");
	wstring localName = GetLocalPathW(L"QuestTrack.bsd");

	RoleDataEx*		pRole =CNetGlobal::RoleData();

	// 讀取共用資料

	// 記錄資料	

	// 關閉檔案

	// 讀取本地資料
	if ( (fptr = _wfopen(localName.c_str(), L"rb")) != NULL )
	{
		int iQuestID;
		int iTrackSize;

		fread(&iTrackSize, sizeof(int), 1, fptr);

		for( int i=0; i< iTrackSize; i++ )
		{
			fread(&iQuestID, sizeof(int), 1, fptr);

			if( pRole->CheckQuestList_IsGotQuest( iQuestID ) != -1 )
			{
				g_pQuestBook->m_setTrackID.insert( iQuestID );
			}
		}
		// 關閉檔案
		fclose(fptr);
	}	

	g_pInterface->SendWorldEvent( UiEvent_ResetQuestTrack );

	SetSaveUpdateTime();
}

// ----------------------------------------------------------------------------
void CUI_QuestBook::SaveClientData()
{
	// 產生暫存資料
	CTempFile tempFile(65536);

	int iTrackSize	= (int)g_pQuestBook->m_setTrackID.size();
	tempFile.Write(&iTrackSize, sizeof(int));

	for( set< int >::iterator it = g_pQuestBook->m_setTrackID.begin(); it != g_pQuestBook->m_setTrackID.end(); it++ )
	{
		int iQuestID = *it;
		tempFile.Write(&iQuestID, sizeof(int));
	}

	g_pGameMain->SaveClientData(eSaveClientData_QuestTrack, tempFile.GetPosition(), tempFile.GetData());

	// 將最後存檔時間去除
	SetSaveUpdateTime(0.0f);
}

// ----------------------------------------------------------------------------
void CUI_QuestBook::LoadClientData(int type, int size, char* data)
{
	if ( type == eSaveClientData_QuestTrack )
	{
		// 產生暫存資料
		CTempFile tempFile(data,size);

		int iQuestID;
		int iTrackSize;

		tempFile.Read(&iTrackSize, sizeof(int));

		RoleDataEx*	pRole = CNetGlobal::RoleData();

		for( int i=0; i< iTrackSize; i++ )
		{
			tempFile.Read(&iQuestID, sizeof(int));

			if( pRole->CheckQuestList_IsGotQuest( iQuestID ) != -1 )
			{
				g_pQuestBook->m_setTrackID.insert( iQuestID );
			}
		}
	}
}
// ----------------------------------------------------------------------------
void CUI_QuestBook::UpdateQuestItem( int iQuestID )
{			
	if( g_pQuestBook->m_setTrackID.find( iQuestID ) != g_pQuestBook->m_setTrackID.end() )
	{
		g_pQuestBook->m_setTrackID.erase(iQuestID);
	}
}

