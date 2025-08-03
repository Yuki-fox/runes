#include "../../mainproc/GameMain.h"
#include "../../Interface/Debug/DebugMessage.h"
#include "../../Interface/WorldFrames/ChatFrame.h"
#include "../../Interface/WorldFrames/CastingBarFrame.h"
#include "../../Interface/WorldFrames/ScrollCombatTextFrame.h"

#include "../net_party/NetCli_PartyChild.h"
#include "../net_script/NetCli_Script.h"
#include "../net_magic/NetCli_MagicChild.h"
#include "NetCli_AttackChild.h"

#include "../../Interface/WorldFrames/CombatMeter.h"

#include "../../Interface/Socal/UI_Socal.h"

//-----------------------------------------------------------------------------
bool NetCli_AttackChild::_Init()
{
	if (This)
		return false;	

	This = new NetCli_AttackChild;
	return NetCli_Attack::_Init();
}

//-----------------------------------------------------------------------------
bool NetCli_AttackChild::_Release()
{
	NetCli_Attack::_Release();

	if (This)
	{
		delete This;
		This = NULL;
	}
	return true;
}

//-----------------------------------------------------------------------------
void NetCli_AttackChild::R_AttackSignalResult( int TargetID , AttackSignalResultENUM Result )
{
	switch( Result )
	{
	case EM_AttackSignalResult_OK:
		g_pDebugMessage->DebugOutput(2, "�}�ҧ���");
		break;
	case EM_AttackSignalResult_TargetDisAttackAbled:
		g_pDebugMessage->DebugOutput(2, "�ؼФ��i����");
		break;
	case EM_AttackSignalResult_TargetNULL:
		g_pDebugMessage->DebugOutput(2, "�ؼФ��s�b");
		break;
	}
}

//-----------------------------------------------------------------------------
void NetCli_AttackChild::R_AttackResult( int AttackID, int UnderAttackID, int DamageHP, int OrgDamage , ATTACK_DMGTYPE_ENUM Type, AttackHandTypeENUM HandType )
{
	g_pDebugMessage->DebugOutput(2, "R_AttackResult AttackID=%d UnderAttackID=%d DamageHP=%d Type=%d AttackHandTypeENUM=%d", AttackID, UnderAttackID, DamageHP,	Type, HandType);
	//g_pError->AddMessage(0, 0, "R_AttackResult AttackID=%d UnderAttackID=%d DamageHP=%d Type=%d AttackHandTypeENUM=%d", AttackID, UnderAttackID, DamageHP,	Type, HandType);

	CRoleSprite* player = g_pGameMain->GetPlayer();
	CRoleSprite* attack = g_pGameMain->FindSpriteByID(AttackID);
	CRoleSprite* target = g_pGameMain->FindSpriteByID(UnderAttackID);
	CRoleSprite* pet	= (player) ? player->GetPetSprite() : NULL;
	bool offHand = (HandType == EM_AttackHandType_Second);

	if ( attack == NULL || target == NULL )
		return;	

	if ( attack->GetRideAttackState() == false )
	{
		// �����̬O�ۤv�۰ʤU��
		if ( attack == player ) {
			g_pGameMain->Dismount();
		} else {
			attack->ReleaseMountSprite();
		}
	}

	int absorbPoint = OrgDamage - DamageHP;
	
	// �Y�ONPC�۰ʭץ����V�A�¦V�Q�����ﹳ
	switch ( attack->GetObjectType() )
	{
	case eSpriteObject_NPC:
	case eSpriteObject_QuestNPC:
		if ( attack && attack->GetMoveTime() < 0.01f && attack->GetFollowSprite() == NULL )
		{
			attack->SetCorrectDir(CRuVector3(0.0f, 0.0f, 0.0f));
			attack->SetDirection(target->GetPos() - attack->GetPos());
		}
		break;
	}

	// �p�G�O���ê���(�@����t��)	
	if ( attack && attack->IsScriptHide() )
		return;

	// �����P�Q������,�Ѱ�(���U,��U,���U)
	if ( Type != EM_ATTACK_DMGTYPE_MISS )
	{
		switch (attack->GetActorState())
		{
		case ruFUSION_ACTORSTATE_SIT_BEGIN:
		case ruFUSION_ACTORSTATE_SIT_LOOP:	
			attack->SetActorState(ruFUSION_ACTORSTATE_SIT_END, 1);
			break;

		case ruFUSION_ACTORSTATE_SLEEP_BEGIN:
		case ruFUSION_ACTORSTATE_SLEEP_LOOP:	
			attack->SetActorState(ruFUSION_ACTORSTATE_SLEEP_END, 1);
			break;

		case ruFUSION_ACTORSTATE_CROUCH_BEGIN:
		case ruFUSION_ACTORSTATE_CROUCH_LOOP:
			attack->SetActorState(ruFUSION_ACTORSTATE_CROUCH_END, 1);
			break;
		}

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

	ACTActionEventsInfo* actionEventInfo = g_pACTActionEventsManager->GetACTActionEvent();
	if ( actionEventInfo )
	{
		CRoleSprite* sprite = attack;
		if ( sprite->GetRidePlayAttackMotion() && sprite->GetMountSprite() )
		{
			sprite = sprite->GetMountSprite();
		}
		
		// �����̰ʧ@
		// �����̻P�i�J�԰��Ҧ�
		sprite->SetCombatState(true);
		sprite->SetAttackMotion(offHand, actionEventInfo->m_actionEvent);

		actionEventInfo->m_target = UnderAttackID;
		actionEventInfo->m_source = AttackID;
		actionEventInfo->m_type = ACT_ACTIONEVENTS_TYPE_ATTACK;
		actionEventInfo->m_attack.m_damage = DamageHP;
		actionEventInfo->m_attack.m_dmgType = Type;
		actionEventInfo->m_attack.m_offHand = offHand;		

		// �]�w�ƥ� Callback
		actionEventInfo->m_actionEvent->RegisterEventTriggerCallback(
			ruENTITYEVENT_HIT,
			boost::bind(&CACTActionEventsManager::EventCallback, g_pACTActionEventsManager, _1, _2),
			(PTRVALUE)actionEventInfo);
	}

	// �Q�����̬O�ۤv
	if ( target == player && target->GetTargetSprite() == NULL )
		g_pGameMain->SetPlayerTarget(attack);

	//////////////////////////////////////////////////////////////////////////
	//�ۤv�������ƥ��x�s
	if( target == player )
	{
		switch( Type )
		{
		case EM_ATTACK_DMGTYPE_MISS:		    //MISS
			break;
		case EM_ATTACK_DMGTYPE_DODGE:		    //DODGE
			RoleData()->TempData.BackInfo.LastDodgeTime = RoleDataEx::G_SysTime;
			break;
		case EM_ATTACK_DMGTYPE_HALF:		    //�Q�j��
			RoleData()->TempData.BackInfo.LastParryTime= RoleDataEx::G_SysTime;			
			break;
		case EM_ATTACK_DMGTYPE_DOUBLE:			//�|�ߤ@��
			RoleData()->TempData.BackInfo.LastBeCriticalTime= RoleDataEx::G_SysTime;
			break;
		case EM_ATTACK_DMGTYPE_SHIELD_BLOCK:	//����
			break;
		}
	}
	else if( attack == player )
	{
		switch( Type )
		{
		case EM_ATTACK_DMGTYPE_MISS:		    //MISS
			RoleData()->TempData.BackInfo.LastMissTime = RoleDataEx::G_SysTime;
			break;
		case EM_ATTACK_DMGTYPE_DODGE:		    //DODGE
			RoleData()->TempData.BackInfo.LastBeDodgeTime = RoleDataEx::G_SysTime;			
			break;
		case EM_ATTACK_DMGTYPE_HALF:		    //�Q�j��
			RoleData()->TempData.BackInfo.LastBeParryTime= RoleDataEx::G_SysTime;
			break;
		case EM_ATTACK_DMGTYPE_DOUBLE:			//�|�ߤ@��
			RoleData()->TempData.BackInfo.LastCriticalTime= RoleDataEx::G_SysTime;
			break;
		case EM_ATTACK_DMGTYPE_SHIELD_BLOCK:	//����
			RoleData()->TempData.BackInfo.LastShieldBlockTime = RoleDataEx::G_SysTime;
			break;
		}
	}

	// �԰��T��
	if ( target && attack && (target == player || attack == player || target == pet || attack == pet) )
	{
		const char* name_s = attack->GetName();
		const char* name_t = target->GetName();

		char dmgStr[256];
		char absorbStr[256];
		char temp[1024];

		sprintf_s(dmgStr, 256, "%d", DamageHP);
		sprintf_s(absorbStr, 256, "%d", absorbPoint);

		/*
		if ( attack == player ) {
			name_s = g_ObjectData->GetString("SYS_YOU");
		}
		if ( target == player ) {
			name_t = g_ObjectData->GetString("SYS_YOU");
		}
		*/

		switch(Type)
		{
		//MISS
		case EM_ATTACK_DMGTYPE_MISS:
			sprintf_s(temp, 1024, "|cFFF4F021%s|r", g_ObjectData->GetString("SYS_COMBAT_ATTACK_MISS"));
			NetCli_MagicChild::CombatLog(temp, name_s, name_t, "", EM_MagicAttackType_HP, 0, 0);
			break;
		case EM_ATTACK_DMGTYPE_DODGE:				//DODGE
			sprintf_s(temp, 1024, "|cFFF4F021%s|r", g_ObjectData->GetString("SYS_COMBAT_ATTACK_DODGE"));
			NetCli_MagicChild::CombatLog(temp, name_s, name_t, "", EM_MagicAttackType_HP, DamageHP, absorbPoint);
			break;
		case EM_ATTACK_DMGTYPE_HALF:				//�Q�j��
			if ( target == player ) {
				sprintf_s(temp, 1024, "|cFFF40F11%s|r", g_ObjectData->GetString("SYS_COMBAT_ATTACK_HALF"));				
			} else {
				sprintf_s(temp, 1024, "|cFFF4F021%s|r", g_ObjectData->GetString("SYS_COMBAT_ATTACK_HALF"));
			}
			NetCli_MagicChild::CombatLog(temp, name_s, name_t, "", EM_MagicAttackType_HP, DamageHP, absorbPoint);
			break;
		case EM_ATTACK_DMGTYPE_NORMAL:				//�@��
			if ( target == player ) {
				sprintf_s(temp, 1024, "|cFFF40F11%s|r", g_ObjectData->GetString("SYS_COMBAT_ATTACK_NORMAL"));
			} else {
				sprintf_s(temp, 1024, "|cFFF4F021%s|r", g_ObjectData->GetString("SYS_COMBAT_ATTACK_NORMAL"));
			}
			NetCli_MagicChild::CombatLog(temp, name_s, name_t, "", EM_MagicAttackType_HP, DamageHP, absorbPoint);
			break;
		case EM_ATTACK_DMGTYPE_DOUBLE:				//�|�ߤ@��
			if ( target == player ) {
				sprintf_s(temp, 1024, "|cFFF40F11%s|r", g_ObjectData->GetString("SYS_COMBAT_ATTACK_DOUBLE"));
			} else {
				sprintf_s(temp, 1024, "|cFFF4F021%s|r", g_ObjectData->GetString("SYS_COMBAT_ATTACK_DOUBLE"));
			}
			NetCli_MagicChild::CombatLog(temp, name_s, name_t, "", EM_MagicAttackType_HP, DamageHP, absorbPoint);
			break;
		case EM_ATTACK_DMGTYPE_TRIPLE:				//�����@��
			if ( target == player ) {
				sprintf_s(temp, 1024, "|cFFF40F11%s|r", g_ObjectData->GetString("SYS_COMBAT_ATTACK_TRIPLE"));
			} else {
				sprintf_s(temp, 1024, "|cFFF4F021%s|r", g_ObjectData->GetString("SYS_COMBAT_ATTACK_TRIPLE"));
			}
			NetCli_MagicChild::CombatLog(temp, name_s, name_t, "", EM_MagicAttackType_HP, DamageHP, absorbPoint);
			break;
		case EM_ATTACK_DMGTYPE_HITBACK:				//����
			if ( target == player ) {
				sprintf_s(temp, 1024, "|cFFF40F11%s|r", g_ObjectData->GetString("SYS_COMBAT_ATTACK_HITBACK"));
			} else {
				sprintf_s(temp, 1024, "|cFFF4F021%s|r", g_ObjectData->GetString("SYS_COMBAT_ATTACK_HITBACK"));
			}
			NetCli_MagicChild::CombatLog(temp, name_s, name_t, "", EM_MagicAttackType_HP, DamageHP, absorbPoint);
			break;
		case EM_ATTACK_DMGTYPE_NO_EFFECT:			//�K��
			sprintf_s(temp, 1024, "|cFFF4F021%s|r", g_ObjectData->GetString("SYS_COMBAT_ATTACK_IMMUNE"));
			NetCli_MagicChild::CombatLog(temp, name_s, name_t, "", EM_MagicAttackType_HP, DamageHP, absorbPoint);
			break;
		case EM_ATTACK_DMGTYPE_ABSORB:				//�l��
			if ( absorbPoint > 0 ) {
				sprintf_s(temp, 1024, "|cFFF4F021%s|r", g_ObjectData->GetString("SYS_COMBAT_ATTACK_ABSORB"));
				NetCli_MagicChild::CombatLog(temp, name_s, name_t, "", EM_MagicAttackType_HP, DamageHP, absorbPoint);
			}
			if ( DamageHP > 0 ) {
				if ( target == player ) {
					sprintf_s(temp, 1024, "|cFFF40F11%s|r", g_ObjectData->GetString("SYS_COMBAT_ATTACK_NORMAL"));					
				} else {
					sprintf_s(temp, 1024, "|cFFF4F021%s|r", g_ObjectData->GetString("SYS_COMBAT_ATTACK_NORMAL"));
				}
				NetCli_MagicChild::CombatLog(temp, name_s, name_t, "", EM_MagicAttackType_HP, DamageHP, absorbPoint);
			}
			break;
		case EM_ATTACK_DMGTYPE_SHIELD_BLOCK:		//����
			if ( target == player ) {
				sprintf_s(temp, 1024, "|cFFF40F11%s|r", g_ObjectData->GetString("SYS_COMBAT_ATTACK_SHIELD_BLOCK"));				
			} else {
				sprintf_s(temp, 1024, "|cFFF4F021%s|r", g_ObjectData->GetString("SYS_COMBAT_ATTACK_SHIELD_BLOCK"));
			}
			NetCli_MagicChild::CombatLog(temp, name_s, name_t, "", EM_MagicAttackType_HP, DamageHP, absorbPoint);
			break;
		}	
	}

	// �԰��T��
	if( attack && target && player )
	{
		const char* szSourceName = attack->GetName();
		const char* szTargetName = target->GetName();

		if( attack == player || NetCli_PartyChild::InGroup( szSourceName ) || target == player || NetCli_PartyChild::InGroup( szTargetName ) || attack == pet || target == pet )
		{
			switch(Type)
			{
			case EM_ATTACK_DMGTYPE_MISS:			    //MISS
				g_pCombatMeter->AddDamage( szSourceName, szTargetName, "ATTACK", "MISS", DamageHP );
				break;
			case EM_ATTACK_DMGTYPE_DODGE:				//DODGE
				g_pCombatMeter->AddDamage( szSourceName, szTargetName, "ATTACK", "DODGE", DamageHP );
				break;
			case EM_ATTACK_DMGTYPE_HALF:				//�Q�j��
				g_pCombatMeter->AddDamage( szSourceName, szTargetName, "ATTACK", "HALF", DamageHP );
				break;
			case EM_ATTACK_DMGTYPE_NORMAL:				//�@��
				g_pCombatMeter->AddDamage( szSourceName, szTargetName, "ATTACK", "NORMAL", DamageHP );
				break;
			case EM_ATTACK_DMGTYPE_DOUBLE:				//�|�ߤ@��
				g_pCombatMeter->AddDamage( szSourceName, szTargetName, "ATTACK", "DOUBLE", DamageHP );
				break;
			case EM_ATTACK_DMGTYPE_TRIPLE:				//�����@��
				g_pCombatMeter->AddDamage( szSourceName, szTargetName, "ATTACK", "TRIPLE", DamageHP );
				break;
			case EM_ATTACK_DMGTYPE_HITBACK:				//����
				g_pCombatMeter->AddDamage( szSourceName, szTargetName, "ATTACK", "HITBACK", DamageHP );
				break;
			case EM_ATTACK_DMGTYPE_NO_EFFECT:			//�S�ĪG
				g_pCombatMeter->AddDamage( szSourceName, szTargetName, "ATTACK", "MISS", DamageHP );
				break;
			case EM_ATTACK_DMGTYPE_ABSORB:				//�l��
				if ( absorbPoint > 0 )
					g_pCombatMeter->AddDamage( szSourceName, szTargetName, "ATTACK", "ABSORB", absorbPoint );
				if ( DamageHP > 0 )
					g_pCombatMeter->AddDamage( szSourceName, szTargetName, "ATTACK", "NORMAL", DamageHP );
				break;
			case EM_ATTACK_DMGTYPE_SHIELD_BLOCK:		//����
				g_pCombatMeter->AddDamage( szSourceName, szTargetName, "ATTACK", "SHIELD_BLOCK", DamageHP );
				break;
			}
		}

	}

}

//-----------------------------------------------------------------------------
void NetCli_AttackChild::R_Dead( int DeaderID , int KillerID , int KillerOwnerID , int Time , RolePosStruct &Pos , int ReviveTime , DeadOptionMode Mode  )
{
	//g_pDebugMessage->DebugOutput(0, "[R_Dead] DeaderID %d KillerID %d Time %d", DeaderID , KillerID , Time);
	//g_pError->AddMessage(0, 0, "[R_Dead] DeaderID %d KillerID %d Time %d", DeaderID , KillerID , Time);

	// �������a
	if ( g_pGameMain->GetGItemID() == DeaderID ) {		
		g_pChatFrame->SendChatMessage(CHAT_MSG_COMBAT, "", g_ObjectData->GetString("SYS_COMBAT_DEATH"));
		g_pCastingBarFrame->CastingFailed(eCastingBar_All);
		g_pGameMain->SpellStopCasting();
		g_pGameMain->PlayerDead(Mode.RaiseMagic, ReviveTime);

		//Buff�M��
		//for( int i = 0 ; i < _RoleData.BaseData.Buff.Size() ; i++ )
		//{				
		//	GameObjDbStructEx* Magic = g_ObjectData->GetObj(_RoleData.BaseData.Buff[i].BuffID);
		//	if ( !Magic->IsMagicBase() || Magic->MagicBase.Setting.DeadNotClear == false )
		//	{
		//		_RoleData.BaseData.Buff.Erase( i );
		//		i--;
		//	}
		//}
		//�����M��server���e
		_RoleData.BaseData.Buff.Clear();

		CRoleSprite* killerSprite = g_pGameMain->FindSpriteByID(KillerOwnerID);
		if ( killerSprite == NULL )
			killerSprite = g_pGameMain->FindSpriteByID(KillerID);

		if ( killerSprite )
		{
			switch( killerSprite->GetObjectType() )
			{
			case eSpriteObject_Player:
				g_pUi_Socal->PlayerKillMe( killerSprite->GetDBID(), killerSprite->GetName() );
				break;
			}
		}

	}

	CRoleSprite* pDeaderSprite = g_pGameMain->FindSpriteByID( DeaderID );
	if ( pDeaderSprite )
	{
		CRoleSprite* spriteControlProxy = pDeaderSprite->GetProxyControlSprite();
		CRuVector3 newPos(Pos.X, Pos.Y, Pos.Z);
		CRuVector3 nowPos = spriteControlProxy->GetPos();

		// �ץ�HP���s
		pDeaderSprite->SetHP(0);
		pDeaderSprite->EndSpell();
		pDeaderSprite->SetCastingTime(0.0f);
		if ( spriteControlProxy->GetGravityMode() )
		{
			// ���s�w��
			CRuOptimalTerrain* pRuTerrain = g_pGameMain->GetRuTerrain();
			if ( pRuTerrain )
			{
				newPos.m_y = RuEntity_FindRestingHeight(newPos, pRuTerrain, 25.0f);
			}
		}

		// �ˬd���`�y�ЬO�_�t�Z�L��,�Y�L���������ܮy�Ц�m
		if ( (newPos - nowPos).Magnitude() > 10.0f || abs(newPos.m_y - nowPos.m_y) > 4.0f )
			spriteControlProxy->SetPos(newPos);
		
		pDeaderSprite->PushActorQueue(ruFUSION_ACTORSTATE_DYING, 0.5f);
		pDeaderSprite->SetDeadState(true);

		//���a���H����
		if( KillerOwnerID == g_pGameMain->GetGItemID() || KillerID == g_pGameMain->GetGItemID() )
		{
			if( pDeaderSprite->GetObjectType() == eSpriteObject_Player )
			{
				g_pUi_Socal->KillPlayer( pDeaderSprite->GetDBID(), pDeaderSprite->GetName() );
			}
		}
		
		/*
		// ���ȭp����ļƶq�ˬd
		//-------------------------------------------------------------
		if( KillerOwnerID == g_pGameMain->GetPlayerID() )	// �ˬd�O���O���⥻���ζ��ͱ���
		{
			// �ն��{�b�٥���@, ����A�B�z
			// ���`�ؼлP�������Z���b���b���\�d��


			// �@�����ŦX, �p����ĭp��
			CNetCli_Script::OnEvent_KillObj( pSprite->GetIdentify() );
		}
		*/
	}
}

//-----------------------------------------------------------------------------
void NetCli_AttackChild::R_Resurrection( int ItemID )
{
	CRoleSprite* pSprite = g_pGameMain->FindSpriteByID( ItemID );
	if ( pSprite )
	{
		//pSprite->SetCombatState(false);
		pSprite->SetDeadState(false);
		pSprite->SetDirection(pSprite->GetDirection(), true);
		pSprite->SetActorState(ruFUSION_ACTORSTATE_REVIVING, 1);
		pSprite->SetActorState(ruFUSION_ACTORSTATE_NORMAL, 2);
		pSprite->SetActorState(ruFUSION_ACTORSTATE_NORMAL, 3);

		if ( pSprite == g_pGameMain->GetPlayer() )
		{
			g_pChatFrame->SendChatMessage(CHAT_MSG_COMBAT, "", g_ObjectData->GetString("SYS_COMBAT_RESURRECTION"));
			g_pGameMain->PlayerResurrect();
		}
		
		/*
		//pSprite->SetDead( false );
		pSprite->SetFight( false );
		pSprite->SetAction(eAction_Revive);
		*/
	}
}

//-----------------------------------------------------------------------------
void NetCli_AttackChild::R_HPMPSP( int ItemID , int HP , int MP , int SP , int SP_Sub )
{
	// �������a
	if ( g_pGameMain->GetGItemID() == ItemID )
	{
		_RoleData.BaseData.HP = (float)HP;
		_RoleData.BaseData.MP = (float)MP;
		_RoleData.BaseData.SP = (float)SP;
		_RoleData.BaseData.SP_Sub = (float)SP_Sub;
	}

	CRoleSprite* pSprite = g_pGameMain->FindSpriteByID( ItemID );
	if ( pSprite )
	{
		int baseValue[2];
		g_pGameMain->TranslateBaseValue(pSprite, MP, SP, SP_Sub, baseValue);
		pSprite->SetHP(HP);
		pSprite->SetMP(baseValue[0]);
		pSprite->SetSP(baseValue[1]);
	}
}

//-----------------------------------------------------------------------------
void NetCli_AttackChild::R_MaxHPMaxMPMaxSP( int ItemID , int MaxHP , int MaxMP , int MaxSP , int MaxSP_Sub )
{
	// �������a
	if ( g_pGameMain->GetGItemID() == ItemID )
	{
	}

	CRoleSprite* pSprite = g_pGameMain->FindSpriteByID( ItemID );
	if ( pSprite )
	{
		int baseValue[2];
		g_pGameMain->TranslateBaseValue(pSprite, MaxMP, MaxSP, MaxSP_Sub, baseValue);
		pSprite->SetMaxHP(MaxHP);
		pSprite->SetMaxMP(baseValue[0]);
		pSprite->SetMaxSP(baseValue[1]);
	}
}

//-----------------------------------------------------------------------------
void NetCli_AttackChild::R_PartyMemberHPMPSP	( int DBID , int HP , int MP , int SP , int SP_Sub )
{
	/*
	char Buf[512];
	sprintf( Buf , "[Party����] : ��Ƨ�s DBID = %d HP = %d MP = %d SP = %d" , DBID , HP , MP , SP );	
	g_pDebugMessage->DebugOutput( 5, Buf );
	*/

	// �u���Ȧs�����T
	PartyMemberInfo* info = NetCli_PartyChild::GetGroupMemberByDBID(DBID);
	if ( info ) {
		CRoleSprite* sprite = (CRoleSprite*)(info->sprite);
		int baseValue[2];
		g_pGameMain->TranslateBaseValue(sprite, MP, SP, SP_Sub, baseValue);
		sprite->SetHP(HP);
		sprite->SetMP(baseValue[0]);
		sprite->SetSP(baseValue[1]);
	} else {
		NetCli_PartyChild::SetPartyMemberBaseValue(DBID, HP, MP, SP, SP_Sub);
	}
}

//-----------------------------------------------------------------------------
void NetCli_AttackChild::R_PartyMemberMaxHPMPSP ( int DBID , int MaxHP , int MaxMP , int MaxSP , int MaxSP_Sub )
{
	/*
	char Buf[512];
	sprintf( Buf , "[Party����] : ��Ƨ�s DBID = %d MaxHP = %d MaxMP = %d MaxSP = %d" , DBID , MaxHP , MaxMP , MaxSP );
	g_pDebugMessage->DebugOutput( 5, Buf );
	*/

	PartyMemberInfo* info = NetCli_PartyChild::GetGroupMemberByDBID(DBID);
	if ( info ) {
		CRoleSprite* sprite = (CRoleSprite*)(info->sprite);
		int baseValue[2];
		g_pGameMain->TranslateBaseValue(sprite, MaxMP, MaxSP, MaxSP_Sub, baseValue);
		sprite->SetMaxHP(MaxHP);
		sprite->SetMaxMP(baseValue[0]);
		sprite->SetMaxSP(baseValue[1]);
	} else {
		NetCli_PartyChild::SetPartyMemberMaxBaseValue(DBID, MaxHP, MaxMP, MaxSP, MaxSP_Sub);
	}
}
//-----------------------------------------------------------------------------
void NetCli_AttackChild::R_NPCAttackTarget		( int NpcGUID , int TargetGUID )
{
	// ID �ݭn���ഫ
	// g_pDebugMessage->DebugOutput(0, "[R_NPCAttackTarget] NpcGUID %d TargetGUID %d ", NpcGUID , TargetGUID);

	CRoleSprite* sourceSprite = g_pGameMain->FindSpriteByID(NpcGUID);
	if ( sourceSprite )
	{
		switch ( sourceSprite->GetObjectType() )
		{
		case eSpriteObject_NPC:
		case eSpriteObject_QuestNPC:
			{
				CRoleSprite* targetSprite = g_pGameMain->FindSpriteByID(TargetGUID);
				sourceSprite->SetTargetSprite(targetSprite);
			}
			break;
		}
	}
}
//-----------------------------------------------------------------------------
void NetCli_AttackChild::R_SetAttackMode		( int GItemID , bool Flag )
{
	// g_pDebugMessage->DebugOutput(0, "[R_SetAttackMode] GItemID %d Flag %d ", GItemID , Flag );
	CRoleSprite* sprite = g_pGameMain->FindSpriteByID(GItemID);
	if ( sprite && sprite != g_pGameMain->GetPlayer() )
	{
		sprite->SetCombatState(Flag);
	}

	/*
	if ( sprite && sprite != g_pGameMain->GetPlayer() )
	{
		sprite->SetCombatState(Flag);
	}
	*/
}
//-----------------------------------------------------------------------------
void NetCli_AttackChild::R_ZonePKFlag			( PKTypeENUM PKType )
{
	g_pGameMain->UpdatePVPZoneState(PKType);
}