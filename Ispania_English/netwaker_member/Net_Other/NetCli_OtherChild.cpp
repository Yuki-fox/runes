#include "NetCli_OtherChild.h"
#include "../net_instance/Net_Instance.h"
#include "../../mainproc/GameMain.h"
#include "../Interface/WorldFrames/ChatFrame.h"
#include "../Interface/WorldFrames/ChatEdit.h"
#include "../Interface/WorldFrames/CastingBarFrame.h"
#include "../Interface/WorldFrames/BagFrame.h"
#include "../Interface/WorldFrames/BankFrame.h"
#include "../INTERFACE/WorldFrames/EmoteFrame.h"
#include "../Interface/WorldFrames/InspectFrame.h"
#include "../Interface/WorldFrames/GameControl.h"
#include "../Interface/WorldFrames/PartyFrame.h "
#include "../INTERFACE/Teach/UI_TeachingFrame.h"
#include "../net_channel/NetCli_ChannelChild.h"
#include "../net_script/NetCli_Script_Child.h"
#include "../Interface/Map/WorldMap.h"
#include "../Interface/WorldFrames/GiveItemFrame.h"
#include "../Interface/itemlink/ItemLink.h"
#include "../Interface/GuildHouses/GuildHousesFrame.h"
#include "../Interface/ScoreBoard/UI_ScoreBoard.h"
#include "../Interface/WorldFrames/UI_SortScoreBoard.h"
#include "../Interface/WorldFrames/ActionFrame.h"
#include "../Interface/CraftFrame/UI_CraftFrame.h"
#include "../Interface/WorldFrames/AncillaryTitleFrame.h "
#include "../Interface/WorldFrames/WeekInstance.h"
#include "../Interface/WorldFrames/PhantomFrame.h"

float	NetCli_OtherChild::_ColorList[3][125];

NetInspectStruct NetCli_OtherChild::s_inspectInfo;
vector<ObjectPosStruct>	NetCli_OtherChild::_AllObjectPos;


int NetCli_OtherChild::s_NPCHateListGUID ;
NPCHateList NetCli_OtherChild::s_NPCHateList ;


bool NetCli_OtherChild::s_bRunGlobalPlotResulted = false;

std::set< string > NetCli_OtherChild::_FindingStructName;
std::set< int >    NetCli_OtherChild::_FindingStructDBID;
std::map< string, FindDBIDRoleNameStruct > NetCli_OtherChild::_NameFindStructMap;
std::map< int   , FindDBIDRoleNameStruct > NetCli_OtherChild::_DBIDFindStructMap;
std::list< IFindDBIDRoleNameWorkBase* > NetCli_OtherChild::_FindDBIDRoleNameWorks;
std::set<INT64> NetCli_OtherChild::m_ReportList;

//-----------------------------------------------------------------------------
bool NetCli_OtherChild::_Init()
{
	if (This)
		return false;

	s_inspectInfo.GItemID = 0;
	s_inspectInfo.EQInfo.Init();	
	s_inspectInfo.note.clear();

	This = new NetCli_OtherChild;

	int  RandNum[125];
	for( int i = 0 ; i < 125 ; i++ )
		RandNum[i] = i;

	for( int i = 0 ; i < 124 ; i++ )
		swap( RandNum[ i ] , RandNum[ rand() % 124 ] );

	for( int i = 0 , x =0 ; i < 5 ; i++ )
		for( int j = 0 ; j < 5 ; j++ )
			for( int k =0 ; k < 5 ; k++)
	{			
			_ColorList[0][RandNum[x]] = 0.2f * i;  
			_ColorList[1][RandNum[x]] = 0.2f * j;
			_ColorList[2][RandNum[x]] = 0.2f * k;
			x++;
	}
	
	return NetCli_Other::_Init();
}
//-----------------------------------------------------------------------------
bool NetCli_OtherChild::_Release()
{
	NetCli_Other::_Release();
	if (This)
	{
		delete This;
		This = NULL;
	}
 
	std::list< IFindDBIDRoleNameWorkBase* >::iterator itPos = _FindDBIDRoleNameWorks.begin();
	std::list< IFindDBIDRoleNameWorkBase* >::iterator itEnd = _FindDBIDRoleNameWorks.end();

	for( ; itPos != itEnd ; ++itPos )
	{
		delete *itPos;
	}

	_FindDBIDRoleNameWorks.clear();

	return true;
}


void NetCli_OtherChild::R_OpenDialog( LuaDialogType_ENUM DialogType , const char* Title, const char* Content , vector<string>& SelectStr, pfCliSpeakDialogfunc pFunc, PVOID pContext )
{
	m_pfuncCliSpeakDialog		= pFunc;
	m_pCliSpeakDialogContext	= pContext;
	m_DialogType = DialogType;

	lua_State* L = g_pGameMain->GetInterface()->GetUi()->GetLuaState();

	// °e¥X LUA ¨Æ¥ó¥´¶}¸ß°Ý¹ï¸Ü²°©Î¿ï«h¹ï¸Ü²°
	if( DialogType == EM_LuaDialogType_YesNo )
	{

		if( SelectStr.size() >= 2 )
		{			
			lua_pushstring( L, CNetCli_Script_Child::ReplaceSystemKeyword( Content ).c_str() );
			lua_setglobal( L, "arg1" );

			lua_pushstring( L, CNetCli_Script_Child::ReplaceSystemKeyword( SelectStr[0].c_str() ).c_str() );
			lua_setglobal( L, "arg2" );

			lua_pushstring( L, CNetCli_Script_Child::ReplaceSystemKeyword( SelectStr[1].c_str() ).c_str() );
			lua_setglobal( L, "arg3" );

			g_pInterface->SendWorldEvent( UiEvent_ShowRequestDialog );
		}
		else
		if( SelectStr.size() == 1 )
		{
			lua_pushstring( L, CNetCli_Script_Child::ReplaceSystemKeyword( Content ).c_str() );
			lua_setglobal( L, "arg1" );

			lua_pushstring( L, CNetCli_Script_Child::ReplaceSystemKeyword( SelectStr[0].c_str() ).c_str() );
			lua_setglobal( L, "arg2" );
			
			g_pInterface->SendWorldEvent( "SHOW_MESSAGE_DIALOG" );
			//UiEvent_ShowRequestDialog
			//g_pInterface->SendWorldEvent( UiEvent_ShowRequestDialog );
		}
	}
	else
	if( DialogType == EM_LuaDialogType_Select )
	{

		CUiFrame* pFrame = (CUiFrame*)g_pGameMain->GetInterface()->GetUiWorld()->FindObject( "SpeakFrame" );
		if( pFrame != NULL )
			pFrame->SetEnable( true );

		SpeakOptionStruct	option;
		RoleDataEx*			pRole		= CNetGlobal::RoleData();

		pRole->PlayerTempData->SpeakInfo.Reset();

		//pRole->PlayerTempData->SpeakInfo.SpeakDetail = CNetCli_Script_Child::ReplaceSystemKeyword( Content ).c_str();
		((CNetCli_Script_Child*)CNetCli_Script_Child::m_pThis)->m_SpeakDetail = CNetCli_Script_Child::ReplaceSystemKeyword( Content ).c_str();

		for( vector<string>::iterator it = SelectStr.begin(); it != SelectStr.end(); it++ )
		{
			option.szOption = CNetCli_Script_Child::ReplaceSystemKeyword( (*it).c_str() ).c_str();
			option.iIconID = EM_MapIconType_Speak;
			pRole->PlayerTempData->SpeakInfo.SpeakOption.Push_Back( option );
		}

		if( Title == NULL )
		{
			lua_pushstring( L, CNetCli_Script_Child::ReplaceSystemKeyword( m_DialogTitle.c_str() ).c_str() );
		}
		else
		{
			lua_pushstring( L, CNetCli_Script_Child::ReplaceSystemKeyword( Title ).c_str() );
		}

		lua_setglobal( L, "arg1" );
		g_pInterface->SendWorldEvent( UiEvent_ShowRequestListDialog );
	}
	else
	if( DialogType == EM_LuaDialogType_Input )
	{
		lua_pushstring( L, CNetCli_Script_Child::ReplaceSystemKeyword( Content ).c_str() );
		lua_setglobal( L, "arg1" );

		lua_pushstring( L, CNetCli_Script_Child::ReplaceSystemKeyword( SelectStr[0].c_str() ).c_str() );
		lua_setglobal( L, "arg2" );

		lua_pushstring( L, CNetCli_Script_Child::ReplaceSystemKeyword( SelectStr[1].c_str() ).c_str() );
		lua_setglobal( L, "arg3" );

		g_pInterface->SendWorldEvent( "SHOW_SERVERINPUT_DIALOG" );
	}
	else if( DialogType == EM_LuaDialogType_Pet )
	{
		lua_pushstring( L, CNetCli_Script_Child::ReplaceSystemKeyword( Content ).c_str() );
		lua_setglobal( L, "arg1" );

		for( int i = 0; i < SelectStr.size(); ++i )
		{
			lua_pushstring( L, CNetCli_Script_Child::ReplaceSystemKeyword( SelectStr[i].c_str() ).c_str() );
			char buf[6];
			sprintf_s( buf, sizeof( buf ), "arg%d", i + 2 );
			lua_setglobal( L, buf );
		}

		g_pInterface->SendWorldEvent( "SHOW_PET_EVENT_DIALOG" );
	}
}

void NetCli_OtherChild::R_CloseDialog()
{

	LuaDialogType_ENUM Type = m_DialogType;

	m_DialogType			= EM_LuaDialogType_None;

	if( Type == EM_LuaDialogType_YesNo )
		g_pInterface->SendWorldEvent( UiEvent_HideRequestDialog );
	else
	if( Type == EM_LuaDialogType_Input )
	{
		g_pInterface->SendWorldEvent( "HIDE_SERVERINPUTDIALOG"  );
	}
	else if( Type == EM_LuaDialogType_Pet )
		g_pInterface->SendWorldEvent( "HIDE_PET_EVENT_DIALOG" );
	else
	{
		CUiFrame* pFrame = (CUiFrame*)g_pGameMain->GetInterface()->GetUiWorld()->FindObject( "SpeakFrame" );
		if( pFrame != NULL )
		{
			pFrame->SetEnable( true );
			if( pFrame->IsVisible() == true )
			{
				g_pInterface->SendWorldEvent( UiEvent_CloseSpeakFrame );
			}

		}

		m_DialogTitle = "";
	}

	m_DialogType = EM_LuaDialogType_None;
}

void NetCli_OtherChild::R_DialogSetContent( const char* Content )
{
	RoleDataEx*			pRole		= CNetGlobal::RoleData();
	CUiFrame* pFrame = (CUiFrame*)g_pGameMain->GetInterface()->GetUiWorld()->FindObject( "SpeakFrame" );
	if( pFrame != NULL )
		pFrame->SetEnable( true );

	
	//pRole->PlayerTempData->SpeakInfo.SpeakDetail = Content;
	((CNetCli_Script_Child*)CNetCli_Script_Child::m_pThis)->m_SpeakDetail	= Content;
	g_pInterface->SendWorldEvent( UiEvent_ResetSpeakFrameText );
}

void NetCli_OtherChild::R_DialogSetTitle( const char* cpszTitle )
{
	//lua_State* L = g_pGameMain->GetInterface()->GetUi()->GetLuaState();

	m_DialogTitle = cpszTitle;
	//lua_pushstring( L, cpszTitle );
	//lua_setglobal( L, "arg1" );

	//g_pGameMain->GetInterface()->GetUiWorld()->SendEvent( UiEvent_RESET_SPEAKFRAME_TITLE );
}

void NetCli_OtherChild::S_Inspect(int GItemID)
{
	s_inspectInfo.GItemID = GItemID;
	S_PlayerInfoRequest(GItemID);
}

ItemFieldStruct* NetCli_OtherChild::GetInspectItemFieldStruct(int index)
{
	if ( index < 0 && index >= EM_EQWearPos_MaxCount )
		return NULL;
	return &s_inspectInfo.EQInfo.Item[index];
}
/*
void NetCli_OtherChild::R_SystemMessage		( SystemMessageENUM Msg )
{
	switch(Msg)
	{
	case EM_SystemMessage_TranStateNotCancelYet:
		g_pChatFrame->AddMessage( 5, "¥æ©öª¬ºA¨S¸Ñ°£" );
		break;
		case EM_SystemMessage_MagicIDError
	}

}
*/
/*
void NetCli_OtherChild::R_SystemMessageData	( SystemMessageDataENUM Msg , int Size , char* Data )
{
	switch(Msg)
	{
	case EM_SystemMessageData_NULL:
		break;
	}
}
*/

//Casting Bar
void NetCli_OtherChild::R_BeginCastingResult	( CastingTypeENUM Type , int ItemID , int CastingTime , bool Result )
{
	const char* name = NULL;

	if ( Result )
	{
		GameObjDbStructEx* itemDB;
		switch (Type)
		{			
		case EM_CastingType_NormalUseItem:
			if ( (itemDB = CNetGlobal::GetObj(ItemID)) != NULL )
				name = itemDB->GetName();
			break;

		case EM_CastingType_Dissolution:
			name = g_ObjectData->GetString("DISSOLUTION_CASTING");
			break;
		}
	}

	if ( name )
		g_pCastingBarFrame->CastingStart(eCastingBar_UseItem, name, CastingTime / 1000.0f);
	else
		g_pGameMain->ItemCastingInterrupt();
	
}
void NetCli_OtherChild::R_InterruptCasting		( CastingTypeENUM Type )
{
	g_pCastingBarFrame->CastingFailed(eCastingBar_UseItem);
	g_pGameMain->ItemCastingInterrupt();
}
void NetCli_OtherChild::R_CastingOK			( CastingTypeENUM Type )
{
	g_pCastingBarFrame->CastingStop(eCastingBar_UseItem);
	g_pGameMain->ItemCastingStop();
}
/*
void NetCli_OtherChild::R_ItemExchangeResult	( ItemExchangeResultTypeENUM ResultType , ItemFieldStruct& Item , int Money , int Money_Account )
{
	switch( ResultType )
	{
	case EM_ItemExchangeResultType_WorldError:
		g_pGameMain->SendWarningMsg( "ITEMEXCHANGE_WORLD_ERROR" );
		break;
	case EM_ItemExchangeResultType_ItemUsed:		//ª««~¤w³Q¨Ï¥Î
		g_pGameMain->SendWarningMsg( "ITEMEXCHANGE_ITEM_USED" );
		break;
	case EM_ItemExchangeResultType_Failed:			//¥¢±Ñ¡AµL¦¹ª«¥ó
		g_pGameMain->SendWarningMsg( "ITEMEXCHANGE_NOT_FIND" );
		break;
	case EM_ItemExchangeResultType_OK:			//¥¢±Ñ¡AµL¦¹ª«¥ó
		g_pGameMain->SendWarningMsg( "ITEMEXCHANGE_OK" );
		break;

	}

}
*/
void NetCli_OtherChild::R_RangeDataTransfer	( int DataSize , const char* Data )
{
	g_pNetCli_ChannelChild->R_ClitoCli_Data(Data , DataSize );

	//if( DataSize < 4 )
	//	return;

	//int checkNum = *((int*)Data);

	////switch( checkNum )

	////test
	//if( checkNum = 0x01 )
	//{
	//	if( DataSize == sizeof(Emote_Package) )
	//		R_DoEmote( (Emote_Package*)Data );
	//}

}


void NetCli_OtherChild::R_CtoC_Emote ( PG_CtoC_Emote* pEmoteInfo )
{
	g_pEmoteFrame->R_DoEmote( pEmoteInfo->sourceID , pEmoteInfo->szTargetName, pEmoteInfo->emoteID );
}

void NetCli_OtherChild::R_ColoringShopResult	( ColoringShopResultENUM Result )
{
	

	g_pTeachingFrame->R_ColoringShopResult(Result);
}

void NetCli_OtherChild::R_PlayerInfoOK			( int PlayerGItemID , EQStruct& EQ , const char* Note )
{
	//g_pGameMain->SendWarningMsg("¬Ý¨ä¥L¨¤¦â¸ê®Æ ok" );

	s_inspectInfo.EQInfo = EQ;
	s_inspectInfo.note = Note;

	// §ó·s¸Ë³Æ
	g_pInspectFrame->UpdateInspectIntroduce();
	g_pInspectFrame->UpdateInspectItems();
}
void NetCli_OtherChild::R_PlayerInfoFailed		( int PlayerGItemID )
{
	g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_INSPECT_FAILED"));

	s_inspectInfo.EQInfo.Init();
	s_inspectInfo.note.clear();

	// §ó·s¸Ë³Æ
	g_pInspectFrame->UpdateInspectItems();
}

void NetCli_OtherChild::R_SysKeyFunctionResult	( bool Result , int KeyID , int TargetGUID )
{

}

void NetCli_OtherChild::R_ObjectPosInfo		( int Count , ObjectPosInfoStruct Info[6000] )
{
	map< int , int >	ColorMap;
	map< int , int >::iterator ColorMapIter;
	int ColorID = 1;
	g_pWorldMapFrame->ClearIcon();
	ObjectPosStruct Temp;
	_AllObjectPos.clear();
	for( int i = 0 ; i < Count ; i++ )
	{
		Temp.Info = Info[i];
		Temp.IsShow = true;

		//¨ú±oª«¥ó¸ê°T
		GameObjDbStructEx*	ObjDB 		= CNetGlobal::GetObj( Temp.Info.OrgObjID );
		if( ObjDB == NULL )
		{
			Temp.ColorID = 124;
		}
		else
		{
			switch( ObjDB->Classification )
			{
			case EM_ObjectClass_NPC:
				ColorMapIter = ColorMap.find( Temp.Info.OrgObjID );
				if( ColorMapIter != ColorMap.end() )
					Temp.ColorID = ColorMapIter->second;
				else
				{
					Temp.ColorID = ColorID;
					ColorMap[ Temp.Info.OrgObjID ] = ColorID;
					if( ColorID < 124 )
						ColorID++;

				}
				
				break;
			default:
				Temp.ColorID = 124;
				break;
			}
		}
		_AllObjectPos.push_back( Temp );
		Map_ShowIcon( Temp );
	}
}

//³]©wÃC¦â¥Îµ¥¯Å
void NetCli_OtherChild::Map_ResetPointColor_Lv		( )
{
	int ColorID = 1;

	g_pWorldMapFrame->ClearIcon();
	for( unsigned i = 0 ; i < _AllObjectPos.size() ; i++ )
	{
		ObjectPosStruct& Temp = _AllObjectPos[i];	

		//¨ú±oª«¥ó¸ê°T
		GameObjDbStructEx*	ObjDB 		= CNetGlobal::GetObj( Temp.Info.OrgObjID );

		switch( ObjDB->Classification )
		{
		case EM_ObjectClass_NPC:
			Temp.ColorID = Temp.Info.Level ;
			if( Temp.ColorID > 100 )
				Temp.ColorID = 100;
			break;
		case EM_ObjectClass_Mine:
			break;
		case EM_ObjectClass_Flag:
			break;
		case EM_ObjectClass_QuestNPC:
			break;
		default:
			break;
		}
		if( Temp.IsShow )
			Map_ShowIcon( Temp );		
	}
}


//²M°£¤p¦a¹Ï¤W©Ò¦³ªºª«¥ó
void NetCli_OtherChild::Map_ClearAllObjectPoint	( )
{
	g_pWorldMapFrame->ClearIcon();
	for( unsigned i = 0 ; i < _AllObjectPos.size() ; i++ )
	{
		_AllObjectPos[i].IsShow = false;
	}
}
//³]©w¬Y¤@­ÓIDªºª«¥ó
void NetCli_OtherChild::Map_SetObject_ID		( int OrgObjID )
{
	g_pWorldMapFrame->ClearIcon();
	for( unsigned i = 0 ; i < _AllObjectPos.size() ; i++ )
	{
		if( _AllObjectPos[i].Info.OrgObjID == OrgObjID )
			_AllObjectPos[i].IsShow = true;

		if( _AllObjectPos[i].IsShow == true )
			Map_ShowIcon( _AllObjectPos[i] );
	}	
}
//³]©w¬Y¤@Ãþ«¬ªºª«¥ó
void NetCli_OtherChild::Map_SetObject_Type		( int TypeID )
{
	GameObjectClassificationENUM Type;
	switch( TypeID )
	{
	case 0:
		Type = EM_ObjectClass_Player;
		break;
	case 1:
		Type = EM_ObjectClass_NPC;
		break;
	case 2:
		Type = EM_ObjectClass_QuestNPC;
		break;
	case 3:
		Type = EM_ObjectClass_Mine;
		break;
	}
	

	g_pWorldMapFrame->ClearIcon();
	for( unsigned i = 0 ; i < _AllObjectPos.size() ; i++ )
	{
		GameObjDbStructEx*	ObjDB = CNetGlobal::GetObj( _AllObjectPos[i].Info.OrgObjID );
		if( ObjDB == NULL )
			continue;

		if( ObjDB->Classification == Type )
			_AllObjectPos[i].IsShow = true;

		if( _AllObjectPos[i].IsShow == true )
			Map_ShowIcon( _AllObjectPos[i] );
	}	
}
//Åã¥Ü¬Yª«¥ó
void NetCli_OtherChild::Map_ShowIcon( ObjectPosStruct Info )
{
	
	GameObjDbStructEx*	ZoneDB		= CNetGlobal::GetObj( (Info.Info.GUID / 0x10000)%_DEF_ZONE_BASE_COUNT_ +  Def_ObjectClass_Zone );
	if( ZoneDB == NULL )
		return;

	//¨ú±oª«¥ó¸ê°T
	GameObjDbStructEx*	ObjDB 		= CNetGlobal::GetObj( Info.Info.OrgObjID );
	
	if( ObjDB == NULL )
	{
		g_pWorldMapFrame->AddIcon( ZoneDB->Zone.MapFile , Info.Info.X , Info.Info.Z 
									, _ColorList[0][ Info.ColorID ] , _ColorList[1][ Info.ColorID ] , _ColorList[2][ Info.ColorID ]  , EM_MapIconType_None
									, 8 , "unknow" , Info.Info.GUID );
		return;
	}

	int IconType = EM_MapIconType_None;

	switch( ObjDB->Classification )
	{
	case EM_ObjectClass_Player:
		IconType = EM_MapIconType_Player;
		break;
	case EM_ObjectClass_NPC:
		IconType = EM_MapIconType_NPC;
		break;
	case EM_ObjectClass_Mine:
		switch( ObjDB->Mine.Type )
		{
		case EM_GameObjectMineType_Mine:
			IconType = EM_MapIconType_Mine;
			break;
		case EM_GameObjectMineType_Wood:
			IconType = EM_MapIconType_Wood;
			break;
		case EM_GameObjectMineType_Herb:
			IconType = EM_MapIconType_Herb;
			break;
		case EM_GameObjectMineType_Fish:
			IconType = EM_MapIconType_Fish;
			break;
			}
		break;
	case EM_ObjectClass_Flag:
		IconType = EM_MapIconType_Flag;
		break;
	case EM_ObjectClass_QuestNPC:
		IconType = EM_MapIconType_NewQuest;
		break;
	default:
		break;
	}


	if( strlen( Info.Info.Name ) != 0  )
	{
		g_pWorldMapFrame->AddIcon( ZoneDB->Zone.MapFile , Info.Info.X , Info.Info.Z 
			, _ColorList[0][ Info.ColorID ] , _ColorList[1][ Info.ColorID ] , _ColorList[2][ Info.ColorID ] , IconType 
			, 10 , Info.Info.Name , Info.Info.GUID );
	}
	else
	{
		g_pWorldMapFrame->AddIcon( ZoneDB->Zone.MapFile , Info.Info.X , Info.Info.Z 
			, _ColorList[0][ Info.ColorID ] , _ColorList[1][ Info.ColorID ] , _ColorList[2][ Info.ColorID ] , IconType 
			, 10 , ObjDB->GetName() , Info.Info.GUID );
	}

}
void NetCli_OtherChild::R_OpenClientMenu		( ClientMenuType_ENUM MenuType )
{
	lua_State* L = g_pInterface->GetLuaState();

	
	if (MenuType>=EM_ClientMenuType_BillBoard&&MenuType<20000)
	{
		int sortype=MenuType-EM_ClientMenuType_BillBoard;
		char buff[1024];//
		int luaSortype=sortype;
		sprintf(buff,"ShowUIPanel(SortScoreBoard);SSB_Frame.sortType=%d;SSB_GetScoreItemBoard(%d);SSB_ST_Open(SSB_SortTypeFrame,%d);SSB_ST_OnUpdate(SSB_SortTypeFrame);",luaSortype,luaSortype,luaSortype);
		

		

		luaL_dostring(L,buff);

		//SSB_GetScoreItemBoard
	}
	switch (MenuType)
	{
	case EM_ClientMenuType_Bag:
		luaL_dostring(L, "ShowUIPanel(BagFrame)");
		break;

	case EM_ClientMenuType_Skill:
		luaL_dostring(L, "ShowUIPanel(UI_SkillBook)");
		break;

	case EM_ClientMenuType_EQ:
		luaL_dostring(L, "ShowUIPanel(CharacterFrame)");
		break;

	case EM_ClientMenuType_Quest:
		luaL_dostring(L, "ShowUIPanel(UI_QuestBook)");
		break;

	case EM_ClientMenuType_FriendList:
		luaL_dostring(L, "ShowUIPanel(SocialFrame)");
		break;

	case EM_ClientMenuType_CardBook:
		luaL_dostring(L, "ShowUIPanel(CardBookFrame)");
		break;

	case EM_ClientMenuType_LifeSkill:
		luaL_dostring(L, "ShowUIPanel(UI_CraftFrame)");
		break;

	case EM_ClientMenuType_Guild:
		luaL_dostring(L, "ShowUIPanel(GuildFrame)");
		break;

	case EM_ClientMenuType_GuildList:
		luaL_dostring(L, "ShowUIPanel(GuildListBoardFrame)");
		break;

	case EM_ClientMenuType_GuildBillboard:
		luaL_dostring(L, "ShowUIPanel(BagFrame)");
		break;

	case EM_ClientMenuType_Loading:
		//g_pGameMain->SetPlayerCameraPos();
		break;

	case EM_ClientMenuType_SetGuildFlag:
		luaL_dostring(L, "ShowUIPanel(GuildFlagFrame)");
		break;

	case EM_ClientMenuType_Merchant:
		luaL_dostring(L, "ShowUIPanel(ItemMallFrame)");
		break;

	case EM_ClientMenuType_GuildBank:
		g_pGuildHousesFrame->OpenGuildBank();
		break;

	case EM_ClientMenuType_BlackSmith:			// ¶}±Ò¥´ÅK
	case EM_ClientMenuType_Carpenter:			// ¶}±Ò¤ì¤u
	case EM_ClientMenuType_MakeArmor:			// ¶}±Ò»s¥Ò
	case EM_ClientMenuType_Tailor:				// ¶}±ÒµôÁ_
	case EM_ClientMenuType_Cook:				// ¶}±Ò²i¶¹
	case EM_ClientMenuType_Alchemy:				// ¶}±Ò·Òª÷
	case EM_ClientMenuType_Mining:				// ¶}±Ò«õÄq
	case EM_ClientMenuType_Lumbering:			// ¶}±Ò¬å§÷
	case EM_ClientMenuType_Herblism:			// ¶}±Ò±ÄÃÄ
	case EM_ClientMenuType_Fishing:				// ¶}±Ò³¨³½
	case EM_ClientMenuType_Plant:				// ¶}±ÒºØ´Ó
		g_pCraftFrame->OpenCraftUI(Def_ObjectClass_KeyItem + EM_SkillValueType_BlackSmith + (MenuType - EM_ClientMenuType_BlackSmith));
		break;
/*
	case EM_ClientMenuType_BillBoard: // ¶}±Ò±Æ¦æº]

		luaL_dostring(L, "ShowUIPanel(SortScoreBoard)");
	
		break;
		*/
	}
}

void NetCli_OtherChild::R_PlayerTarget( int PlayerGItemID, int TargetGItemID )
{
	CRoleSprite* sourceSprite = g_pGameMain->FindSpriteByID(PlayerGItemID);	

	if( sourceSprite == NULL )
		return;
	
	// Á×§K³]©w¨ì¥»¾÷ª±®a
	if ( sourceSprite != g_pGameMain->GetPlayer() ) {
		sourceSprite->SetTargetSprite(g_pGameMain->FindSpriteByID(TargetGItemID));
	}
}

void NetCli_OtherChild::R_RunGlobalPlotResult( bool Result )
{

	if (g_pTeachingFrame)
		g_pTeachingFrame->EndRunGlobalPlot(Result);


	s_bRunGlobalPlotResulted = false;

}

void NetCli_OtherChild::R_HateListResult( int NpcGUID , NPCHateList& NPCHate )
{
	//Åý©Çª«¦º¤F «O¯d¤³«ë¦Cªí
	if( s_NPCHateListGUID == NpcGUID )
	{
		if( NPCHate.Hate.Size() == 0 )
		{
			return;
		}
	}

	s_NPCHateListGUID = NpcGUID;
	s_NPCHateList = NPCHate;

	g_pInterface->SendWorldEvent( UiEvent_TargetHateListUpdated );
}


void NetCli_OtherChild::R_OpenGiveItemPlo		( bool IsSort , int Count , int TargetGUID )
{
	g_pGiveItemFrame->OpenGiveItemFrame( IsSort, Count, TargetGUID );
}

void NetCli_OtherChild::R_CheckPasswordResult	( char* Password , bool Result )
{
	if ( Result )
	{
		g_pGameMain->SetSecondPassword(Password);
		//g_pInterface->SendWorldEvent("PASSWORD_SUCCESS");			// ±K½X¿ù»~
	}
	else
	{		
		g_pGameMain->ClearSecondPassword();
		g_pInterface->SendWorldEvent("PASSWORD_FAILED");			// ±K½X¿ù»~
	}	

}

void NetCli_OtherChild::R_Find_DBID_RoleName	( int DBID , char* RoleName , bool IsFind )
{
	_InsertDBIDRoleName( DBID , RoleName , IsFind );
}

void NetCli_OtherChild::_InsertDBIDRoleName( int DBID, const char* RoleName, bool IsFind )
{
	FindDBIDRoleNameStruct findStruct;
	findStruct.DBID = DBID;
	findStruct.RoleName = RoleName;
	findStruct.IsFind = IsFind;

	if( RoleName )
		_FindingStructName.erase( RoleName );

	if( DBID != -1 )
		_FindingStructDBID.erase( DBID );

	if( IsFind )
	{
		_NameFindStructMap[RoleName] = findStruct;
		_DBIDFindStructMap[DBID] = findStruct;		
	}
	else
	{
		if( DBID != -1 )
		{
			_DBIDFindStructMap[DBID] = findStruct;	
		}
		else
		{
			_NameFindStructMap[RoleName] = findStruct;
		}
	}

	IFindDBIDRoleNameWorkBase* pWork;

	std::list< IFindDBIDRoleNameWorkBase* >::iterator itPos = _FindDBIDRoleNameWorks.begin();

	while( itPos != _FindDBIDRoleNameWorks.end() )
	{
		pWork = *itPos;

		if( pWork->DBID == DBID )
		{
			pWork->RoleName = RoleName;
			pWork->IsFind = IsFind;
			pWork->Update();

			delete pWork;

			_FindDBIDRoleNameWorks.erase( itPos++ );
			continue;
		}

		if( pWork->RoleName == RoleName )
		{
			pWork->DBID = DBID;
			pWork->IsFind = IsFind;
			pWork->Update();

  			delete pWork;

			_FindDBIDRoleNameWorks.erase( itPos++ );
			continue;
		}
		++itPos; 
	}

}

FindDBIDRoleNameStruct* NetCli_OtherChild::GetFindDBIDRoleNameStruct( const char* name )
{
	std::map< string, FindDBIDRoleNameStruct >::iterator it = _NameFindStructMap.find( name );
	if( it != _NameFindStructMap.end() )
	{
		return &(it->second);
	}

	if( _FindingStructName.find( name ) == _FindingStructName.end() )
	{
		_FindingStructName.insert( name );
		NetCli_Other::S_Find_DBID_RoleName( -1, name );
	}

	return NULL;
}

FindDBIDRoleNameStruct* NetCli_OtherChild::GetFindDBIDRoleNameStruct( int DBID )
{
	std::map< int, FindDBIDRoleNameStruct >::iterator it = _DBIDFindStructMap.find( DBID );
	if( it != _DBIDFindStructMap.end() )
	{
		return &(it->second);
	}

	if( _FindingStructDBID.find( DBID ) == _FindingStructDBID.end() )
	{
		_FindingStructDBID.insert( DBID );
		NetCli_Other::S_Find_DBID_RoleName( DBID, NULL );
	}

	return NULL;
}

//¥Î¨¤¦â¦WºÙ¨ú±oDBID §ä¤£¨ì¶Ç¦^-1
int NetCli_OtherChild::GetDBID( const char* RoleName )
{
	FindDBIDRoleNameStruct* pFindDBIDRoleName = GetFindDBIDRoleNameStruct( RoleName );
	if( pFindDBIDRoleName && pFindDBIDRoleName->IsFind )
	{
		return pFindDBIDRoleName->DBID;
	}
	return -1;
}

//¥ÎDBID¨ú±o¨¤¦â¦WºÙ §ä¤£¨ì¶Ç¦^ ""
const char* NetCli_OtherChild::GetRoleName( int DBID )
{
	FindDBIDRoleNameStruct* pFindDBIDRoleName = GetFindDBIDRoleNameStruct( DBID );
	if( pFindDBIDRoleName && pFindDBIDRoleName->IsFind )
	{
		return pFindDBIDRoleName->RoleName.c_str();
	}
	return "";
}

void NetCli_OtherChild::InsertDBIDRoleName( int DBID, const char* RoleName )
{
	std::map< int, FindDBIDRoleNameStruct >::iterator it = _DBIDFindStructMap.find( DBID );
	//if( it != _DBIDFindStructMap.end() && (it->second).IsFind ) //¤w¦s¦b
	if( it != _DBIDFindStructMap.end() ) //¤w¦s¦b
	{
		return;
	}
	
	_InsertDBIDRoleName( DBID , (char*)RoleName , true );
}

void NetCli_OtherChild::AddFindDBIDRoleNameWork( IFindDBIDRoleNameWorkBase* pWork )
{
	if( NULL == pWork )
	{
		return;
	}

	FindDBIDRoleNameStruct* pResult = NULL;

	if( pWork->DBID == -1 )
	{
		pResult = GetFindDBIDRoleNameStruct( pWork->RoleName.c_str() );
	}
	else
	{
		pResult = GetFindDBIDRoleNameStruct( pWork->DBID );
	}

	if( pResult )
	{
		pWork->DBID = pResult->DBID;
		pWork->RoleName = pResult->RoleName;
		pWork->IsFind = pResult->IsFind;
		pWork->Update();
		delete pWork;
	}
	else
	{
		_FindDBIDRoleNameWorks.push_back( pWork );
	}
	
}

// °}Àç³]©w
void NetCli_OtherChild::R_CampID				( int GItemID , CampID_ENUM CampID )
{
	CRoleSprite* sprite = g_pGameMain->FindSpriteByID(GItemID);
	if ( sprite )
	{		
		g_pGameMain->SetSpriteCampId(sprite, CampID);
	}
}

void NetCli_OtherChild::R_GoodEvilValue		( int PlayerGUID , float GoodEvilValue , GoodEvilTypeENUM GoodEvilType )
{
	//g_pError->AddMessage(0, 0, "R_GoodEvilValue %d Type %d Value %.3f", PlayerGUID, GoodEvilType, GoodEvilValue);

	if ( g_pGameMain->GetGItemID() == PlayerGUID )
	{
		_RoleData.PlayerBaseData.GoodEvil = GoodEvilValue;
	}

	CRoleSprite* sprite = g_pGameMain->FindSpriteByID(PlayerGUID);
	if ( sprite )
	{
		// ¥u³B²zª±®a
		if ( g_pGameMain->GetGItemID() == PlayerGUID && sprite->GetGoodEvilType() != GoodEvilType )
		{
			const char* msg = "";
			switch (GoodEvilType)
			{
			case EM_GoodEvil_Demon:
				msg = g_ObjectData->GetString("SYS_GOODEVIL_DEMON");
				break;
			case EM_GoodEvil_Evil3:
				msg = g_ObjectData->GetString("SYS_GOODEVIL_EVIL3");
				break;
			case EM_GoodEvil_Evil2:
				msg = g_ObjectData->GetString("SYS_GOODEVIL_EVIL2");
				break;
			case EM_GoodEvil_Evil1:
				msg = g_ObjectData->GetString("SYS_GOODEVIL_EVIL1");
				break;
			case EM_GoodEvil_Normal:
				msg = g_ObjectData->GetString("SYS_GOODEVIL_NORMAL");
				break;
			case EM_GoodEvil_Good1:
				msg = g_ObjectData->GetString("SYS_GOODEVIL_GOOD1");
				break;
			case EM_GoodEvil_Good2:
				msg = g_ObjectData->GetString("SYS_GOODEVIL_GOOD2");
				break;
			case EM_GoodEvil_Good3:
				msg = g_ObjectData->GetString("SYS_GOODEVIL_GOOD3");
				break;
			case EM_GoodEvil_Hero:
				msg = g_ObjectData->GetString("SYS_GOODEVIL_HERO");
				break;
			}
			g_pGameMain->SendSystemMsg(msg);
		}

		sprite->SetGoodEvilType(GoodEvilType, GoodEvilValue);
	}
}

void NetCli_OtherChild::R_ExchangeMoneyTable	( int Count , int DataSize , char* Data )
{
	g_ObjectData->SetExhcnageMoneyKeyValue( Count , DataSize , Data );
}
void NetCli_OtherChild::R_DBStringTable	( int Count , int DataSize , char* Data )
{
	g_ObjectData->SetDBStringTable( Count , DataSize , Data );
}



//­n¨D¶}»È¦æ
void NetCli_OtherChild::S_OpenBank()
{
	if( false == s_bRunGlobalPlotResulted )
	{
		s_bRunGlobalPlotResulted = true;
		NetCli_OtherChild::S_RunGlobalPlotRequest( 3, 1 );
	}
}

//­n¨D¶}AC
void NetCli_OtherChild::S_OpenAuction()
{
	if( false == s_bRunGlobalPlotResulted )
	{
		s_bRunGlobalPlotResulted = true;
		NetCli_OtherChild::S_RunGlobalPlotRequest( 3, 2 );
	}
}

//­n¨D¶}«H½c
void NetCli_OtherChild::S_OpenMail()
{
	if( false == s_bRunGlobalPlotResulted )
	{
		s_bRunGlobalPlotResulted = true;
		NetCli_OtherChild::S_RunGlobalPlotRequest( 3, 3 );
	}
}

void NetCli_OtherChild::SL_AutoReportPossibleCheater( AutoReportBaseInfoStruct& TargetInfo , AutoReportPossibleCheaterENUM Type, int DBID )
{
	INT64 key = (INT64)DBID | ((INT64)Type << 32);
	if ( m_ReportList.find(key) != m_ReportList.end() )
		return;

	m_ReportList.insert(key);
	NetCli_Other::SL_AutoReportPossibleCheater(TargetInfo, Type);
}

void NetCli_OtherChild::R_ColoringHorseResult( ColoringShopResultENUM Result )
{
	g_pTeachingFrame->R_ColoringShopResult(Result);

}
void NetCli_OtherChild::R_PKDeadDropItem( ItemFieldStruct& Item )
{
	string itemName;
	if ( ItemField_To_ItemLink(&Item, itemName) == false )
		itemName = g_ObjectData->GetItemColorName(&Item);

	g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM_GET, "", g_ObjectData->GetString("SYS_DEAD_DROP_ITEM"), itemName.c_str() );
}
void NetCli_OtherChild::R_PlayerEvent( PlayerEventTypeENUM Type )
{
	switch (Type)
	{
	case EM_PlayerEventType_WeakMagic:
		g_pChatFrame->SendChatMessage(CHAT_MSG_COMBAT, "", g_ObjectData->GetString("SYS_COMBAT_FEEBLE_TIME"));
		break;
	}
}
void NetCli_OtherChild::R_ClientClock( int BeginTime , int NowTime , int EndTime , int Type )
{
	lua_State* L = g_pInterface->GetLuaState();
	if ( L )
	{
		switch ( Type )
		{
		// Close
		case 0:
			g_pInterface->SendWorldEvent("TIMEKEEPER_CLOSE");
			break;

		// Start
		case 1:
			lua_pushnumber(L, BeginTime);
			lua_setglobal(L, "arg1");
			lua_pushnumber(L, EndTime);
			lua_setglobal(L, "arg2");
			lua_pushnumber(L, NowTime);
			lua_setglobal(L, "arg3");
			g_pInterface->SendWorldEvent("TIMEKEEPER_START");
			break;

		// Pause
		case 2:
			lua_pushnumber(L, NowTime);
			lua_setglobal(L, "arg1");
			g_pInterface->SendWorldEvent("TIMEKEEPER_PAUSE");
			break;		
		}
	}
}
void NetCli_OtherChild::R_AccountMoneyLockInfo ( int LockAccountMoney[ Def_AccountMoneyLockTime_Max_Count_ ] , int LockAccount_Forever )
{
	RoleDataEx*			pRole		= CNetGlobal::RoleData();
	memcpy( pRole->PlayerBaseData->LockAccountMoney , LockAccountMoney , sizeof( pRole->PlayerBaseData->LockAccountMoney ) );
	pRole->PlayerBaseData->LockAccountMoney_Forever = LockAccount_Forever;
	pRole->PlayerTempData->LockAccountMoney = pRole->PlayerBaseData->LockAccountMoney_Forever;
	for( int i = 0 ; i < Def_AccountMoneyLockTime_Max_Count_ ; i++ )
		pRole->PlayerTempData->LockAccountMoney += pRole->PlayerBaseData->LockAccountMoney[i];

	g_pGameControl->SendWorldEvent("ACCOUNTBOOK_UPDATE");
}

void NetCli_OtherChild::R_AccountMoneyTranError( )
{
	g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_ACCOUNTMONEYTRANERROR"));
	g_pGameControl->SendWorldEvent("ACCOUNTBOOK_SHOW");
}
void NetCli_OtherChild::RL_SetCycleMagic( int MagicID , int MagicPos )
{
	RoleDataEx*			pRole		= CNetGlobal::RoleData();	
	pRole->TempData.CycleMagic.MagicID = MagicID;
	pRole->TempData.CycleMagic.MagicPos = MagicPos;
}	
//§R°£Â¾·~
void NetCli_OtherChild::RL_DelJob( Voc_ENUM Job )
{
	RoleDataEx*			pRole		= CNetGlobal::RoleData();	

	if( pRole->BaseData.Voc_Sub == Job )
	{
		pRole->BaseData.Voc_Sub = EM_Vocation_None;
		pRole->TempData.UpdateInfo.VocChange = true;
	}
	memset( &(pRole->PlayerBaseData->AbilityList[Job]) , 0 , sizeof(BaseAbilityStruct) );
	
}

void NetCli_OtherChild::R_CreateDialog( LuaDialogType_ENUM DialogType , const char* Content )
{
	if( m_DialogType != EM_LuaDialogType_None )
	{
		if( NetCli_OtherChild::This->m_pfuncCliSpeakDialog == NULL )
		{
			NetCli_Other::S_DialogSelectID( -1 );
		}
		else
		{
			NetCli_OtherChild::This->m_pfuncCliSpeakDialog( -1, m_pCliSpeakDialogContext );
		}

		R_CloseDialog();

		m_DialogType = EM_LuaDialogType_None;
	}

	m_DialogContent = Content;
	m_DialogType	= DialogType;

	m_DialogSelectStr.clear();

}

//////////////////////////////////////////////////////////////////////////
//¤p¹CÀ¸¤¶­±
void NetCli_OtherChild::RL_SetSmallGameMenuType		( int MenuID , SmallGameMenuTypeENUM Type  )
{
	if (g_pScoreBoardFrame)

	g_pScoreBoardFrame->SetSmallGameMenuType(MenuID,Type);

}
void NetCli_OtherChild::RL_SetSmallGameMenuValue	( int MenuID , int ID , int Value )
{

	if (g_pScoreBoardFrame)

		g_pScoreBoardFrame->SetSmallGameMenuValue(MenuID,ID,Value);

}
void NetCli_OtherChild::RL_SetSmallGameMenuStr		( int MenuID , int ID , const char* Content )
{
	if (g_pScoreBoardFrame)

		g_pScoreBoardFrame->SetSmallGameMenuStr(MenuID,ID,Content);

}
void NetCli_OtherChild::RL_ResetInstanceResult		( bool Result )
{
	g_pPartyFrame->RL_ResetInstanceResult(Result);
}

void NetCli_OtherChild::RL_ResetInstanceInfo		( int ZoneID , bool IsReset )
{
	g_pPartyFrame->RL_ResetInstanceInfo(ZoneID, IsReset);
}
void NetCli_OtherChild::RL_PartyDice				( int PlayerDBID ,int Type ,int Rand )
{
	g_pGameControl->R_PartyDice(PlayerDBID, Type, Rand);
}
void NetCli_OtherChild::RL_BodyShopResult			( bool Result )
{
	if (g_pTeachingFrame)
	g_pTeachingFrame->R_BodyShopResult(Result);

}
//////////////////////////////////////////////////////////////////////////
void NetCli_OtherChild::RL_MoneyVendorReport		( int Count )
{
	g_pChatEdit->RL_MoneyVendorReport(Count);
}
void NetCli_OtherChild::RL_BotReport				( int Count )
{

}
void NetCli_OtherChild::RL_UnlockBotReport			( const char* Name , UnlockBotTypeENUM Type )
{

}

void NetCli_OtherChild::RL_SortBankEnd( )
{
	g_pBankFrame->RL_SortBankEnd();
}

// ----------------------------------------------------------------------------
// Åª¨ú¸ê®Æ¦¨¥
void NetCli_OtherChild::RD_LoadClientDataResult( int KeyID , int Size , char* Data  )
{
	g_pGameMain->ReceiveLoadClientData(KeyID, Size, Data);
	//g_pGameMain->LoadClientData(KeyID, Size, Data);
}

// ----------------------------------------------------------------------------
// Àx¦s¸ê®Æµ²ªG
void NetCli_OtherChild::RL_SaveClientDataResult( int KeyID , bool Result  )
{
	int a = 0;
}

void NetCli_OtherChild::RD_LoadClientDataResult_Account( int KeyID , int Size , char* Data  )
{

}
void NetCli_OtherChild::RM_SaveClientDataResult_Account( int KeyID , bool Result  )
{

}
//////////////////////////////////////////////////////////////////////////
//³]©wMap ªº Icon & Tips
void NetCli_OtherChild::RL_SetMapMark		( MapMarkInfoStruct& Info )
{
	const char* pTip = g_ObjectData->GetString( Info.Tip );
	g_pWorldMapFrame->RL_SetMapMark( Info.MarkID, Info.X, Info.Z, pTip, Info.IconID );
}
void NetCli_OtherChild::RL_ClsMapMark		( int MarkID )
{
	g_pWorldMapFrame->RL_ClsMapMark( MarkID );
}
void NetCli_OtherChild::RL_SetRoomValue	( int KeyID , int Value )
{
	g_pGameMain->PushRoomValue(KeyID, Value);	
}
void NetCli_OtherChild::RL_ClsRoomValue	( int KeyID )
{
	switch (KeyID)
	{
	case -1:
		g_pGameMain->ClearRoomValue();
		break;

	default:
		g_pGameMain->RemoveRoomValue(KeyID);
		break;
	}	
}

// ¤@¯ë¬°¸ü¨ã,·s¼W§Þ¯à¶µ¥Ø(¨ú¥N¥Ø«eª±®a§Þ¯à)
void NetCli_OtherChild::RL_AddExSkill( ExSkillBaseStruct&	Info )
{	
	g_pActionFrame->InsertImplementAction(Info.SpellerID, Info.SkillID, Info.SkillLv);
}

// ¤@¯ë¬°¸ü¨ã,·s¼W§Þ¯à¶µ¥Ø(¨ú¥N¥Ø«eª±®a§Þ¯à)
void NetCli_OtherChild::RL_DelExSkill( ExSkillBaseStruct&	Info )
{
	g_pActionFrame->DeleteImplementAction(Info.SpellerID, Info.SkillID, Info.SkillLv);
}

void NetCli_OtherChild::RL_SpellExSkillResult( bool Result , RoleErrorMsgENUM Msg  )
{
}

void NetCli_OtherChild::RL_BaseObjValueChange( int GItemID , ObjValueChangeENUM Type , const char* ValueStr , int Value )
{
	CRoleSprite* sprite = g_pGameMain->FindSpriteByID(GItemID);
	if ( sprite )
	{
		switch (Type)
		{
		case EM_ObjValueChange_Plot_Range:			//½d³ò¼@±¡		( "¦WºÙ" , ½d³ò )
			if ( sprite->GetAreaRange() != Value )
			{
				sprite->SetAreaRange(Value);
				CNetCli_Script_Child::OnModifyScriptState( sprite );
			}			
			break;

		case EM_ObjValueChange_Plot_Touch:			//Ä²¸I¼@±¡		( "¦WºÙ" , ½d³ò )
			if ( sprite->GetTouchRange() != Value )
			{
				sprite->SetTouchRange(Value);
				CNetCli_Script_Child::OnModifyScriptState( sprite );
			}
			break;

		case EM_ObjValueChange_Plot_ClientAuto:		//Client¦Û°Ê¼@±¡( "¦WºÙ" , 0 )
			break;

		case EM_ObjValueChange_Plot_Collision:		//¸I¼²¼@±¡		( "¦WºÙ" , 0 )
			// ³]©wsprite¸I¼²¼@±¡¶}Ãö
			sprite->EnablePlotCollisionTrigger(strlen(ValueStr) > 0);
			break;

		case EM_ObjValueChange_CursorType:			//´å¼Ð			( "" , ´å¼ÐÃþ«¬ )
			sprite->SetCursorMode(Value);
			break;
		}
	}
}

void NetCli_OtherChild::RL_TransferData_Range( int GItemID , char Data[20] )
{
	CRoleSprite* sprite = g_pGameMain->FindSpriteByID(GItemID);
	if ( sprite && sprite != g_pGameMain->GetPlayer() )
	{
		ClientToClientInfo info;
		CRuVector3 offset;
		memcpy(&info, Data, sizeof(ClientToClientInfo));

		offset.m_x = info.parentMount.x;
		offset.m_y = info.parentMount.y;
		offset.m_z = info.parentMount.z;
		if ( info.parentMount.id == 0 )
		{

		}
		else
		{
			CRoleSprite* parentMount = g_pGameMain->FindSpriteByID(info.parentMount.id);
			if ( parentMount )
			{
				CRuVector3 realOffset = parentMount->GetPos() - sprite->GetPos();
				if ( abs(realOffset.Magnitude() - offset.Magnitude()) > 10.0f )
				{

				}
			}
		}
		
		sprite->SetParentMountInfo(info.parentMount.id, offset);
	}
}
//lua Ä²µoªºclient ºÝ¨Æ¥ó
void NetCli_OtherChild::RL_LuaClientEvent( int GItemID , int EventType , const char* Content )
{
	CRoleSprite* sprite = g_pGameMain->FindSpriteByID(GItemID);

	switch ( EventType )
	{
	case EM_CLIENT_LUA_EVENT_TYPE01:
		if ( strcmp( Content, "stopmusic") == 0 )
		{
			g_pRuGlobalEventManager->StopMusic();
			g_pRuGlobalEventManager->SetEnvironmentMusicEnable(FALSE);
		}
		else if ( strcmp( Content, "playmusic") == 0 )
		{
			g_pRuGlobalEventManager->SetEnvironmentMusicEnable(TRUE);
		}
		break;

	case EM_CLIENT_LUA_EVENT_TYPE02:
		if ( Content ) {
			g_pRuGlobalEventManager->SetEnvironmentMusicEnable(FALSE);
			g_pRuGlobalEventManager->PlayMusicByFile(Content, 1.0f, TRUE);
		}
		break;

	case EM_CLIENT_LUA_EVENT_HOUSE_SERVANT:
		g_pHousesFrame->Servant_LuaClientEvent( GItemID , EventType , Content );
		break;
	}
}

void NetCli_OtherChild::RL_SetRoleValueResult	( bool Result )
{

}

void NetCli_OtherChild::RL_ZoneOpen			( int ZoneID )
{
	char temp[512];

	// ­«·s¨ú±o°Æ¥»­«¸m®É¶¡
	for ( int i = 0; i < 100; i++ )
	{
		// ¨ú±o°Æ¥»½s¸¹
		sprintf_s(temp, 512, "InstanceKey%02d", i);
		int zoneId = g_ObjectData->GetSysKeyValue(temp);

		if ( zoneId == ZoneID )
		{
			Net_Instance::SL_InstanceRestTimeRequest(zoneId, i);
			break;
		}
	}
}

void NetCli_OtherChild::RL_SetNewTitleSysResult( bool Result )
{
	g_pAncillaryTitleFrame->R_SetNewTitleSysResult( Result );
}
void NetCli_OtherChild::RL_NewTitleSys_UseItemResult( NewTitleSysUseItemResultENUM Result )
{
	g_pAncillaryTitleFrame->R_NewTitleSys_UseItemResult( Result );
}

void NetCli_OtherChild::RL_OpenSessionResult( SessionTypeENUM Type, int SessionID, int CBID )
{
	m_SessionID[Type] = SessionID;

	std::map<int, OpenSessionResultFunc>::iterator it = m_OpenSessionResultFunc.find(CBID);

	if (it != m_OpenSessionResultFunc.end())
	{
		if (it->second != NULL)
		{
			it->second(Type, SessionID);
		}

		m_OpenSessionResultFunc.erase(it);
	}
}

void NetCli_OtherChild::RL_FixAccountFlag( StaticFlag< _ACCOUNT_FLAG_COUNT_ >& AccountFlag )
{
	RoleDataEx*	pRole = CNetGlobal::RoleData();
	if( pRole == NULL )
		return;

	
	int KeyItemID=-1;

	for (int i=0;i<_ACCOUNT_FLAG_COUNT_;i++)
	{
		if (pRole->Base.AccountFlag.GetFlag( i )!= AccountFlag.GetFlag( i ) )
		{
			KeyItemID=i;
			break;
		}
	}

	pRole->Base.AccountFlag = AccountFlag;

	int id=g_ObjectData->GetSysKeyValue("Wardrobe_ItemPage3");
	if (KeyItemID>=id&&KeyItemID<(id+8))
	{
		g_pInterface->SendWorldEvent( "COOLCLOTHBAG_KEYITEM" );
	}
	else 
	{
		id=g_ObjectData->GetSysKeyValue("Wardrobe_Set6");
		if (KeyItemID>=id&&KeyItemID<(id+5))
		{
			g_pInterface->SendWorldEvent( "COOLSUIT_KEYITEM" );
		}

	}

	
}

//¤µ¤Ñªº½Æ¥»
void NetCli_OtherChild::RL_WeekInstances( int day , int count , WeekZoneStruct info[] )
{
	if (g_pWeekInstanceFrame != NULL)
	{
		g_pWeekInstanceFrame->RL_WeekInstances(day, count, info);
	}
}
//­n¨Dµ²ªG
void NetCli_OtherChild::RL_WeekInstancesTeleportResult( bool result )
{
	if (g_pWeekInstanceFrame != NULL)
	{
		g_pWeekInstanceFrame->RL_WeekInstancesTeleportResult(result);
	}
}
//­«¸m°Æ¥»¶Ç°e¦¸¼Æµ²ªG
void NetCli_OtherChild::RL_WeekInstancesReset( bool result )
{
	if (g_pWeekInstanceFrame != NULL)
	{
		g_pWeekInstanceFrame->RL_WeekInstancesReset(result);
	}
}
//½Æ¥»³Ñ¤Uªº¥Í¦s®É¶¡
void NetCli_OtherChild::RL_WeekInstancesLiveTime( int sec )
{
}
//¶}±ÒÃè¥@¬É°Æ¥»¤¶­±
void NetCli_OtherChild::RL_OpenWeekInstancesFrame(  )
{
	if (g_pWeekInstanceFrame != NULL)
	{
		g_pWeekInstanceFrame->Open();
	}
}
//Å]ÆF¤É¶¥µ²ªG
void NetCli_OtherChild::RL_PhantomRankUpResult( bool summon , bool result )
{
	if (g_pPhantomFrame != NULL)
	{
		g_pPhantomFrame->RL_PhantomRankUpResult(summon, result);
	}
}
//Å]ÆF¤É¯Åµ²ªG
void NetCli_OtherChild::RL_PhantomLevelUpResult( bool result )
{
	if (g_pPhantomFrame != NULL)
	{
		g_pPhantomFrame->RL_PhantomLevelUpResult(result);
	}
}
//Å]ÆF¸Ë³Æµ²ªG
void NetCli_OtherChild::RL_PhantomEQResult( bool result )
{
	if (g_pPhantomFrame != NULL)
	{
		g_pPhantomFrame->RL_PhantomEQResult(result);
	}
}
//­×¥¿Å]ÆF¸Ë³Æ
void NetCli_OtherChild::RL_FixPhantomEQ( int eq[2] )
{
	RoleDataEx*	pRole = CNetGlobal::RoleData();

	if(pRole == NULL)
		return;

	PhantomStruct& info = pRole->PlayerBaseData->Phantom;

	for(int i = 0; i < 2; ++i)
	{
		info.EQ[i] = eq[i];
	}

	if (g_pPhantomFrame != NULL)
	{
		g_pPhantomFrame->RL_FixPhantomEQ();
	}

	pRole->TempData.UpdateInfo.ReSetTalbe = true;
}
//­×¥¿Å]ÆF¸ê®Æ
void NetCli_OtherChild::RL_FixPhantom( int phantomID , PhantomBase& info )
{
	RoleDataEx*	pRole = CNetGlobal::RoleData();

	if(pRole == NULL)
		return;

	int pos = phantomID % 1000;

	if(pos >= MAX_PHANTOM_COUNT)
		return;

	pRole->PlayerBaseData->Phantom.Info[pos] = info;

	if (g_pPhantomFrame != NULL)
	{
		g_pPhantomFrame->RL_FixPhantom(phantomID);
	}
	pRole->TempData.UpdateInfo.ReSetTalbe = true;
}
//¶}±Ò¤ÛÆF¤¶­±
void NetCli_OtherChild::RL_OpenPhantomFrame(  )
{
	if (g_pPhantomFrame != NULL)
	{
		g_pPhantomFrame->RL_OpenPhantomFrame();
	}
}
//«á¥xÅ]¤ÛÄ_²°¸ê®Æ
void NetCli_OtherChild::RL_ItemCombinInfo( int groupID , ItemCombineStruct& info )
{
	ItemCombineStruct infobk = info;
	info.ProductCount = 0;
	info.ProductID = 0;

	int hashID = GetHashCode( &info , sizeof( info ) );
	RoleDataEx::ItemCombineListEx[groupID][hashID].push_back( infobk );
}
//«á¥xÅ]¤ÛÄ_²°¯Á¤ÞID
void NetCli_OtherChild::RL_ItemCombinGroupID( int groupID )
{
	RoleDataEx::ItemCombineListExGroupID = groupID;
}
