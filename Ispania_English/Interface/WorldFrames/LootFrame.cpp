#include "../netwaker_member/NetWakerClientInc.h"
#include "../mainproc/GameMain.h"
#include "../ItemLink/ItemLink.h"
#include "LootFrame.h"
#include "ItemUtil.h"

CLootFrame*				g_pLootFrame = NULL;

int LuaFunc_RollOnLoot(lua_State* L);
int	LuaFunc_AssignOnLoot(lua_State* L);
int LuaFunc_GetLootRollItemInfo(lua_State* L);
int LuaFunc_GetLootAssignItemSize(lua_State* L);
int LuaFunc_GetLootAssignItemInfo(lua_State* L);
int LuaFunc_SetLootAssignMember(lua_State* L);
int LuaFunc_GetLootAssignMember(lua_State* L);
int LuaFunc_SetLootMethod(lua_State* L);
int LuaFunc_GetLootMethod(lua_State* L);
int LuaFunc_SetLootThreshold(lua_State* L);
int LuaFunc_GetLootThreshold(lua_State* L);

// ’¶¨ú¾Ô§Q«~(«ÍÅé)
int LuaFunc_GetBootyNumItems	(lua_State* L);
int LuaFunc_GetBootyItemInfo	(lua_State* L);
int LuaFunc_GetBootyItemLink	(lua_State* L);
int LuaFunc_ClickBootyItem		(lua_State* L);
int LuaFunc_CloseBooty			(lua_State* L);

int LuaFunc_SetAutoTakeLoot		(lua_State* L);
int LuaFunc_IsAutoTakeLoot		(lua_State* L);

// ----------------------------------------------------------------------------
CLootFrame::CLootFrame(CInterface* object)
	: CInterfaceBase(object)
{
	g_pLootFrame = this;

	m_bootyID = 0;
	m_sendBooty = false;	
	m_groupLootItem.empty();
}

// ----------------------------------------------------------------------------
CLootFrame::~CLootFrame()
{
	g_pLootFrame = NULL;
}

// ----------------------------------------------------------------------------
void CLootFrame::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{
		lua_register( L, "RollOnLoot",					LuaFunc_RollOnLoot );
		lua_register( L, "AssignOnLoot",				LuaFunc_AssignOnLoot );
		lua_register( L, "GetLootRollItemInfo",			LuaFunc_GetLootRollItemInfo );
		lua_register( L, "GetLootAssignItemSize",		LuaFunc_GetLootAssignItemSize );
		lua_register( L, "GetLootAssignItemInfo",		LuaFunc_GetLootAssignItemInfo );
		lua_register( L, "SetLootAssignMember",			LuaFunc_SetLootAssignMember );
		lua_register( L, "GetLootAssignMember",			LuaFunc_GetLootAssignMember );
		lua_register( L, "SetLootMethod",				LuaFunc_SetLootMethod );
		lua_register( L, "GetLootMethod",				LuaFunc_GetLootMethod );
		lua_register( L, "SetLootThreshold",			LuaFunc_SetLootThreshold );
		lua_register( L, "GetLootThreshold",			LuaFunc_GetLootThreshold );

		lua_register( L, "GetBootyNumItems",			LuaFunc_GetBootyNumItems );
		lua_register( L, "GetBootyItemInfo",			LuaFunc_GetBootyItemInfo );
		lua_register( L, "GetBootyItemLink",			LuaFunc_GetBootyItemLink );
		lua_register( L, "ClickBootyItem",				LuaFunc_ClickBootyItem );
		lua_register( L, "CloseBooty",					LuaFunc_CloseBooty );		

		lua_register( L, "SetAutoTakeLoot",				LuaFunc_SetAutoTakeLoot );
		lua_register( L, "IsAutoTakeLoot",				LuaFunc_IsAutoTakeLoot );
	}
}

// ----------------------------------------------------------------------------
void CLootFrame::BuildFrames()
{
	m_bootyID = 0;

	m_sendBooty = false;
}

// ----------------------------------------------------------------------------
void CLootFrame::Update(float elapsedTime)
{
	NetCli_PartyChild::Update(elapsedTime);	

	// ÀË¬d¬O§_»Ý­nÃö³¬
	if ( m_bootyID != 0 )
	{
		// ÀË¬d¤w¸g¨S¦³ª««~
		bool isClose = true;
		for ( int i = 0; i < NetCli_Treasure::GetBootyCount(); i++ )
		{
			ItemFieldStruct* item = NetCli_Treasure::GetBootyItem(i);
			if ( item && !item->IsEmpty() )
			{
				isClose = false;
				break;
			}
		}

		if ( isClose == false )
		{
			// ÀË¬d¥Ø¼Ð¬O§_ÁÙ¦s¦b
			CRoleSprite* target = g_pGameMain->FindSpriteByID(m_bootyID);
			if ( target == NULL )
				isClose = true;
			else
			{
				// ´ú¸Õª±®a¶ZÂ÷»P²¾°Ê¶q
				CRoleSprite* player = g_pGameMain->GetPlayer();
				if ( player == NULL )
					isClose = true;
				else
				{
					CRoleSprite* controlSprite = player->GetProxyControlSprite();
					if ( controlSprite->GetMoveTime() > 0.1f )
						isClose = true;
					else
					{
						CRuVector3 disVector = controlSprite->GetPos() - target->GetPos();
						if ( disVector.Magnitude() > _MAX_ROLE_TOUCH_RANGE_ )
							isClose = true;
					}
				}
			}
		}

		if ( isClose )
			CloseBooty();
	}

	vector<AssignItemInfo>::iterator iter = m_uniqueAssignItem.begin();
	while ( iter != m_uniqueAssignItem.end() )
	{
		iter->waitTime -= elapsedTime;
		if ( iter->waitTime < 0.0f )
		{

			iter = m_uniqueAssignItem.erase(iter);
		}
		else
		{
			iter++;
		}
	}	
}

// ----------------------------------------------------------------------------
bool CLootFrame::OpenNewLootItem(int itemVersion)
{
	lua_State* L = GetLuaStateWorld();
	if ( m_groupLootItem.size() < NUM_GROUP_LOOT_ITEM && L )
	{
		m_groupLootItem.insert(itemVersion);
		lua_pushnumber(L, itemVersion);
		lua_setglobal(L, "arg1");
		SendWorldEvent("LOOT_ITEM_SHOW");
		return true;
	}
	return false;
}

// ----------------------------------------------------------------------------
void CLootFrame::DeleteLootItem(int itemVersion)
{
	set<int>::iterator iter = m_groupLootItem.find(itemVersion);
	if ( iter != m_groupLootItem.end() )
	{
		lua_State* L = GetLuaState();
		if ( L )
		{
			lua_pushnumber(L, itemVersion);
			lua_setglobal(L, "arg1");
			SendWorldEvent("CANCEL_LOOT_ROLL");
		}
		m_groupLootItem.erase(iter);
	}
}

// ----------------------------------------------------------------------------
void CLootFrame::RefreshAssignItem()
{
	SendWorldEvent(UIEvent_UpdateLootAssign);
}

// ----------------------------------------------------------------------------
void CLootFrame::SendOpenBooty( int GItemID )
{
	// ¦£¿ý¤¤,¶Ç°e«Ê¥]¤¤
	if ( m_sendBooty )
		return;

	// ­«½Æ¶}±Ò
	if ( GItemID == m_bootyID )
		return;	

	// Ãö³¬¤W¤@­Ó¬É­±
	CloseBooty();

	// ª±®a°Ê§@
	CRoleSprite* player = g_pGameMain->GetPlayer();
	if ( player )
	{
		CRoleSprite* controlSprite = player->GetProxyControlSprite();
		if ( controlSprite )
			controlSprite->SetActorState(ruFUSION_ACTORSTATE_CROUCH_BEGIN);
	}	

	m_sendBooty = true;
	NetCli_Treasure::SL_OpenRequest( GItemID );
}

// ----------------------------------------------------------------------------
void CLootFrame::OpenBooty(bool isResult, int bootyID)
{
	m_sendBooty = false;

	if ( isResult )
	{
		m_bootyID = bootyID;
		if ( m_bootyID != 0 )
		{

			g_pGameMain->Dismount();

			SendWorldEvent("BOOTY_SHOW");
		}
	}
	else
	{
		// ª±®a°Ê§@
		CRoleSprite* player = g_pGameMain->GetPlayer();
		if ( player )
		{
			CRoleSprite* controlSprite = player->GetProxyControlSprite();
			if ( controlSprite )
			{
				switch ( controlSprite->GetActorState() )
				{
				case ruFUSION_ACTORSTATE_CROUCH_BEGIN:
				case ruFUSION_ACTORSTATE_CROUCH_LOOP:
					controlSprite->SetActorState(ruFUSION_ACTORSTATE_CROUCH_END);
				}
			}
		}

		// ¶}±Ò¥¢±Ñ
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_BOOTY_OPEN_FAILED"));
	}
}

// ----------------------------------------------------------------------------
void CLootFrame::CloseBooty(bool onScript)
{
	if ( m_bootyID != 0 )
	{
		// ª±®a°Ê§@
		CRoleSprite* player = g_pGameMain->GetPlayer();
		if ( player )
		{
			CRoleSprite* controlSprite = player->GetProxyControlSprite();
			if ( controlSprite )
			{
				switch ( controlSprite->GetActorState() )
				{
				case ruFUSION_ACTORSTATE_CROUCH_BEGIN:
				case ruFUSION_ACTORSTATE_CROUCH_LOOP:
					controlSprite->SetActorState(ruFUSION_ACTORSTATE_CROUCH_END);
				}
			}
		}	

		// °e¥XÃö³¬«Ê¥]
		NetCli_Treasure::SL_Close(m_bootyID);

		// ²M°£ª««~¯Á¤Þ­È
		// NetCli_Treasure::ClearBooty();

		// ²M°£ID
		m_bootyID = 0;		
	}	

	// 
	if ( onScript == false ) {
		SendWorldEvent("BOOTY_HIDE");
	}
}

// ----------------------------------------------------------------------------
void CLootFrame::UpdateBooty()
{
	if ( m_bootyID != 0 )
	{
		SendWorldEvent("BOOTY_UPDATE");
	}
}

// ----------------------------------------------------------------------------
void CLootFrame::InsertUniqueAssignItem(const char* name, int playerDBID, int itemVersion, int itemID)
{
	if ( name == NULL )
		return;


	for ( vector<AssignItemInfo>::iterator iter = m_uniqueAssignItem.begin(); iter != m_uniqueAssignItem.end(); iter++ )
	{
		if ( iter->itemVersion == itemVersion && iter->itemID == itemID )
		{
			return;
		}
	}

	AssignItemInfo info;
	info.playerName		= name;
	info.waitTime		= 10.0f;
	info.itemID			= itemID;
	info.itemVersion	= itemVersion;
	info.playerDBID		= playerDBID;
	m_uniqueAssignItem.push_back(info);

	// °T®§ª±®a¬O§_¦³¦¹ª««~
	NetCli_Channel::SendToCliAskHasItem(name, g_pGameMain->GetPlayerName(), itemVersion, itemID);
}

// ----------------------------------------------------------------------------
void CLootFrame::ReceiveUniqueAssugnItem(int itemVersion, int itemID, bool hasItem)
{
	for ( vector<AssignItemInfo>::iterator iter = m_uniqueAssignItem.begin(); iter != m_uniqueAssignItem.end(); iter++ )
	{
		if ( iter->itemVersion == itemVersion && iter->itemID == itemID )
		{
			if ( hasItem )
			{
				g_pGameMain->SendWarningMsgEx(g_ObjectData->GetString("SYS_HAS_UNIQUE_ITEM"), iter->playerName.c_str());
			}
			else
			{
				NetCli_PartyChild::AssignTo( itemVersion, iter->playerDBID );
			}

			m_uniqueAssignItem.erase(iter);
			break;
		}
	}
}

// ----------------------------------------------------------------------------
int LuaFunc_RollOnLoot(lua_State* L)
{
	int itemVersion = luaL_checkint(L, 1);
	int type = 0;

	if ( lua_isstring(L, 2) )
	{
		const char* str = lua_tostring(L, 2);
		if ( strcmp(str, "roll") == 0 )				// »Ý¨D
			type = 1;
		else if ( strcmp(str, "greed") == 0 )		// ³g°ý
			type = 2;
		else										// ©ñ±ó
			type = 0;
	}

	switch ( type )
	{
	case 0:
		NetCli_PartyChild::S_PassItem(itemVersion);
		break;

	case 1:
		NetCli_PartyChild::S_LootItem(itemVersion);
		break;

	case 2:
		NetCli_PartyChild::S_LootItem_NotNeed(itemVersion);
		break;
	}

	return 0;
}

// ----------------------------------------------------------------------------
// ¤À°tª««~¥Ñª±®a¦WºÙ¤Ï¦V·j´Mª««~¯Á¤Þ¶¶§Ç
// arg1 itemVersion ª««~½s¸¹

int	LuaFunc_AssignOnLoot(lua_State* L)
{
	int itemVersion = luaL_checkint(L, 1);

	PartyAssignItem* assign = NetCli_PartyChild::GetAssignItem(itemVersion);
	if ( assign )
	{
		const char* name = luaL_checkstring(L, 2);

		for ( vector<PartyMemberInfo>::iterator iter = NetCli_PartyChild::s_member.begin(); iter != NetCli_PartyChild::s_member.end(); iter++ )
		{
			CRoleSprite* sprite = (CRoleSprite*)(iter->sprite);
			if ( strcmp(sprite->GetName(), name) == 0 )
			{
				// ÀË¬dª««~¬O§_°ß¤@
				GameObjDbStruct* itemDB = g_ObjectData->GetObj(assign->item.OrgObjID);
				if ( itemDB && itemDB->Mode.Unique )
				{
					// ¬O§_¬°¦Û¤v
					if ( strcmp(g_pGameMain->GetPlayerName(), name) == 0 )
					{
						// ¨Ã¥B¤w¸g¾Ö¦³§ïª««~
						if ( CNetGlobal::RoleData()->CheckItemInBody( itemDB->GUID ) )
						{
							g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_ITEM_UNIQUE"));
						}
						else
						{
							NetCli_PartyChild::AssignTo( itemVersion, iter->DBID );
						}
					}
					// °T°Ý¸Óª±®a¬O§_¦³¦¹ª««~
					else
					{

						if ( iter->GItemID == -1 )
						{
							g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_LOOT_ASSIGN_OFFLINE"));
							return 0;
						}

						//NetCli_PartyChild::AssignTo( itemVersion, iter->DBID );
						g_pLootFrame->InsertUniqueAssignItem( name, iter->DBID, itemVersion, itemDB->GUID );
					}
				}
				else
				{
					NetCli_PartyChild::AssignTo( itemVersion, iter->DBID );
				}
				break;
			}
		}		
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetLootRollItemInfo(lua_State* L)
{	
	PartyRollItem* loot = NetCli_PartyChild::GetRollItem(luaL_checkint(L, 1));
	if ( loot )
	{
		char tmp[MAX_PATH];
		RoleDataEx::GetItemFieldName(&(loot->item), tmp, MAX_PATH);
		GameObjDbStruct* itemDB = CNetGlobal::GetObj(loot->item.OrgObjID);

		assert( itemDB != NULL );

		lua_pushstring(L, tmp);													// name
		lua_pushstring(L, ItemUTIL::GetItemIcon(&(loot->item)));				// image file
		lua_pushnumber(L, loot->item.Count);									// count
		lua_pushnumber(L, g_ObjectData->GetItemQuality(itemDB, &(loot->item)));	// quality
		lua_pushnumber(L, loot->time);											// ³Ñ©ó®É¶¡
		return 5;
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetLootAssignItemSize(lua_State* L)
{
	lua_pushnumber( L, (lua_Number)NetCli_PartyChild::s_assignItem.size() );
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetLootAssignItemInfo(lua_State* L)
{	
	if ( NetCli_PartyChild::IsAssigner() )
	{
		int index = luaL_checkint(L, 1) - 1;
		if ( index >= 0 && index < (int)NetCli_PartyChild::s_assignItem.size() )
		{
			char tmp[MAX_PATH];
			PartyAssignItem& assign = NetCli_PartyChild::s_assignItem[index];				
			ItemFieldStruct* item = &assign.item;
			RoleDataEx::GetItemFieldName(item, tmp, MAX_PATH);
			GameObjDbStruct* itemDB = CNetGlobal::GetObj(item->OrgObjID);

			assert( itemDB != NULL );

			lua_pushnumber(L, assign.itemVersion);			// item version id
			lua_pushstring(L, tmp);							// name

			GameObjDbStruct* imageDB = CNetGlobal::GetObj(itemDB->ImageID);
			if ( imageDB )
			{
				lua_pushstring(L, imageDB->Image.ACTField);	// image file
			}
			else
			{
				lua_pushstring(L, "");						// image file
			}
			
			lua_pushnumber(L, item->GetCount());			// count
			lua_pushnumber(L, g_ObjectData->GetItemQuality(itemDB, item));		// quality
			return 5;
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetLootAssignMember(lua_State* L)
{	
	lua_pushstring( L, NetCli_PartyChild::GetAssignItemMember(luaL_checkint(L, 1), luaL_checkint(L, 2) - 1) );
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_SetLootAssignMember(lua_State* L)
{
	PartyBaseInfoStruct& info = NetCli_PartyChild::GetPartyInfo();
	switch (info.ItemShareType)
	{
	case EM_PartyItemShare_Assign:
		NetCli_PartyChild::SetLootMethod( EM_PartyItemShare_Assign, luaL_checkstring(L, 1) );
		break;
	}	
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_SetLootMethod(lua_State* L)
{
	const char* method = luaL_checkstring(L, 1);

	// «ü©w¤À°tªÌ
	if ( strcmp(method, "master") == 0 )
	{
		const char* name = NULL;
		if ( lua_isstring(L, 2) )
			name = lua_tostring(L, 2);
		NetCli_PartyChild::SetLootMethod( EM_PartyItemShare_Assign, name );
	}
	// ÂY»ë¦Û¥Ñ¨ú±o
	else if ( strcmp(method, "freeforall") == 0 )
	{
		NetCli_PartyChild::SetLootMethod( EM_PartyItemShare_Roll );
	}
	// ÂY»ë½ü¬y¨ú±o
	else if ( strcmp(method, "alternate") == 0 )
	{
		NetCli_PartyChild::SetLootMethod( EM_PartyItemShare_InOrder );
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetLootMethod(lua_State* L)
{
	PartyBaseInfoStruct& info = NetCli_PartyChild::GetPartyInfo();
	switch( info.ItemShareType )
	{
	case EM_PartyItemShare_Roll:
		lua_pushstring(L, "freeforall");
		break;
	case EM_PartyItemShare_Assign:
		lua_pushstring(L, "master");
		break;
	case EM_PartyItemShare_InOrder:
		lua_pushstring(L, "alternate");
		break;
	default:
		lua_pushstring(L, "alternate");
		break;
	}
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_SetLootThreshold(lua_State* L)
{
	int quality = luaL_checkint(L, 1);
	NetCli_PartyChild::SetLootThreshold(quality);
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetLootThreshold(lua_State* L)
{
	PartyBaseInfoStruct& info = NetCli_PartyChild::GetPartyInfo();
	lua_pushnumber(L, info.RollTypeLV);
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetBootyNumItems	(lua_State* L)
{
	lua_pushnumber(L, NetCli_Treasure::GetBootyCount());
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetBootyItemInfo	(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;	

	ItemFieldStruct* item = NetCli_Treasure::GetBootyItem(index);
	if ( item && !item->IsEmpty() )
	{
		char itemName[MAX_PATH];
		g_ObjectData->GetItemFieldName(item, itemName, MAX_PATH);

		lua_pushstring(L, ItemUTIL::GetItemIcon(item));
		lua_pushstring(L, itemName);
		lua_pushnumber(L, item->GetCount());
		lua_pushnumber(L, g_ObjectData->GetItemQuality(NULL, item));
		return 4;
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetBootyItemLink	(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;

	ItemFieldStruct* item = NetCli_Treasure::GetBootyItem(index);
	if ( item && !item->IsEmpty() )
	{
		string link;
		if( ItemField_To_ItemLink( item, link ) )
		{
			lua_pushstring(L, link.c_str() );
			return 1;
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_ClickBootyItem		(lua_State* L)
{
	RoleDataEx* pRole = CNetGlobal::RoleData();
	int index = luaL_checkint(L, 1) - 1;

	// ¬É­±¥¼¶}±Ò
	if ( g_pLootFrame->GetBootyID() == 0 )
		return 0;

	ItemFieldStruct* item = NetCli_Treasure::GetBootyItem(index);
	if ( item && !item->IsEmpty() && item->Pos == EM_ItemState_NONE )
	{
		// ·j´M¬O§_¬°°ß¤@ª««~
		GameObjDbStructEx* itemDB = g_ObjectData->GetObj(item->OrgObjID);
		if ( itemDB && itemDB->Mode.Unique && CNetGlobal::RoleData()->CheckItemInBody(item->OrgObjID) )
		{
			int quality = 0;
			quality = g_ObjectData->GetItemQuality(itemDB, item);

			// ¦³¶¤¥î¥B¤À°t¤è¦¡¬O«ü©w¤À°t»P½ü¬yÀË¨ú
			PartyBaseInfoStruct& info = NetCli_PartyChild::GetPartyInfo();
			if ( info.PartyID >= 0 && (info.ItemShareType == EM_PartyItemShare_Roll || info.ItemShareType == EM_PartyItemShare_Assign) && quality >= info.RollTypeLV )
			{

			}
			else
			{
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_ITEM_UNIQUE"));
				return 0;
			}
		}

		// ¬O§_¦³­­¨î³Ì¤j®³¨ú­Ó¼Æ
		if ( itemDB->IsItem() && itemDB->Item.BodyMaxCount > 0 )
		{
			int count = pRole->CalBodyItemCount( itemDB->GUID ) + pRole->CalBankItemCount( itemDB->GUID ) + pRole->CalEqItemCount( itemDB->GUID ) + pRole->CountItemFromQueue( itemDB->GUID );
			if ( count + item->Count > itemDB->Item.BodyMaxCount )
			{
				g_pGameMain->SendWarningMsgEx(g_ObjectData->GetString("SYS_ITEM_HAS_NUMBER"), itemDB->Item.BodyMaxCount);
				return 0;
			}
		}

		int pos = CNetGlobal::RoleData()->SearchBodyFirstFit(item, true, item->GetCount());
		if ( pos != -1 )
		{
			ItemFieldStruct* bagItem = CNetGlobal::RoleData()->GetBodyItem(pos);
			item->Pos = EM_ItemState_CliDisabled;
			bagItem->Pos = EM_ItemState_CliDisabled;
			NetCli_Treasure::SL_GetItem(g_pLootFrame->GetBootyID(), item->Serial, pos);
		}
		else 
		{
			// ¨S¦³ªÅ¶¡
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_BAG_ENOUGH_CAPACITY"));
		}		
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_CloseBooty			(lua_State* L)
{	
	g_pLootFrame->CloseBooty(true);
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_SetAutoTakeLoot(lua_State* L)
{
	g_pGameMain->SetAutoTakeLoot(lua_toboolean(L, 1) != 0);
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_IsAutoTakeLoot(lua_State* L)
{
	lua_pushboolean(L, g_pGameMain->GetAutoTakeLoot());
	return 1;
}