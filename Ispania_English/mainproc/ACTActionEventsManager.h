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
	float m_liveTime;					// 產生時間
	int m_source;						// 來源
	int m_target;						// 目的

	union {
		struct {
			int m_dmgType;				// 傷害類型(enum ATTACK_DMGTYPE_ENUM)
			int	m_damage;				// 傷害數值
			bool m_offHand;				// 副手
		}m_attack;

		struct {
			int m_serial;				// 法術流水編號
			int m_shootNum;				// 施法順序編號
		}m_magic;
	};
};

// 發射飛行特效
struct ShootFlyEffectStruct
{
	int target;							// 目標編號
	CRuVector3 targetPosition;			// 目的座標
	CEffectSprite* fly;					// 飛行或串連特效
};

struct SingleMagicPackage
{
	SingleMagicPackage()
	{
		memset(this, 0, sizeof(SingleMagicPackage));
	}

	int target;							// 目標編號
	struct {
		int type;						// 類型
		int magicBase;					// 法術單體索引編號	
		int	valueType;					// 造成傷害類型(HP, MP, SP)
		int value;						// 傷害或補血數值
		int valueDeduct;				// 計算後差距值
	}attack;

	struct {
		int type;						// 類型
		int magicBase;					// 法術單體索引編號	
		int magicLv;					// SERVER計算(記錄用)
		float time;						// BUFF時間
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
			// 試著讓特效結束
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
	bool							m_actionDone;	// 動作結束
	vector<ShootFlyEffectStruct>	m_flyEffect;	// 中間過程目標清單(需要對每一個目標產生特效)
	vector<SingleMagicPackage>		m_list;			// 傷害清單
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

	int m_refCount;							// 參考個數(主要用來對映 Shoot End 個數)
	int m_id;								// SERVER送來的法術流水編號
	int m_spellTime;						// 施法時間
	int m_magic;							// 法術編號
	int m_owner;							// 施法者
	int m_target;							// 目標
	int m_shootIndex;						// 最後一次出手封包
	bool m_isActionDone;					// 動作事件是否已經結束
	float m_x, m_y, m_z;					// 目的座標
	float m_destructTime;					// 回收時間	

	CEffectSprite* m_idleExplode;			// 循環命中特效	
	vector<SpellShootPackage> m_shoot;		// 單次施法
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

