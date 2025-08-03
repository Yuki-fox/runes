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
	eRoleTakeItem_Unarmed,			// 拳套
	eRoleTakeItem_Blade,            // 劍
	eRoleTakeItem_Dagger,           // 匕首
	eRoleTakeItem_Wand,             // 權杖
	eRoleTakeItem_Axe,              // 斧
	eRoleTakeItem_Bludgeon,         // 鎚棍棒
	eRoleTakeItem_Claymore,         // 雙手劍
	eRoleTakeItem_Staff,            // 杖
	eRoleTakeItem_2H_Axe,           // 雙手斧
	eRoleTakeItem_2H_Hammer,        // 雙手鎚
	eRoleTakeItem_Polearm,			// 槍(長矛)
	eRoleTakeItem_Bow,				// 遠程武器 弓
	eRoleTakeItem_CossBow,			// 遠程武器 十字弓
	eRoleTakeItem_Gun,				// 遠程武器 槍
	eRoleTakeItem_Ammo_Bow,			// 子彈 弓
	eRoleTakeItem_Ammo_CossBow,		// 子彈 十字弓
	eRoleTakeItem_Ammo_Gun,			// 子彈 槍
	eRoleTakeItem_Throw,			// 投擲類武器
	eRoleTakeItem_Lumbering,		// 筏木
	eRoleTakeItem_Herbalism,		// 採藥
	eRoleTakeItem_Mining,			// 採礦
	eRoleTakeItem_Fishing,			// 釣魚

	eRoleTakeItem_Shield,			// 盾
	eRoleTakeItem_Implement,		// 手持器具(法器類)
};


enum RoleCheckedStatus {
	eChecked_Null					= 0,
	eChecked_SayPlot,
	eChecked_TouchPlot,
	eChecked_RangePlot,
	eChecked_QuestNPC,						// 給予 Quest 的 NPC
	eChecked_QuestObj,						// Quest Obj
	eChecked_Mine,							// 採集物件
	eChecked_Herb,							// 採集物件
	eChecked_Lumber,						// 採集物件
	eChecked_Flag,
	eChecked_Shop,							// 商人 ( 包含 商店 / 銀行 / 拍賣所 )
	eChecked_Mail,							// 郵筒
	eChecked_FurnitureNormal,				// 一般傢俱
	eChecked_FurnitureStorage,              // 傢俱 寶箱 衣架
 	eChecked_FurnitureSound,                // 傢俱 留聲機
	eChecked_Maid,							// 女僕
	eChecked_Servant,						// 女僕
	eChecked_GuildHousesBuildPoint,			// 公會屋參考點
	eChecked_Tomb,							// Tomb
	eChecked_Player,						// Player
};

enum QuestDisplay {
	eQuest_Display_Null				= 0,
	eQuest_Display_Get_Gary,
	eQuest_Display_UnfinishedQuest,				// 未完成任務
	eQuest_Display_AvailableRepeatQuest,		// 重覆(每日)可接任務
	eQuest_Display_Public_AvailableQuest,
	eQuest_Display_AvailableQuest,				// 正常可接任務
	
	eQuest_Display_FinishedRepeatQuest,			// 重覆(每日)完成任務
	eQuest_Display_Public_FinishedQuest,
	eQuest_Display_FinishedQuest,				// 正常完成任務

	eVendor_Display_WeaponShop,					// 武器商人
	eVendor_Display_ArmorShop,					// 防具商人
	eVendor_Display_AuctionHouse,				// 拍賣所
	eVendor_Display_FoodShop,					// 食物
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
// 路徑移動節點結構
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

	string				sourceName;					// 施法者名稱
	int					source;						// 來源
	int					id;							// 索引資料
	CEffectSprite*		effect;						// 產生特效
	CRuEntity*			armEffect;					// 手臂部特效
	float				destructTime;				// 刪除時間
	float				time;						// BUFF 時間
	float				recycleTime;				// DOT類特效觸發時間
	float				remnantTime;				// DOT類特效剩於時間
	int					dotCount;					// DOT法術做用次數
	bool				faceOff;					// 變身
	bool				mount;						// 坐騎
	int					magicLv;					// Buff累積等級
	float				dotRate;					// 壞的DOT法術,衰減值

	bool				locked;						// 一般為送出取消BUFF
	bool				proxyLoad;					// 延遲載入
	int					ownerID;				
};

//-----------------------------------------------------------------------------
struct DOTBuffValueInfo
{
	const char*			sourceName;					// 施法者名稱
	int					source;						// 來源物件編號
	int					id;							// 法術編號
	int					type;						// 類型(0 HP, 1MP, 2WarriorSP, 3RangerSP, 4RogueSP)
	int					value;						// 數值
	bool				absorb;						// 傷害被吸收
	
};

//-----------------------------------------------------------------------------
struct RoleHandsEntity
{
	RoleHandsType		type;						// 類型
	CRuEntity*			entity;						// 3D物件
	CRuEntity*			enchantEntity;				// 武器附魔特效
	SpriteAttachPoint	point;						// 3D參考點
	int					id;							// ItemDB編號
};

//-----------------------------------------------------------------------------
typedef union
{
	int		temp[16];
	struct {
		char	invisible;					// 隱形
		char	detectInvisible;			// 偵測隱形
		char	sneak;						// 潛行
		char	detectSneak;				// 偵測潛行
		char	lockedMove;					// 限制移動
		char	blind;						// 失明
		char	hitDown;					// 倒地
		char	chaos;						// 混亂
		char	fear;						// 恐懼
		char	sleep;						// 睡眠
		char	dizzy;						// 暈眩
		char	pvp;						// 玩家PVP
		char	protect;					// 保護PVP
		char	inverse;					// 左右相反
		char	freeze;						// 結凍
		char	criminal;					// 罪犯
		char	disableJobSkill;			// 無法使用職業技能
		char	waterWalk;					// 水上行走
		char    cantCTRL;					//無法控制
	};
}SpriteSpecialEffect;

enum SpriteControllerType
{
	eSpriteController_None			= 0,	// 一般
	eSpriteController_Alpha,				// Alpha 控制
	eSpriteController_Scale,				// Scale 控制
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

	// 重力
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
 	virtual	CRuFusion_ActorState	GetDisplayActorState(); //取得顯示出來的ActorState
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

	// 是否可以攻擊角色
	void							SetObjectAttackState(bool isState);
	bool							GetObjectAttackState()					{ return m_controlState.attack; }

	void							SetCanAttackState(bool isState);
	bool							GetCanAttackState();

	// 是否對自己有仇恨
	void							SetEnemyState(bool isState);
	bool							GetEnemyState();

	// 決鬥目標
	void							SetDuelState(bool state);
	bool							GetDuelState()							{ return m_controlState.duel; }

	// PVP 區域
	void							SetPVPZoneState(bool state);
	bool							GetPVPZoneState()						{ return m_controlState.pvpZone; }	

	// 公會野戰
	void							SetPVPGuildState(bool state);
	bool							GetPVPGuildState()						{ return m_controlState.pvpGuild; }

	// 陣營相對關係
	void							SetCampAttackState(bool state);
	bool							GetCampAttackState()					{ return m_controlState.campAttack; }
	void							SetCampEnemyState(bool state);
	bool							GetCampEnemyState()						{ return m_controlState.campEnemy; }

	// PVP 狀態(玩家開啟)
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

	// 禁止轉向
	void							SetDisableRotate(bool disableRotate)	{ m_controlState.disableRotate = disableRotate; }
	bool							GetDisableRotate()						{ return m_controlState.disableRotate; }

	// 將武器一直拿在手上
	void							SetAlwaysHoldWeapon(bool state);
	bool							GetAlwaysHoldWeapon()					{ return m_controlState.alwaysHoldWeapon; }
	void							SetForceCombatMotion(bool state);
	bool							GetForceCombatMotion()					{ return m_controlState.forceCombatMotion; }

	void							SetHideMasterName(bool state)			{ m_controlState.hideMasterName = state; }
	bool							GetHideMasterName()						{ return m_controlState.hideMasterName; }

	// 血條
	void							SetBloodBarHidden(bool hide)			{ m_controlState.hiddenBloodBar = hide; }
	bool							IsBloodBarHidden()						{ return m_controlState.hiddenBloodBar; }

	// 動作
	void							SetDisableAction(bool disableAction)	{ m_controlState.isDisableAction = disableAction; }
	bool							GetDisableAction()						{ return m_controlState.isDisableAction; }

	// 移動
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

	// 裝備第2類(一般為採集工具)物品
	void							AttachCraftingTool(const char* point=NULL);
	void							DetachCraftingTool();

	// 增加控制器
	void							AddController(SpriteControllerType type, float time, float begin, float end);
	void							UpdateController(float elapseTime);
	void							ClearController(SpriteControllerType type);

	// 坐騎
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

	// 屍體特效物件
	void							CreateCorpseEffect();
	void							ReleaseCorpseEffect();

	// 善惡值
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

	// 水上行走
	virtual void					SetAboveWaterState(bool enable);
	virtual bool					IsAboveWater();

	// 被炸上空中
	virtual void					JumpEx();
	virtual bool					IsFloat()											{ return m_controlState.isFloat; }

	virtual int						GetAttachParentId()									{ return m_attachParentId; }
	virtual int						GetMountCarryCount();

	virtual void					SetPriorityAttack(bool state)						{ m_controlState.isPriorityAttack = state; }
	virtual bool					GetPriorityAttack()									{ return m_controlState.isPriorityAttack; }

	// 載具
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
	CRuController_GlobalAlpha*		m_pRuGlobalAlpha;			// 透明值控制器
	CRuEntity_Widget_Label*			m_pRuWidget;				// 3D文字物件
	CRuFusion_Mime*					m_pRuMimeEntity;
	CRuFusion_Actor*				m_pRuActor;					// 動作控制
	CRuFusion_Mime*					m_pRuTempMimeEntity;
	CRuFusion_Actor*				m_pRuTempActor;				// 動作控制
	CRuEntity_Widget_Line*			m_pRuWidgetLine[3];	
	CRuEntity_CollectorFX*			m_pRuCollectorFX;			// 效果光球
	CRuEntity*						m_pRuCollectorEntity;		// 效果光球特效物件	
	CRuEntity*						m_pRuFlagEntity;			// 旗子效果
	CRuEntity*						m_pRuOrnamentEntity;		// 裝飾品
	CRuEntity*						m_pRuBackEntity;			// 部背裝飾品
	RoleHandsEntity					m_handsEntity[eRoleHands_Last];			// 0主 1副 2其他類型 3弓箭
	int								m_handsObjectId[eRoleHands_Last];		// 手持物品資料		
	int								m_handsLevel[eRoleHands_Last];			// 手持物品資料
	int								m_ornamentID;				// 裝飾品編號
	int								m_backID;					// 背部飾品編號
	string							m_ornamentMotion;			// 裝飾品撥放動作

	int								m_refCount;					// 參考個數	
	float							m_bodyHeading;
	float							m_torsoHeading;
	float							m_anchorHeight;				// 高度定位
	float							m_moveTime;					// 移動時間
	float							m_attackRange;				// 攻擊半徑
	bool							m_fadeOut;					// 淡出
	bool							m_isNeedBuild;				//
	bool							m_isWaitingForPaperdollCache;
	bool							m_isNeedResetWidget;
	CRuVector3						m_moveVector;				// 移動向量
	CRuVector3						m_lastTargetPos;			// 記錄上次目標座標點
	CRuVector3						m_targetPos;				// 記錄目標座標點
	CRuVector3						m_nextTargetPos;			// 記錄下一次目標座點
	CRuVector3						m_correctDir;				// Server最後一次送的面向
	SpriteMoveType					m_moveType;					// 移動狀態
	RoleSpriteStatus				m_state;					// 狀態旗標

	// 動作 Queue
	map<int, float>					m_actorQueue;				// 動作暫存
	vector<SpriteActorQueue>		m_actorStateQueue;			// 在物件未產生前暫存動作

	//
	vector<CEffectSprite*>			m_castingEffect;			// 集氣特效	
	CEffectSprite*					m_polyLineEffect;			// 索引式特效
	int								m_spellID;
	float							m_castingTime;				// 施法時間
	float							m_currentTime;				// 目前施法時間

	// buff information
	vector<RoleBuffInfo>			m_buffInfo;
	//vector<RoleBuffInfo*>			m_buffHelpful;
	//vector<RoleBuffInfo*>			m_buffHarmful;	

	CRoleSprite*					m_targetSprite;				// 目前鎖定對象
	CRoleSprite*					m_mountSprite;				// 坐騎物件
	CRoleSprite*					m_masterSprite;				// 主人物件
	CRoleSprite*					m_petSprite;				// 寵物物件
	set<CRoleSprite*>				m_childSprite;				// 關連自己的子物件

	// player specific member
	char*							m_nameStr;					// 玩家名稱
	char*							m_titleStr;					// 玩家稱號
	char*							m_guildStr;					// 玩家公會
	string							m_originalName;				// 玩家原始名稱
	int								m_DBID;
	int								m_worldGUID;
	int								m_worldID;					// 伺服器代碼
	int								m_nameColor;				// 名稱顏色
	int								m_hp;
	int								m_lastHp;
	int								m_maxHp;
	int								m_mp;
	int								m_maxMp;
	int								m_sp;
	int								m_maxSp;
	int								m_guildID;					// 公會編號
	int								m_questId;					// 
	int								m_touchRange;				// 
	int								m_areaRange;				// 	
	int								m_checkedStatus;			// 
	QuestDisplay					m_questDisplay;
	Voc_ENUM						m_vocation;					// 主職
	int								m_level;					// 主職等級
	Voc_ENUM						m_vocationSub;				// 副職
	int								m_levelSub;					// 副職等級
	Sex_ENUM						m_sex;						// 性別
	Race_ENUM						m_race;						// 種族
	int								m_torso;					// 護甲類型
	int								m_minimapIcon;				// NPC 在小地圖顯示的圖示
	int								m_QuestminimapIcon;			// NPC 在小地圖顯示的圖示
	int								m_cursorMode;				// Server
	PlayerStateStruct				m_playerState;				// 玩家狀態
	int								m_masterId;					// 寵物類型
	CampID_ENUM						m_campId;					// 陣營編號
	GoodEvilTypeENUM				m_goodEvilType;				// 善惡值類型
	float							m_goodEvilValue;			// 善惡值數值
	float							m_tempScale;				// 暫時物件縮放值

	SpriteSpecialEffect				m_specialEffect;			// 特殊效果狀態

	// follow sprite
	CRoleSprite*					m_followSprite;				// 跟隨物件
	float							m_followOffset;				// 跟隨偏移量

	union {
		int data[4];
		struct {
			bool	disableRotate		: 1;		// 停止轉向
			bool	touch				: 1;		// 碰觸觸發
			bool	touchIn				: 1;		// 已碰觸觸發
			bool	cursor				: 1;		// 游標是否有做用
			bool	target				: 1;		// 是否可以設定為目標
			bool	nameVisible			: 1;		// 名稱是否顯示
			bool	nameVisibleForce	: 1;		// 名稱強制控制是否顯示
			bool	titleVisible		: 1;		// 稱號是否顯示
			bool	guildVisible		: 1;		// 公會是否顯示

			bool	touchPlot			: 1;		// 是否擁有觸碰劇情
			bool	rangePlot			: 1;		// 是否擁有範圍劇情
			bool	rangePlotTrigger	: 1;		// 玩家是否已經觸發範圍劇情

			bool	attack				: 1;		// Server 設定是否可以攻擊狀態,優先權最大
			bool	pvpZone				: 1;		// PVP 區域狀態
			bool	pvpOpen				: 1;		// PVP 開啟狀態
			bool	pvpGuild			: 1;		// 公會野戰
			bool	duel				: 1;		// 決鬥對象
			bool	campAttack			: 1;		// 是否為可以攻擊陣營
			bool	campEnemy			: 1;		// 是否為仇恨陣營
			bool	canAttack			: 1;		// 是否可以攻擊
			bool	enemy				: 1;		// 會攻擊自己			

			bool	DisableQuest		: 1;		

			bool	HideMinimap			: 1;		// NPC 或 物件在小地圖上面是否顯示 ( 類型外的例外 )
			bool	CollisionPlot		: 1;		// 是否擁有碰撞劇情(Server端)
			bool	inContainer			: 1;		// 管理物件

			bool	sneakAlpha			: 1;		// 潛形半透明
			bool	sneakHidden			: 1;		// 潛形不可見
			bool	sneakMime			: 1;		// 記錄是否設定過MIME

			bool	ride				: 1;		// 騎乘
			bool	rideAttack			: 1;		// 騎乘可以攻擊
			bool	ridePlayAttackMotion	: 1;	// 騎乘撥放騎乘攻擊動作
			bool	party				: 1;		// 是否在小隊內,主要用以判斷角色名稱顏色
			bool	raid				: 1;		// 是否在團隊內
			bool	staticObject		: 1;		// 靜態物件
			bool	corpse				: 1;		// 是否為屍體物件
			bool	cursorEnable		: 1;		// 游標是否有做用, 以 cursor 為判斷, cursorEnable 只是允許 Server 暫時關掉物件得點擊, 以 Action.disableClick 
			bool	titleIconVisible	: 1;		// 是否顯示頭像框 ICON
			bool	gravityState		: 1;		// 是否有重力
			bool	gravityBackup		: 1;		// 保留之前狀態
			bool	alwaysHoldWeapon	: 1;		// 武器總是握在手上,不管做什麼動作
			bool	forceCombatMotion	: 1;		// 強制在戰鬥狀態

			bool	hideMasterName		: 1;		// 隱藏寵物名稱
			bool	lockedDirection		: 1;		// 暫時鎖定NPC面向,不自動轉向目標
			bool	substituteTakeWeapon	: 1;	// 變身時拿取武器
			bool	enemyHide			: 1;		// 敵對時隱藏			

			bool	playerPet			: 1;		// 是否為玩家寵物(只能有一隻)
			bool	hiddenBloodBar		: 1;		// 隱藏血條

			bool	inBattleGround		: 1;		// 戰場中
			bool	isFloat				: 1;		// 漂浮效果
			bool	isPriorityAttack	: 1;		// 戰場中特殊物件優先判斷是否可攻擊
			bool	isDisableAction		: 1;		// 暫時開閉動作
			bool	isScriptHide		: 1;		// 由劇情所隱藏的物件
		};
	}m_controlState;

	// 記錄用
	map<string,float>				m_boneScaleInfo;

	vector<SpriteController>		m_controller;

	map<string, INT64>				m_paperdollColor;

	bool							m_enableDyeFXColor;			// 
	int								m_dyeFXColors[4];

	// Corpse Effect
	CEffectSprite*					m_pCorpseEffect;			// 屍體特效

	float							m_risingimpulse;			// 向上跳高度
	float							m_gravityAcceleration;		// 重力速度

	int								m_raidMarkNumber;			// 團隊標記

	int								m_originalMountId;			// 原始坐騎物件編號
	int								m_originalObjectId;			// 原始物件編號
	LookStruct						m_originalLock;				// 原始物件外型	
	char*							m_equpmentInfo;				// 裝備顯像資料

	int								m_attachParentId;			// 父元件ID
	int								m_implementMaxNum;			// 載具最大乘載數量
	int								m_implementCount;			// 目前載具數量,是否為載具(可供玩家塔乘)
	CRoleSprite*					m_implementSprite;			// 連結載具物件
	int								m_implementSitPos;			// 載具搭乘編號
	bool							m_implementControlMove;		// 是否可以控制載具移動
	bool							m_isImplement;				// 是否為載具
	bool							m_isImplementAlignment;		// 載具對準目標
	CRuVector3						m_lastNowPos;				// 上次坐標點

	CRuVector3						m_parentMountOffset;		// 乘坐物件偏移量
	int								m_parentMountId;			// 乘坐物件編號	

	float							m_immediateScale;			// 直接改變縮放
	
	int								m_attachParentPoint;		// 參考父元件參考點
	CRoleSprite*					m_attachParentSprite;		// 父元件座標值
	bool							m_attachParentRotate;		// 相對旋轉自己

	char							m_mountPointStr[64];		// 坐騎參考點
	bool							m_isHorizontal;				// 水平移動 UpVactor (0, 1, 0)

	vector<CRuVector3>				m_checkPosition;			// 檢查座標點
	bool							m_reportErrorPosition;		//
	int								m_floatingCount;			// 外掛浮空次數

	float							m_floatTime;				//

	bool							m_boTargetMove;				// 玩家固定時間往目標移動
};