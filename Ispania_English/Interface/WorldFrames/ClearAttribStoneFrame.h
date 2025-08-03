
#pragma once
#include "../interface.h"
class CUI_ClearAttribStoneFrame  :public CInterfaceBase 
{
public:
	CUI_ClearAttribStoneFrame(CInterface* object ) ;
	virtual ~CUI_ClearAttribStoneFrame(void);
	void RegisterFunc(void);
	

	void ClearAttribStoneResult(bool ss);
	 void ClearAttribStoneRequest(int index);
	 bool SetClearAttribPos(int index);
	int  GetClearItemInfo(void);
	void ClearClose(void);

	int GetAttribStoneCount(void );

	int m_itemPos;
};
extern CUI_ClearAttribStoneFrame*		g_pClearAttribStoneFrame;