#include "../../mainproc/GameMain.h"
#include "NetCli_Gather_Child.h"

#include "../../Interface/WorldFrames/CastingBarFrame.h"
#include "../../Interface/WorldFrames/ChatFrame.h"

//#include "../../interface/WorldFrames/ChatFrame.h"
//#include "../../interface/quest/UI_QuestList.h"
//#include "../../Interface//WorldFrames/CastingFrame.h"

//#include "../interfacemember/Quest/UI_QuestList.h"
//#include "../interfacemember/Quest/UI_QuestDialog.h"
//#include "../interfacemember/Quest/UI_QuestNPCList.h"
//#include "../interfacemember/UI_CastingTimeBar.h"

//#include "../interfacemember/WorldFrame.h"
//#include "../../MainProc/ObjectDataClass.h"

//using namespace ui;

//CRoleSprite*	CNetCli_Script_Child::m_pLastTouchObj = NULL;


#define		DF_GATHER_TOOLID_MINE	210508
#define		DF_GATHER_TOOLID_LUMER	210509
#define		DF_GATHER_TOOLID_HERB	210510

bool		CNetCli_Gather_Child::m_IsWorking			= false;
float		CNetCli_Gather_Child::m_fGatherRestTimer	= 0;
int			CNetCli_Gather_Child::m_iGatherObj			= 0;

INT64 CNetCli_Gather_Child::m_iGatherSound		= 0;
bool		CNetCli_Gather_Child::m_bAutoGather			= false;

// --------------------------------------------------------------------------------------
bool CNetCli_Gather_Child::Init()
{
	if( m_pThis == NULL )
		m_pThis = new CNetCli_Gather_Child;

	CNetCli_Gather::Init();
	//CNetGlobal::Schedular()->Push( OnTimeUpdate , 500 , NULL , NULL );

	return true;
}
// --------------------------------------------------------------------------------------
bool CNetCli_Gather_Child::Release()
{
	CNetCli_Gather::Release();

	SAFE_DELETE( m_pThis );

	return true;
}


//--------------------------------------------------------------------------------------
//void CNetCli_Script_Child::OnTouchQuestNPC ( CRoleSprite* pRoleObj )
void CNetCli_Gather_Child::OnTouchGatherObj	( int iObjID )
{

	RoleDataEx*		pRole	= CNetGlobal::RoleData();

	if( pRole->PlayerBaseData->PlayTimeQuota < 2*60*60 )
	{
		g_pGameMain->SendSystemMsg(g_ObjectData->GetString( "BTI_MSG_NOTHEALTH" ));
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString( "BTI_MSG_NOTHEALTH" ));
		return;
	}

	CRoleSprite*	pPlayer			= g_pGameMain->GetPlayer();
	CRoleSprite*	pObj			= g_pGameMain->FindSpriteByID( iObjID );
	CRoleSprite*	pLastGatherObj	= g_pGameMain->FindSpriteByID( m_iGatherObj );

	if( pPlayer == NULL )
		return;

	// 角色下馬
	g_pGameMain->Dismount();

	// 下面的 CODE 好像是沒用意義的, 以前持續採集時使用.
	if( pObj == NULL )
	{
		if( m_fGatherRestTimer	!= 0 )
		{
			switch( pPlayer->GetActorState() )
			{
			case ruFUSION_ACTORSTATE_MINING_LOOP:	g_pGameMain->GetPlayer()->SetActorState( ruFUSION_ACTORSTATE_MINING_END );	break; // 關掉連續採擊 
			case ruFUSION_ACTORSTATE_CRAFTING_LOOP: g_pGameMain->GetPlayer()->SetActorState( ruFUSION_ACTORSTATE_CRAFTING_END ); break; // 關掉連續採擊 
			}

			// 拔掉手上採集工具
				pPlayer->DetachCraftingTool();

			m_fGatherRestTimer	= 0;
			m_iGatherObj		= 0;
		}		
		return;
	}

	// 如果點選相同物件, 則跳掉
	if( m_iGatherObj != 0 && m_iGatherObj == iObjID )
	{
		return;
	}

	if( m_iGatherObj == 0 || pLastGatherObj == NULL )
	{
		m_iGatherObj = iObjID;
	}
	else
	{
		// 如果上個點選的物件超過 _MAX_ROLE_TOUCH_RANGE_ ( 40 ) 則重設物件. 有 BUG 存在
		if( pPlayer != NULL && pLastGatherObj != NULL )
		{
			CRuVector3			dir;
			float				magnitude;

			dir			= pPlayer->GetPos() - pLastGatherObj->GetPos();
			dir.m_y		= 0;
			magnitude	= dir.Magnitude();

			if( magnitude > _MAX_ROLE_TOUCH_RANGE_ )
			{
				m_iGatherObj = iObjID;
			}
		}
		else
			return;
	}

	

	ENUM_CHECK_GATHER	emResult	= (ENUM_CHECK_GATHER) CNetCli_Gather_Child::CheckGatherObj( pObj->GetObjectId() );
	GameObjDbStruct*	pObjDB		= g_ObjectData->GetObj( pObj->GetObjectId() );

	
	int					iSkillLV		= 0;
	char				szSkillName[256];
	char				szMsg[256];

	switch( pObjDB->Mine.Type )
	{
	case EM_GameObjectMineType_Mine: 
		{
			iSkillLV = (int)pRole->TempData.Attr.Fin.SkillValue.GetSkill( EM_SkillValueType_Mining );
			strcpy( szSkillName, g_ObjectData->GetString( "SYS_SKILLNAME_MINING" ) );
		} break;
	case EM_GameObjectMineType_Wood: 
		{
			iSkillLV = (int)pRole->TempData.Attr.Fin.SkillValue.GetSkill( EM_SkillValueType_Lumbering );
			strcpy( szSkillName, g_ObjectData->GetString( "SYS_SKILLNAME_LUMBERING" ) );
		} break;
	case EM_GameObjectMineType_Herb: 
		{
			iSkillLV = (int)pRole->TempData.Attr.Fin.SkillValue.GetSkill( EM_SkillValueType_Herblism );
			strcpy( szSkillName, g_ObjectData->GetString( "SYS_SKILLNAME_HERBLISM" ) );
		} break;
	case EM_GameObjectMineType_Fish: 
		{
			iSkillLV = (int)pRole->TempData.Attr.Fin.SkillValue.GetSkill( EM_SkillValueType_Fishing );
			strcpy( szSkillName, g_ObjectData->GetString( "SYS_SKILLNAME_FISHING" ) );
		} break;
	}

	
	if( emResult != EM_CHECK_GATHER_OK )
	{
		// 顯示錯誤
		switch( emResult )
		{
		case EM_CHECK_GATHER_NOSKILL:
			{
				sprintf( szMsg, g_ObjectData->GetString( "GATHER_MSG_NOSKILL" ), szSkillName );
				g_pGameMain->SendWarningMsg( szMsg );
				g_pChatFrame->SendMsgEvent( CHAT_MSG_SYSTEM, szMsg, "" );
			} break;
		case EM_CHECK_GATHER_SKILLTOOLOW:
			{				
				sprintf( szMsg, g_ObjectData->GetString( "GATHER_MSG_SKILLTOOLOW" ), szSkillName );
				g_pGameMain->SendWarningMsg( szMsg );
				g_pChatFrame->SendMsgEvent( CHAT_MSG_SYSTEM, szMsg, "" );
			} break;
		case EM_CHECK_GATHER_WRONGTOOL:		g_pGameMain->SendWarningMsg( "(暫)裝備錯誤的採集工具!" ); break;
		case EM_CHECK_GATHER_NOTOOL:		g_pGameMain->SendWarningMsg( "(暫)沒有裝備採集工具!" ); break;
		}

		m_iGatherObj = 0;

		return;
	}

	// 設定預設的對應採集工具
	//--------------------------------------------------------------------------------
		switch( pObjDB->Mine.Type )
		{
		case EM_GameObjectMineType_Mine: pPlayer->SetHandsObjectId( eRoleHands_Crafting, DF_GATHER_TOOLID_MINE	); break;
		case EM_GameObjectMineType_Wood: pPlayer->SetHandsObjectId( eRoleHands_Crafting, DF_GATHER_TOOLID_LUMER ); break;
		case EM_GameObjectMineType_Herb: pPlayer->SetHandsObjectId( eRoleHands_Crafting, DF_GATHER_TOOLID_HERB ); break;
		}

	ClickGatherObj( iObjID );
	


	m_fGatherRestTimer	= 0;

	// 播放指定動作
	/*
	switch( pObjDB->Mine.Type )
	{
	case EM_GameObjectMineType_Mine:
	case EM_GameObjectMineType_Wood:
		{
			g_pGameMain->GetPlayer()->SetActorState( ruFUSION_ACTORSTATE_MINING_BEGIN );
			g_pGameMain->GetPlayer()->AttachCraftingTool();	// 設定採集工具
		} break;
		
	case EM_GameObjectMineType_Herb:
		{
			g_pGameMain->GetPlayer()->SetActorState( ruFUSION_ACTORSTATE_CRAFTING_BEGIN );
			g_pGameMain->GetPlayer()->AttachCraftingTool();	// 設定採集工具
		} break;
	}
	*/



}
//--------------------------------------------------------------------------------------
void CNetCli_Gather_Child::OnClickGatherObjResult ( int iResult, int iItemID, int iItemVal )
{


	//g_pGameMain->GetPlayer()->SetActorState( ruFUSION_ACTORSTATE_MINING_END );
	//g_pGameMain->GetPlayer()->SetActorState( ruFUSION_ACTORSTATE_MINING_END );
	//g_pCastingBarFrame->CastingStop();

	m_bAutoGather			= false;
	int	 iLastGatherObjID	= m_iGatherObj;

	switch( iResult )
	{
	case EM_GATHER_BEGIN:
		return;
	case EM_GATHER_SUCCESSED:
		{
			g_pCastingBarFrame->CastingStop(eCastingBar_Crafting);
			PlayGatherSuccessSound( GetMineTypeByObjID( m_iGatherObj ) );
			m_bAutoGather = true;
		} break;

	case EM_GATHER_OBJBUSY:
		{
			g_pChatFrame->SendMsgEvent( CHAT_MSG_SYSTEM, g_ObjectData->GetString( "GATHER_MSG_OBJBUSY"), "" );
			g_pCastingBarFrame->CastingFailed(eCastingBar_Crafting);			
			PlayGatherFailedSound( GetMineTypeByObjID( m_iGatherObj ) );
		} break;

	case EM_GATHER_NULLOBJ:
		{
			g_pCastingBarFrame->CastingStop(eCastingBar_Crafting);
		} break;

	//case EM_GATHER_FAILED:
	//case EM_GATHER_GONE:
	//case EM_GATHER_LOWSKILL:
	//case EM_GATHER_NULLOBJ:
	//	{
	//		g_pCastingBarFrame->CastingFailedww
	//	} break;
	default:
		{
			g_pCastingBarFrame->CastingFailed(eCastingBar_Crafting);
			g_pChatFrame->SendMsgEvent( CHAT_MSG_SYSTEM, g_ObjectData->GetString( "GATHER_MSG_FAILED"), "" );
			PlayGatherFailedSound( GetMineTypeByObjID( m_iGatherObj ) );
		} break;

	}

	if( m_bAutoGather == false )
	{
		CRoleSprite* pPlayer = g_pGameMain->GetPlayer();

		if( pPlayer )
		{
			switch( pPlayer->GetActorState() )
			{
			case ruFUSION_ACTORSTATE_MINING_BEGIN:
			case ruFUSION_ACTORSTATE_MINING_LOOP:	
				pPlayer->SetActorState( ruFUSION_ACTORSTATE_MINING_END );	
				pPlayer->DetachCraftingTool();	// 拔掉手上採集工具
				break;
			case ruFUSION_ACTORSTATE_CRAFTING_BEGIN:
			case ruFUSION_ACTORSTATE_CRAFTING_LOOP:
				pPlayer->SetActorState( ruFUSION_ACTORSTATE_CRAFTING_END ); 
				g_pGameMain->GetPlayer()->DetachCraftingTool();	// 拔掉手上採集工具
				break; // 關掉連續採擊 
			case ruFUSION_ACTORSTATE_LUMBERJACK_BEGIN:
			case ruFUSION_ACTORSTATE_LUMBERJACK_LOOP:
				pPlayer->SetActorState( ruFUSION_ACTORSTATE_LUMBERJACK_END ); 
				g_pGameMain->GetPlayer()->DetachCraftingTool();	// 拔掉手上採集工具
				break; // 關掉連續採擊 
			case ruFUSION_ACTORSTATE_GATHER_BEGIN:
			case ruFUSION_ACTORSTATE_GATHER_LOOP:
				pPlayer->SetActorState( ruFUSION_ACTORSTATE_GATHER_END ); 
				g_pGameMain->GetPlayer()->DetachCraftingTool();	// 拔掉手上採集工具
				break;
			}
		}
	}

	// 重置設定
		m_IsWorking			= false;
		m_fGatherRestTimer	= 5;
		m_iGatherObj		= 0;


	if( m_bAutoGather == true )
	{
		OnTouchGatherObj( iLastGatherObjID );
	}

}
//--------------------------------------------------------------------------------------
int	CNetCli_Gather_Child::CheckGatherObj	( int iGatherObjID )
{
	// 檢查採集物件是否能被採擊
	GameObjDbStruct*	pObjDB			= g_ObjectData->GetObj( iGatherObjID );
	RoleDataEx*			pRole			= CNetGlobal::RoleData();
	
	int					iResult			= -1;

	int					iSkillLV		= 0;

	switch( pObjDB->Mine.Type )
	{
	case EM_GameObjectMineType_Mine: iSkillLV = (int)pRole->TempData.Attr.Fin.SkillValue.GetSkill( EM_SkillValueType_Mining ); break;
	case EM_GameObjectMineType_Wood: iSkillLV = (int)pRole->TempData.Attr.Fin.SkillValue.GetSkill( EM_SkillValueType_Lumbering ); break;
	case EM_GameObjectMineType_Herb: iSkillLV = (int)pRole->TempData.Attr.Fin.SkillValue.GetSkill( EM_SkillValueType_Herblism ); break;
	case EM_GameObjectMineType_Fish: iSkillLV = (int)pRole->TempData.Attr.Fin.SkillValue.GetSkill( EM_SkillValueType_Fishing ); break;
	}

	if( iSkillLV == 0 )
	{
		return EM_CHECK_GATHER_NOSKILL;
	}

	// 檢查技能夠不夠
		if( iSkillLV < pObjDB->Mine.LvLimit )
			return EM_CHECK_GATHER_SKILLTOOLOW;	// 技能不夠
 
	// 檢查是否裝備採集工具 2007_11_09 決定不用採集工具裝備
		/*
		GameObjDbStructEx*	pToolObjDB	= g_ObjectData->GetObj( pRole->BaseData.EQ.Manufacture.OrgObjID );

		if( pToolObjDB )
		{
			int iCheck = true;
			switch( pObjDB->Mine.Type )
			{
			case EM_GameObjectMineType_Mine: if( pToolObjDB->Item.WeaponType != EM_Weapon_Mining	) iCheck = false; break;
			case EM_GameObjectMineType_Wood: if( pToolObjDB->Item.WeaponType != EM_Weapon_Lumbering ) iCheck = false; break;
			case EM_GameObjectMineType_Herb: if( pToolObjDB->Item.WeaponType != EM_Weapon_Herbalism ) iCheck = false; break;
			case EM_GameObjectMineType_Fish: if( pToolObjDB->Item.WeaponType != EM_Weapon_Fishing	) iCheck = false; break;
			}

			if( iCheck == false )
				return EM_CHECK_GATHER_WRONGTOOL;
		}
		else
			return EM_CHECK_GATHER_NOTOOL; // 無裝備工具
		*/

	return EM_CHECK_GATHER_OK;
}
//--------------------------------------------------------------------------------------
void CNetCli_Gather_Child::Update( float elapsedTime )
{
	/*
	if( m_fGatherRestTimer != 0 )
	{
		m_fGatherRestTimer -= elapsedTime;
		CRuFusion_ActorState actplaying = g_pGameMain->GetPlayer()->GetActorState(1);
		g_pError->AddMessage( 2, 0, "ACT = %d", actplaying );		
		
		if( !( actplaying == ruFUSION_ACTORSTATE_MINING_END	||
			actplaying == ruFUSION_ACTORSTATE_MINING_LOOP	||
			actplaying == ruFUSION_ACTORSTATE_HOLDING		) )
		{
			m_fGatherRestTimer	= 0;	// 被中斷了
			m_iGatherObj		= 0;
			return;
		}

		if( m_fGatherRestTimer < 0 )
		{
			m_pThis->OnTouchGatherObj( m_iGatherObj );	// 再次發出採集
		}
	}
	*/

	if( m_iGatherObj != 0 && m_IsWorking == true )
	{
		bool bCancel = false;

		CRoleSprite* pPlayer = g_pGameMain->GetPlayer();

		if( pPlayer != NULL )
		{
			CRuFusion_ActorState actplaying = pPlayer->GetActorState(1);		
			switch( actplaying )
			{
			case ruFUSION_ACTORSTATE_CRAFTING_BEGIN:
			case ruFUSION_ACTORSTATE_CRAFTING_LOOP:
			case ruFUSION_ACTORSTATE_CRAFTING_END:
			case ruFUSION_ACTORSTATE_GATHER_BEGIN:
			case ruFUSION_ACTORSTATE_GATHER_LOOP:
			case ruFUSION_ACTORSTATE_GATHER_END:
			case ruFUSION_ACTORSTATE_LUMBERJACK_BEGIN:
			case ruFUSION_ACTORSTATE_LUMBERJACK_LOOP:
			case ruFUSION_ACTORSTATE_LUMBERJACK_END:
			case ruFUSION_ACTORSTATE_MINING_BEGIN:
			case ruFUSION_ACTORSTATE_MINING_LOOP:
			case ruFUSION_ACTORSTATE_MINING_END:
				bCancel = false;
				break;
			default:
				bCancel = true;
			}

			if( bCancel == true )
			{
				CancelGather( m_iGatherObj );
			}
		}
	}
}
//--------------------------------------------------------------------------------------
void CNetCli_Gather_Child::OnGatherMotion ( int iGatherPlayerID, int iMotionID, int iMineType )
{
	CRoleSprite* pObj		= g_pGameMain->FindSpriteByID( iGatherPlayerID );
	CRoleSprite* pPlayer	= g_pGameMain->GetPlayer();

	if( pObj == NULL  )
		return;

	//if( m_iGatherObj == 0 )	// 因為檔了這行會使其它物檢無法表演動作
	//	return;


	CRuFusion_ActorState actID = (CRuFusion_ActorState)iMotionID;

	if( m_bAutoGather == false )
	{

		switch( (GameObjectMineType) iMineType )
		{
		case EM_GameObjectMineType_Mine:
				pObj->SetHandsObjectId( eRoleHands_Crafting, DF_GATHER_TOOLID_MINE	);
			break;
		case EM_GameObjectMineType_Wood:
				pObj->SetHandsObjectId( eRoleHands_Crafting, DF_GATHER_TOOLID_LUMER );
			break;
		case EM_GameObjectMineType_Herb:
				pObj->SetHandsObjectId( eRoleHands_Crafting, DF_GATHER_TOOLID_HERB );
			break;
		}	
	}

	if( pObj )
	{
		switch( actID )
		{
		case ruFUSION_ACTORSTATE_GATHER_END:
		case ruFUSION_ACTORSTATE_LUMBERJACK_END:
		case ruFUSION_ACTORSTATE_CRAFTING_END:
		case ruFUSION_ACTORSTATE_MINING_END:
			{
				if( m_bAutoGather == false )
				{
					pObj->SetActorState( actID );	
					pObj->DetachCraftingTool();	// 拔掉手上採集工具
					StopGatherSound();
				}
				
				if( pObj == pPlayer )
				{
					m_IsWorking = false;
				}			
			} break;

		case ruFUSION_ACTORSTATE_CRAFTING_BEGIN:
		case ruFUSION_ACTORSTATE_MINING_BEGIN:
		case ruFUSION_ACTORSTATE_LUMBERJACK_BEGIN:
		case ruFUSION_ACTORSTATE_GATHER_BEGIN:		
			{
				if( m_bAutoGather == false )
				{
					pObj->SetActorState( actID );
					pObj->AttachCraftingTool();
					PlayGatherSound( (GameObjectMineType) iMineType );
				}				

				if( pObj == pPlayer )
				{
					m_IsWorking = true;

					CRoleSprite* pGatherObj = g_pGameMain->FindSpriteByID( m_iGatherObj );
					if( pGatherObj )
					{
						RoleDataEx* pRole			= CNetGlobal::RoleData();
						float		fGatherTime		= (float)_TIME_CLICKTOGETITEM;

						if( pRole->TempData.Attr.Mid.Body[ EM_WearEqType_GatherSpeedRate ] != 0 )
						{
							float fRate = pRole->TempData.Attr.Mid.Body[ EM_WearEqType_GatherSpeedRate ] / (float)100;
							fGatherTime = fGatherTime / ( 1 + fRate );
						}

						g_pCastingBarFrame->CastingStart( eCastingBar_Crafting, pGatherObj->GetName(), ( fGatherTime / 1000 ));
					}
				}

			} break;
		}
	}


}

void CNetCli_Gather_Child::PlayGatherSound( int iMineType )
{

}

void CNetCli_Gather_Child::StopGatherSound()
{
	if( m_iGatherSound != 0 )
	{
		//g_ruSymphony->StopSound(m_iGatherSound);
		g_ruAudible->Stop(m_iGatherSound, 0.250f);
		m_iGatherSound = 0;
	}

}

void CNetCli_Gather_Child::PlayGatherSuccessSound( int iMineType )
{
	StopGatherSound();

	switch( (GameObjectMineType) iMineType )
	{
	case EM_GameObjectMineType_Mine: 
		{
			//m_iGatherSound = g_ruSymphony->PlaySoundByPath( "Sound\\Interface\\Gather_Mine_Success.mp3", false );
			int loopCount = (FALSE) ? 0x7FFFFFFF : 1;
			m_iGatherSound = g_ruAudible->Play("Sound\\Interface\\Gather_Mine_Success.mp3", (CRuAudible::AudibleType)ruSOUNDTYPE_SFX, (loopCount > 1) ? TRUE : FALSE);				
		} break;
	case EM_GameObjectMineType_Wood: 
		{
			//m_iGatherSound = g_ruSymphony->PlaySoundByPath( "Sound\\Interface\\Gather_Wood_Success.mp3", false );
			int loopCount = (FALSE) ? 0x7FFFFFFF : 1;
			m_iGatherSound = g_ruAudible->Play("Sound\\Interface\\Gather_Wood_Success.mp3", (CRuAudible::AudibleType)ruSOUNDTYPE_SFX, (loopCount > 1) ? TRUE : FALSE);				
		} break;
	case EM_GameObjectMineType_Herb: 
		{
			//m_iGatherSound = g_ruSymphony->PlaySoundByPath( "Sound\\Interface\\Gather_Herb_Success.mp3", false );
			int loopCount = (FALSE) ? 0x7FFFFFFF : 1;
			m_iGatherSound = g_ruAudible->Play("Sound\\Interface\\Gather_Herb_Success.mp3", (CRuAudible::AudibleType)ruSOUNDTYPE_SFX, (loopCount > 1) ? TRUE : FALSE);				
		} break;
	}
}

void CNetCli_Gather_Child::PlayGatherFailedSound( int iMineType )
{
	StopGatherSound();

	switch( (GameObjectMineType) iMineType )
	{
	case EM_GameObjectMineType_Mine: 
		{
			//m_iGatherSound = g_ruSymphony->PlaySoundByPath( "Sound\\Interface\\Gather_Mine_Failed.mp3", false );
			int loopCount = (FALSE) ? 0x7FFFFFFF : 1;
			m_iGatherSound = g_ruAudible->Play("Sound\\Interface\\Gather_Mine_Failed.mp3", (CRuAudible::AudibleType)ruSOUNDTYPE_SFX, (loopCount > 1) ? TRUE : FALSE);				
		} break;
	case EM_GameObjectMineType_Wood: 
		{
			//m_iGatherSound = g_ruSymphony->PlaySoundByPath( "Sound\\Interface\\Gather_Wood_Failed.mp3", false );
			int loopCount = (FALSE) ? 0x7FFFFFFF : 1;
			m_iGatherSound = g_ruAudible->Play("Sound\\Interface\\Gather_Wood_Failed.mp3", (CRuAudible::AudibleType)ruSOUNDTYPE_SFX, (loopCount > 1) ? TRUE : FALSE);				
		} break;
	case EM_GameObjectMineType_Herb: 
		{
			//m_iGatherSound = g_ruSymphony->PlaySoundByPath( "Sound\\Interface\\Gather_Herb_Failed.mp3", false );
			int loopCount = (FALSE) ? 0x7FFFFFFF : 1;
			m_iGatherSound = g_ruAudible->Play("Sound\\Interface\\Gather_Herb_Failed.mp3", (CRuAudible::AudibleType)ruSOUNDTYPE_SFX, (loopCount > 1) ? TRUE : FALSE);				
		} break;
	}
}


int CNetCli_Gather_Child::GetMineTypeByObjID ( int iGatherObjID )
{
	GameObjDbStruct*	pObjDB		= g_ObjectData->GetObj( iGatherObjID );
	if( pObjDB )
		return pObjDB->Mine.Type;
	else
		return 0;
}


void CNetCli_Gather_Child::GetMinetooltips ( int iGatherObjID, string& stringtips, int& iMineType, int& iSkillNow, int& iReqSkill )
{
	// 需要採礦
	// 需要採礦(350)

	// 檢查採集物件是否能被採擊
	GameObjDbStruct*	pObjDB			= g_ObjectData->GetObj( iGatherObjID );
	RoleDataEx*			pRole			= CNetGlobal::RoleData();

	if( pObjDB == NULL )
		return;
	
	string				tips;

	//char				szBuff[256];

	int					iSkillLV		= 0;
		
	iMineType	= pObjDB->Mine.Type;
	
	switch( pObjDB->Mine.Type )
	{
	case EM_GameObjectMineType_Mine: 
		{
			iSkillLV	= (int)pRole->TempData.Attr.Fin.SkillValue.GetSkill( EM_SkillValueType_Mining );
			tips		= g_ObjectData->GetString( "SYS_GATHER_TIPS_MINE" );
		} break;
	case EM_GameObjectMineType_Wood: 
		{
			iSkillLV = (int)pRole->TempData.Attr.Fin.SkillValue.GetSkill( EM_SkillValueType_Lumbering ); 
			tips	 = g_ObjectData->GetString( "SYS_GATHER_TIPS_WOOD" );
		} break;
	case EM_GameObjectMineType_Herb:
		{
			iSkillLV = (int)pRole->TempData.Attr.Fin.SkillValue.GetSkill( EM_SkillValueType_Herblism ); 
			tips	 = g_ObjectData->GetString( "SYS_GATHER_TIPS_HERB" );
		} break;
	}

	iSkillNow	= iSkillLV;
	iReqSkill	= pObjDB->Mine.LvLimit;
	stringtips	= tips;
}