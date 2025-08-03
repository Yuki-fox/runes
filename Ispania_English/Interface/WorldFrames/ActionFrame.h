#pragma once

#include <string>
#include "../interface.h"

#define	ACTION_BAR_MAX_CLASS			16					// 職業各數
#define	ACTION_BAR_MAX_ITEMS			80					// 按鈕數量
#define PETACTIONBAR_MAX_SKILLS			10

class CActionFrame;
extern CActionFrame*				g_pActionFrame;

using namespace std;

enum ActionItemType {
	eActionItemType_None			= 0,					// 空的項目
	eActionItemType_Bag,									// 背包
	eActionItemType_Equipment,								// 裝備
	eActionItemType_Skill,									// 技能
	eActionItemType_Macro,									// 巨集
	eActionItemType_Emote,									// 情緒動作
	eActionItemType_Teleport,								// 傳送書
	eActionItemType_SuitSkill,								// 套裝技能
	eActionItemType_SkillPlate,								// 技能盤
	eActionItemType_HappyPartner,							//快樂的夥伴們
	eActionItemType_CoolSuit,								//酷套裝
};

// 第一版本結構
struct ActionItem
{
	int							type;
	union {
		struct {
			int					objectID;
		};
		struct {
			int					skillID;
		};
		struct {
			int					macroID;
		};
		struct {
			int					emoteID;
		};
		struct {
			int					teleportID;
		};
	};
};

// 第二版本結構(06/16/2008)
#define ACTION_ITEM_VERSION			0x02
struct ActionItem2
{
	int							type;
	union {
		int						_value[4];			// 結構大小,方便資料增加
		// 背包,裝備
		struct {
			int					objectID;			// 基本物品編號
			int					createTime;			// 當物品為不可堆疊時,產生時間識別編號
			int					serial;				// 當物品為不可堆疊時,識別編號
		};
		// 技能編號
		struct {
			int					skillID;
		};
		// 巨集編號
		struct {
			int					macroID;
		};
		// 動作編號
		struct {
			int					emoteID;
		};
		//傳送
		struct {
			int					teleportID;
		};
		//套裝技能
		struct {
			int					suitskillID;
		};
		//技能盤
		struct{
			int					skillplateID;
		};
		//夥伴們
		struct 
		{
			int					PartnerMode;
			int					PartnerPos;
		};
		//酷套裝
		struct{
			int					coolSuitIndex;
		};
	};
};

struct ActionBarSetting
{
	void Init()
	{
		isLocked = false;
		isHideEmpty = false;

		bar[0].isEnable = true;
		bar[0].count = 12;
		bar[0].row = 12;

		bar[1].isEnable = true;
		bar[1].count = 12;
		bar[1].row = 12;

		bar[2].isEnable = false;
		bar[2].count = 12;
		bar[2].row = 1;

		bar[3].isEnable = false;
		bar[3].count = 12;
		bar[3].row = 1;
	}

	union {
		int _value;
		struct {
			bool				isLocked;		// 鎖定快捷鍵
			bool				isHideEmpty;	// 隱藏空的按扭
		};		
	};

	struct {
		bool		isEnable;		// 是否啟用
		int			count;			// 總共個數
		int			row;			// 單行個數
	} bar[4];
};

struct PetActionItem
{
	float	coldownTime;		// CD更新時間
	int		skillID;			// 技能編號(1-6基本命令)
	bool	autoSpell;			// 自動施法
};

struct ExtraActionItem
{
	int buffId;					// 來源BUFF
	int buffPowerLevel;			// 來源Buff PowerLevel
	int skillId;				// 技能編號	
};

// 載具技能
struct ImplementActionItem
{
	int speller;				// 施法者
	int skillId;				// 法術編號
	int skillLevel;				// 技能等級

	bool operator == ( const ImplementActionItem& P ) const
	{
		return (P.speller == speller && P.skillId == skillId && P.skillLevel == skillLevel);
	}
};

class CActionFrame : public CInterfaceBase
{
public:
	CActionFrame(CInterface* object);
	virtual ~CActionFrame();

	void				RegisterFunc();	
	void				DestroyFrames();
	void				Update(float elapsedTime);
	void				Cut(int index);
	void				Paste(int index);
	void				Use(int index);
	ActionItem2*		GetItem(int index);
	void				UpdateCooldown(int ColdownClass, int ColdownType);

	void				DefaultAction();
	void				SaveClientData();
	void				LoadClientData(int type, int size, char* data);
	void				Save();
	void				Load();

	void				UpdatePetSkill(int petMagicID);
	PetActionItem*		GetPetSkill(int index);
	void				UsePetItem(int index, bool autoRun=false);
	void				SetPetAutoSpellState(int skillID, bool state);
	void				UpdatePetColdown(int skillID);

	void				UpdateSkillID();
	void				UpdateSkillAction(int skillId);	

	void				UpdateExtraSkill();	
	ExtraActionItem*	GetExtraItem(int index);
	void				UseExtraItem(int index);
	ExtraActionItem*	GetExtraWaitingItem();
	int					GetWaitingSkillType()					{ return m_waitingType; }
	void				ClearWaitingSpellID()					{ m_waitingType = 0; m_waitingSpellIndex = -1; }
	

	// 快捷鍵設定
	void				SetActionBarSetting(int index, bool enable, int count, int row);
	ActionBarSetting&	GetActionBarSetting()					{ return m_setting; }
	void				SetActionBarLocked(bool locked)			{ m_setting.isLocked = locked; }
	bool				GetActionBarLocked()					{ return m_setting.isLocked; }
	void				SetActionBarHideEmpty(bool hideEmpty)	{ m_setting.isHideEmpty = hideEmpty; }
	bool				GetActionBarHideEmpty()					{ return m_setting.isHideEmpty; }
	void				ResetActionBarSetting();	

	void				SaveClientSetting(bool Immediately=false);	

	// 載具技能
	void				InsertImplementAction(int speller, int skillId, int skillLevel);
	void				DeleteImplementAction(int speller, int skillId, int skillLevel);
	void				ClearImplementAction();
	void				UseImplementAction(int index);
	ImplementActionItem*	GetImplementActionItem(int index);

	// 特殊技能
	void				UseExSkill(int targetId, int x, int y, int z);

protected:
	void				SlotChanged(int index);

protected:
	ActionItem2					m_items[ACTION_BAR_MAX_CLASS][ACTION_BAR_MAX_ITEMS];
	PetActionItem				m_petItems[PETACTIONBAR_MAX_SKILLS];
	vector<ExtraActionItem>		m_extraItems;
	vector<ImplementActionItem>	m_implementItems;
	float						m_lastSaveTime;
	int							m_waitingType;					// 等待攻擊指令類型(0無, 1額外技能, 2載具)
	int							m_waitingSpellIndex;
	float						m_petAllColdown;
	ActionBarSetting			m_setting;
	ActionBarSetting			m_settingBackup;
	double						m_lastSaveSettingTime;
};