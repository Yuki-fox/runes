#include "../../mainproc/GameMain.h"
//#include "../../netwaker_member/NetWakerClientInc.h"
//#include "../../netwaker_member/NetWakerClientInc.h"
#include "../../netwaker_member/Net_Script/NetCli_Script_Child.h"
#include "../../netwaker_member/net_other/NetCli_OtherChild.h"
//#include "../../Roledata/RoleData.h"
#include "UI_QuestDetail.h"
#include "UI_QuestList.h"
#include "UI_Questbook.h"


//#include "../../netmember/NetScript.h"

// 全域宣告
//-------------------------------------------------------------------
//CUI_QuestNPCList			*g_pUi_QuestNPCList = NULL;
/*
CRoleSprite*			CUI_QuestList::m_InteractNPC;

CButton*				CUI_QuestList::m_pQuestSelectButton		[ 10 ];
//CButton*				CUI_QuestList::m_pAvailableQuestButton		[ 5 ];
//CButton*				CUI_QuestList::m_pCurrentQuestButton		[ 5 ];


CFontString*			CUI_QuestList::m_pPanelTitle;
CFontString*			CUI_QuestList::m_pNPC_Text;				//QuestListFrame_NPC_GreetingText

int						CUI_QuestList::m_iQuestIndex = 0;

CLayout*				CUI_QuestList::m_pQuestNPCList = NULL;

vector< int >			CUI_QuestList::m_vecQuestID;
vector< int >			CUI_QuestList::m_vecQuestMode;
*/

CUI_QuestList*			g_pQuestList = NULL;
//-------------------------------------------------------------------
int			LuaFunc_OnLoad_QuestList		( lua_State *L );
int			LuaFunc_OnClick_QuestListButton	( lua_State *L );
int			LuaFunc_GetSpeakDetail			( lua_State *L );
int			LuaFunc_GetNumQuest				( lua_State *L );	
int			LuaFunc_GetQuestNameByIndex		( lua_State *L );	// 傳入 _Type, _Index 
int			LuaFunc_GetSpeakTitle			( lua_State *L );
int			LuaFunc_GetSpeakObjName			( lua_State *L );	

int			LuaFunc_GetSpeakDetail			( lua_State *L );
int			LuaFunc_GetNumSpeakOption		( lua_State *L );
int			LuaFunc_GetSpeakOption			( lua_State *L );

int			LuaFunc_ChoiceOption			( lua_State *L );
int			LuaFunc_ChoiceListDialogOption	( lua_State *L );

//-------------------------------------------------------------------
CUI_QuestList::CUI_QuestList(CInterface* object) : CInterfaceBase(object)
{
	g_pQuestList = this;
}
// --------------------------------------------------------------------------------------
CUI_QuestList::~CUI_QuestList(void)
{
	g_pQuestList = NULL;
}
//--------------------------------------------------------------------------------------
void CUI_QuestList::RegisterFunc(void)
{
	lua_State* l = GetLuaStateWorld();
	if (l)
	{
		lua_register( l, "OnLoad_QuestList",			LuaFunc_OnLoad_QuestList		);
		lua_register( l, "OnClick_QuestListButton",		LuaFunc_OnClick_QuestListButton	);
		lua_register( l, "GetSpeakDetail",				LuaFunc_GetSpeakDetail			);
		lua_register( l, "GetSpeakObjName",				LuaFunc_GetSpeakObjName			);
		lua_register( l, "GetNumQuest",					LuaFunc_GetNumQuest				);
		lua_register( l, "GetQuestNameByIndex",			LuaFunc_GetQuestNameByIndex		);

		//lua_register( l, "GetSpeakDetail",				LuaFunc_GetSpeakDetail			);
		lua_register( l, "GetNumSpeakOption",			LuaFunc_GetNumSpeakOption		);
		lua_register( l, "GetSpeakOption",				LuaFunc_GetSpeakOption			);
		lua_register( l, "ChoiceOption",				LuaFunc_ChoiceOption			);
		lua_register( l, "ChoiceListDialogOption",		LuaFunc_ChoiceListDialogOption	);

		lua_register( l, "GetSpeakTitle",				LuaFunc_GetSpeakTitle			);
		
	}
}
//--------------------------------------------------------------------------------------
void CUI_QuestList::Update(float elapsedTime)
{
	RoleDataEx*			pRole		= CNetGlobal::RoleData();
//	CRoleSprite*		pRoleObj	= g_pGameMain->FindSpriteByID( pRole->PlayerTempData->SpeakInfo.iSpeakNPCID );
	int					id			= pRole->PlayerTempData->SpeakInfo.iSpeakNPCID;

	if ( id > 0 ) 
	{
		// 檢查是否需要關閉
		CRoleSprite* sprite = g_pGameMain->FindSpriteByID(id);
		if ( sprite == NULL || g_pGameMain->GetDistanceWithPlayer(sprite) > sprite->GetClientTouchRange() + 5.0f ) {
			//m_iQuestNPC = 0;
			pRole->PlayerTempData->SpeakInfo.iSpeakNPCID = 0;
			SendWorldEvent("CLOSE_SPEAKFRAME");
		}		
	}
}
//--------------------------------------------------------------------------------------
void CUI_QuestList::Clear ()
{
	// 清除之前資料
	for( int i=0; i<=2; i++ )
		m_vecQuestID[i].clear();

	m_iQuestNPC = 0;
	m_sText.clear();
}
//--------------------------------------------------------------------------------------
void CUI_QuestList::SetQuestNPC ( int iObjID )
{
	CRoleSprite*		pRoleObj	= g_pGameMain->FindSpriteByID( iObjID );
	if ( pRoleObj == NULL )
		return;

	int					iQuestNPCID = pRoleObj->GetObjectId();
	GameObjDbStruct*	pObj		= g_ObjectData->GetObj( iQuestNPCID );
	if ( pObj == NULL )
		return;

	RoleDataEx*			pRole		= CNetGlobal::RoleData();
	int					iSpeaking	= 0;

	if( pRoleObj)

	// 清除設定
		Clear();

	// 設定 NPC
		m_iQuestNPC	= iObjID;

	// 加入 NPC 說話的內容
	//------------------------------------------------------------------------------
		//m_sText = pObj->NPC.szQuestNpcText;
		pRole->PlayerTempData->SpeakInfo.iSpeakNPCID	= iObjID;
		//pRole->PlayerTempData->SpeakInfo.SpeakDetail	= pObj->NPC.szQuestNpcText;

		if( pObj->NPC.szQuestNpcText != NULL )
			((CNetCli_Script_Child*)CNetCli_Script_Child::m_pThis)->m_SpeakDetail = pObj->NPC.szQuestNpcText;
		else
			((CNetCli_Script_Child*)CNetCli_Script_Child::m_pThis)->m_SpeakDetail = "";

	// 加入 NPC 能給的 QUEST
	//------------------------------------------------------------------------------
		for( int index=0; index < _MAX_NPC_QUEST_COUNT_; index++ )
		{
			int iQuestID = pObj->NPC.iQuestID[ index ];

			if( iQuestID != 0 )
			{	

				// 檢查角色是否已答接任務的條件
					GameObjDbStruct			*pObj	= g_ObjectData->GetObj( iQuestID );

					if( pObj )
					{
						GameObjectQuestDetail	*pQuest = &( pObj->QuestDetail );

						// 檢查角色是否已完成任務					
							if( pRole->CheckQuestHistory_IsComplete( iQuestID ) == true )
							{
								if( pQuest->iCheck_Loop == false )
									continue;	// 跳過這個 Quest
							}

						// 檢查角色是否擁有該 Quest, 及是否完成
							int iQuestField = pRole->CheckQuestList_IsGotQuest( iQuestID );

							if( iQuestField != -1 )
							{								

								// 檢查該任務是否為指定另一NPC完成.若是則該 NPC 即不顯示該任務
									if( pQuest->iRequest_QuestID != 0 && pQuest->iRequest_QuestID != iQuestNPCID )
									{
										continue;
									}

									// 檢查是否已完成該 Quest
									if( pRole->CheckQuest_IsComplete( iQuestField, iQuestID, pQuest ) == EM_QUESTCHECK_RESULT_OK )
									{
										// 已完成
										m_vecQuestID[2].push_back( iQuestID );
										iSpeaking = 1;	//  設定對話為已完成
									}
									else
									{
										// 尚未完成
										m_vecQuestID[1].push_back( iQuestID );
									}

								continue;
							}
		
						// 檢查角色是否可以接該任務 ( 呼略等級 )
							if( pRole->CheckQuest_IsAvailable( iQuestID, pQuest, true ) != EM_QUESTCHECK_RESULT_OK )
								continue;
							else
							{
								bool bLuaCheck = true;
								// 檢查有無 LUA QUESTBEGIN
								if( strlen( pQuest->szScript_OnBegin ) != 0 )
								{
									bLuaCheck = g_pQuestBook->CheckQuestBeginLua( pQuest->szScript_OnBegin );
								}

								// 可接任務
								if( bLuaCheck )
									m_vecQuestID[0].push_back( iQuestID );
							}
					}
			}
		}	// end of for loop

		// 設定 NPC 對話
		//------------------------------------------------------------------------------
		if( iSpeaking != 0 && strlen( pObj->NPC.szQuestDoingText ) != 0 )
		{
			// 未完成對話
			//pRole->PlayerTempData->SpeakInfo.SpeakDetail = pObj->NPC.szQuestDoingText;
			((CNetCli_Script_Child*)CNetCli_Script_Child::m_pThis)->m_SpeakDetail = pObj->NPC.szQuestDoingText;

		}
		else
		{			
			// 正常對話
			//pRole->PlayerTempData->SpeakInfo.SpeakDetail = pObj->NPC.szQuestNpcText;
			if( pObj->NPC.szQuestNpcText != NULL )
				((CNetCli_Script_Child*)CNetCli_Script_Child::m_pThis)->m_SpeakDetail = pObj->NPC.szQuestNpcText;
			else
				((CNetCli_Script_Child*)CNetCli_Script_Child::m_pThis)->m_SpeakDetail = "";

		}

	// 檢查該 NPC 是否是指定完成任務 NPC
	// 檢查 Client 身上的 Quest 有無另指定 NPC 完成
	//------------------------------------------------------------------------------
		for( int index = 0; index < _MAX_QUEST_COUNT_; index++ )
		{
			int iQuestID = pRole->PlayerBaseData->Quest.QuestLog[ index ].iQuestID;
			
			if( iQuestID != 0 )
			{
				GameObjDbStruct			*pObj	= g_ObjectData->GetObj( iQuestID );
				if( pObj )
				{	
					GameObjectQuestDetail	*pQuest = &( pObj->QuestDetail );

					if( pQuest->iRequest_QuestID == iQuestNPCID )
					{
						// 檢查及設置 NPC 身上 Quest 狀態
						if( pRole->CheckQuest_IsComplete( index, iQuestID, pQuest ) == EM_QUESTCHECK_RESULT_OK )
						{
							m_vecQuestID[2].push_back( iQuestID );
						}
						else
						{
							m_vecQuestID[1].push_back( iQuestID );
						}
					}
				}
			}
		}
	

	// SpeakOption 處理
	

	// 執行 LUA
	/*
	if( strlen( pObj->NPC.szLuaScript ) != 0 )
	{
		// 若有 Lua 字串表示, 該任務 NPC 會透過 Server 觸發對話選項後, 回傳選項至 Server
		CNetCli_Script::LoadSrvOption( iObjID );

	}
	else
		g_pGameMain->GetInterface()->GetUiWorld()->SendEvent( UiEvent_ShowQuestList );
	*/
	
	
	

	//g_pGameMain->GetInterface()->GetUiWorld()->SendEvent( UiEvent_ShowQuestList );
}
//--------------------------------------------------------------------------------------
int LuaFunc_OnLoad_QuestList	( lua_State *L )
{
	/*
	if( m_pQuestNPCList == NULL )
	{
		m_pQuestNPCList = CLayout::GetLayout( "UI_QuestNPCList" );

		if( m_pQuestNPCList ) 
		{			
			char szObjName[ 255 ];

			for( int Index = 0; Index <= 10; Index++ )
			{
				sprintf( szObjName,			"QuestSelectButton_%d", ( Index + 1 ) );
				m_pQuestSelectButton[ Index ]	= (CButton*)CLayout::GetLayout( szObjName );
				m_pQuestSelectButton[ Index ]->Hide();
			}

			m_pNPC_Text			= (CFontString*)CLayout::GetLayout( "UI_QuestNPCList_Text" );
			m_pPanelTitle		= (CFontString*)CLayout::GetLayout( "UI_QuestNPCList_Panel_Name" );
		}

	}
	*/
	return 0;
}
// --------------------------------------------------------------------------------------
int LuaFunc_GetSpeakDetail ( lua_State *L )
{
	if( g_pQuestList )
	{	
		RoleDataEx*			pRole		= CNetGlobal::RoleData();
		//string				Source		= pRole->PlayerTempData->SpeakInfo.SpeakDetail.Begin();
		string				Source												= ((CNetCli_Script_Child*)CNetCli_Script_Child::m_pThis)->m_SpeakDetail;
		string				str			= g_pQuestDetail->ReplaceKeyWord( Source );
		string				transStr	= g_pQuestDetail->ReplaceKeyWord( str );
		lua_pushstring ( L, transStr.c_str() );	// 1

		return 1;
	}	
	return 0;
}
// --------------------------------------------------------------------------------------
int LuaFunc_GetSpeakObjName ( lua_State *L )
{
	if( g_pQuestList )
	{	
		RoleDataEx*			pRole		= CNetGlobal::RoleData();
		int					iObjID		= pRole->PlayerTempData->SpeakInfo.iSpeakNPCID;
		CRoleSprite*		pRoleObj	= g_pGameMain->FindSpriteByID( iObjID );

		if( pRoleObj != NULL )
		{
			lua_pushstring ( L, pRoleObj->GetName() );	// 1
			return 1;
		}
	}	
	return 0;
}
// --------------------------------------------------------------------------------------
int			LuaFunc_GetNumQuest			( lua_State *L )
{
	if( g_pQuestList )
	{
		int iType	= luaL_checkint( L, 1 ) - 1;

		if( iType >= 0 && iType <= 2 )
		{
			int iNumQuest = (int)g_pQuestList->m_vecQuestID[ iType ].size();
			lua_pushnumber( L, iNumQuest );
			return 1;
		}
	}
	return 0;
}
// --------------------------------------------------------------------------------------
int			LuaFunc_GetQuestNameByIndex	( lua_State *L )
{
	if( g_pQuestList )
	{
		int iType	= luaL_checkint( L, 1 ) - 1;
		int iIndex	= luaL_checkint( L, 2 ) - 1;

		if( iType >= 0 && iType <= 2 )
		{
			int iNumQuest = (int)g_pQuestList->m_vecQuestID[ iType ].size();
			
			if( iIndex < iNumQuest )
			{
				int					iQuestID	= g_pQuestList->m_vecQuestID[ iType ][ iIndex ];
				GameObjDbStructEx*	pObj		= g_ObjectData->GetObj( iQuestID );
				const char*			szText		= pObj->GetName();

				bool bDaily = false;
				if( pObj->QuestDetail.iControlType != 0 || pObj->QuestDetail.iQuestGroup != 0 )
				{
					bDaily = true;
				}

				string				sQuestName	= CNetCli_Script_Child::ReplaceSystemKeyword( szText );

				lua_pushstring( L, sQuestName.c_str() );
				lua_pushboolean( L, bDaily				);	
				lua_pushnumber( L, pObj->QuestDetail.iQuestGroup );

				return 3;
			}
		}
	}	
	return 0;
}
// --------------------------------------------------------------------------------------
int LuaFunc_OnClick_QuestListButton	( lua_State *L )
{
	//CUiButton* pButton = dynamic_cast<CUiButton*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	
	if( g_pQuestList )
	{
		int iType	= luaL_checkint( L, 1 ) - 1;
		int iIndex	= luaL_checkint( L, 2 ) - 1;

		if( iType >= 0 && iType <= 2 )
		{
			int iNumQuest = (int)g_pQuestList->m_vecQuestID[ iType ].size();

			if( iIndex < iNumQuest )
			{
				int			iQuestID	= g_pQuestList->m_vecQuestID[ iType ][ iIndex ];

				g_pQuestDetail->SetQuest( 0, -1, iQuestID );
				return 0;
			}
		}
	}	
	
	return 0;
}
// --------------------------------------------------------------------------------------
int	LuaFunc_GetNumSpeakOption		( lua_State *L )
{
	if( g_pQuestList )
	{	
		RoleDataEx*			pRole		= CNetGlobal::RoleData();
		lua_pushnumber ( L, (int) pRole->PlayerTempData->SpeakInfo.SpeakOption.Size() );	// 1

		return 1;
	}	
	return 0;
}
// --------------------------------------------------------------------------------------
int	LuaFunc_GetSpeakOption			( lua_State *L )
{
	int				iIndex	= luaL_checkint( L, 1 );
	RoleDataEx*		pRole	= CNetGlobal::RoleData();

	if( iIndex >= 1 && iIndex <= (int) pRole->PlayerTempData->SpeakInfo.SpeakOption.Size() )
	{
		lua_pushstring( L, pRole->PlayerTempData->SpeakInfo.SpeakOption[ ( iIndex - 1 ) ].szOption.Begin() );
		lua_pushnumber( L, pRole->PlayerTempData->SpeakInfo.SpeakOption[ ( iIndex - 1 ) ].iIconID );
		return 2;
	}

	return 0;
}
// --------------------------------------------------------------------------------------
int	LuaFunc_ChoiceOption			( lua_State *L )
{
	int				iIndex	= luaL_checkint( L, 1 );

	// 鎖住介面, 不讓玩家操作

	CUiFrame* pFrame = (CUiFrame*)g_pGameMain->GetInterface()->GetUiWorld()->FindObject( "SpeakFrame" );
	if( pFrame != NULL )
		pFrame->SetEnable( false );

	CNetCli_Script::ChoiceOption( iIndex );

	return 0;
}
// --------------------------------------------------------------------------------------
int	LuaFunc_ChoiceListDialogOption( lua_State *L )
{
	int				iIndex	= luaL_checkint( L, 1 );

	// 鎖住介面, 不讓玩家操作

	if( NetCli_OtherChild::This->m_pfuncCliSpeakDialog == NULL )
	{

		if( iIndex != -1 )
		{

			CUiFrame* pFrame = (CUiFrame*)g_pGameMain->GetInterface()->GetUiWorld()->FindObject( "SpeakFrame" );
			if( pFrame != NULL )
				pFrame->SetEnable( false );
		}

		//CNetCli_Script::ChoiceOption( iIndex );
		//CNetCli_Script_Child::m_bSendingOption = true;

		if( NetCli_Other::m_DialogType != EM_LuaDialogType_None )
			NetCli_Other::S_DialogSelectID( iIndex );
	}
	else
	{
		NetCli_OtherChild::This->m_pfuncCliSpeakDialog( iIndex, NetCli_OtherChild:: m_pCliSpeakDialogContext );
	}

	return 0;
}

int	LuaFunc_GetSpeakTitle			( lua_State *L )
{
	RoleDataEx*			pRole		= CNetGlobal::RoleData();

	CRoleSprite*		pRoleObj	= g_pGameMain->FindSpriteByID( pRole->PlayerTempData->SpeakInfo.iSpeakNPCID );

	if( pRoleObj != NULL )
	{
		char szName[256];
		sprintf( szName, pRoleObj->GetName() );		
		lua_pushstring( L, szName );
		return 1;

	}

	return 0;
}
		
/*

void CUI_QuestList::Initial()
{
if ( g_pUi_QuestNPCList == NULL )
{
g_pUi_QuestNPCList = new CUI_QuestNPCList;
}
}
// --------------------------------------------------------------------------------------
void CUI_QuestList::Release()
{
SAFE_DELETE( g_pUi_QuestNPCList );	
}
//--------------------------------------------------------------------------------------
void CUI_QuestList::OnLeave_InteractRange		( CControlInteractUI* pUI, CRoleSprite* pRoleObj )
{
if( m_pQuestNPCList )
m_pQuestNPCList->Hide();
}

void CUI_QuestList::Add_NpcText					( const char* pszNpcText )
{
if( m_pQuestNPCList )
{ 
m_pNPC_Text->SetTextA( pszNpcText );
}
}
//--------------------------------------------------------------------------------------
void CUI_QuestList::Add_Available_Quest			( int iQuestID, const char* pszQuestName )
{
if( m_pQuestNPCList )
{ 
int Index = m_vecQuestID.size();
m_vecQuestID.push_back( iQuestID );
m_vecQuestMode.push_back( 0 );
//m_pAvailableQuestButton_Text[ m_iQuestIndex ]->SetTextA( pszQuestName );;

m_pQuestSelectButton[ Index ]->SetVisible( true );
m_pQuestSelectButton[ Index ]->SetTextA( pszQuestName );
}
}
//--------------------------------------------------------------------------------------
void CUI_QuestList::Add_Current_Quest			( int iQuestID, const char* pszQuestName )
{
if( m_pQuestNPCList )
{
int Index = m_vecQuestID.size();
m_vecQuestID.push_back( iQuestID );
m_vecQuestMode.push_back( 1 );
//m_pAvailableQuestButton_Text[ m_iQuestIndex ]->SetTextA( pszQuestName );;
m_pQuestSelectButton[ Index ]->SetVisible( true );
m_pQuestSelectButton[ Index ]->SetTextA( pszQuestName );
}
}
//--------------------------------------------------------------------------------------
void CUI_QuestList::Add_Complete_Quest			( int iQuestID, const char* pszQuestName )
{
if( m_pQuestNPCList )
{ 
char szQuestName[64];
sprintf( szQuestName, "%s ( Complete )", pszQuestName );

int Index = m_vecQuestID.size();
m_vecQuestID.push_back( iQuestID );
m_vecQuestMode.push_back( 2 );
//m_pAvailableQuestButton_Text[ m_iQuestIndex ]->SetTextA( pszQuestName );;
m_pQuestSelectButton[ Index ]->SetVisible( true );
m_pQuestSelectButton[ Index ]->SetTextA( szQuestName );
}
}
//--------------------------------------------------------------------------------------
void CUI_QuestList::Show						()
{
if( m_pQuestNPCList )
{
int Index = m_vecQuestID.size();
m_vecQuestID.push_back( 0 );
m_vecQuestMode.push_back( -1 );

//m_pAvailableQuestButton_Text[ m_iQuestIndex ]->SetTextA( pszQuestName );;
m_pQuestSelectButton[ Index ]->SetVisible( true );
m_pQuestSelectButton[ Index ]->SetTextA( "大爺現在沒空!!下次再找你聊天." );
}

if( m_pQuestNPCList )
{
// 設定 NPC 移動範圍偵測
//SetInteractRangeOn( g_pUi_QuestNPCList, m_InteractNPC );
m_pQuestNPCList->SetVisible( true );
}
}
//--------------------------------------------------------------------------------------
void CUI_QuestList::ReSet						()
{

m_iQuestIndex = 0;

m_vecQuestID.clear();
m_vecQuestMode.clear();	

if( m_pQuestNPCList  )
{
for( int Index = 0; Index < 10; Index++ )
{
m_pQuestSelectButton[ Index ]->SetVisible( false );
}
}
}
//--------------------------------------------------------------------------------------
void CUI_QuestList::Set_NpcName ( const char* pszNpcName )
{
if( m_pPanelTitle )
m_pPanelTitle->SetTextA( pszNpcName );
}
//--------------------------------------------------------------------------------------
void CUI_QuestList::SelectQuest					( int iID )
{
m_pQuestNPCList->SetVisible( false );

if( iID <= m_vecQuestID.size() )
{
int iQuestID	= m_vecQuestID[ ( iID - 1 ) ];
int	iQuestMode	= m_vecQuestMode[ ( iID - 1 ) ];

//if( iQuestMode != -1 ) 
//g_pGameMain->OnQuestDetail( iQuestID, iQuestMode );
}
}
//--------------------------------------------------------------------------------------
int	CUI_QuestList::GetQuestSelectButtonMode				( int iID )
{
if( iID <= m_vecQuestID.size() )
{
int	iQuestMode	= m_vecQuestMode[ ( iID - 1 ) ];

return iQuestMode;
}
else
return -1;

}



*/



//--------------------------------------------------------------------------------------
