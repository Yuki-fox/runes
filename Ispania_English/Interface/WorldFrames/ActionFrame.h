#pragma once

#include <string>
#include "../interface.h"

#define	ACTION_BAR_MAX_CLASS			16					// ¾�~�U��
#define	ACTION_BAR_MAX_ITEMS			80					// ���s�ƶq
#define PETACTIONBAR_MAX_SKILLS			10

class CActionFrame;
extern CActionFrame*				g_pActionFrame;

using namespace std;

enum ActionItemType {
	eActionItemType_None			= 0,					// �Ū�����
	eActionItemType_Bag,									// �I�]
	eActionItemType_Equipment,								// �˳�
	eActionItemType_Skill,									// �ޯ�
	eActionItemType_Macro,									// ����
	eActionItemType_Emote,									// �����ʧ@
	eActionItemType_Teleport,								// �ǰe��
	eActionItemType_SuitSkill,								// �M�˧ޯ�
	eActionItemType_SkillPlate,								// �ޯ�L
	eActionItemType_HappyPartner,							//�ּ֪��٦��
	eActionItemType_CoolSuit,								//�ŮM��
};

// �Ĥ@�������c
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

// �ĤG�������c(06/16/2008)
#define ACTION_ITEM_VERSION			0x02
struct ActionItem2
{
	int							type;
	union {
		int						_value[4];			// ���c�j�p,��K��ƼW�[
		// �I�],�˳�
		struct {
			int					objectID;			// �򥻪��~�s��
			int					createTime;			// ���~�����i���|��,���ͮɶ��ѧO�s��
			int					serial;				// ���~�����i���|��,�ѧO�s��
		};
		// �ޯ�s��
		struct {
			int					skillID;
		};
		// �����s��
		struct {
			int					macroID;
		};
		// �ʧ@�s��
		struct {
			int					emoteID;
		};
		//�ǰe
		struct {
			int					teleportID;
		};
		//�M�˧ޯ�
		struct {
			int					suitskillID;
		};
		//�ޯ�L
		struct{
			int					skillplateID;
		};
		//�٦��
		struct 
		{
			int					PartnerMode;
			int					PartnerPos;
		};
		//�ŮM��
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
			bool				isLocked;		// ��w�ֱ���
			bool				isHideEmpty;	// ���êŪ�����
		};		
	};

	struct {
		bool		isEnable;		// �O�_�ҥ�
		int			count;			// �`�@�Ӽ�
		int			row;			// ���Ӽ�
	} bar[4];
};

struct PetActionItem
{
	float	coldownTime;		// CD��s�ɶ�
	int		skillID;			// �ޯ�s��(1-6�򥻩R�O)
	bool	autoSpell;			// �۰ʬI�k
};

struct ExtraActionItem
{
	int buffId;					// �ӷ�BUFF
	int buffPowerLevel;			// �ӷ�Buff PowerLevel
	int skillId;				// �ޯ�s��	
};

// ����ޯ�
struct ImplementActionItem
{
	int speller;				// �I�k��
	int skillId;				// �k�N�s��
	int skillLevel;				// �ޯ൥��

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
	

	// �ֱ���]�w
	void				SetActionBarSetting(int index, bool enable, int count, int row);
	ActionBarSetting&	GetActionBarSetting()					{ return m_setting; }
	void				SetActionBarLocked(bool locked)			{ m_setting.isLocked = locked; }
	bool				GetActionBarLocked()					{ return m_setting.isLocked; }
	void				SetActionBarHideEmpty(bool hideEmpty)	{ m_setting.isHideEmpty = hideEmpty; }
	bool				GetActionBarHideEmpty()					{ return m_setting.isHideEmpty; }
	void				ResetActionBarSetting();	

	void				SaveClientSetting(bool Immediately=false);	

	// ����ޯ�
	void				InsertImplementAction(int speller, int skillId, int skillLevel);
	void				DeleteImplementAction(int speller, int skillId, int skillLevel);
	void				ClearImplementAction();
	void				UseImplementAction(int index);
	ImplementActionItem*	GetImplementActionItem(int index);

	// �S��ޯ�
	void				UseExSkill(int targetId, int x, int y, int z);

protected:
	void				SlotChanged(int index);

protected:
	ActionItem2					m_items[ACTION_BAR_MAX_CLASS][ACTION_BAR_MAX_ITEMS];
	PetActionItem				m_petItems[PETACTIONBAR_MAX_SKILLS];
	vector<ExtraActionItem>		m_extraItems;
	vector<ImplementActionItem>	m_implementItems;
	float						m_lastSaveTime;
	int							m_waitingType;					// ���ݧ������O����(0�L, 1�B�~�ޯ�, 2����)
	int							m_waitingSpellIndex;
	float						m_petAllColdown;
	ActionBarSetting			m_setting;
	ActionBarSetting			m_settingBackup;
	double						m_lastSaveSettingTime;
};