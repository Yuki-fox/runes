#pragma once

#include <string>
#include "../interface.h"

class CInspectFrame;
extern CInspectFrame*			g_pInspectFrame;

using namespace std;

class CInspectFrame : public CInterfaceBase
{
public:
	CInspectFrame(CInterface* object);
	virtual ~CInspectFrame();
	
	virtual	void				RegisterFunc();
	virtual	void				UpdateInspectItems();
	virtual	void				UpdateInspectIntroduce();
	virtual	void				BuildFrames();
	virtual void				DestroyFrames();

	virtual bool				InspectUnit(const char* unit);
	virtual const char*			GetInspectUnit()					{ return m_unit.c_str(); }
protected:
	string						m_unit;						// 觀看對象
};