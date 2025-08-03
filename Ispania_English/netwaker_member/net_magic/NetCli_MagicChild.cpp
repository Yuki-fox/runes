#include <map>
#include "NetCli_MagicChild.h"
#include "../mainproc/GameMain.h"
#include "../Interface/debug/DebugMessage.h"

#include "../Interface/WorldFrames/ChatFrame.h"
#include "../Interface/WorldFrames/CastingBarFrame.h"
#include "../Interface/WorldFrames/ScrollCombatTextFrame.h"
#include "../Interface/WorldFrames/ActionFrame.h"
#include "../Interface/WorldFrames/BagFrame.h"
#include "../Interface/WorldFrames/CharacterFrame.h"
#include "../Interface/SkillBook/UI_SkillBook.h"

#include "../net_party/NetCli_PartyChild.h"
#include "../net_move/NetCli_MoveChild.h"
#include "../../Interface/WorldFrames/CombatMeter.h"

//#define DEBUG_MAGIC							1

//-----------------------------------------------------------------------------
bool NetCli_MagicChild::_Init()
{
	if (This)
		return false;

	This = new NetCli_MagicChild;
	return NetCli_Magic::_Init();
}

//-----------------------------------------------------------------------------
bool NetCli_MagicChild::_Release()
{
	NetCli_Magic::_Release();
	if (This)
	{
		delete This;
		This = NULL;
	}
	return true;
}

//-----------------------------------------------------------------------------
void NetCli_MagicChild::R_BeginSpell( int OwnerID , int TargetID , float TargetX , float TargetY , float TargetZ , int MagicID , int SerialID , int SpellTime )
{
	//g_pDebugMessage->DebugOutput(0, "[R_BeginSpell] OwnerID %d TargetId %d MagicID%d SerialID%d", OwnerID , TargetID , MagicID, SerialID);	
#ifdef DEBUG_MAGIC
	g_pError->AddMessage(0, 0, "[R_BeginSpell Time:%.2f] OwnerID:%d TargetId:%d MagicID:%d SerialID:%d TargetPos(%.2f,%.2f,%.2f)", g_pGameMain->GetFrameTime(), OwnerID , TargetID , MagicID, SerialID, TargetX, TargetY, TargetZ);	
#endif

	CRoleSprite* sprite = g_pGameMain->FindSpriteByID(OwnerID);

	// 取得法術結構
	GameObjDbStructEx* itemDB = CNetGlobal::GetObj(MagicID);

	if ( itemDB == NULL )
		return;

	// 檢查法術類型
	switch ( itemDB->MagicCol.TargetType )
	{
	case EM_Releation_Locatoin:
		TargetID = 0;
		break;

	default:
		TargetX = 0.0f;
		TargetY = 0.0f;
		TargetZ = 0.0f;
		break;
	}

	// 如果是隱藏物件(劇情表演用)	
	if ( sprite && sprite->IsScriptHide() )
		return;

	CNetMagicPackageData* magicData = g_pACTActionEventsManager->CreateMagicData(SerialID);
	magicData->AddRefCount();
	magicData->m_x			= TargetX;
	magicData->m_y			= TargetY;
	magicData->m_z			= TargetZ;
	magicData->m_magic		= MagicID;
	magicData->m_owner		= OwnerID;
	magicData->m_target		= TargetID;	
	magicData->m_spellTime	= SpellTime;

	if ( sprite != NULL )
	{
		CRoleSprite* target = g_pGameMain->FindSpriteByID(TargetID);
		CRoleSprite* player = g_pGameMain->GetPlayer();

		// 被攻擊者是自己並且沒有目標,將攻擊者設為目標
		if ( target && player ) {
			if ( target == player && player->GetTargetSprite() == NULL  &&!player->IsCantCtrl()){
				g_pGameMain->SetPlayerTarget(sprite);
			}
		}		

		// 若是NPC暫時停止轉向
		switch ( sprite->GetObjectType() )
		{
		case eSpriteObject_NPC:
		case eSpriteObject_QuestNPC:
			if ( target )
			{
				// 目標不是自己
				if ( target != sprite )
				{					
					sprite->SetCorrectDir(CRuVector3(0.0f, 0.0f, 0.0f));
					sprite->SetDirection(target->GetPos() - sprite->GetPos());

					// 是否有鎖定面向施法目標
					if ( itemDB && itemDB->MagicCol.IsLockTarget )
						sprite->LockedDirection(true);
					else
						sprite->LockedDirection(false);
				}
				// 目標是自己,暫時停止轉動
				else
				{
					sprite->LockedDirection(true);
				}
			}
			else
			{
				CRuVector3 targetPosition;
				targetPosition.m_x = TargetX;
				targetPosition.m_y = TargetY;
				targetPosition.m_z = TargetZ;

				sprite->LockedDirection(true);
				sprite->SetCorrectDir(CRuVector3(0.0f, 0.0f, 0.0f));
				sprite->SetDirection(targetPosition - sprite->GetPos());
			}			
			break;
		}		
		
		if ( itemDB )
		{
			GameObjDbStructEx* imageDB = g_ObjectData->GetObj(itemDB->ImageID);			
			if ( sprite == g_pGameMain->GetPlayer() )
			{
				//sprite->BeginSpell(MagicID, false, target);
				sprite->BeginSpell(MagicID, true, target);
				if ( SpellTime > 0 && itemDB->MagicCol.Flag.HideCastingBar == false )
					g_pCastingBarFrame->CastingStart(eCastingBar_SpellMagic, itemDB->GetName(), SpellTime / 1000.0f);
			}
			else
				sprite->BeginSpell(MagicID, SpellTime > 0, target);

			/*
			// 鎖定面向
			if ( isNPC && itemDB->MagicCol.IsLockTarget == false )
			{
				sprite->SetLockSprite(NULL);
			}
			*/

			// 顯示施法時間
			if ( SpellTime > 0 && itemDB->MagicCol.Flag.HideCastingBar == false )
			{
				// 設定施法時間
				sprite->SetCastingTime(SpellTime / 1000.0f);
			}

			if ( imageDB )
			{
				switch ( CRoleSprite::MotionToActorState(imageDB->Image.ShootMotion) )
				{
				case ruFUSION_ACTORSTATE_ATTACK_BOW_BEGIN:
				case ruFUSION_ACTORSTATE_ATTACK_BOW_LOOP:
				case ruFUSION_ACTORSTATE_ATTACK_GUN_BEGIN:
				case ruFUSION_ACTORSTATE_ATTACK_GUN_LOOP:
				case ruFUSION_ACTORSTATE_BUFF_BEGIN:
				case ruFUSION_ACTORSTATE_BUFF_LOOP:
				case ruFUSION_ACTORSTATE_CAST_BEGIN:
				case ruFUSION_ACTORSTATE_CAST_LOOP:
				case ruFUSION_ACTORSTATE_CHANNEL_BEGIN:
				case ruFUSION_ACTORSTATE_CHANNEL_LOOP:
					g_pACTActionEventsManager->CreateExplodeEffect(SerialID);
					break;
				}
			}			
		}		
	}
}

//-----------------------------------------------------------------------------
void NetCli_MagicChild::R_EndSpell( int SerialID )
{
	//g_pDebugMessage->DebugOutput(0, "[R_EndSpell] SerialID%d", SerialID);
#ifdef DEBUG_MAGIC
	g_pError->AddMessage(0, 0, "[R_EndSpell] SerialID%d", SerialID);
#endif

	CNetMagicPackageData* magicData = g_pACTActionEventsManager->GetMagicData(SerialID);
	if ( magicData == NULL )
		return;

	CRoleSprite* player = g_pGameMain->GetPlayer();
	CRoleSprite* sprite = g_pGameMain->FindSpriteByID(magicData->m_owner);
	if ( sprite )
	{
		sprite->EndSpell();
		sprite->SetCastingTime(0.0f);

		switch ( sprite->GetObjectType() )
		{
		case eSpriteObject_NPC:
		case eSpriteObject_QuestNPC:
			sprite->LockedDirection(false);
			break;
		}		

		// 玩家自己
		if ( sprite == player )
		{
			g_pGameMain->SetCastSpellState(CASTSPELL_NONE, true);
			g_pGameMain->ClearCastSpellCount();
			g_pGameMain->ClearCastSpellTime();
			g_pGameMain->ClearCancelSpellState();
			g_pGameMain->ClearSendSpell();

			// 中斷施法條
			if ( magicData->m_spellTime > 0 )
				g_pCastingBarFrame->CastingStop(eCastingBar_SpellMagic);
		}

		// 寵物條
		if ( sprite->GetIdentify() == g_pGameMain->GetPetGUID() )
		{
			g_pActionFrame->UpdatePetColdown(magicData->m_magic);
		}		

		// 載具技能
		if ( player && sprite == player->GetImplementSprite() )
		{
			NetCli_MoveChild::SetImplementMagicColdown(sprite->GetIdentify(), magicData->m_magic);
		}
	}
	magicData->EndSpell();	
}

//-----------------------------------------------------------------------------
void NetCli_MagicChild::R_SysBeginShoot( int OwnerID , int TargetID , int MagicColID , int SerialID , float TX , float TY , float TZ )
{
	//g_pDebugMessage->DebugOutput(0, "[R_SysBeginShoot] OwnerID %d TargetId %d MagicID%d SerialID%d", OwnerID , TargetID , MagicColID, SerialID);
#ifdef DEBUG_MAGIC
	g_pError->AddMessage(0, 0, "[R_SysBeginShoot] OwnerID:%d TargetId:%d MagicID:%d SerialID:%d TargetPos:(%.2f,%.2f,%.2f)", OwnerID , TargetID , MagicColID, SerialID, TX, TY, TZ);
#endif

	CNetMagicPackageData* magicData = g_pACTActionEventsManager->CreateMagicData(SerialID);
	if ( magicData == NULL )
		return;

	magicData->m_magic		= MagicColID;
	magicData->m_owner		= OwnerID;
	magicData->m_target		= TargetID;	
	magicData->m_spellTime	= 0;

	// 出手特效
	GameObjDbStruct* itemDB = CNetGlobal::GetObj(MagicColID);
	if ( itemDB )
	{
		// 產生單體法術資料
		int shootIndex = magicData->CreateShoot(1, &TargetID, CRuVector3(TX, TY, TZ));
		
		CRoleSprite* owner = g_pGameMain->FindSpriteByID(OwnerID);
		if ( owner )
		{
			// 動作已結束
			magicData->SetShootActionDone();
			magicData->SetActionDone();

			// 出手特效
			g_pACTActionEventsManager->CreateShootEffect(SerialID, shootIndex);

			// 產生特效,沒有動作觸發時間
			if ( !g_pACTActionEventsManager->CreateFlyEffect(SerialID, shootIndex) ) {
				g_pACTActionEventsManager->CreateExplodeEffect(SerialID);
			}

			/*
			// 出手特效
			if ( strlen(gois->ShootACT) > 0 )
			{
				CEffectSprite* shoot = new CEffectSprite;
				if ( shoot->LoadRuEntity(gois->ShootACT) )
				{
					shoot->SetDirection(owner->GetDirection());
					shoot->AttachSprite(owner, IBaseSprite::GetAttachPointEnum(gois->ShootPoint));
					shoot->SetEffectMotionStep(eEffectMotion_Step_Explode);
				}
				else
					delete shoot;
			}
			*/
		}
	}
}

//-----------------------------------------------------------------------------
void NetCli_MagicChild::R_SysEndShoot( int SerialID )
{
	//g_pDebugMessage->DebugOutput(0, "[R_SysEndShoot] SerialID%d", SerialID);
#ifdef DEBUG_MAGIC
	g_pError->AddMessage(0, 0, "[R_SysEndShoot] SerialID%d", SerialID);
#endif

	CNetMagicPackageData* magicData = g_pACTActionEventsManager->GetMagicData(SerialID);
	if ( magicData == NULL )
		return;
	magicData->EndShoot();
	magicData->Release();
}

//-----------------------------------------------------------------------------
void NetCli_MagicChild::R_EndShoot( int SerialID )
{
	//g_pDebugMessage->DebugOutput(0, "[R_EndShoot] SerialID%d", SerialID);
#ifdef DEBUG_MAGIC
	g_pError->AddMessage(0, 0, "[R_EndShoot] SerialID%d", SerialID);
#endif

	CNetMagicPackageData* magicData = g_pACTActionEventsManager->GetMagicData(SerialID);
	if ( magicData == NULL )
		return;
	magicData->EndShoot();
	magicData->DelRefCount();
}
//-----------------------------------------------------------------------------
void NetCli_MagicChild::R_BeginShoot( int SerialID , int TargetCount , int TargetGUIDList[_DEF_MAX_MAGIC_TARGET_COUNT_] )
{
#ifdef DEBUG_MAGIC
	{
		char log[1024];
		char temp[512];

		memset(temp, 0, sizeof(temp));
		sprintf_s(log, 12024, "[R_BeginShoot] SerialID:%d TargetCount:%d (", SerialID, TargetCount);
		for ( int i = 0; i < TargetCount; i++ ) {
			sprintf_s(temp, 512, "%d,", TargetGUIDList[i]);
			strcat(log, temp);
		}
		strcat(log, ")");

		g_pError->AddMessage(0, 0, log);
	}
#endif	

	CNetMagicPackageData* magicData = g_pACTActionEventsManager->GetMagicData(SerialID);	
	if ( magicData == NULL )
		return;	

	// 增加計數器
	magicData->AddRefCount();
	
	GameObjDbStructEx* itemDB = CNetGlobal::GetObj(magicData->m_magic);
	GameObjDbStruct* imageDB;
	if ( itemDB && (imageDB = g_ObjectData->GetObj(itemDB->ImageID)) != NULL )
	{
		CRuVector3 targetPosition(0.0f, 0.0f, 0.0f);

		// 圖形表演方式
		switch ( imageDB->Image.MagicShowType )
		{
		case EM_GameObjectMagicShowType_Single:
		case EM_GameObjectMagicShowType_Parabola:
			if ( TargetCount > 1 ) {
				TargetGUIDList[0] = magicData->m_target;
				TargetCount = 1;
			}
			break;

		case EM_GameObjectMagicShowType_Multi:			
			break;

		case EM_GameObjectMagicShowType_PolyLine:
			break;
		}

		// 對向
		switch ( itemDB->MagicCol.TargetType )
		{
		case EM_Releation_Locatoin:
			TargetCount = 0;
			targetPosition.Set(magicData->m_x, magicData->m_y, magicData->m_z);
			break;

		default:
			// 記錄目標清單最大不超出100個單位
			if ( TargetCount > _DEF_MAX_MAGIC_TARGET_COUNT_ )
				TargetCount = _DEF_MAX_MAGIC_TARGET_COUNT_;
			// 如果沒有目標使用原始目標
			else if ( TargetCount == 0 )
			{
				TargetCount = 1;
				TargetGUIDList[0] = magicData->m_target;
			}
			break;
		}		

		// 產生單體法術資料
		int shootIndex = magicData->CreateShoot(TargetCount, TargetGUIDList, targetPosition);

		GameObjectImageStruct* gois = &(imageDB->Image);
		CRoleSprite* owner = g_pGameMain->FindSpriteByID(magicData->m_owner);
		CRoleSprite* target = g_pGameMain->FindSpriteByID(magicData->m_target);

		// 開啟自動攻擊
		if ( owner == g_pGameMain->GetPlayer() )
		{
			if ( itemDB && itemDB->MagicCol.IsAutoAttack )
				g_pGameMain->AttackTarget(target);
		}

		// 顯示法術名稱
		if ( itemDB->MagicCol.IsShowMagicName ) {
			SCTInfo sctInfo;
			sctInfo.type = eSCT_Show_Text;
			sctInfo.source = magicData->m_owner;
			sctInfo.target = magicData->m_target;
			sctInfo.text = itemDB->GetName();
			g_pScrollCombatTextFrame->PushSCTInfo(sctInfo);
		}

		if ( owner )
		{
			ACTActionEventsInfo* actionEventInfo = g_pACTActionEventsManager->GetACTActionEvent();
			CRuController_EventTrigger* eventTrigger = NULL;
			RuEntityEvent eventType = ruENTITYEVENT_CAST_TRIGGER_01;
			
			if ( actionEventInfo != NULL ) {

				// 設定出手動作
				owner->CastSpell(actionEventInfo->m_actionEvent);

				eventTrigger = actionEventInfo->m_actionEvent->GetEventController(eventType);
				if ( eventTrigger == NULL )
				{
					eventType = ruENTITYEVENT_HIT;
					eventTrigger = actionEventInfo->m_actionEvent->GetEventController(eventType);
				}
			}

			// 檢查事件是否存在,大部份會發生事件點不存在的原因是使用有LOOP的動作或者事件點不存在
			if ( actionEventInfo != NULL && eventTrigger != NULL )
			{
				// 設定觸發事件
				actionEventInfo->m_actionEvent->RegisterEventTriggerCallback(
					eventType,
					boost::bind(&CACTActionEventsManager::EventCallback, g_pACTActionEventsManager, _1, _2),
					(PTRVALUE)actionEventInfo);

				// 事件觸發時使用相關法術資訊
				actionEventInfo->m_type = ACT_ACTIONEVENTS_TYPE_MAGIC;
				actionEventInfo->m_magic.m_serial = SerialID;
				actionEventInfo->m_magic.m_shootNum = shootIndex;
			}
			// 事件點不存在
			else
			{
				// 動作已結束
				magicData->SetShootActionDone();
				magicData->SetActionDone();

				// 將事件觸發結構回收
				actionEventInfo->Recyele();

				// 出手特效
				g_pACTActionEventsManager->CreateShootEffect(SerialID, shootIndex);

				// 產生特效,沒有動作觸發時間
				if ( !g_pACTActionEventsManager->CreateFlyEffect(SerialID, shootIndex) ) {
					g_pACTActionEventsManager->CreateExplodeEffect(SerialID);
				}
			}
		}
	}	
}

//-----------------------------------------------------------------------------
void NetCli_MagicChild::R_MagicEnd( int MagicID , int Coldown_Normal , int Coldown_All )
{
	//g_pDebugMessage->DebugOutput( 0, "[R_MagicEnd] MagicID %d Coldown_Normal %d Coldown_All %d ", MagicID, Coldown_Normal, Coldown_All );
#ifdef DEBUG_MAGIC
	g_pError->AddMessage(0, 0, "[R_MagicEnd] MagicID %d Coldown_Normal %d Coldown_All %d ", MagicID, Coldown_Normal, Coldown_All );
#endif

	GameObjDbStruct* itemDB = CNetGlobal::GetObj(MagicID);
	if ( itemDB )
	{		
		if ( RoleData()->SetMagicColdown(itemDB->MagicCol.Coldown.Class, itemDB->MagicCol.Coldown.Type, Coldown_Normal * 10 , Coldown_All * 10 ) )
		{
			g_pActionFrame->UpdateCooldown(itemDB->MagicCol.Coldown.Class, itemDB->MagicCol.Coldown.Type);
			g_pBagFrame->UpdateCooldown(itemDB->MagicCol.Coldown.Class, itemDB->MagicCol.Coldown.Type);
			g_pCharacterFrame->UpdateCooldown(itemDB->MagicCol.Coldown.Class, itemDB->MagicCol.Coldown.Type);
			g_pUi_SkillBook->UpdateCooldown(itemDB->MagicCol.Coldown.Class, itemDB->MagicCol.Coldown.Type);
		}
	}
}

//-----------------------------------------------------------------------------
void NetCli_MagicChild::R_BeginSpellOK( int MagicID, int SerialID, int SpellTime , int ShootAngle )
{
	//g_pDebugMessage->DebugOutput( 0, "[R_BeginSpellOK] MagicID %d SpellTime %d", MagicID , SpellTime );
#ifdef DEBUG_MAGIC
	g_pError->AddMessage(0, 0, "[R_BeginSpellOK] MagicID %d SpellTime %d", MagicID , SpellTime );
#endif
}

//-----------------------------------------------------------------------------
void NetCli_MagicChild::R_BeginSpellFailed( int MagicID , int SerialID )
{
#ifdef DEBUG_MAGIC
	//g_pDebugMessage->DebugOutput( 0, "[R_BeginSpellFailed] MagicID %d ", MagicID );	
	g_pError->AddMessage(0, 0, "[R_BeginSpellFailed] MagicID %d ", MagicID );
#endif

	g_pGameMain->SetCastSpellState(CASTSPELL_NONE, true);	

	CRoleSprite* player = g_pGameMain->GetPlayer();	
	if ( player )
	{
		player->EndSpell();
	}
}

//-----------------------------------------------------------------------------
void NetCli_MagicChild::R_MagicAttack( int MagicBaseID , int Serial , MagicAttackTypeENUM	AtkType , vector< MagicAtkEffectInfoStruct >& List )
{
#ifdef DEBUG_MAGIC
	//g_pDebugMessage->DebugOutput(0, "[R_MagicAttack] MagicBaseID%d Serial%d", MagicBaseID, Serial);
	g_pError->AddMessage(0, 0, "[R_MagicAttack] MagicBaseID%d Serial%d", MagicBaseID, Serial);
#endif

	CNetMagicPackageData* magicData = g_pACTActionEventsManager->GetMagicData(Serial);
	if ( magicData == NULL )
		return;

	// 法術集合資料
	GameObjDbStructEx* objDB = g_ObjectData->GetObj(magicData->m_magic);
	if ( objDB == NULL ) {
		g_pDebugMessage->DebugOutput(0, "[R_MagicAttack magic's id error!!] MagicBaseID%d", MagicBaseID);
		return;
	}

	/*
	// 設定特效資料
	SpellShootPackage* shoot = magicData->GetLastShootPackage();
	if ( shoot )
	{
		// 堆入法術效果
		for ( vector<MagicAtkEffectInfoStruct>::iterator iter = List.begin(); iter != List.end(); iter++ )
		{
			SingleMagicPackage singleMagic;
			singleMagic.target				= iter->GItemID;
			singleMagic.attack.magicBase	= MagicBaseID;
			singleMagic.attack.type			= iter->Result;
			singleMagic.attack.value		= (int)iter->Value;
			shoot->m_list.push_back(singleMagic);
		}
	}
	*/

	GameObjDbStruct* itemDB = g_ObjectData->GetObj(MagicBaseID);
	if ( itemDB == NULL )
	{
		g_pDebugMessage->DebugOutput(0, "[R_MagicAttack magic's id error!!] MagicBaseID%d", MagicBaseID);
		return;
	}

#ifdef DEBUG_MAGIC
	g_pError->AddMessage(0, 0, "R_MagicAttack MagicBaseID %d, Serial %d size%d", MagicBaseID, Serial, List.size());
#endif

	for ( vector<MagicAtkEffectInfoStruct>::iterator iter = List.begin(); iter != List.end(); iter++ )
	{
#ifdef DEBUG_MAGIC
		g_pError->AddMessage(0, 0, "R_MagicAttack id = %d Result:%d", iter->GItemID, iter->Result);
#endif

		if ( iter->Result == EM_MagicAtkResult_Cancel )
			continue;		

		CRoleSprite* owner	= g_pGameMain->FindSpriteByID(magicData->m_owner);
		CRoleSprite* target = g_pGameMain->FindSpriteByID(iter->GItemID);
		CRoleSprite* player = g_pGameMain->GetPlayer();
		CRoleSprite* pet	= (player) ? player->GetPetSprite() : NULL;

		if ( target == NULL || target->IsScriptHide() )
			continue;

		// 如果攻擊已經死亡動作撥出		

		// 堆入法術效果
		magicData->AddAttackData(Serial, iter->GItemID, iter->Result, MagicBaseID, AtkType, (int)iter->Value);		

		switch ( iter->Result )
		{
		case EM_MagicAtkResult_Miss:
		case EM_MagicAtkResult_Normal:
			break;
		case EM_MagicAtkResult_Critial:
			if ( AtkType == EM_MagicAttackType_HP && iter->Value < 0.0f )
			{
				RoleData()->TempData.BackInfo.LastCriticalTime = RoleDataEx::G_SysTime;
			}
			break;

		case EM_MagicAtkResult_NoEffect:
		case EM_MagicAtkResult_Absorb:
		case EM_MagicAtkResult_NoEffect_Dead:
		case EM_MagicAtkResult_Cancel:
		case EM_MagicAtkResult_SpecialAction:
			break;

		case EM_MagicAtkResult_Parry:
		case EM_MagicAtkResult_ShieldBlock:
			break;
		}

		// 被攻擊者,解除(坐下,趟下,跪下)
		if ( iter->Result != EM_MagicAtkResult_Miss )
		{
			if ( target )
			{
				switch (target->GetActorState())
				{
				case ruFUSION_ACTORSTATE_SIT_BEGIN:
				case ruFUSION_ACTORSTATE_SIT_LOOP:	
					target->SetActorState(ruFUSION_ACTORSTATE_SIT_END, 1);
					break;

				case ruFUSION_ACTORSTATE_SLEEP_BEGIN:
				case ruFUSION_ACTORSTATE_SLEEP_LOOP:	
					target->SetActorState(ruFUSION_ACTORSTATE_SLEEP_END, 1);
					break;

				case ruFUSION_ACTORSTATE_CROUCH_BEGIN:
				case ruFUSION_ACTORSTATE_CROUCH_LOOP:
					target->SetActorState(ruFUSION_ACTORSTATE_CROUCH_END, 1);
					break;
				}
			}
		}

		int value = (int)iter->Value;
		int absorbPoint = (int)(iter->OrgValue - iter->Value);
		if ( owner &&
			 target &&
			 itemDB->MagicBase.Setting.DisableShowMagicInfo == false &&
			 objDB->MagicCol.Flag.IgnoreFightingLog == false &&
			 (owner == player || target == player || owner == pet || target == pet) )
		{
			char temp[1024];
			bool self = ( target == owner ) ? true : false;
			const char* name_s = owner->GetName();
			const char* name_t = target->GetName();

			/*
			bool self = false;
			const char* name_s = "";
			const char* name_t = "";

			if ( owner == player ) {
				name_s = g_ObjectData->GetString("SYS_YOU");
			} else if ( owner ) {
				name_s = owner->GetName();
			}

			if ( target == player ) {
				if ( owner == player ) {
					self = true;
					name_t = g_ObjectData->GetString("SYS_SELF");
				} else {
					name_t = g_ObjectData->GetString("SYS_YOU");
				}
			} else if (target) {
				name_t = target->GetName();
			}
			*/

			// 戰鬥訊息
			switch(iter->Result)
			{
			case EM_MagicAtkResult_Miss:					// 未擊中
				sprintf_s(temp, 1024, "|cFFF4F021%s|r", g_ObjectData->GetString("SYS_COMBAT_MAGIC_MISS"));
				CombatLog(temp, name_s, name_t, objDB->GetName(), AtkType, 0, 0);
				break;
			case EM_MagicAtkResult_Normal:					// 一般傷害
				if ( itemDB->MagicBase.AtkCalType == EM_MagicAttackCalBase_SpecialAction )
				{
					sprintf_s(temp, 1024, "|cFFF40F11%s|r", g_ObjectData->GetString("SYS_COMBAT_MAGIC_ASSIST_HARM"));
					CombatLog(temp, name_s, name_t, objDB->GetName(), AtkType, 0, 0);
				}
				else
				{
					if ( self )
					{
						if ( value > 0 ) {
							sprintf_s(temp, 1024, "|cFF05FF0F%s|r", g_ObjectData->GetString("SYS_COMBAT_MAGIC_HEAL_SELF"));
							CombatLog(g_ObjectData->GetString("SYS_COMBAT_MAGIC_HEAL_SELF"), name_s, name_t, objDB->GetName(), AtkType, value, 0);
						}
						else {
							sprintf_s(temp, 1024, "|cFFF40F11%s|r", g_ObjectData->GetString("SYS_COMBAT_MAGIC_NORMAL_SELF"));
							CombatLog(temp, name_s, name_t, objDB->GetName(), AtkType, -value, 0);
						}
					}
					else
					{
						if ( value > 0 ) {
							sprintf_s(temp, 1024, "|cFF05FF0F%s|r", g_ObjectData->GetString("SYS_COMBAT_MAGIC_HEAL"));
							CombatLog(g_ObjectData->GetString("SYS_COMBAT_MAGIC_HEAL"), name_s, name_t, objDB->GetName(), AtkType, value, 0);
						}
						else {
							sprintf_s(temp, 1024, "|cFFF4F021%s|r", g_ObjectData->GetString("SYS_COMBAT_MAGIC_NORMAL"));
							CombatLog(temp, name_s, name_t, objDB->GetName(), AtkType, -value, 0);
						}
					}
				}
				break;

			case EM_MagicAtkResult_Critial:					// 爆擊
				if ( self )
				{
					if ( value > 0 ) {
						sprintf_s(temp, 1024, "|cFF05FF0F%s|r", g_ObjectData->GetString("SYS_COMBAT_MAGIC_HEAL_CRITIAL_SELF"));
						CombatLog(temp, name_s, name_t, objDB->GetName(), AtkType, value, 0);
					}
					else {
						sprintf_s(temp, 1024, "|cFFF40F11%s|r", g_ObjectData->GetString("SYS_COMBAT_MAGIC_CRITIAL_SELF"));
						CombatLog(temp, name_s, name_t, objDB->GetName(), AtkType, -value, 0);
					}
				}
				else
				{
					if ( value > 0 ) {
						sprintf_s(temp, 1024, "|cFF05FF0F%s|r", g_ObjectData->GetString("SYS_COMBAT_MAGIC_HEAL_CRITIAL"));
						CombatLog(g_ObjectData->GetString("SYS_COMBAT_MAGIC_HEAL_CRITIAL"), name_s, name_t, objDB->GetName(), AtkType, value, 0);
					} else {
						sprintf_s(temp, 1024, "|cFFF4F021%s|r", g_ObjectData->GetString("SYS_COMBAT_MAGIC_CRITIAL"));
						CombatLog(temp, name_s, name_t, objDB->GetName(), AtkType, -value, 0);
					}
				}
				break;
			case EM_MagicAtkResult_NoEffect:				// 沒有效果
				if ( itemDB->MagicBase.Setting.IgnoreBuffMessage )
				{
					sprintf_s(temp, 1024, "|cFFF4F021%s|r", g_ObjectData->GetString("SYS_COMBAT_MAGIC_NOEFFECT"));
					CombatLog(temp, name_s, name_t, objDB->GetName(), AtkType, 0, 0);
				}
				break;

			case EM_MagicAtkResult_Absorb:					// 吸收
				if ( absorbPoint < 0 )
				{
					if ( self ) {
						sprintf_s(temp, 1024, "|cFFF4F021%s|r", g_ObjectData->GetString("SYS_COMBAT_MAGIC_ABSORB_SELF"));
						CombatLog(temp, name_s, name_t, objDB->GetName(), AtkType, 0, -absorbPoint);
					} else {
						sprintf_s(temp, 1024, "|cFFF4F021%s|r", g_ObjectData->GetString("SYS_COMBAT_MAGIC_ABSORB"));
						CombatLog(temp, name_s, name_t, objDB->GetName(), AtkType, 0, -absorbPoint);
					}
				}				

				if ( value < 0 )
				{
					if ( self ) {
						sprintf_s(temp, 1024, "|cFFF40F11%s|r", g_ObjectData->GetString("SYS_COMBAT_MAGIC_NORMAL_SELF"));
						CombatLog(temp, name_s, name_t, objDB->GetName(), AtkType, -value, 0);
					} else {
						sprintf_s(temp, 1024, "|cFFF4F021%s|r", g_ObjectData->GetString("SYS_COMBAT_MAGIC_NORMAL"));
						CombatLog(temp, name_s, name_t, objDB->GetName(), AtkType, -value, 0);
					}
				}
				break;

			case EM_MagicAtkResult_NoEffect_Dead:			// 沒有效果,目標已死亡
				break;

			case EM_MagicAtkResult_Parry:
				sprintf_s(temp, 1024, "|cFFF4F021%s|r", g_ObjectData->GetString("SYS_COMBAT_MAGIC_PARRY"));
				CombatLog(temp, name_s, name_t, objDB->GetName(), AtkType, -value, 0);
				break;

			case EM_MagicAtkResult_ShieldBlock:
				sprintf_s(temp, 1024, "|cFFF4F021%s|r", g_ObjectData->GetString("SYS_COMBAT_MAGIC_SHIELD_BLOCK"));
				CombatLog(temp, name_s, name_t, objDB->GetName(), AtkType, 0, 0);
				break;

			case EM_MagicAtkResult_Dodge:
				if ( itemDB->MagicBase.EffectType == EM_Magic_Magic ) {
					sprintf_s(temp, 1024, "|cFFF4F021%s|r", g_ObjectData->GetString("SYS_COMBAT_MAGIC_RESIST"));
					CombatLog(g_ObjectData->GetString("SYS_COMBAT_MAGIC_RESIST"), name_s, name_t, objDB->GetName(), AtkType, 0, 0);
				}
				else if ( itemDB->MagicBase.EffectType == EM_Magic_Phy ) {
					sprintf_s(temp, 1024, "|cFFF4F021%s|r", g_ObjectData->GetString("SYS_COMBAT_MAGIC_DODGE"));
					CombatLog(g_ObjectData->GetString("SYS_COMBAT_MAGIC_DODGE"), name_s, name_t, objDB->GetName(), AtkType, 0, 0);
				}
				break;
			}
		}
		
		// 戰鬥訊息
		if( owner && target && player && itemDB->MagicBase.Setting.DisableShowMagicInfo == false )
		{
			const char* szSourceName = owner->GetName();
			const char* szTargetName = target->GetName();

			if( AtkType == EM_MagicAttackType_HP && 
				( owner == player || NetCli_PartyChild::InGroup( szSourceName ) || target == player || NetCli_PartyChild::InGroup( szTargetName ) || owner == pet || target == pet ) )
			{
				int value = (int)iter->Value;

				switch( iter->Result )
				{
				case EM_MagicAtkResult_Miss:					// 未擊中
					g_pCombatMeter->AddDamage( szSourceName, szTargetName, objDB->GetName(), "MISS", -value );
					break;

				case EM_MagicAtkResult_Normal:					// 一般傷害
					if ( value > 0 ) 
					{
						g_pCombatMeter->AddHeal( szSourceName, szTargetName, objDB->GetName(), "NORMAL", value );
					}
					else
					{
						g_pCombatMeter->AddDamage( szSourceName, szTargetName, objDB->GetName(), "NORMAL", -value );
					}

					break;

				case EM_MagicAtkResult_Critial:					// 爆擊
					if ( value > 0 ) 
					{
						g_pCombatMeter->AddHeal( szSourceName, szTargetName, objDB->GetName(), "CRITIAL", value );
					}
					else
					{
						g_pCombatMeter->AddDamage( szSourceName, szTargetName, objDB->GetName(), "CRITIAL", -value );
					}
					break;
				case EM_MagicAtkResult_NoEffect:				// 沒有效果
					g_pCombatMeter->AddDamage( szSourceName, szTargetName, objDB->GetName(), "MISS", -value );
					break;
				case EM_MagicAtkResult_Absorb:					// 吸收
					if ( absorbPoint < 0 )
						g_pCombatMeter->AddDamage( szSourceName, szTargetName, objDB->GetName(), "ABSORB", -absorbPoint );
					if ( value < 0 )
						g_pCombatMeter->AddDamage( szSourceName, szTargetName, objDB->GetName(), "NORMAL", -value );
					break;
				case EM_MagicAtkResult_NoEffect_Dead:			// 沒有效果,目標已死亡
					break;

				case EM_MagicAtkResult_Dodge:
					if ( itemDB->MagicBase.EffectType == EM_Magic_Magic )
						g_pCombatMeter->AddDamage( szSourceName, szTargetName, objDB->GetName(), "RESIST", -value );
					else if ( itemDB->MagicBase.EffectType == EM_Magic_Phy )
						g_pCombatMeter->AddDamage( szSourceName, szTargetName, objDB->GetName(), "DODGE", -value );
					break;
				}
			}
   		}
	}
}

//-----------------------------------------------------------------------------
void NetCli_MagicChild::R_MagicAssist( int MagicBaseID , int Serial , vector< MagicAssistEffectInfoStruct >& List )
{
#ifdef DEBUG_MAGIC
	//g_pDebugMessage->DebugOutput(0, "[R_MagicAssist] MagicBaseID%d Serial%d", MagicBaseID, Serial);
	g_pError->AddMessage(0, 0, "[R_MagicAssist] MagicBaseID%d Serial%d", MagicBaseID, Serial);
#endif

	CNetMagicPackageData* magicData = g_pACTActionEventsManager->GetMagicData(Serial);
	if ( magicData == NULL )
		return;

	// 法術集合資料
	GameObjDbStructEx* objDB = g_ObjectData->GetObj(magicData->m_magic);
	GameObjDbStructEx* itemDB = g_ObjectData->GetObj(MagicBaseID);

	if ( objDB == NULL || itemDB == NULL ) {
		g_pDebugMessage->DebugOutput(0, "[R_MagicAssist magic's id error!!] MagicBaseID%d Serial%d", MagicBaseID, Serial);
		return;
	}

	/*
	// 設定特效資料
	SpellShootPackage* shoot = magicData->GetLastShootPackage();
	if ( shoot )
	{
		shoot->m_type		= SPELL_SHOOT_PACKAGE_ASSIST;
		shoot->m_magicBase	= MagicBaseID;
		shoot->m_magicLv	= MagicLv;
		shoot->m_valueType	= 0;

		// 堆入法術效果
		for ( vector<MagicAssistEffectInfoStruct>::iterator iter = List.begin(); iter != List.end(); iter++ )
		{
			SingleMagicPackage singleMagic;
			singleMagic.type	= iter->Result;
			singleMagic.target	= iter->GItemID;
			singleMagic.time	= iter->EffectTime;
			shoot->m_list.push_back(singleMagic);
		}
	}
	*/

	for ( vector<MagicAssistEffectInfoStruct>::iterator iter = List.begin(); iter != List.end(); iter++ )
	{	
		if ( iter->Result == EM_MagicAtkResult_Cancel )
			continue;

		// 戰鬥訊息顯示
		CRoleSprite* owner	= g_pGameMain->FindSpriteByID(magicData->m_owner);
		CRoleSprite* target = g_pGameMain->FindSpriteByID(iter->GItemID);
		CRoleSprite* player = g_pGameMain->GetPlayer();
		CRoleSprite* pet	= (player) ? player->GetPetSprite() : NULL;

		if ( target == NULL || target->IsScriptHide() )
			continue;

		/*
		if ( iter->Result == EM_MagicAtkResult_NoEffect && itemDB->MagicBase.Setting.NoEffect )
			continue;
		*/

		// 堆入法術效果
		magicData->AddAssistData(Serial, iter->GItemID, iter->Result, MagicBaseID, iter->MagicLv, (float)iter->EffectTime);		

		// BUFF 的產生不做暫存處理,避免某些有效果(隱身,不能移動..等等)的法術太晚產生
		switch ( iter->Result )
		{
		case EM_MagicAtkResult_Normal:
		case EM_MagicAtkResult_Critial:
			if ( iter->EffectTime != 0 )
			{
				R_AddBuffInfo(iter->GItemID, magicData->m_owner, MagicBaseID, iter->MagicLv, iter->EffectTime);

				GameObjDbStruct* itemDB = CNetGlobal::GetObj( MagicBaseID );
				if ( target && itemDB->MagicBase.Effect.Ride )
				{
					CRoleSprite* mountSprite = target->GetMountSprite();
					if ( mountSprite )
					{
						mountSprite->SetActorState(ruFUSION_ACTORSTATE_EMOTE_JUMP_INPLACE);
					}
				}
			}
			break;
		}
		
		if ( itemDB->MagicBase.MagicFunc != EM_MAGICFUN_RunPlot && 
			 itemDB->MagicBase.Setting.DisableShowMagicInfo == false &&
			 objDB->MagicCol.Flag.IgnoreFightingLog == false &&
			 owner && target &&
			 owner->GetVocation() != EM_Vocation_GameMaster &&
			 (owner == player || target == player || owner == pet || target == pet) )
		{
			char temp[1024];
			bool self = ( target == owner ) ? true : false;
			const char* name_s = owner->GetName();
			const char* name_t = target->GetName();

			/*
			if ( owner == player ) {
				name_s = g_ObjectData->GetString("SYS_YOU");
			} else if ( owner ) {
				name_s = owner->GetName();
			}

			if ( target == player ) {
				if ( owner == player ) {
					self = true;
					name_t = g_ObjectData->GetString("SYS_SELF");
				} else {
					name_t = g_ObjectData->GetString("SYS_YOU");
				}
			} else if ( target ) {
				name_t = target->GetName();
			}
			*/
			
			/*
			if ( owner == player ) {
				name_s = g_ObjectData->GetString("SYS_YOU");
			}
			if ( target == player ) {
				name_t = g_ObjectData->GetString("SYS_YOU");
			}
			*/

			// 戰鬥訊息
			switch(iter->Result)
			{
			case EM_MagicAtkResult_Miss:					// 未擊中
				sprintf_s(temp, 1024, "|cFFF4F021%s|r", g_ObjectData->GetString("SYS_COMBAT_MAGIC_MISS"));
				CombatLog(g_ObjectData->GetString("SYS_COMBAT_MAGIC_MISS"), name_s, name_t, objDB->GetName(), EM_MagicAttackType_HP, 0, 0);
				break;

			case EM_MagicAtkResult_Normal:					// 一般傷害
			case EM_MagicAtkResult_Critial:					// 爆擊
				if ( self )
				{
					if ( itemDB->MagicBase.Setting.GoodMagic ) {
						sprintf_s(temp, 1024, "|cFF05FF0F%s|r", g_ObjectData->GetString("SYS_COMBAT_MAGIC_ASSIST_HELP_SELF"));
						CombatLog(temp, name_s, name_t, objDB->GetName(), EM_MagicAttackType_HP, 0, 0);
					} else {
						sprintf_s(temp, 1024, "|cFFF4F021%s|r", g_ObjectData->GetString("SYS_COMBAT_MAGIC_ASSIST_HARM_SELF"));
						CombatLog(temp, name_s, name_t, objDB->GetName(), EM_MagicAttackType_HP, 0, 0);
					}
				}
				else
				{
					if ( itemDB->MagicBase.Setting.GoodMagic ) {
						sprintf_s(temp, 1024, "|cFF05FF0F%s|r", g_ObjectData->GetString("SYS_COMBAT_MAGIC_ASSIST_HELP"));
						CombatLog(temp, name_s, name_t, objDB->GetName(), EM_MagicAttackType_HP, 0, 0);
					} else {
						sprintf_s(temp, 1024, "|cFFF4F021%s|r", g_ObjectData->GetString("SYS_COMBAT_MAGIC_ASSIST_HARM"));
						CombatLog(temp, name_s, name_t, objDB->GetName(), EM_MagicAttackType_HP, 0, 0);
					}
				}
				break;
			case EM_MagicAtkResult_NoEffect:				// 沒有效果
				if ( itemDB->MagicBase.Setting.IgnoreBuffMessage )
				{
					sprintf_s(temp, 1024, "|cFFF4F021%s|r", g_ObjectData->GetString("SYS_COMBAT_MAGIC_NOEFFECT"));
					CombatLog(temp, name_s, name_t, objDB->GetName(), EM_MagicAttackType_HP, 0, 0);
				}
				break;
			case EM_MagicAtkResult_Absorb:					// 吸收
				break;
			case EM_MagicAtkResult_NoEffect_Dead:			// 沒有效果,目標已死亡
				break;
			}
		}
	}
}

//-----------------------------------------------------------------------------
void NetCli_MagicChild::R_FixAssistMagicTime ( int MagicID , int EffectTime , int BuffOwnerID )
{
	// RoleData 更新時間
	GameObjDbStructEx* MagicDB = g_ObjectData->GetObj( MagicID );
	if( MagicDB->IsMagicBase() == false )
		return;

	CRoleSprite* player = g_pGameMain->GetPlayer();
	if ( player ) {
		player->AddBuff( 0, MagicID, (float)EffectTime, 0, "", BuffOwnerID , 0 );
	}	
	if ( player == g_pGameMain->GetPlayer() )
	{
		RoleDataEx* Owner = CNetGlobal::RoleData();
		if( Owner )
		{
			for( int i = 0 ; i < Owner->BaseData.Buff.Size() ; i++ )
			{
				if( MagicDB->MagicBase.Setting.SelfBuff && BuffOwnerID != Owner->BaseData.Buff[i].OwnerID )
					continue;

				if( Owner->BaseData.Buff[i].BuffID == MagicID )
				{
					Owner->BaseData.Buff[i].Time = EffectTime;
				}
			}
		}
	}

}

//-----------------------------------------------------------------------------
void NetCli_MagicChild::R_CancelSpellMagic   ( int OwnerID , int SerialID )
{
	//g_pDebugMessage->DebugOutput( 0, "[R_CancelSpellMagic] (取消失法 失法中斷)OwnerID %d ", OwnerID );
#ifdef DEBUG_MAGIC
	g_pError->AddMessage(0, 0, "[R_CancelSpellMagic] (取消失法 失法中斷)OwnerID %d SerialID %d", OwnerID, SerialID );
#endif

	// 中斷施法
	CRoleSprite* sprite = g_pGameMain->FindSpriteByID(OwnerID);	

	if ( sprite == g_pGameMain->GetPlayer() )
	{
		g_pCastingBarFrame->CastingFailed(eCastingBar_SpellMagic);
	}	
	R_EndSpell(SerialID);

	/*
	if ( sprite )
	{
		// 取消施法
		sprite->SetCastingTime(0.0f);
	}
	*/
}

//-----------------------------------------------------------------------------
void NetCli_MagicChild::R_SetMagicPointResult( int MagicID , int MagicPos , SetMagicPointResultCodeENUM Result )
{
	RoleDataEx* Owner = CNetGlobal::RoleData();
	//g_pDebugMessage->DebugOutput( 0, "R_SetMagicPointResult OwnerID" );
	//g_pInterface->SendWorldEvent( UiEvent_SkillUpdate );
	CUI_SkillBook::SkillUpdate();
	Owner->TempData.UpdateInfo.ReSetTalbe = true;
}

//-----------------------------------------------------------------------------
void NetCli_MagicChild::R_ModifyBuffInfo( int OwnerID, int SpellerID, int MagicID, int MagicLv, int EffectTime )
{
	RoleDataEx* Owner = CNetGlobal::RoleData();
	
	if( OwnerID == Owner->GUID() )
	{
		for( int i = 0 ; i < Owner->BaseData.Buff.Size() ; i++ )
		{
			BuffBaseStruct&  Buff = Owner->BaseData.Buff[ i ];

			if( Buff.BuffID == MagicID )
			{
				Buff.Power = MagicLv;
				Buff.Time  = EffectTime;
				break;
			}
		}	
	}

	CRoleSprite* sprite = g_pGameMain->FindSpriteByID(OwnerID);
	if ( sprite )
	{
		CRoleSprite* spellerSprite = g_pGameMain->FindSpriteByID(SpellerID);
		const char* spellerName = (spellerSprite) ? spellerSprite->GetName() : "";
		sprite->AddBuff(SpellerID, MagicID, (float)EffectTime, MagicLv, spellerName, SpellerID, 1);
	}
}
void NetCli_MagicChild::R_AddBuffInfo( int OwnerID, int SpellerID, int MagicID, int MagicLv, int EffectTime )
{	
#ifdef DEBUG_MAGIC
	//g_pDebugMessage->DebugOutput( 0, "[R_AddBuffInfo] OwnerID=%d SpellerID=%d MagicID=%d EffectTime=%d", OwnerID, SpellerID, MagicID, EffectTime );
	g_pError->AddMessage(0, 0, "[R_AddBuffInfo] OwnerID=%d SpellerID=%d MagicID=%d EffectTime=%d", OwnerID, SpellerID, MagicID, EffectTime);
#endif

	/*
	GameObjDbStructEx* itemDB = g_ObjectData->GetObj(MagicID);	
	if ( (itemDB && !itemDB->MagicBase.Setting.ShowBuffTime) || EffectTime > 31536000 )
		EffectTime = -1;	
	*/

	if ( EffectTime > 31536000 )
		EffectTime = -1;

	CRoleSprite* speller = g_pGameMain->FindSpriteByID( SpellerID );
	CRoleSprite* sprite = g_pGameMain->FindSpriteByID( OwnerID );

	// 檢查是否本機端玩家受到影響
	if ( OwnerID == g_pGameMain->GetGItemID() ) {
		bool IsPK = false;
		if ( speller && speller->GetObjectType() == eSpriteObject_Player &&	OwnerID != SpellerID ) {
			IsPK = true;
		}

		RoleDataEx* Owner = CNetGlobal::RoleData();
		//Owner->AssistMagic( Owner, MagicID, MagicLv, IsPK , EffectTime );
		Owner->CliAddBuf( SpellerID , MagicID, MagicLv, IsPK , EffectTime );

		/*
		// PK保護
		{
			GameObjDbStruct* itemDB = g_ObjectData->GetObj(MagicBaseID);
			if ( itemDB && itemDB->MagicBase.Effect.PKFlag_Disabled && magicData->m_owner == g_pGameMain->GetGItemID() )
			{
				g_pGameMain->SendSystemMsg(g_ObjectData->GetString("SYS_PROTECT_BUFF_SELF"));
			}
		}
		*/
	}
	
	if( sprite )
	{
		CRoleSprite* spellerSprite = g_pGameMain->FindSpriteByID(SpellerID);
		const char* spellerName = (spellerSprite) ? spellerSprite->GetName() : "";
		if ( sprite->GetObjectType() != eSpriteObject_Player || OwnerID == SpellerID || (speller && speller->GetObjectType() != eSpriteObject_Player) )
		{			
			sprite->AddBuff(SpellerID, MagicID, (float)EffectTime, MagicLv, spellerName, SpellerID , 1 );
		}
		else
		{
			sprite->AddBuff(SpellerID, MagicID, (float)EffectTime, MagicLv , spellerName, SpellerID , RoleDataEx::GlobalSetting.PK_DotRate );
		}
		
	}

	/*
	for ( int i = 0; i < CNetGlobal::RoleData()->BaseData.Buff.Size(); i++ )
	{
		g_pError->AddMessage(0, 0, "[RoleData Buff] OwnerID=%d BuffID=%d Time=%d",
			CNetGlobal::RoleData()->BaseData.Buff[i].OwnerID,
			CNetGlobal::RoleData()->BaseData.Buff[i].BuffID,
			CNetGlobal::RoleData()->BaseData.Buff[i].Time);
	}
	*/
}

//-----------------------------------------------------------------------------
void NetCli_MagicChild::R_DelBuffInfo( int OwnerID , int MagicID , int BuffOwnerID )
{
#ifdef DEBUG_MAGIC
	// g_pDebugMessage->DebugOutput( 0, "[R_DelBuffInfo] OwnerID=%d MagicID=%d", OwnerID, MagicID );
	g_pError->AddMessage(0, 0, "[R_DelBuffInfo] OwnerID=%d MagicID=%d BuffOwnerID=%d", OwnerID, MagicID, BuffOwnerID );
#endif

	// 檢查是否本機端玩家受到影響
	if ( OwnerID == g_pGameMain->GetGItemID() ) {
		RoleDataEx* Owner = CNetGlobal::RoleData();
		if ( MagicID == -1 )
			Owner->BaseData.Buff.Clear();				// 清除所有BUFF
		else
			Owner->ClearBuff(MagicID,BuffOwnerID);					// 清除單一BUFF
		Owner->TempData.UpdateInfo.Recalculate_Buff = true;
	}	

	CRoleSprite* sprite = g_pGameMain->FindSpriteByID( OwnerID );
	if ( sprite )
	{
		// 特別清除所有BUFF
		if ( MagicID == -1 )
			sprite->ClearBuff();
		else
			sprite->DeleteBuff(MagicID, BuffOwnerID);
	}

	/*
	for ( int i = 0; i < CNetGlobal::RoleData()->BaseData.Buff.Size(); i++ )
	{
		g_pError->AddMessage(0, 0, "[RoleData Buff] OwnerID=%d BuffID=%d Time=%d",
			CNetGlobal::RoleData()->BaseData.Buff[i].OwnerID,
			CNetGlobal::RoleData()->BaseData.Buff[i].BuffID,
			CNetGlobal::RoleData()->BaseData.Buff[i].Time);
	}
	*/
}
//-----------------------------------------------------------------------------
void NetCli_MagicChild::R_ResetColdown		  ( ResetColdownTypeENUM Type )
{ 
	RoleDataEx* Owner = CNetGlobal::RoleData();
	switch( Type )
	{
	case EM_ResetColdownType_All:
		Owner->BaseData.Coldown.Init();
		break;
	case EM_ResetColdownType_NewAll:
		for( int i = 0 ; i < DEF_MAX_COLDOWN_COUNT_JOB_ ; i++ )
		{
			if( g_ObjectData->GetNotColdownJob( i ) )
				continue;
			Owner->BaseData.Coldown.Job[i] = 0;
		}

		for( int i = 0 ; i < DEF_MAX_COLDOWN_COUNT_EQ_ ; i++ )
		{
			Owner->BaseData.Coldown.Eq[i] = 0;
		}

		for( int i = 0 ; i < DEF_MAX_COLDOWN_COUNT_ITEM_ ; i++ )
		{
			Owner->BaseData.Coldown.Item[i] = 0;
		}
		break;
	case EM_ResetColdownType_Job_Normal:
		for( int i = 5 ; i < DEF_MAX_COLDOWN_COUNT_JOB_ ; i++ )
		{
			if( g_ObjectData->GetNotColdownJob( i ) )
				continue;
			Owner->BaseData.Coldown.Job[i] = 0;
		}
		break;
	case EM_ResetColdownType_Job:
		for( int i = 0 ; i < DEF_MAX_COLDOWN_COUNT_JOB_ ; i++ )
		{
			Owner->BaseData.Coldown.Job[i] = 0;
		}
		break;
	case EM_ResetColdownType_EQ:
		for( int i = 0 ; i < DEF_MAX_COLDOWN_COUNT_EQ_ ; i++ )
		{
			Owner->BaseData.Coldown.Eq[i] = 0;
		}
		break;
	case EM_ResetColdownType_Item:
		for( int i = 0 ; i < DEF_MAX_COLDOWN_COUNT_ITEM_ ; i++ )
		{
			Owner->BaseData.Coldown.Item[i] = 0;
		}
		break;
	}
	//Owner->BaseData.Coldown.Init();

	g_pActionFrame->UpdateCooldown( 0 ,0 );
	g_pBagFrame->UpdateCooldown( 0 ,0 );
	g_pCharacterFrame->UpdateCooldown( 0 ,0 );

};
//-----------------------------------------------------------------------------
void NetCli_MagicChild::R_MagicShieldInfo( int AttackGUID , int UnAttackGUID , int AttackMagicID , int ShieldMagicID , int AbsobDamage )
{
	g_pDebugMessage->DebugOutput( 0, "[R_MagicShieldInfo] AttackGUID=%d UnAttackGUID=%d AbsobDamage=%d", AttackGUID, UnAttackGUID, AbsobDamage );

}
//-----------------------------------------------------------------------------
void NetCli_MagicChild::R_CreatePet			( int PetGUID , int PetWorldGUID , int PetItemID , int MagicBaseID , SummonPetTypeENUM	PetType )
{
	g_pDebugMessage->DebugOutput( 0, "寵物產生");
	//---------------------------------------------------------------------------
	RoleDataEx* Owner = CNetGlobal::RoleData();

	PetStruct& PetInfo = Owner->TempData.SummonPet.Info[ PetType ];

	PetInfo.Init();
	PetInfo.PetID = PetGUID;
	PetInfo.MagicID = MagicBaseID;

	
	/*GameObjDbStructEx* OrgDB = GetObj( MagicBaseID );
	if( OrgDB != NULL )
		Owner->SetPetSkill( OrgDB );
		*/
	Owner->SetPetSkill(  );
	//---------------------------------------------------------------------------	
	g_pGameMain->CreatePet(PetType, PetGUID, PetItemID, MagicBaseID);
}
void NetCli_MagicChild::R_DeletePet			(  int PetGUID , SummonPetTypeENUM PetType )
{
	g_pDebugMessage->DebugOutput( 0, "寵物刪除");
	//---------------------------------------------------------------------------
	RoleDataEx* Owner = CNetGlobal::RoleData();
	PetStruct& PetInfo = Owner->TempData.SummonPet.Info[ PetType ];
	PetInfo.Init();
	//---------------------------------------------------------------------------

	g_pGameMain->DeletePet(PetType, PetGUID);
}
void NetCli_MagicChild::R_PetMagicSwitchResult	( int PetGUID , SummonPetTypeENUM PetType , int ActiveMagicID , bool Active )
{
	g_pDebugMessage->DebugOutput( 0, "設定寵物致能的法術");
	//---------------------------------------------------------------------------
	RoleDataEx* Owner = CNetGlobal::RoleData();
	PetStruct& PetInfo = Owner->TempData.SummonPet.Info[ PetType ];

	//檢查是否有此法術
	for( int i = 0 ; i < PetInfo.Skill.Size() ; i++ )
	{
		PetSkillStruct& Skill = PetInfo.Skill[i];

		if( Skill.MagicColID == ActiveMagicID )
		{
			Skill.IsActive = Active;
		}
	}	
	//---------------------------------------------------------------------------

	g_pActionFrame->SetPetAutoSpellState(ActiveMagicID, Active);
}
void NetCli_MagicChild::R_CreatePet_Range		( int PetGUID )
{
	CRoleSprite* sprite = g_pGameMain->FindSpriteByID( PetGUID );
	if ( sprite )
	{
		sprite->SetActorState(ruFUSION_ACTORSTATE_SPAWN_BEGIN);
	}
}
void NetCli_MagicChild::R_RaiseMagic( int SpellerID , int MagicBaseID )
{
	g_pDebugMessage->DebugOutput( 0, "你被施展復活術");
	g_pGameMain->ResurrectRequest(SpellerID, MagicBaseID);
}

//---------------------------------------------------------------------------
void NetCli_MagicChild::CombatLog(string combatStr, string name_s, string name_t, string spellName, MagicAttackTypeENUM atkType, int dmg, int absorb)
{
	if ( combatStr.empty() )
		return;
	
	char dmgStr[256];
	char absorbStr[256];
	sprintf_s(dmgStr, 256, "|cFFFF6600%d|r", dmg);
	sprintf_s(absorbStr, 256, "|cFFFF6600%d|r", absorb);

	/*
	if ( g_pGameMain ) {
		string playerName = g_pGameMain->GetPlayerName();
		if ( playerName.compare(name_s) == 0 )
			name_s = g_ObjectData->GetString("SYS_YOU");
		if ( playerName.compare(name_t) == 0 )
			name_t = g_ObjectData->GetString("SYS_YOU");
	}	
	*/

	const char* atkType_s;
	switch (atkType)
	{
	case EM_MagicAttackType_HP:
		atkType_s = g_ObjectData->GetString("SYS_HEALTH");
		break;
	case EM_MagicAttackType_MP:
		atkType_s = g_ObjectData->GetString("SYS_MANA");
		break;
	case EM_MagicAttackType_SP_Warrior:
		atkType_s = g_ObjectData->GetString("SYS_FURY");
		break;
	case EM_MagicAttackType_SP_Ranger:
		atkType_s = g_ObjectData->GetString("SYS_CONCENTRATION");
		break;
	case EM_MagicAttackType_SP_Rogue:
		atkType_s = g_ObjectData->GetString("SYS_VIGOR");
		break;
	}

	size_t begin;
	while ( (begin = combatStr.find("[$SN]")) != string::npos )
		combatStr.replace(begin, 5, name_s);

	while ( (begin = combatStr.find("[$TN]")) != string::npos )
		combatStr.replace(begin, 5, name_t);

	while ( (begin = combatStr.find("[$MN]")) != string::npos )
		combatStr.replace(begin, 5, spellName);

	while ( (begin = combatStr.find("[$VA]")) != string::npos )
		combatStr.replace(begin, 5, dmgStr);

	while ( (begin = combatStr.find("[$AT]")) != string::npos )
		combatStr.replace(begin, 5, atkType_s);

	while ( (begin = combatStr.find("[$AB]")) != string::npos )
		combatStr.replace(begin, 5, absorbStr);	

	g_pChatFrame->SendChatMessage(CHAT_MSG_COMBAT, "", combatStr.c_str());
}

//---------------------------------------------------------------------------
void NetCli_MagicChild::R_BuffSkillResult		( bool Result  )
{

}

//---------------------------------------------------------------------------
void NetCli_MagicChild::R_PartyMemberBuff ( int MemberDBID , int Count , MemberBuffInfoStruct Buff[_MAX_BUFF_COUNT_] )
{
	PartyMemberInfo* memberInfo = NetCli_PartyChild::GetGroupMemberByDBID(MemberDBID);
	if ( memberInfo && memberInfo->sprite )
	{
		CRoleSprite* sprite = static_cast<CRoleSprite*>(memberInfo->sprite);
		sprite->ClearDisplayBuff();
		for ( int i = 0; i < Count; i++ )
		{
			sprite->AddDisplayBuff(Buff[i].BuffID, Buff[i].PowerLv, (float)Buff[i].Time);
		}
	}
}