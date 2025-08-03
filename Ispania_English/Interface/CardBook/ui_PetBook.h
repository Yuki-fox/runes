#pragma once

#include "../interface.h"


// Globals
//-------------------------------------------------------------------
class	CUI_PetBook;
extern	CUI_PetBook							*g_pUi_PetBook;
//-------------------------------------------------------------------

class CUI_PetBook : public CInterfaceBase
{

public:

	CUI_PetBook( CInterface* pObject );
	~CUI_PetBook();

	void RegisterFunc(void);
	//初使化卡片資料
	void	InitPetInfo( );
	// ----------------------------------------------------------------------------
	void	Open( );
	void	Close( );
	void	ResetPetInfo( );

public:
	//vector<int>		_pets;[EM_Max_Race];
	CUiModel*		_UI_PetImage;

};
