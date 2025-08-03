#include "../GameMain.h"
#include "../InterfaceMember/ChatFrame.h"
#include "../InterfaceMember/WorldFrame.h"
#include "../interfacemember/UIOptionsFrame.h"

void CGameMain::R_AttackSignalResult ( int TargetID , AttackSignalResultENUM Result )
{
    switch( Result )
    {
    case EM_AttackSignalResult_OK:
        g_pError->AddMessage( 2, 0, "�}�ҧ���" );
        break;
    case EM_AttackSignalResult_TargetDisAttackAbled:
        g_pError->AddMessage( 2, 0, "�ؼФ��i����" );
        break;
    case EM_AttackSignalResult_TargetNULL:
        g_pError->AddMessage( 2, 0, "�ؼФ��s�b" );
        break;

    }
}
void CGameMain::R_AttackResult ( int AttackID , int UnderAttackID , int DamageHP , ATTACK_DMGTYPE_ENUM	Type )
{
	CRoleSprite* pAttackPlayer = FindRoleSprite( AttackID );
	CRoleSprite* pHurtPlayer = FindRoleSprite( UnderAttackID );
	int playerId = g_pWorldFrame->GetPlayerID();

	if ( pAttackPlayer && pHurtPlayer )
	{
		// �Y�ONPC�ץ����V
		if ( pAttackPlayer->GetObjectType() == eSpriteObject_NPC )		
			pAttackPlayer->SetDirection( pHurtPlayer->GetPos() - pAttackPlayer->GetPos() );

		// �����̬O�ۤv
		if ( playerId == AttackID )
		{
			if ( g_pUIOptionsFrame->m_isAutoFaceToFace )
				pAttackPlayer->SetDirection( pHurtPlayer->GetPos() - pAttackPlayer->GetPos() );

			switch ( Type )
			{
			case EM_ATTACK_DMGTYPE_MISS:		//MISS
				CChatFrame::AddMessage( 1, DEF_COLOR_AQUAMARINE, "Attack to %s, Miss!!", pHurtPlayer->GetName() );
				break;

			case EM_ATTACK_DMGTYPE_DODGE:
				CChatFrame::AddMessage( 1, DEF_COLOR_AQUAMARINE, "Attack to %s, Dodge!!", pHurtPlayer->GetName() );
				break;

			case EM_ATTACK_DMGTYPE_HALF:
				CChatFrame::AddMessage( 1, DEF_COLOR_AQUAMARINE, "Attack to %s, Parry!!", pHurtPlayer->GetName() );
				break;

			case EM_ATTACK_DMGTYPE_NORMAL:
				CChatFrame::AddMessage( 1, DEF_COLOR_AQUAMARINE, "Attack to %s, Normal damage %d", pHurtPlayer->GetName(), DamageHP );
				break;

			case EM_ATTACK_DMGTYPE_DOUBLE:
				CChatFrame::AddMessage( 1, DEF_COLOR_AQUAMARINE, "Attack to %s, Double damage %d", pHurtPlayer->GetName(), DamageHP );
				break;

			case EM_ATTACK_DMGTYPE_TRIPLE:
				CChatFrame::AddMessage( 1, DEF_COLOR_AQUAMARINE, "Attack to %s, Triple damage %d", pHurtPlayer->GetName(), DamageHP );
				break;

			case EM_ATTACK_DMGTYPE_HITBACK:
				CChatFrame::AddMessage( 4, DEF_COLOR_AQUAMARINE, "Hitback to %s, damage %d", pHurtPlayer->GetName(), DamageHP );
				break;
			}
		}

		if ( playerId == UnderAttackID )
		{
			switch ( Type )
			{
			case EM_ATTACK_DMGTYPE_MISS:
				CChatFrame::AddMessage( 1, DEF_COLOR_RED, "%s Attack to you, Miss!", pAttackPlayer->GetName() );
				break;

			case EM_ATTACK_DMGTYPE_DODGE:
				CChatFrame::AddMessage( 1, DEF_COLOR_RED, "%s Attack to you, Dodge!", pAttackPlayer->GetName() );
				break;

			case EM_ATTACK_DMGTYPE_HALF:			//�Q�j��
				CChatFrame::AddMessage( 1, DEF_COLOR_RED, "%s Attack to you, Parry!", pAttackPlayer->GetName() );
				break;

			case EM_ATTACK_DMGTYPE_NORMAL:			//�@��
				CChatFrame::AddMessage( 1, DEF_COLOR_RED, "%s Attack to you, Normal damage %d", pAttackPlayer->GetName(), DamageHP );
				break;

			case EM_ATTACK_DMGTYPE_DOUBLE:			//�|�ߤ@��
				CChatFrame::AddMessage( 1, DEF_COLOR_RED, "%s Attack to you, Double damage %d", pAttackPlayer->GetName(), DamageHP );
				break;

			case EM_ATTACK_DMGTYPE_TRIPLE:			//�����@��
				CChatFrame::AddMessage( 1, DEF_COLOR_RED, "%s Attack to you, Triple damage %d", pAttackPlayer->GetName(), DamageHP );
				break;

			case EM_ATTACK_DMGTYPE_HITBACK:			//����
				CChatFrame::AddMessage( 1, DEF_COLOR_RED, "%s Hitback to you, Damage %d", pAttackPlayer->GetName(), DamageHP );
				break;				
			}

			if ( g_pWorldFrame->GetTargetPlayer() == NULL )
				g_pWorldFrame->SetTargetPlayer( pAttackPlayer );
		}

		SpriteEventInfo info;
		info.func			= CRoleSprite::GenenalAttack;
		info.nowTime		= 1.5f;								// �������������ɶ�
		info.userData[0]	= (int)pHurtPlayer;					// �����̫���
		info.userData[1]	= Type;								// �l������
		info.userData[2]	= DamageHP;							// �l�˭�
		pAttackPlayer->PushEvent( DEF_ATTACK_HIT_EVENT, info );

		// �����̻P�Q�����̦۰ʶi�J�԰��Ҧ�
		pAttackPlayer->SetAction( eAction_Attack );
		pHurtPlayer->SetFight( true );
		pHurtPlayer->SetHP( pHurtPlayer->GetHP() - DamageHP );
	}
}
void CGameMain::R_Dead         ( int DeaderID , int KillerID , int Time )
{
	g_pError->AddMessage( 0, 0, "[R_Dead] DeaderID %d KillerID %d Time %d", DeaderID , KillerID , Time );

	CRoleSprite* pSprite = g_pGameMain->FindRoleSprite( DeaderID );
	if ( pSprite )
	{
		SpriteEventInfo event;
		pSprite->SetAction( eAction_Dead );
		//pSprite->SetDead( true );
		/*
		if ( DeaderID != g_pWorldFrame->GetPlayerID() )
			pSprite->AdjustPosWithServerPos();

		event.func			= CRoleSprite::DelayAction;
		event.nowTime		= 0.6f;
		event.userData[0]	= eAction_Dead;		
		pSprite->PushEvent( 0, event );
		*/

		// ���ȭp����ļƶq�ˬd
		//-------------------------------------------------------------

		if( KillerID == g_pWorldFrame->GetPlayerID() )	// �ˬd�O���O���⥻���ζ��ͱ���
		{
			// �ն��{�b�٥���@, ����A�B�z
			// ���`�ؼлP�������Z���b���b���\�d��
				

			// �@�����ŦX, �p����ĭp��
			CNetScript::OnKillObj( pSprite );
		}			
	}
}

void CGameMain::R_Resurrection ( int ItemID )
{
	CRoleSprite* pSprite = g_pGameMain->FindRoleSprite( ItemID );
	if ( pSprite )
	{
		//pSprite->SetDead( false );
		pSprite->SetFight( false );
		pSprite->SetAction(eAction_Revive);
	}
}
void CGameMain::R_HPMPSP         ( int ItemID , int HP , int MP , int SP )
{
	CRoleSprite* pSprite = g_pGameMain->FindRoleSprite( ItemID );
	if ( pSprite )
	{
		pSprite->SetHP( HP );
		pSprite->SetMP( MP );
	}
}
void CGameMain::R_MaxHPMaxMPMaxSP   ( int ItemID , int MaxHP , int MaxMP , int MaxSP)
{
	CRoleSprite* pSprite = g_pGameMain->FindRoleSprite( ItemID );
	if ( pSprite )
	{
		pSprite->SetMaxHP( MaxHP );
		pSprite->SetMaxMP( MaxMP );
	}
}