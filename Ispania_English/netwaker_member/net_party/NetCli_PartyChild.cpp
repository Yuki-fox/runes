#include "../mainproc/GameMain.h"
#include "../NetWaker_Member/NetWakerClientInc.h"
#include "../Interface/WorldFrames/ChatFrame.h"
#include "../Interface/WorldFrames/PartyFrame.h"
#include "../Interface/WorldFrames/UnitFrame.h"
#include "../Interface/WorldFrames/LootFrame.h"
#include "../Interface/WorldFrames/RaidTargetFrame.h"
#include "../Interface/ItemLink/ItemLink.h"
#include "../Interface/Login/AccountLogin.h"
#include "NetCli_PartyChild.h"
#include "../../tools/MString/MString.h"
#include "../Interface/PartyBoard/PartyBoardFrame.h"



//-----------------------------------------------------------------------------
PartyBaseInfoStruct				NetCli_PartyChild::s_info;

vector<PartyMemberInfo>			NetCli_PartyChild::s_member;
vector<PartyMemberInfo*>		NetCli_PartyChild::s_party;
vector<PartyAssignItem>			NetCli_PartyChild::s_assignItem;
vector<PartyRollItem>			NetCli_PartyChild::s_rollItem;
PartyStateInfo					NetCli_PartyChild::s_state;
string							NetCli_PartyChild::s_leaderName;
int								NetCli_PartyChild::s_playerNo;
map<int, PartyBaseValueQueue>	NetCli_PartyChild::s_baseValueQueue;

int									NetCli_PartyChild::s_BattleLeaderDBID;		// 戰場小隊領隊DBID
int									NetCli_PartyChild::s_BattlePartyNo;			// 玩家所在小隊編號
string								NetCli_PartyChild::s_BattleLeaderName;		// 戰場小隊領隊名稱
PartyStateInfo						NetCli_PartyChild::s_BattleState;			// 玩家戰場小隊狀態
BattleGroundPartyBaseData			NetCli_PartyChild::s_BattleInfo;			// 戰場小隊資訊
vector<BattleGroundPartyMember>		NetCli_PartyChild::s_BattleMember;			// 戰場隊員資訊
vector<BattleGroundPartyMember*>	NetCli_PartyChild::s_BattleParty;			// 戰場小隊成員

//-----------------------------------------------------------------------------
bool NetCli_PartyChild::_Init()
{
	if (This)
		return false;

	This = new NetCli_PartyChild;

	_Clear();

	return NetCli_Party::_Init();
}

//-----------------------------------------------------------------------------
bool NetCli_PartyChild::_Release()
{
	NetCli_Party::_Release();

	_Clear();

	if (This)
	{
		delete This;
		This = NULL;
	}
	return true;
}
//-----------------------------------------------------------------------------
void NetCli_PartyChild::_Clear()
{
	// 清除隊員資料
	vector<CRoleSprite*> deleteSprite;
	for ( vector<PartyMemberInfo>::iterator iter = s_member.begin(); iter != s_member.end(); iter++ )
		deleteSprite.push_back((CRoleSprite*)iter->sprite);

	s_member.clear();
	s_party.clear();
	s_playerNo			= -1;

	for ( vector<CRoleSprite*>::iterator iter = deleteSprite.begin(); iter != deleteSprite.end(); iter++ )
		delete (*iter);
	
	s_assignItem.clear();
	s_rollItem.clear();
	s_leaderName.clear();
	s_state				= ePartyState_Solo;
	s_info				= PartyBaseInfoStruct();

	s_baseValueQueue.clear();

	s_BattleLeaderDBID	= -1;		// 戰場小隊領隊DBID
	s_BattlePartyNo		= -1;		// 玩家所在小隊編號
	s_BattleState		= ePartyState_Solo;
	s_BattleInfo		= BattleGroundPartyBaseData();
	s_BattleMember.clear();
}
//-----------------------------------------------------------------------------
void NetCli_PartyChild::Update(float elapsedTime)
{
	for ( vector<PartyRollItem>::iterator iter = s_rollItem.begin(); iter != s_rollItem.end(); iter++ )
	{
		PartyRollItem& rollItem = *iter;
		switch ( rollItem.status )
		{
		case eLootItem_None:
			if ( g_pLootFrame->OpenNewLootItem(rollItem.itemVersion) )
				rollItem.status = eLootItem_WaitRoll;
			break;

		case eLootItem_WaitRoll:
		case eLootItem_Done:
			break;

		case eLootItem_Release:
			rollItem.destructTime -= elapsedTime;
			if ( rollItem.destructTime < 0.0f )
			{
				s_rollItem.erase(iter);
				return;
			}
			break;
		}
	}
}
//-----------------------------------------------------------------------------
// [0]第一位隊友 [1]第二 [2]....依序
PartyMemberInfo* NetCli_PartyChild::GetPartyMember(int index)
{
	if ( index >= 0 && index < (int)s_party.size() )
		return s_party[index];
	return NULL;
}

//-----------------------------------------------------------------------------
PartyMemberInfo* NetCli_PartyChild::GetRaidMember(int index)
{
	for ( vector<PartyMemberInfo>::iterator iter = s_member.begin(); iter != s_member.end(); iter++ )
	{
		if ( (*iter).raidNo == index )
			return &(*iter);
	}
	return NULL;
}

//-----------------------------------------------------------------------------
PartyMemberInfo* NetCli_PartyChild::GetGroupMember(int index)
{
	if ( index >= 0 && index < (int)s_member.size() )
		return &s_member[index];
	return NULL;
}

//-----------------------------------------------------------------------------
PartyMemberInfo* NetCli_PartyChild::GetGroupMemberByDBID(int DBID)
{
	for ( vector<PartyMemberInfo>::iterator iter = s_member.begin(); iter != s_member.end(); iter++ )
	{
		if ( (*iter).DBID == DBID )
			return &(*iter);
	}
	return NULL;
}
//-----------------------------------------------------------------------------
int NetCli_PartyChild::GetPartyLeaderIndex()
{
	int i = 0;
	for ( vector<PartyMemberInfo*>::iterator iter = s_party.begin(); iter != s_party.end(); iter++, i++ )
	{
		PartyMemberInfo* info = *iter;
		CRoleSprite* sprite = (CRoleSprite*)info->sprite;
		if ( sprite && s_info.LeaderName.Compare(sprite->GetName()) == 0 )
			break;
	}
	return i;
}
//-----------------------------------------------------------------------------
int	NetCli_PartyChild::GetRaidLeaderIndex()
{
	int i = 0;
	for ( vector<PartyMemberInfo>::iterator iter = s_member.begin(); iter != s_member.end(); iter++, i++ )
	{
		PartyMemberInfo& info = *iter;
		CRoleSprite* sprite = (CRoleSprite*)info.sprite;
		if ( sprite && s_info.LeaderName.Compare(sprite->GetName()) == 0 )
			break;
	}
	return i;
}
//-----------------------------------------------------------------------------
void NetCli_PartyChild::UpdateLeaderInfo()
{
	// 領隊名稱
	s_leaderName = s_info.LeaderName.Begin();

	// 更新隊長資料
	g_pPartyFrame->RefreshPartyLeader();

	// 判斷分流區域
	if ( g_pAccountLogin->GetParalleZoneID() != GetLeaderChannelID() )
	{
		g_pPartyFrame->SendWorldEvent("LEADER_CHANNEL_CONFIRM");
	}
}
//-----------------------------------------------------------------------------
void NetCli_PartyChild::ClearPartyInfo()
{
	vector<CRoleSprite*> deleteSprite;

	// 離開語音頻道.
    if (g_pPartyFrame != NULL)
    {
        if (g_pChatFrame->GetVoiceChannelType() == EVCT_PARTY)
        {
            g_pChatFrame->ExitVoiceChannel();
        }
    }
    else
    {
        // TODO: 暫時避開解構時期使用到空的 g_pPartyFrame 指標 (待修正) 
        char szDebug[256];
        szDebug[255] = '\0';
        _snprintf(szDebug, 255, "NetCli_PartyChild::ClearPartyInfo() : g_pPartyFrame == NULL");
        OutputDebugString(szDebug);
    }

	// 清除隊員資料
	for ( vector<PartyMemberInfo>::iterator iter = s_member.begin(); iter != s_member.end(); iter++ ) {
		CRoleSprite* sprite = (CRoleSprite*)(iter->sprite);		
		/*
		CRoleSprite* sprite = (CRoleSprite*)(iter->sprite);
		CRoleSprite* target = g_pGameMain->GetPlayerTarget();
		g_pGameMain->ClearFocusSprite(sprite);
		if ( sprite == target )
			g_pGameMain->SetPlayerTarget(NULL);
		else if ( target && target->GetTargetSprite() == sprite )
			target->SetTargetSprite(NULL);
		*/

		// 清除所有 Sprite 的指標
		IBaseSprite::Erase(sprite, true);

		// 清除 GameMain 內的所有 Sprite 指標
		g_pGameMain->SendDeleteRoleSprite(sprite);

		deleteSprite.push_back(sprite);
	}
	s_member.clear();
	s_party.clear();
	s_playerNo = -1;

	for ( vector<CRoleSprite*>::iterator iter = deleteSprite.begin(); iter != deleteSprite.end(); iter++ )
		delete (*iter);

	s_baseValueQueue.clear();
	s_leaderName.clear();
	s_state				= ePartyState_Solo;	

	ClearRollItem();
	ClearAssignItem();

	// 更新小隊成員
	if ( g_pPartyFrame )
	{
		g_pPartyFrame->RefreshPartyMember();
		g_pPartyFrame->RefreshPartyLeader();
	}

	if( g_pRaidTargetFrame )
		g_pRaidTargetFrame->ClearAllRaidTarget();

}
//-----------------------------------------------------------------------------
int	NetCli_PartyChild::InviteToParty(const char* name)
{
	if ( name == NULL )
	{
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_INVITE_ERR_NAME"));
		return -1;
	}

	// 防沉迷時間內不能發出邀請
	if( CNetGlobal::RoleData()->PlayerBaseData->PlayTimeQuota < 2*60*60 )
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_PLAYTIME_QOUTA_INVITE"));
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_PLAYTIME_QOUTA_INVITE"));
		return -1;
	}

	if ( g_pGameMain->IsBattleGroundZone() )
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_INVITE_ERR_INBATTLEGROUND"));
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_INVITE_ERR_INBATTLEGROUND"));
		return -1;
	}	

	if ( g_pGameMain->IsDisableParty() )
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_PARTY_INVITE_DISABLE"));
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_PARTY_INVITE_DISABLE"));
		return -1;
	}

	bool isAssistant = false;
	PartyMemberInfo* playerInfo = NetCli_PartyChild::GetGroupMemberByDBID( g_pGameMain->GetDBID() );
  	if ( playerInfo )
	{
		isAssistant = playerInfo->assistant;
	} 

	switch ( s_info.Type )
	{
	// 小隊
	case EM_PartyType_Party:
		if ( s_state != ePartyState_Member )
		{
			if ( s_member.size() >= MAX_PARTY_MEMBERS )
			{
				// 人數已滿
				g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, g_ObjectData->GetString("SYS_ERR_PARTY_2"), "");
				return -3;
			}

			// 榮譽組隊不能超過2個人
			if ( RoleData()->PlayerBaseData->TimeFlag[EM_TimeFlagEffect_HonorParty] >= 0 && s_member.size() >= 2 )
			{
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_HONOR_PARTY_ERR1"));
				g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_HONOR_PARTY_ERR1"));
				return -4;
			}

			S_Invite( (char*)name, RoleData()->PlayerBaseData->TimeFlag[EM_TimeFlagEffect_HonorParty] );

			// 顯示訊息
			g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_PARTY_INVITE_S"), name);
		}
		else
		{
			// 沒有權限
			g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, g_ObjectData->GetString("SYS_ERR_PARTY_1"), "");
			return -2;
		}
		break;

	case EM_PartyType_Raid:
		if ( s_state != ePartyState_Member || isAssistant )
		{
			if ( s_member.size() >= MAX_GROUP_MEMBERS )
			{
				// 人數已滿
				g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, g_ObjectData->GetString("SYS_ERR_PARTY_2"), "");
				return -3;
			}

			// 榮譽組隊不能超過2個人
			if ( RoleData()->PlayerBaseData->TimeFlag[EM_TimeFlagEffect_HonorParty] >= 0 && s_member.size() >= 2 )
			{
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_HONOR_PARTY_ERR1"));
				g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_HONOR_PARTY_ERR1"));
				return -4;
			}

			S_Invite( (char*)name, RoleData()->PlayerBaseData->TimeFlag[EM_TimeFlagEffect_HonorParty] );

			// 顯示訊息
			g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_RAID_INVITE_S"), name);
		}
		else
		{
			// 沒有權限
			g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, g_ObjectData->GetString("SYS_ERR_PARTY_1"), "");
			return -2;
		}
		break;
	}
	return 0;
}
//-----------------------------------------------------------------------------
int	NetCli_PartyChild::AskJoinToParty(char* name)	// Cli to Cli 申請加入隊伍(送出方)
{
	if ( name == NULL )
	{
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_INVITE_ERR_NAME"));
		return -1;
	}

	// 防沉迷時間內不能發出申請
	if( CNetGlobal::RoleData()->PlayerBaseData->PlayTimeQuota < 2*60*60 )
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_PLAYTIME_QOUTA_JOIN"));
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_PLAYTIME_QOUTA_JOIN"));
		return -1;
	}

	if ( g_pGameMain->IsBattleGroundZone() )
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_JOIN_ERR_INBATTLEGROUND"));
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_JOIN_ERR_INBATTLEGROUND"));
		return -1;
	}	

	if ( g_pGameMain->IsDisableParty() )
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_PARTY_JOIN_DISABLE"));
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_PARTY_JOIN_DISABLE"));
		return -1;
	}

	// 榮譽組隊狀態，無法申請加入隊伍。
	if ( RoleData()->PlayerBaseData->TimeFlag[EM_TimeFlagEffect_HonorParty] >= 0 )
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_HAS_HONOR_STATUS"));
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_HAS_HONOR_STATUS"));
		return -1;
	}

	switch ( s_state )
	{
	case ePartyState_Solo:
	case ePartyState_Receive:
		S_AskJoinToParty( name );
		break;

	case ePartyState_Leader:
	case ePartyState_Member:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_YOU_ALREADY_HAS_PARTY"));
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_YOU_ALREADY_HAS_PARTY"));
		break;	

	case ePartyState_Join:
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_PARTY_JOIN_BUSY"));
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_PARTY_JOIN_BUSY"));
		break;
	}

	return 0;
}
//-----------------------------------------------------------------------------
int	NetCli_PartyChild::AskJoinToPartyResult(char* name, bool bYesNo)	// Cli to Cli 申請加入隊伍的回應
{
	char buf[1024];	

	switch ( s_state )
	{	
	case ePartyState_Solo:
	case ePartyState_Receive:
	case ePartyState_Leader:
	case ePartyState_Member:
		return -2;
	
	case ePartyState_Join:
		// 因為ePartyState_Join是臨時狀態，所以在答覆後要回復原狀態。
		if( s_leaderName.compare( g_pGameMain->GetPlayerName() ) == 0 )
			s_state = ePartyState_Leader;
		else
			s_state = ePartyState_Solo;

		if( !bYesNo )
		{	// 本地顯示 "你拒絕了%s申請加入隊伍的請求。"
			g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_DECLINE_JOIN"), name);

			// 轉送 "[$VAR1]拒絕了你申請加入隊伍的請求。"
			SwapStringA(buf, "[%s][$SETVAR1=%s]", "SYS_DECLINE_JOIN_S", g_pGameMain->GetPlayerName());
			NetCli_Channel::SendToCliMessage(name, 0, buf);
		}

		S_AskJoinToPartyResult( name, bYesNo );

		return 0;
	}

	return -1;
}
//-----------------------------------------------------------------------------
void NetCli_PartyChild::DeletePBPost(char* name)
{
	S_DeletePBPost( name );
}
//-----------------------------------------------------------------------------
int	NetCli_PartyChild::InviteAnswerState(bool isAccept)
{
	char tmp[1024];
	switch (s_state)
	{
	case ePartyState_Solo:			// 未受到邀請
		return -2;

	case ePartyState_Receive:		// 是否同意加入隊伍 
		s_state = ePartyState_Solo;
		if ( isAccept )
		{
			S_Join( (char*)s_leaderName.c_str() );
		}
		else
		{
			const char* leaderName = NetCli_PartyChild::GetPartyLeaderName();

			// 本地顯示
			g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_DECLINE_GROUP_S"), leaderName);			

			// 轉送
			SwapStringA(tmp, "[%s][$SETVAR1=%s]", "SYS_DECLINE_GROUP", g_pGameMain->GetPlayerName());
			NetCli_Channel::SendToCliMessage(leaderName, 0, tmp);
		}
		return 0;

	case ePartyState_Leader:		// 已在隊伍中
	case ePartyState_Member:
		return -3;

	case ePartyState_Join:
		return -4;
	}
	return -1;
}

//-----------------------------------------------------------------------------
int	NetCli_PartyChild::PromoteToLeader(const char* name)
{
	if ( name == NULL )
		return -1;

	switch ( s_state )
	{
	case ePartyState_Leader:
		if ( RoleData()->PlayerBaseData->TimeFlag[EM_TimeFlagEffect_HonorParty] >= 0 )
		{
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_SELF_IN_HONOR_PARTY"));
			return -1;
		}
		else
		{
			PartyBaseInfoStruct info = s_info;
			info.LeaderName = name;

			NetCli_PartyChild::S_PartyRule( info );
		}
		break;

	default:
		return -1;
	}

	return 0;
}

//-----------------------------------------------------------------------------
int	NetCli_PartyChild::SetLootMethod(PartyItemShareENUM mode, const char* name)
{
	if ( s_state != ePartyState_Leader )
		return -1;

	if ( name == NULL )
		name = s_info.LeaderName.Begin();
	
	switch ( mode )
	{
	// 完全自由
	case EM_PartyItemShare_Roll:
		if ( s_info.ItemShareType != EM_PartyItemShare_Roll )
		{
			PartyBaseInfoStruct temp = s_info;
			temp.ItemShareType = EM_PartyItemShare_Roll;
			temp.RollTypeLV = 99;
			NetCli_PartyChild::S_PartyRule( temp );
			return 0;
		}
		break;

	// 隊長分配
	case EM_PartyItemShare_Assign:
		if ( s_info.ItemShareType != EM_PartyItemShare_Assign || s_info.ShareName.Compare( name ) != 0 )
		{
			PartyBaseInfoStruct temp = s_info;
			temp.ItemShareType = EM_PartyItemShare_Assign;
			temp.ShareName = name;			
			temp.RollTypeLV = 1;
			NetCli_PartyChild::S_PartyRule( temp );
		}
		return 0;

	// 輪流拾取
	case EM_PartyItemShare_InOrder:
		if ( s_info.ItemShareType != EM_PartyItemShare_InOrder )
		{
			PartyBaseInfoStruct temp = s_info;
			temp.ItemShareType = EM_PartyItemShare_InOrder;
			temp.RollTypeLV = 1;
			NetCli_PartyChild::S_PartyRule( temp );
			return 0;
		}
	}

	return -1;
}

//-----------------------------------------------------------------------------
int	NetCli_PartyChild::SetLootThreshold(int quality)
{
	if ( s_state != ePartyState_Leader )
		return -1;

	if ( s_info.RollTypeLV != quality && (s_info.ItemShareType == EM_PartyItemShare_Assign || s_info.ItemShareType == EM_PartyItemShare_InOrder) )
	{
		s_info.RollTypeLV = quality;
		NetCli_PartyChild::S_PartyRule( s_info );
	}
	return 0;
}

//-----------------------------------------------------------------------------
int NetCli_PartyChild::KickMember(const char* name)
{
	if ( name == NULL )
		return -1;

	switch ( s_state )
	{
	case ePartyState_Leader:
		S_KickMember( (char*)name , s_info.PartyID );
		break;

	case ePartyState_Member:
		if ( strcmp(name, g_pGameMain->GetPlayerName()) == 0 )
			S_KickMember( (char*)name , s_info.PartyID );
		break;

	default:
		return -1;
	}
	return 0;
}

//-----------------------------------------------------------------------------
const char* NetCli_PartyChild::FindMemberName(int DBID)
{
	for ( vector<PartyMemberInfo>::iterator iter = s_member.begin(); iter != s_member.end(); iter++ )
	{
		CRoleSprite* sprite = (CRoleSprite*)((*iter).sprite);
		if ( DBID == (*iter).DBID )
			return sprite->GetName();
	}
	return NULL;
}

//-----------------------------------------------------------------------------
bool NetCli_PartyChild::InParty(const char* name)
{
	if ( name == NULL )
		return false;	

	for ( vector<PartyMemberInfo*>::iterator iter = s_party.begin(); iter != s_party.end(); iter++ )
	{
		CRoleSprite* sprite = (CRoleSprite*)((*iter)->sprite);
		if ( strcmp( sprite->GetName(), name ) == 0 )
			return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
bool NetCli_PartyChild::InGroup(const char* name)
{
	if ( s_info.Type != EM_PartyType_Raid || name == NULL )
		return false;

	for ( vector<PartyMemberInfo>::iterator iter = s_member.begin(); iter != s_member.end(); iter++ )
	{
		CRoleSprite* sprite = (CRoleSprite*)((*iter).sprite);
		if ( strcmp( sprite->GetName(), name ) == 0 )
			return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
int NetCli_PartyChild::GetLeaderChannelID()
{
	int leaderChannelID = g_pAccountLogin->GetParalleZoneID();
	for ( vector<PartyMemberInfo>::iterator iter = s_member.begin(); iter != s_member.end(); iter++ )
	{
		CRoleSprite* sprite = (CRoleSprite*)(iter->sprite);
		if ( s_leaderName.compare(sprite->GetName()) == 0 )
		{
			leaderChannelID = iter->channelID;
			break;
		}
	}
	return leaderChannelID;
}

//-----------------------------------------------------------------------------
bool NetCli_PartyChild::SetInstanceLevel(int level)
{
	if ( s_state == ePartyState_Leader )
	{
		PartyBaseInfoStruct info = s_info;
		if ( info.InstanceLv != level )
		{
			info.InstanceLv = level;
			NetCli_PartyChild::S_PartyRule( info );
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
// 轉換為團隊
bool NetCli_PartyChild::SwitchToRaid()
{
	if ( s_state == ePartyState_Leader && s_info.Type == EM_PartyType_Party )
	{
		PartyBaseInfoStruct info = s_info;
		info.Type = EM_PartyType_Raid;

		NetCli_PartyChild::S_PartyRule( info );

		//g_pPartyFrame->RefreshPartyMember();
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
// 調整助理狀態
bool NetCli_PartyChild::SwitchRaidAssistant(int index, bool state)
{
	if ( s_state == ePartyState_Leader && s_info.Type == EM_PartyType_Raid )
	{
		PartyMemberInfo* info = GetRaidMember(index);
		if ( info )
		{
			PartyMemberModeStruct mode;
			mode._Mode = info->container;
			mode.IsAssistant = state;
			SChat_SetMemberMode(info->DBID, mode);
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
bool NetCli_PartyChild::SwitchRaidMainTank(int index, bool state)
{
	PartyMemberInfo* playerInfo = NetCli_PartyChild::GetGroupMemberByDBID( g_pGameMain->GetDBID() );

	if ( ( s_state == ePartyState_Leader || playerInfo->assistant ) && s_info.Type == EM_PartyType_Raid )
	{
		PartyMemberInfo* info = GetRaidMember(index);
		if ( info )
		{
			PartyMemberModeStruct mode;
			mode._Mode = info->container;
			mode.IsMainTank = state;

			if( mode.IsMainTank )
			{
				mode.IsMainAttack = false;
			}
   
			SChat_SetMemberMode(info->DBID, mode);
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
bool NetCli_PartyChild::SwitchRaidMainAttack(int index, bool state)
{
	PartyMemberInfo* playerInfo = NetCli_PartyChild::GetGroupMemberByDBID( g_pGameMain->GetDBID() );

	if ( ( s_state == ePartyState_Leader || playerInfo->assistant ) && s_info.Type == EM_PartyType_Raid )
	{
		PartyMemberInfo* info = GetRaidMember(index);
		if ( info )
		{
			PartyMemberModeStruct mode;
			mode._Mode = info->container;
			mode.IsMainAttack = state;

			if( mode.IsMainAttack )
			{
				mode.IsMainTank = false;
			}

			SChat_SetMemberMode(info->DBID, mode);
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
bool NetCli_PartyChild::RaidDegrade( int index )
{
	PartyMemberInfo* playerInfo = NetCli_PartyChild::GetGroupMemberByDBID( g_pGameMain->GetDBID() );

	if ( ( s_state == ePartyState_Leader || playerInfo->assistant ) && s_info.Type == EM_PartyType_Raid )
	{
		PartyMemberInfo* info = GetRaidMember(index);
		if ( info )
		{
			PartyMemberModeStruct mode;
			mode._Mode = info->container;

			if( NetCli_PartyChild::s_state == ePartyState_Leader ) 
			{
   				mode.IsAssistant = false;
			}
   
			mode.IsMainTank		 = false;		//坦克
			mode.IsMainAttack	 = false;		//主攻手

			SChat_SetMemberMode(info->DBID, mode);
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
// 更換位置
bool NetCli_PartyChild::MoveRaidMember(int no1, int no2)
{
	PartyMemberInfo* playerInfo = NetCli_PartyChild::GetGroupMemberByDBID( g_pGameMain->GetDBID() );

	if ( ( s_state == ePartyState_Leader || playerInfo->assistant ) && s_info.Type == EM_PartyType_Raid )
	{
		PartyMemberInfo* info1 = GetRaidMember(no1);
		PartyMemberInfo* info2 = GetRaidMember(no2);
		int DBID = -1;

		if ( info1 )
		{
			if ( info2 )
				DBID = info2->DBID;

			SChat_SetMemberPosRequest(info1->DBID, DBID, no2, no1);
			return true;
		}
	}	
	return false;
}

//-----------------------------------------------------------------------------
void NetCli_PartyChild::AddRollItem(PartyRollItem& rollItem)
{
	s_rollItem.push_back( rollItem );
}

//-----------------------------------------------------------------------------
void NetCli_PartyChild::DeleteRollItem(int itemVersion)
{
	for ( vector<PartyRollItem>::iterator iter = s_rollItem.begin(); iter != s_rollItem.end(); iter++ )
	{
		if ( (*iter).itemVersion == itemVersion )
		{
			// 刪除項目
			g_pLootFrame->DeleteLootItem( itemVersion );
			(*iter).status = eLootItem_Done;
			break;
		}
	}
}

//-----------------------------------------------------------------------------
void NetCli_PartyChild::ClearRollItem()
{
	for ( vector<PartyRollItem>::iterator iter = s_rollItem.begin(); iter != s_rollItem.end(); iter++ )
		g_pLootFrame->DeleteLootItem( (*iter).itemVersion );
	s_rollItem.clear();
}

//-----------------------------------------------------------------------------
PartyRollItem* NetCli_PartyChild::GetRollItem(int itemVersion)
{
	for ( vector<PartyRollItem>::iterator iter = s_rollItem.begin(); iter != s_rollItem.end(); iter++ )
	{
		if ( (*iter).itemVersion == itemVersion )
			return &(*iter);
	}
	return NULL;
}

//-----------------------------------------------------------------------------
void NetCli_PartyChild::AddAssignItem(PartyAssignItem& assignItem)
{
	s_assignItem.push_back(assignItem);
	g_pLootFrame->RefreshAssignItem();
}

//-----------------------------------------------------------------------------
void NetCli_PartyChild::DeleteAssignItem(int itemVersion)
{
	for ( vector<PartyAssignItem>::iterator iter = s_assignItem.begin(); iter != s_assignItem.end(); iter++ )
	{
		if ( (*iter).itemVersion == itemVersion )
		{
			s_assignItem.erase(iter);
			g_pLootFrame->RefreshAssignItem();
			break;
		}
	}
}

//-----------------------------------------------------------------------------
void NetCli_PartyChild::ClearAssignItem()
{
	s_assignItem.clear();
	g_pLootFrame->RefreshAssignItem();
}

//-----------------------------------------------------------------------------
PartyAssignItem* NetCli_PartyChild::GetAssignItem(int itemVersion)
{
	for ( vector<PartyAssignItem>::iterator iter = s_assignItem.begin(); iter != s_assignItem.end(); iter++ )
	{
		if ( (*iter).itemVersion == itemVersion )
			return &(*iter);
	}
	return NULL;
}

//-----------------------------------------------------------------------------
const char*	NetCli_PartyChild::GetAssignItemMember(int itemVersion, int index)
{
	PartyAssignItem* assign = GetAssignItem(itemVersion);
	if ( assign )
	{
		if ( (DWORD)index < (DWORD)assign->loot.size() )
		{
			return FindMemberName( assign->loot[index] );
		}
	}
	return NULL;
}

//-----------------------------------------------------------------------------
int NetCli_PartyChild::AssignTo(int itemVersion, int DBID)
{
	PartyAssignItem* assign = GetAssignItem(itemVersion);
	if ( assign )
	{
		for ( vector<int>::iterator iter = assign->loot.begin(); iter != assign->loot.end(); iter++ )
		{
			if ( *iter == DBID )
			{
				S_AssistItem(itemVersion, DBID);
				return 0;
			}
		}
	}
	return -1;
}

//-----------------------------------------------------------------------------
bool NetCli_PartyChild::IsAssigner()
{
	switch ( s_state )
	{
	case ePartyState_Leader:
	case ePartyState_Member:
		if ( s_info.ItemShareType == EM_PartyItemShare_Assign && s_info.ShareName.Compare( g_pGameMain->GetPlayerName() ) == 0 )
			return true;
		break;
	}
	return false;
}

//-----------------------------------------------------------------------------
void NetCli_PartyChild::SetPartyMemberBaseValue(int DBID, int HP, int MP, int SP, int SP_Sub)
{	
	map<int, PartyBaseValueQueue>::iterator iter = s_baseValueQueue.find(DBID);	
	if ( iter != s_baseValueQueue.end() ) {
		iter->second.baseValue[0] = HP;
		iter->second.baseValue[1] = MP;
		iter->second.baseValue[2] = SP;
		iter->second.baseValue[3] = SP_Sub;
	} else {
		PartyBaseValueQueue valueQueue;
		valueQueue.baseValue[0] = HP;
		valueQueue.baseValue[1] = MP;
		valueQueue.baseValue[2] = SP;
		valueQueue.baseValue[3] = SP_Sub;
		s_baseValueQueue.insert(make_pair(DBID, valueQueue));
	}
}

void NetCli_PartyChild::SetPartyMemberMaxBaseValue(int DBID, int HP, int MP, int SP, int SP_Sub)
{
	map<int, PartyBaseValueQueue>::iterator iter = s_baseValueQueue.find(DBID);	
	if ( iter != s_baseValueQueue.end() ) {
		iter->second.maxBaseValue[0] = HP;
		iter->second.maxBaseValue[1] = MP;
		iter->second.maxBaseValue[2] = SP;
		iter->second.maxBaseValue[3] = SP_Sub;
	} else {
		PartyBaseValueQueue valueQueue;
		valueQueue.maxBaseValue[0] = HP;
		valueQueue.maxBaseValue[1] = MP;
		valueQueue.maxBaseValue[2] = SP;
		valueQueue.maxBaseValue[3] = SP_Sub;
		s_baseValueQueue.insert(make_pair(DBID, valueQueue));
	}
}

//-----------------------------------------------------------------------------
void NetCli_PartyChild::ClearBattleGroup()
{
	if ( s_BattlePartyNo == -1 )
		return;

	s_BattleLeaderName.clear();
	s_BattleLeaderDBID	= -1;
	s_BattlePartyNo		= -1;
	s_BattleState		= ePartyState_Solo;	

	s_BattleMember.clear();
	s_BattleParty.clear();

	if ( g_pPartyFrame )
	{
		g_pPartyFrame->RefreshPartyMember();
		g_pPartyFrame->RefreshPartyLeader();
	}

	if( g_pRaidTargetFrame )
		g_pRaidTargetFrame->ClearAllRaidTarget();
}

//-----------------------------------------------------------------------------
void NetCli_PartyChild::InviteBattleGroup(int DBID)
{
	// 該區域內不能邀請組隊
	if ( g_pGameMain->IsDisableParty() )
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_PARTY_INVITE_DISABLE"));
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_PARTY_INVITE_DISABLE"));
		return;
	}

	// 非戰場內無法使用戰場組隊
	if ( g_pGameMain->IsBattleGroundZone() == false )
	{
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_ERR_PARTY_3"));
		return;
	}

	// 非隊長無法發出邀請
	if ( s_BattleState == ePartyState_Member )
	{
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_ERR_PARTY_1"));
		return;
	}

	// 成員是已經達到上限
	if ( s_BattleMember.size() >= MAX_GROUP_MEMBERS )
	{
		// 人數已滿
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_ERR_PARTY_2"));
		return ;
	}

	// 編號錯誤
	if ( DBID == -1 )
		return;

	NetCli_Party::S_Zone_Invite(DBID, 0);
}

//-----------------------------------------------------------------------------
// 同意加入隊伍
void NetCli_PartyChild::AcceptBattleGroup()
{
	switch (s_BattleState)
	{
	case ePartyState_Solo:			// 未受到邀請
		return;

	case ePartyState_Receive:		// 同意加入隊伍
		// 戰場內不能邀請組隊
		if ( g_pGameMain->IsDisableParty() )
		{
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_PARTY_INVITE_DISABLE"));
			g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_PARTY_INVITE_DISABLE"));
			return;
		}

		s_BattleState = ePartyState_Solo;

		S_Zone_Join( s_BattleLeaderDBID );
		break;

	case ePartyState_Leader:		// 已在隊伍中
	case ePartyState_Member:
		return;
	}
}

//-----------------------------------------------------------------------------
// 拒絕加入隊伍
void NetCli_PartyChild::DeclineBattleGroup()
{
	char buf[1024];

	switch (s_BattleState)
	{
	case ePartyState_Solo:			// 未受到邀請
		return;

	case ePartyState_Receive:		// 受到邀請
		// 戰場內不能邀請組隊
		if ( g_pGameMain->IsDisableParty() )
		{
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_PARTY_INVITE_DISABLE"));
			g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_PARTY_INVITE_DISABLE"));
			return;
		}

		// 清除受到邀請狀態
		s_BattleState = ePartyState_Solo;

		// 本地顯示
		g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_DECLINE_GROUP_S"), s_BattleLeaderName.c_str());

		// 轉送
		SwapStringA(buf, "[%s][$SETVAR1=%s]", "SYS_DECLINE_GROUP", g_pGameMain->GetPlayerName());
		NetCli_Channel::SendToCliMessage(s_BattleLeaderName.c_str(), 0, buf);
		break;

	case ePartyState_Leader:		// 已在隊伍中
	case ePartyState_Member:
		return;
	}
}

//-----------------------------------------------------------------------------
// 踢除隊友
void NetCli_PartyChild::UninviteBattleMember(int DBID, bool checkLeader)
{
	// 戰場隊伍才能調動(檢查是否在戰場內,並且可以組成隊伍)
	if ( g_pGameMain->IsBattleGroundZone() == false || g_pGameMain->IsDisableParty() )
	{
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_ERR_PARTY_1"));
		return;
	}

	// 領隊才有踢人權限
	if ( checkLeader && s_BattleState != ePartyState_Leader )
	{
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_ERR_PARTY_1"));
		return;
	}

	// 是否在隊友清單內
	for ( vector<BattleGroundPartyMember>::iterator iter = s_BattleMember.begin(); iter != s_BattleMember.end(); iter++ )
	{
		if ( iter->DBID == DBID )
		{
			NetCli_PartyChild::S_Zone_KickMember(DBID);
			return;
		}
	}
}

//-----------------------------------------------------------------------------
void NetCli_PartyChild::UpdateBattleMember()
{
	s_BattleParty.clear();
	s_BattlePartyNo = -1;

	int playerDBID = g_pGameMain->GetZoneDBID();

	// 取得玩家小隊編號
	for ( vector<BattleGroundPartyMember> ::iterator iter = s_BattleMember.begin(); iter != s_BattleMember.end(); iter++ )
	{
		if ( iter->DBID == playerDBID )
		{
			s_BattlePartyNo = iter->partyNo;
			break;
		}
	}
	
	// 取得同小隊成員
	for ( vector<BattleGroundPartyMember> ::iterator iter = s_BattleMember.begin(); iter != s_BattleMember.end(); iter++ )
	{
		if ( (iter->DBID != playerDBID) && (iter->partyNo / MAX_PARTY_MEMBERS) == (s_playerNo / MAX_PARTY_MEMBERS) )
		{
			s_BattleParty.push_back(&(*iter));
		}
	}

	// 更新隊員界面資料
	if ( g_pPartyFrame )
	{
		g_pPartyFrame->RefreshPartyMember();
	}
}

//-----------------------------------------------------------------------------
// 戰場玩家Sprite可能會在隊員加入才加入,因此當有物件加入時觸發改變隊員資訊
void NetCli_PartyChild::RefreshBattleMember(int DBID)
{
	for ( vector<BattleGroundPartyMember> ::iterator iter = s_BattleMember.begin(); iter != s_BattleMember.end(); iter++ )
	{
		if ( iter->DBID == DBID )
		{
			if ( !iter->isExists )
			{
				iter->isExists = true;

				if ( g_pPartyFrame )
				{
					// 更新隊員界面資料
					g_pPartyFrame->RefreshPartyMember();

					// 更新隊長資料
					if ( s_BattleLeaderDBID == DBID )
					{
						g_pPartyFrame->RefreshPartyLeader();
					}
				}
			}
			break;
		}
	}
}

//-----------------------------------------------------------------------------
BattleGroundPartyMember* NetCli_PartyChild::GetBattlePartyMember(int index)
{
	if ( index >= 0 && index < (int)s_BattleParty.size() )
		return s_BattleParty[index];
	return NULL;
}

//-----------------------------------------------------------------------------
BattleGroundPartyMember* NetCli_PartyChild::GetBattleRaidMember(int index)
{
	for ( vector<BattleGroundPartyMember>::iterator iter = s_BattleMember.begin(); iter != s_BattleMember.end(); iter++ )
	{
		if ( iter->partyNo == index )
			return &(*iter);
	}
	return NULL;
}

//-----------------------------------------------------------------------------
BattleGroundPartyMember* NetCli_PartyChild::GetBattleGroupMemberByDBID(int DBID)
{
	for ( vector<BattleGroundPartyMember>::iterator iter = s_BattleMember.begin(); iter != s_BattleMember.end(); iter++ )
	{
		if ( iter->DBID == DBID )
			return &(*iter);
	}
	return NULL;
}

//-----------------------------------------------------------------------------
void NetCli_PartyChild::MoveBattleMember(int no1, int no2)
{
	if ( no1 == no2 )
		return;

	// 戰場隊伍才能調動(檢查是否在戰場內,並且可以組成隊伍)
	if ( g_pGameMain->IsBattleGroundZone() == false || g_pGameMain->IsDisableParty() )
	{
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_ERR_PARTY_1"));
		return;
	}

	// 領隊才能調動隊伍
	if ( s_BattleState != ePartyState_Leader )
	{
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_ERR_PARTY_1"));
		return;
	}	

	BattleGroundPartyMember* info1 = GetBattleRaidMember(no1);
	BattleGroundPartyMember* info2 = GetBattleRaidMember(no2);
	int DBID1 = -1;
	int DBID2 = -1;

	if ( info1 )
		DBID1 = info1->DBID;

	if ( info2 )
		DBID2 = info2->DBID;

	S_Zone_SetMemberPosRequest(DBID1, no1, DBID2, no2);
}

//-----------------------------------------------------------------------------
void NetCli_PartyChild::PromoteToBattleLeader(int DBID)
{
	// 戰場隊伍才能調動(檢查是否在戰場內,並且可以組成隊伍)
	if ( g_pGameMain->IsBattleGroundZone() == false || g_pGameMain->IsDisableParty() )
	{
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_ERR_PARTY_1"));
		return;
	}

	// 領隊才能調動隊伍
	if ( s_BattleState != ePartyState_Leader )
	{
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_ERR_PARTY_1"));
		return;
	}

	S_Zone_ChangeLeader(DBID);

	/*
	int partyNo = -1;
	for ( vector<BattleGroundPartyMember>::iterator iter = s_BattleMember.begin(); iter != s_BattleMember.end(); iter++ )
	{
		if ( iter->DBID == DBID )
		{
			partyNo = iter->partyNo;
			break;
		}
	}

	// 編號錯誤
	if ( partyNo == -1 )
	{
		return;
	}

	MoveBattleMember(0, partyNo);
	*/
}

//-----------------------------------------------------------------------------
int NetCli_PartyChild::GetBattleLeaderPartyNo()
{
	int i = 0;
	for ( vector<BattleGroundPartyMember*>::iterator iter = s_BattleParty.begin(); iter != s_BattleParty.end(); iter++, i++ )
	{
		if ( (*iter)->DBID == s_BattleLeaderDBID )
			break;
	}
	return i;
}

//-----------------------------------------------------------------------------
int NetCli_PartyChild::GetBattleLeaderRaidNo()
{
	int i = 0;
	for ( vector<BattleGroundPartyMember>::iterator iter = s_BattleMember.begin(); iter != s_BattleMember.end(); iter++, i++ )
	{
		if ( iter->DBID == s_BattleLeaderDBID )
			break;
	}
	return i;
}

//-----------------------------------------------------------------------------
bool NetCli_PartyChild::InBattleParty(int DBID)
{
	for ( vector<BattleGroundPartyMember*>::iterator iter = s_BattleParty.begin(); iter != s_BattleParty.end(); iter++ )
	{
		if ( (*iter)->DBID == DBID )
			return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
bool NetCli_PartyChild::InBattleGroup(int DBID)
{
	for ( vector<BattleGroundPartyMember>::iterator iter = s_BattleMember.begin(); iter != s_BattleMember.end(); iter++ )
	{
		if ( iter->DBID == DBID )
			return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
void NetCli_PartyChild::R_Invite( char* LeaderName , int ClientInfo )
{
	char buf[1024];
	lua_State* L = g_pPartyFrame->GetLuaStateWorld();

	// 防沉迷時間內不接受邀請
	if( CNetGlobal::RoleData()->PlayerBaseData->PlayTimeQuota < 2*60*60 )
	{
		// 轉送
		SwapStringA(buf, "[%s][$SETVAR1=%s]", "SYS_PLAYTIME_QOUTA_DECLINE_PARTY", g_pGameMain->GetPlayerName());
		NetCli_Channel::SendToCliMessage(LeaderName, 0, buf);
		return;
	}

	// 戰場內不接受邀請
	if ( g_pGameMain->IsBattleGroundZone() )
	{
		// 轉送
		SwapStringA(buf, "[%s][$SETVAR1=%s]", "SYS_DECLINE_INBATTLEGROUND", g_pGameMain->GetPlayerName());
		NetCli_Channel::SendToCliMessage(LeaderName, 0, buf);
		return;
	}	

	// 該區域內不能組隊
	if ( g_pGameMain->IsDisableParty() )
	{
		// 轉送
		SwapStringA(buf, "[%s][$SETVAR1=%s]", "SYS_PARTY_RECIVE_DISABLE", g_pGameMain->GetPlayerName());
		NetCli_Channel::SendToCliMessage(LeaderName, 0, buf);
		return;
	}

	//榮譽組隊,自己不能超過30級
	if ( ClientInfo >= 0 )
	{
		int maxLevel = 0;
		for ( int i = 0; i < EM_Max_Vocation; i++ )
			maxLevel = max(maxLevel, CNetGlobal::RoleData()->PlayerBaseData->AbilityList[i].Level);

		if ( maxLevel >= 30 )
		{
			// 轉送
			SwapStringA(buf, "[%s][$SETVAR1=%s]", "SYS_PARTY_OVER_HONOR_LEVEL", g_pGameMain->GetPlayerName());
			NetCli_Channel::SendToCliMessage(LeaderName, 0, buf);
			return;
		}
	}
	// 有榮譽組隊旗標不能加入一般隊伍
	else if ( RoleData()->PlayerBaseData->TimeFlag[EM_TimeFlagEffect_HonorParty] >= 0 )
	{
		// 轉送
		SwapStringA(buf, "[%s][$SETVAR1=%s]", "SYS_PARTY_HAS_HONOR_STATUS", g_pGameMain->GetPlayerName());
		NetCli_Channel::SendToCliMessage(LeaderName, 0, buf);
		return;
	}

	switch ( s_state )
	{
	//
	case ePartyState_Solo:
		s_state = ePartyState_Receive;
		s_leaderName = LeaderName;
		g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_PARTY_INVITE"), LeaderName);

		lua_pushstring(L, s_leaderName.c_str()); 
		lua_setglobal(L, "arg1");

		// 正常模式組隊或榮譽組隊
		if ( ClientInfo == -1 )
			lua_pushnumber(L, 1);		//正常模式組隊
		else
			lua_pushnumber(L, 2);		//榮譽組隊
		lua_setglobal(L, "arg2");
		g_pPartyFrame->SendWorldEvent("PARTY_INVITE_REQUEST");		
		break;

	case ePartyState_Receive:
	case ePartyState_Join:
		// 轉送
		SwapStringA(buf, "[%s]", "SYS_PARTY_INVITE_BUSY");
		NetCli_Channel::SendToCliMessage(LeaderName, 0, buf);
		break;

	case ePartyState_Leader:
	case ePartyState_Member:
		SwapStringA(buf, "[%s][$SETVAR1=%s]", "SYS_ALREADY_HAS_PARTY", g_pGameMain->GetPlayerName());
		NetCli_Channel::SendToCliMessage(LeaderName, 0, buf);
		break;
	}
}
//-----------------------------------------------------------------------------
void NetCli_PartyChild::R_AskJoinToParty( char* szAskerName , SJobInfo sJobInfo )
{
	char buf[1024];
	lua_State* L = g_pPartyFrame->GetLuaStateWorld();

	// 防沉迷時間內不接受申請加入隊伍
	if( CNetGlobal::RoleData()->PlayerBaseData->PlayTimeQuota < 2*60*60 )
	{
		// 轉送
		SwapStringA(buf, "[%s][$SETVAR1=%s]", "SYS_PLAYTIME_QOUTA_DECLINE_JOIN", g_pGameMain->GetPlayerName());
		NetCli_Channel::SendToCliMessage(szAskerName, 0, buf);
		return;
	}

	// 戰場內不接受申請加入隊伍
	if ( g_pGameMain->IsBattleGroundZone() )
	{
		// 轉送
		SwapStringA(buf, "[%s][$SETVAR1=%s]", "SYS_DECLINE_JOIN_INBATTLEGROUND", g_pGameMain->GetPlayerName());
		NetCli_Channel::SendToCliMessage(szAskerName, 0, buf);
		return;
	}	

	// 該區域內不能組隊
	if ( g_pGameMain->IsDisableParty() )
	{
		// 轉送
		SwapStringA(buf, "[%s][$SETVAR1=%s]", "SYS_PARTY_ASK_JOIN_DISABLE", g_pGameMain->GetPlayerName());
		NetCli_Channel::SendToCliMessage(szAskerName, 0, buf);
		return;
	}
	
	// 榮譽組隊不能超過2個人
	if ( RoleData()->PlayerBaseData->TimeFlag[EM_TimeFlagEffect_HonorParty] >= 0 && s_member.size() >= 2 )
	{
		// 轉送
		SwapStringA(buf, "[%s][$SETVAR1=%s]", "SYS_HONOR_PARTY_ERR2", g_pGameMain->GetPlayerName());
		NetCli_Channel::SendToCliMessage(szAskerName, 0, buf);
		return;
	}

	if( s_info.Type == EM_PartyType_Party )
	{
		if( s_member.size() >= MAX_PARTY_MEMBERS )
		{
			if( g_pPartyBoardFrame->IsPostForRaid() )
				SwitchToRaid();
			else
			{
				// "隊伍人數已到上限"
				SwapStringA(buf, "[%s]", "SYS_ERR_PARTY_2");
				NetCli_Channel::SendToCliMessage(szAskerName, 0, buf);
				return;
			}
		}
	}	
	else if( s_info.Type == EM_PartyType_Raid )
	{
		if( s_member.size() >= MAX_GROUP_MEMBERS )
		{
			// "隊伍人數已到上限"
			SwapStringA(buf, "[%s]", "SYS_ERR_PARTY_2");
			NetCli_Channel::SendToCliMessage(szAskerName, 0, buf);
			return;
		}
	}

	switch ( s_state )
	{
	case ePartyState_Solo:
	case ePartyState_Leader:
		{
			s_state = ePartyState_Join;
			g_pPartyBoardFrame->SetAskerName( szAskerName );

			lua_State* L = g_pGameMain->GetInterface()->GetUi()->GetLuaState();

			lua_pushstring( L, szAskerName );
			lua_setglobal( L, "AskerName" );
			lua_pushstring( L, g_ObjectData->GetClassNameByID( sJobInfo.Voc ) );
			lua_setglobal( L, "JobVoc" );
			lua_pushnumber( L, sJobInfo.VocLV );
			lua_setglobal( L, "JobVocLV" );
			lua_pushstring( L, g_ObjectData->GetClassNameByID( sJobInfo.Voc_Sub ) );
			lua_setglobal( L, "JobVoc_Sub" );
			lua_pushnumber( L, sJobInfo.Voc_SubLV );
			lua_setglobal( L, "JobVoc_SubLV" );

			g_pInterface->SendWorldEvent( "ASK_JOIN_TO_PARTY" );
		}		
		break;

	case ePartyState_Receive:
	case ePartyState_Join:		// "對方已受到邀請，請稍候再試。"
		SwapStringA(buf, "[%s]", "SYS_PARTY_INVITE_BUSY");
		NetCli_Channel::SendToCliMessage(szAskerName, 0, buf);
		break;	

	case ePartyState_Member:	// "[$VAR1] 不是隊長，沒有權限接受申請。"
		SwapStringA(buf, "[%s][$SETVAR1=%s]", "SYS_ERR_PARTY_4", g_pGameMain->GetPlayerName());
		NetCli_Channel::SendToCliMessage(szAskerName, 0, buf);
		break;
	}
}
//-----------------------------------------------------------------------------
void NetCli_PartyChild::R_AskJoinToPartyResult( char* szLeaderName , bool bYesNo )
{
	char buf[1024];
	switch ( s_state )
	{
	case ePartyState_Solo:
		if ( bYesNo )
		{
			//s_state = ePartyState_Member;
			s_leaderName = szLeaderName;
			S_Join( (char*)s_leaderName.c_str() );
		}
		break;

	case ePartyState_Receive:
		if ( bYesNo )
		{
			// 本地顯示
			g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_DECLINE_GROUP_S"), s_leaderName.c_str());			

			// 轉送
			SwapStringA(buf, "[%s][$SETVAR1=%s]", "SYS_DECLINE_GROUP", g_pGameMain->GetPlayerName());
			NetCli_Channel::SendToCliMessage(s_leaderName.c_str(), 0, buf);

			g_pInterface->SendWorldEvent( "AUTO_HIDE_PARTY_INVITE" );
			
			//s_state = ePartyState_Member;
			s_leaderName = szLeaderName;
			S_Join( (char*)s_leaderName.c_str() );			
		}
		break;	

	case ePartyState_Leader:
	case ePartyState_Member:
		if( bYesNo )
		{
			SwapStringA(buf, "[%s][$SETVAR1=%s]", "SYS_ALREADY_HAS_PARTY", g_pGameMain->GetPlayerName());
			NetCli_Channel::SendToCliMessage(szLeaderName, 0, buf);
		}
		break;	

	case ePartyState_Join:
		if ( bYesNo )
		{
			string AskerName = g_pPartyBoardFrame->GetAskerName();

			// 本地顯示
			g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_DECLINE_JOIN"), AskerName.c_str());			

			// 轉送
			SwapStringA(buf, "[%s][$SETVAR1=%s]", "SYS_DECLINE_JOIN_S", g_pGameMain->GetPlayerName());
			NetCli_Channel::SendToCliMessage(AskerName.c_str(), 0, buf);

			g_pInterface->SendWorldEvent( "AUTO_HIDE_PARTY_JOIN" );

			//s_state = ePartyState_Member;
			s_leaderName = szLeaderName;
			S_Join( (char*)s_leaderName.c_str() );
		}
		break;
	}
}
//-----------------------------------------------------------------------------
void NetCli_PartyChild::R_DeletePBPost()
{
	NetCli_Talk::SChat_BillBoardLiveTime( 0 );
}
//-----------------------------------------------------------------------------
void NetCli_PartyChild::R_PartyBaseInfo( PartyBaseInfoStruct& Info )
{
	PartyBaseInfoStruct temp = s_info;
	s_info = Info;

	// 設定自己隊伍狀態
	if ( s_info.LeaderName.Compare(g_pGameMain->GetPlayerName()) == 0 )
		s_state = ePartyState_Leader;
	else
		s_state = ePartyState_Member;

	// 檢查隊長是否已更改
	if ( temp.LeaderName.Compare(Info.LeaderName.Begin()) != 0 )
	{
		// 隊長轉移時，更新組隊招募留言板的隊長資料
		if( s_state == ePartyState_Leader && s_leaderName.empty() != true )
			g_pPartyBoardFrame->UpdateLeaderName( temp.LeaderName.Begin() );

		g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_LEADER_CHANGE"), Info.LeaderName.Begin());
		UpdateLeaderInfo();
	}

	// 分配模式不相同,或者改變分配者
	if ( temp.PartyID == -1 || (temp.ItemShareType != Info.ItemShareType || (Info.ItemShareType == EM_PartyItemShare_Assign && Info.ShareName.Compare(temp.ShareName) != 0)) )
	{
		g_pLootFrame->SendWorldEvent("LOOT_METHOD_CHANGED");

		switch( Info.ItemShareType )
		{
		case EM_PartyItemShare_Roll:
			g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_LOOT_FREEFORALL"));
			break;

		case EM_PartyItemShare_InOrder:
			g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_LOOT_ALTERNATE"));
			break;				

		case EM_PartyItemShare_Assign:
			g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_LOOT_MASTER_LOOTER"), Info.ShareName.Begin());
			g_pLootFrame->RefreshAssignItem();
			break;
		}
	}

	// 沒有隊伍或者改變隊伍模式
	if ( temp.PartyID == -1 || temp.Type != Info.Type )
	{
		switch (Info.Type)
		{
		case EM_PartyType_Party:
			g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_PARTY_JOIN"));
			break;

		case EM_PartyType_Raid:
			g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_RAID_JOIN"));
			break;
		}
		g_pPartyFrame->RefreshPartyMember();
 	}

	// 沒有隊伍或者改變隊伍難度模式
	if ( temp.PartyID == -1 || temp.InstanceLv != Info.InstanceLv )
	{
		switch (Info.InstanceLv)
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

//-----------------------------------------------------------------------------
void NetCli_PartyChild::UpdatePartyMember()
{
	s_party.clear();
	s_playerNo = -1;

	int playerDBID = g_pGameMain->GetDBID();

	// 取得玩家小隊編號
	for ( vector<PartyMemberInfo>::iterator iter = s_member.begin(); iter != s_member.end(); iter++ )
	{
		if ( iter->DBID == playerDBID )
		{
			s_playerNo = iter->raidNo;
			break;
		}
	}

	// 取得同小隊成員
	for ( vector<PartyMemberInfo>::iterator iter = s_member.begin(); iter != s_member.end(); iter++ )
	{
		if ( (iter->DBID != playerDBID) && (iter->raidNo / MAX_PARTY_MEMBERS) == (s_playerNo / MAX_PARTY_MEMBERS) )
		{
			s_party.push_back(&(*iter));
		}
	}

	// 更新隊員界面資料
	if ( g_pPartyFrame )
	{
		g_pPartyFrame->RefreshPartyMember();
	}
}

//-----------------------------------------------------------------------------
void NetCli_PartyChild::R_AddMember	( PartyMemberInfoStruct& Member )
{
	// 改變角色隊友狀態
	CRoleSprite* sprite = g_pGameMain->FindSpriteByDBID(Member.DBID);
	if ( sprite )
		sprite->SetPartyState(true);	

	// 加入新隊友
	PartyMemberInfo info;
	memset(&info, 0, sizeof(info));

	sprite				= new CRoleSprite(false);
	info.sprite			= sprite;
	info.DBID			= Member.DBID;		
	info.memberId		= Member.MemberID;
	info.GItemID		= Member.GItemID;
	info.raidNo			= Member.PartyNo;
	info.assistant		= Member.Mode.IsAssistant;	
	info.tank			= Member.Mode.IsMainTank;
	info.attack			= Member.Mode.IsMainAttack;
	info.channelID		= Member.ParallelID;

	sprite->SetObjectType(eSpriteObject_Player);
	sprite->SetPartyState(true);
	sprite->SetName(Member.Name.Begin(), false);		
	sprite->SetDBID(Member.DBID);
	sprite->SetIdentify(Member.GItemID);
	sprite->SetSex((Sex_ENUM)Member.Sex);
	sprite->SetVocation((Voc_ENUM)Member.Voc, (Voc_ENUM)Member.Voc_Sub);
	sprite->SetLevel(Member.Level, Member.Level_Sub);

	map<int, PartyBaseValueQueue>::iterator iter = s_baseValueQueue.find(Member.DBID);
	if ( iter != s_baseValueQueue.end() ) {
		PartyBaseValueQueue& valueQueue = iter->second;

		int baseValue[2];
		g_pGameMain->TranslateBaseValue(sprite, valueQueue.baseValue[1], valueQueue.baseValue[2], valueQueue.baseValue[3], baseValue);
		sprite->SetHP(valueQueue.baseValue[0]);
		sprite->SetMP(baseValue[0]);
		sprite->SetSP(baseValue[1]);

		g_pGameMain->TranslateBaseValue(sprite, valueQueue.maxBaseValue[1], valueQueue.maxBaseValue[2], valueQueue.maxBaseValue[3], baseValue);
		sprite->SetMaxHP(valueQueue.maxBaseValue[0]);
		sprite->SetMaxMP(baseValue[0]);
		sprite->SetMaxSP(baseValue[1]);		

		s_baseValueQueue.erase(iter);
	}

	// 增加隊員
	s_member.push_back(info);
	g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_ADD_PARTY"), sprite->GetName());

	// 更新隊員資料
	UpdatePartyMember();

	// 新加入隊員是否為隊長
	if ( s_info.LeaderName.Compare(sprite->GetName()) == 0 )
	{
		UpdateLeaderInfo();					// 更新隊長資料
	}
	else if( s_info.LeaderName.Compare( g_pGameMain->GetPlayerName() ) == 0 )	// 有新隊員加入時，隊長要處理其組隊招募留言板的資料
	{
		g_pPartyBoardFrame->UpdateLeaderName( sprite->GetName() );
	}

	// 是否自己加入新隊伍  加入語音頻道	    
	if ( Member.DBID == g_pGameMain->GetDBID() )
	{
		e_VoiceChannelType eType = g_pChatFrame->GetVoiceChannelType();

		//目前沒有頻道 加入語音頻道	 
		if( eType == EVCT_NONE )
		{
			g_pChatFrame->JoinVoiceChannel( EVCT_PARTY , "" );
		}
	}
}
//-----------------------------------------------------------------------------
void NetCli_PartyChild::R_DelMember	( PartyMemberInfoStruct& Member )
{
	// g_pError->AddMessage(0, 0, "R_DelMember DBID = %d", Member.DBID);

	// 清除頭上名稱顏色
	CRoleSprite* sprite = g_pGameMain->FindSpriteByDBID(Member.DBID);
	if ( sprite )
		sprite->SetPartyState(false);
	
	// 若自己脫離隊伍
	if ( Member.DBID == g_pGameMain->GetDBID() )
	{
		// 其他玩家
		for ( vector<PartyMemberInfo>::iterator iter = s_member.begin(); iter != s_member.end(); iter++ ) {
			sprite = g_pGameMain->FindSpriteByDBID(iter->DBID);
			if ( sprite && sprite->GetPartyState() ) {
				sprite->SetPartyState(false);
			}
		}

		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_PARTY_LEAVE"));

		// 清除隊伍資訊
		ClearPartyInfo();
	}
	// 隊友脫離隊伍
	else
	{
		// 刪除隊友
		for ( vector<PartyMemberInfo>::iterator iter = s_member.begin(); iter != s_member.end(); iter++ )
		{
			sprite = (CRoleSprite*)iter->sprite;
			if ( iter->DBID == Member.DBID )
			{
				// 清除所有 Sprite 的指標
				IBaseSprite::Erase(sprite, true);

				// 清除 GameMain 內的所有 Sprite 指標
				g_pGameMain->SendDeleteRoleSprite(sprite);

				/*
				g_pGameMain->ClearFocusSprite(sprite);
				CRoleSprite* target = g_pGameMain->GetPlayerTarget();
				if ( sprite == target )
					g_pGameMain->SetPlayerTarget(NULL);
				else if ( target && target->GetTargetSprite() == sprite )
					target->SetTargetSprite(NULL);				
				*/

				// 移除單一成員
				s_member.erase(iter);
				delete sprite;

				// 更新隊員資料
				UpdatePartyMember();

				// 離退訊息
				g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_PARTY_UNINVITE"), Member.Name.Begin());

				// 成員解散訊息
				if ( s_member.empty() )
					g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, g_ObjectData->GetString("SYS_PARTY_DISBAND"), "");

				break;
			}
		}
	}
}
//-----------------------------------------------------------------------------
void NetCli_PartyChild::R_FixMember	( PartyMemberInfoStruct& Member )
{
	/*
	char Buf[1024];
	sprintf( Buf , "[隊伍資訊] : 修改成員 ID =%d  Name = %s" , Member.MemberID , Member.Name.Begin() );
	g_pChatFrame->AddMessage( 5, Buf );	
	*/

	//g_pError->AddMessage(0, 0, "[R_FixMember] MemberID=%d Name=%s GItemID=%x DBID=%d", Member.MemberID, Member.Name, Member.GItemID, Member.DBID);
	
	for ( vector<PartyMemberInfo>::iterator iter = s_member.begin(); iter != s_member.end(); iter++ )
	{
		PartyMemberInfo& info = (*iter);
		if ( info.DBID == Member.DBID )
		{
			CRoleSprite* sprite;
			info.memberId	= Member.MemberID;
			info.GItemID	= Member.GItemID;
			info.raidNo		= Member.PartyNo;
			info.assistant	= Member.Mode.IsAssistant;	
			info.tank		= Member.Mode.IsMainTank;
			info.attack		= Member.Mode.IsMainAttack;
			info.channelID	= Member.ParallelID;

			sprite = (CRoleSprite*)(info.sprite);
			sprite->SetName(Member.Name.Begin());
			sprite->SetSex((Sex_ENUM)Member.Sex);
			sprite->SetVocation((Voc_ENUM)Member.Voc, (Voc_ENUM)Member.Voc_Sub);
			sprite->SetLevel(Member.Level, Member.Level_Sub);
			sprite->SetIdentify(Member.GItemID);

			// 更新隊員資料
			UpdatePartyMember();
			break;
		}
	}
}
//-----------------------------------------------------------------------------
void NetCli_PartyChild::R_PartyTreasure( int ItemVersion, PartyItemShareENUM LootType, ItemFieldStruct& Item, StaticVector< int , _MAX_PARTY_COUNT_ >& LootDBID )
{
	GameObjDbStruct* pObj = CNetGlobal::GetObj( Item.OrgObjID );
	if( pObj == NULL )
	{
		//g_pChatFrame->AddMessage( 5, "[隊員物品] : R_PartyTreasure pObj == NULL" );
		return;
	}

	char msg[1024];
	string itemName;
	if ( ItemField_To_ItemLink(&Item, itemName) == false )
		itemName.clear();

	sprintf( msg, "%s %s", g_ObjectData->GetString("SYS_PARTY_TREASURE"), itemName.c_str() );
	g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, msg, "");

	// 搜尋是否為唯一物品
	bool unique = false;
	GameObjDbStruct* itemDB = g_ObjectData->GetObj(Item.OrgObjID);
	if ( itemDB && itemDB->Mode.Unique )
	{
		unique = true;
	}

	/*
	RoleDataEx::GetItemFieldName(&Item, itemName, MAX_PATH);
	sprintf( msg, "%s[%s]", g_ObjectData->GetString("SYS_PARTY_TREASURE"), itemName );
	g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, msg, "");
	*/

	// 隊伍護得物品
	// 判斷物品取得方式,投擲或分配或輪流取得
	switch ( LootType )
	{
	case EM_PartyItemShare_InOrder:
	case EM_PartyItemShare_Roll:
		// debug
		for ( int i = 0; i < LootDBID.Size(); i++ )
		{
			const char* name = "";
			CRoleSprite* sprite = g_pGameMain->FindSpriteByDBID(LootDBID[i]);
			if ( sprite )
				name = sprite->GetName();

			// g_pError->AddMessage(0, 0, "[LootList]name=%s ID=%d", name, LootDBID[i]);
		}

		for ( int i = 0; i < LootDBID.Size(); i++ )
		{
			if ( LootDBID[i] == g_pGameMain->GetDBID() )
			{
				PartyRollItem roll;
				roll.time			= 5.0f * 60.0f * 60.0f;		// _MAX_ITEM_LOOT_TIME_ 10分鐘
				roll.itemVersion	= ItemVersion;
				roll.item			= Item;
				roll.status			= eLootItem_None;

				// 該物品為唯一物品並且已經擁有
				if ( unique && CNetGlobal::RoleData()->CheckItemInBody(Item.OrgObjID) )
				{
					NetCli_PartyChild::S_PassItem(ItemVersion);
				}
				else
				{
					AddRollItem(roll);
				}
				break;
			}
		}
		break;

	case EM_PartyItemShare_Assign:
		{			
			PartyAssignItem assign;
			assign.itemVersion	= ItemVersion;
			assign.item			= Item;

			for ( int i = 0; i < LootDBID.Size(); i++ )
				assign.loot.push_back(LootDBID[i]);

			AddAssignItem( assign );
		}
		break;
	}
}
//-----------------------------------------------------------------------------
void NetCli_PartyChild::R_PartyTreasureLoot( int ItemVersion ,	int DBID , int LootValue )
{
	/*
	char Buf[1024];
	sprintf( Buf , "[隊員物品] : 執骰子  ItemVersion = %d DBID =%d LootValue = %d " , ItemVersion , DBID , LootValue );
	g_pChatFrame->AddMessage( 5, Buf );
	*/

	const char* name = FindMemberName(DBID);
	if ( name == NULL )
	{
		//g_pChatFrame->SendMsgEvent("CHAT_MSG_SYSTEM", "[封包錯誤][R_PartyTreasureLoot] 該角色DBID不存在", "");
		return;
	}
	else
	{
		PartyRollItem* rollItem = GetRollItem(ItemVersion);
		if ( rollItem == NULL )
		{
			//g_pChatFrame->SendMsgEvent("CHAT_MSG_SYSTEM", "[封包錯誤][R_PartyTreasureLoot] 該物品ItemVersion不存在", "");
			return;
		}
		else
		{
			char msg[1024];
			string itemName;
			if ( ItemField_To_ItemLink(&rollItem->item, itemName) == false )
				itemName.clear();

			if ( LootValue >= 100 ) {
				sprintf( msg, "[%s] : %s%s %d", name, g_ObjectData->GetString("SYS_NEED"), itemName.c_str(), LootValue - 100 + 1);
			} else if ( LootValue >= 0 ) {
				sprintf( msg, "[%s] : %s%s %d", name, g_ObjectData->GetString("SYS_GREED"), itemName.c_str(), LootValue + 1);
			} else {
				sprintf( msg, "[%s] : %s%s", name, g_ObjectData->GetString("SYS_GIVE_UP"), itemName.c_str());
			}
			g_pChatFrame->SendMsgEvent("CHAT_MSG_SYSTEM", msg, "");
		}

		if ( DBID == g_pGameMain->GetDBID() )
		{
			rollItem->status = eLootItem_Done;
			g_pLootFrame->DeleteLootItem(ItemVersion);
		}
	}
}
//-----------------------------------------------------------------------------
void NetCli_PartyChild::R_GiveItemInfo( int DBID , ItemFieldStruct& Item , int ItemVersion )
{
	const char* playerName = FindMemberName(DBID);
	if ( playerName == NULL ) {
		//g_pChatFrame->AddMessage( 0, "[封包錯誤][R_GiveItemInfo] 該角色DBID不存在" );
	} else if ( DBID != g_pGameMain->GetDBID() ) {
		string itemName;
		if ( ItemField_To_ItemLink(&Item, itemName) == false )
			itemName.clear();
		g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_PARTY_GET_ITEM"), playerName, itemName.c_str());
	}

	// 物品分配給某一位隊員
	// 將物品至隊伍暫存區移除
	DeleteAssignItem(ItemVersion);

	PartyRollItem* rollItem = GetRollItem(ItemVersion);
	if ( rollItem )
	{
		rollItem->destructTime = 1.0f;
		rollItem->status = eLootItem_Release;
		g_pLootFrame->DeleteLootItem(ItemVersion);
	}	
}
void NetCli_PartyChild::OnServerDisConnect		( int ServerID )
{
	//Chat 斷線
	if( ServerID != _ServerNetID.m_iChat )
		return;

	ClearPartyInfo();	

}
// 某一玩家離線更新
void NetCli_PartyChild::R_MemberLogout			( int MemberDBID )
{
	// g_pChatFrame->AddMessage( 0, "R_MemberLogout Member logout!!" );

	for ( vector<PartyMemberInfo>::iterator iter = s_member.begin(); iter != s_member.end(); iter++ )
	{
		PartyMemberInfo& info = (*iter);
		if ( info.DBID == MemberDBID )
		{
			info.GItemID = -1;
			break;
		}
	}
	g_pPartyFrame->RefreshPartyMember();
}

void NetCli_PartyChild::R_SetMemberPosResult	( bool Result )
{
	if ( Result )
	{
		// 團隊成員更換位置成?
	}
	else
	{
		// 團隊成員更換位置失敗
	}
}

void NetCli_PartyChild::R_Zone_PartyBase	( int PartyID , int LeaderDBID )
{
	s_BattleInfo.partyID = PartyID;
	s_BattleInfo.leaderDBID = LeaderDBID;

	// 設定自己隊伍狀態
	if ( s_BattleInfo.leaderDBID == g_pGameMain->GetZoneDBID() )
		s_BattleState = ePartyState_Leader;
	else
		s_BattleState = ePartyState_Member;

	// 檢查隊長是否已更改
	if ( s_BattleLeaderDBID != LeaderDBID )
	{		
		CRoleSprite* sprite = g_pGameMain->FindBettleMemberByDBID(LeaderDBID);
		if ( sprite == NULL )
			s_BattleLeaderName.clear();
		else
			s_BattleLeaderName = sprite->GetName();

		s_BattleLeaderDBID = LeaderDBID;

		g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_LEADER_CHANGE"), s_BattleLeaderName.c_str());

		// 更新隊長資料
		g_pPartyFrame->RefreshPartyLeader();
	}
}

void NetCli_PartyChild::R_Zone_Invite		( int LeaderDBID , int ClientInfo )
{
	char buf[1024];
	lua_State* L = g_pPartyFrame->GetLuaStateWorld();	

	const char* leaderName = NULL;	
	CRoleSprite* sprite = g_pGameMain->FindBettleMemberByDBID(LeaderDBID);
	if ( sprite )
	{
		leaderName = sprite->GetName();
	}
	else
	{
		leaderName = "";
	}

	// 該區域內不能組隊
	if ( g_pGameMain->IsDisableParty() )
	{
		// 轉送
		SwapStringA(buf, "[%s][$SETVAR1=%s]", "SYS_PARTY_RECIVE_DISABLE", g_pGameMain->GetPlayerName());
		NetCli_Channel::SendToCliMessage(leaderName, 0, buf);
		return;
	}
	
	// 隊伍狀態
	switch ( s_BattleState )
	{
	// 
	case ePartyState_Solo:
		s_BattleState = ePartyState_Receive;
		s_BattleLeaderName = leaderName;
		s_BattleLeaderDBID = LeaderDBID;

		// 送出被邀請訊息
		g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_PARTY_INVITE"), leaderName);		

		lua_pushstring(L, s_BattleLeaderName.c_str());
		lua_setglobal(L, "arg1");
		lua_pushnumber(L, 3);
		lua_setglobal(L, "arg2");
		g_pPartyFrame->SendWorldEvent("PARTY_INVITE_REQUEST");		
		break;

	case ePartyState_Receive:
		// 轉送
		SwapStringA(buf, "[%s]", "SYS_PARTY_INVITE_BUSY");
		NetCli_Channel::SendToCliMessage(leaderName, 0, buf);
		break;

	case ePartyState_Leader:
	case ePartyState_Member:
		SwapStringA(buf, "[%s][$SETVAR1=%s]", "SYS_ALREADY_HAS_PARTY", g_pGameMain->GetPlayerName());
		NetCli_Channel::SendToCliMessage(leaderName, 0, buf);
		break;	
	}
}
void NetCli_PartyChild::R_Zone_AddMember	( int PartyID , int MemberDBID , int PartyNo , int MemberID )
{
	BattleGroundPartyMember member;
	member.partyID		= PartyID;
	member.DBID			= MemberDBID;
	member.memberID		= MemberID;
	member.partyNo		= PartyNo;
	member.isExists		= g_pGameMain->FindBettleMemberByDBID(MemberDBID) != NULL;	
	s_BattleMember.push_back(member);

	UpdateBattleMember();
}
void NetCli_PartyChild::R_Zone_DelMember	( int MemberDBID )
{
	// 若自己脫離隊伍
	if ( g_pGameMain->GetZoneDBID() == MemberDBID )
	{
		// 其他玩家

		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_PARTY_LEAVE"));

		// 清除隊伍資訊
		ClearBattleGroup();
	}
	// 隊友脫離隊伍
	else
	{
		// 刪除隊友
		for ( vector<BattleGroundPartyMember>::iterator iter = s_BattleMember.begin(); iter != s_BattleMember.end(); iter++ )
		{
			if ( iter->DBID == MemberDBID )
			{
				s_BattleMember.erase(iter);				

				// 更新隊員資料
				UpdateBattleMember();

				CRoleSprite* sprite = g_pGameMain->FindBettleMemberByDBID(MemberDBID);
				if ( sprite )
				{
					// 離退訊息
					g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_PARTY_UNINVITE"), sprite->GetName());
				}				

				// 成員解散訊息
				if ( s_BattleMember.empty() )
					g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, g_ObjectData->GetString("SYS_PARTY_DISBAND"), "");

				break;
			}
		}
	}
}
void NetCli_PartyChild::R_Zone_FixMember	( int PartyID , int MemberDBID , int PartyNo , int MemberID )
{
	for ( vector<BattleGroundPartyMember>::iterator iter = s_BattleMember.begin(); iter != s_BattleMember.end(); iter++ )
	{
		if ( iter->DBID == MemberDBID )
		{
			iter->partyNo	= PartyNo;
			iter->memberID	= MemberID;
			iter->partyID	= PartyID;

			UpdateBattleMember();
			break;
		}
	}
}
void NetCli_PartyChild::R_Zone_SetMemberPosRequest	( bool Result )
{

}
void NetCli_PartyChild::R_Zone_ChangeLeaderResult( bool Result )
{

}

void NetCli_PartyChild::R_Zone_Talk		( int GItemID , const char* Content )
{
	CRoleSprite* sprite = g_pGameMain->FindBettleMemberByGItemID(GItemID);
	if ( sprite )
	{
		g_pChatFrame->SendChatMessage(CHAT_MSG_BG, sprite->GetName(), Content);
	}
}

void NetCli_PartyChild::R_GetMemberInfoResult( int LeaderDBID , vector< PartyMemberInfoStruct > & List )
{
	g_pPartyBoardFrame->AnalyzeRaidInfo( LeaderDBID, List );
}