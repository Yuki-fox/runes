#pragma once

#include "../interface.h"

class CExchangeClassFrame;
extern CExchangeClassFrame*			g_pExchangeClassFrame;

class CExchangeClassFrame : public CInterfaceBase
{
public:
	CExchangeClassFrame( CInterface* object);
	virtual ~CExchangeClassFrame();

	void				RegisterFunc();
	void				Update(float elapsedTime);
	void				Open(int npc);
	void				Close();
	void				ExchangeClass(int mainClass, int subClass);
	int					GetExchangeMainClass()					{ return m_changeJob[0]; }
	int					GetExchangeSubClass()					{ return m_changeJob[1]; }
	void				Locked(bool locked)						{ m_locked = locked; memset(m_changeJob, 0, sizeof(int)* 2); }

protected:
	int							m_npc;
	int							m_changeJob[2];
	bool						m_locked;
};