#include "NetCli_TeleportChild.h"
#include "../Interface/transportbook/TeleportBook.h"

//-----------------------------------------------------------------------------
bool NetCli_TeleportChild::_Init()
{
	if (This)
		return false;

	This = new NetCli_TeleportChild;

	return NetCli_Teleport::_Init();
}

//-----------------------------------------------------------------------------
bool NetCli_TeleportChild::_Release()
{
	NetCli_Teleport::_Release();
	if (This)
	{
		delete This;
		This = NULL;
	}
	return true;
}
void NetCli_TeleportChild::R_AddTeleportResult	( int TeleportPos , LocationSaveStruct& Info , bool Result )
{


	if (g_pTeleportBook)
	g_pTeleportBook->AddTeleportResult(TeleportPos,Info,Result);


}
void NetCli_TeleportChild::R_DelTeleportResult	( int TeleportPos , bool Result )
{
	if (g_pTeleportBook)
		g_pTeleportBook->DelTeleportResult(TeleportPos,Result);
}
void NetCli_TeleportChild::R_SwapTeleportResult	( int TeleportPos[2] , bool Result )
{
	if (g_pTeleportBook)
		g_pTeleportBook->SwapTeleportResult(TeleportPos,Result);
}
void NetCli_TeleportChild::R_ModifyTeleportResult	( int TeleportPos , LocationSaveStruct& Info , bool Result )
{
	if (g_pTeleportBook)
		g_pTeleportBook->ModifyTeleportResult(TeleportPos,Info,Result);

}
void NetCli_TeleportChild::R_UseTeleportResult	( UseTeleportTypeENUM Type , int ItemPos , int TeleportPos , bool Result )
{
	if (g_pTeleportBook)
		g_pTeleportBook->UseTeleportResult(Type,ItemPos,TeleportPos,Result);
}
