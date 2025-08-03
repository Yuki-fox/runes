#include "NetCli_CultivatePetChild.h"
#include "../../mainproc/GameMain.h"
#include "../Interface/WorldFrames/PetFrame.h"
#include "../Interface/WorldFrames/PartnerFrame.h"
#include "../Interface/WorldFrames/ChatFrame.h"

//-----------------------------------------------------------------------------
bool NetCli_CultivatePetChild::_Init()
{
	if (This)
		return false;

	This = new NetCli_CultivatePetChild;

	return NetCli_CultivatePet::_Init();
}

//-----------------------------------------------------------------------------
bool NetCli_CultivatePetChild::_Release()
{
	NetCli_CultivatePet::_Release();
	if (This)
	{
		delete This;
		This = NULL;
	}
	return true;
}

void NetCli_CultivatePetChild::RL_PushPetResultOK		( int PetPos , CultivatePetStruct& PetInfo )
{
	if ( PetPos >= 0 && PetPos < MAX_CultivatePet_Count )
	{
		CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[PetPos] = PetInfo;		

		// 更新技能列表
		g_pPetFrame->UpdateSkillInfo(PetPos);
	}

	g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_PET_PUSH_SUCCESS"));
	g_pPetFrame->SendWorldEvent("PET_PUSH_SUCCESS");
	g_pPetFrame->PushPetOK( PetPos );
}
void NetCli_CultivatePetChild::RL_PushPetResultFailed	( )
{
	g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_PET_PUSH_FAILED"));
}
void NetCli_CultivatePetChild::RL_PopPetResult			( bool Result , int ItemPos )
{
	if ( Result == false )
	{
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_PET_TAKEOUT_FAILED"));
	}
	else
	{
		int index = g_pPetFrame->GetTakeoutIndex();

		if ( index >= 0 && index < MAX_CultivatePet_Count )
		{
			CultivatePetStruct& pet = CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[index];

			// 清除資料
			pet.init();

			// 更新技能列表
			g_pPetFrame->UpdateSkillInfo(index);
		}

		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_PET_TAKEOUT_SUCCESS"));

		g_pPetFrame->SendWorldEvent("PET_TAKEOUT_SUCCESS");
	}

	// 解除物品鎖定
	g_pPetFrame->SetTakeoutIndex(-1);
}
void NetCli_CultivatePetChild::RL_PetNameResult			( bool Result , const char* Name )
{
	int index = g_pPetFrame->GetUpdateNameIndex();
	// 清除鎖定狀態
	g_pPetFrame->SetUpdateNameIndex(-1);
	if ( Result )
	{
		if ( index >= 0 && index < MAX_CultivatePet_Count )
		{
			CNetGlobal::RoleData()->PlayerBaseData->Pet.Base[index].Name = Name;
		}	
		g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_PET_NAME_SUCCESS"), Name);

		g_pPetFrame->SendWorldEvent("PET_NAME_UPDATE");
	}
	else
	{
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_PET_NAME_FAILED"));
		g_pPetFrame->SendWorldEvent("PET_NAME_FAILED");
	}
}
void NetCli_CultivatePetChild::RL_PetEventResult		( bool Result , int PetPos , CultivatePetCommandTypeENUM OrgEvent , CultivatePetCommandTypeENUM Event , int EventTime )
{
	RoleDataEx* roleData = CNetGlobal::RoleData();

	CultivatePetStruct&	PetBase	= roleData->PlayerBaseData->Pet.Base[ PetPos ];

	if( Result )
	{		
		PetBase.EventType = Event;
		PetBase.EventColdown = EventTime;
	}


	switch( Event )
	{
	case EM_CultivatePetCommandType_OnTimeEvent:	//client 端每10秒通知Server 看那隻寵物(處理說話or特別事件)
		break;
	case EM_CultivatePetCommandType_Summon:			//召喚寵物

		roleData->TempData.UpdateInfo.Recalculate_All = true;
		if ( Result )
		{
			g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_PET_SUMMON_SUCCEED"));
			lua_State* L = g_pInterface->GetLuaState();
			if ( L ) 
			{
				lua_pushnumber(L, PetPos + 1);
				lua_setglobal(L, "arg1");
			}
			g_pInterface->SendWorldEvent( "PET_SUMMON_SUCCEED" );
		}
		else
		{
			g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_PET_SUMMON_FAILED"));
		}
		break;

	case EM_CultivatePetCommandType_Feed:			//餵食
		if ( Result )
		{
			g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_PET_FEED_SUCCEED"));
			g_pPetFrame->IsPedFoodNull();
			g_pPetFrame->SendWorldEvent( "PET_FEED_INFO_UPDATE" );
		}
		else
		{
			g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_PET_FEED_FAILED"));
		}
		break;

	case EM_CultivatePetCommandType_Travel:			//旅行
		if ( Result )
		{
			g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_PET_TRAVEL_SUCCEED"));
		}
		else
		{
			g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_PET_TRAVEL_FAILED"));
		}
		break;

	case EM_CultivatePetCommandType_Free:			//放生
		if ( Result )
		{
			PetBase.init();
			g_pPetFrame->ReleasePet(PetPos);
			g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_PET_RELEASE_SUCCEED"));
			g_pPetFrame->SendWorldEvent( "PET_RELEASE_SUCCEED" );
		}
		else
		{
			g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_PET_RELEASE_FAILED"));
		}
		break;

	case EM_CultivatePetCommandType_Train:			//特訓
		if ( Result )
		{
			g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_PET_TRAIN_SUCCEED"));
		}
		else
		{
			g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_PET_TRAIN_FAILED"));
		}
		break;
	/*
	case EM_CultivatePetCommandType_TrainCheck:		//驗收
		break;
	case EM_CultivatePetCommandType_ReTrain:		//複訓(使用物品)
		break;
	case EM_CultivatePetCommandType_TrainAccept:	//驗收確定
		PetBase.TranAccept();
		break;
	*/
	case EM_CultivatePetCommandType_Return:			//召喚出的寵物回收
		roleData->TempData.UpdateInfo.Recalculate_All = true;
		if ( Result )
		{
			g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_PET_RETURN_SUCCEED"));
			g_pPetFrame->SendWorldEvent( "PET_RETURN_SUCCEED" );
		}
		else
		{
			g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_PET_RETURN_FAILED"));
		}
		break;

	case EM_CultivatePetCommandType_LiftSkill:		//生活技能
		break;
	}

}
void NetCli_CultivatePetChild::RL_LearnSkillResult		( bool Result , int PetPos , int MagicID , int MagicLv )
{
	g_pPetFrame->R_LearnSkillResult(Result, PetPos, MagicID, MagicLv);
}
void NetCli_CultivatePetChild::RL_PetLifeResult	( bool Result , CultivatePetLifeSkillTypeENUM SkillType , int PetPos , int EventTime )
{
	g_pPetFrame->R_PetLifeResult(Result, SkillType, PetPos, EventTime);	
}
void NetCli_CultivatePetChild::RL_SwapItemResult		( bool Result )
{
	g_pPetFrame->R_SwapItemResult(Result);	
}
void NetCli_CultivatePetChild::RL_FixItem				( int PetPos , int PetItemPos , ItemFieldStruct& Item )
{
	g_pPetFrame->R_FixItem(PetPos, PetItemPos, Item);	
}
void NetCli_CultivatePetChild::RL_FixPetBase			( int PetPos , CultivatePetStruct& PetBase )
{
	g_pPetFrame->R_FixPetBase(PetPos, PetBase);
}
void NetCli_CultivatePetChild::RL_GetProductResult		( bool Result )
{
	g_pPetFrame->R_GetProductResult(Result);
}
void NetCli_CultivatePetChild::RL_PetEventProcess		( int PetPos , CultivatePetCommandTypeENUM Event )
{
	g_pPetFrame->R_PetEventProcess(Event);
}
void NetCli_CultivatePetChild::RL_PetMargeResult		( PetMargeResultENUM Result , int PetCount , int PetPos[3] , CultivatePetStruct& PetBase )
{
	g_pPetFrame->R_PetMargeResult(Result , PetCount , PetPos , PetBase );
}
//登入寵物卡片
void NetCli_CultivatePetChild::RL_RegPetCardResult( bool result )
{

	g_pPartnerFrame->RegPetCardResult(result  );
}
