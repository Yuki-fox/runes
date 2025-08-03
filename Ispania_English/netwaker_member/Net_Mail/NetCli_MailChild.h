#pragma once

#include <vector>
#include <string>
#include "NetCli_Mail.h"

using namespace std;

#define NET_INBOX_NUM_ITEMS							10

struct NetInboxInfo
{
	string			sender;
	string			subject;
	string			content;
	int				DBID;	
	int				leftTime;
	int				money;
	int				moneyAccount;
	int				bonus;
	bool			isCOD;
	bool			isSystem;
	bool			isGift;
	bool			wasRead;
	bool			wasReturned;
	bool			canReply;	
	ItemFieldStruct	itemData[NET_INBOX_NUM_ITEMS];
	MailBackGroundTypeENUM paperType;

	int GetNumItems()
	{
		int count = 0;
		for ( int i = 0; i < NET_INBOX_NUM_ITEMS; i++ )
		{
			if ( itemData[i].IsEmpty() == false )
				count++;
		}
		return count;
	}

	int GetFirstItemIndex()
	{
		for ( int i = 0; i < NET_INBOX_NUM_ITEMS; i++ )
		{
			if ( itemData[i].IsEmpty() == false )
				return i;
		}
		return -1;
	}

	bool IsItemEmpty()
	{
		return GetFirstItemIndex() == -1;
	}
};

class NetCli_MailChild : public NetCli_Mail
{
public:
	static bool _Init();
	static bool _Release();

public:
	static	void				ClearInboxList		();
	static	int					GetInboxNumItems	()						{ return m_indoxNumItems; }
	static	int					GetInboxMaxNumItems	()						{ return m_inboxMaxNumItems; }
	static	NetInboxInfo*		GetInboxInfo		(int index);
	static	bool				InboxItemCanDelete	(int index);
	static	void				ReadInboxItem		(int index);
	static	void				DeleteInboxItem		(int index);
	static	void				ReturnInboxItem		(int index);
	static	void				TakeInboxItem		(int index, const char* password);

	virtual void RL_SendMailResult	( SendMailResultEnum Result , const char* TargetName );
	virtual void RL_GetItemResult	( int MailDBID , bool Result );
	virtual void RD_MailListResult	( int MailDBID , int Index , MailBaseInfoStruct&  MailBaseInfo , char* Content );
	virtual void RD_MailListCountResult	( int MaxCount , int Count );
	virtual	void RL_OpenMail( int NpcGUID );
	virtual	void RL_CloseMail( );
	virtual void RD_UnReadMailCount( int Count , int GiftCount );
	virtual void RD_NewMail( bool IsGift );

	static	vector<NetInboxInfo>	m_inbox;
	static	int						m_indoxNumItems;
	static	int						m_inboxMaxNumItems;
};

