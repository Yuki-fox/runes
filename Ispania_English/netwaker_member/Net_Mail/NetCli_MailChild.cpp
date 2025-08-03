#include "../MainProc/GameMain.h"
#include "../Net_Script/NetCli_Script_Child.h"
#include "../Interface/WorldFrames/ChatFrame.h"
#include "../Interface/WorldFrames/MailFrame.h"
#include "../Interface/ItemLink/ItemLink.h"
#include "NetCli_MailChild.h"

vector<NetInboxInfo>			NetCli_MailChild::m_inbox;
int								NetCli_MailChild::m_indoxNumItems;
int								NetCli_MailChild::m_inboxMaxNumItems;

//-----------------------------------------------------------------------------
bool NetCli_MailChild::_Init()
{
	if (This)
		return false;

	This = new NetCli_MailChild;
	ClearInboxList();
	return NetCli_Mail::_Init();
}

//-----------------------------------------------------------------------------
bool NetCli_MailChild::_Release()
{
	NetCli_Mail::_Release();
	if (This)
	{
		delete This;
		This = NULL;
	}
	return true;
}

//-----------------------------------------------------------------------------
void NetCli_MailChild::ClearInboxList()
{
	m_inbox.clear();
	m_indoxNumItems = 0;
	m_inboxMaxNumItems = 0;
}

//-----------------------------------------------------------------------------
NetInboxInfo*	NetCli_MailChild::GetInboxInfo(int index)
{
	if ( (unsigned int)index < m_inbox.size() )
		return &m_inbox[index];
	return NULL;
}

//-----------------------------------------------------------------------------
bool NetCli_MailChild::InboxItemCanDelete(int index)
{
	NetInboxInfo* inboxItem = GetInboxInfo(index);
	if ( inboxItem )
	{
		if ( inboxItem->wasReturned || inboxItem->isSystem )
			return true;

		if ( inboxItem->money == 0 && inboxItem->moneyAccount == 0 && inboxItem->IsItemEmpty() )
			return true;
	}	
	return false;
}

//-----------------------------------------------------------------------------
void NetCli_MailChild::ReadInboxItem(int index)
{	
	NetInboxInfo* inboxItem = GetInboxInfo(index);
	if ( inboxItem && !inboxItem->wasRead )
	{
		inboxItem->wasRead = true;
		if ( inboxItem->leftTime > 4319 )
			inboxItem->leftTime = 4319;

		SL_ReadMail(inboxItem->DBID);
	}
}

//-----------------------------------------------------------------------------
void NetCli_MailChild::DeleteInboxItem(int index)
{
	NetInboxInfo* inboxItem = GetInboxInfo(index);
	if ( inboxItem && InboxItemCanDelete(index) )
	{
		SL_DelMail(inboxItem->DBID);
		g_pChatFrame->SendChatMessage(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_DELETE_MAIL_SUCCESS"));
		m_inbox.erase( m_inbox.begin() + index );
		m_indoxNumItems--;
		m_inboxMaxNumItems--;
	}
}

//-----------------------------------------------------------------------------
void NetCli_MailChild::ReturnInboxItem(int index)
{
	NetInboxInfo* inboxItem = GetInboxInfo(index);
	if ( inboxItem )
	{
		SL_SendBack(inboxItem->DBID);
		m_inbox.erase( m_inbox.begin() + index );
		m_indoxNumItems--;
		m_inboxMaxNumItems--;
	}
}

//-----------------------------------------------------------------------------
void NetCli_MailChild::TakeInboxItem(int index, const char* password)
{
	NetInboxInfo* inboxItem = GetInboxInfo(index);
	if ( inboxItem )
	{
		int money = 0;
		int moneyAccount = 0;
		int bonus = 0;

		if ( inboxItem->isCOD )
		{
			if ( inboxItem->money > CNetGlobal::RoleData()->BodyMoney() )
				return;		// ª÷¿ú¤£¨¬
			else if ( inboxItem->moneyAccount > CNetGlobal::RoleData()->PlayerBaseData->Body.Money_Account )
				return;		// ÂI¼Æ¤£¨¬
			else if ( inboxItem->bonus > CNetGlobal::RoleData()->PlayerBaseData->Money_Bonus )
				return;		// ÂI¼Æ¤£¨¬
			else if ( inboxItem->IsItemEmpty() )
				return;		// ¨S¦³ª««~

			money = -inboxItem->money;
			moneyAccount = -inboxItem->moneyAccount;
			bonus = -inboxItem->bonus;
		}
		else if ( inboxItem->money > 0 || inboxItem->moneyAccount > 0 || inboxItem->bonus > 0 )
		{
			money = inboxItem->money;
			moneyAccount = inboxItem->moneyAccount;
			bonus = inboxItem->bonus;
		}
		else if ( inboxItem->IsItemEmpty() )
			return;		// ¨S¦³ª««~¤Îª÷¿ú		

		// «H¥ó¤£¬OªÅªºÀË¬d«H¥ó»Ý¨DªÅ¶¡¬O§_¨¬°÷
		if ( inboxItem->IsItemEmpty() == false )		
		{
			set<int> insertPos;			
			for ( int i = 0; i < NET_INBOX_NUM_ITEMS; i++ )
			{				
				if ( inboxItem->itemData[i].IsEmpty() == false )
				{
					bool canPush = false;

					// ·j´M¬O§_¦³¥i¥H `©ñªº¦ì¸m
					for ( int i = _MAX_BODY_BEGIN_POS_; i < _MAX_BODY_COUNT_; i++ )
					{
						set<int>::iterator iter = insertPos.find(i);
						if ( iter != insertPos.end() )
							continue;

						int flag = (i - _MAX_BODY_BEGIN_POS_) / 30;
						if ( flag == 2 && RoleData()->PlayerBaseData->TimeFlag[EM_TimeFlagEffect_BagPage3] == -1 )
							continue;
						else if ( flag == 3 && RoleData()->PlayerBaseData->TimeFlag[EM_TimeFlagEffect_BagPage4] == -1 )
							continue;
						else if ( flag == 4 && RoleData()->PlayerBaseData->TimeFlag[EM_TimeFlagEffect_BagPage5] == -1 )
							continue;
						else if ( flag == 5 && RoleData()->PlayerBaseData->TimeFlag[EM_TimeFlagEffect_BagPage6] == -1 )
							continue;

						ItemFieldStruct& item = RoleData()->PlayerBaseData->Body.Item[i];
						if ( item.IsEmpty() == true )
						{
							insertPos.insert(i);
							canPush = true;
							break;
						}							
					}

					if ( canPush == false )
					{
						// ­I¥]¤xº¡,µLªk¦¬¨ú«H¥ó
						g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_GAMEMSGEVENT_750"));
						return;
					}
				}
			}
		}

		SL_GetItem(inboxItem->DBID, money, moneyAccount, bonus,(char*)password);
	}
}

//-----------------------------------------------------------------------------
void NetCli_MailChild::RL_SendMailResult	( SendMailResultEnum Result , const char* TargetName )
{
	g_pMailFrame->SendMailUnlocked();

	if ( Result == EM_SendMailResult_OK )
	{
		g_pMailFrame->ClearSendMailItems();
		g_pMailFrame->Clear();		
		g_pMailFrame->SendWorldEvent("MAIL_SEND_SUCCESS");

		g_pChatFrame->SendChatMessageEx(CHAT_MSG_SYSTEM, "", g_ObjectData->GetString("SYS_SEND_MAIL_SUCCESS_LOG"), TargetName);
	}
	else
	{
		switch ( Result )
		{
		case EM_SendMailResult_Unknow:		
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_SENDMAIL_ERROR"));
			break;
		case EM_SendMailResult_MoneyError:
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_SENDMAIL_MONEY_ERROR"));
			break;
		case EM_SendMailResult_ItemError:
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_SENDMAIL_ITEM_ERROR"));
			break;
		case EM_SendMailResult_TargetNotFind:
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_SENDMAIL_TARGET_ERROR"));
			break;
		case EM_SendMailResult_TargetMailFull:
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_SENDMAIL_TARGET_MAILFULL"));
			break;
		case EM_SendMailResult_OwnerNameError:
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_SENDMAIL_OWNER_ERROR"));
			break;
		case EM_SendMailResult_AccountMoneyError:
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_SENDMAIL_ACCOUNTMONEY_ERROR"));
			break;
		case EM_SendMailResult_ItemLock:
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_SENDMAIL_ITEMLOCK"));
			break;
		case EM_SendMailResult_PasswordError:
			g_pGameMain->SendWarningMsg(g_ObjectData->GetString("SYS_MAIL_PASSWORD_ERROR"));
			break;
		case EM_SendMailResult_BonusMoneyError:
			break;
			/*
		case EM_SendMailResult_SendTooOften:
			break;
			*/
		}
		g_pMailFrame->SendWorldEvent("MAIL_FAILED");
	}
}
void NetCli_MailChild::RL_GetItemResult		( int MailDBID , bool Result )
{
	if ( Result )
	{
		for ( vector<NetInboxInfo>::iterator iter = m_inbox.begin(); iter != m_inbox.end(); iter++ )
		{
			NetInboxInfo& info = *iter;
			if ( info.DBID == MailDBID )
			{
				info.isCOD = false;
				info.money = 0;
				info.moneyAccount = 0;
				info.bonus = 0;

				for ( int i = 0; i < 10; i++ )
					info.itemData[i].Init();

				g_pMailFrame->InboxUpdate();
				break;
			}
		}
	}
	else
	{
		// Åª¨ú«H¥ó¿ù»~
	}
}
void NetCli_MailChild::RD_MailListResult	( int MailDBID , int Index , MailBaseInfoStruct&  MailBaseInfo , char* Content )
{	
	NetInboxInfo inboxItem;
	
	inboxItem.DBID			= MailDBID;
	if ( MailBaseInfo.IsSendBack )
	{
		char temp[512];
		inboxItem.sender	= MailBaseInfo.OrgTargetName.Begin();
		sprintf_s(temp, 512, "%s %s", g_ObjectData->GetString("SYS_SEND_BACK"), CNetCli_Script_Child::ReplaceSystemKeyword(MailBaseInfo.Title.Begin()).c_str());		
		inboxItem.subject	= temp;
	}
	else
	{
		inboxItem.sender	= CNetCli_Script_Child::ReplaceSystemKeyword(MailBaseInfo.OrgSendName.Begin()).c_str();
		inboxItem.subject	= CNetCli_Script_Child::ReplaceSystemKeyword(MailBaseInfo.Title.Begin()).c_str();
	}

	//inboxItem.subject		= CNetCli_Script_Child::ReplaceSystemKeyword(MailBaseInfo.Title.Begin()).c_str();
	inboxItem.leftTime		= MailBaseInfo.LiveTime;
	inboxItem.wasRead		= MailBaseInfo.IsRead;
	inboxItem.wasReturned	= MailBaseInfo.IsSendBack;	
	inboxItem.content		= ( Content ) ? CNetCli_Script_Child::ReplaceSystemKeyword(Content).c_str() : "";
	inboxItem.isSystem		= MailBaseInfo.IsSystem;
	inboxItem.isGift		= MailBaseInfo.IsGift;
	inboxItem.isCOD			= (MailBaseInfo.IsSendMoney) ? false : true;
	inboxItem.money			= MailBaseInfo.Money;
	inboxItem.moneyAccount	= MailBaseInfo.Money_Account;
	inboxItem.bonus			= MailBaseInfo.Money_Bonus;
	inboxItem.paperType		= MailBaseInfo.PaperType;

	for ( int i = 0; i < 10; i++ )
	{
		RegisterItemFieldChecksum(&MailBaseInfo.Item[i]);
		inboxItem.itemData[i] = MailBaseInfo.Item[i];
	}

	m_inbox.push_back(inboxItem);

	// ÀË¬d¬O§_³Ì«á¤@­Ó
	if ( m_inbox.size() == (unsigned int)m_indoxNumItems )
	{
		g_pMailFrame->InboxUpdate();
	}
}

/*
void NetCli_MailChild::RD_ReadMailResult	( int MailDBID , MailBaseInfoStruct&  MailBaseInfo , char* Content , bool Result )
{
	if ( !Result )
		return;

	for ( vector<NetInboxInfo>::iterator iter = m_inbox.begin(); iter != m_inbox.end(); iter++ )
	{
		NetInboxInfo& inboxItem = *iter;
		if ( inboxItem.DBID == MailDBID )
		{
			inboxItem.content		= Content;
			inboxItem.wasRead		= MailBaseInfo.IsRead;
			return;
		}
	}
}
*/

void NetCli_MailChild::RD_MailListCountResult	( int MaxCount , int Count )
{
	m_indoxNumItems = Count;
	m_inboxMaxNumItems = MaxCount;
}
//-----------------------------------------------------------------------------
void NetCli_MailChild::RL_OpenMail( int NpcGUID )
{
	//g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, g_ObjectData->GetString("SYS_OPEN_MAIL"), "");
	g_pMailFrame->OpenMail(NpcGUID);
}
void NetCli_MailChild::RL_CloseMail( )
{
	//g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, g_ObjectData->GetString("SYS_CLOSE_MAIL"), "");
	g_pMailFrame->SendMailUnlocked();
	g_pMailFrame->SendWorldEvent("MAIL_CLOSED");
	//g_pMailFrame->CloseMail();
}

void NetCli_MailChild::RD_UnReadMailCount( int Count , int GiftCount )
{
	char Buf[256];
	sprintf(Buf, g_ObjectData->GetString("SYS_UNREAD_MAIL"), Count, GiftCount);
	g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, Buf, "");
}
void NetCli_MailChild::RD_NewMail( bool IsGift )
{
	if ( IsGift )
		g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, g_ObjectData->GetString("SYS_GOT_GIFT"), "");
	else
		g_pChatFrame->SendMsgEvent(CHAT_MSG_SYSTEM, g_ObjectData->GetString("SYS_NEW_MAIL"), "");
}