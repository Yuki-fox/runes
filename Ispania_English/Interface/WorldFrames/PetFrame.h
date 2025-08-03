#pragma once

#include "../interface.h"
#include "../NetWaker_Member/net_cultivatepet/NetCli_CultivatePet.h"

#define PET_SUMMON_LEVEL_LIMIT	5	// 寵物超過玩家 PET_SUMMON_LEVEL_LIMIT 級，會無法召喚
#define PET_SUMMON_COST_TIME	5	// 寵物召喚必須 PET_SUMMON_COST_TIME 秒，才能召喚出來
#define PET_MARGE_COUNT_MAX		3	

class CPetFrame;
extern CPetFrame*				g_pPetFrame;

enum PetLearnSkillResult
{
	ePetLearnSkill_Success		= 0,					// 成
	ePetLearnSkill_Level		= -1,					// 等級不足
	ePetLearnSkill_KeyItem		= -2,					// 缺少重要物品
	ePetLearnSkill_Point		= -3,					// 技能點數不足
	ePetLearnSkill_STR			= -4,					// STR不足
	ePetLearnSkill_STA			= -5,					// STA不足
	ePetLearnSkill_INT			= -6,					// INT不足
	ePetLearnSkill_MND			= -7,					// MND不足
	ePetLearnSkill_AGI			= -8,					// AGI不足
	ePetLearnSkill_Already		= -9,					// 已經最高級了
	ePetLearnSkill_Unknow		= -10,					// 未知錯誤
};

// 單筆技能資訊
struct PetMagicSkillInfo
{
	void Init()
	{
		memset(this, 0, sizeof(PetMagicSkillInfo));
	}

	// 需求限制
	struct
	{
		int Level;				// 寵物等級
		int Str;				// 力量
		int Sta;				// 耐力
		int Int;				// 智慧
		int Mnd;				// 精神
		int Agi;				// 敏捷
		int Point;				// 需求技能點數
		int KeyItem;			// 需求重要物品編號
		int Lock;				
	} Limit;

	bool IsLearn;				// 是否已經學會
	bool IsTopLevel;			// 是否為最高級別
	int MagicID;				// 法術編號
	int PowerLevel;				// 目前等級
};

// 寵物食物數量
struct PetFoodCountInfo
{
	int count;
	int ID; 
	int type;   // 0 ItemEx  1 ItemMagicBox  2 ItemBase
	int pos;
};


// 每一隻寵物所擁有的技能
struct PetMagicSkillList
{
	vector<PetMagicSkillInfo>	item;

	void Clear()
	{
		item.clear();
	}

	void Add(PetMagicSkillInfo& info)
	{
		item.push_back(info);
	}

	int Size()
	{
		return  (int)item.size();
	}

	PetMagicSkillInfo* GetAtIndex(int index)
	{
		if ( index >= 0 && index < (int)item.size() )
			return &item[index];
		return NULL;
	}
};


class CPetFrame : public CInterfaceBase
{
public:
	CPetFrame(CInterface* object);
	virtual ~CPetFrame();

	static CultivatePetEQTypeENUM	GetPetEQTypeENUM(const char* key);
	static const char*				GetPetEQTypeKeyString(CultivatePetEQTypeENUM type);
	
	virtual	void				RegisterFunc();
	virtual	void				BuildFrames();
	virtual void				Update(float elapsedTime);

	virtual void				StopSummonPet();
	virtual bool				IsSummoningPet();

	virtual void				SetTakeoutIndex(int index)		{ m_takeoutIndex = index; }
	virtual	int					GetTakeoutIndex()				{ return m_takeoutIndex; }

	virtual void				SetUpdateNameIndex(int index)	{ m_updateNameIndex = index; }
	virtual int					GetUpdateNameIndex()			{ return m_updateNameIndex; }

	virtual void				SetLearnNeedPoint(int point)	{ m_learnNeedPoint = point; }
	virtual int					GetLearnNeedPoint()				{ return m_learnNeedPoint; }

	virtual void				LearnMagicSkill(int index, int position);

	virtual void				InitSkillTopLevel();
	virtual bool				UpdateSkillInfo(int index);
	virtual PetMagicSkillList*	GetSkillListInfo(int index);
	virtual PetLearnSkillResult	CheckSkillLearnLimit(int index, int skillIndex);
	virtual PetMagicSkillInfo*	GetNextLevelSkill(int index, int iMagicID);
	virtual vector<int>&		GetCraftListInfo(int index)		{ return m_craftList[index]; }

	virtual void				PasteCraftTool(int index);
	virtual void				UpdateCreaftInfo(int index);

	virtual int					GetPickupEQIndex()				{ return m_pickupEQIndex; }
	virtual int					GetPickupEQPosition()			{ return m_pickupEQPosition; }
	virtual void				UnlockPickupEQ();
	virtual void				ClearPickupEQ()					{ m_pickupEQIndex = -1; m_pickupEQPosition = -1; }

	virtual void				SwapItem(int index, int position, int bagIndex, bool isFromBag);
	virtual void				UpdateEQItem(int index, int position);

	// 餵食界面
	virtual void				SetFeedItemIndex(int index);
	virtual void				FeedItemPickup();
	virtual void				FeedItemPutdown(int index);
	virtual void				FeedFood(int index);
	virtual int					GetFeedFoodIndex()				{ return m_feedItemIndex; }	

	virtual int					GetSwapItemType()				{ return m_swapItemType; }
	virtual int					GetSwapItemIndex()				{ return m_swapItemIndex; }
	virtual void				ClearSwapItemIndex()			{ m_swapItemType = m_swapItemIndex = -1; }

	virtual void				PushPetOK(int index);
	virtual void				ReleasePet(int index);

	virtual GameObjDbStructEx*	GetCraftProductItem(int index, int position, int itemIndex);
	virtual void				CraftingStart(int index, float time);
	virtual void				CraftingStop(int index);
	virtual void				CraftingFailed(int index);
	virtual bool				IsStartCrafting(int index)		{ return m_isStartCrafting[index]; }
	virtual void				SendCraftingStop(int index);

	virtual void				UpdateAbility(int index, int eventType);
	virtual void				LeaveWorld();


	// 接收封包
	virtual void				R_FixPetBase			(int PetPos , CultivatePetStruct& PetBase);
	virtual void				R_LearnSkillResult		(bool Result , int PetPos , int MagicID , int MagicLv);
	virtual void				R_PetLifeResult			(bool Result , CultivatePetLifeSkillTypeENUM SkillType , int PetPos , int EventTime);
	virtual void				R_GetProductResult		(bool Result);
	virtual void				R_SwapItemResult		(bool Result);
	virtual void				R_FixItem				(int PetPos , int PetItemPos , ItemFieldStruct& Item);
	virtual void				R_PetEventProcess		(CultivatePetCommandTypeENUM Event);
	virtual void				R_PetMargeResult		(PetMargeResultENUM Result , int PetCount , int PetPos[3] , CultivatePetStruct& PetBase );

	virtual int 				GetFeedFoodTmpIndex(int index)	{ return m_petfood[index].pos + 60 ;  };

	virtual float				CaculatePetMixModeOneCount	( int index , float pet1 , float pet2 );
	virtual float				CaculatePetMixModeTwoCount	( int index , float pet1 , float pet2 , float pet3 );

	virtual void				ResetMergeTime(); //重置
	virtual void				IsPedFoodNull();
	virtual bool 				IsPetStateBusy(int index1, int index2, int index3);
	int							m_petpremixlevel;			//融合前等級
	int							m_PetMix2;			
	int							m_PetMix3;		
	PetFoodCountInfo			m_petfood[10];

protected:
	int							m_takeoutIndex;				// 已送出取出寵物封包
	int							m_updateNameIndex;			// 重新設定寵物名稱
	int							m_selected;					// 目前選擇寵物編號
	int							m_learnNeedPoint;			// 學習技能需要技能點數
	PetMagicSkillList			m_skillList[6];				// 該寵物的技能

	int							m_pickupEQIndex;			// 已經拿取裝備的寵物編號
	int							m_pickupEQPosition;			// 已經拿取的裝備編號

	vector<int>					m_craftList[6];				// 可生產列表
	int							m_feedItemIndex;			// 餵食界面放置物品

	int							m_swapItemType;				// 
	int							m_swapItemIndex;			// 交換物品


	float						m_craftingTime[6];			// 寵物生產
	bool						m_isStartCrafting[6];		// 是否開始生產
	bool						m_isCancelCrafting[6];		// 停止中斷生產
	map<int, int>				m_topLevelMap;				// 記錄每個法術的最高等級
	map<int, PetMagicSkillInfo> m_nextLevelSkill[6];		// 下一等級的技能資料
	bool						m_bAlreadyLearned;			// 判斷要提升的技能是"升級"(已學過)還是"學習"(未學過)

};