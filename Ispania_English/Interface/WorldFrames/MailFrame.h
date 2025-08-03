#pragma once

#include <vector>
#include "../interface.h"

using namespace std;

#define MAX_SEND_MAIL_ITEMS					10

class CMailFrame;
extern CMailFrame*			g_pMailFrame;

enum SendMailGroupType
{
	eSendMailGroup_Friend		= 0,	// 好友
	eSendMailGroup_Guild,				// 公會
};

struct SendMailGroupInfo
{
	SendMailGroupType type;				// 類型
	string name;						// 群組名稱
	vector<string> list;
};

class CMailFrame : public CInterfaceBase
{
public:
	CMailFrame(CInterface* object);
	virtual ~CMailFrame();

	virtual	void				RegisterFunc();
	virtual	void				Update(float elapsedTime);
	virtual	void				OpenMail(int NPCID);
	virtual	void				CloseMail();
	virtual	void				InboxUpdate();
	virtual	void				Clear();
	virtual	void				SetMoney(bool isCOD, int money, int runeMoney);
	virtual void				SetPaperType(int paperType);	
	virtual int					GetPaperType()								{ return m_paperType; }
	virtual	void				Pickup(int index);
	virtual	void				PutDown(int bagIndex);
	virtual	void				SendMail(const char* target, const char* subject, const char* body, const char* password);
	virtual void				SendGroupMail(int group, const char* subject, const char* body);
	virtual	int					GetSendMailItemIndex()						{ return m_sendMailItemIndex; }
	virtual	void				SendMailUnlocked()							{ m_sendMailLocked = false; }

	virtual void				ClearSendMailItems();
	virtual int					GetSendMailItem(int index);
	virtual vector<SendMailGroupInfo>&		GetSendMailGroup()				{ return m_sendMailGroup; }
	
protected:
	virtual	void				SetSendMailItemIndex(int index);
	virtual void				SetSendMailItem(int index, int bagPos);
	virtual void				UpdateSendMailGroup();

protected:
	bool						m_inboxOpenLocked;
	bool						m_sendMailLocked;
	bool						m_isCOD;
	float						m_sendGruopLastTime;
	int							m_money;
	int							m_moneyAccount;
	int							m_sendMailItemIndex;
	int							m_mailNPCID;
	int							m_paperType;
	int							m_sendMailItem[MAX_SEND_MAIL_ITEMS];
	vector<SendMailGroupInfo>	m_sendMailGroup;
};