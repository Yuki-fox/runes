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

				//��j�g
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

	// �d���ƥ�ҥ~
	if ( ValueName < EM_RoleValue_CultivatePet_Value && ValueName >= EM_RoleValue_LuaValueFlag && DValue == 0 ) 
		return;

	// �d���ƥ�
	if ( ValueName >= EM_RoleValue_CultivatePet_Value && ValueName < EM_RoleValue_LuaValueFlag ) 
	{
		int index = (ValueName - EM_RoleValue_CultivatePet_Value) / 100;
		int eventType = (ValueName - EM_RoleValue_CultivatePet_Value) % 100;

		switch ( eventType )
		{
		case EM_CultivatePetStructValueType_EventColdown:		//�ƥ�N�o
			switch ( Role->PlayerBaseData->Pet.Base[index].EventType )
			{
			case EM_CultivatePetCommandType_None:			//�S���A(����ͬ��ޯ�)
			case EM_CultivatePetCommandType_OnTimeEvent:	//client �ݨC10��q��Server �ݨ����d��(�B�z����or�S�O�ƥ�)
			case EM_CultivatePetCommandType_Summon:			//�l���d��
			case EM_CultivatePetCommandType_Feed:			//����
			case EM_CultivatePetCommandType_Travel:			//�Ȧ�
			case EM_CultivatePetCommandType_Free:			//���
			case EM_CultivatePetCommandType_Train:			//�S�V					
			case EM_CultivatePetCommandType_Return:			//�l��X���d���^��
				break;

			case EM_CultivatePetCommandType_LiftSkill:		//�ͬ��ޯ�
				// �}�l�s�y
				if ( Value > 0.0 )
				{
					g_pPetFrame->CraftingStart(index, (float)Value);
				}
				// ���`����
				else
				{
					g_pPetFrame->CraftingStop(index);
				}
				break;
			}			
			break;

		case EM_CultivatePetStructValueType_EventType:			//�ƥ�����
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

		//g_pDebugMessage->DebugOutput( 2, "�Ƿ|�ޯ� SPSkillID(%d) SPSkillPos(%d) LV(%d)" , Role->BaseData.Skill.SP[ SkillPos ].SkillID , SkillPos , (int)Value );        

//		g_pDebugMessage->DebugOutput( 2, "�Ƿ|�ޯ� SPSkillID(%d) SPSkillPos(%d) LV(%d)" , Role->BaseData.Skill.SP[ SkillPos ].SkillID , SkillPos , (int)Value );
				
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
//		g_pDebugMessage->DebugOutput( 2, "�Ƿ|�ޯ� MagicID(%d) MagicPos(%d) LV(%d)" , Role->BaseData.Skill.MainJob[ MagicPos ].SkillID , MagicPos , (int)Value );        
	}
	else if( ValueName >= EM_RoleValue_Skill_Unarmed && ValueName < EM_RoleValue_Skill_Unarmed + _MAX_SKILLVALUE_TYPE )
	{
		g_pDebugMessage->DebugOutput( 2, "���m��ID(%d)  Value=%f" , ValueName , Value );

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

		// �q���Z�����m�ק���
		if ( ValueName >= EM_RoleValue_Skill_Unarmed && ValueName <= EM_RoleValue_Skill_Define )
		{
			g_pInterface->SendWorldEvent("PLAYER_SKILLED_CHANGED");
		}

		//�q��ui�ͬ��ޯ����
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
		case EM_TimeFlagEffect_HonorParty:			//�a�A�ն�
			if ( Value < 0.0f )
				g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_HONOR_PARTY_OFF"));
			else
				g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_HONOR_PARTY_ON"));
			break;
		case EM_TimeFlagEffect_BankBag2:		//�Ȧ�I�] �}��
		case EM_TimeFlagEffect_BankBag3:		//�Ȧ�I�] �}��
		case EM_TimeFlagEffect_BankBag4:		//�Ȧ�I�] �}��
		case EM_TimeFlagEffect_BankBag5:		//�Ȧ�I�] �}��
 			g_pInterface->SendWorldEvent(UiEvent_BankUpdate);
 			break;
		case EM_TimeFlagEffect_BagPage3:			// �I�]����
		case EM_TimeFlagEffect_BagPage4:			// �I�]����
		case EM_TimeFlagEffect_BagPage5:			// �I�]����
		case EM_TimeFlagEffect_BagPage6:			// �I�]����
			g_pInterface->SendWorldEvent("PLAYER_BAG_CHANGED");
			break;
		case EM_TimeFlagEffect_PetPage3:		// �}���d�����|��
		case EM_TimeFlagEffect_PetPage4:		// �}���d�����|��
		case EM_TimeFlagEffect_PetPage5:		// �}���d�����|��
		case EM_TimeFlagEffect_PetPage6:		// �}���d�����|��
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
			 g_pDebugMessage->DebugOutput( 2, "���ŤW�ɬ�%d" , (int)Value );
		 }
		 else
		 {
			 g_pDebugMessage->DebugOutput( 2, "���ŤU����%d" , (int)Value );
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
			 g_pDebugMessage->DebugOutput( 2, "�o��g��� %d" , (int)DValue );
		 }
		 else
		 {
			 g_pDebugMessage->DebugOutput( 2, "���h�g��� %d" , (int)DValue*-1 );
		 }
		 break;
	 case EM_RoleValue_Point:
		 g_pInterface->SendWorldEvent("PLAYER_ABILITYPOINT_CHANGED");
		 break;

	 case EM_RoleValue_SkillPoint:
		 //g_pInterface->SendWorldEvent( UiEvent_SkillUpdate );
		 CUI_SkillBook::SkillUpdate();
		 g_pDebugMessage->DebugOutput( 2, "�ثe�i�t �k�N �I�� %d" , (int)Value );
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
				 //if ( sprite->GetVocation() != Role->BaseData.Voc ) //�D¾
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

	 case EM_RoleValue_AddExp:			//�B�~�W�[�g���
	 case EM_RoleValue_AddTP:			//�B�~�W�[tp
		 g_pInterface->SendWorldEvent("PLAYER_EXP_CHANGED");
		 break;

	 case EM_RoleValue_GoodEvil:		//���c��
		 g_pInterface->SendWorldEvent("PLAYER_GOODEVIL_CHANGED");
		 break;

	 case EM_RoleValue_BoxEnergy:		//�ĦX�c��q
		 g_pInterface->SendWorldEvent("PLAYER_BOXENERGY_CHANGED");
		 break;

		
	
	 case EM_RoleValue_MedalCount:		// �a�A�I�� ( �ª��ѱa�s )
		 g_pInterface->SendWorldEvent("PLAYER_HONORPOINT_CHANGED");
		 break;		
			
	 case EM_RoleValue_RelicsCoin://			,	//�j�N��
	 case EM_RoleValue_DreamlandCoin://		,	//�ڹҥ����L�O
	 case EM_RoleValue_PhiriusShellCoin://	,	//�����Ŷꨩ
	 case EM_RoleValue_EnergyJusticeCoin:	//���q��q
	 case EM_RoleValue_ProofLegendCoin	:	//�ǻ�����
	 case EM_RoleValue_DuelistReward:	// �i�h����
	 case EM_RoleValue_TrialBadgeCoin://		,	//�շ�����
	 case EM_RoleValue_Honor:			// ���H�a�A
		 g_pInterface->SendWorldEvent("PLAYER_HONOR_CHANGED");
		 break;		 
	 case EM_RoleValue_SuitSkill_MaxEqSkillCount:
	     g_pInterface->SendWorldEvent("PLAYER_MAXEQSKILLCOUNT_CHANGED");
		 break;	
	 case EM_RoleValue_IncursivePoint:
		 g_pInterface->SendWorldEvent("WBG_SCORE_UPDATE");
		 break;	
	 case EM_RoleValue_TitleSys_Famous:			//�Y�Ψt�ΦW�n
	 case EM_RoleValue_TitleSys_Lv:				//�Y�Ψt�ε���
	 case EM_RoleValue_TitleSys_Exp:			//�Y�Ψt�θg���		
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
	g_pDebugMessage->DebugOutput( 2, "GItemID(%d)  ���ŤW�ɬ�%d" , ItemID , Level );
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

			//�p�⨤�ⵥ��
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

 		//�ٸ����o	�L	�L	�L	�L	�e�������T�� "���o�ٸ�:%s
		//�ٸ����o	�L	�L	�L	�L	�t���W�D	 "���o�ٸ�:%s
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
		//���o�U¾�~���� 
		//RoleData()->PlayerBaseData->AbilityList[Voc]

		/*
		// �ɯ�
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
	g_pDebugMessage->DebugOutput( 2, "GItemID(%d)  ���ϰ�]�w����" , GItemID  );
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

	// �j���j����e�x�s���
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
	// ����
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

	// �O�_���d��
	if ( g_pGameMain->GetPetGUID() == ItemGUID )
	{
		g_pGameMain->SetPetStrikeBackEnable(Mode.Strikback);
		g_pGameMain->SetPetMovable(Mode.Move);

		// �e�X�ʥ]�x�s�]�w
		NetCli_RoleValue::S_ClientData(CNetGlobal::RoleData()->PlayerBaseData->ClientData);
	}

	if ( sprite )
	{
		SpriteObjectType objectType = (SpriteObjectType)sprite->GetObjectType();

		// �q�X(���רq�X������, �аO/�P��)
		sprite->SetVisible(Mode.Show);
		
		if ( objectType != eSpriteObject_Player )
		{
			sprite->SetCursorState(Mode.Mark);									// �i�аO/�I��(�i�H�����a�I��)
			sprite->SetNameVisible(!Mode.HideName);								// ����X�{�Y�W�r��
			sprite->SetNameVisibleForce(g_pGameMain->GetNPCTitleVisible());		// �t�γ]�w���
		}

		// �|����(���ר�L������)
		sprite->EnableCollision(Mode.Obstruct);
		sprite->EnableCollisionBase(Mode.Obstruct);

		// �����O(�a�ߤޤO, �p�⪫��P�a�����I��, �_�h�̷� Client �e�Ӫ��y��, ���|�̦a�����ק��� y)
		sprite->SetGravityMode(Mode.Gravity);

		// ������O�_�n�x�s
		// Mode.Save

		// �����O��
		sprite->SetHorizontal(Mode.Drag);
		// Mode.Drag		

		// ��������
		// Mode.GMHide

		// ����i������
		// Mode.IsAllZoneVisible

		// �T�����( ���V���|���� ) 
		sprite->SetDisableRotate(Mode.DisableRotate);

		// ��GM�R�O���Y��NPC hide
		// Mode.GM_NpcHide

		// �i�_����(���|�ؿv)
		// Mode.Remove

		// �ѦҥD�H���¤O
		// Mode.RelyOwnerPower

		// ����npc����
		// Mode.IgnoreNpcKill

		// �Y����
		sprite->SetTargetState(Mode.ShowRoleHead);

		// �Ĥ�ݤ��ئ�����
		sprite->SetEnemyHidden(Mode.EnemyHide);

		// ����ܦ��
		sprite->SetBloodBarHidden(Mode.NotShowHPMP);

		// �i�������(�}�Ҧ��X�Ъ�����i�H�Q���)
		sprite->SetObjectAttackState(Mode.Fight);		

		// �Ȯ������ʧ@����
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