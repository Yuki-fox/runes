#include <set>
#include "../mainproc/GameMain.h"
#include "../../netwaker_member/net_mail/NetCli_MailChild.h"
#include "../../netwaker_member/Net_Guild/NetCli_GuildChild.h"
#include "GuildFrame.h"
#include "ItemUtil.h"
#include "ItemClipboard.h"
#include "BagFrame.h"
#include "MailFrame.h"

CMailFrame*					g_pMailFrame = NULL;

int LuaFunc_CloseMail(lua_State* L);
int LuaFunc_ClickSendMailItemButton(lua_State* L);

// Send
int LuaFunc_SendMail(lua_State* L);
int	LuaFunc_SetSendMailMoney(lua_State* L);
int	LuaFunc_SetSendMailCOD(lua_State* L);
int LuaFunc_SetSnedMailPaper(lua_State* L);
int LuaFunc_SetSnedMailNextPaper(lua_State* L);
int LuaFunc_SetSnedMailPrevPaper(lua_State* L);
int LuaFunc_GetSnedMailPaper(lua_State* L);
int	LuaFunc_GetSendMailItem(lua_State* L);
int LuaFunc_GetSnedMailNumPapers(lua_State* L);
int LuaFunc_GetSendMailPaperEnable(lua_State* L);
int LuaFunc_GetSendMailMultiItemEnable(lua_State* L);

int LuaFunc_UpdateSendMailGroup(lua_State* L);
int LuaFunc_GetSendMailNumGroups(lua_State* L);
int LuaFunc_GetSendMailGroupInfo(lua_State* L);
int LuaFunc_SendGroupMail(lua_State* L);
int LuaFunc_GetSendGroupMailMoney(lua_State* L);

// Inbox
int LuaFunc_GetInboxNumItems(lua_State* L);
int LuaFunc_GetInboxHeaderInfo(lua_State* L);
int LuaFunc_InboxItemCanDelete(lua_State* L);
int LuaFunc_DeleteInboxItem(lua_State* L);
int LuaFunc_ReturnInboxItem(lua_State* L);
int	LuaFunc_GetInboxText(lua_State* L);
int LuaFunc_GetInboxItem(lua_State* L);
int	LuaFunc_TakeInboxItem(lua_State* L);
int LuaFunc_TakeInboxTextItem(lua_State* L);


// ----------------------------------------------------------------------------
CMailFrame::CMailFrame(CInterface* object)
	: CInterfaceBase(object)
{
	g_pMailFrame = this;

	m_mailNPCID			= 0;
	m_paperType			= 0;
	m_sendMailLocked	= false;
	m_inboxOpenLocked	= false;

	m_sendGruopLastTime	= 0.0f;
	Clear();
}

// ----------------------------------------------------------------------------
CMailFrame::~CMailFrame()
{
	g_pMailFrame = NULL;
}

// ----------------------------------------------------------------------------
void CMailFrame::RegisterFunc()
{
	lua_State* L = GetLuaStateWorld();
	if (L)
	{
		lua_register(L, "CloseMail",					LuaFunc_CloseMail);
		lua_register(L, "ClickSendMailItemButton",		LuaFunc_ClickSendMailItemButton);

		// Send Mail
		lua_register(L, "SendMail",						LuaFunc_SendMail);
		lua_register(L, "SetSendMailMoney",				LuaFunc_SetSendMailMoney);
		lua_register(L, "SetSendMailCOD",				LuaFunc_SetSendMailCOD);
		lua_register(L, "SetSnedMailPaper",				LuaFunc_SetSnedMailPaper);
		lua_register(L, "SetSnedMailNextPaper",			LuaFunc_SetSnedMailNextPaper);
		lua_register(L, "SetSnedMailPrevPaper",			LuaFunc_SetSnedMailPrevPaper);
		lua_register(L, "GetSnedMailPaper",				LuaFunc_GetSnedMailPaper);
		lua_register(L, "GetSendMailItem",				LuaFunc_GetSendMailItem);
		lua_register(L, "GetSnedMailNumPapers",			LuaFunc_GetSnedMailNumPapers);
		lua_register(L, "GetSendMailPaperEnable",		LuaFunc_GetSendMailPaperEnable);
		lua_register(L, "GetSendMailMultiItemEnable",	LuaFunc_GetSendMailMultiItemEnable);

		lua_register(L, "UpdateSendMailGroup",			LuaFunc_UpdateSendMailGroup);
		lua_register(L, "GetSendMailNumGroups",			LuaFunc_GetSendMailNumGroups);
		lua_register(L, "GetSendMailGroupInfo",			LuaFunc_GetSendMailGroupInfo);
		lua_register(L, "SendGroupMail",				LuaFunc_SendGroupMail);
		lua_register(L, "GetSendGroupMailMoney",		LuaFunc_GetSendGroupMailMoney);

		// Inbox		
		lua_register(L, "GetInboxNumItems",				LuaFunc_GetInboxNumItems);
		lua_register(L, "GetInboxHeaderInfo",			LuaFunc_GetInboxHeaderInfo);
		lua_register(L, "InboxItemCanDelete",			LuaFunc_InboxItemCanDelete);
		lua_register(L, "DeleteInboxItem",				LuaFunc_DeleteInboxItem);
		lua_register(L, "ReturnInboxItem",				LuaFunc_ReturnInboxItem);
		lua_register(L, "GetInboxText",					LuaFunc_GetInboxText);
		lua_register(L, "GetInboxItem",					LuaFunc_GetInboxItem);
		lua_register(L, "TakeInboxItem",				LuaFunc_TakeInboxItem);
	}
}

// ----------------------------------------------------------------------------
void CMailFrame::Update(float elapsedTime)
{
	static float lastTime = 0.0f;
	lastTime += elapsedTime;
	if ( lastTime < 0.2f || m_mailNPCID == 0 )
		return;

	lastTime = 0.0f;
	if ( g_pGameMain->GetDistanceWithPlayer(m_mailNPCID) > _MAX_ROLE_TOUCH_MAXRANGE_ )
	{
		SendWorldEvent("MAIL_CLOSED");
	}

	if ( m_sendGruopLastTime > 0.001f )
	{
		m_sendGruopLastTime -= elapsedTime;
		if ( m_sendGruopLastTime < 0.001f )
		{
			Clear();
			SendMailUnlocked();
			SendWorldEvent("MAIL_SEND_SUCCESS");
		}
	}
}

// ----------------------------------------------------------------------------
void CMailFrame::OpenMail(int NPCID)
{
	if ( NPCID == m_mailNPCID )
		return;

	m_mailNPCID = NPCID;
	m_paperType = 0;
	m_sendGruopLastTime = 0.0f;
	if ( !m_inboxOpenLocked )
	{
		m_inboxOpenLocked = true;
		NetCli_Mail::SL_MailList();

		// 重新更新群組資訊
		UpdateSendMailGroup();

		SendWorldEvent("MAIL_SHOW");
	}
}

// ----------------------------------------------------------------------------
void CMailFrame::UpdateSendMailGroup()
{
	m_sendMailGroup.clear();
	map<int, int> groupMap;

	// 好友
	FriendListStruct& FriendList = CNetGlobal::RoleData()->PlayerSelfData->FriendList;
	BaseFriendStruct* pList = NULL;
	int MaxCount = 0;
	CNetGlobal::RoleData()->PlayerSelfData->FriendList.GetListInfo( EM_FriendListType_Friend, &pList, MaxCount );
	for ( int i = 0; i < MaxCount; i++ )
	{
		if ( pList[i].IsEmpty() )
			continue;

		map<int, int>::iterator iter = groupMap.find(pList[i].GroupID);
		if ( iter != groupMap.end() )
			iter->second = iter->second + 1;
		else
			groupMap.insert({ pList[i].GroupID, 1 });
	}

	// 加入群組內
	for ( map<int, int>::iterator iter = groupMap.begin(); iter != groupMap.end(); iter++ )
	{
		SendMailGroupInfo info;
		info.type = eSendMailGroup_Friend;
		info.name = FriendList.GroupName[iter->first];
		for ( int i = 0; i < MaxCount; i++ )
		{
			if ( pList[i].IsEmpty() )
				continue;
			if ( pList[i].GroupID == iter->first )
				info.list.push_back(pList[i].Name);
		}
		m_sendMailGroup.push_back(info);
	}
	groupMap.clear();
	
	// 公會
	int guildCount = g_pGuildFrame->GetNumGuildMembers();
	for ( int i = 0 ; i < guildCount; i++ )
	{
		GuildMemberListInfo* info = g_pGuildFrame->GetGuildRosterInfo(i);
		if ( info )
		{ 
			GuildMemberBaseInfoStruct* pmember = info->member;
			if ( pmember )
			{
				map<int, int>::iterator iter = groupMap.find((int)pmember->Rank);
				if ( iter != groupMap.end() )
					iter->second = iter->second + 1;
				else
					groupMap.insert(make_pair((int)pmember->Rank, 1));
			}
		}
	}

	// 加入群組內
	for ( map<int, int>::iterator iter = groupMap.begin(); iter != groupMap.end(); iter++ )
	{
		SendMailGroupInfo info;
		info.type = eSendMailGroup_Guild;		

		lua_State* L = GetLuaState();
		if ( L )
		{
			lua_getglobal(L, "GF_GetRankStr");
			lua_pushnumber(L, iter->first + 1);
			lua_call(L, 1, 1);
			if ( lua_isstring(L, -1) )
				info.name = lua_tostring(L, -1);
		}

		for ( int i = 0 ; i < guildCount; i++ )
		{
			GuildMemberListInfo* rosterInfo = g_pGuildFrame->GetGuildRosterInfo(i);
			if ( rosterInfo )
			{ 
				GuildMemberBaseInfoStruct* pmember = rosterInfo->member;
				if ( pmember && pmember->Rank == iter->first )
				{
					// 自己不加入列表
					if ( strcmp(pmember->MemberName.Begin(), g_pGameMain->GetPlayerName()) != 0 )
					{
						info.list.push_back(pmember->MemberName.Begin());
					}
				}
			}
		}
		m_sendMailGroup.push_back(info);
	}
	groupMap.clear();
}

// ----------------------------------------------------------------------------
void CMailFrame::CloseMail()
{
	// 清除所有物品
	ClearSendMailItems();

	//SetSendMailItemIndex(-1);

	/*
	ItemFieldStruct* itemData = CNetGlobal::RoleData()->GetBodyItem(m_sendMailItemIndex);
	if ( itemData && !itemData->IsEmpty() )
		itemData->Pos = EM_ItemState_NONE;
	*/

	switch(g_pItemClipboard->GetType())
	{
	case eClipboardItem_Type_Bag:
		g_pItemClipboard->Clear();
		break;
	}

	m_mailNPCID = 0;
	m_paperType = 0;
	m_sendGruopLastTime = 0.0f;
	if ( m_inboxOpenLocked )
	{
		m_inboxOpenLocked = false;
		m_sendMailItemIndex = -1;
		NetCli_MailChild::ClearInboxList();
		NetCli_MailChild::SL_CloseMail();
	}
}

// ----------------------------------------------------------------------------
void CMailFrame::InboxUpdate()
{
	SendWorldEvent("MAIL_INBOX_UPDATE");
}

// ----------------------------------------------------------------------------
void CMailFrame::Clear()
{	
	m_isCOD				= 0;
	m_money				= 0;
	m_moneyAccount		= 0;
	m_sendGruopLastTime = 0.0f;

	for ( int i = 0; i < MAX_SEND_MAIL_ITEMS; i++ )
	{
		m_sendMailItem[i] = -1;
	}
}

// ----------------------------------------------------------------------------
void CMailFrame::SetMoney(bool isCOD, int money, int runeMoney)
{
	m_isCOD = isCOD;
	m_money = money;
	m_moneyAccount = runeMoney;
}

// ----------------------------------------------------------------------------
void CMailFrame::SetPaperType(int paperType)
{
	if ( paperType < EM_MailBackGroundType_Normal || paperType >= EM_MailBackGroundType_Max )
		paperType = EM_MailBackGroundType_Normal;
	else if ( paperType != EM_MailBackGroundType_Normal && CNetGlobal::RoleData()->PlayerBaseData->TimeFlag[EM_TimeFlagEffect_MailPaper] < 0 )
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_MAILPAPER_DONT_RENT"));		
		return;
	}
	m_paperType = paperType;
}

// ----------------------------------------------------------------------------
// 拿起要寄的物品
void CMailFrame::Pickup(int index)
{
	if ( index < 0 || index >= 10 )
		return;

	ClipboardItemInfo itemInfo = g_pItemClipboard->GetItemInfo();
	switch ( g_pItemClipboard->GetType() )
	{
	// pickup item
	case eClipboardItem_Type_None:
		// 有信件內容
		if ( m_sendMailItem[index] != -1 )
		{
			int pickIndex = m_sendMailItem[index];			

			SetSendMailItem(index, -1);			// 清除信件物品
			g_pBagFrame->Cut(pickIndex);		// 拿取物品
		}
		break;

	//
	case eClipboardItem_Type_Bag:
		{
			ItemFieldStruct* item = CNetGlobal::RoleData()->GetBodyItem(itemInfo.bag.pos);
			if ( g_ObjectData->GetItemFieldTradeState(item) == false )
			{
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_SENDMAIL_SOULBOUND"));
				return;
			}			
		}

		// 原本空的
		if ( m_sendMailItem[index] == -1 )
		{
			// 設定物品編號
			SetSendMailItem(index, itemInfo.bag.pos);

			// 清除物品索引值
			g_pBagFrame->ClearPickupItemIndex();

			// 清除
			g_pItemClipboard->Clear();
		}
		// 交換物品
		else
		{
			int pickIndex = m_sendMailItem[index];

			// 清除物品索引值
			// g_pBagFrame->ClearPickupItemIndex();

			// 設定物品編號
			SetSendMailItem(index, itemInfo.bag.pos);

			// 拿取物品
			g_pBagFrame->Cut(pickIndex);
		}
		break;

	default:
		break;
	}
}

// ----------------------------------------------------------------------------
// 擺放要寄出物品
// bagIndex 背包索引編號
void CMailFrame::PutDown(int bagIndex)
{
	/*
	if ( m_sendMailItemIndex != -1 ) {
		Pickup();
	} else {
		g_pBagFrame->ClearPickupItemIndex();
	}

	g_pBagFrame->Locked(bagIndex);

	SetSendMailItemIndex(bagIndex);
	*/
}

// ----------------------------------------------------------------------------
void CMailFrame::SendMail(const char* target, const char* subject, const char* body, const char* password)
{
	// SERVER DB前後空白會當成相同字串,需要去除空白
	char temp[1024];
	int length = (int)strlen(target);
	const char *begin = target;
	const char *end = target + length - 1;
	while ( *begin && (*begin == '\t' || *begin == ' ') ) begin++;
	while ( end > begin && (*end == '\t' || *end == ' ' || *end == '\n' || *end == 13) ) end--;

	int size = int(end - begin + 1);
	memcpy(temp, begin, size);
	temp[size] = 0;

	CRoleSprite* player = g_pGameMain->GetPlayer();
	if ( player && strcmp(temp, player->GetName()) == 0 )
	{
		const char* text = g_ObjectData->GetString("SYSTEM_MAIL_ERROR1");
		g_pGameMain->SendWarningMsg(text);
		return;
	}

	if ( !m_sendMailLocked )
	{
		MailBaseInfoStruct mailInfo;		
		
		mailInfo.IsSendMoney	= !m_isCOD;
		mailInfo.Money			= m_money;
		mailInfo.Money_Account	= 0;
		mailInfo.Money_Bonus	= 0;
		mailInfo.OrgTargetName	= target;
		mailInfo.Title			= subject;
		mailInfo.OrgSendName	= g_pGameMain->GetPlayerName();
		mailInfo.PaperType		= (MailBackGroundTypeENUM)m_paperType;

		// 物品資料往前堆
		int itemList[10];
		int count = 0;		
		for ( int i = 0; i < MAX_SEND_MAIL_ITEMS; i++ )
		{
			if ( m_sendMailItem[i] != -1 )
			{
				itemList[count] = m_sendMailItem[i];
				count++;
			}
		}
		for ( int i = count; i < MAX_SEND_MAIL_ITEMS; i++ )
			itemList[i] = -1;

		// 寄送10個包裏
		for ( int i = 0; i < MAX_SEND_MAIL_ITEMS; i++ )
		{
			mailInfo.Item[i].Init();
			if ( itemList[i] != -1 )
			{
				ItemFieldStruct* itemData = CNetGlobal::RoleData()->GetBodyItem(itemList[i]);
				if ( itemData )
					mailInfo.Item[i] = *itemData;
			}
		}
		m_sendMailLocked = true;
		NetCli_Mail::SL_SendMail(mailInfo, itemList, (char*)body, (char*)password);
	}
	else
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CANOT_DO_IT"));
	}
}

// ----------------------------------------------------------------------------
void CMailFrame::SendGroupMail(int group, const char* subject, const char* body)
{
	// 沒有目標可以寄送
	if ( m_sendMailGroup.empty() )
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_SENDMAIL_NO_GROUP"));
		return;
	}

	// 寄送群組錯誤
	if ( group < 0 || group >= (int)m_sendMailGroup.size() )
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_SENDMAIL_NO_GROUP"));
		return;
	}

	// 群組內沒有人
	if ( m_sendMailGroup[group].list.empty() )
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_SENDMAIL_NO_GROUP"));
		return;
	}

	if ( !m_sendMailLocked )
	{
		int targetCount = (int)m_sendMailGroup[group].list.size();
		StaticString<_MAX_NAMERECORD_SIZE_> targetName[20];

		// 檢查符文幣
		int money = g_ObjectData->GetMoneyKeyValue("MailUnit", 0);
		int rune = (targetCount+19 / 20) * money;

		if ( CNetGlobal::RoleData()->PlayerBaseData->Body.Money_Account < rune )		
		{
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_MAIL_NOT_ENOUGH_RUNEMONEY"));
			return;
		}

		int begin = 0;
		while ( begin < targetCount )
		{
			int count = 0;
			while ( count < 20 )
			{
				int index = count + begin;
				if ( index >= targetCount )
					break;
				targetName[count] = m_sendMailGroup[group].list[index].c_str();
				count++;
			}
			begin += 20;

			if ( count > 0 )
			{
				NetCli_Mail::SL_SendMailList(count, targetName, subject, (MailBackGroundTypeENUM)m_paperType, body);
			}
		}
		m_sendGruopLastTime = 0.8f;
		m_sendMailLocked = true;
	}
	else
	{
		g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_CANOT_DO_IT"));
	}
}

// ----------------------------------------------------------------------------
void CMailFrame::SetSendMailItemIndex(int index)
{
	if ( m_sendMailItemIndex != index )
	{	
		g_pBagFrame->Unlocked(m_sendMailItemIndex);				// 解除鎖定
		m_sendMailItemIndex = index;
		SendWorldEvent("MAIL_SEND_INFO_UPDATE");
	}
}

// ----------------------------------------------------------------------------
void CMailFrame::SetSendMailItem(int index, int bagPos)
{
	if ( index >= 0 && index < MAX_SEND_MAIL_ITEMS )
	{
		int oldIndex = m_sendMailItem[index];
		if ( oldIndex != bagPos )
		{

			if ( index > 0 && bagPos != -1 && CNetGlobal::RoleData()->PlayerBaseData->TimeFlag[EM_TimeFlagEffect_MailMultiItem] < 0 )
			{
				g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_MAILITEM_DONT_RENT"));
				return;
			}

			m_sendMailItem[index] = bagPos;

			// 解除鎖定
			if ( oldIndex != -1 )
				g_pBagFrame->Unlocked(oldIndex);

			// 鎖定物品
			if ( bagPos != -1 )
				g_pBagFrame->Locked(bagPos);

			// 更新送出包裏內容
			SendWorldEvent("MAIL_SEND_INFO_UPDATE");
		}
	}
}

// ----------------------------------------------------------------------------
void CMailFrame::ClearSendMailItems()
{
	for ( int i = 0; i < MAX_SEND_MAIL_ITEMS; i++ )
	{
		SetSendMailItem(i, -1);
	}
}

// ----------------------------------------------------------------------------
int CMailFrame::GetSendMailItem(int index)
{
	int bagIndex = -1;
	if ( index >= 0 && index < MAX_SEND_MAIL_ITEMS )
		bagIndex = m_sendMailItem[index];
	return bagIndex;
}

// ----------------------------------------------------------------------------
int LuaFunc_CloseMail(lua_State* L)
{
	g_pMailFrame->CloseMail();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_ClickSendMailItemButton(lua_State* L)
{	
	int index = 0;
	if ( lua_isnumber(L, 1) )
		index = (int)lua_tonumber(L, 1) - 1;

	g_pMailFrame->Pickup(index);
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_SendMail(lua_State* L)
{
	const char* target = luaL_checkstring(L, 1);
	const char* subject = luaL_checkstring(L, 2);
	const char* body = lua_isstring(L,3) ? lua_tostring(L, 3) : "";
	const char* password = lua_isstring(L,4) ? lua_tostring(L, 4) : "";
	g_pMailFrame->SendMail(target, subject, body, password);
	return 0;
}

// ----------------------------------------------------------------------------
int	LuaFunc_SetSendMailMoney(lua_State* L)
{
	g_pMailFrame->SetMoney(false, luaL_checkint(L, 1), luaL_checkint(L, 2));
	return 0;
}

// ----------------------------------------------------------------------------
int	LuaFunc_SetSendMailCOD(lua_State* L)
{
	g_pMailFrame->SetMoney(true, luaL_checkint(L, 1), luaL_checkint(L, 2));
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_SetSnedMailPaper(lua_State* L)
{
	int paper = EM_MailBackGroundType_Normal;
	if ( lua_isnumber(L, 1) )
		paper = (int)lua_tonumber(L, 1) - 1;
	g_pMailFrame->SetPaperType(paper);
	return 0;
}

// ----------------------------------------------------------------------------
// 下一個信件背景
int LuaFunc_SetSnedMailNextPaper(lua_State* L)
{
	int paper = g_pMailFrame->GetPaperType() + 1;
	if ( paper >= EM_MailBackGroundType_Max )
		paper = EM_MailBackGroundType_Normal;
	g_pMailFrame->SetPaperType(paper);
	return 0;
}

// ----------------------------------------------------------------------------
// 上一個信件背景
int LuaFunc_SetSnedMailPrevPaper(lua_State* L)
{
	int paper = g_pMailFrame->GetPaperType() - 1;
	if ( paper < EM_MailBackGroundType_Normal )
		paper = EM_MailBackGroundType_Max - 1;
	g_pMailFrame->SetPaperType(paper);
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetSnedMailPaper(lua_State* L)
{
	lua_pushnumber(L, g_pMailFrame->GetPaperType());
	return 1;
}

/*
// ----------------------------------------------------------------------------
// return	1.item name
//			2.item texture file name
//			3.item stack count
int	LuaFunc_GetSendMailItem(lua_State* L)
{	
	const char* name = NULL;
	const char* texture = "";
	int stack = 0;
	char temp[MAX_PATH];

	int itemIndex = g_pMailFrame->GetSendMailItemIndex();
	ItemFieldStruct* itemData = CNetGlobal::RoleData()->GetBodyItem(itemIndex);
	if ( itemData )
	{
		GameObjDbStruct* itemDB = CNetGlobal::GetObj(itemData->OrgObjID);
		RoleDataEx::GetItemFieldName( itemData, temp, MAX_PATH );
		if (itemDB)
		{
			name		= temp;
			texture		= itemDB->ImageObj->ACTField;
			stack		= itemData->Count;
		}
	}

	lua_pushstring(L, name);
	lua_pushstring(L, texture);
	lua_pushnumber(L, stack);
	return 3;
}
*/
// ----------------------------------------------------------------------------
// return	1.item name
//			2.item texture file name
//			3.item stack count
int	LuaFunc_GetSendMailItem(lua_State* L)
{	
	const char* name = NULL;
	const char* texture = "";
	int stack = 0;
	char temp[MAX_PATH];

	int itemIndex = -1;
	if ( lua_isnumber(L, 1) )
		itemIndex = g_pMailFrame->GetSendMailItem((int)lua_tonumber(L, 1) - 1);
	else
		itemIndex = g_pMailFrame->GetSendMailItem(0);

	ItemFieldStruct* itemData = CNetGlobal::RoleData()->GetBodyItem(itemIndex);
	if ( itemData )
	{
		GameObjDbStruct* itemDB = CNetGlobal::GetObj(itemData->OrgObjID);
		RoleDataEx::GetItemFieldName( itemData, temp, MAX_PATH );
		if (itemDB)
		{
			GameObjDbStruct* imageDB = CNetGlobal::GetObj(itemDB->ImageID);
			if ( imageDB )
			{
				texture	= imageDB->Image.ACTField;
			}

			name		= temp;			
			stack		= itemData->Count;
		}
	}

	lua_pushstring(L, name);
	lua_pushstring(L, texture);
	lua_pushnumber(L, stack);
	return 3;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetSnedMailNumPapers(lua_State* L)
{
	lua_pushnumber(L, EM_MailBackGroundType_Max);
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetSendMailPaperEnable(lua_State* L)
{
	lua_pushboolean(L, CNetGlobal::RoleData()->PlayerBaseData->TimeFlag[EM_TimeFlagEffect_MailPaper] >= 0);
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetSendMailMultiItemEnable(lua_State* L)
{
	lua_pushboolean(L, CNetGlobal::RoleData()->PlayerBaseData->TimeFlag[EM_TimeFlagEffect_MailMultiItem] >= 0);
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetSendMailNumGroups(lua_State* L)
{
	vector<SendMailGroupInfo>& groupInfo = g_pMailFrame->GetSendMailGroup();
	lua_pushnumber(L, (lua_Number)groupInfo.size());
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_UpdateSendMailGroup(lua_State* L)
{
	// g_pMailFrame->UpdateSendMailGroup();
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetSendMailGroupInfo(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;

	vector<SendMailGroupInfo>& groupInfo = g_pMailFrame->GetSendMailGroup();
	if ( index >= 0 && index < (int)groupInfo.size() )
	{
		lua_pushstring(L, groupInfo[index].name.c_str());				// 群組名稱
		lua_pushnumber(L, (lua_Number)groupInfo[index].list.size());				// 人數
		return 2;
	}
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_SendGroupMail(lua_State* L)
{
	int group = -1;
	if ( lua_isnumber(L, 1) )
		group = (int)lua_tonumber(L, 1) - 1;

	const char* subject = luaL_checkstring(L, 2);
	const char* body = lua_isstring(L,3) ? lua_tostring(L, 3) : "";
	g_pMailFrame->SendGroupMail(group, subject, body);

	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetSendGroupMailMoney(lua_State* L)
{
	int money = g_ObjectData->GetMoneyKeyValue("MailUnit", 0);
	lua_pushnumber(L, money);
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_GetInboxNumItems(lua_State* L)
{
	lua_pushnumber(L, NetCli_MailChild::GetInboxNumItems());
	return 1;
}

// ----------------------------------------------------------------------------
// return : packageIcon, sender, subject, money, CODAmount, daysLeft, hasItem, wasRead
int LuaFunc_GetInboxHeaderInfo(lua_State* L)
{		
	NetInboxInfo* header = NetCli_MailChild::GetInboxInfo(luaL_checkint(L, 1) - 1);
	if ( header )
	{	
		// 搜尋第一個物品
		const char* icon = NULL;
		int itemPosition = header->GetFirstItemIndex();
		if ( itemPosition != -1 && header->itemData[itemPosition].IsEmpty() == false )
		{
			if ( header->isGift )
				icon = "interface\\Icons\\shop_use_backpack_013.dds";
			else
				icon = ItemUTIL::GetItemIcon(header->itemData + itemPosition);
		}

		lua_pushstring(L, icon);								// icon
		lua_pushstring(L, header->sender.c_str());				// sender
		lua_pushstring(L, header->subject.c_str());				// subject
		lua_pushboolean(L, header->isCOD);						// isCOD

		if ( header->bonus > 0 )
		{
			lua_pushstring(L, MONEY_MODE_BONUS);
			lua_pushnumber(L, header->bonus);
		}
		else if ( header->moneyAccount > 0 )
		{
			lua_pushstring(L, MONEY_MODE_ACCOUNT);
			lua_pushnumber(L, header->moneyAccount);
		}
		else
		{
			lua_pushstring(L, MONEY_MODE_COPPER);
			lua_pushnumber(L, header->money);
		}
		
		lua_pushnumber(L, header->leftTime / (60.0 * 24.0));	// daysLeft
		lua_pushnumber(L, header->paperType);					// paper style
		lua_pushnumber(L, header->GetNumItems());				// has items
		lua_pushboolean(L, header->wasRead);					// wasRead
		lua_pushboolean(L, header->wasReturned);				// wasRetunred
		lua_pushboolean(L, !header->isSystem);					// canReply
		return 12;
	}	
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_InboxItemCanDelete(lua_State* L)
{
	lua_pushboolean(L, NetCli_MailChild::InboxItemCanDelete(luaL_checkint(L, 1) - 1));
	return 1;
}

// ----------------------------------------------------------------------------
int LuaFunc_DeleteInboxItem(lua_State* L)
{
	NetCli_MailChild::DeleteInboxItem(luaL_checkint(L, 1) - 1);
	return 0;
}

// ----------------------------------------------------------------------------
int LuaFunc_ReturnInboxItem(lua_State* L)
{
	NetCli_MailChild::ReturnInboxItem(luaL_checkint(L, 1) - 1);
	return 0;
}

// ----------------------------------------------------------------------------
int	LuaFunc_GetInboxText(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	NetInboxInfo* header = NetCli_MailChild::GetInboxInfo(index);
	NetCli_MailChild::ReadInboxItem(index);	

	if ( header )
	{
		lua_pushstring(L, header->content.c_str());
		return 1;
	}
	return 0;
}

// ----------------------------------------------------------------------------
// 取得來信內的物品資訊
int LuaFunc_GetInboxItem(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;

	NetInboxInfo* header = NetCli_MailChild::GetInboxInfo(index);
	NetCli_MailChild::ReadInboxItem(index);	
	if ( header )
	{
		int itemPosition = header->GetFirstItemIndex();
		if ( lua_isnumber(L, 2) )
			itemPosition = (int)lua_tonumber(L, 2) - 1;
		
		if ( itemPosition >= 0 && itemPosition < 10 )
		{
			ItemFieldStruct* item = header->itemData + itemPosition;
			if ( item->IsEmpty() == false )
			{
				const char* texture = ItemUTIL::GetItemIcon(item);
				if ( texture )
				{
					char name[MAX_PATH];
					g_ObjectData->GetItemFieldName(item, name, MAX_PATH);

					lua_pushstring(L, name);
					lua_pushstring(L, texture);
					lua_pushnumber(L, item->GetCount());
					return 3;
				}
			}
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------
int	LuaFunc_TakeInboxItem(lua_State* L)
{
	int index = luaL_checkint(L, 1) - 1;
	const char* password = lua_isstring(L, 2) ? lua_tostring(L, 2) : "";
	NetCli_MailChild::TakeInboxItem(index, password);
	return 0;
}