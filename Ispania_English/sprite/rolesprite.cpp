#include <windows.h>
#include "sprite.h"
#include "../mainproc/NetGlobal.h"
#include "../ErrorList/errorlist.h"
//#include "../Tools/String/String.h"

#define JUMP_RISINGIMPULSE					60.0f
#define WEWPOA_ARROW_ID						210521

#define DEFAULT_PAPERDOLL_NAME_HEAD			"type01"
#define DEFAULT_PAPERDOLL_NAME_HAIR			"type01"

#ifndef SAFE_DELETE
	#define SAFE_DELETE(x)	if (x) delete x; x = NULL;
#endif

void SetEntityPaperdoll(CRuEntity* entity, const char* partName, const char* name)
{
	if ( entity ) {
		if (name == NULL || name[0] == 0)
		{
			if (strcmp(partName, PAPERDOLL_PART_NAME_HEAD) == 0)
				name = "type01";
			else if (strcmp(partName, PAPERDOLL_PART_NAME_HAIR) == 0)
				name = "type01";
			else if (strcmp(partName, PAPERDOLL_PART_NAME_TORSO) == 0)
				name = "body000-001";
			else if (strcmp(partName, PAPERDOLL_PART_NAME_HAND) == 0)
				name = "body000-001";
			else if (strcmp(partName, PAPERDOLL_PART_NAME_LEG) == 0)
				name = "body000-001";
			else if (strcmp(partName, PAPERDOLL_PART_NAME_FOOT) == 0)
				name = "body000-001";
			else
				name = "";
		}

		RuPaperdoll_SetComponent(entity, partName, name);
	}
}

//-----------------------------------------------------------------------------
CRuFusion_ActorState CRoleSprite::MotionToActorState(const char* motion)
{
	if ( strcmp(motion, "casting_delay") == 0 )
		return ruFUSION_ACTORSTATE_CAST_BEGIN;
	else if ( strcmp(motion, "casting01") == 0 || strcmp(motion, "buff01") == 0 )
		return ruFUSION_ACTORSTATE_BUFF_INSTANT;
	else if ( strcmp(motion, "casting02") == 0 || strcmp(motion, "cast01") == 0 )
		return ruFUSION_ACTORSTATE_CAST_INSTANT;
	else if ( strcmp(motion, "debuff01") == 0 )
		return ruFUSION_ACTORSTATE_DEBUFF_INSTANT;
	else if ( strcmp(motion, "cast_sp01") == 0 )
		return ruFUSION_ACTORSTATE_CAST_SP01;
	else if ( strcmp(motion, "cast_sp02") == 0 )
		return ruFUSION_ACTORSTATE_CAST_SP02;
	else if ( strcmp(motion, "cast_sp03") == 0 )
		return ruFUSION_ACTORSTATE_CAST_SP03;
	else if ( strcmp(motion, "cast_sp04") == 0 )
		return ruFUSION_ACTORSTATE_CAST_SP04;
	else if ( strcmp(motion, "buff_sp01") == 0 )
		return ruFUSION_ACTORSTATE_BUFF_SP01;
	else if ( strcmp(motion, "buff_sp02") == 0 )
		return ruFUSION_ACTORSTATE_BUFF_SP02;
	else if ( strcmp(motion, "buff_sp03") == 0 )
		return ruFUSION_ACTORSTATE_BUFF_SP03;	
	else if ( strcmp(motion, "continuous_cast") == 0 )
		return ruFUSION_ACTORSTATE_CHANNEL_BEGIN;
	else if ( strcmp(motion, "continuous_buff") == 0 )
		return ruFUSION_ACTORSTATE_BUFF_BEGIN;
	else if ( strcmp(motion, "spawn") == 0 )
		return ruFUSION_ACTORSTATE_SPAWN_BEGIN;
	else if ( strcmp(motion, "mining_idle") == 0 )	
		return ruFUSION_ACTORSTATE_MINING_BEGIN;
	else if ( strcmp(motion, "unarmed_attack01") == 0 || strcmp(motion, "unarmed_attack02") == 0 || strcmp(motion, "unarmed_attack03") == 0)
		return ruFUSION_ACTORSTATE_ATTACK_UNARMED;
	else if ( strcmp(motion, "1h_attack01") == 0 || strcmp(motion, "1h_attack02") == 0 || strcmp(motion, "1h_attack03") == 0)
		return ruFUSION_ACTORSTATE_ATTACK_1H;
	else if ( strcmp(motion, "2h_attack01") == 0 || strcmp(motion, "2h_attack02") == 0 || strcmp(motion, "2h_attack03") == 0)
		return ruFUSION_ACTORSTATE_ATTACK_2H;
	else if ( strcmp(motion, "polearm_attack01") == 0 || strcmp(motion, "polearm_attack02") == 0 || strcmp(motion, "polearm_attack03") == 0)
		return ruFUSION_ACTORSTATE_ATTACK_POLEARM;
	else if ( strcmp(motion, "bow_ready") == 0 )
		return ruFUSION_ACTORSTATE_ATTACK_BOW_BEGIN;
	else if ( strcmp(motion, "bow_channel") == 0 )
		return ruFUSION_ACTORSTATE_ATTACK_BOW_LOOP;
	else if ( strcmp(motion, "bow_release") == 0 )
		return ruFUSION_ACTORSTATE_ATTACK_BOW_END;
	else if ( strcmp(motion, "boe_ready_sp") == 0 )
		return ruFUSION_ACTORSTATE_ATTACK_BOW_SP_BEGIN;
	else if ( strcmp(motion, "boe_channel_sp") == 0 )
		return ruFUSION_ACTORSTATE_ATTACK_BOW_SP_LOOP;
	else if ( strcmp(motion, "boe_release_sp") == 0 )
		return ruFUSION_ACTORSTATE_ATTACK_BOW_SP_END;
	else if ( strcmp(motion, "gun_ready") == 0 )
		return ruFUSION_ACTORSTATE_ATTACK_GUN_BEGIN;
	else if ( strcmp(motion, "gun_channel") == 0 )
		return ruFUSION_ACTORSTATE_ATTACK_GUN_LOOP;
	else if ( strcmp(motion, "gun_release") == 0 )
		return ruFUSION_ACTORSTATE_ATTACK_GUN_END;
	else if ( strcmp(motion, "shield_bash") == 0 )
		return ruFUSION_ACTORSTATE_SHIELD_BASH;
	else if ( strcmp(motion, "shield_block") == 0 )
		return ruFUSION_ACTORSTATE_SHIELD_BLOCK;
	else if ( strcmp(motion, "earthstrike") == 0 )
		return ruFUSION_ACTORSTATE_SP_SPINKICK;
	else if ( strcmp(motion, "spinkick") == 0 )
		return ruFUSION_ACTORSTATE_SP_SPINKICK;
	else if ( strcmp(motion, "spinslash") == 0 )
		return ruFUSION_ACTORSTATE_SP_SPINSLASH;
	else if ( strcmp(motion, "1h_sp01" ) == 0 )
		return ruFUSION_ACTORSTATE_ATTACK_1H_SP;
	else if ( strcmp(motion, "1h_sp02" ) == 0 )
		return ruFUSION_ACTORSTATE_ATTACK_1H_SP02;
	else if ( strcmp(motion, "1h_slow_attack01" ) == 0 )
		return ruFUSION_ACTORSTATE_ATTACK_1H_SLOW01;
	else if ( strcmp(motion, "2h_sp01" ) == 0 )
		return ruFUSION_ACTORSTATE_ATTACK_2H_SP;
	else if ( strcmp(motion, "unarmed_sp01" ) == 0 )
		return ruFUSION_ACTORSTATE_ATTACK_UNARMED_SP;
	else if ( strcmp(motion, "polearm_sp01" ) == 0 )
		return ruFUSION_ACTORSTATE_ATTACK_POLEARM_SP;
	else if ( strcmp(motion, "use_item") == 0 )
		return ruFUSION_ACTORSTATE_USE;
	else if ( strcmp(motion, "throw") == 0 )
		return ruFUSION_ACTORSTATE_THROW;
	else if ( strcmp(motion, "drink") == 0 )
		return ruFUSION_ACTORSTATE_EMOTE_DRINK;
	else if ( strcmp(motion, "eat") == 0 )
		return ruFUSION_ACTORSTATE_EMOTE_EAT;
	else if ( strcmp(motion, "crafting") == 0 )
		return ruFUSION_ACTORSTATE_CRAFTING_BEGIN;
	else if ( strcmp(motion, "crouch_up") == 0 )
		return ruFUSION_ACTORSTATE_CROUCH_END;
	else if ( strcmp(motion, "crouch_down") == 0 )
		return ruFUSION_ACTORSTATE_CROUCH_BEGIN;
	else if ( strcmp(motion, "crouch_idle") == 0 )
		return ruFUSION_ACTORSTATE_CROUCH_LOOP;
	else if ( strcmp(motion, "loot_start") == 0 )
		return ruFUSION_ACTORSTATE_LOOT_BEGIN;
	else if ( strcmp(motion, "loot_loop") == 0 )
		return ruFUSION_ACTORSTATE_LOOT_LOOP;
	else if ( strcmp(motion, "loot_end") == 0 )
		return ruFUSION_ACTORSTATE_LOOT_END;
	else if ( strcmp(motion, "carry_begin") == 0 )
		return ruFUSION_ACTORSTATE_CARRY_BEGIN;
	else if ( strcmp(motion, "carry_loop") == 0 )
		return ruFUSION_ACTORSTATE_CARRY_LOOP;
	else if ( strcmp(motion, "carry_end") == 0 )		
		return ruFUSION_ACTORSTATE_CARRY_END;
	else if ( strcmp(motion, "kamehameha") == 0 )
		return ruFUSION_ACTORSTATE_EMOTE_KAMEHAMEHA;
	else if ( strcmp(motion, "kick") == 0 )
		return ruFUSION_ACTORSTATE_EMOTE_KICK;
	else if ( strcmp(motion, "emote_sleep_down") == 0 )
		return ruFUSION_ACTORSTATE_SLEEP_BEGIN;
	else if ( strcmp(motion, "emote_sleep_idle") == 0 )
		return ruFUSION_ACTORSTATE_SLEEP_LOOP;
	else if ( strcmp(motion, "emote_sleep_up") == 0 )
		return ruFUSION_ACTORSTATE_SLEEP_END;
	else if ( strcmp(motion, "emote_kiss") == 0 )
		return ruFUSION_ACTORSTATE_EMOTE_KISS;
	else if ( strcmp(motion, "polearm_idle") == 0 )
		return ruFUSION_ACTORSTATE_COMBAT_POLEARM;
	return ruFUSION_ACTORSTATE_NORMAL;
}

//-----------------------------------------------------------------------------
CRoleSprite::CRoleSprite(bool isAddToContainer)
	: IBaseSprite(isAddToContainer)
{
	//m_shadowType					= eSpriteShadowType_Projected;

	m_classType						= eSpriteClass_Role;
	
	m_pRuQuestEntity				= NULL;
	m_pRuGlobalAlpha				= NULL;
	m_pRuWidget						= NULL;	
	m_pRuMimeEntity					= NULL;
	m_pRuActor						= NULL;
	m_pRuTempMimeEntity				= NULL;
	m_pRuTempActor					= NULL;
	m_pRuWidgetLine[0]				= NULL;
	m_pRuWidgetLine[1]				= NULL;
	m_pRuWidgetLine[2]				= NULL;
	m_pRuCollectorFX				= NULL;
	m_pRuCollectorEntity			= NULL;
	m_pRuFlagEntity					= NULL;
	m_pRuOrnamentEntity				= NULL;
	m_pRuBackEntity					= NULL;

	m_bodyHeading					= 0.0f;
	m_torsoHeading					= 0.0f;	
	m_fadeOut						= true;
	m_isNeedBuild					= false;
	m_isWaitingForPaperdollCache	= true;
	m_isNeedResetWidget				= false;
	m_alpha							= 1.0f;
	m_ornamentID					= 0;
	m_reportErrorPosition			= false;
	m_ornamentMotion.clear();

	for ( int i = 0; i < eRoleHands_Last; i++ )
	{
		m_handsEntity[i].type				= eRoleHands_None;
		m_handsEntity[i].entity				= NULL;
		m_handsEntity[i].enchantEntity		= NULL;
		m_handsEntity[i].point				= eAttach_Null;
		m_handsEntity[i].id					= 0;

		m_handsObjectId[i]	= 0;
		m_handsLevel[i] = 0;
	}

	// move Info
	m_lastTargetPos.Set(0.0f, 0.0f, 0.0f);
	m_targetPos.Set(0.0f, 0.0f, 0.0f);
	m_moveVector.Set(0.0f, 0.0f, 0.0f);
	m_correctDir.Set(0.0f, 0.0f, 0.0f);
	m_moveTime					= 0.0f;		
	m_moveType					= eRoleSpriteMove_Normal;
	m_anchorHeight				= 0.0f;
	m_refCount					= 0;
	
	m_actorQueue.clear();
	m_castingEffect.clear();	
	m_spellID					= 0;
	m_castingTime				= 0.0f;
	m_state.value				= 0;

	m_targetSprite				= NULL;
	m_masterSprite				= NULL;
	m_mountSprite				= NULL;
	m_petSprite					= NULL;

	m_nameStr					= NULL;
	m_guildStr					= NULL;
	m_titleStr					= NULL;

	m_hp						= 0;
	m_mp						= 0;
	m_sp						= 0;
	m_maxHp						= 0;
	m_maxMp						= 0;
	m_maxSp						= 0;
	m_level						= 0;
	m_guildID					= 0;
	m_questId					= 0;
	m_touchRange				= 0;
	m_areaRange					= 0;
	m_attackRange				= 0;
	m_checkedStatus				= eChecked_Null;
	m_vocation					= EM_Vocation_None;
	m_vocationSub				= EM_Vocation_None;
	m_questDisplay				= eQuest_Display_Null;
	m_sex						= EM_Sex_BigMonster;
	m_race						= EM_Race_None;
	m_torso						= 0;
	m_playerState._State		= 0;
	m_campId					= EM_CampID_Unknow;
	m_goodEvilType				= EM_GoodEvil_Normal;
	m_goodEvilValue				= 0.0f;
	
	ZeroMemory( m_controlState.data, sizeof(m_controlState.data) );

	m_controlState.disableRotate	= false;
	m_controlState.touch			= false;
	m_controlState.touchIn			= false;
	m_controlState.cursor			= true;
	m_controlState.target			= true;	
	
	m_controlState.duel				= false;
	m_controlState.pvpZone			= false;
	m_controlState.pvpGuild			= false;
	m_controlState.pvpOpen			= false;
	m_controlState.campAttack		= false;
	m_controlState.campEnemy		= false;	
	m_controlState.canAttack		= false;
	m_controlState.enemy			= false;

	m_controlState.nameVisible		= true;
	m_controlState.titleVisible		= true;		// 稱號是否顯示
	m_controlState.guildVisible		= true;		// 公會是否顯示
	m_controlState.nameVisibleForce	= true;
	
	m_controlState.touchPlot		= false;
	m_controlState.rangePlot		= false;
	m_controlState.rangePlotTrigger	= false;

	m_controlState.sneakAlpha		= false;
	m_controlState.sneakHidden		= false;
	m_controlState.ride				= false;
	m_controlState.rideAttack				= false;
	m_controlState.ridePlayAttackMotion		= false;
	m_controlState.party			= false;
	m_controlState.staticObject		= false;
	m_controlState.inContainer		= false;
	m_controlState.corpse			= false;

	m_controlState.cursorEnable		= true;
	m_controlState.alwaysHoldWeapon	= false;
	m_controlState.forceCombatMotion = false;
	m_controlState.hideMasterName	= false;

	m_controlState.enemyHide		= false;
	m_controlState.hiddenBloodBar	= false;
	m_controlState.inBattleGround	= false;
	m_controlState.isPriorityAttack	= false;
	m_controlState.isDisableAction	= false;

	m_minimapIcon					= 0;
	m_QuestminimapIcon				= 0;
	m_masterId						= -1;
	
	m_pCorpseEffect					= NULL;
	m_enableDyeFXColor				= false;
	memset(m_dyeFXColors, 0, sizeof(m_dyeFXColors));

	m_buffInfo.clear();

	memset(&m_specialEffect, 0, sizeof(m_specialEffect));

	m_risingimpulse					= 1.0f;
	m_gravityAcceleration			= 1.0f;

	m_raidMarkNumber				= -1;

	m_tempScale						= 0.0f;	

	m_followSprite					= NULL;
	m_followOffset					= 0;

	m_cursorMode					= 0;	

	m_originalObjectId				= 0;
	m_originalMountId				= 0;
	m_attachParentId				= 0;	

	m_equpmentInfo					= NULL;

	m_implementMaxNum				= 0;			// 載具最大乘載數量
	m_implementCount				= 0;			// 目前載具數量
	m_implementSprite				= NULL;			// 連結載具物件
	m_implementSitPos				= 0;
	m_implementControlMove			= false;		// 是否可以控制載具移動
	m_isImplement					= false;		// 是否為載具
	m_isImplementAlignment			= false;

	m_immediateScale				= 1.0f;			// 直接改變大小

	
	m_attachParentSprite			= NULL;
	m_attachParentPoint				= 0;
	m_attachParentRotate			= false;

	m_isHorizontal					= true;

	m_floatingCount					= 0;

	m_boTargetMove					= false;
}

//-----------------------------------------------------------------------------
CRoleSprite::~CRoleSprite()
{
	m_followSprite = NULL;

	SetAttachParent(NULL, 0);

	ReleaseEquipmentInfo();

	// 
	SetMasterSprite(NULL);
	/*
	if ( m_masterSprite )
	{
		m_masterSprite->RemoveChildSprite(this);
	}
	m_masterSprite = NULL;
	*/

	ruSAFE_DELETE(m_nameStr);
	ruSAFE_DELETE(m_titleStr);
	ruSAFE_DELETE(m_guildStr);

	/*
	for ( set<CRoleSprite*>::iterator iter = m_childSprite.begin(); iter != m_childSprite.end(); iter++ )
	{
		(*iter)->SetMasterSprite(NULL);
	}	
	m_childSprite.clear();
	*/

	m_targetSprite = NULL;
	m_raidMarkNumber = -1;
	
	ReleaseCorpseEffect();

	m_actorQueue.clear();
	ReleaseBackEntity();
	ReleaseOrnamentEntity();
	ReleaseWidgetLine();

	// 解除載具連結
	DetachImplement();

	// 角色釋放之前,優先釋放坐騎物件
	ReleaseMountSprite();

	// 刪除暫存物件
	ReleaseTempEntity();

	IBaseSprite::Erase(this);	

	ReleaseEntity();

	ClearBuff();

	ClearCastingEffect();

	m_controller.clear();

	/*
	if (s_procFunc)
		s_procFunc(this, eSpriteMsg_Portrait_Changed, 0, 0);
	*/

#ifdef KALYDO
	for(vector<CDownloadHandStruct*>::iterator i = m_DownloadStructs.begin(); i != m_DownloadStructs.end(); i++)
		(*i)->spriteDeleted = true;
#endif // KALYDO
}

//-----------------------------------------------------------------------------
BOOL CRoleSprite::HandleEvent_DequeueAction(RuEventArgs *eventArgs)
{
	RuMimeEvent_DequeueAction_Args *dequeueArgs = static_cast<RuMimeEvent_DequeueAction_Args *>(eventArgs);

	/*
	if ( this->GetObjectType() == eSpriteObject_Player )
	{
		g_pError->AddMessage(0, 0, "%d %d", dequeueArgs->m_channel, dequeueArgs->m_action);
	}
	*/

	if(dequeueArgs->m_channel == ruFUSION_MIME_CHANNEL_FOREGROUND)
	{
		switch (dequeueArgs->m_action)
		{
		case ruFUSION_MIME_IDLE_COMBAT_BOW:
		case ruFUSION_MIME_IDLE_COMBAT_GUN:
			break;

		// 攻擊準備
		case ruFUSION_MIME_IDLE_COMBAT_1H:
		case ruFUSION_MIME_IDLE_COMBAT_2H:		
		case ruFUSION_MIME_IDLE_COMBAT_POLEARM:
		case ruFUSION_MIME_IDLE_COMBAT_UNARMED:
			SetWeaponPosition(true);
			break;

		// 攻擊動作
		case ruFUSION_MIME_ATTACK_1H:
		case ruFUSION_MIME_ATTACK_1H_SP:
		case ruFUSION_MIME_ATTACK_1H_SP02:
		case ruFUSION_MIME_ATTACK_1H_SLOW01:
		case ruFUSION_MIME_ATTACK_1H_PIERCE:			
		case ruFUSION_MIME_ATTACK_2H:
		case ruFUSION_MIME_ATTACK_2H_SP:
		case ruFUSION_MIME_ATTACK_POLEARM:
		case ruFUSION_MIME_ATTACK_POLEARM_SP:
		case ruFUSION_MIME_ATTACK_UNARMED:
		case ruFUSION_MIME_ATTACK_UNARMED_SP:		
		case ruFUSION_MIME_ATTACK_OH:
		case ruFUSION_MIME_ATTACK_OH_PIERCE:
			SetWeaponPosition(true);
			break;

		// 拿取遠程武器
		case ruFUSION_MIME_ATTACK_BOW_LOOP:
		case ruFUSION_MIME_ATTACK_BOW_END:		
		case ruFUSION_MIME_ATTACK_GUN_LOOP:
		case ruFUSION_MIME_ATTACK_GUN_END:
			SetRangedMotion("bow_draw");

		case ruFUSION_MIME_ATTACK_BOW_BEGIN:
		case ruFUSION_MIME_ATTACK_GUN_BEGIN:
			SetWeaponPosition(false);
			CreateHandsEntity(eRoleHands_Ranged, m_handsObjectId[eRoleHands_Ranged], 0);		// 拿取遠程武器
			break;

		// 收武器
		case ruFUSION_MIME_RIDE_IDLE:
		case ruFUSION_MIME_RIDE_IDLE0:
		case ruFUSION_MIME_CRAFTING_BEGIN:
		case ruFUSION_MIME_CRAFTING_LOOP:
		case ruFUSION_MIME_CRAFTING_END:
		case ruFUSION_MIME_MINING_BEGIN:
		case ruFUSION_MIME_MINING_LOOP:
		case ruFUSION_MIME_MINING_END:
		case ruFUSION_MIME_LUMBERJACK_BEGIN:
		case ruFUSION_MIME_LUMBERJACK_LOOP:
		case ruFUSION_MIME_LUMBERJACK_END:
		case ruFUSION_MIME_GATHER_BEGIN:
		case ruFUSION_MIME_GATHER_LOOP:
		case ruFUSION_MIME_GATHER_END:
		case ruFUSION_MIME_ACTIVATE_BEGIN:
		case ruFUSION_MIME_ACTIVATE_LOOP:
		case ruFUSION_MIME_ACTIVATE_END:
		case ruFUSION_MIME_READING_BEGIN:
		case ruFUSION_MIME_READING_LOOP:
		case ruFUSION_MIME_READING_END:
		case ruFUSION_MIME_SIT_CHAIR_BEGIN:
		case ruFUSION_MIME_SIT_CHAIR_LOOP:
		case ruFUSION_MIME_SIT_CHAIR_END:
		case ruFUSION_MIME_SLEEP_DOWN:
		case ruFUSION_MIME_SLEEP_LOOP:
		case ruFUSION_MIME_SLEEP_UP:
		case ruFUSION_MIME_SIT_DOWN:
		case ruFUSION_MIME_SIT_LOOP:
		case ruFUSION_MIME_SIT_UP:
		case ruFUSION_MIME_CROUCH_BEGIN:
		case ruFUSION_MIME_CROUCH_LOOP:
		case ruFUSION_MIME_CROUCH_END:
		case ruFUSION_MIME_DEAD_WATER:

		case ruFUSION_MIME_USE:
		case ruFUSION_MIME_THROW:

		case ruFUSION_MIME_BUFF_BEGIN:
		case ruFUSION_MIME_BUFF_LOOP:
		case ruFUSION_MIME_BUFF_END:
		case ruFUSION_MIME_BUFF_INSTANT:
		case ruFUSION_MIME_CAST_BEGIN:
		case ruFUSION_MIME_CAST_LOOP:
		case ruFUSION_MIME_CAST_END:
		case ruFUSION_MIME_CAST_INSTANT:
		case ruFUSION_MIME_CHANNEL_BEGIN:
		case ruFUSION_MIME_CHANNEL_LOOP:
		case ruFUSION_MIME_CHANNEL_END:
		case ruFUSION_MIME_CHANNEL_INSTANT:

		case ruFUSION_MIME_EMOTE_SWEEP1:
		case ruFUSION_MIME_EMOTE_SWEEP2:
		case ruFUSION_MIME_EMOTE_SWEEP3:

		case ruFUSION_MIME_EMOTE_ANGRY:
		case ruFUSION_MIME_EMOTE_APPLAUSE:
		case ruFUSION_MIME_EMOTE_APPROVAL:
		case ruFUSION_MIME_EMOTE_BEG:
		case ruFUSION_MIME_EMOTE_CRY:
		case ruFUSION_MIME_EMOTE_HEAD_NOD:
		case ruFUSION_MIME_EMOTE_HEAD_SHAKE:
		case ruFUSION_MIME_EMOTE_LAUGH:
		case ruFUSION_MIME_EMOTE_POINT:
		case ruFUSION_MIME_EMOTE_PROVOKE:
		case ruFUSION_MIME_EMOTE_SALUTE:
		case ruFUSION_MIME_EMOTE_SPEAK:
		case ruFUSION_MIME_EMOTE_SURRENDER:
		case ruFUSION_MIME_EMOTE_THREATEN:
		case ruFUSION_MIME_EMOTE_VICTORY:
		case ruFUSION_MIME_EMOTE_WAVE:
		case ruFUSION_MIME_EMOTE_EAT:
		case ruFUSION_MIME_EMOTE_DRINK:
		//case ruFUSION_MIME_EMOTE_IDLE:
		//case ruFUSION_MIME_EMOTE_IDLE0:
		//case ruFUSION_MIME_EMOTE_IDLE1:
		//case ruFUSION_MIME_EMOTE_IDLE2:
		case ruFUSION_MIME_EMOTE_DANCE:
		case ruFUSION_MIME_EMOTE_PLAY_SAXOPHONE:
		case ruFUSION_MIME_EMOTE_PLAY_TAMBOURINE:
		case ruFUSION_MIME_EMOTE_PLAY_GUITAR:			
		case ruFUSION_MIME_EMOTE_JUMP_INPLACE:
		case ruFUSION_MIME_EMOTE_BELLY:
		case ruFUSION_MIME_EMOTE_EMBRAASSED:
		case ruFUSION_MIME_EMOTE_EMBRACE:
		case ruFUSION_MIME_EMOTE_KISS:
		case ruFUSION_MIME_EMOTE_SALUTE2:
		case ruFUSION_MIME_EMOTE_CUTE1:	
		case ruFUSION_MIME_EMOTE_CUTE2:
		case ruFUSION_MIME_EMOTE_CUTE3:
		case ruFUSION_MIME_EMOTE_HANGOVER:
		case ruFUSION_MIME_EMOTE_STRETCH:
		case ruFUSION_MIME_EMOTE_ROCK:
		case ruFUSION_MIME_EMOTE_KAMEHAMEHA:
		case ruFUSION_MIME_EMOTE_KICK:	
		case ruFUSION_MIME_EMOTE_HANDSTAND:
		case ruFUSION_MIME_EMOTE_ROFL:
		case ruFUSION_MIME_EMOTE_BREAKDANCE:
		case ruFUSION_MIME_EMOTE_TAPDANCE:
		case ruFUSION_MIME_EMOTE_TIRED:
		case ruFUSION_MIME_EMOTE_FEAR:
		case ruFUSION_MIME_EMOTE_AMAZED:
		case ruFUSION_MIME_EMOTE_THINK:
			SetWeaponPosition(false);
			ReleaseHandsEntity(eRoleHands_Ranged);
			break;

		case ruFUSION_MIME_BUFF_SP01:
		case ruFUSION_MIME_BUFF_SP02:
		case ruFUSION_MIME_BUFF_SP03:
		case ruFUSION_MIME_CAST_SP01:
		case ruFUSION_MIME_CAST_SP02:
		case ruFUSION_MIME_CAST_SP03:
		case ruFUSION_MIME_CAST_SP04:
			SetWeaponPosition(true);
			ReleaseHandsEntity(eRoleHands_Ranged);
			break;
		}		
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
BOOL CRoleSprite::HandleProxyLoadComplete(RuEventArgs *eventArgs)
{
	// Chain call to base class
	IBaseSprite::HandleProxyLoadComplete(eventArgs);

	if(m_pRuEntity)
	{
		RuACT_PlugMissingMotions(m_pRuEntity, g_ruACT_DefaultMotionChart, FALSE);

		if(m_pRuMimeEntity == NULL)
		{
			CreateFusionMime();
		}
		m_pRuMimeEntity->SetTarget(m_pRuEntity);

		// If the hierarchy does not have two spine segments, use the deprecated template
		CRuHierarchy_GR2 *hierarchy = RuEntity_FindFirstHierarchy_GR2(m_pRuEntity);
		if(hierarchy && hierarchy->GetSubNodeIndexByName("Bip01 Spine2") == -1)
		{
			m_pRuMimeEntity->SetMimeTemplate(s_pRuMimeTemplate_Deprecated);
		}

		// Setup target's motion channels if applicable
		m_pRuMimeEntity->SetupMotionChannels();

		if(m_pRuActor == NULL)
		{
			CreateFusionActor();
		}
		m_pRuActor->SetTargetMime(m_pRuMimeEntity);

		UpdateDyeFXColors();

		// Update Buff
		UpdateBuffEffect();

		// Re-apply bone scale settings
		ReapplyBoneScaleSettings();

		// Mark paperdoll as being in need of a rebuild and rebuild it
		m_isNeedBuild = true;
		PaperdollBuild();	

		// POP actor State 
		for ( vector<SpriteActorQueue>::iterator iter = m_actorStateQueue.begin(); iter != m_actorStateQueue.end(); iter++ )
		{
			SetActorState((CRuFusion_ActorState)(iter->actor), iter->layer);
		}
		m_actorStateQueue.clear();
	}

	// 坐騎
	CRoleSprite* sprite = GetProxyControlSprite();
	if ( sprite != this )
	{
		sprite->LinkSprite(this, m_linkAttachPointName, m_linkControls, m_linkShadow);
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
void CRoleSprite::Update( float elapsedTime )
{
	if ( s_pRuTerrain )
	{
		CRuAABB bounds = s_pRuTerrain->GetEnumerationBounds();
		if( bounds.Contains(m_nowPos) == FALSE )
		{
			return;
		}
		else if ( GetStaticObjectState() && GetGravityMode() && m_isImplement == false && abs(m_nowPos.m_y - m_targetPos.m_y) > 100.0f )
		{
			CRuVector3 position;
			position.m_x = m_nowPos.m_x;
			position.m_y = m_targetPos.m_y;
			position.m_z = m_nowPos.m_z;
			IBaseSprite::SetPos(position);
		}
	}

	// 冰凍
	float freezeTime = elapsedTime;
	if ( m_specialEffect.freeze > 0 )
		freezeTime = 0.0f;

	// Check on status of any on-going paperdoll builds
	PaperdollBuildCheck(true);

	// 更新BUFF顯示
	UpdateBuff(freezeTime);

	// 更新集氣特效
	UpdateCastingEffect(freezeTime);

	// 更新動作時間
	UpdateActorQueue(freezeTime);

	// 控制器
	UpdateController(elapsedTime);	

	// 跟隨
	UpdateFollow(freezeTime);

	// 彈飛
	if ( m_controlState.isFloat )
	{
		m_floatTime += elapsedTime;
		if ( m_floatTime > 0.5f )
		{
			CRuVector3 position = GetPos();
			float height;
			if ( TerrainTestHight(position, height, 50.0f) && (position.m_y - height) < 10.0f )
			{
				m_controlState.isFloat = false;
			}
		}

		/*
		if ( m_pRuTempActor )
		{
			switch (m_pRuTempActor->GetBackgroundState())
			{
			case ruFUSION_ACTORSTATE_JUMP:
			case ruFUSION_ACTORSTATE_FALLING:
				break;

			default:
				m_controlState.isFloat = false;
				break;
			}
		}
		if ( m_pRuActor )
		{
			switch (m_pRuActor->GetBackgroundState())
			{
			case ruFUSION_ACTORSTATE_JUMP:
			case ruFUSION_ACTORSTATE_FALLING:
				break;

			default:
				m_controlState.isFloat = false;
				break;
			}
		}
		*/

		if ( m_controlState.isFloat == false )
		{
			if ( GetObjectType() != eSpriteObject_Player )
			{
				if ( m_pRuActor )
					m_pRuActor->SetMovementType(ruSOJMOVETYPE_NPC);
				if ( m_pRuTempActor )
					m_pRuTempActor->SetMovementType(ruSOJMOVETYPE_NPC);
			}
			return;
		}
	}	

	// Widget Update
	if ( m_isNeedResetWidget )
	{
		DWORD oldColor = m_nameColor;

		m_isNeedResetWidget = false;
		SetWidgetTexture();

		// 更新子物件
		for ( set<CRoleSprite*>::iterator iter = m_childSprite.begin(); iter != m_childSprite.end(); iter++ )
		{
			(*iter)->m_isNeedResetWidget = true;
		}

		if ( oldColor != m_nameColor )
		{
			if ( s_procFunc )
				s_procFunc(this, eSpriteMsg_NameColor_Changed, 0, 0);
		}
	}

	// 施法條
	if ( m_spellID != 0 )
	{
		m_currentTime = m_currentTime + elapsedTime;
		if ( m_currentTime > m_castingTime )
			m_currentTime = m_castingTime;
	}

	// If the dyeFX mask has not yet been loaded, attempt to build it again
	if(m_dyeFXMaskRequiresLoad)
	{
		UpdateDyeFXColors();
	}

	// NPC 物件自動朝向目標
	switch (m_objectType)
	{
	case eSpriteObject_NPC:
	case eSpriteObject_QuestNPC:
		if ( m_moveTime < 0.001f && IsLockedDirection() == false && m_targetSprite && m_targetSprite != this &&
			 m_followSprite == NULL &&
			 IsImplement() == false &&
			 m_specialEffect.hitDown <= 0 &&
			 m_specialEffect.chaos <= 0 &&
			 m_specialEffect.fear <= 0 &&
			 m_specialEffect.sleep <= 0 &&
			 m_specialEffect.dizzy <= 0 )
		{
			SetDirection(m_targetSprite->GetPos() - m_nowPos);
		}
		break;
	}

	if (m_attachParentSprite)
	{
		if ( m_attachParentRotate )
		{
			CRuQuaternion quaternion;
			CRuVector3 rightVector = CrossProduct(m_direction, CRuVector3(0.0f, 1.0f, 0.0f));
			if ( rightVector.Magnitude() > ruEPSILON )
			{
				CRuVector3 upVector = CrossProduct(rightVector, m_direction);
				upVector.Normalize();

				quaternion.RotateIntoVector(upVector, CRuVector3(0.0f,1.0f,0.0f));
				CRuVector3 forward(0.0f, 0.0f, 1.0f);
				quaternion.TransformVector(forward, forward);

				CRuQuaternion temp;
				temp.RotateIntoVector(m_direction, forward, upVector);
				quaternion = quaternion * temp;
			}

			CRuEntity* pPointRuEntity = m_attachParentSprite->FindLinkEntity(m_attachParentPoint);
			if ( pPointRuEntity )
			{
				if ( m_pRuEntity )
					m_pRuEntity->Rotate(quaternion * pPointRuEntity->GetRotation(ruFRAMETYPE_WORLD), ruTRANSFORMOP_SET);
				if ( m_pRuTempEntity )
					m_pRuTempEntity->Rotate(quaternion * pPointRuEntity->GetRotation(ruFRAMETYPE_WORLD), ruTRANSFORMOP_SET);
			}
		}
		else
		{
			/*
			CRuQuaternion quaternion;
			CRuVector3 rightVector = CrossProduct(m_direction, CRuVector3(0.0f, 1.0f, 0.0f));
			if ( rightVector.Magnitude() > ruEPSILON )
			{
				CRuVector3 upVector = CrossProduct(rightVector, m_direction);
				upVector.Normalize();

				quaternion.RotateIntoVector(upVector, CRuVector3(0.0f,1.0f,0.0f));
				CRuVector3 forward(0.0f, 0.0f, 1.0f);
				quaternion.TransformVector(forward, forward);

				CRuQuaternion temp;
				temp.RotateIntoVector(m_direction, forward, upVector);
				quaternion = quaternion * temp;
			}

			if ( m_pRuEntity )
				m_pRuEntity->Rotate(quaternion, ruTRANSFORMOP_SET);
			if ( m_pRuTempEntity )
				m_pRuTempEntity->Rotate(quaternion, ruTRANSFORMOP_SET);
			*/
			//IBaseSprite::SetDirection(m_direction, true);
		}

		IBaseSprite::SetPos(m_attachParentSprite->GetAttachPos(IBaseSprite::GetAttachPointName(m_attachParentPoint)));
	}
	// Calculate movement vector based on user input
	//CRuVector3 moveVector(rightDelta, 0.0f, forwardDelta);
	else if ( m_pRuActor )
	{
		bool resetAnchorHeight = false;
		CRuVector3 moveVector = (m_controlState.isFloat) ? CRuVector3(0.0f, 0.0f, 0.0f) : m_moveVector;

		// 是否在載具上,去除左右移動,只能前進後退
		if ( IsImplement() )
		{			
			if ( moveVector.m_x != 0.0f )
			{
				CRuVector3 direction = m_direction;
				float offset = (m_lastNowPos - m_nowPos).Magnitude();
				float angle = offset / moveVector.m_x / 2.5f;

				// 前進
				if ( moveVector.m_z > 0.0f )
				{
					angle = -angle;
				}

				CRuQuaternion cycleQuat;
				cycleQuat.FromAngleAxis(CRuVector3(0.0f, 1.0f, 0.0f), angle);
				cycleQuat.TransformVector(direction);
				SetDirection(direction, true);
			}
			moveVector.m_x = 0.0f;
		} else if ( m_moveType == eRoleSpriteMove_Normal && m_specialEffect.lockedMove > 0 ) {
			memset(&moveVector, 0, sizeof(moveVector));
		}

		// Set scale into actor object
		m_pRuActor->SetScale(m_scale * m_appnedScale);
		if ( m_pRuTempActor )
			m_pRuTempActor->SetScale(m_scale * m_appnedScale);

		if ( m_moveTime == NEXT_MOVETIME ) {
			m_moveTime = min(freezeTime, 1.0f);
		}
		
		if ( m_moveTime > 0.001f )
		{
			// 如果有移動向量,則做向量移動

			// 單次移動量小於更新時間,衰減移動量
			if ( m_moveTime < freezeTime ) {
				moveVector *= m_moveTime / freezeTime;
			}
			m_moveTime -= min(m_moveTime, freezeTime);
		}		
		else if ( moveVector.Magnitude() > 0.001f )
		{			
			resetAnchorHeight = true;

			// 向量值不為零,但移動時間已經結束.清除移動相關設定
			switch ( m_moveType )
			{
			case eRoleSpriteMove_Normal:
				// 清除移動向量
				ClearMoveVector();
				break;

			case eRoleSpriteMove_Charge:
			case eRoleSpriteMove_StrikeBack:
				SetMoveType(eRoleSpriteMove_Normal);

				// NPC 角色
				switch (m_objectType)
				{
				case eSpriteObject_NPC:
				case eSpriteObject_QuestNPC:
					if ( m_nextTargetPos != m_targetPos )
						SetTargetPos(m_nextTargetPos);
					break;
				}
				break;
			}

			// 更正最後面向
			if ( m_correctDir.m_x != 0.0f && m_correctDir.m_z != 0.0f )
				SetDirection(m_correctDir);
		}
		else
		{
			// 清除移動向量
			ClearMoveVector();
			SetMoveType(eRoleSpriteMove_Normal);
		}

		// Move and update actor
		if ( GetGravityMode() )
		{
			static bool enableCollisionRemove = true;

			CRuContainer_Collision* collisionContainer = IBaseSprite::GetCollisionContainer();
			bool removeSelf = false;			

			/*
			if ( !GetAboveWaterState() ) {
				float height;
				if ( !IBaseSprite::TerrainTestHight(m_nowPos, height, 100) || (m_nowPos.m_y - height) > 5.0f ) {
					moveVector = CRuVector3(0.0f, 0.0f, 0.0f);
				}
			}
			*/			

			m_pRuActor->Move(moveVector);

			// 如果移動中或者有碰撞暫時先移除自己
			if ( enableCollisionRemove && CollisionEnabled() && moveVector != 0.0f && collisionContainer )
			{
				removeSelf = true;
				if ( m_pRuEntity )
					collisionContainer->RemoveCollisionObject(m_pRuEntity);
				if ( m_pRuTempEntity )
					collisionContainer->RemoveCollisionObject(m_pRuTempEntity);
			}

			// 如果玩家沒有移動, 測試地板高度
			if ( m_objectType == eSpriteObject_Player && moveVector.m_x == 0.0f && moveVector.m_y == 0.0f && moveVector.m_z == 0.0f )
			{
				switch ( m_pRuActor->GetBackgroundState() )
				{
				case ruFUSION_ACTORSTATE_JUMP:
				case ruFUSION_ACTORSTATE_FALLING:
				case ruFUSION_ACTORSTATE_SWIM_SURFACE:
				case ruFUSION_ACTORSTATE_SWIM_DIVE:
					break;

				default:
					// 如果玩家沒有移動, 測試地板高度
					if ( m_objectType == eSpriteObject_Player && moveVector.m_x == 0.0f && moveVector.m_y == 0.0f && moveVector.m_z == 0.0f )
					{
						CRuVector3 position = m_pRuEntity->GetTranslation();
						float height = 0.0f;
						position.m_y += 20.0f;
						if ( !IBaseSprite::TerrainTestHight(position, height, 80.0f) || abs(position.m_y - height) > 30.0f ) {
							m_pRuActor->SetBackgroundState(ruFUSION_ACTORSTATE_FALLING, true);
							if ( m_pRuTempActor )
								m_pRuTempActor->SetBackgroundState(ruFUSION_ACTORSTATE_FALLING, true);
							//SetActorState()
							//SetBackgroundState();
							//position.m_y = height;
							//IBaseSprite::SetPos(position);
						}
					}
					break;
				}
			}

			switch ( m_pRuActor->GetBackgroundState() )
			{
			case ruFUSION_ACTORSTATE_JUMP:
			case ruFUSION_ACTORSTATE_FALLING:
				{
					float updateTime = freezeTime;
					while ( updateTime > 0.0f )
					{
						m_pRuActor->Update(min(updateTime, 0.1f));
						updateTime -= 0.1f;
					}
				}				
				break;

			default:
				// 不是載具的 NPC 移動, 特殊修正
				if ( !IsImplement() && (m_objectType == eSpriteObject_NPC || m_objectType == eSpriteObject_QuestNPC || m_objectType == eSpriteObject_BattleGroupObj) )
				{
					// 與SERVER坐標不符合時修正NPC座標
					if ( abs(m_targetPos.m_y - m_nowPos.m_y) > 5.0f )
					{
						// 移動中
						if ( moveVector != 0.0f )
						{
							CRuVector3 nowPos = m_nowPos;
							nowPos.m_y = (nowPos.m_y + m_targetPos.m_y) / 2;
							nowPos.m_y = RuEntity_FindRestingHeight(nowPos, s_collisionContainer, 50.0f, 15.0f);
							IBaseSprite::SetPos(nowPos);
						}
						// 停止中
						/*
						else 
						{
							CRuVector3 nowPos = m_nowPos;
							nowPos.m_y = m_targetPos.m_y;
							nowPos.m_y = RuEntity_FindRestingHeight(nowPos, s_collisionContainer, 50.0f, 15.0f);
							IBaseSprite::SetPos(nowPos);
						}
						*/
					}
				}

				m_pRuActor->Update(freezeTime);
				break;
			}

			if ( m_pRuTempActor )
			{
				m_pRuTempActor->Move(moveVector);

				switch ( m_pRuTempActor->GetBackgroundState() )
				{
				case ruFUSION_ACTORSTATE_JUMP:
				case ruFUSION_ACTORSTATE_FALLING:
					{
						float updateTime = freezeTime;
						while ( updateTime > 0.0f )
						{
							m_pRuTempActor->Update(min(updateTime, 0.1f));
							updateTime -= 0.1f;
						}
					}				
					break;

				default:
					m_pRuTempActor->Update(freezeTime);
					break;
				}
			}

			if ( removeSelf && collisionContainer )
			{
				if ( m_pRuEntity )
					collisionContainer->InsertCollisionObject(m_pRuEntity, m_pRuEntity->GetWorldBounds(), TRUE);
				if ( m_pRuTempEntity )
					collisionContainer->InsertCollisionObject(m_pRuTempEntity, m_pRuTempEntity->GetWorldBounds(), TRUE);
			}
		}
		else
		{
			m_pRuActor->Move(CRuVector3(0.0f, 0.0f, 0.0f));
			m_pRuActor->Update(freezeTime);
			resetAnchorHeight = false;
			IBaseSprite::SetPos(m_nowPos + (moveVector * freezeTime));
		}

		// Update cached sprite position
		m_lastNowPos = m_nowPos;
		m_nowPos = m_pRuEntity->GetTranslation();
		if ( m_pRuTempEntity )
			m_pRuTempEntity->SetTranslation(m_nowPos);

		/*
		if ( m_objectId == 102297 )
		{
			static CRuVector3 tempNowPosition = m_nowPos;
			if ( tempNowPosition != m_nowPos )
			{
				tempNowPosition = m_nowPos;
				g_pError->AddMessage(0, 0, "Now Pos (%.2f, %.2f, %.2f)", m_nowPos.m_x, m_nowPos.m_y, m_nowPos.m_z);
			}
		}
		*/

		/*
		// 非玩家物件需要重新定位高度
		if ( resetAnchorHeight && GetObjectType() != eSpriteObject_Player )
		{
			// 修正高度值
			CRuVector3 anchorPosition = m_nowPos;
			anchorPosition.m_y = RuEntity_FindRestingHeight(anchorPosition, s_pRuTerrain);

			m_nowPos.m_y = anchorPosition.m_y;
			m_pRuEntity->SetTranslation(m_nowPos);

			// Notify actor that its position has been reset
			if(m_pRuActor) {
				m_pRuActor->NotifyPositionReset();
			}
		}
		*/

		if (m_state.isSit)
		{
			switch (m_pRuActor->GetBackgroundState())
			{
			case ruFUSION_ACTORSTATE_SIT_BEGIN:
			case ruFUSION_ACTORSTATE_SIT_LOOP:
			case ruFUSION_ACTORSTATE_SIT_END:
				break;

			default:
				SetSitState(false);
				break;
			}
		}

		if (m_state.isSitChair)
		{
			switch (m_pRuActor->GetBackgroundState())
			{
			case ruFUSION_ACTORSTATE_SIT_CHAIR_BEGIN:
			case ruFUSION_ACTORSTATE_SIT_CHAIR_LOOP:
				break;

			default:
				SetSitChairState(false);
				break;
			}
		}

		EffectUpdate();
		
		/*
		// 戰鬥中,強制設定武器在手上
		if ( GetCombatState() && GetSpellID() == 0 )
		{
			if ( GetActorState(1) == ruFUSION_ACTORSTATE_NORMAL && GetActorState(3) == ruFUSION_ACTORSTATE_NORMAL)
			{
				switch ( GetActorState(2) )
				{
				case ruFUSION_ACTORSTATE_COMBAT_1H:
				case ruFUSION_ACTORSTATE_COMBAT_2H:
				case ruFUSION_ACTORSTATE_COMBAT_POLEARM:
				case ruFUSION_ACTORSTATE_COMBAT_UNARMED:
					SetWeaponPosition(true);
					break;
				}
			}			
		}
		*/

		/*
		// 特別採集動作或者射擊動作
		switch (m_handsEntity[2].type)
		{
		case eRoleHands_Ranged:
			if ( GetActorState(3) == ruFUSION_ACTORSTATE_NORMAL )
			{
				ReleaseHandsEntity(eRoleHands_Arrow);
				ReleaseHandsEntity(eRoleHands_Ranged);
				if ( GetCombatState() )
					SetWeaponPosition(true);
			}
			break;
		}
		*/
	}
	
	if ( m_pRuMimeEntity )
		m_pRuMimeEntity->Update(freezeTime);
	if ( m_pRuTempMimeEntity )
		m_pRuTempMimeEntity->Update(freezeTime);	

	// Entity Update
	/*
	char tmp[MAX_PATH];
	sprintf( tmp, "ID:%d Lv:%d HP:%d/%d MP:%d/%d", m_id, m_level, m_hp, m_maxHp, m_mp, m_maxMp );
	SetDebugOut1( tmp );
	*/

	// 裝飾品物件
	UpdateOrnamentEntity();	

	// 坐騎地板物件顏色
	if ( m_mountSprite )
		m_mountSprite->m_nameColor = m_nameColor;

	// 設定地板物件(Target)顏色
	if ( m_pRuFloorEntity )
	{
		CRuParameterBlock *parameterBlock = m_pRuFloorEntity->GetParameterBlock();
		UINT64 parameterOut;

		parameterBlock->GetParameter(ruPARAMTYPE_MATERIAL_DIFFUSE, parameterOut);
		if ( parameterOut != (UINT64)m_nameColor )
			parameterBlock->SetParameter(ruPARAMTYPE_MATERIAL_DIFFUSE, m_nameColor);
	}

	// 更新效果光球
	if (m_pRuCollectorFX && m_pRuCollectorFX->IsCollectorFXFinished())
	{
		ReleaseCollectorFX();
	}
	
	// Quest Entity
	if ( m_pRuQuestEntity )
	{
		m_pRuQuestEntity->ResetLocalTransform();

		// 顯示名稱
		if ( GetNameVisibleForce() && GetNameVisible() )
			m_pRuQuestEntity->Translate( CRuVector3( 0.0f, 6.0f, 0.0f ) );
		else
			m_pRuQuestEntity->Translate( CRuVector3( 0.0f, 2.0f, 0.0f ) );		
	}

	// Alpha 控制器
	if ( m_pRuGlobalAlpha )
	{
		m_pRuGlobalAlpha->Update(elapsedTime);
		if ( m_pRuGlobalAlpha->HasExpired() )
		{
			ruSAFE_RELEASE(m_pRuGlobalAlpha);
		}
	}
	else if ( m_pRuEntity )
	{
		CRuParameterBlock *parameterBlock = m_pRuEntity->GetParameterBlock();
		float alpha = m_alpha;

		if ( GetSneakAlphaState() )
			alpha *= 0.5f;

		parameterBlock->SetParameter(ruPARAMTYPE_MATERIAL_ALPHA, RuCastFloatToINT64(alpha));
	}

	if ( m_pRuWidgetLine[0] )
	{
		m_pRuWidgetLine[0]->Update(elapsedTime);
		m_pRuWidgetLine[1]->Update(elapsedTime);
		m_pRuWidgetLine[2]->Update(elapsedTime);
	}	

	IBaseSprite::Update( freezeTime );

	if ( !m_attachParentRotate )
		UpdateOrientation();
}

//-----------------------------------------------------------------------------
void CRoleSprite::Render()
{
	if ( s_gm_ShowHidden == false && IsScriptHide() ) { return; }

	if ( m_pRuWidgetLine[0] )
	{
		g_ruDefaultRenderContext->Submit(m_pRuWidgetLine[0]);
		g_ruDefaultRenderContext->Submit(m_pRuWidgetLine[1]);
		g_ruDefaultRenderContext->Submit(m_pRuWidgetLine[2]);
	}	
	IBaseSprite::Render();
}

//-----------------------------------------------------------------------------
void CRoleSprite::ReceiveDestruct(IBaseSprite* pSprite)
{
	RemoveChildSprite((CRoleSprite*)pSprite);
	if ( m_petSprite == pSprite )
		SetPetSprite(NULL);
	if ( m_masterSprite == pSprite )
		SetMasterSprite(NULL, m_controlState.playerPet);
	if ( m_mountSprite == pSprite ) 
		m_mountSprite = NULL;	
	if ( pSprite == m_targetSprite )
		SetTargetSprite(NULL);	
	if ( m_pCorpseEffect == pSprite )
		m_pCorpseEffect = NULL;	
	if ( m_followSprite == pSprite )
		m_followSprite = NULL;
	if ( m_implementSprite == pSprite )
		DetachImplement();
	if ( m_attachParentSprite == pSprite )
		SetAttachParent(NULL, 0);

	/*	
	// 子元件被刪除
	for(INT32 i = 0; i < m_linkedSprites.Count(); ++i)
	{
		if(m_linkedSprites[i] == pSprite)
		{
			// 解除連結
			this->UnlinkSprite(pSprite);
			this->GetRuActor()->UnlinkActor(pSprite->GetCurrentRuActor());
			break;
		}
	}
	*/	
	
	// 清除集氣特效物件
	{
		vector<CEffectSprite*>::iterator iter = m_castingEffect.begin();
		while ( iter != m_castingEffect.end() )
		{
			if ( *iter == pSprite )
				iter = m_castingEffect.erase(iter);
			else
				iter++;
		}
	}

	// 清除BUFF特效物件
	{
		for ( vector<RoleBuffInfo>::iterator iter = m_buffInfo.begin(); iter != m_buffInfo.end(); iter++ )
		{
			if ( iter->effect == pSprite )
				iter->effect = NULL;
		}
	}

	/*
	for ( vector<CEffectSprite*>::iterator iter = m_castingEffect.begin(); iter != m_castingEffect.end(); iter++ ) {
		if ( *iter == pSprite ) {
			m_castingEffect.erase(iter);
			return;
		}
	}	

	// 清除BUFF特效物件
	for ( vector<RoleBuffInfo>::iterator iter = m_buffInfo.begin(); iter != m_buffInfo.end(); iter++ )
	{
		if ( iter->effect == pSprite )
			iter->effect = NULL;
	}
	*/
}

//-----------------------------------------------------------------------------
void CRoleSprite::UnlinkChildSprite()
{
	// 解除連結
	if ( s_procFunc )
		s_procFunc(this, eSpriteMsg_Unlink, 0, 0);

	SetGravityMode(true);
	EnableTorsoTurn(true);
	SetDefaultIdleAction(ruFUSION_MIME_IDLE_STAND);
	SetActorState(ruFUSION_ACTORSTATE_NORMAL);
	SetOrientingAxis(CRuVector3(0.0f, 1.0f, 0.0f));
	AnchorSprite(36.0f);

	// 移除 Detach 在座騎 p_down 物件移置角色 p_down
	for ( map<IBaseSprite*, int>::iterator iter = m_attachSprite.begin(); iter != m_attachSprite.end(); iter++ )
	{
		CRuEntity* entity = iter->first->GetRuEntity();
		CRuEntity* parentEntity = GetCurrentEntity();

		if ( entity )
		{
			if ( iter->second == eAttach_Down )
			{
				entity->DetachFromParent();

				RuEntity_Attach_BFS(parentEntity, entity, GetAttachPointName(iter->second));
			}
		}
	}

	// 重新設定角色參數
	if(m_pRuActor)
	{
		m_pRuActor->EnableSojourner(TRUE);
		m_pRuActor->Update(0.0f);
	}

	// 是否有鎖定移動
	if ( m_specialEffect.lockedMove > 0 )
		m_specialEffect.lockedMove--;

	m_attachParentId = 0;

	IBaseSprite::UnlinkChildSprite();	
}

//-----------------------------------------------------------------------------
void CRoleSprite::SeverSpriteLinks()
{
	// Unlink children
	if ( m_pRuActor )
	{		
		for(INT32 i = 0; i < m_linkedSprites.Count(); ++i)
		{
			m_pRuActor->UnlinkActor(((CRoleSprite*)m_linkedSprites[i])->GetRuActor());
			m_pRuActor->UnlinkActor(((CRoleSprite*)m_linkedSprites[i])->GetRuTempActor());
		}
	}

	// Unlink parent
	if ( m_linkParent )
	{
		((CRoleSprite*)m_linkParent)->GetRuActor()->UnlinkActor(m_pRuTempActor);
		((CRoleSprite*)m_linkParent)->GetRuActor()->UnlinkActor(m_pRuActor);
	}

	IBaseSprite::SeverSpriteLinks();
}

//-----------------------------------------------------------------------------
void CRoleSprite::MotionEvent( int type )
{
	switch ( type )
	{
	case ruENTITYEVENT_HIT:
		break;

	case ruENTITYEVENT_HOLSTER:
		SetWeaponPosition( false );
		break;

	case ruENTITYEVENT_UNHOLSTER:
		SetWeaponPosition( true );
		break;
	}
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetObjectType( SpriteObjectType type )
{
	// Chain call to base class
	IBaseSprite::SetObjectType(type);

	// Set actor movement mode based on object type
	if(m_pRuActor)
	{
		switch(m_objectType)
		{
			case eSpriteObject_Player:
				m_pRuActor->SetMovementType(ruSOJMOVETYPE_FULL);
				break;

			default:
			case eSpriteObject_NPC:
			case eSpriteObject_QuestNPC:
				m_pRuActor->SetMovementType(ruSOJMOVETYPE_NPC);
				break;
		}
	}
}

//-----------------------------------------------------------------------------
bool CRoleSprite::SetObjectId( int id, bool deferredLoad )
{
	if( m_objectId == id )
	{
		return true;
	}	

	ReleasFusionMime();
	ReleasFusionActor();
	//ruSAFE_RELEASE( m_pRuMimeEntity );
	//ruSAFE_RELEASE( m_pRuActor );

	if ( IBaseSprite::SetObjectId( id, deferredLoad ) )
	{
		// 設定紙娃娃部位裝備
		GameObjDbStruct* pObj = CNetGlobal::GetObj(id);
		if ( pObj == NULL )
			return false;

		GameObjDbStruct* imageDB = CNetGlobal::GetObj(pObj->ImageID);
		if ( imageDB == NULL )
			return false;

		if ( m_pRuEntity )
			RuACT_PlugMissingMotions(m_pRuEntity, g_ruACT_DefaultMotionChart, FALSE);		

		// 設定傀儡控制物件
		if(m_pRuMimeEntity == NULL)
		{
			CreateFusionMime();
		}		

		if(m_pRuActor == NULL)
		{
			CreateFusionActor();
		}

		m_attackRange = (float)imageDB->Image.BoundRadiusY;

		SetPriorityAttack(false);
		SetImplementState(false);
		SetImplementAlignment(false);

		switch ( pObj->Classification )
		{
		case EM_ObjectClass_NPC:
		case EM_ObjectClass_QuestNPC:
			// 是否優先判斷攻擊
			SetPriorityAttack(pObj->NPC.IsAttackFirst);

			// 是否為載具
			switch (pObj->NPC.CarryType)
			{
			case EM_NPCCarryType_NoControl:
			case EM_NPCCarryType_Control:
				SetImplementState(true);
				SetImplementMaxNum(pObj->NPC.CarryCount);
				m_orientToSurfaceForword = true;
				break;

			default:
				SetImplementMaxNum(0);
				break;
			}
			break;
		}

		switch (m_objectType)
		{
		case eSpriteObject_Unknown:
			if ( !m_disableUpdateEntity )
			{
				//公會屋 傢俱假人
				SetPaperdollPart( PAPERDOLL_PART_NAME_HEAD      );
				SetPaperdollPart( PAPERDOLL_PART_NAME_HAIR      );			
				SetPaperdollPart( PAPERDOLL_PART_NAME_HELMET    ); //頭盔
				SetPaperdollPart( PAPERDOLL_PART_NAME_HAND      ); //手
				SetPaperdollPart( PAPERDOLL_PART_NAME_SHOULDER  ); //肩
				SetPaperdollPart( PAPERDOLL_PART_NAME_TORSO     ); //軀幹
				SetPaperdollPart( PAPERDOLL_PART_NAME_LEG       ); //腿
				SetPaperdollPart( PAPERDOLL_PART_NAME_BELT      ); //腰帶
				SetPaperdollPart( PAPERDOLL_PART_NAME_FOOT      ); //腳
				SetPaperdollPart( PAPERDOLL_PART_NAME_BACK      ); //背部
				PaperdollBuild();
			}			
			break;

		case eSpriteObject_Player:
			if ( !m_disableUpdateEntity )
			{
				// 設定紙娃娃部位裝備
				SetPaperdollPart(PAPERDOLL_PART_NAME_HEAD);
				SetPaperdollPart(PAPERDOLL_PART_NAME_HAIR);			
				SetPaperdollPart(PAPERDOLL_PART_NAME_TORSO);
				SetPaperdollPart(PAPERDOLL_PART_NAME_HAND);
				SetPaperdollPart(PAPERDOLL_PART_NAME_LEG);
				SetPaperdollPart(PAPERDOLL_PART_NAME_FOOT);
				PaperdollBuild();
				m_pRuActor->EnableTorsoTurn(true);
				m_pRuActor->SetMovementType(ruSOJMOVETYPE_FULL);				
			}
			break;

		case eSpriteObject_NPC:
		case eSpriteObject_QuestNPC:
			if ( !m_disableUpdateEntity )
			{
				SetPaperdollColor(PAPERDOLL_PART_NAME_HEAD, imageDB->Image.SkinColor.Color, 0);
				SetPaperdollColor(PAPERDOLL_PART_NAME_HAIR, imageDB->Image.HairColor.Color, 0);

				SetPaperdollPart(PAPERDOLL_PART_NAME_HEAD, imageDB->Image.PaperdollFace.Name);
				if ( imageDB->Image.PaperdollFace.MainColor.Color != 0 || imageDB->Image.PaperdollFace.OffColor.Color != 0 )
					SetPaperdollColor(PAPERDOLL_PART_NAME_HEAD, imageDB->Image.PaperdollFace.MainColor.Color, imageDB->Image.PaperdollFace.OffColor.Color, false);

				
				SetPaperdollPart(PAPERDOLL_PART_NAME_HAIR, imageDB->Image.PaperdollHair.Name);
				SetPaperdollPart(PAPERDOLL_PART_NAME_HELMET, imageDB->Image.PaperdollHead.Name);
				SetPaperdollColor(PAPERDOLL_PART_NAME_HELMET, imageDB->Image.PaperdollHead.MainColor.Color, imageDB->Image.PaperdollHead.OffColor.Color);
				SetPaperdollPart(PAPERDOLL_PART_NAME_SHOULDER, imageDB->Image.PaperdollShouder.Name);
				SetPaperdollColor(PAPERDOLL_PART_NAME_SHOULDER, imageDB->Image.PaperdollShouder.MainColor.Color, imageDB->Image.PaperdollShouder.OffColor.Color);
				SetPaperdollPart(PAPERDOLL_PART_NAME_TORSO, imageDB->Image.PaperdollClothes.Name);
				SetPaperdollColor(PAPERDOLL_PART_NAME_TORSO, imageDB->Image.PaperdollClothes.MainColor.Color, imageDB->Image.PaperdollClothes.OffColor.Color);
				SetPaperdollPart(PAPERDOLL_PART_NAME_HAND, imageDB->Image.PaperdollGloves.Name);
				SetPaperdollColor(PAPERDOLL_PART_NAME_HAND, imageDB->Image.PaperdollGloves.MainColor.Color, imageDB->Image.PaperdollGloves.OffColor.Color);
				SetPaperdollPart(PAPERDOLL_PART_NAME_BELT, imageDB->Image.PaperdollBelt.Name);
				SetPaperdollColor(PAPERDOLL_PART_NAME_BELT, imageDB->Image.PaperdollBelt.MainColor.Color, imageDB->Image.PaperdollBelt.OffColor.Color);
				SetPaperdollPart(PAPERDOLL_PART_NAME_LEG, imageDB->Image.PaperdollPants.Name);
				SetPaperdollColor(PAPERDOLL_PART_NAME_LEG, imageDB->Image.PaperdollPants.MainColor.Color, imageDB->Image.PaperdollPants.OffColor.Color);
				SetPaperdollPart(PAPERDOLL_PART_NAME_FOOT, imageDB->Image.PaperdollShoes.Name);
				SetPaperdollColor(PAPERDOLL_PART_NAME_FOOT, imageDB->Image.PaperdollShoes.MainColor.Color, imageDB->Image.PaperdollShoes.OffColor.Color);
				SetPaperdollPart(PAPERDOLL_PART_NAME_BACK, imageDB->Image.PaperdollBack.Name);
				SetPaperdollColor(PAPERDOLL_PART_NAME_BACK, imageDB->Image.PaperdollBack.MainColor.Color, imageDB->Image.PaperdollBack.OffColor.Color);
				PaperdollBuild();

				if ( IsImplement() )
				{
					m_pRuActor->SetMovementType(ruSOJMOVETYPE_FULL);
				}
				else
				{
					m_pRuActor->SetMovementType(ruSOJMOVETYPE_NPC);
				}
				
				SetWeaponPosition(true);				// 手持武器
			}			
			break;
		}

		// Update sub node bounds
		if ( m_pRuEntity )
			RuEntity_RefreshHierarchySubNodeBounds(m_pRuEntity);		
		
		SetDefaultIdleAction(ruFUSION_MIME_IDLE_STAND);
		SetActorState(ruFUSION_ACTORSTATE_NORMAL, 1);
		SetDirection(m_direction, true);
		SetGravityMode(false);

		//Update(0.0f);	// DO NOT UN-COMMENT THIS! THIS CAUSES UNNECESSARY TERRAIN LOADING BECAUSE POSITION HAS NOT YET BEEN SET!
		return true;
	}	

	return false;
}

//-----------------------------------------------------------------------------
bool CRoleSprite::SetTempObjectId(int id, bool takeWeapon)
{
	GameObjDbStruct* pObj = CNetGlobal::GetObj(id);
	if ( pObj == NULL )
		return false;

	GameObjDbStruct* imageDB = CNetGlobal::GetObj(pObj->ImageID);
	if ( imageDB == NULL )
		return false;

	if ( pObj && LoadRuTempEntity(imageDB->Image.ACTWorld) )
	{
		// 設定替換動作列表
		if ( m_pRuTempEntity )
		{
			RuACT_PlugMissingMotions(m_pRuTempEntity, g_ruACT_DefaultMotionChart, FALSE);
		}

		switch (m_objectType)
		{
		case eSpriteObject_Player:
			m_pRuTempActor->SetMovementType(ruSOJMOVETYPE_FULL);
			break;

		case eSpriteObject_NPC:
		case eSpriteObject_QuestNPC:
			m_pRuTempActor->SetMovementType(ruSOJMOVETYPE_NPC);
			break;
		}

		// 部位顏色
		BOOL layerActivated[4] = { FALSE, FALSE, FALSE, FALSE };
		RUCOLOR layerColor[4] = { 0, 0, 0, 0 };
		const char* faceName = "";
		const char* hairName = "";
				
		if ( m_pRuEntity )
		{
			IRuPaperdoll* paperdoll = RuEntity_FindFirstPaperdoll(m_pRuEntity);
			if ( paperdoll )
			{
				faceName = paperdoll->GetComponent(PAPERDOLL_PART_NAME_HEAD);
				hairName = paperdoll->GetComponent(PAPERDOLL_PART_NAME_HAIR);
				paperdoll->GetComponentColors(PAPERDOLL_PART_NAME_HEAD, layerActivated, layerColor);
				paperdoll->GetComponentColors(PAPERDOLL_PART_NAME_HAIR, layerActivated, layerColor);
			}
		}

		if ( imageDB->Image.PaperdollFace.Name[0] != 0 ) faceName = imageDB->Image.PaperdollFace.Name;
		if ( imageDB->Image.PaperdollHair.Name[0] != 0 ) hairName = imageDB->Image.PaperdollHair.Name;
		
		// 設定紙娃娃部位
		SetEntityPaperdoll(m_pRuTempEntity, PAPERDOLL_PART_NAME_HEAD, faceName);
		SetEntityPaperdoll(m_pRuTempEntity, PAPERDOLL_PART_NAME_HAIR, hairName);
		SetEntityPaperdoll(m_pRuTempEntity, PAPERDOLL_PART_NAME_HELMET, imageDB->Image.PaperdollHead.Name);
		SetEntityPaperdoll(m_pRuTempEntity, PAPERDOLL_PART_NAME_SHOULDER, imageDB->Image.PaperdollShouder.Name);
		SetEntityPaperdoll(m_pRuTempEntity, PAPERDOLL_PART_NAME_TORSO, imageDB->Image.PaperdollClothes.Name);
		SetEntityPaperdoll(m_pRuTempEntity, PAPERDOLL_PART_NAME_HAND, imageDB->Image.PaperdollGloves.Name);
		SetEntityPaperdoll(m_pRuTempEntity, PAPERDOLL_PART_NAME_BELT, imageDB->Image.PaperdollBelt.Name);
		SetEntityPaperdoll(m_pRuTempEntity, PAPERDOLL_PART_NAME_LEG, imageDB->Image.PaperdollPants.Name);
		SetEntityPaperdoll(m_pRuTempEntity, PAPERDOLL_PART_NAME_FOOT, imageDB->Image.PaperdollShoes.Name);
		SetEntityPaperdoll(m_pRuTempEntity, PAPERDOLL_PART_NAME_BACK, imageDB->Image.PaperdollBack.Name);		

		// 膚色髮色
		if ( imageDB->Image.SkinColor.Color != 0 )  
		{
			layerColor[0] = imageDB->Image.SkinColor.Color;			
			layerActivated[0] = layerColor[0] & 0xFF000000;
		}
		if ( imageDB->Image.HairColor.Color != 0 )
		{
			layerColor[1] = imageDB->Image.HairColor.Color;
			layerActivated[1] = layerColor[1] & 0xFF000000;
		}		
		
		RuPaperdoll_SetComponentColors(m_pRuTempEntity, PAPERDOLL_PART_NAME_HEAD, layerActivated, layerColor);
		RuPaperdoll_SetComponentColors(m_pRuTempEntity, PAPERDOLL_PART_NAME_HAIR, layerActivated, layerColor);

		// 頭
		layerColor[2] = imageDB->Image.PaperdollHead.MainColor.Color;
		layerColor[3] = imageDB->Image.PaperdollHead.OffColor.Color;
		layerActivated[2] = layerColor[2] & 0xFF000000;
		layerActivated[3] = layerColor[3] & 0xFF000000;
		RuPaperdoll_SetComponentColors(m_pRuTempEntity, PAPERDOLL_PART_NAME_HELMET, layerActivated, layerColor);
		// 肩
		layerColor[2] = imageDB->Image.PaperdollShouder.MainColor.Color;
		layerColor[3] = imageDB->Image.PaperdollShouder.OffColor.Color;
		layerActivated[2] = layerColor[2] & 0xFF000000;
		layerActivated[3] = layerColor[3] & 0xFF000000;
		RuPaperdoll_SetComponentColors(m_pRuTempEntity, PAPERDOLL_PART_NAME_SHOULDER, layerActivated, layerColor);
		// 身
		layerColor[2] = imageDB->Image.PaperdollClothes.MainColor.Color;
		layerColor[3] = imageDB->Image.PaperdollClothes.OffColor.Color;
		layerActivated[2] = layerColor[2] & 0xFF000000;
		layerActivated[3] = layerColor[3] & 0xFF000000;
		RuPaperdoll_SetComponentColors(m_pRuTempEntity, PAPERDOLL_PART_NAME_TORSO, layerActivated, layerColor);
		// 手
		layerColor[2] = imageDB->Image.PaperdollGloves.MainColor.Color;
		layerColor[3] = imageDB->Image.PaperdollGloves.OffColor.Color;
		layerActivated[2] = layerColor[2] & 0xFF000000;
		layerActivated[3] = layerColor[3] & 0xFF000000;
		RuPaperdoll_SetComponentColors(m_pRuTempEntity, PAPERDOLL_PART_NAME_HAND, layerActivated, layerColor);
		// 腰
		layerColor[2] = imageDB->Image.PaperdollBelt.MainColor.Color;
		layerColor[3] = imageDB->Image.PaperdollBelt.OffColor.Color;
		layerActivated[2] = layerColor[2] & 0xFF000000;
		layerActivated[3] = layerColor[3] & 0xFF000000;
		RuPaperdoll_SetComponentColors(m_pRuTempEntity, PAPERDOLL_PART_NAME_BELT, layerActivated, layerColor);
		// 腿
		layerColor[2] = imageDB->Image.PaperdollPants.MainColor.Color;
		layerColor[3] = imageDB->Image.PaperdollPants.OffColor.Color;
		layerActivated[2] = layerColor[2] & 0xFF000000;
		layerActivated[3] = layerColor[3] & 0xFF000000;
		RuPaperdoll_SetComponentColors(m_pRuTempEntity, PAPERDOLL_PART_NAME_LEG, layerActivated, layerColor);
		// 腳
		layerColor[2] = imageDB->Image.PaperdollShoes.MainColor.Color;
		layerColor[3] = imageDB->Image.PaperdollShoes.OffColor.Color;
		layerActivated[2] = layerColor[2] & 0xFF000000;
		layerActivated[3] = layerColor[3] & 0xFF000000;
		RuPaperdoll_SetComponentColors(m_pRuTempEntity, PAPERDOLL_PART_NAME_FOOT, layerActivated, layerColor);
		// 背
		layerColor[2] = imageDB->Image.PaperdollBack.MainColor.Color;
		layerColor[3] = imageDB->Image.PaperdollBack.OffColor.Color;
		layerActivated[2] = layerColor[2] & 0xFF000000;
		layerActivated[3] = layerColor[3] & 0xFF000000;
		RuPaperdoll_SetComponentColors(m_pRuTempEntity, PAPERDOLL_PART_NAME_BACK, layerActivated, layerColor);
		RuPaperdoll_Build(m_pRuTempEntity, FALSE);

		// 座騎
		if ( m_mountSprite )
		{
			m_mountSprite->GetRuActor()->UnlinkActor(m_pRuActor);
			m_mountSprite->GetRuActor()->LinkActor(m_pRuTempActor);
		}		

		RuACT_PlayPersistentMotion(m_pRuTempEntity);

		// Update sub node bounds
		if ( m_pRuTempEntity )
		{
			RuEntity_RefreshHierarchySubNodeBounds(m_pRuTempEntity);

			// 更換附加物件
			TransAttachSprite(m_pRuTempEntity);
		}

		if ( m_pRuTempActor )
		{			
			m_pRuTempActor->SetUpVector(this->GetOrientingAxis());
			m_pRuTempActor->SetForwardVector(m_direction, true);

			// 設定顯示比例
			m_pRuTempActor->SetScale(imageDB->Image.ModelSize / 100.0f);

			if ( m_pRuActor )
			{
				m_pRuTempActor->SetDefaultWalkingAction(m_pRuActor->GetDefaultWalkingAction());
				m_pRuTempActor->SetDefaultRunningAction(m_pRuActor->GetDefaultRunningAction());
				m_pRuTempActor->SetDefaultBackAction(m_pRuActor->GetDefaultBackAction());
			}
		}
		UpdateTitleVisible();

		// 裝備武器
		m_controlState.substituteTakeWeapon = takeWeapon;

		m_tempScale = min(max(imageDB->Image.ModelSize / 100.0f, 0.1f), 100.0f);

		// 去除時間變化
		ClearController(eSpriteController_Scale);

		// 控制時間變化
		AppnedScale(m_tempScale * m_appnedScale);

		// 更新頭像框
		if ( s_procFunc )
			s_procFunc(this, eSpriteMsg_Portrait_Changed, 0, 0);

		//SetDefaultIdleAction(ruFUSION_ACTORSTATE_NORMAL);
		//SetActorState(ruFUSION_ACTORSTATE_NORMAL, 1);
		 return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
bool CRoleSprite::LoadRuTempEntity(const char* actName)
{
	if ( IBaseSprite::LoadRuTempEntity(actName) )
	{
		ruSAFE_RELEASE( m_pRuTempMimeEntity );
		ruSAFE_RELEASE( m_pRuTempActor );
		m_pRuTempMimeEntity = ruNEW CRuFusion_Mime();
		m_pRuTempMimeEntity->SetMimeTemplate(s_pRuMimeTemplate);
		m_pRuTempMimeEntity->SetTarget(m_pRuTempEntity);

		m_pRuTempActor = ruNEW CRuFusion_Actor();
		m_pRuTempActor->SetTargetMime(m_pRuTempMimeEntity);
		m_pRuTempActor->SetCollisionObject(this->GetCollisionContainer());
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
void CRoleSprite::ReleaseTempEntity()
{
	if ( m_pRuTempActor && m_tempScale != 0.0f )
	{
		// 去除時間變化
		ClearController(eSpriteController_Scale);

		// 回復正常比例
		AppnedScale(m_appnedScale / m_tempScale);

		m_tempScale = 0.0f;
	}

	// 武器
	m_controlState.substituteTakeWeapon = false;

	// 座騎
	if ( m_mountSprite )
	{
		m_mountSprite->GetRuActor()->UnlinkActor(m_pRuTempActor);
		m_mountSprite->GetRuActor()->LinkActor(m_pRuActor);
	}	

	// 更換附加物件
	TransAttachSprite(m_pRuEntity);
	ruSAFE_RELEASE(m_pRuTempActor);
	ruSAFE_RELEASE(m_pRuTempMimeEntity);	
	IBaseSprite::ReleaseTempEntity();	

	UpdateTitleVisible();

	// 更新頭像框
	if ( s_procFunc )
		s_procFunc(this, eSpriteMsg_Portrait_Changed, 0, 0);
}

//-----------------------------------------------------------------------------
void CRoleSprite::CreateFusionActor()
{
	ReleasFusionActor();
	if ( m_pRuActor == NULL )
	{
		m_pRuActor = ruNEW CRuFusion_Actor();
		m_pRuActor->SetTargetMime(m_pRuMimeEntity);
		m_pRuActor->SetCollisionObject(this->GetCollisionContainer());
		m_pRuActor->EnableTorsoTurn(false);

		// 浮在水上
		SetAboveWaterState(m_aboveWater);

		// 連結中,重新建立連結
		CRoleSprite* sprite = GetProxyControlSprite();
		if ( sprite->GetRuActor() && sprite->GetRuActor() != m_pRuActor )
		{
			sprite->GetRuActor()->LinkActor(m_pRuActor);
		}
	}
}

//-----------------------------------------------------------------------------
void CRoleSprite::ReleasFusionActor()
{
	if ( m_pRuActor )
	{
		CRoleSprite* sprite = GetProxyControlSprite();
		if ( sprite && sprite->GetRuActor() )
		{
			sprite->GetRuActor()->UnlinkActor(m_pRuActor);
		}
	}

	ruSAFE_RELEASE(m_pRuActor);
}

//-----------------------------------------------------------------------------
void CRoleSprite::CreateFusionMime()
{
	ReleasFusionMime();
	if ( m_pRuMimeEntity == NULL )
	{
		m_pRuMimeEntity = ruNEW CRuFusion_Mime();
		m_pRuMimeEntity->SetMimeTemplate(s_pRuMimeTemplate);
		m_pRuMimeEntity->SetTarget(m_pRuEntity);

		CRuGUID eventHandlerGUID;
		m_pRuMimeEntity->Event_DequeueAction().RegisterHandler(eventHandlerGUID, boost::bind(&CRoleSprite::HandleEvent_DequeueAction, this, _1));
	}
}

//-----------------------------------------------------------------------------
void CRoleSprite::ReleasFusionMime()
{
	ruSAFE_RELEASE(m_pRuMimeEntity);
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetPos( CRuVector3 &pos ) 
{	
	SetMoveType(eRoleSpriteMove_Normal);
	ClearMoveVector();
	m_targetPos	= pos;
	m_nextTargetPos = m_targetPos;
	IBaseSprite::SetPos(pos);
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetMoveSpeed(float speed)
{
	if ( m_moveSpeed == speed )
		return;
	IBaseSprite::SetMoveSpeed(speed);
	if ( m_moveTime > 0.1f && m_moveTime < NEXT_MOVETIME )
		SetTargetPos(m_targetPos);	
}

//-----------------------------------------------------------------------------
float CRoleSprite::GetMoveSpeed()
{
	if ( m_implementSprite )
		return m_implementSprite->GetMoveSpeed();
	return m_moveSpeed;
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetDirection(CRuVector3 direction, bool IsForce, bool IsInitial)
{
	if ( GetDisableRotate() && IsForce == false )
		return;	

	if ( IsSame(direction.m_x, 0.0f) && IsSame(direction.m_z, 0.0f) && IsSame(direction.m_y, 0.0f) )
		return;

	//if (IsSame(direction.m_x, 0.0f) && IsSame(direction.m_z, 0.0f))
	//	return;
	//direction.m_y = 0.0f;

	if ( m_moveType != eRoleSpriteMove_Normal )
		return;	

	if ( m_direction != direction.Normalize() || IsForce )
	{
		m_direction = direction;

		if ( m_pRuActor )
		{
			BOOL requiresRebasing = m_rebaseDirection && (DotProduct(CRuVector3(0.0f, 1.0f, 0.0f), this->GetOrientingAxis()) < (1.0f - ruEPSILON));

			m_pRuActor->SetEnableRebasingOnUpVector(m_rebaseDirection);
			m_pRuActor->SetUpVector(this->GetOrientingAxis());
			m_pRuActor->SetForwardVector(m_direction, IsInitial);
			if ( m_pRuTempActor )
			{
				m_pRuTempActor->SetEnableRebasingOnUpVector(m_rebaseDirection);
				m_pRuTempActor->SetUpVector(this->GetOrientingAxis());
				m_pRuTempActor->SetForwardVector(m_direction, IsInitial);
			}
		}
		else
			IBaseSprite::SetRuEntityDirection(m_pRuEntity, m_direction, m_scale * m_appnedScale);
	}
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetForwardVector(CRuVector3& forwardVector)
{
	if ( m_pRuActor )
		m_pRuActor->SetForwardVector(forwardVector, true);
}

//-----------------------------------------------------------------------------
void CRoleSprite::UpdateOrientation()
{
	if ( m_pRuActor )
	{
		m_pRuActor->SetEnableRebasingOnUpVector(m_rebaseDirection);
		m_pRuActor->SetUpVector(this->GetOrientingAxis());
		m_pRuActor->SetForwardVector(m_direction, (m_attachParentSprite) ? TRUE : FALSE);
		if ( m_pRuTempActor )
		{
			m_pRuTempActor->SetEnableRebasingOnUpVector(m_rebaseDirection);
			m_pRuTempActor->SetUpVector(this->GetOrientingAxis());
			m_pRuTempActor->SetForwardVector(m_direction, (m_attachParentSprite) ? TRUE : FALSE);
		}
	}
}

//-----------------------------------------------------------------------------
CRuEntity* CRoleSprite::FindLinkEntity( int point )
{
	return IBaseSprite::FindLinkEntity(point);
}

//-----------------------------------------------------------------------------
bool CRoleSprite::GetAttachPos(int point, CRuVector3& pos)
{	
	return IBaseSprite::GetAttachPos(point, pos);
}

//-----------------------------------------------------------------------------
bool CRoleSprite::GetShootAttachPos(CRuVector3& pos)
{
	if ( m_handsEntity[eRoleHands_Ranged].type == eRoleHands_Ranged && m_handsEntity[eRoleHands_Ranged].entity != NULL )
	{
		// Find attachment link frame
		CRuEntity *attachmentFrame = RuEntity_FindLinkFrame( m_handsEntity[eRoleHands_Ranged].entity, ATTACH_POINT_SHOOT, TRUE);

		if(attachmentFrame)
		{
			pos = attachmentFrame->GetTranslation(ruFRAMETYPE_WORLD);
			return true;
		}
	}

	return false;
}

/*
//-----------------------------------------------------------------------------
bool CRoleSprite::CreateFloorEntity(const char* actName)
{
	if ( m_mountSprite )		
		return m_mountSprite->CreateFloorEntity(actName);
	else
		return IBaseSprite::CreateFloorEntity(actName);
}

//-----------------------------------------------------------------------------
void CRoleSprite::ReleaseFloorEntity()
{
	IBaseSprite::ReleaseFloorEntity();

	CRoleSprite* spriteProxy = GetProxyControlSprite();
	if ( spriteProxy && spriteProxy != this )
		return spriteProxy->ReleaseFloorEntity();
	else
		return IBaseSprite::ReleaseFloorEntity();
}
*/


//-----------------------------------------------------------------------------
void CRoleSprite::DelRefCount(bool immediately)
{
	m_refCount--;
	if ( m_refCount <= 0 )
	{
		GameObjDbStruct* itemDB = CNetGlobal::GetObj(m_objectId);
		if ( itemDB && itemDB->Classification == EM_ObjectClass_NPC && itemDB->NPC.ClientDelNoWaitting )
		{
			immediately = true;
		}

		if ( immediately )
		{
			SetDestructTime(0.0f);
		}
		else
		{
			SetFade(true, 2.0f);
			if ( m_isDestruct == false )
				SetDestructTime(2.0f);
		}
	}
}

//-----------------------------------------------------------------------------
CRoleSprite *CRoleSprite::GetProxyControlSprite()
{
	// If this sprite has a link parent, and controls are also linked, use the parent sprite
	if(m_linkParent && m_linkParent->GetClassType() == eSpriteClass_Role && this->SpriteControlsAreLinked())
	{
		return static_cast<CRoleSprite *>(m_linkParent)->GetProxyControlSprite();
	}	
	return this;
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetOrientMode(CRuFusion_Actor::OrientMode orientMode)
{
	if(m_pRuActor)
	{
		m_pRuActor->SetOrientMode(orientMode);
	}
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetMoveVector( CRuVector3& vec, float time, bool isInternal, bool boTargetMove )
{
	if ( GetDeadState() )
		return;

	if ( m_moveType != eRoleSpriteMove_Normal )
		return;	

	switch ( GetActorState(1) )
	{
	case ruFUSION_ACTORSTATE_MINING_BEGIN:
	case ruFUSION_ACTORSTATE_MINING_LOOP:
		SetActorState(ruFUSION_ACTORSTATE_MINING_END, 1);
		break;
	case ruFUSION_ACTORSTATE_LUMBERJACK_BEGIN:
	case ruFUSION_ACTORSTATE_LUMBERJACK_LOOP:
		SetActorState(ruFUSION_ACTORSTATE_LUMBERJACK_END, 1);
		break;
	case ruFUSION_ACTORSTATE_GATHER_BEGIN:
	case ruFUSION_ACTORSTATE_GATHER_LOOP:
		SetActorState(ruFUSION_ACTORSTATE_GATHER_END, 1);
		break;
	case ruFUSION_ACTORSTATE_SIT_CHAIR_BEGIN:
	case ruFUSION_ACTORSTATE_SIT_CHAIR_LOOP:
		SetSitChairState(false);
		break;
	}
	
	m_boTargetMove = boTargetMove;
	m_moveVector = vec;
	m_moveTime = time;
	/*
	if ( !isInternal && (m_moveVector.m_x != 0.0f || m_moveVector.m_z != 0.0f) )
	{		
		float speed = m_moveVector.Magnitude() / 2;
		if ( m_moveVector.m_x > speed )
			m_moveVector.m_x = speed * 2.0f;
		else if ( m_moveVector.m_x < -speed )
			m_moveVector.m_x = -speed * 2.0f;
		else
			m_moveVector.m_x = 0.0f;

		if ( m_moveVector.m_z > speed )
			m_moveVector.m_z = speed * 2.0f;
		else if ( m_moveVector.m_z < -speed )
			m_moveVector.m_z = -speed * 2.0f;
		else
			m_moveVector.m_z = 0.0f;

		if ( m_moveVector.m_x != 0.0f && m_moveVector.m_z != 0.0f )
		{
			m_moveVector.m_x /= 1.41421356f;
			m_moveVector.m_z /= 1.41421356f;
		}
	}
	*/
}

//-----------------------------------------------------------------------------
void CRoleSprite::ClearMoveVector()
{
	m_moveVector.m_x = 0.0f;
	m_moveVector.m_y = 0.0f;
	m_moveVector.m_z = 0.0f;
	m_moveTime = 0.0f;
	m_boTargetMove = false;
}

//-----------------------------------------------------------------------------
bool CRoleSprite::Jump()
{
	if ( GetDeadState() || m_implementSprite || IsImplement() )
		return false;

	if ( GetSitChairState() )
		SetSitChairState(false);
	
	if ( m_pRuActor )
	{
		if ( GetActorState(1) != ruFUSION_ACTORSTATE_NORMAL )
		{
			return false;
		}

		switch ( GetActorState(3) )
		{
		case ruFUSION_ACTORSTATE_EMOTE_BREAKDANCE:
		case ruFUSION_ACTORSTATE_EMOTE_TAPDANCE:
			return false;
		}
	}

	if ( m_pRuActor )
		m_pRuActor->Jump(JUMP_RISINGIMPULSE * m_risingimpulse); 

	if ( m_pRuTempActor )
		m_pRuTempActor->Jump(JUMP_RISINGIMPULSE * m_risingimpulse);

	return true;
}

//-----------------------------------------------------------------------------
void CRoleSprite::JumpEx()
{
	if ( m_controlState.isFloat || m_implementSprite || IsImplement() )
		return;

	m_floatTime = 0.0f;
	m_controlState.isFloat = true;

	if ( GetObjectType() != eSpriteObject_Player )
	{
		if ( m_pRuActor )
			m_pRuActor->SetMovementType(ruSOJMOVETYPE_FULL);
		if ( m_pRuTempActor )
			m_pRuTempActor->SetMovementType(ruSOJMOVETYPE_FULL);
	}

	// 清除移動向量
	if ( m_pRuActor )
	{
		m_pRuActor->SetBackgroundState(ruFUSION_ACTORSTATE_NORMAL, TRUE);
		m_pRuActor->Move(CRuVector3(0.0f, 0.0f ,0.0f));
		m_pRuActor->Jump(JUMP_RISINGIMPULSE * 3.0f);
	}
	if ( m_pRuTempActor )
	{
		m_pRuTempActor->SetBackgroundState(ruFUSION_ACTORSTATE_NORMAL, TRUE);
		m_pRuTempActor->Move(CRuVector3(0.0f, 0.0f ,0.0f));
		m_pRuTempActor->Jump(JUMP_RISINGIMPULSE * 3.0f);
	}
}

//-----------------------------------------------------------------------------
bool CRoleSprite::ForcedJump(const CRuVector3 &latentVelocity)
{	
	if ( GetDeadState() || m_implementSprite || IsImplement() )
		return false;

	if ( m_pRuActor )
		m_pRuActor->ForcedJump(JUMP_RISINGIMPULSE * m_risingimpulse, latentVelocity);
	if ( m_pRuTempActor )
		m_pRuTempActor->ForcedJump(JUMP_RISINGIMPULSE * m_risingimpulse, latentVelocity);
	return false;
}

//-----------------------------------------------------------------------------
CRuVector3 CRoleSprite::GetLatentVelocity()
{
	CRuVector3 latentVelocity;
	if ( m_pRuActor )
		latentVelocity = m_pRuActor->GetLatentVelocity();
	return latentVelocity;
}

//-----------------------------------------------------------------------------
void CRoleSprite::ChargeToPos(CRuVector3& pos)
{
	if ( GetDeadState() )
		return;	

	// 重新定位
	if ( s_pRuTerrain )
	{
		CRuVector3 position = pos;

		position.m_y = RuEntity_FindRestingHeight(position, s_pRuTerrain, 100.0f);
		
		// 誤差在10單位量內
		if ( abs(pos.m_y - position.m_y) < 100.0f )
			pos.m_y = position.m_y;
	}

	CRuVector3 moveVector(pos.m_x - m_nowPos.m_x, pos.m_y - m_nowPos.m_y, pos.m_z - m_nowPos.m_z);
	float speed = 150.0f;

	// 朝正Z軸移動
	SetDirection(moveVector);
	SetMoveVector(CRuVector3(0.0f, 0.0f, speed), moveVector.Magnitude()/speed);

	// 暫時關閉碰撞
	if ( m_pRuActor )
	{
		// 設定上下測試範圍
		m_pRuActor->GetSojourner()->SetRestingHeightSearchLimits(25.0f, 50.0f);

		// 解除其他動作(主要是跳,會影響到角色移動量)
		m_pRuActor->SetBackgroundState(ruFUSION_ACTORSTATE_NORMAL, TRUE);

		m_pRuActor->SetDefaultRunningAction(ruFUSION_MIME_RUN_CHARGE2);
		m_pRuActor->SetMovementType(ruSOJMOVETYPE_NPC);
	}

	SetMoveType(eRoleSpriteMove_Charge);
	m_targetPos	= pos;
	m_nextTargetPos = m_targetPos;
}

//-----------------------------------------------------------------------------
void CRoleSprite::StrikeBack(CRuVector3& pos)
{
	if ( GetDeadState() )
		return;	

	// 重新定位
	if ( s_pRuTerrain )
	{
		CRuVector3 position = pos;

		position.m_y = RuEntity_FindRestingHeight(position, s_pRuTerrain, 50.0f);

		// 誤差在10單位量內
		if ( abs(pos.m_y - position.m_y) < 50.0f )
			pos.m_y = position.m_y;
	}

	CRuVector3 newVector(pos.m_x - m_nowPos.m_x, 0.0f, pos.m_z - m_nowPos.m_z);
	float speed = 80.0f;

	// 移動到達時間
	float moveTime = newVector.Magnitude() / speed;
	if ( moveTime < 0.05f )
		return;

	CRuVector3 moveVector = newVector.Normalize() * speed;
	CRuVector3 forward = m_direction;
	forward.m_y = 0.0f;

	// 將目標向量轉換為角色Z軸向量
	CRuQuaternion quaternion;
	quaternion.RotateIntoVector(CRuVector3(0.0f, 0.0f, 1.0f), forward);
	quaternion.TransformVector(moveVector);

	SetMoveVector(moveVector, moveTime);
	SetActorState(ruFUSION_ACTORSTATE_HURT_NORMAL);
	SetMoveType(eRoleSpriteMove_StrikeBack);

	m_targetPos	= pos;
	m_nextTargetPos = m_targetPos;
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetTargetPos(CRuVector3& pos, bool isForward)
{
	/*
	if ( m_objectId == 102297 )
	{
		g_pError->AddMessage(0, 0, "SetTargetPos (%.2f, %.2f, %.2f)", pos.m_x, pos.m_y, pos.m_z);
	}
	*/

	// 如果在載具上無法使用點移動
	if ( m_implementSprite || IsImplement() || m_attachParentSprite )
		return;

	// 若特殊移動中,保留最後一次目標座標
	if ( m_moveType != eRoleSpriteMove_Normal )
	{
		m_nextTargetPos = pos;
		return;
	}

	// 取得當前速度
	float speed = GetMoveSpeed();	
	if ( GetWalkState() )
		speed /= WALK_SLOW_SCALE;

	// 解除坐下
	if ( GetSitChairState() )
	{
		SetSitChairState(false);
	}

	// 重力模式
	if ( GetGravityMode() )
	{
		CRuVector3 moveVector;
		moveVector.m_x = pos.m_x - m_nowPos.m_x;
		moveVector.m_y = pos.m_y - m_nowPos.m_y;
		moveVector.m_z = pos.m_z - m_nowPos.m_z;

		// 太接近不做移動
		if ( moveVector.m_x > -0.1f && moveVector.m_x < 0.1f && moveVector.m_z > -0.1f && moveVector.m_z < 0.1f )
		{
			SetDirection(m_correctDir, true);
			return;
		}		

		// 朝正Z軸移動
		if ( isForward && GetDisableRotate() == false )
		{
			SetDirection(moveVector, true, true);
			SetMoveVector(CRuVector3(0.0f, 0.0f, speed), moveVector.Magnitude()/speed);
		}
		else
		{
			CRuQuaternion quaternion;
			float time = moveVector.Magnitude() / speed;
			moveVector = moveVector / time;
			quaternion.RotateIntoVector(CRuVector3(0.0f, 0.0f, 1.0f), m_direction);
			quaternion.TransformVector(moveVector);
			SetMoveVector(moveVector, time);
		}
	}
	// 無重力模式
	else
	{
		CRuVector3 moveVector;
		moveVector.m_x = pos.m_x - m_nowPos.m_x;
		moveVector.m_y = pos.m_y - m_nowPos.m_y;
		moveVector.m_z = pos.m_z - m_nowPos.m_z;

		// 太接近不做移動
		if ( moveVector.m_x > -0.1f && moveVector.m_x < 0.1f && moveVector.m_y > -0.1f && moveVector.m_y < 0.1f && moveVector.m_z > -0.1f && moveVector.m_z < 0.1f )
		{
			//SetDirection(m_correctDir, true);
			return;
		}		

		float moveTime = moveVector.Magnitude() / speed;
		moveVector = moveVector.Normalize() * speed;

		// 3D 軸向移動
		CRuVector3 direction = moveVector;
		CRuVector3 oldDirection = m_direction;
		direction.Normalize();

		if ( !m_isHorizontal )
		{			
			CRuVector3 rightVector = CrossProduct(direction, CRuVector3(0.0f, 1.0f, 0.0f));
			CRuVector3 upVector;
			if( rightVector.Magnitude() > ruEPSILON )
			{
				rightVector.Normalize();

				upVector = CrossProduct(rightVector, direction);

				if ( upVector.Magnitude() > ruEPSILON )
				{
					upVector.Normalize();
					SetOrientingAxis(upVector, true);
				}
				else
				{
					SetOrientingAxis(CRuVector3(0.0f, 1.0f, 0.0f), true);
				}
			}

			/*
			CRuQuaternion quaternion[3];
			quaternion[0].RotateIntoVector(upVector, CRuVector3(0.0f, 1.0f, 0.0f));

			CRuVector3 forward(0.0f, 0.0f, -1.0f);
			quaternion[0].TransformVector(forward, forward);
			quaternion[1].RotateIntoVector(direction, forward, upVector);
			quaternion[2] = quaternion[0] * quaternion[1];			

			m_pRuEntity->Rotate(quaternion[2], ruTRANSFORMOP_SET);
			*/
		}

		SetDirection(moveVector, true, true);
		SetMoveVector(moveVector, moveTime);
	}

	if ( m_pRuWidgetLine[0] )
	{
		CRuVector3 src, dest;
		src = m_nowPos;
		dest = pos;
		src.m_y += 10;
		dest.m_y += 10;
		if ( src != dest )
		{
			m_pRuWidgetLine[0]->SetLine(src, dest, FALSE, TRUE, 0xFF00FF00);
			m_pRuWidgetLine[0]->SetOption(ruENTITYOPTION_BILLBOARD, ruBILLBOARDTYPE_AXIAL);
		}

		src = m_targetPos;
		src.m_y += 10;
		if ( src != dest )
		{
			m_pRuWidgetLine[1]->SetLine(src, dest, FALSE, TRUE, 0xFFFF0000);
			m_pRuWidgetLine[1]->SetOption(ruENTITYOPTION_BILLBOARD, ruBILLBOARDTYPE_AXIAL);
		}

		src = m_lastTargetPos;
		dest = m_targetPos;
		src.m_y += 10;
		dest.m_y += 10;
		if ( src != dest )
		{
			m_pRuWidgetLine[2]->SetLine(src, dest, FALSE, TRUE, 0xFFFFFF00);
			m_pRuWidgetLine[2]->SetOption(ruENTITYOPTION_BILLBOARD, ruBILLBOARDTYPE_AXIAL);
		}
	}
	m_lastTargetPos		= m_nowPos;
	m_targetPos			= pos;
	m_nextTargetPos		= m_targetPos;
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetCorrectDir(CRuVector3 dir)
{
	dir.m_y = 0.0f;
	if ( dir.m_x != 0.0f || dir.m_z != 0.0f )
		dir.Normalize();
	m_correctDir = dir;
}

//-----------------------------------------------------------------------------
void CRoleSprite::EnableTorsoTurn(bool enableTorsoTurn)
{
	if(m_pRuActor)
	{
		m_pRuActor->EnableTorsoTurn(enableTorsoTurn ? TRUE : FALSE);
	}
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::SetFade(bool isFadeOut, float durativeTime)
{
	if (m_pRuEntity == NULL)
		return;

	if (m_fadeOut != isFadeOut || m_pRuGlobalAlpha == NULL)
	{
		m_fadeOut = isFadeOut;
		if (m_pRuGlobalAlpha == NULL)
			m_pRuGlobalAlpha = ruNEW CRuController_GlobalAlpha;

		CRuParameterBlock *parameterBlock = m_pRuEntity->GetParameterBlock();
		UINT64 alphaValue;
		parameterBlock->GetParameter(ruPARAMTYPE_MATERIAL_ALPHA, alphaValue);

		m_pRuGlobalAlpha->SetTarget(m_pRuEntity);
		m_pRuGlobalAlpha->SetDuration(durativeTime);
		if (m_fadeOut)
		{
			m_pRuGlobalAlpha->SetStartAlpha(RuCastINT64ToFloat(alphaValue));
			m_pRuGlobalAlpha->SetEndAlpha(0.0f);
		}
		else
		{
			m_pRuGlobalAlpha->SetStartAlpha(RuCastINT64ToFloat(alphaValue));
			m_pRuGlobalAlpha->SetEndAlpha(m_alpha);
		}
	}
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::SetGravityMode(bool state)
{
	float gravityAcceleration;
	if ( state )
		gravityAcceleration = -150 / m_gravityAcceleration;
	else
		gravityAcceleration = 0.0f;

	if ( m_pRuActor )
		m_pRuActor->SetGravityAcceleration( gravityAcceleration );
	m_controlState.gravityState = state;
}

//-----------------------------------------------------------------------------
CRuVector3 CRoleSprite::ComputeMoveVector(float forwardDelta, float rightDelta)
{
	CRuVector3 moveVector;
	float speed = GetMoveSpeed();

	if ( IsImplement() )
	{		
		if ( forwardDelta < 0.0f )
			moveVector.m_z = -speed / 2.0f;
		else  if ( forwardDelta > 0.0f )		
			moveVector.m_z = speed;

		if ( rightDelta < 0.0f )
			moveVector.m_x = -speed;
		else  if ( rightDelta > 0.0f )
			moveVector.m_x = speed;
	}
	else
	{
		if ( GetWalkState() )
			speed /= WALK_SLOW_SCALE;

		if ( forwardDelta != 0.0f && rightDelta != 0.0f )
			speed /= 1.41421356f;

		if ( forwardDelta > 0.0f )
			moveVector.m_z = speed;
		else if ( forwardDelta < 0.0f )
		{
			if ( GetWalkState() )
				speed /= WALK_BACK_SCALE;
			else
				speed /= RUN_BACK_SCALE;
			moveVector.m_z = -speed;
		}

		if ( rightDelta > 0.0f )
			moveVector.m_x = speed;
		else if ( rightDelta < 0.0f )
			moveVector.m_x = -speed;
	}
	return moveVector;
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::SetNameVisible(bool isShow)
{
	if ( m_controlState.nameVisible != isShow )
	{
		m_controlState.nameVisible = isShow;
		UpdateTitleVisible();
	}
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::SetTitleVisible(bool isShow)
{
	if ( m_controlState.titleVisible != isShow )
	{
		m_controlState.titleVisible = isShow;
		m_isNeedResetWidget = true;
	}
	//SetWidgetTexture();
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::SetGuildVisible(bool isShow)
{
	m_controlState.guildVisible = isShow;
	m_isNeedResetWidget = true;
	//SetWidgetTexture();
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::SetNameVisibleForce(bool isShow)
{
	if ( m_controlState.nameVisibleForce != isShow )
	{
		m_controlState.nameVisibleForce = isShow;
		m_isNeedResetWidget = true;
		UpdateTitleVisible();
	}
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::SetTitleIconVisible(bool isShow)
{
	m_controlState.titleIconVisible = isShow;
	m_isNeedResetWidget = true;
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::SetObjectAttackState(bool isState)
{
	m_controlState.attack = isState;
	m_isNeedResetWidget = true;
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::SetCanAttackState(bool isState)
{
	m_controlState.canAttack = isState;
	m_isNeedResetWidget = true;
}

//-------------------------------------------------------------------------------------------------
bool CRoleSprite::GetCanAttackState()
{
	if ( m_masterSprite )
	{
		return m_masterSprite->GetCanAttackState();
	}

	if ( m_controlState.attack == false )
		return false;
	if ( m_controlState.duel )
		return true;
	if ( IsInBattleGround() == false && (m_controlState.party || m_controlState.raid) )
		return false;
	if ( GetProtectBuffState() )
		return false;
	return /*m_controlState.canAttack ||*/ m_controlState.campAttack || m_controlState.pvpZone || m_controlState.pvpOpen || m_controlState.pvpGuild;
}

//-------------------------------------------------------------------------------------------------
bool CRoleSprite::GetEnemyState()
{
	if ( m_masterSprite )
	{
		return m_masterSprite->GetEnemyState();
	}

	if ( m_controlState.duel )
		return true;
	if ( IsInBattleGround() == false && (m_controlState.party || m_controlState.raid) )
		return false;
	if ( GetProtectBuffState() )
		return false;
	return /*m_controlState.enemy ||*/ m_controlState.campEnemy || m_controlState.pvpZone || m_controlState.pvpOpen || m_controlState.pvpGuild;
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::SetEnemyState(bool isState)
{
	m_controlState.enemy = isState;
	m_isNeedResetWidget = true;
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::SetDuelState(bool state)
{
	m_controlState.duel = state;
	m_isNeedResetWidget = true;
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::SetPVPZoneState(bool state)
{
	m_controlState.pvpZone = state;
	m_isNeedResetWidget = true;
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::SetPVPGuildState(bool state)
{
	m_controlState.pvpGuild = state;
	m_isNeedResetWidget = true;
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::SetCampAttackState(bool state)
{
	m_controlState.campAttack = state;
	m_isNeedResetWidget = true;
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::SetCampEnemyState(bool state)
{
	m_controlState.campEnemy = state;
	m_isNeedResetWidget = true;
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::SetPVPState(bool state)
{
	m_controlState.pvpOpen = state;
	m_isNeedResetWidget = true;
}

//-------------------------------------------------------------------------------------------------
bool CRoleSprite::GetPKBuffState()
{
	return (m_specialEffect.pvp > 0);
}

//-------------------------------------------------------------------------------------------------
bool CRoleSprite::GetCriminalBuffState()
{
	return (m_specialEffect.criminal > 0);
}

//-------------------------------------------------------------------------------------------------
bool CRoleSprite::GetProtectBuffState()
{
	return (m_specialEffect.protect > 0);
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::SetDisableQuestState(bool isState)
{
	if ( m_controlState.DisableQuest != isState )
	{
		m_controlState.DisableQuest = isState;
		UpdateTitleVisible();
	}	
}

//-------------------------------------------------------------------------------------------------
int CRoleSprite::GetCursorMode()
{
	if ( GetObjectType() == eSpriteObject_Player )
		return 0;

	return m_cursorMode;
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::SetAlwaysHoldWeapon(bool state)
{
	m_controlState.alwaysHoldWeapon = state;
	if ( m_controlState.alwaysHoldWeapon )
		SetWeaponPosition(true, true);
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::SetForceCombatMotion(bool state)
{
	m_controlState.forceCombatMotion = state;
	SetCombatMotion();
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::SetPartyState(bool inParty)
{
	m_controlState.party = inParty;
	m_isNeedResetWidget = true;
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::SetRaidState(bool inRaid)
{
	m_controlState.raid = inRaid;
	m_isNeedResetWidget = true;
}

//-------------------------------------------------------------------------------------------------
bool CRoleSprite::GetMoveControl()
{
	if ( m_specialEffect.lockedMove > 0 || m_specialEffect.fear > 0 )
		return false;

	// 是否在載具上
	if ( m_implementSprite )
	{
		if ( m_implementControlMove == false )
			return false;

		return m_implementSprite->GetMoveControl();
	}
	return true;
}

//-------------------------------------------------------------------------------------------------
const char* CRoleSprite::GetSoundWeaponType(bool offHand)
{
	GameObjDbStruct* pObj = NULL;

	// 副手
	if ( offHand )
	{
		switch ( GetTakeItemType(m_handsEntity[1].id) )
		{
		case eRoleTakeItem_Unarmed:
			return SOUND_WEAPON_UNARMED;

		case eRoleTakeItem_Dagger:
			return SOUND_WEAPON_DAGGER;

		case eRoleTakeItem_Blade:
			return SOUND_WEAPON_BLADE;

		case eRoleTakeItem_Wand:
			return SOUND_WEAPON_CLUB;

		case eRoleTakeItem_Axe:
			return SOUND_WEAPON_AXE;

		case eRoleTakeItem_Bludgeon:
			return SOUND_WEAPON_HAMMER;
		}
	}
	else
	{
		switch ( GetTakeItemType(m_handsEntity[0].id) )
		{
		case eRoleTakeItem_Unarmed:
			return SOUND_WEAPON_UNARMED;

		case eRoleTakeItem_Dagger:
			return SOUND_WEAPON_DAGGER;

		case eRoleTakeItem_Blade:
			return SOUND_WEAPON_BLADE;

		case eRoleTakeItem_Wand:
			return SOUND_WEAPON_CLUB;

		case eRoleTakeItem_Axe:
			return SOUND_WEAPON_AXE;

		case eRoleTakeItem_Bludgeon:
			return SOUND_WEAPON_HAMMER;

		case eRoleTakeItem_Claymore:
			return SOUND_WEAPON_2H_BLADE;

		case eRoleTakeItem_2H_Axe:
			return SOUND_WEAPON_2H_AXE;

		case eRoleTakeItem_2H_Hammer:
			return SOUND_WEAPON_2H_HAMMER;

		case eRoleTakeItem_Staff:
			return SOUND_WEAPON_STAFF;

		case eRoleTakeItem_Polearm:
			return SOUND_WEAPON_2H_BLADE;
		}
	}

	pObj = CNetGlobal::GetObj(m_objectId);
	if ( pObj && pObj->Classification == EM_ObjectClass_NPC )
	{
		switch ( pObj->NPC.OrgWeaponType )
		{
		case EM_Weapon_Unarmed:
			return SOUND_WEAPON_UNARMED;

		case EM_Weapon_Blade:
			return SOUND_WEAPON_BLADE;

		case EM_Weapon_Dagger:
			return SOUND_WEAPON_DAGGER;

		case EM_Weapon_Wand:
			return SOUND_WEAPON_CLUB;

		case EM_Weapon_Axe:
			return SOUND_WEAPON_AXE;

		case EM_Weapon_Bludgeon:
			return SOUND_WEAPON_HAMMER;

		case EM_Weapon_Claymore:
			return SOUND_WEAPON_2H_BLADE;

		case EM_Weapon_Staff:
			return SOUND_WEAPON_STAFF;

		case EM_Weapon_2H_Axe:
			return SOUND_WEAPON_2H_AXE;

		case EM_Weapon_2H_Hammer:
			return SOUND_WEAPON_2H_HAMMER;

		case EM_Weapon_Polearm:
			return SOUND_WEAPON_2H_BLADE;
		}
	}
	return SOUND_WEAPON_UNARMED;
}

//-------------------------------------------------------------------------------------------------
const char* CRoleSprite::GetSoundArmorType()
{
	GameObjDbStruct* pObj = NULL;

	switch (m_objectType)
	{
	case eSpriteObject_Player:
		pObj = CNetGlobal::GetObj(m_torso);
		if ( pObj && pObj->Classification == EM_ObjectClass_Item )
		{
			if ( pObj->Item.Class == EM_Item_Armor )
			{
				switch (pObj->Item.ArmorType)
				{
				case EM_Armor_HeavyArmed:
					return SOUND_ARMOR_PLATE;

				case EM_Armor_LightArmed:
					return SOUND_ARMOR_CHAIN;

				case EM_Armor_Leather:
				case EM_Armor_Clothes:
				case EM_Armor_Robe:
					return SOUND_ARMOR_FLESH;

				case EM_Armor_Shield:
				case EM_Armor_Wood:
				case EM_Armor_Stone:
				case EM_Armor_Iron:
					return SOUND_ARMOR_FLESH;
				}
			}
		}
		return SOUND_ARMOR_FLESH;

	case eSpriteObject_QuestNPC:
	case eSpriteObject_NPC:
		pObj = CNetGlobal::GetObj(m_objectId);
		if ( pObj && pObj->Classification == EM_ObjectClass_NPC )
		{
			switch (pObj->NPC.OrgArmorType)
			{
			case EM_Armor_HeavyArmed:
				return SOUND_ARMOR_PLATE;

			case EM_Armor_LightArmed:
				return SOUND_ARMOR_CHAIN;

			case EM_Armor_Leather:
			case EM_Armor_Clothes:
			case EM_Armor_Robe:
				return SOUND_ARMOR_FLESH;

			case EM_Armor_Shield:
				return SOUND_ARMOR_SHIELD;

			case EM_Armor_Stone:
				return SOUND_ARMOR_STONE;

			case EM_Armor_Wood:			
			case EM_Armor_Iron:
				return SOUND_ARMOR_WOOD;
			}
		}		
		break;
	}

	return SOUND_ARMOR_FLESH;
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::CreateCollectorFX(CRuVector3 position)
{
	if ( m_pRuEntity == NULL )
		return;

	ReleaseCollectorFX();

	m_pRuCollectorEntity = g_ruResourceManager->LoadEntity("model\\item\\ui_object\\effect_item\\act_effect_active_fx_i.ros");

	m_pRuCollectorFX = ruNEW CRuEntity_CollectorFX();
	m_pRuCollectorFX->SetEffectEntity(m_pRuCollectorEntity);
	m_pRuCollectorFX->SetEffectStartMotion("appear");
	m_pRuCollectorFX->SetEffectFlightMotion("idle");
	m_pRuCollectorFX->SetEffectEndMotion("disappear");
	m_pRuCollectorFX->SetEffectEntityGenerateCount(5);

	m_pRuCollectorFX->SetGeneratorPosition(position);
	m_pRuCollectorFX->SetGeneratorStrength(2.5f);
	m_pRuCollectorFX->SetGeneratorVelocityDamping(25.0f);
	m_pRuCollectorFX->SetPathDeviation(5.0f);
	m_pRuCollectorFX->SetCollectionSpeed(2.0f);

	RuEntity_Attach(m_pRuEntity, m_pRuCollectorFX, "p_top");
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::ReleaseCollectorFX()
{
	if(m_pRuCollectorFX)
	{
		m_pRuCollectorFX->DetachFromParent();
		ruSAFE_RELEASE(m_pRuCollectorFX);
	}
	ruSAFE_RELEASE(m_pRuCollectorEntity);
}

//-------------------------------------------------------------------------------------------------
bool CRoleSprite::CheckRenderState()
{
	if ( !IBaseSprite::CheckRenderState() )
		return false;
	if ( m_pRuEntity == NULL || RuPaperdoll_IsBuilding(m_pRuEntity) )
		return false;
	if ( m_specialEffect.invisible > 0 )
		return false;
	if ( GetSneakHiddenState() )
		return false;
	if ( GetEnemyHidden() && (GetCanAttackState() || GetEnemyState()) )
		return false;
	return true;
}
//-------------------------------------------------------------------------------------------------
bool CRoleSprite::CheckSelectState()
{
	if ( m_specialEffect.invisible > 0 )
		return false;
	if ( GetSneakHiddenState() )
		return false;
	return true;
}
//-------------------------------------------------------------------------------------------------
// 產生生產工具
// point 工具要參生的參考點位置
void CRoleSprite::AttachCraftingTool(const char* point)
{
	CreateHandsEntity(eRoleHands_Crafting, m_handsObjectId[eRoleHands_Crafting], 0, point);
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::DetachCraftingTool()
{
	ReleaseHandsEntity(eRoleHands_Crafting);
	m_handsObjectId[eRoleHands_Crafting] = 0;
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::AddController(SpriteControllerType type, float time, float begin, float end)
{
	SpriteController controller;
	controller.type		= type;
	controller.time		= time;
	controller.update	= 0.0f;
	controller.begin	= begin;
	controller.end		= end;
	m_controller.push_back(controller);
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::UpdateController(float elapseTime)
{
	vector<SpriteController>::iterator iter = m_controller.begin();
	while (iter != m_controller.end())
	{
		SpriteController& controller = *iter;
		bool isDelete = false;
		float scale;

		controller.update += elapseTime;
		if ( controller.update > controller.time )
		{
			scale = controller.end;
			isDelete = true;
		}
		else
			scale = controller.begin + (controller.end - controller.begin) * controller.update / controller.time;

		switch (controller.type)
		{
		case eSpriteController_None:
			break;

		case eSpriteController_Alpha:
			break;

		case eSpriteController_Scale:
			AppnedScale(scale);
			break;
		}

		if ( isDelete )
			iter = m_controller.erase(iter);
		else
			iter++;
	}
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::ClearController(SpriteControllerType type)
{
	// 將狀態設定為最後狀態
	vector<SpriteController>::iterator iter = m_controller.begin();
	while ( iter != m_controller.end() ) {
		if ( iter->type == type ) {
			switch (type)
			{
			case eSpriteController_None:
				break;

			case eSpriteController_Alpha:
				break;

			case eSpriteController_Scale:
				AppnedScale(iter->end);
				break;
			}
			iter = m_controller.erase(iter);
		} else {
			iter++;
		}
	}
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::CreateWidgetLine()
{
	if ( !m_pRuWidgetLine[0] )
		m_pRuWidgetLine[0] = ruNEW CRuEntity_Widget_Line;
	if ( !m_pRuWidgetLine[1] )
		m_pRuWidgetLine[1] = ruNEW CRuEntity_Widget_Line;
	if ( !m_pRuWidgetLine[2] )
		m_pRuWidgetLine[2] = ruNEW CRuEntity_Widget_Line;
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::ReleaseWidgetLine()
{
	ruSAFE_RELEASE(m_pRuWidgetLine[0]);
	ruSAFE_RELEASE(m_pRuWidgetLine[1]);
	ruSAFE_RELEASE(m_pRuWidgetLine[2]);
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::SetHandsObjectId(RoleHandsType type, int id, int level, bool hold)
{
	if ( type == eRoleHands_None || (m_handsObjectId[type] == id && m_handsLevel[type] == level) )
		return;

	m_handsObjectId[type] = id;
	m_handsLevel[type] = level;
	switch(type)
	{
	case eRoleHands_MainHand:
		if ( hold )
			CreateHandsEntity(type, id, level);
		SetCombatMotion();
		break;

	case eRoleHands_OffHand:
		if ( hold )
			CreateHandsEntity(type, id, level);
		SetCombatMotion();
		break;

	case eRoleHands_Ranged:
		if ( hold )
			CreateHandsEntity(type, id, level);
		break;

	case eRoleHands_Crafting:
		if ( hold )
			CreateHandsEntity(type, id, level);
		break;
	}

	if ( m_isNeedBuild == false && s_procFunc )
		s_procFunc(this, eSpriteMsg_Portrait_Changed, 0, 0);
}

//-------------------------------------------------------------------------------------------------
int CRoleSprite::GetHandsObjectId(RoleHandsType type)
{
	return m_handsObjectId[type];
}

//-------------------------------------------------------------------------------------------------
bool CRoleSprite::SetPaperdollPart(const char* partName, int id)
{
	/*
	*	紙娃娃設定,在身體上可見部位都需要有一個預設參數
	*  外加部位若沒有,則不需要設定
	*/

	const char* name = NULL;
	GameObjDbStruct* pObj = CNetGlobal::GetObj(id);
	GameObjDbStruct* imageDB = NULL;
	if ( pObj )
	{
		imageDB = CNetGlobal::GetObj(pObj->ImageID);
	}	

	if ( imageDB )
		name = imageDB->Image.ACTWorld;
	else
	{
		pObj = CNetGlobal::GetObj(m_objectId);
		if ( pObj && (imageDB = CNetGlobal::GetObj(pObj->ImageID)) != NULL )
		{
			if (strcmp(PAPERDOLL_PART_NAME_HEAD, partName) == 0)
				name = imageDB->Image.PaperdollFace.Name;
			else if (strcmp(PAPERDOLL_PART_NAME_HAIR, partName) == 0)
				name = imageDB->Image.PaperdollHair.Name;
			else if (strcmp(PAPERDOLL_PART_NAME_HELMET, partName) == 0)
				name = imageDB->Image.PaperdollHead.Name;
			else if (strcmp(PAPERDOLL_PART_NAME_SHOULDER, partName) == 0)
				name = imageDB->Image.PaperdollShouder.Name;
			else if (strcmp(PAPERDOLL_PART_NAME_TORSO, partName) == 0)
				name = imageDB->Image.PaperdollClothes.Name;
			else if (strcmp(PAPERDOLL_PART_NAME_HAND, partName) == 0)
				name = imageDB->Image.PaperdollGloves.Name;
			else if (strcmp(PAPERDOLL_PART_NAME_BELT, partName) == 0)
				name = imageDB->Image.PaperdollBelt.Name;
			else if (strcmp(PAPERDOLL_PART_NAME_LEG, partName) == 0)
				name = imageDB->Image.PaperdollPants.Name;
			else if (strcmp(PAPERDOLL_PART_NAME_FOOT, partName) == 0)
				name = imageDB->Image.PaperdollShoes.Name;
			else if (strcmp(PAPERDOLL_PART_NAME_BACK, partName) == 0)
				name = imageDB->Image.PaperdollBack.Name;
		}
	}
	return SetPaperdollPart(partName, name);
}

//-------------------------------------------------------------------------------------------------
bool CRoleSprite::SetPaperdollPart(const char* partName, const char* name)
{
	if ( m_pRuEntity == NULL )
		return false;

	if (name == NULL || name[0] == 0)
	{
		if (strcmp(partName, PAPERDOLL_PART_NAME_HEAD) == 0)
			name = "type01";
		else if (strcmp(partName, PAPERDOLL_PART_NAME_HAIR) == 0)
			name = "type01";
		else if (strcmp(partName, PAPERDOLL_PART_NAME_TORSO) == 0)
			name = "body000-001";
		else if (strcmp(partName, PAPERDOLL_PART_NAME_HAND) == 0)
			name = "body000-001";
		else if (strcmp(partName, PAPERDOLL_PART_NAME_LEG) == 0)
			name = "body000-001";
		else if (strcmp(partName, PAPERDOLL_PART_NAME_FOOT) == 0)
			name = "body000-001";
		else
			name = "";
	}

	// 檢查是否有不相同
	IRuPaperdoll* pRuPaperdoll = RuEntity_FindFirstPaperdoll(m_pRuEntity);
	const char* originalName = "";
	if ( pRuPaperdoll ) {
		originalName = pRuPaperdoll->GetComponent(partName);
	}
	if ( originalName == NULL )
		originalName = "";

	if ( strcmp(originalName, name) == 0 )
		return true;		
	
	m_isNeedBuild = true;
	return RuPaperdoll_SetComponent(m_pRuEntity, partName, name) == TRUE;
}

//-------------------------------------------------------------------------------------------------
const char* CRoleSprite::GetPaperdollPart(const char* partName)
{
	if ( m_pRuEntity )
	{
		IRuPaperdoll* ruPaperdoll = RuEntity_FindFirstPaperdoll(m_pRuEntity);
		if (ruPaperdoll)
			return ruPaperdoll->GetComponent(partName);
	}
	return "";
}

//-------------------------------------------------------------------------------------------------
bool CRoleSprite::SetPaperdollColor(const char* partName, int mainColor, int secondColor, bool boSetBaseColor)
{
	if ( m_pRuEntity && partName )
	{
		// 更換為物件預設值
		if ( mainColor == 0 && secondColor == 0 )
		{
			GameObjDbStruct* pObj = CNetGlobal::GetObj(m_objectId);
			if ( pObj == NULL )
				return false;

			GameObjDbStruct* imageDB = CNetGlobal::GetObj(pObj->ImageID);
			if ( imageDB == NULL )
				return false;

			if ( strcmp(PAPERDOLL_PART_NAME_HELMET, partName) == 0 )
			{
				mainColor = imageDB->Image.PaperdollHead.MainColor.Color;
				secondColor = imageDB->Image.PaperdollHead.OffColor.Color;
			} 
			else if ( strcmp(PAPERDOLL_PART_NAME_SHOULDER, partName) == 0 )
			{
				mainColor = imageDB->Image.PaperdollShouder.MainColor.Color;
				secondColor = imageDB->Image.PaperdollShouder.OffColor.Color;
			}
			else if ( strcmp(PAPERDOLL_PART_NAME_TORSO, partName) == 0 )
			{
				mainColor = imageDB->Image.PaperdollClothes.MainColor.Color;
				secondColor = imageDB->Image.PaperdollClothes.OffColor.Color;
			}
			else if ( strcmp(PAPERDOLL_PART_NAME_HAND, partName) == 0 )
			{
				mainColor = imageDB->Image.PaperdollGloves.MainColor.Color;
				secondColor = imageDB->Image.PaperdollGloves.OffColor.Color;
			}
			else if ( strcmp(PAPERDOLL_PART_NAME_BELT, partName) == 0 )
			{
				mainColor = imageDB->Image.PaperdollBelt.MainColor.Color;
				secondColor = imageDB->Image.PaperdollBelt.OffColor.Color;
			}
			else if ( strcmp(PAPERDOLL_PART_NAME_LEG, partName) == 0 )
			{
				mainColor = imageDB->Image.PaperdollPants.MainColor.Color;
				secondColor = imageDB->Image.PaperdollPants.OffColor.Color;
			}
			else if ( strcmp(PAPERDOLL_PART_NAME_FOOT, partName) == 0 )
			{
				mainColor = imageDB->Image.PaperdollShoes.MainColor.Color;
				secondColor = imageDB->Image.PaperdollShoes.OffColor.Color;
			}
			else if ( strcmp(PAPERDOLL_PART_NAME_BACK, partName) == 0 )
			{
				mainColor = imageDB->Image.PaperdollBack.MainColor.Color;
				secondColor = imageDB->Image.PaperdollBack.OffColor.Color;
			}
		}

		INT64 color = mainColor;
		color = (color << 32) | (secondColor & 0xFFFFFFFF);
		map<string, INT64>::iterator iter = m_paperdollColor.find(partName);
		if ( /*m_isNeedBuild == false &&*/ iter != m_paperdollColor.end() && iter->second == color ) {
			return true;
		}

		IRuPaperdoll* ruPaperdoll = RuEntity_FindFirstPaperdoll(m_pRuEntity);
		if ( ruPaperdoll )
		{
			const char* partArray[10] = {
				PAPERDOLL_PART_NAME_HEAD,
				PAPERDOLL_PART_NAME_HAIR,
				PAPERDOLL_PART_NAME_HELMET,
				PAPERDOLL_PART_NAME_SHOULDER,
				PAPERDOLL_PART_NAME_TORSO,
				PAPERDOLL_PART_NAME_HAND,
				PAPERDOLL_PART_NAME_BELT,
				PAPERDOLL_PART_NAME_LEG,
				PAPERDOLL_PART_NAME_FOOT,
				PAPERDOLL_PART_NAME_BACK
			};

			BOOL layerActivated[4] = { FALSE, FALSE, FALSE, FALSE };
			RUCOLOR layerColor[4] = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF };
			
			if ( strcmp(partName, PAPERDOLL_PART_NAME_HEAD) == 0 && boSetBaseColor )
			{
				for (int i = 0; i < 10; i++)
				{
					ruPaperdoll->GetComponentColors(partArray[i], layerActivated, layerColor);
					layerActivated[0]	= ((mainColor & 0xFF000000) == 0xFF000000);
					layerColor[0]		= mainColor;
					ruPaperdoll->SetComponentColors(partArray[i], layerActivated, layerColor);
				}
			}
			else if ( strcmp(partName, PAPERDOLL_PART_NAME_HAIR) == 0 )
			{
				for (int i = 0; i < 10; i++)
				{
					ruPaperdoll->GetComponentColors(partArray[i], layerActivated, layerColor);
					layerActivated[1]	= ((mainColor & 0xFF000000) == 0xFF000000);
					layerColor[1]		= mainColor;
					ruPaperdoll->SetComponentColors(partArray[i], layerActivated, layerColor);
				}
			}
			else if ( ruPaperdoll->GetComponentColors(partName, layerActivated, layerColor) )
			{
				layerActivated[2] = ((mainColor & 0xFF000000) == 0xFF000000);
				layerActivated[3] = ((secondColor & 0xFF000000) == 0xFF000000);
				layerColor[2] = mainColor;
				layerColor[3] = secondColor;
				ruPaperdoll->SetComponentColors(partName, layerActivated, layerColor);
			}

			if ( iter != m_paperdollColor.end() ) {
				iter->second = color;
			} else {
				m_paperdollColor.insert(make_pair(partName, color));
			}
			m_isNeedBuild = true;
			return true;
		}
	}
	return false;
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::SetDyeFXColors(int* colors)
{	
	if ( colors == NULL )
		return;

	m_enableDyeFXColor = true;
	memcpy(m_dyeFXColors, colors, sizeof(m_dyeFXColors));

	if ( m_proxyLoadCompleted )
	{
		UpdateDyeFXColors();
	}
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::UpdateDyeFXColors()
{
	if ( m_mountSprite )
	{
		m_mountSprite->SetDyeFXColors(m_dyeFXColors);
	}
	else
	{
		// Enumerate all materials
		CRuArrayList<IRuMaterial *> materials;
		RuEntity_EnumerateMaterials(m_pRuEntity, materials);

		for(INT32 i = 0; i < materials.Count(); ++i)
		{
			RuPropertyType propertyValue;
			BOOL dyeFXEnabled  = materials[i]->GetProperty("material:dyeFX", propertyValue) ? boost::get<BOOL>(propertyValue) : FALSE;

			// If DyeFX is enabled, build the DyeFX mask and the color map
			if(dyeFXEnabled)
			{
				BOOL layerActivated[4] = { FALSE, FALSE, FALSE, FALSE };
				RUCOLOR layerColor[4] = { 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF };

				for ( int n = 0; n < 4; n++ )
				{
					layerActivated[n] = m_dyeFXColors[n] & 0xFF000000;
					layerColor[n] = m_dyeFXColors[n];
				}

				// Build DyeFX mask
				if(RuMaterial_BuildDyeFXMask(materials[i]) == FALSE)
				{
					m_dyeFXMaskRequiresLoad = true;
				}

				// Set DyeFX colors
				RuMaterial_SetDyeFXColors(materials[i], layerActivated, layerColor);
			}
		}
	}	

	m_enableDyeFXColor = false;
}

//-------------------------------------------------------------------------------------------------
bool CRoleSprite::GetPaperdollColor(const char* partName, int* outColor)
{
	if ( m_pRuEntity )
	{
		IRuPaperdoll* ruPaperdoll = RuEntity_FindFirstPaperdoll(m_pRuEntity);
		if ( ruPaperdoll )
		{
			BOOL layerActivated[4];
			RUCOLOR layerColor[4];

			if ( ruPaperdoll->GetComponentColors(partName, layerActivated, layerColor) && outColor )
			{
				if ( strcmp(partName, PAPERDOLL_PART_NAME_HEAD) == 0 )
				{
					if ( layerActivated[0] )
						outColor[0] = layerColor[0] | 0xFF000000;
					else
						outColor[0] = 0;
				}
				else if ( strcmp(partName, PAPERDOLL_PART_NAME_HAIR) == 0 )
				{
					if ( layerActivated[1] )
						outColor[0] = layerColor[1] | 0xFF000000;
					else
						outColor[0] = 0;
				}
				else
				{
					if ( layerActivated[2] )
						outColor[0] = layerColor[2] | 0xFF000000;
					else
						outColor[0] = 0;

					if ( layerActivated[3] )
						outColor[1] = layerColor[3] | 0xFF000000;
					else
						outColor[1] = 0;
				}
				return true;
			}
		}
	}
	return false;
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::PaperdollBuild(bool allowDelayedLoad)
{
	if ( m_pRuEntity && m_isNeedBuild )
	{
		m_isNeedBuild = false;

		switch(this->GetObjectType())
		{
			default:
				RuPaperdoll_Build(m_pRuEntity, allowDelayedLoad);
				break;

			case eSpriteObject_NPC:
			case eSpriteObject_QuestNPC:
				{
					IRuPaperdoll *paperdoll = RuEntity_FindFirstPaperdoll(m_pRuEntity);

					if(paperdoll && paperdoll->GetType().IsSubClassOf(CRuPaperdoll::Type()))
					{
						if(allowDelayedLoad)
						{
							// Mark sprite as waiting for paperdoll cache resolution
							m_isWaitingForPaperdollCache = true;

							CRuModel *cachedPaperdollModel = NULL;
							if(s_paperdollCache->LoadCachedPaperdoll(m_pRuEntity->GetResourceName(), static_cast<CRuPaperdoll *>(paperdoll), &cachedPaperdollModel))
							{
								if(cachedPaperdollModel)
								{
									// Mark sprite as not waiting for paperdoll cache resolution
									m_isWaitingForPaperdollCache = false;

									// Reset the internal composite model
									static_cast<CRuPaperdoll *>(paperdoll)->GetCompositeModel()->ResetModel();

									// Remove any models that's not a composite model
									CRuEntity *curChild = paperdoll->GetFirstChild();
									CRuEntity *nextChild = NULL;
									while(curChild)
									{
										// Store pointer to next child
										nextChild = curChild->GetNextSibling();

										if(curChild->GetType().IsSubClassOf(CRuCompositeModel::Type()) == FALSE)
										{
											// Detach child
											curChild->DetachFromParent();
										}

										curChild = nextChild;
									}

									// Append cached paperdoll model
									paperdoll->AddChild(cachedPaperdollModel);
									ruSAFE_RELEASE(cachedPaperdollModel);

									// Update caster class flags
									SetCasterClass();

									// Enable alpha-Z
									RuEntity_EnableAlphaZ(m_pRuEntity);

									// Replace interior composite model with the cached model
//									RuEntity_ReplaceInteriorEntity(m_pRuEntity, paperdoll, cachedPaperdollModel);
								}
							}
							else
							{
								// Cache lookup failed, proceed with normal paperdoll build
								RuPaperdoll_Build(m_pRuEntity, allowDelayedLoad);
							}
						}
						else
						{
							// Mark sprite as not waiting for paperdoll cache resolution
							m_isWaitingForPaperdollCache = false;

							CRuModel *cachedPaperdollModel = s_paperdollCache->LoadCachedPaperdoll(m_pRuEntity->GetResourceName(), static_cast<CRuPaperdoll *>(paperdoll));
							if(cachedPaperdollModel)
							{
								// Remove any models that's not a composite model
								CRuEntity *curChild = paperdoll->GetFirstChild();
								CRuEntity *nextChild = NULL;
								while(curChild)
								{
									// Store pointer to next child
									nextChild = curChild->GetNextSibling();

									if(curChild->GetType().IsSubClassOf(CRuCompositeModel::Type()) == FALSE)
									{
										// Detach child
										curChild->DetachFromParent();
									}

									curChild = nextChild;
								}

								// Reset the internal composite model
								static_cast<CRuPaperdoll *>(paperdoll)->GetCompositeModel()->ResetModel();
								ruSAFE_RELEASE(cachedPaperdollModel);

								// Update caster class flags
								SetCasterClass();

								// Enable alpha-Z
								RuEntity_EnableAlphaZ(m_pRuEntity);

								// Replace interior composite model with the cached model
//								RuEntity_ReplaceInteriorEntity(m_pRuEntity, paperdoll, cachedPaperdollModel);
							}
							else
							{
								// Cache lookup failed, proceed with normal paperdoll build
								RuPaperdoll_Build(m_pRuEntity, allowDelayedLoad);
							}
						}
					}
				}

				break;
		}

		// 關閉任務影子
		if ( m_pRuQuestEntity )
		{
			RuEntity_SetOptionWithTypeFilter(m_pRuQuestEntity, ruENTITYOPTION_CASTERCLASS, 0, CRuModel::Type());
			RuEntity_SetOptionWithTypeFilter(m_pRuQuestEntity, ruENTITYOPTION_CASTERCLASS, 0, CRuCompositeModel::Type());
		}

		if (s_procFunc)
			s_procFunc(this, eSpriteMsg_Portrait_Changed, 0, 0);
	}
}

//-----------------------------------------------------------------------------
void CRoleSprite::PaperdollBuildCheck(bool allowDelayedLoad)
{
	if(m_isWaitingForPaperdollCache && m_pRuEntity)
	{
		// Find applicable paperdoll
		IRuPaperdoll *paperdoll = RuEntity_FindFirstPaperdoll(m_pRuEntity);

		if(paperdoll && paperdoll->GetType().IsSubClassOf(CRuPaperdoll::Type()))
		{
			CRuModel *cachedPaperdollModel = NULL;
			if(s_paperdollCache->LoadCachedPaperdoll(m_pRuEntity->GetResourceName(), static_cast<CRuPaperdoll *>(paperdoll), &cachedPaperdollModel))
			{
				if(cachedPaperdollModel)
				{
					// Mark sprite as not waiting for paperdoll cache resolution
					m_isWaitingForPaperdollCache = false;

					// Reset the internal composite model
					static_cast<CRuPaperdoll *>(paperdoll)->GetCompositeModel()->ResetModel();

					// Remove any models that's not a composite model
					CRuEntity *curChild = paperdoll->GetFirstChild();
					CRuEntity *nextChild = NULL;
					while(curChild)
					{
						// Store pointer to next child
						nextChild = curChild->GetNextSibling();

						if(curChild->GetType().IsSubClassOf(CRuCompositeModel::Type()) == FALSE)
						{
							// Detach child
							curChild->DetachFromParent();
						}

						curChild = nextChild;
					}

					// Append cached paperdoll model
					paperdoll->AddChild(cachedPaperdollModel);
					ruSAFE_RELEASE(cachedPaperdollModel);

					// Update caster class flags
					SetCasterClass();

					// Enable alpha-Z
					RuEntity_EnableAlphaZ(m_pRuEntity);

					// Replace interior composite model with the cached model
//					RuEntity_ReplaceInteriorEntity(m_pRuEntity, paperdoll, cachedPaperdollModel);
				}
			}
			else
			{
				// Mark sprite as not waiting for paperdoll cache resolution
				m_isWaitingForPaperdollCache = false;

				// Cache lookup failed, proceed with normal paperdoll build
				RuPaperdoll_Build(m_pRuEntity, allowDelayedLoad);
			}
		}
	}
}

//-----------------------------------------------------------------------------
bool CRoleSprite::SetBoneScale(const char* boneName, float scale)
{
	if(scale < 0.25f || scale > 3.0f)
	{
		scale = 1.0f;
	}
	
	if ( strcmp(boneName, BONE_SCALE_NAME_HEAD) == 0 ) {
		if ( scale < 0.85f ) scale = 0.85f;
		if ( scale > 1.2f ) scale = 1.2f;
	} else if ( strcmp(boneName, BONE_SCALE_NAME_BREASTS) == 0 ) {
		if ( scale < 0.8f ) scale = 0.8f;
		if ( scale > 1.2f ) scale = 1.2f;
	} else if ( strcmp(boneName, BONE_SCALE_NAME_BODY) == 0 ) {
		if ( scale < 0.75f ) scale = 0.75f;
		if ( scale > 1.3f ) scale = 1.3f;
	} else if ( strcmp(boneName, BONE_SCALE_NAME_CHEST) == 0 ) {
		if ( scale < 0.85f ) scale = 0.85f;
		if ( scale > 1.1f ) scale = 1.1f;
	} else if ( strcmp(boneName, BONE_SCALE_NAME_PELVIS) == 0 ) {
		if ( scale < 0.80f ) scale = 0.80f;
		if ( scale > 1.20f ) scale = 1.2f;
	} else if ( strcmp(boneName, BONE_SCALE_NAME_ARM) == 0 ) {
		if ( scale < 0.80f ) scale = 0.80f;
		if ( scale > 1.50f ) scale = 1.50f;
	} else if ( strcmp(boneName, BONE_SCALE_NAME_FOREARM) == 0 ) {
		if ( scale < 0.80f ) scale = 0.80f;
		if ( scale > 1.30f ) scale = 1.30f;
	} else if ( strcmp(boneName, BONE_SCALE_NAME_HAND) == 0 ) {
		if ( scale < 0.70f ) scale = 0.70f;
		if ( scale > 1.30f ) scale = 1.30f;
	} else if ( strcmp(boneName, BONE_SCALE_NAME_LEGS) == 0 ) {
		if ( scale < 0.70f ) scale = 0.70f;
		if ( scale > 1.20f ) scale = 1.20f;
	} else if ( strcmp(boneName, BONE_SCALE_NAME_CALF) == 0 ) {
		if ( scale < 0.8f ) scale = 0.8f;
		if ( scale > 1.4f ) scale = 1.4f;
	} else if ( strcmp(boneName, BONE_SCALE_NAME_FOOT) == 0 ) {
		if ( scale < 0.70f ) scale = 0.70f;
		if ( scale > 1.30f ) scale = 1.30f;
	} else {
		if ( scale < 0.80f ) scale = 0.80f;
		if ( scale > 1.20f ) scale = 1.20f;
	}

	if (m_pRuEntity && m_pRuMimeEntity)
	{
		//if ( RuMime_SetBoneScale(m_pRuMimeEntity, boneName, scale) )
		//{
			//vector<RoleBoneScaleInfo>::iterator iter = m_boneScaleInfo.begin();
			//for ( ; iter != m_boneScaleInfo.end(); iter++ )
			//{
			//	if ( iter->name.compare(boneName) == 0 )
			//	{
			//		iter->scale = scale;
			//		break;
			//	}
			//}
			//if ( iter == m_boneScaleInfo.end() )
			//{
			//	m_boneScaleInfo.push_back(RoleBoneScaleInfo(boneName, scale));
			//}
		//	return true;
		//}
		RuMime_SetBoneScale(m_pRuMimeEntity, boneName, scale);
		m_boneScaleInfo[boneName] = scale;
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
int CRoleSprite::GetNumBoneScales()
{
	if ( m_pRuMimeEntity )
	{
		return m_pRuMimeEntity->GetNumBoneScalers();
	}
	return 0;
}

//-----------------------------------------------------------------------------
float CRoleSprite::GetBoneScaleByIndex(int index)
{
	CRuVector3 boneScale;
	if ( m_pRuMimeEntity )
	{
		m_pRuMimeEntity->GetBoneScalerParametersByIndex(index, boneScale);
	}
	return boneScale.m_y;
}

//-----------------------------------------------------------------------------
const char* CRoleSprite::GetBoneScaleNameByIndex(int index)
{
	if ( m_pRuMimeEntity )
	{
		return m_pRuMimeEntity->GetBoneScalerNameByIndex(index);
	}
	return "";
}

//-----------------------------------------------------------------------------
void CRoleSprite::ReapplyBoneScaleSettings()
{
	SetBoneScale(BONE_SCALE_NAME_HEAD,		m_boneScaleInfo[BONE_SCALE_NAME_HEAD]);
	SetBoneScale(BONE_SCALE_NAME_BREASTS,	m_boneScaleInfo[BONE_SCALE_NAME_BREASTS]);
	SetBoneScale(BONE_SCALE_NAME_BODY,		m_boneScaleInfo[BONE_SCALE_NAME_BODY]);
	SetBoneScale(BONE_SCALE_NAME_CHEST,		m_boneScaleInfo[BONE_SCALE_NAME_CHEST]);
	SetBoneScale(BONE_SCALE_NAME_PELVIS,	m_boneScaleInfo[BONE_SCALE_NAME_PELVIS]);
	SetBoneScale(BONE_SCALE_NAME_ARM,		m_boneScaleInfo[BONE_SCALE_NAME_ARM]);
	SetBoneScale(BONE_SCALE_NAME_FOREARM,	m_boneScaleInfo[BONE_SCALE_NAME_FOREARM]);
	SetBoneScale(BONE_SCALE_NAME_HAND,		m_boneScaleInfo[BONE_SCALE_NAME_HAND]);
	SetBoneScale(BONE_SCALE_NAME_LEGS,		m_boneScaleInfo[BONE_SCALE_NAME_LEGS]);
	SetBoneScale(BONE_SCALE_NAME_CALF,		m_boneScaleInfo[BONE_SCALE_NAME_CALF]);
	SetBoneScale(BONE_SCALE_NAME_FOOT,		m_boneScaleInfo[BONE_SCALE_NAME_FOOT]);
}

/*
//-----------------------------------------------------------------------------
void CRoleSprite::CopyPaperdoll(CRoleSprite* source)
{
	if ( source )
	{
		SetObjectId(source->GetObjectId());

		SetPaperdollSkinColor(source->GetPaperdollSkinColor());
		SetPaperdollHairColor(source->GetPaperdollHairColor());
		
		int layerColor[2];
		if ( source->GetPaperdollColor(PAPERDOLL_PART_NAME_HELMET, layerColor) )		
			SetPaperdollColor(PAPERDOLL_PART_NAME_HELMET, layerColor[0], layerColor[1]);
		if ( source->GetPaperdollColor(PAPERDOLL_PART_NAME_SHOULDER, layerColor) )		
			SetPaperdollColor(PAPERDOLL_PART_NAME_SHOULDER, layerColor[0], layerColor[1]);
		if ( source->GetPaperdollColor(PAPERDOLL_PART_NAME_TORSO, layerColor) )		
			SetPaperdollColor(PAPERDOLL_PART_NAME_TORSO, layerColor[0], layerColor[1]);
		if ( source->GetPaperdollColor(PAPERDOLL_PART_NAME_HAND, layerColor) )		
			SetPaperdollColor(PAPERDOLL_PART_NAME_HAND, layerColor[0], layerColor[1]);
		if ( source->GetPaperdollColor(PAPERDOLL_PART_NAME_BELT, layerColor) )		
			SetPaperdollColor(PAPERDOLL_PART_NAME_BELT, layerColor[0], layerColor[1]);
		if ( source->GetPaperdollColor(PAPERDOLL_PART_NAME_LEG, layerColor) )		
			SetPaperdollColor(PAPERDOLL_PART_NAME_LEG, layerColor[0], layerColor[1]);
		if ( source->GetPaperdollColor(PAPERDOLL_PART_NAME_FOOT, layerColor) )		
			SetPaperdollColor(PAPERDOLL_PART_NAME_FOOT, layerColor[0], layerColor[1]);
		if ( source->GetPaperdollColor(PAPERDOLL_PART_NAME_BACK, layerColor) )		
			SetPaperdollColor(PAPERDOLL_PART_NAME_BACK, layerColor[0], layerColor[1]);	

		SetPaperdollPart(PAPERDOLL_PART_NAME_HEAD,		source->GetPaperdollPart(PAPERDOLL_PART_NAME_HEAD));
		SetPaperdollPart(PAPERDOLL_PART_NAME_HAIR,		source->GetPaperdollPart(PAPERDOLL_PART_NAME_HAIR));
		SetPaperdollPart(PAPERDOLL_PART_NAME_HELMET,	source->GetPaperdollPart(PAPERDOLL_PART_NAME_HELMET));
		SetPaperdollPart(PAPERDOLL_PART_NAME_SHOULDER,	source->GetPaperdollPart(PAPERDOLL_PART_NAME_SHOULDER));
		SetPaperdollPart(PAPERDOLL_PART_NAME_TORSO,		source->GetPaperdollPart(PAPERDOLL_PART_NAME_TORSO));
		SetPaperdollPart(PAPERDOLL_PART_NAME_HAND,		source->GetPaperdollPart(PAPERDOLL_PART_NAME_HAND));
		SetPaperdollPart(PAPERDOLL_PART_NAME_BELT,		source->GetPaperdollPart(PAPERDOLL_PART_NAME_BELT));
		SetPaperdollPart(PAPERDOLL_PART_NAME_LEG,		source->GetPaperdollPart(PAPERDOLL_PART_NAME_LEG));
		SetPaperdollPart(PAPERDOLL_PART_NAME_FOOT,		source->GetPaperdollPart(PAPERDOLL_PART_NAME_FOOT));
		SetPaperdollPart(PAPERDOLL_PART_NAME_BACK,		source->GetPaperdollPart(PAPERDOLL_PART_NAME_BACK));	

		if ( source->m_pRuMimeEntity && m_pRuMimeEntity )
		{
			CRuVector3 boneScale;
			const char* boneName;

			int count = source->m_pRuMimeEntity->GetNumBoneScalers();
			for ( int i = 0; i < count; i++ )
			{
				boneName = source->m_pRuMimeEntity->GetBoneScalerNameByIndex(i);
				if ( source->m_pRuMimeEntity->GetBoneScalerParametersByIndex(i, boneScale) )
				{
					m_pRuMimeEntity->SetBoneScalerParameters(boneName, boneScale);
				}
			}
		}

		PaperdollBuild();

		SetHandsObjectId(eRoleHands_MainHand,	source->m_handsObjectId[eRoleHands_MainHand]);
		SetHandsObjectId(eRoleHands_OffHand,	source->m_handsObjectId[eRoleHands_OffHand]);
		SetWeaponPosition(true);
	}
}
*/

//-----------------------------------------------------------------------------
// 設定戰鬥狀態
void CRoleSprite::SetCombatState(bool isCombat)
{
	if ( m_state.isCombat != isCombat )
	{
		m_state.isCombat = isCombat;

		if ( GetCombatState() )
		{
			// 將第三類物品移除
			ReleaseHandsEntity(eRoleHands_Ranged);
			//ReleaseHandsEntity(eRoleHands_Crafting);
			if ( !m_state.isSpell )					// 若不在施法中，設定武器拿在手中
			{
				SetWeaponPosition( true );
			}
		} else if ( m_pRuActor ) {
			//m_pRuActor->ClearForegroundQueue();
		}
		SetCombatMotion();
	}
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetWalkState(bool isWalk)
{
	if (m_state.isWalk != isWalk)
	{
		m_state.isWalk = isWalk;

		// 潛行中
		if ( m_controlState.sneakMime == false )
		{
			if ( isWalk )
			{
				if ( m_pRuActor && m_pRuActor->GetDefaultRunningAction() == ruFUSION_MIME_RUN_FORWARD )
				{
					m_pRuActor->SetDefaultRunningAction(ruFUSION_MIME_WALK_FORWARD);
				}
			}
			else
			{
				if ( m_pRuActor && m_pRuActor->GetDefaultRunningAction() == ruFUSION_MIME_WALK_FORWARD )
				{
					m_pRuActor->SetDefaultRunningAction(ruFUSION_MIME_RUN_FORWARD);
				}
			}
		}

		if ( m_moveTime > 0.1f && m_moveTime < NEXT_MOVETIME )
			SetTargetPos(m_targetPos);
	}
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetUnholsterState(bool isUnholster)
{
	switch ( GetActorState(3) )
	{
	case ruFUSION_ACTORSTATE_HOLSTER:
	case ruFUSION_ACTORSTATE_UNHOLSTER:
	case ruFUSION_ACTORSTATE_HOLSTER_2H:
	case ruFUSION_ACTORSTATE_UNHOLSTER_2H:
		return;		

	default:
		if ( m_state.isUnholster == isUnholster )
			return;

		// right hand
		switch ( GetTakeItemType(m_handsEntity[0].id) )
		{
		case eRoleTakeItem_Bow:
		case eRoleTakeItem_CossBow:
		case eRoleTakeItem_Gun:
			break;

		case eRoleTakeItem_Claymore:
		case eRoleTakeItem_Staff:
		case eRoleTakeItem_2H_Axe:
		case eRoleTakeItem_2H_Hammer:
		case eRoleTakeItem_Polearm:
			if ( isUnholster )
				SetActorState(ruFUSION_ACTORSTATE_UNHOLSTER_2H, 3);
			else
				SetActorState(ruFUSION_ACTORSTATE_HOLSTER_2H, 3);
			break;

		case eRoleTakeItem_Unarmed:
		case eRoleTakeItem_Blade:
		case eRoleTakeItem_Dagger:
		case eRoleTakeItem_Wand:
		case eRoleTakeItem_Axe:
		case eRoleTakeItem_Bludgeon:
		case eRoleTakeItem_Implement:
			if ( isUnholster )
				SetActorState(ruFUSION_ACTORSTATE_UNHOLSTER, 3);
			else
				SetActorState(ruFUSION_ACTORSTATE_HOLSTER, 3);
			break;
		}
	}
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetDeadState(bool isDead)
{
	if ( m_state.isDead != isDead )
	{
		m_state.isDead = isDead;

		if ( m_state.isDead )
		{
			SetCombatState(false);			// 停止戰鬥
			EndSpell();						// 停止施法過程

			// 清除 BUFF
			int loop = 0;
			while ( loop < (int)m_buffInfo.size() )
			{
				bool clear = true;
				RoleBuffInfo& buff = m_buffInfo[loop];

				GameObjDbStruct* itemDB = CNetGlobal::GetObj(buff.id);
				if ( itemDB && itemDB->MagicBase.Setting.DeadNotClear )
					clear = false;

				if ( clear )
				{
					EraseBuff(buff.id, buff.ownerID);
				}
				else
				{
					loop++;
				}
			}

			float y;
			if ( TerrainTestHight(m_nowPos, y, 50.0f) && m_nowPos.m_y - y > 12 ) {
				SetPos(CRuVector3(m_nowPos.m_x, y, m_nowPos.m_z));
			}

			// ClearBuff();					// 若角色死亡則清除Buff資料

			// 死亡清除裝飾品
			// DetachOrnamentEntity(false);
			// DetachBackEntity();				// 背後旗幟
		}
		else
		{
			// 死亡清除裝飾品
			// AttachOrnamentEntity();
			// AttachBackEntity();				// 背後旗幟
		}
	}	
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::SetSitState(bool isSit)
{
	if ( m_state.isSit != isSit )
	{
		m_state.isSit = isSit;
		if (m_state.isSit)
		{
			// 沒有施法並且沒有恐懼
			if ( m_spellID == 0 && m_specialEffect.fear <= 0 )
			{
				SetActorState(ruFUSION_ACTORSTATE_SIT_BEGIN, 1);
			}
		}
		else
		{
			switch (GetActorState(1))
			{
			case ruFUSION_ACTORSTATE_SIT_BEGIN:
			case ruFUSION_ACTORSTATE_SIT_LOOP:
				SetActorState(ruFUSION_ACTORSTATE_SIT_END, 1);
				break;
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::SetSitChairState(bool isSit)
{
	if ( m_state.isSitChair != isSit )
	{
		m_state.isSitChair = isSit;
		if (m_state.isSitChair)
		{
			// 沒有施法並且沒有恐懼
			if ( m_spellID == 0 && m_specialEffect.fear <= 0 )
			{
				SetActorState(ruFUSION_ACTORSTATE_SIT_CHAIR_LOOP, 1);
				SetGravityMode(false);
			}
		}
		else
		{
			SetGravityMode(true);
			switch (GetActorState(1))
			{
			case ruFUSION_ACTORSTATE_SIT_CHAIR_BEGIN:
			case ruFUSION_ACTORSTATE_SIT_CHAIR_LOOP:
				SetActorState(ruFUSION_ACTORSTATE_SIT_CHAIR_END, 1);				
				break;
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::SetWeaponPosition(bool isUnholster, bool force)
{
	if (!force && m_state.isUnholster == isUnholster)
		return;

	m_state.isUnholster = isUnholster;
	if ( m_state.isUnholster )
	{
		// 移除第三類物品,並將武器拿在手上
		ReleaseHandsEntity(eRoleHands_Ranged);
		ReleaseHandsEntity(eRoleHands_Crafting);

		// right hand
		switch ( GetTakeItemType(m_handsEntity[eRoleHands_MainHand].id) )
		{
		case eRoleTakeItem_Bow:
		case eRoleTakeItem_CossBow:
		case eRoleTakeItem_Gun:
			AttachHandsEntity(eRoleHands_MainHand, eAttach_Left_Hand);
			break;

		case eRoleTakeItem_Claymore:
		case eRoleTakeItem_Staff:
		case eRoleTakeItem_2H_Axe:
		case eRoleTakeItem_2H_Hammer:
		case eRoleTakeItem_Polearm:
		case eRoleTakeItem_Unarmed:
		case eRoleTakeItem_Blade:
		case eRoleTakeItem_Dagger:
		case eRoleTakeItem_Wand:
		case eRoleTakeItem_Axe:
		case eRoleTakeItem_Bludgeon:
		case eRoleTakeItem_Implement:
			AttachHandsEntity(eRoleHands_MainHand, eAttach_Right_Hand);
			break;

		default:
			AttachHandsEntity(eRoleHands_MainHand, eAttach_Null);
			break;
		}

		// left hand
		switch ( GetTakeItemType(m_handsEntity[eRoleHands_OffHand].id) )
		{
		case eRoleTakeItem_Unarmed:
		case eRoleTakeItem_Blade:
		case eRoleTakeItem_Dagger:
		case eRoleTakeItem_Wand:
		case eRoleTakeItem_Axe:
		case eRoleTakeItem_Bludgeon:
		case eRoleTakeItem_Implement:
			AttachHandsEntity(eRoleHands_OffHand, eAttach_Left_Hand);
			break;

		case eRoleTakeItem_Shield:
			AttachHandsEntity(eRoleHands_OffHand, eAttach_Hand_Shield);
			break;

		default:
			AttachHandsEntity(eRoleHands_OffHand, eAttach_Null);
			break;
		}
	}
	else if ( m_controlState.alwaysHoldWeapon == false )
	{
		// right hand
		switch ( GetTakeItemType(m_handsEntity[eRoleHands_MainHand].id) )
		{
		case eRoleTakeItem_Polearm:
		case eRoleTakeItem_Staff:
			AttachHandsEntity(eRoleHands_MainHand, eAttach_Back_Polearm);
			break;

		case eRoleTakeItem_Claymore:		
		case eRoleTakeItem_2H_Axe:
		case eRoleTakeItem_2H_Hammer:
			AttachHandsEntity(eRoleHands_MainHand, eAttach_Back_Sword);
			break;

		case eRoleTakeItem_Unarmed:
		case eRoleTakeItem_Blade:
		case eRoleTakeItem_Dagger:
		case eRoleTakeItem_Wand:
		case eRoleTakeItem_Axe:
		case eRoleTakeItem_Bludgeon:
		case eRoleTakeItem_Implement:
			AttachHandsEntity(eRoleHands_MainHand, eAttach_Left_Weapon);
			break;

		default:
			AttachHandsEntity(eRoleHands_MainHand, eAttach_Null);
			break;
		}

		// left hand
		switch ( GetTakeItemType(m_handsEntity[eRoleHands_OffHand].id) )
		{
		case eRoleTakeItem_Unarmed:
		case eRoleTakeItem_Blade:
		case eRoleTakeItem_Dagger:
		case eRoleTakeItem_Wand:
		case eRoleTakeItem_Axe:
		case eRoleTakeItem_Bludgeon:
		case eRoleTakeItem_Implement:
			AttachHandsEntity(eRoleHands_OffHand, eAttach_Right_Weapon);
			break;

		case eRoleTakeItem_Shield:
			AttachHandsEntity(eRoleHands_OffHand, eAttach_Back_Shield);
			break;

		default:
			AttachHandsEntity(eRoleHands_OffHand, eAttach_Null);
			break;
		}
	}
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetRangedMotion(const char* motion)
{
	// g_pError->AddMessage(0, 0, "SetRangedMotion %s", motion);

	if ( m_handsEntity[2].entity == NULL )
		return;

	if ( m_handsEntity[2].type == eRoleHands_Ranged )
	{
		// 產生弓箭
		if (strcmp("bow_draw", motion) == 0 && GetTakeItemType(m_handsEntity[2].id) == eRoleTakeItem_Bow /*&& m_spellID != 0*/ )
		{
			CreateHandsEntity(eRoleHands_Arrow, WEWPOA_ARROW_ID, 0);
		}
		// 弓箭消失
		else if (strcmp("bow_shoot", motion) == 0) 
		{
			ReleaseHandsEntity(eRoleHands_Arrow);
		}
		RuACT_PlayMotionEx(m_handsEntity[2].entity, -1, motion);
	}
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetCombatMotion()
{
	if ( m_state.isCombat )
	{
		// 進入戰鬥
		SetWeaponPosition(true);

		/*
		// 檢查是否有使用遠程武器
		if ( m_handsEntity[2].type == eRoleHands_Ranged )
		{
			switch ( GetTakeItemType(m_handsEntity[2].id) )
			{
			case eRoleTakeItem_Bow:			
				SetActorState(ruFUSION_ACTORSTATE_COMBAT_BOW, 2);
				break;
			
			case eRoleTakeItem_CossBow:
			case eRoleTakeItem_Gun:
				SetActorState(ruFUSION_ACTORSTATE_COMBAT_GUN, 2);
				break;

			default:
				SetActorState(ruFUSION_ACTORSTATE_NORMAL, 2);
				break;
			}
		}
		else
		*/
		{
			// 依拿取的武器類型，設定動作
			switch ( GetTakeItemType(m_handsEntity[0].id) )
			{
			case eRoleTakeItem_Unarmed:
				SetActorState(ruFUSION_ACTORSTATE_COMBAT_UNARMED, 2);
				break;

			case eRoleTakeItem_Dagger:
			case eRoleTakeItem_Blade:
			case eRoleTakeItem_Wand:
			case eRoleTakeItem_Axe:
			case eRoleTakeItem_Bludgeon:
				SetActorState(ruFUSION_ACTORSTATE_COMBAT_1H, 2);
				break;

			case eRoleTakeItem_Claymore:
			case eRoleTakeItem_2H_Axe:
			case eRoleTakeItem_2H_Hammer:
				SetActorState(ruFUSION_ACTORSTATE_COMBAT_2H, 2);
				break;

			case eRoleTakeItem_Staff:				
			case eRoleTakeItem_Polearm:
				SetActorState(ruFUSION_ACTORSTATE_COMBAT_POLEARM, 2);
				break;

			case eRoleTakeItem_Bow:
				SetActorState(ruFUSION_ACTORSTATE_COMBAT_BOW, 2);
				break;

			case eRoleTakeItem_CossBow:
			case eRoleTakeItem_Gun:
				SetActorState(ruFUSION_ACTORSTATE_COMBAT_GUN, 2);
				break;

			default:
				switch (GetTakeItemType(m_handsEntity[1].id))
				{
				case eRoleTakeItem_None:
					SetActorState(ruFUSION_ACTORSTATE_COMBAT_UNARMED, 2);
					break;

				default:
					SetActorState(ruFUSION_ACTORSTATE_COMBAT_1H, 2);
					break;
				}
			}
		}		
	}
	else if ( m_controlState.forceCombatMotion == false )
	{
		/*
		// Transition back to non-combat state, clear the entire foreground queue
		if(m_pRuActor)
		{
			m_pRuActor->ClearForegroundQueue();
		}
		*/

		SetActorState(ruFUSION_ACTORSTATE_NORMAL, 2);
	}
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetAttackMotion(bool offHand, CRuACT_ActionEvents* actionEvents, bool unarmed)
{	
	// 檢查是否已經死亡
	if ( GetDeadState() )
	{
		return;
	}

	if ( unarmed )
	{
		SetActorState(ruFUSION_ACTORSTATE_ATTACK_UNARMED, 3, false, actionEvents);
	}
	else
	{
		// 移除第三類物品,並將武器拿在手上
		// ReleaseHandsEntity(2);
		SetWeaponPosition(true);	

		// 副手
		if ( offHand )
		{
			switch ( GetTakeItemType(m_handsEntity[1].id) )
			{
			case eRoleTakeItem_Unarmed:
				SetActorState(ruFUSION_ACTORSTATE_ATTACK_UNARMED, 3, false, actionEvents);
				break;

			case eRoleTakeItem_Dagger:
				SetActorState(ruFUSION_ACTORSTATE_ATTACK_OH_PIERCE, 3, false, actionEvents);
				break;

			case eRoleTakeItem_Blade:
			case eRoleTakeItem_Wand:
			case eRoleTakeItem_Axe:
			case eRoleTakeItem_Bludgeon:
				SetActorState(ruFUSION_ACTORSTATE_ATTACK_OH, 3, false, actionEvents);
				break;
			}
		}
		else
		{
			switch ( GetTakeItemType(m_handsEntity[0].id) )
			{
			case eRoleTakeItem_Unarmed:
				SetActorState(ruFUSION_ACTORSTATE_ATTACK_UNARMED, 3, false, actionEvents);
				break;

			case eRoleTakeItem_Dagger:
				SetActorState(ruFUSION_ACTORSTATE_ATTACK_1H_PIERCE, 3, false, actionEvents);
				break;

			case eRoleTakeItem_Blade:			
			case eRoleTakeItem_Wand:
			case eRoleTakeItem_Axe:
			case eRoleTakeItem_Bludgeon:
				SetActorState(ruFUSION_ACTORSTATE_ATTACK_1H, 3, false, actionEvents);
				break;

			case eRoleTakeItem_Claymore:
			case eRoleTakeItem_2H_Axe:
			case eRoleTakeItem_2H_Hammer:
				SetActorState(ruFUSION_ACTORSTATE_ATTACK_2H, 3, false, actionEvents);
				break;

			case eRoleTakeItem_Staff:
			case eRoleTakeItem_Polearm:
				SetActorState(ruFUSION_ACTORSTATE_ATTACK_POLEARM, 3, false, actionEvents);
				break;

				/*
				case eRoleTakeItem_Bow:
				SetActorState(ruFUSION_ACTORSTATE_ATTACK_BOW_BEGIN, 3);
				break;

				case eRoleTakeItem_CossBow:
				case eRoleTakeItem_Gun:
				SetActorState(ruFUSION_ACTORSTATE_ATTACK_GUN_BEGIN, 3);
				break;
				*/

			default:
				SetActorState(ruFUSION_ACTORSTATE_ATTACK_UNARMED, 3, false, actionEvents);
				break;
			}
		}
	}	
}

//-----------------------------------------------------------------------------
CRuFusion_ActorState CRoleSprite::GetParryMotion()
{
	if ( m_handsEntity[2].type == eRoleHands_Ranged )
	{
		switch ( GetTakeItemType(m_handsEntity[2].id) )
		{
		case eRoleTakeItem_Bow:		
			return ruFUSION_ACTORSTATE_PARRY_BOW;

		case eRoleTakeItem_CossBow:
		case eRoleTakeItem_Gun:
			return ruFUSION_ACTORSTATE_PARRY_GUN;			
		}
	}
	else
	{
		switch ( GetTakeItemType(m_handsEntity[0].id) )
		{
		case eRoleTakeItem_Unarmed:
			return ruFUSION_ACTORSTATE_PARRY_UNARMED;

		case eRoleTakeItem_Blade:
		case eRoleTakeItem_Dagger:
		case eRoleTakeItem_Wand:
		case eRoleTakeItem_Axe:
		case eRoleTakeItem_Bludgeon:
			return ruFUSION_ACTORSTATE_PARRY_1H;

		case eRoleTakeItem_Claymore:
		case eRoleTakeItem_2H_Axe:
		case eRoleTakeItem_2H_Hammer:
			return ruFUSION_ACTORSTATE_PARRY_2H;

		case eRoleTakeItem_Staff:
		case eRoleTakeItem_Polearm:
			return ruFUSION_ACTORSTATE_PARRY_POLEARM;
		}
	}	
	return ruFUSION_ACTORSTATE_NORMAL;
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetSpellMotion(const char* motionName)
{
	// 檢查是否已經死亡
	if ( GetDeadState() )
	{
		return;
	}

	if ( motionName && strlen(motionName) > 0 )
	{
		CRuFusion_ActorState actor = MotionToActorState(motionName);

		// SetActorState(ruFUSION_ACTORSTATE_NORMAL, 1);

		// 遠距離技能檢查
		switch(actor)
		{
		case ruFUSION_ACTORSTATE_ATTACK_BOW_BEGIN:		
			switch (GetTakeItemType(m_handsObjectId[eRoleHands_Ranged]))
			{
			case eRoleTakeItem_Bow:
				actor = ruFUSION_ACTORSTATE_ATTACK_BOW_BEGIN;
				break;

			case eRoleTakeItem_CossBow:
			case eRoleTakeItem_Gun:
				actor = ruFUSION_ACTORSTATE_ATTACK_GUN_BEGIN;
				break;
			}
			break;

		case ruFUSION_ACTORSTATE_ATTACK_BOW_END:
			switch (GetTakeItemType(m_handsObjectId[eRoleHands_Ranged]))
			{
			case eRoleTakeItem_Bow:
				actor = ruFUSION_ACTORSTATE_ATTACK_BOW_END;
				break;

			case eRoleTakeItem_CossBow:
			case eRoleTakeItem_Gun:
				actor = ruFUSION_ACTORSTATE_ATTACK_GUN_END;
				break;
			}
			break;
		}
		SetActorState(actor);
	}
}

//-----------------------------------------------------------------------------
void CRoleSprite::BeginSpell(int id, bool hasMotion, CRoleSprite* target)
{
	GameObjDbStruct* itemDB = CNetGlobal::GetObj(id);	

	if ( itemDB->MagicCol.IsRidePetSkill && m_mountSprite )
	{
		m_mountSprite->BeginSpell(id, hasMotion, target);
	}
	else
	{
		ClearCastingEffect();

		if ( itemDB )
		{
			GameObjDbStruct* imageDB = CNetGlobal::GetObj(itemDB->ImageID);
			if ( imageDB == NULL )
				return;		

			if ( !itemDB->MagicCol.Flag.forceDisplayEffect && s_hideEffectFunc && !s_hideEffectFunc(m_id, 0) )
				return;

			ReleaseHandsEntity(eRoleHands_Arrow);				// 清除彈藥

			// 是否有動作
			if ( hasMotion )
			{
				SetSpellMotion(imageDB->Image.GatherMotion);
			}		

			// 產生集氣特效
			for ( int i = 0; i < 3; i++ )
			{
				MagicPartical& gather = imageDB->Image.GatherPartical[i];
				float scale = imageDB->Image.ModelSize * imageDB->Image.ActRate[EM_ImageActType_Gather] / 10000.0f;
				BeginCastingEffect(gather.Name, gather.Point, gather.IsSoundEnabled, scale);
			}
		}		
	}

	m_spellID = id;
}

//-----------------------------------------------------------------------------
void CRoleSprite::EndSpell()
{
	// 
	switch( GetActorState(3) )
	{
	case ruFUSION_ACTORSTATE_BUFF_BEGIN:
	case ruFUSION_ACTORSTATE_BUFF_LOOP:
	case ruFUSION_ACTORSTATE_CAST_BEGIN:
	case ruFUSION_ACTORSTATE_CAST_LOOP:
	case ruFUSION_ACTORSTATE_CHANNEL_BEGIN:
	case ruFUSION_ACTORSTATE_CHANNEL_LOOP:
	case ruFUSION_ACTORSTATE_ATTACK_BOW_BEGIN:
	case ruFUSION_ACTORSTATE_ATTACK_BOW_LOOP:
	case ruFUSION_ACTORSTATE_ATTACK_GUN_BEGIN:
	case ruFUSION_ACTORSTATE_ATTACK_GUN_LOOP:	
		SetActorState(ruFUSION_ACTORSTATE_NORMAL, 3);
		break;	

	case ruFUSION_ACTORSTATE_ATTACK_BOW_END:
	case ruFUSION_ACTORSTATE_ATTACK_GUN_END:
		break;
	}

	//
	switch ( GetActorState(1) )
	{
	case ruFUSION_ACTORSTATE_CRAFTING_BEGIN:
	case ruFUSION_ACTORSTATE_CRAFTING_LOOP:
		SetActorState(ruFUSION_ACTORSTATE_CRAFTING_END, 1);
		break;
	}	
	
	/*
	switch (m_handsEntity[2].type)
	{
	case eRoleHands_Ranged:
		//SetCombatMotion();		
		SetRangedMotion("bow_idle");
		break;

	default:		
		// 如果在戰鬥中,設定武器拿在手上		
		if ( GetCombatState() )
			SetWeaponPosition(true);		
		break;
	}		
	
	// 如果在戰鬥中,設定武器拿在手上
	if ( GetCombatState() )
	{
		SetWeaponPosition(true);
	}
	*/

	m_spellID = 0;
	EndCastingEffect();
}

//-----------------------------------------------------------------------------
bool CRoleSprite::CastSpell(CRuACT_ActionEvents* actionEvents)
{
	GameObjDbStruct* itemDB = NULL;
	if ( m_spellID != 0 && (itemDB = CNetGlobal::GetObj(m_spellID)) != NULL )
	{
		GameObjDbStruct* imageDB = CNetGlobal::GetObj(itemDB->ImageID);		
		if ( imageDB )
		{
			if ( strlen(imageDB->Image.ShootMotion) > 0 )
			{
				CRuFusion_ActorState actor;
				if ( strcmp(imageDB->Image.ShootMotion, "weapon_attack") == 0 )
				{
					actor = ruFUSION_ACTORSTATE_ATTACK_UNARMED;
					switch ( GetTakeItemType(m_handsObjectId[eRoleHands_MainHand]) )
					{
					case eRoleTakeItem_Unarmed:
						actor = ruFUSION_ACTORSTATE_ATTACK_UNARMED_SP;
						break;

					case eRoleTakeItem_Blade:
					case eRoleTakeItem_Dagger:
					case eRoleTakeItem_Wand:
					case eRoleTakeItem_Axe:
					case eRoleTakeItem_Bludgeon:
						actor = ruFUSION_ACTORSTATE_ATTACK_1H_SP;
						break;

					case eRoleTakeItem_Claymore:
					case eRoleTakeItem_2H_Axe:
					case eRoleTakeItem_2H_Hammer:
						actor = ruFUSION_ACTORSTATE_ATTACK_2H_SP;
						break;

					case eRoleTakeItem_Staff:				
					case eRoleTakeItem_Polearm:
						actor = ruFUSION_ACTORSTATE_ATTACK_POLEARM_SP;
						break;
					}

					// 強制武器拿在手上
					SetWeaponPosition(true);
				}
				else
				{
					actor = MotionToActorState(imageDB->Image.ShootMotion);

					// 遠距離技能檢查
					switch(actor)
					{
					case ruFUSION_ACTORSTATE_ATTACK_BOW_BEGIN:		
						switch (GetTakeItemType(m_handsObjectId[eRoleHands_Ranged]))
						{
						case eRoleTakeItem_Bow:
							actor = ruFUSION_ACTORSTATE_ATTACK_BOW_BEGIN;
							break;

						case eRoleTakeItem_CossBow:
						case eRoleTakeItem_Gun:
							actor = ruFUSION_ACTORSTATE_ATTACK_GUN_BEGIN;
							actor = ruFUSION_ACTORSTATE_ATTACK_GUN_BEGIN;
							break;
						}
						break;

					case ruFUSION_ACTORSTATE_ATTACK_BOW_END:
						switch (GetTakeItemType(m_handsObjectId[eRoleHands_Ranged]))
						{
						case eRoleTakeItem_Bow:
							// 如果沒有箭矢在手上,則產生箭矢
							if ( m_handsEntity[eRoleHands_Arrow].entity == NULL )
								CreateHandsEntity(eRoleHands_Arrow, WEWPOA_ARROW_ID, 0);

							actor = ruFUSION_ACTORSTATE_ATTACK_BOW_END;
							break;

						case eRoleTakeItem_CossBow:
						case eRoleTakeItem_Gun:
							actor = ruFUSION_ACTORSTATE_ATTACK_GUN_END;
							actor = ruFUSION_ACTORSTATE_ATTACK_GUN_END;
							break;
						}
						break;
					}
				}		
				SetActorState(actor, 0, false, actionEvents);
				return true;
			}
		}				
	}
	return false;
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetCastingTime(float time, float nowTime)
{
	if ( m_castingTime != time )
	{
		m_castingTime = time;
		m_currentTime = nowTime;
		if ( s_procFunc )
			s_procFunc(this, eSpriteMsg_CastingTime_Changed, 0, 0);
	}
}

//-----------------------------------------------------------------------------
void CRoleSprite::BeginCastingEffect(const char* actName, const char* point, bool sound, float scale)
{
	if ( actName == NULL || strlen(actName) < 1 )
		return;	

	CEffectSprite* effect = new CEffectSprite;
	if ( !effect->LoadRuEntity(actName) )
	{
		delete effect;
		return;
	}
	
	effect->SetSoundState(sound);
	effect->SetScale(scale, eSpriteScale_World);
	effect->SetAutoDestruct(false);
	effect->SetEffectMotionStep(eEffectMotion_Step_Appear);
	effect->AttachSprite(this, IBaseSprite::GetAttachPointEnum(point));
	m_castingEffect.push_back(effect);
}

//-----------------------------------------------------------------------------
void CRoleSprite::EndCastingEffect()
{
	for ( vector<CEffectSprite*>::iterator iter = m_castingEffect.begin(); iter != m_castingEffect.end(); iter++ )
	{
		CEffectSprite* effect = *iter;
		switch ( effect->GetEffectMotionStep() )
		{
		case eEffectMotion_Step_Appear:					// 循環特效開始
			effect->SetEffectMotionStep(eEffectMotion_Step_Disappear);
			effect->SetIdleEffectLiveTime(1.0f);
			break;
		case eEffectMotion_Step_Idle:					// 循環特效中間(Loop)
			effect->SetIdleEffectLiveTime(1.0f);
			break;
		case eEffectMotion_Step_Disappear:				// 循環特效結束
		case eEffectMotion_Step_Explode:				// 爆炸
		case eEffectMotion_Step_Dead:					// 結束
			break;
		}
	}
}

//-----------------------------------------------------------------------------
void CRoleSprite::UpdateCastingEffect( float elapseTime )
{
	vector<CEffectSprite*>::iterator iter = m_castingEffect.begin();
	while ( iter != m_castingEffect.end() )
	{
		CEffectSprite* effect = *iter;
		if ( effect->GetEffectMotionStep() == eEffectMotion_Step_Dead )
		{
			iter = m_castingEffect.erase(iter);
			delete effect;
		} else
		{
			iter++;
		}
	}
}

//-----------------------------------------------------------------------------
void CRoleSprite::ClearCastingEffect()
{
	vector<CEffectSprite*>::iterator iter = m_castingEffect.begin();
	while ( iter != m_castingEffect.end() ) {
		CEffectSprite* effect = *iter;
		iter = m_castingEffect.erase(iter);
		delete effect;
	}
	m_castingEffect.clear();
}

//-----------------------------------------------------------------------------
void CRoleSprite::AddBuff(int source, int id, float time, int magicLv, const char* name, int ownerID, float dotRate)
{	
	GameObjDbStruct* itemDB = CNetGlobal::GetObj( id );
	if ( itemDB == NULL || itemDB->Classification != EM_ObjectClass_MagicBase )
		return;

	// 檢查是否有相同編號,相同編號則更新時間
	for ( vector<RoleBuffInfo>::iterator iter = m_buffInfo.begin(); iter != m_buffInfo.end(); iter++ )
	{
		bool sameBuff = false;

		if ( itemDB->MagicBase.Setting.SelfBuff )
		{
			if ( ownerID != -1 && ownerID == iter->ownerID && iter->id == id )
				sameBuff = true;
		}
		else if ( iter->id == id )
		{
			sameBuff = true;
		}

		if ( sameBuff )
		{			
			iter->time			= time;
			iter->magicLv		= magicLv;
			iter->recycleTime	= (float)itemDB->MagicBase.DotMagic.Time;
			iter->remnantTime	= 0.0f;		
			iter->destructTime	= 0.0f;
			if ( iter->time > 0.0f )
				iter->dotCount	= (int)(iter->time / iter->recycleTime);				
			else
				iter->dotCount	= 0x7fffffff;

			if ( s_procFunc )
				s_procFunc(this, eSpriteMsg_Buff_Changed, 0, 0);
			return;
		}

		/*
		if( itemDB->MagicBase.Setting.SelfBuff )
		{
			if( ownerID != buff->ownerID && ownerID != -1)
				continue;
		}
		if ( buff->id == id )
		{
			buff->time = time;
			if ( s_procFunc )
				s_procFunc(this, eSpriteMsg_Buff_Changed, 0, 0);
			return;
		}
		*/
	}	

	RoleBuffInfo buff;
	buff.sourceName = (name) ? name : "";
	buff.source		= source;
	buff.id			= id;
	buff.time		= time;	
	buff.magicLv	= magicLv;
	buff.dotRate	= dotRate;
	buff.ownerID	= ownerID;
	buff.destructTime = 0.0f;

	// 變身術
	if ( itemDB->MagicBase.Setting.FaceOffMagic )
	{
		buff.faceOff = true;
		//CreateSubstituteSprite(itemDB->MagicBase.FaceOffID);
		SetTempObjectId(itemDB->MagicBase.FaceOffID, itemDB->MagicBase.Setting.FaceOffMagic_ShowEq);
	}

	// DOT類觸發特效
	if ( itemDB->MagicBase.Setting.DotMagic )
	{
		//buff.remnantTime	= min(buff.recycleTime, 0.01f);
		buff.magicLv		= magicLv;
		buff.recycleTime	= (float)itemDB->MagicBase.DotMagic.Time;

		int iTime = (int)buff.time;
		int iRecycleTime = (int)buff.recycleTime;

		if ( iTime > 0 && iRecycleTime > 0 ) {
			buff.dotCount		= (int)(buff.time / buff.recycleTime);
			buff.remnantTime	= (float)((iTime % iRecycleTime != 0) ? iTime % iRecycleTime : iRecycleTime);
		} else {
			buff.dotCount		= 100000;
			buff.remnantTime	= buff.recycleTime;
		}
	}

	// 產生持續性特效	
	{
		if ( IsProxyLoadComplete() )
		{
			CreateBuffEffect(itemDB, &buff);
		}
		else
			buff.proxyLoad = true;
	}

	// 無效效果的旗標
	MagicEffectStruct validEffect;
	memset(&validEffect, 0xFF, sizeof(MagicEffectStruct));

	GameObjDbStruct* pObj = CNetGlobal::GetObj(m_objectId);
	if ( pObj && pObj->Classification == EM_ObjectClass_NPC )
	{
		validEffect = pObj->NPC.Effect;
	}

	// 計算特殊效果旗標
	if ( itemDB->MagicBase.Effect.Stop && validEffect.Stop )						// 鎖定移動
		m_specialEffect.lockedMove++;
	if ( itemDB->MagicBase.Effect.Blind && validEffect.Blind )						// 失明
		m_specialEffect.blind++;
	if ( itemDB->MagicBase.Effect.Client_Dizzy && validEffect.Client_Dizzy )		// 暈眩
		m_specialEffect.dizzy++;
	if ( itemDB->MagicBase.Effect.Client_Sleep && validEffect.Client_Sleep )		// 睡眠
		m_specialEffect.sleep++;
	if ( itemDB->MagicBase.Effect.Fear && validEffect.Fear )						// 恐懼
	{
		m_specialEffect.fear++;
		if ( m_specialEffect.fear > 0 && m_pRuActor )
		{
			switch (m_pRuActor->GetBackgroundState())
			{
			case ruFUSION_ACTORSTATE_SIT_BEGIN:
			case ruFUSION_ACTORSTATE_SIT_LOOP:
			case ruFUSION_ACTORSTATE_SIT_END:
			case ruFUSION_ACTORSTATE_SLEEP_BEGIN:
			case ruFUSION_ACTORSTATE_SLEEP_LOOP:
			case ruFUSION_ACTORSTATE_SLEEP_END:
			case ruFUSION_ACTORSTATE_CROUCH_BEGIN:
			case ruFUSION_ACTORSTATE_CROUCH_LOOP:
			case ruFUSION_ACTORSTATE_CROUCH_END:
			case ruFUSION_ACTORSTATE_CRAFTING_BEGIN:
			case ruFUSION_ACTORSTATE_CRAFTING_LOOP:
			case ruFUSION_ACTORSTATE_CRAFTING_END:
			case ruFUSION_ACTORSTATE_MINING_BEGIN:
			case ruFUSION_ACTORSTATE_MINING_LOOP:
			case ruFUSION_ACTORSTATE_MINING_END:
			case ruFUSION_ACTORSTATE_MINING_END_HOLD:
			case ruFUSION_ACTORSTATE_LUMBERJACK_BEGIN:
			case ruFUSION_ACTORSTATE_LUMBERJACK_LOOP:
			case ruFUSION_ACTORSTATE_LUMBERJACK_END:
			case ruFUSION_ACTORSTATE_LOOT_BEGIN:
			case ruFUSION_ACTORSTATE_LOOT_LOOP:
			case ruFUSION_ACTORSTATE_LOOT_END:
			case ruFUSION_ACTORSTATE_CARRY_BEGIN:
			case ruFUSION_ACTORSTATE_CARRY_LOOP:
			case ruFUSION_ACTORSTATE_CARRY_END:
			case ruFUSION_ACTORSTATE_EMOTE_HANDSTAND:
			case ruFUSION_ACTORSTATE_EMOTE_ROFL:
			case ruFUSION_ACTORSTATE_EMOTE_SWEEP1:
			case ruFUSION_ACTORSTATE_EMOTE_SWEEP2:
			case ruFUSION_ACTORSTATE_EMOTE_SWEEP3:
				m_pRuActor->SetBackgroundState(ruFUSION_ACTORSTATE_NORMAL);
				break;
			}
		}
	}
	if ( itemDB->MagicBase.Effect.CliOutofContorl && validEffect.CliOutofContorl )	// 無法控制
	{

		if ( s_procFunc ) s_procFunc(this, eSpriteMsg_CantCtrl, source, 0);
		m_specialEffect.cantCTRL++;
	}
	if ( itemDB->MagicBase.Effect.Chaos && validEffect.Chaos )						// 混亂
		m_specialEffect.chaos++;
	if ( itemDB->MagicBase.Effect.HitDown && validEffect.HitDown )					// 擊倒
		m_specialEffect.hitDown++;
	if ( itemDB->MagicBase.Effect.Cover && validEffect.Cover )						// 隱形
	{
		m_specialEffect.invisible++;
		if ( s_procFunc ) s_procFunc(this, eSpriteMsg_CancelSpell, 0, 0);
	}
	if ( itemDB->MagicBase.Effect.DetectCover && validEffect.DetectCover )			// 偵測隱形
		m_specialEffect.detectInvisible++;
	if ( itemDB->MagicBase.Effect.Sneak && validEffect.Sneak )						// 潛形
	{
		m_specialEffect.sneak++;
		if ( s_procFunc ) s_procFunc(this, eSpriteMsg_CancelSpell, 0, 0);
	}
	if ( itemDB->MagicBase.Effect.DetectSneak &&validEffect.DetectSneak  )			// 偵測潛形
		m_specialEffect.detectSneak++;	
	if ( itemDB->MagicBase.Effect.PKFlag )				// pvp
	{
		m_specialEffect.pvp++;
		m_isNeedResetWidget = true;
	}
	if ( itemDB->MagicBase.Effect.PKFlag_Disabled )		// 保護PK
	{
		m_specialEffect.protect++;
		m_isNeedResetWidget = true;
	}
	if ( itemDB->MagicBase.Effect.Freeze )				// 冰凍
		m_specialEffect.freeze++;
	if ( itemDB->MagicBase.Effect.Guilty )				// 犯罪者
		m_specialEffect.criminal++;
	if ( itemDB->MagicBase.Effect.DisableJobSkill )		// 無法使用本職業技能
		m_specialEffect.disableJobSkill++;
	if ( itemDB->MagicBase.Effect.GravityDisable )		// 暫時無重力
	{
		m_controlState.gravityBackup = m_controlState.gravityState;
		SetGravityMode(false);
	}

	if ( itemDB->MagicBase.Effect.ChangeMotion )
	{
		if ( m_pRuActor )
		{
			for ( int i = 0; i < 4; i++ )
			{
				switch ( itemDB->MagicBase.MotionChangeType[i] )
				{
				case 1:
					SetDefaultWalkingAction((CRuFusion_Mime_Action)itemDB->MagicBase.MotionChangeID[i]);
					break;

				case 2:
					SetDefaultBackAction((CRuFusion_Mime_Action)itemDB->MagicBase.MotionChangeID[i]);
					break;

				case 3:
					SetDefaultRunningAction((CRuFusion_Mime_Action)itemDB->MagicBase.MotionChangeID[i]);
					break;

				case 4:
					SetDefaultBackAction((CRuFusion_Mime_Action)itemDB->MagicBase.MotionChangeID[i]);
					break;

				case 5:
					break;

				case 6:
					break;
				}
			}
		}
	}

	EffectUpdate();	

	// 騎乘為唯一旗標,不能重複
	if ( itemDB->MagicBase.Effect.Ride ) {
		buff.mount = true;
		CreateMountSprite(itemDB->MagicBase.RideID);
	}

	// 騎乘可以攻擊
	if ( itemDB->MagicBase.Setting.RideCanAttack )
	{
		m_controlState.rideAttack = true;
	}

	// 騎乘使用玩家動作
	if ( itemDB->MagicBase.Setting.RidePetAttack )
	{
		m_controlState.ridePlayAttackMotion = true;
	}
	
	// 水上行走
	if ( itemDB->MagicBase.Effect.WaterWalk )
	{
		SetAboveWaterState(true);
	}

	// 縮放
	if ( itemDB->MagicBase.ModelSize != 1.0f ) {
		float scale = min(max(itemDB->MagicBase.ModelSize, 0.1f), 10.0f);

		// 將上一次有縮放狀態改變
		ClearController(eSpriteController_Scale);

		// 控制時間變化
		AddController(eSpriteController_Scale, 1.0f, m_appnedScale, m_appnedScale * scale);
	}
	
	m_buffInfo.push_back( buff );
	if ( s_procFunc )
		s_procFunc(this, eSpriteMsg_Buff_Changed, 0, 0);	
}

//-----------------------------------------------------------------------------
void CRoleSprite::DeleteBuff(int id, int buffOwnerID, bool immediately)
{
	if ( immediately == true )
		EraseBuff(id, buffOwnerID);
	else
	{
		// 找出要刪除的BUFF結構
		GameObjDbStructEx* itemDB = CNetGlobal::GetObj(id);
		if( itemDB == NULL || itemDB->IsMagicBase() == false )
			return;

		for ( vector<RoleBuffInfo>::iterator iter = m_buffInfo.begin(); iter != m_buffInfo.end(); iter++ )
		{			
			if( itemDB->MagicBase.Setting.SelfBuff == true && buffOwnerID != iter->ownerID )
				continue;

			if ( iter->id == id )
			{
				// 若時間小於1秒延遲刪除,不然立即刪除
				if ( iter->time > 0.0f && iter->time < 1.1f )
					iter->destructTime = iter->time;
				else
				{
					EraseBuff(id, buffOwnerID);
				}
				break;
			}
		}
	}
}

//-----------------------------------------------------------------------------
bool	CheckDisplayDotAttack( vector<RoleBuffInfo>& BuffList , GameObjDbStruct* AtkMagic )
{
	for( int i = 0 ; i < (int)BuffList.size() ; i++ )
	{   
		RoleBuffInfo* Buff = &(BuffList[i]);
		GameObjDbStructEx* MagicBaseObj = CNetGlobal::GetObj(Buff->id);
		if( MagicBaseObj == NULL )
			continue;

		GameObjectMagicStruct* MagicBase = &(MagicBaseObj->MagicBase);

		if(  MagicBase->Setting.MagicShield == false )
			continue;

		if(		AtkMagic == NULL 
			||	AtkMagic->MagicBase.EffectType == EM_Magic_Phy )
		{
			if(		MagicBase->MagicShield.Effect != EM_MagicShieldEffec_All 
				&&	MagicBase->MagicShield.Effect != EM_MagicShieldEffec_Phy )
				continue;
		}
		else if( AtkMagic->MagicBase.EffectType == EM_Magic_Magic )
		{
			switch( MagicBase->MagicShield.Effect )
			{
			case EM_MagicShieldEffec_All:			//所有攻擊
				break;
			case EM_MagicShieldEffec_Phy:			//物理攻擊
				continue;
			case EM_MagicShieldEffec_Magic:			//法術攻擊
				break;
			case EM_MagicShieldEffec_Earth:			//地系法術攻擊
				if( MagicBase->MagicType != EM_MagicResist_Earth )
					continue;
				break;
			case EM_MagicShieldEffec_Water:    		//水系法術攻擊
				if( MagicBase->MagicType != EM_MagicResist_Water )
					continue;
				break;
			case EM_MagicShieldEffec_Fire:     		//火系法術攻擊
				if( MagicBase->MagicType != EM_MagicResist_Fire )
					continue;
				break;
			case EM_MagicShieldEffec_Wind:     		//風系法術攻擊
				if( MagicBase->MagicType != EM_MagicResist_Wind )
					continue;
				break;
			case EM_MagicShieldEffec_Light:    		//光系法術攻擊
				if( MagicBase->MagicType != EM_MagicResist_Light )
					continue;
				break;
			case EM_MagicShieldEffec_Darkness: 		//暗系法術攻擊
				if( MagicBase->MagicType != EM_MagicResist_Darkness )
					continue;
				break;
			}
		}	
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
void CRoleSprite::UpdateBuff( float elapsedTime )
{
	// 刪除延遲刪除BUFF
	while ( true )
	{
		bool isBreak = true;

		vector<RoleBuffInfo>::iterator iter = m_buffInfo.begin();
		while ( iter != m_buffInfo.end() )
		{
			if ( iter->destructTime > 0.0f )
			{
				iter->destructTime -= elapsedTime;
				if ( iter->destructTime < 0.0f )
				{
					EraseBuff(iter->id, iter->ownerID);
					isBreak = false;
					break;
				}
			}
			iter++;
		}

		if ( isBreak )
			break;
	}

	for ( vector<RoleBuffInfo>::iterator iter = m_buffInfo.begin(); iter != m_buffInfo.end(); iter++ )
	{
		if ( iter->time > 0.0f )
		{
			iter->time -= elapsedTime;
			if ( iter->time < 0.0f )
			{
				iter->time = 0.0f;
				iter->destructTime = 1.0f;
			}
		}

		if ( iter->effect )
		{
			switch ( iter->effect->GetEffectMotionStep() )
			{
			case eEffectMotion_Step_Dead:
				iter->effect->SetDestructTime(0.5f);
				iter->effect = NULL;
				break;
			}
		}

		if ( iter->recycleTime != 0.0f )				// DOT類型
		{
			iter->remnantTime -= elapsedTime;
			if ( iter->remnantTime < 0.2f && iter->dotCount > 0 )
			{				
				GameObjDbStruct* itemDB = CNetGlobal::GetObj(iter->id);
				iter->remnantTime += iter->recycleTime;
				iter->dotCount--;
				if ( itemDB )
				{
					// 檢查是否要產生特效(非自己的不顯示)
					if ( itemDB->MagicBase.Setting.OnlyYouCanSee == false || iter->source == s_localPlayerId )
					{
						bool boShow = (s_hideEffectFunc) ? s_hideEffectFunc(m_id, 0) : false;
						if ( itemDB->MagicBase.Setting.forceDisplayEffect )
							boShow = true;

						GameObjDbStruct* imageDB = CNetGlobal::GetObj(itemDB->ImageID);
						if ( imageDB && boShow )
						{
							// 特效表現
							CEffectSprite* effect = new CEffectSprite;
							if ( effect->LoadRuEntity(imageDB->Image.BuffACT) )
							{
								float scale = imageDB->Image.ModelSize * imageDB->Image.ActRate[EM_ImageActType_BuffExtend] / 10000.0f;
								effect->SetScale(scale, eSpriteScale_World);
								effect->AttachSprite(this, GetAttachPointEnum(imageDB->Image.BuffPoint));
								effect->SetEffectMotionStep( eEffectMotion_Step_Explode );
							}
							else
							{
								delete effect;
							}
						}
					}

					// 數值計算
					DOTBuffValueInfo bvi;					
					int value = (int)(itemDB->MagicBase.DotMagic.Base * ( 1.0f + (itemDB->MagicBase.DotMagic.DotSkillLVArg * iter->magicLv / 100 )));

					// DOT衰減值
					if ( value < 0 ) {
						value = (int)(value * iter->dotRate);
						if ( itemDB->MagicBase.DotMagic.Type == EM_DotMagicType_HP )
							bvi.absorb = CheckDisplayDotAttack(m_buffInfo , itemDB);
						else
							bvi.absorb = false;
					} else {
						bvi.absorb = false;
					}
					
					bvi.sourceName	= iter->sourceName.c_str();
					bvi.source		= iter->source;
					bvi.id			= iter->id;
					bvi.type		= 0;
					bvi.value		= 0;

					switch (itemDB->MagicBase.DotMagic.Type)
					{
					case EM_DotMagicType_HP:
						DeductHP(value, false);
						bvi.type = 0;
						bvi.value = value;
						break;

					case EM_DotMagicType_MP:
					case EM_DotMagicType_WarriorSP:
					case EM_DotMagicType_RangerSP:
					case EM_DotMagicType_RogueSP:
						if ( itemDB->MagicBase.DotMagic.Type == EM_DotMagicType_MP )
							bvi.type = EM_MagicAttackType_MP;
						else if ( itemDB->MagicBase.DotMagic.Type == EM_DotMagicType_WarriorSP )
							bvi.type = EM_MagicAttackType_SP_Warrior;
						else if ( itemDB->MagicBase.DotMagic.Type == EM_DotMagicType_RangerSP )
							bvi.type = EM_MagicAttackType_SP_Ranger;
						else if ( itemDB->MagicBase.DotMagic.Type == EM_DotMagicType_RogueSP )
							bvi.type = EM_MagicAttackType_SP_Rogue;
						
						if ( GetManaType() == bvi.type )
							bvi.value = DeductMP(value, false);
						else if ( GetSkillType() == bvi.type )
							bvi.value = DeductSP(value, false);
						break;

					case EM_DotMagicType_HP_Per:
						bvi.type = 0;
						{
							double newValue = (double)m_maxHp * (double)value / 100.0;
							bvi.value = (int)newValue;
						}
						break;

					case EM_DotMagicType_MP_Per:
						{
							int maxMp = m_maxMp;
							switch ( m_vocationSub )
							{
							case EM_Vocation_Wizard:			// 法師
							case EM_Vocation_Priest:			// 僧侶
							case EM_Vocation_Knight:			// 騎士
							case EM_Vocation_RuneDancer:		// 
							case EM_Vocation_Druid:				//
								if ( m_maxSp > 0 )
									maxMp = m_maxSp;
								break;
							}

							bvi.type = EM_MagicAttackType_MP;
							double newValue = (double)maxMp * (double)value / 100.0;

							if ( GetManaType() == bvi.type )
								bvi.value = DeductMP((int)newValue, false);
							else if ( GetSkillType() == bvi.type )
								bvi.value = DeductSP((int)newValue, false);
						}
						break;
					}

					if ( itemDB->MagicBase.Setting.DisableShowMagicInfo == false )
					{
						// 傳出數值改變
						if (s_procFunc)
							s_procFunc(this, eSpriteMsg_Dot_Value, (INT64)(&bvi), 0);
					}
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
void CRoleSprite::UpdateBuffEffect()
{
	for ( vector<RoleBuffInfo>::iterator iter = m_buffInfo.begin(); iter != m_buffInfo.end(); iter++ )
	{		
		GameObjDbStruct* itemDB = CNetGlobal::GetObj(iter->id);
		if ( itemDB )
		{
			// DOT類
			if ( itemDB->MagicBase.Setting.DotMagic )
			{
			}
			else if ( iter->proxyLoad )
			{
				iter->proxyLoad = false;
				CreateBuffEffect(itemDB, &(*iter));
			}
		}
	}
}

//-----------------------------------------------------------------------------
void CRoleSprite::EraseBuff(int id, int BuffOwnerID)
{
	GameObjDbStructEx* itemDB = CNetGlobal::GetObj(id);
	if( itemDB == NULL || itemDB->IsMagicBase() == false )
		return;

	for ( vector<RoleBuffInfo>::iterator iter = m_buffInfo.begin(); iter != m_buffInfo.end(); iter++ )
	{			
		if( itemDB->MagicBase.Setting.SelfBuff == true && BuffOwnerID != iter->ownerID )
			continue;

		if ( iter->id == id )
		{
			// 讓特效自己結束並刪除
			CEffectSprite* effect = iter->effect;
			if ( effect )
			{
				effect->SetAutoDestruct(true);
				switch ( effect->GetEffectMotionStep() )
				{
				case eEffectMotion_Step_Appear:
				case eEffectMotion_Step_Idle:
					effect->SetEffectMotionStep(eEffectMotion_Step_Disappear);
					break;
				}
			}

			// 清除手臂特效
			if ( iter->armEffect )
			{
				iter->armEffect->DetachFromParent();
				ruSAFE_RELEASE(iter->armEffect);
			}
	
			if ( itemDB ) 
			{
				MagicEffectStruct &effect = itemDB->MagicBase.Effect;

				// 計算特殊效果旗標

				if ( effect.CliOutofContorl && m_specialEffect.cantCTRL > 0 )				// 混亂
					m_specialEffect.cantCTRL--;
				if ( effect.Stop && m_specialEffect.lockedMove > 0 )			// 鎖定移動
					m_specialEffect.lockedMove--;
				if ( effect.Blind && m_specialEffect.blind > 0 )				// 失明
					m_specialEffect.blind--;
				if ( effect.Client_Dizzy && m_specialEffect.dizzy > 0 )			// 暈眩
					m_specialEffect.dizzy--;
				if ( effect.Client_Sleep && m_specialEffect.sleep > 0 )			// 睡眠
					m_specialEffect.sleep--;
				if ( effect.Fear && m_specialEffect.fear > 0 )					// 恐懼
					m_specialEffect.fear--;
				if ( effect.Chaos && m_specialEffect.chaos > 0 )				// 混亂
					m_specialEffect.chaos--;
				if ( effect.HitDown && m_specialEffect.hitDown > 0 )			// 擊倒
					m_specialEffect.hitDown--;
				if ( effect.Cover && m_specialEffect.invisible > 0 )			// 隱形
					m_specialEffect.invisible--;
				if ( effect.DetectCover && m_specialEffect.detectInvisible > 0 )// 偵測隱形
					m_specialEffect.detectInvisible--;
				if ( effect.Sneak && m_specialEffect.sneak > 0 )				// 潛形
					m_specialEffect.sneak--;
				if ( effect.DetectSneak && m_specialEffect.detectSneak > 0 )	// 偵測潛形
					m_specialEffect.detectSneak--;
				if ( effect.PKFlag && m_specialEffect.pvp > 0 )					// pvp
				{
					m_specialEffect.pvp--;
					m_isNeedResetWidget = true;
				}
				if ( effect.PKFlag_Disabled && m_specialEffect.protect > 0 )	// 保護PK
				{
					m_specialEffect.protect--;
					m_isNeedResetWidget = true;
				}
				if ( effect.Freeze && m_specialEffect.freeze > 0 )				// 冰凍
					m_specialEffect.freeze--;
				if ( effect.Guilty && m_specialEffect.criminal > 0 )			// 罪犯
					m_specialEffect.criminal--;
				if ( effect.DisableJobSkill && m_specialEffect.disableJobSkill > 0 )	// 無法使用本職業技能
					m_specialEffect.disableJobSkill--;
				if ( effect.GravityDisable )									// 暫時無重力
				{
					SetGravityMode(m_controlState.gravityBackup);
				}

				if ( itemDB->MagicBase.Effect.ChangeMotion )
				{
					if ( m_pRuActor )
					{
						for ( int i = 0; i < 4; i++ )
						{
							switch ( itemDB->MagicBase.MotionChangeType[i] )
							{
							case 1:
								SetDefaultWalkingAction(ruFUSION_MIME_WALK_FORWARD);
								break;

							case 2:
								SetDefaultBackAction(ruFUSION_MIME_RUN_BACKWARD);
								break;

							case 3:
								SetDefaultRunningAction(ruFUSION_MIME_RUN_FORWARD);
								break;

							case 4:
								SetDefaultBackAction(ruFUSION_MIME_RUN_BACKWARD);
								break;

							case 5:
								break;

							case 6:
								break;
							}
						}
					}
				}

				EffectUpdate();

				// 變身術
				if ( iter->faceOff ) {
					iter->faceOff = false;
					ReleaseTempEntity();

					// 查詢是否還有變身術
					for ( vector<RoleBuffInfo>::reverse_iterator buffIter = m_buffInfo.rbegin(); buffIter != m_buffInfo.rend(); buffIter++ )
					{
						if ( buffIter->id == iter->id && buffIter->ownerID == iter->ownerID )
							continue;

						if ( buffIter->faceOff == true )
						{
							GameObjDbStructEx* magicDB = CNetGlobal::GetObj(buffIter->id);
							if ( magicDB )
							{
								SetTempObjectId(magicDB->MagicBase.FaceOffID, magicDB->MagicBase.Setting.FaceOffMagic_ShowEq);
							}
							break;
						}
					}
				}

				// 騎乘術
				if ( effect.Ride ) {
					ReleaseMountSprite();
				}

				// 水上行走
				if ( effect.WaterWalk )
				{
					SetAboveWaterState(false);
				}

				// 騎乘可以攻擊
				if ( itemDB->MagicBase.Setting.RideCanAttack ) {
					m_controlState.rideAttack = false;
				}

				// 騎乘使用玩家動作
				if ( itemDB->MagicBase.Setting.RidePetAttack ) {
					m_controlState.ridePlayAttackMotion = false;
				}

				// 縮放
				if ( itemDB->MagicBase.ModelSize != 1.0f ) {
					float scale = min(max(itemDB->MagicBase.ModelSize, 0.1f), 10.0f);
					ClearController(eSpriteController_Scale);
					AddController(eSpriteController_Scale, 1.0f, m_appnedScale, m_appnedScale/scale);
				}
				
				m_buffInfo.erase( iter );
			}

			if ( s_procFunc )
				s_procFunc(this, eSpriteMsg_Buff_Changed, 0, 0);
			return;
		}
	}
}

//-----------------------------------------------------------------------------
RoleBuffInfo* CRoleSprite::GetBuffInfo(int index)
{
	if ( index >= 0 && index < (int)m_buffInfo.size() )
		return &(m_buffInfo[index]);
	return NULL;
}

//-----------------------------------------------------------------------------
int	CRoleSprite::GetBuffHelpfulIndex(int order, bool getHide)
{	
	int i = 0;
	for ( vector<RoleBuffInfo>::iterator iter = m_buffInfo.begin(); iter != m_buffInfo.end(); i++, iter++ )
	{
		GameObjDbStruct* itemDB = CNetGlobal::GetObj(iter->id);
		if ( itemDB &&
			 itemDB->MagicBase.Setting.GoodMagic &&
			 (itemDB->MagicBase.Setting.ShowBuf || getHide) )
		{
			if ( order == 0 )
				return i;
			else
				order--;
		}
	}
	return -1;
}

//-----------------------------------------------------------------------------
int CRoleSprite::GetBuffHarmfulIndex(int order, bool getHide)
{
	int i = 0;
	for ( vector<RoleBuffInfo>::iterator iter = m_buffInfo.begin(); iter != m_buffInfo.end(); i++, iter++ )
	{
		GameObjDbStruct* itemDB = CNetGlobal::GetObj(iter->id);
		if ( itemDB &&
			 !itemDB->MagicBase.Setting.GoodMagic &&
			 (itemDB->MagicBase.Setting.ShowBuf || getHide) )
		{
			if ( order == 0 )
				return i;
			else
				order--;
		}
	}
	return -1;
}

//-----------------------------------------------------------------------------
void CRoleSprite::ClearBuff()
{
	for ( vector<RoleBuffInfo>::iterator iter = m_buffInfo.begin(); iter != m_buffInfo.end(); iter++ )
	{		
		GameObjDbStruct* itemDB = CNetGlobal::GetObj(iter->id);

		if ( itemDB )
		{
			// 死亡不清除
			if ( itemDB->MagicBase.Setting.DeadNotClear )
			{
			}

			// 縮放
			if ( itemDB->MagicBase.ModelSize != 1.0f ) {
				float scale = min(max(itemDB->MagicBase.ModelSize, 0.1f), 10.0f);
				ClearController(eSpriteController_Scale);
				AddController(eSpriteController_Scale, 1.0f, m_appnedScale, m_appnedScale/scale);
			}

			// 改變重力
			if ( itemDB->MagicBase.Effect.GravityDisable )
				SetGravityMode(m_controlState.gravityBackup);
		}		

		// 變身術
		if ( iter->faceOff ) {
			iter->faceOff = false;
			ReleaseTempEntity();
		}					

		// 清除BUFF特效
		if ( iter->effect && IBaseSprite::CheckedSpriteIsLife(iter->effect) )
			delete iter->effect;
		iter->effect = NULL;

		// 手臂特效
		if ( iter->armEffect )
		{
			iter->armEffect->DetachFromParent();
			ruSAFE_RELEASE(iter->armEffect);
		}
	}
	m_buffInfo.clear();

	// 騎乘術
	ReleaseMountSprite();

	// 計算特殊效果旗標
	memset(&m_specialEffect, 0, sizeof(m_specialEffect));
	EffectUpdate();

	if ( s_procFunc )
		s_procFunc(this, eSpriteMsg_Buff_Changed, 0, 0);
}

//-----------------------------------------------------------------------------
void CRoleSprite::AddDisplayBuff(int magicId, int magicLv, float time)
{
	RoleBuffInfo info;

	info.id				= magicId;
	info.destructTime	= 0.0f;
	info.magicLv		= magicLv;
	info.time			= time;	

	m_buffInfo.push_back(info);

	if ( s_procFunc )
		s_procFunc(this, eSpriteMsg_Buff_Changed, 0, 0);
}

//-----------------------------------------------------------------------------
void CRoleSprite::ClearDisplayBuff()
{
	m_buffInfo.clear();

	if ( s_procFunc )
		s_procFunc(this, eSpriteMsg_Buff_Changed, 0, 0);
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetTargetSprite(CRoleSprite* pTarget)
{
	if (m_targetSprite != pTarget)
	{
		CRoleSprite* oldTarget = m_targetSprite;

		m_targetSprite = pTarget;
		if (s_procFunc)
			s_procFunc(this, eSpriteMsg_Target_Changed, (INT64)m_targetSprite, (INT64)oldTarget);

		if (m_targetSprite == NULL)
		{
			if ( GetCombatState() )
				SetCombatState(false);
		}
	}
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetMasterSprite(CRoleSprite* pMaster, bool isPlayerPet)
{
	m_controlState.playerPet = isPlayerPet;
	if ( isPlayerPet && pMaster )
		pMaster->SetPetSprite(this);

	if ( m_masterSprite != pMaster )
	{
		if ( m_masterSprite )
			m_masterSprite->RemoveChildSprite(this);

		m_masterSprite = pMaster;

		if (s_procFunc && isPlayerPet)
			s_procFunc(this, eSpriteMsg_Pet_Changed, (INT64)m_masterSprite, 0);

		if ( m_masterSprite )
			m_masterSprite->InsertChildSprite(this);

		m_isNeedResetWidget = true;
	}
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetPetSprite(CRoleSprite* pPet)
{
	if ( m_petSprite != pPet )
	{
		m_petSprite = pPet;

		if (s_procFunc && m_petSprite == NULL )
			s_procFunc(this, eSpriteMsg_Pet_Changed, (INT64)m_petSprite, 0);
	}
}

//-----------------------------------------------------------------------------
CRoleSprite* CRoleSprite::GetPetSprite()
{
	// 如果有載具，寵物資訊換為載具
	if ( m_implementSprite )
		return m_implementSprite;
	return m_petSprite;
}

//-----------------------------------------------------------------------------
void CRoleSprite::InsertChildSprite(CRoleSprite* pChild)
{
	if ( pChild == NULL )
		return;

	m_childSprite.insert(pChild);	
}

//-----------------------------------------------------------------------------
void CRoleSprite::RemoveChildSprite(CRoleSprite* pChild)
{
	if ( pChild == NULL )
		return;

	set<CRoleSprite*>::iterator iter = m_childSprite.find(pChild);
	if ( iter != m_childSprite.end() )
	{
		m_childSprite.erase(iter);
	}
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetName( const char* nameStr, bool showWidget )
{
	if ( nameStr && m_nameStr && strcmp(nameStr, m_nameStr) == 0 )
		return;

	ruSAFE_DELETE(m_nameStr);

	if ( nameStr )
	{
		int iSize = (int)strlen(nameStr) + 1;
		m_nameStr = new char[ iSize ];
		strcpy(m_nameStr, nameStr);
	}

	if ( showWidget ) {
		m_isNeedResetWidget = true;
	}
}

//-----------------------------------------------------------------------------
const char* CRoleSprite::GetName()
{
	static const char* static_empty_str = "";

	if ( m_originalName.empty() == false )
		return m_originalName.c_str();

	if ( m_nameStr )
		return m_nameStr;

	return static_empty_str;
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetOriginalName( const char* nameStr )
{
	m_originalName.clear();

	if ( nameStr )
		m_originalName = nameStr;
}

//-----------------------------------------------------------------------------
const char*	CRoleSprite::GetOriginalName()
{
	return m_originalName.c_str();
}
const char*  CRoleSprite::GetTitleText()
{

	return m_titleStr;

}
//-----------------------------------------------------------------------------
void CRoleSprite::SetTitleText(const char* titleStr)
{
	if ( titleStr && m_titleStr && strcmp(titleStr, m_titleStr) == 0 )
		return;

	ruSAFE_DELETE(m_titleStr);

	if ( titleStr )
	{
		m_titleStr = new char[strlen(titleStr) + 1];
		strcpy(m_titleStr, titleStr);
	}

	m_isNeedResetWidget = true;
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetGuildText(const char* guildStr)
{
	if ( guildStr && m_guildStr && strcmp(guildStr, m_guildStr) == 0 )
		return;

	ruSAFE_DELETE(m_guildStr);

	if ( guildStr )
	{
		m_guildStr = new char[strlen(guildStr) + 1];
		strcpy(m_guildStr, guildStr);
	}

	m_isNeedResetWidget = true;
}
const char*		CRoleSprite::GetGuildText()
{
	return m_guildStr;

}
//-----------------------------------------------------------------------------
void CRoleSprite::SetWidgetTexture()
{
	if ( m_pRuWidget == NULL )
	{
		if ( (m_pRuWidget = ruNEW CRuEntity_Widget_Label()) == NULL )
			return;

//		m_pRuWidget->SetTexture( CRuFontTexture::GetRuTextureD3D() );

		// Update widget and make sure it does not appear in shadow maps
		if (m_pRuWidget)
		{
			RuEntity_SetOption(m_pRuWidget, ruENTITYOPTION_CASTERCLASS, 0);
		}
	}	

	m_nameColor = 0xFFFFFFFF;
	m_pRuWidget->ClearAllGlyphs();
	m_pRuWidget->SetLabelEffect(CRuEntity_Widget_Label::ruLABELEFFECT_BORDER);

	// 設定旗標顏色
	if ( GetEnemyState() ) {
		m_nameColor = 0xFFED1C24;
	} else if ( GetCanAttackState() ) {
		m_nameColor = 0xFFFFF568;
	} else if ( GetPartyState() ) {
		m_nameColor = 0xFF46B4FF;
	} else {
		if ( m_objectType == eSpriteObject_Player ) {
			m_nameColor = 0xFFB4E1EE;
		} else {
			m_nameColor = 0xFF00FF00;
		}
	}
	
	bool titleVisible = false;
	int count = 0;
	float width[4] = { 32.0f, 32.0f, 32.0f, 32.0f };

	// 團隊標記
	if ( m_raidMarkNumber != -1 )
	{
		// Class Icon		
		WidgetAddRaidMarkIcon(0);
	}	

	// 稱號
	if ( GetTitleVisible() && m_titleStr && m_titleStr[0] )
	{		
		titleVisible = true;
		WCHAR* tmp = MultiByteToWChar(m_titleStr);
		width[count++] = WidgetAddRowText(tmp, 26.0f * s_fontScale, m_nameColor);
		delete [] tmp;
	}	

	// Name 
	if ( m_nameStr && m_nameStr[0] )
	{
		if ( titleVisible )
			width[count++] = WidgetAddRowText(L" ", 12.0f, 0xFFFFFFFF);

		WCHAR* tmp = MultiByteToWChar(m_nameStr);
		if ( m_vocation == EM_Vocation_GameMaster && GetObjectType() == eSpriteObject_Player )
			width[count++] = WidgetAddRowText(tmp, 46.0f * s_fontScale, m_nameColor);
		else
			width[count++] = WidgetAddRowText(tmp, 32.0f * s_fontScale, m_nameColor);
		delete [] tmp;
	}

	// Title
	if ( m_guildStr && m_guildStr[0] && GetGuildVisible() )
	{
		char buf[256];
		sprintf(buf, "[%s]", m_guildStr);
		WCHAR* tmp = MultiByteToWChar(buf);
		width[count++] = WidgetAddRowText(tmp, 26.0f * s_fontScale, m_nameColor);
		delete [] tmp;
	}
	
	if ( m_objectType == eSpriteObject_Player && GetTitleIconVisible() )
	{
		int row = max(m_pRuWidget->GetRowCount() - 1, 0);

		float maxWidth = max(max(width[0], width[1]), width[2]);

		float offset = max(maxWidth - width[row], 0.0f);		

		// Class Icon
		WidgetAddClassIcon(row, offset / 2.0f + 12.0f);

		// GoodEvil Icon
		WidgetAddGoodEvilIcon(row, offset / 2.0f + 12.0f);
	}

	m_pRuWidget->SetTranslation(CRuVector3(0.0f, -2.5f, 0.0f));
	m_pRuWidget->Update(0.0f);
	UpdateTitleVisible();

	if ( s_procFunc )
		s_procFunc(this, eSpriteMsg_Relation_Changed, 0, 0);
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetVocation(Voc_ENUM main, Voc_ENUM sub)
{
	if (main != m_vocation || sub != m_vocationSub)
	{
		m_vocation = main;
		m_vocationSub = sub;

		if ( s_procFunc )
			s_procFunc(this, eSpriteMsg_Vocation_Changed, 0, 0);

		m_isNeedResetWidget = true;
	}
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetHP( int hp )
{
	hp = min(max(0, hp), m_maxHp);
	if (m_hp != hp)
	{
		m_lastHp = m_hp;
		m_hp = hp;
		if ( s_procFunc )
			s_procFunc(this, eSpriteMsg_HP_Changed, m_hp, 0);
	}
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetMaxHP( int maxHp )
{
	maxHp = max(0, maxHp);
	if (m_maxHp != maxHp)
	{
		m_maxHp = maxHp;
		SetHP(m_hp);
		if ( s_procFunc )
			s_procFunc( this, eSpriteMsg_MaxHP_Changed, m_maxHp, 0 );
	}
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetMP( int mp )
{
	mp = min(max(0, mp), m_maxMp);
	if (m_mp != mp)
	{
		m_mp = mp;
		if ( s_procFunc )
			s_procFunc( this, eSpriteMsg_MP_Changed, m_mp, 0 );
	}
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetMaxMP( int maxMp )
{
	maxMp = max(0, maxMp);
	if ( m_maxMp != maxMp )
	{
		m_maxMp = maxMp;
		SetMP(m_mp);
		if ( s_procFunc )
			s_procFunc( this, eSpriteMsg_MaxMP_Changed, m_maxMp, 0 );
	}
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetSP( int sp )
{
	sp = min(max(0, sp), m_maxSp);
	if (m_sp != sp)
	{
		m_sp = sp;
		if ( s_procFunc )
			s_procFunc( this, eSpriteMsg_SP_Changed, m_sp, 0 );
	}
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetMaxSP( int maxSp )
{
	maxSp = max(0, maxSp);
	if ( m_maxSp != maxSp )
	{
		m_maxSp = maxSp;
		SetSP(m_sp);
		if ( s_procFunc )
			s_procFunc( this, eSpriteMsg_MaxSP_Changed, m_maxSp, 0 );
	}
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetLevel(int main, int sub)
{
	if (m_level != main || m_levelSub != sub)
	{
		m_level = main;
		m_levelSub = sub;
		if ( s_procFunc )
			s_procFunc(this, eSpriteMsg_Level_Changed, m_level, m_levelSub);
	}
}

//-----------------------------------------------------------------------------
int CRoleSprite::DeductHP(int hp, bool boUpdate)
{
	int oldValue = m_hp;
	if ( boUpdate )
	{
		SetHP(oldValue + hp);
		return m_hp - oldValue;
	}
	else
	{
		return min(max(0, oldValue + hp), m_maxHp) - oldValue;
	}
}

//-----------------------------------------------------------------------------
int CRoleSprite::DeductMP(int mp, bool boUpdate)
{
	int oldValue = m_mp;
	if ( boUpdate )
	{
		SetMP(oldValue + mp);
		return m_mp - oldValue;
	}
	else
	{
		return min(max(0, oldValue + mp), m_maxMp) - oldValue;
	}

	/*
	int oldValue;
	switch(m_vocation)
	{
	case EM_Vocation_Wizard:
	case EM_Vocation_Priest:
	case EM_Vocation_Knight:
	case EM_Vocation_RuneDancer:
	case EM_Vocation_Druid:
		oldValue = m_mp;
		if ( boUpdate )
		{
			SetMP(oldValue + mp);
			return m_mp - oldValue;
		}
		else
		{
			return min(max(0, oldValue + mp), m_maxMp) - oldValue;
		}
		break;

	default:
		switch(m_vocationSub)
		{
		case EM_Vocation_Wizard:
		case EM_Vocation_Priest:
		case EM_Vocation_Knight:
		case EM_Vocation_RuneDancer:
		case EM_Vocation_Druid:
			oldValue = m_sp;
			if ( boUpdate )
			{
				SetSP(oldValue + mp);
				return m_sp - oldValue;
			}
			else
			{
				return min(max(0, oldValue + mp), m_maxSp) - oldValue;
			}
		}
		break;
	}
	*/

	return 0;
}

//-----------------------------------------------------------------------------
int CRoleSprite::DeductSP(int sp, bool boUpdate)
{
	int oldValue = m_sp;
	if ( boUpdate )
	{
		SetSP(oldValue + sp);
		return m_sp - oldValue;
	}
	else
	{
		return min(max(0, oldValue + sp), m_maxSp) - oldValue;
	}

	/*
	int oldValue;
	switch(m_vocation)
	{
	case EM_Vocation_Warrior:
	case EM_Vocation_Ranger:
	case EM_Vocation_Rogue:
	case EM_Vocation_Harpsyn:
	case EM_Vocation_Psyron:
		oldValue = m_mp;
		if ( boUpdate )
		{
			SetMP(oldValue + sp);
			return m_mp - oldValue;
		}
		else
		{
			return min(max(0, oldValue + sp), m_maxMp) - oldValue;
		}
		break;

	default:
		switch(m_vocationSub)
		{
		case EM_Vocation_Warrior:
		case EM_Vocation_Ranger:
		case EM_Vocation_Rogue:
		case EM_Vocation_Harpsyn:
		case EM_Vocation_Psyron:
			oldValue = m_sp;
			if ( boUpdate )
			{
				SetSP(oldValue + sp);
				return m_sp - oldValue;
			}
			else
			{
				return min(max(0, oldValue + sp), m_maxSp) - oldValue;
			}
		}
		break;
	}
	return 0;
	*/
}

//-----------------------------------------------------------------------------
int	CRoleSprite::GetManaType()
{
	switch (GetVocation())
	{
	case EM_Vocation_Wizard:			// 法師
	case EM_Vocation_Priest:			// 僧侶
	case EM_Vocation_RuneDancer:		// 
	case EM_Vocation_Druid:
		return EM_MagicAttackType_MP;

	case EM_Vocation_Warrior:			// 戰士		
	case EM_Vocation_Psyron:
		return EM_MagicAttackType_SP_Warrior;

	case EM_Vocation_Ranger:			// 遊俠					
	case EM_Vocation_Harpsyn:		
		return EM_MagicAttackType_SP_Ranger;	

	case EM_Vocation_Rogue:				// 盜賊
	case EM_Vocation_Duelist:			// 鬥棋士
		return EM_MagicAttackType_SP_Rogue;
	}

	return 1;
}

//-----------------------------------------------------------------------------
int	CRoleSprite::GetSkillType()
{
	switch (GetVocationSub())
	{
	case EM_Vocation_Wizard:			// 法師
	case EM_Vocation_Priest:			// 僧侶
	case EM_Vocation_RuneDancer:		// 
	case EM_Vocation_Druid:
		return EM_MagicAttackType_MP;

	case EM_Vocation_Warrior:			// 戰士
	case EM_Vocation_Psyron:
		return EM_MagicAttackType_SP_Warrior;

	case EM_Vocation_Ranger:			// 遊俠			
	case EM_Vocation_Harpsyn:
		return EM_MagicAttackType_SP_Ranger;

	case EM_Vocation_Rogue:				// 盜賊
	case EM_Vocation_Duelist:			// 鬥棋士
		return EM_MagicAttackType_SP_Rogue;
	}
	return 1;
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetQuestInfo( int id, int touch, int area )
{
	m_questId		= id;
	m_touchRange	= touch;
	m_areaRange		= area;
}

//-----------------------------------------------------------------------------
int	CRoleSprite::GetTouchRange()
{
	if ( m_objectType == eSpriteObject_Player )
		return 0;
	return m_touchRange;
}

//-----------------------------------------------------------------------------
int	CRoleSprite::GetAreaRange()
{
	if ( m_objectType == eSpriteObject_Player )
		return 0;
	return m_areaRange;
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetQuestDisplay( QuestDisplay display )
{
	if( display != m_questDisplay )
	{
		const char* str = NULL;
		ReleaseQuestEntity();

		
		switch ( display )
		{
		case eQuest_Display_Null:
			return;
		case eQuest_Display_AvailableQuest:
			//str = "model\\item\\ui_object\\vendor_icon\\act_vandor_0.ros";
			str = "model\\item\\ui_object\\quest\\act_quest_get.ros";
			break;
		case eQuest_Display_AvailableRepeatQuest:
			//str = "model\\item\\ui_object\\vendor_icon\\act_vandor_1.ros";
			str = "model\\item\\ui_object\\quest\\act_quest_get_blue.ros";
			break;
		case eQuest_Display_Get_Gary:
			//str = "model\\item\\ui_object\\vendor_icon\\act_vandor_2.ros";
			str = "model\\item\\ui_object\\quest\\act_quest_get_gray.ros";
			break;
		case eQuest_Display_UnfinishedQuest:
			//str = "model\\item\\ui_object\\vendor_icon\\act_vandor_3.ros";
			str = "model\\item\\ui_object\\quest\\act_quest_doing.ros";
			break;
		case eQuest_Display_FinishedQuest:
			//str = "model\\item\\ui_object\\vendor_icon\\act_vandor_4.ros";
			str = "model\\item\\ui_object\\quest\\act_quest_done.ros";
			break;
		case eQuest_Display_FinishedRepeatQuest:
			//str = "model\\item\\ui_object\\vendor_icon\\act_vandor_5.ros";
			str = "model\\item\\ui_object\\quest\\act_quest_repeat.ros";
			break;

		case eQuest_Display_Public_AvailableQuest:
			//str = "model\\item\\ui_object\\vendor_icon\\act_vandor_4.ros";
			str = "model\\item\\ui_object\\quest\\act_quest_get_Pink.ros";
			break;
		case eQuest_Display_Public_FinishedQuest:
			//str = "model\\item\\ui_object\\vendor_icon\\act_vandor_5.ros";
			str = "model\\item\\ui_object\\quest\\act_quest_done_Pink.ros";
			break;

		}

		if ( str )
		{
#ifdef KALYDO
			// this needs to be delay loaded, so the game can download it whenever it needs it.
			if ( g_ruResourceManager->LoadEntity(str, &m_pRuQuestEntity) == TRUE && m_pRuQuestEntity )
#else
			if ( m_pRuQuestEntity = g_ruResourceManager->LoadEntity(str) )
#endif // KALYDO
			{
				m_pRuQuestEntity->SetTranslation(CRuVector3(0.0f, 2.0f, 0.0f));
				m_pRuQuestEntity->Update(0.0f);
				RuACT_PlayMotion(m_pRuQuestEntity, "idle");
				AddOnEntity(true, m_pRuQuestEntity, eAttach_Name_Title);

				// 關閉影子
				RuEntity_SetOptionWithTypeFilter(m_pRuQuestEntity, ruENTITYOPTION_CASTERCLASS, 0, CRuModel::Type());
				RuEntity_SetOptionWithTypeFilter(m_pRuQuestEntity, ruENTITYOPTION_CASTERCLASS, 0, CRuCompositeModel::Type());

				m_questDisplay = display;
			}
		}
	}
}

//-----------------------------------------------------------------------------
bool CRoleSprite::HasQuestEvent()
{
	if ( m_touchRange != 0 )
		return true;

	switch (m_checkedStatus)
	{
	case eChecked_TouchPlot:
	case eChecked_RangePlot:
	case eChecked_QuestNPC:
	case eChecked_QuestObj:
	case eChecked_Mine:
	case eChecked_Herb:
	case eChecked_Lumber:
	case eChecked_Flag:
	case eChecked_Shop:
	case eChecked_Mail:
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
bool CRoleSprite::CheckTouchRange(float distance)
{
	if ( GetCorpseState() )
	{
		return 18.0f > distance;
	}

	if ( GetPriorityAttack() && GetCanAttackState() )
	{
		float range = max(GetAttackRange(), 10.0f) + 10.0f;
		return range > distance;
	}

	if ( m_touchRange != 0 )
	{
		return (float)m_touchRange > distance;
	}

	if ( GetCanAttackState() )
	{
		float range = max(GetAttackRange(), 10.0f) + 10.0f;
		return range > distance;
	}

	if ( GetImplementMaxNum() > GetImplementCount() )
	{
		return distance < MAX_IMPLEMENT_TOUCH_RANGE;
	}

	float range = max(m_attackRange, _MAX_ROLE_TOUCH_RANGE_);

	switch (m_checkedStatus)
	{
	case eChecked_TouchPlot:
	case eChecked_RangePlot:
	case eChecked_QuestNPC:
	case eChecked_QuestObj:
	case eChecked_Mine:
	case eChecked_Herb:
	case eChecked_Lumber:
	case eChecked_Flag:
	case eChecked_Shop:
	case eChecked_Mail:
	case eChecked_FurnitureNormal:  // 一般傢俱 
	case eChecked_FurnitureSound:   // 傢俱 留聲機
 	case eChecked_FurnitureStorage: // 傢俱 寶箱 衣架
  	case eChecked_Maid:		        // 女僕
	case eChecked_Servant:		    // 女僕
   	case eChecked_GuildHousesBuildPoint:	// 公會屋參考點
 		return range > distance;
	}

	return false;
}

//-----------------------------------------------------------------------------
void CRoleSprite::ReleaseEntity()
{
	m_paperdollColor.clear();	

	ruSAFE_RELEASE( m_pRuGlobalAlpha );

	ReleasFusionMime();
	ReleasFusionActor();	
	//ruSAFE_RELEASE( m_pRuMimeEntity );
	//ruSAFE_RELEASE( m_pRuActor );	
	
	ReleaseHandsEntity(eRoleHands_MainHand);
	ReleaseHandsEntity(eRoleHands_OffHand);
	ReleaseHandsEntity(eRoleHands_Ranged);
	ReleaseHandsEntity(eRoleHands_Crafting);
	ReleaseHandsEntity(eRoleHands_Arrow);

	memset(m_handsObjectId, 0, sizeof(m_handsObjectId));

	ReleaseWidget();

	ReleaseQuestEntity();

	IBaseSprite::ReleaseEntity();
}

#ifdef KALYDO
void CRoleSprite::KalydoLoadHandsCallback(const char* fileName, TKResult result, void* id)
{
	if (KRF_FAILURE(result))
	{
		// don't try again; this is done in the resource manager.
		//krfRequestKCPFile(fileName, &KalydoLoadHandsCallback, id);
		return;
	}

	CDownloadHandStruct* ds = reinterpret_cast<CDownloadHandStruct*>(id);

	if (!ds->spriteDeleted)
	{
		if (!ds->enchanted)
		{
			ds->sprite->m_handsEntity[ds->type].entity = g_ruResourceManager->LoadEntity(static_cast<const char*>(ds->fileName));
			if ( ds->sprite->m_handsEntity[ds->type].entity ) {
				// 裝備等級
				if ( RuEntity_FindLinkFrame(ds->sprite->m_handsEntity[ds->type].entity, "enfx01") && RuEntity_FindLinkFrame(ds->sprite->m_handsEntity[ds->type].entity, "enfx02") ) {
					ds->fileName = g_ObjectData->GetEquipRefineFilename(ds->level);
					ds->enchanted = true;
					TKResult res = g_ruResourceManager->DownloadEntityAndDependencies(static_cast<const char*>(ds->fileName), &KalydoLoadHandsCallback, ds);
					if (KRF_SUCCESS(res))
						// reuse ds struct
						return;
				}
			}
		}
		else
		{
			ds->sprite->m_handsEntity[ds->type].enchantEntity = g_ruResourceManager->LoadEntity(ds->fileName);
			if ( ds->sprite->m_handsEntity[ds->type].enchantEntity && ds->sprite->m_handsEntity[ds->type].entity ) {
				RuACT_PlayMotion(ds->sprite->m_handsEntity[ds->type].enchantEntity, "idle");
				ds->sprite->m_handsEntity[ds->type].entity->AddChild(ds->sprite->m_handsEntity[ds->type].enchantEntity);
			}
		}

		ds->sprite->CreateHandsEntityKalydo(ds->type, ds->id, ds->level, ds->point.GetLength() == 0 ? NULL : static_cast<const char*>(ds->point));

		for(vector<CDownloadHandStruct*>::iterator i = ds->sprite->m_DownloadStructs.begin(); i != ds->sprite->m_DownloadStructs.end(); i++)
		{
			if ((*i) == ds)
			{
				ds->sprite->m_DownloadStructs.erase(i);
				break;
			}
		}
	}

	
	delete ds;
}

void CRoleSprite::CreateHandsEntity(RoleHandsType type, int id, int level, const char* point)
{
	ReleaseHandsEntity(type);	

	if ( type != eRoleHands_None )
	{
		GameObjDbStruct* imageDB;
		GameObjDbStruct* pObj = CNetGlobal::GetObj(id);
		if ( pObj && (imageDB = CNetGlobal::GetObj(pObj->ImageID)) != NULL )
		{
			CDownloadHandStruct* ds = new CDownloadHandStruct();
			ds->sprite = this;
			ds->level = level;
			ds->type = type;
			ds->id = id;
			ds->fileName = imageDB->Image.ACTWorld;
			ds->enchanted = false;
			ds->point = point;
			ds->spriteDeleted = false;

			// in stead of calling krfRequestKCPFile, we need this entity and all its dependencies (lots of weapons are ACT templates,
			// and if the loading fails of its dependencies, then the weapons are not shown)
			TKResult res = g_ruResourceManager->DownloadEntityAndDependencies(imageDB->Image.ACTWorld, &KalydoLoadHandsCallback, ds);
			if (KRF_SUCCESS(res))
			{
				m_DownloadStructs.push_back(ds);

				// return if success; everything has been called.
				return;
			}

			delete ds;
		}
	}

	CreateHandsEntityKalydo(type, id, level, point);
}

//-----------------------------------------------------------------------------
// rename the CreateHandsEntity so everything can be downloaded beforehand.
void CRoleSprite::CreateHandsEntityKalydo(RoleHandsType type, int id, int level, const char* point)
#else // KALYDO
void CRoleSprite::CreateHandsEntity(RoleHandsType type, int id, int level, const char* point)
#endif // KALYDO
{	
	/*
	if ( m_handsEntity[type].id == id && m_handsEntity[type].type == type )
		return;
	*/

	ReleaseHandsEntity(type);	
	m_handsEntity[type].id = id;
	m_handsEntity[type].type = type;
	if ( m_handsEntity[type].type != eRoleHands_None )
	{
		GameObjDbStruct* imageDB;
		GameObjDbStruct* pObj = CNetGlobal::GetObj(id);
		if ( pObj && (imageDB = CNetGlobal::GetObj(pObj->ImageID)) != NULL )
		{
			m_handsEntity[type].entity = g_ruResourceManager->LoadEntity(imageDB->Image.ACTWorld);

			if ( m_handsEntity[type].entity ) {
				// 裝備等級
				if ( RuEntity_FindLinkFrame(m_handsEntity[type].entity, "enfx01") && RuEntity_FindLinkFrame(m_handsEntity[type].entity, "enfx02") ) {
					m_handsEntity[type].enchantEntity = g_ruResourceManager->LoadEntity(g_ObjectData->GetEquipRefineFilename(level));
					if ( m_handsEntity[type].enchantEntity ) {
						RuACT_PlayMotion(m_handsEntity[type].enchantEntity, "idle");
						m_handsEntity[type].entity->AddChild(m_handsEntity[type].enchantEntity);
					}
				}
			}
		}

		switch (type)
		{
		// 主手
		case eRoleHands_MainHand:
			SetWeaponPosition(GetUnholsterState(), true);
			break;

		// 副手
		case eRoleHands_OffHand:
			SetWeaponPosition(GetUnholsterState(), true);
			break;

		// 次要手持物品
		case eRoleHands_Ranged:
			// ReleaseHandsEntity(eRoleHands_Arrow);		// 清除彈藥
			ReleaseHandsEntity(eRoleHands_Crafting);
			SetWeaponPosition(false, true);
			switch ( GetTakeItemType(id) )
			{
			case eRoleTakeItem_Bow:				
				AttachHandsEntity(type, eAttach_Left_Hand);				
				break;

			case eRoleTakeItem_CossBow:
			case eRoleTakeItem_Gun:
				AttachHandsEntity(type, eAttach_Right_Hand);
				break;

			default:
				AttachHandsEntity(type, eAttach_Null);
				break;
			}
			break;		

		case eRoleHands_Crafting:
			ReleaseHandsEntity(eRoleHands_Arrow);			// 清除彈藥
			ReleaseHandsEntity(eRoleHands_Ranged);			// 清除遠程武器

			// 測試參考點位置
			{
				CRuEntity* linkFrame = NULL;
				if ( point )
					linkFrame = RuEntity_FindLinkFrame(m_handsEntity[type].entity, point);

				if ( linkFrame )
					AttachHandsEntity(type, (SpriteAttachPoint)GetAttachPointEnum(point));
				else
					AttachHandsEntity(type, eAttach_Right_Hand);
			}			
			break;

		case eRoleHands_Arrow:
			AttachHandsEntity(type, eAttach_Right_Hand);
		}

		if ( m_handsEntity[type].entity )
			RuACT_PlayMotion(m_handsEntity[type].entity, "persistent");
	}
}

//-----------------------------------------------------------------------------
void CRoleSprite::ReleaseHandsEntity(RoleHandsType type)
{
	// 清除彈藥
	if ( type == eRoleHands_Ranged )
		ReleaseHandsEntity(eRoleHands_Arrow);

	if ( m_handsEntity[type].enchantEntity ) {
		ruSAFE_RELEASE(m_handsEntity[type].enchantEntity);
	}

	if ( m_handsEntity[type].entity )
	{
		AddOnEntity(false, m_handsEntity[type].entity, m_handsEntity[type].point );
		ruSAFE_RELEASE(m_handsEntity[type].entity);
	}
	m_handsEntity[type].id		= 0;
	m_handsEntity[type].point	= eAttach_Null;
	m_handsEntity[type].type	= eRoleHands_None;
	m_handsEntity[type].enchantEntity = NULL;	
}

//-----------------------------------------------------------------------------
void CRoleSprite::AttachHandsEntity(RoleHandsType type, SpriteAttachPoint point)
{
	if ( m_handsEntity[type].point != point )
	{
		SpriteAttachPoint oldPoint = m_handsEntity[type].point;
		m_handsEntity[type].point = point;
		
		AddOnEntity(false, m_handsEntity[type].entity, oldPoint);
		AddOnEntity(true, m_handsEntity[type].entity, point);

		// Update caster class flags
		SetCasterClass();
	}
}

//-----------------------------------------------------------------------------
CRuEntity* CRoleSprite::LoadOrnamentEntity(int id, const char* insignia, int insigniaMainColor, int insigniaOffColor, const char* banner, int bannerMainColor, int bannerOffColor)
{
	GameObjDbStruct* pObj = CNetGlobal::GetObj(id);
	if ( pObj == NULL )
		return NULL;

	GameObjDbStruct* imageDB = CNetGlobal::GetObj(pObj->ImageID);
	if ( imageDB == NULL )
		return NULL;

	CRuEntity* entity = NULL;
	switch (imageDB->Image.ImageType)
	{
	// 旗幟
	case EM_GameObjectImageType_Flag:
		{
			// Load entity
			entity = g_ruResourceManager->LoadEntity(imageDB->Image.ACTWorld);
			if ( entity )
			{
				entity->SetOption(ruENTITYOPTION_NOMOTIONPROPAGATE, TRUE);

				if ( insignia == NULL )
				{
					insignia			= imageDB->Image.Flag.Insignia;
					insigniaMainColor	= imageDB->Image.Flag.InsigniaColor[0];
					insigniaOffColor	= imageDB->Image.Flag.InsigniaColor[1];
				}
				if ( banner == NULL )
				{
					banner				= imageDB->Image.Flag.Banner;
					bannerMainColor		= imageDB->Image.Flag.BannerColor[0];
					bannerOffColor		= imageDB->Image.Flag.BannerColor[1];
				}

				// 設定類型
				RuPaperdoll_SetComponent( entity, "flagstaff", imageDB->Image.Flag.Type );
				RuPaperdoll_SetComponent( entity, "insignia", insignia );
				RuPaperdoll_SetComponent( entity, "banner", banner );

				// 設定顏色
				BOOL layerActivated[4] = {TRUE, TRUE, TRUE, TRUE};
				RUCOLOR layerColor[4] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};		
				layerColor[2] = insigniaMainColor;
				layerColor[3] = insigniaOffColor;
				RuPaperdoll_SetComponentColors( entity, "insignia", layerActivated, layerColor);

				layerColor[2] = bannerMainColor;
				layerColor[3] = bannerOffColor;
				RuPaperdoll_SetComponentColors( entity, "banner", layerActivated, layerColor);

				RuPaperdoll_Build( entity , false );

				CRuQuaternion quaternion;
				quaternion.FromAngleAxis( CRuVector3(0.0f, 1.0f, 0.0f), ruPI);
				entity->Rotate(quaternion);

				entity->SetTranslation( CRuVector3(0.0f, -5.5f, -0.8f) );				
				entity->Scale(0.5f);
				entity->Update(0.0f);
			}
		}
		break;

	// 翅膀
	case EM_GameObjectImageType_Wing:
		{
			// Load entity
			entity = g_ruResourceManager->LoadEntity(imageDB->Image.ACTWorld);
			if ( entity )
			{
				entity->SetOption(ruENTITYOPTION_NOMOTIONPROPAGATE, TRUE);
			}
		}		
		break;
	}

	return entity;
}

//-----------------------------------------------------------------------------
void CRoleSprite::ReleaseWidget()
{
	if ( m_pRuWidget )
	{
		AddOnEntity(false, m_pRuWidget, eAttach_Name_Title);
		ruSAFE_RELEASE( m_pRuWidget );
	}
}

//-----------------------------------------------------------------------------
void CRoleSprite::ReleaseQuestEntity()
{
	if (m_pRuQuestEntity)
	{
		AddOnEntity(false, m_pRuQuestEntity, eAttach_Name_Title);
		ruSAFE_RELEASE( m_pRuQuestEntity );
	}
	m_questDisplay = eQuest_Display_Null;
}

//-----------------------------------------------------------------------------
// 設定角色動作
// actorState 動作列表
// layer [0,1,2,3]動作層級 0Auto 1Background 2Foreground 3Transient 4Auxiliary
void CRoleSprite::SetActorState(CRuFusion_ActorState actorState, int layer, bool disableMovement, CRuACT_ActionEvents *actionEventsOut)
{
	/*
	if ( GetObjectType() == eSpriteObject_Player && layer > 0 )
	{	
		g_pError->AddMessage(0, 0, "ID:%d actorNum:%d", m_id, actorState);
	}
	*/
	
	/*
	if ( m_objectId == 117218 ) {		
		g_pError->AddMessage(0, 0, "ID:%d actorNum:%d", m_id, actorState);
	}
	*/

	if ( GetDisableAction() )
		return;

	// 當物件未產生時,堆入暫存區
	if ( m_proxyLoadCompleted == false )
	{
		SpriteActorQueue temp;
		temp.layer = layer;
		temp.actor = actorState;
		m_actorStateQueue.push_back(temp);
		return;
	}

	if ( m_pRuActor == NULL )
		return;

		
	// 死亡時,限制可以撥放動作類型,恐懼中不能使用動作
	if ( GetDeadState() && m_specialEffect.fear >= 0 )
	{
		switch (actorState)
		{
		case ruFUSION_ACTORSTATE_DYING:
		case ruFUSION_ACTORSTATE_DEAD:
		case ruFUSION_ACTORSTATE_REVIVING:
			break;

		default:
			return;
		}
	}

	// 特殊狀態,限制可以撥放動作類型
	if ( m_specialEffect.fear > 0 )
	{
		switch (actorState)
		{
		case ruFUSION_ACTORSTATE_SIT_BEGIN:
		case ruFUSION_ACTORSTATE_SIT_LOOP:
		case ruFUSION_ACTORSTATE_SIT_END:
		case ruFUSION_ACTORSTATE_SLEEP_BEGIN:
		case ruFUSION_ACTORSTATE_SLEEP_LOOP:
		case ruFUSION_ACTORSTATE_SLEEP_END:
		case ruFUSION_MIME_EMOTE_HANDSTAND:
		case ruFUSION_MIME_EMOTE_ROFL:
		case ruFUSION_MIME_EMOTE_SWEEP1:
		case ruFUSION_MIME_EMOTE_SWEEP2:
		case ruFUSION_MIME_EMOTE_SWEEP3:
			return;
		}
	}

	// 目前背景動作
	CRuFusion_ActorState backgroundActor = GetActorState();

	// 坐騎上不能做的動作
	if ( m_mountSprite || m_linkParent ||
		 backgroundActor == ruFUSION_ACTORSTATE_SWIM_SURFACE ||
		 backgroundActor == ruFUSION_ACTORSTATE_SWIM_DIVE )
	{
		switch(actorState)
		{
		case ruFUSION_ACTORSTATE_SIT_BEGIN:
		case ruFUSION_ACTORSTATE_SIT_LOOP:
		case ruFUSION_ACTORSTATE_SIT_END:
		case ruFUSION_ACTORSTATE_SLEEP_BEGIN:
		case ruFUSION_ACTORSTATE_SLEEP_LOOP:
		case ruFUSION_ACTORSTATE_SLEEP_END:
		case ruFUSION_ACTORSTATE_CRAFTING_BEGIN:
		case ruFUSION_ACTORSTATE_CRAFTING_LOOP:
		case ruFUSION_ACTORSTATE_CRAFTING_END:
		case ruFUSION_ACTORSTATE_MINING_BEGIN:
		case ruFUSION_ACTORSTATE_MINING_LOOP:
		case ruFUSION_ACTORSTATE_MINING_END:
		case ruFUSION_ACTORSTATE_CROUCH_BEGIN:
		case ruFUSION_ACTORSTATE_CROUCH_LOOP:
		case ruFUSION_ACTORSTATE_CROUCH_END:
		case ruFUSION_ACTORSTATE_READING_BEGIN:
		case ruFUSION_ACTORSTATE_READING_LOOP:
		case ruFUSION_ACTORSTATE_READING_END:
		case ruFUSION_ACTORSTATE_LUMBERJACK_BEGIN:
		case ruFUSION_ACTORSTATE_LUMBERJACK_LOOP:
		case ruFUSION_ACTORSTATE_LUMBERJACK_END:
		case ruFUSION_ACTORSTATE_SLUMBER_BEGIN:
		case ruFUSION_ACTORSTATE_SLUMBER_LOOP:
		case ruFUSION_ACTORSTATE_SLUMBER_END:

		case ruFUSION_ACTORSTATE_GATHER_BEGIN:
		case ruFUSION_ACTORSTATE_GATHER_LOOP:
		case ruFUSION_ACTORSTATE_GATHER_END:
		case ruFUSION_ACTORSTATE_EMOTE_DANCE:				// 跳舞
		case ruFUSION_ACTORSTATE_EMOTE_STRETCH:				// 伸懶腰
		case ruFUSION_ACTORSTATE_EMOTE_BREAKDANCE:
		case ruFUSION_ACTORSTATE_EMOTE_TAPDANCE:
		case ruFUSION_ACTORSTATE_EMOTE_TIRED:
		case ruFUSION_ACTORSTATE_EMOTE_FEAR:
		case ruFUSION_ACTORSTATE_EMOTE_AMAZED:
		case ruFUSION_ACTORSTATE_EMOTE_THINK:
		case ruFUSION_ACTORSTATE_EMOTE_SURRENDER:
		case ruFUSION_ACTORSTATE_EMOTE_KICK:
		case ruFUSION_ACTORSTATE_EMOTE_HANDSTAND:
		case ruFUSION_ACTORSTATE_EMOTE_ROFL:
		case ruFUSION_ACTORSTATE_EMOTE_SWEEP1:
		case ruFUSION_ACTORSTATE_EMOTE_SWEEP2:
		case ruFUSION_ACTORSTATE_EMOTE_SWEEP3:
			return;
		}
	}

	switch ( layer )
	{
	case 1:
		{
			// 相同動作無做用
			if ( GetActorState(1) == actorState )
				return;

			// 清除特別動作
			InitialBackgroundActor();

			switch ( actorState )
			{
			// 特殊動作,收回武器
			case ruFUSION_ACTORSTATE_CRAFTING_BEGIN:
			case ruFUSION_ACTORSTATE_CRAFTING_LOOP:
			case ruFUSION_ACTORSTATE_CRAFTING_END:
			case ruFUSION_ACTORSTATE_MINING_BEGIN:
			case ruFUSION_ACTORSTATE_MINING_LOOP:
			case ruFUSION_ACTORSTATE_MINING_END:
			case ruFUSION_ACTORSTATE_LUMBERJACK_BEGIN:
			case ruFUSION_ACTORSTATE_LUMBERJACK_LOOP:
			case ruFUSION_ACTORSTATE_LUMBERJACK_END:
			case ruFUSION_ACTORSTATE_GATHER_BEGIN:
			case ruFUSION_ACTORSTATE_GATHER_LOOP:
			case ruFUSION_ACTORSTATE_GATHER_END:
			case ruFUSION_ACTORSTATE_REVIVING:
			case ruFUSION_ACTORSTATE_SIT_BEGIN:
			case ruFUSION_ACTORSTATE_SIT_LOOP:
			case ruFUSION_ACTORSTATE_SIT_END:
			case ruFUSION_ACTORSTATE_SLEEP_BEGIN:
			case ruFUSION_ACTORSTATE_SLEEP_LOOP:
			case ruFUSION_ACTORSTATE_SLEEP_END:
			case ruFUSION_ACTORSTATE_CROUCH_BEGIN:
			case ruFUSION_ACTORSTATE_CROUCH_LOOP:
			case ruFUSION_ACTORSTATE_CROUCH_END:
			case ruFUSION_ACTORSTATE_READING_BEGIN:
			case ruFUSION_ACTORSTATE_READING_LOOP:
			case ruFUSION_ACTORSTATE_READING_END:
			case ruFUSION_ACTORSTATE_EMOTE_HANDSTAND:
			case ruFUSION_ACTORSTATE_EMOTE_ROFL:
			case ruFUSION_ACTORSTATE_EMOTE_SWEEP1:
			case ruFUSION_ACTORSTATE_EMOTE_SWEEP2:
			case ruFUSION_ACTORSTATE_EMOTE_SWEEP3:
				ReleaseHandsEntity(eRoleHands_Ranged);
				SetWeaponPosition(false);
				break;
			}

			BOOL force = (actorState == ruFUSION_ACTORSTATE_DYING) ? TRUE : FALSE;
			m_pRuActor->SetBackgroundState(actorState, force);
			if ( m_pRuTempActor )
				m_pRuTempActor->SetBackgroundState(actorState, force);
		}
		break;

	case 2:
		m_pRuActor->SetForegroundState(actorState);
		if ( m_pRuTempActor )
			m_pRuTempActor->SetForegroundState(actorState);
		break;

	case 3:
		// 清除特別動作
		InitialBackgroundActor();

		m_pRuActor->SetTransientState(actorState, disableMovement, actionEventsOut);
		if ( m_pRuTempActor )
			m_pRuTempActor->SetTransientState(actorState);
		break;

	case 4:
		m_pRuActor->SetAuxiliaryState(actorState);
		if ( m_pRuTempActor )
			m_pRuTempActor->SetAuxiliaryState(actorState);
		break;

	default:
		switch ( actorState )
		{
			// "Background" States
		case ruFUSION_ACTORSTATE_NORMAL:
		case ruFUSION_ACTORSTATE_JUMP:
		case ruFUSION_ACTORSTATE_FALLING:
		case ruFUSION_ACTORSTATE_SWIM_SURFACE:
		case ruFUSION_ACTORSTATE_SWIM_DIVE:
		case ruFUSION_ACTORSTATE_SIT_BEGIN:
		case ruFUSION_ACTORSTATE_SIT_LOOP:
		case ruFUSION_ACTORSTATE_SIT_END:
		case ruFUSION_ACTORSTATE_SLEEP_BEGIN:
		case ruFUSION_ACTORSTATE_SLEEP_LOOP:
		case ruFUSION_ACTORSTATE_SLEEP_END:
		case ruFUSION_ACTORSTATE_DYING:
		case ruFUSION_ACTORSTATE_DEAD:
		case ruFUSION_ACTORSTATE_REVIVING:
		case ruFUSION_ACTORSTATE_SPAWN_BEGIN:
		case ruFUSION_ACTORSTATE_SPAWN_END:
		case ruFUSION_ACTORSTATE_DEAD_WATER:

		case ruFUSION_ACTORSTATE_CRAFTING_BEGIN:
		case ruFUSION_ACTORSTATE_CRAFTING_LOOP:
		case ruFUSION_ACTORSTATE_CRAFTING_END:
		case ruFUSION_ACTORSTATE_MINING_BEGIN:
		case ruFUSION_ACTORSTATE_MINING_LOOP:
		case ruFUSION_ACTORSTATE_MINING_END:

		case ruFUSION_ACTORSTATE_CROUCH_BEGIN:
		case ruFUSION_ACTORSTATE_CROUCH_LOOP:
		case ruFUSION_ACTORSTATE_CROUCH_END:

		case ruFUSION_ACTORSTATE_LUMBERJACK_BEGIN:
		case ruFUSION_ACTORSTATE_LUMBERJACK_LOOP:
		case ruFUSION_ACTORSTATE_LUMBERJACK_END:

		case ruFUSION_ACTORSTATE_GATHER_BEGIN:
		case ruFUSION_ACTORSTATE_GATHER_LOOP:
		case ruFUSION_ACTORSTATE_GATHER_END:

		case ruFUSION_ACTORSTATE_LOOT_BEGIN:
		case ruFUSION_ACTORSTATE_LOOT_LOOP:
		case ruFUSION_ACTORSTATE_LOOT_END:

		case ruFUSION_ACTORSTATE_CARRY_BEGIN:
		case ruFUSION_ACTORSTATE_CARRY_LOOP:
		case ruFUSION_ACTORSTATE_CARRY_END:

		case ruFUSION_ACTORSTATE_DAZED_BEGIN:
		case ruFUSION_ACTORSTATE_DAZED_LOOP:
		case ruFUSION_ACTORSTATE_DAZED_END:

		case ruFUSION_ACTORSTATE_SLUMBER_BEGIN:
		case ruFUSION_ACTORSTATE_SLUMBER_LOOP:
		case ruFUSION_ACTORSTATE_SLUMBER_END:

		case ruFUSION_ACTORSTATE_ACTIVATE_BEGIN:
		case ruFUSION_ACTORSTATE_ACTIVATE_LOOP:
		case ruFUSION_ACTORSTATE_ACTIVATE_END:

		case ruFUSION_ACTORSTATE_READING_BEGIN:
		case ruFUSION_ACTORSTATE_READING_LOOP:
		case ruFUSION_ACTORSTATE_READING_END:

		case ruFUSION_ACTORSTATE_SIT_CHAIR_BEGIN:
		case ruFUSION_ACTORSTATE_SIT_CHAIR_LOOP:
		case ruFUSION_ACTORSTATE_SIT_CHAIR_END:

		case ruFUSION_ACTORSTATE_KNOCKDOWN_BEGIN:
		case ruFUSION_ACTORSTATE_KNOCKDOWN_LOOP:
		case ruFUSION_ACTORSTATE_KNOCKDOWN_END:
		case ruFUSION_ACTORSTATE_KNOCKDOWN_RECOVER:		

		case ruFUSION_ACTORSTATE_EMOTE_HANDSTAND:
		case ruFUSION_ACTORSTATE_EMOTE_ROFL:

		case ruFUSION_ACTORSTATE_EMOTE_SWEEP1:
		case ruFUSION_ACTORSTATE_EMOTE_SWEEP2:
		case ruFUSION_ACTORSTATE_EMOTE_SWEEP3:

		case ruFUSION_ACTORSTATE_HOLDING:
		case ruFUSION_ACTORSTATE_SPIRAL_RIGHT:
		case ruFUSION_ACTORSTATE_SPIRAL_LEFT:
		case ruFUSION_ACTORSTATE_SPIRAL_LOOP:
		case ruFUSION_ACTORSTATE_FLY_BEGIN:
		case ruFUSION_ACTORSTATE_FLY_LOOP:
		case ruFUSION_ACTORSTATE_FLY_END:
			if ( GetObjectType() == eSpriteObject_Player )
			{
				switch (actorState)
				{
				case ruFUSION_ACTORSTATE_SIT_CHAIR_BEGIN:
				case ruFUSION_ACTORSTATE_SIT_CHAIR_LOOP:
					SetSitChairState(true);
					return;

				case ruFUSION_ACTORSTATE_SIT_CHAIR_END:
					SetSitChairState(false);
					return;
				}
			}
			SetActorState(actorState, 1, disableMovement, actionEventsOut);
			break;

			// "Foreground" States			
		case ruFUSION_ACTORSTATE_COMBAT_1H:
		case ruFUSION_ACTORSTATE_COMBAT_2H:
		case ruFUSION_ACTORSTATE_COMBAT_BOW:
		case ruFUSION_ACTORSTATE_COMBAT_GUN:
		case ruFUSION_ACTORSTATE_COMBAT_POLEARM:
		case ruFUSION_ACTORSTATE_COMBAT_UNARMED:
			SetActorState(actorState, 2, disableMovement, actionEventsOut);
			break;

			// "Transient" States
		case ruFUSION_ACTORSTATE_ATTACK_1H:
		case ruFUSION_ACTORSTATE_ATTACK_1H_SP:
		case ruFUSION_ACTORSTATE_ATTACK_1H_SP02:
		case ruFUSION_ACTORSTATE_ATTACK_1H_SLOW01:
		case ruFUSION_ACTORSTATE_ATTACK_2H:
		case ruFUSION_ACTORSTATE_ATTACK_2H_SP:
		case ruFUSION_ACTORSTATE_ATTACK_POLEARM:
		case ruFUSION_ACTORSTATE_ATTACK_POLEARM_SP:
		case ruFUSION_ACTORSTATE_ATTACK_UNARMED:
		case ruFUSION_ACTORSTATE_ATTACK_UNARMED_SP:
		case ruFUSION_ACTORSTATE_ATTACK_BOW_BEGIN:
		case ruFUSION_ACTORSTATE_ATTACK_BOW_LOOP:
		case ruFUSION_ACTORSTATE_ATTACK_BOW_END:
		case ruFUSION_ACTORSTATE_ATTACK_BOW_SP_BEGIN:
		case ruFUSION_ACTORSTATE_ATTACK_BOW_SP_LOOP:
		case ruFUSION_ACTORSTATE_ATTACK_BOW_SP_END:
		case ruFUSION_ACTORSTATE_ATTACK_GUN_BEGIN:
		case ruFUSION_ACTORSTATE_ATTACK_GUN_LOOP:
		case ruFUSION_ACTORSTATE_ATTACK_GUN_END:
		case ruFUSION_ACTORSTATE_ATTACK_OH:
		case ruFUSION_ACTORSTATE_ATTACK_OH_PIERCE:

		case ruFUSION_ACTORSTATE_PARRY_1H:
		case ruFUSION_ACTORSTATE_PARRY_2H:
		case ruFUSION_ACTORSTATE_PARRY_BOW:
		case ruFUSION_ACTORSTATE_PARRY_GUN:
		case ruFUSION_ACTORSTATE_PARRY_POLEARM:
		case ruFUSION_ACTORSTATE_PARRY_UNARMED:

		case ruFUSION_ACTORSTATE_SHIELD_BASH:
		case ruFUSION_ACTORSTATE_SHIELD_BLOCK:

		case ruFUSION_ACTORSTATE_USE:
		case ruFUSION_ACTORSTATE_THROW:

		case ruFUSION_ACTORSTATE_BUFF_BEGIN:
		case ruFUSION_ACTORSTATE_BUFF_LOOP:
		case ruFUSION_ACTORSTATE_BUFF_END:
		case ruFUSION_ACTORSTATE_BUFF_INSTANT:

		case ruFUSION_ACTORSTATE_BUFF_SP01:
		case ruFUSION_ACTORSTATE_BUFF_SP02:
		case ruFUSION_ACTORSTATE_BUFF_SP03:

		case ruFUSION_ACTORSTATE_CAST_BEGIN:
		case ruFUSION_ACTORSTATE_CAST_LOOP:
		case ruFUSION_ACTORSTATE_CAST_END:
		case ruFUSION_ACTORSTATE_CAST_INSTANT:

		case ruFUSION_ACTORSTATE_CAST_SP01:
		case ruFUSION_ACTORSTATE_CAST_SP02:
		case ruFUSION_ACTORSTATE_CAST_SP03:
		case ruFUSION_ACTORSTATE_CAST_SP04:
		case ruFUSION_ACTORSTATE_DEBUFF_INSTANT:

		case ruFUSION_ACTORSTATE_CHANNEL_BEGIN:
		case ruFUSION_ACTORSTATE_CHANNEL_LOOP:
		case ruFUSION_ACTORSTATE_CHANNEL_END:
		case ruFUSION_ACTORSTATE_CHANNEL_INSTANT:

		case ruFUSION_ACTORSTATE_FISHING_CAST:
		case ruFUSION_ACTORSTATE_FISHING_IDLE:
		case ruFUSION_ACTORSTATE_FISHING_REEL:

		case ruFUSION_ACTORSTATE_SP_SPINKICK:
		case ruFUSION_ACTORSTATE_SP_SPINSLASH:

		case ruFUSION_ACTORSTATE_HOLSTER:
		case ruFUSION_ACTORSTATE_UNHOLSTER:
		case ruFUSION_ACTORSTATE_HOLSTER_2H:
		case ruFUSION_ACTORSTATE_UNHOLSTER_2H:
		case ruFUSION_ACTORSTATE_EMOTE_ANGRY:
		case ruFUSION_ACTORSTATE_EMOTE_APPLAUSE:
		case ruFUSION_ACTORSTATE_EMOTE_APPROVAL:
		case ruFUSION_ACTORSTATE_EMOTE_BEG:
		case ruFUSION_ACTORSTATE_EMOTE_CRY:
		case ruFUSION_ACTORSTATE_EMOTE_HEAD_NOD:
		case ruFUSION_ACTORSTATE_EMOTE_HEAD_SHAKE:
		case ruFUSION_ACTORSTATE_EMOTE_LAUGH:
		case ruFUSION_ACTORSTATE_EMOTE_POINT:
		case ruFUSION_ACTORSTATE_EMOTE_PROVOKE:
		case ruFUSION_ACTORSTATE_EMOTE_SALUTE:
		case ruFUSION_ACTORSTATE_EMOTE_SPEAK:
		case ruFUSION_ACTORSTATE_EMOTE_SURRENDER:
		case ruFUSION_ACTORSTATE_EMOTE_THREATEN:
		case ruFUSION_ACTORSTATE_EMOTE_VICTORY:
		case ruFUSION_ACTORSTATE_EMOTE_WAVE:
		case ruFUSION_ACTORSTATE_EMOTE_EAT:
		case ruFUSION_ACTORSTATE_EMOTE_DRINK:
		case ruFUSION_ACTORSTATE_EMOTE_IDLE:
		case ruFUSION_ACTORSTATE_EMOTE_IDLE0:
		case ruFUSION_ACTORSTATE_EMOTE_IDLE1:
		case ruFUSION_ACTORSTATE_EMOTE_IDLE2:
		case ruFUSION_ACTORSTATE_EMOTE_DANCE:		
		case ruFUSION_ACTORSTATE_EMOTE_PLAY_SAXOPHONE:
		case ruFUSION_ACTORSTATE_EMOTE_PLAY_TAMBOURINE:
		case ruFUSION_ACTORSTATE_EMOTE_PLAY_GUITAR:
		case ruFUSION_ACTORSTATE_EMOTE_JUMP_INPLACE:
		case ruFUSION_ACTORSTATE_EMOTE_BELLY:
		case ruFUSION_ACTORSTATE_EMOTE_EMBRAASSED:
		case ruFUSION_ACTORSTATE_EMOTE_EMBRACE:
		case ruFUSION_ACTORSTATE_EMOTE_KISS:
		case ruFUSION_ACTORSTATE_EMOTE_SALUTE2:
		case ruFUSION_ACTORSTATE_EMOTE_CUTE1:
		case ruFUSION_ACTORSTATE_EMOTE_CUTE2:
		case ruFUSION_ACTORSTATE_EMOTE_CUTE3:
		case ruFUSION_ACTORSTATE_EMOTE_HANGOVER:
		case ruFUSION_ACTORSTATE_EMOTE_STRETCH:
		case ruFUSION_ACTORSTATE_EMOTE_ROCK:
		case ruFUSION_ACTORSTATE_EMOTE_KAMEHAMEHA:
		case ruFUSION_ACTORSTATE_EMOTE_KICK:		
		case ruFUSION_ACTORSTATE_EMOTE_BREAKDANCE:
		case ruFUSION_ACTORSTATE_EMOTE_TAPDANCE:
		case ruFUSION_ACTORSTATE_EMOTE_TIRED:
		case ruFUSION_ACTORSTATE_EMOTE_FEAR:
		case ruFUSION_ACTORSTATE_EMOTE_AMAZED:
		case ruFUSION_ACTORSTATE_EMOTE_THINK:
			SetActorState(actorState, 3, disableMovement, actionEventsOut);
			break;

		case ruFUSION_ACTORSTATE_HURT_NORMAL:
		case ruFUSION_ACTORSTATE_HURT_CRITICAL:
		case ruFUSION_ACTORSTATE_DODGE:
			SetActorState(actorState, 4, disableMovement, actionEventsOut);
			break;
		}		
	}
}

//-----------------------------------------------------------------------------
void CRoleSprite::InitialBackgroundActor()
{
	if ( m_pRuActor == NULL )
		return;

	// 清除特別動作,當在LOOP動作中先清除狀態在設定新動作
	switch ( GetActorState(1) )
	{
	case ruFUSION_ACTORSTATE_CRAFTING_BEGIN:
	case ruFUSION_ACTORSTATE_CRAFTING_LOOP:
	case ruFUSION_ACTORSTATE_MINING_BEGIN:
	case ruFUSION_ACTORSTATE_MINING_LOOP:	
	case ruFUSION_ACTORSTATE_CROUCH_BEGIN:
	case ruFUSION_ACTORSTATE_CROUCH_LOOP:
	case ruFUSION_ACTORSTATE_READING_BEGIN:
	case ruFUSION_ACTORSTATE_READING_LOOP:
	case ruFUSION_ACTORSTATE_GATHER_BEGIN:
	case ruFUSION_ACTORSTATE_GATHER_LOOP:
	case ruFUSION_ACTORSTATE_LUMBERJACK_BEGIN:
	case ruFUSION_ACTORSTATE_LUMBERJACK_LOOP:
		m_pRuActor->SetBackgroundState(ruFUSION_ACTORSTATE_NORMAL);
		break;

	case ruFUSION_ACTORSTATE_SIT_BEGIN:
	case ruFUSION_ACTORSTATE_SIT_LOOP:
		m_pRuActor->SetBackgroundState(ruFUSION_ACTORSTATE_SIT_END);
		break;

	case ruFUSION_ACTORSTATE_SLEEP_BEGIN:
	case ruFUSION_ACTORSTATE_SLEEP_LOOP:
		m_pRuActor->SetBackgroundState(ruFUSION_ACTORSTATE_SLEEP_END);
		// m_pRuActor->SetBackgroundState(ruFUSION_ACTORSTATE_NORMAL);
		break;
	}
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetDefaultWalkingAction(CRuFusion_Mime_Action action)
{
	if ( m_pRuActor )
		m_pRuActor->SetDefaultWalkingAction(action);
	if ( m_pRuTempActor )
		m_pRuTempActor->SetDefaultWalkingAction(action);
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetDefaultRunningAction(CRuFusion_Mime_Action action)
{
	if ( m_pRuActor )
		m_pRuActor->SetDefaultRunningAction(action);
	if ( m_pRuTempActor )
		m_pRuTempActor->SetDefaultRunningAction(action);
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetDefaultBackAction(CRuFusion_Mime_Action action)
{
	if ( m_pRuActor )
		m_pRuActor->SetDefaultBackAction(action);
	if ( m_pRuTempActor )
		m_pRuTempActor->SetDefaultBackAction(action);
}

//-----------------------------------------------------------------------------
CRuFusion_Mime_Action CRoleSprite::GetDefaultWalkingAction()
{
	return (m_pRuActor) ? m_pRuActor->GetDefaultWalkingAction() : ruFUSION_MIME_NONE;
}

//-----------------------------------------------------------------------------
CRuFusion_Mime_Action CRoleSprite::GetDefaultRunningAction()
{
	return (m_pRuActor) ? m_pRuActor->GetDefaultRunningAction() : ruFUSION_MIME_NONE;
}

//-----------------------------------------------------------------------------
CRuFusion_Mime_Action CRoleSprite::GetDefaultBackAction()
{
	return (m_pRuActor) ? m_pRuActor->GetDefaultBackAction() : ruFUSION_MIME_NONE;
}

//-----------------------------------------------------------------------------
CRuFusion_ActorState CRoleSprite::GetActorState(int layer)
{
	if ( m_pRuActor == NULL )
		return ruFUSION_ACTORSTATE_NORMAL;

	switch ( layer )
	{
	case 2:
		return m_pRuActor->GetForegroundState();

	case 3:
		return m_pRuActor->GetTransientState();

	default:
		return m_pRuActor->GetBackgroundState();
	}
}

//-----------------------------------------------------------------------------
CRuFusion_ActorState CRoleSprite::GetDisplayActorState()
{
	CRuFusion_ActorState actorState = ruFUSION_ACTORSTATE_NORMAL;  
	if ( m_pRuActor == NULL )
		return actorState;

	actorState = m_pRuActor->GetTransientState();

	if( actorState != ruFUSION_ACTORSTATE_NORMAL )
	{
		return actorState; 
	}

	actorState = m_pRuActor->GetForegroundState();

	if( actorState != ruFUSION_ACTORSTATE_NORMAL )
	{
		return actorState; 
	}

	actorState = m_pRuActor->GetBackgroundState();

	return actorState; 
}

//-----------------------------------------------------------------------------
void CRoleSprite::PushActorQueue(CRuFusion_ActorState actorState, float delayTime)
{
	// 死亡時,限制可以撥放動作類型
	if ( GetDeadState() )
	{
		switch (actorState)
		{
		case ruFUSION_ACTORSTATE_DYING:
		case ruFUSION_ACTORSTATE_DEAD:
		case ruFUSION_ACTORSTATE_REVIVING:
			break;

		default:
			return;
		}
	}

	map<int, float>::iterator iter = m_actorQueue.find(actorState);
	if ( iter != m_actorQueue.end() )
	{
		if ( iter->second > delayTime )
			iter->second = delayTime;
	}
	else
		m_actorQueue.insert(make_pair(actorState, delayTime));
}

//-----------------------------------------------------------------------------
void CRoleSprite::UpdateActorQueue(float elapsedTime)
{
	vector<map<int, float>::iterator> removeIter;
	for ( map<int, float>::iterator iter = m_actorQueue.begin(); iter != m_actorQueue.end(); iter++ )
	{
		if ( iter->second < elapsedTime )
		{
			SetActorState(CRuFusion_ActorState(iter->first));
			removeIter.push_back(iter);
		}
		else
			iter->second -= elapsedTime;
	}

	for ( vector<map<int, float>::iterator>::iterator iter = removeIter.begin(); iter != removeIter.end(); iter++ )
		m_actorQueue.erase(*iter);
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::SetDefaultIdleAction(CRuFusion_Mime_Action defaultIdleAction)
{
	if(m_pRuActor)
	{
		m_pRuActor->SetDefaultIdleAction(defaultIdleAction);
	}
}

//-------------------------------------------------------------------------------------------------
float CRoleSprite::WidgetAddRowText( const WCHAR* textStr, float scale, int color)
{
	assert( m_pRuWidget );

	vector<CRuRectangle> stock;

	vector<wstring> strList;
	CRuFontString::SplitString(textStr, (int)wcslen(textStr), strList);

	for ( vector<wstring>::iterator iter = strList.begin(); iter != strList.end(); iter++ )
	{
		CRuFontString* fontString = CRuFontString::GetFontString(s_pFontType, iter->c_str(), (int)iter->length(), TRUE);
		if ( fontString )
		{
			CRuFontTexture2* fontTexture = fontString->GetFontTexture();
			if ( fontTexture )
			{
				stock.push_back( fontTexture->GetRectangle() );
			}			
		}
	}

	float width = 0.0f;
	if ( !stock.empty() )
	{
		int row = m_pRuWidget->GetRowCount();
		m_pRuWidget->InsertGlyph( row, (INT32)stock.size(), &stock[0], scale, color, CRuFontTexture2::GetRuTextureD3D() );

		// 計算最大寬度
		for ( vector<CRuRectangle>::iterator iter = stock.begin(); iter != stock.end(); iter++ )
			width += (iter->m_right - iter->m_left) / (iter->m_bottom - iter->m_top) * scale;
	}

	return width;
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::WidgetAddClassIcon(int row, float width)
{
	assert( m_pRuWidget );

	IRuTexture* texture = g_ruResourceManager->LoadTexture("Interface\\WidgetIcons\\Space");
	if ( texture )
	{
		CRuRectangle stock(0.0f, 0.0f, (float)texture->GetLevelWidth(0), (float)texture->GetLevelHeight(0));
		m_pRuWidget->InsertGlyph( row, 1, &stock, width, 0xFFFFFFFF, texture, CRuEntity_Widget_Label::ruGLYPHJUSTIFY_LEFT );
	}

	char filename[MAX_PATH];
	char mainStr[MAX_PATH];
	char subStr[MAX_PATH];
	strcpy(mainStr, g_ObjectData->GetClassTokenByID(m_vocation));

	if ( m_vocation == EM_Vocation_GameMaster )
	{
		strcpy(filename, "Interface\\WidgetIcons\\ClassIcon_GM");
	}
	else
	{
		if ( m_vocationSub != EM_Vocation_None )
		{
			strcpy(subStr, g_ObjectData->GetClassTokenByID(m_vocationSub));
			sprintf_s(filename, MAX_PATH, "Interface\\WidgetIcons\\ClassIcon_%s_%s", mainStr, subStr);
		}
		else
		{
			sprintf_s(filename, MAX_PATH, "Interface\\WidgetIcons\\ClassIcon_%s", mainStr);
		}
	}
	
	texture = g_ruResourceManager->LoadTexture(filename);
	if ( texture )
	{
		CRuRectangle stock(0.0f, 0.0f, (float)texture->GetLevelWidth(0), (float)texture->GetLevelHeight(0));
		m_pRuWidget->InsertGlyph( row, 1, &stock, 48.0f, 0xFFFFFFFF, texture, CRuEntity_Widget_Label::ruGLYPHJUSTIFY_LEFT );
	}
}

//-------------------------------------------------------------------------------------------------
void CRoleSprite::WidgetAddGoodEvilIcon(int row, float width)
{
	assert( m_pRuWidget );
	IRuTexture* texture = NULL;

	texture = g_ruResourceManager->LoadTexture("Interface\\WidgetIcons\\Space");
	if ( texture )
	{
		CRuRectangle stock(0.0f, 0.0f, (float)texture->GetLevelWidth(0), (float)texture->GetLevelHeight(0));		
		m_pRuWidget->InsertGlyph( row, 1, &stock, width, 0xFFFFFFFF, texture, CRuEntity_Widget_Label::ruGLYPHJUSTIFY_RIGHT );
	}

	switch (m_goodEvilType)
	{
	case EM_GoodEvil_Demon:
		texture = g_ruResourceManager->LoadTexture("Interface\\WidgetIcons\\GoodEvil_Demon");
		break;

	case EM_GoodEvil_Evil3:
		texture = g_ruResourceManager->LoadTexture("Interface\\WidgetIcons\\GoodEvil_Evil03");
		break;

	case EM_GoodEvil_Evil2:
		texture = g_ruResourceManager->LoadTexture("Interface\\WidgetIcons\\GoodEvil_Evil02");
		break;

	case EM_GoodEvil_Evil1:
		texture = g_ruResourceManager->LoadTexture("Interface\\WidgetIcons\\GoodEvil_Evil01");
		break;

	case EM_GoodEvil_Normal:
		texture = g_ruResourceManager->LoadTexture("Interface\\WidgetIcons\\GoodEvil_Ordinary");
		break;

	case EM_GoodEvil_Good1:
		texture = g_ruResourceManager->LoadTexture("Interface\\WidgetIcons\\GoodEvil_Good01");
		break;

	case EM_GoodEvil_Good2:
		texture = g_ruResourceManager->LoadTexture("Interface\\WidgetIcons\\GoodEvil_Good02");
		break;

	case EM_GoodEvil_Good3:
		texture = g_ruResourceManager->LoadTexture("Interface\\WidgetIcons\\GoodEvil_Good03");
		break;

	case EM_GoodEvil_Hero:
		texture = g_ruResourceManager->LoadTexture("Interface\\WidgetIcons\\GoodEvil_Hero");
		break;
	}
	if ( texture )
	{
		CRuRectangle stock(0.0f, 0.0f, (float)texture->GetLevelWidth(0), (float)texture->GetLevelHeight(0));		
		m_pRuWidget->InsertGlyph( row++, 1, &stock, 48.0f, 0xFFFFFFFF, texture, CRuEntity_Widget_Label::ruGLYPHJUSTIFY_RIGHT );
	}
}

//-------------------------------------------------------------------------------------------------
float CRoleSprite::WidgetAddRaidMarkIcon(int row)
{
	float width = 0.0f;
	if ( m_pRuWidget == NULL || m_raidMarkNumber < 0 || m_raidMarkNumber > 7 )
		return width;

	IRuTexture* texture = g_ruResourceManager->LoadTexture("Interface\\Common\\RaidTarget");
	if ( texture )
	{
		CRuRectangle stock;
		stock.m_left = (m_raidMarkNumber / 8.0f) * texture->GetLevelWidth(0);
		stock.m_right = stock.m_left + (1.0f / 8.0f) * texture->GetLevelWidth(0);
		stock.m_top = 0.0f;
		stock.m_bottom = (float)texture->GetLevelHeight(0);
		m_pRuWidget->InsertGlyph( row, 1, &stock, 72.0f, 0xFFFFFFFF, texture );

		width = (stock.m_right - stock.m_left) / (stock.m_bottom - stock.m_top) * 72.0f;
	}
	return width;
}

//-------------------------------------------------------------------------------------------------
int CRoleSprite::GetTakeItemType( int item )
{
	GameObjDbStruct* pObj = NULL ;
	if ( item != 0 && (pObj = CNetGlobal::GetObj(item)) != NULL )
	{
		if ( pObj->Classification == EM_ObjectClass_Item )
		{
			switch ( pObj->Item.Class )
			{
			case EM_Item_Weapon:
				switch ( pObj->Item.WeaponType )
				{
				case EM_Weapon_Unarmed:
					return eRoleTakeItem_Unarmed;
				case EM_Weapon_Blade:
					return eRoleTakeItem_Blade;
				case EM_Weapon_Dagger:
					return eRoleTakeItem_Dagger;
				case EM_Weapon_Wand:
					return eRoleTakeItem_Wand;
				case EM_Weapon_Axe:
					return eRoleTakeItem_Axe;
				case EM_Weapon_Bludgeon:
					return eRoleTakeItem_Bludgeon;
				case EM_Weapon_Claymore:
					return eRoleTakeItem_Claymore;
				case EM_Weapon_Staff:
					return eRoleTakeItem_Staff;
				case EM_Weapon_2H_Axe:
					return eRoleTakeItem_2H_Axe;
				case EM_Weapon_2H_Hammer:
					return eRoleTakeItem_2H_Hammer;
				case EM_Weapon_Polearm:
					return eRoleTakeItem_Polearm;
				case EM_Weapon_Bow:
					return eRoleTakeItem_Bow;
				case EM_Weapon_CossBow:
					return eRoleTakeItem_CossBow;
				case EM_Weapon_Gun:
					return eRoleTakeItem_Gun;
				case EM_Weapon_Ammo_Bow:
					return eRoleTakeItem_Ammo_Bow;
//				case EM_Weapon_Ammo_CossBow:
//					return eRoleTakeItem_Ammo_CossBow;
				case EM_Weapon_Ammo_Gun:
					return eRoleTakeItem_Ammo_Gun;
				case EM_Weapon_Throw:
					return eRoleTakeItem_Throw;
				case EM_Weapon_Lumbering:
					return eRoleTakeItem_Lumbering;
				case EM_Weapon_Herbalism:
					return eRoleTakeItem_Herbalism;
				case EM_Weapon_Mining:
					return eRoleTakeItem_Mining;
				case EM_Weapon_Fishing:
					return eRoleTakeItem_Fishing;
				}

			case EM_Item_Armor:
				switch (pObj->Item.ArmorType)
				{
				case EM_Armor_Shield:
					return eRoleTakeItem_Shield;
				case EM_Armor_Implement:
					return eRoleTakeItem_Implement;
				}
			}
		}
	}
	return eRoleTakeItem_None;
}

//-----------------------------------------------------------------------------
float CRoleSprite::ComputeFallingTime( CRuVector3& pos, CRuVector3& vec, float& h )
{
	float time = 0.0f;

	CRuVector3 now_pos = pos;
	float last_h = now_pos.m_y;
	while ( true )
	{	
		float now_h;
		now_pos += (vec * time);
		now_h = pos.m_y - (0.5f * 98.0f * time * time);

		now_pos.m_y = last_h;
		if ( !TerrainTestHight(now_pos, h, 500.0f) )
			return 0.0f;
		else if ( now_h < h )
			return time;
		
		last_h = now_h;
		time += 0.05f;
	}
}

//-----------------------------------------------------------------------------
void CRoleSprite::UpdateTitleVisible()
{
	if ( m_pRuWidget == NULL )
		return;

	bool titleVisible = false;
	if ( m_controlState.nameVisibleForce )
		titleVisible = m_controlState.nameVisible;	

	/*
	if ( m_controlState.DisableQuest )
		titleVisible = false;	
	else 
		*/

	if ( m_pRuTempEntity )
	{
		AddOnEntity(false, m_pRuWidget, eAttach_Name_Title);			// Detach
		if ( titleVisible )
			RuEntity_Attach(m_pRuTempEntity, m_pRuWidget, GetAttachPointName(eAttach_Name_Title));
		else
			RuEntity_Detach(m_pRuTempEntity, m_pRuWidget, GetAttachPointName(eAttach_Name_Title));
	}
	else
	{
		AddOnEntity(titleVisible, m_pRuWidget, eAttach_Name_Title);
	}
}

//-----------------------------------------------------------------------------
void CRoleSprite::EffectUpdate()
{
	// 檢測背景動作
	CRuFusion_ActorState state = GetActorState();
	
	// 睡眠
	if ( m_specialEffect.sleep > 0 ) {
		SetActorState(ruFUSION_ACTORSTATE_SLUMBER_BEGIN, 1);
	} else {
		switch ( state )
		{
		case ruFUSION_ACTORSTATE_SLUMBER_BEGIN:
		case ruFUSION_ACTORSTATE_SLUMBER_LOOP:
			SetActorState(ruFUSION_ACTORSTATE_SLUMBER_END, 1);
			break;
		}		
	}

	// 
	if ( m_specialEffect.dizzy > 0 ) {
		SetActorState(ruFUSION_ACTORSTATE_DAZED_BEGIN, 1);
	} else {
		switch (state)
		{
		case ruFUSION_ACTORSTATE_DAZED_BEGIN:
		case ruFUSION_ACTORSTATE_DAZED_LOOP:
			SetActorState(ruFUSION_ACTORSTATE_DAZED_END, 1);
			break;
		}		
	}

	// 擊倒
	if ( m_specialEffect.hitDown > 0 ) {
		SetActorState(ruFUSION_ACTORSTATE_KNOCKDOWN_END, 1);
	} else {
		switch (state)
		{
		case ruFUSION_ACTORSTATE_KNOCKDOWN_BEGIN:
		case ruFUSION_ACTORSTATE_KNOCKDOWN_LOOP:
		case ruFUSION_ACTORSTATE_KNOCKDOWN_END:
			SetActorState(ruFUSION_ACTORSTATE_KNOCKDOWN_RECOVER, 1);
			break;
		}
	}

	if ( m_specialEffect.chaos > 0 ) {
	} else if ( m_specialEffect.chaos < 0 ) {
	}

	// 潛形
	if ( m_specialEffect.sneak > 0 ) {
		m_controlState.sneakMime = true;

		SetDefaultWalkingAction(ruFUSION_MIME_SNEAK_FORWARD);
		SetDefaultRunningAction(ruFUSION_MIME_SNEAK_FORWARD);
		SetDefaultBackAction(ruFUSION_MIME_SNEAK_BACKWARD);
	} else if ( m_controlState.sneakMime ) {
		m_controlState.sneakMime = false;
		if ( m_pRuActor ) {

			if ( GetDefaultWalkingAction() == ruFUSION_MIME_SNEAK_FORWARD )
				SetDefaultWalkingAction(ruFUSION_MIME_WALK_FORWARD);

			if ( GetWalkState() )
			{
				if ( GetDefaultRunningAction() == ruFUSION_MIME_SNEAK_FORWARD )
					SetDefaultRunningAction(ruFUSION_MIME_WALK_FORWARD);
				if ( GetDefaultBackAction() == ruFUSION_MIME_SNEAK_BACKWARD )
					SetDefaultRunningAction(ruFUSION_MIME_WALK_FORWARD);
			}
			else
			{
				if ( GetDefaultRunningAction() == ruFUSION_MIME_SNEAK_FORWARD )
					SetDefaultRunningAction(ruFUSION_MIME_RUN_FORWARD);
				if ( GetDefaultBackAction() == ruFUSION_MIME_SNEAK_BACKWARD )
					SetDefaultBackAction(ruFUSION_MIME_RUN_BACKWARD);
			}
		}
	}
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetMoveType(SpriteMoveType type)
{
	if ( m_moveType != type ) {
		// 設定類型
		switch ( type )
		{
		case eRoleSpriteMove_Normal:
			switch ( m_moveType )
			{
			// 震退
			case eRoleSpriteMove_StrikeBack:
				ClearMoveVector();
				SetActorState(ruFUSION_ACTORSTATE_NORMAL);
				break;

			// 衝封
			case eRoleSpriteMove_Charge:
				m_pRuActor->SetDefaultRunningAction(ruFUSION_MIME_RUN_FORWARD);
				ClearMoveVector();

				if ( m_objectType == eSpriteObject_Player ) {
					// 玩家
					m_pRuActor->SetMovementType(ruSOJMOVETYPE_FULL);
				}
				break;
			}
			break;

		case eRoleSpriteMove_StrikeBack:
			break;

		case eRoleSpriteMove_Charge:
			break;			
		}

		m_moveType = type;
	}	
}

//-----------------------------------------------------------------------------
void CRoleSprite::CreateBuffEffect(GameObjDbStruct* itemDB, RoleBuffInfo* buff)
{	
	if ( buff == NULL || itemDB == NULL )
		return;

	GameObjDbStruct* imageDB = CNetGlobal::GetObj(itemDB->ImageID);
	if ( imageDB == NULL )
		return;

	// 檢查是否要產生特效(非自己的不顯示)
	if ( itemDB->MagicBase.Setting.OnlyYouCanSee && buff->source != s_localPlayerId )
		return;

	bool boShow = (s_hideEffectFunc) ? s_hideEffectFunc(buff->source, m_id) : false;
	if ( itemDB->MagicBase.Setting.forceDisplayEffect )
		boShow = true;

	// 有特效名稱但是未產生特效	
	if ( boShow && buff->effect == NULL && imageDB->Image.BuffACT[0] != 0 )
	{
		buff->effect = new CEffectSprite;		
		if ( buff->effect->LoadRuEntity(imageDB->Image.BuffACT) )
		{
			float scale = imageDB->Image.ModelSize * imageDB->Image.ActRate[EM_ImageActType_BuffExtend] / 10000.0f;

			buff->effect->SetScale(scale, eSpriteScale_World);
			buff->effect->SetAutoDestruct(false);
			buff->effect->AttachSprite(this, IBaseSprite::GetAttachPointEnum(imageDB->Image.BuffPoint));
			if ( buff->effect->HasMotion("explode") )
				buff->effect->SetEffectMotionStep(eEffectMotion_Step_Explode);
			else
				buff->effect->SetEffectMotionStep(eEffectMotion_Step_Appear);
		}
		else
		{
			delete buff->effect;
			buff->effect = NULL;
		}
	}

	if ( buff->armEffect == NULL && imageDB->Image.BuffACTEx[0] != 0 )
	{
		char filename[512];
		char tmp[512];
		char race[64];
		char sex[64];

		sprintf_s(filename, 512, imageDB->Image.BuffACTEx);

		// 找到最後一個 '\' or '/' 的下一個字元
		const char *last = strrchr(filename, '/');
		if ( last == NULL )
			last = strrchr(filename, '\\');

		memset(race, 0, sizeof(race));
		switch (m_race)
		{
		case EM_Race1:
			strcpy(race, "human");
			break;

		case EM_Race2:
			strcpy(race, "elf");
			break;
		}
		
		memset(sex, 0, sizeof(sex));
		switch (m_sex)
		{
		case EM_Sex_Boy:
			strcpy(sex, "male");
			break;

		case EM_Sex_Girl:
			strcpy(sex, "female");
			break;
		}

		if ( last != NULL )
		{
			sprintf_s(tmp, 512, "model/character/pc/%s/%s/%s", race, sex, last + 1);
		}
		else
		{
			sprintf_s(tmp, 512, "model/character/pc/%s/%s/%s", race, sex, filename);
		}

		// 組合字串 model\\character\\pc\\$race\\$sex\\   $race 替換種族  $sex 替換性別		

		buff->armEffect = g_ruResourceManager->LoadEntity(tmp);
		if ( buff->armEffect )
		{			
			// 撥放特效
			//RuACT_PlayMotion(buff->armEffect, "persistent");
			RuACT_PlayPersistentMotion(buff->armEffect);

			// buff->armEffect->Update(0.0f);

			// 附加到母元件
			// m_pRuEntity->AddChild(buff->armEffect);
			RuEntity_Attach(m_pRuEntity, buff->armEffect, ATTACH_POINT_DOWN);
		}
	}
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetAboveWaterState(bool enable)
{
	if ( m_pRuActor )
	{
		if ( enable )
		{
			CRuFusion_Sojourner *soj = m_pRuActor->GetSojourner();
			soj->SetCollisionMask(RuWorld::ruTRIFLAGS_FOLIAGE);
		}
		else
		{
			CRuFusion_Sojourner *soj = m_pRuActor->GetSojourner();
			soj->SetCollisionMask(RuWorld::ruTRIFLAGS_WATER | RuWorld::ruTRIFLAGS_FOLIAGE);
		}
	}

	IBaseSprite::SetAboveWaterState(enable);
}

//-----------------------------------------------------------------------------
bool CRoleSprite::IsAboveWater()
{
	if ( m_pRuActor )
		return (bool)m_pRuActor->IsAboveWater();
	return false;
}

//-----------------------------------------------------------------------------
int	CRoleSprite::GetMountCarryCount()
{
	if ( m_controlState.ride )
	{
		for ( vector<RoleBuffInfo>::iterator iter = m_buffInfo.begin(); iter != m_buffInfo.end(); iter++ )
		{
			if ( iter->mount )
			{
				GameObjDbStruct* itemDB = CNetGlobal::GetObj(iter->id);
				if ( itemDB )
				{
					return itemDB->MagicBase.CarryCount;
				}
			}
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetImplementMaxNum(int maxNum)
{
	m_implementMaxNum = maxNum;
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetImplementCount(int count)
{
	m_implementCount = max(count, 0);
}

//-----------------------------------------------------------------------------
// sprite 載掛載具
// attachPoint 連結座標點
// idleMotion 乘座者動作
// sitPos 乘做位置
void CRoleSprite::AttachImplement(CRoleSprite* sprite, SpriteAttachPoint attachPoint, CRuFusion_Mime_Action idleMotion, int sitPos)
{
	if ( sprite == NULL )
		return;

	DetachImplement();

	if ( sprite != m_implementSprite )
	{
		const char* attachPointString = "";

		bool controlMove = false;

		bool isAlignment = false;

		bool visible = true;		

		int objectId = sprite->GetObjectId();

		GameObjDbStruct* pObj = CNetGlobal::GetObj( objectId );
		
		if ( pObj && pObj->Classification == EM_ObjectClass_NPC )
		{
			switch (pObj->NPC.CarryType)
			{
			case EM_NPCCarryType_None:
				break;

			// 是否自動面向攻擊方向
			case EM_NPCCarryType_NoControl:
				if ( sitPos == 1 )
					isAlignment = true;
				break;

			// 可控載具,只有第一個可以移動
			case EM_NPCCarryType_Control:
				if ( sitPos == 1 )
					controlMove = true;
				break;
			}

			// 是否顯示
			visible = !pObj->NPC.Invisible_InWagon;

			if ( pObj->NPC.CheckCarryLuaPoint[0] )
			{
				attachPoint = (SpriteAttachPoint)IBaseSprite::GetAttachPointEnum(pObj->NPC.CheckCarryLuaPoint);
			}
		}

		switch ( attachPoint )
		{
		case eAttach_Top:
			attachPointString = ATTACH_POINT_TOP;
			break;

		case eAttach_Down:
			attachPointString = ATTACH_POINT_DOWN;
			break;

		case eAttach_Hit_Point1:
			attachPointString = ATTACH_POINT_HIT01;
			break;

		case eAttach_Hit_Point2:
			attachPointString = ATTACH_POINT_HIT02;
			break;

		case eAttach_Back_Polearm:
			attachPointString = ATTACH_POINT_BACK_STAFF;
			break;

		case eAttach_Back_Shield:
			attachPointString = ATTACH_POINT_BACK_SHIELD;
			break;

		case eAttach_Back_Sword:
			attachPointString = ATTACH_POINT_BACK_SWORD;
			break;

		case eAttach_Name_Title:
			attachPointString = ATTACH_POINT_NAME_TITLE;
			break;

		case eAttach_Hand_Shield:
			attachPointString = ATTACH_POINT_HAND_SHIELD;
			break;

		case eAttach_Left_Hand:
			attachPointString = ATTACH_POINT_LEFT_HAND;
			break;

		case eAttach_Right_Hand:
			attachPointString = ATTACH_POINT_RIGHT_HAND;
			break;

		case eAttach_Left_Foot:
			attachPointString = ATTACH_POINT_LEFT_FOOT;
			break;

		case eAttach_Right_Foot:
			attachPointString = ATTACH_POINT_RIGHT_FOOT;
			break;

		case eAttach_Left_Weapon:
			attachPointString = ATTACH_POINT_LEFT_WEAPON;
			break;

		case eAttach_Right_Weapon:
			attachPointString = ATTACH_POINT_RIGHT_WEAPON;
			break;

		case eAttach_Shoot:
			attachPointString = ATTACH_POINT_SHOOT;
			break;

		case eAttach_Mouth:
			attachPointString = ATTACH_POINT_MOUTH;
			break;
		}

		sprite->LinkSprite(this, attachPointString, true, false);
		sprite->GetRuActor()->DisableInPlaceJump(FALSE);
		sprite->GetRuActor()->LinkActor(GetRuActor());
		sprite->GetRuActor()->SetMovementType(ruSOJMOVETYPE_FULL);
		//sprite->m_orientToSurfaceForword = true;		
		//sprite->SetOrientToSurfaceNormal(true);

		// Set player parameters
		SetGravityMode(false);
		EnableTorsoTurn(false);
		SetImplementVisible(visible);
		SetDefaultIdleAction(idleMotion);
		SetImplementSprite(sprite);
		SetImplementAlignment(isAlignment);

		if( m_pRuActor )
		{
			m_pRuActor->EnableSojourner(FALSE);
			m_pRuActor->Update(0.0f);
		}

		//m_implementSprite = sprite;
		m_implementSitPos = sitPos;
		m_implementControlMove = controlMove;
		m_implementSprite->m_implementCount++;
	}
}

//-----------------------------------------------------------------------------
void CRoleSprite::DetachImplement()
{
	if ( m_implementSprite )
	{
		m_implementSprite->UnlinkSprite(this);

		//m_implementSprite->SetOrientToSurfaceNormal(false);
		m_implementSprite->GetRuActor()->UnlinkActor(GetRuActor());
		m_implementSprite->m_implementCount--;

		SetGravityMode(true);
		EnableTorsoTurn(true);
		SetImplementVisible(true);
		SetDefaultIdleAction(ruFUSION_MIME_IDLE_STAND);
		SetActorState(ruFUSION_ACTORSTATE_NORMAL);
		SetOrientingAxis(CRuVector3(0.0f, 1.0f, 0.0f));		
		AnchorSprite();

		// 重新設定角色參數
		if( m_pRuActor )
		{
			m_pRuActor->EnableSojourner(TRUE);
			m_pRuActor->Update(0.0f);
		}

		SetImplementSprite(NULL);
		//m_implementSprite = NULL;
		m_implementSitPos = 0;
	}
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetImplementSprite(CRoleSprite* sprite)
{
	if ( m_implementSprite != sprite )
	{
		m_implementSprite = sprite;

		s_procFunc(this, eSpriteMsg_Pet_Changed, (INT64)m_implementSprite, 0);
	}
}

//-----------------------------------------------------------------------------
bool CRoleSprite::IsDisableWagon()
{
	for ( vector<RoleBuffInfo>::iterator iter = m_buffInfo.begin(); iter != m_buffInfo.end(); iter++ )
	{
		GameObjDbStruct* obj = CNetGlobal::GetObj(iter->id);
		if ( obj != NULL )
		{
			if ( obj->MagicBase.Effect.DisableWagon )
				return true;
		}		
	}
	return false;
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetParentMountInfo(int parentMountId, CRuVector3 parentMountOffset)
{
	m_parentMountOffset = parentMountOffset;
	m_parentMountId = parentMountId;
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetImmediateScale(float scale)
{
	if ( m_immediateScale != scale )
	{
		ClearController(eSpriteController_Scale);

		SetScale((1.0f / m_immediateScale) * scale, eSpriteScale_Local);

		m_immediateScale = scale;
	}
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetAttachParent(CRoleSprite* parent, int point, bool boRotate)
{
	if ( m_attachParentSprite != parent )
	{
		m_attachParentSprite = parent;
		m_attachParentPoint = point;
		m_attachParentRotate = boRotate;

		if (m_attachParentSprite == NULL)
		{
			if ( GetGravityMode() )
			{
				AnchorSprite(40.0f);
			}

			SetDirection(m_direction, true);
		}
	}
}

//-----------------------------------------------------------------------------
// 游標是否移到物件上並且可以點選
bool CRoleSprite::GetCursorState()
{
	if ( m_controlState.cursor == false )
		return false;
	if ( m_controlState.cursorEnable == false )
		return false;
	if ( GetRuEntity() == NULL )
		return false;
	if ( IsScriptHide() )
		return false;
	if ( GetImplementSprite() )
		return false;
	if ( m_specialEffect.invisible > 0 )	// 
		return false;
	if ( GetSneakHiddenState() )
		return false;
	return true;
}

//-----------------------------------------------------------------------------
void CRoleSprite::CreateMountSprite(int orgObjID)
{
	ReleaseMountSprite();

	m_controlState.ride = true;
	m_originalMountId = orgObjID;

	if ( m_pRuEntity == NULL )
		return;

	m_mountSprite = new CRoleSprite;
	// Set mount parameters
#ifdef KALYDO
	// allow delay loading for mounts
	if ( !m_mountSprite->SetObjectId(orgObjID, true) ) {
#else
	if ( !m_mountSprite->SetObjectId(orgObjID) ) {
#endif // KALYDO
		// 坐騎產生錯誤
		// ReleaseMountSprite();
		delete m_mountSprite;
		m_mountSprite = NULL;
		return;
	}

	GameObjDbStruct* pImageObj = NULL;
	GameObjDbStruct* pObj = CNetGlobal::GetObj(orgObjID);
	GameObjectMountRideMotion rideMotion = EM_GameObjectMountRideMotion_Ride;
	if ( pObj )
	{
		pImageObj = CNetGlobal::GetObj(pObj->ImageID);
		if ( pImageObj )
		{
			rideMotion = pImageObj->Image.RideMotion;
		}
	}	

	m_mountSprite->SetHorseState(true);
	m_mountSprite->SetContainerState(false);
	m_mountSprite->SetObjectType(eSpriteObject_Player);
	m_mountSprite->SetGravityMode(true);
	m_mountSprite->SetPos(GetPos());
	m_mountSprite->SetVisible(true);
	m_mountSprite->SetOrientToSurfaceNormal(true);

	m_mountSprite->SetDirection(m_direction, true, true);
	m_mountSprite->SetMoveSpeed(GetMoveSpeed());	
	m_mountSprite->LinkSprite(this, ATTACH_POINT_RIDE_01, true, true);
	m_mountSprite->GetRuActor()->DisableInPlaceJump(TRUE);
	m_mountSprite->GetRuActor()->LinkActor(GetCurrentRuActor());
	m_mountSprite->SetDyeFXColors(m_dyeFXColors);

	/*
	RuEntity_SetOption(m_mountSprite->GetRuEntity(), ruENTITYOPTION_TRANSFORMMODE, ruENTITYTRANSFORMMODE_NOROTATE);	
	m_mountSprite->GetRuEntity()->SetOption(ruENTITYOPTION_TRANSFORMMODE, ruENTITYTRANSFORMMODE_NOROTATE);
	*/

	/*
	{
		RuEntity_Attach_BFS(m_mountSprite->GetRuEntity(), GetRuEntity(), ATTACH_POINT_RIDE_01);
	}
	*/

	/*
	// TEST
	{
		CRuEntity* entity = g_ruResourceManager->LoadEntity("model\\item\\weapon\\2h_blade\\2h_blade01.ros");
		if ( entity )
		{
			RuEntity_Attach_BFS(m_mountSprite->GetRuEntity(), entity, ATTACH_POINT_MOUTH);
		}
	}
	*/
	
	m_mountSprite->EnableTorsoTurn(false);

	// 原本 Attach 在角色 p_down 物件移置座騎 p_down
	for ( map<IBaseSprite*, int>::iterator iter = m_attachSprite.begin(); iter != m_attachSprite.end(); iter++ )
	{
		CRuEntity* entity = iter->first->GetRuEntity();

		if ( entity )
		{
			if ( iter->second == eAttach_Down )
			{
				entity->DetachFromParent();

				RuEntity_Attach_BFS(m_mountSprite->GetRuEntity(), entity, GetAttachPointName(iter->second));
			}
		}
	}

	// Set player parameters
	SetGravityMode(false);
	EnableTorsoTurn(false);

	switch (rideMotion)
	{
	case EM_GameObjectMountRideMotion_Ride1:
		SetDefaultIdleAction(ruFUSION_MIME_RIDE_IDLE0);
		break;

	case EM_GameObjectMountRideMotion_Ride2:
		SetDefaultIdleAction(ruFUSION_MIME_RIDE_IDLE3);
		break;

	case EM_GameObjectMountRideMotion_Stand:
		SetDefaultIdleAction(ruFUSION_MIME_RIDE_IDLE2);
		break;

	case EM_GameObjectMountRideMotion_Sit:
		SetDefaultIdleAction(ruFUSION_MIME_RIDE_IDLE1);
		break;

	case EM_GameObjectMountRideMotion_Ride:
	default:
		SetDefaultIdleAction(ruFUSION_MIME_RIDE_IDLE);		
		break;
	}

	if(m_pRuActor)
	{
		m_pRuActor->EnableSojourner(FALSE);
		m_pRuActor->Update(0.0f);

		/*
		// 坐騎
		if ( m_mountSprite->m_pRuActor )
			m_mountSprite->m_pRuActor->SetTransientState(ruFUSION_ACTORSTATE_EMOTE_JUMP_INPLACE);
		*/
	}
}

//-----------------------------------------------------------------------------
void CRoleSprite::ReleaseMountSprite()
{
	m_originalMountId = 0;

	// Dismount
	if ( m_mountSprite ) {
		// 解除連結
		m_mountSprite->UnlinkSprite(this);
		m_mountSprite->GetRuActor()->UnlinkActor(GetCurrentRuActor());

		SetGravityMode(true);
		EnableTorsoTurn(true);
		SetDefaultIdleAction(ruFUSION_MIME_IDLE_STAND);
		SetActorState(ruFUSION_ACTORSTATE_NORMAL);
		SetOrientingAxis(CRuVector3(0.0f, 1.0f, 0.0f));
		SetPos(m_mountSprite->GetPos());
		AnchorSprite();
		memset(m_dyeFXColors, 0, sizeof(int) * 4);

		// 移除 Detach 在座騎 p_down 物件移置角色 p_down
		for ( map<IBaseSprite*, int>::iterator iter = m_attachSprite.begin(); iter != m_attachSprite.end(); iter++ )
		{
			CRuEntity* entity = iter->first->GetRuEntity();
			CRuEntity* parentEntity = GetCurrentEntity();

			if ( entity )
			{
				if ( iter->second == eAttach_Down )
				{
					entity->DetachFromParent();

					RuEntity_Attach_BFS(parentEntity, entity, GetAttachPointName(iter->second));
				}
			}
		}

		// 重新設定角色參數
		if(m_pRuActor)
		{
			m_pRuActor->EnableSojourner(TRUE);
			m_pRuActor->Update(0.0f);
		}

		delete m_mountSprite;
		m_mountSprite = NULL;
	}
	m_controlState.ride = false;
}

//-----------------------------------------------------------------------------
void CRoleSprite::ResetMount()
{
	if ( m_originalMountId == 0 )
		return;

	GameObjDbStruct* pImageObj = NULL;
	GameObjDbStruct* pObj = CNetGlobal::GetObj(m_originalMountId);
	GameObjectMountRideMotion rideMotion = EM_GameObjectMountRideMotion_Ride;
	if ( pObj )
	{
		pImageObj = CNetGlobal::GetObj(pObj->ImageID);
		if ( pImageObj )
		{
			rideMotion = pImageObj->Image.RideMotion;
		}
	}

	// Set player parameters
	SetGravityMode(false);
	EnableTorsoTurn(false);	

	switch (rideMotion)
	{
	case EM_GameObjectMountRideMotion_Ride1:
		SetDefaultIdleAction(ruFUSION_MIME_RIDE_IDLE0);
		break;

	case EM_GameObjectMountRideMotion_Ride2:
		SetDefaultIdleAction(ruFUSION_MIME_RIDE_IDLE3);
		break;

	case EM_GameObjectMountRideMotion_Stand:
		SetDefaultIdleAction(ruFUSION_MIME_RIDE_IDLE2);
		break;

	case EM_GameObjectMountRideMotion_Sit:
		SetDefaultIdleAction(ruFUSION_MIME_RIDE_IDLE1);
		break;

	case EM_GameObjectMountRideMotion_Ride:
	default:
		SetDefaultIdleAction(ruFUSION_MIME_RIDE_IDLE);		
		break;
	}

	if(m_pRuActor)
	{
		m_pRuActor->EnableSojourner(FALSE);
		m_pRuActor->Update(0.0f);
	}
}

//-----------------------------------------------------------------------------
bool CRoleSprite::AttachMountSprite(CRoleSprite* parent, const char* point)
{
	if ( parent == NULL || parent == this )
		return false;

	if ( m_linkParent != parent )
	{
		// 解除物件之間連結
		DetachMountSprite();

		if ( parent )
		{
			CRoleSprite* proxySprite = parent->GetProxyControlSprite();
			if ( proxySprite )
			{
				strncpy(m_mountPointStr, point, 63);
				m_mountPointStr[63] = 0;
				
				proxySprite->LinkSprite(this, m_mountPointStr, true, true);
				proxySprite->GetRuActor()->LinkActor(GetCurrentRuActor());

				// 原本 Attach 在角色 p_down 物件移置座騎 p_down
				for ( map<IBaseSprite*, int>::iterator iter = m_attachSprite.begin(); iter != m_attachSprite.end(); iter++ )
				{
					CRuEntity* entity = iter->first->GetRuEntity();

					if ( entity )
					{
						if ( iter->second == eAttach_Down )
						{
							entity->DetachFromParent();

							RuEntity_Attach_BFS(proxySprite->GetRuEntity(), entity, GetAttachPointName(iter->second));
						}
					}
				}
			}

			// Set player parameters
			SetGravityMode(false);
			EnableTorsoTurn(false);

			// 設定騎乘動作
			GameObjDbStruct* pImageObj = NULL;
			GameObjDbStruct* pObj = CNetGlobal::GetObj(proxySprite->GetObjectId());
			GameObjectMountRideMotion rideMotion = EM_GameObjectMountRideMotion_Ride;
			if ( pObj )
			{
				pImageObj = CNetGlobal::GetObj(pObj->ImageID);
				if ( pImageObj )
				{
					rideMotion = pImageObj->Image.RideMotion;
				}
			}	

			switch (rideMotion)
			{
			case EM_GameObjectMountRideMotion_Ride1:
				SetDefaultIdleAction(ruFUSION_MIME_RIDE_IDLE0);
				break;

			case EM_GameObjectMountRideMotion_Ride2:
				SetDefaultIdleAction(ruFUSION_MIME_RIDE_IDLE3);
				break;

			case EM_GameObjectMountRideMotion_Stand:
				SetDefaultIdleAction(ruFUSION_MIME_RIDE_IDLE2);
				break;

			case EM_GameObjectMountRideMotion_Sit:
				SetDefaultIdleAction(ruFUSION_MIME_RIDE_IDLE1);
				break;

			case EM_GameObjectMountRideMotion_Ride:
			default:
				SetDefaultIdleAction(ruFUSION_MIME_RIDE_IDLE);		
				break;
			}

			if(m_pRuActor)
			{
				m_pRuActor->EnableSojourner(FALSE);
				m_pRuActor->Update(0.0f);
			}

			m_attachParentId = parent->GetIdentify();

			m_specialEffect.lockedMove++;
		}
	}

	return true;
}

//-----------------------------------------------------------------------------
void CRoleSprite::DetachMountSprite()
{
	if ( m_attachParentId != 0 )
	{
		CRuVector3 parentPos = m_nowPos;
		if ( m_linkParent )
		{
			parentPos = m_linkParent->GetPos();
		}

		SeverSpriteLinks();

		// 找尋可以坐落點
		{
			CRuQuaternion cycleQuat;
			CRuVector3 forward = m_direction;
			CRuVector3 nowPos = parentPos + CRuVector3(0.0f, 10.0f, 0.0f);
			forward.m_y = 0.0f;
			forward.Normalize();
			nowPos += forward * -1.5f;
			
			cycleQuat.FromAngleAxis(CRuVector3(0.0, 1.0f, 0.0f), ruPI / 4.0f);
			cycleQuat.TransformVector(forward);

			nowPos += forward * 10.0f;

			// 設定測試用線段及其他設定
			REAL nearestMountPointDist = FLT_MAX;	
			CRuCollisionQuery_Segment_Nearest segQuery;	
			segQuery.m_colSegment = CRuSegment3( m_nowPos, nowPos );
			segQuery.m_exclusionMask = RuWorld::ruTRIFLAGS_WATER | RuWorld::ruTRIFLAGS_FOLIAGE;
			segQuery.m_ignoreBackface = FALSE;
			segQuery.m_ignoreCollisionFlag = TRUE;
			
			if ( s_collisionContainer )
			{
				BOOL removeResult = FALSE;

				// 暫時移除自己,避免測試到自已
				if ( m_enableCollision )
					removeResult = s_collisionContainer->RemoveCollisionObject(m_pRuEntity);

				s_collisionContainer->QueryCollision(&segQuery);

				if ( removeResult )
					s_collisionContainer->InsertCollisionObject(m_pRuEntity, m_pRuEntity->GetWorldBounds(), TRUE);
			}
			
			if( !segQuery.m_positiveHit ) {
				IBaseSprite::SetPos(nowPos);
			} else {
				IBaseSprite::SetPos(parentPos);
			}
			AnchorSprite(10.0f);
		}

		SetGravityMode(true);
		EnableTorsoTurn(true);
		SetDefaultIdleAction(ruFUSION_MIME_IDLE_STAND);
		SetActorState(ruFUSION_ACTORSTATE_NORMAL);
		SetOrientingAxis(CRuVector3(0.0f, 1.0f, 0.0f));
		AnchorSprite(20.0f);

		// 移除 Detach 在座騎 p_down 物件移置角色 p_down
		for ( map<IBaseSprite*, int>::iterator iter = m_attachSprite.begin(); iter != m_attachSprite.end(); iter++ )
		{
			CRuEntity* entity = iter->first->GetRuEntity();
			CRuEntity* parentEntity = GetCurrentEntity();

			if ( entity )
			{
				if ( iter->second == eAttach_Down )
				{
					entity->DetachFromParent();

					RuEntity_Attach_BFS(parentEntity, entity, GetAttachPointName(iter->second));
				}
			}
		}

		// 重新設定角色參數
		if(m_pRuActor)
		{
			m_pRuActor->EnableSojourner(TRUE);
			m_pRuActor->Update(0.0f);
		}

		// 鎖定移動
		if ( m_specialEffect.lockedMove > 0 )
			m_specialEffect.lockedMove--;

		m_attachParentId = 0;
	}	
}

//-----------------------------------------------------------------------------
bool CRoleSprite::IsInWater()
{
	if ( m_pRuActor &&
		(m_pRuActor->IsInWater() ||
		 m_pRuActor->GetBackgroundState() == ruFUSION_ACTORSTATE_SWIM_SURFACE ||
		 m_pRuActor->GetBackgroundState() == ruFUSION_ACTORSTATE_SWIM_DIVE) )
		 return true;
	return false;
}

/*
//-----------------------------------------------------------------------------
void CRoleSprite::CreateFlagEntity(int id, const char* insignia, int insigniaMainColor, int insigniaOffColor, const char* banner, int bannerMainColor, int bannerOffColor)
{
	const char* entityFileName = "model\\item\\guild_standard\\act_guild_standard.ros";
	ReleaseOrnamentEntity();

	m_ornamentID = id;

	if ( insignia == NULL )
		insignia = "";
	if ( banner == NULL )
		banner = "";

	GameObjDbStruct* pObj = NULL;
	if ( (pObj = CNetGlobal::GetObj( id )) == NULL )
	{
		// 測試用旗幟
		//m_pRuFlagEntity = g_ruResourceManager->LoadEntity("model\\element\\guildhall_war\\act_war_flag.ros");
		//CRuQuaternion quaternion;
		//quaternion.FromAngleAxis( CRuVector3(0.0f, 1.0f, 0.0f), ruPI * -0.5f);
		//m_pRuFlagEntity->Rotate(quaternion);
		//m_pRuFlagEntity->SetTranslation( CRuVector3(0.0f, -4.0f, -1.0f) );
		//m_pRuFlagEntity->Scale(0.5f);
		//m_pRuFlagEntity->Update(0.0f);
		//RuACT_PlayMotion(m_pRuFlagEntity, "idle");
		//AddOnEntity(true, m_pRuFlagEntity, eAttach_Back_Shield);		

		return;
	}

	// Load entity
	m_pRuFlagEntity = g_ruResourceManager->LoadEntity(entityFileName);
	if ( m_pRuFlagEntity )
	{
		// 設定類型
		RuPaperdoll_SetComponent( m_pRuFlagEntity, "flagstaff", pObj->ImageObj->ACTWorld);
		RuPaperdoll_SetComponent( m_pRuFlagEntity, "insignia", insignia );
		RuPaperdoll_SetComponent( m_pRuFlagEntity, "banner", banner );

		// 設定顏色
		BOOL layerActivated[4] = {TRUE, TRUE, TRUE, TRUE};
		RUCOLOR layerColor[4] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};		
		layerColor[2] = insigniaMainColor;
		layerColor[3] = insigniaOffColor;
		RuPaperdoll_SetComponentColors( m_pRuFlagEntity, "insignia", layerActivated, layerColor);

		layerColor[2] = bannerMainColor;
		layerColor[3] = bannerOffColor;
		RuPaperdoll_SetComponentColors( m_pRuFlagEntity, "banner", layerActivated, layerColor);

		RuPaperdoll_Build( m_pRuFlagEntity , false );

		CRuQuaternion quaternion;
		quaternion.FromAngleAxis( CRuVector3(0.0f, 1.0f, 0.0f), ruPI);
		m_pRuFlagEntity->Rotate(quaternion);

		m_pRuFlagEntity->SetTranslation( CRuVector3(0.0f, -5.5f, -0.8f) );
		m_pRuFlagEntity->Update(0.0f);
		m_pRuFlagEntity->Scale(0.5f);
		// RuACT_PlayMotion(m_pRuFlagEntity, "idle");
		AddOnEntity(true, m_pRuFlagEntity, eAttach_Back_Shield);
	}
}
*/

//-----------------------------------------------------------------------------
// 產生裝飾品物件
void CRoleSprite::CreateOrnamentEntity(int id, const char* insignia, int insigniaMainColor, int insigniaOffColor, const char* banner, int bannerMainColor, int bannerOffColor, bool motionRelease)
{
	// 移除舊有裝飾品物件
	ReleaseOrnamentEntity();	

	m_pRuOrnamentEntity = LoadOrnamentEntity(id, insignia, insigniaMainColor, insigniaOffColor, banner, bannerMainColor, bannerOffColor);
	if ( m_pRuOrnamentEntity )
	{
		m_ornamentID = id;
		AttachOrnamentEntity(motionRelease);
	}

	// 更新頭像框
	if ( s_procFunc )
		s_procFunc(this, eSpriteMsg_Portrait_Changed, 0, 0);
}

//-----------------------------------------------------------------------------
void CRoleSprite::ReleaseOrnamentEntity()
{
	if ( m_pRuOrnamentEntity )
	{
		DetachOrnamentEntity(true);
		ruSAFE_RELEASE(m_pRuOrnamentEntity);
	}
	m_ornamentID = 0;
	m_ornamentMotion.clear();
}

//-----------------------------------------------------------------------------
void CRoleSprite::AttachOrnamentEntity(bool motionRelease)
{
	if ( m_pRuOrnamentEntity == NULL || m_pRuOrnamentEntity->GetParent() != NULL )
		return;

	// 附加到主物件上
	AddOnEntity(true, m_pRuOrnamentEntity, eAttach_Back_Shield);

	if ( motionRelease )
	{
		// 撥放動作
		PlayMotionOrnament("idle");
	}
	else
	{
		// 撥放動作
		PlayMotionOrnament("appear");
	}	
}

//-----------------------------------------------------------------------------
// 裝飾品物件消失
void CRoleSprite::DetachOrnamentEntity(bool immediately)
{
	if ( m_pRuOrnamentEntity == NULL || m_pRuOrnamentEntity->GetParent() == NULL )
		return;

	// 立即移除
	if ( immediately )
	{
		AddOnEntity(false, m_pRuOrnamentEntity, eAttach_Back_Shield);
	}
	// 等待 Disappear 時間到解除
	else
	{
		// 撥放動作
		PlayMotionOrnament("disappear");

		// "disappear" Motion 不存在立即解除
		if ( strcmp(m_ornamentMotion.c_str(), "idle") == 0 ) {
			AddOnEntity(false, m_pRuOrnamentEntity, eAttach_Back_Shield);
		}
	}
}

//-----------------------------------------------------------------------------
void CRoleSprite::UpdateOrnamentEntity()
{
	if (m_pRuOrnamentEntity)
	{		
		if ( strcmp(m_ornamentMotion.c_str(), "appear") == 0 )
		{
			// appear -> idle
			if ( RuACT_IsMotionComplete(m_pRuOrnamentEntity, 0xFFFFFFFF, TRUE) == TRUE ) {
				PlayMotionOrnament("idle");
			}
		}
		else if ( strcmp(m_ornamentMotion.c_str(), "idle") == 0 )
		{
		}
		else if ( strcmp(m_ornamentMotion.c_str(), "disappear") == 0 )
		{
			// disappear -> detach
			if ( RuACT_IsMotionComplete(m_pRuOrnamentEntity, 0xFFFFFFFF, TRUE) == TRUE ) {
				DetachOrnamentEntity();
			}
		}
		else if ( strcmp(m_ornamentMotion.c_str(), "explode") == 0 )
		{
			// explode -> idle
			if ( RuACT_IsMotionComplete(m_pRuOrnamentEntity, 0xFFFFFFFF, TRUE) == TRUE ) {
				PlayMotionOrnament("idle");
			}
		}
	}
}

//-----------------------------------------------------------------------------
bool CRoleSprite::PlayMotionOrnament(const char* motionName)
{
	if ( motionName == NULL || m_pRuOrnamentEntity == NULL )
		return false;

	m_ornamentMotion = motionName;
	if ( strcmp(motionName, "appear") == 0 ) {
		if ( RuACT_PlayMotion(m_pRuOrnamentEntity, motionName) == FALSE ) {
			PlayMotionOrnament("idle");
		}
	} else if ( strcmp(motionName, "idle") == 0 ) {
		if ( RuACT_PlayMotion(m_pRuOrnamentEntity, motionName) == FALSE ) {
			// 這有問題,不應該沒有idle動作
		}
	} else if ( strcmp(motionName, "disappear") == 0 ) {
		if ( RuACT_PlayMotion(m_pRuOrnamentEntity, motionName) == FALSE ) {
			PlayMotionOrnament("idle");
		}
	} else if ( strcmp(motionName, "explode") == 0 ) {
		 
		if ( RuACT_PlayMotion(m_pRuOrnamentEntity, motionName) == FALSE ) {
			PlayMotionOrnament("idle");
		}
	} else {
		m_ornamentMotion.clear();
		return false;
	}
	return true;
}

//-----------------------------------------------------------------------------
void CRoleSprite::CreateBackEntity(int id)
{
	// 移除舊有裝飾品物件
	ReleaseBackEntity();

	m_pRuBackEntity = LoadOrnamentEntity(id, NULL, 0, 0, NULL, 0, 0);
	if ( m_pRuBackEntity == NULL )
		return;
	
	m_backID = id;	
	AttachBackEntity();
}

//-----------------------------------------------------------------------------
void CRoleSprite::ReleaseBackEntity()
{
	if ( m_pRuBackEntity )
	{
		DetachBackEntity();
		ruSAFE_RELEASE(m_pRuBackEntity);
	}
	m_backID = 0;
}

//-----------------------------------------------------------------------------
void CRoleSprite::AttachBackEntity()
{
	if ( m_pRuBackEntity == NULL || m_pRuBackEntity->GetParent() != NULL )
		return;

	// 附加到主物件上
	AddOnEntity(true, m_pRuBackEntity, eAttach_Back_Shield);

	// 撥放動作
	RuACT_PlayMotion(m_pRuBackEntity, "idle");
}

//-----------------------------------------------------------------------------
void CRoleSprite::DetachBackEntity()
{
	if ( m_pRuBackEntity == NULL || m_pRuBackEntity->GetParent() == NULL )
		return;

	AddOnEntity(false, m_pRuBackEntity, eAttach_Back_Shield);
}

//-----------------------------------------------------------------------------
void CRoleSprite::CreateCorpseEffect()
{
	ReleaseCorpseEffect();

	m_pCorpseEffect = new CEffectSprite;
	if ( m_pCorpseEffect )
	{
		if ( m_pCorpseEffect->LoadRuEntity("model\\item\\ui_object\\pick_fx\\act_pickup_t_freeze_down_i.ros") == false )
		{
			ReleaseCorpseEffect();
			return;
		}

		m_pCorpseEffect->SetAutoDestruct(false);
		m_pCorpseEffect->AttachSprite(this, eAttach_Down);
		m_pCorpseEffect->SetEffectMotionStep(eEffectMotion_Step_Idle);
	}
}

//-----------------------------------------------------------------------------
void CRoleSprite::ReleaseCorpseEffect()
{
	SAFE_DELETE(m_pCorpseEffect);
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetGoodEvilType(GoodEvilTypeENUM type, float value)
{
	m_goodEvilType = type;
	m_goodEvilValue = value;
	m_isNeedResetWidget = true;
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetRisingimpulse(float risingimpulse)
{
	m_risingimpulse = min(max(risingimpulse, 0.1f), 3.0f);
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetGravityAcceleration(float gravityAcceleration)
{
	m_gravityAcceleration = min(max(gravityAcceleration, 0.1f), 3.0f);
	SetGravityMode(m_controlState.gravityState);
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetRaidMarkTarget(int markNumber)
{
	if ( m_raidMarkNumber != markNumber )
	{
		m_raidMarkNumber = markNumber;
		m_isNeedResetWidget = true;
	}
}

//-----------------------------------------------------------------------------
float CRoleSprite::GetClientTouchRange()
{
	return max(m_attackRange, _MAX_ROLE_TOUCH_RANGE_);
}

//-----------------------------------------------------------------------------
bool CRoleSprite::AttachSprite(IBaseSprite* sprite, int point)
{
	if ( point == eAttach_Down && m_mountSprite )
	{
		if ( sprite )
		{
			CRuEntity* childEntity = sprite->GetRuEntity();
			CRuEntity* parentEntity = m_mountSprite->GetRuEntity();

			if ( parentEntity )
			{
				RuEntity_Attach_BFS(parentEntity, childEntity, GetAttachPointName(point));

				// 記錄目前已經 Attach Entity, 在變身時需要做替換
				m_attachSprite.insert(make_pair(sprite, point));

				return true;
			}
		}
	}
	else
		return IBaseSprite::AttachSprite(sprite, point);	

	return false;
}

//-----------------------------------------------------------------------------
void CRoleSprite::FollowSprite(CRoleSprite* targetSprite, const char* targetPoint, const char* point)
{
	float distance = 0.0f;

	if ( targetSprite )
	{
		CRuVector3 p1, p2;

		// 取得目的坐標點
		p1 = targetSprite->GetAttachPos(targetPoint);

		// 取得距離
		distance += (p1 - targetSprite->GetPos()).Magnitude();

		// 取得來源坐標點
		p2 = GetAttachPos(point);

		// 取得距離
		distance += (p2 - GetPos()).Magnitude();
	}

	FollowSprite(targetSprite, distance);
}

//-----------------------------------------------------------------------------
void CRoleSprite::FollowSprite(CRoleSprite* targetSprite, float offset)
{
	m_followSprite = targetSprite;
	m_followOffset = offset;

	if ( m_followSprite )
	{		
		CRuVector3 direction = m_followSprite->GetDirection();
		CRuVector3 newPos = m_followSprite->GetPos();

		// 設定相同面向
		SetDirection(direction, true);

		// 新坐標(減掉反方向向量)
		newPos -= direction * offset;

		// 定位Y軸
		newPos.m_y = RuEntity_FindRestingHeight(newPos, s_pRuTerrain, 100.0f);

		SetPos(newPos);
	}
}

//-----------------------------------------------------------------------------
// 跟隨另一物件移動
void CRoleSprite::UpdateFollow(float elapsedTime)
{
	if ( m_followSprite )
	{
		CRuVector3 position = m_followSprite->GetPos();
		CRuVector3 moveVector = position - m_nowPos;
		float distance = moveVector.Magnitude();

		float speed = m_followSprite->GetMoveSpeed();
		if ( m_followSprite->GetWalkState() )
			speed /= WALK_SLOW_SCALE;

		if ( distance > m_followOffset )
		{
			SetCorrectDir(CRuVector3(0.0f, 0.0f, 0.0f));
			SetDirection(moveVector);
			SetMoveVector(CRuVector3(0.0f, 0.0f, speed), NEXT_MOVETIME);
		}
	}
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetLock(LookStruct& lock)
{
	m_originalLock = lock;

	SetPaperdollColor(PAPERDOLL_PART_NAME_HEAD, lock.BodyColor, 0);
	SetPaperdollColor(PAPERDOLL_PART_NAME_HAIR, lock.HairColor, 0);
	SetPaperdollPart(PAPERDOLL_PART_NAME_HEAD, lock.FaceID);
	SetPaperdollPart(PAPERDOLL_PART_NAME_HAIR, lock.HairID);

	SetBoneScale(BONE_SCALE_NAME_HEAD,		(BYTE) lock.BoneScale[0] / 100.0f);
	SetBoneScale(BONE_SCALE_NAME_BREASTS,	(BYTE) lock.BoneScale[1] / 100.0f);
	SetBoneScale(BONE_SCALE_NAME_BODY,		(BYTE) lock.BoneScale[2] / 100.0f);
	SetBoneScale(BONE_SCALE_NAME_CHEST,		(BYTE) lock.BoneScale[3] / 100.0f);
	SetBoneScale(BONE_SCALE_NAME_PELVIS,	(BYTE) lock.BoneScale[4] / 100.0f);
	SetBoneScale(BONE_SCALE_NAME_ARM,		(BYTE) lock.BoneScale[5] / 100.0f);
	SetBoneScale(BONE_SCALE_NAME_FOREARM,	(BYTE) lock.BoneScale[6] / 100.0f);
	SetBoneScale(BONE_SCALE_NAME_HAND,		(BYTE) lock.BoneScale[7] / 100.0f);
	SetBoneScale(BONE_SCALE_NAME_LEGS,		(BYTE) lock.BoneScale[8] / 100.0f);
	SetBoneScale(BONE_SCALE_NAME_CALF,		(BYTE) lock.BoneScale[9] / 100.0f);
	SetBoneScale(BONE_SCALE_NAME_FOOT,		(BYTE) lock.BoneScale[10] / 100.0f);
	PaperdollBuild();
}

//-----------------------------------------------------------------------------
void CRoleSprite::CreateEquipmentInfo(void* data, int size)
{
	ReleaseEquipmentInfo();

	if ( data && size > 0 )
	{		
		m_equpmentInfo = new char[size];
		memcpy(m_equpmentInfo, data, size);
	}	
}

//-----------------------------------------------------------------------------
void CRoleSprite::ReleaseEquipmentInfo()
{
	if ( m_equpmentInfo ) 
	{
		delete [] m_equpmentInfo;
		m_equpmentInfo = NULL;
	}
}

//-----------------------------------------------------------------------------
void CRoleSprite::SetHorizontal(bool isHorizontal)
{
	if ( m_objectId == 117218 ) {
		g_pError->AddMessage(0, 0, "SetHorizontal %s(old:%s)", (isHorizontal) ? "true":"false", (m_isHorizontal) ? "true" : "false");
	}
	
	if ( m_isHorizontal == isHorizontal )
		return;

	m_isHorizontal = isHorizontal;
	if ( m_isHorizontal ) {
		SetOrientingAxis(CRuVector3(0.0f, 1.0f, 0.0f), true);
	}
}

// ---------------------------------------------------------------------------
bool CRoleSprite::QueryCollision(CRuVector3& beginPoint, CRuVector3& endPoint, CRuVector3& outCollisionPoint, bool boTerrain, bool spriteCollision, CRuEntity* ignoreEntity)
{
	CRuCollisionQuery_Segment_Nearest segQuery;	
	REAL nearestMountPointDist = FLT_MAX;	

	if ( boTerrain )
	{
		// 設定測試用線段及其他設定
		segQuery.m_colSegment = CRuSegment3( beginPoint, endPoint );
		segQuery.m_exclusionMask = RuWorld::ruTRIFLAGS_WATER /*| RuWorld::ruTRIFLAGS_FOLIAGE*/;
		segQuery.m_ignoreBackface = FALSE;
		segQuery.m_ignoreCollisionFlag = FALSE;

		CRuContainer_Collision* collisionContainer = IBaseSprite::GetCollisionContainer();
		if ( collisionContainer )
		{
			BOOL removeResult = FALSE;

			// 暫時移除自己,避免測試到自已
			if ( ignoreEntity )
			{
				removeResult = collisionContainer->RemoveCollisionObject(ignoreEntity);
			}

			collisionContainer->QueryCollision(&segQuery);

			if ( ignoreEntity && removeResult )
				collisionContainer->InsertCollisionObject(ignoreEntity, ignoreEntity->GetWorldBounds(), TRUE);
		}

		// Keep the closest candidate sprite
		if(segQuery.m_positiveHit && segQuery.m_collisionT < nearestMountPointDist)
		{
			nearestMountPointDist = segQuery.m_collisionT;
			outCollisionPoint = segQuery.m_collisionPoint;
		}
	}	

	return nearestMountPointDist < FLT_MAX;
}

//-----------------------------------------------------------------------------
int CRoleSprite::PushCheckPosition(const CRuVector3& point)
{
	if ( m_reportErrorPosition )
		return 0;

	if ( m_checkPosition.size() >= 2 ) {
		m_checkPosition[0] = m_checkPosition[1];
		m_checkPosition[1] = point;
	} else {
		m_checkPosition.push_back(point);
	}

	if ( m_checkPosition.size() >= 2 )
	{
		CRuVector3 p1 = m_checkPosition[0];
		CRuVector3 p2 = m_checkPosition[1];
		CRuVector3 p3;

		// 距離過遠
		if ( (p1 - p2).Magnitude() > m_moveSpeed * 2 ) {
			m_reportErrorPosition = true;
			return -2;
		}

		p1.m_y += 6.0f;
		p2.m_y += 6.0f;

		// 多測幾條線避免剛好中間有空隙
		int errorCount = 0;
		for ( float i = 0.0f; i < 10.0f; i++ ) {
			p1.m_y += i;
			p2.m_y += i;
			if ( QueryCollision(p1, p2, p3, true, true, GetRuEntity()) )
			{
				errorCount++;
			}
		}

		// 穿牆
		if ( errorCount > 4 ) {
			m_reportErrorPosition = true;
			return -1;
		}

		p1 = m_checkPosition[0];
		p2 = m_checkPosition[1];
		p3 = p2 + CRuVector3(0.0f, 10.0f, 0.0f);

		// 檢查浮空
		float height = p2.m_y;
		if ( p2.m_y >= p1.m_y && !TerrainTestHight(p3, height, 100.0f) || (p2.m_y - height) > 32.0f )
		{
			m_floatingCount++;
			if ( m_floatingCount > 10 ) {
				m_reportErrorPosition = true;
				return -3;
			}
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
void CRoleSprite::ClearCheckPosition()
{
	m_checkPosition.clear();
}