#pragma once

#include <string>
#include "../interface.h"

class CItemClipboard;
extern CItemClipboard*			g_pItemClipboard;

using namespace std;

enum ClipboardItemType {
	eClipboardItem_Type_None			= 0,
	eClipboardItem_Type_Bag,					// 背包
	eClipboardItem_Type_SplitBagItem,			// 包包分堆
	eClipboardItem_Type_Bank,					// 銀行
	eClipboardItem_Type_Equipment,				// 裝備
	eClipboardItem_Type_Trade,					// 交易
	eClipboardItem_Type_Action,					// 快捷
	eClipboardItem_Type_Skill,					// 技能
	eClipboardItem_Type_Shop,					// 商品
	eClipboardItem_Type_Macro,					// 巨集
	eClipboardItem_Type_Emote,					// 情緒動作
	eClipboardItem_Type_Merchant,				// 點數商城
	eClipboardItem_Type_House,					// 房屋系統 家具列表或寶箱
	eClipboardItem_Type_Teleport,				// 傳送書 傳送點
	eClipboardItem_Type_GuildBank,				// 公會銀行
	eClipboardItem_Type_GuildFurniture,			// 公會屋家具
	eClipboardItem_Type_PetEQ,					// 寵物裝備
	eClipboardItem_Type_SuitSkill,				// 套裝技能
	eClipboardItem_Type_SuitSkillequip,			// 技能粹取
	eClipboardItem_Type_SkillPlate,				// 技能盤
	eClipboardItem_Type_CoolClothBag,			// 酷裝包
 	eClipboardItem_Type_CoolSuit,			    // 酷套裝
	eClipboardItem_Type_HappyPartner,           //快樂的夥伴們?
};

typedef union 
{	 
	struct {
		int	itemID;							// 物品編號
		int	pos;							// 背包索引編號
		int count;							// 物品個數(-1)代表全部
	}bag;

	struct {
		int	itemID;							// 物品編號
		int	pos;							// 背包索引編號
		int count;							// 物品個數(-1)代表全部
	}splitBagItem;

	struct {
		int	itemID;
		int	pos;
	}bank;	

	struct {
		int	itemID;
		int	pos;
	}equipment;

	struct {
		int	type;
		int	id;
		int arg[2];						// 物品唯一編號(建立時間,與序列號)
	}action;

	struct {
		int	type;
		int pos;
		int id;
	}trade;

	struct {
		int	skillID;
	}skill;

	struct {
		int type;
		int pos;
	}shop;

	struct {
		int id;
	}macro;

	struct {
		int id;
	}emote;

	struct {
		int	itemID;
		int parentID;
		int	pos;
	}house;

	struct {	
		int	pos;
	}teleport;

	struct {
		int	itemID;
		int	pos;
	}guildbank;

	struct {
		int	itemID;
		int	pos;
	}guildfurniture;

	struct {
		int index;						// 寵物編號
		int pos;						// 裝備代碼
	}petEQ;

	struct {
		int skillID;					// 技能編碼
	}SuitSkill;

	struct {
		int itemID;					// 技能粹取
		int pos;
	}SuitSkillequip;

	struct {
		int skillID;				// 技能盤
		int pos;
	}SkillPlate;

	struct {						 // 酷裝包
		int index;
	}CoolClothBag;

	struct {						 // 酷套裝
		int index;
	}CoolSuit;

	struct  //夥伴
	{	
		int mode;
		int	pos;
	}partner;


}ClipboardItemInfo;

class CItemClipboard : public CInterfaceBase
{
public:
	CItemClipboard(CInterface* object);
	virtual ~CItemClipboard();

	virtual	void					RegisterFunc		();
	virtual	void					Update				(float elapsedTime);
	virtual	void					Clear				();
	virtual void					Unlock				();

	virtual	void					SetName				(const char* name);
	virtual	void					SetFile				(const char* file);
	virtual void					SetFileFromImageID	(int id);
	virtual	void					PickupBag			(int itemID, int pos);
	virtual void					PickupSplitBagItem  (int itemID, int pos, int count);
	virtual	void					PickupBank			(int itemID, int pos);
	virtual	void					PickupEquipment		(int itemID, int pos);
	virtual	void					PickupAction		(int actionType, int id, int arg1=0, int arg2=0);
	virtual	void					PickupTrade			(int tradeType, int pos, int id);
	virtual void					PickupSkill			(int skillID );
	virtual	void					PickupShop			(int shopType, int pos);
	virtual	void					PickupMacro			(int id);
	virtual	void					PickupEmote			(int id);
 	virtual	void					PickupHouse			(int itemID, int parentID, int pos);
	void							PickupTeleport		(int index);
	void							PickupPartner		(int mode,int index);
	virtual	void					PickupGuildBank		(int itemID, int pos);
	virtual	void					PickupGuildFurniture(int itemID, int pos);
	virtual void					PickupPetEQ			(int index, int pos);
	virtual void					PickupSuitSkill		(int skillID );
	virtual void					PickupSuitSkillequip(int itemID, int pos);
	virtual void					PickupSkillPlate	(int skillID, int pos);
 	virtual void					PickupCoolClothBag	(int index);
	virtual void					PickupCoolSuit		(int index);


	virtual	ClipboardItemType		GetType				()						{ return m_type; }
	virtual ClipboardItemInfo		GetItemInfo			()						{ return m_itemInfo; }
	virtual	const char*				GetName				()						{ return m_name.c_str(); }
	virtual	const char*				GetFile				()						{ return m_file.c_str(); }
	virtual	bool					IsEmpty				()						{ return m_type == eClipboardItem_Type_None; }

	static const char*				GetItemTypeString	(ClipboardItemType itemType);

protected:
	virtual void					UpdateItemInfo();

protected:
	ClipboardItemType			m_type;							// 來源物件類型
	ClipboardItemInfo			m_itemInfo;						// 物品資料
	string						m_file;							// Icon 圖形資料
	string						m_name;							// 名稱
};