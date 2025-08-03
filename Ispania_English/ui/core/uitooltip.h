#pragma once

#include <map>
#include "uiframe.h"

using namespace std;

// ----------------------------------------------------------------------------------
class CUiLuaTooltip
{
public:
	static	void			OpenLib(lua_State* L);
	static	int				SetMetaTable(lua_State* L);
};

// ----------------------------------------------------------------------------------
int	Tooltip_SetBagItem				(lua_State* L);
int	Tooltip_SetBankItem				(lua_State* L);
int	Tooltip_SetEquipmentItem		(lua_State* L);
int Tooltip_SetInventoryItem		(lua_State* L);
int	Tooltip_SetSkillItem			(lua_State* L);
int	Tooltip_SetSkillItem_LevelUp	(lua_State* L);
int Tooltip_SetActionItem			(lua_State* L);
int Tooltip_SetPetActionItem		(lua_State* L);
int Tooltip_SetStoreItem			(lua_State* L);
int	Tooltip_SetInboxItem			(lua_State* L);
int	Tooltip_SetSendMailItem			(lua_State* L);
int Tooltip_SetTradePlayerItem		(lua_State* L);
int Tooltip_SetTradeTargetItem		(lua_State* L);
int Tooltip_SetAuctionItem			(lua_State* L);
int Tooltip_SetAuctionBrowseItem	(lua_State* L);
int Tooltip_SetAuctionBidItem		(lua_State* L);
int Tooltip_SetAuctionSellItem		(lua_State* L);
int Tooltip_SetUnitBuff				(lua_State* L);
int Tooltip_SetUnitDebuff			(lua_State* L);
int Tooltip_SetQuestItem			(lua_State* L);
int Tooltip_SetTitle                (lua_State* L);
int Tooltip_SetMerchantItem			(lua_State* L);
int Tooltip_SetItemMall				(lua_State* L);
int Tooltip_SetShoppingCartItem		(lua_State* L);
int Tooltip_SetCraftItem			(lua_State* L);
int Tooltip_SetCraftItemRuneHole	(lua_State* L);
int Tooltip_SetCraftRequestItem		(lua_State* L);
int Tooltip_SetCraftQueueRequestItem(lua_State* L);
int Tooltip_SetLootItem				(lua_State* L);
int Tooltip_SetAssignItem			(lua_State* L);
int Tooltip_SetHouseItem            (lua_State* L);
int Tooltip_SetBulletinBoardItem    (lua_State* L);
int Tooltip_SetHyperLink			(lua_State* L);
int Tooltip_SetHistoryItem			(lua_State* L);
int Tooltip_SetAccountBagItem		(lua_State* L);
int Tooltip_SetBootyItem			(lua_State* L);
int Tooltip_SetRecipeStoreItem		(lua_State* L);
int Tooltip_SetExtraActionItem		(lua_State* L);
int Tooltip_SetPetSkillItem			(lua_State* L);
int Tooltip_SetPetTalentSkillItem	(lua_State* L);
int Tooltip_SetAbility				(lua_State* L);
int Tooltip_SetPetFeedFoodItem		(lua_State* L);
int Tooltip_SetPetEquipmentItem		(lua_State* L);
int Tooltip_SetPetProductItem		(lua_State* L);
int Tooltip_SetImplementActionItem	(lua_State* L);
int Tooltip_SetFusionStone			(lua_State* L);
int Tooltip_SetSuitSkillPlateItem	(lua_State* L);
int Tooltip_SetSuitSkillEquipmentItem	(lua_State* L);
int Tooltip_SetSuitSkillSkillItem	(lua_State* L);
int Tooltip_SetSuitSkillListItem	(lua_State* L);

int Tooltip_SetPetFeedFoodItemTmp	(lua_State* L);
int Tooltip_SetRuneExChangeItem		(lua_State* L);
int Tooltip_SetATFUseItem			(lua_State* L);
int Tooltip_SetATFTitleItem			(lua_State* L);
int Tooltip_SetATFTitleTipItem		(lua_State* L);

int Tooltip_SetItemDB				(lua_State* L);
int Tooltip_SetMagicBaseDB			(lua_State* L);
int Tooltip_SetMagicCollectDB		(lua_State* L);



// ----------------------------------------------------------------------------------
class CUiTooltip : public CUiFrame
{
public:
	CUiTooltip(const char* name, CUiLayout* pParent, CUi* pUi);
	virtual ~CUiTooltip();

	virtual	void			SetLuaMetaTable(lua_State* l)			{ CUiLuaTooltip::SetMetaTable(l); }
	virtual	void			ClearLines();
	virtual	void			AddDoubleLine(const wchar_t* textL, const wchar_t* textR, float rL, float gL, float bL, float rR, float gR, float bR );
	virtual	void			AddDoubleLine(const char* textL, const char* textR, float rL, float gL, float bL, float rR, float gR, float bR );
	virtual	void			AddLine(const wchar_t* text, float r, float g, float b, bool warp=false, int tier=0);
	virtual	void			AddLine(const char* text, float r, float g, float b, bool warp=false, int tier=0);
	virtual void			AddSeparator();
	virtual	void			SetText(const wchar_t* text, float r, float g, float b, bool warp=false);
	virtual	void			SetText(const char* text, float r, float g, float b, bool warp=false);
	virtual	void			SetOwner(CUiLayout* pUiLayout, int anchor, float xOff, float yOff);
	virtual	CUiLayout*		GetOwner()								{ return m_pOwner; }	
	virtual void			SetRTL(bool isRTL)						{ m_isRTL = isRTL; }
	virtual bool			IsRTL()									{ return m_isRTL; }
	virtual int				GetMemSize();

protected:
	virtual CUiLayout*		GetLine(int index, bool right = false);
	virtual void			Anchor(CUiLayout* layout, int index);
	virtual void			OnHide(lua_State* L);
	virtual	void			TooltipUpdate(float width);

protected:
	CUiLayout*				m_pOwner;					// ¹ï»ô¤¸¥ó
	int						m_lines;
	float					m_maxWidth;
	bool					m_isRTL;
	bool					m_warp[100];
	map<int, int>			m_separator;
};