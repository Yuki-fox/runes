#include "../GameMain.h"
#include "../InterfaceMember/WorldFrame.h"

// --------------------------------------------------------------------------------------
int ChangeAttachPoint( ArgumentPointENUM point )
{
	switch ( point )
	{
	case EM_Argument_Hit_Point:
		return eAttach_Hit_Point1;

	case EM_Argument_Back_Staff:
		return eAttach_Back_Staff;

	case EM_Argument_Back_Shield:
		return eAttach_Back_Shield;

	case EM_Argument_Back_Sworld:
		return eAttach_Back_Sword;

	case EM_Argument_Name_Title:
		return eAttach_Name_Title;

	case EM_Argument_Hand_Shield:
		return eAttach_Hand_Shield;

	case EM_Argument_Left_Hand:
		return eAttach_Left_Hand;

	case EM_Argument_Right_Hand:
		return eAttach_Right_Hand;

	case EM_Argument_Left_Foot:
		return eAttach_Left_Foot;

	case EM_Argument_Right_Foot:
		return eAttach_Right_Foot;

	case EM_Argument_Left_Weapon:
		return eAttach_Left_Weapon;

	case EM_Argument_Right_Weapon:
		return eAttach_Right_Weapon;
	}

	return eAttach_Null;
}

// --------------------------------------------------------------------------------------
int ChangeMotionId( MotionENUM motion )
{
	switch ( motion )
	{
	case EM_Motion_Casting_Delay:
		return eMotion_Casting_Delay;

	case EM_Motion_Cast_01:
		return eMotion_Cast_01;

	case EM_Motion_Cast_02:
		return eMotion_Cast_02;
	}
	return eMotion_Null;
}

// --------------------------------------------------------------------------------------
void CGameMain::R_BeginSpell         ( int OwnerID, int TargetID, int MagicID )
{
    g_pError->AddMessage( 0, 0, "[R_BeginSpell] OwnerID %d TargetId %d MagicID%d", OwnerID , TargetID , MagicID );

	GameObjDbStruct* pObj = g_ObjectData->GetObj(MagicID);
	if ( pObj )
	{
		CRoleSprite* player = FindRoleSprite(OwnerID);
		if ( player )
		{				
			CEffectSprite* pEffect[2];

			for ( int i = 0; i < 2; i++ )
			{
				pEffect[i] = NULL;
				if ( pObj->ImageObj->GatherPartical[i].Name[0] > 32 )
				{
					pEffect[i] = new CEffectSprite;
					if ( pEffect[i]->LoadRuEntity( pObj->ImageObj->GatherPartical[i].Name ) )
					{							
						pEffect[i]->SetEffectMotionStep( eEffectMotion_Step_Appear );
						pEffect[i]->SetRelative( player );
						pEffect[i]->SetAttachPoint( ChangeAttachPoint(pObj->ImageObj->GatherPartical[i].Point) );
					}
					else
					{
						delete pEffect[i];
						pEffect[i] = NULL;
					}
				}
			}

			player->PushGatherEffect( pEffect[0], pEffect[1] );
			player->SetSpell( true );
			if ( pObj->ImageObj->GatherMotion != EM_Motion_Null )
			{
				player->SetGatherMotion( ChangeMotionId(pObj->ImageObj->GatherMotion) );
				player->SetAction( eAction_Gather_Delay );
			}
		}
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::R_EndSpell           ( int OwnerID )
{	
    g_pError->AddMessage( 0, 0, "[R_EndSpell] OwnerID %d ", OwnerID );

	CRoleSprite* player = FindRoleSprite(OwnerID);
	if ( player )
	{
		player->SetSpell( false );
		player->ClearGatherEffect();
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::R_BeginSpellOK       ( int MagicID )
{
    g_pError->AddMessage( 0, 0, "[R_BeginSpellOK] MagicID %d ", MagicID );
}

// --------------------------------------------------------------------------------------
void CGameMain::R_BeginSpellFailed   ( int MagicID )
{
    g_pError->AddMessage( 0, 0, "[R_BeginSpellFailed] MagicID %d ", MagicID );

	/*
	CRoleSprite* player = FindRoleSprite(OwnerID);
	if ( player )
		player->CancelCasting();
	*/
}

// --------------------------------------------------------------------------------------
/*
void CGameMain::R_MagicAttackOK      ( int OwnerID , int TargetID , int MagicID , int DmgHP , int DmgMP )
{	
    g_pError->AddMessage( 0, 0, "[R_MagicAttackOK] AtkID = %d  Target=%d MagicID %d  DmgHP=%d DmgMp=%d", OwnerID , TargetID , MagicID , DmgHP , DmgMP  );

	GameObjDbStruct* pObj = g_ObjectData->GetObj(MagicID);
	if ( pObj )
	{
		CRoleSprite* attack = FindRoleSprite(OwnerID);
		CRoleSprite* accept = FindRoleSprite(TargetID);
		if ( attack )
		{
			if ( pObj->ImageObj->ShootMotion != EM_Motion_Null )
			{
				attack->SetCastingMotion( ChangeMotionId(pObj->ImageObj->ShootMotion) );
				attack->SetAction( eAction_Casting );
			}
		}

		if ( accept )
		{
			accept->SetHP( accept->GetHP() + DmgHP );
			accept->SetMP( accept->GetMP() + DmgMP );
		}
		
		CEffectSprite* pSpawn = NULL;
		CEffectSprite* pExplode = NULL;
		CLineEffectSprite* pFly = NULL;		

		// 出手特效
		if ( pObj->ImageObj->ShootACT[0] > 32 )
		{
			pSpawn = new CEffectSprite;
			if ( pSpawn->LoadRuEntity( pObj->ImageObj->ShootACT ) )
			{
				pSpawn->SetEffectMotionStep( eEffectMotion_Step_Explode );

				pSpawn->SetPauseTime( pObj->ImageObj->ShootMotionDelayTime / 10.0f );
				pSpawn->BeginSprite( attack, ChangeAttachPoint(pObj->ImageObj->FlyBeginPoint) );
			}
			else
			{
				delete pSpawn;
				pSpawn = NULL;
			}
		}


		// 飛行特效
		if ( pObj->ImageObj->FlyACT[0] > 32 )
		{		
			pFly = new CLineEffectSprite;
			if ( pFly->LoadRuEntity( pObj->ImageObj->FlyACT ) )
			{
				pFly->SetEffectMotionStep( eEffectMotion_Step_Appear );

				pFly->SetPauseTime( pObj->ImageObj->ShootMotionDelayTime / 10.0f );
				pFly->BeginSprite( attack, ChangeAttachPoint(pObj->ImageObj->FlyBeginPoint) );
				pFly->SetRelative( accept );
				pFly->SetAttachPoint( ChangeAttachPoint(pObj->ImageObj->TargetPoint) );
				pFly->SetMoveSpeed( 32.0f );				
			}
			else
			{
				delete pFly;
				pFly = NULL;
			}
		}

		// 目的身上特效		
		if ( pObj->ImageObj->ExplodeACT[0] > 32 )
		{
			pExplode = new CEffectSprite;
			if ( pExplode->LoadRuEntity( pObj->ImageObj->ExplodeACT ) )
			{

				pExplode->SetEffectMotionStep( eEffectMotion_Step_Explode );
				if ( pFly )
				{
					pFly->AttachExplodeEffect( pExplode );
					pExplode->SetPauseTime( pObj->ImageObj->ShootMotionDelayTime / 10.0f + 3.0f );
				}
				else
					pExplode->SetPauseTime( pObj->ImageObj->ShootMotionDelayTime / 10.0f );
				
				pExplode->BeginSprite( attack, eAttach_Null );
				pExplode->SetRelative( accept );
				pExplode->SetAttachPoint( ChangeAttachPoint(pObj->ImageObj->TargetPoint) );
				pExplode->SetMagicInfo( MagicID, DmgHP, DmgMP );
			}
			else
			{
				delete pExplode;
				pExplode = NULL;
			}
		}
	}
}

// --------------------------------------------------------------------------------------
void CGameMain::R_MagicAttackMiss    ( int OwnerID , int TargetID , int MagicID )
{	
    g_pError->AddMessage( 0, 0, "[R_MagicAttackMiss] AtkID = %d  Target=%d MagicID %d ", OwnerID , TargetID , MagicID  );
}

// --------------------------------------------------------------------------------------
void CGameMain::R_MagicAssistOK      ( int OwnerID , int TargetID , int MagicID )
{
    g_pError->AddMessage( 0, 0, "[R_MagicAssistOK] AtkID = %d  Target=%d MagicID %d ", OwnerID , TargetID , MagicID  );
}

// --------------------------------------------------------------------------------------
void CGameMain::R_MagicAssistFailed  ( int OwnerID , int TargetID , int MagicID )
{
    g_pError->AddMessage( 0, 0, "[R_MagicAssistFailed] AtkID = %d  Target=%d MagicID %d ", OwnerID , TargetID , MagicID  );
}
*/
// --------------------------------------------------------------------------------------
void CGameMain::R_FixAssistMagicTime ( int MagicID , int EffectTime )
{
    g_pError->AddMessage( 0, 0, "[R_FixAssistMagicTime] MagicID =%d EffectTime=%d", MagicID , EffectTime  );
}

// --------------------------------------------------------------------------------------
void CGameMain::R_CancelSpellMagic   ( int OwnerID )
{
     g_pError->AddMessage( 0, 0, "[R_CancelSpellMagic] OwnerID =%d ", OwnerID );
}
// --------------------------------------------------------------------------------------
void CGameMain::R_MagicAttack        ( int OwnerID , int TargetID , int MagicID , vector< MagicAtkEffectInfoStruct >& List )
{

}
// --------------------------------------------------------------------------------------
void CGameMain::R_MagicAssist        ( int OwnerID , int TargetID , int MagicID , vector< MagicAssistEffectInfoStruct >& List )
{

}
// --------------------------------------------------------------------------------------
void CGameMain::R_SetMagicPointResult( int MagicID , int MagicPos , bool Result , int ErrorCode )
{
    if( Result == true )
    {
        g_pError->AddMessage( 0, 0, "MaigcID(%d) ", MagicID );
    }
    else
    {
        g_pError->AddMessage( 0, 0, "MaigcID(%d) ", MagicID );
    }
}
// --------------------------------------------------------------------------------------