#include "../../mainproc/GameMain.h"
#include "../../netwaker_member/NetWakerClientInc.h"
#include "ItemClipboard.h"
#include "StoreFrame.h"
#include "BankFrame.h"
#include "BagFrame.h"
#include "CharacterFrame.h"
#include "PetFrame.h"
#include "../Houses/HousesFrame.h"
#include "../GuildHouses/GuildHousesFrame.h"
#include "../WorldFrames/ChatFrame.h"
#include "../WorldFrames/SuitSkillFrame.h"
#include "ItemUtil.h"
#include "../ItemLink/ItemLink.h"
#include "../WorldFrames/RuneExchange.h"

CBagFrame*					g_pBagFrame		= NULL;



#define UiEvent_GambleItemChanged "GAMBLE_ITEM_CHANGED"

#define UiEvent_GambleResultOK "GAMBLE_RESULT_OK"
#define UiEvent_GambleResultFailed "GAMBLE_RESULT_FAILED"

#define UiEvent_GambleItemExchanged "GAMBLE_ITEM_EXCHANGED"

#define UiEvent_GambleOpen  "GAMBLE_OPEN"
#define UiEvent_GambleClose "GAMBLE_CLOSE"

//#define UiEvent_GambleStepChanged "GAMBLE_STEP_CHANGED"



int	LuaFunc_GetBagItemInfo		(lua_State* L);
int LuaFunc_GetBagPageLetTime	(lua_State* L);
int LuaFunc_GetBagItemCooldown	(lua_State* L);
int	LuaFunc_GetBagCount			(lua_State* L);
int	LuaFunc_PickupBagItem		(lua_State* L); 
int	LuaFunc_UseBagItem			(lua_State* L);
int LuaFunc_UseItemByName		(lua_State* L);
int LuaFunc_CloseBag			(lua_State* L);
int LuaFunc_SplitBagItem		(lua_State* L);
int LuaFunc_RefreshBag			(lua_State* L);
int	LuaFunc_GetBagItemCount		(lua_State* L);


//物品融合
int LuaFunc_IMF_GetCursorItem(lua_State* L);
int LuaFunc_IMF_GetMargeItem(lua_State* L);  
int LuaFunc_IMF_RemoveMargeItem(lua_State* L);  
int LuaFunc_IMF_MargeItemClose(lua_State* L);  
int LuaFunc_IMF_MargeItemApply(lua_State* L);  


// 物品分配暫存取,事件函式
// 分配物品需要回傳
//int Lua_AssignBagQueueItem		(lua_State* L);

int LuaFunc_GetItemQueueInfo		(lua_State* L);
int	LuaFunc_ItemQueueAssignToBag	(lua_State* L);

int LuaFunc_UnlockPendingItem		(lua_State* L);
int LuaFunc_CancelUnlockItem		(lua_State* L);

//ItemLink
int LuaFunc_GetBagItemLink			(lua_State* L);

// Garbage Item
int LuaFunc_CloseGarbageItems		(lua_State* L);
int LuaFunc_GetGarbageMaxItems		(lua_State* L);
int LuaFunc_GetGarbageItemInfo		(lua_State* L);
int LuaFunc_InsertGarbageItem		(lua_State* L);
int LuaFunc_RemoveGarbageItem		(lua_State* L);
int LuaFunc_DeleteGarbageItems		(lua_State* L);

// Goods Item
int LuaFunc_GetGoodsItemInfo		(lua_State* L);
int LuaFunc_GetGoodsItemCooldown	(lua_State* L);


//MagicBox
int LuaFunc_IsMagicBoxEnable		(lua_State* L);
int LuaFunc_GetMagicBoxPreviewResult(lua_State* L);
int LuaFunc_MagicBoxRequest			(lua_State* L);
int LuaFunc_GetMagicBoxEnergy       (lua_State* L);	

int LuaFunc_MagicBoxDrawStone		(lua_State* L);
int LuaFunc_MagicBoxCanDrawStone	(lua_State* L);

//Gamble
int LuaFunc_Gamble_Close			(lua_State* L);

int LuaFunc_Gamble_PickupItem		(lua_State* L);
int LuaFunc_Gamble_GetItemInfo		(lua_State* L);

int LuaFunc_Gamble_GetLockCount		(lua_State* L);

int LuaFunc_Gamble_GetAttributeInfo	(lua_State* L);
int LuaFunc_Gamble_GetPrizeInfo		(lua_State* L);
int LuaFunc_Gamble_Roll				(lua_State* L);
int LuaFunc_Gamble_Stop				(lua_State* L);
int LuaFunc_Gamble_Stoped			(lua_State* L);
int LuaFunc_Gamble_GetResult        (lua_State* L);
int LuaFunc_Gamble_Exchange			(lua_State* L);
int LuaFunc_Gamble_GiveUp           (lua_State* L);

int LuaFunc_Gamble_ClearItem		(lua_State* L);

int LuaFunc_Gamble_GetStep          (lua_State* L);

int LuaFunc_Gamble_SetLockFlag		(lua_State* L);
int LuaFunc_Gamble_GetLockFlag		(lua_State* L);

#define USE_BANK_STORE 1
// ----------------------------------------------------------------------------
CBagFrame::CBagFrame(CInterface* object)
: CInterfaceBase(object)
{
	g_pBagFrame = this;

	m_pickupItemIndex	= -1;
	m_useIndex			= -1;
	m_useToLocation		= -1;
	m_ItemMargePos		= -1;	
	m_waitQueueUpdate	= false;
	m_lockedQueueTime	= 0.0f;
	m_lockedPos			= 0;
	m_updateIndex		= false;
	m_billdin			= 0;

	m_splitBagItemIndex = -1;
	m_splitBagItemCount = 0;

	memset(m_itemPos, 0, sizeof(m_itemPos));

	CancelUnlockedItem();

	m_eGambleStep = GS_NoItem;

	m_GambleBagItemIndex = -1;

	m_isGiveUp = false;

	m_GambleManage = -1;

}

// ----------------------------------------------------------------------------
CBagFrame::~CBagFrame()
{
	g_pBagFrame = NULL;
}

// ----------------------------------------------------------------------------
void CBagFrame::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{
		lua_register(L, "GetBagItemInfo",		LuaFunc_GetBagItemInfo);
		lua_register(L, "GetBagPageLetTime",	LuaFunc_GetBagPageLetTime);
		lua_register(L, "GetBagItemCooldown",	LuaFunc_GetBagItemCooldown);
		lua_register(L, "GetBagCount",			LuaFunc_GetBagCount);
		lua_register(L, "PickupBagItem",		LuaFunc_PickupBagItem);
		lua_register(L, "UseBagItem",			LuaFunc_UseBagItem);
		lua_register(L, "UseItemByName",		LuaFunc_UseItemByName);
		lua_register(L, "CloseBag",				LuaFunc_CloseBag);
		lua_register(L, "SplitBagItem",			LuaFunc_SplitBagItem);
		lua_register(L, "RefreshBag",			LuaFunc_RefreshBag);
		lua_register(L, "GetBagItemCount",		LuaFunc_GetBagItemCount);

		lua_register(L, "GetItemQueueInfo",		LuaFunc_GetItemQueueInfo);
		lua_register(L, "ItemQueueAssignToBag",	LuaFunc_ItemQueueAssignToBag);

		lua_register(L, "UnlockPendingItem",	LuaFunc_UnlockPendingItem);
		lua_register(L, "CancelUnlockItem",		LuaFunc_CancelUnlockItem);

		//物品融合
		lua_register(L, "IMF_GetCursorItem",		LuaFunc_IMF_GetCursorItem);

		lua_register(L, "IMF_GetMargeItem",		LuaFunc_IMF_GetMargeItem);

		
		lua_register(L, "IMF_RemoveMargeItem",	LuaFunc_IMF_RemoveMargeItem);
		lua_register(L, "IMF_MargeItemClose",	LuaFunc_IMF_MargeItemClose);
		lua_register(L, "IMF_MargeItemApply",	LuaFunc_IMF_MargeItemApply);

 		lua_register(L, "GetBagItemLink",	LuaFunc_GetBagItemLink);

		// 垃圾桶
		lua_register(L, "CloseGarbageItems",		LuaFunc_CloseGarbageItems);
		lua_register(L, "GetGarbageMaxItems",		LuaFunc_GetGarbageMaxItems);
		lua_register(L, "GetGarbageItemInfo",		LuaFunc_GetGarbageItemInfo);
		lua_register(L, "InsertGarbageItem",		LuaFunc_InsertGarbageItem);
		lua_register(L, "RemoveGarbageItem",		LuaFunc_RemoveGarbageItem);
		lua_register(L, "DeleteGarbageItems",		LuaFunc_DeleteGarbageItems);		

		lua_register(L, "GetGoodsItemInfo",			LuaFunc_GetGoodsItemInfo);
		lua_register(L, "GetGoodsItemCooldown",		LuaFunc_GetGoodsItemCooldown);

		//MagicBox
		lua_register(L, "IsMagicBoxEnable",			LuaFunc_IsMagicBoxEnable);
		lua_register(L, "GetMagicBoxPreviewResult",	LuaFunc_GetMagicBoxPreviewResult);
		lua_register(L, "MagicBoxRequest",			LuaFunc_MagicBoxRequest);
		lua_register(L, "MagicBoxDrawStone",		LuaFunc_MagicBoxDrawStone);
		lua_register(L, "MagicBoxCanDrawStone",		LuaFunc_MagicBoxCanDrawStone);

		

		
   		lua_register(L, "GetMagicBoxEnergy",		LuaFunc_GetMagicBoxEnergy);

		//Gamble

		lua_register(L, "Gamble_Close",				LuaFunc_Gamble_Close);

		lua_register(L, "Gamble_PickupItem",		LuaFunc_Gamble_PickupItem);
		lua_register(L, "Gamble_GetItemInfo",		LuaFunc_Gamble_GetItemInfo);

		lua_register(L, "Gamble_GetLockCount",		LuaFunc_Gamble_GetLockCount);

		lua_register(L, "Gamble_GetAttributeInfo",  LuaFunc_Gamble_GetAttributeInfo);
		lua_register(L, "Gamble_GetPrizeInfo",		LuaFunc_Gamble_GetPrizeInfo);

		lua_register(L, "Gamble_Roll",				LuaFunc_Gamble_Roll);
		lua_register(L, "Gamble_Stop",				LuaFunc_Gamble_Stop);
		lua_register(L, "Gamble_Stoped",			LuaFunc_Gamble_Stoped);

		lua_register(L, "Gamble_GetResult",			LuaFunc_Gamble_GetResult);
		lua_register(L, "Gamble_Exchange",			LuaFunc_Gamble_Exchange);
		lua_register(L, "Gamble_GiveUp",			LuaFunc_Gamble_GiveUp);
		lua_register(L, "Gamble_ClearItem",			LuaFunc_Gamble_ClearItem);


		lua_register(L, "Gamble_GetStep",			LuaFunc_Gamble_GetStep);

		lua_register(L, "Gamble_SetLockFlag",		LuaFunc_Gamble_SetLockFlag);
		lua_register(L, "Gamble_GetLockFlag",		LuaFunc_Gamble_GetLockFlag);

	}
}

// ----------------------------------------------------------------------------
void CBagFrame::BuildFrames()
{
	m_pickupItemIndex	= -1;
	m_useIndex			= -1;
	m_ItemMargePos		= -1;
	m_waitQueueUpdate	= false;
	m_lockedQueueTime	= 0.0f;
	m_lockedPos			= 0;
	m_updateIndex		= false;
	m_splitBagItemIndex = -1;
	m_splitBagItemCount = 0;

	memset(m_itemPos, 0, sizeof(m_itemPos));

	m_garbage.clear();
}

// ----------------------------------------------------------------------------
void CBagFrame::DestroyFrames()
{
	m_ItemMargePos		= -1;
	m_lockedPos			= 0;
	m_updateIndex		= false;

	m_garbage.clear();
	NetCli_Item::_EQCombinClose( );
}

// ----------------------------------------------------------------------------
int CBagFrame::AssignQueueItemToBag()
{
	RoleDataEx* role = CNetGlobal::RoleData();

	if ( role->PlayerBaseData->ItemTemp.IsEmpty() ) {
		return 0;											// 沒有物件在序列中
	}
	if ( m_waitQueueUpdate ) {								// 正在等待回應中
		return -1;
	}

	ItemFieldStruct* item = role->PlayerBaseData->ItemTemp.Begin();
	GameObjDbStruct* itemDB = g_ObjectData->GetObj(item->OrgObjID);
	if ( itemDB == NULL ) {
		return -2;											// 物品資料錯誤,不應該發生的錯誤
	}

	int pos = role->SearchBodyFirstFit(item, true, item->Count);			// 找尋一個可以插入背包的位置
	if ( pos == - 1 ) {
		// 物品未找到可以用的空間, 大於7個個數後需要將物品做清除動作
		return -3;
	} else {
		m_waitQueueUpdate	= true;
		m_lockedQueueTime	= 10.0f;
		m_lockedPos			= pos;

		ItemFieldStruct* bagItem = role->GetBodyItem(pos);
		bagItem->Pos = EM_ItemState_CliDisabled;
		int Count = __min( item->Count, max(itemDB->MaxHeap, 1) - bagItem->Count );
		NetCli_Item::_GetBufferItem( *item, pos, Count );
	}
	return 0;
}

// ----------------------------------------------------------------------------
// SERVER 回應物品新進背包
void CBagFrame::ReceiveQueueItemToBag(int ItemID)
{
	m_waitQueueUpdate	= false;
	m_lockedQueueTime	= 0.0f;
	m_lockedPos			= -1;

	lua_State* L = GetLuaStateWorld();
	lua_pushnumber( L, ItemID );
	lua_setglobal(L, "arg1");
	// 物品新進背包
	g_pBagFrame->SendWorldEvent("ITEMQUEUE_UPDATE");
}

// ----------------------------------------------------------------------------
// 拾取新物品
void CBagFrame::GiveNewItem()
{
	// 物品新進背包
	g_pBagFrame->SendWorldEvent("GIVE_NEW_ITEM");
}

// ----------------------------------------------------------------------------
// SERVER 堆入新物品
void CBagFrame::InsertItemQueueItem()
{
	g_pBagFrame->SendWorldEvent("ITEMQUEUE_INSERT");
}

// ----------------------------------------------------------------------------
void CBagFrame::ReflashItemQueueTime()
{
	m_lockedQueueTime = 0.5f;
}

// ----------------------------------------------------------------------------
void CBagFrame::ClearAllQueueItem()
{
	g_pBagFrame->SendWorldEvent("ITEMQUEUE_CLEAR");
}

// ----------------------------------------------------------------------------
void CBagFrame::Update(float elapsedTime)
{
	// 解除因封包未回應造成的物品鎖定
	if ( m_waitQueueUpdate && m_lockedQueueTime > 0.0f ) {
		m_lockedQueueTime -= elapsedTime;
		if ( m_lockedQueueTime < 0.001f ) {
			ItemFieldStruct* bagItem = CNetGlobal::RoleData()->GetBodyItem(m_lockedPos);
			if ( bagItem && bagItem->Pos == EM_ItemState_CliDisabled ) {
				bagItem->Pos = EM_ItemState_NONE;
			}
			m_waitQueueUpdate	= false;
			m_lockedQueueTime	= 0.0f;
			m_lockedPos			= -1;
		}
	}

	if ( m_pickupItemIndex != -1  )
	{
		ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(m_pickupItemIndex);
		if ( item )
		{
			if ( item->Pos == EM_ItemState_CliDisabled )
			{
				ClipboardItemInfo itemInfo = g_pItemClipboard->GetItemInfo();
				if ( g_pItemClipboard->GetType() != eClipboardItem_Type_Bag || itemInfo.bag.pos != m_pickupItemIndex )
				{
					item->Pos = EM_ItemState_NONE;
					m_pickupItemIndex = -1;
				}					
			}
			else
			{
				m_pickupItemIndex = -1;
			}

		}
		else
		{
			m_pickupItemIndex = -1;
		}
	}

	//分堆
	if( m_splitBagItemIndex != -1 )
	{
		ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem( m_splitBagItemIndex );
		if ( item )
		{
			if ( item->Pos == EM_ItemState_CliDisabled )
			{
				ClipboardItemInfo itemInfo = g_pItemClipboard->GetItemInfo();
				if ( g_pItemClipboard->GetType() != eClipboardItem_Type_SplitBagItem || itemInfo.splitBagItem.pos != m_splitBagItemIndex )
				{
					item->Pos = EM_ItemState_NONE;
					m_splitBagItemIndex = -1;
					m_splitBagItemCount = 0;
				}					
			}
			else
			{
				m_splitBagItemIndex = -1;
				m_splitBagItemCount = 0;
			}
		}
		else
		{
			m_splitBagItemIndex = -1;
			m_splitBagItemCount = 0;
		}

	}

	//Gamble
	if( m_GambleBagItemIndex != -1 )
	{
		ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem( m_GambleBagItemIndex );
		if ( item )
		{
			if ( item->Pos == EM_ItemState_NONE )
			{
				item->Pos = EM_ItemState_CliDisabled;
			}
		}
		else
		{
			m_GambleBagItemIndex = -1;
		}
	}

	if ( m_GambleManage >= 0 )
	{
		if (g_pGameMain->GetDistanceWithPlayer(m_GambleManage) > _MAX_ROLE_TOUCH_MAXRANGE_)
		{
			SendWorldEvent( UiEvent_GambleClose );
		}
   }
	

	// 檢查垃圾桶資料是否有錯誤
	bool needUpdate = false;
	vector<int>::iterator iter = m_garbage.begin();
	while ( iter != m_garbage.end() ) {
		ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(*iter);
		if ( item == NULL || item->IsEmpty() ) {
			needUpdate = true;
			iter = m_garbage.erase(iter);
		} else {
			iter++;
		}
	}
	if ( needUpdate ) {
		UpdateGarbageItem();
	}

	// 更新背包索引值
	if ( m_updateIndex )
	{
		m_updateIndex = false;

		// 通知界面更新
		UpdateItem(-1);
		
		// 送出封包
		NetCli_RoleValue::S_ClientData(CNetGlobal::RoleData()->PlayerBaseData->ClientData);
	}

	// 檢查是否有項目需要更新
	for ( int i = 0; i < _MAX_BODY_COUNT_; i++ )
	{
		ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(i);
		if ( item->Pos != m_itemPos[i] )
			UpdateItem(i);
	}	
}

// ----------------------------------------------------------------------------
void CBagFrame::Close()
{
	// 關閉背包

	switch(g_pItemClipboard->GetType())
	{
	case eClipboardItem_Type_Bag:
		g_pItemClipboard->Clear();
		break;
	case eClipboardItem_Type_SplitBagItem:
		g_pItemClipboard->Clear();
		break;
	}

	// 物品對物品使用中
	// m_casting.m_type != BAG_CASTING_NULL

	// 背包物品對物品使用等待中
	// m_useIndex
}
//------------------------------------------------------------------------------
void CBagFrame::UnlockPendingItem(const char* password)
{
	// 記錄密碼
	m_password = password;

	UseItemToItem(m_unlockType, m_unlockPos);
}
//------------------------------------------------------------------------------
void CBagFrame::CancelUnlockedItem()
{
	ItemFieldStruct* item;
	item = CNetGlobal::RoleData()->GetBodyItem(m_unlockSlot);
	if (item && !item->IsEmpty())
		item->Pos = EM_ItemState_NONE;

	item = CNetGlobal::RoleData()->GetBodyItem(m_unlockPos);
	if (item && !item->IsEmpty())
		item->Pos = EM_ItemState_NONE;

	m_password.clear();
	m_unlockSlot = -1;
	m_unlockType = -1;
	m_unlockPos = -1;
}
//------------------------------------------------------------------------------
void CBagFrame::ClearGarbageItems()
{
	for ( vector<int>::iterator iter = m_garbage.begin(); iter != m_garbage.end(); iter++ )
	{
		ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(*iter);
		if ( item != NULL && !item->IsEmpty() )
			item->Pos = EM_ItemState_NONE;
	}
	m_garbage.clear();
}
//------------------------------------------------------------------------------
int CBagFrame::GetGarbageItemIndex(int index)
{
	if ( index < 0 || index >= (int)m_garbage.size() )
		return -1;
	return m_garbage[index];
}
//------------------------------------------------------------------------------
bool CBagFrame::InsertGarbageItem(int bagIndex, bool ignoreCursorItem)
{
	for ( vector<int>::iterator iter = m_garbage.begin(); iter != m_garbage.end(); iter++ )
	{
		// 檢查是否重複加入
		if ( *iter == bagIndex )
			return false;
	}	

	if ( m_garbage.size() >= 20 )
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_GARBAGE_FULL"));
		return false;
	}

	ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(bagIndex);
	if ( item == NULL || item->IsEmpty() )
	{
		return false;
	}

	GameObjDbStructEx* itemDB = CNetGlobal::GetObj(item->OrgObjID);
	if ( itemDB && itemDB->Classification == EM_ObjectClass_Item && itemDB->Item.ItemType == EM_ItemType_Plot )
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_GARBAGE_ERROR_QUESTITEM"));
		return false;
	}

	if ( item->Pos != EM_ItemState_NONE )
	{
		ClipboardItemInfo itemInfo = g_pItemClipboard->GetItemInfo();
		if ( ignoreCursorItem && g_pItemClipboard->GetType() == eClipboardItem_Type_Bag && itemInfo.bag.pos == bagIndex )
		{
			ClearPickupItemIndex();
			g_pItemClipboard->Clear();
		}
		else
		{
			return false;
		}
	}

	item->Pos = EM_ItemState_CliDisabled;
	m_garbage.push_back(bagIndex);	
	UpdateGarbageItem();
	return true;
}
//------------------------------------------------------------------------------
bool CBagFrame::RemoveGarbageItem(int bagIndex)
{
	for ( vector<int>::iterator iter = m_garbage.begin(); iter != m_garbage.end(); iter++ )
	{
		// 檢查是否重複加入
		if ( *iter == bagIndex )
		{
			ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(bagIndex);
			if ( item != NULL && !item->IsEmpty() )
			{
				item->Pos = EM_ItemState_NONE;
			}

			m_garbage.erase(iter);
			UpdateGarbageItem();
			return true;
		}
	}
	return false;
}

//------------------------------------------------------------------------------
bool CBagFrame::DeleteGarbageItems()
{
	if ( m_garbage.empty() )
		return false;
	
	for ( vector<int>::iterator iter = m_garbage.begin(); iter != m_garbage.end(); iter++ )
	{
		ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(*iter);
		NetCli_Item::_DeleteItem_Body(*item, *iter);
	}
	m_garbage.clear();
	return true;
}

//------------------------------------------------------------------------------
void CBagFrame::UpdateGarbageItem()
{
	SendWorldEvent("GARBAGE_UPDATE");
}

//------------------------------------------------------------------------------
void CBagFrame::SortItems(int page)
{
	if ( page < 0 || page >= 6 )
		return;

	if ( page == 2 && CNetGlobal::RoleData()->PlayerBaseData->TimeFlag[EM_TimeFlagEffect_BagPage3] == -1 )
		return;
	if ( page == 3 && CNetGlobal::RoleData()->PlayerBaseData->TimeFlag[EM_TimeFlagEffect_BagPage4] == -1 )
		return;
	if ( page == 4 && CNetGlobal::RoleData()->PlayerBaseData->TimeFlag[EM_TimeFlagEffect_BagPage5] == -1 )
		return;
	if ( page == 5 && CNetGlobal::RoleData()->PlayerBaseData->TimeFlag[EM_TimeFlagEffect_BagPage6] == -1 )
		return;

	BodyItemStruct& bodyItems = CNetGlobal::RoleData()->PlayerBaseData->Body;
	int count = 30;
	int offset = page * count;

	// 清除暫存資料重新排序
	unsigned char* bag = CNetGlobal::RoleData()->PlayerBaseData->ClientData;
	for ( int i = 0; i < count; i++ )
	{
		bag[i + offset] = _MAX_BODY_EX_COUNT_ + _MAX_BODY_MAGIC_BOX_COUNT_ + i + offset;
	}
	
	ItemFieldStruct* item = NULL;
	for ( int i = 0; i < count; i++ )
	{		
		item = CNetGlobal::RoleData()->GetBodyItem(bag[i + offset]);
		GameObjDbStruct* srcItem = g_ObjectData->GetObj(item->OrgObjID);
		for ( int j = i + 1; j < count; j++ )
		{
			item = CNetGlobal::RoleData()->GetBodyItem(bag[j + offset]);
			GameObjDbStruct* destItem = g_ObjectData->GetObj(item->OrgObjID);
			if ( g_ObjectData->CompareItemOrder(srcItem, destItem) < 0 )
			{
				int temp = bag[i + offset];
				bag[i + offset] = bag[j + offset];
				bag[j + offset] = temp;

				srcItem = destItem;
			}
		}
	}

	// Update Bag
	m_updateIndex = true;
}
//------------------------------------------------------------------------------
void CBagFrame::InsertSortItem(int index)
{
	/*
	char* bag = CNetGlobal::RoleData()->PlayerBaseData->ClientData;
	for ( int i = 0; i < _MAX_BODY_COUNT_; i++ )
	{
		ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyVirtualItem(i);
		if ( item && item->IsEmpty() )
		{
			bag[i] = index;
			break;
		}
	}
	*/

	// Update Bag
	m_updateIndex = true;
}
//------------------------------------------------------------------------------
void CBagFrame::DeleteSortItem(int index)
{	
	// Update Bag
	m_updateIndex = true;
}
//------------------------------------------------------------------------------
void  CBagFrame::MargeItemApply(int a,int b)
{	
	NetCli_Item::_EQCombinRequest( a,b,m_ItemMargePos);
	Unlocked(m_ItemMargePos);
	m_ItemMargePos=-1;
}

void CBagFrame::MargeItemClose(void)
{
	if (m_ItemMargePos!=-1)
		Unlocked(m_ItemMargePos);
	m_ItemMargePos=-1;
NetCli_Item::_EQCombinClose( );
}
void CBagFrame::ItemMargeResult(bool Result)
{

	const char *str=NULL;
	if (Result)
		str=g_ObjectData->GetString("SYS_ITEM_MARGE_SUCCEED");
	else
		str=g_ObjectData->GetString("SYS_ITEM_MARGE_FAIL");
	

	g_pChatFrame->SendMsgEvent("CHAT_MSG_SYSTEM", str, "" );
	g_pGameMain->SendWarningMsg(str);	

	NetCli_Item::_EQCombinClose( );

	// 關閉融合界面
	SendWorldEvent("ITEMMARGE_CLOSE");
}

void CBagFrame::OpenItemMarge(int npc, int pos ,int Type)
{
	// 打開融合界面
	
	lua_State* L = GetLuaState();
	lua_pushnumber(L, Type);
	lua_setglobal(L, "arg1");

	SendWorldEvent("ITEMMARGE_OPEN");
	m_ItemMargePos=pos;
	Locked(pos);

}
void CBagFrame::CloseItemMarge(void)
{

	// 關閉融合界面
	SendWorldEvent("ITEMMARGE_CLOSE");
	if (m_ItemMargePos!=-1)
	Unlocked(m_ItemMargePos);
	m_ItemMargePos=-1;
}

// ----------------------------------------------------------------------------
bool CBagFrame::Use(int index, bool fromBag)
{	
	ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(index);
	if ( item && !item->IsEmpty() && item->Pos == EM_ItemState_NONE )
	{
		GameObjDbStruct* itemDB = CNetGlobal::GetObj(item->OrgObjID);

		// 若商店開啟中
		if ( fromBag && g_pStoreFrame->IsOpen() && index >= _MAX_BODY_EX_COUNT_ )
		{
			g_pStoreFrame->Sell(index);
		}
		// 若銀行開啟中
		else if ( fromBag && g_pBankFrame->IsOpen() )
		{
			int bankPos=-1;
#if USE_BANK_STORE ==1
			if (itemDB&&itemDB->Mode.DepartmentStore)// 如果是商城物品		
			{
				
				int storeBank=200;
				for (int i=0;i<100;i++)
				{
					ItemFieldStruct* itemS =CNetGlobal::RoleData()->GetBankItem(storeBank);//優先放入商城背包銀行
					if (itemS&&!itemS->IsEmpty())
					{
						if (itemS->OrgObjID==item->OrgObjID)
						{
							if( itemS->Count + 1 <= max(itemDB->MaxHeap, 1) )								
							{
								bankPos=storeBank;
								break;
							}						
						}					
					}
					storeBank++;
				}
				if (bankPos==-1)
				{
					storeBank=200;
					for (int i=0;i<100;i++)
					{
						ItemFieldStruct* itemS =CNetGlobal::RoleData()->GetBankItem(storeBank);//優先放入商城背包銀行
						if (itemS&&itemS->IsEmpty())
					 {
						 bankPos=storeBank;
						 break;
					 }
						storeBank++;
					}
				}
			
			}
#endif
			if ( bankPos == -1 )
				bankPos = CNetGlobal::RoleData()->SearchBankEmptyPos(item, true, 0);
		
			if ( bankPos == -1 )
			{
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_BANKITEM_FULL"));
			}
			else
			{
				ItemCommand itemCommand;
				itemCommand.type		= ITEM_COMMAND_USE_BAG;
				itemCommand.index		= index;
				itemCommand.fromBag		= fromBag;
				itemCommand.objiectID	= item->OrgObjID;
				itemCommand.serial		= item->Serial;
				itemCommand.createTime	= item->CreateTime;
				int id = g_pGameMain->PushItemCommand(itemCommand);

				if ( g_pBankFrame->Deposit( bankPos, CBankFrame::eBank_Type_Bag, index, false, id ) )
				{
					g_pRuGlobalEventManager->PlaySoundByFile("sound\\interface\\ui_icon_drop.mp3", false, 0.0f, ruSOUNDTYPE_INTERFACE);
				}
			}
		}
		//丟到房屋
		else if( fromBag && g_pHousesFrame->DepositFromBag( index ) )
		{
			g_pRuGlobalEventManager->PlaySoundByFile("sound\\interface\\ui_icon_drop.mp3", false, 0.0f, ruSOUNDTYPE_INTERFACE);
		}
		//丟到套裝技能淬取
		else if ( fromBag && g_pSuitSkillFrame->IsOpen() && itemDB != NULL )
		{
			switch (itemDB->Classification)
			{
			case EM_ObjectClass_Item:
				switch ( itemDB->Item.Class )
				{
				case EM_Item_Weapon:
					switch ( itemDB->Item.WeaponPos )
					{
					case EM_WeaponPos_MainHand:			// 主手
					case EM_WeaponPos_TwoHand:			// 兩手
						g_pSuitSkillFrame->InsertItem(EM_EQWearPos_MainHand, 0, index);
						break;

					case EM_WeaponPos_OneHand:			// 雙手都可以裝備
					case EM_WeaponPos_SecondHand:		// 副手
						g_pSuitSkillFrame->InsertItem(EM_EQWearPos_SecondHand, 0, index);
						break;

					case EM_WeaponPos_Ammo:				// 子彈
						g_pSuitSkillFrame->InsertItem(EM_EQWearPos_Ammo, 0, index);
						break;

					case EM_WeaponPos_Bow:				// 弓
						g_pSuitSkillFrame->InsertItem(EM_EQWearPos_Bow, 0, index);
						break;
					}
					break;

				case EM_Item_Armor:
					switch (itemDB->Item.ArmorPos)
					{
					case EM_ArmorPos_Head:
						g_pSuitSkillFrame->InsertItem(EM_EQWearPos_Head, 0, index);
						break;
					case EM_ArmorPos_Gloves:
						g_pSuitSkillFrame->InsertItem(EM_EQWearPos_Gloves, 0, index);
						break;
					case EM_ArmorPos_Shoes:
						g_pSuitSkillFrame->InsertItem(EM_EQWearPos_Shoes, 0, index);
						break;
					case EM_ArmorPos_Clothes:
						g_pSuitSkillFrame->InsertItem(EM_EQWearPos_Clothes, 0, index);
						break;
					case EM_ArmorPos_Pants:
						g_pSuitSkillFrame->InsertItem(EM_EQWearPos_Pants, 0, index);
						break;
					case EM_ArmorPos_Back:
						g_pSuitSkillFrame->InsertItem(EM_EQWearPos_Back, 0, index);
						break;
					case EM_ArmorPos_Belt:
						g_pSuitSkillFrame->InsertItem(EM_EQWearPos_Belt, 0, index);
						break;
					case EM_ArmorPos_Shoulder:
						g_pSuitSkillFrame->InsertItem(EM_EQWearPos_Shoulder, 0, index);
						break;
					case EM_ArmorPos_Necklace:
						g_pSuitSkillFrame->InsertItem(EM_EQWearPos_Necklace, 0, index);
						break;
					case EM_ArmorPos_Ring:
						{
							// 如果左有就換右
							SoubleItemLocked* item = g_pSuitSkillFrame->GetSoubleItem(EM_EQWearPos_Ring1);
							if ( item == NULL )
								g_pSuitSkillFrame->InsertItem(EM_EQWearPos_Ring1, 0, index);
							else
								g_pSuitSkillFrame->InsertItem(EM_EQWearPos_Ring2, 0, index);
						}
						break;

					case EM_ArmorPos_Earring:
						{
							// 如果左有就換右
							SoubleItemLocked* item = g_pSuitSkillFrame->GetSoubleItem(EM_EQWearPos_Earring1);
							if ( item == NULL )
								g_pSuitSkillFrame->InsertItem(EM_EQWearPos_Earring1, 0, index);
							else
								g_pSuitSkillFrame->InsertItem(EM_EQWearPos_Earring1, 0, index);
						}
						break;

					case EM_ArmorPos_SecondHand:
						g_pSuitSkillFrame->InsertItem(EM_EQWearPos_SecondHand, 0, index);
						break;

					case EM_ArmorPos_MagicTool:
						{
							/*
							ItemFieldStruct* item = CNetGlobal::RoleData()->GetEqItem(EM_EQWearPos_MagicTool1);
							if ( item && item->IsEmpty() )
								g_pCharacterFrame->Equip(EM_EQWearPos_MagicTool1, 0, index);
							else
							{
								item = CNetGlobal::RoleData()->GetEqItem(EM_EQWearPos_MagicTool2);
								if ( item && item->IsEmpty() )
									g_pCharacterFrame->Equip(EM_EQWearPos_MagicTool2, 0, index);
								else
								{
									item = CNetGlobal::RoleData()->GetEqItem(EM_EQWearPos_MagicTool3);
									if ( item && item->IsEmpty() )
										g_pCharacterFrame->Equip(EM_EQWearPos_MagicTool3, 0, index);
									else
										g_pCharacterFrame->Equip(EM_EQWearPos_MagicTool1, 0, index);
								}
							}
							*/
						}
						break;

					case EM_ArmorPos_Ornament:
						g_pSuitSkillFrame->InsertItem(EM_EQWearPos_Ornament, 0, index);
						break;
					}				
					break;
				}
			}
		}
		else if ( itemDB != NULL )
		{
			// 在坐騎上不能使用物品
			CRoleSprite* player = g_pGameMain->GetPlayer();
			if ( player && player->GetAttachParentId() )
			{
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_USE_ITEM_MOUNT_ERROR"));
				return false;
			}

			if ( itemDB->Limit.Level > CNetGlobal::RoleBaseAttr()->Level )
			{
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_LIMIT_LEVEL"));
				return false;
			}

			switch (itemDB->Classification)
			{
			case EM_ObjectClass_Item:
				switch ( itemDB->Item.Class )
				{
				case EM_Item_Weapon:
					switch ( itemDB->Item.WeaponPos )
					{
					case EM_WeaponPos_MainHand:			// 主手
					case EM_WeaponPos_OneHand:			// 雙手都可以裝備
					case EM_WeaponPos_TwoHand:			// 兩手
						g_pCharacterFrame->Equip(EM_EQWearPos_MainHand, 0, index);
						break;

					case EM_WeaponPos_SecondHand:		// 副手
						g_pCharacterFrame->Equip(EM_EQWearPos_SecondHand, 0, index);
						break;

					case EM_WeaponPos_Ammo:				// 子彈
						g_pCharacterFrame->Equip(EM_EQWearPos_Ammo, 0, index);
						break;

					case EM_WeaponPos_Bow:				// 弓
						g_pCharacterFrame->Equip(EM_EQWearPos_Bow, 0, index);
						break;
					}
					break;

				case EM_Item_Armor:
					switch (itemDB->Item.ArmorPos)
					{
					case EM_ArmorPos_Head:
						g_pCharacterFrame->Equip(EM_EQWearPos_Head, 0, index);
						break;
					case EM_ArmorPos_Gloves:
						g_pCharacterFrame->Equip(EM_EQWearPos_Gloves, 0, index);
						break;
					case EM_ArmorPos_Shoes:
						g_pCharacterFrame->Equip(EM_EQWearPos_Shoes, 0, index);
						break;
					case EM_ArmorPos_Clothes:
						g_pCharacterFrame->Equip(EM_EQWearPos_Clothes, 0, index);
						break;
					case EM_ArmorPos_Pants:
						g_pCharacterFrame->Equip(EM_EQWearPos_Pants, 0, index);
						break;
					case EM_ArmorPos_Back:
						g_pCharacterFrame->Equip(EM_EQWearPos_Back, 0, index);
						break;
					case EM_ArmorPos_Belt:
						g_pCharacterFrame->Equip(EM_EQWearPos_Belt, 0, index);
						break;
					case EM_ArmorPos_Shoulder:
						g_pCharacterFrame->Equip(EM_EQWearPos_Shoulder, 0, index);
						break;
					case EM_ArmorPos_Necklace:
						g_pCharacterFrame->Equip(EM_EQWearPos_Necklace, 0, index);
						break;
					case EM_ArmorPos_Ring:
						{// 如果左有就換右
							ItemFieldStruct* item= CNetGlobal::RoleData()->GetEqItem(EM_EQWearPos_Ring1);
							if (item->IsEmpty())
								g_pCharacterFrame->Equip(EM_EQWearPos_Ring1, 0, index);
							else
								g_pCharacterFrame->Equip(EM_EQWearPos_Ring2, 0, index);
						}
						break;
					case EM_ArmorPos_Earring:
						{// 如果左有就換右
							ItemFieldStruct* item= CNetGlobal::RoleData()->GetEqItem(EM_EQWearPos_Earring1);
							if (item->IsEmpty())
								g_pCharacterFrame->Equip(EM_EQWearPos_Earring1, 0, index);
							else
								g_pCharacterFrame->Equip(EM_EQWearPos_Earring2, 0, index);
						}
					
						break;
					case EM_ArmorPos_SecondHand:
						g_pCharacterFrame->Equip(EM_EQWearPos_SecondHand, 0, index);
						break;
					case EM_ArmorPos_MagicTool:
						{
							ItemFieldStruct* item = CNetGlobal::RoleData()->GetEqItem(EM_EQWearPos_MagicTool1);
							if ( item && item->IsEmpty() )
								g_pCharacterFrame->Equip(EM_EQWearPos_MagicTool1, 0, index);
							else
							{
								item = CNetGlobal::RoleData()->GetEqItem(EM_EQWearPos_MagicTool2);
								if ( item && item->IsEmpty() )
									g_pCharacterFrame->Equip(EM_EQWearPos_MagicTool2, 0, index);
								else
								{
									item = CNetGlobal::RoleData()->GetEqItem(EM_EQWearPos_MagicTool3);
									if ( item && item->IsEmpty() )
										g_pCharacterFrame->Equip(EM_EQWearPos_MagicTool3, 0, index);
									else
										g_pCharacterFrame->Equip(EM_EQWearPos_MagicTool1, 0, index);
								}
							}
						}
						break;

					case EM_ArmorPos_Ornament:
						g_pCharacterFrame->Equip(EM_EQWearPos_Ornament, 0, index);
						break;
					}				
					break;

				case EM_Item_Rune:
					if ( m_useIndex != -1 )
						return false;
					else
					{
						item->Pos = EM_USE_ITEM;
						SetUseItem(index);
					}
					break;

				case EM_Item_Card:
					if ( CNetGlobal::RoleData()->CheckCardID(item->OrgObjID) )
						g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_ITEM_CARD_ALREADY"));
					else
						g_pGameMain->UseBagItem(index);
					break;

				default:
					// 檢查Update 不能使用物品
					if ( g_pGameMain->IsUIUpdate() )
						return false;

					// 檢查物品是否已鎖定
					if ( index >= 120 && CNetGlobal::RoleData()->PlayerBaseData->TimeFlag[EM_TimeFlagEffect_BagPage3 + (index - 120) / 30] < 0 )
						return false;
					//清潔石

					int DBID=g_ObjectData->GetSysKeyValue("AttributeClearItemID");
					if ( item->OrgObjID==DBID)
					{
						SendWorldEvent("OPEN_CLEARATTRIBSTONE_FRAME"); 
						return true;
					}

					// 寵物物品
					if ( itemDB->Item.ItemType == EM_ItemType_Pet )
					{
						if ( m_useIndex != -1 )
							return false;
						else
						{
							item->Pos = EM_USE_ITEM;
							SetUseItem(index);
							SendWorldEvent("OPEN_PET_FRAME");
						}
					}
					else
					{
						switch (itemDB->Item.UseType)
						{
						case EM_ItemUseType_Magic:
							// 檢查物品是否有法術效果
							if ( itemDB->Item.IncMagic_Onuse != 0 )
							{
								// 檢查法術類型,對地使用
								GameObjDbStruct* magicDB = g_ObjectData->GetObj(itemDB->Item.IncMagic_Onuse);

								// 在水中不可以使用
								if ( magicDB && magicDB->MagicCol.IsWaterDisabled == true && g_pGameMain->IsInWater() )
								{
									g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_GAMEMSGEVENT_172"));
									return false;
								}

								if ( magicDB && magicDB->MagicCol.TargetType == EM_Releation_Locatoin )
								{
									if ( m_useToLocation != -1 )
										return false;

									item->Pos = EM_USE_ITEM;
									m_useToLocation = index;

									// 顯示選取區域
									g_pGameMain->DisplaySelectAreaEntity((float)magicDB->MagicCol.AttackDistance, magicDB->MagicCol.EffectRange / 25.0f);
								} else {
									g_pGameMain->UseBagItem(index);
								}							
							}
							break;

						case EM_ItemUseType_SrvScript:
						case EM_ItemUseType_UnPacket:
						case EM_ItemUseType_UnPacket_DropList:
						case EM_ItemUseType_Egg:
							g_pGameMain->UseBagItem(index);
							break;

						case EM_ItemUseType_CliScript:
							break;

						case EM_ItemUseType_EqRefine:		//衝等
						case EM_ItemUseType_FixEq:			//修裝
						case EM_ItemUseType_Dissolution:	//拆解物品
						case EM_ItemUseType_Coloring:		//拆解物品
						case EM_ItemUseType_ClsEqSoulbound:	//清除Soulbound
						case EM_ItemUseType_ClsTrade:		//開啟交易
						case EM_ItemUseType_EQDigHole:		//裝備挖洞
						case EM_ItemUseType_EQClearRune:	//清除裝備符文
						case EM_ItemUseType_ItemLock:		//鎖定裝備
						case EM_ItemUseType_ItemUnLock:		//解鎖裝備
						case EM_ItemUseType_EQProtect:		//裝備保護
						case EM_ItemUseType_EQLimitMax:		//裝備耐久上限到Max
						case EM_ItemUseType_IgnoreLimitLv:	//忽略等級限制
						case EM_ItemUseType_PK_EQProtect:	//PK裝備保護
						case EM_ItemUseType_PK_EQProtect_Clear:	//PK裝備保護清除
						case EM_ItemUseType_ClsEqAbility:
						case EM_ItemUseType_ItemtoItemLua:	//物品對物品觸發lua
							if ( m_useIndex != -1 )
								return false;

							item->Pos = EM_USE_ITEM;
							SetUseItem(index);
							break;
						case EM_ItemUseType_ResetAbility:		
						case EM_ItemUseType_ResetSkillPoint:	//重設技能點數
							{
								g_pGameMain->UseBagItem(index);
							}
							break;
						}
					}					
					break;
				}
				break;

			case EM_ObjectClass_Recipe:
				g_pGameMain->UseBagItem(index);
				break;
			}
		}
		return true;
	}
	return false;
}

// ----------------------------------------------------------------------------
bool CBagFrame::UseItemByName(const char* name)
{
	return Use(FindIndexByName(name));
}

// ----------------------------------------------------------------------------
bool CBagFrame::Cut(int index)
{	
	ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem( index );
	if ( item )
	{
		GameObjDbStruct* itemDB = CNetGlobal::GetObj( item->OrgObjID );
		if ( itemDB )
		{
			item->Pos = EM_ItemState_CliDisabled;
			m_pickupItemIndex = index;
			g_pItemClipboard->SetFile(ItemUTIL::GetItemIcon(item));			
			g_pItemClipboard->PickupBag(item->OrgObjID, index);

			// g_pError->AddMessage(0, 0, "Bag cut EM_ItemState_CliDisabled [%d]", index);
			return true;
		}
	}
	return false;
}

// ----------------------------------------------------------------------------
void CBagFrame::Paste(int index)
{	
	ClipboardItemType itemType = g_pItemClipboard->GetType();
	ClipboardItemInfo itemInfo = g_pItemClipboard->GetItemInfo();
	ItemFieldStruct* destItem = CNetGlobal::RoleData()->GetBodyItem(index);
	// 拿取與做用物品相同
	if ( itemType == eClipboardItem_Type_Bag )
	{
		if (index == itemInfo.bag.pos)
		{
			destItem->Pos = EM_ItemState_NONE;
			m_pickupItemIndex = -1;
			g_pItemClipboard->Clear();
			return;
		}
	}

	//物品融合道具不能移動
	if (m_ItemMargePos==index)
	{
		destItem->Pos=EM_ItemState_CliDisabled;
		return ;
	}

	// 物品不存在或者該物品被鎖定中
	if ( destItem == NULL || destItem->Pos != EM_ItemState_NONE )
		return;


	if ( CNetGlobal::RoleData()->CheckBodyItemEnable(index)	== false )
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_BAG_RENT_ERROR"));
		return;
	}	

	// 存在物品對物品使用中
	if ( m_useIndex != -1 )
	{
		if ( !UseItemToItem(0, index) )
			UseItemCancel();
		return;
	}
	// 存在商店修理物品
	else if ( g_pStoreFrame->GetRepairState() )
	{
		switch (RoleDataEx::CheckFixEq( NULL , destItem, CNetGlobal::RoleData()->BodyMoney() ))
		{
		case EM_CheckFixEqResult_OK:				
			NetCli_Shop::S_RepairWeaponRequest(index, false);
			break;

		case EM_CheckFixEqResult_DataErr:
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_FIXEQ_DATA_ERROR"));
			break;

		case EM_CheckFixEqResult_NotFix:
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_FIXEQ_NOT_FIX"));
			break;

		case EM_CheckFixEqResult_NotEQ:
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_FIXEQ_NOT_EQ"));
			break;

		case EM_CheckFixEqResult_NewOne:
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_FIXEQ_NOT_BREAK"));
			break;

		case EM_CheckFixEqResult_NotEnoughMoney:
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_AC_NOT_ENOUGH_MONEY"));
			break;
		}
		return;
	}
	// 物品對物品使用
	else if ( g_pGameMain->GetCastSpellToItemID() != 0 )
	{
		if ( destItem->IsEmpty() )
		{
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_USEITEM_TARGET_ERROR"));		// 對象錯誤
		}

		g_pGameMain->ItemCastingStart(ITEM_CASTING_DISSOLUTION, -1, 0, index);		
		return;
	}

	ItemFieldStruct* srcItem = NULL;
	GameObjDbStruct* srcItemDB = NULL;
	switch (itemType)
	{
	case eClipboardItem_Type_None:					// 拿取物品
		Cut(index);
		return;

	case eClipboardItem_Type_Bag:					// 背包 -> 背包
		srcItem = CNetGlobal::RoleData()->GetBodyItem(itemInfo.bag.pos);
		if ( srcItem == NULL )
			return;

		srcItemDB = g_ObjectData->GetObj(srcItem->OrgObjID);

		// 由背包內放回商城背包內
		if ( index < _MAX_BODY_EX_COUNT_ )
		{
			// 檢查不是在商城背包內,需要為商城物品才能成立
			if ( itemInfo.bag.pos >= _MAX_BODY_EX_COUNT_ && (srcItemDB && srcItemDB->Mode.DepartmentStore == false) )
			{
				// 不能放回商城背包
				//g_pGameMain->SendWarningMsg( g_ObjectData->GetString("SYS_BAG_TO_GOODS_FAILED") );
				g_pGameMain->SendWarningMsg( g_ObjectData->GetString("SYS_CANNOT_MOVE_TO_ACCOUNTBAG") );
				return;
			}
		}
		// 來源為商城背包要與背包內欄位做交換
		else if ( itemInfo.bag.pos < _MAX_BODY_EX_COUNT_ )
		{
			GameObjDbStruct* destItemDB = g_ObjectData->GetObj(destItem->OrgObjID);
			if ( destItem->IsEmpty() == false && (destItemDB && destItemDB->Mode.DepartmentStore == false) )
			{
				// 不能放回商城背包
				// g_pGameMain->SendWarningMsg( g_ObjectData->GetString("SYS_BAG_TO_GOODS_FAILED") );
				g_pGameMain->SendWarningMsg( g_ObjectData->GetString("SYS_CANNOT_MOVE_TO_ACCOUNTBAG") );
				return;
			}
		}
		
		if ( destItem && srcItem )
		{
			ClearPickupItemIndex();
			destItem->Pos = srcItem->Pos = EM_ItemState_CliDisabled;
			NetCli_Item::_ExchangeField_Body(*srcItem, itemInfo.bag.pos, *destItem , index);
		}
		break;

	case eClipboardItem_Type_SplitBagItem:					// 分堆 -> 背包
		if ( (srcItem = CNetGlobal::RoleData()->GetBodyItem(itemInfo.splitBagItem.pos)) == NULL )
			return;

		srcItemDB = g_ObjectData->GetObj(srcItem->OrgObjID);

		// 目的位置
		if ( index < _MAX_BODY_EX_COUNT_ )
		{
			// 放回商城背包內
			if ( itemInfo.splitBagItem.pos >= _MAX_BODY_EX_COUNT_ && (srcItemDB && srcItemDB->Mode.DepartmentStore == false) )
			{
				// 不能放回商城背包
				// g_pGameMain->SendWarningMsg( g_ObjectData->GetString("SYS_BAG_TO_GOODS_FAILED") );
				g_pGameMain->SendWarningMsg( g_ObjectData->GetString("SYS_CANNOT_MOVE_TO_ACCOUNTBAG") );
				return;
			}
		}

		if( destItem->IsEmpty() == false )
		{
			//break;
		}
		srcItem = CNetGlobal::RoleData()->GetBodyItem( itemInfo.splitBagItem.pos );
  		srcItem->Pos = EM_ItemState_CliDisabled;						// Client鎖定
		destItem->Pos = EM_ItemState_CliDisabled;						// Client鎖定
		NetCli_Item::_ApartBodyItemRequest( *srcItem, itemInfo.splitBagItem.pos, itemInfo.splitBagItem.count, index );
	
		break;
	case eClipboardItem_Type_Bank:			
		{
			// 銀行 -> 背包
			if ( (srcItem = CNetGlobal::RoleData()->GetBankItem(itemInfo.bank.pos)) == NULL )
				return;
			srcItemDB = g_ObjectData->GetObj(srcItem->OrgObjID);

			bool ErrorForDepartmentStore=false;
			int srcindex=itemInfo.bank.pos;

			if (destItem&&srcindex>=200&&srcindex<_MAX_BANK_COUNT_)//商城物品銀行200~300
			{
				GameObjDbStruct* destItemDB = g_ObjectData->GetObj(destItem->OrgObjID);
				if (destItemDB&& destItemDB->Mode.DepartmentStore == false) 
				{
					// 不能放到銀行商城背包
					ErrorForDepartmentStore=true;
				}
			}

			if ( index < _MAX_BODY_EX_COUNT_ && (srcItemDB && srcItemDB->Mode.DepartmentStore == false) )
			{
				// 不能放回商城背包
				// g_pGameMain->SendWarningMsg( g_ObjectData->GetString("SYS_BANK_TO_GOODS_FAILED") );
				ErrorForDepartmentStore=true;
			}
			if (ErrorForDepartmentStore)
			{
				g_pGameMain->SendWarningMsg( g_ObjectData->GetString("SYS_CANNOT_MOVE_TO_ACCOUNTBAG") );
				return;
			}
			else
			{
				//解除鎖定
				ItemFieldStruct* bankItem = CNetGlobal::RoleData()->GetBankItem(itemInfo.bank.pos);
				if( bankItem )
				{
					bankItem->Pos = EM_ItemState_NONE;
				}

				g_pBankFrame->Deposit(itemInfo.bank.pos, CBankFrame::eBank_Type_Bag, index, true);
			}
			break;
		}
		

	case eClipboardItem_Type_Equipment:				// 裝備 -> 背包
		if ( (srcItem = CNetGlobal::RoleData()->GetBodyItem(itemInfo.equipment.pos)) == NULL )
			return;
		srcItemDB = g_ObjectData->GetObj(srcItem->OrgObjID);

		if ( index < _MAX_BODY_EX_COUNT_ && (srcItemDB && srcItemDB->Mode.DepartmentStore == false) )
		{
			// 不能放回商城背包
			// g_pGameMain->SendWarningMsg( g_ObjectData->GetString("SYS_EQ_TO_GOODS_FAILED") );
			g_pGameMain->SendWarningMsg( g_ObjectData->GetString("SYS_CANNOT_MOVE_TO_ACCOUNTBAG") );
			return;
		}
		g_pCharacterFrame->Equip(itemInfo.equipment.pos, 0, index);
		break;

	case eClipboardItem_Type_Shop:					// 商店 -> 背包
		if ( (srcItem = CNetGlobal::RoleData()->GetBodyItem(itemInfo.shop.pos)) == NULL )
			return;
		srcItemDB = g_ObjectData->GetObj(srcItem->OrgObjID);

		if ( index < _MAX_BODY_EX_COUNT_ && (srcItemDB && srcItemDB->Mode.DepartmentStore == false) )
		{
			// 不能放回商城背包
			// g_pGameMain->SendWarningMsg( g_ObjectData->GetString("SYS_STORE_TO_GOODS_FAILED") );
			g_pGameMain->SendWarningMsg( g_ObjectData->GetString("SYS_CANNOT_MOVE_TO_ACCOUNTBAG") );
			return;
		}
		g_pStoreFrame->Buy(itemInfo.shop.type, itemInfo.shop.pos, 1);
		break;

	case eClipboardItem_Type_House:					//房屋系統 -> 背包
		if ( index < _MAX_BODY_EX_COUNT_ )
		{
			int ParentID = itemInfo.house.parentID;
			int Index = itemInfo.house.pos;
			HouseItemDBStruct* pDestHouseItemDB = g_pHousesFrame->GetHousesItemDB( ParentID , Index );
			GameObjDbStruct* pDestItemDB = CNetGlobal::GetObj( pDestHouseItemDB->Item.OrgObjID );
			if ( pDestItemDB && !pDestItemDB->Mode.DepartmentStore )
			{
				// 不能放回商城背包
				// g_pGameMain->SendWarningMsg( g_ObjectData->GetString("SYS_HOUSE_TO_GOODS_FAILED") );
				g_pGameMain->SendWarningMsg( g_ObjectData->GetString("SYS_CANNOT_MOVE_TO_ACCOUNTBAG") );
				return;
			}
		}

		{
			if( destItem->OrgObjID == 0 ) //空的
			{
				//交換物品
				NetCli_Houses::S_SwapBodyHouseItem( index , itemInfo.house.pos , itemInfo.house.parentID , false );
			}
			else
			{
				int ParentID = itemInfo.house.parentID;
				int Index = itemInfo.house.pos;
				g_pItemClipboard->Clear();
				g_pItemClipboard->PickupBag(  destItem->OrgObjID , index );
				g_pHousesFrame->PickupItem( ParentID , Index, false, true );
			}
		}
		break;

	case eClipboardItem_Type_GuildBank:				// 公會銀行 -> 背包
		{
			if ( index < _MAX_BODY_EX_COUNT_ )
			{
				// 不能放回商城背包
				g_pGameMain->SendWarningMsg( g_ObjectData->GetString("SYS_GUILDHOUSE_TO_GOODS_FAILED") );
				return;
			}

			NetCli_GuildHouses::SL_SwapBodyHouseItem( index , itemInfo.guildbank.pos / DF_GuildBankItemMax , itemInfo.guildbank.pos % DF_GuildBankItemMax , false );
		}

		break;

	case eClipboardItem_Type_GuildFurniture:	// 公會傢俱 -> 背包
		{
			if ( index < _MAX_BODY_EX_COUNT_ )
			{
				// 不能放回商城背包
				g_pGameMain->SendWarningMsg( g_ObjectData->GetString("SYS_HOUSE_TO_GOODS_FAILED") );
				return;
			}

			NetCli_GuildHouses::SL_SwapBodyHouseFurniture( index , itemInfo.guildfurniture.pos );
		}
		break;

	case eClipboardItem_Type_PetEQ:
		{
			if ( index < _MAX_BODY_EX_COUNT_ )
			{
				// 不能放回商城背包
				g_pGameMain->SendWarningMsg( g_ObjectData->GetString("SYS_PET_TO_GOODS_FAILED") );
				return;
			}

			g_pPetFrame->SwapItem(itemInfo.petEQ.index, itemInfo.petEQ.pos, index, false);
		}
		break;

	case eClipboardItem_Type_Trade:
	case eClipboardItem_Type_Action:
	case eClipboardItem_Type_Skill:
	case eClipboardItem_Type_Macro:
	case eClipboardItem_Type_Emote:
		break;
	}

	// clear item clipboard data
	g_pItemClipboard->Clear();
}

// ----------------------------------------------------------------------------
void CBagFrame::SetUseItem(int index)
{
	m_useIndex = index;
	CancelUseItemToLocation();
}

// ----------------------------------------------------------------------------
bool CBagFrame::UseItemToItem(int targetType, int index)
{
	int srcIndex = (m_useIndex == -1) ? m_unlockSlot : m_useIndex;
	if ( srcIndex == -1 )
		return false;

	ItemFieldStruct* srcItem = CNetGlobal::RoleData()->GetBodyItem(srcIndex);	
	ItemFieldStruct* destItem = NULL;	

	switch (targetType)
	{
	case 0:		// 背包
		destItem = CNetGlobal::RoleData()->GetBodyItem(index);
		break;

	case 1:		// 裝備
		destItem = CNetGlobal::RoleData()->GetEqItem(index);
		break;

	case 2:		// 銀行
		destItem = CNetGlobal::RoleData()->GetBankItem(index);
		break;
	}

	if ( destItem == NULL )
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_USEITEM_TARGET_ERROR"));
		return false;			// 對象錯誤
	}
	
	// 判斷來源物品類型
	GameObjDbStructEx* itemDB = CNetGlobal::GetObj(srcItem->OrgObjID);
	switch( itemDB->Item.Class )
	{
	case EM_Item_Rune:
		switch(RoleDataEx::CheckAddRune(srcItem, destItem))
		{
		case EM_CheckAddRuneResult_OK:			
			break;
		case EM_CheckAddRuneResult_TargetItemErr:		//目標物品有問題
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_ADDRUNE_TARGET_ERROR"));
			return false;
		case EM_CheckAddRuneResult_NotRune:				//不是符文
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_ADDRUNE_NOTRUNE"));
			return false;
		case EM_CheckAddRuneResult_RuneVolumeErr:		//符文容量不足
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_ADDRUNE_VOLUME_NOT_ENOUGH"));
			return false;
		case EM_CheckAddRuneResult_WeaponErr:			//鑲嵌武器類型有問題
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_ADDRUNE_WEAPON_ERROR"));
			return false;
		case EM_CheckAddRuneResult_ArmorErr:			//鑲嵌防具類型有問題		
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_ADDRUNE_ARMOR_ERROR"));
			return false;
		case EM_CheckAddRuneResult_RuneDuplicate:		//符文重復
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_ADDRUNE_RUNEDUPLICATE"));
			return false;
		case EM_CheckAddRuneResult_RuneFull:			//符文滿格
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_ADDRUNE_RUNEFULL"));
			return false;				
		case EM_CheckAddRuneResult_LevelError:			//使用的等級有問題
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_ADDRUNE_LEVELERROR"));
			return false;
		default:
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_ADDRUNE_UNKNOW"));
			return false;
		}
		break;

	default:
		switch (itemDB->Item.UseType)
		{
		case EM_ItemUseType_EqRefine:		//衝等
			switch (RoleDataEx::CheckEqRefine(srcItem , destItem))
			{
			case EM_CheckEqRefineResult_OK:					//OK
				break;
			case EM_CheckEqRefineResult_DataErr:			//資料有問題
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_REFINE_DATA_ERROR"));
				return false;
			case EM_CheckEqRefineResult_MaxLevel:			//已到等級上限
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_REFINE_MAXLEVEL"));
				return false;
			case EM_CheckEqRefineResult_NotWeapon:			//只能精鍊武器類型
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_REFINE_NOTWEAPON"));
				return false;
			case EM_CheckEqRefineResult_NotArmor:			//只能精鍊防具類型
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_REFINE_NOTARMOR"));
				return false;
			case EM_CheckEqRefineResult_NotJewels:			//只能精鍊首飾類型
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_REFINE_NOTJEWELS"));
				return false;
			case EM_CheckEqRefineResult_ItemLimitMinLevel:	//可精鍊武器等級太低有問題
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_REFINE_EXCEED_LEVEL"));
				return false;
			case EM_CheckEqRefineResult_ItemLimitMaxLevel:	//可精鍊武器等級已達上限有問題
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_REFINE_MAX_LEVEL"));
				return false;
			case EM_CheckEqRefineResult_NotOrnament:		//只能裝飾品類型
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_REFINE_NOTORNAMENT"));
				return false;
			case EM_CheckEqRefineResult_CantRefine:
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_REFINE_DISABLE"));
				return false;
			default:
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_REFINE_UNKNOW"));
				return false;
			}
			break;

		case EM_ItemUseType_Dissolution:	//拆解物品
			switch (RoleDataEx::CheckDissolution(srcItem,destItem))
			{
			case EM_CheckDissolutionResult_OK:
				break;
			case EM_CheckDissolutionResult_Err:			//此物品不可拆解
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_DISSOLUTION_ERROR"));
				return false;
			default:
				return false;
			}
			break;

		case EM_ItemUseType_Coloring:		//染色
			switch (RoleDataEx::CheckColoring(srcItem,destItem))
			{
			case EM_CheckColoringResult_OK:
				break;
			case EM_CheckColoringResult_Err:		//此物品不可染色
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_COLORING_ITEM_ERROR"));
				return false;
			default:
				return false;
			}
			break;

		case EM_ItemUseType_ClsEqSoulbound:		//清除Soulbound
			if( targetType == 1 )
			{
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CLSSOULDBOUND_ITEM_ERR3"));
				return false;
			}
			switch (RoleDataEx::CheckClsSoulBound(srcItem,destItem)) 
			{
			case EM_CheckClsSoulBoundResult_OK					:	//OK
				break;
			case EM_CheckClsSoulBoundResult_Err					:	//此物品不可清除Soulbond
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CLSSOULDBOUND_ITEM_ERR4"));
				break;
			case EM_CheckClsSoulBoundResult_Err_NoSoulBound		:	//此物品目前沒Soulbond
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CLSSOULDBOUND_ITEM_ERR1"));
				break;
			case EM_CheckClsSoulBoundResult_Err_NoEqSoulBound	:	//非裝備綁定不能解除
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CLSSOULDBOUND_ITEM_ERR2"));
				break;
			default:
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CLSSOULDBOUND_ITEM_ERR0"));
				return false;
			}
			break;
		case EM_ItemUseType_ClsTrade:		//清除Soulbound
			if( targetType == 1 )
			{
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CLSSOULDBOUND_ITEM_ERR3"));
				return false;
			}
			switch (RoleDataEx::CheckClsTrade(srcItem,destItem)) 
			{
			case EM_CheckClsSoulBoundResult_OK					:	//OK
				break;
			case EM_CheckClsSoulBoundResult_Err					:	//此物品不可清除Soulbond
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CLSSOULDBOUND_ITEM_ERR4"));
				break;
			case EM_CheckClsSoulBoundResult_Err_NoSoulBound		:	//此物品目前沒Soulbond
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CLSSOULDBOUND_ITEM_ERR1"));
				break;
			case EM_CheckClsSoulBoundResult_Err_NoEqSoulBound	:	//非裝備綁定不能解除
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CLSSOULDBOUND_ITEM_ERR2"));
				break;
			default:
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CLSSOULDBOUND_ITEM_ERR0"));
				return false;
			}
			break;

		case EM_ItemUseType_FixEq:			//修裝
			switch ( RoleDataEx::CheckFixEq(srcItem,destItem) )
			{
			case EM_CheckFixEqResult_OK:
				break;
			case EM_CheckFixEqResult_DataErr:		//資料有問題
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_FIXEQ_DATA_ERROR"));
				return false;
			case EM_CheckFixEqResult_NotEQ:			//非裝備
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_FIXEQ_NOT_EQ"));
				return false;
			case EM_CheckFixEqResult_NewOne:		//物品沒損壞
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_FIXEQ_NOT_BREAK"));
				return false;
			case EM_CheckFixEqResult_NotFix:		//物品不能修
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_FIXEQ_NOT_FIX"));
				return false;
			default:
				return false;
			}
			break;

		//case EM_ItemUseType_ResetSkillPoint:	//重設技能點數
		case EM_ItemUseType_EQDigHole:			//裝備挖洞
			switch (RoleDataEx::CheckDigHole(srcItem , destItem))
			{
			case EM_CheckDigHoleResult_OK:					//OK
				break;
			case EM_CheckDigHoleResult_TargetErr:			//不可對此物品挖洞
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_HOLE_ERR01"));
				return false;
			case EM_CheckDigHoleResult_SrcErr:				//使用物品有問題
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_HOLE_ERR02"));
				return false;
			case EM_CheckDigHoleResult_WeaponOnly:			//只能對武器使用
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_HOLE_ERR03"));
				return false;
			case EM_CheckDigHoleResult_ArmorOnly:			//只能對防具使用
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_HOLE_ERR04"));
				return false;
			case EM_CheckDigHoleResult_OrnamentOnly:		//只能對飾品使用
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_HOLE_ERR07"));

			case EM_CheckDigHoleResult_NotJewels:		//只能對飾品使用
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_HOLE_ERR05"));
				return false;
			case EM_CheckDigHoleResult_HoleErr:				//洞數過多
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_HOLE_ERR06"));
				return false;
			}
			break;

		case EM_ItemUseType_EQClearRune:		//清除裝備符文
			switch (RoleDataEx::CheckClsRune(srcItem , destItem))
			{
			case EM_CheckClsRuneResult_OK:					//OK
				break;
			case EM_CheckClsRuneResult_TargetErr:			//不可對此物品挖洞
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CLEAR_RUNE_ERR01"));
				return false;
			case EM_CheckClsRuneResult_SrcErr:				//使用物品有問題
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CLEAR_RUNE_ERR02"));
				return false;
			}
			break;

		case EM_ItemUseType_ItemLock:			// 裝備鎖
			switch(RoleDataEx::CheckItemLock(srcItem, destItem))
			{
			case EM_CheckItemLockResult_OK:
				break;
			case EM_CheckItemLockResult_TargetErr:
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_LOCKITEM_ERROR_TARGET"));
				return false;
			case EM_CheckItemLockResult_SrcErr:
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_LOCKITEM_ERROR_SOURCE"));
				return false;
			}
			break;

		case EM_ItemUseType_ItemUnLock:			// 解鎖
			switch (RoleDataEx::CheckItemUnLock(srcItem, destItem))
			{
			case EM_CheckItemUnLockResult_OK:
				srcItem->Pos = EM_ItemState_CliDisabled;

				// 如果是第一次
				if ( m_unlockSlot == -1 ) {
					m_unlockSlot = srcIndex;
					m_unlockType = targetType;
					m_unlockPos = index;

					// 彈出密碼框
					lua_State* L = GetLuaState();
					if ( L ) {
						lua_pushnumber(L, m_unlockSlot);
						lua_setglobal(L, "arg1");
						SendWorldEvent("UNLOCKED_ITEM_CONFIRM");
					}

					// 清除ICON
					m_useIndex		= -1;
					destItem->Pos	= EM_ItemState_CliDisabled;	
					return true;
				}
				break;
			case EM_CheckItemUnLockResult_TargetErr:
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_USEITEM_ERROR_TARGET"));
				return false;
			case EM_CheckItemUnLockResult_SrcErr:
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_USEITEM_ERROR_SOURCE"));
				return false;
			}
			break;

		case EM_ItemUseType_EQProtect:
			switch ( RoleDataEx::CheckEQProtect(srcItem, destItem) )
			{
			case EM_CheckEQProtectResult_OK:					//OK
				break;
			case EM_CheckEQProtectResult_TargetErr:				//
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_USEITEM_ERROR_TARGET"));
				return false;
			case EM_CheckEQProtectResult_SrcErr:				//
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_USEITEM_ERROR_SOURCE"));
				return false;				
			}
			break;

		case EM_ItemUseType_EQLimitMax:			//裝備耐久上限到Max
			switch ( RoleDataEx::CheckEQLimitMax(srcItem, destItem) )
			{
			case EM_CheckEQLimitMaxResult_OK:			//OK
				break;
			case EM_CheckEQLimitMaxResult_TargetErr:	//
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_USEITEM_ERROR_TARGET"));
				return false;
			case EM_CheckEQLimitMaxResult_SrcErr:		//
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_USEITEM_ERROR_SOURCE"));
				return false;
			}
			break;

		case EM_ItemUseType_IgnoreLimitLv:		//忽略等級限制
			switch ( RoleDataEx::CheckIgnoreLimitLv(srcItem, destItem) )
			{
			case EM_CheckIgnoreLimitLvMaxResult_OK:				//OK
				break;
			case EM_CheckIgnoreLimitLvMaxResult_TargetErr:		//
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_USEITEM_ERROR_TARGET"));
				return false;
			case EM_CheckIgnoreLimitLvMaxResult_SrcErr:			//
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_USEITEM_ERROR_SOURCE"));
				return false;
			}
			break;
		case EM_ItemUseType_PK_EQProtect:		//PK裝備保護
			switch ( RoleDataEx::CheckPkEqProtect(srcItem, destItem) )
			{
			case EM_CheckPkEqProtectResult_OK:				//OK
				break;
			case EM_CheckPkEqProtectResult_TargetErr:		//
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_USEITEM_ERROR_TARGET"));
				return false;
			case EM_CheckPkEqProtectResult_SrcErr:			//
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_USEITEM_ERROR_SOURCE"));
				return false;
			}
			break;
		case EM_ItemUseType_PK_EQProtect_Clear:	//PK裝備保護清除
			switch ( RoleDataEx::CheckPkEqProtectClear(srcItem, destItem) )
			{
			case EM_CheckPkEqProtectClearResult_OK:				//OK
				break;
			case EM_CheckPkEqProtectClearResult_TargetErr:		//
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_USEITEM_ERROR_TARGET"));
				return false;
			case EM_CheckPkEqProtectClearResult_SrcErr:			//
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_USEITEM_ERROR_SOURCE"));
				return false;
			}
			break;
		case EM_ItemUseType_ClsEqAbility:
			switch ( RoleDataEx::CheckClsEqAbility(srcItem, destItem) )
			{
			case EM_CheckClsEqAbilityResult_OK:				//OK
				break;
			case EM_CheckClsEqAbilityResult_TargetErr:		//
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_USEITEM_ERROR_TARGET"));
				return false;
			case EM_CheckClsEqAbilityResult_SrcErr:			//
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_USEITEM_ERROR_SOURCE"));
				return false;
			}
			break;
		}
		break;
	}

	if ( itemDB->Item.CastingTime > 0 )
	{
		if ( g_pGameMain->ItemCastingStart(ITEM_CASTING_USEITEM, srcIndex, targetType, index) == false )
		{
			return false;
		}
	}
	else
	{
		switch(targetType)
		{
		case 0:
			NetCli_Item::_UseItemToItem_Body(*srcItem, srcIndex, *destItem, index, (char*)(m_password.c_str()));
			break;
		case 1:
			NetCli_Item::_UseItemToItem_EQ(*srcItem, srcIndex, *destItem, index, (char*)(m_password.c_str()));
			break;
		case 2:
			NetCli_Item::_UseItemToItem_Bank(*srcItem, srcIndex, *destItem, index, (char*)(m_password.c_str()));
			break;
		}
	}	

	destItem->Pos	= EM_ItemState_CliDisabled;	
	m_useIndex		= -1;		// 清除物品使用編號,但物品EM_ItemState_CliDisabled狀態由SERVER回應時在清除		
	m_unlockSlot	= -1;
	m_unlockType	= -1;
	m_unlockPos		= -1;
	m_password.clear();
	return true;
}

// ----------------------------------------------------------------------------
void CBagFrame::UseItemCancel()
{	
	ItemFieldStruct* srcItem = CNetGlobal::RoleData()->GetBodyItem(m_useIndex);
	if (srcItem && !srcItem->IsEmpty())
		srcItem->Pos = EM_ItemState_NONE;
	m_useIndex = -1;	
}

// ----------------------------------------------------------------------------
void CBagFrame::CancelUseItemToLocation()
{
	ItemFieldStruct* srcItem = CNetGlobal::RoleData()->GetBodyItem(m_useToLocation);
	if (srcItem && !srcItem->IsEmpty())
	{
		srcItem->Pos = EM_ItemState_NONE;

		// 刪除選取物件
		g_pGameMain->ClearSelectAreaEntity();
	}
	m_useToLocation = -1;
}

// ----------------------------------------------------------------------------
int	CBagFrame::GetIndex(int objectID)
{
	RoleDataEx* role = CNetGlobal::RoleData();
	for ( int i = 0; i < role->PlayerBaseData->Body.Count; i++ )
	{
		ItemFieldStruct* item = role->GetBodyItem(i);
		if ( item && item->OrgObjID == objectID )
		{
			// 檢查物品是否已鎖定
			if ( i < 120 || CNetGlobal::RoleData()->PlayerBaseData->TimeFlag[EM_TimeFlagEffect_BagPage3 + (i - 120) / 30] != -1 )
				return i;
		}
	}
	return -1;
}

// ----------------------------------------------------------------------------
int	CBagFrame::GetIndex(const char* name)
{
	return -1;
}

// ----------------------------------------------------------------------------
void CBagFrame::Locked(int index)
{
	if( m_pickupItemIndex == index )
		m_pickupItemIndex = -1;

	ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(index);
	if ( item && item->Pos == EM_ItemState_NONE )
		item->Pos = EM_ItemState_CliDisabled;
}

// ----------------------------------------------------------------------------
void CBagFrame::Unlocked(int index)
{
	ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(index);
	if ( item && item->Pos == EM_ItemState_CliDisabled )
		item->Pos = EM_ItemState_NONE;
}

// ----------------------------------------------------------------------------
int CBagFrame::FindIndexByName(const char* name)
{
	if ( name && name[0] ) {
		BodyItemStruct &bodyItems = CNetGlobal::RoleData()->PlayerBaseData->Body;
		for ( int i = 0; i < bodyItems.Count; i++ )
		{
			if ( bodyItems.Item[i].IsEmpty() ) {
				continue;
			} else {
				char itemName[MAX_PATH];
				RoleDataEx::GetItemFieldName( &bodyItems.Item[i], itemName, MAX_PATH );
				if ( strcmp(itemName, name) == 0 ) {
					return i;
				}
			}
		}
	}

	return -1;
}

// ----------------------------------------------------------------------------
int CBagFrame::SearchItem(int createTime, int serial)
{	
	for ( int i = 0; i < CNetGlobal::RoleData()->PlayerBaseData->Body.Count; i++ )
	{
		ItemFieldStruct* item = CNetGlobal::RoleData()->PlayerBaseData->Body.Item + i;
		if ( item->IsEmpty() )
			continue;

		if ( item->CreateTime == createTime && item->Serial == serial )
			return i;
	}
	return -1;
}

// ----------------------------------------------------------------------------
void CBagFrame::UpdateItem(int index)
{
	if ( index == -1 )
	{
		for ( int i = 0; i < CNetGlobal::RoleData()->PlayerBaseData->Body.Count; i++ )
			UpdateItem(i);
	}

	lua_State* L = GetLuaState();
	if ( L )
	{
		ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(index);
		if ( item )
			m_itemPos[index] = item->Pos;

		lua_pushnumber(L, index + 1);
		lua_setglobal(L, "arg1");
		SendWorldEvent("BAG_ITEM_UPDATE");
	}
}
void  CBagFrame::MagicBoxAttrDrawOffResult(bool Result)
{
	if (Result)
	{

		g_pInterface->SendWorldEvent( UiEvent_MagicboxChanged );
	}
	else
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_MAGICBOX_ERROR"));


}
// ----------------------------------------------------------------------------
void CBagFrame::UpdateCooldown(int ColdownClass, int ColdownType)
{
	/*
	// 比對物品是否有項目相同
	for ( int i = 0; i < CNetGlobal::RoleData()->PlayerBaseData->Body.Count; i++ )
	{
		ItemFieldStruct* item = CNetGlobal::RoleData()->PlayerBaseData->Body.Item[i];
		if ( item )
		{
			GameObjDbStruct* itemDB = CNetGlobal::GetObj( item->OrgObjID );
			if ( itemDB )
			{
				switch (itemDB->Classification)
				{
				case EM_ObjectClass_Item:
					switch (itemDB->Item.UseType)
					{
					case EM_ItemUseType_Magic:						
						if ( itemDB->Item.IncMagic_Onuse != 0 )		// 檢查物品是否有法術效果
						{
						}
						break;
					}
				}
			}
		}
	}
	*/
	SendWorldEvent("BAG_UPDATE_COOLDOWN");
}
// ----------------------------------------------------------------------------
// 更換區域解除鎖定的背包欄位
void CBagFrame::ChangeZone()
{
	m_waitQueueUpdate	= false;
	m_lockedQueueTime	= 0.0f;
	m_lockedPos			= -1;

	for (int i = 0; i < _MAX_BODY_COUNT_; i++ )
	{
		bool resetState = true;
		if ( i == m_pickupItemIndex || i == m_useIndex || i == m_useToLocation || i == m_ItemMargePos )
		{
			 resetState = false;
		}
		else
		{
			for ( vector<int>::iterator iter = m_garbage.begin(); iter != m_garbage.end(); iter++ )
			{
				if ( i == *iter )
				{
					resetState = false;
					break;
				}
			}			
		}

		if ( resetState )
		{
			ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(i);
			if ( item )
				item->Pos = EM_ItemState_NONE;
		}
	}
}
// ----------------------------------------------------------------------------
void CBagFrame::RefreshCoin()
{
	int coin = 0;
	int itemID = g_ObjectData->GetSysKeyValue("SYS_PRICETYPE_ICONID_03");
	for ( int i = 0; i < _MAX_BODY_COUNT_; i++ )
	{
		ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(i);
		if ( item == NULL || item->IsEmpty() )
			continue;

		if ( item->OrgObjID == itemID )
		{
			coin += item->GetCount();
		}
	}

	if ( coin != m_billdin )
	{
		m_billdin = coin;

		// 通知角色金錢數值更改
		g_pBagFrame->SendWorldEvent(UiEvent_Role_Money_Change);
	}
}

// ----------------------------------------------------------------------------
bool CBagFrame::Gamble_ItemChanged( int index )
{
	ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem( index );
	if( NULL == item )
	{
		return false;
	}

	GameObjDbStructEx* pItemDB = CNetGlobal::GetObj( item->OrgObjID );
	if( NULL == pItemDB )
	{
		return false;
	}

	//裝備 有4屬性就能賭

	if( pItemDB->Classification != EM_ObjectClass_Item )
	{
		g_pGameMain->SendWarningMsg( g_ObjectData->GetString("SYS_GAMBLE_FAIL2") );
		return false;
	}

	if( pItemDB->Item.Class != EM_Item_Weapon && pItemDB->Item.Class != EM_Item_Armor )
	{
		g_pGameMain->SendWarningMsg( g_ObjectData->GetString("SYS_GAMBLE_FAIL2") );
		return false;
	}

	int AttributeLv = 0;

	int high1 = 0;
	int high2 = 0;

	int AttributeCount = 0;
	for ( int i = 0; i < 6; i++ )
	{
		GameObjDbStruct* inherent = CNetGlobal::GetObj( item->Inherent(i) );
		if ( inherent && inherent->Classification == EM_ObjectClass_Attribute )
		{
			// 能力
			++AttributeCount;

			AttributeLv += inherent->Attirbute.StandardAbility_Lv; 
	
			if( high1 < inherent->Attirbute.StandardAbility_Lv )
			{						
				high1 = inherent->Attirbute.StandardAbility_Lv;
			}
			if( high1 > high2 )
			{
				swap( high1, high2 );
			}
		}
	}

	AttributeLv -= high1;
	AttributeLv -= high2;

	if( AttributeCount < 4 )
	{
		//g_pGameMain->SendSystemMsg( g_ObjectData->GetString("SYS_HOUSE_NOT_SEED") );
		g_pGameMain->SendWarningMsg( g_ObjectData->GetString("SYS_GAMBLE_FAIL2") );
		return false;
	}

	//AttributeLv = ( AttributeLv + 5 ) / 6;
	AttributeLv = AttributeLv / 4;

	if( g_pGameMain->IsShowServerDebugMsg() ) 
	{
		g_pGameMain->SendSystemChatMsgEx( "Gamble_PickupItem BagItemIndex=%d AttributeLv=%d" ,index , AttributeLv );
	}

	m_Gamble_AttributeLv = AttributeLv;

	ObjectDataClass::GetGamebleItemStateCost(
		pItemDB->Rare, AttributeLv, pItemDB->Limit.Level, 
		m_Gamble_FirstTimeCost, m_Gamble_ContinueCost );

	return true;
}

// ----------------------------------------------------------------------------
void CBagFrame::Gamble_SetBagItemIndex( int index )
{
	Gamble_ClearItem();

	if( false == Gamble_ItemChanged( index ) )
	{
		return ;
	}
	
	m_GambleBagItemIndex = index;

	m_eGambleStep = GS_Ready;

	SendWorldEvent( UiEvent_GambleItemChanged );
}

// ----------------------------------------------------------------------------
void _Gamble_AddAttribute( vector<int>& rGambleAttributes, int DropListID )
{
	GameObjDbStructEx* pDropList = g_ObjectData->GetObj( DropListID );
	if( NULL == pDropList )
	{
		return ;
	}

	if( pDropList->Classification != EM_ObjectClass_Treasure )
	{
		return ;
	}

	GameObjDbStructEx* pItemDB;
	int i;
	for( i = 0 ; i < pDropList->Treasure.ItemCount ; ++i )
	{
		pItemDB = g_ObjectData->GetObj( pDropList->Treasure.Item[i].OrgID );
		if( pItemDB )
		{
			if( pItemDB->Classification == EM_ObjectClass_Attribute )
			{
				rGambleAttributes.push_back( pDropList->Treasure.Item[i].OrgID );
			}
			else if( pItemDB->Classification == EM_ObjectClass_Treasure )
			{
				_Gamble_AddAttribute( rGambleAttributes, pDropList->Treasure.Item[i].OrgID );
			}
		}
	}

}

int g_GamblePrizesNewIndex[24] =
{
	0,12,6,18,2,14,8,20,4,16,10,22,1,13,7,19,3,15,9,21,5,17,11,23
};

// ----------------------------------------------------------------------------
void CBagFrame::Gamble_SetGambleTableIndex( int index )
{
	m_GamblePrizes.clear();

	if( index < 0 || index >= g_ObjectData->_GambleItemStateTable.size() )
	{
		return;
	}

	vector<int> TempPrizes;
	_Gamble_AddAttribute( TempPrizes, g_ObjectData->_GambleItemStateTable[index].GambleObjID );

	m_GamblePrizes.resize(24);
	
	//把表打散
	int newIndex = 0;
	int i;
	for( i = 0 ; i < 24 ; ++i )
	{
		newIndex = g_GamblePrizesNewIndex[i];
		m_GamblePrizes[ newIndex ] = TempPrizes[i];
	}

	SendWorldEvent( UiEvent_GambleItemChanged );
}

// ----------------------------------------------------------------------------
void CBagFrame::Gamble_PickupItem( )
{
	if( m_eGambleStep == GS_NoItem || 
		m_eGambleStep == GS_Ready  || 
		m_eGambleStep == GS_Result )
	{
		
	}
	else
	{
		return ;
	}
	
	ClipboardItemType itemType = g_pItemClipboard->GetType();
 
	if( itemType == eClipboardItem_Type_None )   // 拿取物品
	{
		g_pItemClipboard->Clear();
		//m_BagItemIndex_Gamble.Cut();

		int index = m_GambleBagItemIndex;

		Gamble_ClearItem();

		if( index > 0 )
		{
			g_pBagFrame->Cut( index );
		}

		return;
	}

	if( itemType != eClipboardItem_Type_Bag ) // 不是背包 -> 
	{
		g_pGameMain->SendWarningMsg( g_ObjectData->GetString("SYS_GAMBLE_FAIL1") );
		g_pItemClipboard->Clear();
		return ;
	}

	ClipboardItemInfo itemInfo = g_pItemClipboard->GetItemInfo();
	g_pItemClipboard->Clear();

    
	int index = itemInfo.bag.pos;

	Gamble_SetBagItemIndex( index );
}


// ----------------------------------------------------------------------------
void CBagFrame::Gamble_ClearItem( )
{
	if( m_GambleBagItemIndex > 0 )
	{
		g_pBagFrame->Unlocked( m_GambleBagItemIndex );

		ItemFieldStruct* pItem = CNetGlobal::RoleData()->GetBodyItem( m_GambleBagItemIndex );
		if ( pItem )
		{
			if( pItem->Pos == EM_USE_ITEM_GAMBLE )
			{
				m_isGiveUp = true;
				NetCli_Item::SL_GetGamebleItemState( true );
			}
		}
	}

  	m_GambleLockFlag = 0;
	m_GambleBagItemIndex = -1;
	m_Gamble_FirstTimeCost = 0;
	m_Gamble_ContinueCost  = 0;
	m_GamblePrizes.clear();
	m_eGambleStep = GS_NoItem;
    
	SendWorldEvent( UiEvent_GambleItemChanged );
}

// ----------------------------------------------------------------------------
void CBagFrame::Gamble_GiveUp()
{
	if( m_eGambleStep != GS_Result )
	{
		return;
	}

	m_isGiveUp = true;

	NetCli_Item::SL_GetGamebleItemState( true );

	m_eGambleStep = GS_Ready;

	m_GamblePrizes.clear();

	SendWorldEvent( UiEvent_GambleItemChanged );
}

// ----------------------------------------------------------------------------
void CBagFrame::Gamble_Roll( )
{
	if( m_eGambleStep != GS_Ready && 
		m_eGambleStep != GS_Result )
	{
		return ;
	}

	bool bContiune = ( m_eGambleStep == GS_Result ) ? true : false;

	ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem( m_GambleBagItemIndex );
	if( NULL == item )
	{
		return;
	}

	//判斷鎖的數量
	{
		int lockcount = CNetGlobal::RoleData()->GetBagItemCount( g_ObjectData->SysValue().GamebleItemState_LockItemID );
		int i;
		int locked = 0;
		for( i = 0 ; i < 6; ++i )
		{
			if( ( m_GambleLockFlag >> i & 1 ) == 1 )
			{
				++locked;
			}
		}

		if( locked > lockcount )
		{
			g_pGameMain->SendWarningMsg( g_ObjectData->GetString( "SYS_GAMBLE_FAIL4" ) );
			SendWorldEvent( UiEvent_GambleItemChanged );
			return;
		}
	}

	//判斷錢夠不夠
	{
		int playerMoney = CNetGlobal::RoleData()->BodyMoney();

		int cost = bContiune ? m_Gamble_ContinueCost : m_Gamble_FirstTimeCost;

		if( cost > playerMoney )
		{
			g_pGameMain->SendWarningMsg( g_ObjectData->GetString( "SYS_GAMBLE_FAIL5" ) );
			SendWorldEvent( UiEvent_GambleItemChanged );
			return;
		}
	}

 	m_eGambleStep = GS_Roll;

	ItemFieldStruct BodyItem = *item;

	int tableID = -1;

	NetCli_Item::SL_GamebleItemState( 
		tableID, 
		g_pBagFrame->m_GambleBagItemIndex, 
		BodyItem , 
		g_pBagFrame->m_GambleLockFlag );

}

// ----------------------------------------------------------------------------
void CBagFrame::Gamble_Stop( )
{
	if( m_eGambleStep != GS_Rolling )
	{
		return ;
	}
	m_eGambleStep = GS_Stop;

	if( g_pGameMain->IsShowServerDebugMsg() ) 
	{
		string strResult;
		switch( m_Gamble_Result )
		{
		case EM_GamebleItemStateResult_OK                  : strResult = "OK"                  ; break;
		case EM_GamebleItemStateResult_Failed              : strResult = "Failed"              ; break;
		case EM_GamebleItemStateResult_DataError           : strResult = "DataError"           ; break;
		case EM_GamebleItemStateResult_LockCountError      : strResult = "LockCountError"      ; break;
		case EM_GamebleItemStateResult_AttributeCountError : strResult = "AttributeCountError" ; break;
		case EM_GamebleItemStateResult_TableIDError        : strResult = "TableIDError"        ; break;
		case EM_GamebleItemStateResult_MoneyError          : strResult = "MoneyError"          ; break;
		case EM_GamebleItemStateResult_TableDataError      : strResult = "TableDataError"      ; break;
		case EM_GamebleItemStateResult_LockItemError       : strResult = "LockItemError"       ; break;
		}

		g_pGameMain->SendSystemChatMsgEx( "R_GamebleItemStateResult TableID=%d StatePos=%d StateID=%d Result=%s" ,
			m_Gamble_TableID , m_Gamble_StatePos, m_Gamble_StateID, strResult.c_str() );
	}

	if( m_Gamble_Result == EM_GamebleItemStateResult_OK )
	{
		m_eGambleStep = GS_Stoping;

		m_GamblePrizesIndex = 0;
		int i;
		for( i = 0 ; i < m_GamblePrizes.size() ; i++ )
		{
			if( m_GamblePrizes[i] == m_Gamble_StateID )
			{
				m_GamblePrizesIndex = i;
				break;
			}
		}

		m_GamblePrizesIndex = m_GamblePrizesIndex + 1;
		m_GambleAbilityIndex = m_Gamble_StatePos + 1;

		SendWorldEvent( UiEvent_GambleResultOK );
	}
	else
	{
		m_eGambleStep = GS_Ready;
		m_GamblePrizes.clear();

		SendWorldEvent( UiEvent_GambleResultFailed );
		SendWorldEvent( UiEvent_GambleItemChanged );
	}
}

// ----------------------------------------------------------------------------
void CBagFrame::R_GamebleItemStateResult( int TableID , int StatePos , int StateID , GamebleItemStateResultENUM Result )
{
	//EM_GamebleItemStateResult_OK					,
	//EM_GamebleItemStateResult_Failed				,	
	//EM_GamebleItemStateResult_DataError				,
	//EM_GamebleItemStateResult_LockCountError		,
	//EM_GamebleItemStateResult_AttributeCountError	,
	//EM_GamebleItemStateResult_TableIDError			,
	//EM_GamebleItemStateResult_MoneyError			,
	//EM_GamebleItemStateResult_TableDataError		,	//掉寶表有問題
	//EM_GamebleItemStateResult_LockItemError			,	

	m_Gamble_TableID  = TableID ;
	m_Gamble_StatePos = StatePos;
	m_Gamble_StateID  = StateID ;
	m_Gamble_Result   = Result  ;
	
	m_eGambleStep = GS_Rolling;

	Gamble_SetGambleTableIndex( m_Gamble_TableID );

	//if( m_eGambleStep == GS_Stop )
	//{
	//	Gamble_Stop();
	//}
}

// ----------------------------------------------------------------------------
void CBagFrame::R_GetGamebleItemStateResult( bool Result )
{

	if( m_GambleBagItemIndex == -1 )
	{
		m_eGambleStep = GS_NoItem;
	}
	else if( m_isGiveUp )
	{

	}
	else
	{
		m_eGambleStep = GS_Ready;

		m_GamblePrizes.clear();

		Gamble_ItemChanged( m_GambleBagItemIndex );
 
		if( Result )
		{
			SendWorldEvent( UiEvent_GambleItemExchanged );
		}
		else
		{
			SendWorldEvent( UiEvent_GambleItemChanged );
		}	
	}
}

// ----------------------------------------------------------------------------
void CBagFrame::R_OpenGamebleItemState( int TargetID )
{
	m_GambleManage = TargetID;
	SendWorldEvent( UiEvent_GambleOpen );
}

// ----------------------------------------------------------------------------
void CBagFrame::R_CloseGamebleItemState( )
{
	if ( m_GambleManage >= 0 )
	{
		m_GambleManage = -1;
		SendWorldEvent( UiEvent_GambleClose );
	}
}

// ----------------------------------------------------------------------------
int	LuaFunc_GetBagItemInfo(lua_State* L)
{
	int pos = luaL_checkint(L, 1) - 1;
	int index = CNetGlobal::RoleData()->GetBodyRealPos(pos);
	int count = 0;
	const char* texture = "";
	const char* name = "";
	int locked = 0;
	int quality = 0;

	if ( index != -1 )
	{
		ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(index);
		if ( item )
		{
			GameObjDbStructEx* itemDB = CNetGlobal::GetObj(item->OrgObjID);
			if ( itemDB )
			{
				texture		= ItemUTIL::GetItemIcon(item);
				name		= itemDB->GetName();
				count		= item->GetCount();
				locked		= item->Pos - EM_ItemState_NONE;
				quality		= g_ObjectData->GetItemQuality(itemDB, item);

				if( index == g_pBagFrame->m_splitBagItemIndex )
				{
					count = count - g_pBagFrame->m_splitBagItemCount;
				}
	   		}
		}
		index = index + 1;
	}
	lua_pushnumber(L, index);			// push sort index
	lua_pushstring(L, texture);			// push item texture file
	lua_pushstring(L, name);			// push Item name
	lua_pushnumber(L, count);			// push item count
	lua_pushboolean(L, locked);			// push item is locked
	lua_pushnumber(L, quality);			// push item quality
	return 6;
}
// ----------------------------------------------------------------------------
// 回傳
int LuaFunc_GetBagPageLetTime(lua_State* L)
{
	//int time;
	switch (luaL_checkint(L, 1))
	{	
	case 1:
	case 2:
		lua_pushboolean(L, false);
		lua_pushnil(L);
		break;
	case 3:
		lua_pushboolean(L, true);
		lua_pushnumber(L, CNetGlobal::RoleData()->PlayerBaseData->TimeFlag[EM_TimeFlagEffect_BagPage3]);		
		break;
	case 4:
		lua_pushboolean(L, true);
		lua_pushnumber(L, CNetGlobal::RoleData()->PlayerBaseData->TimeFlag[EM_TimeFlagEffect_BagPage4]);		
		break;
	case 5:
		lua_pushboolean(L, true);
		lua_pushnumber(L, CNetGlobal::RoleData()->PlayerBaseData->TimeFlag[EM_TimeFlagEffect_BagPage5]);
		break;
	case 6:
		lua_pushboolean(L, true);
		lua_pushnumber(L, CNetGlobal::RoleData()->PlayerBaseData->TimeFlag[EM_TimeFlagEffect_BagPage6]);
		break;
	default:
		lua_pushboolean(L, false);
		lua_pushnil(L);
		break;
	}
	return 2;
}
// ----------------------------------------------------------------------------
int LuaFunc_GetBagItemCooldown(lua_State* L)
{
	float cooldown[2] = { 0.0f, 0.0f };

	if ( lua_isnumber(L, 1) )
	{
		ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem((int)lua_tonumber(L, 1) - 1);
		if ( item )
			CNetGlobal::RoleData()->GetObjectCooldown(item->OrgObjID, cooldown);
	}
	lua_pushnumber(L, cooldown[0]);
	lua_pushnumber(L, cooldown[1]);
	return 2;
}

// ----------------------------------------------------------------------------
int	LuaFunc_GetBagCount(lua_State* L)
{
	int maxCount = 0;
	for ( int i = 0; i < _MAX_BODY_COUNT_-_MAX_BODY_BEGIN_POS_; i++ )
	{
		ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(_MAX_BODY_BEGIN_POS_ + i);
		int index = i / 30 - 2;
		if ( item && item->IsEmpty() == false && (index < 0 || CNetGlobal::RoleData()->PlayerBaseData->TimeFlag[index + EM_TimeFlagEffect_BagPage3] != -1) )
			maxCount++;
	}
	
	int count = 30 * 2;
	if ( CNetGlobal::RoleData()->PlayerBaseData->TimeFlag[EM_TimeFlagEffect_BagPage3] != -1 )
		count += 30;
	if ( CNetGlobal::RoleData()->PlayerBaseData->TimeFlag[EM_TimeFlagEffect_BagPage4] != -1 )
		count += 30;
	if ( CNetGlobal::RoleData()->PlayerBaseData->TimeFlag[EM_TimeFlagEffect_BagPage5] != -1 )
		count += 30;
	if ( CNetGlobal::RoleData()->PlayerBaseData->TimeFlag[EM_TimeFlagEffect_BagPage6] != -1 )
		count += 30;

	lua_pushnumber(L, maxCount);
	lua_pushnumber(L, count);
	lua_pushnumber(L, _MAX_BODY_COUNT_-_MAX_BODY_BEGIN_POS_);

	return 3;
}

// ----------------------------------------------------------------------------
int	LuaFunc_PickupBagItem(lua_State* L)
{
	g_pBagFrame->Paste( luaL_checkint(L, 1) - 1 );
	return 0;
}

// ----------------------------------------------------------------------------
int	LuaFunc_UseBagItem(lua_State* L)
{	
	g_pBagFrame->Use( luaL_checkint(L, 1) - 1 , true );
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_UseItemByName(lua_State* L)
{	
	g_pBagFrame->UseItemByName(luaL_checkstring(L, 1));
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_CloseBag(lua_State* L)
{
	g_pBagFrame->Close();
	return 0;
}

// ----------------------------------------------------------------------------
// arg1 背包索引值 1開始
// arg2 拆堆個數
int LuaFunc_SplitBagItem(lua_State* L)
{
	if ( lua_isnumber(L, 1) && lua_isnumber(L, 2) )
	{
		int index = (int)lua_tonumber(L, 1) - 1;
		int count = (int)lua_tonumber(L, 2);

		ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(index);
		if ( item &&
			 !item->IsEmpty() &&
			 item->Pos == EM_ItemState_NONE &&
			 count > 0 &&
			 item->GetCount() > count &&
			 g_pBagFrame->m_splitBagItemIndex == -1 )
		{
			//// 搜尋可以安插位置
			//int destPos = CNetGlobal::RoleData()->SearchBodyFirstFit(NULL, false);		// 找尋一個可以插入背包的位置
			//if ( destPos != -1 )
			//{
			//	ItemFieldStruct* destItem = CNetGlobal::RoleData()->GetBodyItem(destPos);

			//	item->Pos = EM_ItemState_CliDisabled;							// Client鎖定
			//	destItem->Pos = EM_ItemState_CliDisabled;						// Client鎖定
			//	NetCli_Item::_ApartBodyItemRequest(*item, index, count, destPos);
			//	return 0;
			//}			

			//// 沒有空間
			//g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_SPLITBAG_ENOUGH_CAPACITY"));

			g_pBagFrame->m_splitBagItemIndex = index;
			g_pBagFrame->m_splitBagItemCount = count;

			item->Pos = EM_ItemState_CliDisabled;

			//m_pickupItemIndex = index;
			g_pItemClipboard->SetFile( ItemUTIL::GetItemIcon(item) );			
			g_pItemClipboard->PickupSplitBagItem( item->OrgObjID, index, count );


			lua_State* L = g_pInterface->GetLuaState();
			if ( L ) 
			{
				lua_pushnumber(L, index );
				lua_setglobal(L, "arg1");
				g_pInterface->SendWorldEvent("BAG_ITEM_UPDATE");
			}
		}
		else
		{
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_SPLITBAG_DATA_ERROR"));
		}
	}
	return 0;
}

// 更新背包,重新排序
int LuaFunc_RefreshBag(lua_State* L)
{
	g_pBagFrame->SortItems(luaL_checkint(L, 1) - 1);
	return 0;
}

// ----------------------------------------------------------------------------
// 計數某個物品身上有的個數
int LuaFunc_GetBagItemCount(lua_State* L)
{
	int count = 0;

	int ObjID = luaL_checkint(L, 1);
	for ( int i = 0; i < _MAX_BODY_COUNT_; i++ )
	{
		ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(i);
		if ( item && item->OrgObjID == ObjID )
		{
			count += item->GetCount();
		}
	}

	lua_pushnumber(L, count);
	return 1;
}

int LuaFunc_GetItemQueueInfo(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	if ( index >= 0 && index < CNetGlobal::RoleData()->PlayerBaseData->ItemTemp.Size() )
	{
		ItemFieldStruct& item = CNetGlobal::RoleData()->PlayerBaseData->ItemTemp[index];
		GameObjDbStructEx* itemDB = g_ObjectData->GetObj(item.OrgObjID);

		lua_pushstring(L, itemDB->GetName());
		lua_pushstring(L, ItemUTIL::GetItemIcon(&item));			          // push item texture file
		lua_pushnumber(L, item.Count);
		lua_pushnumber(L, g_ObjectData->GetItemQuality(itemDB, &item));
		return 4;
	}
	return 0;
}

int LuaFunc_ItemQueueAssignToBag(lua_State* L)
{
	const char* resultStr = "";
	switch ( g_pBagFrame->AssignQueueItemToBag() )
	{
	case  0:
		lua_pushstring(L, "empty");
		break;

	case -1:
		lua_pushstring(L, "wait");
		break;

	case -2:
		lua_pushstring(L, "dataER");
		break;

	case -3:
		lua_pushstring(L, "full");
		break;

	default:
		lua_pushnil(L);
		break;
	}
	
	//NetCli_ItemChild::_Update(0.1f);
	return 1;
}
// 解除物品鎖定確認
int LuaFunc_UnlockPendingItem(lua_State* L)
{
	g_pBagFrame->UnlockPendingItem(luaL_checkstring(L, 1));
	return 0;
}
// 取消解除物品鎖定
int LuaFunc_CancelUnlockItem(lua_State* L)
{
	g_pBagFrame->CancelUnlockedItem();
	return 0;
}

//物品融合

int LuaFunc_IMF_GetCursorItem(lua_State* L)
{
	int index=-1;
	ClipboardItemInfo itemInfo = g_pItemClipboard->GetItemInfo();
	ClipboardItemType itemType = g_pItemClipboard->GetType();


	if (itemInfo.bag.itemID>0)
	{
		if( itemType == eClipboardItem_Type_Bag )
		{
			index=itemInfo.bag.pos;
			g_pItemClipboard->Clear();
			g_pBagFrame->ClearPickupItemIndex();	
		}
		else
		{
			const char* tempStr;

			tempStr=g_ObjectData->GetString("SYS_ITEM_MARGE_ERRORMSG");

			g_pGameMain->SendWarningMsg(tempStr);		
		}

	}
	lua_pushnumber(L, index+1);			

	return 1;
}


//物品融合
int LuaFunc_IMF_GetMargeItem(lua_State* L)
{

	

	ItemFieldStruct* item = NULL;



	
	{
		int index=(int)lua_tonumber(L,1)-1;

		item = CNetGlobal::RoleData()->GetBodyItem(index);
		if (item)
		{
			GameObjDbStructEx* itemDB = CNetGlobal::GetObj(item->OrgObjID);		
			if (itemDB&& itemDB->Classification==EM_ObjectClass_Item ||itemDB->Classification==EM_ObjectClass_Image)
			{
				int imageSex=0;
				int Class=0;

				int type=0;
				int pos=0;
				int sex=0;

				int SpecialSuit=itemDB->Mode.SpecialSuit;
				char SP1[512];
				char SP2[512];
				char SP3[512];
				char tmpL[512];
				char tmpR[512];
				SP1[0]=0;
				SP2[0]=0;
				SP3[0]=0;
			
				const char *leftText=0;
				const char *rightText=0;
				const char *sexText=0;
			//	g_pItemClipboard->Clear();
			//	g_pBagFrame->ClearPickupItemIndex();		
				item->Pos=EM_USE_ITEM;
				if (itemDB->Classification==EM_ObjectClass_Image)
				{
					Class=EM_Item_Armor;
					type=0;//itemDB->Image.ArmorType;
					pos=itemDB->Image.ArmorPos;
					imageSex=itemDB->Image.LimitSex._Sex;

					if ( itemDB->Mode.SpecialSuit )
					{
						leftText= g_ObjectData->GetString("SYS_SPECIALSUIT");
						
					}
					else
					{

						sprintf(tmpL, "SYS_ARMORTYPE_%02d", itemDB->Item.ArmorType);
						leftText = g_ObjectData->GetString(tmpL);
					}
					if (leftText)
					sprintf(SP1, leftText);
					sprintf(tmpR, "SYS_ARMORPOS_%02d", itemDB->Item.ArmorPos);
					sprintf(SP2, g_ObjectData->GetString(tmpR));


					if (imageSex==1)
						sexText=g_ObjectData->GetString("SYS_SEX_BOY");
					else if (imageSex==2)
						sexText=g_ObjectData->GetString("SYS_SEX_GIRL");


					if (sexText)
					sprintf(SP3, sexText);

					
					
				}
				else if (itemDB&&itemDB->Item.ItemType == EM_ItemType_Normal)
				{
					if (itemDB->Item.Class==EM_Item_Weapon ||itemDB->Item.Class==EM_Item_Armor)			    
					{
						Class=itemDB->Item.Class;
						type=itemDB->Item.WeaponType;
						pos=itemDB->Item.WeaponPos;
						sex=itemDB->Limit.Sex._Sex;						
						if ( itemDB->Mode.SpecialSuit )
						{
							leftText= g_ObjectData->GetString("SYS_SPECIALSUIT");
							
						}
						else if (itemDB->Item.Class==EM_Item_Armor)
						{

							sprintf(tmpL, "SYS_ARMORTYPE_%02d", itemDB->Item.ArmorType);
							leftText = g_ObjectData->GetString(tmpL);
						}
						else
						{
							sprintf(tmpL, "SYS_WEAPON_TYPE%02d", itemDB->Item.WeaponType);
							leftText = g_ObjectData->GetString(tmpL);

						}

						if (leftText)
						sprintf(SP1, leftText);

						if (itemDB->Item.Class==EM_Item_Armor)
						{

							sprintf(tmpR, "SYS_ARMORPOS_%02d", itemDB->Item.ArmorPos);
							rightText = g_ObjectData->GetString(tmpR);

						}
						else
						{
						
								sprintf(tmpR, "SYS_WEAPON_POS%02d", itemDB->Item.WeaponPos);
								rightText = g_ObjectData->GetString(tmpR);


						}
						if (rightText)

						sprintf(SP2, rightText);
						if (sex==1)
							sexText=g_ObjectData->GetString("SYS_SEX_BOY");
						else if (sex==2)
							sexText=g_ObjectData->GetString("SYS_SEX_GIRL");




						if (sexText)
						
					sprintf(SP3, sexText);
					}
				}
				
				lua_pushstring(L, ItemUTIL::GetItemIcon(item));			          // push item texture file
				lua_pushstring(L, itemDB->GetName());						// push Item name		
				lua_pushnumber(L, Class);		
				lua_pushnumber(L, type);		
				lua_pushnumber(L, pos);	
				lua_pushnumber(L, sex);	
				lua_pushnumber(L, imageSex);	
				lua_pushnumber(L, SpecialSuit);	
				lua_pushstring(L, SP1);	
				lua_pushstring(L, SP2);	
				lua_pushstring(L, SP3);	

				
				return 11;
			}
			else
			{

				const char* tempStr;
				tempStr=g_ObjectData->GetString("MSG_ITEMMARGE_WRONGTHING");

				g_pGameMain->SendWarningMsg(tempStr);

					//g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, tempStr, "" );
			}
		}
		
	}
	


	return 0;

}
int LuaFunc_IMF_RemoveMargeItem(lua_State* L)
{
	if (lua_isnumber(L,1))
	{
		int index=(int)lua_tonumber(L,1)-1;
		ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(index);
		if (item)
		{
			GameObjDbStruct* itemDB = CNetGlobal::GetObj(item->OrgObjID);	
			g_pBagFrame->Unlocked(index);

			//	g_pBagFrame->Cut(index);

			item->Pos=EM_ItemState_NONE;

			lua_State* L = g_pInterface->GetLuaState();
			if ( L ) 
			{
				lua_pushnumber(L, index );
				lua_setglobal(L, "arg1");
				g_pInterface->SendWorldEvent("BAG_ITEM_UPDATE");
			}
		}
	}
	return 0;
}

int LuaFunc_IMF_MargeItemClose(lua_State* L)
{
	g_pBagFrame->MargeItemClose();

	
	NetCli_Item::_EQCombinClose( );
	return 0;
}

int LuaFunc_IMF_MargeItemApply(lua_State* L)
{
	int itemA = luaL_checkint(L, 1) - 1;
	int itemB = luaL_checkint(L, 2) - 1;
	if (g_pBagFrame)
	g_pBagFrame->MargeItemApply(itemB,itemA);
	
	
	
	
	return 0;
}




//-------------------------------------------------------------------------------------
int LuaFunc_GetBagItemLink(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;

	bool bSurface = false;
	if( !lua_isnil( L , 2 ) )
	{
		bSurface = (lua_toboolean(L, 2) != 0);
	}

  	ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(index);
	if (item)
	{
		GameObjDbStruct* itemDB = CNetGlobal::GetObj( item->OrgObjID );
		if (itemDB)
		{
			string link;
			if( bSurface )
			{
				if( !ItemGUID_To_ItemLink( item->ImageObjectID, link ) )
				{
					return 0;
				}
			}
			else
			{
				if( !ItemField_To_ItemLink( item, link, true ) )
				{
					return 0;
				}
			}

			lua_pushstring(L, link.c_str() );
			return 1;
		}
	}
 	return 0;
}

//-------------------------------------------------------------------------------------
int LuaFunc_CloseGarbageItems		(lua_State* L)
{
	g_pBagFrame->ClearGarbageItems();
	return 0;
}

//-------------------------------------------------------------------------------------
int LuaFunc_GetGarbageMaxItems		(lua_State* L)
{
	lua_pushnumber(L, g_pBagFrame->GetGarbageMaxItems());
	return 1;
}

//-------------------------------------------------------------------------------------
int LuaFunc_GetGarbageItemInfo		(lua_State* L)
{	
	int index = g_pBagFrame->GetGarbageItemIndex(luaL_checkint(L, 1) - 1);
	if ( index >= 0 )
	{
		ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(index);
		if ( item != NULL && item->IsEmpty() == false )
		{
			lua_pushnumber(L, index + 1);
			lua_pushstring(L, ItemUTIL::GetItemIcon(item));			          // push item texture file
			lua_pushnumber(L, item->GetCount());
			lua_pushnumber(L, g_ObjectData->GetItemQuality(NULL, item));

			char itemName[MAX_PATH];
			memset(itemName, 0, sizeof(itemName));
			g_ObjectData->GetItemFieldName(item, itemName, MAX_PATH);
			lua_pushstring(L, itemName);
			return 5;
		}
	}
	return 0;
}

//-------------------------------------------------------------------------------------
int LuaFunc_InsertGarbageItem(lua_State* L)
{
	g_pBagFrame->InsertGarbageItem(luaL_checkint(L, 1) - 1, (lua_toboolean(L, 2) != 0));
	return 0;
}

//-------------------------------------------------------------------------------------
int LuaFunc_RemoveGarbageItem(lua_State* L)
{
	g_pBagFrame->RemoveGarbageItem(luaL_checkint(L, 1) - 1);
	return 0;
}

//-------------------------------------------------------------------------------------
int LuaFunc_DeleteGarbageItems(lua_State* L)
{
	g_pBagFrame->DeleteGarbageItems();
	return 0;
}

//-------------------------------------------------------------------------------------
int LuaFunc_GetGoodsItemInfo		(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	int count = 0;
	const char* texture = "";
	const char* name = "";
	int locked = 0;
	
	ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(index);
	if ( item )
	{
		GameObjDbStructEx* itemDB = CNetGlobal::GetObj(item->OrgObjID);
		if ( itemDB )
		{
			texture		= ItemUTIL::GetItemIcon(item);
			name		= itemDB->GetName();
			count		= item->GetCount();
			locked		= item->Pos - EM_ItemState_NONE;
		}
	}

	lua_pushstring(L, texture);			// push item texture file
	lua_pushstring(L, name);			// push Item name
	lua_pushnumber(L, count);			// push item count
	lua_pushboolean(L, locked);			// push item is locked
	return 4;
}

//-------------------------------------------------------------------------------------
int LuaFunc_GetGoodsItemCooldown	(lua_State* L)
{
	float cooldown[2] = { 0.0f, 0.0f };

	if ( lua_isnumber(L, 1) )
	{
		ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem((int)lua_tonumber(L, 1) - 1);
		if ( item )
			CNetGlobal::RoleData()->GetObjectCooldown(item->OrgObjID, cooldown);
	}
	lua_pushnumber(L, cooldown[0]);
	lua_pushnumber(L, cooldown[1]);
	return 2;
}

//bool bMagicBoxOpen = CNetGlobal::RoleData()->CheckKeyItem( g_ObjectData->SysValue().MagicBoxOpenKeyItemID );
//-------------------------------------------------------------------------------------
int LuaFunc_IsMagicBoxEnable(lua_State* L)
{
	bool bMagicBoxOpen = CNetGlobal::RoleData()->CheckKeyItem( g_ObjectData->SysValue().MagicBoxOpenKeyItemID );

	lua_pushboolean( L, bMagicBoxOpen );

	return 1;
}


//-------------------------------------------------------------------------------------
int LuaFunc_GetMagicBoxPreviewResult(lua_State* L)
{
	ItemFieldStruct ResultItem;
	bool bResult = CNetGlobal::RoleData()->MagicBoxSmelt( &ResultItem );

	if( !bResult )
	{
		//g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_MAGICBOX_ERROR"));
		return 0;
	}

	int count = 0;
	const char* texture = "";
	const char* name = "";
	string link;

	ItemFieldStruct* item = &ResultItem;
	GameObjDbStructEx* itemDB = CNetGlobal::GetObj(item->OrgObjID);

	if( !itemDB )
	{
		return 0;
	}

	texture		= ItemUTIL::GetItemIcon(item);
	name		= itemDB->GetName();
	count		= item->GetCount();

	if( !ItemField_To_ItemLink( item, link ) )
	{
		return 0;
	}
  
	lua_pushstring(L, texture);			// push item texture file
	lua_pushstring(L, name);			// push Item name
	lua_pushnumber(L, count);			// push item count
	lua_pushstring(L, link.c_str() );

	return 4;
}

int LuaFunc_MagicBoxCanDrawStone(lua_State* L)
{

	bool ret=true;

	int Energy = CNetGlobal::RoleData()->PlayerBaseData->BoxEnergy;
	if (Energy<=0)
		ret=false;
	bool bMagicBoxOpen = CNetGlobal::RoleData()->CheckKeyItem( g_ObjectData->SysValue().MagicBoxOpenKeyItemID );
	if (bMagicBoxOpen==false)
		ret=false;


	ItemFieldStruct* pItem;

	bool Ok1=false;
	pItem = CNetGlobal::RoleData()->GetBodyItem( 1 + _MAX_BODY_EX_COUNT_ );
	if (pItem&&!pItem->IsEmpty())
	{

		GameObjDbStructEx* itemDB = CNetGlobal::GetObj(pItem->OrgObjID);
		if (itemDB)
		{
			if ( itemDB->IsWeapon() || itemDB->IsArmor() )
			{
				if (!itemDB->Mode.DisableMagicBox)
				{
					int  count=0;
					for ( int i = 0; i < 6; i++ )
					{
						GameObjDbStruct* inherent = CNetGlobal::GetObj( pItem->Inherent(i) );
						if (inherent)
							count++;		

					}
					if (count>0)

						Ok1=true;
				}
			
			}
			
		}
	}
	bool Ok2=false;
	pItem = CNetGlobal::RoleData()->GetBodyItem( 2 + _MAX_BODY_EX_COUNT_ );
	if (pItem&&!pItem->IsEmpty())
	{
		int DBID=g_ObjectData->GetSysKeyValue("AttributeDrawOffItemID");
		
		if (pItem->OrgObjID==DBID)
			Ok2=true;
	}
	bool Ok3=false;
	pItem = CNetGlobal::RoleData()->GetBodyItem( 0 + _MAX_BODY_EX_COUNT_ );
	if (pItem&&pItem->IsEmpty())
		Ok3=true;

	for( int i = 1 ; i < 3 ; ++i )
	{
		pItem = CNetGlobal::RoleData()->GetBodyItem( i + _MAX_BODY_EX_COUNT_ );
		if( pItem->Pos != EM_ItemState_NONE )
		{
			ret=false;
			break;
		}
	}
	if (!Ok3 ||!Ok2  ||!Ok1)
		ret=false;
	lua_pushboolean(L,ret);


	return 1;
}
int LuaFunc_MagicBoxDrawStone(lua_State* L)
{

	bool bMagicBoxOpen = CNetGlobal::RoleData()->CheckKeyItem( g_ObjectData->SysValue().MagicBoxOpenKeyItemID );
	if( bMagicBoxOpen )
	{
		

		NetCli_Item::SL_MagicBoxAttrDrawOff();
	}

	return 0;


	
}
//-------------------------------------------------------------------------------------
int LuaFunc_MagicBoxRequest(lua_State* L)
{
	ItemFieldStruct ResultItem;
	bool bResult = CNetGlobal::RoleData()->MagicBoxSmelt( &ResultItem );
	if( !bResult )
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_MAGICBOX_ERROR"));
		return 0;
	}

	if( CNetGlobal::RoleData()->PlayerBaseData->BoxEnergy == 0 )
	{
		g_pInterface->SendWorldEvent( UiEvent_MagicboxNotEnoughEnergy );
		return 0;
	}

	bool bMagicBoxOpen = CNetGlobal::RoleData()->CheckKeyItem( g_ObjectData->SysValue().MagicBoxOpenKeyItemID );
	if( bMagicBoxOpen )
	{
		//判斷物品有沒有被鎖定
		int i;
		ItemFieldStruct* pItem;
		for( i = 0 ; i < 10 ; ++i )
		{
			pItem = CNetGlobal::RoleData()->GetBodyItem( i + _MAX_BODY_EX_COUNT_ );
			if( pItem->Pos != EM_ItemState_NONE )
			{
				g_pGameMain->SendWarningMsg( g_ObjectData->GetString( "SYS_MAGICBOX_ITEM_LOCKED" ) );
				return 0;
			}
		}

		NetCli_Item::SL_MagicBoxRequest();
	}
 	
	return 0;
}

//-------------------------------------------------------------------------------------
int LuaFunc_GetMagicBoxEnergy(lua_State* L)
{
	int Energy = CNetGlobal::RoleData()->PlayerBaseData->BoxEnergy;

  	lua_pushnumber( L, Energy );			// push item count
	return 1;
}

//-------------------------------------------------------------------------------------
int LuaFunc_Gamble_Close(lua_State* L)
{
	if ( g_pBagFrame->m_GambleManage >= 0 )
	{
		g_pBagFrame->m_GambleManage = -1;
   		NetCli_Item::SL_CloseGamebleItemState();
	}
	return 0;
}

//-------------------------------------------------------------------------------------
int LuaFunc_Gamble_PickupItem(lua_State* L)
{
	g_pBagFrame->Gamble_PickupItem();
	return 0;
}

//-------------------------------------------------------------------------------------
int LuaFunc_Gamble_GetItemInfo(lua_State* L)
{
	int index = g_pBagFrame->m_GambleBagItemIndex;
	const char* texture = "";
	const char* name = "";

	ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(index);
	if ( item )
	{
		GameObjDbStructEx* itemDB = CNetGlobal::GetObj(item->OrgObjID);
		if ( itemDB )
		{
			texture = ItemUTIL::GetItemIcon(item);
			name    = itemDB->GetName();
		}
	}

	lua_pushstring(L, texture);			// push item texture file
	lua_pushstring(L, name);			// push Item name
 	lua_pushnumber(L, index + 1);


	lua_pushnumber( L, g_pBagFrame->m_Gamble_FirstTimeCost );
	lua_pushnumber( L, g_pBagFrame->m_Gamble_ContinueCost  );
	
	return 5;
}

//-------------------------------------------------------------------------------------
int LuaFunc_Gamble_GetLockCount(lua_State* L)
{
	int ItemID = g_ObjectData->SysValue().GamebleItemState_LockItemID;

	int count = CNetGlobal::RoleData()->GetBagItemCount( ItemID );

	lua_pushnumber( L, count );

	return 1;
}

//-------------------------------------------------------------------------------------
int LuaFunc_Gamble_GetAttributeInfo(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;

	const char* texture = "";
	const char* name = "";
	int DBID = 0;

	ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem( g_pBagFrame->m_GambleBagItemIndex );
	if ( !item )
	{
		return 0;
	}

	GameObjDbStruct* pItemDB = CNetGlobal::GetObj(item->OrgObjID);
	if ( !pItemDB )
	{
		return 0;
	}	

	if( pItemDB->Classification != EM_ObjectClass_Item )
	{
		return 0;
	}

	if( pItemDB->Item.Class != EM_Item_Weapon && pItemDB->Item.Class != EM_Item_Armor )
	{
		return 0;
	}

	DBID = item->Inherent( index );

	GameObjDbStructEx* inherent = CNetGlobal::GetObj( DBID );
	if( !inherent || inherent->Classification != EM_ObjectClass_Attribute )
	{
		return 0;
	}

	name = inherent->GetName();

	GameObjDbStruct* imageDB = CNetGlobal::GetObj(inherent->ImageID);
	if( imageDB )
	{
		texture = imageDB->Image.ACTField;
	}

	string strlevel = "";
	
	const char* ptemp = NULL;
	int i = (int)strlen( name ) - 1;
	while( i >= 0 )
	{
		//羅馬數字
		if( name[i] == 'I' || //1 
			name[i] == 'V' || //5
			name[i] == 'X' || //10
			name[i] == 'L' || //50
			name[i] == 'C' )  //100
		{
			ptemp = &name[i];
		}
		else
		{
			break;
		}
		--i;
	}

	if( ptemp )
		strlevel = ptemp;

	int lv = inherent->Attirbute.StandardAbility_Lv; 

	lua_pushstring(L, texture);			// push item texture file
	lua_pushstring(L, name);			// push Item name
	lua_pushstring(L, strlevel.c_str()); 
	lua_pushnumber(L, DBID ); 
 	lua_pushnumber(L, lv ); 
	return 5;
}

//-------------------------------------------------------------------------------------
int LuaFunc_Gamble_GetPrizeInfo(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	int DBID = 0;

	if( index < 0 || index >= g_pBagFrame->m_GamblePrizes.size() )
	{
		return 0;
	}

	DBID = g_pBagFrame->m_GamblePrizes[ index ];

	GameObjDbStructEx* inherent = CNetGlobal::GetObj( DBID );
	if ( !inherent || inherent->Classification != EM_ObjectClass_Attribute )
	{
		return 0;
	}

	GameObjDbStruct* imageDB = CNetGlobal::GetObj( inherent->ImageID );
	if ( imageDB == NULL )
	{
		return 0;
	}

	const char* texture = imageDB->Image.ACTField;
	const char* name = "";
	name = inherent->GetName();

	string strlevel = "";

	const char* ptemp = NULL;
	int i = (int)strlen( name ) - 1;
	while( i >= 0 )
	{
		//羅馬數字
		if( name[i] == 'I' || //1 
			name[i] == 'V' || //5
			name[i] == 'X' || //10
			name[i] == 'L' || //50
			name[i] == 'C' )  //100
		{
			ptemp = &name[i];
		}
		else
		{
			break;
		}
		--i;
	}

	if( ptemp )
		strlevel = ptemp;

	int lv = inherent->Attirbute.StandardAbility_Lv; 

	lua_pushstring(L, texture);			 // push item texture file
	lua_pushstring(L, name);		 	 // push Item name
	lua_pushstring(L, strlevel.c_str()); 
	lua_pushnumber(L, DBID ); 
	lua_pushnumber(L, lv ); 
	return 5;
}

//-------------------------------------------------------------------------------------
int LuaFunc_Gamble_Roll(lua_State* L)
{
	g_pBagFrame->Gamble_Roll( );
 
	return 0;
}

//-------------------------------------------------------------------------------------
int LuaFunc_Gamble_Stop(lua_State* L)
{
	g_pBagFrame->Gamble_Stop( );

	return 0;
}

//-------------------------------------------------------------------------------------
int LuaFunc_Gamble_Stoped(lua_State* L)
{
	if( g_pBagFrame->m_eGambleStep != CBagFrame::GS_Stoping )
	{
		return 0;
	}
	g_pBagFrame->m_eGambleStep = CBagFrame::GS_Result;

	return 0;
}

//-------------------------------------------------------------------------------------
int LuaFunc_Gamble_Exchange(lua_State* L)
{
	if( g_pBagFrame->m_eGambleStep != CBagFrame::GS_Result )
	{
		return 0;
	}
	g_pBagFrame->m_eGambleStep = CBagFrame::GS_Exchange;

	g_pBagFrame->m_isGiveUp = false;
	NetCli_Item::SL_GetGamebleItemState( false );
	return 0;
}

//-------------------------------------------------------------------------------------
int LuaFunc_Gamble_GetResult(lua_State* L)
{
	lua_pushnumber( L, g_pBagFrame->m_GambleAbilityIndex );
	lua_pushnumber( L, g_pBagFrame->m_GamblePrizesIndex );
	return 2;
}

//-------------------------------------------------------------------------------------
int LuaFunc_Gamble_GiveUp(lua_State* L)
{
	g_pBagFrame->Gamble_GiveUp();
 	return 0;
}

//-------------------------------------------------------------------------------------
int LuaFunc_Gamble_ClearItem(lua_State* L)
{
	g_pBagFrame->Gamble_ClearItem();
 	return 0;
}

//-------------------------------------------------------------------------------------
int LuaFunc_Gamble_GetStep(lua_State* L)
{
	std::string strStep = "";

	switch( g_pBagFrame->m_eGambleStep)
	{
	case CBagFrame::GS_NoItem  : strStep = "NoItem"  ; break;
  	case CBagFrame::GS_Ready   : strStep = "Ready"   ; break;
	case CBagFrame::GS_Roll    : strStep = "Roll"    ; break;
	case CBagFrame::GS_Rolling : strStep = "Rolling" ; break;
	case CBagFrame::GS_Stop    : strStep = "Stop"    ; break;
	case CBagFrame::GS_Stoping : strStep = "Stoping" ; break;
	case CBagFrame::GS_Result  : strStep = "Result"  ; break;
	case CBagFrame::GS_Exchange: strStep = "Exchange"; break;
	}

	lua_pushstring( L, strStep.c_str() );

	return 1;
}

//-------------------------------------------------------------------------------------
int LuaFunc_Gamble_SetLockFlag(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	bool lock = lua_toboolean( L, 2 ) == 1;

	if( g_pBagFrame->m_eGambleStep == CBagFrame::GS_Ready  || 
		g_pBagFrame->m_eGambleStep == CBagFrame::GS_Result )

	{

	}
	else
	{
		return 0;
	}

	if( index < 0 || index > 6 )
	{
		return 0;
	}

	if( lock )
	{
		int i;
		int lockCount = 0;

		for( i = 0 ; i < 6; ++i )
		{
			if(  ( g_pBagFrame->m_GambleLockFlag >> i & 1 ) == 1 )
			{
				++lockCount;
			}
		}

		if( lockCount >= 3 )
		{
			g_pGameMain->SendWarningMsg( g_ObjectData->GetString("SYS_GAMBLE_FAIL3") );
			return 0;
		}
	}

	if( lock )
	{
		g_pBagFrame->m_GambleLockFlag = g_pBagFrame->m_GambleLockFlag | ( 1 << index );
	}
	else
	{
		g_pBagFrame->m_GambleLockFlag = g_pBagFrame->m_GambleLockFlag & ~( 1 << index );
	}

	g_pInterface->SendWorldEvent( UiEvent_GambleItemChanged );

	return 0;
}

//-------------------------------------------------------------------------------------
int LuaFunc_Gamble_GetLockFlag(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;

	if( index < 0 || index > 6 )
	{
		return 0;
	}

	bool lock = g_pBagFrame->m_GambleLockFlag >> index & 1;
	
	lua_pushboolean( L, lock );

	return 1;
}



































//-------------------------------------------------------------------------------------
CBagItemIndex::CBagItemIndex():m_index(-1)
{

}

CBagItemIndex::CBagItemIndex( int index ):m_index(index)
{
	if( g_pBagFrame && m_index > 0 )
	{
		g_pBagFrame->Locked( m_index );
	}
}

CBagItemIndex::~CBagItemIndex()
{

}


//operators
void CBagItemIndex::operator = ( int index )
{
	if( g_pBagFrame && m_index > 0 )
	{
		g_pBagFrame->Unlocked( m_index );
	}
	m_index = index;
	if( g_pBagFrame && m_index > 0 )
	{
		g_pBagFrame->Locked( m_index );
	}
}

bool CBagItemIndex::operator == ( int index ) const
{
	return m_index == index;
}

CBagItemIndex::operator int() const
{
	return m_index;
}

void CBagItemIndex::Cut()
{
	if( g_pBagFrame && m_index > 0 )
	{
		g_pBagFrame->Locked( m_index );
		g_pBagFrame->Cut( m_index );
	}
	m_index = -1;
}
