#pragma once

#include "../interface.h"

class CBuffFrame;
extern CBuffFrame*			g_pBuffFrame;

class CBuffFrame : public CInterfaceBase
{
public:
	CBuffFrame(CInterface* object);
	virtual ~CBuffFrame();

	virtual	void				RegisterFunc();
protected:	
};