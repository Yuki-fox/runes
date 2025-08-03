#pragma once

#include "../interface.h"

#define TRADE_ITEM_MAX_SIZE			8

class CTradeFrame;
extern CTradeFrame*				g_pTradeFrame;

// 交易物品來源類型
enum TradeItemType {
	eTradeItemType_None			= 0,
	eTradeItemType_Bag,
	eTradeItemType_Bank,
	eTradeItemType_Equipment,
};

// 交易狀態
enum TradeState {
	eTradeState_None			= 0,
	eTradeState_Request_Made,				// 初步要跟對方交易
	eTradeState_Request_Receive,			// 被要求與對方交易,出現訊問題示
	eTradeState_Agree,						// 同意與對方交易
	eTradeState_Disagree,					// 不同意與對方交易
	eTradeState_Traded,						// 交易過程中
};

enum TradeAcceptState {
	eTradeAcceptState_None		= 0,
	eTradeAcceptState_Ready		= 1,
	eTradeAcceptState_Trade		= 2,
	//eTradeAcceptState_Player	= 1,		// 玩家鎖定
	//eTradeAcceptState_Target	= 2,		// 對方鎖定
};

struct TradeItem {
	int type;								// 來源
	int pos;								// 位置
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
	int							m_moneyMode;					// 0遊戲幣 1帳號幣
	int							m_money;
	int							m_targetMoneyMode;				// 0遊戲幣 1帳號幣
	int							m_targetMoney;
	float						m_time;
	TradeItem					m_items[TRADE_ITEM_MAX_SIZE];
	string						m_targetName;					// 避免角色消失,先記錄角色名稱
};