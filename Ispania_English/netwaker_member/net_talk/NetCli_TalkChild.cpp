#include "NetCli_TalkChild.h"
#include "../net_channel/NetCli_ChannelChild.h"
#include "../../mainproc/GameMain.h"
#include "../../Interface/WorldFrames/ChatEdit.h"
#include "../../Interface/WorldFrames/ChatFrame.h"
#include "../../Interface/WorldFrames/SpeakFrame.h"
#include "../../Interface/Debug/DebugMessage.h"
#include "smallobj/SmallObj.h"
#include "../Net_Script/NetCli_Script_Child.h"
#include "../../Interface/PartyBoard/PartyBoardFrame.h"
#include "../../Interface/WorldFrames/GuildFrame.h"
//#include "../../Interface/WorldFrames/ChatMsn.h"

//-----------------------------------------------------------------------------
bool NetCli_TalkChild::_Init()
{
	if (This)
		return false;
	This = new NetCli_TalkChild;

	((NetCli_TalkChild*)This)->PrinttoFile.SetOutputFileName( "Chat.Log" );
	((NetCli_TalkChild*)This)->PrinttoFile.SetOutputFieldName( "Log" );

	return NetCli_Talk::_Init();
}

//-----------------------------------------------------------------------------
bool NetCli_TalkChild::_Release()
{
	NetCli_Talk::_Release();
	if (This)
	{
		delete This;
		This = NULL;
	}
	return true;
}

//-----------------------------------------------------------------------------
void NetCli_TalkChild::Rev_TalkTo( Voc_ENUM Job , bool IsSystem , const char* SenderName , const char* TargetName, int ContentSize , const char* Content, TalkSenderInfo& SenderInfo )
{
	char szReplaceString[4096];

	//SenderInfo.vocation[0] = Job;

	if( IsSystem == true )
	{
		string sRangeString = CNetCli_Script_Child::ReplaceSystemKeyword( Content );

		ContentSize = (int)sRangeString.size();
		Content		= sRangeString.c_str();

		ZeroMemory( szReplaceString, sizeof( szReplaceString ) );
		memcpy( szReplaceString, Content, ContentSize );

		Content		= szReplaceString;
		ContentSize	= ContentSize + 1;

		// 因為語系的因素, Server 發送過來的 NPC 名稱, 會以模版的名稱送過來, 比方說 Sys111282_name
		SenderName = g_ObjectData->GetSpecialString( SenderName );
	}

	// 系統發出
	if ( IsSystem ) {
		char senderName[MAX_PATH];
		senderName[0] = NPC_HEADER;
		strcpy(senderName+1, SenderName);
		g_pChatFrame->SendChatMessage(CHAT_MSG_WHISPER, senderName, Content);
	} else {
		// 轉換字串
		g_pGameMain->MatchPatternString((BYTE*)Content);

		// 對自己說話
		if ( strcmp(SenderName, g_pGameMain->GetPlayerName()) == 0 )
		{
			g_pChatFrame->SendChatMessage(CHAT_MSG_WHISPER_INFORM, TargetName, Content);
			g_pChatFrame->HaveNewWhisper(TargetName, Content, 1);
		}
		// 接收密語,黑名單除外
		else if ( !RoleData()->CheckIgnoreNameList(SenderName) && !g_pChatEdit->IsUndesirable(SenderName) )
		{
			// g_ruSymphony->PlaySoundByPath( "Sound\\Interface\\whisper.mp3", false );
			int loopCount = (FALSE) ? 0x7FFFFFFF : 1;
			g_ruAudible->Play("Sound\\Interface\\whisper.mp3", (CRuAudible::AudibleType)ruSOUNDTYPE_SFX, (loopCount > 1) ? TRUE : FALSE);						
			g_pChatFrame->SendChatMessage(CHAT_MSG_WHISPER, SenderName, Content, SenderInfo.vocation[0]);
			g_pChatFrame->HaveNewWhisper(SenderName, Content, 0);
		}
	}

	if( g_pGameMain->IsChatLog()  )
	{
		char tmp[512];
		//g_pDebugMessage->DebugOutput(0, "[Tell]%s->%s:%s", SenderName , TargetName ,Content);
		sprintf( tmp ,"[Tell]%s->%s:%s", SenderName , TargetName ,Content );
		PrinttoFile.OutputToFile( tmp );
	}
}

//-----------------------------------------------------------------------------
void NetCli_TalkChild::Rev_Range( Voc_ENUM Job , bool IsSystem , int Sayer , const char* Name , int ContentSize , const char* Content, bool IsNPCGossip, TalkSenderInfo& SenderInfo )
{
	char szReplaceString[4096];

	// 替換系統字串
	CRoleSprite* sprite = g_pGameMain->FindSpriteByID(Sayer);
	if (sprite && sprite->IsVisible() == false)
		return;	

	//SenderInfo.vocation[0] = Job;

	if( IsSystem == true )
	{
		string sRangeString = CNetCli_Script_Child::ReplaceSystemKeyword( Content );

		ContentSize = (int)sRangeString.size();
		Content		= sRangeString.c_str();

		ZeroMemory( szReplaceString, sizeof( szReplaceString ) );
		memcpy( szReplaceString, Content, ContentSize );

		Content		= szReplaceString;
		ContentSize	= ContentSize + 1;

		// 因為語系的因素, Server 發送過來的 NPC 名稱, 會以模版的名稱送過來, 比方說 Sys111282_name
		Name = g_ObjectData->GetSpecialString( Name );
	}

	std::string szContent = Content;
	const char* key = "[$HIDECHAT]";
	int len = (int)strlen(key);
	size_t begin;
	while ( (begin = szContent.find(key)) != string::npos )
		szContent.replace(begin, len, "");

	if ( IsSystem )	{
		g_pSpeakFrame->AddText(Sayer, szContent.c_str(), 1.0f, 1.0f, 1.0f);

		char senderName[MAX_PATH];
		senderName[0] = NPC_HEADER;
		strcpy(senderName+1, Name);

		if ( IsNPCGossip && g_pGameMain->GetDisableDisplayNPCTalk() )
		{
			// 目前閒聊不顯示
		}
		else
		{
			if ( strstr(Content, "[$HIDECHAT]") == NULL )
			{
				g_pChatFrame->SendChatMessage(CHAT_MSG_SAY, senderName, szContent.c_str());
			}
		}		
	} else if ( !RoleData()->CheckIgnoreNameList(Name) && !g_pChatEdit->IsUndesirable(Name) ) {
		// 轉換字串
		g_pGameMain->MatchPatternString((BYTE*)szContent.c_str());

		g_pSpeakFrame->AddText(Sayer, szContent.c_str(), 1.0f, 1.0f, 1.0f);

		if ( IsNPCGossip && g_pGameMain->GetDisableDisplayNPCTalk() )
		{
			// 目前閒聊不顯示
		}
		else
		{
			g_pChatFrame->SendChatMessage(CHAT_MSG_SAY, Name, szContent.c_str(), SenderInfo.vocation[0]);
		}
	}

	if( g_pGameMain->IsChatLog()  )
	{
		char tmp[512];
		//sprintf( tmp ,CHAT_MSG_SAY ,Content, Name );
		sprintf( tmp ,"[Range]%s:%s" ,Name , Content);

		PrinttoFile.OutputToFile( tmp );
	}
}

//-----------------------------------------------------------------------------
/*
void NetCli_TalkChild::Rev_Zone( int Sayer , const char* Name , int ContentSize , const char* Content )
{
	char tmp[512];
	sprintf( tmp, "[%s]:%s", Name, Content );
	g_pChatFrame->AddMessage(eMessage_Channel_Zone, tmp);
}
*/
//-----------------------------------------------------------------------------
void NetCli_TalkChild::Rev_SystemMsg( int ContentSize , const char* Content )
{
	if ( g_pGameMain->IsShowServerDebugMsg() ) {
		g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, Content, "");
	}
}

//-----------------------------------------------------------------------------
void NetCli_TalkChild::Rev_GmMsg( const char* Msg )
{
	string sString = CNetCli_Script_Child::ReplaceSystemKeyword( Msg );
	g_pChatFrame->SendChatMessageEx(CHAT_MSG_GM_TALK, "", "[GM]%s", sString.c_str());
}
//-----------------------------------------------------------------------------
void NetCli_TalkChild::Rev_Channel	( Voc_ENUM Job , GroupObjectTypeEnum Type , int ChannelID , TalkChannelTypeENUM MsgType , char* Name , char* Content, TalkSenderInfo& SenderInfo )
{
	char tmp[512];
	const char *ChannelName=NULL;

	if ( RoleData()->CheckIgnoreNameList(Name) == true || g_pChatEdit->IsUndesirable(Name) == true )
		return;	

	//SenderInfo.vocation[0] = Job;
	g_pGameMain->MatchPatternString((BYTE*)Content);

	switch( Type )
	{
	case EM_GroupObjectType_SysChannel:

		g_pChatFrame->SendChatMessage(CHAT_MSG_YELL, Name, Content, SenderInfo.vocation[0]);

		//sprintf( tmp, "[SysChannel %d][%s]:%s", ChannelID ,  Name, Content );
		//g_pChatFrame->AddMessage(0, tmp);
		break;
	case EM_GroupObjectType_SysZoneChannel:
	
		g_pChatFrame->SendChatMessage(CHAT_MSG_ZONE, Name, Content, SenderInfo.vocation[0]);		

		/*
		g_pChatFrame->SendChatMessageEx(CHAT_MSG_CHANNEL, Name, "[%d.%s]%s", ChannelID, g_pGameMain->GetZoneName(), Content);
		sprintf( tmp, "[ZoneChannel %d][%s]:%s", ChannelID ,  Name, Content );
		*/
		break;

	case EM_GroupObjectType_PlayerChannel:
		{
			const char* channelName = NetCli_ChannelChild::GetChannelNameByID(ChannelID);
		
			
			if ( channelName != NULL )
			{				
				sprintf_s(tmp, "%s%s", CHAT_MSG_CHANNEL, channelName);
				
				g_pChatFrame->SendChatMessage(tmp, Name, Content, SenderInfo.vocation[0]);
			}
		}
		break;
	case EM_GroupObjectType_Party:
		
	
		g_pChatFrame->SendChatMessage(CHAT_MSG_PARTY, Name, Content, SenderInfo.vocation[0]);
		break;
	case EM_GroupObjectType_Guild:
		
		g_pChatFrame->SendChatMessage(CHAT_MSG_GUILD, Name, Content, SenderInfo.vocation[0]);
		break;
	}

	int isSelf=0;

	// 自己發言
	if ( strcmp(Name, g_pGameMain->GetPlayerName()) == 0 )
		isSelf=1;


	{
	 	//ChannelName=g_ObjectData->GetString(ChannelName);

		g_pChatFrame->HaveNewChannel(Type,ChannelID, Name, Content, isSelf);

	}
	

	if( g_pGameMain->IsChatLog()  )
		PrinttoFile.OutputToFile( tmp );
}
//-----------------------------------------------------------------------------
void NetCli_TalkChild::Rev_GameMessageDataEvent	( GameMessageEventDataENUM Event , int iNumArg, GameMessageDataArgStruct Data[5] )
{
	char KeyStr[256];
	sprintf( KeyStr , "SYS_GAMEMSGEVENTDATA_%03d" , Event );
	const char* MsgStr = g_ObjectData->GetString( KeyStr );

	char ArgStr[5][64];

	for( int i = 0 ; i < iNumArg ; i++ )
	{
		switch( Data[i].Type )
		{
		case EM_GameMessageDataType_None:
			ArgStr[i][0] = 0;
			break;
		case EM_GameMessageDataType_Int:
			if(strcmp(KeyStr,"SYS_GAMEMSGEVENTDATA_004") == 0)
			{
				sprintf( ArgStr[i] , "%02d" , Data[i].Value );
				g_pGuildFrame->GuildCanNotJoin();
			}
			else
			{
				sprintf( ArgStr[i] , "%d" , Data[i].Value );
			}
			break;
		case EM_GameMessageDataType_ObjID_Name:
			GameObjDbStructEx* ObjDB = g_ObjectData->GetObj( Data[i].Value );
			if( ObjDB == NULL )
				ArgStr[i][0] = 0;
			else
				sprintf( ArgStr[i] , "%s" , ObjDB->GetName() );
			break;
		}
	}
	
	char Content[4096];
	
	switch( iNumArg )
	{
		case 1:	sprintf( Content ,  MsgStr , ArgStr[0] );	break;
		case 2:	sprintf( Content ,  MsgStr , ArgStr[0], ArgStr[1] );	break;
		case 3:	sprintf( Content ,  MsgStr , ArgStr[0], ArgStr[1], ArgStr[2] );	break;
		case 4:	sprintf( Content ,  MsgStr , ArgStr[0], ArgStr[1], ArgStr[2], ArgStr[3] );	break;
		case 5:	sprintf( Content ,  MsgStr , ArgStr[0], ArgStr[1], ArgStr[2], ArgStr[3], ArgStr[4] );	break;
	}

	//sprintf( Content ,  MsgStr , ArgStr[0] , ArgStr[1], ArgStr[2], ArgStr[3], ArgStr[4] );
	g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, Content, "" );
	g_pGameMain->SendWarningMsg(Content);
}
//-----------------------------------------------------------------------------
void NetCli_TalkChild::Rev_GameMessageEvent	( GameMessageEventENUM Evnet )
{
	float frameTime = (float)g_pGameMain->GetFrameTime();

	char KeyStr[256];
	sprintf( KeyStr , "SYS_GAMEMSGEVENT_%03d" , Evnet );
	const char* MsgStr = g_ObjectData->GetString( KeyStr );

	switch(Evnet)
	{
	case EM_GameMessageEvent_Role_Right_NoSpeak:			//GM對你 永久禁言
	case EM_GameMessageEvent_Role_Right_Speak:				//GM對你 解除禁言
	case EM_GameMessageEvent_Role_Right_NoTrade:			//GM對你 永久禁止交易
	case EM_GameMessageEvent_Role_Right_Trade:				//GM對你 解除禁止交易
	case EM_GameMessageEvent_TargetHasParty:
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", MsgStr);
		break;

	case EM_GameMessageEvent_AttackError_TargetTooFar:		//攻擊目標過遠
	case EM_GameMessageEvent_AttackError_Dir:				//攻擊面向有問題
		{
			static float time = 0.0f;
			if ( frameTime - time > 5.0f )
			{
				time = frameTime;
				g_pGameMain->SendWarningMsg(MsgStr);
			}
		}
		break;

	case EM_GameMessageEvent_MagicError_MPNotEnough:			//MP不夠
	case EM_GameMessageEvent_MagicError_WarriorSPNotEnough:		//戰士SP不夠
	case EM_GameMessageEvent_MagicError_RangerSPNotEnough:		//遊俠SP不夠
	case EM_GameMessageEvent_MagicError_ThiefSPNotEnough:		//盜賊SP不夠
		g_pChatFrame->SendWorldEvent("UPDATE_MOUSE_LIGHT_NotEnough");// 送出事件給LUA

	default:
		g_pGameMain->SendWarningMsg(MsgStr);
		break;
		/*
	case EM_GameMessageEvent_AttackError_TargetDead:		//目標死亡
		g_pGameMain->SendWarningMsg("目標死亡");
		break;
	case EM_GameMessageEvent_AttackError_Target:			//目標不可攻擊
		g_pGameMain->SendWarningMsg("目標不可攻擊");
		break;
	case EM_GameMessageEvent_MagicError_OnSpell:			//已在施法中
		g_pGameMain->SendWarningMsg("已在施法中");
		break;
	case EM_GameMessageEvent_MagicError_TargetTooFar:		//施法距離過遠
		g_pGameMain->SendWarningMsg("施法距離過遠");
		break;
	case EM_GameMessageEvent_MagicError_Request:			//法術需求有問題
		g_pGameMain->SendWarningMsg("法術需求有問題");
		break;
	case EM_GameMessageEvent_MagicError_Target:				//法術目標有問題
		g_pGameMain->SendWarningMsg("法術目標有問題");
		break;
	case EM_GameMessageEvent_MagicError_Back:				//要在目標背後才可施展
		g_pGameMain->SendWarningMsg("要在目標背後才可施展");
		break;
	case EM_GameMessageEvent_MagicError_Front:				//要在目標前面才可施展
		g_pGameMain->SendWarningMsg("要在目標前面才可施展");
		break;
	case EM_GameMessageEvent_MagicError_LookAt:				//要看著目標才可施展
		g_pGameMain->SendWarningMsg("要看著目標才可施展");
		break;
	case EM_GameMessageEvent_MagicError_Coldown:			//冷卻中
		g_pGameMain->SendWarningMsg("冷卻中");
		break;
	case	EM_GameMessageEvent_OK								:	//沒有問題
		g_pGameMain->SendWarningMsg("冷卻中");
		break;
	case	EM_GameMessageEvent_Failed							:	//一般失敗
		g_pGameMain->SendWarningMsg("冷卻中");
		break;
	case	EM_GameMessageEvent_FightNoLogout					:	//戰鬥中不可登出
		g_pGameMain->SendWarningMsg("冷卻中");
		break;
	case	EM_GameMessageEvent_ChanageZoneError_PraviteZoneFull:	//獨立區域滿了
		g_pGameMain->SendWarningMsg("冷卻中");
		break;
	case	EM_GameMessageEvent_ChanageZoneError_Failed			:	//換區失敗
		g_pGameMain->SendWarningMsg("冷卻中");
		break;
	case	EM_GameMessageEvent_MoveSpeedDataError				:	//移動資料有問題
		g_pGameMain->SendWarningMsg("冷卻中");
		break;
	case	EM_GameMessageEvent_MoveSpeedToFastError			:	//移動加速??
		g_pGameMain->SendWarningMsg("冷卻中");
		break;
	case	EM_GameMessageEvent_TotalMoveSpeedToFastError		:	//移動總量加速??
		g_pGameMain->SendWarningMsg("冷卻中");
		break;
	case	EM_GameMessageEvent_MoveVectorError					:	//移動向量過大
		g_pGameMain->SendWarningMsg("冷卻中");
		break;
	case	EM_GameMessageEvent_NoMove							:	//目前被施展定身法術，不可移動
		g_pGameMain->SendWarningMsg("冷卻中");
		break;
	case	EM_GameMessageEvent_CharacterNotFound				:	//角色不存在
		g_pGameMain->SendWarningMsg("冷卻中");
		break;
	case	EM_GameMessageEvent_AttackError_PhyDisable			:	//不可物理攻擊
		g_pGameMain->SendWarningMsg("冷卻中");
		break;
	case	EM_GameMessageEvent_AttackError_MagicDisable		:	//不可法術攻擊
		g_pGameMain->SendWarningMsg("冷卻中");
		break;
	case	EM_GameMessageEvent_MagicError_MagicIDError			:	//MagicID有問題
		g_pGameMain->SendWarningMsg("冷卻中");
		break;
	case	EM_GameMessageEvent_MagicError_CreateObjectError	:	//件立物件失敗
		g_pGameMain->SendWarningMsg("冷卻中");
		break;
	case	EM_GameMessageEvent_MagicError_HPNotEnough			:	//HP不夠
		g_pGameMain->SendWarningMsg("冷卻中");
		break;
	case	EM_GameMessageEvent_MagicError_MPNotEnough			:	//MP不夠
		g_pGameMain->SendWarningMsg("冷卻中");
		break;
	case	EM_GameMessageEvent_MagicError_WarriorSPNotEnough	:	//戰士SP不夠
		g_pGameMain->SendWarningMsg("冷卻中");
		break;
	case	EM_GameMessageEvent_MagicError_RangerSPNotEnough	:	//遊俠SP不夠
		g_pGameMain->SendWarningMsg("冷卻中");
		break;
	case	EM_GameMessageEvent_MagicError_ThiefSPNotEnough		:	//盜賊SP不夠
		g_pGameMain->SendWarningMsg("冷卻中");
		break;
	case	EM_GameMessageEvent_MagicError_StomachFull			:	//吃不下
		g_pGameMain->SendWarningMsg("冷卻中");
		break;
	case	EM_GameMessageEvent_MagicError_CostItemError		:	//消耗物品有問題
		g_pGameMain->SendWarningMsg("冷卻中");
		break;
	case	EM_GameMessageEvent_MagicError_NoGun				:	//沒槍
		g_pGameMain->SendWarningMsg("冷卻中");
		break;
	case	EM_GameMessageEvent_MagicError_NoBow				:	//沒弓
		g_pGameMain->SendWarningMsg("冷卻中");
		break;
	case	EM_GameMessageEvent_MagicError_NoCossBow			:	//沒十字弓
		g_pGameMain->SendWarningMsg("冷卻中");
		break;
	case	EM_GameMessageEvent_MagicError_NoGunAmmo			:	//沒子彈
		g_pGameMain->SendWarningMsg("冷卻中");
		break;
	case	EM_GameMessageEvent_MagicError_NoBowAmmo			:	//沒箭
		g_pGameMain->SendWarningMsg("冷卻中");
		break;
	case	EM_GameMessageEvent_MagicError_NoCossBowAmmo		:	//沒矢
		g_pGameMain->SendWarningMsg("冷卻中");
		break;
	case	EM_GameMessageEvent_MagicError_NoThrowAmmo			:	//沒投擲物
		g_pGameMain->SendWarningMsg("冷卻中");
		break;
	case	EM_GameMessageEvent_MagicError_NoAllLongWeapon		:	//沒遠程武器
		g_pGameMain->SendWarningMsg("冷卻中");
		break;
	case	EM_GameMessageEvent_MagicError_NoAmmo				:	//沒彈藥
		g_pGameMain->SendWarningMsg("冷卻中");
		break;
	case	EM_GameMessageEvent_MagicError_NeedWeaponError		:	//武器無法施展此招式
		g_pGameMain->SendWarningMsg("冷卻中");
		break;
	case	EM_GameMessageEvent_MagicError_NeedBuffError		:	//缺少前置的法術Buf
		g_pGameMain->SendWarningMsg("冷卻中");
		break;
	case	EM_GameMessageEvent_MagicError_NeedNoBuffError		:	//抗Buff還沒有消失
		g_pGameMain->SendWarningMsg("冷卻中");
		break;
	case	EM_GameMessageEvent_MagicError_NeedTargetNoBuffError:	//目標抗Buff還沒有消失
	case	EM_GameMessageEvent_MagicError_NeedUnarmed			:	//需要空手才可施展
	case	EM_GameMessageEvent_MagicError_NeedBlade			:	//需要主手拿單手劍才可施展
	case	EM_GameMessageEvent_MagicError_NeedDagger			:	//需要主手拿匕首才可施展
	case	EM_GameMessageEvent_MagicError_NeedWand				:	//需要主手拿權杖才可施展
	case	EM_GameMessageEvent_MagicError_NeedAxe				:	//需要主手拿單手斧才可施展
	case	EM_GameMessageEvent_MagicError_NeedBludgeon			:	//需要拿棍棒才可施展
	case	EM_GameMessageEvent_MagicError_NeedClaymore			:	//需要拿雙手劍才可施展
	case	EM_GameMessageEvent_MagicError_NeedStaff			:	//需要拿雙手杖才可施展
	case	EM_GameMessageEvent_MagicError_Need2HAxe			:	//需要拿雙手斧才可施展
	case	EM_GameMessageEvent_MagicError_Need2HHammer			:	//需要拿雙手鎚才可施展
	case	EM_GameMessageEvent_MagicError_NeedPolearm			:	//需要拿長矛才可施展
	case	EM_GameMessageEvent_MagicError_NeedSwordType		:	//需要拿劍類才可施展
	case	EM_GameMessageEvent_MagicError_NeedAxeType			:	//需要拿斧才可施展
	case	EM_GameMessageEvent_MagicError_NeedShield			:	//需要拿盾才可施展
	case	EM_GameMessageEvent_MagicError_NeedDistance			:	//需要距離不夠
	case	EM_GameMessageEvent_MagicError_NeedNoAttack			:	//需要自己非戰鬥
	case	EM_GameMessageEvent_MagicError_NeedAttack			:	//需要戰鬥才可使用
	case	EM_GameMessageEvent_MagicError_NeedCritical			:	//爆擊後才可使用
	case	EM_GameMessageEvent_MagicError_NeedBeCritical		:	//被爆擊後才可使用
	case	EM_GameMessageEvent_MagicError_NeedDodge			:	//閃避後才可使用
	case	EM_GameMessageEvent_MagicError_NeedMiss				:	//Miss後才可使用
	case	EM_GameMessageEvent_MagicError_NeedParry			:	//格檔後才可使用
	case	EM_GameMessageEvent_MagicError_NeedBeParry			:	//被格檔後才可使用
	case	EM_GameMessageEvent_MagicError_NeedTargetNoAttack	:	//目標非戰鬥才可使用
	case	EM_GameMessageEvent_MagicError_NeedTargetAttack		:	//目標戰鬥才可使用
	case	EM_GameMessageEvent_MagicError_NeedBeDodge			:	//被閃避才可使用
	case	EM_GameMessageEvent_MagicError_NeedHPSmallerPercent	:	//HP過多無法使用
	case	EM_GameMessageEvent_MagicError_NeedHPGreaterPercent	:	//HP過少無法使用
	case	EM_GameMessageEvent_MagicError_NeedJob				:	//此職業無法施展
	case	EM_GameMessageEvent_MagicError_NeedLongDistWeapon	:	//需要拿長距離武器才可使用
	case	EM_GameMessageEvent_MagicError_NeedHammerType		:	//需要拿鎚類武器才可使用
	case	EM_GameMessageEvent_MagicError_Need2HWeapon			:	//需要拿雙手武器才可使用
	case	EM_GameMessageEvent_MagicError_PetNotFound			:	//找不到寵物
	case	EM_GameMessageEvent_Shop_NotCloseYet				:	//交易狀態沒解除
	case	EM_GameMessageEvent_Shop_ACOpenYet					:	//交易所正在開啟中
	case	EM_GameMessageEvent_Shop_MailOpenYet				:	//郵件正在開啟中
	case	EM_GameMessageEvent_Shop_ShopOpenYet				:	//商店正在開啟中
	case	EM_GameMessageEvent_Shop_BankOpenYet				:	//銀行正在開啟中
	case	EM_GameMessageEvent_Shop_ChangeJobOpenYet			:	//換角介面正在開啟中
	case	EM_GameMessageEvent_Shop_BillBoardOpenYet			:	//排行榜介面正在開啟中
	case	EM_GameMessageEvent_Shop_ACNoOpen					:	//交易所還沒開啟
	case	EM_GameMessageEvent_Shop_MailNoOpen					:	//郵件還沒開啟
	case	EM_GameMessageEvent_Shop_ShopNoOpen					:	//商店還沒開啟
	case	EM_GameMessageEvent_Shop_BankNoOpen					:	//銀行還沒開啟
	case	EM_GameMessageEvent_Shop_ChangeJobNoOpen			:	//換角還沒開啟
	case	EM_GameMessageEvent_Shop_BillBoardNoOpen			:	//排行榜還沒開啟
	case	EM_GameMessageEvent_Shop_ItemNotSell				:	//沒賣此物品
	case	EM_GameMessageEvent_Shop_CountTooMany				:	//購買過多	
	case	EM_GameMessageEvent_Shop_ItemNotExist				:	//此物品不存在
	case	EM_GameMessageEvent_Shop_CountError					:	//購買數量有問題
	case	EM_GameMessageEvent_Shop_NoSellAccountItem			:	//不可購買點數商品
	case	EM_GameMessageEvent_Shop_MomeyNotEnought			:	//金額不足
	case	EM_GameMessageEvent_Shop_BagFull					:	//包裹滿了
	case	EM_GameMessageEvent_Shop_SellDataError				:	//物品資料不符
	case	EM_GameMessageEvent_Shop_SellItemOnUse				:	//物品還在使用中
	case	EM_GameMessageEvent_Shop_CanNotSell					:	//此物品不可賣
	case	EM_GameMessageEvent_Shop_PepairDataError			:	//物品資料不符
	case	EM_GameMessageEvent_EQError_WearPos_Body			:	//裝備位置有問題
	case	EM_GameMessageEvent_EQError_DataInfo_Body			:	//裝備資料有問題
	case	EM_GameMessageEvent_EQError_OrgObjID_Body			:	//裝備物件有問題
	case	EM_GameMessageEvent_EQError_WearPos_Bank			:	//裝備位置有問題
	case	EM_GameMessageEvent_EQError_DataInfo_Bank			:	//裝備資料有問題
	case	EM_GameMessageEvent_EQError_OrgObjID_Bank			:	//裝備物件有問題
	case	EM_GameMessageEvent_EQError_WearPos_EQ				:	//裝備位置有問題
	case	EM_GameMessageEvent_EQError_DataInfo_EQ				:	//裝備資料有問題
	case	EM_GameMessageEvent_EQError_OrgObjID_EQ				:	//裝備物件有問題
	case	EM_GameMessageEvent_FriendList_DelError				:	//刪除好友有問題
	case	EM_GameMessageEvent_Guild_GuildNoFound				:	//自己沒有公會
	case	EM_GameMessageEvent_Guild_NoGuild					:	//沒有公會
	case	EM_GameMessageEvent_Guild_GuildIsReady				:	//已經是正式公會
	case	EM_GameMessageEvent_Guild_NotLeader					:	//非公會會長
	case	EM_GameMessageEvent_Guild_TargetNotOnline			:	//此人不在線上
	case	EM_GameMessageEvent_Guild_LeaderNotOnline			:	//公會會長不在線上
	case	EM_GameMessageEvent_Guild_GuildIsNotReady			:	//還在連署中
	case	EM_GameMessageEvent_Guild_NoInviteRight				:	//沒有邀請加入的權限
	case	EM_GameMessageEvent_Guild_InviterNotOnline			:	//邀請者不在線上
	case	EM_GameMessageEvent_Item_OrgObjIDError				:	//物件資料有問題
	case	EM_GameMessageEvent_Item_NotItem					:	//非物品
	case	EM_GameMessageEvent_Item_PosError					:	//要求放的位置有問題
	case	EM_GameMessageEvent_Item_Queue_PosError				:	//要求放的位置有問題
	case	EM_GameMessageEvent_Item_Queue_BufferEmpty			:	//物品序列內是空的
	case	EM_GameMessageEvent_Item_Queue_DataError			:	//物品資料錯誤
	case	EM_GameMessageEvent_Item_Queue_CountError			:	//物品數量錯誤
	case	EM_GameMessageEvent_Item_DelError					:	//資料刪除錯誤
	case	EM_GameMessageEvent_Item_ExchangeError				:	//物品交換有問題
	case	EM_GameMessageEvent_Item_BankOpenError				:	//銀行開啟狀況有問題
	case	EM_GameMessageEvent_Item_BankTooFar					:	//銀行距離過遠
	case	EM_GameMessageEvent_Item_ExchangeMoneyError			:	//金錢交換有問題
	case	EM_GameMessageEvent_Item_CountError					:	//數量有問題
	case	EM_GameMessageEvent_PK_PKError						:	//PK處理有問題
	case	EM_GameMessageEvent_PK_InviteError					:	//已要請其他目標
	case	EM_GameMessageEvent_PK_Prepar						:	//已在PK準備中
	case	EM_GameMessageEvent_PK_Begin						:	//已在PK中
	case	EM_GameMessageEvent_PK_TargetNotExist				:	//目標不存在
	case	EM_GameMessageEvent_PK_TargetTooFar					:	//目標過遠
	case	EM_GameMessageEvent_PK_TargetInviteOther			:	//目標要請其他人
	case	EM_GameMessageEvent_PK_TargetPK						:	//目標PK中
	case	EM_GameMessageEvent_PK_PKInviteResultError			:	//PK回應資料有問題
	case	EM_GameMessageEvent_Trade_ItemError					:	//交易物品有問題
	case	EM_GameMessageEvent_Trade_CanNotTrade				:	//物品不可交易
	case	EM_GameMessageEvent_Trade_CanNotModify				:	//交易確定後不可修改
	case	EM_GameMessageEvent_Trade_PosError					:	//放置位置有問題
	case	EM_GameMessageEvent_Trade_PosOnLock					:	//物品鎖定中
	case	EM_GameMessageEvent_Trade_MoneyError				:	//金錢為負的
	case	EM_GameMessageEvent_Trade_AccountError				:	//符文幣為負的
	case	EM_GameMessageEvent_Trade_MoneyNotEnough			:	//金錢不足
	case	EM_GameMessageEvent_Trade_AccountNotEnough			:	//點數不足
	case	EM_GameMessageEvent_Trade_Error						:	//交易失敗
	*/
	}
}
//-----------------------------------------------------------------------------
void NetCli_TalkChild::Rev_ScriptMessage	( int Sayer, int iTypeID, int iColorID, int iStringLen, const char* szString )
{
	EM_SCRIPTMESSAGE emType = (EM_SCRIPTMESSAGE)iTypeID;

	// 替換系統字串
	CRoleSprite* sprite = g_pGameMain->FindSpriteByID(Sayer);
	if (sprite && sprite->IsScriptHide())
		return;

	//char szReplaceString[4096];

	const char* key = "[$MUTE]";
	string sRangeString = CNetCli_Script_Child::ReplaceSystemKeyword( szString );	
	bool boMute = (strncmp(sRangeString.c_str(), "[$MUTE]", 7) == 0) ? true : false;	
	int len = (int)strlen(key);
	size_t begin;
	while ( (begin = sRangeString.find(key)) != string::npos )
		sRangeString.replace(begin, len, "");

		/*
		ContentSize = sRangeString.size();
		Content		= sRangeString.c_str();

		ZeroMemory( szReplaceString, sizeof( szReplaceString ) );
		memcpy( szReplaceString, Content, ContentSize );

		Content		= szReplaceString;
		ContentSize	= ContentSize + 1;
		*/


	char szTempString[4096];
	if( iColorID != 0 )
	{
		sprintf( szTempString, "%s|c%x%s|r", (boMute) ? key : "", iColorID, sRangeString.c_str() );
	}
	else
	{
		sprintf( szTempString, "%s%s", (boMute) ? key : "", sRangeString.c_str() );
	}

	switch( emType )
	{
	case EM_SCRIPTMESSAGE_WARNING:	// 出現在畫面中間的
		{
			g_pGameMain->SendWarningMsg( szTempString );
		} break;
	case EM_SCRIPTMESSAGE_NOTICE:	// 出現在畫面左下方 CHAT FRAME 的訓息,但不帶說話者
		{
			g_pChatFrame->SendMsgEvent( CHAT_MSG_SYSTEM, szTempString, "" );
		}
		break;
	case EM_SCRIPTMESSAGE_SYSTEM:
		{
			g_pGameMain->SendSystemMsg( szTempString );
		} break;
	case EM_SCRIPTMESSAGE_BOTH:
		{
			g_pGameMain->SendSystemMsg( szTempString );
			g_pGameMain->SendSystemChatMsg( szTempString );
		} break;
	}
}
void NetCli_TalkChild::Rev_RunningMsgEx( RunningMsgExTypeENUM Type , const char* Msg )
{
	lua_State* L = g_pGameMain->GetInterface()->GetUi()->GetLuaState();

	string strMsg = CNetCli_Script_Child::ReplaceSystemKeyword( Msg );
	const char* msg = strMsg.c_str();

	lua_pushstring( L, msg );
	lua_setglobal( L, "arg1" );


	//lua_pushstring( L, CNetCli_Script_Child::ReplaceSystemKeyword( Msg ).c_str() );

	if( Type == EM_RunningMsgExType_GM )
	{
		lua_pushstring( L, "gm" );
	}
	else if( Type == EM_RunningMsgExType_Monitor )
	{
		lua_pushstring( L, "monitor" );
	}
	else if( Type == EM_RunningMsgExType_Shop )
	{
		lua_pushstring( L, "shop" );
	}
	else
	{
 		lua_pushstring( L, "other" );
	}
	lua_setglobal( L, "arg2" );

	g_pInterface->SendWorldEvent( UiEvent_ScrollBannerMessageEX );

	g_pGameMain->SendSystemChatMsg( msg );
}

void NetCli_TalkChild::Rev_RunningMsg				( const char* Msg )
{
	string strMsg = CNetCli_Script_Child::ReplaceSystemKeyword( Msg );
	const char* msg = strMsg.c_str();

	g_pGameMain->SendScrollBannerMessage( msg );
}

void NetCli_TalkChild::Rev_OfflineMessage			( int FromDBID , int Time , const char* FromName  , const char* ToName , const char* Content )
{
	// 轉換字串
	g_pGameMain->MatchPatternString((BYTE*)Content);

	// 接收密語,黑名單除外
	if ( RoleData()->CheckIgnoreNameList(FromName) == false && !g_pChatEdit->IsUndesirable(FromName) )
	{
		g_pChatFrame->AddOfflineMessage(FromName, Content);
	}
}

void NetCli_TalkChild::Rev_BillBoardInfo			( int PlayerDBID , int DataSize , char* Data )
{
	g_pPartyBoardFrame->AddPost( PlayerDBID, DataSize, Data );
}

void NetCli_TalkChild::Rev_BillBoardDelete			( int PlayerDBID )
{
	g_pPartyBoardFrame->DeletePost( PlayerDBID );
}