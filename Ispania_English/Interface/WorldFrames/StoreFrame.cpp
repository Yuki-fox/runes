#include "../netwaker_member/NetWakerClientInc.h"
#include "../mainproc/GameMain.h"
#include "ItemClipboard.h"
#include "StoreFrame.h"
#include "../ItemLink/ItemLink.h"

CStoreFrame*				g_pStoreFrame = NULL;

int	LuaFunc_GetStoreSellItemInfo		(lua_State* L);
int	LuaFunc_GetStoreSellItems			(lua_State* L);
int	LuaFunc_GetStoreBuyBackItemInfo		(lua_State* L);
int	LuaFunc_GetStoreBuyBackItems		(lua_State* L);
int LuaFunc_PickupStoreSellItem			(lua_State* L);
int	LuaFunc_PickupStoreBuyBackItem		(lua_State* L);
int LuaFunc_StoreBuyItem				(lua_State* L);
int	LuaFunc_StoreBuyBackItem			(lua_State* L);
int	LuaFunc_IsStoreCanFix				(lua_State* L);
int LuaFunc_GetEquipmentRepairMoney		(lua_State* L);
int LuaFunc_GetEquipmentRepairAllMoney	(lua_State* L);
int	LuaFunc_ClickRepairButton			(lua_State* L);
int LuaFunc_ClickRepairAllButton		(lua_State* L);
int	LuaFunc_CloseStore					(lua_State* L);

//ItemLink
int	LuaFunc_GetStoreSellItemLink		(lua_State* L);
int	LuaFunc_GetStoreBuyBackItemLink		(lua_State* L);


// ----------------------------------------------------------------------------
CStoreFrame::CStoreFrame(CInterface* object)
	: CInterfaceBase(object)
{
	m_shopId = 0;
	m_npcId = 0;
	m_count = 0;
	m_receiveClose = false;
	m_repairEnable = false;
	memset( m_itemCount, 0 , sizeof(m_itemCount) );
	memset( m_index, 0, sizeof(m_index) );

	m_queueID = 0;
	m_queueCount = 0;
	m_queueIndex = -1;
	m_lastSendBuyTime = 0.0f;

	g_pStoreFrame = this;
}

// ----------------------------------------------------------------------------
CStoreFrame::~CStoreFrame()
{
	g_pStoreFrame = NULL;
}

// ----------------------------------------------------------------------------
void CStoreFrame::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{
		lua_register( L, "GetStoreSellItemInfo",		LuaFunc_GetStoreSellItemInfo );
		lua_register( L, "GetStoreSellItems",			LuaFunc_GetStoreSellItems );
		lua_register( L, "GetStoreBuyBackItemInfo",		LuaFunc_GetStoreBuyBackItemInfo );
		lua_register( L, "GetStoreBuyBackItems",		LuaFunc_GetStoreBuyBackItems );
		lua_register( L, "PickupStoreSellItem",			LuaFunc_PickupStoreSellItem );
		lua_register( L, "PickupStoreBuyBackItem",		LuaFunc_PickupStoreBuyBackItem );
		lua_register( L, "StoreBuyItem",				LuaFunc_StoreBuyItem );
		lua_register( L, "StoreBuyBackItem",			LuaFunc_StoreBuyBackItem );
		lua_register( L, "IsStoreCanFix",				LuaFunc_IsStoreCanFix );
		lua_register( L, "GetEquipmentRepairMoney",		LuaFunc_GetEquipmentRepairMoney );
		lua_register( L, "GetEquipmentRepairAllMoney",	LuaFunc_GetEquipmentRepairAllMoney );
		lua_register( L, "ClickRepairButton",			LuaFunc_ClickRepairButton );
		lua_register( L, "ClickRepairAllButton",		LuaFunc_ClickRepairAllButton );
		lua_register( L, "CloseStore",					LuaFunc_CloseStore );

		lua_register( L, "GetStoreSellItemLink",		LuaFunc_GetStoreSellItemLink );
		lua_register( L, "GetStoreBuyBackItemLink",		LuaFunc_GetStoreBuyBackItemLink );

	}
}

// ----------------------------------------------------------------------------
void CStoreFrame::Update(float elapsedTime)
{
	if ( !IsOpen() )
		return;

	CRoleSprite* sprite = g_pGameMain->FindSpriteByID(m_npcId);

	// 檢查是否需要關閉
	if ( sprite == NULL || g_pGameMain->GetDistanceWithPlayer(sprite) > _MAX_ROLE_TOUCH_MAXRANGE_ + sprite->GetAttackRange() ) {
		SendWorldEvent("STORE_CLOSE");
	}

	// 是否有暫存購買物件
	if ( m_queueID != 0 && m_queueCount > 0 ) {
		if ( m_lastSendBuyTime + 0.5f < g_pGameMain->GetFrameTime() ) {
			SendBuyItem();
		}
	}
}

// ----------------------------------------------------------------------------
void CStoreFrame::Open(int npcId, int storeId, int* itemCount)
{
	if ( IsOpen() ) {
		// 避免重送關閉
		m_receiveClose = false;

		// 關閉商店
		g_pStoreFrame->SendWorldEvent("STORE_CLOSE");
	}

	GameObjDbStruct* itemDB = CNetGlobal::GetObj( storeId );
	if ( itemDB && itemDB->Classification == EM_ObjectClass_Shop )
	{
		m_count		= 0;						// 記錄總販賣種類數量		
		m_npcId		= npcId;					// 
		m_shopId	= storeId;					// 商店物品資料
		m_receiveClose = true;

		memset( m_itemCount, 0, sizeof(m_itemCount) );		// 物品販賣數量
		memset( m_index, 0, sizeof(m_index) );				// 物品在資料庫索引值

		for ( int i = 0; i < MAX_STORE_ITEM; i++ )
		{
			if ( itemDB->Shop.SellItemID[i] != 0 )
			{
				m_index[m_count] = i;
				m_itemCount[m_count] = itemCount[i];
				m_count++;
			}		
		}

		// 開啟商店界面
		SendWorldEvent("STORE_OPEN");
	}
}

// ----------------------------------------------------------------------------
void CStoreFrame::Close()
{	
	switch(g_pItemClipboard->GetType())
	{
	case eClipboardItem_Type_Shop:
		g_pItemClipboard->Clear();
		break;
	}

	SetRepairState(false);
	if ( m_shopId != 0 )
	{
		m_shopId = 0;
		m_npcId = 0;
		m_count = 0;
		m_repairEnable = false;
		memset( m_itemCount, 0 , sizeof(m_itemCount) );
		memset( m_index, 0, sizeof(m_index) );

		if ( m_receiveClose  ) {
			// 界面關閉,通知關閉商店
			NetCli_Shop::S_ShopCloseNotify();
		}
	}

	m_queueID = 0;
	m_queueCount = 0;
}

// ----------------------------------------------------------------------------
void CStoreFrame::UpdateBuy(int index, int count)
{
	if ( !IsOpen() )
		return;

	for ( int i = 0; i < m_count; i++ )
	{
		if ( m_index[i] == index )
		{
			m_itemCount[i] = count;			
			SendWorldEvent("UPDATE_STORE_SELL_ITEMS");		// 更新物品數量
			break;
		}
	}
}

// ----------------------------------------------------------------------------
void CStoreFrame::UpdateSell()
{
	// 更新物品數量
	SendWorldEvent("UPDATE_STORE_BUYBACK_ITEMS");	
}

// ----------------------------------------------------------------------------
int CStoreFrame::GetShopItemIndex(int index)
{
	if ( IsOpen() && index >= 0 && index < m_count )
		return m_index[index];
	return 0;
}

// ----------------------------------------------------------------------------
int CStoreFrame::GetItemId(int index)
{
	if ( IsOpen() && index >= 0 && index < m_count )
	{
		GameObjDbStruct* itemDB = CNetGlobal::GetObj( m_shopId );
		return itemDB->Shop.SellItemID[m_index[index]];
	}
	return 0;
}

// ----------------------------------------------------------------------------
int CStoreFrame::GetItemStock(int index)
{
	if ( IsOpen() && index >= 0 && index < m_count )
		return m_itemCount[index];
	return 0;
}

// ----------------------------------------------------------------------------
bool CStoreFrame::GetCanFix()
{
	if ( IsOpen() )
	{
		GameObjDbStruct* itemDB = CNetGlobal::GetObj( m_shopId );
		if ( itemDB && itemDB->Shop.WeaponRepair == EM_Yes )
			return true;
	}
	return false;
}

// ----------------------------------------------------------------------------
void CStoreFrame::Cut(int state, int index)
{
	if ( !IsOpen() )
		return;

	switch ( state )
	{
	case eStore_Type_Buy:
		{
			GameObjDbStruct* itemDB = CNetGlobal::GetObj( GetItemId(index) );
			if ( itemDB )
			{
				g_pItemClipboard->SetFileFromImageID(itemDB->ImageID);
				g_pItemClipboard->PickupShop(eStore_Type_Buy, index);
			}
		}
		break;

	case eStore_Type_Sell:
		break;
	}
}

// ----------------------------------------------------------------------------
void CStoreFrame::Paste(int state, int index)
{
	if ( !IsOpen() )
		return;

	ClipboardItemInfo itemInfo = g_pItemClipboard->GetItemInfo();
	switch ( state )
	{
	case eStore_Type_Buy:
		switch (g_pItemClipboard->GetType())
		{
		case eClipboardItem_Type_None:					// 拿取物
			Cut( eStore_Type_Buy, index );
			return;

		case eClipboardItem_Type_Bag:					// 背包
			Sell(itemInfo.bag.pos);
			g_pItemClipboard->Clear();
			break;
		}
		break;

	case eStore_Type_Sell:
		switch ( g_pItemClipboard->GetType() )
		{
		case eClipboardItem_Type_None:					// 拿取物
			return;

		case eClipboardItem_Type_Bag:					// 背包
			Sell(itemInfo.bag.pos);
			g_pItemClipboard->Clear();
			break;
		}
		break;
	}

	// clear item clipboard data
	// g_pItemClipboard->Clear();
}

// ----------------------------------------------------------------------------
void CStoreFrame::Buy(int type, int index, int count, bool immediately)
{
	if ( !IsOpen() )
		return;	

	count = max(count, 1);
	switch ( type )
	{
	case eStore_Type_Buy:
		{
			int id = GetItemId(index);
			int stock = GetItemStock(index);
			if ( id == 0 ) {
				// 物品不存在
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_STORE_ITEM_NOT_EXIST"));
				return;
			}
			if ( stock < count ) {
				// 已無存貨,或者存貨不足
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_STORE_ITEMSTOCKS_EMPTY"));
				return;
			}
			
			GameObjDbStructEx* goods = CNetGlobal::GetObj(id);
			if ( goods ) {
				int unit = count;
				if ( immediately == false )
					unit *= max(goods->BuyUnit, 1);
				//AddBuyItem(index, id, unit, immediately);
				AddBuyItem(m_index[index], id, unit, immediately); // fixed the shop bug where you could not buy items when shopobj had empty spaced
			}
		}
		break;

	case eStore_Type_Sell:
		{
			SellItemFieldStruct* sellItem = CNetGlobal::RoleData()->GetSellItem(index);
			if ( sellItem )
				NetCli_Shop::S_BuySellItemRecord( *sellItem );
		}
		break;
	}	
}

// ----------------------------------------------------------------------------
// 目前只提供有背包賣出
// pos 背包序列索引值
void CStoreFrame::Sell(int pos)
{
	ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(pos);
	if ( item && !item->IsEmpty() )
	{
		item->Pos = EM_ItemState_CliDisabled;
		NetCli_Shop::S_SelectSellItemRequest( pos, item );
	}
}

// ----------------------------------------------------------------------------
void CStoreFrame::AddBuyItem(int index, int id, int count, bool immediately)
{
	GameObjDbStructEx* goods = CNetGlobal::GetObj(id);
	if ( goods == NULL )
		return;

	if ( immediately == false )
	{
		// 檢查是否封包傳遞間隔太短，若時間間隔太短累計封包個數
		if ( goods->MaxHeap <= 1 )
		{
			static int packageCount = 0;	// 記錄短時間傳遞移動封包個數	
			if ( g_pGameMain->GetFrameTime() - m_lastSendBuyTime > 1.0f ) {
				if ( packageCount > 0 )
					packageCount--;
			} else if ( packageCount < 5 )
				packageCount++;
			else
				return;
		}

		// 購買相同物品,如果未達物品堆疊上限則在累加
		//if ( m_queueID == id && goods->MaxHeap >= m_queueCount + count ) {
		if ( m_queueIndex == index && goods->MaxHeap >= m_queueCount + count ) {
			m_queueCount += count;
			return;
		}

		// 送出上次購買商品
		SendBuyItem();
	}	

	// 將新物品放入堆疊
	m_queueIndex = index;
	m_queueID = id;
	m_queueCount = count;

	// 如果是不能堆疊物品不暫存
	if ( goods->MaxHeap <= 1 || immediately ) {
		SendBuyItem();
	}
}

// ----------------------------------------------------------------------------
void CStoreFrame::SendBuyItem()
{
	if ( m_queueID != 0 && m_queueCount > 0 ) {
		NetCli_Shop::S_BuyItemRequest(m_queueID, m_queueCount, m_queueIndex);

		m_lastSendBuyTime = (float)g_pGameMain->GetFrameTime();
		m_queueID = 0;
		m_queueCount = 0;
		m_queueIndex = -1;
	}
}

// ----------------------------------------------------------------------------
void CStoreFrame::ReceiveClose()
{
	m_receiveClose = true;
	g_pStoreFrame->SendWorldEvent("STORE_CLOSE");
}

// ----------------------------------------------------------------------------
int	LuaFunc_GetStoreSellItemInfo(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	if ( !g_pStoreFrame->IsOpen() )
		return 0;

	const char* icon = NULL;
	const char* name = NULL;
	int count = 0;
	int price = 0;
	int stock = 0;
	int moneyType = 0;
	int maxHeap = 0;
	int secondPrice = 0;
	int secondMoneyType = 0;
	if ( index >= 0 && index < g_pStoreFrame->GetCount() )
	{
		GameObjDbStructEx* shop = CNetGlobal::GetObj( g_pStoreFrame->GetShopId() );
		GameObjDbStructEx* goods = CNetGlobal::GetObj( g_pStoreFrame->GetItemId(index) );
		int shopItemIndex = g_pStoreFrame->GetShopItemIndex(index);

		if ( shop && goods )
		{
			count	= g_pStoreFrame->GetItemStock(index);
			stock	= goods->BuyUnit;			
			name	= goods->GetName();
			maxHeap = goods->MaxHeap;

			GameObjDbStruct* imageDB = CNetGlobal::GetObj(goods->ImageID);
			if ( imageDB )
			{
				icon = imageDB->Image.ACTField;
			}

			// 是否使用商店內定價
			if ( shop->Shop.SellCost[shopItemIndex][0] == 0 && shop->Shop.SellCost[shopItemIndex][1] == 0 )
			{
				if ( goods->PricesType > EM_ACPriceType_GameMoney )
				{
					moneyType	= goods->PricesType;
					price		= goods->SpecialMoneyCost;
				}
				else
				{
					moneyType	= 0;
					price		= goods->Cost;
				}
			}
			else
			{
				if ( shop->Shop.SellCost[shopItemIndex][0] == 0 )
				{
					price = shop->Shop.SellCost[shopItemIndex][1];
					moneyType = shop->Shop.CostType[shopItemIndex][1];
					secondPrice = shop->Shop.SellCost[shopItemIndex][0];
					secondMoneyType = shop->Shop.CostType[shopItemIndex][0];
				}
				else
				{
					price = shop->Shop.SellCost[shopItemIndex][0];
					moneyType = shop->Shop.CostType[shopItemIndex][0];
					secondPrice = shop->Shop.SellCost[shopItemIndex][1];
					secondMoneyType = shop->Shop.CostType[shopItemIndex][1];
				}
			}

			price = (price * shop->Shop.BuyRate) / 100;
			secondPrice = (secondPrice * shop->Shop.BuyRate) / 100;

			if ( stock > 1 )
			{
				price *= stock;
				secondPrice *= stock;
			}
			else
				stock = 0;
		}
	}

	lua_pushstring(L, icon);
	lua_pushstring(L, name);
	lua_pushnumber(L, price);
	lua_pushnumber(L, moneyType);
	lua_pushnumber(L, count);
	lua_pushnumber(L, stock);
	lua_pushnumber(L, maxHeap);
	lua_pushnumber(L, secondPrice);
	lua_pushnumber(L, secondMoneyType);
	return 9;
}

// ----------------------------------------------------------------------------
int	LuaFunc_GetStoreSellItems(lua_State* L)
{
	lua_pushnumber( L, g_pStoreFrame->GetCount() );
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_PickupStoreSellItem(lua_State* L)
{
	g_pStoreFrame->Paste( CStoreFrame::eStore_Type_Buy, luaL_checkint(L, 1) - 1 );
	return 0;
}

// ----------------------------------------------------------------------------
int	LuaFunc_GetStoreBuyBackItemInfo(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	if ( !g_pStoreFrame->IsOpen() )
		return 0;

	SellItemFieldStruct* item = CNetGlobal::RoleData()->GetSellItem(index);
	if ( item )
	{
		GameObjDbStructEx* itemDB = CNetGlobal::GetObj( item->Item.OrgObjID );
		if ( itemDB )
		{
			GameObjDbStruct* imageDB = CNetGlobal::GetObj( itemDB->ImageID );
			if ( imageDB )
			{
				lua_pushstring( L, imageDB->Image.ACTField );
				lua_pushstring( L, itemDB->GetName() );
				lua_pushnumber( L, item->Money );
				lua_pushnumber( L, item->Item.Count );
				lua_pushnumber( L, 0 );
				return 5;
			}
		}
	}
	
	return 0;
}

// ----------------------------------------------------------------------------
int	LuaFunc_GetStoreBuyBackItems(lua_State* L)
{
	lua_pushnumber( L, CNetGlobal::RoleData()->GetSellItemCount() );
	return 1;
}

// ----------------------------------------------------------------------------
int	LuaFunc_PickupStoreBuyBackItem(lua_State* L)
{
	g_pStoreFrame->Paste( CStoreFrame::eStore_Type_Sell, luaL_checkint(L, 1) - 1 );
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_StoreBuyItem(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	int count = 1;
	bool immediately = false;
	if ( lua_isnumber(L, 2) ) {
		immediately = true;
		count = (int)lua_tonumber(L, 2);
	}
	
	g_pStoreFrame->Buy( CStoreFrame::eStore_Type_Buy, luaL_checkint(L, 1) - 1, count, immediately );
	return 0;
}

// ----------------------------------------------------------------------------
int	LuaFunc_StoreBuyBackItem(lua_State* L)
{
	g_pStoreFrame->Buy( CStoreFrame::eStore_Type_Sell, luaL_checkint(L, 1) - 1, 1 );
	return 0;
}

// ----------------------------------------------------------------------------
int	LuaFunc_IsStoreCanFix(lua_State* L)
{
	lua_pushboolean(L, g_pStoreFrame->GetCanFix());	
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetEquipmentRepairMoney(lua_State* L)
{
	ItemFieldStruct* item = CNetGlobal::RoleData()->GetEqItem(luaL_checkint(L, 1));
	lua_pushnumber(L, RoleDataEx::CalRepairWeapon(item, NULL));	
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetEquipmentRepairAllMoney(lua_State* L)
{
	int money = 0;
	for ( int i = 0; i < EM_EQWearPos_MaxCount; i++ ) {
		ItemFieldStruct* item = CNetGlobal::RoleData()->GetEqItem(i);
		money += -RoleDataEx::CalRepairWeapon(item, NULL);
	}
	lua_pushnumber(L, money);
	return 1;
}

// ----------------------------------------------------------------------------
int	LuaFunc_ClickRepairButton(lua_State* L)
{
	if ( g_pStoreFrame->GetCanFix() )
	{
		g_pStoreFrame->SetRepairState(true);
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_ClickRepairAllButton(lua_State* L)
{
	if ( g_pStoreFrame->GetCanFix() ) {
		int money = 0;

		for ( int i = 0; i < EM_EQWearPos_MaxCount; i++ ) {
			ItemFieldStruct* item = CNetGlobal::RoleData()->GetEqItem(i);
			money += -RoleDataEx::CalRepairWeapon(item, NULL);
		}

		// 裝備無須修理
		if ( money == 0 )
		{
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_REPAIR_ALLEQ_FAILED"));
			return 0;
		}

		// 檢查金錢是否足夠
		if ( CNetGlobal::RoleData()->BodyMoney() < money ) {
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_REPAIR_ALERT_MONEY"));
			return 0;
		}

		NetCli_Shop::S_RepairWeaponRequest(-1, true);
	}
	return 0;
}

// ----------------------------------------------------------------------------
int	LuaFunc_CloseStore(lua_State* L)
{
	g_pStoreFrame->Close();
	return 0;
}

// ----------------------------------------------------------------------------
int	LuaFunc_GetStoreSellItemLink(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	if ( !g_pStoreFrame->IsOpen() )
		return 0;

	if ( index >= 0 && index < g_pStoreFrame->GetCount() )
	{
		GameObjDbStructEx* shop = CNetGlobal::GetObj( g_pStoreFrame->GetShopId() );
		GameObjDbStructEx* goods = CNetGlobal::GetObj( g_pStoreFrame->GetItemId(index) );
		if ( shop && goods )
		{
			//ItemFieldStruct ItemField;
			//ItemField.Init();
			//ItemField.OrgObjID = goods->GUID ;
			//ItemField.ImageObjectID = goods->GUID ;

			//string link;
			//if( !ItemField_To_ItemLink( &ItemField, link ) )
			//{
			//	return 0;
			//}

			string link;
			if( !ItemGUID_To_ItemLink( goods->GUID, link ) )
			{
				return 0;
			}
			lua_pushstring(L, link.c_str() );
			return 1;
		}
	}
  
	return 0;
}



// ----------------------------------------------------------------------------
int	LuaFunc_GetStoreBuyBackItemLink(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	if ( !g_pStoreFrame->IsOpen() )
		return 0;

	SellItemFieldStruct* item = CNetGlobal::RoleData()->GetSellItem(index);
	if ( item )
	{
  		string link;
		if( !ItemField_To_ItemLink( &item->Item, link ) )
		{
			return 0;
		}
		lua_pushstring(L, link.c_str() );
		return 1;
	}

	return 0;
}



