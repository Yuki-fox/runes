#pragma once 
#include "../interface.h"
class CUI_scoreBoard  :public CInterfaceBase 
{
public:
	CUI_scoreBoard(CInterface* object ) ;
	~CUI_scoreBoard(void);


	void SetSmallGameMenuType		( int MenuID , int Type  );


	void SetSmallGameMenuValue	( int MenuID , int ID , int Value );
	void SetSmallGameMenuStr	( int MenuID , int ID , const char* Content  );

	void RegisterFunc(void);

//
private:
	
};

extern CUI_scoreBoard*		g_pScoreBoardFrame;