#pragma	  once

#include <vector>

class CEffectSprite;

using namespace std;

enum EffectMotionStep
{
	eEffectMotion_Step_Null				= 0,
	eEffectMotion_Step_Appear,					// 循環特效開始
	eEffectMotion_Step_Idle,					// 循環特效中間(Loop)
	eEffectMotion_Step_Disappear,				// 循環特效結束
	eEffectMotion_Step_Explode,					// 爆炸
	eEffectMotion_Step_Dead,					// 結束
};

enum EffectDamageType
{
	eEffectDamageType_Null				= 0,
	eEffectDamageType_HP,
	eEffectDamageType_MP,
	eEffectDamageType_SP,
};

enum EffectDamageState
{
	eEffectDamageState_Normal,
	eEffectDamageState_Miss,
};

enum EffectTargetType
{
	EffectTarget_Sprite,
	EffectTarget_LinePosition,
};

//-----------------------------------------------------------------------------
// 觸發事件
typedef void (*EfectSpriteCallback)	(CEffectSprite* sprite, int userData);

struct EffectCallbackInfo
{
	EffectCallbackInfo()
	{
		callback = NULL;
		userData = 0;
	}

	EfectSpriteCallback callback;
	int userData;
};

class CEffectSprite : public IBaseSprite
{
public:
	CEffectSprite(bool isAddToContainer=true);
	virtual ~CEffectSprite();

	static	BOOL					SetPloyLineArgument(CRuEntity* entity, float Deviation, float VSpeed, float ParabolaHeight, float segmentLength=1.0f);
	virtual	void					ReceiveDestruct(IBaseSprite* pSprite);
	virtual	void					Update(float elapsed);
	virtual void					Render();
	virtual bool					IsVisible();

	virtual	void					AttachSprite(IBaseSprite* pRelative, int point, CRuVector3* offset=NULL);
	virtual	void					DetachSprite();

	virtual	void					SetPolyLineTarget(IBaseSprite* target, int point);

	virtual void					SetEffectMotionStep(int step, CRuACT_ActionEvents *actionEventsOut = NULL, bool immediately=false);
	virtual int						GetEffectMotionStep()							{ return m_effectMotionStep; }
	virtual void					SetIdleEffectLiveTime(float time);

	virtual	void					SetAutoDestruct( bool isAutoDestruct )			{ m_isAutoDestruct = isAutoDestruct; }
	virtual bool					IsAutoDestruct()								{ return m_isAutoDestruct; }
	virtual	void					SetSoundState(bool isEnable)					{ m_isEnableSound = isEnable; }
	virtual	bool					GetSoundState()									{ return m_isEnableSound; }

	virtual void					SetReleaseCallback(EfectSpriteCallback callback, int userdata);
	virtual void					SetActionEventCallback(EfectSpriteCallback callback, int userdata);
	virtual void					SetUpdateCallback(EfectSpriteCallback callback, int userdata);
	virtual CRuEntity*				GetRelativeRuEntity();
	virtual CRuEntity*				GetPolyLineRuEntity();	

	virtual	void					SetBeginSprite(IBaseSprite* pSprite, int point)	{}
	virtual	void					SetEndSprite(IBaseSprite* pSprite, int point)	{}
	virtual void					SetTargetPosition(CRuVector3 position)			{}
	virtual	void					SetStartMoveDelayTime(float time)				{}
	virtual void					SetMoveEndCallback(EfectSpriteCallback callback, int userdata)		{}	

protected:	
	virtual	void					UpdateEffectMotion(float elapsed);

public:
	int								m_arg1, m_arg2;						// 自定參數

protected:
	EffectCallbackInfo				m_OnRelease;					// 當物件要被刪除時觸發
	EffectCallbackInfo				m_OnActionEvent;				// 動作物件觸發
	EffectCallbackInfo				m_OnUpdate;						// 定時觸發

	IBaseSprite*					m_pRelative;					// 關連角色物件
	IBaseSprite*					m_pPolyLineSprite;				// 串連特效
	int								m_attachPoint;					// 關連參考點
	int								m_polyLinePoint;				// 串連特效參考點
	int								m_effectMotionStep;
	float							m_effectStepTime;
	float							m_explodeLeftTime;				// 爆炸特效消失時間
	float							m_idleEffectLiveTime;			// IDLE 類型存在時間
	bool							m_isStartUpdate;				// 初次更新
	bool							m_isAutoDestruct;				// 是否自動刪除本身物件
	bool							m_isEnableSound;				// 音效開關	
};

class CLineEffectSprite : public CEffectSprite
{
public:
	CLineEffectSprite(bool isAddToContainer=true);

	virtual	void					ReceiveDestruct(IBaseSprite* pSprite);
	virtual	void					Update(float elapsed);
	virtual	void					SetDirection(CRuVector3 direction, bool IsImmediate = true);
	virtual	void					SetBeginSprite(IBaseSprite* pSprite, int point);
	virtual	void					SetEndSprite(IBaseSprite* pSprite, int point);
	virtual void					SetTargetPosition(CRuVector3 position);

	virtual	void					SetStartMoveDelayTime(float time)				{ m_startMoveDelayTime = time; }
	virtual	float					GetStartMoveDelayTime()							{ return m_startMoveDelayTime; }
	virtual	void					SetMoveSpeed(float speed);
	virtual	float					GetFlyTime();	

	virtual void					SetMoveEndCallback(EfectSpriteCallback callback, int userdata);

protected:
	EffectCallbackInfo				m_OnMoveEnd;					// 移動到目的時觸發

	EffectTargetType				m_targetType;	
	IBaseSprite*					m_pBegin;						// 開始物件
	int								m_beginPoint;					// 開始參考點
	float							m_delayEndTime;					// 結束觸發時間
	float							m_velocity;						// 目前速度
	float							m_startMoveDelayTime;			// 移動延遲	
	CRuVector3						m_targetPosition;				// 目的坐標
};

// 拋物線特效
class CParabolaEffectSprite : public CLineEffectSprite
{
public:
	CParabolaEffectSprite(bool isAddToContainer=true);

	static	float					GetMagicParabolaTime(CRuVector3& p1, CRuVector3& p2, int parabolaHeight, float speed);
	virtual	void					Update(float elapsed);
	virtual void					SetParabolaHeight(float parabolaHeight);
	virtual float					GetMoveTime();

protected:
	float							m_moveTime;						// 結束時間
	float							m_updateTime;					// 經過總時間
	float							m_parabolaHeight;				// 最大高度
	CRuVector3						m_beginPosition;				// 開始坐標
};