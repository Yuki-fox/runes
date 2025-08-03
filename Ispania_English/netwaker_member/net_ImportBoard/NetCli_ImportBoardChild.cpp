#include "NetCli_ImportBoardChild.h"
#include "../../mainproc/GameMain.h"
#include "../../Interface/WorldFrames/BulletinBoard.h"
#include "smallobj/SmallObj.h"

vector<ImportBoardBaseStruct>	NetCli_ImportBoardChild::s_importBoardList;
float							NetCli_ImportBoardChild::s_importBoardUpdateTime = 0.0f;
float							NetCli_ImportBoardChild::s_importBoardNewVersion = 0.0f;
float							NetCli_ImportBoardChild::s_importBoardLastUpdateTime = 0.0f;

//-----------------------------------------------------------------------------
bool NetCli_ImportBoardChild::_Init()
{
	if (This)
		return false;
	This = new NetCli_ImportBoardChild;

	return NetCli_ImportBoard::_Init();
}

//-----------------------------------------------------------------------------
bool NetCli_ImportBoardChild::_Release()
{
	NetCli_ImportBoard::_Release();
	if (This)
	{
		delete This;
		This = NULL;
	}
	return true;
}


void NetCli_ImportBoardChild::RD_ImportBoardInfo( float UpdateTime , int DataCount , ImportBoardBaseStruct Data[] )
{
	s_importBoardUpdateTime = UpdateTime;
	s_importBoardLastUpdateTime = UpdateTime;

	if ( DataCount > 0 ) {
		s_importBoardList.clear();

		for ( int i = 0; i < DataCount; i++ ) {
			s_importBoardList.push_back(Data[i]);
		}
	}

	g_pInterface->SendWorldEvent("BULLETINBOARD_UPDATE");
}
void NetCli_ImportBoardChild::RD_ImportBoardNewVersion( float UpdateTime )
{
	s_importBoardNewVersion = UpdateTime;

	/*
	// �q��Ĳ�o�L
	if ( s_importBoardUpdateTime == 0.0f ) {
		// ���a���n�D�}�Ҭɭ�,���ݭn���s�n�D
	} else if ( s_importBoardNeedRequest && s_importBoardNewVersion != s_importBoardUpdateTime ) {
		// �n�D���s���o��T
		SL_ImportBoardRequest(s_importBoardUpdateTime);
	}
	*/
}