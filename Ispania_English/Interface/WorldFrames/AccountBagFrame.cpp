#include "../../netwaker_member/NetWakerClientInc.h"
#include "../../mainproc/GameMain.h"
#include "ItemUtil.h"
#include "AccountBagFrame.h"

CAccountBagFrame*				g_pAccountBagFrame = NULL;

int LuaFunc_GetAccountBagItemInfo(lua_State* L);
int LuaFunc_GetAccountBagNumItems(lua_State* L);
int LuaFunc_OpenAccountBag(lua_State* L);
int LuaFunc_CloseAccountBag(lua_State* L);
int LuaFunc_ClickAccountBagItem(lua_State* L);

CAccountBagFrame::CAccountBagFrame(CInterface* object)
	: CInterfaceBase(object)
{
	g_pAccountBagFrame = this;
}

CAccountBagFrame::~CAccountBagFrame()
{
	g_pAccountBagFrame = NULL;
}

void CAccountBagFrame::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();
	if ( L )
	{
		lua_register(L, "GetAccountBagItemInfo",	LuaFunc_GetAccountBagItemInfo);
		lua_register(L, "GetAccountBagNumItems",	LuaFunc_GetAccountBagNumItems);
		lua_register(L, "OpenAccountBag",			LuaFunc_OpenAccountBag);
		lua_register(L, "CloseAccountBag",			LuaFunc_CloseAccountBag);
		lua_register(L, "ClickAccountBagItem",		LuaFunc_ClickAccountBagItem);
	}
}

void CAccountBagFrame::BuildFrames()
{
}

void CAccountBagFrame::DestroyFrames()
{
	if ( NetCli_ItemChild::s_npcID > 0 ) {
		NetCli_Item::SL_AccountBagClose();
		NetCli_ItemChild::ClearAccountBag();
	}
}

void CAccountBagFrame::Update(float elapsedTime)
{
	int npcID = NetCli_ItemChild::GetAccountBagNPCID();

	// 檢查是否需要關閉
	if ( npcID > 0 && g_pGameMain->GetDistanceWithPlayer(NetCli_ItemChild::s_npcID) > _MAX_ROLE_TOUCH_MAXRANGE_ )
	{
		SendWorldEvent("ACCOUNTBAG_CLOSE");
	}
}

int LuaFunc_OpenAccountBag(lua_State* L)
{
	int npcID = NetCli_ItemChild::GetAccountBagNPCID();

	if ( npcID > 0 )
	{
		g_pAccountBagFrame->SendWorldEvent("ACCOUNTBAG_UPDATE");
	}
	return 0;
}

int LuaFunc_CloseAccountBag(lua_State* L)
{
	int npcID = NetCli_ItemChild::GetAccountBagNPCID();

	if ( npcID > 0 ) {
		NetCli_Item::SL_AccountBagClose();
		NetCli_ItemChild::ClearAccountBag();
	}
	return 0;
}

int LuaFunc_GetAccountBagItemInfo(lua_State* L)
{
	// 索引編號
	int index = luaL_checkint(L, 1) - 1;

	// 檢查是否有物品資料
	NetAccountBagItem* accountItem = NetCli_ItemChild::GetAccountBagItemInfo(index);
	if ( accountItem == NULL )
		return 0;

	// 檢查是否有物品資料
	if ( accountItem->DBID <= 0 || accountItem->item.IsEmpty() )
		return 0;

	GameObjDbStructEx* itemDB = CNetGlobal::GetObj(accountItem->item.OrgObjID);
	if ( itemDB == NULL )
		return 0;

	ItemFieldStruct* item = &(accountItem->item);
	lua_pushstring(L, ItemUTIL::GetItemIcon(item));				// push item texture file
	lua_pushstring(L, itemDB->GetName());							// push Item name
	lua_pushnumber(L, item->GetCount());						// push item count
	lua_pushboolean(L, item->Pos != EM_ItemState_NONE);			// push item is locked
	return 4;
}

int LuaFunc_GetAccountBagNumItems(lua_State* L)
{
	lua_pushnumber(L, NetCli_ItemChild::GetAccountBagNumItems());
	return 1;
}

int LuaFunc_ClickAccountBagItem(lua_State* L)
{
	// 索引編號
	int index = luaL_checkint(L, 1) - 1;

	// 檢查是否有物品資料
	NetAccountBagItem* accountItem = NetCli_ItemChild::GetAccountBagItemInfo(index);
	if ( accountItem == NULL )
		return 0;

	// 檢查是否有物品資料
	if ( accountItem->DBID <= 0 || accountItem->item.IsEmpty() || accountItem->item.Pos != EM_ItemState_NONE )
		return 0;

	// 搜尋空格
	int destPos = CNetGlobal::RoleData()->SearchBodyFirstFit(NULL, false);		// 找尋一個可以插入背包的位置
	if ( destPos != -1 )
	{
		ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(destPos);
		item->Pos = EM_ItemState_CliDisabled;							// Client鎖定 
		NetCli_Item::SL_AccountBagGetItemRequest(destPos, accountItem->DBID);
		g_pAccountBagFrame->SendWorldEvent("ACCOUNTBAG_UPDATE");
		return 0;
	}

	// 沒有空間
	g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_ACCOUNTBAG_ENOUGH_CAPACITY"));
	return 0;
}