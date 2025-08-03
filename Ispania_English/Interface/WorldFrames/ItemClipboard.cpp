#include "../netwaker_member/NetWakerClientInc.h"
#include "../../mainproc/GameMain.h"
#include "../Houses/HousesFrame.h"
#include "BagFrame.h"
#include "ItemClipboard.h"

CItemClipboard*				g_pItemClipboard	= NULL;

int LuaFunc_ItemClipboard_Clear			(lua_State* L);
int LuaFunc_CursorItemType				(lua_State* L);
int LuaFunc_CursorHasItem				(lua_State* L);
int LuaFunc_DeleteCursorItem			(lua_State* L);
int LuaFunc_GetCursorItemInfo			(lua_State* L);

// ----------------------------------------------------------------------------
CItemClipboard::CItemClipboard(CInterface* object)
: CInterfaceBase(object)
{
	m_type = eClipboardItem_Type_None;	
	m_file.clear();
	m_name.clear();

	g_pItemClipboard	= this;
}

// ----------------------------------------------------------------------------
CItemClipboard::~CItemClipboard()
{
	g_pItemClipboard	= NULL;
}

// ----------------------------------------------------------------------------
void CItemClipboard::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{
		lua_register(L, "ItemClipboard_Clear",			LuaFunc_ItemClipboard_Clear);
		lua_register(L, "CursorItemType",				LuaFunc_CursorItemType);
		lua_register(L, "CursorHasItem",				LuaFunc_CursorHasItem);
		lua_register(L, "DeleteCursorItem",				LuaFunc_DeleteCursorItem);
		lua_register(L, "GetCursorItemInfo",			LuaFunc_GetCursorItemInfo);
	}
}

// ----------------------------------------------------------------------------
void CItemClipboard::Update(float elapsedTime)
{
	ItemFieldStruct* item = NULL;

	switch(m_type)
	{
	case eClipboardItem_Type_Bag:
		if ( (item = CNetGlobal::RoleData()->GetBodyItem(m_itemInfo.bag.pos)) != NULL )
			item->Pos = EM_ItemState_CliDisabled;
		break;

	case eClipboardItem_Type_Equipment:
		if ( (item = CNetGlobal::RoleData()->GetEqItem(m_itemInfo.equipment.pos)) != NULL )
			item->Pos = EM_ItemState_CliDisabled;
		break;
	case eClipboardItem_Type_Teleport:
		break;
	case eClipboardItem_Type_Bank:
	case eClipboardItem_Type_Trade:
	case eClipboardItem_Type_Action:
	case eClipboardItem_Type_Skill:
	case eClipboardItem_Type_SuitSkillequip:
	case eClipboardItem_Type_Shop:
	case eClipboardItem_Type_Macro:
	case eClipboardItem_Type_Emote:
	case eClipboardItem_Type_SuitSkill:
	case eClipboardItem_Type_House:	
	case eClipboardItem_Type_CoolClothBag:
	case eClipboardItem_Type_CoolSuit:
		break;
	}
}

// ----------------------------------------------------------------------------
void CItemClipboard::Clear()
{
	// �M��ICON����,�@�묰��U�ʧ@
	if ( m_type != eClipboardItem_Type_None ) {
		g_pRuGlobalEventManager->PlaySoundByFile("sound\\interface\\ui_icon_drop.mp3", false, 0.0f, ruSOUNDTYPE_INTERFACE);
	}	

	m_type		= eClipboardItem_Type_None;	
	m_file.clear();
	m_name.clear();	

	UpdateItemInfo();
}

// ----------------------------------------------------------------------------
void CItemClipboard::Unlock()
{
	// �ˬd�쪫�~�O�_���������A
	ItemFieldStruct* item = NULL;
	switch (m_type)
	{
	case eClipboardItem_Type_Bag:
		if ( (item = CNetGlobal::RoleData()->GetBodyItem(m_itemInfo.bag.pos)) != NULL ) {
			item->Pos = EM_ItemState_NONE;
		}
		break;

	case eClipboardItem_Type_Equipment:
		if ( (item = CNetGlobal::RoleData()->GetEqItem(m_itemInfo.equipment.pos)) != NULL ) {
			item->Pos = EM_ItemState_NONE;
		}
		break;

	case eClipboardItem_Type_Bank:
		if ( (item = CNetGlobal::RoleData()->GetBankItem(m_itemInfo.bank.pos)) != NULL ) {
			item->Pos = EM_ItemState_NONE;
		}
		break;

	case eClipboardItem_Type_Trade:
	case eClipboardItem_Type_Action:
	case eClipboardItem_Type_Skill:
	case eClipboardItem_Type_Shop:
	case eClipboardItem_Type_Macro:
	case eClipboardItem_Type_Emote:
	case eClipboardItem_Type_House:
	case eClipboardItem_Type_SuitSkill:
	case eClipboardItem_Type_SuitSkillequip:
	case eClipboardItem_Type_Teleport:
	case eClipboardItem_Type_CoolClothBag:
	case eClipboardItem_Type_CoolSuit:
	case eClipboardItem_Type_HappyPartner:
		break;
	}
}

// ----------------------------------------------------------------------------
void CItemClipboard::SetName(const char* name)
{
	m_name.clear();
	if ( name )
		m_name = name;
}

// ----------------------------------------------------------------------------
void CItemClipboard::SetFile(const char* file)
{
	m_file.clear();
	if ( file )
		m_file = file;
}

// ----------------------------------------------------------------------------
void CItemClipboard::SetFileFromImageID(int id)
{
	GameObjDbStruct* image = CNetGlobal::GetObj(id);	
	if ( image )
		SetFile(image->Image.ACTField);
	else
		SetFile("");
}
// ----------------------------------------------------------------------------
void CItemClipboard::PickupPartner(int Mode, int pos)
{
	m_type					= eClipboardItem_Type_HappyPartner;
	m_itemInfo.partner.mode	= Mode;
	m_itemInfo.partner.pos		= pos;

	UpdateItemInfo();
}

// ----------------------------------------------------------------------------
void CItemClipboard::PickupBag(int itemID, int pos)
{
	m_type					= eClipboardItem_Type_Bag;
	m_itemInfo.bag.itemID	= itemID;
	m_itemInfo.bag.pos		= pos;

	UpdateItemInfo();
}

// ----------------------------------------------------------------------------
void CItemClipboard::PickupSplitBagItem( int itemID, int pos, int count )
{
	m_type                         = eClipboardItem_Type_SplitBagItem;
	m_itemInfo.splitBagItem.itemID = itemID;
	m_itemInfo.splitBagItem.pos    = pos;
  	m_itemInfo.splitBagItem.count  = count;

	UpdateItemInfo();
}


// ----------------------------------------------------------------------------
void CItemClipboard::PickupBank(int itemID, int pos)
{
	m_type					= eClipboardItem_Type_Bank;
	m_itemInfo.bank.itemID	= itemID;
	m_itemInfo.bank.pos		= pos;

	UpdateItemInfo();
}


// ----------------------------------------------------------------------------
void CItemClipboard::PickupEquipment(int itemID, int pos)
{
	m_type						= eClipboardItem_Type_Equipment;	
	m_itemInfo.equipment.itemID	= itemID;
	m_itemInfo.equipment.pos	= pos;

	UpdateItemInfo();
}

// ----------------------------------------------------------------------------
void CItemClipboard::PickupAction(int actionType, int id, int arg1, int arg2)
{
	m_type						= eClipboardItem_Type_Action;
	m_itemInfo.action.type		= actionType;
	m_itemInfo.action.id		= id;
	m_itemInfo.action.arg[0]	= arg1;
	m_itemInfo.action.arg[1]	= arg2;

	UpdateItemInfo();
}

// ----------------------------------------------------------------------------
void CItemClipboard::PickupTrade(int tradeType, int pos, int id)
{
	m_type					= eClipboardItem_Type_Trade;	
	m_itemInfo.trade.type	= tradeType;
	m_itemInfo.trade.pos	= pos;
	m_itemInfo.trade.id		= id;

	UpdateItemInfo();
}

// ----------------------------------------------------------------------------
void CItemClipboard::PickupSkill(int skillID )
{	
	m_type						= eClipboardItem_Type_Skill;
	m_itemInfo.skill.skillID	= skillID;

	UpdateItemInfo();
}


// ----------------------------------------------------------------------------
void CItemClipboard::PickupShop(int shopType, int pos)
{
	m_type					= eClipboardItem_Type_Shop;
	m_itemInfo.shop.type	= shopType;
	m_itemInfo.shop.pos		= pos;

	UpdateItemInfo();
}

// ----------------------------------------------------------------------------
void CItemClipboard::PickupMacro(int id)
{
	m_type					= eClipboardItem_Type_Macro;
	m_itemInfo.macro.id		= id;

	UpdateItemInfo();
}

// ----------------------------------------------------------------------------
void CItemClipboard::PickupEmote(int id)
{
	m_type					= eClipboardItem_Type_Emote;
	m_itemInfo.emote.id		= id;
	UpdateItemInfo();
}

// ----------------------------------------------------------------------------
void CItemClipboard::PickupHouse(int itemID, int parentID, int pos)
{
	m_type                    = eClipboardItem_Type_House;
	m_itemInfo.house.itemID	  = itemID;
  	m_itemInfo.house.parentID = parentID;
	m_itemInfo.house.pos      = pos;
	UpdateItemInfo();
}
void CItemClipboard::PickupTeleport		(int index)
{
	m_type                    = eClipboardItem_Type_Teleport;
	m_itemInfo.teleport.pos	  = index;
	UpdateItemInfo();
}

// ----------------------------------------------------------------------------
void CItemClipboard::PickupGuildBank(int itemID, int pos)
{
	m_type					= eClipboardItem_Type_GuildBank;
	m_itemInfo.guildbank.itemID	= itemID;
	m_itemInfo.guildbank.pos	= pos;

	UpdateItemInfo();
}

// ----------------------------------------------------------------------------
void CItemClipboard::PickupGuildFurniture(int itemID, int pos)
{
	m_type								= eClipboardItem_Type_GuildFurniture;
	m_itemInfo.guildfurniture.itemID	= itemID;
	m_itemInfo.guildfurniture.pos		= pos;

	UpdateItemInfo();
}

// ----------------------------------------------------------------------------
void CItemClipboard::PickupPetEQ(int index, int pos)
{
	m_type					= eClipboardItem_Type_PetEQ;
	m_itemInfo.petEQ.index	= index;
	m_itemInfo.petEQ.pos	= pos;
}
// ----------------------------------------------------------------------------
void CItemClipboard::PickupSuitSkill(int skillID )
{	
	m_type							= eClipboardItem_Type_SuitSkill;
	m_itemInfo.SuitSkill.skillID	= skillID;

	UpdateItemInfo();
}
// ----------------------------------------------------------------------------
void CItemClipboard::PickupSkillPlate(int skillID , int pos)
{	
	m_type							= eClipboardItem_Type_SkillPlate;
	m_itemInfo.SkillPlate.skillID	= skillID;
	m_itemInfo.SkillPlate.pos		= pos ; 
	UpdateItemInfo();
}

// ----------------------------------------------------------------------------
void CItemClipboard::PickupCoolClothBag	(int index)
{
	m_type							= eClipboardItem_Type_CoolClothBag;
	m_itemInfo.CoolClothBag.index	= index;
	UpdateItemInfo();
}

// ----------------------------------------------------------------------------
void CItemClipboard::PickupCoolSuit	(int index)
{
	m_type						= eClipboardItem_Type_CoolSuit;
	m_itemInfo.CoolSuit.index	= index;
	UpdateItemInfo();
}

//----------------------------------------------------------------------------
void CItemClipboard::PickupSuitSkillequip	(int itemID , int pos )
{
	m_type								= eClipboardItem_Type_SuitSkillequip;
	m_itemInfo.SuitSkillequip.itemID	= itemID;
	m_itemInfo.SuitSkillequip.pos		= pos;
	UpdateItemInfo();
}

// ----------------------------------------------------------------------------
void CItemClipboard::UpdateItemInfo()
{
	if( m_type != eClipboardItem_Type_None )
	{
		g_pRuGlobalEventManager->PlaySoundByFile("sound\\interface\\ui_icon_pickup.mp3", false, 0.0f, ruSOUNDTYPE_INTERFACE);
	}

	SendWorldEvent("CURSOR_ITEM_UPDATE");
}

// ----------------------------------------------------------------------------
const char* CItemClipboard::GetItemTypeString(ClipboardItemType itemType)
{
	switch (itemType)
	{
	case eClipboardItem_Type_Bag:					// �I�]
		return "bag";
	case eClipboardItem_Type_SplitBagItem:			// �]�]����
		return "bag";
	case eClipboardItem_Type_Bank:					// �Ȧ�
		return "bank";
	case eClipboardItem_Type_Equipment:				// �˳�
		return "equip";
	case eClipboardItem_Type_Trade:					// ���
		return "trade";
	case eClipboardItem_Type_Action:				// �ֱ�
		return "action";
	case eClipboardItem_Type_Skill:					// �ޯ�
		return "skill";
	case eClipboardItem_Type_Shop:					// �ӫ~
		return "shop";
	case eClipboardItem_Type_Macro:					// ����
		return "macro";
	case eClipboardItem_Type_Emote:					// �����ʧ@
		return "emote";
	case eClipboardItem_Type_Merchant:				// �I�ưӫ�
		return "merchant";
	case eClipboardItem_Type_House:					// �ЫΨt�� �a��C����_�c
		return "house";
	case eClipboardItem_Type_Teleport:				// �ǰe�� �ǰe�I
		return "teleport";
	case eClipboardItem_Type_GuildBank:				// ���|�Ȧ�
		return "guildbank";
	case eClipboardItem_Type_GuildFurniture:		// ���|�ήa��
		return "guildfurniture";
	case eClipboardItem_Type_PetEQ:					// �d���˳�
		return "petItem";
	case eClipboardItem_Type_SuitSkill:				//�M�˧ޯ�
		return "SuitSkill";
	case eClipboardItem_Type_SkillPlate:
		return "SkillPlate";
	case eClipboardItem_Type_SuitSkillequip:			//�M�˧ޯ�˳�
		return "SuitSkillequip";
	case eClipboardItem_Type_CoolClothBag:			//�Ÿ˥]
		return "CoolClothBag";
	case eClipboardItem_Type_CoolSuit:				//�ŮM��
		return "CoolSuit";
	case eClipboardItem_Type_HappyPartner:			//�٦� �d�� �y�M
		return "HappyPartner";
	}
	return "";
}

// ----------------------------------------------------------------------------
int LuaFunc_ItemClipboard_Clear(lua_State* L)
{
	if (!g_pItemClipboard->IsEmpty())
		g_pItemClipboard->Clear();

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_CursorItemType(lua_State* L)
{
	switch (g_pItemClipboard->GetType())
	{
	case eClipboardItem_Type_Bag:					// �I�]
		lua_pushstring(L, "bag");
		return 1;
	case eClipboardItem_Type_SplitBagItem:			// �]�]����
		lua_pushstring(L, "bag");
		return 1;
	case eClipboardItem_Type_Bank:					// �Ȧ�
		lua_pushstring(L, "bank");
		return 1;
	case eClipboardItem_Type_Equipment:				// �˳�
		lua_pushstring(L, "equip");
		return 1;
	case eClipboardItem_Type_Trade:					// ���
		lua_pushstring(L, "trade");
		return 1;
	case eClipboardItem_Type_Action:				// �ֱ�
		lua_pushstring(L, "action");
		return 1;
	case eClipboardItem_Type_Skill:					// �ޯ�
		lua_pushstring(L, "skill");
		return 1;
	case eClipboardItem_Type_Shop:					// �ӫ~
		lua_pushstring(L, "shop");
		return 1;
	case eClipboardItem_Type_Macro:					// ����
		lua_pushstring(L, "macro");
		return 1;
	case eClipboardItem_Type_Emote:					// �����ʧ@
		lua_pushstring(L, "emote");
		return 1;
	case eClipboardItem_Type_Merchant:				// �I�ưӫ�
		lua_pushstring(L, "merchant");
		return 1;
	case eClipboardItem_Type_House:					// �ЫΨt�� �a��C����_�c
		lua_pushstring(L, "house");
		return 1;
	case eClipboardItem_Type_Teleport:				// �ǰe�� �ǰe�I
		lua_pushstring(L, "teleport");
		return 1;
	case eClipboardItem_Type_GuildBank:				// ���|�Ȧ�
		lua_pushstring(L, "guildbank");
		return 1;
	case eClipboardItem_Type_GuildFurniture:		// ���|�Ȧ�
		lua_pushstring(L, "guildfurniture");
		return 1;
	case eClipboardItem_Type_SuitSkill:				//�M�˧ޯ�
		lua_pushstring(L, "SuitSkill");
		return 1;
	case eClipboardItem_Type_SkillPlate:			//�ޯ�L
		lua_pushstring(L, "SkillPlate");
		return 1;
	case eClipboardItem_Type_SuitSkillequip:		//�M�˧ޯ�˳�
		lua_pushstring(L, "SuitSkillequip");
		return 1;
	case eClipboardItem_Type_CoolClothBag:		// �Ÿ˥]
		lua_pushstring(L, "CoolClothBag");
		return 1;
	case eClipboardItem_Type_CoolSuit:		// �Ÿ˥]
		lua_pushstring(L, "CoolSuit");
		return 1;
	case eClipboardItem_Type_HappyPartner:			//�٦� �d�� �y�M
		lua_pushstring(L, "HappyPartner");
		return 1;
	}

	// �ˬd�I�]�ϥΪ��~����
	int index = g_pBagFrame->GetUseItemIndex();
	if ( index != 0 )
	{
		ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(index);
		if ( item )
		{
			GameObjDbStruct* itemDB = g_ObjectData->GetObj(item->OrgObjID);
			if ( itemDB->Item.ItemType == EM_ItemType_Pet )
			{
				lua_pushstring(L, "petegg");
				return 1;
			}
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_CursorHasItem(lua_State* L)
{
	lua_pushboolean(L, g_pItemClipboard->GetType() != eClipboardItem_Type_None);
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_DeleteCursorItem(lua_State* L)
{
	ClipboardItemInfo itemInfo = g_pItemClipboard->GetItemInfo();

	ItemFieldStruct* pItemField = NULL;
	switch (g_pItemClipboard->GetType())
	{
	case eClipboardItem_Type_Bag:
		if ( (pItemField = CNetGlobal::RoleData()->GetBodyItem(itemInfo.bag.pos)) != NULL )
			NetCli_Item::_DeleteItem_Body(*pItemField, itemInfo.bag.pos);
		break;

	case eClipboardItem_Type_Bank:
		if ( (pItemField = CNetGlobal::RoleData()->GetBankItem(itemInfo.bank.pos)) != NULL )
			NetCli_Item::_DeleteItem_Bank(*pItemField, itemInfo.bank.pos);
		break;

	case eClipboardItem_Type_Equipment:
		if ( (pItemField = CNetGlobal::RoleData()->GetEqItem(itemInfo.equipment.pos)) != NULL )
			NetCli_Item::_DeleteItem_EQ(*pItemField, itemInfo.equipment.pos);
		break;
	case eClipboardItem_Type_House:
		{
			HouseItemDBStruct* pHouseItemDB = g_pHousesFrame->GetHousesItemDB( itemInfo.house.parentID , itemInfo.house.pos );
			if( pHouseItemDB )
			{
				NetCli_HousesChild::S_DestroyItemRequest( pHouseItemDB->ItemDBID );
			}
		}
		break;
	case eClipboardItem_Type_CoolClothBag:
		{
			int index = itemInfo.CoolClothBag.index;
			if( index < 0 || index >= _DEF_MAX_COOL_CLOTH_COUNT_ )
			{
				return 0;
			}

			PlayerBaseRoleData* PlayerBaseData = CNetGlobal::RoleData()->PlayerBaseData;
			int ImageObjectID = PlayerBaseData->CoolClothList[index];
			NetCli_ItemChild::DelCoolClothRequest ( index , ImageObjectID );
		}
		break;
	}	
	g_pItemClipboard->Clear();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetCursorItemInfo(lua_State* L)
{
	ClipboardItemInfo itemInfo = g_pItemClipboard->GetItemInfo();	

	switch (g_pItemClipboard->GetType())
	{
	case eClipboardItem_Type_Bag:					// �I�]
		lua_pushnumber(L, itemInfo.bag.pos + 1);
		return 1;
	case eClipboardItem_Type_SplitBagItem:			// �]�]����		
		lua_pushnumber(L, itemInfo.splitBagItem.pos + 1);
		return 1;
	case eClipboardItem_Type_Bank:					// �Ȧ�
		lua_pushnumber(L, itemInfo.bank.pos + 1);
		return 1;
	case eClipboardItem_Type_Equipment:				// �˳�		
		lua_pushnumber(L, itemInfo.equipment.pos + 1);
		return 1;
	case eClipboardItem_Type_Trade:					// ���
		lua_pushnumber(L, itemInfo.trade.pos + 1);
		return 1;
	case eClipboardItem_Type_Action:				// �ֱ�
		lua_pushnumber(L, itemInfo.action.id + 1);
		return 1;
	case eClipboardItem_Type_Skill:					// �ޯ�
		return 0;
	case eClipboardItem_Type_Shop:					// �ӫ~
		lua_pushnumber(L, itemInfo.shop.pos + 1);
		return 1;
	case eClipboardItem_Type_Macro:					// ����
		lua_pushnumber(L, itemInfo.macro.id + 1);
		return 1;
	case eClipboardItem_Type_Emote:					// �����ʧ@
		lua_pushnumber(L, itemInfo.emote.id + 1);
		return 1;
	case eClipboardItem_Type_Merchant:				// �I�ưӫ�		
		return 0;
	case eClipboardItem_Type_House:					// �ЫΨt�� �a��C����_�c
		return 0;
	case eClipboardItem_Type_Teleport:				// �ǰe�� �ǰe�I
		return 0;

	case eClipboardItem_Type_GuildBank:				// ���|�Ȧ�
		return 0;
	case eClipboardItem_Type_GuildFurniture:		// ���|�Ȧ�
		return 0;
	case eClipboardItem_Type_SuitSkill:				//�M�˧ޯ�
		return 0;
	case eClipboardItem_Type_SuitSkillequip:		//�M�˧ޯ�˳�
		return 0;
	case eClipboardItem_Type_SkillPlate:			//�ޯ�L
		return 0;
	case eClipboardItem_Type_HappyPartner:			//�٦� �d�� �y�M
		return 0;
	}
	return 0;
}