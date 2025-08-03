#ifndef _INTERFACE_BASE_H
#define _INTERFACE_BASE_H

#include <map>

// **********************************************************************************************
// Globals
class	CInterfaceBase;

extern	CInterfaceBase						*g_pInterfaceBase;

// **********************************************************************************************

class CInterfaceBase
{
public:
	CInterfaceBase();
	virtual ~CInterfaceBase();

	static	void			Initial();
	static	void			Release();
	//void					Update( float elapsedTime );

protected:
	std::map<int, int>		m_funcHandle;
};

#endif _INTERFACE_BASE_H

