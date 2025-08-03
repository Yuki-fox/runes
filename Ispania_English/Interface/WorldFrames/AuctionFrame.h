#pragma once

#include "../interface.h"

class CAuctionFrame;
extern CAuctionFrame*			g_pAuctionFrame;

class CAuctionFrame : public CInterfaceBase
{
public:
	CAuctionFrame(CInterface* object);
	virtual ~CAuctionFrame();

	virtual	void				RegisterFunc();
	virtual	void				Update(float elapsedTime);
	virtual	void				Pickup();
	virtual	void				PutDown(int index);
	virtual	void				OpenAC(int npcID);
	virtual	void				CloseAC();
	virtual	void				SetAuctionItemIndex(int index);
	virtual	int					GetAuctionItemIndex()					{ return m_auctionItemIndex; }
	virtual	void				SetSearchWaitTime(float time)			{ m_searchWaitTime = time; }
	virtual float				GetSearchWaitTime()						{ return m_searchWaitTime; }
	virtual void				ClearSearchWaitTime();
	virtual int					TimeTypeToMinute(int type);
	virtual void				SetSellMoneyType(int type)				{ m_sellMoneyType = type; }
	virtual int					GetSellMoneyType()						{ return m_sellMoneyType; }

protected:
	float						m_lastTime;
	float						m_searchWaitTime;						// 下次搜尋物品等待時間
	int							m_npcID;
	int							m_auctionItemIndex;	
	int							m_sellMoneyType;
};