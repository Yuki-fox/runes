#include "..\GameMain.h"
#include "InterfaceBase.h"

// **********************************************************************************************
// Globals

CInterfaceBase*						g_pInterfaceBase = NULL;

// **********************************************************************************************
void CInterfaceBase::Initial()
{
	if ( g_pInterfaceBase == NULL )
	{
		g_pInterfaceBase = new CInterfaceBase;
	}
}

void CInterfaceBase::Release()
{
}

CInterfaceBase::CInterfaceBase()
{
	m_funcHandle.clear();
}

CInterfaceBase::~CInterfaceBase()
{
	m_funcHandle.clear();
}