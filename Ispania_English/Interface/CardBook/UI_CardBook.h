#pragma once

#include "../interface.h"


// Globals
//-------------------------------------------------------------------
class	CUI_CardBook;
extern	CUI_CardBook							*g_pUi_CardBook;
//-------------------------------------------------------------------

class CUI_CardBook : public CInterfaceBase
{
	
public:

	CUI_CardBook( CInterface* pObject );
	~CUI_CardBook();

	void RegisterFunc(void);
	//初使化卡片資料
	void	InitCardInfo( );
	// ----------------------------------------------------------------------------
	void	Open( );
	void	Close( );
	void	ResetCardInfo( );

public:
	vector<int>		_MonsterCard[EM_Max_Race];
	CUiModel*		_UI_CardImage;

};
