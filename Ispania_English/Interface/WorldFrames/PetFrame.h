#pragma once

#include "../interface.h"
#include "../NetWaker_Member/net_cultivatepet/NetCli_CultivatePet.h"

#define PET_SUMMON_LEVEL_LIMIT	5	// �d���W�L���a PET_SUMMON_LEVEL_LIMIT �šA�|�L�k�l��
#define PET_SUMMON_COST_TIME	5	// �d���l�ꥲ�� PET_SUMMON_COST_TIME ��A�~��l��X��
#define PET_MARGE_COUNT_MAX		3	

class CPetFrame;
extern CPetFrame*				g_pPetFrame;

enum PetLearnSkillResult
{
	ePetLearnSkill_Success		= 0,					// ��
	ePetLearnSkill_Level		= -1,					// ���Ť���
	ePetLearnSkill_KeyItem		= -2,					// �ʤ֭��n���~
	ePetLearnSkill_Point		= -3,					// �ޯ��I�Ƥ���
	ePetLearnSkill_STR			= -4,					// STR����
	ePetLearnSkill_STA			= -5,					// STA����
	ePetLearnSkill_INT			= -6,					// INT����
	ePetLearnSkill_MND			= -7,					// MND����
	ePetLearnSkill_AGI			= -8,					// AGI����
	ePetLearnSkill_Already		= -9,					// �w�g�̰��ŤF
	ePetLearnSkill_Unknow		= -10,					// �������~
};

// �浧�ޯ��T
struct PetMagicSkillInfo
{
	void Init()
	{
		memset(this, 0, sizeof(PetMagicSkillInfo));
	}

	// �ݨD����
	struct
	{
		int Level;				// �d������
		int Str;				// �O�q
		int Sta;				// �@�O
		int Int;				// ���z
		int Mnd;				// �믫
		int Agi;				// �ӱ�
		int Point;				// �ݨD�ޯ��I��
		int KeyItem;			// �ݨD���n���~�s��
		int Lock;				
	} Limit;

	bool IsLearn;				// �O�_�w�g�Ƿ|
	bool IsTopLevel;			// �O�_���̰��ŧO
	int MagicID;				// �k�N�s��
	int PowerLevel;				// �ثe����
};

// �d�������ƶq
struct PetFoodCountInfo
{
	int count;
	int ID; 
	int type;   // 0 ItemEx  1 ItemMagicBox  2 ItemBase
	int pos;
};


// �C�@���d���Ҿ֦����ޯ�
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

	// �����ɭ�
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


	// �����ʥ]
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

	virtual void				ResetMergeTime(); //���m
	virtual void				IsPedFoodNull();
	virtual bool 				IsPetStateBusy(int index1, int index2, int index3);
	int							m_petpremixlevel;			//�ĦX�e����
	int							m_PetMix2;			
	int							m_PetMix3;		
	PetFoodCountInfo			m_petfood[10];

protected:
	int							m_takeoutIndex;				// �w�e�X���X�d���ʥ]
	int							m_updateNameIndex;			// ���s�]�w�d���W��
	int							m_selected;					// �ثe����d���s��
	int							m_learnNeedPoint;			// �ǲߧޯ�ݭn�ޯ��I��
	PetMagicSkillList			m_skillList[6];				// ���d�����ޯ�

	int							m_pickupEQIndex;			// �w�g�����˳ƪ��d���s��
	int							m_pickupEQPosition;			// �w�g�������˳ƽs��

	vector<int>					m_craftList[6];				// �i�Ͳ��C��
	int							m_feedItemIndex;			// �����ɭ���m���~

	int							m_swapItemType;				// 
	int							m_swapItemIndex;			// �洫���~


	float						m_craftingTime[6];			// �d���Ͳ�
	bool						m_isStartCrafting[6];		// �O�_�}�l�Ͳ�
	bool						m_isCancelCrafting[6];		// ����_�Ͳ�
	map<int, int>				m_topLevelMap;				// �O���C�Ӫk�N���̰�����
	map<int, PetMagicSkillInfo> m_nextLevelSkill[6];		// �U�@���Ū��ޯ���
	bool						m_bAlreadyLearned;			// �P�_�n���ɪ��ޯ�O"�ɯ�"(�w�ǹL)�٬O"�ǲ�"(���ǹL)

};