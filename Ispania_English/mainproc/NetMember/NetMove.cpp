#include "../GameMain.h"
#include "../InterfaceMember/WorldFrame.h"
#include "../InterfaceMember/ChatFrame.h"

// --------------------------------------------------------------------------------------
// �إߪ����@�ɤ�
void CGameMain::AddToPartition( int GItemID, int LGItemID, RolePosStruct* Pos, PlayerGraphStruct* GraphObj )
{
	g_pError->AddMessage( 2, 0, "AddToPartition GItemID(%d) LGItemID(%d) Name(%s) LV(%d) MaxHP(%d)", GItemID, LGItemID , GraphObj->Name.Begin() , GraphObj->Level , GraphObj->MaxHP);
	CRoleSprite* pSprite = CreateRoleSprite( GItemID, LGItemID );
	if ( pSprite )
	{		
		CRuVector3 pos(Pos->X, Pos->Y, Pos->Z);
        
		pSprite->ClearFade();
		
		pSprite->SetObjectId( GraphObj->OrgObjID );
		pSprite->SetName( GraphObj->Name.Begin() );
		pSprite->SetMaxHP( GraphObj->MaxHP );
		pSprite->SetHP( GraphObj->HP );
		pSprite->SetLevel( GraphObj->Level );
		pSprite->SetMoveSpeed( GraphObj->MoveSpeed );
		pSprite->SetModelRate( GraphObj->ModelRate );
        pSprite->SetDirection( Pos->Dir * PI / 180.0f );		
		pSprite->AdjustSlefPos( pos + g_offsetPosstion );
		pSprite->SetQuestInfo( GraphObj->NpcQuestID, GraphObj->TouchPlot_Range, GraphObj->RangePlot_Range );

		// ���`
		if ( GraphObj->Action.Dead )
		{
		}

		// �԰�
		if ( GraphObj->Action.AttackMode )
		{
		}

		// �P�_����欰. ��h�O, �@�Ӫ���P�����ӥu�঳�@�ئ欰. �n�O����إH�W, ���N�N���Ƴ]�w���~
		//-------------------------------------------------------------------------------
		int	iSetCount = 0;

		// �P�_�O�_�O�i�԰�������
		if ( GraphObj->Mode.Fight )
		{
			pSprite->SetCheckedStatus( eChecked_Fight );
			iSetCount++;
		}
	
		// �������� NPC �� ���� Obj
		//if( GraphObj->NpcQuestID != 0 )
			//int iQuestNpcBeginID	= Def_ObjectClass_QuestNPC;
			//int iQuestNpcEndID		= Def_ObjectClass_QuestNPC + Def_ObjectClass_QuestNPC_Total;

			GameObjDbStruct	*pObj = g_ObjectData->GetObj( GraphObj->OrgObjID );
			
			//if( GraphObj->OrgObjID >= iQuestNpcBeginID && GraphObj->OrgObjID < iQuestNpcEndID )

			if( pObj && pObj->Classification == EM_ObjectClass_QuestNPC )
			{
				// ���o QuestNpc ����
				GameObjectNpcStruct			*pNPC	= &( pObj->NPC );

				if( pNPC->iQuestMode == 0 )
				{
					pSprite->SetCheckedStatus( eChecked_QuestNPC );		
				}
				else
				{
					pSprite->SetCheckedStatus( eChecked_QuestObj );
				}

				iSetCount++;
			}

		// Ĳ�IĲ�o�@�� OBJ
			if ( GraphObj->TouchPlot_Range != 0 )
			{
				pSprite->SetCheckedStatus( eChecked_TouchPlot );
				iSetCount++;
			}

		// �Z��Ĳ�o�@�� OBJ
			if ( GraphObj->RangePlot_Range != 0 )
			{
				// 10/5 �Ҽ{ �d��Ĳ�o �� �I��Ĳ�o�i�H�@�s
				//pSprite->SetCheckedStatus( eChecked_RangePlot );
				//iSetCount++;
			}
		
		if( iSetCount > 1 )
		{
			// POP Message AddToPartition ����ӥH�W���欰�]�w.
			g_pError->AddMessage( 0, 0, "AddToPartition  GItemID(%d) LGItemID(%d) Obj ����ӥH�W���欰�]�w", GItemID, LGItemID );
		}
		//-------------------------------------------------------------------------------
	}
}

// --------------------------------------------------------------------------------------
// �R������
void CGameMain::DelFromPartition( int GItemID  )
{
	g_pError->AddMessage( 2, 0, "DelFromPartition GItemID(%d)", GItemID );
	CRoleSprite* pSprite = FindRoleSprite( GItemID );
	if ( pSprite )
		pSprite->SetFade( true, 2.0f );
	DeleteRoleSprite( GItemID );
}

// --------------------------------------------------------------------------------------
// �]�w����y��
void CGameMain::SetPos( int GItemID, RolePosStruct* Pos )
{
	CRoleSprite* pSprite = FindRoleSprite( GItemID );	
	if ( pSprite )
	{
		CRuVector3 pos(Pos->X, Pos->Y, Pos->Z);
		pos += g_offsetPosstion;
		pSprite->SetServerPos( pos );
		pSprite->SetPos( pos );
		pSprite->SetDirection( Pos->Dir * PI / 180.0f );
	}
	else
		g_pError->AddMessage( 0, 0, "[SetPos] Can't Find then GItemID(%d)", GItemID );
}

// --------------------------------------------------------------------------------------
// ���󲾰�
void CGameMain::ObjectMove( int GItemID, RolePosStruct* Pos )
{
	CRoleSprite* pSprite = FindRoleSprite( GItemID );
	if ( pSprite )
	{
		CRuVector3 tarPos(Pos->X, Pos->Y, Pos->Z);
		// g_pError->AddMessage( 2, 0, "[SetPos] (%.2f, %.2f, %.2f)", Pos->X, Pos->Y, Pos->Z );

		tarPos += g_offsetPosstion;
		pSprite->SetDirection( Pos->Dir * PI / 180.0f );
		if ( pSprite->GetServerPos() != tarPos )
		{
			pSprite->AdjustPosWithServerPos();
			pSprite->SetServerPos( tarPos );
			pSprite->PathMove( tarPos );
		}		
	}
	else
		g_pError->AddMessage( 0, 0, "[ObjectMove] Can't Find then GItemID(%d)", GItemID );
}

// --------------------------------------------------------------------------------------
// ���w����ʰ�
void CGameMain::DoAction( int GItemID, int ActionID , int TempAction)
{
	/*
	if ( GItemID == g_pWorldFrame->GetPlayerID() )
		return;
	*/

	CRoleSprite* pSprite = FindRoleSprite( GItemID );
	if ( pSprite )
	{			
		RoleSpriteStatus status;
		status.flag = ActionID >> 16;
		int action = ActionID & 0xFFFF;

		pSprite->SetFight( status.isFight );
		pSprite->SetWalk( status.isWalk );
		pSprite->SetWeaponPosition( status.isUnholster );		

		switch ( action )
		{
		case eAction_Sit:
		case eAction_Sleep:
			if ( pSprite->IsMove() )
				pSprite->SetAction( action );
			else
			{
				SpriteEventInfo event;
				event.func = CRoleSprite::DelayAction;
				event.nowTime = 0.5f;
				event.userData[0] = action;
				pSprite->PushEvent( -2, event );
			}
			break;

		default:
			if ( action != eAction_Null )
				pSprite->SetAction( action );
			break;
		}
	}
	else
		g_pError->AddMessage( 0, 0, "[DoAction] Can't Find then GItemID(%d)", GItemID );
}

// --------------------------------------------------------------------------------------
// �]�w���ʳt��
void CGameMain::MoveSpeed( int GItemID, float Speed )
{
	CRoleSprite* pSprite = FindRoleSprite( GItemID );
	if ( pSprite )
	{
		pSprite->SetMoveSpeed( Speed );
	}
	else
		g_pError->AddMessage( 0, 0, "[MoveSpeed] Can't Find then GItemID(%d)", GItemID );
}

//--------------------------------------------------------------------------------------
void CGameMain::ModelRate( int GItemID, float ModelRate )
{
	CRoleSprite* pSprite = FindRoleSprite( GItemID );
	if ( pSprite )
	{
	}
	else
		g_pError->AddMessage( 0, 0, "[ModelRate] Can't Find then GItemID(%d)", GItemID );
}

//--------------------------------------------------------------------------------------
void CGameMain::EQInfo ( int GItemID , LookEQInfoStruct&    EQLook )
{
	CRoleSprite* pSprite = FindRoleSprite( GItemID );	
	if ( pSprite )
	{
		bool isChange = false;
		pSprite->SetPaperdollId( ePaperdoll_Helmet, EQLook.Head );
		pSprite->SetPaperdollId( ePaperdoll_Hand, EQLook.Gloves );
		pSprite->SetPaperdollId( ePaperdoll_Shoulder, EQLook.Shoulder );
		pSprite->SetPaperdollId( ePaperdoll_Torso, EQLook.Clothes );
		pSprite->SetPaperdollId( ePaperdoll_Leg, EQLook.Pants );
		pSprite->SetPaperdollId( ePaperdoll_Belt, EQLook.Belt );
		pSprite->SetPaperdollId( ePaperdoll_Foot, EQLook.Shoes );
		pSprite->SetPaperdollId( ePaperdoll_Back, EQLook.Back );
		pSprite->PaperdollBuild();

		pSprite->SetRightObjectId( EQLook.MainHand );
		pSprite->SetLeftObjectId( EQLook.SecondHand );
	}
	else
		g_pError->AddMessage( 0, 0, "[EQInfo] Can't Find then GItemID(%d)", GItemID );
}

//--------------------------------------------------------------------------------------
void CGameMain::FixEQInfo ( int GItemID , EQWearPosENUM  Pos , int EQ )
{
	CRoleSprite* pSprite = FindRoleSprite( GItemID );
	if ( pSprite )
	{		
		switch ( Pos )
		{
		case EM_EQWearPos_Head:
			pSprite->SetPaperdollId( ePaperdoll_Helmet, EQ );
			break;

		case EM_EQWearPos_Gloves:
			pSprite->SetPaperdollId( ePaperdoll_Hand, EQ );
			break;

		case EM_EQWearPos_Shoes:
			pSprite->SetPaperdollId( ePaperdoll_Foot, EQ );
			break;

		case EM_EQWearPos_Clothes:
			pSprite->SetPaperdollId( ePaperdoll_Torso, EQ );
			break;

		case EM_EQWearPos_Pants:
			pSprite->SetPaperdollId( ePaperdoll_Leg, EQ );
			break;

		case EM_EQWearPos_Back:
			pSprite->SetPaperdollId( ePaperdoll_Back, EQ );
			break;

		case EM_EQWearPos_Belt:
			pSprite->SetPaperdollId( ePaperdoll_Belt, EQ );
			break;

		case EM_EQWearPos_Shoulder:
			pSprite->SetPaperdollId( ePaperdoll_Shoulder, EQ );
			break;

		case EM_EQWearPos_Necklace:
			break;

		case EM_EQWearPos_Ammo:
			break;

		case EM_EQWearPos_Ring1:
			break;

		case EM_EQWearPos_Ring2:
			break;

		case EM_EQWearPos_Earring1:
			break;

		case EM_EQWearPos_Earring2:
			break;

		case EM_EQWearPos_MainHand:
			pSprite->SetLeftObjectId( EQ );			
			break;

		case EM_EQWearPos_SecondHand:
			pSprite->SetLeftObjectId( EQ );
			break;
		}

		pSprite->PaperdollBuild();
	}
	else
		g_pError->AddMessage( 0, 0, "[FixEQInfo] Can't Find then GItemID(%d)", GItemID );
}

//--------------------------------------------------------------------------------------
void CGameMain::LookFace ( int GItemID , LookStruct& Look )
{
	CRoleSprite* pSprite = FindRoleSprite( GItemID );
	if ( pSprite )
	{
		g_pError->AddMessage( 2, 0, "[LookFace] GItemID(%d)", GItemID );
		pSprite->SetPaperdollColor( Look.HairColor, Look.BodyColor );
		pSprite->SetPaperdollId( ePaperdoll_Face, Look.FaceID );
		pSprite->SetPaperdollId( ePaperdoll_Hair, Look.HairID );
		pSprite->PaperdollBuild();
	}
	else
		g_pError->AddMessage( 0, 0, "[LookFace] Can't Find then GItemID(%d)", GItemID );
}

void CGameMain::ClientAutoPlot     ( int GItemID , char* AutoPlot )
{

}

//--------------------------------------------------------------------------------------
void CGameMain::ActionType ( int GItemID, RoleActionStruct Action )
{
	if ( GItemID == g_pWorldFrame->GetPlayerID() )
	{
		if ( Action.AttackMode )
        {
            g_pError->AddMessage( 2, 0, "AttackMode ON " );
            g_pWorldFrame->LockFight();
        }
		else
        {
            g_pError->AddMessage( 2, 0, "AttackMode OFF " );
            g_pWorldFrame->UnlockFight();
        }
		
	}
}
void   CGameMain::R_JumpBegin( int GItemID , float X , float Y , float Z , float Dir , float vX , float vY , float vZ )
{

}
/*
void   CGameMain::R_JumpEnd( int GItemID , float X , float Y , float Z , float Dir )
{

}
*/