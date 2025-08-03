#pragma once

#include <string>
#include "../interface.h"

class CLoginParent;

extern CLoginParent*				g_pLoginParent;

using namespace std;

class CLoginParent : public CInterfaceBase
{
public:
	CLoginParent(CInterface* object);
	virtual ~CLoginParent();

	virtual	void				RegisterFunc();
	virtual	void				SetCurrentScreen(const char* name)				{ m_currentScreen = name; }
	virtual	const char*			GetCurrentScreen()								{ return m_currentScreen.c_str(); }	

	virtual	void				SendLoginScreen(const char* arg);
protected:
	string						m_currentScreen;
};
