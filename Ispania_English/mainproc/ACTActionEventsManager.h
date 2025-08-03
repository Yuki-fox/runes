#pragma once

#include <set>
#include <map>
#include <vector>
#include "RuneDev.h"
#include "../tools/RecycleBin/recyclebin.h"

using namespace std;

enum ACTActionEventsType
{
	ACT_ACTIONEVENTS_TYPE_NONE			= 0,
	ACT_ACTIONEVENTS_TYPE_ATTACK,
	ACT_ACTIONEVENTS_TYPE_MAGIC,
};

enum SpellShootPackageType
{
	SPELL_SHOOT_PACKAGE_NONE			= 0,
	SPELL_SHOOT_PACKAGE_ATTACK,
	SPELL_SHOOT_PACKAGE_ASSIST,
};

struct ACTActionEventsInfo
{
	ACTActionEventsInfo();
	~ACTActionEventsInfo();
	void Clear();
	void Reset();
	void Recyele();
	void Update(float elapsedTime);
	

	ACTActionEventsType m_type;
	CRuACT_ActionEvents* m_actionEvent;
	float m_liveTime;					// ���ͮɶ�
	int m_source;						// �ӷ�
	int m_target;						// �ت�

	union {
		struct {
			int m_dmgType;				// �ˮ`����(enum ATTACK_DMGTYPE_ENUM)
			int	m_damage;				// �ˮ`�ƭ�
			bool m_offHand;				// �Ƥ�
		}m_attack;

		struct {
			int m_serial;				// �k�N�y���s��
			int m_shootNum;				// �I�k���ǽs��
		}m_magic;
	};
};

// �o�g����S��
struct ShootFlyEffectStruct
{
	int target;							// �ؼнs��
	CRuVector3 targetPosition;			// �ت��y��
	CEffectSprite* fly;					// ����Φ�s�S��
};

struct SingleMagicPackage
{
	SingleMagicPackage()
	{
		memset(this, 0, sizeof(SingleMagicPackage));
	}

	int target;							// �ؼнs��
	struct {
		int type;						// ����
		int magicBase;					// �k�N������޽s��	
		int	valueType;					// �y���ˮ`����(HP, MP, SP)
		int value;						// �ˮ`�θɦ�ƭ�
		int valueDeduct;				// �p���t�Z��
	}attack;

	struct {
		int type;						// ����
		int magicBase;					// �k�N������޽s��	
		int magicLv;					// SERVER�p��(�O����)
		float time;						// BUFF�ɶ�
	}assist;
};

struct SpellShootPackage
{
	SpellShootPackage()
	{
		m_actionDone = false;
		m_list.clear();
		m_flyEffect.clear();
	}

	void Clear()
	{		
		for ( vector<ShootFlyEffectStruct>::iterator iter = m_flyEffect.begin(); iter != m_flyEffect.end(); iter++ )
		{
			// �յ����S�ĵ���
			if ( iter->fly )
				iter->fly->SetEffectMotionStep(eEffectMotion_Step_Disappear);

			iter->fly = NULL;
			iter->target = 0;
		}
		m_flyEffect.clear();
		m_list.clear();
	}

	bool Empty()								{ return m_list.empty(); }
	
	//
	bool							m_actionDone;	// �ʧ@����
	vector<ShootFlyEffectStruct>	m_flyEffect;	// �����L�{�ؼвM��(�ݭn��C�@�ӥؼв��ͯS��)
	vector<SingleMagicPackage>		m_list;			// �ˮ`�M��
};

class CNetMagicPackageData
{
public:
	CNetMagicPackageData();
	void Clear();
	void Update(float elapsedTime);
	void Release();
	void ReleaseEffect(CEffectSprite* sprite);
	void EndSpell();
	void SetActionDone()						{ m_isActionDone = true; }
	void AddRefCount()							{ m_refCount++; if ( m_refCount > 0 ) m_isActionDone = false; }
	void DelRefCount()							{ m_refCount--; }	
	int GetRefCount()							{ return m_refCount; }
	int CreateShoot(int targetCount, int* targetList, CRuVector3 position);
	void EndShoot()								{ m_shootIndex += 1; }
	SpellShootPackage* GetLastShootPackage();
	void SetShootActionDone(int index=-1);
	void AddAttackData(int magicSerail,int target, int type, int magicBase, int valueType, int value);
	void AddAssistData(int magicSerail,int target, int type, int magicBase, int magicLv, float time);

	CEffectSprite* GetIdleExplode();
	bool CheckFlyEffectEnd();

	int m_refCount;							// �ѦҭӼ�(�D�n�Ψӹ�M Shoot End �Ӽ�)
	int m_id;								// SERVER�e�Ӫ��k�N�y���s��
	int m_spellTime;						// �I�k�ɶ�
	int m_magic;							// �k�N�s��
	int m_owner;							// �I�k��
	int m_target;							// �ؼ�
	int m_shootIndex;						// �̫�@���X��ʥ]
	bool m_isActionDone;					// �ʧ@�ƥ�O�_�w�g����
	float m_x, m_y, m_z;					// �ت��y��
	float m_destructTime;					// �^���ɶ�	

	CEffectSprite* m_idleExplode;			// �`���R���S��	
	vector<SpellShootPackage> m_shoot;		// �榸�I�k
};

class CACTActionEventsManager
{
public:
	CACTActionEventsManager();
	~CACTActionEventsManager();
	
	void Release();
	void Update(float elapsedTime);
	void EventCallback(CRuController_EventTrigger *controller, PTRVALUE userData);
	void ReleaseACTActionEvent(ACTActionEventsInfo* pActionEvent);
	ACTActionEventsInfo*		GetACTActionEvent();	
	bool						CheckACTActionEventByID(int id);

	CNetMagicPackageData*		CreateMagicData(int magicSerail);
	CNetMagicPackageData*		GetMagicData(int magicSerail);
	void						ReleaseMagicData(int magicSerail);

	bool CreateFlyEffect(int magicSerail, int shootNumber);
	bool CreateExplodeEffect(int magicSerail, int shootNumber = 0);
	void CreateShootEffect(int magicSerail, int shootNumber);
	void HandleMagicResult(int magicSerail, int shootNumber);
	void HandleMagicAttackByID(int magicSerail, int shootNumber, int targetId);
	void HandleMagicExtend(CNetMagicPackageData* magicData, SingleMagicPackage* single);

	CEffectSprite* CreateEffect(const char* filename, float scale = 1.0f);

private:
	RecycleBin<ACTActionEventsInfo>		m_actionEventInfo;
	set<ACTActionEventsInfo*>			m_usedEvent;

	RecycleBin<CNetMagicPackageData>	m_magicData;
	map<int, CNetMagicPackageData*>		m_usedMagic;
};

extern	CACTActionEventsManager*		g_pACTActionEventsManager;

