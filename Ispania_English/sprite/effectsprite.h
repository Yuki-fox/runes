#pragma	  once

#include <vector>

class CEffectSprite;

using namespace std;

enum EffectMotionStep
{
	eEffectMotion_Step_Null				= 0,
	eEffectMotion_Step_Appear,					// �`���S�Ķ}�l
	eEffectMotion_Step_Idle,					// �`���S�Ĥ���(Loop)
	eEffectMotion_Step_Disappear,				// �`���S�ĵ���
	eEffectMotion_Step_Explode,					// �z��
	eEffectMotion_Step_Dead,					// ����
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
// Ĳ�o�ƥ�
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
	int								m_arg1, m_arg2;						// �۩w�Ѽ�

protected:
	EffectCallbackInfo				m_OnRelease;					// ����n�Q�R����Ĳ�o
	EffectCallbackInfo				m_OnActionEvent;				// �ʧ@����Ĳ�o
	EffectCallbackInfo				m_OnUpdate;						// �w��Ĳ�o

	IBaseSprite*					m_pRelative;					// ���s���⪫��
	IBaseSprite*					m_pPolyLineSprite;				// ��s�S��
	int								m_attachPoint;					// ���s�Ѧ��I
	int								m_polyLinePoint;				// ��s�S�İѦ��I
	int								m_effectMotionStep;
	float							m_effectStepTime;
	float							m_explodeLeftTime;				// �z���S�Į����ɶ�
	float							m_idleEffectLiveTime;			// IDLE �����s�b�ɶ�
	bool							m_isStartUpdate;				// �즸��s
	bool							m_isAutoDestruct;				// �O�_�۰ʧR����������
	bool							m_isEnableSound;				// ���Ķ}��	
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
	EffectCallbackInfo				m_OnMoveEnd;					// ���ʨ�ت���Ĳ�o

	EffectTargetType				m_targetType;	
	IBaseSprite*					m_pBegin;						// �}�l����
	int								m_beginPoint;					// �}�l�Ѧ��I
	float							m_delayEndTime;					// ����Ĳ�o�ɶ�
	float							m_velocity;						// �ثe�t��
	float							m_startMoveDelayTime;			// ���ʩ���	
	CRuVector3						m_targetPosition;				// �ت�����
};

// �ߪ��u�S��
class CParabolaEffectSprite : public CLineEffectSprite
{
public:
	CParabolaEffectSprite(bool isAddToContainer=true);

	static	float					GetMagicParabolaTime(CRuVector3& p1, CRuVector3& p2, int parabolaHeight, float speed);
	virtual	void					Update(float elapsed);
	virtual void					SetParabolaHeight(float parabolaHeight);
	virtual float					GetMoveTime();

protected:
	float							m_moveTime;						// �����ɶ�
	float							m_updateTime;					// �g�L�`�ɶ�
	float							m_parabolaHeight;				// �̤j����
	CRuVector3						m_beginPosition;				// �}�l����
};