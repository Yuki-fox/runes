#pragma once

#include "NetCli_Festival.h"

class CNetCli_FestivalChild : public CNetCli_Festival
{
public:
	static	bool	_Init	();
	static	bool	_Release();

public:
	virtual void	OnRZ_FestivalStatus	( int iSrvID, int iFestivalID, int iStatus,const char* pszSceneCtrl );
};