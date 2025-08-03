
#pragma once
#include "../interface.h"
class CUI_DrawRuneFrame  :public CInterfaceBase 
{
public:
	CUI_DrawRuneFrame(CInterface* object ) ;
	virtual ~CUI_DrawRuneFrame(void);
	void RegisterFunc(void);
	

	void DrawOutRuneResult(int ss);
	 void DrawOutRuneRequest(int index);
	 bool SetDrawItemPos(int index);
	int  GetDrawItemInfo(void);
	void DrawClose(void);

	int GetItemRuneCount(void );

	int m_itemPos;
};
extern CUI_DrawRuneFrame*		g_pDrawRuneFrame;