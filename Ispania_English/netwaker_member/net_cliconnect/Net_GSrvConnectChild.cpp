#include "../net_talk/NetCli_TalkChild.h"
#include "../net_channel/NetCli_Channel.h"
#include "../net_item/NetCli_ItemChild.h"
#include "../net_rolevalue/NetCli_RoleValue.h"
#include "../net_party/NetCli_PartyChild.h"
#include "../net_other/NetCli_OtherChild.h"
#include "../../mainproc/GameMain.h"
#include "../../Interface/maintain/Maintain.h"
#include "../../Interface/gm/GM_ObjEdit.h"
#include "../../Interface/Login/AccountLogin.h"
#include "../../Interface/GuildHouses/GuildHouseStatus.h"
#include "../../Interface/WorldFrames/ChatEdit.h"
#include "../../Interface/WorldFrames/ChatFrame.h"
#include "../../Interface/WorldFrames/ActionFrame.h"
#include "../../Interface/WorldFrames/BagFrame.h"
#include "../../Interface/WorldFrames/PetFrame.h"
#include "../../Interface/WorldFrames/MerchantFrame.h"
#include "../../Interface/Map/WorldMap.h"
#include "Net_GSrvConnectChild.h"
#include "../net_login/NetCli_Login_Child.h"

//#include "../../Tools/ApexClient/RaApexClient.h"

#include "../../ErrorList/errorlist.h"

#include "../../Interface/Socal/UI_Socal.h"

#include "../Interface/ItemLink/ItemLink.h"

ZoneInfoModeStruct	Net_GSrvConnectChild::s_zoneMode;
int	Net_GSrvConnectChild::CheckLoginBattleGroundCount = 0;
//-----------------------------------------------------------------------------
bool Net_GSrvConnectChild::_Init()
{	
	if (This)
		return false;

	This = new Net_GSrvConnectChild;
	return Net_GSrvConnect::_Init();
}

//-----------------------------------------------------------------------------
bool Net_GSrvConnectChild::_Release()
{
	Net_GSrvConnect::_Release();

	if (This)
	{
		delete This;
		This = NULL;
	}

	return true;
}

//-----------------------------------------------------------------------------
void Net_GSrvConnectChild::PlayerItemID ( int GItemID, int WorldGUID , int DBID )
{
	g_pGameMain->SetPlayerID(WorldGUID);
	g_pGameMain->SetGItemID(GItemID);
	g_pGameMain->SetZoneDBID(DBID);
	g_pGameMain->SendLoadClientData();
	// g_pError->AddMessage(1, 0, "PlayerItemID GItemID[%d] WorldGUID[%d]", GItemID, WorldGUID);
}

//-----------------------------------------------------------------------------
void Net_GSrvConnectChild::ZoneInfo ( int GSrvNetID , int ZoneID , int GSrvID , int MapID , int CampObjID , LanguageTypeENUM LanguageType , ZoneInfoModeStruct ZoneMode, int teamNumPeople )
{
	s_zoneMode = ZoneMode;

	GameObjDbStructEx* ZoneDB = g_ObjectData->GetObj( ZoneID % _DEF_ZONE_BASE_COUNT_ + Def_ObjectClass_Zone );
	RoleDataEx::G_ZoneID = ZoneID;
	if( ZoneDB != NULL )
	{		
		g_pGameMain->SetMapName(ZoneDB->Zone.MapFile);
	}

	g_pGameMain->SetPlayerTarget(NULL);
	g_pGameMain->SetTimeGroupType(0);

	// 解除背包內的鎖定
	g_pBagFrame->ChangeZone();

	if ( ZoneMode.IsZonePartyEnabled )
	{
		ZoneMode.IsBattleGround = true;
		ZoneMode.IsDisableParty = false;
	}

	if ( ZoneDB )
	{
		g_pGameMain->SetWorldBattleGround(ZoneDB->Zone.IsWorldBattleGround);	// 是否世界戰場
	}

	g_pGameMain->SetBattleGroundZone(ZoneMode.IsBattleGround);				// 是否戰場
	g_pGameMain->SetDisableTrade(ZoneMode.IsDisableTrade);					// 不可交易
	g_pGameMain->SetDisableDuel(ZoneMode.IsDisableDual);					// 不可比試
	g_pGameMain->SetPVEState(ZoneMode.IsPvE);								// 是否PVE
	g_pGameMain->SetDisableParty(ZoneMode.IsDisableParty);					// 不能組隊
	g_pGameMain->SetEnableBGParty(ZoneMode.IsZonePartyEnabled);				// 開啟另一種區域(戰場)組隊
	g_pGameMain->SetNoSwimming(ZoneMode.IsNoSwimming);						// 不能游泳

	g_pGameMain->SetZoneID(ZoneID);
	g_pGameMain->SetCampTableObj(CampObjID);
	NetCli_Channel::S_ChannelZoneID(ZoneID);
	//g_pError->AddMessage(1, 0, "ZoneInfo ZoneID %d MapID %d", ZoneID, MapID);

	g_pUi_Socal->SetIsDisablePVPRule( ZoneMode.IsDisablePVPRule );
	g_pMerchantFrame->setDisableSendGift(ZoneMode.IsDisableSendGift);

	// 停止倒數計時
	if ( g_pInterface )
	{
		g_pInterface->SendWorldEvent("COUNTDOWN_END");
	}
}

//-----------------------------------------------------------------------------
void Net_GSrvConnectChild::PlayerData ( BaseRoleData *BaseRole , PlayerBaseRoleData *PlayerBaseRole , SysBaseRoleData* Base )
{
	RoleDataEx* roleData = CNetGlobal::RoleData();
	if (roleData)
	{
		roleData->BaseData = *BaseRole;
		*(roleData->PlayerBaseData) = *PlayerBaseRole;
		roleData->Base = *Base;
		roleData->Base.Type = EM_ObjectClass_Player;
		roleData->InitCal();
		roleData->TempData.UpdateInfo._Event = 0xffffffff;

		roleData->PlayerTempData->LockAccountMoney = roleData->PlayerBaseData->LockAccountMoney_Forever;
		for( int i = 0 ; i < Def_AccountMoneyLockTime_Max_Count_ ; i++ )
			roleData->PlayerTempData->LockAccountMoney += roleData->PlayerBaseData->LockAccountMoney[i];

		for ( int i = 0; i < DEF_MAX_COLDOWN_COUNT_ALL; i++ )
		{
			roleData->BaseData.Coldown.All[i] = BaseRole->Coldown.All[i] * 10;
		}

		for( int i =0 ; i < roleData->BaseData.Buff.Size() ; i++ )
		{
			BuffBaseStruct& Buf = roleData->BaseData.Buff[i];
			Buf.Magic = g_ObjectData->GetObj( Buf.BuffID );
		}


		set<int> bag;
		for ( int i = 0; i < _MAX_BODY_COUNT_ - _MAX_BODY_EX_COUNT_ - _MAX_BODY_MAGIC_BOX_COUNT_; i++ )
		{
			int index = roleData->PlayerBaseData->ClientData[i];
			set<int>::iterator iter = bag.find(index);

			// 發生欄位重複,重新設定索引值為實體位置
			if ( iter != bag.end() || index < _MAX_BODY_EX_COUNT_ + _MAX_BODY_MAGIC_BOX_COUNT_ )
			{
				for ( int j = 0; j < _MAX_BODY_COUNT_ - _MAX_BODY_EX_COUNT_ - _MAX_BODY_MAGIC_BOX_COUNT_; j++ )
					roleData->PlayerBaseData->ClientData[j] = _MAX_BODY_EX_COUNT_ + _MAX_BODY_MAGIC_BOX_COUNT_ + j;

				NetCli_RoleValue::S_ClientData(roleData->PlayerBaseData->ClientData);
				break;
			}

			bag.insert(index);
		}

		// 更新寵物技能
		for ( int i = 0; i < MAX_CultivatePet_Count; i++ )
		{
			g_pPetFrame->UpdateSkillInfo(i);
		}

		// g_pError->AddMessage(1, 0, "RoleData's guildID %d", BaseRole->GuildID);
		InitializeBaseData();
		NetCli_ItemChild::ClearAccountBag();
	}

	//將身上物品 註冊給itemlink
	int i;

	for( i = 0 ; i < _MAX_BODY_COUNT_ ; ++i )
	{
		RegisterItemFieldChecksum( &roleData->PlayerBaseData->Body.Item[i]);
	}

	for( i = 0 ; i < EM_EQWearPos_MaxCount ; ++i )
	{
		RegisterItemFieldChecksum( &roleData->BaseData.EQ.Item[i]);
	}

	for( i = 0 ; i < _MAX_BANK_COUNT_ ; ++i )
	{
		RegisterItemFieldChecksum( &roleData->PlayerBaseData->Bank.Item[i]);
	}

	// 是否被禁止發言
	if ( PlayerBaseRole )
	{
		int count = (int)(PlayerBaseRole->Defendant.MoneyTraderCount - 0.01f + 1);
		g_pChatEdit->RL_MoneyVendorReport(count);
	}

	// 通知角色金錢數值更改
	g_pChatEdit->SendWorldEvent(UiEvent_Role_Money_Change);

	//g_pInterface->VariablesLoaded();
	//g_pGameMain->SetRoleDataState(true);
}

//-----------------------------------------------------------------------------
void Net_GSrvConnectChild::PrepareChangeZone ()
{
	//g_pError->AddMessage(1, 0, "PrepareChangeZone");
}

//-----------------------------------------------------------------------------
void Net_GSrvConnectChild::ChangeZoneOK ()
{	
	RoleDataEx* roleData = CNetGlobal::RoleData();
	//g_pError->AddMessage(1, 0, "ChangeZoneOK");
	memset( roleData->PlayerTempData->Skill.LuaSkill , 0 , sizeof( roleData->PlayerTempData->Skill.LuaSkill ) );


	// 若玩家換區成, 關畢 Script 產生的選項


	// 清除戰場產生物件
	NetCli_PartyChild::ClearBattleGroup();
	g_pGameMain->ClearAllBattleMember();
	g_pGameMain->SetPetMovable(true);
	g_pGameMain->ClearAttachObj();

	if ( g_pInterface )
		g_pInterface->PlayerChangeZone();

	if (g_pGameMain->IsBattleGroundZone())
	{

		g_pWorldMapFrame->ClearMapIconWhenChangeZone(g_pGameMain->GetBattleGroundType());

	}

	NetCli_Other::SL_ClientLanguage(ObjectDataClass::GetLanguageType(g_pGameMain->GetLanguage()));
}

//-----------------------------------------------------------------------------
void Net_GSrvConnectChild::R_ModyNPC ( ModifyNPCInfo*  Info )
{
	if(g_pGM_ObjEdit && g_pGM_ObjEdit->IsVisible())
	{
		g_pGM_ObjEdit->UpdateModifyNPCInfo(Info);
	}
	else
	{
		g_pMaintain->ModifyOpen( Info );
	}
}
//-----------------------------------------------------------------------------
//回應 npc 路徑資訊
void Net_GSrvConnectChild::R_GetNPCMoveInfoResult	( int iObjID , vector<NPC_MoveBaseStruct>& List )
{
	if(g_pGM_ObjEdit && g_pGM_ObjEdit->IsVisible())
	{
		g_pGM_ObjEdit->UpdateNPCMoveInfo(iObjID, List);
	}
	else
	{
		// 將取得的路徑插上顯像旗子
		//NetCli_Talk::GMCommand(text);
		char szCommand[256];
		vector<NPC_MoveBaseStruct>::iterator it;
		
		int iCount = 0;
		for( it = List.begin(); it != List.end(); it++ )
		{

			float X		= (*it).X;
			float Y		= (*it).Y;
			float Z		= (*it).Z;
			float Dir	= (*it).Dir;

			sprintf( szCommand, "createobj 580000 P-%d %f %f %f %f", iCount, X, Y, Z, Dir );
			NetCli_Talk::GMCommand( szCommand );
			iCount++;
		}
	}	
}

void Net_GSrvConnectChild::R_GetFlagListResult ( int GitemID, vector< vector < CliFlagPosInfo > >& vecFlagList )
{
	if(g_pGM_ObjEdit && g_pGM_ObjEdit->IsVisible())
	{
		g_pGM_ObjEdit->UpdateMarkerList(GitemID, &vecFlagList);
	}
}

void Net_GSrvConnectChild::R_EditFlagResult(int GitemID, int iResult)
{
}

void Net_GSrvConnectChild::R_DelFlagResult(int GitemID, int iResult)
{
}

void Net_GSrvConnectChild::R_DelFlagGroupResult(int GitemID, int iResult)
{
}

void Net_GSrvConnectChild::R_LogoutResult			( bool Result )
{
	//CApexClient::Release();

	// 在遊戲內退回選角界面
	if ( Result ) {

		g_pGameMain->ReturnCharacterSelect();	
			
	} else {
		// 錯誤
	}
}

void Net_GSrvConnectChild::R_ParalledID			( int ParalledID )
{
	// 記錄目前分流
	g_pGameMain->ClearChangeParalleID();
	g_pAccountLogin->SetParalleZoneID(ParalledID);

	// 提示訊息
	g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_PARALLEL_ID"), ParalledID + 1);

	// 更新界面
	g_pInterface->SendWorldEvent("UPDATE_PARALLEL");
}

void	Net_GSrvConnectChild::R_SysVersionInfo		( int LearnMagicID , SystemVersionENUM Version , float PK_DotRate )
{
	RoleDataEx::GlobalSetting.LearnMagicID	= LearnMagicID;
	RoleDataEx::GlobalSetting.Version		= Version;
	RoleDataEx::GlobalSetting.PK_DotRate	= PK_DotRate;

	g_pActionFrame->DefaultAction();   // 因為學習表格會有偏位,所以需要改在版號進來時做設定
	//g_pError->AddMessage(1, 0, "R_SysVersionInfo LearnMagicID[%d] Version[%d]", LearnMagicID, Version);
}


void	Net_GSrvConnectChild::R_ConnectToBattleWorld( char* IPStr , int Port  )
{
	_NetBattle.Connect( IPStr , Port , CNetCli_Login::m_szAccount , "", g_pGameMain->GetMac(), false );
}

void	Net_GSrvConnectChild::R_SkyProcType			( ClientSkyProcTypeENUM Type )
{
	switch (Type)
	{	
	case EM_ClientSkyProcType_Day:			//白天
		g_pGameMain->SetTimeGroupType(2);
		break;
	case EM_ClientSkyProcType_NightFall:	//黃昏
		g_pGameMain->SetTimeGroupType(3);
		break;
	case EM_ClientSkyProcType_Night:		//晚上
		g_pGameMain->SetTimeGroupType(4);
		break;
	case EM_ClientSkyProcType_Dawn:			//黎明
		g_pGameMain->SetTimeGroupType(1);
		break;
	case EM_ClientSkyProcType_Normal:		//一般
	default:	
		g_pGameMain->SetTimeGroupType(0);
		break;
	}
}

void	Net_GSrvConnectChild::R_TouchTomb				( )
{
	g_pGameMain->SendSystemMsg(g_ObjectData->GetString("SYS_TOUCH_TOMB"));
	g_pGameMain->ClearTomb();
	
	CRoleSprite* player = g_pGameMain->GetPlayer();
	if ( player )
	{
		CEffectSprite* pEffect = new CEffectSprite;
		if ( pEffect->LoadRuEntity("model\\fx\\pickbody\\act_pickbody_t_hit_point_e.ros") )
		{
			pEffect->AttachSprite(player, eAttach_Hit_Point2);
			pEffect->SetEffectMotionStep(eEffectMotion_Step_Explode);
		}
		else
			delete pEffect;
	}
}

void	Net_GSrvConnectChild::R_TombInfo ( int CreateTime , int ZoneID , float X , float Y , float Z , int DebtExp , int DebtTp , int Exp )
{

	CRuVector3 position;
	position.m_x = X;
	position.m_y = Y;
	position.m_z = Z;
	g_pGameMain->SetTombInfo(ZoneID, position, (float)CreateTime, DebtExp, DebtTp, Exp);
}

void	Net_GSrvConnectChild::OnLoginBattleGround		( )
{
	_InBattleGround = true;
	g_pGuildHouseWarStatus->Reset();

	TRACE2( "OnLoginBattleGround CheckLoginBattleGroundCount=%d CliID=%d" , CheckLoginBattleGroundCount , _NetBattle.CliID() );
	g_pError->AddMessage( 0, 0, "OnLoginBattleGround CheckLoginBattleGroundCount=%d CliID=%d" , CheckLoginBattleGroundCount , _NetBattle.CliID() );	
	CheckLoginBattleGroundCount++;
	S_ConnectToBattleWorldOK( _NetBattle.CliID() );
}

void	Net_GSrvConnectChild::OnLogoutBattleGround		( )
{
	CheckLoginBattleGroundCount--;
	TRACE2( "OnLoginBattleGround CheckLoginBattleGroundCount=%d CliID=%d" , CheckLoginBattleGroundCount , _NetBattle.CliID() );
	g_pError->AddMessage( 0, 0, "OnLoginBattleGround CheckLoginBattleGroundCount=%d CliID=%d" , CheckLoginBattleGroundCount , _NetBattle.CliID() );
}


void	Net_GSrvConnectChild::R_WorldReturnNotify( int ZoneID )
{
	if( ZoneID != -1 && ZoneID != g_pGameMain->GetZoneID() )
		return;

	_InBattleGround = false;

	S_WorldReturnOK();
	SetSrvID_Local_Battle( -1 );	
	_NetBattle.Logout( EM_ClientNetCloseType_LeaveBattleWorld );
}
//////////////////////////////////////////////////////////////////////////
void	Net_GSrvConnectChild::RL_ZoneOpenState				( int ZoneID , bool IsOpen )
{
	g_pGameMain->SetZoneChannel(ZoneID, IsOpen);
}