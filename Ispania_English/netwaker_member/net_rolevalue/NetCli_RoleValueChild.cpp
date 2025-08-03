#include "NetCli_RoleValueChild.h"
#include "../net_party/NetCli_PartyChild.h"
#include "../ErrorList/errorlist.h"
#include "../../mainproc/GameMain.h"
#include "../interface/debug/DebugMessage.h"
#include "../Interface/Login/AccountLogin.h"
#include "../Interface/WorldFrames/ChatFrame.h"
#include "../Interface/WorldFrames/ExchangeClassFrame.h"
#include "../Interface/WorldFrames/CharacterFrame.h"
#include "../Interface/WorldFrames/ScrollCombatTextFrame.h"
#include "../Interface/WorldFrames/ActionFrame.h"
#include "../Interface/WorldFrames/BagFrame.h"
#include "../Interface/ItemLink/ItemLink.h"
#include "../Interface/SkillBook/UI_SkillBook.h"
#include "../Interface/WorldFrames/PetFrame.h"
#include "../Interface/WorldFrames/SuitSkillFrame.h"
#include "../Interface/Socal/UI_Socal.h"
#include "../tools/Common/Com_Function.h"
#include "../Interface/WorldFrames/AncillaryTitleFrame.h"
#include <windows.h>
#include "psapi.h"
//-----------------------------------------------------------------------------
bool NetCli_RoleValueChild::_Init()
{
	if (This)
		return false;

	This = new NetCli_RoleValueChild;
	return NetCli_RoleValue::_Init();
}
//-----------------------------------------------------------------------------
bool NetCli_RoleValueChild::_Release()
{
	NetCli_RoleValue::_Release();
	if (This)
	{
		delete This;
		This = NULL;
	}
	return true;
}
//-----------------------------------------------------------------------------
void NetCli_RoleValueChild::R_FixAllRoleValue   ( BaseAbilityStruct& Ability )
{

}
//-----------------------------------------------------------------------------
void NetCli_RoleValueChild::R_DeltaRoleValue      ( RoleValueName_ENUM ValueName , double value )
{
	float Value = float(value);
	CRoleSprite* player = g_pGameMain->GetPlayer();
	if ( player == NULL )
		return;

	SCTInfo info;		
	info.source = player->GetIdentify();
	info.target = player->GetIdentify();

	switch(ValueName)
	{
	case EM_RoleValue_EXP:
		if ( Value > 0.0f )
		{
			info.type = eSCT_Increase_EXP;
			info.value = (int)Value;
			g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM_VALUE, "", g_ObjectData->GetString("SYS_INCREASE_EXP"), info.value);
		}
		else
		{
			info.type = eSCT_Decrease_EXP;
			info.value = (int)-Value;
			g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM_VALUE, "", g_ObjectData->GetString("SYS_DECREASE_EXP"), info.value);
		}		
		g_pScrollCombatTextFrame->PushSCTInfo(info);		
		break;

	case EM_RoleValue_TpExp:
		if ( Value > 0.0f )
		{
			info.type = eSCT_Increase_TP;
			info.value = (int)Value;
			g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM_VALUE, "", g_ObjectData->GetString("SYS_INCREASE_TP"), info.value);
		}
		else
		{
			info.type = eSCT_Decrease_TP;
			info.value = (int)-Value;
			g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM_VALUE, "", g_ObjectData->GetString("SYS_DECREASE_TP"), info.value);
		}		
		g_pScrollCombatTextFrame->PushSCTInfo(info);
		break;

	case EM_RoleValue_DebtExp:
		if ( Value < 0.0f )			
			g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM_VALUE, "", g_ObjectData->GetString("SYS_DECREASE_DEBT_EXP"), (int)-Value);
		else
			g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM_VALUE, "", g_ObjectData->GetString("SYS_INCREASE_DEBT_EXP"), (int)Value);
		break;

	case EM_RoleValue_DebtTP:
		if ( Value < 0.0f )
			g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM_VALUE, "", g_ObjectData->GetString("SYS_DECREASE_DEBT_TP"), (int)-Value);
		else
			g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM_VALUE, "", g_ObjectData->GetString("SYS_INCREASE_DEBT_TP"), (int)Value);
		break;
	case EM_RoleValue_Honor:
		{
			if ( Value > 0.0f )
			{
				if( Value >= 1.0f )
				{
					info.type = eSCT_Increase_Honor;
					info.value = (int)Value;
					g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM_VALUE, "", g_ObjectData->GetString("SYS_INCREASE_HONOR"), info.value);
				}
			}
			else
			{
				if( Value <= -1.0f )
				{
					info.type = eSCT_Decrease_Honor;
					info.value = (int)-Value;
					g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM_VALUE, "", g_ObjectData->GetString("SYS_DECREASE_HONOR"), info.value);
				}
			}
			g_pScrollCombatTextFrame->PushSCTInfo(info);		
		} break;
	}
}
//-----------------------------------------------------------------------------
void NetCli_RoleValueChild::R_FixRoleValue      ( RoleValueName_ENUM ValueName , double Value )
{
	if( unsigned( ValueName - EM_RoleValue_Coldown ) < DEF_MAX_COLDOWN_COUNT_ALL )
	{
		Value *= 10;
	}
	
	//RoleDataEx*    Role    = g_pGameMain->RoleData();	
	RoleDataEx*    Role    = CNetGlobal::RoleData();
	double         OrgValue = Role->GetValue( ValueName );


	{
		int ID = ValueName - EM_RoleValue_FamilyFriendListLv_Pos;
		if( ID >= 0 && ID < _MAX_FRIENDLIST_FAMILY_COUNT_  )
		{
			int oldLv = (int)Role->PlayerSelfData->FriendList.FamilyList[ID].Lv; 
			int newLv = int(Value);

			if( newLv > oldLv )
			{
				//g_pGameMain->SendSystemChatMsg( msg.c_str() );

				string RelationString = g_pUi_Socal->GetRelationString( Role->PlayerSelfData->FriendList.FamilyList[ID].Relation );
				string msg = "SYS_RELATION_LV_UP_" + RelationString;

				//轉大寫
				for ( int i = 0; i < msg.length(); ++i )
					msg[i] = ::toupper(msg[i]);

				msg = g_ObjectData->GetString( msg.c_str() );
				msg = ReplaceKeyString( msg, "<name>", Role->PlayerSelfData->FriendList.FamilyList[ID].Name );
				msg = ReplaceKeyString( msg, "<lv>", newLv );
				g_pGameMain->SendSystemChatMsg( msg.c_str() );
			}
		}
	}

	Role->SetValue( ValueName , Value );

	{
		int ID = ValueName - EM_RoleValue_FamilyFriendListLv_Pos;
		if( ID >= 0 && ID < _MAX_FRIENDLIST_FAMILY_COUNT_  )
		{
			g_pInterface->SendWorldEvent( UiEvent_ResetFriend );			
			return;
		}
	}


	float        DValue = float(Value - OrgValue);

	// 寵物事件例外
	if ( ValueName < EM_RoleValue_CultivatePet_Value && ValueName >= EM_RoleValue_LuaValueFlag && DValue == 0 ) 
		return;

	// 寵物事件
	if ( ValueName >= EM_RoleValue_CultivatePet_Value && ValueName < EM_RoleValue_LuaValueFlag ) 
	{
		int index = (ValueName - EM_RoleValue_CultivatePet_Value) / 100;
		int eventType = (ValueName - EM_RoleValue_CultivatePet_Value) % 100;

		switch ( eventType )
		{
		case EM_CultivatePetStructValueType_EventColdown:		//事件冷卻
			switch ( Role->PlayerBaseData->Pet.Base[index].EventType )
			{
			case EM_CultivatePetCommandType_None:			//沒狀態(停止生活技能)
			case EM_CultivatePetCommandType_OnTimeEvent:	//client 端每10秒通知Server 看那隻寵物(處理說話or特別事件)
			case EM_CultivatePetCommandType_Summon:			//召喚寵物
			case EM_CultivatePetCommandType_Feed:			//餵食
			case EM_CultivatePetCommandType_Travel:			//旅行
			case EM_CultivatePetCommandType_Free:			//放生
			case EM_CultivatePetCommandType_Train:			//特訓					
			case EM_CultivatePetCommandType_Return:			//召喚出的寵物回收
				break;

			case EM_CultivatePetCommandType_LiftSkill:		//生活技能
				// 開始製造
				if ( Value > 0.0 )
				{
					g_pPetFrame->CraftingStart(index, (float)Value);
				}
				// 正常結束
				else
				{
					g_pPetFrame->CraftingStop(index);
				}
				break;
			}			
			break;

		case EM_CultivatePetStructValueType_EventType:			//事件類型
			{
				if( int(OrgValue ) == EM_CultivatePetCommandType_Summon )
					Role->TempData.UpdateInfo.Recalculate_All = true;
				
				/*
				CultivatePetStruct& pet = CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[index];
				if ( pet.EventType == EM_CultivatePetCommandType_None )
				{
					if ( g_pPetFrame->IsStartCrafting(index) )
					{
						g_pPetFrame->CraftingFailed(index);
					}
				}
				*/
			}
			break;


		default:
			g_pPetFrame->UpdateAbility(index, eventType);
			break;
		}
	}
	else if( ValueName >= EM_RoleValue_SPMagic && ValueName < EM_RoleValue_SPMagic + _MAX_SPSkill_COUNT_ )
	{
		Role->TempData.UpdateInfo.ReSetTalbe = true;
		int SkillPos = ValueName - EM_RoleValue_SPMagic;
		//g_pInterface->SendWorldEvent( UiEvent_SkillUpdate );
		CUI_SkillBook::SkillUpdate();

		//g_pDebugMessage->DebugOutput( 2, "學會技能 SPSkillID(%d) SPSkillPos(%d) LV(%d)" , Role->BaseData.Skill.SP[ SkillPos ].SkillID , SkillPos , (int)Value );        

//		g_pDebugMessage->DebugOutput( 2, "學會技能 SPSkillID(%d) SPSkillPos(%d) LV(%d)" , Role->BaseData.Skill.SP[ SkillPos ].SkillID , SkillPos , (int)Value );
				
	}
	else if ( unsigned( ValueName - EM_RoleValue_Coldown ) < DEF_MAX_COLDOWN_COUNT_ALL )
	{
		g_pActionFrame->UpdateCooldown(0, 0);
		g_pBagFrame->UpdateCooldown(0, 0);
		g_pCharacterFrame->UpdateCooldown(0, 0);
		g_pUi_SkillBook->UpdateCooldown(0, 0);
	}
	else if( ValueName >= EM_RoleValue_NormalMagic && ValueName < EM_RoleValue_NormalMagic + _MAX_NormalSkill_COUNT_ )
	{
		Role->TempData.UpdateInfo.ReSetTalbe = true;
		int MagicPos = ValueName - EM_RoleValue_NormalMagic;
		//g_pInterface->SendWorldEvent( UiEvent_SkillUpdate );
		CUI_SkillBook::SkillUpdate();
//		g_pDebugMessage->DebugOutput( 2, "學會技能 MagicID(%d) MagicPos(%d) LV(%d)" , Role->BaseData.Skill.MainJob[ MagicPos ].SkillID , MagicPos , (int)Value );        
	}
	else if( ValueName >= EM_RoleValue_Skill_Unarmed && ValueName < EM_RoleValue_Skill_Unarmed + _MAX_SKILLVALUE_TYPE )
	{
		g_pDebugMessage->DebugOutput( 2, "熟練度ID(%d)  Value=%f" , ValueName , Value );

		char szBuf[256];
		char szSkillName[64];
		bool showUpdate = true;

		switch( ValueName )
		{
		case EM_RoleValue_Skill_BlackSmith:	strcpy( szSkillName, g_ObjectData->GetString( "SYS_SKILLNAME_BLACKSMITH" ) ); break;
		case EM_RoleValue_Skill_Carpenter:	strcpy( szSkillName, g_ObjectData->GetString( "SYS_SKILLNAME_CARPENTER" ) ); break;
		case EM_RoleValue_Skill_MakeArmor:	strcpy( szSkillName, g_ObjectData->GetString( "SYS_SKILLNAME_MAKEARMOR" ) ); break;
		case EM_RoleValue_Skill_Tailor:		strcpy( szSkillName, g_ObjectData->GetString( "SYS_SKILLNAME_TAILOR" ) ); break;
		case EM_RoleValue_Skill_Cook:		strcpy( szSkillName, g_ObjectData->GetString( "SYS_SKILLNAME_COOK" ) ); break;
		case EM_RoleValue_Skill_Alchemy:	strcpy( szSkillName, g_ObjectData->GetString( "SYS_SKILLNAME_ALCHEMY" ) ); break;
		case EM_RoleValue_Skill_Mining:		strcpy( szSkillName, g_ObjectData->GetString( "SYS_SKILLNAME_MINING" ) ); break;
		case EM_RoleValue_Skill_Lumbering:	strcpy( szSkillName, g_ObjectData->GetString( "SYS_SKILLNAME_LUMBERING" ) ); break;
		case EM_RoleValue_Skill_Herblism:	strcpy( szSkillName, g_ObjectData->GetString( "SYS_SKILLNAME_HERBLISM" ) ); break;
		case EM_RoleValue_Skill_Fishing:	strcpy( szSkillName, g_ObjectData->GetString( "SYS_SKILLNAME_FISHING" ) ); break;
		case EM_RoleValue_Skill_Plant:		strcpy( szSkillName, g_ObjectData->GetString( "SYS_SKILLNAME_PLANT" ) ); break;
		default:
			char KeyStr[256];
			sprintf( KeyStr , "SYS_SKILLNAME_%d" , ValueName );
			strcpy( szSkillName, g_ObjectData->GetString( KeyStr ) );
			break;
		}
		
		int iSkillValue = (int)Role->GetValue_Int( ValueName );
		if( iSkillValue!= int( OrgValue ) )
		{			
			sprintf( szBuf, g_ObjectData->GetString("SYS_SKILLLIFE_TEXT"), szSkillName, iSkillValue );
			g_pChatFrame->SendMsgEvent( CHAT_MSG_SYSTEM, szBuf, "" );
			showUpdate = false;
		}

		// 通知武器熟練度改變
		if ( ValueName >= EM_RoleValue_Skill_Unarmed && ValueName <= EM_RoleValue_Skill_Define )
		{
			g_pInterface->SendWorldEvent("PLAYER_SKILLED_CHANGED");
		}

		//通知ui生活技能改變
		if( ValueName >= EM_RoleValue_Skill_BlackSmith && ValueName <= EM_RoleValue_Skill_Plant )
		{
			float skillOffset = (float)(Role->GetValue_Float(ValueName) - OrgValue);
			if ( skillOffset >= 0.0001f && showUpdate )
			{
				g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_SKILLLIFE_UP_TEXT"), szSkillName, skillOffset * 100.0f);
			}
		
			lua_State* L = g_pInterface->GetLuaState();
			if ( L ) 
			{
				int iType = ValueName - EM_RoleValue_Skill_BlackSmith + EM_SkillValueType_BlackSmith;
				const char* szCraftSkillID = g_ObjectData->GetSkillValueString( (SkillValueTypeENUM)iType );
				lua_pushstring(L, szCraftSkillID);
				lua_setglobal(L, "arg1");
				lua_pushnumber(L, skillOffset * 100.0f);
				lua_setglobal(L, "arg2");
			}
			
			g_pInterface->SendWorldEvent( UiEvent_LifeSkillChange );
		}
  	}
	else if ( ValueName >= EM_RoleValue_TimeFlag && ValueName < EM_RoleValue_TimeFlag + _MAX_PLAYTIME_FLAG_ )
	{
		switch ( ValueName - EM_RoleValue_TimeFlag )
		{
		case EM_TimeFlagEffect_HonorParty:			//榮譽組隊
			if ( Value < 0.0f )
				g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_HONOR_PARTY_OFF"));
			else
				g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_HONOR_PARTY_ON"));
			break;
		case EM_TimeFlagEffect_BankBag2:		//銀行背包 開放
		case EM_TimeFlagEffect_BankBag3:		//銀行背包 開放
		case EM_TimeFlagEffect_BankBag4:		//銀行背包 開放
		case EM_TimeFlagEffect_BankBag5:		//銀行背包 開放
 			g_pInterface->SendWorldEvent(UiEvent_BankUpdate);
 			break;
		case EM_TimeFlagEffect_BagPage3:			// 背包租用
		case EM_TimeFlagEffect_BagPage4:			// 背包租用
		case EM_TimeFlagEffect_BagPage5:			// 背包租用
		case EM_TimeFlagEffect_BagPage6:			// 背包租用
			g_pInterface->SendWorldEvent("PLAYER_BAG_CHANGED");
			break;
		case EM_TimeFlagEffect_PetPage3:		// 開啟寵物培育欄
		case EM_TimeFlagEffect_PetPage4:		// 開啟寵物培育欄
		case EM_TimeFlagEffect_PetPage5:		// 開啟寵物培育欄
		case EM_TimeFlagEffect_PetPage6:		// 開啟寵物培育欄
			g_pInterface->SendWorldEvent("PET_COUNT_CHANGE");
			break;
		}

	}
	else if ( ValueName >= EM_RoleValue_ClinetSetting && ValueName < EM_RoleValue_ClinetSetting + _MAX_CLIENT_SETTING_SIZE_ )
	{
		if ( ValueName == EM_RoleValue_ClinetSetting )
		{	
			if ( NetCli_PartyChild::GetPartyState() == ePartyState_Solo || NetCli_PartyChild::GetPartyState() == ePartyState_Receive )
			{
				switch ((int)Value)
				{
				case 0:
					g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_INSTANCE_LEVEL_EASY"));
					break;

				case 1:
					g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_INSTANCE_LEVEL_NORMAL"));
					break;

				case 2:
					g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_INSTANCE_LEVEL_HARD"));
					break;
				}
			}
		}
	}
	else if (ValueName>=EM_RoleValue_PetCard&&ValueName<(EM_RoleValue_PetCard+_MAX_PET_CARD_COUNT_))
	{
			g_pInterface->SendWorldEvent("PETBOOKFRAME_UPDATE");
	}
	else switch( ValueName )
	{
	case EM_RoleValue_StomachPoint:
		break;

	 case EM_RoleValue_LV:
		 if( DValue >= 1 )
		 {  
			 g_pDebugMessage->DebugOutput( 2, "等級上升為%d" , (int)Value );
		 }
		 else
		 {
			 g_pDebugMessage->DebugOutput( 2, "等級下降為%d" , (int)Value );
		 }
		 //g_pInterface->GetUiWorld()->SendEvent( UiEvent_SkillUpdate );
		 break;
	 case EM_RoleValue_DebtExp:
	 case EM_RoleValue_DebtTP:
		 g_pInterface->SendWorldEvent("PLAYER_EXP_CHANGED");
		 break;

	 case EM_RoleValue_EXP:
		 if( DValue > 0  )
		 {  
			 g_pDebugMessage->DebugOutput( 2, "得到經驗值 %d" , (int)DValue );
		 }
		 else
		 {
			 g_pDebugMessage->DebugOutput( 2, "失去經驗值 %d" , (int)DValue*-1 );
		 }
		 break;
	 case EM_RoleValue_Point:
		 g_pInterface->SendWorldEvent("PLAYER_ABILITYPOINT_CHANGED");
		 break;

	 case EM_RoleValue_SkillPoint:
		 //g_pInterface->SendWorldEvent( UiEvent_SkillUpdate );
		 CUI_SkillBook::SkillUpdate();
		 g_pDebugMessage->DebugOutput( 2, "目前可配 法術 點數 %d" , (int)Value );
		 break;

	 case EM_RoleValue_TpExp:
		 {
			 g_pInterface->SendWorldEvent( UiEvent_TpExpUpdate );
		 } 
		 break;

	 case EM_RoleValue_VOC:
	 case EM_RoleValue_VOC_SUB:
		 {
			 CRoleSprite* sprite = g_pGameMain->GetPlayer();
			 if (sprite)			 
			 {
				 // 
				 //if ( sprite->GetVocation() != Role->BaseData.Voc ) //主職
				 //{	
					g_pSuitSkillFrame->Loadplateitems_EQ();   
					 //Role->BaseData.Voc;
				 //}

				 sprite->SetVocation(Role->BaseData.Voc, Role->BaseData.Voc_Sub);				 
				 
				 //sprite->SetLevel(Role->Level());
				 //g_pInterface->GetUiWorld()->SendEvent( UiEvent_SkillUpdate );
			 }
		 }
	 case EM_RoleValue_BankCount:
		 g_pInterface->SendWorldEvent("BANK_CAPACITY_CHANGED");
		 break;

	 case EM_RoleValue_PlayTimeQuota:
		 g_pInterface->SendWorldEvent("UPDATE_PLAYTIMEQUOTA");
		 //g_pGameMain->SendSystemMsg(g_ObjectData->GetString("SYS_PLAYTIMEQUOTA_TYPE1"));
		 break;

	 case EM_RoleValue_AddExp:			//額外增加經驗值
	 case EM_RoleValue_AddTP:			//額外增加tp
		 g_pInterface->SendWorldEvent("PLAYER_EXP_CHANGED");
		 break;

	 case EM_RoleValue_GoodEvil:		//善惡值
		 g_pInterface->SendWorldEvent("PLAYER_GOODEVIL_CHANGED");
		 break;

	 case EM_RoleValue_BoxEnergy:		//融合箱能量
		 g_pInterface->SendWorldEvent("PLAYER_BOXENERGY_CHANGED");
		 break;

		
	
	 case EM_RoleValue_MedalCount:		// 榮譽點數 ( 舊的老帶新 )
		 g_pInterface->SendWorldEvent("PLAYER_HONORPOINT_CHANGED");
		 break;		
			
	 case EM_RoleValue_RelicsCoin://			,	//古代遺物
	 case EM_RoleValue_DreamlandCoin://		,	//夢境先遣印記
	 case EM_RoleValue_PhiriusShellCoin://	,	//必爾汀圓貝
	 case EM_RoleValue_EnergyJusticeCoin:	//正義能量
	 case EM_RoleValue_ProofLegendCoin	:	//傳說之證
	 case EM_RoleValue_DuelistReward:	// 勇士勳章
	 case EM_RoleValue_TrialBadgeCoin://		,	//試煉徽章
	 case EM_RoleValue_Honor:			// 殺人榮譽
		 g_pInterface->SendWorldEvent("PLAYER_HONOR_CHANGED");
		 break;		 
	 case EM_RoleValue_SuitSkill_MaxEqSkillCount:
	     g_pInterface->SendWorldEvent("PLAYER_MAXEQSKILLCOUNT_CHANGED");
		 break;	
	 case EM_RoleValue_IncursivePoint:
		 g_pInterface->SendWorldEvent("WBG_SCORE_UPDATE");
		 break;	
	 case EM_RoleValue_TitleSys_Famous:			//頭銜系統名聲
	 case EM_RoleValue_TitleSys_Lv:				//頭銜系統等級
	 case EM_RoleValue_TitleSys_Exp:			//頭銜系統經驗值		
		 g_pAncillaryTitleFrame->UpdateATF_TitleFrame();
		 break;
	}

}
//-----------------------------------------------------------------------------
void NetCli_RoleValueChild::R_SetPoint_OK       ( )
{

}
//-----------------------------------------------------------------------------
void NetCli_RoleValueChild::R_SetPoint_Failed   ( )
{

}
//-----------------------------------------------------------------------------

void NetCli_RoleValueChild::R_NotifyLvUp        ( int ItemID , int Level )
{
	RoleDataEx*    Role    = CNetGlobal::RoleData();
	g_pDebugMessage->DebugOutput( 2, "GItemID(%d)  等級上升為%d" , ItemID , Level );
	CRoleSprite* sprite = g_pGameMain->FindSpriteByID( ItemID );
	if ( sprite )
	{
		sprite->SetLevel(Level, sprite->GetLevelSub());
		CEffectSprite* pEffect = new CEffectSprite;
		if ( pEffect->LoadRuEntity("model\\fx\\level_up\\act_level_up.ros") )
		{
			pEffect->AttachSprite(sprite, eAttach_Down);
			pEffect->SetEffectMotionStep(eEffectMotion_Step_Explode);
		}
		else
			delete pEffect;

		if( sprite == g_pGameMain->GetPlayer() )
		{			
			Role->TempData.Attr.Ability->EXP = 0;

			//char szBuf[256];
			//char szBuf2[256];
			GameObjDbStructEx* LearnMagic = g_ObjectData->GetObj( RoleDataEx::GlobalSetting.LearnMagicID + Role->BaseData.Voc );

			//計算角色等級
			RoleValueData	TempAttr;
			TempAttr.FixArgTableID = 1;
			TempAttr.Race		= Role->BaseData.Race;
			TempAttr.Sex		= Role->BaseData.Sex; 
			TempAttr.Voc   		= Role->BaseData.Voc;
			TempAttr.Voc_Sub	= EM_Vocation_None;
			TempAttr.Level		= Level - 1;
			TempAttr.ReSetTable();
			

			int	OrgStr = int(TempAttr.Bas.BaseStr);
			int	OrgSta = int(TempAttr.Bas.BaseSta);
			int	OrgAgi = int(TempAttr.Bas.BaseAgi);
			int	OrgInt = int(TempAttr.Bas.BaseInt);
			int	OrgMnd = int(TempAttr.Bas.BaseMnd);

			TempAttr.Level		= Level;
			TempAttr.ReSetTable();
			//|cffffff00

			g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM_VALUE, "", g_ObjectData->GetString( "LEVELUP_MESSAGE" ), Level);
			g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM_VALUE, "", g_ObjectData->GetString( "LEVELUP_MESSAGE_STR" ), int( TempAttr.Bas.BaseStr - OrgStr ));
			g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM_VALUE, "", g_ObjectData->GetString( "LEVELUP_MESSAGE_AGI" ), int( TempAttr.Bas.BaseAgi - OrgAgi ));
			g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM_VALUE, "", g_ObjectData->GetString( "LEVELUP_MESSAGE_STA" ), int( TempAttr.Bas.BaseSta - OrgSta ));
			g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM_VALUE, "", g_ObjectData->GetString( "LEVELUP_MESSAGE_INT" ), int( TempAttr.Bas.BaseInt - OrgInt ));
			g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM_VALUE, "", g_ObjectData->GetString( "LEVELUP_MESSAGE_MND" ), int( TempAttr.Bas.BaseMnd - OrgMnd ));
//			g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM_VALUE, "", g_ObjectData->GetString( "LEVELUP_MESSAGE_GETPOINT" ), 1);

			for( int i = 0 ; i < LearnMagic->LearnMagic.NormalSkillCount ; i++ )
			{
				JobSkillBaseStruct& SkillInfo = LearnMagic->LearnMagic.NormalSkill[i];
				if( SkillInfo.LearnLV == Level )
				{
					if(		SkillInfo.KeyItemID == 0
						||	Role->BaseData.KeyItem.GetFlag( SkillInfo.KeyItemID - Def_ObjectClass_KeyItem) == true )
					{
						GameObjDbStructEx* NewSkill = g_ObjectData->GetObj( SkillInfo.MagicID );
						if( NewSkill == NULL )
							continue;


						int skillpoint = (int)CNetGlobal::RoleData()->GetSkillPoint( NewSkill->GUID );

						string link;
						if( false == SkillGUID_To_HyperLink( NewSkill->GUID, skillpoint, link ) )
						{
							link = NewSkill->GetName();
						}
						g_pChatFrame->SendChatMessageEx( CHAT_MSG_SYSTEM_VALUE, "", g_ObjectData->GetString("LEVELUP_MESSAGE_SKILL"), link.c_str() );
					}
				}
			}

			for( int i = 0 ; i < LearnMagic->LearnMagic.SPSkillCount ; i++ )
			{
				JobSkillBaseStruct& SkillInfo = LearnMagic->LearnMagic.SPSkill[i];
				if( SkillInfo.LearnLV == Level )
				{
					if(		SkillInfo.KeyItemID == 0
						||	Role->BaseData.KeyItem.GetFlag( SkillInfo.KeyItemID - Def_ObjectClass_KeyItem) == true )
					{
						GameObjDbStructEx* NewSkill = g_ObjectData->GetObj( SkillInfo.MagicID );
						if( NewSkill == NULL )
							continue;

						int skillpoint = (int)CNetGlobal::RoleData()->GetSkillPoint( NewSkill->GUID );

						string link;
						if( false == SkillGUID_To_HyperLink( NewSkill->GUID, skillpoint, link ) )
						{
							link = NewSkill->GetName();
						}
						g_pChatFrame->SendChatMessageEx( CHAT_MSG_SYSTEM_VALUE, "", g_ObjectData->GetString("LEVELUP_MESSAGE_SKILL"), link.c_str() );
					}
				}
			}

			g_pGameMain->SendSystemMsgEx(g_ObjectData->GetString("LEVELUP_MESSAGE_POINT"), Level);
			g_pInterface->SendWorldEvent("PLAYER_LEVEL_UP");
		}
	}	
}

//-----------------------------------------------------------------------------
void NetCli_RoleValueChild::R_SetTitleFlag      ( int TitleID , bool Flag )
{
	RoleDataEx* Role = CNetGlobal::RoleData();

	if( Flag )
	{
		Role->BaseData.TitleID = TitleID;
		Role->TempData.UpdateInfo.Recalculate_All = true;
		Role->PlayerBaseData->Title.SetFlagOn( TitleID );

		GameObjDbStructEx* pTitleObj = g_ObjectData->GetObj( TitleID + Def_ObjectClass_Title );

 		//稱號取得	無	無	無	無	畫面中間訊息 "取得稱號:%s
		//稱號取得	無	無	無	無	系統頻道	 "取得稱號:%s
		//char* msg = Com_FormatString( "%s%s" , g_ObjectData->GetString( "SYS_GET_TITLE" ), pTitleObj->Name );
		char msg[256];
		sprintf( msg , g_ObjectData->GetString( "SYS_GET_TITLE" ), pTitleObj->GetName() );
		g_pGameMain->SendSystemMsg( msg );
		g_pGameMain->SendSystemChatMsg( msg );
   		
	}
	else
	{
		if( Role->BaseData.TitleID == TitleID )
		{
			Role->BaseData.TitleID = 0;
			Role->TempData.UpdateInfo.Recalculate_All = true;
		}

		Role->PlayerBaseData->Title.SetFlagOff( TitleID );
	}

	g_pInterface->SendWorldEvent( UiEvent_PlayerTitleFlagChanged );
}
//-----------------------------------------------------------------------------
void NetCli_RoleValueChild::R_SetFlag           ( int FlagID , bool Flag )
{

}

//-----------------------------------------------------------------------------
void NetCli_RoleValueChild::R_SetTitle_OK       ( int TitleID )
{
	RoleDataEx* Role = CNetGlobal::RoleData();
	Role->BaseData.TitleID = TitleID;
	Role->TempData.UpdateInfo.Recalculate_All = true;
	g_pCharacterFrame->R_SetTitle_OK( TitleID );
}

//-----------------------------------------------------------------------------
void NetCli_RoleValueChild::R_SetTitle_Failed   ( )
{
	g_pCharacterFrame->R_SetTitle_Failed();
}
//-----------------------------------------------------------------------------
void NetCli_RoleValueChild::R_RoleInfoChange( int ItemID  , int Level , int Level_Sub , Voc_ENUM Voc , Voc_ENUM Voc_Sub )
{
	CRoleSprite* sprite = g_pGameMain->FindSpriteByID( ItemID );
	if ( sprite )
	{
		//取得各職業等級 
		//RoleData()->PlayerBaseData->AbilityList[Voc]

		/*
		// 升級
		if (sprite->GetVocation() == Voc && sprite->GetVocationSub() == Voc_Sub && Level > sprite->GetLevel())
		{
			CEffectSprite* pEffect = new CEffectSprite;
			if ( pEffect->LoadRuEntity("model\\fx\\level_up\\act_level_up.ros") )
			{
				pEffect->SetAttachSprite(sprite, eAttach_Down);
				pEffect->SetEffectMotionStep(eEffectMotion_Step_Explode);
			}
			else
				delete pEffect;
		}
		*/
		
		sprite->SetLevel(Level, Level_Sub);
		sprite->SetVocation(Voc, Voc_Sub);
	}
}
//-----------------------------------------------------------------------------
void NetCli_RoleValueChild::R_OpenChangeJob( int TargetID )
{
	g_pExchangeClassFrame->Open(TargetID);
}
//-----------------------------------------------------------------------------
void NetCli_RoleValueChild::R_ChangeJobResult( ChangeJobResult_ENUM Result )
{	
	int job[2] = { 0, 0 };
	int level[2] = { 0, 0 };

	job[0] = g_pExchangeClassFrame->GetExchangeMainClass();
	job[1] = g_pExchangeClassFrame->GetExchangeSubClass();
	if ( job[0] > 0 )
		level[0] = RoleData()->PlayerBaseData->AbilityList[job[0]].Level;
	if ( job[1] > 0 )
		level[1] = RoleData()->PlayerBaseData->AbilityList[job[1]].Level;

	g_pExchangeClassFrame->Locked(false);
	switch (Result)
	{
	case EM_ChangeJobResult_OK:
		g_pExchangeClassFrame->SendWorldEvent("EXCHANGECLASS_SUCCESS");
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetSpecialString("SYS_EXCHANGE_CLASS_SUCCESS"));

		// message
		{			
			CRoleSprite* player = g_pGameMain->GetPlayer();
			if ( player ) {
				char buf[512];
				char temp[512];				

				strcpy(buf, g_ObjectData->GetString("SYS_EXCHANGE_CLASS_RESULT"));
				if ( level[0] > 0 )
				{
					sprintf(temp, "%s Lv%d", g_ObjectData->GetClassNameByID((Voc_ENUM)job[0]), level[0]);
					strcat(buf, temp);
				}
				if ( level[1] > 0 )
				{
					sprintf(temp, "%s Lv%d", g_ObjectData->GetClassNameByID((Voc_ENUM)job[1]), level[1]);
					strcat(buf, temp);
				}
				g_pGameMain->SendSystemMsg(buf);
			}
		}
		return;

	case EM_ChangeJobResult_Failed:
		break;
	}

	const char* msg = g_ObjectData->GetSpecialString("SYS_EXCHANGE_CLASS_ERR_1");
	g_pGameMain->SendWarningMsg(msg);
	g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, msg, "");
}

void NetCli_RoleValueChild::R_CloseChangeJob	 ( )
{
	//g_pError->AddMessage(0, 0, "R_CloseChangeJob");
	g_pExchangeClassFrame->SendWorldEvent("EXCHANGECLASS_CLOSED");
}

void NetCli_RoleValueChild::R_SetPlayerAllZoneState( int GItemID , PlayerStateStruct& State )
{
	g_pDebugMessage->DebugOutput( 2, "GItemID(%d)  全區域設定改變" , GItemID  );
	CRoleSprite* pSprite = g_pGameMain->FindSpriteByID( GItemID );

	if( pSprite )
	{
		pSprite->SetAFK( State.AFK );
		pSprite->SetFindParty( State.FindParty );
		pSprite->SetBotting( State.Dig );
	}
}
void NetCli_RoleValueChild::R_ClientLogoutResult	( int Second )
{
	CRoleSprite* player = g_pGameMain->GetPlayer();
	if ( player && !player->GetDeadState() ) {
		player->SetActorState(ruFUSION_ACTORSTATE_SIT_BEGIN);
	}

	lua_State* L = g_pInterface->GetLuaState();
	if ( L ) {
		lua_pushstring(L, "CAMP");
		lua_setglobal(L, "arg1");
		lua_pushnumber(L, Second);
		lua_setglobal(L, "arg2");
		lua_pushstring(L, "");
		lua_setglobal(L, "arg3");
		g_pInterface->SendWorldEvent("FORCE_DIALOG_SHOW");
	}	

	// 強制返迴角色前儲存資料
	if ( g_pGameMain )
	{
		g_pAccountLogin->SetReturnSelectCharacter(true);
		g_pGameMain->SaveClientSetting(true);
	}

	if ( g_pInterface )
	{
		g_pInterface->SaveClientData();
	}
}

void NetCli_RoleValueChild::R_CancelClientLogout	( )
{
	g_pAccountLogin->SetReturnSelectCharacter(false);
	g_pGameMain->SetCampState(false);
}

void NetCli_RoleValueChild::R_WaitChangeParallel	( int Second , bool Result )
{
	// 失敗
	if ( Result == false ) {
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_PARALLEL_FAILED"));
		g_pGameMain->ClearChangeParalleID();
	} else {
		CRoleSprite* player = g_pGameMain->GetPlayer();
		if ( player && !player->GetDeadState() ) {
			g_pGameMain->ToggleAutoRune(2);
			player->SetActorState(ruFUSION_ACTORSTATE_SIT_BEGIN);
		}

		lua_State* L = g_pInterface->GetLuaState();
		if ( L ) {
			lua_pushstring(L, "PARALLEL");
			lua_setglobal(L, "arg1");
			lua_pushnumber(L, Second + 5);
			lua_setglobal(L, "arg2");
			lua_pushnumber(L, g_pGameMain->GetChangeParallelID() + 1);
			lua_setglobal(L, "arg3");
			g_pInterface->SendWorldEvent("FORCE_DIALOG_SHOW");
		}
	}
}

void NetCli_RoleValueChild::R_ClientComputerInfoRequest( )
{
	ClientComputerInfoStruct info;
	MEMORYSTATUSEX					m_ProcessMemory;
	m_ProcessMemory.dwLength = sizeof(m_ProcessMemory);
	GlobalMemoryStatusEx(&m_ProcessMemory);
	PROCESS_MEMORY_COUNTERS cos;
	::GetProcessMemoryInfo(GetCurrentProcess(), &cos, sizeof(cos));

	int count = g_pGameMain->GetTotalCount();
	info.RAM_Free = (int)m_ProcessMemory.ullAvailPhys/1024/1024;//MB
	info.RAM_Total = (int)m_ProcessMemory.ullTotalPhys/1024/1024;//MB
	info.RAM_Game = int(cos.WorkingSetSize/1024/1024);//MB
	info.CPU = g_pGameMain->GetCpuUsage();
	info.IPNum = g_pGameMain->GetServerIPNum();
	info.Ping = g_pGameMain->GetPing();
	info.FPS = (int)g_pGameMain->GetFramerate();
	info.ZoneID = g_pGameMain->GetZoneID();

	g_pGameMain->ClearTotalCount();

	S_ClientComputerInfo(info);
}

void NetCli_RoleValueChild::R_ObjMode( int ItemGUID , ObjectModeStruct& Mode )
{
	CRoleSprite* sprite = g_pGameMain->FindSpriteByID(ItemGUID);

	// 是否為寵物
	if ( g_pGameMain->GetPetGUID() == ItemGUID )
	{
		g_pGameMain->SetPetStrikeBackEnable(Mode.Strikback);
		g_pGameMain->SetPetMovable(Mode.Move);

		// 送出封包儲存設定
		NetCli_RoleValue::S_ClientData(CNetGlobal::RoleData()->PlayerBaseData->ClientData);
	}

	if ( sprite )
	{
		SpriteObjectType objectType = (SpriteObjectType)sprite->GetObjectType();

		// 秀出(不論秀出或隱藏, 標記/致能)
		sprite->SetVisible(Mode.Show);
		
		if ( objectType != eSpriteObject_Player )
		{
			sprite->SetCursorState(Mode.Mark);									// 可標記/點選(可以讓玩家點選)
			sprite->SetNameVisible(!Mode.HideName);								// 不能出現頭上字串
			sprite->SetNameVisibleForce(g_pGameMain->GetNPCTitleVisible());		// 系統設定顯示
		}

		// 會阻擋(阻擋其他的物件)
		sprite->EnableCollision(Mode.Obstruct);
		sprite->EnableCollisionBase(Mode.Obstruct);

		// 有重力(地心引力, 計算物件與地面的碰撞, 否則依照 Client 送來的座標, 不會依地面高度改變 y)
		sprite->SetGravityMode(Mode.Gravity);

		// 此物件是否要儲存
		// Mode.Save

		// 有阻力的
		sprite->SetHorizontal(Mode.Drag);
		// Mode.Drag		

		// 完全隱行
		// Mode.GMHide

		// 全域可視物件
		// Mode.IsAllZoneVisible

		// 禁止旋轉( 面向不會改變 ) 
		sprite->SetDisableRotate(Mode.DisableRotate);

		// 用GM命令讓某個NPC hide
		// Mode.GM_NpcHide

		// 可否移除(公會建築)
		// Mode.Remove

		// 參考主人的威力
		// Mode.RelyOwnerPower

		// 忽略npc殺死
		// Mode.IgnoreNpcKill

		// 頭像框
		sprite->SetTargetState(Mode.ShowRoleHead);

		// 敵方看不建此物件
		sprite->SetEnemyHidden(Mode.EnemyHide);

		// 不顯示血條
		sprite->SetBloodBarHidden(Mode.NotShowHPMP);

		// 可砍殺攻擊(開啟此旗標的物件可以被砍殺)
		sprite->SetObjectAttackState(Mode.Fight);		

		// 暫時關閉動作改變
		sprite->SetDisableAction(Mode.DisableAction);
	}
}
void NetCli_RoleValueChild::R_ObjRoleInfo( PlayerRoleData& FullRole )
{
	FullRole.SetLink();
	RoleDataEx* Role = (RoleDataEx*)&FullRole;
	Role->InitCal( );
}

void NetCli_RoleValueChild::R_GetTargetRoleValue	( int ItemID, RoleValueName_ENUM ValueName , double Value )
{

	

}