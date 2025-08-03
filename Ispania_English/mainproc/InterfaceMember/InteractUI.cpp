//#include "../GameMain.h"
#include "WorldFrame.h"
#include "InteractUI.h"


bool								CInteractUI::m_bOnUpdate			=	 false;
map< CRoleSprite* , CInteractUI* >	CInteractUI::m_mapInteractRange;

//---------------------------------------------------------------------------------------
void CInteractUI::SetInteractRangeOn	( CInteractUI* pUI, CRoleSprite* pRoleObj )
{
	m_mapInteractRange[ pRoleObj ] = pUI;

	if( m_bOnUpdate == false )
		CNetGlobal::Schedular()->Push( OnTimeUpdate , 300 , NULL , NULL );
}
//---------------------------------------------------------------------------------------
void CInteractUI::SetInteractRangeOff	( CInteractUI* pUI, CRoleSprite* pRoleObj )
{
	m_mapInteractRange.erase( pRoleObj );
}
//---------------------------------------------------------------------------------------
int	 CInteractUI::OnTimeUpdate		( SchedularInfo* Obj , void* InputClass )
{
	map< CRoleSprite* , CInteractUI* >::iterator it = m_mapInteractRange.begin();

	CRoleSprite*	pRole			= g_pWorldFrame->GetPlayer();
	CRoleSprite*	pInteractRole	= NULL;	// 互動角色
	
	float			fMaxRange		= _MAX_ROLE_TOUCH_RANGE_ - 10;

	while( it != m_mapInteractRange.end() )
	{		
		float fRange = 0;

		pInteractRole = it->first;

		// 計算 pRole 和 pInteractRole 之間的距離		
		fRange = (pInteractRole->GetPos() - pRole->GetPos()).Magnitude();

		if( fRange > fMaxRange )
		{
			// 脫離範圍. 通知該 UI 處理
			CInteractUI* pUI = it->second;
			pUI->OnLeave_InteractRange( pUI, pInteractRole );
			it = m_mapInteractRange.erase( it );
		}
		else
		{
			it++;
		}
	}

	if( m_mapInteractRange.empty() )
	{
		m_bOnUpdate = false;
	}
	else
	{
		CNetGlobal::Schedular()->Push( OnTimeUpdate , 300 , NULL , NULL );
	}

	return 0;
}