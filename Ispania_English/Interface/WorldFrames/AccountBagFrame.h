#pragma once

#include "../interface.h"
class CAccountBagFrame;
extern CAccountBagFrame*				g_pAccountBagFrame;

class CAccountBagFrame : public CInterfaceBase
{
public:
	CAccountBagFrame(CInterface* object);
	virtual ~CAccountBagFrame();	

	void				RegisterFunc();
	void				BuildFrames();
	void				DestroyFrames();
	void				Update(float elapsedTime);
protected:
};