#include <time.h>
#include "../MainProc/GameMain.h"
#include "../../netwaker_member/NetWakerClientInc.h"
#include "../Login/CharacterSelect.h"
#include "ItemUtil.h"
#include "ChatFrame.h"
#include "ItemClipboard.h"
#include "BagFrame.h"
#include "AuctionFrame.h"
#include "../ItemLink/ItemLink.h"

#define MAX_AUCTION_FILTER_ITEMS						16

CAuctionFrame*			g_pAuctionFrame = NULL;

int LuaFunc_ClickAuctionItemButton		(lua_State* L);
int	LuaFunc_SetSellMoneyType			(lua_State* L);
int	LuaFunc_GetSellMoneyType			(lua_State* L);
int	LuaFunc_GetAuctionItem				(lua_State* L);
int LuaFunc_GetAuctionNumBrowseItems	(lua_State* L);
int LuaFunc_GetAuctionBrowseFilterList	(lua_State* L);
int LuaFunc_GetAuctionBrowseFilterMaxItems	(lua_State* L);
int LuaFunc_GetAuctionNumSellItems		(lua_State* L);
int LuaFunc_GetAuctionBrowseItemInfo	(lua_State* L);
int LuaFunc_GetAuctionSellItemInfo		(lua_State* L);
int	LuaFunc_GetAuctionNumBidItems		(lua_State* L);
int LuaFunc_GetAuctionBidItemInfo		(lua_State* L);
int	LuaFunc_CreateAuctionItem			(lua_State* L);
int	LuaFunc_CreateAuctionMoney			(lua_State* L);
int LuaFunc_AuctionCancelSell			(lua_State* L);
int LuaFunc_AuctionClose				(lua_State* L);
int	LuaFunc_AuctionBrowseSearchItem		(lua_State* L);
int LuaFunc_AuctionBrowseBuyItem		(lua_State* L);
int LuaFunc_AuctionBidBuyItem			(lua_State* L);
int	LuaFunc_GetAuctionBrowseMaxItems	(lua_State* L);
int	LuaFunc_GetAuctionBrowseMaxPages	(lua_State* L);

int LuaFunc_AuctionBrowsePervPage		(lua_State* L);
int LuaFunc_AuctionBrowseNextPage		(lua_State* L);
int LuaFunc_GetAuctionHistoryItemNums	(lua_State* L);
int LuaFunc_GetAuctionHistoryItemInfo	(lua_State* L);
int LuaFunc_AuctionBrowseHistoryRequest	(lua_State* L);
int LuaFunc_AuctionBidHistoryRequest	(lua_State* L);
int LuaFunc_AuctionItemHistoryRequest	(lua_State* L);

//ItemLink
int LuaFunc_GetAuctionBrowseItemLink	(lua_State* L);
int LuaFunc_IsAuctionItemAccountMoneyTrade	(lua_State* L);
int LuaFunc_IsAuctionAccountMoneyTrade	(lua_State* L);

// ----------------------------------------------------------------------------
CAuctionFrame::CAuctionFrame(CInterface* object)
	: CInterfaceBase(object)
{
	m_auctionItemIndex	= -1;
	m_sellMoneyType		= -1;
	m_lastTime			= 0.0f;
	m_searchWaitTime	= 0.0f;
	m_npcID				= 0;

	g_pAuctionFrame		= this;
}

// ----------------------------------------------------------------------------
CAuctionFrame::~CAuctionFrame()
{
	g_pAuctionFrame		= NULL;
}

// ----------------------------------------------------------------------------
void CAuctionFrame::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{
		// general
		lua_register(L, "AuctionClose",					LuaFunc_AuctionClose);

		// browse tab
		lua_register(L, "AuctionBrowseSearchItem",		LuaFunc_AuctionBrowseSearchItem);
		lua_register(L, "GetAuctionBrowseFilterList",	LuaFunc_GetAuctionBrowseFilterList);
		lua_register(L, "GetAuctionBrowseFilterMaxItems",	LuaFunc_GetAuctionBrowseFilterMaxItems);
		lua_register(L, "GetAuctionNumBrowseItems",		LuaFunc_GetAuctionNumBrowseItems);
		lua_register(L, "GetAuctionBrowseItemInfo",		LuaFunc_GetAuctionBrowseItemInfo);
		lua_register(L, "AuctionBrowseBuyItem",			LuaFunc_AuctionBrowseBuyItem);
		lua_register(L, "GetAuctionBrowseMaxItems",		LuaFunc_GetAuctionBrowseMaxItems);
		lua_register(L, "GetAuctionBrowseMaxPages",		LuaFunc_GetAuctionBrowseMaxPages);
		lua_register(L, "AuctionBrowsePervPage",		LuaFunc_AuctionBrowsePervPage);
		lua_register(L, "AuctionBrowseNextPage",		LuaFunc_AuctionBrowseNextPage);
		lua_register(L, "AuctionBrowseHistoryRequest",	LuaFunc_AuctionBrowseHistoryRequest);
		lua_register(L, "GetAuctionHistoryItemNums",	LuaFunc_GetAuctionHistoryItemNums);
		lua_register(L, "GetAuctionHistoryItemInfo",	LuaFunc_GetAuctionHistoryItemInfo);

		// bid tab
		lua_register(L, "GetAuctionNumBidItems",		LuaFunc_GetAuctionNumBidItems);
		lua_register(L, "GetAuctionBidItemInfo",		LuaFunc_GetAuctionBidItemInfo);
		lua_register(L, "AuctionBidBuyItem",			LuaFunc_AuctionBidBuyItem);
		lua_register(L, "AuctionBidHistoryRequest",		LuaFunc_AuctionBidHistoryRequest);

		// sell tab
		lua_register(L, "ClickAuctionItemButton",		LuaFunc_ClickAuctionItemButton);
		lua_register(L, "SetSellMoneyType",				LuaFunc_SetSellMoneyType);
		lua_register(L, "GetSellMoneyType",				LuaFunc_GetSellMoneyType);
		lua_register(L, "GetAuctionNumSellItems",		LuaFunc_GetAuctionNumSellItems);
		lua_register(L, "GetAuctionSellItemInfo",		LuaFunc_GetAuctionSellItemInfo);
		lua_register(L, "GetAuctionItem",				LuaFunc_GetAuctionItem);
		lua_register(L, "CreateAuctionItem",			LuaFunc_CreateAuctionItem);
		lua_register(L, "CreateAuctionMoney",			LuaFunc_CreateAuctionMoney);
		lua_register(L, "AuctionCancelSell",			LuaFunc_AuctionCancelSell);
		lua_register(L, "AuctionItemHistoryRequest",	LuaFunc_AuctionItemHistoryRequest);

		//ItemLink
		lua_register(L, "GetAuctionBrowseItemLink",		LuaFunc_GetAuctionBrowseItemLink);

		lua_register(L, "IsAuctionItemAccountMoneyTrade",		LuaFunc_IsAuctionItemAccountMoneyTrade);
		lua_register(L, "IsAuctionAccountMoneyTrade",	LuaFunc_IsAuctionAccountMoneyTrade);
	}
}

// ----------------------------------------------------------------------------
void CAuctionFrame::Update(float elapsedTime)
{
	m_lastTime += elapsedTime;
	if ( m_lastTime > 1.0f )
	{
		m_lastTime = 0.0f;
		if ( NetCli_ACChild::s_isUpdateBuyList )
		{
			NetCli_ACChild::s_isUpdateBuyList = false;
			SendWorldEvent("AUCTION_BUY_UPDATE");
		}
		if ( NetCli_ACChild::s_isUpdateSellList )
		{
			NetCli_ACChild::s_isUpdateSellList = false;
			SendWorldEvent("AUCTION_SELL_UPDATE");
		}		
	}

	if ( m_searchWaitTime > 0.001f )
	{
		m_searchWaitTime -= elapsedTime;
		if ( m_searchWaitTime < 0.001f )
		{
			ClearSearchWaitTime();
		}
	}

	if ( m_npcID && g_pGameMain->GetDistanceWithPlayer(m_npcID) > _MAX_ROLE_TOUCH_MAXRANGE_ )
	{
		SendWorldEvent("AUCTION_CLOSE");
	}
}

// ----------------------------------------------------------------------------
void CAuctionFrame::Pickup()
{
	int itemIndex = m_auctionItemIndex;	

	SetAuctionItemIndex(-1);

	g_pBagFrame->Cut(itemIndex);
}

// ----------------------------------------------------------------------------
void CAuctionFrame::PutDown(int index)
{	
	if ( m_auctionItemIndex != -1 ) {
		// 拿取上一個舊物品
		Pickup();
	} else {
		// 更新物品不是背包鎖定
		g_pBagFrame->ClearPickupItemIndex();
	}

	// 鎖定該物品
	g_pBagFrame->Locked(index);

	// 設定販賣物品
	SetAuctionItemIndex(index);
}

// ----------------------------------------------------------------------------
void CAuctionFrame::OpenAC(int npcID)
{
	m_npcID = npcID;
	g_pAuctionFrame->SendWorldEvent("AUCTION_OPEN");
}

// ----------------------------------------------------------------------------
void CAuctionFrame::CloseAC()
{
	SetAuctionItemIndex(-1);
	if ( m_npcID )
	{
		m_npcID = 0;
		NetCli_ACChild::SL_CloseAC();
	}
}

// ----------------------------------------------------------------------------
void CAuctionFrame::SetAuctionItemIndex(int index)
{
	// 清除
	if ( index == -1 )
		SetSellMoneyType(-1);

	if ( m_auctionItemIndex != index )
	{
		g_pBagFrame->Unlocked(m_auctionItemIndex);
		m_auctionItemIndex = index;
		SendWorldEvent("AUCTION_AUCTION_INFO_UPDATE");
	} else if ( NetCli_ACChild::s_createPrice > 0 )
	{
		SendWorldEvent("AUCTION_AUCTION_MONEY_UPDATE");
	}
}

// ----------------------------------------------------------------------------
void CAuctionFrame::ClearSearchWaitTime()
{
	m_searchWaitTime = 0.0f;
	SendWorldEvent("AUCTION_SEARCH_RESULT");
}

// ----------------------------------------------------------------------------
int CAuctionFrame::TimeTypeToMinute(int type)
{
	int time = 0;
	switch ( type )
	{
	case 1:
		time = 12 * 60;
		break;

	case 2:
		time = 24 * 60;
		break;

	case 3:
		time = 48 * 60;
		break;

	case 4:
		time = 72 * 60;
		break;
	}
	return time;
}

// ----------------------------------------------------------------------------
int LuaFunc_ClickAuctionItemButton(lua_State* L)
{
	ClipboardItemInfo itemInfo = g_pItemClipboard->GetItemInfo();

	switch ( g_pItemClipboard->GetType() )
	{
	// pickup item
	case eClipboardItem_Type_None:
		g_pAuctionFrame->Pickup();
		break;

	//
	case eClipboardItem_Type_Bag:
		g_pItemClipboard->Clear();
		g_pAuctionFrame->PutDown(itemInfo.bag.pos);
		break;

	default:
		break;
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_SetSellMoneyType(lua_State* L)
{
	int type = -1;
	if ( lua_isnumber(L, 1) )
	{
		switch ((int)lua_tonumber(L, 1))
		{
		case 1:
			type = EM_ACPriceType_GameMoney;
			break;

		case 2:
			type = EM_ACPriceType_AccountMoney;
			break;
		}
	}
	g_pAuctionFrame->SetSellMoneyType(type);
	return 0;
}

// ----------------------------------------------------------------------------
int	LuaFunc_GetSellMoneyType(lua_State* L)
{
	int type = g_pAuctionFrame->GetSellMoneyType();
	if ( type == -1 )
		lua_pushnil(L);
	else
		lua_pushnumber(L, type);
	return 1;
}

// ----------------------------------------------------------------------------
int	LuaFunc_GetAuctionItem(lua_State* L)
{
	const char* name = "";
	const char* texture = "";
	int stack = 0;
	int sellPrice = 0;

	int index = g_pAuctionFrame->GetAuctionItemIndex();
	ItemFieldStruct* itemData = CNetGlobal::RoleData()->GetBodyItem(index);
	if ( itemData )
	{
		char temp[MAX_PATH];
		GameObjDbStruct* itemDB = CNetGlobal::GetObj(itemData->OrgObjID);
		RoleDataEx::GetItemFieldName( itemData, temp, MAX_PATH );
		if (itemDB)
		{
			name		= temp;
			texture		= ItemUTIL::GetItemIcon(itemData);
			stack		= itemData->Count;
			sellPrice	= itemDB->Cost;
			lua_pushstring(L, name);
			lua_pushstring(L, texture);
			lua_pushnumber(L, max(stack, 1));
			lua_pushnumber(L, sellPrice);
			return 4;
		}
	}	
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetAuctionNumBrowseItems(lua_State* L)
{
	lua_pushnumber( L, NetCli_ACChild::GetBrowseNumItems());
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetAuctionBrowseFilterList(lua_State* L)
{	
	char tmp[MAX_PATH];	
	const char* str;
	int arg1, arg2;
	int count = 0;

	switch ( lua_gettop(L) )
	{
	case 0:
		for ( int i = 0; i < MAX_AUCTION_FILTER_ITEMS; i++ )
		{
			sprintf( tmp, "AC_ITEMTYPENAME_%d", i );
			str = g_ObjectData->GetString(tmp);
			if ( strlen(str) > 0 )
			{
				lua_pushstring(L, str);
				count++;
			}
		}
		break;

	case 1:
		arg1 = luaL_checkint(L, 1) - 1;
		{
			int loop = arg1;
			for ( int i = 0; loop >= 0 ; i++ ) {
				sprintf( tmp, "AC_ITEMTYPENAME_%d", i );
				str = g_ObjectData->GetString(tmp);
				if ( strlen(str) > 0 ) {
					loop--;
				} else {
					arg1++;
				}
			}
		}

		for ( int i = 0; i < MAX_AUCTION_FILTER_ITEMS; i++ )
		{
			sprintf( tmp, "AC_ITEMTYPENAME_%d_%d", arg1, i );
			str = g_ObjectData->GetString(tmp);
			if ( strlen(str) > 0 )
			{
				lua_pushstring(L, str);
				count++;
			}
		}
		break;

	case 2:
		arg1 = luaL_checkint(L, 1) - 1;
		arg2 = luaL_checkint(L, 2) - 1;

		{
			int loop = arg1;
			for ( int i = 0; loop >= 0 ; i++ ) {
				sprintf( tmp, "AC_ITEMTYPENAME_%d", i );
				str = g_ObjectData->GetString(tmp);
				if ( strlen(str) > 0 ) {
					loop--;
				} else {
					arg1++;
				}
			}

			loop = arg2;
			for ( int i = 0; loop >= 0 ; i++ ) {
				sprintf( tmp, "AC_ITEMTYPENAME_%d_%d", arg1, i );
				str = g_ObjectData->GetString(tmp);
				if ( strlen(str) > 0 ) {
					loop--;
				} else {
					arg2++;
				}
			}
		}

		for ( int i = 0; i < MAX_AUCTION_FILTER_ITEMS; i++ )
		{
			sprintf( tmp, "AC_ITEMTYPENAME_%d_%d_%d", arg1, arg2, i );
			str = g_ObjectData->GetString(tmp);
			if ( strlen(str) > 0 )
			{
				lua_pushstring(L, str);
				count++;
			}
		}
		break;
	}
	return count;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetAuctionBrowseFilterMaxItems(lua_State* L)
{
	char tmp[MAX_PATH];
	const char* str;	
	int arg[2];
	int count = 0;

	for ( int i = 0; i < MAX_AUCTION_FILTER_ITEMS; i++ )
	{
		sprintf( tmp, "AC_ITEMTYPENAME_%d", i );
		str = g_ObjectData->GetString(tmp);
		if ( strlen(str) > 0 )
			count++;
	}

	if ( lua_isnumber(L, 1) )
	{
		arg[0] = (int)lua_tonumber(L, 1) - 1;		

		// 跳過空的項目
		int loop = arg[0];
		for ( int i = 0; loop >= 0 ; i++ ) {
			sprintf( tmp, "AC_ITEMTYPENAME_%d", i );
			str = g_ObjectData->GetString(tmp);
			if ( strlen(str) > 0 ) {
				loop--;
			} else {
				arg[0]++;
			}
		}

		for ( int i = 0; i < MAX_AUCTION_FILTER_ITEMS; i++ )
		{
			sprintf( tmp, "AC_ITEMTYPENAME_%d_%d", arg[0], i );
			str = g_ObjectData->GetString(tmp);
			if ( strlen(str) > 0 )
				count++;
		}
		if ( lua_isnumber(L, 2) )
		{
			arg[1] = (int)lua_tonumber(L, 2) - 1;

			// 跳過空的項目
			int loop = arg[1];
			for ( int i = 0; loop >= 0 ; i++ ) {
				sprintf( tmp, "AC_ITEMTYPENAME_%d_%d", arg[0], i );
				str = g_ObjectData->GetString(tmp);
				if ( strlen(str) > 0 ) {
					loop--;
				} else {
					arg[1]++;
				}
			}

			for ( int i = 0; i < MAX_AUCTION_FILTER_ITEMS; i++ )
			{
				sprintf( tmp, "AC_ITEMTYPENAME_%d_%d_%d", arg[0], arg[1], i );
				str = g_ObjectData->GetString(tmp);
				if ( strlen(str) > 0 )
					count++;
			}
		}
	}

	lua_pushnumber(L, count);
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetAuctionNumSellItems(lua_State* L)
{
	lua_pushnumber(L, NetCli_ACChild::GetSellNumItems());
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_AuctionCancelSell(lua_State* L)
{
	AuctionSellItemInfo* info = NetCli_ACChild::GetSellItem(luaL_checkint(L, 1) - 1);
	if ( info )
		NetCli_ACChild::SL_CancelSellItem(info->data.DBID);
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetAuctionBrowseItemInfo(lua_State* L)
{
	int id = (luaL_checkint(L, 1) - 1) * 50 + (luaL_checkint(L, 2) - 1);

	AuctionBrowseItemInfo* info = NetCli_ACChild::GetBrowseItem(id);
	if ( info )
	{
		char temp[MAX_PATH];
		memset(temp, 0, sizeof(temp));

		ItemFieldStruct* itemData = &info->data.Item;
		GameObjDbStruct* itemDB = CNetGlobal::GetObj(itemData->OrgObjID);
		g_ObjectData->GetItemFieldName(itemData, temp, MAX_PATH);
		if ( itemDB )
		{
			lua_pushnumber(L, id);
			lua_pushstring(L, temp);
			lua_pushnumber(L, max(itemData->Count, 1));
			lua_pushnumber(L, g_ObjectData->GetItemQuality(itemDB, itemData));
			lua_pushstring(L, ItemUTIL::GetItemIcon(itemData));
			lua_pushnumber(L, itemDB->Limit.Level);
			lua_pushnumber(L, info->data.LiveTime);
			lua_pushstring(L, info->sellerName.c_str());
			lua_pushboolean(L, info->isBuyer);

			// 幣值類型
			switch (info->data.Type)
			{
			case EM_ACPriceType_GameMoney:
				lua_pushstring(L, MONEY_MODE_COPPER);
				break;

			case EM_ACPriceType_AccountMoney:
				lua_pushstring(L, MONEY_MODE_ACCOUNT);
				break;

			case EM_ACPriceType_BonusMoney:
				lua_pushstring(L, MONEY_MODE_BONUS);
				break;
			}

			// Bid Price
			lua_pushnumber(L, info->data.NowPrices);

			// Buyout Price
			if ( info->data.BuyOutPrices > _MAX_MONEY_COUNT_ )
				lua_pushnumber(L, 0);
			else
				lua_pushnumber(L, info->data.BuyOutPrices);
			return 12;
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------
int	LuaFunc_GetAuctionNumBidItems(lua_State* L)
{
	lua_pushnumber(L, NetCli_ACChild::GetBuyNumItems());
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetAuctionBidItemInfo(lua_State* L)
{
	AuctionBuyItemInfo* info = NetCli_ACChild::GetBuyItem(luaL_checkint(L, 1) - 1);
	if ( info )
	{
		char temp[MAX_PATH];
		memset(temp, 0, sizeof(temp));

		ItemFieldStruct* itemData = &info->data.Item;
		GameObjDbStruct* itemDB = CNetGlobal::GetObj(itemData->OrgObjID);
		g_ObjectData->GetItemFieldName(itemData, temp, MAX_PATH);
		if ( itemDB )
		{
			lua_pushstring(L, temp);
			lua_pushnumber(L, max(itemData->Count, 1));
			lua_pushnumber(L, g_ObjectData->GetItemQuality(itemDB, itemData));
			lua_pushstring(L, ItemUTIL::GetItemIcon(itemData));
			lua_pushnumber(L, itemDB->Limit.Level);
			lua_pushnumber(L, info->data.LiveTime);
			lua_pushstring(L, info->sellerName.c_str());
			lua_pushboolean(L, info->isHigher);

			// 幣值類型
			switch (info->data.Type)
			{
			case EM_ACPriceType_GameMoney:
				lua_pushstring(L, MONEY_MODE_COPPER);
				break;

			case EM_ACPriceType_AccountMoney:
				lua_pushstring(L, MONEY_MODE_ACCOUNT);
				break;

			case EM_ACPriceType_BonusMoney:
				lua_pushstring(L, MONEY_MODE_BONUS);
				break;
			}

			lua_pushnumber(L, info->data.NowPrices);
			if ( info->data.BuyOutPrices > _MAX_MONEY_COUNT_ )
				lua_pushnumber(L, 0);
			else
				lua_pushnumber(L, info->data.BuyOutPrices);
			return 11;
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetAuctionSellItemInfo(lua_State* L)
{
	AuctionSellItemInfo* info = NetCli_ACChild::GetSellItem(luaL_checkint(L, 1) - 1);
	if ( info )
	{
		char temp[MAX_PATH];
		memset(temp, 0, sizeof(temp));

		ItemFieldStruct* itemData = &info->data.Item;
		GameObjDbStruct* itemDB = CNetGlobal::GetObj(itemData->OrgObjID);
		g_ObjectData->GetItemFieldName(itemData, temp, MAX_PATH);
		if ( itemDB )
		{
			lua_pushstring(L, temp);
			lua_pushnumber(L, max(itemData->Count, 1));
			lua_pushnumber(L, g_ObjectData->GetItemQuality(itemDB, itemData));
			lua_pushstring(L, ItemUTIL::GetItemIcon(itemData));
			lua_pushnumber(L, itemDB->Limit.Level);
			lua_pushnumber(L, info->data.LiveTime);
			lua_pushstring(L, info->buyerName.c_str());

			// 幣值類型
			switch (info->data.Type)
			{
			case EM_ACPriceType_GameMoney:
				lua_pushstring(L, MONEY_MODE_COPPER);
				break;

			case EM_ACPriceType_AccountMoney:
				lua_pushstring(L, MONEY_MODE_ACCOUNT);
				break;

			case EM_ACPriceType_BonusMoney:
				lua_pushstring(L, MONEY_MODE_BONUS);
				break;
			}

			lua_pushnumber(L, info->data.NowPrices);
			if ( info->data.BuyOutPrices > _MAX_MONEY_COUNT_ )
				lua_pushnumber(L, 0);
			else
				lua_pushnumber(L, info->data.BuyOutPrices);
			return 10;
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_CreateAuctionItem(lua_State* L)
{
	int index = g_pAuctionFrame->GetAuctionItemIndex();	
	if ( index != -1 )
	{
		ItemFieldStruct* itemData = CNetGlobal::RoleData()->GetBodyItem(index);
		if ( itemData == NULL || itemData->IsEmpty() )
			return 0;

		GameObjDbStructEx* itemDB = g_ObjectData->GetObj(itemData->OrgObjID);
		int time = g_pAuctionFrame->TimeTypeToMinute(luaL_checkint(L, 1));
		int price = luaL_checkint(L, 2);
		int buyout = 0;
		PriceTypeENUM moneyType;

		switch ( luaL_checkint(L, 3) )
		{
		case 1:
			moneyType = EM_ACPriceType_GameMoney;
			break;

		case 2:
			if ( Net_GSrvConnectChild::s_zoneMode.AcAccountMoneyTrade == false && Net_GSrvConnectChild::s_zoneMode.AcItemAccountMoneyTrade == false )
			{
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_AUCTION_CREATE_MONEY_DISABLE"));
				return 0;
			}
			moneyType = EM_ACPriceType_AccountMoney;
			break;
		}

		// 幣值為零
		if (price <= 0 || price > _MAX_MONEY_COUNT_)
		{
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_AUCTION_CREATE_MONEY_ERROR"));
			return 0;
		}

		if ( lua_isnumber(L, 4) )
			buyout = (int)lua_tonumber(L, 4);

		if ( buyout < price )
			buyout = _MAX_MONEY_COUNT_;

		/*
		// 物品耐久度
		switch (itemDB->Item.Class)
		{
		case EM_Item_Weapon:
		case EM_Item_Armor:
			if ( itemDB->MaxHeap <= 1 && itemData->Durable < itemDB->Item.Durable * itemData->Quality ) {
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_AC_CREATE_DURABLE_ERROR"));
				return 0;
			}
			break;
		}
		*/

		//數量到達上限
		if ( NetCli_ACChild::GetSellNumItems() >= _MAX_AC_SELL_ITEM_COUNT_ ) {
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_AC_CREATE_ITEM_MAXCOUNT"));
			return 0;
		}
		
		if ( itemData ) {
			if ( !itemData->Mode.Trade ) {
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_AC_CREATE_SOULBOUND"));
				return 0;
			}
			NetCli_ACChild::SL_SellItem(index, *itemData, price , buyout, moneyType , time);
		}
	}

	return 0;
}

// ----------------------------------------------------------------------------
int	LuaFunc_CreateAuctionMoney(lua_State* L)
{
	int time = g_pAuctionFrame->TimeTypeToMinute(luaL_checkint(L, 1));

	PriceTypeENUM moneyType, sellMoneyType;
	switch ( luaL_checkint(L, 2) )
	{
	case 1:
		sellMoneyType = EM_ACPriceType_GameMoney;
		break;

	case 2:
		sellMoneyType = EM_ACPriceType_AccountMoney;
		break;
	}

	int sellMoney = luaL_checkint(L, 3);
	int price = luaL_checkint(L, 4);
	switch ( luaL_checkint(L, 5) )
	{
	case 1:
		moneyType = EM_ACPriceType_GameMoney;
		break;

	case 2:
		if ( Net_GSrvConnectChild::s_zoneMode.AcAccountMoneyTrade == false )
		{
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_AUCTION_CREATE_MONEY_DISABLE"));
			return 0;
		}
		moneyType = EM_ACPriceType_AccountMoney;
		break;
	}

	// 金錢數值錯誤
	if ( sellMoney <= 0 )
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_AUCTION_SELL_MONEY_ERROR"));
		return 0;
	}
	if ( price <= 0 )
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_AUCTION_CREATE_MONEY_ERROR"));
		return 0;
	}	

	int buyout = 0;
	if ( lua_isnumber(L, 6) )
		buyout = (int)lua_tonumber(L, 6);
	if ( buyout < price )
		buyout = _MAX_MONEY_COUNT_;		

	//數量到達上限
	if ( NetCli_ACChild::GetSellNumItems() >= _MAX_AC_SELL_ITEM_COUNT_ ) {
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_AC_CREATE_ITEM_MAXCOUNT"));
		return 0;
	}

	NetCli_ACChild::CreateMoney(sellMoneyType, sellMoney, price, buyout, time, moneyType, g_pGameMain->GetSecondPassword());
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_AuctionClose(lua_State* L)
{
	g_pAuctionFrame->CloseAC();
	return 0;
}

// ----------------------------------------------------------------------------
int	LuaFunc_AuctionBrowseSearchItem(lua_State* L)
{
	ACSearchConditionStruct data;
	RoleDataEx* role = CNetGlobal::RoleData();
	data.PriceType = EM_ACPriceType_None;
	int low = -1;
	int high = -1;

	// 重複搜尋需要等待時間
	if ( g_pAuctionFrame->GetSearchWaitTime() > 0.01f )
	{
		return 0;
	}

	if ( lua_isstring(L, 1) )
		data.SearchName = lua_tostring(L, 1);
	if ( lua_isnumber(L, 2) )
	{
		low = (int)lua_tonumber(L, 2);
		if ( low == 0 )
			low = -1;
	}
	if ( lua_isnumber(L, 3) )
	{
		high = (int)lua_tonumber(L, 3);
		if ( high == 0 || high > 100 )
			high = -1;
	}	

	data.Rare = -1;
	if ( lua_isnumber(L, 4) )
	{		
		data.Rare = (int)lua_tonumber(L, 4) - 1;
		if ( data.Rare == 0 )
			data.Rare = -1;
	}

	data.PriceType = EM_ACPriceType_None;
	if ( lua_isnumber(L, 5) )
	{
		int priceType = (int)(lua_tonumber(L, 5) - 2);
		if ( priceType == EM_ACPriceType_GameMoney ||
			 priceType == EM_ACPriceType_AccountMoney ||
			 priceType == EM_ACPriceType_BonusMoney )
			data.PriceType = (PriceTypeENUM)priceType;
	}

	data.RuneVolume = -1;
	if ( lua_isnumber(L, 6) )
	{
		data.RuneVolume = (int)lua_tonumber(L, 6) - 1;
		if ( data.RuneVolume == 0 )
			data.RuneVolume = -1;
	}

	if ( lua_toboolean(L, 7) )
	{
		switch ( role->BaseData.Sex )
		{
		case EM_Sex_Boy:
			data.Sex.Boy = true;
			break;

		case EM_Sex_Girl:
			data.Sex.Girl = true;
			break;

		default:
			data.Sex._Sex = -1;
			break;
		}

		switch ( role->BaseData.Race )
		{
		case EM_Race1:
			data.Race.Race0 = true;
			break;

		case EM_Race2:
			data.Race.Race1 = true;
			break;

		case EM_Race3:
			data.Race.Race2 = true;
			break;

		case EM_Race4:
			data.Race.Race3 = true;
			break;

		default:
			data.Race._Race = -1;
			break;
		}
		
		switch( role->BaseData.Voc )
		{
		case EM_Vocation_GameMaster:
			data.JobType.Voc0 = true;
			break;

		case EM_Vocation_Warrior:						// 戰士
			data.JobType.Voc1 = true;
			break;

		case EM_Vocation_Ranger:						// 弓箭手
			data.JobType.Voc2 = true;
			break;

		case EM_Vocation_Rogue:							// 盜賊
			data.JobType.Voc3 = true;
			break;

		case EM_Vocation_Wizard:						// 法師
			data.JobType.Voc4 = true;
			break;

		case EM_Vocation_Priest:						// 僧侶
			data.JobType.Voc5 = true;
			break;

		case EM_Vocation_Knight:						// 騎士
			data.JobType.Voc6 = true;
			break;

		case EM_Vocation_RuneDancer:					// 森林牧者
			data.JobType.Voc7 = true;
			break;

		case EM_Vocation_Druid:							// 自然神官
			data.JobType.Voc8 = true;
			break;

		case EM_Vocation_Harpsyn:						// 靈能者
			data.JobType.Voc9 = true;
			break;

		case EM_Vocation_Psyron:						// 符文工匠
			data.JobType.Voc10 = true;
			break;

		case EM_Vocation_Duelist:						// 鬥棋士
			data.JobType.Voc11 = true;
			break;

		default:
			data.JobType._Voc = -1;
			break;
		}
		data.SearchType._Skill = role->TempData.Attr.Fin.WearSkill._Skill;
		high = role->PlayerBaseData->AbilityList[role->BaseData.Voc].Level;
	}
	else
	{
		data.Sex._Sex = -1;
		data.Race._Race = -1;
		data.JobType._Voc = -1;
		data.SearchType._Skill = -1;
	}
	data.LevelLow = min(low, high);
	data.LevelHi = max(low, high);

	memset( data.Type, -1, sizeof(data.Type) );
	for ( int i = 0; i < 3; i++ )
	{
		if ( lua_isnumber(L, i + 8) )
			data.Type[i] = (int)lua_tonumber(L, i + 8) - 1;
		else
			break;
	}

	// 修正索引值,將空白索引項目跳過
	{
		char tmp[256];
		const char* str = NULL;
		if ( data.Type[0] >= 0 ) {
			int count = data.Type[0];

			for ( int i = 0;  count >= 0 ; i++ ) {
				sprintf( tmp, "AC_ITEMTYPENAME_%d", i );
				str = g_ObjectData->GetString(tmp);
				if ( strlen(str) > 0 ) {
					count--;
				} else {
					data.Type[0]++;
				}
			}		
		}

		if ( data.Type[1] >= 0 ) {
			int count = data.Type[1];

			for ( int i = 0;  count >= 0 ; i++ ) {
				sprintf( tmp, "AC_ITEMTYPENAME_%d_%d", data.Type[0], i );
				str = g_ObjectData->GetString(tmp);
				if ( strlen(str) > 0 ) {
					count--;
				} else {
					data.Type[1]++;
				}
			}
		}

		if ( data.Type[2] >= 0 ) {
			int count = data.Type[2];

			for ( int i = 0;  count >= 0 ; i++ ) {
				sprintf( tmp, "AC_ITEMTYPENAME_%d_%d_%d", data.Type[0], data.Type[1], i );
				str = g_ObjectData->GetString(tmp);
				if ( strlen(str) > 0 ) {
					count--;
				} else {
					data.Type[2]++;
				}
			}
		}
	}

	const char* language = g_pGameMain->GetLanguage();
	if ( stricmp(language, LANGUAGE_CODE_TW) == 0 )
		data.Language = EM_LanguageType_TW;
	else if ( stricmp(language, LANGUAGE_CODE_CN) == 0 )
		data.Language = EM_LanguageType_CN;
	else if ( stricmp(language, LANGUAGE_CODE_DE) == 0 )
		data.Language = EM_LanguageType_DE;
	else if ( stricmp(language, LANGUAGE_CODE_JP) == 0 )
		data.Language = EM_LanguageType_JP;
	else if ( stricmp(language, LANGUAGE_CODE_KR) == 0 )
		data.Language = EM_LanguageType_KR;
	else if ( stricmp(language, LANGUAGE_CODE_ENUS) == 0 )
		data.Language = EM_LanguageType_ENEU;
	else if ( stricmp(language, LANGUAGE_CODE_ENEU) == 0 )
		data.Language = EM_LanguageType_ENUS;
	else if ( stricmp(language, LANGUAGE_CODE_SG) == 0 )
		data.Language = EM_LanguageType_ENEU;
	else if ( stricmp(language, LANGUAGE_CODE_BR) == 0 )
		data.Language = EM_LanguageType_BR;
	else if ( stricmp(language, LANGUAGE_CODE_TR) == 0 )
		data.Language = EM_LanguageType_TR;
	else if ( stricmp(language, LANGUAGE_CODE_RU) == 0 )
		data.Language = EM_LanguageType_RU;
	else if ( stricmp(language, LANGUAGE_CODE_FR) == 0 )
		data.Language = EM_LanguageType_FR;
	else if ( stricmp(language, LANGUAGE_CODE_PT) == 0 )
		data.Language = EM_LanguageType_PT;
	else if ( stricmp(language, LANGUAGE_CODE_PL) == 0 )
		data.Language = EM_LanguageType_PL;
	else if ( stricmp(language, LANGUAGE_CODE_VN) == 0 )
		data.Language = EM_LanguageType_VN;
	else if ( stricmp(language, LANGUAGE_CODE_ES) == 0 )
		data.Language = EM_LanguageType_ES;
	else if ( stricmp(language, LANGUAGE_CODE_ID) == 0 )
		data.Language = EM_LanguageType_ID;
	//else if ( stricmp(language, LANGUAGE_CODE_NL) == 0 )
	//	data.Language = EM_LanguageType_NL;	
	//else if ( stricmp(language, LANGUAGE_CODE_RO) == 0 )
	//	data.Language = EM_LanguageType_RO;
	else if ( stricmp(language, LANGUAGE_CODE_PH) == 0 )
		data.Language = EM_LanguageType_PH;	
	else if ( stricmp(language, LANGUAGE_CODE_SAES) == 0 )
		data.Language = EM_LanguageType_SAES;	
	else if ( stricmp(language, LANGUAGE_CODE_SAPT) == 0 )
		data.Language = EM_LanguageType_SAPT;
	else if ( stricmp(language, LANGUAGE_CODE_AE) == 0 )
		data.Language = EM_LanguageType_ENEU;
	else if ( stricmp(language, LANGUAGE_CODE_ENAR) == 0 )	
		data.Language = EM_LanguageType_ENAR;
	else if ( stricmp(language, LANGUAGE_CODE_AUS) == 0 )	
		data.Language = EM_LanguageType_ENEU;
	else if ( stricmp(language, LANGUAGE_CODE_IT) == 0 )
		data.Language = EM_LanguageType_IT;
	else
		data.Language = EM_LanguageType_TW;

	g_pAuctionFrame->SetSearchWaitTime(4.0f);			// 需等待6秒後才可以在次搜尋
	NetCli_ACChild::SL_SearchItem(data);
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_AuctionBrowseBuyItem(lua_State* L)
{
	AuctionBrowseItemInfo* info = NetCli_ACChild::GetBrowseItem(luaL_checkint(L, 1));
	if ( info )
	{
		int price;

		// 不能競標自己的物品
		vector<string>& playerNameList = CNetCli_Login::GetPlayerNameList();
		for ( vector<string>::iterator iter = playerNameList.begin(); iter != playerNameList.end(); iter++ ) {
			if ( strcmp(iter->c_str(), info->sellerName.c_str()) == 0 ) {
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_AC_BID_SELF_ERROR"));
				return 0;
			}
		}

		price = info->data.BuyOutPrices;
		if ( lua_isnumber(L, 2) )
		{
			price = (int)lua_tonumber(L, 2);
			if ( price > info->data.BuyOutPrices )
				price = info->data.BuyOutPrices;
		}

		// 競標
		if ( price != info->data.BuyOutPrices )
		{
			// 若最高標者是自己
			if ( info->isBuyer )
			{
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_AC_BID_HIGHER"));
				return 0;
			}

			// 競標金額需高於底標
			if ( price < (int)(info->data.NowPrices * AC_BID_LIMIT_PERCENTAGE) )
			{
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_AC_BID_PERICES_TO_LOW"));
				return 0;
			}
		}

		switch ( info->data.Type )
		{
		case EM_ACPriceType_GameMoney:
			// 檢查玩家金錢
			if ( CNetGlobal::RoleData()->BodyMoney() < price )
			{
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_AC_NOT_ENOUGH_MONEY"));
				return 0;
			}
			break;

		case EM_ACPriceType_AccountMoney:
			// 檢查玩家帳號幣
			if ( CNetGlobal::RoleData()->PlayerBaseData->Body.Money_Account < price )
			{
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_AC_NOT_ENOUGH_RUNEMONEY"));
				return 0;
			}
			break;

		case EM_ACPriceType_BonusMoney:
			// 檢查玩家紅利點數
			if ( CNetGlobal::RoleData()->PlayerBaseData->Money_Bonus < price )
			{
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_AC_NOT_ENOUGH_BONUSMONEY"));
				return 0;
			}
			break;

		default:
			// 貨幣類型錯誤
			return 0;
		}

		const char* password = "";
		if ( lua_isstring(L, 3) )
			password = lua_tostring(L, 3);

		NetCli_ACChild::SL_BuyItem(info->data.DBID, price, info->data.Type, (char*)password);
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_AuctionBidBuyItem(lua_State* L)
{
	AuctionBuyItemInfo* info = NetCli_ACChild::GetBuyItem(luaL_checkint(L, 1) - 1);
	if ( info )
	{
		int price;

		price = info->data.BuyOutPrices;
		if ( lua_isnumber(L, 2) )
		{
			price = (int)lua_tonumber(L, 2);
			if ( price > info->data.BuyOutPrices )
				price = info->data.BuyOutPrices;
		}
		
		// 競標
		if ( price != info->data.BuyOutPrices )
		{
			// 最高標不能重複競標
			if ( info->isHigher )
			{
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_AC_BID_HIGHER"));
				return 0;
			}

			// 競標金額需高於底標
			if ( price < (int)(info->data.NowPrices * AC_BID_LIMIT_PERCENTAGE) )
			{
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_AC_BID_PERICES_TO_LOW"));
				return 0;
			}
		}

		switch ( info->data.Type )
		{		
		case EM_ACPriceType_GameMoney:
			// 檢查玩家金錢
			if ( CNetGlobal::RoleData()->BodyMoney() < price )
			{
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_AC_NOT_ENOUGH_MONEY"));
				return 0;
			}
			break;
		
		case EM_ACPriceType_AccountMoney:
			// 檢查玩家帳號幣
			if ( CNetGlobal::RoleData()->PlayerBaseData->Body.Money_Account < price )
			{
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_AC_NOT_ENOUGH_RUNEMONEY"));
				return 0;
			}
			break;

		case EM_ACPriceType_BonusMoney:
			// 檢查玩家紅利點數
			if ( CNetGlobal::RoleData()->PlayerBaseData->Money_Bonus < price )
			{
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_AC_NOT_ENOUGH_BONUSMONEY"));
				return 0;
			}
			break;

		default:
			// 貨幣類型錯誤
			return 0;
		}

		const char* password = "";
		if ( lua_isstring(L, 3) )
			password = lua_tostring(L, 3);

		NetCli_ACChild::SL_BuyItem(info->data.DBID, price, info->data.Type, (char*)password);
	}
	return 0;
}

// ----------------------------------------------------------------------------
int	LuaFunc_GetAuctionBrowseMaxItems(lua_State* L)
{
	lua_pushnumber(L, NetCli_ACChild::GetBrowseMaxNums());
	return 1;
}

// ----------------------------------------------------------------------------
int	LuaFunc_GetAuctionBrowseMaxPages(lua_State* L)
{
	lua_pushnumber(L, NetCli_ACChild::GetBrowseMaxPages());
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_AuctionBrowsePervPage(lua_State* L)
{
	g_pAuctionFrame->SendWorldEvent("AUCTION_BROWSE_UPDATE");
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_AuctionBrowseNextPage(lua_State* L)
{
	// 重複搜尋需要等待時間
	if ( g_pAuctionFrame->GetSearchWaitTime() > 0.01f )
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_AUCTION_WAIT_NEXT"));
		lua_pushboolean(L, false);
		return 1;
	}

	if ( NetCli_ACChild::GetBrowseNextCount() > 0 )
	{
		g_pAuctionFrame->SetSearchWaitTime(6.0f);
		NetCli_ACChild::SL_GetNextSearchItem();
	}
	else
		g_pAuctionFrame->SendWorldEvent("AUCTION_BROWSE_UPDATE");

	lua_pushboolean(L, true);
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetAuctionHistoryItemNums(lua_State* L)
{
	lua_pushnumber(L, NetCli_ACChild::GetHistoryNumItems());
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetAuctionHistoryItemInfo(lua_State* L)
{
	AuctionHistoryItemInfo* info = NetCli_ACChild::GetHistoryItem(luaL_checkint(L, 1) - 1);
	if ( info )
	{
		ItemFieldStruct* itemData = &(info->item);
		GameObjDbStruct* itemDB = CNetGlobal::GetObj(itemData->OrgObjID);
		if ( itemDB )
		{
			char itemName[MAX_PATH];
			memset(itemName, 0, sizeof(itemName));
			g_ObjectData->GetItemFieldName(itemData, itemName, MAX_PATH);

			struct tm* tmnow;
			time_t time = info->data;
			tmnow = localtime(&time);

			lua_pushstring(L, itemName);
			lua_pushnumber(L, max(itemData->Count, 1));
			lua_pushnumber(L, g_ObjectData->GetItemQuality(itemDB, itemData));
			lua_pushstring(L, ItemUTIL::GetItemIcon(itemData));
			lua_pushstring(L, info->SellerName.Begin());
			lua_pushstring(L, info->BuyerName.Begin());

			// 幣值類型
			switch (info->priceType)
			{
			case EM_ACPriceType_GameMoney:
				lua_pushstring(L, MONEY_MODE_COPPER);
				break;

			case EM_ACPriceType_AccountMoney:
				lua_pushstring(L, MONEY_MODE_ACCOUNT);
				break;

			case EM_ACPriceType_BonusMoney:
				lua_pushstring(L, MONEY_MODE_BONUS);
				break;

			default:
				lua_pushnil(L);
				break;
			}				

			lua_pushnumber(L, info->price);
			lua_pushnumber(L, tmnow->tm_mon+1);
			lua_pushnumber(L, tmnow->tm_mday);
			lua_pushnumber(L, tmnow->tm_hour);
			lua_pushnumber(L, tmnow->tm_min);
			return 12;
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_AuctionBrowseHistoryRequest(lua_State* L)
{
	AuctionBrowseItemInfo* info = NetCli_ACChild::GetBrowseItem(luaL_checkint(L, 1));
	if ( info )
		NetCli_ACChild::SL_ItemHistoryRequest(info->data.Item.OrgObjID);
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_AuctionBidHistoryRequest(lua_State* L)
{
	AuctionBuyItemInfo* info = NetCli_ACChild::GetBuyItem(luaL_checkint(L, 1) - 1);
	if ( info )
		NetCli_ACChild::SL_ItemHistoryRequest(info->data.Item.OrgObjID);
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_AuctionItemHistoryRequest	(lua_State* L)
{
	ItemFieldStruct* itemData = CNetGlobal::RoleData()->GetBodyItem(g_pAuctionFrame->GetAuctionItemIndex());
	if ( itemData )
	{
		NetCli_ACChild::SL_ItemHistoryRequest(itemData->OrgObjID);
	}
	else if ( g_pAuctionFrame->GetSellMoneyType() != -1 )
	{
		int itemID = g_ObjectData->GetMonetItemID((PriceTypeENUM)g_pAuctionFrame->GetSellMoneyType());
		if ( itemID > 0 )
		{
			NetCli_ACChild::SL_ItemHistoryRequest(itemID);
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetAuctionBrowseItemLink(lua_State* L)
{
	int id = luaL_checkint(L, 1) ;

	AuctionBrowseItemInfo* info = NetCli_ACChild::GetBrowseItem(id);

	if ( info )
	{
		ItemFieldStruct* itemData = &info->data.Item;
		string link;
		if( !ItemField_To_ItemLink( itemData, link ) )
		{
			return 0;
		}
		lua_pushstring(L, link.c_str() );
		return 1;
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_IsAuctionItemAccountMoneyTrade(lua_State* L)
{
	lua_pushboolean(L, Net_GSrvConnectChild::s_zoneMode.AcItemAccountMoneyTrade);
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_IsAuctionAccountMoneyTrade(lua_State* L)
{
	lua_pushboolean(L, Net_GSrvConnectChild::s_zoneMode.AcAccountMoneyTrade);
	return 1;
}