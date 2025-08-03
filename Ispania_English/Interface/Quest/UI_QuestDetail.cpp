#include "../../mainproc/GameMain.h"					// For Obj and Role struct
#include "../../netwaker_member/NetWakerClientInc.h"	// For NetGlobal

#include "UI_QuestBook.h"
#include "UI_QuestList.h"
#include "UI_QuestDetail.h"
#include "../WorldFrames/ChatFrame.h"
#include "../ItemLink/ItemLink.h"

#include "RoleData/ObjectDataClass.h"

#include "../../tools/Common/Com_Function.h"


CUI_QuestDetail*		g_pQuestDetail	= NULL;
//-------------------------------------------------------------------
int			LuaFunc_GetQuestName_QuestDetail			( lua_State *L );
int			LuaFunc_GetQuestDesc_QuestDetail			( lua_State *L );
int			LuaFunc_GetQuestDetail_QuestDetail			( lua_State *L );
int			LuaFunc_GetQuestDetail_QuestDetailShort		( lua_State *L );
int			LuaFunc_GetQuestItemNumByType_QuestDetail	( lua_State *L );
int			LuaFunc_GetQuestItemInfo_QuestDetail		( lua_State *L );
int			LuaFunc_GetQuestLV_QuestDetail				( lua_State *L );
int			LuaFunc_GetQuestExp_QuestDetail				( lua_State *L );
int			LuaFunc_GetQuestTP_QuestDetail				( lua_State *L );
int			LuaFunc_GetQuestMoney_QuestDetail			( lua_State *L );



int			LuaFunc_ProcessQuest						( lua_State *L );
int			LuaFunc_DeleteQuest							( lua_State *L );
int			LuaFunc_AcceptQuest							( lua_State *L );
int			LuaFunc_CompleteQuest						( lua_State *L );
int			LuaFunc_IsQuestComplete_QuestDetail			( lua_State *L );
int			LuaFunc_GetQuestStatus_QuestDetail			( lua_State *L );
int			LuaFunc_SetChoiceItem_QuestDetail			( lua_State *L );
int			LuaFunc_GetChoiceItem_QuestDetail			( lua_State *L );
int			LuaFunc_GetQuestRequest						( lua_State *L );

int			LuaFunc_RegColorKeyWord						( lua_State *L );
int			LuaFunc_GetNewbieQuestGuide					( lua_State *L );

int			LuaFunc_QuestDetail_GetQuestNPC				( lua_State *L );
int			LuaFunc_QuestDetail_GetRequestQuestNPC		( lua_State *L );

//-------------------------------------------------------------------
CUI_QuestDetail::CUI_QuestDetail( CInterface* object ) : CInterfaceBase( object )
{
	g_pQuestDetail			= this;	
	m_iQuestID				= 0;
	m_bInitLuaColorKeyWord	= false;
}
// --------------------------------------------------------------------------------------
CUI_QuestDetail::~CUI_QuestDetail(void)
{
	g_pQuestDetail = NULL;
}
//--------------------------------------------------------------------------------------
void CUI_QuestDetail::RegisterFunc(void)
{
	lua_State* l = GetLuaStateWorld();
	if (l)
	{
		lua_register( l, "GetQuestName_QuestDetail",				LuaFunc_GetQuestName_QuestDetail			);
		lua_register( l, "GetQuestDesc_QuestDetail",				LuaFunc_GetQuestDesc_QuestDetail			);
		lua_register( l, "GetQuestDetail_QuestDetail",				LuaFunc_GetQuestDetail_QuestDetail			);
		lua_register( l, "GetQuestDetail_QuestDetailShort",			LuaFunc_GetQuestDetail_QuestDetailShort		);
		lua_register( l, "GetQuestItemNumByType_QuestDetail",		LuaFunc_GetQuestItemNumByType_QuestDetail	);
		lua_register( l, "GetQuestItemInfo_QuestDetail",			LuaFunc_GetQuestItemInfo_QuestDetail		);
		lua_register( l, "GetQuestLV_QuestDetail",					LuaFunc_GetQuestLV_QuestDetail				);
		lua_register( l, "GetQuestExp_QuestDetail",					LuaFunc_GetQuestExp_QuestDetail				);
		lua_register( l, "GetQuestTP_QuestDetail",					LuaFunc_GetQuestTP_QuestDetail				);		
		lua_register( l, "GetQuestMoney_QuestDetail",				LuaFunc_GetQuestMoney_QuestDetail			);
		lua_register( l, "ProcessQuest",							LuaFunc_ProcessQuest						);
		lua_register( l, "DeleteQuest",								LuaFunc_DeleteQuest							);
		lua_register( l, "AcceptQuest",								LuaFunc_AcceptQuest							);
		lua_register( l, "CompleteQuest",							LuaFunc_CompleteQuest						);
		lua_register( l, "IsQuestComplete_QuestDetail",				LuaFunc_IsQuestComplete_QuestDetail			);
		lua_register( l, "GetQuestStatus_QuestDetail",				LuaFunc_GetQuestStatus_QuestDetail			);
		lua_register( l, "SetChoiceItem_QuestDetail",				LuaFunc_SetChoiceItem_QuestDetail			);
		lua_register( l, "GetChoiceItem_QuestDetail",				LuaFunc_GetChoiceItem_QuestDetail			);
		lua_register( l, "GetQuestRequest",							LuaFunc_GetQuestRequest						);
		lua_register( l, "RegColorKeyWord",							LuaFunc_RegColorKeyWord						);
		lua_register( l, "GetNewbieQuestGuide",						LuaFunc_GetNewbieQuestGuide					);

		lua_register( l, "QuestDetail_GetQuestNPC",					LuaFunc_QuestDetail_GetQuestNPC				);
		lua_register( l, "QuestDetail_GetRequestQuestNPC",			LuaFunc_QuestDetail_GetRequestQuestNPC		);		
	}
}
//--------------------------------------------------------------------------------------
void CUI_QuestDetail::SetQuest		( int iMode, int iIndex, int iQuestID )
{ 
	GameObjDbStruct*	pObj	= NULL;
	if( g_pQuestDetail )
	{
		GameObjDbStructEx*	pObj		= g_ObjectData->GetObj( iQuestID );

		if( pObj )
		{
			// 重置狀態
			m_vecQuestItem[0].clear();
			m_vecQuestItem[1].clear();
			m_vecQuestItem[2].clear();

			m_iRewardItemChoiceID	= -1;

			// 設定資料
			RoleDataEx*			pRole		= CNetGlobal::RoleData();

			m_iMode		= iMode;
			m_iQuestID	= iQuestID;
			m_sName		= pObj->GetName();

			m_iLV		= pObj->QuestDetail.iCheck_LV;
			m_iExp		= 0;
			m_iTP		= 0;
			m_iMoney	= 0;


			int lv = m_iLV - 1;

			if( lv < 0 )
				lv = 0;

			if( RoleDataEx::PlotExpTable.size() > (unsigned)lv )
				m_iExp = RoleDataEx::PlotExpTable[ lv ] * pObj->QuestDetail.iReward_Exp / 100 ;

			if( RoleDataEx::PlotMoneyTable.size() > (unsigned)(lv) )
				m_iMoney = RoleDataEx::PlotMoneyTable[ lv ] * pObj->QuestDetail.iReward_Money / 100 ;

			m_iTP	= int(m_iExp * ((float)pObj->QuestDetail.iReward_TP / 100));

			GameObjectQuestDetail	*pQuest		= &( pObj->QuestDetail );

			// 設定物品資料

			// 需求物品
			for( int i = 0; i < _MAX_SIZE_QUEST_ITEM; i++ )
			{
				int iItemID = pQuest->iRequest_ItemID[ i ] ;
				if( iItemID != 0 )
				{
					StructQuestItem		item;
					GameObjDbStructEx*	pItemObj	= g_ObjectData->GetObj( iItemID );

					if( pItemObj && pItemObj->IsItem() )
					{
						if( pItemObj->IsKeyItem() && pItemObj->KeyItem.Type == 0 )
							continue; // 物品隱藏

						GameObjDbStructEx* imageDB = g_ObjectData->GetObj(pItemObj->ImageID);
						if ( imageDB )
							item.sIconPath = imageDB->Image.ACTField;
						else
							item.sIconPath = "";

						item.iItemID	= pQuest->iRequest_ItemID[ i ];
						item.iItemVal	= pQuest->iRequest_ItemVal[ i ];
						item.sName		= pItemObj->GetName();

						m_vecQuestItem[0].push_back( item );

					}
				}
			}

			// 獎勵物品
			for( int i = 0; i < _MAX_SIZE_QUEST_ITEM; i++ )
			{
				int iItemID = pQuest->iReward_ItemID[ i ] ;
				if( iItemID != 0 )
				{
					StructQuestItem		item;
					GameObjDbStructEx*	pItemObj	= g_ObjectData->GetObj( iItemID );

					if( pItemObj )
					{
						GameObjDbStructEx* imageDB = g_ObjectData->GetObj(pItemObj->ImageID);
						if ( imageDB )
							item.sIconPath = imageDB->Image.ACTField;
						else
							item.sIconPath = "";

						item.iItemID	= pQuest->iReward_ItemID[ i ];
						item.iItemVal	= pQuest->iReward_ItemVal[ i ];
						item.sName		= pItemObj->GetName();

						m_vecQuestItem[2].push_back( item );

					}
				}
			}

			// 獎勵選則物品
			for( int i = 0; i < _MAX_SIZE_QUEST_ITEM; i++ )
			{
				int iItemID = pQuest->iReward_ChoiceID[ i ] ;



				if( iItemID != 0 )
				{
					StructQuestItem		item;
					GameObjDbStructEx*	pItemObj	= g_ObjectData->GetObj( iItemID );

					if( pItemObj )
					{	
						GameObjDbStructEx* imageDB = g_ObjectData->GetObj(pItemObj->ImageID);
						if ( imageDB )
							item.sIconPath = imageDB->Image.ACTField;
						else
							item.sIconPath = "";
													
						m_iRewardItemChoiceID			= 0;	

						item.iItemID	= pQuest->iReward_ChoiceID[ i ];
						item.iItemVal	= pQuest->iReward_ChoiceVal[ i ];						
						item.sName		= pItemObj->GetName();

						m_vecQuestItem[1].push_back( item );
					}
				}
			}

			//m_isComplete	= pRole->CheckQuest_IsComplete( 

			if( iMode == 0 )
			{
				// From NPC

				int						iQuestField = pRole->CheckQuestList_IsGotQuest( iQuestID );

				if( iQuestField != -1 )
				{
					// 檢查是否已完成該 Quest
					if( pRole->CheckQuest_IsComplete( iQuestField, iQuestID, pQuest ) == EM_QUESTCHECK_RESULT_OK )
					{
						// 已完成
						m_sDetail	= pQuest->szQuest_Complete_Detail;
						m_iStatus	= 2;
					}
					else
					{
						m_iStatus	= 1;

						if( strlen( pQuest->szQuest_UnComplete_Detail ) != 0 )
						{							
							m_sDetail	= pQuest->szQuest_UnComplete_Detail;	// 未完成							
						}
						else
						{
							m_sDetail	= g_ObjectData->GetString( "QUEST_MSG_UNCOMPLETE" );
						}
					}
				}
				else
				{
					// 待接
					m_sDesc		= pQuest->szQuest_Desc;
					m_sDetail	= pQuest->szQuest_Accept_Detail;
					m_iStatus	= 0;
				}

				m_iQuestNPC = -1;
				m_iRequestQuestNPC = -1;

				g_pInterface->SendWorldEvent(UiEvent_ShowQuestDetailFromNPC);
			}
			else
			{
				// From Book

				m_sDesc		= pQuest->szQuest_Desc;
				m_sDetail	= pQuest->szQuest_Accept_Detail;

				//找出給任務NPC 跟 回任務NPC
				m_iQuestNPC = -1;
				m_iRequestQuestNPC = -1;

				int QuestNPC_Begin = Def_ObjectClass_QuestNPC;
				int QuestNPC_End = Def_ObjectClass_Item - 1;

				GameObjDbStructEx* pObjNPC;
				//GameObjDbStructEx* pObjQuest;

				//int iGotQuestIndex;
				int* pQuestList;
				int i,n;
				for( i = QuestNPC_Begin ; i < QuestNPC_End ; ++i )
				{
					pObjNPC = g_ObjectData->GetObj( i );
					if( !pObjNPC )
						continue;

					if( pObjNPC->NPC.ZoneID <= 0 )
						continue;

					pQuestList = pObjNPC->NPC.iQuestID;

					for( n = 0; n < _MAX_NPC_QUEST_COUNT_; ++n )
					{
						if( pQuestList[ n ] == 0 )
						{
							break;
						}

						if( pQuestList[ n ] == iQuestID )
						{
							m_iQuestNPC = i;
							break;
						}
					}

					if( m_iQuestNPC != -1 )
					{
						break;
					}
				}


				if( pQuest->iRequest_QuestID != 0 )
				{
					m_iRequestQuestNPC = pQuest->iRequest_QuestID;
				}
				else
				{
					m_iRequestQuestNPC = m_iQuestNPC;
				}

				//判斷NPC能不能搜索的到

				if( m_iQuestNPC > 0 )
				{
					//pObjDB->NPC.ZoneID
					pObjNPC = g_ObjectData->GetObj( m_iQuestNPC );
					if( pObjNPC == NULL || pObjNPC->NPC.ZoneID <= 0 )
					{
						m_iQuestNPC = -1;	
					}

				}

				if( m_iRequestQuestNPC > 0 )
				{
					pObjNPC = g_ObjectData->GetObj( m_iRequestQuestNPC );
					if( pObjNPC == NULL || pObjNPC->NPC.ZoneID <= 0 )
					{
						m_iRequestQuestNPC = -1;	
					}
				}

				// 插入選則的 Index
				lua_State* L = GetLuaStateWorld();

				lua_pushnumber(L, ( iIndex + 1 ) );
				lua_setglobal(L, "arg1");
				g_pInterface->SendWorldEvent(UiEvent_ShowQuestDetailFromBook);
			}
		}
	}


}
//--------------------------------------------------------------------------------------
int	CUI_QuestDetail::GetQuestRequest ( int iQuestID )
{
	GameObjDbStruct*	pObj		= g_ObjectData->GetObj( iQuestID );
	RoleDataEx*			pRole		= CNetGlobal::RoleData();
	//char				szStr[256];

	if( pObj )
	{
		int iCount = 0;

		// 取得打倒目標數量
		for( int i = 0; i < _MAX_SIZE_QUEST_CONDITION; i++ )
		{
			int iGUID = pObj->QuestDetail.iRequest_KillID[ i ];
			if( iGUID != 0 )
			{
				iCount++;
			}
			else
				break;
		}

		// 取得需求物品數量		
		for( int i = 0; i < _MAX_SIZE_QUEST_CONDITION; i++ )
		{
			int iGUID = pObj->QuestDetail.iRequest_ItemID[ i ];
			if( iGUID != 0 )
			{
				GameObjDbStruct*	pRequest	= g_ObjectData->GetObj( iGUID );

				if( pRequest != NULL )
				{
					// 檢查是否跳過
					if( pRequest->Classification == EM_ObjectClass_KeyItem )
					{
						if( pRequest->KeyItem.Type == EM_KeyItemType_Invisible )
						{
							continue;
						}
					}					
					iCount++;
				}
			}
			else
				break;
		}

		return iCount;
	}

	return 0;
}
//--------------------------------------------------------------------------------------
int			LuaFunc_GetQuestItemNumByType_QuestDetail	( lua_State *L )
{
	if( g_pQuestDetail )
	{
		int iType	= luaL_checkint( L, 1 ) - 1;

		if( iType >= 0 && iType <= 2 )
		{
			int iNumItem = (int)g_pQuestDetail->m_vecQuestItem[ iType ].size();
			lua_pushnumber( L, iNumItem );
			return 1;
		}
	}
	return 0;
}
//--------------------------------------------------------------------------------------
int			LuaFunc_GetQuestItemInfo_QuestDetail	( lua_State *L )
{
	if( g_pQuestDetail )
	{
		int iType	= luaL_checkint( L, 1 ) - 1;
		int iIndex	= luaL_checkint( L, 2 ) - 1;

		if( iType >= 0 && iType <= 2 )
		{
			int iNumItem = (int)g_pQuestDetail->m_vecQuestItem[ iType ].size();

			if( iIndex < iNumItem )
			{
				CUI_QuestDetail::StructQuestItem&	item		= g_pQuestDetail->m_vecQuestItem[ iType ][ iIndex ];

				lua_pushstring( L, item.sName.c_str() );
				lua_pushstring( L, item.sIconPath.c_str() );
				lua_pushnumber( L, item.iItemVal );
				return 3;
			}
		}
	}	
	return 0;
}
//--------------------------------------------------------------------------------------
int			LuaFunc_GetQuestName_QuestDetail			( lua_State *L )
{
	if( g_pQuestDetail )
	{
		lua_pushstring( L, g_pQuestDetail->m_sName.c_str() );
		return 1;
	}

	return 0;
}
//--------------------------------------------------------------------------------------
int			LuaFunc_GetQuestDesc_QuestDetail			( lua_State *L )
{
	if( g_pQuestDetail )
	{
		bool bHyperlink = false;

		int n = lua_gettop(L);
		if( n > 0 )
		{
			bHyperlink = luaL_checknumber( L, 1 ) == 1 ? true : false ;
		}

		string sString = g_pQuestDetail->ReplaceKeyWord( g_pQuestDetail->m_sDesc, bHyperlink );
		lua_pushstring( L, sString.c_str() );
		return 1;
	}

	return 0;
}
//--------------------------------------------------------------------------------------
int			LuaFunc_GetQuestDetail_QuestDetail			( lua_State *L )
{
	if( g_pQuestDetail )
	{
		bool bHyperlink = false;

		int n = lua_gettop(L);
		if( n > 0 )
		{
			bHyperlink = luaL_checknumber( L, 1 ) == 1 ? true : false ;
		}

		// 比對 Color Key Word, 檢查是否需要變色
		string sString = g_pQuestDetail->ReplaceKeyWord( g_pQuestDetail->m_sDetail, bHyperlink );
		lua_pushstring( L, sString.c_str() );
		return 1;
	}
	return 0;
}
//--------------------------------------------------------------------------------------
int			LuaFunc_GetQuestDetail_QuestDetailShort		( lua_State *L )
{
	if( g_pQuestDetail )
	{
		// 比對 Color Key Word, 檢查是否需要變色
		string sString = g_pQuestDetail->ReplaceKeyWord( g_pQuestDetail->m_sDetail, false );

		string temp;

		CUiFontString::HyperlinkTextA(sString.c_str(), -1, temp);

		char shortText[64];

		int i = 0;
		while ( temp[i] )
		{
			char ch = temp[i];
			if ( i >= 40 )
				break;
			else
			{
				if ( (unsigned char)ch >= 0xF0 && (unsigned char)ch <= 0xF4 )
				{
					memcpy(shortText + i, temp.c_str() + i, 4);
					i += 4;
				}
				else if ( (unsigned char)ch >= 0xE0 && (unsigned char)ch <= 0xEF )
				{
					memcpy(shortText + i, temp.c_str() + i, 3);
					i += 3;
				}
				else if ( (unsigned char)ch >= 0xC2 && (unsigned char)ch <= 0xDF )
				{
					memcpy(shortText + i, temp.c_str() + i, 2);
					i += 2;
				}
				else
				{
					memcpy(shortText + i, temp.c_str() + i, 1);
					i += 1;
				}
			}
		}
		shortText[i] = 0;

		lua_pushstring(L, shortText);

		return 1;
	}
	return 0;
}
//--------------------------------------------------------------------------------------
int LuaFunc_GetQuestLV_QuestDetail( lua_State *L )
{
	if( g_pQuestDetail )
	{
		lua_pushnumber( L, g_pQuestDetail->m_iLV );
		return 1;
	}
    return 0;
}
//--------------------------------------------------------------------------------------
int LuaFunc_GetQuestExp_QuestDetail( lua_State *L )
{
	if( g_pQuestDetail )
	{
		lua_pushnumber( L, g_pQuestDetail->m_iExp );
		return 1;
	}
    return 0;
}
//--------------------------------------------------------------------------------------
int LuaFunc_GetQuestTP_QuestDetail( lua_State *L )
{
	if( g_pQuestDetail )
	{
		lua_pushnumber( L, g_pQuestDetail->m_iTP );
		return 1;
	}
    return 0;
}
//--------------------------------------------------------------------------------------
int LuaFunc_GetQuestMoney_QuestDetail( lua_State *L )
{
	if( g_pQuestDetail )
	{
		lua_pushnumber( L, g_pQuestDetail->m_iMoney );
		return 1;
	}
    return 0;
}

//--------------------------------------------------------------------------------------
string CUI_QuestDetail::ReplaceKeyWord	( string pSource, bool bHyperlink )
{
	return CNetCli_Script_Child::ReplaceSystemKeyword( pSource, true, bHyperlink, true );

/*
	size_t	iBegin		= 0;
	size_t	iEnd		= 0;

	int		iColor_NPC	= ObjectDataClass::St_SysValue()->SysQuestColor_NPC;
	int		iColor_Item	= ObjectDataClass::St_SysValue()->SysQuestColor_ITEM;
	int		iColor_Zone = ObjectDataClass::St_SysValue()->SysQuestColor_ZONE;


	char	szKeyWord[256];
	char	szColorText[256];

	int off = 0;

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

		if( iKeyWordLen == 6 && iGUID >= 100000 )
		{
			GameObjDbStruct*	pObj		= g_ObjectData->GetObj( iGUID );

			if( pObj == NULL )
				return pSource;

			if( iGUID >= 100000 && iGUID < 110000 ) // 怪物
			{	
				if( bHyperlink )
				{
					//sprintf( szColorText, "|Hnpc:%d|h|cff770dd0[%s]|r|h", iGUID, pObj->Name );
					sprintf( szColorText, "|Hnpc:%d|h|c%08x[%s]|r|h", iGUID, iColor_NPC, pObj->Name );
   				}
				else
				{
					//sprintf( szColorText, "|cff770dd0%s|r", pObj->Name );
					sprintf( szColorText, "|c%08x%s|r", iColor_NPC, pObj->Name );
				}
			}
			else if( iGUID >= 110000 && iGUID < 120000 )
			{
				// 任務 NPC
				if( pObj->NPC.iQuestMode == 0 )
				{
					if( bHyperlink )
					{
						//sprintf( szColorText, "|Hnpc:%d|h|cff0022ee[%s]|r|h", iGUID, pObj->Name );
						sprintf( szColorText, "|Hnpc:%d|h|c%08x[%s]|r|h", iGUID, iColor_NPC, pObj->Name );
					}
					else
					{
						//sprintf( szColorText, "|cff0022ee%s|r", pObj->Name );
						sprintf( szColorText, "|c%08x%s|r", iColor_NPC, pObj->Name );
					}
				}
				else
				{
					if( bHyperlink )
					{
						//sprintf( szColorText, "|Hnpc:%d|h|cff770dd0[%s]|r|h", iGUID, pObj->Name );
						sprintf( szColorText, "|Hnpc:%d|h|c%08x[%s]|r|h", iGUID, iColor_NPC, pObj->Name );
					}
					else
					{
						//sprintf( szColorText, "|cff770dd0%s|r", pObj->Name );
						sprintf( szColorText, "|c%08x%s|r", iColor_NPC, pObj->Name );
					}
				}
			}
			else if( iGUID >= 490000 && iGUID < 500000 )
			{
				if( bHyperlink )
				{
					string strlink;
					if( SkillGUID_To_HyperLink( iGUID, 0, strlink ) )
					{
						strcpy( szColorText , strlink.c_str() );
					}
					else
					{
						sprintf( szColorText, "|c%08x%s|r", iColor_Item, pObj->Name );
					}
				}
				else
				{
					sprintf( szColorText, "|c%08x%s|r", iColor_Item, pObj->Name );
				}

			}
			else					
			{						
				// 其於的都當物品
				//sprintf( szColorText, "|cff770dd0%s|r", pObj->Name );

				if( bHyperlink )
				{
					//link = Com_FormatString( "|Hitem:%x|h|o%08x[%s]|r|h", iGUID, color, pObj->Name ); 
					sprintf( szColorText, "|Hitem:%x|h|c%08x[%s]|r|h", iGUID, iColor_Item, pObj->Name );


					//if( ItemGUID_To_ItemLink( iGUID, link ) )
					//{
					//	strcpy( szColorText , link.c_str() );
					//}
					//else
					//{
					//	//sprintf( szColorText, "|cff770dd0%s|r", pObj->Name );
					//	//int color = g_ObjectData->GetItemQualityColor( g_ObjectData->GetItemQuality( pObj, NULL ) );
					//	//sprintf( szColorText, "|c%08x%s|r", color, pObj->Name );
					//	sprintf( szColorText, "|c%08x%s|r", color, pObj->Name );

					//}

				}
				else
				{					
					//int color = g_ObjectData->GetItemQualityColor( g_ObjectData->GetItemQuality( pObj, NULL ) );
					sprintf( szColorText, "|c%08x%s|r", iColor_Item, pObj->Name );
				}
			}

			pSource.replace( iBegin, ( strlen( szKeyWord ) + 2 ), szColorText );

			off = iBegin + strlen( szColorText ) ;
		}
		else
		{
			if( strncmp( szKeyWord, "ZONE_", strlen( "ZONE_" ) ) == 0 )
			{
				sprintf( szColorText, "|c%08x%s|r", iColor_Zone, g_ObjectData->GetString( szKeyWord ) );
				pSource.replace( iBegin, ( strlen( szKeyWord ) + 2 ), szColorText );
				off = iBegin + strlen( szColorText ) ;
			}
			else
			{
				break;
			}
		}
	}

	if( m_bInitLuaColorKeyWord == false )
	{
		m_bInitLuaColorKeyWord = true;

		lua_State*	L		= GetLuaStateWorld();
		int			iStack	= lua_gettop (L);
		bool		bReturn = false;

		if ( L )
		{		
			lua_getglobal( L, "LuaInit_RegColor" );

			if (!lua_isnil( L, -1))
			{
				if (lua_pcall( L, 0, 0, 0) != 0)
				{
					const char* err = lua_tostring( L, -1);
					UiOuputError(err);
				}
			}
		}
	}

	for( map< string, string >::iterator it = m_mapColorKeyWord.begin(); it != m_mapColorKeyWord.end(); it++ )
	{
		string	sKey		= it->first;
		string	sReplace	= it->second;


		size_t	iBegin;
		while ( (iBegin = pSource.find( sKey )) != string::npos ) 
		{
			pSource.replace( iBegin, sKey.size(), sReplace );
		}
	}

	return pSource;
	*/

}

//--------------------------------------------------------------------------------------
void CUI_QuestDetail::InitKeyWorld()
{
	lua_State*	L		= GetLuaStateWorld();
	int			iStack	= lua_gettop (L);
	bool		bReturn = false;

	if ( L )
	{		
		lua_getglobal( L, "LuaInit_RegColor" );

		if (!lua_isnil( L, -1))
		{
			if (lua_pcall( L, 0, 0, 0) != 0)
			{
				const char* err = lua_tostring( L, -1);
				UiOuputError(err);
			}
		}
	}
}
//--------------------------------------------------------------------------------------
int			LuaFunc_AcceptQuest	( lua_State *L )
{
	if( g_pQuestDetail->m_iMode == 0 )	
	{
		// From NPC
		if( g_pQuestDetail->m_iStatus == 0 )
		{

			RoleDataEx*		pRole	= CNetGlobal::RoleData();
			if( pRole->PlayerBaseData->PlayTimeQuota < 2*60*60 )
			{
				g_pGameMain->SendSystemMsg(g_ObjectData->GetString( "BTI_MSG_NOTHEALTH" ));
				g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString( "BTI_MSG_NOTHEALTH" ));
				return 0;
			}



			CNetCli_Script::AcceptQuest( g_pQuestList->GetQuestNPC(), g_pQuestDetail->m_iQuestID ) ;		
			lua_pushnumber( L, 1 );
			return 1;
		}
	}

	return 0;
}
//--------------------------------------------------------------------------------------
int			LuaFunc_DeleteQuest	( lua_State *L )
{
	// From NPC
	RoleDataEx*		pRole	= CNetGlobal::RoleData();
	int				iField	= pRole->CheckQuestList_IsGotQuest( g_pQuestDetail->m_iQuestID );

	if( iField != -1 )
		CNetCli_Script::DeleteQuest( iField, g_pQuestDetail->m_iQuestID );

	// 檢查是不是有快顯
	if( g_pQuestBook->m_setTrackID.find( g_pQuestDetail->m_iQuestID ) != g_pQuestBook->m_setTrackID.end() )
	{
		g_pQuestBook->m_setTrackID.erase( g_pQuestDetail->m_iQuestID );
		g_pInterface->SendWorldEvent( UiEvent_ResetQuestTrack );
	}



	return 0;
}
//--------------------------------------------------------------------------------------
int			LuaFunc_CompleteQuest	( lua_State *L )
{
	RoleDataEx*		pRole	= CNetGlobal::RoleData();

	if( g_pQuestDetail->m_iMode == 0 && g_pQuestDetail->m_iStatus == 2 )
	{
		
		if( pRole->PlayerBaseData->PlayTimeQuota < 2*60*60 )
		{
			g_pGameMain->SendSystemMsg(g_ObjectData->GetString( "BTI_MSG_NOTHEALTH" ));
			g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString( "BTI_MSG_NOTHEALTH" ));
			return 0;
		}

		GameObjDbStruct*	pObj		= g_ObjectData->GetObj( g_pQuestDetail->m_iQuestID );

		if( pObj == NULL )
			return 0;

		GameObjectQuestDetail* pQuest	= &( pObj->QuestDetail );


		// 檢查等級差, 至少目前主職的等級要大於回報任務的最低顯示
		{
		
			int iLowerLV = pQuest->iCheck_LV - 10;
			
			if( iLowerLV <= 0 )
			{
				iLowerLV = 1;
			}

			if( pRole->TempData.Attr.Level < iLowerLV )
			{
				g_pGameMain->SendSystemMsgEx(g_ObjectData->GetString( "QUEST_MSG_COMPLETE_LVTOOLOW" ), iLowerLV );
				g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString( "QUEST_MSG_COMPLETE_LVTOOLOW" ), iLowerLV );
				return 0;
			}		
		}

		// 檢查物品是否足夠, 不足夠的話, 若銀行有滿, 則秀訊息
		for( int index = 0; index < _MAX_SIZE_QUEST_CONDITION; index++ )
		{
			if( pQuest->iRequest_ItemID[ index ] != 0 )
			{
				if( pRole->Sc_CheckItemFromBody( pQuest->iRequest_ItemID[ index ], pQuest->iRequest_ItemVal[ index ] ) == false )
				{			
					g_pGameMain->SendSystemMsg(g_ObjectData->GetString( "QUEST_MSG_REQITEM_INBANK" ) );
					g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString( "QUEST_MSG_REQITEM_INBANK" ) );
					return 0;
				}
			}
		}


		// From NPC
		if( g_pQuestDetail->m_iRewardItemChoiceID == -1 || g_pQuestDetail->m_iRewardItemChoiceID != 0 )
		{

			// 檢查選則物品的合理性
			if( g_pQuestDetail->m_iRewardItemChoiceID != -1 )
			{
				int	iNumItem	= (int)g_pQuestDetail->m_vecQuestItem[1].size();

				if( iNumItem < g_pQuestDetail->m_iRewardItemChoiceID && g_pQuestDetail->m_iRewardItemChoiceID < 0 )
				{
					return 0;
				}
			}


			// 檢查是不是有快顯
			if( g_pQuestBook->m_setTrackID.find( g_pQuestDetail->m_iQuestID ) != g_pQuestBook->m_setTrackID.end() )
			{
				g_pQuestBook->m_setTrackID.erase( g_pQuestDetail->m_iQuestID );
				g_pInterface->SendWorldEvent( UiEvent_ResetQuestTrack );
			}

			// 發出封包給 Server
			CNetCli_Script::CompleteQuest( g_pQuestList->GetQuestNPC(), g_pQuestDetail->m_iQuestID, ( g_pQuestDetail->m_iRewardItemChoiceID - 1 ) );
			lua_pushnumber( L, 1 );
			return 1;
		}
		else
		{
			//g_pGameMain->SendWarningMsg( g_ObjectData->GetString( "QUEST_MSG_CHOICEITEM" ) );
			//叫玩家選ITEM
			g_pInterface->SendWorldEvent( UiEvent_SelectRewardItem );
		}
	}

	return 0;
}
//--------------------------------------------------------------------------------------
int			LuaFunc_ProcessQuest	( lua_State *L )
{
	if( g_pQuestDetail )
	{
		if( g_pQuestDetail->m_iMode == 0 )
		{
			// From NPC
			if( g_pQuestDetail->m_iStatus == 0 )
				CNetCli_Script::AcceptQuest( g_pQuestList->GetQuestNPC(), g_pQuestDetail->m_iQuestID ) ;
			else
			{
				if( g_pQuestDetail->m_iRewardItemChoiceID != -1 )
					CNetCli_Script::CompleteQuest( g_pQuestList->GetQuestNPC(), g_pQuestDetail->m_iQuestID, g_pQuestDetail->m_iRewardItemChoiceID );
				else
				{
					lua_pushnumber( L, -1 );
					return 1;
				}
			}
		}
		else
		{
			// From Book
			// Search Role Struct for field
			RoleDataEx*		pRole	= CNetGlobal::RoleData();
			int				iField	= pRole->CheckQuestList_IsGotQuest( g_pQuestDetail->m_iQuestID );

			CNetCli_Script::DeleteQuest( iField, g_pQuestDetail->m_iQuestID );
		}
		lua_pushnumber( L, 1 );
		return 1;
	}

	return 0;
}
//--------------------------------------------------------------------------------------
int	LuaFunc_IsQuestComplete_QuestDetail	( lua_State *L )
{
	if( g_pQuestDetail )
	{





	}

	return 0;
}
//--------------------------------------------------------------------------------------
int	LuaFunc_GetQuestStatus_QuestDetail ( lua_State *L )
{
	if( g_pQuestDetail )
	{
		lua_pushnumber( L, g_pQuestDetail->m_iStatus );
		return 1;
	}
	return 0;
}
//--------------------------------------------------------------------------------------
int LuaFunc_SetChoiceItem_QuestDetail ( lua_State *L )
{
	if( g_pQuestDetail )
	{
		int iIndex		= luaL_checkint( L, 1 );
		int	iNumItem	= (int)g_pQuestDetail->m_vecQuestItem[1].size();

		if( iNumItem >= iIndex && iIndex > 0 )
		{
			if( g_pQuestDetail->m_iRewardItemChoiceID == iIndex )
			{
				g_pQuestDetail->m_iRewardItemChoiceID = 0;							
			}
			else
				g_pQuestDetail->m_iRewardItemChoiceID = iIndex;			
		}
	}
	return 0;
}
//--------------------------------------------------------------------------------------
int LuaFunc_GetChoiceItem_QuestDetail ( lua_State *L )
{
	if( g_pQuestDetail )
	{
		lua_pushnumber( L, g_pQuestDetail->m_iRewardItemChoiceID );
		return 1;
	}
	return 0;
}
//--------------------------------------------------------------------------------------
int	LuaFunc_GetQuestRequest ( lua_State *L )
{
	int					iQuestIndex	= luaL_checkint( L, 1 );
	int					iIndex		= luaL_checkint( L, 2 );

	//GameObjDbStruct*	pObj		= g_ObjectData->GetObj( g_pQuestDetail->m_iQuestID );
	int					iQuestGUID	= g_pQuestBook->GetQuestGUID( ( iQuestIndex - 1 ) );
	GameObjDbStructEx*	pObj		= g_ObjectData->GetObj( iQuestGUID );

	RoleDataEx*			pRole		= CNetGlobal::RoleData();
	//char				szStr[256];
	//char				szKeyWord[256];
	//char				szPreString[256];


	if( pObj )
	{
		if( iIndex == -1 )
		{
			int iCount = g_pQuestDetail->GetQuestRequest( iQuestGUID );
			lua_pushnumber( L, iCount );
			return 1;
		}
		else
			if( iIndex == -2 )	// 任務名稱
			{
				lua_pushstring( L, pObj->GetName() );
				return 1;
			}
			else
				if( iIndex == -3 ) // 任務等級
				{
					lua_pushnumber( L, pObj->QuestDetail.iCheck_LV );
					return 1;
				}
				else
				{

					// 由於有兩個檢查表, 所以這邊的設計比較特殊, 介面輸入的 index 要由 count 計算個數, 找出實際對應的位置

					// 檢查需求物品
					int					iResult		= 0;
					int					iGUID		= 0;
					GameObjDbStructEx*	pRequest	= NULL;
					int					iCount		= 0;

					iIndex--;

					// 檢查擊殺目標
					for( int i = 0; i < _MAX_SIZE_QUEST_CONDITION; i++ )
					{
						iGUID		= pObj->QuestDetail.iRequest_KillID[ i ];
						pRequest	= g_ObjectData->GetObj( iGUID );

						//if( pRequest->
						if( pRequest == NULL )
							break;

						if( iCount != iIndex )
						{
							iCount++;
							continue;
						}

						// 組合字串
						int iKillVal	= 0;
						int iQuestField = pRole->CheckQuestList_IsGotQuest( iQuestGUID );
						if( iQuestField == -1 )
							return 0;
						else
						{
							//iKillVal = pRole->PlayerBaseData->Quest.iQuestKillCount[ iQuestField ][ i ];
							iKillVal = pRole->PlayerBaseData->Quest.QuestLog[iQuestField].iQuestKillCount[i];
						}

						//Q_REQUEST_KILL_FORMAT <type><monster> <count>/<max><complete>
						string strTemp = g_ObjectData->GetString( "Q_REQUEST_KILL_FORMAT" );

						// 檢查有無前置字串
						if( pObj->QuestDetail.iRequest_KillStr[ i ] != 0 )
						{
							string strType = g_ObjectData->GetString( Com_FormatString( "Q_REQUEST_KILLSTR_%02d", pObj->QuestDetail.iRequest_KillStr[ i ] ) );
		   					strTemp = ReplaceKeyString( strTemp, "<type>", strType );
						}
						else
						{
							strTemp = ReplaceKeyString( strTemp, "<type>", "" );
						}

						//判斷複數
						string strName;
						if( pObj->QuestDetail.iRequest_KillVal[ i ] > 1 )
						{
							strName = pRequest->GetPluralName();
						}
						else
						{
							strName = pRequest->GetName();
						}

 						strTemp = ReplaceKeyString( strTemp, "<monster>", strName );
						strTemp = ReplaceKeyString( strTemp, "<count>", iKillVal );
						strTemp = ReplaceKeyString( strTemp, "<max>", pObj->QuestDetail.iRequest_KillVal[ i ] );

						if( iKillVal >= pObj->QuestDetail.iRequest_KillVal[ i ] )
						{
							iResult		= 1;
							strTemp = ReplaceKeyString( strTemp, "<complete>", g_ObjectData->GetString( "QUEST_MSG_REQUEST_COMPLETE" ) );
						}
						else
						{
							strTemp = ReplaceKeyString( strTemp, "<complete>", "" );
						}

						lua_pushstring( L, strTemp.c_str() );
						lua_pushnumber( L, iResult );
						return 2;
					}

					// 檢查物品需求
					for( int i = 0; i < _MAX_SIZE_QUEST_CONDITION; i++ )
					{
						iGUID		= pObj->QuestDetail.iRequest_ItemID[ i ];
						pRequest	= g_ObjectData->GetObj( iGUID );

						if( pRequest == NULL )
							break;

						// 檢查是否跳過
						if( pRequest->Classification == EM_ObjectClass_KeyItem )
						{
							if( pRequest->KeyItem.Type == EM_KeyItemType_Invisible )
							{
								continue;
							}
						}					


						if( iCount != iIndex )
						{
							iCount++;
							continue;
						}

						// 拼湊並回傳字串
						//pObj->QuestDetail.iRequest_ItemID[ i ]	

						string	strTemp;
						int		iCount = 0, iMaxCount = 0;

						switch( pRequest->Classification )
						{
						case EM_ObjectClass_Item:	// 身上要有指定物品
							{
								//Q_REQUEST_ITEM_FORMAT <type><item> <count>/<max><complete>
								strTemp = g_ObjectData->GetString( "Q_REQUEST_ITEM_FORMAT" );

								// 檢查有無前置字串
								if( pObj->QuestDetail.iRequest_ItemStr[ i ] != 0 )
								{
									string strType = g_ObjectData->GetString( Com_FormatString( "Q_REQUEST_ITEMSTR_%02d", pObj->QuestDetail.iRequest_ItemStr[ i ] ) );
									strTemp = ReplaceKeyString( strTemp, "<type>", strType );
								}
								else
								{
									strTemp = ReplaceKeyString( strTemp, "<type>", "" );
								}

						 		// 取出字串
								int iNumCollect = pRole->CalBankItemCount( iGUID ) + pRole->CalBodyItemCount( iGUID );
								iCount			= iNumCollect;
								iMaxCount		= pObj->QuestDetail.iRequest_ItemVal[ i ];
				
								//判斷複數
								string strName;
								if( pObj->QuestDetail.iRequest_ItemVal[ i ] > 1 )
								{
									strName = pRequest->GetPluralName();
								}
								else
								{
									strName = pRequest->GetName();
								}

								strTemp = ReplaceKeyString( strTemp, "<item>", strName );

	   		  
								if( iNumCollect >= pObj->QuestDetail.iRequest_ItemVal[ i ] )
								{
									iNumCollect = pObj->QuestDetail.iRequest_ItemVal[ i ];
									iResult		= 1;	// 條件滿足
									strTemp = ReplaceKeyString( strTemp, "<complete>", g_ObjectData->GetString( "QUEST_MSG_REQUEST_COMPLETE" ) );
								}
								else
								{
									strTemp = ReplaceKeyString( strTemp, "<complete>", "" );
								}

								strTemp = ReplaceKeyString( strTemp, "<count>", iNumCollect );
								strTemp = ReplaceKeyString( strTemp, "<max>", pObj->QuestDetail.iRequest_ItemVal[ i ] );

								//lua_pushstring( L, strTemp.c_str() );
								//lua_pushnumber( L, iResult );
								//return 2;

							} break;
						case EM_ObjectClass_KeyItem:
							{
								//Q_REQUEST_KEYITEM_FORMAT   <type><item> <complete>
								strTemp = g_ObjectData->GetString( "Q_REQUEST_KEYITEM_FORMAT" );
								// 檢查有無前置字串
								if( pObj->QuestDetail.iRequest_ItemStr[ i ] != 0 )
								{
									string strType = g_ObjectData->GetString( Com_FormatString( "Q_REQUEST_ITEMSTR_%02d", pObj->QuestDetail.iRequest_ItemStr[ i ] ) );
									strTemp = ReplaceKeyString( strTemp, "<type>", strType );
								}
								else
								{
									strTemp = ReplaceKeyString( strTemp, "<type>", "" );
								}

  								strTemp = ReplaceKeyString( strTemp, "<item>", pRequest->GetName() );


								if( pRole->CheckKeyItem( iGUID ) == true )
								{
									iCount	= 1;
									iResult = 1;
									//sprintf( szStr, "%s (完成)", pRequest->Name );
									strTemp = ReplaceKeyString( strTemp, "<complete>", g_ObjectData->GetString( "QUEST_MSG_REQUEST_COMPLETE" ) );
								}
								else
								{
									//sprintf( szStr, "%s" );
									iCount	= 0;
									strTemp = ReplaceKeyString( strTemp, "<complete>", "" );
								}

								iMaxCount = 1;


							} break;
						}

						if( strlen( pObj->QuestDetail.szScript_RequestString ) != 0 )
						{
							strTemp = g_pQuestDetail->GetLuaStringResult( pObj->QuestDetail.szScript_RequestString, iQuestGUID, iGUID, iCount, iMaxCount, iResult );
						}

						lua_pushstring( L, strTemp.c_str() );
						lua_pushnumber( L, iResult );
						return 2;
					}
				}

	}

	return 0;
}
//--------------------------------------------------------------------------------------
string CUI_QuestDetail::GetLuaStringResult	( const char* pszScript, int iQuestID, int iItemGUID, int iCount, int iMaxCount, int iResult )
{
	lua_State*	L			= GetLuaStateWorld();
	int			iStack		= lua_gettop (L);

	string		str;

	if ( L )
	{		
		lua_getglobal( L, pszScript );

		lua_pushnumber( L, iQuestID );
		lua_pushnumber( L, iItemGUID );
		lua_pushnumber( L, iCount );
		lua_pushnumber( L, iMaxCount );
		lua_pushnumber( L, iResult );

		if (!lua_isnil( L, -1))
		{
			if (lua_pcall( L, 5, 1, 0) != 0)
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
					str = lua_tostring(L,-1);
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
void CUI_QuestDetail::GetItemQueue	( int iItemID, int iCount )	// 當系統得到新物品時產生的事件
{
	GameObjDbStructEx*	pItemObj	= g_ObjectData->GetObj( iItemID );

	if( pItemObj == NULL )
		return;

	RoleDataEx*			pRole		= CNetGlobal::RoleData();

	if( pItemObj == NULL )
		return;

	bool				bKeyItem	= pItemObj->IsKeyItem();

	// 進去每個任務檢查, 看得到的物品是否為任務物品
	for( int i = 0; i < _MAX_QUEST_COUNT_; i++ )
	{
		int iQuestID = pRole->PlayerBaseData->Quest.QuestLog[ i ].iQuestID;
		if( iQuestID != 0 )
		{

			GameObjDbStructEx*		pObj	= g_ObjectData->GetObj( iQuestID );
			GameObjectQuestDetail*	pQuest	= NULL;

			if( pObj )
			{
				pQuest = &( pObj->QuestDetail );

				for( int j = 0; j < _MAX_SIZE_QUEST_CONDITION; j++ )
				{
					if( pQuest->iRequest_ItemID[ j ] == iItemID )
					{
						int iNum			= 0;
						int iNumBase		= 0;

						if( bKeyItem == false )
						{
							//iNum		= pRole->Sc_GetItemNumFromRole( iItemID ) + iCount;
							iNum		= pRole->Sc_GetItemNumFromRole( iItemID );
							iNumBase	= iNum - iCount;
						}

						if( iNumBase >= pQuest->iRequest_ItemVal[ j ] && bKeyItem == false )
						{
							break; // 原本就已達成完成了, 不處理, 跳出該任務
						}
						else
						{
							lua_State* L = g_pGameMain->GetInterface()->GetLuaState();
							lua_pushnumber( L, iQuestID);	
							lua_setglobal( L, "arg1" );
							g_pInterface->SendWorldEvent( UiEvent_CheckGetItemBeQueueItem );

							if( bKeyItem == false )
							{
								int iMax = min(iNum, pQuest->iRequest_ItemVal[j]);

								// 秀出得到任務物品的訊息
								//const char* fmt = g_ObjectData->GetString("QUEST_MSG_GET_ITEM");
								//g_pGameMain->SendSystemMsgEx(fmt, pObj->GetName(), pItemObj->GetName(), iMax, pQuest->iRequest_ItemVal[j]);

								// 秀出得到任務物品的訊息
								//QUEST_MSG_GET_ITEM_FORMAT <quest>:<type><item> <count>/<max> <complete>
								string strTemp = g_ObjectData->GetString( "QUEST_MSG_GET_ITEM_FORMAT" );

								strTemp = ReplaceKeyString( strTemp, "<quest>", pObj->GetName() );

								// 檢查有無前置字串
								if( pQuest->iRequest_ItemStr[ j ] != 0 )
								{
									string strType = g_ObjectData->GetString( Com_FormatString( "Q_REQUEST_ITEMSTR_%02d", pQuest->iRequest_ItemStr[ j ] ) );
									strTemp = ReplaceKeyString( strTemp, "<type>", strType );
								}
								else
								{
									strTemp = ReplaceKeyString( strTemp, "<type>", "" );
								}

								strTemp = ReplaceKeyString( strTemp, "<item>", pItemObj->GetName() );

								strTemp = ReplaceKeyString( strTemp, "<count>", iMax );
								strTemp = ReplaceKeyString( strTemp, "<max>", pQuest->iRequest_ItemVal[ j ] );

								// 附加完成
								if( iNum == pQuest->iRequest_ItemVal[ j ] )
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

							}
							else
							{



								//char szMsg[256];
								//sprintf( szMsg, "%s%s", pItemObj->GetName(), g_ObjectData->GetString("QUEST_MSG_REQUEST_COMPLETE") );
								//g_pGameMain->SendSystemMsg(szMsg);
								//g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", szMsg);


								// 秀出得到任務物品的訊息
								//QUEST_MSG_GET_KEYITEM_FORMAT <quest>:<type><item> <count>/<max> <complete>
								string strTemp = g_ObjectData->GetString( "QUEST_MSG_GET_KEYITEM_FORMAT" );

								strTemp = ReplaceKeyString( strTemp, "<quest>", pObj->GetName() );

								// 檢查有無前置字串
								if( pQuest->iRequest_ItemStr[ j ] != 0 )
								{
									string strType = g_ObjectData->GetString( Com_FormatString( "Q_REQUEST_ITEMSTR_%02d", pQuest->iRequest_ItemStr[ j ] ) );
									strTemp = ReplaceKeyString( strTemp, "<type>", strType );
								}
								else
								{
									strTemp = ReplaceKeyString( strTemp, "<type>", "" );
								}

								strTemp = ReplaceKeyString( strTemp, "<item>", pItemObj->GetName() );

								char temp[256];
								sprintf(temp, "(%s)", g_ObjectData->GetString("QUEST_S_REQUEST_COMPLETE") );
								strTemp = ReplaceKeyString( strTemp, "<complete>", temp );

								g_pGameMain->SendSystemMsg( strTemp.c_str() );
								g_pGameMain->SendSystemChatMsg( strTemp.c_str() );

							}


							// 已達滿足條件, 檢查是否完成該任務
							if( pRole->CheckQuest_IsComplete( i, iQuestID, pQuest ) == EM_QUESTCHECK_RESULT_OK )
							{
								// 任務完成
								// 訊息顯示
								char szMsg[256];
								sprintf(szMsg, g_ObjectData->GetString("QUEST_MSG_CONDITION_FINISHED"), pObj->GetName());
								g_pGameMain->SendSystemMsg(szMsg);
								g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", szMsg);
								break;
							}
						}						
					}
				}
			}
		}
	}


}
//--------------------------------------------------------------------------------------
int	LuaFunc_RegColorKeyWord ( lua_State *L )
{
	const char* pszKeyWord	= luaL_checkstring( L, 1 );
	const char* color		= luaL_checkstring( L, 2 );

	// 重複定義以新的為準(一般發生在reload)
	map<string, string>::iterator iter = g_pQuestDetail->m_mapColorKeyWord.find(pszKeyWord);
	if ( iter != g_pQuestDetail->m_mapColorKeyWord.end() ) {
		iter->second = color;
	} else {
		g_pQuestDetail->m_mapColorKeyWord[pszKeyWord] = color;
	}

	return 0;
}

int	LuaFunc_GetNewbieQuestGuide ( lua_State *L )
{
	//如果正在進行新手交學就傳 1, 否則 0
	lua_pushnumber( L, 0 );
	return 1;
}

int LuaFunc_QuestDetail_GetQuestNPC( lua_State *L )
{

	if( g_pQuestDetail->m_iQuestNPC <= 0 )
	{
		return 0;
	}

	lua_pushnumber( L, g_pQuestDetail->m_iQuestNPC );
	return 1;
}

int LuaFunc_QuestDetail_GetRequestQuestNPC(lua_State *L )
{
	if( g_pQuestDetail->m_iRequestQuestNPC <= 0 )
	{
		return 0;
	}

	lua_pushnumber( L, g_pQuestDetail->m_iRequestQuestNPC );
	return 1;
}