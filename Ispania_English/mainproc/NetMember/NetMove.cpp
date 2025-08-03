#include "../GameMain.h"
#include "../InterfaceMember/WorldFrame.h"
#include "../InterfaceMember/ChatFrame.h"

// --------------------------------------------------------------------------------------
// 建立物件到世界內
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

		// 死亡
		if ( GraphObj->Action.Dead )
		{
		}

		// 戰鬥
		if ( GraphObj->Action.AttackMode )
		{
		}

		// 判斷物件行為. 原則是, 一個物件同時應該只能有一種行為. 要是有兩種以上, 那就代表資料設定錯誤
		//-------------------------------------------------------------------------------
		int	iSetCount = 0;

		// 判斷是否是可戰鬥的角色
		if ( GraphObj->Mode.Fight )
		{
			pSprite->SetCheckedStatus( eChecked_Fight );
			iSetCount++;
		}
	
		// 給予任務 NPC 或 任務 Obj
		//if( GraphObj->NpcQuestID != 0 )
			//int iQuestNpcBeginID	= Def_ObjectClass_QuestNPC;
			//int iQuestNpcEndID		= Def_ObjectClass_QuestNPC + Def_ObjectClass_QuestNPC_Total;

			GameObjDbStruct	*pObj = g_ObjectData->GetObj( GraphObj->OrgObjID );
			
			//if( GraphObj->OrgObjID >= iQuestNpcBeginID && GraphObj->OrgObjID < iQuestNpcEndID )

			if( pObj && pObj->Classification == EM_ObjectClass_QuestNPC )
			{
				// 取得 QuestNpc 物件
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

		// 觸碰觸發劇情 OBJ
			if ( GraphObj->TouchPlot_Range != 0 )
			{
				pSprite->SetCheckedStatus( eChecked_TouchPlot );
				iSetCount++;
			}

		// 距離觸發劇情 OBJ
			if ( GraphObj->RangePlot_Range != 0 )
			{
				// 10/5 考慮 範圍觸發 及 點擊觸發可以共存
				//pSprite->SetCheckedStatus( eChecked_RangePlot );
				//iSetCount++;
			}
		
		if( iSetCount > 1 )
		{
			// POP Message AddToPartition 有兩個以上的行為設定.
			g_pError->AddMessage( 0, 0, "AddToPartition  GItemID(%d) LGItemID(%d) Obj 有兩個以上的行為設定", GItemID, LGItemID );
		}
		//-------------------------------------------------------------------------------
	}
}

// --------------------------------------------------------------------------------------
// 刪除物件
void CGameMain::DelFromPartition( int GItemID  )
{
	g_pError->AddMessage( 2, 0, "DelFromPartition GItemID(%d)", GItemID );
	CRoleSprite* pSprite = FindRoleSprite( GItemID );
	if ( pSprite )
		pSprite->SetFade( true, 2.0f );
	DeleteRoleSprite( GItemID );
}

// --------------------------------------------------------------------------------------
// 設定物件座標
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
// 物件移動
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
// 指定物件動做
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
// 設定移動速度
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