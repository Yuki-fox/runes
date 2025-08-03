#pragma once

#include "../interface.h"
#include <string>
#include <map>

using namespace std;

class CClientCountDownBarFrame;
extern CClientCountDownBarFrame*		g_pClientCountDownBarFrame;

class CClientCountDownBarFrame : public CInterfaceBase
{
public:
	CClientCountDownBarFrame(CInterface* object);
	virtual ~CClientCountDownBarFrame();

	virtual	void				RegisterFunc();
	virtual	void				CountDownStart(const char* text, float time, float nTinme, int iMode);
	virtual	void				CountDownStop();
};