#pragma once

#include "../interface.h"

class CBulletinBoard;
extern CBulletinBoard*			g_pBulletinBoard;

class CBulletinBoard : public CInterfaceBase
{
public:
	CBulletinBoard(CInterface* object);
	virtual ~CBulletinBoard();	
	
	virtual	void				RegisterFunc();
	virtual void				DestroyFrames();
	void						Save();
	void						Load();
protected:
};