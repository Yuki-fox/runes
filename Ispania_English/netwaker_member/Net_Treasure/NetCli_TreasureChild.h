#pragma once

#include "NetCli_Treasure.h"

class NetCli_TreasureChild : public NetCli_Treasure
{
public:	
	static	bool _Init();
	static	bool _Release();

public:

	virtual void R_BaseInfo( int ItemGUID , bool IsEmpty );
	virtual void R_OpenFailed( int ItemGUID );
	virtual void R_Open( int ItemGUID , int MaxCount , ItemFieldStruct* Items );
	virtual void R_GetItemResult( int BodyPos , int ItemSerial , TreasureGetItemResultENUM Result );

};

