#pragma	  once

#include <map>
#include <list>
#include <vector>

using namespace std;

class CRoleSprite;

#define MAX_ROLE_NAME					63
#define MAX_TITLE_TEXT					63
#define WALK_SLOW_SCALE					3.0f
#define RUN_BACK_SCALE					1.3f
#define WALK_BACK_SCALE					1.0f
#define BASIC_WALK_HIGHT				3.0f

#define NEXT_MOVETIME					1000.0f
#define DEF_ATTACK_HIT_EVENT			9999
#define MAX_IMPLEMENT_TOUCH_RANGE		50.0f

#define PAPERDOLL_PART_NAME_HEAD			"head"
#define PAPERDOLL_PART_NAME_HAIR			"hair"
#define PAPERDOLL_PART_NAME_HELMET			"helmet"
#define PAPERDOLL_PART_NAME_SHOULDER		"shoulder"
#define PAPERDOLL_PART_NAME_TORSO			"torso"
#define PAPERDOLL_PART_NAME_HAND			"hand"
#define PAPERDOLL_PART_NAME_BELT			"belt"
#define PAPERDOLL_PART_NAME_LEG				"leg"
#define PAPERDOLL_PART_NAME_FOOT			"foot"
#define PAPERDOLL_PART_NAME_BACK			"back"

#define BONE_SCALE_NAME_BODY				"Body"
#define BONE_SCALE_NAME_HEAD				"Head"
#define BONE_SCALE_NAME_BREASTS				"Breasts"
#define BONE_SCALE_NAME_CHEST				"Chest"
#define BONE_SCALE_NAME_PELVIS				"Pelvis"
#define BONE_SCALE_NAME_ARM					"Arms"
#define BONE_SCALE_NAME_FOREARM				"Forearm"
#define BONE_SCALE_NAME_HAND				"Hand"
#define BONE_SCALE_NAME_LEGS				"Legs"
#define BONE_SCALE_NAME_CALF				"Calf"
#define BONE_SCALE_NAME_FOOT				"Foot"

#define	SOUND_WEAPON_UNARMED				"unarmed"
#define	SOUND_WEAPON_AXE					"axe"
#define	SOUND_WEAPON_BLADE					"blade"
#define	SOUND_WEAPON_CLUB					"club"
#define	SOUND_WEAPON_DAGGER					"dagger"
#define	SOUND_WEAPON_HAMMER					"hammer"
#define	SOUND_WEAPON_STAFF					"staff"
#define	SOUND_WEAPON_2H_AXE					"2h_axe"
#define	SOUND_WEAPON_2H_BLADE				"2h_blade"
#define	SOUND_WEAPON_2H_HAMMER				"2h_hammer"

#define	SOUND_ARMOR_FLESH					"flesh"
#define	SOUND_ARMOR_CHAIN					"chain"
#define	SOUND_ARMOR_PLATE					"plate"
#define	SOUND_ARMOR_STONE					"stone"
#define	SOUND_ARMOR_WOOD					"wood"
#define	SOUND_ARMOR_SHIELD					"shield"

enum RoleTakeItemType {
	eRoleTakeItem_None				= 0,
	eRoleTakeItem_Unarmed,			// ���M
	eRoleTakeItem_Blade,            // �C
	eRoleTakeItem_Dagger,           // �P��
	eRoleTakeItem_Wand,             // �v��
	eRoleTakeItem_Axe,              // ��
	eRoleTakeItem_Bludgeon,         // ��Ҵ�
	eRoleTakeItem_Claymore,         // ����C
	eRoleTakeItem_Staff,            // ��
	eRoleTakeItem_2H_Axe,           // �����
	eRoleTakeItem_2H_Hammer,        // ������
	eRoleTakeItem_Polearm,			// �j(����)
	eRoleTakeItem_Bow,				// ���{�Z�� �}
	eRoleTakeItem_CossBow,			// ���{�Z�� �Q�r�}
	eRoleTakeItem_Gun,				// ���{�Z�� �j
	eRoleTakeItem_Ammo_Bow,			// �l�u �}
	eRoleTakeItem_Ammo_CossBow,		// �l�u �Q�r�}
	eRoleTakeItem_Ammo_Gun,			// �l�u �j
	eRoleTakeItem_Throw,			// ���Y���Z��
	eRoleTakeItem_Lumbering,		// ����
	eRoleTakeItem_Herbalism,		// ����
	eRoleTakeItem_Mining,			// ���q
	eRoleTakeItem_Fishing,			// ����

	eRoleTakeItem_Shield,			// ��
	eRoleTakeItem_Implement,		// �������(�k����)
};


enum RoleCheckedStatus {
	eChecked_Null					= 0,
	eChecked_SayPlot,
	eChecked_TouchPlot,
	eChecked_RangePlot,
	eChecked_QuestNPC,						// ���� Quest �� NPC
	eChecked_QuestObj,						// Quest Obj
	eChecked_Mine,							// �Ķ�����
	eChecked_Herb,							// �Ķ�����
	eChecked_Lumber,						// �Ķ�����
	eChecked_Flag,
	eChecked_Shop,							// �ӤH ( �]�t �ө� / �Ȧ� / ���� )
	eChecked_Mail,							// �l��
	eChecked_FurnitureNormal,				// �@��í�
	eChecked_FurnitureStorage,              // �í� �_�c ��[
 	eChecked_FurnitureSound,                // �í� �d�n��
	eChecked_Maid,							// �k��
	eChecked_Servant,						// �k��
	eChecked_GuildHousesBuildPoint,			// ���|�ΰѦ��I
	eChecked_Tomb,							// Tomb
	eChecked_Player,						// Player
};

enum QuestDisplay {
	eQuest_Display_Null				= 0,
	eQuest_Display_Get_Gary,
	eQuest_Display_UnfinishedQuest,				// ����������
	eQuest_Display_AvailableRepeatQuest,		// ����(�C��)�i������
	eQuest_Display_Public_AvailableQuest,
	eQuest_Display_AvailableQuest,				// ���`�i������
	
	eQuest_Display_FinishedRepeatQuest,			// ����(�C��)��������
	eQuest_Display_Public_FinishedQuest,
	eQuest_Display_FinishedQuest,				// ���`��������

	eVendor_Display_WeaponShop,					// �Z���ӤH
	eVendor_Display_ArmorShop,					// ����ӤH
	eVendor_Display_AuctionHouse,				// ����
	eVendor_Display_FoodShop,					// ����
};

enum RoleSpriteActionType {	
	eAction_Null					= 0,
	eAction_Idle,
	eAction_Fight_Idle,
	eAction_Sit_Idle,
	eAction_Sleep_Idle,
	eAction_Dead_Idle,

	eAction_Move,
	eAction_Attack,	
	eAction_Hurt,
	eAction_Parry,
	eAction_Dodge,
	eAction_Sit,
	eAction_StandUp,
	eAction_Sleep,
	eAction_Dead,
	eAction_Revive,
	eAction_Holster,
	eAction_Unholster,
	eAction_Gather_Delay,
	eAction_Casting
};

enum RoleHandsType	{
	eRoleHands_None					= -1,
	eRoleHands_MainHand				= 0,
	eRoleHands_OffHand,
	eRoleHands_Ranged,	
	eRoleHands_Crafting,
	eRoleHands_Arrow,
	eRoleHands_Last,
};

enum SpriteMoveType {
	eRoleSpriteMove_Normal			= 0,
	eRoleSpriteMove_Charge,
	eRoleSpriteMove_StrikeBack,
};

//-----------------------------------------------------------------------------
//struct RoleBoneScaleInfo
//{
//	RoleBoneScaleInfo()											{ name.clear(); scale = 1.0f; }
//	RoleBoneScaleInfo(const char* boneName, float boneScale)	{ name = boneName; scale = boneScale; }
//
//	string			name;
//	float			scale;
//};

//-----------------------------------------------------------------------------
// ���|���ʸ`�I���c
struct PathMoveNode
{
	CRuVector3		vec;
	float			time;
};

//-----------------------------------------------------------------------------
struct RoleBuffInfo
{
	RoleBuffInfo()		
	{
		source		= 0;
		id			= 0;
		effect		= NULL;
		armEffect	= NULL;
		time		= 0.0f;
		recycleTime = 0.0f;
		remnantTime = 0.0f;
		dotCount	= 0;
		faceOff		= false;
		mount		= false;
		magicLv		= 0;
		dotRate		= 1.0f;
		locked		= false;
		ownerID		= 0;
		proxyLoad	= false;
	}

	string				sourceName;					// �I�k�̦W��
	int					source;						// �ӷ�
	int					id;							// ���޸��
	CEffectSprite*		effect;						// ���ͯS��
	CRuEntity*			armEffect;					// ���u���S��
	float				destructTime;				// �R���ɶ�
	float				time;						// BUFF �ɶ�
	float				recycleTime;				// DOT���S��Ĳ�o�ɶ�
	float				remnantTime;				// DOT���S�ĳѩ�ɶ�
	int					dotCount;					// DOT�k�N���Φ���
	bool				faceOff;					// �ܨ�
	bool				mount;						// ���M
	int					magicLv;					// Buff�ֿn����
	float				dotRate;					// �a��DOT�k�N,�I���

	bool				locked;						// �@�묰�e�X����BUFF
	bool				proxyLoad;					// ������J
	int					ownerID;				
};

//-----------------------------------------------------------------------------
struct DOTBuffValueInfo
{
	const char*			sourceName;					// �I�k�̦W��
	int					source;						// �ӷ�����s��
	int					id;							// �k�N�s��
	int					type;						// ����(0 HP, 1MP, 2WarriorSP, 3RangerSP, 4RogueSP)
	int					value;						// �ƭ�
	bool				absorb;						// �ˮ`�Q�l��
	
};

//-----------------------------------------------------------------------------
struct RoleHandsEntity
{
	RoleHandsType		type;						// ����
	CRuEntity*			entity;						// 3D����
	CRuEntity*			enchantEntity;				// �Z�����]�S��
	SpriteAttachPoint	point;						// 3D�Ѧ��I
	int					id;							// ItemDB�s��
};

//-----------------------------------------------------------------------------
typedef union
{
	int		temp[16];
	struct {
		char	invisible;					// ����
		char	detectInvisible;			// ��������
		char	sneak;						// ���
		char	detectSneak;				// �������
		char	lockedMove;					// �����
		char	blind;						// ����
		char	hitDown;					// �˦a
		char	chaos;						// �V��
		char	fear;						// ����
		char	sleep;						// �ίv
		char	dizzy;						// �w�t
		char	pvp;						// ���aPVP
		char	protect;					// �O�@PVP
		char	inverse;					// ���k�ۤ�
		char	freeze;						// ����
		char	criminal;					// �o��
		char	disableJobSkill;			// �L�k�ϥ�¾�~�ޯ�
		char	waterWalk;					// ���W�樫
		char    cantCTRL;					//�L�k����
	};
}SpriteSpecialEffect;

enum SpriteControllerType
{
	eSpriteController_None			= 0,	// �@��
	eSpriteController_Alpha,				// Alpha ����
	eSpriteController_Scale,				// Scale ����
};

//-----------------------------------------------------------------------------
struct SpriteController
{
	SpriteControllerType type;	
	float time;
	float update;
	float begin;
	float end;
};

//-----------------------------------------------------------------------------
struct SpriteActorQueue
{
	union {
		int _data;
		struct {
			unsigned short layer;
			unsigned short actor;
		};
	};
};

#ifdef KALYDO

class CRoleSprite;
//-----------------------------------------------------------------------------
struct CDownloadHandStruct
{
	CRoleSprite* sprite;
	RoleHandsType type;
	CRuString fileName;
	CRuString point;
	int id;
	int level;
	int dependencies;
	bool enchanted;

	bool spriteDeleted;
};
#endif // KALYDO

//-----------------------------------------------------------------------------
class CRoleSprite : public IBaseSprite
{
public:
	static	CRuFusion_ActorState	MotionToActorState(const char* motion);

public:
	CRoleSprite(bool isAddToContainer=true);
	virtual ~CRoleSprite();
	
	BOOL							HandleEvent_DequeueAction(RuEventArgs *eventArgs);
	virtual BOOL					HandleProxyLoadComplete(RuEventArgs *eventArgs);

	virtual	void					Update(float elapseTime);	
	virtual	void					Render();
	virtual	void					ReceiveDestruct(IBaseSprite* pSprite);	
	virtual	void					MotionEvent(int type);
	virtual	void					SetObjectType( SpriteObjectType type );
	virtual bool					SetObjectId(int id, bool deferredLoad = false);
	virtual	void					ReleaseEntity();	
	virtual bool					LoadRuTempEntity(const char* actName);
	virtual bool					SetTempObjectId(int id, bool takeWeapon=false);
	virtual	bool					CheckRenderState();

	virtual	void					SetPos(CRuVector3 &pos);
	virtual void					SetMoveSpeed(float speed);
	virtual float					GetMoveSpeed();
	virtual	void					SetDirection(CRuVector3 direction, bool IsForce = false, bool IsInitial = false);
	virtual	void					SetForwardVector(CRuVector3& forwardVector);
	virtual void					UpdateOrientation();

	virtual	CRuEntity*				FindLinkEntity(int point);
	virtual bool					GetAttachPos(int point, CRuVector3& pos);
	virtual CRuVector3				GetAttachPos(const char* pointName)		{ return IBaseSprite::GetAttachPos(pointName); }
	virtual bool					GetShootAttachPos(CRuVector3& pos);
	//virtual	bool					CreateFloorEntity(const char* actName);
	//virtual	void					ReleaseFloorEntity();

	// Sprite linking
	virtual void					UnlinkChildSprite();
	virtual void					SeverSpriteLinks();

	CRuFusion_Mime*					GetRuFusionMime()						{ return m_pRuMimeEntity; }
	CRuFusion_Mime*					GetRuTempFusionMime()					{ return m_pRuTempMimeEntity; }
	CRuFusion_Actor*				GetRuActor()							{ return m_pRuActor; }
	CRuFusion_Actor*				GetRuTempActor()						{ return m_pRuTempActor; }
	CRuFusion_Actor*				GetCurrentRuActor()						{ return (m_pRuTempActor) ? m_pRuTempActor : m_pRuActor; }

	void							AddRefCount()							{ m_refCount++; }
	void							DelRefCount(bool immediately=false);
	int								RefCount()								{ return m_refCount; }

	// Control proxies
	CRoleSprite*					GetProxyControlSprite();

	// Others
	virtual void					SetOrientMode(CRuFusion_Actor::OrientMode orientMode);

	// Movement	
	CRuVector3						ComputeMoveVector(float forwardDelta, float rightDelta);
	void							SetMoveVector(CRuVector3& vec, float time=0.0f, bool isInternal=false, bool boTargetMove=false);
	void							ClearMoveVector();
	CRuVector3						GetMoveVector()							{ return m_moveVector; }
	float							GetMoveTime()							{ return m_moveTime; }	
	bool							Jump();
	bool							ForcedJump(const CRuVector3 &latentVelocity);
	CRuVector3						GetLatentVelocity();
	void							ChargeToPos(CRuVector3& pos);
	void							StrikeBack(CRuVector3& pos);
	void							SetTargetPos(CRuVector3& pos, bool isForward=true);
	CRuVector3						GetTargetPos()							{ return m_targetPos; }
	void							SetCorrectDir(CRuVector3 dir);
	SpriteMoveType					GetMoveType()							{ return m_moveType; }
	void							SetAnchorHeight(float height)			{ m_anchorHeight = height; }
	float							GetAnchorHeight()						{ return m_anchorHeight; }
	void							EnableTorsoTurn(bool enableTorsoTurn);

	// Fade
	void							SetFade(bool isFadeOut, float durativeTime);	

	// ���O
	void							SetGravityMode(bool state);
	bool							GetGravityMode()						{ return m_controlState.gravityState; }

	// Widget_Line
	void							CreateWidgetLine();
	void							ReleaseWidgetLine();

	// entity object & set paperdoll
	virtual	bool					SetPaperdollPart(const char* partName, int id);
	virtual	bool					SetPaperdollPart(const char* partName, const char* name=NULL);
	virtual	const char*				GetPaperdollPart(const char* partName);
	virtual	bool					SetPaperdollColor(const char* partName, int mainColor, int secondColor, bool boSetBaseColor=true);
	virtual	bool					GetPaperdollColor(const char* partName, int* outColor);
	virtual void					SetDyeFXColors(int* colors);
	virtual void					UpdateDyeFXColors();
	virtual	void					PaperdollBuild(bool allowDelayedLoad = true);
	virtual void					PaperdollBuildCheck(bool allowDelayedLoad = true);

	virtual	bool					SetBoneScale(const char* boneName, float scale);
	virtual	map<string,float>		GetBoneScaleInfo()					{ return m_boneScaleInfo; }
	virtual int						GetNumBoneScales();
	virtual float					GetBoneScaleByIndex(int index);
	virtual const char*				GetBoneScaleNameByIndex(int index);
	virtual void					ReapplyBoneScaleSettings();

	//virtual	void					CopyPaperdoll(CRoleSprite* source);
	virtual	void					SetHandsObjectId(RoleHandsType type, int id, int level=0, bool hold=true);
	virtual int						GetHandsObjectId(RoleHandsType type);
	virtual	void					SetTorsoID(int id)						{ m_torso = id; }
	virtual	int						GetTorsoID()							{ return m_torso; }

	void							SetCombatState(bool isCombat);	
	bool							GetCombatState()						{ return m_state.isCombat; }
	void							SetDeadState(bool isDead);
	bool							GetDeadState()							{ return m_state.isDead; }
	void							SetSitState(bool isSit);
	bool							GetSitState()							{ return m_state.isSit; }
	void							SetSitChairState(bool isSit);
	bool							GetSitChairState()						{ return m_state.isSitChair; }
	void							SetWalkState(bool isWalk);
	bool							GetWalkState()							{ return m_state.isWalk; }
	void							SetUnholsterState(bool isUnholster);
	bool							GetUnholsterState()						{ return m_state.isUnholster; }		
	void							SetWeaponPosition(bool isUnholster, bool force=false);	
	int								GetTakeItemType(int item);
	int								GetStateFlag()							{ return m_state.value; }

	virtual void					SetRangedMotion(const char* motion);
	void							SetCombatMotion();
	void							SetAttackMotion(bool offHand=false, CRuACT_ActionEvents* actionEvents=NULL, bool unarmed=false);
	CRuFusion_ActorState			GetParryMotion();

	// character base data
	void							SetName( const char* nameStr, bool showWidget=true );	
	const char*						GetName();
	void							SetOriginalName( const char* nameStr );
	const char*						GetOriginalName();
	void							SetTitleText(const char* titleStr);
	const char*						GetTitleText();
	void							SetGuildText(const char* guildStr);
	const char*						GetGuildText();
	void							SetDebugOut1( const char* debugStr );
	void							SetWidgetTexture();		

	// Spell
	void							SetSpellMotion(const char* motionName);
	void							BeginSpell(int id, bool hasMotion, CRoleSprite* target);
	void							EndSpell();
	bool							CastSpell(CRuACT_ActionEvents* actionEvents=NULL);
	int								GetSpellID()							{ return m_spellID; }
	void							SetCastingTime(float time, float nowTime=0.0f);
	float							GetCastingMaxTime()						{ return m_castingTime; }
	float							GetCastingCurrentTime()					{ return m_currentTime; }

	// Create Gather Effect
	void							BeginCastingEffect(const char* actName, const char* point, bool sound, float scale=1.0f);
	void							EndCastingEffect();
	void							UpdateCastingEffect( float elapseTime );
	void							ClearCastingEffect();

	// Buff
	void							AddBuff(int source, int id, float time, int magicLv, const char* name, int ownerID=-1, float dotRate=1.0f);
	void							DeleteBuff(int id, int buffOwnerID, bool immediately=false);
	void							EraseBuff(int id,int BuffOwnerID);		
	void							ClearBuff();

	void							AddDisplayBuff(int magicId, int magicLv, float time);
	void							ClearDisplayBuff();

	RoleBuffInfo*					GetBuffInfo(int index);
	int								GetBuffSize()							{ return (int)m_buffInfo.size(); }
	int								GetBuffHelpfulIndex(int order, bool getHide=false);
	int								GetBuffHarmfulIndex(int order, bool getHide=false);
	bool							IsBuffChanged()							{ return true; }
	bool							IsDisableJobSkill()						{ return m_specialEffect.disableJobSkill > 0; }

	bool							IsCantCtrl()						{ return m_specialEffect.cantCTRL > 0; }

	// Target
	void							SetTargetSprite(CRoleSprite* pTarget);
	CRoleSprite*					GetTargetSprite()						{ return m_targetSprite; }	

	// 
	void							SetMasterSprite(CRoleSprite* pMaster, bool isPlayerPet=true);
	CRoleSprite*					GetMasterSprite()						{ return m_masterSprite; }
	void							SetPetSprite(CRoleSprite* pPet);
	CRoleSprite*					GetPetSprite();
	void							InsertChildSprite(CRoleSprite* pChild);
	void							RemoveChildSprite(CRoleSprite* pChild);
	void							SetMasterID(int id)						{ m_masterId = id; }
	int								GetMasterID()							{ return m_masterId; }
	bool							IsPlayerPet()							{ return m_controlState.playerPet; }

	void							SetVocation(Voc_ENUM main, Voc_ENUM sub);
	void							SetSex(Sex_ENUM sex)					{ m_sex = sex; }
	void							SetRace(Race_ENUM race)					{ m_race = race; }
	void							SetCampID(CampID_ENUM campId)			{ m_campId = campId; }
	void							SetDBID(int DBID)						{ m_DBID = DBID; }
	void							SetWorldGUID(int worldGUID)				{ m_worldGUID = worldGUID; }
	void							SetWorldID(int worldID)					{ m_worldID = worldID; }
	void							SetHP(int hp);
	void							SetMaxHP(int maxHp);
	void							SetMP(int mp);
	void							SetMaxMP(int maxMp);
	void							SetSP(int sp);
	void							SetMaxSP(int maxSp);
	void							SetLevel(int main, int sub);
	int								DeductHP(int hp, bool boUpdate=true);
	int								DeductMP(int mp, bool boUpdate=true);
	int								DeductSP(int sp, bool boUpdate=true);
	int								GetManaType();
	int								GetSkillType();
	Voc_ENUM						GetVocation()							{ return m_vocation; }
	Voc_ENUM						GetVocationSub()						{ return m_vocationSub; }
	Sex_ENUM						GetSex()								{ return m_sex; }
	Race_ENUM						GetRace()								{ return m_race; }
	CampID_ENUM						GetCampID()								{ return m_campId; }
	int								GetDBID()								{ return m_DBID; }
	int								GetWorldGUID()							{ return m_worldGUID; }
	int								GetWorldID()							{ return m_worldID; }
	int								GetLastHP()								{ return m_lastHp; }
	int								GetHP()									{ return m_hp; }
	int								GetMP()									{ return m_mp; }
	int								GetSP()									{ return m_sp; }
	int								GetMaxHP()								{ return m_maxHp; }
	int								GetMaxMP()								{ return m_maxMp; }
	int								GetMaxSP()								{ return m_maxSp; }	
	int								GetLevel()								{ return m_level; }
	int								GetLevelSub()							{ return m_levelSub; }
	int								GetNameColor()							{ return m_nameColor; }

	float							GetRadius()								{ return GetRuEntity()->GetWorldBounds().Radius(); }

	// quest touch
	void							SetQuestInfo( int id, int touch, int area );	
	int								GetQuestId()							{ return m_questId; }
	void							SetTouchRange(int touchRange)			{ m_touchRange = touchRange; }
	int								GetTouchRange();
	void							SetAreaRange(int areaRange)				{ m_areaRange = areaRange; }
	int								GetAreaRange();
	void							SetQuestDisplay( QuestDisplay display );
	QuestDisplay					GetQuestDisplay()						{ return m_questDisplay; }	
	bool							HasQuestEvent();
	bool							CheckTouchRange(float distance);

	void							SetMinimapIcon( int iIconID )			{ m_minimapIcon = iIconID; }
	int								GetMinimapIcon()						{ return m_minimapIcon; }

	void							SetQuestMinimapIcon( int iIconID )		{ m_QuestminimapIcon = iIconID; }
	int								GetQuestMinimapIcon()					{ return m_QuestminimapIcon; }


	// Guild
	void							SetGuildID(int guildID)					{ m_guildID = guildID; }
	int								GetGuildID()							{ return m_guildID; }

	// 
	void							SetCheckedStatus( int value )			{ m_checkedStatus = value; }
	int								GetCheckedStatus()						{ return m_checkedStatus; }	
	
	virtual	void					SetActorState(CRuFusion_ActorState actorState, int layer = 0, bool disableMovement=false, CRuACT_ActionEvents *actionEventsOut=NULL);
	virtual	CRuFusion_ActorState	GetActorState(int layer=1);
 	virtual	CRuFusion_ActorState	GetDisplayActorState(); //���o��ܥX�Ӫ�ActorState
	virtual	void					PushActorQueue(CRuFusion_ActorState actorState, float delayTime);
	virtual	void					UpdateActorQueue(float elapsedTime);
	virtual void					SetDefaultIdleAction(CRuFusion_Mime_Action defaultIdleAction);
	
	void							SetTouchState(bool isState)				{ m_controlState.touch = isState; }
	bool							GetTouchState()							{ return m_controlState.touch; }
	void							SetTouchInState(bool isState)			{ m_controlState.touchIn = isState; }
	bool							GetTouchInState()						{ return m_controlState.touchIn; }

	void							SetCursorState(bool isState)			{ m_controlState.cursor = isState; }
	bool							GetCursorState();

	void							SetCursorEnableState(bool isState)		{ m_controlState.cursorEnable = isState; }
	bool							GetCursorEnableState()					{ return m_controlState.cursorEnable; }

	void							SetTargetState(bool isState)			{ m_controlState.target = isState; }
	bool							GetTargetState()						{ return m_controlState.target; }
	void							SetNameVisible(bool isShow);
	bool							GetNameVisible()						{ return m_controlState.nameVisible; }
	void							SetTitleVisible(bool isShow);
	bool							GetTitleVisible()						{ return m_controlState.titleVisible; }
	void							SetGuildVisible(bool isShow);
	bool							GetGuildVisible()						{ return m_controlState.guildVisible; }
	void							SetNameVisibleForce(bool isShow);
	bool							GetNameVisibleForce()					{ return m_controlState.nameVisibleForce; }	
	void							SetTitleIconVisible(bool isShow);
	bool							GetTitleIconVisible()					{ return m_controlState.titleIconVisible; }
	void							SetTouchPlotState( bool isState )		{ m_controlState.touchPlot = isState; }
	bool							GetTouchPlotState()						{ return m_controlState.touchPlot; }
	void							SetRangePlotState( bool isState )		{ m_controlState.rangePlot = isState; }
	bool							GetRangePlotState()						{ return m_controlState.rangePlot; }
	void							SetRangePlotTrigger( bool isState )		{ m_controlState.rangePlotTrigger = isState; }
	bool							GetRangePlotTrigger()					{ return m_controlState.rangePlotTrigger; }
	void							SetCollisionTrigger( bool isState )		{ m_controlState.CollisionPlot = isState; }
	bool							GetCollisionTrigger()					{ return m_controlState.CollisionPlot; }
	void							SetSneakAlphaState(bool alpha)			{ m_controlState.sneakAlpha = alpha; }
	bool							GetSneakAlphaState()					{ return m_controlState.sneakAlpha; }
	void							SetSneakHiddenState(bool hidden)		{ m_controlState.sneakHidden = hidden; }
	bool							GetSneakHiddenState()					{ return m_controlState.sneakHidden; }
	bool							GetRideState()							{ return m_controlState.ride; }
	void							SetPartyState(bool inParty);
	bool							GetPartyState()							{ return m_controlState.party; }
	void							SetRaidState(bool inRaid);
	bool							GetRaidState()							{ return m_controlState.raid; }
	void							SetStaticObjectState(bool isStatic)		{ m_controlState.staticObject = isStatic; }
	bool							GetStaticObjectState()					{ return m_controlState.staticObject; }
	void							SetContainerState(bool state)			{ m_controlState.inContainer = state; }
	bool							GetContainerState()						{ return m_controlState.inContainer; }
	void							SetCorpseState(bool state)				{ m_controlState.corpse = state; }
	bool							GetCorpseState()						{ return m_controlState.corpse; }
	void							SetBattleGround(bool inBattleGround)	{ m_controlState.inBattleGround = inBattleGround; }
	bool							IsInBattleGround()						{ return m_controlState.inBattleGround; }

	void							SetAFK( bool isState )					{ m_playerState.AFK = isState; }
	bool							GetAFK()								{ return m_playerState.AFK; }
	void							SetBotting( bool isState )				{ m_playerState.Dig = isState; }
	bool							GetBotting()							{ return m_playerState.Dig; }
	void							SetFindParty( bool isState )			{ m_playerState.FindParty = isState; }
	bool							GetFindParty()							{ return m_playerState.FindParty; }	

	void							LockedDirection(bool locked)			{ m_controlState.lockedDirection = locked; }
	bool							IsLockedDirection()						{ return m_controlState.lockedDirection; }

	void							SetEnemyHidden( bool hidden )			{ m_controlState.enemyHide = hidden; }
	bool							GetEnemyHidden()						{ return m_controlState.enemyHide; }

	void							SetRideAttackState( bool enable )		{ m_controlState.rideAttack = enable; }
	bool							GetRideAttackState()					{ return m_controlState.rideAttack; }

	void							SetRidePlayAttackMotion( bool enable )	{ m_controlState.ridePlayAttackMotion = enable; }
	bool							GetRidePlayAttackMotion()				{ return m_controlState.ridePlayAttackMotion; }

	void							ScriptHide()							{ m_controlState.isScriptHide = true; }
	void							ScriptShow()							{ m_controlState.isScriptHide = false; }
	bool							IsScriptHide()							{ return m_controlState.isScriptHide; }

	// �O�_�i�H��������
	void							SetObjectAttackState(bool isState);
	bool							GetObjectAttackState()					{ return m_controlState.attack; }

	void							SetCanAttackState(bool isState);
	bool							GetCanAttackState();

	// �O�_��ۤv������
	void							SetEnemyState(bool isState);
	bool							GetEnemyState();

	// �M���ؼ�
	void							SetDuelState(bool state);
	bool							GetDuelState()							{ return m_controlState.duel; }

	// PVP �ϰ�
	void							SetPVPZoneState(bool state);
	bool							GetPVPZoneState()						{ return m_controlState.pvpZone; }	

	// ���|����
	void							SetPVPGuildState(bool state);
	bool							GetPVPGuildState()						{ return m_controlState.pvpGuild; }

	// �}��۹����Y
	void							SetCampAttackState(bool state);
	bool							GetCampAttackState()					{ return m_controlState.campAttack; }
	void							SetCampEnemyState(bool state);
	bool							GetCampEnemyState()						{ return m_controlState.campEnemy; }

	// PVP ���A(���a�}��)
	void							SetPVPState(bool state);
	bool							GetPVPState()							{ return m_controlState.pvpOpen; }	
	bool							GetPKBuffState();
	bool							GetCriminalBuffState();
	bool							GetProtectBuffState();	

	void							SetDisableQuestState(bool isState);
	bool							GetDisableQuestState()					{ return m_controlState.DisableQuest; }	

	void							SetPlayerState(PlayerStateStruct state)	{ m_playerState = state; }
	PlayerStateStruct				GetPlayerState()						{ return m_playerState; }

	void							SetCursorMode( int iMode )				{ m_cursorMode = iMode; }
	int								GetCursorMode();

	// �T����V
	void							SetDisableRotate(bool disableRotate)	{ m_controlState.disableRotate = disableRotate; }
	bool							GetDisableRotate()						{ return m_controlState.disableRotate; }

	// �N�Z���@�����b��W
	void							SetAlwaysHoldWeapon(bool state);
	bool							GetAlwaysHoldWeapon()					{ return m_controlState.alwaysHoldWeapon; }
	void							SetForceCombatMotion(bool state);
	bool							GetForceCombatMotion()					{ return m_controlState.forceCombatMotion; }

	void							SetHideMasterName(bool state)			{ m_controlState.hideMasterName = state; }
	bool							GetHideMasterName()						{ return m_controlState.hideMasterName; }

	// ���
	void							SetBloodBarHidden(bool hide)			{ m_controlState.hiddenBloodBar = hide; }
	bool							IsBloodBarHidden()						{ return m_controlState.hiddenBloodBar; }

	// �ʧ@
	void							SetDisableAction(bool disableAction)	{ m_controlState.isDisableAction = disableAction; }
	bool							GetDisableAction()						{ return m_controlState.isDisableAction; }

	// ����
	bool							GetMoveControl();

	// sound 
	const char*						GetSoundWeaponType(bool offHand=false);
	const char*						GetSoundArmorType();

	// Special Effect
	SpriteSpecialEffect				GetSpecailEffect()						{ return m_specialEffect; }

	// collector FX
	void							CreateCollectorFX(CRuVector3 position);
	void							ReleaseCollectorFX();

	bool							CheckSelectState();

	// �˳Ʋ�2��(�@�묰�Ķ��u��)���~
	void							AttachCraftingTool(const char* point=NULL);
	void							DetachCraftingTool();

	// �W�[���
	void							AddController(SpriteControllerType type, float time, float begin, float end);
	void							UpdateController(float elapseTime);
	void							ClearController(SpriteControllerType type);

	// ���M
	void							CreateMountSprite(int orgObjID);
	void							ReleaseMountSprite();
	CRoleSprite*					GetMountSprite()								{ return m_mountSprite; }
	void							ResetMount();
	bool							AttachMountSprite(CRoleSprite* parent, const char* point);
	void							DetachMountSprite();

	bool							IsInWater();
	void							CreateOrnamentEntity(int id, const char* insignia, int insigniaMainColor, int insigniaOffColor, const char* banner, int bannerMainColor, int bannerOffColor, bool motionRelease=false);
	void							ReleaseOrnamentEntity();
	void							AttachOrnamentEntity(bool motionRelease=false);
	void							DetachOrnamentEntity(bool immediately=true);
	void							UpdateOrnamentEntity();
	bool							PlayMotionOrnament(const char* motionName);
	int								GetOrnamentID()									{ return m_ornamentID; }
	void							CreateBackEntity(int id);
	void							ReleaseBackEntity();
	void							AttachBackEntity();
	void							DetachBackEntity();

	// ����S�Ī���
	void							CreateCorpseEffect();
	void							ReleaseCorpseEffect();

	// ���c��
	void							SetGoodEvilType(GoodEvilTypeENUM type, float value);
	GoodEvilTypeENUM				GetGoodEvilType()									{ return m_goodEvilType; }
	float							GetGoodEvilValue()									{ return m_goodEvilValue; }

	void							SetRisingimpulse(float risingimpulse);
	float							GetRisingimpulse()									{ return m_risingimpulse; }
	void							SetGravityAcceleration(float gravityAcceleration);
	float							GetGravityAcceleration()							{ return m_gravityAcceleration; }

	// Raid Mark Target
	void							SetRaidMarkTarget(int markNumber);

	float							GetAttackRange()									{ return m_attackRange; }
	float							GetClientTouchRange();
	
	virtual bool					AttachSprite(IBaseSprite* sprite, int point);

	virtual void					FollowSprite(CRoleSprite* targetSprite, const char* targetPoint, const char* point);
	virtual void					FollowSprite(CRoleSprite* targetSprite, float offset=0.0f);
	virtual void					UpdateFollow(float elapsedTime);
	virtual CRoleSprite*			GetFollowSprite()									{ return m_followSprite; }

	void							SetOriginalObjectId(int objectId)					{ m_originalObjectId = objectId; }
	int								GetOriginalObjectId()								{ return m_originalObjectId; }
	int								GetOriginalMountId()								{ return m_originalMountId; }
	void							SetLock(LookStruct& lock);
	LookStruct&						GetLock()											{ return m_originalLock; }	

	void							RefreshWidget()										{ m_isNeedResetWidget = true; }
	void							CreateEquipmentInfo(void* data, int size);
	void							ReleaseEquipmentInfo();
	void*							GetEquipmentInfo()									{ return m_equpmentInfo; }

	// ���W�樫
	virtual void					SetAboveWaterState(bool enable);
	virtual bool					IsAboveWater();

	// �Q���W�Ť�
	virtual void					JumpEx();
	virtual bool					IsFloat()											{ return m_controlState.isFloat; }

	virtual int						GetAttachParentId()									{ return m_attachParentId; }
	virtual int						GetMountCarryCount();

	virtual void					SetPriorityAttack(bool state)						{ m_controlState.isPriorityAttack = state; }
	virtual bool					GetPriorityAttack()									{ return m_controlState.isPriorityAttack; }

	// ����
	virtual void					SetImplementMaxNum(int maxNum);
	virtual int						GetImplementMaxNum()								{ return m_implementMaxNum; }
	virtual void					SetImplementCount(int count);
	virtual int						GetImplementCount()									{ return m_implementCount; }	
	virtual void					AttachImplement(CRoleSprite* sprite, SpriteAttachPoint attachPoint, CRuFusion_Mime_Action idleMotion, int sitPos);
	virtual void					DetachImplement();
	virtual void					SetImplementSprite(CRoleSprite* sprite);
	virtual CRoleSprite*			GetImplementSprite()								{ return m_implementSprite; }
	virtual void					SetImplementState(bool isImplement)					{ m_isImplement = isImplement; }	
	virtual bool					IsImplement()										{ return m_isImplement; }
	virtual void					SetImplementAlignment(bool isAlignment)				{ m_isImplementAlignment = isAlignment; }
	virtual bool					IsImplementAlignment()								{ return m_isImplementAlignment; }
	virtual bool					IsDisableWagon();

	virtual void					SetParentMountInfo(int parentMountId, CRuVector3 parentMountOffset);
	virtual CRuVector3				GetParentMountOffset()								{ return m_parentMountOffset; }
	virtual int						GetParentMountId()									{ return m_parentMountId; }

	virtual void					SetImmediateScale(float scale);
	virtual float					GetImmediateScale()									{ return m_immediateScale; }

	virtual void					SetAttachParent(CRoleSprite* parent, int point, bool boRotate = false);
	virtual CRoleSprite*			GetAttachParent()									{ return m_attachParentSprite; }
	virtual bool					IsTargetMove()										{ return m_boTargetMove; }

#ifdef KALYDO
	virtual void					CreateHandsEntityKalydo(RoleHandsType type, int id, int level, const char* point=NULL);
#endif // KALYDO
	virtual void					CreateHandsEntity(RoleHandsType type, int id, int level, const char* point=NULL);
	virtual void					ReleaseHandsEntity(RoleHandsType type);

	virtual void					SetHorizontal(bool isHorizontal);
	virtual bool					GetHorizontal()										{ return m_isHorizontal; }

	virtual int						PushCheckPosition(const CRuVector3& point);
	virtual void					ClearCheckPosition();

protected:
	virtual bool					QueryCollision(CRuVector3& beginPoint, CRuVector3& endPoint, CRuVector3& outCollisionPoint, bool boTerrain, bool spriteCollision, CRuEntity* ignoreEntity);
	virtual	void					AttachHandsEntity(RoleHandsType type, SpriteAttachPoint point);
	virtual CRuEntity*				LoadOrnamentEntity(int id, const char* insignia, int insigniaMainColor, int insigniaOffColor, const char* banner, int bannerMainColor, int bannerOffColor);
	
	virtual void					ReleaseWidget();
	virtual void					ReleaseQuestEntity();
	virtual	void					ReleaseTempEntity();
	virtual void					CreateFusionActor();
	virtual void					ReleasFusionActor();
	virtual void					CreateFusionMime();
	virtual void					ReleasFusionMime();
	virtual	void					UpdateBuff(float elapsedTime);
	virtual void					UpdateBuffEffect();
	float							WidgetAddRowText(const WCHAR* textStr, float scale, int color);
	void							WidgetAddClassIcon(int row, float width);
	void							WidgetAddGoodEvilIcon(int row, float width);
	float							WidgetAddRaidMarkIcon(int row);
	void							RestoreAction();
	float							ComputeFallingTime( CRuVector3& pos, CRuVector3& vec, float& h );
	void							UpdateTitleVisible();	
	void							EffectUpdate();	
	void							SetMoveType(SpriteMoveType type);
	void							CreateBuffEffect(GameObjDbStruct* itemDB, RoleBuffInfo* buff);
	void							InitialBackgroundActor();

	void							SetDefaultWalkingAction(CRuFusion_Mime_Action action);
	void							SetDefaultRunningAction(CRuFusion_Mime_Action action);
	void							SetDefaultBackAction(CRuFusion_Mime_Action action);

	CRuFusion_Mime_Action			GetDefaultWalkingAction();
	CRuFusion_Mime_Action			GetDefaultRunningAction();
	CRuFusion_Mime_Action			GetDefaultBackAction();

protected:
#ifdef KALYDO
	static void KalydoLoadHandsCallback(const char* fileName, TKResult result, void* id);
	void KalydoLoadHandsEntity(RoleHandsType type, const char* imageName, int level);

	vector<CDownloadHandStruct*>		m_DownloadStructs;
#endif // KALYDO

	// 3D Objects	
	CRuEntity*						m_pRuQuestEntity;			// Quest entity
	CRuController_GlobalAlpha*		m_pRuGlobalAlpha;			// �z���ȱ��
	CRuEntity_Widget_Label*			m_pRuWidget;				// 3D��r����
	CRuFusion_Mime*					m_pRuMimeEntity;
	CRuFusion_Actor*				m_pRuActor;					// �ʧ@����
	CRuFusion_Mime*					m_pRuTempMimeEntity;
	CRuFusion_Actor*				m_pRuTempActor;				// �ʧ@����
	CRuEntity_Widget_Line*			m_pRuWidgetLine[3];	
	CRuEntity_CollectorFX*			m_pRuCollectorFX;			// �ĪG���y
	CRuEntity*						m_pRuCollectorEntity;		// �ĪG���y�S�Ī���	
	CRuEntity*						m_pRuFlagEntity;			// �X�l�ĪG
	CRuEntity*						m_pRuOrnamentEntity;		// �˹��~
	CRuEntity*						m_pRuBackEntity;			// ���I�˹��~
	RoleHandsEntity					m_handsEntity[eRoleHands_Last];			// 0�D 1�� 2��L���� 3�}�b
	int								m_handsObjectId[eRoleHands_Last];		// ������~���		
	int								m_handsLevel[eRoleHands_Last];			// ������~���
	int								m_ornamentID;				// �˹��~�s��
	int								m_backID;					// �I�����~�s��
	string							m_ornamentMotion;			// �˹��~����ʧ@

	int								m_refCount;					// �ѦҭӼ�	
	float							m_bodyHeading;
	float							m_torsoHeading;
	float							m_anchorHeight;				// ���שw��
	float							m_moveTime;					// ���ʮɶ�
	float							m_attackRange;				// �����b�|
	bool							m_fadeOut;					// �H�X
	bool							m_isNeedBuild;				//
	bool							m_isWaitingForPaperdollCache;
	bool							m_isNeedResetWidget;
	CRuVector3						m_moveVector;				// ���ʦV�q
	CRuVector3						m_lastTargetPos;			// �O���W���ؼЮy���I
	CRuVector3						m_targetPos;				// �O���ؼЮy���I
	CRuVector3						m_nextTargetPos;			// �O���U�@���ؼЮy�I
	CRuVector3						m_correctDir;				// Server�̫�@���e�����V
	SpriteMoveType					m_moveType;					// ���ʪ��A
	RoleSpriteStatus				m_state;					// ���A�X��

	// �ʧ@ Queue
	map<int, float>					m_actorQueue;				// �ʧ@�Ȧs
	vector<SpriteActorQueue>		m_actorStateQueue;			// �b���󥼲��ͫe�Ȧs�ʧ@

	//
	vector<CEffectSprite*>			m_castingEffect;			// ����S��	
	CEffectSprite*					m_polyLineEffect;			// ���ަ��S��
	int								m_spellID;
	float							m_castingTime;				// �I�k�ɶ�
	float							m_currentTime;				// �ثe�I�k�ɶ�

	// buff information
	vector<RoleBuffInfo>			m_buffInfo;
	//vector<RoleBuffInfo*>			m_buffHelpful;
	//vector<RoleBuffInfo*>			m_buffHarmful;	

	CRoleSprite*					m_targetSprite;				// �ثe��w��H
	CRoleSprite*					m_mountSprite;				// ���M����
	CRoleSprite*					m_masterSprite;				// �D�H����
	CRoleSprite*					m_petSprite;				// �d������
	set<CRoleSprite*>				m_childSprite;				// ���s�ۤv���l����

	// player specific member
	char*							m_nameStr;					// ���a�W��
	char*							m_titleStr;					// ���a�ٸ�
	char*							m_guildStr;					// ���a���|
	string							m_originalName;				// ���a��l�W��
	int								m_DBID;
	int								m_worldGUID;
	int								m_worldID;					// ���A���N�X
	int								m_nameColor;				// �W���C��
	int								m_hp;
	int								m_lastHp;
	int								m_maxHp;
	int								m_mp;
	int								m_maxMp;
	int								m_sp;
	int								m_maxSp;
	int								m_guildID;					// ���|�s��
	int								m_questId;					// 
	int								m_touchRange;				// 
	int								m_areaRange;				// 	
	int								m_checkedStatus;			// 
	QuestDisplay					m_questDisplay;
	Voc_ENUM						m_vocation;					// �D¾
	int								m_level;					// �D¾����
	Voc_ENUM						m_vocationSub;				// ��¾
	int								m_levelSub;					// ��¾����
	Sex_ENUM						m_sex;						// �ʧO
	Race_ENUM						m_race;						// �ر�
	int								m_torso;					// �@������
	int								m_minimapIcon;				// NPC �b�p�a����ܪ��ϥ�
	int								m_QuestminimapIcon;			// NPC �b�p�a����ܪ��ϥ�
	int								m_cursorMode;				// Server
	PlayerStateStruct				m_playerState;				// ���a���A
	int								m_masterId;					// �d������
	CampID_ENUM						m_campId;					// �}��s��
	GoodEvilTypeENUM				m_goodEvilType;				// ���c������
	float							m_goodEvilValue;			// ���c�ȼƭ�
	float							m_tempScale;				// �Ȯɪ����Y���

	SpriteSpecialEffect				m_specialEffect;			// �S��ĪG���A

	// follow sprite
	CRoleSprite*					m_followSprite;				// ���H����
	float							m_followOffset;				// ���H�����q

	union {
		int data[4];
		struct {
			bool	disableRotate		: 1;		// ������V
			bool	touch				: 1;		// �IĲĲ�o
			bool	touchIn				: 1;		// �w�IĲĲ�o
			bool	cursor				: 1;		// ��ЬO�_������
			bool	target				: 1;		// �O�_�i�H�]�w���ؼ�
			bool	nameVisible			: 1;		// �W�٬O�_���
			bool	nameVisibleForce	: 1;		// �W�ٱj���O�_���
			bool	titleVisible		: 1;		// �ٸ��O�_���
			bool	guildVisible		: 1;		// ���|�O�_���

			bool	touchPlot			: 1;		// �O�_�֦�Ĳ�I�@��
			bool	rangePlot			: 1;		// �O�_�֦��d��@��
			bool	rangePlotTrigger	: 1;		// ���a�O�_�w�gĲ�o�d��@��

			bool	attack				: 1;		// Server �]�w�O�_�i�H�������A,�u���v�̤j
			bool	pvpZone				: 1;		// PVP �ϰ쪬�A
			bool	pvpOpen				: 1;		// PVP �}�Ҫ��A
			bool	pvpGuild			: 1;		// ���|����
			bool	duel				: 1;		// �M����H
			bool	campAttack			: 1;		// �O�_���i�H�����}��
			bool	campEnemy			: 1;		// �O�_������}��
			bool	canAttack			: 1;		// �O�_�i�H����
			bool	enemy				: 1;		// �|�����ۤv			

			bool	DisableQuest		: 1;		

			bool	HideMinimap			: 1;		// NPC �� ����b�p�a�ϤW���O�_��� ( �����~���ҥ~ )
			bool	CollisionPlot		: 1;		// �O�_�֦��I���@��(Server��)
			bool	inContainer			: 1;		// �޲z����

			bool	sneakAlpha			: 1;		// ��Υb�z��
			bool	sneakHidden			: 1;		// ��Τ��i��
			bool	sneakMime			: 1;		// �O���O�_�]�w�LMIME

			bool	ride				: 1;		// �M��
			bool	rideAttack			: 1;		// �M���i�H����
			bool	ridePlayAttackMotion	: 1;	// �M�������M�������ʧ@
			bool	party				: 1;		// �O�_�b�p����,�D�n�ΥH�P�_����W���C��
			bool	raid				: 1;		// �O�_�b�ζ���
			bool	staticObject		: 1;		// �R�A����
			bool	corpse				: 1;		// �O�_�����骫��
			bool	cursorEnable		: 1;		// ��ЬO�_������, �H cursor ���P�_, cursorEnable �u�O���\ Server �Ȯ���������o�I��, �H Action.disableClick 
			bool	titleIconVisible	: 1;		// �O�_����Y���� ICON
			bool	gravityState		: 1;		// �O�_�����O
			bool	gravityBackup		: 1;		// �O�d���e���A
			bool	alwaysHoldWeapon	: 1;		// �Z���`�O���b��W,���ް�����ʧ@
			bool	forceCombatMotion	: 1;		// �j��b�԰����A

			bool	hideMasterName		: 1;		// �����d���W��
			bool	lockedDirection		: 1;		// �Ȯ���wNPC���V,���۰���V�ؼ�
			bool	substituteTakeWeapon	: 1;	// �ܨ��ɮ����Z��
			bool	enemyHide			: 1;		// �Ĺ������			

			bool	playerPet			: 1;		// �O�_�����a�d��(�u�঳�@��)
			bool	hiddenBloodBar		: 1;		// ���æ��

			bool	inBattleGround		: 1;		// �Գ���
			bool	isFloat				: 1;		// �}�B�ĪG
			bool	isPriorityAttack	: 1;		// �Գ����S�����u���P�_�O�_�i����
			bool	isDisableAction		: 1;		// �Ȯɶ}���ʧ@
			bool	isScriptHide		: 1;		// �Ѽ@�������ê�����
		};
	}m_controlState;

	// �O����
	map<string,float>				m_boneScaleInfo;

	vector<SpriteController>		m_controller;

	map<string, INT64>				m_paperdollColor;

	bool							m_enableDyeFXColor;			// 
	int								m_dyeFXColors[4];

	// Corpse Effect
	CEffectSprite*					m_pCorpseEffect;			// ����S��

	float							m_risingimpulse;			// �V�W������
	float							m_gravityAcceleration;		// ���O�t��

	int								m_raidMarkNumber;			// �ζ��аO

	int								m_originalMountId;			// ��l���M����s��
	int								m_originalObjectId;			// ��l����s��
	LookStruct						m_originalLock;				// ��l����~��	
	char*							m_equpmentInfo;				// �˳��㹳���

	int								m_attachParentId;			// ������ID
	int								m_implementMaxNum;			// ����̤j�����ƶq
	int								m_implementCount;			// �ثe����ƶq,�O�_������(�i�Ѫ��a��)
	CRoleSprite*					m_implementSprite;			// �s�����㪫��
	int								m_implementSitPos;			// ����f���s��
	bool							m_implementControlMove;		// �O�_�i�H������㲾��
	bool							m_isImplement;				// �O�_������
	bool							m_isImplementAlignment;		// �����ǥؼ�
	CRuVector3						m_lastNowPos;				// �W�������I

	CRuVector3						m_parentMountOffset;		// �������󰾲��q
	int								m_parentMountId;			// ��������s��	

	float							m_immediateScale;			// ���������Y��
	
	int								m_attachParentPoint;		// �ѦҤ�����Ѧ��I
	CRoleSprite*					m_attachParentSprite;		// ������y�Э�
	bool							m_attachParentRotate;		// �۹����ۤv

	char							m_mountPointStr[64];		// ���M�Ѧ��I
	bool							m_isHorizontal;				// �������� UpVactor (0, 1, 0)

	vector<CRuVector3>				m_checkPosition;			// �ˬd�y���I
	bool							m_reportErrorPosition;		//
	int								m_floatingCount;			// �~���B�Ŧ���

	float							m_floatTime;				//

	bool							m_boTargetMove;				// ���a�T�w�ɶ����ؼв���
};