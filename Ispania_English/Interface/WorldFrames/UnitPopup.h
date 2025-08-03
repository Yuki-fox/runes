#pragma once

#include "../interface.h"

class CUnitPopup;
extern CUnitPopup*				g_pUnitPopup;

class CUnitPopup : public CInterfaceBase
{
public:
	CUnitPopup(CInterface* object);
	virtual ~CUnitPopup();

	virtual	void				RegisterFunc();
protected:
};