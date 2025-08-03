#pragma once

#include <map>
#include "../interface.h"

class CGameTooltip;
extern CGameTooltip*				g_pGameTooltip;

using namespace std;

enum GameTooltipType
{
	eGameTooltip_None				= 0,
	eGameTooltip_Bag				,
	eGameTooltip_Bank				,
	eGameTooltip_Equipment			,
	eGameTooltip_Skill				,
	eGameTooltip_Action				,
	eGameTooltip_Store				,
	eGameTooltip_Inbox				,
	eGameTooltip_SendMailItem		,
	eGameTooltip_TradePlayer		,
	eGameTooltip_TradeTarget		,
	eGameTooltip_Auction			,
	eGameTooltip_AuctionBrowse		,
	eGameTooltip_AuctionBid			,
	eGameTooltip_AuctionSell		,
	eGameTooltip_UnitBuff			,
	eGameTooltip_UnitDebuff			,
	eGameTooltip_QuestItem			,
	eGameTooltip_Inventory			,
	eGameTooltip_Merchant			,
	eGameTooltip_Merchant2			,
	eGameTooltip_ShoppingCart		,
	eGameTooltip_LootItem			,
	eGameTooltip_RecipeStore		,
	eGameTooltip_SuitSkill			,
	eGameTooltip_HappyPartner		,
	eGameTooltip_RuneExChange		,
	eGameTooltip_ATFEffect			,
	eGameTooltip_GameBar			,

};

class CGameTooltip : public CInterfaceBase
{
public:
	CGameTooltip(CInterface* object);
	virtual ~CGameTooltip();
	
	virtual	void RegisterFunc();
	virtual	void Load();
	virtual void DestroyFrames();
	virtual	void SetTooltip( GameTooltipType type, CUiTooltip* tooltip, int index, const char* unit );
	
	
	void Update(float elapsedTime);
   	void SetSysTooltips( CUiTooltip* tooltip, int index );
	bool TooltipSetItem( CUiTooltip* tooltip, bool equip, ItemFieldStruct* item, GameObjDbStructEx* itemDB, bool compareSelf=true );
	void SetHyperLink( CUiTooltip *tooltip,const char* link );
	void Clear()									{ memset(m_unit, 0, sizeof(m_unit)); m_index = -1; }	
	void SetNeedUpdateTime(bool boUpdate)			{ m_boUpdate = boUpdate; }
	
protected:	
	CUiTooltip*			m_gameTooltip;
	CUiTooltip*			m_SysTooltip[2];
	GameTooltipType		m_type;

	int					m_index;
	char				m_unit[80];
	bool				m_boUpdate;
};