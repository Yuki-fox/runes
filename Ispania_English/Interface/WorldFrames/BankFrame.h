#pragma once

#include "../interface.h"

class CBankFrame;
extern CBankFrame*			g_pBankFrame;

class CBankFrame : public CInterfaceBase
{
public:
	CBankFrame(CInterface* object);
	virtual ~CBankFrame();

	enum {
		eBank_Type_Bag				= 0,
		eBank_Type_Bank,
		eBank_Type_Equipment,
	};

	virtual	void				RegisterFunc();
	virtual	void				Update(float elapsedTime);
	virtual	void				Open(int bankManage);
	virtual	void				Close();
	virtual	bool				IsOpen()						{ return m_bankManage != -1; }
	virtual	bool				Cut(int index);					// °Å¤U
	virtual	void				Paste(int index);				// ¶K¤W

	virtual	bool				Deposit(int index, int srcType, int pos, bool toBag=false, int clientID=0);
	virtual	void				Draw(int index);
	virtual void				SortItem();
	virtual void				RL_SortBankEnd();
protected:
	float						m_sortLastTime;
	int							m_bankManage;
	int							m_pickupItemIndex;
};