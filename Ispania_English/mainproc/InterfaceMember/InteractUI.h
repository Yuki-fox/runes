#pragma once

#include "../GameMain.h"

#include <map>

class CInteractUI
{
public:
	CInteractUI(void){};
	~CInteractUI(void){};

	static	void	SetInteractRangeOn		( CInteractUI* pUI, CRoleSprite* pRoleObj );
	static	void	SetInteractRangeOff		( CInteractUI* pUI, CRoleSprite* pRoleObj );

	static	int		OnTimeUpdate			( SchedularInfo* Obj , void* InputClass );	

	virtual	void	OnLeave_InteractRange	( CInteractUI* pUI, CRoleSprite* pRoleObj ) = 0;

	static map< CRoleSprite* , CInteractUI* >	m_mapInteractRange;
	static bool									m_bOnUpdate;

};
