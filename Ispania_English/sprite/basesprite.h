#pragma once

#include <set>
#include <map>
#include <string>

#include "Rune Engine/Rune/Rune.h"
#include "RoleData/ObjectDataClass.h"
#include "eventmanager.h"

class IBaseSprite;

#define IsSame(a, b)					(((a) > (b)) ? ((a)-(b) < 0.00001f) : ((b)-(a) < 0.00001f))

#define DUMMY_ID						1008

#define ATTACH_POINT_TOP				"p_top"
#define ATTACH_POINT_DOWN				"p_down"
#define ATTACH_POINT_HIT01				"p_hit_point01"
#define ATTACH_POINT_HIT02				"p_hit_point02"
#define ATTACH_POINT_BACK_STAFF			"p_back_staff"
#define ATTACH_POINT_BACK_SHIELD		"p_back_shield"
#define ATTACH_POINT_BACK_SWORD			"p_back_sword"
#define ATTACH_POINT_BACK_BOW			"p_back_bow"
#define ATTACH_POINT_BACK_GUN			"p_back_gun"
#define ATTACH_POINT_NAME_TITLE			"p_name_title"
#define ATTACH_POINT_HAND_SHIELD		"p_hand_shield"
#define ATTACH_POINT_LEFT_HAND			"p_left_hand"
#define ATTACH_POINT_RIGHT_HAND			"p_right_hand"
#define ATTACH_POINT_LEFT_FOOT			"p_left_foot"
#define ATTACH_POINT_RIGHT_FOOT			"p_right_foot"
#define ATTACH_POINT_LEFT_WEAPON		"p_left_weapon"
#define ATTACH_POINT_RIGHT_WEAPON		"p_right_weapon"
#define ATTACH_POINT_SHOOT				"p_shoot"
#define ATTACH_POINT_MOUTH				"p_mouth"
#define ATTACH_POINT_RIDE_01			"p_mount01"
#define ATTACH_POINT_RIDE_02			"p_mount02"
#define ATTACH_POINT_RIDE_03			"p_mount03"
#define ATTACH_POINT_RIDE_04			"p_mount04"
#define ATTACH_POINT_RIDE_05			"p_mount05"
#define ATTACH_POINT_RIDE_06			"p_mount06"
#define ATTACH_POINT_FREEZE_DOWN		"freeze_down"
#define ATTACH_POINT_FREEZE_HIT			"freeze_hit"
#define ATTACH_POINT_WEAPON_EFFECT01	"effect01"
#define ATTACH_POINT_WEAPON_EFFECT02	"effect02"
#define ATTACH_POINT_WEAPON_EFFECT03	"effect03"

enum SpriteObjectType {
	eSpriteObject_Unknown			= 0,
	eSpriteObject_Player,
	eSpriteObject_NPC,
	eSpriteObject_Effect,
	eSpriteObject_QuestNPC,
	eSpriteObject_BattleGroupObj,

};

enum SpriteClassType {
	eSpriteClass_Base				= 0,	
	eSpriteClass_Role,
	eSpriteClass_Effect,
	eSpriteClass_Effect_Line,
	eSpriteClass_Effect_Parabola,
	eSpriteClass_Text,
};

enum SpriteMotionType {
	eMotion_Null					= 0,

	// normal
	eMotion_Stand,
	eMotion_Stand_Idle01,
	eMotion_Stand_Idle02,
	eMotion_Stand_Idle03,	

	// move
	eMotion_Run_Forward,
	eMotion_Run_Back,	
	eMotion_Walk_Forward,
	eMotion_Walk_Back,
	eMotion_Shuffle_Left,				// �४
	eMotion_Shuffle_Right,				// ��k

	// ��a
	eMotion_Swim_Idle,					// ��a-���m����(loop)
	eMotion_Swim_Forward,				// ��a-�V�e
	eMotion_Swim_Back,					// ��a-�V��
	eMotion_Swim_Left,					// ��a-������
	eMotion_Swim_Right,					// ��a-�k����
	eMotion_Drown,						// �Ĥ�
	eMotion_Death_Drowned,				// �Ĥ�-���`
	eMotion_Dead_Water,					// �Ĥ�(���b����)

	// jump
	eMotion_Jump_Up,
	eMotion_Jump_Loop,
	eMotion_Jump_Down,
	eMotion_Jump_Lendrun,

	// fight
	eMotion_Unarmed_Idle,
	eMotion_Unarmed_Attack01,
	eMotion_Unarmed_Attack02,
	eMotion_Unarmed_Attack03,
	eMotion_Unarmed_Parry,
	eMotion_1H_Idle,
	eMotion_1H_Attack01,
	eMotion_1H_Attack02,
	eMotion_1H_Attack03,
	eMotion_1H_Pierce01,
	eMotion_OH_Pierce01,
	eMotion_OH_Attack01,
	eMotion_1H_Parry,
	eMotion_2H_Idle,
	eMotion_2H_Attack01,
	eMotion_2H_Attack02,
	eMotion_2H_Attack03,
	eMotion_2H_Parry,
	eMotion_Polearm_Idle,
	eMotion_Polearm_Attack01,
	eMotion_Polearm_Attack02,
	eMotion_Polearm_Attack03,
	eMotion_Polearm_Parry,
	eMotion_Bow_Idle,
	eMotion_Bow_Ready,
	eMotion_Bow_Channel,
	eMotion_Bow_Release,
	eMotion_Bow_Parry,
	eMotion_Gun_Idle,
	eMotion_Gun_Ready,
	eMotion_Gun_Channel,
	eMotion_Gun_Release,
	eMotion_Gun_Parry,

	eMotion_Shield_Block,				// �����X����
	eMotion_Shield_Bash,				// �����X����
		
	// �I�k�ʧ@
	eMotion_Casting_Delay,				// ��������ʧ@(loop)
	eMotion_Cast_01,					// �X��ʧ@,��ۤv
	eMotion_Cast_02,					// �X��ʧ@,��ؼ�
	eMotion_Continuous_Cast,			// �X��ʧ@,����I�k(loop)

	eMotion_1H_Holster,					// ���k���ި��Z��
	eMotion_1H_Unholster,				// ���k�������Z��
	eMotion_2H_Holster,					// �᰼�ި��Z��
	eMotion_2H_Unholster,				// �᰼�����Z��

	eMotion_Dodge,						// �{��
	eMotion_Hurt,						// ����
	eMotion_Death,						// ���`(�ˤU�ʧ@)
	eMotion_Dead,						// ���`(�w�b�a�O)(loop)
	eMotion_Revive,						// �_��
	eMotion_Dazed,						// �w�t
	eMotion_Slumber,					// �ίv
	eMotion_Knockdown_Start,			// ������h-�}�l
	eMotion_Knockdown_Idle,				// ������h(loop)
	eMotion_Knockdown_End,				// ������h-����
	eMotion_Knockdown_Recover,			// ������h-�_��

	// �ͬ��ޯ�
	eMotion_Fishing_Cast,				// ����-�Ϭ�
	eMotion_Fishing_Idle,				// ����-����(loop)
	eMotion_Fishing_Reel,				// ����-����
	eMotion_Gathering_Start,			// �Ķ�-�}�l
	eMotion_Gathering_Idle,				// �Ķ�(loop)
	eMotion_Gathering_End,				// �Ķ�-����
	eMotion_Mining_Start,				// ���q-�}�l
	eMotion_Mining_Idle,				// ���q(loop)
	eMotion_Mining_End,					// ���q-����
	eMotion_Crafting_Start,				// �[�u-�}�l
	eMotion_Crafting_Idle,				// �[�u(loop)
	eMotion_Crafting_End,				// �[�u-����

	// emotion
	eMotion_Sit,						// ���U
	eMotion_Sit_Idle,					// ��(loop)
	eMotion_Stand_Up,					// �_��(�ѧ��U���_)
	eMotion_Sleep_Down,					// ��(�ˤU)
	eMotion_Sleep_Idle,					// ��(loop)
	eMotion_Sleep_Up,					// ��(�_��)
	eMotion_Crouch_Down,				// �ۤU
	eMotion_Crouch_Idle,				// ��(loop)
	eMotion_Crouch_Up,					// �۰_
	eMotion_Drink,						// ��
	eMotion_Eat,						// �Y
	eMotion_Use_Item,					// �ϥΪ��~

	eMotion_Speak01,					// ����1
	eMotion_Speak02,					// ����2
	eMotion_Head_Nod,					// �I�Y�]�O�B�n�^
	eMotion_Head_Shake,					// �n�Y�]�_�B���n�^
	eMotion_Laugh,						// ��
	eMotion_Angry,						// �ͮ�
	eMotion_Cry,						// ���_
	eMotion_Approval,					// �٦P
	eMotion_Victory,					// �w�I(�ӧQ)
	eMotion_Applause,					// ���
	eMotion_Salute,						// �q§
	eMotion_Wave,						// �ۤ�
	eMotion_Point,						// ����e��
	eMotion_Provoke,					// �D��
	eMotion_Beg,						// �^�Q
	eMotion_Dance,						// ���R
	eMotion_Surrender,					// �{��]�̫��U���^
	eMotion_Signature,					// �۵P�ʧ@�]�W�H�Զ��^
	eMotion_Threaten,					// ���~

	// effect
	eMotion_Effect_Appear,
	eMotion_Effect_Idle,
	eMotion_Effect_Disappear,
	eMotion_Effect_Explode
};

enum SpriteAttachPoint {
	eAttach_Null					= 0,	
	eAttach_Top,
	eAttach_Down,
	eAttach_Hit_Point1,
	eAttach_Hit_Point2,
	eAttach_Back_Polearm,
	eAttach_Back_Shield,
	eAttach_Back_Sword,
	eAttach_Name_Title,
	eAttach_Hand_Shield,
	eAttach_Left_Hand,
	eAttach_Right_Hand,
	eAttach_Left_Foot,
	eAttach_Right_Foot,
	eAttach_Left_Weapon,
	eAttach_Right_Weapon,
	eAttach_Shoot,
	eAttach_Mouth,
	eAttach_Freeze_Down,
	eAttach_Freeze_Hit,
	eAttach_Weapon_Effect01,
	eAttach_Weapon_Effect02,
	eAttach_Weapon_Effect03,
};

enum SpriteMoveDirection {
	eMove_NULL						= 0,
	eMove_Forward,
	eMove_Forward_Left,
	eMove_Forward_Right,
	eMove_Side_Left,
	eMove_Side_Right,
	eMove_Back,
	eMove_Back_Left,
	eMove_Back_Right
};

enum SpriteMessageType {	
	eSpriteMsg_Position_Null			= 0,
	eSpriteMsg_Create,
	eSpriteMsg_Delete,
	eSpriteMsg_Position_Changed,
	eSpriteMsg_Vocation_Changed,
	eSpriteMsg_HP_Changed,
	eSpriteMsg_MaxHP_Changed,
	eSpriteMsg_MP_Changed,
	eSpriteMsg_MaxMP_Changed,
	eSpriteMsg_SP_Changed,
	eSpriteMsg_MaxSP_Changed,
	eSpriteMsg_Level_Changed,
	eSpriteMsg_Target_Changed,
	eSpriteMsg_Pet_Changed,
	eSpriteMsg_Buff_Changed,
	eSpriteMsg_Dot_Value,
	eSpriteMsg_Portrait_Changed,	
	eSpriteMsg_Combat_Logo,
	eSpriteMsg_Relation_Changed,
	eSpriteMsg_CastingTime_Changed,
	eSpriteMsg_NameColor_Changed,
	eSpriteMsg_CancelSpell,
	eSpriteMsg_CantCtrl,//���౱��
	eSpriteMsg_Unlink,
};

enum SpriteScaleType {
	eSpriteScale_Local					= 0,
	eSpriteScale_World,
};

enum SpriteShadowType
{
	eSpriteShadowType_None				= 0,
	eSpriteShadowType_Dot				= 1,
	eSpriteShadowType_Projected			= 2
};

struct ruACTActionEventInfo
{
	CRuACT_ActionEvents*			m_pRuActionEvent;
	float							m_update;
};

//-----------------------------------------------------------------------------
// Ĳ�o�ƥ�
typedef void (*SpriteMsgProc)		(IBaseSprite* sprite, SpriteMessageType msg, INT64 arg1, INT64 arg2);
typedef bool (*HideEffectFunc)		(int id, int targetId);

using namespace std;

//-----------------------------------------------------------------------------
class IBaseSprite
{
public:
	static	void					Initialize();
	static	void					Shutdown();
	static	void					ReleaseAll();
	static	set<IBaseSprite*>&		GetContainer()							{ return s_containers; }
	static	void					UpdateAll( float elapsedTime );
	static	void					UpdateShadowMode();
	static	void					RenderAll();
	static	void					SetFontType( CRuFontType* pFontType );
	static	void					SetRuTerrain( CRuOptimalTerrain* pRuTerrain );
	static	CRuContainer_Collision*	GetCollisionContainer();
	static	void					SetMainPlayerId( int id );
	static	bool					CheckedSpriteIsLife( IBaseSprite* pBaseSprite );
	static	const char*				GetAttachPointName( int point );
	static	int						GetAttachPointEnum( const char* point );
	//static	const char*				GetMotionName( int motion );
	//static	int						GetMotionEnum( const char* motion );
	static	void					SetRuEntityDirection( CRuEntity* pRuEntity, CRuVector3& dir, float entityScale );
	static	bool					TerrainTest( CRuVector3& pt1, CRuVector3& pt2, CRuVector3 *newPos );
	static	bool					TerrainTestHight(CRuVector3& pos, float& y, float testRange);
	static	bool					TestSphere( CRuVector3& pos,bool isJump = false);
	static	bool					ModifyPosOnTerrain( CRuVector3& pos );
	static	void					SetSpriteMsgProc(SpriteMsgProc func)	{ s_procFunc = func; }
	static	void					SetLocalPlayerId(int id)				{ s_localPlayerId = id; }
	static	void					SetFontScale(float scale);
	static	void					SetBloodDisable(bool isDisable);

	static void						ResetAllCollisionData();

	static void						SetPaperdollCache(CRuPaperdollCache *paperdollCache) { s_paperdollCache = paperdollCache; }

	static CRuArrayList<IBaseSprite *>&		GetPlotCollisionSprites()		{ return s_plotColSprites; }

	static	void					Insert(IBaseSprite* pSprite);
	static	void					Erase(IBaseSprite* pSprite, bool force = false);
	static	void					SetCheckEffectDisplayFunc(HideEffectFunc func)				{ s_hideEffectFunc = func; }

public:
	IBaseSprite(bool isAddToContainer=true);
	virtual ~IBaseSprite();
	
	virtual BOOL					HandleProxyLoadComplete(RuEventArgs *eventArgs);
	virtual bool					IsProxyLoadComplete()					{ return m_proxyLoadCompleted; }
	
	virtual	void					Update( float elapsedTime );
	virtual void					Render();
	virtual	void					ReceiveDestruct(IBaseSprite* pSprite)	{}
	
	virtual	void					SetIdentify( int id )					{ m_id = id; }
	virtual	int						GetIdentify()							{ return m_id; }
	virtual int						GetClassType()							{ return m_classType; }
	virtual	CRuEntity*				GetRuEntity()							{ return m_pRuEntity; }
	virtual CRuEntity*				GetRuTempEntity()						{ return m_pRuTempEntity; }
	virtual CRuEntity*				GetCurrentEntity()						{ return (m_pRuTempEntity) ? m_pRuTempEntity : m_pRuEntity; }

	virtual void					Show()									{ m_isVisible = true; }
	virtual void					Hide()									{ m_isVisible = false; }
	virtual	void					SetVisible(bool show);
	virtual	bool					IsVisible()								{ return m_isVisible; }
	virtual void					SetImplementVisible(bool visible)		{ m_isImplementVisible = visible; }	
	virtual bool					IsImplementVisible()					{ return m_isImplementVisible; }
	virtual bool					CheckRenderState();

	virtual void					EnableCollision(bool enable)			{ m_enableCollision = enable; }
	virtual bool					CollisionEnabled()						{ return m_enableCollision; }

	virtual void					EnableCollisionBase(bool enable)		{ m_enableCollision_base = enable; }
	virtual bool					CollisionBaseEnabled()					{ return m_enableCollision_base; }

	virtual void					SetServerMove( bool enable )			{ m_isServerMove = enable; }
	virtual bool					GetServerMove()							{ return m_isServerMove; }


	virtual void					EnablePlotCollisionTrigger(bool enable)	{ m_enablePlotColTrigger = enable; }
	virtual bool					PlotCollisionTriggerEnabled()			{ return m_enablePlotColTrigger; }

	virtual	void					SetObjectType( SpriteObjectType type )	{ m_objectType = type; }
	virtual	int						GetObjectType()							{ return m_objectType; }
	virtual bool					SetObjectId( int id, bool deferredLoad = false );
	virtual int						GetObjectId()							{ return m_objectId; }

	void							SetCasterClass();

	virtual bool					LoadRuEntity(const char* actName, bool deferredLoad = false, float scale = 1.0f);
	virtual bool					LoadRuTempEntity(const char* actName);

	virtual	bool					CreateFloorEntity(const char* actName);
	virtual	void					ReleaseFloorEntity();
	virtual void					FindFloorParentEntity();

	virtual void					MovePos( CRuVector3 &pos );
	virtual	void					SetPos( CRuVector3 &pos );
	virtual	CRuVector3				GetPos()								{ return m_nowPos; }
	virtual	void					SetDirection(CRuVector3 direction, bool IsForce = false, bool IsInitial = false);
	virtual	CRuVector3				GetDirection()							{ return m_direction; }
	virtual void					SetMoveSpeed( float speed )				{ m_moveSpeed = speed; }
	virtual float					GetMoveSpeed()							{ return m_moveSpeed; }	
	virtual void					SetOrientingAxis(const CRuVector3 &vec, bool force = false);
	virtual CRuVector3				GetOrientingAxis()						{ return m_orientingAxis; }
	virtual void					SetRebaseDirection(bool rebase)			{ m_rebaseDirection = rebase; }
	virtual bool					GetRebaseDirection()					{ return m_rebaseDirection; }
	virtual void					SetOrientToSurfaceNormal(bool orient)	{ m_orientToSurfaceNormal = orient; }
	virtual bool					GetOrientToSurfaceNormal()				{ return m_orientToSurfaceNormal; }
	virtual void					UpdatePositionDirectionFromEntity();
	virtual void					UpdateOrientation();
	virtual void					SetOrientMode(CRuFusion_Actor::OrientMode orientMode);

	// Alpha
	void							SetAlpha(float alpha)					{ m_alpha = max(min(alpha, 1.0f), 0.0f); }
	float							GetAlpha()								{ return m_alpha; }

	//
	virtual	void					SetScale(float scale, SpriteScaleType type = eSpriteScale_Local);
	virtual	void					AppnedScale(float scale);
	virtual	float					GetScale()								{ return m_scale; }
	virtual	float					GetEntityWorldScale();

	// destruct
	virtual void					ClearDestruct();
	virtual void					SetDestructTime( float time );
	virtual float					GetDestructTime()						{ return m_destructTime; }
	virtual bool					IsDestruct()							{ return m_isDestruct; }

	// pause time
	virtual void					SetPauseTime( float time )				{ m_pauseTime = time; }
	virtual float					GetPauseTime()							{ return m_pauseTime; }	

	virtual	CRuEntity*				FindLinkEntity( int point );
	virtual bool					GetAttachPos( int point, CRuVector3& pos );
	virtual CRuVector3				GetAttachPos( const char *attachPointName );
	virtual CRuVector3				GetAttachDir( const char *attachPointName );

	// touch motion event
	virtual	void					MotionEvent( int type )					{}

	virtual	SpriteMoveDirection		ComputeDirection( CRuVector3 forward );	
	virtual CRuVector3				RotateDirection( float delta );
	virtual bool					CheckSameForward(CRuVector3 forward);

	// play motion
	virtual bool					PlayMotion(const char *motionStr, CRuACT_ActionEvents *actionEventsOut = NULL);
	virtual	bool					HasMotion(const char *motionStr);

	// highlights
	void							EnableCelHighlight(RUCOLOR color);
	void							DisableCelHighlight();

	// utility
	virtual bool					AnchorSprite(float testLimit = 4.0f);

	// shadow
	virtual	void					SetShadowType(SpriteShadowType shadowType)  { m_shadowType = shadowType; }

	// Sprite linking
	virtual void					LinkSprite(IBaseSprite *linkedSprite, const char *attachPointName, bool linkControls = true, bool linkShadow = true);
	virtual void					UnlinkSprite(IBaseSprite *linkedSprite);
	virtual void					UnlinkChildSprite();
	virtual void					SeverSpriteLinks();	
	virtual IBaseSprite*			GetLinkParent();
	virtual bool					SpriteControlsAreLinked();
	virtual void					UpdateLinkedSprites();
	virtual int						GetLinkSpriteCount()						{ return m_linkedSprites.Count(); }

	// Temporal sprite link relations
	IBaseSprite*					FindMountCandidate();
	void							SetParentMount(IBaseSprite *parentMount);
	void							MoveWithParentMount();
	IBaseSprite*					GetParentMount()							{ return m_parentMount; }
	void							UpdateChildSprite();

	// Compass
	BOOL							CreateCompass(INT32 id, const char *resourceName, RUCOLOR color);
	BOOL							SetCompassParameters(INT32 id, const CRuVector3 &destPt);
	BOOL							HideCompass(INT32 id);
	BOOL							ShowCompass(INT32 id);
	void							HideAllCompasses();
	void							ShowAllCompasses();

	static CRuGlobalEventManager*	GetGlobalEventManager();

	// �O���Ѧ��I
	virtual bool					AttachSprite(IBaseSprite* sprite, int point);
	virtual bool					DetachSprite(IBaseSprite* sprite);
	virtual void					TransAttachSprite(CRuEntity* newSprite);

	virtual void					SetAboveWaterState(bool enable);
	virtual bool					GetAboveWaterState()								{ return m_aboveWater; }

	virtual void					SetHorseState(bool ishorse)							{ m_isHorse = ishorse; }
	virtual bool					IsHorse()											{ return m_isHorse; }
	

protected:
	virtual	void					ReleaseEntity();
	virtual	void					ReleaseTempEntity();
	virtual bool					IsMotionComplete();
	virtual	void					AddOnEntity( bool isAttach, CRuEntity* pRuEntity, SpriteAttachPoint point );	
	virtual	void					UpdateScale();

protected:
	static	set<IBaseSprite*>		s_containers;				// 	
	static	CRuFontType*			s_pFontType;				// �r�����	
	static	CRuOptimalTerrain*		s_pRuTerrain;				// �a���T
	static	CRuGlobalEventManager*	s_pRuEventListener;			// �ʧ@�ƥ�
	static	CRuFusion_Mime_Template*s_pRuMimeTemplate;
	static	CRuFusion_Mime_Template*s_pRuMimeTemplate_Deprecated;
	static	SpriteMsgProc			s_procFunc;	
	static	HideEffectFunc			s_hideEffectFunc;			// ���ïS��
	static	int						s_localPlayerId;			// �������a�s��
	static	float					s_fontScale;				// �r���Y��j�p	

	static bool						s_gm_ShowHidden;			// �j��������ê���

	static INT32					s_probe_SpriteHouseKeeping;

	static CRuPaperdollCache*		s_paperdollCache;			// Paperdoll cache manager

	static CRuArrayList<IBaseSprite *>	s_colSprites;
	static CRuArrayList<IBaseSprite *>	s_plotColSprites;

	// Global collision container
	static CRuContainer_Collision*	s_collisionContainer;
	static bool						s_isNoBlood;				// ����{	

protected:
	// 3D Objects
	CRuEntity*						m_pRuEntity;				// entity ����
	CRuEntity*						m_pRuTempEntity;			// �Ȯɴ��� entity ����
	CRuEntity*						m_pRuFloorEntity;			// �a�O����
	CRuEntity_DecalShadow*			m_pRuDecalShadow;			// Projected shadow
	int								m_objectId;					// ������޽s��
	int								m_objectType;				// ��������

	// other	
	int								m_classType;				// ���O����
	int								m_id;						// ����s��
	SpriteShadowType				m_shadowType;				// ���⳱�v�Ҧ�

	// move
	CRuVector3						m_nowPos;					// �{�b�y�Э�
	CRuVector3						m_direction;				// �¦V��V
	float							m_moveSpeed;				// ���ʳt��
	CRuVector3						m_lastOrientingAxisCheckPos;// Last position at which we checked for the orienting axis
	CRuVector3						m_orientingAxis;			// Upward orienting axis
	CRuVector3						m_targetOrientingAxis;		// Target upward orienting axis	

	// destruct 		
	float							m_destructTime;				// �ۧڧR���ɶ�

	// pause time
	float							m_pauseTime;				// �Ȱ��B�z�ɶ�

	// scale
	float							m_scale;					// �Y��
	float							m_appnedScale;				// ���[�Y����

	// alpha
	float							m_alpha;					// �z����

	// Sprite links
	IBaseSprite*					m_linkParent;				// Parent sprite to which this sprite is linked
	const char*						m_linkAttachPointName;		// Attachment point link, NULL to disable	
	CRuVector3						m_linkParentLastPosition;	// Last known position of the link parent
	CRuArrayList<IBaseSprite *>		m_linkedSprites;			// List of sprites linked to this sprite	

	// Mounting
	IBaseSprite*					m_parentMount;
	IBaseSprite*					m_lastParentMount;
	CRuVector3						m_lastParentMountPosition;

	// Compass
	struct CompassDesc
	{
		INT32						m_id;						// Compass ID
		CRuEntity*					m_compassEntity;			// Compass display entity
		RUCOLOR						m_compassColor;				// Compass color
		CRuVector3					m_compassTarget;			// Target position to which the compass points
	};

	CRuArrayList<CompassDesc>		m_compassList;

	bool							m_isVisible;				// ��ܩ�����

	union {
		int __flag[2];
		struct {			
			bool					m_isImplementVisible		:1;			// �������úX��(�ثe�O�b����W����)
			bool					m_enableCollision			:1;			// ����I���}��
			bool					m_enablePlotColTrigger		:1;			// �����I���@��
			bool					m_isDestruct				:1;			// �O�_�۰ʧR��
			bool					m_rebaseDirection			:1;			// Re-base direction vector by using m_orientingAxis as up vector?
			bool					m_orientToSurfaceNormal		:1;			// Sprite's upward axis is always aligned with the surface normal of the triangle it rests on
			bool					m_linkControls				:1;			// Are controls also linked to the parent sprite?
			bool					m_linkShadow				:1;			// Shadow is linked to the link parent
			bool					m_proxyLoadCompleted		:1;			// Proxy load is finished
			bool					m_dyeFXMaskRequiresLoad		:1;			// DyeFX mask requires further loading
			bool					m_enableCollision_base		:1;			// ����I���}��, ��l�]�w, �קK Client �@�������㹳�᭫�m��
			bool					m_aboveWater				:1;			// �i�H�b���W�樫
			bool					m_isHorse					:1;			// �B�~���͸���,server���s�b������	
			bool					m_isServerMove				:1;			// �O�_�O Server ����ʪ���
			bool					m_orientToSurfaceForword	:1;			// ���e�����,������
		};
	};

	// �O���Ҧ����[����
	map<IBaseSprite*, int>			m_attachSprite;

	bool							m_disableUpdateEntity;				// 
};
