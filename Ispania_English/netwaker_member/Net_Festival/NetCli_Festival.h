#ifndef __NETWAKER_FESTIVAL_H__
#define __NETWAKER_FESTIVAL_H__
#include "PG/PG_Festival.h"
#include "../../MainProc/NetGlobal.h"

class CNetCli_Festival : public CNetGlobal
{
protected:
	//-------------------------------------------------------------------
	static CNetCli_Festival*	m_pThis;
	static bool _Init();
	static bool _Release();
	//-------------------------------------------------------------------

	static void _PG_Festival_LtoC_FestivalStatus         		( int iSrvID , int iSize , PVOID pData );

public:    
	CNetCli_Festival(void);
	~CNetCli_Festival(void);

	virtual void	OnRZ_FestivalStatus	( int iSrvID, int iFestivalID, int iStatus,const char* pszSceneCtrl ) = 0;
};

#endif
