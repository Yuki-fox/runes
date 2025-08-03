//#include "../GameMain.h"
#include "WorldFrame.h"
#include "Control_Utility.h"


bool										CControlInteractUI::m_IsUpdate	= false;
map< CRoleSprite* , CControlInteractUI* >	CControlInteractUI::m_mapInteractRange;

PF_EVENT_INTERRUPTED	CCastingTimeBar_Controler::m_pfInterrupted;
PF_EVENT_FINISHED		CCastingTimeBar_Controler::m_pfFinished;

//---------------------------------------------------------------------------------------
void CControlInteractUI::SetInteractRangeOn	( CControlInteractUI* pUI, CRoleSprite* pRoleObj )
{
	m_mapInteractRange[ pRoleObj ] = pUI;

	if( m_IsUpdate == false )
		CNetGlobal::Schedular()->Push( OnTimeUpdate , 300 , NULL , NULL );
}
//---------------------------------------------------------------------------------------
void CControlInteractUI::SetInteractRangeOff	( CControlInteractUI* pUI, CRoleSprite* pRoleObj )
{
	m_mapInteractRange.erase( pRoleObj );
}
//---------------------------------------------------------------------------------------
int	 CControlInteractUI::OnTimeUpdate		( SchedularInfo* Obj , void* InputClass )
{
	map< CRoleSprite* , CControlInteractUI* >::iterator it = m_mapInteractRange.begin();

	CRoleSprite*	pRole			= g_pWorldFrame->GetPlayer();
	CRoleSprite*	pInteractRole	= NULL;	// ���ʨ���
	
	float			fMaxRange		= _MAX_ROLE_TOUCH_RANGE_;

	while( it != m_mapInteractRange.end() )
	{		
		float fRange = 0;

		pInteractRole = it->first;

		// �p�� pRole �M pInteractRole �������Z��		
		fRange = (pInteractRole->GetPos() - pRole->GetPos()).Magnitude();

		if( fRange > fMaxRange )
		{
			// �����d��. �q���� UI �B�z
			CControlInteractUI* pUI = it->second;
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
		m_IsUpdate = false;
	}
	else
	{
		CNetGlobal::Schedular()->Push( OnTimeUpdate , 300 , NULL , NULL );
	}

	return 0;
}

int	CCastingTimeBar_Controler::OnTimeUpdate			()
{

	return 0;
}