#include "../../mainproc/GameMain.h"
#include "NetCli_RecipeChild.h"
#include "../interface/debug/DebugMessage.h"
#include "../../Interface/WorldFrames/ChatFrame.h"
#include "../../Interface/RecipeStore/UI_RecipeStore.h"
#include "../../Interface/CraftFrame/UI_CraftFrame.h"
#include "../../Interface/WorldFrames/CastingBarFrame.h"
//-----------------------------------------------------------------------------
bool NetCli_RecipeChild::_Init()
{
	if (This)
		return false;

	This = new NetCli_RecipeChild;

	return NetCli_Recipe::_Init();
}

//-----------------------------------------------------------------------------
bool NetCli_RecipeChild::_Release()
{
	NetCli_Recipe::_Release();
	if (This)
	{
		delete This;
		This = NULL;
	}
	return true;

}

//-----------------------------------------------------------------------------
//¦X¦¨¦¨¥
void NetCli_RecipeChild::RL_LockResourceAndRefineResultOK( int RecipeID ,vector<ItemFieldStruct>& ItemList  )
{
	g_pDebugMessage->DebugOutput( 2 , "¦X¦¨¦¨¥" );
	g_pRuGlobalEventManager->PlaySoundByFile("sound\\interface\\make_success.mp3", false, 0.0f, ruSOUNDTYPE_INTERFACE);
	GameObjDbStructEx*	pRecipe			= NULL;
	pRecipe		= g_ObjectData->GetObj( RecipeID );
	if( pRecipe )
	{
		if (pRecipe->Recipe.Coldown>0)
		{
			RefineBuffStruct Cooldown;
			Cooldown. RecipeID=RecipeID;
			Cooldown. Coldown=pRecipe->Recipe.Coldown;
		
			RL_RefineCooldown( 	Cooldown );
		}


	}
		
	if( ItemList.size() == 0 )
	{
		g_pChatFrame->SendMsgEvent( CHAT_MSG_SYSTEM, g_ObjectData->GetString( "CRAFT_MSG_ERROR" ), "" );
	}
}
//¦X¦¨¥¢±Ñ
void NetCli_RecipeChild::RL_LockResourceAndRefineResultFailed( int RecipeID , vector<ItemFieldStruct>& ItemList  )
{
	g_pDebugMessage->DebugOutput( 2 , "¦X¦¨¥¢±Ñ" );
	g_pRuGlobalEventManager->PlaySoundByFile("sound\\interface\\make_fail.mp3", false, 0.0f, ruSOUNDTYPE_INTERFACE);

	char szMsg[256];
	GameObjDbStructEx*	pCraftItem		= NULL;
	GameObjDbStructEx*	pRecipe			= NULL;

	pRecipe		= g_ObjectData->GetObj( RecipeID );
	if( pRecipe )
	{
		pCraftItem =  g_ObjectData->GetObj( pRecipe->Recipe.Item1_Normal.OrgObjID );
		if( pCraftItem )
		{
			sprintf( szMsg, g_ObjectData->GetString( "CRAFT_MSG_FAILED" ), pCraftItem->GetName() );
			g_pChatFrame->SendMsgEvent( CHAT_MSG_SYSTEM, szMsg, "" );
		}
	}
	else
	{
		g_pChatFrame->SendMsgEvent( CHAT_MSG_SYSTEM, g_ObjectData->GetString( "CRAFT_MSG_ERROR" ), "" );
	}

}

void NetCli_RecipeChild::ShowRequestItemError()
{
	string sMsg = g_ObjectData->GetString( "CRAFT_STRING_NOREQITEM" );

	g_pGameMain->SendSystemMsg( sMsg.c_str() );
	g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", sMsg.c_str() );

	g_pCastingBarFrame->CastingFailed(eCastingBar_Crafting);

	g_pCraftFrame->m_iCraftItemNumber	= 0;
	g_pCraftFrame->m_iCraftItemTotal	= 0;
	g_pCraftFrame->m_iCraftItem			= 0;
	g_pCraftFrame->m_fTimeToCreate		= 0;

	// ¨¤¦â°Ê§@»P¯S®Ä
	CRoleSprite* player = g_pGameMain->GetPlayer();
	if ( player ) 
	{
		player->SetActorState(ruFUSION_ACTORSTATE_CRAFTING_END);
		player->EndCastingEffect();
	}

	g_pCraftFrame->StopCraftSound();

	if( g_pCraftFrame->m_iFromQueue == 0 )
		g_pCraftFrame->SendWorldEvent( "CRAFT_STOP_CREATE" );
	else
		g_pCraftFrame->SendWorldEvent( "CRAFTQUEUE_NEXT_CREATE" );
}

//¦X¦¨±ø¥ó¤£²Å¦X
void NetCli_RecipeChild::RL_LockResourceAndRefineResultError( int RecipeID , RefineResultENUM	Result )
{
	g_pDebugMessage->DebugOutput( 2 , "¦X¦¨±ø¥ó¦³°ÝÃD" );

	//char szMsg[256];
	GameObjDbStructEx*	pCraftItem		= NULL;
	GameObjDbStructEx*	pRecipe			= NULL;

	pRecipe		= g_ObjectData->GetObj( RecipeID );
	if( pRecipe )
	{
		pCraftItem =  g_ObjectData->GetObj( pRecipe->Recipe.Item1_Normal.OrgObjID );
		
		if( pCraftItem )
		{
			//sprintf( szMsg, g_ObjectData->GetString( "CRAFT_MSG_FAILED" ), pCraftItem->GetName() );
			//g_pChatFrame->SendMsgEvent( CHAT_MSG_SYSTEM, szMsg, "" );

			if( Result == EM_RefineResult_ResourceError )
			{
				ShowRequestItemError();
			}
		}
	}
	else
	{
		g_pChatFrame->SendMsgEvent( CHAT_MSG_SYSTEM, g_ObjectData->GetString( "CRAFT_MSG_ERROR" ), "" );
	}
}

void NetCli_RecipeChild::RL_ProduceItemResult(PG_Recipe_LtoC_ProduceItemResult *item)
{
	// ÀË¬d¨î³y¥X HQ ª««~
	if( item->Item.Mode.IsHQ )
	{
		CRoleSprite* player = g_pGameMain->GetPlayer();
		if (player ) {
			CEffectSprite* pEffect = new CEffectSprite;
			if ( pEffect->LoadRuEntity(g_ObjectData->GetString("FX_NAME_CRAFT_HQ")) )
			{
				pEffect->AttachSprite(player, eAttach_Down);
				pEffect->SetEffectMotionStep(eEffectMotion_Step_Explode);
			}
			else
				delete pEffect;
		}

	} else {
		// ¤@¯ë¨î³y¦¨«~
		
	}
}
//-----------------------------------------------------------------------------
void NetCli_RecipeChild::RL_RecipeList( StaticFlag< DF_MAX_RECIPELIST >* pRecipeList )
{
	if (g_pRecipeStore)
	g_pRecipeStore->RecipeList(pRecipeList);
}
//-----------------------------------------------------------------------------
void NetCli_RecipeChild::RL_LearnRecipeResult( int iRecipeID, EM_LearnRecipeResult iResult )
{
	if (g_pRecipeStore)
		g_pRecipeStore->LearnRecipeResult(iRecipeID,iResult);
}
//-----------------------------------------------------------------------------
void NetCli_RecipeChild::RL_RefineCooldown( RefineBuffStruct	Cooldown )
{
	RoleDataEx*		pRole		= CNetGlobal::RoleData();

	if( pRole != NULL )
	{
		pRole->PlayerBaseData->RefineColdown.Push_Back( Cooldown );
	}
}
//-----------------------------------------------------------------------------