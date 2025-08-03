#pragma once

#include "../interface.h"

#define TRADE_ITEM_MAX_SIZE			8

class CTradeFrame;
extern CTradeFrame*				g_pTradeFrame;

// ������~�ӷ�����
enum TradeItemType {
	eTradeItemType_None			= 0,
	eTradeItemType_Bag,
	eTradeItemType_Bank,
	eTradeItemType_Equipment,
};

// ������A
enum TradeState {
	eTradeState_None			= 0,
	eTradeState_Request_Made,				// ��B�n������
	eTradeState_Request_Receive,			// �Q�n�D�P�����,�X�{�T���D��
	eTradeState_Agree,						// �P�N�P�����
	eTradeState_Disagree,					// ���P�N�P�����
	eTradeState_Traded,						// ����L�{��
};

enum TradeAcceptState {
	eTradeAcceptState_None		= 0,
	eTradeAcceptState_Ready		= 1,
	eTradeAcceptState_Trade		= 2,
	//eTradeAcceptState_Player	= 1,		// ���a��w
	//eTradeAcceptState_Target	= 2,		// �����w
};

struct TradeItem {
	int type;								// �ӷ�
	int pos;								// ��m
	int id;
};

class CTradeFrame : public CInterfaceBase
{
public:
	CTradeFrame(CInterface* object);
	virtual ~CTradeFrame();

	virtual	void				RegisterFunc();
	virtual	void				Update(float elapsedTime);
	virtual	int					SetProcess(TradeState state, int target);
	virtual void				TradeClose();
	virtual	void				AcceptTrade(const char* password);
	virtual	void				CancelTradeAccept();
	virtual	void				TradeAcceptState(int player, int target);
	virtual	TradeItem*			GetItem(int index);
	virtual	void				Cut(int index);
	virtual	void				Paste(int index);
	virtual	void				Refresh();
	virtual	int					GetTradeTarget()						{ return m_target; }
	virtual	TradeState			GetTradeState()							{ return m_tradeState; }
	virtual	void				SetTradeMoney(int money, int money_Account );
	virtual	int					GetTradeMoneyMode()						{ return m_moneyMode; }
	virtual	int					GetTradeMoney()							{ return m_money; }
	virtual	void				SetTradeTargetMoney(int money , int money_Account );
	virtual	int					GetTradeTargetMoneyMode()				{ return m_targetMoneyMode; }
	virtual	int					GetTradeTargetMoney()					{ return m_targetMoney; }

protected:
	virtual	void				PutItem(int type, int index, int pos);

protected:
	TradeState					m_tradeState;
	int							m_playerAcceptState;
	int							m_targetAcceptState;
	int							m_target;
	int							m_moneyMode;					// 0�C���� 1�b����
	int							m_money;
	int							m_targetMoneyMode;				// 0�C���� 1�b����
	int							m_targetMoney;
	float						m_time;
	TradeItem					m_items[TRADE_ITEM_MAX_SIZE];
	string						m_targetName;					// �קK�������,���O������W��
};