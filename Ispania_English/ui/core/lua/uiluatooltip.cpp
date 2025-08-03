#include <windows.h>
#include "../uitooltip.h"

int Tooltip_AddLine				(lua_State* L);
int Tooltip_AddDoubleLine		(lua_State* L);
int Tooltip_AddSeparator		(lua_State* L);
int Tooltip_SetOwner			(lua_State* L);
int Tooltip_IsOwned				(lua_State* L);
int Tooltip_ClearLines			(lua_State* L);
int Tooltip_SetText				(lua_State* L);
int Tooltip_SetPartnerItem		(lua_State* L);
int Tooltip_SetGameBarItem		(lua_State* L);

int Tooltip_SetRTL				(lua_State* L);

// ----------------------------------------------------------------------------------
static const struct luaL_reg g_tooltipTable [] = {		
	{"AddLine",					Tooltip_AddLine},
	{"AddDoubleLine",			Tooltip_AddDoubleLine},
	{"AddSeparator",			Tooltip_AddSeparator},
	{"SetOwner",				Tooltip_SetOwner},
	{"IsOwned",					Tooltip_IsOwned},
	{"ClearLines",				Tooltip_ClearLines},
	{"SetText",					Tooltip_SetText},	

	{"SetBankItem",				Tooltip_SetBankItem},
	{"SetEquipmentItem",		Tooltip_SetEquipmentItem},
	{"SetInventoryItem",		Tooltip_SetInventoryItem},
	{"SetSkillItem",			Tooltip_SetSkillItem},
	{"SetSkillItem_LevelUp",	Tooltip_SetSkillItem_LevelUp},
	{"SetQuestItem",			Tooltip_SetQuestItem},
	{"SetActionItem",			Tooltip_SetActionItem},
	{"SetPetActionItem",		Tooltip_SetPetActionItem},
	{"SetStoreItem",			Tooltip_SetStoreItem},
	{"SetTradePlayerItem",		Tooltip_SetTradePlayerItem},
	{"SetTradeTargetItem",		Tooltip_SetTradeTargetItem},
	{"SetInboxItem",			Tooltip_SetInboxItem},
	{"SetBagItem",				Tooltip_SetBagItem},
	{"SetPartnerItem",			Tooltip_SetPartnerItem},
	{"SetGameBarItem",			Tooltip_SetGameBarItem},
	{"SetSendMailItem",			Tooltip_SetSendMailItem},
	{"SetAuctionItem",			Tooltip_SetAuctionItem},
	{"SetAuctionBrowseItem",	Tooltip_SetAuctionBrowseItem},
	{"SetAuctionBidItem",		Tooltip_SetAuctionBidItem},
	{"SetAuctionSellItem",		Tooltip_SetAuctionSellItem},
	{"SetUnitBuff",				Tooltip_SetUnitBuff},
	{"SetUnitDebuff",			Tooltip_SetUnitDebuff},
	{"SetTitle",				Tooltip_SetTitle},
	{"SetMerchantItem",			Tooltip_SetMerchantItem},
	{"SetItemMall",				Tooltip_SetItemMall},
	{"SetShoppingCartItem",		Tooltip_SetShoppingCartItem},
	{"SetCraftItem",			Tooltip_SetCraftItem},
	{"SetCraftItemRuneHole",	Tooltip_SetCraftItemRuneHole},
	{"SetCraftRequestItem",		Tooltip_SetCraftRequestItem},
	{"SetCraftQueueRequestItem",Tooltip_SetCraftQueueRequestItem},
	{"SetLootItem",				Tooltip_SetLootItem},
	{"SetAssignItem",			Tooltip_SetAssignItem},
 	{"SetHouseItem",			Tooltip_SetHouseItem},
	{"SetBulletinBoardItem",	Tooltip_SetBulletinBoardItem},
   	{"SetHyperLink",			Tooltip_SetHyperLink},
	{"SetHistoryItem",			Tooltip_SetHistoryItem},
	{"SetAccountBagItem",		Tooltip_SetAccountBagItem},
	{"SetBootyItem",			Tooltip_SetBootyItem},
	{"SetRecipeStoreItem",		Tooltip_SetRecipeStoreItem},
	{"SetExtraActionItem",		Tooltip_SetExtraActionItem},
	{"SetPetSkillItem",			Tooltip_SetPetSkillItem},
	{"SetPetTalentSkillItem",	Tooltip_SetPetTalentSkillItem},
 	{"SetAbility",				Tooltip_SetAbility},
	{"SetPetFeedFoodItem",		Tooltip_SetPetFeedFoodItem},
	{"SetPetEquipmentItem",		Tooltip_SetPetEquipmentItem},
	{"SetPetProductItem",		Tooltip_SetPetProductItem},
	{"SetImplementActionItem",	Tooltip_SetImplementActionItem},
	{"SetFusionStone",			Tooltip_SetFusionStone},
	{"SetSuitSkillPlateItem",	Tooltip_SetSuitSkillPlateItem},
	{"SetSuitSkillEquipmentItem",	Tooltip_SetSuitSkillEquipmentItem},
	{"SetSuitSkillSkillItem",	Tooltip_SetSuitSkillSkillItem},
	{"SetSuitSkillListItem",	Tooltip_SetSuitSkillListItem},
	{"SetPetFeedFoodItemTmp",	Tooltip_SetPetFeedFoodItemTmp},
	{"SetRTL",					Tooltip_SetRTL},
	{"SetRuneExChangeItem",		Tooltip_SetRuneExChangeItem},
	{"SetATFUseItem",			Tooltip_SetATFUseItem},
	{"SetATFTitleItem",			Tooltip_SetATFTitleItem},
	{"SetATFTitleTipItem",		Tooltip_SetATFTitleTipItem},
	{"SetItemDB",				Tooltip_SetItemDB},
	{"SetMagicBaseDB",			Tooltip_SetMagicBaseDB},
	{"SetMagicCollectDB",		Tooltip_SetMagicCollectDB},

  	{NULL, NULL}
};

// ----------------------------------------------------------------------------------
void CUiLuaTooltip::OpenLib(lua_State* L)
{
	luaL_newmetatable(L, UI_LUA_TYPE_TOOLTIP);			// create new metatable
	luaL_getmetatable(L, UI_LUA_TYPE_FRAME);			// inherit layout
	lua_setmetatable(L, -2);
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);
	luaL_openlib(L, NULL, g_tooltipTable, 0);
}

// ----------------------------------------------------------------------------------
int CUiLuaTooltip::SetMetaTable(lua_State* L)
{
	if (lua_istable(L, -1))
	{
		luaL_getmetatable(L, UI_LUA_TYPE_TOOLTIP);
		lua_setmetatable(L, -2);
		return 1;
	}
	return 0;
}

// ----------------------------------------------------------------------------------
int Tooltip_AddLine(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (tooltip == NULL)
		lua_error(L);

	const char* text = (lua_isstring(L, 2)) ? lua_tostring(L, 2) : "";
	float r = (lua_isnumber(L, 3)) ? (float)lua_tonumber(L, 3) : 1.0f;
	float g = (lua_isnumber(L, 4)) ? (float)lua_tonumber(L, 4) : 1.0f;
	float b = (lua_isnumber(L, 5)) ? (float)lua_tonumber(L, 5) : 1.0f;
	tooltip->AddLine(text, r, g, b, lua_toboolean(L, 6) != 0);
	return 0;
}

// ----------------------------------------------------------------------------------
int Tooltip_AddDoubleLine(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (tooltip == NULL)
		lua_error(L);

	const char* textL = (lua_isstring(L, 2)) ? lua_tostring(L, 2) : "";
	const char* textR = (lua_isstring(L, 3)) ? lua_tostring(L, 3) : "";
	float rL = (lua_isnumber(L, 4)) ? (float)lua_tonumber(L, 4) : 1.0f;
	float gL = (lua_isnumber(L, 5)) ? (float)lua_tonumber(L, 5) : 1.0f;
	float bL = (lua_isnumber(L, 6)) ? (float)lua_tonumber(L, 6) : 1.0f;
	float rR = (lua_isnumber(L, 7)) ? (float)lua_tonumber(L, 7) : 1.0f;
	float gR = (lua_isnumber(L, 8)) ? (float)lua_tonumber(L, 8) : 1.0f;
	float bR = (lua_isnumber(L, 9)) ? (float)lua_tonumber(L, 9) : 1.0f;
	tooltip->AddDoubleLine(textL, textR, rL, gL, bL, rR, gR, bR);
	return 0;
}

// ----------------------------------------------------------------------------------
int Tooltip_AddSeparator(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (tooltip == NULL)
		lua_error(L);

	tooltip->AddSeparator();
	return 0;
}

// ----------------------------------------------------------------------------------
// stack index 1 CUiLayout Table
// stack index 2 Relative to
// stack index 3 Anchor
// stack index 4 Offset X
// stack index 5 Offset Y
int Tooltip_SetOwner(lua_State* L)
{
	CUiTooltip* tooltip = (CUiTooltip*)CUiLuaLayout::CheckLayoutTable(L, 1);
	CUiLayout* relative = NULL;
	int point = CUiAnchor::eAnchor_TopLeft;
	float x = 0.0f, y = 0.0f;

	for ( int i = 2; i <= lua_gettop(L); i++ )
	{
		switch(i)
		{
		case 2:
			switch (lua_type(L, 2))
			{
			case LUA_TTABLE:
				relative = CUiLuaLayout::CheckLayoutTable(L, 2);
				break;
			case LUA_TSTRING:
				relative = tooltip->FindByName(tooltip->TransName(luaL_checkstring(L, 2)));
				break;
			default:
				lua_error(L);
				break;
			}
			break;

		case 3:
			{
				const char* str = luaL_checkstring(L, 3);
				if ( strcmp(str, "ANCHOR_TOPLEFT") == 0 )
					point = CUiAnchor::eAnchor_TopLeft;
				else if ( strcmp(str, "ANCHOR_TOP") == 0 )
					point = CUiAnchor::eAnchor_Top;
				else if ( strcmp(str, "ANCHOR_TOPRIGHT") == 0 )
					point = CUiAnchor::eAnchor_TopRight;
				else if ( strcmp(str, "ANCHOR_LEFT") == 0 )
					point = CUiAnchor::eAnchor_Left;
				else if ( strcmp(str, "ANCHOR_CENTER") == 0 )
					point = CUiAnchor::eAnchor_Center;
				else if ( strcmp(str, "ANCHOR_RIGHT") == 0 )
					point = CUiAnchor::eAnchor_Right;
				else if ( strcmp(str, "ANCHOR_BOTTOMLEFT") == 0 )
					point = CUiAnchor::eAnchor_BottomLeft;
				else if ( strcmp(str, "ANCHOR_BOTTOM") == 0 )
					point = CUiAnchor::eAnchor_Bottom;
				else if ( strcmp(str, "ANCHOR_BOTTOMRIGHT") == 0 )
					point = CUiAnchor::eAnchor_BottomRight;
			}
			break;

		case 4:
			x = (float)luaL_checknumber(L, 4);
			break;

		case 5:
			y = (float)luaL_checknumber(L, 5);
			break;
		}
	}
	tooltip->SetOwner(relative, point, x, y);
	return 0;
}

// ----------------------------------------------------------------------------------
int Tooltip_IsOwned(lua_State* L)
{
	CUiTooltip* tooltip = (CUiTooltip*)CUiLuaLayout::CheckLayoutTable(L, 1);
	CUiLayout* owner = CUiLuaLayout::CheckLayoutTable(L, 2);
	lua_pushboolean(L, owner == tooltip->GetOwner());
	return 1;
}


// ----------------------------------------------------------------------------------
int Tooltip_ClearLines(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (tooltip == NULL)
		lua_error(L);

	tooltip->ClearLines();
	return 0;
}

// ----------------------------------------------------------------------------------
int Tooltip_SetText(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (tooltip == NULL)
		lua_error(L);

	const char* text = (lua_isstring(L, 2)) ? lua_tostring(L, 2) : "";
	float r = (lua_isnumber(L, 3)) ? (float)lua_tonumber(L, 3) : 1.0f;
	float g = (lua_isnumber(L, 4)) ? (float)lua_tonumber(L, 4) : 1.0f;
	float b = (lua_isnumber(L, 5)) ? (float)lua_tonumber(L, 5) : 1.0f;
	tooltip->SetText(text, r, g, b, lua_toboolean(L, 6) != 0);
	tooltip->SetVisible(true);
	return 0;
}

// ----------------------------------------------------------------------------------
int Tooltip_SetRTL(lua_State* L)
{
	CUiTooltip* tooltip = dynamic_cast<CUiTooltip*>( CUiLuaLayout::CheckLayoutTable(L, 1) );
	if (tooltip == NULL)
		lua_error(L);

	tooltip->SetRTL(lua_toboolean(L, 2) != 0);
	return 0;
}