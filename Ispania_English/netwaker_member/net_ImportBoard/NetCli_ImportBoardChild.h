#pragma once

#include "NetCli_ImportBoard.h"

class NetCli_ImportBoardChild : public NetCli_ImportBoard
{

public:
	static bool _Init();
	static bool _Release();

	static vector<ImportBoardBaseStruct> s_importBoardList;
	static float s_importBoardNewVersion;
	static float s_importBoardUpdateTime;
	static float s_importBoardLastUpdateTime;

	virtual void RD_ImportBoardInfo( float UpdateTime , int DataCount , ImportBoardBaseStruct Data[] );
	virtual void RD_ImportBoardNewVersion( float UpdateTime );
};