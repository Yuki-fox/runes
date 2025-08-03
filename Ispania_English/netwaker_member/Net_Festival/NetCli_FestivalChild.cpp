#include "../../mainproc/GameMain.h"
#include "NetCli_FestivalChild.h"

bool CNetCli_FestivalChild::_Init()
{
	if( m_pThis == NULL )
		m_pThis = new CNetCli_FestivalChild();

	CNetCli_Festival::_Init();

	return true;
}

bool CNetCli_FestivalChild::_Release()
{
	CNetCli_Festival::_Release();
	SAFE_DELETE( m_pThis );
	return true;
}

void CNetCli_FestivalChild::OnRZ_FestivalStatus	( int iSrvID, int iFestivalID, int iStatus,const char* pszSceneCtrl )
{
	/*
	// status
	EM_FESTIVAL_STATUS_BEGIN = 1,
	EM_FESTIVAL_STATUS_END
	*/

	switch ( iStatus )
	{
	case EM_FESTIVAL_STATUS_BEGIN:
		g_pGameMain->PushSpecialEvent(pszSceneCtrl);
		break;

	case EM_FESTIVAL_STATUS_END:
		g_pGameMain->RemoveSpecialEvent(pszSceneCtrl);
		break;
	}
}