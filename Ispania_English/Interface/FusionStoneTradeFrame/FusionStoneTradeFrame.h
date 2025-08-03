#pragma once

#include "../interface.h"

class CFusionStoneTradeFrame : public CInterfaceBase
{
public:
	CFusionStoneTradeFrame	( CInterface* pObject );
	~CFusionStoneTradeFrame	( void );

	virtual	void			RegisterFunc					 ();

	void SetOpenInfo( int MagicStoneID, int Lv, int Money[2] );

	int GetStoneID()	{ return m_iStoneID;	}
	int GetHighestLv()	{ return m_iHighestLv;	}
	int Get1stMoney()	{ return m_iMoney[0];	}
	int Get2ndMoney()	{ return m_iMoney[1];	}

private:
	int m_iStoneID;
	int m_iHighestLv;
	int m_iMoney[2];
};

extern CFusionStoneTradeFrame* g_pFusionStoneTradeFrame;