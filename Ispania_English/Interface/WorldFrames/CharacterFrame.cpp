#include "../../mainproc/GameMain.h"
#include "../../netwaker_member/NetWakerClientInc.h"
#include "../Debug/DebugMessage.h"
#include "ItemClipboard.h"
#include "BagFrame.h"
#include "StoreFrame.h"
#include "CharacterFrame.h"
#include "../Houses/HousesFrame.h"
#include "ItemUtil.h"
#include "UnitFrame.h"

#include "../ItemLink/ItemLink.h"

#include "../SkillBook/UI_SkillBook.h"
#include "../../tools/Common/Com_Function.h"
#include "../../tools/Mstring/Mstring.h"


#include "RoleData/RoleBaseDef.h"

CCharacterFrame*			g_pCharacterFrame = NULL;




static string g_strCoolSuitModel;
int Func_CoolSuit_SetItemObjID( int equipIndex, int objectID );


// ----------------------------------------------------------------------------
// lua functions
//
// Ability
int	LuaFunc_GetPlayerAbility(lua_State* L);
int	LuaFunc_GetPlayerResistance(lua_State* L);
int	LuaFunc_GetPlayerGoodEvil(lua_State* L);
int LuaFunc_GetPlayerHonorPoint(lua_State* L);
int LuaFunc_GetPlayerArenaPoint(lua_State* L);
int LuaFunc_GetPlayerPointInfo(lua_State* L);
int LuaFunc_GetPlayerDuelistRewardPoint(lua_State* L);
int LuaFunc_GetPlayerMedalCount(lua_State* L);
int LuaFunc_GetGoodEvilTypeColor(lua_State* L);

//QuickEquipBar
int LuaFunc_GetEquipBySlot(lua_State* L);
int LuaFunc_GetEquipSlotInfo(lua_State* L);


// Equipment 
int	LuaFunc_PickupEquipmentItem(lua_State* L);
int	LuaFunc_UseEquipmentItem(lua_State* L);
int LuaFunc_GetPlayerIntroduce(lua_State* L);
int LuaFunc_SetPlayerIntroduce(lua_State* L);
int LuaFunc_EquipPendingItem(lua_State* L);
int LuaFunc_CancelPendingItem(lua_State* L);
int LuaFunc_EquipItem(lua_State* L);
int LuaFunc_GetEquipItemCooldown(lua_State* L);
int LuaFunc_GetEquipItemShown(lua_State* L);		// 裝備部位顯示狀態
int LuaFunc_SetEquipItemShown(lua_State* L);		// 裝備部位顯示
int LuaFunc_HasSecondEquipment(lua_State* L);		// 是否可以裝備第二套裝備
int LuaFunc_SwapEquipmentItem(lua_State* L);		// 交換裝備
int LuaFunc_GetEuipmentNumber(lua_State* L);		// 目前裝備編號
int LuaFunc_CharactFrame_GetEquipSlotCount(lua_State* L);

//魔法衣櫥
int LuaFunc_CoolClothBag_GetItemMax(lua_State* L);
int LuaFunc_CoolClothBag_GetItemCount(lua_State* L);
int LuaFunc_CoolClothBag_GetItem(lua_State* L);
int LuaFunc_CoolClothBag_Pickup(lua_State* L);
int LuaFunc_CoolClothBag_Equip(lua_State* L);
int LuaFunc_CoolClothBag_Delele(lua_State* L);

int LuaFunc_CoolClothBag_PushPendingItem(lua_State* L);
int LuaFunc_CoolClothBag_CancelPendingItem(lua_State* L);
int LuaFunc_CoolClothBag_GetPageInfo(lua_State* L);
int LuaFunc_CoolClothBag_BuyNewPage(lua_State* L);
int LuaFunc_CoolClothBag_GetPageTabEnable(lua_State* L);
//套裝
int LuaFunc_CoolSuit_GetTierInfo(lua_State* L);
int LuaFunc_CoolSuit_BuyNewTier(lua_State* L);
int LuaFunc_CoolSuit_GetTierEnable(lua_State* L);
int LuaFunc_CoolSuit_GetPageID(lua_State* L);
int LuaFunc_CoolSuit_SetPageID(lua_State* L);

int LuaFunc_CoolSuit_GetPageTexture(lua_State* L);

int LuaFunc_CoolSuit_Pickup(lua_State* L);

int	LuaFunc_CoolSuit_PickupEquipmentItem(lua_State* L);


int LuaFunc_CoolSuit_SetModel(lua_State* L);

int LuaFunc_CoolSuit_GetItemInfo(lua_State* L);

int LuaFunc_CoolSuit_SetItemObjID(lua_State* L);
int LuaFunc_CoolSuit_SetItemColor(lua_State* L);


int LuaFunc_CoolSuit_IsSetuping(lua_State* L);

int LuaFunc_CoolSuit_SetSuitRequest(lua_State* L);
int LuaFunc_CoolSuit_SetSuitCancel(lua_State* L);




// ----------------------------------------------------------------------------
// Skill Practice
int LuaFunc_GetPlayerSkilled(lua_State* L);
int LuaFunc_GetPlayerCurrentSkillValue(lua_State* L);
int LuaFunc_GetPlayerMaxSkillValue(lua_State* L);

//Title 稱號
int Luafunc_UpdateTitleInfo( lua_State* L );   //更新稱號資訊 
int Luafunc_ClearTitleInfo( lua_State* L );    //清除稱號資訊 
int Luafunc_GetTitleCount( lua_State* L );     //取得稱號數量
int Luafunc_GetTitleMaxLv( lua_State* L );     //取得最高稱號等級
int LuaFunc_GetTitleInfoByIndex(lua_State* L); //取得稱號資訊
int LuaFunc_GetCusomizeTitle(lua_State* L);    //取得自訂稱號
int LuaFunc_GetCurrentTitle(lua_State* L);     //取得現在的稱號
int Luafunc_SetTitleRequest(lua_State* L);     //裝備稱號
int LuaFunc_SetDisableTitleHide(lua_State* L); //在稱號裝備介面中 不可裝備的稱號要不要顯示
int LuaFunc_GetDisableTitleHide(lua_State* L); //在稱號裝備介面中 不可裝備的稱號要不要顯示


int LuaFunc_CheckAllTitles(lua_State* L);		//檢查有無新稱號

int LuaFunc_GetItemQualityColor(lua_State* L);		// 取得物品品質顏色


// 特殊旗標狀態
int LuaFunc_CheckHonorPartyState(lua_State* L);		// 檢查榮譽組隊旗標狀態

// ----------------------------------------------------------------------------
CCharacterFrame::CCharacterFrame(CInterface* object)
	: CInterfaceBase(object)
{
	g_pCharacterFrame = this;

	m_pickupItemIndex = -1;

	m_bTitleRequested = false;

	m_bCoolSuitSetuping = false;

	m_CoolSuitIndex = -1;

	for(int i=0;i<5;i++)
		m_SlotTab[i]=i+1;
}

// ----------------------------------------------------------------------------
CCharacterFrame::~CCharacterFrame()
{
	g_pCharacterFrame = NULL;
}
#define _COOL_SUIT_OPEN 1
// ----------------------------------------------------------------------------
void CCharacterFrame::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{		
		lua_register(L, "GetPlayerAbility",			LuaFunc_GetPlayerAbility);		
		lua_register(L, "GetPlayerResistance",		LuaFunc_GetPlayerResistance);
		lua_register(L, "GetPlayerGoodEvil",		LuaFunc_GetPlayerGoodEvil);
		lua_register(L, "GetPlayerHonorPoint",		LuaFunc_GetPlayerHonorPoint);
		lua_register(L, "GetPlayerDuelistRewardPoint",		LuaFunc_GetPlayerDuelistRewardPoint);
		lua_register(L, "GetPlayerArenaPoint",		LuaFunc_GetPlayerArenaPoint);
		lua_register(L, "GetPlayerPointInfo",		LuaFunc_GetPlayerPointInfo);

		
		lua_register(L, "GetPlayerMedalCount",		LuaFunc_GetPlayerMedalCount);
		lua_register(L, "GetGoodEvilTypeColor",		LuaFunc_GetGoodEvilTypeColor);

		lua_register(L, "GetEquipBySlot",			LuaFunc_GetEquipBySlot);		
		lua_register(L, "GetEquipSlotInfo",			LuaFunc_GetEquipSlotInfo);		
		
		lua_register(L, "PickupEquipmentItem",		LuaFunc_PickupEquipmentItem);
		lua_register(L, "UseEquipmentItem",			LuaFunc_UseEquipmentItem);
		lua_register(L, "GetRoleIntroduce",			LuaFunc_GetPlayerIntroduce);
		lua_register(L, "SetRoleIntroduce",			LuaFunc_SetPlayerIntroduce);
		lua_register(L, "EquipPendingItem",			LuaFunc_EquipPendingItem);
		lua_register(L, "CancelPendingItem",		LuaFunc_CancelPendingItem);
		lua_register(L, "EquipItem",				LuaFunc_EquipItem);
		lua_register(L, "GetEquipItemCooldown",		LuaFunc_GetEquipItemCooldown);		
		lua_register(L, "GetEquipItemShown",		LuaFunc_GetEquipItemShown);
		lua_register(L, "SetEquipItemShown",		LuaFunc_SetEquipItemShown);
		lua_register(L, "HasSecondEquipment",		LuaFunc_HasSecondEquipment);
		lua_register(L, "SwapEquipmentItem",		LuaFunc_SwapEquipmentItem);
		lua_register(L, "GetEuipmentNumber",		LuaFunc_GetEuipmentNumber);
		lua_register(L, "CharactFrame_GetEquipSlotCount",		LuaFunc_CharactFrame_GetEquipSlotCount);

		

#ifdef _COOL_SUIT_OPEN


		lua_register(L, "CoolClothBag_GetItemMax",	LuaFunc_CoolClothBag_GetItemMax);
		lua_register(L, "CoolClothBag_GetItemCount",	LuaFunc_CoolClothBag_GetItemCount);

		
		lua_register(L, "CoolClothBag_GetItem",		LuaFunc_CoolClothBag_GetItem);
		lua_register(L, "CoolClothBag_Pickup",		LuaFunc_CoolClothBag_Pickup);
		lua_register(L, "CoolClothBag_Equip",		LuaFunc_CoolClothBag_Equip);

		lua_register(L, "CoolClothBag_Delele",		LuaFunc_CoolClothBag_Delele);
		lua_register(L, "CoolClothBag_PushPendingItem",		LuaFunc_CoolClothBag_PushPendingItem);
		lua_register(L, "CoolClothBag_CancelPendingItem",	LuaFunc_CoolClothBag_CancelPendingItem);
		lua_register(L, "CoolClothBag_GetPageInfo",	LuaFunc_CoolClothBag_GetPageInfo);
		lua_register(L, "CoolClothBag_BuyNewPage",		LuaFunc_CoolClothBag_BuyNewPage);
		lua_register(L, "CoolClothBag_GetPageTabEnable",	LuaFunc_CoolClothBag_GetPageTabEnable);
		


			lua_register(L, "CoolSuit_GetTierInfo",		LuaFunc_CoolSuit_GetTierInfo);
		lua_register(L, "CoolSuit_BuyNewTier",		LuaFunc_CoolSuit_BuyNewTier);

		lua_register(L, "CoolSuit_GetTierEnable",	LuaFunc_CoolSuit_GetTierEnable);
		lua_register(L, "CoolSuit_GetPageID",		LuaFunc_CoolSuit_GetPageID);
		lua_register(L, "CoolSuit_SetPageID",		LuaFunc_CoolSuit_SetPageID);
		lua_register(L, "CoolSuit_GetPageTexture",	LuaFunc_CoolSuit_GetPageTexture);
		lua_register(L, "CoolSuit_Pickup",			LuaFunc_CoolSuit_Pickup);

		lua_register(L, "CoolSuit_PickupEquipmentItem",	LuaFunc_CoolSuit_PickupEquipmentItem);

		lua_register(L, "CoolSuit_SetModel",		LuaFunc_CoolSuit_SetModel);

		lua_register(L, "CoolSuit_GetItemInfo",		LuaFunc_CoolSuit_GetItemInfo);
		lua_register(L, "CoolSuit_SetItemObjID",	LuaFunc_CoolSuit_SetItemObjID);
		lua_register(L, "CoolSuit_SetItemColor",	LuaFunc_CoolSuit_SetItemColor);

		lua_register(L, "CoolSuit_IsSetuping",		LuaFunc_CoolSuit_IsSetuping);
		lua_register(L, "CoolSuit_SetSuitRequest",	LuaFunc_CoolSuit_SetSuitRequest);
		lua_register(L, "CoolSuit_SetSuitCancel",	LuaFunc_CoolSuit_SetSuitCancel);
#endif

		lua_register(L, "GetPlayerSkilled",			LuaFunc_GetPlayerSkilled);
		lua_register(L, "GetPlayerCurrentSkillValue", LuaFunc_GetPlayerCurrentSkillValue);
		lua_register(L, "GetPlayerMaxSkillValue",	LuaFunc_GetPlayerMaxSkillValue);

		lua_register(L, "UpdateTitleInfo",          Luafunc_UpdateTitleInfo);
		lua_register(L, "ClearTitleInfo",           Luafunc_ClearTitleInfo);
		lua_register(L, "GetTitleCount",            Luafunc_GetTitleCount);
		lua_register(L, "GetTitleMaxLv",            Luafunc_GetTitleMaxLv);	 		
		lua_register(L, "GetTitleInfoByIndex",      LuaFunc_GetTitleInfoByIndex);
		lua_register(L, "GetCusomizeTitle",         LuaFunc_GetCusomizeTitle);
		lua_register(L, "GetCurrentTitle",			LuaFunc_GetCurrentTitle);
		lua_register(L, "SetTitleRequest",			Luafunc_SetTitleRequest);
		lua_register(L, "SetDisableTitleHide",		LuaFunc_SetDisableTitleHide);
		lua_register(L, "GetDisableTitleHide",		LuaFunc_GetDisableTitleHide);

		lua_register(L, "CheckAllTitles",		LuaFunc_CheckAllTitles);



		lua_register(L, "GetItemQualityColor",		LuaFunc_GetItemQualityColor);

		lua_register(L, "CheckHonorPartyState",		LuaFunc_CheckHonorPartyState);
	}
}

void CCharacterFrame::BuildFrames()
{
	m_swapLocked = false;
}

// ----------------------------------------------------------------------------
void CCharacterFrame::Update(float elapsedTime)
{
	if ( m_pickupItemIndex != -1 )
	{
		ItemFieldStruct* item = CNetGlobal::RoleData()->GetEqItem(m_pickupItemIndex);
		if ( item )
		{
			if ( item->Pos == EM_ItemState_CliDisabled )
			{
				ClipboardItemInfo itemInfo = g_pItemClipboard->GetItemInfo();
				if (g_pItemClipboard->GetType() != eClipboardItem_Type_Equipment || itemInfo.equipment.pos != m_pickupItemIndex)
				{
					item->Pos = EM_ItemState_NONE;
					m_pickupItemIndex = -1;
				}					
			}
		}
		else
			m_pickupItemIndex = -1;
	}
  
	if( m_CoolSuitIndex != -1 )
	{
		static int s_SetCoolSuitIndex_LastTime = 0;
		if( timeGetTime() - s_SetCoolSuitIndex_LastTime < 2000 )
		{
			//修改太頻繁
			return ;
		}
		s_SetCoolSuitIndex_LastTime = timeGetTime();

		PlayerBaseRoleData* PlayerBaseData = CNetGlobal::RoleData()->PlayerBaseData;
		if( PlayerBaseData->CoolSuitID != m_CoolSuitIndex )
		{
			NetCli_ItemChild::ShowCoolSuitIndexRequest( m_CoolSuitIndex );
		}
		m_CoolSuitIndex = -1;
	}

}

// ----------------------------------------------------------------------------
void CCharacterFrame::ChangeZone()
{
	m_swapLocked = false;

	m_CoolSuitIndex = -1;
}

// ----------------------------------------------------------------------------
bool CCharacterFrame::Cut(int index)
{
	ItemFieldStruct* item = CNetGlobal::RoleData()->GetEqItem( index );
	if ( item )
	{
		GameObjDbStruct* itemDB = CNetGlobal::GetObj( item->OrgObjID );
		if ( itemDB )
		{
			item->Pos = EM_ItemState_CliDisabled;
			
			g_pItemClipboard->SetFile(ItemUTIL::GetItemIcon(item));
			g_pItemClipboard->PickupEquipment(item->OrgObjID, index);
			m_pickupItemIndex = index;
			return true;
		}
	}
	return false;
}

// ----------------------------------------------------------------------------
void CCharacterFrame::Paste(int index)
{	
	ItemFieldStruct* destItem = CNetGlobal::RoleData()->GetEqItem(index);
	ClipboardItemInfo itemInfo = g_pItemClipboard->GetItemInfo();

	// 拿取與做用物品相同
	if (g_pItemClipboard->GetType() == eClipboardItem_Type_Equipment && itemInfo.equipment.pos == index)
	{
		g_pItemClipboard->Clear();
		return;
	}

	// 物品資料錯誤或不存在
	if ( destItem == NULL || destItem->Pos != EM_ItemState_NONE )
		return;

	// 存在物品對物品使用中
	int useItem = g_pBagFrame->GetUseItemIndex();
	if ( useItem != -1 )
	{
		if ( !g_pBagFrame->UseItemToItem(1, index) )
			g_pBagFrame->UseItemCancel();
		return;
	}
	else if ( g_pStoreFrame->GetRepairState() )
	{
		switch (RoleDataEx::CheckFixEq( NULL , destItem ))
		{
		case EM_CheckFixEqResult_OK:
			NetCli_Shop::S_RepairWeaponRequest(index, true);
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
	
	switch(g_pItemClipboard->GetType())
	{
	case eClipboardItem_Type_None:					// 拿取物品
		Cut(index);
		return;

	case eClipboardItem_Type_Bag:					// 背包 -> 裝備
		Equip(index, 0, itemInfo.bag.pos);
		break;

	case eClipboardItem_Type_Bank:					// 銀行 -> 裝備
		Equip(index, 1, itemInfo.bank.pos);
		break;

	case eClipboardItem_Type_Equipment:				// 裝備 -> 裝備
		Equip(index, 2, itemInfo.equipment.pos);
		break;

	case eClipboardItem_Type_House:					// 房屋 -> 裝備
   		Equip(index, 3, itemInfo.house.pos, itemInfo.house.parentID);
		break;

	case eClipboardItem_Type_Trade:
	case eClipboardItem_Type_Action:
	case eClipboardItem_Type_Skill:
	case eClipboardItem_Type_Shop:
	case eClipboardItem_Type_Macro:
	case eClipboardItem_Type_Emote:
		break;
	}
	g_pItemClipboard->Clear();
}

//----------------------------------------------------------------------------
bool CCharacterFrame::Use(int index)
{
	RoleDataEx* role = CNetGlobal::RoleData();
	ItemFieldStruct* item = role->GetEqItem(index);

	if ( m_swapLocked )
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_EQUIPMENT_SWAP_LOCKED"));
		return false;
	}

	if ( item && !item->IsEmpty() && item->Pos == EM_ItemState_NONE )
	{
		GameObjDbStruct* itemDB = CNetGlobal::GetObj(item->OrgObjID);
		if ( itemDB )
		{
			CRoleSprite* targetPlayer = g_pGameMain->GetPlayerTarget();
			switch (itemDB->Item.UseType)
			{
			case EM_ItemUseType_Magic:
				if ( itemDB->Item.IncMagic_Onuse != 0 )
				{
					item->Pos = EM_USE_ITEM;
					if ( targetPlayer )
						NetCli_Item::_UseItem_EQ(*item, index, targetPlayer->GetIdentify());
					else
						NetCli_Item::_UseItem_EQ(*item, index, -1);

					return true;
				}
				break;

			case EM_ItemUseType_SrvScript:
			case EM_ItemUseType_UnPacket:
			case EM_ItemUseType_UnPacket_DropList:
			case EM_ItemUseType_Egg:
				item->Pos = EM_USE_ITEM;
				if ( targetPlayer )
					NetCli_Item::_UseItem_EQ(*item, index, targetPlayer->GetIdentify());
				else
					NetCli_Item::_UseItem_EQ(*item, index, -1);
				return true;			
			}
		}
	}
	return false;
}

// ----------------------------------------------------------------------------
void CCharacterFrame::UpdateCooldown(int ColdownClass, int ColdownType)
{
	SendWorldEvent("EQUIPMENT_UPDATE_COOLDOWN");
}

// ----------------------------------------------------------------------------
void CCharacterFrame::EquipPendingItem(int slot)
{
	map<int, EquipPendingSlot>::iterator iter = m_pendingSolt.find(slot);
	if ( iter != m_pendingSolt.end() )
	{
		SendEquip(slot, (iter->second).type, (iter->second).index, (iter->second).ContainerID);
		m_pendingSolt.erase(iter);
	}
}

// ----------------------------------------------------------------------------
void CCharacterFrame::CancelPendingItem(int slot)
{
	map<int, EquipPendingSlot>::iterator iter = m_pendingSolt.find(slot);
	if ( iter != m_pendingSolt.end() )
	{
		m_pendingSolt.erase(iter);
	}
}

// ----------------------------------------------------------------------------
void CCharacterFrame::Equip(int slot, int type, int index ,int ContainerID)
{
	ItemFieldStruct* equipItem = CNetGlobal::RoleData()->GetEqItem(slot);
	ItemFieldStruct* item = NULL;
	if ( equipItem == NULL )
		return;

	GameObjDbStruct* equipItemDB = g_ObjectData->GetObj(equipItem->OrgObjID);
	switch (type)
	{
	case 0:
		if ( CNetGlobal::RoleData()->IsActiveBagItem(index) == false )
		{
			g_pGameMain->SendWarningMsg( g_ObjectData->GetString("SYS_INACTIVE_BAG_FIELD") );
			return;
		}

		switch ( slot )
		{
		case EM_EQWearPos_MagicTool1:
		case EM_EQWearPos_MagicTool2:
		case EM_EQWearPos_MagicTool3:
			// 底下會判斷
			break;

		default:
			// 商城背包
			if ( index < _MAX_BODY_EX_COUNT_ && (equipItem && equipItem->IsEmpty() == false && equipItemDB->Mode.DepartmentStore == false) )
			{
				// 不能放回商城背包
				// g_pGameMain->SendWarningMsg( g_ObjectData->GetString("SYS_EQ_TO_GOODS_FAILED") );
				g_pGameMain->SendWarningMsg( g_ObjectData->GetString("SYS_CANNOT_MOVE_TO_ACCOUNTBAG") );
				return;
			}
			break;
		}
		item = CNetGlobal::RoleData()->GetBodyItem(index);		
		break;

	case 1:
		item = CNetGlobal::RoleData()->GetBankItem(index);
		break;

	case 2:
		if ( (slot == EM_EQWearPos_MagicTool1 || slot == EM_EQWearPos_MagicTool2 || slot == EM_EQWearPos_MagicTool3) &&
			 (index == EM_EQWearPos_MagicTool1 || index == EM_EQWearPos_MagicTool2 || index == EM_EQWearPos_MagicTool3) )
		{
			// 法保不能交換位置
			g_pGameMain->SendWarningMsg( g_ObjectData->GetString("SYS_MAGICTOOL_NOT_CHANGE") );
			return;
		}
		// 裝備不能互相取代
		item = CNetGlobal::RoleData()->GetEqItem(index);
	
	case 3: //房屋容器
		{
			HouseItemDBStruct* pHouseItemDB = g_pHousesFrame->GetHousesItemDB( ContainerID , index );
			if( pHouseItemDB )
			{
				item = &pHouseItemDB->Item;
			}
			else
			{
				// 可以交換裝備
				SendEquip(slot, type, index, ContainerID);
				return;
			}
		}
		break;
	}

	if ( item == NULL )
		return;	

	switch (slot)
	{
	case EM_EQWearPos_MagicTool1:
	case EM_EQWearPos_MagicTool2:
	case EM_EQWearPos_MagicTool3:
		if ( !equipItem->IsEmpty() && item->IsEmpty() )
		{
			// 法保不能交換位置
			g_pGameMain->SendWarningMsg( g_ObjectData->GetString("SYS_MAGICTOOL_NOT_CHANGE") );			
			return;
		}
		else if ( !equipItem->IsEmpty() )
		{
			// 復
			PendingEquip(slot, type, index, ContainerID );
			return;
		}		
	}

	// 未綁定
	if ( item->Mode.Trade && item->Mode.EQSoulBound )
	{
		PendingEquip(slot, type, index, ContainerID);
		return;
	}
	
	// 可以直接裝備
	SendEquip(slot, type, index, ContainerID);
}

// ----------------------------------------------------------------------------
void CCharacterFrame::SendEquip(int slot, int type, int index, int ContainerID)
{
	ItemFieldStruct* item = NULL;
	switch (type)
	{
	case 0:
		if ( (item = CNetGlobal::RoleData()->GetBodyItem(index)) != NULL )
			NetCli_EQChild::S_EQ2Body(*item, index, slot);
		break;

	case 1:
		if ( (item = CNetGlobal::RoleData()->GetBankItem(index)) != NULL )
			NetCli_EQChild::S_SetEQ_Bank(*item, index, slot, false);
		break;

	case 2:
		if ( (item = CNetGlobal::RoleData()->GetEqItem(index)) != NULL )
			NetCli_EQChild::S_SetEQ_EQ(*item, index, slot);
		break;
	case 3:
			NetCli_Houses::S_ExchangeEq( ContainerID , index , (EQWearPosENUM)slot );
		break;
	}

	switch (slot)
	{
	case EM_EQWearPos_Necklace:
	case EM_EQWearPos_Ring1:
	case EM_EQWearPos_Ring2:
	case EM_EQWearPos_Earring1:
	case EM_EQWearPos_Earring2:
	case EM_EQWearPos_Head:
	case EM_EQWearPos_Gloves:
	case EM_EQWearPos_Shoes:
	case EM_EQWearPos_Clothes:
	case EM_EQWearPos_Pants:
	case EM_EQWearPos_Back:
	case EM_EQWearPos_Belt:
	case EM_EQWearPos_Shoulder:	
	case EM_EQWearPos_Ammo:
	case EM_EQWearPos_Bow:
	case EM_EQWearPos_MainHand:
	case EM_EQWearPos_SecondHand:
	case EM_EQWearPos_Manufactory:	
	case EM_EQWearPos_MagicTool1:
	case EM_EQWearPos_MagicTool2:
	case EM_EQWearPos_MagicTool3:
	case EM_EQWearPos_Ornament:
		g_pRuGlobalEventManager->PlaySoundByFile("sound\\interface\\ui_player_equip.mp3", false, 0.0f, ruSOUNDTYPE_INTERFACE);
		break;
	}	
}

// ----------------------------------------------------------------------------
void CCharacterFrame::PendingEquip(int slot, int type, int index,int ContainerID)
{
	EquipPendingSlot srcItem;
	srcItem.type = type;
	srcItem.index = index;
	srcItem.ContainerID = ContainerID;

	map<int, EquipPendingSlot>::iterator iter = m_pendingSolt.find(slot);
	if ( iter != m_pendingSolt.end() )
		iter->second = srcItem;
	else
		m_pendingSolt.insert(make_pair(slot, srcItem));

	lua_State* L = GetLuaState();
	lua_pushnumber(L, slot);
	lua_setglobal(L, "arg1");
	if ( (slot == EM_EQWearPos_MagicTool1) || (slot == EM_EQWearPos_MagicTool2) || (slot == EM_EQWearPos_MagicTool3) )
		SendWorldEvent("TALISMAN_BIND_CONFIRM");
	else
		SendWorldEvent("EQUIP_BIND_CONFIRM");
}

// ----------------------------------------------------------------------------
int CCharacterFrame::SearchEquip(int createTime, int serial)
{
	// 搜尋裝備
	for ( int i = 0; i < EM_EQWearPos_MaxCount; i++ )
	{
		ItemFieldStruct* itemField = CNetGlobal::RoleData()->GetEqItem(i);
		if ( itemField != NULL && itemField->IsEmpty() == false ) {
			if ( createTime == itemField->CreateTime && serial == itemField->Serial ) {
				return i;
			}
		}
	}
	return -1;
}

// ----------------------------------------------------------------------------
void CCharacterFrame::SwapEquip(int slotID)
{
	if ( m_swapLocked )
		return;

	int currID=CNetGlobal::RoleData()->PlayerBaseData->ClientData[255] ;
	if (currID==slotID)
		return ;

	int count = 0;
	for ( int i = 0; i < EM_Max_Vocation; i++ )
	{
		if ( CNetGlobal::RoleData()->PlayerBaseData->AbilityList[i].Level > 0 )
			count++;
	}

	if ( count <= 1 )
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_SWAP_EQUIPMENT_ERROR"));
		return;
	}	

	// 是否死亡
	CRoleSprite* player = g_pGameMain->GetPlayer();
	if ( player && player->GetDeadState() )
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_SWAPEQ_ERROR_DEAD"));
		return;
	}

	if ( player == NULL )
	{
		return;
	}

	/*
	// 是否戰鬥中
	if ( g_pGameMain->GetCombatState() )
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_SWAPEQ_ERROR_COMBAT"));
		return;
	}
	*/
	CheckSlotTab();
	int slotIndex=0;
	for (int i=0;i<5;i++)
	{
		if (m_SlotTab[i]==slotID)
		{
			slotIndex=i;
			break;
		}


	}
	m_slotIndex=slotIndex;

	NetCli_EQ::S_SwapBackupEQ( slotIndex );
	m_swapLocked = true;
}
void CCharacterFrame::SaveClientData()
{

	g_pGameMain->SaveClientData(eSaveClientData_EquipSlotTab, 5, m_SlotTab);
	

}
void  CCharacterFrame::CheckSlotTab()
{

	int currID=CNetGlobal::RoleData()->PlayerBaseData->ClientData[255] ;


	bool rebuild=false;

	char v[6]={0};

	for (int i=0;i<5;i++)
	{
		int t=m_SlotTab[i];
		if (t==currID)
		{
			rebuild=true;
			break;
		}
		if (t<0&&t>5)
		{
			rebuild=true;
			break;
		}
		v[t]++;
		if (v[t]>1)
		{
			rebuild=true;
			break;
		}


	}
	if (rebuild)
	{
		char temp[7]={0,1,2,3,4,5};
		for (int i=0;i<6;i++)
		{
			if (temp[i]==currID)
			{
				for (int j=i+1;j<6;j++)
				{
					temp[j-1]=temp[j];

				}
				break;
			}
		}
		for (int i=0;i<5;i++)
		{
			m_SlotTab[i]=temp[i];

		}


	}

}

void CCharacterFrame::LoadClientData(int type, int size, char* data)
{
	if (eSaveClientData_EquipSlotTab==type)
	{

		if (size==5)
			memcpy(m_SlotTab,data,size);


		CheckSlotTab();
		






	}

}
// ----------------------------------------------------------------------------
void CCharacterFrame::SwapEquipResult(bool success)
{
	m_swapLocked = false;

	if ( success )
	{


		int currID=CNetGlobal::RoleData()->PlayerBaseData->ClientData[255] ;

		int newID=m_SlotTab[m_slotIndex];

		CNetGlobal::RoleData()->PlayerBaseData->ClientData[255]=newID;
		m_SlotTab[m_slotIndex]=currID;

		SaveClientData();

/*

		if ( CNetGlobal::RoleData()->PlayerBaseData->ClientData[255] == 0 ) 
			CNetGlobal::RoleData()->PlayerBaseData->ClientData[255] = 1;
		else
			CNetGlobal::RoleData()->PlayerBaseData->ClientData[255] = 0;
			*/

		// 送出封包
		NetCli_RoleValue::S_ClientData(CNetGlobal::RoleData()->PlayerBaseData->ClientData);

		SendWorldEvent("SWAP_EQUIPMENT_SUCCESS");
	}
}

// ----------------------------------------------------------------------------
//S_SetTitleRequest 
// ----------------------------------------------------------------------------
void CCharacterFrame::S_SetTitleRequest( int TitleID )
{
	if( !m_bTitleRequested )
	{
		m_bTitleRequested = true;

		NetCli_RoleValue::S_SetTitleRequest( TitleID , true );
	}

}

// ----------------------------------------------------------------------------
//R_SetTitle_OK 
// ----------------------------------------------------------------------------
void CCharacterFrame::R_SetTitle_OK( int TitleID )
{
	RoleDataEx* Role = CNetGlobal::RoleData();
	Role->BaseData.TitleID = TitleID;
	m_interface->SendWorldEvent(UiEvent_PlayerTitleIDChanged);
	m_bTitleRequested = false;

	//稱號技能
	g_pUi_SkillBook->SkillUpdate( 0 ); // 技能書第一頁更新
}

// ----------------------------------------------------------------------------
//R_SetTitle_Failed 
// ----------------------------------------------------------------------------
void CCharacterFrame::R_SetTitle_Failed( )
{
	m_interface->SendWorldEvent(UiEvent_PlayerTitleIDChanged);
	m_bTitleRequested = false;
}


// ----------------------------------------------------------------------------
//更新稱號資訊 
// ----------------------------------------------------------------------------
void CCharacterFrame::UpdateTitleInfo()    
{

	m_TitleInfos.clear();
	
	GameObjDbStructEx* pTitleObj;
	
	PlayerBaseRoleData* pBaseData = CNetGlobal::RoleData()->PlayerBaseData;

	//TempRoleData* pTempData = &CNetGlobal::RoleData()->TempData;

	
	//bool bDisable;
	
	int nTitleID;

	int index;

	bool bShow;
	bool bGeted;

	sTitleInfo temp;

	for( index = 0 ; index < _MAX_TITLE_FLAG_COUNT_ ; ++index )
	{
		bShow = false;
		bGeted = false;

  		nTitleID = index + Def_ObjectClass_Title;
		pTitleObj = g_ObjectData->GetObj( nTitleID );

		if( pBaseData->Title.GetFlag(index) )
		{
			bShow = true;
			bGeted = true;
   		}
		else if( pTitleObj && pTitleObj->Mode.PreViewTitle )
		{
			bShow = true;
		}

		if( bShow )
		{
			if( pTitleObj == NULL || pTitleObj->Classification != EM_ObjectClass_Title )
			{
				//X_ERR輸出錯誤
				//g_pError->AddMessage(0, 0, "not Find title id %d", nTitleID);
				continue;
			}
					

			//判斷 能不能裝備
			if( false == CNetGlobal::RoleData()->CheckGetJobTitle( nTitleID ) )
			{
				bGeted = false;
			}

//			if( bDisable && g_pGameMain->GetDisableTitleHide() )
//				continue;

			temp.nID = nTitleID;
			temp.bGeted = bGeted;

			m_TitleInfos.push_back( temp );
		}
	}


}

// ----------------------------------------------------------------------------
//清除稱號資訊
// ----------------------------------------------------------------------------
void CCharacterFrame::ClearTitleInfo()
{
	m_TitleInfos.clear();
}

// ----------------------------------------------------------------------------
//取得稱號數量
// ----------------------------------------------------------------------------
int CCharacterFrame::GetTitleCount()      
{
	return (int)m_TitleInfos.size();
}
// ----------------------------------------------------------------------------
//取得最高稱號等級
// ----------------------------------------------------------------------------
int CCharacterFrame::GetTitleMaxLv()
{	
	int MaxRare = 0;
	GameObjDbStructEx* pTitleObj;
	for( int i = 0 ; i < m_TitleInfos.size() ; i++)
	{
		pTitleObj = g_ObjectData->GetObj( m_TitleInfos[i].nID );
		if( pTitleObj )
		{
			if( pTitleObj->Rare > MaxRare )
			{
				MaxRare = pTitleObj->Rare;
			}
		}
	} 
	return MaxRare;
}
// ----------------------------------------------------------------------------
//取得稱號資訊
// ----------------------------------------------------------------------------
sTitleInfo* CCharacterFrame::GetTitleInfoByIndex( int index ) 
{
	if( index < 0 || index >= (int)m_TitleInfos.size() )
	{
		//X_ERR輸出錯誤 
		return NULL;
	}

	return &m_TitleInfos[index];
}

// ----------------------------------------------------------------------------
void CCharacterFrame::SetCoolSuitIndex( int index )
{
	if( index < 0 || index >= _DEF_MAX_COOL_SUIT_COUNT_ )
	{
		return ;
	}

	PlayerBaseRoleData* PlayerBaseData = CNetGlobal::RoleData()->PlayerBaseData;
	if( PlayerBaseData->CoolSuitID == index )
	{
		m_CoolSuitIndex = -1;
		return ;
	}

	//NetCli_ItemChild::ShowCoolSuitIndexRequest( index );

	m_CoolSuitIndex = index;
	m_bCoolSuitSetuping = false;

}

// ----------------------------------------------------------------------------
void CCharacterFrame::PickupCoolSuit( int index )
{
	string texture = GetCoolSuitPageTexture( index );
	g_pItemClipboard->SetFile( texture.c_str() );
	g_pItemClipboard->PickupCoolSuit( index );
}

struct _CoolSuitEqIndex
{
	_CoolSuitEqIndex( int _eq, int _coolSuit )
	{
		eqIndex = _eq;
		coolSuitIndex = _coolSuit;
	}
	int eqIndex;
	int coolSuitIndex;
};

vector<_CoolSuitEqIndex> g_CoolSuitEqIndexTable;


// ----------------------------------------------------------------------------
void CCharacterFrame::CoolSuitSetupChanged()
{
	//g_pGameMain->GetGItemID();

	//LookEQInfoStruct EQLook;

	//if( g_CoolSuitEqIndexTable.empty() )
	//{
	//	g_CoolSuitEqIndexTable.push_back( _CoolSuitEqIndex( EM_EQWearPos_Head       ,  0 ) ); //Head;        
	//	g_CoolSuitEqIndexTable.push_back( _CoolSuitEqIndex( EM_EQWearPos_Gloves     ,  1 ) ); //Gloves;	   
	//	g_CoolSuitEqIndexTable.push_back( _CoolSuitEqIndex( EM_EQWearPos_Shoes      ,  2 ) ); //Shoes;	
	//	g_CoolSuitEqIndexTable.push_back( _CoolSuitEqIndex( EM_EQWearPos_Clothes    ,  3 ) ); //Clothes;	 
	//	g_CoolSuitEqIndexTable.push_back( _CoolSuitEqIndex( EM_EQWearPos_Pants      ,  4 ) ); //Pants;	
	//	g_CoolSuitEqIndexTable.push_back( _CoolSuitEqIndex( EM_EQWearPos_Back       ,  5 ) ); //Back;		 
	//	g_CoolSuitEqIndexTable.push_back( _CoolSuitEqIndex( EM_EQWearPos_Belt       ,  6 ) ); //Belt;		 
	//	g_CoolSuitEqIndexTable.push_back( _CoolSuitEqIndex( EM_EQWearPos_Shoulder   ,  7 ) ); //Shoulder;	 
	//	g_CoolSuitEqIndexTable.push_back( _CoolSuitEqIndex( EM_EQWearPos_MainHand   , -1 ) ); //MainHand;	 
	//	g_CoolSuitEqIndexTable.push_back( _CoolSuitEqIndex( EM_EQWearPos_SecondHand , -1 ) ); //SecondHand;  
	//	g_CoolSuitEqIndexTable.push_back( _CoolSuitEqIndex( EM_EQWearPos_Bow        , -1 ) ); //Bow;	
	//	g_CoolSuitEqIndexTable.push_back( _CoolSuitEqIndex( EM_EQWearPos_Manufactory, -1 ) ); //Manufacture;
	//	g_CoolSuitEqIndexTable.push_back( _CoolSuitEqIndex( EM_EQWearPos_Ornament   ,  8 ) ); //Ornament;
	//}

	//CoolSuitBase* pCoolSuit;
	//if( m_bCoolSuitSetuping )
	//{
	//	pCoolSuit = &m_CoolSuitSetupTemp;
	//}
	//else
	//{
	//	PlayerBaseRoleData* PlayerBaseData = CNetGlobal::RoleData()->PlayerBaseData;
	//	int suitID = PlayerBaseData->CoolSuitID;
	//	if( suitID < 0 || suitID >= _DEF_MAX_COOL_SUIT_COUNT_ )
	//	{
	//		return ;
	//	}
	//	pCoolSuit = &PlayerBaseData->CoolSuitList[suitID];
	//}

	//CoolClothBase* pCoolCloth;
	//ItemFieldStruct* pItemField;
	//for( int i = 0 ; i < g_CoolSuitEqIndexTable.size() ; ++i )
	//{
	//	_CoolSuitEqIndex& rInfo = g_CoolSuitEqIndexTable[i];

	//	pCoolCloth = &pCoolSuit->Item[rInfo.coolSuitIndex];

	//	EQLook.EQ.EQ[i] = pCoolCloth->ImageObjectID;
	//	EQLook.MainColor.EQ[i] = pCoolCloth->MainColor;
	//	EQLook.OffColor.EQ[i] = pCoolCloth->OffColor;
	//	EQLook.Level.EQ[i] = 0;

	//	if( EQLook.EQ.EQ[i] == 0 )
	//	{
	//		pItemField = CNetGlobal::RoleData()->GetEqItem( rInfo.eqIndex );
	//		if( pItemField )
	//		{
	//			EQLook.EQ.EQ[i] = pItemField->ImageObjectID;
	//			EQLook.Level.EQ[i] = pItemField->Level;
	//		}
	//	}
 //	}
 //  
	////NetCli_MoveChild::EQInfo( g_pGameMain->GetGItemID(), EQLook, m_CoolSuitSetupTemp.ShowEQ );
	//CRoleSprite* pSprite = g_pGameMain->GetPlayer();
	//if( pSprite )
	//{
	//	NetCli_MoveChild::SetEquipmentInfo( pSprite, EQLook, pCoolSuit->ShowEQ );

	//	if ( g_pInterface )
	//	{
	//		lua_State* L = g_pInterface->GetLuaState();
	//		if ( L )
	//		{
	//			g_pInterface->SendWorldEvent( UiEvent_RoleCoolSuitChange );
	//		}
	//	}
	//}
}

// ----------------------------------------------------------------------------
string CCharacterFrame::GetCoolSuitPageTexture( int PageID )
{
	string texture = Com_FormatString( "interface\\Icons\\CoolSuit\\CoolSuit_%02d.dds", PageID + 1 );

	//有胸部 用胸部的圖
	if( PageID < 0 || PageID >= _DEF_MAX_COOL_SUIT_COUNT_ )
	{
		return texture;
	}

	PlayerBaseRoleData* PlayerBaseData = CNetGlobal::RoleData()->PlayerBaseData;
	CoolSuitBase* pCoolSuitBase = &PlayerBaseData->CoolSuitList[PageID];

	if( g_pCharacterFrame->m_bCoolSuitSetuping && PageID == PlayerBaseData->CoolSuitID )
	{
		pCoolSuitBase = &g_pCharacterFrame->m_CoolSuitSetupTemp;
	}

	if( pCoolSuitBase->Clothes.ImageObjectID <= 0 )
	{
		return texture;
	}

	GameObjDbStruct* itemDB = CNetGlobal::GetObj( pCoolSuitBase->Clothes.ImageObjectID );
	if( NULL == itemDB )
	{
		return texture;
	}

	GameObjDbStruct* imageDB = g_ObjectData->GetObj(itemDB->ImageID);
	if( NULL == imageDB )
	{
		return texture;
	}

	texture = imageDB->Image.ACTField;
	return texture;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetPlayerAbility(lua_State* L)
{
	int level = CNetGlobal::RoleBaseAttr()->Level;

	const char* keyStr = luaL_checkstring(L, 1);	
	if ( strcmp(keyStr, "STR") == 0 )
	{
		int temp[3];
		CNetGlobal::RoleData()->GetAbilityDetailValues(EM_RoleValue_STR, temp);
		lua_pushnumber(L, temp[0] + temp[1]);
		lua_pushnumber(L, temp[2]);
		return 2;
	}
	else if ( strcmp(keyStr, "STA") == 0 )
	{
		int temp[3];
		CNetGlobal::RoleData()->GetAbilityDetailValues(EM_RoleValue_STA, temp);
		lua_pushnumber(L, temp[0] + temp[1]);
		lua_pushnumber(L, temp[2]);
		return 2;
	}
	else if ( strcmp(keyStr, "AGI") == 0 )
	{
		int temp[3];
		CNetGlobal::RoleData()->GetAbilityDetailValues(EM_RoleValue_AGI, temp);
		lua_pushnumber(L, temp[0] + temp[1]);
		lua_pushnumber(L, temp[2]);
		return 2;
	}
	else if ( strcmp(keyStr, "INT") == 0 )
	{
		int temp[3];
		CNetGlobal::RoleData()->GetAbilityDetailValues(EM_RoleValue_INT, temp);
		lua_pushnumber(L, temp[0] + temp[1]);
		lua_pushnumber(L, temp[2]);
		return 2;
	}
	else if ( strcmp(keyStr, "MND") == 0 )
	{
		int temp[3];
		CNetGlobal::RoleData()->GetAbilityDetailValues(EM_RoleValue_MND, temp);
		lua_pushnumber(L, temp[0] + temp[1]);
		lua_pushnumber(L, temp[2]);
		return 2;
	}
	// 主手傷害
	else if ( strcmp(keyStr, "MELEE_MAIN_DAMAGE") == 0 )
	{
		float value[2];
		value[0] = CNetGlobal::RoleBaseAttr()->Fin.Main_DMG;
		value[1] = CNetGlobal::RoleData()->TempData.Attr.Fin.Main_DMG - value[0];		
		lua_pushnumber(L, floor(value[0]));
		lua_pushnumber(L, floor(value[1]));
		return 2;
	}
	// 副手傷害
	else if ( strcmp(keyStr, "MELEE_OFF_DAMAGE") == 0 )
	{
		float value[2];
		value[0] = CNetGlobal::RoleBaseAttr()->Fin.Sec_DMG;
		value[1] = CNetGlobal::RoleData()->TempData.Attr.Fin.Sec_DMG - value[0];		
		lua_pushnumber(L, floor(value[0]));
		lua_pushnumber(L, floor(value[1]));
		return 2;
	}
	// 主手強度
	else if ( strcmp(keyStr, "MELEE_ATTACK") == 0 )
	{
		float value[2];
		value[0] = CNetGlobal::RoleBaseAttr()->Fin.ATK;
		value[1] = CNetGlobal::RoleData()->TempData.Attr.Fin.ATK - value[0];
		lua_pushnumber(L, floor(value[0]));
		lua_pushnumber(L, floor(value[1]));
		return 2;
	}
	// 近戰爆擊
	else if ( strcmp(keyStr, "MELEE_CRITICAL") == 0 )
	{
		float value[3];
		value[0] = CNetGlobal::RoleBaseAttr()->Fin.Main_CriticalHit;
		value[1] = CNetGlobal::RoleData()->TempData.Attr.Fin.Main_CriticalHit - value[0];
		value[2] = CNetGlobal::RoleData()->TempData.Attr.CalCritialRate(value[0] + value[1], level, 0.0f) * 100.0f;
		lua_pushnumber(L, floor(value[0]));
		lua_pushnumber(L, floor(value[1]));
		lua_pushnumber(L, value[2]);
		return 3;
	}
	else if ( strcmp(keyStr, "MELEE_MAIN_CRITICAL") == 0 )
	{
		float value[3];
		value[0] = CNetGlobal::RoleBaseAttr()->Fin.Main_CriticalHit;
		value[1] = CNetGlobal::RoleData()->TempData.Attr.Fin.Main_CriticalHit - value[0];
		value[2] = CNetGlobal::RoleData()->TempData.Attr.CalCritialRate(value[0] + value[1], level, 0.0f) * 100.0f;
		lua_pushnumber(L, floor(value[0]));
		lua_pushnumber(L, floor(value[1]));
		lua_pushnumber(L, value[2]);
		return 3;
	}
	// 近戰爆擊
	else if ( strcmp(keyStr, "MELEE_OFF_CRITICAL") == 0 )
	{
		float value[3];
		value[0] = CNetGlobal::RoleBaseAttr()->Fin.Sec_CriticalHit;
		value[1] = CNetGlobal::RoleData()->TempData.Attr.Fin.Sec_CriticalHit - value[0];
		value[2] = CNetGlobal::RoleData()->TempData.Attr.CalCritialRate(value[0] + value[1], level, 0.0f) * 100.0f;
		lua_pushnumber(L, floor(value[0]));
		lua_pushnumber(L, floor(value[1]));
		lua_pushnumber(L, value[2]);
		return 3;
	}
	// 遠程傷害
	else if ( strcmp(keyStr, "RANGE_DAMAGE") == 0 )
	{
		float value[2];
		value[0] = CNetGlobal::RoleBaseAttr()->Fin.Bow_DMG;
		value[1] = CNetGlobal::RoleData()->TempData.Attr.Fin.Bow_DMG - value[0];		
		lua_pushnumber(L, floor(value[0]));
		lua_pushnumber(L, floor(value[1]));
		return 2;
	}
	// 遠程強度
	else if ( strcmp(keyStr, "RANGE_ATTACK") == 0 )
	{
		float value[2];
		value[0] = CNetGlobal::RoleBaseAttr()->Fin.ATK;
		value[1] = CNetGlobal::RoleData()->TempData.Attr.Fin.ATK - value[0];
		lua_pushnumber(L, floor(value[0]));
		lua_pushnumber(L, floor(value[1]));
		return 2;
	}
	// 遠程爆擊
	else if ( strcmp(keyStr, "RANGE_CRITICAL") == 0 )
	{
		float value[3];
		value[0] = CNetGlobal::RoleBaseAttr()->Fin.Bow_CriticalHit;
		value[1] = CNetGlobal::RoleData()->TempData.Attr.Fin.Bow_CriticalHit - value[0];
		value[2] = CNetGlobal::RoleData()->TempData.Attr.CalCritialRate(value[0] + value[1], level, 0.0f) * 100.0f;
		lua_pushnumber(L, floor(value[0]));
		lua_pushnumber(L, floor(value[1]));
		lua_pushnumber(L, value[2]);
		return 3;
	}
	// 遠程命中等級
	else if ( strcmp(keyStr, "RANGE_HIT") == 0 )
	{
		float value[2];
		value[0] = CNetGlobal::RoleBaseAttr()->Fin.HitRate;
		value[1] = CNetGlobal::RoleData()->TempData.Attr.Fin.HitRate - value[0];
		lua_pushnumber(L, floor(value[0]));
		lua_pushnumber(L, floor(value[1]));
		return 2;
	}
	// 法術傷害
	else if ( strcmp(keyStr, "MAGIC_DAMAGE") == 0 )
	{
		float value[2];
		value[0] = CNetGlobal::RoleBaseAttr()->Fin.MDMG;
		value[1] = CNetGlobal::RoleData()->TempData.Attr.Fin.MDMG - value[0];		
		lua_pushnumber(L, floor(value[0]));
		lua_pushnumber(L, floor(value[1]));
		return 2;
	}
	// 治療加成
	else if ( strcmp(keyStr, "MAGIC_HEAL") == 0 )
	{
		float scale = RoleValueData::FixArgTable[1].Dmg_HealArg;
		float value[2];
		value[0] = CNetGlobal::RoleBaseAttr()->Fin.MDMG * scale;
		value[1] = CNetGlobal::RoleData()->TempData.Attr.Fin.MDMG * scale - value[0];
		lua_pushnumber(L, floor(value[0]));
		lua_pushnumber(L, floor(value[1]));
		return 2;
	}
	// 法術強度
	else if ( strcmp(keyStr, "MAGIC_ATTACK") == 0 )
	{
		float value[2];
		value[0] = CNetGlobal::RoleBaseAttr()->Fin.MATK;
		value[1] = CNetGlobal::RoleData()->TempData.Attr.Fin.MATK - value[0];
		lua_pushnumber(L, floor(value[0]));
		lua_pushnumber(L, floor(value[1]));
		return 2;
	}
	// 法術爆擊
	else if ( strcmp(keyStr, "MAGIC_CRITICAL") == 0 )
	{
		float value[3];
		value[0] = CNetGlobal::RoleBaseAttr()->Fin.MagicCriticalHit;
		value[1] = CNetGlobal::RoleData()->TempData.Attr.Fin.MagicCriticalHit - value[0];
		value[2] = CNetGlobal::RoleData()->TempData.Attr.CalMagCritialRate(value[0] + value[1], level, 0.0f) * 100.0f;
		lua_pushnumber(L, floor(value[0]));
		lua_pushnumber(L, floor(value[1]));
		lua_pushnumber(L, value[2]);
		return 3;
	}
	// 物理防禦
	else if ( strcmp(keyStr, "PHYSICAL_DEFENCE") == 0 )
	{
		float value[2];
		value[0] = CNetGlobal::RoleBaseAttr()->Fin.DEF;
		value[1] = CNetGlobal::RoleData()->TempData.Attr.Fin.DEF - value[0];
		lua_pushnumber(L, floor(value[0]));
		lua_pushnumber(L, floor(value[1]));
		return 2;
	}
	// 物理格檔
	else if ( strcmp(keyStr, "PHYSICAL_PARRY") == 0 )
	{
		float value[3];
		value[0] = CNetGlobal::RoleBaseAttr()->Fin.ParryRate;
		value[1] = CNetGlobal::RoleData()->TempData.Attr.Fin.ParryRate - value[0];
		value[2] = CNetGlobal::RoleData()->TempData.Attr.CalParryRate(level, 0.0f) * 100.0f;
		lua_pushnumber(L, floor(value[0]));		// 基礎值
		lua_pushnumber(L, floor(value[1]));		// 額外增加
		lua_pushnumber(L, value[2]);			// 同等級發生率
		return 3;
	}
	// 同等級盾牌格檔率
	else if ( strcmp(keyStr, "PHYSICAL_SHIELDBLOCK") == 0 )
	{
		float value[3];
		value[0] = CNetGlobal::RoleBaseAttr()->Fin.ShieldBlockRate;
		value[1] = CNetGlobal::RoleData()->TempData.Attr.Fin.ShieldBlockRate - value[0];
		value[2] = CNetGlobal::RoleData()->TempData.Attr.CalShieldBlockRate(level, 0.0f) * 100.0f;
		lua_pushnumber(L, floor(value[0]));		// 基礎值
		lua_pushnumber(L, floor(value[1]));		// 額外增加
		lua_pushnumber(L, value[2]);			// 同等級發生率
		return 3;
	}
	// 主手物理命中等級
	else if ( strcmp(keyStr, "PHYSICAL_MAIN_HIT") == 0 )
	{
		float value[2];
		value[0] = CNetGlobal::RoleBaseAttr()->Fin.Main_HitRate;
		value[1] = CNetGlobal::RoleData()->TempData.Attr.Fin.Main_HitRate - value[0];
		lua_pushnumber(L, floor(value[0]));
		lua_pushnumber(L, floor(value[1]));
		return 2;
	}
	// 副手物理命中等級
	else if ( strcmp(keyStr, "PHYSICAL_OFF_HIT") == 0 )
	{
		float value[2];
		value[0] = CNetGlobal::RoleBaseAttr()->Fin.Sec_HitRate;
		value[1] = CNetGlobal::RoleData()->TempData.Attr.Fin.Sec_HitRate - value[0];
		lua_pushnumber(L, floor(value[0]));
		lua_pushnumber(L, floor(value[1]));
		return 2;
	}
	else if ( strcmp(keyStr, "PHYSICAL_HIT") == 0 )
	{
		float value[2];
		value[0] = CNetGlobal::RoleBaseAttr()->Fin.HitRate;
		value[1] = CNetGlobal::RoleData()->TempData.Attr.Fin.HitRate - value[0];
		lua_pushnumber(L, floor(value[0]));
		lua_pushnumber(L, floor(value[1]));
		return 2;
	}	
	// 物理命中比率
	else if ( strcmp(keyStr, "PHYSICAL_HIT_RATE") == 0 )
	{
		float value[2];
		value[0] = CNetGlobal::RoleBaseAttr()->Mid.Body[EM_WearEqType_PhyHitRate];
		value[1] = CNetGlobal::RoleData()->TempData.Attr.Mid.Body[EM_WearEqType_PhyHitRate] - value[0];
		lua_pushnumber(L, floor(value[0]));
		lua_pushnumber(L, floor(value[1]));
		return 2;
	}
	// 物理低抗爆擊等級
	else if ( strcmp(keyStr, "PHYSICAL_RESIST_CRITICAL") == 0 )
	{
		float value[2];
		value[0] = CNetGlobal::RoleBaseAttr()->Fin.ResistCriticalHit;
		value[1] = CNetGlobal::RoleData()->TempData.Attr.Fin.ResistCriticalHit - value[0];
		lua_pushnumber(L, floor(value[0]));
		lua_pushnumber(L, floor(value[1]));
		return 2;
	}
	// 物理閃躲等級
	else if ( strcmp(keyStr, "PHYSICAL_DODGE") == 0 )
	{
		float value[2];
		value[0] = CNetGlobal::RoleBaseAttr()->Fin.DodgeRate;
		value[1] = CNetGlobal::RoleData()->TempData.Attr.Fin.DodgeRate - value[0];
		lua_pushnumber(L, floor(value[0]));
		lua_pushnumber(L, floor(value[1]));
		return 2;
	}
	// 法術命中等級
	else if ( strcmp(keyStr, "MAGIC_HIT") == 0 )
	{
		float value[2];
		value[0] = CNetGlobal::RoleBaseAttr()->Fin.MagicHitRate;
		value[1] = CNetGlobal::RoleData()->TempData.Attr.Fin.MagicHitRate - value[0] /*+ CNetGlobal::RoleData()->TempData.Attr.Mid.Body[EM_WearEqType_MagicHitRate]*/;
		lua_pushnumber(L, floor(value[0]));
		lua_pushnumber(L, floor(value[1]));
		return 2;
	}
	// 法術命中等級
	else if ( strcmp(keyStr, "MAGIC_HIT_RATE") == 0 )
	{
		float value[2];
		value[0] = CNetGlobal::RoleBaseAttr()->Mid.Body[EM_WearEqType_MagicHitRate];
		value[1] = CNetGlobal::RoleData()->TempData.Attr.Mid.Body[EM_WearEqType_MagicHitRate] - value[0];
		lua_pushnumber(L, floor(value[0]));
		lua_pushnumber(L, floor(value[1]));
		return 2;
	}
	// 法術低抗爆擊等級
	else if ( strcmp(keyStr, "MAGIC_RESIST_CRITICAL") == 0 )
	{
		float value[2];
		value[0] = CNetGlobal::RoleBaseAttr()->Fin.ResistMagicCriticalHit;
		value[1] = CNetGlobal::RoleData()->TempData.Attr.Fin.ResistMagicCriticalHit - value[0];
		lua_pushnumber(L, floor(value[0]));
		lua_pushnumber(L, floor(value[1]));
		return 2;
	}
	// 法術閃躲等級
	else if ( strcmp(keyStr, "MAGIC_DODGE") == 0 )
	{
		float value[2];
		value[0] = CNetGlobal::RoleBaseAttr()->Fin.MagicDodgeRate;
		value[1] = CNetGlobal::RoleData()->TempData.Attr.Fin.MagicDodgeRate - value[0];
		lua_pushnumber(L, floor(value[0]));
		lua_pushnumber(L, floor(value[1]));
		return 2;
	}
	// 法術防禦
	else if ( strcmp(keyStr, "MAGIC_DEFENCE") == 0 )
	{
		float value[2];
		value[0] = CNetGlobal::RoleBaseAttr()->Fin.MDEF;
		value[1] = CNetGlobal::RoleData()->TempData.Attr.Fin.MDEF - value[0];
		lua_pushnumber(L, floor(value[0]));
		lua_pushnumber(L, floor(value[1]));
		return 2;
	}
	else if ( strcmp(keyStr, "DPS") == 0 )
	{
		float dps[2];
		CNetGlobal::RoleData()->GetPlayerDPS(dps);
		lua_pushnumber(L, dps[0] + dps[1]);
		lua_pushnumber(L, 0);
		return 2;
	}
	return 0;
}

// ----------------------------------------------------------------------------
int	LuaFunc_GetPlayerResistance(lua_State* L)
{
	RoleDataEx* role = CNetGlobal::RoleData();
	lua_pushnumber(L, int(role->TempData.Attr.Fin.Resist.Earth));
	lua_pushnumber(L, int(role->TempData.Attr.Fin.Resist.Water));
	lua_pushnumber(L, int(role->TempData.Attr.Fin.Resist.Fire));
	lua_pushnumber(L, int(role->TempData.Attr.Fin.Resist.Wind));
	lua_pushnumber(L, int(role->TempData.Attr.Fin.Resist.Light));
	lua_pushnumber(L, int(role->TempData.Attr.Fin.Resist.Darkness));
	return 6;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetPlayerGoodEvil(lua_State* L)
{
	lua_pushnumber(L, CNetGlobal::RoleData()->PlayerBaseData->GoodEvil);
	lua_pushnumber(L, CNetGlobal::RoleData()->GetGoodEvilType());
	return 2;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetPlayerHonorPoint(lua_State* L)
{
	lua_pushnumber(L, CNetGlobal::RoleData()->PlayerBaseData->Honor);
	return 1;
}
// ----------------------------------------------------------------------------
int LuaFunc_GetPlayerDuelistRewardPoint(lua_State* L)
{
	lua_pushnumber(L, CNetGlobal::RoleData()->PlayerBaseData->DuelistReward);
	return 1;
}

int LuaFunc_GetPlayerArenaPoint(lua_State* L)
{
	lua_pushnumber(L, 0);
	return 1;
}
int LuaFunc_GetPlayerPointInfo(lua_State* L)
{
	int g = luaL_checkint(L, 1)-1;	
	int i = luaL_checkint(L, 2)-1;	
	const char *tips=g_ObjectData->GetString(luaL_checkstring(L, 3));
	int smax=0;
	int score=0;

	PlayerBaseRoleData*playerbase=CNetGlobal::RoleData()->PlayerBaseData;
	/*
	int					TrialBadgeCoin;			//試煉徽章
	int					RelicsCoin;				//古代遺物
	int					DreamlandCoin;			//夢境先遣印記
	int					PhiriusShellCoin;		//必爾汀圓貝
	int					EnergyJusticeCoin;		//正義能量
	int					ProofLegendCoin;		//傳說之證
	*/
	
	switch (g)
	{
	case 0:
		if (i==0)
		{
			
			score=playerbase->PhiriusShellCoin;
			
			int tmax=_MAX_PhiriusShell_COIN_COUNT_;//g_ObjectData->GetSysKeyValue("PhiriusShellCoinMax");
			if (tmax>0)			
				smax=tmax;
			
		}
		else if (i==1)
		{
			score=playerbase->EnergyJusticeCoin;
			int tmax= _MAX_EnergyJusticeCoin_COUNT_;//g_ObjectData->GetSysKeyValue("RelicsCoinMax");
			if (tmax>0)
				smax=tmax;
		}	
		else if (i==2)
		{


			score=playerbase->DreamlandCoin;
		}	

		break;
	case 1:
		if (i==0)
		{
			score=playerbase->RelicsCoin;
			int tmax= _MAX_Relics_COIN_COUNT_;//g_ObjectData->GetSysKeyValue("RelicsCoinMax");
			if (tmax>0)
				smax=tmax;
		}
		else if (i==1)
		{


			score=playerbase->ProofLegendCoin;
		}	
		else if (i==2)
		{


			score=playerbase->MirrorCoin;
		}	


		break;

	case 2:
		if (i==0)
		{
			score=(int)playerbase->Honor;
			int tmax= _MAX_Honor_COIN_COUNT_;//g_ObjectData->GetSysKeyValue("RelicsCoinMax");
			if (tmax>0)
				smax=tmax;
			
		}
		else if (i==1)
		{
			score=playerbase->TrialBadgeCoin;
		}	
		else if (i==2)
		{
			int tmax= _MAX_DuelistReward_COIN_COUNT_;//g_ObjectData->GetSysKeyValue("RelicsCoinMax");
			if (tmax>0)
				smax=tmax;;
			score=playerbase->DuelistReward;
		}	

		break;

	}
	lua_pushnumber(L, score);
	lua_pushnumber(L, smax);

	//char temp[512];
	//strcpy(temp,tips);
	//if (smax!=-1)
	//{
	//	SwapStringA(temp, tips, smax);
	//}
	lua_pushstring(L, tips);
	return 3;
}


int LuaFunc_GetPlayerMedalCount(lua_State* L)
{
	lua_pushnumber(L, CNetGlobal::RoleData()->PlayerBaseData->MedalCount);
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetGoodEvilTypeColor(lua_State* L)
{
	GoodEvilTypeENUM type = (GoodEvilTypeENUM)luaL_checkint(L, 1);
	DWORD color = g_ObjectData->GetGoodEvilColor(type);
	lua_pushnumber(L, ((color & 0x00FF0000)>>16) / 256.0f);
	lua_pushnumber(L, ((color & 0x0000FF00)>>8) / 256.0f);
	lua_pushnumber(L, (color & 0x0000000FF) / 256.0f);
	return 3;
}

// ----------------------------------------------------------------------------
int	LuaFunc_PickupEquipmentItem(lua_State* L)
{	
	g_pCharacterFrame->Paste(luaL_checkint(L, 1));
	return 0;
}

// ----------------------------------------------------------------------------
int	LuaFunc_UseEquipmentItem(lua_State* L)
{	
	g_pCharacterFrame->Use(luaL_checkint(L, 1));
	return 0;
}
// ----------------------------------------------------------------------------
int LuaFunc_GetPlayerIntroduce(lua_State* L)
{
	const char* note = CNetGlobal::RoleData()->PlayerSelfData->FriendList.Note.Begin();
	g_pGameMain->MatchPatternString((BYTE*)note);
	lua_pushstring( L, note );
	return 1;
}
// ----------------------------------------------------------------------------
int LuaFunc_SetPlayerIntroduce(lua_State* L)
{
	CNetGlobal::RoleData()->PlayerSelfData->FriendList.Note = (char*)luaL_checkstring(L,1);
	NetCli_FriendList::SL_ModifyNote( (char*)CNetGlobal::RoleData()->PlayerSelfData->FriendList.Note.Begin() );
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_EquipPendingItem(lua_State* L)
{
	g_pCharacterFrame->EquipPendingItem(luaL_checkint(L, 1));
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_CancelPendingItem(lua_State* L)
{
	g_pCharacterFrame->CancelPendingItem(luaL_checkint(L, 1));
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_EquipItem(lua_State* L)
{
	int equipIndex = luaL_checkint(L, 1) - 1;
	int destPos = -1;

	if ( lua_isnumber(L, 2) == 0 )
	{
		// 搜尋背包空位
		destPos = CNetGlobal::RoleData()->SearchBodyFirstFit(NULL, false);		// 找尋一個可以插入背包的位置
		if ( destPos == -1 )
		{
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("WARRING_BACKPACK_FULL_CHANG_WEAP"));
			return 0;
		}
	}
	else
		destPos = (int)(lua_tonumber(L, 2) - 1);

	g_pCharacterFrame->Equip(equipIndex, 0, destPos);
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetEquipItemCooldown(lua_State* L)
{
	float cooldown[2] = { 0.0f, 0.0f };

	if ( lua_isnumber(L, 1) )
	{
		ItemFieldStruct* item = CNetGlobal::RoleData()->GetEqItem((int)lua_tonumber(L, 1));
		if ( item )
			CNetGlobal::RoleData()->GetObjectCooldown(item->OrgObjID, cooldown);
	}
	lua_pushnumber(L, cooldown[0]);
	lua_pushnumber(L, cooldown[1]);
	return 2;
}

// ----------------------------------------------------------------------------
// 裝備部位顯示狀態
int LuaFunc_GetEquipItemShown(lua_State* L)
{
	bool visible = false;


	int suitID = 0;
	int clothID = luaL_checkint(L, 1);

	if( suitID < 0 || suitID >= _DEF_MAX_COOL_CLOTH_COUNT_ )
	{
		return 0;
	}

	PlayerBaseRoleData* PlayerBaseData = CNetGlobal::RoleData()->PlayerBaseData;
	CoolSuitBase* pCoolSuitBase = &PlayerBaseData->CoolSuitList[suitID];

	CoolClothBase* pCoolClothBase = NULL;
	bool show = false;
	switch( clothID )
	{
	case EM_EQWearPos_Head:
		pCoolClothBase = &pCoolSuitBase->Head; 
		show = pCoolSuitBase->ShowEQ.Head;
		break;

	case EM_EQWearPos_Gloves:
		pCoolClothBase = &pCoolSuitBase->Gloves; 
		show = pCoolSuitBase->ShowEQ.Gloves;			
		break;

	case EM_EQWearPos_Shoes:
		pCoolClothBase = &pCoolSuitBase->Shoes; 
		show = pCoolSuitBase->ShowEQ.Shoes;					
		break;

	case EM_EQWearPos_Clothes:
		pCoolClothBase = &pCoolSuitBase->Clothes; 
		show = pCoolSuitBase->ShowEQ.Clothes;				
		break;

	case EM_EQWearPos_Pants:
		pCoolClothBase = &pCoolSuitBase->Pants; 
		show = pCoolSuitBase->ShowEQ.Pants;	
		break;

	case EM_EQWearPos_Back:
		pCoolClothBase = &pCoolSuitBase->Back; 
		show = pCoolSuitBase->ShowEQ.Back;
		break;

	case EM_EQWearPos_Belt:
		pCoolClothBase = &pCoolSuitBase->Belt; 
		show = pCoolSuitBase->ShowEQ.Belt;	
		break;

	case EM_EQWearPos_Shoulder:
		pCoolClothBase = &pCoolSuitBase->Shoulder; 
		show = pCoolSuitBase->ShowEQ.Shoulder;
		break;

	case EM_EQWearPos_Ornament:	
		pCoolClothBase = &pCoolSuitBase->Ornament; 
		show = pCoolSuitBase->ShowEQ.Ornament;
		break;

	default:
		return 0;

	}

	lua_pushboolean( L, show );
	return 1;


	/*
	switch (luaL_checkint(L, 1))
	{	
	case EM_EQWearPos_Head:			//0
		visible = CNetGlobal::RoleData()->PlayerBaseData->ShowEQ.Head;
		break;
		
	case EM_EQWearPos_Gloves:		//1
		visible = CNetGlobal::RoleData()->PlayerBaseData->ShowEQ.Gloves;
		break;

	case EM_EQWearPos_Shoes:		//2
		visible = CNetGlobal::RoleData()->PlayerBaseData->ShowEQ.Shoes;
		break;

	case EM_EQWearPos_Clothes:		//3
		visible = CNetGlobal::RoleData()->PlayerBaseData->ShowEQ.Clothes;
		break;

	case EM_EQWearPos_Pants:		//4
		visible = CNetGlobal::RoleData()->PlayerBaseData->ShowEQ.Pants;
		break;

	case EM_EQWearPos_Back:			//5
		visible = CNetGlobal::RoleData()->PlayerBaseData->ShowEQ.Back;
		break;

	case EM_EQWearPos_Belt:			//6
		visible = CNetGlobal::RoleData()->PlayerBaseData->ShowEQ.Belt;
		break;

	case EM_EQWearPos_Shoulder:		//7
		visible = CNetGlobal::RoleData()->PlayerBaseData->ShowEQ.Shoulder;
		break;

	case EM_EQWearPos_Necklace:		//8
		visible = CNetGlobal::RoleData()->PlayerBaseData->ShowEQ.Necklace;
		break;

	case EM_EQWearPos_Ammo:			//9
		visible = CNetGlobal::RoleData()->PlayerBaseData->ShowEQ.Ammo;
		break;

	case EM_EQWearPos_Bow:			//10
		visible = CNetGlobal::RoleData()->PlayerBaseData->ShowEQ.Bow;
		break;

	case EM_EQWearPos_Ring1:		//11
		visible = CNetGlobal::RoleData()->PlayerBaseData->ShowEQ.Ring1;
		break;

	case EM_EQWearPos_Ring2:		//12
		visible = CNetGlobal::RoleData()->PlayerBaseData->ShowEQ.Ring2;
		break;

	case EM_EQWearPos_Earring1:		//13
		visible = CNetGlobal::RoleData()->PlayerBaseData->ShowEQ.Earring1;
		break;

	case EM_EQWearPos_Earring2:		//14
		visible = CNetGlobal::RoleData()->PlayerBaseData->ShowEQ.Earring2;
		break;

	case EM_EQWearPos_MainHand:		//15
		visible = CNetGlobal::RoleData()->PlayerBaseData->ShowEQ.MainHand;
		break;

	case EM_EQWearPos_SecondHand:	//16
		visible = CNetGlobal::RoleData()->PlayerBaseData->ShowEQ.SecondHand;
		break;

	case EM_EQWearPos_Manufactory:	//17
		visible = CNetGlobal::RoleData()->PlayerBaseData->ShowEQ.Manufacture;
		break;

	case EM_EQWearPos_MagicTool1:	//18
		visible = CNetGlobal::RoleData()->PlayerBaseData->ShowEQ.MagicTool1;
		break;

	case EM_EQWearPos_MagicTool2:	//19
		visible = CNetGlobal::RoleData()->PlayerBaseData->ShowEQ.MagicTool2;
		break;

	case EM_EQWearPos_MagicTool3:	//20
		visible = CNetGlobal::RoleData()->PlayerBaseData->ShowEQ.MagicTool3;
		break;

	case EM_EQWearPos_Ornament:		//21
		visible = CNetGlobal::RoleData()->PlayerBaseData->ShowEQ.Ornament;
		break;
	}
	lua_pushboolean(L, visible);
	return 1;
	*/

}

// ----------------------------------------------------------------------------
int LuaFunc_SetEquipItemShown(lua_State* L)
{

	bool visible = false;


	int suitID = 0;
	int clothID = luaL_checkint(L, 1);
	bool show = (lua_toboolean(L, 2)) ? true : false;

	if( suitID < 0 || suitID >= _DEF_MAX_COOL_CLOTH_COUNT_ )
	{
		return 0;
	}

	PlayerBaseRoleData* PlayerBaseData = CNetGlobal::RoleData()->PlayerBaseData;
	CoolSuitBase* pCoolSuitBase = &PlayerBaseData->CoolSuitList[suitID];

	switch( clothID )
	{
	case EM_EQWearPos_Head:
		pCoolSuitBase->ShowEQ.Head = show;
		break;

	case EM_EQWearPos_Gloves:
		pCoolSuitBase->ShowEQ.Gloves = show;			
		break;

	case EM_EQWearPos_Shoes:
		pCoolSuitBase->ShowEQ.Shoes = show;					
		break;

	case EM_EQWearPos_Clothes:
		pCoolSuitBase->ShowEQ.Clothes = show;				
		break;

	case EM_EQWearPos_Pants:
		pCoolSuitBase->ShowEQ.Pants = show;	
		break;

	case EM_EQWearPos_Back:
		pCoolSuitBase->ShowEQ.Back = show;
		break;

	case EM_EQWearPos_Belt:
		pCoolSuitBase->ShowEQ.Belt = show;	
		break;

	case EM_EQWearPos_Shoulder:
		pCoolSuitBase->ShowEQ.Shoulder = show;
		break;

	case EM_EQWearPos_Ornament:	
		pCoolSuitBase->ShowEQ.Ornament = show;
		break;

	default:
		return 0;

	}

	NetCli_Item::SL_SetCoolSuitRequest( suitID , *pCoolSuitBase );	

	return 0;



	/*
	ShowEQStruct oldShowEQ = CNetGlobal::RoleData()->PlayerBaseData->ShowEQ;
	ShowEQStruct& ShowEQ = CNetGlobal::RoleData()->PlayerBaseData->ShowEQ;
	switch (luaL_checkint(L, 1))
	{	
	case EM_EQWearPos_Head:			//0
		ShowEQ.Head = (lua_toboolean(L, 2)) ? true : false;
		break;

	case EM_EQWearPos_Gloves:		//1
		ShowEQ.Gloves = (lua_toboolean(L, 2)) ? true : false;
		break;

	case EM_EQWearPos_Shoes:		//2
		ShowEQ.Shoes = (lua_toboolean(L, 2)) ? true : false;
		break;

	case EM_EQWearPos_Clothes:		//3
		ShowEQ.Clothes = (lua_toboolean(L, 2)) ? true : false;
		break;

	case EM_EQWearPos_Pants:		//4
		ShowEQ.Pants = (lua_toboolean(L, 2)) ? true : false;
		break;

	case EM_EQWearPos_Back:			//5
		ShowEQ.Back = (lua_toboolean(L, 2)) ? true : false;
		break;

	case EM_EQWearPos_Belt:			//6
		ShowEQ.Belt = (lua_toboolean(L, 2)) ? true : false;
		break;

	case EM_EQWearPos_Shoulder:		//7
		ShowEQ.Shoulder = (lua_toboolean(L, 2)) ? true : false;
		break;

	case EM_EQWearPos_Necklace:		//8
		ShowEQ.Necklace = (lua_toboolean(L, 2)) ? true : false;
		break;

	case EM_EQWearPos_Ammo:			//9
		ShowEQ.Ammo = (lua_toboolean(L, 2)) ? true : false;
		break;

	case EM_EQWearPos_Bow:			//10
		ShowEQ.Bow = (lua_toboolean(L, 2)) ? true : false;
		break;

	case EM_EQWearPos_Ring1:		//11
		ShowEQ.Ring1 = (lua_toboolean(L, 2)) ? true : false;
		break;

	case EM_EQWearPos_Ring2:		//12
		ShowEQ.Ring2 = (lua_toboolean(L, 2)) ? true : false;
		break;

	case EM_EQWearPos_Earring1:		//13
		ShowEQ.Earring1 = (lua_toboolean(L, 2)) ? true : false;
		break;

	case EM_EQWearPos_Earring2:		//14
		ShowEQ.Earring2 = (lua_toboolean(L, 2)) ? true : false;
		break;

	case EM_EQWearPos_MainHand:		//15
		ShowEQ.MainHand = (lua_toboolean(L, 2)) ? true : false;
		break;

	case EM_EQWearPos_SecondHand:	//16
		ShowEQ.SecondHand = (lua_toboolean(L, 2)) ? true : false;
		break;

	case EM_EQWearPos_Manufactory:	//17
		ShowEQ.Manufacture = (lua_toboolean(L, 2)) ? true : false;
		break;

	case EM_EQWearPos_MagicTool1:	//18
		ShowEQ.MagicTool1 = (lua_toboolean(L, 2)) ? true : false;
		break;

	case EM_EQWearPos_MagicTool2:	//19
		ShowEQ.MagicTool2 = (lua_toboolean(L, 2)) ? true : false;
		break;

	case EM_EQWearPos_MagicTool3:	//20
		ShowEQ.MagicTool3 = (lua_toboolean(L, 2)) ? true : false;
		break;

	case EM_EQWearPos_Ornament:		//21
		ShowEQ.Ornament = (lua_toboolean(L, 2)) ? true : false;
		break;

	default:
		return 0;
	}

	// 沒有改變
	if ( oldShowEQ._Value == ShowEQ._Value )
		return 0;

	// 送出封包
	NetCli_EQ::S_SetShowEQ(ShowEQ);

	return 0;
	*/
}

// ----------------------------------------------------------------------------
int LuaFunc_HasSecondEquipment(lua_State* L)
{
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_SwapEquipmentItem(lua_State* L)
{
	int slot=0;
	if (lua_isnumber(L,1))
	{
		slot=(int)lua_tonumber(L, 1);
	}
	
	g_pCharacterFrame->SwapEquip(slot);
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetEuipmentNumber(lua_State* L)
{
	int slotID = CNetGlobal::RoleData()->PlayerBaseData->ClientData[255];
	if ( slotID<0&&slotID>5)
		slotID = 0;

	lua_pushnumber(L, slotID + 1);
	return 1;
}


// ----------------------------------------------------------------------------
int LuaFunc_CoolClothBag_GetItemMax(lua_State* L)
{

	int maxCount=100;
	char temp[128];
	for (int i=3;i<=10;i++)
	{

		sprintf(temp,"Wardrobe_ItemPage%d",i);
		int KeyItem=g_ObjectData->GetSysKeyValue(temp);
		bool bResult =true;
		if (KeyItem>0)		
			bResult=CNetGlobal::RoleData()->Base.AccountFlag.GetFlag( KeyItem ) ;	
		if (bResult)
			maxCount+=50;


	}




	



	lua_pushnumber( L, maxCount);
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_CoolClothBag_GetItemCount(lua_State* L)
{
	int count = 0;
	PlayerBaseRoleData* PlayerBaseData = CNetGlobal::RoleData()->PlayerBaseData;
	for( int i = 0 ; i < _DEF_MAX_COOL_CLOTH_COUNT_ ; ++i )
	{
		if( PlayerBaseData->CoolClothList[i] > 0 )
		{
			++count;
		}
	}
	lua_pushnumber( L, count );
	return 1;
}


// ----------------------------------------------------------------------------
int LuaFunc_CoolClothBag_GetItem(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;

	if( index < 0 || index >= _DEF_MAX_COOL_CLOTH_COUNT_ )
	{
		return 0;
	}

	PlayerBaseRoleData* PlayerBaseData = CNetGlobal::RoleData()->PlayerBaseData;
	int objID = PlayerBaseData->CoolClothList[index];

	GameObjDbStructEx* itemDB = CNetGlobal::GetObj( objID );
	if( NULL == itemDB )
	{
		return 0;
	}

	bool locked = false;
	if( g_pItemClipboard->GetType() == eClipboardItem_Type_CoolClothBag &&
		g_pItemClipboard->GetItemInfo().CoolClothBag.index == index  )
	{
		locked = true;	
	}

	int quality = g_ObjectData->GetItemQuality( itemDB );

	string strItemLink;
	if( false == ItemGUID_To_ItemLink( objID, strItemLink ) )
	{
		return 0;
	}

	int eqPos = EM_EQWearPos_Head;

	switch( itemDB->Item.ArmorPos )
	{
	case EM_ArmorPos_Head       : eqPos = EM_EQWearPos_Head; break;
	case EM_ArmorPos_Gloves     : eqPos = EM_EQWearPos_Gloves;  break;
	case EM_ArmorPos_Shoes      : eqPos = EM_EQWearPos_Shoes;  break;
	case EM_ArmorPos_Clothes    : eqPos = EM_EQWearPos_Clothes;  break;
	case EM_ArmorPos_Pants      : eqPos = EM_EQWearPos_Pants;  break;
	case EM_ArmorPos_Back       : eqPos = EM_EQWearPos_Back;  break;
	case EM_ArmorPos_Belt       : eqPos = EM_EQWearPos_Belt;  break;
	case EM_ArmorPos_Shoulder   : eqPos = EM_EQWearPos_Shoulder;  break;
	case EM_ArmorPos_Ornament   : eqPos = EM_EQWearPos_Ornament;  break;
	default:
		return 0;
	}

	{
		SexFlagStruct sex = itemDB->Limit.Sex;


		GameObjDbStructEx* imageDB = CNetGlobal::GetObj(itemDB->ImageID);
		if( imageDB )
		{
			SexFlagStruct& LimitSex = imageDB->Image.LimitSex;
			if( LimitSex._Sex != 0 && LimitSex._Sex != -1 )
			{
				sex.Boy &= LimitSex.Boy;
				sex.Girl &= LimitSex.Girl;
			}
 		}

		if( CNetGlobal::RoleData()->BaseData.Sex == EM_Sex_Boy && false == sex.Boy )
		{
			eqPos = -1;
		}
		else if( CNetGlobal::RoleData()->BaseData.Sex == EM_Sex_Girl && false == sex.Girl )
		{
			eqPos = -1;
		}
	}


	lua_pushnumber( L, objID );
	
	lua_pushstring( L, ItemUTIL::GetItemIcon( itemDB, true ) );  // push item texture file

	lua_pushnumber(L, quality);			// push item quality


	//lua_pushstring( L, itemDB->GetName() );						 // push Item name
	lua_pushboolean( L, locked );	
  	lua_pushstring( L, strItemLink.c_str() );	

  	lua_pushnumber( L, eqPos + 1 );	

	return 6;

}


// ----------------------------------------------------------------------------
int LuaFunc_CoolClothBag_Pickup(lua_State* L)
{
	//NetCli_Item::SL_PutCoolClothRequest( int BodyPos );
	int index = luaL_checkint(L, 1) - 1;

	if( index < 0 || index >= _DEF_MAX_COOL_CLOTH_COUNT_ )
	{
		return 0;
	}

	PlayerBaseRoleData* PlayerBaseData = CNetGlobal::RoleData()->PlayerBaseData;
	int clothDBID = PlayerBaseData->CoolClothList[index];

	//ItemFieldStruct* destItem = CNetGlobal::RoleData()->GetEqItem(index);
	ClipboardItemInfo itemInfo = g_pItemClipboard->GetItemInfo();

	// 物品資料錯誤或不存在
	//if ( destItem == NULL || destItem->Pos != EM_ItemState_NONE )
	//	return 0;

//	if( g_pItemClipboard->GetType() != eClipboardItem_Type_Bag )
//	{
//		g_pItemClipboard->Clear();
//		return 0;
//	}


	switch( g_pItemClipboard->GetType() )
	{	
	case eClipboardItem_Type_None:				// 拿取物品
		{
			GameObjDbStruct* itemDB = CNetGlobal::GetObj( clothDBID );
			if ( itemDB )
			{
				g_pItemClipboard->SetFile( ItemUTIL::GetItemIcon( itemDB, true ) );
				g_pItemClipboard->PickupCoolClothBag( index );
				//m_pickupItemIndex = index;
			}
		}
		return 0;
	case eClipboardItem_Type_Bag:				// 背包 -> 酷裝包
		{
			//包包不是空的
			if( clothDBID > 0 )
			{
				return 0;
			}


			int pos = g_pItemClipboard->GetItemInfo().bag.pos;
			g_pItemClipboard->Clear();
			ItemFieldStruct* pItemField = CNetGlobal::RoleData()->GetBodyItem( pos );	

			GameObjDbStruct* pItemDB = CNetGlobal::GetObj( pItemField->ImageObjectID );
			if( pItemDB == NULL )
			{
				return 0;
			}

			if( pItemDB->Classification != EM_ObjectClass_Item )
			{
				g_pGameMain->SendWarningMsg( g_ObjectData->GetString( "SYS_COOLCLOTHBAG_PICKUP_FAIL1" ) );
				return 0;
			}
			//防具
			if( pItemDB->Item.Class != EM_Item_Armor )
			{
				g_pGameMain->SendWarningMsg( g_ObjectData->GetString( "SYS_COOLCLOTHBAG_PICKUP_FAIL1" ) );
				return 0;
			}
			//副手
			if( pItemDB->Item.ArmorPos == EM_ArmorPos_SecondHand || 
				pItemDB->Item.ArmorPos ==EM_ArmorPos_Earring ||
				pItemDB->Item.ArmorPos ==EM_ArmorPos_MagicTool ||
				pItemDB->Item.ArmorPos ==EM_ArmorPos_Necklace ||
				pItemDB->Item.ArmorPos ==EM_ArmorPos_Ring /*||
				pItemDB->Item.ArmorPos == EM_ArmorPos_Ornament  */ )
			{
				g_pGameMain->SendWarningMsg( g_ObjectData->GetString( "SYS_COOLCLOTHBAG_PICKUP_FAIL1" ) );
				return 0;
			}

			//已有這件裝備
			PlayerBaseRoleData* PlayerBaseData = CNetGlobal::RoleData()->PlayerBaseData;
			for( int i = 0 ; i < _DEF_MAX_COOL_CLOTH_COUNT_ ; ++i )
			{
				if( PlayerBaseData->CoolClothList[i] == pItemField->ImageObjectID )
				{
					g_pGameMain->SendWarningMsg( g_ObjectData->GetString( "SYS_COOLCLOTHBAG_PICKUP_FAIL2" ) );
					return 0;
				}
			}

			{
				bool bUseItem=false;
				bool bHaveMarge=false;
				bool bHavePowerUp=false;



				//有融合
				if( pItemField->OrgObjID != pItemField->ImageObjectID )
				{
					//g_pCharacterFrame->m_CoolClothBag_Pickup_Pending_Pos = pos;
					//g_pCharacterFrame->m_CoolClothBag_Pickup_Pending_Index = index;
					//g_pCharacterFrame->m_CoolClothBag_Pickup_Pending_ObjectID = pItemField->ImageObjectID;

					{
						bHaveMarge=true;//g_pInterface->SendWorldEvent( "COOLCLOTHBAG_PENDING1_CONFIRM" );
					}
					//return 0;
				}
				//有能力
				if( pItemField->Level > 0 || ( pItemField->_Rune[0] > 0 || pItemField->RuneVolume > 0 ) ||
					pItemField->_Inherent[0] != 0 ||
					pItemField->_Inherent[1] != 0 ||
					pItemField->_Inherent[2] != 0 ||
					pItemField->_Inherent[3] != 0 ||
					pItemField->_Inherent[4] != 0 ||
					pItemField->_Inherent[5] != 0 )
				{
					//g_pCharacterFrame->m_CoolClothBag_Pickup_Pending_Pos = pos;
					//g_pCharacterFrame->m_CoolClothBag_Pickup_Pending_Index = index;
					//g_pCharacterFrame->m_CoolClothBag_Pickup_Pending_ObjectID = pItemField->ImageObjectID;

					//	g_pInterface->SendWorldEvent( "COOLCLOTHBAG_PENDING2_CONFIRM" );
					bHavePowerUp=true;
					//return 0;
				}

				//酷裝或是一般裝備
				int Wardrobe_PutInItemID=0;
				if( !pItemDB->Mode.SpecialSuit )
				{
					Wardrobe_PutInItemID=g_ObjectData->GetSysKeyValue("Wardrobe_PutInItemID");
					if (Wardrobe_PutInItemID>0)
					{
						bUseItem=true;
					}
				}

				if (bUseItem||bHavePowerUp||bHaveMarge)
				{
					string outmsg;

					string temp=g_ObjectData->GetString("COOLCLOTHBAG_PENDING_CONFIRM");

					g_pCharacterFrame->m_CoolClothBag_Pickup_Pending_Pos = pos;
					g_pCharacterFrame->m_CoolClothBag_Pickup_Pending_Index = index;
					g_pCharacterFrame->m_CoolClothBag_Pickup_Pending_ObjectID = pItemField->ImageObjectID;

					string pMsg1;
					string pMsg2;
					string temp1;
					if (bHaveMarge)				
						pMsg1=g_ObjectData->GetString("COOLCLOTHBAG_MSG1");		

					if (bHavePowerUp)
						pMsg2=g_ObjectData->GetString("COOLCLOTHBAG_MSG2");		

					bool wanring=false;
					if (bUseItem)
					{

						if (g_pBagFrame->GetIndex(Wardrobe_PutInItemID)!=-1)
							temp1=g_ObjectData->GetString("COOLCLOTHBAG_MSG3");		
						else
						{
							temp1=g_ObjectData->GetString("COOLCLOTHBAG_NOITEM");		
							wanring=true;

						}

						//Wardrobe_PutInItemID
						GameObjDbStruct* pWardrobeItemDB = 	g_ObjectData->GetObj(Wardrobe_PutInItemID);
						temp1=ReplaceKeyString(temp1,"<item>",pWardrobeItemDB->Name);
						if (wanring)
						{
							lua_pushstring(L, temp1.c_str());
							lua_setglobal(L, "arg1");	
							g_pInterface->SendWorldEvent( "COOLCLOTHBAG_PENDING1_CONFIRM" );
							g_pGameMain->SendWarningMsg( temp1.c_str() );

							return 0;
						}
					}

					temp=ReplaceKeyString(temp,"<msg1>",pMsg1);		
					temp=ReplaceKeyString(temp,"<msg2>",pMsg2);		
					temp=ReplaceKeyString(temp,"<msg3>",temp1);		



					lua_pushstring(L, temp.c_str());
					lua_setglobal(L, "arg1");				

					g_pInterface->SendWorldEvent( "COOLCLOTHBAG_PENDING3_CONFIRM" );

					return 0;





				}		
			}
			
			
			NetCli_ItemChild::PutCoolClothRequest( pos, index, pItemField->ImageObjectID );
		}
  		break;
	case eClipboardItem_Type_CoolClothBag:				// 酷裝包 -> 酷裝包
		{
			int sourceIndex = g_pItemClipboard->GetItemInfo().CoolClothBag.index;

			if( sourceIndex == index )
			{
				break;
			}

			NetCli_ItemChild::SwapCoolClothRequest( sourceIndex, index );
		}
		break;
	}

	g_pItemClipboard->Clear();


	// 存在物品對物品使用中
	//	int useItem = g_pBagFrame->GetUseItemIndex();
	//	if ( useItem != -1 )
	//	{
	//		if ( !g_pBagFrame->UseItemToItem(1, index) )
	//			g_pBagFrame->UseItemCancel();
	//		return 0;
	//	}

	//int BodyPos = itemInfo.bag.pos;
	g_pItemClipboard->Clear();

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_CoolClothBag_Equip(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;

	if( index < 0 || index >= _DEF_MAX_COOL_CLOTH_COUNT_ )
	{
		return 0;
	}

	PlayerBaseRoleData* PlayerBaseData = CNetGlobal::RoleData()->PlayerBaseData;
	int clothDBID = PlayerBaseData->CoolClothList[index];

	GameObjDbStruct* itemDB = CNetGlobal::GetObj( clothDBID );
	if ( NULL == itemDB )
	{
		return 0;
	}

	if( itemDB->Classification != EM_ObjectClass_Item || itemDB->Item.Class != EM_Item_Armor )
	{
		return 0;
	}

	int eqWearPos = -1;

	switch (itemDB->Item.ArmorPos)
	{
	case EM_ArmorPos_Head:
		eqWearPos = EM_EQWearPos_Head;
		break;
	case EM_ArmorPos_Gloves:
		eqWearPos = EM_EQWearPos_Gloves;
		break;
	case EM_ArmorPos_Shoes:
		eqWearPos = EM_EQWearPos_Shoes;
		break;
	case EM_ArmorPos_Clothes:
		eqWearPos = EM_EQWearPos_Clothes;
		break;
	case EM_ArmorPos_Pants:
		eqWearPos = EM_EQWearPos_Pants;
		break;
	case EM_ArmorPos_Back:
		eqWearPos = EM_EQWearPos_Back;
		break;
	case EM_ArmorPos_Belt:
		eqWearPos = EM_EQWearPos_Belt;
		break;
	case EM_ArmorPos_Shoulder:
		eqWearPos = EM_EQWearPos_Shoulder;
		break;
	case EM_ArmorPos_Necklace:
		eqWearPos = EM_EQWearPos_Necklace;
		break;
	case EM_ArmorPos_Ornament:
		eqWearPos = EM_EQWearPos_Ornament;
		break;
	default:
		return 0;
	}

	Func_CoolSuit_SetItemObjID( eqWearPos , clothDBID );	

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_CoolClothBag_Delele(lua_State* L)
{
	int index = luaL_checkint(L, 1);

	if( index < 0 || index >= _DEF_MAX_COOL_CLOTH_COUNT_ )
	{
		return 0;
	}

	PlayerBaseRoleData* PlayerBaseData = CNetGlobal::RoleData()->PlayerBaseData;
	int ImageObjectID = PlayerBaseData->CoolClothList[index];
  	NetCli_ItemChild::DelCoolClothRequest ( index , ImageObjectID );

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_CoolClothBag_PushPendingItem(lua_State* L)
{
	NetCli_ItemChild::PutCoolClothRequest( 
		g_pCharacterFrame->m_CoolClothBag_Pickup_Pending_Pos, 
		g_pCharacterFrame->m_CoolClothBag_Pickup_Pending_Index, 
		g_pCharacterFrame->m_CoolClothBag_Pickup_Pending_ObjectID );

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_CoolClothBag_CancelPendingItem(lua_State* L)
{
	g_pCharacterFrame->m_CoolClothBag_Pickup_Pending_Pos = -1;
	g_pCharacterFrame->m_CoolClothBag_Pickup_Pending_Index = 0;
	g_pCharacterFrame->m_CoolClothBag_Pickup_Pending_ObjectID = 0;

	return 0;
}
// ----------------------------------------------------------------------------
int LuaFunc_CoolSuit_GetPageID(lua_State* L)
{


	PlayerBaseRoleData* PlayerBaseData = CNetGlobal::RoleData()->PlayerBaseData;

	lua_pushnumber( L, PlayerBaseData->CoolSuitID + 1 );
	return 1;
}
// ----------------------------------------------------------------------------

int LuaFunc_CoolClothBag_GetPageTabEnable(lua_State* L)
{
	//int count=2;
	char temp[512];

	int index = luaL_checkint(L, 1) ;

	sprintf(temp,"Wardrobe_ItemPage%d",index);
	int KeyItem=g_ObjectData->GetSysKeyValue(temp);
	bool bResult =true;

	if (KeyItem>0)		
	 	bResult=CNetGlobal::RoleData()->Base.AccountFlag.GetFlag( KeyItem );	
	
	lua_pushboolean( L,bResult );
	return 1;
}

int LuaFunc_CoolSuit_GetPageTexture(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	if( index < 0 || index >= _DEF_MAX_COOL_SUIT_COUNT_ )
	{
		return 0;
	}

	string texture = g_pCharacterFrame->GetCoolSuitPageTexture( index );
  
	lua_pushstring( L, texture.c_str() );
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_CoolSuit_SetPageID(lua_State* L)
{
	int suitID = luaL_checkint(L, 1) - 1;

	g_pCharacterFrame->SetCoolSuitIndex( suitID );

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_CoolSuit_Pickup(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;

	if( index < 0 || index >= _DEF_MAX_COOL_SUIT_COUNT_ )
	{
		return 0;
	}

	PlayerBaseRoleData* PlayerBaseData = CNetGlobal::RoleData()->PlayerBaseData;
	int clothDBID = PlayerBaseData->CoolClothList[index];

	switch( g_pItemClipboard->GetType() )
	{	
	case eClipboardItem_Type_None:				// 拿取物品
		{
			g_pCharacterFrame->PickupCoolSuit( index );
		}
		break;
	default:
		g_pItemClipboard->Clear();
	}
	

	return 0;
}

int Func_CoolSuit_SetItemObjID( int equipIndex, int objectID )
{
	PlayerBaseRoleData* PlayerBaseData = CNetGlobal::RoleData()->PlayerBaseData;
	int suitID = PlayerBaseData->CoolSuitID;

	if( suitID < 0 || suitID >= _DEF_MAX_COOL_SUIT_COUNT_ )
	{
		return 0;
	}

	if( false == g_pCharacterFrame->m_bCoolSuitSetuping )
	{
		g_pCharacterFrame->m_CoolSuitSetupTemp = PlayerBaseData->CoolSuitList[suitID];

		g_pCharacterFrame->m_bCoolSuitSetuping = true;
	}

	CoolSuitBase* pCoolSuitBase = &g_pCharacterFrame->m_CoolSuitSetupTemp;
	CoolClothBase* pCoolClothBase = NULL;
	bool bShow = false;

	if( objectID == -1 )
	{
		bShow = false;
		objectID = 0;
	}
	else
	{
		bShow = true;

		bool bFind = false;
		for( int i = 0 ; i < _DEF_MAX_COOL_CLOTH_COUNT_ ; ++i )
		{
			if( PlayerBaseData->CoolClothList[i] == objectID )
			{
				bFind = true;
				break;
			}
		}

		if( false == bFind )
		{
			return 0;
		}
	}

	GameObjDbStruct* itemDB = NULL;
	GameObjDbStruct* imageDB = NULL;
	if( objectID > 0 )
	{
		itemDB = CNetGlobal::GetObj( objectID );
		if( NULL == itemDB )
		{
			return 0;
		}

		imageDB = g_ObjectData->GetObj(itemDB->ImageID);
		if( NULL == imageDB )
		{
			return 0;
		}

		//檢查性別
 		SexFlagStruct sex = itemDB->Limit.Sex;
		imageDB = CNetGlobal::GetObj(itemDB->ImageID);
		if( imageDB )
		{
			SexFlagStruct& LimitSex = imageDB->Image.LimitSex;
			if( LimitSex._Sex != 0 && LimitSex._Sex != -1 )
			{
				sex.Boy &= LimitSex.Boy;
				sex.Girl &= LimitSex.Girl;
			}
		}
		if( CNetGlobal::RoleData()->BaseData.Sex == EM_Sex_Boy && false == sex.Boy )
		{
			g_pGameMain->SendWarningMsg( g_ObjectData->GetString( "SYS_COOLSUIT_SETITEMOBJID_FAIL1" ) );
			return 0;
		}
		else if( CNetGlobal::RoleData()->BaseData.Sex == EM_Sex_Girl && false == sex.Girl )
		{
			g_pGameMain->SendWarningMsg( g_ObjectData->GetString( "SYS_COOLSUIT_SETITEMOBJID_FAIL1" ) );
			return 0;
		}
 

		//
		if( itemDB->Classification != EM_ObjectClass_Item || itemDB->Item.Class != EM_Item_Armor )
		{
			return 0;
		}

		switch( equipIndex )
		{
		case EM_EQWearPos_Head     : if( itemDB->Item.ArmorPos != EM_ArmorPos_Head     ){ return 0; } break;   //
		case EM_EQWearPos_Gloves   : if( itemDB->Item.ArmorPos != EM_ArmorPos_Gloves   ){ return 0; } break;   //手套
		case EM_EQWearPos_Shoes    : if( itemDB->Item.ArmorPos != EM_ArmorPos_Shoes    ){ return 0; } break;   //鞋子
		case EM_EQWearPos_Clothes  : if( itemDB->Item.ArmorPos != EM_ArmorPos_Clothes  ){ return 0; } break;   //上衣
		case EM_EQWearPos_Pants    : if( itemDB->Item.ArmorPos != EM_ArmorPos_Pants    ){ return 0; } break;   //褲子
		case EM_EQWearPos_Back     : if( itemDB->Item.ArmorPos != EM_ArmorPos_Back     ){ return 0; } break;   //背部
		case EM_EQWearPos_Belt     : if( itemDB->Item.ArmorPos != EM_ArmorPos_Belt     ){ return 0; } break;   //腰帶
		case EM_EQWearPos_Shoulder : if( itemDB->Item.ArmorPos != EM_ArmorPos_Shoulder ){ return 0; } break;   //肩甲
		case EM_EQWearPos_Ornament : if( itemDB->Item.ArmorPos != EM_ArmorPos_Ornament ){ return 0; } break;
		default:
			return 0;
		}
	}
		
	int eqIndex = 0;

	string PaperdollPartName;
	switch( equipIndex )
	{
	case EM_EQWearPos_Head:			// 1
		pCoolClothBase = &pCoolSuitBase->Head; 
		pCoolSuitBase->ShowEQ.Head = bShow;
		PaperdollPartName = PAPERDOLL_PART_NAME_HELMET;
		eqIndex = 0;
		break;

	case EM_EQWearPos_Gloves:		// 2
		pCoolClothBase = &pCoolSuitBase->Gloves; 
		pCoolSuitBase->ShowEQ.Gloves = bShow;
		PaperdollPartName = PAPERDOLL_PART_NAME_HAND;
		eqIndex = 1;
		break;

	case EM_EQWearPos_Shoes:		// 3
		pCoolClothBase = &pCoolSuitBase->Shoes; 
		pCoolSuitBase->ShowEQ.Shoes = bShow;
		PaperdollPartName = PAPERDOLL_PART_NAME_FOOT;
		eqIndex = 2;
		break;

	case EM_EQWearPos_Clothes:		// 4
		pCoolClothBase = &pCoolSuitBase->Clothes;
		pCoolSuitBase->ShowEQ.Clothes = bShow;
		PaperdollPartName = PAPERDOLL_PART_NAME_TORSO;
		eqIndex = 3;
		break;

	case EM_EQWearPos_Pants:		// 5
		pCoolClothBase = &pCoolSuitBase->Pants; 
		pCoolSuitBase->ShowEQ.Pants = bShow;
		PaperdollPartName = PAPERDOLL_PART_NAME_LEG;
		eqIndex = 4;
		break;

	case EM_EQWearPos_Back:			// 6
		pCoolClothBase = &pCoolSuitBase->Back; 
		pCoolSuitBase->ShowEQ.Back = bShow;
		PaperdollPartName = PAPERDOLL_PART_NAME_BACK;
		eqIndex = 5;
		break;

	case EM_EQWearPos_Belt:			// 7
		pCoolClothBase = &pCoolSuitBase->Belt; 
		pCoolSuitBase->ShowEQ.Belt = bShow;
		PaperdollPartName = PAPERDOLL_PART_NAME_BELT;
		eqIndex = 6;
		break;

	case EM_EQWearPos_Shoulder:		// 8
		pCoolClothBase = &pCoolSuitBase->Shoulder; 
		pCoolSuitBase->ShowEQ.Shoulder = bShow;
		PaperdollPartName = PAPERDOLL_PART_NAME_SHOULDER;
		eqIndex = 7;
		break;

	case EM_EQWearPos_Ornament:		// 20 
		pCoolClothBase = &pCoolSuitBase->Ornament; 
		pCoolSuitBase->ShowEQ.Ornament = bShow;
		eqIndex = 21;
		break;

	default:
		return 0;
	}
 
	pCoolClothBase->ImageObjectID = objectID;

	CUiModel* model = dynamic_cast<CUiModel*>( g_pGameMain->GetInterface()->GetUiWorld()->FindObject( g_strCoolSuitModel.c_str() ) );
	if( model )
	{
		if( objectID == 0 )
		{
			ItemFieldStruct* pItemField = CNetGlobal::RoleData()->GetEqItem( eqIndex );
			if( !pItemField )
			{
				bShow = false;
			}
		}

		if( bShow == false )
		{
			if( equipIndex == EM_EQWearPos_Ornament )
			{
				model->AttachEntity( ATTACH_POINT_BACK_SHIELD, "" );
			}
			else
			{
				model->SetPaperdollPart( PaperdollPartName.c_str(), "" );
				model->SetPaperdollColor( PaperdollPartName.c_str(), 0, 0 );	
			}
		}
		else if( objectID == 0 )
		{
			ItemFieldStruct* pItemField = CNetGlobal::RoleData()->GetEqItem( eqIndex );
			if( !pItemField )
			{
				return 0;
			}

			string strACTWorld = "";
			int color[2];
			color[1] = 0;
			color[0] = 0;

			GameObjDbStruct* _itemDB = CNetGlobal::GetObj( pItemField->ImageObjectID );
			if( _itemDB )
			{
				GameObjDbStruct* _imageDB = g_ObjectData->GetObj( _itemDB->ImageID );
				if( _imageDB )
				{
					strACTWorld = _imageDB->Image.ACTWorld;

					if( pItemField && pItemField->MainColor != 0 )
						color[0] = pItemField->MainColor;
					else
						color[0] = _imageDB->Image.ArmorMainColor.Color;	

					if( pItemField && pItemField->OffColor != 0 )
						color[1] = pItemField->OffColor; 
					else
						color[1] = _imageDB->Image.ArmorOffColor.Color;
				}
			}

			if( equipIndex == EM_EQWearPos_Ornament )
			{
				model->AttachEntity( ATTACH_POINT_BACK_SHIELD, strACTWorld.c_str() );
			}
			else
			{
				model->SetPaperdollPart( PaperdollPartName.c_str(), strACTWorld.c_str() );
				model->SetPaperdollColor( PaperdollPartName.c_str(), color[0], color[1] );
			}

		}
		else if( imageDB )
		{
			if( equipIndex == EM_EQWearPos_Ornament )
			{
				model->AttachEntity( ATTACH_POINT_BACK_SHIELD, imageDB->Image.ACTWorld );
			}
			else
			{
				model->SetPaperdollPart( PaperdollPartName.c_str(), imageDB->Image.ACTWorld );
				model->SetPaperdollColor( PaperdollPartName.c_str(), pCoolClothBase->MainColor, pCoolClothBase->OffColor );	
			}
		}
	}

	//g_pCharacterFrame->CoolSuitSetupChanged();
	g_pInterface->SendWorldEvent( UiEvent_RoleCoolSuitChange );

	return 0;
}

int	LuaFunc_CoolSuit_PickupEquipmentItem(lua_State* L)
{
	int equipIndex = luaL_checkint(L, 1);

	int sourceIndex = 0;
	if( g_pItemClipboard->GetType() == eClipboardItem_Type_CoolClothBag )
	{
		sourceIndex = g_pItemClipboard->GetItemInfo().CoolClothBag.index;
		g_pItemClipboard->Clear();
	}
	else
	{
		g_pItemClipboard->Clear();
		return 0;
	}

	if( sourceIndex < 0 || sourceIndex >= _DEF_MAX_COOL_CLOTH_COUNT_ )
	{
		return 0;
	}

  	PlayerBaseRoleData* PlayerBaseData = CNetGlobal::RoleData()->PlayerBaseData;
	int objID = PlayerBaseData->CoolClothList[sourceIndex];

	return Func_CoolSuit_SetItemObjID( equipIndex, objID );
}



// ----------------------------------------------------------------------------
int LuaFunc_CoolSuit_SetModel(lua_State* L)
{
	g_strCoolSuitModel = luaL_checkstring(L, 1);
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_CoolSuit_GetItemInfo(lua_State* L)
{
	int suitID = luaL_checkint(L, 1) - 1;
	int equipIndex = luaL_checkint(L, 2);

 	if( suitID < 0 || suitID >= _DEF_MAX_COOL_SUIT_COUNT_ )
	{
		return 0;
	}

	//if( clothID < 0 || clothID >= 9 )
	//{
	//	return 0;
	//}

	PlayerBaseRoleData* PlayerBaseData = CNetGlobal::RoleData()->PlayerBaseData;
	CoolSuitBase* pCoolSuitBase = &PlayerBaseData->CoolSuitList[suitID];

	if( suitID == PlayerBaseData->CoolSuitID && g_pCharacterFrame->m_bCoolSuitSetuping )
	{
		pCoolSuitBase = &g_pCharacterFrame->m_CoolSuitSetupTemp;
	}

	CoolClothBase* pCoolClothBase = NULL;
	bool show = false;
	switch( equipIndex )
	{
	case EM_EQWearPos_Head:			// 1
  		pCoolClothBase = &pCoolSuitBase->Head; 
		show = pCoolSuitBase->ShowEQ.Head;
		break;

	case EM_EQWearPos_Gloves:		// 2
		pCoolClothBase = &pCoolSuitBase->Gloves; 
		show = pCoolSuitBase->ShowEQ.Gloves;			
		break;

	case EM_EQWearPos_Shoes:		// 3
		pCoolClothBase = &pCoolSuitBase->Shoes; 
		show = pCoolSuitBase->ShowEQ.Shoes;					
		break;

	case EM_EQWearPos_Clothes:		// 4
		pCoolClothBase = &pCoolSuitBase->Clothes; 
		show = pCoolSuitBase->ShowEQ.Clothes;				
		break;

	case EM_EQWearPos_Pants:		// 5
		pCoolClothBase = &pCoolSuitBase->Pants; 
		show = pCoolSuitBase->ShowEQ.Pants;	
		break;

	case EM_EQWearPos_Back:			// 6
		pCoolClothBase = &pCoolSuitBase->Back; 
		show = pCoolSuitBase->ShowEQ.Back;
		break;

	case EM_EQWearPos_Belt:			// 7
		pCoolClothBase = &pCoolSuitBase->Belt; 
		show = pCoolSuitBase->ShowEQ.Belt;	
		break;

	case EM_EQWearPos_Shoulder:		// 8
		pCoolClothBase = &pCoolSuitBase->Shoulder; 
		show = pCoolSuitBase->ShowEQ.Shoulder;
		break;

	case EM_EQWearPos_Ornament:		// 20 
		pCoolClothBase = &pCoolSuitBase->Ornament; 
		show = pCoolSuitBase->ShowEQ.Ornament;
		break;

	default:
		return 0;

	}

	if( false == show )
	{
		lua_pushnumber( L, -1 );
		return 1;
	}

	if( pCoolClothBase->ImageObjectID == 0 )
	{
		lua_pushnumber( L, 0 );
		return 1;
	}

	GameObjDbStruct* itemDB = CNetGlobal::GetObj( pCoolClothBase->ImageObjectID );
 	if( NULL == itemDB )
	{
		return 0;
	}

	GameObjDbStruct* imageDB = g_ObjectData->GetObj(itemDB->ImageID);
	if( NULL == imageDB )
	{
		return 0;
	}

	string link;
	if( false == ItemGUID_To_ItemLink( pCoolClothBase->ImageObjectID, link ) )
	{
		return 0;
	}

	int quality = g_ObjectData->GetItemQuality( imageDB );


	CRuColor4 MainColor( (UINT32)pCoolClothBase->MainColor );
	CRuColor4 OffColor ( (UINT32)pCoolClothBase->OffColor  );

	bool coloring = itemDB->Mode.Coloring;
	if( equipIndex == EM_EQWearPos_Ornament )
	{
		coloring = false;
	}
 
	lua_pushnumber( L, pCoolClothBase->ImageObjectID );
	lua_pushstring( L, imageDB->Image.ACTField );
	lua_pushnumber( L, quality );
	lua_pushstring( L, link.c_str() );

	lua_pushboolean( L, coloring );
 	lua_pushnumber( L, MainColor.m_a );
	lua_pushnumber( L, MainColor.m_r );
	lua_pushnumber( L, MainColor.m_g );
	lua_pushnumber( L, MainColor.m_b );

 	lua_pushnumber( L, OffColor.m_a );
	lua_pushnumber( L, OffColor.m_r );
	lua_pushnumber( L, OffColor.m_g );
	lua_pushnumber( L, OffColor.m_b );
 
 	return 13;
}


// ----------------------------------------------------------------------------
int LuaFunc_CoolSuit_SetItemObjID(lua_State* L)
{
	//int suitID = luaL_checkint(L, 1) - 1;
	int equipIndex = luaL_checkint(L, 1);
 	int objectID = luaL_checkint(L, 2);

 	return Func_CoolSuit_SetItemObjID( equipIndex, objectID );
}

// ----------------------------------------------------------------------------
int LuaFunc_CoolSuit_SetItemColor(lua_State* L)
{
	//int suitID = luaL_checkint(L, 1) - 1;
	int equipIndex = luaL_checkint(L, 1);

	int colorID = (int)luaL_checknumber(L,2);
	CRuColor4 Color( (float)luaL_checknumber(L,3), (float)luaL_checknumber(L,4), (float)luaL_checknumber(L,5), (float)luaL_checknumber(L,6) );
	
	PlayerBaseRoleData* PlayerBaseData = CNetGlobal::RoleData()->PlayerBaseData;
	int suitID = PlayerBaseData->CoolSuitID;

	if( suitID < 0 || suitID >= _DEF_MAX_COOL_SUIT_COUNT_ )
	{
		return 0;
	}

	if( false == g_pCharacterFrame->m_bCoolSuitSetuping )
	{
		g_pCharacterFrame->m_CoolSuitSetupTemp = PlayerBaseData->CoolSuitList[suitID];

		g_pCharacterFrame->m_bCoolSuitSetuping = true;
	}

	CoolSuitBase* pCoolSuitBase = &g_pCharacterFrame->m_CoolSuitSetupTemp;
	CoolClothBase* pCoolClothBase = NULL;


	string PaperdollPartName;
	switch( equipIndex )
	{
	case EM_EQWearPos_Head:			// 1
		pCoolClothBase = &pCoolSuitBase->Head; 
		PaperdollPartName = PAPERDOLL_PART_NAME_HELMET;
		break;

	case EM_EQWearPos_Gloves:		// 2
		pCoolClothBase = &pCoolSuitBase->Gloves; 
		PaperdollPartName = PAPERDOLL_PART_NAME_HAND;
		break;

	case EM_EQWearPos_Shoes:		// 3
		pCoolClothBase = &pCoolSuitBase->Shoes;
		PaperdollPartName = PAPERDOLL_PART_NAME_FOOT;
		break;

	case EM_EQWearPos_Clothes:		// 4
		pCoolClothBase = &pCoolSuitBase->Clothes;
		PaperdollPartName = PAPERDOLL_PART_NAME_TORSO;
		break;

	case EM_EQWearPos_Pants:		// 5
		pCoolClothBase = &pCoolSuitBase->Pants; 
		PaperdollPartName = PAPERDOLL_PART_NAME_LEG;
		break;

	case EM_EQWearPos_Back:			// 6
		pCoolClothBase = &pCoolSuitBase->Back; 
		PaperdollPartName = PAPERDOLL_PART_NAME_BACK;
		break;

	case EM_EQWearPos_Belt:			// 7
		pCoolClothBase = &pCoolSuitBase->Belt; 
		PaperdollPartName = PAPERDOLL_PART_NAME_BELT;
		break;

	case EM_EQWearPos_Shoulder:		// 8
		pCoolClothBase = &pCoolSuitBase->Shoulder; 
		PaperdollPartName = PAPERDOLL_PART_NAME_SHOULDER;
		break;

	//case EM_EQWearPos_Ornament:		// 20 
	//	pCoolClothBase = &pCoolSuitBase->Ornament; 
	//	break;

	default:
		return 0;
	}

	int nColor = Color.GetARGB();
	if( nColor < 0xFF000000 )
	{
		nColor = 0;
	}

	if( colorID == 1 )
	{
		pCoolClothBase->MainColor = nColor;
 	}
	else
	{
		pCoolClothBase->OffColor = nColor;
	}

	//可能是批次設定改的
	if( pCoolClothBase->ImageObjectID <= 0 )
	{
		return 0;
	}

	CUiModel* model = dynamic_cast<CUiModel*>( g_pGameMain->GetInterface()->GetUiWorld()->FindObject( g_strCoolSuitModel.c_str() ) );
	if( model )
	{
		model->SetPaperdollColor( PaperdollPartName.c_str(), pCoolClothBase->MainColor, pCoolClothBase->OffColor );	
	}

//	g_pCharacterFrame->CoolSuitSetupChanged();
	g_pInterface->SendWorldEvent( UiEvent_RoleCoolSuitChange );

	return 0;
}


// ----------------------------------------------------------------------------
int LuaFunc_CoolSuit_IsSetuping(lua_State* L)
{
	if( g_pCharacterFrame->m_bCoolSuitSetuping )
	{
 		PlayerBaseRoleData* PlayerBaseData = CNetGlobal::RoleData()->PlayerBaseData;
		int suitID = PlayerBaseData->CoolSuitID;

		if( suitID < 0 || suitID >= _DEF_MAX_COOL_SUIT_COUNT_ )
		{
			return 0;
		}

		if( false == g_pCharacterFrame->m_bCoolSuitSetuping )
		{
			g_pCharacterFrame->m_CoolSuitSetupTemp = PlayerBaseData->CoolSuitList[suitID];

			g_pCharacterFrame->m_bCoolSuitSetuping = true;
		}

		if( memcmp( &g_pCharacterFrame->m_CoolSuitSetupTemp, &PlayerBaseData->CoolSuitList[suitID], sizeof(CoolSuitBase) ) == 0 )
		{
			g_pCharacterFrame->m_bCoolSuitSetuping = false;
		}
	}

	lua_pushboolean( L, g_pCharacterFrame->m_bCoolSuitSetuping );

	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_CoolSuit_SetSuitRequest(lua_State* L)
{
	if( !g_pCharacterFrame->m_bCoolSuitSetuping )
	{
		return 0;
	}

	static int lastTime = 0;
	if( timeGetTime() - lastTime < 2000 )
	{
		//修改太頻繁
		return 0;
	}
	lastTime = timeGetTime();

	PlayerBaseRoleData* PlayerBaseData = CNetGlobal::RoleData()->PlayerBaseData;
	int suitID = PlayerBaseData->CoolSuitID;

	if( suitID < 0 || suitID >= _DEF_MAX_COOL_SUIT_COUNT_ )
	{
		return 0;
	}

	//判斷有沒有裝備被刪除
	bool bFind = false;
	CoolClothBase* pCoolCloth;
	for( int i = 0 ; i < 9 ; ++i )
	{
		pCoolCloth = &g_pCharacterFrame->m_CoolSuitSetupTemp.Item[i];
		if( pCoolCloth->ImageObjectID > 0 )
		{
			bFind = false;

			for( int n = 0 ; n < _DEF_MAX_COOL_CLOTH_COUNT_ ; ++n )
			{
				if( PlayerBaseData->CoolClothList[n] == pCoolCloth->ImageObjectID )
				{
					bFind = true;
					break;
				}
			}

			if( false == bFind )
			{
				pCoolCloth->ImageObjectID = 0;
   			}
		}
	}
	
	NetCli_ItemChild::SetCoolSuitRequest( suitID , g_pCharacterFrame->m_CoolSuitSetupTemp );

	g_pCharacterFrame->m_bCoolSuitSetuping = false;
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_CoolSuit_SetSuitCancel(lua_State* L)
{
	if( g_pCharacterFrame->m_bCoolSuitSetuping )
	{
		g_pCharacterFrame->m_bCoolSuitSetuping = false;
		//g_pCharacterFrame->CoolSuitSetupChanged();

		CUiModel* model = dynamic_cast<CUiModel*>( g_pGameMain->GetInterface()->GetUiWorld()->FindObject( g_strCoolSuitModel.c_str() ) );
		if( model )
		{
			CUnitFrame::Model_SetRoleSprite( model, g_pGameMain->GetPlayer(), true, false );
		}

		g_pInterface->SendWorldEvent( UiEvent_RoleCoolSuitChange );
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetPlayerSkilled(lua_State* L)
{	
	const char* str = luaL_checkstring(L, 1);
	RoleValueData& Attr = CNetGlobal::RoleData()->TempData.Attr;	
	float extraValue, baseValue, maxValue, currValue;
	
	//int type = -1;
	//
	//if ( strcmp(str, "UNARMED") == 0 )
	//	type = EM_SkillValueType_Unarmed;
	//else if ( strcmp(str, "BLADE") == 0 )
	//	type = EM_SkillValueType_Blade;
	//else if ( strcmp(str, "DAGGER") == 0 )
	//	type = EM_SkillValueType_Dagger;
	//else if ( strcmp(str, "WAND") == 0 )
	//	type = EM_SkillValueType_Wand;
	//else if ( strcmp(str, "AXE") == 0 )
	//	type = EM_SkillValueType_Axe;
	//else if ( strcmp(str, "BLUDGEON") == 0 )
	//	type = EM_SkillValueType_Bludgeon;
	//else if ( strcmp(str, "CLAYMORE") == 0 )
	//	type = EM_SkillValueType_Claymore;
	//else if ( strcmp(str, "STAFF") == 0 )
	//	type = EM_SkillValueType_Staff;
	//else if ( strcmp(str, "2HAXE") == 0 )
	//	type = EM_SkillValueType_2H_Axe;
	//else if ( strcmp(str, "HAMMER") == 0 )
	//	type = EM_SkillValueType_2H_Hammer;
	//else if ( strcmp(str, "POLEARM") == 0 )
	//	type = EM_SkillValueType_Polearm;
	//else if ( strcmp(str, "BOW") == 0 )
	//	type = EM_SkillValueType_Bow;
	//else if ( strcmp(str, "COSSBOW") == 0 )
	//	type = EM_SkillValueType_CossBow;
	//else if ( strcmp(str, "GUN") == 0 )
	//	type = EM_SkillValueType_Gun;
	//else if ( strcmp(str, "DEFENCE") == 0 )
	//	type = EM_SkillValueType_Define;
	//else if ( strcmp(str, "BLACKSMITH") == 0 )
	//	type = EM_SkillValueType_BlackSmith;
	//else if ( strcmp(str, "CARPENTER") == 0 )
	//	type = EM_SkillValueType_Carpenter;
	//else if ( strcmp(str, "LEATHERWORKING") == 0 )
	//	type = EM_SkillValueType_MakeArmor;
	//else if ( strcmp(str, "TAILOR") == 0 )
	//	type = EM_SkillValueType_Tailor;
	//else if ( strcmp(str, "COOK") == 0 )
	//	type = EM_SkillValueType_Cook;
	//else if ( strcmp(str, "ALCHEMY") == 0 )
	//	type = EM_SkillValueType_Alchemy;
	//else if ( strcmp(str, "MINING") == 0 )
	//	type = EM_SkillValueType_Mining;
	//else if ( strcmp(str, "LUMBERING") == 0 )
	//	type = EM_SkillValueType_Lumbering;
	//else if ( strcmp(str, "HERBLISM") == 0 )
	//	type = EM_SkillValueType_Herblism;
	//else if ( strcmp(str, "FISHING") == 0 )
	//	type = EM_SkillValueType_Fishing;
	//else if ( strcmp(str, "PLANT") == 0 )
	//	type = EM_SkillValueType_Plant;
	//else
	//	return 0;

	int type = g_ObjectData->GetSkillValueTypeENUM(str);
	if( type == EM_SkillValueType_None )
		return 0;

	maxValue = Attr.MaxSkillValue.GetSkill(type);
	baseValue = Attr.SkillValue->GetSkill(type);
	currValue = Attr.Fin.SkillValue.GetSkill(type);
	extraValue = currValue - min(maxValue, baseValue);

	lua_pushnumber(L, currValue);
	lua_pushnumber(L, extraValue);
	lua_pushnumber(L, maxValue);
	return 3;
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
int LuaFunc_GetPlayerCurrentSkillValue(lua_State* L)
{
	const char* str = luaL_checkstring(L, 1);
	RoleValueData& Attr = CNetGlobal::RoleData()->TempData.Attr;	

	float fValue = 0.0f;
	int type = g_ObjectData->GetSkillValueTypeENUM(str);
	if( type == EM_SkillValueType_None )
	{
		fValue = 0.0f;
	}
	else
	{
		fValue = Attr.Fin.SkillValue.GetSkill(type);
	}
	lua_pushnumber(L, fValue);
	return 1;
}


// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
int LuaFunc_GetPlayerMaxSkillValue(lua_State* L)
{
	const char* str = luaL_checkstring(L, 1);
	RoleValueData& Attr = CNetGlobal::RoleData()->TempData.Attr;	

	float fValue = 0.0f;
	int type = g_ObjectData->GetSkillValueTypeENUM(str);
	if( type == EM_SkillValueType_None )
	{
		fValue = 0.0f;
	}
	else
	{
		fValue = Attr.MaxSkillValue.GetSkill(type);
	}
	lua_pushnumber(L, fValue);
	return 1;
}

// ----------------------------------------------------------------------------
//更新稱號資訊
// ----------------------------------------------------------------------------
int Luafunc_UpdateTitleInfo(lua_State* L)
{
	g_pCharacterFrame->UpdateTitleInfo();
	return 0;
}

// ----------------------------------------------------------------------------
//清除稱號資訊
// ----------------------------------------------------------------------------
int Luafunc_ClearTitleInfo(lua_State* L)
{
	g_pCharacterFrame->ClearTitleInfo();
	return 0;
}

// ----------------------------------------------------------------------------
//取得稱號數量
// ----------------------------------------------------------------------------
int Luafunc_GetTitleCount(lua_State* L) 
{
	lua_pushnumber( L, g_pCharacterFrame->GetTitleCount() );
	return 1;
}
// ----------------------------------------------------------------------------
//取得最高稱號等級
// ----------------------------------------------------------------------------
int Luafunc_GetTitleMaxLv(lua_State* L) 
{
	lua_pushnumber( L, g_pCharacterFrame->GetTitleMaxLv() );
	return 1;
}

// ----------------------------------------------------------------------------
//取得稱號資訊
// ----------------------------------------------------------------------------
int LuaFunc_GetTitleInfoByIndex(lua_State* L) 
{
	int index = luaL_checkint(L, 1);

	sTitleInfo* pTitleInfo = g_pCharacterFrame->GetTitleInfoByIndex( index );

	if( NULL == pTitleInfo )
		return 0;

	GameObjDbStructEx* pTitleObj = g_ObjectData->GetObj( pTitleInfo->nID );

	if( NULL == pTitleObj )
		return 0;

	string strShortNote = CNetCli_Script_Child::ReplaceSystemKeyword( pTitleObj->ShortNote );
   

	string strName;

	if( pTitleInfo->bGeted )
	{
		strName = pTitleObj->GetName();
	}
	else
	{
		strName = g_ObjectData->GetString( "SYS_TITLE_UNGET" );
	}
  
	lua_pushstring( L, strName.c_str() );
	lua_pushnumber( L, pTitleInfo->nID );
	lua_pushboolean( L, pTitleInfo->bGeted );

	 
 	GameObjDbStruct* imageDB = CNetGlobal::GetObj( pTitleObj->ImageID );
	if ( imageDB )
	{
		lua_pushstring( L, imageDB->Image.ACTField ); // ICON 位置
	}
	else
	{
		lua_pushnil( L );
	}	

	lua_pushnumber( L, pTitleObj->Title.Classify[0] );
	lua_pushnumber( L, pTitleObj->Title.Classify[1] );

	lua_pushstring( L, strShortNote.c_str() );

	string strBrief = strShortNote.c_str();

	for( int i = 0 ; i < strBrief.length() ; ++i )
	{
		if( strBrief.at(i) == 0x0d || strBrief.at(i) == 0x0a )
		{
			strBrief.resize( i );
		}
	}

 	lua_pushstring( L, strBrief.c_str() );
	lua_pushnumber( L, pTitleObj->Rare );

	return 9;
}

// ----------------------------------------------------------------------------
//取得自訂稱號
// ----------------------------------------------------------------------------
int LuaFunc_GetCusomizeTitle(lua_State* L)
{
	PlayerBaseRoleData* pBaseData = CNetGlobal::RoleData()->PlayerBaseData;
	lua_pushstring( L, pBaseData->TitleStr );
	return 1;
}


// ----------------------------------------------------------------------------
//取得現在的稱號
// ----------------------------------------------------------------------------
int LuaFunc_GetCurrentTitle(lua_State* L)
{
	GameObjDbStructEx* pTitleObj;
	//
//	PlayerBaseRoleData* pBaseData = CNetGlobal::RoleData()->PlayerBaseData;
//	PlayerTempRoleData* pTempData = CNetGlobal::RoleData()->PlayerTempData;

	int nTileID = CNetGlobal::RoleData()->BaseData.TitleID;// + Def_ObjectClass_Title;
	lua_pushnumber( L, nTileID );
	
	//無稱號
	if( nTileID == 0 )
	{
		lua_pushstring( L, "" );
	}
	//使用自訂稱號
	else if( nTileID == -1 )
	{
		lua_pushstring( L, CNetGlobal::RoleData()->PlayerBaseData->TitleStr );
	}
	else
	{
		pTitleObj = g_ObjectData->GetObj( nTileID );
		if( pTitleObj )
		{
			lua_pushstring( L, pTitleObj->GetName() );	
		}
		else
		{
			lua_pushstring( L, "" );
		}
	}

	return 2;
}

// ----------------------------------------------------------------------------
//裝備稱號
// ----------------------------------------------------------------------------
int Luafunc_SetTitleRequest(lua_State* L)     
{
	int nID = luaL_checkint(L, 1);
	
//	g_pCharacterFrame->R_SetTitle_OK( nID );
	g_pCharacterFrame->S_SetTitleRequest( nID );
	return 0;
}

// ----------------------------------------------------------------------------
//在稱號裝備介面中 不可裝備的稱號要不要顯示
// ----------------------------------------------------------------------------
int LuaFunc_SetDisableTitleHide(lua_State* L)
{
	bool bIs = ( lua_toboolean(L, 1) == 0 ) ? false : true ;
	g_pGameMain->SetDisableTitleHide( bIs );
	return 0;
}

// ----------------------------------------------------------------------------
//在稱號裝備介面中 不可裝備的稱號要不要顯示
// ----------------------------------------------------------------------------
int LuaFunc_GetDisableTitleHide(lua_State* L)
{
	lua_pushboolean( L, g_pGameMain->GetDisableTitleHide() );
	return 1;
}

// ----------------------------------------------------------------------------
//檢查有無新稱號
// ----------------------------------------------------------------------------
int LuaFunc_CheckAllTitles(lua_State* L)
{
	//
	static int s_lastCheckTime = 0;
	
	int nowTime = timeGetTime();
	if( s_lastCheckTime == 0 || nowTime - s_lastCheckTime > 300000 )
	{
		s_lastCheckTime = nowTime;
		NetCli_OtherChild::S_RunGlobalPlotRequest( 8, 0 );
	}
	else
	{
		//已檢查過，請稍候在嘗試。
		g_pGameMain->SendSystemChatMsg( g_ObjectData->GetString( "SYS_CHECKALLTITLES_FAIL" ) );
	}
	
	return 0;
}

// ----------------------------------------------------------------------------
// 取得物品品質顏色
int LuaFunc_GetItemQualityColor(lua_State* L)
{
	int color = 0;
	if ( lua_isnumber(L, 1) )
	{
		color = g_ObjectData->GetItemQualityColor((int)lua_tonumber(L, 1));
	}
	lua_pushnumber(L, ((color & 0x00FF0000)>>16) / 256.0f);
	lua_pushnumber(L, ((color & 0x0000FF00)>>8) / 256.0f);
	lua_pushnumber(L, (color & 0x0000000FF) / 256.0f);
	return 3;
}
// ----------------------------------------------------------------------------
// 取得物品
int LuaFunc_GetEquipBySlot(lua_State* L)
{
	int index = (int)lua_tonumber(L, 1) - 1 + _MAX_BODY_BEGIN_POS_;
	int Slot = (int)lua_tonumber(L, 2) - 1;
	//int index = CNetGlobal::RoleData()->GetBodyRealPos(pos);
	bool canUse=CNetGlobal::RoleData()->IsActiveBagItem(index);
	//if(index>=0&&index<BagCount)
	{
		ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(index);
		if (item&&item->Pos == EM_ItemState_NONE)
		{
			GameObjDbStruct* itemDB = CNetGlobal::GetObj(item->OrgObjID);
			if (itemDB&&itemDB->Classification==EM_ObjectClass_Item)
			{
				const char *Str=NULL;
				int quality = g_ObjectData->GetItemQuality(itemDB, item);
				int count = item->GetCount();

				
				switch(Slot)
				{
				//	Str=ItemUTIL::GetItemIcon(item);//
					case EM_EQWearPos_Head:			// 1
						if (itemDB->Item.Class== EM_Item_Armor&&itemDB->Item.ArmorPos==EM_ArmorPos_Head)
							Str=ItemUTIL::GetItemIcon(item);//Str=itemDB->ImageObj->ACTField;						
						break;

					case EM_EQWearPos_Gloves:		// 2
						if (itemDB->Item.Class== EM_Item_Armor&&itemDB->Item.ArmorPos==EM_ArmorPos_Gloves)
							Str=ItemUTIL::GetItemIcon(item);//Str=itemDB->ImageObj->ACTField;						
						break;

					case EM_EQWearPos_Shoes:		// 3
						if (itemDB->Item.Class== EM_Item_Armor&&itemDB->Item.ArmorPos==EM_ArmorPos_Shoes)
							Str=ItemUTIL::GetItemIcon(item);//Str=itemDB->ImageObj->ACTField;						
						break;

					case EM_EQWearPos_Clothes:		// 4
						if (itemDB->Item.Class== EM_Item_Armor&&itemDB->Item.ArmorPos==EM_ArmorPos_Clothes)
							Str=ItemUTIL::GetItemIcon(item);//Str=itemDB->ImageObj->ACTField;						
						break;

					case EM_EQWearPos_Pants:		// 5
						if (itemDB->Item.Class== EM_Item_Armor&&itemDB->Item.ArmorPos==EM_ArmorPos_Pants)
							Str=ItemUTIL::GetItemIcon(item);//Str=itemDB->ImageObj->ACTField;		
						break;

					case EM_EQWearPos_Back:			// 6
						if (itemDB->Item.Class== EM_Item_Armor&&itemDB->Item.ArmorPos==EM_ArmorPos_Back)
							Str=ItemUTIL::GetItemIcon(item);//Str=itemDB->ImageObj->ACTField;		
						break;

					case EM_EQWearPos_Belt:			// 7
						if (itemDB->Item.Class== EM_Item_Armor&&itemDB->Item.ArmorPos==EM_ArmorPos_Belt)
							Str=ItemUTIL::GetItemIcon(item);//Str=itemDB->ImageObj->ACTField;		
						break;

					case EM_EQWearPos_Shoulder:		// 8
						if (itemDB->Item.Class== EM_Item_Armor&&itemDB->Item.ArmorPos==EM_ArmorPos_Shoulder)
							Str=ItemUTIL::GetItemIcon(item);//Str=itemDB->ImageObj->ACTField;		
						break;


					case EM_EQWearPos_Necklace:		// 9
						if (itemDB->Item.Class== EM_Item_Armor&&itemDB->Item.ArmorPos==EM_ArmorPos_Necklace)
							Str=ItemUTIL::GetItemIcon(item);//Str=itemDB->ImageObj->ACTField;		
						break;

					case EM_EQWearPos_Ammo:			// 10
						if (itemDB->Item.Class== EM_Item_Weapon&&itemDB->Item.WeaponPos==EM_WeaponPos_Ammo)
							Str=ItemUTIL::GetItemIcon(item);//Str=itemDB->ImageObj->ACTField;		
						break;

					case EM_EQWearPos_Bow:			// 11
						if (itemDB->Item.Class== EM_Item_Weapon&&itemDB->Item.WeaponPos==EM_WeaponPos_Bow)
							Str=ItemUTIL::GetItemIcon(item);//Str=itemDB->ImageObj->ACTField;		
						break;

					case EM_EQWearPos_Ring1:		// 12
						
					case EM_EQWearPos_Ring2:		// 13
						if (itemDB->Item.Class== EM_Item_Armor&&itemDB->Item.ArmorPos==EM_ArmorPos_Ring)
							Str=ItemUTIL::GetItemIcon(item);//Str=itemDB->ImageObj->ACTField;		
						break;

					case EM_EQWearPos_Earring1:		// 14
					case EM_EQWearPos_Earring2:		// 15
						if (itemDB->Item.Class== EM_Item_Armor&&itemDB->Item.ArmorPos==EM_ArmorPos_Earring)
							Str=ItemUTIL::GetItemIcon(item);//Str=itemDB->ImageObj->ACTField;		
						break;

					case EM_EQWearPos_MainHand:		// 16
						if (itemDB->Item.Class== EM_Item_Weapon&&(itemDB->Item.WeaponPos==EM_WeaponPos_MainHand||itemDB->Item.WeaponPos==EM_WeaponPos_OneHand ||itemDB->Item.WeaponPos==EM_WeaponPos_TwoHand ))
							Str=ItemUTIL::GetItemIcon(item);//Str=itemDB->ImageObj->ACTField;		
						break;

					case EM_EQWearPos_SecondHand:	// 17
						if (itemDB->Item.Class== EM_Item_Weapon&&(itemDB->Item.WeaponPos==EM_WeaponPos_SecondHand||itemDB->Item.WeaponPos==EM_WeaponPos_OneHand )|| 
							itemDB->Item.Class== EM_Item_Armor&&(itemDB->Item.ArmorPos==EM_ArmorPos_SecondHand ))
							Str=ItemUTIL::GetItemIcon(item);//Str=itemDB->ImageObj->ACTField;		
						break;
					case EM_EQWearPos_Manufactory:	// 18
						if (itemDB->Item.Class== EM_Item_Weapon&&(itemDB->Item.WeaponPos==EM_WeaponPos_Manufacture ))
							Str=ItemUTIL::GetItemIcon(item);//Str=itemDB->ImageObj->ACTField;		
						break;
						
					case EM_EQWearPos_MagicTool1:	// 19
						if (itemDB->Item.Class== EM_Item_Armor&&(itemDB->Item.ArmorPos==EM_ArmorPos_MagicTool))
							Str=ItemUTIL::GetItemIcon(item);//Str=itemDB->ImageObj->ACTField;		
						break;
					case EM_EQWearPos_Ornament:		// 20 
						if (itemDB->Item.Class== EM_Item_Armor&&(itemDB->Item.ArmorPos==EM_ArmorPos_Ornament ))
							Str=ItemUTIL::GetItemIcon(item);//Str=itemDB->ImageObj->ACTField;		
						break;
					
				}
				if(Str)
				{
					lua_pushstring(L, Str);			// push item texture file
					lua_pushnumber(L, quality);			// push item quality					
					lua_pushnumber(L, count);			// push item count
					lua_pushboolean(L, !(CNetGlobal::RoleData()->CanWearObject(itemDB, Slot)&canUse)); // push item inv
					lua_pushnumber(L, index + 1);
					return 5;

				}
				
			}
		}	
	}
	return 0;
}

//取得裝備位置的底圖是否有裝備道具
int LuaFunc_GetEquipSlotInfo(lua_State* L)
{	
	RoleDataEx* role = CNetGlobal::RoleData();
	int Slot = luaL_checkint(L, 1) - 1;	

	ItemFieldStruct* item = role->GetEqItem(Slot);
	if ( item )
	{
		const char *SlotBaseTexture=NULL;
		if ( !item->IsEmpty() )
		{
			switch (Slot)
			{
			case EM_EQWearPos_Head:			// 1
				SlotBaseTexture="interface\\CharacterFrame\\Equipment-HelmetSlot";
				break;

			case EM_EQWearPos_Gloves:		// 2
				SlotBaseTexture="interface\\CharacterFrame\\Equipment-GlovesSlot";
				break;

			case EM_EQWearPos_Shoes:		// 3
				SlotBaseTexture="interface\\CharacterFrame\\Equipment-FeetSlot";
				break;

			case EM_EQWearPos_Clothes:		// 4
				SlotBaseTexture="interface\\CharacterFrame\\Equipment-ChestSlot";
				break;

			case EM_EQWearPos_Pants:		// 5
				SlotBaseTexture="interface\\CharacterFrame\\Equipment-LegsSlot";
				break;

			case EM_EQWearPos_Back:			// 6
				SlotBaseTexture="interface\\CharacterFrame\\Equipment-BackSlot";
				break;

			case EM_EQWearPos_Belt:			// 7
				SlotBaseTexture="interface\\CharacterFrame\\Equipment-WaistSlot";
				break;

			case EM_EQWearPos_Shoulder:		// 8
				SlotBaseTexture="interface\\CharacterFrame\\Equipment-ShoulderSlot";
				break;

			case EM_EQWearPos_Necklace:		// 9
				SlotBaseTexture="interface\\CharacterFrame\\Equipment-NecklaceSlot";
				break;

			case EM_EQWearPos_Ammo:			// 10
				SlotBaseTexture="interface\\CharacterFrame\\Equipment-AmmoSlot";
				break;

			case EM_EQWearPos_Bow:			// 11
				SlotBaseTexture="interface\\CharacterFrame\\Equipment-RangedSlot";
				break;

			case EM_EQWearPos_Ring1:		// 12
			case EM_EQWearPos_Ring2:		// 13
				SlotBaseTexture="interface\\CharacterFrame\\Equipment-RingsSlot";
				break;

			case EM_EQWearPos_Earring1:		// 14
			case EM_EQWearPos_Earring2:		// 15
				SlotBaseTexture="interface\\CharacterFrame\\Equipment-EarringsSlot";
				break;

			case EM_EQWearPos_MainHand:		// 16
				SlotBaseTexture="interface\\CharacterFrame\\Equipment-MainHandSlot";
				break;

			case EM_EQWearPos_SecondHand:	// 17
				SlotBaseTexture="interface\\CharacterFrame\\Equipment-SecondaryHandSlot";
				break;

			case EM_EQWearPos_Manufactory:	// 18
				SlotBaseTexture="interface\\CharacterFrame\\Equipment-UtensilSlot";
				break;
			case EM_EQWearPos_MagicTool1:	// 19
			case EM_EQWearPos_MagicTool2:	// 20
			case EM_EQWearPos_MagicTool3:	// 21
				SlotBaseTexture="interface\\CharacterFrame\\Equipment-TalismanSlot";
				break;
			case EM_EQWearPos_Ornament:		// 22
				SlotBaseTexture="interface\\CharacterFrame\\Equipment-AdornmentSlot";
				break;
			}
		}
		if (SlotBaseTexture)
		{

		
			lua_pushstring(L,SlotBaseTexture);
			return 1;
		}
		
	}
	return 0;

}

// ----------------------------------------------------------------------------
int LuaFunc_CheckHonorPartyState(lua_State* L)
{
	lua_pushboolean(L, CNetGlobal::RoleData()->PlayerBaseData->TimeFlag[EM_TimeFlagEffect_HonorParty] >= 0);
	return 1;
}


int LuaFunc_CoolClothBag_GetPageInfo(lua_State* L)
{
	int index = (int)lua_tonumber(L, 1);
	char temp[512]; 
	sprintf(temp,"Wardrobe_PageCost%d",index);


	int v1=65535,v2=65535,v3=65535;
	bool open=true;
	map<string,MoneyExchangeKeyValueStruct>::iterator Iter;
	Iter = ObjectDataClass::_MoneyExchangeKeyValue.find( temp );
	if( Iter == ObjectDataClass::_MoneyExchangeKeyValue.end() )
		open=false;
	else
	{
		MoneyExchangeKeyValueStruct &v= Iter->second;

		v1=v.Value[ 0 ];
		v2=v.Value[ 1 ];
		v3=v.Value[ 2 ];
	}
	lua_pushnumber(L,v1);
	lua_pushnumber(L,v2);
	lua_pushnumber(L,v3);
	lua_pushboolean(L,open);
	return 4;
}

int LuaFunc_CharactFrame_GetEquipSlotCount(lua_State* L)
{
	char temp[512];

	int count=0;
	
	for (int i=0;i<6;i++)
	{
		sprintf(temp,"BackupEQSlot%d_KeyItemID",i+1);
		int KeyItem=g_ObjectData->GetSysKeyValue(temp);
		bool bResult =true;
		if (KeyItem>0)		
			bResult=CNetGlobal::RoleData()->BaseData.KeyItem.GetFlag( KeyItem - Def_ObjectClass_KeyItem) ;	
		if (bResult)
			count++;

	}

	lua_pushnumber( L,count );
	return 1;

	
	
}
int LuaFunc_CoolSuit_GetTierEnable(lua_State* L)
{
	//int count=2;
	char temp[512];

	int index = luaL_checkint(L, 1) ;

	sprintf(temp,"Wardrobe_Set%d",index);
	int KeyItem=g_ObjectData->GetSysKeyValue(temp);
	bool bResult =true;

	if (KeyItem>0)		
		bResult=CNetGlobal::RoleData()->Base.AccountFlag.GetFlag( KeyItem ) ;	

	lua_pushboolean( L,bResult );
	return 1;
}
int LuaFunc_CoolSuit_BuyNewTier(lua_State* L)
{
	if (lua_isnumber(L, 1))
	{
		int index = (int)lua_tonumber(L, 1);

		char temp[512];
		sprintf(temp,"Wardrobe_SetCost%d", index);

		map<string,MoneyExchangeKeyValueStruct>::iterator Iter = ObjectDataClass::_MoneyExchangeKeyValue.find( temp );
		if (Iter == ObjectDataClass::_MoneyExchangeKeyValue.end())
		{
			return 0;
		}

		MoneyExchangeKeyValueStruct &v= Iter->second;
		if (v.Value[ 2 ] > 0 && !g_pGameMain->CheckSecondPassword())
			return 0;

		NetCli_OtherChild::S_RunGlobalPlotRequest( 11 , index );
	}

	return 0;
}
int LuaFunc_CoolSuit_GetTierInfo(lua_State* L)
{
	int index = (int)lua_tonumber(L, 1);
	char temp[512]; 
	sprintf(temp,"Wardrobe_SetCost%d",index);

	int v1=65535,v2=65535,v3=65535;
	bool open=true;
	map<string,MoneyExchangeKeyValueStruct>::iterator Iter;
	Iter = ObjectDataClass::_MoneyExchangeKeyValue.find( temp );
	if( Iter == ObjectDataClass::_MoneyExchangeKeyValue.end() )
		open=false;
	else
	{
		MoneyExchangeKeyValueStruct &v= Iter->second;

		v1=v.Value[ 0 ];
		v2=v.Value[ 1 ];
		v3=v.Value[ 2 ];
	}
	lua_pushnumber(L,v1);
	lua_pushnumber(L,v2);
	lua_pushnumber(L,v3);
	lua_pushboolean(L,open);
	return 4;
}
int LuaFunc_CoolClothBag_BuyNewPage(lua_State* L)
{
	if (lua_isnumber(L, 1))
	{
		int index = (int)lua_tonumber(L, 1);

		char temp[512];
		sprintf(temp,"Wardrobe_PageCost%d", index);

		map<string,MoneyExchangeKeyValueStruct>::iterator Iter = ObjectDataClass::_MoneyExchangeKeyValue.find( temp );
		if (Iter == ObjectDataClass::_MoneyExchangeKeyValue.end())
		{
			return 0;
		}

		MoneyExchangeKeyValueStruct &v= Iter->second;
		if (v.Value[ 2 ] > 0 && !g_pGameMain->CheckSecondPassword())
			return 0;

		NetCli_OtherChild::S_RunGlobalPlotRequest( 10 , index );
	}

	return 0;
}